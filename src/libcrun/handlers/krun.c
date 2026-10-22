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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "../custom-handler.h"
#അന്തര്ഭാവ "../container.h"
#അന്തര്ഭാവ "../utils.h"
#അന്തര്ഭാവ "../linux.h"
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <sys/socket.h>
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <sys/mount.h>
#അന്തര്ഭാവ <sys/param.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <sys/sysmacros.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <sched.h>
#അന്തര്ഭാവ <ocispec/runtime_spec_schema_config_schema.h>

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

ഘടന krun_config
{
  ശൂന്യം *handle;
  ശൂന്യം *handle_sev;
  ശൂന്യം *handle_awsnitro;
  bool sev;
  bool awsnitro;
  int32_t ctx_id;
  int32_t ctx_id_sev;
  int32_t ctx_id_awsnitro;
  bool has_kvm;
  bool has_awsnitro;
  പൂർണ്ണസംഖ്യ passt_fds[2];
  json_object *config_doc;
  json_object *config_വൃക്ഷം;
  bool use_passt;
  സ്ഥിരം അക്ഷരം *tap_name;
  പൂർണ്ണസംഖ്യ gpu_render_server_fd;
  പൂർണ്ണസംഖ്യ gpu_flags;
};

/* libkrun handler.  */
#എങ്കിൽ HAVE_DLOPEN && HAVE_LIBKRUN
സ്ഥിതം int32_t
libkrun_create_context (ശൂന്യം *handle, libcrun_error_t *err)
{
  int32_t (*krun_create_ctx) ();
  int32_t ctx_id;

  krun_create_ctx = dlsym (handle, "krun_create_ctx");
  എങ്കിൽ (krun_create_ctx == ശൂന്യ)
    തിരികെ crun_make_error (err, 0, "could not find symbol in the krun library");

  ctx_id = krun_create_ctx ();
  എങ്കിൽ (UNLIKELY (ctx_id < 0))
    തിരികെ crun_make_error (err, -ctx_id, "could not create krun context");

  തിരികെ ctx_id;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libkrun_configure_kernel (uint32_t ctx_id, ശൂന്യം *handle, json_object *config_വൃക്ഷം, libcrun_error_t *err)
{
  int32_t (*krun_set_kernel) (uint32_t ctx_id, സ്ഥിരം അക്ഷരം *kernel_path,
                              uint32_t kernel_format, സ്ഥിരം അക്ഷരം *initrd_path, സ്ഥിരം അക്ഷരം *kernel_cmdline);
  json_object *kernel_path = ശൂന്യ;
  json_object *kernel_format = ശൂന്യ;
  json_object *val_initrd_path = ശൂന്യ;
  json_object *val_kernel_cmdline = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *initrd_path = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *kernel_cmdline = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;

  /* kernel_path and kernel_format must be present */
  kernel_path = json_object_object_get (config_വൃക്ഷം, "kernel_path");
  എങ്കിൽ (kernel_path == ശൂന്യ || ! json_object_is_type (kernel_path, json_type_string))
    തിരികെ 0;

  kernel_format = json_object_object_get (config_വൃക്ഷം, "kernel_format");
  എങ്കിൽ (kernel_format == ശൂന്യ || ! json_object_is_type (kernel_format, json_type_int))
    തിരികെ 0;

  /* initrd and kernel_cmdline are optional */
  val_initrd_path = json_object_object_get (config_വൃക്ഷം, "initrd_path");
  എങ്കിൽ (val_initrd_path != ശൂന്യ && json_object_is_type (val_initrd_path, json_type_string))
    initrd_path = json_object_get_string (val_initrd_path);

  val_kernel_cmdline = json_object_object_get (config_വൃക്ഷം, "kernel_cmdline");
  എങ്കിൽ (val_kernel_cmdline != ശൂന്യ && json_object_is_type (val_kernel_cmdline, json_type_string))
    kernel_cmdline = json_object_get_string (val_kernel_cmdline);

  krun_set_kernel = dlsym (handle, "krun_set_kernel");
  എങ്കിൽ (krun_set_kernel == ശൂന്യ)
    തിരികെ crun_make_error (err, 0, "could not find symbol in krun library");

  ret = krun_set_kernel (ctx_id,
                         json_object_get_string (kernel_path),
                         (uint32_t) json_object_get_int64 (kernel_format),
                         initrd_path, kernel_cmdline);

  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, -ret, "could not configure a krun external kernel");

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libkrun_read_vm_config (ഘടന krun_config *kconf, പൂർണ്ണസംഖ്യ rootfsfd, സ്ഥിരം അക്ഷരം *rootfs, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  cleanup_free അക്ഷരം *config = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ pathfd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
  proc_fd_path_t fd_path;
  ഘടന stat st;

  /* The content of the rootfs is controlled by the user: look at the file through an
     O_PATH descriptor first and refuse anything that is not a small regular file, as
     opening a FIFO blocks and a device node must not be accessed.  */
  pathfd = safe_openat (rootfsfd, rootfs, KRUN_VM_FILE, O_PATH | O_CLOEXEC | O_NOFOLLOW, 0, err);
  എങ്കിൽ (pathfd < 0)
    {
      // The configuration file is optional, don't generate an error if it's missing.
      എങ്കിൽ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          തിരികെ 0;
        }
      തിരികെ pathfd;
    }

  ret = fstat (pathfd, &st);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "stat `%s`", KRUN_VM_FILE);

  എങ്കിൽ (! S_ISREG (st.st_mode))
    തിരികെ crun_make_error (err, 0, "`%s` is not a regular file", KRUN_VM_FILE);

  get_proc_self_fd_path (fd_path, pathfd);

  fd = open (fd_path, O_RDONLY | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ crun_make_error (err, errno, "open `%s`", KRUN_VM_FILE);

  ret = read_all_fd_with_max_size (fd, KRUN_VM_FILE, &config, ശൂന്യ, KRUN_VM_FILE_MAX_SIZE, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = parse_json_file (&kconf->config_doc, config, ശൂന്യ, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  kconf->config_വൃക്ഷം = kconf->config_doc;
  തിരികെ 0;
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
സ്ഥിതം പൂർണ്ണസംഖ്യ
libkrun_parse_resource_configuration (json_object *config_വൃക്ഷം, libcrun_container_t *container, സ്ഥിരം അക്ഷരം *annotation, സ്ഥിരം അക്ഷരം *താക്കോൽ, bool use_config_file)
{
  അക്ഷരം *val_str, *endptr;
  പൂർണ്ണസംഖ്യ val = -1;
  json_object *val_json = ശൂന്യ;

  val_str = (അക്ഷരം *) find_annotation (container, annotation);
  എങ്കിൽ (val_str != ശൂന്യ)
    {
      errno = 0;
      val = (പൂർണ്ണസംഖ്യ) strtol (val_str, &endptr, 10);
      എങ്കിൽ (errno != 0 || endptr == val_str || *endptr != '\0')
        /* Annotations value is not a valid integer. */
        error (EXIT_FAILURE, 0, "krun annotation %s value cannot be converted to an integer", annotation);

      എങ്കിൽ (val < 0)
        error (EXIT_FAILURE, 0, "krun annotation %s value must be a positive integer", annotation);

      തിരികെ val;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (use_config_file && config_വൃക്ഷം != ശൂന്യ)
    {
      val_json = json_object_object_get (config_വൃക്ഷം, താക്കോൽ);
      എങ്കിൽ (val_json == ശൂന്യ)
        തിരികെ val;
      എങ്കിൽ (! json_object_is_type (val_json, json_type_int))
        error (EXIT_FAILURE, 0, "krun krun_vm.json %s value is not an integer", താക്കോൽ);

      val = (പൂർണ്ണസംഖ്യ) json_object_get_int64 (val_json);
    }

  തിരികെ val;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libkrun_parse_string_configuration (json_object *config_വൃക്ഷം, libcrun_container_t *container,
                                    സ്ഥിരം അക്ഷരം *annotation, സ്ഥിരം അക്ഷരം *താക്കോൽ,
                                    സ്ഥിരം അക്ഷരം **മൂല്യം, libcrun_error_t *err, bool use_config_file)
{
  സ്ഥിരം അക്ഷരം *val;
  json_object *val_json = ശൂന്യ;

  *മൂല്യം = ശൂന്യ;

  val = find_annotation (container, annotation);
  എങ്കിൽ (val != ശൂന്യ)
    {
      *മൂല്യം = val;
      തിരികെ 0;
    }

  എങ്കിൽ (! use_config_file || config_വൃക്ഷം == ശൂന്യ)
    തിരികെ 0;

  val_json = json_object_object_get (config_വൃക്ഷം, താക്കോൽ);
  എങ്കിൽ (val_json == ശൂന്യ)
    തിരികെ 0;

  എങ്കിൽ (! json_object_is_type (val_json, json_type_string))
    തിരികെ crun_make_error (err, 0, ".krun_vm.json %s value is not a string", താക്കോൽ);

  *മൂല്യം = json_object_get_string (val_json);
  തിരികെ 0;
}

സ്ഥിതം ശൂന്യം
libkrun_make_tap_mac (സ്ഥിരം അക്ഷരം *id, uint8_t mac[6])
{
  uint64_t hash = 14695981039346656037ULL;
  size_t i;

  വേണ്ടി (; *id != '\0'; id++)
    {
      hash ^= (uint8_t) *id;
      hash *= 1099511628211ULL;
    }

  വേണ്ടി (i = 0; i < 6; i++)
    mac[i] = (uint8_t) (hash >> (i * 8));

  mac[0] = (mac[0] & 0xfe) | 0x02;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libkrun_configure_vm (uint32_t ctx_id, ശൂന്യം *handle, ഘടന krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_set_vm_config) (uint32_t ctx_id, uint8_t അക്കം_vcpus, uint32_t ram_mib);
  int32_t (*krun_add_net_unixstream) (uint32_t ctx_id, സ്ഥിരം അക്ഷരം *c_path, പൂർണ്ണസംഖ്യ fd, uint8_t *സ്ഥിരം c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_add_net_tap) (uint32_t ctx_id, സ്ഥിരം അക്ഷരം *c_tap_name, സ്ഥിരം uint8_t *c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_set_gpu_options) (uint32_t ctx_id, uint32_t virgl_flags);
  int32_t (*krun_set_gpu_render_server_fd) (uint32_t ctx_id, പൂർണ്ണസംഖ്യ fd);
  പൂർണ്ണസംഖ്യ cpus, ram_mib, nested_virt, ret;
  cpu_set_t set;

  /* We let the OCI set the number of vCPUs for the VM, since cgroups CPU restrictions still apply. */
  cpus = libkrun_parse_resource_configuration (kconf->config_വൃക്ഷം, container, "krun.cpus", "cpus", true);
  എങ്കിൽ (cpus <= 0)
    {
      CPU_ZERO (&set);
      എങ്കിൽ (sched_getaffinity (getpid (), വലുപ്പം (set), &set) == 0)
        cpus = MIN (CPU_എണ്ണം (&set), LIBKRUN_MAX_VCPUS);
      അല്ലെങ്കിൽ
        cpus = 1;
    }

  /* We let the OCI set the amount of RAM for the VM, since cgroups memory restrictions still apply. */
  ram_mib = libkrun_parse_resource_configuration (kconf->config_വൃക്ഷം, container, "krun.ram_mib", "ram_mib", true);
  എങ്കിൽ (ram_mib <= LIBKRUN_MINIMUM_RAM_MIB)
    {
      എങ്കിൽ (def && def->linux && def->linux->resources && def->linux->resources->memory
          && def->linux->resources->memory->limit_present)
        ram_mib = def->linux->resources->memory->limit / (1024 * 1024);
      അല്ലെങ്കിൽ
        ram_mib = LIBKRUN_DEFAULT_RAM_MIB;
    }

  krun_set_vm_config = dlsym (handle, "krun_set_vm_config");

  എങ്കിൽ (krun_set_vm_config == ശൂന്യ)
    തിരികെ crun_make_error (err, 0, "could not find symbol in the krun library");

  ret = krun_set_vm_config (ctx_id, cpus, ram_mib);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, -ret, "could not set krun vm configuration");

  nested_virt = libkrun_parse_resource_configuration (kconf->config_വൃക്ഷം, container, "krun.nested_virt", "nested_virt", false);
  എങ്കിൽ (nested_virt > 0)
    {
      int32_t (*krun_check_nested_virt) (ശൂന്യം);
      int32_t (*krun_set_nested_virt) (uint32_t ctx_id, bool enabled);

      krun_check_nested_virt = dlsym (handle, "krun_check_nested_virt");
      എങ്കിൽ (krun_check_nested_virt != ശൂന്യ && krun_check_nested_virt () != 1)
        libcrun_warning ("nested virtualization requested but may not be supported on this host");

      krun_set_nested_virt = dlsym (handle, "krun_set_nested_virt");
      എങ്കിൽ (krun_set_nested_virt == ശൂന്യ)
        തിരികെ crun_make_error (err, 0, "could not find symbol `krun_set_nested_virt` in the krun library");

      ret = krun_set_nested_virt (ctx_id, true);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, -ret, "could not enable nested virtualization");
    }

  എങ്കിൽ (kconf->tap_name != ശൂന്യ)
    {
      krun_add_net_tap = dlsym (handle, "krun_add_net_tap");
      എങ്കിൽ (krun_add_net_tap == ശൂന്യ)
        തിരികെ crun_make_error (err, 0, "could not find symbol `krun_add_net_tap` in the krun library");

      uint8_t mac[6];
      libkrun_make_tap_mac (container->context->id, mac);
      ret = krun_add_net_tap (ctx_id, kconf->tap_name, &mac[0], COMPAT_NET_FEATURES, 0);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, -ret, "could not add krun TAP interface `%s`", kconf->tap_name);
    }
  അല്ലെങ്കിൽ എങ്കിൽ (kconf->use_passt)
    {
      krun_add_net_unixstream = dlsym (handle, "krun_add_net_unixstream");
      എങ്കിൽ (krun_add_net_unixstream == ശൂന്യ)
        തിരികെ crun_make_error (err, 0, "could not find symbol `krun_add_net_unixstream` in the krun library");

      uint8_t mac[] = { 0x5a, 0x94, 0xef, 0xe4, 0x0c, 0xee };
      ret = krun_add_net_unixstream (ctx_id, ശൂന്യ, kconf->passt_fds[FD_PAIR_PARENT], &mac[0], COMPAT_NET_FEATURES, NET_FLAG_DHCP_CLIENT);
      എങ്കിൽ (UNLIKELY (ret == -EINVAL))
        ret = krun_add_net_unixstream (ctx_id, ശൂന്യ, kconf->passt_fds[FD_PAIR_PARENT], &mac[0], COMPAT_NET_FEATURES, 0);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, -ret, "could not set krun net configuration");
    }

  എങ്കിൽ (kconf->gpu_flags > 0)
    {
      krun_set_gpu_options = dlsym (kconf->handle, "krun_set_gpu_options");
      എങ്കിൽ (krun_set_gpu_options == ശൂന്യ)
        തിരികെ crun_make_error (err, 0, "gpu requested but the version of libkrun in this system does not support it");

      ret = krun_set_gpu_options (kconf->ctx_id, kconf->gpu_flags);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, -ret, "gpu requested but could not configure virtio gpu device");

      എങ്കിൽ (kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER)
        {
          krun_set_gpu_render_server_fd = dlsym (kconf->handle, "krun_set_gpu_render_server_fd");
          എങ്കിൽ (krun_set_gpu_render_server_fd == ശൂന്യ)
            തിരികെ crun_make_error (err, 0, "gpu with render server requested but the version of libkrun in this system does not support it");

          ret = krun_set_gpu_render_server_fd (kconf->ctx_id, kconf->gpu_render_server_fd);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, -ret, "gpu with render server requested but could not configure its file descriptor");
        }
    }

  എങ്കിൽ (kconf->config_വൃക്ഷം != ശൂന്യ)
    {
      /* Try to configure an external kernel. If the configuration file doesn't
       * specify a kernel, libkrun automatically fall back to using libkrunfw,
       * if the library is present and was loaded while creating the context.
       */
      പൂർണ്ണസംഖ്യ custom_kernel = libkrun_parse_resource_configuration (kconf->config_വൃക്ഷം, container, "krun.custom_kernel", "custom_kernel", false);
      എങ്കിൽ (custom_kernel > 0)
        {
          ret = libkrun_configure_kernel (ctx_id, handle, kconf->config_വൃക്ഷം, err);
          എങ്കിൽ (UNLIKELY (ret))
            തിരികെ ret;
        }
    }

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libkrun_configure_flavor (ശൂന്യം *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret, sev_indicated = 0, awsnitro_indicated = 0;
  ഘടന krun_config *kconf = (ഘടന krun_config *) cookie;
  സ്ഥിരം അക്ഷരം *flavor = ശൂന്യ;
  ശൂന്യം *close_handles[2];

  close_handles[0] = ശൂന്യ;
  close_handles[1] = ശൂന്യ;

  // Check if the user provided the krun variant through OCI annotations.
  flavor = find_annotation (container, "krun.variant");
  എങ്കിൽ (flavor != ശൂന്യ)
    {
      sev_indicated |= strcmp (flavor, KRUN_FLAVOR_SEV) == 0;
      awsnitro_indicated |= strcmp (flavor, KRUN_FLAVOR_AWS_NITRO) == 0;
    }

  എങ്കിൽ (sev_indicated)
    {
      എങ്കിൽ (kconf->handle_sev == ശൂന്യ)
        error (EXIT_FAILURE, 0, "the container requires libkrun-sev but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_awsnitro;

      kconf->handle = kconf->handle_sev;
      kconf->ctx_id = kconf->ctx_id_sev;
      kconf->sev = true;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (awsnitro_indicated)
    {
      എങ്കിൽ (kconf->handle_awsnitro == ശൂന്യ)
        error (EXIT_FAILURE, 0, "the container requires libkrun-awsnitro but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_sev;

      kconf->handle = kconf->handle_awsnitro;
      kconf->ctx_id = kconf->ctx_id_awsnitro;
      kconf->awsnitro = true;
    }
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (kconf->handle == ശൂന്യ)
        error (EXIT_FAILURE, 0, "the container requires libkrun but it's not available");

      close_handles[0] = kconf->handle_sev;
      close_handles[1] = kconf->handle_awsnitro;
    }

  // We no longer need the other two libkrun handles.
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 2; i++)
    {
      എങ്കിൽ (close_handles[i] == ശൂന്യ)
        തുടരുക;

      ret = dlclose (close_handles[i]);
      എങ്കിൽ (UNLIKELY (ret != 0))
        തിരികെ crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libkrun_exec (ശൂന്യം *cookie, libcrun_container_t *container, സ്ഥിരം അക്ഷരം *pathname, അക്ഷരം *സ്ഥിരം argv[])
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_set_log_level) (uint32_t level);
  പൂർണ്ണസംഖ്യ (*krun_start_enter) (uint32_t ctx_id);
  int32_t (*krun_add_virtiofs2) (uint32_t ctx_id, സ്ഥിരം അക്ഷരം *c_tag, സ്ഥിരം അക്ഷരം *c_path, uint64_t shm_size);
  int32_t (*krun_set_വേര്_disk) (uint32_t ctx_id, സ്ഥിരം അക്ഷരം *disk_path);
  int32_t (*krun_set_tee_config_file) (uint32_t ctx_id, സ്ഥിരം അക്ഷരം *file_path);
  int32_t (*krun_set_console_output) (uint32_t ctx_id, സ്ഥിരം അക്ഷരം *c_filepath);
  int32_t (*krun_set_exec) (uint32_t ctx_id, സ്ഥിരം അക്ഷരം *exec_path,
                            സ്ഥിരം അക്ഷരം *സ്ഥിരം argv[], സ്ഥിരം അക്ഷരം *സ്ഥിരം envp[]);
  ഘടന krun_config *kconf = (ഘടന krun_config *) cookie;
  ശൂന്യം *handle;
  int32_t ctx_id, ret;
  libcrun_error_t err;

  ret = libkrun_configure_flavor (cookie, container, &err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      പൂർണ്ണസംഖ്യ errcode = crun_error_get_errno (&err);
      crun_error_release (&err);
      error (EXIT_FAILURE, errcode, "unable to configure libkrun flavor");
    }

  // /dev/kvm is required for all non AWS nitro workloads.
  എങ്കിൽ (! kconf->awsnitro && ! kconf->has_kvm)
    error (EXIT_FAILURE, 0, "`/dev/kvm` unavailable");

  handle = kconf->handle;
  ctx_id = kconf->ctx_id;

  krun_set_log_level = dlsym (handle, "krun_set_log_level");
  krun_start_enter = dlsym (handle, "krun_start_enter");
  എങ്കിൽ (krun_set_log_level == ശൂന്യ || krun_start_enter == ശൂന്യ)
    error (EXIT_FAILURE, 0, "could not find symbol in the krun library");

  /* Set log level according to crun's verbosity. */
  തിരഞ്ഞെടുപ്പ് (libcrun_get_verbosity ())
    {
    അവസ്ഥ LIBCRUN_VERBOSITY_DEBUG:
      krun_set_log_level (KRUN_LOG_LEVEL_DEBUG);
      നിർത്തുക;
    അവസ്ഥ LIBCRUN_VERBOSITY_WARNING:
      krun_set_log_level (KRUN_LOG_LEVEL_WARN);
      നിർത്തുക;
    സ്ഥിരസ്ഥിതി:
      krun_set_log_level (KRUN_LOG_LEVEL_ERROR);
      നിർത്തുക;
    }

  എങ്കിൽ (kconf->sev)
    {
      krun_set_വേര്_disk = dlsym (handle, "krun_set_root_disk");
      krun_set_tee_config_file = dlsym (handle, "krun_set_tee_config_file");
      എങ്കിൽ (krun_set_വേര്_disk == ശൂന്യ || krun_set_tee_config_file == ശൂന്യ)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-sev.so`");

      ret = krun_set_വേര്_disk (ctx_id, "/disk.img");
      എങ്കിൽ (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set root disk");

      ret = krun_set_tee_config_file (ctx_id, KRUN_SEV_FILE);
      എങ്കിൽ (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set krun tee config file");
    }
  അല്ലെങ്കിൽ
    {
      json_object *val_virtiofs_tag = ശൂന്യ;
      json_object *val_virtiofs_shm_size = ശൂന്യ;
      സ്ഥിരം അക്ഷരം *virtiofs_tag = ശൂന്യ;
      // Default to a conservative DAX size of 512MB, just like krun_set_root() does.
      uint64_t virtiofs_shm_size = 512 * 1024 * 1024ULL;

      എങ്കിൽ (kconf->config_വൃക്ഷം != ശൂന്യ)
        {
          val_virtiofs_tag = json_object_object_get (kconf->config_വൃക്ഷം, "virtiofs_tag");
          എങ്കിൽ (val_virtiofs_tag != ശൂന്യ && json_object_is_type (val_virtiofs_tag, json_type_string))
            virtiofs_tag = json_object_get_string (val_virtiofs_tag);

          val_virtiofs_shm_size = json_object_object_get (kconf->config_വൃക്ഷം, "virtiofs_shm_size");
          എങ്കിൽ (val_virtiofs_shm_size != ശൂന്യ && json_object_is_type (val_virtiofs_shm_size, json_type_int))
            virtiofs_shm_size = json_object_get_uint64 (val_virtiofs_shm_size);
        }

      എങ്കിൽ (virtiofs_tag == ശൂന്യ)
        virtiofs_tag = "/dev/root";

      krun_add_virtiofs2 = dlsym (handle, "krun_add_virtiofs2");

      എങ്കിൽ (krun_add_virtiofs2 == ശൂന്യ)
        error (EXIT_FAILURE, 0, "could not find symbol `krun_add_virtiofs2` in `libkrun.so`");

      ret = krun_add_virtiofs2 (ctx_id, virtiofs_tag, "/", virtiofs_shm_size);
      എങ്കിൽ (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not add virtiofs root with tag `%s`", virtiofs_tag);
    }

  എങ്കിൽ (kconf->awsnitro)
    {
      krun_set_console_output = dlsym (handle, "krun_set_console_output");
      krun_set_exec = dlsym (handle, "krun_set_exec");
      എങ്കിൽ (krun_set_console_output == ശൂന്യ || krun_set_exec == ശൂന്യ)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-awsnitro.so`");

      // Redirect all enclave output (read from vsock) to stdout.
      ret = krun_set_console_output (ctx_id, "/dev/stdout");
      എങ്കിൽ (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not redirect enclave output to stdout");

      ret = krun_set_exec (ctx_id, pathname, (സ്ഥിരം അക്ഷരം *സ്ഥിരം *) argv,
                           (സ്ഥിരം അക്ഷരം *സ്ഥിരം *) def->process->env);
      എങ്കിൽ (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set enclave execution arguments");
    }

  ret = libkrun_configure_vm (ctx_id, handle, kconf, container, &err);
  എങ്കിൽ (UNLIKELY (ret))
    {
      പൂർണ്ണസംഖ്യ errcode = crun_error_get_errno (&err);
      libcrun_error_t *tmp_err = &err;
      libcrun_error_report_and_release (tmp_err);
      error (EXIT_FAILURE, errcode, "could not configure krun vm");
    }

  json_object_put (kconf->config_doc);

  ret = krun_start_enter (ctx_id);
  എങ്കിൽ (UNLIKELY (ret < 0))
    error (EXIT_FAILURE, -ret, "could not start krun");

  തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libkrun_start_render_server (ഘടന krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  kconf->gpu_flags = libkrun_parse_resource_configuration (kconf->config_വൃക്ഷം, container, "krun.gpu_flags", "gpu_flags", false);
  എങ്കിൽ (kconf->gpu_flags > 0)
    {
      എങ്കിൽ (access ("/dev/dri", F_OK) != 0)
        തിരികെ crun_make_error (err, errno, "gpu requested but /dev/dri is not available");

      എങ്കിൽ ((kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER) != 0)
        {
          അക്ഷരം *rs_argv[40];
          പൂർണ്ണസംഖ്യ rs_argc = 0;
          അക്ഷരം fd_as_str[16];
          പൂർണ്ണസംഖ്യ gpu_fds[2];
          pid_t pid;
          പൂർണ്ണസംഖ്യ null;

          എങ്കിൽ (access ("/usr/libexec/virgl_render_server", X_OK) != 0)
            തിരികെ crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/libexec/virgl_render_server binary");

          എങ്കിൽ (access ("/usr/bin/bwrap", X_OK) != 0)
            തിരികെ crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/bin/bwrap binary");

          ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, gpu_fds);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "could not create socketpair for virgl_render_server");

          snprintf (fd_as_str, വലുപ്പം (fd_as_str), "%d", gpu_fds[FD_PAIR_CHILD]);

          rs_argv[rs_argc++] = (അക്ഷരം *) "bwrap";
          rs_argv[rs_argc++] = (അക്ഷരം *) "--ro-bind";
          rs_argv[rs_argc++] = (അക്ഷരം *) "/usr";
          rs_argv[rs_argc++] = (അക്ഷരം *) "/usr";
          എങ്കിൽ (access ("/lib", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (അക്ഷരം *) "--ro-bind";
              rs_argv[rs_argc++] = (അക്ഷരം *) "/lib";
              rs_argv[rs_argc++] = (അക്ഷരം *) "/lib";
            }
          എങ്കിൽ (access ("/lib64", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (അക്ഷരം *) "--ro-bind";
              rs_argv[rs_argc++] = (അക്ഷരം *) "/lib64";
              rs_argv[rs_argc++] = (അക്ഷരം *) "/lib64";
            }
          rs_argv[rs_argc++] = (അക്ഷരം *) "--ro-bind";
          rs_argv[rs_argc++] = (അക്ഷരം *) "/etc";
          rs_argv[rs_argc++] = (അക്ഷരം *) "/etc";
          rs_argv[rs_argc++] = (അക്ഷരം *) "--ro-bind";
          rs_argv[rs_argc++] = (അക്ഷരം *) "/sys";
          rs_argv[rs_argc++] = (അക്ഷരം *) "/sys";
          rs_argv[rs_argc++] = (അക്ഷരം *) "--dev";
          rs_argv[rs_argc++] = (അക്ഷരം *) "/dev";
          rs_argv[rs_argc++] = (അക്ഷരം *) "--dev-bind";
          rs_argv[rs_argc++] = (അക്ഷരം *) "/dev/dri";
          rs_argv[rs_argc++] = (അക്ഷരം *) "/dev/dri";
          rs_argv[rs_argc++] = (അക്ഷരം *) "--proc";
          rs_argv[rs_argc++] = (അക്ഷരം *) "/proc";
          rs_argv[rs_argc++] = (അക്ഷരം *) "--tmpfs";
          rs_argv[rs_argc++] = (അക്ഷരം *) "/tmp";
          rs_argv[rs_argc++] = (അക്ഷരം *) "--unshare-pid";
          rs_argv[rs_argc++] = (അക്ഷരം *) "--unshare-ipc";
          rs_argv[rs_argc++] = (അക്ഷരം *) "--unshare-net";
          rs_argv[rs_argc++] = (അക്ഷരം *) "--";
          rs_argv[rs_argc++] = (അക്ഷരം *) "/usr/libexec/virgl_render_server";
          rs_argv[rs_argc++] = (അക്ഷരം *) "--socket-fd";
          rs_argv[rs_argc++] = fd_as_str;
          rs_argv[rs_argc] = ശൂന്യ;

          pid = fork ();
          എങ്കിൽ (pid < 0)
            {
              close (gpu_fds[FD_PAIR_PARENT]);
              close (gpu_fds[FD_PAIR_CHILD]);
              തിരികെ crun_make_error (err, errno, "could not fork for virgl_render_server");
            }
          അല്ലെങ്കിൽ എങ്കിൽ (pid == 0)
            {
              close (gpu_fds[FD_PAIR_PARENT]);

              null = open ("/dev/null", O_WRONLY);
              എങ്കിൽ (null == -1)
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

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libkrun_configure_network (ശൂന്യം *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  ഘടന krun_config *kconf = (ഘടന krun_config *) cookie;
  pid_t pid;
  അക്ഷരം *passt_argv[10];
  അക്ഷരം fd_as_str[16];
  പൂർണ്ണസംഖ്യ use_passt;
  പൂർണ്ണസംഖ്യ argv_idx;
  പൂർണ്ണസംഖ്യ status;
  പൂർണ്ണസംഖ്യ null;
  പൂർണ്ണസംഖ്യ ret;

  ret = libkrun_parse_string_configuration (kconf->config_വൃക്ഷം, container,
                                            "krun.tap_name", "tap_name",
                                            &kconf->tap_name, err, false);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (kconf->tap_name != ശൂന്യ && is_empty_string (kconf->tap_name))
    തിരികെ crun_make_error (err, 0, "krun.tap_name cannot be empty");

  use_passt = libkrun_parse_resource_configuration (kconf->config_വൃക്ഷം, container, "krun.use_passt", "use_passt", false);

  എങ്കിൽ (use_passt > 0)
    {
      എങ്കിൽ (kconf->tap_name != ശൂന്യ)
        തിരികെ crun_make_error (err, 0, "krun.tap_name and krun.use_passt are mutually exclusive");

      kconf->use_passt = 1;
    }
  അല്ലെങ്കിൽ
    തിരികെ 0;

  ret = socketpair (AF_UNIX, SOCK_STREAM, 0, kconf->passt_fds);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "create passt socketpair");

  snprintf (fd_as_str, വലുപ്പം (fd_as_str), "%d", kconf->passt_fds[FD_PAIR_CHILD]);

  argv_idx = 0;
  passt_argv[argv_idx++] = (അക്ഷരം *) "passt";
  passt_argv[argv_idx++] = (അക്ഷരം *) "-t";
  passt_argv[argv_idx++] = (അക്ഷരം *) "all";

  എങ്കിൽ (! kconf->has_awsnitro)
    {
      passt_argv[argv_idx++] = (അക്ഷരം *) "-u";
      passt_argv[argv_idx++] = (അക്ഷരം *) "all";
      passt_argv[argv_idx++] = (അക്ഷരം *) "--no-dhcp-dns";
    }

  /* Set --no-map-gw. If the gateway for the network is also the DNS server
   * we won't be able to query the DNS server without --no-map-gw.
   * See discussion in https://github.com/containers/crun/pull/2099
   */
  passt_argv[argv_idx++] = (അക്ഷരം *) "--no-map-gw";

  passt_argv[argv_idx++] = (അക്ഷരം *) "--fd";
  passt_argv[argv_idx++] = fd_as_str;
  passt_argv[argv_idx] = ശൂന്യ;

  pid = fork ();
  എങ്കിൽ (pid < 0)
    തിരികെ crun_make_error (err, errno, "fork passt");
  അല്ലെങ്കിൽ എങ്കിൽ (pid == 0)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t i;

      close (kconf->passt_fds[FD_PAIR_PARENT]);

      null = open ("/dev/null", O_WRONLY);
      എങ്കിൽ (null == -1)
        _exit (EXIT_FAILURE);

      /* Remount /proc if the parent detached its PID namespace, as passt(1)
       * needs a consistent /proc view to operate properly.
       */
      വേണ്ടി (i = 0; i < def->linux->namespaces_len; i++)
        {
          പൂർണ്ണസംഖ്യ മൂല്യം = libcrun_find_namespace (def->linux->namespaces[i]->type);

          എങ്കിൽ (മൂല്യം == CLONE_NEWPID)
            {
              എങ്കിൽ (mount ("", "/proc", "proc", MS_NOSUID | MS_NODEV | MS_NOEXEC, ശൂന്യ))
                _exit (EXIT_FAILURE);

              നിർത്തുക;
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
  എങ്കിൽ (! (WIFEXITED (status)) || WEXITSTATUS (status) != 0)
    തിരികെ crun_make_error (err, 0, "start passt");

  തിരികെ 0;
}

/* Return true if the spec already declares a device with the given PATH.  */
സ്ഥിതം bool
spec_has_device (runtime_spec_schema_config_schema *def, സ്ഥിരം അക്ഷരം *path)
{
  size_t i;

  വേണ്ടി (i = 0; i < def->linux->devices_len; i++)
    എങ്കിൽ (strcmp (def->linux->devices[i]->path, path) == 0)
      തിരികെ true;
  തിരികെ false;
}

/* libkrun_create_kvm_device: explicitly adds kvm device.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
libkrun_configure_container (ശൂന്യം *cookie, എണ്ണൽ handler_configure_phase phase,
                             libcrun_context_t *context, libcrun_container_t *container,
                             സ്ഥിരം അക്ഷരം *rootfs, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret, rootfsfd;
  ഘടന krun_config *kconf = (ഘടന krun_config *) cookie;
  ഘടന device_s kvm_device = { "/dev/kvm", "c", 10, 232, 0666, 0, 0 };
  ഘടന device_s sev_device = { "/dev/sev", "c", 10, 124, 0666, 0, 0 };
  ഘടന device_s nitro_device = { "/dev/nitro_enclaves", "c", 10, 122, 0666, 0, 0 };
  cleanup_close പൂർണ്ണസംഖ്യ devfd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ rootfsfd_cleanup = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  bool create_sev = false, create_awsnitro = false;
  bool is_user_ns;

  എങ്കിൽ (rootfs == ശൂന്യ)
    rootfsfd = AT_FDCWD;
  അല്ലെങ്കിൽ
    {
      rootfsfd = rootfsfd_cleanup = open (rootfs, O_PATH | O_CLOEXEC);
      എങ്കിൽ (UNLIKELY (rootfsfd < 0))
        തിരികെ crun_make_error (err, errno, "open `%s`", rootfs);
    }

  എങ്കിൽ (phase == HANDLER_CONFIGURE_BEFORE_USERNS)
    {
      cleanup_free അക്ഷരം *origin_config_path = ശൂന്യ;
      cleanup_free അക്ഷരം *state_dir = ശൂന്യ;
      cleanup_free അക്ഷരം *config = ശൂന്യ;
      cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
      size_t config_size;

      ret = libcrun_get_state_directory (&state_dir, context->state_വേര്, context->id, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = append_paths (&origin_config_path, err, state_dir, "config.json", ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = read_all_file (origin_config_path, &config, &config_size, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      /* CVE-2025-24965: the content below rootfs cannot be trusted because it is controlled by the user.  We
         must ensure the file is opened below the rootfs directory.  Whatever the image ships at that name
         could also be a device node or a FIFO, so never open it: unlink it and create a new regular file.  */
      ret = unlinkat (rootfsfd, KRUN_CONFIG_FILE, 0);
      എങ്കിൽ (UNLIKELY (ret < 0 && errno != ENOENT))
        തിരികെ crun_make_error (err, errno, "unlink `%s`", KRUN_CONFIG_FILE);

      fd = safe_openat (rootfsfd, rootfs, KRUN_CONFIG_FILE, WRITE_FILE_DEFAULT_FLAGS | O_EXCL | O_NOFOLLOW, S_IRUSR | S_IRGRP | S_IROTH, err);
      എങ്കിൽ (UNLIKELY (fd < 0))
        തിരികെ fd;

      ret = safe_write (fd, KRUN_CONFIG_FILE, config, config_size, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = libkrun_read_vm_config (kconf, rootfsfd, rootfs, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = libkrun_start_render_server (kconf, container, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    തിരികെ 0;

  ret = libkrun_configure_network (cookie, container, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  /* Do nothing if /dev/kvm is already present in spec */
  എങ്കിൽ (spec_has_device (def, "/dev/kvm"))
    തിരികെ 0;

  എങ്കിൽ (kconf->handle_sev != ശൂന്യ)
    create_sev = ! spec_has_device (def, "/dev/sev");

  എങ്കിൽ (kconf->handle_awsnitro != ശൂന്യ)
    create_awsnitro = ! spec_has_device (def, "/dev/nitro_enclaves");

  devfd = safe_openat (rootfsfd, rootfs, "dev", O_PATH | O_DIRECTORY | O_CLOEXEC, 0, err);
  എങ്കിൽ (UNLIKELY (devfd < 0))
    തിരികെ devfd;

  ret = check_running_in_user_namespace (err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;
  is_user_ns = ret;

  എങ്കിൽ (kconf->has_kvm)
    {
      ret = libcrun_create_dev (container, devfd, -1, &kvm_device, is_user_ns, true, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (create_sev)
    {
      ret = libcrun_create_dev (container, devfd, -1, &sev_device, is_user_ns, true, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_sev);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
          kconf->handle_sev = ശൂന്യ;
        }
    }

  എങ്കിൽ (create_awsnitro)
    {
      ret = libcrun_create_dev (container, devfd, -1, &nitro_device, is_user_ns, true, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_awsnitro);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
          kconf->handle_awsnitro = ശൂന്യ;
        }
    }

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libkrun_load (ശൂന്യം **cookie, libcrun_error_t *err)
{
  int32_t ret;
  ഘടന krun_config *kconf;
  സ്ഥിരം അക്ഷരം *libkrun_so = "libkrun.so.1";
  സ്ഥിരം അക്ഷരം *libkrun_sev_so = "libkrun-sev.so.1";
  സ്ഥിരം അക്ഷരം *libkrun_awsnitro_so = "libkrun-awsnitro.so.1";

  kconf = malloc (വലുപ്പം (ഘടന krun_config));
  എങ്കിൽ (kconf == ശൂന്യ)
    തിരികെ crun_make_error (err, 0, "could not allocate memory for krun_config");
  memset (kconf, 0, വലുപ്പം (ഘടന krun_config));

  kconf->gpu_render_server_fd = -1;

  kconf->handle = dlopen (libkrun_so, RTLD_NOW);
  kconf->handle_sev = dlopen (libkrun_sev_so, RTLD_NOW);
  kconf->handle_awsnitro = dlopen (libkrun_awsnitro_so, RTLD_NOW);

  എങ്കിൽ (kconf->handle == ശൂന്യ && kconf->handle_sev == ശൂന്യ && kconf->handle_awsnitro == ശൂന്യ)
    {
      സ്വതന്ത്രം (kconf);
      തിരികെ crun_make_error (err, 0, "failed to open `%s`, `%s`, and `%s` for krun_config: %s", libkrun_so, libkrun_sev_so, libkrun_awsnitro_so, dlerror ());
    }

  kconf->sev = false;
  kconf->awsnitro = false;

  /* Newer versions of libkrun no longer link against libkrunfw and
     instead they open it when creating the context. This implies
     we need to call "krun_create_ctx" before switching namespaces
     or it won't be able to find the library bundling the kernel. */
  എങ്കിൽ (kconf->handle)
    {
      ret = libkrun_create_context (kconf->handle, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        പോകുക error;
      kconf->ctx_id = ret;
    }

  എങ്കിൽ (kconf->handle_sev)
    {
      ret = libkrun_create_context (kconf->handle_sev, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        പോകുക error;
      kconf->ctx_id_sev = ret;
    }
  എങ്കിൽ (kconf->handle_awsnitro)
    {
      ret = libkrun_create_context (kconf->handle_awsnitro, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        പോകുക error;
      kconf->ctx_id_awsnitro = ret;
    }

  *cookie = kconf;

  തിരികെ 0;

error:
  എങ്കിൽ (kconf->handle)
    dlclose (kconf->handle);
  എങ്കിൽ (kconf->handle_sev)
    dlclose (kconf->handle_sev);
  എങ്കിൽ (kconf->handle_awsnitro)
    dlclose (kconf->handle_awsnitro);
  സ്വതന്ത്രം (kconf);
  തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libkrun_unload (ശൂന്യം *cookie, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ r;

  ഘടന krun_config *kconf = (ഘടന krun_config *) cookie;
  എങ്കിൽ (kconf != ശൂന്യ)
    {
      എങ്കിൽ (kconf->handle != ശൂന്യ)
        {
          r = dlclose (kconf->handle);
          എങ്കിൽ (UNLIKELY (r != 0))
            തിരികെ crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
        }
      എങ്കിൽ (kconf->handle_sev != ശൂന്യ)
        {
          r = dlclose (kconf->handle_sev);
          എങ്കിൽ (UNLIKELY (r != 0))
            തിരികെ crun_make_error (err, 0, "could not unload handle_sev: `%s`", dlerror ());
        }
      എങ്കിൽ (kconf->handle_awsnitro != ശൂന്യ)
        {
          r = dlclose (kconf->handle_awsnitro);
          എങ്കിൽ (UNLIKELY (r != 0))
            തിരികെ crun_make_error (err, 0, "could not unload handle_awsnitro: `%s`", dlerror ());
        }
      സ്വതന്ത്രം (kconf);
    }
  തിരികെ 0;
}

സ്ഥിതം runtime_spec_schema_defs_linux_device_cgroup *
make_oci_spec_dev (സ്ഥിരം അക്ഷരം *type, dev_t device, bool allow, സ്ഥിരം അക്ഷരം *access)
{
  runtime_spec_schema_defs_linux_device_cgroup *dev = xmalloc0 (വലുപ്പം (*dev));

  dev->allow = allow;
  dev->allow_present = 1;

  dev->type = xstrdup (type);

  dev->major = major (device);
  dev->major_present = 1;

  dev->minor = minor (device);
  dev->minor_present = 1;

  dev->access = xstrdup (access);

  തിരികെ dev;
}

/* stat an optional device: on ENOENT clear *PRESENT, any other error fails.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
stat_optional_device (സ്ഥിരം അക്ഷരം *path, ഘടന stat *st, bool *present, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret = stat (path, st);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      എങ്കിൽ (errno != ENOENT)
        തിരികെ crun_make_error (err, errno, "stat `%s`", path);
      *present = false;
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libkrun_modify_oci_configuration (ശൂന്യം *cookie arg_unused, libcrun_context_t *context arg_unused,
                                  runtime_spec_schema_config_schema *def,
                                  libcrun_error_t *err)
{
  സ്ഥിരം size_t device_size = വലുപ്പം (runtime_spec_schema_defs_linux_device_cgroup);
  ഘടന krun_config *kconf = (ഘടന krun_config *) cookie;
  ഘടന stat st_kvm, st_sev, st_awsnitro;
  bool has_kvm = true, has_sev = true, has_awsnitro = true;
  size_t old_len, new_len;
  പൂർണ്ണസംഖ്യ ret;

  /* Always allow the /dev/kvm device.  */

  ret = stat_optional_device ("/dev/kvm", &st_kvm, &has_kvm, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = stat_optional_device ("/dev/sev", &st_sev, &has_sev, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = stat_optional_device ("/dev/nitro_enclaves", &st_awsnitro, &has_awsnitro, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  kconf->has_kvm = has_kvm;
  kconf->has_awsnitro = has_awsnitro;

  എങ്കിൽ (! has_kvm && ! has_awsnitro)
    തിരികെ 0;

  /* spec says these are optional, ensure they exist so we can add our devices */
  എങ്കിൽ (def->linux == ശൂന്യ)
    def->linux = xmalloc0 (വലുപ്പം (runtime_spec_schema_config_linux));

  എങ്കിൽ (def->linux->resources == ശൂന്യ)
    def->linux->resources = xmalloc0 (വലുപ്പം (runtime_spec_schema_config_linux_resources));

  old_len = def->linux->resources->devices_len;
  new_len = old_len;
  എങ്കിൽ (has_kvm)
    new_len += has_sev ? 2 : 1;
  എങ്കിൽ (has_awsnitro)
    new_len += 1;

  def->linux->resources->devices = xrealloc (def->linux->resources->devices, device_size * (new_len + 1));
  def->linux->resources->devices_len = new_len;

  എങ്കിൽ (has_kvm)
    {
      def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_kvm.st_rdev, true, "rwm");
      എങ്കിൽ (has_sev)
        def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_sev.st_rdev, true, "rwm");
    }

  എങ്കിൽ (has_awsnitro)
    def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_awsnitro.st_rdev, true, "rwm");

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libkrun_close_fds (ശൂന്യം *cookie, libcrun_container_t *container, പൂർണ്ണസംഖ്യ preserve_fds, libcrun_error_t *err)
{
  ഘടന krun_config *kconf = (ഘടന krun_config *) cookie;
  പൂർണ്ണസംഖ്യ first_fd_to_close = preserve_fds + 3;
  പൂർണ്ണസംഖ്യ താഴെയുള്ള_fd, മുകളിലെ_fd;
  പൂർണ്ണസംഖ്യ i;

  താഴെയുള്ള_fd = മുകളിലെ_fd = -1;

  എങ്കിൽ (kconf->use_passt)
    മുകളിലെ_fd = kconf->passt_fds[FD_PAIR_PARENT];

  എങ്കിൽ (kconf->gpu_render_server_fd != -1)
    {
      എങ്കിൽ (kconf->gpu_render_server_fd > മുകളിലെ_fd)
        {
          താഴെയുള്ള_fd = മുകളിലെ_fd;
          മുകളിലെ_fd = kconf->gpu_render_server_fd;
        }
      അല്ലെങ്കിൽ
        താഴെയുള്ള_fd = kconf->gpu_render_server_fd;
    }

  എങ്കിൽ (first_fd_to_close <= മുകളിലെ_fd)
    {
      വേണ്ടി (i = first_fd_to_close; i < മുകളിലെ_fd; i++)
        {
          // If we're closing proc_fd, make sure to invalidate it.
          എങ്കിൽ (i == container->proc_fd)
            container->proc_fd = -1;
          അല്ലെങ്കിൽ എങ്കിൽ (i == താഴെയുള്ള_fd)
            തുടരുക;
          close (i);
        }

      first_fd_to_close = മുകളിലെ_fd + 1;
    }

  തിരികെ mark_or_close_fds_ge_than (container, first_fd_to_close, true, err);
}

ഘടന custom_handler_s handler_libkrun = {
  .name = "krun",
  .alias = ശൂന്യ,
  .feature_string = "LIBKRUN",
  .supports_open_വൃക്ഷം_namespace = false,
  .load = libkrun_load,
  .unload = libkrun_unload,
  .run_func = libkrun_exec,
  .configure_container = libkrun_configure_container,
  .modify_oci_configuration = libkrun_modify_oci_configuration,
  .close_fds = libkrun_close_fds,
};

#endif
