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

#ضمن <config.h>
#ضمن "../custom-handler.h"
#ضمن "../container.h"
#ضمن "../utils.h"
#ضمن "../linux.h"
#ضمن <unistd.h>
#ضمن <sys/stat.h>
#ضمن <sys/socket.h>
#ضمن <errno.h>
#ضمن <sys/mount.h>
#ضمن <sys/param.h>
#ضمن <sys/types.h>
#ضمن <sys/sysmacros.h>
#ضمن <fcntl.h>
#ضمن <sched.h>
#ضمن <ocispec/runtime_spec_schema_config_schema.h>

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

بنية krun_config
{
  فراغ *handle;
  فراغ *handle_sev;
  فراغ *handle_awsnitro;
  bool sev;
  bool awsnitro;
  int32_t ctx_id;
  int32_t ctx_id_sev;
  int32_t ctx_id_awsnitro;
  bool has_kvm;
  bool has_awsnitro;
  صحيح passt_fds[2];
  json_object *config_doc;
  json_object *config_tree;
  bool use_passt;
  ثابت حرف *tap_name;
  صحيح gpu_render_server_fd;
  صحيح gpu_flags;
};

/* libkrun handler.  */
#𐡄𐡍 HAVE_DLOPEN && HAVE_LIBKRUN
ساكن int32_t
libkrun_create_context (فراغ *handle, libcrun_error_t *err)
{
  int32_t (*krun_create_ctx) ();
  int32_t ctx_id;

  krun_create_ctx = dlsym (handle, "krun_create_ctx");
  𐡄𐡍 (krun_create_ctx == عدم)
    أرجع crun_make_error (err, 0, "could not find symbol in the krun library");

  ctx_id = krun_create_ctx ();
  𐡄𐡍 (UNLIKELY (ctx_id < 0))
    أرجع crun_make_error (err, -ctx_id, "could not create krun context");

  أرجع ctx_id;
}

ساكن صحيح
libkrun_configure_kernel (uint32_t ctx_id, فراغ *handle, json_object *config_tree, libcrun_error_t *err)
{
  int32_t (*krun_set_kernel) (uint32_t ctx_id, ثابت حرف *kernel_path,
                              uint32_t kernel_format, ثابت حرف *initrd_path, ثابت حرف *kernel_cmdline);
  json_object *kernel_path = عدم;
  json_object *kernel_format = عدم;
  json_object *val_initrd_path = عدم;
  json_object *val_kernel_cmdline = عدم;
  ثابت حرف *initrd_path = عدم;
  ثابت حرف *kernel_cmdline = عدم;
  صحيح ret;

  /* kernel_path and kernel_format must be present */
  kernel_path = json_object_object_get (config_tree, "kernel_path");
  𐡄𐡍 (kernel_path == عدم || ! json_object_is_type (kernel_path, json_type_string))
    أرجع 0;

  kernel_format = json_object_object_get (config_tree, "kernel_format");
  𐡄𐡍 (kernel_format == عدم || ! json_object_is_type (kernel_format, json_type_int))
    أرجع 0;

  /* initrd and kernel_cmdline are optional */
  val_initrd_path = json_object_object_get (config_tree, "initrd_path");
  𐡄𐡍 (val_initrd_path != عدم && json_object_is_type (val_initrd_path, json_type_string))
    initrd_path = json_object_get_string (val_initrd_path);

  val_kernel_cmdline = json_object_object_get (config_tree, "kernel_cmdline");
  𐡄𐡍 (val_kernel_cmdline != عدم && json_object_is_type (val_kernel_cmdline, json_type_string))
    kernel_cmdline = json_object_get_string (val_kernel_cmdline);

  krun_set_kernel = dlsym (handle, "krun_set_kernel");
  𐡄𐡍 (krun_set_kernel == عدم)
    أرجع crun_make_error (err, 0, "could not find symbol in krun library");

  ret = krun_set_kernel (ctx_id,
                         json_object_get_string (kernel_path),
                         (uint32_t) json_object_get_int64 (kernel_format),
                         initrd_path, kernel_cmdline);

  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, -ret, "could not configure a krun external kernel");

  أرجع 0;
}

ساكن صحيح
libkrun_read_vm_config (بنية krun_config *kconf, صحيح rootfsfd, ثابت حرف *rootfs, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_free حرف *config = عدم;
  cleanup_close صحيح pathfd = -1;
  cleanup_close صحيح fd = -1;
  proc_fd_path_t fd_path;
  بنية stat st;

  /* The content of the rootfs is controlled by the user: look at the file through an
     O_PATH descriptor first and refuse anything that is not a small regular file, as
     opening a FIFO blocks and a device node must not be accessed.  */
  pathfd = safe_openat (rootfsfd, rootfs, KRUN_VM_FILE, O_PATH | O_CLOEXEC | O_NOFOLLOW, 0, err);
  𐡄𐡍 (pathfd < 0)
    {
      // The configuration file is optional, don't generate an error if it's missing.
      𐡄𐡍 (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          أرجع 0;
        }
      أرجع pathfd;
    }

  ret = fstat (pathfd, &st);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "stat `%s`", KRUN_VM_FILE);

  𐡄𐡍 (! S_ISREG (st.st_mode))
    أرجع crun_make_error (err, 0, "`%s` is not a regular file", KRUN_VM_FILE);

  get_proc_self_fd_path (fd_path, pathfd);

  fd = open (fd_path, O_RDONLY | O_CLOEXEC);
  𐡄𐡍 (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", KRUN_VM_FILE);

  ret = read_all_fd_with_max_size (fd, KRUN_VM_FILE, &config, عدم, KRUN_VM_FILE_MAX_SIZE, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  ret = parse_json_file (&kconf->config_doc, config, عدم, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  kconf->config_tree = kconf->config_doc;
  أرجع 0;
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
ساكن صحيح
libkrun_parse_resource_configuration (json_object *config_tree, libcrun_container_t *container, ثابت حرف *annotation, ثابت حرف *key, bool use_config_file)
{
  حرف *val_str, *endptr;
  صحيح val = -1;
  json_object *val_json = عدم;

  val_str = (حرف *) find_annotation (container, annotation);
  𐡄𐡍 (val_str != عدم)
    {
      errno = 0;
      val = (صحيح) strtol (val_str, &endptr, 10);
      𐡄𐡍 (errno != 0 || endptr == val_str || *endptr != '\0')
        /* Annotations value is not a valid integer. */
        error (EXIT_FAILURE, 0, "krun annotation %s value cannot be converted to an integer", annotation);

      𐡄𐡍 (val < 0)
        error (EXIT_FAILURE, 0, "krun annotation %s value must be a positive integer", annotation);

      أرجع val;
    }
  وإلا 𐡄𐡍 (use_config_file && config_tree != عدم)
    {
      val_json = json_object_object_get (config_tree, key);
      𐡄𐡍 (val_json == عدم)
        أرجع val;
      𐡄𐡍 (! json_object_is_type (val_json, json_type_int))
        error (EXIT_FAILURE, 0, "krun krun_vm.json %s value is not an integer", key);

      val = (صحيح) json_object_get_int64 (val_json);
    }

  أرجع val;
}

ساكن صحيح
libkrun_parse_string_configuration (json_object *config_tree, libcrun_container_t *container,
                                    ثابت حرف *annotation, ثابت حرف *key,
                                    ثابت حرف **value, libcrun_error_t *err, bool use_config_file)
{
  ثابت حرف *val;
  json_object *val_json = عدم;

  *value = عدم;

  val = find_annotation (container, annotation);
  𐡄𐡍 (val != عدم)
    {
      *value = val;
      أرجع 0;
    }

  𐡄𐡍 (! use_config_file || config_tree == عدم)
    أرجع 0;

  val_json = json_object_object_get (config_tree, key);
  𐡄𐡍 (val_json == عدم)
    أرجع 0;

  𐡄𐡍 (! json_object_is_type (val_json, json_type_string))
    أرجع crun_make_error (err, 0, ".krun_vm.json %s value is not a string", key);

  *value = json_object_get_string (val_json);
  أرجع 0;
}

ساكن فراغ
libkrun_make_tap_mac (ثابت حرف *id, uint8_t mac[6])
{
  uint64_t hash = 14695981039346656037ULL;
  size_t i;

  لكل (; *id != '\0'; id++)
    {
      hash ^= (uint8_t) *id;
      hash *= 1099511628211ULL;
    }

  لكل (i = 0; i < 6; i++)
    mac[i] = (uint8_t) (hash >> (i * 8));

  mac[0] = (mac[0] & 0xfe) | 0x02;
}

ساكن صحيح
libkrun_configure_vm (uint32_t ctx_id, فراغ *handle, بنية krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_set_vm_config) (uint32_t ctx_id, uint8_t num_vcpus, uint32_t ram_mib);
  int32_t (*krun_add_net_unixstream) (uint32_t ctx_id, ثابت حرف *c_path, صحيح fd, uint8_t *ثابت c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_add_net_tap) (uint32_t ctx_id, ثابت حرف *c_tap_name, ثابت uint8_t *c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_set_gpu_options) (uint32_t ctx_id, uint32_t virgl_flags);
  int32_t (*krun_set_gpu_render_server_fd) (uint32_t ctx_id, صحيح fd);
  صحيح cpus, ram_mib, nested_virt, ret;
  cpu_set_t set;

  /* We let the OCI set the number of vCPUs for the VM, since cgroups CPU restrictions still apply. */
  cpus = libkrun_parse_resource_configuration (kconf->config_tree, container, "krun.cpus", "cpus", true);
  𐡄𐡍 (cpus <= 0)
    {
      CPU_ZERO (&set);
      𐡄𐡍 (sched_getaffinity (getpid (), حجم (set), &set) == 0)
        cpus = MIN (CPU_COUNT (&set), LIBKRUN_MAX_VCPUS);
      وإلا
        cpus = 1;
    }

  /* We let the OCI set the amount of RAM for the VM, since cgroups memory restrictions still apply. */
  ram_mib = libkrun_parse_resource_configuration (kconf->config_tree, container, "krun.ram_mib", "ram_mib", true);
  𐡄𐡍 (ram_mib <= LIBKRUN_MINIMUM_RAM_MIB)
    {
      𐡄𐡍 (def && def->linux && def->linux->resources && def->linux->resources->memory
          && def->linux->resources->memory->limit_present)
        ram_mib = def->linux->resources->memory->limit / (1024 * 1024);
      وإلا
        ram_mib = LIBKRUN_DEFAULT_RAM_MIB;
    }

  krun_set_vm_config = dlsym (handle, "krun_set_vm_config");

  𐡄𐡍 (krun_set_vm_config == عدم)
    أرجع crun_make_error (err, 0, "could not find symbol in the krun library");

  ret = krun_set_vm_config (ctx_id, cpus, ram_mib);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, -ret, "could not set krun vm configuration");

  nested_virt = libkrun_parse_resource_configuration (kconf->config_tree, container, "krun.nested_virt", "nested_virt", false);
  𐡄𐡍 (nested_virt > 0)
    {
      int32_t (*krun_check_nested_virt) (فراغ);
      int32_t (*krun_set_nested_virt) (uint32_t ctx_id, bool enabled);

      krun_check_nested_virt = dlsym (handle, "krun_check_nested_virt");
      𐡄𐡍 (krun_check_nested_virt != عدم && krun_check_nested_virt () != 1)
        libcrun_warning ("nested virtualization requested but may not be supported on this host");

      krun_set_nested_virt = dlsym (handle, "krun_set_nested_virt");
      𐡄𐡍 (krun_set_nested_virt == عدم)
        أرجع crun_make_error (err, 0, "could not find symbol `krun_set_nested_virt` in the krun library");

      ret = krun_set_nested_virt (ctx_id, true);
      𐡄𐡍 (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, -ret, "could not enable nested virtualization");
    }

  𐡄𐡍 (kconf->tap_name != عدم)
    {
      krun_add_net_tap = dlsym (handle, "krun_add_net_tap");
      𐡄𐡍 (krun_add_net_tap == عدم)
        أرجع crun_make_error (err, 0, "could not find symbol `krun_add_net_tap` in the krun library");

      uint8_t mac[6];
      libkrun_make_tap_mac (container->context->id, mac);
      ret = krun_add_net_tap (ctx_id, kconf->tap_name, &mac[0], COMPAT_NET_FEATURES, 0);
      𐡄𐡍 (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, -ret, "could not add krun TAP interface `%s`", kconf->tap_name);
    }
  وإلا 𐡄𐡍 (kconf->use_passt)
    {
      krun_add_net_unixstream = dlsym (handle, "krun_add_net_unixstream");
      𐡄𐡍 (krun_add_net_unixstream == عدم)
        أرجع crun_make_error (err, 0, "could not find symbol `krun_add_net_unixstream` in the krun library");

      uint8_t mac[] = { 0x5a, 0x94, 0xef, 0xe4, 0x0c, 0xee };
      ret = krun_add_net_unixstream (ctx_id, عدم, kconf->passt_fds[FD_PAIR_PARENT], &mac[0], COMPAT_NET_FEATURES, NET_FLAG_DHCP_CLIENT);
      𐡄𐡍 (UNLIKELY (ret == -EINVAL))
        ret = krun_add_net_unixstream (ctx_id, عدم, kconf->passt_fds[FD_PAIR_PARENT], &mac[0], COMPAT_NET_FEATURES, 0);
      𐡄𐡍 (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, -ret, "could not set krun net configuration");
    }

  𐡄𐡍 (kconf->gpu_flags > 0)
    {
      krun_set_gpu_options = dlsym (kconf->handle, "krun_set_gpu_options");
      𐡄𐡍 (krun_set_gpu_options == عدم)
        أرجع crun_make_error (err, 0, "gpu requested but the version of libkrun in this system does not support it");

      ret = krun_set_gpu_options (kconf->ctx_id, kconf->gpu_flags);
      𐡄𐡍 (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, -ret, "gpu requested but could not configure virtio gpu device");

      𐡄𐡍 (kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER)
        {
          krun_set_gpu_render_server_fd = dlsym (kconf->handle, "krun_set_gpu_render_server_fd");
          𐡄𐡍 (krun_set_gpu_render_server_fd == عدم)
            أرجع crun_make_error (err, 0, "gpu with render server requested but the version of libkrun in this system does not support it");

          ret = krun_set_gpu_render_server_fd (kconf->ctx_id, kconf->gpu_render_server_fd);
          𐡄𐡍 (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, -ret, "gpu with render server requested but could not configure its file descriptor");
        }
    }

  𐡄𐡍 (kconf->config_tree != عدم)
    {
      /* Try to configure an external kernel. If the configuration file doesn't
       * specify a kernel, libkrun automatically fall back to using libkrunfw,
       * if the library is present and was loaded while creating the context.
       */
      صحيح custom_kernel = libkrun_parse_resource_configuration (kconf->config_tree, container, "krun.custom_kernel", "custom_kernel", false);
      𐡄𐡍 (custom_kernel > 0)
        {
          ret = libkrun_configure_kernel (ctx_id, handle, kconf->config_tree, err);
          𐡄𐡍 (UNLIKELY (ret))
            أرجع ret;
        }
    }

  أرجع 0;
}

ساكن صحيح
libkrun_configure_flavor (فراغ *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  صحيح ret, sev_indicated = 0, awsnitro_indicated = 0;
  بنية krun_config *kconf = (بنية krun_config *) cookie;
  ثابت حرف *flavor = عدم;
  فراغ *close_handles[2];

  close_handles[0] = عدم;
  close_handles[1] = عدم;

  // Check if the user provided the krun variant through OCI annotations.
  flavor = find_annotation (container, "krun.variant");
  𐡄𐡍 (flavor != عدم)
    {
      sev_indicated |= strcmp (flavor, KRUN_FLAVOR_SEV) == 0;
      awsnitro_indicated |= strcmp (flavor, KRUN_FLAVOR_AWS_NITRO) == 0;
    }

  𐡄𐡍 (sev_indicated)
    {
      𐡄𐡍 (kconf->handle_sev == عدم)
        error (EXIT_FAILURE, 0, "the container requires libkrun-sev but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_awsnitro;

      kconf->handle = kconf->handle_sev;
      kconf->ctx_id = kconf->ctx_id_sev;
      kconf->sev = true;
    }
  وإلا 𐡄𐡍 (awsnitro_indicated)
    {
      𐡄𐡍 (kconf->handle_awsnitro == عدم)
        error (EXIT_FAILURE, 0, "the container requires libkrun-awsnitro but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_sev;

      kconf->handle = kconf->handle_awsnitro;
      kconf->ctx_id = kconf->ctx_id_awsnitro;
      kconf->awsnitro = true;
    }
  وإلا
    {
      𐡄𐡍 (kconf->handle == عدم)
        error (EXIT_FAILURE, 0, "the container requires libkrun but it's not available");

      close_handles[0] = kconf->handle_sev;
      close_handles[1] = kconf->handle_awsnitro;
    }

  // We no longer need the other two libkrun handles.
  لكل (صحيح i = 0; i < 2; i++)
    {
      𐡄𐡍 (close_handles[i] == عدم)
        استمر;

      ret = dlclose (close_handles[i]);
      𐡄𐡍 (UNLIKELY (ret != 0))
        أرجع crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }

  أرجع 0;
}

ساكن صحيح
libkrun_exec (فراغ *cookie, libcrun_container_t *container, ثابت حرف *pathname, حرف *ثابت argv[])
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_set_log_level) (uint32_t level);
  صحيح (*krun_start_enter) (uint32_t ctx_id);
  int32_t (*krun_add_virtiofs2) (uint32_t ctx_id, ثابت حرف *c_tag, ثابت حرف *c_path, uint64_t shm_size);
  int32_t (*krun_set_root_disk) (uint32_t ctx_id, ثابت حرف *disk_path);
  int32_t (*krun_set_tee_config_file) (uint32_t ctx_id, ثابت حرف *file_path);
  int32_t (*krun_set_console_output) (uint32_t ctx_id, ثابت حرف *c_filepath);
  int32_t (*krun_set_exec) (uint32_t ctx_id, ثابت حرف *exec_path,
                            ثابت حرف *ثابت argv[], ثابت حرف *ثابت envp[]);
  بنية krun_config *kconf = (بنية krun_config *) cookie;
  فراغ *handle;
  int32_t ctx_id, ret;
  libcrun_error_t err;

  ret = libkrun_configure_flavor (cookie, container, &err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    {
      صحيح errcode = crun_error_get_errno (&err);
      crun_error_release (&err);
      error (EXIT_FAILURE, errcode, "unable to configure libkrun flavor");
    }

  // /dev/kvm is required for all non AWS nitro workloads.
  𐡄𐡍 (! kconf->awsnitro && ! kconf->has_kvm)
    error (EXIT_FAILURE, 0, "`/dev/kvm` unavailable");

  handle = kconf->handle;
  ctx_id = kconf->ctx_id;

  krun_set_log_level = dlsym (handle, "krun_set_log_level");
  krun_start_enter = dlsym (handle, "krun_start_enter");
  𐡄𐡍 (krun_set_log_level == عدم || krun_start_enter == عدم)
    error (EXIT_FAILURE, 0, "could not find symbol in the krun library");

  /* Set log level according to crun's verbosity. */
  بدل (libcrun_get_verbosity ())
    {
    حالة LIBCRUN_VERBOSITY_DEBUG:
      krun_set_log_level (KRUN_LOG_LEVEL_DEBUG);
      اكسر;
    حالة LIBCRUN_VERBOSITY_WARNING:
      krun_set_log_level (KRUN_LOG_LEVEL_WARN);
      اكسر;
    افتراضي:
      krun_set_log_level (KRUN_LOG_LEVEL_ERROR);
      اكسر;
    }

  𐡄𐡍 (kconf->sev)
    {
      krun_set_root_disk = dlsym (handle, "krun_set_root_disk");
      krun_set_tee_config_file = dlsym (handle, "krun_set_tee_config_file");
      𐡄𐡍 (krun_set_root_disk == عدم || krun_set_tee_config_file == عدم)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-sev.so`");

      ret = krun_set_root_disk (ctx_id, "/disk.img");
      𐡄𐡍 (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set root disk");

      ret = krun_set_tee_config_file (ctx_id, KRUN_SEV_FILE);
      𐡄𐡍 (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set krun tee config file");
    }
  وإلا
    {
      json_object *val_virtiofs_tag = عدم;
      json_object *val_virtiofs_shm_size = عدم;
      ثابت حرف *virtiofs_tag = عدم;
      // Default to a conservative DAX size of 512MB, just like krun_set_root() does.
      uint64_t virtiofs_shm_size = 512 * 1024 * 1024ULL;

      𐡄𐡍 (kconf->config_tree != عدم)
        {
          val_virtiofs_tag = json_object_object_get (kconf->config_tree, "virtiofs_tag");
          𐡄𐡍 (val_virtiofs_tag != عدم && json_object_is_type (val_virtiofs_tag, json_type_string))
            virtiofs_tag = json_object_get_string (val_virtiofs_tag);

          val_virtiofs_shm_size = json_object_object_get (kconf->config_tree, "virtiofs_shm_size");
          𐡄𐡍 (val_virtiofs_shm_size != عدم && json_object_is_type (val_virtiofs_shm_size, json_type_int))
            virtiofs_shm_size = json_object_get_uint64 (val_virtiofs_shm_size);
        }

      𐡄𐡍 (virtiofs_tag == عدم)
        virtiofs_tag = "/dev/root";

      krun_add_virtiofs2 = dlsym (handle, "krun_add_virtiofs2");

      𐡄𐡍 (krun_add_virtiofs2 == عدم)
        error (EXIT_FAILURE, 0, "could not find symbol `krun_add_virtiofs2` in `libkrun.so`");

      ret = krun_add_virtiofs2 (ctx_id, virtiofs_tag, "/", virtiofs_shm_size);
      𐡄𐡍 (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not add virtiofs root with tag `%s`", virtiofs_tag);
    }

  𐡄𐡍 (kconf->awsnitro)
    {
      krun_set_console_output = dlsym (handle, "krun_set_console_output");
      krun_set_exec = dlsym (handle, "krun_set_exec");
      𐡄𐡍 (krun_set_console_output == عدم || krun_set_exec == عدم)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-awsnitro.so`");

      // Redirect all enclave output (read from vsock) to stdout.
      ret = krun_set_console_output (ctx_id, "/dev/stdout");
      𐡄𐡍 (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not redirect enclave output to stdout");

      ret = krun_set_exec (ctx_id, pathname, (ثابت حرف *ثابت *) argv,
                           (ثابت حرف *ثابت *) def->process->env);
      𐡄𐡍 (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set enclave execution arguments");
    }

  ret = libkrun_configure_vm (ctx_id, handle, kconf, container, &err);
  𐡄𐡍 (UNLIKELY (ret))
    {
      صحيح errcode = crun_error_get_errno (&err);
      libcrun_error_t *tmp_err = &err;
      libcrun_error_report_and_release (tmp_err);
      error (EXIT_FAILURE, errcode, "could not configure krun vm");
    }

  json_object_put (kconf->config_doc);

  ret = krun_start_enter (ctx_id);
  𐡄𐡍 (UNLIKELY (ret < 0))
    error (EXIT_FAILURE, -ret, "could not start krun");

  أرجع ret;
}

ساكن صحيح
libkrun_start_render_server (بنية krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  صحيح ret;

  kconf->gpu_flags = libkrun_parse_resource_configuration (kconf->config_tree, container, "krun.gpu_flags", "gpu_flags", false);
  𐡄𐡍 (kconf->gpu_flags > 0)
    {
      𐡄𐡍 (access ("/dev/dri", F_OK) != 0)
        أرجع crun_make_error (err, errno, "gpu requested but /dev/dri is not available");

      𐡄𐡍 ((kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER) != 0)
        {
          حرف *rs_argv[40];
          صحيح rs_argc = 0;
          حرف fd_as_str[16];
          صحيح gpu_fds[2];
          pid_t pid;
          صحيح null;

          𐡄𐡍 (access ("/usr/libexec/virgl_render_server", X_OK) != 0)
            أرجع crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/libexec/virgl_render_server binary");

          𐡄𐡍 (access ("/usr/bin/bwrap", X_OK) != 0)
            أرجع crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/bin/bwrap binary");

          ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, gpu_fds);
          𐡄𐡍 (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "could not create socketpair for virgl_render_server");

          snprintf (fd_as_str, حجم (fd_as_str), "%d", gpu_fds[FD_PAIR_CHILD]);

          rs_argv[rs_argc++] = (حرف *) "bwrap";
          rs_argv[rs_argc++] = (حرف *) "--ro-bind";
          rs_argv[rs_argc++] = (حرف *) "/usr";
          rs_argv[rs_argc++] = (حرف *) "/usr";
          𐡄𐡍 (access ("/lib", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (حرف *) "--ro-bind";
              rs_argv[rs_argc++] = (حرف *) "/lib";
              rs_argv[rs_argc++] = (حرف *) "/lib";
            }
          𐡄𐡍 (access ("/lib64", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (حرف *) "--ro-bind";
              rs_argv[rs_argc++] = (حرف *) "/lib64";
              rs_argv[rs_argc++] = (حرف *) "/lib64";
            }
          rs_argv[rs_argc++] = (حرف *) "--ro-bind";
          rs_argv[rs_argc++] = (حرف *) "/etc";
          rs_argv[rs_argc++] = (حرف *) "/etc";
          rs_argv[rs_argc++] = (حرف *) "--ro-bind";
          rs_argv[rs_argc++] = (حرف *) "/sys";
          rs_argv[rs_argc++] = (حرف *) "/sys";
          rs_argv[rs_argc++] = (حرف *) "--dev";
          rs_argv[rs_argc++] = (حرف *) "/dev";
          rs_argv[rs_argc++] = (حرف *) "--dev-bind";
          rs_argv[rs_argc++] = (حرف *) "/dev/dri";
          rs_argv[rs_argc++] = (حرف *) "/dev/dri";
          rs_argv[rs_argc++] = (حرف *) "--proc";
          rs_argv[rs_argc++] = (حرف *) "/proc";
          rs_argv[rs_argc++] = (حرف *) "--tmpfs";
          rs_argv[rs_argc++] = (حرف *) "/tmp";
          rs_argv[rs_argc++] = (حرف *) "--unshare-pid";
          rs_argv[rs_argc++] = (حرف *) "--unshare-ipc";
          rs_argv[rs_argc++] = (حرف *) "--unshare-net";
          rs_argv[rs_argc++] = (حرف *) "--";
          rs_argv[rs_argc++] = (حرف *) "/usr/libexec/virgl_render_server";
          rs_argv[rs_argc++] = (حرف *) "--socket-fd";
          rs_argv[rs_argc++] = fd_as_str;
          rs_argv[rs_argc] = عدم;

          pid = fork ();
          𐡄𐡍 (pid < 0)
            {
              close (gpu_fds[FD_PAIR_PARENT]);
              close (gpu_fds[FD_PAIR_CHILD]);
              أرجع crun_make_error (err, errno, "could not fork for virgl_render_server");
            }
          وإلا 𐡄𐡍 (pid == 0)
            {
              close (gpu_fds[FD_PAIR_PARENT]);

              null = open ("/dev/null", O_WRONLY);
              𐡄𐡍 (null == -1)
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

  أرجع 0;
}

ساكن صحيح
libkrun_configure_network (فراغ *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  بنية krun_config *kconf = (بنية krun_config *) cookie;
  pid_t pid;
  حرف *passt_argv[10];
  حرف fd_as_str[16];
  صحيح use_passt;
  صحيح argv_idx;
  صحيح status;
  صحيح null;
  صحيح ret;

  ret = libkrun_parse_string_configuration (kconf->config_tree, container,
                                            "krun.tap_name", "tap_name",
                                            &kconf->tap_name, err, false);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  𐡄𐡍 (kconf->tap_name != عدم && is_empty_string (kconf->tap_name))
    أرجع crun_make_error (err, 0, "krun.tap_name cannot be empty");

  use_passt = libkrun_parse_resource_configuration (kconf->config_tree, container, "krun.use_passt", "use_passt", false);

  𐡄𐡍 (use_passt > 0)
    {
      𐡄𐡍 (kconf->tap_name != عدم)
        أرجع crun_make_error (err, 0, "krun.tap_name and krun.use_passt are mutually exclusive");

      kconf->use_passt = 1;
    }
  وإلا
    أرجع 0;

  ret = socketpair (AF_UNIX, SOCK_STREAM, 0, kconf->passt_fds);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "create passt socketpair");

  snprintf (fd_as_str, حجم (fd_as_str), "%d", kconf->passt_fds[FD_PAIR_CHILD]);

  argv_idx = 0;
  passt_argv[argv_idx++] = (حرف *) "passt";
  passt_argv[argv_idx++] = (حرف *) "-t";
  passt_argv[argv_idx++] = (حرف *) "all";

  𐡄𐡍 (! kconf->has_awsnitro)
    {
      passt_argv[argv_idx++] = (حرف *) "-u";
      passt_argv[argv_idx++] = (حرف *) "all";
      passt_argv[argv_idx++] = (حرف *) "--no-dhcp-dns";
    }

  /* Set --no-map-gw. If the gateway for the network is also the DNS server
   * we won't be able to query the DNS server without --no-map-gw.
   * See discussion in https://github.com/containers/crun/pull/2099
   */
  passt_argv[argv_idx++] = (حرف *) "--no-map-gw";

  passt_argv[argv_idx++] = (حرف *) "--fd";
  passt_argv[argv_idx++] = fd_as_str;
  passt_argv[argv_idx] = عدم;

  pid = fork ();
  𐡄𐡍 (pid < 0)
    أرجع crun_make_error (err, errno, "fork passt");
  وإلا 𐡄𐡍 (pid == 0)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t i;

      close (kconf->passt_fds[FD_PAIR_PARENT]);

      null = open ("/dev/null", O_WRONLY);
      𐡄𐡍 (null == -1)
        _exit (EXIT_FAILURE);

      /* Remount /proc if the parent detached its PID namespace, as passt(1)
       * needs a consistent /proc view to operate properly.
       */
      لكل (i = 0; i < def->linux->namespaces_len; i++)
        {
          صحيح value = libcrun_find_namespace (def->linux->namespaces[i]->type);

          𐡄𐡍 (value == CLONE_NEWPID)
            {
              𐡄𐡍 (mount ("", "/proc", "proc", MS_NOSUID | MS_NODEV | MS_NOEXEC, عدم))
                _exit (EXIT_FAILURE);

              اكسر;
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
  𐡄𐡍 (! (WIFEXITED (status)) || WEXITSTATUS (status) != 0)
    أرجع crun_make_error (err, 0, "start passt");

  أرجع 0;
}

/* Return true if the spec already declares a device with the given PATH.  */
ساكن bool
spec_has_device (runtime_spec_schema_config_schema *def, ثابت حرف *path)
{
  size_t i;

  لكل (i = 0; i < def->linux->devices_len; i++)
    𐡄𐡍 (strcmp (def->linux->devices[i]->path, path) == 0)
      أرجع true;
  أرجع false;
}

/* libkrun_create_kvm_device: explicitly adds kvm device.  */
ساكن صحيح
libkrun_configure_container (فراغ *cookie, تعداد handler_configure_phase phase,
                             libcrun_context_t *context, libcrun_container_t *container,
                             ثابت حرف *rootfs, libcrun_error_t *err)
{
  صحيح ret, rootfsfd;
  بنية krun_config *kconf = (بنية krun_config *) cookie;
  بنية device_s kvm_device = { "/dev/kvm", "c", 10, 232, 0666, 0, 0 };
  بنية device_s sev_device = { "/dev/sev", "c", 10, 124, 0666, 0, 0 };
  بنية device_s nitro_device = { "/dev/nitro_enclaves", "c", 10, 122, 0666, 0, 0 };
  cleanup_close صحيح devfd = -1;
  cleanup_close صحيح rootfsfd_cleanup = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  bool create_sev = false, create_awsnitro = false;
  bool is_user_ns;

  𐡄𐡍 (rootfs == عدم)
    rootfsfd = AT_FDCWD;
  وإلا
    {
      rootfsfd = rootfsfd_cleanup = open (rootfs, O_PATH | O_CLOEXEC);
      𐡄𐡍 (UNLIKELY (rootfsfd < 0))
        أرجع crun_make_error (err, errno, "open `%s`", rootfs);
    }

  𐡄𐡍 (phase == HANDLER_CONFIGURE_BEFORE_USERNS)
    {
      cleanup_free حرف *origin_config_path = عدم;
      cleanup_free حرف *state_dir = عدم;
      cleanup_free حرف *config = عدم;
      cleanup_close صحيح fd = -1;
      size_t config_size;

      ret = libcrun_get_state_directory (&state_dir, context->state_root, context->id, err);
      𐡄𐡍 (UNLIKELY (ret < 0))
        أرجع ret;

      ret = append_paths (&origin_config_path, err, state_dir, "config.json", عدم);
      𐡄𐡍 (UNLIKELY (ret < 0))
        أرجع ret;

      ret = read_all_file (origin_config_path, &config, &config_size, err);
      𐡄𐡍 (UNLIKELY (ret < 0))
        أرجع ret;

      /* CVE-2025-24965: the content below rootfs cannot be trusted because it is controlled by the user.  We
         must ensure the file is opened below the rootfs directory.  Whatever the image ships at that name
         could also be a device node or a FIFO, so never open it: unlink it and create a new regular file.  */
      ret = unlinkat (rootfsfd, KRUN_CONFIG_FILE, 0);
      𐡄𐡍 (UNLIKELY (ret < 0 && errno != ENOENT))
        أرجع crun_make_error (err, errno, "unlink `%s`", KRUN_CONFIG_FILE);

      fd = safe_openat (rootfsfd, rootfs, KRUN_CONFIG_FILE, WRITE_FILE_DEFAULT_FLAGS | O_EXCL | O_NOFOLLOW, S_IRUSR | S_IRGRP | S_IROTH, err);
      𐡄𐡍 (UNLIKELY (fd < 0))
        أرجع fd;

      ret = safe_write (fd, KRUN_CONFIG_FILE, config, config_size, err);
      𐡄𐡍 (UNLIKELY (ret < 0))
        أرجع ret;

      ret = libkrun_read_vm_config (kconf, rootfsfd, rootfs, err);
      𐡄𐡍 (UNLIKELY (ret < 0))
        أرجع ret;

      ret = libkrun_start_render_server (kconf, container, err);
      𐡄𐡍 (UNLIKELY (ret < 0))
        أرجع ret;
    }

  𐡄𐡍 (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    أرجع 0;

  ret = libkrun_configure_network (cookie, container, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  /* Do nothing if /dev/kvm is already present in spec */
  𐡄𐡍 (spec_has_device (def, "/dev/kvm"))
    أرجع 0;

  𐡄𐡍 (kconf->handle_sev != عدم)
    create_sev = ! spec_has_device (def, "/dev/sev");

  𐡄𐡍 (kconf->handle_awsnitro != عدم)
    create_awsnitro = ! spec_has_device (def, "/dev/nitro_enclaves");

  devfd = safe_openat (rootfsfd, rootfs, "dev", O_PATH | O_DIRECTORY | O_CLOEXEC, 0, err);
  𐡄𐡍 (UNLIKELY (devfd < 0))
    أرجع devfd;

  ret = check_running_in_user_namespace (err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;
  is_user_ns = ret;

  𐡄𐡍 (kconf->has_kvm)
    {
      ret = libcrun_create_dev (container, devfd, -1, &kvm_device, is_user_ns, true, err);
      𐡄𐡍 (UNLIKELY (ret < 0))
        أرجع ret;
    }

  𐡄𐡍 (create_sev)
    {
      ret = libcrun_create_dev (container, devfd, -1, &sev_device, is_user_ns, true, err);
      𐡄𐡍 (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_sev);
          𐡄𐡍 (UNLIKELY (ret < 0))
            أرجع ret;
          kconf->handle_sev = عدم;
        }
    }

  𐡄𐡍 (create_awsnitro)
    {
      ret = libcrun_create_dev (container, devfd, -1, &nitro_device, is_user_ns, true, err);
      𐡄𐡍 (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_awsnitro);
          𐡄𐡍 (UNLIKELY (ret < 0))
            أرجع ret;
          kconf->handle_awsnitro = عدم;
        }
    }

  أرجع 0;
}

ساكن صحيح
libkrun_load (فراغ **cookie, libcrun_error_t *err)
{
  int32_t ret;
  بنية krun_config *kconf;
  ثابت حرف *libkrun_so = "libkrun.so.1";
  ثابت حرف *libkrun_sev_so = "libkrun-sev.so.1";
  ثابت حرف *libkrun_awsnitro_so = "libkrun-awsnitro.so.1";

  kconf = malloc (حجم (بنية krun_config));
  𐡄𐡍 (kconf == عدم)
    أرجع crun_make_error (err, 0, "could not allocate memory for krun_config");
  memset (kconf, 0, حجم (بنية krun_config));

  kconf->gpu_render_server_fd = -1;

  kconf->handle = dlopen (libkrun_so, RTLD_NOW);
  kconf->handle_sev = dlopen (libkrun_sev_so, RTLD_NOW);
  kconf->handle_awsnitro = dlopen (libkrun_awsnitro_so, RTLD_NOW);

  𐡄𐡍 (kconf->handle == عدم && kconf->handle_sev == عدم && kconf->handle_awsnitro == عدم)
    {
      حرر (kconf);
      أرجع crun_make_error (err, 0, "failed to open `%s`, `%s`, and `%s` for krun_config: %s", libkrun_so, libkrun_sev_so, libkrun_awsnitro_so, dlerror ());
    }

  kconf->sev = false;
  kconf->awsnitro = false;

  /* Newer versions of libkrun no longer link against libkrunfw and
     instead they open it when creating the context. This implies
     we need to call "krun_create_ctx" before switching namespaces
     or it won't be able to find the library bundling the kernel. */
  𐡄𐡍 (kconf->handle)
    {
      ret = libkrun_create_context (kconf->handle, err);
      𐡄𐡍 (UNLIKELY (ret < 0))
        اذهب error;
      kconf->ctx_id = ret;
    }

  𐡄𐡍 (kconf->handle_sev)
    {
      ret = libkrun_create_context (kconf->handle_sev, err);
      𐡄𐡍 (UNLIKELY (ret < 0))
        اذهب error;
      kconf->ctx_id_sev = ret;
    }
  𐡄𐡍 (kconf->handle_awsnitro)
    {
      ret = libkrun_create_context (kconf->handle_awsnitro, err);
      𐡄𐡍 (UNLIKELY (ret < 0))
        اذهب error;
      kconf->ctx_id_awsnitro = ret;
    }

  *cookie = kconf;

  أرجع 0;

error:
  𐡄𐡍 (kconf->handle)
    dlclose (kconf->handle);
  𐡄𐡍 (kconf->handle_sev)
    dlclose (kconf->handle_sev);
  𐡄𐡍 (kconf->handle_awsnitro)
    dlclose (kconf->handle_awsnitro);
  حرر (kconf);
  أرجع ret;
}

ساكن صحيح
libkrun_unload (فراغ *cookie, libcrun_error_t *err)
{
  صحيح r;

  بنية krun_config *kconf = (بنية krun_config *) cookie;
  𐡄𐡍 (kconf != عدم)
    {
      𐡄𐡍 (kconf->handle != عدم)
        {
          r = dlclose (kconf->handle);
          𐡄𐡍 (UNLIKELY (r != 0))
            أرجع crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
        }
      𐡄𐡍 (kconf->handle_sev != عدم)
        {
          r = dlclose (kconf->handle_sev);
          𐡄𐡍 (UNLIKELY (r != 0))
            أرجع crun_make_error (err, 0, "could not unload handle_sev: `%s`", dlerror ());
        }
      𐡄𐡍 (kconf->handle_awsnitro != عدم)
        {
          r = dlclose (kconf->handle_awsnitro);
          𐡄𐡍 (UNLIKELY (r != 0))
            أرجع crun_make_error (err, 0, "could not unload handle_awsnitro: `%s`", dlerror ());
        }
      حرر (kconf);
    }
  أرجع 0;
}

ساكن runtime_spec_schema_defs_linux_device_cgroup *
make_oci_spec_dev (ثابت حرف *type, dev_t device, bool allow, ثابت حرف *access)
{
  runtime_spec_schema_defs_linux_device_cgroup *dev = xmalloc0 (حجم (*dev));

  dev->allow = allow;
  dev->allow_present = 1;

  dev->type = xstrdup (type);

  dev->major = major (device);
  dev->major_present = 1;

  dev->minor = minor (device);
  dev->minor_present = 1;

  dev->access = xstrdup (access);

  أرجع dev;
}

/* stat an optional device: on ENOENT clear *PRESENT, any other error fails.  */
ساكن صحيح
stat_optional_device (ثابت حرف *path, بنية stat *st, bool *present, libcrun_error_t *err)
{
  صحيح ret = stat (path, st);
  𐡄𐡍 (UNLIKELY (ret < 0))
    {
      𐡄𐡍 (errno != ENOENT)
        أرجع crun_make_error (err, errno, "stat `%s`", path);
      *present = false;
    }
  أرجع 0;
}

ساكن صحيح
libkrun_modify_oci_configuration (فراغ *cookie arg_unused, libcrun_context_t *context arg_unused,
                                  runtime_spec_schema_config_schema *def,
                                  libcrun_error_t *err)
{
  ثابت size_t device_size = حجم (runtime_spec_schema_defs_linux_device_cgroup);
  بنية krun_config *kconf = (بنية krun_config *) cookie;
  بنية stat st_kvm, st_sev, st_awsnitro;
  bool has_kvm = true, has_sev = true, has_awsnitro = true;
  size_t old_len, new_len;
  صحيح ret;

  /* Always allow the /dev/kvm device.  */

  ret = stat_optional_device ("/dev/kvm", &st_kvm, &has_kvm, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  ret = stat_optional_device ("/dev/sev", &st_sev, &has_sev, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  ret = stat_optional_device ("/dev/nitro_enclaves", &st_awsnitro, &has_awsnitro, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  kconf->has_kvm = has_kvm;
  kconf->has_awsnitro = has_awsnitro;

  𐡄𐡍 (! has_kvm && ! has_awsnitro)
    أرجع 0;

  /* spec says these are optional, ensure they exist so we can add our devices */
  𐡄𐡍 (def->linux == عدم)
    def->linux = xmalloc0 (حجم (runtime_spec_schema_config_linux));

  𐡄𐡍 (def->linux->resources == عدم)
    def->linux->resources = xmalloc0 (حجم (runtime_spec_schema_config_linux_resources));

  old_len = def->linux->resources->devices_len;
  new_len = old_len;
  𐡄𐡍 (has_kvm)
    new_len += has_sev ? 2 : 1;
  𐡄𐡍 (has_awsnitro)
    new_len += 1;

  def->linux->resources->devices = xrealloc (def->linux->resources->devices, device_size * (new_len + 1));
  def->linux->resources->devices_len = new_len;

  𐡄𐡍 (has_kvm)
    {
      def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_kvm.st_rdev, true, "rwm");
      𐡄𐡍 (has_sev)
        def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_sev.st_rdev, true, "rwm");
    }

  𐡄𐡍 (has_awsnitro)
    def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_awsnitro.st_rdev, true, "rwm");

  أرجع 0;
}

ساكن صحيح
libkrun_close_fds (فراغ *cookie, libcrun_container_t *container, صحيح preserve_fds, libcrun_error_t *err)
{
  بنية krun_config *kconf = (بنية krun_config *) cookie;
  صحيح first_fd_to_close = preserve_fds + 3;
  صحيح low_fd, high_fd;
  صحيح i;

  low_fd = high_fd = -1;

  𐡄𐡍 (kconf->use_passt)
    high_fd = kconf->passt_fds[FD_PAIR_PARENT];

  𐡄𐡍 (kconf->gpu_render_server_fd != -1)
    {
      𐡄𐡍 (kconf->gpu_render_server_fd > high_fd)
        {
          low_fd = high_fd;
          high_fd = kconf->gpu_render_server_fd;
        }
      وإلا
        low_fd = kconf->gpu_render_server_fd;
    }

  𐡄𐡍 (first_fd_to_close <= high_fd)
    {
      لكل (i = first_fd_to_close; i < high_fd; i++)
        {
          // If we're closing proc_fd, make sure to invalidate it.
          𐡄𐡍 (i == container->proc_fd)
            container->proc_fd = -1;
          وإلا 𐡄𐡍 (i == low_fd)
            استمر;
          close (i);
        }

      first_fd_to_close = high_fd + 1;
    }

  أرجع mark_or_close_fds_ge_than (container, first_fd_to_close, true, err);
}

بنية custom_handler_s handler_libkrun = {
  .name = "krun",
  .alias = عدم,
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
