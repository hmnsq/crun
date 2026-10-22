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
  पूर्णाङ्क has_terminal_socket_pair;
  पूर्णाङ्क terminal_socketpair[2];

  /* Used by log_write_to_sync_socket.  */
  पूर्णाङ्क sync_socket;

  पूर्णाङ्क seccomp_fd;
  पूर्णाङ्क seccomp_receiver_fd;
  पूर्णाङ्क console_socket_fd;

  पूर्णाङ्क hooks_out_fd;
  पूर्णाङ्क hooks_err_fd;

  /* Absolute bundle path, to be reported to hooks run in the container.  */
  ध्रुव वर्ण *bundle;

  संरचना custom_handler_instance_s *custom_handler;
};

संरचना sync_socket_message_s
{
  पूर्णाङ्क type;
  पूर्णाङ्क error_मानम्;
  वर्ण message[512];
};

प्रकारनिर्देश runtime_spec_schema_defs_hook hook;

// linux hooks
वर्ण *hooks[] = {
  "prestart",
  "createRuntime",
  "createContainer",
  "startContainer",
  "poststart",
  "poststop"
};

// linux namespaces
स्थिर वर्ण *namespaces[] = {
  "cgroup",
  "ipc",
  "mount",
  "network",
  "pid",
  "user",
  "uts"
};

स्थिर वर्ण *actions[] = {
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

स्थिर वर्ण *operators[] = {
  "SCMP_CMP_NE",
  "SCMP_CMP_LT",
  "SCMP_CMP_LE",
  "SCMP_CMP_EQ",
  "SCMP_CMP_GE",
  "SCMP_CMP_GT",
  "SCMP_CMP_MASKED_EQ",
};

स्थिर वर्ण *archs[] = {
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

स्थिर वर्ण *mempolicy_modes[] = {
  "MPOL_DEFAULT",
  "MPOL_PREFERRED",
  "MPOL_BIND",
  "MPOL_INTERLEAVE",
  "MPOL_LOCAL",
  "MPOL_PREFERRED_MANY",
  "MPOL_WEIGHTED_INTERLEAVE"
};

स्थिर वर्ण *mempolicy_flags[] = {
  "MPOL_F_NUMA_BALANCING",
  "MPOL_F_RELATIVE_NODES",
  "MPOL_F_STATIC_NODES"
};

स्थिर वर्ण *potentially_unsafe_annotations[] = {
  "module.wasm.image/variant",
  "io.kubernetes.cri.container-type",
  "run.oci.",
  "org.criu.",
  "krun.",
};

#define SYNC_SOCKET_MESSAGE_LEN(x, l) (offsetof (संरचना sync_socket_message_s, message) + l)

स्थिर पूर्णाङ्क
sync_socket_write_सन्देशः (पूर्णाङ्क fd, पूर्णाङ्क verbosity, पूर्णाङ्क err_मानम्, ध्रुव वर्ण *log_सन्देशः)
{
  पूर्णाङ्क ret;
  size_t err_len;
  संरचना sync_socket_message_s सन्देशः;
  विकल्प (verbosity)
    {
    पक्ष LIBCRUN_VERBOSITY_DEBUG:
      सन्देशः.type = SYNC_SOCKET_DEBUG_MESSAGE;
      भङ्ग;
    पक्ष LIBCRUN_VERBOSITY_WARNING:
      सन्देशः.type = SYNC_SOCKET_WARNING_MESSAGE;
      भङ्ग;
    पक्ष LIBCRUN_VERBOSITY_ERROR:
      सन्देशः.type = SYNC_SOCKET_ERROR_MESSAGE;
      भङ्ग;
    }
  सन्देशः.error_मानम् = err_मानम्;

  सचे (fd < 0)
    पटिनिवत्त 0;

  err_len = strlen (log_सन्देशः);
  सचे (err_len >= परिमाण (सन्देशः.message))
    err_len = परिमाण (सन्देशः.message) - 1;

  memcpy (सन्देशः.message, log_सन्देशः, err_len);
  सन्देशः.message[err_len] = '\0';

  ret = TEMP_FAILURE_RETRY (लिख (fd, &सन्देशः, SYNC_SOCKET_MESSAGE_LEN (सन्देशः, err_len + 1)));
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त -1;

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
sync_socket_write_error (पूर्णाङ्क fd, libcrun_error_t *out_err)
{
  सचे (fd < 0)
    पटिनिवत्त 0;
  पटिनिवत्त sync_socket_write_सन्देशः (fd, false, (*out_err)->status, (*out_err)->सन्देशः);
}

स्थिर शून्य
log_write_to_sync_socket (पूर्णाङ्क errno_, ध्रुव वर्ण *सन्देशः, पूर्णाङ्क verbosity, शून्य *arg)
{
  संरचना container_entrypoint_s *entrypoint_args = arg;
  पूर्णाङ्क fd = entrypoint_args->sync_socket;

  सचे (fd < 0)
    पटिनिवत्त;

  सचे (sync_socket_write_सन्देशः (fd, verbosity, errno_, सन्देशः) < 0)
    log_write_to_stderr (errno_, सन्देशः, verbosity, arg);
}

स्थिर bool
is_memory_limit_too_नीचः (runtime_spec_schema_config_schema *def)
{
  ध्रुव long memory_limit_too_नीचः = 1024 * 1024;

  सचे (def->linux == रिक्त || def->linux->resources == रिक्त)
    पटिनिवत्त false;

  सचे (def->linux->resources->memory
      && def->linux->resources->memory->limit_present
      && def->linux->resources->memory->limit < memory_limit_too_नीचः)
    पटिनिवत्त true;

  सचे (def->linux->resources->unified)
    {
      size_t i;

      अत्थाय (i = 0; i < def->linux->resources->unified->len; i++)
        सचे (strcmp (def->linux->resources->unified->कुञ्चिकाः[i], "memory.max") == 0)
          {
            long limit;

            errno = 0;
            limit = strtol (def->linux->resources->unified->मानानि[i], रिक्त, 10);
            सचे (errno != 0)
              पटिनिवत्त false;
            सचे (limit < memory_limit_too_नीचः)
              पटिनिवत्त true;
          }
    }

  पटिनिवत्त false;
}

स्थिर पूर्णाङ्क
sync_socket_wait_sync (libcrun_context_t *context, पूर्णाङ्क fd, bool flush, libcrun_error_t *err)
{
  संरचना sync_socket_message_s सन्देशः;

  सचे (fd < 0)
    पटिनिवत्त 0;

  याव (true)
    {
      पूर्णाङ्क ret;

      ret = TEMP_FAILURE_RETRY (पठ (fd, &सन्देशः, परिमाण (सन्देशः)));
      सचे (UNLIKELY (ret < 0))
        {
          सचे (flush)
            पटिनिवत्त 0;
          पटिनिवत्त crun_make_error (err, errno, "read from sync socket");
        }

      सचे (ret == 0)
        {
          सचे (flush)
            पटिनिवत्त 0;

          पटिनिवत्त crun_make_error (err, 0, "read from the init process");
        }

      सचे (! flush && सन्देशः.type == SYNC_SOCKET_SYNC_MESSAGE)
        पटिनिवत्त 0;
      अञ्ञथा सचे (सन्देशः.type == SYNC_SOCKET_DEBUG_MESSAGE)
        {
          सचे (context)
            context->output_handler (सन्देशः.error_मानम्, सन्देशः.message, LIBCRUN_VERBOSITY_DEBUG, context->output_handler_arg);
          अनुवर्त;
        }
      अञ्ञथा सचे (सन्देशः.type == SYNC_SOCKET_WARNING_MESSAGE)
        {
          सचे (context)
            context->output_handler (सन्देशः.error_मानम्, सन्देशः.message, LIBCRUN_VERBOSITY_WARNING, context->output_handler_arg);
          अनुवर्त;
        }
      अञ्ञथा सचे (सन्देशः.type == SYNC_SOCKET_ERROR_MESSAGE)
        पटिनिवत्त crun_make_error (err, सन्देशः.error_मानम्, "%s", सन्देशः.message);
    }
}

स्थिर पूर्णाङ्क
sync_socket_send_sync (पूर्णाङ्क fd, bool flush_errors, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  संरचना sync_socket_message_s सन्देशः = {
    0,
  };
  सन्देशः.type = SYNC_SOCKET_SYNC_MESSAGE;

  सचे (fd < 0)
    पटिनिवत्त 0;

  ret = TEMP_FAILURE_RETRY (लिख (fd, &सन्देशः, SYNC_SOCKET_MESSAGE_LEN (सन्देशः, 0)));
  सचे (UNLIKELY (ret < 0))
    {
      सचे (flush_errors)
        {
          पूर्णाङ्क saved_errno = errno;
          ret = TEMP_FAILURE_RETRY (पठ (fd, &सन्देशः, परिमाण (सन्देशः)));
          सचे (ret >= 0 && सन्देशः.type == SYNC_SOCKET_ERROR_MESSAGE)
            पटिनिवत्त crun_make_error (err, सन्देशः.error_मानम्, "%s", सन्देशः.message);
          errno = saved_errno;
        }
      पटिनिवत्त crun_make_error (err, errno, "write to sync socket");
    }

  पटिनिवत्त 0;
}

स्थिर bool
has_cap_sys_ptrace ()
{
#ifdef HAVE_CAP
  संरचना __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  संरचना __user_cap_data_struct data[2] = { { 0 } };

  सचे (UNLIKELY (capget (&hdr, data) < 0))
    पटिनिवत्त true;

  पटिनिवत्त (data[0].effective & (1U << CAP_SYS_PTRACE)) != 0;
#अञ्ञथा
  पटिनिवत्त geteuid () == 0;
#endif
}

स्थिर libcrun_container_t *
make_container (runtime_spec_schema_config_schema *container_def, ध्रुव वर्ण *path, ध्रुव वर्ण *config)
{
  libcrun_container_t *container = xmalloc0 (परिमाण (*container));
  container->container_def = container_def;

  container->host_uid = geteuid ();
  container->host_gid = getegid ();
  container->host_has_cap_sys_ptrace = has_cap_sys_ptrace ();
  container->proc_fd = -1;

  container->annotations = make_string_map_from_json (container_def->annotations);

  /* An empty namespace path means that a new namespace is to be created,
     the same as if it is not specified.  */
  सचे (container_def->linux)
    {
      size_t i;

      अत्थाय (i = 0; i < container_def->linux->namespaces_len; i++)
        सचे (container_def->linux->namespaces[i]->path && container_def->linux->namespaces[i]->path[0] == '\0')
          {
            मुक्त (container_def->linux->namespaces[i]->path);
            container_def->linux->namespaces[i]->path = रिक्त;
          }
    }

  सचे (path)
    container->config_file = xstrdup (path);
  सचे (config)
    container->config_file_content = xstrdup (config);

  पटिनिवत्त container;
}

libcrun_container_t *
libcrun_container_load_from_memory (ध्रुव वर्ण *json, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free वर्ण *oci_error = रिक्त;
  container_def = runtime_spec_schema_config_schema_parse_data (json, रिक्त, &oci_error);
  सचे (container_def == रिक्त)
    {
      crun_make_error (err, 0, "load: `%s`", oci_error);
      पटिनिवत्त रिक्त;
    }
  पटिनिवत्त make_container (container_def, रिक्त, json);
}

libcrun_container_t *
libcrun_container_load_from_file (ध्रुव वर्ण *path, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free वर्ण *oci_error = रिक्त;
  cleanup_free वर्ण *content = रिक्त;
  size_t len;
  पूर्णाङ्क ret;

  libcrun_debug ("Loading container from config file: `%s`", path);
  ret = read_all_file (path, &content, &len, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त रिक्त;

  container_def = runtime_spec_schema_config_schema_parse_data (content, रिक्त, &oci_error);
  सचे (container_def == रिक्त)
    {
      crun_make_error (err, 0, "load `%s`: %s", path, oci_error);
      पटिनिवत्त रिक्त;
    }
  पटिनिवत्त make_container (container_def, रिक्त, content);
}

ध्रुव वर्ण *
libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err)
{
  संरचना parser_context ctx = { 0, stderr };
  parser_error gen_err = रिक्त;

  सचे (container == रिक्त)
    {
      crun_make_error (err, EINVAL, "no container specified");
      पटिनिवत्त रिक्त;
    }

  सचे (container->config_file_content == रिक्त && container->container_def)
    {
      container->config_file_content = runtime_spec_schema_config_schema_generate_json (container->container_def, &ctx, &gen_err);
      सचे (container->config_file_content == रिक्त)
        {
          crun_make_error (err, 0, "cannot generate the configuration JSON: %s", gen_err ?: "unknown error");
          मुक्त (gen_err);
          पटिनिवत्त रिक्त;
        }
      मुक्त (gen_err);
    }

  सचे (container->config_file_content == रिक्त)
    {
      crun_make_error (err, 0, "the container has no configuration");
      पटिनिवत्त रिक्त;
    }

  पटिनिवत्त container->config_file_content;
}

ध्रुव वर्ण *
libcrun_container_get_annotation (libcrun_container_t *container, ध्रुव वर्ण *कुञ्चिका)
{
  सचे (container == रिक्त)
    पटिनिवत्त रिक्त;
  पटिनिवत्त find_string_map_मानम् (container->annotations, कुञ्चिका);
}

size_t
libcrun_container_get_annotations_len (libcrun_container_t *container)
{
  सचे (container == रिक्त || container->annotations == रिक्त)
    पटिनिवत्त 0;
  पटिनिवत्त string_map_size (container->annotations);
}

पूर्णाङ्क
libcrun_container_get_annotation_at (libcrun_container_t *container, size_t index, ध्रुव वर्ण **कुञ्चिका,
                                     ध्रुव वर्ण **मानम्)
{
  सचे (container == रिक्त)
    {
      errno = ERANGE;
      पटिनिवत्त -1;
    }
  पटिनिवत्त string_map_get_at (container->annotations, index, कुञ्चिका, मानम्);
}

uid_t
libcrun_container_get_uid (libcrun_container_t *container)
{
  सचे (container == रिक्त)
    पटिनिवत्त (uid_t) -1;
  पटिनिवत्त container->host_uid;
}

gid_t
libcrun_container_get_gid (libcrun_container_t *container)
{
  सचे (container == रिक्त)
    पटिनिवत्त (gid_t) -1;
  पटिनिवत्त container->host_gid;
}

शून्य
libcrun_container_free (libcrun_container_t *ctr)
{
  सचे (ctr == रिक्त)
    पटिनिवत्त;

  सचे (ctr->cleanup_private_data)
    ctr->cleanup_private_data (ctr->private_data);

  सचे (ctr->container_def)
    free_runtime_spec_schema_config_schema (ctr->container_def);

  free_string_map (ctr->annotations);

  सचे (ctr->proc_fd >= 0)
    close (ctr->proc_fd);

  मुक्त (ctr->config_file_content);
  मुक्त (ctr->config_file);
  मुक्त (ctr);
}

libcrun_context_t *
libcrun_context_new (ध्रुव वर्ण *id, ध्रुव वर्ण *state_मूलम्, libcrun_error_t *err)
{
  libcrun_context_t *ctx = xmalloc0 (परिमाण (*ctx));

  /* Go through the setters so that the owned_* copies and the borrowed
     pointers stay in sync and are released by libcrun_context_free.  */
  libcrun_context_set_id (ctx, id);
  libcrun_context_set_state_मूलम् (ctx, state_मूलम्);
  libcrun_context_set_bundle (ctx, ".");
  ctx->fifo_exec_wait_fd = -1;

  सचे (libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, id, रिक्त, err) < 0)
    {
      libcrun_context_free (ctx);
      पटिनिवत्त रिक्त;
    }

  ctx->handler_manager = libcrun_handler_manager_create (err);
  सचे (ctx->handler_manager == रिक्त)
    {
      libcrun_context_free (ctx);
      पटिनिवत्त रिक्त;
    }

#ifdef CRUN_LIBDIR
  सचे (access (CRUN_LIBDIR "/handlers", F_OK) == 0)
    {
      सचे (libcrun_handler_manager_load_directory (ctx->handler_manager, CRUN_LIBDIR "/handlers", err) < 0)
        {
          libcrun_context_free (ctx);
          पटिनिवत्त रिक्त;
        }
    }
#endif

  पटिनिवत्त ctx;
}

शून्य
libcrun_context_free (libcrun_context_t *ctx)
{
  सचे (ctx == रिक्त)
    पटिनिवत्त;

  मुक्त (ctx->owned_state_मूलम्);
  मुक्त (ctx->owned_id);
  मुक्त (ctx->owned_bundle);
  मुक्त (ctx->owned_console_socket);
  मुक्त (ctx->owned_pid_file);
  मुक्त (ctx->owned_notify_socket);
  मुक्त (ctx->owned_handler);

  सचे (ctx->handler_manager)
    handler_manager_free (ctx->handler_manager);

  मुक्त (ctx);
}

#define LIBCRUN_CONTEXT_SET_STRING(field)                  \
  करोहि                                                       \
    {                                                      \
      मुक्त (ctx->owned_##field);                           \
      ctx->owned_##field = मानम् ? xstrdup (मानम्) : रिक्त; \
      ctx->field = ctx->owned_##field;                     \
  } याव (0)

शून्य
libcrun_context_set_id (libcrun_context_t *ctx, ध्रुव वर्ण *मानम्)
{
  LIBCRUN_CONTEXT_SET_STRING (id);
}

शून्य
libcrun_context_set_state_मूलम् (libcrun_context_t *ctx, ध्रुव वर्ण *मानम्)
{
  LIBCRUN_CONTEXT_SET_STRING (state_मूलम्);
}

शून्य
libcrun_context_set_bundle (libcrun_context_t *ctx, ध्रुव वर्ण *मानम्)
{
  LIBCRUN_CONTEXT_SET_STRING (bundle);
}

शून्य
libcrun_context_set_console_socket (libcrun_context_t *ctx, ध्रुव वर्ण *मानम्)
{
  LIBCRUN_CONTEXT_SET_STRING (console_socket);
}

शून्य
libcrun_context_set_pid_file (libcrun_context_t *ctx, ध्रुव वर्ण *मानम्)
{
  LIBCRUN_CONTEXT_SET_STRING (pid_file);
}

शून्य
libcrun_context_set_notify_socket (libcrun_context_t *ctx, ध्रुव वर्ण *मानम्)
{
  LIBCRUN_CONTEXT_SET_STRING (notify_socket);
}

शून्य
libcrun_context_set_handler (libcrun_context_t *ctx, ध्रुव वर्ण *मानम्)
{
  LIBCRUN_CONTEXT_SET_STRING (handler);
}

#undef LIBCRUN_CONTEXT_SET_STRING

ध्रुव वर्ण *
libcrun_context_get_pid_file (libcrun_context_t *ctx)
{
  पटिनिवत्त ctx->pid_file;
}

ध्रुव वर्ण *
libcrun_context_get_state_मूलम् (libcrun_context_t *ctx)
{
  सचे (ctx == रिक्त)
    पटिनिवत्त रिक्त;
  पटिनिवत्त ctx->state_मूलम्;
}

शून्य
libcrun_context_set_preserve_fds (libcrun_context_t *ctx, पूर्णाङ्क preserve_fds)
{
  ctx->preserve_fds = preserve_fds;
}

पूर्णाङ्क
libcrun_context_get_preserve_fds (libcrun_context_t *ctx)
{
  पटिनिवत्त ctx->preserve_fds;
}

शून्य
libcrun_context_set_listen_fds (libcrun_context_t *ctx, पूर्णाङ्क listen_fds)
{
  ctx->listen_fds = listen_fds;
}

शून्य
libcrun_context_set_args (libcrun_context_t *ctx, पूर्णाङ्क argc, वर्ण **argv)
{
  ctx->argc = argc;
  ctx->argv = argv;
}

शून्य
libcrun_context_set_systemd_cgroup (libcrun_context_t *ctx, bool मानम्)
{
  ctx->systemd_cgroup = मानम्;
}

शून्य
libcrun_context_set_detach (libcrun_context_t *ctx, bool मानम्)
{
  ctx->detach = मानम्;
}

शून्य
libcrun_context_set_no_new_keyring (libcrun_context_t *ctx, bool मानम्)
{
  ctx->no_new_keyring = मानम्;
}

शून्य
libcrun_context_set_no_pivot (libcrun_context_t *ctx, bool मानम्)
{
  ctx->no_pivot = मानम्;
}

शून्य
libcrun_context_set_force_no_cgroup (libcrun_context_t *ctx, bool मानम्)
{
  ctx->force_no_cgroup = मानम्;
}

शून्य
libcrun_context_set_output_handler (libcrun_context_t *ctx, crun_output_handler handler, शून्य *arg)
{
  ctx->output_handler = handler;
  ctx->output_handler_arg = arg;
}

पूर्णाङ्क
libcrun_context_init_logging (libcrun_context_t *ctx, ध्रुव वर्ण *log, libcrun_error_t *err)
{
  पटिनिवत्त libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, ctx->id, log, err);
}

स्थिर पूर्णाङ्क
block_signals (libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  sigset_t mask;
  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, रिक्त);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "sigprocmask");
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
unblock_signals (libcrun_error_t *err)
{
  पूर्णाङ्क i;
  पूर्णाङ्क ret;
  sigset_t mask;
  संरचना sigaction act = {};

  sigfillset (&mask);
  ret = sigprocmask (SIG_UNBLOCK, &mask, रिक्त);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "sigprocmask");

  act.sa_handler = SIG_DFL;
  अत्थाय (i = 0; i < NSIG; i++)
    {
      ret = sigaction (i, &act, रिक्त);
      सचे (ret < 0 && errno != EINVAL)
        पटिनिवत्त crun_make_error (err, errno, "sigaction");
    }

  पटिनिवत्त 0;
}

/* must be used on the host before pivot_root(2).  */
स्थिर पूर्णाङ्क
initialize_security (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = libcrun_init_caps (container, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (UNLIKELY (proc == रिक्त))
    पटिनिवत्त 0;

  सचे (proc->apparmor_profile)
    {
      ret = libcrun_initialize_apparmor (err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  ret = libcrun_initialize_selinux (container, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  पटिनिवत्त 0;
}

/* Emit the "annotations" map.  Returns a json_gen_status; callers can wrap
   the call with GEN_OR_FAIL.  */
स्थिर पूर्णाङ्क
gen_annotations (json_gen_ctx *gen, json_map_string_string *annotations)
{
  size_t i;
  पूर्णाङ्क r;

  GEN_कुञ्चिका (gen, "annotations");
  GEN_OR_FAIL (json_gen_map_open (gen));

  अत्थाय (i = 0; i < annotations->len; i++)
    {
      /* An annotation whose value is null in the document is parsed into a
         NULL, which cannot be emitted as a JSON string; leave it out.  */
      सचे (annotations->कुञ्चिकाः[i] == रिक्त || annotations->मानानि[i] == रिक्त)
        अनुवर्त;

      GEN_STR (gen, annotations->कुञ्चिकाः[i]);
      GEN_STR (gen, annotations->मानानि[i]);
    }

  पटिनिवत्त json_gen_map_close (gen);

gen_error:
  पटिनिवत्त r;
}

/* Return the bundle path to report in the container state.  crun always
   chdir()s into the bundle, so fall back to getcwd() when the caller has no
   absolute bundle path, as can happen when libcrun is used as a library.
   *ALLOCATED must be freed by the caller.  */
स्थिर ध्रुव वर्ण *
get_bundle_path (ध्रुव वर्ण *bundle, वर्ण **allocated)
{
  सचे (bundle != रिक्त && bundle[0] == '/')
    पटिनिवत्त bundle;

  *allocated = getcwd (रिक्त, 0);
  सचे (*allocated == रिक्त)
    OOM ();
  पटिनिवत्त *allocated;
}

स्थिर पूर्णाङ्क
do_hooks (runtime_spec_schema_config_schema *def, pid_t pid, ध्रुव वर्ण *id, bool keep_going, ध्रुव वर्ण *bundle,
          ध्रुव वर्ण *cwd, ध्रुव वर्ण *status, ध्रुव वर्ण *name, hook **hooks, size_t hooks_len, पूर्णाङ्क out_fd,
          पूर्णाङ्क err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  size_t i, stdin_len;
  पूर्णाङ्क r, ret;
  वर्ण *stdin = रिक्त;
  cleanup_free वर्ण *bundle_allocated = रिक्त;
  ध्रुव वर्ण *rootfs = def->मूलम् ? def->मूलम्->path : "";
  cleanup_json_gen json_gen_ctx *gen = रिक्त;

  bundle = get_bundle_path (bundle, &bundle_allocated);

  सचे (! json_gen_init (&gen, रिक्त))
    पटिनिवत्त crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 0);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_कुञ्चिका (gen, "ociVersion");
  GEN_STR (gen, "1.0");

  GEN_कुञ्चिका (gen, "id");
  GEN_STR (gen, id);

  GEN_कुञ्चिका (gen, "pid");
  GEN_OR_FAIL (map_int (gen, pid));

  GEN_कुञ्चिका (gen, "root");
  GEN_STR (gen, rootfs);

  GEN_कुञ्चिका (gen, "bundle");
  GEN_STR (gen, bundle);

  GEN_कुञ्चिका (gen, "status");
  GEN_STR (gen, status);

  सचे (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, (ध्रुव वर्ण **) &stdin, &stdin_len));

  ret = 0;

  bool error_created = false;
  अत्थाय (i = 0; i < hooks_len; i++)
    {
      वर्ण **env = environ;

      सचे (hooks[i]->env)
        env = hooks[i]->env;

      /* Release the error from the previous iteration, if any.  */
      सचे (error_created)
        {
          crun_error_release (err);
          error_created = false;
        }

      ret = run_process_with_stdin_timeout_envp (hooks[i]->path, hooks[i]->args, cwd, hooks[i]->timeout, env,
                                                 stdin, stdin_len, out_fd, err_fd, can_ignore_chdir_errors, err);
      सचे (UNLIKELY (ret < 0))
        error_created = true;

      सचे (UNLIKELY (ret != 0))
        {
          सचे (keep_going)
            libcrun_warning ("error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
          अञ्ञथा
            {
              libcrun_error (0, "error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
              भङ्ग;
            }
        }
    }

  पटिनिवत्त ret;

gen_error:
  पटिनिवत्त json_gen_error_to_crun_error (r, err);
}

स्थिर पूर्णाङ्क
get_json_gen_result (json_gen_ctx *gen, वर्ण **out, size_t *out_len)
{
  ध्रुव वर्ण *buf = रिक्त;
  size_t buf_len = 0;
  पूर्णाङ्क r;

  r = json_gen_get_buf (gen, &buf, &buf_len);
  सचे (UNLIKELY (r != json_gen_status_ok))
    पटिनिवत्त r;

  *out_len = buf_len;

  *out = xmalloc (buf_len + 1);
  memcpy (*out, buf, buf_len);
  (*out)[buf_len] = '\0';

  पटिनिवत्त json_gen_status_ok;
}

स्थिर पूर्णाङ्क
get_seccomp_receiver_fd_payload (libcrun_container_t *container, ध्रुव वर्ण *status, pid_t own_pid,
                                 वर्ण **seccomp_fd_payload, size_t *seccomp_fd_payload_len, libcrun_error_t *err)
{
  पूर्णाङ्क r;
  json_gen_ctx *gen = रिक्त;
  runtime_spec_schema_config_schema *def = container->container_def;
  ध्रुव वर्ण *ध्रुव OCI_VERSION = "0.2.0";

  सचे (! json_gen_init (&gen, रिक्त))
    पटिनिवत्त crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_कुञ्चिका (gen, "ociVersion");
  GEN_STR (gen, OCI_VERSION);

  GEN_कुञ्चिका (gen, "fds");
  GEN_OR_FAIL (json_gen_array_open (gen));

  GEN_कुञ्चिका (gen, "seccompFd");
  GEN_OR_FAIL (json_gen_array_close (gen));

  GEN_कुञ्चिका (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  सचे (def && def->linux && def->linux->seccomp)
    {
      ध्रुव वर्ण *metadata = def->linux->seccomp->listener_metadata;

      सचे (metadata)
        {
          GEN_कुञ्चिका (gen, "metadata");
          GEN_STR (gen, metadata);
        }
    }

  /* State.  */
  GEN_कुञ्चिका (gen, "state");
  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_कुञ्चिका (gen, "ociVersion");
  GEN_STR (gen, OCI_VERSION);

  सचे (container->context && container->context->id)
    {
      GEN_कुञ्चिका (gen, "id");
      GEN_STR (gen, container->context->id);
    }

  GEN_कुञ्चिका (gen, "status");
  GEN_STR (gen, status);

  GEN_कुञ्चिका (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  सचे (container->context && container->context->bundle)
    {
      GEN_कुञ्चिका (gen, "bundle");
      GEN_STR (gen, container->context->bundle);
    }

  सचे (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));
  /* End state.  */

  GEN_OR_FAIL (json_gen_map_close (gen));

  r = get_json_gen_result (gen, seccomp_fd_payload, seccomp_fd_payload_len);

gen_error:
  json_gen_free (gen);

  पटिनिवत्त json_gen_error_to_crun_error (r, err);
}

स्थिर पूर्णाङ्क
send_sync_cb (शून्य *data, libcrun_error_t *err)
{
  पूर्णाङ्क sync_socket_fd = *((पूर्णाङ्क *) data);
  पूर्णाङ्क ret;

  /* sync 2.  */
  ret = sync_socket_send_sync (sync_socket_fd, false, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  /* sync 3.  */
  पटिनिवत्त sync_socket_wait_sync (रिक्त, sync_socket_fd, false, err);
}

स्थिर पूर्णाङ्क
maybe_chown_std_streams (uid_t container_uid, gid_t container_gid, libcrun_error_t *err)
{
  पूर्णाङ्क ret, i;

  अत्थाय (i = 0; i < 3; i++)
    {
      सचे (! isatty (i))
        {
          संरचना stat statbuf;
          ret = fstat (i, &statbuf);
          सचे (UNLIKELY (ret < 0))
            {
              सचे (errno == EBADF)
                अनुवर्त;
              पटिनिवत्त crun_make_error (err, errno, "fstat fd `%d`", i);
            }

          /* Skip chown for device files */
          सचे (S_ISCHR (statbuf.st_mode) || S_ISBLK (statbuf.st_mode))
            अनुवर्त;

          ret = fchown (i, container_uid, container_gid);
          सचे (UNLIKELY (ret < 0))
            {
              /* EINVAL means the user is not mapped in the current userns.
                 Ignore EPERM and EROFS as well as there is no reason to fail
                 so early, and let the container payload deal with it.
                 EBADF means fd is closed.
              */
              सचे (errno == EINVAL || errno == EPERM || errno == EROFS || errno == EBADF)
                अनुवर्त;

              पटिनिवत्त crun_make_error (err, errno, "fchown std stream `%i`", i);
            }
        }
    }
  पटिनिवत्त 0;
}

पूर्णाङ्क
libcrun_container_notify_handler (संरचना container_entrypoint_s *args,
                                  गणना handler_configure_phase phase,
                                  libcrun_container_t *container, ध्रुव वर्ण *rootfs,
                                  libcrun_error_t *err)
{
  संरचना custom_handler_s *h;

  सचे (args->custom_handler == रिक्त)
    पटिनिवत्त 0;

  h = args->custom_handler->vtable;
  सचे (h == रिक्त || h->configure_container == रिक्त)
    पटिनिवत्त 0;

  पटिनिवत्त h->configure_container (args->custom_handler->cookie, phase,
                                 args->context, container,
                                 rootfs, err);
}

/* Resolve and normalize the container rootfs path.  */
स्थिर पूर्णाङ्क
resolve_rootfs_path (libcrun_container_t *container, वर्ण **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  सचे (def->मूलम् && def->मूलम्->path)
    {
      *rootfs = realpath (def->मूलम्->path, रिक्त);
      सचे (UNLIKELY (*rootfs == रिक्त))
        {
          पूर्णाङ्क saved_errno = errno;

          /* If realpath failed for any reason, try the relative directory.  */
          सचे (def->मूलम्->path[0] == '/')
            {
              cleanup_free वर्ण *cwd = रिक्त;
              libcrun_error_t tmp_err = रिक्त;
              ssize_t len;
              पूर्णाङ्क proc_fd;

              proc_fd = libcrun_get_cached_proc_fd (container, &tmp_err);
              सचे (LIKELY (proc_fd >= 0))
                {
                  len = safe_readlinkat (proc_fd, "self/cwd", &cwd, 0, &tmp_err);
                  सचे (LIKELY (len > 0))
                    {
                      /* If the rootfs is under the current working directory, just use its relative path.  */
                      सचे (has_dir_prefix (def->मूलम्->path, cwd))
                        {
                          ध्रुव वर्ण *it = consume_slashes (def->मूलम्->path + len);
                          सचे (LIKELY (*it))
                            {
                              पूर्णाङ्क ret = faccessat (AT_FDCWD, it, F_OK, 0);
                              सचे (LIKELY (ret == 0))
                                *rootfs = xstrdup (it);
                            }
                        }
                    }
                }
              crun_error_release (&tmp_err);
            }

          सचे (*rootfs == रिक्त)
            {
              सचे (saved_errno == ENOENT)
                पटिनिवत्त crun_make_error (err, ENOENT, "the rootfs path `%s` does not exist", def->मूलम्->path);

              *rootfs = xstrdup (def->मूलम्->path);
            }
        }
    }
  पटिनिवत्त 0;
}

/* Initialize the environment variables.  */
स्थिर पूर्णाङ्क
setup_environment (runtime_spec_schema_config_schema *def, uid_t container_uid, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  सचे (clearenv ())
    पटिनिवत्त crun_make_error (err, errno, "clearenv");

  सचे (def->process)
    {
      size_t i;

      अत्थाय (i = 0; i < def->process->env_len; i++)
        {
          /* A null element of the array of variables is parsed into a NULL,
             which putenv(3) cannot be handed.  */
          सचे (def->process->env[i] == रिक्त)
            पटिनिवत्त crun_make_error (err, EINVAL, "environment variable is not specified");

          सचे (putenv (def->process->env[i]) < 0)
            पटिनिवत्त crun_make_error (err, errno, "putenv `%s`", def->process->env[i]);
        }
    }

  सचे (is_empty_string (getenv ("HOME")))
    {
      ret = set_home_env (container_uid);
      सचे (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          सचे (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            पटिनिवत्त crun_make_error (err, errno, "setenv HOME");
        }
    }

  पटिनिवत्त 0;
}

/* Configure terminal and console for interactive containers.  */
स्थिर पूर्णाङ्क
setup_terminal (संरचना container_entrypoint_s *entrypoint_args, libcrun_container_t *container,
                पूर्णाङ्क console_socket, पूर्णाङ्क console_socketpair, पूर्णाङ्क has_terminal, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क terminal_fd = -1;
  पूर्णाङ्क ret;

  सचे (! has_terminal)
    पटिनिवत्त 0;

  fflush (stderr);

  सचे (console_socket >= 0 || (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0))
    {
      terminal_fd = libcrun_set_terminal (container, err);
      सचे (UNLIKELY (terminal_fd < 0))
        पटिनिवत्त terminal_fd;
    }

  सचे (console_socket >= 0)
    {
      ret = send_fd_to_socket (console_socket, terminal_fd, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }
  अञ्ञथा सचे (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0)
    {
      ret = send_fd_to_socket (console_socketpair, terminal_fd, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  पटिनिवत्त 0;
}

/* Resolve and validate the container's executable path.  */
स्थिर पूर्णाङ्क
setup_executable_path (संरचना container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                       वर्ण **exec_path, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  सचे (def->process && def->process->args)
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      सचे (UNLIKELY (ret < 0))
        {
          सचे (entrypoint_args->custom_handler == रिक्त && crun_error_get_errno (err) == ENOENT)
            पटिनिवत्त ret;

          /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
             once the process switched to the use that runs in the container.  This might be necessary
             when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
             is not honored.  */
          crun_error_release (err);
        }
    }

  पटिनिवत्त 0;
}

/* Apply security settings for the container process.  */
स्थिर पूर्णाङ्क
apply_security_settings (संरचना container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                         libcrun_container_t *container, pid_t own_pid, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  runtime_spec_schema_config_schema_process_capabilities *capabilities;
  पूर्णाङ्क no_new_privs;

  सचे (def->process && ! def->process->no_new_privileges)
    {
      वर्ण **seccomp_flags = रिक्त;
      size_t seccomp_flags_len = 0;
      cleanup_free वर्ण *seccomp_fd_payload = रिक्त;
      size_t seccomp_fd_payload_len = 0;

      सचे (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      सचे (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  capabilities = def->process ? def->process->capabilities : रिक्त;
  no_new_privs = def->process ? def->process->no_new_privileges : 1;
  ret = libcrun_set_caps (capabilities, container->container_uid, container->container_gid, no_new_privs,
                          entrypoint_args->custom_handler != रिक्त, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  पटिनिवत्त 0;
}

/* Create the process that joins the existing PID namespace now that the
   host mounts are not visible anymore, and continue the setup from there.  */
स्थिर पूर्णाङ्क
do_delayed_pidns_fork (पूर्णाङ्क sync_socket, pid_t *own_pid, libcrun_error_t *err)
{
  pid_t pid;
  पूर्णाङ्क ret;

  pid = fork ();
  सचे (UNLIKELY (pid < 0))
    पटिनिवत्त crun_make_error (err, errno, "fork");

  सचे (pid)
    {
      /* Report the new PID to the parent and exit immediately.  */
      ret = sync_socket_send_sync (sync_socket, false, err);
      सचे (LIKELY (ret == 0))
        {
          ret = TEMP_FAILURE_RETRY (लिख (sync_socket, &pid, परिमाण (pid)));
          सचे (LIKELY (ret == परिमाण (pid)))
            _safe_exit (EXIT_SUCCESS);
        }

      kill (pid, SIGKILL);
      _safe_exit (EXIT_FAILURE);
    }

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (पठ (sync_socket, own_pid, परिमाण (*own_pid)));
  सचे (UNLIKELY (ret != परिमाण (*own_pid)))
    {
      सचे (ret >= 0)
        errno = 0;
      पटिनिवत्त crun_make_error (err, errno, "read from sync socket");
    }

  पटिनिवत्त 0;
}

संरचना delayed_pidns_fork_s
{
  libcrun_container_t *container;
  पूर्णाङ्क sync_socket;
  pid_t *own_pid;
};

स्थिर पूर्णाङ्क
delayed_pidns_fork_cb (शून्य *data, libcrun_error_t *err)
{
  संरचना delayed_pidns_fork_s *d = data;

  सचे (! libcrun_container_delayed_pidns_fork (d->container))
    पटिनिवत्त 0;

  पटिनिवत्त do_delayed_pidns_fork (d->sync_socket, d->own_pid, err);
}

स्थिर पूर्णाङ्क
container_init_setup (शून्य *args, pid_t *own_pid, वर्ण *notify_socket,
                      पूर्णाङ्क sync_socket, वर्ण **exec_path, libcrun_error_t *err)
{
  संरचना container_entrypoint_s *entrypoint_args = args;
  libcrun_container_t *container = entrypoint_args->container;
  bool chdir_done = false;
  पूर्णाङ्क ret;
  पूर्णाङ्क has_terminal;
  cleanup_close पूर्णाङ्क console_socket = -1;
  cleanup_close पूर्णाङ्क console_socketpair = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free वर्ण *rootfs = रिक्त;
  संरचना delayed_pidns_fork_s delayed_pidns_fork = {
    .container = container,
    .sync_socket = sync_socket,
    .own_pid = own_pid,
  };

  ret = initialize_security (container, def->process, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = libcrun_configure_network (container, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = resolve_rootfs_path (container, &rootfs, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  /* Configure terminal socket pair for container communication.  */
  सचे (entrypoint_args->terminal_socketpair[0] >= 0)
    {
      close_and_reset (&entrypoint_args->terminal_socketpair[0]);
      console_socketpair = entrypoint_args->terminal_socketpair[1];
    }

  /* sync 1.  */
  ret = sync_socket_wait_sync (रिक्त, sync_socket, false, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  has_terminal = container->container_def->process && container->container_def->process->terminal;
  सचे (has_terminal && entrypoint_args->context->console_socket)
    console_socket = entrypoint_args->console_socket_fd;

  ret = libcrun_set_sysctl (container, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_BEFORE_MOUNTS, container, rootfs, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = libcrun_do_pivot_मूलम् (container, entrypoint_args->context->no_pivot, &rootfs, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  /* sync 2 and 3 are sent as part of libcrun_set_mounts.  */
  ret = libcrun_set_mounts (entrypoint_args, container, rootfs, send_sync_cb, &sync_socket, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  /* The createContainer hooks MUST run after the mounts are set up but before
     the pivot_root.  When these hooks are present can_use_open_tree_namespace()
     forces the deferred-pivot path, so the actual pivot happens later in
     libcrun_finalize_mounts().  */
  सचे (def->hooks && def->hooks->create_container_len)
    {
      libcrun_error_t tmp_err = रिक्त;
      पूर्णाङ्क in_userns = check_running_in_user_namespace (&tmp_err);
      सचे (tmp_err)
        crun_error_release (&tmp_err);
      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, रिक्त, "created", "createContainer",
                      (hook **) def->hooks->create_container,
                      def->hooks->create_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      सचे (UNLIKELY (ret != 0))
        पटिनिवत्त ret;
    }

  ret = libcrun_finalize_mounts (entrypoint_args, container, rootfs, delayed_pidns_fork_cb, &delayed_pidns_fork, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (def->process)
    {
      ret = libcrun_set_selinux_label (container, def->process, false, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = libcrun_set_apparmor_profile (container, def->process, false, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  ret = mark_or_close_fds_ge_than (container, entrypoint_args->context->preserve_fds + 3, false, err);
  सचे (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ret = libcrun_reopen_dev_null (err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = maybe_chown_std_streams (container->container_uid, container->container_gid, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = setup_environment (def, container->container_uid, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  /* Set primary process to 1 explicitly if nothing is configured and LISTEN_FD is not set.  */
  सचे (entrypoint_args->context->listen_fds > 0 && getenv ("LISTEN_PID") == रिक्त)
    {
      libcrun_warning ("setting LISTEN_PID=1 since no previous configuration was found");
      सचे (UNLIKELY (setenv ("LISTEN_PID", "1", 1) < 0))
        पटिनिवत्त crun_make_error (err, errno, "setenv LISTEN_PID");
    }

  /* Attempt to chdir immediately here, before doing the setresuid.  If we fail here, let's
     try again later once the process switched to the user that runs in the container.  */
  सचे (def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      सचे (LIKELY (ret == 0))
        chdir_done = true;
      अञ्ञथा
        crun_error_release (err);
    }

  ret = setsid ();
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "setsid");

  ret = setup_terminal (entrypoint_args, container, console_socket, console_socketpair, has_terminal, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  close_and_reset (&console_socket);
  close_and_reset (&console_socketpair);

  ret = setup_executable_path (entrypoint_args, def, exec_path, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = libcrun_set_hostname (container, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = libcrun_set_domainname (container, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  सचे (def->process && def->process->user)
    umask (def->process->user->umask_present ? def->process->user->umask : 0022);

  ret = apply_security_settings (entrypoint_args, def, container, *own_pid, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (UNLIKELY (def->process && def->process->args && *exec_path == रिक्त))
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      सचे (UNLIKELY (ret < 0))
        {
          सचे (entrypoint_args->custom_handler == रिक्त || is_empty_string (def->process->args[0]))
            पटिनिवत्त ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          *exec_path = xstrdup (def->process->args[0]);
        }
    }

  /* The chdir was not already performed, so try again now after switching to the UID/GID in the container.  */
  सचे (! chdir_done && def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  सचे (notify_socket && putenv (notify_socket) < 0)
    पटिनिवत्त crun_make_error (err, errno, "putenv `%s`", notify_socket);

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
open_hooks_output (libcrun_container_t *container, पूर्णाङ्क *out_fd, पूर्णाङ्क *err_fd, libcrun_error_t *err)
{
  ध्रुव वर्ण *annotation;

  *err_fd = *out_fd = -1;

  libcrun_debug ("Opening hooks output");
  annotation = find_annotation (container, "run.oci.hooks.stdout");
  सचे (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stdout` annotation");
      *out_fd = TEMP_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      सचे (UNLIKELY (*out_fd < 0))
        पटिनिवत्त crun_make_error (err, errno, "open `%s`", annotation);
    }

  annotation = find_annotation (container, "run.oci.hooks.stderr");
  सचे (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stderr` annotation");
      *err_fd = TEMP_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      सचे (UNLIKELY (*err_fd < 0))
        पटिनिवत्त crun_make_error (err, errno, "open `%s`", annotation);
    }

  पटिनिवत्त 0;
}

/* The environment the process was started with lives on the initial stack, right after the
   argv strings and before the string AT_EXECFN points to.  clearenv(3) only drops the pointers
   to it, so when there is no execv to replace the stack, erase it or the container process can
   still read it, e.g. through /proc/self/environ.  */
स्थिर शून्य
erase_initial_environment (वर्ण **argv, पूर्णाङ्क argc)
{
  वर्ण *start = argv[argc - 1] + strlen (argv[argc - 1]) + 1;
  वर्ण *end = (वर्ण *) getauxval (AT_EXECFN);

  सचे (end > start)
    memset (start, 0, end - start);
}

स्थिर शून्य
rewrite_argv (वर्ण **argv, पूर्णाङ्क argc, ध्रुव वर्ण *name, वर्ण **args, size_t args_len)
{
  cleanup_free वर्ण *decorated_name = रिक्त;
  size_t available_len = argv[argc - 1] - argv[0] + strlen (argv[argc - 1]) + 1;
  cleanup_free वर्ण *new_argv = रिक्त;
  size_t i, so_far = 0, needed = 0;

  needed = xasprintf (&decorated_name, "[libcrun:%s]", name) + 1;
  अत्थाय (i = 0; i < args_len; i++)
    needed += strlen (args[i]) + 1;

  new_argv = xmalloc0 (needed + 1);
  strcpy (new_argv, decorated_name);
  so_far = strlen (decorated_name) + 1;

  सचे (available_len >= needed)
    {
      अत्थाय (i = 0; i < args_len; i++)
        {
          strcpy (new_argv + so_far, args[i]);
          so_far += strlen (args[i]) + 1;
        }
    }

  सचे (so_far >= available_len)
    so_far = available_len - 1;

  memset (argv[0], 0, available_len);
  memcpy (argv[0], new_argv, so_far);
}

/* Entrypoint to the container.  */
स्थिर पूर्णाङ्क
container_init (शून्य *args, वर्ण *notify_socket, पूर्णाङ्क sync_socket, libcrun_error_t *err)
{
  संरचना container_entrypoint_s *entrypoint_args = args;
  पूर्णाङ्क ret;
  runtime_spec_schema_config_schema *def = entrypoint_args->container->container_def;
  cleanup_free वर्ण *exec_path = रिक्त;
  __attribute__ ((unused)) cleanup_free वर्ण *notify_socket_cleanup = notify_socket;
  pid_t own_pid = 0;

  entrypoint_args->sync_socket = sync_socket;

  crun_set_output_handler (log_write_to_sync_socket, args);

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (पठ (sync_socket, &own_pid, परिमाण (own_pid)));
  सचे (UNLIKELY (ret != परिमाण (own_pid)))
    {
      सचे (ret >= 0)
        errno = 0;
      पटिनिवत्त crun_make_error (err, errno, "read from sync socket");
    }

  ret = container_init_setup (args, &own_pid, notify_socket, sync_socket, &exec_path, err);
  सचे (UNLIKELY (ret < 0))
    {
      /* If it fails to write the error using the sync socket, then fallback
         to stderr.  */
      सचे (sync_socket_write_error (sync_socket, err) == 0)
        {
          crun_error_release (err);
          /* A return value of > 0 means to fail with an error without err being set so to avoid
             printing it twice.  */
          पटिनिवत्त 1;
        }
      पटिनिवत्त ret;
    }

  entrypoint_args->sync_socket = -1;

  ret = unblock_signals (err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  /* sync 4.  */
  ret = sync_socket_send_sync (sync_socket, false, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  close_and_reset (&sync_socket);

  सचे (entrypoint_args->context->fifo_exec_wait_fd >= 0)
    {
      वर्ण buffer[1];
      fd_set read_set;
      cleanup_close पूर्णाङ्क fd = entrypoint_args->context->fifo_exec_wait_fd;
      entrypoint_args->context->fifo_exec_wait_fd = -1;

      FD_ZERO (&read_set);
      FD_SET (fd, &read_set);
      करोहि
        {
          ret = select (fd + 1, &read_set, रिक्त, रिक्त, रिक्त);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "select");

          ret = TEMP_FAILURE_RETRY (पठ (fd, buffer, परिमाण (buffer)));
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "read from the exec fifo");
      } याव (ret == 0);

      close_and_reset (&entrypoint_args->context->fifo_exec_wait_fd);
    }

  crun_set_output_handler (log_write_to_stderr, रिक्त);

  सचे (def->process && def->process->no_new_privileges)
    {
      वर्ण **seccomp_flags = रिक्त;
      size_t seccomp_flags_len = 0;
      cleanup_free वर्ण *seccomp_fd_payload = रिक्त;
      size_t seccomp_fd_payload_len = 0;

      सचे (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      सचे (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (entrypoint_args->container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags,
                                   seccomp_flags_len, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  सचे (UNLIKELY (def->process == रिक्त))
    पटिनिवत्त crun_make_error (err, 0, "block `process` not found");

  सचे (UNLIKELY (exec_path == रिक्त))
    पटिनिवत्त crun_make_error (err, 0, "executable path not specified");

  सचे (def->hooks && def->hooks->start_container_len)
    {
      libcrun_container_t *container = entrypoint_args->container;
      libcrun_error_t tmp_err = रिक्त;
      पूर्णाङ्क in_userns = check_running_in_user_namespace (&tmp_err);
      सचे (tmp_err)
        crun_error_release (&tmp_err);

      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, रिक्त, "created", "startContainer",
                      (hook **) def->hooks->start_container,
                      def->hooks->start_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      सचे (UNLIKELY (ret != 0))
        पटिनिवत्त ret;

      /* Seek stdout/stderr to the end.  If the hooks were using the same files,
         the container process overwrites what was previously written.  */
      (शून्य) lseek (1, 0, SEEK_END);
      (शून्य) lseek (2, 0, SEEK_END);
    }

  सचे (entrypoint_args->custom_handler)
    {
      /* Files marked with O_CLOEXEC are closed at execv time, so make sure they are closed now.
         This is a best effort operation, because the seccomp filter is already in place and it could
         stop some syscalls used by mark_or_close_fds_ge_than.
      */
      सचे (entrypoint_args->custom_handler->vtable->close_fds)
        {
          ret = entrypoint_args->custom_handler->vtable->close_fds (entrypoint_args->custom_handler->cookie,
                                                                    entrypoint_args->container,
                                                                    entrypoint_args->context->preserve_fds,
                                                                    err);
        }
      अञ्ञथा
        {
          ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
        }
      सचे (UNLIKELY (ret < 0))
        crun_error_release (err);

      prctl (PR_SET_NAME, entrypoint_args->custom_handler->vtable->name);

      सचे (entrypoint_args->context->argv)
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
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = libcrun_set_apparmor_profile (entrypoint_args->container, def->process, true, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = entrypoint_args->custom_handler->vtable->run_func (entrypoint_args->custom_handler->cookie,
                                                               entrypoint_args->container,
                                                               exec_path,
                                                               def->process->args);
      सचे (ret != 0)
        पटिनिवत्त crun_make_error (err, 0, "exec container process failed with handler as `%s`", entrypoint_args->custom_handler->vtable->name);

      पटिनिवत्त ret;
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation since the seccomp profile is already in place now and might block
     some of the syscalls needed by mark_or_close_fds_ge_than.  */
  ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
  सचे (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, def->process->args));

  सचे (errno == ENOENT)
    पटिनिवत्त crun_make_error (err, errno, "exec container process (missing dynamic library?) `%s`", exec_path);

  पटिनिवत्त crun_make_error (err, errno, "exec container process `%s`", exec_path);
}

स्थिर पूर्णाङ्क
read_container_config_from_state (libcrun_container_t **container, ध्रुव वर्ण *state_मूलम्, ध्रुव वर्ण *id,
                                  libcrun_error_t *err)
{
  cleanup_free वर्ण *config_file = रिक्त;
  cleanup_free वर्ण *dir = रिक्त;
  पूर्णाङ्क ret;

  *container = रिक्त;

  ret = libcrun_get_state_directory (&dir, state_मूलम्, id, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = append_paths (&config_file, err, dir, "config.json", रिक्त);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  *container = libcrun_container_load_from_file (config_file, err);
  सचे (*container == रिक्त)
    पटिनिवत्त -1;

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
run_poststop_hooks (arg_unused libcrun_context_t *context, libcrun_container_t *container,
                    runtime_spec_schema_config_schema *def,
                    libcrun_container_status_t *status, ध्रुव वर्ण *state_मूलम्, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container_cleanup = रिक्त;
  पूर्णाङ्क ret;

  सचे (def == रिक्त)
    {
      सचे (container == रिक्त)
        {
          ret = read_container_config_from_state (&container_cleanup, state_मूलम्, id, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
          container = container_cleanup;
        }

      def = container->container_def;
    }

  सचे (def->hooks && def->hooks->poststop_len)
    {
      cleanup_close पूर्णाङ्क hooks_out_fd = -1;
      cleanup_close पूर्णाङ्क hooks_err_fd = -1;

      सचे (container == रिक्त)
        {
          ret = read_container_config_from_state (&container_cleanup, state_मूलम्, id, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
          container = container_cleanup;
        }

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = do_hooks (def, 0, id, true, status->bundle, status->bundle, "stopped", "poststop", (hook **) def->hooks->poststop,
                      def->hooks->poststop_len, hooks_out_fd, hooks_err_fd, false, err);
      सचे (UNLIKELY (ret != 0))
        {
          सचे (ret < 0)
            libcrun_error_report_and_release (err);
          अञ्ञथा
            libcrun_error (0, "poststop hook failed with exit code: %d", ret);
        }
    }
  पटिनिवत्त 0;
}

स्थिर bool
has_new_pid_namespace (runtime_spec_schema_config_schema *def)
{
  size_t i;

  सचे (def->linux == रिक्त || def->linux->namespaces == रिक्त)
    पटिनिवत्त false;

  अत्थाय (i = 0; i < def->linux->namespaces_len; i++)
    {
      सचे (strcmp (def->linux->namespaces[i]->type, "pid") == 0 && def->linux->namespaces[i]->path == रिक्त)
        पटिनिवत्त true;
    }
  पटिनिवत्त false;
}

/* Delete the container state.  CONTAINER is the container the caller already has
   available, or NULL.  When it is not NULL, the poststop hooks are run even if the
   status file was never written, e.g. when the container creation failed.  */
स्थिर पूर्णाङ्क
container_delete_internal (libcrun_context_t *context, libcrun_container_t *container,
                           runtime_spec_schema_config_schema *def, ध्रुव वर्ण *id, bool force, bool killall,
                           libcrun_error_t *err)
{
  cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = रिक्त;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_container libcrun_container_t *container_cleanup = रिक्त;
  ध्रुव वर्ण *state_मूलम् = context->state_मूलम्;
  पूर्णाङ्क ret;

  ret = libcrun_read_container_status (&status, state_मूलम्, id, err);
  सचे (UNLIKELY (ret < 0))
    {
      सचे (force && crun_error_get_errno (err) == ENOENT)
        {
          libcrun_error_t tmp_err = रिक्त;

          crun_error_release (err);

          सचे (container)
            {
              libcrun_container_status_t partial_status = {
                .bundle = (वर्ण *) context->bundle,
              };

              ret = run_poststop_hooks (context, container, def, &partial_status, state_मूलम्, id, err);
              सचे (UNLIKELY (ret < 0))
                crun_error_write_warning_and_release (context->output_handler_arg, &err);
            }

          libcrun_container_delete_status (state_मूलम्, id, &tmp_err);
          crun_error_release (&tmp_err);
          पटिनिवत्त 0;
        }
      crun_error_release (err);
      पटिनिवत्त libcrun_container_delete_status (state_मूलम्, id, err);
    }

  cgroup_status = libcrun_cgroup_make_status (&status);

  सचे (! force)
    {
      ret = libcrun_is_container_running (&status, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
      सचे (ret == 1)
        {
          ret = libcrun_status_has_read_exec_fifo (state_मूलम्, id, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
          सचे (ret == 0)
            पटिनिवत्त crun_make_error (err, 0, "the container `%s` is not in `created` or `stopped` state", id);

          /* If the container is in "created" state, then do the equivalent of delete --force.  */
          killall = force = true;
        }
    }

  सचे (def == रिक्त)
    {
      सचे (container == रिक्त)
        {
          ret = read_container_config_from_state (&container_cleanup, state_मूलम्, id, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;

          container = container_cleanup;
        }

      def = container->container_def;
    }

  सचे (killall && force)
    {
      /* If the container has a pid namespace, it is enough to kill the first
         process (pid=1 in the namespace).
      */
      सचे (has_new_pid_namespace (def))
        {
          ret = libcrun_kill_linux (&status, SIGKILL, err);
          सचे (UNLIKELY (ret < 0))
            {
              सचे (crun_error_get_errno (err) != ESRCH)
                पटिनिवत्त ret;

              crun_error_release (err);
            }
        }
      अञ्ञथा सचे (status.cgroup_path)
        {
          ret = libcrun_cgroup_killall (cgroup_status, SIGKILL, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }
    }

  सचे (def->linux && def->linux->intel_rdt)
    {
      ret = libcrun_destroy_intelrdt (id, def, err);
      सचे (UNLIKELY (ret < 0))
        libcrun_error_report_and_release (err);
    }

  सचे (status.cgroup_path)
    {
      /* If the cgroup cannot be destroyed, there are still processes in it.
         Keep the container status, it is the only record of the cgroup, so
         that the delete can be retried.  */
      ret = libcrun_cgroup_destroy (cgroup_status, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  ret = run_poststop_hooks (context, container, def, &status, state_मूलम्, id, err);
  सचे (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  पटिनिवत्त libcrun_container_delete_status (state_मूलम्, id, err);
}

पूर्णाङ्क
libcrun_container_delete (libcrun_context_t *context, ध्रुव वर्ण *id, bool force, libcrun_error_t *err)
{
  पटिनिवत्त container_delete_internal (context, रिक्त, रिक्त, id, force, true, err);
}

पूर्णाङ्क
libcrun_container_kill (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *सङ्केत, libcrun_error_t *err)
{
  पूर्णाङ्क sig, ret;
  ध्रुव वर्ण *state_मूलम् = context->state_मूलम्;
  cleanup_container_status libcrun_container_status_t status = {};

  sig = str2sig (सङ्केत);
  सचे (UNLIKELY (sig < 0))
    पटिनिवत्त crun_make_error (err, 0, "unknown signal `%s`", सङ्केत);

  ret = libcrun_read_container_status (&status, state_मूलम्, id, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  पटिनिवत्त libcrun_kill_linux (&status, sig, err);
}

पूर्णाङ्क
libcrun_container_killall (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *सङ्केत, libcrun_error_t *err)
{
  पूर्णाङ्क sig, ret;
  ध्रुव वर्ण *state_मूलम् = context->state_मूलम्;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = रिक्त;

  sig = str2sig (सङ्केत);
  सचे (UNLIKELY (sig < 0))
    पटिनिवत्त crun_make_error (err, 0, "unknown signal `%s`", सङ्केत);

  ret = libcrun_read_container_status (&status, state_मूलम्, id, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  cgroup_status = libcrun_cgroup_make_status (&status);

  ret = libcrun_cgroup_killall (cgroup_status, sig, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
write_container_status (libcrun_container_t *container, libcrun_context_t *context,
                        pid_t pid, संरचना libcrun_cgroup_status *cgroup_status,
                        libcrun_error_t *err)
{
  cleanup_free वर्ण *bundle_allocated = रिक्त;
  ध्रुव वर्ण *bundle = get_bundle_path (context->bundle, &bundle_allocated);
  cleanup_free वर्ण *owner = get_user_name (geteuid ());
  वर्ण *external_descriptors = libcrun_get_external_descriptors (container);
  वर्ण *rootfs = container->container_def->मूलम् ? container->container_def->मूलम्->path : "";
  वर्ण created[35];

  libcrun_container_status_t status = {
    .pid = pid,
    .rootfs = rootfs,
    .bundle = (वर्ण *) bundle,
    .created = created,
    .owner = owner,
    .systemd_cgroup = context->systemd_cgroup,
    .detached = context->detach,
    .no_new_keyring = context->no_new_keyring,
    .external_descriptors = external_descriptors,
    .cgroup_path = रिक्त,
    .scope = रिक्त,
  };

  get_current_timestamp (created, परिमाण (created));

  सचे (cgroup_status)
    {
      पूर्णाङ्क ret;

      ret = libcrun_cgroup_get_status (cgroup_status, &status, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  सचे (external_descriptors == रिक्त)
    पटिनिवत्त crun_make_error (err, 0, "invalid internal state.  No external descriptors found");
  पटिनिवत्त libcrun_write_container_status (context->state_मूलम्, context->id, &status, err);
}

स्थिर पूर्णाङ्क
reap_subprocesses (pid_t *main_process, पूर्णाङ्क *main_process_exit, पूर्णाङ्क *last_process, libcrun_error_t *err)
{
  *last_process = 0;
  याव (1)
    {
      पूर्णाङ्क status;
      पूर्णाङ्क r = waitpid_ignore_stopped (-1, &status, WNOHANG);
      सचे (r < 0)
        {
          सचे (errno == EINTR)
            अनुवर्त;
          सचे (errno == ECHILD)
            {
              *last_process = 1;
              पटिनिवत्त 0;
            }
          पटिनिवत्त crun_make_error (err, errno, "waitpid");
        }
      सचे (r == 0)
        भङ्ग;
      सचे (r != *main_process)
        अनुवर्त;

      *main_process_exit = get_process_exit_status (status);

      /* Once reaped, the pid can be reused by an unrelated process.  */
      *main_process = 0;
    }
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
send_sd_notify (ध्रुव वर्ण *ready_str, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  पूर्णाङ्क ret = sd_notify (0, ready_str);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, -ret, "sd_notify");

#  सचे HAVE_SD_NOTIFY_BARRIER
  /* Hard-code a 30 seconds timeout.  Ignore errors.  */
  sd_notify_barrier (0, 30 * 1000000);
#  endif
#अञ्ञथा
  (शून्य) ready_str;
  (शून्य) err;
#endif
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
handle_notify_socket (पूर्णाङ्क notify_socketfd, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  पूर्णाङ्क ret;
  वर्ण buf[256];
  ध्रुव वर्ण *ready_str = "READY=1";

  ret = recvfrom (notify_socketfd, buf, परिमाण (buf) - 1, 0, रिक्त, रिक्त);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "recvfrom notify socket");

  buf[ret] = '\0';
  सचे (strstr (buf, ready_str))
    {
      ret = send_sd_notify (ready_str, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      पटिनिवत्त 1;
    }
  पटिनिवत्त 0;
#अञ्ञथा
  (शून्य) notify_socketfd;
  (शून्य) err;
  पटिनिवत्त 1;
#endif
}

संरचना wait_for_process_args
{
  pid_t pid;
  libcrun_context_t *context;
  पूर्णाङ्क terminal_fd;
  पूर्णाङ्क notify_socket;
  पूर्णाङ्क *container_ready_fd;
  पूर्णाङ्क seccomp_notify_fd;
  ध्रुव वर्ण *seccomp_notify_plugins;
};

स्थिर पूर्णाङ्क
write_pid_file (ध्रुव वर्ण *pid_file, pid_t pid, libcrun_error_t *err)
{
  वर्ण buf[32];
  पूर्णाङ्क buf_len = snprintf (buf, परिमाण (buf), "%d", pid);
  सचे (UNLIKELY (buf_len >= (पूर्णाङ्क) परिमाण (buf)))
    पटिनिवत्त crun_make_error (err, 0, "internal error: static buffer too small");
  पटिनिवत्त write_file_at_with_flags (AT_FDCWD, O_CREAT | O_TRUNC, 0700, pid_file, buf, buf_len, err);
}

#define MAX_EVENTS 10

स्थिर bool
is_fd_pollable (पूर्णाङ्क fd)
{
  संरचना epoll_event ev = { .events = EPOLLIN };
  cleanup_close पूर्णाङ्क epollfd = epoll_create1 (EPOLL_CLOEXEC);

  सचे (epollfd < 0)
    पटिनिवत्त true;

  पटिनिवत्त epoll_ctl (epollfd, EPOLL_CTL_ADD, fd, &ev) == 0 || errno != EPERM;
}

स्थिर पूर्णाङ्क
wait_for_process (संरचना wait_for_process_args *args, libcrun_error_t *err)
{
  cleanup_channel_fd_pair संरचना channel_fd_pair *from_terminal = रिक्त;
  cleanup_channel_fd_pair संरचना channel_fd_pair *to_terminal = रिक्त;
  पूर्णाङ्क ret, container_exit_code = 0, last_process;
  cleanup_close पूर्णाङ्क terminal_fd_from = -1;
  cleanup_close पूर्णाङ्क terminal_fd_to = -1;
  bool stdin_pollable = false, stdout_pollable = false;
  cleanup_close पूर्णाङ्क epollfd = -1;
  cleanup_close पूर्णाङ्क signalfd = -1;
  sigset_t mask;
  पूर्णाङ्क in_fds[MAX_EVENTS];
  पूर्णाङ्क in_fds_len = 0;
  पूर्णाङ्क out_fds[MAX_EVENTS];
  पूर्णाङ्क out_fds_len = 0;
  size_t i;

  cleanup_seccomp_notify_context संरचना seccomp_notify_context_s *seccomp_notify_ctx = रिक्त;

  container_exit_code = 0;

  सचे (args == रिक्त || args->context == रिक्त)
    पटिनिवत्त crun_make_error (err, 0, "internal error: context is empty");

  अत्थाय (i = 0; i < MAX_EVENTS; i++)
    {
      in_fds[i] = -1;
      out_fds[i] = -1;
    }

  सचे (args->context->pid_file)
    {
      ret = write_pid_file (args->context->pid_file, args->pid, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  /* Also exit if there is nothing more to wait for.  */
  सचे (args->context->detach && args->notify_socket < 0)
    पटिनिवत्त 0;

  सचे (args->container_ready_fd)
    {
      ret = 0;
      TEMP_FAILURE_RETRY (लिख (*args->container_ready_fd, &ret, परिमाण (ret)));
      close_and_reset (args->container_ready_fd);
    }

  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, रिक्त);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "sigprocmask");

  signalfd = create_signalfd (&mask, err);
  सचे (UNLIKELY (signalfd < 0))
    पटिनिवत्त signalfd;

  ret = reap_subprocesses (&args->pid, &container_exit_code, &last_process, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (last_process)
    पटिनिवत्त container_exit_code;

  सचे (args->seccomp_notify_fd >= 0)
    {
      cleanup_free वर्ण *state_मूलम् = रिक्त;
      cleanup_free वर्ण *oci_config_path = रिक्त;

      संरचना libcrun_load_seccomp_notify_conf_s conf;
      memset (&conf, 0, परिमाण conf);

      ret = libcrun_get_state_directory (&state_मूलम्, args->context->state_मूलम्, args->context->id, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = append_paths (&oci_config_path, err, state_मूलम्, "config.json", रिक्त);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      conf.runtime_मूलम्_path = state_मूलम्;
      conf.name = args->context->id;
      conf.bundle_path = args->context->bundle;
      conf.oci_config_path = oci_config_path;

      ret = set_blocking_fd (args->seccomp_notify_fd, false, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = libcrun_load_seccomp_notify_plugins (&seccomp_notify_ctx,
                                                 args->seccomp_notify_plugins,
                                                 &conf, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      in_fds[in_fds_len++] = args->seccomp_notify_fd;
    }

  सचे (args->terminal_fd >= 0)
    {
      /* The terminal_fd is dup()ed so that it can be registered with
         epoll multiple times using different masks.  */
      terminal_fd_from = dup (args->terminal_fd);
      सचे (UNLIKELY (terminal_fd_from < 0))
        पटिनिवत्त crun_make_error (err, errno, "dup terminal fd");
      terminal_fd_to = dup (args->terminal_fd);
      सचे (UNLIKELY (terminal_fd_to < 0))
        पटिनिवत्त crun_make_error (err, errno, "dup terminal fd");

      पूर्णाङ्क i, non_blocking_fds[] = { terminal_fd_from, terminal_fd_to, 0, 1, -1 };
      अत्थाय (i = 0; non_blocking_fds[i] >= 0; i++)
        {
          ret = set_blocking_fd (non_blocking_fds[i], false, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }

      /* stdin and stdout can be something epoll does not support, such as
         /dev/null or a regular file.  Such a stdin is not forwarded, and
         such a stdout is written to without waiting for it to be ready.  */
      stdin_pollable = is_fd_pollable (0);
      stdout_pollable = is_fd_pollable (1);

      from_terminal = channel_fd_pair_new (terminal_fd_from, 1, BUFSIZ);
      सचे (stdin_pollable)
        to_terminal = channel_fd_pair_new (0, terminal_fd_to, BUFSIZ);
    }

  in_fds[in_fds_len++] = signalfd;
  सचे (args->notify_socket >= 0)
    in_fds[in_fds_len++] = args->notify_socket;
  सचे (args->terminal_fd >= 0)
    {
      सचे (stdin_pollable)
        {
          in_fds[in_fds_len++] = 0;
          out_fds[out_fds_len++] = terminal_fd_to;
        }

      in_fds[in_fds_len++] = terminal_fd_from;
      सचे (stdout_pollable)
        out_fds[out_fds_len++] = 1;
    }

  epollfd = epoll_helper (in_fds, रिक्त, out_fds, रिक्त, err);
  सचे (UNLIKELY (epollfd < 0))
    पटिनिवत्त epollfd;

  याव (1)
    {
      संरचना epoll_event events[MAX_EVENTS];
      संरचना signalfd_siginfo si;
      संरचना winsize ws;
      पूर्णाङ्क i, nr_events;
      ssize_t res;

      nr_events = TEMP_FAILURE_RETRY (epoll_wait (epollfd, events, MAX_EVENTS, -1));
      सचे (UNLIKELY (nr_events < 0))
        पटिनिवत्त crun_make_error (err, errno, "epoll_wait");

      अत्थाय (i = 0; i < nr_events; i++)
        {
          सचे (events[i].data.fd == 0 || events[i].data.fd == terminal_fd_to)
            {
              ret = channel_fd_pair_process (to_terminal, epollfd, err);
              सचे (UNLIKELY (ret < 0))
                पटिनिवत्त crun_error_wrap (err, "copy to terminal fd");
            }
          अञ्ञथा सचे (events[i].data.fd == 1 || events[i].data.fd == terminal_fd_from)
            {
              ret = channel_fd_pair_process (from_terminal, epollfd, err);
              सचे (UNLIKELY (ret < 0))
                पटिनिवत्त crun_error_wrap (err, "copy from terminal fd");
            }
          अञ्ञथा सचे (events[i].data.fd == args->seccomp_notify_fd)
            {
              ret = libcrun_seccomp_notify_plugins (seccomp_notify_ctx,
                                                    args->seccomp_notify_fd, err);
              सचे (UNLIKELY (ret < 0))
                पटिनिवत्त ret;
            }
          अञ्ञथा सचे (events[i].data.fd == args->notify_socket)
            {
              ret = handle_notify_socket (args->notify_socket, err);
              सचे (UNLIKELY (ret < 0))
                पटिनिवत्त ret;
              सचे (ret && args->context->detach)
                पटिनिवत्त 0;
            }
          अञ्ञथा सचे (events[i].data.fd == signalfd)
            {
              res = TEMP_FAILURE_RETRY (पठ (signalfd, &si, परिमाण (si)));
              सचे (UNLIKELY (res < 0))
                पटिनिवत्त crun_make_error (err, errno, "read from signalfd");
              सचे (si.ssi_signo == SIGCHLD)
                {
                  ret = reap_subprocesses (&args->pid, &container_exit_code,
                                           &last_process, err);
                  सचे (UNLIKELY (ret < 0))
                    पटिनिवत्त ret;
                  सचे (last_process)
                    पटिनिवत्त container_exit_code;
                }
              अञ्ञथा सचे (si.ssi_signo == SIGWINCH)
                {
                  /* Ignore the signal if the terminal is not available.  */
                  सचे (args->terminal_fd > 0)
                    {
                      ret = ioctl (0, TIOCGWINSZ, &ws);
                      सचे (UNLIKELY (ret < 0))
                        पटिनिवत्त crun_make_error (err, errno, "ioctl TIOCGWINSZ copy terminal size from stdin");

                      ret = ioctl (args->terminal_fd, TIOCSWINSZ, &ws);
                      सचे (UNLIKELY (ret < 0))
                        पटिनिवत्त crun_make_error (err, errno, "ioctl TIOCSWINSZ copy terminal size to pty");
                    }
                }
              अञ्ञथा सचे (args->pid > 0)
                {
                  /* Send any other signal to the child process, if it was not reaped yet.  */
                  ret = kill (args->pid, si.ssi_signo);
                }
            }
          अञ्ञथा
            {
              पटिनिवत्त crun_make_error (err, 0, "internal error: unknown fd from epoll_wait");
            }
        }
    }

  पटिनिवत्त 0;
}

स्थिर शून्य
flush_fd_to_err (libcrun_context_t *context, पूर्णाङ्क terminal_fd)
{
  वर्ण buf[256];
  पूर्णाङ्क flags;

  सचे (terminal_fd < 0 || stderr == रिक्त)
    पटिनिवत्त;

  flags = fcntl (terminal_fd, F_GETFL, 0);
  सचे (flags == -1)
    पटिनिवत्त;
  सचे (fcntl (terminal_fd, F_SETFL, flags | O_NONBLOCK) < 0)
    पटिनिवत्त;

  अत्थाय (;;)
    {
      पूर्णाङ्क ret = TEMP_FAILURE_RETRY (पठ (terminal_fd, buf, परिमाण (buf) - 1));
      सचे (ret <= 0)
        भङ्ग;
      buf[ret] = '\0';
      सचे (context->output_handler)
        context->output_handler (0, buf, LIBCRUN_VERBOSITY_ERROR, context->output_handler_arg);
    }
  (शून्य) fcntl (terminal_fd, F_SETFL, flags);
  fflush (stderr);
  fsync (1);
  fsync (2);
}

स्थिर पूर्णाङ्क
cleanup_watch (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
               संरचना libcrun_cgroup_status *cgroup_status, pid_t init_pid, पूर्णाङ्क sync_socket,
               पूर्णाङ्क terminal_fd, libcrun_error_t *err)
{
  ध्रुव वर्ण *oom_message = रिक्त;
  libcrun_error_t tmp_err = रिक्त;
  पूर्णाङ्क ret;

  सचे (init_pid)
    {
      /* Try to detect whether the cgroup has a OOM.  */
      सचे (cgroup_status)
        {
          पूर्णाङ्क has_oom;

          has_oom = libcrun_cgroup_has_oom (cgroup_status, &tmp_err);
          सचे (has_oom > 0)
            oom_message = "OOM: the memory limit could be too low";
          अञ्ञथा सचे (has_oom < 0)
            {
              /* If the detection has failed for any reason, e.g. the cgroup was
                 already deleted by the time it was checked, just ignore the
                 failure.  */
              crun_error_release (&tmp_err);
            }
        }
      /* If the OOM wasn't detected, look into the static configuration.  */
      सचे (oom_message == रिक्त && is_memory_limit_too_नीचः (def))
        oom_message = "the memory limit could be too low";

      kill (init_pid, SIGKILL);
      waitpid_ignore_stopped (init_pid, रिक्त, 0);
    }

  ret = sync_socket_wait_sync (context, sync_socket, true, &tmp_err);
  सचे (UNLIKELY (ret < 0))
    {
      crun_error_release (err);
      *err = tmp_err;
    }

  सचे (terminal_fd >= 0)
    flush_fd_to_err (context, terminal_fd);

  सचे (oom_message)
    पटिनिवत्त crun_error_wrap (err, "%s", oom_message);

  पटिनिवत्त -1;
}

/* Find the uid:gid that is mapped to root inside the container user namespace.  */
शून्य
get_मूलम्_in_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid, uid_t *uid,
                        gid_t *gid)
{
  *uid = -1;
  *gid = -1;

  /* Not root in the namespace.  */
  सचे (host_uid)
    पटिनिवत्त;

  सचे (def->linux && def->linux->uid_mappings)
    {
      size_t i;

      अत्थाय (i = 0; i < def->linux->uid_mappings_len; i++)
        सचे (def->linux->uid_mappings[i]->container_id == 0)
          {
            *uid = def->linux->uid_mappings[i]->host_id;
            भङ्ग;
          }
    }
  सचे (def->linux && def->linux->gid_mappings)
    {
      size_t i;

      अत्थाय (i = 0; i < def->linux->gid_mappings_len; i++)
        सचे (def->linux->gid_mappings[i]->container_id == 0)
          {
            *gid = def->linux->gid_mappings[i]->host_id;
            भङ्ग;
          }
    }

  /* If the uid and the gid are not changed, do not attempt any chown.  */
  सचे (*uid == host_uid && *gid == host_gid)
    *uid = *gid = -1;
}

स्थिर पूर्णाङ्क
get_seccomp_receiver_fd (libcrun_container_t *container, पूर्णाङ्क *fd, पूर्णाङ्क *self_receiver_fd, ध्रुव वर्ण **plugins,
                         libcrun_error_t *err)
{
  ध्रुव वर्ण *tmp;
  runtime_spec_schema_config_schema *def = container->container_def;

  *fd = -1;
  *self_receiver_fd = -1;

  tmp = find_annotation (container, "run.oci.seccomp.plugins");
  सचे (tmp)
    {
      पूर्णाङ्क fds[2];
      पूर्णाङ्क ret;

      ret = create_socket_pair (fds, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_error_wrap (err, "create socket pair");

      *fd = fds[0];
      *self_receiver_fd = fds[1];
      *plugins = tmp;
      पटिनिवत्त 0;
    }

  सचे (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    tmp = def->linux->seccomp->listener_path;
  अञ्ञथा
    tmp = find_annotation (container, "run.oci.seccomp.receiver");
  सचे (tmp == रिक्त)
    tmp = getenv ("RUN_OCI_SECCOMP_RECEIVER");
  सचे (tmp)
    {
      सचे (tmp[0] != '/')
        पटिनिवत्त crun_make_error (err, 0, "the seccomp receiver `%s` is not an absolute path", tmp);

      *fd = open_unix_domain_client_socket (tmp, 0, err);
      सचे (UNLIKELY (*fd < 0))
        पटिनिवत्त crun_error_wrap (err, "open seccomp receiver");
    }

  पटिनिवत्त 0;
}

स्थिर bool
has_seccomp_receiver (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;

  सचे (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    पटिनिवत्त true;

  सचे (find_annotation (container, "run.oci.seccomp.receiver") != रिक्त || getenv ("RUN_OCI_SECCOMP_RECEIVER") != रिक्त)
    पटिनिवत्त true;

  पटिनिवत्त false;
}

स्थिर पूर्णाङ्क
setup_container_hooks_output (libcrun_container_t *container, runtime_spec_schema_config_schema *def,
                              संरचना container_entrypoint_s *container_args, पूर्णाङ्क *hooks_out_fd,
                              पूर्णाङ्क *hooks_err_fd, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  सचे (def->hooks
      && (def->hooks->prestart_len || def->hooks->poststart_len || def->hooks->create_runtime_len
          || def->hooks->create_container_len || def->hooks->start_container_len))
    {
      ret = open_hooks_output (container, hooks_out_fd, hooks_err_fd, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
      container_args->hooks_out_fd = *hooks_out_fd;
      container_args->hooks_err_fd = *hooks_err_fd;
    }
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
setup_container_keyring (libcrun_container_t *container, libcrun_context_t *context, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णाङ्क ret;

  सचे (! context->no_new_keyring)
    {
      ध्रुव वर्ण *label = रिक्त;

      libcrun_debug ("Creating new keyring");

      सचे (def->process)
        {
          label = def->process->selinux_label;
          सचे (label)
            libcrun_debug ("Using SELinux process label: `%s`", label);
        }

      ret = libcrun_create_keyring (container, container->context->id, label, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
setup_terminal_socket_pair (libcrun_container_t *container, libcrun_context_t *context,
                            संरचना container_entrypoint_s *container_args, पूर्णाङ्क *socket_pair_0,
                            पूर्णाङ्क *socket_pair_1, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णाङ्क ret;
  पूर्णाङ्क detach = context->detach;

  सचे (def->process && def->process->terminal && ! detach && context->console_socket == रिक्त)
    {
      libcrun_debug ("Creating terminal socket pair");
      container_args->has_terminal_socket_pair = 1;
      ret = create_socket_pair (container_args->terminal_socketpair, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_error_wrap (err, "create terminal socket");

      *socket_pair_0 = container_args->terminal_socketpair[0];
      *socket_pair_1 = container_args->terminal_socketpair[1];
    }
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
setup_seccomp (libcrun_container_t *container, ध्रुव वर्ण *seccomp_bpf_data,
               संरचना libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, पूर्णाङ्क *seccomp_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णाङ्क ret;

  सचे (find_annotation (container, "run.oci.seccomp.plugins") != रिक्त && has_seccomp_receiver (container))
    {
      पटिनिवत्त crun_make_error (err, 0, "seccomp plugins and seccomp receivers cannot be declared at the same time");
    }

  सचे (def->linux && (def->linux->seccomp || seccomp_bpf_data))
    {
      unsigned पूर्णाङ्क seccomp_gen_options = 0;
      ध्रुव वर्ण *annotation;

      libcrun_debug ("Initializing seccomp");
      annotation = find_annotation (container, "run.oci.seccomp_fail_unknown_syscall");
      सचे (annotation && strcmp (annotation, "0") != 0)
        seccomp_gen_options = LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL;

      सचे (seccomp_bpf_data)
        seccomp_gen_options |= LIBCRUN_SECCOMP_SKIP_CACHE;

      libcrun_seccomp_gen_ctx_init (seccomp_gen_ctx, container, true, seccomp_gen_options);

      ret = libcrun_open_seccomp_bpf (seccomp_gen_ctx, seccomp_fd, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
setup_console_socket (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
                      संरचना container_entrypoint_s *container_args, पूर्णाङ्क *console_socket_fd,
                      libcrun_error_t *err)
{
  सचे (context->console_socket && def->process && def->process->terminal)
    {
      *console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
      सचे (UNLIKELY (*console_socket_fd < 0))
        पटिनिवत्त crun_error_wrap (err, "open console socket");
      container_args->console_socket_fd = *console_socket_fd;
    }
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
setup_cgroup_manager (libcrun_context_t *context, libcrun_container_t *container,
                      संरचना libcrun_cgroup_args *cg, पूर्णाङ्क *cgroup_dirfd,
                      संरचना libcrun_dirfd_s *cgroup_dirfd_s, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णाङ्क cgroup_manager;
  uid_t मूलम्_uid = -1;
  gid_t मूलम्_gid = -1;
  पूर्णाङ्क ret;

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  सचे (context->systemd_cgroup)
    {
      libcrun_debug ("Using systemd cgroup manager");
      cgroup_manager = CGROUP_MANAGER_SYSTEMD;
    }
  अञ्ञथा सचे (context->force_no_cgroup)
    {
      libcrun_debug ("Disabling cgroup manager");
      cgroup_manager = CGROUP_MANAGER_DISABLED;
    }
  अञ्ञथा
    libcrun_debug ("Using cgroupfs cgroup manager");

  /* If we are root (either on the host or in a namespace), then chown the cgroup to root
     in the container user namespace.  */
  get_मूलम्_in_the_userns (def, container->host_uid, container->host_gid, &मूलम्_uid, &मूलम्_gid);
  libcrun_debug ("Using container host UID `%d` and GID `%d`", container->host_uid, container->host_gid);

  memset (cg, 0, परिमाण (*cg));

  cg->cgroup_path = def->linux ? def->linux->cgroups_path : "";
  cg->manager = cgroup_manager;
  cg->id = context->id;
  cg->resources = def->linux ? def->linux->resources : रिक्त;
  cg->annotations = container->annotations;
  cg->मूलम्_uid = मूलम्_uid;
  cg->मूलम्_gid = मूलम्_gid;
  cg->state_मूलम् = context->state_मूलम्;
  cg->container = container;

  ret = libcrun_cgroup_preenter (cg, cgroup_dirfd, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  cgroup_dirfd_s->dirfd = cgroup_dirfd;
  cgroup_dirfd_s->joined = false;

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
set_scheduler (pid_t pid, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = libcrun_set_scheduler (pid, def->process, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = libcrun_reset_cpu_affinity_mask (pid, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = libcrun_set_io_priority (pid, def->process, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
seccomp_generation (पूर्णाङ्क seccomp_fd, ध्रुव वर्ण *seccomp_bpf_data,
                    संरचना libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  सचे (seccomp_fd >= 0)
    {
      सचे (seccomp_bpf_data != रिक्त)
        {
          ret = libcrun_copy_seccomp (seccomp_gen_ctx, seccomp_bpf_data, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }
      अञ्ञथा
        {
          ret = libcrun_generate_seccomp (seccomp_gen_ctx, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
        }
    }
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
terminal_setup (runtime_spec_schema_config_schema *def, libcrun_context_t *context,
                पूर्णाङ्क socket_pair_0, पूर्णाङ्क *terminal_fd, शून्य **orig_terminal, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  पूर्णाङ्क detach = context->detach;

  सचे (def->process && def->process->terminal && ! detach && context->console_socket == रिक्त)
    {
      libcrun_debug ("Receiving console socket fd");
      *terminal_fd = receive_fd_from_socket (socket_pair_0, err);
      सचे (UNLIKELY (*terminal_fd < 0))
        पटिनिवत्त -1;

      ret = libcrun_clear_onlcr (*terminal_fd, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = libcrun_set_raw (0, orig_terminal, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
libcrun_container_run_internal (libcrun_container_t *container, libcrun_context_t *context,
                                पूर्णाङ्क *container_ready_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णाङ्क ret;
  pid_t pid;
  पूर्णाङ्क detach = context->detach;
  cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = रिक्त;
  cleanup_close पूर्णाङ्क terminal_fd = -1;
  cleanup_terminal शून्य *orig_terminal = रिक्त;
  cleanup_close पूर्णाङ्क sync_socket = -1;
  cleanup_close पूर्णाङ्क notify_socket = -1;
  cleanup_close पूर्णाङ्क socket_pair_0 = -1;
  cleanup_close पूर्णाङ्क socket_pair_1 = -1;
  cleanup_close पूर्णाङ्क seccomp_fd = -1;
  cleanup_close पूर्णाङ्क console_socket_fd = -1;
  cleanup_close पूर्णाङ्क hooks_out_fd = -1;
  cleanup_close पूर्णाङ्क hooks_err_fd = -1;
  cleanup_close पूर्णाङ्क own_seccomp_receiver_fd = -1;
  cleanup_close पूर्णाङ्क seccomp_notify_fd = -1;
  ध्रुव वर्ण *seccomp_notify_plugins = रिक्त;
  cleanup_cgroup_preenter संरचना libcrun_cgroup_args cg = {};
  cleanup_free वर्ण *bundle_allocated = रिक्त;
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
  cleanup_close पूर्णाङ्क cgroup_dirfd = -1;
  संरचना libcrun_dirfd_s cgroup_dirfd_s;
  संरचना libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  ध्रुव वर्ण *seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");
  पूर्णाङ्क cgroup_mode;

  सचे (! context->force_no_cgroup)
    {
      cgroup_mode = libcrun_get_cgroup_mode (err);
      सचे (UNLIKELY (cgroup_mode < 0))
        पटिनिवत्त cgroup_mode;

      सचे (cgroup_mode != CGROUP_MODE_UNIFIED)
        libcrun_warning ("cgroup v1 is deprecated and will be removed in a future release.  Use cgroup v2");
    }

  ret = setup_container_hooks_output (container, def, &container_args, &hooks_out_fd, &hooks_err_fd, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  container->context = context;

  सचे (! detach || context->notify_socket)
    {
      libcrun_debug ("Setting child subreaper");
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "prctl set child subreaper");
    }

  ret = setup_container_keyring (container, context, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = setup_terminal_socket_pair (container, context, &container_args, &socket_pair_0, &socket_pair_1, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = block_signals (err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  umask (0);

  ret = libcrun_set_mempolicy (def, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;
  container_args.seccomp_fd = seccomp_fd;

  सचे (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &container_args.seccomp_receiver_fd, &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  ret = setup_console_socket (context, def, &container_args, &console_socket_fd, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = setup_cgroup_manager (context, container, &cg, &cgroup_dirfd, &cgroup_dirfd_s, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = libcrun_configure_handler (container_args.context->handler_manager,
                                   container_args.context,
                                   container,
                                   &(container_args.custom_handler),
                                   err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (container_args.custom_handler && container_args.custom_handler->vtable->modify_oci_configuration)
    {
      libcrun_debug ("Using custom handler to modify OCI configuration");
      ret = container_args.custom_handler->vtable->modify_oci_configuration (container_args.custom_handler->cookie,
                                                                             container_args.context,
                                                                             container->container_def,
                                                                             err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  pid = libcrun_run_linux_container (container, container_init, &container_args, &sync_socket, &cgroup_dirfd_s, err);
  सचे (UNLIKELY (pid < 0))
    पटिनिवत्त pid;

  cg.pid = pid;
  cg.joined = cgroup_dirfd_s.joined;
  libcrun_debug ("Running container on PID: `%d`", pid);

  सचे (context->fifo_exec_wait_fd < 0 && context->notify_socket)
    {
      libcrun_debug ("Using notify socket: `%s`", context->notify_socket);
      /* Do not open the notify socket here on "create".  "start" will take care of it.  */
      ret = get_notify_fd (context, container, &notify_socket, err);
      सचे (UNLIKELY (ret < 0))
        गच्छ fail;
    }

  सचे (container_args.terminal_socketpair[1] >= 0)
    close_and_reset (&socket_pair_1);

  ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
  सचे (UNLIKELY (ret < 0))
    गच्छ fail;

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE, err);
  सचे (UNLIKELY (ret < 0))
    गच्छ fail;

  ret = libcrun_move_network_devices (container, pid, err);
  सचे (UNLIKELY (ret < 0))
    गच्छ fail;

  /* sync send own pid.  */
  ret = TEMP_FAILURE_RETRY (लिख (sync_socket, &pid, परिमाण (pid)));
  सचे (UNLIKELY (ret != परिमाण (pid)))
    {
      सचे (ret >= 0)
        errno = 0;
      crun_make_error (err, errno, "write to sync socket");
      गच्छ fail;
    }

  /* sync 1.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  सचे (UNLIKELY (ret < 0))
    गच्छ fail;

  /* sync 2.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  सचे (UNLIKELY (ret < 0))
    गच्छ fail;

  ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
  सचे (UNLIKELY (ret < 0))
    गच्छ fail;

  /* If the container process is created later, the scheduler is set on it then.  */
  सचे (! libcrun_container_delayed_pidns_fork (container))
    {
      ret = set_scheduler (pid, def, err);
      सचे (UNLIKELY (ret < 0))
        गच्छ fail;
    }

  /* The container is waiting that we write back.  In this phase we can launch the
     prestart hooks.  */
  सचे (def->hooks && def->hooks->prestart_len)
    {
      libcrun_debug ("Running `prestart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "prestart", (hook **) def->hooks->prestart,
                      def->hooks->prestart_len, hooks_out_fd, hooks_err_fd, false, err);
      सचे (UNLIKELY (ret != 0))
        गच्छ fail;
    }
  सचे (def->hooks && def->hooks->create_runtime_len)
    {
      libcrun_debug ("Running `create` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "createRuntime", (hook **) def->hooks->create_runtime,
                      def->hooks->create_runtime_len, hooks_out_fd, hooks_err_fd, false, err);
      सचे (UNLIKELY (ret != 0))
        गच्छ fail;
    }

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  सचे (UNLIKELY (ret < 0))
    गच्छ fail;
  close_and_reset (&seccomp_fd);

  /* sync 3.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  सचे (UNLIKELY (ret < 0))
    गच्छ fail;

  /* The process joining an existing PID namespace is created once the container root is in place.  */
  सचे (libcrun_container_delayed_pidns_fork (container))
    {
      pid_t new_pid = 0;

      ret = sync_socket_wait_sync (context, sync_socket, false, err);
      सचे (UNLIKELY (ret < 0))
        गच्छ fail;

      ret = TEMP_FAILURE_RETRY (पठ (sync_socket, &new_pid, परिमाण (new_pid)));
      सचे (UNLIKELY (ret != परिमाण (new_pid)))
        {
          सचे (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "read pid from sync socket");
          गच्छ fail;
        }

      /* Cleanup the first process.  */
      waitpid_ignore_stopped (pid, रिक्त, 0);

      pid = new_pid;

      ret = set_scheduler (pid, def, err);
      सचे (UNLIKELY (ret < 0))
        गच्छ fail;

      ret = libcrun_handle_pidfd_receiver (pid, container, err);
      सचे (UNLIKELY (ret < 0))
        गच्छ fail;

      /* sync send own pid.  */
      ret = TEMP_FAILURE_RETRY (लिख (sync_socket, &pid, परिमाण (pid)));
      सचे (UNLIKELY (ret != परिमाण (pid)))
        {
          सचे (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "write to sync socket");
          गच्छ fail;
        }
    }

  ret = terminal_setup (def, context, socket_pair_0, &terminal_fd, &orig_terminal, err);
  सचे (UNLIKELY (ret < 0))
    गच्छ fail;

  close_and_reset (&socket_pair_0);

  /* sync 4.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  सचे (UNLIKELY (ret < 0))
    गच्छ fail;

  ret = close_and_reset (&sync_socket);
  सचे (UNLIKELY (ret < 0))
    {
      crun_make_error (err, errno, "close sync_socket failed");
      गच्छ fail;
    }

  libcrun_debug ("Writing container status");
  ret = write_container_status (container, context, pid, cgroup_status, err);
  सचे (UNLIKELY (ret < 0))
    गच्छ fail;

  /* Run poststart hooks here only if the container is created using "run".  For create+start, the
     hooks will be executed as part of the start command.  */
  सचे (context->fifo_exec_wait_fd < 0 && def->hooks && def->hooks->poststart_len)
    {
      libcrun_debug ("Running `poststart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      सचे (UNLIKELY (ret != 0))
        गच्छ fail;
    }

  /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
  सचे (own_seccomp_receiver_fd >= 0)
    {
      libcrun_debug ("Receiving seccomp fd");
      seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
      सचे (UNLIKELY (seccomp_notify_fd < 0))
        गच्छ fail;

      ret = close_and_reset (&own_seccomp_receiver_fd);
      सचे (UNLIKELY (ret < 0))
        {
          crun_make_error (err, errno, "close seccomp receiver fd failed");
          गच्छ fail;
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
  सचे (! context->detach)
    {
      libcrun_error_t tmp_err = रिक्त;
      cleanup_watch (context, def, cgroup_status, 0, sync_socket, terminal_fd, &tmp_err);
      crun_error_release (&tmp_err);
    }

  पटिनिवत्त ret;

fail:
  ret = cleanup_watch (context, def, cgroup_status, pid, sync_socket, terminal_fd, err);
  सचे (cgroup_status)
    {
      libcrun_error_t tmp_err = रिक्त;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  पटिनिवत्त ret;
}

स्थिर पूर्णाङ्क
check_config_file (runtime_spec_schema_config_schema *def, libcrun_context_t *context, libcrun_error_t *err)
{
  सचे (UNLIKELY (def->linux == रिक्त))
    पटिनिवत्त crun_make_error (err, 0, "invalid config file, no `linux` block specified");

  सचे (context->handler == रिक्त)
    {
      सचे (UNLIKELY (def->मूलम् == रिक्त))
        पटिनिवत्त crun_make_error (err, 0, "invalid config file, no `root` block specified");
      सचे (UNLIKELY (def->mounts == रिक्त))
        पटिनिवत्त crun_make_error (err, 0, "invalid config file, no `mounts` block specified");
    }
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
libcrun_copy_config_file (ध्रुव वर्ण *id, ध्रुव वर्ण *state_मूलम्, libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_free वर्ण *dest_path = रिक्त;
  cleanup_free वर्ण *dir = रिक्त;
  cleanup_free वर्ण *buffer = रिक्त;
  size_t len;

  ret = libcrun_get_state_directory (&dir, state_मूलम्, id, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = append_paths (&dest_path, err, dir, "config.json", रिक्त);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (container->config_file == रिक्त && container->config_file_content == रिक्त)
    पटिनिवत्त crun_make_error (err, 0, "config file not specified");

  सचे (container->config_file == रिक्त)
    {
      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, container->config_file_content, strlen (container->config_file_content), err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }
  अञ्ञथा
    {
      libcrun_debug ("Reading config file: `%s`", container->config_file);
      ret = read_all_file (container->config_file, &buffer, &len, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, buffer, len, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  पटिनिवत्त 0;
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
write_error_to_pipe (पूर्णाङ्क fd, libcrun_error_t *err)
{
  ध्रुव वर्ण *सन्देशः = (*err)->सन्देशः;
  पूर्णाङ्क errcode = crun_error_get_errno (err);

  TEMP_FAILURE_RETRY (लिख (fd, &errcode, परिमाण (errcode)));
  TEMP_FAILURE_RETRY (लिख (fd, सन्देशः, strlen (सन्देशः) + 1));
}

पूर्णाङ्क
libcrun_container_run (libcrun_context_t *context, libcrun_container_t *container, unsigned पूर्णाङ्क options,
                       libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णाङ्क ret;
  पूर्णाङ्क detach = context->detach;
  पूर्णाङ्क container_ret_status[2];
  cleanup_close पूर्णाङ्क pipefd0 = -1;
  cleanup_close पूर्णाङ्क pipefd1 = -1;
  libcrun_error_t tmp_err = रिक्त;

  container->context = context;

  ret = validate_options (options, LIBCRUN_RUN_OPTIONS_PREFORK | LIBCRUN_RUN_OPTIONS_KEEP, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = check_config_file (def, context, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (def->process && def->process->terminal && detach && context->console_socket == रिक्त)
    पटिनिवत्त crun_make_error (err, 0, "use --console-socket with --detach when a terminal is used");

  ret = libcrun_status_check_directories (context->state_मूलम्, context->id, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (! detach && (options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      ret = libcrun_copy_config_file (context->id, context->state_मूलम्, container, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = libcrun_container_run_internal (container, context, रिक्त, err);
      सचे (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
        force_delete_container_status (context, container, def);
      पटिनिवत्त ret;
    }

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  ret = fork ();
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "fork");
  सचे (ret)
    {
      पूर्णाङ्क status;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, रिक्त, 0);

      ret = TEMP_FAILURE_RETRY (पठ (pipefd0, &status, परिमाण (status)));
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, errno, "read from sync pipe");

      सचे (status < 0)
        {
          पूर्णाङ्क errno_;
          वर्ण buf[512];
          ret = TEMP_FAILURE_RETRY (पठ (pipefd0, &errno_, परिमाण (errno_)));
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "read from sync pipe");

          ret = TEMP_FAILURE_RETRY (पठ (pipefd0, buf, परिमाण (buf) - 1));
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "read from sync pipe");
          buf[ret] = '\0';

          पटिनिवत्त crun_make_error (err, errno_, "%s", buf);
        }

      पटिनिवत्त status;
    }

  close_and_reset (&pipefd0);

  /* forked process.  */
  ret = detach_process ();
  सचे (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_मूलम्, container, &tmp_err);
  सचे (UNLIKELY (ret < 0))
    गच्छ fail;

  ret = libcrun_container_run_internal (container, context, रिक्त, &tmp_err);
  TEMP_FAILURE_RETRY (लिख (pipefd1, &ret, परिमाण (ret)));
  सचे (UNLIKELY (ret < 0))
    गच्छ fail;

  exit (EXIT_SUCCESS);
fail:

  सचे (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
    force_delete_container_status (context, container, def);
  सचे (tmp_err)
    {
      write_error_to_pipe (pipefd1, &tmp_err);
      crun_error_release (&tmp_err);
    }

  exit (EXIT_FAILURE);
}

पूर्णाङ्क
libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container, unsigned पूर्णाङ्क options,
                          libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णाङ्क ret;
  पूर्णाङ्क container_ready_pipe[2];
  cleanup_close पूर्णाङ्क pipefd0 = -1;
  cleanup_close पूर्णाङ्क pipefd1 = -1;
  cleanup_close पूर्णाङ्क exec_fifo_fd = -1;
  context->detach = 1;

  libcrun_debug ("Creating container: `%s`", context->id);
  container->context = context;

  ret = validate_options (options, LIBCRUN_CREATE_OPTIONS_PREFORK, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = check_config_file (def, context, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (def->process && def->process->terminal && context->console_socket == रिक्त)
    पटिनिवत्त crun_make_error (err, 0, "use --console-socket with create when a terminal is used");

  ret = libcrun_status_check_directories (context->state_मूलम्, context->id, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  exec_fifo_fd = libcrun_status_create_exec_fifo (context->state_मूलम्, context->id, err);
  सचे (UNLIKELY (exec_fifo_fd < 0))
    पटिनिवत्त exec_fifo_fd;

  context->fifo_exec_wait_fd = exec_fifo_fd;
  exec_fifo_fd = -1;

  सचे ((options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      libcrun_debug ("Running with prefork enabled");
      ret = libcrun_copy_config_file (context->id, context->state_मूलम्, container, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
      ret = libcrun_container_run_internal (container, context, रिक्त, err);
      सचे (UNLIKELY (ret < 0))
        force_delete_container_status (context, container, def);
      पटिनिवत्त ret;
    }

  ret = pipe2 (container_ready_pipe, O_CLOEXEC);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "pipe");
  pipefd0 = container_ready_pipe[0];
  pipefd1 = container_ready_pipe[1];

  ret = fork ();
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "fork");
  सचे (ret)
    {
      पूर्णाङ्क exit_code;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, रिक्त, 0);

      ret = TEMP_FAILURE_RETRY (पठ (pipefd0, &exit_code, परिमाण (exit_code)));
      सचे (UNLIKELY (ret <= 0))
        पटिनिवत्त crun_make_error (err, ret < 0 ? errno : 0, "waiting for container to be ready");

      सचे (ret != परिमाण (exit_code) || exit_code < 0)
        पटिनिवत्त crun_make_error (err, 0, "internal error: read invalid exit code");

      सचे (exit_code == 0)
        पटिनिवत्त 0;

      libcrun_debug ("Exit code is `%d`, deleting container", exit_code);
      libcrun_error_t tmp_err = रिक्त;
      container_delete_internal (context, रिक्त, def, context->id, true, true, &tmp_err);
      libcrun_error_report_and_release (&tmp_err);
      पटिनिवत्त crun_make_error (err, 0, "error creating container");
    }

  /* forked process.  */
  ret = detach_process ();
  सचे (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_मूलम्, container, err);
  सचे (UNLIKELY (ret < 0))
    {
      पूर्णाङ्क errcode = crun_error_get_errno (err);
      crun_error_release (err);
      libcrun_fail_with_error (errcode, "copy config file");
    }

  ret = libcrun_container_run_internal (container, context, &pipefd1, err);
  सचे (UNLIKELY (ret < 0))
    {
      force_delete_container_status (context, container, def);
      libcrun_error ((*err)->status, "%s", (*err)->सन्देशः);
      crun_error_release (err);
      crun_set_output_handler (log_write_to_stderr, रिक्त);
    }

  सचे (pipefd1 >= 0)
    TEMP_FAILURE_RETRY (लिख (pipefd1, &ret, परिमाण (ret)));
  exit (ret ? EXIT_FAILURE : 0);
}

पूर्णाङ्क
libcrun_container_start (libcrun_context_t *context, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = रिक्त;
  ध्रुव वर्ण *state_मूलम् = context->state_मूलम्;
  runtime_spec_schema_config_schema *def;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क ret;

  ret = libcrun_read_container_status (&status, state_मूलम्, id, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = libcrun_is_container_running (&status, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (! ret)
    पटिनिवत्त crun_make_error (err, 0, "container `%s` is not running", id);

  ret = read_container_config_from_state (&container, state_मूलम्, id, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (context->notify_socket)
    {
      ret = get_notify_fd (context, container, &fd, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  ret = libcrun_status_write_exec_fifo (context->state_मूलम्, id, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  def = container->container_def;

  सचे (context->notify_socket)
    {
      सचे (fd >= 0)
        {
          fd_set read_set;

          याव (1)
            {
              संरचना timeval timeout = {
                .tv_sec = 0,
                .tv_usec = 10000,
              };
              FD_ZERO (&read_set);
              FD_SET (fd, &read_set);

              ret = select (fd + 1, &read_set, रिक्त, रिक्त, &timeout);
              सचे (UNLIKELY (ret < 0))
                पटिनिवत्त crun_make_error (err, errno, "select failed");
              सचे (ret)
                {
                  ret = handle_notify_socket (fd, err);
                  सचे (UNLIKELY (ret < 0))
                    पटिनिवत्त ret;
                  सचे (ret)
                    भङ्ग;
                }
              अञ्ञथा
                {
                  ret = libcrun_is_container_running (&status, err);
                  सचे (UNLIKELY (ret < 0))
                    पटिनिवत्त ret;
                  सचे (! ret)
                    पटिनिवत्त 0;
                }
            }
        }
    }

  /* The container is considered running only after we got the notification from the
     notify_socket, if any.  */
  सचे (def->hooks && def->hooks->poststart_len)
    {
      cleanup_close पूर्णाङ्क hooks_out_fd = -1;
      cleanup_close पूर्णाङ्क hooks_err_fd = -1;

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      ret = do_hooks (def, status.pid, context->id, false, status.bundle, status.bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      सचे (UNLIKELY (ret != 0))
        {
          libcrun_error_t tmp_err = रिक्त;
          container_delete_internal (context, container, def, id, true, true, &tmp_err);
          libcrun_error_report_and_release (&tmp_err);
          पटिनिवत्त ret;
        }
    }

  पटिनिवत्त 0;
}

पूर्णाङ्क
libcrun_get_container_state_string (ध्रुव वर्ण *id, libcrun_container_status_t *status, ध्रुव वर्ण *state_मूलम्,
                                    ध्रुव वर्ण **container_status, पूर्णाङ्क *running, libcrun_error_t *err)
{
  पूर्णाङ्क ret, has_fifo = 0;
  bool paused = false;

  ret = libcrun_is_container_running (status, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;
  *running = ret;

  सचे (*running)
    {
      ret = libcrun_status_has_read_exec_fifo (state_मूलम्, id, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
      has_fifo = ret;
    }

  सचे (*running && ! has_fifo)
    {
      cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = रिक्त;

      cgroup_status = libcrun_cgroup_make_status (status);

      ret = libcrun_cgroup_is_container_paused (cgroup_status, &paused, err);
      सचे (UNLIKELY (ret < 0))
        {
          /*
            The cgroup might have been cleaned up by the time we try to read it, ignore both
            ENOENT and ENODEV:
            - ENOENT: if the open(CGROUP_PATH) fails because the cgroup was deleted.
            - ENODEV: if the cgroup is deleted between the open and reading the freeze status.
          */

          errno = crun_error_get_errno (err);
          सचे (errno == ENOENT || errno == ENODEV)
            {
              crun_error_release (err);
              *container_status = "stopped";
              पटिनिवत्त 0;
            }

          पटिनिवत्त ret;
        }
    }

  सचे (! *running)
    *container_status = "stopped";
  अञ्ञथा सचे (has_fifo)
    *container_status = "created";
  अञ्ञथा सचे (paused)
    *container_status = "paused";
  अञ्ञथा
    *container_status = "running";

  पटिनिवत्त 0;
}

पूर्णाङ्क
libcrun_container_state_json (libcrun_context_t *context, ध्रुव वर्ण *id, वर्ण **out, libcrun_error_t *err)
{
  वर्ण *buffer = रिक्त;
  size_t len = 0;
  FILE *f;
  पूर्णाङ्क ret;

  f = open_memstream (&buffer, &len);
  सचे (f == रिक्त)
    पटिनिवत्त crun_make_error (err, errno, "open_memstream");

  ret = libcrun_container_state (context, id, f, err);
  fclose (f);
  सचे (UNLIKELY (ret < 0))
    {
      मुक्त (buffer);
      पटिनिवत्त ret;
    }

  *out = buffer;
  पटिनिवत्त 0;
}

पूर्णाङ्क
libcrun_container_state (libcrun_context_t *context, ध्रुव वर्ण *id, FILE *out, libcrun_error_t *err)
{
  ध्रुव वर्ण *ध्रुव OCI_CONFIG_VERSION = "1.0.0";
  libcrun_container_status_t status = {};
  ध्रुव वर्ण *state_मूलम् = context->state_मूलम्;
  ध्रुव वर्ण *container_status = रिक्त;
  json_gen_ctx *gen = रिक्त;
  ध्रुव वर्ण *buf;
  पूर्णाङ्क ret = 0;
  पूर्णाङ्क r = json_gen_status_ok;
  पूर्णाङ्क running;
  size_t len;

  ret = libcrun_read_container_status (&status, state_मूलम्, id, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = libcrun_get_container_state_string (id, &status, state_मूलम्, &container_status, &running, err);
  सचे (UNLIKELY (ret < 0))
    गच्छ exit;

  ret = 0;
  सचे (! json_gen_init (&gen, रिक्त))
    पटिनिवत्त crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));
  GEN_कुञ्चिका (gen, "ociVersion");
  GEN_STR (gen, OCI_CONFIG_VERSION);

  GEN_कुञ्चिका (gen, "id");
  GEN_STR (gen, id);

  GEN_कुञ्चिका (gen, "pid");
  GEN_OR_FAIL (map_int (gen, running ? status.pid : 0));

  GEN_कुञ्चिका (gen, "status");
  GEN_STR (gen, container_status);

  GEN_कुञ्चिका (gen, "bundle");
  GEN_STR (gen, status.bundle);

  GEN_कुञ्चिका (gen, "rootfs");
  GEN_STR (gen, status.rootfs);

  GEN_कुञ्चिका (gen, "created");
  GEN_STR (gen, status.created);

  सचे (status.scope)
    {
      GEN_कुञ्चिका (gen, "systemd-scope");
      GEN_STR (gen, status.scope);
    }

  सचे (status.owner)
    {
      GEN_कुञ्चिका (gen, "owner");
      GEN_STR (gen, status.owner);
    }

  {
    cleanup_container libcrun_container_t *container = रिक्त;

    ret = read_container_config_from_state (&container, state_मूलम्, id, err);
    सचे (UNLIKELY (ret < 0))
      गच्छ exit;

    सचे (container->container_def->annotations && container->container_def->annotations->len)
      GEN_OR_FAIL (gen_annotations (gen, container->container_def->annotations));
  }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  fprintf (out, "%s\n", buf);

exit:
  सचे (gen)
    json_gen_free (gen);
  libcrun_free_container_status (&status);
  पटिनिवत्त ret;

gen_error:
  ret = json_gen_error_to_crun_error (r, err);
  गच्छ exit;
}

पूर्णाङ्क
libcrun_container_exec (libcrun_context_t *context, ध्रुव वर्ण *id, runtime_spec_schema_config_schema_process *process,
                        libcrun_error_t *err)
{
  संरचना libcrun_container_exec_options_s opts;
  memset (&opts, 0, परिमाण (opts));

  opts.struct_size = परिमाण (opts);
  opts.process = process;

  पटिनिवत्त libcrun_container_exec_with_options (context, id, &opts, err);
}

पूर्णाङ्क
libcrun_container_exec_process_file (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *path, libcrun_error_t *err)
{
  संरचना libcrun_container_exec_options_s opts;
  memset (&opts, 0, परिमाण (opts));

  opts.struct_size = परिमाण (opts);
  opts.path = path;

  पटिनिवत्त libcrun_container_exec_with_options (context, id, &opts, err);
}

स्थिर पूर्णाङ्क
exec_from_process_json (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *process_json, ध्रुव वर्ण *cgroup,
                        bool merge_env, libcrun_error_t *err)
{
  संरचना parser_context parser_ctx = { .options = 0, .errfile = stderr };
  संरचना libcrun_container_exec_options_s opts;
  runtime_spec_schema_config_schema_process *process;
  parser_error parser_err = रिक्त;
  json_object *doc = रिक्त;
  पूर्णाङ्क ret;

  ret = parse_json_file (&doc, process_json, &parser_ctx, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  process = make_runtime_spec_schema_config_schema_process (doc, &parser_ctx, &parser_err);
  json_object_put (doc);
  सचे (process == रिक्त)
    {
      ret = crun_make_error (err, 0, "cannot parse process: `%s`", parser_err ? parser_err : "unknown error");
      मुक्त (parser_err);
      पटिनिवत्त ret;
    }

  memset (&opts, 0, परिमाण (opts));
  opts.struct_size = परिमाण (opts);
  opts.process = process;
  opts.cgroup = cgroup;
  opts.merge_env = merge_env;

  ret = libcrun_container_exec_with_options (context, id, &opts, err);
  free_runtime_spec_schema_config_schema_process (process);
  पटिनिवत्त ret;
}

पूर्णाङ्क
libcrun_container_exec_json (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *process_json, libcrun_error_t *err)
{
  पटिनिवत्त exec_from_process_json (context, id, process_json, रिक्त, false, err);
}

पूर्णाङ्क
libcrun_container_exec_with_options_json (libcrun_context_t *context, ध्रुव वर्ण *id,
                                          ध्रुव संरचना libcrun_exec_options_s *opts, libcrun_error_t *err)
{
  सचे (opts == रिक्त || opts->struct_size == 0)
    पटिनिवत्त crun_make_error (err, EINVAL, "invalid exec options");

  पटिनिवत्त exec_from_process_json (context, id, opts->process_json, opts->cgroup, opts->merge_env, err);
}

#define cleanup_process_schema __attribute__ ((cleanup (cleanup_process_schemap)))

स्थिर अन्तःस्थ शून्य
cleanup_process_schemap (runtime_spec_schema_config_schema_process **p)
{
  runtime_spec_schema_config_schema_process *process = *p;
  सचे (process)
    (शून्य) free_runtime_spec_schema_config_schema_process (process);
}

/* Build the seccomp receiver payload (when needed), apply the seccomp profile and
   close the seccomp file descriptors.  Used from exec_process_entrypoint both before
   and after the capabilities are set, depending on process->no_new_privileges.  */
स्थिर पूर्णाङ्क
apply_seccomp_for_exec (libcrun_container_t *container, pid_t own_pid, पूर्णाङ्क seccomp_fd,
                        पूर्णाङ्क seccomp_receiver_fd, वर्ण **seccomp_flags, size_t seccomp_flags_len,
                        libcrun_error_t *err)
{
  cleanup_free वर्ण *seccomp_fd_payload = रिक्त;
  size_t seccomp_fd_payload_len = 0;
  पूर्णाङ्क ret;

  सचे (seccomp_receiver_fd >= 0)
    {
      ret = get_seccomp_receiver_fd_payload (container, "running", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  ret = libcrun_apply_seccomp (seccomp_fd, seccomp_receiver_fd, seccomp_fd_payload,
                               seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  close_and_reset (&seccomp_fd);
  close_and_reset (&seccomp_receiver_fd);
  पटिनिवत्त 0;
}

स्थिर पूर्णाङ्क
exec_process_entrypoint (libcrun_context_t *context,
                         libcrun_container_t *container,
                         runtime_spec_schema_config_schema_process *process,
                         पूर्णाङ्क *pipefd1,
                         पूर्णाङ्क seccomp_fd,
                         पूर्णाङ्क seccomp_receiver_fd,
                         संरचना custom_handler_instance_s *custom_handler,
                         bool merge_env,
                         libcrun_error_t *err)
{
  runtime_spec_schema_config_schema_process_capabilities *capabilities = रिक्त;
  cleanup_free वर्ण *exec_path = रिक्त;
  uid_t container_uid;
  gid_t container_gid;
  ध्रुव वर्ण *cwd;
  bool chdir_done = false;
  size_t seccomp_flags_len = 0;
  वर्ण **seccomp_flags = रिक्त;
  pid_t own_pid = 0;
  size_t i;
  पूर्णाङ्क ret;

  container_uid = process->user ? process->user->uid : 0;
  container_gid = process->user ? process->user->gid : 0;

  TEMP_FAILURE_RETRY (पठ (*pipefd1, &own_pid, परिमाण (own_pid)));

  cwd = process->cwd ? process->cwd : "/";
  सचे (LIKELY (libcrun_safe_chdir (cwd, err) == 0))
    chdir_done = true;
  अञ्ञथा
    crun_error_release (err);

  ret = unblock_signals (err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = clearenv ();
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "clearenv");

  सचे ((merge_env || ! process->env_len) && container->container_def->process && container->container_def->process->env_len)
    {
      वर्ण *e;

      अत्थाय (i = 0; i < container->container_def->process->env_len; i++)
        {
          e = container->container_def->process->env[i];
          सचे (putenv (e) < 0)
            पटिनिवत्त crun_make_error (err, errno, "putenv `%s`", e);
        }
    }
  सचे (process->env_len)
    {
      अत्थाय (i = 0; i < process->env_len; i++)
        सचे (putenv (process->env[i]) < 0)
          पटिनिवत्त crun_make_error (err, errno, "putenv `%s`", process->env[i]);
    }

  सचे (is_empty_string (getenv ("HOME")))
    {
      ret = set_home_env (container_uid);
      सचे (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          सचे (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            पटिनिवत्त crun_make_error (err, errno, "setenv HOME");
        }
    }

  ret = libcrun_set_selinux_label (container, process, false, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = libcrun_set_apparmor_profile (container, process, false, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (container->container_def->linux && container->container_def->linux->seccomp)
    {
      seccomp_flags = container->container_def->linux->seccomp->flags;
      seccomp_flags_len = container->container_def->linux->seccomp->flags_len;
    }

  ret = find_executable (&exec_path, process->args[0], process->cwd, err);
  सचे (UNLIKELY (ret < 0))
    {
      सचे (crun_error_get_errno (err) == ENOENT)
        पटिनिवत्त ret;

      /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
         once the process switched to the use that runs in the container.  This might be necessary
         when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
         is not honored.  */
      crun_error_release (err);
    }

  सचे (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, false, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = libcrun_reopen_dev_null (err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (! process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  ret = libcrun_container_setgroups (container, process, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = maybe_chown_std_streams (container_uid, container_gid, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (process->capabilities)
    capabilities = process->capabilities;
  अञ्ञथा सचे (container->container_def->process)
    capabilities = container->container_def->process->capabilities;

  ret = libcrun_set_caps (capabilities, container_uid, container_gid, process->no_new_privileges,
                          custom_handler != रिक्त, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (UNLIKELY (exec_path == रिक्त))
    {
      ret = find_executable (&exec_path, process->args[0], process->cwd, err);
      सचे (UNLIKELY (ret < 0))
        {
          सचे (custom_handler == रिक्त || is_empty_string (process->args[0]))
            पटिनिवत्त ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          exec_path = xstrdup (process->args[0]);
        }
    }

  सचे (! chdir_done)
    {
      ret = libcrun_safe_chdir (cwd, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  सचे (process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  सचे (process->user)
    umask (process->user->umask_present ? process->user->umask : 0022);

  ret = 0;
  TEMP_FAILURE_RETRY (लिख (*pipefd1, &ret, परिमाण (ret)));
  TEMP_FAILURE_RETRY (close (*pipefd1));
  *pipefd1 = -1;

  सचे (custom_handler)
    {
      सचे (custom_handler->vtable->exec_func == रिक्त)
        पटिनिवत्त crun_make_error (err, 0, "the handler does not support exec");

      सचे (context->argv)
        erase_initial_environment (context->argv, context->argc);

      ret = custom_handler->vtable->exec_func (custom_handler->cookie,
                                               container,
                                               exec_path,
                                               process->args);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त crun_make_error (err, -ret, "exec container process failed with handler as `%s`", custom_handler->vtable->name);

      _safe_exit (EXIT_FAILURE);
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation, because the seccomp filter is already in place and it could
     stop some syscalls used by mark_or_close_fds_ge_than.
  */
  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, true, err);
  सचे (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, process->args));
  libcrun_fail_with_error (errno, "exec `%s`", exec_path);
}

/* Close the file descriptors inherited from the caller, other than those
   to be passed to the container process.  */
पूर्णाङ्क
libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err)
{
  पटिनिवत्त mark_or_close_fds_ge_than (रिक्त, context->preserve_fds + 3, true, err);
}

पूर्णाङ्क
libcrun_load_process_from_file (ध्रुव वर्ण *path, runtime_spec_schema_config_schema_process **process,
                                libcrun_error_t *err)
{
  संरचना parser_context ctx = { 0, stderr };
  cleanup_free वर्ण *content = रिक्त;
  parser_error parser_err = रिक्त;
  json_object *doc = रिक्त;
  size_t len;
  पूर्णाङ्क ret;

  ret = read_all_file (path, &content, &len, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  *process = make_runtime_spec_schema_config_schema_process (doc, &ctx, &parser_err);
  सचे (UNLIKELY (*process == रिक्त))
    {
      ret = crun_make_error (err, 0, "cannot parse process file: `%s`", parser_err);
      मुक्त (parser_err);
      सचे (doc)
        json_object_put (doc);
      पटिनिवत्त ret;
    }

  मुक्त (parser_err);
  सचे (doc)
    json_object_put (doc);

  पटिनिवत्त 0;
}

पूर्णाङ्क
libcrun_container_exec_with_options (libcrun_context_t *context, ध्रुव वर्ण *id,
                                     संरचना libcrun_container_exec_options_s *opts,
                                     libcrun_error_t *err)
{
  cleanup_custom_handler_instance संरचना custom_handler_instance_s *custom_handler = रिक्त;
  पूर्णाङ्क container_status, ret;
  bool container_paused = false;
  pid_t pid;
  libcrun_container_status_t status = {};
  ध्रुव वर्ण *state_मूलम् = context->state_मूलम्;
  cleanup_close पूर्णाङ्क terminal_fd = -1;
  cleanup_close पूर्णाङ्क seccomp_fd = -1;
  cleanup_terminal शून्य *orig_terminal = रिक्त;
  cleanup_container libcrun_container_t *container = रिक्त;
  पूर्णाङ्क container_ret_status[2];
  cleanup_close पूर्णाङ्क pipefd0 = -1;
  cleanup_close पूर्णाङ्क pipefd1 = -1;
  cleanup_close पूर्णाङ्क seccomp_receiver_fd = -1;
  cleanup_close पूर्णाङ्क own_seccomp_receiver_fd = -1;
  cleanup_close पूर्णाङ्क seccomp_notify_fd = -1;
  ध्रुव वर्ण *seccomp_notify_plugins = रिक्त;
  __attribute__ ((unused)) cleanup_process_schema runtime_spec_schema_config_schema_process *process_cleanup = रिक्त;
  runtime_spec_schema_config_schema_process *process = opts->process;
  संरचना libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  पूर्णाङ्क ret_from_child = 0;

  ret = libcrun_read_container_status (&status, state_मूलम्, id, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = libcrun_is_container_running (&status, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;
  container_status = ret;

  ret = read_container_config_from_state (&container, state_मूलम्, id, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  container->context = context;

  सचे (container_status == 0)
    पटिनिवत्त crun_make_error (err, 0, "the container `%s` is not running", id);

  {
    cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = रिक्त;

    cgroup_status = libcrun_cgroup_make_status (&status);

    ret = libcrun_cgroup_is_container_paused (cgroup_status, &container_paused, err);
    सचे (UNLIKELY (ret < 0))
      पटिनिवत्त ret;
  }

  सचे (UNLIKELY (container_paused))
    पटिनिवत्त crun_make_error (err, 0, "cannot exec in a paused container `%s`", id);

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = block_signals (err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  libcrun_seccomp_gen_ctx_init (&seccomp_gen_ctx, container, false, 0);

  ret = libcrun_open_seccomp_bpf (&seccomp_gen_ctx, &seccomp_fd, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &seccomp_receiver_fd,
                                     &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  सचे (परिमाण (*opts) != opts->struct_size)
    पटिनिवत्त crun_make_error (err, EINVAL, "invalid libcrun_container_exec_options_s struct");

  सचे (opts->path)
    {
      सचे (process)
        पटिनिवत्त crun_make_error (err, EINVAL, "cannot specify both exec file and options");

      ret = libcrun_load_process_from_file (opts->path, &process, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      process_cleanup = process;
    }

  /* This must be done before we enter a user namespace.  If the process does
     not specify any rlimits, use the ones from the container configuration.  */
  सचे (process->rlimits_len == 0 && container->container_def->process)
    ret = libcrun_set_rlimits (container->container_def->process->rlimits,
                               container->container_def->process->rlimits_len, err);
  अञ्ञथा
    ret = libcrun_set_rlimits (process->rlimits, process->rlimits_len, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = libcrun_set_mempolicy (container->container_def, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  /* If the new process block doesn't specify a SELinux label, AppArmor profile, user,
     I/O priority or scheduler, then use the configuration from the original config file.  */
  सचे (container->container_def->process)
    {
      सचे (process->io_priority == रिक्त && container->container_def->process->io_priority)
        {
          process->io_priority = clone_runtime_spec_schema_config_schema_process_io_priority (container->container_def->process->io_priority);
          सचे (process->io_priority == रिक्त)
            OOM ();
        }

      सचे (process->scheduler == रिक्त && container->container_def->process->scheduler)
        {
          process->scheduler = clone_runtime_spec_schema_config_schema_process_scheduler (container->container_def->process->scheduler);
          सचे (process->scheduler == रिक्त)
            OOM ();
        }

      सचे (process->selinux_label == रिक्त && container->container_def->process->selinux_label)
        process->selinux_label = xstrdup (container->container_def->process->selinux_label);

      सचे (process->apparmor_profile == रिक्त && container->container_def->process->apparmor_profile)
        process->apparmor_profile = xstrdup (container->container_def->process->apparmor_profile);

      सचे (process->user == रिक्त && container->container_def->process->user)
        {
          process->user = clone_runtime_spec_schema_config_schema_process_user (container->container_def->process->user);
          सचे (process->user == रिक्त)
            OOM ();
        }
    }

  ret = initialize_security (container, process, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  /* Do not leak the session keyring of the caller into the container.  The
     keyring created for the container cannot be joined, so use an anonymous one.  */
  सचे (! context->no_new_keyring && ! status.no_new_keyring)
    {
      libcrun_debug ("Creating new keyring for the exec process");

      ret = libcrun_create_keyring (container, रिक्त, process->selinux_label, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
    }

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त crun_make_error (err, errno, "prctl unset dumpable");

  pid = libcrun_join_process (context, container, status.pid, &status, opts->cgroup, context->detach,
                              process, process->terminal ? &terminal_fd : रिक्त, err);
  सचे (UNLIKELY (pid < 0))
    पटिनिवत्त pid;

  /* Process to exec.  */
  सचे (pid == 0)
    {
      TEMP_FAILURE_RETRY (close (pipefd0));
      pipefd0 = -1;

      exec_process_entrypoint (context, container, process, &pipefd1, seccomp_fd, seccomp_receiver_fd, custom_handler, opts->merge_env, err);
      /* It gets here only on errors.  */
      सचे (*err)
        {
          सचे (pipefd1 < 0)
            libcrun_fail_with_error ((*err)->status, "%s", (*err)->सन्देशः);
          अञ्ञथा
            {
              write_error_to_pipe (pipefd1, err);
              TEMP_FAILURE_RETRY (close (pipefd1));
              pipefd1 = -1;
            }
        }
      _safe_exit (EXIT_FAILURE);
    }

  TEMP_FAILURE_RETRY (close (pipefd1));
  pipefd1 = -1;

  TEMP_FAILURE_RETRY (लिख (pipefd0, &pid, परिमाण (pid)));

  सचे (seccomp_fd >= 0)
    close_and_reset (&seccomp_fd);

  सचे (terminal_fd >= 0)
    {
      unsigned short rows = 0, cols = 0;

      सचे (process->console_size)
        {
          cols = process->console_size->width;
          rows = process->console_size->height;
        }

      ret = libcrun_terminal_setup_size (terminal_fd, rows, cols, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;

      सचे (context->console_socket)
        {
          पूर्णाङ्क ret;
          cleanup_close पूर्णाङ्क console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
          सचे (UNLIKELY (console_socket_fd < 0))
            पटिनिवत्त console_socket_fd;
          ret = send_fd_to_socket (console_socket_fd, terminal_fd, err);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त ret;
          close_and_reset (&terminal_fd);
        }
      अञ्ञथा
        {
          ret = libcrun_clear_onlcr (terminal_fd, err);
          सचे (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              पटिनिवत्त ret;
            }

          ret = libcrun_set_raw (0, &orig_terminal, err);
          सचे (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              पटिनिवत्त ret;
            }
        }
    }

  ret = TEMP_FAILURE_RETRY (पठ (pipefd0, &ret_from_child, परिमाण (ret_from_child)));
  सचे (ret != परिमाण (ret_from_child))
    ret = crun_make_error (err, 0, "read pipe failed");
  अञ्ञथा सचे (ret_from_child != 0)
    {
      cleanup_free वर्ण *सन्देशः = रिक्त;
      size_t len = 0;

      ret = read_all_fd (pipefd0, "error stream", &सन्देशः, &len, err);
      सचे (UNLIKELY (ret < 0))
        पटिनिवत्त ret;
      /* the string from read_all_fd is always NUL terminated.  */
      ret = crun_make_error (err, ret_from_child, "%s", सन्देशः);
    }
  अञ्ञथा
    {
      /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
      सचे (own_seccomp_receiver_fd >= 0)
        {
          seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
          सचे (UNLIKELY (seccomp_notify_fd < 0))
            पटिनिवत्त seccomp_notify_fd;

          ret = close_and_reset (&own_seccomp_receiver_fd);
          सचे (UNLIKELY (ret < 0))
            पटिनिवत्त crun_make_error (err, errno, "close seccomp receiver fd failed");
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
  पटिनिवत्त ret;
}

पूर्णाङ्क
libcrun_container_update (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *content, size_t len arg_unused,
                          libcrun_error_t *err)
{
  cleanup_custom_handler_instance संरचना custom_handler_instance_s *custom_handler = रिक्त;
  runtime_spec_schema_config_linux_resources *resources = रिक्त;
  cleanup_container libcrun_container_t *container = रिक्त;
  ध्रुव वर्ण *state_मूलम् = context->state_मूलम्;
  संरचना parser_context ctx = { 0, stderr };
  libcrun_container_status_t status = {};
  parser_error parser_err = रिक्त;
  json_object *doc = रिक्त;
  पूर्णाङ्क ret;

  ret = libcrun_read_container_status (&status, state_मूलम्, id, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = read_container_config_from_state (&container, state_मूलम्, id, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  resources = make_runtime_spec_schema_config_linux_resources (doc, &ctx, &parser_err);
  सचे (UNLIKELY (resources == रिक्त))
    {
      ret = crun_make_error (err, 0, "cannot parse resources: %s", parser_err);
      गच्छ cleanup;
    }

  सचे (custom_handler && custom_handler->vtable->modify_oci_configuration)
    {
      /* Adapt RESOURCES to be used from the modify_oci_configuration hook.  */
      cleanup_free runtime_spec_schema_config_linux *linux = xmalloc0 (परिमाण (*linux));
      cleanup_free runtime_spec_schema_config_schema *def = xmalloc0 (परिमाण (*def));

      def->linux = linux;
      linux->resources = resources;

      ret = custom_handler->vtable->modify_oci_configuration (custom_handler->cookie,
                                                              context,
                                                              def,
                                                              err);
      सचे (UNLIKELY (ret < 0))
        गच्छ cleanup;
    }

  ret = libcrun_linux_container_update (&status, state_मूलम्, resources, err);

cleanup:
  सचे (doc)
    json_object_put (doc);
  मुक्त (parser_err);
  सचे (resources)
    free_runtime_spec_schema_config_linux_resources (resources);

  पटिनिवत्त ret;
}

पूर्णाङ्क
libcrun_container_update_from_file (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *file, libcrun_error_t *err)
{
  cleanup_free वर्ण *content = रिक्त;
  size_t len;
  पूर्णाङ्क ret;

  ret = read_all_file (file, &content, &len, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  पटिनिवत्त libcrun_container_update (context, id, content, len, err);
}

स्थिर पूर्णाङ्क
compare_update_मानानि (ध्रुव शून्य *a, ध्रुव शून्य *b)
{
  ध्रुव संरचना libcrun_update_मानम्_s *aa = a;
  ध्रुव संरचना libcrun_update_मानम्_s *bb = b;
  पूर्णाङ्क ret;

  ret = strcmp (aa->section, bb->section);
  सचे (ret)
    पटिनिवत्त ret;
  पटिनिवत्त strcmp (aa->name, bb->name);
}

पूर्णाङ्क
libcrun_container_update_from_मानानि (libcrun_context_t *context, ध्रुव वर्ण *id,
                                      संरचना libcrun_update_मानम्_s *मानानि, size_t len,
                                      libcrun_error_t *err)
{
  ध्रुव वर्ण *current_section = रिक्त;
  ध्रुव वर्ण *buf;
  json_gen_ctx *gen = रिक्त;
  size_t i, buf_len;
  पूर्णाङ्क ret;

  सचे (! json_gen_init (&gen, रिक्त))
    पटिनिवत्त crun_make_error (err, 0, "json_gen_init failed");
  json_gen_map_open (gen);

  qsort (मानानि, len, परिमाण (संरचना libcrun_update_मानम्_s), compare_update_मानानि);

  अत्थाय (i = 0; i < len; i++)
    {
      सचे (current_section == रिक्त || strcmp (मानानि[i].section, current_section))
        {
          सचे (i > 0)
            json_gen_map_close (gen);

          current_section = मानानि[i].section;
          json_gen_string (gen, current_section, strlen (current_section));
          json_gen_map_open (gen);
        }

      json_gen_string (gen, मानानि[i].name, strlen (मानानि[i].name));

      सचे (मानानि[i].numeric)
        json_gen_सङ्ख्या (gen, मानानि[i].मानम्, strlen (मानानि[i].मानम्));
      अञ्ञथा
        json_gen_string (gen, मानानि[i].मानम्, strlen (मानानि[i].मानम्));
    }
  सचे (len)
    json_gen_map_close (gen);

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);

  ret = libcrun_container_update (context, id, buf, buf_len, err);

  json_gen_free (gen);

  पटिनिवत्त ret;
}

स्थिर शून्य
populate_array_field (वर्ण ***field, वर्ण *array[], size_t अङ्कः_elements)
{
  size_t i;

  *field = xmalloc0 ((अङ्कः_elements + 1) * परिमाण (वर्ण *));
  अत्थाय (i = 0; i < अङ्कः_elements; i++)
    (*field)[i] = xstrdup (array[i]);

  (*field)[i] = रिक्त;
}

#ifdef HAVE_CAP
स्थिर शून्य
populate_capabilities (संरचना features_info_s *info, वर्ण ***capabilities, size_t *अङ्कः_capabilities)
{
  size_t index = 0;
  cap_मानम्_t i;
  वर्ण *endptr;
  पूर्णाङ्क j;

  *अङ्कः_capabilities = 0;
  अत्थाय (i = 0;; i++)
    {
      वर्ण *v = cap_to_name (i);
      सचे (v == रिक्त)
        भङ्ग;
      strtol (v, &endptr, 10);
      सचे (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          भङ्ग;
        }
      (*अङ्कः_capabilities)++;
    }

  *capabilities = xmalloc0 ((*अङ्कः_capabilities + 1) * परिमाण (ध्रुव वर्ण *));
  अत्थाय (i = 0; i < (cap_मानम्_t) *अङ्कः_capabilities; i++)
    {
      वर्ण *v = cap_to_name (i);
      सचे (v == रिक्त)
        भङ्ग;
      strtol (v, &endptr, 10);
      सचे (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          भङ्ग;
        }

      // Convert capability name to uppercase
      अत्थाय (j = 0; v[j] != '\0'; j++)
        v[j] = toupper (v[j]);

      (*capabilities)[index] = v;
      index++;
    }

  (*capabilities)[index] = रिक्त; // Terminate the array with NULL
  populate_array_field (&(info->linux.capabilities), *capabilities, *अङ्कः_capabilities);
}
#endif

स्थिर शून्य
retrieve_mount_options (संरचना features_info_s **info)
{
  cleanup_free ध्रुव संरचना propagation_flags_s *mount_options_list = रिक्त;
  size_t अङ्कः_mount_options = 0;

  // Retrieve mount options from wordlist
  mount_options_list = get_mount_flags_from_wordlist ();

  // Calculate the number of mount options
  याव (mount_options_list[अङ्कः_mount_options].name != रिक्त)
    अङ्कः_mount_options++;

  // Allocate memory for mount options in info struct
  (*info)->mount_options = xmalloc0 ((अङ्कः_mount_options + 1) * परिमाण (वर्ण *));

  // Copy mount options to info struct
  अत्थाय (size_t i = 0; i < अङ्कः_mount_options; i++)
    (*info)->mount_options[i] = xstrdup (mount_options_list[i].name);
}

पूर्णाङ्क
libcrun_container_get_features (libcrun_context_t *context, संरचना features_info_s **info, libcrun_error_t *err arg_unused)
{
  // Allocate memory for the features_info_s structure
  size_t अङ्कः_namspaces = परिमाण (namespaces) / परिमाण (namespaces[0]);
  size_t अङ्कः_operators = परिमाण (operators) / परिमाण (operators[0]);
  size_t अङ्कः_actions = परिमाण (actions) / परिमाण (actions[0]);
  size_t अङ्कः_hooks = परिमाण (hooks) / परिमाण (hooks[0]);
  size_t अङ्कः_archs = परिमाण (archs) / परिमाण (archs[0]);
  size_t अङ्कः_unsafe_annotations = परिमाण (potentially_unsafe_annotations) / परिमाण (potentially_unsafe_annotations[0]);
  cleanup_free वर्ण **capabilities = रिक्त;
  size_t अङ्कः_capabilities = 0;
  size_t अङ्कः_mempolicy_modes = परिमाण (mempolicy_modes) / परिमाण (mempolicy_modes[0]);
  size_t अङ्कः_mempolicy_flags = परिमाण (mempolicy_flags) / परिमाण (mempolicy_flags[0]);

  *info = xmalloc0 (परिमाण (संरचना features_info_s));

  // Hardcoded feature information
  (*info)->oci_version_min = xstrdup ("1.0.0");
  (*info)->oci_version_max = xstrdup ("1.1.0+dev");

  // Populate hooks
  populate_array_field (&((*info)->hooks), hooks, अङ्कः_hooks);

  // Populate mount_options
  retrieve_mount_options (info);

  // Populate namespaces
  populate_array_field (&((*info)->linux.namespaces), namespaces, अङ्कः_namspaces);

#ifdef HAVE_CAP
  // Populate capabilities
  populate_capabilities (*info, &capabilities, &अङ्कः_capabilities);
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
  populate_array_field (&((*info)->linux.seccomp.actions), actions, अङ्कः_actions);
  // Populate operators
  populate_array_field (&((*info)->linux.seccomp.operators), operators, अङ्कः_operators);
  // Populate archs
  populate_array_field (&((*info)->linux.seccomp.archs), archs, अङ्कः_archs);
#अञ्ञथा
  (*info)->linux.seccomp.enabled = false;
#endif

  // Put values for apparmor and selinux
  (*info)->linux.apparmor.enabled = true;
  (*info)->linux.selinux.enabled = true;

  (*info)->linux.intel_rdt.enabled = true;

  (*info)->linux.net_devices.enabled = true;

  populate_array_field (&((*info)->linux.memory_policy.mode), mempolicy_modes, अङ्कः_mempolicy_modes);
  populate_array_field (&((*info)->linux.memory_policy.flags), mempolicy_flags, अङ्कः_mempolicy_flags);

  // Put the values for mount extensions
  (*info)->linux.mount_ext.idmap.enabled = true;

  // Populate the values for annotations
#ifdef HAVE_SECCOMP
  {
    ध्रुव संरचना scmp_version *version = seccomp_version ();
    वर्ण *version_string = रिक्त;

    xasprintf (&version_string, "%u.%u.%u", version->major, version->minor, version->micro);
    (*info)->annotations.io_github_seccomp_libseccomp_version = version_string;
  }
#endif

  सचे (context->handler_manager && handler_by_name (context->handler_manager, "wasm"))
    (*info)->annotations.run_oci_crun_wasm = true;

#सचे HAVE_CRIU
  (*info)->annotations.run_oci_crun_checkpoint_enabled = true;
#endif
  (*info)->annotations.run_oci_crun_commit = GIT_VERSION;
  (*info)->annotations.run_oci_crun_version = PACKAGE_VERSION;

  populate_array_field (&((*info)->potentially_unsafe_annotations), potentially_unsafe_annotations, अङ्कः_unsafe_annotations);

  पटिनिवत्त 0;
}

स्थिर शून्य
features_add_string (json_gen_ctx *json_gen, ध्रुव वर्ण *कुञ्चिका, ध्रुव वर्ण *मानम्)
{
  json_gen_string (json_gen, कुञ्चिका, strlen (कुञ्चिका));
  json_gen_string (json_gen, मानम्, strlen (मानम्));
}

स्थिर शून्य
features_add_bool (json_gen_ctx *json_gen, ध्रुव वर्ण *कुञ्चिका, पूर्णाङ्क मानम्)
{
  json_gen_string (json_gen, कुञ्चिका, strlen (कुञ्चिका));
  json_gen_bool (json_gen, मानम्);
}

स्थिर शून्य
features_add_bool_str (json_gen_ctx *json_gen, ध्रुव वर्ण *कुञ्चिका, पूर्णाङ्क मानम्)
{
  features_add_string (json_gen, कुञ्चिका, मानम् ? "true" : "false");
}

स्थिर शून्य
features_add_array (json_gen_ctx *json_gen, ध्रुव वर्ण *कुञ्चिका, वर्ण **array)
{
  size_t i;

  json_gen_string (json_gen, कुञ्चिका, strlen (कुञ्चिका));
  json_gen_array_open (json_gen);
  सचे (array)
    अत्थाय (i = 0; array[i] != रिक्त; i++)
      json_gen_string (json_gen, array[i], strlen (array[i]));
  json_gen_array_close (json_gen);
}

स्थिर शून्य
features_add_linux_info (json_gen_ctx *json_gen, ध्रुव संरचना linux_info_s *linux)
{
  json_gen_string (json_gen, "linux", strlen ("linux"));
  json_gen_map_open (json_gen);

  features_add_array (json_gen, "namespaces", linux->namespaces);
  features_add_array (json_gen, "capabilities", linux->capabilities);

  json_gen_string (json_gen, "cgroup", strlen ("cgroup"));
  json_gen_map_open (json_gen);
  features_add_bool (json_gen, "v1", linux->cgroup.v1);
  features_add_bool (json_gen, "v2", linux->cgroup.v2);
  features_add_bool (json_gen, "systemd", linux->cgroup.systemd);
  features_add_bool (json_gen, "systemdUser", linux->cgroup.systemd_user);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "seccomp", strlen ("seccomp"));
  json_gen_map_open (json_gen);
  features_add_bool (json_gen, "enabled", linux->seccomp.enabled);
  सचे (linux->seccomp.actions)
    features_add_array (json_gen, "actions", linux->seccomp.actions);
  सचे (linux->seccomp.operators)
    features_add_array (json_gen, "operators", linux->seccomp.operators);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "apparmor", strlen ("apparmor"));
  json_gen_map_open (json_gen);
  features_add_bool (json_gen, "enabled", linux->apparmor.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "selinux", strlen ("selinux"));
  json_gen_map_open (json_gen);
  features_add_bool (json_gen, "enabled", linux->selinux.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "mountExtensions", strlen ("mountExtensions"));
  json_gen_map_open (json_gen);
  json_gen_string (json_gen, "idmap", strlen ("idmap"));
  json_gen_map_open (json_gen);
  features_add_bool (json_gen, "enabled", linux->mount_ext.idmap.enabled);
  json_gen_map_close (json_gen);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "intelRdt", strlen ("intelRdt"));
  json_gen_map_open (json_gen);
  features_add_bool (json_gen, "enabled", linux->intel_rdt.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "netDevices", strlen ("netDevices"));
  json_gen_map_open (json_gen);
  features_add_bool (json_gen, "enabled", linux->net_devices.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "memoryPolicy", strlen ("memoryPolicy"));
  json_gen_map_open (json_gen);
  सचे (linux->memory_policy.mode)
    features_add_array (json_gen, "modes", linux->memory_policy.mode);
  सचे (linux->memory_policy.flags)
    features_add_array (json_gen, "flags", linux->memory_policy.flags);
  json_gen_map_close (json_gen);

  json_gen_map_close (json_gen);
}

स्थिर शून्य
features_add_annotations_info (json_gen_ctx *json_gen, ध्रुव संरचना annotations_info_s *annotation)
{
  json_gen_string (json_gen, "annotations", strlen ("annotations"));
  json_gen_map_open (json_gen);

  सचे (! is_empty_string (annotation->io_github_seccomp_libseccomp_version))
    features_add_string (json_gen, "io.github.seccomp.libseccomp.version", annotation->io_github_seccomp_libseccomp_version);

  features_add_bool_str (json_gen, "org.opencontainers.runc.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);
  features_add_bool_str (json_gen, "run.oci.crun.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);

  features_add_string (json_gen, "run.oci.crun.commit", annotation->run_oci_crun_commit);
  features_add_string (json_gen, "run.oci.crun.version", annotation->run_oci_crun_version);

  features_add_bool_str (json_gen, "run.oci.crun.wasm", annotation->run_oci_crun_wasm);

  json_gen_map_close (json_gen);
}

पूर्णाङ्क
libcrun_container_get_features_json (libcrun_context_t *context, वर्ण **out, libcrun_error_t *err)
{
  cleanup_struct_features संरचना features_info_s *info = रिक्त;
  ध्रुव वर्ण *buffer = रिक्त;
  json_gen_ctx *json_gen;
  size_t buffer_len;
  पूर्णाङ्क ret;

  *out = रिक्त;

  ret = libcrun_container_get_features (context, &info, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (! json_gen_init (&json_gen, रिक्त))
    पटिनिवत्त crun_make_error (err, 0, "cannot initialize JSON generator");

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

  पटिनिवत्त 0;
}

/* Read the container status and fail if the container is not running.  */
स्थिर पूर्णाङ्क
read_status_require_running (libcrun_context_t *context, ध्रुव वर्ण *id,
                             libcrun_container_status_t *status, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = libcrun_read_container_status (status, context->state_मूलम्, id, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = libcrun_is_container_running (status, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;
  सचे (ret == 0)
    पटिनिवत्त crun_make_error (err, 0, "the container `%s` is not running", id);

  पटिनिवत्त 0;
}

पूर्णाङ्क
libcrun_container_pause (libcrun_context_t *context, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  पटिनिवत्त libcrun_container_pause_linux (&status, err);
}

पूर्णाङ्क
libcrun_container_unpause (libcrun_context_t *context, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  पटिनिवत्त libcrun_container_unpause_linux (&status, err);
}

/* Translate the public options struct into the internal representation used by
   the linux/CRIU layer.  Strings are borrowed (not copied).  */
स्थिर शून्य
cr_options_from_public (libcrun_checkpoint_restore_t *dst, संरचना libcrun_checkpoint_restore_options_s *src)
{
  memset (dst, 0, परिमाण (*dst));
  dst->image_path = (वर्ण *) src->image_path;
  dst->work_path = (वर्ण *) src->work_path;
  dst->parent_path = (वर्ण *) src->parent_path;
  dst->console_socket = src->console_socket;
  dst->lsm_profile = (वर्ण *) src->lsm_profile;
  dst->lsm_mount_context = (वर्ण *) src->lsm_mount_context;
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

पूर्णाङ्क
libcrun_container_checkpoint (libcrun_context_t *context, ध्रुव वर्ण *id,
                              संरचना libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  ध्रुव वर्ण *state_मूलम् = context->state_मूलम्;
  libcrun_container_status_t status = {};
  libcrun_checkpoint_restore_t cr;
  cleanup_container libcrun_container_t *container = रिक्त;

  cr_options_from_public (&cr, cr_options);

  ret = read_status_require_running (context, id, &status, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = read_container_config_from_state (&container, state_मूलम्, id, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  /* Needed to use the same paths (e.g. the shared empty directory used for
     masked paths) as the container runtime and restore do.  */
  container->context = context;

  ret = libcrun_container_checkpoint_linux (&status, container, &cr, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (! (cr_options->leave_running || cr_options->pre_dump))
    पटिनिवत्त container_delete_internal (context, रिक्त, रिक्त, id, true, true, err);

  पटिनिवत्त 0;
}

/* The dummy process only keeps the cgroup populated until the container is restored there.
   Its cgroup is below the container cgroup, so once the container runs it can freeze or kill
   the dummy process: never depend on it after that point.  */
स्थिर शून्य
restore_proxy_process (पूर्णाङ्क proxy_pid_pipe)
{
  वर्ण c;

  /* Wait to be killed by the parent, or for the parent to go away.  */
  TEMP_FAILURE_RETRY (पठ (proxy_pid_pipe, &c, परिमाण (c)));
}

स्थिर पूर्णाङ्क
restore_terminate_proxy_process (pid_t proxy_pid, ध्रुव वर्ण *proxy_cgroup, pid_t container_pid, पूर्णाङ्क cgroup_manager, libcrun_error_t *err)
{
  पूर्णाङ्क mode;
  पूर्णाङ्क ret;

  सचे (cgroup_manager == CGROUP_MANAGER_SYSTEMD)
    {
      वर्ण ready_str[64];

      ret = snprintf (ready_str, परिमाण (ready_str), "MAINPID=%d", container_pid);
      सचे (UNLIKELY (ret >= (पूर्णाङ्क) परिमाण (ready_str)))
        पटिनिवत्त crun_make_error (err, 0, "internal error: static buffer too small");

      ret = send_sd_notify (ready_str, err);
      /* Do not fail on errors.  */
      सचे (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  mode = libcrun_get_cgroup_mode (err);
  सचे (UNLIKELY (mode < 0))
    पटिनिवत्त mode;

  /* SIGKILL terminates a process frozen with cgroup v2, while the cgroup v1 freezer does not
     deliver it until the cgroup is thawed, so do not wait for it there.  */
  kill (proxy_pid, SIGKILL);
  TEMP_FAILURE_RETRY (waitpid (proxy_pid, रिक्त, mode == CGROUP_MODE_UNIFIED ? 0 : WNOHANG));

  पटिनिवत्त destroy_cgroup_path (proxy_cgroup, mode, err);
}

पूर्णाङ्क
libcrun_container_restore (libcrun_context_t *context, ध्रुव वर्ण *id arg_unused,
                           संरचना libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = रिक्त;
  cleanup_container libcrun_container_t *container = रिक्त;
  cleanup_close पूर्णाङ्क proxy_pid_pipe0 = -1;
  cleanup_close पूर्णाङ्क proxy_pid_pipe1 = -1;
  runtime_spec_schema_config_schema *def;
  libcrun_container_status_t status = {};
  संरचना libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  libcrun_checkpoint_restore_t cr;
  cleanup_pid pid_t proxy_pid = -1;
  cleanup_close पूर्णाङ्क seccomp_fd = -1;
  ध्रुव वर्ण *seccomp_bpf_data;
  पूर्णाङ्क proxy_pid_pipe[2];
  पूर्णाङ्क cgroup_manager;
  uid_t मूलम्_uid = -1;
  gid_t मूलम्_gid = -1;
  पूर्णाङ्क ret;

  cr_options_from_public (&cr, cr_options);

  container = libcrun_container_load_from_file ("config.json", err);
  सचे (container == रिक्त)
    पटिनिवत्त -1;

  container->context = context;
  def = container->container_def;

  ret = check_config_file (def, context, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = libcrun_status_check_directories (context->state_मूलम्, context->id, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = libcrun_copy_config_file (context->id, context->state_मूलम्, container, err);
  सचे (UNLIKELY (ret < 0))
    गच्छ fail;

  /* CRIU restores the seccomp filters of the checkpointed processes, but
     exec loads the filter from the seccomp.bpf file in the state directory,
     so it must be generated again for the restored container.  */
  seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  सचे (UNLIKELY (ret < 0))
    गच्छ fail;

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  सचे (UNLIKELY (ret < 0))
    गच्छ fail;
  close_and_reset (&seccomp_fd);

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  सचे (context->systemd_cgroup)
    cgroup_manager = CGROUP_MANAGER_SYSTEMD;
  अञ्ञथा सचे (context->force_no_cgroup)
    cgroup_manager = CGROUP_MANAGER_DISABLED;

  def = container->container_def;

  /* If we are root (either on the host or in a namespace),
   * then chown the cgroup to root in the container user namespace. */
  get_मूलम्_in_the_userns (def, container->host_uid, container->host_gid, &मूलम्_uid, &मूलम्_gid);

  /* If the root in the container is different than the current root user, attempt to chown
     the std streams before entering the user namespace.  Otherwise we might lose access
     to the user (as it is not mapped in the user namespace) and cannot chown them.  */
  सचे (मूलम्_uid > 0 || मूलम्_gid > 0)
    {
      ret = maybe_chown_std_streams (मूलम्_uid, मूलम्_gid, err);
      सचे (UNLIKELY (ret < 0))
        गच्छ fail;
    }

  /* CRIU runs as a child process, so the restored processes would inherit the session keyring.  */
  ret = setup_container_keyring (container, context, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  {
    cleanup_close पूर्णाङ्क cgroup_dirfd = -1;
    bool needs_proxy_process;
    cleanup_cgroup_preenter संरचना libcrun_cgroup_args cg = {
      .resources = def->linux ? def->linux->resources : रिक्त,
      .annotations = container->annotations,
      .cgroup_path = def->linux ? def->linux->cgroups_path : "",
      .manager = cgroup_manager,
      .मूलम्_uid = मूलम्_uid,
      .मूलम्_gid = मूलम्_gid,
      .id = context->id,
      .state_मूलम् = context->state_मूलम्,
      .container = container,
    };

    /* The CRIU restore code uses bundle, rootfs and cgroup_path of status.   The cgroup_path is set later.  */
    status.bundle = (वर्ण *) context->bundle;
    status.rootfs = def->मूलम्->path;

    ret = libcrun_cgroup_preenter (&cg, &cgroup_dirfd, err);
    सचे (UNLIKELY (ret < 0))
      गच्छ fail;

    needs_proxy_process = cgroup_dirfd < 0;

    /* If the target cgroup was already created (with cgroupfs), we can restore the container directly there.  */
    सचे (! needs_proxy_process)
      {
        cleanup_free वर्ण *cgroup_path = रिक्त;

        ret = get_cgroup_dirfd_path (cgroup_dirfd, &cgroup_path, err);
        सचे (UNLIKELY (ret < 0))
          गच्छ fail;

        /* Complete the configuration for the cgroup before the container is
           restored there: CRIU resumes the restored processes before it
           returns, so the device filter and the limits must already be in
           place.  There is no process to move, as CRIU is run in the cgroup,
           and the controllers were enabled by precreate_cgroup.  */
        cg.joined = true;

        ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
        सचे (UNLIKELY (ret < 0))
          गच्छ fail;

        /* Restore the container directly in the desired cgroup.  */
        status.cgroup_path = cgroup_path;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        सचे (UNLIKELY (ret < 0))
          गच्छ fail;

        /* Now that the container is restored, keep any signal for it, so that
           it is forwarded to the init by wait_for_process, rather than acted
           upon by crun.  This cannot be done earlier, as CRIU, which is run as
           a child process, would inherit the blocked signals.  */
        ret = block_signals (err);
        सचे (UNLIKELY (ret < 0))
          गच्छ fail;

        /* Use the container first process PID to finalize the cgroup.  */
        cg.pid = status.pid;
      }
    अञ्ञथा
      {
        /* When using systemd, we need a process first that must exist before a cgroup is created.
           Create a dummy process that sits and waits until the container is restored.  */
        ret = pipe2 (proxy_pid_pipe, O_CLOEXEC);
        सचे (UNLIKELY (ret < 0))
          {
            ret = crun_make_error (err, errno, "pipe");
            गच्छ fail;
          }

        /* Only used for auto cleanup.  */
        proxy_pid_pipe0 = proxy_pid_pipe[0];
        proxy_pid_pipe1 = proxy_pid_pipe[1];

        proxy_pid = fork ();
        सचे (UNLIKELY (proxy_pid < 0))
          {
            ret = crun_make_error (err, errno, "fork");
            गच्छ fail;
          }

        सचे (proxy_pid == 0)
          {
            close_and_reset (&proxy_pid_pipe1);

            restore_proxy_process (proxy_pid_pipe0);
            _safe_exit (EXIT_SUCCESS);
          }

        close_and_reset (&proxy_pid_pipe0);

        /* Use the dummy process PID as the PID to setup the cgroup.  */
        cg.pid = proxy_pid;

        /* Complete the configuration for the cgroup.  It contains only the dummy process.  */
        ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
        सचे (UNLIKELY (ret < 0))
          गच्छ fail;
      }

    ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
    सचे (UNLIKELY (ret < 0))
      गच्छ fail;

    /* When using a dummy process, the container is restored here once the cgroup is configured.  */
    सचे (needs_proxy_process)
      {
        cleanup_free वर्ण *target_cgroup = रिक्त;
        cleanup_free वर्ण *proxy_cgroup = रिक्त;

        ret = libcrun_get_cgroup_process (proxy_pid, &target_cgroup, false, err);
        सचे (UNLIKELY (ret < 0))
          गच्छ fail;

        /* Move the dummy process to a sub-cgroup.  */
        ret = append_paths (&proxy_cgroup, err, target_cgroup, ".proxy-crun", रिक्त);
        सचे (UNLIKELY (ret < 0))
          गच्छ fail;

        ret = libcrun_move_process_to_cgroup (proxy_pid, 0, proxy_cgroup, true, err);
        सचे (UNLIKELY (ret < 0))
          गच्छ fail;

        /* Restore the container in the same cgroup where the dummy process was.  */
        status.cgroup_path = target_cgroup;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        सचे (UNLIKELY (ret < 0))
          गच्छ fail;

        /* See the comment for the other libcrun_container_restore_linux call.  */
        ret = block_signals (err);
        सचे (UNLIKELY (ret < 0))
          गच्छ fail;

        /* The container is running, so do not fail because of the dummy process.  */
        ret = restore_terminate_proxy_process (proxy_pid, proxy_cgroup, status.pid, cgroup_manager, err);
        सचे (UNLIKELY (ret < 0))
          crun_error_release (err);
        proxy_pid = -1;

        close_and_reset (&proxy_pid_pipe1);
      }
  }

  context->detach = cr_options->detach;
  ret = write_container_status (container, context, status.pid, cgroup_status, err);
  सचे (UNLIKELY (ret < 0))
    गच्छ fail;

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

    पटिनिवत्त wait_for_process (&args, err);
  }

fail:
  /* Once restored, the container is running.  Without a status file nothing
     can reach it anymore, so do not leave it behind.  */
  सचे (status.pid > 0)
    {
      kill (status.pid, SIGKILL);
      waitpid_ignore_stopped (status.pid, रिक्त, 0);
    }
  सचे (cgroup_status)
    {
      libcrun_error_t tmp_err = रिक्त;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  {
    libcrun_error_t tmp_err = रिक्त;

    सचे (libcrun_container_delete_status (context->state_मूलम्, context->id, &tmp_err) < 0)
      crun_error_release (&tmp_err);
  }
  पटिनिवत्त ret;
}

पूर्णाङ्क
libcrun_container_read_pids (libcrun_context_t *context, ध्रुव वर्ण *id, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  cleanup_cgroup_status संरचना libcrun_cgroup_status *cgroup_status = रिक्त;
  cleanup_container_status libcrun_container_status_t status = {};
  पूर्णाङ्क ret;

  ret = libcrun_read_container_status (&status, context->state_मूलम्, id, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (status.cgroup_path == रिक्त || status.cgroup_path[0] == '\0')
    पटिनिवत्त crun_make_error (err, 0, "the container is not using cgroups");

  cgroup_status = libcrun_cgroup_make_status (&status);

  पटिनिवत्त libcrun_cgroup_read_pids (cgroup_status, recurse, pids, err);
}

पूर्णाङ्क
libcrun_container_list_json (libcrun_context_t *context, वर्ण **out, libcrun_error_t *err)
{
  libcrun_container_list_t *list = रिक्त, *it;
  ध्रुव वर्ण *content = रिक्त;
  json_gen_ctx *gen = रिक्त;
  size_t len;
  पूर्णाङ्क ret;

  *out = रिक्त;

  ret = libcrun_get_containers_list (&list, context->state_मूलम्, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (! json_gen_init (&gen, रिक्त))
    {
      ret = crun_make_error (err, 0, "cannot allocate json generator");
      गच्छ exit;
    }

  json_gen_config (gen, json_gen_beautify, 1);
  json_gen_array_open (gen);

  अत्थाय (it = list; it; it = it->next)
    {
      libcrun_container_status_t status;
      पूर्णाङ्क running = 0;
      पूर्णाङ्क pid;
      ध्रुव वर्ण *container_status = रिक्त;

      ret = libcrun_read_container_status (&status, context->state_मूलम्, it->name, err);
      सचे (UNLIKELY (ret < 0))
        गच्छ exit;

      pid = status.pid;

      ret = libcrun_get_container_state_string (it->name, &status, context->state_मूलम्, &container_status,
                                                &running, err);
      सचे (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          अनुवर्त;
        }

      सचे (! running)
        pid = 0;

      json_gen_map_open (gen);
      json_gen_string (gen, "id", strlen ("id"));
      json_gen_string (gen, it->name, strlen (it->name));
      json_gen_string (gen, "pid", strlen ("pid"));
      map_int (gen, pid);
      json_gen_string (gen, "status", strlen ("status"));
      json_gen_string (gen, container_status, strlen (container_status));
      json_gen_string (gen, "bundle", strlen ("bundle"));
      json_gen_string (gen, status.bundle, strlen (status.bundle));
      json_gen_string (gen, "created", strlen ("created"));
      json_gen_string (gen, status.created, strlen (status.created));
      json_gen_string (gen, "owner", strlen ("owner"));
      json_gen_string (gen, status.owner, safe_strlen (status.owner));
      json_gen_map_close (gen);

      libcrun_free_container_status (&status);
    }

  json_gen_array_close (gen);
  सचे (json_gen_get_buf (gen, &content, &len) != json_gen_status_ok)
    {
      ret = libcrun_make_error (err, 0, "cannot generate json list");
      गच्छ exit;
    }

  *out = xstrdup (content);
  ret = 0;

exit:
  सचे (list)
    libcrun_free_containers_list (list);
  सचे (gen)
    json_gen_free (gen);

  पटिनिवत्त ret;
}

पूर्णाङ्क
libcrun_write_json_containers_list (libcrun_context_t *context, FILE *out, libcrun_error_t *err)
{
  cleanup_free वर्ण *content = रिक्त;
  size_t len;
  पूर्णाङ्क ret;

  ret = libcrun_container_list_json (context, &content, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  len = strlen (content);
  सचे (len && fwrite (content, 1, len, out) != len)
    पटिनिवत्त crun_make_error (err, errno, "error writing to file");

  पटिनिवत्त 0;
}

पूर्णाङ्क
libcrun_container_update_intel_rdt (libcrun_context_t *context, ध्रुव वर्ण *id, संरचना libcrun_intel_rdt_update *update, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = रिक्त;
  पूर्णाङ्क ret;

  ret = read_container_config_from_state (&container, context->state_मूलम्, id, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  पटिनिवत्त libcrun_update_intel_rdt (id, container, update->l3_cache_schema, update->mem_bw_schema, update->schemata, err);
}

स्थिर पूर्णाङ्क
libcrun_container_add_or_remove_mounts_from_file (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *file, bool add, libcrun_error_t *err)
{
  cleanup_custom_handler_instance संरचना custom_handler_instance_s *custom_handler = रिक्त;
  cleanup_container libcrun_container_t *container = रिक्त;
  cleanup_free runtime_spec_schema_defs_mount **mounts = रिक्त;
  ध्रुव वर्ण *state_मूलम् = context->state_मूलम्;
  cleanup_free parser_error parser_err = रिक्त;
  संरचना parser_context pctx = { 0, stderr };
  libcrun_container_status_t status = {};
  cleanup_free वर्ण *content = रिक्त;
  json_object *doc = रिक्त;
  size_t n_mounts = 0, len, i;
  पूर्णाङ्क ret = 1;

  ret = read_all_file (file, &content, &len, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = libcrun_read_container_status (&status, state_मूलम्, id, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = read_container_config_from_state (&container, state_मूलम्, id, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  ret = parse_json_file (&doc, content, &pctx, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (! json_object_is_type (doc, json_type_array))
    {
      ret = crun_make_error (err, 0, "mounts must be an array");
      गच्छ cleanup;
    }
  अञ्ञथा
    {
      n_mounts = json_object_array_length (doc);

      mounts = xmalloc0 ((n_mounts + 1) * परिमाण (*mounts));
      अत्थाय (i = 0; i < n_mounts; i++)
        {
          mounts[i] = make_runtime_spec_schema_defs_mount (json_object_array_get_idx (doc, i), &pctx, &parser_err);
          सचे (mounts[i] == रिक्त)
            {
              ret = crun_make_error (err, 0, "cannot parse mount: %s", parser_err);
              गच्छ cleanup;
            }
        }
    }

  सचे (add)
    ret = libcrun_make_runtime_mounts (container, &status, mounts, n_mounts, err);
  अञ्ञथा
    ret = libcrun_destroy_runtime_mounts (container, &status, mounts, n_mounts, err);

cleanup:
  सचे (doc)
    json_object_put (doc);
  सचे (mounts)
    अत्थाय (i = 0; i < n_mounts; i++)
      free_runtime_spec_schema_defs_mount (mounts[i]);
  पटिनिवत्त ret;
}

पूर्णाङ्क
libcrun_container_add_mounts_from_file (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *file, libcrun_error_t *err)
{
  पटिनिवत्त libcrun_container_add_or_remove_mounts_from_file (context, id, file, true, err);
}

पूर्णाङ्क
libcrun_container_remove_mounts_from_file (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *file, libcrun_error_t *err)
{
  पटिनिवत्त libcrun_container_add_or_remove_mounts_from_file (context, id, file, false, err);
}
