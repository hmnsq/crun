/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019, 2020 Giuseppe Scrivano <giuseppe@scrivano.org>
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
#अन्तर्भाव <ocispec/runtime_spec_schema_config_schema.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव "container.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "json_gen_utils.h"
#अन्तर्भाव "seccomp.h"
#अन्तर्भाव "mempolicy.h"
#ifdef HAVE_SECCOMP
#  include <seccomp.h>
#endif
#अन्तर्भाव "scheduler.h"
#अन्तर्भाव "seccomp_notify.h"
#अन्तर्भाव "custom-handler.h"
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <argp.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sched.h>
#अन्तर्भाव <sys/wait.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव "status.h"
#अन्तर्भाव "mount_flags.h"
#अन्तर्भाव "linux.h"
#अन्तर्भाव "terminal.h"
#अन्तर्भाव "io_priority.h"
#अन्तर्भाव "cgroup.h"
#अन्तर्भाव "cgroup-utils.h"
#अन्तर्भाव <sys/prctl.h>
#अन्तर्भाव <sys/auxv.h>
#अन्तर्भाव <sys/signalfd.h>
#अन्तर्भाव <sys/epoll.h>
#अन्तर्भाव <sys/socket.h>
#ifdef HAVE_CAP
#  include <sys/capability.h>
#endif
#अन्तर्भाव <sys/ioctl.h>
#अन्तर्भाव <termios.h>
#अन्तर्भाव <grp.h>
#अन्तर्भाव <git-version.h>

#ifdef HAVE_SYSTEMD
#  include <systemd/sd-daemon.h>
#endif

#अन्तर्भाव <json-c/json.h>
#अन्तर्भाव <ocispec/json_common.h>

गणना
{
  SYNC_SOCKET_SYNC_MESSAGE,
  SYNC_SOCKET_ERROR_MESSAGE,
  SYNC_SOCKET_WARNING_MESSAGE,
  SYNC_SOCKET_DEBUG_MESSAGE,
};

संरचना container_entrypoint_s
{
  libcrun_container_t *container;
  libcrun_context_t *context;
  पूर्णांक has_terminal_socket_pair;
  पूर्णांक terminal_socketpair[2];

  /* Used by log_write_to_sync_socket.  */
  पूर्णांक sync_socket;

  पूर्णांक seccomp_fd;
  पूर्णांक seccomp_receiver_fd;
  पूर्णांक console_socket_fd;

  पूर्णांक hooks_out_fd;
  पूर्णांक hooks_err_fd;

  /* Absolute bundle path, to be reported to hooks run in the container.  */
  स्थिरांक अक्षर *bundle;

  संरचना custom_handler_instance_s *custom_handler;
};

संरचना sync_socket_message_s
{
  पूर्णांक type;
  पूर्णांक error_मूल्य;
  अक्षर message[512];
};

प्रकारव्याख्या runtime_spec_schema_defs_hook hook;

// linux hooks
अक्षर *hooks[] = {
  "prestart",
  "createRuntime",
  "createContainer",
  "startContainer",
  "poststart",
  "poststop"
};

// linux namespaces
स्थिर अक्षर *namespaces[] = {
  "cgroup",
  "ipc",
  "mount",
  "network",
  "pid",
  "user",
  "uts"
};

स्थिर अक्षर *actions[] = {
  "SCMP_ACT_ALLOW",
  "SCMP_ACT_ERRNO",
  "SCMP_ACT_KILL",
  "SCMP_ACT_KILL_PROCESS",
  "SCMP_ACT_KILL_THREAD",
  "SCMP_ACT_LOG",
  "SCMP_ACT_NOTIFY",
  "SCMP_ACT_TRACE",
  "SCMP_ACT_TRAP"
};

स्थिर अक्षर *operators[] = {
  "SCMP_CMP_NE",
  "SCMP_CMP_LT",
  "SCMP_CMP_LE",
  "SCMP_CMP_EQ",
  "SCMP_CMP_GE",
  "SCMP_CMP_GT",
  "SCMP_CMP_MASKED_EQ",
};

स्थिर अक्षर *archs[] = {
  "SCMP_ARCH_AARCH64",
  "SCMP_ARCH_ARM",
  "SCMP_ARCH_MIPS",
  "SCMP_ARCH_MIPS64",
  "SCMP_ARCH_MIPS64N32",
  "SCMP_ARCH_MIPSEL",
  "SCMP_ARCH_MIPSEL64",
  "SCMP_ARCH_MIPSEL64N32",
  "SCMP_ARCH_PPC",
  "SCMP_ARCH_PPC64",
  "SCMP_ARCH_PPC64LE",
  "SCMP_ARCH_RISCV64",
  "SCMP_ARCH_S390",
  "SCMP_ARCH_S390X",
  "SCMP_ARCH_X32",
  "SCMP_ARCH_X86",
  "SCMP_ARCH_X86_64"
};

स्थिर अक्षर *mempolicy_modes[] = {
  "MPOL_DEFAULT",
  "MPOL_PREFERRED",
  "MPOL_BIND",
  "MPOL_INTERLEAVE",
  "MPOL_LOCAL",
  "MPOL_PREFERRED_MANY",
  "MPOL_WEIGHTED_INTERLEAVE"
};

स्थिर अक्षर *mempolicy_flags[] = {
  "MPOL_F_NUMA_BALANCING",
  "MPOL_F_RELATIVE_NODES",
  "MPOL_F_STATIC_NODES"
};

स्थिर अक्षर *potentially_unsafe_annotations[] = {
  "module.wasm.image/variant",
  "io.kubernetes.cri.container-type",
  "run.oci.",
  "org.criu.",
  "krun.",
};

#define SYNC_SOCKET_MESSAGE_LEN(x, l) (दुरत्व (संरचना sync_socket_message_s, message) + l)

स्थिर पूर्णांक
sync_socket_write_संदेश (पूर्णांक fd, पूर्णांक verbosity, पूर्णांक err_मूल्य, स्थिरांक अक्षर *log_संदेश)
{
  पूर्णांक ret;
  माप_प्रकार err_len;
  संरचना sync_socket_message_s संदेश;
  निवड (verbosity)
    {
    स्थिती LIBCRUN_VERBOSITY_DEBUG:
      संदेश.type = SYNC_SOCKET_DEBUG_MESSAGE;
      थांबा;
    स्थिती LIBCRUN_VERBOSITY_WARNING:
      संदेश.type = SYNC_SOCKET_WARNING_MESSAGE;
      थांबा;
    स्थिती LIBCRUN_VERBOSITY_ERROR:
      संदेश.type = SYNC_SOCKET_ERROR_MESSAGE;
      थांबा;
    }
  संदेश.error_मूल्य = err_मूल्य;

  जर (fd < 0)
    परत 0;

  err_len = म_माप (log_संदेश);
  जर (err_len >= आकार (संदेश.message))
    err_len = आकार (संदेश.message) - 1;

  स_नकल (संदेश.message, log_संदेश, err_len);
  संदेश.message[err_len] = '\0';

  ret = TEMP_FAILURE_RETRY (लिख (fd, &संदेश, SYNC_SOCKET_MESSAGE_LEN (संदेश, err_len + 1)));
  जर (UNLIKELY (ret < 0))
    परत -1;

  परत 0;
}

स्थिर पूर्णांक
sync_socket_write_error (पूर्णांक fd, libcrun_error_t *out_err)
{
  जर (fd < 0)
    परत 0;
  परत sync_socket_write_संदेश (fd, false, (*out_err)->status, (*out_err)->संदेश);
}

स्थिर शून्य
log_write_to_sync_socket (पूर्णांक errno_, स्थिरांक अक्षर *संदेश, पूर्णांक verbosity, शून्य *arg)
{
  संरचना container_entrypoint_s *entrypoint_args = arg;
  पूर्णांक fd = entrypoint_args->sync_socket;

  जर (fd < 0)
    परत;

  जर (sync_socket_write_संदेश (fd, verbosity, errno_, संदेश) < 0)
    log_write_to_stderr (errno_, संदेश, verbosity, arg);
}

स्थिर bool
is_memory_limit_too_खालचा (runtime_spec_schema_config_schema *def)
{
  स्थिरांक दीर्घ memory_limit_too_खालचा = 1024 * 1024;

  जर (def->linux == रिक्त || def->linux->resources == रिक्त)
    परत false;

  जर (def->linux->resources->memory
      && def->linux->resources->memory->limit_present
      && def->linux->resources->memory->limit < memory_limit_too_खालचा)
    परत true;

  जर (def->linux->resources->unified)
    {
      माप_प्रकार i;

      साठी (i = 0; i < def->linux->resources->unified->len; i++)
        जर (म_भेद (def->linux->resources->unified->किल्ल्या[i], "memory.max") == 0)
          {
            दीर्घ limit;

            त्रुटि_सं = 0;
            limit = म_से_दीर्घ (def->linux->resources->unified->मूल्ये[i], रिक्त, 10);
            जर (त्रुटि_सं != 0)
              परत false;
            जर (limit < memory_limit_too_खालचा)
              परत true;
          }
    }

  परत false;
}

स्थिर पूर्णांक
sync_socket_wait_sync (libcrun_context_t *context, पूर्णांक fd, bool flush, libcrun_error_t *err)
{
  संरचना sync_socket_message_s संदेश;

  जर (fd < 0)
    परत 0;

  पर्यंत (true)
    {
      पूर्णांक ret;

      ret = TEMP_FAILURE_RETRY (पठ (fd, &संदेश, आकार (संदेश)));
      जर (UNLIKELY (ret < 0))
        {
          जर (flush)
            परत 0;
          परत crun_make_error (err, त्रुटि_सं, "read from sync socket");
        }

      जर (ret == 0)
        {
          जर (flush)
            परत 0;

          परत crun_make_error (err, 0, "read from the init process");
        }

      जर (! flush && संदेश.type == SYNC_SOCKET_SYNC_MESSAGE)
        परत 0;
      अन्यथा जर (संदेश.type == SYNC_SOCKET_DEBUG_MESSAGE)
        {
          जर (context)
            context->output_handler (संदेश.error_मूल्य, संदेश.message, LIBCRUN_VERBOSITY_DEBUG, context->output_handler_arg);
          सुरूठेवा;
        }
      अन्यथा जर (संदेश.type == SYNC_SOCKET_WARNING_MESSAGE)
        {
          जर (context)
            context->output_handler (संदेश.error_मूल्य, संदेश.message, LIBCRUN_VERBOSITY_WARNING, context->output_handler_arg);
          सुरूठेवा;
        }
      अन्यथा जर (संदेश.type == SYNC_SOCKET_ERROR_MESSAGE)
        परत crun_make_error (err, संदेश.error_मूल्य, "%s", संदेश.message);
    }
}

स्थिर पूर्णांक
sync_socket_send_sync (पूर्णांक fd, bool flush_errors, libcrun_error_t *err)
{
  पूर्णांक ret;
  संरचना sync_socket_message_s संदेश = {
    0,
  };
  संदेश.type = SYNC_SOCKET_SYNC_MESSAGE;

  जर (fd < 0)
    परत 0;

  ret = TEMP_FAILURE_RETRY (लिख (fd, &संदेश, SYNC_SOCKET_MESSAGE_LEN (संदेश, 0)));
  जर (UNLIKELY (ret < 0))
    {
      जर (flush_errors)
        {
          पूर्णांक saved_errno = त्रुटि_सं;
          ret = TEMP_FAILURE_RETRY (पठ (fd, &संदेश, आकार (संदेश)));
          जर (ret >= 0 && संदेश.type == SYNC_SOCKET_ERROR_MESSAGE)
            परत crun_make_error (err, संदेश.error_मूल्य, "%s", संदेश.message);
          त्रुटि_सं = saved_errno;
        }
      परत crun_make_error (err, त्रुटि_सं, "write to sync socket");
    }

  परत 0;
}

स्थिर bool
has_cap_sys_ptrace ()
{
#ifdef HAVE_CAP
  संरचना __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  संरचना __user_cap_data_struct data[2] = { { 0 } };

  जर (UNLIKELY (capget (&hdr, data) < 0))
    परत true;

  परत (data[0].effective & (1U << CAP_SYS_PTRACE)) != 0;
#अन्यथा
  परत geteuid () == 0;
#endif
}

स्थिर libcrun_container_t *
make_container (runtime_spec_schema_config_schema *container_def, स्थिरांक अक्षर *path, स्थिरांक अक्षर *config)
{
  libcrun_container_t *container = xmalloc0 (आकार (*container));
  container->container_def = container_def;

  container->host_uid = geteuid ();
  container->host_gid = getegid ();
  container->host_has_cap_sys_ptrace = has_cap_sys_ptrace ();
  container->proc_fd = -1;

  container->annotations = make_string_map_from_json (container_def->annotations);

  /* An empty namespace path means that a new namespace is to be created,
     the same as if it is not specified.  */
  जर (container_def->linux)
    {
      माप_प्रकार i;

      साठी (i = 0; i < container_def->linux->namespaces_len; i++)
        जर (container_def->linux->namespaces[i]->path && container_def->linux->namespaces[i]->path[0] == '\0')
          {
            मुक्त (container_def->linux->namespaces[i]->path);
            container_def->linux->namespaces[i]->path = रिक्त;
          }
    }

  जर (path)
    container->config_file = xstrdup (path);
  जर (config)
    container->config_file_content = xstrdup (config);

  परत container;
}

libcrun_container_t *
libcrun_container_load_from_memory (स्थिरांक अक्षर *json, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free अक्षर *oci_error = रिक्त;
  container_def = runtime_spec_schema_config_schema_parse_data (json, रिक्त, &oci_error);
  जर (container_def == रिक्त)
    {
      crun_make_error (err, 0, "load: `%s`", oci_error);
      परत रिक्त;
    }
  परत make_container (container_def, रिक्त, json);
}

libcrun_container_t *
libcrun_container_load_from_file (स्थिरांक अक्षर *path, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free अक्षर *oci_error = रिक्त;
  cleanup_free अक्षर *content = रिक्त;
  माप_प्रकार len;
  पूर्णांक ret;

  libcrun_debug ("Loading container from config file: `%s`", path);
  ret = read_all_file (path, &content, &len, err);
  जर (UNLIKELY (ret < 0))
    परत रिक्त;

  container_def = runtime_spec_schema_config_schema_parse_data (content, रिक्त, &oci_error);
  जर (container_def == रिक्त)
    {
      crun_make_error (err, 0, "load `%s`: %s", path, oci_error);
      परत रिक्त;
    }
  परत make_container (container_def, रिक्त, content);
}

स्थिरांक अक्षर *
libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err)
{
  संरचना parser_context ctx = { 0, मानक_त्रुटि };
  parser_error gen_err = रिक्त;

  जर (container == रिक्त)
    {
      crun_make_error (err, EINVAL, "no container specified");
      परत रिक्त;
    }

  जर (container->config_file_content == रिक्त && container->container_def)
    {
      container->config_file_content = runtime_spec_schema_config_schema_generate_json (container->container_def, &ctx, &gen_err);
      जर (container->config_file_content == रिक्त)
        {
          crun_make_error (err, 0, "cannot generate the configuration JSON: %s", gen_err ?: "unknown error");
          मुक्त (gen_err);
          परत रिक्त;
        }
      मुक्त (gen_err);
    }

  जर (container->config_file_content == रिक्त)
    {
      crun_make_error (err, 0, "the container has no configuration");
      परत रिक्त;
    }

  परत container->config_file_content;
}

स्थिरांक अक्षर *
libcrun_container_get_annotation (libcrun_container_t *container, स्थिरांक अक्षर *किल्ली)
{
  जर (container == रिक्त)
    परत रिक्त;
  परत find_string_map_मूल्य (container->annotations, किल्ली);
}

माप_प्रकार
libcrun_container_get_annotations_len (libcrun_container_t *container)
{
  जर (container == रिक्त || container->annotations == रिक्त)
    परत 0;
  परत string_map_size (container->annotations);
}

पूर्णांक
libcrun_container_get_annotation_at (libcrun_container_t *container, माप_प्रकार index, स्थिरांक अक्षर **किल्ली,
                                     स्थिरांक अक्षर **मूल्य)
{
  जर (container == रिक्त)
    {
      त्रुटि_सं = दुस्फल;
      परत -1;
    }
  परत string_map_get_at (container->annotations, index, किल्ली, मूल्य);
}

uid_t
libcrun_container_get_uid (libcrun_container_t *container)
{
  जर (container == रिक्त)
    परत (uid_t) -1;
  परत container->host_uid;
}

gid_t
libcrun_container_get_gid (libcrun_container_t *container)
{
  जर (container == रिक्त)
    परत (gid_t) -1;
  परत container->host_gid;
}

शून्य
libcrun_container_free (libcrun_container_t *ctr)
{
  जर (ctr == रिक्त)
    परत;

  जर (ctr->cleanup_private_data)
    ctr->cleanup_private_data (ctr->private_data);

  जर (ctr->container_def)
    free_runtime_spec_schema_config_schema (ctr->container_def);

  free_string_map (ctr->annotations);

  जर (ctr->proc_fd >= 0)
    बंद (ctr->proc_fd);

  मुक्त (ctr->config_file_content);
  मुक्त (ctr->config_file);
  मुक्त (ctr);
}

libcrun_context_t *
libcrun_context_new (स्थिरांक अक्षर *id, स्थिरांक अक्षर *state_मूळ, libcrun_error_t *err)
{
  libcrun_context_t *ctx = xmalloc0 (आकार (*ctx));

  /* Go through the setters so that the owned_* copies and the borrowed
     pointers stay in sync and are released by libcrun_context_free.  */
  libcrun_context_set_id (ctx, id);
  libcrun_context_set_state_मूळ (ctx, state_मूळ);
  libcrun_context_set_bundle (ctx, ".");
  ctx->fifo_exec_wait_fd = -1;

  जर (libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, id, रिक्त, err) < 0)
    {
      libcrun_context_free (ctx);
      परत रिक्त;
    }

  ctx->handler_manager = libcrun_handler_manager_create (err);
  जर (ctx->handler_manager == रिक्त)
    {
      libcrun_context_free (ctx);
      परत रिक्त;
    }

#ifdef CRUN_LIBDIR
  जर (access (CRUN_LIBDIR "/handlers", F_OK) == 0)
    {
      जर (libcrun_handler_manager_load_directory (ctx->handler_manager, CRUN_LIBDIR "/handlers", err) < 0)
        {
          libcrun_context_free (ctx);
          परत रिक्त;
        }
    }
#endif

  परत ctx;
}

शून्य
libcrun_context_free (libcrun_context_t *ctx)
{
  जर (ctx == रिक्त)
    परत;

  मुक्त (ctx->owned_state_मूळ);
  मुक्त (ctx->owned_id);
  मुक्त (ctx->owned_bundle);
  मुक्त (ctx->owned_console_socket);
  मुक्त (ctx->owned_pid_file);
  मुक्त (ctx->owned_notify_socket);
  मुक्त (ctx->owned_handler);

  जर (ctx->handler_manager)
    handler_manager_free (ctx->handler_manager);

  मुक्त (ctx);
}

#define LIBCRUN_CONTEXT_SET_STRING(field)                  \
  करा                                                       \
    {                                                      \
      मुक्त (ctx->owned_##field);                           \
      ctx->owned_##field = मूल्य ? xstrdup (मूल्य) : रिक्त; \
      ctx->field = ctx->owned_##field;                     \
  } पर्यंत (0)

शून्य
libcrun_context_set_id (libcrun_context_t *ctx, स्थिरांक अक्षर *मूल्य)
{
  LIBCRUN_CONTEXT_SET_STRING (id);
}

शून्य
libcrun_context_set_state_मूळ (libcrun_context_t *ctx, स्थिरांक अक्षर *मूल्य)
{
  LIBCRUN_CONTEXT_SET_STRING (state_मूळ);
}

शून्य
libcrun_context_set_bundle (libcrun_context_t *ctx, स्थिरांक अक्षर *मूल्य)
{
  LIBCRUN_CONTEXT_SET_STRING (bundle);
}

शून्य
libcrun_context_set_console_socket (libcrun_context_t *ctx, स्थिरांक अक्षर *मूल्य)
{
  LIBCRUN_CONTEXT_SET_STRING (console_socket);
}

शून्य
libcrun_context_set_pid_file (libcrun_context_t *ctx, स्थिरांक अक्षर *मूल्य)
{
  LIBCRUN_CONTEXT_SET_STRING (pid_file);
}

शून्य
libcrun_context_set_notify_socket (libcrun_context_t *ctx, स्थिरांक अक्षर *मूल्य)
{
  LIBCRUN_CONTEXT_SET_STRING (notify_socket);
}

शून्य
libcrun_context_set_handler (libcrun_context_t *ctx, स्थिरांक अक्षर *मूल्य)
{
  LIBCRUN_CONTEXT_SET_STRING (handler);
}

#undef LIBCRUN_CONTEXT_SET_STRING

स्थिरांक अक्षर *
libcrun_context_get_pid_file (libcrun_context_t *ctx)
{
  परत ctx->pid_file;
}

स्थिरांक अक्षर *
libcrun_context_get_state_मूळ (libcrun_context_t *ctx)
{
  जर (ctx == रिक्त)
    परत रिक्त;
  परत ctx->state_मूळ;
}

शून्य
libcrun_context_set_preserve_fds (libcrun_context_t *ctx, पूर्णांक preserve_fds)
{
  ctx->preserve_fds = preserve_fds;
}

पूर्णांक
libcrun_context_get_preserve_fds (libcrun_context_t *ctx)
{
  परत ctx->preserve_fds;
}

शून्य
libcrun_context_set_listen_fds (libcrun_context_t *ctx, पूर्णांक listen_fds)
{
  ctx->listen_fds = listen_fds;
}

शून्य
libcrun_context_set_args (libcrun_context_t *ctx, पूर्णांक argc, अक्षर **argv)
{
  ctx->argc = argc;
  ctx->argv = argv;
}

शून्य
libcrun_context_set_systemd_cgroup (libcrun_context_t *ctx, bool मूल्य)
{
  ctx->systemd_cgroup = मूल्य;
}

शून्य
libcrun_context_set_detach (libcrun_context_t *ctx, bool मूल्य)
{
  ctx->detach = मूल्य;
}

शून्य
libcrun_context_set_no_new_keyring (libcrun_context_t *ctx, bool मूल्य)
{
  ctx->no_new_keyring = मूल्य;
}

शून्य
libcrun_context_set_no_pivot (libcrun_context_t *ctx, bool मूल्य)
{
  ctx->no_pivot = मूल्य;
}

शून्य
libcrun_context_set_force_no_cgroup (libcrun_context_t *ctx, bool मूल्य)
{
  ctx->force_no_cgroup = मूल्य;
}

शून्य
libcrun_context_set_output_handler (libcrun_context_t *ctx, crun_output_handler handler, शून्य *arg)
{
  ctx->output_handler = handler;
  ctx->output_handler_arg = arg;
}

पूर्णांक
libcrun_context_init_logging (libcrun_context_t *ctx, स्थिरांक अक्षर *log, libcrun_error_t *err)
{
  परत libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, ctx->id, log, err);
}

स्थिर पूर्णांक
block_signals (libcrun_error_t *err)
{
  पूर्णांक ret;
  sigset_t mask;
  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, रिक्त);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "sigprocmask");
  परत 0;
}

स्थिर पूर्णांक
unblock_signals (libcrun_error_t *err)
{
  पूर्णांक i;
  पूर्णांक ret;
  sigset_t mask;
  संरचना sigaction act = {};

  sigfillset (&mask);
  ret = sigprocmask (SIG_UNBLOCK, &mask, रिक्त);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "sigprocmask");

  act.sa_handler = संक_पूर्व;
  साठी (i = 0; i < NSIG; i++)
    {
      ret = sigaction (i, &act, रिक्त);
      जर (ret < 0 && त्रुटि_सं != EINVAL)
        परत crun_make_error (err, त्रुटि_सं, "sigaction");
    }

  परत 0;
}

/* must be used on the host before pivot_root(2).  */
स्थिर पूर्णांक
initialize_security (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = libcrun_init_caps (container, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (UNLIKELY (proc == रिक्त))
    परत 0;

  जर (proc->apparmor_profile)
    {
      ret = libcrun_initialize_apparmor (err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  ret = libcrun_initialize_selinux (container, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  परत 0;
}

/* Emit the "annotations" map.  Returns a json_gen_status; callers can wrap
   the call with GEN_OR_FAIL.  */
स्थिर पूर्णांक
gen_annotations (json_gen_ctx *gen, json_map_string_string *annotations)
{
  माप_प्रकार i;
  पूर्णांक r;

  GEN_किल्ली (gen, "annotations");
  GEN_OR_FAIL (json_gen_map_open (gen));

  साठी (i = 0; i < annotations->len; i++)
    {
      /* An annotation whose value is null in the document is parsed into a
         NULL, which cannot be emitted as a JSON string; leave it out.  */
      जर (annotations->किल्ल्या[i] == रिक्त || annotations->मूल्ये[i] == रिक्त)
        सुरूठेवा;

      GEN_STR (gen, annotations->किल्ल्या[i]);
      GEN_STR (gen, annotations->मूल्ये[i]);
    }

  परत json_gen_map_close (gen);

gen_error:
  परत r;
}

/* Return the bundle path to report in the container state.  crun always
   chdir()s into the bundle, so fall back to getcwd() when the caller has no
   absolute bundle path, as can happen when libcrun is used as a library.
   *ALLOCATED must be freed by the caller.  */
स्थिर स्थिरांक अक्षर *
get_bundle_path (स्थिरांक अक्षर *bundle, अक्षर **allocated)
{
  जर (bundle != रिक्त && bundle[0] == '/')
    परत bundle;

  *allocated = getcwd (रिक्त, 0);
  जर (*allocated == रिक्त)
    OOM ();
  परत *allocated;
}

स्थिर पूर्णांक
do_hooks (runtime_spec_schema_config_schema *def, pid_t pid, स्थिरांक अक्षर *id, bool keep_going, स्थिरांक अक्षर *bundle,
          स्थिरांक अक्षर *cwd, स्थिरांक अक्षर *status, स्थिरांक अक्षर *name, hook **hooks, माप_प्रकार hooks_len, पूर्णांक out_fd,
          पूर्णांक err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  माप_प्रकार i, stdin_len;
  पूर्णांक r, ret;
  अक्षर *मानक_निवेश = रिक्त;
  cleanup_free अक्षर *bundle_allocated = रिक्त;
  स्थिरांक अक्षर *rootfs = def->मूळ ? def->मूळ->path : "";
  cleanup_json_gen json_gen_ctx *gen = रिक्त;

  bundle = get_bundle_path (bundle, &bundle_allocated);

  जर (! json_gen_init (&gen, रिक्त))
    परत crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 0);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_किल्ली (gen, "ociVersion");
  GEN_STR (gen, "1.0");

  GEN_किल्ली (gen, "id");
  GEN_STR (gen, id);

  GEN_किल्ली (gen, "pid");
  GEN_OR_FAIL (map_int (gen, pid));

  GEN_किल्ली (gen, "root");
  GEN_STR (gen, rootfs);

  GEN_किल्ली (gen, "bundle");
  GEN_STR (gen, bundle);

  GEN_किल्ली (gen, "status");
  GEN_STR (gen, status);

  जर (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, (स्थिरांक अक्षर **) &मानक_निवेश, &stdin_len));

  ret = 0;

  bool error_created = false;
  साठी (i = 0; i < hooks_len; i++)
    {
      अक्षर **env = environ;

      जर (hooks[i]->env)
        env = hooks[i]->env;

      /* Release the error from the previous iteration, if any.  */
      जर (error_created)
        {
          crun_error_release (err);
          error_created = false;
        }

      ret = run_process_with_stdin_timeout_envp (hooks[i]->path, hooks[i]->args, cwd, hooks[i]->timeout, env,
                                                 मानक_निवेश, stdin_len, out_fd, err_fd, can_ignore_chdir_errors, err);
      जर (UNLIKELY (ret < 0))
        error_created = true;

      जर (UNLIKELY (ret != 0))
        {
          जर (keep_going)
            libcrun_warning ("error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
          अन्यथा
            {
              libcrun_error (0, "error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
              थांबा;
            }
        }
    }

  परत ret;

gen_error:
  परत json_gen_error_to_crun_error (r, err);
}

स्थिर पूर्णांक
get_json_gen_result (json_gen_ctx *gen, अक्षर **out, माप_प्रकार *out_len)
{
  स्थिरांक अक्षर *buf = रिक्त;
  माप_प्रकार buf_len = 0;
  पूर्णांक r;

  r = json_gen_get_buf (gen, &buf, &buf_len);
  जर (UNLIKELY (r != json_gen_status_ok))
    परत r;

  *out_len = buf_len;

  *out = xmalloc (buf_len + 1);
  स_नकल (*out, buf, buf_len);
  (*out)[buf_len] = '\0';

  परत json_gen_status_ok;
}

स्थिर पूर्णांक
get_seccomp_receiver_fd_payload (libcrun_container_t *container, स्थिरांक अक्षर *status, pid_t own_pid,
                                 अक्षर **seccomp_fd_payload, माप_प्रकार *seccomp_fd_payload_len, libcrun_error_t *err)
{
  पूर्णांक r;
  json_gen_ctx *gen = रिक्त;
  runtime_spec_schema_config_schema *def = container->container_def;
  स्थिरांक अक्षर *स्थिरांक OCI_VERSION = "0.2.0";

  जर (! json_gen_init (&gen, रिक्त))
    परत crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_किल्ली (gen, "ociVersion");
  GEN_STR (gen, OCI_VERSION);

  GEN_किल्ली (gen, "fds");
  GEN_OR_FAIL (json_gen_array_open (gen));

  GEN_किल्ली (gen, "seccompFd");
  GEN_OR_FAIL (json_gen_array_close (gen));

  GEN_किल्ली (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  जर (def && def->linux && def->linux->seccomp)
    {
      स्थिरांक अक्षर *metadata = def->linux->seccomp->listener_metadata;

      जर (metadata)
        {
          GEN_किल्ली (gen, "metadata");
          GEN_STR (gen, metadata);
        }
    }

  /* State.  */
  GEN_किल्ली (gen, "state");
  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_किल्ली (gen, "ociVersion");
  GEN_STR (gen, OCI_VERSION);

  जर (container->context && container->context->id)
    {
      GEN_किल्ली (gen, "id");
      GEN_STR (gen, container->context->id);
    }

  GEN_किल्ली (gen, "status");
  GEN_STR (gen, status);

  GEN_किल्ली (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  जर (container->context && container->context->bundle)
    {
      GEN_किल्ली (gen, "bundle");
      GEN_STR (gen, container->context->bundle);
    }

  जर (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));
  /* End state.  */

  GEN_OR_FAIL (json_gen_map_close (gen));

  r = get_json_gen_result (gen, seccomp_fd_payload, seccomp_fd_payload_len);

gen_error:
  json_gen_free (gen);

  परत json_gen_error_to_crun_error (r, err);
}

स्थिर पूर्णांक
send_sync_cb (शून्य *data, libcrun_error_t *err)
{
  पूर्णांक sync_socket_fd = *((पूर्णांक *) data);
  पूर्णांक ret;

  /* sync 2.  */
  ret = sync_socket_send_sync (sync_socket_fd, false, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  /* sync 3.  */
  परत sync_socket_wait_sync (रिक्त, sync_socket_fd, false, err);
}

स्थिर पूर्णांक
maybe_chown_std_streams (uid_t container_uid, gid_t container_gid, libcrun_error_t *err)
{
  पूर्णांक ret, i;

  साठी (i = 0; i < 3; i++)
    {
      जर (! isatty (i))
        {
          संरचना stat statbuf;
          ret = ख_स्थिति (i, &statbuf);
          जर (UNLIKELY (ret < 0))
            {
              जर (त्रुटि_सं == EBADF)
                सुरूठेवा;
              परत crun_make_error (err, त्रुटि_सं, "fstat fd `%d`", i);
            }

          /* Skip chown for device files */
          जर (S_ISCHR (statbuf.st_mode) || S_ISBLK (statbuf.st_mode))
            सुरूठेवा;

          ret = fchown (i, container_uid, container_gid);
          जर (UNLIKELY (ret < 0))
            {
              /* EINVAL means the user is not mapped in the current userns.
                 Ignore EPERM and EROFS as well as there is no reason to fail
                 so early, and let the container payload deal with it.
                 EBADF means fd is closed.
              */
              जर (त्रुटि_सं == EINVAL || त्रुटि_सं == EPERM || त्रुटि_सं == EROFS || त्रुटि_सं == EBADF)
                सुरूठेवा;

              परत crun_make_error (err, त्रुटि_सं, "fchown std stream `%i`", i);
            }
        }
    }
  परत 0;
}

पूर्णांक
libcrun_container_notify_handler (संरचना container_entrypoint_s *args,
                                  गणना handler_configure_phase phase,
                                  libcrun_container_t *container, स्थिरांक अक्षर *rootfs,
                                  libcrun_error_t *err)
{
  संरचना custom_handler_s *h;

  जर (args->custom_handler == रिक्त)
    परत 0;

  h = args->custom_handler->vtable;
  जर (h == रिक्त || h->configure_container == रिक्त)
    परत 0;

  परत h->configure_container (args->custom_handler->cookie, phase,
                                 args->context, container,
                                 rootfs, err);
}

/* Resolve and normalize the container rootfs path.  */
स्थिर पूर्णांक
resolve_rootfs_path (libcrun_container_t *container, अक्षर **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  जर (def->मूळ && def->मूळ->path)
    {
      *rootfs = realpath (def->मूळ->path, रिक्त);
      जर (UNLIKELY (*rootfs == रिक्त))
        {
          पूर्णांक saved_errno = त्रुटि_सं;

          /* If realpath failed for any reason, try the relative directory.  */
          जर (def->मूळ->path[0] == '/')
            {
              cleanup_free अक्षर *cwd = रिक्त;
              libcrun_error_t tmp_err = रिक्त;
              ssize_t len;
              पूर्णांक proc_fd;

              proc_fd = libcrun_get_cached_proc_fd (container, &tmp_err);
              जर (LIKELY (proc_fd >= 0))
                {
                  len = safe_readlinkat (proc_fd, "self/cwd", &cwd, 0, &tmp_err);
                  जर (LIKELY (len > 0))
                    {
                      /* If the rootfs is under the current working directory, just use its relative path.  */
                      जर (has_dir_prefix (def->मूळ->path, cwd))
                        {
                          स्थिरांक अक्षर *it = consume_slashes (def->मूळ->path + len);
                          जर (LIKELY (*it))
                            {
                              पूर्णांक ret = faccessat (AT_FDCWD, it, F_OK, 0);
                              जर (LIKELY (ret == 0))
                                *rootfs = xstrdup (it);
                            }
                        }
                    }
                }
              crun_error_release (&tmp_err);
            }

          जर (*rootfs == रिक्त)
            {
              जर (saved_errno == ENOENT)
                परत crun_make_error (err, ENOENT, "the rootfs path `%s` does not exist", def->मूळ->path);

              *rootfs = xstrdup (def->मूळ->path);
            }
        }
    }
  परत 0;
}

/* Initialize the environment variables.  */
स्थिर पूर्णांक
setup_environment (runtime_spec_schema_config_schema *def, uid_t container_uid, libcrun_error_t *err)
{
  पूर्णांक ret;

  जर (clearenv ())
    परत crun_make_error (err, त्रुटि_सं, "clearenv");

  जर (def->process)
    {
      माप_प्रकार i;

      साठी (i = 0; i < def->process->env_len; i++)
        {
          /* A null element of the array of variables is parsed into a NULL,
             which putenv(3) cannot be handed.  */
          जर (def->process->env[i] == रिक्त)
            परत crun_make_error (err, EINVAL, "environment variable is not specified");

          जर (putenv (def->process->env[i]) < 0)
            परत crun_make_error (err, त्रुटि_सं, "putenv `%s`", def->process->env[i]);
        }
    }

  जर (is_empty_string (दो_पर्या ("HOME")))
    {
      ret = set_home_env (container_uid);
      जर (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          जर (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            परत crun_make_error (err, त्रुटि_सं, "setenv HOME");
        }
    }

  परत 0;
}

/* Configure terminal and console for interactive containers.  */
स्थिर पूर्णांक
setup_terminal (संरचना container_entrypoint_s *entrypoint_args, libcrun_container_t *container,
                पूर्णांक console_socket, पूर्णांक console_socketpair, पूर्णांक has_terminal, libcrun_error_t *err)
{
  cleanup_close पूर्णांक terminal_fd = -1;
  पूर्णांक ret;

  जर (! has_terminal)
    परत 0;

  ख_साफ (मानक_त्रुटि);

  जर (console_socket >= 0 || (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0))
    {
      terminal_fd = libcrun_set_terminal (container, err);
      जर (UNLIKELY (terminal_fd < 0))
        परत terminal_fd;
    }

  जर (console_socket >= 0)
    {
      ret = send_fd_to_socket (console_socket, terminal_fd, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }
  अन्यथा जर (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0)
    {
      ret = send_fd_to_socket (console_socketpair, terminal_fd, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  परत 0;
}

/* Resolve and validate the container's executable path.  */
स्थिर पूर्णांक
setup_executable_path (संरचना container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                       अक्षर **exec_path, libcrun_error_t *err)
{
  पूर्णांक ret;

  जर (def->process && def->process->args)
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      जर (UNLIKELY (ret < 0))
        {
          जर (entrypoint_args->custom_handler == रिक्त && crun_error_get_errno (err) == ENOENT)
            परत ret;

          /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
             once the process switched to the use that runs in the container.  This might be necessary
             when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
             is not honored.  */
          crun_error_release (err);
        }
    }

  परत 0;
}

/* Apply security settings for the container process.  */
स्थिर पूर्णांक
apply_security_settings (संरचना container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                         libcrun_container_t *container, pid_t own_pid, libcrun_error_t *err)
{
  पूर्णांक ret;
  runtime_spec_schema_config_schema_process_capabilities *capabilities;
  पूर्णांक no_new_privs;

  जर (def->process && ! def->process->no_new_privileges)
    {
      अक्षर **seccomp_flags = रिक्त;
      माप_प्रकार seccomp_flags_len = 0;
      cleanup_free अक्षर *seccomp_fd_payload = रिक्त;
      माप_प्रकार seccomp_fd_payload_len = 0;

      जर (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      जर (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  capabilities = def->process ? def->process->capabilities : रिक्त;
  no_new_privs = def->process ? def->process->no_new_privileges : 1;
  ret = libcrun_set_caps (capabilities, container->container_uid, container->container_gid, no_new_privs,
                          entrypoint_args->custom_handler != रिक्त, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  परत 0;
}

/* Create the process that joins the existing PID namespace now that the
   host mounts are not visible anymore, and continue the setup from there.  */
स्थिर पूर्णांक
do_delayed_pidns_fork (पूर्णांक sync_socket, pid_t *own_pid, libcrun_error_t *err)
{
  pid_t pid;
  पूर्णांक ret;

  pid = विभाजन ();
  जर (UNLIKELY (pid < 0))
    परत crun_make_error (err, त्रुटि_सं, "fork");

  जर (pid)
    {
      /* Report the new PID to the parent and exit immediately.  */
      ret = sync_socket_send_sync (sync_socket, false, err);
      जर (LIKELY (ret == 0))
        {
          ret = TEMP_FAILURE_RETRY (लिख (sync_socket, &pid, आकार (pid)));
          जर (LIKELY (ret == आकार (pid)))
            _safe_exit (निकास_सफल);
        }

      समाप्त (pid, SIGKILL);
      _safe_exit (निकास_त्रुटि);
    }

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (पठ (sync_socket, own_pid, आकार (*own_pid)));
  जर (UNLIKELY (ret != आकार (*own_pid)))
    {
      जर (ret >= 0)
        त्रुटि_सं = 0;
      परत crun_make_error (err, त्रुटि_सं, "read from sync socket");
    }

  परत 0;
}

संरचना delayed_pidns_fork_s
{
  libcrun_container_t *container;
  पूर्णांक sync_socket;
  pid_t *own_pid;
};

स्थिर पूर्णांक
delayed_pidns_fork_cb (शून्य *data, libcrun_error_t *err)
{
  संरचना delayed_pidns_fork_s *d = data;

  जर (! libcrun_container_delayed_pidns_fork (d->container))
    परत 0;

  परत do_delayed_pidns_fork (d->sync_socket, d->own_pid, err);
}

स्थिर पूर्णांक
container_init_setup (शून्य *args, pid_t *own_pid, अक्षर *notify_socket,
                      पूर्णांक sync_socket, अक्षर **exec_path, libcrun_error_t *err)
{
  संरचना container_entrypoint_s *entrypoint_args = args;
  libcrun_container_t *container = entrypoint_args->container;
  bool chdir_done = false;
  पूर्णांक ret;
  पूर्णांक has_terminal;
  cleanup_close पूर्णांक console_socket = -1;
  cleanup_close पूर्णांक console_socketpair = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free अक्षर *rootfs = रिक्त;
  संरचना delayed_pidns_fork_s delayed_pidns_fork = {
    .container = container,
    .sync_socket = sync_socket,
    .own_pid = own_pid,
  };

  ret = initialize_security (container, def->process, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = libcrun_configure_network (container, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = resolve_rootfs_path (container, &rootfs, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  /* Configure terminal socket pair for container communication.  */
  जर (entrypoint_args->terminal_socketpair[0] >= 0)
    {
      close_and_reset (&entrypoint_args->terminal_socketpair[0]);
      console_socketpair = entrypoint_args->terminal_socketpair[1];
    }

  /* sync 1.  */
  ret = sync_socket_wait_sync (रिक्त, sync_socket, false, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  has_terminal = container->container_def->process && container->container_def->process->terminal;
  जर (has_terminal && entrypoint_args->context->console_socket)
    console_socket = entrypoint_args->console_socket_fd;

  ret = libcrun_set_sysctl (container, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_BEFORE_MOUNTS, container, rootfs, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = libcrun_do_pivot_मूळ (container, entrypoint_args->context->no_pivot, &rootfs, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  /* sync 2 and 3 are sent as part of libcrun_set_mounts.  */
  ret = libcrun_set_mounts (entrypoint_args, container, rootfs, send_sync_cb, &sync_socket, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  /* The createContainer hooks MUST run after the mounts are set up but before
     the pivot_root.  When these hooks are present can_use_open_tree_namespace()
     forces the deferred-pivot path, so the actual pivot happens later in
     libcrun_finalize_mounts().  */
  जर (def->hooks && def->hooks->create_container_len)
    {
      libcrun_error_t tmp_err = रिक्त;
      पूर्णांक in_userns = check_running_in_user_namespace (&tmp_err);
      जर (tmp_err)
        crun_error_release (&tmp_err);
      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, रिक्त, "created", "createContainer",
                      (hook **) def->hooks->create_container,
                      def->hooks->create_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      जर (UNLIKELY (ret != 0))
        परत ret;
    }

  ret = libcrun_finalize_mounts (entrypoint_args, container, rootfs, delayed_pidns_fork_cb, &delayed_pidns_fork, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (def->process)
    {
      ret = libcrun_set_selinux_label (container, def->process, false, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      ret = libcrun_set_apparmor_profile (container, def->process, false, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  ret = mark_or_close_fds_ge_than (container, entrypoint_args->context->preserve_fds + 3, false, err);
  जर (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ret = libcrun_reopen_dev_null (err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = maybe_chown_std_streams (container->container_uid, container->container_gid, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = setup_environment (def, container->container_uid, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  /* Set primary process to 1 explicitly if nothing is configured and LISTEN_FD is not set.  */
  जर (entrypoint_args->context->listen_fds > 0 && दो_पर्या ("LISTEN_PID") == रिक्त)
    {
      libcrun_warning ("setting LISTEN_PID=1 since no previous configuration was found");
      जर (UNLIKELY (setenv ("LISTEN_PID", "1", 1) < 0))
        परत crun_make_error (err, त्रुटि_सं, "setenv LISTEN_PID");
    }

  /* Attempt to chdir immediately here, before doing the setresuid.  If we fail here, let's
     try again later once the process switched to the user that runs in the container.  */
  जर (def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      जर (LIKELY (ret == 0))
        chdir_done = true;
      अन्यथा
        crun_error_release (err);
    }

  ret = setsid ();
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "setsid");

  ret = setup_terminal (entrypoint_args, container, console_socket, console_socketpair, has_terminal, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  close_and_reset (&console_socket);
  close_and_reset (&console_socketpair);

  ret = setup_executable_path (entrypoint_args, def, exec_path, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = libcrun_set_hostname (container, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = libcrun_set_domainname (container, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  जर (def->process && def->process->user)
    umask (def->process->user->umask_present ? def->process->user->umask : 0022);

  ret = apply_security_settings (entrypoint_args, def, container, *own_pid, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (UNLIKELY (def->process && def->process->args && *exec_path == रिक्त))
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      जर (UNLIKELY (ret < 0))
        {
          जर (entrypoint_args->custom_handler == रिक्त || is_empty_string (def->process->args[0]))
            परत ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          *exec_path = xstrdup (def->process->args[0]);
        }
    }

  /* The chdir was not already performed, so try again now after switching to the UID/GID in the container.  */
  जर (! chdir_done && def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  जर (notify_socket && putenv (notify_socket) < 0)
    परत crun_make_error (err, त्रुटि_सं, "putenv `%s`", notify_socket);

  परत 0;
}

स्थिर पूर्णांक
open_hooks_output (libcrun_container_t *container, पूर्णांक *out_fd, पूर्णांक *err_fd, libcrun_error_t *err)
{
  स्थिरांक अक्षर *annotation;

  *err_fd = *out_fd = -1;

  libcrun_debug ("Opening hooks output");
  annotation = find_annotation (container, "run.oci.hooks.stdout");
  जर (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stdout` annotation");
      *out_fd = TEMP_FAILURE_RETRY (खोलो (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      जर (UNLIKELY (*out_fd < 0))
        परत crun_make_error (err, त्रुटि_सं, "open `%s`", annotation);
    }

  annotation = find_annotation (container, "run.oci.hooks.stderr");
  जर (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stderr` annotation");
      *err_fd = TEMP_FAILURE_RETRY (खोलो (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      जर (UNLIKELY (*err_fd < 0))
        परत crun_make_error (err, त्रुटि_सं, "open `%s`", annotation);
    }

  परत 0;
}

/* The environment the process was started with lives on the initial stack, right after the
   argv strings and before the string AT_EXECFN points to.  clearenv(3) only drops the pointers
   to it, so when there is no execv to replace the stack, erase it or the container process can
   still read it, e.g. through /proc/self/environ.  */
स्थिर शून्य
erase_initial_environment (अक्षर **argv, पूर्णांक argc)
{
  अक्षर *start = argv[argc - 1] + म_माप (argv[argc - 1]) + 1;
  अक्षर *end = (अक्षर *) getauxval (AT_EXECFN);

  जर (end > start)
    स_रखो (start, 0, end - start);
}

स्थिर शून्य
rewrite_argv (अक्षर **argv, पूर्णांक argc, स्थिरांक अक्षर *name, अक्षर **args, माप_प्रकार args_len)
{
  cleanup_free अक्षर *decorated_name = रिक्त;
  माप_प्रकार available_len = argv[argc - 1] - argv[0] + म_माप (argv[argc - 1]) + 1;
  cleanup_free अक्षर *new_argv = रिक्त;
  माप_प्रकार i, so_far = 0, needed = 0;

  needed = xasprintf (&decorated_name, "[libcrun:%s]", name) + 1;
  साठी (i = 0; i < args_len; i++)
    needed += म_माप (args[i]) + 1;

  new_argv = xmalloc0 (needed + 1);
  म_नकल (new_argv, decorated_name);
  so_far = म_माप (decorated_name) + 1;

  जर (available_len >= needed)
    {
      साठी (i = 0; i < args_len; i++)
        {
          म_नकल (new_argv + so_far, args[i]);
          so_far += म_माप (args[i]) + 1;
        }
    }

  जर (so_far >= available_len)
    so_far = available_len - 1;

  स_रखो (argv[0], 0, available_len);
  स_नकल (argv[0], new_argv, so_far);
}

/* Entrypoint to the container.  */
स्थिर पूर्णांक
container_init (शून्य *args, अक्षर *notify_socket, पूर्णांक sync_socket, libcrun_error_t *err)
{
  संरचना container_entrypoint_s *entrypoint_args = args;
  पूर्णांक ret;
  runtime_spec_schema_config_schema *def = entrypoint_args->container->container_def;
  cleanup_free अक्षर *exec_path = रिक्त;
  __attribute__ ((unused)) cleanup_free अक्षर *notify_socket_cleanup = notify_socket;
  pid_t own_pid = 0;

  entrypoint_args->sync_socket = sync_socket;

  crun_set_output_handler (log_write_to_sync_socket, args);

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (पठ (sync_socket, &own_pid, आकार (own_pid)));
  जर (UNLIKELY (ret != आकार (own_pid)))
    {
      जर (ret >= 0)
        त्रुटि_सं = 0;
      परत crun_make_error (err, त्रुटि_सं, "read from sync socket");
    }

  ret = container_init_setup (args, &own_pid, notify_socket, sync_socket, &exec_path, err);
  जर (UNLIKELY (ret < 0))
    {
      /* If it fails to write the error using the sync socket, then fallback
         to stderr.  */
      जर (sync_socket_write_error (sync_socket, err) == 0)
        {
          crun_error_release (err);
          /* A return value of > 0 means to fail with an error without err being set so to avoid
             printing it twice.  */
          परत 1;
        }
      परत ret;
    }

  entrypoint_args->sync_socket = -1;

  ret = unblock_signals (err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  /* sync 4.  */
  ret = sync_socket_send_sync (sync_socket, false, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  close_and_reset (&sync_socket);

  जर (entrypoint_args->context->fifo_exec_wait_fd >= 0)
    {
      अक्षर buffer[1];
      fd_set read_set;
      cleanup_close पूर्णांक fd = entrypoint_args->context->fifo_exec_wait_fd;
      entrypoint_args->context->fifo_exec_wait_fd = -1;

      FD_ZERO (&read_set);
      FD_SET (fd, &read_set);
      करा
        {
          ret = select (fd + 1, &read_set, रिक्त, रिक्त, रिक्त);
          जर (UNLIKELY (ret < 0))
            परत crun_make_error (err, त्रुटि_सं, "select");

          ret = TEMP_FAILURE_RETRY (पठ (fd, buffer, आकार (buffer)));
          जर (UNLIKELY (ret < 0))
            परत crun_make_error (err, त्रुटि_सं, "read from the exec fifo");
      } पर्यंत (ret == 0);

      close_and_reset (&entrypoint_args->context->fifo_exec_wait_fd);
    }

  crun_set_output_handler (log_write_to_stderr, रिक्त);

  जर (def->process && def->process->no_new_privileges)
    {
      अक्षर **seccomp_flags = रिक्त;
      माप_प्रकार seccomp_flags_len = 0;
      cleanup_free अक्षर *seccomp_fd_payload = रिक्त;
      माप_प्रकार seccomp_fd_payload_len = 0;

      जर (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      जर (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (entrypoint_args->container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags,
                                   seccomp_flags_len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  जर (UNLIKELY (def->process == रिक्त))
    परत crun_make_error (err, 0, "block `process` not found");

  जर (UNLIKELY (exec_path == रिक्त))
    परत crun_make_error (err, 0, "executable path not specified");

  जर (def->hooks && def->hooks->start_container_len)
    {
      libcrun_container_t *container = entrypoint_args->container;
      libcrun_error_t tmp_err = रिक्त;
      पूर्णांक in_userns = check_running_in_user_namespace (&tmp_err);
      जर (tmp_err)
        crun_error_release (&tmp_err);

      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, रिक्त, "created", "startContainer",
                      (hook **) def->hooks->start_container,
                      def->hooks->start_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      जर (UNLIKELY (ret != 0))
        परत ret;

      /* Seek stdout/stderr to the end.  If the hooks were using the same files,
         the container process overwrites what was previously written.  */
      (शून्य) lseek (1, 0, अंत_से);
      (शून्य) lseek (2, 0, अंत_से);
    }

  जर (entrypoint_args->custom_handler)
    {
      /* Files marked with O_CLOEXEC are closed at execv time, so make sure they are closed now.
         This is a best effort operation, because the seccomp filter is already in place and it could
         stop some syscalls used by mark_or_close_fds_ge_than.
      */
      जर (entrypoint_args->custom_handler->vtable->close_fds)
        {
          ret = entrypoint_args->custom_handler->vtable->close_fds (entrypoint_args->custom_handler->cookie,
                                                                    entrypoint_args->container,
                                                                    entrypoint_args->context->preserve_fds,
                                                                    err);
        }
      अन्यथा
        {
          ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
        }
      जर (UNLIKELY (ret < 0))
        crun_error_release (err);

      prctl (PR_SET_NAME, entrypoint_args->custom_handler->vtable->name);

      जर (entrypoint_args->context->argv)
        {
          erase_initial_environment (entrypoint_args->context->argv, entrypoint_args->context->argc);
          rewrite_argv (entrypoint_args->context->argv, entrypoint_args->context->argc,
                        entrypoint_args->custom_handler->vtable->name, def->process->args,
                        def->process->args_len);

          /* It is a quite destructive operation as we might be referencing data from the old
             argv, so make sure the context is not reused.  */
          entrypoint_args->context->argv = रिक्त;
          entrypoint_args->context = रिक्त;
        }

      ret = libcrun_set_selinux_label (entrypoint_args->container, def->process, true, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      ret = libcrun_set_apparmor_profile (entrypoint_args->container, def->process, true, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      ret = entrypoint_args->custom_handler->vtable->run_func (entrypoint_args->custom_handler->cookie,
                                                               entrypoint_args->container,
                                                               exec_path,
                                                               def->process->args);
      जर (ret != 0)
        परत crun_make_error (err, 0, "exec container process failed with handler as `%s`", entrypoint_args->custom_handler->vtable->name);

      परत ret;
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation since the seccomp profile is already in place now and might block
     some of the syscalls needed by mark_or_close_fds_ge_than.  */
  ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
  जर (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, def->process->args));

  जर (त्रुटि_सं == ENOENT)
    परत crun_make_error (err, त्रुटि_सं, "exec container process (missing dynamic library?) `%s`", exec_path);

  परत crun_make_error (err, त्रुटि_सं, "exec container process `%s`", exec_path);
}

स्थिर पूर्णांक
read_container_config_from_state (libcrun_container_t **container, स्थिरांक अक्षर *state_मूळ, स्थिरांक अक्षर *id,
                                  libcrun_error_t *err)
{
  cleanup_free अक्षर *config_file = रिक्त;
  cleanup_free अक्षर *dir = रिक्त;
  पूर्णांक ret;

  *container = रिक्त;

  ret = libcrun_get_state_directory (&dir, state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = append_paths (&config_file, err, dir, "config.json", रिक्त);
  जर (UNLIKELY (ret < 0))
    परत ret;

  *container = libcrun_container_load_from_file (config_file, err);
  जर (*container == रिक्त)
    परत -1;

  परत 0;
}

स्थिर पूर्णांक
run_poststop_hooks (arg_unused libcrun_context_t *context, libcrun_container_t *container,
                    runtime_spec_schema_config_schema *def,
                    libcrun_container_status_t *status, स्थिरांक अक्षर *state_मूळ, स्थिरांक अक्षर *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container_cleanup = रिक्त;
  पूर्णांक ret;

  जर (def == रिक्त)
    {
      जर (container == रिक्त)
        {
          ret = read_container_config_from_state (&container_cleanup, state_मूळ, id, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
          container = container_cleanup;
        }

      def = container->container_def;
    }

  जर (def->hooks && def->hooks->poststop_len)
    {
      cleanup_close पूर्णांक hooks_out_fd = -1;
      cleanup_close पूर्णांक hooks_err_fd = -1;

      जर (container == रिक्त)
        {
          ret = read_container_config_from_state (&container_cleanup, state_मूळ, id, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
          container = container_cleanup;
        }

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      ret = do_hooks (def, 0, id, true, status->bundle, status->bundle, "stopped", "poststop", (hook **) def->hooks->poststop,
                      def->hooks->poststop_len, hooks_out_fd, hooks_err_fd, false, err);
      जर (UNLIKELY (ret != 0))
        {
          जर (ret < 0)
            libcrun_error_report_and_release (err);
          अन्यथा
            libcrun_error (0, "poststop hook failed with exit code: %d", ret);
        }
    }
  परत 0;
}

स्थिर bool
has_new_pid_namespace (runtime_spec_schema_config_schema *def)
{
  माप_प्रकार i;

  जर (def->linux == रिक्त || def->linux->namespaces == रिक्त)
    परत false;

  साठी (i = 0; i < def->linux->namespaces_len; i++)
    {
      जर (म_भेद (def->linux->namespaces[i]->type, "pid") == 0 && def->linux->namespaces[i]->path == रिक्त)
        परत true;
    }
  परत false;
}

/* Delete the container state.  CONTAINER is the container the caller already has
   available, or NULL.  When it is not NULL, the poststop hooks are run even if the
   status file was never written, e.g. when the container creation failed.  */
स्थिर पूर्णांक
container_delete_internal (libcrun_context_t *context, libcrun_container_t *container,
                           runtime_spec_schema_config_schema *def, स्थिरांक अक्षर *id, bool force, bool killall,
                           libcrun_error_t *err)
{
  cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = रिक्त;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_container libcrun_container_t *container_cleanup = रिक्त;
  स्थिरांक अक्षर *state_मूळ = context->state_मूळ;
  पूर्णांक ret;

  ret = libcrun_read_container_status (&status, state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    {
      जर (force && crun_error_get_errno (err) == ENOENT)
        {
          libcrun_error_t tmp_err = रिक्त;

          crun_error_release (err);

          जर (container)
            {
              libcrun_container_status_t partial_status = {
                .bundle = (अक्षर *) context->bundle,
              };

              ret = run_poststop_hooks (context, container, def, &partial_status, state_मूळ, id, err);
              जर (UNLIKELY (ret < 0))
                crun_error_write_warning_and_release (context->output_handler_arg, &err);
            }

          libcrun_container_delete_status (state_मूळ, id, &tmp_err);
          crun_error_release (&tmp_err);
          परत 0;
        }
      crun_error_release (err);
      परत libcrun_container_delete_status (state_मूळ, id, err);
    }

  cgroup_status = libcrun_cgroup_make_status (&status);

  जर (! force)
    {
      ret = libcrun_is_container_running (&status, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
      जर (ret == 1)
        {
          ret = libcrun_status_has_read_exec_fifo (state_मूळ, id, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
          जर (ret == 0)
            परत crun_make_error (err, 0, "the container `%s` is not in `created` or `stopped` state", id);

          /* If the container is in "created" state, then do the equivalent of delete --force.  */
          killall = force = true;
        }
    }

  जर (def == रिक्त)
    {
      जर (container == रिक्त)
        {
          ret = read_container_config_from_state (&container_cleanup, state_मूळ, id, err);
          जर (UNLIKELY (ret < 0))
            परत ret;

          container = container_cleanup;
        }

      def = container->container_def;
    }

  जर (killall && force)
    {
      /* If the container has a pid namespace, it is enough to kill the first
         process (pid=1 in the namespace).
      */
      जर (has_new_pid_namespace (def))
        {
          ret = libcrun_kill_linux (&status, SIGKILL, err);
          जर (UNLIKELY (ret < 0))
            {
              जर (crun_error_get_errno (err) != ESRCH)
                परत ret;

              crun_error_release (err);
            }
        }
      अन्यथा जर (status.cgroup_path)
        {
          ret = libcrun_cgroup_killall (cgroup_status, SIGKILL, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
        }
    }

  जर (def->linux && def->linux->intel_rdt)
    {
      ret = libcrun_destroy_intelrdt (id, def, err);
      जर (UNLIKELY (ret < 0))
        libcrun_error_report_and_release (err);
    }

  जर (status.cgroup_path)
    {
      /* If the cgroup cannot be destroyed, there are still processes in it.
         Keep the container status, it is the only record of the cgroup, so
         that the delete can be retried.  */
      ret = libcrun_cgroup_destroy (cgroup_status, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  ret = run_poststop_hooks (context, container, def, &status, state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  परत libcrun_container_delete_status (state_मूळ, id, err);
}

पूर्णांक
libcrun_container_delete (libcrun_context_t *context, स्थिरांक अक्षर *id, bool force, libcrun_error_t *err)
{
  परत container_delete_internal (context, रिक्त, रिक्त, id, force, true, err);
}

पूर्णांक
libcrun_container_kill (libcrun_context_t *context, स्थिरांक अक्षर *id, स्थिरांक अक्षर *संकेत, libcrun_error_t *err)
{
  पूर्णांक sig, ret;
  स्थिरांक अक्षर *state_मूळ = context->state_मूळ;
  cleanup_container_status libcrun_container_status_t status = {};

  sig = str2sig (संकेत);
  जर (UNLIKELY (sig < 0))
    परत crun_make_error (err, 0, "unknown signal `%s`", संकेत);

  ret = libcrun_read_container_status (&status, state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  परत libcrun_kill_linux (&status, sig, err);
}

पूर्णांक
libcrun_container_killall (libcrun_context_t *context, स्थिरांक अक्षर *id, स्थिरांक अक्षर *संकेत, libcrun_error_t *err)
{
  पूर्णांक sig, ret;
  स्थिरांक अक्षर *state_मूळ = context->state_मूळ;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = रिक्त;

  sig = str2sig (संकेत);
  जर (UNLIKELY (sig < 0))
    परत crun_make_error (err, 0, "unknown signal `%s`", संकेत);

  ret = libcrun_read_container_status (&status, state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  cgroup_status = libcrun_cgroup_make_status (&status);

  ret = libcrun_cgroup_killall (cgroup_status, sig, err);
  जर (UNLIKELY (ret < 0))
    परत ret;
  परत 0;
}

स्थिर पूर्णांक
write_container_status (libcrun_container_t *container, libcrun_context_t *context,
                        pid_t pid, संरचना libcrun_cgroup_status *cgroup_status,
                        libcrun_error_t *err)
{
  cleanup_free अक्षर *bundle_allocated = रिक्त;
  स्थिरांक अक्षर *bundle = get_bundle_path (context->bundle, &bundle_allocated);
  cleanup_free अक्षर *owner = get_user_name (geteuid ());
  अक्षर *external_descriptors = libcrun_get_external_descriptors (container);
  अक्षर *rootfs = container->container_def->मूळ ? container->container_def->मूळ->path : "";
  अक्षर created[35];

  libcrun_container_status_t status = {
    .pid = pid,
    .rootfs = rootfs,
    .bundle = (अक्षर *) bundle,
    .created = created,
    .owner = owner,
    .systemd_cgroup = context->systemd_cgroup,
    .detached = context->detach,
    .no_new_keyring = context->no_new_keyring,
    .external_descriptors = external_descriptors,
    .cgroup_path = रिक्त,
    .scope = रिक्त,
  };

  get_current_timestamp (created, आकार (created));

  जर (cgroup_status)
    {
      पूर्णांक ret;

      ret = libcrun_cgroup_get_status (cgroup_status, &status, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  जर (external_descriptors == रिक्त)
    परत crun_make_error (err, 0, "invalid internal state.  No external descriptors found");
  परत libcrun_write_container_status (context->state_मूळ, context->id, &status, err);
}

स्थिर पूर्णांक
reap_subprocesses (pid_t *main_process, पूर्णांक *main_process_exit, पूर्णांक *last_process, libcrun_error_t *err)
{
  *last_process = 0;
  पर्यंत (1)
    {
      पूर्णांक status;
      पूर्णांक r = waitpid_ignore_stopped (-1, &status, WNOHANG);
      जर (r < 0)
        {
          जर (त्रुटि_सं == EINTR)
            सुरूठेवा;
          जर (त्रुटि_सं == ECHILD)
            {
              *last_process = 1;
              परत 0;
            }
          परत crun_make_error (err, त्रुटि_सं, "waitpid");
        }
      जर (r == 0)
        थांबा;
      जर (r != *main_process)
        सुरूठेवा;

      *main_process_exit = get_process_exit_status (status);

      /* Once reaped, the pid can be reused by an unrelated process.  */
      *main_process = 0;
    }
  परत 0;
}

स्थिर पूर्णांक
send_sd_notify (स्थिरांक अक्षर *ready_str, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  पूर्णांक ret = sd_notify (0, ready_str);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, -ret, "sd_notify");

#  जर HAVE_SD_NOTIFY_BARRIER
  /* Hard-code a 30 seconds timeout.  Ignore errors.  */
  sd_notify_barrier (0, 30 * 1000000);
#  endif
#अन्यथा
  (शून्य) ready_str;
  (शून्य) err;
#endif
  परत 0;
}

स्थिर पूर्णांक
handle_notify_socket (पूर्णांक notify_socketfd, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  पूर्णांक ret;
  अक्षर buf[256];
  स्थिरांक अक्षर *ready_str = "READY=1";

  ret = recvfrom (notify_socketfd, buf, आकार (buf) - 1, 0, रिक्त, रिक्त);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "recvfrom notify socket");

  buf[ret] = '\0';
  जर (म_माला (buf, ready_str))
    {
      ret = send_sd_notify (ready_str, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      परत 1;
    }
  परत 0;
#अन्यथा
  (शून्य) notify_socketfd;
  (शून्य) err;
  परत 1;
#endif
}

संरचना wait_for_process_args
{
  pid_t pid;
  libcrun_context_t *context;
  पूर्णांक terminal_fd;
  पूर्णांक notify_socket;
  पूर्णांक *container_ready_fd;
  पूर्णांक seccomp_notify_fd;
  स्थिरांक अक्षर *seccomp_notify_plugins;
};

स्थिर पूर्णांक
write_pid_file (स्थिरांक अक्षर *pid_file, pid_t pid, libcrun_error_t *err)
{
  अक्षर buf[32];
  पूर्णांक buf_len = snprintf (buf, आकार (buf), "%d", pid);
  जर (UNLIKELY (buf_len >= (पूर्णांक) आकार (buf)))
    परत crun_make_error (err, 0, "internal error: static buffer too small");
  परत write_file_at_with_flags (AT_FDCWD, O_CREAT | O_TRUNC, 0700, pid_file, buf, buf_len, err);
}

#define MAX_EVENTS 10

स्थिर bool
is_fd_pollable (पूर्णांक fd)
{
  संरचना epoll_event ev = { .events = EPOLLIN };
  cleanup_close पूर्णांक epollfd = epoll_create1 (EPOLL_CLOEXEC);

  जर (epollfd < 0)
    परत true;

  परत epoll_ctl (epollfd, EPOLL_CTL_ADD, fd, &ev) == 0 || त्रुटि_सं != EPERM;
}

स्थिर पूर्णांक
wait_for_process (संरचना wait_for_process_args *args, libcrun_error_t *err)
{
  cleanup_channel_fd_pair संरचना channel_fd_pair *from_terminal = रिक्त;
  cleanup_channel_fd_pair संरचना channel_fd_pair *to_terminal = रिक्त;
  पूर्णांक ret, container_exit_code = 0, last_process;
  cleanup_close पूर्णांक terminal_fd_from = -1;
  cleanup_close पूर्णांक terminal_fd_to = -1;
  bool stdin_pollable = false, stdout_pollable = false;
  cleanup_close पूर्णांक epollfd = -1;
  cleanup_close पूर्णांक signalfd = -1;
  sigset_t mask;
  पूर्णांक in_fds[MAX_EVENTS];
  पूर्णांक in_fds_len = 0;
  पूर्णांक out_fds[MAX_EVENTS];
  पूर्णांक out_fds_len = 0;
  माप_प्रकार i;

  cleanup_seccomp_notify_context संरचना seccomp_notify_context_s *seccomp_notify_ctx = रिक्त;

  container_exit_code = 0;

  जर (args == रिक्त || args->context == रिक्त)
    परत crun_make_error (err, 0, "internal error: context is empty");

  साठी (i = 0; i < MAX_EVENTS; i++)
    {
      in_fds[i] = -1;
      out_fds[i] = -1;
    }

  जर (args->context->pid_file)
    {
      ret = write_pid_file (args->context->pid_file, args->pid, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  /* Also exit if there is nothing more to wait for.  */
  जर (args->context->detach && args->notify_socket < 0)
    परत 0;

  जर (args->container_ready_fd)
    {
      ret = 0;
      TEMP_FAILURE_RETRY (लिख (*args->container_ready_fd, &ret, आकार (ret)));
      close_and_reset (args->container_ready_fd);
    }

  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, रिक्त);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "sigprocmask");

  signalfd = create_signalfd (&mask, err);
  जर (UNLIKELY (signalfd < 0))
    परत signalfd;

  ret = reap_subprocesses (&args->pid, &container_exit_code, &last_process, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (last_process)
    परत container_exit_code;

  जर (args->seccomp_notify_fd >= 0)
    {
      cleanup_free अक्षर *state_मूळ = रिक्त;
      cleanup_free अक्षर *oci_config_path = रिक्त;

      संरचना libcrun_load_seccomp_notify_conf_s conf;
      स_रखो (&conf, 0, आकार conf);

      ret = libcrun_get_state_directory (&state_मूळ, args->context->state_मूळ, args->context->id, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      ret = append_paths (&oci_config_path, err, state_मूळ, "config.json", रिक्त);
      जर (UNLIKELY (ret < 0))
        परत ret;

      conf.runtime_मूळ_path = state_मूळ;
      conf.name = args->context->id;
      conf.bundle_path = args->context->bundle;
      conf.oci_config_path = oci_config_path;

      ret = set_blocking_fd (args->seccomp_notify_fd, false, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      ret = libcrun_load_seccomp_notify_plugins (&seccomp_notify_ctx,
                                                 args->seccomp_notify_plugins,
                                                 &conf, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      in_fds[in_fds_len++] = args->seccomp_notify_fd;
    }

  जर (args->terminal_fd >= 0)
    {
      /* The terminal_fd is dup()ed so that it can be registered with
         epoll multiple times using different masks.  */
      terminal_fd_from = dup (args->terminal_fd);
      जर (UNLIKELY (terminal_fd_from < 0))
        परत crun_make_error (err, त्रुटि_सं, "dup terminal fd");
      terminal_fd_to = dup (args->terminal_fd);
      जर (UNLIKELY (terminal_fd_to < 0))
        परत crun_make_error (err, त्रुटि_सं, "dup terminal fd");

      पूर्णांक i, non_blocking_fds[] = { terminal_fd_from, terminal_fd_to, 0, 1, -1 };
      साठी (i = 0; non_blocking_fds[i] >= 0; i++)
        {
          ret = set_blocking_fd (non_blocking_fds[i], false, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
        }

      /* stdin and stdout can be something epoll does not support, such as
         /dev/null or a regular file.  Such a stdin is not forwarded, and
         such a stdout is written to without waiting for it to be ready.  */
      stdin_pollable = is_fd_pollable (0);
      stdout_pollable = is_fd_pollable (1);

      from_terminal = channel_fd_pair_new (terminal_fd_from, 1, बफ_मान);
      जर (stdin_pollable)
        to_terminal = channel_fd_pair_new (0, terminal_fd_to, बफ_मान);
    }

  in_fds[in_fds_len++] = signalfd;
  जर (args->notify_socket >= 0)
    in_fds[in_fds_len++] = args->notify_socket;
  जर (args->terminal_fd >= 0)
    {
      जर (stdin_pollable)
        {
          in_fds[in_fds_len++] = 0;
          out_fds[out_fds_len++] = terminal_fd_to;
        }

      in_fds[in_fds_len++] = terminal_fd_from;
      जर (stdout_pollable)
        out_fds[out_fds_len++] = 1;
    }

  epollfd = epoll_helper (in_fds, रिक्त, out_fds, रिक्त, err);
  जर (UNLIKELY (epollfd < 0))
    परत epollfd;

  पर्यंत (1)
    {
      संरचना epoll_event events[MAX_EVENTS];
      संरचना signalfd_siginfo si;
      संरचना winsize ws;
      पूर्णांक i, nr_events;
      ssize_t res;

      nr_events = TEMP_FAILURE_RETRY (epoll_wait (epollfd, events, MAX_EVENTS, -1));
      जर (UNLIKELY (nr_events < 0))
        परत crun_make_error (err, त्रुटि_सं, "epoll_wait");

      साठी (i = 0; i < nr_events; i++)
        {
          जर (events[i].data.fd == 0 || events[i].data.fd == terminal_fd_to)
            {
              ret = channel_fd_pair_process (to_terminal, epollfd, err);
              जर (UNLIKELY (ret < 0))
                परत crun_error_wrap (err, "copy to terminal fd");
            }
          अन्यथा जर (events[i].data.fd == 1 || events[i].data.fd == terminal_fd_from)
            {
              ret = channel_fd_pair_process (from_terminal, epollfd, err);
              जर (UNLIKELY (ret < 0))
                परत crun_error_wrap (err, "copy from terminal fd");
            }
          अन्यथा जर (events[i].data.fd == args->seccomp_notify_fd)
            {
              ret = libcrun_seccomp_notify_plugins (seccomp_notify_ctx,
                                                    args->seccomp_notify_fd, err);
              जर (UNLIKELY (ret < 0))
                परत ret;
            }
          अन्यथा जर (events[i].data.fd == args->notify_socket)
            {
              ret = handle_notify_socket (args->notify_socket, err);
              जर (UNLIKELY (ret < 0))
                परत ret;
              जर (ret && args->context->detach)
                परत 0;
            }
          अन्यथा जर (events[i].data.fd == signalfd)
            {
              res = TEMP_FAILURE_RETRY (पठ (signalfd, &si, आकार (si)));
              जर (UNLIKELY (res < 0))
                परत crun_make_error (err, त्रुटि_सं, "read from signalfd");
              जर (si.ssi_signo == SIGCHLD)
                {
                  ret = reap_subprocesses (&args->pid, &container_exit_code,
                                           &last_process, err);
                  जर (UNLIKELY (ret < 0))
                    परत ret;
                  जर (last_process)
                    परत container_exit_code;
                }
              अन्यथा जर (si.ssi_signo == SIGWINCH)
                {
                  /* Ignore the signal if the terminal is not available.  */
                  जर (args->terminal_fd > 0)
                    {
                      ret = ioctl (0, TIOCGWINSZ, &ws);
                      जर (UNLIKELY (ret < 0))
                        परत crun_make_error (err, त्रुटि_सं, "ioctl TIOCGWINSZ copy terminal size from stdin");

                      ret = ioctl (args->terminal_fd, TIOCSWINSZ, &ws);
                      जर (UNLIKELY (ret < 0))
                        परत crun_make_error (err, त्रुटि_सं, "ioctl TIOCSWINSZ copy terminal size to pty");
                    }
                }
              अन्यथा जर (args->pid > 0)
                {
                  /* Send any other signal to the child process, if it was not reaped yet.  */
                  ret = समाप्त (args->pid, si.ssi_signo);
                }
            }
          अन्यथा
            {
              परत crun_make_error (err, 0, "internal error: unknown fd from epoll_wait");
            }
        }
    }

  परत 0;
}

स्थिर शून्य
flush_fd_to_err (libcrun_context_t *context, पूर्णांक terminal_fd)
{
  अक्षर buf[256];
  पूर्णांक flags;

  जर (terminal_fd < 0 || मानक_त्रुटि == रिक्त)
    परत;

  flags = fcntl (terminal_fd, F_GETFL, 0);
  जर (flags == -1)
    परत;
  जर (fcntl (terminal_fd, F_SETFL, flags | O_NONBLOCK) < 0)
    परत;

  साठी (;;)
    {
      पूर्णांक ret = TEMP_FAILURE_RETRY (पठ (terminal_fd, buf, आकार (buf) - 1));
      जर (ret <= 0)
        थांबा;
      buf[ret] = '\0';
      जर (context->output_handler)
        context->output_handler (0, buf, LIBCRUN_VERBOSITY_ERROR, context->output_handler_arg);
    }
  (शून्य) fcntl (terminal_fd, F_SETFL, flags);
  ख_साफ (मानक_त्रुटि);
  fsync (1);
  fsync (2);
}

स्थिर पूर्णांक
cleanup_watch (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
               संरचना libcrun_cgroup_status *cgroup_status, pid_t init_pid, पूर्णांक sync_socket,
               पूर्णांक terminal_fd, libcrun_error_t *err)
{
  स्थिरांक अक्षर *oom_message = रिक्त;
  libcrun_error_t tmp_err = रिक्त;
  पूर्णांक ret;

  जर (init_pid)
    {
      /* Try to detect whether the cgroup has a OOM.  */
      जर (cgroup_status)
        {
          पूर्णांक has_oom;

          has_oom = libcrun_cgroup_has_oom (cgroup_status, &tmp_err);
          जर (has_oom > 0)
            oom_message = "OOM: the memory limit could be too low";
          अन्यथा जर (has_oom < 0)
            {
              /* If the detection has failed for any reason, e.g. the cgroup was
                 already deleted by the time it was checked, just ignore the
                 failure.  */
              crun_error_release (&tmp_err);
            }
        }
      /* If the OOM wasn't detected, look into the static configuration.  */
      जर (oom_message == रिक्त && is_memory_limit_too_खालचा (def))
        oom_message = "the memory limit could be too low";

      समाप्त (init_pid, SIGKILL);
      waitpid_ignore_stopped (init_pid, रिक्त, 0);
    }

  ret = sync_socket_wait_sync (context, sync_socket, true, &tmp_err);
  जर (UNLIKELY (ret < 0))
    {
      crun_error_release (err);
      *err = tmp_err;
    }

  जर (terminal_fd >= 0)
    flush_fd_to_err (context, terminal_fd);

  जर (oom_message)
    परत crun_error_wrap (err, "%s", oom_message);

  परत -1;
}

/* Find the uid:gid that is mapped to root inside the container user namespace.  */
शून्य
get_मूळ_in_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid, uid_t *uid,
                        gid_t *gid)
{
  *uid = -1;
  *gid = -1;

  /* Not root in the namespace.  */
  जर (host_uid)
    परत;

  जर (def->linux && def->linux->uid_mappings)
    {
      माप_प्रकार i;

      साठी (i = 0; i < def->linux->uid_mappings_len; i++)
        जर (def->linux->uid_mappings[i]->container_id == 0)
          {
            *uid = def->linux->uid_mappings[i]->host_id;
            थांबा;
          }
    }
  जर (def->linux && def->linux->gid_mappings)
    {
      माप_प्रकार i;

      साठी (i = 0; i < def->linux->gid_mappings_len; i++)
        जर (def->linux->gid_mappings[i]->container_id == 0)
          {
            *gid = def->linux->gid_mappings[i]->host_id;
            थांबा;
          }
    }

  /* If the uid and the gid are not changed, do not attempt any chown.  */
  जर (*uid == host_uid && *gid == host_gid)
    *uid = *gid = -1;
}

स्थिर पूर्णांक
get_seccomp_receiver_fd (libcrun_container_t *container, पूर्णांक *fd, पूर्णांक *self_receiver_fd, स्थिरांक अक्षर **plugins,
                         libcrun_error_t *err)
{
  स्थिरांक अक्षर *tmp;
  runtime_spec_schema_config_schema *def = container->container_def;

  *fd = -1;
  *self_receiver_fd = -1;

  tmp = find_annotation (container, "run.oci.seccomp.plugins");
  जर (tmp)
    {
      पूर्णांक fds[2];
      पूर्णांक ret;

      ret = create_socket_pair (fds, err);
      जर (UNLIKELY (ret < 0))
        परत crun_error_wrap (err, "create socket pair");

      *fd = fds[0];
      *self_receiver_fd = fds[1];
      *plugins = tmp;
      परत 0;
    }

  जर (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    tmp = def->linux->seccomp->listener_path;
  अन्यथा
    tmp = find_annotation (container, "run.oci.seccomp.receiver");
  जर (tmp == रिक्त)
    tmp = दो_पर्या ("RUN_OCI_SECCOMP_RECEIVER");
  जर (tmp)
    {
      जर (tmp[0] != '/')
        परत crun_make_error (err, 0, "the seccomp receiver `%s` is not an absolute path", tmp);

      *fd = open_unix_domain_client_socket (tmp, 0, err);
      जर (UNLIKELY (*fd < 0))
        परत crun_error_wrap (err, "open seccomp receiver");
    }

  परत 0;
}

स्थिर bool
has_seccomp_receiver (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;

  जर (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    परत true;

  जर (find_annotation (container, "run.oci.seccomp.receiver") != रिक्त || दो_पर्या ("RUN_OCI_SECCOMP_RECEIVER") != रिक्त)
    परत true;

  परत false;
}

स्थिर पूर्णांक
setup_container_hooks_output (libcrun_container_t *container, runtime_spec_schema_config_schema *def,
                              संरचना container_entrypoint_s *container_args, पूर्णांक *hooks_out_fd,
                              पूर्णांक *hooks_err_fd, libcrun_error_t *err)
{
  पूर्णांक ret;

  जर (def->hooks
      && (def->hooks->prestart_len || def->hooks->poststart_len || def->hooks->create_runtime_len
          || def->hooks->create_container_len || def->hooks->start_container_len))
    {
      ret = open_hooks_output (container, hooks_out_fd, hooks_err_fd, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
      container_args->hooks_out_fd = *hooks_out_fd;
      container_args->hooks_err_fd = *hooks_err_fd;
    }
  परत 0;
}

स्थिर पूर्णांक
setup_container_keyring (libcrun_container_t *container, libcrun_context_t *context, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णांक ret;

  जर (! context->no_new_keyring)
    {
      स्थिरांक अक्षर *label = रिक्त;

      libcrun_debug ("Creating new keyring");

      जर (def->process)
        {
          label = def->process->selinux_label;
          जर (label)
            libcrun_debug ("Using SELinux process label: `%s`", label);
        }

      ret = libcrun_create_keyring (container, container->context->id, label, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }
  परत 0;
}

स्थिर पूर्णांक
setup_terminal_socket_pair (libcrun_container_t *container, libcrun_context_t *context,
                            संरचना container_entrypoint_s *container_args, पूर्णांक *socket_pair_0,
                            पूर्णांक *socket_pair_1, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णांक ret;
  पूर्णांक detach = context->detach;

  जर (def->process && def->process->terminal && ! detach && context->console_socket == रिक्त)
    {
      libcrun_debug ("Creating terminal socket pair");
      container_args->has_terminal_socket_pair = 1;
      ret = create_socket_pair (container_args->terminal_socketpair, err);
      जर (UNLIKELY (ret < 0))
        परत crun_error_wrap (err, "create terminal socket");

      *socket_pair_0 = container_args->terminal_socketpair[0];
      *socket_pair_1 = container_args->terminal_socketpair[1];
    }
  परत 0;
}

स्थिर पूर्णांक
setup_seccomp (libcrun_container_t *container, स्थिरांक अक्षर *seccomp_bpf_data,
               संरचना libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, पूर्णांक *seccomp_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णांक ret;

  जर (find_annotation (container, "run.oci.seccomp.plugins") != रिक्त && has_seccomp_receiver (container))
    {
      परत crun_make_error (err, 0, "seccomp plugins and seccomp receivers cannot be declared at the same time");
    }

  जर (def->linux && (def->linux->seccomp || seccomp_bpf_data))
    {
      अचिन्हित पूर्णांक seccomp_gen_options = 0;
      स्थिरांक अक्षर *annotation;

      libcrun_debug ("Initializing seccomp");
      annotation = find_annotation (container, "run.oci.seccomp_fail_unknown_syscall");
      जर (annotation && म_भेद (annotation, "0") != 0)
        seccomp_gen_options = LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL;

      जर (seccomp_bpf_data)
        seccomp_gen_options |= LIBCRUN_SECCOMP_SKIP_CACHE;

      libcrun_seccomp_gen_ctx_init (seccomp_gen_ctx, container, true, seccomp_gen_options);

      ret = libcrun_open_seccomp_bpf (seccomp_gen_ctx, seccomp_fd, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }
  परत 0;
}

स्थिर पूर्णांक
setup_console_socket (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
                      संरचना container_entrypoint_s *container_args, पूर्णांक *console_socket_fd,
                      libcrun_error_t *err)
{
  जर (context->console_socket && def->process && def->process->terminal)
    {
      *console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
      जर (UNLIKELY (*console_socket_fd < 0))
        परत crun_error_wrap (err, "open console socket");
      container_args->console_socket_fd = *console_socket_fd;
    }
  परत 0;
}

स्थिर पूर्णांक
setup_cgroup_manager (libcrun_context_t *context, libcrun_container_t *container,
                      संरचना libcrun_cgroup_args *cg, पूर्णांक *cgroup_dirfd,
                      संरचना libcrun_dirfd_s *cgroup_dirfd_s, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णांक cgroup_manager;
  uid_t मूळ_uid = -1;
  gid_t मूळ_gid = -1;
  पूर्णांक ret;

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  जर (context->systemd_cgroup)
    {
      libcrun_debug ("Using systemd cgroup manager");
      cgroup_manager = CGROUP_MANAGER_SYSTEMD;
    }
  अन्यथा जर (context->force_no_cgroup)
    {
      libcrun_debug ("Disabling cgroup manager");
      cgroup_manager = CGROUP_MANAGER_DISABLED;
    }
  अन्यथा
    libcrun_debug ("Using cgroupfs cgroup manager");

  /* If we are root (either on the host or in a namespace), then chown the cgroup to root
     in the container user namespace.  */
  get_मूळ_in_the_userns (def, container->host_uid, container->host_gid, &मूळ_uid, &मूळ_gid);
  libcrun_debug ("Using container host UID `%d` and GID `%d`", container->host_uid, container->host_gid);

  स_रखो (cg, 0, आकार (*cg));

  cg->cgroup_path = def->linux ? def->linux->cgroups_path : "";
  cg->manager = cgroup_manager;
  cg->id = context->id;
  cg->resources = def->linux ? def->linux->resources : रिक्त;
  cg->annotations = container->annotations;
  cg->मूळ_uid = मूळ_uid;
  cg->मूळ_gid = मूळ_gid;
  cg->state_मूळ = context->state_मूळ;
  cg->container = container;

  ret = libcrun_cgroup_preenter (cg, cgroup_dirfd, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  cgroup_dirfd_s->dirfd = cgroup_dirfd;
  cgroup_dirfd_s->joined = false;

  परत 0;
}

स्थिर पूर्णांक
set_scheduler (pid_t pid, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = libcrun_set_scheduler (pid, def->process, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = libcrun_reset_cpu_affinity_mask (pid, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = libcrun_set_io_priority (pid, def->process, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  परत 0;
}

स्थिर पूर्णांक
seccomp_generation (पूर्णांक seccomp_fd, स्थिरांक अक्षर *seccomp_bpf_data,
                    संरचना libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, libcrun_error_t *err)
{
  पूर्णांक ret;

  जर (seccomp_fd >= 0)
    {
      जर (seccomp_bpf_data != रिक्त)
        {
          ret = libcrun_copy_seccomp (seccomp_gen_ctx, seccomp_bpf_data, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
        }
      अन्यथा
        {
          ret = libcrun_generate_seccomp (seccomp_gen_ctx, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
        }
    }
  परत 0;
}

स्थिर पूर्णांक
terminal_setup (runtime_spec_schema_config_schema *def, libcrun_context_t *context,
                पूर्णांक socket_pair_0, पूर्णांक *terminal_fd, शून्य **orig_terminal, libcrun_error_t *err)
{
  पूर्णांक ret;
  पूर्णांक detach = context->detach;

  जर (def->process && def->process->terminal && ! detach && context->console_socket == रिक्त)
    {
      libcrun_debug ("Receiving console socket fd");
      *terminal_fd = receive_fd_from_socket (socket_pair_0, err);
      जर (UNLIKELY (*terminal_fd < 0))
        परत -1;

      ret = libcrun_clear_onlcr (*terminal_fd, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      ret = libcrun_set_raw (0, orig_terminal, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }
  परत 0;
}

स्थिर पूर्णांक
libcrun_container_run_internal (libcrun_container_t *container, libcrun_context_t *context,
                                पूर्णांक *container_ready_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णांक ret;
  pid_t pid;
  पूर्णांक detach = context->detach;
  cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = रिक्त;
  cleanup_close पूर्णांक terminal_fd = -1;
  cleanup_terminal शून्य *orig_terminal = रिक्त;
  cleanup_close पूर्णांक sync_socket = -1;
  cleanup_close पूर्णांक notify_socket = -1;
  cleanup_close पूर्णांक socket_pair_0 = -1;
  cleanup_close पूर्णांक socket_pair_1 = -1;
  cleanup_close पूर्णांक seccomp_fd = -1;
  cleanup_close पूर्णांक console_socket_fd = -1;
  cleanup_close पूर्णांक hooks_out_fd = -1;
  cleanup_close पूर्णांक hooks_err_fd = -1;
  cleanup_close पूर्णांक own_seccomp_receiver_fd = -1;
  cleanup_close पूर्णांक seccomp_notify_fd = -1;
  स्थिरांक अक्षर *seccomp_notify_plugins = रिक्त;
  cleanup_cgroup_preenter संरचना libcrun_cgroup_args cg = {};
  cleanup_free अक्षर *bundle_allocated = रिक्त;
  संरचना container_entrypoint_s container_args = {
    .container = container,
    .context = context,
    .bundle = get_bundle_path (context->bundle, &bundle_allocated),
    .terminal_socketpair = { -1, -1 },
    .console_socket_fd = -1,
    .hooks_out_fd = -1,
    .hooks_err_fd = -1,
    .seccomp_receiver_fd = -1,
    .custom_handler = रिक्त,
  };
  cleanup_close पूर्णांक cgroup_dirfd = -1;
  संरचना libcrun_dirfd_s cgroup_dirfd_s;
  संरचना libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  स्थिरांक अक्षर *seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");
  पूर्णांक cgroup_mode;

  जर (! context->force_no_cgroup)
    {
      cgroup_mode = libcrun_get_cgroup_mode (err);
      जर (UNLIKELY (cgroup_mode < 0))
        परत cgroup_mode;

      जर (cgroup_mode != CGROUP_MODE_UNIFIED)
        libcrun_warning ("cgroup v1 is deprecated and will be removed in a future release.  Use cgroup v2");
    }

  ret = setup_container_hooks_output (container, def, &container_args, &hooks_out_fd, &hooks_err_fd, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  container->context = context;

  जर (! detach || context->notify_socket)
    {
      libcrun_debug ("Setting child subreaper");
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      जर (UNLIKELY (ret < 0))
        परत crun_make_error (err, त्रुटि_सं, "prctl set child subreaper");
    }

  ret = setup_container_keyring (container, context, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = setup_terminal_socket_pair (container, context, &container_args, &socket_pair_0, &socket_pair_1, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = block_signals (err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  umask (0);

  ret = libcrun_set_mempolicy (def, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  जर (UNLIKELY (ret < 0))
    परत ret;
  container_args.seccomp_fd = seccomp_fd;

  जर (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &container_args.seccomp_receiver_fd, &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  ret = setup_console_socket (context, def, &container_args, &console_socket_fd, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = setup_cgroup_manager (context, container, &cg, &cgroup_dirfd, &cgroup_dirfd_s, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = libcrun_configure_handler (container_args.context->handler_manager,
                                   container_args.context,
                                   container,
                                   &(container_args.custom_handler),
                                   err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (container_args.custom_handler && container_args.custom_handler->vtable->modify_oci_configuration)
    {
      libcrun_debug ("Using custom handler to modify OCI configuration");
      ret = container_args.custom_handler->vtable->modify_oci_configuration (container_args.custom_handler->cookie,
                                                                             container_args.context,
                                                                             container->container_def,
                                                                             err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  pid = libcrun_run_linux_container (container, container_init, &container_args, &sync_socket, &cgroup_dirfd_s, err);
  जर (UNLIKELY (pid < 0))
    परत pid;

  cg.pid = pid;
  cg.joined = cgroup_dirfd_s.joined;
  libcrun_debug ("Running container on PID: `%d`", pid);

  जर (context->fifo_exec_wait_fd < 0 && context->notify_socket)
    {
      libcrun_debug ("Using notify socket: `%s`", context->notify_socket);
      /* Do not open the notify socket here on "create".  "start" will take care of it.  */
      ret = get_notify_fd (context, container, &notify_socket, err);
      जर (UNLIKELY (ret < 0))
        जा fail;
    }

  जर (container_args.terminal_socketpair[1] >= 0)
    close_and_reset (&socket_pair_1);

  ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
  जर (UNLIKELY (ret < 0))
    जा fail;

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE, err);
  जर (UNLIKELY (ret < 0))
    जा fail;

  ret = libcrun_move_network_devices (container, pid, err);
  जर (UNLIKELY (ret < 0))
    जा fail;

  /* sync send own pid.  */
  ret = TEMP_FAILURE_RETRY (लिख (sync_socket, &pid, आकार (pid)));
  जर (UNLIKELY (ret != आकार (pid)))
    {
      जर (ret >= 0)
        त्रुटि_सं = 0;
      crun_make_error (err, त्रुटि_सं, "write to sync socket");
      जा fail;
    }

  /* sync 1.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  जर (UNLIKELY (ret < 0))
    जा fail;

  /* sync 2.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  जर (UNLIKELY (ret < 0))
    जा fail;

  ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
  जर (UNLIKELY (ret < 0))
    जा fail;

  /* If the container process is created later, the scheduler is set on it then.  */
  जर (! libcrun_container_delayed_pidns_fork (container))
    {
      ret = set_scheduler (pid, def, err);
      जर (UNLIKELY (ret < 0))
        जा fail;
    }

  /* The container is waiting that we write back.  In this phase we can launch the
     prestart hooks.  */
  जर (def->hooks && def->hooks->prestart_len)
    {
      libcrun_debug ("Running `prestart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "prestart", (hook **) def->hooks->prestart,
                      def->hooks->prestart_len, hooks_out_fd, hooks_err_fd, false, err);
      जर (UNLIKELY (ret != 0))
        जा fail;
    }
  जर (def->hooks && def->hooks->create_runtime_len)
    {
      libcrun_debug ("Running `create` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "createRuntime", (hook **) def->hooks->create_runtime,
                      def->hooks->create_runtime_len, hooks_out_fd, hooks_err_fd, false, err);
      जर (UNLIKELY (ret != 0))
        जा fail;
    }

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  जर (UNLIKELY (ret < 0))
    जा fail;
  close_and_reset (&seccomp_fd);

  /* sync 3.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  जर (UNLIKELY (ret < 0))
    जा fail;

  /* The process joining an existing PID namespace is created once the container root is in place.  */
  जर (libcrun_container_delayed_pidns_fork (container))
    {
      pid_t new_pid = 0;

      ret = sync_socket_wait_sync (context, sync_socket, false, err);
      जर (UNLIKELY (ret < 0))
        जा fail;

      ret = TEMP_FAILURE_RETRY (पठ (sync_socket, &new_pid, आकार (new_pid)));
      जर (UNLIKELY (ret != आकार (new_pid)))
        {
          जर (ret >= 0)
            त्रुटि_सं = 0;
          crun_make_error (err, त्रुटि_सं, "read pid from sync socket");
          जा fail;
        }

      /* Cleanup the first process.  */
      waitpid_ignore_stopped (pid, रिक्त, 0);

      pid = new_pid;

      ret = set_scheduler (pid, def, err);
      जर (UNLIKELY (ret < 0))
        जा fail;

      ret = libcrun_handle_pidfd_receiver (pid, container, err);
      जर (UNLIKELY (ret < 0))
        जा fail;

      /* sync send own pid.  */
      ret = TEMP_FAILURE_RETRY (लिख (sync_socket, &pid, आकार (pid)));
      जर (UNLIKELY (ret != आकार (pid)))
        {
          जर (ret >= 0)
            त्रुटि_सं = 0;
          crun_make_error (err, त्रुटि_सं, "write to sync socket");
          जा fail;
        }
    }

  ret = terminal_setup (def, context, socket_pair_0, &terminal_fd, &orig_terminal, err);
  जर (UNLIKELY (ret < 0))
    जा fail;

  close_and_reset (&socket_pair_0);

  /* sync 4.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  जर (UNLIKELY (ret < 0))
    जा fail;

  ret = close_and_reset (&sync_socket);
  जर (UNLIKELY (ret < 0))
    {
      crun_make_error (err, त्रुटि_सं, "close sync_socket failed");
      जा fail;
    }

  libcrun_debug ("Writing container status");
  ret = write_container_status (container, context, pid, cgroup_status, err);
  जर (UNLIKELY (ret < 0))
    जा fail;

  /* Run poststart hooks here only if the container is created using "run".  For create+start, the
     hooks will be executed as part of the start command.  */
  जर (context->fifo_exec_wait_fd < 0 && def->hooks && def->hooks->poststart_len)
    {
      libcrun_debug ("Running `poststart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      जर (UNLIKELY (ret != 0))
        जा fail;
    }

  /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
  जर (own_seccomp_receiver_fd >= 0)
    {
      libcrun_debug ("Receiving seccomp fd");
      seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
      जर (UNLIKELY (seccomp_notify_fd < 0))
        जा fail;

      ret = close_and_reset (&own_seccomp_receiver_fd);
      जर (UNLIKELY (ret < 0))
        {
          crun_make_error (err, त्रुटि_सं, "close seccomp receiver fd failed");
          जा fail;
        }
    }

  {
    संरचना wait_for_process_args args = {
      .pid = pid,
      .context = context,
      .terminal_fd = terminal_fd,
      .notify_socket = notify_socket,
      .container_ready_fd = container_ready_fd,
      .seccomp_notify_fd = seccomp_notify_fd,
      .seccomp_notify_plugins = seccomp_notify_plugins,
    };
    ret = wait_for_process (&args, err);
  }
  जर (! context->detach)
    {
      libcrun_error_t tmp_err = रिक्त;
      cleanup_watch (context, def, cgroup_status, 0, sync_socket, terminal_fd, &tmp_err);
      crun_error_release (&tmp_err);
    }

  परत ret;

fail:
  ret = cleanup_watch (context, def, cgroup_status, pid, sync_socket, terminal_fd, err);
  जर (cgroup_status)
    {
      libcrun_error_t tmp_err = रिक्त;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  परत ret;
}

स्थिर पूर्णांक
check_config_file (runtime_spec_schema_config_schema *def, libcrun_context_t *context, libcrun_error_t *err)
{
  जर (UNLIKELY (def->linux == रिक्त))
    परत crun_make_error (err, 0, "invalid config file, no `linux` block specified");

  जर (context->handler == रिक्त)
    {
      जर (UNLIKELY (def->मूळ == रिक्त))
        परत crun_make_error (err, 0, "invalid config file, no `root` block specified");
      जर (UNLIKELY (def->mounts == रिक्त))
        परत crun_make_error (err, 0, "invalid config file, no `mounts` block specified");
    }
  परत 0;
}

स्थिर पूर्णांक
libcrun_copy_config_file (स्थिरांक अक्षर *id, स्थिरांक अक्षर *state_मूळ, libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णांक ret;
  cleanup_free अक्षर *dest_path = रिक्त;
  cleanup_free अक्षर *dir = रिक्त;
  cleanup_free अक्षर *buffer = रिक्त;
  माप_प्रकार len;

  ret = libcrun_get_state_directory (&dir, state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = append_paths (&dest_path, err, dir, "config.json", रिक्त);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (container->config_file == रिक्त && container->config_file_content == रिक्त)
    परत crun_make_error (err, 0, "config file not specified");

  जर (container->config_file == रिक्त)
    {
      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, container->config_file_content, म_माप (container->config_file_content), err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }
  अन्यथा
    {
      libcrun_debug ("Reading config file: `%s`", container->config_file);
      ret = read_all_file (container->config_file, &buffer, &len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, buffer, len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  परत 0;
}

स्थिर शून्य
force_delete_container_status (libcrun_context_t *context, libcrun_container_t *container,
                               runtime_spec_schema_config_schema *def)
{
  libcrun_error_t tmp_err = रिक्त;
  container_delete_internal (context, container, def, context->id, true, false, &tmp_err);
  libcrun_error_report_and_release (&tmp_err);
}

/* Serialize ERR to FD as an int errno followed by a NUL-terminated message.
   Used by forked children to report a failure to the parent process.  */
स्थिर शून्य
write_error_to_pipe (पूर्णांक fd, libcrun_error_t *err)
{
  स्थिरांक अक्षर *संदेश = (*err)->संदेश;
  पूर्णांक errcode = crun_error_get_errno (err);

  TEMP_FAILURE_RETRY (लिख (fd, &errcode, आकार (errcode)));
  TEMP_FAILURE_RETRY (लिख (fd, संदेश, म_माप (संदेश) + 1));
}

पूर्णांक
libcrun_container_run (libcrun_context_t *context, libcrun_container_t *container, अचिन्हित पूर्णांक options,
                       libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णांक ret;
  पूर्णांक detach = context->detach;
  पूर्णांक container_ret_status[2];
  cleanup_close पूर्णांक pipefd0 = -1;
  cleanup_close पूर्णांक pipefd1 = -1;
  libcrun_error_t tmp_err = रिक्त;

  container->context = context;

  ret = validate_options (options, LIBCRUN_RUN_OPTIONS_PREFORK | LIBCRUN_RUN_OPTIONS_KEEP, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = check_config_file (def, context, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (def->process && def->process->terminal && detach && context->console_socket == रिक्त)
    परत crun_make_error (err, 0, "use --console-socket with --detach when a terminal is used");

  ret = libcrun_status_check_directories (context->state_मूळ, context->id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (! detach && (options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      ret = libcrun_copy_config_file (context->id, context->state_मूळ, container, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      ret = libcrun_container_run_internal (container, context, रिक्त, err);
      जर (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
        force_delete_container_status (context, container, def);
      परत ret;
    }

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  ret = विभाजन ();
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "fork");
  जर (ret)
    {
      पूर्णांक status;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, रिक्त, 0);

      ret = TEMP_FAILURE_RETRY (पठ (pipefd0, &status, आकार (status)));
      जर (UNLIKELY (ret < 0))
        परत crun_make_error (err, त्रुटि_सं, "read from sync pipe");

      जर (status < 0)
        {
          पूर्णांक errno_;
          अक्षर buf[512];
          ret = TEMP_FAILURE_RETRY (पठ (pipefd0, &errno_, आकार (errno_)));
          जर (UNLIKELY (ret < 0))
            परत crun_make_error (err, त्रुटि_सं, "read from sync pipe");

          ret = TEMP_FAILURE_RETRY (पठ (pipefd0, buf, आकार (buf) - 1));
          जर (UNLIKELY (ret < 0))
            परत crun_make_error (err, त्रुटि_सं, "read from sync pipe");
          buf[ret] = '\0';

          परत crun_make_error (err, errno_, "%s", buf);
        }

      परत status;
    }

  close_and_reset (&pipefd0);

  /* forked process.  */
  ret = detach_process ();
  जर (UNLIKELY (ret < 0))
    libcrun_fail_with_error (त्रुटि_सं, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_मूळ, container, &tmp_err);
  जर (UNLIKELY (ret < 0))
    जा fail;

  ret = libcrun_container_run_internal (container, context, रिक्त, &tmp_err);
  TEMP_FAILURE_RETRY (लिख (pipefd1, &ret, आकार (ret)));
  जर (UNLIKELY (ret < 0))
    जा fail;

  निकास (निकास_सफल);
fail:

  जर (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
    force_delete_container_status (context, container, def);
  जर (tmp_err)
    {
      write_error_to_pipe (pipefd1, &tmp_err);
      crun_error_release (&tmp_err);
    }

  निकास (निकास_त्रुटि);
}

पूर्णांक
libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container, अचिन्हित पूर्णांक options,
                          libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णांक ret;
  पूर्णांक container_ready_pipe[2];
  cleanup_close पूर्णांक pipefd0 = -1;
  cleanup_close पूर्णांक pipefd1 = -1;
  cleanup_close पूर्णांक exec_fifo_fd = -1;
  context->detach = 1;

  libcrun_debug ("Creating container: `%s`", context->id);
  container->context = context;

  ret = validate_options (options, LIBCRUN_CREATE_OPTIONS_PREFORK, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = check_config_file (def, context, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (def->process && def->process->terminal && context->console_socket == रिक्त)
    परत crun_make_error (err, 0, "use --console-socket with create when a terminal is used");

  ret = libcrun_status_check_directories (context->state_मूळ, context->id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  exec_fifo_fd = libcrun_status_create_exec_fifo (context->state_मूळ, context->id, err);
  जर (UNLIKELY (exec_fifo_fd < 0))
    परत exec_fifo_fd;

  context->fifo_exec_wait_fd = exec_fifo_fd;
  exec_fifo_fd = -1;

  जर ((options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      libcrun_debug ("Running with prefork enabled");
      ret = libcrun_copy_config_file (context->id, context->state_मूळ, container, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
      ret = libcrun_container_run_internal (container, context, रिक्त, err);
      जर (UNLIKELY (ret < 0))
        force_delete_container_status (context, container, def);
      परत ret;
    }

  ret = pipe2 (container_ready_pipe, O_CLOEXEC);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "pipe");
  pipefd0 = container_ready_pipe[0];
  pipefd1 = container_ready_pipe[1];

  ret = विभाजन ();
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "fork");
  जर (ret)
    {
      पूर्णांक exit_code;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, रिक्त, 0);

      ret = TEMP_FAILURE_RETRY (पठ (pipefd0, &exit_code, आकार (exit_code)));
      जर (UNLIKELY (ret <= 0))
        परत crun_make_error (err, ret < 0 ? त्रुटि_सं : 0, "waiting for container to be ready");

      जर (ret != आकार (exit_code) || exit_code < 0)
        परत crun_make_error (err, 0, "internal error: read invalid exit code");

      जर (exit_code == 0)
        परत 0;

      libcrun_debug ("Exit code is `%d`, deleting container", exit_code);
      libcrun_error_t tmp_err = रिक्त;
      container_delete_internal (context, रिक्त, def, context->id, true, true, &tmp_err);
      libcrun_error_report_and_release (&tmp_err);
      परत crun_make_error (err, 0, "error creating container");
    }

  /* forked process.  */
  ret = detach_process ();
  जर (UNLIKELY (ret < 0))
    libcrun_fail_with_error (त्रुटि_सं, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_मूळ, container, err);
  जर (UNLIKELY (ret < 0))
    {
      पूर्णांक errcode = crun_error_get_errno (err);
      crun_error_release (err);
      libcrun_fail_with_error (errcode, "copy config file");
    }

  ret = libcrun_container_run_internal (container, context, &pipefd1, err);
  जर (UNLIKELY (ret < 0))
    {
      force_delete_container_status (context, container, def);
      libcrun_error ((*err)->status, "%s", (*err)->संदेश);
      crun_error_release (err);
      crun_set_output_handler (log_write_to_stderr, रिक्त);
    }

  जर (pipefd1 >= 0)
    TEMP_FAILURE_RETRY (लिख (pipefd1, &ret, आकार (ret)));
  निकास (ret ? निकास_त्रुटि : 0);
}

पूर्णांक
libcrun_container_start (libcrun_context_t *context, स्थिरांक अक्षर *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = रिक्त;
  स्थिरांक अक्षर *state_मूळ = context->state_मूळ;
  runtime_spec_schema_config_schema *def;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_close पूर्णांक fd = -1;
  पूर्णांक ret;

  ret = libcrun_read_container_status (&status, state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = libcrun_is_container_running (&status, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (! ret)
    परत crun_make_error (err, 0, "container `%s` is not running", id);

  ret = read_container_config_from_state (&container, state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (context->notify_socket)
    {
      ret = get_notify_fd (context, container, &fd, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  ret = libcrun_status_write_exec_fifo (context->state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  def = container->container_def;

  जर (context->notify_socket)
    {
      जर (fd >= 0)
        {
          fd_set read_set;

          पर्यंत (1)
            {
              संरचना timeval timeout = {
                .tv_sec = 0,
                .tv_usec = 10000,
              };
              FD_ZERO (&read_set);
              FD_SET (fd, &read_set);

              ret = select (fd + 1, &read_set, रिक्त, रिक्त, &timeout);
              जर (UNLIKELY (ret < 0))
                परत crun_make_error (err, त्रुटि_सं, "select failed");
              जर (ret)
                {
                  ret = handle_notify_socket (fd, err);
                  जर (UNLIKELY (ret < 0))
                    परत ret;
                  जर (ret)
                    थांबा;
                }
              अन्यथा
                {
                  ret = libcrun_is_container_running (&status, err);
                  जर (UNLIKELY (ret < 0))
                    परत ret;
                  जर (! ret)
                    परत 0;
                }
            }
        }
    }

  /* The container is considered running only after we got the notification from the
     notify_socket, if any.  */
  जर (def->hooks && def->hooks->poststart_len)
    {
      cleanup_close पूर्णांक hooks_out_fd = -1;
      cleanup_close पूर्णांक hooks_err_fd = -1;

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      ret = do_hooks (def, status.pid, context->id, false, status.bundle, status.bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      जर (UNLIKELY (ret != 0))
        {
          libcrun_error_t tmp_err = रिक्त;
          container_delete_internal (context, container, def, id, true, true, &tmp_err);
          libcrun_error_report_and_release (&tmp_err);
          परत ret;
        }
    }

  परत 0;
}

पूर्णांक
libcrun_get_container_state_string (स्थिरांक अक्षर *id, libcrun_container_status_t *status, स्थिरांक अक्षर *state_मूळ,
                                    स्थिरांक अक्षर **container_status, पूर्णांक *running, libcrun_error_t *err)
{
  पूर्णांक ret, has_fifo = 0;
  bool paused = false;

  ret = libcrun_is_container_running (status, err);
  जर (UNLIKELY (ret < 0))
    परत ret;
  *running = ret;

  जर (*running)
    {
      ret = libcrun_status_has_read_exec_fifo (state_मूळ, id, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
      has_fifo = ret;
    }

  जर (*running && ! has_fifo)
    {
      cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = रिक्त;

      cgroup_status = libcrun_cgroup_make_status (status);

      ret = libcrun_cgroup_is_container_paused (cgroup_status, &paused, err);
      जर (UNLIKELY (ret < 0))
        {
          /*
            The cgroup might have been cleaned up by the time we try to read it, ignore both
            ENOENT and ENODEV:
            - ENOENT: if the open(CGROUP_PATH) fails because the cgroup was deleted.
            - ENODEV: if the cgroup is deleted between the open and reading the freeze status.
          */

          त्रुटि_सं = crun_error_get_errno (err);
          जर (त्रुटि_सं == ENOENT || त्रुटि_सं == ENODEV)
            {
              crun_error_release (err);
              *container_status = "stopped";
              परत 0;
            }

          परत ret;
        }
    }

  जर (! *running)
    *container_status = "stopped";
  अन्यथा जर (has_fifo)
    *container_status = "created";
  अन्यथा जर (paused)
    *container_status = "paused";
  अन्यथा
    *container_status = "running";

  परत 0;
}

पूर्णांक
libcrun_container_state_json (libcrun_context_t *context, स्थिरांक अक्षर *id, अक्षर **out, libcrun_error_t *err)
{
  अक्षर *buffer = रिक्त;
  माप_प्रकार len = 0;
  खाता *f;
  पूर्णांक ret;

  f = open_memstream (&buffer, &len);
  जर (f == रिक्त)
    परत crun_make_error (err, त्रुटि_सं, "open_memstream");

  ret = libcrun_container_state (context, id, f, err);
  ख_बंद (f);
  जर (UNLIKELY (ret < 0))
    {
      मुक्त (buffer);
      परत ret;
    }

  *out = buffer;
  परत 0;
}

पूर्णांक
libcrun_container_state (libcrun_context_t *context, स्थिरांक अक्षर *id, खाता *out, libcrun_error_t *err)
{
  स्थिरांक अक्षर *स्थिरांक OCI_CONFIG_VERSION = "1.0.0";
  libcrun_container_status_t status = {};
  स्थिरांक अक्षर *state_मूळ = context->state_मूळ;
  स्थिरांक अक्षर *container_status = रिक्त;
  json_gen_ctx *gen = रिक्त;
  स्थिरांक अक्षर *buf;
  पूर्णांक ret = 0;
  पूर्णांक r = json_gen_status_ok;
  पूर्णांक running;
  माप_प्रकार len;

  ret = libcrun_read_container_status (&status, state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = libcrun_get_container_state_string (id, &status, state_मूळ, &container_status, &running, err);
  जर (UNLIKELY (ret < 0))
    जा निकास;

  ret = 0;
  जर (! json_gen_init (&gen, रिक्त))
    परत crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));
  GEN_किल्ली (gen, "ociVersion");
  GEN_STR (gen, OCI_CONFIG_VERSION);

  GEN_किल्ली (gen, "id");
  GEN_STR (gen, id);

  GEN_किल्ली (gen, "pid");
  GEN_OR_FAIL (map_int (gen, running ? status.pid : 0));

  GEN_किल्ली (gen, "status");
  GEN_STR (gen, container_status);

  GEN_किल्ली (gen, "bundle");
  GEN_STR (gen, status.bundle);

  GEN_किल्ली (gen, "rootfs");
  GEN_STR (gen, status.rootfs);

  GEN_किल्ली (gen, "created");
  GEN_STR (gen, status.created);

  जर (status.scope)
    {
      GEN_किल्ली (gen, "systemd-scope");
      GEN_STR (gen, status.scope);
    }

  जर (status.owner)
    {
      GEN_किल्ली (gen, "owner");
      GEN_STR (gen, status.owner);
    }

  {
    cleanup_container libcrun_container_t *container = रिक्त;

    ret = read_container_config_from_state (&container, state_मूळ, id, err);
    जर (UNLIKELY (ret < 0))
      जा निकास;

    जर (container->container_def->annotations && container->container_def->annotations->len)
      GEN_OR_FAIL (gen_annotations (gen, container->container_def->annotations));
  }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  ख_लिखो (out, "%s\n", buf);

निकास:
  जर (gen)
    json_gen_free (gen);
  libcrun_free_container_status (&status);
  परत ret;

gen_error:
  ret = json_gen_error_to_crun_error (r, err);
  जा निकास;
}

पूर्णांक
libcrun_container_exec (libcrun_context_t *context, स्थिरांक अक्षर *id, runtime_spec_schema_config_schema_process *process,
                        libcrun_error_t *err)
{
  संरचना libcrun_container_exec_options_s opts;
  स_रखो (&opts, 0, आकार (opts));

  opts.struct_size = आकार (opts);
  opts.process = process;

  परत libcrun_container_exec_with_options (context, id, &opts, err);
}

पूर्णांक
libcrun_container_exec_process_file (libcrun_context_t *context, स्थिरांक अक्षर *id, स्थिरांक अक्षर *path, libcrun_error_t *err)
{
  संरचना libcrun_container_exec_options_s opts;
  स_रखो (&opts, 0, आकार (opts));

  opts.struct_size = आकार (opts);
  opts.path = path;

  परत libcrun_container_exec_with_options (context, id, &opts, err);
}

स्थिर पूर्णांक
exec_from_process_json (libcrun_context_t *context, स्थिरांक अक्षर *id, स्थिरांक अक्षर *process_json, स्थिरांक अक्षर *cgroup,
                        bool merge_env, libcrun_error_t *err)
{
  संरचना parser_context parser_ctx = { .options = 0, .errfile = मानक_त्रुटि };
  संरचना libcrun_container_exec_options_s opts;
  runtime_spec_schema_config_schema_process *process;
  parser_error parser_err = रिक्त;
  json_object *doc = रिक्त;
  पूर्णांक ret;

  ret = parse_json_file (&doc, process_json, &parser_ctx, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  process = make_runtime_spec_schema_config_schema_process (doc, &parser_ctx, &parser_err);
  json_object_put (doc);
  जर (process == रिक्त)
    {
      ret = crun_make_error (err, 0, "cannot parse process: `%s`", parser_err ? parser_err : "unknown error");
      मुक्त (parser_err);
      परत ret;
    }

  स_रखो (&opts, 0, आकार (opts));
  opts.struct_size = आकार (opts);
  opts.process = process;
  opts.cgroup = cgroup;
  opts.merge_env = merge_env;

  ret = libcrun_container_exec_with_options (context, id, &opts, err);
  free_runtime_spec_schema_config_schema_process (process);
  परत ret;
}

पूर्णांक
libcrun_container_exec_json (libcrun_context_t *context, स्थिरांक अक्षर *id, स्थिरांक अक्षर *process_json, libcrun_error_t *err)
{
  परत exec_from_process_json (context, id, process_json, रिक्त, false, err);
}

पूर्णांक
libcrun_container_exec_with_options_json (libcrun_context_t *context, स्थिरांक अक्षर *id,
                                          स्थिरांक संरचना libcrun_exec_options_s *opts, libcrun_error_t *err)
{
  जर (opts == रिक्त || opts->struct_size == 0)
    परत crun_make_error (err, EINVAL, "invalid exec options");

  परत exec_from_process_json (context, id, opts->process_json, opts->cgroup, opts->merge_env, err);
}

#define cleanup_process_schema __attribute__ ((cleanup (cleanup_process_schemap)))

स्थिर ओळीत शून्य
cleanup_process_schemap (runtime_spec_schema_config_schema_process **p)
{
  runtime_spec_schema_config_schema_process *process = *p;
  जर (process)
    (शून्य) free_runtime_spec_schema_config_schema_process (process);
}

/* Build the seccomp receiver payload (when needed), apply the seccomp profile and
   close the seccomp file descriptors.  Used from exec_process_entrypoint both before
   and after the capabilities are set, depending on process->no_new_privileges.  */
स्थिर पूर्णांक
apply_seccomp_for_exec (libcrun_container_t *container, pid_t own_pid, पूर्णांक seccomp_fd,
                        पूर्णांक seccomp_receiver_fd, अक्षर **seccomp_flags, माप_प्रकार seccomp_flags_len,
                        libcrun_error_t *err)
{
  cleanup_free अक्षर *seccomp_fd_payload = रिक्त;
  माप_प्रकार seccomp_fd_payload_len = 0;
  पूर्णांक ret;

  जर (seccomp_receiver_fd >= 0)
    {
      ret = get_seccomp_receiver_fd_payload (container, "running", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  ret = libcrun_apply_seccomp (seccomp_fd, seccomp_receiver_fd, seccomp_fd_payload,
                               seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  close_and_reset (&seccomp_fd);
  close_and_reset (&seccomp_receiver_fd);
  परत 0;
}

स्थिर पूर्णांक
exec_process_entrypoint (libcrun_context_t *context,
                         libcrun_container_t *container,
                         runtime_spec_schema_config_schema_process *process,
                         पूर्णांक *pipefd1,
                         पूर्णांक seccomp_fd,
                         पूर्णांक seccomp_receiver_fd,
                         संरचना custom_handler_instance_s *custom_handler,
                         bool merge_env,
                         libcrun_error_t *err)
{
  runtime_spec_schema_config_schema_process_capabilities *capabilities = रिक्त;
  cleanup_free अक्षर *exec_path = रिक्त;
  uid_t container_uid;
  gid_t container_gid;
  स्थिरांक अक्षर *cwd;
  bool chdir_done = false;
  माप_प्रकार seccomp_flags_len = 0;
  अक्षर **seccomp_flags = रिक्त;
  pid_t own_pid = 0;
  माप_प्रकार i;
  पूर्णांक ret;

  container_uid = process->user ? process->user->uid : 0;
  container_gid = process->user ? process->user->gid : 0;

  TEMP_FAILURE_RETRY (पठ (*pipefd1, &own_pid, आकार (own_pid)));

  cwd = process->cwd ? process->cwd : "/";
  जर (LIKELY (libcrun_safe_chdir (cwd, err) == 0))
    chdir_done = true;
  अन्यथा
    crun_error_release (err);

  ret = unblock_signals (err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = clearenv ();
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "clearenv");

  जर ((merge_env || ! process->env_len) && container->container_def->process && container->container_def->process->env_len)
    {
      अक्षर *e;

      साठी (i = 0; i < container->container_def->process->env_len; i++)
        {
          e = container->container_def->process->env[i];
          जर (putenv (e) < 0)
            परत crun_make_error (err, त्रुटि_सं, "putenv `%s`", e);
        }
    }
  जर (process->env_len)
    {
      साठी (i = 0; i < process->env_len; i++)
        जर (putenv (process->env[i]) < 0)
          परत crun_make_error (err, त्रुटि_सं, "putenv `%s`", process->env[i]);
    }

  जर (is_empty_string (दो_पर्या ("HOME")))
    {
      ret = set_home_env (container_uid);
      जर (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          जर (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            परत crun_make_error (err, त्रुटि_सं, "setenv HOME");
        }
    }

  ret = libcrun_set_selinux_label (container, process, false, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = libcrun_set_apparmor_profile (container, process, false, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (container->container_def->linux && container->container_def->linux->seccomp)
    {
      seccomp_flags = container->container_def->linux->seccomp->flags;
      seccomp_flags_len = container->container_def->linux->seccomp->flags_len;
    }

  ret = find_executable (&exec_path, process->args[0], process->cwd, err);
  जर (UNLIKELY (ret < 0))
    {
      जर (crun_error_get_errno (err) == ENOENT)
        परत ret;

      /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
         once the process switched to the use that runs in the container.  This might be necessary
         when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
         is not honored.  */
      crun_error_release (err);
    }

  जर (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, false, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = libcrun_reopen_dev_null (err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (! process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  ret = libcrun_container_setgroups (container, process, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = maybe_chown_std_streams (container_uid, container_gid, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (process->capabilities)
    capabilities = process->capabilities;
  अन्यथा जर (container->container_def->process)
    capabilities = container->container_def->process->capabilities;

  ret = libcrun_set_caps (capabilities, container_uid, container_gid, process->no_new_privileges,
                          custom_handler != रिक्त, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (UNLIKELY (exec_path == रिक्त))
    {
      ret = find_executable (&exec_path, process->args[0], process->cwd, err);
      जर (UNLIKELY (ret < 0))
        {
          जर (custom_handler == रिक्त || is_empty_string (process->args[0]))
            परत ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          exec_path = xstrdup (process->args[0]);
        }
    }

  जर (! chdir_done)
    {
      ret = libcrun_safe_chdir (cwd, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  जर (process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  जर (process->user)
    umask (process->user->umask_present ? process->user->umask : 0022);

  ret = 0;
  TEMP_FAILURE_RETRY (लिख (*pipefd1, &ret, आकार (ret)));
  TEMP_FAILURE_RETRY (बंद (*pipefd1));
  *pipefd1 = -1;

  जर (custom_handler)
    {
      जर (custom_handler->vtable->exec_func == रिक्त)
        परत crun_make_error (err, 0, "the handler does not support exec");

      जर (context->argv)
        erase_initial_environment (context->argv, context->argc);

      ret = custom_handler->vtable->exec_func (custom_handler->cookie,
                                               container,
                                               exec_path,
                                               process->args);
      जर (UNLIKELY (ret < 0))
        परत crun_make_error (err, -ret, "exec container process failed with handler as `%s`", custom_handler->vtable->name);

      _safe_exit (निकास_त्रुटि);
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation, because the seccomp filter is already in place and it could
     stop some syscalls used by mark_or_close_fds_ge_than.
  */
  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, true, err);
  जर (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, process->args));
  libcrun_fail_with_error (त्रुटि_सं, "exec `%s`", exec_path);
}

/* Close the file descriptors inherited from the caller, other than those
   to be passed to the container process.  */
पूर्णांक
libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err)
{
  परत mark_or_close_fds_ge_than (रिक्त, context->preserve_fds + 3, true, err);
}

पूर्णांक
libcrun_load_process_from_file (स्थिरांक अक्षर *path, runtime_spec_schema_config_schema_process **process,
                                libcrun_error_t *err)
{
  संरचना parser_context ctx = { 0, मानक_त्रुटि };
  cleanup_free अक्षर *content = रिक्त;
  parser_error parser_err = रिक्त;
  json_object *doc = रिक्त;
  माप_प्रकार len;
  पूर्णांक ret;

  ret = read_all_file (path, &content, &len, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  *process = make_runtime_spec_schema_config_schema_process (doc, &ctx, &parser_err);
  जर (UNLIKELY (*process == रिक्त))
    {
      ret = crun_make_error (err, 0, "cannot parse process file: `%s`", parser_err);
      मुक्त (parser_err);
      जर (doc)
        json_object_put (doc);
      परत ret;
    }

  मुक्त (parser_err);
  जर (doc)
    json_object_put (doc);

  परत 0;
}

पूर्णांक
libcrun_container_exec_with_options (libcrun_context_t *context, स्थिरांक अक्षर *id,
                                     संरचना libcrun_container_exec_options_s *opts,
                                     libcrun_error_t *err)
{
  cleanup_custom_handler_instance संरचना custom_handler_instance_s *custom_handler = रिक्त;
  पूर्णांक container_status, ret;
  bool container_paused = false;
  pid_t pid;
  libcrun_container_status_t status = {};
  स्थिरांक अक्षर *state_मूळ = context->state_मूळ;
  cleanup_close पूर्णांक terminal_fd = -1;
  cleanup_close पूर्णांक seccomp_fd = -1;
  cleanup_terminal शून्य *orig_terminal = रिक्त;
  cleanup_container libcrun_container_t *container = रिक्त;
  पूर्णांक container_ret_status[2];
  cleanup_close पूर्णांक pipefd0 = -1;
  cleanup_close पूर्णांक pipefd1 = -1;
  cleanup_close पूर्णांक seccomp_receiver_fd = -1;
  cleanup_close पूर्णांक own_seccomp_receiver_fd = -1;
  cleanup_close पूर्णांक seccomp_notify_fd = -1;
  स्थिरांक अक्षर *seccomp_notify_plugins = रिक्त;
  __attribute__ ((unused)) cleanup_process_schema runtime_spec_schema_config_schema_process *process_cleanup = रिक्त;
  runtime_spec_schema_config_schema_process *process = opts->process;
  संरचना libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  पूर्णांक ret_from_child = 0;

  ret = libcrun_read_container_status (&status, state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = libcrun_is_container_running (&status, err);
  जर (UNLIKELY (ret < 0))
    परत ret;
  container_status = ret;

  ret = read_container_config_from_state (&container, state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  container->context = context;

  जर (container_status == 0)
    परत crun_make_error (err, 0, "the container `%s` is not running", id);

  {
    cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = रिक्त;

    cgroup_status = libcrun_cgroup_make_status (&status);

    ret = libcrun_cgroup_is_container_paused (cgroup_status, &container_paused, err);
    जर (UNLIKELY (ret < 0))
      परत ret;
  }

  जर (UNLIKELY (container_paused))
    परत crun_make_error (err, 0, "cannot exec in a paused container `%s`", id);

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = block_signals (err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  libcrun_seccomp_gen_ctx_init (&seccomp_gen_ctx, container, false, 0);

  ret = libcrun_open_seccomp_bpf (&seccomp_gen_ctx, &seccomp_fd, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &seccomp_receiver_fd,
                                     &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  जर (आकार (*opts) != opts->struct_size)
    परत crun_make_error (err, EINVAL, "invalid libcrun_container_exec_options_s struct");

  जर (opts->path)
    {
      जर (process)
        परत crun_make_error (err, EINVAL, "cannot specify both exec file and options");

      ret = libcrun_load_process_from_file (opts->path, &process, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      process_cleanup = process;
    }

  /* This must be done before we enter a user namespace.  If the process does
     not specify any rlimits, use the ones from the container configuration.  */
  जर (process->rlimits_len == 0 && container->container_def->process)
    ret = libcrun_set_rlimits (container->container_def->process->rlimits,
                               container->container_def->process->rlimits_len, err);
  अन्यथा
    ret = libcrun_set_rlimits (process->rlimits, process->rlimits_len, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = libcrun_set_mempolicy (container->container_def, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  /* If the new process block doesn't specify a SELinux label, AppArmor profile, user,
     I/O priority or scheduler, then use the configuration from the original config file.  */
  जर (container->container_def->process)
    {
      जर (process->io_priority == रिक्त && container->container_def->process->io_priority)
        {
          process->io_priority = clone_runtime_spec_schema_config_schema_process_io_priority (container->container_def->process->io_priority);
          जर (process->io_priority == रिक्त)
            OOM ();
        }

      जर (process->scheduler == रिक्त && container->container_def->process->scheduler)
        {
          process->scheduler = clone_runtime_spec_schema_config_schema_process_scheduler (container->container_def->process->scheduler);
          जर (process->scheduler == रिक्त)
            OOM ();
        }

      जर (process->selinux_label == रिक्त && container->container_def->process->selinux_label)
        process->selinux_label = xstrdup (container->container_def->process->selinux_label);

      जर (process->apparmor_profile == रिक्त && container->container_def->process->apparmor_profile)
        process->apparmor_profile = xstrdup (container->container_def->process->apparmor_profile);

      जर (process->user == रिक्त && container->container_def->process->user)
        {
          process->user = clone_runtime_spec_schema_config_schema_process_user (container->container_def->process->user);
          जर (process->user == रिक्त)
            OOM ();
        }
    }

  ret = initialize_security (container, process, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  /* Do not leak the session keyring of the caller into the container.  The
     keyring created for the container cannot be joined, so use an anonymous one.  */
  जर (! context->no_new_keyring && ! status.no_new_keyring)
    {
      libcrun_debug ("Creating new keyring for the exec process");

      ret = libcrun_create_keyring (container, रिक्त, process->selinux_label, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "prctl unset dumpable");

  pid = libcrun_join_process (context, container, status.pid, &status, opts->cgroup, context->detach,
                              process, process->terminal ? &terminal_fd : रिक्त, err);
  जर (UNLIKELY (pid < 0))
    परत pid;

  /* Process to exec.  */
  जर (pid == 0)
    {
      TEMP_FAILURE_RETRY (बंद (pipefd0));
      pipefd0 = -1;

      exec_process_entrypoint (context, container, process, &pipefd1, seccomp_fd, seccomp_receiver_fd, custom_handler, opts->merge_env, err);
      /* It gets here only on errors.  */
      जर (*err)
        {
          जर (pipefd1 < 0)
            libcrun_fail_with_error ((*err)->status, "%s", (*err)->संदेश);
          अन्यथा
            {
              write_error_to_pipe (pipefd1, err);
              TEMP_FAILURE_RETRY (बंद (pipefd1));
              pipefd1 = -1;
            }
        }
      _safe_exit (निकास_त्रुटि);
    }

  TEMP_FAILURE_RETRY (बंद (pipefd1));
  pipefd1 = -1;

  TEMP_FAILURE_RETRY (लिख (pipefd0, &pid, आकार (pid)));

  जर (seccomp_fd >= 0)
    close_and_reset (&seccomp_fd);

  जर (terminal_fd >= 0)
    {
      अचिन्हित लघु rows = 0, cols = 0;

      जर (process->console_size)
        {
          cols = process->console_size->width;
          rows = process->console_size->height;
        }

      ret = libcrun_terminal_setup_size (terminal_fd, rows, cols, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      जर (context->console_socket)
        {
          पूर्णांक ret;
          cleanup_close पूर्णांक console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
          जर (UNLIKELY (console_socket_fd < 0))
            परत console_socket_fd;
          ret = send_fd_to_socket (console_socket_fd, terminal_fd, err);
          जर (UNLIKELY (ret < 0))
            परत ret;
          close_and_reset (&terminal_fd);
        }
      अन्यथा
        {
          ret = libcrun_clear_onlcr (terminal_fd, err);
          जर (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              परत ret;
            }

          ret = libcrun_set_raw (0, &orig_terminal, err);
          जर (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              परत ret;
            }
        }
    }

  ret = TEMP_FAILURE_RETRY (पठ (pipefd0, &ret_from_child, आकार (ret_from_child)));
  जर (ret != आकार (ret_from_child))
    ret = crun_make_error (err, 0, "read pipe failed");
  अन्यथा जर (ret_from_child != 0)
    {
      cleanup_free अक्षर *संदेश = रिक्त;
      माप_प्रकार len = 0;

      ret = read_all_fd (pipefd0, "error stream", &संदेश, &len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
      /* the string from read_all_fd is always NUL terminated.  */
      ret = crun_make_error (err, ret_from_child, "%s", संदेश);
    }
  अन्यथा
    {
      /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
      जर (own_seccomp_receiver_fd >= 0)
        {
          seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
          जर (UNLIKELY (seccomp_notify_fd < 0))
            परत seccomp_notify_fd;

          ret = close_and_reset (&own_seccomp_receiver_fd);
          जर (UNLIKELY (ret < 0))
            परत crun_make_error (err, त्रुटि_सं, "close seccomp receiver fd failed");
        }

      {
        संरचना wait_for_process_args args = {
          .pid = pid,
          .context = context,
          .terminal_fd = terminal_fd,
          .notify_socket = -1,
          .container_ready_fd = रिक्त,
          .seccomp_notify_fd = seccomp_notify_fd,
          .seccomp_notify_plugins = seccomp_notify_plugins,
        };

        ret = wait_for_process (&args, err);
      }
    }

  flush_fd_to_err (context, terminal_fd);
  परत ret;
}

पूर्णांक
libcrun_container_update (libcrun_context_t *context, स्थिरांक अक्षर *id, स्थिरांक अक्षर *content, माप_प्रकार len arg_unused,
                          libcrun_error_t *err)
{
  cleanup_custom_handler_instance संरचना custom_handler_instance_s *custom_handler = रिक्त;
  runtime_spec_schema_config_linux_resources *resources = रिक्त;
  cleanup_container libcrun_container_t *container = रिक्त;
  स्थिरांक अक्षर *state_मूळ = context->state_मूळ;
  संरचना parser_context ctx = { 0, मानक_त्रुटि };
  libcrun_container_status_t status = {};
  parser_error parser_err = रिक्त;
  json_object *doc = रिक्त;
  पूर्णांक ret;

  ret = libcrun_read_container_status (&status, state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = read_container_config_from_state (&container, state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  resources = make_runtime_spec_schema_config_linux_resources (doc, &ctx, &parser_err);
  जर (UNLIKELY (resources == रिक्त))
    {
      ret = crun_make_error (err, 0, "cannot parse resources: %s", parser_err);
      जा cleanup;
    }

  जर (custom_handler && custom_handler->vtable->modify_oci_configuration)
    {
      /* Adapt RESOURCES to be used from the modify_oci_configuration hook.  */
      cleanup_free runtime_spec_schema_config_linux *linux = xmalloc0 (आकार (*linux));
      cleanup_free runtime_spec_schema_config_schema *def = xmalloc0 (आकार (*def));

      def->linux = linux;
      linux->resources = resources;

      ret = custom_handler->vtable->modify_oci_configuration (custom_handler->cookie,
                                                              context,
                                                              def,
                                                              err);
      जर (UNLIKELY (ret < 0))
        जा cleanup;
    }

  ret = libcrun_linux_container_update (&status, state_मूळ, resources, err);

cleanup:
  जर (doc)
    json_object_put (doc);
  मुक्त (parser_err);
  जर (resources)
    free_runtime_spec_schema_config_linux_resources (resources);

  परत ret;
}

पूर्णांक
libcrun_container_update_from_file (libcrun_context_t *context, स्थिरांक अक्षर *id, स्थिरांक अक्षर *file, libcrun_error_t *err)
{
  cleanup_free अक्षर *content = रिक्त;
  माप_प्रकार len;
  पूर्णांक ret;

  ret = read_all_file (file, &content, &len, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  परत libcrun_container_update (context, id, content, len, err);
}

स्थिर पूर्णांक
compare_update_मूल्ये (स्थिरांक शून्य *a, स्थिरांक शून्य *b)
{
  स्थिरांक संरचना libcrun_update_मूल्य_s *aa = a;
  स्थिरांक संरचना libcrun_update_मूल्य_s *bb = b;
  पूर्णांक ret;

  ret = म_भेद (aa->section, bb->section);
  जर (ret)
    परत ret;
  परत म_भेद (aa->name, bb->name);
}

पूर्णांक
libcrun_container_update_from_मूल्ये (libcrun_context_t *context, स्थिरांक अक्षर *id,
                                      संरचना libcrun_update_मूल्य_s *मूल्ये, माप_प्रकार len,
                                      libcrun_error_t *err)
{
  स्थिरांक अक्षर *current_section = रिक्त;
  स्थिरांक अक्षर *buf;
  json_gen_ctx *gen = रिक्त;
  माप_प्रकार i, buf_len;
  पूर्णांक ret;

  जर (! json_gen_init (&gen, रिक्त))
    परत crun_make_error (err, 0, "json_gen_init failed");
  json_gen_map_open (gen);

  क्विक (मूल्ये, len, आकार (संरचना libcrun_update_मूल्य_s), compare_update_मूल्ये);

  साठी (i = 0; i < len; i++)
    {
      जर (current_section == रिक्त || म_भेद (मूल्ये[i].section, current_section))
        {
          जर (i > 0)
            json_gen_map_close (gen);

          current_section = मूल्ये[i].section;
          json_gen_string (gen, current_section, म_माप (current_section));
          json_gen_map_open (gen);
        }

      json_gen_string (gen, मूल्ये[i].name, म_माप (मूल्ये[i].name));

      जर (मूल्ये[i].numeric)
        json_gen_संख्या (gen, मूल्ये[i].मूल्य, म_माप (मूल्ये[i].मूल्य));
      अन्यथा
        json_gen_string (gen, मूल्ये[i].मूल्य, म_माप (मूल्ये[i].मूल्य));
    }
  जर (len)
    json_gen_map_close (gen);

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);

  ret = libcrun_container_update (context, id, buf, buf_len, err);

  json_gen_free (gen);

  परत ret;
}

स्थिर शून्य
populate_array_field (अक्षर ***field, अक्षर *array[], माप_प्रकार अंक_elements)
{
  माप_प्रकार i;

  *field = xmalloc0 ((अंक_elements + 1) * आकार (अक्षर *));
  साठी (i = 0; i < अंक_elements; i++)
    (*field)[i] = xstrdup (array[i]);

  (*field)[i] = रिक्त;
}

#ifdef HAVE_CAP
स्थिर शून्य
populate_capabilities (संरचना features_info_s *info, अक्षर ***capabilities, माप_प्रकार *अंक_capabilities)
{
  माप_प्रकार index = 0;
  cap_मूल्य_t i;
  अक्षर *endptr;
  पूर्णांक j;

  *अंक_capabilities = 0;
  साठी (i = 0;; i++)
    {
      अक्षर *v = cap_to_name (i);
      जर (v == रिक्त)
        थांबा;
      म_से_दीर्घ (v, &endptr, 10);
      जर (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          थांबा;
        }
      (*अंक_capabilities)++;
    }

  *capabilities = xmalloc0 ((*अंक_capabilities + 1) * आकार (स्थिरांक अक्षर *));
  साठी (i = 0; i < (cap_मूल्य_t) *अंक_capabilities; i++)
    {
      अक्षर *v = cap_to_name (i);
      जर (v == रिक्त)
        थांबा;
      म_से_दीर्घ (v, &endptr, 10);
      जर (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          थांबा;
        }

      // Convert capability name to uppercase
      साठी (j = 0; v[j] != '\0'; j++)
        v[j] = बड़े (v[j]);

      (*capabilities)[index] = v;
      index++;
    }

  (*capabilities)[index] = रिक्त; // Terminate the array with NULL
  populate_array_field (&(info->linux.capabilities), *capabilities, *अंक_capabilities);
}
#endif

स्थिर शून्य
retrieve_mount_options (संरचना features_info_s **info)
{
  cleanup_free स्थिरांक संरचना propagation_flags_s *mount_options_list = रिक्त;
  माप_प्रकार अंक_mount_options = 0;

  // Retrieve mount options from wordlist
  mount_options_list = get_mount_flags_from_wordlist ();

  // Calculate the number of mount options
  पर्यंत (mount_options_list[अंक_mount_options].name != रिक्त)
    अंक_mount_options++;

  // Allocate memory for mount options in info struct
  (*info)->mount_options = xmalloc0 ((अंक_mount_options + 1) * आकार (अक्षर *));

  // Copy mount options to info struct
  साठी (माप_प्रकार i = 0; i < अंक_mount_options; i++)
    (*info)->mount_options[i] = xstrdup (mount_options_list[i].name);
}

पूर्णांक
libcrun_container_get_features (libcrun_context_t *context, संरचना features_info_s **info, libcrun_error_t *err arg_unused)
{
  // Allocate memory for the features_info_s structure
  माप_प्रकार अंक_namspaces = आकार (namespaces) / आकार (namespaces[0]);
  माप_प्रकार अंक_operators = आकार (operators) / आकार (operators[0]);
  माप_प्रकार अंक_actions = आकार (actions) / आकार (actions[0]);
  माप_प्रकार अंक_hooks = आकार (hooks) / आकार (hooks[0]);
  माप_प्रकार अंक_archs = आकार (archs) / आकार (archs[0]);
  माप_प्रकार अंक_unsafe_annotations = आकार (potentially_unsafe_annotations) / आकार (potentially_unsafe_annotations[0]);
  cleanup_free अक्षर **capabilities = रिक्त;
  माप_प्रकार अंक_capabilities = 0;
  माप_प्रकार अंक_mempolicy_modes = आकार (mempolicy_modes) / आकार (mempolicy_modes[0]);
  माप_प्रकार अंक_mempolicy_flags = आकार (mempolicy_flags) / आकार (mempolicy_flags[0]);

  *info = xmalloc0 (आकार (संरचना features_info_s));

  // Hardcoded feature information
  (*info)->oci_version_min = xstrdup ("1.0.0");
  (*info)->oci_version_max = xstrdup ("1.1.0+dev");

  // Populate hooks
  populate_array_field (&((*info)->hooks), hooks, अंक_hooks);

  // Populate mount_options
  retrieve_mount_options (info);

  // Populate namespaces
  populate_array_field (&((*info)->linux.namespaces), namespaces, अंक_namspaces);

#ifdef HAVE_CAP
  // Populate capabilities
  populate_capabilities (*info, &capabilities, &अंक_capabilities);
#endif

  // Hardcode the values for cgroup
  (*info)->linux.cgroup.v1 = true;
  (*info)->linux.cgroup.v2 = true;
#ifdef HAVE_SYSTEMD
  (*info)->linux.cgroup.systemd = true;
  (*info)->linux.cgroup.systemd_user = true;
#endif

  // Put seccomp values
#ifdef HAVE_SECCOMP
  (*info)->linux.seccomp.enabled = true;
  // Populate actions
  populate_array_field (&((*info)->linux.seccomp.actions), actions, अंक_actions);
  // Populate operators
  populate_array_field (&((*info)->linux.seccomp.operators), operators, अंक_operators);
  // Populate archs
  populate_array_field (&((*info)->linux.seccomp.archs), archs, अंक_archs);
#अन्यथा
  (*info)->linux.seccomp.enabled = false;
#endif

  // Put values for apparmor and selinux
  (*info)->linux.apparmor.enabled = true;
  (*info)->linux.selinux.enabled = true;

  (*info)->linux.intel_rdt.enabled = true;

  (*info)->linux.net_devices.enabled = true;

  populate_array_field (&((*info)->linux.memory_policy.mode), mempolicy_modes, अंक_mempolicy_modes);
  populate_array_field (&((*info)->linux.memory_policy.flags), mempolicy_flags, अंक_mempolicy_flags);

  // Put the values for mount extensions
  (*info)->linux.mount_ext.idmap.enabled = true;

  // Populate the values for annotations
#ifdef HAVE_SECCOMP
  {
    स्थिरांक संरचना scmp_version *version = seccomp_version ();
    अक्षर *version_string = रिक्त;

    xasprintf (&version_string, "%u.%u.%u", version->major, version->minor, version->micro);
    (*info)->annotations.io_github_seccomp_libseccomp_version = version_string;
  }
#endif

  जर (context->handler_manager && handler_by_name (context->handler_manager, "wasm"))
    (*info)->annotations.run_oci_crun_wasm = true;

#जर HAVE_CRIU
  (*info)->annotations.run_oci_crun_checkpoint_enabled = true;
#endif
  (*info)->annotations.run_oci_crun_commit = GIT_VERSION;
  (*info)->annotations.run_oci_crun_version = PACKAGE_VERSION;

  populate_array_field (&((*info)->potentially_unsafe_annotations), potentially_unsafe_annotations, अंक_unsafe_annotations);

  परत 0;
}

स्थिर शून्य
features_add_string (json_gen_ctx *json_gen, स्थिरांक अक्षर *किल्ली, स्थिरांक अक्षर *मूल्य)
{
  json_gen_string (json_gen, किल्ली, म_माप (किल्ली));
  json_gen_string (json_gen, मूल्य, म_माप (मूल्य));
}

स्थिर शून्य
features_add_bool (json_gen_ctx *json_gen, स्थिरांक अक्षर *किल्ली, पूर्णांक मूल्य)
{
  json_gen_string (json_gen, किल्ली, म_माप (किल्ली));
  json_gen_bool (json_gen, मूल्य);
}

स्थिर शून्य
features_add_bool_str (json_gen_ctx *json_gen, स्थिरांक अक्षर *किल्ली, पूर्णांक मूल्य)
{
  features_add_string (json_gen, किल्ली, मूल्य ? "true" : "false");
}

स्थिर शून्य
features_add_array (json_gen_ctx *json_gen, स्थिरांक अक्षर *किल्ली, अक्षर **array)
{
  माप_प्रकार i;

  json_gen_string (json_gen, किल्ली, म_माप (किल्ली));
  json_gen_array_open (json_gen);
  जर (array)
    साठी (i = 0; array[i] != रिक्त; i++)
      json_gen_string (json_gen, array[i], म_माप (array[i]));
  json_gen_array_close (json_gen);
}

स्थिर शून्य
features_add_linux_info (json_gen_ctx *json_gen, स्थिरांक संरचना linux_info_s *linux)
{
  json_gen_string (json_gen, "linux", म_माप ("linux"));
  json_gen_map_open (json_gen);

  features_add_array (json_gen, "namespaces", linux->namespaces);
  features_add_array (json_gen, "capabilities", linux->capabilities);

  json_gen_string (json_gen, "cgroup", म_माप ("cgroup"));
  json_gen_map_open (json_gen);
  features_add_bool (json_gen, "v1", linux->cgroup.v1);
  features_add_bool (json_gen, "v2", linux->cgroup.v2);
  features_add_bool (json_gen, "systemd", linux->cgroup.systemd);
  features_add_bool (json_gen, "systemdUser", linux->cgroup.systemd_user);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "seccomp", म_माप ("seccomp"));
  json_gen_map_open (json_gen);
  features_add_bool (json_gen, "enabled", linux->seccomp.enabled);
  जर (linux->seccomp.actions)
    features_add_array (json_gen, "actions", linux->seccomp.actions);
  जर (linux->seccomp.operators)
    features_add_array (json_gen, "operators", linux->seccomp.operators);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "apparmor", म_माप ("apparmor"));
  json_gen_map_open (json_gen);
  features_add_bool (json_gen, "enabled", linux->apparmor.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "selinux", म_माप ("selinux"));
  json_gen_map_open (json_gen);
  features_add_bool (json_gen, "enabled", linux->selinux.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "mountExtensions", म_माप ("mountExtensions"));
  json_gen_map_open (json_gen);
  json_gen_string (json_gen, "idmap", म_माप ("idmap"));
  json_gen_map_open (json_gen);
  features_add_bool (json_gen, "enabled", linux->mount_ext.idmap.enabled);
  json_gen_map_close (json_gen);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "intelRdt", म_माप ("intelRdt"));
  json_gen_map_open (json_gen);
  features_add_bool (json_gen, "enabled", linux->intel_rdt.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "netDevices", म_माप ("netDevices"));
  json_gen_map_open (json_gen);
  features_add_bool (json_gen, "enabled", linux->net_devices.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "memoryPolicy", म_माप ("memoryPolicy"));
  json_gen_map_open (json_gen);
  जर (linux->memory_policy.mode)
    features_add_array (json_gen, "modes", linux->memory_policy.mode);
  जर (linux->memory_policy.flags)
    features_add_array (json_gen, "flags", linux->memory_policy.flags);
  json_gen_map_close (json_gen);

  json_gen_map_close (json_gen);
}

स्थिर शून्य
features_add_annotations_info (json_gen_ctx *json_gen, स्थिरांक संरचना annotations_info_s *annotation)
{
  json_gen_string (json_gen, "annotations", म_माप ("annotations"));
  json_gen_map_open (json_gen);

  जर (! is_empty_string (annotation->io_github_seccomp_libseccomp_version))
    features_add_string (json_gen, "io.github.seccomp.libseccomp.version", annotation->io_github_seccomp_libseccomp_version);

  features_add_bool_str (json_gen, "org.opencontainers.runc.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);
  features_add_bool_str (json_gen, "run.oci.crun.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);

  features_add_string (json_gen, "run.oci.crun.commit", annotation->run_oci_crun_commit);
  features_add_string (json_gen, "run.oci.crun.version", annotation->run_oci_crun_version);

  features_add_bool_str (json_gen, "run.oci.crun.wasm", annotation->run_oci_crun_wasm);

  json_gen_map_close (json_gen);
}

पूर्णांक
libcrun_container_get_features_json (libcrun_context_t *context, अक्षर **out, libcrun_error_t *err)
{
  cleanup_struct_features संरचना features_info_s *info = रिक्त;
  स्थिरांक अक्षर *buffer = रिक्त;
  json_gen_ctx *json_gen;
  माप_प्रकार buffer_len;
  पूर्णांक ret;

  *out = रिक्त;

  ret = libcrun_container_get_features (context, &info, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (! json_gen_init (&json_gen, रिक्त))
    परत crun_make_error (err, 0, "cannot initialize JSON generator");

  json_gen_config (json_gen, json_gen_beautify, 1);

  json_gen_map_open (json_gen);

  features_add_string (json_gen, "ociVersionMin", info->oci_version_min);
  features_add_string (json_gen, "ociVersionMax", info->oci_version_max);
  features_add_array (json_gen, "hooks", info->hooks);
  features_add_array (json_gen, "mountOptions", info->mount_options);
  features_add_linux_info (json_gen, &info->linux);
  features_add_annotations_info (json_gen, &info->annotations);
  features_add_array (json_gen, "potentiallyUnsafeConfigAnnotations", info->potentially_unsafe_annotations);

  json_gen_map_close (json_gen);

  json_gen_get_buf (json_gen, &buffer, &buffer_len);
  *out = xstrdup (buffer);
  json_gen_free (json_gen);

  परत 0;
}

/* Read the container status and fail if the container is not running.  */
स्थिर पूर्णांक
read_status_require_running (libcrun_context_t *context, स्थिरांक अक्षर *id,
                             libcrun_container_status_t *status, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = libcrun_read_container_status (status, context->state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = libcrun_is_container_running (status, err);
  जर (UNLIKELY (ret < 0))
    परत ret;
  जर (ret == 0)
    परत crun_make_error (err, 0, "the container `%s` is not running", id);

  परत 0;
}

पूर्णांक
libcrun_container_pause (libcrun_context_t *context, स्थिरांक अक्षर *id, libcrun_error_t *err)
{
  पूर्णांक ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  परत libcrun_container_pause_linux (&status, err);
}

पूर्णांक
libcrun_container_unpause (libcrun_context_t *context, स्थिरांक अक्षर *id, libcrun_error_t *err)
{
  पूर्णांक ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  परत libcrun_container_unpause_linux (&status, err);
}

/* Translate the public options struct into the internal representation used by
   the linux/CRIU layer.  Strings are borrowed (not copied).  */
स्थिर शून्य
cr_options_from_public (libcrun_checkpoint_restore_t *dst, संरचना libcrun_checkpoint_restore_options_s *src)
{
  स_रखो (dst, 0, आकार (*dst));
  dst->image_path = (अक्षर *) src->image_path;
  dst->work_path = (अक्षर *) src->work_path;
  dst->parent_path = (अक्षर *) src->parent_path;
  dst->console_socket = src->console_socket;
  dst->lsm_profile = (अक्षर *) src->lsm_profile;
  dst->lsm_mount_context = (अक्षर *) src->lsm_mount_context;
  dst->leave_running = src->leave_running;
  dst->tcp_established = src->tcp_established;
  dst->tcp_close = src->tcp_close;
  dst->ext_unix_sk = src->ext_unix_sk;
  dst->shell_job = src->shell_job;
  dst->file_locks = src->file_locks;
  dst->pre_dump = src->pre_dump;
  dst->detach = src->detach;
  dst->manage_cgroups_mode = src->manage_cgroups_mode;
  dst->network_lock_method = src->network_lock_method;
}

पूर्णांक
libcrun_container_checkpoint (libcrun_context_t *context, स्थिरांक अक्षर *id,
                              संरचना libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  पूर्णांक ret;
  स्थिरांक अक्षर *state_मूळ = context->state_मूळ;
  libcrun_container_status_t status = {};
  libcrun_checkpoint_restore_t cr;
  cleanup_container libcrun_container_t *container = रिक्त;

  cr_options_from_public (&cr, cr_options);

  ret = read_status_require_running (context, id, &status, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = read_container_config_from_state (&container, state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  /* Needed to use the same paths (e.g. the shared empty directory used for
     masked paths) as the container runtime and restore do.  */
  container->context = context;

  ret = libcrun_container_checkpoint_linux (&status, container, &cr, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (! (cr_options->leave_running || cr_options->pre_dump))
    परत container_delete_internal (context, रिक्त, रिक्त, id, true, true, err);

  परत 0;
}

/* The dummy process only keeps the cgroup populated until the container is restored there.
   Its cgroup is below the container cgroup, so once the container runs it can freeze or kill
   the dummy process: never depend on it after that point.  */
स्थिर शून्य
restore_proxy_process (पूर्णांक proxy_pid_pipe)
{
  अक्षर c;

  /* Wait to be killed by the parent, or for the parent to go away.  */
  TEMP_FAILURE_RETRY (पठ (proxy_pid_pipe, &c, आकार (c)));
}

स्थिर पूर्णांक
restore_terminate_proxy_process (pid_t proxy_pid, स्थिरांक अक्षर *proxy_cgroup, pid_t container_pid, पूर्णांक cgroup_manager, libcrun_error_t *err)
{
  पूर्णांक mode;
  पूर्णांक ret;

  जर (cgroup_manager == CGROUP_MANAGER_SYSTEMD)
    {
      अक्षर ready_str[64];

      ret = snprintf (ready_str, आकार (ready_str), "MAINPID=%d", container_pid);
      जर (UNLIKELY (ret >= (पूर्णांक) आकार (ready_str)))
        परत crun_make_error (err, 0, "internal error: static buffer too small");

      ret = send_sd_notify (ready_str, err);
      /* Do not fail on errors.  */
      जर (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  mode = libcrun_get_cgroup_mode (err);
  जर (UNLIKELY (mode < 0))
    परत mode;

  /* SIGKILL terminates a process frozen with cgroup v2, while the cgroup v1 freezer does not
     deliver it until the cgroup is thawed, so do not wait for it there.  */
  समाप्त (proxy_pid, SIGKILL);
  TEMP_FAILURE_RETRY (waitpid (proxy_pid, रिक्त, mode == CGROUP_MODE_UNIFIED ? 0 : WNOHANG));

  परत destroy_cgroup_path (proxy_cgroup, mode, err);
}

पूर्णांक
libcrun_container_restore (libcrun_context_t *context, स्थिरांक अक्षर *id arg_unused,
                           संरचना libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = रिक्त;
  cleanup_container libcrun_container_t *container = रिक्त;
  cleanup_close पूर्णांक proxy_pid_pipe0 = -1;
  cleanup_close पूर्णांक proxy_pid_pipe1 = -1;
  runtime_spec_schema_config_schema *def;
  libcrun_container_status_t status = {};
  संरचना libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  libcrun_checkpoint_restore_t cr;
  cleanup_pid pid_t proxy_pid = -1;
  cleanup_close पूर्णांक seccomp_fd = -1;
  स्थिरांक अक्षर *seccomp_bpf_data;
  पूर्णांक proxy_pid_pipe[2];
  पूर्णांक cgroup_manager;
  uid_t मूळ_uid = -1;
  gid_t मूळ_gid = -1;
  पूर्णांक ret;

  cr_options_from_public (&cr, cr_options);

  container = libcrun_container_load_from_file ("config.json", err);
  जर (container == रिक्त)
    परत -1;

  container->context = context;
  def = container->container_def;

  ret = check_config_file (def, context, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = libcrun_status_check_directories (context->state_मूळ, context->id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = libcrun_copy_config_file (context->id, context->state_मूळ, container, err);
  जर (UNLIKELY (ret < 0))
    जा fail;

  /* CRIU restores the seccomp filters of the checkpointed processes, but
     exec loads the filter from the seccomp.bpf file in the state directory,
     so it must be generated again for the restored container.  */
  seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  जर (UNLIKELY (ret < 0))
    जा fail;

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  जर (UNLIKELY (ret < 0))
    जा fail;
  close_and_reset (&seccomp_fd);

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  जर (context->systemd_cgroup)
    cgroup_manager = CGROUP_MANAGER_SYSTEMD;
  अन्यथा जर (context->force_no_cgroup)
    cgroup_manager = CGROUP_MANAGER_DISABLED;

  def = container->container_def;

  /* If we are root (either on the host or in a namespace),
   * then chown the cgroup to root in the container user namespace. */
  get_मूळ_in_the_userns (def, container->host_uid, container->host_gid, &मूळ_uid, &मूळ_gid);

  /* If the root in the container is different than the current root user, attempt to chown
     the std streams before entering the user namespace.  Otherwise we might lose access
     to the user (as it is not mapped in the user namespace) and cannot chown them.  */
  जर (मूळ_uid > 0 || मूळ_gid > 0)
    {
      ret = maybe_chown_std_streams (मूळ_uid, मूळ_gid, err);
      जर (UNLIKELY (ret < 0))
        जा fail;
    }

  /* CRIU runs as a child process, so the restored processes would inherit the session keyring.  */
  ret = setup_container_keyring (container, context, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  {
    cleanup_close पूर्णांक cgroup_dirfd = -1;
    bool needs_proxy_process;
    cleanup_cgroup_preenter संरचना libcrun_cgroup_args cg = {
      .resources = def->linux ? def->linux->resources : रिक्त,
      .annotations = container->annotations,
      .cgroup_path = def->linux ? def->linux->cgroups_path : "",
      .manager = cgroup_manager,
      .मूळ_uid = मूळ_uid,
      .मूळ_gid = मूळ_gid,
      .id = context->id,
      .state_मूळ = context->state_मूळ,
      .container = container,
    };

    /* The CRIU restore code uses bundle, rootfs and cgroup_path of status.   The cgroup_path is set later.  */
    status.bundle = (अक्षर *) context->bundle;
    status.rootfs = def->मूळ->path;

    ret = libcrun_cgroup_preenter (&cg, &cgroup_dirfd, err);
    जर (UNLIKELY (ret < 0))
      जा fail;

    needs_proxy_process = cgroup_dirfd < 0;

    /* If the target cgroup was already created (with cgroupfs), we can restore the container directly there.  */
    जर (! needs_proxy_process)
      {
        cleanup_free अक्षर *cgroup_path = रिक्त;

        ret = get_cgroup_dirfd_path (cgroup_dirfd, &cgroup_path, err);
        जर (UNLIKELY (ret < 0))
          जा fail;

        /* Complete the configuration for the cgroup before the container is
           restored there: CRIU resumes the restored processes before it
           returns, so the device filter and the limits must already be in
           place.  There is no process to move, as CRIU is run in the cgroup,
           and the controllers were enabled by precreate_cgroup.  */
        cg.joined = true;

        ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
        जर (UNLIKELY (ret < 0))
          जा fail;

        /* Restore the container directly in the desired cgroup.  */
        status.cgroup_path = cgroup_path;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        जर (UNLIKELY (ret < 0))
          जा fail;

        /* Now that the container is restored, keep any signal for it, so that
           it is forwarded to the init by wait_for_process, rather than acted
           upon by crun.  This cannot be done earlier, as CRIU, which is run as
           a child process, would inherit the blocked signals.  */
        ret = block_signals (err);
        जर (UNLIKELY (ret < 0))
          जा fail;

        /* Use the container first process PID to finalize the cgroup.  */
        cg.pid = status.pid;
      }
    अन्यथा
      {
        /* When using systemd, we need a process first that must exist before a cgroup is created.
           Create a dummy process that sits and waits until the container is restored.  */
        ret = pipe2 (proxy_pid_pipe, O_CLOEXEC);
        जर (UNLIKELY (ret < 0))
          {
            ret = crun_make_error (err, त्रुटि_सं, "pipe");
            जा fail;
          }

        /* Only used for auto cleanup.  */
        proxy_pid_pipe0 = proxy_pid_pipe[0];
        proxy_pid_pipe1 = proxy_pid_pipe[1];

        proxy_pid = विभाजन ();
        जर (UNLIKELY (proxy_pid < 0))
          {
            ret = crun_make_error (err, त्रुटि_सं, "fork");
            जा fail;
          }

        जर (proxy_pid == 0)
          {
            close_and_reset (&proxy_pid_pipe1);

            restore_proxy_process (proxy_pid_pipe0);
            _safe_exit (निकास_सफल);
          }

        close_and_reset (&proxy_pid_pipe0);

        /* Use the dummy process PID as the PID to setup the cgroup.  */
        cg.pid = proxy_pid;

        /* Complete the configuration for the cgroup.  It contains only the dummy process.  */
        ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
        जर (UNLIKELY (ret < 0))
          जा fail;
      }

    ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
    जर (UNLIKELY (ret < 0))
      जा fail;

    /* When using a dummy process, the container is restored here once the cgroup is configured.  */
    जर (needs_proxy_process)
      {
        cleanup_free अक्षर *target_cgroup = रिक्त;
        cleanup_free अक्षर *proxy_cgroup = रिक्त;

        ret = libcrun_get_cgroup_process (proxy_pid, &target_cgroup, false, err);
        जर (UNLIKELY (ret < 0))
          जा fail;

        /* Move the dummy process to a sub-cgroup.  */
        ret = append_paths (&proxy_cgroup, err, target_cgroup, ".proxy-crun", रिक्त);
        जर (UNLIKELY (ret < 0))
          जा fail;

        ret = libcrun_move_process_to_cgroup (proxy_pid, 0, proxy_cgroup, true, err);
        जर (UNLIKELY (ret < 0))
          जा fail;

        /* Restore the container in the same cgroup where the dummy process was.  */
        status.cgroup_path = target_cgroup;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        जर (UNLIKELY (ret < 0))
          जा fail;

        /* See the comment for the other libcrun_container_restore_linux call.  */
        ret = block_signals (err);
        जर (UNLIKELY (ret < 0))
          जा fail;

        /* The container is running, so do not fail because of the dummy process.  */
        ret = restore_terminate_proxy_process (proxy_pid, proxy_cgroup, status.pid, cgroup_manager, err);
        जर (UNLIKELY (ret < 0))
          crun_error_release (err);
        proxy_pid = -1;

        close_and_reset (&proxy_pid_pipe1);
      }
  }

  context->detach = cr_options->detach;
  ret = write_container_status (container, context, status.pid, cgroup_status, err);
  जर (UNLIKELY (ret < 0))
    जा fail;

  {
    /* Wait the same way run and exec do: apart from waiting for the container
       init, this writes the pid file, forwards any signal to the init, and
       reaps every child, not only the init.  */
    संरचना wait_for_process_args args = {
      .pid = status.pid,
      .context = context,
      .terminal_fd = -1,
      .notify_socket = -1,
      .container_ready_fd = रिक्त,
      .seccomp_notify_fd = -1,
      .seccomp_notify_plugins = रिक्त,
    };

    परत wait_for_process (&args, err);
  }

fail:
  /* Once restored, the container is running.  Without a status file nothing
     can reach it anymore, so do not leave it behind.  */
  जर (status.pid > 0)
    {
      समाप्त (status.pid, SIGKILL);
      waitpid_ignore_stopped (status.pid, रिक्त, 0);
    }
  जर (cgroup_status)
    {
      libcrun_error_t tmp_err = रिक्त;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  {
    libcrun_error_t tmp_err = रिक्त;

    जर (libcrun_container_delete_status (context->state_मूळ, context->id, &tmp_err) < 0)
      crun_error_release (&tmp_err);
  }
  परत ret;
}

पूर्णांक
libcrun_container_read_pids (libcrun_context_t *context, स्थिरांक अक्षर *id, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = रिक्त;
  cleanup_container_status libcrun_container_status_t status = {};
  पूर्णांक ret;

  ret = libcrun_read_container_status (&status, context->state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (status.cgroup_path == रिक्त || status.cgroup_path[0] == '\0')
    परत crun_make_error (err, 0, "the container is not using cgroups");

  cgroup_status = libcrun_cgroup_make_status (&status);

  परत libcrun_cgroup_read_pids (cgroup_status, recurse, pids, err);
}

पूर्णांक
libcrun_container_list_json (libcrun_context_t *context, अक्षर **out, libcrun_error_t *err)
{
  libcrun_container_list_t *list = रिक्त, *it;
  स्थिरांक अक्षर *content = रिक्त;
  json_gen_ctx *gen = रिक्त;
  माप_प्रकार len;
  पूर्णांक ret;

  *out = रिक्त;

  ret = libcrun_get_containers_list (&list, context->state_मूळ, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (! json_gen_init (&gen, रिक्त))
    {
      ret = crun_make_error (err, 0, "cannot allocate json generator");
      जा निकास;
    }

  json_gen_config (gen, json_gen_beautify, 1);
  json_gen_array_open (gen);

  साठी (it = list; it; it = it->next)
    {
      libcrun_container_status_t status;
      पूर्णांक running = 0;
      पूर्णांक pid;
      स्थिरांक अक्षर *container_status = रिक्त;

      ret = libcrun_read_container_status (&status, context->state_मूळ, it->name, err);
      जर (UNLIKELY (ret < 0))
        जा निकास;

      pid = status.pid;

      ret = libcrun_get_container_state_string (it->name, &status, context->state_मूळ, &container_status,
                                                &running, err);
      जर (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          सुरूठेवा;
        }

      जर (! running)
        pid = 0;

      json_gen_map_open (gen);
      json_gen_string (gen, "id", म_माप ("id"));
      json_gen_string (gen, it->name, म_माप (it->name));
      json_gen_string (gen, "pid", म_माप ("pid"));
      map_int (gen, pid);
      json_gen_string (gen, "status", म_माप ("status"));
      json_gen_string (gen, container_status, म_माप (container_status));
      json_gen_string (gen, "bundle", म_माप ("bundle"));
      json_gen_string (gen, status.bundle, म_माप (status.bundle));
      json_gen_string (gen, "created", म_माप ("created"));
      json_gen_string (gen, status.created, म_माप (status.created));
      json_gen_string (gen, "owner", म_माप ("owner"));
      json_gen_string (gen, status.owner, safe_strlen (status.owner));
      json_gen_map_close (gen);

      libcrun_free_container_status (&status);
    }

  json_gen_array_close (gen);
  जर (json_gen_get_buf (gen, &content, &len) != json_gen_status_ok)
    {
      ret = libcrun_make_error (err, 0, "cannot generate json list");
      जा निकास;
    }

  *out = xstrdup (content);
  ret = 0;

निकास:
  जर (list)
    libcrun_free_containers_list (list);
  जर (gen)
    json_gen_free (gen);

  परत ret;
}

पूर्णांक
libcrun_write_json_containers_list (libcrun_context_t *context, खाता *out, libcrun_error_t *err)
{
  cleanup_free अक्षर *content = रिक्त;
  माप_प्रकार len;
  पूर्णांक ret;

  ret = libcrun_container_list_json (context, &content, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  len = म_माप (content);
  जर (len && ख_डालो (content, 1, len, out) != len)
    परत crun_make_error (err, त्रुटि_सं, "error writing to file");

  परत 0;
}

पूर्णांक
libcrun_container_update_intel_rdt (libcrun_context_t *context, स्थिरांक अक्षर *id, संरचना libcrun_intel_rdt_update *update, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = रिक्त;
  पूर्णांक ret;

  ret = read_container_config_from_state (&container, context->state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  परत libcrun_update_intel_rdt (id, container, update->l3_cache_schema, update->mem_bw_schema, update->schemata, err);
}

स्थिर पूर्णांक
libcrun_container_add_or_remove_mounts_from_file (libcrun_context_t *context, स्थिरांक अक्षर *id, स्थिरांक अक्षर *file, bool add, libcrun_error_t *err)
{
  cleanup_custom_handler_instance संरचना custom_handler_instance_s *custom_handler = रिक्त;
  cleanup_container libcrun_container_t *container = रिक्त;
  cleanup_free runtime_spec_schema_defs_mount **mounts = रिक्त;
  स्थिरांक अक्षर *state_मूळ = context->state_मूळ;
  cleanup_free parser_error parser_err = रिक्त;
  संरचना parser_context pctx = { 0, मानक_त्रुटि };
  libcrun_container_status_t status = {};
  cleanup_free अक्षर *content = रिक्त;
  json_object *doc = रिक्त;
  माप_प्रकार n_mounts = 0, len, i;
  पूर्णांक ret = 1;

  ret = read_all_file (file, &content, &len, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = libcrun_read_container_status (&status, state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = read_container_config_from_state (&container, state_मूळ, id, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = parse_json_file (&doc, content, &pctx, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (! json_object_is_type (doc, json_type_array))
    {
      ret = crun_make_error (err, 0, "mounts must be an array");
      जा cleanup;
    }
  अन्यथा
    {
      n_mounts = json_object_array_length (doc);

      mounts = xmalloc0 ((n_mounts + 1) * आकार (*mounts));
      साठी (i = 0; i < n_mounts; i++)
        {
          mounts[i] = make_runtime_spec_schema_defs_mount (json_object_array_get_idx (doc, i), &pctx, &parser_err);
          जर (mounts[i] == रिक्त)
            {
              ret = crun_make_error (err, 0, "cannot parse mount: %s", parser_err);
              जा cleanup;
            }
        }
    }

  जर (add)
    ret = libcrun_make_runtime_mounts (container, &status, mounts, n_mounts, err);
  अन्यथा
    ret = libcrun_destroy_runtime_mounts (container, &status, mounts, n_mounts, err);

cleanup:
  जर (doc)
    json_object_put (doc);
  जर (mounts)
    साठी (i = 0; i < n_mounts; i++)
      free_runtime_spec_schema_defs_mount (mounts[i]);
  परत ret;
}

पूर्णांक
libcrun_container_add_mounts_from_file (libcrun_context_t *context, स्थिरांक अक्षर *id, स्थिरांक अक्षर *file, libcrun_error_t *err)
{
  परत libcrun_container_add_or_remove_mounts_from_file (context, id, file, true, err);
}

पूर्णांक
libcrun_container_remove_mounts_from_file (libcrun_context_t *context, स्थिरांक अक्षर *id, स्थिरांक अक्षर *file, libcrun_error_t *err)
{
  परत libcrun_container_add_or_remove_mounts_from_file (context, id, file, false, err);
}
