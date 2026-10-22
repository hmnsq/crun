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
#నిర్వచించు GNU_ఆధారం

#అన్తర్భావ <config.h>
#అన్తర్భావ "../custom-handler.h"
#అన్తర్భావ "../container.h"
#అన్తర్భావ "../utils.h"
#అన్తర్భావ "../linux.h"
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <sys/socket.h>
#అన్తర్భావ <errno.h>
#అన్తర్భావ <sys/mount.h>
#అన్తర్భావ <sys/param.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <sys/sysmacros.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <sched.h>
#అన్తర్భావ <ocispec/runtime_spec_schema_config_schema.h>

#నిర్వచితమైతే HAVE_DLOPEN
#  include <dlfcn.h>
#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_LIBKRUN
#  include <libkrun.h>
#అయితే_ముగింపు

/* This allows us to build even with headers from older libkrun versions.
 * If the installed version of libkrun doesn't support this feature,
 * krun_add_net_unixstream() will return EINVAL.
 */
#నిర్వచితం_కాకపోతే NET_FLAG_DHCP_CLIENT
#  define NET_FLAG_DHCP_CLIENT (1 << 1)
#అయితే_ముగింపు
/* libkrun has a hard-limit of 16 vCPUs per microVM. */
#నిర్వచించు LIBKRUN_MAX_VCPUS 16

/* If the user doesn't configure the RAM amount, fallback to this value. */
#నిర్వచించు LIBKRUN_DEFAULT_RAM_MIB 1024

/* The minimum amount of RAM for a viable microVM is 128 MB. */
#నిర్వచించు LIBKRUN_MINIMUM_RAM_MIB 128

/* crun dumps the container configuration into this file, which will be read by
 * libkrun to set up the environment for the workload inside the microVM.
 */
#నిర్వచించు KRUN_CONFIG_FILE ".krun_config.json"

/* The presence of this file indicates this is a container intended to be run
 * as a confidential workload inside a SEV-powered TEE.
 */
#నిర్వచించు KRUN_SEV_FILE "/krun-sev.json"

/* This file contains configuration parameters for the microVM. crun needs to
 * read and parse it, using the information obtained from it to configure
 * libkrun as required.
 */
#నిర్వచించు KRUN_VM_FILE "/.krun_vm.json"

/* KRUN_VM_FILE holds a handful of settings and it is parsed by crun
 * itself before the container process exists, refuse anything bigger.
 */
#నిర్వచించు KRUN_VM_FILE_MAX_కొలత (64 * 1024)

#నిర్వచించు KRUN_FLAVOR_AWS_NITRO "aws-nitro"
#నిర్వచించు KRUN_FLAVOR_SEV "sev"

#నిర్వచించు FD_PAIR_మాతృ 0
#నిర్వచించు FD_PAIR_బిడ్డ 1

నిర్మాణం krun_config
{
  శూన్య *handle;
  శూన్య *handle_sev;
  శూన్య *handle_awsnitro;
  bool sev;
  bool awsnitro;
  int32_t ctx_id;
  int32_t ctx_id_sev;
  int32_t ctx_id_awsnitro;
  bool has_kvm;
  bool has_awsnitro;
  పూర్ణాంకం passt_fds[2];
  json_object *config_doc;
  json_object *config_వృక్షం;
  bool use_passt;
  స్థిరరాశి అక్షరం *tap_name;
  పూర్ణాంకం gpu_render_server_fd;
  పూర్ణాంకం gpu_flags;
};

/* libkrun handler.  */
#ఒకవేళ HAVE_DLOPEN && HAVE_LIBKRUN
స్థిర int32_t
libkrun_create_context (శూన్య *handle, libcrun_error_t *err)
{
  int32_t (*krun_create_ctx) ();
  int32_t ctx_id;

  krun_create_ctx = dlsym (handle, "krun_create_ctx");
  ఒకవేళ (krun_create_ctx == ఖాళీ)
    తిరిగి crun_make_error (err, 0, "could not find symbol in the krun library");

  ctx_id = krun_create_ctx ();
  ఒకవేళ (UNLIKELY (ctx_id < 0))
    తిరిగి crun_make_error (err, -ctx_id, "could not create krun context");

  తిరిగి ctx_id;
}

స్థిర పూర్ణాంకం
libkrun_configure_kernel (uint32_t ctx_id, శూన్య *handle, json_object *config_వృక్షం, libcrun_error_t *err)
{
  int32_t (*krun_సమితి_kernel) (uint32_t ctx_id, స్థిరరాశి అక్షరం *kernel_మార్గం,
                              uint32_t kernel_format, స్థిరరాశి అక్షరం *initrd_మార్గం, స్థిరరాశి అక్షరం *kernel_cmdline);
  json_object *kernel_మార్గం = ఖాళీ;
  json_object *kernel_format = ఖాళీ;
  json_object *val_initrd_మార్గం = ఖాళీ;
  json_object *val_kernel_cmdline = ఖాళీ;
  స్థిరరాశి అక్షరం *initrd_మార్గం = ఖాళీ;
  స్థిరరాశి అక్షరం *kernel_cmdline = ఖాళీ;
  పూర్ణాంకం ret;

  /* kernel_path and kernel_format must be present */
  kernel_మార్గం = json_object_object_get (config_వృక్షం, "kernel_path");
  ఒకవేళ (kernel_మార్గం == ఖాళీ || ! json_object_is_type (kernel_మార్గం, json_type_string))
    తిరిగి 0;

  kernel_format = json_object_object_get (config_వృక్షం, "kernel_format");
  ఒకవేళ (kernel_format == ఖాళీ || ! json_object_is_type (kernel_format, json_type_int))
    తిరిగి 0;

  /* initrd and kernel_cmdline are optional */
  val_initrd_మార్గం = json_object_object_get (config_వృక్షం, "initrd_path");
  ఒకవేళ (val_initrd_మార్గం != ఖాళీ && json_object_is_type (val_initrd_మార్గం, json_type_string))
    initrd_మార్గం = json_object_get_string (val_initrd_మార్గం);

  val_kernel_cmdline = json_object_object_get (config_వృక్షం, "kernel_cmdline");
  ఒకవేళ (val_kernel_cmdline != ఖాళీ && json_object_is_type (val_kernel_cmdline, json_type_string))
    kernel_cmdline = json_object_get_string (val_kernel_cmdline);

  krun_సమితి_kernel = dlsym (handle, "krun_set_kernel");
  ఒకవేళ (krun_సమితి_kernel == ఖాళీ)
    తిరిగి crun_make_error (err, 0, "could not find symbol in krun library");

  ret = krun_సమితి_kernel (ctx_id,
                         json_object_get_string (kernel_మార్గం),
                         (uint32_t) json_object_get_int64 (kernel_format),
                         initrd_మార్గం, kernel_cmdline);

  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, -ret, "could not configure a krun external kernel");

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
libkrun_read_vm_config (నిర్మాణం krun_config *kconf, పూర్ణాంకం rootfsfd, స్థిరరాశి అక్షరం *rootfs, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  cleanup_free అక్షరం *config = ఖాళీ;
  cleanup_close పూర్ణాంకం pathfd = -1;
  cleanup_close పూర్ణాంకం fd = -1;
  proc_fd_మార్గం_t fd_మార్గం;
  నిర్మాణం stat st;

  /* The content of the rootfs is controlled by the user: look at the file through an
     O_PATH descriptor first and refuse anything that is not a small regular file, as
     opening a FIFO blocks and a device node must not be accessed.  */
  pathfd = సురక్షిత_openat (rootfsfd, rootfs, KRUN_VM_FILE, O_మార్గం | O_CLOEXEC | O_NOFOLLOW, 0, err);
  ఒకవేళ (pathfd < 0)
    {
      // The configuration file is optional, don't generate an error if it's missing.
      ఒకవేళ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          తిరిగి 0;
        }
      తిరిగి pathfd;
    }

  ret = fstat (pathfd, &st);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "stat `%s`", KRUN_VM_FILE);

  ఒకవేళ (! S_ISREG (st.st_mode))
    తిరిగి crun_make_error (err, 0, "`%s` is not a regular file", KRUN_VM_FILE);

  get_proc_self_fd_మార్గం (fd_మార్గం, pathfd);

  fd = open (fd_మార్గం, O_RDONLY | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి crun_make_error (err, errno, "open `%s`", KRUN_VM_FILE);

  ret = read_all_fd_with_max_కొలత (fd, KRUN_VM_FILE, &config, ఖాళీ, KRUN_VM_FILE_MAX_కొలత, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = parse_json_file (&kconf->config_doc, config, ఖాళీ, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  kconf->config_వృక్షం = kconf->config_doc;
  తిరిగి 0;
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
స్థిర పూర్ణాంకం
libkrun_parse_resource_configuration (json_object *config_వృక్షం, libcrun_container_t *container, స్థిరరాశి అక్షరం *annotation, స్థిరరాశి అక్షరం *కీలకం, bool use_config_file)
{
  అక్షరం *val_str, *endptr;
  పూర్ణాంకం val = -1;
  json_object *val_json = ఖాళీ;

  val_str = (అక్షరం *) కనుగొను_annotation (container, annotation);
  ఒకవేళ (val_str != ఖాళీ)
    {
      errno = 0;
      val = (పూర్ణాంకం) strtol (val_str, &endptr, 10);
      ఒకవేళ (errno != 0 || endptr == val_str || *endptr != '\0')
        /* Annotations value is not a valid integer. */
        error (EXIT_FAILURE, 0, "krun annotation %s value cannot be converted to an integer", annotation);

      ఒకవేళ (val < 0)
        error (EXIT_FAILURE, 0, "krun annotation %s value must be a positive integer", annotation);

      తిరిగి val;
    }
  లేకపోతే ఒకవేళ (use_config_file && config_వృక్షం != ఖాళీ)
    {
      val_json = json_object_object_get (config_వృక్షం, కీలకం);
      ఒకవేళ (val_json == ఖాళీ)
        తిరిగి val;
      ఒకవేళ (! json_object_is_type (val_json, json_type_int))
        error (EXIT_FAILURE, 0, "krun krun_vm.json %s value is not an integer", కీలకం);

      val = (పూర్ణాంకం) json_object_get_int64 (val_json);
    }

  తిరిగి val;
}

స్థిర పూర్ణాంకం
libkrun_parse_string_configuration (json_object *config_వృక్షం, libcrun_container_t *container,
                                    స్థిరరాశి అక్షరం *annotation, స్థిరరాశి అక్షరం *కీలకం,
                                    స్థిరరాశి అక్షరం **విలువ, libcrun_error_t *err, bool use_config_file)
{
  స్థిరరాశి అక్షరం *val;
  json_object *val_json = ఖాళీ;

  *విలువ = ఖాళీ;

  val = కనుగొను_annotation (container, annotation);
  ఒకవేళ (val != ఖాళీ)
    {
      *విలువ = val;
      తిరిగి 0;
    }

  ఒకవేళ (! use_config_file || config_వృక్షం == ఖాళీ)
    తిరిగి 0;

  val_json = json_object_object_get (config_వృక్షం, కీలకం);
  ఒకవేళ (val_json == ఖాళీ)
    తిరిగి 0;

  ఒకవేళ (! json_object_is_type (val_json, json_type_string))
    తిరిగి crun_make_error (err, 0, ".krun_vm.json %s value is not a string", కీలకం);

  *విలువ = json_object_get_string (val_json);
  తిరిగి 0;
}

స్థిర శూన్య
libkrun_make_tap_mac (స్థిరరాశి అక్షరం *id, uint8_t mac[6])
{
  uint64_t hash = 14695981039346656037ULL;
  size_t i;

  కొరకు (; *id != '\0'; id++)
    {
      hash ^= (uint8_t) *id;
      hash *= 1099511628211ULL;
    }

  కొరకు (i = 0; i < 6; i++)
    mac[i] = (uint8_t) (hash >> (i * 8));

  mac[0] = (mac[0] & 0xfe) | 0x02;
}

స్థిర పూర్ణాంకం
libkrun_configure_vm (uint32_t ctx_id, శూన్య *handle, నిర్మాణం krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_సమితి_vm_config) (uint32_t ctx_id, uint8_t అంకె_vcpus, uint32_t ram_mib);
  int32_t (*krun_కూడు_net_unixstream) (uint32_t ctx_id, స్థిరరాశి అక్షరం *c_మార్గం, పూర్ణాంకం fd, uint8_t *స్థిరరాశి c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_కూడు_net_tap) (uint32_t ctx_id, స్థిరరాశి అక్షరం *c_tap_name, స్థిరరాశి uint8_t *c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_సమితి_gpu_options) (uint32_t ctx_id, uint32_t virgl_flags);
  int32_t (*krun_సమితి_gpu_render_server_fd) (uint32_t ctx_id, పూర్ణాంకం fd);
  పూర్ణాంకం cpus, ram_mib, nested_virt, ret;
  cpu_సమితి_t సమితి;

  /* We let the OCI set the number of vCPUs for the VM, since cgroups CPU restrictions still apply. */
  cpus = libkrun_parse_resource_configuration (kconf->config_వృక్షం, container, "krun.cpus", "cpus", true);
  ఒకవేళ (cpus <= 0)
    {
      CPU_ZERO (&సమితి);
      ఒకవేళ (sched_getaffinity (getpid (), పరిమాణం (సమితి), &సమితి) == 0)
        cpus = MIN (CPU_లెక్క (&సమితి), LIBKRUN_MAX_VCPUS);
      లేకపోతే
        cpus = 1;
    }

  /* We let the OCI set the amount of RAM for the VM, since cgroups memory restrictions still apply. */
  ram_mib = libkrun_parse_resource_configuration (kconf->config_వృక్షం, container, "krun.ram_mib", "ram_mib", true);
  ఒకవేళ (ram_mib <= LIBKRUN_MINIMUM_RAM_MIB)
    {
      ఒకవేళ (def && def->linux && def->linux->resources && def->linux->resources->జ్ఞాపకం
          && def->linux->resources->జ్ఞాపకం->హద్దు_present)
        ram_mib = def->linux->resources->జ్ఞాపకం->హద్దు / (1024 * 1024);
      లేకపోతే
        ram_mib = LIBKRUN_DEFAULT_RAM_MIB;
    }

  krun_సమితి_vm_config = dlsym (handle, "krun_set_vm_config");

  ఒకవేళ (krun_సమితి_vm_config == ఖాళీ)
    తిరిగి crun_make_error (err, 0, "could not find symbol in the krun library");

  ret = krun_సమితి_vm_config (ctx_id, cpus, ram_mib);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, -ret, "could not set krun vm configuration");

  nested_virt = libkrun_parse_resource_configuration (kconf->config_వృక్షం, container, "krun.nested_virt", "nested_virt", false);
  ఒకవేళ (nested_virt > 0)
    {
      int32_t (*krun_check_nested_virt) (శూన్య);
      int32_t (*krun_సమితి_nested_virt) (uint32_t ctx_id, bool enabled);

      krun_check_nested_virt = dlsym (handle, "krun_check_nested_virt");
      ఒకవేళ (krun_check_nested_virt != ఖాళీ && krun_check_nested_virt () != 1)
        libcrun_warning ("nested virtualization requested but may not be supported on this host");

      krun_సమితి_nested_virt = dlsym (handle, "krun_set_nested_virt");
      ఒకవేళ (krun_సమితి_nested_virt == ఖాళీ)
        తిరిగి crun_make_error (err, 0, "could not find symbol `krun_set_nested_virt` in the krun library");

      ret = krun_సమితి_nested_virt (ctx_id, true);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, -ret, "could not enable nested virtualization");
    }

  ఒకవేళ (kconf->tap_name != ఖాళీ)
    {
      krun_కూడు_net_tap = dlsym (handle, "krun_add_net_tap");
      ఒకవేళ (krun_కూడు_net_tap == ఖాళీ)
        తిరిగి crun_make_error (err, 0, "could not find symbol `krun_add_net_tap` in the krun library");

      uint8_t mac[6];
      libkrun_make_tap_mac (container->context->id, mac);
      ret = krun_కూడు_net_tap (ctx_id, kconf->tap_name, &mac[0], COMPAT_NET_FEATURES, 0);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, -ret, "could not add krun TAP interface `%s`", kconf->tap_name);
    }
  లేకపోతే ఒకవేళ (kconf->use_passt)
    {
      krun_కూడు_net_unixstream = dlsym (handle, "krun_add_net_unixstream");
      ఒకవేళ (krun_కూడు_net_unixstream == ఖాళీ)
        తిరిగి crun_make_error (err, 0, "could not find symbol `krun_add_net_unixstream` in the krun library");

      uint8_t mac[] = { 0x5a, 0x94, 0xef, 0xe4, 0x0c, 0xee };
      ret = krun_కూడు_net_unixstream (ctx_id, ఖాళీ, kconf->passt_fds[FD_PAIR_మాతృ], &mac[0], COMPAT_NET_FEATURES, NET_FLAG_DHCP_CLIENT);
      ఒకవేళ (UNLIKELY (ret == -EINVAL))
        ret = krun_కూడు_net_unixstream (ctx_id, ఖాళీ, kconf->passt_fds[FD_PAIR_మాతృ], &mac[0], COMPAT_NET_FEATURES, 0);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, -ret, "could not set krun net configuration");
    }

  ఒకవేళ (kconf->gpu_flags > 0)
    {
      krun_సమితి_gpu_options = dlsym (kconf->handle, "krun_set_gpu_options");
      ఒకవేళ (krun_సమితి_gpu_options == ఖాళీ)
        తిరిగి crun_make_error (err, 0, "gpu requested but the version of libkrun in this system does not support it");

      ret = krun_సమితి_gpu_options (kconf->ctx_id, kconf->gpu_flags);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, -ret, "gpu requested but could not configure virtio gpu device");

      ఒకవేళ (kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER)
        {
          krun_సమితి_gpu_render_server_fd = dlsym (kconf->handle, "krun_set_gpu_render_server_fd");
          ఒకవేళ (krun_సమితి_gpu_render_server_fd == ఖాళీ)
            తిరిగి crun_make_error (err, 0, "gpu with render server requested but the version of libkrun in this system does not support it");

          ret = krun_సమితి_gpu_render_server_fd (kconf->ctx_id, kconf->gpu_render_server_fd);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, -ret, "gpu with render server requested but could not configure its file descriptor");
        }
    }

  ఒకవేళ (kconf->config_వృక్షం != ఖాళీ)
    {
      /* Try to configure an external kernel. If the configuration file doesn't
       * specify a kernel, libkrun automatically fall back to using libkrunfw,
       * if the library is present and was loaded while creating the context.
       */
      పూర్ణాంకం custom_kernel = libkrun_parse_resource_configuration (kconf->config_వృక్షం, container, "krun.custom_kernel", "custom_kernel", false);
      ఒకవేళ (custom_kernel > 0)
        {
          ret = libkrun_configure_kernel (ctx_id, handle, kconf->config_వృక్షం, err);
          ఒకవేళ (UNLIKELY (ret))
            తిరిగి ret;
        }
    }

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
libkrun_configure_flavor (శూన్య *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  పూర్ణాంకం ret, sev_indicated = 0, awsnitro_indicated = 0;
  నిర్మాణం krun_config *kconf = (నిర్మాణం krun_config *) cookie;
  స్థిరరాశి అక్షరం *flavor = ఖాళీ;
  శూన్య *close_handles[2];

  close_handles[0] = ఖాళీ;
  close_handles[1] = ఖాళీ;

  // Check if the user provided the krun variant through OCI annotations.
  flavor = కనుగొను_annotation (container, "krun.variant");
  ఒకవేళ (flavor != ఖాళీ)
    {
      sev_indicated |= strcmp (flavor, KRUN_FLAVOR_SEV) == 0;
      awsnitro_indicated |= strcmp (flavor, KRUN_FLAVOR_AWS_NITRO) == 0;
    }

  ఒకవేళ (sev_indicated)
    {
      ఒకవేళ (kconf->handle_sev == ఖాళీ)
        error (EXIT_FAILURE, 0, "the container requires libkrun-sev but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_awsnitro;

      kconf->handle = kconf->handle_sev;
      kconf->ctx_id = kconf->ctx_id_sev;
      kconf->sev = true;
    }
  లేకపోతే ఒకవేళ (awsnitro_indicated)
    {
      ఒకవేళ (kconf->handle_awsnitro == ఖాళీ)
        error (EXIT_FAILURE, 0, "the container requires libkrun-awsnitro but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_sev;

      kconf->handle = kconf->handle_awsnitro;
      kconf->ctx_id = kconf->ctx_id_awsnitro;
      kconf->awsnitro = true;
    }
  లేకపోతే
    {
      ఒకవేళ (kconf->handle == ఖాళీ)
        error (EXIT_FAILURE, 0, "the container requires libkrun but it's not available");

      close_handles[0] = kconf->handle_sev;
      close_handles[1] = kconf->handle_awsnitro;
    }

  // We no longer need the other two libkrun handles.
  కొరకు (పూర్ణాంకం i = 0; i < 2; i++)
    {
      ఒకవేళ (close_handles[i] == ఖాళీ)
        కొనసాగు;

      ret = dlclose (close_handles[i]);
      ఒకవేళ (UNLIKELY (ret != 0))
        తిరిగి crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
libkrun_exec (శూన్య *cookie, libcrun_container_t *container, స్థిరరాశి అక్షరం *pathname, అక్షరం *స్థిరరాశి argv[])
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_సమితి_log_level) (uint32_t level);
  పూర్ణాంకం (*krun_ప్రారంభం_enter) (uint32_t ctx_id);
  int32_t (*krun_కూడు_virtiofs2) (uint32_t ctx_id, స్థిరరాశి అక్షరం *c_ట్యాగ్, స్థిరరాశి అక్షరం *c_మార్గం, uint64_t shm_కొలత);
  int32_t (*krun_సమితి_మూలం_disk) (uint32_t ctx_id, స్థిరరాశి అక్షరం *disk_మార్గం);
  int32_t (*krun_సమితి_tee_config_file) (uint32_t ctx_id, స్థిరరాశి అక్షరం *file_మార్గం);
  int32_t (*krun_సమితి_console_output) (uint32_t ctx_id, స్థిరరాశి అక్షరం *c_filepath);
  int32_t (*krun_సమితి_exec) (uint32_t ctx_id, స్థిరరాశి అక్షరం *exec_మార్గం,
                            స్థిరరాశి అక్షరం *స్థిరరాశి argv[], స్థిరరాశి అక్షరం *స్థిరరాశి envp[]);
  నిర్మాణం krun_config *kconf = (నిర్మాణం krun_config *) cookie;
  శూన్య *handle;
  int32_t ctx_id, ret;
  libcrun_error_t err;

  ret = libkrun_configure_flavor (cookie, container, &err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      పూర్ణాంకం errcode = crun_error_get_errno (&err);
      crun_error_release (&err);
      error (EXIT_FAILURE, errcode, "unable to configure libkrun flavor");
    }

  // /dev/kvm is required for all non AWS nitro workloads.
  ఒకవేళ (! kconf->awsnitro && ! kconf->has_kvm)
    error (EXIT_FAILURE, 0, "`/dev/kvm` unavailable");

  handle = kconf->handle;
  ctx_id = kconf->ctx_id;

  krun_సమితి_log_level = dlsym (handle, "krun_set_log_level");
  krun_ప్రారంభం_enter = dlsym (handle, "krun_start_enter");
  ఒకవేళ (krun_సమితి_log_level == ఖాళీ || krun_ప్రారంభం_enter == ఖాళీ)
    error (EXIT_FAILURE, 0, "could not find symbol in the krun library");

  /* Set log level according to crun's verbosity. */
  ఎంపిక (libcrun_get_verbosity ())
    {
    స్థితి LIBCRUN_VERBOSITY_DEBUG:
      krun_సమితి_log_level (KRUN_LOG_LEVEL_DEBUG);
      ఆపు;
    స్థితి LIBCRUN_VERBOSITY_WARNING:
      krun_సమితి_log_level (KRUN_LOG_LEVEL_WARN);
      ఆపు;
    అప్రమేయ:
      krun_సమితి_log_level (KRUN_LOG_LEVEL_ERROR);
      ఆపు;
    }

  ఒకవేళ (kconf->sev)
    {
      krun_సమితి_మూలం_disk = dlsym (handle, "krun_set_root_disk");
      krun_సమితి_tee_config_file = dlsym (handle, "krun_set_tee_config_file");
      ఒకవేళ (krun_సమితి_మూలం_disk == ఖాళీ || krun_సమితి_tee_config_file == ఖాళీ)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-sev.so`");

      ret = krun_సమితి_మూలం_disk (ctx_id, "/disk.img");
      ఒకవేళ (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set root disk");

      ret = krun_సమితి_tee_config_file (ctx_id, KRUN_SEV_FILE);
      ఒకవేళ (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set krun tee config file");
    }
  లేకపోతే
    {
      json_object *val_virtiofs_ట్యాగ్ = ఖాళీ;
      json_object *val_virtiofs_shm_కొలత = ఖాళీ;
      స్థిరరాశి అక్షరం *virtiofs_ట్యాగ్ = ఖాళీ;
      // Default to a conservative DAX size of 512MB, just like krun_set_root() does.
      uint64_t virtiofs_shm_కొలత = 512 * 1024 * 1024ULL;

      ఒకవేళ (kconf->config_వృక్షం != ఖాళీ)
        {
          val_virtiofs_ట్యాగ్ = json_object_object_get (kconf->config_వృక్షం, "virtiofs_tag");
          ఒకవేళ (val_virtiofs_ట్యాగ్ != ఖాళీ && json_object_is_type (val_virtiofs_ట్యాగ్, json_type_string))
            virtiofs_ట్యాగ్ = json_object_get_string (val_virtiofs_ట్యాగ్);

          val_virtiofs_shm_కొలత = json_object_object_get (kconf->config_వృక్షం, "virtiofs_shm_size");
          ఒకవేళ (val_virtiofs_shm_కొలత != ఖాళీ && json_object_is_type (val_virtiofs_shm_కొలత, json_type_int))
            virtiofs_shm_కొలత = json_object_get_uint64 (val_virtiofs_shm_కొలత);
        }

      ఒకవేళ (virtiofs_ట్యాగ్ == ఖాళీ)
        virtiofs_ట్యాగ్ = "/dev/root";

      krun_కూడు_virtiofs2 = dlsym (handle, "krun_add_virtiofs2");

      ఒకవేళ (krun_కూడు_virtiofs2 == ఖాళీ)
        error (EXIT_FAILURE, 0, "could not find symbol `krun_add_virtiofs2` in `libkrun.so`");

      ret = krun_కూడు_virtiofs2 (ctx_id, virtiofs_ట్యాగ్, "/", virtiofs_shm_కొలత);
      ఒకవేళ (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not add virtiofs root with tag `%s`", virtiofs_ట్యాగ్);
    }

  ఒకవేళ (kconf->awsnitro)
    {
      krun_సమితి_console_output = dlsym (handle, "krun_set_console_output");
      krun_సమితి_exec = dlsym (handle, "krun_set_exec");
      ఒకవేళ (krun_సమితి_console_output == ఖాళీ || krun_సమితి_exec == ఖాళీ)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-awsnitro.so`");

      // Redirect all enclave output (read from vsock) to stdout.
      ret = krun_సమితి_console_output (ctx_id, "/dev/stdout");
      ఒకవేళ (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not redirect enclave output to stdout");

      ret = krun_సమితి_exec (ctx_id, pathname, (స్థిరరాశి అక్షరం *స్థిరరాశి *) argv,
                           (స్థిరరాశి అక్షరం *స్థిరరాశి *) def->process->env);
      ఒకవేళ (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set enclave execution arguments");
    }

  ret = libkrun_configure_vm (ctx_id, handle, kconf, container, &err);
  ఒకవేళ (UNLIKELY (ret))
    {
      పూర్ణాంకం errcode = crun_error_get_errno (&err);
      libcrun_error_t *tmp_err = &err;
      libcrun_error_report_and_release (tmp_err);
      error (EXIT_FAILURE, errcode, "could not configure krun vm");
    }

  json_object_put (kconf->config_doc);

  ret = krun_ప్రారంభం_enter (ctx_id);
  ఒకవేళ (UNLIKELY (ret < 0))
    error (EXIT_FAILURE, -ret, "could not start krun");

  తిరిగి ret;
}

స్థిర పూర్ణాంకం
libkrun_ప్రారంభం_render_server (నిర్మాణం krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  kconf->gpu_flags = libkrun_parse_resource_configuration (kconf->config_వృక్షం, container, "krun.gpu_flags", "gpu_flags", false);
  ఒకవేళ (kconf->gpu_flags > 0)
    {
      ఒకవేళ (access ("/dev/dri", F_OK) != 0)
        తిరిగి crun_make_error (err, errno, "gpu requested but /dev/dri is not available");

      ఒకవేళ ((kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER) != 0)
        {
          అక్షరం *rs_argv[40];
          పూర్ణాంకం rs_argc = 0;
          అక్షరం fd_as_str[16];
          పూర్ణాంకం gpu_fds[2];
          pid_t pid;
          పూర్ణాంకం null;

          ఒకవేళ (access ("/usr/libexec/virgl_render_server", X_OK) != 0)
            తిరిగి crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/libexec/virgl_render_server binary");

          ఒకవేళ (access ("/usr/bin/bwrap", X_OK) != 0)
            తిరిగి crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/bin/bwrap binary");

          ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, gpu_fds);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "could not create socketpair for virgl_render_server");

          snprintf (fd_as_str, పరిమాణం (fd_as_str), "%d", gpu_fds[FD_PAIR_బిడ్డ]);

          rs_argv[rs_argc++] = (అక్షరం *) "bwrap";
          rs_argv[rs_argc++] = (అక్షరం *) "--ro-bind";
          rs_argv[rs_argc++] = (అక్షరం *) "/usr";
          rs_argv[rs_argc++] = (అక్షరం *) "/usr";
          ఒకవేళ (access ("/lib", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (అక్షరం *) "--ro-bind";
              rs_argv[rs_argc++] = (అక్షరం *) "/lib";
              rs_argv[rs_argc++] = (అక్షరం *) "/lib";
            }
          ఒకవేళ (access ("/lib64", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (అక్షరం *) "--ro-bind";
              rs_argv[rs_argc++] = (అక్షరం *) "/lib64";
              rs_argv[rs_argc++] = (అక్షరం *) "/lib64";
            }
          rs_argv[rs_argc++] = (అక్షరం *) "--ro-bind";
          rs_argv[rs_argc++] = (అక్షరం *) "/etc";
          rs_argv[rs_argc++] = (అక్షరం *) "/etc";
          rs_argv[rs_argc++] = (అక్షరం *) "--ro-bind";
          rs_argv[rs_argc++] = (అక్షరం *) "/sys";
          rs_argv[rs_argc++] = (అక్షరం *) "/sys";
          rs_argv[rs_argc++] = (అక్షరం *) "--dev";
          rs_argv[rs_argc++] = (అక్షరం *) "/dev";
          rs_argv[rs_argc++] = (అక్షరం *) "--dev-bind";
          rs_argv[rs_argc++] = (అక్షరం *) "/dev/dri";
          rs_argv[rs_argc++] = (అక్షరం *) "/dev/dri";
          rs_argv[rs_argc++] = (అక్షరం *) "--proc";
          rs_argv[rs_argc++] = (అక్షరం *) "/proc";
          rs_argv[rs_argc++] = (అక్షరం *) "--tmpfs";
          rs_argv[rs_argc++] = (అక్షరం *) "/tmp";
          rs_argv[rs_argc++] = (అక్షరం *) "--unshare-pid";
          rs_argv[rs_argc++] = (అక్షరం *) "--unshare-ipc";
          rs_argv[rs_argc++] = (అక్షరం *) "--unshare-net";
          rs_argv[rs_argc++] = (అక్షరం *) "--";
          rs_argv[rs_argc++] = (అక్షరం *) "/usr/libexec/virgl_render_server";
          rs_argv[rs_argc++] = (అక్షరం *) "--socket-fd";
          rs_argv[rs_argc++] = fd_as_str;
          rs_argv[rs_argc] = ఖాళీ;

          pid = fork ();
          ఒకవేళ (pid < 0)
            {
              close (gpu_fds[FD_PAIR_మాతృ]);
              close (gpu_fds[FD_PAIR_బిడ్డ]);
              తిరిగి crun_make_error (err, errno, "could not fork for virgl_render_server");
            }
          లేకపోతే ఒకవేళ (pid == 0)
            {
              close (gpu_fds[FD_PAIR_మాతృ]);

              null = open ("/dev/null", O_WRONLY);
              ఒకవేళ (null == -1)
                _exit (EXIT_FAILURE);

              dup2 (null, STDOUT_FILENO);
              dup2 (null, STDERR_FILENO);
              close (null);

              execvp ("/usr/bin/bwrap", rs_argv);
              _exit (EXIT_FAILURE);
            }

          close (gpu_fds[FD_PAIR_బిడ్డ]);
          kconf->gpu_render_server_fd = gpu_fds[FD_PAIR_మాతృ];
        }
    }

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
libkrun_configure_జాలం (శూన్య *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  నిర్మాణం krun_config *kconf = (నిర్మాణం krun_config *) cookie;
  pid_t pid;
  అక్షరం *passt_argv[10];
  అక్షరం fd_as_str[16];
  పూర్ణాంకం use_passt;
  పూర్ణాంకం argv_idx;
  పూర్ణాంకం స్థితి_2;
  పూర్ణాంకం null;
  పూర్ణాంకం ret;

  ret = libkrun_parse_string_configuration (kconf->config_వృక్షం, container,
                                            "krun.tap_name", "tap_name",
                                            &kconf->tap_name, err, false);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (kconf->tap_name != ఖాళీ && is_ఖాళీలు_string (kconf->tap_name))
    తిరిగి crun_make_error (err, 0, "krun.tap_name cannot be empty");

  use_passt = libkrun_parse_resource_configuration (kconf->config_వృక్షం, container, "krun.use_passt", "use_passt", false);

  ఒకవేళ (use_passt > 0)
    {
      ఒకవేళ (kconf->tap_name != ఖాళీ)
        తిరిగి crun_make_error (err, 0, "krun.tap_name and krun.use_passt are mutually exclusive");

      kconf->use_passt = 1;
    }
  లేకపోతే
    తిరిగి 0;

  ret = socketpair (AF_UNIX, SOCK_ప్రవాహం, 0, kconf->passt_fds);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "create passt socketpair");

  snprintf (fd_as_str, పరిమాణం (fd_as_str), "%d", kconf->passt_fds[FD_PAIR_బిడ్డ]);

  argv_idx = 0;
  passt_argv[argv_idx++] = (అక్షరం *) "passt";
  passt_argv[argv_idx++] = (అక్షరం *) "-t";
  passt_argv[argv_idx++] = (అక్షరం *) "all";

  ఒకవేళ (! kconf->has_awsnitro)
    {
      passt_argv[argv_idx++] = (అక్షరం *) "-u";
      passt_argv[argv_idx++] = (అక్షరం *) "all";
      passt_argv[argv_idx++] = (అక్షరం *) "--no-dhcp-dns";
    }

  /* Set --no-map-gw. If the gateway for the network is also the DNS server
   * we won't be able to query the DNS server without --no-map-gw.
   * See discussion in https://github.com/containers/crun/pull/2099
   */
  passt_argv[argv_idx++] = (అక్షరం *) "--no-map-gw";

  passt_argv[argv_idx++] = (అక్షరం *) "--fd";
  passt_argv[argv_idx++] = fd_as_str;
  passt_argv[argv_idx] = ఖాళీ;

  pid = fork ();
  ఒకవేళ (pid < 0)
    తిరిగి crun_make_error (err, errno, "fork passt");
  లేకపోతే ఒకవేళ (pid == 0)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t i;

      close (kconf->passt_fds[FD_PAIR_మాతృ]);

      null = open ("/dev/null", O_WRONLY);
      ఒకవేళ (null == -1)
        _exit (EXIT_FAILURE);

      /* Remount /proc if the parent detached its PID namespace, as passt(1)
       * needs a consistent /proc view to operate properly.
       */
      కొరకు (i = 0; i < def->linux->namespaces_len; i++)
        {
          పూర్ణాంకం విలువ = libcrun_కనుగొను_namespace (def->linux->namespaces[i]->type);

          ఒకవేళ (విలువ == CLONE_NEWPID)
            {
              ఒకవేళ (mount ("", "/proc", "proc", MS_NOSUID | MS_NODEV | MS_NOEXEC, ఖాళీ))
                _exit (EXIT_FAILURE);

              ఆపు;
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

  close (kconf->passt_fds[FD_PAIR_బిడ్డ]);

  // Wait for passt to daemonize itself.
  waitpid (pid, &స్థితి_2, 0);
  ఒకవేళ (! (WIFEXITED (స్థితి_2)) || WEXITSTATUS (స్థితి_2) != 0)
    తిరిగి crun_make_error (err, 0, "start passt");

  తిరిగి 0;
}

/* Return true if the spec already declares a device with the given PATH.  */
స్థిర bool
spec_has_device (runtime_spec_schema_config_schema *def, స్థిరరాశి అక్షరం *మార్గం)
{
  size_t i;

  కొరకు (i = 0; i < def->linux->devices_len; i++)
    ఒకవేళ (strcmp (def->linux->devices[i]->మార్గం, మార్గం) == 0)
      తిరిగి true;
  తిరిగి false;
}

/* libkrun_create_kvm_device: explicitly adds kvm device.  */
స్థిర పూర్ణాంకం
libkrun_configure_container (శూన్య *cookie, గణన handler_configure_phase phase,
                             libcrun_context_t *context, libcrun_container_t *container,
                             స్థిరరాశి అక్షరం *rootfs, libcrun_error_t *err)
{
  పూర్ణాంకం ret, rootfsfd;
  నిర్మాణం krun_config *kconf = (నిర్మాణం krun_config *) cookie;
  నిర్మాణం device_s kvm_device = { "/dev/kvm", "c", 10, 232, 0666, 0, 0 };
  నిర్మాణం device_s sev_device = { "/dev/sev", "c", 10, 124, 0666, 0, 0 };
  నిర్మాణం device_s nitro_device = { "/dev/nitro_enclaves", "c", 10, 122, 0666, 0, 0 };
  cleanup_close పూర్ణాంకం devfd = -1;
  cleanup_close పూర్ణాంకం rootfsfd_cleanup = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  bool create_sev = false, create_awsnitro = false;
  bool is_user_ns;

  ఒకవేళ (rootfs == ఖాళీ)
    rootfsfd = AT_FDCWD;
  లేకపోతే
    {
      rootfsfd = rootfsfd_cleanup = open (rootfs, O_మార్గం | O_CLOEXEC);
      ఒకవేళ (UNLIKELY (rootfsfd < 0))
        తిరిగి crun_make_error (err, errno, "open `%s`", rootfs);
    }

  ఒకవేళ (phase == HANDLER_CONFIGURE_BEFORE_USERNS)
    {
      cleanup_free అక్షరం *origin_config_మార్గం = ఖాళీ;
      cleanup_free అక్షరం *state_dir = ఖాళీ;
      cleanup_free అక్షరం *config = ఖాళీ;
      cleanup_close పూర్ణాంకం fd = -1;
      size_t config_కొలత;

      ret = libcrun_get_state_directory (&state_dir, context->state_మూలం, context->id, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = append_paths (&origin_config_మార్గం, err, state_dir, "config.json", ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = read_all_file (origin_config_మార్గం, &config, &config_కొలత, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      /* CVE-2025-24965: the content below rootfs cannot be trusted because it is controlled by the user.  We
         must ensure the file is opened below the rootfs directory.  Whatever the image ships at that name
         could also be a device node or a FIFO, so never open it: unlink it and create a new regular file.  */
      ret = unlinkat (rootfsfd, KRUN_CONFIG_FILE, 0);
      ఒకవేళ (UNLIKELY (ret < 0 && errno != ENOENT))
        తిరిగి crun_make_error (err, errno, "unlink `%s`", KRUN_CONFIG_FILE);

      fd = సురక్షిత_openat (rootfsfd, rootfs, KRUN_CONFIG_FILE, WRITE_FILE_DEFAULT_FLAGS | O_EXCL | O_NOFOLLOW, S_IRUSR | S_IRGRP | S_IROTH, err);
      ఒకవేళ (UNLIKELY (fd < 0))
        తిరిగి fd;

      ret = సురక్షిత_write (fd, KRUN_CONFIG_FILE, config, config_కొలత, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = libkrun_read_vm_config (kconf, rootfsfd, rootfs, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = libkrun_ప్రారంభం_render_server (kconf, container, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    తిరిగి 0;

  ret = libkrun_configure_జాలం (cookie, container, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  /* Do nothing if /dev/kvm is already present in spec */
  ఒకవేళ (spec_has_device (def, "/dev/kvm"))
    తిరిగి 0;

  ఒకవేళ (kconf->handle_sev != ఖాళీ)
    create_sev = ! spec_has_device (def, "/dev/sev");

  ఒకవేళ (kconf->handle_awsnitro != ఖాళీ)
    create_awsnitro = ! spec_has_device (def, "/dev/nitro_enclaves");

  devfd = సురక్షిత_openat (rootfsfd, rootfs, "dev", O_మార్గం | O_DIRECTORY | O_CLOEXEC, 0, err);
  ఒకవేళ (UNLIKELY (devfd < 0))
    తిరిగి devfd;

  ret = check_running_లోపల_user_namespace (err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;
  is_user_ns = ret;

  ఒకవేళ (kconf->has_kvm)
    {
      ret = libcrun_create_dev (container, devfd, -1, &kvm_device, is_user_ns, true, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (create_sev)
    {
      ret = libcrun_create_dev (container, devfd, -1, &sev_device, is_user_ns, true, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_sev);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
          kconf->handle_sev = ఖాళీ;
        }
    }

  ఒకవేళ (create_awsnitro)
    {
      ret = libcrun_create_dev (container, devfd, -1, &nitro_device, is_user_ns, true, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_awsnitro);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
          kconf->handle_awsnitro = ఖాళీ;
        }
    }

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
libkrun_ఎక్కించు (శూన్య **cookie, libcrun_error_t *err)
{
  int32_t ret;
  నిర్మాణం krun_config *kconf;
  స్థిరరాశి అక్షరం *libkrun_so = "libkrun.so.1";
  స్థిరరాశి అక్షరం *libkrun_sev_so = "libkrun-sev.so.1";
  స్థిరరాశి అక్షరం *libkrun_awsnitro_so = "libkrun-awsnitro.so.1";

  kconf = జ్ఞాపకం_ఇవ్వు (పరిమాణం (నిర్మాణం krun_config));
  ఒకవేళ (kconf == ఖాళీ)
    తిరిగి crun_make_error (err, 0, "could not allocate memory for krun_config");
  memset (kconf, 0, పరిమాణం (నిర్మాణం krun_config));

  kconf->gpu_render_server_fd = -1;

  kconf->handle = dlopen (libkrun_so, RTLD_NOW);
  kconf->handle_sev = dlopen (libkrun_sev_so, RTLD_NOW);
  kconf->handle_awsnitro = dlopen (libkrun_awsnitro_so, RTLD_NOW);

  ఒకవేళ (kconf->handle == ఖాళీ && kconf->handle_sev == ఖాళీ && kconf->handle_awsnitro == ఖాళీ)
    {
      విడుదల (kconf);
      తిరిగి crun_make_error (err, 0, "failed to open `%s`, `%s`, and `%s` for krun_config: %s", libkrun_so, libkrun_sev_so, libkrun_awsnitro_so, dlerror ());
    }

  kconf->sev = false;
  kconf->awsnitro = false;

  /* Newer versions of libkrun no longer link against libkrunfw and
     instead they open it when creating the context. This implies
     we need to call "krun_create_ctx" before switching namespaces
     or it won't be able to find the library bundling the kernel. */
  ఒకవేళ (kconf->handle)
    {
      ret = libkrun_create_context (kconf->handle, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        వెళ్ళు error;
      kconf->ctx_id = ret;
    }

  ఒకవేళ (kconf->handle_sev)
    {
      ret = libkrun_create_context (kconf->handle_sev, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        వెళ్ళు error;
      kconf->ctx_id_sev = ret;
    }
  ఒకవేళ (kconf->handle_awsnitro)
    {
      ret = libkrun_create_context (kconf->handle_awsnitro, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        వెళ్ళు error;
      kconf->ctx_id_awsnitro = ret;
    }

  *cookie = kconf;

  తిరిగి 0;

error:
  ఒకవేళ (kconf->handle)
    dlclose (kconf->handle);
  ఒకవేళ (kconf->handle_sev)
    dlclose (kconf->handle_sev);
  ఒకవేళ (kconf->handle_awsnitro)
    dlclose (kconf->handle_awsnitro);
  విడుదల (kconf);
  తిరిగి ret;
}

స్థిర పూర్ణాంకం
libkrun_unload (శూన్య *cookie, libcrun_error_t *err)
{
  పూర్ణాంకం r;

  నిర్మాణం krun_config *kconf = (నిర్మాణం krun_config *) cookie;
  ఒకవేళ (kconf != ఖాళీ)
    {
      ఒకవేళ (kconf->handle != ఖాళీ)
        {
          r = dlclose (kconf->handle);
          ఒకవేళ (UNLIKELY (r != 0))
            తిరిగి crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
        }
      ఒకవేళ (kconf->handle_sev != ఖాళీ)
        {
          r = dlclose (kconf->handle_sev);
          ఒకవేళ (UNLIKELY (r != 0))
            తిరిగి crun_make_error (err, 0, "could not unload handle_sev: `%s`", dlerror ());
        }
      ఒకవేళ (kconf->handle_awsnitro != ఖాళీ)
        {
          r = dlclose (kconf->handle_awsnitro);
          ఒకవేళ (UNLIKELY (r != 0))
            తిరిగి crun_make_error (err, 0, "could not unload handle_awsnitro: `%s`", dlerror ());
        }
      విడుదల (kconf);
    }
  తిరిగి 0;
}

స్థిర runtime_spec_schema_defs_linux_device_cgroup *
make_oci_spec_dev (స్థిరరాశి అక్షరం *type, dev_t device, bool allow, స్థిరరాశి అక్షరం *access)
{
  runtime_spec_schema_defs_linux_device_cgroup *dev = xmalloc0 (పరిమాణం (*dev));

  dev->allow = allow;
  dev->allow_present = 1;

  dev->type = xstrdup (type);

  dev->major = major (device);
  dev->major_present = 1;

  dev->minor = minor (device);
  dev->minor_present = 1;

  dev->access = xstrdup (access);

  తిరిగి dev;
}

/* stat an optional device: on ENOENT clear *PRESENT, any other error fails.  */
స్థిర పూర్ణాంకం
stat_optional_device (స్థిరరాశి అక్షరం *మార్గం, నిర్మాణం stat *st, bool *present, libcrun_error_t *err)
{
  పూర్ణాంకం ret = stat (మార్గం, st);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ఒకవేళ (errno != ENOENT)
        తిరిగి crun_make_error (err, errno, "stat `%s`", మార్గం);
      *present = false;
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
libkrun_modify_oci_configuration (శూన్య *cookie arg_వాడని, libcrun_context_t *context arg_వాడని,
                                  runtime_spec_schema_config_schema *def,
                                  libcrun_error_t *err)
{
  స్థిరరాశి size_t device_కొలత = పరిమాణం (runtime_spec_schema_defs_linux_device_cgroup);
  నిర్మాణం krun_config *kconf = (నిర్మాణం krun_config *) cookie;
  నిర్మాణం stat st_kvm, st_sev, st_awsnitro;
  bool has_kvm = true, has_sev = true, has_awsnitro = true;
  size_t old_len, new_len;
  పూర్ణాంకం ret;

  /* Always allow the /dev/kvm device.  */

  ret = stat_optional_device ("/dev/kvm", &st_kvm, &has_kvm, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = stat_optional_device ("/dev/sev", &st_sev, &has_sev, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = stat_optional_device ("/dev/nitro_enclaves", &st_awsnitro, &has_awsnitro, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  kconf->has_kvm = has_kvm;
  kconf->has_awsnitro = has_awsnitro;

  ఒకవేళ (! has_kvm && ! has_awsnitro)
    తిరిగి 0;

  /* spec says these are optional, ensure they exist so we can add our devices */
  ఒకవేళ (def->linux == ఖాళీ)
    def->linux = xmalloc0 (పరిమాణం (runtime_spec_schema_config_linux));

  ఒకవేళ (def->linux->resources == ఖాళీ)
    def->linux->resources = xmalloc0 (పరిమాణం (runtime_spec_schema_config_linux_resources));

  old_len = def->linux->resources->devices_len;
  new_len = old_len;
  ఒకవేళ (has_kvm)
    new_len += has_sev ? 2 : 1;
  ఒకవేళ (has_awsnitro)
    new_len += 1;

  def->linux->resources->devices = xrealloc (def->linux->resources->devices, device_కొలత * (new_len + 1));
  def->linux->resources->devices_len = new_len;

  ఒకవేళ (has_kvm)
    {
      def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_kvm.st_rdev, true, "rwm");
      ఒకవేళ (has_sev)
        def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_sev.st_rdev, true, "rwm");
    }

  ఒకవేళ (has_awsnitro)
    def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_awsnitro.st_rdev, true, "rwm");

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
libkrun_close_fds (శూన్య *cookie, libcrun_container_t *container, పూర్ణాంకం preserve_fds, libcrun_error_t *err)
{
  నిర్మాణం krun_config *kconf = (నిర్మాణం krun_config *) cookie;
  పూర్ణాంకం మొదటి_fd_గమ్యం_close = preserve_fds + 3;
  పూర్ణాంకం దిగువ_fd, ఎగువ_fd;
  పూర్ణాంకం i;

  దిగువ_fd = ఎగువ_fd = -1;

  ఒకవేళ (kconf->use_passt)
    ఎగువ_fd = kconf->passt_fds[FD_PAIR_మాతృ];

  ఒకవేళ (kconf->gpu_render_server_fd != -1)
    {
      ఒకవేళ (kconf->gpu_render_server_fd > ఎగువ_fd)
        {
          దిగువ_fd = ఎగువ_fd;
          ఎగువ_fd = kconf->gpu_render_server_fd;
        }
      లేకపోతే
        దిగువ_fd = kconf->gpu_render_server_fd;
    }

  ఒకవేళ (మొదటి_fd_గమ్యం_close <= ఎగువ_fd)
    {
      కొరకు (i = మొదటి_fd_గమ్యం_close; i < ఎగువ_fd; i++)
        {
          // If we're closing proc_fd, make sure to invalidate it.
          ఒకవేళ (i == container->proc_fd)
            container->proc_fd = -1;
          లేకపోతే ఒకవేళ (i == దిగువ_fd)
            కొనసాగు;
          close (i);
        }

      మొదటి_fd_గమ్యం_close = ఎగువ_fd + 1;
    }

  తిరిగి mark_or_close_fds_ge_than (container, మొదటి_fd_గమ్యం_close, true, err);
}

నిర్మాణం custom_handler_s handler_libkrun = {
  .name = "krun",
  .alias = ఖాళీ,
  .feature_string = "LIBKRUN",
  .supports_open_వృక్షం_namespace = false,
  .ఎక్కించు = libkrun_ఎక్కించు,
  .unload = libkrun_unload,
  .నిరంతరం_func = libkrun_exec,
  .configure_container = libkrun_configure_container,
  .modify_oci_configuration = libkrun_modify_oci_configuration,
  .close_fds = libkrun_close_fds,
};

#అయితే_ముగింపు
