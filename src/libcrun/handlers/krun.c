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
#घोषणा _GNU_SOURCE

#समावेश <config.h>
#समावेश "../custom-handler.h"
#समावेश "../container.h"
#समावेश "../utils.h"
#समावेश "../linux.h"
#समावेश <unistd.h>
#समावेश <sys/stat.h>
#समावेश <sys/socket.h>
#समावेश <errno.h>
#समावेश <sys/mount.h>
#समावेश <sys/param.h>
#समावेश <sys/types.h>
#समावेश <sys/sysmacros.h>
#समावेश <fcntl.h>
#समावेश <sched.h>
#समावेश <ocispec/runtime_spec_schema_config_schema.h>

#अगर_घोषित HAVE_DLOPEN
#  include <dlfcn.h>
#पूर्ण_अगर

#अगर_घोषित HAVE_LIBKRUN
#  include <libkrun.h>
#पूर्ण_अगर

/* This allows us to build even with headers from older libkrun versions.
 * If the installed version of libkrun doesn't support this feature,
 * krun_add_net_unixstream() will return EINVAL.
 */
#अगर_अघोषित NET_FLAG_DHCP_CLIENT
#  define NET_FLAG_DHCP_CLIENT (1 << 1)
#पूर्ण_अगर
/* libkrun has a hard-limit of 16 vCPUs per microVM. */
#घोषणा LIBKRUN_MAX_VCPUS 16

/* If the user doesn't configure the RAM amount, fallback to this value. */
#घोषणा LIBKRUN_DEFAULT_RAM_MIB 1024

/* The minimum amount of RAM for a viable microVM is 128 MB. */
#घोषणा LIBKRUN_MINIMUM_RAM_MIB 128

/* crun dumps the container configuration into this file, which will be read by
 * libkrun to set up the environment for the workload inside the microVM.
 */
#घोषणा KRUN_CONFIG_FILE ".krun_config.json"

/* The presence of this file indicates this is a container intended to be run
 * as a confidential workload inside a SEV-powered TEE.
 */
#घोषणा KRUN_SEV_FILE "/krun-sev.json"

/* This file contains configuration parameters for the microVM. crun needs to
 * read and parse it, using the information obtained from it to configure
 * libkrun as required.
 */
#घोषणा KRUN_VM_FILE "/.krun_vm.json"

/* KRUN_VM_FILE holds a handful of settings and it is parsed by crun
 * itself before the container process exists, refuse anything bigger.
 */
#घोषणा KRUN_VM_FILE_MAX_आकार (64 * 1024)

#घोषणा KRUN_FLAVOR_AWS_NITRO "aws-nitro"
#घोषणा KRUN_FLAVOR_SEV "sev"

#घोषणा FD_PAIR_PARENT 0
#घोषणा FD_PAIR_CHILD 1

काष्ठा krun_config
{
  व्योम *handle;
  व्योम *handle_sev;
  व्योम *handle_awsnitro;
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
  स्थिर अक्षर *tap_name;
  पूर्णांक gpu_render_server_fd;
  पूर्णांक gpu_flags;
};

/* libkrun handler.  */
#अगर HAVE_DLOPEN && HAVE_LIBKRUN
अटल int32_t
libkrun_create_context (व्योम *handle, libcrun_error_t *err)
{
  int32_t (*krun_create_ctx) ();
  int32_t ctx_id;

  krun_create_ctx = dlsym (handle, "krun_create_ctx");
  अगर (krun_create_ctx == शून्य)
    वापस crun_make_error (err, 0, "could not find symbol in the krun library");

  ctx_id = krun_create_ctx ();
  अगर (UNLIKELY (ctx_id < 0))
    वापस crun_make_error (err, -ctx_id, "could not create krun context");

  वापस ctx_id;
}

अटल पूर्णांक
libkrun_configure_kernel (uint32_t ctx_id, व्योम *handle, json_object *config_वृक्ष, libcrun_error_t *err)
{
  int32_t (*krun_set_kernel) (uint32_t ctx_id, स्थिर अक्षर *kernel_path,
                              uint32_t kernel_format, स्थिर अक्षर *initrd_path, स्थिर अक्षर *kernel_cmdline);
  json_object *kernel_path = शून्य;
  json_object *kernel_format = शून्य;
  json_object *val_initrd_path = शून्य;
  json_object *val_kernel_cmdline = शून्य;
  स्थिर अक्षर *initrd_path = शून्य;
  स्थिर अक्षर *kernel_cmdline = शून्य;
  पूर्णांक ret;

  /* kernel_path and kernel_format must be present */
  kernel_path = json_object_object_get (config_वृक्ष, "kernel_path");
  अगर (kernel_path == शून्य || ! json_object_is_type (kernel_path, json_type_string))
    वापस 0;

  kernel_format = json_object_object_get (config_वृक्ष, "kernel_format");
  अगर (kernel_format == शून्य || ! json_object_is_type (kernel_format, json_type_int))
    वापस 0;

  /* initrd and kernel_cmdline are optional */
  val_initrd_path = json_object_object_get (config_वृक्ष, "initrd_path");
  अगर (val_initrd_path != शून्य && json_object_is_type (val_initrd_path, json_type_string))
    initrd_path = json_object_get_string (val_initrd_path);

  val_kernel_cmdline = json_object_object_get (config_वृक्ष, "kernel_cmdline");
  अगर (val_kernel_cmdline != शून्य && json_object_is_type (val_kernel_cmdline, json_type_string))
    kernel_cmdline = json_object_get_string (val_kernel_cmdline);

  krun_set_kernel = dlsym (handle, "krun_set_kernel");
  अगर (krun_set_kernel == शून्य)
    वापस crun_make_error (err, 0, "could not find symbol in krun library");

  ret = krun_set_kernel (ctx_id,
                         json_object_get_string (kernel_path),
                         (uint32_t) json_object_get_int64 (kernel_format),
                         initrd_path, kernel_cmdline);

  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, -ret, "could not configure a krun external kernel");

  वापस 0;
}

अटल पूर्णांक
libkrun_read_vm_config (काष्ठा krun_config *kconf, पूर्णांक rootfsfd, स्थिर अक्षर *rootfs, libcrun_error_t *err)
{
  पूर्णांक ret;
  cleanup_free अक्षर *config = शून्य;
  cleanup_close पूर्णांक pathfd = -1;
  cleanup_close पूर्णांक fd = -1;
  proc_fd_path_t fd_path;
  काष्ठा stat st;

  /* The content of the rootfs is controlled by the user: look at the file through an
     O_PATH descriptor first and refuse anything that is not a small regular file, as
     opening a FIFO blocks and a device node must not be accessed.  */
  pathfd = safe_openat (rootfsfd, rootfs, KRUN_VM_FILE, O_PATH | O_CLOEXEC | O_NOFOLLOW, 0, err);
  अगर (pathfd < 0)
    {
      // The configuration file is optional, don't generate an error if it's missing.
      अगर (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          वापस 0;
        }
      वापस pathfd;
    }

  ret = ख_स्थिति (pathfd, &st);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "stat `%s`", KRUN_VM_FILE);

  अगर (! S_ISREG (st.st_mode))
    वापस crun_make_error (err, 0, "`%s` is not a regular file", KRUN_VM_FILE);

  get_proc_self_fd_path (fd_path, pathfd);

  fd = खोलो (fd_path, O_RDONLY | O_CLOEXEC);
  अगर (UNLIKELY (fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s`", KRUN_VM_FILE);

  ret = read_all_fd_with_max_आकार (fd, KRUN_VM_FILE, &config, शून्य, KRUN_VM_FILE_MAX_आकार, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = parse_json_file (&kconf->config_doc, config, शून्य, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  kconf->config_वृक्ष = kconf->config_doc;
  वापस 0;
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
अटल पूर्णांक
libkrun_parse_resource_configuration (json_object *config_वृक्ष, libcrun_container_t *container, स्थिर अक्षर *annotation, स्थिर अक्षर *कुंजी, bool use_config_file)
{
  अक्षर *val_str, *endptr;
  पूर्णांक val = -1;
  json_object *val_json = शून्य;

  val_str = (अक्षर *) find_annotation (container, annotation);
  अगर (val_str != शून्य)
    {
      त्रुटि_सं = 0;
      val = (पूर्णांक) म_से_दीर्घ (val_str, &endptr, 10);
      अगर (त्रुटि_सं != 0 || endptr == val_str || *endptr != '\0')
        /* Annotations value is not a valid integer. */
        error (निकास_त्रुटि, 0, "krun annotation %s value cannot be converted to an integer", annotation);

      अगर (val < 0)
        error (निकास_त्रुटि, 0, "krun annotation %s value must be a positive integer", annotation);

      वापस val;
    }
  अन्यथा अगर (use_config_file && config_वृक्ष != शून्य)
    {
      val_json = json_object_object_get (config_वृक्ष, कुंजी);
      अगर (val_json == शून्य)
        वापस val;
      अगर (! json_object_is_type (val_json, json_type_int))
        error (निकास_त्रुटि, 0, "krun krun_vm.json %s value is not an integer", कुंजी);

      val = (पूर्णांक) json_object_get_int64 (val_json);
    }

  वापस val;
}

अटल पूर्णांक
libkrun_parse_string_configuration (json_object *config_वृक्ष, libcrun_container_t *container,
                                    स्थिर अक्षर *annotation, स्थिर अक्षर *कुंजी,
                                    स्थिर अक्षर **मूल्य, libcrun_error_t *err, bool use_config_file)
{
  स्थिर अक्षर *val;
  json_object *val_json = शून्य;

  *मूल्य = शून्य;

  val = find_annotation (container, annotation);
  अगर (val != शून्य)
    {
      *मूल्य = val;
      वापस 0;
    }

  अगर (! use_config_file || config_वृक्ष == शून्य)
    वापस 0;

  val_json = json_object_object_get (config_वृक्ष, कुंजी);
  अगर (val_json == शून्य)
    वापस 0;

  अगर (! json_object_is_type (val_json, json_type_string))
    वापस crun_make_error (err, 0, ".krun_vm.json %s value is not a string", कुंजी);

  *मूल्य = json_object_get_string (val_json);
  वापस 0;
}

अटल व्योम
libkrun_make_tap_mac (स्थिर अक्षर *id, uint8_t mac[6])
{
  uint64_t hash = 14695981039346656037ULL;
  माप_प्रकार i;

  क्रम (; *id != '\0'; id++)
    {
      hash ^= (uint8_t) *id;
      hash *= 1099511628211ULL;
    }

  क्रम (i = 0; i < 6; i++)
    mac[i] = (uint8_t) (hash >> (i * 8));

  mac[0] = (mac[0] & 0xfe) | 0x02;
}

अटल पूर्णांक
libkrun_configure_vm (uint32_t ctx_id, व्योम *handle, काष्ठा krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_set_vm_config) (uint32_t ctx_id, uint8_t अंक_vcpus, uint32_t ram_mib);
  int32_t (*krun_add_net_unixstream) (uint32_t ctx_id, स्थिर अक्षर *c_path, पूर्णांक fd, uint8_t *स्थिर c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_add_net_tap) (uint32_t ctx_id, स्थिर अक्षर *c_tap_name, स्थिर uint8_t *c_mac, uint32_t features, uint32_t flags);
  int32_t (*krun_set_gpu_options) (uint32_t ctx_id, uint32_t virgl_flags);
  int32_t (*krun_set_gpu_render_server_fd) (uint32_t ctx_id, पूर्णांक fd);
  पूर्णांक cpus, ram_mib, nested_virt, ret;
  cpu_set_t set;

  /* We let the OCI set the number of vCPUs for the VM, since cgroups CPU restrictions still apply. */
  cpus = libkrun_parse_resource_configuration (kconf->config_वृक्ष, container, "krun.cpus", "cpus", true);
  अगर (cpus <= 0)
    {
      CPU_ZERO (&set);
      अगर (sched_getaffinity (getpid (), माप (set), &set) == 0)
        cpus = MIN (CPU_गिनती (&set), LIBKRUN_MAX_VCPUS);
      अन्यथा
        cpus = 1;
    }

  /* We let the OCI set the amount of RAM for the VM, since cgroups memory restrictions still apply. */
  ram_mib = libkrun_parse_resource_configuration (kconf->config_वृक्ष, container, "krun.ram_mib", "ram_mib", true);
  अगर (ram_mib <= LIBKRUN_MINIMUM_RAM_MIB)
    {
      अगर (def && def->linux && def->linux->resources && def->linux->resources->memory
          && def->linux->resources->memory->limit_present)
        ram_mib = def->linux->resources->memory->limit / (1024 * 1024);
      अन्यथा
        ram_mib = LIBKRUN_DEFAULT_RAM_MIB;
    }

  krun_set_vm_config = dlsym (handle, "krun_set_vm_config");

  अगर (krun_set_vm_config == शून्य)
    वापस crun_make_error (err, 0, "could not find symbol in the krun library");

  ret = krun_set_vm_config (ctx_id, cpus, ram_mib);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, -ret, "could not set krun vm configuration");

  nested_virt = libkrun_parse_resource_configuration (kconf->config_वृक्ष, container, "krun.nested_virt", "nested_virt", false);
  अगर (nested_virt > 0)
    {
      int32_t (*krun_check_nested_virt) (व्योम);
      int32_t (*krun_set_nested_virt) (uint32_t ctx_id, bool enabled);

      krun_check_nested_virt = dlsym (handle, "krun_check_nested_virt");
      अगर (krun_check_nested_virt != शून्य && krun_check_nested_virt () != 1)
        libcrun_warning ("nested virtualization requested but may not be supported on this host");

      krun_set_nested_virt = dlsym (handle, "krun_set_nested_virt");
      अगर (krun_set_nested_virt == शून्य)
        वापस crun_make_error (err, 0, "could not find symbol `krun_set_nested_virt` in the krun library");

      ret = krun_set_nested_virt (ctx_id, true);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, -ret, "could not enable nested virtualization");
    }

  अगर (kconf->tap_name != शून्य)
    {
      krun_add_net_tap = dlsym (handle, "krun_add_net_tap");
      अगर (krun_add_net_tap == शून्य)
        वापस crun_make_error (err, 0, "could not find symbol `krun_add_net_tap` in the krun library");

      uint8_t mac[6];
      libkrun_make_tap_mac (container->context->id, mac);
      ret = krun_add_net_tap (ctx_id, kconf->tap_name, &mac[0], COMPAT_NET_FEATURES, 0);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, -ret, "could not add krun TAP interface `%s`", kconf->tap_name);
    }
  अन्यथा अगर (kconf->use_passt)
    {
      krun_add_net_unixstream = dlsym (handle, "krun_add_net_unixstream");
      अगर (krun_add_net_unixstream == शून्य)
        वापस crun_make_error (err, 0, "could not find symbol `krun_add_net_unixstream` in the krun library");

      uint8_t mac[] = { 0x5a, 0x94, 0xef, 0xe4, 0x0c, 0xee };
      ret = krun_add_net_unixstream (ctx_id, शून्य, kconf->passt_fds[FD_PAIR_PARENT], &mac[0], COMPAT_NET_FEATURES, NET_FLAG_DHCP_CLIENT);
      अगर (UNLIKELY (ret == -EINVAL))
        ret = krun_add_net_unixstream (ctx_id, शून्य, kconf->passt_fds[FD_PAIR_PARENT], &mac[0], COMPAT_NET_FEATURES, 0);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, -ret, "could not set krun net configuration");
    }

  अगर (kconf->gpu_flags > 0)
    {
      krun_set_gpu_options = dlsym (kconf->handle, "krun_set_gpu_options");
      अगर (krun_set_gpu_options == शून्य)
        वापस crun_make_error (err, 0, "gpu requested but the version of libkrun in this system does not support it");

      ret = krun_set_gpu_options (kconf->ctx_id, kconf->gpu_flags);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, -ret, "gpu requested but could not configure virtio gpu device");

      अगर (kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER)
        {
          krun_set_gpu_render_server_fd = dlsym (kconf->handle, "krun_set_gpu_render_server_fd");
          अगर (krun_set_gpu_render_server_fd == शून्य)
            वापस crun_make_error (err, 0, "gpu with render server requested but the version of libkrun in this system does not support it");

          ret = krun_set_gpu_render_server_fd (kconf->ctx_id, kconf->gpu_render_server_fd);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, -ret, "gpu with render server requested but could not configure its file descriptor");
        }
    }

  अगर (kconf->config_वृक्ष != शून्य)
    {
      /* Try to configure an external kernel. If the configuration file doesn't
       * specify a kernel, libkrun automatically fall back to using libkrunfw,
       * if the library is present and was loaded while creating the context.
       */
      पूर्णांक custom_kernel = libkrun_parse_resource_configuration (kconf->config_वृक्ष, container, "krun.custom_kernel", "custom_kernel", false);
      अगर (custom_kernel > 0)
        {
          ret = libkrun_configure_kernel (ctx_id, handle, kconf->config_वृक्ष, err);
          अगर (UNLIKELY (ret))
            वापस ret;
        }
    }

  वापस 0;
}

अटल पूर्णांक
libkrun_configure_flavor (व्योम *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णांक ret, sev_indicated = 0, awsnitro_indicated = 0;
  काष्ठा krun_config *kconf = (काष्ठा krun_config *) cookie;
  स्थिर अक्षर *flavor = शून्य;
  व्योम *close_handles[2];

  close_handles[0] = शून्य;
  close_handles[1] = शून्य;

  // Check if the user provided the krun variant through OCI annotations.
  flavor = find_annotation (container, "krun.variant");
  अगर (flavor != शून्य)
    {
      sev_indicated |= म_भेद (flavor, KRUN_FLAVOR_SEV) == 0;
      awsnitro_indicated |= म_भेद (flavor, KRUN_FLAVOR_AWS_NITRO) == 0;
    }

  अगर (sev_indicated)
    {
      अगर (kconf->handle_sev == शून्य)
        error (निकास_त्रुटि, 0, "the container requires libkrun-sev but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_awsnitro;

      kconf->handle = kconf->handle_sev;
      kconf->ctx_id = kconf->ctx_id_sev;
      kconf->sev = true;
    }
  अन्यथा अगर (awsnitro_indicated)
    {
      अगर (kconf->handle_awsnitro == शून्य)
        error (निकास_त्रुटि, 0, "the container requires libkrun-awsnitro but it's not available");

      close_handles[0] = kconf->handle;
      close_handles[1] = kconf->handle_sev;

      kconf->handle = kconf->handle_awsnitro;
      kconf->ctx_id = kconf->ctx_id_awsnitro;
      kconf->awsnitro = true;
    }
  अन्यथा
    {
      अगर (kconf->handle == शून्य)
        error (निकास_त्रुटि, 0, "the container requires libkrun but it's not available");

      close_handles[0] = kconf->handle_sev;
      close_handles[1] = kconf->handle_awsnitro;
    }

  // We no longer need the other two libkrun handles.
  क्रम (पूर्णांक i = 0; i < 2; i++)
    {
      अगर (close_handles[i] == शून्य)
        जारी;

      ret = dlclose (close_handles[i]);
      अगर (UNLIKELY (ret != 0))
        वापस crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
    }

  वापस 0;
}

अटल पूर्णांक
libkrun_exec (व्योम *cookie, libcrun_container_t *container, स्थिर अक्षर *pathname, अक्षर *स्थिर argv[])
{
  runtime_spec_schema_config_schema *def = container->container_def;
  int32_t (*krun_set_log_level) (uint32_t level);
  पूर्णांक (*krun_start_enter) (uint32_t ctx_id);
  int32_t (*krun_add_virtiofs2) (uint32_t ctx_id, स्थिर अक्षर *c_tag, स्थिर अक्षर *c_path, uint64_t shm_आकार);
  int32_t (*krun_set_मूल_disk) (uint32_t ctx_id, स्थिर अक्षर *disk_path);
  int32_t (*krun_set_tee_config_file) (uint32_t ctx_id, स्थिर अक्षर *file_path);
  int32_t (*krun_set_console_output) (uint32_t ctx_id, स्थिर अक्षर *c_filepath);
  int32_t (*krun_set_exec) (uint32_t ctx_id, स्थिर अक्षर *exec_path,
                            स्थिर अक्षर *स्थिर argv[], स्थिर अक्षर *स्थिर envp[]);
  काष्ठा krun_config *kconf = (काष्ठा krun_config *) cookie;
  व्योम *handle;
  int32_t ctx_id, ret;
  libcrun_error_t err;

  ret = libkrun_configure_flavor (cookie, container, &err);
  अगर (UNLIKELY (ret < 0))
    {
      पूर्णांक errcode = crun_error_get_errno (&err);
      crun_error_release (&err);
      error (निकास_त्रुटि, errcode, "unable to configure libkrun flavor");
    }

  // /dev/kvm is required for all non AWS nitro workloads.
  अगर (! kconf->awsnitro && ! kconf->has_kvm)
    error (निकास_त्रुटि, 0, "`/dev/kvm` unavailable");

  handle = kconf->handle;
  ctx_id = kconf->ctx_id;

  krun_set_log_level = dlsym (handle, "krun_set_log_level");
  krun_start_enter = dlsym (handle, "krun_start_enter");
  अगर (krun_set_log_level == शून्य || krun_start_enter == शून्य)
    error (निकास_त्रुटि, 0, "could not find symbol in the krun library");

  /* Set log level according to crun's verbosity. */
  चयन (libcrun_get_verbosity ())
    {
    हाल LIBCRUN_VERBOSITY_DEBUG:
      krun_set_log_level (KRUN_LOG_LEVEL_DEBUG);
      अवरोध;
    हाल LIBCRUN_VERBOSITY_WARNING:
      krun_set_log_level (KRUN_LOG_LEVEL_WARN);
      अवरोध;
    शेष:
      krun_set_log_level (KRUN_LOG_LEVEL_ERROR);
      अवरोध;
    }

  अगर (kconf->sev)
    {
      krun_set_मूल_disk = dlsym (handle, "krun_set_root_disk");
      krun_set_tee_config_file = dlsym (handle, "krun_set_tee_config_file");
      अगर (krun_set_मूल_disk == शून्य || krun_set_tee_config_file == शून्य)
        error (निकास_त्रुटि, 0, "could not find symbol in `libkrun-sev.so`");

      ret = krun_set_मूल_disk (ctx_id, "/disk.img");
      अगर (UNLIKELY (ret < 0))
        error (निकास_त्रुटि, -ret, "could not set root disk");

      ret = krun_set_tee_config_file (ctx_id, KRUN_SEV_FILE);
      अगर (UNLIKELY (ret < 0))
        error (निकास_त्रुटि, -ret, "could not set krun tee config file");
    }
  अन्यथा
    {
      json_object *val_virtiofs_tag = शून्य;
      json_object *val_virtiofs_shm_आकार = शून्य;
      स्थिर अक्षर *virtiofs_tag = शून्य;
      // Default to a conservative DAX size of 512MB, just like krun_set_root() does.
      uint64_t virtiofs_shm_आकार = 512 * 1024 * 1024ULL;

      अगर (kconf->config_वृक्ष != शून्य)
        {
          val_virtiofs_tag = json_object_object_get (kconf->config_वृक्ष, "virtiofs_tag");
          अगर (val_virtiofs_tag != शून्य && json_object_is_type (val_virtiofs_tag, json_type_string))
            virtiofs_tag = json_object_get_string (val_virtiofs_tag);

          val_virtiofs_shm_आकार = json_object_object_get (kconf->config_वृक्ष, "virtiofs_shm_size");
          अगर (val_virtiofs_shm_आकार != शून्य && json_object_is_type (val_virtiofs_shm_आकार, json_type_int))
            virtiofs_shm_आकार = json_object_get_uint64 (val_virtiofs_shm_आकार);
        }

      अगर (virtiofs_tag == शून्य)
        virtiofs_tag = "/dev/root";

      krun_add_virtiofs2 = dlsym (handle, "krun_add_virtiofs2");

      अगर (krun_add_virtiofs2 == शून्य)
        error (निकास_त्रुटि, 0, "could not find symbol `krun_add_virtiofs2` in `libkrun.so`");

      ret = krun_add_virtiofs2 (ctx_id, virtiofs_tag, "/", virtiofs_shm_आकार);
      अगर (UNLIKELY (ret < 0))
        error (निकास_त्रुटि, -ret, "could not add virtiofs root with tag `%s`", virtiofs_tag);
    }

  अगर (kconf->awsnitro)
    {
      krun_set_console_output = dlsym (handle, "krun_set_console_output");
      krun_set_exec = dlsym (handle, "krun_set_exec");
      अगर (krun_set_console_output == शून्य || krun_set_exec == शून्य)
        error (निकास_त्रुटि, 0, "could not find symbol in `libkrun-awsnitro.so`");

      // Redirect all enclave output (read from vsock) to stdout.
      ret = krun_set_console_output (ctx_id, "/dev/stdout");
      अगर (UNLIKELY (ret < 0))
        error (निकास_त्रुटि, -ret, "could not redirect enclave output to stdout");

      ret = krun_set_exec (ctx_id, pathname, (स्थिर अक्षर *स्थिर *) argv,
                           (स्थिर अक्षर *स्थिर *) def->process->env);
      अगर (UNLIKELY (ret < 0))
        error (निकास_त्रुटि, -ret, "could not set enclave execution arguments");
    }

  ret = libkrun_configure_vm (ctx_id, handle, kconf, container, &err);
  अगर (UNLIKELY (ret))
    {
      पूर्णांक errcode = crun_error_get_errno (&err);
      libcrun_error_t *tmp_err = &err;
      libcrun_error_report_and_release (tmp_err);
      error (निकास_त्रुटि, errcode, "could not configure krun vm");
    }

  json_object_put (kconf->config_doc);

  ret = krun_start_enter (ctx_id);
  अगर (UNLIKELY (ret < 0))
    error (निकास_त्रुटि, -ret, "could not start krun");

  वापस ret;
}

अटल पूर्णांक
libkrun_start_render_server (काष्ठा krun_config *kconf, libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णांक ret;

  kconf->gpu_flags = libkrun_parse_resource_configuration (kconf->config_वृक्ष, container, "krun.gpu_flags", "gpu_flags", false);
  अगर (kconf->gpu_flags > 0)
    {
      अगर (access ("/dev/dri", F_OK) != 0)
        वापस crun_make_error (err, त्रुटि_सं, "gpu requested but /dev/dri is not available");

      अगर ((kconf->gpu_flags & VIRGLRENDERER_RENDER_SERVER) != 0)
        {
          अक्षर *rs_argv[40];
          पूर्णांक rs_argc = 0;
          अक्षर fd_as_str[16];
          पूर्णांक gpu_fds[2];
          pid_t pid;
          पूर्णांक null;

          अगर (access ("/usr/libexec/virgl_render_server", X_OK) != 0)
            वापस crun_make_error (err, त्रुटि_सं, "gpu with render server requested but couldn't find /usr/libexec/virgl_render_server binary");

          अगर (access ("/usr/bin/bwrap", X_OK) != 0)
            वापस crun_make_error (err, त्रुटि_सं, "gpu with render server requested but couldn't find /usr/bin/bwrap binary");

          ret = socketpair (AF_UNIX, SOCK_SEQPACKET, 0, gpu_fds);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "could not create socketpair for virgl_render_server");

          snprintf (fd_as_str, माप (fd_as_str), "%d", gpu_fds[FD_PAIR_CHILD]);

          rs_argv[rs_argc++] = (अक्षर *) "bwrap";
          rs_argv[rs_argc++] = (अक्षर *) "--ro-bind";
          rs_argv[rs_argc++] = (अक्षर *) "/usr";
          rs_argv[rs_argc++] = (अक्षर *) "/usr";
          अगर (access ("/lib", F_OK) == 0)
            {
              rs_argv[rs_argc++] = (अक्षर *) "--ro-bind";
              rs_argv[rs_argc++] = (अक्षर *) "/lib";
              rs_argv[rs_argc++] = (अक्षर *) "/lib";
            }
          अगर (access ("/lib64", F_OK) == 0)
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
          rs_argv[rs_argc] = शून्य;

          pid = विभाजन ();
          अगर (pid < 0)
            {
              बंद (gpu_fds[FD_PAIR_PARENT]);
              बंद (gpu_fds[FD_PAIR_CHILD]);
              वापस crun_make_error (err, त्रुटि_सं, "could not fork for virgl_render_server");
            }
          अन्यथा अगर (pid == 0)
            {
              बंद (gpu_fds[FD_PAIR_PARENT]);

              null = खोलो ("/dev/null", O_WRONLY);
              अगर (null == -1)
                _निकास (निकास_त्रुटि);

              dup2 (null, STDOUT_FILENO);
              dup2 (null, STDERR_FILENO);
              बंद (null);

              execvp ("/usr/bin/bwrap", rs_argv);
              _निकास (निकास_त्रुटि);
            }

          बंद (gpu_fds[FD_PAIR_CHILD]);
          kconf->gpu_render_server_fd = gpu_fds[FD_PAIR_PARENT];
        }
    }

  वापस 0;
}

अटल पूर्णांक
libkrun_configure_network (व्योम *cookie, libcrun_container_t *container, libcrun_error_t *err)
{
  काष्ठा krun_config *kconf = (काष्ठा krun_config *) cookie;
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
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (kconf->tap_name != शून्य && is_empty_string (kconf->tap_name))
    वापस crun_make_error (err, 0, "krun.tap_name cannot be empty");

  use_passt = libkrun_parse_resource_configuration (kconf->config_वृक्ष, container, "krun.use_passt", "use_passt", false);

  अगर (use_passt > 0)
    {
      अगर (kconf->tap_name != शून्य)
        वापस crun_make_error (err, 0, "krun.tap_name and krun.use_passt are mutually exclusive");

      kconf->use_passt = 1;
    }
  अन्यथा
    वापस 0;

  ret = socketpair (AF_UNIX, SOCK_STREAM, 0, kconf->passt_fds);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "create passt socketpair");

  snprintf (fd_as_str, माप (fd_as_str), "%d", kconf->passt_fds[FD_PAIR_CHILD]);

  argv_idx = 0;
  passt_argv[argv_idx++] = (अक्षर *) "passt";
  passt_argv[argv_idx++] = (अक्षर *) "-t";
  passt_argv[argv_idx++] = (अक्षर *) "all";

  अगर (! kconf->has_awsnitro)
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
  passt_argv[argv_idx] = शून्य;

  pid = विभाजन ();
  अगर (pid < 0)
    वापस crun_make_error (err, त्रुटि_सं, "fork passt");
  अन्यथा अगर (pid == 0)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      माप_प्रकार i;

      बंद (kconf->passt_fds[FD_PAIR_PARENT]);

      null = खोलो ("/dev/null", O_WRONLY);
      अगर (null == -1)
        _निकास (निकास_त्रुटि);

      /* Remount /proc if the parent detached its PID namespace, as passt(1)
       * needs a consistent /proc view to operate properly.
       */
      क्रम (i = 0; i < def->linux->namespaces_len; i++)
        {
          पूर्णांक मूल्य = libcrun_find_namespace (def->linux->namespaces[i]->type);

          अगर (मूल्य == CLONE_NEWPID)
            {
              अगर (mount ("", "/proc", "proc", MS_NOSUID | MS_NODEV | MS_NOEXEC, शून्य))
                _निकास (निकास_त्रुटि);

              अवरोध;
            }
        }

      // Redirect passt's stdout and stderr to /dev/null, as closing them here
      // instead will cause passt to exit with an error.
      dup2 (null, STDOUT_FILENO);
      dup2 (null, STDERR_FILENO);
      बंद (null);

      execvp ("/usr/bin/passt", passt_argv);
      // Only reachable on error.
      _निकास (निकास_त्रुटि);
    }

  बंद (kconf->passt_fds[FD_PAIR_CHILD]);

  // Wait for passt to daemonize itself.
  waitpid (pid, &status, 0);
  अगर (! (WIFEXITED (status)) || WEXITSTATUS (status) != 0)
    वापस crun_make_error (err, 0, "start passt");

  वापस 0;
}

/* Return true if the spec already declares a device with the given PATH.  */
अटल bool
spec_has_device (runtime_spec_schema_config_schema *def, स्थिर अक्षर *path)
{
  माप_प्रकार i;

  क्रम (i = 0; i < def->linux->devices_len; i++)
    अगर (म_भेद (def->linux->devices[i]->path, path) == 0)
      वापस true;
  वापस false;
}

/* libkrun_create_kvm_device: explicitly adds kvm device.  */
अटल पूर्णांक
libkrun_configure_container (व्योम *cookie, क्रमागत handler_configure_phase phase,
                             libcrun_context_t *context, libcrun_container_t *container,
                             स्थिर अक्षर *rootfs, libcrun_error_t *err)
{
  पूर्णांक ret, rootfsfd;
  काष्ठा krun_config *kconf = (काष्ठा krun_config *) cookie;
  काष्ठा device_s kvm_device = { "/dev/kvm", "c", 10, 232, 0666, 0, 0 };
  काष्ठा device_s sev_device = { "/dev/sev", "c", 10, 124, 0666, 0, 0 };
  काष्ठा device_s nitro_device = { "/dev/nitro_enclaves", "c", 10, 122, 0666, 0, 0 };
  cleanup_close पूर्णांक devfd = -1;
  cleanup_close पूर्णांक rootfsfd_cleanup = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  bool create_sev = false, create_awsnitro = false;
  bool is_user_ns;

  अगर (rootfs == शून्य)
    rootfsfd = AT_FDCWD;
  अन्यथा
    {
      rootfsfd = rootfsfd_cleanup = खोलो (rootfs, O_PATH | O_CLOEXEC);
      अगर (UNLIKELY (rootfsfd < 0))
        वापस crun_make_error (err, त्रुटि_सं, "open `%s`", rootfs);
    }

  अगर (phase == HANDLER_CONFIGURE_BEFORE_USERNS)
    {
      cleanup_free अक्षर *origin_config_path = शून्य;
      cleanup_free अक्षर *state_dir = शून्य;
      cleanup_free अक्षर *config = शून्य;
      cleanup_close पूर्णांक fd = -1;
      माप_प्रकार config_आकार;

      ret = libcrun_get_state_directory (&state_dir, context->state_मूल, context->id, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = append_paths (&origin_config_path, err, state_dir, "config.json", शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = read_all_file (origin_config_path, &config, &config_आकार, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      /* CVE-2025-24965: the content below rootfs cannot be trusted because it is controlled by the user.  We
         must ensure the file is opened below the rootfs directory.  Whatever the image ships at that name
         could also be a device node or a FIFO, so never open it: unlink it and create a new regular file.  */
      ret = unlinkat (rootfsfd, KRUN_CONFIG_FILE, 0);
      अगर (UNLIKELY (ret < 0 && त्रुटि_सं != ENOENT))
        वापस crun_make_error (err, त्रुटि_सं, "unlink `%s`", KRUN_CONFIG_FILE);

      fd = safe_openat (rootfsfd, rootfs, KRUN_CONFIG_FILE, WRITE_FILE_DEFAULT_FLAGS | O_EXCL | O_NOFOLLOW, S_IRUSR | S_IRGRP | S_IROTH, err);
      अगर (UNLIKELY (fd < 0))
        वापस fd;

      ret = safe_write (fd, KRUN_CONFIG_FILE, config, config_आकार, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = libkrun_read_vm_config (kconf, rootfsfd, rootfs, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = libkrun_start_render_server (kconf, container, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (phase != HANDLER_CONFIGURE_AFTER_MOUNTS)
    वापस 0;

  ret = libkrun_configure_network (cookie, container, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  /* Do nothing if /dev/kvm is already present in spec */
  अगर (spec_has_device (def, "/dev/kvm"))
    वापस 0;

  अगर (kconf->handle_sev != शून्य)
    create_sev = ! spec_has_device (def, "/dev/sev");

  अगर (kconf->handle_awsnitro != शून्य)
    create_awsnitro = ! spec_has_device (def, "/dev/nitro_enclaves");

  devfd = safe_openat (rootfsfd, rootfs, "dev", O_PATH | O_DIRECTORY | O_CLOEXEC, 0, err);
  अगर (UNLIKELY (devfd < 0))
    वापस devfd;

  ret = check_running_in_user_namespace (err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;
  is_user_ns = ret;

  अगर (kconf->has_kvm)
    {
      ret = libcrun_create_dev (container, devfd, -1, &kvm_device, is_user_ns, true, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (create_sev)
    {
      ret = libcrun_create_dev (container, devfd, -1, &sev_device, is_user_ns, true, err);
      अगर (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_sev);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
          kconf->handle_sev = शून्य;
        }
    }

  अगर (create_awsnitro)
    {
      ret = libcrun_create_dev (container, devfd, -1, &nitro_device, is_user_ns, true, err);
      अगर (UNLIKELY (ret < 0))
        {
          ret = dlclose (kconf->handle_awsnitro);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
          kconf->handle_awsnitro = शून्य;
        }
    }

  वापस 0;
}

अटल पूर्णांक
libkrun_load (व्योम **cookie, libcrun_error_t *err)
{
  int32_t ret;
  काष्ठा krun_config *kconf;
  स्थिर अक्षर *libkrun_so = "libkrun.so.1";
  स्थिर अक्षर *libkrun_sev_so = "libkrun-sev.so.1";
  स्थिर अक्षर *libkrun_awsnitro_so = "libkrun-awsnitro.so.1";

  kconf = दो_स्मृति (माप (काष्ठा krun_config));
  अगर (kconf == शून्य)
    वापस crun_make_error (err, 0, "could not allocate memory for krun_config");
  स_रखो (kconf, 0, माप (काष्ठा krun_config));

  kconf->gpu_render_server_fd = -1;

  kconf->handle = dlopen (libkrun_so, RTLD_NOW);
  kconf->handle_sev = dlopen (libkrun_sev_so, RTLD_NOW);
  kconf->handle_awsnitro = dlopen (libkrun_awsnitro_so, RTLD_NOW);

  अगर (kconf->handle == शून्य && kconf->handle_sev == शून्य && kconf->handle_awsnitro == शून्य)
    {
      मुक्त (kconf);
      वापस crun_make_error (err, 0, "failed to open `%s`, `%s`, and `%s` for krun_config: %s", libkrun_so, libkrun_sev_so, libkrun_awsnitro_so, dlerror ());
    }

  kconf->sev = false;
  kconf->awsnitro = false;

  /* Newer versions of libkrun no longer link against libkrunfw and
     instead they open it when creating the context. This implies
     we need to call "krun_create_ctx" before switching namespaces
     or it won't be able to find the library bundling the kernel. */
  अगर (kconf->handle)
    {
      ret = libkrun_create_context (kconf->handle, err);
      अगर (UNLIKELY (ret < 0))
        जाओ error;
      kconf->ctx_id = ret;
    }

  अगर (kconf->handle_sev)
    {
      ret = libkrun_create_context (kconf->handle_sev, err);
      अगर (UNLIKELY (ret < 0))
        जाओ error;
      kconf->ctx_id_sev = ret;
    }
  अगर (kconf->handle_awsnitro)
    {
      ret = libkrun_create_context (kconf->handle_awsnitro, err);
      अगर (UNLIKELY (ret < 0))
        जाओ error;
      kconf->ctx_id_awsnitro = ret;
    }

  *cookie = kconf;

  वापस 0;

error:
  अगर (kconf->handle)
    dlclose (kconf->handle);
  अगर (kconf->handle_sev)
    dlclose (kconf->handle_sev);
  अगर (kconf->handle_awsnitro)
    dlclose (kconf->handle_awsnitro);
  मुक्त (kconf);
  वापस ret;
}

अटल पूर्णांक
libkrun_unload (व्योम *cookie, libcrun_error_t *err)
{
  पूर्णांक r;

  काष्ठा krun_config *kconf = (काष्ठा krun_config *) cookie;
  अगर (kconf != शून्य)
    {
      अगर (kconf->handle != शून्य)
        {
          r = dlclose (kconf->handle);
          अगर (UNLIKELY (r != 0))
            वापस crun_make_error (err, 0, "could not unload handle: `%s`", dlerror ());
        }
      अगर (kconf->handle_sev != शून्य)
        {
          r = dlclose (kconf->handle_sev);
          अगर (UNLIKELY (r != 0))
            वापस crun_make_error (err, 0, "could not unload handle_sev: `%s`", dlerror ());
        }
      अगर (kconf->handle_awsnitro != शून्य)
        {
          r = dlclose (kconf->handle_awsnitro);
          अगर (UNLIKELY (r != 0))
            वापस crun_make_error (err, 0, "could not unload handle_awsnitro: `%s`", dlerror ());
        }
      मुक्त (kconf);
    }
  वापस 0;
}

अटल runtime_spec_schema_defs_linux_device_cgroup *
make_oci_spec_dev (स्थिर अक्षर *type, dev_t device, bool allow, स्थिर अक्षर *access)
{
  runtime_spec_schema_defs_linux_device_cgroup *dev = xmalloc0 (माप (*dev));

  dev->allow = allow;
  dev->allow_present = 1;

  dev->type = xstrdup (type);

  dev->major = major (device);
  dev->major_present = 1;

  dev->minor = minor (device);
  dev->minor_present = 1;

  dev->access = xstrdup (access);

  वापस dev;
}

/* stat an optional device: on ENOENT clear *PRESENT, any other error fails.  */
अटल पूर्णांक
stat_optional_device (स्थिर अक्षर *path, काष्ठा stat *st, bool *present, libcrun_error_t *err)
{
  पूर्णांक ret = stat (path, st);
  अगर (UNLIKELY (ret < 0))
    {
      अगर (त्रुटि_सं != ENOENT)
        वापस crun_make_error (err, त्रुटि_सं, "stat `%s`", path);
      *present = false;
    }
  वापस 0;
}

अटल पूर्णांक
libkrun_modify_oci_configuration (व्योम *cookie arg_unused, libcrun_context_t *context arg_unused,
                                  runtime_spec_schema_config_schema *def,
                                  libcrun_error_t *err)
{
  स्थिर माप_प्रकार device_आकार = माप (runtime_spec_schema_defs_linux_device_cgroup);
  काष्ठा krun_config *kconf = (काष्ठा krun_config *) cookie;
  काष्ठा stat st_kvm, st_sev, st_awsnitro;
  bool has_kvm = true, has_sev = true, has_awsnitro = true;
  माप_प्रकार old_len, new_len;
  पूर्णांक ret;

  /* Always allow the /dev/kvm device.  */

  ret = stat_optional_device ("/dev/kvm", &st_kvm, &has_kvm, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = stat_optional_device ("/dev/sev", &st_sev, &has_sev, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = stat_optional_device ("/dev/nitro_enclaves", &st_awsnitro, &has_awsnitro, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  kconf->has_kvm = has_kvm;
  kconf->has_awsnitro = has_awsnitro;

  अगर (! has_kvm && ! has_awsnitro)
    वापस 0;

  /* spec says these are optional, ensure they exist so we can add our devices */
  अगर (def->linux == शून्य)
    def->linux = xmalloc0 (माप (runtime_spec_schema_config_linux));

  अगर (def->linux->resources == शून्य)
    def->linux->resources = xmalloc0 (माप (runtime_spec_schema_config_linux_resources));

  old_len = def->linux->resources->devices_len;
  new_len = old_len;
  अगर (has_kvm)
    new_len += has_sev ? 2 : 1;
  अगर (has_awsnitro)
    new_len += 1;

  def->linux->resources->devices = xrealloc (def->linux->resources->devices, device_आकार * (new_len + 1));
  def->linux->resources->devices_len = new_len;

  अगर (has_kvm)
    {
      def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_kvm.st_rdev, true, "rwm");
      अगर (has_sev)
        def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_sev.st_rdev, true, "rwm");
    }

  अगर (has_awsnitro)
    def->linux->resources->devices[old_len++] = make_oci_spec_dev ("a", st_awsnitro.st_rdev, true, "rwm");

  वापस 0;
}

अटल पूर्णांक
libkrun_close_fds (व्योम *cookie, libcrun_container_t *container, पूर्णांक preserve_fds, libcrun_error_t *err)
{
  काष्ठा krun_config *kconf = (काष्ठा krun_config *) cookie;
  पूर्णांक first_fd_to_close = preserve_fds + 3;
  पूर्णांक निचला_fd, ऊपरी_fd;
  पूर्णांक i;

  निचला_fd = ऊपरी_fd = -1;

  अगर (kconf->use_passt)
    ऊपरी_fd = kconf->passt_fds[FD_PAIR_PARENT];

  अगर (kconf->gpu_render_server_fd != -1)
    {
      अगर (kconf->gpu_render_server_fd > ऊपरी_fd)
        {
          निचला_fd = ऊपरी_fd;
          ऊपरी_fd = kconf->gpu_render_server_fd;
        }
      अन्यथा
        निचला_fd = kconf->gpu_render_server_fd;
    }

  अगर (first_fd_to_close <= ऊपरी_fd)
    {
      क्रम (i = first_fd_to_close; i < ऊपरी_fd; i++)
        {
          // If we're closing proc_fd, make sure to invalidate it.
          अगर (i == container->proc_fd)
            container->proc_fd = -1;
          अन्यथा अगर (i == निचला_fd)
            जारी;
          बंद (i);
        }

      first_fd_to_close = ऊपरी_fd + 1;
    }

  वापस mark_or_close_fds_ge_than (container, first_fd_to_close, true, err);
}

काष्ठा custom_handler_s handler_libkrun = {
  .name = "krun",
  .alias = शून्य,
  .feature_string = "LIBKRUN",
  .supports_open_वृक्ष_namespace = false,
  .load = libkrun_load,
  .unload = libkrun_unload,
  .run_func = libkrun_exec,
  .configure_container = libkrun_configure_container,
  .modify_oci_configuration = libkrun_modify_oci_configuration,
  .close_fds = libkrun_close_fds,
};

#पूर्ण_अगर
