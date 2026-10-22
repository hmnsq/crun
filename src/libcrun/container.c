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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <ocispec/runtime_spec_schema_config_schema.h>
#અન્તર્ભાવ <stdbool.h>
#અન્તર્ભાવ "container.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "json_gen_utils.h"
#અન્તર્ભાવ "seccomp.h"
#અન્તર્ભાવ "mempolicy.h"
#ifdef HAVE_SECCOMP
#  include <seccomp.h>
#endif
#અન્તર્ભાવ "scheduler.h"
#અન્તર્ભાવ "seccomp_notify.h"
#અન્તર્ભાવ "custom-handler.h"
#અન્તર્ભાવ <stdbool.h>
#અન્તર્ભાવ <argp.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <sched.h>
#અન્તર્ભાવ <sys/wait.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ "status.h"
#અન્તર્ભાવ "mount_flags.h"
#અન્તર્ભાવ "linux.h"
#અન્તર્ભાવ "terminal.h"
#અન્તર્ભાવ "io_priority.h"
#અન્તર્ભાવ "cgroup.h"
#અન્તર્ભાવ "cgroup-utils.h"
#અન્તર્ભાવ <sys/prctl.h>
#અન્તર્ભાવ <sys/auxv.h>
#અન્તર્ભાવ <sys/signalfd.h>
#અન્તર્ભાવ <sys/epoll.h>
#અન્તર્ભાવ <sys/socket.h>
#ifdef HAVE_CAP
#  include <sys/capability.h>
#endif
#અન્તર્ભાવ <sys/ioctl.h>
#અન્તર્ભાવ <termios.h>
#અન્તર્ભાવ <grp.h>
#અન્તર્ભાવ <git-version.h>

#ifdef HAVE_SYSTEMD
#  include <systemd/sd-daemon.h>
#endif

#અન્તર્ભાવ <json-c/json.h>
#અન્તર્ભાવ <ocispec/json_common.h>

ગણના
{
  SYNC_SOCKET_SYNC_MESSAGE,
  SYNC_SOCKET_ERROR_MESSAGE,
  SYNC_SOCKET_WARNING_MESSAGE,
  SYNC_SOCKET_DEBUG_MESSAGE,
};

સંરચના container_entrypoint_s
{
  libcrun_container_t *container;
  libcrun_context_t *context;
  પૂર્ણાંક has_terminal_socket_pair;
  પૂર્ણાંક terminal_socketpair[2];

  /* Used by log_write_to_sync_socket.  */
  પૂર્ણાંક sync_socket;

  પૂર્ણાંક seccomp_fd;
  પૂર્ણાંક seccomp_receiver_fd;
  પૂર્ણાંક console_socket_fd;

  પૂર્ણાંક hooks_out_fd;
  પૂર્ણાંક hooks_err_fd;

  /* Absolute bundle path, to be reported to hooks run in the container.  */
  અચલ અક્ષર *bundle;

  સંરચના custom_handler_instance_s *custom_handler;
};

સંરચના sync_socket_message_s
{
  પૂર્ણાંક type;
  પૂર્ણાંક error_કિંમત;
  અક્ષર message[512];
};

પ્રકારવ્યાખ્યા runtime_spec_schema_defs_hook hook;

// linux hooks
અક્ષર *hooks[] = {
  "prestart",
  "createRuntime",
  "createContainer",
  "startContainer",
  "poststart",
  "poststop"
};

// linux namespaces
સ્થિર અક્ષર *namespaces[] = {
  "cgroup",
  "ipc",
  "mount",
  "network",
  "pid",
  "user",
  "uts"
};

સ્થિર અક્ષર *actions[] = {
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

સ્થિર અક્ષર *operators[] = {
  "SCMP_CMP_NE",
  "SCMP_CMP_LT",
  "SCMP_CMP_LE",
  "SCMP_CMP_EQ",
  "SCMP_CMP_GE",
  "SCMP_CMP_GT",
  "SCMP_CMP_MASKED_EQ",
};

સ્થિર અક્ષર *archs[] = {
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

સ્થિર અક્ષર *mempolicy_modes[] = {
  "MPOL_DEFAULT",
  "MPOL_PREFERRED",
  "MPOL_BIND",
  "MPOL_INTERLEAVE",
  "MPOL_LOCAL",
  "MPOL_PREFERRED_MANY",
  "MPOL_WEIGHTED_INTERLEAVE"
};

સ્થિર અક્ષર *mempolicy_flags[] = {
  "MPOL_F_NUMA_BALANCING",
  "MPOL_F_RELATIVE_NODES",
  "MPOL_F_STATIC_NODES"
};

સ્થિર અક્ષર *potentially_unsafe_annotations[] = {
  "module.wasm.image/variant",
  "io.kubernetes.cri.container-type",
  "run.oci.",
  "org.criu.",
  "krun.",
};

#define SYNC_SOCKET_MESSAGE_LEN(x, l) (offsetof (સંરચના sync_socket_message_s, message) + l)

સ્થિર પૂર્ણાંક
sync_socket_write_સંદેશ (પૂર્ણાંક fd, પૂર્ણાંક verbosity, પૂર્ણાંક err_કિંમત, અચલ અક્ષર *log_સંદેશ)
{
  પૂર્ણાંક ret;
  size_t err_len;
  સંરચના sync_socket_message_s સંદેશ;
  પસંદગી (verbosity)
    {
    સ્થિતિ LIBCRUN_VERBOSITY_DEBUG:
      સંદેશ.type = SYNC_SOCKET_DEBUG_MESSAGE;
      તોડો;
    સ્થિતિ LIBCRUN_VERBOSITY_WARNING:
      સંદેશ.type = SYNC_SOCKET_WARNING_MESSAGE;
      તોડો;
    સ્થિતિ LIBCRUN_VERBOSITY_ERROR:
      સંદેશ.type = SYNC_SOCKET_ERROR_MESSAGE;
      તોડો;
    }
  સંદેશ.error_કિંમત = err_કિંમત;

  જો (fd < 0)
    પાછા 0;

  err_len = strlen (log_સંદેશ);
  જો (err_len >= કદ (સંદેશ.message))
    err_len = કદ (સંદેશ.message) - 1;

  memcpy (સંદેશ.message, log_સંદેશ, err_len);
  સંદેશ.message[err_len] = '\0';

  ret = TEMP_FAILURE_RETRY (લિખ (fd, &સંદેશ, SYNC_SOCKET_MESSAGE_LEN (સંદેશ, err_len + 1)));
  જો (UNLIKELY (ret < 0))
    પાછા -1;

  પાછા 0;
}

સ્થિર પૂર્ણાંક
sync_socket_write_error (પૂર્ણાંક fd, libcrun_error_t *out_err)
{
  જો (fd < 0)
    પાછા 0;
  પાછા sync_socket_write_સંદેશ (fd, false, (*out_err)->status, (*out_err)->સંદેશ);
}

સ્થિર શૂન્ય
log_write_to_sync_socket (પૂર્ણાંક errno_, અચલ અક્ષર *સંદેશ, પૂર્ણાંક verbosity, શૂન્ય *arg)
{
  સંરચના container_entrypoint_s *entrypoint_args = arg;
  પૂર્ણાંક fd = entrypoint_args->sync_socket;

  જો (fd < 0)
    પાછા;

  જો (sync_socket_write_સંદેશ (fd, verbosity, errno_, સંદેશ) < 0)
    log_write_to_stderr (errno_, સંદેશ, verbosity, arg);
}

સ્થિર bool
is_memory_limit_too_નીચલું (runtime_spec_schema_config_schema *def)
{
  અચલ long memory_limit_too_નીચલું = 1024 * 1024;

  જો (def->linux == ખાલી || def->linux->resources == ખાલી)
    પાછા false;

  જો (def->linux->resources->memory
      && def->linux->resources->memory->limit_present
      && def->linux->resources->memory->limit < memory_limit_too_નીચલું)
    પાછા true;

  જો (def->linux->resources->unified)
    {
      size_t i;

      માટે (i = 0; i < def->linux->resources->unified->len; i++)
        જો (strcmp (def->linux->resources->unified->ચાવીઓ[i], "memory.max") == 0)
          {
            long limit;

            errno = 0;
            limit = strtol (def->linux->resources->unified->કિંમતો[i], ખાલી, 10);
            જો (errno != 0)
              પાછા false;
            જો (limit < memory_limit_too_નીચલું)
              પાછા true;
          }
    }

  પાછા false;
}

સ્થિર પૂર્ણાંક
sync_socket_wait_sync (libcrun_context_t *context, પૂર્ણાંક fd, bool flush, libcrun_error_t *err)
{
  સંરચના sync_socket_message_s સંદેશ;

  જો (fd < 0)
    પાછા 0;

  જ્યારે (true)
    {
      પૂર્ણાંક ret;

      ret = TEMP_FAILURE_RETRY (પઠ (fd, &સંદેશ, કદ (સંદેશ)));
      જો (UNLIKELY (ret < 0))
        {
          જો (flush)
            પાછા 0;
          પાછા crun_make_error (err, errno, "read from sync socket");
        }

      જો (ret == 0)
        {
          જો (flush)
            પાછા 0;

          પાછા crun_make_error (err, 0, "read from the init process");
        }

      જો (! flush && સંદેશ.type == SYNC_SOCKET_SYNC_MESSAGE)
        પાછા 0;
      નહીંતર જો (સંદેશ.type == SYNC_SOCKET_DEBUG_MESSAGE)
        {
          જો (context)
            context->output_handler (સંદેશ.error_કિંમત, સંદેશ.message, LIBCRUN_VERBOSITY_DEBUG, context->output_handler_arg);
          ચાલુ;
        }
      નહીંતર જો (સંદેશ.type == SYNC_SOCKET_WARNING_MESSAGE)
        {
          જો (context)
            context->output_handler (સંદેશ.error_કિંમત, સંદેશ.message, LIBCRUN_VERBOSITY_WARNING, context->output_handler_arg);
          ચાલુ;
        }
      નહીંતર જો (સંદેશ.type == SYNC_SOCKET_ERROR_MESSAGE)
        પાછા crun_make_error (err, સંદેશ.error_કિંમત, "%s", સંદેશ.message);
    }
}

સ્થિર પૂર્ણાંક
sync_socket_send_sync (પૂર્ણાંક fd, bool flush_errors, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  સંરચના sync_socket_message_s સંદેશ = {
    0,
  };
  સંદેશ.type = SYNC_SOCKET_SYNC_MESSAGE;

  જો (fd < 0)
    પાછા 0;

  ret = TEMP_FAILURE_RETRY (લિખ (fd, &સંદેશ, SYNC_SOCKET_MESSAGE_LEN (સંદેશ, 0)));
  જો (UNLIKELY (ret < 0))
    {
      જો (flush_errors)
        {
          પૂર્ણાંક saved_errno = errno;
          ret = TEMP_FAILURE_RETRY (પઠ (fd, &સંદેશ, કદ (સંદેશ)));
          જો (ret >= 0 && સંદેશ.type == SYNC_SOCKET_ERROR_MESSAGE)
            પાછા crun_make_error (err, સંદેશ.error_કિંમત, "%s", સંદેશ.message);
          errno = saved_errno;
        }
      પાછા crun_make_error (err, errno, "write to sync socket");
    }

  પાછા 0;
}

સ્થિર bool
has_cap_sys_ptrace ()
{
#ifdef HAVE_CAP
  સંરચના __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  સંરચના __user_cap_data_struct data[2] = { { 0 } };

  જો (UNLIKELY (capget (&hdr, data) < 0))
    પાછા true;

  પાછા (data[0].effective & (1U << CAP_SYS_PTRACE)) != 0;
#નહીંતર
  પાછા geteuid () == 0;
#endif
}

સ્થિર libcrun_container_t *
make_container (runtime_spec_schema_config_schema *container_def, અચલ અક્ષર *path, અચલ અક્ષર *config)
{
  libcrun_container_t *container = xmalloc0 (કદ (*container));
  container->container_def = container_def;

  container->host_uid = geteuid ();
  container->host_gid = getegid ();
  container->host_has_cap_sys_ptrace = has_cap_sys_ptrace ();
  container->proc_fd = -1;

  container->annotations = make_string_map_from_json (container_def->annotations);

  /* An empty namespace path means that a new namespace is to be created,
     the same as if it is not specified.  */
  જો (container_def->linux)
    {
      size_t i;

      માટે (i = 0; i < container_def->linux->namespaces_len; i++)
        જો (container_def->linux->namespaces[i]->path && container_def->linux->namespaces[i]->path[0] == '\0')
          {
            મુક્ત (container_def->linux->namespaces[i]->path);
            container_def->linux->namespaces[i]->path = ખાલી;
          }
    }

  જો (path)
    container->config_file = xstrdup (path);
  જો (config)
    container->config_file_content = xstrdup (config);

  પાછા container;
}

libcrun_container_t *
libcrun_container_load_from_memory (અચલ અક્ષર *json, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free અક્ષર *oci_error = ખાલી;
  container_def = runtime_spec_schema_config_schema_parse_data (json, ખાલી, &oci_error);
  જો (container_def == ખાલી)
    {
      crun_make_error (err, 0, "load: `%s`", oci_error);
      પાછા ખાલી;
    }
  પાછા make_container (container_def, ખાલી, json);
}

libcrun_container_t *
libcrun_container_load_from_file (અચલ અક્ષર *path, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free અક્ષર *oci_error = ખાલી;
  cleanup_free અક્ષર *content = ખાલી;
  size_t len;
  પૂર્ણાંક ret;

  libcrun_debug ("Loading container from config file: `%s`", path);
  ret = read_all_file (path, &content, &len, err);
  જો (UNLIKELY (ret < 0))
    પાછા ખાલી;

  container_def = runtime_spec_schema_config_schema_parse_data (content, ખાલી, &oci_error);
  જો (container_def == ખાલી)
    {
      crun_make_error (err, 0, "load `%s`: %s", path, oci_error);
      પાછા ખાલી;
    }
  પાછા make_container (container_def, ખાલી, content);
}

અચલ અક્ષર *
libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err)
{
  સંરચના parser_context ctx = { 0, stderr };
  parser_error gen_err = ખાલી;

  જો (container == ખાલી)
    {
      crun_make_error (err, EINVAL, "no container specified");
      પાછા ખાલી;
    }

  જો (container->config_file_content == ખાલી && container->container_def)
    {
      container->config_file_content = runtime_spec_schema_config_schema_generate_json (container->container_def, &ctx, &gen_err);
      જો (container->config_file_content == ખાલી)
        {
          crun_make_error (err, 0, "cannot generate the configuration JSON: %s", gen_err ?: "unknown error");
          મુક્ત (gen_err);
          પાછા ખાલી;
        }
      મુક્ત (gen_err);
    }

  જો (container->config_file_content == ખાલી)
    {
      crun_make_error (err, 0, "the container has no configuration");
      પાછા ખાલી;
    }

  પાછા container->config_file_content;
}

અચલ અક્ષર *
libcrun_container_get_annotation (libcrun_container_t *container, અચલ અક્ષર *ચાવી)
{
  જો (container == ખાલી)
    પાછા ખાલી;
  પાછા find_string_map_કિંમત (container->annotations, ચાવી);
}

size_t
libcrun_container_get_annotations_len (libcrun_container_t *container)
{
  જો (container == ખાલી || container->annotations == ખાલી)
    પાછા 0;
  પાછા string_map_size (container->annotations);
}

પૂર્ણાંક
libcrun_container_get_annotation_at (libcrun_container_t *container, size_t index, અચલ અક્ષર **ચાવી,
                                     અચલ અક્ષર **કિંમત)
{
  જો (container == ખાલી)
    {
      errno = ERANGE;
      પાછા -1;
    }
  પાછા string_map_get_at (container->annotations, index, ચાવી, કિંમત);
}

uid_t
libcrun_container_get_uid (libcrun_container_t *container)
{
  જો (container == ખાલી)
    પાછા (uid_t) -1;
  પાછા container->host_uid;
}

gid_t
libcrun_container_get_gid (libcrun_container_t *container)
{
  જો (container == ખાલી)
    પાછા (gid_t) -1;
  પાછા container->host_gid;
}

શૂન્ય
libcrun_container_free (libcrun_container_t *ctr)
{
  જો (ctr == ખાલી)
    પાછા;

  જો (ctr->cleanup_private_data)
    ctr->cleanup_private_data (ctr->private_data);

  જો (ctr->container_def)
    free_runtime_spec_schema_config_schema (ctr->container_def);

  free_string_map (ctr->annotations);

  જો (ctr->proc_fd >= 0)
    close (ctr->proc_fd);

  મુક્ત (ctr->config_file_content);
  મુક્ત (ctr->config_file);
  મુક્ત (ctr);
}

libcrun_context_t *
libcrun_context_new (અચલ અક્ષર *id, અચલ અક્ષર *state_મૂળ, libcrun_error_t *err)
{
  libcrun_context_t *ctx = xmalloc0 (કદ (*ctx));

  /* Go through the setters so that the owned_* copies and the borrowed
     pointers stay in sync and are released by libcrun_context_free.  */
  libcrun_context_set_id (ctx, id);
  libcrun_context_set_state_મૂળ (ctx, state_મૂળ);
  libcrun_context_set_bundle (ctx, ".");
  ctx->fifo_exec_wait_fd = -1;

  જો (libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, id, ખાલી, err) < 0)
    {
      libcrun_context_free (ctx);
      પાછા ખાલી;
    }

  ctx->handler_manager = libcrun_handler_manager_create (err);
  જો (ctx->handler_manager == ખાલી)
    {
      libcrun_context_free (ctx);
      પાછા ખાલી;
    }

#ifdef CRUN_LIBDIR
  જો (access (CRUN_LIBDIR "/handlers", F_OK) == 0)
    {
      જો (libcrun_handler_manager_load_directory (ctx->handler_manager, CRUN_LIBDIR "/handlers", err) < 0)
        {
          libcrun_context_free (ctx);
          પાછા ખાલી;
        }
    }
#endif

  પાછા ctx;
}

શૂન્ય
libcrun_context_free (libcrun_context_t *ctx)
{
  જો (ctx == ખાલી)
    પાછા;

  મુક્ત (ctx->owned_state_મૂળ);
  મુક્ત (ctx->owned_id);
  મુક્ત (ctx->owned_bundle);
  મુક્ત (ctx->owned_console_socket);
  મુક્ત (ctx->owned_pid_file);
  મુક્ત (ctx->owned_notify_socket);
  મુક્ત (ctx->owned_handler);

  જો (ctx->handler_manager)
    handler_manager_free (ctx->handler_manager);

  મુક્ત (ctx);
}

#define LIBCRUN_CONTEXT_SET_STRING(field)                  \
  કરો                                                       \
    {                                                      \
      મુક્ત (ctx->owned_##field);                           \
      ctx->owned_##field = કિંમત ? xstrdup (કિંમત) : ખાલી; \
      ctx->field = ctx->owned_##field;                     \
  } જ્યારે (0)

શૂન્ય
libcrun_context_set_id (libcrun_context_t *ctx, અચલ અક્ષર *કિંમત)
{
  LIBCRUN_CONTEXT_SET_STRING (id);
}

શૂન્ય
libcrun_context_set_state_મૂળ (libcrun_context_t *ctx, અચલ અક્ષર *કિંમત)
{
  LIBCRUN_CONTEXT_SET_STRING (state_મૂળ);
}

શૂન્ય
libcrun_context_set_bundle (libcrun_context_t *ctx, અચલ અક્ષર *કિંમત)
{
  LIBCRUN_CONTEXT_SET_STRING (bundle);
}

શૂન્ય
libcrun_context_set_console_socket (libcrun_context_t *ctx, અચલ અક્ષર *કિંમત)
{
  LIBCRUN_CONTEXT_SET_STRING (console_socket);
}

શૂન્ય
libcrun_context_set_pid_file (libcrun_context_t *ctx, અચલ અક્ષર *કિંમત)
{
  LIBCRUN_CONTEXT_SET_STRING (pid_file);
}

શૂન્ય
libcrun_context_set_notify_socket (libcrun_context_t *ctx, અચલ અક્ષર *કિંમત)
{
  LIBCRUN_CONTEXT_SET_STRING (notify_socket);
}

શૂન્ય
libcrun_context_set_handler (libcrun_context_t *ctx, અચલ અક્ષર *કિંમત)
{
  LIBCRUN_CONTEXT_SET_STRING (handler);
}

#undef LIBCRUN_CONTEXT_SET_STRING

અચલ અક્ષર *
libcrun_context_get_pid_file (libcrun_context_t *ctx)
{
  પાછા ctx->pid_file;
}

અચલ અક્ષર *
libcrun_context_get_state_મૂળ (libcrun_context_t *ctx)
{
  જો (ctx == ખાલી)
    પાછા ખાલી;
  પાછા ctx->state_મૂળ;
}

શૂન્ય
libcrun_context_set_preserve_fds (libcrun_context_t *ctx, પૂર્ણાંક preserve_fds)
{
  ctx->preserve_fds = preserve_fds;
}

પૂર્ણાંક
libcrun_context_get_preserve_fds (libcrun_context_t *ctx)
{
  પાછા ctx->preserve_fds;
}

શૂન્ય
libcrun_context_set_listen_fds (libcrun_context_t *ctx, પૂર્ણાંક listen_fds)
{
  ctx->listen_fds = listen_fds;
}

શૂન્ય
libcrun_context_set_args (libcrun_context_t *ctx, પૂર્ણાંક argc, અક્ષર **argv)
{
  ctx->argc = argc;
  ctx->argv = argv;
}

શૂન્ય
libcrun_context_set_systemd_cgroup (libcrun_context_t *ctx, bool કિંમત)
{
  ctx->systemd_cgroup = કિંમત;
}

શૂન્ય
libcrun_context_set_detach (libcrun_context_t *ctx, bool કિંમત)
{
  ctx->detach = કિંમત;
}

શૂન્ય
libcrun_context_set_no_new_keyring (libcrun_context_t *ctx, bool કિંમત)
{
  ctx->no_new_keyring = કિંમત;
}

શૂન્ય
libcrun_context_set_no_pivot (libcrun_context_t *ctx, bool કિંમત)
{
  ctx->no_pivot = કિંમત;
}

શૂન્ય
libcrun_context_set_force_no_cgroup (libcrun_context_t *ctx, bool કિંમત)
{
  ctx->force_no_cgroup = કિંમત;
}

શૂન્ય
libcrun_context_set_output_handler (libcrun_context_t *ctx, crun_output_handler handler, શૂન્ય *arg)
{
  ctx->output_handler = handler;
  ctx->output_handler_arg = arg;
}

પૂર્ણાંક
libcrun_context_init_logging (libcrun_context_t *ctx, અચલ અક્ષર *log, libcrun_error_t *err)
{
  પાછા libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, ctx->id, log, err);
}

સ્થિર પૂર્ણાંક
block_signals (libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  sigset_t mask;
  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "sigprocmask");
  પાછા 0;
}

સ્થિર પૂર્ણાંક
unblock_signals (libcrun_error_t *err)
{
  પૂર્ણાંક i;
  પૂર્ણાંક ret;
  sigset_t mask;
  સંરચના sigaction act = {};

  sigfillset (&mask);
  ret = sigprocmask (SIG_UNBLOCK, &mask, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "sigprocmask");

  act.sa_handler = SIG_DFL;
  માટે (i = 0; i < NSIG; i++)
    {
      ret = sigaction (i, &act, ખાલી);
      જો (ret < 0 && errno != EINVAL)
        પાછા crun_make_error (err, errno, "sigaction");
    }

  પાછા 0;
}

/* must be used on the host before pivot_root(2).  */
સ્થિર પૂર્ણાંક
initialize_security (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  ret = libcrun_init_caps (container, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (UNLIKELY (proc == ખાલી))
    પાછા 0;

  જો (proc->apparmor_profile)
    {
      ret = libcrun_initialize_apparmor (err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  ret = libcrun_initialize_selinux (container, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા 0;
}

/* Emit the "annotations" map.  Returns a json_gen_status; callers can wrap
   the call with GEN_OR_FAIL.  */
સ્થિર પૂર્ણાંક
gen_annotations (json_gen_ctx *gen, json_map_string_string *annotations)
{
  size_t i;
  પૂર્ણાંક r;

  GEN_ચાવી (gen, "annotations");
  GEN_OR_FAIL (json_gen_map_open (gen));

  માટે (i = 0; i < annotations->len; i++)
    {
      /* An annotation whose value is null in the document is parsed into a
         NULL, which cannot be emitted as a JSON string; leave it out.  */
      જો (annotations->ચાવીઓ[i] == ખાલી || annotations->કિંમતો[i] == ખાલી)
        ચાલુ;

      GEN_STR (gen, annotations->ચાવીઓ[i]);
      GEN_STR (gen, annotations->કિંમતો[i]);
    }

  પાછા json_gen_map_close (gen);

gen_error:
  પાછા r;
}

/* Return the bundle path to report in the container state.  crun always
   chdir()s into the bundle, so fall back to getcwd() when the caller has no
   absolute bundle path, as can happen when libcrun is used as a library.
   *ALLOCATED must be freed by the caller.  */
સ્થિર અચલ અક્ષર *
get_bundle_path (અચલ અક્ષર *bundle, અક્ષર **allocated)
{
  જો (bundle != ખાલી && bundle[0] == '/')
    પાછા bundle;

  *allocated = getcwd (ખાલી, 0);
  જો (*allocated == ખાલી)
    OOM ();
  પાછા *allocated;
}

સ્થિર પૂર્ણાંક
do_hooks (runtime_spec_schema_config_schema *def, pid_t pid, અચલ અક્ષર *id, bool keep_going, અચલ અક્ષર *bundle,
          અચલ અક્ષર *cwd, અચલ અક્ષર *status, અચલ અક્ષર *name, hook **hooks, size_t hooks_len, પૂર્ણાંક out_fd,
          પૂર્ણાંક err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  size_t i, stdin_len;
  પૂર્ણાંક r, ret;
  અક્ષર *stdin = ખાલી;
  cleanup_free અક્ષર *bundle_allocated = ખાલી;
  અચલ અક્ષર *rootfs = def->મૂળ ? def->મૂળ->path : "";
  cleanup_json_gen json_gen_ctx *gen = ખાલી;

  bundle = get_bundle_path (bundle, &bundle_allocated);

  જો (! json_gen_init (&gen, ખાલી))
    પાછા crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 0);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_ચાવી (gen, "ociVersion");
  GEN_STR (gen, "1.0");

  GEN_ચાવી (gen, "id");
  GEN_STR (gen, id);

  GEN_ચાવી (gen, "pid");
  GEN_OR_FAIL (map_int (gen, pid));

  GEN_ચાવી (gen, "root");
  GEN_STR (gen, rootfs);

  GEN_ચાવી (gen, "bundle");
  GEN_STR (gen, bundle);

  GEN_ચાવી (gen, "status");
  GEN_STR (gen, status);

  જો (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, (અચલ અક્ષર **) &stdin, &stdin_len));

  ret = 0;

  bool error_created = false;
  માટે (i = 0; i < hooks_len; i++)
    {
      અક્ષર **env = environ;

      જો (hooks[i]->env)
        env = hooks[i]->env;

      /* Release the error from the previous iteration, if any.  */
      જો (error_created)
        {
          crun_error_release (err);
          error_created = false;
        }

      ret = run_process_with_stdin_timeout_envp (hooks[i]->path, hooks[i]->args, cwd, hooks[i]->timeout, env,
                                                 stdin, stdin_len, out_fd, err_fd, can_ignore_chdir_errors, err);
      જો (UNLIKELY (ret < 0))
        error_created = true;

      જો (UNLIKELY (ret != 0))
        {
          જો (keep_going)
            libcrun_warning ("error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
          નહીંતર
            {
              libcrun_error (0, "error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
              તોડો;
            }
        }
    }

  પાછા ret;

gen_error:
  પાછા json_gen_error_to_crun_error (r, err);
}

સ્થિર પૂર્ણાંક
get_json_gen_result (json_gen_ctx *gen, અક્ષર **out, size_t *out_len)
{
  અચલ અક્ષર *buf = ખાલી;
  size_t buf_len = 0;
  પૂર્ણાંક r;

  r = json_gen_get_buf (gen, &buf, &buf_len);
  જો (UNLIKELY (r != json_gen_status_ok))
    પાછા r;

  *out_len = buf_len;

  *out = xmalloc (buf_len + 1);
  memcpy (*out, buf, buf_len);
  (*out)[buf_len] = '\0';

  પાછા json_gen_status_ok;
}

સ્થિર પૂર્ણાંક
get_seccomp_receiver_fd_payload (libcrun_container_t *container, અચલ અક્ષર *status, pid_t own_pid,
                                 અક્ષર **seccomp_fd_payload, size_t *seccomp_fd_payload_len, libcrun_error_t *err)
{
  પૂર્ણાંક r;
  json_gen_ctx *gen = ખાલી;
  runtime_spec_schema_config_schema *def = container->container_def;
  અચલ અક્ષર *અચલ OCI_VERSION = "0.2.0";

  જો (! json_gen_init (&gen, ખાલી))
    પાછા crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_ચાવી (gen, "ociVersion");
  GEN_STR (gen, OCI_VERSION);

  GEN_ચાવી (gen, "fds");
  GEN_OR_FAIL (json_gen_array_open (gen));

  GEN_ચાવી (gen, "seccompFd");
  GEN_OR_FAIL (json_gen_array_close (gen));

  GEN_ચાવી (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  જો (def && def->linux && def->linux->seccomp)
    {
      અચલ અક્ષર *metadata = def->linux->seccomp->listener_metadata;

      જો (metadata)
        {
          GEN_ચાવી (gen, "metadata");
          GEN_STR (gen, metadata);
        }
    }

  /* State.  */
  GEN_ચાવી (gen, "state");
  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_ચાવી (gen, "ociVersion");
  GEN_STR (gen, OCI_VERSION);

  જો (container->context && container->context->id)
    {
      GEN_ચાવી (gen, "id");
      GEN_STR (gen, container->context->id);
    }

  GEN_ચાવી (gen, "status");
  GEN_STR (gen, status);

  GEN_ચાવી (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  જો (container->context && container->context->bundle)
    {
      GEN_ચાવી (gen, "bundle");
      GEN_STR (gen, container->context->bundle);
    }

  જો (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));
  /* End state.  */

  GEN_OR_FAIL (json_gen_map_close (gen));

  r = get_json_gen_result (gen, seccomp_fd_payload, seccomp_fd_payload_len);

gen_error:
  json_gen_free (gen);

  પાછા json_gen_error_to_crun_error (r, err);
}

સ્થિર પૂર્ણાંક
send_sync_cb (શૂન્ય *data, libcrun_error_t *err)
{
  પૂર્ણાંક sync_socket_fd = *((પૂર્ણાંક *) data);
  પૂર્ણાંક ret;

  /* sync 2.  */
  ret = sync_socket_send_sync (sync_socket_fd, false, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  /* sync 3.  */
  પાછા sync_socket_wait_sync (ખાલી, sync_socket_fd, false, err);
}

સ્થિર પૂર્ણાંક
maybe_chown_std_streams (uid_t container_uid, gid_t container_gid, libcrun_error_t *err)
{
  પૂર્ણાંક ret, i;

  માટે (i = 0; i < 3; i++)
    {
      જો (! isatty (i))
        {
          સંરચના stat statbuf;
          ret = fstat (i, &statbuf);
          જો (UNLIKELY (ret < 0))
            {
              જો (errno == EBADF)
                ચાલુ;
              પાછા crun_make_error (err, errno, "fstat fd `%d`", i);
            }

          /* Skip chown for device files */
          જો (S_ISCHR (statbuf.st_mode) || S_ISBLK (statbuf.st_mode))
            ચાલુ;

          ret = fchown (i, container_uid, container_gid);
          જો (UNLIKELY (ret < 0))
            {
              /* EINVAL means the user is not mapped in the current userns.
                 Ignore EPERM and EROFS as well as there is no reason to fail
                 so early, and let the container payload deal with it.
                 EBADF means fd is closed.
              */
              જો (errno == EINVAL || errno == EPERM || errno == EROFS || errno == EBADF)
                ચાલુ;

              પાછા crun_make_error (err, errno, "fchown std stream `%i`", i);
            }
        }
    }
  પાછા 0;
}

પૂર્ણાંક
libcrun_container_notify_handler (સંરચના container_entrypoint_s *args,
                                  ગણના handler_configure_phase phase,
                                  libcrun_container_t *container, અચલ અક્ષર *rootfs,
                                  libcrun_error_t *err)
{
  સંરચના custom_handler_s *h;

  જો (args->custom_handler == ખાલી)
    પાછા 0;

  h = args->custom_handler->vtable;
  જો (h == ખાલી || h->configure_container == ખાલી)
    પાછા 0;

  પાછા h->configure_container (args->custom_handler->cookie, phase,
                                 args->context, container,
                                 rootfs, err);
}

/* Resolve and normalize the container rootfs path.  */
સ્થિર પૂર્ણાંક
resolve_rootfs_path (libcrun_container_t *container, અક્ષર **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  જો (def->મૂળ && def->મૂળ->path)
    {
      *rootfs = realpath (def->મૂળ->path, ખાલી);
      જો (UNLIKELY (*rootfs == ખાલી))
        {
          પૂર્ણાંક saved_errno = errno;

          /* If realpath failed for any reason, try the relative directory.  */
          જો (def->મૂળ->path[0] == '/')
            {
              cleanup_free અક્ષર *cwd = ખાલી;
              libcrun_error_t tmp_err = ખાલી;
              ssize_t len;
              પૂર્ણાંક proc_fd;

              proc_fd = libcrun_get_cached_proc_fd (container, &tmp_err);
              જો (LIKELY (proc_fd >= 0))
                {
                  len = safe_readlinkat (proc_fd, "self/cwd", &cwd, 0, &tmp_err);
                  જો (LIKELY (len > 0))
                    {
                      /* If the rootfs is under the current working directory, just use its relative path.  */
                      જો (has_dir_prefix (def->મૂળ->path, cwd))
                        {
                          અચલ અક્ષર *it = consume_slashes (def->મૂળ->path + len);
                          જો (LIKELY (*it))
                            {
                              પૂર્ણાંક ret = faccessat (AT_FDCWD, it, F_OK, 0);
                              જો (LIKELY (ret == 0))
                                *rootfs = xstrdup (it);
                            }
                        }
                    }
                }
              crun_error_release (&tmp_err);
            }

          જો (*rootfs == ખાલી)
            {
              જો (saved_errno == ENOENT)
                પાછા crun_make_error (err, ENOENT, "the rootfs path `%s` does not exist", def->મૂળ->path);

              *rootfs = xstrdup (def->મૂળ->path);
            }
        }
    }
  પાછા 0;
}

/* Initialize the environment variables.  */
સ્થિર પૂર્ણાંક
setup_environment (runtime_spec_schema_config_schema *def, uid_t container_uid, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  જો (clearenv ())
    પાછા crun_make_error (err, errno, "clearenv");

  જો (def->process)
    {
      size_t i;

      માટે (i = 0; i < def->process->env_len; i++)
        {
          /* A null element of the array of variables is parsed into a NULL,
             which putenv(3) cannot be handed.  */
          જો (def->process->env[i] == ખાલી)
            પાછા crun_make_error (err, EINVAL, "environment variable is not specified");

          જો (putenv (def->process->env[i]) < 0)
            પાછા crun_make_error (err, errno, "putenv `%s`", def->process->env[i]);
        }
    }

  જો (is_empty_string (getenv ("HOME")))
    {
      ret = set_home_env (container_uid);
      જો (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          જો (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            પાછા crun_make_error (err, errno, "setenv HOME");
        }
    }

  પાછા 0;
}

/* Configure terminal and console for interactive containers.  */
સ્થિર પૂર્ણાંક
setup_terminal (સંરચના container_entrypoint_s *entrypoint_args, libcrun_container_t *container,
                પૂર્ણાંક console_socket, પૂર્ણાંક console_socketpair, પૂર્ણાંક has_terminal, libcrun_error_t *err)
{
  cleanup_close પૂર્ણાંક terminal_fd = -1;
  પૂર્ણાંક ret;

  જો (! has_terminal)
    પાછા 0;

  fflush (stderr);

  જો (console_socket >= 0 || (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0))
    {
      terminal_fd = libcrun_set_terminal (container, err);
      જો (UNLIKELY (terminal_fd < 0))
        પાછા terminal_fd;
    }

  જો (console_socket >= 0)
    {
      ret = send_fd_to_socket (console_socket, terminal_fd, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  નહીંતર જો (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0)
    {
      ret = send_fd_to_socket (console_socketpair, terminal_fd, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  પાછા 0;
}

/* Resolve and validate the container's executable path.  */
સ્થિર પૂર્ણાંક
setup_executable_path (સંરચના container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                       અક્ષર **exec_path, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  જો (def->process && def->process->args)
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      જો (UNLIKELY (ret < 0))
        {
          જો (entrypoint_args->custom_handler == ખાલી && crun_error_get_errno (err) == ENOENT)
            પાછા ret;

          /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
             once the process switched to the use that runs in the container.  This might be necessary
             when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
             is not honored.  */
          crun_error_release (err);
        }
    }

  પાછા 0;
}

/* Apply security settings for the container process.  */
સ્થિર પૂર્ણાંક
apply_security_settings (સંરચના container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                         libcrun_container_t *container, pid_t own_pid, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  runtime_spec_schema_config_schema_process_capabilities *capabilities;
  પૂર્ણાંક no_new_privs;

  જો (def->process && ! def->process->no_new_privileges)
    {
      અક્ષર **seccomp_flags = ખાલી;
      size_t seccomp_flags_len = 0;
      cleanup_free અક્ષર *seccomp_fd_payload = ખાલી;
      size_t seccomp_fd_payload_len = 0;

      જો (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      જો (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  capabilities = def->process ? def->process->capabilities : ખાલી;
  no_new_privs = def->process ? def->process->no_new_privileges : 1;
  ret = libcrun_set_caps (capabilities, container->container_uid, container->container_gid, no_new_privs,
                          entrypoint_args->custom_handler != ખાલી, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા 0;
}

/* Create the process that joins the existing PID namespace now that the
   host mounts are not visible anymore, and continue the setup from there.  */
સ્થિર પૂર્ણાંક
do_delayed_pidns_fork (પૂર્ણાંક sync_socket, pid_t *own_pid, libcrun_error_t *err)
{
  pid_t pid;
  પૂર્ણાંક ret;

  pid = fork ();
  જો (UNLIKELY (pid < 0))
    પાછા crun_make_error (err, errno, "fork");

  જો (pid)
    {
      /* Report the new PID to the parent and exit immediately.  */
      ret = sync_socket_send_sync (sync_socket, false, err);
      જો (LIKELY (ret == 0))
        {
          ret = TEMP_FAILURE_RETRY (લિખ (sync_socket, &pid, કદ (pid)));
          જો (LIKELY (ret == કદ (pid)))
            _safe_exit (EXIT_SUCCESS);
        }

      kill (pid, SIGKILL);
      _safe_exit (EXIT_FAILURE);
    }

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (પઠ (sync_socket, own_pid, કદ (*own_pid)));
  જો (UNLIKELY (ret != કદ (*own_pid)))
    {
      જો (ret >= 0)
        errno = 0;
      પાછા crun_make_error (err, errno, "read from sync socket");
    }

  પાછા 0;
}

સંરચના delayed_pidns_fork_s
{
  libcrun_container_t *container;
  પૂર્ણાંક sync_socket;
  pid_t *own_pid;
};

સ્થિર પૂર્ણાંક
delayed_pidns_fork_cb (શૂન્ય *data, libcrun_error_t *err)
{
  સંરચના delayed_pidns_fork_s *d = data;

  જો (! libcrun_container_delayed_pidns_fork (d->container))
    પાછા 0;

  પાછા do_delayed_pidns_fork (d->sync_socket, d->own_pid, err);
}

સ્થિર પૂર્ણાંક
container_init_setup (શૂન્ય *args, pid_t *own_pid, અક્ષર *notify_socket,
                      પૂર્ણાંક sync_socket, અક્ષર **exec_path, libcrun_error_t *err)
{
  સંરચના container_entrypoint_s *entrypoint_args = args;
  libcrun_container_t *container = entrypoint_args->container;
  bool chdir_done = false;
  પૂર્ણાંક ret;
  પૂર્ણાંક has_terminal;
  cleanup_close પૂર્ણાંક console_socket = -1;
  cleanup_close પૂર્ણાંક console_socketpair = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free અક્ષર *rootfs = ખાલી;
  સંરચના delayed_pidns_fork_s delayed_pidns_fork = {
    .container = container,
    .sync_socket = sync_socket,
    .own_pid = own_pid,
  };

  ret = initialize_security (container, def->process, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_configure_network (container, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = resolve_rootfs_path (container, &rootfs, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  /* Configure terminal socket pair for container communication.  */
  જો (entrypoint_args->terminal_socketpair[0] >= 0)
    {
      close_and_reset (&entrypoint_args->terminal_socketpair[0]);
      console_socketpair = entrypoint_args->terminal_socketpair[1];
    }

  /* sync 1.  */
  ret = sync_socket_wait_sync (ખાલી, sync_socket, false, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  has_terminal = container->container_def->process && container->container_def->process->terminal;
  જો (has_terminal && entrypoint_args->context->console_socket)
    console_socket = entrypoint_args->console_socket_fd;

  ret = libcrun_set_sysctl (container, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_BEFORE_MOUNTS, container, rootfs, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_do_pivot_મૂળ (container, entrypoint_args->context->no_pivot, &rootfs, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  /* sync 2 and 3 are sent as part of libcrun_set_mounts.  */
  ret = libcrun_set_mounts (entrypoint_args, container, rootfs, send_sync_cb, &sync_socket, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  /* The createContainer hooks MUST run after the mounts are set up but before
     the pivot_root.  When these hooks are present can_use_open_tree_namespace()
     forces the deferred-pivot path, so the actual pivot happens later in
     libcrun_finalize_mounts().  */
  જો (def->hooks && def->hooks->create_container_len)
    {
      libcrun_error_t tmp_err = ખાલી;
      પૂર્ણાંક in_userns = check_running_in_user_namespace (&tmp_err);
      જો (tmp_err)
        crun_error_release (&tmp_err);
      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, ખાલી, "created", "createContainer",
                      (hook **) def->hooks->create_container,
                      def->hooks->create_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      જો (UNLIKELY (ret != 0))
        પાછા ret;
    }

  ret = libcrun_finalize_mounts (entrypoint_args, container, rootfs, delayed_pidns_fork_cb, &delayed_pidns_fork, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (def->process)
    {
      ret = libcrun_set_selinux_label (container, def->process, false, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = libcrun_set_apparmor_profile (container, def->process, false, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  ret = mark_or_close_fds_ge_than (container, entrypoint_args->context->preserve_fds + 3, false, err);
  જો (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ret = libcrun_reopen_dev_null (err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = maybe_chown_std_streams (container->container_uid, container->container_gid, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = setup_environment (def, container->container_uid, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  /* Set primary process to 1 explicitly if nothing is configured and LISTEN_FD is not set.  */
  જો (entrypoint_args->context->listen_fds > 0 && getenv ("LISTEN_PID") == ખાલી)
    {
      libcrun_warning ("setting LISTEN_PID=1 since no previous configuration was found");
      જો (UNLIKELY (setenv ("LISTEN_PID", "1", 1) < 0))
        પાછા crun_make_error (err, errno, "setenv LISTEN_PID");
    }

  /* Attempt to chdir immediately here, before doing the setresuid.  If we fail here, let's
     try again later once the process switched to the user that runs in the container.  */
  જો (def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      જો (LIKELY (ret == 0))
        chdir_done = true;
      નહીંતર
        crun_error_release (err);
    }

  ret = setsid ();
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "setsid");

  ret = setup_terminal (entrypoint_args, container, console_socket, console_socketpair, has_terminal, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  close_and_reset (&console_socket);
  close_and_reset (&console_socketpair);

  ret = setup_executable_path (entrypoint_args, def, exec_path, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_set_hostname (container, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_set_domainname (container, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (def->process && def->process->user)
    umask (def->process->user->umask_present ? def->process->user->umask : 0022);

  ret = apply_security_settings (entrypoint_args, def, container, *own_pid, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (UNLIKELY (def->process && def->process->args && *exec_path == ખાલી))
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      જો (UNLIKELY (ret < 0))
        {
          જો (entrypoint_args->custom_handler == ખાલી || is_empty_string (def->process->args[0]))
            પાછા ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          *exec_path = xstrdup (def->process->args[0]);
        }
    }

  /* The chdir was not already performed, so try again now after switching to the UID/GID in the container.  */
  જો (! chdir_done && def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (notify_socket && putenv (notify_socket) < 0)
    પાછા crun_make_error (err, errno, "putenv `%s`", notify_socket);

  પાછા 0;
}

સ્થિર પૂર્ણાંક
open_hooks_output (libcrun_container_t *container, પૂર્ણાંક *out_fd, પૂર્ણાંક *err_fd, libcrun_error_t *err)
{
  અચલ અક્ષર *annotation;

  *err_fd = *out_fd = -1;

  libcrun_debug ("Opening hooks output");
  annotation = find_annotation (container, "run.oci.hooks.stdout");
  જો (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stdout` annotation");
      *out_fd = TEMP_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      જો (UNLIKELY (*out_fd < 0))
        પાછા crun_make_error (err, errno, "open `%s`", annotation);
    }

  annotation = find_annotation (container, "run.oci.hooks.stderr");
  જો (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stderr` annotation");
      *err_fd = TEMP_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      જો (UNLIKELY (*err_fd < 0))
        પાછા crun_make_error (err, errno, "open `%s`", annotation);
    }

  પાછા 0;
}

/* The environment the process was started with lives on the initial stack, right after the
   argv strings and before the string AT_EXECFN points to.  clearenv(3) only drops the pointers
   to it, so when there is no execv to replace the stack, erase it or the container process can
   still read it, e.g. through /proc/self/environ.  */
સ્થિર શૂન્ય
erase_initial_environment (અક્ષર **argv, પૂર્ણાંક argc)
{
  અક્ષર *start = argv[argc - 1] + strlen (argv[argc - 1]) + 1;
  અક્ષર *end = (અક્ષર *) getauxval (AT_EXECFN);

  જો (end > start)
    memset (start, 0, end - start);
}

સ્થિર શૂન્ય
rewrite_argv (અક્ષર **argv, પૂર્ણાંક argc, અચલ અક્ષર *name, અક્ષર **args, size_t args_len)
{
  cleanup_free અક્ષર *decorated_name = ખાલી;
  size_t available_len = argv[argc - 1] - argv[0] + strlen (argv[argc - 1]) + 1;
  cleanup_free અક્ષર *new_argv = ખાલી;
  size_t i, so_far = 0, needed = 0;

  needed = xasprintf (&decorated_name, "[libcrun:%s]", name) + 1;
  માટે (i = 0; i < args_len; i++)
    needed += strlen (args[i]) + 1;

  new_argv = xmalloc0 (needed + 1);
  strcpy (new_argv, decorated_name);
  so_far = strlen (decorated_name) + 1;

  જો (available_len >= needed)
    {
      માટે (i = 0; i < args_len; i++)
        {
          strcpy (new_argv + so_far, args[i]);
          so_far += strlen (args[i]) + 1;
        }
    }

  જો (so_far >= available_len)
    so_far = available_len - 1;

  memset (argv[0], 0, available_len);
  memcpy (argv[0], new_argv, so_far);
}

/* Entrypoint to the container.  */
સ્થિર પૂર્ણાંક
container_init (શૂન્ય *args, અક્ષર *notify_socket, પૂર્ણાંક sync_socket, libcrun_error_t *err)
{
  સંરચના container_entrypoint_s *entrypoint_args = args;
  પૂર્ણાંક ret;
  runtime_spec_schema_config_schema *def = entrypoint_args->container->container_def;
  cleanup_free અક્ષર *exec_path = ખાલી;
  __attribute__ ((unused)) cleanup_free અક્ષર *notify_socket_cleanup = notify_socket;
  pid_t own_pid = 0;

  entrypoint_args->sync_socket = sync_socket;

  crun_set_output_handler (log_write_to_sync_socket, args);

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (પઠ (sync_socket, &own_pid, કદ (own_pid)));
  જો (UNLIKELY (ret != કદ (own_pid)))
    {
      જો (ret >= 0)
        errno = 0;
      પાછા crun_make_error (err, errno, "read from sync socket");
    }

  ret = container_init_setup (args, &own_pid, notify_socket, sync_socket, &exec_path, err);
  જો (UNLIKELY (ret < 0))
    {
      /* If it fails to write the error using the sync socket, then fallback
         to stderr.  */
      જો (sync_socket_write_error (sync_socket, err) == 0)
        {
          crun_error_release (err);
          /* A return value of > 0 means to fail with an error without err being set so to avoid
             printing it twice.  */
          પાછા 1;
        }
      પાછા ret;
    }

  entrypoint_args->sync_socket = -1;

  ret = unblock_signals (err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  /* sync 4.  */
  ret = sync_socket_send_sync (sync_socket, false, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  close_and_reset (&sync_socket);

  જો (entrypoint_args->context->fifo_exec_wait_fd >= 0)
    {
      અક્ષર buffer[1];
      fd_set read_set;
      cleanup_close પૂર્ણાંક fd = entrypoint_args->context->fifo_exec_wait_fd;
      entrypoint_args->context->fifo_exec_wait_fd = -1;

      FD_ZERO (&read_set);
      FD_SET (fd, &read_set);
      કરો
        {
          ret = select (fd + 1, &read_set, ખાલી, ખાલી, ખાલી);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "select");

          ret = TEMP_FAILURE_RETRY (પઠ (fd, buffer, કદ (buffer)));
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "read from the exec fifo");
      } જ્યારે (ret == 0);

      close_and_reset (&entrypoint_args->context->fifo_exec_wait_fd);
    }

  crun_set_output_handler (log_write_to_stderr, ખાલી);

  જો (def->process && def->process->no_new_privileges)
    {
      અક્ષર **seccomp_flags = ખાલી;
      size_t seccomp_flags_len = 0;
      cleanup_free અક્ષર *seccomp_fd_payload = ખાલી;
      size_t seccomp_fd_payload_len = 0;

      જો (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      જો (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (entrypoint_args->container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags,
                                   seccomp_flags_len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  જો (UNLIKELY (def->process == ખાલી))
    પાછા crun_make_error (err, 0, "block `process` not found");

  જો (UNLIKELY (exec_path == ખાલી))
    પાછા crun_make_error (err, 0, "executable path not specified");

  જો (def->hooks && def->hooks->start_container_len)
    {
      libcrun_container_t *container = entrypoint_args->container;
      libcrun_error_t tmp_err = ખાલી;
      પૂર્ણાંક in_userns = check_running_in_user_namespace (&tmp_err);
      જો (tmp_err)
        crun_error_release (&tmp_err);

      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, ખાલી, "created", "startContainer",
                      (hook **) def->hooks->start_container,
                      def->hooks->start_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      જો (UNLIKELY (ret != 0))
        પાછા ret;

      /* Seek stdout/stderr to the end.  If the hooks were using the same files,
         the container process overwrites what was previously written.  */
      (શૂન્ય) lseek (1, 0, SEEK_END);
      (શૂન્ય) lseek (2, 0, SEEK_END);
    }

  જો (entrypoint_args->custom_handler)
    {
      /* Files marked with O_CLOEXEC are closed at execv time, so make sure they are closed now.
         This is a best effort operation, because the seccomp filter is already in place and it could
         stop some syscalls used by mark_or_close_fds_ge_than.
      */
      જો (entrypoint_args->custom_handler->vtable->close_fds)
        {
          ret = entrypoint_args->custom_handler->vtable->close_fds (entrypoint_args->custom_handler->cookie,
                                                                    entrypoint_args->container,
                                                                    entrypoint_args->context->preserve_fds,
                                                                    err);
        }
      નહીંતર
        {
          ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
        }
      જો (UNLIKELY (ret < 0))
        crun_error_release (err);

      prctl (PR_SET_NAME, entrypoint_args->custom_handler->vtable->name);

      જો (entrypoint_args->context->argv)
        {
          erase_initial_environment (entrypoint_args->context->argv, entrypoint_args->context->argc);
          rewrite_argv (entrypoint_args->context->argv, entrypoint_args->context->argc,
                        entrypoint_args->custom_handler->vtable->name, def->process->args,
                        def->process->args_len);

          /* It is a quite destructive operation as we might be referencing data from the old
             argv, so make sure the context is not reused.  */
          entrypoint_args->context->argv = ખાલી;
          entrypoint_args->context = ખાલી;
        }

      ret = libcrun_set_selinux_label (entrypoint_args->container, def->process, true, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = libcrun_set_apparmor_profile (entrypoint_args->container, def->process, true, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = entrypoint_args->custom_handler->vtable->run_func (entrypoint_args->custom_handler->cookie,
                                                               entrypoint_args->container,
                                                               exec_path,
                                                               def->process->args);
      જો (ret != 0)
        પાછા crun_make_error (err, 0, "exec container process failed with handler as `%s`", entrypoint_args->custom_handler->vtable->name);

      પાછા ret;
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation since the seccomp profile is already in place now and might block
     some of the syscalls needed by mark_or_close_fds_ge_than.  */
  ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
  જો (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, def->process->args));

  જો (errno == ENOENT)
    પાછા crun_make_error (err, errno, "exec container process (missing dynamic library?) `%s`", exec_path);

  પાછા crun_make_error (err, errno, "exec container process `%s`", exec_path);
}

સ્થિર પૂર્ણાંક
read_container_config_from_state (libcrun_container_t **container, અચલ અક્ષર *state_મૂળ, અચલ અક્ષર *id,
                                  libcrun_error_t *err)
{
  cleanup_free અક્ષર *config_file = ખાલી;
  cleanup_free અક્ષર *dir = ખાલી;
  પૂર્ણાંક ret;

  *container = ખાલી;

  ret = libcrun_get_state_directory (&dir, state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = append_paths (&config_file, err, dir, "config.json", ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  *container = libcrun_container_load_from_file (config_file, err);
  જો (*container == ખાલી)
    પાછા -1;

  પાછા 0;
}

સ્થિર પૂર્ણાંક
run_poststop_hooks (arg_unused libcrun_context_t *context, libcrun_container_t *container,
                    runtime_spec_schema_config_schema *def,
                    libcrun_container_status_t *status, અચલ અક્ષર *state_મૂળ, અચલ અક્ષર *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container_cleanup = ખાલી;
  પૂર્ણાંક ret;

  જો (def == ખાલી)
    {
      જો (container == ખાલી)
        {
          ret = read_container_config_from_state (&container_cleanup, state_મૂળ, id, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
          container = container_cleanup;
        }

      def = container->container_def;
    }

  જો (def->hooks && def->hooks->poststop_len)
    {
      cleanup_close પૂર્ણાંક hooks_out_fd = -1;
      cleanup_close પૂર્ણાંક hooks_err_fd = -1;

      જો (container == ખાલી)
        {
          ret = read_container_config_from_state (&container_cleanup, state_મૂળ, id, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
          container = container_cleanup;
        }

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = do_hooks (def, 0, id, true, status->bundle, status->bundle, "stopped", "poststop", (hook **) def->hooks->poststop,
                      def->hooks->poststop_len, hooks_out_fd, hooks_err_fd, false, err);
      જો (UNLIKELY (ret != 0))
        {
          જો (ret < 0)
            libcrun_error_report_and_release (err);
          નહીંતર
            libcrun_error (0, "poststop hook failed with exit code: %d", ret);
        }
    }
  પાછા 0;
}

સ્થિર bool
has_new_pid_namespace (runtime_spec_schema_config_schema *def)
{
  size_t i;

  જો (def->linux == ખાલી || def->linux->namespaces == ખાલી)
    પાછા false;

  માટે (i = 0; i < def->linux->namespaces_len; i++)
    {
      જો (strcmp (def->linux->namespaces[i]->type, "pid") == 0 && def->linux->namespaces[i]->path == ખાલી)
        પાછા true;
    }
  પાછા false;
}

/* Delete the container state.  CONTAINER is the container the caller already has
   available, or NULL.  When it is not NULL, the poststop hooks are run even if the
   status file was never written, e.g. when the container creation failed.  */
સ્થિર પૂર્ણાંક
container_delete_internal (libcrun_context_t *context, libcrun_container_t *container,
                           runtime_spec_schema_config_schema *def, અચલ અક્ષર *id, bool force, bool killall,
                           libcrun_error_t *err)
{
  cleanup_cgroup_status સંરચના libcrun_cgroup_status *cgroup_status = ખાલી;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_container libcrun_container_t *container_cleanup = ખાલી;
  અચલ અક્ષર *state_મૂળ = context->state_મૂળ;
  પૂર્ણાંક ret;

  ret = libcrun_read_container_status (&status, state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    {
      જો (force && crun_error_get_errno (err) == ENOENT)
        {
          libcrun_error_t tmp_err = ખાલી;

          crun_error_release (err);

          જો (container)
            {
              libcrun_container_status_t partial_status = {
                .bundle = (અક્ષર *) context->bundle,
              };

              ret = run_poststop_hooks (context, container, def, &partial_status, state_મૂળ, id, err);
              જો (UNLIKELY (ret < 0))
                crun_error_write_warning_and_release (context->output_handler_arg, &err);
            }

          libcrun_container_delete_status (state_મૂળ, id, &tmp_err);
          crun_error_release (&tmp_err);
          પાછા 0;
        }
      crun_error_release (err);
      પાછા libcrun_container_delete_status (state_મૂળ, id, err);
    }

  cgroup_status = libcrun_cgroup_make_status (&status);

  જો (! force)
    {
      ret = libcrun_is_container_running (&status, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
      જો (ret == 1)
        {
          ret = libcrun_status_has_read_exec_fifo (state_મૂળ, id, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
          જો (ret == 0)
            પાછા crun_make_error (err, 0, "the container `%s` is not in `created` or `stopped` state", id);

          /* If the container is in "created" state, then do the equivalent of delete --force.  */
          killall = force = true;
        }
    }

  જો (def == ખાલી)
    {
      જો (container == ખાલી)
        {
          ret = read_container_config_from_state (&container_cleanup, state_મૂળ, id, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;

          container = container_cleanup;
        }

      def = container->container_def;
    }

  જો (killall && force)
    {
      /* If the container has a pid namespace, it is enough to kill the first
         process (pid=1 in the namespace).
      */
      જો (has_new_pid_namespace (def))
        {
          ret = libcrun_kill_linux (&status, SIGKILL, err);
          જો (UNLIKELY (ret < 0))
            {
              જો (crun_error_get_errno (err) != ESRCH)
                પાછા ret;

              crun_error_release (err);
            }
        }
      નહીંતર જો (status.cgroup_path)
        {
          ret = libcrun_cgroup_killall (cgroup_status, SIGKILL, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }
    }

  જો (def->linux && def->linux->intel_rdt)
    {
      ret = libcrun_destroy_intelrdt (id, def, err);
      જો (UNLIKELY (ret < 0))
        libcrun_error_report_and_release (err);
    }

  જો (status.cgroup_path)
    {
      /* If the cgroup cannot be destroyed, there are still processes in it.
         Keep the container status, it is the only record of the cgroup, so
         that the delete can be retried.  */
      ret = libcrun_cgroup_destroy (cgroup_status, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  ret = run_poststop_hooks (context, container, def, &status, state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  પાછા libcrun_container_delete_status (state_મૂળ, id, err);
}

પૂર્ણાંક
libcrun_container_delete (libcrun_context_t *context, અચલ અક્ષર *id, bool force, libcrun_error_t *err)
{
  પાછા container_delete_internal (context, ખાલી, ખાલી, id, force, true, err);
}

પૂર્ણાંક
libcrun_container_kill (libcrun_context_t *context, અચલ અક્ષર *id, અચલ અક્ષર *સંકેત, libcrun_error_t *err)
{
  પૂર્ણાંક sig, ret;
  અચલ અક્ષર *state_મૂળ = context->state_મૂળ;
  cleanup_container_status libcrun_container_status_t status = {};

  sig = str2sig (સંકેત);
  જો (UNLIKELY (sig < 0))
    પાછા crun_make_error (err, 0, "unknown signal `%s`", સંકેત);

  ret = libcrun_read_container_status (&status, state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા libcrun_kill_linux (&status, sig, err);
}

પૂર્ણાંક
libcrun_container_killall (libcrun_context_t *context, અચલ અક્ષર *id, અચલ અક્ષર *સંકેત, libcrun_error_t *err)
{
  પૂર્ણાંક sig, ret;
  અચલ અક્ષર *state_મૂળ = context->state_મૂળ;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_cgroup_status સંરચના libcrun_cgroup_status *cgroup_status = ખાલી;

  sig = str2sig (સંકેત);
  જો (UNLIKELY (sig < 0))
    પાછા crun_make_error (err, 0, "unknown signal `%s`", સંકેત);

  ret = libcrun_read_container_status (&status, state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  cgroup_status = libcrun_cgroup_make_status (&status);

  ret = libcrun_cgroup_killall (cgroup_status, sig, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;
  પાછા 0;
}

સ્થિર પૂર્ણાંક
write_container_status (libcrun_container_t *container, libcrun_context_t *context,
                        pid_t pid, સંરચના libcrun_cgroup_status *cgroup_status,
                        libcrun_error_t *err)
{
  cleanup_free અક્ષર *bundle_allocated = ખાલી;
  અચલ અક્ષર *bundle = get_bundle_path (context->bundle, &bundle_allocated);
  cleanup_free અક્ષર *owner = get_user_name (geteuid ());
  અક્ષર *external_descriptors = libcrun_get_external_descriptors (container);
  અક્ષર *rootfs = container->container_def->મૂળ ? container->container_def->મૂળ->path : "";
  અક્ષર created[35];

  libcrun_container_status_t status = {
    .pid = pid,
    .rootfs = rootfs,
    .bundle = (અક્ષર *) bundle,
    .created = created,
    .owner = owner,
    .systemd_cgroup = context->systemd_cgroup,
    .detached = context->detach,
    .no_new_keyring = context->no_new_keyring,
    .external_descriptors = external_descriptors,
    .cgroup_path = ખાલી,
    .scope = ખાલી,
  };

  get_current_timestamp (created, કદ (created));

  જો (cgroup_status)
    {
      પૂર્ણાંક ret;

      ret = libcrun_cgroup_get_status (cgroup_status, &status, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (external_descriptors == ખાલી)
    પાછા crun_make_error (err, 0, "invalid internal state.  No external descriptors found");
  પાછા libcrun_write_container_status (context->state_મૂળ, context->id, &status, err);
}

સ્થિર પૂર્ણાંક
reap_subprocesses (pid_t *main_process, પૂર્ણાંક *main_process_exit, પૂર્ણાંક *last_process, libcrun_error_t *err)
{
  *last_process = 0;
  જ્યારે (1)
    {
      પૂર્ણાંક status;
      પૂર્ણાંક r = waitpid_ignore_stopped (-1, &status, WNOHANG);
      જો (r < 0)
        {
          જો (errno == EINTR)
            ચાલુ;
          જો (errno == ECHILD)
            {
              *last_process = 1;
              પાછા 0;
            }
          પાછા crun_make_error (err, errno, "waitpid");
        }
      જો (r == 0)
        તોડો;
      જો (r != *main_process)
        ચાલુ;

      *main_process_exit = get_process_exit_status (status);

      /* Once reaped, the pid can be reused by an unrelated process.  */
      *main_process = 0;
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
send_sd_notify (અચલ અક્ષર *ready_str, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  પૂર્ણાંક ret = sd_notify (0, ready_str);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, -ret, "sd_notify");

#  જો HAVE_SD_NOTIFY_BARRIER
  /* Hard-code a 30 seconds timeout.  Ignore errors.  */
  sd_notify_barrier (0, 30 * 1000000);
#  endif
#નહીંતર
  (શૂન્ય) ready_str;
  (શૂન્ય) err;
#endif
  પાછા 0;
}

સ્થિર પૂર્ણાંક
handle_notify_socket (પૂર્ણાંક notify_socketfd, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  પૂર્ણાંક ret;
  અક્ષર buf[256];
  અચલ અક્ષર *ready_str = "READY=1";

  ret = recvfrom (notify_socketfd, buf, કદ (buf) - 1, 0, ખાલી, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "recvfrom notify socket");

  buf[ret] = '\0';
  જો (strstr (buf, ready_str))
    {
      ret = send_sd_notify (ready_str, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      પાછા 1;
    }
  પાછા 0;
#નહીંતર
  (શૂન્ય) notify_socketfd;
  (શૂન્ય) err;
  પાછા 1;
#endif
}

સંરચના wait_for_process_args
{
  pid_t pid;
  libcrun_context_t *context;
  પૂર્ણાંક terminal_fd;
  પૂર્ણાંક notify_socket;
  પૂર્ણાંક *container_ready_fd;
  પૂર્ણાંક seccomp_notify_fd;
  અચલ અક્ષર *seccomp_notify_plugins;
};

સ્થિર પૂર્ણાંક
write_pid_file (અચલ અક્ષર *pid_file, pid_t pid, libcrun_error_t *err)
{
  અક્ષર buf[32];
  પૂર્ણાંક buf_len = snprintf (buf, કદ (buf), "%d", pid);
  જો (UNLIKELY (buf_len >= (પૂર્ણાંક) કદ (buf)))
    પાછા crun_make_error (err, 0, "internal error: static buffer too small");
  પાછા write_file_at_with_flags (AT_FDCWD, O_CREAT | O_TRUNC, 0700, pid_file, buf, buf_len, err);
}

#define MAX_EVENTS 10

સ્થિર bool
is_fd_pollable (પૂર્ણાંક fd)
{
  સંરચના epoll_event ev = { .events = EPOLLIN };
  cleanup_close પૂર્ણાંક epollfd = epoll_create1 (EPOLL_CLOEXEC);

  જો (epollfd < 0)
    પાછા true;

  પાછા epoll_ctl (epollfd, EPOLL_CTL_ADD, fd, &ev) == 0 || errno != EPERM;
}

સ્થિર પૂર્ણાંક
wait_for_process (સંરચના wait_for_process_args *args, libcrun_error_t *err)
{
  cleanup_channel_fd_pair સંરચના channel_fd_pair *from_terminal = ખાલી;
  cleanup_channel_fd_pair સંરચના channel_fd_pair *to_terminal = ખાલી;
  પૂર્ણાંક ret, container_exit_code = 0, last_process;
  cleanup_close પૂર્ણાંક terminal_fd_from = -1;
  cleanup_close પૂર્ણાંક terminal_fd_to = -1;
  bool stdin_pollable = false, stdout_pollable = false;
  cleanup_close પૂર્ણાંક epollfd = -1;
  cleanup_close પૂર્ણાંક signalfd = -1;
  sigset_t mask;
  પૂર્ણાંક in_fds[MAX_EVENTS];
  પૂર્ણાંક in_fds_len = 0;
  પૂર્ણાંક out_fds[MAX_EVENTS];
  પૂર્ણાંક out_fds_len = 0;
  size_t i;

  cleanup_seccomp_notify_context સંરચના seccomp_notify_context_s *seccomp_notify_ctx = ખાલી;

  container_exit_code = 0;

  જો (args == ખાલી || args->context == ખાલી)
    પાછા crun_make_error (err, 0, "internal error: context is empty");

  માટે (i = 0; i < MAX_EVENTS; i++)
    {
      in_fds[i] = -1;
      out_fds[i] = -1;
    }

  જો (args->context->pid_file)
    {
      ret = write_pid_file (args->context->pid_file, args->pid, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  /* Also exit if there is nothing more to wait for.  */
  જો (args->context->detach && args->notify_socket < 0)
    પાછા 0;

  જો (args->container_ready_fd)
    {
      ret = 0;
      TEMP_FAILURE_RETRY (લિખ (*args->container_ready_fd, &ret, કદ (ret)));
      close_and_reset (args->container_ready_fd);
    }

  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "sigprocmask");

  signalfd = create_signalfd (&mask, err);
  જો (UNLIKELY (signalfd < 0))
    પાછા signalfd;

  ret = reap_subprocesses (&args->pid, &container_exit_code, &last_process, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (last_process)
    પાછા container_exit_code;

  જો (args->seccomp_notify_fd >= 0)
    {
      cleanup_free અક્ષર *state_મૂળ = ખાલી;
      cleanup_free અક્ષર *oci_config_path = ખાલી;

      સંરચના libcrun_load_seccomp_notify_conf_s conf;
      memset (&conf, 0, કદ conf);

      ret = libcrun_get_state_directory (&state_મૂળ, args->context->state_મૂળ, args->context->id, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = append_paths (&oci_config_path, err, state_મૂળ, "config.json", ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      conf.runtime_મૂળ_path = state_મૂળ;
      conf.name = args->context->id;
      conf.bundle_path = args->context->bundle;
      conf.oci_config_path = oci_config_path;

      ret = set_blocking_fd (args->seccomp_notify_fd, false, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = libcrun_load_seccomp_notify_plugins (&seccomp_notify_ctx,
                                                 args->seccomp_notify_plugins,
                                                 &conf, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      in_fds[in_fds_len++] = args->seccomp_notify_fd;
    }

  જો (args->terminal_fd >= 0)
    {
      /* The terminal_fd is dup()ed so that it can be registered with
         epoll multiple times using different masks.  */
      terminal_fd_from = dup (args->terminal_fd);
      જો (UNLIKELY (terminal_fd_from < 0))
        પાછા crun_make_error (err, errno, "dup terminal fd");
      terminal_fd_to = dup (args->terminal_fd);
      જો (UNLIKELY (terminal_fd_to < 0))
        પાછા crun_make_error (err, errno, "dup terminal fd");

      પૂર્ણાંક i, non_blocking_fds[] = { terminal_fd_from, terminal_fd_to, 0, 1, -1 };
      માટે (i = 0; non_blocking_fds[i] >= 0; i++)
        {
          ret = set_blocking_fd (non_blocking_fds[i], false, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }

      /* stdin and stdout can be something epoll does not support, such as
         /dev/null or a regular file.  Such a stdin is not forwarded, and
         such a stdout is written to without waiting for it to be ready.  */
      stdin_pollable = is_fd_pollable (0);
      stdout_pollable = is_fd_pollable (1);

      from_terminal = channel_fd_pair_new (terminal_fd_from, 1, BUFSIZ);
      જો (stdin_pollable)
        to_terminal = channel_fd_pair_new (0, terminal_fd_to, BUFSIZ);
    }

  in_fds[in_fds_len++] = signalfd;
  જો (args->notify_socket >= 0)
    in_fds[in_fds_len++] = args->notify_socket;
  જો (args->terminal_fd >= 0)
    {
      જો (stdin_pollable)
        {
          in_fds[in_fds_len++] = 0;
          out_fds[out_fds_len++] = terminal_fd_to;
        }

      in_fds[in_fds_len++] = terminal_fd_from;
      જો (stdout_pollable)
        out_fds[out_fds_len++] = 1;
    }

  epollfd = epoll_helper (in_fds, ખાલી, out_fds, ખાલી, err);
  જો (UNLIKELY (epollfd < 0))
    પાછા epollfd;

  જ્યારે (1)
    {
      સંરચના epoll_event events[MAX_EVENTS];
      સંરચના signalfd_siginfo si;
      સંરચના winsize ws;
      પૂર્ણાંક i, nr_events;
      ssize_t res;

      nr_events = TEMP_FAILURE_RETRY (epoll_wait (epollfd, events, MAX_EVENTS, -1));
      જો (UNLIKELY (nr_events < 0))
        પાછા crun_make_error (err, errno, "epoll_wait");

      માટે (i = 0; i < nr_events; i++)
        {
          જો (events[i].data.fd == 0 || events[i].data.fd == terminal_fd_to)
            {
              ret = channel_fd_pair_process (to_terminal, epollfd, err);
              જો (UNLIKELY (ret < 0))
                પાછા crun_error_wrap (err, "copy to terminal fd");
            }
          નહીંતર જો (events[i].data.fd == 1 || events[i].data.fd == terminal_fd_from)
            {
              ret = channel_fd_pair_process (from_terminal, epollfd, err);
              જો (UNLIKELY (ret < 0))
                પાછા crun_error_wrap (err, "copy from terminal fd");
            }
          નહીંતર જો (events[i].data.fd == args->seccomp_notify_fd)
            {
              ret = libcrun_seccomp_notify_plugins (seccomp_notify_ctx,
                                                    args->seccomp_notify_fd, err);
              જો (UNLIKELY (ret < 0))
                પાછા ret;
            }
          નહીંતર જો (events[i].data.fd == args->notify_socket)
            {
              ret = handle_notify_socket (args->notify_socket, err);
              જો (UNLIKELY (ret < 0))
                પાછા ret;
              જો (ret && args->context->detach)
                પાછા 0;
            }
          નહીંતર જો (events[i].data.fd == signalfd)
            {
              res = TEMP_FAILURE_RETRY (પઠ (signalfd, &si, કદ (si)));
              જો (UNLIKELY (res < 0))
                પાછા crun_make_error (err, errno, "read from signalfd");
              જો (si.ssi_signo == SIGCHLD)
                {
                  ret = reap_subprocesses (&args->pid, &container_exit_code,
                                           &last_process, err);
                  જો (UNLIKELY (ret < 0))
                    પાછા ret;
                  જો (last_process)
                    પાછા container_exit_code;
                }
              નહીંતર જો (si.ssi_signo == SIGWINCH)
                {
                  /* Ignore the signal if the terminal is not available.  */
                  જો (args->terminal_fd > 0)
                    {
                      ret = ioctl (0, TIOCGWINSZ, &ws);
                      જો (UNLIKELY (ret < 0))
                        પાછા crun_make_error (err, errno, "ioctl TIOCGWINSZ copy terminal size from stdin");

                      ret = ioctl (args->terminal_fd, TIOCSWINSZ, &ws);
                      જો (UNLIKELY (ret < 0))
                        પાછા crun_make_error (err, errno, "ioctl TIOCSWINSZ copy terminal size to pty");
                    }
                }
              નહીંતર જો (args->pid > 0)
                {
                  /* Send any other signal to the child process, if it was not reaped yet.  */
                  ret = kill (args->pid, si.ssi_signo);
                }
            }
          નહીંતર
            {
              પાછા crun_make_error (err, 0, "internal error: unknown fd from epoll_wait");
            }
        }
    }

  પાછા 0;
}

સ્થિર શૂન્ય
flush_fd_to_err (libcrun_context_t *context, પૂર્ણાંક terminal_fd)
{
  અક્ષર buf[256];
  પૂર્ણાંક flags;

  જો (terminal_fd < 0 || stderr == ખાલી)
    પાછા;

  flags = fcntl (terminal_fd, F_GETFL, 0);
  જો (flags == -1)
    પાછા;
  જો (fcntl (terminal_fd, F_SETFL, flags | O_NONBLOCK) < 0)
    પાછા;

  માટે (;;)
    {
      પૂર્ણાંક ret = TEMP_FAILURE_RETRY (પઠ (terminal_fd, buf, કદ (buf) - 1));
      જો (ret <= 0)
        તોડો;
      buf[ret] = '\0';
      જો (context->output_handler)
        context->output_handler (0, buf, LIBCRUN_VERBOSITY_ERROR, context->output_handler_arg);
    }
  (શૂન્ય) fcntl (terminal_fd, F_SETFL, flags);
  fflush (stderr);
  fsync (1);
  fsync (2);
}

સ્થિર પૂર્ણાંક
cleanup_watch (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
               સંરચના libcrun_cgroup_status *cgroup_status, pid_t init_pid, પૂર્ણાંક sync_socket,
               પૂર્ણાંક terminal_fd, libcrun_error_t *err)
{
  અચલ અક્ષર *oom_message = ખાલી;
  libcrun_error_t tmp_err = ખાલી;
  પૂર્ણાંક ret;

  જો (init_pid)
    {
      /* Try to detect whether the cgroup has a OOM.  */
      જો (cgroup_status)
        {
          પૂર્ણાંક has_oom;

          has_oom = libcrun_cgroup_has_oom (cgroup_status, &tmp_err);
          જો (has_oom > 0)
            oom_message = "OOM: the memory limit could be too low";
          નહીંતર જો (has_oom < 0)
            {
              /* If the detection has failed for any reason, e.g. the cgroup was
                 already deleted by the time it was checked, just ignore the
                 failure.  */
              crun_error_release (&tmp_err);
            }
        }
      /* If the OOM wasn't detected, look into the static configuration.  */
      જો (oom_message == ખાલી && is_memory_limit_too_નીચલું (def))
        oom_message = "the memory limit could be too low";

      kill (init_pid, SIGKILL);
      waitpid_ignore_stopped (init_pid, ખાલી, 0);
    }

  ret = sync_socket_wait_sync (context, sync_socket, true, &tmp_err);
  જો (UNLIKELY (ret < 0))
    {
      crun_error_release (err);
      *err = tmp_err;
    }

  જો (terminal_fd >= 0)
    flush_fd_to_err (context, terminal_fd);

  જો (oom_message)
    પાછા crun_error_wrap (err, "%s", oom_message);

  પાછા -1;
}

/* Find the uid:gid that is mapped to root inside the container user namespace.  */
શૂન્ય
get_મૂળ_in_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid, uid_t *uid,
                        gid_t *gid)
{
  *uid = -1;
  *gid = -1;

  /* Not root in the namespace.  */
  જો (host_uid)
    પાછા;

  જો (def->linux && def->linux->uid_mappings)
    {
      size_t i;

      માટે (i = 0; i < def->linux->uid_mappings_len; i++)
        જો (def->linux->uid_mappings[i]->container_id == 0)
          {
            *uid = def->linux->uid_mappings[i]->host_id;
            તોડો;
          }
    }
  જો (def->linux && def->linux->gid_mappings)
    {
      size_t i;

      માટે (i = 0; i < def->linux->gid_mappings_len; i++)
        જો (def->linux->gid_mappings[i]->container_id == 0)
          {
            *gid = def->linux->gid_mappings[i]->host_id;
            તોડો;
          }
    }

  /* If the uid and the gid are not changed, do not attempt any chown.  */
  જો (*uid == host_uid && *gid == host_gid)
    *uid = *gid = -1;
}

સ્થિર પૂર્ણાંક
get_seccomp_receiver_fd (libcrun_container_t *container, પૂર્ણાંક *fd, પૂર્ણાંક *self_receiver_fd, અચલ અક્ષર **plugins,
                         libcrun_error_t *err)
{
  અચલ અક્ષર *tmp;
  runtime_spec_schema_config_schema *def = container->container_def;

  *fd = -1;
  *self_receiver_fd = -1;

  tmp = find_annotation (container, "run.oci.seccomp.plugins");
  જો (tmp)
    {
      પૂર્ણાંક fds[2];
      પૂર્ણાંક ret;

      ret = create_socket_pair (fds, err);
      જો (UNLIKELY (ret < 0))
        પાછા crun_error_wrap (err, "create socket pair");

      *fd = fds[0];
      *self_receiver_fd = fds[1];
      *plugins = tmp;
      પાછા 0;
    }

  જો (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    tmp = def->linux->seccomp->listener_path;
  નહીંતર
    tmp = find_annotation (container, "run.oci.seccomp.receiver");
  જો (tmp == ખાલી)
    tmp = getenv ("RUN_OCI_SECCOMP_RECEIVER");
  જો (tmp)
    {
      જો (tmp[0] != '/')
        પાછા crun_make_error (err, 0, "the seccomp receiver `%s` is not an absolute path", tmp);

      *fd = open_unix_domain_client_socket (tmp, 0, err);
      જો (UNLIKELY (*fd < 0))
        પાછા crun_error_wrap (err, "open seccomp receiver");
    }

  પાછા 0;
}

સ્થિર bool
has_seccomp_receiver (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;

  જો (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    પાછા true;

  જો (find_annotation (container, "run.oci.seccomp.receiver") != ખાલી || getenv ("RUN_OCI_SECCOMP_RECEIVER") != ખાલી)
    પાછા true;

  પાછા false;
}

સ્થિર પૂર્ણાંક
setup_container_hooks_output (libcrun_container_t *container, runtime_spec_schema_config_schema *def,
                              સંરચના container_entrypoint_s *container_args, પૂર્ણાંક *hooks_out_fd,
                              પૂર્ણાંક *hooks_err_fd, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  જો (def->hooks
      && (def->hooks->prestart_len || def->hooks->poststart_len || def->hooks->create_runtime_len
          || def->hooks->create_container_len || def->hooks->start_container_len))
    {
      ret = open_hooks_output (container, hooks_out_fd, hooks_err_fd, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
      container_args->hooks_out_fd = *hooks_out_fd;
      container_args->hooks_err_fd = *hooks_err_fd;
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
setup_container_keyring (libcrun_container_t *container, libcrun_context_t *context, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  પૂર્ણાંક ret;

  જો (! context->no_new_keyring)
    {
      અચલ અક્ષર *label = ખાલી;

      libcrun_debug ("Creating new keyring");

      જો (def->process)
        {
          label = def->process->selinux_label;
          જો (label)
            libcrun_debug ("Using SELinux process label: `%s`", label);
        }

      ret = libcrun_create_keyring (container, container->context->id, label, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
setup_terminal_socket_pair (libcrun_container_t *container, libcrun_context_t *context,
                            સંરચના container_entrypoint_s *container_args, પૂર્ણાંક *socket_pair_0,
                            પૂર્ણાંક *socket_pair_1, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  પૂર્ણાંક ret;
  પૂર્ણાંક detach = context->detach;

  જો (def->process && def->process->terminal && ! detach && context->console_socket == ખાલી)
    {
      libcrun_debug ("Creating terminal socket pair");
      container_args->has_terminal_socket_pair = 1;
      ret = create_socket_pair (container_args->terminal_socketpair, err);
      જો (UNLIKELY (ret < 0))
        પાછા crun_error_wrap (err, "create terminal socket");

      *socket_pair_0 = container_args->terminal_socketpair[0];
      *socket_pair_1 = container_args->terminal_socketpair[1];
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
setup_seccomp (libcrun_container_t *container, અચલ અક્ષર *seccomp_bpf_data,
               સંરચના libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, પૂર્ણાંક *seccomp_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  પૂર્ણાંક ret;

  જો (find_annotation (container, "run.oci.seccomp.plugins") != ખાલી && has_seccomp_receiver (container))
    {
      પાછા crun_make_error (err, 0, "seccomp plugins and seccomp receivers cannot be declared at the same time");
    }

  જો (def->linux && (def->linux->seccomp || seccomp_bpf_data))
    {
      unsigned પૂર્ણાંક seccomp_gen_options = 0;
      અચલ અક્ષર *annotation;

      libcrun_debug ("Initializing seccomp");
      annotation = find_annotation (container, "run.oci.seccomp_fail_unknown_syscall");
      જો (annotation && strcmp (annotation, "0") != 0)
        seccomp_gen_options = LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL;

      જો (seccomp_bpf_data)
        seccomp_gen_options |= LIBCRUN_SECCOMP_SKIP_CACHE;

      libcrun_seccomp_gen_ctx_init (seccomp_gen_ctx, container, true, seccomp_gen_options);

      ret = libcrun_open_seccomp_bpf (seccomp_gen_ctx, seccomp_fd, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
setup_console_socket (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
                      સંરચના container_entrypoint_s *container_args, પૂર્ણાંક *console_socket_fd,
                      libcrun_error_t *err)
{
  જો (context->console_socket && def->process && def->process->terminal)
    {
      *console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
      જો (UNLIKELY (*console_socket_fd < 0))
        પાછા crun_error_wrap (err, "open console socket");
      container_args->console_socket_fd = *console_socket_fd;
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
setup_cgroup_manager (libcrun_context_t *context, libcrun_container_t *container,
                      સંરચના libcrun_cgroup_args *cg, પૂર્ણાંક *cgroup_dirfd,
                      સંરચના libcrun_dirfd_s *cgroup_dirfd_s, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  પૂર્ણાંક cgroup_manager;
  uid_t મૂળ_uid = -1;
  gid_t મૂળ_gid = -1;
  પૂર્ણાંક ret;

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  જો (context->systemd_cgroup)
    {
      libcrun_debug ("Using systemd cgroup manager");
      cgroup_manager = CGROUP_MANAGER_SYSTEMD;
    }
  નહીંતર જો (context->force_no_cgroup)
    {
      libcrun_debug ("Disabling cgroup manager");
      cgroup_manager = CGROUP_MANAGER_DISABLED;
    }
  નહીંતર
    libcrun_debug ("Using cgroupfs cgroup manager");

  /* If we are root (either on the host or in a namespace), then chown the cgroup to root
     in the container user namespace.  */
  get_મૂળ_in_the_userns (def, container->host_uid, container->host_gid, &મૂળ_uid, &મૂળ_gid);
  libcrun_debug ("Using container host UID `%d` and GID `%d`", container->host_uid, container->host_gid);

  memset (cg, 0, કદ (*cg));

  cg->cgroup_path = def->linux ? def->linux->cgroups_path : "";
  cg->manager = cgroup_manager;
  cg->id = context->id;
  cg->resources = def->linux ? def->linux->resources : ખાલી;
  cg->annotations = container->annotations;
  cg->મૂળ_uid = મૂળ_uid;
  cg->મૂળ_gid = મૂળ_gid;
  cg->state_મૂળ = context->state_મૂળ;
  cg->container = container;

  ret = libcrun_cgroup_preenter (cg, cgroup_dirfd, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  cgroup_dirfd_s->dirfd = cgroup_dirfd;
  cgroup_dirfd_s->joined = false;

  પાછા 0;
}

સ્થિર પૂર્ણાંક
set_scheduler (pid_t pid, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  ret = libcrun_set_scheduler (pid, def->process, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_reset_cpu_affinity_mask (pid, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_set_io_priority (pid, def->process, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા 0;
}

સ્થિર પૂર્ણાંક
seccomp_generation (પૂર્ણાંક seccomp_fd, અચલ અક્ષર *seccomp_bpf_data,
                    સંરચના libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  જો (seccomp_fd >= 0)
    {
      જો (seccomp_bpf_data != ખાલી)
        {
          ret = libcrun_copy_seccomp (seccomp_gen_ctx, seccomp_bpf_data, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }
      નહીંતર
        {
          ret = libcrun_generate_seccomp (seccomp_gen_ctx, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
terminal_setup (runtime_spec_schema_config_schema *def, libcrun_context_t *context,
                પૂર્ણાંક socket_pair_0, પૂર્ણાંક *terminal_fd, શૂન્ય **orig_terminal, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  પૂર્ણાંક detach = context->detach;

  જો (def->process && def->process->terminal && ! detach && context->console_socket == ખાલી)
    {
      libcrun_debug ("Receiving console socket fd");
      *terminal_fd = receive_fd_from_socket (socket_pair_0, err);
      જો (UNLIKELY (*terminal_fd < 0))
        પાછા -1;

      ret = libcrun_clear_onlcr (*terminal_fd, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = libcrun_set_raw (0, orig_terminal, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
libcrun_container_run_internal (libcrun_container_t *container, libcrun_context_t *context,
                                પૂર્ણાંક *container_ready_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  પૂર્ણાંક ret;
  pid_t pid;
  પૂર્ણાંક detach = context->detach;
  cleanup_cgroup_status સંરચના libcrun_cgroup_status *cgroup_status = ખાલી;
  cleanup_close પૂર્ણાંક terminal_fd = -1;
  cleanup_terminal શૂન્ય *orig_terminal = ખાલી;
  cleanup_close પૂર્ણાંક sync_socket = -1;
  cleanup_close પૂર્ણાંક notify_socket = -1;
  cleanup_close પૂર્ણાંક socket_pair_0 = -1;
  cleanup_close પૂર્ણાંક socket_pair_1 = -1;
  cleanup_close પૂર્ણાંક seccomp_fd = -1;
  cleanup_close પૂર્ણાંક console_socket_fd = -1;
  cleanup_close પૂર્ણાંક hooks_out_fd = -1;
  cleanup_close પૂર્ણાંક hooks_err_fd = -1;
  cleanup_close પૂર્ણાંક own_seccomp_receiver_fd = -1;
  cleanup_close પૂર્ણાંક seccomp_notify_fd = -1;
  અચલ અક્ષર *seccomp_notify_plugins = ખાલી;
  cleanup_cgroup_preenter સંરચના libcrun_cgroup_args cg = {};
  cleanup_free અક્ષર *bundle_allocated = ખાલી;
  સંરચના container_entrypoint_s container_args = {
    .container = container,
    .context = context,
    .bundle = get_bundle_path (context->bundle, &bundle_allocated),
    .terminal_socketpair = { -1, -1 },
    .console_socket_fd = -1,
    .hooks_out_fd = -1,
    .hooks_err_fd = -1,
    .seccomp_receiver_fd = -1,
    .custom_handler = ખાલી,
  };
  cleanup_close પૂર્ણાંક cgroup_dirfd = -1;
  સંરચના libcrun_dirfd_s cgroup_dirfd_s;
  સંરચના libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  અચલ અક્ષર *seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");
  પૂર્ણાંક cgroup_mode;

  જો (! context->force_no_cgroup)
    {
      cgroup_mode = libcrun_get_cgroup_mode (err);
      જો (UNLIKELY (cgroup_mode < 0))
        પાછા cgroup_mode;

      જો (cgroup_mode != CGROUP_MODE_UNIFIED)
        libcrun_warning ("cgroup v1 is deprecated and will be removed in a future release.  Use cgroup v2");
    }

  ret = setup_container_hooks_output (container, def, &container_args, &hooks_out_fd, &hooks_err_fd, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  container->context = context;

  જો (! detach || context->notify_socket)
    {
      libcrun_debug ("Setting child subreaper");
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "prctl set child subreaper");
    }

  ret = setup_container_keyring (container, context, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = setup_terminal_socket_pair (container, context, &container_args, &socket_pair_0, &socket_pair_1, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = block_signals (err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  umask (0);

  ret = libcrun_set_mempolicy (def, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;
  container_args.seccomp_fd = seccomp_fd;

  જો (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &container_args.seccomp_receiver_fd, &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  ret = setup_console_socket (context, def, &container_args, &console_socket_fd, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = setup_cgroup_manager (context, container, &cg, &cgroup_dirfd, &cgroup_dirfd_s, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_configure_handler (container_args.context->handler_manager,
                                   container_args.context,
                                   container,
                                   &(container_args.custom_handler),
                                   err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (container_args.custom_handler && container_args.custom_handler->vtable->modify_oci_configuration)
    {
      libcrun_debug ("Using custom handler to modify OCI configuration");
      ret = container_args.custom_handler->vtable->modify_oci_configuration (container_args.custom_handler->cookie,
                                                                             container_args.context,
                                                                             container->container_def,
                                                                             err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  pid = libcrun_run_linux_container (container, container_init, &container_args, &sync_socket, &cgroup_dirfd_s, err);
  જો (UNLIKELY (pid < 0))
    પાછા pid;

  cg.pid = pid;
  cg.joined = cgroup_dirfd_s.joined;
  libcrun_debug ("Running container on PID: `%d`", pid);

  જો (context->fifo_exec_wait_fd < 0 && context->notify_socket)
    {
      libcrun_debug ("Using notify socket: `%s`", context->notify_socket);
      /* Do not open the notify socket here on "create".  "start" will take care of it.  */
      ret = get_notify_fd (context, container, &notify_socket, err);
      જો (UNLIKELY (ret < 0))
        જાઓ fail;
    }

  જો (container_args.terminal_socketpair[1] >= 0)
    close_and_reset (&socket_pair_1);

  ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
  જો (UNLIKELY (ret < 0))
    જાઓ fail;

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE, err);
  જો (UNLIKELY (ret < 0))
    જાઓ fail;

  ret = libcrun_move_network_devices (container, pid, err);
  જો (UNLIKELY (ret < 0))
    જાઓ fail;

  /* sync send own pid.  */
  ret = TEMP_FAILURE_RETRY (લિખ (sync_socket, &pid, કદ (pid)));
  જો (UNLIKELY (ret != કદ (pid)))
    {
      જો (ret >= 0)
        errno = 0;
      crun_make_error (err, errno, "write to sync socket");
      જાઓ fail;
    }

  /* sync 1.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  જો (UNLIKELY (ret < 0))
    જાઓ fail;

  /* sync 2.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  જો (UNLIKELY (ret < 0))
    જાઓ fail;

  ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
  જો (UNLIKELY (ret < 0))
    જાઓ fail;

  /* If the container process is created later, the scheduler is set on it then.  */
  જો (! libcrun_container_delayed_pidns_fork (container))
    {
      ret = set_scheduler (pid, def, err);
      જો (UNLIKELY (ret < 0))
        જાઓ fail;
    }

  /* The container is waiting that we write back.  In this phase we can launch the
     prestart hooks.  */
  જો (def->hooks && def->hooks->prestart_len)
    {
      libcrun_debug ("Running `prestart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "prestart", (hook **) def->hooks->prestart,
                      def->hooks->prestart_len, hooks_out_fd, hooks_err_fd, false, err);
      જો (UNLIKELY (ret != 0))
        જાઓ fail;
    }
  જો (def->hooks && def->hooks->create_runtime_len)
    {
      libcrun_debug ("Running `create` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "createRuntime", (hook **) def->hooks->create_runtime,
                      def->hooks->create_runtime_len, hooks_out_fd, hooks_err_fd, false, err);
      જો (UNLIKELY (ret != 0))
        જાઓ fail;
    }

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  જો (UNLIKELY (ret < 0))
    જાઓ fail;
  close_and_reset (&seccomp_fd);

  /* sync 3.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  જો (UNLIKELY (ret < 0))
    જાઓ fail;

  /* The process joining an existing PID namespace is created once the container root is in place.  */
  જો (libcrun_container_delayed_pidns_fork (container))
    {
      pid_t new_pid = 0;

      ret = sync_socket_wait_sync (context, sync_socket, false, err);
      જો (UNLIKELY (ret < 0))
        જાઓ fail;

      ret = TEMP_FAILURE_RETRY (પઠ (sync_socket, &new_pid, કદ (new_pid)));
      જો (UNLIKELY (ret != કદ (new_pid)))
        {
          જો (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "read pid from sync socket");
          જાઓ fail;
        }

      /* Cleanup the first process.  */
      waitpid_ignore_stopped (pid, ખાલી, 0);

      pid = new_pid;

      ret = set_scheduler (pid, def, err);
      જો (UNLIKELY (ret < 0))
        જાઓ fail;

      ret = libcrun_handle_pidfd_receiver (pid, container, err);
      જો (UNLIKELY (ret < 0))
        જાઓ fail;

      /* sync send own pid.  */
      ret = TEMP_FAILURE_RETRY (લિખ (sync_socket, &pid, કદ (pid)));
      જો (UNLIKELY (ret != કદ (pid)))
        {
          જો (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "write to sync socket");
          જાઓ fail;
        }
    }

  ret = terminal_setup (def, context, socket_pair_0, &terminal_fd, &orig_terminal, err);
  જો (UNLIKELY (ret < 0))
    જાઓ fail;

  close_and_reset (&socket_pair_0);

  /* sync 4.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  જો (UNLIKELY (ret < 0))
    જાઓ fail;

  ret = close_and_reset (&sync_socket);
  જો (UNLIKELY (ret < 0))
    {
      crun_make_error (err, errno, "close sync_socket failed");
      જાઓ fail;
    }

  libcrun_debug ("Writing container status");
  ret = write_container_status (container, context, pid, cgroup_status, err);
  જો (UNLIKELY (ret < 0))
    જાઓ fail;

  /* Run poststart hooks here only if the container is created using "run".  For create+start, the
     hooks will be executed as part of the start command.  */
  જો (context->fifo_exec_wait_fd < 0 && def->hooks && def->hooks->poststart_len)
    {
      libcrun_debug ("Running `poststart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      જો (UNLIKELY (ret != 0))
        જાઓ fail;
    }

  /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
  જો (own_seccomp_receiver_fd >= 0)
    {
      libcrun_debug ("Receiving seccomp fd");
      seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
      જો (UNLIKELY (seccomp_notify_fd < 0))
        જાઓ fail;

      ret = close_and_reset (&own_seccomp_receiver_fd);
      જો (UNLIKELY (ret < 0))
        {
          crun_make_error (err, errno, "close seccomp receiver fd failed");
          જાઓ fail;
        }
    }

  {
    સંરચના wait_for_process_args args = {
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
  જો (! context->detach)
    {
      libcrun_error_t tmp_err = ખાલી;
      cleanup_watch (context, def, cgroup_status, 0, sync_socket, terminal_fd, &tmp_err);
      crun_error_release (&tmp_err);
    }

  પાછા ret;

fail:
  ret = cleanup_watch (context, def, cgroup_status, pid, sync_socket, terminal_fd, err);
  જો (cgroup_status)
    {
      libcrun_error_t tmp_err = ખાલી;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  પાછા ret;
}

સ્થિર પૂર્ણાંક
check_config_file (runtime_spec_schema_config_schema *def, libcrun_context_t *context, libcrun_error_t *err)
{
  જો (UNLIKELY (def->linux == ખાલી))
    પાછા crun_make_error (err, 0, "invalid config file, no `linux` block specified");

  જો (context->handler == ખાલી)
    {
      જો (UNLIKELY (def->મૂળ == ખાલી))
        પાછા crun_make_error (err, 0, "invalid config file, no `root` block specified");
      જો (UNLIKELY (def->mounts == ખાલી))
        પાછા crun_make_error (err, 0, "invalid config file, no `mounts` block specified");
    }
  પાછા 0;
}

સ્થિર પૂર્ણાંક
libcrun_copy_config_file (અચલ અક્ષર *id, અચલ અક્ષર *state_મૂળ, libcrun_container_t *container, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  cleanup_free અક્ષર *dest_path = ખાલી;
  cleanup_free અક્ષર *dir = ખાલી;
  cleanup_free અક્ષર *buffer = ખાલી;
  size_t len;

  ret = libcrun_get_state_directory (&dir, state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = append_paths (&dest_path, err, dir, "config.json", ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (container->config_file == ખાલી && container->config_file_content == ખાલી)
    પાછા crun_make_error (err, 0, "config file not specified");

  જો (container->config_file == ખાલી)
    {
      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, container->config_file_content, strlen (container->config_file_content), err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  નહીંતર
    {
      libcrun_debug ("Reading config file: `%s`", container->config_file);
      ret = read_all_file (container->config_file, &buffer, &len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, buffer, len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  પાછા 0;
}

સ્થિર શૂન્ય
force_delete_container_status (libcrun_context_t *context, libcrun_container_t *container,
                               runtime_spec_schema_config_schema *def)
{
  libcrun_error_t tmp_err = ખાલી;
  container_delete_internal (context, container, def, context->id, true, false, &tmp_err);
  libcrun_error_report_and_release (&tmp_err);
}

/* Serialize ERR to FD as an int errno followed by a NUL-terminated message.
   Used by forked children to report a failure to the parent process.  */
સ્થિર શૂન્ય
write_error_to_pipe (પૂર્ણાંક fd, libcrun_error_t *err)
{
  અચલ અક્ષર *સંદેશ = (*err)->સંદેશ;
  પૂર્ણાંક errcode = crun_error_get_errno (err);

  TEMP_FAILURE_RETRY (લિખ (fd, &errcode, કદ (errcode)));
  TEMP_FAILURE_RETRY (લિખ (fd, સંદેશ, strlen (સંદેશ) + 1));
}

પૂર્ણાંક
libcrun_container_run (libcrun_context_t *context, libcrun_container_t *container, unsigned પૂર્ણાંક options,
                       libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  પૂર્ણાંક ret;
  પૂર્ણાંક detach = context->detach;
  પૂર્ણાંક container_ret_status[2];
  cleanup_close પૂર્ણાંક pipefd0 = -1;
  cleanup_close પૂર્ણાંક pipefd1 = -1;
  libcrun_error_t tmp_err = ખાલી;

  container->context = context;

  ret = validate_options (options, LIBCRUN_RUN_OPTIONS_PREFORK | LIBCRUN_RUN_OPTIONS_KEEP, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = check_config_file (def, context, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (def->process && def->process->terminal && detach && context->console_socket == ખાલી)
    પાછા crun_make_error (err, 0, "use --console-socket with --detach when a terminal is used");

  ret = libcrun_status_check_directories (context->state_મૂળ, context->id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (! detach && (options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      ret = libcrun_copy_config_file (context->id, context->state_મૂળ, container, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = libcrun_container_run_internal (container, context, ખાલી, err);
      જો (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
        force_delete_container_status (context, container, def);
      પાછા ret;
    }

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  ret = fork ();
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "fork");
  જો (ret)
    {
      પૂર્ણાંક status;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, ખાલી, 0);

      ret = TEMP_FAILURE_RETRY (પઠ (pipefd0, &status, કદ (status)));
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, errno, "read from sync pipe");

      જો (status < 0)
        {
          પૂર્ણાંક errno_;
          અક્ષર buf[512];
          ret = TEMP_FAILURE_RETRY (પઠ (pipefd0, &errno_, કદ (errno_)));
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "read from sync pipe");

          ret = TEMP_FAILURE_RETRY (પઠ (pipefd0, buf, કદ (buf) - 1));
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "read from sync pipe");
          buf[ret] = '\0';

          પાછા crun_make_error (err, errno_, "%s", buf);
        }

      પાછા status;
    }

  close_and_reset (&pipefd0);

  /* forked process.  */
  ret = detach_process ();
  જો (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_મૂળ, container, &tmp_err);
  જો (UNLIKELY (ret < 0))
    જાઓ fail;

  ret = libcrun_container_run_internal (container, context, ખાલી, &tmp_err);
  TEMP_FAILURE_RETRY (લિખ (pipefd1, &ret, કદ (ret)));
  જો (UNLIKELY (ret < 0))
    જાઓ fail;

  exit (EXIT_SUCCESS);
fail:

  જો (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
    force_delete_container_status (context, container, def);
  જો (tmp_err)
    {
      write_error_to_pipe (pipefd1, &tmp_err);
      crun_error_release (&tmp_err);
    }

  exit (EXIT_FAILURE);
}

પૂર્ણાંક
libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container, unsigned પૂર્ણાંક options,
                          libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  પૂર્ણાંક ret;
  પૂર્ણાંક container_ready_pipe[2];
  cleanup_close પૂર્ણાંક pipefd0 = -1;
  cleanup_close પૂર્ણાંક pipefd1 = -1;
  cleanup_close પૂર્ણાંક exec_fifo_fd = -1;
  context->detach = 1;

  libcrun_debug ("Creating container: `%s`", context->id);
  container->context = context;

  ret = validate_options (options, LIBCRUN_CREATE_OPTIONS_PREFORK, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = check_config_file (def, context, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (def->process && def->process->terminal && context->console_socket == ખાલી)
    પાછા crun_make_error (err, 0, "use --console-socket with create when a terminal is used");

  ret = libcrun_status_check_directories (context->state_મૂળ, context->id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  exec_fifo_fd = libcrun_status_create_exec_fifo (context->state_મૂળ, context->id, err);
  જો (UNLIKELY (exec_fifo_fd < 0))
    પાછા exec_fifo_fd;

  context->fifo_exec_wait_fd = exec_fifo_fd;
  exec_fifo_fd = -1;

  જો ((options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      libcrun_debug ("Running with prefork enabled");
      ret = libcrun_copy_config_file (context->id, context->state_મૂળ, container, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
      ret = libcrun_container_run_internal (container, context, ખાલી, err);
      જો (UNLIKELY (ret < 0))
        force_delete_container_status (context, container, def);
      પાછા ret;
    }

  ret = pipe2 (container_ready_pipe, O_CLOEXEC);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "pipe");
  pipefd0 = container_ready_pipe[0];
  pipefd1 = container_ready_pipe[1];

  ret = fork ();
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "fork");
  જો (ret)
    {
      પૂર્ણાંક exit_code;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, ખાલી, 0);

      ret = TEMP_FAILURE_RETRY (પઠ (pipefd0, &exit_code, કદ (exit_code)));
      જો (UNLIKELY (ret <= 0))
        પાછા crun_make_error (err, ret < 0 ? errno : 0, "waiting for container to be ready");

      જો (ret != કદ (exit_code) || exit_code < 0)
        પાછા crun_make_error (err, 0, "internal error: read invalid exit code");

      જો (exit_code == 0)
        પાછા 0;

      libcrun_debug ("Exit code is `%d`, deleting container", exit_code);
      libcrun_error_t tmp_err = ખાલી;
      container_delete_internal (context, ખાલી, def, context->id, true, true, &tmp_err);
      libcrun_error_report_and_release (&tmp_err);
      પાછા crun_make_error (err, 0, "error creating container");
    }

  /* forked process.  */
  ret = detach_process ();
  જો (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_મૂળ, container, err);
  જો (UNLIKELY (ret < 0))
    {
      પૂર્ણાંક errcode = crun_error_get_errno (err);
      crun_error_release (err);
      libcrun_fail_with_error (errcode, "copy config file");
    }

  ret = libcrun_container_run_internal (container, context, &pipefd1, err);
  જો (UNLIKELY (ret < 0))
    {
      force_delete_container_status (context, container, def);
      libcrun_error ((*err)->status, "%s", (*err)->સંદેશ);
      crun_error_release (err);
      crun_set_output_handler (log_write_to_stderr, ખાલી);
    }

  જો (pipefd1 >= 0)
    TEMP_FAILURE_RETRY (લિખ (pipefd1, &ret, કદ (ret)));
  exit (ret ? EXIT_FAILURE : 0);
}

પૂર્ણાંક
libcrun_container_start (libcrun_context_t *context, અચલ અક્ષર *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = ખાલી;
  અચલ અક્ષર *state_મૂળ = context->state_મૂળ;
  runtime_spec_schema_config_schema *def;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_close પૂર્ણાંક fd = -1;
  પૂર્ણાંક ret;

  ret = libcrun_read_container_status (&status, state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_is_container_running (&status, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (! ret)
    પાછા crun_make_error (err, 0, "container `%s` is not running", id);

  ret = read_container_config_from_state (&container, state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (context->notify_socket)
    {
      ret = get_notify_fd (context, container, &fd, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  ret = libcrun_status_write_exec_fifo (context->state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  def = container->container_def;

  જો (context->notify_socket)
    {
      જો (fd >= 0)
        {
          fd_set read_set;

          જ્યારે (1)
            {
              સંરચના timeval timeout = {
                .tv_sec = 0,
                .tv_usec = 10000,
              };
              FD_ZERO (&read_set);
              FD_SET (fd, &read_set);

              ret = select (fd + 1, &read_set, ખાલી, ખાલી, &timeout);
              જો (UNLIKELY (ret < 0))
                પાછા crun_make_error (err, errno, "select failed");
              જો (ret)
                {
                  ret = handle_notify_socket (fd, err);
                  જો (UNLIKELY (ret < 0))
                    પાછા ret;
                  જો (ret)
                    તોડો;
                }
              નહીંતર
                {
                  ret = libcrun_is_container_running (&status, err);
                  જો (UNLIKELY (ret < 0))
                    પાછા ret;
                  જો (! ret)
                    પાછા 0;
                }
            }
        }
    }

  /* The container is considered running only after we got the notification from the
     notify_socket, if any.  */
  જો (def->hooks && def->hooks->poststart_len)
    {
      cleanup_close પૂર્ણાંક hooks_out_fd = -1;
      cleanup_close પૂર્ણાંક hooks_err_fd = -1;

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = do_hooks (def, status.pid, context->id, false, status.bundle, status.bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      જો (UNLIKELY (ret != 0))
        {
          libcrun_error_t tmp_err = ખાલી;
          container_delete_internal (context, container, def, id, true, true, &tmp_err);
          libcrun_error_report_and_release (&tmp_err);
          પાછા ret;
        }
    }

  પાછા 0;
}

પૂર્ણાંક
libcrun_get_container_state_string (અચલ અક્ષર *id, libcrun_container_status_t *status, અચલ અક્ષર *state_મૂળ,
                                    અચલ અક્ષર **container_status, પૂર્ણાંક *running, libcrun_error_t *err)
{
  પૂર્ણાંક ret, has_fifo = 0;
  bool paused = false;

  ret = libcrun_is_container_running (status, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;
  *running = ret;

  જો (*running)
    {
      ret = libcrun_status_has_read_exec_fifo (state_મૂળ, id, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
      has_fifo = ret;
    }

  જો (*running && ! has_fifo)
    {
      cleanup_cgroup_status સંરચના libcrun_cgroup_status *cgroup_status = ખાલી;

      cgroup_status = libcrun_cgroup_make_status (status);

      ret = libcrun_cgroup_is_container_paused (cgroup_status, &paused, err);
      જો (UNLIKELY (ret < 0))
        {
          /*
            The cgroup might have been cleaned up by the time we try to read it, ignore both
            ENOENT and ENODEV:
            - ENOENT: if the open(CGROUP_PATH) fails because the cgroup was deleted.
            - ENODEV: if the cgroup is deleted between the open and reading the freeze status.
          */

          errno = crun_error_get_errno (err);
          જો (errno == ENOENT || errno == ENODEV)
            {
              crun_error_release (err);
              *container_status = "stopped";
              પાછા 0;
            }

          પાછા ret;
        }
    }

  જો (! *running)
    *container_status = "stopped";
  નહીંતર જો (has_fifo)
    *container_status = "created";
  નહીંતર જો (paused)
    *container_status = "paused";
  નહીંતર
    *container_status = "running";

  પાછા 0;
}

પૂર્ણાંક
libcrun_container_state_json (libcrun_context_t *context, અચલ અક્ષર *id, અક્ષર **out, libcrun_error_t *err)
{
  અક્ષર *buffer = ખાલી;
  size_t len = 0;
  FILE *f;
  પૂર્ણાંક ret;

  f = open_memstream (&buffer, &len);
  જો (f == ખાલી)
    પાછા crun_make_error (err, errno, "open_memstream");

  ret = libcrun_container_state (context, id, f, err);
  fclose (f);
  જો (UNLIKELY (ret < 0))
    {
      મુક્ત (buffer);
      પાછા ret;
    }

  *out = buffer;
  પાછા 0;
}

પૂર્ણાંક
libcrun_container_state (libcrun_context_t *context, અચલ અક્ષર *id, FILE *out, libcrun_error_t *err)
{
  અચલ અક્ષર *અચલ OCI_CONFIG_VERSION = "1.0.0";
  libcrun_container_status_t status = {};
  અચલ અક્ષર *state_મૂળ = context->state_મૂળ;
  અચલ અક્ષર *container_status = ખાલી;
  json_gen_ctx *gen = ખાલી;
  અચલ અક્ષર *buf;
  પૂર્ણાંક ret = 0;
  પૂર્ણાંક r = json_gen_status_ok;
  પૂર્ણાંક running;
  size_t len;

  ret = libcrun_read_container_status (&status, state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_get_container_state_string (id, &status, state_મૂળ, &container_status, &running, err);
  જો (UNLIKELY (ret < 0))
    જાઓ exit;

  ret = 0;
  જો (! json_gen_init (&gen, ખાલી))
    પાછા crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));
  GEN_ચાવી (gen, "ociVersion");
  GEN_STR (gen, OCI_CONFIG_VERSION);

  GEN_ચાવી (gen, "id");
  GEN_STR (gen, id);

  GEN_ચાવી (gen, "pid");
  GEN_OR_FAIL (map_int (gen, running ? status.pid : 0));

  GEN_ચાવી (gen, "status");
  GEN_STR (gen, container_status);

  GEN_ચાવી (gen, "bundle");
  GEN_STR (gen, status.bundle);

  GEN_ચાવી (gen, "rootfs");
  GEN_STR (gen, status.rootfs);

  GEN_ચાવી (gen, "created");
  GEN_STR (gen, status.created);

  જો (status.scope)
    {
      GEN_ચાવી (gen, "systemd-scope");
      GEN_STR (gen, status.scope);
    }

  જો (status.owner)
    {
      GEN_ચાવી (gen, "owner");
      GEN_STR (gen, status.owner);
    }

  {
    cleanup_container libcrun_container_t *container = ખાલી;

    ret = read_container_config_from_state (&container, state_મૂળ, id, err);
    જો (UNLIKELY (ret < 0))
      જાઓ exit;

    જો (container->container_def->annotations && container->container_def->annotations->len)
      GEN_OR_FAIL (gen_annotations (gen, container->container_def->annotations));
  }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  fprintf (out, "%s\n", buf);

exit:
  જો (gen)
    json_gen_free (gen);
  libcrun_free_container_status (&status);
  પાછા ret;

gen_error:
  ret = json_gen_error_to_crun_error (r, err);
  જાઓ exit;
}

પૂર્ણાંક
libcrun_container_exec (libcrun_context_t *context, અચલ અક્ષર *id, runtime_spec_schema_config_schema_process *process,
                        libcrun_error_t *err)
{
  સંરચના libcrun_container_exec_options_s opts;
  memset (&opts, 0, કદ (opts));

  opts.struct_size = કદ (opts);
  opts.process = process;

  પાછા libcrun_container_exec_with_options (context, id, &opts, err);
}

પૂર્ણાંક
libcrun_container_exec_process_file (libcrun_context_t *context, અચલ અક્ષર *id, અચલ અક્ષર *path, libcrun_error_t *err)
{
  સંરચના libcrun_container_exec_options_s opts;
  memset (&opts, 0, કદ (opts));

  opts.struct_size = કદ (opts);
  opts.path = path;

  પાછા libcrun_container_exec_with_options (context, id, &opts, err);
}

સ્થિર પૂર્ણાંક
exec_from_process_json (libcrun_context_t *context, અચલ અક્ષર *id, અચલ અક્ષર *process_json, અચલ અક્ષર *cgroup,
                        bool merge_env, libcrun_error_t *err)
{
  સંરચના parser_context parser_ctx = { .options = 0, .errfile = stderr };
  સંરચના libcrun_container_exec_options_s opts;
  runtime_spec_schema_config_schema_process *process;
  parser_error parser_err = ખાલી;
  json_object *doc = ખાલી;
  પૂર્ણાંક ret;

  ret = parse_json_file (&doc, process_json, &parser_ctx, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  process = make_runtime_spec_schema_config_schema_process (doc, &parser_ctx, &parser_err);
  json_object_put (doc);
  જો (process == ખાલી)
    {
      ret = crun_make_error (err, 0, "cannot parse process: `%s`", parser_err ? parser_err : "unknown error");
      મુક્ત (parser_err);
      પાછા ret;
    }

  memset (&opts, 0, કદ (opts));
  opts.struct_size = કદ (opts);
  opts.process = process;
  opts.cgroup = cgroup;
  opts.merge_env = merge_env;

  ret = libcrun_container_exec_with_options (context, id, &opts, err);
  free_runtime_spec_schema_config_schema_process (process);
  પાછા ret;
}

પૂર્ણાંક
libcrun_container_exec_json (libcrun_context_t *context, અચલ અક્ષર *id, અચલ અક્ષર *process_json, libcrun_error_t *err)
{
  પાછા exec_from_process_json (context, id, process_json, ખાલી, false, err);
}

પૂર્ણાંક
libcrun_container_exec_with_options_json (libcrun_context_t *context, અચલ અક્ષર *id,
                                          અચલ સંરચના libcrun_exec_options_s *opts, libcrun_error_t *err)
{
  જો (opts == ખાલી || opts->struct_size == 0)
    પાછા crun_make_error (err, EINVAL, "invalid exec options");

  પાછા exec_from_process_json (context, id, opts->process_json, opts->cgroup, opts->merge_env, err);
}

#define cleanup_process_schema __attribute__ ((cleanup (cleanup_process_schemap)))

સ્થિર અંતઃરેખ શૂન્ય
cleanup_process_schemap (runtime_spec_schema_config_schema_process **p)
{
  runtime_spec_schema_config_schema_process *process = *p;
  જો (process)
    (શૂન્ય) free_runtime_spec_schema_config_schema_process (process);
}

/* Build the seccomp receiver payload (when needed), apply the seccomp profile and
   close the seccomp file descriptors.  Used from exec_process_entrypoint both before
   and after the capabilities are set, depending on process->no_new_privileges.  */
સ્થિર પૂર્ણાંક
apply_seccomp_for_exec (libcrun_container_t *container, pid_t own_pid, પૂર્ણાંક seccomp_fd,
                        પૂર્ણાંક seccomp_receiver_fd, અક્ષર **seccomp_flags, size_t seccomp_flags_len,
                        libcrun_error_t *err)
{
  cleanup_free અક્ષર *seccomp_fd_payload = ખાલી;
  size_t seccomp_fd_payload_len = 0;
  પૂર્ણાંક ret;

  જો (seccomp_receiver_fd >= 0)
    {
      ret = get_seccomp_receiver_fd_payload (container, "running", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  ret = libcrun_apply_seccomp (seccomp_fd, seccomp_receiver_fd, seccomp_fd_payload,
                               seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  close_and_reset (&seccomp_fd);
  close_and_reset (&seccomp_receiver_fd);
  પાછા 0;
}

સ્થિર પૂર્ણાંક
exec_process_entrypoint (libcrun_context_t *context,
                         libcrun_container_t *container,
                         runtime_spec_schema_config_schema_process *process,
                         પૂર્ણાંક *pipefd1,
                         પૂર્ણાંક seccomp_fd,
                         પૂર્ણાંક seccomp_receiver_fd,
                         સંરચના custom_handler_instance_s *custom_handler,
                         bool merge_env,
                         libcrun_error_t *err)
{
  runtime_spec_schema_config_schema_process_capabilities *capabilities = ખાલી;
  cleanup_free અક્ષર *exec_path = ખાલી;
  uid_t container_uid;
  gid_t container_gid;
  અચલ અક્ષર *cwd;
  bool chdir_done = false;
  size_t seccomp_flags_len = 0;
  અક્ષર **seccomp_flags = ખાલી;
  pid_t own_pid = 0;
  size_t i;
  પૂર્ણાંક ret;

  container_uid = process->user ? process->user->uid : 0;
  container_gid = process->user ? process->user->gid : 0;

  TEMP_FAILURE_RETRY (પઠ (*pipefd1, &own_pid, કદ (own_pid)));

  cwd = process->cwd ? process->cwd : "/";
  જો (LIKELY (libcrun_safe_chdir (cwd, err) == 0))
    chdir_done = true;
  નહીંતર
    crun_error_release (err);

  ret = unblock_signals (err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = clearenv ();
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "clearenv");

  જો ((merge_env || ! process->env_len) && container->container_def->process && container->container_def->process->env_len)
    {
      અક્ષર *e;

      માટે (i = 0; i < container->container_def->process->env_len; i++)
        {
          e = container->container_def->process->env[i];
          જો (putenv (e) < 0)
            પાછા crun_make_error (err, errno, "putenv `%s`", e);
        }
    }
  જો (process->env_len)
    {
      માટે (i = 0; i < process->env_len; i++)
        જો (putenv (process->env[i]) < 0)
          પાછા crun_make_error (err, errno, "putenv `%s`", process->env[i]);
    }

  જો (is_empty_string (getenv ("HOME")))
    {
      ret = set_home_env (container_uid);
      જો (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          જો (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            પાછા crun_make_error (err, errno, "setenv HOME");
        }
    }

  ret = libcrun_set_selinux_label (container, process, false, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_set_apparmor_profile (container, process, false, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (container->container_def->linux && container->container_def->linux->seccomp)
    {
      seccomp_flags = container->container_def->linux->seccomp->flags;
      seccomp_flags_len = container->container_def->linux->seccomp->flags_len;
    }

  ret = find_executable (&exec_path, process->args[0], process->cwd, err);
  જો (UNLIKELY (ret < 0))
    {
      જો (crun_error_get_errno (err) == ENOENT)
        પાછા ret;

      /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
         once the process switched to the use that runs in the container.  This might be necessary
         when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
         is not honored.  */
      crun_error_release (err);
    }

  જો (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, false, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_reopen_dev_null (err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (! process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  ret = libcrun_container_setgroups (container, process, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = maybe_chown_std_streams (container_uid, container_gid, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (process->capabilities)
    capabilities = process->capabilities;
  નહીંતર જો (container->container_def->process)
    capabilities = container->container_def->process->capabilities;

  ret = libcrun_set_caps (capabilities, container_uid, container_gid, process->no_new_privileges,
                          custom_handler != ખાલી, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (UNLIKELY (exec_path == ખાલી))
    {
      ret = find_executable (&exec_path, process->args[0], process->cwd, err);
      જો (UNLIKELY (ret < 0))
        {
          જો (custom_handler == ખાલી || is_empty_string (process->args[0]))
            પાછા ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          exec_path = xstrdup (process->args[0]);
        }
    }

  જો (! chdir_done)
    {
      ret = libcrun_safe_chdir (cwd, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (process->user)
    umask (process->user->umask_present ? process->user->umask : 0022);

  ret = 0;
  TEMP_FAILURE_RETRY (લિખ (*pipefd1, &ret, કદ (ret)));
  TEMP_FAILURE_RETRY (close (*pipefd1));
  *pipefd1 = -1;

  જો (custom_handler)
    {
      જો (custom_handler->vtable->exec_func == ખાલી)
        પાછા crun_make_error (err, 0, "the handler does not support exec");

      જો (context->argv)
        erase_initial_environment (context->argv, context->argc);

      ret = custom_handler->vtable->exec_func (custom_handler->cookie,
                                               container,
                                               exec_path,
                                               process->args);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, -ret, "exec container process failed with handler as `%s`", custom_handler->vtable->name);

      _safe_exit (EXIT_FAILURE);
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation, because the seccomp filter is already in place and it could
     stop some syscalls used by mark_or_close_fds_ge_than.
  */
  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, true, err);
  જો (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, process->args));
  libcrun_fail_with_error (errno, "exec `%s`", exec_path);
}

/* Close the file descriptors inherited from the caller, other than those
   to be passed to the container process.  */
પૂર્ણાંક
libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err)
{
  પાછા mark_or_close_fds_ge_than (ખાલી, context->preserve_fds + 3, true, err);
}

પૂર્ણાંક
libcrun_load_process_from_file (અચલ અક્ષર *path, runtime_spec_schema_config_schema_process **process,
                                libcrun_error_t *err)
{
  સંરચના parser_context ctx = { 0, stderr };
  cleanup_free અક્ષર *content = ખાલી;
  parser_error parser_err = ખાલી;
  json_object *doc = ખાલી;
  size_t len;
  પૂર્ણાંક ret;

  ret = read_all_file (path, &content, &len, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  *process = make_runtime_spec_schema_config_schema_process (doc, &ctx, &parser_err);
  જો (UNLIKELY (*process == ખાલી))
    {
      ret = crun_make_error (err, 0, "cannot parse process file: `%s`", parser_err);
      મુક્ત (parser_err);
      જો (doc)
        json_object_put (doc);
      પાછા ret;
    }

  મુક્ત (parser_err);
  જો (doc)
    json_object_put (doc);

  પાછા 0;
}

પૂર્ણાંક
libcrun_container_exec_with_options (libcrun_context_t *context, અચલ અક્ષર *id,
                                     સંરચના libcrun_container_exec_options_s *opts,
                                     libcrun_error_t *err)
{
  cleanup_custom_handler_instance સંરચના custom_handler_instance_s *custom_handler = ખાલી;
  પૂર્ણાંક container_status, ret;
  bool container_paused = false;
  pid_t pid;
  libcrun_container_status_t status = {};
  અચલ અક્ષર *state_મૂળ = context->state_મૂળ;
  cleanup_close પૂર્ણાંક terminal_fd = -1;
  cleanup_close પૂર્ણાંક seccomp_fd = -1;
  cleanup_terminal શૂન્ય *orig_terminal = ખાલી;
  cleanup_container libcrun_container_t *container = ખાલી;
  પૂર્ણાંક container_ret_status[2];
  cleanup_close પૂર્ણાંક pipefd0 = -1;
  cleanup_close પૂર્ણાંક pipefd1 = -1;
  cleanup_close પૂર્ણાંક seccomp_receiver_fd = -1;
  cleanup_close પૂર્ણાંક own_seccomp_receiver_fd = -1;
  cleanup_close પૂર્ણાંક seccomp_notify_fd = -1;
  અચલ અક્ષર *seccomp_notify_plugins = ખાલી;
  __attribute__ ((unused)) cleanup_process_schema runtime_spec_schema_config_schema_process *process_cleanup = ખાલી;
  runtime_spec_schema_config_schema_process *process = opts->process;
  સંરચના libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  પૂર્ણાંક ret_from_child = 0;

  ret = libcrun_read_container_status (&status, state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_is_container_running (&status, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;
  container_status = ret;

  ret = read_container_config_from_state (&container, state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  container->context = context;

  જો (container_status == 0)
    પાછા crun_make_error (err, 0, "the container `%s` is not running", id);

  {
    cleanup_cgroup_status સંરચના libcrun_cgroup_status *cgroup_status = ખાલી;

    cgroup_status = libcrun_cgroup_make_status (&status);

    ret = libcrun_cgroup_is_container_paused (cgroup_status, &container_paused, err);
    જો (UNLIKELY (ret < 0))
      પાછા ret;
  }

  જો (UNLIKELY (container_paused))
    પાછા crun_make_error (err, 0, "cannot exec in a paused container `%s`", id);

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = block_signals (err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  libcrun_seccomp_gen_ctx_init (&seccomp_gen_ctx, container, false, 0);

  ret = libcrun_open_seccomp_bpf (&seccomp_gen_ctx, &seccomp_fd, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &seccomp_receiver_fd,
                                     &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  જો (કદ (*opts) != opts->struct_size)
    પાછા crun_make_error (err, EINVAL, "invalid libcrun_container_exec_options_s struct");

  જો (opts->path)
    {
      જો (process)
        પાછા crun_make_error (err, EINVAL, "cannot specify both exec file and options");

      ret = libcrun_load_process_from_file (opts->path, &process, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      process_cleanup = process;
    }

  /* This must be done before we enter a user namespace.  If the process does
     not specify any rlimits, use the ones from the container configuration.  */
  જો (process->rlimits_len == 0 && container->container_def->process)
    ret = libcrun_set_rlimits (container->container_def->process->rlimits,
                               container->container_def->process->rlimits_len, err);
  નહીંતર
    ret = libcrun_set_rlimits (process->rlimits, process->rlimits_len, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_set_mempolicy (container->container_def, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  /* If the new process block doesn't specify a SELinux label, AppArmor profile, user,
     I/O priority or scheduler, then use the configuration from the original config file.  */
  જો (container->container_def->process)
    {
      જો (process->io_priority == ખાલી && container->container_def->process->io_priority)
        {
          process->io_priority = clone_runtime_spec_schema_config_schema_process_io_priority (container->container_def->process->io_priority);
          જો (process->io_priority == ખાલી)
            OOM ();
        }

      જો (process->scheduler == ખાલી && container->container_def->process->scheduler)
        {
          process->scheduler = clone_runtime_spec_schema_config_schema_process_scheduler (container->container_def->process->scheduler);
          જો (process->scheduler == ખાલી)
            OOM ();
        }

      જો (process->selinux_label == ખાલી && container->container_def->process->selinux_label)
        process->selinux_label = xstrdup (container->container_def->process->selinux_label);

      જો (process->apparmor_profile == ખાલી && container->container_def->process->apparmor_profile)
        process->apparmor_profile = xstrdup (container->container_def->process->apparmor_profile);

      જો (process->user == ખાલી && container->container_def->process->user)
        {
          process->user = clone_runtime_spec_schema_config_schema_process_user (container->container_def->process->user);
          જો (process->user == ખાલી)
            OOM ();
        }
    }

  ret = initialize_security (container, process, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  /* Do not leak the session keyring of the caller into the container.  The
     keyring created for the container cannot be joined, so use an anonymous one.  */
  જો (! context->no_new_keyring && ! status.no_new_keyring)
    {
      libcrun_debug ("Creating new keyring for the exec process");

      ret = libcrun_create_keyring (container, ખાલી, process->selinux_label, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "prctl unset dumpable");

  pid = libcrun_join_process (context, container, status.pid, &status, opts->cgroup, context->detach,
                              process, process->terminal ? &terminal_fd : ખાલી, err);
  જો (UNLIKELY (pid < 0))
    પાછા pid;

  /* Process to exec.  */
  જો (pid == 0)
    {
      TEMP_FAILURE_RETRY (close (pipefd0));
      pipefd0 = -1;

      exec_process_entrypoint (context, container, process, &pipefd1, seccomp_fd, seccomp_receiver_fd, custom_handler, opts->merge_env, err);
      /* It gets here only on errors.  */
      જો (*err)
        {
          જો (pipefd1 < 0)
            libcrun_fail_with_error ((*err)->status, "%s", (*err)->સંદેશ);
          નહીંતર
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

  TEMP_FAILURE_RETRY (લિખ (pipefd0, &pid, કદ (pid)));

  જો (seccomp_fd >= 0)
    close_and_reset (&seccomp_fd);

  જો (terminal_fd >= 0)
    {
      unsigned short rows = 0, cols = 0;

      જો (process->console_size)
        {
          cols = process->console_size->width;
          rows = process->console_size->height;
        }

      ret = libcrun_terminal_setup_size (terminal_fd, rows, cols, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      જો (context->console_socket)
        {
          પૂર્ણાંક ret;
          cleanup_close પૂર્ણાંક console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
          જો (UNLIKELY (console_socket_fd < 0))
            પાછા console_socket_fd;
          ret = send_fd_to_socket (console_socket_fd, terminal_fd, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
          close_and_reset (&terminal_fd);
        }
      નહીંતર
        {
          ret = libcrun_clear_onlcr (terminal_fd, err);
          જો (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              પાછા ret;
            }

          ret = libcrun_set_raw (0, &orig_terminal, err);
          જો (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              પાછા ret;
            }
        }
    }

  ret = TEMP_FAILURE_RETRY (પઠ (pipefd0, &ret_from_child, કદ (ret_from_child)));
  જો (ret != કદ (ret_from_child))
    ret = crun_make_error (err, 0, "read pipe failed");
  નહીંતર જો (ret_from_child != 0)
    {
      cleanup_free અક્ષર *સંદેશ = ખાલી;
      size_t len = 0;

      ret = read_all_fd (pipefd0, "error stream", &સંદેશ, &len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
      /* the string from read_all_fd is always NUL terminated.  */
      ret = crun_make_error (err, ret_from_child, "%s", સંદેશ);
    }
  નહીંતર
    {
      /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
      જો (own_seccomp_receiver_fd >= 0)
        {
          seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
          જો (UNLIKELY (seccomp_notify_fd < 0))
            પાછા seccomp_notify_fd;

          ret = close_and_reset (&own_seccomp_receiver_fd);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "close seccomp receiver fd failed");
        }

      {
        સંરચના wait_for_process_args args = {
          .pid = pid,
          .context = context,
          .terminal_fd = terminal_fd,
          .notify_socket = -1,
          .container_ready_fd = ખાલી,
          .seccomp_notify_fd = seccomp_notify_fd,
          .seccomp_notify_plugins = seccomp_notify_plugins,
        };

        ret = wait_for_process (&args, err);
      }
    }

  flush_fd_to_err (context, terminal_fd);
  પાછા ret;
}

પૂર્ણાંક
libcrun_container_update (libcrun_context_t *context, અચલ અક્ષર *id, અચલ અક્ષર *content, size_t len arg_unused,
                          libcrun_error_t *err)
{
  cleanup_custom_handler_instance સંરચના custom_handler_instance_s *custom_handler = ખાલી;
  runtime_spec_schema_config_linux_resources *resources = ખાલી;
  cleanup_container libcrun_container_t *container = ખાલી;
  અચલ અક્ષર *state_મૂળ = context->state_મૂળ;
  સંરચના parser_context ctx = { 0, stderr };
  libcrun_container_status_t status = {};
  parser_error parser_err = ખાલી;
  json_object *doc = ખાલી;
  પૂર્ણાંક ret;

  ret = libcrun_read_container_status (&status, state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = read_container_config_from_state (&container, state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  resources = make_runtime_spec_schema_config_linux_resources (doc, &ctx, &parser_err);
  જો (UNLIKELY (resources == ખાલી))
    {
      ret = crun_make_error (err, 0, "cannot parse resources: %s", parser_err);
      જાઓ cleanup;
    }

  જો (custom_handler && custom_handler->vtable->modify_oci_configuration)
    {
      /* Adapt RESOURCES to be used from the modify_oci_configuration hook.  */
      cleanup_free runtime_spec_schema_config_linux *linux = xmalloc0 (કદ (*linux));
      cleanup_free runtime_spec_schema_config_schema *def = xmalloc0 (કદ (*def));

      def->linux = linux;
      linux->resources = resources;

      ret = custom_handler->vtable->modify_oci_configuration (custom_handler->cookie,
                                                              context,
                                                              def,
                                                              err);
      જો (UNLIKELY (ret < 0))
        જાઓ cleanup;
    }

  ret = libcrun_linux_container_update (&status, state_મૂળ, resources, err);

cleanup:
  જો (doc)
    json_object_put (doc);
  મુક્ત (parser_err);
  જો (resources)
    free_runtime_spec_schema_config_linux_resources (resources);

  પાછા ret;
}

પૂર્ણાંક
libcrun_container_update_from_file (libcrun_context_t *context, અચલ અક્ષર *id, અચલ અક્ષર *file, libcrun_error_t *err)
{
  cleanup_free અક્ષર *content = ખાલી;
  size_t len;
  પૂર્ણાંક ret;

  ret = read_all_file (file, &content, &len, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા libcrun_container_update (context, id, content, len, err);
}

સ્થિર પૂર્ણાંક
compare_update_કિંમતો (અચલ શૂન્ય *a, અચલ શૂન્ય *b)
{
  અચલ સંરચના libcrun_update_કિંમત_s *aa = a;
  અચલ સંરચના libcrun_update_કિંમત_s *bb = b;
  પૂર્ણાંક ret;

  ret = strcmp (aa->section, bb->section);
  જો (ret)
    પાછા ret;
  પાછા strcmp (aa->name, bb->name);
}

પૂર્ણાંક
libcrun_container_update_from_કિંમતો (libcrun_context_t *context, અચલ અક્ષર *id,
                                      સંરચના libcrun_update_કિંમત_s *કિંમતો, size_t len,
                                      libcrun_error_t *err)
{
  અચલ અક્ષર *current_section = ખાલી;
  અચલ અક્ષર *buf;
  json_gen_ctx *gen = ખાલી;
  size_t i, buf_len;
  પૂર્ણાંક ret;

  જો (! json_gen_init (&gen, ખાલી))
    પાછા crun_make_error (err, 0, "json_gen_init failed");
  json_gen_map_open (gen);

  qsort (કિંમતો, len, કદ (સંરચના libcrun_update_કિંમત_s), compare_update_કિંમતો);

  માટે (i = 0; i < len; i++)
    {
      જો (current_section == ખાલી || strcmp (કિંમતો[i].section, current_section))
        {
          જો (i > 0)
            json_gen_map_close (gen);

          current_section = કિંમતો[i].section;
          json_gen_string (gen, current_section, strlen (current_section));
          json_gen_map_open (gen);
        }

      json_gen_string (gen, કિંમતો[i].name, strlen (કિંમતો[i].name));

      જો (કિંમતો[i].numeric)
        json_gen_સંખ્યા (gen, કિંમતો[i].કિંમત, strlen (કિંમતો[i].કિંમત));
      નહીંતર
        json_gen_string (gen, કિંમતો[i].કિંમત, strlen (કિંમતો[i].કિંમત));
    }
  જો (len)
    json_gen_map_close (gen);

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);

  ret = libcrun_container_update (context, id, buf, buf_len, err);

  json_gen_free (gen);

  પાછા ret;
}

સ્થિર શૂન્ય
populate_array_field (અક્ષર ***field, અક્ષર *array[], size_t અંક_elements)
{
  size_t i;

  *field = xmalloc0 ((અંક_elements + 1) * કદ (અક્ષર *));
  માટે (i = 0; i < અંક_elements; i++)
    (*field)[i] = xstrdup (array[i]);

  (*field)[i] = ખાલી;
}

#ifdef HAVE_CAP
સ્થિર શૂન્ય
populate_capabilities (સંરચના features_info_s *info, અક્ષર ***capabilities, size_t *અંક_capabilities)
{
  size_t index = 0;
  cap_કિંમત_t i;
  અક્ષર *endptr;
  પૂર્ણાંક j;

  *અંક_capabilities = 0;
  માટે (i = 0;; i++)
    {
      અક્ષર *v = cap_to_name (i);
      જો (v == ખાલી)
        તોડો;
      strtol (v, &endptr, 10);
      જો (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          તોડો;
        }
      (*અંક_capabilities)++;
    }

  *capabilities = xmalloc0 ((*અંક_capabilities + 1) * કદ (અચલ અક્ષર *));
  માટે (i = 0; i < (cap_કિંમત_t) *અંક_capabilities; i++)
    {
      અક્ષર *v = cap_to_name (i);
      જો (v == ખાલી)
        તોડો;
      strtol (v, &endptr, 10);
      જો (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          તોડો;
        }

      // Convert capability name to uppercase
      માટે (j = 0; v[j] != '\0'; j++)
        v[j] = toupper (v[j]);

      (*capabilities)[index] = v;
      index++;
    }

  (*capabilities)[index] = ખાલી; // Terminate the array with NULL
  populate_array_field (&(info->linux.capabilities), *capabilities, *અંક_capabilities);
}
#endif

સ્થિર શૂન્ય
retrieve_mount_options (સંરચના features_info_s **info)
{
  cleanup_free અચલ સંરચના propagation_flags_s *mount_options_list = ખાલી;
  size_t અંક_mount_options = 0;

  // Retrieve mount options from wordlist
  mount_options_list = get_mount_flags_from_wordlist ();

  // Calculate the number of mount options
  જ્યારે (mount_options_list[અંક_mount_options].name != ખાલી)
    અંક_mount_options++;

  // Allocate memory for mount options in info struct
  (*info)->mount_options = xmalloc0 ((અંક_mount_options + 1) * કદ (અક્ષર *));

  // Copy mount options to info struct
  માટે (size_t i = 0; i < અંક_mount_options; i++)
    (*info)->mount_options[i] = xstrdup (mount_options_list[i].name);
}

પૂર્ણાંક
libcrun_container_get_features (libcrun_context_t *context, સંરચના features_info_s **info, libcrun_error_t *err arg_unused)
{
  // Allocate memory for the features_info_s structure
  size_t અંક_namspaces = કદ (namespaces) / કદ (namespaces[0]);
  size_t અંક_operators = કદ (operators) / કદ (operators[0]);
  size_t અંક_actions = કદ (actions) / કદ (actions[0]);
  size_t અંક_hooks = કદ (hooks) / કદ (hooks[0]);
  size_t અંક_archs = કદ (archs) / કદ (archs[0]);
  size_t અંક_unsafe_annotations = કદ (potentially_unsafe_annotations) / કદ (potentially_unsafe_annotations[0]);
  cleanup_free અક્ષર **capabilities = ખાલી;
  size_t અંક_capabilities = 0;
  size_t અંક_mempolicy_modes = કદ (mempolicy_modes) / કદ (mempolicy_modes[0]);
  size_t અંક_mempolicy_flags = કદ (mempolicy_flags) / કદ (mempolicy_flags[0]);

  *info = xmalloc0 (કદ (સંરચના features_info_s));

  // Hardcoded feature information
  (*info)->oci_version_min = xstrdup ("1.0.0");
  (*info)->oci_version_max = xstrdup ("1.1.0+dev");

  // Populate hooks
  populate_array_field (&((*info)->hooks), hooks, અંક_hooks);

  // Populate mount_options
  retrieve_mount_options (info);

  // Populate namespaces
  populate_array_field (&((*info)->linux.namespaces), namespaces, અંક_namspaces);

#ifdef HAVE_CAP
  // Populate capabilities
  populate_capabilities (*info, &capabilities, &અંક_capabilities);
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
  populate_array_field (&((*info)->linux.seccomp.actions), actions, અંક_actions);
  // Populate operators
  populate_array_field (&((*info)->linux.seccomp.operators), operators, અંક_operators);
  // Populate archs
  populate_array_field (&((*info)->linux.seccomp.archs), archs, અંક_archs);
#નહીંતર
  (*info)->linux.seccomp.enabled = false;
#endif

  // Put values for apparmor and selinux
  (*info)->linux.apparmor.enabled = true;
  (*info)->linux.selinux.enabled = true;

  (*info)->linux.intel_rdt.enabled = true;

  (*info)->linux.net_devices.enabled = true;

  populate_array_field (&((*info)->linux.memory_policy.mode), mempolicy_modes, અંક_mempolicy_modes);
  populate_array_field (&((*info)->linux.memory_policy.flags), mempolicy_flags, અંક_mempolicy_flags);

  // Put the values for mount extensions
  (*info)->linux.mount_ext.idmap.enabled = true;

  // Populate the values for annotations
#ifdef HAVE_SECCOMP
  {
    અચલ સંરચના scmp_version *version = seccomp_version ();
    અક્ષર *version_string = ખાલી;

    xasprintf (&version_string, "%u.%u.%u", version->major, version->minor, version->micro);
    (*info)->annotations.io_github_seccomp_libseccomp_version = version_string;
  }
#endif

  જો (context->handler_manager && handler_by_name (context->handler_manager, "wasm"))
    (*info)->annotations.run_oci_crun_wasm = true;

#જો HAVE_CRIU
  (*info)->annotations.run_oci_crun_checkpoint_enabled = true;
#endif
  (*info)->annotations.run_oci_crun_commit = GIT_VERSION;
  (*info)->annotations.run_oci_crun_version = PACKAGE_VERSION;

  populate_array_field (&((*info)->potentially_unsafe_annotations), potentially_unsafe_annotations, અંક_unsafe_annotations);

  પાછા 0;
}

સ્થિર શૂન્ય
features_add_string (json_gen_ctx *json_gen, અચલ અક્ષર *ચાવી, અચલ અક્ષર *કિંમત)
{
  json_gen_string (json_gen, ચાવી, strlen (ચાવી));
  json_gen_string (json_gen, કિંમત, strlen (કિંમત));
}

સ્થિર શૂન્ય
features_add_bool (json_gen_ctx *json_gen, અચલ અક્ષર *ચાવી, પૂર્ણાંક કિંમત)
{
  json_gen_string (json_gen, ચાવી, strlen (ચાવી));
  json_gen_bool (json_gen, કિંમત);
}

સ્થિર શૂન્ય
features_add_bool_str (json_gen_ctx *json_gen, અચલ અક્ષર *ચાવી, પૂર્ણાંક કિંમત)
{
  features_add_string (json_gen, ચાવી, કિંમત ? "true" : "false");
}

સ્થિર શૂન્ય
features_add_array (json_gen_ctx *json_gen, અચલ અક્ષર *ચાવી, અક્ષર **array)
{
  size_t i;

  json_gen_string (json_gen, ચાવી, strlen (ચાવી));
  json_gen_array_open (json_gen);
  જો (array)
    માટે (i = 0; array[i] != ખાલી; i++)
      json_gen_string (json_gen, array[i], strlen (array[i]));
  json_gen_array_close (json_gen);
}

સ્થિર શૂન્ય
features_add_linux_info (json_gen_ctx *json_gen, અચલ સંરચના linux_info_s *linux)
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
  જો (linux->seccomp.actions)
    features_add_array (json_gen, "actions", linux->seccomp.actions);
  જો (linux->seccomp.operators)
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
  જો (linux->memory_policy.mode)
    features_add_array (json_gen, "modes", linux->memory_policy.mode);
  જો (linux->memory_policy.flags)
    features_add_array (json_gen, "flags", linux->memory_policy.flags);
  json_gen_map_close (json_gen);

  json_gen_map_close (json_gen);
}

સ્થિર શૂન્ય
features_add_annotations_info (json_gen_ctx *json_gen, અચલ સંરચના annotations_info_s *annotation)
{
  json_gen_string (json_gen, "annotations", strlen ("annotations"));
  json_gen_map_open (json_gen);

  જો (! is_empty_string (annotation->io_github_seccomp_libseccomp_version))
    features_add_string (json_gen, "io.github.seccomp.libseccomp.version", annotation->io_github_seccomp_libseccomp_version);

  features_add_bool_str (json_gen, "org.opencontainers.runc.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);
  features_add_bool_str (json_gen, "run.oci.crun.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);

  features_add_string (json_gen, "run.oci.crun.commit", annotation->run_oci_crun_commit);
  features_add_string (json_gen, "run.oci.crun.version", annotation->run_oci_crun_version);

  features_add_bool_str (json_gen, "run.oci.crun.wasm", annotation->run_oci_crun_wasm);

  json_gen_map_close (json_gen);
}

પૂર્ણાંક
libcrun_container_get_features_json (libcrun_context_t *context, અક્ષર **out, libcrun_error_t *err)
{
  cleanup_struct_features સંરચના features_info_s *info = ખાલી;
  અચલ અક્ષર *buffer = ખાલી;
  json_gen_ctx *json_gen;
  size_t buffer_len;
  પૂર્ણાંક ret;

  *out = ખાલી;

  ret = libcrun_container_get_features (context, &info, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (! json_gen_init (&json_gen, ખાલી))
    પાછા crun_make_error (err, 0, "cannot initialize JSON generator");

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

  પાછા 0;
}

/* Read the container status and fail if the container is not running.  */
સ્થિર પૂર્ણાંક
read_status_require_running (libcrun_context_t *context, અચલ અક્ષર *id,
                             libcrun_container_status_t *status, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  ret = libcrun_read_container_status (status, context->state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_is_container_running (status, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;
  જો (ret == 0)
    પાછા crun_make_error (err, 0, "the container `%s` is not running", id);

  પાછા 0;
}

પૂર્ણાંક
libcrun_container_pause (libcrun_context_t *context, અચલ અક્ષર *id, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા libcrun_container_pause_linux (&status, err);
}

પૂર્ણાંક
libcrun_container_unpause (libcrun_context_t *context, અચલ અક્ષર *id, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા libcrun_container_unpause_linux (&status, err);
}

/* Translate the public options struct into the internal representation used by
   the linux/CRIU layer.  Strings are borrowed (not copied).  */
સ્થિર શૂન્ય
cr_options_from_public (libcrun_checkpoint_restore_t *dst, સંરચના libcrun_checkpoint_restore_options_s *src)
{
  memset (dst, 0, કદ (*dst));
  dst->image_path = (અક્ષર *) src->image_path;
  dst->work_path = (અક્ષર *) src->work_path;
  dst->parent_path = (અક્ષર *) src->parent_path;
  dst->console_socket = src->console_socket;
  dst->lsm_profile = (અક્ષર *) src->lsm_profile;
  dst->lsm_mount_context = (અક્ષર *) src->lsm_mount_context;
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

પૂર્ણાંક
libcrun_container_checkpoint (libcrun_context_t *context, અચલ અક્ષર *id,
                              સંરચના libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  અચલ અક્ષર *state_મૂળ = context->state_મૂળ;
  libcrun_container_status_t status = {};
  libcrun_checkpoint_restore_t cr;
  cleanup_container libcrun_container_t *container = ખાલી;

  cr_options_from_public (&cr, cr_options);

  ret = read_status_require_running (context, id, &status, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = read_container_config_from_state (&container, state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  /* Needed to use the same paths (e.g. the shared empty directory used for
     masked paths) as the container runtime and restore do.  */
  container->context = context;

  ret = libcrun_container_checkpoint_linux (&status, container, &cr, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (! (cr_options->leave_running || cr_options->pre_dump))
    પાછા container_delete_internal (context, ખાલી, ખાલી, id, true, true, err);

  પાછા 0;
}

/* The dummy process only keeps the cgroup populated until the container is restored there.
   Its cgroup is below the container cgroup, so once the container runs it can freeze or kill
   the dummy process: never depend on it after that point.  */
સ્થિર શૂન્ય
restore_proxy_process (પૂર્ણાંક proxy_pid_pipe)
{
  અક્ષર c;

  /* Wait to be killed by the parent, or for the parent to go away.  */
  TEMP_FAILURE_RETRY (પઠ (proxy_pid_pipe, &c, કદ (c)));
}

સ્થિર પૂર્ણાંક
restore_terminate_proxy_process (pid_t proxy_pid, અચલ અક્ષર *proxy_cgroup, pid_t container_pid, પૂર્ણાંક cgroup_manager, libcrun_error_t *err)
{
  પૂર્ણાંક mode;
  પૂર્ણાંક ret;

  જો (cgroup_manager == CGROUP_MANAGER_SYSTEMD)
    {
      અક્ષર ready_str[64];

      ret = snprintf (ready_str, કદ (ready_str), "MAINPID=%d", container_pid);
      જો (UNLIKELY (ret >= (પૂર્ણાંક) કદ (ready_str)))
        પાછા crun_make_error (err, 0, "internal error: static buffer too small");

      ret = send_sd_notify (ready_str, err);
      /* Do not fail on errors.  */
      જો (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  mode = libcrun_get_cgroup_mode (err);
  જો (UNLIKELY (mode < 0))
    પાછા mode;

  /* SIGKILL terminates a process frozen with cgroup v2, while the cgroup v1 freezer does not
     deliver it until the cgroup is thawed, so do not wait for it there.  */
  kill (proxy_pid, SIGKILL);
  TEMP_FAILURE_RETRY (waitpid (proxy_pid, ખાલી, mode == CGROUP_MODE_UNIFIED ? 0 : WNOHANG));

  પાછા destroy_cgroup_path (proxy_cgroup, mode, err);
}

પૂર્ણાંક
libcrun_container_restore (libcrun_context_t *context, અચલ અક્ષર *id arg_unused,
                           સંરચના libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  cleanup_cgroup_status સંરચના libcrun_cgroup_status *cgroup_status = ખાલી;
  cleanup_container libcrun_container_t *container = ખાલી;
  cleanup_close પૂર્ણાંક proxy_pid_pipe0 = -1;
  cleanup_close પૂર્ણાંક proxy_pid_pipe1 = -1;
  runtime_spec_schema_config_schema *def;
  libcrun_container_status_t status = {};
  સંરચના libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  libcrun_checkpoint_restore_t cr;
  cleanup_pid pid_t proxy_pid = -1;
  cleanup_close પૂર્ણાંક seccomp_fd = -1;
  અચલ અક્ષર *seccomp_bpf_data;
  પૂર્ણાંક proxy_pid_pipe[2];
  પૂર્ણાંક cgroup_manager;
  uid_t મૂળ_uid = -1;
  gid_t મૂળ_gid = -1;
  પૂર્ણાંક ret;

  cr_options_from_public (&cr, cr_options);

  container = libcrun_container_load_from_file ("config.json", err);
  જો (container == ખાલી)
    પાછા -1;

  container->context = context;
  def = container->container_def;

  ret = check_config_file (def, context, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_status_check_directories (context->state_મૂળ, context->id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_copy_config_file (context->id, context->state_મૂળ, container, err);
  જો (UNLIKELY (ret < 0))
    જાઓ fail;

  /* CRIU restores the seccomp filters of the checkpointed processes, but
     exec loads the filter from the seccomp.bpf file in the state directory,
     so it must be generated again for the restored container.  */
  seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  જો (UNLIKELY (ret < 0))
    જાઓ fail;

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  જો (UNLIKELY (ret < 0))
    જાઓ fail;
  close_and_reset (&seccomp_fd);

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  જો (context->systemd_cgroup)
    cgroup_manager = CGROUP_MANAGER_SYSTEMD;
  નહીંતર જો (context->force_no_cgroup)
    cgroup_manager = CGROUP_MANAGER_DISABLED;

  def = container->container_def;

  /* If we are root (either on the host or in a namespace),
   * then chown the cgroup to root in the container user namespace. */
  get_મૂળ_in_the_userns (def, container->host_uid, container->host_gid, &મૂળ_uid, &મૂળ_gid);

  /* If the root in the container is different than the current root user, attempt to chown
     the std streams before entering the user namespace.  Otherwise we might lose access
     to the user (as it is not mapped in the user namespace) and cannot chown them.  */
  જો (મૂળ_uid > 0 || મૂળ_gid > 0)
    {
      ret = maybe_chown_std_streams (મૂળ_uid, મૂળ_gid, err);
      જો (UNLIKELY (ret < 0))
        જાઓ fail;
    }

  /* CRIU runs as a child process, so the restored processes would inherit the session keyring.  */
  ret = setup_container_keyring (container, context, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  {
    cleanup_close પૂર્ણાંક cgroup_dirfd = -1;
    bool needs_proxy_process;
    cleanup_cgroup_preenter સંરચના libcrun_cgroup_args cg = {
      .resources = def->linux ? def->linux->resources : ખાલી,
      .annotations = container->annotations,
      .cgroup_path = def->linux ? def->linux->cgroups_path : "",
      .manager = cgroup_manager,
      .મૂળ_uid = મૂળ_uid,
      .મૂળ_gid = મૂળ_gid,
      .id = context->id,
      .state_મૂળ = context->state_મૂળ,
      .container = container,
    };

    /* The CRIU restore code uses bundle, rootfs and cgroup_path of status.   The cgroup_path is set later.  */
    status.bundle = (અક્ષર *) context->bundle;
    status.rootfs = def->મૂળ->path;

    ret = libcrun_cgroup_preenter (&cg, &cgroup_dirfd, err);
    જો (UNLIKELY (ret < 0))
      જાઓ fail;

    needs_proxy_process = cgroup_dirfd < 0;

    /* If the target cgroup was already created (with cgroupfs), we can restore the container directly there.  */
    જો (! needs_proxy_process)
      {
        cleanup_free અક્ષર *cgroup_path = ખાલી;

        ret = get_cgroup_dirfd_path (cgroup_dirfd, &cgroup_path, err);
        જો (UNLIKELY (ret < 0))
          જાઓ fail;

        /* Complete the configuration for the cgroup before the container is
           restored there: CRIU resumes the restored processes before it
           returns, so the device filter and the limits must already be in
           place.  There is no process to move, as CRIU is run in the cgroup,
           and the controllers were enabled by precreate_cgroup.  */
        cg.joined = true;

        ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
        જો (UNLIKELY (ret < 0))
          જાઓ fail;

        /* Restore the container directly in the desired cgroup.  */
        status.cgroup_path = cgroup_path;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        જો (UNLIKELY (ret < 0))
          જાઓ fail;

        /* Now that the container is restored, keep any signal for it, so that
           it is forwarded to the init by wait_for_process, rather than acted
           upon by crun.  This cannot be done earlier, as CRIU, which is run as
           a child process, would inherit the blocked signals.  */
        ret = block_signals (err);
        જો (UNLIKELY (ret < 0))
          જાઓ fail;

        /* Use the container first process PID to finalize the cgroup.  */
        cg.pid = status.pid;
      }
    નહીંતર
      {
        /* When using systemd, we need a process first that must exist before a cgroup is created.
           Create a dummy process that sits and waits until the container is restored.  */
        ret = pipe2 (proxy_pid_pipe, O_CLOEXEC);
        જો (UNLIKELY (ret < 0))
          {
            ret = crun_make_error (err, errno, "pipe");
            જાઓ fail;
          }

        /* Only used for auto cleanup.  */
        proxy_pid_pipe0 = proxy_pid_pipe[0];
        proxy_pid_pipe1 = proxy_pid_pipe[1];

        proxy_pid = fork ();
        જો (UNLIKELY (proxy_pid < 0))
          {
            ret = crun_make_error (err, errno, "fork");
            જાઓ fail;
          }

        જો (proxy_pid == 0)
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
        જો (UNLIKELY (ret < 0))
          જાઓ fail;
      }

    ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
    જો (UNLIKELY (ret < 0))
      જાઓ fail;

    /* When using a dummy process, the container is restored here once the cgroup is configured.  */
    જો (needs_proxy_process)
      {
        cleanup_free અક્ષર *target_cgroup = ખાલી;
        cleanup_free અક્ષર *proxy_cgroup = ખાલી;

        ret = libcrun_get_cgroup_process (proxy_pid, &target_cgroup, false, err);
        જો (UNLIKELY (ret < 0))
          જાઓ fail;

        /* Move the dummy process to a sub-cgroup.  */
        ret = append_paths (&proxy_cgroup, err, target_cgroup, ".proxy-crun", ખાલી);
        જો (UNLIKELY (ret < 0))
          જાઓ fail;

        ret = libcrun_move_process_to_cgroup (proxy_pid, 0, proxy_cgroup, true, err);
        જો (UNLIKELY (ret < 0))
          જાઓ fail;

        /* Restore the container in the same cgroup where the dummy process was.  */
        status.cgroup_path = target_cgroup;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        જો (UNLIKELY (ret < 0))
          જાઓ fail;

        /* See the comment for the other libcrun_container_restore_linux call.  */
        ret = block_signals (err);
        જો (UNLIKELY (ret < 0))
          જાઓ fail;

        /* The container is running, so do not fail because of the dummy process.  */
        ret = restore_terminate_proxy_process (proxy_pid, proxy_cgroup, status.pid, cgroup_manager, err);
        જો (UNLIKELY (ret < 0))
          crun_error_release (err);
        proxy_pid = -1;

        close_and_reset (&proxy_pid_pipe1);
      }
  }

  context->detach = cr_options->detach;
  ret = write_container_status (container, context, status.pid, cgroup_status, err);
  જો (UNLIKELY (ret < 0))
    જાઓ fail;

  {
    /* Wait the same way run and exec do: apart from waiting for the container
       init, this writes the pid file, forwards any signal to the init, and
       reaps every child, not only the init.  */
    સંરચના wait_for_process_args args = {
      .pid = status.pid,
      .context = context,
      .terminal_fd = -1,
      .notify_socket = -1,
      .container_ready_fd = ખાલી,
      .seccomp_notify_fd = -1,
      .seccomp_notify_plugins = ખાલી,
    };

    પાછા wait_for_process (&args, err);
  }

fail:
  /* Once restored, the container is running.  Without a status file nothing
     can reach it anymore, so do not leave it behind.  */
  જો (status.pid > 0)
    {
      kill (status.pid, SIGKILL);
      waitpid_ignore_stopped (status.pid, ખાલી, 0);
    }
  જો (cgroup_status)
    {
      libcrun_error_t tmp_err = ખાલી;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  {
    libcrun_error_t tmp_err = ખાલી;

    જો (libcrun_container_delete_status (context->state_મૂળ, context->id, &tmp_err) < 0)
      crun_error_release (&tmp_err);
  }
  પાછા ret;
}

પૂર્ણાંક
libcrun_container_read_pids (libcrun_context_t *context, અચલ અક્ષર *id, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  cleanup_cgroup_status સંરચના libcrun_cgroup_status *cgroup_status = ખાલી;
  cleanup_container_status libcrun_container_status_t status = {};
  પૂર્ણાંક ret;

  ret = libcrun_read_container_status (&status, context->state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (status.cgroup_path == ખાલી || status.cgroup_path[0] == '\0')
    પાછા crun_make_error (err, 0, "the container is not using cgroups");

  cgroup_status = libcrun_cgroup_make_status (&status);

  પાછા libcrun_cgroup_read_pids (cgroup_status, recurse, pids, err);
}

પૂર્ણાંક
libcrun_container_list_json (libcrun_context_t *context, અક્ષર **out, libcrun_error_t *err)
{
  libcrun_container_list_t *list = ખાલી, *it;
  અચલ અક્ષર *content = ખાલી;
  json_gen_ctx *gen = ખાલી;
  size_t len;
  પૂર્ણાંક ret;

  *out = ખાલી;

  ret = libcrun_get_containers_list (&list, context->state_મૂળ, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (! json_gen_init (&gen, ખાલી))
    {
      ret = crun_make_error (err, 0, "cannot allocate json generator");
      જાઓ exit;
    }

  json_gen_config (gen, json_gen_beautify, 1);
  json_gen_array_open (gen);

  માટે (it = list; it; it = it->next)
    {
      libcrun_container_status_t status;
      પૂર્ણાંક running = 0;
      પૂર્ણાંક pid;
      અચલ અક્ષર *container_status = ખાલી;

      ret = libcrun_read_container_status (&status, context->state_મૂળ, it->name, err);
      જો (UNLIKELY (ret < 0))
        જાઓ exit;

      pid = status.pid;

      ret = libcrun_get_container_state_string (it->name, &status, context->state_મૂળ, &container_status,
                                                &running, err);
      જો (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          ચાલુ;
        }

      જો (! running)
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
  જો (json_gen_get_buf (gen, &content, &len) != json_gen_status_ok)
    {
      ret = libcrun_make_error (err, 0, "cannot generate json list");
      જાઓ exit;
    }

  *out = xstrdup (content);
  ret = 0;

exit:
  જો (list)
    libcrun_free_containers_list (list);
  જો (gen)
    json_gen_free (gen);

  પાછા ret;
}

પૂર્ણાંક
libcrun_write_json_containers_list (libcrun_context_t *context, FILE *out, libcrun_error_t *err)
{
  cleanup_free અક્ષર *content = ખાલી;
  size_t len;
  પૂર્ણાંક ret;

  ret = libcrun_container_list_json (context, &content, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  len = strlen (content);
  જો (len && fwrite (content, 1, len, out) != len)
    પાછા crun_make_error (err, errno, "error writing to file");

  પાછા 0;
}

પૂર્ણાંક
libcrun_container_update_intel_rdt (libcrun_context_t *context, અચલ અક્ષર *id, સંરચના libcrun_intel_rdt_update *update, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = ખાલી;
  પૂર્ણાંક ret;

  ret = read_container_config_from_state (&container, context->state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા libcrun_update_intel_rdt (id, container, update->l3_cache_schema, update->mem_bw_schema, update->schemata, err);
}

સ્થિર પૂર્ણાંક
libcrun_container_add_or_remove_mounts_from_file (libcrun_context_t *context, અચલ અક્ષર *id, અચલ અક્ષર *file, bool add, libcrun_error_t *err)
{
  cleanup_custom_handler_instance સંરચના custom_handler_instance_s *custom_handler = ખાલી;
  cleanup_container libcrun_container_t *container = ખાલી;
  cleanup_free runtime_spec_schema_defs_mount **mounts = ખાલી;
  અચલ અક્ષર *state_મૂળ = context->state_મૂળ;
  cleanup_free parser_error parser_err = ખાલી;
  સંરચના parser_context pctx = { 0, stderr };
  libcrun_container_status_t status = {};
  cleanup_free અક્ષર *content = ખાલી;
  json_object *doc = ખાલી;
  size_t n_mounts = 0, len, i;
  પૂર્ણાંક ret = 1;

  ret = read_all_file (file, &content, &len, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_read_container_status (&status, state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = read_container_config_from_state (&container, state_મૂળ, id, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = parse_json_file (&doc, content, &pctx, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (! json_object_is_type (doc, json_type_array))
    {
      ret = crun_make_error (err, 0, "mounts must be an array");
      જાઓ cleanup;
    }
  નહીંતર
    {
      n_mounts = json_object_array_length (doc);

      mounts = xmalloc0 ((n_mounts + 1) * કદ (*mounts));
      માટે (i = 0; i < n_mounts; i++)
        {
          mounts[i] = make_runtime_spec_schema_defs_mount (json_object_array_get_idx (doc, i), &pctx, &parser_err);
          જો (mounts[i] == ખાલી)
            {
              ret = crun_make_error (err, 0, "cannot parse mount: %s", parser_err);
              જાઓ cleanup;
            }
        }
    }

  જો (add)
    ret = libcrun_make_runtime_mounts (container, &status, mounts, n_mounts, err);
  નહીંતર
    ret = libcrun_destroy_runtime_mounts (container, &status, mounts, n_mounts, err);

cleanup:
  જો (doc)
    json_object_put (doc);
  જો (mounts)
    માટે (i = 0; i < n_mounts; i++)
      free_runtime_spec_schema_defs_mount (mounts[i]);
  પાછા ret;
}

પૂર્ણાંક
libcrun_container_add_mounts_from_file (libcrun_context_t *context, અચલ અક્ષર *id, અચલ અક્ષર *file, libcrun_error_t *err)
{
  પાછા libcrun_container_add_or_remove_mounts_from_file (context, id, file, true, err);
}

પૂર્ણાંક
libcrun_container_remove_mounts_from_file (libcrun_context_t *context, અચલ અક્ષર *id, અચલ અક્ષર *file, libcrun_error_t *err)
{
  પાછા libcrun_container_add_or_remove_mounts_from_file (context, id, file, false, err);
}
