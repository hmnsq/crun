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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "../custom-handler.h"
#અન્તર્ભાવ "../container.h"
#અન્તર્ભાવ "../utils.h"
#અન્તર્ભાવ "../linux.h"
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <sys/socket.h>
#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <sys/mount.h>
#અન્તર્ભાવ <sys/param.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <sys/sysmacros.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <sched.h>
#અન્તર્ભાવ <ocispec/runtime_spec_schema_config_schema.h>

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

સંરચના krun_config
{
  શૂન્ય *handle;
  શૂન્ય *handle_sev;
  શૂન્ય *handle_awsnitro;
  bool sev;
  bool awsnitro;
  int32_t ctx_id;
  int32_t ctx_id_sev;
  int32_t ctx_id_awsnitro;
  bool has_kvm;
  bool has_awsnitro;
  પૂર્ણાંક passt_fds[2];
  json_object *config_doc;
  json_object *config_વૃક્ષ;
  bool use_passt;
  અચલ અક્ષર *tap_name;
  પૂર્ણાંક gpu_render_server_fd;
  પૂર્ણાંક gpu_flags;
};

/* libkrun handler.  */
#જો HAVE_DLOPEN && HAVE_LIBKRUN
સ્થિર int32_t
libkrun_create_context (શૂન્ય *handle, libcrun_error_t *err)
{
  int32_t (*krun_create_ctx) ();
  int32_t ctx_id;

  krun_create_ctx = dlsym (handle, "krun_create_ctx");
  જો (krun_create_ctx == ખાલી)
    પાછા crun_make_error (err, 0, "could not find symbol in the krun library");

  ctx_id = krun_create_ctx ();
  જો (UNLIKELY (ctx_id < 0))
    પાછા crun_make_error (err, -ctx_id, "could not create krun context");

  પાછા ctx_id;
}

સ્થિર પૂર્ણાંક
libkrun_configure_kernel (uint32_t ctx_id, શૂન્ય *handle, json_object *config_વૃક્ષ, libcrun_error_t *err)
{
  int32_t (*krun_set_kernel) (uint32_t ctx_id, અચલ અક્ષર *kernel_path,
                              uint32_t kernel_format, અચલ અક્ષર *initrd_path, અચલ અક્ષર *kernel_cmdline);
  json_object *kernel_path = ખાલી;
  json_object *kernel_format = ખાલી;
  json_object *val_initrd_path = ખાલી;
  json_object *val_kernel_cmdline = ખાલી;
  અચલ અક્ષર *initrd_path = ખાલી;
  અચલ અક્ષર *kernel_cmdline = ખાલી;
  પૂર્ણાંક ret;

  /* kernel_path and kernel_format must be present */
  kernel_path = json_object_object_get (config_વૃક્ષ, "kernel_path");
  જો (kernel_path == ખાલી || ! json_object_is_type (kernel_path, json_type_string))
    પાછા 0;

  kernel_format = json_object_object_get (config_વૃક્ષ, "kernel_format");
  જો (kernel_format == ખાલી || ! json_object_is_type (kernel_format, json_type_int))
    પાછા 0;

  /* initrd and kernel_cmdline are optional */
  val_initrd_path = json_object_object_get (config_વૃક્ષ, "initrd_path");
  જો (val_initrd_path != ખાલી && json_object_is_type (val_initrd_path, json_type_string))
    initrd_path = json_object_get_string (val_initrd_path);

  val_kernel_cmdline = json_object_object_get (config_વૃક્ષ, "kernel_cmdline");
  જો (val_kernel_cmdline != ખાલી && json_object_is_type (val_kernel_cmdline, json_type_string))
    kernel_cmdline = json_object_get_string (val_kernel_cmdline);

  krun_set_kernel = dlsym (handle, "krun_set_kernel");
  જો (krun_set_kernel == ખાલી)
    પાછા crun_make_error (err, 0, "could not find symbol in krun library");

  ret = krun_set_kernel (ctx_id,
                         json_object_get_string (kernel_path),
                         (uint32_t) json_object_get_int64 (kernel_format),
                         initrd_path, kernel_cmdline);

  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, -ret, "could not configure a krun external kernel");

  પાછા 0;
}

સ્થિર પૂર્ણાંક
libkrun_read_vm_config (સંરચના krun_config *kconf, પૂર્ણાંક rootfsfd, અચલ અક્ષર *rootfs, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  cleanup_free અક્ષર *config = ખાલી;
  cleanup_close પૂર્ણાંક pathfd = -1;
  cleanup_close પૂર્ણાંક fd = -1;
  proc_fd_path_t fd_path;
  સંરચના stat st;

  /* The content of the rootfs is controlled by the user: look at the file through an
     O_PATH descriptor first and refuse anything that is not a small regular file, as
     opening a FIFO blocks and a device node must not be accessed.  */
  pathfd = safe_openat (rootfsfd, rootfs, KRUN_VM_FILE, O_PATH | O_CLOEXEC | O_NOFOLLOW, 0, err);
  જો (pathfd < 0)
    {
      // The configuration file is optional, don't generate an error if it's missing.
      જો (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          પાછા 0;
        }
      પાછા pathfd;
    }

  ret = fstat (pathfd, &st);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "stat `%s`", KRUN_VM_FILE);

  જો (! S_ISREG (st.st_mode))
    પાછા crun_make_error (err, 0, "`%s` is not a regular file", KRUN_VM_FILE);

  get_proc_self_fd_path (fd_path, pathfd);

  fd = open (fd_path, O_RDONLY | O_CLOEXEC);
  જો (UNLIKELY (fd < 0))
    પાછા crun_make_error (err, errno, "open `%s`", KRUN_VM_FILE);

  ret = read_all_fd_with_max_size (fd, KRUN_VM_FILE, &config, ખાલી, KRUN_VM_FILE_MAX_SIZE, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = parse_json_file (&kconf->config_doc, config, ખાલી, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  kconf->config_વૃક્ષ = kconf->config_doc;
  પાછા 0;
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
સ્થિર પૂર્ણાંક
libkrun_parse_resource_configuration (json_object *config_વૃક્ષ, libcrun_container_t *container, અચલ અક્ષર *annotation, અચલ અક્ષર *ચાવી, bool use_config_file)
{
  અક્ષર *val_str, *endptr;
  પૂર્ણાંક val = -1;
  json_object *val_json = ખાલી;

  val_str = (અક્ષર *) find_annotation (container, annotation);
  જો (val_str != ખાલી)
    {
      errno = 0;
      val = (પૂર્ણાંક) strtol (val_str, &endptr, 10);
      જો (errno != 0 || endptr == val_str || *endptr != '\0')
        /* Annotations value is not a valid integer. */
        error (EXIT_FAILURE, 0, "krun annotation %s value cannot be converted to an integer", annotation);

      જો (val < 0)
        error (EXIT_FAILURE, 0, "krun annotation %s value must be a positive integer", annotation);

      પાછા val;
    }
  નહીંતર જો (use_config_file && config_વૃક્ષ != ખાલી)
    {
      val_json = json_object_object_get (config_વૃક્ષ, ચાવી);
      જો (val_json == ખાલી)
        પાછા val;
      જો (! json_object_is_type (val_json, json_type_int))
        error (EXIT_FAILURE, 0, "krun krun_vm.json %s value is not an integer", ચાવી);

      val = (પૂર્ણાંક) json_object_get_int64 (val_json);
    }

  પાછા val;
}

સ્થિર પૂર્ણાંક
libkrun_parse_string_configuration (json_object *config_વૃક્ષ, libcrun_container_t *container,
                                    અચલ અક્ષર *annotation, અચલ અક્ષર *ચાવી,
                                    અચલ અક્ષર **કિંમત, libcrun_error_t *err, bool use_config_file)
{
  અચલ અક્ષર *val;
  json_object *val_json = ખાલી;

  *કિંમત = ખાલી;

  val = find_annotation (container, annotation);
  જો (val != ખાલી)
    {
      *કિંમત = val;
      પાછા 0;
    }

  જો (! use_config_file || config_વૃક્ષ == ખાલી)
    પાછા 0;

  val_json = json_object_object_get (config_વૃક્ષ, ચાવી);
  જો (val_json == ખાલી)
    પાછા 0;

  જો (! json_object_is_type (val_json, json_type_string))
    પાછા crun_make_error (err, 0, ".krun_vm.json %s value is not a string", ચાવી);

  *કિંમત = json_object_get_string (val_json);
  પાછા 0;
}

સ્થિર શૂન્ય
libkrun_make_tap_mac (અચલ અક્ષર *id, uint8_t mac[6])
{
  uint64_t hash = 14695981039346656037ULL;
  size_t i;

  માટે (; *id != '\0'; id++)
    {
      hash ^= (uint8_t) *id;
      hash *= 1099511628211ULL;
    }

  માટે (i = 0; i < 6; i++)
    mac[i] = (uint8_t) (hash >> (i * 8));

  mac[0] = (mac[0] & 0xfe) | 0x02;
}

સ્થિર પૂર્ણાંક
libkrun_configure_vm (uint32_t ctx_id, શૂન્ય *handle, સંરચના krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_set_vm_config) (uint32_t ctx_id, uint8_t અંક_vcpus, uint32_t ram_mib);
  int32_t (*krun_add_net_unixstream) (uint32_t ctx_id, અચલ અક્ષર *c_path, પૂર્ણાંક fd, uint8_t *અચલ c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_add_net_tap) (uint32_t ctx_id, અચલ અક્ષર *c_tap_name, અચલ uint8_t *c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_set_gpu_options) (uint32_t ctx_id, uint32_t virgl_flags);
  int32_t (*krun_set_gpu_render_server_fd) (uint32_t ctx_id, પૂર્ણાંક fd);
  પૂર્ણાંક cpus, ram_mib, nested_virt, ret;
  cpu_set_t set;

  /* We let the OCI set the number of vCPUs for the VM, since cgroups CPU restrictions still apply. */
  cpus = libkrun_parse_resource_configuration (kconf->config_વૃક્ષ, container, "krun.cpus", "cpus", true);
  જો (cpus <= 0)
    {
      CPU_ZERO (&set);
      જો (sched_getaffinity (getpid (), કદ (set), &set) == 0)
        cpus = MIN (CPU_ગણતરી (&set), LIBKRUN_MAX_VCPUS);
      નહીંતર
        cpus = 1;
    }

  /* We let the OCI set the amount of RAM for the VM, since cgroups memory restrictions still apply. */
  ram_mib = libkrun_parse_resource_configuration (kconf->config_વૃક્ષ, container, "krun.ram_mib", "ram_mib", true);
  જો (ram_mib <= LIBKRUN_MINIMUM_RAM_MIB)
    {
      જો (def && def->linux && def->linux->resources && def->linux->resources->memory
          && def->linux->resources->memory->limit_present)
        ram_mib = def->linux->resources->memory->limit / (1024 * 1024);
      નહીંતર
        ram_mib = LIBKRUN_DEFAULT_RAM_MIB;
    }

  krun_set_vm_config = dlsym (handle, "krun_set_vm_config");

  જો (krun_set_vm_config == ખાલી)
    પાછા crun_make_error (err, 0, "could not find symbol in the krun library");

  ret = krun_set_vm_config (ctx_id, cpus, ram_mib);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, -ret, "could not set krun vm configuration");

  nested_virt = libkrun_parse_resource_configuration (kconf->config_વૃક્ષ, container, "krun.nested_virt", "nested_virt", false);
  જો (nested_virt > 0)
    {
      int32_t (*krun_check_nested_virt) (શૂન્ય);
      int32_t (*krun_set_nested_virt) (uint32_t ctx_id, bool enabled);

      krun_check_nested_virt = dlsym (handle, "krun_check_nested_virt");
      જો (krun_check_nested_virt != ખાલી && krun_check_nested_virt () != 1)
        libcrun_warning ("nested virtualization requested but may not be supported on this host");

      krun_set_nested_virt = dlsym (handle, "krun_set_nested_virt");
      જો (krun_set_nested_virt == ખાલી)
        પાછા crun_make_error (err, 0, "could not find symbol `krun_set_nested_virt` in the krun library");

      ret = krun_set_nested_virt (ctx_id, true);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, -ret, "could not enable nested virtualization");
    }

  જો (kconf->tap_name != ખાલી)
    {
      krun_add_net_tap = dlsym (handle, "krun_add_net_tap");
      જો (krun_add_net_tap == ખાલી)
        પાછા crun_make_error (err, 0, "could not find symbol `krun_add_net_tap` in the krun library");

      uint8_t mac[6];
      libkrun_make_tap_mac (container->context->id, mac);
      ret = krun_add_net_tap (ctx_id, kconf->tap_name, &mac[0], COMPAT_NET_FEATURES, 0);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, -ret, "could not add krun TAP interface `%s`", kconf->tap_name);
    }
  નહીંતર જો (kconf->use_passt)
    {
      krun_add_net_unixstream = dlsym (handle, "krun_add_net_unixstream");
      જો (krun_add_net_unixstream == ખાલી)
        પાછા crun_make_error (err, 0, "could not find symbol `krun_add_net_unixstream` in the krun library");

      uint8_t mac[] = { 0x5a, 0x94, 0xef, 0xe4, 0x0c, 0xee };
      ret = krun_add_net_unixstream (ctx_id, ખાલી, kconf->passt_fds[FD_PAIR_PARENT], &mac[0], COMPAT_NET_FEATURES, NET_FLAG_DHCP_CLIENT);
      જો (UNLIKELY (ret == -EINVAL))
        ret = krun_add_net_unixstream (ctx_id, ખાલી, kconf->passt_fds[FD_PAIR_PARENT], &mac[0], COMPAT_NET_FEATURES, 0);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, -ret, "could not set krun net configuration");
    }

  જો (kconf->gpu_flags > 0)
    {
      krun_set_gpu_options = dlsym (kconf->handle, "krun_set_gpu_options");
      જો (krun_set_gpu_options == ખાલી)
        પાછા crun_make_error (err, 0, "gpu requested but the version of libkrun in this system does not support it");

      ret = krun_set_gpu_options (kconf->ctx_id, kconf->gpu_flags);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, -ret, "gpu requested but could not configure virtio gpu device");

      જો (kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER)
        {
          krun_set_gpu_render_server_fd = dlsym (kconf->handle, "krun_set_gpu_render_server_fd");
          જો (krun_set_gpu_render_server_fd == ખાલી)
            પાછા crun_make_error (err, 0, "gpu with render server requested but the version of libkrun in this system does not support it");

          ret = krun_set_gpu_render_server_fd (kconf->ctx_id, kconf->gpu_render_server_fd);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, -ret, "gpu with render server requested but could not configure its file descriptor");
        }
    }

  જો (kconf->config_વૃક્ષ != ખાલી)
    {
      /* Try to configure an external kernel. If the configuration file doesn't
       * specify a kernel, libkrun automatically fall back to using libkrunfw,
       * if the library is present and was loaded while creating the context.
       */
      પૂર્ણાંક custom_kernel = libkrun_parse_resource_configuration (kconf->config_વૃક્ષ, container, "krun.custom_kernel", "custom_kernel", false);
      જો (custom_kernel > 0)
        {
          ret = libkrun_configure_kernel (ctx_id, handle, kconf->config_વૃક્ષ, err);
          જો (UNLIKELY (ret))
            પાછા ret;
        }
    }

  પાછા 0;
}

સ્થિર પૂર્ણાંક
libkrun_configure_flavor (શૂન્ય *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  પૂર્ણાંક ret, sev_indicated = 0, awsnitro_indicated = 0;
  સંરચના krun_config *kconf = (સંરચના krun_config *) cookie;
  અચલ અક્ષર *flavor = ખાલી;
  શૂન્ય *close_handles[2];

  close_handles[0] = ખાલી;
  close_handles[1] = ખાલી;

  // Check if the user provided the krun variant through OCI annotations.
  flavor = find_annotation (container, "krun.variant");
  જો (flavor != ખાલી)
    {
      sev_indicated |= strcmp (flavor, KRUN_FLAVOR_SEV) == 0;
      awsnitro_indicated |= strcmp (flavor, KRUN_FLAVOR_AWS_NITRO) == 0;
    }

  જો (sev_indicated)
    {
      જો (kconf->handle_sev == ખાલી)
        error (EXIT_FAILURE, 0, "the container requires libkrun-sev but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_awsnitro;

      kconf->handle = kconf->handle_sev;
      kconf->ctx_id = kconf->ctx_id_sev;
      kconf->sev = true;
    }
  નહીંતર જો (awsnitro_indicated)
    {
      જો (kconf->handle_awsnitro == ખાલી)
        error (EXIT_FAILURE, 0, "the container requires libkrun-awsnitro but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_sev;

      kconf->handle = kconf->handle_awsnitro;
      kconf->ctx_id = kconf->ctx_id_awsnitro;
      kconf->awsnitro = true;
    }
  નહીંતર
    {
      જો (kconf->handle == ખાલી)
        error (EXIT_FAILURE, 0, "the container requires libkrun but it's not available");

      close_handles[0] = kconf->handle_sev;
      close_handles[1] = kconf->handle_awsnitro;
    }

  // We no longer need the other two libkrun handles.
  માટે (પૂર્ણાંક i = 0; i < 2; i++)
    {
      જો (close_handles[i] == ખાલી)
        ચાલુ;

      ret = dlclose (close_handles[i]);
      જો (UNLIKELY (ret != 0))
        પાછા crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }

  પાછા 0;
}

સ્થિર પૂર્ણાંક
libkrun_exec (શૂન્ય *cookie, libcrun_container_t *container, અચલ અક્ષર *pathname, અક્ષર *અચલ argv[])
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_set_log_level) (uint32_t level);
  પૂર્ણાંક (*krun_start_enter) (uint32_t ctx_id);
  int32_t (*krun_add_virtiofs2) (uint32_t ctx_id, અચલ અક્ષર *c_tag, અચલ અક્ષર *c_path, uint64_t shm_size);
  int32_t (*krun_set_મૂળ_disk) (uint32_t ctx_id, અચલ અક્ષર *disk_path);
  int32_t (*krun_set_tee_config_file) (uint32_t ctx_id, અચલ અક્ષર *file_path);
  int32_t (*krun_set_console_output) (uint32_t ctx_id, અચલ અક્ષર *c_filepath);
  int32_t (*krun_set_exec) (uint32_t ctx_id, અચલ અક્ષર *exec_path,
                            અચલ અક્ષર *અચલ argv[], અચલ અક્ષર *અચલ envp[]);
  સંરચના krun_config *kconf = (સંરચના krun_config *) cookie;
  શૂન્ય *handle;
  int32_t ctx_id, ret;
  libcrun_error_t err;

  ret = libkrun_configure_flavor (cookie, container, &err);
  જો (UNLIKELY (ret < 0))
    {
      પૂર્ણાંક errcode = crun_error_get_errno (&err);
      crun_error_release (&err);
      error (EXIT_FAILURE, errcode, "unable to configure libkrun flavor");
    }

  // /dev/kvm is required for all non AWS nitro workloads.
  જો (! kconf->awsnitro && ! kconf->has_kvm)
    error (EXIT_FAILURE, 0, "`/dev/kvm` unavailable");

  handle = kconf->handle;
  ctx_id = kconf->ctx_id;

  krun_set_log_level = dlsym (handle, "krun_set_log_level");
  krun_start_enter = dlsym (handle, "krun_start_enter");
  જો (krun_set_log_level == ખાલી || krun_start_enter == ખાલી)
    error (EXIT_FAILURE, 0, "could not find symbol in the krun library");

  /* Set log level according to crun's verbosity. */
  પસંદગી (libcrun_get_verbosity ())
    {
    સ્થિતિ LIBCRUN_VERBOSITY_DEBUG:
      krun_set_log_level (KRUN_LOG_LEVEL_DEBUG);
      તોડો;
    સ્થિતિ LIBCRUN_VERBOSITY_WARNING:
      krun_set_log_level (KRUN_LOG_LEVEL_WARN);
      તોડો;
    મૂળભૂત:
      krun_set_log_level (KRUN_LOG_LEVEL_ERROR);
      તોડો;
    }

  જો (kconf->sev)
    {
      krun_set_મૂળ_disk = dlsym (handle, "krun_set_root_disk");
      krun_set_tee_config_file = dlsym (handle, "krun_set_tee_config_file");
      જો (krun_set_મૂળ_disk == ખાલી || krun_set_tee_config_file == ખાલી)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-sev.so`");

      ret = krun_set_મૂળ_disk (ctx_id, "/disk.img");
      જો (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set root disk");

      ret = krun_set_tee_config_file (ctx_id, KRUN_SEV_FILE);
      જો (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set krun tee config file");
    }
  નહીંતર
    {
      json_object *val_virtiofs_tag = ખાલી;
      json_object *val_virtiofs_shm_size = ખાલી;
      અચલ અક્ષર *virtiofs_tag = ખાલી;
      // Default to a conservative DAX size of 512MB, just like krun_set_root() does.
      uint64_t virtiofs_shm_size = 512 * 1024 * 1024ULL;

      જો (kconf->config_વૃક્ષ != ખાલી)
        {
          val_virtiofs_tag = json_object_object_get (kconf->config_વૃક્ષ, "virtiofs_tag");
          જો (val_virtiofs_tag != ખાલી && json_object_is_type (val_virtiofs_tag, json_type_string))
            virtiofs_tag = json_object_get_string (val_virtiofs_tag);

          val_virtiofs_shm_size = json_object_object_get (kconf->config_વૃક્ષ, "virtiofs_shm_size");
          જો (val_virtiofs_shm_size != ખાલી && json_object_is_type (val_virtiofs_shm_size, json_type_int))
            virtiofs_shm_size = json_object_get_uint64 (val_virtiofs_shm_size);
        }

      જો (virtiofs_tag == ખાલી)
        virtiofs_tag = "/dev/root";

      krun_add_virtiofs2 = dlsym (handle, "krun_add_virtiofs2");

      જો (krun_add_virtiofs2 == ખાલી)
        error (EXIT_FAILURE, 0, "could not find symbol `krun_add_virtiofs2` in `libkrun.so`");

      ret = krun_add_virtiofs2 (ctx_id, virtiofs_tag, "/", virtiofs_shm_size);
      જો (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not add virtiofs root with tag `%s`", virtiofs_tag);
    }

  જો (kconf->awsnitro)
    {
      krun_set_console_output = dlsym (handle, "krun_set_console_output");
      krun_set_exec = dlsym (handle, "krun_set_exec");
      જો (krun_set_console_output == ખાલી || krun_set_exec == ખાલી)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-awsnitro.so`");

      // Redirect all enclave output (read from vsock) to stdout.
      ret = krun_set_console_output (ctx_id, "/dev/stdout");
      જો (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not redirect enclave output to stdout");

      ret = krun_set_exec (ctx_id, pathname, (અચલ અક્ષર *અચલ *) argv,
                           (અચલ અક્ષર *અચલ *) def->process->env);
      જો (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set enclave execution arguments");
    }

  ret = libkrun_configure_vm (ctx_id, handle, kconf, container, &err);
  જો (UNLIKELY (ret))
    {
      પૂર્ણાંક errcode = crun_error_get_errno (&err);
      libcrun_error_t *tmp_err = &err;
      libcrun_error_report_and_release (tmp_err);
      error (EXIT_FAILURE, errcode, "could not configure krun vm");
    }

  json_object_put (kconf->config_doc);

  ret = krun_start_enter (ctx_id);
  જો (UNLIKELY (ret < 0))
    error (EXIT_FAILURE, -ret, "could not start krun");

  પાછા ret;
}

સ્થિર પૂર્ણાંક
libkrun_start_render_server (સંરચના krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  kconf->gpu_flags = libkrun_parse_resource_configuration (kconf->config_વૃક્ષ, container, "krun.gpu_flags", "gpu_flags", false);
  જો (kconf->gpu_flags > 0)
    {
      જો (access ("/dev/dri", F_OK) != 0)
        પાછા crun_make_error (err, errno, "gpu requested but /dev/dri is not available");

      જો ((kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER) != 0)
        {
          અક્ષર *rs_argv[40];
          પૂર્ણાંક rs_argc = 0;
          અક્ષર fd_as_str[16];
          પૂર્ણાંક gpu_fds[2];
          pid_t pid;
          પૂર્ણાંક null;

          જો (access ("/usr/libexec/virgl_render_server", X_OK) != 0)
            પાછા crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/libexec/virgl_render_server binary");

          જો (access ("/usr/bin/bwrap", X_OK) != 0)
            પાછા crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/bin/bwrap binary");

          ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, gpu_fds);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "could not create socketpair for virgl_render_server");

          snprintf (fd_as_str, કદ (fd_as_str), "%d", gpu_fds[FD_PAIR_CHILD]);

          rs_argv[rs_argc++] = (અક્ષર *) "bwrap";
          rs_argv[rs_argc++] = (અક્ષર *) "--ro-bind";
          rs_argv[rs_argc++] = (અક્ષર *) "/usr";
          rs_argv[rs_argc++] = (અક્ષર *) "/usr";
          જો (access ("/lib", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (અક્ષર *) "--ro-bind";
              rs_argv[rs_argc++] = (અક્ષર *) "/lib";
              rs_argv[rs_argc++] = (અક્ષર *) "/lib";
            }
          જો (access ("/lib64", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (અક્ષર *) "--ro-bind";
              rs_argv[rs_argc++] = (અક્ષર *) "/lib64";
              rs_argv[rs_argc++] = (અક્ષર *) "/lib64";
            }
          rs_argv[rs_argc++] = (અક્ષર *) "--ro-bind";
          rs_argv[rs_argc++] = (અક્ષર *) "/etc";
          rs_argv[rs_argc++] = (અક્ષર *) "/etc";
          rs_argv[rs_argc++] = (અક્ષર *) "--ro-bind";
          rs_argv[rs_argc++] = (અક્ષર *) "/sys";
          rs_argv[rs_argc++] = (અક્ષર *) "/sys";
          rs_argv[rs_argc++] = (અક્ષર *) "--dev";
          rs_argv[rs_argc++] = (અક્ષર *) "/dev";
          rs_argv[rs_argc++] = (અક્ષર *) "--dev-bind";
          rs_argv[rs_argc++] = (અક્ષર *) "/dev/dri";
          rs_argv[rs_argc++] = (અક્ષર *) "/dev/dri";
          rs_argv[rs_argc++] = (અક્ષર *) "--proc";
          rs_argv[rs_argc++] = (અક્ષર *) "/proc";
          rs_argv[rs_argc++] = (અક્ષર *) "--tmpfs";
          rs_argv[rs_argc++] = (અક્ષર *) "/tmp";
          rs_argv[rs_argc++] = (અક્ષર *) "--unshare-pid";
          rs_argv[rs_argc++] = (અક્ષર *) "--unshare-ipc";
          rs_argv[rs_argc++] = (અક્ષર *) "--unshare-net";
          rs_argv[rs_argc++] = (અક્ષર *) "--";
          rs_argv[rs_argc++] = (અક્ષર *) "/usr/libexec/virgl_render_server";
          rs_argv[rs_argc++] = (અક્ષર *) "--socket-fd";
          rs_argv[rs_argc++] = fd_as_str;
          rs_argv[rs_argc] = ખાલી;

          pid = fork ();
          જો (pid < 0)
            {
              close (gpu_fds[FD_PAIR_PARENT]);
              close (gpu_fds[FD_PAIR_CHILD]);
              પાછા crun_make_error (err, errno, "could not fork for virgl_render_server");
            }
          નહીંતર જો (pid == 0)
            {
              close (gpu_fds[FD_PAIR_PARENT]);

              null = open ("/dev/null", O_WRONLY);
              જો (null == -1)
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

  પાછા 0;
}

સ્થિર પૂર્ણાંક
libkrun_configure_network (શૂન્ય *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  સંરચના krun_config *kconf = (સંરચના krun_config *) cookie;
  pid_t pid;
  અક્ષર *passt_argv[10];
  અક્ષર fd_as_str[16];
  પૂર્ણાંક use_passt;
  પૂર્ણાંક argv_idx;
  પૂર્ણાંક status;
  પૂર્ણાંક null;
  પૂર્ણાંક ret;

  ret = libkrun_parse_string_configuration (kconf->config_વૃક્ષ, container,
                                            "krun.tap_name", "tap_name",
                                            &kconf->tap_name, err, false);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (kconf->tap_name != ખાલી && is_empty_string (kconf->tap_name))
    પાછા crun_make_error (err, 0, "krun.tap_name cannot be empty");

  use_passt = libkrun_parse_resource_configuration (kconf->config_વૃક્ષ, container, "krun.use_passt", "use_passt", false);

  જો (use_passt > 0)
    {
      જો (kconf->tap_name != ખાલી)
        પાછા crun_make_error (err, 0, "krun.tap_name and krun.use_passt are mutually exclusive");

      kconf->use_passt = 1;
    }
  નહીંતર
    પાછા 0;

  ret = socketpair (AF_UNIX, SOCK_STREAM, 0, kconf->passt_fds);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "create passt socketpair");

  snprintf (fd_as_str, કદ (fd_as_str), "%d", kconf->passt_fds[FD_PAIR_CHILD]);

  argv_idx = 0;
  passt_argv[argv_idx++] = (અક્ષર *) "passt";
  passt_argv[argv_idx++] = (અક્ષર *) "-t";
  passt_argv[argv_idx++] = (અક્ષર *) "all";

  જો (! kconf->has_awsnitro)
    {
      passt_argv[argv_idx++] = (અક્ષર *) "-u";
      passt_argv[argv_idx++] = (અક્ષર *) "all";
      passt_argv[argv_idx++] = (અક્ષર *) "--no-dhcp-dns";
    }

  /* Set --no-map-gw. If the gateway for the network is also the DNS server
   * we won't be able to query the DNS server without --no-map-gw.
   * See discussion in https://github.com/containers/crun/pull/2099
   */
  passt_argv[argv_idx++] = (અક્ષર *) "--no-map-gw";

  passt_argv[argv_idx++] = (અક્ષર *) "--fd";
  passt_argv[argv_idx++] = fd_as_str;
  passt_argv[argv_idx] = ખાલી;

  pid = fork ();
  જો (pid < 0)
    પાછા crun_make_error (err, errno, "fork passt");
  નહીંતર જો (pid == 0)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t i;

      close (kconf->passt_fds[FD_PAIR_PARENT]);

      null = open ("/dev/null", O_WRONLY);
      જો (null == -1)
        _exit (EXIT_FAILURE);

      /* Remount /proc if the parent detached its PID namespace, as passt(1)
       * needs a consistent /proc view to operate properly.
       */
      માટે (i = 0; i < def->linux->namespaces_len; i++)
        {
          પૂર્ણાંક કિંમત = libcrun_find_namespace (def->linux->namespaces[i]->type);

          જો (કિંમત == CLONE_NEWPID)
            {
              જો (mount ("", "/proc", "proc", MS_NOSUID | MS_NODEV | MS_NOEXEC, ખાલી))
                _exit (EXIT_FAILURE);

              તોડો;
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
  જો (! (WIFEXITED (status)) || WEXITSTATUS (status) != 0)
    પાછા crun_make_error (err, 0, "start passt");

  પાછા 0;
}

/* Return true if the spec already declares a device with the given PATH.  */
સ્થિર bool
spec_has_device (runtime_spec_schema_config_schema *def, અચલ અક્ષર *path)
{
  size_t i;

  માટે (i = 0; i < def->linux->devices_len; i++)
    જો (strcmp (def->linux->devices[i]->path, path) == 0)
      પાછા true;
  પાછા false;
}

/* libkrun_create_kvm_device: explicitly adds kvm device.  */
સ્થિર પૂર્ણાંક
libkrun_configure_container (શૂન્ય *cookie, ગણના handler_configure_phase phase,
                             libcrun_context_t *context, libcrun_container_t *container,
                             અચલ અક્ષર *rootfs, libcrun_error_t *err)
{
  પૂર્ણાંક ret, rootfsfd;
  સંરચના krun_config *kconf = (સંરચના krun_config *) cookie;
  સંરચના device_s kvm_device = { "/dev/kvm", "c", 10, 232, 0666, 0, 0 };
  સંરચના device_s sev_device = { "/dev/sev", "c", 10, 124, 0666, 0, 0 };
  સંરચના device_s nitro_device = { "/dev/nitro_enclaves", "c", 10, 122, 0666, 0, 0 };
  cleanup_close પૂર્ણાંક devfd = -1;
  cleanup_close પૂર્ણાંક rootfsfd_cleanup = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  bool create_sev = false, create_awsnitro = false;
  bool is_user_ns;

  જો (rootfs == ખાલી)
    rootfsfd = AT_FDCWD;
  નહીંતર
    {
      rootfsfd = rootfsfd_cleanup = open (rootfs, O_PATH | O_CLOEXEC);
      જો (UNLIKELY (rootfsfd < 0))
        પાછા crun_make_error (err, errno, "open `%s`", rootfs);
    }

  જો (phase == HANDLER_CONFIGURE_BEFORE_USERNS)
    {
      cleanup_free અક્ષર *origin_config_path = ખાલી;
      cleanup_free અક્ષર *state_dir = ખાલી;
      cleanup_free અક્ષર *config = ખાલી;
      cleanup_close પૂર્ણાંક fd = -1;
      size_t config_size;

      ret = libcrun_get_state_directory (&state_dir, context->state_મૂળ, context->id, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = append_paths (&origin_config_path, err, state_dir, "config.json", ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = read_all_file (origin_config_path, &config, &config_size, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      /* CVE-2025-24965: the content below rootfs cannot be trusted because it is controlled by the user.  We
         must ensure the file is opened below the rootfs directory.  Whatever the image ships at that name
         could also be a device node or a FIFO, so never open it: unlink it and create a new regular file.  */
      ret = unlinkat (rootfsfd, KRUN_CONFIG_FILE, 0);
      જો (UNLIKELY (ret < 0 && errno != ENOENT))
        પાછા crun_make_error (err, errno, "unlink `%s`", KRUN_CONFIG_FILE);

      fd = safe_openat (rootfsfd, rootfs, KRUN_CONFIG_FILE, WRITE_FILE_DEFAULT_FLAGS | O_EXCL | O_NOFOLLOW, S_IRUSR | S_IRGRP | S_IROTH, err);
      જો (UNLIKELY (fd < 0))
        પાછા fd;

      ret = safe_write (fd, KRUN_CONFIG_FILE, config, config_size, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = libkrun_read_vm_config (kconf, rootfsfd, rootfs, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = libkrun_start_render_server (kconf, container, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    પાછા 0;

  ret = libkrun_configure_network (cookie, container, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  /* Do nothing if /dev/kvm is already present in spec */
  જો (spec_has_device (def, "/dev/kvm"))
    પાછા 0;

  જો (kconf->handle_sev != ખાલી)
    create_sev = ! spec_has_device (def, "/dev/sev");

  જો (kconf->handle_awsnitro != ખાલી)
    create_awsnitro = ! spec_has_device (def, "/dev/nitro_enclaves");

  devfd = safe_openat (rootfsfd, rootfs, "dev", O_PATH | O_DIRECTORY | O_CLOEXEC, 0, err);
  જો (UNLIKELY (devfd < 0))
    પાછા devfd;

  ret = check_running_in_user_namespace (err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;
  is_user_ns = ret;

  જો (kconf->has_kvm)
    {
      ret = libcrun_create_dev (container, devfd, -1, &kvm_device, is_user_ns, true, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (create_sev)
    {
      ret = libcrun_create_dev (container, devfd, -1, &sev_device, is_user_ns, true, err);
      જો (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_sev);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
          kconf->handle_sev = ખાલી;
        }
    }

  જો (create_awsnitro)
    {
      ret = libcrun_create_dev (container, devfd, -1, &nitro_device, is_user_ns, true, err);
      જો (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_awsnitro);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
          kconf->handle_awsnitro = ખાલી;
        }
    }

  પાછા 0;
}

સ્થિર પૂર્ણાંક
libkrun_load (શૂન્ય **cookie, libcrun_error_t *err)
{
  int32_t ret;
  સંરચના krun_config *kconf;
  અચલ અક્ષર *libkrun_so = "libkrun.so.1";
  અચલ અક્ષર *libkrun_sev_so = "libkrun-sev.so.1";
  અચલ અક્ષર *libkrun_awsnitro_so = "libkrun-awsnitro.so.1";

  kconf = malloc (કદ (સંરચના krun_config));
  જો (kconf == ખાલી)
    પાછા crun_make_error (err, 0, "could not allocate memory for krun_config");
  memset (kconf, 0, કદ (સંરચના krun_config));

  kconf->gpu_render_server_fd = -1;

  kconf->handle = dlopen (libkrun_so, RTLD_NOW);
  kconf->handle_sev = dlopen (libkrun_sev_so, RTLD_NOW);
  kconf->handle_awsnitro = dlopen (libkrun_awsnitro_so, RTLD_NOW);

  જો (kconf->handle == ખાલી && kconf->handle_sev == ખાલી && kconf->handle_awsnitro == ખાલી)
    {
      મુક્ત (kconf);
      પાછા crun_make_error (err, 0, "failed to open `%s`, `%s`, and `%s` for krun_config: %s", libkrun_so, libkrun_sev_so, libkrun_awsnitro_so, dlerror ());
    }

  kconf->sev = false;
  kconf->awsnitro = false;

  /* Newer versions of libkrun no longer link against libkrunfw and
     instead they open it when creating the context. This implies
     we need to call "krun_create_ctx" before switching namespaces
     or it won't be able to find the library bundling the kernel. */
  જો (kconf->handle)
    {
      ret = libkrun_create_context (kconf->handle, err);
      જો (UNLIKELY (ret < 0))
        જાઓ error;
      kconf->ctx_id = ret;
    }

  જો (kconf->handle_sev)
    {
      ret = libkrun_create_context (kconf->handle_sev, err);
      જો (UNLIKELY (ret < 0))
        જાઓ error;
      kconf->ctx_id_sev = ret;
    }
  જો (kconf->handle_awsnitro)
    {
      ret = libkrun_create_context (kconf->handle_awsnitro, err);
      જો (UNLIKELY (ret < 0))
        જાઓ error;
      kconf->ctx_id_awsnitro = ret;
    }

  *cookie = kconf;

  પાછા 0;

error:
  જો (kconf->handle)
    dlclose (kconf->handle);
  જો (kconf->handle_sev)
    dlclose (kconf->handle_sev);
  જો (kconf->handle_awsnitro)
    dlclose (kconf->handle_awsnitro);
  મુક્ત (kconf);
  પાછા ret;
}

સ્થિર પૂર્ણાંક
libkrun_unload (શૂન્ય *cookie, libcrun_error_t *err)
{
  પૂર્ણાંક r;

  સંરચના krun_config *kconf = (સંરચના krun_config *) cookie;
  જો (kconf != ખાલી)
    {
      જો (kconf->handle != ખાલી)
        {
          r = dlclose (kconf->handle);
          જો (UNLIKELY (r != 0))
            પાછા crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
        }
      જો (kconf->handle_sev != ખાલી)
        {
          r = dlclose (kconf->handle_sev);
          જો (UNLIKELY (r != 0))
            પાછા crun_make_error (err, 0, "could not unload handle_sev: `%s`", dlerror ());
        }
      જો (kconf->handle_awsnitro != ખાલી)
        {
          r = dlclose (kconf->handle_awsnitro);
          જો (UNLIKELY (r != 0))
            પાછા crun_make_error (err, 0, "could not unload handle_awsnitro: `%s`", dlerror ());
        }
      મુક્ત (kconf);
    }
  પાછા 0;
}

સ્થિર runtime_spec_schema_defs_linux_device_cgroup *
make_oci_spec_dev (અચલ અક્ષર *type, dev_t device, bool allow, અચલ અક્ષર *access)
{
  runtime_spec_schema_defs_linux_device_cgroup *dev = xmalloc0 (કદ (*dev));

  dev->allow = allow;
  dev->allow_present = 1;

  dev->type = xstrdup (type);

  dev->major = major (device);
  dev->major_present = 1;

  dev->minor = minor (device);
  dev->minor_present = 1;

  dev->access = xstrdup (access);

  પાછા dev;
}

/* stat an optional device: on ENOENT clear *PRESENT, any other error fails.  */
સ્થિર પૂર્ણાંક
stat_optional_device (અચલ અક્ષર *path, સંરચના stat *st, bool *present, libcrun_error_t *err)
{
  પૂર્ણાંક ret = stat (path, st);
  જો (UNLIKELY (ret < 0))
    {
      જો (errno != ENOENT)
        પાછા crun_make_error (err, errno, "stat `%s`", path);
      *present = false;
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
libkrun_modify_oci_configuration (શૂન્ય *cookie arg_unused, libcrun_context_t *context arg_unused,
                                  runtime_spec_schema_config_schema *def,
                                  libcrun_error_t *err)
{
  અચલ size_t device_size = કદ (runtime_spec_schema_defs_linux_device_cgroup);
  સંરચના krun_config *kconf = (સંરચના krun_config *) cookie;
  સંરચના stat st_kvm, st_sev, st_awsnitro;
  bool has_kvm = true, has_sev = true, has_awsnitro = true;
  size_t old_len, new_len;
  પૂર્ણાંક ret;

  /* Always allow the /dev/kvm device.  */

  ret = stat_optional_device ("/dev/kvm", &st_kvm, &has_kvm, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = stat_optional_device ("/dev/sev", &st_sev, &has_sev, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = stat_optional_device ("/dev/nitro_enclaves", &st_awsnitro, &has_awsnitro, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  kconf->has_kvm = has_kvm;
  kconf->has_awsnitro = has_awsnitro;

  જો (! has_kvm && ! has_awsnitro)
    પાછા 0;

  /* spec says these are optional, ensure they exist so we can add our devices */
  જો (def->linux == ખાલી)
    def->linux = xmalloc0 (કદ (runtime_spec_schema_config_linux));

  જો (def->linux->resources == ખાલી)
    def->linux->resources = xmalloc0 (કદ (runtime_spec_schema_config_linux_resources));

  old_len = def->linux->resources->devices_len;
  new_len = old_len;
  જો (has_kvm)
    new_len += has_sev ? 2 : 1;
  જો (has_awsnitro)
    new_len += 1;

  def->linux->resources->devices = xrealloc (def->linux->resources->devices, device_size * (new_len + 1));
  def->linux->resources->devices_len = new_len;

  જો (has_kvm)
    {
      def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_kvm.st_rdev, true, "rwm");
      જો (has_sev)
        def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_sev.st_rdev, true, "rwm");
    }

  જો (has_awsnitro)
    def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_awsnitro.st_rdev, true, "rwm");

  પાછા 0;
}

સ્થિર પૂર્ણાંક
libkrun_close_fds (શૂન્ય *cookie, libcrun_container_t *container, પૂર્ણાંક preserve_fds, libcrun_error_t *err)
{
  સંરચના krun_config *kconf = (સંરચના krun_config *) cookie;
  પૂર્ણાંક first_fd_to_close = preserve_fds + 3;
  પૂર્ણાંક નીચલું_fd, ઉપલું_fd;
  પૂર્ણાંક i;

  નીચલું_fd = ઉપલું_fd = -1;

  જો (kconf->use_passt)
    ઉપલું_fd = kconf->passt_fds[FD_PAIR_PARENT];

  જો (kconf->gpu_render_server_fd != -1)
    {
      જો (kconf->gpu_render_server_fd > ઉપલું_fd)
        {
          નીચલું_fd = ઉપલું_fd;
          ઉપલું_fd = kconf->gpu_render_server_fd;
        }
      નહીંતર
        નીચલું_fd = kconf->gpu_render_server_fd;
    }

  જો (first_fd_to_close <= ઉપલું_fd)
    {
      માટે (i = first_fd_to_close; i < ઉપલું_fd; i++)
        {
          // If we're closing proc_fd, make sure to invalidate it.
          જો (i == container->proc_fd)
            container->proc_fd = -1;
          નહીંતર જો (i == નીચલું_fd)
            ચાલુ;
          close (i);
        }

      first_fd_to_close = ઉપલું_fd + 1;
    }

  પાછા mark_or_close_fds_ge_than (container, first_fd_to_close, true, err);
}

સંરચના custom_handler_s handler_libkrun = {
  .name = "krun",
  .alias = ખાલી,
  .feature_string = "LIBKRUN",
  .supports_open_વૃક્ષ_namespace = false,
  .load = libkrun_load,
  .unload = libkrun_unload,
  .run_func = libkrun_exec,
  .configure_container = libkrun_configure_container,
  .modify_oci_configuration = libkrun_modify_oci_configuration,
  .close_fds = libkrun_close_fds,
};

#endif
