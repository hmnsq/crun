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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ "../custom-handler.h"
#অন্তর্ভাৱ "../container.h"
#অন্তর্ভাৱ "../utils.h"
#অন্তর্ভাৱ "../linux.h"
#অন্তর্ভাৱ <unistd.h>
#অন্তর্ভাৱ <sys/stat.h>
#অন্তর্ভাৱ <sys/socket.h>
#অন্তর্ভাৱ <errno.h>
#অন্তর্ভাৱ <sys/mount.h>
#অন্তর্ভাৱ <sys/param.h>
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ <sys/sysmacros.h>
#অন্তর্ভাৱ <fcntl.h>
#অন্তর্ভাৱ <sched.h>
#অন্তর্ভাৱ <ocispec/runtime_spec_schema_config_schema.h>

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

সংরচনা krun_config
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
  পূর্ণাঙ্ক passt_fds[2];
  json_object *config_doc;
  json_object *config_বৃক্ষ;
  bool use_passt;
  ধ্রুৱ ৱর্ণ *tap_name;
  পূর্ণাঙ্ক gpu_render_server_fd;
  পূর্ণাঙ্ক gpu_flags;
};

/* libkrun handler.  */
#যদি HAVE_DLOPEN && HAVE_LIBKRUN
স্থির int32_t
libkrun_create_context (শূন্য *handle, libcrun_error_t *err)
{
  int32_t (*krun_create_ctx) ();
  int32_t ctx_id;

  krun_create_ctx = dlsym (handle, "krun_create_ctx");
  যদি (krun_create_ctx == রিক্ত)
    ঘূৰাই crun_make_error (err, 0, "could not find symbol in the krun library");

  ctx_id = krun_create_ctx ();
  যদি (UNLIKELY (ctx_id < 0))
    ঘূৰাই crun_make_error (err, -ctx_id, "could not create krun context");

  ঘূৰাই ctx_id;
}

স্থির পূর্ণাঙ্ক
libkrun_configure_kernel (uint32_t ctx_id, শূন্য *handle, json_object *config_বৃক্ষ, libcrun_error_t *err)
{
  int32_t (*krun_set_kernel) (uint32_t ctx_id, ধ্রুৱ ৱর্ণ *kernel_path,
                              uint32_t kernel_format, ধ্রুৱ ৱর্ণ *initrd_path, ধ্রুৱ ৱর্ণ *kernel_cmdline);
  json_object *kernel_path = রিক্ত;
  json_object *kernel_format = রিক্ত;
  json_object *val_initrd_path = রিক্ত;
  json_object *val_kernel_cmdline = রিক্ত;
  ধ্রুৱ ৱর্ণ *initrd_path = রিক্ত;
  ধ্রুৱ ৱর্ণ *kernel_cmdline = রিক্ত;
  পূর্ণাঙ্ক ret;

  /* kernel_path and kernel_format must be present */
  kernel_path = json_object_object_get (config_বৃক্ষ, "kernel_path");
  যদি (kernel_path == রিক্ত || ! json_object_is_type (kernel_path, json_type_string))
    ঘূৰাই 0;

  kernel_format = json_object_object_get (config_বৃক্ষ, "kernel_format");
  যদি (kernel_format == রিক্ত || ! json_object_is_type (kernel_format, json_type_int))
    ঘূৰাই 0;

  /* initrd and kernel_cmdline are optional */
  val_initrd_path = json_object_object_get (config_বৃক্ষ, "initrd_path");
  যদি (val_initrd_path != রিক্ত && json_object_is_type (val_initrd_path, json_type_string))
    initrd_path = json_object_get_string (val_initrd_path);

  val_kernel_cmdline = json_object_object_get (config_বৃক্ষ, "kernel_cmdline");
  যদি (val_kernel_cmdline != রিক্ত && json_object_is_type (val_kernel_cmdline, json_type_string))
    kernel_cmdline = json_object_get_string (val_kernel_cmdline);

  krun_set_kernel = dlsym (handle, "krun_set_kernel");
  যদি (krun_set_kernel == রিক্ত)
    ঘূৰাই crun_make_error (err, 0, "could not find symbol in krun library");

  ret = krun_set_kernel (ctx_id,
                         json_object_get_string (kernel_path),
                         (uint32_t) json_object_get_int64 (kernel_format),
                         initrd_path, kernel_cmdline);

  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, -ret, "could not configure a krun external kernel");

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
libkrun_read_vm_config (সংরচনা krun_config *kconf, পূর্ণাঙ্ক rootfsfd, ধ্রুৱ ৱর্ণ *rootfs, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  cleanup_free ৱর্ণ *config = রিক্ত;
  cleanup_close পূর্ণাঙ্ক pathfd = -1;
  cleanup_close পূর্ণাঙ্ক fd = -1;
  proc_fd_path_t fd_path;
  সংরচনা stat st;

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
          ঘূৰাই 0;
        }
      ঘূৰাই pathfd;
    }

  ret = fstat (pathfd, &st);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "stat `%s`", KRUN_VM_FILE);

  যদি (! S_ISREG (st.st_mode))
    ঘূৰাই crun_make_error (err, 0, "`%s` is not a regular file", KRUN_VM_FILE);

  get_proc_self_fd_path (fd_path, pathfd);

  fd = open (fd_path, O_RDONLY | O_CLOEXEC);
  যদি (UNLIKELY (fd < 0))
    ঘূৰাই crun_make_error (err, errno, "open `%s`", KRUN_VM_FILE);

  ret = read_all_fd_with_max_size (fd, KRUN_VM_FILE, &config, রিক্ত, KRUN_VM_FILE_MAX_SIZE, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = parse_json_file (&kconf->config_doc, config, রিক্ত, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  kconf->config_বৃক্ষ = kconf->config_doc;
  ঘূৰাই 0;
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
স্থির পূর্ণাঙ্ক
libkrun_parse_resource_configuration (json_object *config_বৃক্ষ, libcrun_container_t *container, ধ্রুৱ ৱর্ণ *annotation, ধ্রুৱ ৱর্ণ *চাবি, bool use_config_file)
{
  ৱর্ণ *val_str, *endptr;
  পূর্ণাঙ্ক val = -1;
  json_object *val_json = রিক্ত;

  val_str = (ৱর্ণ *) find_annotation (container, annotation);
  যদি (val_str != রিক্ত)
    {
      errno = 0;
      val = (পূর্ণাঙ্ক) strtol (val_str, &endptr, 10);
      যদি (errno != 0 || endptr == val_str || *endptr != '\0')
        /* Annotations value is not a valid integer. */
        error (EXIT_FAILURE, 0, "krun annotation %s value cannot be converted to an integer", annotation);

      যদি (val < 0)
        error (EXIT_FAILURE, 0, "krun annotation %s value must be a positive integer", annotation);

      ঘূৰাই val;
    }
  অন্যথা যদি (use_config_file && config_বৃক্ষ != রিক্ত)
    {
      val_json = json_object_object_get (config_বৃক্ষ, চাবি);
      যদি (val_json == রিক্ত)
        ঘূৰাই val;
      যদি (! json_object_is_type (val_json, json_type_int))
        error (EXIT_FAILURE, 0, "krun krun_vm.json %s value is not an integer", চাবি);

      val = (পূর্ণাঙ্ক) json_object_get_int64 (val_json);
    }

  ঘূৰাই val;
}

স্থির পূর্ণাঙ্ক
libkrun_parse_string_configuration (json_object *config_বৃক্ষ, libcrun_container_t *container,
                                    ধ্রুৱ ৱর্ণ *annotation, ধ্রুৱ ৱর্ণ *চাবি,
                                    ধ্রুৱ ৱর্ণ **মান, libcrun_error_t *err, bool use_config_file)
{
  ধ্রুৱ ৱর্ণ *val;
  json_object *val_json = রিক্ত;

  *মান = রিক্ত;

  val = find_annotation (container, annotation);
  যদি (val != রিক্ত)
    {
      *মান = val;
      ঘূৰাই 0;
    }

  যদি (! use_config_file || config_বৃক্ষ == রিক্ত)
    ঘূৰাই 0;

  val_json = json_object_object_get (config_বৃক্ষ, চাবি);
  যদি (val_json == রিক্ত)
    ঘূৰাই 0;

  যদি (! json_object_is_type (val_json, json_type_string))
    ঘূৰাই crun_make_error (err, 0, ".krun_vm.json %s value is not a string", চাবি);

  *মান = json_object_get_string (val_json);
  ঘূৰাই 0;
}

স্থির শূন্য
libkrun_make_tap_mac (ধ্রুৱ ৱর্ণ *id, uint8_t mac[6])
{
  uint64_t hash = 14695981039346656037ULL;
  size_t i;

  বাবে (; *id != '\0'; id++)
    {
      hash ^= (uint8_t) *id;
      hash *= 1099511628211ULL;
    }

  বাবে (i = 0; i < 6; i++)
    mac[i] = (uint8_t) (hash >> (i * 8));

  mac[0] = (mac[0] & 0xfe) | 0x02;
}

স্থির পূর্ণাঙ্ক
libkrun_configure_vm (uint32_t ctx_id, শূন্য *handle, সংরচনা krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_set_vm_config) (uint32_t ctx_id, uint8_t অঙ্ক_vcpus, uint32_t ram_mib);
  int32_t (*krun_add_net_unixstream) (uint32_t ctx_id, ধ্রুৱ ৱর্ণ *c_path, পূর্ণাঙ্ক fd, uint8_t *ধ্রুৱ c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_add_net_tap) (uint32_t ctx_id, ধ্রুৱ ৱর্ণ *c_tap_name, ধ্রুৱ uint8_t *c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_set_gpu_options) (uint32_t ctx_id, uint32_t virgl_flags);
  int32_t (*krun_set_gpu_render_server_fd) (uint32_t ctx_id, পূর্ণাঙ্ক fd);
  পূর্ণাঙ্ক cpus, ram_mib, nested_virt, ret;
  cpu_set_t set;

  /* We let the OCI set the number of vCPUs for the VM, since cgroups CPU restrictions still apply. */
  cpus = libkrun_parse_resource_configuration (kconf->config_বৃক্ষ, container, "krun.cpus", "cpus", true);
  যদি (cpus <= 0)
    {
      CPU_ZERO (&set);
      যদি (sched_getaffinity (getpid (), পরিমাণ (set), &set) == 0)
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

  যদি (krun_set_vm_config == রিক্ত)
    ঘূৰাই crun_make_error (err, 0, "could not find symbol in the krun library");

  ret = krun_set_vm_config (ctx_id, cpus, ram_mib);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, -ret, "could not set krun vm configuration");

  nested_virt = libkrun_parse_resource_configuration (kconf->config_বৃক্ষ, container, "krun.nested_virt", "nested_virt", false);
  যদি (nested_virt > 0)
    {
      int32_t (*krun_check_nested_virt) (শূন্য);
      int32_t (*krun_set_nested_virt) (uint32_t ctx_id, bool enabled);

      krun_check_nested_virt = dlsym (handle, "krun_check_nested_virt");
      যদি (krun_check_nested_virt != রিক্ত && krun_check_nested_virt () != 1)
        libcrun_warning ("nested virtualization requested but may not be supported on this host");

      krun_set_nested_virt = dlsym (handle, "krun_set_nested_virt");
      যদি (krun_set_nested_virt == রিক্ত)
        ঘূৰাই crun_make_error (err, 0, "could not find symbol `krun_set_nested_virt` in the krun library");

      ret = krun_set_nested_virt (ctx_id, true);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, -ret, "could not enable nested virtualization");
    }

  যদি (kconf->tap_name != রিক্ত)
    {
      krun_add_net_tap = dlsym (handle, "krun_add_net_tap");
      যদি (krun_add_net_tap == রিক্ত)
        ঘূৰাই crun_make_error (err, 0, "could not find symbol `krun_add_net_tap` in the krun library");

      uint8_t mac[6];
      libkrun_make_tap_mac (container->context->id, mac);
      ret = krun_add_net_tap (ctx_id, kconf->tap_name, &mac[0], COMPAT_NET_FEATURES, 0);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, -ret, "could not add krun TAP interface `%s`", kconf->tap_name);
    }
  অন্যথা যদি (kconf->use_passt)
    {
      krun_add_net_unixstream = dlsym (handle, "krun_add_net_unixstream");
      যদি (krun_add_net_unixstream == রিক্ত)
        ঘূৰাই crun_make_error (err, 0, "could not find symbol `krun_add_net_unixstream` in the krun library");

      uint8_t mac[] = { 0x5a, 0x94, 0xef, 0xe4, 0x0c, 0xee };
      ret = krun_add_net_unixstream (ctx_id, রিক্ত, kconf->passt_fds[FD_PAIR_PARENT], &mac[0], COMPAT_NET_FEATURES, NET_FLAG_DHCP_CLIENT);
      যদি (UNLIKELY (ret == -EINVAL))
        ret = krun_add_net_unixstream (ctx_id, রিক্ত, kconf->passt_fds[FD_PAIR_PARENT], &mac[0], COMPAT_NET_FEATURES, 0);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, -ret, "could not set krun net configuration");
    }

  যদি (kconf->gpu_flags > 0)
    {
      krun_set_gpu_options = dlsym (kconf->handle, "krun_set_gpu_options");
      যদি (krun_set_gpu_options == রিক্ত)
        ঘূৰাই crun_make_error (err, 0, "gpu requested but the version of libkrun in this system does not support it");

      ret = krun_set_gpu_options (kconf->ctx_id, kconf->gpu_flags);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, -ret, "gpu requested but could not configure virtio gpu device");

      যদি (kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER)
        {
          krun_set_gpu_render_server_fd = dlsym (kconf->handle, "krun_set_gpu_render_server_fd");
          যদি (krun_set_gpu_render_server_fd == রিক্ত)
            ঘূৰাই crun_make_error (err, 0, "gpu with render server requested but the version of libkrun in this system does not support it");

          ret = krun_set_gpu_render_server_fd (kconf->ctx_id, kconf->gpu_render_server_fd);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, -ret, "gpu with render server requested but could not configure its file descriptor");
        }
    }

  যদি (kconf->config_বৃক্ষ != রিক্ত)
    {
      /* Try to configure an external kernel. If the configuration file doesn't
       * specify a kernel, libkrun automatically fall back to using libkrunfw,
       * if the library is present and was loaded while creating the context.
       */
      পূর্ণাঙ্ক custom_kernel = libkrun_parse_resource_configuration (kconf->config_বৃক্ষ, container, "krun.custom_kernel", "custom_kernel", false);
      যদি (custom_kernel > 0)
        {
          ret = libkrun_configure_kernel (ctx_id, handle, kconf->config_বৃক্ষ, err);
          যদি (UNLIKELY (ret))
            ঘূৰাই ret;
        }
    }

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
libkrun_configure_flavor (শূন্য *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret, sev_indicated = 0, awsnitro_indicated = 0;
  সংরচনা krun_config *kconf = (সংরচনা krun_config *) cookie;
  ধ্রুৱ ৱর্ণ *flavor = রিক্ত;
  শূন্য *close_handles[2];

  close_handles[0] = রিক্ত;
  close_handles[1] = রিক্ত;

  // Check if the user provided the krun variant through OCI annotations.
  flavor = find_annotation (container, "krun.variant");
  যদি (flavor != রিক্ত)
    {
      sev_indicated |= strcmp (flavor, KRUN_FLAVOR_SEV) == 0;
      awsnitro_indicated |= strcmp (flavor, KRUN_FLAVOR_AWS_NITRO) == 0;
    }

  যদি (sev_indicated)
    {
      যদি (kconf->handle_sev == রিক্ত)
        error (EXIT_FAILURE, 0, "the container requires libkrun-sev but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_awsnitro;

      kconf->handle = kconf->handle_sev;
      kconf->ctx_id = kconf->ctx_id_sev;
      kconf->sev = true;
    }
  অন্যথা যদি (awsnitro_indicated)
    {
      যদি (kconf->handle_awsnitro == রিক্ত)
        error (EXIT_FAILURE, 0, "the container requires libkrun-awsnitro but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_sev;

      kconf->handle = kconf->handle_awsnitro;
      kconf->ctx_id = kconf->ctx_id_awsnitro;
      kconf->awsnitro = true;
    }
  অন্যথা
    {
      যদি (kconf->handle == রিক্ত)
        error (EXIT_FAILURE, 0, "the container requires libkrun but it's not available");

      close_handles[0] = kconf->handle_sev;
      close_handles[1] = kconf->handle_awsnitro;
    }

  // We no longer need the other two libkrun handles.
  বাবে (পূর্ণাঙ্ক i = 0; i < 2; i++)
    {
      যদি (close_handles[i] == রিক্ত)
        অনুৱর্ত;

      ret = dlclose (close_handles[i]);
      যদি (UNLIKELY (ret != 0))
        ঘূৰাই crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
libkrun_exec (শূন্য *cookie, libcrun_container_t *container, ধ্রুৱ ৱর্ণ *pathname, ৱর্ণ *ধ্রুৱ argv[])
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_set_log_level) (uint32_t level);
  পূর্ণাঙ্ক (*krun_start_enter) (uint32_t ctx_id);
  int32_t (*krun_add_virtiofs2) (uint32_t ctx_id, ধ্রুৱ ৱর্ণ *c_tag, ধ্রুৱ ৱর্ণ *c_path, uint64_t shm_size);
  int32_t (*krun_set_মূল_disk) (uint32_t ctx_id, ধ্রুৱ ৱর্ণ *disk_path);
  int32_t (*krun_set_tee_config_file) (uint32_t ctx_id, ধ্রুৱ ৱর্ণ *file_path);
  int32_t (*krun_set_console_output) (uint32_t ctx_id, ধ্রুৱ ৱর্ণ *c_filepath);
  int32_t (*krun_set_exec) (uint32_t ctx_id, ধ্রুৱ ৱর্ণ *exec_path,
                            ধ্রুৱ ৱর্ণ *ধ্রুৱ argv[], ধ্রুৱ ৱর্ণ *ধ্রুৱ envp[]);
  সংরচনা krun_config *kconf = (সংরচনা krun_config *) cookie;
  শূন্য *handle;
  int32_t ctx_id, ret;
  libcrun_error_t err;

  ret = libkrun_configure_flavor (cookie, container, &err);
  যদি (UNLIKELY (ret < 0))
    {
      পূর্ণাঙ্ক errcode = crun_error_get_errno (&err);
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
  যদি (krun_set_log_level == রিক্ত || krun_start_enter == রিক্ত)
    error (EXIT_FAILURE, 0, "could not find symbol in the krun library");

  /* Set log level according to crun's verbosity. */
  ৱিকল্প (libcrun_get_verbosity ())
    {
    পক্ষ LIBCRUN_VERBOSITY_DEBUG:
      krun_set_log_level (KRUN_LOG_LEVEL_DEBUG);
      ভঙ্গ;
    পক্ষ LIBCRUN_VERBOSITY_WARNING:
      krun_set_log_level (KRUN_LOG_LEVEL_WARN);
      ভঙ্গ;
    স্ৱভাৱ:
      krun_set_log_level (KRUN_LOG_LEVEL_ERROR);
      ভঙ্গ;
    }

  যদি (kconf->sev)
    {
      krun_set_মূল_disk = dlsym (handle, "krun_set_root_disk");
      krun_set_tee_config_file = dlsym (handle, "krun_set_tee_config_file");
      যদি (krun_set_মূল_disk == রিক্ত || krun_set_tee_config_file == রিক্ত)
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
      json_object *val_virtiofs_tag = রিক্ত;
      json_object *val_virtiofs_shm_size = রিক্ত;
      ধ্রুৱ ৱর্ণ *virtiofs_tag = রিক্ত;
      // Default to a conservative DAX size of 512MB, just like krun_set_root() does.
      uint64_t virtiofs_shm_size = 512 * 1024 * 1024ULL;

      যদি (kconf->config_বৃক্ষ != রিক্ত)
        {
          val_virtiofs_tag = json_object_object_get (kconf->config_বৃক্ষ, "virtiofs_tag");
          যদি (val_virtiofs_tag != রিক্ত && json_object_is_type (val_virtiofs_tag, json_type_string))
            virtiofs_tag = json_object_get_string (val_virtiofs_tag);

          val_virtiofs_shm_size = json_object_object_get (kconf->config_বৃক্ষ, "virtiofs_shm_size");
          যদি (val_virtiofs_shm_size != রিক্ত && json_object_is_type (val_virtiofs_shm_size, json_type_int))
            virtiofs_shm_size = json_object_get_uint64 (val_virtiofs_shm_size);
        }

      যদি (virtiofs_tag == রিক্ত)
        virtiofs_tag = "/dev/root";

      krun_add_virtiofs2 = dlsym (handle, "krun_add_virtiofs2");

      যদি (krun_add_virtiofs2 == রিক্ত)
        error (EXIT_FAILURE, 0, "could not find symbol `krun_add_virtiofs2` in `libkrun.so`");

      ret = krun_add_virtiofs2 (ctx_id, virtiofs_tag, "/", virtiofs_shm_size);
      যদি (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not add virtiofs root with tag `%s`", virtiofs_tag);
    }

  যদি (kconf->awsnitro)
    {
      krun_set_console_output = dlsym (handle, "krun_set_console_output");
      krun_set_exec = dlsym (handle, "krun_set_exec");
      যদি (krun_set_console_output == রিক্ত || krun_set_exec == রিক্ত)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-awsnitro.so`");

      // Redirect all enclave output (read from vsock) to stdout.
      ret = krun_set_console_output (ctx_id, "/dev/stdout");
      যদি (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not redirect enclave output to stdout");

      ret = krun_set_exec (ctx_id, pathname, (ধ্রুৱ ৱর্ণ *ধ্রুৱ *) argv,
                           (ধ্রুৱ ৱর্ণ *ধ্রুৱ *) def->process->env);
      যদি (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set enclave execution arguments");
    }

  ret = libkrun_configure_vm (ctx_id, handle, kconf, container, &err);
  যদি (UNLIKELY (ret))
    {
      পূর্ণাঙ্ক errcode = crun_error_get_errno (&err);
      libcrun_error_t *tmp_err = &err;
      libcrun_error_report_and_release (tmp_err);
      error (EXIT_FAILURE, errcode, "could not configure krun vm");
    }

  json_object_put (kconf->config_doc);

  ret = krun_start_enter (ctx_id);
  যদি (UNLIKELY (ret < 0))
    error (EXIT_FAILURE, -ret, "could not start krun");

  ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক
libkrun_start_render_server (সংরচনা krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  kconf->gpu_flags = libkrun_parse_resource_configuration (kconf->config_বৃক্ষ, container, "krun.gpu_flags", "gpu_flags", false);
  যদি (kconf->gpu_flags > 0)
    {
      যদি (access ("/dev/dri", F_OK) != 0)
        ঘূৰাই crun_make_error (err, errno, "gpu requested but /dev/dri is not available");

      যদি ((kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER) != 0)
        {
          ৱর্ণ *rs_argv[40];
          পূর্ণাঙ্ক rs_argc = 0;
          ৱর্ণ fd_as_str[16];
          পূর্ণাঙ্ক gpu_fds[2];
          pid_t pid;
          পূর্ণাঙ্ক null;

          যদি (access ("/usr/libexec/virgl_render_server", X_OK) != 0)
            ঘূৰাই crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/libexec/virgl_render_server binary");

          যদি (access ("/usr/bin/bwrap", X_OK) != 0)
            ঘূৰাই crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/bin/bwrap binary");

          ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, gpu_fds);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, errno, "could not create socketpair for virgl_render_server");

          snprintf (fd_as_str, পরিমাণ (fd_as_str), "%d", gpu_fds[FD_PAIR_CHILD]);

          rs_argv[rs_argc++] = (ৱর্ণ *) "bwrap";
          rs_argv[rs_argc++] = (ৱর্ণ *) "--ro-bind";
          rs_argv[rs_argc++] = (ৱর্ণ *) "/usr";
          rs_argv[rs_argc++] = (ৱর্ণ *) "/usr";
          যদি (access ("/lib", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (ৱর্ণ *) "--ro-bind";
              rs_argv[rs_argc++] = (ৱর্ণ *) "/lib";
              rs_argv[rs_argc++] = (ৱর্ণ *) "/lib";
            }
          যদি (access ("/lib64", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (ৱর্ণ *) "--ro-bind";
              rs_argv[rs_argc++] = (ৱর্ণ *) "/lib64";
              rs_argv[rs_argc++] = (ৱর্ণ *) "/lib64";
            }
          rs_argv[rs_argc++] = (ৱর্ণ *) "--ro-bind";
          rs_argv[rs_argc++] = (ৱর্ণ *) "/etc";
          rs_argv[rs_argc++] = (ৱর্ণ *) "/etc";
          rs_argv[rs_argc++] = (ৱর্ণ *) "--ro-bind";
          rs_argv[rs_argc++] = (ৱর্ণ *) "/sys";
          rs_argv[rs_argc++] = (ৱর্ণ *) "/sys";
          rs_argv[rs_argc++] = (ৱর্ণ *) "--dev";
          rs_argv[rs_argc++] = (ৱর্ণ *) "/dev";
          rs_argv[rs_argc++] = (ৱর্ণ *) "--dev-bind";
          rs_argv[rs_argc++] = (ৱর্ণ *) "/dev/dri";
          rs_argv[rs_argc++] = (ৱর্ণ *) "/dev/dri";
          rs_argv[rs_argc++] = (ৱর্ণ *) "--proc";
          rs_argv[rs_argc++] = (ৱর্ণ *) "/proc";
          rs_argv[rs_argc++] = (ৱর্ণ *) "--tmpfs";
          rs_argv[rs_argc++] = (ৱর্ণ *) "/tmp";
          rs_argv[rs_argc++] = (ৱর্ণ *) "--unshare-pid";
          rs_argv[rs_argc++] = (ৱর্ণ *) "--unshare-ipc";
          rs_argv[rs_argc++] = (ৱর্ণ *) "--unshare-net";
          rs_argv[rs_argc++] = (ৱর্ণ *) "--";
          rs_argv[rs_argc++] = (ৱর্ণ *) "/usr/libexec/virgl_render_server";
          rs_argv[rs_argc++] = (ৱর্ণ *) "--socket-fd";
          rs_argv[rs_argc++] = fd_as_str;
          rs_argv[rs_argc] = রিক্ত;

          pid = fork ();
          যদি (pid < 0)
            {
              close (gpu_fds[FD_PAIR_PARENT]);
              close (gpu_fds[FD_PAIR_CHILD]);
              ঘূৰাই crun_make_error (err, errno, "could not fork for virgl_render_server");
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

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
libkrun_configure_network (শূন্য *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  সংরচনা krun_config *kconf = (সংরচনা krun_config *) cookie;
  pid_t pid;
  ৱর্ণ *passt_argv[10];
  ৱর্ণ fd_as_str[16];
  পূর্ণাঙ্ক use_passt;
  পূর্ণাঙ্ক argv_idx;
  পূর্ণাঙ্ক status;
  পূর্ণাঙ্ক null;
  পূর্ণাঙ্ক ret;

  ret = libkrun_parse_string_configuration (kconf->config_বৃক্ষ, container,
                                            "krun.tap_name", "tap_name",
                                            &kconf->tap_name, err, false);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  যদি (kconf->tap_name != রিক্ত && is_empty_string (kconf->tap_name))
    ঘূৰাই crun_make_error (err, 0, "krun.tap_name cannot be empty");

  use_passt = libkrun_parse_resource_configuration (kconf->config_বৃক্ষ, container, "krun.use_passt", "use_passt", false);

  যদি (use_passt > 0)
    {
      যদি (kconf->tap_name != রিক্ত)
        ঘূৰাই crun_make_error (err, 0, "krun.tap_name and krun.use_passt are mutually exclusive");

      kconf->use_passt = 1;
    }
  অন্যথা
    ঘূৰাই 0;

  ret = socketpair (AF_UNIX, SOCK_STREAM, 0, kconf->passt_fds);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "create passt socketpair");

  snprintf (fd_as_str, পরিমাণ (fd_as_str), "%d", kconf->passt_fds[FD_PAIR_CHILD]);

  argv_idx = 0;
  passt_argv[argv_idx++] = (ৱর্ণ *) "passt";
  passt_argv[argv_idx++] = (ৱর্ণ *) "-t";
  passt_argv[argv_idx++] = (ৱর্ণ *) "all";

  যদি (! kconf->has_awsnitro)
    {
      passt_argv[argv_idx++] = (ৱর্ণ *) "-u";
      passt_argv[argv_idx++] = (ৱর্ণ *) "all";
      passt_argv[argv_idx++] = (ৱর্ণ *) "--no-dhcp-dns";
    }

  /* Set --no-map-gw. If the gateway for the network is also the DNS server
   * we won't be able to query the DNS server without --no-map-gw.
   * See discussion in https://github.com/containers/crun/pull/2099
   */
  passt_argv[argv_idx++] = (ৱর্ণ *) "--no-map-gw";

  passt_argv[argv_idx++] = (ৱর্ণ *) "--fd";
  passt_argv[argv_idx++] = fd_as_str;
  passt_argv[argv_idx] = রিক্ত;

  pid = fork ();
  যদি (pid < 0)
    ঘূৰাই crun_make_error (err, errno, "fork passt");
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
      বাবে (i = 0; i < def->linux->namespaces_len; i++)
        {
          পূর্ণাঙ্ক মান = libcrun_find_namespace (def->linux->namespaces[i]->type);

          যদি (মান == CLONE_NEWPID)
            {
              যদি (mount ("", "/proc", "proc", MS_NOSUID | MS_NODEV | MS_NOEXEC, রিক্ত))
                _exit (EXIT_FAILURE);

              ভঙ্গ;
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
    ঘূৰাই crun_make_error (err, 0, "start passt");

  ঘূৰাই 0;
}

/* Return true if the spec already declares a device with the given PATH.  */
স্থির bool
spec_has_device (runtime_spec_schema_config_schema *def, ধ্রুৱ ৱর্ণ *path)
{
  size_t i;

  বাবে (i = 0; i < def->linux->devices_len; i++)
    যদি (strcmp (def->linux->devices[i]->path, path) == 0)
      ঘূৰাই true;
  ঘূৰাই false;
}

/* libkrun_create_kvm_device: explicitly adds kvm device.  */
স্থির পূর্ণাঙ্ক
libkrun_configure_container (শূন্য *cookie, গণনা handler_configure_phase phase,
                             libcrun_context_t *context, libcrun_container_t *container,
                             ধ্রুৱ ৱর্ণ *rootfs, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret, rootfsfd;
  সংরচনা krun_config *kconf = (সংরচনা krun_config *) cookie;
  সংরচনা device_s kvm_device = { "/dev/kvm", "c", 10, 232, 0666, 0, 0 };
  সংরচনা device_s sev_device = { "/dev/sev", "c", 10, 124, 0666, 0, 0 };
  সংরচনা device_s nitro_device = { "/dev/nitro_enclaves", "c", 10, 122, 0666, 0, 0 };
  cleanup_close পূর্ণাঙ্ক devfd = -1;
  cleanup_close পূর্ণাঙ্ক rootfsfd_cleanup = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  bool create_sev = false, create_awsnitro = false;
  bool is_user_ns;

  যদি (rootfs == রিক্ত)
    rootfsfd = AT_FDCWD;
  অন্যথা
    {
      rootfsfd = rootfsfd_cleanup = open (rootfs, O_PATH | O_CLOEXEC);
      যদি (UNLIKELY (rootfsfd < 0))
        ঘূৰাই crun_make_error (err, errno, "open `%s`", rootfs);
    }

  যদি (phase == HANDLER_CONFIGURE_BEFORE_USERNS)
    {
      cleanup_free ৱর্ণ *origin_config_path = রিক্ত;
      cleanup_free ৱর্ণ *state_dir = রিক্ত;
      cleanup_free ৱর্ণ *config = রিক্ত;
      cleanup_close পূর্ণাঙ্ক fd = -1;
      size_t config_size;

      ret = libcrun_get_state_directory (&state_dir, context->state_মূল, context->id, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = append_paths (&origin_config_path, err, state_dir, "config.json", রিক্ত);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = read_all_file (origin_config_path, &config, &config_size, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      /* CVE-2025-24965: the content below rootfs cannot be trusted because it is controlled by the user.  We
         must ensure the file is opened below the rootfs directory.  Whatever the image ships at that name
         could also be a device node or a FIFO, so never open it: unlink it and create a new regular file.  */
      ret = unlinkat (rootfsfd, KRUN_CONFIG_FILE, 0);
      যদি (UNLIKELY (ret < 0 && errno != ENOENT))
        ঘূৰাই crun_make_error (err, errno, "unlink `%s`", KRUN_CONFIG_FILE);

      fd = safe_openat (rootfsfd, rootfs, KRUN_CONFIG_FILE, WRITE_FILE_DEFAULT_FLAGS | O_EXCL | O_NOFOLLOW, S_IRUSR | S_IRGRP | S_IROTH, err);
      যদি (UNLIKELY (fd < 0))
        ঘূৰাই fd;

      ret = safe_write (fd, KRUN_CONFIG_FILE, config, config_size, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = libkrun_read_vm_config (kconf, rootfsfd, rootfs, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = libkrun_start_render_server (kconf, container, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  যদি (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    ঘূৰাই 0;

  ret = libkrun_configure_network (cookie, container, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  /* Do nothing if /dev/kvm is already present in spec */
  যদি (spec_has_device (def, "/dev/kvm"))
    ঘূৰাই 0;

  যদি (kconf->handle_sev != রিক্ত)
    create_sev = ! spec_has_device (def, "/dev/sev");

  যদি (kconf->handle_awsnitro != রিক্ত)
    create_awsnitro = ! spec_has_device (def, "/dev/nitro_enclaves");

  devfd = safe_openat (rootfsfd, rootfs, "dev", O_PATH | O_DIRECTORY | O_CLOEXEC, 0, err);
  যদি (UNLIKELY (devfd < 0))
    ঘূৰাই devfd;

  ret = check_running_in_user_namespace (err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;
  is_user_ns = ret;

  যদি (kconf->has_kvm)
    {
      ret = libcrun_create_dev (container, devfd, -1, &kvm_device, is_user_ns, true, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  যদি (create_sev)
    {
      ret = libcrun_create_dev (container, devfd, -1, &sev_device, is_user_ns, true, err);
      যদি (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_sev);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
          kconf->handle_sev = রিক্ত;
        }
    }

  যদি (create_awsnitro)
    {
      ret = libcrun_create_dev (container, devfd, -1, &nitro_device, is_user_ns, true, err);
      যদি (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_awsnitro);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
          kconf->handle_awsnitro = রিক্ত;
        }
    }

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
libkrun_load (শূন্য **cookie, libcrun_error_t *err)
{
  int32_t ret;
  সংরচনা krun_config *kconf;
  ধ্রুৱ ৱর্ণ *libkrun_so = "libkrun.so.1";
  ধ্রুৱ ৱর্ণ *libkrun_sev_so = "libkrun-sev.so.1";
  ধ্রুৱ ৱর্ণ *libkrun_awsnitro_so = "libkrun-awsnitro.so.1";

  kconf = malloc (পরিমাণ (সংরচনা krun_config));
  যদি (kconf == রিক্ত)
    ঘূৰাই crun_make_error (err, 0, "could not allocate memory for krun_config");
  memset (kconf, 0, পরিমাণ (সংরচনা krun_config));

  kconf->gpu_render_server_fd = -1;

  kconf->handle = dlopen (libkrun_so, RTLD_NOW);
  kconf->handle_sev = dlopen (libkrun_sev_so, RTLD_NOW);
  kconf->handle_awsnitro = dlopen (libkrun_awsnitro_so, RTLD_NOW);

  যদি (kconf->handle == রিক্ত && kconf->handle_sev == রিক্ত && kconf->handle_awsnitro == রিক্ত)
    {
      মুক্ত (kconf);
      ঘূৰাই crun_make_error (err, 0, "failed to open `%s`, `%s`, and `%s` for krun_config: %s", libkrun_so, libkrun_sev_so, libkrun_awsnitro_so, dlerror ());
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
        গচ্ছ error;
      kconf->ctx_id = ret;
    }

  যদি (kconf->handle_sev)
    {
      ret = libkrun_create_context (kconf->handle_sev, err);
      যদি (UNLIKELY (ret < 0))
        গচ্ছ error;
      kconf->ctx_id_sev = ret;
    }
  যদি (kconf->handle_awsnitro)
    {
      ret = libkrun_create_context (kconf->handle_awsnitro, err);
      যদি (UNLIKELY (ret < 0))
        গচ্ছ error;
      kconf->ctx_id_awsnitro = ret;
    }

  *cookie = kconf;

  ঘূৰাই 0;

error:
  যদি (kconf->handle)
    dlclose (kconf->handle);
  যদি (kconf->handle_sev)
    dlclose (kconf->handle_sev);
  যদি (kconf->handle_awsnitro)
    dlclose (kconf->handle_awsnitro);
  মুক্ত (kconf);
  ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক
libkrun_unload (শূন্য *cookie, libcrun_error_t *err)
{
  পূর্ণাঙ্ক r;

  সংরচনা krun_config *kconf = (সংরচনা krun_config *) cookie;
  যদি (kconf != রিক্ত)
    {
      যদি (kconf->handle != রিক্ত)
        {
          r = dlclose (kconf->handle);
          যদি (UNLIKELY (r != 0))
            ঘূৰাই crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
        }
      যদি (kconf->handle_sev != রিক্ত)
        {
          r = dlclose (kconf->handle_sev);
          যদি (UNLIKELY (r != 0))
            ঘূৰাই crun_make_error (err, 0, "could not unload handle_sev: `%s`", dlerror ());
        }
      যদি (kconf->handle_awsnitro != রিক্ত)
        {
          r = dlclose (kconf->handle_awsnitro);
          যদি (UNLIKELY (r != 0))
            ঘূৰাই crun_make_error (err, 0, "could not unload handle_awsnitro: `%s`", dlerror ());
        }
      মুক্ত (kconf);
    }
  ঘূৰাই 0;
}

স্থির runtime_spec_schema_defs_linux_device_cgroup *
make_oci_spec_dev (ধ্রুৱ ৱর্ণ *type, dev_t device, bool allow, ধ্রুৱ ৱর্ণ *access)
{
  runtime_spec_schema_defs_linux_device_cgroup *dev = xmalloc0 (পরিমাণ (*dev));

  dev->allow = allow;
  dev->allow_present = 1;

  dev->type = xstrdup (type);

  dev->major = major (device);
  dev->major_present = 1;

  dev->minor = minor (device);
  dev->minor_present = 1;

  dev->access = xstrdup (access);

  ঘূৰাই dev;
}

/* stat an optional device: on ENOENT clear *PRESENT, any other error fails.  */
স্থির পূর্ণাঙ্ক
stat_optional_device (ধ্রুৱ ৱর্ণ *path, সংরচনা stat *st, bool *present, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret = stat (path, st);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (errno != ENOENT)
        ঘূৰাই crun_make_error (err, errno, "stat `%s`", path);
      *present = false;
    }
  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
libkrun_modify_oci_configuration (শূন্য *cookie arg_unused, libcrun_context_t *context arg_unused,
                                  runtime_spec_schema_config_schema *def,
                                  libcrun_error_t *err)
{
  ধ্রুৱ size_t device_size = পরিমাণ (runtime_spec_schema_defs_linux_device_cgroup);
  সংরচনা krun_config *kconf = (সংরচনা krun_config *) cookie;
  সংরচনা stat st_kvm, st_sev, st_awsnitro;
  bool has_kvm = true, has_sev = true, has_awsnitro = true;
  size_t old_len, new_len;
  পূর্ণাঙ্ক ret;

  /* Always allow the /dev/kvm device.  */

  ret = stat_optional_device ("/dev/kvm", &st_kvm, &has_kvm, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = stat_optional_device ("/dev/sev", &st_sev, &has_sev, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = stat_optional_device ("/dev/nitro_enclaves", &st_awsnitro, &has_awsnitro, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  kconf->has_kvm = has_kvm;
  kconf->has_awsnitro = has_awsnitro;

  যদি (! has_kvm && ! has_awsnitro)
    ঘূৰাই 0;

  /* spec says these are optional, ensure they exist so we can add our devices */
  যদি (def->linux == রিক্ত)
    def->linux = xmalloc0 (পরিমাণ (runtime_spec_schema_config_linux));

  যদি (def->linux->resources == রিক্ত)
    def->linux->resources = xmalloc0 (পরিমাণ (runtime_spec_schema_config_linux_resources));

  old_len = def->linux->resources->devices_len;
  new_len = old_len;
  যদি (has_kvm)
    new_len += has_sev ? 2 : 1;
  যদি (has_awsnitro)
    new_len += 1;

  def->linux->resources->devices = xrealloc (def->linux->resources->devices, device_size * (new_len + 1));
  def->linux->resources->devices_len = new_len;

  যদি (has_kvm)
    {
      def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_kvm.st_rdev, true, "rwm");
      যদি (has_sev)
        def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_sev.st_rdev, true, "rwm");
    }

  যদি (has_awsnitro)
    def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_awsnitro.st_rdev, true, "rwm");

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
libkrun_close_fds (শূন্য *cookie, libcrun_container_t *container, পূর্ণাঙ্ক preserve_fds, libcrun_error_t *err)
{
  সংরচনা krun_config *kconf = (সংরচনা krun_config *) cookie;
  পূর্ণাঙ্ক first_fd_to_close = preserve_fds + 3;
  পূর্ণাঙ্ক নিচেৰ_fd, উপৰেৰ_fd;
  পূর্ণাঙ্ক i;

  নিচেৰ_fd = উপৰেৰ_fd = -1;

  যদি (kconf->use_passt)
    উপৰেৰ_fd = kconf->passt_fds[FD_PAIR_PARENT];

  যদি (kconf->gpu_render_server_fd != -1)
    {
      যদি (kconf->gpu_render_server_fd > উপৰেৰ_fd)
        {
          নিচেৰ_fd = উপৰেৰ_fd;
          উপৰেৰ_fd = kconf->gpu_render_server_fd;
        }
      অন্যথা
        নিচেৰ_fd = kconf->gpu_render_server_fd;
    }

  যদি (first_fd_to_close <= উপৰেৰ_fd)
    {
      বাবে (i = first_fd_to_close; i < উপৰেৰ_fd; i++)
        {
          // If we're closing proc_fd, make sure to invalidate it.
          যদি (i == container->proc_fd)
            container->proc_fd = -1;
          অন্যথা যদি (i == নিচেৰ_fd)
            অনুৱর্ত;
          close (i);
        }

      first_fd_to_close = উপৰেৰ_fd + 1;
    }

  ঘূৰাই mark_or_close_fds_ge_than (container, first_fd_to_close, true, err);
}

সংরচনা custom_handler_s handler_libkrun = {
  .name = "krun",
  .alias = রিক্ত,
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
