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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "../custom-handler.h"
#অন্তর্ভাব "../container.h"
#অন্তর্ভাব "../utils.h"
#অন্তর্ভাব "../linux.h"
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <sys/socket.h>
#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <sys/mount.h>
#অন্তর্ভাব <sys/param.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <sys/sysmacros.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <sched.h>
#অন্তর্ভাব <ocispec/runtime_spec_schema_config_schema.h>

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
#define KRUN_VM_FILE_MAX_আকার (64 * 1024)

#define KRUN_FLAVOR_AWS_NITRO "aws-nitro"
#define KRUN_FLAVOR_SEV "sev"

#define FD_PAIR_PARENT 0
#define FD_PAIR_CHILD 1

কাঠামো krun_config
{
  শূন্য *handle;
  শূন্য *handle_sev;
  শূন্য *handle_awsnitro;
  bool sev;
  bool awsnitro;
  int32_t ctx_id;
  int32_t ctx_id_sev;
  int32_t ctx_id_awsnitro;
  bool has_kvm;
  bool has_awsnitro;
  পূর্ণসংখ্যা passt_fds[2];
  json_object *config_doc;
  json_object *config_বৃক্ষ;
  bool use_passt;
  ধ্রুবক অক্ষর *tap_name;
  পূর্ণসংখ্যা gpu_render_server_fd;
  পূর্ণসংখ্যা gpu_flags;
};

/* libkrun handler.  */
#যদি HAVE_DLOPEN && HAVE_LIBKRUN
স্থির int32_t
libkrun_create_context (শূন্য *handle, libcrun_error_t *err)
{
  int32_t (*krun_create_ctx) ();
  int32_t ctx_id;

  krun_create_ctx = dlsym (handle, "krun_create_ctx");
  যদি (krun_create_ctx == ফাঁকা)
    ফেরত crun_make_error (err, 0, "could not find symbol in the krun library");

  ctx_id = krun_create_ctx ();
  যদি (UNLIKELY (ctx_id < 0))
    ফেরত crun_make_error (err, -ctx_id, "could not create krun context");

  ফেরত ctx_id;
}

স্থির পূর্ণসংখ্যা
libkrun_configure_kernel (uint32_t ctx_id, শূন্য *handle, json_object *config_বৃক্ষ, libcrun_error_t *err)
{
  int32_t (*krun_set_kernel) (uint32_t ctx_id, ধ্রুবক অক্ষর *kernel_path,
                              uint32_t kernel_format, ধ্রুবক অক্ষর *initrd_path, ধ্রুবক অক্ষর *kernel_cmdline);
  json_object *kernel_path = ফাঁকা;
  json_object *kernel_format = ফাঁকা;
  json_object *val_initrd_path = ফাঁকা;
  json_object *val_kernel_cmdline = ফাঁকা;
  ধ্রুবক অক্ষর *initrd_path = ফাঁকা;
  ধ্রুবক অক্ষর *kernel_cmdline = ফাঁকা;
  পূর্ণসংখ্যা ret;

  /* kernel_path and kernel_format must be present */
  kernel_path = json_object_object_get (config_বৃক্ষ, "kernel_path");
  যদি (kernel_path == ফাঁকা || ! json_object_is_type (kernel_path, json_type_string))
    ফেরত 0;

  kernel_format = json_object_object_get (config_বৃক্ষ, "kernel_format");
  যদি (kernel_format == ফাঁকা || ! json_object_is_type (kernel_format, json_type_int))
    ফেরত 0;

  /* initrd and kernel_cmdline are optional */
  val_initrd_path = json_object_object_get (config_বৃক্ষ, "initrd_path");
  যদি (val_initrd_path != ফাঁকা && json_object_is_type (val_initrd_path, json_type_string))
    initrd_path = json_object_get_string (val_initrd_path);

  val_kernel_cmdline = json_object_object_get (config_বৃক্ষ, "kernel_cmdline");
  যদি (val_kernel_cmdline != ফাঁকা && json_object_is_type (val_kernel_cmdline, json_type_string))
    kernel_cmdline = json_object_get_string (val_kernel_cmdline);

  krun_set_kernel = dlsym (handle, "krun_set_kernel");
  যদি (krun_set_kernel == ফাঁকা)
    ফেরত crun_make_error (err, 0, "could not find symbol in krun library");

  ret = krun_set_kernel (ctx_id,
                         json_object_get_string (kernel_path),
                         (uint32_t) json_object_get_int64 (kernel_format),
                         initrd_path, kernel_cmdline);

  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, -ret, "could not configure a krun external kernel");

  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
libkrun_read_vm_config (কাঠামো krun_config *kconf, পূর্ণসংখ্যা rootfsfd, ধ্রুবক অক্ষর *rootfs, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;
  cleanup_free অক্ষর *config = ফাঁকা;
  cleanup_close পূর্ণসংখ্যা pathfd = -1;
  cleanup_close পূর্ণসংখ্যা fd = -1;
  proc_fd_path_t fd_path;
  কাঠামো stat st;

  /* The content of the rootfs is controlled by the user: look at the file through an
     O_PATH descriptor first and refuse anything that is not a small regular file, as
     opening a FIFO blocks and a device node must not be accessed.  */
  pathfd = safe_openat (rootfsfd, rootfs, KRUN_VM_FILE, O_PATH | O_CLOEXEC | O_NOFOLLOW, 0, err);
  যদি (pathfd < 0)
    {
      // The configuration file is optional, don't generate an error if it's missing.
      যদি (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ফেরত 0;
        }
      ফেরত pathfd;
    }

  ret = fstat (pathfd, &st);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "stat `%s`", KRUN_VM_FILE);

  যদি (! S_ISREG (st.st_mode))
    ফেরত crun_make_error (err, 0, "`%s` is not a regular file", KRUN_VM_FILE);

  get_proc_self_fd_path (fd_path, pathfd);

  fd = open (fd_path, O_RDONLY | O_CLOEXEC);
  যদি (UNLIKELY (fd < 0))
    ফেরত crun_make_error (err, errno, "open `%s`", KRUN_VM_FILE);

  ret = read_all_fd_with_max_আকার (fd, KRUN_VM_FILE, &config, ফাঁকা, KRUN_VM_FILE_MAX_আকার, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = parse_json_file (&kconf->config_doc, config, ফাঁকা, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  kconf->config_বৃক্ষ = kconf->config_doc;
  ফেরত 0;
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
স্থির পূর্ণসংখ্যা
libkrun_parse_resource_configuration (json_object *config_বৃক্ষ, libcrun_container_t *container, ধ্রুবক অক্ষর *annotation, ধ্রুবক অক্ষর *চাবি, bool use_config_file)
{
  অক্ষর *val_str, *endptr;
  পূর্ণসংখ্যা val = -1;
  json_object *val_json = ফাঁকা;

  val_str = (অক্ষর *) find_annotation (container, annotation);
  যদি (val_str != ফাঁকা)
    {
      errno = 0;
      val = (পূর্ণসংখ্যা) strtol (val_str, &endptr, 10);
      যদি (errno != 0 || endptr == val_str || *endptr != '\0')
        /* Annotations value is not a valid integer. */
        error (EXIT_FAILURE, 0, "krun annotation %s value cannot be converted to an integer", annotation);

      যদি (val < 0)
        error (EXIT_FAILURE, 0, "krun annotation %s value must be a positive integer", annotation);

      ফেরত val;
    }
  অন্যথা যদি (use_config_file && config_বৃক্ষ != ফাঁকা)
    {
      val_json = json_object_object_get (config_বৃক্ষ, চাবি);
      যদি (val_json == ফাঁকা)
        ফেরত val;
      যদি (! json_object_is_type (val_json, json_type_int))
        error (EXIT_FAILURE, 0, "krun krun_vm.json %s value is not an integer", চাবি);

      val = (পূর্ণসংখ্যা) json_object_get_int64 (val_json);
    }

  ফেরত val;
}

স্থির পূর্ণসংখ্যা
libkrun_parse_string_configuration (json_object *config_বৃক্ষ, libcrun_container_t *container,
                                    ধ্রুবক অক্ষর *annotation, ধ্রুবক অক্ষর *চাবি,
                                    ধ্রুবক অক্ষর **মান, libcrun_error_t *err, bool use_config_file)
{
  ধ্রুবক অক্ষর *val;
  json_object *val_json = ফাঁকা;

  *মান = ফাঁকা;

  val = find_annotation (container, annotation);
  যদি (val != ফাঁকা)
    {
      *মান = val;
      ফেরত 0;
    }

  যদি (! use_config_file || config_বৃক্ষ == ফাঁকা)
    ফেরত 0;

  val_json = json_object_object_get (config_বৃক্ষ, চাবি);
  যদি (val_json == ফাঁকা)
    ফেরত 0;

  যদি (! json_object_is_type (val_json, json_type_string))
    ফেরত crun_make_error (err, 0, ".krun_vm.json %s value is not a string", চাবি);

  *মান = json_object_get_string (val_json);
  ফেরত 0;
}

স্থির শূন্য
libkrun_make_tap_mac (ধ্রুবক অক্ষর *id, uint8_t mac[6])
{
  uint64_t hash = 14695981039346656037ULL;
  size_t i;

  জন্য (; *id != '\0'; id++)
    {
      hash ^= (uint8_t) *id;
      hash *= 1099511628211ULL;
    }

  জন্য (i = 0; i < 6; i++)
    mac[i] = (uint8_t) (hash >> (i * 8));

  mac[0] = (mac[0] & 0xfe) | 0x02;
}

স্থির পূর্ণসংখ্যা
libkrun_configure_vm (uint32_t ctx_id, শূন্য *handle, কাঠামো krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_set_vm_config) (uint32_t ctx_id, uint8_t অঙ্ক_vcpus, uint32_t ram_mib);
  int32_t (*krun_add_net_unixstream) (uint32_t ctx_id, ধ্রুবক অক্ষর *c_path, পূর্ণসংখ্যা fd, uint8_t *ধ্রুবক c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_add_net_tap) (uint32_t ctx_id, ধ্রুবক অক্ষর *c_tap_name, ধ্রুবক uint8_t *c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_set_gpu_options) (uint32_t ctx_id, uint32_t virgl_flags);
  int32_t (*krun_set_gpu_render_server_fd) (uint32_t ctx_id, পূর্ণসংখ্যা fd);
  পূর্ণসংখ্যা cpus, ram_mib, nested_virt, ret;
  cpu_set_t set;

  /* We let the OCI set the number of vCPUs for the VM, since cgroups CPU restrictions still apply. */
  cpus = libkrun_parse_resource_configuration (kconf->config_বৃক্ষ, container, "krun.cpus", "cpus", true);
  যদি (cpus <= 0)
    {
      CPU_ZERO (&set);
      যদি (sched_getaffinity (getpid (), আকার (set), &set) == 0)
        cpus = MIN (CPU_গুনতি (&set), LIBKRUN_MAX_VCPUS);
      অন্যথা
        cpus = 1;
    }

  /* We let the OCI set the amount of RAM for the VM, since cgroups memory restrictions still apply. */
  ram_mib = libkrun_parse_resource_configuration (kconf->config_বৃক্ষ, container, "krun.ram_mib", "ram_mib", true);
  যদি (ram_mib <= LIBKRUN_MINIMUM_RAM_MIB)
    {
      যদি (def && def->linux && def->linux->resources && def->linux->resources->memory
          && def->linux->resources->memory->limit_present)
        ram_mib = def->linux->resources->memory->limit / (1024 * 1024);
      অন্যথা
        ram_mib = LIBKRUN_DEFAULT_RAM_MIB;
    }

  krun_set_vm_config = dlsym (handle, "krun_set_vm_config");

  যদি (krun_set_vm_config == ফাঁকা)
    ফেরত crun_make_error (err, 0, "could not find symbol in the krun library");

  ret = krun_set_vm_config (ctx_id, cpus, ram_mib);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, -ret, "could not set krun vm configuration");

  nested_virt = libkrun_parse_resource_configuration (kconf->config_বৃক্ষ, container, "krun.nested_virt", "nested_virt", false);
  যদি (nested_virt > 0)
    {
      int32_t (*krun_check_nested_virt) (শূন্য);
      int32_t (*krun_set_nested_virt) (uint32_t ctx_id, bool enabled);

      krun_check_nested_virt = dlsym (handle, "krun_check_nested_virt");
      যদি (krun_check_nested_virt != ফাঁকা && krun_check_nested_virt () != 1)
        libcrun_warning ("nested virtualization requested but may not be supported on this host");

      krun_set_nested_virt = dlsym (handle, "krun_set_nested_virt");
      যদি (krun_set_nested_virt == ফাঁকা)
        ফেরত crun_make_error (err, 0, "could not find symbol `krun_set_nested_virt` in the krun library");

      ret = krun_set_nested_virt (ctx_id, true);
      যদি (UNLIKELY (ret < 0))
        ফেরত crun_make_error (err, -ret, "could not enable nested virtualization");
    }

  যদি (kconf->tap_name != ফাঁকা)
    {
      krun_add_net_tap = dlsym (handle, "krun_add_net_tap");
      যদি (krun_add_net_tap == ফাঁকা)
        ফেরত crun_make_error (err, 0, "could not find symbol `krun_add_net_tap` in the krun library");

      uint8_t mac[6];
      libkrun_make_tap_mac (container->context->id, mac);
      ret = krun_add_net_tap (ctx_id, kconf->tap_name, &mac[0], COMPAT_NET_FEATURES, 0);
      যদি (UNLIKELY (ret < 0))
        ফেরত crun_make_error (err, -ret, "could not add krun TAP interface `%s`", kconf->tap_name);
    }
  অন্যথা যদি (kconf->use_passt)
    {
      krun_add_net_unixstream = dlsym (handle, "krun_add_net_unixstream");
      যদি (krun_add_net_unixstream == ফাঁকা)
        ফেরত crun_make_error (err, 0, "could not find symbol `krun_add_net_unixstream` in the krun library");

      uint8_t mac[] = { 0x5a, 0x94, 0xef, 0xe4, 0x0c, 0xee };
      ret = krun_add_net_unixstream (ctx_id, ফাঁকা, kconf->passt_fds[FD_PAIR_PARENT], &mac[0], COMPAT_NET_FEATURES, NET_FLAG_DHCP_CLIENT);
      যদি (UNLIKELY (ret == -EINVAL))
        ret = krun_add_net_unixstream (ctx_id, ফাঁকা, kconf->passt_fds[FD_PAIR_PARENT], &mac[0], COMPAT_NET_FEATURES, 0);
      যদি (UNLIKELY (ret < 0))
        ফেরত crun_make_error (err, -ret, "could not set krun net configuration");
    }

  যদি (kconf->gpu_flags > 0)
    {
      krun_set_gpu_options = dlsym (kconf->handle, "krun_set_gpu_options");
      যদি (krun_set_gpu_options == ফাঁকা)
        ফেরত crun_make_error (err, 0, "gpu requested but the version of libkrun in this system does not support it");

      ret = krun_set_gpu_options (kconf->ctx_id, kconf->gpu_flags);
      যদি (UNLIKELY (ret < 0))
        ফেরত crun_make_error (err, -ret, "gpu requested but could not configure virtio gpu device");

      যদি (kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER)
        {
          krun_set_gpu_render_server_fd = dlsym (kconf->handle, "krun_set_gpu_render_server_fd");
          যদি (krun_set_gpu_render_server_fd == ফাঁকা)
            ফেরত crun_make_error (err, 0, "gpu with render server requested but the version of libkrun in this system does not support it");

          ret = krun_set_gpu_render_server_fd (kconf->ctx_id, kconf->gpu_render_server_fd);
          যদি (UNLIKELY (ret < 0))
            ফেরত crun_make_error (err, -ret, "gpu with render server requested but could not configure its file descriptor");
        }
    }

  যদি (kconf->config_বৃক্ষ != ফাঁকা)
    {
      /* Try to configure an external kernel. If the configuration file doesn't
       * specify a kernel, libkrun automatically fall back to using libkrunfw,
       * if the library is present and was loaded while creating the context.
       */
      পূর্ণসংখ্যা custom_kernel = libkrun_parse_resource_configuration (kconf->config_বৃক্ষ, container, "krun.custom_kernel", "custom_kernel", false);
      যদি (custom_kernel > 0)
        {
          ret = libkrun_configure_kernel (ctx_id, handle, kconf->config_বৃক্ষ, err);
          যদি (UNLIKELY (ret))
            ফেরত ret;
        }
    }

  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
libkrun_configure_flavor (শূন্য *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret, sev_indicated = 0, awsnitro_indicated = 0;
  কাঠামো krun_config *kconf = (কাঠামো krun_config *) cookie;
  ধ্রুবক অক্ষর *flavor = ফাঁকা;
  শূন্য *close_handles[2];

  close_handles[0] = ফাঁকা;
  close_handles[1] = ফাঁকা;

  // Check if the user provided the krun variant through OCI annotations.
  flavor = find_annotation (container, "krun.variant");
  যদি (flavor != ফাঁকা)
    {
      sev_indicated |= strcmp (flavor, KRUN_FLAVOR_SEV) == 0;
      awsnitro_indicated |= strcmp (flavor, KRUN_FLAVOR_AWS_NITRO) == 0;
    }

  যদি (sev_indicated)
    {
      যদি (kconf->handle_sev == ফাঁকা)
        error (EXIT_FAILURE, 0, "the container requires libkrun-sev but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_awsnitro;

      kconf->handle = kconf->handle_sev;
      kconf->ctx_id = kconf->ctx_id_sev;
      kconf->sev = true;
    }
  অন্যথা যদি (awsnitro_indicated)
    {
      যদি (kconf->handle_awsnitro == ফাঁকা)
        error (EXIT_FAILURE, 0, "the container requires libkrun-awsnitro but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_sev;

      kconf->handle = kconf->handle_awsnitro;
      kconf->ctx_id = kconf->ctx_id_awsnitro;
      kconf->awsnitro = true;
    }
  অন্যথা
    {
      যদি (kconf->handle == ফাঁকা)
        error (EXIT_FAILURE, 0, "the container requires libkrun but it's not available");

      close_handles[0] = kconf->handle_sev;
      close_handles[1] = kconf->handle_awsnitro;
    }

  // We no longer need the other two libkrun handles.
  জন্য (পূর্ণসংখ্যা i = 0; i < 2; i++)
    {
      যদি (close_handles[i] == ফাঁকা)
        চালু;

      ret = dlclose (close_handles[i]);
      যদি (UNLIKELY (ret != 0))
        ফেরত crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }

  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
libkrun_exec (শূন্য *cookie, libcrun_container_t *container, ধ্রুবক অক্ষর *pathname, অক্ষর *ধ্রুবক argv[])
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_set_log_level) (uint32_t level);
  পূর্ণসংখ্যা (*krun_start_enter) (uint32_t ctx_id);
  int32_t (*krun_add_virtiofs2) (uint32_t ctx_id, ধ্রুবক অক্ষর *c_tag, ধ্রুবক অক্ষর *c_path, uint64_t shm_আকার);
  int32_t (*krun_set_মূল_disk) (uint32_t ctx_id, ধ্রুবক অক্ষর *disk_path);
  int32_t (*krun_set_tee_config_file) (uint32_t ctx_id, ধ্রুবক অক্ষর *file_path);
  int32_t (*krun_set_console_output) (uint32_t ctx_id, ধ্রুবক অক্ষর *c_filepath);
  int32_t (*krun_set_exec) (uint32_t ctx_id, ধ্রুবক অক্ষর *exec_path,
                            ধ্রুবক অক্ষর *ধ্রুবক argv[], ধ্রুবক অক্ষর *ধ্রুবক envp[]);
  কাঠামো krun_config *kconf = (কাঠামো krun_config *) cookie;
  শূন্য *handle;
  int32_t ctx_id, ret;
  libcrun_error_t err;

  ret = libkrun_configure_flavor (cookie, container, &err);
  যদি (UNLIKELY (ret < 0))
    {
      পূর্ণসংখ্যা errcode = crun_error_get_errno (&err);
      crun_error_release (&err);
      error (EXIT_FAILURE, errcode, "unable to configure libkrun flavor");
    }

  // /dev/kvm is required for all non AWS nitro workloads.
  যদি (! kconf->awsnitro && ! kconf->has_kvm)
    error (EXIT_FAILURE, 0, "`/dev/kvm` unavailable");

  handle = kconf->handle;
  ctx_id = kconf->ctx_id;

  krun_set_log_level = dlsym (handle, "krun_set_log_level");
  krun_start_enter = dlsym (handle, "krun_start_enter");
  যদি (krun_set_log_level == ফাঁকা || krun_start_enter == ফাঁকা)
    error (EXIT_FAILURE, 0, "could not find symbol in the krun library");

  /* Set log level according to crun's verbosity. */
  নির্বাচন (libcrun_get_verbosity ())
    {
    ক্ষেত্র LIBCRUN_VERBOSITY_DEBUG:
      krun_set_log_level (KRUN_LOG_LEVEL_DEBUG);
      বিরতি;
    ক্ষেত্র LIBCRUN_VERBOSITY_WARNING:
      krun_set_log_level (KRUN_LOG_LEVEL_WARN);
      বিরতি;
    পূর্বনির্ধারিত:
      krun_set_log_level (KRUN_LOG_LEVEL_ERROR);
      বিরতি;
    }

  যদি (kconf->sev)
    {
      krun_set_মূল_disk = dlsym (handle, "krun_set_root_disk");
      krun_set_tee_config_file = dlsym (handle, "krun_set_tee_config_file");
      যদি (krun_set_মূল_disk == ফাঁকা || krun_set_tee_config_file == ফাঁকা)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-sev.so`");

      ret = krun_set_মূল_disk (ctx_id, "/disk.img");
      যদি (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set root disk");

      ret = krun_set_tee_config_file (ctx_id, KRUN_SEV_FILE);
      যদি (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set krun tee config file");
    }
  অন্যথা
    {
      json_object *val_virtiofs_tag = ফাঁকা;
      json_object *val_virtiofs_shm_আকার = ফাঁকা;
      ধ্রুবক অক্ষর *virtiofs_tag = ফাঁকা;
      // Default to a conservative DAX size of 512MB, just like krun_set_root() does.
      uint64_t virtiofs_shm_আকার = 512 * 1024 * 1024ULL;

      যদি (kconf->config_বৃক্ষ != ফাঁকা)
        {
          val_virtiofs_tag = json_object_object_get (kconf->config_বৃক্ষ, "virtiofs_tag");
          যদি (val_virtiofs_tag != ফাঁকা && json_object_is_type (val_virtiofs_tag, json_type_string))
            virtiofs_tag = json_object_get_string (val_virtiofs_tag);

          val_virtiofs_shm_আকার = json_object_object_get (kconf->config_বৃক্ষ, "virtiofs_shm_size");
          যদি (val_virtiofs_shm_আকার != ফাঁকা && json_object_is_type (val_virtiofs_shm_আকার, json_type_int))
            virtiofs_shm_আকার = json_object_get_uint64 (val_virtiofs_shm_আকার);
        }

      যদি (virtiofs_tag == ফাঁকা)
        virtiofs_tag = "/dev/root";

      krun_add_virtiofs2 = dlsym (handle, "krun_add_virtiofs2");

      যদি (krun_add_virtiofs2 == ফাঁকা)
        error (EXIT_FAILURE, 0, "could not find symbol `krun_add_virtiofs2` in `libkrun.so`");

      ret = krun_add_virtiofs2 (ctx_id, virtiofs_tag, "/", virtiofs_shm_আকার);
      যদি (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not add virtiofs root with tag `%s`", virtiofs_tag);
    }

  যদি (kconf->awsnitro)
    {
      krun_set_console_output = dlsym (handle, "krun_set_console_output");
      krun_set_exec = dlsym (handle, "krun_set_exec");
      যদি (krun_set_console_output == ফাঁকা || krun_set_exec == ফাঁকা)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-awsnitro.so`");

      // Redirect all enclave output (read from vsock) to stdout.
      ret = krun_set_console_output (ctx_id, "/dev/stdout");
      যদি (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not redirect enclave output to stdout");

      ret = krun_set_exec (ctx_id, pathname, (ধ্রুবক অক্ষর *ধ্রুবক *) argv,
                           (ধ্রুবক অক্ষর *ধ্রুবক *) def->process->env);
      যদি (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set enclave execution arguments");
    }

  ret = libkrun_configure_vm (ctx_id, handle, kconf, container, &err);
  যদি (UNLIKELY (ret))
    {
      পূর্ণসংখ্যা errcode = crun_error_get_errno (&err);
      libcrun_error_t *tmp_err = &err;
      libcrun_error_report_and_release (tmp_err);
      error (EXIT_FAILURE, errcode, "could not configure krun vm");
    }

  json_object_put (kconf->config_doc);

  ret = krun_start_enter (ctx_id);
  যদি (UNLIKELY (ret < 0))
    error (EXIT_FAILURE, -ret, "could not start krun");

  ফেরত ret;
}

স্থির পূর্ণসংখ্যা
libkrun_start_render_server (কাঠামো krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;

  kconf->gpu_flags = libkrun_parse_resource_configuration (kconf->config_বৃক্ষ, container, "krun.gpu_flags", "gpu_flags", false);
  যদি (kconf->gpu_flags > 0)
    {
      যদি (access ("/dev/dri", F_OK) != 0)
        ফেরত crun_make_error (err, errno, "gpu requested but /dev/dri is not available");

      যদি ((kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER) != 0)
        {
          অক্ষর *rs_argv[40];
          পূর্ণসংখ্যা rs_argc = 0;
          অক্ষর fd_as_str[16];
          পূর্ণসংখ্যা gpu_fds[2];
          pid_t pid;
          পূর্ণসংখ্যা null;

          যদি (access ("/usr/libexec/virgl_render_server", X_OK) != 0)
            ফেরত crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/libexec/virgl_render_server binary");

          যদি (access ("/usr/bin/bwrap", X_OK) != 0)
            ফেরত crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/bin/bwrap binary");

          ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, gpu_fds);
          যদি (UNLIKELY (ret < 0))
            ফেরত crun_make_error (err, errno, "could not create socketpair for virgl_render_server");

          snprintf (fd_as_str, আকার (fd_as_str), "%d", gpu_fds[FD_PAIR_CHILD]);

          rs_argv[rs_argc++] = (অক্ষর *) "bwrap";
          rs_argv[rs_argc++] = (অক্ষর *) "--ro-bind";
          rs_argv[rs_argc++] = (অক্ষর *) "/usr";
          rs_argv[rs_argc++] = (অক্ষর *) "/usr";
          যদি (access ("/lib", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (অক্ষর *) "--ro-bind";
              rs_argv[rs_argc++] = (অক্ষর *) "/lib";
              rs_argv[rs_argc++] = (অক্ষর *) "/lib";
            }
          যদি (access ("/lib64", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (অক্ষর *) "--ro-bind";
              rs_argv[rs_argc++] = (অক্ষর *) "/lib64";
              rs_argv[rs_argc++] = (অক্ষর *) "/lib64";
            }
          rs_argv[rs_argc++] = (অক্ষর *) "--ro-bind";
          rs_argv[rs_argc++] = (অক্ষর *) "/etc";
          rs_argv[rs_argc++] = (অক্ষর *) "/etc";
          rs_argv[rs_argc++] = (অক্ষর *) "--ro-bind";
          rs_argv[rs_argc++] = (অক্ষর *) "/sys";
          rs_argv[rs_argc++] = (অক্ষর *) "/sys";
          rs_argv[rs_argc++] = (অক্ষর *) "--dev";
          rs_argv[rs_argc++] = (অক্ষর *) "/dev";
          rs_argv[rs_argc++] = (অক্ষর *) "--dev-bind";
          rs_argv[rs_argc++] = (অক্ষর *) "/dev/dri";
          rs_argv[rs_argc++] = (অক্ষর *) "/dev/dri";
          rs_argv[rs_argc++] = (অক্ষর *) "--proc";
          rs_argv[rs_argc++] = (অক্ষর *) "/proc";
          rs_argv[rs_argc++] = (অক্ষর *) "--tmpfs";
          rs_argv[rs_argc++] = (অক্ষর *) "/tmp";
          rs_argv[rs_argc++] = (অক্ষর *) "--unshare-pid";
          rs_argv[rs_argc++] = (অক্ষর *) "--unshare-ipc";
          rs_argv[rs_argc++] = (অক্ষর *) "--unshare-net";
          rs_argv[rs_argc++] = (অক্ষর *) "--";
          rs_argv[rs_argc++] = (অক্ষর *) "/usr/libexec/virgl_render_server";
          rs_argv[rs_argc++] = (অক্ষর *) "--socket-fd";
          rs_argv[rs_argc++] = fd_as_str;
          rs_argv[rs_argc] = ফাঁকা;

          pid = fork ();
          যদি (pid < 0)
            {
              close (gpu_fds[FD_PAIR_PARENT]);
              close (gpu_fds[FD_PAIR_CHILD]);
              ফেরত crun_make_error (err, errno, "could not fork for virgl_render_server");
            }
          অন্যথা যদি (pid == 0)
            {
              close (gpu_fds[FD_PAIR_PARENT]);

              null = open ("/dev/null", O_WRONLY);
              যদি (null == -1)
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

  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
libkrun_configure_network (শূন্য *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  কাঠামো krun_config *kconf = (কাঠামো krun_config *) cookie;
  pid_t pid;
  অক্ষর *passt_argv[10];
  অক্ষর fd_as_str[16];
  পূর্ণসংখ্যা use_passt;
  পূর্ণসংখ্যা argv_idx;
  পূর্ণসংখ্যা status;
  পূর্ণসংখ্যা null;
  পূর্ণসংখ্যা ret;

  ret = libkrun_parse_string_configuration (kconf->config_বৃক্ষ, container,
                                            "krun.tap_name", "tap_name",
                                            &kconf->tap_name, err, false);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  যদি (kconf->tap_name != ফাঁকা && is_empty_string (kconf->tap_name))
    ফেরত crun_make_error (err, 0, "krun.tap_name cannot be empty");

  use_passt = libkrun_parse_resource_configuration (kconf->config_বৃক্ষ, container, "krun.use_passt", "use_passt", false);

  যদি (use_passt > 0)
    {
      যদি (kconf->tap_name != ফাঁকা)
        ফেরত crun_make_error (err, 0, "krun.tap_name and krun.use_passt are mutually exclusive");

      kconf->use_passt = 1;
    }
  অন্যথা
    ফেরত 0;

  ret = socketpair (AF_UNIX, SOCK_STREAM, 0, kconf->passt_fds);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "create passt socketpair");

  snprintf (fd_as_str, আকার (fd_as_str), "%d", kconf->passt_fds[FD_PAIR_CHILD]);

  argv_idx = 0;
  passt_argv[argv_idx++] = (অক্ষর *) "passt";
  passt_argv[argv_idx++] = (অক্ষর *) "-t";
  passt_argv[argv_idx++] = (অক্ষর *) "all";

  যদি (! kconf->has_awsnitro)
    {
      passt_argv[argv_idx++] = (অক্ষর *) "-u";
      passt_argv[argv_idx++] = (অক্ষর *) "all";
      passt_argv[argv_idx++] = (অক্ষর *) "--no-dhcp-dns";
    }

  /* Set --no-map-gw. If the gateway for the network is also the DNS server
   * we won't be able to query the DNS server without --no-map-gw.
   * See discussion in https://github.com/containers/crun/pull/2099
   */
  passt_argv[argv_idx++] = (অক্ষর *) "--no-map-gw";

  passt_argv[argv_idx++] = (অক্ষর *) "--fd";
  passt_argv[argv_idx++] = fd_as_str;
  passt_argv[argv_idx] = ফাঁকা;

  pid = fork ();
  যদি (pid < 0)
    ফেরত crun_make_error (err, errno, "fork passt");
  অন্যথা যদি (pid == 0)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t i;

      close (kconf->passt_fds[FD_PAIR_PARENT]);

      null = open ("/dev/null", O_WRONLY);
      যদি (null == -1)
        _exit (EXIT_FAILURE);

      /* Remount /proc if the parent detached its PID namespace, as passt(1)
       * needs a consistent /proc view to operate properly.
       */
      জন্য (i = 0; i < def->linux->namespaces_len; i++)
        {
          পূর্ণসংখ্যা মান = libcrun_find_namespace (def->linux->namespaces[i]->type);

          যদি (মান == CLONE_NEWPID)
            {
              যদি (mount ("", "/proc", "proc", MS_NOSUID | MS_NODEV | MS_NOEXEC, ফাঁকা))
                _exit (EXIT_FAILURE);

              বিরতি;
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
  যদি (! (WIFEXITED (status)) || WEXITSTATUS (status) != 0)
    ফেরত crun_make_error (err, 0, "start passt");

  ফেরত 0;
}

/* Return true if the spec already declares a device with the given PATH.  */
স্থির bool
spec_has_device (runtime_spec_schema_config_schema *def, ধ্রুবক অক্ষর *path)
{
  size_t i;

  জন্য (i = 0; i < def->linux->devices_len; i++)
    যদি (strcmp (def->linux->devices[i]->path, path) == 0)
      ফেরত true;
  ফেরত false;
}

/* libkrun_create_kvm_device: explicitly adds kvm device.  */
স্থির পূর্ণসংখ্যা
libkrun_configure_container (শূন্য *cookie, গণনা handler_configure_phase phase,
                             libcrun_context_t *context, libcrun_container_t *container,
                             ধ্রুবক অক্ষর *rootfs, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret, rootfsfd;
  কাঠামো krun_config *kconf = (কাঠামো krun_config *) cookie;
  কাঠামো device_s kvm_device = { "/dev/kvm", "c", 10, 232, 0666, 0, 0 };
  কাঠামো device_s sev_device = { "/dev/sev", "c", 10, 124, 0666, 0, 0 };
  কাঠামো device_s nitro_device = { "/dev/nitro_enclaves", "c", 10, 122, 0666, 0, 0 };
  cleanup_close পূর্ণসংখ্যা devfd = -1;
  cleanup_close পূর্ণসংখ্যা rootfsfd_cleanup = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  bool create_sev = false, create_awsnitro = false;
  bool is_user_ns;

  যদি (rootfs == ফাঁকা)
    rootfsfd = AT_FDCWD;
  অন্যথা
    {
      rootfsfd = rootfsfd_cleanup = open (rootfs, O_PATH | O_CLOEXEC);
      যদি (UNLIKELY (rootfsfd < 0))
        ফেরত crun_make_error (err, errno, "open `%s`", rootfs);
    }

  যদি (phase == HANDLER_CONFIGURE_BEFORE_USERNS)
    {
      cleanup_free অক্ষর *origin_config_path = ফাঁকা;
      cleanup_free অক্ষর *state_dir = ফাঁকা;
      cleanup_free অক্ষর *config = ফাঁকা;
      cleanup_close পূর্ণসংখ্যা fd = -1;
      size_t config_আকার;

      ret = libcrun_get_state_directory (&state_dir, context->state_মূল, context->id, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      ret = append_paths (&origin_config_path, err, state_dir, "config.json", ফাঁকা);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      ret = read_all_file (origin_config_path, &config, &config_আকার, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      /* CVE-2025-24965: the content below rootfs cannot be trusted because it is controlled by the user.  We
         must ensure the file is opened below the rootfs directory.  Whatever the image ships at that name
         could also be a device node or a FIFO, so never open it: unlink it and create a new regular file.  */
      ret = unlinkat (rootfsfd, KRUN_CONFIG_FILE, 0);
      যদি (UNLIKELY (ret < 0 && errno != ENOENT))
        ফেরত crun_make_error (err, errno, "unlink `%s`", KRUN_CONFIG_FILE);

      fd = safe_openat (rootfsfd, rootfs, KRUN_CONFIG_FILE, WRITE_FILE_DEFAULT_FLAGS | O_EXCL | O_NOFOLLOW, S_IRUSR | S_IRGRP | S_IROTH, err);
      যদি (UNLIKELY (fd < 0))
        ফেরত fd;

      ret = safe_write (fd, KRUN_CONFIG_FILE, config, config_আকার, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      ret = libkrun_read_vm_config (kconf, rootfsfd, rootfs, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      ret = libkrun_start_render_server (kconf, container, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;
    }

  যদি (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    ফেরত 0;

  ret = libkrun_configure_network (cookie, container, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  /* Do nothing if /dev/kvm is already present in spec */
  যদি (spec_has_device (def, "/dev/kvm"))
    ফেরত 0;

  যদি (kconf->handle_sev != ফাঁকা)
    create_sev = ! spec_has_device (def, "/dev/sev");

  যদি (kconf->handle_awsnitro != ফাঁকা)
    create_awsnitro = ! spec_has_device (def, "/dev/nitro_enclaves");

  devfd = safe_openat (rootfsfd, rootfs, "dev", O_PATH | O_DIRECTORY | O_CLOEXEC, 0, err);
  যদি (UNLIKELY (devfd < 0))
    ফেরত devfd;

  ret = check_running_in_user_namespace (err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;
  is_user_ns = ret;

  যদি (kconf->has_kvm)
    {
      ret = libcrun_create_dev (container, devfd, -1, &kvm_device, is_user_ns, true, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;
    }

  যদি (create_sev)
    {
      ret = libcrun_create_dev (container, devfd, -1, &sev_device, is_user_ns, true, err);
      যদি (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_sev);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;
          kconf->handle_sev = ফাঁকা;
        }
    }

  যদি (create_awsnitro)
    {
      ret = libcrun_create_dev (container, devfd, -1, &nitro_device, is_user_ns, true, err);
      যদি (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_awsnitro);
          যদি (UNLIKELY (ret < 0))
            ফেরত ret;
          kconf->handle_awsnitro = ফাঁকা;
        }
    }

  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
libkrun_load (শূন্য **cookie, libcrun_error_t *err)
{
  int32_t ret;
  কাঠামো krun_config *kconf;
  ধ্রুবক অক্ষর *libkrun_so = "libkrun.so.1";
  ধ্রুবক অক্ষর *libkrun_sev_so = "libkrun-sev.so.1";
  ধ্রুবক অক্ষর *libkrun_awsnitro_so = "libkrun-awsnitro.so.1";

  kconf = malloc (আকার (কাঠামো krun_config));
  যদি (kconf == ফাঁকা)
    ফেরত crun_make_error (err, 0, "could not allocate memory for krun_config");
  memset (kconf, 0, আকার (কাঠামো krun_config));

  kconf->gpu_render_server_fd = -1;

  kconf->handle = dlopen (libkrun_so, RTLD_NOW);
  kconf->handle_sev = dlopen (libkrun_sev_so, RTLD_NOW);
  kconf->handle_awsnitro = dlopen (libkrun_awsnitro_so, RTLD_NOW);

  যদি (kconf->handle == ফাঁকা && kconf->handle_sev == ফাঁকা && kconf->handle_awsnitro == ফাঁকা)
    {
      মুক্ত (kconf);
      ফেরত crun_make_error (err, 0, "failed to open `%s`, `%s`, and `%s` for krun_config: %s", libkrun_so, libkrun_sev_so, libkrun_awsnitro_so, dlerror ());
    }

  kconf->sev = false;
  kconf->awsnitro = false;

  /* Newer versions of libkrun no longer link against libkrunfw and
     instead they open it when creating the context. This implies
     we need to call "krun_create_ctx" before switching namespaces
     or it won't be able to find the library bundling the kernel. */
  যদি (kconf->handle)
    {
      ret = libkrun_create_context (kconf->handle, err);
      যদি (UNLIKELY (ret < 0))
        যাও error;
      kconf->ctx_id = ret;
    }

  যদি (kconf->handle_sev)
    {
      ret = libkrun_create_context (kconf->handle_sev, err);
      যদি (UNLIKELY (ret < 0))
        যাও error;
      kconf->ctx_id_sev = ret;
    }
  যদি (kconf->handle_awsnitro)
    {
      ret = libkrun_create_context (kconf->handle_awsnitro, err);
      যদি (UNLIKELY (ret < 0))
        যাও error;
      kconf->ctx_id_awsnitro = ret;
    }

  *cookie = kconf;

  ফেরত 0;

error:
  যদি (kconf->handle)
    dlclose (kconf->handle);
  যদি (kconf->handle_sev)
    dlclose (kconf->handle_sev);
  যদি (kconf->handle_awsnitro)
    dlclose (kconf->handle_awsnitro);
  মুক্ত (kconf);
  ফেরত ret;
}

স্থির পূর্ণসংখ্যা
libkrun_unload (শূন্য *cookie, libcrun_error_t *err)
{
  পূর্ণসংখ্যা r;

  কাঠামো krun_config *kconf = (কাঠামো krun_config *) cookie;
  যদি (kconf != ফাঁকা)
    {
      যদি (kconf->handle != ফাঁকা)
        {
          r = dlclose (kconf->handle);
          যদি (UNLIKELY (r != 0))
            ফেরত crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
        }
      যদি (kconf->handle_sev != ফাঁকা)
        {
          r = dlclose (kconf->handle_sev);
          যদি (UNLIKELY (r != 0))
            ফেরত crun_make_error (err, 0, "could not unload handle_sev: `%s`", dlerror ());
        }
      যদি (kconf->handle_awsnitro != ফাঁকা)
        {
          r = dlclose (kconf->handle_awsnitro);
          যদি (UNLIKELY (r != 0))
            ফেরত crun_make_error (err, 0, "could not unload handle_awsnitro: `%s`", dlerror ());
        }
      মুক্ত (kconf);
    }
  ফেরত 0;
}

স্থির runtime_spec_schema_defs_linux_device_cgroup *
make_oci_spec_dev (ধ্রুবক অক্ষর *type, dev_t device, bool allow, ধ্রুবক অক্ষর *access)
{
  runtime_spec_schema_defs_linux_device_cgroup *dev = xmalloc0 (আকার (*dev));

  dev->allow = allow;
  dev->allow_present = 1;

  dev->type = xstrdup (type);

  dev->major = major (device);
  dev->major_present = 1;

  dev->minor = minor (device);
  dev->minor_present = 1;

  dev->access = xstrdup (access);

  ফেরত dev;
}

/* stat an optional device: on ENOENT clear *PRESENT, any other error fails.  */
স্থির পূর্ণসংখ্যা
stat_optional_device (ধ্রুবক অক্ষর *path, কাঠামো stat *st, bool *present, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret = stat (path, st);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (errno != ENOENT)
        ফেরত crun_make_error (err, errno, "stat `%s`", path);
      *present = false;
    }
  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
libkrun_modify_oci_configuration (শূন্য *cookie arg_unused, libcrun_context_t *context arg_unused,
                                  runtime_spec_schema_config_schema *def,
                                  libcrun_error_t *err)
{
  ধ্রুবক size_t device_আকার = আকার (runtime_spec_schema_defs_linux_device_cgroup);
  কাঠামো krun_config *kconf = (কাঠামো krun_config *) cookie;
  কাঠামো stat st_kvm, st_sev, st_awsnitro;
  bool has_kvm = true, has_sev = true, has_awsnitro = true;
  size_t old_len, new_len;
  পূর্ণসংখ্যা ret;

  /* Always allow the /dev/kvm device.  */

  ret = stat_optional_device ("/dev/kvm", &st_kvm, &has_kvm, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = stat_optional_device ("/dev/sev", &st_sev, &has_sev, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = stat_optional_device ("/dev/nitro_enclaves", &st_awsnitro, &has_awsnitro, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  kconf->has_kvm = has_kvm;
  kconf->has_awsnitro = has_awsnitro;

  যদি (! has_kvm && ! has_awsnitro)
    ফেরত 0;

  /* spec says these are optional, ensure they exist so we can add our devices */
  যদি (def->linux == ফাঁকা)
    def->linux = xmalloc0 (আকার (runtime_spec_schema_config_linux));

  যদি (def->linux->resources == ফাঁকা)
    def->linux->resources = xmalloc0 (আকার (runtime_spec_schema_config_linux_resources));

  old_len = def->linux->resources->devices_len;
  new_len = old_len;
  যদি (has_kvm)
    new_len += has_sev ? 2 : 1;
  যদি (has_awsnitro)
    new_len += 1;

  def->linux->resources->devices = xrealloc (def->linux->resources->devices, device_আকার * (new_len + 1));
  def->linux->resources->devices_len = new_len;

  যদি (has_kvm)
    {
      def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_kvm.st_rdev, true, "rwm");
      যদি (has_sev)
        def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_sev.st_rdev, true, "rwm");
    }

  যদি (has_awsnitro)
    def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_awsnitro.st_rdev, true, "rwm");

  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
libkrun_close_fds (শূন্য *cookie, libcrun_container_t *container, পূর্ণসংখ্যা preserve_fds, libcrun_error_t *err)
{
  কাঠামো krun_config *kconf = (কাঠামো krun_config *) cookie;
  পূর্ণসংখ্যা first_fd_to_close = preserve_fds + 3;
  পূর্ণসংখ্যা নিচের_fd, উপরের_fd;
  পূর্ণসংখ্যা i;

  নিচের_fd = উপরের_fd = -1;

  যদি (kconf->use_passt)
    উপরের_fd = kconf->passt_fds[FD_PAIR_PARENT];

  যদি (kconf->gpu_render_server_fd != -1)
    {
      যদি (kconf->gpu_render_server_fd > উপরের_fd)
        {
          নিচের_fd = উপরের_fd;
          উপরের_fd = kconf->gpu_render_server_fd;
        }
      অন্যথা
        নিচের_fd = kconf->gpu_render_server_fd;
    }

  যদি (first_fd_to_close <= উপরের_fd)
    {
      জন্য (i = first_fd_to_close; i < উপরের_fd; i++)
        {
          // If we're closing proc_fd, make sure to invalidate it.
          যদি (i == container->proc_fd)
            container->proc_fd = -1;
          অন্যথা যদি (i == নিচের_fd)
            চালু;
          close (i);
        }

      first_fd_to_close = উপরের_fd + 1;
    }

  ফেরত mark_or_close_fds_ge_than (container, first_fd_to_close, true, err);
}

কাঠামো custom_handler_s handler_libkrun = {
  .name = "krun",
  .alias = ফাঁকা,
  .feature_string = "LIBKRUN",
  .supports_open_বৃক্ষ_namespace = false,
  .load = libkrun_load,
  .unload = libkrun_unload,
  .run_func = libkrun_exec,
  .configure_container = libkrun_configure_container,
  .modify_oci_configuration = libkrun_modify_oci_configuration,
  .close_fds = libkrun_close_fds,
};

#endif
