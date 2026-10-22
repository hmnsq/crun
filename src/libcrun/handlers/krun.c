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
#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ

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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DLOPEN
#  include <dlfcn.h>
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_LIBKRUN
#  include <libkrun.h>
#ಆದರೆ_ಅಂತ್ಯ

/* This allows us to build even with headers from older libkrun versions.
 * If the installed version of libkrun doesn't support this feature,
 * krun_add_net_unixstream() will return EINVAL.
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NET_FLAG_DHCP_CLIENT
#  define NET_FLAG_DHCP_CLIENT (1 << 1)
#ಆದರೆ_ಅಂತ್ಯ
/* libkrun has a hard-limit of 16 vCPUs per microVM. */
#ವ್ಯಾಖ್ಯಾನಿಸು LIBKRUN_MAX_VCPUS 16

/* If the user doesn't configure the RAM amount, fallback to this value. */
#ವ್ಯಾಖ್ಯಾನಿಸು LIBKRUN_DEFAULT_RAM_MIB 1024

/* The minimum amount of RAM for a viable microVM is 128 MB. */
#ವ್ಯಾಖ್ಯಾನಿಸು LIBKRUN_MINIMUM_RAM_MIB 128

/* crun dumps the container configuration into this file, which will be read by
 * libkrun to set up the environment for the workload inside the microVM.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು KRUN_CONFIG_FILE ".krun_config.json"

/* The presence of this file indicates this is a container intended to be run
 * as a confidential workload inside a SEV-powered TEE.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು KRUN_SEV_FILE "/krun-sev.json"

/* This file contains configuration parameters for the microVM. crun needs to
 * read and parse it, using the information obtained from it to configure
 * libkrun as required.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು KRUN_VM_FILE "/.krun_vm.json"

/* KRUN_VM_FILE holds a handful of settings and it is parsed by crun
 * itself before the container process exists, refuse anything bigger.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು KRUN_VM_FILE_MAX_ಗಾತ್ರ (64 * 1024)

#ವ್ಯಾಖ್ಯಾನಿಸು KRUN_FLAVOR_AWS_NITRO "aws-nitro"
#ವ್ಯಾಖ್ಯಾನಿಸು KRUN_FLAVOR_SEV "sev"

#ವ್ಯಾಖ್ಯಾನಿಸು FD_PAIR_ಪೋಷಕ 0
#ವ್ಯಾಖ್ಯಾನಿಸು FD_PAIR_ಮಗು 1

ರಚನೆ krun_config
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
  ಪೂರ್ಣಾಂಕ passt_fds[2];
  json_object *config_doc;
  json_object *config_ವೃಕ್ಷ;
  bool use_passt;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *tap_name;
  ಪೂರ್ಣಾಂಕ gpu_render_server_fd;
  ಪೂರ್ಣಾಂಕ gpu_flags;
};

/* libkrun handler.  */
#ಆದರೆ HAVE_DLOPEN && HAVE_LIBKRUN
ಸ್ಥಿರ int32_t
libkrun_create_context (ಶೂನ್ಯ *handle, libcrun_error_t *err)
{
  int32_t (*krun_create_ctx) ();
  int32_t ctx_id;

  krun_create_ctx = dlsym (handle, "krun_create_ctx");
  ಆದರೆ (krun_create_ctx == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "could not find symbol in the krun library");

  ctx_id = krun_create_ctx ();
  ಆದರೆ (UNLIKELY (ctx_id < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, -ctx_id, "could not create krun context");

  ಹಿಂತಿರುಗಿ ctx_id;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libkrun_configure_kernel (uint32_t ctx_id, ಶೂನ್ಯ *handle, json_object *config_ವೃಕ್ಷ, libcrun_error_t *err)
{
  int32_t (*krun_ಗಣ_kernel) (uint32_t ctx_id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *kernel_ಮಾರ್ಗ,
                              uint32_t kernel_format, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *initrd_ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *kernel_cmdline);
  json_object *kernel_ಮಾರ್ಗ = ಖಾಲಿ;
  json_object *kernel_format = ಖಾಲಿ;
  json_object *val_initrd_ಮಾರ್ಗ = ಖಾಲಿ;
  json_object *val_kernel_cmdline = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *initrd_ಮಾರ್ಗ = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *kernel_cmdline = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;

  /* kernel_path and kernel_format must be present */
  kernel_ಮಾರ್ಗ = json_object_object_get (config_ವೃಕ್ಷ, "kernel_path");
  ಆದರೆ (kernel_ಮಾರ್ಗ == ಖಾಲಿ || ! json_object_is_type (kernel_ಮಾರ್ಗ, json_type_string))
    ಹಿಂತಿರುಗಿ 0;

  kernel_format = json_object_object_get (config_ವೃಕ್ಷ, "kernel_format");
  ಆದರೆ (kernel_format == ಖಾಲಿ || ! json_object_is_type (kernel_format, json_type_int))
    ಹಿಂತಿರುಗಿ 0;

  /* initrd and kernel_cmdline are optional */
  val_initrd_ಮಾರ್ಗ = json_object_object_get (config_ವೃಕ್ಷ, "initrd_path");
  ಆದರೆ (val_initrd_ಮಾರ್ಗ != ಖಾಲಿ && json_object_is_type (val_initrd_ಮಾರ್ಗ, json_type_string))
    initrd_ಮಾರ್ಗ = json_object_get_string (val_initrd_ಮಾರ್ಗ);

  val_kernel_cmdline = json_object_object_get (config_ವೃಕ್ಷ, "kernel_cmdline");
  ಆದರೆ (val_kernel_cmdline != ಖಾಲಿ && json_object_is_type (val_kernel_cmdline, json_type_string))
    kernel_cmdline = json_object_get_string (val_kernel_cmdline);

  krun_ಗಣ_kernel = dlsym (handle, "krun_set_kernel");
  ಆದರೆ (krun_ಗಣ_kernel == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "could not find symbol in krun library");

  ret = krun_ಗಣ_kernel (ctx_id,
                         json_object_get_string (kernel_ಮಾರ್ಗ),
                         (uint32_t) json_object_get_int64 (kernel_format),
                         initrd_ಮಾರ್ಗ, kernel_cmdline);

  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "could not configure a krun external kernel");

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libkrun_read_vm_config (ರಚನೆ krun_config *kconf, ಪೂರ್ಣಾಂಕ rootfsfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  cleanup_free ಅಕ್ಷರ *config = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ pathfd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
  proc_fd_ಮಾರ್ಗ_t fd_ಮಾರ್ಗ;
  ರಚನೆ stat st;

  /* The content of the rootfs is controlled by the user: look at the file through an
     O_PATH descriptor first and refuse anything that is not a small regular file, as
     opening a FIFO blocks and a device node must not be accessed.  */
  pathfd = ಸುರಕ್ಷಿತ_openat (rootfsfd, rootfs, KRUN_VM_FILE, O_ಮಾರ್ಗ | O_CLOEXEC | O_NOFOLLOW, 0, err);
  ಆದರೆ (pathfd < 0)
    {
      // The configuration file is optional, don't generate an error if it's missing.
      ಆದರೆ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ಹಿಂತಿರುಗಿ 0;
        }
      ಹಿಂತಿರುಗಿ pathfd;
    }

  ret = fstat (pathfd, &st);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "stat `%s`", KRUN_VM_FILE);

  ಆದರೆ (! S_ISREG (st.st_mode))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "`%s` is not a regular file", KRUN_VM_FILE);

  get_proc_self_fd_ಮಾರ್ಗ (fd_ಮಾರ್ಗ, pathfd);

  fd = open (fd_ಮಾರ್ಗ, O_RDONLY | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", KRUN_VM_FILE);

  ret = read_all_fd_with_max_ಗಾತ್ರ (fd, KRUN_VM_FILE, &config, ಖಾಲಿ, KRUN_VM_FILE_MAX_ಗಾತ್ರ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = parse_json_file (&kconf->config_doc, config, ಖಾಲಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  kconf->config_ವೃಕ್ಷ = kconf->config_doc;
  ಹಿಂತಿರುಗಿ 0;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libkrun_parse_resource_configuration (json_object *config_ವೃಕ್ಷ, libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *annotation, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ, bool use_config_file)
{
  ಅಕ್ಷರ *val_str, *endptr;
  ಪೂರ್ಣಾಂಕ val = -1;
  json_object *val_json = ಖಾಲಿ;

  val_str = (ಅಕ್ಷರ *) ಕಂಡುಹಿಡಿ_annotation (container, annotation);
  ಆದರೆ (val_str != ಖಾಲಿ)
    {
      errno = 0;
      val = (ಪೂರ್ಣಾಂಕ) strtol (val_str, &endptr, 10);
      ಆದರೆ (errno != 0 || endptr == val_str || *endptr != '\0')
        /* Annotations value is not a valid integer. */
        error (EXIT_FAILURE, 0, "krun annotation %s value cannot be converted to an integer", annotation);

      ಆದರೆ (val < 0)
        error (EXIT_FAILURE, 0, "krun annotation %s value must be a positive integer", annotation);

      ಹಿಂತಿರುಗಿ val;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (use_config_file && config_ವೃಕ್ಷ != ಖಾಲಿ)
    {
      val_json = json_object_object_get (config_ವೃಕ್ಷ, ಕೀಲಿ);
      ಆದರೆ (val_json == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ val;
      ಆದರೆ (! json_object_is_type (val_json, json_type_int))
        error (EXIT_FAILURE, 0, "krun krun_vm.json %s value is not an integer", ಕೀಲಿ);

      val = (ಪೂರ್ಣಾಂಕ) json_object_get_int64 (val_json);
    }

  ಹಿಂತಿರುಗಿ val;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libkrun_parse_string_configuration (json_object *config_ವೃಕ್ಷ, libcrun_container_t *container,
                                    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *annotation, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ,
                                    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಮೌಲ್ಯ, libcrun_error_t *err, bool use_config_file)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *val;
  json_object *val_json = ಖಾಲಿ;

  *ಮೌಲ್ಯ = ಖಾಲಿ;

  val = ಕಂಡುಹಿಡಿ_annotation (container, annotation);
  ಆದರೆ (val != ಖಾಲಿ)
    {
      *ಮೌಲ್ಯ = val;
      ಹಿಂತಿರುಗಿ 0;
    }

  ಆದರೆ (! use_config_file || config_ವೃಕ್ಷ == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  val_json = json_object_object_get (config_ವೃಕ್ಷ, ಕೀಲಿ);
  ಆದರೆ (val_json == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  ಆದರೆ (! json_object_is_type (val_json, json_type_string))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, ".krun_vm.json %s value is not a string", ಕೀಲಿ);

  *ಮೌಲ್ಯ = json_object_get_string (val_json);
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ
libkrun_make_tap_mac (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, uint8_t mac[6])
{
  uint64_t hash = 14695981039346656037ULL;
  size_t i;

  ಗಾಗಿ (; *id != '\0'; id++)
    {
      hash ^= (uint8_t) *id;
      hash *= 1099511628211ULL;
    }

  ಗಾಗಿ (i = 0; i < 6; i++)
    mac[i] = (uint8_t) (hash >> (i * 8));

  mac[0] = (mac[0] & 0xfe) | 0x02;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libkrun_configure_vm (uint32_t ctx_id, ಶೂನ್ಯ *handle, ರಚನೆ krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_ಗಣ_vm_config) (uint32_t ctx_id, uint8_t ಅಂಕೆ_vcpus, uint32_t ram_mib);
  int32_t (*krun_ಕೂಡಿಸು_net_unixstream) (uint32_t ctx_id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *c_ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ fd, uint8_t *ಸ್ಥಿರಾಂಕ c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_ಕೂಡಿಸು_net_tap) (uint32_t ctx_id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *c_tap_name, ಸ್ಥಿರಾಂಕ uint8_t *c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_ಗಣ_gpu_options) (uint32_t ctx_id, uint32_t virgl_flags);
  int32_t (*krun_ಗಣ_gpu_render_server_fd) (uint32_t ctx_id, ಪೂರ್ಣಾಂಕ fd);
  ಪೂರ್ಣಾಂಕ cpus, ram_mib, nested_virt, ret;
  cpu_ಗಣ_t ಗಣ;

  /* We let the OCI set the number of vCPUs for the VM, since cgroups CPU restrictions still apply. */
  cpus = libkrun_parse_resource_configuration (kconf->config_ವೃಕ್ಷ, container, "krun.cpus", "cpus", true);
  ಆದರೆ (cpus <= 0)
    {
      CPU_ZERO (&ಗಣ);
      ಆದರೆ (sched_getaffinity (getpid (), ಗಾತ್ರ (ಗಣ), &ಗಣ) == 0)
        cpus = MIN (CPU_ಎಣಿಕೆ (&ಗಣ), LIBKRUN_MAX_VCPUS);
      ಇಲ್ಲದಿದ್ದರೆ
        cpus = 1;
    }

  /* We let the OCI set the amount of RAM for the VM, since cgroups memory restrictions still apply. */
  ram_mib = libkrun_parse_resource_configuration (kconf->config_ವೃಕ್ಷ, container, "krun.ram_mib", "ram_mib", true);
  ಆದರೆ (ram_mib <= LIBKRUN_MINIMUM_RAM_MIB)
    {
      ಆದರೆ (def && def->linux && def->linux->resources && def->linux->resources->ಸ್ಮರಣೆ
          && def->linux->resources->ಸ್ಮರಣೆ->ಎಲ್ಲೆ_present)
        ram_mib = def->linux->resources->ಸ್ಮರಣೆ->ಎಲ್ಲೆ / (1024 * 1024);
      ಇಲ್ಲದಿದ್ದರೆ
        ram_mib = LIBKRUN_DEFAULT_RAM_MIB;
    }

  krun_ಗಣ_vm_config = dlsym (handle, "krun_set_vm_config");

  ಆದರೆ (krun_ಗಣ_vm_config == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "could not find symbol in the krun library");

  ret = krun_ಗಣ_vm_config (ctx_id, cpus, ram_mib);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "could not set krun vm configuration");

  nested_virt = libkrun_parse_resource_configuration (kconf->config_ವೃಕ್ಷ, container, "krun.nested_virt", "nested_virt", false);
  ಆದರೆ (nested_virt > 0)
    {
      int32_t (*krun_check_nested_virt) (ಶೂನ್ಯ);
      int32_t (*krun_ಗಣ_nested_virt) (uint32_t ctx_id, bool enabled);

      krun_check_nested_virt = dlsym (handle, "krun_check_nested_virt");
      ಆದರೆ (krun_check_nested_virt != ಖಾಲಿ && krun_check_nested_virt () != 1)
        libcrun_warning ("nested virtualization requested but may not be supported on this host");

      krun_ಗಣ_nested_virt = dlsym (handle, "krun_set_nested_virt");
      ಆದರೆ (krun_ಗಣ_nested_virt == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "could not find symbol `krun_set_nested_virt` in the krun library");

      ret = krun_ಗಣ_nested_virt (ctx_id, true);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "could not enable nested virtualization");
    }

  ಆದರೆ (kconf->tap_name != ಖಾಲಿ)
    {
      krun_ಕೂಡಿಸು_net_tap = dlsym (handle, "krun_add_net_tap");
      ಆದರೆ (krun_ಕೂಡಿಸು_net_tap == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "could not find symbol `krun_add_net_tap` in the krun library");

      uint8_t mac[6];
      libkrun_make_tap_mac (container->context->id, mac);
      ret = krun_ಕೂಡಿಸು_net_tap (ctx_id, kconf->tap_name, &mac[0], COMPAT_NET_FEATURES, 0);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "could not add krun TAP interface `%s`", kconf->tap_name);
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (kconf->use_passt)
    {
      krun_ಕೂಡಿಸು_net_unixstream = dlsym (handle, "krun_add_net_unixstream");
      ಆದರೆ (krun_ಕೂಡಿಸು_net_unixstream == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "could not find symbol `krun_add_net_unixstream` in the krun library");

      uint8_t mac[] = { 0x5a, 0x94, 0xef, 0xe4, 0x0c, 0xee };
      ret = krun_ಕೂಡಿಸು_net_unixstream (ctx_id, ಖಾಲಿ, kconf->passt_fds[FD_PAIR_ಪೋಷಕ], &mac[0], COMPAT_NET_FEATURES, NET_FLAG_DHCP_CLIENT);
      ಆದರೆ (UNLIKELY (ret == -EINVAL))
        ret = krun_ಕೂಡಿಸು_net_unixstream (ctx_id, ಖಾಲಿ, kconf->passt_fds[FD_PAIR_ಪೋಷಕ], &mac[0], COMPAT_NET_FEATURES, 0);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "could not set krun net configuration");
    }

  ಆದರೆ (kconf->gpu_flags > 0)
    {
      krun_ಗಣ_gpu_options = dlsym (kconf->handle, "krun_set_gpu_options");
      ಆದರೆ (krun_ಗಣ_gpu_options == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "gpu requested but the version of libkrun in this system does not support it");

      ret = krun_ಗಣ_gpu_options (kconf->ctx_id, kconf->gpu_flags);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "gpu requested but could not configure virtio gpu device");

      ಆದರೆ (kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER)
        {
          krun_ಗಣ_gpu_render_server_fd = dlsym (kconf->handle, "krun_set_gpu_render_server_fd");
          ಆದರೆ (krun_ಗಣ_gpu_render_server_fd == ಖಾಲಿ)
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "gpu with render server requested but the version of libkrun in this system does not support it");

          ret = krun_ಗಣ_gpu_render_server_fd (kconf->ctx_id, kconf->gpu_render_server_fd);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "gpu with render server requested but could not configure its file descriptor");
        }
    }

  ಆದರೆ (kconf->config_ವೃಕ್ಷ != ಖಾಲಿ)
    {
      /* Try to configure an external kernel. If the configuration file doesn't
       * specify a kernel, libkrun automatically fall back to using libkrunfw,
       * if the library is present and was loaded while creating the context.
       */
      ಪೂರ್ಣಾಂಕ custom_kernel = libkrun_parse_resource_configuration (kconf->config_ವೃಕ್ಷ, container, "krun.custom_kernel", "custom_kernel", false);
      ಆದರೆ (custom_kernel > 0)
        {
          ret = libkrun_configure_kernel (ctx_id, handle, kconf->config_ವೃಕ್ಷ, err);
          ಆದರೆ (UNLIKELY (ret))
            ಹಿಂತಿರುಗಿ ret;
        }
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libkrun_configure_flavor (ಶೂನ್ಯ *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret, sev_indicated = 0, awsnitro_indicated = 0;
  ರಚನೆ krun_config *kconf = (ರಚನೆ krun_config *) cookie;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *flavor = ಖಾಲಿ;
  ಶೂನ್ಯ *close_handles[2];

  close_handles[0] = ಖಾಲಿ;
  close_handles[1] = ಖಾಲಿ;

  // Check if the user provided the krun variant through OCI annotations.
  flavor = ಕಂಡುಹಿಡಿ_annotation (container, "krun.variant");
  ಆದರೆ (flavor != ಖಾಲಿ)
    {
      sev_indicated |= strcmp (flavor, KRUN_FLAVOR_SEV) == 0;
      awsnitro_indicated |= strcmp (flavor, KRUN_FLAVOR_AWS_NITRO) == 0;
    }

  ಆದರೆ (sev_indicated)
    {
      ಆದರೆ (kconf->handle_sev == ಖಾಲಿ)
        error (EXIT_FAILURE, 0, "the container requires libkrun-sev but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_awsnitro;

      kconf->handle = kconf->handle_sev;
      kconf->ctx_id = kconf->ctx_id_sev;
      kconf->sev = true;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (awsnitro_indicated)
    {
      ಆದರೆ (kconf->handle_awsnitro == ಖಾಲಿ)
        error (EXIT_FAILURE, 0, "the container requires libkrun-awsnitro but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_sev;

      kconf->handle = kconf->handle_awsnitro;
      kconf->ctx_id = kconf->ctx_id_awsnitro;
      kconf->awsnitro = true;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (kconf->handle == ಖಾಲಿ)
        error (EXIT_FAILURE, 0, "the container requires libkrun but it's not available");

      close_handles[0] = kconf->handle_sev;
      close_handles[1] = kconf->handle_awsnitro;
    }

  // We no longer need the other two libkrun handles.
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 2; i++)
    {
      ಆದರೆ (close_handles[i] == ಖಾಲಿ)
        ಮುಂದುವರಿ;

      ret = dlclose (close_handles[i]);
      ಆದರೆ (UNLIKELY (ret != 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libkrun_exec (ಶೂನ್ಯ *cookie, libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathname, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ argv[])
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_ಗಣ_log_level) (uint32_t level);
  ಪೂರ್ಣಾಂಕ (*krun_ಆರಂಭ_enter) (uint32_t ctx_id);
  int32_t (*krun_ಕೂಡಿಸು_virtiofs2) (uint32_t ctx_id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *c_ಟ್ಯಾಗ್, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *c_ಮಾರ್ಗ, uint64_t shm_ಗಾತ್ರ);
  int32_t (*krun_ಗಣ_ಬೇರು_disk) (uint32_t ctx_id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *disk_ಮಾರ್ಗ);
  int32_t (*krun_ಗಣ_tee_config_file) (uint32_t ctx_id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file_ಮಾರ್ಗ);
  int32_t (*krun_ಗಣ_console_output) (uint32_t ctx_id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *c_filepath);
  int32_t (*krun_ಗಣ_exec) (uint32_t ctx_id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *exec_ಮಾರ್ಗ,
                            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ argv[], ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ envp[]);
  ರಚನೆ krun_config *kconf = (ರಚನೆ krun_config *) cookie;
  ಶೂನ್ಯ *handle;
  int32_t ctx_id, ret;
  libcrun_error_t err;

  ret = libkrun_configure_flavor (cookie, container, &err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಪೂರ್ಣಾಂಕ errcode = crun_error_get_errno (&err);
      crun_error_release (&err);
      error (EXIT_FAILURE, errcode, "unable to configure libkrun flavor");
    }

  // /dev/kvm is required for all non AWS nitro workloads.
  ಆದರೆ (! kconf->awsnitro && ! kconf->has_kvm)
    error (EXIT_FAILURE, 0, "`/dev/kvm` unavailable");

  handle = kconf->handle;
  ctx_id = kconf->ctx_id;

  krun_ಗಣ_log_level = dlsym (handle, "krun_set_log_level");
  krun_ಆರಂಭ_enter = dlsym (handle, "krun_start_enter");
  ಆದರೆ (krun_ಗಣ_log_level == ಖಾಲಿ || krun_ಆರಂಭ_enter == ಖಾಲಿ)
    error (EXIT_FAILURE, 0, "could not find symbol in the krun library");

  /* Set log level according to crun's verbosity. */
  ಆಯ್ಕೆ (libcrun_get_verbosity ())
    {
    ಸ್ಥಿತಿ LIBCRUN_VERBOSITY_DEBUG:
      krun_ಗಣ_log_level (KRUN_LOG_LEVEL_DEBUG);
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ LIBCRUN_VERBOSITY_WARNING:
      krun_ಗಣ_log_level (KRUN_LOG_LEVEL_WARN);
      ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
      krun_ಗಣ_log_level (KRUN_LOG_LEVEL_ERROR);
      ನಿಲ್ಲಿಸು;
    }

  ಆದರೆ (kconf->sev)
    {
      krun_ಗಣ_ಬೇರು_disk = dlsym (handle, "krun_set_root_disk");
      krun_ಗಣ_tee_config_file = dlsym (handle, "krun_set_tee_config_file");
      ಆದರೆ (krun_ಗಣ_ಬೇರು_disk == ಖಾಲಿ || krun_ಗಣ_tee_config_file == ಖಾಲಿ)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-sev.so`");

      ret = krun_ಗಣ_ಬೇರು_disk (ctx_id, "/disk.img");
      ಆದರೆ (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set root disk");

      ret = krun_ಗಣ_tee_config_file (ctx_id, KRUN_SEV_FILE);
      ಆದರೆ (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set krun tee config file");
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      json_object *val_virtiofs_ಟ್ಯಾಗ್ = ಖಾಲಿ;
      json_object *val_virtiofs_shm_ಗಾತ್ರ = ಖಾಲಿ;
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *virtiofs_ಟ್ಯಾಗ್ = ಖಾಲಿ;
      // Default to a conservative DAX size of 512MB, just like krun_set_root() does.
      uint64_t virtiofs_shm_ಗಾತ್ರ = 512 * 1024 * 1024ULL;

      ಆದರೆ (kconf->config_ವೃಕ್ಷ != ಖಾಲಿ)
        {
          val_virtiofs_ಟ್ಯಾಗ್ = json_object_object_get (kconf->config_ವೃಕ್ಷ, "virtiofs_tag");
          ಆದರೆ (val_virtiofs_ಟ್ಯಾಗ್ != ಖಾಲಿ && json_object_is_type (val_virtiofs_ಟ್ಯಾಗ್, json_type_string))
            virtiofs_ಟ್ಯಾಗ್ = json_object_get_string (val_virtiofs_ಟ್ಯಾಗ್);

          val_virtiofs_shm_ಗಾತ್ರ = json_object_object_get (kconf->config_ವೃಕ್ಷ, "virtiofs_shm_size");
          ಆದರೆ (val_virtiofs_shm_ಗಾತ್ರ != ಖಾಲಿ && json_object_is_type (val_virtiofs_shm_ಗಾತ್ರ, json_type_int))
            virtiofs_shm_ಗಾತ್ರ = json_object_get_uint64 (val_virtiofs_shm_ಗಾತ್ರ);
        }

      ಆದರೆ (virtiofs_ಟ್ಯಾಗ್ == ಖಾಲಿ)
        virtiofs_ಟ್ಯಾಗ್ = "/dev/root";

      krun_ಕೂಡಿಸು_virtiofs2 = dlsym (handle, "krun_add_virtiofs2");

      ಆದರೆ (krun_ಕೂಡಿಸು_virtiofs2 == ಖಾಲಿ)
        error (EXIT_FAILURE, 0, "could not find symbol `krun_add_virtiofs2` in `libkrun.so`");

      ret = krun_ಕೂಡಿಸು_virtiofs2 (ctx_id, virtiofs_ಟ್ಯಾಗ್, "/", virtiofs_shm_ಗಾತ್ರ);
      ಆದರೆ (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not add virtiofs root with tag `%s`", virtiofs_ಟ್ಯಾಗ್);
    }

  ಆದರೆ (kconf->awsnitro)
    {
      krun_ಗಣ_console_output = dlsym (handle, "krun_set_console_output");
      krun_ಗಣ_exec = dlsym (handle, "krun_set_exec");
      ಆದರೆ (krun_ಗಣ_console_output == ಖಾಲಿ || krun_ಗಣ_exec == ಖಾಲಿ)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-awsnitro.so`");

      // Redirect all enclave output (read from vsock) to stdout.
      ret = krun_ಗಣ_console_output (ctx_id, "/dev/stdout");
      ಆದರೆ (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not redirect enclave output to stdout");

      ret = krun_ಗಣ_exec (ctx_id, pathname, (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *) argv,
                           (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *) def->process->env);
      ಆದರೆ (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set enclave execution arguments");
    }

  ret = libkrun_configure_vm (ctx_id, handle, kconf, container, &err);
  ಆದರೆ (UNLIKELY (ret))
    {
      ಪೂರ್ಣಾಂಕ errcode = crun_error_get_errno (&err);
      libcrun_error_t *tmp_err = &err;
      libcrun_error_report_and_release (tmp_err);
      error (EXIT_FAILURE, errcode, "could not configure krun vm");
    }

  json_object_put (kconf->config_doc);

  ret = krun_ಆರಂಭ_enter (ctx_id);
  ಆದರೆ (UNLIKELY (ret < 0))
    error (EXIT_FAILURE, -ret, "could not start krun");

  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libkrun_ಆರಂಭ_render_server (ರಚನೆ krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  kconf->gpu_flags = libkrun_parse_resource_configuration (kconf->config_ವೃಕ್ಷ, container, "krun.gpu_flags", "gpu_flags", false);
  ಆದರೆ (kconf->gpu_flags > 0)
    {
      ಆದರೆ (access ("/dev/dri", F_OK) != 0)
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "gpu requested but /dev/dri is not available");

      ಆದರೆ ((kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER) != 0)
        {
          ಅಕ್ಷರ *rs_argv[40];
          ಪೂರ್ಣಾಂಕ rs_argc = 0;
          ಅಕ್ಷರ fd_as_str[16];
          ಪೂರ್ಣಾಂಕ gpu_fds[2];
          pid_t pid;
          ಪೂರ್ಣಾಂಕ null;

          ಆದರೆ (access ("/usr/libexec/virgl_render_server", X_OK) != 0)
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/libexec/virgl_render_server binary");

          ಆದರೆ (access ("/usr/bin/bwrap", X_OK) != 0)
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/bin/bwrap binary");

          ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, gpu_fds);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "could not create socketpair for virgl_render_server");

          snprintf (fd_as_str, ಗಾತ್ರ (fd_as_str), "%d", gpu_fds[FD_PAIR_ಮಗು]);

          rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "bwrap";
          rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "--ro-bind";
          rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "/usr";
          rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "/usr";
          ಆದರೆ (access ("/lib", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "--ro-bind";
              rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "/lib";
              rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "/lib";
            }
          ಆದರೆ (access ("/lib64", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "--ro-bind";
              rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "/lib64";
              rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "/lib64";
            }
          rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "--ro-bind";
          rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "/etc";
          rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "/etc";
          rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "--ro-bind";
          rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "/sys";
          rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "/sys";
          rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "--dev";
          rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "/dev";
          rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "--dev-bind";
          rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "/dev/dri";
          rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "/dev/dri";
          rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "--proc";
          rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "/proc";
          rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "--tmpfs";
          rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "/tmp";
          rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "--unshare-pid";
          rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "--unshare-ipc";
          rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "--unshare-net";
          rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "--";
          rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "/usr/libexec/virgl_render_server";
          rs_argv[rs_argc++] = (ಅಕ್ಷರ *) "--socket-fd";
          rs_argv[rs_argc++] = fd_as_str;
          rs_argv[rs_argc] = ಖಾಲಿ;

          pid = fork ();
          ಆದರೆ (pid < 0)
            {
              close (gpu_fds[FD_PAIR_ಪೋಷಕ]);
              close (gpu_fds[FD_PAIR_ಮಗು]);
              ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "could not fork for virgl_render_server");
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (pid == 0)
            {
              close (gpu_fds[FD_PAIR_ಪೋಷಕ]);

              null = open ("/dev/null", O_WRONLY);
              ಆದರೆ (null == -1)
                _exit (EXIT_FAILURE);

              dup2 (null, STDOUT_FILENO);
              dup2 (null, STDERR_FILENO);
              close (null);

              execvp ("/usr/bin/bwrap", rs_argv);
              _exit (EXIT_FAILURE);
            }

          close (gpu_fds[FD_PAIR_ಮಗು]);
          kconf->gpu_render_server_fd = gpu_fds[FD_PAIR_ಪೋಷಕ];
        }
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libkrun_configure_ಜಾಲ (ಶೂನ್ಯ *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  ರಚನೆ krun_config *kconf = (ರಚನೆ krun_config *) cookie;
  pid_t pid;
  ಅಕ್ಷರ *passt_argv[10];
  ಅಕ್ಷರ fd_as_str[16];
  ಪೂರ್ಣಾಂಕ use_passt;
  ಪೂರ್ಣಾಂಕ argv_idx;
  ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2;
  ಪೂರ್ಣಾಂಕ null;
  ಪೂರ್ಣಾಂಕ ret;

  ret = libkrun_parse_string_configuration (kconf->config_ವೃಕ್ಷ, container,
                                            "krun.tap_name", "tap_name",
                                            &kconf->tap_name, err, false);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (kconf->tap_name != ಖಾಲಿ && is_ಖಾಲಿಗಳು_string (kconf->tap_name))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "krun.tap_name cannot be empty");

  use_passt = libkrun_parse_resource_configuration (kconf->config_ವೃಕ್ಷ, container, "krun.use_passt", "use_passt", false);

  ಆದರೆ (use_passt > 0)
    {
      ಆದರೆ (kconf->tap_name != ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "krun.tap_name and krun.use_passt are mutually exclusive");

      kconf->use_passt = 1;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ 0;

  ret = socketpair (AF_UNIX, SOCK_ಹರಿವು, 0, kconf->passt_fds);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "create passt socketpair");

  snprintf (fd_as_str, ಗಾತ್ರ (fd_as_str), "%d", kconf->passt_fds[FD_PAIR_ಮಗು]);

  argv_idx = 0;
  passt_argv[argv_idx++] = (ಅಕ್ಷರ *) "passt";
  passt_argv[argv_idx++] = (ಅಕ್ಷರ *) "-t";
  passt_argv[argv_idx++] = (ಅಕ್ಷರ *) "all";

  ಆದರೆ (! kconf->has_awsnitro)
    {
      passt_argv[argv_idx++] = (ಅಕ್ಷರ *) "-u";
      passt_argv[argv_idx++] = (ಅಕ್ಷರ *) "all";
      passt_argv[argv_idx++] = (ಅಕ್ಷರ *) "--no-dhcp-dns";
    }

  /* Set --no-map-gw. If the gateway for the network is also the DNS server
   * we won't be able to query the DNS server without --no-map-gw.
   * See discussion in https://github.com/containers/crun/pull/2099
   */
  passt_argv[argv_idx++] = (ಅಕ್ಷರ *) "--no-map-gw";

  passt_argv[argv_idx++] = (ಅಕ್ಷರ *) "--fd";
  passt_argv[argv_idx++] = fd_as_str;
  passt_argv[argv_idx] = ಖಾಲಿ;

  pid = fork ();
  ಆದರೆ (pid < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fork passt");
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (pid == 0)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t i;

      close (kconf->passt_fds[FD_PAIR_ಪೋಷಕ]);

      null = open ("/dev/null", O_WRONLY);
      ಆದರೆ (null == -1)
        _exit (EXIT_FAILURE);

      /* Remount /proc if the parent detached its PID namespace, as passt(1)
       * needs a consistent /proc view to operate properly.
       */
      ಗಾಗಿ (i = 0; i < def->linux->namespaces_len; i++)
        {
          ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ = libcrun_ಕಂಡುಹಿಡಿ_namespace (def->linux->namespaces[i]->type);

          ಆದರೆ (ಮೌಲ್ಯ == CLONE_NEWPID)
            {
              ಆದರೆ (mount ("", "/proc", "proc", MS_NOSUID | MS_NODEV | MS_NOEXEC, ಖಾಲಿ))
                _exit (EXIT_FAILURE);

              ನಿಲ್ಲಿಸು;
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

  close (kconf->passt_fds[FD_PAIR_ಮಗು]);

  // Wait for passt to daemonize itself.
  waitpid (pid, &ಸ್ಥಿತಿ_2, 0);
  ಆದರೆ (! (WIFEXITED (ಸ್ಥಿತಿ_2)) || WEXITSTATUS (ಸ್ಥಿತಿ_2) != 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "start passt");

  ಹಿಂತಿರುಗಿ 0;
}

/* Return true if the spec already declares a device with the given PATH.  */
ಸ್ಥಿರ bool
spec_has_device (runtime_spec_schema_config_schema *def, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
  size_t i;

  ಗಾಗಿ (i = 0; i < def->linux->devices_len; i++)
    ಆದರೆ (strcmp (def->linux->devices[i]->ಮಾರ್ಗ, ಮಾರ್ಗ) == 0)
      ಹಿಂತಿರುಗಿ true;
  ಹಿಂತಿರುಗಿ false;
}

/* libkrun_create_kvm_device: explicitly adds kvm device.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libkrun_configure_container (ಶೂನ್ಯ *cookie, ಎಣಿಕೆ handler_configure_phase phase,
                             libcrun_context_t *context, libcrun_container_t *container,
                             ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret, rootfsfd;
  ರಚನೆ krun_config *kconf = (ರಚನೆ krun_config *) cookie;
  ರಚನೆ device_s kvm_device = { "/dev/kvm", "c", 10, 232, 0666, 0, 0 };
  ರಚನೆ device_s sev_device = { "/dev/sev", "c", 10, 124, 0666, 0, 0 };
  ರಚನೆ device_s nitro_device = { "/dev/nitro_enclaves", "c", 10, 122, 0666, 0, 0 };
  cleanup_close ಪೂರ್ಣಾಂಕ devfd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ rootfsfd_cleanup = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  bool create_sev = false, create_awsnitro = false;
  bool is_user_ns;

  ಆದರೆ (rootfs == ಖಾಲಿ)
    rootfsfd = AT_FDCWD;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      rootfsfd = rootfsfd_cleanup = open (rootfs, O_ಮಾರ್ಗ | O_CLOEXEC);
      ಆದರೆ (UNLIKELY (rootfsfd < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", rootfs);
    }

  ಆದರೆ (phase == HANDLER_CONFIGURE_BEFORE_USERNS)
    {
      cleanup_free ಅಕ್ಷರ *origin_config_ಮಾರ್ಗ = ಖಾಲಿ;
      cleanup_free ಅಕ್ಷರ *state_dir = ಖಾಲಿ;
      cleanup_free ಅಕ್ಷರ *config = ಖಾಲಿ;
      cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
      size_t config_ಗಾತ್ರ;

      ret = libcrun_get_state_directory (&state_dir, context->state_ಬೇರು, context->id, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = append_paths (&origin_config_ಮಾರ್ಗ, err, state_dir, "config.json", ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = read_all_file (origin_config_ಮಾರ್ಗ, &config, &config_ಗಾತ್ರ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      /* CVE-2025-24965: the content below rootfs cannot be trusted because it is controlled by the user.  We
         must ensure the file is opened below the rootfs directory.  Whatever the image ships at that name
         could also be a device node or a FIFO, so never open it: unlink it and create a new regular file.  */
      ret = unlinkat (rootfsfd, KRUN_CONFIG_FILE, 0);
      ಆದರೆ (UNLIKELY (ret < 0 && errno != ENOENT))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "unlink `%s`", KRUN_CONFIG_FILE);

      fd = ಸುರಕ್ಷಿತ_openat (rootfsfd, rootfs, KRUN_CONFIG_FILE, WRITE_FILE_DEFAULT_FLAGS | O_EXCL | O_NOFOLLOW, S_IRUSR | S_IRGRP | S_IROTH, err);
      ಆದರೆ (UNLIKELY (fd < 0))
        ಹಿಂತಿರುಗಿ fd;

      ret = ಸುರಕ್ಷಿತ_write (fd, KRUN_CONFIG_FILE, config, config_ಗಾತ್ರ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = libkrun_read_vm_config (kconf, rootfsfd, rootfs, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = libkrun_ಆರಂಭ_render_server (kconf, container, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    ಹಿಂತಿರುಗಿ 0;

  ret = libkrun_configure_ಜಾಲ (cookie, container, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  /* Do nothing if /dev/kvm is already present in spec */
  ಆದರೆ (spec_has_device (def, "/dev/kvm"))
    ಹಿಂತಿರುಗಿ 0;

  ಆದರೆ (kconf->handle_sev != ಖಾಲಿ)
    create_sev = ! spec_has_device (def, "/dev/sev");

  ಆದರೆ (kconf->handle_awsnitro != ಖಾಲಿ)
    create_awsnitro = ! spec_has_device (def, "/dev/nitro_enclaves");

  devfd = ಸುರಕ್ಷಿತ_openat (rootfsfd, rootfs, "dev", O_ಮಾರ್ಗ | O_DIRECTORY | O_CLOEXEC, 0, err);
  ಆದರೆ (UNLIKELY (devfd < 0))
    ಹಿಂತಿರುಗಿ devfd;

  ret = check_running_ಒಳ_user_namespace (err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;
  is_user_ns = ret;

  ಆದರೆ (kconf->has_kvm)
    {
      ret = libcrun_create_dev (container, devfd, -1, &kvm_device, is_user_ns, true, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (create_sev)
    {
      ret = libcrun_create_dev (container, devfd, -1, &sev_device, is_user_ns, true, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_sev);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
          kconf->handle_sev = ಖಾಲಿ;
        }
    }

  ಆದರೆ (create_awsnitro)
    {
      ret = libcrun_create_dev (container, devfd, -1, &nitro_device, is_user_ns, true, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_awsnitro);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
          kconf->handle_awsnitro = ಖಾಲಿ;
        }
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libkrun_ತುಂಬು (ಶೂನ್ಯ **cookie, libcrun_error_t *err)
{
  int32_t ret;
  ರಚನೆ krun_config *kconf;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libkrun_so = "libkrun.so.1";
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libkrun_sev_so = "libkrun-sev.so.1";
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libkrun_awsnitro_so = "libkrun-awsnitro.so.1";

  kconf = ಸ್ಮರಣೆ_ಕೊಡು (ಗಾತ್ರ (ರಚನೆ krun_config));
  ಆದರೆ (kconf == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "could not allocate memory for krun_config");
  memset (kconf, 0, ಗಾತ್ರ (ರಚನೆ krun_config));

  kconf->gpu_render_server_fd = -1;

  kconf->handle = dlopen (libkrun_so, RTLD_NOW);
  kconf->handle_sev = dlopen (libkrun_sev_so, RTLD_NOW);
  kconf->handle_awsnitro = dlopen (libkrun_awsnitro_so, RTLD_NOW);

  ಆದರೆ (kconf->handle == ಖಾಲಿ && kconf->handle_sev == ಖಾಲಿ && kconf->handle_awsnitro == ಖಾಲಿ)
    {
      ಬಿಡುಗಡೆ (kconf);
      ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "failed to open `%s`, `%s`, and `%s` for krun_config: %s", libkrun_so, libkrun_sev_so, libkrun_awsnitro_so, dlerror ());
    }

  kconf->sev = false;
  kconf->awsnitro = false;

  /* Newer versions of libkrun no longer link against libkrunfw and
     instead they open it when creating the context. This implies
     we need to call "krun_create_ctx" before switching namespaces
     or it won't be able to find the library bundling the kernel. */
  ಆದರೆ (kconf->handle)
    {
      ret = libkrun_create_context (kconf->handle, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹೋಗು error;
      kconf->ctx_id = ret;
    }

  ಆದರೆ (kconf->handle_sev)
    {
      ret = libkrun_create_context (kconf->handle_sev, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹೋಗು error;
      kconf->ctx_id_sev = ret;
    }
  ಆದರೆ (kconf->handle_awsnitro)
    {
      ret = libkrun_create_context (kconf->handle_awsnitro, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹೋಗು error;
      kconf->ctx_id_awsnitro = ret;
    }

  *cookie = kconf;

  ಹಿಂತಿರುಗಿ 0;

error:
  ಆದರೆ (kconf->handle)
    dlclose (kconf->handle);
  ಆದರೆ (kconf->handle_sev)
    dlclose (kconf->handle_sev);
  ಆದರೆ (kconf->handle_awsnitro)
    dlclose (kconf->handle_awsnitro);
  ಬಿಡುಗಡೆ (kconf);
  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libkrun_unload (ಶೂನ್ಯ *cookie, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ r;

  ರಚನೆ krun_config *kconf = (ರಚನೆ krun_config *) cookie;
  ಆದರೆ (kconf != ಖಾಲಿ)
    {
      ಆದರೆ (kconf->handle != ಖಾಲಿ)
        {
          r = dlclose (kconf->handle);
          ಆದರೆ (UNLIKELY (r != 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
        }
      ಆದರೆ (kconf->handle_sev != ಖಾಲಿ)
        {
          r = dlclose (kconf->handle_sev);
          ಆದರೆ (UNLIKELY (r != 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "could not unload handle_sev: `%s`", dlerror ());
        }
      ಆದರೆ (kconf->handle_awsnitro != ಖಾಲಿ)
        {
          r = dlclose (kconf->handle_awsnitro);
          ಆದರೆ (UNLIKELY (r != 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "could not unload handle_awsnitro: `%s`", dlerror ());
        }
      ಬಿಡುಗಡೆ (kconf);
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ runtime_spec_schema_defs_linux_device_cgroup *
make_oci_spec_dev (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *type, dev_t device, bool allow, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *access)
{
  runtime_spec_schema_defs_linux_device_cgroup *dev = xmalloc0 (ಗಾತ್ರ (*dev));

  dev->allow = allow;
  dev->allow_present = 1;

  dev->type = xstrdup (type);

  dev->major = major (device);
  dev->major_present = 1;

  dev->minor = minor (device);
  dev->minor_present = 1;

  dev->access = xstrdup (access);

  ಹಿಂತಿರುಗಿ dev;
}

/* stat an optional device: on ENOENT clear *PRESENT, any other error fails.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
stat_optional_device (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ರಚನೆ stat *st, bool *present, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret = stat (ಮಾರ್ಗ, st);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಆದರೆ (errno != ENOENT)
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "stat `%s`", ಮಾರ್ಗ);
      *present = false;
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libkrun_modify_oci_configuration (ಶೂನ್ಯ *cookie arg_ಬಳಸದ, libcrun_context_t *context arg_ಬಳಸದ,
                                  runtime_spec_schema_config_schema *def,
                                  libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ size_t device_ಗಾತ್ರ = ಗಾತ್ರ (runtime_spec_schema_defs_linux_device_cgroup);
  ರಚನೆ krun_config *kconf = (ರಚನೆ krun_config *) cookie;
  ರಚನೆ stat st_kvm, st_sev, st_awsnitro;
  bool has_kvm = true, has_sev = true, has_awsnitro = true;
  size_t old_len, new_len;
  ಪೂರ್ಣಾಂಕ ret;

  /* Always allow the /dev/kvm device.  */

  ret = stat_optional_device ("/dev/kvm", &st_kvm, &has_kvm, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = stat_optional_device ("/dev/sev", &st_sev, &has_sev, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = stat_optional_device ("/dev/nitro_enclaves", &st_awsnitro, &has_awsnitro, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  kconf->has_kvm = has_kvm;
  kconf->has_awsnitro = has_awsnitro;

  ಆದರೆ (! has_kvm && ! has_awsnitro)
    ಹಿಂತಿರುಗಿ 0;

  /* spec says these are optional, ensure they exist so we can add our devices */
  ಆದರೆ (def->linux == ಖಾಲಿ)
    def->linux = xmalloc0 (ಗಾತ್ರ (runtime_spec_schema_config_linux));

  ಆದರೆ (def->linux->resources == ಖಾಲಿ)
    def->linux->resources = xmalloc0 (ಗಾತ್ರ (runtime_spec_schema_config_linux_resources));

  old_len = def->linux->resources->devices_len;
  new_len = old_len;
  ಆದರೆ (has_kvm)
    new_len += has_sev ? 2 : 1;
  ಆದರೆ (has_awsnitro)
    new_len += 1;

  def->linux->resources->devices = xrealloc (def->linux->resources->devices, device_ಗಾತ್ರ * (new_len + 1));
  def->linux->resources->devices_len = new_len;

  ಆದರೆ (has_kvm)
    {
      def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_kvm.st_rdev, true, "rwm");
      ಆದರೆ (has_sev)
        def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_sev.st_rdev, true, "rwm");
    }

  ಆದರೆ (has_awsnitro)
    def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_awsnitro.st_rdev, true, "rwm");

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libkrun_close_fds (ಶೂನ್ಯ *cookie, libcrun_container_t *container, ಪೂರ್ಣಾಂಕ preserve_fds, libcrun_error_t *err)
{
  ರಚನೆ krun_config *kconf = (ರಚನೆ krun_config *) cookie;
  ಪೂರ್ಣಾಂಕ ಮೊದಲ_fd_ಗಮ್ಯ_close = preserve_fds + 3;
  ಪೂರ್ಣಾಂಕ ಕೆಳಗಿನ_fd, ಮೇಲಿನ_fd;
  ಪೂರ್ಣಾಂಕ i;

  ಕೆಳಗಿನ_fd = ಮೇಲಿನ_fd = -1;

  ಆದರೆ (kconf->use_passt)
    ಮೇಲಿನ_fd = kconf->passt_fds[FD_PAIR_ಪೋಷಕ];

  ಆದರೆ (kconf->gpu_render_server_fd != -1)
    {
      ಆದರೆ (kconf->gpu_render_server_fd > ಮೇಲಿನ_fd)
        {
          ಕೆಳಗಿನ_fd = ಮೇಲಿನ_fd;
          ಮೇಲಿನ_fd = kconf->gpu_render_server_fd;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        ಕೆಳಗಿನ_fd = kconf->gpu_render_server_fd;
    }

  ಆದರೆ (ಮೊದಲ_fd_ಗಮ್ಯ_close <= ಮೇಲಿನ_fd)
    {
      ಗಾಗಿ (i = ಮೊದಲ_fd_ಗಮ್ಯ_close; i < ಮೇಲಿನ_fd; i++)
        {
          // If we're closing proc_fd, make sure to invalidate it.
          ಆದರೆ (i == container->proc_fd)
            container->proc_fd = -1;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (i == ಕೆಳಗಿನ_fd)
            ಮುಂದುವರಿ;
          close (i);
        }

      ಮೊದಲ_fd_ಗಮ್ಯ_close = ಮೇಲಿನ_fd + 1;
    }

  ಹಿಂತಿರುಗಿ mark_or_close_fds_ge_than (container, ಮೊದಲ_fd_ಗಮ್ಯ_close, true, err);
}

ರಚನೆ custom_handler_s handler_libkrun = {
  .name = "krun",
  .alias = ಖಾಲಿ,
  .feature_string = "LIBKRUN",
  .supports_open_ವೃಕ್ಷ_namespace = false,
  .ತುಂಬು = libkrun_ತುಂಬು,
  .unload = libkrun_unload,
  .ಸತತ_func = libkrun_exec,
  .configure_container = libkrun_configure_container,
  .modify_oci_configuration = libkrun_modify_oci_configuration,
  .close_fds = libkrun_close_fds,
};

#ಆದರೆ_ಅಂತ್ಯ
