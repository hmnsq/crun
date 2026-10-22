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

ساختار krun_config
{
  تهی *handle;
  تهی *handle_sev;
  تهی *handle_awsnitro;
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
  ثابت نویسه *tap_name;
  صحیح gpu_render_server_fd;
  صحیح gpu_flags;
};

/* libkrun handler.  */
#اگر HAVE_DLOPEN && HAVE_LIBKRUN
ایستا int32_t
libkrun_create_context (تهی *handle, libcrun_error_t *err)
{
  int32_t (*krun_create_ctx) ();
  int32_t ctx_id;

  krun_create_ctx = dlsym (handle, "krun_create_ctx");
  اگر (krun_create_ctx == پوچ)
    بازگشت crun_make_error (err, 0, "could not find symbol in the krun library");

  ctx_id = krun_create_ctx ();
  اگر (UNLIKELY (ctx_id < 0))
    بازگشت crun_make_error (err, -ctx_id, "could not create krun context");

  بازگشت ctx_id;
}

ایستا صحیح
libkrun_configure_kernel (uint32_t ctx_id, تهی *handle, json_object *config_درخت, libcrun_error_t *err)
{
  int32_t (*krun_set_kernel) (uint32_t ctx_id, ثابت نویسه *kernel_path,
                              uint32_t kernel_format, ثابت نویسه *initrd_path, ثابت نویسه *kernel_cmdline);
  json_object *kernel_path = پوچ;
  json_object *kernel_format = پوچ;
  json_object *val_initrd_path = پوچ;
  json_object *val_kernel_cmdline = پوچ;
  ثابت نویسه *initrd_path = پوچ;
  ثابت نویسه *kernel_cmdline = پوچ;
  صحیح ret;

  /* kernel_path and kernel_format must be present */
  kernel_path = json_object_object_get (config_درخت, "kernel_path");
  اگر (kernel_path == پوچ || ! json_object_is_type (kernel_path, json_type_string))
    بازگشت 0;

  kernel_format = json_object_object_get (config_درخت, "kernel_format");
  اگر (kernel_format == پوچ || ! json_object_is_type (kernel_format, json_type_int))
    بازگشت 0;

  /* initrd and kernel_cmdline are optional */
  val_initrd_path = json_object_object_get (config_درخت, "initrd_path");
  اگر (val_initrd_path != پوچ && json_object_is_type (val_initrd_path, json_type_string))
    initrd_path = json_object_get_string (val_initrd_path);

  val_kernel_cmdline = json_object_object_get (config_درخت, "kernel_cmdline");
  اگر (val_kernel_cmdline != پوچ && json_object_is_type (val_kernel_cmdline, json_type_string))
    kernel_cmdline = json_object_get_string (val_kernel_cmdline);

  krun_set_kernel = dlsym (handle, "krun_set_kernel");
  اگر (krun_set_kernel == پوچ)
    بازگشت crun_make_error (err, 0, "could not find symbol in krun library");

  ret = krun_set_kernel (ctx_id,
                         json_object_get_string (kernel_path),
                         (uint32_t) json_object_get_int64 (kernel_format),
                         initrd_path, kernel_cmdline);

  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, -ret, "could not configure a krun external kernel");

  بازگشت 0;
}

ایستا صحیح
libkrun_read_vm_config (ساختار krun_config *kconf, صحیح rootfsfd, ثابت نویسه *rootfs, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_free نویسه *config = پوچ;
  cleanup_close صحیح pathfd = -1;
  cleanup_close صحیح fd = -1;
  proc_fd_path_t fd_path;
  ساختار stat st;

  /* The content of the rootfs is controlled by the user: look at the file through an
     O_PATH descriptor first and refuse anything that is not a small regular file, as
     opening a FIFO blocks and a device node must not be accessed.  */
  pathfd = safe_openat (rootfsfd, rootfs, KRUN_VM_FILE, O_PATH | O_CLOEXEC | O_NOFOLLOW, 0, err);
  اگر (pathfd < 0)
    {
      // The configuration file is optional, don't generate an error if it's missing.
      اگر (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          بازگشت 0;
        }
      بازگشت pathfd;
    }

  ret = fstat (pathfd, &st);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "stat `%s`", KRUN_VM_FILE);

  اگر (! S_ISREG (st.st_mode))
    بازگشت crun_make_error (err, 0, "`%s` is not a regular file", KRUN_VM_FILE);

  get_proc_self_fd_path (fd_path, pathfd);

  fd = open (fd_path, O_RDONLY | O_CLOEXEC);
  اگر (UNLIKELY (fd < 0))
    بازگشت crun_make_error (err, errno, "open `%s`", KRUN_VM_FILE);

  ret = read_all_fd_with_max_size (fd, KRUN_VM_FILE, &config, پوچ, KRUN_VM_FILE_MAX_SIZE, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = parse_json_file (&kconf->config_doc, config, پوچ, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  kconf->config_درخت = kconf->config_doc;
  بازگشت 0;
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
ایستا صحیح
libkrun_parse_resource_configuration (json_object *config_درخت, libcrun_container_t *container, ثابت نویسه *annotation, ثابت نویسه *کلید, bool use_config_file)
{
  نویسه *val_str, *endptr;
  صحیح val = -1;
  json_object *val_json = پوچ;

  val_str = (نویسه *) find_annotation (container, annotation);
  اگر (val_str != پوچ)
    {
      errno = 0;
      val = (صحیح) strtol (val_str, &endptr, 10);
      اگر (errno != 0 || endptr == val_str || *endptr != '\0')
        /* Annotations value is not a valid integer. */
        error (EXIT_FAILURE, 0, "krun annotation %s value cannot be converted to an integer", annotation);

      اگر (val < 0)
        error (EXIT_FAILURE, 0, "krun annotation %s value must be a positive integer", annotation);

      بازگشت val;
    }
  وگرنه اگر (use_config_file && config_درخت != پوچ)
    {
      val_json = json_object_object_get (config_درخت, کلید);
      اگر (val_json == پوچ)
        بازگشت val;
      اگر (! json_object_is_type (val_json, json_type_int))
        error (EXIT_FAILURE, 0, "krun krun_vm.json %s value is not an integer", کلید);

      val = (صحیح) json_object_get_int64 (val_json);
    }

  بازگشت val;
}

ایستا صحیح
libkrun_parse_string_configuration (json_object *config_درخت, libcrun_container_t *container,
                                    ثابت نویسه *annotation, ثابت نویسه *کلید,
                                    ثابت نویسه **مقدار, libcrun_error_t *err, bool use_config_file)
{
  ثابت نویسه *val;
  json_object *val_json = پوچ;

  *مقدار = پوچ;

  val = find_annotation (container, annotation);
  اگر (val != پوچ)
    {
      *مقدار = val;
      بازگشت 0;
    }

  اگر (! use_config_file || config_درخت == پوچ)
    بازگشت 0;

  val_json = json_object_object_get (config_درخت, کلید);
  اگر (val_json == پوچ)
    بازگشت 0;

  اگر (! json_object_is_type (val_json, json_type_string))
    بازگشت crun_make_error (err, 0, ".krun_vm.json %s value is not a string", کلید);

  *مقدار = json_object_get_string (val_json);
  بازگشت 0;
}

ایستا تهی
libkrun_make_tap_mac (ثابت نویسه *id, uint8_t mac[6])
{
  uint64_t hash = 14695981039346656037ULL;
  size_t i;

  برای (; *id != '\0'; id++)
    {
      hash ^= (uint8_t) *id;
      hash *= 1099511628211ULL;
    }

  برای (i = 0; i < 6; i++)
    mac[i] = (uint8_t) (hash >> (i * 8));

  mac[0] = (mac[0] & 0xfe) | 0x02;
}

ایستا صحیح
libkrun_configure_vm (uint32_t ctx_id, تهی *handle, ساختار krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_set_vm_config) (uint32_t ctx_id, uint8_t رقم_vcpus, uint32_t ram_mib);
  int32_t (*krun_add_net_unixstream) (uint32_t ctx_id, ثابت نویسه *c_path, صحیح fd, uint8_t *ثابت c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_add_net_tap) (uint32_t ctx_id, ثابت نویسه *c_tap_name, ثابت uint8_t *c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_set_gpu_options) (uint32_t ctx_id, uint32_t virgl_flags);
  int32_t (*krun_set_gpu_render_server_fd) (uint32_t ctx_id, صحیح fd);
  صحیح cpus, ram_mib, nested_virt, ret;
  cpu_set_t set;

  /* We let the OCI set the number of vCPUs for the VM, since cgroups CPU restrictions still apply. */
  cpus = libkrun_parse_resource_configuration (kconf->config_درخت, container, "krun.cpus", "cpus", true);
  اگر (cpus <= 0)
    {
      CPU_ZERO (&set);
      اگر (sched_getaffinity (getpid (), اندازه (set), &set) == 0)
        cpus = MIN (CPU_شمار (&set), LIBKRUN_MAX_VCPUS);
      وگرنه
        cpus = 1;
    }

  /* We let the OCI set the amount of RAM for the VM, since cgroups memory restrictions still apply. */
  ram_mib = libkrun_parse_resource_configuration (kconf->config_درخت, container, "krun.ram_mib", "ram_mib", true);
  اگر (ram_mib <= LIBKRUN_MINIMUM_RAM_MIB)
    {
      اگر (def && def->linux && def->linux->resources && def->linux->resources->memory
          && def->linux->resources->memory->limit_present)
        ram_mib = def->linux->resources->memory->limit / (1024 * 1024);
      وگرنه
        ram_mib = LIBKRUN_DEFAULT_RAM_MIB;
    }

  krun_set_vm_config = dlsym (handle, "krun_set_vm_config");

  اگر (krun_set_vm_config == پوچ)
    بازگشت crun_make_error (err, 0, "could not find symbol in the krun library");

  ret = krun_set_vm_config (ctx_id, cpus, ram_mib);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, -ret, "could not set krun vm configuration");

  nested_virt = libkrun_parse_resource_configuration (kconf->config_درخت, container, "krun.nested_virt", "nested_virt", false);
  اگر (nested_virt > 0)
    {
      int32_t (*krun_check_nested_virt) (تهی);
      int32_t (*krun_set_nested_virt) (uint32_t ctx_id, bool enabled);

      krun_check_nested_virt = dlsym (handle, "krun_check_nested_virt");
      اگر (krun_check_nested_virt != پوچ && krun_check_nested_virt () != 1)
        libcrun_warning ("nested virtualization requested but may not be supported on this host");

      krun_set_nested_virt = dlsym (handle, "krun_set_nested_virt");
      اگر (krun_set_nested_virt == پوچ)
        بازگشت crun_make_error (err, 0, "could not find symbol `krun_set_nested_virt` in the krun library");

      ret = krun_set_nested_virt (ctx_id, true);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, -ret, "could not enable nested virtualization");
    }

  اگر (kconf->tap_name != پوچ)
    {
      krun_add_net_tap = dlsym (handle, "krun_add_net_tap");
      اگر (krun_add_net_tap == پوچ)
        بازگشت crun_make_error (err, 0, "could not find symbol `krun_add_net_tap` in the krun library");

      uint8_t mac[6];
      libkrun_make_tap_mac (container->context->id, mac);
      ret = krun_add_net_tap (ctx_id, kconf->tap_name, &mac[0], COMPAT_NET_FEATURES, 0);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, -ret, "could not add krun TAP interface `%s`", kconf->tap_name);
    }
  وگرنه اگر (kconf->use_passt)
    {
      krun_add_net_unixstream = dlsym (handle, "krun_add_net_unixstream");
      اگر (krun_add_net_unixstream == پوچ)
        بازگشت crun_make_error (err, 0, "could not find symbol `krun_add_net_unixstream` in the krun library");

      uint8_t mac[] = { 0x5a, 0x94, 0xef, 0xe4, 0x0c, 0xee };
      ret = krun_add_net_unixstream (ctx_id, پوچ, kconf->passt_fds[FD_PAIR_PARENT], &mac[0], COMPAT_NET_FEATURES, NET_FLAG_DHCP_CLIENT);
      اگر (UNLIKELY (ret == -EINVAL))
        ret = krun_add_net_unixstream (ctx_id, پوچ, kconf->passt_fds[FD_PAIR_PARENT], &mac[0], COMPAT_NET_FEATURES, 0);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, -ret, "could not set krun net configuration");
    }

  اگر (kconf->gpu_flags > 0)
    {
      krun_set_gpu_options = dlsym (kconf->handle, "krun_set_gpu_options");
      اگر (krun_set_gpu_options == پوچ)
        بازگشت crun_make_error (err, 0, "gpu requested but the version of libkrun in this system does not support it");

      ret = krun_set_gpu_options (kconf->ctx_id, kconf->gpu_flags);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, -ret, "gpu requested but could not configure virtio gpu device");

      اگر (kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER)
        {
          krun_set_gpu_render_server_fd = dlsym (kconf->handle, "krun_set_gpu_render_server_fd");
          اگر (krun_set_gpu_render_server_fd == پوچ)
            بازگشت crun_make_error (err, 0, "gpu with render server requested but the version of libkrun in this system does not support it");

          ret = krun_set_gpu_render_server_fd (kconf->ctx_id, kconf->gpu_render_server_fd);
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, -ret, "gpu with render server requested but could not configure its file descriptor");
        }
    }

  اگر (kconf->config_درخت != پوچ)
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
            بازگشت ret;
        }
    }

  بازگشت 0;
}

ایستا صحیح
libkrun_configure_flavor (تهی *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  صحیح ret, sev_indicated = 0, awsnitro_indicated = 0;
  ساختار krun_config *kconf = (ساختار krun_config *) cookie;
  ثابت نویسه *flavor = پوچ;
  تهی *close_handles[2];

  close_handles[0] = پوچ;
  close_handles[1] = پوچ;

  // Check if the user provided the krun variant through OCI annotations.
  flavor = find_annotation (container, "krun.variant");
  اگر (flavor != پوچ)
    {
      sev_indicated |= strcmp (flavor, KRUN_FLAVOR_SEV) == 0;
      awsnitro_indicated |= strcmp (flavor, KRUN_FLAVOR_AWS_NITRO) == 0;
    }

  اگر (sev_indicated)
    {
      اگر (kconf->handle_sev == پوچ)
        error (EXIT_FAILURE, 0, "the container requires libkrun-sev but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_awsnitro;

      kconf->handle = kconf->handle_sev;
      kconf->ctx_id = kconf->ctx_id_sev;
      kconf->sev = true;
    }
  وگرنه اگر (awsnitro_indicated)
    {
      اگر (kconf->handle_awsnitro == پوچ)
        error (EXIT_FAILURE, 0, "the container requires libkrun-awsnitro but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_sev;

      kconf->handle = kconf->handle_awsnitro;
      kconf->ctx_id = kconf->ctx_id_awsnitro;
      kconf->awsnitro = true;
    }
  وگرنه
    {
      اگر (kconf->handle == پوچ)
        error (EXIT_FAILURE, 0, "the container requires libkrun but it's not available");

      close_handles[0] = kconf->handle_sev;
      close_handles[1] = kconf->handle_awsnitro;
    }

  // We no longer need the other two libkrun handles.
  برای (صحیح i = 0; i < 2; i++)
    {
      اگر (close_handles[i] == پوچ)
        ادامه;

      ret = dlclose (close_handles[i]);
      اگر (UNLIKELY (ret != 0))
        بازگشت crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }

  بازگشت 0;
}

ایستا صحیح
libkrun_exec (تهی *cookie, libcrun_container_t *container, ثابت نویسه *pathname, نویسه *ثابت argv[])
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_set_log_level) (uint32_t level);
  صحیح (*krun_start_enter) (uint32_t ctx_id);
  int32_t (*krun_add_virtiofs2) (uint32_t ctx_id, ثابت نویسه *c_tag, ثابت نویسه *c_path, uint64_t shm_size);
  int32_t (*krun_set_ریشه_disk) (uint32_t ctx_id, ثابت نویسه *disk_path);
  int32_t (*krun_set_tee_config_file) (uint32_t ctx_id, ثابت نویسه *file_path);
  int32_t (*krun_set_console_output) (uint32_t ctx_id, ثابت نویسه *c_filepath);
  int32_t (*krun_set_exec) (uint32_t ctx_id, ثابت نویسه *exec_path,
                            ثابت نویسه *ثابت argv[], ثابت نویسه *ثابت envp[]);
  ساختار krun_config *kconf = (ساختار krun_config *) cookie;
  تهی *handle;
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

  krun_set_log_level = dlsym (handle, "krun_set_log_level");
  krun_start_enter = dlsym (handle, "krun_start_enter");
  اگر (krun_set_log_level == پوچ || krun_start_enter == پوچ)
    error (EXIT_FAILURE, 0, "could not find symbol in the krun library");

  /* Set log level according to crun's verbosity. */
  انتخاب (libcrun_get_verbosity ())
    {
    حالت LIBCRUN_VERBOSITY_DEBUG:
      krun_set_log_level (KRUN_LOG_LEVEL_DEBUG);
      بشکن;
    حالت LIBCRUN_VERBOSITY_WARNING:
      krun_set_log_level (KRUN_LOG_LEVEL_WARN);
      بشکن;
    پیشفرض:
      krun_set_log_level (KRUN_LOG_LEVEL_ERROR);
      بشکن;
    }

  اگر (kconf->sev)
    {
      krun_set_ریشه_disk = dlsym (handle, "krun_set_root_disk");
      krun_set_tee_config_file = dlsym (handle, "krun_set_tee_config_file");
      اگر (krun_set_ریشه_disk == پوچ || krun_set_tee_config_file == پوچ)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-sev.so`");

      ret = krun_set_ریشه_disk (ctx_id, "/disk.img");
      اگر (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set root disk");

      ret = krun_set_tee_config_file (ctx_id, KRUN_SEV_FILE);
      اگر (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not set krun tee config file");
    }
  وگرنه
    {
      json_object *val_virtiofs_tag = پوچ;
      json_object *val_virtiofs_shm_size = پوچ;
      ثابت نویسه *virtiofs_tag = پوچ;
      // Default to a conservative DAX size of 512MB, just like krun_set_root() does.
      uint64_t virtiofs_shm_size = 512 * 1024 * 1024ULL;

      اگر (kconf->config_درخت != پوچ)
        {
          val_virtiofs_tag = json_object_object_get (kconf->config_درخت, "virtiofs_tag");
          اگر (val_virtiofs_tag != پوچ && json_object_is_type (val_virtiofs_tag, json_type_string))
            virtiofs_tag = json_object_get_string (val_virtiofs_tag);

          val_virtiofs_shm_size = json_object_object_get (kconf->config_درخت, "virtiofs_shm_size");
          اگر (val_virtiofs_shm_size != پوچ && json_object_is_type (val_virtiofs_shm_size, json_type_int))
            virtiofs_shm_size = json_object_get_uint64 (val_virtiofs_shm_size);
        }

      اگر (virtiofs_tag == پوچ)
        virtiofs_tag = "/dev/root";

      krun_add_virtiofs2 = dlsym (handle, "krun_add_virtiofs2");

      اگر (krun_add_virtiofs2 == پوچ)
        error (EXIT_FAILURE, 0, "could not find symbol `krun_add_virtiofs2` in `libkrun.so`");

      ret = krun_add_virtiofs2 (ctx_id, virtiofs_tag, "/", virtiofs_shm_size);
      اگر (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not add virtiofs root with tag `%s`", virtiofs_tag);
    }

  اگر (kconf->awsnitro)
    {
      krun_set_console_output = dlsym (handle, "krun_set_console_output");
      krun_set_exec = dlsym (handle, "krun_set_exec");
      اگر (krun_set_console_output == پوچ || krun_set_exec == پوچ)
        error (EXIT_FAILURE, 0, "could not find symbol in `libkrun-awsnitro.so`");

      // Redirect all enclave output (read from vsock) to stdout.
      ret = krun_set_console_output (ctx_id, "/dev/stdout");
      اگر (UNLIKELY (ret < 0))
        error (EXIT_FAILURE, -ret, "could not redirect enclave output to stdout");

      ret = krun_set_exec (ctx_id, pathname, (ثابت نویسه *ثابت *) argv,
                           (ثابت نویسه *ثابت *) def->process->env);
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

  ret = krun_start_enter (ctx_id);
  اگر (UNLIKELY (ret < 0))
    error (EXIT_FAILURE, -ret, "could not start krun");

  بازگشت ret;
}

ایستا صحیح
libkrun_start_render_server (ساختار krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  صحیح ret;

  kconf->gpu_flags = libkrun_parse_resource_configuration (kconf->config_درخت, container, "krun.gpu_flags", "gpu_flags", false);
  اگر (kconf->gpu_flags > 0)
    {
      اگر (access ("/dev/dri", F_OK) != 0)
        بازگشت crun_make_error (err, errno, "gpu requested but /dev/dri is not available");

      اگر ((kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER) != 0)
        {
          نویسه *rs_argv[40];
          صحیح rs_argc = 0;
          نویسه fd_as_str[16];
          صحیح gpu_fds[2];
          pid_t pid;
          صحیح null;

          اگر (access ("/usr/libexec/virgl_render_server", X_OK) != 0)
            بازگشت crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/libexec/virgl_render_server binary");

          اگر (access ("/usr/bin/bwrap", X_OK) != 0)
            بازگشت crun_make_error (err, errno, "gpu with render server requested but couldn't find /usr/bin/bwrap binary");

          ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, gpu_fds);
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "could not create socketpair for virgl_render_server");

          snprintf (fd_as_str, اندازه (fd_as_str), "%d", gpu_fds[FD_PAIR_CHILD]);

          rs_argv[rs_argc++] = (نویسه *) "bwrap";
          rs_argv[rs_argc++] = (نویسه *) "--ro-bind";
          rs_argv[rs_argc++] = (نویسه *) "/usr";
          rs_argv[rs_argc++] = (نویسه *) "/usr";
          اگر (access ("/lib", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (نویسه *) "--ro-bind";
              rs_argv[rs_argc++] = (نویسه *) "/lib";
              rs_argv[rs_argc++] = (نویسه *) "/lib";
            }
          اگر (access ("/lib64", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (نویسه *) "--ro-bind";
              rs_argv[rs_argc++] = (نویسه *) "/lib64";
              rs_argv[rs_argc++] = (نویسه *) "/lib64";
            }
          rs_argv[rs_argc++] = (نویسه *) "--ro-bind";
          rs_argv[rs_argc++] = (نویسه *) "/etc";
          rs_argv[rs_argc++] = (نویسه *) "/etc";
          rs_argv[rs_argc++] = (نویسه *) "--ro-bind";
          rs_argv[rs_argc++] = (نویسه *) "/sys";
          rs_argv[rs_argc++] = (نویسه *) "/sys";
          rs_argv[rs_argc++] = (نویسه *) "--dev";
          rs_argv[rs_argc++] = (نویسه *) "/dev";
          rs_argv[rs_argc++] = (نویسه *) "--dev-bind";
          rs_argv[rs_argc++] = (نویسه *) "/dev/dri";
          rs_argv[rs_argc++] = (نویسه *) "/dev/dri";
          rs_argv[rs_argc++] = (نویسه *) "--proc";
          rs_argv[rs_argc++] = (نویسه *) "/proc";
          rs_argv[rs_argc++] = (نویسه *) "--tmpfs";
          rs_argv[rs_argc++] = (نویسه *) "/tmp";
          rs_argv[rs_argc++] = (نویسه *) "--unshare-pid";
          rs_argv[rs_argc++] = (نویسه *) "--unshare-ipc";
          rs_argv[rs_argc++] = (نویسه *) "--unshare-net";
          rs_argv[rs_argc++] = (نویسه *) "--";
          rs_argv[rs_argc++] = (نویسه *) "/usr/libexec/virgl_render_server";
          rs_argv[rs_argc++] = (نویسه *) "--socket-fd";
          rs_argv[rs_argc++] = fd_as_str;
          rs_argv[rs_argc] = پوچ;

          pid = fork ();
          اگر (pid < 0)
            {
              close (gpu_fds[FD_PAIR_PARENT]);
              close (gpu_fds[FD_PAIR_CHILD]);
              بازگشت crun_make_error (err, errno, "could not fork for virgl_render_server");
            }
          وگرنه اگر (pid == 0)
            {
              close (gpu_fds[FD_PAIR_PARENT]);

              null = open ("/dev/null", O_WRONLY);
              اگر (null == -1)
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

  بازگشت 0;
}

ایستا صحیح
libkrun_configure_network (تهی *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  ساختار krun_config *kconf = (ساختار krun_config *) cookie;
  pid_t pid;
  نویسه *passt_argv[10];
  نویسه fd_as_str[16];
  صحیح use_passt;
  صحیح argv_idx;
  صحیح status;
  صحیح null;
  صحیح ret;

  ret = libkrun_parse_string_configuration (kconf->config_درخت, container,
                                            "krun.tap_name", "tap_name",
                                            &kconf->tap_name, err, false);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (kconf->tap_name != پوچ && is_empty_string (kconf->tap_name))
    بازگشت crun_make_error (err, 0, "krun.tap_name cannot be empty");

  use_passt = libkrun_parse_resource_configuration (kconf->config_درخت, container, "krun.use_passt", "use_passt", false);

  اگر (use_passt > 0)
    {
      اگر (kconf->tap_name != پوچ)
        بازگشت crun_make_error (err, 0, "krun.tap_name and krun.use_passt are mutually exclusive");

      kconf->use_passt = 1;
    }
  وگرنه
    بازگشت 0;

  ret = socketpair (AF_UNIX, SOCK_STREAM, 0, kconf->passt_fds);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "create passt socketpair");

  snprintf (fd_as_str, اندازه (fd_as_str), "%d", kconf->passt_fds[FD_PAIR_CHILD]);

  argv_idx = 0;
  passt_argv[argv_idx++] = (نویسه *) "passt";
  passt_argv[argv_idx++] = (نویسه *) "-t";
  passt_argv[argv_idx++] = (نویسه *) "all";

  اگر (! kconf->has_awsnitro)
    {
      passt_argv[argv_idx++] = (نویسه *) "-u";
      passt_argv[argv_idx++] = (نویسه *) "all";
      passt_argv[argv_idx++] = (نویسه *) "--no-dhcp-dns";
    }

  /* Set --no-map-gw. If the gateway for the network is also the DNS server
   * we won't be able to query the DNS server without --no-map-gw.
   * See discussion in https://github.com/containers/crun/pull/2099
   */
  passt_argv[argv_idx++] = (نویسه *) "--no-map-gw";

  passt_argv[argv_idx++] = (نویسه *) "--fd";
  passt_argv[argv_idx++] = fd_as_str;
  passt_argv[argv_idx] = پوچ;

  pid = fork ();
  اگر (pid < 0)
    بازگشت crun_make_error (err, errno, "fork passt");
  وگرنه اگر (pid == 0)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t i;

      close (kconf->passt_fds[FD_PAIR_PARENT]);

      null = open ("/dev/null", O_WRONLY);
      اگر (null == -1)
        _exit (EXIT_FAILURE);

      /* Remount /proc if the parent detached its PID namespace, as passt(1)
       * needs a consistent /proc view to operate properly.
       */
      برای (i = 0; i < def->linux->namespaces_len; i++)
        {
          صحیح مقدار = libcrun_find_namespace (def->linux->namespaces[i]->type);

          اگر (مقدار == CLONE_NEWPID)
            {
              اگر (mount ("", "/proc", "proc", MS_NOSUID | MS_NODEV | MS_NOEXEC, پوچ))
                _exit (EXIT_FAILURE);

              بشکن;
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
  اگر (! (WIFEXITED (status)) || WEXITSTATUS (status) != 0)
    بازگشت crun_make_error (err, 0, "start passt");

  بازگشت 0;
}

/* Return true if the spec already declares a device with the given PATH.  */
ایستا bool
spec_has_device (runtime_spec_schema_config_schema *def, ثابت نویسه *path)
{
  size_t i;

  برای (i = 0; i < def->linux->devices_len; i++)
    اگر (strcmp (def->linux->devices[i]->path, path) == 0)
      بازگشت true;
  بازگشت false;
}

/* libkrun_create_kvm_device: explicitly adds kvm device.  */
ایستا صحیح
libkrun_configure_container (تهی *cookie, شمارش handler_configure_phase phase,
                             libcrun_context_t *context, libcrun_container_t *container,
                             ثابت نویسه *rootfs, libcrun_error_t *err)
{
  صحیح ret, rootfsfd;
  ساختار krun_config *kconf = (ساختار krun_config *) cookie;
  ساختار device_s kvm_device = { "/dev/kvm", "c", 10, 232, 0666, 0, 0 };
  ساختار device_s sev_device = { "/dev/sev", "c", 10, 124, 0666, 0, 0 };
  ساختار device_s nitro_device = { "/dev/nitro_enclaves", "c", 10, 122, 0666, 0, 0 };
  cleanup_close صحیح devfd = -1;
  cleanup_close صحیح rootfsfd_cleanup = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  bool create_sev = false, create_awsnitro = false;
  bool is_user_ns;

  اگر (rootfs == پوچ)
    rootfsfd = AT_FDCWD;
  وگرنه
    {
      rootfsfd = rootfsfd_cleanup = open (rootfs, O_PATH | O_CLOEXEC);
      اگر (UNLIKELY (rootfsfd < 0))
        بازگشت crun_make_error (err, errno, "open `%s`", rootfs);
    }

  اگر (phase == HANDLER_CONFIGURE_BEFORE_USERNS)
    {
      cleanup_free نویسه *origin_config_path = پوچ;
      cleanup_free نویسه *state_dir = پوچ;
      cleanup_free نویسه *config = پوچ;
      cleanup_close صحیح fd = -1;
      size_t config_size;

      ret = libcrun_get_state_directory (&state_dir, context->state_ریشه, context->id, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = append_paths (&origin_config_path, err, state_dir, "config.json", پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = read_all_file (origin_config_path, &config, &config_size, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      /* CVE-2025-24965: the content below rootfs cannot be trusted because it is controlled by the user.  We
         must ensure the file is opened below the rootfs directory.  Whatever the image ships at that name
         could also be a device node or a FIFO, so never open it: unlink it and create a new regular file.  */
      ret = unlinkat (rootfsfd, KRUN_CONFIG_FILE, 0);
      اگر (UNLIKELY (ret < 0 && errno != ENOENT))
        بازگشت crun_make_error (err, errno, "unlink `%s`", KRUN_CONFIG_FILE);

      fd = safe_openat (rootfsfd, rootfs, KRUN_CONFIG_FILE, WRITE_FILE_DEFAULT_FLAGS | O_EXCL | O_NOFOLLOW, S_IRUSR | S_IRGRP | S_IROTH, err);
      اگر (UNLIKELY (fd < 0))
        بازگشت fd;

      ret = safe_write (fd, KRUN_CONFIG_FILE, config, config_size, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = libkrun_read_vm_config (kconf, rootfsfd, rootfs, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = libkrun_start_render_server (kconf, container, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    بازگشت 0;

  ret = libkrun_configure_network (cookie, container, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  /* Do nothing if /dev/kvm is already present in spec */
  اگر (spec_has_device (def, "/dev/kvm"))
    بازگشت 0;

  اگر (kconf->handle_sev != پوچ)
    create_sev = ! spec_has_device (def, "/dev/sev");

  اگر (kconf->handle_awsnitro != پوچ)
    create_awsnitro = ! spec_has_device (def, "/dev/nitro_enclaves");

  devfd = safe_openat (rootfsfd, rootfs, "dev", O_PATH | O_DIRECTORY | O_CLOEXEC, 0, err);
  اگر (UNLIKELY (devfd < 0))
    بازگشت devfd;

  ret = check_running_in_user_namespace (err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;
  is_user_ns = ret;

  اگر (kconf->has_kvm)
    {
      ret = libcrun_create_dev (container, devfd, -1, &kvm_device, is_user_ns, true, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (create_sev)
    {
      ret = libcrun_create_dev (container, devfd, -1, &sev_device, is_user_ns, true, err);
      اگر (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_sev);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
          kconf->handle_sev = پوچ;
        }
    }

  اگر (create_awsnitro)
    {
      ret = libcrun_create_dev (container, devfd, -1, &nitro_device, is_user_ns, true, err);
      اگر (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_awsnitro);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
          kconf->handle_awsnitro = پوچ;
        }
    }

  بازگشت 0;
}

ایستا صحیح
libkrun_load (تهی **cookie, libcrun_error_t *err)
{
  int32_t ret;
  ساختار krun_config *kconf;
  ثابت نویسه *libkrun_so = "libkrun.so.1";
  ثابت نویسه *libkrun_sev_so = "libkrun-sev.so.1";
  ثابت نویسه *libkrun_awsnitro_so = "libkrun-awsnitro.so.1";

  kconf = malloc (اندازه (ساختار krun_config));
  اگر (kconf == پوچ)
    بازگشت crun_make_error (err, 0, "could not allocate memory for krun_config");
  memset (kconf, 0, اندازه (ساختار krun_config));

  kconf->gpu_render_server_fd = -1;

  kconf->handle = dlopen (libkrun_so, RTLD_NOW);
  kconf->handle_sev = dlopen (libkrun_sev_so, RTLD_NOW);
  kconf->handle_awsnitro = dlopen (libkrun_awsnitro_so, RTLD_NOW);

  اگر (kconf->handle == پوچ && kconf->handle_sev == پوچ && kconf->handle_awsnitro == پوچ)
    {
      آزاد (kconf);
      بازگشت crun_make_error (err, 0, "failed to open `%s`, `%s`, and `%s` for krun_config: %s", libkrun_so, libkrun_sev_so, libkrun_awsnitro_so, dlerror ());
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
        برو error;
      kconf->ctx_id = ret;
    }

  اگر (kconf->handle_sev)
    {
      ret = libkrun_create_context (kconf->handle_sev, err);
      اگر (UNLIKELY (ret < 0))
        برو error;
      kconf->ctx_id_sev = ret;
    }
  اگر (kconf->handle_awsnitro)
    {
      ret = libkrun_create_context (kconf->handle_awsnitro, err);
      اگر (UNLIKELY (ret < 0))
        برو error;
      kconf->ctx_id_awsnitro = ret;
    }

  *cookie = kconf;

  بازگشت 0;

error:
  اگر (kconf->handle)
    dlclose (kconf->handle);
  اگر (kconf->handle_sev)
    dlclose (kconf->handle_sev);
  اگر (kconf->handle_awsnitro)
    dlclose (kconf->handle_awsnitro);
  آزاد (kconf);
  بازگشت ret;
}

ایستا صحیح
libkrun_unload (تهی *cookie, libcrun_error_t *err)
{
  صحیح r;

  ساختار krun_config *kconf = (ساختار krun_config *) cookie;
  اگر (kconf != پوچ)
    {
      اگر (kconf->handle != پوچ)
        {
          r = dlclose (kconf->handle);
          اگر (UNLIKELY (r != 0))
            بازگشت crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
        }
      اگر (kconf->handle_sev != پوچ)
        {
          r = dlclose (kconf->handle_sev);
          اگر (UNLIKELY (r != 0))
            بازگشت crun_make_error (err, 0, "could not unload handle_sev: `%s`", dlerror ());
        }
      اگر (kconf->handle_awsnitro != پوچ)
        {
          r = dlclose (kconf->handle_awsnitro);
          اگر (UNLIKELY (r != 0))
            بازگشت crun_make_error (err, 0, "could not unload handle_awsnitro: `%s`", dlerror ());
        }
      آزاد (kconf);
    }
  بازگشت 0;
}

ایستا runtime_spec_schema_defs_linux_device_cgroup *
make_oci_spec_dev (ثابت نویسه *type, dev_t device, bool allow, ثابت نویسه *access)
{
  runtime_spec_schema_defs_linux_device_cgroup *dev = xmalloc0 (اندازه (*dev));

  dev->allow = allow;
  dev->allow_present = 1;

  dev->type = xstrdup (type);

  dev->major = major (device);
  dev->major_present = 1;

  dev->minor = minor (device);
  dev->minor_present = 1;

  dev->access = xstrdup (access);

  بازگشت dev;
}

/* stat an optional device: on ENOENT clear *PRESENT, any other error fails.  */
ایستا صحیح
stat_optional_device (ثابت نویسه *path, ساختار stat *st, bool *present, libcrun_error_t *err)
{
  صحیح ret = stat (path, st);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (errno != ENOENT)
        بازگشت crun_make_error (err, errno, "stat `%s`", path);
      *present = false;
    }
  بازگشت 0;
}

ایستا صحیح
libkrun_modify_oci_configuration (تهی *cookie arg_unused, libcrun_context_t *context arg_unused,
                                  runtime_spec_schema_config_schema *def,
                                  libcrun_error_t *err)
{
  ثابت size_t device_size = اندازه (runtime_spec_schema_defs_linux_device_cgroup);
  ساختار krun_config *kconf = (ساختار krun_config *) cookie;
  ساختار stat st_kvm, st_sev, st_awsnitro;
  bool has_kvm = true, has_sev = true, has_awsnitro = true;
  size_t old_len, new_len;
  صحیح ret;

  /* Always allow the /dev/kvm device.  */

  ret = stat_optional_device ("/dev/kvm", &st_kvm, &has_kvm, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = stat_optional_device ("/dev/sev", &st_sev, &has_sev, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = stat_optional_device ("/dev/nitro_enclaves", &st_awsnitro, &has_awsnitro, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  kconf->has_kvm = has_kvm;
  kconf->has_awsnitro = has_awsnitro;

  اگر (! has_kvm && ! has_awsnitro)
    بازگشت 0;

  /* spec says these are optional, ensure they exist so we can add our devices */
  اگر (def->linux == پوچ)
    def->linux = xmalloc0 (اندازه (runtime_spec_schema_config_linux));

  اگر (def->linux->resources == پوچ)
    def->linux->resources = xmalloc0 (اندازه (runtime_spec_schema_config_linux_resources));

  old_len = def->linux->resources->devices_len;
  new_len = old_len;
  اگر (has_kvm)
    new_len += has_sev ? 2 : 1;
  اگر (has_awsnitro)
    new_len += 1;

  def->linux->resources->devices = xrealloc (def->linux->resources->devices, device_size * (new_len + 1));
  def->linux->resources->devices_len = new_len;

  اگر (has_kvm)
    {
      def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_kvm.st_rdev, true, "rwm");
      اگر (has_sev)
        def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_sev.st_rdev, true, "rwm");
    }

  اگر (has_awsnitro)
    def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_awsnitro.st_rdev, true, "rwm");

  بازگشت 0;
}

ایستا صحیح
libkrun_close_fds (تهی *cookie, libcrun_container_t *container, صحیح preserve_fds, libcrun_error_t *err)
{
  ساختار krun_config *kconf = (ساختار krun_config *) cookie;
  صحیح first_fd_to_close = preserve_fds + 3;
  صحیح کم_fd, بالا_fd;
  صحیح i;

  کم_fd = بالا_fd = -1;

  اگر (kconf->use_passt)
    بالا_fd = kconf->passt_fds[FD_PAIR_PARENT];

  اگر (kconf->gpu_render_server_fd != -1)
    {
      اگر (kconf->gpu_render_server_fd > بالا_fd)
        {
          کم_fd = بالا_fd;
          بالا_fd = kconf->gpu_render_server_fd;
        }
      وگرنه
        کم_fd = kconf->gpu_render_server_fd;
    }

  اگر (first_fd_to_close <= بالا_fd)
    {
      برای (i = first_fd_to_close; i < بالا_fd; i++)
        {
          // If we're closing proc_fd, make sure to invalidate it.
          اگر (i == container->proc_fd)
            container->proc_fd = -1;
          وگرنه اگر (i == کم_fd)
            ادامه;
          close (i);
        }

      first_fd_to_close = بالا_fd + 1;
    }

  بازگشت mark_or_close_fds_ge_than (container, first_fd_to_close, true, err);
}

ساختار custom_handler_s handler_libkrun = {
  .name = "krun",
  .alias = پوچ,
  .feature_string = "LIBKRUN",
  .supports_open_درخت_namespace = false,
  .load = libkrun_load,
  .unload = libkrun_unload,
  .run_func = libkrun_exec,
  .configure_container = libkrun_configure_container,
  .modify_oci_configuration = libkrun_modify_oci_configuration,
  .close_fds = libkrun_close_fds,
};

#endif
