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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "../custom-handler.h"
#அந்தர்பாவ "../container.h"
#அந்தர்பாவ "../utils.h"
#அந்தர்பாவ "../linux.h"
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <sys/socket.h>
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <sys/mount.h>
#அந்தர்பாவ <sys/param.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <sys/sysmacros.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <sched.h>
#அந்தர்பாவ <ocispec/runtime_spec_schema_config_schema.h>

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

கட்டமைப்பு krun_config
{
  வெறுமை *handle;
  வெறுமை *handle_sev;
  வெறுமை *handle_awsnitro;
  bool sev;
  bool awsnitro;
  int32_t ctx_id;
  int32_t ctx_id_sev;
  int32_t ctx_id_awsnitro;
  bool has_kvm;
  bool has_awsnitro;
  முழு passt_fds[2];
  json_object *config_doc;
  json_object *config_மரம்;
  bool use_passt;
  மாறிலி எழுத்து *tap_name;
  முழு gpu_render_server_fd;
  முழு gpu_flags;
};

/* libkrun handler.  */
#எனில் HAVE_DLOPEN && HAVE_LIBKRUN
நிலையான int32_t
libkrun_create_context (வெறுமை *handle, libcrun_error_t *err)
{
  int32_t (*krun_create_ctx) ();
  int32_t ctx_id;

  krun_create_ctx = dlsym (handle, "krun_create_ctx");
  எனில் (krun_create_ctx == வெற்று)
    திரும்பு crun_make_error (err, 0, "could not find symbol in the krun library");

  ctx_id = krun_create_ctx ();
  எனில் (UNLIKELY (ctx_id < 0))
    திரும்பு crun_make_error (err, -ctx_id, "could not create krun context");

  திரும்பு ctx_id;
}

நிலையான முழு
libkrun_configure_kernel (uint32_t ctx_id, வெறுமை *handle, json_object *config_மரம், libcrun_error_t *err)
{
  int32_t (*krun_set_kernel) (uint32_t ctx_id, மாறிலி எழுத்து *kernel_path,
                              uint32_t kernel_format, மாறிலி எழுத்து *initrd_path, மாறிலி எழுத்து *kernel_cmdline);
  json_object *kernel_path = வெற்று;
  json_object *kernel_format = வெற்று;
  json_object *val_initrd_path = வெற்று;
  json_object *val_kernel_cmdline = வெற்று;
  மாறிலி எழுத்து *initrd_path = வெற்று;
  மாறிலி எழுத்து *kernel_cmdline = வெற்று;
  முழு ret;

  /* kernel_path and kernel_format must be present */
  kernel_path = json_object_object_get (config_மரம், "kernel_path");
  எனில் (kernel_path == வெற்று || ! json_object_is_type (kernel_path, json_type_string))
    திரும்பு 0;

  kernel_format = json_object_object_get (config_மரம், "kernel_format");
  எனில் (kernel_format == வெற்று || ! json_object_is_type (kernel_format, json_type_int))
    திரும்பு 0;

  /* initrd and kernel_cmdline are optional */
  val_initrd_path = json_object_object_get (config_மரம், "initrd_path");
  எனில் (val_initrd_path != வெற்று && json_object_is_type (val_initrd_path, json_type_string))
    initrd_path = json_object_get_string (val_initrd_path);

  val_kernel_cmdline = json_object_object_get (config_மரம், "kernel_cmdline");
  எனில் (val_kernel_cmdline != வெற்று && json_object_is_type (val_kernel_cmdline, json_type_string))
    kernel_cmdline = json_object_get_string (val_kernel_cmdline);

  krun_set_kernel = dlsym (handle, "krun_set_kernel");
  எனில் (krun_set_kernel == வெற்று)
    திரும்பு crun_make_error (err, 0, "could not find symbol in krun library");

  ret = krun_set_kernel (ctx_id,
                         json_object_get_string (kernel_path),
                         (uint32_t) json_object_get_int64 (kernel_format),
                         initrd_path, kernel_cmdline);

  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, -ret, "could not configure a krun external kernel");

  திரும்பு 0;
}

நிலையான முழு
libkrun_read_vm_config (கட்டமைப்பு krun_config *kconf, முழு rootfsfd, மாறிலி எழுத்து *rootfs, libcrun_error_t *err)
{
  முழு ret;
  cleanup_free எழுத்து *config = வெற்று;
  cleanup_close முழு pathfd = -1;
  cleanup_close முழு fd = -1;
  proc_fd_path_t fd_path;
  கட்டமைப்பு stat st;

  /* The content of the rootfs is controlled by the user: look at the file through an
     O_PATH descriptor first and refuse anything that is not a small regular file, as
     opening a FIFO blocks and a device node must not be accessed.  */
  pathfd = safe_openat (rootfsfd, rootfs, KRUN_VM_FILE, O_PATH | O_CLOEXEC | O_NOFOLLOW, 0, err);
  எனில் (pathfd < 0)
    {
      // The configuration file is optional, don't generate an error if it's missing.
      எனில் (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          திரும்பு 0;
        }
      திரும்பு pathfd;
    }

  ret = fstat (pathfd, &st);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "stat `%s`", KRUN_VM_FILE);

  எனில் (! S_ISREG (st.st_mode))
    திரும்பு crun_make_error (err, 0, "`%s` is not a regular file", KRUN_VM_FILE);

  get_proc_self_fd_path (fd_path, pathfd);

  fd = open (fd_path, O_RDONLY | O_CLOEXEC);
  எனில் (UNLIKELY (fd < 0))
    திரும்பு crun_make_error (err, errno, "open `%s`", KRUN_VM_FILE);

  ret = read_all_fd_with_max_size (fd, KRUN_VM_FILE, &config, வெற்று, KRUN_VM_FILE_MAX_SIZE, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = parse_json_file (&kconf->config_doc, config, வெற்று, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  kconf->config_மரம் = kconf->config_doc;
  திரும்பு 0;
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
நிலையான முழு
libkrun_parse_resource_configuration (json_object *config_மரம், libcrun_container_t *container, மாறிலி எழுத்து *annotation, மாறிலி எழுத்து *சாவி, bool use_config_file)
{
  எழுத்து *val_str, *endptr;
  முழு val = -1;
  json_object *val_json = வெற்று;

  val_str = (எழுத்து *) find_annotation (container, annotation);
  எனில் (val_str != வெற்று)
    {
      errno = 0;
      val = (முழு) strtol (val_str, &endptr, 10);
      எனில் (errno != 0 || endptr == val_str || *endptr != '\0')
        /* Annotations value is not a valid integer. */
        error (EXIT_FAILURE, 0, "krun annotation %s value cannot be converted to an integer", annotation);

      எனில் (val < 0)
        error (EXIT_FAILURE, 0, "krun annotation %s value must be a positive integer", annotation);

      திரும்பு val;
    }
  இல்லையெனில் எனில் (use_config_file && config_மரம் != வெற்று)
    {
      val_json = json_object_object_get (config_மரம், சாவி);
      எனில் (val_json == வெற்று)
        திரும்பு val;
      எனில் (! json_object_is_type (val_json, json_type_int))
        error (EXIT_FAILURE, 0, "krun krun_vm.json %s value is not an integer", சாவி);

      val = (முழு) json_object_get_int64 (val_json);
    }

  திரும்பு val;
}

நிலையான முழு
libkrun_parse_string_configuration (json_object *config_மரம், libcrun_container_t *container,
                                    மாறிலி எழுத்து *annotation, மாறிலி எழுத்து *சாவி,
                                    மாறிலி எழுத்து **மதிப்பு, libcrun_error_t *err, bool use_config_file)
{
  மாறிலி எழுத்து *val;
  json_object *val_json = வெற்று;

  *மதிப்பு = வெற்று;

  val = find_annotation (container, annotation);
  எனில் (val != வெற்று)
    {
      *மதிப்பு = val;
      திரும்பு 0;
    }

  எனில் (! use_config_file || config_மரம் == வெற்று)
    திரும்பு 0;

  val_json = json_object_object_get (config_மரம், சாவி);
  எனில் (val_json == வெற்று)
    திரும்பு 0;

  எனில் (! json_object_is_type (val_json, json_type_string))
    திரும்பு crun_make_error (err, 0, ".krun_vm.json %s value is not a string", சாவி);

  *மதிப்பு = json_object_get_string (val_json);
  திரும்பு 0;
}

நிலையான வெறுமை
libkrun_make_tap_mac (மாறிலி எழுத்து *id, uint8_t mac[6])
{
  uint64_t hash = 14695981039346656037ULL;
  size_t i;

  க்கு (; *id != '\0'; id++)
    {
      hash ^= (uint8_t) *id;
      hash *= 1099511628211ULL;
    }

  க்கு (i = 0; i < 6; i++)
    mac[i] = (uint8_t) (hash >> (i * 8));

  mac[0] = (mac[0] & 0xfe) | 0x02;
}

நிலையான முழு
libkrun_configure_vm (uint32_t ctx_id, வெறுமை *handle, கட்டமைப்பு krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_set_vm_config) (uint32_t ctx_id, uint8_t இலக்கம்_vcpus, uint32_t ram_mib);
  int32_t (*krun_add_net_unixstream) (uint32_t ctx_id, மாறிலி எழுத்து *c_path, முழு fd, uint8_t *மாறிலி c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_add_net_tap) (uint32_t ctx_id, மாறிலி எழுத்து *c_tap_name, மாறிலி uint8_t *c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_set_gpu_options) (uint32_t ctx_id, uint32_t virgl_flags);
  int32_t (*krun_set_gpu_render_server_fd) (uint32_t ctx_id, முழு fd);
  முழு cpus, ram_mib, nested_virt, ret;
  cpu_set_t set;

  /* We let the OCI set the number of vCPUs for the VM, since cgroups CPU restrictions still apply. */
  cpus = libkrun_parse_resource_configuration (kconf->config_மரம், container, "krun.cpus", "cpus", true);
  எனில் (cpus <= 0)
    {
      CPU_ZERO (&set);
      எனில் (sched_getaffinity (getpid (), அளவு (set), &set) == 0)
        cpus = MIN (CPU_மொத்தம் (&set), LIBKRUN_MAX_VCPUS);
      இல்லையெனில்
        cpus = 1;
    }

  /* We let the OCI set the amount of RAM for the VM, since cgroups memory restrictions still apply. */
  ram_mib = libkrun_parse_resource_configuration (kconf->config_மரம், container, "krun.ram_mib", "ram_mib", true);
  எனில் (ram_mib <= LIBKRUN_MINIMUM_RAM_MIB)
    {
      எனில் (def && def->linux && def->linux->resources && def->linux->resources->memory
          && def->linux->resources->memory->limit_present)
        ram_mib = def->linux->resources->memory->limit / (1024 * 1024);
      இல்லையெனில்
        ram_mib = LIBKRUN_DEFAULT_RAM_MIB;
    }

  krun_set_vm_config = dlsym (handle, "krun_set_vm_config");

  எனில் (krun_set_vm_config == வெற்று)
    திரும்பு crun_make_error (err, 0, "could not find symbol in the krun library");

  ret = krun_set_vm_config (ctx_id, cpus, ram_mib);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, -ret, "could not set krun vm configuration");

  nested_virt = libkrun_parse_resource_configuration (kconf->config_மரம், container, "krun.nested_virt", "nested_virt", false);
  எனில் (nested_virt > 0)
    {
      int32_t (*krun_check_nested_virt) (வெறுமை);
      int32_t (*krun_set_nested_virt) (uint32_t ctx_id, bool enabled);

      krun_check_nested_virt = dlsym (handle, "krun_check_nested_virt");
      எனில் (krun_check_nested_virt != வெற்று && krun_check_nested_virt () != 1)
        libcrun_warning ("nested virtualization requested but may not be supported on this host");

      krun_set_nested_virt = dlsym (handle, "krun_set_nested_virt");
      எனில் (krun_set_nested_virt == வெற்று)
        திரும்பு crun_make_error (err, 0, "could not find symbol `krun_set_nested_virt` in the krun library");

      ret = krun_set_nested_virt (ctx_id, true);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, -ret, "could not enable nested virtualization");
    }

  எனில் (kconf->tap_name != வெற்று)
    {
      krun_add_net_tap = dlsym (handle, "krun_add_net_tap");
      எனில் (krun_add_net_tap == வெற்று)
        திரும்பு crun_make_error (err, 0, "could not find symbol `krun_add_net_tap` in the krun library");

      uint8_t mac[6];
      libkrun_make_tap_mac (container->context->id, mac);
      ret = krun_add_net_tap (ctx_id, kconf->tap_name, &mac[0], COMPAT_NET_FEATURES, 0);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, -ret, "could not add krun TAP interface `%s`", kconf->tap_name);
    }
  இல்லையெனில் எனில் (kconf->use_passt)
    {
      krun_add_net_unixstream = dlsym (handle, "krun_add_net_unixstream");
      எனில் (krun_add_net_unixstream == வெற்று)
        திரும்பு crun_make_error (err, 0, "could not find symbol `krun_add_net_unixstream` in the krun library");

      uint8_t mac[] = { 0x5a, 0x94, 0xef, 0xe4, 0x0c, 0xee };
      ret = krun_add_net_unixstream (ctx_id, வெற்று, kconf->passt_fds[FD_PAIR_PARENT], &mac[0], COMPAT_NET_FEATURES, NET_FLAG_DHCP_CLIENT);
      எனில் (UNLIKELY (ret == -EINVAL))
        ret = krun_add_net_unixstream (ctx_id, வெற்று, kconf->passt_fds[FD_PAIR_PARENT], &mac[0], COMPAT_NET_FEATURES, 0);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, -ret, "could not set krun net configuration");
    }

  எனில் (kconf->gpu_flags > 0)
    {
      krun_set_gpu_options = dlsym (kconf->handle, "krun_set_gpu_options");
      எனில் (krun_set_gpu_options == வெற்று)
        திரும்பு crun_make_error (err, 0, "gpu requested but the version of libkrun in this system does not support it");

      ret = krun_set_gpu_options (kconf->ctx_id, kconf->gpu_flags);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, -ret, "gpu requested but could not configure virtio gpu device");

      எனில் (kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER)
        {
          krun_set_gpu_render_server_fd = dlsym (kconf->handle, "krun_set_gpu_render_server_fd");
          எனில் (krun_set_gpu_render_server_fd == வெற்று)
            திரும்பு crun_make_error (err, 0, "gpu with render server requested but the version of libkrun in this system does not support it");

          ret = krun_set_gpu_render_server_fd (kconf->ctx_id, kconf->gpu_render_server_fd);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, -ret, "gpu with render server requested but could not configure its file descriptor");
        }
    }

  எனில் (kconf->config_மரம் != வெற்று)
    {
      /* Try to configure an external kernel. If the configuration file doesn't
       * specify a kernel, libkrun automatically fall back to using libkrunfw,
       * if the library is present and was loaded while creating the context.
       */
      முழு custom_kernel = libkrun_parse_resource_configuration (kconf->config_மரம், container, "krun.custom_kernel", "custom_kernel", false);
      எனில் (custom_kernel > 0)
        {
          ret = libkrun_configure_kernel (ctx_id, handle, kconf->config_மரம், err);
          எனில் (UNLIKELY (ret))
            திரும்பு ret;
        }
    }

  திரும்பு 0;
}

நிலையான முழு
libkrun_configure_flavor (வெறுமை *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  முழு ret, sev_indicated = 0, awsnitro_indicated = 0;
  கட்டமைப்பு krun_config *kconf = (கட்டமைப்பு krun_config *) cookie;
  மாறிலி எழுத்து *flavor = வெற்று;
  வெறுமை *close_handles[2];

  close_handles[0] = வெற்று;
  close_handles[1] = வெற்று;

  // Check if the user provided the krun variant through OCI annotations.
  flavor = find_annotation (container, "krun.variant");
  எனில் (flavor != வெற்று)
    {
      sev_indicated |= strcmp (flavor, KRUN_FLAVOR_SEV) == 0;
      awsnitro_indicated |= strcmp (flavor, KRUN_FLAVOR_AWS_NITRO) == 0;
    }

  எனில் (sev_indicated)
    {
      எனில் (kconf->handle_sev == வெற்று)
        error (EXIT_FAILURE, 0, "the container requires libkrun-sev but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_awsnitro;

      kconf->handle = kconf->handle_sev;
      kconf->ctx_id = kconf->ctx_id_sev;
      kconf->sev = true;
    }
  இல்லையெனில் எனில் (awsnitro_indicated)
    {
      எனில் (kconf->handle_awsnitro == வெற்று)
        error (EXIT_FAILURE, 0, "the container requires libkrun-awsnitro but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_sev;

      kconf->handle = kconf->handle_awsnitro;
      kconf->ctx_id = kconf->ctx_id_awsnitro;
      kconf->awsnitro = true;
    }
  இல்லையெனில்
    {
      எனில் (kconf->handle == வெற்று)
        error (EXIT_FAILURE, 0, "the container requires libkrun but it's not available");

      close_handles[0] = kconf->handle_sev;
      close_handles[1] = kconf->handle_awsnitro;
    }

  // We no longer need the other two libkrun handles.
  க்கு (முழு i = 0; i < 2; i++)
    {
      எனில் (close_handles[i] == வெற்று)
        தொடர்;

      ret = dlclose (close_handles[i]);
      எனில் (UNLIKELY (ret != 0))
        திரும்பு crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }

  திரும்பு 0;
}

நிலையான முழு
libkrun_exec (வெறுமை *cookie, libcrun_container_t *container, மாறிலி எழுத்து *pathname, எழுத்து *மாறிலி argv[])
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_set_log_level) (uint32_t level);
  முழு (*krun_start_enter) (uint32_t ctx_id);
  int32_t (*krun_add_virtiofs2) (uint32_t ctx_id, மாறிலி எழுத்து *c_tag, மாறிலி எழுத்து *c_path, uint64_t shm_size);
  int32_t (*krun_set_வேர்_disk) (uint32_t ctx_id, மாறிலி எழுத்து *disk_path);
  int32_t (*krun_set_tee_config_file) (uint32_t ctx_id, மாறிலி எழுத்து *file_path);
  int32_t (*krun_set_console_output) (uint32_t ctx_id, மாறிலி எழுத்து *c_filepath);
  int32_t (*krun_set_exec) (uint32_t ctx_id, மாறிலி எழுத்து *exec_path,
                            மாறிலி எழுத்து *மாறிலி argv[], மாறிலி எழுத்து *மாறிலி envp[]);
  கட்டமைப்பு krun_config *kconf = (கட்டமைப்பு krun_config *) cookie;
  வெறுமை *handle;
  int32_t ctx_id, ret;
  libcrun_error_t err;

  ret = libkrun_configure_flavor (cookie, container, &err);
  எனில் (UNLIKELY (ret < 0))
    {
      முழு errcode = crun_error_get_errno (&err);
      crun_error_release (&err);
      error (EXIT_FAILURE, errcode, "unable to configure libkrun flavor");
    }

  // /dev/kvm is required for all non AWS nitro workloads.
  எனில் (! kconf->awsnitro && ! kconf->has_kvm)
    error (EXIT_FAILURE, 0, "`/dev/kvm` unavailable");

  handle = kconf->handle;
  ctx_id = kconf->ctx_id;

  krun_set_log_level = dlsym (handle, "krun_set_log_level");
  krun_start_enter = dlsym (handle, "krun_start_enter");
  எனில் (krun_set_log_level == வெற்று || krun_start_enter == வெற்று)
    error (EXIT_FAILURE, 0, "could not find symbol in the krun library");

  /* Set log level according to crun's verbosity. */
  தேர்வு (libcrun_get_verbosity ())
    {
    நிலை LIBCRUN_VERBOSITY_DEBUG:
      krun_set_log_level (KRUN_LOG_LEVEL_DEBUG);
      நிறுத்து;
    நிலை LIBCRUN_VERBOSITY_WARNING:
      krun_set_log_level (KRUN_LOG_LEVEL_WARN);
      நிறுத்து;
    இயல்பு:
      krun_set_log_level (KRUN_LOG_LEVEL_ERROR);
      நிறுத்து;
    }

  எனில் (kconf->sev)
    {
      krun_set_வேர்_disk = dlsym (handle, "krun_set_root_disk");
      krun_set_tee_config_file = dlsym (handle, "krun_set_tee_config_file");
      எனில் (krun_set_வேர்_disk == வெற்று || krun_set_tee_config_file == வெற்று)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-sev.so`");

      ret = krun_set_வேர்_disk (ctx_id, "/disk.img");
      எனில் (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set root disk");

      ret = krun_set_tee_config_file (ctx_id, KRUN_SEV_FILE);
      எனில் (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set krun tee config file");
    }
  இல்லையெனில்
    {
      json_object *val_virtiofs_tag = வெற்று;
      json_object *val_virtiofs_shm_size = வெற்று;
      மாறிலி எழுத்து *virtiofs_tag = வெற்று;
      // Default to a conservative DAX size of 512MB, just like krun_set_root() does.
      uint64_t virtiofs_shm_size = 512 * 1024 * 1024ULL;

      எனில் (kconf->config_மரம் != வெற்று)
        {
          val_virtiofs_tag = json_object_object_get (kconf->config_மரம், "virtiofs_tag");
          எனில் (val_virtiofs_tag != வெற்று && json_object_is_type (val_virtiofs_tag, json_type_string))
            virtiofs_tag = json_object_get_string (val_virtiofs_tag);

          val_virtiofs_shm_size = json_object_object_get (kconf->config_மரம், "virtiofs_shm_size");
          எனில் (val_virtiofs_shm_size != வெற்று && json_object_is_type (val_virtiofs_shm_size, json_type_int))
            virtiofs_shm_size = json_object_get_uint64 (val_virtiofs_shm_size);
        }

      எனில் (virtiofs_tag == வெற்று)
        virtiofs_tag = "/dev/root";

      krun_add_virtiofs2 = dlsym (handle, "krun_add_virtiofs2");

      எனில் (krun_add_virtiofs2 == வெற்று)
        error (EXIT_FAILURE, 0, "could not find symbol `krun_add_virtiofs2` in `libkrun.so`");

      ret = krun_add_virtiofs2 (ctx_id, virtiofs_tag, "/", virtiofs_shm_size);
      எனில் (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not add virtiofs root with tag `%s`", virtiofs_tag);
    }

  எனில் (kconf->awsnitro)
    {
      krun_set_console_output = dlsym (handle, "krun_set_console_output");
      krun_set_exec = dlsym (handle, "krun_set_exec");
      எனில் (krun_set_console_output == வெற்று || krun_set_exec == வெற்று)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-awsnitro.so`");

      // Redirect all enclave output (read from vsock) to stdout.
      ret = krun_set_console_output (ctx_id, "/dev/stdout");
      எனில் (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not redirect enclave output to stdout");

      ret = krun_set_exec (ctx_id, pathname, (மாறிலி எழுத்து *மாறிலி *) argv,
                           (மாறிலி எழுத்து *மாறிலி *) def->process->env);
      எனில் (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set enclave execution arguments");
    }

  ret = libkrun_configure_vm (ctx_id, handle, kconf, container, &err);
  எனில் (UNLIKELY (ret))
    {
      முழு errcode = crun_error_get_errno (&err);
      libcrun_error_t *tmp_err = &err;
      libcrun_error_report_and_release (tmp_err);
      error (EXIT_FAILURE, errcode, "could not configure krun vm");
    }

  json_object_put (kconf->config_doc);

  ret = krun_start_enter (ctx_id);
  எனில் (UNLIKELY (ret < 0))
    error (EXIT_FAILURE, -ret, "could not start krun");

  திரும்பு ret;
}

நிலையான முழு
libkrun_start_render_server (கட்டமைப்பு krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  முழு ret;

  kconf->gpu_flags = libkrun_parse_resource_configuration (kconf->config_மரம், container, "krun.gpu_flags", "gpu_flags", false);
  எனில் (kconf->gpu_flags > 0)
    {
      எனில் (access ("/dev/dri", F_OK) != 0)
        திரும்பு crun_make_error (err, errno, "gpu requested but /dev/dri is not available");

      எனில் ((kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER) != 0)
        {
          எழுத்து *rs_argv[40];
          முழு rs_argc = 0;
          எழுத்து fd_as_str[16];
          முழு gpu_fds[2];
          pid_t pid;
          முழு null;

          எனில் (access ("/usr/libexec/virgl_render_server", X_OK) != 0)
            திரும்பு crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/libexec/virgl_render_server binary");

          எனில் (access ("/usr/bin/bwrap", X_OK) != 0)
            திரும்பு crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/bin/bwrap binary");

          ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, gpu_fds);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "could not create socketpair for virgl_render_server");

          snprintf (fd_as_str, அளவு (fd_as_str), "%d", gpu_fds[FD_PAIR_CHILD]);

          rs_argv[rs_argc++] = (எழுத்து *) "bwrap";
          rs_argv[rs_argc++] = (எழுத்து *) "--ro-bind";
          rs_argv[rs_argc++] = (எழுத்து *) "/usr";
          rs_argv[rs_argc++] = (எழுத்து *) "/usr";
          எனில் (access ("/lib", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (எழுத்து *) "--ro-bind";
              rs_argv[rs_argc++] = (எழுத்து *) "/lib";
              rs_argv[rs_argc++] = (எழுத்து *) "/lib";
            }
          எனில் (access ("/lib64", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (எழுத்து *) "--ro-bind";
              rs_argv[rs_argc++] = (எழுத்து *) "/lib64";
              rs_argv[rs_argc++] = (எழுத்து *) "/lib64";
            }
          rs_argv[rs_argc++] = (எழுத்து *) "--ro-bind";
          rs_argv[rs_argc++] = (எழுத்து *) "/etc";
          rs_argv[rs_argc++] = (எழுத்து *) "/etc";
          rs_argv[rs_argc++] = (எழுத்து *) "--ro-bind";
          rs_argv[rs_argc++] = (எழுத்து *) "/sys";
          rs_argv[rs_argc++] = (எழுத்து *) "/sys";
          rs_argv[rs_argc++] = (எழுத்து *) "--dev";
          rs_argv[rs_argc++] = (எழுத்து *) "/dev";
          rs_argv[rs_argc++] = (எழுத்து *) "--dev-bind";
          rs_argv[rs_argc++] = (எழுத்து *) "/dev/dri";
          rs_argv[rs_argc++] = (எழுத்து *) "/dev/dri";
          rs_argv[rs_argc++] = (எழுத்து *) "--proc";
          rs_argv[rs_argc++] = (எழுத்து *) "/proc";
          rs_argv[rs_argc++] = (எழுத்து *) "--tmpfs";
          rs_argv[rs_argc++] = (எழுத்து *) "/tmp";
          rs_argv[rs_argc++] = (எழுத்து *) "--unshare-pid";
          rs_argv[rs_argc++] = (எழுத்து *) "--unshare-ipc";
          rs_argv[rs_argc++] = (எழுத்து *) "--unshare-net";
          rs_argv[rs_argc++] = (எழுத்து *) "--";
          rs_argv[rs_argc++] = (எழுத்து *) "/usr/libexec/virgl_render_server";
          rs_argv[rs_argc++] = (எழுத்து *) "--socket-fd";
          rs_argv[rs_argc++] = fd_as_str;
          rs_argv[rs_argc] = வெற்று;

          pid = fork ();
          எனில் (pid < 0)
            {
              close (gpu_fds[FD_PAIR_PARENT]);
              close (gpu_fds[FD_PAIR_CHILD]);
              திரும்பு crun_make_error (err, errno, "could not fork for virgl_render_server");
            }
          இல்லையெனில் எனில் (pid == 0)
            {
              close (gpu_fds[FD_PAIR_PARENT]);

              null = open ("/dev/null", O_WRONLY);
              எனில் (null == -1)
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

  திரும்பு 0;
}

நிலையான முழு
libkrun_configure_network (வெறுமை *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  கட்டமைப்பு krun_config *kconf = (கட்டமைப்பு krun_config *) cookie;
  pid_t pid;
  எழுத்து *passt_argv[10];
  எழுத்து fd_as_str[16];
  முழு use_passt;
  முழு argv_idx;
  முழு status;
  முழு null;
  முழு ret;

  ret = libkrun_parse_string_configuration (kconf->config_மரம், container,
                                            "krun.tap_name", "tap_name",
                                            &kconf->tap_name, err, false);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (kconf->tap_name != வெற்று && is_empty_string (kconf->tap_name))
    திரும்பு crun_make_error (err, 0, "krun.tap_name cannot be empty");

  use_passt = libkrun_parse_resource_configuration (kconf->config_மரம், container, "krun.use_passt", "use_passt", false);

  எனில் (use_passt > 0)
    {
      எனில் (kconf->tap_name != வெற்று)
        திரும்பு crun_make_error (err, 0, "krun.tap_name and krun.use_passt are mutually exclusive");

      kconf->use_passt = 1;
    }
  இல்லையெனில்
    திரும்பு 0;

  ret = socketpair (AF_UNIX, SOCK_STREAM, 0, kconf->passt_fds);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "create passt socketpair");

  snprintf (fd_as_str, அளவு (fd_as_str), "%d", kconf->passt_fds[FD_PAIR_CHILD]);

  argv_idx = 0;
  passt_argv[argv_idx++] = (எழுத்து *) "passt";
  passt_argv[argv_idx++] = (எழுத்து *) "-t";
  passt_argv[argv_idx++] = (எழுத்து *) "all";

  எனில் (! kconf->has_awsnitro)
    {
      passt_argv[argv_idx++] = (எழுத்து *) "-u";
      passt_argv[argv_idx++] = (எழுத்து *) "all";
      passt_argv[argv_idx++] = (எழுத்து *) "--no-dhcp-dns";
    }

  /* Set --no-map-gw. If the gateway for the network is also the DNS server
   * we won't be able to query the DNS server without --no-map-gw.
   * See discussion in https://github.com/containers/crun/pull/2099
   */
  passt_argv[argv_idx++] = (எழுத்து *) "--no-map-gw";

  passt_argv[argv_idx++] = (எழுத்து *) "--fd";
  passt_argv[argv_idx++] = fd_as_str;
  passt_argv[argv_idx] = வெற்று;

  pid = fork ();
  எனில் (pid < 0)
    திரும்பு crun_make_error (err, errno, "fork passt");
  இல்லையெனில் எனில் (pid == 0)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t i;

      close (kconf->passt_fds[FD_PAIR_PARENT]);

      null = open ("/dev/null", O_WRONLY);
      எனில் (null == -1)
        _exit (EXIT_FAILURE);

      /* Remount /proc if the parent detached its PID namespace, as passt(1)
       * needs a consistent /proc view to operate properly.
       */
      க்கு (i = 0; i < def->linux->namespaces_len; i++)
        {
          முழு மதிப்பு = libcrun_find_namespace (def->linux->namespaces[i]->type);

          எனில் (மதிப்பு == CLONE_NEWPID)
            {
              எனில் (mount ("", "/proc", "proc", MS_NOSUID | MS_NODEV | MS_NOEXEC, வெற்று))
                _exit (EXIT_FAILURE);

              நிறுத்து;
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
  எனில் (! (WIFEXITED (status)) || WEXITSTATUS (status) != 0)
    திரும்பு crun_make_error (err, 0, "start passt");

  திரும்பு 0;
}

/* Return true if the spec already declares a device with the given PATH.  */
நிலையான bool
spec_has_device (runtime_spec_schema_config_schema *def, மாறிலி எழுத்து *path)
{
  size_t i;

  க்கு (i = 0; i < def->linux->devices_len; i++)
    எனில் (strcmp (def->linux->devices[i]->path, path) == 0)
      திரும்பு true;
  திரும்பு false;
}

/* libkrun_create_kvm_device: explicitly adds kvm device.  */
நிலையான முழு
libkrun_configure_container (வெறுமை *cookie, எண்ணிக்கை handler_configure_phase phase,
                             libcrun_context_t *context, libcrun_container_t *container,
                             மாறிலி எழுத்து *rootfs, libcrun_error_t *err)
{
  முழு ret, rootfsfd;
  கட்டமைப்பு krun_config *kconf = (கட்டமைப்பு krun_config *) cookie;
  கட்டமைப்பு device_s kvm_device = { "/dev/kvm", "c", 10, 232, 0666, 0, 0 };
  கட்டமைப்பு device_s sev_device = { "/dev/sev", "c", 10, 124, 0666, 0, 0 };
  கட்டமைப்பு device_s nitro_device = { "/dev/nitro_enclaves", "c", 10, 122, 0666, 0, 0 };
  cleanup_close முழு devfd = -1;
  cleanup_close முழு rootfsfd_cleanup = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  bool create_sev = false, create_awsnitro = false;
  bool is_user_ns;

  எனில் (rootfs == வெற்று)
    rootfsfd = AT_FDCWD;
  இல்லையெனில்
    {
      rootfsfd = rootfsfd_cleanup = open (rootfs, O_PATH | O_CLOEXEC);
      எனில் (UNLIKELY (rootfsfd < 0))
        திரும்பு crun_make_error (err, errno, "open `%s`", rootfs);
    }

  எனில் (phase == HANDLER_CONFIGURE_BEFORE_USERNS)
    {
      cleanup_free எழுத்து *origin_config_path = வெற்று;
      cleanup_free எழுத்து *state_dir = வெற்று;
      cleanup_free எழுத்து *config = வெற்று;
      cleanup_close முழு fd = -1;
      size_t config_size;

      ret = libcrun_get_state_directory (&state_dir, context->state_வேர், context->id, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = append_paths (&origin_config_path, err, state_dir, "config.json", வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = read_all_file (origin_config_path, &config, &config_size, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      /* CVE-2025-24965: the content below rootfs cannot be trusted because it is controlled by the user.  We
         must ensure the file is opened below the rootfs directory.  Whatever the image ships at that name
         could also be a device node or a FIFO, so never open it: unlink it and create a new regular file.  */
      ret = unlinkat (rootfsfd, KRUN_CONFIG_FILE, 0);
      எனில் (UNLIKELY (ret < 0 && errno != ENOENT))
        திரும்பு crun_make_error (err, errno, "unlink `%s`", KRUN_CONFIG_FILE);

      fd = safe_openat (rootfsfd, rootfs, KRUN_CONFIG_FILE, WRITE_FILE_DEFAULT_FLAGS | O_EXCL | O_NOFOLLOW, S_IRUSR | S_IRGRP | S_IROTH, err);
      எனில் (UNLIKELY (fd < 0))
        திரும்பு fd;

      ret = safe_write (fd, KRUN_CONFIG_FILE, config, config_size, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = libkrun_read_vm_config (kconf, rootfsfd, rootfs, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = libkrun_start_render_server (kconf, container, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    திரும்பு 0;

  ret = libkrun_configure_network (cookie, container, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  /* Do nothing if /dev/kvm is already present in spec */
  எனில் (spec_has_device (def, "/dev/kvm"))
    திரும்பு 0;

  எனில் (kconf->handle_sev != வெற்று)
    create_sev = ! spec_has_device (def, "/dev/sev");

  எனில் (kconf->handle_awsnitro != வெற்று)
    create_awsnitro = ! spec_has_device (def, "/dev/nitro_enclaves");

  devfd = safe_openat (rootfsfd, rootfs, "dev", O_PATH | O_DIRECTORY | O_CLOEXEC, 0, err);
  எனில் (UNLIKELY (devfd < 0))
    திரும்பு devfd;

  ret = check_running_in_user_namespace (err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;
  is_user_ns = ret;

  எனில் (kconf->has_kvm)
    {
      ret = libcrun_create_dev (container, devfd, -1, &kvm_device, is_user_ns, true, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (create_sev)
    {
      ret = libcrun_create_dev (container, devfd, -1, &sev_device, is_user_ns, true, err);
      எனில் (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_sev);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
          kconf->handle_sev = வெற்று;
        }
    }

  எனில் (create_awsnitro)
    {
      ret = libcrun_create_dev (container, devfd, -1, &nitro_device, is_user_ns, true, err);
      எனில் (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_awsnitro);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
          kconf->handle_awsnitro = வெற்று;
        }
    }

  திரும்பு 0;
}

நிலையான முழு
libkrun_load (வெறுமை **cookie, libcrun_error_t *err)
{
  int32_t ret;
  கட்டமைப்பு krun_config *kconf;
  மாறிலி எழுத்து *libkrun_so = "libkrun.so.1";
  மாறிலி எழுத்து *libkrun_sev_so = "libkrun-sev.so.1";
  மாறிலி எழுத்து *libkrun_awsnitro_so = "libkrun-awsnitro.so.1";

  kconf = malloc (அளவு (கட்டமைப்பு krun_config));
  எனில் (kconf == வெற்று)
    திரும்பு crun_make_error (err, 0, "could not allocate memory for krun_config");
  memset (kconf, 0, அளவு (கட்டமைப்பு krun_config));

  kconf->gpu_render_server_fd = -1;

  kconf->handle = dlopen (libkrun_so, RTLD_NOW);
  kconf->handle_sev = dlopen (libkrun_sev_so, RTLD_NOW);
  kconf->handle_awsnitro = dlopen (libkrun_awsnitro_so, RTLD_NOW);

  எனில் (kconf->handle == வெற்று && kconf->handle_sev == வெற்று && kconf->handle_awsnitro == வெற்று)
    {
      விடுவி (kconf);
      திரும்பு crun_make_error (err, 0, "failed to open `%s`, `%s`, and `%s` for krun_config: %s", libkrun_so, libkrun_sev_so, libkrun_awsnitro_so, dlerror ());
    }

  kconf->sev = false;
  kconf->awsnitro = false;

  /* Newer versions of libkrun no longer link against libkrunfw and
     instead they open it when creating the context. This implies
     we need to call "krun_create_ctx" before switching namespaces
     or it won't be able to find the library bundling the kernel. */
  எனில் (kconf->handle)
    {
      ret = libkrun_create_context (kconf->handle, err);
      எனில் (UNLIKELY (ret < 0))
        செல் error;
      kconf->ctx_id = ret;
    }

  எனில் (kconf->handle_sev)
    {
      ret = libkrun_create_context (kconf->handle_sev, err);
      எனில் (UNLIKELY (ret < 0))
        செல் error;
      kconf->ctx_id_sev = ret;
    }
  எனில் (kconf->handle_awsnitro)
    {
      ret = libkrun_create_context (kconf->handle_awsnitro, err);
      எனில் (UNLIKELY (ret < 0))
        செல் error;
      kconf->ctx_id_awsnitro = ret;
    }

  *cookie = kconf;

  திரும்பு 0;

error:
  எனில் (kconf->handle)
    dlclose (kconf->handle);
  எனில் (kconf->handle_sev)
    dlclose (kconf->handle_sev);
  எனில் (kconf->handle_awsnitro)
    dlclose (kconf->handle_awsnitro);
  விடுவி (kconf);
  திரும்பு ret;
}

நிலையான முழு
libkrun_unload (வெறுமை *cookie, libcrun_error_t *err)
{
  முழு r;

  கட்டமைப்பு krun_config *kconf = (கட்டமைப்பு krun_config *) cookie;
  எனில் (kconf != வெற்று)
    {
      எனில் (kconf->handle != வெற்று)
        {
          r = dlclose (kconf->handle);
          எனில் (UNLIKELY (r != 0))
            திரும்பு crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
        }
      எனில் (kconf->handle_sev != வெற்று)
        {
          r = dlclose (kconf->handle_sev);
          எனில் (UNLIKELY (r != 0))
            திரும்பு crun_make_error (err, 0, "could not unload handle_sev: `%s`", dlerror ());
        }
      எனில் (kconf->handle_awsnitro != வெற்று)
        {
          r = dlclose (kconf->handle_awsnitro);
          எனில் (UNLIKELY (r != 0))
            திரும்பு crun_make_error (err, 0, "could not unload handle_awsnitro: `%s`", dlerror ());
        }
      விடுவி (kconf);
    }
  திரும்பு 0;
}

நிலையான runtime_spec_schema_defs_linux_device_cgroup *
make_oci_spec_dev (மாறிலி எழுத்து *type, dev_t device, bool allow, மாறிலி எழுத்து *access)
{
  runtime_spec_schema_defs_linux_device_cgroup *dev = xmalloc0 (அளவு (*dev));

  dev->allow = allow;
  dev->allow_present = 1;

  dev->type = xstrdup (type);

  dev->major = major (device);
  dev->major_present = 1;

  dev->minor = minor (device);
  dev->minor_present = 1;

  dev->access = xstrdup (access);

  திரும்பு dev;
}

/* stat an optional device: on ENOENT clear *PRESENT, any other error fails.  */
நிலையான முழு
stat_optional_device (மாறிலி எழுத்து *path, கட்டமைப்பு stat *st, bool *present, libcrun_error_t *err)
{
  முழு ret = stat (path, st);
  எனில் (UNLIKELY (ret < 0))
    {
      எனில் (errno != ENOENT)
        திரும்பு crun_make_error (err, errno, "stat `%s`", path);
      *present = false;
    }
  திரும்பு 0;
}

நிலையான முழு
libkrun_modify_oci_configuration (வெறுமை *cookie arg_unused, libcrun_context_t *context arg_unused,
                                  runtime_spec_schema_config_schema *def,
                                  libcrun_error_t *err)
{
  மாறிலி size_t device_size = அளவு (runtime_spec_schema_defs_linux_device_cgroup);
  கட்டமைப்பு krun_config *kconf = (கட்டமைப்பு krun_config *) cookie;
  கட்டமைப்பு stat st_kvm, st_sev, st_awsnitro;
  bool has_kvm = true, has_sev = true, has_awsnitro = true;
  size_t old_len, new_len;
  முழு ret;

  /* Always allow the /dev/kvm device.  */

  ret = stat_optional_device ("/dev/kvm", &st_kvm, &has_kvm, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = stat_optional_device ("/dev/sev", &st_sev, &has_sev, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = stat_optional_device ("/dev/nitro_enclaves", &st_awsnitro, &has_awsnitro, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  kconf->has_kvm = has_kvm;
  kconf->has_awsnitro = has_awsnitro;

  எனில் (! has_kvm && ! has_awsnitro)
    திரும்பு 0;

  /* spec says these are optional, ensure they exist so we can add our devices */
  எனில் (def->linux == வெற்று)
    def->linux = xmalloc0 (அளவு (runtime_spec_schema_config_linux));

  எனில் (def->linux->resources == வெற்று)
    def->linux->resources = xmalloc0 (அளவு (runtime_spec_schema_config_linux_resources));

  old_len = def->linux->resources->devices_len;
  new_len = old_len;
  எனில் (has_kvm)
    new_len += has_sev ? 2 : 1;
  எனில் (has_awsnitro)
    new_len += 1;

  def->linux->resources->devices = xrealloc (def->linux->resources->devices, device_size * (new_len + 1));
  def->linux->resources->devices_len = new_len;

  எனில் (has_kvm)
    {
      def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_kvm.st_rdev, true, "rwm");
      எனில் (has_sev)
        def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_sev.st_rdev, true, "rwm");
    }

  எனில் (has_awsnitro)
    def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_awsnitro.st_rdev, true, "rwm");

  திரும்பு 0;
}

நிலையான முழு
libkrun_close_fds (வெறுமை *cookie, libcrun_container_t *container, முழு preserve_fds, libcrun_error_t *err)
{
  கட்டமைப்பு krun_config *kconf = (கட்டமைப்பு krun_config *) cookie;
  முழு first_fd_to_close = preserve_fds + 3;
  முழு கீழ்_fd, உயர்வு_fd;
  முழு i;

  கீழ்_fd = உயர்வு_fd = -1;

  எனில் (kconf->use_passt)
    உயர்வு_fd = kconf->passt_fds[FD_PAIR_PARENT];

  எனில் (kconf->gpu_render_server_fd != -1)
    {
      எனில் (kconf->gpu_render_server_fd > உயர்வு_fd)
        {
          கீழ்_fd = உயர்வு_fd;
          உயர்வு_fd = kconf->gpu_render_server_fd;
        }
      இல்லையெனில்
        கீழ்_fd = kconf->gpu_render_server_fd;
    }

  எனில் (first_fd_to_close <= உயர்வு_fd)
    {
      க்கு (i = first_fd_to_close; i < உயர்வு_fd; i++)
        {
          // If we're closing proc_fd, make sure to invalidate it.
          எனில் (i == container->proc_fd)
            container->proc_fd = -1;
          இல்லையெனில் எனில் (i == கீழ்_fd)
            தொடர்;
          close (i);
        }

      first_fd_to_close = உயர்வு_fd + 1;
    }

  திரும்பு mark_or_close_fds_ge_than (container, first_fd_to_close, true, err);
}

கட்டமைப்பு custom_handler_s handler_libkrun = {
  .name = "krun",
  .alias = வெற்று,
  .feature_string = "LIBKRUN",
  .supports_open_மரம்_namespace = false,
  .load = libkrun_load,
  .unload = libkrun_unload,
  .run_func = libkrun_exec,
  .configure_container = libkrun_configure_container,
  .modify_oci_configuration = libkrun_modify_oci_configuration,
  .close_fds = libkrun_close_fds,
};

#endif
