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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "../custom-handler.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../container.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "../linux.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/socket.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/mount.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/param.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/sysmacros.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sched.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ocispec/runtime_spec_schema_config_schema.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DLOPEN
#  include <dlfcn.h>
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_LIBKRUN
#  include <libkrun.h>
#ਜੇ_ਅੰਤ

/* This allows us to build even with headers from older libkrun versions.
 * If the installed version of libkrun doesn't support this feature,
 * krun_add_net_unixstream() will return EINVAL.
 */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NET_FLAG_DHCP_CLIENT
#  define NET_FLAG_DHCP_CLIENT (1 << 1)
#ਜੇ_ਅੰਤ
/* libkrun has a hard-limit of 16 vCPUs per microVM. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LIBKRUN_MAX_VCPUS 16

/* If the user doesn't configure the RAM amount, fallback to this value. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LIBKRUN_DEFAULT_RAM_MIB 1024

/* The minimum amount of RAM for a viable microVM is 128 MB. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LIBKRUN_MINIMUM_RAM_MIB 128

/* crun dumps the container configuration into this file, which will be read by
 * libkrun to set up the environment for the workload inside the microVM.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ KRUN_CONFIG_FILE ".krun_config.json"

/* The presence of this file indicates this is a container intended to be run
 * as a confidential workload inside a SEV-powered TEE.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ KRUN_SEV_FILE "/krun-sev.json"

/* This file contains configuration parameters for the microVM. crun needs to
 * read and parse it, using the information obtained from it to configure
 * libkrun as required.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ KRUN_VM_FILE "/.krun_vm.json"

/* KRUN_VM_FILE holds a handful of settings and it is parsed by crun
 * itself before the container process exists, refuse anything bigger.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ KRUN_VM_FILE_MAX_ਮਾਪ (64 * 1024)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ KRUN_FLAVOR_AWS_NITRO "aws-nitro"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ KRUN_FLAVOR_SEV "sev"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FD_PAIR_ਮਾਪਾ 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FD_PAIR_ਬੱਚਾ 1

ਬਣਤਰ krun_config
{
  ਖਾਲੀ *handle;
  ਖਾਲੀ *handle_sev;
  ਖਾਲੀ *handle_awsnitro;
  bool sev;
  bool awsnitro;
  int32_t ctx_id;
  int32_t ctx_id_sev;
  int32_t ctx_id_awsnitro;
  bool has_kvm;
  bool has_awsnitro;
  ਪੂਰਨ passt_fds[2];
  json_object *config_doc;
  json_object *config_ਰੁੱਖ;
  bool use_passt;
  ਅਚਲ ਅੱਖਰ *tap_name;
  ਪੂਰਨ gpu_render_server_fd;
  ਪੂਰਨ gpu_flags;
};

/* libkrun handler.  */
#ਜੇ HAVE_DLOPEN && HAVE_LIBKRUN
ਸਥਿਰ int32_t
libkrun_create_context (ਖਾਲੀ *handle, libcrun_error_t *err)
{
  int32_t (*krun_create_ctx) ();
  int32_t ctx_id;

  krun_create_ctx = dlsym (handle, "krun_create_ctx");
  ਜੇ (krun_create_ctx == ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, 0, "could not find symbol in the krun library");

  ctx_id = krun_create_ctx ();
  ਜੇ (UNLIKELY (ctx_id < 0))
    ਵਾਪਸ crun_make_error (err, -ctx_id, "could not create krun context");

  ਵਾਪਸ ctx_id;
}

ਸਥਿਰ ਪੂਰਨ
libkrun_configure_kernel (uint32_t ctx_id, ਖਾਲੀ *handle, json_object *config_ਰੁੱਖ, libcrun_error_t *err)
{
  int32_t (*krun_ਸੈੱਟ_kernel) (uint32_t ctx_id, ਅਚਲ ਅੱਖਰ *kernel_ਰਸਤਾ,
                              uint32_t kernel_format, ਅਚਲ ਅੱਖਰ *initrd_ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *kernel_cmdline);
  json_object *kernel_ਰਸਤਾ = ਨਿਰਾ;
  json_object *kernel_format = ਨਿਰਾ;
  json_object *val_initrd_ਰਸਤਾ = ਨਿਰਾ;
  json_object *val_kernel_cmdline = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *initrd_ਰਸਤਾ = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *kernel_cmdline = ਨਿਰਾ;
  ਪੂਰਨ ret;

  /* kernel_path and kernel_format must be present */
  kernel_ਰਸਤਾ = json_object_object_get (config_ਰੁੱਖ, "kernel_path");
  ਜੇ (kernel_ਰਸਤਾ == ਨਿਰਾ || ! json_object_is_type (kernel_ਰਸਤਾ, json_type_string))
    ਵਾਪਸ 0;

  kernel_format = json_object_object_get (config_ਰੁੱਖ, "kernel_format");
  ਜੇ (kernel_format == ਨਿਰਾ || ! json_object_is_type (kernel_format, json_type_int))
    ਵਾਪਸ 0;

  /* initrd and kernel_cmdline are optional */
  val_initrd_ਰਸਤਾ = json_object_object_get (config_ਰੁੱਖ, "initrd_path");
  ਜੇ (val_initrd_ਰਸਤਾ != ਨਿਰਾ && json_object_is_type (val_initrd_ਰਸਤਾ, json_type_string))
    initrd_ਰਸਤਾ = json_object_get_string (val_initrd_ਰਸਤਾ);

  val_kernel_cmdline = json_object_object_get (config_ਰੁੱਖ, "kernel_cmdline");
  ਜੇ (val_kernel_cmdline != ਨਿਰਾ && json_object_is_type (val_kernel_cmdline, json_type_string))
    kernel_cmdline = json_object_get_string (val_kernel_cmdline);

  krun_ਸੈੱਟ_kernel = dlsym (handle, "krun_set_kernel");
  ਜੇ (krun_ਸੈੱਟ_kernel == ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, 0, "could not find symbol in krun library");

  ret = krun_ਸੈੱਟ_kernel (ctx_id,
                         json_object_get_string (kernel_ਰਸਤਾ),
                         (uint32_t) json_object_get_int64 (kernel_format),
                         initrd_ਰਸਤਾ, kernel_cmdline);

  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, -ret, "could not configure a krun external kernel");

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
libkrun_read_vm_config (ਬਣਤਰ krun_config *kconf, ਪੂਰਨ rootfsfd, ਅਚਲ ਅੱਖਰ *rootfs, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  cleanup_free ਅੱਖਰ *config = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ pathfd = -1;
  cleanup_close ਪੂਰਨ fd = -1;
  proc_fd_ਰਸਤਾ_t fd_ਰਸਤਾ;
  ਬਣਤਰ stat st;

  /* The content of the rootfs is controlled by the user: look at the file through an
     O_PATH descriptor first and refuse anything that is not a small regular file, as
     opening a FIFO blocks and a device node must not be accessed.  */
  pathfd = ਸੁਰੱਖਿਅਤ_openat (rootfsfd, rootfs, KRUN_VM_FILE, O_ਰਸਤਾ | O_CLOEXEC | O_NOFOLLOW, 0, err);
  ਜੇ (pathfd < 0)
    {
      // The configuration file is optional, don't generate an error if it's missing.
      ਜੇ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ਵਾਪਸ 0;
        }
      ਵਾਪਸ pathfd;
    }

  ret = fstat (pathfd, &st);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "stat `%s`", KRUN_VM_FILE);

  ਜੇ (! S_ISREG (st.st_mode))
    ਵਾਪਸ crun_make_error (err, 0, "`%s` is not a regular file", KRUN_VM_FILE);

  get_proc_self_fd_ਰਸਤਾ (fd_ਰਸਤਾ, pathfd);

  fd = open (fd_ਰਸਤਾ, O_RDONLY | O_CLOEXEC);
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s`", KRUN_VM_FILE);

  ret = read_all_fd_with_max_ਮਾਪ (fd, KRUN_VM_FILE, &config, ਨਿਰਾ, KRUN_VM_FILE_MAX_ਮਾਪ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = parse_json_file (&kconf->config_doc, config, ਨਿਰਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  kconf->config_ਰੁੱਖ = kconf->config_doc;
  ਵਾਪਸ 0;
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
ਸਥਿਰ ਪੂਰਨ
libkrun_parse_resource_configuration (json_object *config_ਰੁੱਖ, libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *annotation, ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ, bool use_config_file)
{
  ਅੱਖਰ *val_str, *endptr;
  ਪੂਰਨ val = -1;
  json_object *val_json = ਨਿਰਾ;

  val_str = (ਅੱਖਰ *) ਲੱਭੋ_annotation (container, annotation);
  ਜੇ (val_str != ਨਿਰਾ)
    {
      errno = 0;
      val = (ਪੂਰਨ) strtol (val_str, &endptr, 10);
      ਜੇ (errno != 0 || endptr == val_str || *endptr != '\0')
        /* Annotations value is not a valid integer. */
        error (EXIT_FAILURE, 0, "krun annotation %s value cannot be converted to an integer", annotation);

      ਜੇ (val < 0)
        error (EXIT_FAILURE, 0, "krun annotation %s value must be a positive integer", annotation);

      ਵਾਪਸ val;
    }
  ਨਹੀਂਤਾਂ ਜੇ (use_config_file && config_ਰੁੱਖ != ਨਿਰਾ)
    {
      val_json = json_object_object_get (config_ਰੁੱਖ, ਕੁੰਜੀ);
      ਜੇ (val_json == ਨਿਰਾ)
        ਵਾਪਸ val;
      ਜੇ (! json_object_is_type (val_json, json_type_int))
        error (EXIT_FAILURE, 0, "krun krun_vm.json %s value is not an integer", ਕੁੰਜੀ);

      val = (ਪੂਰਨ) json_object_get_int64 (val_json);
    }

  ਵਾਪਸ val;
}

ਸਥਿਰ ਪੂਰਨ
libkrun_parse_string_configuration (json_object *config_ਰੁੱਖ, libcrun_container_t *container,
                                    ਅਚਲ ਅੱਖਰ *annotation, ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ,
                                    ਅਚਲ ਅੱਖਰ **ਮੁੱਲ, libcrun_error_t *err, bool use_config_file)
{
  ਅਚਲ ਅੱਖਰ *val;
  json_object *val_json = ਨਿਰਾ;

  *ਮੁੱਲ = ਨਿਰਾ;

  val = ਲੱਭੋ_annotation (container, annotation);
  ਜੇ (val != ਨਿਰਾ)
    {
      *ਮੁੱਲ = val;
      ਵਾਪਸ 0;
    }

  ਜੇ (! use_config_file || config_ਰੁੱਖ == ਨਿਰਾ)
    ਵਾਪਸ 0;

  val_json = json_object_object_get (config_ਰੁੱਖ, ਕੁੰਜੀ);
  ਜੇ (val_json == ਨਿਰਾ)
    ਵਾਪਸ 0;

  ਜੇ (! json_object_is_type (val_json, json_type_string))
    ਵਾਪਸ crun_make_error (err, 0, ".krun_vm.json %s value is not a string", ਕੁੰਜੀ);

  *ਮੁੱਲ = json_object_get_string (val_json);
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ
libkrun_make_tap_mac (ਅਚਲ ਅੱਖਰ *id, uint8_t mac[6])
{
  uint64_t hash = 14695981039346656037ULL;
  size_t i;

  ਲਈ (; *id != '\0'; id++)
    {
      hash ^= (uint8_t) *id;
      hash *= 1099511628211ULL;
    }

  ਲਈ (i = 0; i < 6; i++)
    mac[i] = (uint8_t) (hash >> (i * 8));

  mac[0] = (mac[0] & 0xfe) | 0x02;
}

ਸਥਿਰ ਪੂਰਨ
libkrun_configure_vm (uint32_t ctx_id, ਖਾਲੀ *handle, ਬਣਤਰ krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_ਸੈੱਟ_vm_config) (uint32_t ctx_id, uint8_t ਅੰਕ_vcpus, uint32_t ram_mib);
  int32_t (*krun_ਜੋੜੋ_net_unixstream) (uint32_t ctx_id, ਅਚਲ ਅੱਖਰ *c_ਰਸਤਾ, ਪੂਰਨ fd, uint8_t *ਅਚਲ c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_ਜੋੜੋ_net_tap) (uint32_t ctx_id, ਅਚਲ ਅੱਖਰ *c_tap_name, ਅਚਲ uint8_t *c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_ਸੈੱਟ_gpu_options) (uint32_t ctx_id, uint32_t virgl_flags);
  int32_t (*krun_ਸੈੱਟ_gpu_render_server_fd) (uint32_t ctx_id, ਪੂਰਨ fd);
  ਪੂਰਨ cpus, ram_mib, nested_virt, ret;
  cpu_ਸੈੱਟ_t ਸੈੱਟ;

  /* We let the OCI set the number of vCPUs for the VM, since cgroups CPU restrictions still apply. */
  cpus = libkrun_parse_resource_configuration (kconf->config_ਰੁੱਖ, container, "krun.cpus", "cpus", true);
  ਜੇ (cpus <= 0)
    {
      CPU_ZERO (&ਸੈੱਟ);
      ਜੇ (sched_getaffinity (getpid (), ਆਕਾਰ (ਸੈੱਟ), &ਸੈੱਟ) == 0)
        cpus = MIN (CPU_ਗਿਣਤੀ (&ਸੈੱਟ), LIBKRUN_MAX_VCPUS);
      ਨਹੀਂਤਾਂ
        cpus = 1;
    }

  /* We let the OCI set the amount of RAM for the VM, since cgroups memory restrictions still apply. */
  ram_mib = libkrun_parse_resource_configuration (kconf->config_ਰੁੱਖ, container, "krun.ram_mib", "ram_mib", true);
  ਜੇ (ram_mib <= LIBKRUN_MINIMUM_RAM_MIB)
    {
      ਜੇ (def && def->linux && def->linux->resources && def->linux->resources->ਸਮ੍ਰਿਤੀ
          && def->linux->resources->ਸਮ੍ਰਿਤੀ->ਹੱਦਬੰਦੀ_present)
        ram_mib = def->linux->resources->ਸਮ੍ਰਿਤੀ->ਹੱਦਬੰਦੀ / (1024 * 1024);
      ਨਹੀਂਤਾਂ
        ram_mib = LIBKRUN_DEFAULT_RAM_MIB;
    }

  krun_ਸੈੱਟ_vm_config = dlsym (handle, "krun_set_vm_config");

  ਜੇ (krun_ਸੈੱਟ_vm_config == ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, 0, "could not find symbol in the krun library");

  ret = krun_ਸੈੱਟ_vm_config (ctx_id, cpus, ram_mib);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, -ret, "could not set krun vm configuration");

  nested_virt = libkrun_parse_resource_configuration (kconf->config_ਰੁੱਖ, container, "krun.nested_virt", "nested_virt", false);
  ਜੇ (nested_virt > 0)
    {
      int32_t (*krun_check_nested_virt) (ਖਾਲੀ);
      int32_t (*krun_ਸੈੱਟ_nested_virt) (uint32_t ctx_id, bool enabled);

      krun_check_nested_virt = dlsym (handle, "krun_check_nested_virt");
      ਜੇ (krun_check_nested_virt != ਨਿਰਾ && krun_check_nested_virt () != 1)
        libcrun_warning ("nested virtualization requested but may not be supported on this host");

      krun_ਸੈੱਟ_nested_virt = dlsym (handle, "krun_set_nested_virt");
      ਜੇ (krun_ਸੈੱਟ_nested_virt == ਨਿਰਾ)
        ਵਾਪਸ crun_make_error (err, 0, "could not find symbol `krun_set_nested_virt` in the krun library");

      ret = krun_ਸੈੱਟ_nested_virt (ctx_id, true);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, -ret, "could not enable nested virtualization");
    }

  ਜੇ (kconf->tap_name != ਨਿਰਾ)
    {
      krun_ਜੋੜੋ_net_tap = dlsym (handle, "krun_add_net_tap");
      ਜੇ (krun_ਜੋੜੋ_net_tap == ਨਿਰਾ)
        ਵਾਪਸ crun_make_error (err, 0, "could not find symbol `krun_add_net_tap` in the krun library");

      uint8_t mac[6];
      libkrun_make_tap_mac (container->context->id, mac);
      ret = krun_ਜੋੜੋ_net_tap (ctx_id, kconf->tap_name, &mac[0], COMPAT_NET_FEATURES, 0);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, -ret, "could not add krun TAP interface `%s`", kconf->tap_name);
    }
  ਨਹੀਂਤਾਂ ਜੇ (kconf->use_passt)
    {
      krun_ਜੋੜੋ_net_unixstream = dlsym (handle, "krun_add_net_unixstream");
      ਜੇ (krun_ਜੋੜੋ_net_unixstream == ਨਿਰਾ)
        ਵਾਪਸ crun_make_error (err, 0, "could not find symbol `krun_add_net_unixstream` in the krun library");

      uint8_t mac[] = { 0x5a, 0x94, 0xef, 0xe4, 0x0c, 0xee };
      ret = krun_ਜੋੜੋ_net_unixstream (ctx_id, ਨਿਰਾ, kconf->passt_fds[FD_PAIR_ਮਾਪਾ], &mac[0], COMPAT_NET_FEATURES, NET_FLAG_DHCP_CLIENT);
      ਜੇ (UNLIKELY (ret == -EINVAL))
        ret = krun_ਜੋੜੋ_net_unixstream (ctx_id, ਨਿਰਾ, kconf->passt_fds[FD_PAIR_ਮਾਪਾ], &mac[0], COMPAT_NET_FEATURES, 0);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, -ret, "could not set krun net configuration");
    }

  ਜੇ (kconf->gpu_flags > 0)
    {
      krun_ਸੈੱਟ_gpu_options = dlsym (kconf->handle, "krun_set_gpu_options");
      ਜੇ (krun_ਸੈੱਟ_gpu_options == ਨਿਰਾ)
        ਵਾਪਸ crun_make_error (err, 0, "gpu requested but the version of libkrun in this system does not support it");

      ret = krun_ਸੈੱਟ_gpu_options (kconf->ctx_id, kconf->gpu_flags);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, -ret, "gpu requested but could not configure virtio gpu device");

      ਜੇ (kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER)
        {
          krun_ਸੈੱਟ_gpu_render_server_fd = dlsym (kconf->handle, "krun_set_gpu_render_server_fd");
          ਜੇ (krun_ਸੈੱਟ_gpu_render_server_fd == ਨਿਰਾ)
            ਵਾਪਸ crun_make_error (err, 0, "gpu with render server requested but the version of libkrun in this system does not support it");

          ret = krun_ਸੈੱਟ_gpu_render_server_fd (kconf->ctx_id, kconf->gpu_render_server_fd);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, -ret, "gpu with render server requested but could not configure its file descriptor");
        }
    }

  ਜੇ (kconf->config_ਰੁੱਖ != ਨਿਰਾ)
    {
      /* Try to configure an external kernel. If the configuration file doesn't
       * specify a kernel, libkrun automatically fall back to using libkrunfw,
       * if the library is present and was loaded while creating the context.
       */
      ਪੂਰਨ custom_kernel = libkrun_parse_resource_configuration (kconf->config_ਰੁੱਖ, container, "krun.custom_kernel", "custom_kernel", false);
      ਜੇ (custom_kernel > 0)
        {
          ret = libkrun_configure_kernel (ctx_id, handle, kconf->config_ਰੁੱਖ, err);
          ਜੇ (UNLIKELY (ret))
            ਵਾਪਸ ret;
        }
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
libkrun_configure_flavor (ਖਾਲੀ *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  ਪੂਰਨ ret, sev_indicated = 0, awsnitro_indicated = 0;
  ਬਣਤਰ krun_config *kconf = (ਬਣਤਰ krun_config *) cookie;
  ਅਚਲ ਅੱਖਰ *flavor = ਨਿਰਾ;
  ਖਾਲੀ *close_handles[2];

  close_handles[0] = ਨਿਰਾ;
  close_handles[1] = ਨਿਰਾ;

  // Check if the user provided the krun variant through OCI annotations.
  flavor = ਲੱਭੋ_annotation (container, "krun.variant");
  ਜੇ (flavor != ਨਿਰਾ)
    {
      sev_indicated |= strcmp (flavor, KRUN_FLAVOR_SEV) == 0;
      awsnitro_indicated |= strcmp (flavor, KRUN_FLAVOR_AWS_NITRO) == 0;
    }

  ਜੇ (sev_indicated)
    {
      ਜੇ (kconf->handle_sev == ਨਿਰਾ)
        error (EXIT_FAILURE, 0, "the container requires libkrun-sev but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_awsnitro;

      kconf->handle = kconf->handle_sev;
      kconf->ctx_id = kconf->ctx_id_sev;
      kconf->sev = true;
    }
  ਨਹੀਂਤਾਂ ਜੇ (awsnitro_indicated)
    {
      ਜੇ (kconf->handle_awsnitro == ਨਿਰਾ)
        error (EXIT_FAILURE, 0, "the container requires libkrun-awsnitro but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_sev;

      kconf->handle = kconf->handle_awsnitro;
      kconf->ctx_id = kconf->ctx_id_awsnitro;
      kconf->awsnitro = true;
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (kconf->handle == ਨਿਰਾ)
        error (EXIT_FAILURE, 0, "the container requires libkrun but it's not available");

      close_handles[0] = kconf->handle_sev;
      close_handles[1] = kconf->handle_awsnitro;
    }

  // We no longer need the other two libkrun handles.
  ਲਈ (ਪੂਰਨ i = 0; i < 2; i++)
    {
      ਜੇ (close_handles[i] == ਨਿਰਾ)
        ਜਾਰੀ;

      ret = dlclose (close_handles[i]);
      ਜੇ (UNLIKELY (ret != 0))
        ਵਾਪਸ crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
libkrun_exec (ਖਾਲੀ *cookie, libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *pathname, ਅੱਖਰ *ਅਚਲ argv[])
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_ਸੈੱਟ_log_level) (uint32_t level);
  ਪੂਰਨ (*krun_ਸ਼ੁਰੂ_enter) (uint32_t ctx_id);
  int32_t (*krun_ਜੋੜੋ_virtiofs2) (uint32_t ctx_id, ਅਚਲ ਅੱਖਰ *c_ਟੈਗ, ਅਚਲ ਅੱਖਰ *c_ਰਸਤਾ, uint64_t shm_ਮਾਪ);
  int32_t (*krun_ਸੈੱਟ_ਜੜ੍ਹ_disk) (uint32_t ctx_id, ਅਚਲ ਅੱਖਰ *disk_ਰਸਤਾ);
  int32_t (*krun_ਸੈੱਟ_tee_config_file) (uint32_t ctx_id, ਅਚਲ ਅੱਖਰ *file_ਰਸਤਾ);
  int32_t (*krun_ਸੈੱਟ_console_output) (uint32_t ctx_id, ਅਚਲ ਅੱਖਰ *c_filepath);
  int32_t (*krun_ਸੈੱਟ_exec) (uint32_t ctx_id, ਅਚਲ ਅੱਖਰ *exec_ਰਸਤਾ,
                            ਅਚਲ ਅੱਖਰ *ਅਚਲ argv[], ਅਚਲ ਅੱਖਰ *ਅਚਲ envp[]);
  ਬਣਤਰ krun_config *kconf = (ਬਣਤਰ krun_config *) cookie;
  ਖਾਲੀ *handle;
  int32_t ctx_id, ret;
  libcrun_error_t err;

  ret = libkrun_configure_flavor (cookie, container, &err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਪੂਰਨ errcode = crun_error_get_errno (&err);
      crun_error_release (&err);
      error (EXIT_FAILURE, errcode, "unable to configure libkrun flavor");
    }

  // /dev/kvm is required for all non AWS nitro workloads.
  ਜੇ (! kconf->awsnitro && ! kconf->has_kvm)
    error (EXIT_FAILURE, 0, "`/dev/kvm` unavailable");

  handle = kconf->handle;
  ctx_id = kconf->ctx_id;

  krun_ਸੈੱਟ_log_level = dlsym (handle, "krun_set_log_level");
  krun_ਸ਼ੁਰੂ_enter = dlsym (handle, "krun_start_enter");
  ਜੇ (krun_ਸੈੱਟ_log_level == ਨਿਰਾ || krun_ਸ਼ੁਰੂ_enter == ਨਿਰਾ)
    error (EXIT_FAILURE, 0, "could not find symbol in the krun library");

  /* Set log level according to crun's verbosity. */
  ਚੋਣ (libcrun_get_verbosity ())
    {
    ਹਾਲਤ LIBCRUN_VERBOSITY_DEBUG:
      krun_ਸੈੱਟ_log_level (KRUN_LOG_LEVEL_DEBUG);
      ਰੋਕੋ;
    ਹਾਲਤ LIBCRUN_VERBOSITY_WARNING:
      krun_ਸੈੱਟ_log_level (KRUN_LOG_LEVEL_WARN);
      ਰੋਕੋ;
    ਮੂਲ:
      krun_ਸੈੱਟ_log_level (KRUN_LOG_LEVEL_ERROR);
      ਰੋਕੋ;
    }

  ਜੇ (kconf->sev)
    {
      krun_ਸੈੱਟ_ਜੜ੍ਹ_disk = dlsym (handle, "krun_set_root_disk");
      krun_ਸੈੱਟ_tee_config_file = dlsym (handle, "krun_set_tee_config_file");
      ਜੇ (krun_ਸੈੱਟ_ਜੜ੍ਹ_disk == ਨਿਰਾ || krun_ਸੈੱਟ_tee_config_file == ਨਿਰਾ)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-sev.so`");

      ret = krun_ਸੈੱਟ_ਜੜ੍ਹ_disk (ctx_id, "/disk.img");
      ਜੇ (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set root disk");

      ret = krun_ਸੈੱਟ_tee_config_file (ctx_id, KRUN_SEV_FILE);
      ਜੇ (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set krun tee config file");
    }
  ਨਹੀਂਤਾਂ
    {
      json_object *val_virtiofs_ਟੈਗ = ਨਿਰਾ;
      json_object *val_virtiofs_shm_ਮਾਪ = ਨਿਰਾ;
      ਅਚਲ ਅੱਖਰ *virtiofs_ਟੈਗ = ਨਿਰਾ;
      // Default to a conservative DAX size of 512MB, just like krun_set_root() does.
      uint64_t virtiofs_shm_ਮਾਪ = 512 * 1024 * 1024ULL;

      ਜੇ (kconf->config_ਰੁੱਖ != ਨਿਰਾ)
        {
          val_virtiofs_ਟੈਗ = json_object_object_get (kconf->config_ਰੁੱਖ, "virtiofs_tag");
          ਜੇ (val_virtiofs_ਟੈਗ != ਨਿਰਾ && json_object_is_type (val_virtiofs_ਟੈਗ, json_type_string))
            virtiofs_ਟੈਗ = json_object_get_string (val_virtiofs_ਟੈਗ);

          val_virtiofs_shm_ਮਾਪ = json_object_object_get (kconf->config_ਰੁੱਖ, "virtiofs_shm_size");
          ਜੇ (val_virtiofs_shm_ਮਾਪ != ਨਿਰਾ && json_object_is_type (val_virtiofs_shm_ਮਾਪ, json_type_int))
            virtiofs_shm_ਮਾਪ = json_object_get_uint64 (val_virtiofs_shm_ਮਾਪ);
        }

      ਜੇ (virtiofs_ਟੈਗ == ਨਿਰਾ)
        virtiofs_ਟੈਗ = "/dev/root";

      krun_ਜੋੜੋ_virtiofs2 = dlsym (handle, "krun_add_virtiofs2");

      ਜੇ (krun_ਜੋੜੋ_virtiofs2 == ਨਿਰਾ)
        error (EXIT_FAILURE, 0, "could not find symbol `krun_add_virtiofs2` in `libkrun.so`");

      ret = krun_ਜੋੜੋ_virtiofs2 (ctx_id, virtiofs_ਟੈਗ, "/", virtiofs_shm_ਮਾਪ);
      ਜੇ (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not add virtiofs root with tag `%s`", virtiofs_ਟੈਗ);
    }

  ਜੇ (kconf->awsnitro)
    {
      krun_ਸੈੱਟ_console_output = dlsym (handle, "krun_set_console_output");
      krun_ਸੈੱਟ_exec = dlsym (handle, "krun_set_exec");
      ਜੇ (krun_ਸੈੱਟ_console_output == ਨਿਰਾ || krun_ਸੈੱਟ_exec == ਨਿਰਾ)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-awsnitro.so`");

      // Redirect all enclave output (read from vsock) to stdout.
      ret = krun_ਸੈੱਟ_console_output (ctx_id, "/dev/stdout");
      ਜੇ (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not redirect enclave output to stdout");

      ret = krun_ਸੈੱਟ_exec (ctx_id, pathname, (ਅਚਲ ਅੱਖਰ *ਅਚਲ *) argv,
                           (ਅਚਲ ਅੱਖਰ *ਅਚਲ *) def->process->env);
      ਜੇ (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set enclave execution arguments");
    }

  ret = libkrun_configure_vm (ctx_id, handle, kconf, container, &err);
  ਜੇ (UNLIKELY (ret))
    {
      ਪੂਰਨ errcode = crun_error_get_errno (&err);
      libcrun_error_t *tmp_err = &err;
      libcrun_error_report_and_release (tmp_err);
      error (EXIT_FAILURE, errcode, "could not configure krun vm");
    }

  json_object_put (kconf->config_doc);

  ret = krun_ਸ਼ੁਰੂ_enter (ctx_id);
  ਜੇ (UNLIKELY (ret < 0))
    error (EXIT_FAILURE, -ret, "could not start krun");

  ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ
libkrun_ਸ਼ੁਰੂ_render_server (ਬਣਤਰ krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  kconf->gpu_flags = libkrun_parse_resource_configuration (kconf->config_ਰੁੱਖ, container, "krun.gpu_flags", "gpu_flags", false);
  ਜੇ (kconf->gpu_flags > 0)
    {
      ਜੇ (access ("/dev/dri", F_OK) != 0)
        ਵਾਪਸ crun_make_error (err, errno, "gpu requested but /dev/dri is not available");

      ਜੇ ((kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER) != 0)
        {
          ਅੱਖਰ *rs_argv[40];
          ਪੂਰਨ rs_argc = 0;
          ਅੱਖਰ fd_as_str[16];
          ਪੂਰਨ gpu_fds[2];
          pid_t pid;
          ਪੂਰਨ null;

          ਜੇ (access ("/usr/libexec/virgl_render_server", X_OK) != 0)
            ਵਾਪਸ crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/libexec/virgl_render_server binary");

          ਜੇ (access ("/usr/bin/bwrap", X_OK) != 0)
            ਵਾਪਸ crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/bin/bwrap binary");

          ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, gpu_fds);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "could not create socketpair for virgl_render_server");

          snprintf (fd_as_str, ਆਕਾਰ (fd_as_str), "%d", gpu_fds[FD_PAIR_ਬੱਚਾ]);

          rs_argv[rs_argc++] = (ਅੱਖਰ *) "bwrap";
          rs_argv[rs_argc++] = (ਅੱਖਰ *) "--ro-bind";
          rs_argv[rs_argc++] = (ਅੱਖਰ *) "/usr";
          rs_argv[rs_argc++] = (ਅੱਖਰ *) "/usr";
          ਜੇ (access ("/lib", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (ਅੱਖਰ *) "--ro-bind";
              rs_argv[rs_argc++] = (ਅੱਖਰ *) "/lib";
              rs_argv[rs_argc++] = (ਅੱਖਰ *) "/lib";
            }
          ਜੇ (access ("/lib64", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (ਅੱਖਰ *) "--ro-bind";
              rs_argv[rs_argc++] = (ਅੱਖਰ *) "/lib64";
              rs_argv[rs_argc++] = (ਅੱਖਰ *) "/lib64";
            }
          rs_argv[rs_argc++] = (ਅੱਖਰ *) "--ro-bind";
          rs_argv[rs_argc++] = (ਅੱਖਰ *) "/etc";
          rs_argv[rs_argc++] = (ਅੱਖਰ *) "/etc";
          rs_argv[rs_argc++] = (ਅੱਖਰ *) "--ro-bind";
          rs_argv[rs_argc++] = (ਅੱਖਰ *) "/sys";
          rs_argv[rs_argc++] = (ਅੱਖਰ *) "/sys";
          rs_argv[rs_argc++] = (ਅੱਖਰ *) "--dev";
          rs_argv[rs_argc++] = (ਅੱਖਰ *) "/dev";
          rs_argv[rs_argc++] = (ਅੱਖਰ *) "--dev-bind";
          rs_argv[rs_argc++] = (ਅੱਖਰ *) "/dev/dri";
          rs_argv[rs_argc++] = (ਅੱਖਰ *) "/dev/dri";
          rs_argv[rs_argc++] = (ਅੱਖਰ *) "--proc";
          rs_argv[rs_argc++] = (ਅੱਖਰ *) "/proc";
          rs_argv[rs_argc++] = (ਅੱਖਰ *) "--tmpfs";
          rs_argv[rs_argc++] = (ਅੱਖਰ *) "/tmp";
          rs_argv[rs_argc++] = (ਅੱਖਰ *) "--unshare-pid";
          rs_argv[rs_argc++] = (ਅੱਖਰ *) "--unshare-ipc";
          rs_argv[rs_argc++] = (ਅੱਖਰ *) "--unshare-net";
          rs_argv[rs_argc++] = (ਅੱਖਰ *) "--";
          rs_argv[rs_argc++] = (ਅੱਖਰ *) "/usr/libexec/virgl_render_server";
          rs_argv[rs_argc++] = (ਅੱਖਰ *) "--socket-fd";
          rs_argv[rs_argc++] = fd_as_str;
          rs_argv[rs_argc] = ਨਿਰਾ;

          pid = fork ();
          ਜੇ (pid < 0)
            {
              close (gpu_fds[FD_PAIR_ਮਾਪਾ]);
              close (gpu_fds[FD_PAIR_ਬੱਚਾ]);
              ਵਾਪਸ crun_make_error (err, errno, "could not fork for virgl_render_server");
            }
          ਨਹੀਂਤਾਂ ਜੇ (pid == 0)
            {
              close (gpu_fds[FD_PAIR_ਮਾਪਾ]);

              null = open ("/dev/null", O_WRONLY);
              ਜੇ (null == -1)
                _exit (EXIT_FAILURE);

              dup2 (null, STDOUT_FILENO);
              dup2 (null, STDERR_FILENO);
              close (null);

              execvp ("/usr/bin/bwrap", rs_argv);
              _exit (EXIT_FAILURE);
            }

          close (gpu_fds[FD_PAIR_ਬੱਚਾ]);
          kconf->gpu_render_server_fd = gpu_fds[FD_PAIR_ਮਾਪਾ];
        }
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
libkrun_configure_ਜਾਲ (ਖਾਲੀ *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  ਬਣਤਰ krun_config *kconf = (ਬਣਤਰ krun_config *) cookie;
  pid_t pid;
  ਅੱਖਰ *passt_argv[10];
  ਅੱਖਰ fd_as_str[16];
  ਪੂਰਨ use_passt;
  ਪੂਰਨ argv_idx;
  ਪੂਰਨ ਸਥਿਤੀ;
  ਪੂਰਨ null;
  ਪੂਰਨ ret;

  ret = libkrun_parse_string_configuration (kconf->config_ਰੁੱਖ, container,
                                            "krun.tap_name", "tap_name",
                                            &kconf->tap_name, err, false);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (kconf->tap_name != ਨਿਰਾ && is_ਖਾਲੀ_ਥਾਵਾਂ_string (kconf->tap_name))
    ਵਾਪਸ crun_make_error (err, 0, "krun.tap_name cannot be empty");

  use_passt = libkrun_parse_resource_configuration (kconf->config_ਰੁੱਖ, container, "krun.use_passt", "use_passt", false);

  ਜੇ (use_passt > 0)
    {
      ਜੇ (kconf->tap_name != ਨਿਰਾ)
        ਵਾਪਸ crun_make_error (err, 0, "krun.tap_name and krun.use_passt are mutually exclusive");

      kconf->use_passt = 1;
    }
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ 0;

  ret = socketpair (AF_UNIX, SOCK_ਧਾਰਾ, 0, kconf->passt_fds);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "create passt socketpair");

  snprintf (fd_as_str, ਆਕਾਰ (fd_as_str), "%d", kconf->passt_fds[FD_PAIR_ਬੱਚਾ]);

  argv_idx = 0;
  passt_argv[argv_idx++] = (ਅੱਖਰ *) "passt";
  passt_argv[argv_idx++] = (ਅੱਖਰ *) "-t";
  passt_argv[argv_idx++] = (ਅੱਖਰ *) "all";

  ਜੇ (! kconf->has_awsnitro)
    {
      passt_argv[argv_idx++] = (ਅੱਖਰ *) "-u";
      passt_argv[argv_idx++] = (ਅੱਖਰ *) "all";
      passt_argv[argv_idx++] = (ਅੱਖਰ *) "--no-dhcp-dns";
    }

  /* Set --no-map-gw. If the gateway for the network is also the DNS server
   * we won't be able to query the DNS server without --no-map-gw.
   * See discussion in https://github.com/containers/crun/pull/2099
   */
  passt_argv[argv_idx++] = (ਅੱਖਰ *) "--no-map-gw";

  passt_argv[argv_idx++] = (ਅੱਖਰ *) "--fd";
  passt_argv[argv_idx++] = fd_as_str;
  passt_argv[argv_idx] = ਨਿਰਾ;

  pid = fork ();
  ਜੇ (pid < 0)
    ਵਾਪਸ crun_make_error (err, errno, "fork passt");
  ਨਹੀਂਤਾਂ ਜੇ (pid == 0)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t i;

      close (kconf->passt_fds[FD_PAIR_ਮਾਪਾ]);

      null = open ("/dev/null", O_WRONLY);
      ਜੇ (null == -1)
        _exit (EXIT_FAILURE);

      /* Remount /proc if the parent detached its PID namespace, as passt(1)
       * needs a consistent /proc view to operate properly.
       */
      ਲਈ (i = 0; i < def->linux->namespaces_len; i++)
        {
          ਪੂਰਨ ਮੁੱਲ = libcrun_ਲੱਭੋ_namespace (def->linux->namespaces[i]->type);

          ਜੇ (ਮੁੱਲ == CLONE_NEWPID)
            {
              ਜੇ (mount ("", "/proc", "proc", MS_NOSUID | MS_NODEV | MS_NOEXEC, ਨਿਰਾ))
                _exit (EXIT_FAILURE);

              ਰੋਕੋ;
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

  close (kconf->passt_fds[FD_PAIR_ਬੱਚਾ]);

  // Wait for passt to daemonize itself.
  waitpid (pid, &ਸਥਿਤੀ, 0);
  ਜੇ (! (WIFEXITED (ਸਥਿਤੀ)) || WEXITSTATUS (ਸਥਿਤੀ) != 0)
    ਵਾਪਸ crun_make_error (err, 0, "start passt");

  ਵਾਪਸ 0;
}

/* Return true if the spec already declares a device with the given PATH.  */
ਸਥਿਰ bool
spec_has_device (runtime_spec_schema_config_schema *def, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
  size_t i;

  ਲਈ (i = 0; i < def->linux->devices_len; i++)
    ਜੇ (strcmp (def->linux->devices[i]->ਰਸਤਾ, ਰਸਤਾ) == 0)
      ਵਾਪਸ true;
  ਵਾਪਸ false;
}

/* libkrun_create_kvm_device: explicitly adds kvm device.  */
ਸਥਿਰ ਪੂਰਨ
libkrun_configure_container (ਖਾਲੀ *cookie, ਗਿਣਤੀ handler_configure_phase phase,
                             libcrun_context_t *context, libcrun_container_t *container,
                             ਅਚਲ ਅੱਖਰ *rootfs, libcrun_error_t *err)
{
  ਪੂਰਨ ret, rootfsfd;
  ਬਣਤਰ krun_config *kconf = (ਬਣਤਰ krun_config *) cookie;
  ਬਣਤਰ device_s kvm_device = { "/dev/kvm", "c", 10, 232, 0666, 0, 0 };
  ਬਣਤਰ device_s sev_device = { "/dev/sev", "c", 10, 124, 0666, 0, 0 };
  ਬਣਤਰ device_s nitro_device = { "/dev/nitro_enclaves", "c", 10, 122, 0666, 0, 0 };
  cleanup_close ਪੂਰਨ devfd = -1;
  cleanup_close ਪੂਰਨ rootfsfd_cleanup = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  bool create_sev = false, create_awsnitro = false;
  bool is_user_ns;

  ਜੇ (rootfs == ਨਿਰਾ)
    rootfsfd = AT_FDCWD;
  ਨਹੀਂਤਾਂ
    {
      rootfsfd = rootfsfd_cleanup = open (rootfs, O_ਰਸਤਾ | O_CLOEXEC);
      ਜੇ (UNLIKELY (rootfsfd < 0))
        ਵਾਪਸ crun_make_error (err, errno, "open `%s`", rootfs);
    }

  ਜੇ (phase == HANDLER_CONFIGURE_BEFORE_USERNS)
    {
      cleanup_free ਅੱਖਰ *origin_config_ਰਸਤਾ = ਨਿਰਾ;
      cleanup_free ਅੱਖਰ *state_dir = ਨਿਰਾ;
      cleanup_free ਅੱਖਰ *config = ਨਿਰਾ;
      cleanup_close ਪੂਰਨ fd = -1;
      size_t config_ਮਾਪ;

      ret = libcrun_get_state_directory (&state_dir, context->state_ਜੜ੍ਹ, context->id, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = append_paths (&origin_config_ਰਸਤਾ, err, state_dir, "config.json", ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = read_all_file (origin_config_ਰਸਤਾ, &config, &config_ਮਾਪ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      /* CVE-2025-24965: the content below rootfs cannot be trusted because it is controlled by the user.  We
         must ensure the file is opened below the rootfs directory.  Whatever the image ships at that name
         could also be a device node or a FIFO, so never open it: unlink it and create a new regular file.  */
      ret = unlinkat (rootfsfd, KRUN_CONFIG_FILE, 0);
      ਜੇ (UNLIKELY (ret < 0 && errno != ENOENT))
        ਵਾਪਸ crun_make_error (err, errno, "unlink `%s`", KRUN_CONFIG_FILE);

      fd = ਸੁਰੱਖਿਅਤ_openat (rootfsfd, rootfs, KRUN_CONFIG_FILE, WRITE_FILE_DEFAULT_FLAGS | O_EXCL | O_NOFOLLOW, S_IRUSR | S_IRGRP | S_IROTH, err);
      ਜੇ (UNLIKELY (fd < 0))
        ਵਾਪਸ fd;

      ret = ਸੁਰੱਖਿਅਤ_write (fd, KRUN_CONFIG_FILE, config, config_ਮਾਪ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = libkrun_read_vm_config (kconf, rootfsfd, rootfs, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = libkrun_ਸ਼ੁਰੂ_render_server (kconf, container, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    ਵਾਪਸ 0;

  ret = libkrun_configure_ਜਾਲ (cookie, container, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  /* Do nothing if /dev/kvm is already present in spec */
  ਜੇ (spec_has_device (def, "/dev/kvm"))
    ਵਾਪਸ 0;

  ਜੇ (kconf->handle_sev != ਨਿਰਾ)
    create_sev = ! spec_has_device (def, "/dev/sev");

  ਜੇ (kconf->handle_awsnitro != ਨਿਰਾ)
    create_awsnitro = ! spec_has_device (def, "/dev/nitro_enclaves");

  devfd = ਸੁਰੱਖਿਅਤ_openat (rootfsfd, rootfs, "dev", O_ਰਸਤਾ | O_DIRECTORY | O_CLOEXEC, 0, err);
  ਜੇ (UNLIKELY (devfd < 0))
    ਵਾਪਸ devfd;

  ret = check_running_ਵਿੱਚ_user_namespace (err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;
  is_user_ns = ret;

  ਜੇ (kconf->has_kvm)
    {
      ret = libcrun_create_dev (container, devfd, -1, &kvm_device, is_user_ns, true, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (create_sev)
    {
      ret = libcrun_create_dev (container, devfd, -1, &sev_device, is_user_ns, true, err);
      ਜੇ (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_sev);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
          kconf->handle_sev = ਨਿਰਾ;
        }
    }

  ਜੇ (create_awsnitro)
    {
      ret = libcrun_create_dev (container, devfd, -1, &nitro_device, is_user_ns, true, err);
      ਜੇ (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_awsnitro);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
          kconf->handle_awsnitro = ਨਿਰਾ;
        }
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
libkrun_ਲੱਦੋ (ਖਾਲੀ **cookie, libcrun_error_t *err)
{
  int32_t ret;
  ਬਣਤਰ krun_config *kconf;
  ਅਚਲ ਅੱਖਰ *libkrun_so = "libkrun.so.1";
  ਅਚਲ ਅੱਖਰ *libkrun_sev_so = "libkrun-sev.so.1";
  ਅਚਲ ਅੱਖਰ *libkrun_awsnitro_so = "libkrun-awsnitro.so.1";

  kconf = ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਆਕਾਰ (ਬਣਤਰ krun_config));
  ਜੇ (kconf == ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, 0, "could not allocate memory for krun_config");
  memset (kconf, 0, ਆਕਾਰ (ਬਣਤਰ krun_config));

  kconf->gpu_render_server_fd = -1;

  kconf->handle = dlopen (libkrun_so, RTLD_NOW);
  kconf->handle_sev = dlopen (libkrun_sev_so, RTLD_NOW);
  kconf->handle_awsnitro = dlopen (libkrun_awsnitro_so, RTLD_NOW);

  ਜੇ (kconf->handle == ਨਿਰਾ && kconf->handle_sev == ਨਿਰਾ && kconf->handle_awsnitro == ਨਿਰਾ)
    {
      ਮੁਕਤ (kconf);
      ਵਾਪਸ crun_make_error (err, 0, "failed to open `%s`, `%s`, and `%s` for krun_config: %s", libkrun_so, libkrun_sev_so, libkrun_awsnitro_so, dlerror ());
    }

  kconf->sev = false;
  kconf->awsnitro = false;

  /* Newer versions of libkrun no longer link against libkrunfw and
     instead they open it when creating the context. This implies
     we need to call "krun_create_ctx" before switching namespaces
     or it won't be able to find the library bundling the kernel. */
  ਜੇ (kconf->handle)
    {
      ret = libkrun_create_context (kconf->handle, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਜਾਓ error;
      kconf->ctx_id = ret;
    }

  ਜੇ (kconf->handle_sev)
    {
      ret = libkrun_create_context (kconf->handle_sev, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਜਾਓ error;
      kconf->ctx_id_sev = ret;
    }
  ਜੇ (kconf->handle_awsnitro)
    {
      ret = libkrun_create_context (kconf->handle_awsnitro, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਜਾਓ error;
      kconf->ctx_id_awsnitro = ret;
    }

  *cookie = kconf;

  ਵਾਪਸ 0;

error:
  ਜੇ (kconf->handle)
    dlclose (kconf->handle);
  ਜੇ (kconf->handle_sev)
    dlclose (kconf->handle_sev);
  ਜੇ (kconf->handle_awsnitro)
    dlclose (kconf->handle_awsnitro);
  ਮੁਕਤ (kconf);
  ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ
libkrun_unload (ਖਾਲੀ *cookie, libcrun_error_t *err)
{
  ਪੂਰਨ r;

  ਬਣਤਰ krun_config *kconf = (ਬਣਤਰ krun_config *) cookie;
  ਜੇ (kconf != ਨਿਰਾ)
    {
      ਜੇ (kconf->handle != ਨਿਰਾ)
        {
          r = dlclose (kconf->handle);
          ਜੇ (UNLIKELY (r != 0))
            ਵਾਪਸ crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
        }
      ਜੇ (kconf->handle_sev != ਨਿਰਾ)
        {
          r = dlclose (kconf->handle_sev);
          ਜੇ (UNLIKELY (r != 0))
            ਵਾਪਸ crun_make_error (err, 0, "could not unload handle_sev: `%s`", dlerror ());
        }
      ਜੇ (kconf->handle_awsnitro != ਨਿਰਾ)
        {
          r = dlclose (kconf->handle_awsnitro);
          ਜੇ (UNLIKELY (r != 0))
            ਵਾਪਸ crun_make_error (err, 0, "could not unload handle_awsnitro: `%s`", dlerror ());
        }
      ਮੁਕਤ (kconf);
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ runtime_spec_schema_defs_linux_device_cgroup *
make_oci_spec_dev (ਅਚਲ ਅੱਖਰ *type, dev_t device, bool allow, ਅਚਲ ਅੱਖਰ *access)
{
  runtime_spec_schema_defs_linux_device_cgroup *dev = xmalloc0 (ਆਕਾਰ (*dev));

  dev->allow = allow;
  dev->allow_present = 1;

  dev->type = xstrdup (type);

  dev->major = major (device);
  dev->major_present = 1;

  dev->minor = minor (device);
  dev->minor_present = 1;

  dev->access = xstrdup (access);

  ਵਾਪਸ dev;
}

/* stat an optional device: on ENOENT clear *PRESENT, any other error fails.  */
ਸਥਿਰ ਪੂਰਨ
stat_optional_device (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਬਣਤਰ stat *st, bool *present, libcrun_error_t *err)
{
  ਪੂਰਨ ret = stat (ਰਸਤਾ, st);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਜੇ (errno != ENOENT)
        ਵਾਪਸ crun_make_error (err, errno, "stat `%s`", ਰਸਤਾ);
      *present = false;
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
libkrun_modify_oci_configuration (ਖਾਲੀ *cookie arg_ਅਣਵਰਤਿਆ, libcrun_context_t *context arg_ਅਣਵਰਤਿਆ,
                                  runtime_spec_schema_config_schema *def,
                                  libcrun_error_t *err)
{
  ਅਚਲ size_t device_ਮਾਪ = ਆਕਾਰ (runtime_spec_schema_defs_linux_device_cgroup);
  ਬਣਤਰ krun_config *kconf = (ਬਣਤਰ krun_config *) cookie;
  ਬਣਤਰ stat st_kvm, st_sev, st_awsnitro;
  bool has_kvm = true, has_sev = true, has_awsnitro = true;
  size_t old_len, new_len;
  ਪੂਰਨ ret;

  /* Always allow the /dev/kvm device.  */

  ret = stat_optional_device ("/dev/kvm", &st_kvm, &has_kvm, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = stat_optional_device ("/dev/sev", &st_sev, &has_sev, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = stat_optional_device ("/dev/nitro_enclaves", &st_awsnitro, &has_awsnitro, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  kconf->has_kvm = has_kvm;
  kconf->has_awsnitro = has_awsnitro;

  ਜੇ (! has_kvm && ! has_awsnitro)
    ਵਾਪਸ 0;

  /* spec says these are optional, ensure they exist so we can add our devices */
  ਜੇ (def->linux == ਨਿਰਾ)
    def->linux = xmalloc0 (ਆਕਾਰ (runtime_spec_schema_config_linux));

  ਜੇ (def->linux->resources == ਨਿਰਾ)
    def->linux->resources = xmalloc0 (ਆਕਾਰ (runtime_spec_schema_config_linux_resources));

  old_len = def->linux->resources->devices_len;
  new_len = old_len;
  ਜੇ (has_kvm)
    new_len += has_sev ? 2 : 1;
  ਜੇ (has_awsnitro)
    new_len += 1;

  def->linux->resources->devices = xrealloc (def->linux->resources->devices, device_ਮਾਪ * (new_len + 1));
  def->linux->resources->devices_len = new_len;

  ਜੇ (has_kvm)
    {
      def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_kvm.st_rdev, true, "rwm");
      ਜੇ (has_sev)
        def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_sev.st_rdev, true, "rwm");
    }

  ਜੇ (has_awsnitro)
    def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_awsnitro.st_rdev, true, "rwm");

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
libkrun_close_fds (ਖਾਲੀ *cookie, libcrun_container_t *container, ਪੂਰਨ preserve_fds, libcrun_error_t *err)
{
  ਬਣਤਰ krun_config *kconf = (ਬਣਤਰ krun_config *) cookie;
  ਪੂਰਨ ਪਹਿਲਾ_fd_ਮੰਜ਼ਿਲ_close = preserve_fds + 3;
  ਪੂਰਨ ਨੀਵਾਂ_fd, ਉੱਚਾ_fd;
  ਪੂਰਨ i;

  ਨੀਵਾਂ_fd = ਉੱਚਾ_fd = -1;

  ਜੇ (kconf->use_passt)
    ਉੱਚਾ_fd = kconf->passt_fds[FD_PAIR_ਮਾਪਾ];

  ਜੇ (kconf->gpu_render_server_fd != -1)
    {
      ਜੇ (kconf->gpu_render_server_fd > ਉੱਚਾ_fd)
        {
          ਨੀਵਾਂ_fd = ਉੱਚਾ_fd;
          ਉੱਚਾ_fd = kconf->gpu_render_server_fd;
        }
      ਨਹੀਂਤਾਂ
        ਨੀਵਾਂ_fd = kconf->gpu_render_server_fd;
    }

  ਜੇ (ਪਹਿਲਾ_fd_ਮੰਜ਼ਿਲ_close <= ਉੱਚਾ_fd)
    {
      ਲਈ (i = ਪਹਿਲਾ_fd_ਮੰਜ਼ਿਲ_close; i < ਉੱਚਾ_fd; i++)
        {
          // If we're closing proc_fd, make sure to invalidate it.
          ਜੇ (i == container->proc_fd)
            container->proc_fd = -1;
          ਨਹੀਂਤਾਂ ਜੇ (i == ਨੀਵਾਂ_fd)
            ਜਾਰੀ;
          close (i);
        }

      ਪਹਿਲਾ_fd_ਮੰਜ਼ਿਲ_close = ਉੱਚਾ_fd + 1;
    }

  ਵਾਪਸ mark_or_close_fds_ge_than (container, ਪਹਿਲਾ_fd_ਮੰਜ਼ਿਲ_close, true, err);
}

ਬਣਤਰ custom_handler_s handler_libkrun = {
  .name = "krun",
  .alias = ਨਿਰਾ,
  .feature_string = "LIBKRUN",
  .supports_open_ਰੁੱਖ_namespace = false,
  .ਲੱਦੋ = libkrun_ਲੱਦੋ,
  .unload = libkrun_unload,
  .ਲੜੀਵਾਰ_func = libkrun_exec,
  .configure_container = libkrun_configure_container,
  .modify_oci_configuration = libkrun_modify_oci_configuration,
  .close_fds = libkrun_close_fds,
};

#ਜੇ_ਅੰਤ
