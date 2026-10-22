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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "../custom-handler.h"
#ಅನ್ತರ್ಭಾವ "../container.h"
#ಅನ್ತರ್ಭಾವ "../utils.h"
#ಅನ್ತರ್ಭಾವ "../linux.h"
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/socket.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <sys/mount.h>
#ಅನ್ತರ್ಭಾವ <sys/param.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <sys/sysmacros.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <sched.h>
#ಅನ್ತರ್ಭಾವ <ocispec/runtime_spec_schema_config_schema.h>

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

ಸಂರಚನಾ krun_config
{
  ಶೂನ್ಯ *handle;
  ಶೂನ್ಯ *handle_sev;
  ಶೂನ್ಯ *handle_awsnitro;
  bool sev;
  bool awsnitro;
  int32_t ctx_id;
  int32_t ctx_id_sev;
  int32_t ctx_id_awsnitro;
  bool has_kvm;
  bool has_awsnitro;
  ಪೂರ್ಣಾಙ್ಕ passt_fds[2];
  json_object *config_doc;
  json_object *config_ವೃಕ್ಷ;
  bool use_passt;
  ಧ್ರುವ ವರ್ಣ *tap_name;
  ಪೂರ್ಣಾಙ್ಕ gpu_render_server_fd;
  ಪೂರ್ಣಾಙ್ಕ gpu_flags;
};

/* libkrun handler.  */
#ಯದಿ HAVE_DLOPEN && HAVE_LIBKRUN
ಸ್ಥಿರ int32_t
libkrun_create_context (ಶೂನ್ಯ *handle, libcrun_error_t *err)
{
  int32_t (*krun_create_ctx) ();
  int32_t ctx_id;

  krun_create_ctx = dlsym (handle, "krun_create_ctx");
  ಯದಿ (krun_create_ctx == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not find symbol in the krun library");

  ctx_id = krun_create_ctx ();
  ಯದಿ (UNLIKELY (ctx_id < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ctx_id, "could not create krun context");

  ಪ್ರತ್ಯಾಗಮ ctx_id;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libkrun_configure_kernel (uint32_t ctx_id, ಶೂನ್ಯ *handle, json_object *config_ವೃಕ್ಷ, libcrun_error_t *err)
{
  int32_t (*krun_set_kernel) (uint32_t ctx_id, ಧ್ರುವ ವರ್ಣ *kernel_path,
                              uint32_t kernel_format, ಧ್ರುವ ವರ್ಣ *initrd_path, ಧ್ರುವ ವರ್ಣ *kernel_cmdline);
  json_object *kernel_path = ರಿಕ್ತ;
  json_object *kernel_format = ರಿಕ್ತ;
  json_object *val_initrd_path = ರಿಕ್ತ;
  json_object *val_kernel_cmdline = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *initrd_path = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *kernel_cmdline = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  /* kernel_path and kernel_format must be present */
  kernel_path = json_object_object_get (config_ವೃಕ್ಷ, "kernel_path");
  ಯದಿ (kernel_path == ರಿಕ್ತ || ! json_object_is_type (kernel_path, json_type_string))
    ಪ್ರತ್ಯಾಗಮ 0;

  kernel_format = json_object_object_get (config_ವೃಕ್ಷ, "kernel_format");
  ಯದಿ (kernel_format == ರಿಕ್ತ || ! json_object_is_type (kernel_format, json_type_int))
    ಪ್ರತ್ಯಾಗಮ 0;

  /* initrd and kernel_cmdline are optional */
  val_initrd_path = json_object_object_get (config_ವೃಕ್ಷ, "initrd_path");
  ಯದಿ (val_initrd_path != ರಿಕ್ತ && json_object_is_type (val_initrd_path, json_type_string))
    initrd_path = json_object_get_string (val_initrd_path);

  val_kernel_cmdline = json_object_object_get (config_ವೃಕ್ಷ, "kernel_cmdline");
  ಯದಿ (val_kernel_cmdline != ರಿಕ್ತ && json_object_is_type (val_kernel_cmdline, json_type_string))
    kernel_cmdline = json_object_get_string (val_kernel_cmdline);

  krun_set_kernel = dlsym (handle, "krun_set_kernel");
  ಯದಿ (krun_set_kernel == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not find symbol in krun library");

  ret = krun_set_kernel (ctx_id,
                         json_object_get_string (kernel_path),
                         (uint32_t) json_object_get_int64 (kernel_format),
                         initrd_path, kernel_cmdline);

  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "could not configure a krun external kernel");

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libkrun_read_vm_config (ಸಂರಚನಾ krun_config *kconf, ಪೂರ್ಣಾಙ್ಕ rootfsfd, ಧ್ರುವ ವರ್ಣ *rootfs, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  cleanup_free ವರ್ಣ *config = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ pathfd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
  proc_fd_path_t fd_path;
  ಸಂರಚನಾ stat st;

  /* The content of the rootfs is controlled by the user: look at the file through an
     O_PATH descriptor first and refuse anything that is not a small regular file, as
     opening a FIFO blocks and a device node must not be accessed.  */
  pathfd = safe_openat (rootfsfd, rootfs, KRUN_VM_FILE, O_PATH | O_CLOEXEC | O_NOFOLLOW, 0, err);
  ಯದಿ (pathfd < 0)
    {
      // The configuration file is optional, don't generate an error if it's missing.
      ಯದಿ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ಪ್ರತ್ಯಾಗಮ 0;
        }
      ಪ್ರತ್ಯಾಗಮ pathfd;
    }

  ret = fstat (pathfd, &st);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "stat `%s`", KRUN_VM_FILE);

  ಯದಿ (! S_ISREG (st.st_mode))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "`%s` is not a regular file", KRUN_VM_FILE);

  get_proc_self_fd_path (fd_path, pathfd);

  fd = open (fd_path, O_RDONLY | O_CLOEXEC);
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", KRUN_VM_FILE);

  ret = read_all_fd_with_max_size (fd, KRUN_VM_FILE, &config, ರಿಕ್ತ, KRUN_VM_FILE_MAX_SIZE, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = parse_json_file (&kconf->config_doc, config, ರಿಕ್ತ, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  kconf->config_ವೃಕ್ಷ = kconf->config_doc;
  ಪ್ರತ್ಯಾಗಮ 0;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libkrun_parse_resource_configuration (json_object *config_ವೃಕ್ಷ, libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *annotation, ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ, bool use_config_file)
{
  ವರ್ಣ *val_str, *endptr;
  ಪೂರ್ಣಾಙ್ಕ val = -1;
  json_object *val_json = ರಿಕ್ತ;

  val_str = (ವರ್ಣ *) find_annotation (container, annotation);
  ಯದಿ (val_str != ರಿಕ್ತ)
    {
      errno = 0;
      val = (ಪೂರ್ಣಾಙ್ಕ) strtol (val_str, &endptr, 10);
      ಯದಿ (errno != 0 || endptr == val_str || *endptr != '\0')
        /* Annotations value is not a valid integer. */
        error (EXIT_FAILURE, 0, "krun annotation %s value cannot be converted to an integer", annotation);

      ಯದಿ (val < 0)
        error (EXIT_FAILURE, 0, "krun annotation %s value must be a positive integer", annotation);

      ಪ್ರತ್ಯಾಗಮ val;
    }
  ಅನ್ಯಥಾ ಯದಿ (use_config_file && config_ವೃಕ್ಷ != ರಿಕ್ತ)
    {
      val_json = json_object_object_get (config_ವೃಕ್ಷ, ಕೀಲಿ);
      ಯದಿ (val_json == ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ val;
      ಯದಿ (! json_object_is_type (val_json, json_type_int))
        error (EXIT_FAILURE, 0, "krun krun_vm.json %s value is not an integer", ಕೀಲಿ);

      val = (ಪೂರ್ಣಾಙ್ಕ) json_object_get_int64 (val_json);
    }

  ಪ್ರತ್ಯಾಗಮ val;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libkrun_parse_string_configuration (json_object *config_ವೃಕ್ಷ, libcrun_container_t *container,
                                    ಧ್ರುವ ವರ್ಣ *annotation, ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ,
                                    ಧ್ರುವ ವರ್ಣ **ಮೌಲ್ಯ, libcrun_error_t *err, bool use_config_file)
{
  ಧ್ರುವ ವರ್ಣ *val;
  json_object *val_json = ರಿಕ್ತ;

  *ಮೌಲ್ಯ = ರಿಕ್ತ;

  val = find_annotation (container, annotation);
  ಯದಿ (val != ರಿಕ್ತ)
    {
      *ಮೌಲ್ಯ = val;
      ಪ್ರತ್ಯಾಗಮ 0;
    }

  ಯದಿ (! use_config_file || config_ವೃಕ್ಷ == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  val_json = json_object_object_get (config_ವೃಕ್ಷ, ಕೀಲಿ);
  ಯದಿ (val_json == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯದಿ (! json_object_is_type (val_json, json_type_string))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, ".krun_vm.json %s value is not a string", ಕೀಲಿ);

  *ಮೌಲ್ಯ = json_object_get_string (val_json);
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ
libkrun_make_tap_mac (ಧ್ರುವ ವರ್ಣ *id, uint8_t mac[6])
{
  uint64_t hash = 14695981039346656037ULL;
  size_t i;

  ಪ್ರತಿ (; *id != '\0'; id++)
    {
      hash ^= (uint8_t) *id;
      hash *= 1099511628211ULL;
    }

  ಪ್ರತಿ (i = 0; i < 6; i++)
    mac[i] = (uint8_t) (hash >> (i * 8));

  mac[0] = (mac[0] & 0xfe) | 0x02;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libkrun_configure_vm (uint32_t ctx_id, ಶೂನ್ಯ *handle, ಸಂರಚನಾ krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_set_vm_config) (uint32_t ctx_id, uint8_t ಅಂಕೆ_vcpus, uint32_t ram_mib);
  int32_t (*krun_add_net_unixstream) (uint32_t ctx_id, ಧ್ರುವ ವರ್ಣ *c_path, ಪೂರ್ಣಾಙ್ಕ fd, uint8_t *ಧ್ರುವ c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_add_net_tap) (uint32_t ctx_id, ಧ್ರುವ ವರ್ಣ *c_tap_name, ಧ್ರುವ uint8_t *c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_set_gpu_options) (uint32_t ctx_id, uint32_t virgl_flags);
  int32_t (*krun_set_gpu_render_server_fd) (uint32_t ctx_id, ಪೂರ್ಣಾಙ್ಕ fd);
  ಪೂರ್ಣಾಙ್ಕ cpus, ram_mib, nested_virt, ret;
  cpu_set_t set;

  /* We let the OCI set the number of vCPUs for the VM, since cgroups CPU restrictions still apply. */
  cpus = libkrun_parse_resource_configuration (kconf->config_ವೃಕ್ಷ, container, "krun.cpus", "cpus", true);
  ಯದಿ (cpus <= 0)
    {
      CPU_ZERO (&set);
      ಯದಿ (sched_getaffinity (getpid (), ಪರಿಮಾಣ (set), &set) == 0)
        cpus = MIN (CPU_ಎಣಿಕೆ (&set), LIBKRUN_MAX_VCPUS);
      ಅನ್ಯಥಾ
        cpus = 1;
    }

  /* We let the OCI set the amount of RAM for the VM, since cgroups memory restrictions still apply. */
  ram_mib = libkrun_parse_resource_configuration (kconf->config_ವೃಕ್ಷ, container, "krun.ram_mib", "ram_mib", true);
  ಯದಿ (ram_mib <= LIBKRUN_MINIMUM_RAM_MIB)
    {
      ಯದಿ (def && def->linux && def->linux->resources && def->linux->resources->memory
          && def->linux->resources->memory->limit_present)
        ram_mib = def->linux->resources->memory->limit / (1024 * 1024);
      ಅನ್ಯಥಾ
        ram_mib = LIBKRUN_DEFAULT_RAM_MIB;
    }

  krun_set_vm_config = dlsym (handle, "krun_set_vm_config");

  ಯದಿ (krun_set_vm_config == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not find symbol in the krun library");

  ret = krun_set_vm_config (ctx_id, cpus, ram_mib);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "could not set krun vm configuration");

  nested_virt = libkrun_parse_resource_configuration (kconf->config_ವೃಕ್ಷ, container, "krun.nested_virt", "nested_virt", false);
  ಯದಿ (nested_virt > 0)
    {
      int32_t (*krun_check_nested_virt) (ಶೂನ್ಯ);
      int32_t (*krun_set_nested_virt) (uint32_t ctx_id, bool enabled);

      krun_check_nested_virt = dlsym (handle, "krun_check_nested_virt");
      ಯದಿ (krun_check_nested_virt != ರಿಕ್ತ && krun_check_nested_virt () != 1)
        libcrun_warning ("nested virtualization requested but may not be supported on this host");

      krun_set_nested_virt = dlsym (handle, "krun_set_nested_virt");
      ಯದಿ (krun_set_nested_virt == ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not find symbol `krun_set_nested_virt` in the krun library");

      ret = krun_set_nested_virt (ctx_id, true);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "could not enable nested virtualization");
    }

  ಯದಿ (kconf->tap_name != ರಿಕ್ತ)
    {
      krun_add_net_tap = dlsym (handle, "krun_add_net_tap");
      ಯದಿ (krun_add_net_tap == ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not find symbol `krun_add_net_tap` in the krun library");

      uint8_t mac[6];
      libkrun_make_tap_mac (container->context->id, mac);
      ret = krun_add_net_tap (ctx_id, kconf->tap_name, &mac[0], COMPAT_NET_FEATURES, 0);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "could not add krun TAP interface `%s`", kconf->tap_name);
    }
  ಅನ್ಯಥಾ ಯದಿ (kconf->use_passt)
    {
      krun_add_net_unixstream = dlsym (handle, "krun_add_net_unixstream");
      ಯದಿ (krun_add_net_unixstream == ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not find symbol `krun_add_net_unixstream` in the krun library");

      uint8_t mac[] = { 0x5a, 0x94, 0xef, 0xe4, 0x0c, 0xee };
      ret = krun_add_net_unixstream (ctx_id, ರಿಕ್ತ, kconf->passt_fds[FD_PAIR_PARENT], &mac[0], COMPAT_NET_FEATURES, NET_FLAG_DHCP_CLIENT);
      ಯದಿ (UNLIKELY (ret == -EINVAL))
        ret = krun_add_net_unixstream (ctx_id, ರಿಕ್ತ, kconf->passt_fds[FD_PAIR_PARENT], &mac[0], COMPAT_NET_FEATURES, 0);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "could not set krun net configuration");
    }

  ಯದಿ (kconf->gpu_flags > 0)
    {
      krun_set_gpu_options = dlsym (kconf->handle, "krun_set_gpu_options");
      ಯದಿ (krun_set_gpu_options == ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "gpu requested but the version of libkrun in this system does not support it");

      ret = krun_set_gpu_options (kconf->ctx_id, kconf->gpu_flags);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "gpu requested but could not configure virtio gpu device");

      ಯದಿ (kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER)
        {
          krun_set_gpu_render_server_fd = dlsym (kconf->handle, "krun_set_gpu_render_server_fd");
          ಯದಿ (krun_set_gpu_render_server_fd == ರಿಕ್ತ)
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "gpu with render server requested but the version of libkrun in this system does not support it");

          ret = krun_set_gpu_render_server_fd (kconf->ctx_id, kconf->gpu_render_server_fd);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "gpu with render server requested but could not configure its file descriptor");
        }
    }

  ಯದಿ (kconf->config_ವೃಕ್ಷ != ರಿಕ್ತ)
    {
      /* Try to configure an external kernel. If the configuration file doesn't
       * specify a kernel, libkrun automatically fall back to using libkrunfw,
       * if the library is present and was loaded while creating the context.
       */
      ಪೂರ್ಣಾಙ್ಕ custom_kernel = libkrun_parse_resource_configuration (kconf->config_ವೃಕ್ಷ, container, "krun.custom_kernel", "custom_kernel", false);
      ಯದಿ (custom_kernel > 0)
        {
          ret = libkrun_configure_kernel (ctx_id, handle, kconf->config_ವೃಕ್ಷ, err);
          ಯದಿ (UNLIKELY (ret))
            ಪ್ರತ್ಯಾಗಮ ret;
        }
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libkrun_configure_flavor (ಶೂನ್ಯ *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret, sev_indicated = 0, awsnitro_indicated = 0;
  ಸಂರಚನಾ krun_config *kconf = (ಸಂರಚನಾ krun_config *) cookie;
  ಧ್ರುವ ವರ್ಣ *flavor = ರಿಕ್ತ;
  ಶೂನ್ಯ *close_handles[2];

  close_handles[0] = ರಿಕ್ತ;
  close_handles[1] = ರಿಕ್ತ;

  // Check if the user provided the krun variant through OCI annotations.
  flavor = find_annotation (container, "krun.variant");
  ಯದಿ (flavor != ರಿಕ್ತ)
    {
      sev_indicated |= strcmp (flavor, KRUN_FLAVOR_SEV) == 0;
      awsnitro_indicated |= strcmp (flavor, KRUN_FLAVOR_AWS_NITRO) == 0;
    }

  ಯದಿ (sev_indicated)
    {
      ಯದಿ (kconf->handle_sev == ರಿಕ್ತ)
        error (EXIT_FAILURE, 0, "the container requires libkrun-sev but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_awsnitro;

      kconf->handle = kconf->handle_sev;
      kconf->ctx_id = kconf->ctx_id_sev;
      kconf->sev = true;
    }
  ಅನ್ಯಥಾ ಯದಿ (awsnitro_indicated)
    {
      ಯದಿ (kconf->handle_awsnitro == ರಿಕ್ತ)
        error (EXIT_FAILURE, 0, "the container requires libkrun-awsnitro but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_sev;

      kconf->handle = kconf->handle_awsnitro;
      kconf->ctx_id = kconf->ctx_id_awsnitro;
      kconf->awsnitro = true;
    }
  ಅನ್ಯಥಾ
    {
      ಯದಿ (kconf->handle == ರಿಕ್ತ)
        error (EXIT_FAILURE, 0, "the container requires libkrun but it's not available");

      close_handles[0] = kconf->handle_sev;
      close_handles[1] = kconf->handle_awsnitro;
    }

  // We no longer need the other two libkrun handles.
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 2; i++)
    {
      ಯದಿ (close_handles[i] == ರಿಕ್ತ)
        ಅನುವರ್ತ;

      ret = dlclose (close_handles[i]);
      ಯದಿ (UNLIKELY (ret != 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libkrun_exec (ಶೂನ್ಯ *cookie, libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *pathname, ವರ್ಣ *ಧ್ರುವ argv[])
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_set_log_level) (uint32_t level);
  ಪೂರ್ಣಾಙ್ಕ (*krun_start_enter) (uint32_t ctx_id);
  int32_t (*krun_add_virtiofs2) (uint32_t ctx_id, ಧ್ರುವ ವರ್ಣ *c_tag, ಧ್ರುವ ವರ್ಣ *c_path, uint64_t shm_size);
  int32_t (*krun_set_ಬೇರು_disk) (uint32_t ctx_id, ಧ್ರುವ ವರ್ಣ *disk_path);
  int32_t (*krun_set_tee_config_file) (uint32_t ctx_id, ಧ್ರುವ ವರ್ಣ *file_path);
  int32_t (*krun_set_console_output) (uint32_t ctx_id, ಧ್ರುವ ವರ್ಣ *c_filepath);
  int32_t (*krun_set_exec) (uint32_t ctx_id, ಧ್ರುವ ವರ್ಣ *exec_path,
                            ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ argv[], ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ envp[]);
  ಸಂರಚನಾ krun_config *kconf = (ಸಂರಚನಾ krun_config *) cookie;
  ಶೂನ್ಯ *handle;
  int32_t ctx_id, ret;
  libcrun_error_t err;

  ret = libkrun_configure_flavor (cookie, container, &err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ಪೂರ್ಣಾಙ್ಕ errcode = crun_error_get_errno (&err);
      crun_error_release (&err);
      error (EXIT_FAILURE, errcode, "unable to configure libkrun flavor");
    }

  // /dev/kvm is required for all non AWS nitro workloads.
  ಯದಿ (! kconf->awsnitro && ! kconf->has_kvm)
    error (EXIT_FAILURE, 0, "`/dev/kvm` unavailable");

  handle = kconf->handle;
  ctx_id = kconf->ctx_id;

  krun_set_log_level = dlsym (handle, "krun_set_log_level");
  krun_start_enter = dlsym (handle, "krun_start_enter");
  ಯದಿ (krun_set_log_level == ರಿಕ್ತ || krun_start_enter == ರಿಕ್ತ)
    error (EXIT_FAILURE, 0, "could not find symbol in the krun library");

  /* Set log level according to crun's verbosity. */
  ವಿಕಲ್ಪ (libcrun_get_verbosity ())
    {
    ಪಕ್ಷ LIBCRUN_VERBOSITY_DEBUG:
      krun_set_log_level (KRUN_LOG_LEVEL_DEBUG);
      ಭಙ್ಗ;
    ಪಕ್ಷ LIBCRUN_VERBOSITY_WARNING:
      krun_set_log_level (KRUN_LOG_LEVEL_WARN);
      ಭಙ್ಗ;
    ಸ್ವಭಾವ:
      krun_set_log_level (KRUN_LOG_LEVEL_ERROR);
      ಭಙ್ಗ;
    }

  ಯದಿ (kconf->sev)
    {
      krun_set_ಬೇರು_disk = dlsym (handle, "krun_set_root_disk");
      krun_set_tee_config_file = dlsym (handle, "krun_set_tee_config_file");
      ಯದಿ (krun_set_ಬೇರು_disk == ರಿಕ್ತ || krun_set_tee_config_file == ರಿಕ್ತ)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-sev.so`");

      ret = krun_set_ಬೇರು_disk (ctx_id, "/disk.img");
      ಯದಿ (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set root disk");

      ret = krun_set_tee_config_file (ctx_id, KRUN_SEV_FILE);
      ಯದಿ (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set krun tee config file");
    }
  ಅನ್ಯಥಾ
    {
      json_object *val_virtiofs_tag = ರಿಕ್ತ;
      json_object *val_virtiofs_shm_size = ರಿಕ್ತ;
      ಧ್ರುವ ವರ್ಣ *virtiofs_tag = ರಿಕ್ತ;
      // Default to a conservative DAX size of 512MB, just like krun_set_root() does.
      uint64_t virtiofs_shm_size = 512 * 1024 * 1024ULL;

      ಯದಿ (kconf->config_ವೃಕ್ಷ != ರಿಕ್ತ)
        {
          val_virtiofs_tag = json_object_object_get (kconf->config_ವೃಕ್ಷ, "virtiofs_tag");
          ಯದಿ (val_virtiofs_tag != ರಿಕ್ತ && json_object_is_type (val_virtiofs_tag, json_type_string))
            virtiofs_tag = json_object_get_string (val_virtiofs_tag);

          val_virtiofs_shm_size = json_object_object_get (kconf->config_ವೃಕ್ಷ, "virtiofs_shm_size");
          ಯದಿ (val_virtiofs_shm_size != ರಿಕ್ತ && json_object_is_type (val_virtiofs_shm_size, json_type_int))
            virtiofs_shm_size = json_object_get_uint64 (val_virtiofs_shm_size);
        }

      ಯದಿ (virtiofs_tag == ರಿಕ್ತ)
        virtiofs_tag = "/dev/root";

      krun_add_virtiofs2 = dlsym (handle, "krun_add_virtiofs2");

      ಯದಿ (krun_add_virtiofs2 == ರಿಕ್ತ)
        error (EXIT_FAILURE, 0, "could not find symbol `krun_add_virtiofs2` in `libkrun.so`");

      ret = krun_add_virtiofs2 (ctx_id, virtiofs_tag, "/", virtiofs_shm_size);
      ಯದಿ (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not add virtiofs root with tag `%s`", virtiofs_tag);
    }

  ಯದಿ (kconf->awsnitro)
    {
      krun_set_console_output = dlsym (handle, "krun_set_console_output");
      krun_set_exec = dlsym (handle, "krun_set_exec");
      ಯದಿ (krun_set_console_output == ರಿಕ್ತ || krun_set_exec == ರಿಕ್ತ)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-awsnitro.so`");

      // Redirect all enclave output (read from vsock) to stdout.
      ret = krun_set_console_output (ctx_id, "/dev/stdout");
      ಯದಿ (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not redirect enclave output to stdout");

      ret = krun_set_exec (ctx_id, pathname, (ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ *) argv,
                           (ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ *) def->process->env);
      ಯದಿ (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set enclave execution arguments");
    }

  ret = libkrun_configure_vm (ctx_id, handle, kconf, container, &err);
  ಯದಿ (UNLIKELY (ret))
    {
      ಪೂರ್ಣಾಙ್ಕ errcode = crun_error_get_errno (&err);
      libcrun_error_t *tmp_err = &err;
      libcrun_error_report_and_release (tmp_err);
      error (EXIT_FAILURE, errcode, "could not configure krun vm");
    }

  json_object_put (kconf->config_doc);

  ret = krun_start_enter (ctx_id);
  ಯದಿ (UNLIKELY (ret < 0))
    error (EXIT_FAILURE, -ret, "could not start krun");

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libkrun_start_render_server (ಸಂರಚನಾ krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  kconf->gpu_flags = libkrun_parse_resource_configuration (kconf->config_ವೃಕ್ಷ, container, "krun.gpu_flags", "gpu_flags", false);
  ಯದಿ (kconf->gpu_flags > 0)
    {
      ಯದಿ (access ("/dev/dri", F_OK) != 0)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "gpu requested but /dev/dri is not available");

      ಯದಿ ((kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER) != 0)
        {
          ವರ್ಣ *rs_argv[40];
          ಪೂರ್ಣಾಙ್ಕ rs_argc = 0;
          ವರ್ಣ fd_as_str[16];
          ಪೂರ್ಣಾಙ್ಕ gpu_fds[2];
          pid_t pid;
          ಪೂರ್ಣಾಙ್ಕ null;

          ಯದಿ (access ("/usr/libexec/virgl_render_server", X_OK) != 0)
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/libexec/virgl_render_server binary");

          ಯದಿ (access ("/usr/bin/bwrap", X_OK) != 0)
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/bin/bwrap binary");

          ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, gpu_fds);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "could not create socketpair for virgl_render_server");

          snprintf (fd_as_str, ಪರಿಮಾಣ (fd_as_str), "%d", gpu_fds[FD_PAIR_CHILD]);

          rs_argv[rs_argc++] = (ವರ್ಣ *) "bwrap";
          rs_argv[rs_argc++] = (ವರ್ಣ *) "--ro-bind";
          rs_argv[rs_argc++] = (ವರ್ಣ *) "/usr";
          rs_argv[rs_argc++] = (ವರ್ಣ *) "/usr";
          ಯದಿ (access ("/lib", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (ವರ್ಣ *) "--ro-bind";
              rs_argv[rs_argc++] = (ವರ್ಣ *) "/lib";
              rs_argv[rs_argc++] = (ವರ್ಣ *) "/lib";
            }
          ಯದಿ (access ("/lib64", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (ವರ್ಣ *) "--ro-bind";
              rs_argv[rs_argc++] = (ವರ್ಣ *) "/lib64";
              rs_argv[rs_argc++] = (ವರ್ಣ *) "/lib64";
            }
          rs_argv[rs_argc++] = (ವರ್ಣ *) "--ro-bind";
          rs_argv[rs_argc++] = (ವರ್ಣ *) "/etc";
          rs_argv[rs_argc++] = (ವರ್ಣ *) "/etc";
          rs_argv[rs_argc++] = (ವರ್ಣ *) "--ro-bind";
          rs_argv[rs_argc++] = (ವರ್ಣ *) "/sys";
          rs_argv[rs_argc++] = (ವರ್ಣ *) "/sys";
          rs_argv[rs_argc++] = (ವರ್ಣ *) "--dev";
          rs_argv[rs_argc++] = (ವರ್ಣ *) "/dev";
          rs_argv[rs_argc++] = (ವರ್ಣ *) "--dev-bind";
          rs_argv[rs_argc++] = (ವರ್ಣ *) "/dev/dri";
          rs_argv[rs_argc++] = (ವರ್ಣ *) "/dev/dri";
          rs_argv[rs_argc++] = (ವರ್ಣ *) "--proc";
          rs_argv[rs_argc++] = (ವರ್ಣ *) "/proc";
          rs_argv[rs_argc++] = (ವರ್ಣ *) "--tmpfs";
          rs_argv[rs_argc++] = (ವರ್ಣ *) "/tmp";
          rs_argv[rs_argc++] = (ವರ್ಣ *) "--unshare-pid";
          rs_argv[rs_argc++] = (ವರ್ಣ *) "--unshare-ipc";
          rs_argv[rs_argc++] = (ವರ್ಣ *) "--unshare-net";
          rs_argv[rs_argc++] = (ವರ್ಣ *) "--";
          rs_argv[rs_argc++] = (ವರ್ಣ *) "/usr/libexec/virgl_render_server";
          rs_argv[rs_argc++] = (ವರ್ಣ *) "--socket-fd";
          rs_argv[rs_argc++] = fd_as_str;
          rs_argv[rs_argc] = ರಿಕ್ತ;

          pid = fork ();
          ಯದಿ (pid < 0)
            {
              close (gpu_fds[FD_PAIR_PARENT]);
              close (gpu_fds[FD_PAIR_CHILD]);
              ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "could not fork for virgl_render_server");
            }
          ಅನ್ಯಥಾ ಯದಿ (pid == 0)
            {
              close (gpu_fds[FD_PAIR_PARENT]);

              null = open ("/dev/null", O_WRONLY);
              ಯದಿ (null == -1)
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

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libkrun_configure_network (ಶೂನ್ಯ *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  ಸಂರಚನಾ krun_config *kconf = (ಸಂರಚನಾ krun_config *) cookie;
  pid_t pid;
  ವರ್ಣ *passt_argv[10];
  ವರ್ಣ fd_as_str[16];
  ಪೂರ್ಣಾಙ್ಕ use_passt;
  ಪೂರ್ಣಾಙ್ಕ argv_idx;
  ಪೂರ್ಣಾಙ್ಕ status;
  ಪೂರ್ಣಾಙ್ಕ null;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = libkrun_parse_string_configuration (kconf->config_ವೃಕ್ಷ, container,
                                            "krun.tap_name", "tap_name",
                                            &kconf->tap_name, err, false);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (kconf->tap_name != ರಿಕ್ತ && is_empty_string (kconf->tap_name))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "krun.tap_name cannot be empty");

  use_passt = libkrun_parse_resource_configuration (kconf->config_ವೃಕ್ಷ, container, "krun.use_passt", "use_passt", false);

  ಯದಿ (use_passt > 0)
    {
      ಯದಿ (kconf->tap_name != ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "krun.tap_name and krun.use_passt are mutually exclusive");

      kconf->use_passt = 1;
    }
  ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ 0;

  ret = socketpair (AF_UNIX, SOCK_STREAM, 0, kconf->passt_fds);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "create passt socketpair");

  snprintf (fd_as_str, ಪರಿಮಾಣ (fd_as_str), "%d", kconf->passt_fds[FD_PAIR_CHILD]);

  argv_idx = 0;
  passt_argv[argv_idx++] = (ವರ್ಣ *) "passt";
  passt_argv[argv_idx++] = (ವರ್ಣ *) "-t";
  passt_argv[argv_idx++] = (ವರ್ಣ *) "all";

  ಯದಿ (! kconf->has_awsnitro)
    {
      passt_argv[argv_idx++] = (ವರ್ಣ *) "-u";
      passt_argv[argv_idx++] = (ವರ್ಣ *) "all";
      passt_argv[argv_idx++] = (ವರ್ಣ *) "--no-dhcp-dns";
    }

  /* Set --no-map-gw. If the gateway for the network is also the DNS server
   * we won't be able to query the DNS server without --no-map-gw.
   * See discussion in https://github.com/containers/crun/pull/2099
   */
  passt_argv[argv_idx++] = (ವರ್ಣ *) "--no-map-gw";

  passt_argv[argv_idx++] = (ವರ್ಣ *) "--fd";
  passt_argv[argv_idx++] = fd_as_str;
  passt_argv[argv_idx] = ರಿಕ್ತ;

  pid = fork ();
  ಯದಿ (pid < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fork passt");
  ಅನ್ಯಥಾ ಯದಿ (pid == 0)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t i;

      close (kconf->passt_fds[FD_PAIR_PARENT]);

      null = open ("/dev/null", O_WRONLY);
      ಯದಿ (null == -1)
        _exit (EXIT_FAILURE);

      /* Remount /proc if the parent detached its PID namespace, as passt(1)
       * needs a consistent /proc view to operate properly.
       */
      ಪ್ರತಿ (i = 0; i < def->linux->namespaces_len; i++)
        {
          ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ = libcrun_find_namespace (def->linux->namespaces[i]->type);

          ಯದಿ (ಮೌಲ್ಯ == CLONE_NEWPID)
            {
              ಯದಿ (mount ("", "/proc", "proc", MS_NOSUID | MS_NODEV | MS_NOEXEC, ರಿಕ್ತ))
                _exit (EXIT_FAILURE);

              ಭಙ್ಗ;
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
  ಯದಿ (! (WIFEXITED (status)) || WEXITSTATUS (status) != 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "start passt");

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Return true if the spec already declares a device with the given PATH.  */
ಸ್ಥಿರ bool
spec_has_device (runtime_spec_schema_config_schema *def, ಧ್ರುವ ವರ್ಣ *path)
{
  size_t i;

  ಪ್ರತಿ (i = 0; i < def->linux->devices_len; i++)
    ಯದಿ (strcmp (def->linux->devices[i]->path, path) == 0)
      ಪ್ರತ್ಯಾಗಮ true;
  ಪ್ರತ್ಯಾಗಮ false;
}

/* libkrun_create_kvm_device: explicitly adds kvm device.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libkrun_configure_container (ಶೂನ್ಯ *cookie, ಗಣನಾ handler_configure_phase phase,
                             libcrun_context_t *context, libcrun_container_t *container,
                             ಧ್ರುವ ವರ್ಣ *rootfs, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret, rootfsfd;
  ಸಂರಚನಾ krun_config *kconf = (ಸಂರಚನಾ krun_config *) cookie;
  ಸಂರಚನಾ device_s kvm_device = { "/dev/kvm", "c", 10, 232, 0666, 0, 0 };
  ಸಂರಚನಾ device_s sev_device = { "/dev/sev", "c", 10, 124, 0666, 0, 0 };
  ಸಂರಚನಾ device_s nitro_device = { "/dev/nitro_enclaves", "c", 10, 122, 0666, 0, 0 };
  cleanup_close ಪೂರ್ಣಾಙ್ಕ devfd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ rootfsfd_cleanup = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  bool create_sev = false, create_awsnitro = false;
  bool is_user_ns;

  ಯದಿ (rootfs == ರಿಕ್ತ)
    rootfsfd = AT_FDCWD;
  ಅನ್ಯಥಾ
    {
      rootfsfd = rootfsfd_cleanup = open (rootfs, O_PATH | O_CLOEXEC);
      ಯದಿ (UNLIKELY (rootfsfd < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", rootfs);
    }

  ಯದಿ (phase == HANDLER_CONFIGURE_BEFORE_USERNS)
    {
      cleanup_free ವರ್ಣ *origin_config_path = ರಿಕ್ತ;
      cleanup_free ವರ್ಣ *state_dir = ರಿಕ್ತ;
      cleanup_free ವರ್ಣ *config = ರಿಕ್ತ;
      cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
      size_t config_size;

      ret = libcrun_get_state_directory (&state_dir, context->state_ಬೇರು, context->id, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = append_paths (&origin_config_path, err, state_dir, "config.json", ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = read_all_file (origin_config_path, &config, &config_size, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      /* CVE-2025-24965: the content below rootfs cannot be trusted because it is controlled by the user.  We
         must ensure the file is opened below the rootfs directory.  Whatever the image ships at that name
         could also be a device node or a FIFO, so never open it: unlink it and create a new regular file.  */
      ret = unlinkat (rootfsfd, KRUN_CONFIG_FILE, 0);
      ಯದಿ (UNLIKELY (ret < 0 && errno != ENOENT))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "unlink `%s`", KRUN_CONFIG_FILE);

      fd = safe_openat (rootfsfd, rootfs, KRUN_CONFIG_FILE, WRITE_FILE_DEFAULT_FLAGS | O_EXCL | O_NOFOLLOW, S_IRUSR | S_IRGRP | S_IROTH, err);
      ಯದಿ (UNLIKELY (fd < 0))
        ಪ್ರತ್ಯಾಗಮ fd;

      ret = safe_write (fd, KRUN_CONFIG_FILE, config, config_size, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = libkrun_read_vm_config (kconf, rootfsfd, rootfs, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = libkrun_start_render_server (kconf, container, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    ಪ್ರತ್ಯಾಗಮ 0;

  ret = libkrun_configure_network (cookie, container, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* Do nothing if /dev/kvm is already present in spec */
  ಯದಿ (spec_has_device (def, "/dev/kvm"))
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯದಿ (kconf->handle_sev != ರಿಕ್ತ)
    create_sev = ! spec_has_device (def, "/dev/sev");

  ಯದಿ (kconf->handle_awsnitro != ರಿಕ್ತ)
    create_awsnitro = ! spec_has_device (def, "/dev/nitro_enclaves");

  devfd = safe_openat (rootfsfd, rootfs, "dev", O_PATH | O_DIRECTORY | O_CLOEXEC, 0, err);
  ಯದಿ (UNLIKELY (devfd < 0))
    ಪ್ರತ್ಯಾಗಮ devfd;

  ret = check_running_in_user_namespace (err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;
  is_user_ns = ret;

  ಯದಿ (kconf->has_kvm)
    {
      ret = libcrun_create_dev (container, devfd, -1, &kvm_device, is_user_ns, true, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (create_sev)
    {
      ret = libcrun_create_dev (container, devfd, -1, &sev_device, is_user_ns, true, err);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_sev);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
          kconf->handle_sev = ರಿಕ್ತ;
        }
    }

  ಯದಿ (create_awsnitro)
    {
      ret = libcrun_create_dev (container, devfd, -1, &nitro_device, is_user_ns, true, err);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_awsnitro);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
          kconf->handle_awsnitro = ರಿಕ್ತ;
        }
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libkrun_load (ಶೂನ್ಯ **cookie, libcrun_error_t *err)
{
  int32_t ret;
  ಸಂರಚನಾ krun_config *kconf;
  ಧ್ರುವ ವರ್ಣ *libkrun_so = "libkrun.so.1";
  ಧ್ರುವ ವರ್ಣ *libkrun_sev_so = "libkrun-sev.so.1";
  ಧ್ರುವ ವರ್ಣ *libkrun_awsnitro_so = "libkrun-awsnitro.so.1";

  kconf = malloc (ಪರಿಮಾಣ (ಸಂರಚನಾ krun_config));
  ಯದಿ (kconf == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not allocate memory for krun_config");
  memset (kconf, 0, ಪರಿಮಾಣ (ಸಂರಚನಾ krun_config));

  kconf->gpu_render_server_fd = -1;

  kconf->handle = dlopen (libkrun_so, RTLD_NOW);
  kconf->handle_sev = dlopen (libkrun_sev_so, RTLD_NOW);
  kconf->handle_awsnitro = dlopen (libkrun_awsnitro_so, RTLD_NOW);

  ಯದಿ (kconf->handle == ರಿಕ್ತ && kconf->handle_sev == ರಿಕ್ತ && kconf->handle_awsnitro == ರಿಕ್ತ)
    {
      ಮುಕ್ತ (kconf);
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "failed to open `%s`, `%s`, and `%s` for krun_config: %s", libkrun_so, libkrun_sev_so, libkrun_awsnitro_so, dlerror ());
    }

  kconf->sev = false;
  kconf->awsnitro = false;

  /* Newer versions of libkrun no longer link against libkrunfw and
     instead they open it when creating the context. This implies
     we need to call "krun_create_ctx" before switching namespaces
     or it won't be able to find the library bundling the kernel. */
  ಯದಿ (kconf->handle)
    {
      ret = libkrun_create_context (kconf->handle, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಗಚ್ಛ error;
      kconf->ctx_id = ret;
    }

  ಯದಿ (kconf->handle_sev)
    {
      ret = libkrun_create_context (kconf->handle_sev, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಗಚ್ಛ error;
      kconf->ctx_id_sev = ret;
    }
  ಯದಿ (kconf->handle_awsnitro)
    {
      ret = libkrun_create_context (kconf->handle_awsnitro, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಗಚ್ಛ error;
      kconf->ctx_id_awsnitro = ret;
    }

  *cookie = kconf;

  ಪ್ರತ್ಯಾಗಮ 0;

error:
  ಯದಿ (kconf->handle)
    dlclose (kconf->handle);
  ಯದಿ (kconf->handle_sev)
    dlclose (kconf->handle_sev);
  ಯದಿ (kconf->handle_awsnitro)
    dlclose (kconf->handle_awsnitro);
  ಮುಕ್ತ (kconf);
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libkrun_unload (ಶೂನ್ಯ *cookie, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ r;

  ಸಂರಚನಾ krun_config *kconf = (ಸಂರಚನಾ krun_config *) cookie;
  ಯದಿ (kconf != ರಿಕ್ತ)
    {
      ಯದಿ (kconf->handle != ರಿಕ್ತ)
        {
          r = dlclose (kconf->handle);
          ಯದಿ (UNLIKELY (r != 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
        }
      ಯದಿ (kconf->handle_sev != ರಿಕ್ತ)
        {
          r = dlclose (kconf->handle_sev);
          ಯದಿ (UNLIKELY (r != 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not unload handle_sev: `%s`", dlerror ());
        }
      ಯದಿ (kconf->handle_awsnitro != ರಿಕ್ತ)
        {
          r = dlclose (kconf->handle_awsnitro);
          ಯದಿ (UNLIKELY (r != 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not unload handle_awsnitro: `%s`", dlerror ());
        }
      ಮುಕ್ತ (kconf);
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ runtime_spec_schema_defs_linux_device_cgroup *
make_oci_spec_dev (ಧ್ರುವ ವರ್ಣ *type, dev_t device, bool allow, ಧ್ರುವ ವರ್ಣ *access)
{
  runtime_spec_schema_defs_linux_device_cgroup *dev = xmalloc0 (ಪರಿಮಾಣ (*dev));

  dev->allow = allow;
  dev->allow_present = 1;

  dev->type = xstrdup (type);

  dev->major = major (device);
  dev->major_present = 1;

  dev->minor = minor (device);
  dev->minor_present = 1;

  dev->access = xstrdup (access);

  ಪ್ರತ್ಯಾಗಮ dev;
}

/* stat an optional device: on ENOENT clear *PRESENT, any other error fails.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
stat_optional_device (ಧ್ರುವ ವರ್ಣ *path, ಸಂರಚನಾ stat *st, bool *present, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret = stat (path, st);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ಯದಿ (errno != ENOENT)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "stat `%s`", path);
      *present = false;
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libkrun_modify_oci_configuration (ಶೂನ್ಯ *cookie arg_unused, libcrun_context_t *context arg_unused,
                                  runtime_spec_schema_config_schema *def,
                                  libcrun_error_t *err)
{
  ಧ್ರುವ size_t device_size = ಪರಿಮಾಣ (runtime_spec_schema_defs_linux_device_cgroup);
  ಸಂರಚನಾ krun_config *kconf = (ಸಂರಚನಾ krun_config *) cookie;
  ಸಂರಚನಾ stat st_kvm, st_sev, st_awsnitro;
  bool has_kvm = true, has_sev = true, has_awsnitro = true;
  size_t old_len, new_len;
  ಪೂರ್ಣಾಙ್ಕ ret;

  /* Always allow the /dev/kvm device.  */

  ret = stat_optional_device ("/dev/kvm", &st_kvm, &has_kvm, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = stat_optional_device ("/dev/sev", &st_sev, &has_sev, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = stat_optional_device ("/dev/nitro_enclaves", &st_awsnitro, &has_awsnitro, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  kconf->has_kvm = has_kvm;
  kconf->has_awsnitro = has_awsnitro;

  ಯದಿ (! has_kvm && ! has_awsnitro)
    ಪ್ರತ್ಯಾಗಮ 0;

  /* spec says these are optional, ensure they exist so we can add our devices */
  ಯದಿ (def->linux == ರಿಕ್ತ)
    def->linux = xmalloc0 (ಪರಿಮಾಣ (runtime_spec_schema_config_linux));

  ಯದಿ (def->linux->resources == ರಿಕ್ತ)
    def->linux->resources = xmalloc0 (ಪರಿಮಾಣ (runtime_spec_schema_config_linux_resources));

  old_len = def->linux->resources->devices_len;
  new_len = old_len;
  ಯದಿ (has_kvm)
    new_len += has_sev ? 2 : 1;
  ಯದಿ (has_awsnitro)
    new_len += 1;

  def->linux->resources->devices = xrealloc (def->linux->resources->devices, device_size * (new_len + 1));
  def->linux->resources->devices_len = new_len;

  ಯದಿ (has_kvm)
    {
      def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_kvm.st_rdev, true, "rwm");
      ಯದಿ (has_sev)
        def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_sev.st_rdev, true, "rwm");
    }

  ಯದಿ (has_awsnitro)
    def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_awsnitro.st_rdev, true, "rwm");

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libkrun_close_fds (ಶೂನ್ಯ *cookie, libcrun_container_t *container, ಪೂರ್ಣಾಙ್ಕ preserve_fds, libcrun_error_t *err)
{
  ಸಂರಚನಾ krun_config *kconf = (ಸಂರಚನಾ krun_config *) cookie;
  ಪೂರ್ಣಾಙ್ಕ first_fd_to_close = preserve_fds + 3;
  ಪೂರ್ಣಾಙ್ಕ ಕೆಳಗಿನ_fd, ಮೇಲಿನ_fd;
  ಪೂರ್ಣಾಙ್ಕ i;

  ಕೆಳಗಿನ_fd = ಮೇಲಿನ_fd = -1;

  ಯದಿ (kconf->use_passt)
    ಮೇಲಿನ_fd = kconf->passt_fds[FD_PAIR_PARENT];

  ಯದಿ (kconf->gpu_render_server_fd != -1)
    {
      ಯದಿ (kconf->gpu_render_server_fd > ಮೇಲಿನ_fd)
        {
          ಕೆಳಗಿನ_fd = ಮೇಲಿನ_fd;
          ಮೇಲಿನ_fd = kconf->gpu_render_server_fd;
        }
      ಅನ್ಯಥಾ
        ಕೆಳಗಿನ_fd = kconf->gpu_render_server_fd;
    }

  ಯದಿ (first_fd_to_close <= ಮೇಲಿನ_fd)
    {
      ಪ್ರತಿ (i = first_fd_to_close; i < ಮೇಲಿನ_fd; i++)
        {
          // If we're closing proc_fd, make sure to invalidate it.
          ಯದಿ (i == container->proc_fd)
            container->proc_fd = -1;
          ಅನ್ಯಥಾ ಯದಿ (i == ಕೆಳಗಿನ_fd)
            ಅನುವರ್ತ;
          close (i);
        }

      first_fd_to_close = ಮೇಲಿನ_fd + 1;
    }

  ಪ್ರತ್ಯಾಗಮ mark_or_close_fds_ge_than (container, first_fd_to_close, true, err);
}

ಸಂರಚನಾ custom_handler_s handler_libkrun = {
  .name = "krun",
  .alias = ರಿಕ್ತ,
  .feature_string = "LIBKRUN",
  .supports_open_ವೃಕ್ಷ_namespace = false,
  .load = libkrun_load,
  .unload = libkrun_unload,
  .run_func = libkrun_exec,
  .configure_container = libkrun_configure_container,
  .modify_oci_configuration = libkrun_modify_oci_configuration,
  .close_fds = libkrun_close_fds,
};

#endif
