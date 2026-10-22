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

#अन्तर्भाव <config.h>
#अन्तर्भाव "../custom-handler.h"
#अन्तर्भाव "../container.h"
#अन्तर्भाव "../utils.h"
#अन्तर्भाव "../linux.h"
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/socket.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <sys/mount.h>
#अन्तर्भाव <sys/param.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sys/sysmacros.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <sched.h>
#अन्तर्भाव <ocispec/runtime_spec_schema_config_schema.h>

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

संरचना krun_config
{
  शून्य *handle;
  शून्य *handle_sev;
  शून्य *handle_awsnitro;
  bool sev;
  bool awsnitro;
  int32_t ctx_id;
  int32_t ctx_id_sev;
  int32_t ctx_id_awsnitro;
  bool has_kvm;
  bool has_awsnitro;
  पूर्णांक passt_fds[2];
  json_object *config_doc;
  json_object *config_वृक्ष;
  bool use_passt;
  स्थिरांक अक्षर *tap_name;
  पूर्णांक gpu_render_server_fd;
  पूर्णांक gpu_flags;
};

/* libkrun handler.  */
#जर HAVE_DLOPEN && HAVE_LIBKRUN
स्थिर int32_t
libkrun_create_context (शून्य *handle, libcrun_error_t *err)
{
  int32_t (*krun_create_ctx) ();
  int32_t ctx_id;

  krun_create_ctx = dlsym (handle, "krun_create_ctx");
  जर (krun_create_ctx == रिक्त)
    परत crun_make_error (err, 0, "could not find symbol in the krun library");

  ctx_id = krun_create_ctx ();
  जर (UNLIKELY (ctx_id < 0))
    परत crun_make_error (err, -ctx_id, "could not create krun context");

  परत ctx_id;
}

स्थिर पूर्णांक
libkrun_configure_kernel (uint32_t ctx_id, शून्य *handle, json_object *config_वृक्ष, libcrun_error_t *err)
{
  int32_t (*krun_set_kernel) (uint32_t ctx_id, स्थिरांक अक्षर *kernel_path,
                              uint32_t kernel_format, स्थिरांक अक्षर *initrd_path, स्थिरांक अक्षर *kernel_cmdline);
  json_object *kernel_path = रिक्त;
  json_object *kernel_format = रिक्त;
  json_object *val_initrd_path = रिक्त;
  json_object *val_kernel_cmdline = रिक्त;
  स्थिरांक अक्षर *initrd_path = रिक्त;
  स्थिरांक अक्षर *kernel_cmdline = रिक्त;
  पूर्णांक ret;

  /* kernel_path and kernel_format must be present */
  kernel_path = json_object_object_get (config_वृक्ष, "kernel_path");
  जर (kernel_path == रिक्त || ! json_object_is_type (kernel_path, json_type_string))
    परत 0;

  kernel_format = json_object_object_get (config_वृक्ष, "kernel_format");
  जर (kernel_format == रिक्त || ! json_object_is_type (kernel_format, json_type_int))
    परत 0;

  /* initrd and kernel_cmdline are optional */
  val_initrd_path = json_object_object_get (config_वृक्ष, "initrd_path");
  जर (val_initrd_path != रिक्त && json_object_is_type (val_initrd_path, json_type_string))
    initrd_path = json_object_get_string (val_initrd_path);

  val_kernel_cmdline = json_object_object_get (config_वृक्ष, "kernel_cmdline");
  जर (val_kernel_cmdline != रिक्त && json_object_is_type (val_kernel_cmdline, json_type_string))
    kernel_cmdline = json_object_get_string (val_kernel_cmdline);

  krun_set_kernel = dlsym (handle, "krun_set_kernel");
  जर (krun_set_kernel == रिक्त)
    परत crun_make_error (err, 0, "could not find symbol in krun library");

  ret = krun_set_kernel (ctx_id,
                         json_object_get_string (kernel_path),
                         (uint32_t) json_object_get_int64 (kernel_format),
                         initrd_path, kernel_cmdline);

  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, -ret, "could not configure a krun external kernel");

  परत 0;
}

स्थिर पूर्णांक
libkrun_read_vm_config (संरचना krun_config *kconf, पूर्णांक rootfsfd, स्थिरांक अक्षर *rootfs, libcrun_error_t *err)
{
  पूर्णांक ret;
  cleanup_free अक्षर *config = रिक्त;
  cleanup_close पूर्णांक pathfd = -1;
  cleanup_close पूर्णांक fd = -1;
  proc_fd_path_t fd_path;
  संरचना stat st;

  /* The content of the rootfs is controlled by the user: look at the file through an
     O_PATH descriptor first and refuse anything that is not a small regular file, as
     opening a FIFO blocks and a device node must not be accessed.  */
  pathfd = safe_openat (rootfsfd, rootfs, KRUN_VM_FILE, O_PATH | O_CLOEXEC | O_NOFOLLOW, 0, err);
  जर (pathfd < 0)
    {
      // The configuration file is optional, don't generate an error if it's missing.
      जर (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          परत 0;
        }
      परत pathfd;
    }

  ret = ख_स्थिति (pathfd, &st);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "stat `%s`", KRUN_VM_FILE);

  जर (! S_ISREG (st.st_mode))
    परत crun_make_error (err, 0, "`%s` is not a regular file", KRUN_VM_FILE);

  get_proc_self_fd_path (fd_path, pathfd);

  fd = खोलो (fd_path, O_RDONLY | O_CLOEXEC);
  जर (UNLIKELY (fd < 0))
    परत crun_make_error (err, त्रुटि_सं, "open `%s`", KRUN_VM_FILE);

  ret = read_all_fd_with_max_size (fd, KRUN_VM_FILE, &config, रिक्त, KRUN_VM_FILE_MAX_SIZE, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = parse_json_file (&kconf->config_doc, config, रिक्त, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  kconf->config_वृक्ष = kconf->config_doc;
  परत 0;
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
स्थिर पूर्णांक
libkrun_parse_resource_configuration (json_object *config_वृक्ष, libcrun_container_t *container, स्थिरांक अक्षर *annotation, स्थिरांक अक्षर *किल्ली, bool use_config_file)
{
  अक्षर *val_str, *endptr;
  पूर्णांक val = -1;
  json_object *val_json = रिक्त;

  val_str = (अक्षर *) find_annotation (container, annotation);
  जर (val_str != रिक्त)
    {
      त्रुटि_सं = 0;
      val = (पूर्णांक) म_से_दीर्घ (val_str, &endptr, 10);
      जर (त्रुटि_सं != 0 || endptr == val_str || *endptr != '\0')
        /* Annotations value is not a valid integer. */
        error (निकास_त्रुटि, 0, "krun annotation %s value cannot be converted to an integer", annotation);

      जर (val < 0)
        error (निकास_त्रुटि, 0, "krun annotation %s value must be a positive integer", annotation);

      परत val;
    }
  अन्यथा जर (use_config_file && config_वृक्ष != रिक्त)
    {
      val_json = json_object_object_get (config_वृक्ष, किल्ली);
      जर (val_json == रिक्त)
        परत val;
      जर (! json_object_is_type (val_json, json_type_int))
        error (निकास_त्रुटि, 0, "krun krun_vm.json %s value is not an integer", किल्ली);

      val = (पूर्णांक) json_object_get_int64 (val_json);
    }

  परत val;
}

स्थिर पूर्णांक
libkrun_parse_string_configuration (json_object *config_वृक्ष, libcrun_container_t *container,
                                    स्थिरांक अक्षर *annotation, स्थिरांक अक्षर *किल्ली,
                                    स्थिरांक अक्षर **मूल्य, libcrun_error_t *err, bool use_config_file)
{
  स्थिरांक अक्षर *val;
  json_object *val_json = रिक्त;

  *मूल्य = रिक्त;

  val = find_annotation (container, annotation);
  जर (val != रिक्त)
    {
      *मूल्य = val;
      परत 0;
    }

  जर (! use_config_file || config_वृक्ष == रिक्त)
    परत 0;

  val_json = json_object_object_get (config_वृक्ष, किल्ली);
  जर (val_json == रिक्त)
    परत 0;

  जर (! json_object_is_type (val_json, json_type_string))
    परत crun_make_error (err, 0, ".krun_vm.json %s value is not a string", किल्ली);

  *मूल्य = json_object_get_string (val_json);
  परत 0;
}

स्थिर शून्य
libkrun_make_tap_mac (स्थिरांक अक्षर *id, uint8_t mac[6])
{
  uint64_t hash = 14695981039346656037ULL;
  माप_प्रकार i;

  साठी (; *id != '\0'; id++)
    {
      hash ^= (uint8_t) *id;
      hash *= 1099511628211ULL;
    }

  साठी (i = 0; i < 6; i++)
    mac[i] = (uint8_t) (hash >> (i * 8));

  mac[0] = (mac[0] & 0xfe) | 0x02;
}

स्थिर पूर्णांक
libkrun_configure_vm (uint32_t ctx_id, शून्य *handle, संरचना krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_set_vm_config) (uint32_t ctx_id, uint8_t अंक_vcpus, uint32_t ram_mib);
  int32_t (*krun_add_net_unixstream) (uint32_t ctx_id, स्थिरांक अक्षर *c_path, पूर्णांक fd, uint8_t *स्थिरांक c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_add_net_tap) (uint32_t ctx_id, स्थिरांक अक्षर *c_tap_name, स्थिरांक uint8_t *c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_set_gpu_options) (uint32_t ctx_id, uint32_t virgl_flags);
  int32_t (*krun_set_gpu_render_server_fd) (uint32_t ctx_id, पूर्णांक fd);
  पूर्णांक cpus, ram_mib, nested_virt, ret;
  cpu_set_t set;

  /* We let the OCI set the number of vCPUs for the VM, since cgroups CPU restrictions still apply. */
  cpus = libkrun_parse_resource_configuration (kconf->config_वृक्ष, container, "krun.cpus", "cpus", true);
  जर (cpus <= 0)
    {
      CPU_ZERO (&set);
      जर (sched_getaffinity (getpid (), आकार (set), &set) == 0)
        cpus = MIN (CPU_मोजणी (&set), LIBKRUN_MAX_VCPUS);
      अन्यथा
        cpus = 1;
    }

  /* We let the OCI set the amount of RAM for the VM, since cgroups memory restrictions still apply. */
  ram_mib = libkrun_parse_resource_configuration (kconf->config_वृक्ष, container, "krun.ram_mib", "ram_mib", true);
  जर (ram_mib <= LIBKRUN_MINIMUM_RAM_MIB)
    {
      जर (def && def->linux && def->linux->resources && def->linux->resources->memory
          && def->linux->resources->memory->limit_present)
        ram_mib = def->linux->resources->memory->limit / (1024 * 1024);
      अन्यथा
        ram_mib = LIBKRUN_DEFAULT_RAM_MIB;
    }

  krun_set_vm_config = dlsym (handle, "krun_set_vm_config");

  जर (krun_set_vm_config == रिक्त)
    परत crun_make_error (err, 0, "could not find symbol in the krun library");

  ret = krun_set_vm_config (ctx_id, cpus, ram_mib);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, -ret, "could not set krun vm configuration");

  nested_virt = libkrun_parse_resource_configuration (kconf->config_वृक्ष, container, "krun.nested_virt", "nested_virt", false);
  जर (nested_virt > 0)
    {
      int32_t (*krun_check_nested_virt) (शून्य);
      int32_t (*krun_set_nested_virt) (uint32_t ctx_id, bool enabled);

      krun_check_nested_virt = dlsym (handle, "krun_check_nested_virt");
      जर (krun_check_nested_virt != रिक्त && krun_check_nested_virt () != 1)
        libcrun_warning ("nested virtualization requested but may not be supported on this host");

      krun_set_nested_virt = dlsym (handle, "krun_set_nested_virt");
      जर (krun_set_nested_virt == रिक्त)
        परत crun_make_error (err, 0, "could not find symbol `krun_set_nested_virt` in the krun library");

      ret = krun_set_nested_virt (ctx_id, true);
      जर (UNLIKELY (ret < 0))
        परत crun_make_error (err, -ret, "could not enable nested virtualization");
    }

  जर (kconf->tap_name != रिक्त)
    {
      krun_add_net_tap = dlsym (handle, "krun_add_net_tap");
      जर (krun_add_net_tap == रिक्त)
        परत crun_make_error (err, 0, "could not find symbol `krun_add_net_tap` in the krun library");

      uint8_t mac[6];
      libkrun_make_tap_mac (container->context->id, mac);
      ret = krun_add_net_tap (ctx_id, kconf->tap_name, &mac[0], COMPAT_NET_FEATURES, 0);
      जर (UNLIKELY (ret < 0))
        परत crun_make_error (err, -ret, "could not add krun TAP interface `%s`", kconf->tap_name);
    }
  अन्यथा जर (kconf->use_passt)
    {
      krun_add_net_unixstream = dlsym (handle, "krun_add_net_unixstream");
      जर (krun_add_net_unixstream == रिक्त)
        परत crun_make_error (err, 0, "could not find symbol `krun_add_net_unixstream` in the krun library");

      uint8_t mac[] = { 0x5a, 0x94, 0xef, 0xe4, 0x0c, 0xee };
      ret = krun_add_net_unixstream (ctx_id, रिक्त, kconf->passt_fds[FD_PAIR_PARENT], &mac[0], COMPAT_NET_FEATURES, NET_FLAG_DHCP_CLIENT);
      जर (UNLIKELY (ret == -EINVAL))
        ret = krun_add_net_unixstream (ctx_id, रिक्त, kconf->passt_fds[FD_PAIR_PARENT], &mac[0], COMPAT_NET_FEATURES, 0);
      जर (UNLIKELY (ret < 0))
        परत crun_make_error (err, -ret, "could not set krun net configuration");
    }

  जर (kconf->gpu_flags > 0)
    {
      krun_set_gpu_options = dlsym (kconf->handle, "krun_set_gpu_options");
      जर (krun_set_gpu_options == रिक्त)
        परत crun_make_error (err, 0, "gpu requested but the version of libkrun in this system does not support it");

      ret = krun_set_gpu_options (kconf->ctx_id, kconf->gpu_flags);
      जर (UNLIKELY (ret < 0))
        परत crun_make_error (err, -ret, "gpu requested but could not configure virtio gpu device");

      जर (kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER)
        {
          krun_set_gpu_render_server_fd = dlsym (kconf->handle, "krun_set_gpu_render_server_fd");
          जर (krun_set_gpu_render_server_fd == रिक्त)
            परत crun_make_error (err, 0, "gpu with render server requested but the version of libkrun in this system does not support it");

          ret = krun_set_gpu_render_server_fd (kconf->ctx_id, kconf->gpu_render_server_fd);
          जर (UNLIKELY (ret < 0))
            परत crun_make_error (err, -ret, "gpu with render server requested but could not configure its file descriptor");
        }
    }

  जर (kconf->config_वृक्ष != रिक्त)
    {
      /* Try to configure an external kernel. If the configuration file doesn't
       * specify a kernel, libkrun automatically fall back to using libkrunfw,
       * if the library is present and was loaded while creating the context.
       */
      पूर्णांक custom_kernel = libkrun_parse_resource_configuration (kconf->config_वृक्ष, container, "krun.custom_kernel", "custom_kernel", false);
      जर (custom_kernel > 0)
        {
          ret = libkrun_configure_kernel (ctx_id, handle, kconf->config_वृक्ष, err);
          जर (UNLIKELY (ret))
            परत ret;
        }
    }

  परत 0;
}

स्थिर पूर्णांक
libkrun_configure_flavor (शून्य *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णांक ret, sev_indicated = 0, awsnitro_indicated = 0;
  संरचना krun_config *kconf = (संरचना krun_config *) cookie;
  स्थिरांक अक्षर *flavor = रिक्त;
  शून्य *close_handles[2];

  close_handles[0] = रिक्त;
  close_handles[1] = रिक्त;

  // Check if the user provided the krun variant through OCI annotations.
  flavor = find_annotation (container, "krun.variant");
  जर (flavor != रिक्त)
    {
      sev_indicated |= म_भेद (flavor, KRUN_FLAVOR_SEV) == 0;
      awsnitro_indicated |= म_भेद (flavor, KRUN_FLAVOR_AWS_NITRO) == 0;
    }

  जर (sev_indicated)
    {
      जर (kconf->handle_sev == रिक्त)
        error (निकास_त्रुटि, 0, "the container requires libkrun-sev but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_awsnitro;

      kconf->handle = kconf->handle_sev;
      kconf->ctx_id = kconf->ctx_id_sev;
      kconf->sev = true;
    }
  अन्यथा जर (awsnitro_indicated)
    {
      जर (kconf->handle_awsnitro == रिक्त)
        error (निकास_त्रुटि, 0, "the container requires libkrun-awsnitro but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_sev;

      kconf->handle = kconf->handle_awsnitro;
      kconf->ctx_id = kconf->ctx_id_awsnitro;
      kconf->awsnitro = true;
    }
  अन्यथा
    {
      जर (kconf->handle == रिक्त)
        error (निकास_त्रुटि, 0, "the container requires libkrun but it's not available");

      close_handles[0] = kconf->handle_sev;
      close_handles[1] = kconf->handle_awsnitro;
    }

  // We no longer need the other two libkrun handles.
  साठी (पूर्णांक i = 0; i < 2; i++)
    {
      जर (close_handles[i] == रिक्त)
        सुरूठेवा;

      ret = dlclose (close_handles[i]);
      जर (UNLIKELY (ret != 0))
        परत crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }

  परत 0;
}

स्थिर पूर्णांक
libkrun_exec (शून्य *cookie, libcrun_container_t *container, स्थिरांक अक्षर *pathname, अक्षर *स्थिरांक argv[])
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_set_log_level) (uint32_t level);
  पूर्णांक (*krun_start_enter) (uint32_t ctx_id);
  int32_t (*krun_add_virtiofs2) (uint32_t ctx_id, स्थिरांक अक्षर *c_tag, स्थिरांक अक्षर *c_path, uint64_t shm_size);
  int32_t (*krun_set_मूळ_disk) (uint32_t ctx_id, स्थिरांक अक्षर *disk_path);
  int32_t (*krun_set_tee_config_file) (uint32_t ctx_id, स्थिरांक अक्षर *file_path);
  int32_t (*krun_set_console_output) (uint32_t ctx_id, स्थिरांक अक्षर *c_filepath);
  int32_t (*krun_set_exec) (uint32_t ctx_id, स्थिरांक अक्षर *exec_path,
                            स्थिरांक अक्षर *स्थिरांक argv[], स्थिरांक अक्षर *स्थिरांक envp[]);
  संरचना krun_config *kconf = (संरचना krun_config *) cookie;
  शून्य *handle;
  int32_t ctx_id, ret;
  libcrun_error_t err;

  ret = libkrun_configure_flavor (cookie, container, &err);
  जर (UNLIKELY (ret < 0))
    {
      पूर्णांक errcode = crun_error_get_errno (&err);
      crun_error_release (&err);
      error (निकास_त्रुटि, errcode, "unable to configure libkrun flavor");
    }

  // /dev/kvm is required for all non AWS nitro workloads.
  जर (! kconf->awsnitro && ! kconf->has_kvm)
    error (निकास_त्रुटि, 0, "`/dev/kvm` unavailable");

  handle = kconf->handle;
  ctx_id = kconf->ctx_id;

  krun_set_log_level = dlsym (handle, "krun_set_log_level");
  krun_start_enter = dlsym (handle, "krun_start_enter");
  जर (krun_set_log_level == रिक्त || krun_start_enter == रिक्त)
    error (निकास_त्रुटि, 0, "could not find symbol in the krun library");

  /* Set log level according to crun's verbosity. */
  निवड (libcrun_get_verbosity ())
    {
    स्थिती LIBCRUN_VERBOSITY_DEBUG:
      krun_set_log_level (KRUN_LOG_LEVEL_DEBUG);
      थांबा;
    स्थिती LIBCRUN_VERBOSITY_WARNING:
      krun_set_log_level (KRUN_LOG_LEVEL_WARN);
      थांबा;
    पूर्वनिर्धारित:
      krun_set_log_level (KRUN_LOG_LEVEL_ERROR);
      थांबा;
    }

  जर (kconf->sev)
    {
      krun_set_मूळ_disk = dlsym (handle, "krun_set_root_disk");
      krun_set_tee_config_file = dlsym (handle, "krun_set_tee_config_file");
      जर (krun_set_मूळ_disk == रिक्त || krun_set_tee_config_file == रिक्त)
        error (निकास_त्रुटि, 0, "could not find symbol in `libkrun-sev.so`");

      ret = krun_set_मूळ_disk (ctx_id, "/disk.img");
      जर (UNLIKELY (ret < 0))
        error (निकास_त्रुटि, -ret, "could not set root disk");

      ret = krun_set_tee_config_file (ctx_id, KRUN_SEV_FILE);
      जर (UNLIKELY (ret < 0))
        error (निकास_त्रुटि, -ret, "could not set krun tee config file");
    }
  अन्यथा
    {
      json_object *val_virtiofs_tag = रिक्त;
      json_object *val_virtiofs_shm_size = रिक्त;
      स्थिरांक अक्षर *virtiofs_tag = रिक्त;
      // Default to a conservative DAX size of 512MB, just like krun_set_root() does.
      uint64_t virtiofs_shm_size = 512 * 1024 * 1024ULL;

      जर (kconf->config_वृक्ष != रिक्त)
        {
          val_virtiofs_tag = json_object_object_get (kconf->config_वृक्ष, "virtiofs_tag");
          जर (val_virtiofs_tag != रिक्त && json_object_is_type (val_virtiofs_tag, json_type_string))
            virtiofs_tag = json_object_get_string (val_virtiofs_tag);

          val_virtiofs_shm_size = json_object_object_get (kconf->config_वृक्ष, "virtiofs_shm_size");
          जर (val_virtiofs_shm_size != रिक्त && json_object_is_type (val_virtiofs_shm_size, json_type_int))
            virtiofs_shm_size = json_object_get_uint64 (val_virtiofs_shm_size);
        }

      जर (virtiofs_tag == रिक्त)
        virtiofs_tag = "/dev/root";

      krun_add_virtiofs2 = dlsym (handle, "krun_add_virtiofs2");

      जर (krun_add_virtiofs2 == रिक्त)
        error (निकास_त्रुटि, 0, "could not find symbol `krun_add_virtiofs2` in `libkrun.so`");

      ret = krun_add_virtiofs2 (ctx_id, virtiofs_tag, "/", virtiofs_shm_size);
      जर (UNLIKELY (ret < 0))
        error (निकास_त्रुटि, -ret, "could not add virtiofs root with tag `%s`", virtiofs_tag);
    }

  जर (kconf->awsnitro)
    {
      krun_set_console_output = dlsym (handle, "krun_set_console_output");
      krun_set_exec = dlsym (handle, "krun_set_exec");
      जर (krun_set_console_output == रिक्त || krun_set_exec == रिक्त)
        error (निकास_त्रुटि, 0, "could not find symbol in `libkrun-awsnitro.so`");

      // Redirect all enclave output (read from vsock) to stdout.
      ret = krun_set_console_output (ctx_id, "/dev/stdout");
      जर (UNLIKELY (ret < 0))
        error (निकास_त्रुटि, -ret, "could not redirect enclave output to stdout");

      ret = krun_set_exec (ctx_id, pathname, (स्थिरांक अक्षर *स्थिरांक *) argv,
                           (स्थिरांक अक्षर *स्थिरांक *) def->process->env);
      जर (UNLIKELY (ret < 0))
        error (निकास_त्रुटि, -ret, "could not set enclave execution arguments");
    }

  ret = libkrun_configure_vm (ctx_id, handle, kconf, container, &err);
  जर (UNLIKELY (ret))
    {
      पूर्णांक errcode = crun_error_get_errno (&err);
      libcrun_error_t *tmp_err = &err;
      libcrun_error_report_and_release (tmp_err);
      error (निकास_त्रुटि, errcode, "could not configure krun vm");
    }

  json_object_put (kconf->config_doc);

  ret = krun_start_enter (ctx_id);
  जर (UNLIKELY (ret < 0))
    error (निकास_त्रुटि, -ret, "could not start krun");

  परत ret;
}

स्थिर पूर्णांक
libkrun_start_render_server (संरचना krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णांक ret;

  kconf->gpu_flags = libkrun_parse_resource_configuration (kconf->config_वृक्ष, container, "krun.gpu_flags", "gpu_flags", false);
  जर (kconf->gpu_flags > 0)
    {
      जर (access ("/dev/dri", F_OK) != 0)
        परत crun_make_error (err, त्रुटि_सं, "gpu requested but /dev/dri is not available");

      जर ((kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER) != 0)
        {
          अक्षर *rs_argv[40];
          पूर्णांक rs_argc = 0;
          अक्षर fd_as_str[16];
          पूर्णांक gpu_fds[2];
          pid_t pid;
          पूर्णांक null;

          जर (access ("/usr/libexec/virgl_render_server", X_OK) != 0)
            परत crun_make_error (err, त्रुटि_सं, "gpu with render server requested but couldn't find /usr/libexec/virgl_render_server binary");

          जर (access ("/usr/bin/bwrap", X_OK) != 0)
            परत crun_make_error (err, त्रुटि_सं, "gpu with render server requested but couldn't find /usr/bin/bwrap binary");

          ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, gpu_fds);
          जर (UNLIKELY (ret < 0))
            परत crun_make_error (err, त्रुटि_सं, "could not create socketpair for virgl_render_server");

          snprintf (fd_as_str, आकार (fd_as_str), "%d", gpu_fds[FD_PAIR_CHILD]);

          rs_argv[rs_argc++] = (अक्षर *) "bwrap";
          rs_argv[rs_argc++] = (अक्षर *) "--ro-bind";
          rs_argv[rs_argc++] = (अक्षर *) "/usr";
          rs_argv[rs_argc++] = (अक्षर *) "/usr";
          जर (access ("/lib", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (अक्षर *) "--ro-bind";
              rs_argv[rs_argc++] = (अक्षर *) "/lib";
              rs_argv[rs_argc++] = (अक्षर *) "/lib";
            }
          जर (access ("/lib64", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (अक्षर *) "--ro-bind";
              rs_argv[rs_argc++] = (अक्षर *) "/lib64";
              rs_argv[rs_argc++] = (अक्षर *) "/lib64";
            }
          rs_argv[rs_argc++] = (अक्षर *) "--ro-bind";
          rs_argv[rs_argc++] = (अक्षर *) "/etc";
          rs_argv[rs_argc++] = (अक्षर *) "/etc";
          rs_argv[rs_argc++] = (अक्षर *) "--ro-bind";
          rs_argv[rs_argc++] = (अक्षर *) "/sys";
          rs_argv[rs_argc++] = (अक्षर *) "/sys";
          rs_argv[rs_argc++] = (अक्षर *) "--dev";
          rs_argv[rs_argc++] = (अक्षर *) "/dev";
          rs_argv[rs_argc++] = (अक्षर *) "--dev-bind";
          rs_argv[rs_argc++] = (अक्षर *) "/dev/dri";
          rs_argv[rs_argc++] = (अक्षर *) "/dev/dri";
          rs_argv[rs_argc++] = (अक्षर *) "--proc";
          rs_argv[rs_argc++] = (अक्षर *) "/proc";
          rs_argv[rs_argc++] = (अक्षर *) "--tmpfs";
          rs_argv[rs_argc++] = (अक्षर *) "/tmp";
          rs_argv[rs_argc++] = (अक्षर *) "--unshare-pid";
          rs_argv[rs_argc++] = (अक्षर *) "--unshare-ipc";
          rs_argv[rs_argc++] = (अक्षर *) "--unshare-net";
          rs_argv[rs_argc++] = (अक्षर *) "--";
          rs_argv[rs_argc++] = (अक्षर *) "/usr/libexec/virgl_render_server";
          rs_argv[rs_argc++] = (अक्षर *) "--socket-fd";
          rs_argv[rs_argc++] = fd_as_str;
          rs_argv[rs_argc] = रिक्त;

          pid = विभाजन ();
          जर (pid < 0)
            {
              बंद (gpu_fds[FD_PAIR_PARENT]);
              बंद (gpu_fds[FD_PAIR_CHILD]);
              परत crun_make_error (err, त्रुटि_सं, "could not fork for virgl_render_server");
            }
          अन्यथा जर (pid == 0)
            {
              बंद (gpu_fds[FD_PAIR_PARENT]);

              null = खोलो ("/dev/null", O_WRONLY);
              जर (null == -1)
                _exit (निकास_त्रुटि);

              dup2 (null, STDOUT_FILENO);
              dup2 (null, STDERR_FILENO);
              बंद (null);

              execvp ("/usr/bin/bwrap", rs_argv);
              _exit (निकास_त्रुटि);
            }

          बंद (gpu_fds[FD_PAIR_CHILD]);
          kconf->gpu_render_server_fd = gpu_fds[FD_PAIR_PARENT];
        }
    }

  परत 0;
}

स्थिर पूर्णांक
libkrun_configure_network (शून्य *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  संरचना krun_config *kconf = (संरचना krun_config *) cookie;
  pid_t pid;
  अक्षर *passt_argv[10];
  अक्षर fd_as_str[16];
  पूर्णांक use_passt;
  पूर्णांक argv_idx;
  पूर्णांक status;
  पूर्णांक null;
  पूर्णांक ret;

  ret = libkrun_parse_string_configuration (kconf->config_वृक्ष, container,
                                            "krun.tap_name", "tap_name",
                                            &kconf->tap_name, err, false);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (kconf->tap_name != रिक्त && is_empty_string (kconf->tap_name))
    परत crun_make_error (err, 0, "krun.tap_name cannot be empty");

  use_passt = libkrun_parse_resource_configuration (kconf->config_वृक्ष, container, "krun.use_passt", "use_passt", false);

  जर (use_passt > 0)
    {
      जर (kconf->tap_name != रिक्त)
        परत crun_make_error (err, 0, "krun.tap_name and krun.use_passt are mutually exclusive");

      kconf->use_passt = 1;
    }
  अन्यथा
    परत 0;

  ret = socketpair (AF_UNIX, SOCK_STREAM, 0, kconf->passt_fds);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "create passt socketpair");

  snprintf (fd_as_str, आकार (fd_as_str), "%d", kconf->passt_fds[FD_PAIR_CHILD]);

  argv_idx = 0;
  passt_argv[argv_idx++] = (अक्षर *) "passt";
  passt_argv[argv_idx++] = (अक्षर *) "-t";
  passt_argv[argv_idx++] = (अक्षर *) "all";

  जर (! kconf->has_awsnitro)
    {
      passt_argv[argv_idx++] = (अक्षर *) "-u";
      passt_argv[argv_idx++] = (अक्षर *) "all";
      passt_argv[argv_idx++] = (अक्षर *) "--no-dhcp-dns";
    }

  /* Set --no-map-gw. If the gateway for the network is also the DNS server
   * we won't be able to query the DNS server without --no-map-gw.
   * See discussion in https://github.com/containers/crun/pull/2099
   */
  passt_argv[argv_idx++] = (अक्षर *) "--no-map-gw";

  passt_argv[argv_idx++] = (अक्षर *) "--fd";
  passt_argv[argv_idx++] = fd_as_str;
  passt_argv[argv_idx] = रिक्त;

  pid = विभाजन ();
  जर (pid < 0)
    परत crun_make_error (err, त्रुटि_सं, "fork passt");
  अन्यथा जर (pid == 0)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      माप_प्रकार i;

      बंद (kconf->passt_fds[FD_PAIR_PARENT]);

      null = खोलो ("/dev/null", O_WRONLY);
      जर (null == -1)
        _exit (निकास_त्रुटि);

      /* Remount /proc if the parent detached its PID namespace, as passt(1)
       * needs a consistent /proc view to operate properly.
       */
      साठी (i = 0; i < def->linux->namespaces_len; i++)
        {
          पूर्णांक मूल्य = libcrun_find_namespace (def->linux->namespaces[i]->type);

          जर (मूल्य == CLONE_NEWPID)
            {
              जर (mount ("", "/proc", "proc", MS_NOSUID | MS_NODEV | MS_NOEXEC, रिक्त))
                _exit (निकास_त्रुटि);

              थांबा;
            }
        }

      // Redirect passt's stdout and stderr to /dev/null, as closing them here
      // instead will cause passt to exit with an error.
      dup2 (null, STDOUT_FILENO);
      dup2 (null, STDERR_FILENO);
      बंद (null);

      execvp ("/usr/bin/passt", passt_argv);
      // Only reachable on error.
      _exit (निकास_त्रुटि);
    }

  बंद (kconf->passt_fds[FD_PAIR_CHILD]);

  // Wait for passt to daemonize itself.
  waitpid (pid, &status, 0);
  जर (! (WIFEXITED (status)) || WEXITSTATUS (status) != 0)
    परत crun_make_error (err, 0, "start passt");

  परत 0;
}

/* Return true if the spec already declares a device with the given PATH.  */
स्थिर bool
spec_has_device (runtime_spec_schema_config_schema *def, स्थिरांक अक्षर *path)
{
  माप_प्रकार i;

  साठी (i = 0; i < def->linux->devices_len; i++)
    जर (म_भेद (def->linux->devices[i]->path, path) == 0)
      परत true;
  परत false;
}

/* libkrun_create_kvm_device: explicitly adds kvm device.  */
स्थिर पूर्णांक
libkrun_configure_container (शून्य *cookie, गणना handler_configure_phase phase,
                             libcrun_context_t *context, libcrun_container_t *container,
                             स्थिरांक अक्षर *rootfs, libcrun_error_t *err)
{
  पूर्णांक ret, rootfsfd;
  संरचना krun_config *kconf = (संरचना krun_config *) cookie;
  संरचना device_s kvm_device = { "/dev/kvm", "c", 10, 232, 0666, 0, 0 };
  संरचना device_s sev_device = { "/dev/sev", "c", 10, 124, 0666, 0, 0 };
  संरचना device_s nitro_device = { "/dev/nitro_enclaves", "c", 10, 122, 0666, 0, 0 };
  cleanup_close पूर्णांक devfd = -1;
  cleanup_close पूर्णांक rootfsfd_cleanup = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  bool create_sev = false, create_awsnitro = false;
  bool is_user_ns;

  जर (rootfs == रिक्त)
    rootfsfd = AT_FDCWD;
  अन्यथा
    {
      rootfsfd = rootfsfd_cleanup = खोलो (rootfs, O_PATH | O_CLOEXEC);
      जर (UNLIKELY (rootfsfd < 0))
        परत crun_make_error (err, त्रुटि_सं, "open `%s`", rootfs);
    }

  जर (phase == HANDLER_CONFIGURE_BEFORE_USERNS)
    {
      cleanup_free अक्षर *origin_config_path = रिक्त;
      cleanup_free अक्षर *state_dir = रिक्त;
      cleanup_free अक्षर *config = रिक्त;
      cleanup_close पूर्णांक fd = -1;
      माप_प्रकार config_size;

      ret = libcrun_get_state_directory (&state_dir, context->state_मूळ, context->id, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      ret = append_paths (&origin_config_path, err, state_dir, "config.json", रिक्त);
      जर (UNLIKELY (ret < 0))
        परत ret;

      ret = read_all_file (origin_config_path, &config, &config_size, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      /* CVE-2025-24965: the content below rootfs cannot be trusted because it is controlled by the user.  We
         must ensure the file is opened below the rootfs directory.  Whatever the image ships at that name
         could also be a device node or a FIFO, so never open it: unlink it and create a new regular file.  */
      ret = unlinkat (rootfsfd, KRUN_CONFIG_FILE, 0);
      जर (UNLIKELY (ret < 0 && त्रुटि_सं != ENOENT))
        परत crun_make_error (err, त्रुटि_सं, "unlink `%s`", KRUN_CONFIG_FILE);

      fd = safe_openat (rootfsfd, rootfs, KRUN_CONFIG_FILE, WRITE_FILE_DEFAULT_FLAGS | O_EXCL | O_NOFOLLOW, S_IRUSR | S_IRGRP | S_IROTH, err);
      जर (UNLIKELY (fd < 0))
        परत fd;

      ret = safe_write (fd, KRUN_CONFIG_FILE, config, config_size, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      ret = libkrun_read_vm_config (kconf, rootfsfd, rootfs, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      ret = libkrun_start_render_server (kconf, container, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  जर (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    परत 0;

  ret = libkrun_configure_network (cookie, container, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  /* Do nothing if /dev/kvm is already present in spec */
  जर (spec_has_device (def, "/dev/kvm"))
    परत 0;

  जर (kconf->handle_sev != रिक्त)
    create_sev = ! spec_has_device (def, "/dev/sev");

  जर (kconf->handle_awsnitro != रिक्त)
    create_awsnitro = ! spec_has_device (def, "/dev/nitro_enclaves");

  devfd = safe_openat (rootfsfd, rootfs, "dev", O_PATH | O_DIRECTORY | O_CLOEXEC, 0, err);
  जर (UNLIKELY (devfd < 0))
    परत devfd;

  ret = check_running_in_user_namespace (err);
  जर (UNLIKELY (ret < 0))
    परत ret;
  is_user_ns = ret;

  जर (kconf->has_kvm)
    {
      ret = libcrun_create_dev (container, devfd, -1, &kvm_device, is_user_ns, true, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  जर (create_sev)
    {
      ret = libcrun_create_dev (container, devfd, -1, &sev_device, is_user_ns, true, err);
      जर (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_sev);
          जर (UNLIKELY (ret < 0))
            परत ret;
          kconf->handle_sev = रिक्त;
        }
    }

  जर (create_awsnitro)
    {
      ret = libcrun_create_dev (container, devfd, -1, &nitro_device, is_user_ns, true, err);
      जर (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_awsnitro);
          जर (UNLIKELY (ret < 0))
            परत ret;
          kconf->handle_awsnitro = रिक्त;
        }
    }

  परत 0;
}

स्थिर पूर्णांक
libkrun_load (शून्य **cookie, libcrun_error_t *err)
{
  int32_t ret;
  संरचना krun_config *kconf;
  स्थिरांक अक्षर *libkrun_so = "libkrun.so.1";
  स्थिरांक अक्षर *libkrun_sev_so = "libkrun-sev.so.1";
  स्थिरांक अक्षर *libkrun_awsnitro_so = "libkrun-awsnitro.so.1";

  kconf = दो_स्मृति (आकार (संरचना krun_config));
  जर (kconf == रिक्त)
    परत crun_make_error (err, 0, "could not allocate memory for krun_config");
  स_रखो (kconf, 0, आकार (संरचना krun_config));

  kconf->gpu_render_server_fd = -1;

  kconf->handle = dlopen (libkrun_so, RTLD_NOW);
  kconf->handle_sev = dlopen (libkrun_sev_so, RTLD_NOW);
  kconf->handle_awsnitro = dlopen (libkrun_awsnitro_so, RTLD_NOW);

  जर (kconf->handle == रिक्त && kconf->handle_sev == रिक्त && kconf->handle_awsnitro == रिक्त)
    {
      मुक्त (kconf);
      परत crun_make_error (err, 0, "failed to open `%s`, `%s`, and `%s` for krun_config: %s", libkrun_so, libkrun_sev_so, libkrun_awsnitro_so, dlerror ());
    }

  kconf->sev = false;
  kconf->awsnitro = false;

  /* Newer versions of libkrun no longer link against libkrunfw and
     instead they open it when creating the context. This implies
     we need to call "krun_create_ctx" before switching namespaces
     or it won't be able to find the library bundling the kernel. */
  जर (kconf->handle)
    {
      ret = libkrun_create_context (kconf->handle, err);
      जर (UNLIKELY (ret < 0))
        जा error;
      kconf->ctx_id = ret;
    }

  जर (kconf->handle_sev)
    {
      ret = libkrun_create_context (kconf->handle_sev, err);
      जर (UNLIKELY (ret < 0))
        जा error;
      kconf->ctx_id_sev = ret;
    }
  जर (kconf->handle_awsnitro)
    {
      ret = libkrun_create_context (kconf->handle_awsnitro, err);
      जर (UNLIKELY (ret < 0))
        जा error;
      kconf->ctx_id_awsnitro = ret;
    }

  *cookie = kconf;

  परत 0;

error:
  जर (kconf->handle)
    dlclose (kconf->handle);
  जर (kconf->handle_sev)
    dlclose (kconf->handle_sev);
  जर (kconf->handle_awsnitro)
    dlclose (kconf->handle_awsnitro);
  मुक्त (kconf);
  परत ret;
}

स्थिर पूर्णांक
libkrun_unload (शून्य *cookie, libcrun_error_t *err)
{
  पूर्णांक r;

  संरचना krun_config *kconf = (संरचना krun_config *) cookie;
  जर (kconf != रिक्त)
    {
      जर (kconf->handle != रिक्त)
        {
          r = dlclose (kconf->handle);
          जर (UNLIKELY (r != 0))
            परत crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
        }
      जर (kconf->handle_sev != रिक्त)
        {
          r = dlclose (kconf->handle_sev);
          जर (UNLIKELY (r != 0))
            परत crun_make_error (err, 0, "could not unload handle_sev: `%s`", dlerror ());
        }
      जर (kconf->handle_awsnitro != रिक्त)
        {
          r = dlclose (kconf->handle_awsnitro);
          जर (UNLIKELY (r != 0))
            परत crun_make_error (err, 0, "could not unload handle_awsnitro: `%s`", dlerror ());
        }
      मुक्त (kconf);
    }
  परत 0;
}

स्थिर runtime_spec_schema_defs_linux_device_cgroup *
make_oci_spec_dev (स्थिरांक अक्षर *type, dev_t device, bool allow, स्थिरांक अक्षर *access)
{
  runtime_spec_schema_defs_linux_device_cgroup *dev = xmalloc0 (आकार (*dev));

  dev->allow = allow;
  dev->allow_present = 1;

  dev->type = xstrdup (type);

  dev->major = major (device);
  dev->major_present = 1;

  dev->minor = minor (device);
  dev->minor_present = 1;

  dev->access = xstrdup (access);

  परत dev;
}

/* stat an optional device: on ENOENT clear *PRESENT, any other error fails.  */
स्थिर पूर्णांक
stat_optional_device (स्थिरांक अक्षर *path, संरचना stat *st, bool *present, libcrun_error_t *err)
{
  पूर्णांक ret = stat (path, st);
  जर (UNLIKELY (ret < 0))
    {
      जर (त्रुटि_सं != ENOENT)
        परत crun_make_error (err, त्रुटि_सं, "stat `%s`", path);
      *present = false;
    }
  परत 0;
}

स्थिर पूर्णांक
libkrun_modify_oci_configuration (शून्य *cookie arg_unused, libcrun_context_t *context arg_unused,
                                  runtime_spec_schema_config_schema *def,
                                  libcrun_error_t *err)
{
  स्थिरांक माप_प्रकार device_size = आकार (runtime_spec_schema_defs_linux_device_cgroup);
  संरचना krun_config *kconf = (संरचना krun_config *) cookie;
  संरचना stat st_kvm, st_sev, st_awsnitro;
  bool has_kvm = true, has_sev = true, has_awsnitro = true;
  माप_प्रकार old_len, new_len;
  पूर्णांक ret;

  /* Always allow the /dev/kvm device.  */

  ret = stat_optional_device ("/dev/kvm", &st_kvm, &has_kvm, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = stat_optional_device ("/dev/sev", &st_sev, &has_sev, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = stat_optional_device ("/dev/nitro_enclaves", &st_awsnitro, &has_awsnitro, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  kconf->has_kvm = has_kvm;
  kconf->has_awsnitro = has_awsnitro;

  जर (! has_kvm && ! has_awsnitro)
    परत 0;

  /* spec says these are optional, ensure they exist so we can add our devices */
  जर (def->linux == रिक्त)
    def->linux = xmalloc0 (आकार (runtime_spec_schema_config_linux));

  जर (def->linux->resources == रिक्त)
    def->linux->resources = xmalloc0 (आकार (runtime_spec_schema_config_linux_resources));

  old_len = def->linux->resources->devices_len;
  new_len = old_len;
  जर (has_kvm)
    new_len += has_sev ? 2 : 1;
  जर (has_awsnitro)
    new_len += 1;

  def->linux->resources->devices = xrealloc (def->linux->resources->devices, device_size * (new_len + 1));
  def->linux->resources->devices_len = new_len;

  जर (has_kvm)
    {
      def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_kvm.st_rdev, true, "rwm");
      जर (has_sev)
        def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_sev.st_rdev, true, "rwm");
    }

  जर (has_awsnitro)
    def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_awsnitro.st_rdev, true, "rwm");

  परत 0;
}

स्थिर पूर्णांक
libkrun_close_fds (शून्य *cookie, libcrun_container_t *container, पूर्णांक preserve_fds, libcrun_error_t *err)
{
  संरचना krun_config *kconf = (संरचना krun_config *) cookie;
  पूर्णांक first_fd_to_close = preserve_fds + 3;
  पूर्णांक खालचा_fd, वरचा_fd;
  पूर्णांक i;

  खालचा_fd = वरचा_fd = -1;

  जर (kconf->use_passt)
    वरचा_fd = kconf->passt_fds[FD_PAIR_PARENT];

  जर (kconf->gpu_render_server_fd != -1)
    {
      जर (kconf->gpu_render_server_fd > वरचा_fd)
        {
          खालचा_fd = वरचा_fd;
          वरचा_fd = kconf->gpu_render_server_fd;
        }
      अन्यथा
        खालचा_fd = kconf->gpu_render_server_fd;
    }

  जर (first_fd_to_close <= वरचा_fd)
    {
      साठी (i = first_fd_to_close; i < वरचा_fd; i++)
        {
          // If we're closing proc_fd, make sure to invalidate it.
          जर (i == container->proc_fd)
            container->proc_fd = -1;
          अन्यथा जर (i == खालचा_fd)
            सुरूठेवा;
          बंद (i);
        }

      first_fd_to_close = वरचा_fd + 1;
    }

  परत mark_or_close_fds_ge_than (container, first_fd_to_close, true, err);
}

संरचना custom_handler_s handler_libkrun = {
  .name = "krun",
  .alias = रिक्त,
  .feature_string = "LIBKRUN",
  .supports_open_वृक्ष_namespace = false,
  .load = libkrun_load,
  .unload = libkrun_unload,
  .run_func = libkrun_exec,
  .configure_container = libkrun_configure_container,
  .modify_oci_configuration = libkrun_modify_oci_configuration,
  .close_fds = libkrun_close_fds,
};

#endif
