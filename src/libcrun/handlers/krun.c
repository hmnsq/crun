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
#परिभाषय _GNU_SOURCE

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

#यदि_परिभाषितम् HAVE_DLOPEN
#  include <dlfcn.h>
#यदि_अन्तः

#यदि_परिभाषितम् HAVE_LIBKRUN
#  include <libkrun.h>
#यदि_अन्तः

/* This allows us to build even with headers from older libkrun versions.
 * If the installed version of libkrun doesn't support this feature,
 * krun_add_net_unixstream() will return EINVAL.
 */
#यदि_अपरिभाषितम् NET_FLAG_DHCP_CLIENT
#  define NET_FLAG_DHCP_CLIENT (1 << 1)
#यदि_अन्तः
/* libkrun has a hard-limit of 16 vCPUs per microVM. */
#परिभाषय LIBKRUN_MAX_VCPUS 16

/* If the user doesn't configure the RAM amount, fallback to this value. */
#परिभाषय LIBKRUN_DEFAULT_RAM_MIB 1024

/* The minimum amount of RAM for a viable microVM is 128 MB. */
#परिभाषय LIBKRUN_MINIMUM_RAM_MIB 128

/* crun dumps the container configuration into this file, which will be read by
 * libkrun to set up the environment for the workload inside the microVM.
 */
#परिभाषय KRUN_CONFIG_FILE ".krun_config.json"

/* The presence of this file indicates this is a container intended to be run
 * as a confidential workload inside a SEV-powered TEE.
 */
#परिभाषय KRUN_SEV_FILE "/krun-sev.json"

/* This file contains configuration parameters for the microVM. crun needs to
 * read and parse it, using the information obtained from it to configure
 * libkrun as required.
 */
#परिभाषय KRUN_VM_FILE "/.krun_vm.json"

/* KRUN_VM_FILE holds a handful of settings and it is parsed by crun
 * itself before the container process exists, refuse anything bigger.
 */
#परिभाषय KRUN_VM_FILE_MAX_आकार (64 * 1024)

#परिभाषय KRUN_FLAVOR_AWS_NITRO "aws-nitro"
#परिभाषय KRUN_FLAVOR_SEV "sev"

#परिभाषय FD_PAIR_PARENT 0
#परिभाषय FD_PAIR_CHILD 1

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
  पूर्णाङ्क passt_fds[2];
  json_object *config_doc;
  json_object *config_वृक्षः;
  bool use_passt;
  ध्रुव वर्ण *tap_name;
  पूर्णाङ्क gpu_render_server_fd;
  पूर्णाङ्क gpu_flags;
};

/* libkrun handler.  */
#यदि HAVE_DLOPEN && HAVE_LIBKRUN
स्थिर int32_t
libkrun_create_context (शून्य *handle, libcrun_error_t *err)
{
  int32_t (*krun_create_ctx) ();
  int32_t ctx_id;

  krun_create_ctx = dlsym (handle, "krun_create_ctx");
  यदि (krun_create_ctx == रिक्त)
    प्रत्यागम crun_make_error (err, 0, "could not find symbol in the krun library");

  ctx_id = krun_create_ctx ();
  यदि (UNLIKELY (ctx_id < 0))
    प्रत्यागम crun_make_error (err, -ctx_id, "could not create krun context");

  प्रत्यागम ctx_id;
}

स्थिर पूर्णाङ्क
libkrun_configure_kernel (uint32_t ctx_id, शून्य *handle, json_object *config_वृक्षः, libcrun_error_t *err)
{
  int32_t (*krun_समुच्चयः_kernel) (uint32_t ctx_id, ध्रुव वर्ण *kernel_मार्गः,
                              uint32_t kernel_format, ध्रुव वर्ण *initrd_मार्गः, ध्रुव वर्ण *kernel_cmdline);
  json_object *kernel_मार्गः = रिक्त;
  json_object *kernel_format = रिक्त;
  json_object *val_initrd_मार्गः = रिक्त;
  json_object *val_kernel_cmdline = रिक्त;
  ध्रुव वर्ण *initrd_मार्गः = रिक्त;
  ध्रुव वर्ण *kernel_cmdline = रिक्त;
  पूर्णाङ्क ret;

  /* kernel_path and kernel_format must be present */
  kernel_मार्गः = json_object_object_get (config_वृक्षः, "kernel_path");
  यदि (kernel_मार्गः == रिक्त || ! json_object_is_type (kernel_मार्गः, json_type_string))
    प्रत्यागम 0;

  kernel_format = json_object_object_get (config_वृक्षः, "kernel_format");
  यदि (kernel_format == रिक्त || ! json_object_is_type (kernel_format, json_type_int))
    प्रत्यागम 0;

  /* initrd and kernel_cmdline are optional */
  val_initrd_मार्गः = json_object_object_get (config_वृक्षः, "initrd_path");
  यदि (val_initrd_मार्गः != रिक्त && json_object_is_type (val_initrd_मार्गः, json_type_string))
    initrd_मार्गः = json_object_get_string (val_initrd_मार्गः);

  val_kernel_cmdline = json_object_object_get (config_वृक्षः, "kernel_cmdline");
  यदि (val_kernel_cmdline != रिक्त && json_object_is_type (val_kernel_cmdline, json_type_string))
    kernel_cmdline = json_object_get_string (val_kernel_cmdline);

  krun_समुच्चयः_kernel = dlsym (handle, "krun_set_kernel");
  यदि (krun_समुच्चयः_kernel == रिक्त)
    प्रत्यागम crun_make_error (err, 0, "could not find symbol in krun library");

  ret = krun_समुच्चयः_kernel (ctx_id,
                         json_object_get_string (kernel_मार्गः),
                         (uint32_t) json_object_get_int64 (kernel_format),
                         initrd_मार्गः, kernel_cmdline);

  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, -ret, "could not configure a krun external kernel");

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
libkrun_read_vm_config (संरचना krun_config *kconf, पूर्णाङ्क rootfsfd, ध्रुव वर्ण *rootfs, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_free वर्ण *config = रिक्त;
  cleanup_close पूर्णाङ्क pathfd = -1;
  cleanup_close पूर्णाङ्क fd = -1;
  proc_fd_मार्गः_t fd_मार्गः;
  संरचना stat st;

  /* The content of the rootfs is controlled by the user: look at the file through an
     O_PATH descriptor first and refuse anything that is not a small regular file, as
     opening a FIFO blocks and a device node must not be accessed.  */
  pathfd = safe_openat (rootfsfd, rootfs, KRUN_VM_FILE, O_मार्गः | O_CLOEXEC | O_NOFOLLOW, 0, err);
  यदि (pathfd < 0)
    {
      // The configuration file is optional, don't generate an error if it's missing.
      यदि (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          प्रत्यागम 0;
        }
      प्रत्यागम pathfd;
    }

  ret = fstat (pathfd, &st);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "stat `%s`", KRUN_VM_FILE);

  यदि (! S_ISREG (st.st_mode))
    प्रत्यागम crun_make_error (err, 0, "`%s` is not a regular file", KRUN_VM_FILE);

  get_proc_self_fd_मार्गः (fd_मार्गः, pathfd);

  fd = उद्घाटय (fd_मार्गः, O_RDONLY | O_CLOEXEC);
  यदि (UNLIKELY (fd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s`", KRUN_VM_FILE);

  ret = read_all_fd_with_max_आकार (fd, KRUN_VM_FILE, &config, रिक्त, KRUN_VM_FILE_MAX_आकार, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = parse_json_file (&kconf->config_doc, config, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  kconf->config_वृक्षः = kconf->config_doc;
  प्रत्यागम 0;
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
स्थिर पूर्णाङ्क
libkrun_parse_resource_configuration (json_object *config_वृक्षः, libcrun_container_t *container, ध्रुव वर्ण *annotation, ध्रुव वर्ण *कुञ्चिका, bool use_config_file)
{
  वर्ण *val_str, *endptr;
  पूर्णाङ्क val = -1;
  json_object *val_json = रिक्त;

  val_str = (वर्ण *) find_annotation (container, annotation);
  यदि (val_str != रिक्त)
    {
      errno = 0;
      val = (पूर्णाङ्क) strtol (val_str, &endptr, 10);
      यदि (errno != 0 || endptr == val_str || *endptr != '\0')
        /* Annotations value is not a valid integer. */
        error (EXIT_FAILURE, 0, "krun annotation %s value cannot be converted to an integer", annotation);

      यदि (val < 0)
        error (EXIT_FAILURE, 0, "krun annotation %s value must be a positive integer", annotation);

      प्रत्यागम val;
    }
  अन्यथा यदि (use_config_file && config_वृक्षः != रिक्त)
    {
      val_json = json_object_object_get (config_वृक्षः, कुञ्चिका);
      यदि (val_json == रिक्त)
        प्रत्यागम val;
      यदि (! json_object_is_type (val_json, json_type_int))
        error (EXIT_FAILURE, 0, "krun krun_vm.json %s value is not an integer", कुञ्चिका);

      val = (पूर्णाङ्क) json_object_get_int64 (val_json);
    }

  प्रत्यागम val;
}

स्थिर पूर्णाङ्क
libkrun_parse_string_configuration (json_object *config_वृक्षः, libcrun_container_t *container,
                                    ध्रुव वर्ण *annotation, ध्रुव वर्ण *कुञ्चिका,
                                    ध्रुव वर्ण **मानम्, libcrun_error_t *err, bool use_config_file)
{
  ध्रुव वर्ण *val;
  json_object *val_json = रिक्त;

  *मानम् = रिक्त;

  val = find_annotation (container, annotation);
  यदि (val != रिक्त)
    {
      *मानम् = val;
      प्रत्यागम 0;
    }

  यदि (! use_config_file || config_वृक्षः == रिक्त)
    प्रत्यागम 0;

  val_json = json_object_object_get (config_वृक्षः, कुञ्चिका);
  यदि (val_json == रिक्त)
    प्रत्यागम 0;

  यदि (! json_object_is_type (val_json, json_type_string))
    प्रत्यागम crun_make_error (err, 0, ".krun_vm.json %s value is not a string", कुञ्चिका);

  *मानम् = json_object_get_string (val_json);
  प्रत्यागम 0;
}

स्थिर शून्य
libkrun_make_tap_mac (ध्रुव वर्ण *id, uint8_t mac[6])
{
  uint64_t hash = 14695981039346656037ULL;
  size_t i;

  प्रति (; *id != '\0'; id++)
    {
      hash ^= (uint8_t) *id;
      hash *= 1099511628211ULL;
    }

  प्रति (i = 0; i < 6; i++)
    mac[i] = (uint8_t) (hash >> (i * 8));

  mac[0] = (mac[0] & 0xfe) | 0x02;
}

स्थिर पूर्णाङ्क
libkrun_configure_vm (uint32_t ctx_id, शून्य *handle, संरचना krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_समुच्चयः_vm_config) (uint32_t ctx_id, uint8_t अङ्कः_vcpus, uint32_t ram_mib);
  int32_t (*krun_सङ्कलय_net_unixstream) (uint32_t ctx_id, ध्रुव वर्ण *c_मार्गः, पूर्णाङ्क fd, uint8_t *ध्रुव c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_सङ्कलय_net_tap) (uint32_t ctx_id, ध्रुव वर्ण *c_tap_name, ध्रुव uint8_t *c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_समुच्चयः_gpu_options) (uint32_t ctx_id, uint32_t virgl_flags);
  int32_t (*krun_समुच्चयः_gpu_render_server_fd) (uint32_t ctx_id, पूर्णाङ्क fd);
  पूर्णाङ्क cpus, ram_mib, nested_virt, ret;
  cpu_समुच्चयः_t समुच्चयः;

  /* We let the OCI set the number of vCPUs for the VM, since cgroups CPU restrictions still apply. */
  cpus = libkrun_parse_resource_configuration (kconf->config_वृक्षः, container, "krun.cpus", "cpus", true);
  यदि (cpus <= 0)
    {
      CPU_ZERO (&समुच्चयः);
      यदि (sched_getaffinity (getpid (), परिमाण (समुच्चयः), &समुच्चयः) == 0)
        cpus = MIN (CPU_गणनम् (&समुच्चयः), LIBKRUN_MAX_VCPUS);
      अन्यथा
        cpus = 1;
    }

  /* We let the OCI set the amount of RAM for the VM, since cgroups memory restrictions still apply. */
  ram_mib = libkrun_parse_resource_configuration (kconf->config_वृक्षः, container, "krun.ram_mib", "ram_mib", true);
  यदि (ram_mib <= LIBKRUN_MINIMUM_RAM_MIB)
    {
      यदि (def && def->linux && def->linux->resources && def->linux->resources->स्मृतिः
          && def->linux->resources->स्मृतिः->सीमा_present)
        ram_mib = def->linux->resources->स्मृतिः->सीमा / (1024 * 1024);
      अन्यथा
        ram_mib = LIBKRUN_DEFAULT_RAM_MIB;
    }

  krun_समुच्चयः_vm_config = dlsym (handle, "krun_set_vm_config");

  यदि (krun_समुच्चयः_vm_config == रिक्त)
    प्रत्यागम crun_make_error (err, 0, "could not find symbol in the krun library");

  ret = krun_समुच्चयः_vm_config (ctx_id, cpus, ram_mib);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, -ret, "could not set krun vm configuration");

  nested_virt = libkrun_parse_resource_configuration (kconf->config_वृक्षः, container, "krun.nested_virt", "nested_virt", false);
  यदि (nested_virt > 0)
    {
      int32_t (*krun_check_nested_virt) (शून्य);
      int32_t (*krun_समुच्चयः_nested_virt) (uint32_t ctx_id, bool enabled);

      krun_check_nested_virt = dlsym (handle, "krun_check_nested_virt");
      यदि (krun_check_nested_virt != रिक्त && krun_check_nested_virt () != 1)
        libcrun_warning ("nested virtualization requested but may not be supported on this host");

      krun_समुच्चयः_nested_virt = dlsym (handle, "krun_set_nested_virt");
      यदि (krun_समुच्चयः_nested_virt == रिक्त)
        प्रत्यागम crun_make_error (err, 0, "could not find symbol `krun_set_nested_virt` in the krun library");

      ret = krun_समुच्चयः_nested_virt (ctx_id, true);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, -ret, "could not enable nested virtualization");
    }

  यदि (kconf->tap_name != रिक्त)
    {
      krun_सङ्कलय_net_tap = dlsym (handle, "krun_add_net_tap");
      यदि (krun_सङ्कलय_net_tap == रिक्त)
        प्रत्यागम crun_make_error (err, 0, "could not find symbol `krun_add_net_tap` in the krun library");

      uint8_t mac[6];
      libkrun_make_tap_mac (container->context->id, mac);
      ret = krun_सङ्कलय_net_tap (ctx_id, kconf->tap_name, &mac[0], COMPAT_NET_FEATURES, 0);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, -ret, "could not add krun TAP interface `%s`", kconf->tap_name);
    }
  अन्यथा यदि (kconf->use_passt)
    {
      krun_सङ्कलय_net_unixstream = dlsym (handle, "krun_add_net_unixstream");
      यदि (krun_सङ्कलय_net_unixstream == रिक्त)
        प्रत्यागम crun_make_error (err, 0, "could not find symbol `krun_add_net_unixstream` in the krun library");

      uint8_t mac[] = { 0x5a, 0x94, 0xef, 0xe4, 0x0c, 0xee };
      ret = krun_सङ्कलय_net_unixstream (ctx_id, रिक्त, kconf->passt_fds[FD_PAIR_PARENT], &mac[0], COMPAT_NET_FEATURES, NET_FLAG_DHCP_CLIENT);
      यदि (UNLIKELY (ret == -EINVAL))
        ret = krun_सङ्कलय_net_unixstream (ctx_id, रिक्त, kconf->passt_fds[FD_PAIR_PARENT], &mac[0], COMPAT_NET_FEATURES, 0);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, -ret, "could not set krun net configuration");
    }

  यदि (kconf->gpu_flags > 0)
    {
      krun_समुच्चयः_gpu_options = dlsym (kconf->handle, "krun_set_gpu_options");
      यदि (krun_समुच्चयः_gpu_options == रिक्त)
        प्रत्यागम crun_make_error (err, 0, "gpu requested but the version of libkrun in this system does not support it");

      ret = krun_समुच्चयः_gpu_options (kconf->ctx_id, kconf->gpu_flags);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, -ret, "gpu requested but could not configure virtio gpu device");

      यदि (kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER)
        {
          krun_समुच्चयः_gpu_render_server_fd = dlsym (kconf->handle, "krun_set_gpu_render_server_fd");
          यदि (krun_समुच्चयः_gpu_render_server_fd == रिक्त)
            प्रत्यागम crun_make_error (err, 0, "gpu with render server requested but the version of libkrun in this system does not support it");

          ret = krun_समुच्चयः_gpu_render_server_fd (kconf->ctx_id, kconf->gpu_render_server_fd);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, -ret, "gpu with render server requested but could not configure its file descriptor");
        }
    }

  यदि (kconf->config_वृक्षः != रिक्त)
    {
      /* Try to configure an external kernel. If the configuration file doesn't
       * specify a kernel, libkrun automatically fall back to using libkrunfw,
       * if the library is present and was loaded while creating the context.
       */
      पूर्णाङ्क custom_kernel = libkrun_parse_resource_configuration (kconf->config_वृक्षः, container, "krun.custom_kernel", "custom_kernel", false);
      यदि (custom_kernel > 0)
        {
          ret = libkrun_configure_kernel (ctx_id, handle, kconf->config_वृक्षः, err);
          यदि (UNLIKELY (ret))
            प्रत्यागम ret;
        }
    }

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
libkrun_configure_flavor (शून्य *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णाङ्क ret, sev_indicated = 0, awsnitro_indicated = 0;
  संरचना krun_config *kconf = (संरचना krun_config *) cookie;
  ध्रुव वर्ण *flavor = रिक्त;
  शून्य *close_handles[2];

  close_handles[0] = रिक्त;
  close_handles[1] = रिक्त;

  // Check if the user provided the krun variant through OCI annotations.
  flavor = find_annotation (container, "krun.variant");
  यदि (flavor != रिक्त)
    {
      sev_indicated |= वाक्यं_तुलय (flavor, KRUN_FLAVOR_SEV) == 0;
      awsnitro_indicated |= वाक्यं_तुलय (flavor, KRUN_FLAVOR_AWS_NITRO) == 0;
    }

  यदि (sev_indicated)
    {
      यदि (kconf->handle_sev == रिक्त)
        error (EXIT_FAILURE, 0, "the container requires libkrun-sev but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_awsnitro;

      kconf->handle = kconf->handle_sev;
      kconf->ctx_id = kconf->ctx_id_sev;
      kconf->sev = true;
    }
  अन्यथा यदि (awsnitro_indicated)
    {
      यदि (kconf->handle_awsnitro == रिक्त)
        error (EXIT_FAILURE, 0, "the container requires libkrun-awsnitro but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_sev;

      kconf->handle = kconf->handle_awsnitro;
      kconf->ctx_id = kconf->ctx_id_awsnitro;
      kconf->awsnitro = true;
    }
  अन्यथा
    {
      यदि (kconf->handle == रिक्त)
        error (EXIT_FAILURE, 0, "the container requires libkrun but it's not available");

      close_handles[0] = kconf->handle_sev;
      close_handles[1] = kconf->handle_awsnitro;
    }

  // We no longer need the other two libkrun handles.
  प्रति (पूर्णाङ्क i = 0; i < 2; i++)
    {
      यदि (close_handles[i] == रिक्त)
        अनुवर्त;

      ret = dlclose (close_handles[i]);
      यदि (UNLIKELY (ret != 0))
        प्रत्यागम crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
libkrun_exec (शून्य *cookie, libcrun_container_t *container, ध्रुव वर्ण *pathname, वर्ण *ध्रुव argv[])
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_समुच्चयः_log_level) (uint32_t level);
  पूर्णाङ्क (*krun_start_enter) (uint32_t ctx_id);
  int32_t (*krun_सङ्कलय_virtiofs2) (uint32_t ctx_id, ध्रुव वर्ण *c_चिह्नकम्, ध्रुव वर्ण *c_मार्गः, uint64_t shm_आकार);
  int32_t (*krun_समुच्चयः_मूलम्_disk) (uint32_t ctx_id, ध्रुव वर्ण *disk_मार्गः);
  int32_t (*krun_समुच्चयः_tee_config_file) (uint32_t ctx_id, ध्रुव वर्ण *file_मार्गः);
  int32_t (*krun_समुच्चयः_console_output) (uint32_t ctx_id, ध्रुव वर्ण *c_filepath);
  int32_t (*krun_समुच्चयः_exec) (uint32_t ctx_id, ध्रुव वर्ण *exec_मार्गः,
                            ध्रुव वर्ण *ध्रुव argv[], ध्रुव वर्ण *ध्रुव envp[]);
  संरचना krun_config *kconf = (संरचना krun_config *) cookie;
  शून्य *handle;
  int32_t ctx_id, ret;
  libcrun_error_t err;

  ret = libkrun_configure_flavor (cookie, container, &err);
  यदि (UNLIKELY (ret < 0))
    {
      पूर्णाङ्क errcode = crun_error_get_errno (&err);
      crun_error_release (&err);
      error (EXIT_FAILURE, errcode, "unable to configure libkrun flavor");
    }

  // /dev/kvm is required for all non AWS nitro workloads.
  यदि (! kconf->awsnitro && ! kconf->has_kvm)
    error (EXIT_FAILURE, 0, "`/dev/kvm` unavailable");

  handle = kconf->handle;
  ctx_id = kconf->ctx_id;

  krun_समुच्चयः_log_level = dlsym (handle, "krun_set_log_level");
  krun_start_enter = dlsym (handle, "krun_start_enter");
  यदि (krun_समुच्चयः_log_level == रिक्त || krun_start_enter == रिक्त)
    error (EXIT_FAILURE, 0, "could not find symbol in the krun library");

  /* Set log level according to crun's verbosity. */
  विकल्प (libcrun_get_verbosity ())
    {
    पक्ष LIBCRUN_VERBOSITY_DEBUG:
      krun_समुच्चयः_log_level (KRUN_LOG_LEVEL_DEBUG);
      भङ्ग;
    पक्ष LIBCRUN_VERBOSITY_WARNING:
      krun_समुच्चयः_log_level (KRUN_LOG_LEVEL_WARN);
      भङ्ग;
    स्वभाव:
      krun_समुच्चयः_log_level (KRUN_LOG_LEVEL_ERROR);
      भङ्ग;
    }

  यदि (kconf->sev)
    {
      krun_समुच्चयः_मूलम्_disk = dlsym (handle, "krun_set_root_disk");
      krun_समुच्चयः_tee_config_file = dlsym (handle, "krun_set_tee_config_file");
      यदि (krun_समुच्चयः_मूलम्_disk == रिक्त || krun_समुच्चयः_tee_config_file == रिक्त)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-sev.so`");

      ret = krun_समुच्चयः_मूलम्_disk (ctx_id, "/disk.img");
      यदि (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set root disk");

      ret = krun_समुच्चयः_tee_config_file (ctx_id, KRUN_SEV_FILE);
      यदि (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set krun tee config file");
    }
  अन्यथा
    {
      json_object *val_virtiofs_चिह्नकम् = रिक्त;
      json_object *val_virtiofs_shm_आकार = रिक्त;
      ध्रुव वर्ण *virtiofs_चिह्नकम् = रिक्त;
      // Default to a conservative DAX size of 512MB, just like krun_set_root() does.
      uint64_t virtiofs_shm_आकार = 512 * 1024 * 1024ULL;

      यदि (kconf->config_वृक्षः != रिक्त)
        {
          val_virtiofs_चिह्नकम् = json_object_object_get (kconf->config_वृक्षः, "virtiofs_tag");
          यदि (val_virtiofs_चिह्नकम् != रिक्त && json_object_is_type (val_virtiofs_चिह्नकम्, json_type_string))
            virtiofs_चिह्नकम् = json_object_get_string (val_virtiofs_चिह्नकम्);

          val_virtiofs_shm_आकार = json_object_object_get (kconf->config_वृक्षः, "virtiofs_shm_size");
          यदि (val_virtiofs_shm_आकार != रिक्त && json_object_is_type (val_virtiofs_shm_आकार, json_type_int))
            virtiofs_shm_आकार = json_object_get_uint64 (val_virtiofs_shm_आकार);
        }

      यदि (virtiofs_चिह्नकम् == रिक्त)
        virtiofs_चिह्नकम् = "/dev/root";

      krun_सङ्कलय_virtiofs2 = dlsym (handle, "krun_add_virtiofs2");

      यदि (krun_सङ्कलय_virtiofs2 == रिक्त)
        error (EXIT_FAILURE, 0, "could not find symbol `krun_add_virtiofs2` in `libkrun.so`");

      ret = krun_सङ्कलय_virtiofs2 (ctx_id, virtiofs_चिह्नकम्, "/", virtiofs_shm_आकार);
      यदि (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not add virtiofs root with tag `%s`", virtiofs_चिह्नकम्);
    }

  यदि (kconf->awsnitro)
    {
      krun_समुच्चयः_console_output = dlsym (handle, "krun_set_console_output");
      krun_समुच्चयः_exec = dlsym (handle, "krun_set_exec");
      यदि (krun_समुच्चयः_console_output == रिक्त || krun_समुच्चयः_exec == रिक्त)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-awsnitro.so`");

      // Redirect all enclave output (read from vsock) to stdout.
      ret = krun_समुच्चयः_console_output (ctx_id, "/dev/stdout");
      यदि (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not redirect enclave output to stdout");

      ret = krun_समुच्चयः_exec (ctx_id, pathname, (ध्रुव वर्ण *ध्रुव *) argv,
                           (ध्रुव वर्ण *ध्रुव *) def->process->env);
      यदि (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set enclave execution arguments");
    }

  ret = libkrun_configure_vm (ctx_id, handle, kconf, container, &err);
  यदि (UNLIKELY (ret))
    {
      पूर्णाङ्क errcode = crun_error_get_errno (&err);
      libcrun_error_t *tmp_err = &err;
      libcrun_error_report_and_release (tmp_err);
      error (EXIT_FAILURE, errcode, "could not configure krun vm");
    }

  json_object_put (kconf->config_doc);

  ret = krun_start_enter (ctx_id);
  यदि (UNLIKELY (ret < 0))
    error (EXIT_FAILURE, -ret, "could not start krun");

  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
libkrun_start_render_server (संरचना krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  kconf->gpu_flags = libkrun_parse_resource_configuration (kconf->config_वृक्षः, container, "krun.gpu_flags", "gpu_flags", false);
  यदि (kconf->gpu_flags > 0)
    {
      यदि (access ("/dev/dri", F_OK) != 0)
        प्रत्यागम crun_make_error (err, errno, "gpu requested but /dev/dri is not available");

      यदि ((kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER) != 0)
        {
          वर्ण *rs_argv[40];
          पूर्णाङ्क rs_argc = 0;
          वर्ण fd_as_str[16];
          पूर्णाङ्क gpu_fds[2];
          pid_t pid;
          पूर्णाङ्क null;

          यदि (access ("/usr/libexec/virgl_render_server", X_OK) != 0)
            प्रत्यागम crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/libexec/virgl_render_server binary");

          यदि (access ("/usr/bin/bwrap", X_OK) != 0)
            प्रत्यागम crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/bin/bwrap binary");

          ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, gpu_fds);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "could not create socketpair for virgl_render_server");

          snprintf (fd_as_str, परिमाण (fd_as_str), "%d", gpu_fds[FD_PAIR_CHILD]);

          rs_argv[rs_argc++] = (वर्ण *) "bwrap";
          rs_argv[rs_argc++] = (वर्ण *) "--ro-bind";
          rs_argv[rs_argc++] = (वर्ण *) "/usr";
          rs_argv[rs_argc++] = (वर्ण *) "/usr";
          यदि (access ("/lib", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (वर्ण *) "--ro-bind";
              rs_argv[rs_argc++] = (वर्ण *) "/lib";
              rs_argv[rs_argc++] = (वर्ण *) "/lib";
            }
          यदि (access ("/lib64", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (वर्ण *) "--ro-bind";
              rs_argv[rs_argc++] = (वर्ण *) "/lib64";
              rs_argv[rs_argc++] = (वर्ण *) "/lib64";
            }
          rs_argv[rs_argc++] = (वर्ण *) "--ro-bind";
          rs_argv[rs_argc++] = (वर्ण *) "/etc";
          rs_argv[rs_argc++] = (वर्ण *) "/etc";
          rs_argv[rs_argc++] = (वर्ण *) "--ro-bind";
          rs_argv[rs_argc++] = (वर्ण *) "/sys";
          rs_argv[rs_argc++] = (वर्ण *) "/sys";
          rs_argv[rs_argc++] = (वर्ण *) "--dev";
          rs_argv[rs_argc++] = (वर्ण *) "/dev";
          rs_argv[rs_argc++] = (वर्ण *) "--dev-bind";
          rs_argv[rs_argc++] = (वर्ण *) "/dev/dri";
          rs_argv[rs_argc++] = (वर्ण *) "/dev/dri";
          rs_argv[rs_argc++] = (वर्ण *) "--proc";
          rs_argv[rs_argc++] = (वर्ण *) "/proc";
          rs_argv[rs_argc++] = (वर्ण *) "--tmpfs";
          rs_argv[rs_argc++] = (वर्ण *) "/tmp";
          rs_argv[rs_argc++] = (वर्ण *) "--unshare-pid";
          rs_argv[rs_argc++] = (वर्ण *) "--unshare-ipc";
          rs_argv[rs_argc++] = (वर्ण *) "--unshare-net";
          rs_argv[rs_argc++] = (वर्ण *) "--";
          rs_argv[rs_argc++] = (वर्ण *) "/usr/libexec/virgl_render_server";
          rs_argv[rs_argc++] = (वर्ण *) "--socket-fd";
          rs_argv[rs_argc++] = fd_as_str;
          rs_argv[rs_argc] = रिक्त;

          pid = fork ();
          यदि (pid < 0)
            {
              close (gpu_fds[FD_PAIR_PARENT]);
              close (gpu_fds[FD_PAIR_CHILD]);
              प्रत्यागम crun_make_error (err, errno, "could not fork for virgl_render_server");
            }
          अन्यथा यदि (pid == 0)
            {
              close (gpu_fds[FD_PAIR_PARENT]);

              null = उद्घाटय ("/dev/null", O_WRONLY);
              यदि (null == -1)
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

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
libkrun_configure_जालम् (शून्य *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  संरचना krun_config *kconf = (संरचना krun_config *) cookie;
  pid_t pid;
  वर्ण *passt_argv[10];
  वर्ण fd_as_str[16];
  पूर्णाङ्क use_passt;
  पूर्णाङ्क argv_idx;
  पूर्णाङ्क स्थितिः;
  पूर्णाङ्क null;
  पूर्णाङ्क ret;

  ret = libkrun_parse_string_configuration (kconf->config_वृक्षः, container,
                                            "krun.tap_name", "tap_name",
                                            &kconf->tap_name, err, false);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (kconf->tap_name != रिक्त && is_रिक्तानि_string (kconf->tap_name))
    प्रत्यागम crun_make_error (err, 0, "krun.tap_name cannot be empty");

  use_passt = libkrun_parse_resource_configuration (kconf->config_वृक्षः, container, "krun.use_passt", "use_passt", false);

  यदि (use_passt > 0)
    {
      यदि (kconf->tap_name != रिक्त)
        प्रत्यागम crun_make_error (err, 0, "krun.tap_name and krun.use_passt are mutually exclusive");

      kconf->use_passt = 1;
    }
  अन्यथा
    प्रत्यागम 0;

  ret = socketpair (AF_UNIX, SOCK_प्रवाहः, 0, kconf->passt_fds);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "create passt socketpair");

  snprintf (fd_as_str, परिमाण (fd_as_str), "%d", kconf->passt_fds[FD_PAIR_CHILD]);

  argv_idx = 0;
  passt_argv[argv_idx++] = (वर्ण *) "passt";
  passt_argv[argv_idx++] = (वर्ण *) "-t";
  passt_argv[argv_idx++] = (वर्ण *) "all";

  यदि (! kconf->has_awsnitro)
    {
      passt_argv[argv_idx++] = (वर्ण *) "-u";
      passt_argv[argv_idx++] = (वर्ण *) "all";
      passt_argv[argv_idx++] = (वर्ण *) "--no-dhcp-dns";
    }

  /* Set --no-map-gw. If the gateway for the network is also the DNS server
   * we won't be able to query the DNS server without --no-map-gw.
   * See discussion in https://github.com/containers/crun/pull/2099
   */
  passt_argv[argv_idx++] = (वर्ण *) "--no-map-gw";

  passt_argv[argv_idx++] = (वर्ण *) "--fd";
  passt_argv[argv_idx++] = fd_as_str;
  passt_argv[argv_idx] = रिक्त;

  pid = fork ();
  यदि (pid < 0)
    प्रत्यागम crun_make_error (err, errno, "fork passt");
  अन्यथा यदि (pid == 0)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t i;

      close (kconf->passt_fds[FD_PAIR_PARENT]);

      null = उद्घाटय ("/dev/null", O_WRONLY);
      यदि (null == -1)
        _exit (EXIT_FAILURE);

      /* Remount /proc if the parent detached its PID namespace, as passt(1)
       * needs a consistent /proc view to operate properly.
       */
      प्रति (i = 0; i < def->linux->namespaces_len; i++)
        {
          पूर्णाङ्क मानम् = libcrun_find_namespace (def->linux->namespaces[i]->type);

          यदि (मानम् == CLONE_NEWPID)
            {
              यदि (mount ("", "/proc", "proc", MS_NOSUID | MS_NODEV | MS_NOEXEC, रिक्त))
                _exit (EXIT_FAILURE);

              भङ्ग;
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
  waitpid (pid, &स्थितिः, 0);
  यदि (! (WIFEXITED (स्थितिः)) || WEXITSTATUS (स्थितिः) != 0)
    प्रत्यागम crun_make_error (err, 0, "start passt");

  प्रत्यागम 0;
}

/* Return true if the spec already declares a device with the given PATH.  */
स्थिर bool
spec_has_device (runtime_spec_schema_config_schema *def, ध्रुव वर्ण *मार्गः)
{
  size_t i;

  प्रति (i = 0; i < def->linux->devices_len; i++)
    यदि (वाक्यं_तुलय (def->linux->devices[i]->मार्गः, मार्गः) == 0)
      प्रत्यागम true;
  प्रत्यागम false;
}

/* libkrun_create_kvm_device: explicitly adds kvm device.  */
स्थिर पूर्णाङ्क
libkrun_configure_container (शून्य *cookie, गणना handler_configure_phase phase,
                             libcrun_context_t *context, libcrun_container_t *container,
                             ध्रुव वर्ण *rootfs, libcrun_error_t *err)
{
  पूर्णाङ्क ret, rootfsfd;
  संरचना krun_config *kconf = (संरचना krun_config *) cookie;
  संरचना device_s kvm_device = { "/dev/kvm", "c", 10, 232, 0666, 0, 0 };
  संरचना device_s sev_device = { "/dev/sev", "c", 10, 124, 0666, 0, 0 };
  संरचना device_s nitro_device = { "/dev/nitro_enclaves", "c", 10, 122, 0666, 0, 0 };
  cleanup_close पूर्णाङ्क devfd = -1;
  cleanup_close पूर्णाङ्क rootfsfd_cleanup = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  bool create_sev = false, create_awsnitro = false;
  bool is_user_ns;

  यदि (rootfs == रिक्त)
    rootfsfd = AT_FDCWD;
  अन्यथा
    {
      rootfsfd = rootfsfd_cleanup = उद्घाटय (rootfs, O_मार्गः | O_CLOEXEC);
      यदि (UNLIKELY (rootfsfd < 0))
        प्रत्यागम crun_make_error (err, errno, "open `%s`", rootfs);
    }

  यदि (phase == HANDLER_CONFIGURE_BEFORE_USERNS)
    {
      cleanup_free वर्ण *origin_config_मार्गः = रिक्त;
      cleanup_free वर्ण *state_dir = रिक्त;
      cleanup_free वर्ण *config = रिक्त;
      cleanup_close पूर्णाङ्क fd = -1;
      size_t config_आकार;

      ret = libcrun_get_state_directory (&state_dir, context->state_मूलम्, context->id, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = append_paths (&origin_config_मार्गः, err, state_dir, "config.json", रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = read_all_file (origin_config_मार्गः, &config, &config_आकार, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      /* CVE-2025-24965: the content below rootfs cannot be trusted because it is controlled by the user.  We
         must ensure the file is opened below the rootfs directory.  Whatever the image ships at that name
         could also be a device node or a FIFO, so never open it: unlink it and create a new regular file.  */
      ret = unlinkat (rootfsfd, KRUN_CONFIG_FILE, 0);
      यदि (UNLIKELY (ret < 0 && errno != ENOENT))
        प्रत्यागम crun_make_error (err, errno, "unlink `%s`", KRUN_CONFIG_FILE);

      fd = safe_openat (rootfsfd, rootfs, KRUN_CONFIG_FILE, WRITE_FILE_DEFAULT_FLAGS | O_EXCL | O_NOFOLLOW, S_IRUSR | S_IRGRP | S_IROTH, err);
      यदि (UNLIKELY (fd < 0))
        प्रत्यागम fd;

      ret = safe_write (fd, KRUN_CONFIG_FILE, config, config_आकार, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = libkrun_read_vm_config (kconf, rootfsfd, rootfs, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = libkrun_start_render_server (kconf, container, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  यदि (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    प्रत्यागम 0;

  ret = libkrun_configure_जालम् (cookie, container, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* Do nothing if /dev/kvm is already present in spec */
  यदि (spec_has_device (def, "/dev/kvm"))
    प्रत्यागम 0;

  यदि (kconf->handle_sev != रिक्त)
    create_sev = ! spec_has_device (def, "/dev/sev");

  यदि (kconf->handle_awsnitro != रिक्त)
    create_awsnitro = ! spec_has_device (def, "/dev/nitro_enclaves");

  devfd = safe_openat (rootfsfd, rootfs, "dev", O_मार्गः | O_DIRECTORY | O_CLOEXEC, 0, err);
  यदि (UNLIKELY (devfd < 0))
    प्रत्यागम devfd;

  ret = check_running_अन्तर्_user_namespace (err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;
  is_user_ns = ret;

  यदि (kconf->has_kvm)
    {
      ret = libcrun_create_dev (container, devfd, -1, &kvm_device, is_user_ns, true, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  यदि (create_sev)
    {
      ret = libcrun_create_dev (container, devfd, -1, &sev_device, is_user_ns, true, err);
      यदि (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_sev);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
          kconf->handle_sev = रिक्त;
        }
    }

  यदि (create_awsnitro)
    {
      ret = libcrun_create_dev (container, devfd, -1, &nitro_device, is_user_ns, true, err);
      यदि (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_awsnitro);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
          kconf->handle_awsnitro = रिक्त;
        }
    }

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
libkrun_आरोपय (शून्य **cookie, libcrun_error_t *err)
{
  int32_t ret;
  संरचना krun_config *kconf;
  ध्रुव वर्ण *libkrun_so = "libkrun.so.1";
  ध्रुव वर्ण *libkrun_sev_so = "libkrun-sev.so.1";
  ध्रुव वर्ण *libkrun_awsnitro_so = "libkrun-awsnitro.so.1";

  kconf = स्मृतिं_देहि (परिमाण (संरचना krun_config));
  यदि (kconf == रिक्त)
    प्रत्यागम crun_make_error (err, 0, "could not allocate memory for krun_config");
  स्मृतिं_पूरय (kconf, 0, परिमाण (संरचना krun_config));

  kconf->gpu_render_server_fd = -1;

  kconf->handle = dlopen (libkrun_so, RTLD_NOW);
  kconf->handle_sev = dlopen (libkrun_sev_so, RTLD_NOW);
  kconf->handle_awsnitro = dlopen (libkrun_awsnitro_so, RTLD_NOW);

  यदि (kconf->handle == रिक्त && kconf->handle_sev == रिक्त && kconf->handle_awsnitro == रिक्त)
    {
      मुक्त (kconf);
      प्रत्यागम crun_make_error (err, 0, "failed to open `%s`, `%s`, and `%s` for krun_config: %s", libkrun_so, libkrun_sev_so, libkrun_awsnitro_so, dlerror ());
    }

  kconf->sev = false;
  kconf->awsnitro = false;

  /* Newer versions of libkrun no longer link against libkrunfw and
     instead they open it when creating the context. This implies
     we need to call "krun_create_ctx" before switching namespaces
     or it won't be able to find the library bundling the kernel. */
  यदि (kconf->handle)
    {
      ret = libkrun_create_context (kconf->handle, err);
      यदि (UNLIKELY (ret < 0))
        गच्छ error;
      kconf->ctx_id = ret;
    }

  यदि (kconf->handle_sev)
    {
      ret = libkrun_create_context (kconf->handle_sev, err);
      यदि (UNLIKELY (ret < 0))
        गच्छ error;
      kconf->ctx_id_sev = ret;
    }
  यदि (kconf->handle_awsnitro)
    {
      ret = libkrun_create_context (kconf->handle_awsnitro, err);
      यदि (UNLIKELY (ret < 0))
        गच्छ error;
      kconf->ctx_id_awsnitro = ret;
    }

  *cookie = kconf;

  प्रत्यागम 0;

error:
  यदि (kconf->handle)
    dlclose (kconf->handle);
  यदि (kconf->handle_sev)
    dlclose (kconf->handle_sev);
  यदि (kconf->handle_awsnitro)
    dlclose (kconf->handle_awsnitro);
  मुक्त (kconf);
  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
libkrun_unload (शून्य *cookie, libcrun_error_t *err)
{
  पूर्णाङ्क r;

  संरचना krun_config *kconf = (संरचना krun_config *) cookie;
  यदि (kconf != रिक्त)
    {
      यदि (kconf->handle != रिक्त)
        {
          r = dlclose (kconf->handle);
          यदि (UNLIKELY (r != 0))
            प्रत्यागम crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
        }
      यदि (kconf->handle_sev != रिक्त)
        {
          r = dlclose (kconf->handle_sev);
          यदि (UNLIKELY (r != 0))
            प्रत्यागम crun_make_error (err, 0, "could not unload handle_sev: `%s`", dlerror ());
        }
      यदि (kconf->handle_awsnitro != रिक्त)
        {
          r = dlclose (kconf->handle_awsnitro);
          यदि (UNLIKELY (r != 0))
            प्रत्यागम crun_make_error (err, 0, "could not unload handle_awsnitro: `%s`", dlerror ());
        }
      मुक्त (kconf);
    }
  प्रत्यागम 0;
}

स्थिर runtime_spec_schema_defs_linux_device_cgroup *
make_oci_spec_dev (ध्रुव वर्ण *type, dev_t device, bool allow, ध्रुव वर्ण *access)
{
  runtime_spec_schema_defs_linux_device_cgroup *dev = xmalloc0 (परिमाण (*dev));

  dev->allow = allow;
  dev->allow_present = 1;

  dev->type = xstrdup (type);

  dev->major = major (device);
  dev->major_present = 1;

  dev->minor = minor (device);
  dev->minor_present = 1;

  dev->access = xstrdup (access);

  प्रत्यागम dev;
}

/* stat an optional device: on ENOENT clear *PRESENT, any other error fails.  */
स्थिर पूर्णाङ्क
stat_optional_device (ध्रुव वर्ण *मार्गः, संरचना stat *st, bool *present, libcrun_error_t *err)
{
  पूर्णाङ्क ret = stat (मार्गः, st);
  यदि (UNLIKELY (ret < 0))
    {
      यदि (errno != ENOENT)
        प्रत्यागम crun_make_error (err, errno, "stat `%s`", मार्गः);
      *present = false;
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
libkrun_modify_oci_configuration (शून्य *cookie arg_अप्रयुक्तम्, libcrun_context_t *context arg_अप्रयुक्तम्,
                                  runtime_spec_schema_config_schema *def,
                                  libcrun_error_t *err)
{
  ध्रुव size_t device_आकार = परिमाण (runtime_spec_schema_defs_linux_device_cgroup);
  संरचना krun_config *kconf = (संरचना krun_config *) cookie;
  संरचना stat st_kvm, st_sev, st_awsnitro;
  bool has_kvm = true, has_sev = true, has_awsnitro = true;
  size_t old_len, new_len;
  पूर्णाङ्क ret;

  /* Always allow the /dev/kvm device.  */

  ret = stat_optional_device ("/dev/kvm", &st_kvm, &has_kvm, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = stat_optional_device ("/dev/sev", &st_sev, &has_sev, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = stat_optional_device ("/dev/nitro_enclaves", &st_awsnitro, &has_awsnitro, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  kconf->has_kvm = has_kvm;
  kconf->has_awsnitro = has_awsnitro;

  यदि (! has_kvm && ! has_awsnitro)
    प्रत्यागम 0;

  /* spec says these are optional, ensure they exist so we can add our devices */
  यदि (def->linux == रिक्त)
    def->linux = xmalloc0 (परिमाण (runtime_spec_schema_config_linux));

  यदि (def->linux->resources == रिक्त)
    def->linux->resources = xmalloc0 (परिमाण (runtime_spec_schema_config_linux_resources));

  old_len = def->linux->resources->devices_len;
  new_len = old_len;
  यदि (has_kvm)
    new_len += has_sev ? 2 : 1;
  यदि (has_awsnitro)
    new_len += 1;

  def->linux->resources->devices = xrealloc (def->linux->resources->devices, device_आकार * (new_len + 1));
  def->linux->resources->devices_len = new_len;

  यदि (has_kvm)
    {
      def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_kvm.st_rdev, true, "rwm");
      यदि (has_sev)
        def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_sev.st_rdev, true, "rwm");
    }

  यदि (has_awsnitro)
    def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_awsnitro.st_rdev, true, "rwm");

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
libkrun_close_fds (शून्य *cookie, libcrun_container_t *container, पूर्णाङ्क preserve_fds, libcrun_error_t *err)
{
  संरचना krun_config *kconf = (संरचना krun_config *) cookie;
  पूर्णाङ्क first_fd_गन्तव्यम्_close = preserve_fds + 3;
  पूर्णाङ्क नीचः_fd, उच्चः_fd;
  पूर्णाङ्क i;

  नीचः_fd = उच्चः_fd = -1;

  यदि (kconf->use_passt)
    उच्चः_fd = kconf->passt_fds[FD_PAIR_PARENT];

  यदि (kconf->gpu_render_server_fd != -1)
    {
      यदि (kconf->gpu_render_server_fd > उच्चः_fd)
        {
          नीचः_fd = उच्चः_fd;
          उच्चः_fd = kconf->gpu_render_server_fd;
        }
      अन्यथा
        नीचः_fd = kconf->gpu_render_server_fd;
    }

  यदि (first_fd_गन्तव्यम्_close <= उच्चः_fd)
    {
      प्रति (i = first_fd_गन्तव्यम्_close; i < उच्चः_fd; i++)
        {
          // If we're closing proc_fd, make sure to invalidate it.
          यदि (i == container->proc_fd)
            container->proc_fd = -1;
          अन्यथा यदि (i == नीचः_fd)
            अनुवर्त;
          close (i);
        }

      first_fd_गन्तव्यम्_close = उच्चः_fd + 1;
    }

  प्रत्यागम mark_or_close_fds_ge_than (container, first_fd_गन्तव्यम्_close, true, err);
}

संरचना custom_handler_s handler_libkrun = {
  .name = "krun",
  .alias = रिक्त,
  .feature_string = "LIBKRUN",
  .supports_open_वृक्षः_namespace = false,
  .आरोपय = libkrun_आरोपय,
  .unload = libkrun_unload,
  .सततम्_func = libkrun_exec,
  .configure_container = libkrun_configure_container,
  .modify_oci_configuration = libkrun_modify_oci_configuration,
  .close_fds = libkrun_close_fds,
};

#यदि_अन्तः
