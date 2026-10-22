/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019, 2020, 2021 Giuseppe Scrivano <giuseppe@scrivano.org>
 * crun is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * crun is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with crun.  If not, see <http://www.gnu.org/licenses/>.
 */
#define _GNU_SOURCE

#include <config.h>
#include "../custom-handler.h"
#include "../container.h"
#include "../utils.h"
#include "../linux.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/mount.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/sysmacros.h>
#include <fcntl.h>
#include <sched.h>
#include <ocispec/runtime_spec_schema_config_schema.h>

#ifdef HAVE_DLOPEN
#  include <dlfcn.h>
#endif

#ifdef HAVE_LIBKRUN
#  include <libkrun.h>
#endif

/* This allows us to build even with headers from older libkrun versions.
 * If the installed version of libkrun doesn't support this feature,
 * krun_add_net_unixstream() will return EINVAL.
 */
#ifndef NET_FLAG_DHCP_CLIENT
#  define NET_FLAG_DHCP_CLIENT (1 << 1)
#endif
/* libkrun has a hard-limit of 16 vCPUs per microVM. */
#define LIBKRUN_MAX_VCPUS 16

/* If the user doesn't configure the RAM amount, fallback to this value. */
#define LIBKRUN_DEFAULT_RAM_MIB 1024

/* The minimum amount of RAM for a viable microVM is 128 MB. */
#define LIBKRUN_MINIMUM_RAM_MIB 128

/* crun dumps the container configuration into this file, which will be read by
 * libkrun to set up the environment for the workload inside the microVM.
 */
#define KRUN_CONFIG_FILE ".krun_config.json"

/* The presence of this file indicates this is a container intended to be run
 * as a confidential workload inside a SEV-powered TEE.
 */
#define KRUN_SEV_FILE "/krun-sev.json"

/* This file contains configuration parameters for the microVM. crun needs to
 * read and parse it, using the information obtained from it to configure
 * libkrun as required.
 */
#define KRUN_VM_FILE "/.krun_vm.json"

/* KRUN_VM_FILE holds a handful of settings and it is parsed by crun
 * itself before the container process exists, refuse anything bigger.
 */
#define KRUN_VM_FILE_MAX_SIZE (64 * 1024)

#define KRUN_FLAVOR_AWS_NITRO "aws-nitro"
#define KRUN_FLAVOR_SEV "sev"

#define FD_PAIR_PARENT 0
#define FD_PAIR_CHILD 1

struct krun_config
{
  void *handle;
  void *handle_sev;
  void *handle_awsnitro;
  bool sev;
  bool awsnitro;
  int32_t ctx_id;
  int32_t ctx_id_sev;
  int32_t ctx_id_awsnitro;
  bool has_kvm;
  bool has_awsnitro;
  ціле passt_fds[2];
  json_object *config_doc;
  json_object *config_tree;
  bool use_passt;
  const char *tap_name;
  ціле gpu_render_server_fd;
  ціле gpu_flags;
};

/* libkrun handler.  */
#якщо HAVE_DLOPEN && HAVE_LIBKRUN
static int32_t
libkrun_create_context (void *handle, libcrun_error_t *err)
{
  int32_t (*krun_create_ctx) ();
  int32_t ctx_id;

  krun_create_ctx = dlsym (handle, "krun_create_ctx");
  якщо (krun_create_ctx == NULL)
    повернути crun_make_error (err, 0, "could not find symbol in the krun library");

  ctx_id = krun_create_ctx ();
  якщо (UNLIKELY (ctx_id < 0))
    повернути crun_make_error (err, -ctx_id, "could not create krun context");

  повернути ctx_id;
}

static ціле
libkrun_configure_kernel (uint32_t ctx_id, void *handle, json_object *config_tree, libcrun_error_t *err)
{
  int32_t (*krun_set_kernel) (uint32_t ctx_id, const char *kernel_path,
                              uint32_t kernel_format, const char *initrd_path, const char *kernel_cmdline);
  json_object *kernel_path = NULL;
  json_object *kernel_format = NULL;
  json_object *val_initrd_path = NULL;
  json_object *val_kernel_cmdline = NULL;
  const char *initrd_path = NULL;
  const char *kernel_cmdline = NULL;
  ціле ret;

  /* kernel_path and kernel_format must be present */
  kernel_path = json_object_object_get (config_tree, "kernel_path");
  якщо (kernel_path == NULL || ! json_object_is_type (kernel_path, json_type_string))
    повернути 0;

  kernel_format = json_object_object_get (config_tree, "kernel_format");
  якщо (kernel_format == NULL || ! json_object_is_type (kernel_format, json_type_int))
    повернути 0;

  /* initrd and kernel_cmdline are optional */
  val_initrd_path = json_object_object_get (config_tree, "initrd_path");
  якщо (val_initrd_path != NULL && json_object_is_type (val_initrd_path, json_type_string))
    initrd_path = json_object_get_string (val_initrd_path);

  val_kernel_cmdline = json_object_object_get (config_tree, "kernel_cmdline");
  якщо (val_kernel_cmdline != NULL && json_object_is_type (val_kernel_cmdline, json_type_string))
    kernel_cmdline = json_object_get_string (val_kernel_cmdline);

  krun_set_kernel = dlsym (handle, "krun_set_kernel");
  якщо (krun_set_kernel == NULL)
    повернути crun_make_error (err, 0, "could not find symbol in krun library");

  ret = krun_set_kernel (ctx_id,
                         json_object_get_string (kernel_path),
                         (uint32_t) json_object_get_int64 (kernel_format),
                         initrd_path, kernel_cmdline);

  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, -ret, "could not configure a krun external kernel");

  повернути 0;
}

static ціле
libkrun_read_vm_config (struct krun_config *kconf, ціле rootfsfd, const char *rootfs, libcrun_error_t *err)
{
  ціле ret;
  cleanup_free char *config = NULL;
  cleanup_close ціле pathfd = -1;
  cleanup_close ціле fd = -1;
  proc_fd_path_t fd_path;
  struct stat st;

  /* The content of the rootfs is controlled by the user: look at the file through an
     O_PATH descriptor first and refuse anything that is not a small regular file, as
     opening a FIFO blocks and a device node must not be accessed.  */
  pathfd = safe_openat (rootfsfd, rootfs, KRUN_VM_FILE, O_PATH | O_CLOEXEC | O_NOFOLLOW, 0, err);
  якщо (pathfd < 0)
    {
      // The configuration file is optional, don't generate an error if it's missing.
      якщо (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          повернути 0;
        }
      повернути pathfd;
    }

  ret = fstat (pathfd, &st);
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "stat `%s`", KRUN_VM_FILE);

  якщо (! S_ISREG (st.st_mode))
    повернути crun_make_error (err, 0, "`%s` is not a regular file", KRUN_VM_FILE);

  get_proc_self_fd_path (fd_path, pathfd);

  fd = open (fd_path, O_RDONLY | O_CLOEXEC);
  якщо (UNLIKELY (fd < 0))
    повернути crun_make_error (err, errno, "open `%s`", KRUN_VM_FILE);

  ret = read_all_fd_with_max_size (fd, KRUN_VM_FILE, &config, NULL, KRUN_VM_FILE_MAX_SIZE, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = parse_json_file (&kconf->config_doc, config, NULL, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  kconf->config_tree = kconf->config_doc;
  повернути 0;
}

/*
 * Default to parsing the OCI annotations to find the specified microVM
 * attribute. If the desired attribute is not found within the OCI annotations
 * or the krun_vm.json file, then return a negative integer.
 *
 * The configuration precedence is as follows:
 * OCI annotations -> krun_vm.json.
 * The config file is only used if "use_config_file" is "true".
 */
static ціле
libkrun_parse_resource_configuration (json_object *config_tree, libcrun_container_t *container, const char *annotation, const char *key, bool use_config_file)
{
  char *val_str, *endptr;
  ціле val = -1;
  json_object *val_json = NULL;

  val_str = (char *) find_annotation (container, annotation);
  якщо (val_str != NULL)
    {
      errno = 0;
      val = (ціле) strtol (val_str, &endptr, 10);
      якщо (errno != 0 || endptr == val_str || *endptr != '\0')
        /* Annotations value is not a valid integer. */
        error (EXIT_FAILURE, 0, "krun annotation %s value cannot be converted to an integer", annotation);

      якщо (val < 0)
        error (EXIT_FAILURE, 0, "krun annotation %s value must be a positive integer", annotation);

      повернути val;
    }
  інакше якщо (use_config_file && config_tree != NULL)
    {
      val_json = json_object_object_get (config_tree, key);
      якщо (val_json == NULL)
        повернути val;
      якщо (! json_object_is_type (val_json, json_type_int))
        error (EXIT_FAILURE, 0, "krun krun_vm.json %s value is not an integer", key);

      val = (ціле) json_object_get_int64 (val_json);
    }

  повернути val;
}

static ціле
libkrun_parse_string_configuration (json_object *config_tree, libcrun_container_t *container,
                                    const char *annotation, const char *key,
                                    const char **value, libcrun_error_t *err, bool use_config_file)
{
  const char *val;
  json_object *val_json = NULL;

  *value = NULL;

  val = find_annotation (container, annotation);
  якщо (val != NULL)
    {
      *value = val;
      повернути 0;
    }

  якщо (! use_config_file || config_tree == NULL)
    повернути 0;

  val_json = json_object_object_get (config_tree, key);
  якщо (val_json == NULL)
    повернути 0;

  якщо (! json_object_is_type (val_json, json_type_string))
    повернути crun_make_error (err, 0, ".krun_vm.json %s value is not a string", key);

  *value = json_object_get_string (val_json);
  повернути 0;
}

static void
libkrun_make_tap_mac (const char *id, uint8_t mac[6])
{
  uint64_t hash = 14695981039346656037ULL;
  size_t i;

  для (; *id != '\0'; id++)
    {
      hash ^= (uint8_t) *id;
      hash *= 1099511628211ULL;
    }

  для (i = 0; i < 6; i++)
    mac[i] = (uint8_t) (hash >> (i * 8));

  mac[0] = (mac[0] & 0xfe) | 0x02;
}

static ціле
libkrun_configure_vm (uint32_t ctx_id, void *handle, struct krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_set_vm_config) (uint32_t ctx_id, uint8_t num_vcpus, uint32_t ram_mib);
  int32_t (*krun_add_net_unixstream) (uint32_t ctx_id, const char *c_path, ціле fd, uint8_t *const c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_add_net_tap) (uint32_t ctx_id, const char *c_tap_name, const uint8_t *c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_set_gpu_options) (uint32_t ctx_id, uint32_t virgl_flags);
  int32_t (*krun_set_gpu_render_server_fd) (uint32_t ctx_id, ціле fd);
  ціле cpus, ram_mib, nested_virt, ret;
  cpu_set_t set;

  /* We let the OCI set the number of vCPUs for the VM, since cgroups CPU restrictions still apply. */
  cpus = libkrun_parse_resource_configuration (kconf->config_tree, container, "krun.cpus", "cpus", true);
  якщо (cpus <= 0)
    {
      CPU_ZERO (&set);
      якщо (sched_getaffinity (getpid (), sizeof (set), &set) == 0)
        cpus = MIN (CPU_COUNT (&set), LIBKRUN_MAX_VCPUS);
      інакше
        cpus = 1;
    }

  /* We let the OCI set the amount of RAM for the VM, since cgroups memory restrictions still apply. */
  ram_mib = libkrun_parse_resource_configuration (kconf->config_tree, container, "krun.ram_mib", "ram_mib", true);
  якщо (ram_mib <= LIBKRUN_MINIMUM_RAM_MIB)
    {
      якщо (def && def->linux && def->linux->resources && def->linux->resources->memory
          && def->linux->resources->memory->limit_present)
        ram_mib = def->linux->resources->memory->limit / (1024 * 1024);
      інакше
        ram_mib = LIBKRUN_DEFAULT_RAM_MIB;
    }

  krun_set_vm_config = dlsym (handle, "krun_set_vm_config");

  якщо (krun_set_vm_config == NULL)
    повернути crun_make_error (err, 0, "could not find symbol in the krun library");

  ret = krun_set_vm_config (ctx_id, cpus, ram_mib);
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, -ret, "could not set krun vm configuration");

  nested_virt = libkrun_parse_resource_configuration (kconf->config_tree, container, "krun.nested_virt", "nested_virt", false);
  якщо (nested_virt > 0)
    {
      int32_t (*krun_check_nested_virt) (void);
      int32_t (*krun_set_nested_virt) (uint32_t ctx_id, bool enabled);

      krun_check_nested_virt = dlsym (handle, "krun_check_nested_virt");
      якщо (krun_check_nested_virt != NULL && krun_check_nested_virt () != 1)
        libcrun_warning ("nested virtualization requested but may not be supported on this host");

      krun_set_nested_virt = dlsym (handle, "krun_set_nested_virt");
      якщо (krun_set_nested_virt == NULL)
        повернути crun_make_error (err, 0, "could not find symbol `krun_set_nested_virt` in the krun library");

      ret = krun_set_nested_virt (ctx_id, true);
      якщо (UNLIKELY (ret < 0))
        повернути crun_make_error (err, -ret, "could not enable nested virtualization");
    }

  якщо (kconf->tap_name != NULL)
    {
      krun_add_net_tap = dlsym (handle, "krun_add_net_tap");
      якщо (krun_add_net_tap == NULL)
        повернути crun_make_error (err, 0, "could not find symbol `krun_add_net_tap` in the krun library");

      uint8_t mac[6];
      libkrun_make_tap_mac (container->context->id, mac);
      ret = krun_add_net_tap (ctx_id, kconf->tap_name, &mac[0], COMPAT_NET_FEATURES, 0);
      якщо (UNLIKELY (ret < 0))
        повернути crun_make_error (err, -ret, "could not add krun TAP interface `%s`", kconf->tap_name);
    }
  інакше якщо (kconf->use_passt)
    {
      krun_add_net_unixstream = dlsym (handle, "krun_add_net_unixstream");
      якщо (krun_add_net_unixstream == NULL)
        повернути crun_make_error (err, 0, "could not find symbol `krun_add_net_unixstream` in the krun library");

      uint8_t mac[] = { 0x5a, 0x94, 0xef, 0xe4, 0x0c, 0xee };
      ret = krun_add_net_unixstream (ctx_id, NULL, kconf->passt_fds[FD_PAIR_PARENT], &mac[0], COMPAT_NET_FEATURES, NET_FLAG_DHCP_CLIENT);
      якщо (UNLIKELY (ret == -EINVAL))
        ret = krun_add_net_unixstream (ctx_id, NULL, kconf->passt_fds[FD_PAIR_PARENT], &mac[0], COMPAT_NET_FEATURES, 0);
      якщо (UNLIKELY (ret < 0))
        повернути crun_make_error (err, -ret, "could not set krun net configuration");
    }

  якщо (kconf->gpu_flags > 0)
    {
      krun_set_gpu_options = dlsym (kconf->handle, "krun_set_gpu_options");
      якщо (krun_set_gpu_options == NULL)
        повернути crun_make_error (err, 0, "gpu requested but the version of libkrun in this system does not support it");

      ret = krun_set_gpu_options (kconf->ctx_id, kconf->gpu_flags);
      якщо (UNLIKELY (ret < 0))
        повернути crun_make_error (err, -ret, "gpu requested but could not configure virtio gpu device");

      якщо (kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER)
        {
          krun_set_gpu_render_server_fd = dlsym (kconf->handle, "krun_set_gpu_render_server_fd");
          якщо (krun_set_gpu_render_server_fd == NULL)
            повернути crun_make_error (err, 0, "gpu with render server requested but the version of libkrun in this system does not support it");

          ret = krun_set_gpu_render_server_fd (kconf->ctx_id, kconf->gpu_render_server_fd);
          якщо (UNLIKELY (ret < 0))
            повернути crun_make_error (err, -ret, "gpu with render server requested but could not configure its file descriptor");
        }
    }

  якщо (kconf->config_tree != NULL)
    {
      /* Try to configure an external kernel. If the configuration file doesn't
       * specify a kernel, libkrun automatically fall back to using libkrunfw,
       * if the library is present and was loaded while creating the context.
       */
      ціле custom_kernel = libkrun_parse_resource_configuration (kconf->config_tree, container, "krun.custom_kernel", "custom_kernel", false);
      якщо (custom_kernel > 0)
        {
          ret = libkrun_configure_kernel (ctx_id, handle, kconf->config_tree, err);
          якщо (UNLIKELY (ret))
            повернути ret;
        }
    }

  повернути 0;
}

static ціле
libkrun_configure_flavor (void *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  ціле ret, sev_indicated = 0, awsnitro_indicated = 0;
  struct krun_config *kconf = (struct krun_config *) cookie;
  const char *flavor = NULL;
  void *close_handles[2];

  close_handles[0] = NULL;
  close_handles[1] = NULL;

  // Check if the user provided the krun variant through OCI annotations.
  flavor = find_annotation (container, "krun.variant");
  якщо (flavor != NULL)
    {
      sev_indicated |= strcmp (flavor, KRUN_FLAVOR_SEV) == 0;
      awsnitro_indicated |= strcmp (flavor, KRUN_FLAVOR_AWS_NITRO) == 0;
    }

  якщо (sev_indicated)
    {
      якщо (kconf->handle_sev == NULL)
        error (EXIT_FAILURE, 0, "the container requires libkrun-sev but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_awsnitro;

      kconf->handle = kconf->handle_sev;
      kconf->ctx_id = kconf->ctx_id_sev;
      kconf->sev = true;
    }
  інакше якщо (awsnitro_indicated)
    {
      якщо (kconf->handle_awsnitro == NULL)
        error (EXIT_FAILURE, 0, "the container requires libkrun-awsnitro but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_sev;

      kconf->handle = kconf->handle_awsnitro;
      kconf->ctx_id = kconf->ctx_id_awsnitro;
      kconf->awsnitro = true;
    }
  інакше
    {
      якщо (kconf->handle == NULL)
        error (EXIT_FAILURE, 0, "the container requires libkrun but it's not available");

      close_handles[0] = kconf->handle_sev;
      close_handles[1] = kconf->handle_awsnitro;
    }

  // We no longer need the other two libkrun handles.
  для (ціле i = 0; i < 2; i++)
    {
      якщо (close_handles[i] == NULL)
        continue;

      ret = dlclose (close_handles[i]);
      якщо (UNLIKELY (ret != 0))
        повернути crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }

  повернути 0;
}

static ціле
libkrun_exec (void *cookie, libcrun_container_t *container, const char *pathname, char *const argv[])
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_set_log_level) (uint32_t level);
  ціле (*krun_start_enter) (uint32_t ctx_id);
  int32_t (*krun_add_virtiofs2) (uint32_t ctx_id, const char *c_tag, const char *c_path, uint64_t shm_size);
  int32_t (*krun_set_root_disk) (uint32_t ctx_id, const char *disk_path);
  int32_t (*krun_set_tee_config_file) (uint32_t ctx_id, const char *file_path);
  int32_t (*krun_set_console_output) (uint32_t ctx_id, const char *c_filepath);
  int32_t (*krun_set_exec) (uint32_t ctx_id, const char *exec_path,
                            const char *const argv[], const char *const envp[]);
  struct krun_config *kconf = (struct krun_config *) cookie;
  void *handle;
  int32_t ctx_id, ret;
  libcrun_error_t err;

  ret = libkrun_configure_flavor (cookie, container, &err);
  якщо (UNLIKELY (ret < 0))
    {
      ціле errcode = crun_error_get_errno (&err);
      crun_error_release (&err);
      error (EXIT_FAILURE, errcode, "unable to configure libkrun flavor");
    }

  // /dev/kvm is required for all non AWS nitro workloads.
  якщо (! kconf->awsnitro && ! kconf->has_kvm)
    error (EXIT_FAILURE, 0, "`/dev/kvm` unavailable");

  handle = kconf->handle;
  ctx_id = kconf->ctx_id;

  krun_set_log_level = dlsym (handle, "krun_set_log_level");
  krun_start_enter = dlsym (handle, "krun_start_enter");
  якщо (krun_set_log_level == NULL || krun_start_enter == NULL)
    error (EXIT_FAILURE, 0, "could not find symbol in the krun library");

  /* Set log level according to crun's verbosity. */
  switch (libcrun_get_verbosity ())
    {
    case LIBCRUN_VERBOSITY_DEBUG:
      krun_set_log_level (KRUN_LOG_LEVEL_DEBUG);
      break;
    case LIBCRUN_VERBOSITY_WARNING:
      krun_set_log_level (KRUN_LOG_LEVEL_WARN);
      break;
    default:
      krun_set_log_level (KRUN_LOG_LEVEL_ERROR);
      break;
    }

  якщо (kconf->sev)
    {
      krun_set_root_disk = dlsym (handle, "krun_set_root_disk");
      krun_set_tee_config_file = dlsym (handle, "krun_set_tee_config_file");
      якщо (krun_set_root_disk == NULL || krun_set_tee_config_file == NULL)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-sev.so`");

      ret = krun_set_root_disk (ctx_id, "/disk.img");
      якщо (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set root disk");

      ret = krun_set_tee_config_file (ctx_id, KRUN_SEV_FILE);
      якщо (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set krun tee config file");
    }
  інакше
    {
      json_object *val_virtiofs_tag = NULL;
      json_object *val_virtiofs_shm_size = NULL;
      const char *virtiofs_tag = NULL;
      // Default to a conservative DAX size of 512MB, just like krun_set_root() does.
      uint64_t virtiofs_shm_size = 512 * 1024 * 1024ULL;

      якщо (kconf->config_tree != NULL)
        {
          val_virtiofs_tag = json_object_object_get (kconf->config_tree, "virtiofs_tag");
          якщо (val_virtiofs_tag != NULL && json_object_is_type (val_virtiofs_tag, json_type_string))
            virtiofs_tag = json_object_get_string (val_virtiofs_tag);

          val_virtiofs_shm_size = json_object_object_get (kconf->config_tree, "virtiofs_shm_size");
          якщо (val_virtiofs_shm_size != NULL && json_object_is_type (val_virtiofs_shm_size, json_type_int))
            virtiofs_shm_size = json_object_get_uint64 (val_virtiofs_shm_size);
        }

      якщо (virtiofs_tag == NULL)
        virtiofs_tag = "/dev/root";

      krun_add_virtiofs2 = dlsym (handle, "krun_add_virtiofs2");

      якщо (krun_add_virtiofs2 == NULL)
        error (EXIT_FAILURE, 0, "could not find symbol `krun_add_virtiofs2` in `libkrun.so`");

      ret = krun_add_virtiofs2 (ctx_id, virtiofs_tag, "/", virtiofs_shm_size);
      якщо (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not add virtiofs root with tag `%s`", virtiofs_tag);
    }

  якщо (kconf->awsnitro)
    {
      krun_set_console_output = dlsym (handle, "krun_set_console_output");
      krun_set_exec = dlsym (handle, "krun_set_exec");
      якщо (krun_set_console_output == NULL || krun_set_exec == NULL)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-awsnitro.so`");

      // Redirect all enclave output (read from vsock) to stdout.
      ret = krun_set_console_output (ctx_id, "/dev/stdout");
      якщо (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not redirect enclave output to stdout");

      ret = krun_set_exec (ctx_id, pathname, (const char *const *) argv,
                           (const char *const *) def->process->env);
      якщо (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set enclave execution arguments");
    }

  ret = libkrun_configure_vm (ctx_id, handle, kconf, container, &err);
  якщо (UNLIKELY (ret))
    {
      ціле errcode = crun_error_get_errno (&err);
      libcrun_error_t *tmp_err = &err;
      libcrun_error_report_and_release (tmp_err);
      error (EXIT_FAILURE, errcode, "could not configure krun vm");
    }

  json_object_put (kconf->config_doc);

  ret = krun_start_enter (ctx_id);
  якщо (UNLIKELY (ret < 0))
    error (EXIT_FAILURE, -ret, "could not start krun");

  повернути ret;
}

static ціле
libkrun_start_render_server (struct krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  ціле ret;

  kconf->gpu_flags = libkrun_parse_resource_configuration (kconf->config_tree, container, "krun.gpu_flags", "gpu_flags", false);
  якщо (kconf->gpu_flags > 0)
    {
      якщо (access ("/dev/dri", F_OK) != 0)
        повернути crun_make_error (err, errno, "gpu requested but /dev/dri is not available");

      якщо ((kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER) != 0)
        {
          char *rs_argv[40];
          ціле rs_argc = 0;
          char fd_as_str[16];
          ціле gpu_fds[2];
          pid_t pid;
          ціле null;

          якщо (access ("/usr/libexec/virgl_render_server", X_OK) != 0)
            повернути crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/libexec/virgl_render_server binary");

          якщо (access ("/usr/bin/bwrap", X_OK) != 0)
            повернути crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/bin/bwrap binary");

          ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, gpu_fds);
          якщо (UNLIKELY (ret < 0))
            повернути crun_make_error (err, errno, "could not create socketpair for virgl_render_server");

          snprintf (fd_as_str, sizeof (fd_as_str), "%d", gpu_fds[FD_PAIR_CHILD]);

          rs_argv[rs_argc++] = (char *) "bwrap";
          rs_argv[rs_argc++] = (char *) "--ro-bind";
          rs_argv[rs_argc++] = (char *) "/usr";
          rs_argv[rs_argc++] = (char *) "/usr";
          якщо (access ("/lib", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (char *) "--ro-bind";
              rs_argv[rs_argc++] = (char *) "/lib";
              rs_argv[rs_argc++] = (char *) "/lib";
            }
          якщо (access ("/lib64", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (char *) "--ro-bind";
              rs_argv[rs_argc++] = (char *) "/lib64";
              rs_argv[rs_argc++] = (char *) "/lib64";
            }
          rs_argv[rs_argc++] = (char *) "--ro-bind";
          rs_argv[rs_argc++] = (char *) "/etc";
          rs_argv[rs_argc++] = (char *) "/etc";
          rs_argv[rs_argc++] = (char *) "--ro-bind";
          rs_argv[rs_argc++] = (char *) "/sys";
          rs_argv[rs_argc++] = (char *) "/sys";
          rs_argv[rs_argc++] = (char *) "--dev";
          rs_argv[rs_argc++] = (char *) "/dev";
          rs_argv[rs_argc++] = (char *) "--dev-bind";
          rs_argv[rs_argc++] = (char *) "/dev/dri";
          rs_argv[rs_argc++] = (char *) "/dev/dri";
          rs_argv[rs_argc++] = (char *) "--proc";
          rs_argv[rs_argc++] = (char *) "/proc";
          rs_argv[rs_argc++] = (char *) "--tmpfs";
          rs_argv[rs_argc++] = (char *) "/tmp";
          rs_argv[rs_argc++] = (char *) "--unshare-pid";
          rs_argv[rs_argc++] = (char *) "--unshare-ipc";
          rs_argv[rs_argc++] = (char *) "--unshare-net";
          rs_argv[rs_argc++] = (char *) "--";
          rs_argv[rs_argc++] = (char *) "/usr/libexec/virgl_render_server";
          rs_argv[rs_argc++] = (char *) "--socket-fd";
          rs_argv[rs_argc++] = fd_as_str;
          rs_argv[rs_argc] = NULL;

          pid = fork ();
          якщо (pid < 0)
            {
              close (gpu_fds[FD_PAIR_PARENT]);
              close (gpu_fds[FD_PAIR_CHILD]);
              повернути crun_make_error (err, errno, "could not fork for virgl_render_server");
            }
          інакше якщо (pid == 0)
            {
              close (gpu_fds[FD_PAIR_PARENT]);

              null = open ("/dev/null", O_WRONLY);
              якщо (null == -1)
                _exit (EXIT_FAILURE);

              dup2 (null, STDOUT_FILENO);
              dup2 (null, STDERR_FILENO);
              close (null);

              execvp ("/usr/bin/bwrap", rs_argv);
              _exit (EXIT_FAILURE);
            }

          close (gpu_fds[FD_PAIR_CHILD]);
          kconf->gpu_render_server_fd = gpu_fds[FD_PAIR_PARENT];
        }
    }

  повернути 0;
}

static ціле
libkrun_configure_network (void *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  struct krun_config *kconf = (struct krun_config *) cookie;
  pid_t pid;
  char *passt_argv[10];
  char fd_as_str[16];
  ціле use_passt;
  ціле argv_idx;
  ціле status;
  ціле null;
  ціле ret;

  ret = libkrun_parse_string_configuration (kconf->config_tree, container,
                                            "krun.tap_name", "tap_name",
                                            &kconf->tap_name, err, false);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  якщо (kconf->tap_name != NULL && is_empty_string (kconf->tap_name))
    повернути crun_make_error (err, 0, "krun.tap_name cannot be empty");

  use_passt = libkrun_parse_resource_configuration (kconf->config_tree, container, "krun.use_passt", "use_passt", false);

  якщо (use_passt > 0)
    {
      якщо (kconf->tap_name != NULL)
        повернути crun_make_error (err, 0, "krun.tap_name and krun.use_passt are mutually exclusive");

      kconf->use_passt = 1;
    }
  інакше
    повернути 0;

  ret = socketpair (AF_UNIX, SOCK_STREAM, 0, kconf->passt_fds);
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, errno, "create passt socketpair");

  snprintf (fd_as_str, sizeof (fd_as_str), "%d", kconf->passt_fds[FD_PAIR_CHILD]);

  argv_idx = 0;
  passt_argv[argv_idx++] = (char *) "passt";
  passt_argv[argv_idx++] = (char *) "-t";
  passt_argv[argv_idx++] = (char *) "all";

  якщо (! kconf->has_awsnitro)
    {
      passt_argv[argv_idx++] = (char *) "-u";
      passt_argv[argv_idx++] = (char *) "all";
      passt_argv[argv_idx++] = (char *) "--no-dhcp-dns";
    }

  /* Set --no-map-gw. If the gateway for the network is also the DNS server
   * we won't be able to query the DNS server without --no-map-gw.
   * See discussion in https://github.com/containers/crun/pull/2099
   */
  passt_argv[argv_idx++] = (char *) "--no-map-gw";

  passt_argv[argv_idx++] = (char *) "--fd";
  passt_argv[argv_idx++] = fd_as_str;
  passt_argv[argv_idx] = NULL;

  pid = fork ();
  якщо (pid < 0)
    повернути crun_make_error (err, errno, "fork passt");
  інакше якщо (pid == 0)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t i;

      close (kconf->passt_fds[FD_PAIR_PARENT]);

      null = open ("/dev/null", O_WRONLY);
      якщо (null == -1)
        _exit (EXIT_FAILURE);

      /* Remount /proc if the parent detached its PID namespace, as passt(1)
       * needs a consistent /proc view to operate properly.
       */
      для (i = 0; i < def->linux->namespaces_len; i++)
        {
          ціле value = libcrun_find_namespace (def->linux->namespaces[i]->type);

          якщо (value == CLONE_NEWPID)
            {
              якщо (mount ("", "/proc", "proc", MS_NOSUID | MS_NODEV | MS_NOEXEC, NULL))
                _exit (EXIT_FAILURE);

              break;
            }
        }

      // Redirect passt's stdout and stderr to /dev/null, as closing them here
      // instead will cause passt to exit with an error.
      dup2 (null, STDOUT_FILENO);
      dup2 (null, STDERR_FILENO);
      close (null);

      execvp ("/usr/bin/passt", passt_argv);
      // Only reachable on error.
      _exit (EXIT_FAILURE);
    }

  close (kconf->passt_fds[FD_PAIR_CHILD]);

  // Wait for passt to daemonize itself.
  waitpid (pid, &status, 0);
  якщо (! (WIFEXITED (status)) || WEXITSTATUS (status) != 0)
    повернути crun_make_error (err, 0, "start passt");

  повернути 0;
}

/* Return true if the spec already declares a device with the given PATH.  */
static bool
spec_has_device (runtime_spec_schema_config_schema *def, const char *path)
{
  size_t i;

  для (i = 0; i < def->linux->devices_len; i++)
    якщо (strcmp (def->linux->devices[i]->path, path) == 0)
      повернути true;
  повернути false;
}

/* libkrun_create_kvm_device: explicitly adds kvm device.  */
static ціле
libkrun_configure_container (void *cookie, enum handler_configure_phase phase,
                             libcrun_context_t *context, libcrun_container_t *container,
                             const char *rootfs, libcrun_error_t *err)
{
  ціле ret, rootfsfd;
  struct krun_config *kconf = (struct krun_config *) cookie;
  struct device_s kvm_device = { "/dev/kvm", "c", 10, 232, 0666, 0, 0 };
  struct device_s sev_device = { "/dev/sev", "c", 10, 124, 0666, 0, 0 };
  struct device_s nitro_device = { "/dev/nitro_enclaves", "c", 10, 122, 0666, 0, 0 };
  cleanup_close ціле devfd = -1;
  cleanup_close ціле rootfsfd_cleanup = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  bool create_sev = false, create_awsnitro = false;
  bool is_user_ns;

  якщо (rootfs == NULL)
    rootfsfd = AT_FDCWD;
  інакше
    {
      rootfsfd = rootfsfd_cleanup = open (rootfs, O_PATH | O_CLOEXEC);
      якщо (UNLIKELY (rootfsfd < 0))
        повернути crun_make_error (err, errno, "open `%s`", rootfs);
    }

  якщо (phase == HANDLER_CONFIGURE_BEFORE_USERNS)
    {
      cleanup_free char *origin_config_path = NULL;
      cleanup_free char *state_dir = NULL;
      cleanup_free char *config = NULL;
      cleanup_close ціле fd = -1;
      size_t config_size;

      ret = libcrun_get_state_directory (&state_dir, context->state_root, context->id, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;

      ret = append_paths (&origin_config_path, err, state_dir, "config.json", NULL);
      якщо (UNLIKELY (ret < 0))
        повернути ret;

      ret = read_all_file (origin_config_path, &config, &config_size, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;

      /* CVE-2025-24965: the content below rootfs cannot be trusted because it is controlled by the user.  We
         must ensure the file is opened below the rootfs directory.  Whatever the image ships at that name
         could also be a device node or a FIFO, so never open it: unlink it and create a new regular file.  */
      ret = unlinkat (rootfsfd, KRUN_CONFIG_FILE, 0);
      якщо (UNLIKELY (ret < 0 && errno != ENOENT))
        повернути crun_make_error (err, errno, "unlink `%s`", KRUN_CONFIG_FILE);

      fd = safe_openat (rootfsfd, rootfs, KRUN_CONFIG_FILE, WRITE_FILE_DEFAULT_FLAGS | O_EXCL | O_NOFOLLOW, S_IRUSR | S_IRGRP | S_IROTH, err);
      якщо (UNLIKELY (fd < 0))
        повернути fd;

      ret = safe_write (fd, KRUN_CONFIG_FILE, config, config_size, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;

      ret = libkrun_read_vm_config (kconf, rootfsfd, rootfs, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;

      ret = libkrun_start_render_server (kconf, container, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;
    }

  якщо (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    повернути 0;

  ret = libkrun_configure_network (cookie, container, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  /* Do nothing if /dev/kvm is already present in spec */
  якщо (spec_has_device (def, "/dev/kvm"))
    повернути 0;

  якщо (kconf->handle_sev != NULL)
    create_sev = ! spec_has_device (def, "/dev/sev");

  якщо (kconf->handle_awsnitro != NULL)
    create_awsnitro = ! spec_has_device (def, "/dev/nitro_enclaves");

  devfd = safe_openat (rootfsfd, rootfs, "dev", O_PATH | O_DIRECTORY | O_CLOEXEC, 0, err);
  якщо (UNLIKELY (devfd < 0))
    повернути devfd;

  ret = check_running_in_user_namespace (err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;
  is_user_ns = ret;

  якщо (kconf->has_kvm)
    {
      ret = libcrun_create_dev (container, devfd, -1, &kvm_device, is_user_ns, true, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;
    }

  якщо (create_sev)
    {
      ret = libcrun_create_dev (container, devfd, -1, &sev_device, is_user_ns, true, err);
      якщо (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_sev);
          якщо (UNLIKELY (ret < 0))
            повернути ret;
          kconf->handle_sev = NULL;
        }
    }

  якщо (create_awsnitro)
    {
      ret = libcrun_create_dev (container, devfd, -1, &nitro_device, is_user_ns, true, err);
      якщо (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_awsnitro);
          якщо (UNLIKELY (ret < 0))
            повернути ret;
          kconf->handle_awsnitro = NULL;
        }
    }

  повернути 0;
}

static ціле
libkrun_load (void **cookie, libcrun_error_t *err)
{
  int32_t ret;
  struct krun_config *kconf;
  const char *libkrun_so = "libkrun.so.1";
  const char *libkrun_sev_so = "libkrun-sev.so.1";
  const char *libkrun_awsnitro_so = "libkrun-awsnitro.so.1";

  kconf = malloc (sizeof (struct krun_config));
  якщо (kconf == NULL)
    повернути crun_make_error (err, 0, "could not allocate memory for krun_config");
  memset (kconf, 0, sizeof (struct krun_config));

  kconf->gpu_render_server_fd = -1;

  kconf->handle = dlopen (libkrun_so, RTLD_NOW);
  kconf->handle_sev = dlopen (libkrun_sev_so, RTLD_NOW);
  kconf->handle_awsnitro = dlopen (libkrun_awsnitro_so, RTLD_NOW);

  якщо (kconf->handle == NULL && kconf->handle_sev == NULL && kconf->handle_awsnitro == NULL)
    {
      free (kconf);
      повернути crun_make_error (err, 0, "failed to open `%s`, `%s`, and `%s` for krun_config: %s", libkrun_so, libkrun_sev_so, libkrun_awsnitro_so, dlerror ());
    }

  kconf->sev = false;
  kconf->awsnitro = false;

  /* Newer versions of libkrun no longer link against libkrunfw and
     instead they open it when creating the context. This implies
     we need to call "krun_create_ctx" before switching namespaces
     or it won't be able to find the library bundling the kernel. */
  якщо (kconf->handle)
    {
      ret = libkrun_create_context (kconf->handle, err);
      якщо (UNLIKELY (ret < 0))
        goto error;
      kconf->ctx_id = ret;
    }

  якщо (kconf->handle_sev)
    {
      ret = libkrun_create_context (kconf->handle_sev, err);
      якщо (UNLIKELY (ret < 0))
        goto error;
      kconf->ctx_id_sev = ret;
    }
  якщо (kconf->handle_awsnitro)
    {
      ret = libkrun_create_context (kconf->handle_awsnitro, err);
      якщо (UNLIKELY (ret < 0))
        goto error;
      kconf->ctx_id_awsnitro = ret;
    }

  *cookie = kconf;

  повернути 0;

error:
  якщо (kconf->handle)
    dlclose (kconf->handle);
  якщо (kconf->handle_sev)
    dlclose (kconf->handle_sev);
  якщо (kconf->handle_awsnitro)
    dlclose (kconf->handle_awsnitro);
  free (kconf);
  повернути ret;
}

static ціле
libkrun_unload (void *cookie, libcrun_error_t *err)
{
  ціле r;

  struct krun_config *kconf = (struct krun_config *) cookie;
  якщо (kconf != NULL)
    {
      якщо (kconf->handle != NULL)
        {
          r = dlclose (kconf->handle);
          якщо (UNLIKELY (r != 0))
            повернути crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
        }
      якщо (kconf->handle_sev != NULL)
        {
          r = dlclose (kconf->handle_sev);
          якщо (UNLIKELY (r != 0))
            повернути crun_make_error (err, 0, "could not unload handle_sev: `%s`", dlerror ());
        }
      якщо (kconf->handle_awsnitro != NULL)
        {
          r = dlclose (kconf->handle_awsnitro);
          якщо (UNLIKELY (r != 0))
            повернути crun_make_error (err, 0, "could not unload handle_awsnitro: `%s`", dlerror ());
        }
      free (kconf);
    }
  повернути 0;
}

static runtime_spec_schema_defs_linux_device_cgroup *
make_oci_spec_dev (const char *type, dev_t device, bool allow, const char *access)
{
  runtime_spec_schema_defs_linux_device_cgroup *dev = xmalloc0 (sizeof (*dev));

  dev->allow = allow;
  dev->allow_present = 1;

  dev->type = xstrdup (type);

  dev->major = major (device);
  dev->major_present = 1;

  dev->minor = minor (device);
  dev->minor_present = 1;

  dev->access = xstrdup (access);

  повернути dev;
}

/* stat an optional device: on ENOENT clear *PRESENT, any other error fails.  */
static ціле
stat_optional_device (const char *path, struct stat *st, bool *present, libcrun_error_t *err)
{
  ціле ret = stat (path, st);
  якщо (UNLIKELY (ret < 0))
    {
      якщо (errno != ENOENT)
        повернути crun_make_error (err, errno, "stat `%s`", path);
      *present = false;
    }
  повернути 0;
}

static ціле
libkrun_modify_oci_configuration (void *cookie arg_unused, libcrun_context_t *context arg_unused,
                                  runtime_spec_schema_config_schema *def,
                                  libcrun_error_t *err)
{
  const size_t device_size = sizeof (runtime_spec_schema_defs_linux_device_cgroup);
  struct krun_config *kconf = (struct krun_config *) cookie;
  struct stat st_kvm, st_sev, st_awsnitro;
  bool has_kvm = true, has_sev = true, has_awsnitro = true;
  size_t old_len, new_len;
  ціле ret;

  /* Always allow the /dev/kvm device.  */

  ret = stat_optional_device ("/dev/kvm", &st_kvm, &has_kvm, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = stat_optional_device ("/dev/sev", &st_sev, &has_sev, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = stat_optional_device ("/dev/nitro_enclaves", &st_awsnitro, &has_awsnitro, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  kconf->has_kvm = has_kvm;
  kconf->has_awsnitro = has_awsnitro;

  якщо (! has_kvm && ! has_awsnitro)
    повернути 0;

  /* spec says these are optional, ensure they exist so we can add our devices */
  якщо (def->linux == NULL)
    def->linux = xmalloc0 (sizeof (runtime_spec_schema_config_linux));

  якщо (def->linux->resources == NULL)
    def->linux->resources = xmalloc0 (sizeof (runtime_spec_schema_config_linux_resources));

  old_len = def->linux->resources->devices_len;
  new_len = old_len;
  якщо (has_kvm)
    new_len += has_sev ? 2 : 1;
  якщо (has_awsnitro)
    new_len += 1;

  def->linux->resources->devices = xrealloc (def->linux->resources->devices, device_size * (new_len + 1));
  def->linux->resources->devices_len = new_len;

  якщо (has_kvm)
    {
      def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_kvm.st_rdev, true, "rwm");
      якщо (has_sev)
        def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_sev.st_rdev, true, "rwm");
    }

  якщо (has_awsnitro)
    def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_awsnitro.st_rdev, true, "rwm");

  повернути 0;
}

static ціле
libkrun_close_fds (void *cookie, libcrun_container_t *container, ціле preserve_fds, libcrun_error_t *err)
{
  struct krun_config *kconf = (struct krun_config *) cookie;
  ціле first_fd_to_close = preserve_fds + 3;
  ціле low_fd, high_fd;
  ціле i;

  low_fd = high_fd = -1;

  якщо (kconf->use_passt)
    high_fd = kconf->passt_fds[FD_PAIR_PARENT];

  якщо (kconf->gpu_render_server_fd != -1)
    {
      якщо (kconf->gpu_render_server_fd > high_fd)
        {
          low_fd = high_fd;
          high_fd = kconf->gpu_render_server_fd;
        }
      інакше
        low_fd = kconf->gpu_render_server_fd;
    }

  якщо (first_fd_to_close <= high_fd)
    {
      для (i = first_fd_to_close; i < high_fd; i++)
        {
          // If we're closing proc_fd, make sure to invalidate it.
          якщо (i == container->proc_fd)
            container->proc_fd = -1;
          інакше якщо (i == low_fd)
            continue;
          close (i);
        }

      first_fd_to_close = high_fd + 1;
    }

  повернути mark_or_close_fds_ge_than (container, first_fd_to_close, true, err);
}

struct custom_handler_s handler_libkrun = {
  .name = "krun",
  .alias = NULL,
  .feature_string = "LIBKRUN",
  .supports_open_tree_namespace = false,
  .load = libkrun_load,
  .unload = libkrun_unload,
  .run_func = libkrun_exec,
  .configure_container = libkrun_configure_container,
  .modify_oci_configuration = libkrun_modify_oci_configuration,
  .close_fds = libkrun_close_fds,
};

#endif
