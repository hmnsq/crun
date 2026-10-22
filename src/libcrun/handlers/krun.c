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
#تعریف_کرو GNU_ماخذ

#شامل <config.h>
#شامل "../custom-handler.h"
#شامل "../container.h"
#شامل "../utils.h"
#شامل "../linux.h"
#شامل <unistd.h>
#شامل <sys/stat.h>
#شامل <sys/socket.h>
#شامل <errno.h>
#شامل <sys/mount.h>
#شامل <sys/param.h>
#شامل <sys/types.h>
#شامل <sys/sysmacros.h>
#شامل <fcntl.h>
#شامل <sched.h>
#شامل <ocispec/runtime_spec_schema_config_schema.h>

#اگر_تعریف_شدہ HAVE_DLOPEN
#  include <dlfcn.h>
#اگر_ختم

#اگر_تعریف_شدہ HAVE_LIBKRUN
#  include <libkrun.h>
#اگر_ختم

/* This allows us to build even with headers from older libkrun versions.
 * If the installed version of libkrun doesn't support this feature,
 * krun_add_net_unixstream() will return EINVAL.
 */
#اگر_غیر_تعریف_شدہ NET_FLAG_DHCP_CLIENT
#  define NET_FLAG_DHCP_CLIENT (1 << 1)
#اگر_ختم
/* libkrun has a hard-limit of 16 vCPUs per microVM. */
#تعریف_کرو LIBKRUN_MAX_VCPUS 16

/* If the user doesn't configure the RAM amount, fallback to this value. */
#تعریف_کرو LIBKRUN_DEFAULT_RAM_MIB 1024

/* The minimum amount of RAM for a viable microVM is 128 MB. */
#تعریف_کرو LIBKRUN_MINIMUM_RAM_MIB 128

/* crun dumps the container configuration into this file, which will be read by
 * libkrun to set up the environment for the workload inside the microVM.
 */
#تعریف_کرو KRUN_CONFIG_FILE ".krun_config.json"

/* The presence of this file indicates this is a container intended to be run
 * as a confidential workload inside a SEV-powered TEE.
 */
#تعریف_کرو KRUN_SEV_FILE "/krun-sev.json"

/* This file contains configuration parameters for the microVM. crun needs to
 * read and parse it, using the information obtained from it to configure
 * libkrun as required.
 */
#تعریف_کرو KRUN_VM_FILE "/.krun_vm.json"

/* KRUN_VM_FILE holds a handful of settings and it is parsed by crun
 * itself before the container process exists, refuse anything bigger.
 */
#تعریف_کرو KRUN_VM_FILE_MAX_سائز (64 * 1024)

#تعریف_کرو KRUN_FLAVOR_AWS_NITRO "aws-nitro"
#تعریف_کرو KRUN_FLAVOR_SEV "sev"

#تعریف_کرو FD_PAIR_والد 0
#تعریف_کرو FD_PAIR_بچہ 1

ساخت krun_config
{
  خالی *handle;
  خالی *handle_sev;
  خالی *handle_awsnitro;
  bool sev;
  bool awsnitro;
  int32_t ctx_id;
  int32_t ctx_id_sev;
  int32_t ctx_id_awsnitro;
  bool has_kvm;
  bool has_awsnitro;
  صحیح passt_fds[2];
  json_object *config_doc;
  json_object *config_درخت;
  bool use_passt;
  ثابت حرف *tap_name;
  صحیح gpu_render_server_fd;
  صحیح gpu_flags;
};

/* libkrun handler.  */
#اگر HAVE_DLOPEN && HAVE_LIBKRUN
ساکن int32_t
libkrun_create_context (خالی *handle, libcrun_error_t *err)
{
  int32_t (*krun_create_ctx) ();
  int32_t ctx_id;

  krun_create_ctx = dlsym (handle, "krun_create_ctx");
  اگر (krun_create_ctx == تہی)
    واپس crun_make_error (err, 0, "could not find symbol in the krun library");

  ctx_id = krun_create_ctx ();
  اگر (UNLIKELY (ctx_id < 0))
    واپس crun_make_error (err, -ctx_id, "could not create krun context");

  واپس ctx_id;
}

ساکن صحیح
libkrun_configure_kernel (uint32_t ctx_id, خالی *handle, json_object *config_درخت, libcrun_error_t *err)
{
  int32_t (*krun_سیٹ_kernel) (uint32_t ctx_id, ثابت حرف *kernel_راستہ,
                              uint32_t kernel_format, ثابت حرف *initrd_راستہ, ثابت حرف *kernel_cmdline);
  json_object *kernel_راستہ = تہی;
  json_object *kernel_format = تہی;
  json_object *val_initrd_راستہ = تہی;
  json_object *val_kernel_cmdline = تہی;
  ثابت حرف *initrd_راستہ = تہی;
  ثابت حرف *kernel_cmdline = تہی;
  صحیح ret;

  /* kernel_path and kernel_format must be present */
  kernel_راستہ = json_object_object_get (config_درخت, "kernel_path");
  اگر (kernel_راستہ == تہی || ! json_object_is_type (kernel_راستہ, json_type_string))
    واپس 0;

  kernel_format = json_object_object_get (config_درخت, "kernel_format");
  اگر (kernel_format == تہی || ! json_object_is_type (kernel_format, json_type_int))
    واپس 0;

  /* initrd and kernel_cmdline are optional */
  val_initrd_راستہ = json_object_object_get (config_درخت, "initrd_path");
  اگر (val_initrd_راستہ != تہی && json_object_is_type (val_initrd_راستہ, json_type_string))
    initrd_راستہ = json_object_get_string (val_initrd_راستہ);

  val_kernel_cmdline = json_object_object_get (config_درخت, "kernel_cmdline");
  اگر (val_kernel_cmdline != تہی && json_object_is_type (val_kernel_cmdline, json_type_string))
    kernel_cmdline = json_object_get_string (val_kernel_cmdline);

  krun_سیٹ_kernel = dlsym (handle, "krun_set_kernel");
  اگر (krun_سیٹ_kernel == تہی)
    واپس crun_make_error (err, 0, "could not find symbol in krun library");

  ret = krun_سیٹ_kernel (ctx_id,
                         json_object_get_string (kernel_راستہ),
                         (uint32_t) json_object_get_int64 (kernel_format),
                         initrd_راستہ, kernel_cmdline);

  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, -ret, "could not configure a krun external kernel");

  واپس 0;
}

ساکن صحیح
libkrun_read_vm_config (ساخت krun_config *kconf, صحیح rootfsfd, ثابت حرف *rootfs, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_free حرف *config = تہی;
  cleanup_close صحیح pathfd = -1;
  cleanup_close صحیح fd = -1;
  proc_fd_راستہ_t fd_راستہ;
  ساخت stat st;

  /* The content of the rootfs is controlled by the user: look at the file through an
     O_PATH descriptor first and refuse anything that is not a small regular file, as
     opening a FIFO blocks and a device node must not be accessed.  */
  pathfd = محفوظ_openat (rootfsfd, rootfs, KRUN_VM_FILE, O_راستہ | O_CLOEXEC | O_NOFOLLOW, 0, err);
  اگر (pathfd < 0)
    {
      // The configuration file is optional, don't generate an error if it's missing.
      اگر (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          واپس 0;
        }
      واپس pathfd;
    }

  ret = fstat (pathfd, &st);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "stat `%s`", KRUN_VM_FILE);

  اگر (! S_ISREG (st.st_mode))
    واپس crun_make_error (err, 0, "`%s` is not a regular file", KRUN_VM_FILE);

  get_proc_self_fd_راستہ (fd_راستہ, pathfd);

  fd = open (fd_راستہ, O_RDONLY | O_CLOEXEC);
  اگر (UNLIKELY (fd < 0))
    واپس crun_make_error (err, errno, "open `%s`", KRUN_VM_FILE);

  ret = read_all_fd_with_max_سائز (fd, KRUN_VM_FILE, &config, تہی, KRUN_VM_FILE_MAX_سائز, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = parse_json_file (&kconf->config_doc, config, تہی, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  kconf->config_درخت = kconf->config_doc;
  واپس 0;
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
ساکن صحیح
libkrun_parse_resource_configuration (json_object *config_درخت, libcrun_container_t *container, ثابت حرف *annotation, ثابت حرف *کلید, bool use_config_file)
{
  حرف *val_str, *endptr;
  صحیح val = -1;
  json_object *val_json = تہی;

  val_str = (حرف *) ڈھونڈو_annotation (container, annotation);
  اگر (val_str != تہی)
    {
      errno = 0;
      val = (صحیح) strtol (val_str, &endptr, 10);
      اگر (errno != 0 || endptr == val_str || *endptr != '\0')
        /* Annotations value is not a valid integer. */
        error (EXIT_FAILURE, 0, "krun annotation %s value cannot be converted to an integer", annotation);

      اگر (val < 0)
        error (EXIT_FAILURE, 0, "krun annotation %s value must be a positive integer", annotation);

      واپس val;
    }
  ورنہ اگر (use_config_file && config_درخت != تہی)
    {
      val_json = json_object_object_get (config_درخت, کلید);
      اگر (val_json == تہی)
        واپس val;
      اگر (! json_object_is_type (val_json, json_type_int))
        error (EXIT_FAILURE, 0, "krun krun_vm.json %s value is not an integer", کلید);

      val = (صحیح) json_object_get_int64 (val_json);
    }

  واپس val;
}

ساکن صحیح
libkrun_parse_string_configuration (json_object *config_درخت, libcrun_container_t *container,
                                    ثابت حرف *annotation, ثابت حرف *کلید,
                                    ثابت حرف **قدر, libcrun_error_t *err, bool use_config_file)
{
  ثابت حرف *val;
  json_object *val_json = تہی;

  *قدر = تہی;

  val = ڈھونڈو_annotation (container, annotation);
  اگر (val != تہی)
    {
      *قدر = val;
      واپس 0;
    }

  اگر (! use_config_file || config_درخت == تہی)
    واپس 0;

  val_json = json_object_object_get (config_درخت, کلید);
  اگر (val_json == تہی)
    واپس 0;

  اگر (! json_object_is_type (val_json, json_type_string))
    واپس crun_make_error (err, 0, ".krun_vm.json %s value is not a string", کلید);

  *قدر = json_object_get_string (val_json);
  واپس 0;
}

ساکن خالی
libkrun_make_tap_mac (ثابت حرف *id, uint8_t mac[6])
{
  uint64_t hash = 14695981039346656037ULL;
  size_t i;

  برائے (; *id != '\0'; id++)
    {
      hash ^= (uint8_t) *id;
      hash *= 1099511628211ULL;
    }

  برائے (i = 0; i < 6; i++)
    mac[i] = (uint8_t) (hash >> (i * 8));

  mac[0] = (mac[0] & 0xfe) | 0x02;
}

ساکن صحیح
libkrun_configure_vm (uint32_t ctx_id, خالی *handle, ساخت krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_سیٹ_vm_config) (uint32_t ctx_id, uint8_t عدد_vcpus, uint32_t ram_mib);
  int32_t (*krun_جوڑو_net_unixstream) (uint32_t ctx_id, ثابت حرف *c_راستہ, صحیح fd, uint8_t *ثابت c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_جوڑو_net_tap) (uint32_t ctx_id, ثابت حرف *c_tap_name, ثابت uint8_t *c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_سیٹ_gpu_options) (uint32_t ctx_id, uint32_t virgl_flags);
  int32_t (*krun_سیٹ_gpu_render_server_fd) (uint32_t ctx_id, صحیح fd);
  صحیح cpus, ram_mib, nested_virt, ret;
  cpu_سیٹ_t سیٹ;

  /* We let the OCI set the number of vCPUs for the VM, since cgroups CPU restrictions still apply. */
  cpus = libkrun_parse_resource_configuration (kconf->config_درخت, container, "krun.cpus", "cpus", true);
  اگر (cpus <= 0)
    {
      CPU_ZERO (&سیٹ);
      اگر (sched_getaffinity (getpid (), حجم (سیٹ), &سیٹ) == 0)
        cpus = MIN (CPU_گنتی (&سیٹ), LIBKRUN_MAX_VCPUS);
      ورنہ
        cpus = 1;
    }

  /* We let the OCI set the amount of RAM for the VM, since cgroups memory restrictions still apply. */
  ram_mib = libkrun_parse_resource_configuration (kconf->config_درخت, container, "krun.ram_mib", "ram_mib", true);
  اگر (ram_mib <= LIBKRUN_MINIMUM_RAM_MIB)
    {
      اگر (def && def->linux && def->linux->resources && def->linux->resources->حافظہ
          && def->linux->resources->حافظہ->حد_بندی_present)
        ram_mib = def->linux->resources->حافظہ->حد_بندی / (1024 * 1024);
      ورنہ
        ram_mib = LIBKRUN_DEFAULT_RAM_MIB;
    }

  krun_سیٹ_vm_config = dlsym (handle, "krun_set_vm_config");

  اگر (krun_سیٹ_vm_config == تہی)
    واپس crun_make_error (err, 0, "could not find symbol in the krun library");

  ret = krun_سیٹ_vm_config (ctx_id, cpus, ram_mib);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, -ret, "could not set krun vm configuration");

  nested_virt = libkrun_parse_resource_configuration (kconf->config_درخت, container, "krun.nested_virt", "nested_virt", false);
  اگر (nested_virt > 0)
    {
      int32_t (*krun_check_nested_virt) (خالی);
      int32_t (*krun_سیٹ_nested_virt) (uint32_t ctx_id, bool enabled);

      krun_check_nested_virt = dlsym (handle, "krun_check_nested_virt");
      اگر (krun_check_nested_virt != تہی && krun_check_nested_virt () != 1)
        libcrun_warning ("nested virtualization requested but may not be supported on this host");

      krun_سیٹ_nested_virt = dlsym (handle, "krun_set_nested_virt");
      اگر (krun_سیٹ_nested_virt == تہی)
        واپس crun_make_error (err, 0, "could not find symbol `krun_set_nested_virt` in the krun library");

      ret = krun_سیٹ_nested_virt (ctx_id, true);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, -ret, "could not enable nested virtualization");
    }

  اگر (kconf->tap_name != تہی)
    {
      krun_جوڑو_net_tap = dlsym (handle, "krun_add_net_tap");
      اگر (krun_جوڑو_net_tap == تہی)
        واپس crun_make_error (err, 0, "could not find symbol `krun_add_net_tap` in the krun library");

      uint8_t mac[6];
      libkrun_make_tap_mac (container->context->id, mac);
      ret = krun_جوڑو_net_tap (ctx_id, kconf->tap_name, &mac[0], COMPAT_NET_FEATURES, 0);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, -ret, "could not add krun TAP interface `%s`", kconf->tap_name);
    }
  ورنہ اگر (kconf->use_passt)
    {
      krun_جوڑو_net_unixstream = dlsym (handle, "krun_add_net_unixstream");
      اگر (krun_جوڑو_net_unixstream == تہی)
        واپس crun_make_error (err, 0, "could not find symbol `krun_add_net_unixstream` in the krun library");

      uint8_t mac[] = { 0x5a, 0x94, 0xef, 0xe4, 0x0c, 0xee };
      ret = krun_جوڑو_net_unixstream (ctx_id, تہی, kconf->passt_fds[FD_PAIR_والد], &mac[0], COMPAT_NET_FEATURES, NET_FLAG_DHCP_CLIENT);
      اگر (UNLIKELY (ret == -EINVAL))
        ret = krun_جوڑو_net_unixstream (ctx_id, تہی, kconf->passt_fds[FD_PAIR_والد], &mac[0], COMPAT_NET_FEATURES, 0);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, -ret, "could not set krun net configuration");
    }

  اگر (kconf->gpu_flags > 0)
    {
      krun_سیٹ_gpu_options = dlsym (kconf->handle, "krun_set_gpu_options");
      اگر (krun_سیٹ_gpu_options == تہی)
        واپس crun_make_error (err, 0, "gpu requested but the version of libkrun in this system does not support it");

      ret = krun_سیٹ_gpu_options (kconf->ctx_id, kconf->gpu_flags);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, -ret, "gpu requested but could not configure virtio gpu device");

      اگر (kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER)
        {
          krun_سیٹ_gpu_render_server_fd = dlsym (kconf->handle, "krun_set_gpu_render_server_fd");
          اگر (krun_سیٹ_gpu_render_server_fd == تہی)
            واپس crun_make_error (err, 0, "gpu with render server requested but the version of libkrun in this system does not support it");

          ret = krun_سیٹ_gpu_render_server_fd (kconf->ctx_id, kconf->gpu_render_server_fd);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, -ret, "gpu with render server requested but could not configure its file descriptor");
        }
    }

  اگر (kconf->config_درخت != تہی)
    {
      /* Try to configure an external kernel. If the configuration file doesn't
       * specify a kernel, libkrun automatically fall back to using libkrunfw,
       * if the library is present and was loaded while creating the context.
       */
      صحیح custom_kernel = libkrun_parse_resource_configuration (kconf->config_درخت, container, "krun.custom_kernel", "custom_kernel", false);
      اگر (custom_kernel > 0)
        {
          ret = libkrun_configure_kernel (ctx_id, handle, kconf->config_درخت, err);
          اگر (UNLIKELY (ret))
            واپس ret;
        }
    }

  واپس 0;
}

ساکن صحیح
libkrun_configure_flavor (خالی *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  صحیح ret, sev_indicated = 0, awsnitro_indicated = 0;
  ساخت krun_config *kconf = (ساخت krun_config *) cookie;
  ثابت حرف *flavor = تہی;
  خالی *close_handles[2];

  close_handles[0] = تہی;
  close_handles[1] = تہی;

  // Check if the user provided the krun variant through OCI annotations.
  flavor = ڈھونڈو_annotation (container, "krun.variant");
  اگر (flavor != تہی)
    {
      sev_indicated |= strcmp (flavor, KRUN_FLAVOR_SEV) == 0;
      awsnitro_indicated |= strcmp (flavor, KRUN_FLAVOR_AWS_NITRO) == 0;
    }

  اگر (sev_indicated)
    {
      اگر (kconf->handle_sev == تہی)
        error (EXIT_FAILURE, 0, "the container requires libkrun-sev but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_awsnitro;

      kconf->handle = kconf->handle_sev;
      kconf->ctx_id = kconf->ctx_id_sev;
      kconf->sev = true;
    }
  ورنہ اگر (awsnitro_indicated)
    {
      اگر (kconf->handle_awsnitro == تہی)
        error (EXIT_FAILURE, 0, "the container requires libkrun-awsnitro but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_sev;

      kconf->handle = kconf->handle_awsnitro;
      kconf->ctx_id = kconf->ctx_id_awsnitro;
      kconf->awsnitro = true;
    }
  ورنہ
    {
      اگر (kconf->handle == تہی)
        error (EXIT_FAILURE, 0, "the container requires libkrun but it's not available");

      close_handles[0] = kconf->handle_sev;
      close_handles[1] = kconf->handle_awsnitro;
    }

  // We no longer need the other two libkrun handles.
  برائے (صحیح i = 0; i < 2; i++)
    {
      اگر (close_handles[i] == تہی)
        جاری;

      ret = dlclose (close_handles[i]);
      اگر (UNLIKELY (ret != 0))
        واپس crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }

  واپس 0;
}

ساکن صحیح
libkrun_exec (خالی *cookie, libcrun_container_t *container, ثابت حرف *pathname, حرف *ثابت argv[])
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_سیٹ_log_level) (uint32_t level);
  صحیح (*krun_آغاز_enter) (uint32_t ctx_id);
  int32_t (*krun_جوڑو_virtiofs2) (uint32_t ctx_id, ثابت حرف *c_لیبل, ثابت حرف *c_راستہ, uint64_t shm_سائز);
  int32_t (*krun_سیٹ_جڑ_disk) (uint32_t ctx_id, ثابت حرف *disk_راستہ);
  int32_t (*krun_سیٹ_tee_config_file) (uint32_t ctx_id, ثابت حرف *file_راستہ);
  int32_t (*krun_سیٹ_console_output) (uint32_t ctx_id, ثابت حرف *c_filepath);
  int32_t (*krun_سیٹ_exec) (uint32_t ctx_id, ثابت حرف *exec_راستہ,
                            ثابت حرف *ثابت argv[], ثابت حرف *ثابت envp[]);
  ساخت krun_config *kconf = (ساخت krun_config *) cookie;
  خالی *handle;
  int32_t ctx_id, ret;
  libcrun_error_t err;

  ret = libkrun_configure_flavor (cookie, container, &err);
  اگر (UNLIKELY (ret < 0))
    {
      صحیح errcode = crun_error_get_errno (&err);
      crun_error_release (&err);
      error (EXIT_FAILURE, errcode, "unable to configure libkrun flavor");
    }

  // /dev/kvm is required for all non AWS nitro workloads.
  اگر (! kconf->awsnitro && ! kconf->has_kvm)
    error (EXIT_FAILURE, 0, "`/dev/kvm` unavailable");

  handle = kconf->handle;
  ctx_id = kconf->ctx_id;

  krun_سیٹ_log_level = dlsym (handle, "krun_set_log_level");
  krun_آغاز_enter = dlsym (handle, "krun_start_enter");
  اگر (krun_سیٹ_log_level == تہی || krun_آغاز_enter == تہی)
    error (EXIT_FAILURE, 0, "could not find symbol in the krun library");

  /* Set log level according to crun's verbosity. */
  انتخاب (libcrun_get_verbosity ())
    {
    حالت LIBCRUN_VERBOSITY_DEBUG:
      krun_سیٹ_log_level (KRUN_LOG_LEVEL_DEBUG);
      توڑو;
    حالت LIBCRUN_VERBOSITY_WARNING:
      krun_سیٹ_log_level (KRUN_LOG_LEVEL_WARN);
      توڑو;
    طےشدہ:
      krun_سیٹ_log_level (KRUN_LOG_LEVEL_ERROR);
      توڑو;
    }

  اگر (kconf->sev)
    {
      krun_سیٹ_جڑ_disk = dlsym (handle, "krun_set_root_disk");
      krun_سیٹ_tee_config_file = dlsym (handle, "krun_set_tee_config_file");
      اگر (krun_سیٹ_جڑ_disk == تہی || krun_سیٹ_tee_config_file == تہی)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-sev.so`");

      ret = krun_سیٹ_جڑ_disk (ctx_id, "/disk.img");
      اگر (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set root disk");

      ret = krun_سیٹ_tee_config_file (ctx_id, KRUN_SEV_FILE);
      اگر (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set krun tee config file");
    }
  ورنہ
    {
      json_object *val_virtiofs_لیبل = تہی;
      json_object *val_virtiofs_shm_سائز = تہی;
      ثابت حرف *virtiofs_لیبل = تہی;
      // Default to a conservative DAX size of 512MB, just like krun_set_root() does.
      uint64_t virtiofs_shm_سائز = 512 * 1024 * 1024ULL;

      اگر (kconf->config_درخت != تہی)
        {
          val_virtiofs_لیبل = json_object_object_get (kconf->config_درخت, "virtiofs_tag");
          اگر (val_virtiofs_لیبل != تہی && json_object_is_type (val_virtiofs_لیبل, json_type_string))
            virtiofs_لیبل = json_object_get_string (val_virtiofs_لیبل);

          val_virtiofs_shm_سائز = json_object_object_get (kconf->config_درخت, "virtiofs_shm_size");
          اگر (val_virtiofs_shm_سائز != تہی && json_object_is_type (val_virtiofs_shm_سائز, json_type_int))
            virtiofs_shm_سائز = json_object_get_uint64 (val_virtiofs_shm_سائز);
        }

      اگر (virtiofs_لیبل == تہی)
        virtiofs_لیبل = "/dev/root";

      krun_جوڑو_virtiofs2 = dlsym (handle, "krun_add_virtiofs2");

      اگر (krun_جوڑو_virtiofs2 == تہی)
        error (EXIT_FAILURE, 0, "could not find symbol `krun_add_virtiofs2` in `libkrun.so`");

      ret = krun_جوڑو_virtiofs2 (ctx_id, virtiofs_لیبل, "/", virtiofs_shm_سائز);
      اگر (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not add virtiofs root with tag `%s`", virtiofs_لیبل);
    }

  اگر (kconf->awsnitro)
    {
      krun_سیٹ_console_output = dlsym (handle, "krun_set_console_output");
      krun_سیٹ_exec = dlsym (handle, "krun_set_exec");
      اگر (krun_سیٹ_console_output == تہی || krun_سیٹ_exec == تہی)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-awsnitro.so`");

      // Redirect all enclave output (read from vsock) to stdout.
      ret = krun_سیٹ_console_output (ctx_id, "/dev/stdout");
      اگر (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not redirect enclave output to stdout");

      ret = krun_سیٹ_exec (ctx_id, pathname, (ثابت حرف *ثابت *) argv,
                           (ثابت حرف *ثابت *) def->process->env);
      اگر (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set enclave execution arguments");
    }

  ret = libkrun_configure_vm (ctx_id, handle, kconf, container, &err);
  اگر (UNLIKELY (ret))
    {
      صحیح errcode = crun_error_get_errno (&err);
      libcrun_error_t *tmp_err = &err;
      libcrun_error_report_and_release (tmp_err);
      error (EXIT_FAILURE, errcode, "could not configure krun vm");
    }

  json_object_put (kconf->config_doc);

  ret = krun_آغاز_enter (ctx_id);
  اگر (UNLIKELY (ret < 0))
    error (EXIT_FAILURE, -ret, "could not start krun");

  واپس ret;
}

ساکن صحیح
libkrun_آغاز_render_server (ساخت krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  صحیح ret;

  kconf->gpu_flags = libkrun_parse_resource_configuration (kconf->config_درخت, container, "krun.gpu_flags", "gpu_flags", false);
  اگر (kconf->gpu_flags > 0)
    {
      اگر (access ("/dev/dri", F_OK) != 0)
        واپس crun_make_error (err, errno, "gpu requested but /dev/dri is not available");

      اگر ((kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER) != 0)
        {
          حرف *rs_argv[40];
          صحیح rs_argc = 0;
          حرف fd_as_str[16];
          صحیح gpu_fds[2];
          pid_t pid;
          صحیح null;

          اگر (access ("/usr/libexec/virgl_render_server", X_OK) != 0)
            واپس crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/libexec/virgl_render_server binary");

          اگر (access ("/usr/bin/bwrap", X_OK) != 0)
            واپس crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/bin/bwrap binary");

          ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, gpu_fds);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "could not create socketpair for virgl_render_server");

          snprintf (fd_as_str, حجم (fd_as_str), "%d", gpu_fds[FD_PAIR_بچہ]);

          rs_argv[rs_argc++] = (حرف *) "bwrap";
          rs_argv[rs_argc++] = (حرف *) "--ro-bind";
          rs_argv[rs_argc++] = (حرف *) "/usr";
          rs_argv[rs_argc++] = (حرف *) "/usr";
          اگر (access ("/lib", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (حرف *) "--ro-bind";
              rs_argv[rs_argc++] = (حرف *) "/lib";
              rs_argv[rs_argc++] = (حرف *) "/lib";
            }
          اگر (access ("/lib64", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (حرف *) "--ro-bind";
              rs_argv[rs_argc++] = (حرف *) "/lib64";
              rs_argv[rs_argc++] = (حرف *) "/lib64";
            }
          rs_argv[rs_argc++] = (حرف *) "--ro-bind";
          rs_argv[rs_argc++] = (حرف *) "/etc";
          rs_argv[rs_argc++] = (حرف *) "/etc";
          rs_argv[rs_argc++] = (حرف *) "--ro-bind";
          rs_argv[rs_argc++] = (حرف *) "/sys";
          rs_argv[rs_argc++] = (حرف *) "/sys";
          rs_argv[rs_argc++] = (حرف *) "--dev";
          rs_argv[rs_argc++] = (حرف *) "/dev";
          rs_argv[rs_argc++] = (حرف *) "--dev-bind";
          rs_argv[rs_argc++] = (حرف *) "/dev/dri";
          rs_argv[rs_argc++] = (حرف *) "/dev/dri";
          rs_argv[rs_argc++] = (حرف *) "--proc";
          rs_argv[rs_argc++] = (حرف *) "/proc";
          rs_argv[rs_argc++] = (حرف *) "--tmpfs";
          rs_argv[rs_argc++] = (حرف *) "/tmp";
          rs_argv[rs_argc++] = (حرف *) "--unshare-pid";
          rs_argv[rs_argc++] = (حرف *) "--unshare-ipc";
          rs_argv[rs_argc++] = (حرف *) "--unshare-net";
          rs_argv[rs_argc++] = (حرف *) "--";
          rs_argv[rs_argc++] = (حرف *) "/usr/libexec/virgl_render_server";
          rs_argv[rs_argc++] = (حرف *) "--socket-fd";
          rs_argv[rs_argc++] = fd_as_str;
          rs_argv[rs_argc] = تہی;

          pid = fork ();
          اگر (pid < 0)
            {
              close (gpu_fds[FD_PAIR_والد]);
              close (gpu_fds[FD_PAIR_بچہ]);
              واپس crun_make_error (err, errno, "could not fork for virgl_render_server");
            }
          ورنہ اگر (pid == 0)
            {
              close (gpu_fds[FD_PAIR_والد]);

              null = open ("/dev/null", O_WRONLY);
              اگر (null == -1)
                _exit (EXIT_FAILURE);

              dup2 (null, STDOUT_FILENO);
              dup2 (null, STDERR_FILENO);
              close (null);

              execvp ("/usr/bin/bwrap", rs_argv);
              _exit (EXIT_FAILURE);
            }

          close (gpu_fds[FD_PAIR_بچہ]);
          kconf->gpu_render_server_fd = gpu_fds[FD_PAIR_والد];
        }
    }

  واپس 0;
}

ساکن صحیح
libkrun_configure_جال (خالی *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  ساخت krun_config *kconf = (ساخت krun_config *) cookie;
  pid_t pid;
  حرف *passt_argv[10];
  حرف fd_as_str[16];
  صحیح use_passt;
  صحیح argv_idx;
  صحیح حالت_2;
  صحیح null;
  صحیح ret;

  ret = libkrun_parse_string_configuration (kconf->config_درخت, container,
                                            "krun.tap_name", "tap_name",
                                            &kconf->tap_name, err, false);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (kconf->tap_name != تہی && is_خالی_خانے_string (kconf->tap_name))
    واپس crun_make_error (err, 0, "krun.tap_name cannot be empty");

  use_passt = libkrun_parse_resource_configuration (kconf->config_درخت, container, "krun.use_passt", "use_passt", false);

  اگر (use_passt > 0)
    {
      اگر (kconf->tap_name != تہی)
        واپس crun_make_error (err, 0, "krun.tap_name and krun.use_passt are mutually exclusive");

      kconf->use_passt = 1;
    }
  ورنہ
    واپس 0;

  ret = socketpair (AF_UNIX, SOCK_دھارا, 0, kconf->passt_fds);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "create passt socketpair");

  snprintf (fd_as_str, حجم (fd_as_str), "%d", kconf->passt_fds[FD_PAIR_بچہ]);

  argv_idx = 0;
  passt_argv[argv_idx++] = (حرف *) "passt";
  passt_argv[argv_idx++] = (حرف *) "-t";
  passt_argv[argv_idx++] = (حرف *) "all";

  اگر (! kconf->has_awsnitro)
    {
      passt_argv[argv_idx++] = (حرف *) "-u";
      passt_argv[argv_idx++] = (حرف *) "all";
      passt_argv[argv_idx++] = (حرف *) "--no-dhcp-dns";
    }

  /* Set --no-map-gw. If the gateway for the network is also the DNS server
   * we won't be able to query the DNS server without --no-map-gw.
   * See discussion in https://github.com/containers/crun/pull/2099
   */
  passt_argv[argv_idx++] = (حرف *) "--no-map-gw";

  passt_argv[argv_idx++] = (حرف *) "--fd";
  passt_argv[argv_idx++] = fd_as_str;
  passt_argv[argv_idx] = تہی;

  pid = fork ();
  اگر (pid < 0)
    واپس crun_make_error (err, errno, "fork passt");
  ورنہ اگر (pid == 0)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t i;

      close (kconf->passt_fds[FD_PAIR_والد]);

      null = open ("/dev/null", O_WRONLY);
      اگر (null == -1)
        _exit (EXIT_FAILURE);

      /* Remount /proc if the parent detached its PID namespace, as passt(1)
       * needs a consistent /proc view to operate properly.
       */
      برائے (i = 0; i < def->linux->namespaces_len; i++)
        {
          صحیح قدر = libcrun_ڈھونڈو_namespace (def->linux->namespaces[i]->type);

          اگر (قدر == CLONE_NEWPID)
            {
              اگر (mount ("", "/proc", "proc", MS_NOSUID | MS_NODEV | MS_NOEXEC, تہی))
                _exit (EXIT_FAILURE);

              توڑو;
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

  close (kconf->passt_fds[FD_PAIR_بچہ]);

  // Wait for passt to daemonize itself.
  waitpid (pid, &حالت_2, 0);
  اگر (! (WIFEXITED (حالت_2)) || WEXITSTATUS (حالت_2) != 0)
    واپس crun_make_error (err, 0, "start passt");

  واپس 0;
}

/* Return true if the spec already declares a device with the given PATH.  */
ساکن bool
spec_has_device (runtime_spec_schema_config_schema *def, ثابت حرف *راستہ)
{
  size_t i;

  برائے (i = 0; i < def->linux->devices_len; i++)
    اگر (strcmp (def->linux->devices[i]->راستہ, راستہ) == 0)
      واپس true;
  واپس false;
}

/* libkrun_create_kvm_device: explicitly adds kvm device.  */
ساکن صحیح
libkrun_configure_container (خالی *cookie, شمار handler_configure_phase phase,
                             libcrun_context_t *context, libcrun_container_t *container,
                             ثابت حرف *rootfs, libcrun_error_t *err)
{
  صحیح ret, rootfsfd;
  ساخت krun_config *kconf = (ساخت krun_config *) cookie;
  ساخت device_s kvm_device = { "/dev/kvm", "c", 10, 232, 0666, 0, 0 };
  ساخت device_s sev_device = { "/dev/sev", "c", 10, 124, 0666, 0, 0 };
  ساخت device_s nitro_device = { "/dev/nitro_enclaves", "c", 10, 122, 0666, 0, 0 };
  cleanup_close صحیح devfd = -1;
  cleanup_close صحیح rootfsfd_cleanup = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  bool create_sev = false, create_awsnitro = false;
  bool is_user_ns;

  اگر (rootfs == تہی)
    rootfsfd = AT_FDCWD;
  ورنہ
    {
      rootfsfd = rootfsfd_cleanup = open (rootfs, O_راستہ | O_CLOEXEC);
      اگر (UNLIKELY (rootfsfd < 0))
        واپس crun_make_error (err, errno, "open `%s`", rootfs);
    }

  اگر (phase == HANDLER_CONFIGURE_BEFORE_USERNS)
    {
      cleanup_free حرف *origin_config_راستہ = تہی;
      cleanup_free حرف *state_dir = تہی;
      cleanup_free حرف *config = تہی;
      cleanup_close صحیح fd = -1;
      size_t config_سائز;

      ret = libcrun_get_state_directory (&state_dir, context->state_جڑ, context->id, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = append_paths (&origin_config_راستہ, err, state_dir, "config.json", تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = read_all_file (origin_config_راستہ, &config, &config_سائز, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      /* CVE-2025-24965: the content below rootfs cannot be trusted because it is controlled by the user.  We
         must ensure the file is opened below the rootfs directory.  Whatever the image ships at that name
         could also be a device node or a FIFO, so never open it: unlink it and create a new regular file.  */
      ret = unlinkat (rootfsfd, KRUN_CONFIG_FILE, 0);
      اگر (UNLIKELY (ret < 0 && errno != ENOENT))
        واپس crun_make_error (err, errno, "unlink `%s`", KRUN_CONFIG_FILE);

      fd = محفوظ_openat (rootfsfd, rootfs, KRUN_CONFIG_FILE, WRITE_FILE_DEFAULT_FLAGS | O_EXCL | O_NOFOLLOW, S_IRUSR | S_IRGRP | S_IROTH, err);
      اگر (UNLIKELY (fd < 0))
        واپس fd;

      ret = محفوظ_write (fd, KRUN_CONFIG_FILE, config, config_سائز, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = libkrun_read_vm_config (kconf, rootfsfd, rootfs, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = libkrun_آغاز_render_server (kconf, container, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    واپس 0;

  ret = libkrun_configure_جال (cookie, container, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  /* Do nothing if /dev/kvm is already present in spec */
  اگر (spec_has_device (def, "/dev/kvm"))
    واپس 0;

  اگر (kconf->handle_sev != تہی)
    create_sev = ! spec_has_device (def, "/dev/sev");

  اگر (kconf->handle_awsnitro != تہی)
    create_awsnitro = ! spec_has_device (def, "/dev/nitro_enclaves");

  devfd = محفوظ_openat (rootfsfd, rootfs, "dev", O_راستہ | O_DIRECTORY | O_CLOEXEC, 0, err);
  اگر (UNLIKELY (devfd < 0))
    واپس devfd;

  ret = check_running_اندر_user_namespace (err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;
  is_user_ns = ret;

  اگر (kconf->has_kvm)
    {
      ret = libcrun_create_dev (container, devfd, -1, &kvm_device, is_user_ns, true, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (create_sev)
    {
      ret = libcrun_create_dev (container, devfd, -1, &sev_device, is_user_ns, true, err);
      اگر (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_sev);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
          kconf->handle_sev = تہی;
        }
    }

  اگر (create_awsnitro)
    {
      ret = libcrun_create_dev (container, devfd, -1, &nitro_device, is_user_ns, true, err);
      اگر (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_awsnitro);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
          kconf->handle_awsnitro = تہی;
        }
    }

  واپس 0;
}

ساکن صحیح
libkrun_بھرو (خالی **cookie, libcrun_error_t *err)
{
  int32_t ret;
  ساخت krun_config *kconf;
  ثابت حرف *libkrun_so = "libkrun.so.1";
  ثابت حرف *libkrun_sev_so = "libkrun-sev.so.1";
  ثابت حرف *libkrun_awsnitro_so = "libkrun-awsnitro.so.1";

  kconf = حافظہ_دو (حجم (ساخت krun_config));
  اگر (kconf == تہی)
    واپس crun_make_error (err, 0, "could not allocate memory for krun_config");
  memset (kconf, 0, حجم (ساخت krun_config));

  kconf->gpu_render_server_fd = -1;

  kconf->handle = dlopen (libkrun_so, RTLD_NOW);
  kconf->handle_sev = dlopen (libkrun_sev_so, RTLD_NOW);
  kconf->handle_awsnitro = dlopen (libkrun_awsnitro_so, RTLD_NOW);

  اگر (kconf->handle == تہی && kconf->handle_sev == تہی && kconf->handle_awsnitro == تہی)
    {
      آزاد (kconf);
      واپس crun_make_error (err, 0, "failed to open `%s`, `%s`, and `%s` for krun_config: %s", libkrun_so, libkrun_sev_so, libkrun_awsnitro_so, dlerror ());
    }

  kconf->sev = false;
  kconf->awsnitro = false;

  /* Newer versions of libkrun no longer link against libkrunfw and
     instead they open it when creating the context. This implies
     we need to call "krun_create_ctx" before switching namespaces
     or it won't be able to find the library bundling the kernel. */
  اگر (kconf->handle)
    {
      ret = libkrun_create_context (kconf->handle, err);
      اگر (UNLIKELY (ret < 0))
        جاؤ error;
      kconf->ctx_id = ret;
    }

  اگر (kconf->handle_sev)
    {
      ret = libkrun_create_context (kconf->handle_sev, err);
      اگر (UNLIKELY (ret < 0))
        جاؤ error;
      kconf->ctx_id_sev = ret;
    }
  اگر (kconf->handle_awsnitro)
    {
      ret = libkrun_create_context (kconf->handle_awsnitro, err);
      اگر (UNLIKELY (ret < 0))
        جاؤ error;
      kconf->ctx_id_awsnitro = ret;
    }

  *cookie = kconf;

  واپس 0;

error:
  اگر (kconf->handle)
    dlclose (kconf->handle);
  اگر (kconf->handle_sev)
    dlclose (kconf->handle_sev);
  اگر (kconf->handle_awsnitro)
    dlclose (kconf->handle_awsnitro);
  آزاد (kconf);
  واپس ret;
}

ساکن صحیح
libkrun_unload (خالی *cookie, libcrun_error_t *err)
{
  صحیح r;

  ساخت krun_config *kconf = (ساخت krun_config *) cookie;
  اگر (kconf != تہی)
    {
      اگر (kconf->handle != تہی)
        {
          r = dlclose (kconf->handle);
          اگر (UNLIKELY (r != 0))
            واپس crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
        }
      اگر (kconf->handle_sev != تہی)
        {
          r = dlclose (kconf->handle_sev);
          اگر (UNLIKELY (r != 0))
            واپس crun_make_error (err, 0, "could not unload handle_sev: `%s`", dlerror ());
        }
      اگر (kconf->handle_awsnitro != تہی)
        {
          r = dlclose (kconf->handle_awsnitro);
          اگر (UNLIKELY (r != 0))
            واپس crun_make_error (err, 0, "could not unload handle_awsnitro: `%s`", dlerror ());
        }
      آزاد (kconf);
    }
  واپس 0;
}

ساکن runtime_spec_schema_defs_linux_device_cgroup *
make_oci_spec_dev (ثابت حرف *type, dev_t device, bool allow, ثابت حرف *access)
{
  runtime_spec_schema_defs_linux_device_cgroup *dev = xmalloc0 (حجم (*dev));

  dev->allow = allow;
  dev->allow_present = 1;

  dev->type = xstrdup (type);

  dev->major = major (device);
  dev->major_present = 1;

  dev->minor = minor (device);
  dev->minor_present = 1;

  dev->access = xstrdup (access);

  واپس dev;
}

/* stat an optional device: on ENOENT clear *PRESENT, any other error fails.  */
ساکن صحیح
stat_optional_device (ثابت حرف *راستہ, ساخت stat *st, bool *present, libcrun_error_t *err)
{
  صحیح ret = stat (راستہ, st);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (errno != ENOENT)
        واپس crun_make_error (err, errno, "stat `%s`", راستہ);
      *present = false;
    }
  واپس 0;
}

ساکن صحیح
libkrun_modify_oci_configuration (خالی *cookie arg_غیر_مستعمل, libcrun_context_t *context arg_غیر_مستعمل,
                                  runtime_spec_schema_config_schema *def,
                                  libcrun_error_t *err)
{
  ثابت size_t device_سائز = حجم (runtime_spec_schema_defs_linux_device_cgroup);
  ساخت krun_config *kconf = (ساخت krun_config *) cookie;
  ساخت stat st_kvm, st_sev, st_awsnitro;
  bool has_kvm = true, has_sev = true, has_awsnitro = true;
  size_t old_len, new_len;
  صحیح ret;

  /* Always allow the /dev/kvm device.  */

  ret = stat_optional_device ("/dev/kvm", &st_kvm, &has_kvm, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = stat_optional_device ("/dev/sev", &st_sev, &has_sev, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = stat_optional_device ("/dev/nitro_enclaves", &st_awsnitro, &has_awsnitro, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  kconf->has_kvm = has_kvm;
  kconf->has_awsnitro = has_awsnitro;

  اگر (! has_kvm && ! has_awsnitro)
    واپس 0;

  /* spec says these are optional, ensure they exist so we can add our devices */
  اگر (def->linux == تہی)
    def->linux = xmalloc0 (حجم (runtime_spec_schema_config_linux));

  اگر (def->linux->resources == تہی)
    def->linux->resources = xmalloc0 (حجم (runtime_spec_schema_config_linux_resources));

  old_len = def->linux->resources->devices_len;
  new_len = old_len;
  اگر (has_kvm)
    new_len += has_sev ? 2 : 1;
  اگر (has_awsnitro)
    new_len += 1;

  def->linux->resources->devices = xrealloc (def->linux->resources->devices, device_سائز * (new_len + 1));
  def->linux->resources->devices_len = new_len;

  اگر (has_kvm)
    {
      def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_kvm.st_rdev, true, "rwm");
      اگر (has_sev)
        def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_sev.st_rdev, true, "rwm");
    }

  اگر (has_awsnitro)
    def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_awsnitro.st_rdev, true, "rwm");

  واپس 0;
}

ساکن صحیح
libkrun_close_fds (خالی *cookie, libcrun_container_t *container, صحیح preserve_fds, libcrun_error_t *err)
{
  ساخت krun_config *kconf = (ساخت krun_config *) cookie;
  صحیح پہلا_fd_مقصد_close = preserve_fds + 3;
  صحیح کم_fd, بلند_fd;
  صحیح i;

  کم_fd = بلند_fd = -1;

  اگر (kconf->use_passt)
    بلند_fd = kconf->passt_fds[FD_PAIR_والد];

  اگر (kconf->gpu_render_server_fd != -1)
    {
      اگر (kconf->gpu_render_server_fd > بلند_fd)
        {
          کم_fd = بلند_fd;
          بلند_fd = kconf->gpu_render_server_fd;
        }
      ورنہ
        کم_fd = kconf->gpu_render_server_fd;
    }

  اگر (پہلا_fd_مقصد_close <= بلند_fd)
    {
      برائے (i = پہلا_fd_مقصد_close; i < بلند_fd; i++)
        {
          // If we're closing proc_fd, make sure to invalidate it.
          اگر (i == container->proc_fd)
            container->proc_fd = -1;
          ورنہ اگر (i == کم_fd)
            جاری;
          close (i);
        }

      پہلا_fd_مقصد_close = بلند_fd + 1;
    }

  واپس mark_or_close_fds_ge_than (container, پہلا_fd_مقصد_close, true, err);
}

ساخت custom_handler_s handler_libkrun = {
  .name = "krun",
  .alias = تہی,
  .feature_string = "LIBKRUN",
  .supports_open_درخت_namespace = false,
  .بھرو = libkrun_بھرو,
  .unload = libkrun_unload,
  .تسلسل_func = libkrun_exec,
  .configure_container = libkrun_configure_container,
  .modify_oci_configuration = libkrun_modify_oci_configuration,
  .close_fds = libkrun_close_fds,
};

#اگر_ختم
