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

#شامل <config.h>
#شامل <ocispec/runtime_spec_schema_config_schema.h>
#شامل <stdbool.h>
#شامل "container.h"
#شامل "utils.h"
#شامل "json_gen_utils.h"
#شامل "seccomp.h"
#شامل "mempolicy.h"
#ifdef HAVE_SECCOMP
#  include <seccomp.h>
#endif
#شامل "scheduler.h"
#شامل "seccomp_notify.h"
#شامل "custom-handler.h"
#شامل <stdbool.h>
#شامل <argp.h>
#شامل <unistd.h>
#شامل <sys/stat.h>
#شامل <sys/types.h>
#شامل <sched.h>
#شامل <sys/wait.h>
#شامل <string.h>
#شامل <fcntl.h>
#شامل "status.h"
#شامل "mount_flags.h"
#شامل "linux.h"
#شامل "terminal.h"
#شامل "io_priority.h"
#شامل "cgroup.h"
#شامل "cgroup-utils.h"
#شامل <sys/prctl.h>
#شامل <sys/auxv.h>
#شامل <sys/signalfd.h>
#شامل <sys/epoll.h>
#شامل <sys/socket.h>
#ifdef HAVE_CAP
#  include <sys/capability.h>
#endif
#شامل <sys/ioctl.h>
#شامل <termios.h>
#شامل <grp.h>
#شامل <git-version.h>

#ifdef HAVE_SYSTEMD
#  include <systemd/sd-daemon.h>
#endif

#شامل <json-c/json.h>
#شامل <ocispec/json_common.h>

شمارش
{
  SYNC_SOCKET_SYNC_MESSAGE,
  SYNC_SOCKET_ERROR_MESSAGE,
  SYNC_SOCKET_WARNING_MESSAGE,
  SYNC_SOCKET_DEBUG_MESSAGE,
};

ساختار container_entrypoint_s
{
  libcrun_container_t *container;
  libcrun_context_t *context;
  صحیح has_terminal_socket_pair;
  صحیح terminal_socketpair[2];

  /* Used by log_write_to_sync_socket.  */
  صحیح sync_socket;

  صحیح seccomp_fd;
  صحیح seccomp_receiver_fd;
  صحیح console_socket_fd;

  صحیح hooks_out_fd;
  صحیح hooks_err_fd;

  /* Absolute bundle path, to be reported to hooks run in the container.  */
  ثابت نویسه *bundle;

  ساختار custom_handler_instance_s *custom_handler;
};

ساختار sync_socket_message_s
{
  صحیح type;
  صحیح error_مقدار;
  نویسه message[512];
};

تعریفنوع runtime_spec_schema_defs_hook hook;

// linux hooks
نویسه *hooks[] = {
  "prestart",
  "createRuntime",
  "createContainer",
  "startContainer",
  "poststart",
  "poststop"
};

// linux namespaces
ایستا نویسه *namespaces[] = {
  "cgroup",
  "ipc",
  "mount",
  "network",
  "pid",
  "user",
  "uts"
};

ایستا نویسه *actions[] = {
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

ایستا نویسه *operators[] = {
  "SCMP_CMP_NE",
  "SCMP_CMP_LT",
  "SCMP_CMP_LE",
  "SCMP_CMP_EQ",
  "SCMP_CMP_GE",
  "SCMP_CMP_GT",
  "SCMP_CMP_MASKED_EQ",
};

ایستا نویسه *archs[] = {
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

ایستا نویسه *mempolicy_modes[] = {
  "MPOL_DEFAULT",
  "MPOL_PREFERRED",
  "MPOL_BIND",
  "MPOL_INTERLEAVE",
  "MPOL_LOCAL",
  "MPOL_PREFERRED_MANY",
  "MPOL_WEIGHTED_INTERLEAVE"
};

ایستا نویسه *mempolicy_flags[] = {
  "MPOL_F_NUMA_BALANCING",
  "MPOL_F_RELATIVE_NODES",
  "MPOL_F_STATIC_NODES"
};

ایستا نویسه *potentially_unsafe_annotations[] = {
  "module.wasm.image/variant",
  "io.kubernetes.cri.container-type",
  "run.oci.",
  "org.criu.",
  "krun.",
};

#define SYNC_SOCKET_MESSAGE_LEN(x, l) (offsetof (ساختار sync_socket_message_s, message) + l)

ایستا صحیح
sync_socket_write_پیام (صحیح fd, صحیح verbosity, صحیح err_مقدار, ثابت نویسه *log_پیام)
{
  صحیح ret;
  size_t err_len;
  ساختار sync_socket_message_s پیام;
  انتخاب (verbosity)
    {
    حالت LIBCRUN_VERBOSITY_DEBUG:
      پیام.type = SYNC_SOCKET_DEBUG_MESSAGE;
      بشکن;
    حالت LIBCRUN_VERBOSITY_WARNING:
      پیام.type = SYNC_SOCKET_WARNING_MESSAGE;
      بشکن;
    حالت LIBCRUN_VERBOSITY_ERROR:
      پیام.type = SYNC_SOCKET_ERROR_MESSAGE;
      بشکن;
    }
  پیام.error_مقدار = err_مقدار;

  اگر (fd < 0)
    بازگشت 0;

  err_len = strlen (log_پیام);
  اگر (err_len >= اندازه (پیام.message))
    err_len = اندازه (پیام.message) - 1;

  memcpy (پیام.message, log_پیام, err_len);
  پیام.message[err_len] = '\0';

  ret = TEMP_FAILURE_RETRY (اكتب (fd, &پیام, SYNC_SOCKET_MESSAGE_LEN (پیام, err_len + 1)));
  اگر (UNLIKELY (ret < 0))
    بازگشت -1;

  بازگشت 0;
}

ایستا صحیح
sync_socket_write_error (صحیح fd, libcrun_error_t *out_err)
{
  اگر (fd < 0)
    بازگشت 0;
  بازگشت sync_socket_write_پیام (fd, false, (*out_err)->status, (*out_err)->پیام);
}

ایستا تهی
log_write_to_sync_socket (صحیح errno_, ثابت نویسه *پیام, صحیح verbosity, تهی *arg)
{
  ساختار container_entrypoint_s *entrypoint_args = arg;
  صحیح fd = entrypoint_args->sync_socket;

  اگر (fd < 0)
    بازگشت;

  اگر (sync_socket_write_پیام (fd, verbosity, errno_, پیام) < 0)
    log_write_to_stderr (errno_, پیام, verbosity, arg);
}

ایستا bool
is_memory_limit_too_کم (runtime_spec_schema_config_schema *def)
{
  ثابت long memory_limit_too_کم = 1024 * 1024;

  اگر (def->linux == پوچ || def->linux->resources == پوچ)
    بازگشت false;

  اگر (def->linux->resources->memory
      && def->linux->resources->memory->limit_present
      && def->linux->resources->memory->limit < memory_limit_too_کم)
    بازگشت true;

  اگر (def->linux->resources->unified)
    {
      size_t i;

      برای (i = 0; i < def->linux->resources->unified->len; i++)
        اگر (strcmp (def->linux->resources->unified->کلیدها[i], "memory.max") == 0)
          {
            long limit;

            errno = 0;
            limit = strtol (def->linux->resources->unified->مقادیر[i], پوچ, 10);
            اگر (errno != 0)
              بازگشت false;
            اگر (limit < memory_limit_too_کم)
              بازگشت true;
          }
    }

  بازگشت false;
}

ایستا صحیح
sync_socket_wait_sync (libcrun_context_t *context, صحیح fd, bool flush, libcrun_error_t *err)
{
  ساختار sync_socket_message_s پیام;

  اگر (fd < 0)
    بازگشت 0;

  وقتی (true)
    {
      صحیح ret;

      ret = TEMP_FAILURE_RETRY (اقرأ (fd, &پیام, اندازه (پیام)));
      اگر (UNLIKELY (ret < 0))
        {
          اگر (flush)
            بازگشت 0;
          بازگشت crun_make_error (err, errno, "read from sync socket");
        }

      اگر (ret == 0)
        {
          اگر (flush)
            بازگشت 0;

          بازگشت crun_make_error (err, 0, "read from the init process");
        }

      اگر (! flush && پیام.type == SYNC_SOCKET_SYNC_MESSAGE)
        بازگشت 0;
      وگرنه اگر (پیام.type == SYNC_SOCKET_DEBUG_MESSAGE)
        {
          اگر (context)
            context->output_handler (پیام.error_مقدار, پیام.message, LIBCRUN_VERBOSITY_DEBUG, context->output_handler_arg);
          ادامه;
        }
      وگرنه اگر (پیام.type == SYNC_SOCKET_WARNING_MESSAGE)
        {
          اگر (context)
            context->output_handler (پیام.error_مقدار, پیام.message, LIBCRUN_VERBOSITY_WARNING, context->output_handler_arg);
          ادامه;
        }
      وگرنه اگر (پیام.type == SYNC_SOCKET_ERROR_MESSAGE)
        بازگشت crun_make_error (err, پیام.error_مقدار, "%s", پیام.message);
    }
}

ایستا صحیح
sync_socket_send_sync (صحیح fd, bool flush_errors, libcrun_error_t *err)
{
  صحیح ret;
  ساختار sync_socket_message_s پیام = {
    0,
  };
  پیام.type = SYNC_SOCKET_SYNC_MESSAGE;

  اگر (fd < 0)
    بازگشت 0;

  ret = TEMP_FAILURE_RETRY (اكتب (fd, &پیام, SYNC_SOCKET_MESSAGE_LEN (پیام, 0)));
  اگر (UNLIKELY (ret < 0))
    {
      اگر (flush_errors)
        {
          صحیح saved_errno = errno;
          ret = TEMP_FAILURE_RETRY (اقرأ (fd, &پیام, اندازه (پیام)));
          اگر (ret >= 0 && پیام.type == SYNC_SOCKET_ERROR_MESSAGE)
            بازگشت crun_make_error (err, پیام.error_مقدار, "%s", پیام.message);
          errno = saved_errno;
        }
      بازگشت crun_make_error (err, errno, "write to sync socket");
    }

  بازگشت 0;
}

ایستا bool
has_cap_sys_ptrace ()
{
#ifdef HAVE_CAP
  ساختار __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  ساختار __user_cap_data_struct data[2] = { { 0 } };

  اگر (UNLIKELY (capget (&hdr, data) < 0))
    بازگشت true;

  بازگشت (data[0].effective & (1U << CAP_SYS_PTRACE)) != 0;
#وگرنه
  بازگشت geteuid () == 0;
#endif
}

ایستا libcrun_container_t *
make_container (runtime_spec_schema_config_schema *container_def, ثابت نویسه *path, ثابت نویسه *config)
{
  libcrun_container_t *container = xmalloc0 (اندازه (*container));
  container->container_def = container_def;

  container->host_uid = geteuid ();
  container->host_gid = getegid ();
  container->host_has_cap_sys_ptrace = has_cap_sys_ptrace ();
  container->proc_fd = -1;

  container->annotations = make_string_map_from_json (container_def->annotations);

  /* An empty namespace path means that a new namespace is to be created,
     the same as if it is not specified.  */
  اگر (container_def->linux)
    {
      size_t i;

      برای (i = 0; i < container_def->linux->namespaces_len; i++)
        اگر (container_def->linux->namespaces[i]->path && container_def->linux->namespaces[i]->path[0] == '\0')
          {
            آزاد (container_def->linux->namespaces[i]->path);
            container_def->linux->namespaces[i]->path = پوچ;
          }
    }

  اگر (path)
    container->config_file = xstrdup (path);
  اگر (config)
    container->config_file_content = xstrdup (config);

  بازگشت container;
}

libcrun_container_t *
libcrun_container_load_from_memory (ثابت نویسه *json, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free نویسه *oci_error = پوچ;
  container_def = runtime_spec_schema_config_schema_parse_data (json, پوچ, &oci_error);
  اگر (container_def == پوچ)
    {
      crun_make_error (err, 0, "load: `%s`", oci_error);
      بازگشت پوچ;
    }
  بازگشت make_container (container_def, پوچ, json);
}

libcrun_container_t *
libcrun_container_load_from_file (ثابت نویسه *path, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free نویسه *oci_error = پوچ;
  cleanup_free نویسه *content = پوچ;
  size_t len;
  صحیح ret;

  libcrun_debug ("Loading container from config file: `%s`", path);
  ret = read_all_file (path, &content, &len, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت پوچ;

  container_def = runtime_spec_schema_config_schema_parse_data (content, پوچ, &oci_error);
  اگر (container_def == پوچ)
    {
      crun_make_error (err, 0, "load `%s`: %s", path, oci_error);
      بازگشت پوچ;
    }
  بازگشت make_container (container_def, پوچ, content);
}

ثابت نویسه *
libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err)
{
  ساختار parser_context ctx = { 0, stderr };
  parser_error gen_err = پوچ;

  اگر (container == پوچ)
    {
      crun_make_error (err, EINVAL, "no container specified");
      بازگشت پوچ;
    }

  اگر (container->config_file_content == پوچ && container->container_def)
    {
      container->config_file_content = runtime_spec_schema_config_schema_generate_json (container->container_def, &ctx, &gen_err);
      اگر (container->config_file_content == پوچ)
        {
          crun_make_error (err, 0, "cannot generate the configuration JSON: %s", gen_err ?: "unknown error");
          آزاد (gen_err);
          بازگشت پوچ;
        }
      آزاد (gen_err);
    }

  اگر (container->config_file_content == پوچ)
    {
      crun_make_error (err, 0, "the container has no configuration");
      بازگشت پوچ;
    }

  بازگشت container->config_file_content;
}

ثابت نویسه *
libcrun_container_get_annotation (libcrun_container_t *container, ثابت نویسه *کلید)
{
  اگر (container == پوچ)
    بازگشت پوچ;
  بازگشت find_string_map_مقدار (container->annotations, کلید);
}

size_t
libcrun_container_get_annotations_len (libcrun_container_t *container)
{
  اگر (container == پوچ || container->annotations == پوچ)
    بازگشت 0;
  بازگشت string_map_size (container->annotations);
}

صحیح
libcrun_container_get_annotation_at (libcrun_container_t *container, size_t index, ثابت نویسه **کلید,
                                     ثابت نویسه **مقدار)
{
  اگر (container == پوچ)
    {
      errno = ERANGE;
      بازگشت -1;
    }
  بازگشت string_map_get_at (container->annotations, index, کلید, مقدار);
}

uid_t
libcrun_container_get_uid (libcrun_container_t *container)
{
  اگر (container == پوچ)
    بازگشت (uid_t) -1;
  بازگشت container->host_uid;
}

gid_t
libcrun_container_get_gid (libcrun_container_t *container)
{
  اگر (container == پوچ)
    بازگشت (gid_t) -1;
  بازگشت container->host_gid;
}

تهی
libcrun_container_free (libcrun_container_t *ctr)
{
  اگر (ctr == پوچ)
    بازگشت;

  اگر (ctr->cleanup_private_data)
    ctr->cleanup_private_data (ctr->private_data);

  اگر (ctr->container_def)
    free_runtime_spec_schema_config_schema (ctr->container_def);

  free_string_map (ctr->annotations);

  اگر (ctr->proc_fd >= 0)
    close (ctr->proc_fd);

  آزاد (ctr->config_file_content);
  آزاد (ctr->config_file);
  آزاد (ctr);
}

libcrun_context_t *
libcrun_context_new (ثابت نویسه *id, ثابت نویسه *state_ریشه, libcrun_error_t *err)
{
  libcrun_context_t *ctx = xmalloc0 (اندازه (*ctx));

  /* Go through the setters so that the owned_* copies and the borrowed
     pointers stay in sync and are released by libcrun_context_free.  */
  libcrun_context_set_id (ctx, id);
  libcrun_context_set_state_ریشه (ctx, state_ریشه);
  libcrun_context_set_bundle (ctx, ".");
  ctx->fifo_exec_wait_fd = -1;

  اگر (libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, id, پوچ, err) < 0)
    {
      libcrun_context_free (ctx);
      بازگشت پوچ;
    }

  ctx->handler_manager = libcrun_handler_manager_create (err);
  اگر (ctx->handler_manager == پوچ)
    {
      libcrun_context_free (ctx);
      بازگشت پوچ;
    }

#ifdef CRUN_LIBDIR
  اگر (access (CRUN_LIBDIR "/handlers", F_OK) == 0)
    {
      اگر (libcrun_handler_manager_load_directory (ctx->handler_manager, CRUN_LIBDIR "/handlers", err) < 0)
        {
          libcrun_context_free (ctx);
          بازگشت پوچ;
        }
    }
#endif

  بازگشت ctx;
}

تهی
libcrun_context_free (libcrun_context_t *ctx)
{
  اگر (ctx == پوچ)
    بازگشت;

  آزاد (ctx->owned_state_ریشه);
  آزاد (ctx->owned_id);
  آزاد (ctx->owned_bundle);
  آزاد (ctx->owned_console_socket);
  آزاد (ctx->owned_pid_file);
  آزاد (ctx->owned_notify_socket);
  آزاد (ctx->owned_handler);

  اگر (ctx->handler_manager)
    handler_manager_free (ctx->handler_manager);

  آزاد (ctx);
}

#define LIBCRUN_CONTEXT_SET_STRING(field)                  \
  انجامبده                                                       \
    {                                                      \
      آزاد (ctx->owned_##field);                           \
      ctx->owned_##field = مقدار ? xstrdup (مقدار) : پوچ; \
      ctx->field = ctx->owned_##field;                     \
  } وقتی (0)

تهی
libcrun_context_set_id (libcrun_context_t *ctx, ثابت نویسه *مقدار)
{
  LIBCRUN_CONTEXT_SET_STRING (id);
}

تهی
libcrun_context_set_state_ریشه (libcrun_context_t *ctx, ثابت نویسه *مقدار)
{
  LIBCRUN_CONTEXT_SET_STRING (state_ریشه);
}

تهی
libcrun_context_set_bundle (libcrun_context_t *ctx, ثابت نویسه *مقدار)
{
  LIBCRUN_CONTEXT_SET_STRING (bundle);
}

تهی
libcrun_context_set_console_socket (libcrun_context_t *ctx, ثابت نویسه *مقدار)
{
  LIBCRUN_CONTEXT_SET_STRING (console_socket);
}

تهی
libcrun_context_set_pid_file (libcrun_context_t *ctx, ثابت نویسه *مقدار)
{
  LIBCRUN_CONTEXT_SET_STRING (pid_file);
}

تهی
libcrun_context_set_notify_socket (libcrun_context_t *ctx, ثابت نویسه *مقدار)
{
  LIBCRUN_CONTEXT_SET_STRING (notify_socket);
}

تهی
libcrun_context_set_handler (libcrun_context_t *ctx, ثابت نویسه *مقدار)
{
  LIBCRUN_CONTEXT_SET_STRING (handler);
}

#undef LIBCRUN_CONTEXT_SET_STRING

ثابت نویسه *
libcrun_context_get_pid_file (libcrun_context_t *ctx)
{
  بازگشت ctx->pid_file;
}

ثابت نویسه *
libcrun_context_get_state_ریشه (libcrun_context_t *ctx)
{
  اگر (ctx == پوچ)
    بازگشت پوچ;
  بازگشت ctx->state_ریشه;
}

تهی
libcrun_context_set_preserve_fds (libcrun_context_t *ctx, صحیح preserve_fds)
{
  ctx->preserve_fds = preserve_fds;
}

صحیح
libcrun_context_get_preserve_fds (libcrun_context_t *ctx)
{
  بازگشت ctx->preserve_fds;
}

تهی
libcrun_context_set_listen_fds (libcrun_context_t *ctx, صحیح listen_fds)
{
  ctx->listen_fds = listen_fds;
}

تهی
libcrun_context_set_args (libcrun_context_t *ctx, صحیح argc, نویسه **argv)
{
  ctx->argc = argc;
  ctx->argv = argv;
}

تهی
libcrun_context_set_systemd_cgroup (libcrun_context_t *ctx, bool مقدار)
{
  ctx->systemd_cgroup = مقدار;
}

تهی
libcrun_context_set_detach (libcrun_context_t *ctx, bool مقدار)
{
  ctx->detach = مقدار;
}

تهی
libcrun_context_set_no_new_keyring (libcrun_context_t *ctx, bool مقدار)
{
  ctx->no_new_keyring = مقدار;
}

تهی
libcrun_context_set_no_pivot (libcrun_context_t *ctx, bool مقدار)
{
  ctx->no_pivot = مقدار;
}

تهی
libcrun_context_set_force_no_cgroup (libcrun_context_t *ctx, bool مقدار)
{
  ctx->force_no_cgroup = مقدار;
}

تهی
libcrun_context_set_output_handler (libcrun_context_t *ctx, crun_output_handler handler, تهی *arg)
{
  ctx->output_handler = handler;
  ctx->output_handler_arg = arg;
}

صحیح
libcrun_context_init_logging (libcrun_context_t *ctx, ثابت نویسه *log, libcrun_error_t *err)
{
  بازگشت libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, ctx->id, log, err);
}

ایستا صحیح
block_signals (libcrun_error_t *err)
{
  صحیح ret;
  sigset_t mask;
  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "sigprocmask");
  بازگشت 0;
}

ایستا صحیح
unblock_signals (libcrun_error_t *err)
{
  صحیح i;
  صحیح ret;
  sigset_t mask;
  ساختار sigaction act = {};

  sigfillset (&mask);
  ret = sigprocmask (SIG_UNBLOCK, &mask, پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "sigprocmask");

  act.sa_handler = SIG_DFL;
  برای (i = 0; i < NSIG; i++)
    {
      ret = sigaction (i, &act, پوچ);
      اگر (ret < 0 && errno != EINVAL)
        بازگشت crun_make_error (err, errno, "sigaction");
    }

  بازگشت 0;
}

/* must be used on the host before pivot_root(2).  */
ایستا صحیح
initialize_security (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, libcrun_error_t *err)
{
  صحیح ret;

  ret = libcrun_init_caps (container, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (UNLIKELY (proc == پوچ))
    بازگشت 0;

  اگر (proc->apparmor_profile)
    {
      ret = libcrun_initialize_apparmor (err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  ret = libcrun_initialize_selinux (container, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت 0;
}

/* Emit the "annotations" map.  Returns a json_gen_status; callers can wrap
   the call with GEN_OR_FAIL.  */
ایستا صحیح
gen_annotations (json_gen_ctx *gen, json_map_string_string *annotations)
{
  size_t i;
  صحیح r;

  GEN_کلید (gen, "annotations");
  GEN_OR_FAIL (json_gen_map_open (gen));

  برای (i = 0; i < annotations->len; i++)
    {
      /* An annotation whose value is null in the document is parsed into a
         NULL, which cannot be emitted as a JSON string; leave it out.  */
      اگر (annotations->کلیدها[i] == پوچ || annotations->مقادیر[i] == پوچ)
        ادامه;

      GEN_STR (gen, annotations->کلیدها[i]);
      GEN_STR (gen, annotations->مقادیر[i]);
    }

  بازگشت json_gen_map_close (gen);

gen_error:
  بازگشت r;
}

/* Return the bundle path to report in the container state.  crun always
   chdir()s into the bundle, so fall back to getcwd() when the caller has no
   absolute bundle path, as can happen when libcrun is used as a library.
   *ALLOCATED must be freed by the caller.  */
ایستا ثابت نویسه *
get_bundle_path (ثابت نویسه *bundle, نویسه **allocated)
{
  اگر (bundle != پوچ && bundle[0] == '/')
    بازگشت bundle;

  *allocated = getcwd (پوچ, 0);
  اگر (*allocated == پوچ)
    OOM ();
  بازگشت *allocated;
}

ایستا صحیح
do_hooks (runtime_spec_schema_config_schema *def, pid_t pid, ثابت نویسه *id, bool keep_going, ثابت نویسه *bundle,
          ثابت نویسه *cwd, ثابت نویسه *status, ثابت نویسه *name, hook **hooks, size_t hooks_len, صحیح out_fd,
          صحیح err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  size_t i, stdin_len;
  صحیح r, ret;
  نویسه *stdin = پوچ;
  cleanup_free نویسه *bundle_allocated = پوچ;
  ثابت نویسه *rootfs = def->ریشه ? def->ریشه->path : "";
  cleanup_json_gen json_gen_ctx *gen = پوچ;

  bundle = get_bundle_path (bundle, &bundle_allocated);

  اگر (! json_gen_init (&gen, پوچ))
    بازگشت crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 0);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_کلید (gen, "ociVersion");
  GEN_STR (gen, "1.0");

  GEN_کلید (gen, "id");
  GEN_STR (gen, id);

  GEN_کلید (gen, "pid");
  GEN_OR_FAIL (map_int (gen, pid));

  GEN_کلید (gen, "root");
  GEN_STR (gen, rootfs);

  GEN_کلید (gen, "bundle");
  GEN_STR (gen, bundle);

  GEN_کلید (gen, "status");
  GEN_STR (gen, status);

  اگر (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, (ثابت نویسه **) &stdin, &stdin_len));

  ret = 0;

  bool error_created = false;
  برای (i = 0; i < hooks_len; i++)
    {
      نویسه **env = environ;

      اگر (hooks[i]->env)
        env = hooks[i]->env;

      /* Release the error from the previous iteration, if any.  */
      اگر (error_created)
        {
          crun_error_release (err);
          error_created = false;
        }

      ret = run_process_with_stdin_timeout_envp (hooks[i]->path, hooks[i]->args, cwd, hooks[i]->timeout, env,
                                                 stdin, stdin_len, out_fd, err_fd, can_ignore_chdir_errors, err);
      اگر (UNLIKELY (ret < 0))
        error_created = true;

      اگر (UNLIKELY (ret != 0))
        {
          اگر (keep_going)
            libcrun_warning ("error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
          وگرنه
            {
              libcrun_error (0, "error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
              بشکن;
            }
        }
    }

  بازگشت ret;

gen_error:
  بازگشت json_gen_error_to_crun_error (r, err);
}

ایستا صحیح
get_json_gen_result (json_gen_ctx *gen, نویسه **out, size_t *out_len)
{
  ثابت نویسه *buf = پوچ;
  size_t buf_len = 0;
  صحیح r;

  r = json_gen_get_buf (gen, &buf, &buf_len);
  اگر (UNLIKELY (r != json_gen_status_ok))
    بازگشت r;

  *out_len = buf_len;

  *out = xmalloc (buf_len + 1);
  memcpy (*out, buf, buf_len);
  (*out)[buf_len] = '\0';

  بازگشت json_gen_status_ok;
}

ایستا صحیح
get_seccomp_receiver_fd_payload (libcrun_container_t *container, ثابت نویسه *status, pid_t own_pid,
                                 نویسه **seccomp_fd_payload, size_t *seccomp_fd_payload_len, libcrun_error_t *err)
{
  صحیح r;
  json_gen_ctx *gen = پوچ;
  runtime_spec_schema_config_schema *def = container->container_def;
  ثابت نویسه *ثابت OCI_VERSION = "0.2.0";

  اگر (! json_gen_init (&gen, پوچ))
    بازگشت crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_کلید (gen, "ociVersion");
  GEN_STR (gen, OCI_VERSION);

  GEN_کلید (gen, "fds");
  GEN_OR_FAIL (json_gen_array_open (gen));

  GEN_کلید (gen, "seccompFd");
  GEN_OR_FAIL (json_gen_array_close (gen));

  GEN_کلید (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  اگر (def && def->linux && def->linux->seccomp)
    {
      ثابت نویسه *metadata = def->linux->seccomp->listener_metadata;

      اگر (metadata)
        {
          GEN_کلید (gen, "metadata");
          GEN_STR (gen, metadata);
        }
    }

  /* State.  */
  GEN_کلید (gen, "state");
  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_کلید (gen, "ociVersion");
  GEN_STR (gen, OCI_VERSION);

  اگر (container->context && container->context->id)
    {
      GEN_کلید (gen, "id");
      GEN_STR (gen, container->context->id);
    }

  GEN_کلید (gen, "status");
  GEN_STR (gen, status);

  GEN_کلید (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  اگر (container->context && container->context->bundle)
    {
      GEN_کلید (gen, "bundle");
      GEN_STR (gen, container->context->bundle);
    }

  اگر (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));
  /* End state.  */

  GEN_OR_FAIL (json_gen_map_close (gen));

  r = get_json_gen_result (gen, seccomp_fd_payload, seccomp_fd_payload_len);

gen_error:
  json_gen_free (gen);

  بازگشت json_gen_error_to_crun_error (r, err);
}

ایستا صحیح
send_sync_cb (تهی *data, libcrun_error_t *err)
{
  صحیح sync_socket_fd = *((صحیح *) data);
  صحیح ret;

  /* sync 2.  */
  ret = sync_socket_send_sync (sync_socket_fd, false, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  /* sync 3.  */
  بازگشت sync_socket_wait_sync (پوچ, sync_socket_fd, false, err);
}

ایستا صحیح
maybe_chown_std_streams (uid_t container_uid, gid_t container_gid, libcrun_error_t *err)
{
  صحیح ret, i;

  برای (i = 0; i < 3; i++)
    {
      اگر (! isatty (i))
        {
          ساختار stat statbuf;
          ret = fstat (i, &statbuf);
          اگر (UNLIKELY (ret < 0))
            {
              اگر (errno == EBADF)
                ادامه;
              بازگشت crun_make_error (err, errno, "fstat fd `%d`", i);
            }

          /* Skip chown for device files */
          اگر (S_ISCHR (statbuf.st_mode) || S_ISBLK (statbuf.st_mode))
            ادامه;

          ret = fchown (i, container_uid, container_gid);
          اگر (UNLIKELY (ret < 0))
            {
              /* EINVAL means the user is not mapped in the current userns.
                 Ignore EPERM and EROFS as well as there is no reason to fail
                 so early, and let the container payload deal with it.
                 EBADF means fd is closed.
              */
              اگر (errno == EINVAL || errno == EPERM || errno == EROFS || errno == EBADF)
                ادامه;

              بازگشت crun_make_error (err, errno, "fchown std stream `%i`", i);
            }
        }
    }
  بازگشت 0;
}

صحیح
libcrun_container_notify_handler (ساختار container_entrypoint_s *args,
                                  شمارش handler_configure_phase phase,
                                  libcrun_container_t *container, ثابت نویسه *rootfs,
                                  libcrun_error_t *err)
{
  ساختار custom_handler_s *h;

  اگر (args->custom_handler == پوچ)
    بازگشت 0;

  h = args->custom_handler->vtable;
  اگر (h == پوچ || h->configure_container == پوچ)
    بازگشت 0;

  بازگشت h->configure_container (args->custom_handler->cookie, phase,
                                 args->context, container,
                                 rootfs, err);
}

/* Resolve and normalize the container rootfs path.  */
ایستا صحیح
resolve_rootfs_path (libcrun_container_t *container, نویسه **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  اگر (def->ریشه && def->ریشه->path)
    {
      *rootfs = realpath (def->ریشه->path, پوچ);
      اگر (UNLIKELY (*rootfs == پوچ))
        {
          صحیح saved_errno = errno;

          /* If realpath failed for any reason, try the relative directory.  */
          اگر (def->ریشه->path[0] == '/')
            {
              cleanup_free نویسه *cwd = پوچ;
              libcrun_error_t tmp_err = پوچ;
              ssize_t len;
              صحیح proc_fd;

              proc_fd = libcrun_get_cached_proc_fd (container, &tmp_err);
              اگر (LIKELY (proc_fd >= 0))
                {
                  len = safe_readlinkat (proc_fd, "self/cwd", &cwd, 0, &tmp_err);
                  اگر (LIKELY (len > 0))
                    {
                      /* If the rootfs is under the current working directory, just use its relative path.  */
                      اگر (has_dir_prefix (def->ریشه->path, cwd))
                        {
                          ثابت نویسه *it = consume_slashes (def->ریشه->path + len);
                          اگر (LIKELY (*it))
                            {
                              صحیح ret = faccessat (AT_FDCWD, it, F_OK, 0);
                              اگر (LIKELY (ret == 0))
                                *rootfs = xstrdup (it);
                            }
                        }
                    }
                }
              crun_error_release (&tmp_err);
            }

          اگر (*rootfs == پوچ)
            {
              اگر (saved_errno == ENOENT)
                بازگشت crun_make_error (err, ENOENT, "the rootfs path `%s` does not exist", def->ریشه->path);

              *rootfs = xstrdup (def->ریشه->path);
            }
        }
    }
  بازگشت 0;
}

/* Initialize the environment variables.  */
ایستا صحیح
setup_environment (runtime_spec_schema_config_schema *def, uid_t container_uid, libcrun_error_t *err)
{
  صحیح ret;

  اگر (clearenv ())
    بازگشت crun_make_error (err, errno, "clearenv");

  اگر (def->process)
    {
      size_t i;

      برای (i = 0; i < def->process->env_len; i++)
        {
          /* A null element of the array of variables is parsed into a NULL,
             which putenv(3) cannot be handed.  */
          اگر (def->process->env[i] == پوچ)
            بازگشت crun_make_error (err, EINVAL, "environment variable is not specified");

          اگر (putenv (def->process->env[i]) < 0)
            بازگشت crun_make_error (err, errno, "putenv `%s`", def->process->env[i]);
        }
    }

  اگر (is_empty_string (getenv ("HOME")))
    {
      ret = set_home_env (container_uid);
      اگر (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          اگر (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            بازگشت crun_make_error (err, errno, "setenv HOME");
        }
    }

  بازگشت 0;
}

/* Configure terminal and console for interactive containers.  */
ایستا صحیح
setup_terminal (ساختار container_entrypoint_s *entrypoint_args, libcrun_container_t *container,
                صحیح console_socket, صحیح console_socketpair, صحیح has_terminal, libcrun_error_t *err)
{
  cleanup_close صحیح terminal_fd = -1;
  صحیح ret;

  اگر (! has_terminal)
    بازگشت 0;

  fflush (stderr);

  اگر (console_socket >= 0 || (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0))
    {
      terminal_fd = libcrun_set_terminal (container, err);
      اگر (UNLIKELY (terminal_fd < 0))
        بازگشت terminal_fd;
    }

  اگر (console_socket >= 0)
    {
      ret = send_fd_to_socket (console_socket, terminal_fd, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  وگرنه اگر (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0)
    {
      ret = send_fd_to_socket (console_socketpair, terminal_fd, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  بازگشت 0;
}

/* Resolve and validate the container's executable path.  */
ایستا صحیح
setup_executable_path (ساختار container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                       نویسه **exec_path, libcrun_error_t *err)
{
  صحیح ret;

  اگر (def->process && def->process->args)
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      اگر (UNLIKELY (ret < 0))
        {
          اگر (entrypoint_args->custom_handler == پوچ && crun_error_get_errno (err) == ENOENT)
            بازگشت ret;

          /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
             once the process switched to the use that runs in the container.  This might be necessary
             when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
             is not honored.  */
          crun_error_release (err);
        }
    }

  بازگشت 0;
}

/* Apply security settings for the container process.  */
ایستا صحیح
apply_security_settings (ساختار container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                         libcrun_container_t *container, pid_t own_pid, libcrun_error_t *err)
{
  صحیح ret;
  runtime_spec_schema_config_schema_process_capabilities *capabilities;
  صحیح no_new_privs;

  اگر (def->process && ! def->process->no_new_privileges)
    {
      نویسه **seccomp_flags = پوچ;
      size_t seccomp_flags_len = 0;
      cleanup_free نویسه *seccomp_fd_payload = پوچ;
      size_t seccomp_fd_payload_len = 0;

      اگر (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      اگر (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  capabilities = def->process ? def->process->capabilities : پوچ;
  no_new_privs = def->process ? def->process->no_new_privileges : 1;
  ret = libcrun_set_caps (capabilities, container->container_uid, container->container_gid, no_new_privs,
                          entrypoint_args->custom_handler != پوچ, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت 0;
}

/* Create the process that joins the existing PID namespace now that the
   host mounts are not visible anymore, and continue the setup from there.  */
ایستا صحیح
do_delayed_pidns_fork (صحیح sync_socket, pid_t *own_pid, libcrun_error_t *err)
{
  pid_t pid;
  صحیح ret;

  pid = fork ();
  اگر (UNLIKELY (pid < 0))
    بازگشت crun_make_error (err, errno, "fork");

  اگر (pid)
    {
      /* Report the new PID to the parent and exit immediately.  */
      ret = sync_socket_send_sync (sync_socket, false, err);
      اگر (LIKELY (ret == 0))
        {
          ret = TEMP_FAILURE_RETRY (اكتب (sync_socket, &pid, اندازه (pid)));
          اگر (LIKELY (ret == اندازه (pid)))
            _safe_exit (EXIT_SUCCESS);
        }

      kill (pid, SIGKILL);
      _safe_exit (EXIT_FAILURE);
    }

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (اقرأ (sync_socket, own_pid, اندازه (*own_pid)));
  اگر (UNLIKELY (ret != اندازه (*own_pid)))
    {
      اگر (ret >= 0)
        errno = 0;
      بازگشت crun_make_error (err, errno, "read from sync socket");
    }

  بازگشت 0;
}

ساختار delayed_pidns_fork_s
{
  libcrun_container_t *container;
  صحیح sync_socket;
  pid_t *own_pid;
};

ایستا صحیح
delayed_pidns_fork_cb (تهی *data, libcrun_error_t *err)
{
  ساختار delayed_pidns_fork_s *d = data;

  اگر (! libcrun_container_delayed_pidns_fork (d->container))
    بازگشت 0;

  بازگشت do_delayed_pidns_fork (d->sync_socket, d->own_pid, err);
}

ایستا صحیح
container_init_setup (تهی *args, pid_t *own_pid, نویسه *notify_socket,
                      صحیح sync_socket, نویسه **exec_path, libcrun_error_t *err)
{
  ساختار container_entrypoint_s *entrypoint_args = args;
  libcrun_container_t *container = entrypoint_args->container;
  bool chdir_done = false;
  صحیح ret;
  صحیح has_terminal;
  cleanup_close صحیح console_socket = -1;
  cleanup_close صحیح console_socketpair = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free نویسه *rootfs = پوچ;
  ساختار delayed_pidns_fork_s delayed_pidns_fork = {
    .container = container,
    .sync_socket = sync_socket,
    .own_pid = own_pid,
  };

  ret = initialize_security (container, def->process, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_configure_network (container, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = resolve_rootfs_path (container, &rootfs, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  /* Configure terminal socket pair for container communication.  */
  اگر (entrypoint_args->terminal_socketpair[0] >= 0)
    {
      close_and_reset (&entrypoint_args->terminal_socketpair[0]);
      console_socketpair = entrypoint_args->terminal_socketpair[1];
    }

  /* sync 1.  */
  ret = sync_socket_wait_sync (پوچ, sync_socket, false, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  has_terminal = container->container_def->process && container->container_def->process->terminal;
  اگر (has_terminal && entrypoint_args->context->console_socket)
    console_socket = entrypoint_args->console_socket_fd;

  ret = libcrun_set_sysctl (container, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_BEFORE_MOUNTS, container, rootfs, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_do_pivot_ریشه (container, entrypoint_args->context->no_pivot, &rootfs, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  /* sync 2 and 3 are sent as part of libcrun_set_mounts.  */
  ret = libcrun_set_mounts (entrypoint_args, container, rootfs, send_sync_cb, &sync_socket, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  /* The createContainer hooks MUST run after the mounts are set up but before
     the pivot_root.  When these hooks are present can_use_open_tree_namespace()
     forces the deferred-pivot path, so the actual pivot happens later in
     libcrun_finalize_mounts().  */
  اگر (def->hooks && def->hooks->create_container_len)
    {
      libcrun_error_t tmp_err = پوچ;
      صحیح in_userns = check_running_in_user_namespace (&tmp_err);
      اگر (tmp_err)
        crun_error_release (&tmp_err);
      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, پوچ, "created", "createContainer",
                      (hook **) def->hooks->create_container,
                      def->hooks->create_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      اگر (UNLIKELY (ret != 0))
        بازگشت ret;
    }

  ret = libcrun_finalize_mounts (entrypoint_args, container, rootfs, delayed_pidns_fork_cb, &delayed_pidns_fork, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (def->process)
    {
      ret = libcrun_set_selinux_label (container, def->process, false, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = libcrun_set_apparmor_profile (container, def->process, false, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  ret = mark_or_close_fds_ge_than (container, entrypoint_args->context->preserve_fds + 3, false, err);
  اگر (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ret = libcrun_reopen_dev_null (err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = maybe_chown_std_streams (container->container_uid, container->container_gid, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = setup_environment (def, container->container_uid, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  /* Set primary process to 1 explicitly if nothing is configured and LISTEN_FD is not set.  */
  اگر (entrypoint_args->context->listen_fds > 0 && getenv ("LISTEN_PID") == پوچ)
    {
      libcrun_warning ("setting LISTEN_PID=1 since no previous configuration was found");
      اگر (UNLIKELY (setenv ("LISTEN_PID", "1", 1) < 0))
        بازگشت crun_make_error (err, errno, "setenv LISTEN_PID");
    }

  /* Attempt to chdir immediately here, before doing the setresuid.  If we fail here, let's
     try again later once the process switched to the user that runs in the container.  */
  اگر (def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      اگر (LIKELY (ret == 0))
        chdir_done = true;
      وگرنه
        crun_error_release (err);
    }

  ret = setsid ();
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "setsid");

  ret = setup_terminal (entrypoint_args, container, console_socket, console_socketpair, has_terminal, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  close_and_reset (&console_socket);
  close_and_reset (&console_socketpair);

  ret = setup_executable_path (entrypoint_args, def, exec_path, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_set_hostname (container, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_set_domainname (container, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (def->process && def->process->user)
    umask (def->process->user->umask_present ? def->process->user->umask : 0022);

  ret = apply_security_settings (entrypoint_args, def, container, *own_pid, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (UNLIKELY (def->process && def->process->args && *exec_path == پوچ))
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      اگر (UNLIKELY (ret < 0))
        {
          اگر (entrypoint_args->custom_handler == پوچ || is_empty_string (def->process->args[0]))
            بازگشت ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          *exec_path = xstrdup (def->process->args[0]);
        }
    }

  /* The chdir was not already performed, so try again now after switching to the UID/GID in the container.  */
  اگر (! chdir_done && def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (notify_socket && putenv (notify_socket) < 0)
    بازگشت crun_make_error (err, errno, "putenv `%s`", notify_socket);

  بازگشت 0;
}

ایستا صحیح
open_hooks_output (libcrun_container_t *container, صحیح *out_fd, صحیح *err_fd, libcrun_error_t *err)
{
  ثابت نویسه *annotation;

  *err_fd = *out_fd = -1;

  libcrun_debug ("Opening hooks output");
  annotation = find_annotation (container, "run.oci.hooks.stdout");
  اگر (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stdout` annotation");
      *out_fd = TEMP_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      اگر (UNLIKELY (*out_fd < 0))
        بازگشت crun_make_error (err, errno, "open `%s`", annotation);
    }

  annotation = find_annotation (container, "run.oci.hooks.stderr");
  اگر (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stderr` annotation");
      *err_fd = TEMP_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      اگر (UNLIKELY (*err_fd < 0))
        بازگشت crun_make_error (err, errno, "open `%s`", annotation);
    }

  بازگشت 0;
}

/* The environment the process was started with lives on the initial stack, right after the
   argv strings and before the string AT_EXECFN points to.  clearenv(3) only drops the pointers
   to it, so when there is no execv to replace the stack, erase it or the container process can
   still read it, e.g. through /proc/self/environ.  */
ایستا تهی
erase_initial_environment (نویسه **argv, صحیح argc)
{
  نویسه *start = argv[argc - 1] + strlen (argv[argc - 1]) + 1;
  نویسه *end = (نویسه *) getauxval (AT_EXECFN);

  اگر (end > start)
    memset (start, 0, end - start);
}

ایستا تهی
rewrite_argv (نویسه **argv, صحیح argc, ثابت نویسه *name, نویسه **args, size_t args_len)
{
  cleanup_free نویسه *decorated_name = پوچ;
  size_t available_len = argv[argc - 1] - argv[0] + strlen (argv[argc - 1]) + 1;
  cleanup_free نویسه *new_argv = پوچ;
  size_t i, so_far = 0, needed = 0;

  needed = xasprintf (&decorated_name, "[libcrun:%s]", name) + 1;
  برای (i = 0; i < args_len; i++)
    needed += strlen (args[i]) + 1;

  new_argv = xmalloc0 (needed + 1);
  strcpy (new_argv, decorated_name);
  so_far = strlen (decorated_name) + 1;

  اگر (available_len >= needed)
    {
      برای (i = 0; i < args_len; i++)
        {
          strcpy (new_argv + so_far, args[i]);
          so_far += strlen (args[i]) + 1;
        }
    }

  اگر (so_far >= available_len)
    so_far = available_len - 1;

  memset (argv[0], 0, available_len);
  memcpy (argv[0], new_argv, so_far);
}

/* Entrypoint to the container.  */
ایستا صحیح
container_init (تهی *args, نویسه *notify_socket, صحیح sync_socket, libcrun_error_t *err)
{
  ساختار container_entrypoint_s *entrypoint_args = args;
  صحیح ret;
  runtime_spec_schema_config_schema *def = entrypoint_args->container->container_def;
  cleanup_free نویسه *exec_path = پوچ;
  __attribute__ ((unused)) cleanup_free نویسه *notify_socket_cleanup = notify_socket;
  pid_t own_pid = 0;

  entrypoint_args->sync_socket = sync_socket;

  crun_set_output_handler (log_write_to_sync_socket, args);

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (اقرأ (sync_socket, &own_pid, اندازه (own_pid)));
  اگر (UNLIKELY (ret != اندازه (own_pid)))
    {
      اگر (ret >= 0)
        errno = 0;
      بازگشت crun_make_error (err, errno, "read from sync socket");
    }

  ret = container_init_setup (args, &own_pid, notify_socket, sync_socket, &exec_path, err);
  اگر (UNLIKELY (ret < 0))
    {
      /* If it fails to write the error using the sync socket, then fallback
         to stderr.  */
      اگر (sync_socket_write_error (sync_socket, err) == 0)
        {
          crun_error_release (err);
          /* A return value of > 0 means to fail with an error without err being set so to avoid
             printing it twice.  */
          بازگشت 1;
        }
      بازگشت ret;
    }

  entrypoint_args->sync_socket = -1;

  ret = unblock_signals (err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  /* sync 4.  */
  ret = sync_socket_send_sync (sync_socket, false, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  close_and_reset (&sync_socket);

  اگر (entrypoint_args->context->fifo_exec_wait_fd >= 0)
    {
      نویسه buffer[1];
      fd_set read_set;
      cleanup_close صحیح fd = entrypoint_args->context->fifo_exec_wait_fd;
      entrypoint_args->context->fifo_exec_wait_fd = -1;

      FD_ZERO (&read_set);
      FD_SET (fd, &read_set);
      انجامبده
        {
          ret = select (fd + 1, &read_set, پوچ, پوچ, پوچ);
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "select");

          ret = TEMP_FAILURE_RETRY (اقرأ (fd, buffer, اندازه (buffer)));
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "read from the exec fifo");
      } وقتی (ret == 0);

      close_and_reset (&entrypoint_args->context->fifo_exec_wait_fd);
    }

  crun_set_output_handler (log_write_to_stderr, پوچ);

  اگر (def->process && def->process->no_new_privileges)
    {
      نویسه **seccomp_flags = پوچ;
      size_t seccomp_flags_len = 0;
      cleanup_free نویسه *seccomp_fd_payload = پوچ;
      size_t seccomp_fd_payload_len = 0;

      اگر (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      اگر (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (entrypoint_args->container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags,
                                   seccomp_flags_len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  اگر (UNLIKELY (def->process == پوچ))
    بازگشت crun_make_error (err, 0, "block `process` not found");

  اگر (UNLIKELY (exec_path == پوچ))
    بازگشت crun_make_error (err, 0, "executable path not specified");

  اگر (def->hooks && def->hooks->start_container_len)
    {
      libcrun_container_t *container = entrypoint_args->container;
      libcrun_error_t tmp_err = پوچ;
      صحیح in_userns = check_running_in_user_namespace (&tmp_err);
      اگر (tmp_err)
        crun_error_release (&tmp_err);

      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, پوچ, "created", "startContainer",
                      (hook **) def->hooks->start_container,
                      def->hooks->start_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      اگر (UNLIKELY (ret != 0))
        بازگشت ret;

      /* Seek stdout/stderr to the end.  If the hooks were using the same files,
         the container process overwrites what was previously written.  */
      (تهی) lseek (1, 0, SEEK_END);
      (تهی) lseek (2, 0, SEEK_END);
    }

  اگر (entrypoint_args->custom_handler)
    {
      /* Files marked with O_CLOEXEC are closed at execv time, so make sure they are closed now.
         This is a best effort operation, because the seccomp filter is already in place and it could
         stop some syscalls used by mark_or_close_fds_ge_than.
      */
      اگر (entrypoint_args->custom_handler->vtable->close_fds)
        {
          ret = entrypoint_args->custom_handler->vtable->close_fds (entrypoint_args->custom_handler->cookie,
                                                                    entrypoint_args->container,
                                                                    entrypoint_args->context->preserve_fds,
                                                                    err);
        }
      وگرنه
        {
          ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
        }
      اگر (UNLIKELY (ret < 0))
        crun_error_release (err);

      prctl (PR_SET_NAME, entrypoint_args->custom_handler->vtable->name);

      اگر (entrypoint_args->context->argv)
        {
          erase_initial_environment (entrypoint_args->context->argv, entrypoint_args->context->argc);
          rewrite_argv (entrypoint_args->context->argv, entrypoint_args->context->argc,
                        entrypoint_args->custom_handler->vtable->name, def->process->args,
                        def->process->args_len);

          /* It is a quite destructive operation as we might be referencing data from the old
             argv, so make sure the context is not reused.  */
          entrypoint_args->context->argv = پوچ;
          entrypoint_args->context = پوچ;
        }

      ret = libcrun_set_selinux_label (entrypoint_args->container, def->process, true, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = libcrun_set_apparmor_profile (entrypoint_args->container, def->process, true, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = entrypoint_args->custom_handler->vtable->run_func (entrypoint_args->custom_handler->cookie,
                                                               entrypoint_args->container,
                                                               exec_path,
                                                               def->process->args);
      اگر (ret != 0)
        بازگشت crun_make_error (err, 0, "exec container process failed with handler as `%s`", entrypoint_args->custom_handler->vtable->name);

      بازگشت ret;
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation since the seccomp profile is already in place now and might block
     some of the syscalls needed by mark_or_close_fds_ge_than.  */
  ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
  اگر (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, def->process->args));

  اگر (errno == ENOENT)
    بازگشت crun_make_error (err, errno, "exec container process (missing dynamic library?) `%s`", exec_path);

  بازگشت crun_make_error (err, errno, "exec container process `%s`", exec_path);
}

ایستا صحیح
read_container_config_from_state (libcrun_container_t **container, ثابت نویسه *state_ریشه, ثابت نویسه *id,
                                  libcrun_error_t *err)
{
  cleanup_free نویسه *config_file = پوچ;
  cleanup_free نویسه *dir = پوچ;
  صحیح ret;

  *container = پوچ;

  ret = libcrun_get_state_directory (&dir, state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = append_paths (&config_file, err, dir, "config.json", پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  *container = libcrun_container_load_from_file (config_file, err);
  اگر (*container == پوچ)
    بازگشت -1;

  بازگشت 0;
}

ایستا صحیح
run_poststop_hooks (arg_unused libcrun_context_t *context, libcrun_container_t *container,
                    runtime_spec_schema_config_schema *def,
                    libcrun_container_status_t *status, ثابت نویسه *state_ریشه, ثابت نویسه *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container_cleanup = پوچ;
  صحیح ret;

  اگر (def == پوچ)
    {
      اگر (container == پوچ)
        {
          ret = read_container_config_from_state (&container_cleanup, state_ریشه, id, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
          container = container_cleanup;
        }

      def = container->container_def;
    }

  اگر (def->hooks && def->hooks->poststop_len)
    {
      cleanup_close صحیح hooks_out_fd = -1;
      cleanup_close صحیح hooks_err_fd = -1;

      اگر (container == پوچ)
        {
          ret = read_container_config_from_state (&container_cleanup, state_ریشه, id, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
          container = container_cleanup;
        }

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = do_hooks (def, 0, id, true, status->bundle, status->bundle, "stopped", "poststop", (hook **) def->hooks->poststop,
                      def->hooks->poststop_len, hooks_out_fd, hooks_err_fd, false, err);
      اگر (UNLIKELY (ret != 0))
        {
          اگر (ret < 0)
            libcrun_error_report_and_release (err);
          وگرنه
            libcrun_error (0, "poststop hook failed with exit code: %d", ret);
        }
    }
  بازگشت 0;
}

ایستا bool
has_new_pid_namespace (runtime_spec_schema_config_schema *def)
{
  size_t i;

  اگر (def->linux == پوچ || def->linux->namespaces == پوچ)
    بازگشت false;

  برای (i = 0; i < def->linux->namespaces_len; i++)
    {
      اگر (strcmp (def->linux->namespaces[i]->type, "pid") == 0 && def->linux->namespaces[i]->path == پوچ)
        بازگشت true;
    }
  بازگشت false;
}

/* Delete the container state.  CONTAINER is the container the caller already has
   available, or NULL.  When it is not NULL, the poststop hooks are run even if the
   status file was never written, e.g. when the container creation failed.  */
ایستا صحیح
container_delete_internal (libcrun_context_t *context, libcrun_container_t *container,
                           runtime_spec_schema_config_schema *def, ثابت نویسه *id, bool force, bool killall,
                           libcrun_error_t *err)
{
  cleanup_cgroup_status ساختار libcrun_cgroup_status *cgroup_status = پوچ;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_container libcrun_container_t *container_cleanup = پوچ;
  ثابت نویسه *state_ریشه = context->state_ریشه;
  صحیح ret;

  ret = libcrun_read_container_status (&status, state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (force && crun_error_get_errno (err) == ENOENT)
        {
          libcrun_error_t tmp_err = پوچ;

          crun_error_release (err);

          اگر (container)
            {
              libcrun_container_status_t partial_status = {
                .bundle = (نویسه *) context->bundle,
              };

              ret = run_poststop_hooks (context, container, def, &partial_status, state_ریشه, id, err);
              اگر (UNLIKELY (ret < 0))
                crun_error_write_warning_and_release (context->output_handler_arg, &err);
            }

          libcrun_container_delete_status (state_ریشه, id, &tmp_err);
          crun_error_release (&tmp_err);
          بازگشت 0;
        }
      crun_error_release (err);
      بازگشت libcrun_container_delete_status (state_ریشه, id, err);
    }

  cgroup_status = libcrun_cgroup_make_status (&status);

  اگر (! force)
    {
      ret = libcrun_is_container_running (&status, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
      اگر (ret == 1)
        {
          ret = libcrun_status_has_read_exec_fifo (state_ریشه, id, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
          اگر (ret == 0)
            بازگشت crun_make_error (err, 0, "the container `%s` is not in `created` or `stopped` state", id);

          /* If the container is in "created" state, then do the equivalent of delete --force.  */
          killall = force = true;
        }
    }

  اگر (def == پوچ)
    {
      اگر (container == پوچ)
        {
          ret = read_container_config_from_state (&container_cleanup, state_ریشه, id, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;

          container = container_cleanup;
        }

      def = container->container_def;
    }

  اگر (killall && force)
    {
      /* If the container has a pid namespace, it is enough to kill the first
         process (pid=1 in the namespace).
      */
      اگر (has_new_pid_namespace (def))
        {
          ret = libcrun_kill_linux (&status, SIGKILL, err);
          اگر (UNLIKELY (ret < 0))
            {
              اگر (crun_error_get_errno (err) != ESRCH)
                بازگشت ret;

              crun_error_release (err);
            }
        }
      وگرنه اگر (status.cgroup_path)
        {
          ret = libcrun_cgroup_killall (cgroup_status, SIGKILL, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }
    }

  اگر (def->linux && def->linux->intel_rdt)
    {
      ret = libcrun_destroy_intelrdt (id, def, err);
      اگر (UNLIKELY (ret < 0))
        libcrun_error_report_and_release (err);
    }

  اگر (status.cgroup_path)
    {
      /* If the cgroup cannot be destroyed, there are still processes in it.
         Keep the container status, it is the only record of the cgroup, so
         that the delete can be retried.  */
      ret = libcrun_cgroup_destroy (cgroup_status, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  ret = run_poststop_hooks (context, container, def, &status, state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  بازگشت libcrun_container_delete_status (state_ریشه, id, err);
}

صحیح
libcrun_container_delete (libcrun_context_t *context, ثابت نویسه *id, bool force, libcrun_error_t *err)
{
  بازگشت container_delete_internal (context, پوچ, پوچ, id, force, true, err);
}

صحیح
libcrun_container_kill (libcrun_context_t *context, ثابت نویسه *id, ثابت نویسه *سیگنال, libcrun_error_t *err)
{
  صحیح sig, ret;
  ثابت نویسه *state_ریشه = context->state_ریشه;
  cleanup_container_status libcrun_container_status_t status = {};

  sig = str2sig (سیگنال);
  اگر (UNLIKELY (sig < 0))
    بازگشت crun_make_error (err, 0, "unknown signal `%s`", سیگنال);

  ret = libcrun_read_container_status (&status, state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت libcrun_kill_linux (&status, sig, err);
}

صحیح
libcrun_container_killall (libcrun_context_t *context, ثابت نویسه *id, ثابت نویسه *سیگنال, libcrun_error_t *err)
{
  صحیح sig, ret;
  ثابت نویسه *state_ریشه = context->state_ریشه;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_cgroup_status ساختار libcrun_cgroup_status *cgroup_status = پوچ;

  sig = str2sig (سیگنال);
  اگر (UNLIKELY (sig < 0))
    بازگشت crun_make_error (err, 0, "unknown signal `%s`", سیگنال);

  ret = libcrun_read_container_status (&status, state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  cgroup_status = libcrun_cgroup_make_status (&status);

  ret = libcrun_cgroup_killall (cgroup_status, sig, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;
  بازگشت 0;
}

ایستا صحیح
write_container_status (libcrun_container_t *container, libcrun_context_t *context,
                        pid_t pid, ساختار libcrun_cgroup_status *cgroup_status,
                        libcrun_error_t *err)
{
  cleanup_free نویسه *bundle_allocated = پوچ;
  ثابت نویسه *bundle = get_bundle_path (context->bundle, &bundle_allocated);
  cleanup_free نویسه *owner = get_user_name (geteuid ());
  نویسه *external_descriptors = libcrun_get_external_descriptors (container);
  نویسه *rootfs = container->container_def->ریشه ? container->container_def->ریشه->path : "";
  نویسه created[35];

  libcrun_container_status_t status = {
    .pid = pid,
    .rootfs = rootfs,
    .bundle = (نویسه *) bundle,
    .created = created,
    .owner = owner,
    .systemd_cgroup = context->systemd_cgroup,
    .detached = context->detach,
    .no_new_keyring = context->no_new_keyring,
    .external_descriptors = external_descriptors,
    .cgroup_path = پوچ,
    .scope = پوچ,
  };

  get_current_timestamp (created, اندازه (created));

  اگر (cgroup_status)
    {
      صحیح ret;

      ret = libcrun_cgroup_get_status (cgroup_status, &status, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (external_descriptors == پوچ)
    بازگشت crun_make_error (err, 0, "invalid internal state.  No external descriptors found");
  بازگشت libcrun_write_container_status (context->state_ریشه, context->id, &status, err);
}

ایستا صحیح
reap_subprocesses (pid_t *main_process, صحیح *main_process_exit, صحیح *last_process, libcrun_error_t *err)
{
  *last_process = 0;
  وقتی (1)
    {
      صحیح status;
      صحیح r = waitpid_ignore_stopped (-1, &status, WNOHANG);
      اگر (r < 0)
        {
          اگر (errno == EINTR)
            ادامه;
          اگر (errno == ECHILD)
            {
              *last_process = 1;
              بازگشت 0;
            }
          بازگشت crun_make_error (err, errno, "waitpid");
        }
      اگر (r == 0)
        بشکن;
      اگر (r != *main_process)
        ادامه;

      *main_process_exit = get_process_exit_status (status);

      /* Once reaped, the pid can be reused by an unrelated process.  */
      *main_process = 0;
    }
  بازگشت 0;
}

ایستا صحیح
send_sd_notify (ثابت نویسه *ready_str, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  صحیح ret = sd_notify (0, ready_str);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, -ret, "sd_notify");

#  اگر HAVE_SD_NOTIFY_BARRIER
  /* Hard-code a 30 seconds timeout.  Ignore errors.  */
  sd_notify_barrier (0, 30 * 1000000);
#  endif
#وگرنه
  (تهی) ready_str;
  (تهی) err;
#endif
  بازگشت 0;
}

ایستا صحیح
handle_notify_socket (صحیح notify_socketfd, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  صحیح ret;
  نویسه buf[256];
  ثابت نویسه *ready_str = "READY=1";

  ret = recvfrom (notify_socketfd, buf, اندازه (buf) - 1, 0, پوچ, پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "recvfrom notify socket");

  buf[ret] = '\0';
  اگر (strstr (buf, ready_str))
    {
      ret = send_sd_notify (ready_str, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      بازگشت 1;
    }
  بازگشت 0;
#وگرنه
  (تهی) notify_socketfd;
  (تهی) err;
  بازگشت 1;
#endif
}

ساختار wait_for_process_args
{
  pid_t pid;
  libcrun_context_t *context;
  صحیح terminal_fd;
  صحیح notify_socket;
  صحیح *container_ready_fd;
  صحیح seccomp_notify_fd;
  ثابت نویسه *seccomp_notify_plugins;
};

ایستا صحیح
write_pid_file (ثابت نویسه *pid_file, pid_t pid, libcrun_error_t *err)
{
  نویسه buf[32];
  صحیح buf_len = snprintf (buf, اندازه (buf), "%d", pid);
  اگر (UNLIKELY (buf_len >= (صحیح) اندازه (buf)))
    بازگشت crun_make_error (err, 0, "internal error: static buffer too small");
  بازگشت write_file_at_with_flags (AT_FDCWD, O_CREAT | O_TRUNC, 0700, pid_file, buf, buf_len, err);
}

#define MAX_EVENTS 10

ایستا bool
is_fd_pollable (صحیح fd)
{
  ساختار epoll_event ev = { .events = EPOLLIN };
  cleanup_close صحیح epollfd = epoll_create1 (EPOLL_CLOEXEC);

  اگر (epollfd < 0)
    بازگشت true;

  بازگشت epoll_ctl (epollfd, EPOLL_CTL_ADD, fd, &ev) == 0 || errno != EPERM;
}

ایستا صحیح
wait_for_process (ساختار wait_for_process_args *args, libcrun_error_t *err)
{
  cleanup_channel_fd_pair ساختار channel_fd_pair *from_terminal = پوچ;
  cleanup_channel_fd_pair ساختار channel_fd_pair *to_terminal = پوچ;
  صحیح ret, container_exit_code = 0, last_process;
  cleanup_close صحیح terminal_fd_from = -1;
  cleanup_close صحیح terminal_fd_to = -1;
  bool stdin_pollable = false, stdout_pollable = false;
  cleanup_close صحیح epollfd = -1;
  cleanup_close صحیح signalfd = -1;
  sigset_t mask;
  صحیح in_fds[MAX_EVENTS];
  صحیح in_fds_len = 0;
  صحیح out_fds[MAX_EVENTS];
  صحیح out_fds_len = 0;
  size_t i;

  cleanup_seccomp_notify_context ساختار seccomp_notify_context_s *seccomp_notify_ctx = پوچ;

  container_exit_code = 0;

  اگر (args == پوچ || args->context == پوچ)
    بازگشت crun_make_error (err, 0, "internal error: context is empty");

  برای (i = 0; i < MAX_EVENTS; i++)
    {
      in_fds[i] = -1;
      out_fds[i] = -1;
    }

  اگر (args->context->pid_file)
    {
      ret = write_pid_file (args->context->pid_file, args->pid, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  /* Also exit if there is nothing more to wait for.  */
  اگر (args->context->detach && args->notify_socket < 0)
    بازگشت 0;

  اگر (args->container_ready_fd)
    {
      ret = 0;
      TEMP_FAILURE_RETRY (اكتب (*args->container_ready_fd, &ret, اندازه (ret)));
      close_and_reset (args->container_ready_fd);
    }

  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "sigprocmask");

  signalfd = create_signalfd (&mask, err);
  اگر (UNLIKELY (signalfd < 0))
    بازگشت signalfd;

  ret = reap_subprocesses (&args->pid, &container_exit_code, &last_process, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (last_process)
    بازگشت container_exit_code;

  اگر (args->seccomp_notify_fd >= 0)
    {
      cleanup_free نویسه *state_ریشه = پوچ;
      cleanup_free نویسه *oci_config_path = پوچ;

      ساختار libcrun_load_seccomp_notify_conf_s conf;
      memset (&conf, 0, اندازه conf);

      ret = libcrun_get_state_directory (&state_ریشه, args->context->state_ریشه, args->context->id, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = append_paths (&oci_config_path, err, state_ریشه, "config.json", پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      conf.runtime_ریشه_path = state_ریشه;
      conf.name = args->context->id;
      conf.bundle_path = args->context->bundle;
      conf.oci_config_path = oci_config_path;

      ret = set_blocking_fd (args->seccomp_notify_fd, false, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = libcrun_load_seccomp_notify_plugins (&seccomp_notify_ctx,
                                                 args->seccomp_notify_plugins,
                                                 &conf, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      in_fds[in_fds_len++] = args->seccomp_notify_fd;
    }

  اگر (args->terminal_fd >= 0)
    {
      /* The terminal_fd is dup()ed so that it can be registered with
         epoll multiple times using different masks.  */
      terminal_fd_from = dup (args->terminal_fd);
      اگر (UNLIKELY (terminal_fd_from < 0))
        بازگشت crun_make_error (err, errno, "dup terminal fd");
      terminal_fd_to = dup (args->terminal_fd);
      اگر (UNLIKELY (terminal_fd_to < 0))
        بازگشت crun_make_error (err, errno, "dup terminal fd");

      صحیح i, non_blocking_fds[] = { terminal_fd_from, terminal_fd_to, 0, 1, -1 };
      برای (i = 0; non_blocking_fds[i] >= 0; i++)
        {
          ret = set_blocking_fd (non_blocking_fds[i], false, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }

      /* stdin and stdout can be something epoll does not support, such as
         /dev/null or a regular file.  Such a stdin is not forwarded, and
         such a stdout is written to without waiting for it to be ready.  */
      stdin_pollable = is_fd_pollable (0);
      stdout_pollable = is_fd_pollable (1);

      from_terminal = channel_fd_pair_new (terminal_fd_from, 1, BUFSIZ);
      اگر (stdin_pollable)
        to_terminal = channel_fd_pair_new (0, terminal_fd_to, BUFSIZ);
    }

  in_fds[in_fds_len++] = signalfd;
  اگر (args->notify_socket >= 0)
    in_fds[in_fds_len++] = args->notify_socket;
  اگر (args->terminal_fd >= 0)
    {
      اگر (stdin_pollable)
        {
          in_fds[in_fds_len++] = 0;
          out_fds[out_fds_len++] = terminal_fd_to;
        }

      in_fds[in_fds_len++] = terminal_fd_from;
      اگر (stdout_pollable)
        out_fds[out_fds_len++] = 1;
    }

  epollfd = epoll_helper (in_fds, پوچ, out_fds, پوچ, err);
  اگر (UNLIKELY (epollfd < 0))
    بازگشت epollfd;

  وقتی (1)
    {
      ساختار epoll_event events[MAX_EVENTS];
      ساختار signalfd_siginfo si;
      ساختار winsize ws;
      صحیح i, nr_events;
      ssize_t res;

      nr_events = TEMP_FAILURE_RETRY (epoll_wait (epollfd, events, MAX_EVENTS, -1));
      اگر (UNLIKELY (nr_events < 0))
        بازگشت crun_make_error (err, errno, "epoll_wait");

      برای (i = 0; i < nr_events; i++)
        {
          اگر (events[i].data.fd == 0 || events[i].data.fd == terminal_fd_to)
            {
              ret = channel_fd_pair_process (to_terminal, epollfd, err);
              اگر (UNLIKELY (ret < 0))
                بازگشت crun_error_wrap (err, "copy to terminal fd");
            }
          وگرنه اگر (events[i].data.fd == 1 || events[i].data.fd == terminal_fd_from)
            {
              ret = channel_fd_pair_process (from_terminal, epollfd, err);
              اگر (UNLIKELY (ret < 0))
                بازگشت crun_error_wrap (err, "copy from terminal fd");
            }
          وگرنه اگر (events[i].data.fd == args->seccomp_notify_fd)
            {
              ret = libcrun_seccomp_notify_plugins (seccomp_notify_ctx,
                                                    args->seccomp_notify_fd, err);
              اگر (UNLIKELY (ret < 0))
                بازگشت ret;
            }
          وگرنه اگر (events[i].data.fd == args->notify_socket)
            {
              ret = handle_notify_socket (args->notify_socket, err);
              اگر (UNLIKELY (ret < 0))
                بازگشت ret;
              اگر (ret && args->context->detach)
                بازگشت 0;
            }
          وگرنه اگر (events[i].data.fd == signalfd)
            {
              res = TEMP_FAILURE_RETRY (اقرأ (signalfd, &si, اندازه (si)));
              اگر (UNLIKELY (res < 0))
                بازگشت crun_make_error (err, errno, "read from signalfd");
              اگر (si.ssi_signo == SIGCHLD)
                {
                  ret = reap_subprocesses (&args->pid, &container_exit_code,
                                           &last_process, err);
                  اگر (UNLIKELY (ret < 0))
                    بازگشت ret;
                  اگر (last_process)
                    بازگشت container_exit_code;
                }
              وگرنه اگر (si.ssi_signo == SIGWINCH)
                {
                  /* Ignore the signal if the terminal is not available.  */
                  اگر (args->terminal_fd > 0)
                    {
                      ret = ioctl (0, TIOCGWINSZ, &ws);
                      اگر (UNLIKELY (ret < 0))
                        بازگشت crun_make_error (err, errno, "ioctl TIOCGWINSZ copy terminal size from stdin");

                      ret = ioctl (args->terminal_fd, TIOCSWINSZ, &ws);
                      اگر (UNLIKELY (ret < 0))
                        بازگشت crun_make_error (err, errno, "ioctl TIOCSWINSZ copy terminal size to pty");
                    }
                }
              وگرنه اگر (args->pid > 0)
                {
                  /* Send any other signal to the child process, if it was not reaped yet.  */
                  ret = kill (args->pid, si.ssi_signo);
                }
            }
          وگرنه
            {
              بازگشت crun_make_error (err, 0, "internal error: unknown fd from epoll_wait");
            }
        }
    }

  بازگشت 0;
}

ایستا تهی
flush_fd_to_err (libcrun_context_t *context, صحیح terminal_fd)
{
  نویسه buf[256];
  صحیح flags;

  اگر (terminal_fd < 0 || stderr == پوچ)
    بازگشت;

  flags = fcntl (terminal_fd, F_GETFL, 0);
  اگر (flags == -1)
    بازگشت;
  اگر (fcntl (terminal_fd, F_SETFL, flags | O_NONBLOCK) < 0)
    بازگشت;

  برای (;;)
    {
      صحیح ret = TEMP_FAILURE_RETRY (اقرأ (terminal_fd, buf, اندازه (buf) - 1));
      اگر (ret <= 0)
        بشکن;
      buf[ret] = '\0';
      اگر (context->output_handler)
        context->output_handler (0, buf, LIBCRUN_VERBOSITY_ERROR, context->output_handler_arg);
    }
  (تهی) fcntl (terminal_fd, F_SETFL, flags);
  fflush (stderr);
  fsync (1);
  fsync (2);
}

ایستا صحیح
cleanup_watch (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
               ساختار libcrun_cgroup_status *cgroup_status, pid_t init_pid, صحیح sync_socket,
               صحیح terminal_fd, libcrun_error_t *err)
{
  ثابت نویسه *oom_message = پوچ;
  libcrun_error_t tmp_err = پوچ;
  صحیح ret;

  اگر (init_pid)
    {
      /* Try to detect whether the cgroup has a OOM.  */
      اگر (cgroup_status)
        {
          صحیح has_oom;

          has_oom = libcrun_cgroup_has_oom (cgroup_status, &tmp_err);
          اگر (has_oom > 0)
            oom_message = "OOM: the memory limit could be too low";
          وگرنه اگر (has_oom < 0)
            {
              /* If the detection has failed for any reason, e.g. the cgroup was
                 already deleted by the time it was checked, just ignore the
                 failure.  */
              crun_error_release (&tmp_err);
            }
        }
      /* If the OOM wasn't detected, look into the static configuration.  */
      اگر (oom_message == پوچ && is_memory_limit_too_کم (def))
        oom_message = "the memory limit could be too low";

      kill (init_pid, SIGKILL);
      waitpid_ignore_stopped (init_pid, پوچ, 0);
    }

  ret = sync_socket_wait_sync (context, sync_socket, true, &tmp_err);
  اگر (UNLIKELY (ret < 0))
    {
      crun_error_release (err);
      *err = tmp_err;
    }

  اگر (terminal_fd >= 0)
    flush_fd_to_err (context, terminal_fd);

  اگر (oom_message)
    بازگشت crun_error_wrap (err, "%s", oom_message);

  بازگشت -1;
}

/* Find the uid:gid that is mapped to root inside the container user namespace.  */
تهی
get_ریشه_in_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid, uid_t *uid,
                        gid_t *gid)
{
  *uid = -1;
  *gid = -1;

  /* Not root in the namespace.  */
  اگر (host_uid)
    بازگشت;

  اگر (def->linux && def->linux->uid_mappings)
    {
      size_t i;

      برای (i = 0; i < def->linux->uid_mappings_len; i++)
        اگر (def->linux->uid_mappings[i]->container_id == 0)
          {
            *uid = def->linux->uid_mappings[i]->host_id;
            بشکن;
          }
    }
  اگر (def->linux && def->linux->gid_mappings)
    {
      size_t i;

      برای (i = 0; i < def->linux->gid_mappings_len; i++)
        اگر (def->linux->gid_mappings[i]->container_id == 0)
          {
            *gid = def->linux->gid_mappings[i]->host_id;
            بشکن;
          }
    }

  /* If the uid and the gid are not changed, do not attempt any chown.  */
  اگر (*uid == host_uid && *gid == host_gid)
    *uid = *gid = -1;
}

ایستا صحیح
get_seccomp_receiver_fd (libcrun_container_t *container, صحیح *fd, صحیح *self_receiver_fd, ثابت نویسه **plugins,
                         libcrun_error_t *err)
{
  ثابت نویسه *tmp;
  runtime_spec_schema_config_schema *def = container->container_def;

  *fd = -1;
  *self_receiver_fd = -1;

  tmp = find_annotation (container, "run.oci.seccomp.plugins");
  اگر (tmp)
    {
      صحیح fds[2];
      صحیح ret;

      ret = create_socket_pair (fds, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_error_wrap (err, "create socket pair");

      *fd = fds[0];
      *self_receiver_fd = fds[1];
      *plugins = tmp;
      بازگشت 0;
    }

  اگر (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    tmp = def->linux->seccomp->listener_path;
  وگرنه
    tmp = find_annotation (container, "run.oci.seccomp.receiver");
  اگر (tmp == پوچ)
    tmp = getenv ("RUN_OCI_SECCOMP_RECEIVER");
  اگر (tmp)
    {
      اگر (tmp[0] != '/')
        بازگشت crun_make_error (err, 0, "the seccomp receiver `%s` is not an absolute path", tmp);

      *fd = open_unix_domain_client_socket (tmp, 0, err);
      اگر (UNLIKELY (*fd < 0))
        بازگشت crun_error_wrap (err, "open seccomp receiver");
    }

  بازگشت 0;
}

ایستا bool
has_seccomp_receiver (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;

  اگر (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    بازگشت true;

  اگر (find_annotation (container, "run.oci.seccomp.receiver") != پوچ || getenv ("RUN_OCI_SECCOMP_RECEIVER") != پوچ)
    بازگشت true;

  بازگشت false;
}

ایستا صحیح
setup_container_hooks_output (libcrun_container_t *container, runtime_spec_schema_config_schema *def,
                              ساختار container_entrypoint_s *container_args, صحیح *hooks_out_fd,
                              صحیح *hooks_err_fd, libcrun_error_t *err)
{
  صحیح ret;

  اگر (def->hooks
      && (def->hooks->prestart_len || def->hooks->poststart_len || def->hooks->create_runtime_len
          || def->hooks->create_container_len || def->hooks->start_container_len))
    {
      ret = open_hooks_output (container, hooks_out_fd, hooks_err_fd, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
      container_args->hooks_out_fd = *hooks_out_fd;
      container_args->hooks_err_fd = *hooks_err_fd;
    }
  بازگشت 0;
}

ایستا صحیح
setup_container_keyring (libcrun_container_t *container, libcrun_context_t *context, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحیح ret;

  اگر (! context->no_new_keyring)
    {
      ثابت نویسه *label = پوچ;

      libcrun_debug ("Creating new keyring");

      اگر (def->process)
        {
          label = def->process->selinux_label;
          اگر (label)
            libcrun_debug ("Using SELinux process label: `%s`", label);
        }

      ret = libcrun_create_keyring (container, container->context->id, label, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  بازگشت 0;
}

ایستا صحیح
setup_terminal_socket_pair (libcrun_container_t *container, libcrun_context_t *context,
                            ساختار container_entrypoint_s *container_args, صحیح *socket_pair_0,
                            صحیح *socket_pair_1, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحیح ret;
  صحیح detach = context->detach;

  اگر (def->process && def->process->terminal && ! detach && context->console_socket == پوچ)
    {
      libcrun_debug ("Creating terminal socket pair");
      container_args->has_terminal_socket_pair = 1;
      ret = create_socket_pair (container_args->terminal_socketpair, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_error_wrap (err, "create terminal socket");

      *socket_pair_0 = container_args->terminal_socketpair[0];
      *socket_pair_1 = container_args->terminal_socketpair[1];
    }
  بازگشت 0;
}

ایستا صحیح
setup_seccomp (libcrun_container_t *container, ثابت نویسه *seccomp_bpf_data,
               ساختار libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, صحیح *seccomp_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحیح ret;

  اگر (find_annotation (container, "run.oci.seccomp.plugins") != پوچ && has_seccomp_receiver (container))
    {
      بازگشت crun_make_error (err, 0, "seccomp plugins and seccomp receivers cannot be declared at the same time");
    }

  اگر (def->linux && (def->linux->seccomp || seccomp_bpf_data))
    {
      unsigned صحیح seccomp_gen_options = 0;
      ثابت نویسه *annotation;

      libcrun_debug ("Initializing seccomp");
      annotation = find_annotation (container, "run.oci.seccomp_fail_unknown_syscall");
      اگر (annotation && strcmp (annotation, "0") != 0)
        seccomp_gen_options = LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL;

      اگر (seccomp_bpf_data)
        seccomp_gen_options |= LIBCRUN_SECCOMP_SKIP_CACHE;

      libcrun_seccomp_gen_ctx_init (seccomp_gen_ctx, container, true, seccomp_gen_options);

      ret = libcrun_open_seccomp_bpf (seccomp_gen_ctx, seccomp_fd, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  بازگشت 0;
}

ایستا صحیح
setup_console_socket (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
                      ساختار container_entrypoint_s *container_args, صحیح *console_socket_fd,
                      libcrun_error_t *err)
{
  اگر (context->console_socket && def->process && def->process->terminal)
    {
      *console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
      اگر (UNLIKELY (*console_socket_fd < 0))
        بازگشت crun_error_wrap (err, "open console socket");
      container_args->console_socket_fd = *console_socket_fd;
    }
  بازگشت 0;
}

ایستا صحیح
setup_cgroup_manager (libcrun_context_t *context, libcrun_container_t *container,
                      ساختار libcrun_cgroup_args *cg, صحیح *cgroup_dirfd,
                      ساختار libcrun_dirfd_s *cgroup_dirfd_s, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحیح cgroup_manager;
  uid_t ریشه_uid = -1;
  gid_t ریشه_gid = -1;
  صحیح ret;

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  اگر (context->systemd_cgroup)
    {
      libcrun_debug ("Using systemd cgroup manager");
      cgroup_manager = CGROUP_MANAGER_SYSTEMD;
    }
  وگرنه اگر (context->force_no_cgroup)
    {
      libcrun_debug ("Disabling cgroup manager");
      cgroup_manager = CGROUP_MANAGER_DISABLED;
    }
  وگرنه
    libcrun_debug ("Using cgroupfs cgroup manager");

  /* If we are root (either on the host or in a namespace), then chown the cgroup to root
     in the container user namespace.  */
  get_ریشه_in_the_userns (def, container->host_uid, container->host_gid, &ریشه_uid, &ریشه_gid);
  libcrun_debug ("Using container host UID `%d` and GID `%d`", container->host_uid, container->host_gid);

  memset (cg, 0, اندازه (*cg));

  cg->cgroup_path = def->linux ? def->linux->cgroups_path : "";
  cg->manager = cgroup_manager;
  cg->id = context->id;
  cg->resources = def->linux ? def->linux->resources : پوچ;
  cg->annotations = container->annotations;
  cg->ریشه_uid = ریشه_uid;
  cg->ریشه_gid = ریشه_gid;
  cg->state_ریشه = context->state_ریشه;
  cg->container = container;

  ret = libcrun_cgroup_preenter (cg, cgroup_dirfd, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  cgroup_dirfd_s->dirfd = cgroup_dirfd;
  cgroup_dirfd_s->joined = false;

  بازگشت 0;
}

ایستا صحیح
set_scheduler (pid_t pid, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  صحیح ret;

  ret = libcrun_set_scheduler (pid, def->process, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_reset_cpu_affinity_mask (pid, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_set_io_priority (pid, def->process, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت 0;
}

ایستا صحیح
seccomp_generation (صحیح seccomp_fd, ثابت نویسه *seccomp_bpf_data,
                    ساختار libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, libcrun_error_t *err)
{
  صحیح ret;

  اگر (seccomp_fd >= 0)
    {
      اگر (seccomp_bpf_data != پوچ)
        {
          ret = libcrun_copy_seccomp (seccomp_gen_ctx, seccomp_bpf_data, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }
      وگرنه
        {
          ret = libcrun_generate_seccomp (seccomp_gen_ctx, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
        }
    }
  بازگشت 0;
}

ایستا صحیح
terminal_setup (runtime_spec_schema_config_schema *def, libcrun_context_t *context,
                صحیح socket_pair_0, صحیح *terminal_fd, تهی **orig_terminal, libcrun_error_t *err)
{
  صحیح ret;
  صحیح detach = context->detach;

  اگر (def->process && def->process->terminal && ! detach && context->console_socket == پوچ)
    {
      libcrun_debug ("Receiving console socket fd");
      *terminal_fd = receive_fd_from_socket (socket_pair_0, err);
      اگر (UNLIKELY (*terminal_fd < 0))
        بازگشت -1;

      ret = libcrun_clear_onlcr (*terminal_fd, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = libcrun_set_raw (0, orig_terminal, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  بازگشت 0;
}

ایستا صحیح
libcrun_container_run_internal (libcrun_container_t *container, libcrun_context_t *context,
                                صحیح *container_ready_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحیح ret;
  pid_t pid;
  صحیح detach = context->detach;
  cleanup_cgroup_status ساختار libcrun_cgroup_status *cgroup_status = پوچ;
  cleanup_close صحیح terminal_fd = -1;
  cleanup_terminal تهی *orig_terminal = پوچ;
  cleanup_close صحیح sync_socket = -1;
  cleanup_close صحیح notify_socket = -1;
  cleanup_close صحیح socket_pair_0 = -1;
  cleanup_close صحیح socket_pair_1 = -1;
  cleanup_close صحیح seccomp_fd = -1;
  cleanup_close صحیح console_socket_fd = -1;
  cleanup_close صحیح hooks_out_fd = -1;
  cleanup_close صحیح hooks_err_fd = -1;
  cleanup_close صحیح own_seccomp_receiver_fd = -1;
  cleanup_close صحیح seccomp_notify_fd = -1;
  ثابت نویسه *seccomp_notify_plugins = پوچ;
  cleanup_cgroup_preenter ساختار libcrun_cgroup_args cg = {};
  cleanup_free نویسه *bundle_allocated = پوچ;
  ساختار container_entrypoint_s container_args = {
    .container = container,
    .context = context,
    .bundle = get_bundle_path (context->bundle, &bundle_allocated),
    .terminal_socketpair = { -1, -1 },
    .console_socket_fd = -1,
    .hooks_out_fd = -1,
    .hooks_err_fd = -1,
    .seccomp_receiver_fd = -1,
    .custom_handler = پوچ,
  };
  cleanup_close صحیح cgroup_dirfd = -1;
  ساختار libcrun_dirfd_s cgroup_dirfd_s;
  ساختار libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  ثابت نویسه *seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");
  صحیح cgroup_mode;

  اگر (! context->force_no_cgroup)
    {
      cgroup_mode = libcrun_get_cgroup_mode (err);
      اگر (UNLIKELY (cgroup_mode < 0))
        بازگشت cgroup_mode;

      اگر (cgroup_mode != CGROUP_MODE_UNIFIED)
        libcrun_warning ("cgroup v1 is deprecated and will be removed in a future release.  Use cgroup v2");
    }

  ret = setup_container_hooks_output (container, def, &container_args, &hooks_out_fd, &hooks_err_fd, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  container->context = context;

  اگر (! detach || context->notify_socket)
    {
      libcrun_debug ("Setting child subreaper");
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "prctl set child subreaper");
    }

  ret = setup_container_keyring (container, context, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = setup_terminal_socket_pair (container, context, &container_args, &socket_pair_0, &socket_pair_1, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = block_signals (err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  umask (0);

  ret = libcrun_set_mempolicy (def, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;
  container_args.seccomp_fd = seccomp_fd;

  اگر (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &container_args.seccomp_receiver_fd, &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  ret = setup_console_socket (context, def, &container_args, &console_socket_fd, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = setup_cgroup_manager (context, container, &cg, &cgroup_dirfd, &cgroup_dirfd_s, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_configure_handler (container_args.context->handler_manager,
                                   container_args.context,
                                   container,
                                   &(container_args.custom_handler),
                                   err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (container_args.custom_handler && container_args.custom_handler->vtable->modify_oci_configuration)
    {
      libcrun_debug ("Using custom handler to modify OCI configuration");
      ret = container_args.custom_handler->vtable->modify_oci_configuration (container_args.custom_handler->cookie,
                                                                             container_args.context,
                                                                             container->container_def,
                                                                             err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  pid = libcrun_run_linux_container (container, container_init, &container_args, &sync_socket, &cgroup_dirfd_s, err);
  اگر (UNLIKELY (pid < 0))
    بازگشت pid;

  cg.pid = pid;
  cg.joined = cgroup_dirfd_s.joined;
  libcrun_debug ("Running container on PID: `%d`", pid);

  اگر (context->fifo_exec_wait_fd < 0 && context->notify_socket)
    {
      libcrun_debug ("Using notify socket: `%s`", context->notify_socket);
      /* Do not open the notify socket here on "create".  "start" will take care of it.  */
      ret = get_notify_fd (context, container, &notify_socket, err);
      اگر (UNLIKELY (ret < 0))
        برو fail;
    }

  اگر (container_args.terminal_socketpair[1] >= 0)
    close_and_reset (&socket_pair_1);

  ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
  اگر (UNLIKELY (ret < 0))
    برو fail;

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE, err);
  اگر (UNLIKELY (ret < 0))
    برو fail;

  ret = libcrun_move_network_devices (container, pid, err);
  اگر (UNLIKELY (ret < 0))
    برو fail;

  /* sync send own pid.  */
  ret = TEMP_FAILURE_RETRY (اكتب (sync_socket, &pid, اندازه (pid)));
  اگر (UNLIKELY (ret != اندازه (pid)))
    {
      اگر (ret >= 0)
        errno = 0;
      crun_make_error (err, errno, "write to sync socket");
      برو fail;
    }

  /* sync 1.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  اگر (UNLIKELY (ret < 0))
    برو fail;

  /* sync 2.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  اگر (UNLIKELY (ret < 0))
    برو fail;

  ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
  اگر (UNLIKELY (ret < 0))
    برو fail;

  /* If the container process is created later, the scheduler is set on it then.  */
  اگر (! libcrun_container_delayed_pidns_fork (container))
    {
      ret = set_scheduler (pid, def, err);
      اگر (UNLIKELY (ret < 0))
        برو fail;
    }

  /* The container is waiting that we write back.  In this phase we can launch the
     prestart hooks.  */
  اگر (def->hooks && def->hooks->prestart_len)
    {
      libcrun_debug ("Running `prestart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "prestart", (hook **) def->hooks->prestart,
                      def->hooks->prestart_len, hooks_out_fd, hooks_err_fd, false, err);
      اگر (UNLIKELY (ret != 0))
        برو fail;
    }
  اگر (def->hooks && def->hooks->create_runtime_len)
    {
      libcrun_debug ("Running `create` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "createRuntime", (hook **) def->hooks->create_runtime,
                      def->hooks->create_runtime_len, hooks_out_fd, hooks_err_fd, false, err);
      اگر (UNLIKELY (ret != 0))
        برو fail;
    }

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  اگر (UNLIKELY (ret < 0))
    برو fail;
  close_and_reset (&seccomp_fd);

  /* sync 3.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  اگر (UNLIKELY (ret < 0))
    برو fail;

  /* The process joining an existing PID namespace is created once the container root is in place.  */
  اگر (libcrun_container_delayed_pidns_fork (container))
    {
      pid_t new_pid = 0;

      ret = sync_socket_wait_sync (context, sync_socket, false, err);
      اگر (UNLIKELY (ret < 0))
        برو fail;

      ret = TEMP_FAILURE_RETRY (اقرأ (sync_socket, &new_pid, اندازه (new_pid)));
      اگر (UNLIKELY (ret != اندازه (new_pid)))
        {
          اگر (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "read pid from sync socket");
          برو fail;
        }

      /* Cleanup the first process.  */
      waitpid_ignore_stopped (pid, پوچ, 0);

      pid = new_pid;

      ret = set_scheduler (pid, def, err);
      اگر (UNLIKELY (ret < 0))
        برو fail;

      ret = libcrun_handle_pidfd_receiver (pid, container, err);
      اگر (UNLIKELY (ret < 0))
        برو fail;

      /* sync send own pid.  */
      ret = TEMP_FAILURE_RETRY (اكتب (sync_socket, &pid, اندازه (pid)));
      اگر (UNLIKELY (ret != اندازه (pid)))
        {
          اگر (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "write to sync socket");
          برو fail;
        }
    }

  ret = terminal_setup (def, context, socket_pair_0, &terminal_fd, &orig_terminal, err);
  اگر (UNLIKELY (ret < 0))
    برو fail;

  close_and_reset (&socket_pair_0);

  /* sync 4.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  اگر (UNLIKELY (ret < 0))
    برو fail;

  ret = close_and_reset (&sync_socket);
  اگر (UNLIKELY (ret < 0))
    {
      crun_make_error (err, errno, "close sync_socket failed");
      برو fail;
    }

  libcrun_debug ("Writing container status");
  ret = write_container_status (container, context, pid, cgroup_status, err);
  اگر (UNLIKELY (ret < 0))
    برو fail;

  /* Run poststart hooks here only if the container is created using "run".  For create+start, the
     hooks will be executed as part of the start command.  */
  اگر (context->fifo_exec_wait_fd < 0 && def->hooks && def->hooks->poststart_len)
    {
      libcrun_debug ("Running `poststart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      اگر (UNLIKELY (ret != 0))
        برو fail;
    }

  /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
  اگر (own_seccomp_receiver_fd >= 0)
    {
      libcrun_debug ("Receiving seccomp fd");
      seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
      اگر (UNLIKELY (seccomp_notify_fd < 0))
        برو fail;

      ret = close_and_reset (&own_seccomp_receiver_fd);
      اگر (UNLIKELY (ret < 0))
        {
          crun_make_error (err, errno, "close seccomp receiver fd failed");
          برو fail;
        }
    }

  {
    ساختار wait_for_process_args args = {
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
  اگر (! context->detach)
    {
      libcrun_error_t tmp_err = پوچ;
      cleanup_watch (context, def, cgroup_status, 0, sync_socket, terminal_fd, &tmp_err);
      crun_error_release (&tmp_err);
    }

  بازگشت ret;

fail:
  ret = cleanup_watch (context, def, cgroup_status, pid, sync_socket, terminal_fd, err);
  اگر (cgroup_status)
    {
      libcrun_error_t tmp_err = پوچ;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  بازگشت ret;
}

ایستا صحیح
check_config_file (runtime_spec_schema_config_schema *def, libcrun_context_t *context, libcrun_error_t *err)
{
  اگر (UNLIKELY (def->linux == پوچ))
    بازگشت crun_make_error (err, 0, "invalid config file, no `linux` block specified");

  اگر (context->handler == پوچ)
    {
      اگر (UNLIKELY (def->ریشه == پوچ))
        بازگشت crun_make_error (err, 0, "invalid config file, no `root` block specified");
      اگر (UNLIKELY (def->mounts == پوچ))
        بازگشت crun_make_error (err, 0, "invalid config file, no `mounts` block specified");
    }
  بازگشت 0;
}

ایستا صحیح
libcrun_copy_config_file (ثابت نویسه *id, ثابت نویسه *state_ریشه, libcrun_container_t *container, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_free نویسه *dest_path = پوچ;
  cleanup_free نویسه *dir = پوچ;
  cleanup_free نویسه *buffer = پوچ;
  size_t len;

  ret = libcrun_get_state_directory (&dir, state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = append_paths (&dest_path, err, dir, "config.json", پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (container->config_file == پوچ && container->config_file_content == پوچ)
    بازگشت crun_make_error (err, 0, "config file not specified");

  اگر (container->config_file == پوچ)
    {
      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, container->config_file_content, strlen (container->config_file_content), err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }
  وگرنه
    {
      libcrun_debug ("Reading config file: `%s`", container->config_file);
      ret = read_all_file (container->config_file, &buffer, &len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, buffer, len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  بازگشت 0;
}

ایستا تهی
force_delete_container_status (libcrun_context_t *context, libcrun_container_t *container,
                               runtime_spec_schema_config_schema *def)
{
  libcrun_error_t tmp_err = پوچ;
  container_delete_internal (context, container, def, context->id, true, false, &tmp_err);
  libcrun_error_report_and_release (&tmp_err);
}

/* Serialize ERR to FD as an int errno followed by a NUL-terminated message.
   Used by forked children to report a failure to the parent process.  */
ایستا تهی
write_error_to_pipe (صحیح fd, libcrun_error_t *err)
{
  ثابت نویسه *پیام = (*err)->پیام;
  صحیح errcode = crun_error_get_errno (err);

  TEMP_FAILURE_RETRY (اكتب (fd, &errcode, اندازه (errcode)));
  TEMP_FAILURE_RETRY (اكتب (fd, پیام, strlen (پیام) + 1));
}

صحیح
libcrun_container_run (libcrun_context_t *context, libcrun_container_t *container, unsigned صحیح options,
                       libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحیح ret;
  صحیح detach = context->detach;
  صحیح container_ret_status[2];
  cleanup_close صحیح pipefd0 = -1;
  cleanup_close صحیح pipefd1 = -1;
  libcrun_error_t tmp_err = پوچ;

  container->context = context;

  ret = validate_options (options, LIBCRUN_RUN_OPTIONS_PREFORK | LIBCRUN_RUN_OPTIONS_KEEP, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = check_config_file (def, context, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (def->process && def->process->terminal && detach && context->console_socket == پوچ)
    بازگشت crun_make_error (err, 0, "use --console-socket with --detach when a terminal is used");

  ret = libcrun_status_check_directories (context->state_ریشه, context->id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (! detach && (options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      ret = libcrun_copy_config_file (context->id, context->state_ریشه, container, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = libcrun_container_run_internal (container, context, پوچ, err);
      اگر (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
        force_delete_container_status (context, container, def);
      بازگشت ret;
    }

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  ret = fork ();
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "fork");
  اگر (ret)
    {
      صحیح status;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, پوچ, 0);

      ret = TEMP_FAILURE_RETRY (اقرأ (pipefd0, &status, اندازه (status)));
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, errno, "read from sync pipe");

      اگر (status < 0)
        {
          صحیح errno_;
          نویسه buf[512];
          ret = TEMP_FAILURE_RETRY (اقرأ (pipefd0, &errno_, اندازه (errno_)));
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "read from sync pipe");

          ret = TEMP_FAILURE_RETRY (اقرأ (pipefd0, buf, اندازه (buf) - 1));
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "read from sync pipe");
          buf[ret] = '\0';

          بازگشت crun_make_error (err, errno_, "%s", buf);
        }

      بازگشت status;
    }

  close_and_reset (&pipefd0);

  /* forked process.  */
  ret = detach_process ();
  اگر (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_ریشه, container, &tmp_err);
  اگر (UNLIKELY (ret < 0))
    برو fail;

  ret = libcrun_container_run_internal (container, context, پوچ, &tmp_err);
  TEMP_FAILURE_RETRY (اكتب (pipefd1, &ret, اندازه (ret)));
  اگر (UNLIKELY (ret < 0))
    برو fail;

  exit (EXIT_SUCCESS);
fail:

  اگر (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
    force_delete_container_status (context, container, def);
  اگر (tmp_err)
    {
      write_error_to_pipe (pipefd1, &tmp_err);
      crun_error_release (&tmp_err);
    }

  exit (EXIT_FAILURE);
}

صحیح
libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container, unsigned صحیح options,
                          libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحیح ret;
  صحیح container_ready_pipe[2];
  cleanup_close صحیح pipefd0 = -1;
  cleanup_close صحیح pipefd1 = -1;
  cleanup_close صحیح exec_fifo_fd = -1;
  context->detach = 1;

  libcrun_debug ("Creating container: `%s`", context->id);
  container->context = context;

  ret = validate_options (options, LIBCRUN_CREATE_OPTIONS_PREFORK, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = check_config_file (def, context, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (def->process && def->process->terminal && context->console_socket == پوچ)
    بازگشت crun_make_error (err, 0, "use --console-socket with create when a terminal is used");

  ret = libcrun_status_check_directories (context->state_ریشه, context->id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  exec_fifo_fd = libcrun_status_create_exec_fifo (context->state_ریشه, context->id, err);
  اگر (UNLIKELY (exec_fifo_fd < 0))
    بازگشت exec_fifo_fd;

  context->fifo_exec_wait_fd = exec_fifo_fd;
  exec_fifo_fd = -1;

  اگر ((options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      libcrun_debug ("Running with prefork enabled");
      ret = libcrun_copy_config_file (context->id, context->state_ریشه, container, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
      ret = libcrun_container_run_internal (container, context, پوچ, err);
      اگر (UNLIKELY (ret < 0))
        force_delete_container_status (context, container, def);
      بازگشت ret;
    }

  ret = pipe2 (container_ready_pipe, O_CLOEXEC);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "pipe");
  pipefd0 = container_ready_pipe[0];
  pipefd1 = container_ready_pipe[1];

  ret = fork ();
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "fork");
  اگر (ret)
    {
      صحیح exit_code;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, پوچ, 0);

      ret = TEMP_FAILURE_RETRY (اقرأ (pipefd0, &exit_code, اندازه (exit_code)));
      اگر (UNLIKELY (ret <= 0))
        بازگشت crun_make_error (err, ret < 0 ? errno : 0, "waiting for container to be ready");

      اگر (ret != اندازه (exit_code) || exit_code < 0)
        بازگشت crun_make_error (err, 0, "internal error: read invalid exit code");

      اگر (exit_code == 0)
        بازگشت 0;

      libcrun_debug ("Exit code is `%d`, deleting container", exit_code);
      libcrun_error_t tmp_err = پوچ;
      container_delete_internal (context, پوچ, def, context->id, true, true, &tmp_err);
      libcrun_error_report_and_release (&tmp_err);
      بازگشت crun_make_error (err, 0, "error creating container");
    }

  /* forked process.  */
  ret = detach_process ();
  اگر (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_ریشه, container, err);
  اگر (UNLIKELY (ret < 0))
    {
      صحیح errcode = crun_error_get_errno (err);
      crun_error_release (err);
      libcrun_fail_with_error (errcode, "copy config file");
    }

  ret = libcrun_container_run_internal (container, context, &pipefd1, err);
  اگر (UNLIKELY (ret < 0))
    {
      force_delete_container_status (context, container, def);
      libcrun_error ((*err)->status, "%s", (*err)->پیام);
      crun_error_release (err);
      crun_set_output_handler (log_write_to_stderr, پوچ);
    }

  اگر (pipefd1 >= 0)
    TEMP_FAILURE_RETRY (اكتب (pipefd1, &ret, اندازه (ret)));
  exit (ret ? EXIT_FAILURE : 0);
}

صحیح
libcrun_container_start (libcrun_context_t *context, ثابت نویسه *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = پوچ;
  ثابت نویسه *state_ریشه = context->state_ریشه;
  runtime_spec_schema_config_schema *def;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_close صحیح fd = -1;
  صحیح ret;

  ret = libcrun_read_container_status (&status, state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_is_container_running (&status, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (! ret)
    بازگشت crun_make_error (err, 0, "container `%s` is not running", id);

  ret = read_container_config_from_state (&container, state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (context->notify_socket)
    {
      ret = get_notify_fd (context, container, &fd, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  ret = libcrun_status_write_exec_fifo (context->state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  def = container->container_def;

  اگر (context->notify_socket)
    {
      اگر (fd >= 0)
        {
          fd_set read_set;

          وقتی (1)
            {
              ساختار timeval timeout = {
                .tv_sec = 0,
                .tv_usec = 10000,
              };
              FD_ZERO (&read_set);
              FD_SET (fd, &read_set);

              ret = select (fd + 1, &read_set, پوچ, پوچ, &timeout);
              اگر (UNLIKELY (ret < 0))
                بازگشت crun_make_error (err, errno, "select failed");
              اگر (ret)
                {
                  ret = handle_notify_socket (fd, err);
                  اگر (UNLIKELY (ret < 0))
                    بازگشت ret;
                  اگر (ret)
                    بشکن;
                }
              وگرنه
                {
                  ret = libcrun_is_container_running (&status, err);
                  اگر (UNLIKELY (ret < 0))
                    بازگشت ret;
                  اگر (! ret)
                    بازگشت 0;
                }
            }
        }
    }

  /* The container is considered running only after we got the notification from the
     notify_socket, if any.  */
  اگر (def->hooks && def->hooks->poststart_len)
    {
      cleanup_close صحیح hooks_out_fd = -1;
      cleanup_close صحیح hooks_err_fd = -1;

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = do_hooks (def, status.pid, context->id, false, status.bundle, status.bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      اگر (UNLIKELY (ret != 0))
        {
          libcrun_error_t tmp_err = پوچ;
          container_delete_internal (context, container, def, id, true, true, &tmp_err);
          libcrun_error_report_and_release (&tmp_err);
          بازگشت ret;
        }
    }

  بازگشت 0;
}

صحیح
libcrun_get_container_state_string (ثابت نویسه *id, libcrun_container_status_t *status, ثابت نویسه *state_ریشه,
                                    ثابت نویسه **container_status, صحیح *running, libcrun_error_t *err)
{
  صحیح ret, has_fifo = 0;
  bool paused = false;

  ret = libcrun_is_container_running (status, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;
  *running = ret;

  اگر (*running)
    {
      ret = libcrun_status_has_read_exec_fifo (state_ریشه, id, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
      has_fifo = ret;
    }

  اگر (*running && ! has_fifo)
    {
      cleanup_cgroup_status ساختار libcrun_cgroup_status *cgroup_status = پوچ;

      cgroup_status = libcrun_cgroup_make_status (status);

      ret = libcrun_cgroup_is_container_paused (cgroup_status, &paused, err);
      اگر (UNLIKELY (ret < 0))
        {
          /*
            The cgroup might have been cleaned up by the time we try to read it, ignore both
            ENOENT and ENODEV:
            - ENOENT: if the open(CGROUP_PATH) fails because the cgroup was deleted.
            - ENODEV: if the cgroup is deleted between the open and reading the freeze status.
          */

          errno = crun_error_get_errno (err);
          اگر (errno == ENOENT || errno == ENODEV)
            {
              crun_error_release (err);
              *container_status = "stopped";
              بازگشت 0;
            }

          بازگشت ret;
        }
    }

  اگر (! *running)
    *container_status = "stopped";
  وگرنه اگر (has_fifo)
    *container_status = "created";
  وگرنه اگر (paused)
    *container_status = "paused";
  وگرنه
    *container_status = "running";

  بازگشت 0;
}

صحیح
libcrun_container_state_json (libcrun_context_t *context, ثابت نویسه *id, نویسه **out, libcrun_error_t *err)
{
  نویسه *buffer = پوچ;
  size_t len = 0;
  FILE *f;
  صحیح ret;

  f = open_memstream (&buffer, &len);
  اگر (f == پوچ)
    بازگشت crun_make_error (err, errno, "open_memstream");

  ret = libcrun_container_state (context, id, f, err);
  fclose (f);
  اگر (UNLIKELY (ret < 0))
    {
      آزاد (buffer);
      بازگشت ret;
    }

  *out = buffer;
  بازگشت 0;
}

صحیح
libcrun_container_state (libcrun_context_t *context, ثابت نویسه *id, FILE *out, libcrun_error_t *err)
{
  ثابت نویسه *ثابت OCI_CONFIG_VERSION = "1.0.0";
  libcrun_container_status_t status = {};
  ثابت نویسه *state_ریشه = context->state_ریشه;
  ثابت نویسه *container_status = پوچ;
  json_gen_ctx *gen = پوچ;
  ثابت نویسه *buf;
  صحیح ret = 0;
  صحیح r = json_gen_status_ok;
  صحیح running;
  size_t len;

  ret = libcrun_read_container_status (&status, state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_get_container_state_string (id, &status, state_ریشه, &container_status, &running, err);
  اگر (UNLIKELY (ret < 0))
    برو exit;

  ret = 0;
  اگر (! json_gen_init (&gen, پوچ))
    بازگشت crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));
  GEN_کلید (gen, "ociVersion");
  GEN_STR (gen, OCI_CONFIG_VERSION);

  GEN_کلید (gen, "id");
  GEN_STR (gen, id);

  GEN_کلید (gen, "pid");
  GEN_OR_FAIL (map_int (gen, running ? status.pid : 0));

  GEN_کلید (gen, "status");
  GEN_STR (gen, container_status);

  GEN_کلید (gen, "bundle");
  GEN_STR (gen, status.bundle);

  GEN_کلید (gen, "rootfs");
  GEN_STR (gen, status.rootfs);

  GEN_کلید (gen, "created");
  GEN_STR (gen, status.created);

  اگر (status.scope)
    {
      GEN_کلید (gen, "systemd-scope");
      GEN_STR (gen, status.scope);
    }

  اگر (status.owner)
    {
      GEN_کلید (gen, "owner");
      GEN_STR (gen, status.owner);
    }

  {
    cleanup_container libcrun_container_t *container = پوچ;

    ret = read_container_config_from_state (&container, state_ریشه, id, err);
    اگر (UNLIKELY (ret < 0))
      برو exit;

    اگر (container->container_def->annotations && container->container_def->annotations->len)
      GEN_OR_FAIL (gen_annotations (gen, container->container_def->annotations));
  }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  fprintf (out, "%s\n", buf);

exit:
  اگر (gen)
    json_gen_free (gen);
  libcrun_free_container_status (&status);
  بازگشت ret;

gen_error:
  ret = json_gen_error_to_crun_error (r, err);
  برو exit;
}

صحیح
libcrun_container_exec (libcrun_context_t *context, ثابت نویسه *id, runtime_spec_schema_config_schema_process *process,
                        libcrun_error_t *err)
{
  ساختار libcrun_container_exec_options_s opts;
  memset (&opts, 0, اندازه (opts));

  opts.struct_size = اندازه (opts);
  opts.process = process;

  بازگشت libcrun_container_exec_with_options (context, id, &opts, err);
}

صحیح
libcrun_container_exec_process_file (libcrun_context_t *context, ثابت نویسه *id, ثابت نویسه *path, libcrun_error_t *err)
{
  ساختار libcrun_container_exec_options_s opts;
  memset (&opts, 0, اندازه (opts));

  opts.struct_size = اندازه (opts);
  opts.path = path;

  بازگشت libcrun_container_exec_with_options (context, id, &opts, err);
}

ایستا صحیح
exec_from_process_json (libcrun_context_t *context, ثابت نویسه *id, ثابت نویسه *process_json, ثابت نویسه *cgroup,
                        bool merge_env, libcrun_error_t *err)
{
  ساختار parser_context parser_ctx = { .options = 0, .errfile = stderr };
  ساختار libcrun_container_exec_options_s opts;
  runtime_spec_schema_config_schema_process *process;
  parser_error parser_err = پوچ;
  json_object *doc = پوچ;
  صحیح ret;

  ret = parse_json_file (&doc, process_json, &parser_ctx, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  process = make_runtime_spec_schema_config_schema_process (doc, &parser_ctx, &parser_err);
  json_object_put (doc);
  اگر (process == پوچ)
    {
      ret = crun_make_error (err, 0, "cannot parse process: `%s`", parser_err ? parser_err : "unknown error");
      آزاد (parser_err);
      بازگشت ret;
    }

  memset (&opts, 0, اندازه (opts));
  opts.struct_size = اندازه (opts);
  opts.process = process;
  opts.cgroup = cgroup;
  opts.merge_env = merge_env;

  ret = libcrun_container_exec_with_options (context, id, &opts, err);
  free_runtime_spec_schema_config_schema_process (process);
  بازگشت ret;
}

صحیح
libcrun_container_exec_json (libcrun_context_t *context, ثابت نویسه *id, ثابت نویسه *process_json, libcrun_error_t *err)
{
  بازگشت exec_from_process_json (context, id, process_json, پوچ, false, err);
}

صحیح
libcrun_container_exec_with_options_json (libcrun_context_t *context, ثابت نویسه *id,
                                          ثابت ساختار libcrun_exec_options_s *opts, libcrun_error_t *err)
{
  اگر (opts == پوچ || opts->struct_size == 0)
    بازگشت crun_make_error (err, EINVAL, "invalid exec options");

  بازگشت exec_from_process_json (context, id, opts->process_json, opts->cgroup, opts->merge_env, err);
}

#define cleanup_process_schema __attribute__ ((cleanup (cleanup_process_schemap)))

ایستا درونخطی تهی
cleanup_process_schemap (runtime_spec_schema_config_schema_process **p)
{
  runtime_spec_schema_config_schema_process *process = *p;
  اگر (process)
    (تهی) free_runtime_spec_schema_config_schema_process (process);
}

/* Build the seccomp receiver payload (when needed), apply the seccomp profile and
   close the seccomp file descriptors.  Used from exec_process_entrypoint both before
   and after the capabilities are set, depending on process->no_new_privileges.  */
ایستا صحیح
apply_seccomp_for_exec (libcrun_container_t *container, pid_t own_pid, صحیح seccomp_fd,
                        صحیح seccomp_receiver_fd, نویسه **seccomp_flags, size_t seccomp_flags_len,
                        libcrun_error_t *err)
{
  cleanup_free نویسه *seccomp_fd_payload = پوچ;
  size_t seccomp_fd_payload_len = 0;
  صحیح ret;

  اگر (seccomp_receiver_fd >= 0)
    {
      ret = get_seccomp_receiver_fd_payload (container, "running", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  ret = libcrun_apply_seccomp (seccomp_fd, seccomp_receiver_fd, seccomp_fd_payload,
                               seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  close_and_reset (&seccomp_fd);
  close_and_reset (&seccomp_receiver_fd);
  بازگشت 0;
}

ایستا صحیح
exec_process_entrypoint (libcrun_context_t *context,
                         libcrun_container_t *container,
                         runtime_spec_schema_config_schema_process *process,
                         صحیح *pipefd1,
                         صحیح seccomp_fd,
                         صحیح seccomp_receiver_fd,
                         ساختار custom_handler_instance_s *custom_handler,
                         bool merge_env,
                         libcrun_error_t *err)
{
  runtime_spec_schema_config_schema_process_capabilities *capabilities = پوچ;
  cleanup_free نویسه *exec_path = پوچ;
  uid_t container_uid;
  gid_t container_gid;
  ثابت نویسه *cwd;
  bool chdir_done = false;
  size_t seccomp_flags_len = 0;
  نویسه **seccomp_flags = پوچ;
  pid_t own_pid = 0;
  size_t i;
  صحیح ret;

  container_uid = process->user ? process->user->uid : 0;
  container_gid = process->user ? process->user->gid : 0;

  TEMP_FAILURE_RETRY (اقرأ (*pipefd1, &own_pid, اندازه (own_pid)));

  cwd = process->cwd ? process->cwd : "/";
  اگر (LIKELY (libcrun_safe_chdir (cwd, err) == 0))
    chdir_done = true;
  وگرنه
    crun_error_release (err);

  ret = unblock_signals (err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = clearenv ();
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "clearenv");

  اگر ((merge_env || ! process->env_len) && container->container_def->process && container->container_def->process->env_len)
    {
      نویسه *e;

      برای (i = 0; i < container->container_def->process->env_len; i++)
        {
          e = container->container_def->process->env[i];
          اگر (putenv (e) < 0)
            بازگشت crun_make_error (err, errno, "putenv `%s`", e);
        }
    }
  اگر (process->env_len)
    {
      برای (i = 0; i < process->env_len; i++)
        اگر (putenv (process->env[i]) < 0)
          بازگشت crun_make_error (err, errno, "putenv `%s`", process->env[i]);
    }

  اگر (is_empty_string (getenv ("HOME")))
    {
      ret = set_home_env (container_uid);
      اگر (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          اگر (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            بازگشت crun_make_error (err, errno, "setenv HOME");
        }
    }

  ret = libcrun_set_selinux_label (container, process, false, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_set_apparmor_profile (container, process, false, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (container->container_def->linux && container->container_def->linux->seccomp)
    {
      seccomp_flags = container->container_def->linux->seccomp->flags;
      seccomp_flags_len = container->container_def->linux->seccomp->flags_len;
    }

  ret = find_executable (&exec_path, process->args[0], process->cwd, err);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (crun_error_get_errno (err) == ENOENT)
        بازگشت ret;

      /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
         once the process switched to the use that runs in the container.  This might be necessary
         when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
         is not honored.  */
      crun_error_release (err);
    }

  اگر (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, false, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_reopen_dev_null (err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (! process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  ret = libcrun_container_setgroups (container, process, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = maybe_chown_std_streams (container_uid, container_gid, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (process->capabilities)
    capabilities = process->capabilities;
  وگرنه اگر (container->container_def->process)
    capabilities = container->container_def->process->capabilities;

  ret = libcrun_set_caps (capabilities, container_uid, container_gid, process->no_new_privileges,
                          custom_handler != پوچ, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (UNLIKELY (exec_path == پوچ))
    {
      ret = find_executable (&exec_path, process->args[0], process->cwd, err);
      اگر (UNLIKELY (ret < 0))
        {
          اگر (custom_handler == پوچ || is_empty_string (process->args[0]))
            بازگشت ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          exec_path = xstrdup (process->args[0]);
        }
    }

  اگر (! chdir_done)
    {
      ret = libcrun_safe_chdir (cwd, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (process->user)
    umask (process->user->umask_present ? process->user->umask : 0022);

  ret = 0;
  TEMP_FAILURE_RETRY (اكتب (*pipefd1, &ret, اندازه (ret)));
  TEMP_FAILURE_RETRY (close (*pipefd1));
  *pipefd1 = -1;

  اگر (custom_handler)
    {
      اگر (custom_handler->vtable->exec_func == پوچ)
        بازگشت crun_make_error (err, 0, "the handler does not support exec");

      اگر (context->argv)
        erase_initial_environment (context->argv, context->argc);

      ret = custom_handler->vtable->exec_func (custom_handler->cookie,
                                               container,
                                               exec_path,
                                               process->args);
      اگر (UNLIKELY (ret < 0))
        بازگشت crun_make_error (err, -ret, "exec container process failed with handler as `%s`", custom_handler->vtable->name);

      _safe_exit (EXIT_FAILURE);
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation, because the seccomp filter is already in place and it could
     stop some syscalls used by mark_or_close_fds_ge_than.
  */
  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, true, err);
  اگر (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, process->args));
  libcrun_fail_with_error (errno, "exec `%s`", exec_path);
}

/* Close the file descriptors inherited from the caller, other than those
   to be passed to the container process.  */
صحیح
libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err)
{
  بازگشت mark_or_close_fds_ge_than (پوچ, context->preserve_fds + 3, true, err);
}

صحیح
libcrun_load_process_from_file (ثابت نویسه *path, runtime_spec_schema_config_schema_process **process,
                                libcrun_error_t *err)
{
  ساختار parser_context ctx = { 0, stderr };
  cleanup_free نویسه *content = پوچ;
  parser_error parser_err = پوچ;
  json_object *doc = پوچ;
  size_t len;
  صحیح ret;

  ret = read_all_file (path, &content, &len, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  *process = make_runtime_spec_schema_config_schema_process (doc, &ctx, &parser_err);
  اگر (UNLIKELY (*process == پوچ))
    {
      ret = crun_make_error (err, 0, "cannot parse process file: `%s`", parser_err);
      آزاد (parser_err);
      اگر (doc)
        json_object_put (doc);
      بازگشت ret;
    }

  آزاد (parser_err);
  اگر (doc)
    json_object_put (doc);

  بازگشت 0;
}

صحیح
libcrun_container_exec_with_options (libcrun_context_t *context, ثابت نویسه *id,
                                     ساختار libcrun_container_exec_options_s *opts,
                                     libcrun_error_t *err)
{
  cleanup_custom_handler_instance ساختار custom_handler_instance_s *custom_handler = پوچ;
  صحیح container_status, ret;
  bool container_paused = false;
  pid_t pid;
  libcrun_container_status_t status = {};
  ثابت نویسه *state_ریشه = context->state_ریشه;
  cleanup_close صحیح terminal_fd = -1;
  cleanup_close صحیح seccomp_fd = -1;
  cleanup_terminal تهی *orig_terminal = پوچ;
  cleanup_container libcrun_container_t *container = پوچ;
  صحیح container_ret_status[2];
  cleanup_close صحیح pipefd0 = -1;
  cleanup_close صحیح pipefd1 = -1;
  cleanup_close صحیح seccomp_receiver_fd = -1;
  cleanup_close صحیح own_seccomp_receiver_fd = -1;
  cleanup_close صحیح seccomp_notify_fd = -1;
  ثابت نویسه *seccomp_notify_plugins = پوچ;
  __attribute__ ((unused)) cleanup_process_schema runtime_spec_schema_config_schema_process *process_cleanup = پوچ;
  runtime_spec_schema_config_schema_process *process = opts->process;
  ساختار libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  صحیح ret_from_child = 0;

  ret = libcrun_read_container_status (&status, state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_is_container_running (&status, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;
  container_status = ret;

  ret = read_container_config_from_state (&container, state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  container->context = context;

  اگر (container_status == 0)
    بازگشت crun_make_error (err, 0, "the container `%s` is not running", id);

  {
    cleanup_cgroup_status ساختار libcrun_cgroup_status *cgroup_status = پوچ;

    cgroup_status = libcrun_cgroup_make_status (&status);

    ret = libcrun_cgroup_is_container_paused (cgroup_status, &container_paused, err);
    اگر (UNLIKELY (ret < 0))
      بازگشت ret;
  }

  اگر (UNLIKELY (container_paused))
    بازگشت crun_make_error (err, 0, "cannot exec in a paused container `%s`", id);

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = block_signals (err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  libcrun_seccomp_gen_ctx_init (&seccomp_gen_ctx, container, false, 0);

  ret = libcrun_open_seccomp_bpf (&seccomp_gen_ctx, &seccomp_fd, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &seccomp_receiver_fd,
                                     &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  اگر (اندازه (*opts) != opts->struct_size)
    بازگشت crun_make_error (err, EINVAL, "invalid libcrun_container_exec_options_s struct");

  اگر (opts->path)
    {
      اگر (process)
        بازگشت crun_make_error (err, EINVAL, "cannot specify both exec file and options");

      ret = libcrun_load_process_from_file (opts->path, &process, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      process_cleanup = process;
    }

  /* This must be done before we enter a user namespace.  If the process does
     not specify any rlimits, use the ones from the container configuration.  */
  اگر (process->rlimits_len == 0 && container->container_def->process)
    ret = libcrun_set_rlimits (container->container_def->process->rlimits,
                               container->container_def->process->rlimits_len, err);
  وگرنه
    ret = libcrun_set_rlimits (process->rlimits, process->rlimits_len, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_set_mempolicy (container->container_def, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  /* If the new process block doesn't specify a SELinux label, AppArmor profile, user,
     I/O priority or scheduler, then use the configuration from the original config file.  */
  اگر (container->container_def->process)
    {
      اگر (process->io_priority == پوچ && container->container_def->process->io_priority)
        {
          process->io_priority = clone_runtime_spec_schema_config_schema_process_io_priority (container->container_def->process->io_priority);
          اگر (process->io_priority == پوچ)
            OOM ();
        }

      اگر (process->scheduler == پوچ && container->container_def->process->scheduler)
        {
          process->scheduler = clone_runtime_spec_schema_config_schema_process_scheduler (container->container_def->process->scheduler);
          اگر (process->scheduler == پوچ)
            OOM ();
        }

      اگر (process->selinux_label == پوچ && container->container_def->process->selinux_label)
        process->selinux_label = xstrdup (container->container_def->process->selinux_label);

      اگر (process->apparmor_profile == پوچ && container->container_def->process->apparmor_profile)
        process->apparmor_profile = xstrdup (container->container_def->process->apparmor_profile);

      اگر (process->user == پوچ && container->container_def->process->user)
        {
          process->user = clone_runtime_spec_schema_config_schema_process_user (container->container_def->process->user);
          اگر (process->user == پوچ)
            OOM ();
        }
    }

  ret = initialize_security (container, process, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  /* Do not leak the session keyring of the caller into the container.  The
     keyring created for the container cannot be joined, so use an anonymous one.  */
  اگر (! context->no_new_keyring && ! status.no_new_keyring)
    {
      libcrun_debug ("Creating new keyring for the exec process");

      ret = libcrun_create_keyring (container, پوچ, process->selinux_label, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "prctl unset dumpable");

  pid = libcrun_join_process (context, container, status.pid, &status, opts->cgroup, context->detach,
                              process, process->terminal ? &terminal_fd : پوچ, err);
  اگر (UNLIKELY (pid < 0))
    بازگشت pid;

  /* Process to exec.  */
  اگر (pid == 0)
    {
      TEMP_FAILURE_RETRY (close (pipefd0));
      pipefd0 = -1;

      exec_process_entrypoint (context, container, process, &pipefd1, seccomp_fd, seccomp_receiver_fd, custom_handler, opts->merge_env, err);
      /* It gets here only on errors.  */
      اگر (*err)
        {
          اگر (pipefd1 < 0)
            libcrun_fail_with_error ((*err)->status, "%s", (*err)->پیام);
          وگرنه
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

  TEMP_FAILURE_RETRY (اكتب (pipefd0, &pid, اندازه (pid)));

  اگر (seccomp_fd >= 0)
    close_and_reset (&seccomp_fd);

  اگر (terminal_fd >= 0)
    {
      unsigned short rows = 0, cols = 0;

      اگر (process->console_size)
        {
          cols = process->console_size->width;
          rows = process->console_size->height;
        }

      ret = libcrun_terminal_setup_size (terminal_fd, rows, cols, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      اگر (context->console_socket)
        {
          صحیح ret;
          cleanup_close صحیح console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
          اگر (UNLIKELY (console_socket_fd < 0))
            بازگشت console_socket_fd;
          ret = send_fd_to_socket (console_socket_fd, terminal_fd, err);
          اگر (UNLIKELY (ret < 0))
            بازگشت ret;
          close_and_reset (&terminal_fd);
        }
      وگرنه
        {
          ret = libcrun_clear_onlcr (terminal_fd, err);
          اگر (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              بازگشت ret;
            }

          ret = libcrun_set_raw (0, &orig_terminal, err);
          اگر (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              بازگشت ret;
            }
        }
    }

  ret = TEMP_FAILURE_RETRY (اقرأ (pipefd0, &ret_from_child, اندازه (ret_from_child)));
  اگر (ret != اندازه (ret_from_child))
    ret = crun_make_error (err, 0, "read pipe failed");
  وگرنه اگر (ret_from_child != 0)
    {
      cleanup_free نویسه *پیام = پوچ;
      size_t len = 0;

      ret = read_all_fd (pipefd0, "error stream", &پیام, &len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
      /* the string from read_all_fd is always NUL terminated.  */
      ret = crun_make_error (err, ret_from_child, "%s", پیام);
    }
  وگرنه
    {
      /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
      اگر (own_seccomp_receiver_fd >= 0)
        {
          seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
          اگر (UNLIKELY (seccomp_notify_fd < 0))
            بازگشت seccomp_notify_fd;

          ret = close_and_reset (&own_seccomp_receiver_fd);
          اگر (UNLIKELY (ret < 0))
            بازگشت crun_make_error (err, errno, "close seccomp receiver fd failed");
        }

      {
        ساختار wait_for_process_args args = {
          .pid = pid,
          .context = context,
          .terminal_fd = terminal_fd,
          .notify_socket = -1,
          .container_ready_fd = پوچ,
          .seccomp_notify_fd = seccomp_notify_fd,
          .seccomp_notify_plugins = seccomp_notify_plugins,
        };

        ret = wait_for_process (&args, err);
      }
    }

  flush_fd_to_err (context, terminal_fd);
  بازگشت ret;
}

صحیح
libcrun_container_update (libcrun_context_t *context, ثابت نویسه *id, ثابت نویسه *content, size_t len arg_unused,
                          libcrun_error_t *err)
{
  cleanup_custom_handler_instance ساختار custom_handler_instance_s *custom_handler = پوچ;
  runtime_spec_schema_config_linux_resources *resources = پوچ;
  cleanup_container libcrun_container_t *container = پوچ;
  ثابت نویسه *state_ریشه = context->state_ریشه;
  ساختار parser_context ctx = { 0, stderr };
  libcrun_container_status_t status = {};
  parser_error parser_err = پوچ;
  json_object *doc = پوچ;
  صحیح ret;

  ret = libcrun_read_container_status (&status, state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = read_container_config_from_state (&container, state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  resources = make_runtime_spec_schema_config_linux_resources (doc, &ctx, &parser_err);
  اگر (UNLIKELY (resources == پوچ))
    {
      ret = crun_make_error (err, 0, "cannot parse resources: %s", parser_err);
      برو cleanup;
    }

  اگر (custom_handler && custom_handler->vtable->modify_oci_configuration)
    {
      /* Adapt RESOURCES to be used from the modify_oci_configuration hook.  */
      cleanup_free runtime_spec_schema_config_linux *linux = xmalloc0 (اندازه (*linux));
      cleanup_free runtime_spec_schema_config_schema *def = xmalloc0 (اندازه (*def));

      def->linux = linux;
      linux->resources = resources;

      ret = custom_handler->vtable->modify_oci_configuration (custom_handler->cookie,
                                                              context,
                                                              def,
                                                              err);
      اگر (UNLIKELY (ret < 0))
        برو cleanup;
    }

  ret = libcrun_linux_container_update (&status, state_ریشه, resources, err);

cleanup:
  اگر (doc)
    json_object_put (doc);
  آزاد (parser_err);
  اگر (resources)
    free_runtime_spec_schema_config_linux_resources (resources);

  بازگشت ret;
}

صحیح
libcrun_container_update_from_file (libcrun_context_t *context, ثابت نویسه *id, ثابت نویسه *file, libcrun_error_t *err)
{
  cleanup_free نویسه *content = پوچ;
  size_t len;
  صحیح ret;

  ret = read_all_file (file, &content, &len, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت libcrun_container_update (context, id, content, len, err);
}

ایستا صحیح
compare_update_مقادیر (ثابت تهی *a, ثابت تهی *b)
{
  ثابت ساختار libcrun_update_مقدار_s *aa = a;
  ثابت ساختار libcrun_update_مقدار_s *bb = b;
  صحیح ret;

  ret = strcmp (aa->section, bb->section);
  اگر (ret)
    بازگشت ret;
  بازگشت strcmp (aa->name, bb->name);
}

صحیح
libcrun_container_update_from_مقادیر (libcrun_context_t *context, ثابت نویسه *id,
                                      ساختار libcrun_update_مقدار_s *مقادیر, size_t len,
                                      libcrun_error_t *err)
{
  ثابت نویسه *current_section = پوچ;
  ثابت نویسه *buf;
  json_gen_ctx *gen = پوچ;
  size_t i, buf_len;
  صحیح ret;

  اگر (! json_gen_init (&gen, پوچ))
    بازگشت crun_make_error (err, 0, "json_gen_init failed");
  json_gen_map_open (gen);

  qsort (مقادیر, len, اندازه (ساختار libcrun_update_مقدار_s), compare_update_مقادیر);

  برای (i = 0; i < len; i++)
    {
      اگر (current_section == پوچ || strcmp (مقادیر[i].section, current_section))
        {
          اگر (i > 0)
            json_gen_map_close (gen);

          current_section = مقادیر[i].section;
          json_gen_string (gen, current_section, strlen (current_section));
          json_gen_map_open (gen);
        }

      json_gen_string (gen, مقادیر[i].name, strlen (مقادیر[i].name));

      اگر (مقادیر[i].numeric)
        json_gen_عدد (gen, مقادیر[i].مقدار, strlen (مقادیر[i].مقدار));
      وگرنه
        json_gen_string (gen, مقادیر[i].مقدار, strlen (مقادیر[i].مقدار));
    }
  اگر (len)
    json_gen_map_close (gen);

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);

  ret = libcrun_container_update (context, id, buf, buf_len, err);

  json_gen_free (gen);

  بازگشت ret;
}

ایستا تهی
populate_array_field (نویسه ***field, نویسه *array[], size_t رقم_elements)
{
  size_t i;

  *field = xmalloc0 ((رقم_elements + 1) * اندازه (نویسه *));
  برای (i = 0; i < رقم_elements; i++)
    (*field)[i] = xstrdup (array[i]);

  (*field)[i] = پوچ;
}

#ifdef HAVE_CAP
ایستا تهی
populate_capabilities (ساختار features_info_s *info, نویسه ***capabilities, size_t *رقم_capabilities)
{
  size_t index = 0;
  cap_مقدار_t i;
  نویسه *endptr;
  صحیح j;

  *رقم_capabilities = 0;
  برای (i = 0;; i++)
    {
      نویسه *v = cap_to_name (i);
      اگر (v == پوچ)
        بشکن;
      strtol (v, &endptr, 10);
      اگر (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          بشکن;
        }
      (*رقم_capabilities)++;
    }

  *capabilities = xmalloc0 ((*رقم_capabilities + 1) * اندازه (ثابت نویسه *));
  برای (i = 0; i < (cap_مقدار_t) *رقم_capabilities; i++)
    {
      نویسه *v = cap_to_name (i);
      اگر (v == پوچ)
        بشکن;
      strtol (v, &endptr, 10);
      اگر (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          بشکن;
        }

      // Convert capability name to uppercase
      برای (j = 0; v[j] != '\0'; j++)
        v[j] = toupper (v[j]);

      (*capabilities)[index] = v;
      index++;
    }

  (*capabilities)[index] = پوچ; // Terminate the array with NULL
  populate_array_field (&(info->linux.capabilities), *capabilities, *رقم_capabilities);
}
#endif

ایستا تهی
retrieve_mount_options (ساختار features_info_s **info)
{
  cleanup_free ثابت ساختار propagation_flags_s *mount_options_list = پوچ;
  size_t رقم_mount_options = 0;

  // Retrieve mount options from wordlist
  mount_options_list = get_mount_flags_from_wordlist ();

  // Calculate the number of mount options
  وقتی (mount_options_list[رقم_mount_options].name != پوچ)
    رقم_mount_options++;

  // Allocate memory for mount options in info struct
  (*info)->mount_options = xmalloc0 ((رقم_mount_options + 1) * اندازه (نویسه *));

  // Copy mount options to info struct
  برای (size_t i = 0; i < رقم_mount_options; i++)
    (*info)->mount_options[i] = xstrdup (mount_options_list[i].name);
}

صحیح
libcrun_container_get_features (libcrun_context_t *context, ساختار features_info_s **info, libcrun_error_t *err arg_unused)
{
  // Allocate memory for the features_info_s structure
  size_t رقم_namspaces = اندازه (namespaces) / اندازه (namespaces[0]);
  size_t رقم_operators = اندازه (operators) / اندازه (operators[0]);
  size_t رقم_actions = اندازه (actions) / اندازه (actions[0]);
  size_t رقم_hooks = اندازه (hooks) / اندازه (hooks[0]);
  size_t رقم_archs = اندازه (archs) / اندازه (archs[0]);
  size_t رقم_unsafe_annotations = اندازه (potentially_unsafe_annotations) / اندازه (potentially_unsafe_annotations[0]);
  cleanup_free نویسه **capabilities = پوچ;
  size_t رقم_capabilities = 0;
  size_t رقم_mempolicy_modes = اندازه (mempolicy_modes) / اندازه (mempolicy_modes[0]);
  size_t رقم_mempolicy_flags = اندازه (mempolicy_flags) / اندازه (mempolicy_flags[0]);

  *info = xmalloc0 (اندازه (ساختار features_info_s));

  // Hardcoded feature information
  (*info)->oci_version_min = xstrdup ("1.0.0");
  (*info)->oci_version_max = xstrdup ("1.1.0+dev");

  // Populate hooks
  populate_array_field (&((*info)->hooks), hooks, رقم_hooks);

  // Populate mount_options
  retrieve_mount_options (info);

  // Populate namespaces
  populate_array_field (&((*info)->linux.namespaces), namespaces, رقم_namspaces);

#ifdef HAVE_CAP
  // Populate capabilities
  populate_capabilities (*info, &capabilities, &رقم_capabilities);
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
  populate_array_field (&((*info)->linux.seccomp.actions), actions, رقم_actions);
  // Populate operators
  populate_array_field (&((*info)->linux.seccomp.operators), operators, رقم_operators);
  // Populate archs
  populate_array_field (&((*info)->linux.seccomp.archs), archs, رقم_archs);
#وگرنه
  (*info)->linux.seccomp.enabled = false;
#endif

  // Put values for apparmor and selinux
  (*info)->linux.apparmor.enabled = true;
  (*info)->linux.selinux.enabled = true;

  (*info)->linux.intel_rdt.enabled = true;

  (*info)->linux.net_devices.enabled = true;

  populate_array_field (&((*info)->linux.memory_policy.mode), mempolicy_modes, رقم_mempolicy_modes);
  populate_array_field (&((*info)->linux.memory_policy.flags), mempolicy_flags, رقم_mempolicy_flags);

  // Put the values for mount extensions
  (*info)->linux.mount_ext.idmap.enabled = true;

  // Populate the values for annotations
#ifdef HAVE_SECCOMP
  {
    ثابت ساختار scmp_version *version = seccomp_version ();
    نویسه *version_string = پوچ;

    xasprintf (&version_string, "%u.%u.%u", version->major, version->minor, version->micro);
    (*info)->annotations.io_github_seccomp_libseccomp_version = version_string;
  }
#endif

  اگر (context->handler_manager && handler_by_name (context->handler_manager, "wasm"))
    (*info)->annotations.run_oci_crun_wasm = true;

#اگر HAVE_CRIU
  (*info)->annotations.run_oci_crun_checkpoint_enabled = true;
#endif
  (*info)->annotations.run_oci_crun_commit = GIT_VERSION;
  (*info)->annotations.run_oci_crun_version = PACKAGE_VERSION;

  populate_array_field (&((*info)->potentially_unsafe_annotations), potentially_unsafe_annotations, رقم_unsafe_annotations);

  بازگشت 0;
}

ایستا تهی
features_add_string (json_gen_ctx *json_gen, ثابت نویسه *کلید, ثابت نویسه *مقدار)
{
  json_gen_string (json_gen, کلید, strlen (کلید));
  json_gen_string (json_gen, مقدار, strlen (مقدار));
}

ایستا تهی
features_add_bool (json_gen_ctx *json_gen, ثابت نویسه *کلید, صحیح مقدار)
{
  json_gen_string (json_gen, کلید, strlen (کلید));
  json_gen_bool (json_gen, مقدار);
}

ایستا تهی
features_add_bool_str (json_gen_ctx *json_gen, ثابت نویسه *کلید, صحیح مقدار)
{
  features_add_string (json_gen, کلید, مقدار ? "true" : "false");
}

ایستا تهی
features_add_array (json_gen_ctx *json_gen, ثابت نویسه *کلید, نویسه **array)
{
  size_t i;

  json_gen_string (json_gen, کلید, strlen (کلید));
  json_gen_array_open (json_gen);
  اگر (array)
    برای (i = 0; array[i] != پوچ; i++)
      json_gen_string (json_gen, array[i], strlen (array[i]));
  json_gen_array_close (json_gen);
}

ایستا تهی
features_add_linux_info (json_gen_ctx *json_gen, ثابت ساختار linux_info_s *linux)
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
  اگر (linux->seccomp.actions)
    features_add_array (json_gen, "actions", linux->seccomp.actions);
  اگر (linux->seccomp.operators)
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
  اگر (linux->memory_policy.mode)
    features_add_array (json_gen, "modes", linux->memory_policy.mode);
  اگر (linux->memory_policy.flags)
    features_add_array (json_gen, "flags", linux->memory_policy.flags);
  json_gen_map_close (json_gen);

  json_gen_map_close (json_gen);
}

ایستا تهی
features_add_annotations_info (json_gen_ctx *json_gen, ثابت ساختار annotations_info_s *annotation)
{
  json_gen_string (json_gen, "annotations", strlen ("annotations"));
  json_gen_map_open (json_gen);

  اگر (! is_empty_string (annotation->io_github_seccomp_libseccomp_version))
    features_add_string (json_gen, "io.github.seccomp.libseccomp.version", annotation->io_github_seccomp_libseccomp_version);

  features_add_bool_str (json_gen, "org.opencontainers.runc.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);
  features_add_bool_str (json_gen, "run.oci.crun.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);

  features_add_string (json_gen, "run.oci.crun.commit", annotation->run_oci_crun_commit);
  features_add_string (json_gen, "run.oci.crun.version", annotation->run_oci_crun_version);

  features_add_bool_str (json_gen, "run.oci.crun.wasm", annotation->run_oci_crun_wasm);

  json_gen_map_close (json_gen);
}

صحیح
libcrun_container_get_features_json (libcrun_context_t *context, نویسه **out, libcrun_error_t *err)
{
  cleanup_struct_features ساختار features_info_s *info = پوچ;
  ثابت نویسه *buffer = پوچ;
  json_gen_ctx *json_gen;
  size_t buffer_len;
  صحیح ret;

  *out = پوچ;

  ret = libcrun_container_get_features (context, &info, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (! json_gen_init (&json_gen, پوچ))
    بازگشت crun_make_error (err, 0, "cannot initialize JSON generator");

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

  بازگشت 0;
}

/* Read the container status and fail if the container is not running.  */
ایستا صحیح
read_status_require_running (libcrun_context_t *context, ثابت نویسه *id,
                             libcrun_container_status_t *status, libcrun_error_t *err)
{
  صحیح ret;

  ret = libcrun_read_container_status (status, context->state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_is_container_running (status, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;
  اگر (ret == 0)
    بازگشت crun_make_error (err, 0, "the container `%s` is not running", id);

  بازگشت 0;
}

صحیح
libcrun_container_pause (libcrun_context_t *context, ثابت نویسه *id, libcrun_error_t *err)
{
  صحیح ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت libcrun_container_pause_linux (&status, err);
}

صحیح
libcrun_container_unpause (libcrun_context_t *context, ثابت نویسه *id, libcrun_error_t *err)
{
  صحیح ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت libcrun_container_unpause_linux (&status, err);
}

/* Translate the public options struct into the internal representation used by
   the linux/CRIU layer.  Strings are borrowed (not copied).  */
ایستا تهی
cr_options_from_public (libcrun_checkpoint_restore_t *dst, ساختار libcrun_checkpoint_restore_options_s *src)
{
  memset (dst, 0, اندازه (*dst));
  dst->image_path = (نویسه *) src->image_path;
  dst->work_path = (نویسه *) src->work_path;
  dst->parent_path = (نویسه *) src->parent_path;
  dst->console_socket = src->console_socket;
  dst->lsm_profile = (نویسه *) src->lsm_profile;
  dst->lsm_mount_context = (نویسه *) src->lsm_mount_context;
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

صحیح
libcrun_container_checkpoint (libcrun_context_t *context, ثابت نویسه *id,
                              ساختار libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  صحیح ret;
  ثابت نویسه *state_ریشه = context->state_ریشه;
  libcrun_container_status_t status = {};
  libcrun_checkpoint_restore_t cr;
  cleanup_container libcrun_container_t *container = پوچ;

  cr_options_from_public (&cr, cr_options);

  ret = read_status_require_running (context, id, &status, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = read_container_config_from_state (&container, state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  /* Needed to use the same paths (e.g. the shared empty directory used for
     masked paths) as the container runtime and restore do.  */
  container->context = context;

  ret = libcrun_container_checkpoint_linux (&status, container, &cr, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (! (cr_options->leave_running || cr_options->pre_dump))
    بازگشت container_delete_internal (context, پوچ, پوچ, id, true, true, err);

  بازگشت 0;
}

/* The dummy process only keeps the cgroup populated until the container is restored there.
   Its cgroup is below the container cgroup, so once the container runs it can freeze or kill
   the dummy process: never depend on it after that point.  */
ایستا تهی
restore_proxy_process (صحیح proxy_pid_pipe)
{
  نویسه c;

  /* Wait to be killed by the parent, or for the parent to go away.  */
  TEMP_FAILURE_RETRY (اقرأ (proxy_pid_pipe, &c, اندازه (c)));
}

ایستا صحیح
restore_terminate_proxy_process (pid_t proxy_pid, ثابت نویسه *proxy_cgroup, pid_t container_pid, صحیح cgroup_manager, libcrun_error_t *err)
{
  صحیح mode;
  صحیح ret;

  اگر (cgroup_manager == CGROUP_MANAGER_SYSTEMD)
    {
      نویسه ready_str[64];

      ret = snprintf (ready_str, اندازه (ready_str), "MAINPID=%d", container_pid);
      اگر (UNLIKELY (ret >= (صحیح) اندازه (ready_str)))
        بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

      ret = send_sd_notify (ready_str, err);
      /* Do not fail on errors.  */
      اگر (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  mode = libcrun_get_cgroup_mode (err);
  اگر (UNLIKELY (mode < 0))
    بازگشت mode;

  /* SIGKILL terminates a process frozen with cgroup v2, while the cgroup v1 freezer does not
     deliver it until the cgroup is thawed, so do not wait for it there.  */
  kill (proxy_pid, SIGKILL);
  TEMP_FAILURE_RETRY (waitpid (proxy_pid, پوچ, mode == CGROUP_MODE_UNIFIED ? 0 : WNOHANG));

  بازگشت destroy_cgroup_path (proxy_cgroup, mode, err);
}

صحیح
libcrun_container_restore (libcrun_context_t *context, ثابت نویسه *id arg_unused,
                           ساختار libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  cleanup_cgroup_status ساختار libcrun_cgroup_status *cgroup_status = پوچ;
  cleanup_container libcrun_container_t *container = پوچ;
  cleanup_close صحیح proxy_pid_pipe0 = -1;
  cleanup_close صحیح proxy_pid_pipe1 = -1;
  runtime_spec_schema_config_schema *def;
  libcrun_container_status_t status = {};
  ساختار libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  libcrun_checkpoint_restore_t cr;
  cleanup_pid pid_t proxy_pid = -1;
  cleanup_close صحیح seccomp_fd = -1;
  ثابت نویسه *seccomp_bpf_data;
  صحیح proxy_pid_pipe[2];
  صحیح cgroup_manager;
  uid_t ریشه_uid = -1;
  gid_t ریشه_gid = -1;
  صحیح ret;

  cr_options_from_public (&cr, cr_options);

  container = libcrun_container_load_from_file ("config.json", err);
  اگر (container == پوچ)
    بازگشت -1;

  container->context = context;
  def = container->container_def;

  ret = check_config_file (def, context, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_status_check_directories (context->state_ریشه, context->id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_copy_config_file (context->id, context->state_ریشه, container, err);
  اگر (UNLIKELY (ret < 0))
    برو fail;

  /* CRIU restores the seccomp filters of the checkpointed processes, but
     exec loads the filter from the seccomp.bpf file in the state directory,
     so it must be generated again for the restored container.  */
  seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  اگر (UNLIKELY (ret < 0))
    برو fail;

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  اگر (UNLIKELY (ret < 0))
    برو fail;
  close_and_reset (&seccomp_fd);

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  اگر (context->systemd_cgroup)
    cgroup_manager = CGROUP_MANAGER_SYSTEMD;
  وگرنه اگر (context->force_no_cgroup)
    cgroup_manager = CGROUP_MANAGER_DISABLED;

  def = container->container_def;

  /* If we are root (either on the host or in a namespace),
   * then chown the cgroup to root in the container user namespace. */
  get_ریشه_in_the_userns (def, container->host_uid, container->host_gid, &ریشه_uid, &ریشه_gid);

  /* If the root in the container is different than the current root user, attempt to chown
     the std streams before entering the user namespace.  Otherwise we might lose access
     to the user (as it is not mapped in the user namespace) and cannot chown them.  */
  اگر (ریشه_uid > 0 || ریشه_gid > 0)
    {
      ret = maybe_chown_std_streams (ریشه_uid, ریشه_gid, err);
      اگر (UNLIKELY (ret < 0))
        برو fail;
    }

  /* CRIU runs as a child process, so the restored processes would inherit the session keyring.  */
  ret = setup_container_keyring (container, context, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  {
    cleanup_close صحیح cgroup_dirfd = -1;
    bool needs_proxy_process;
    cleanup_cgroup_preenter ساختار libcrun_cgroup_args cg = {
      .resources = def->linux ? def->linux->resources : پوچ,
      .annotations = container->annotations,
      .cgroup_path = def->linux ? def->linux->cgroups_path : "",
      .manager = cgroup_manager,
      .ریشه_uid = ریشه_uid,
      .ریشه_gid = ریشه_gid,
      .id = context->id,
      .state_ریشه = context->state_ریشه,
      .container = container,
    };

    /* The CRIU restore code uses bundle, rootfs and cgroup_path of status.   The cgroup_path is set later.  */
    status.bundle = (نویسه *) context->bundle;
    status.rootfs = def->ریشه->path;

    ret = libcrun_cgroup_preenter (&cg, &cgroup_dirfd, err);
    اگر (UNLIKELY (ret < 0))
      برو fail;

    needs_proxy_process = cgroup_dirfd < 0;

    /* If the target cgroup was already created (with cgroupfs), we can restore the container directly there.  */
    اگر (! needs_proxy_process)
      {
        cleanup_free نویسه *cgroup_path = پوچ;

        ret = get_cgroup_dirfd_path (cgroup_dirfd, &cgroup_path, err);
        اگر (UNLIKELY (ret < 0))
          برو fail;

        /* Complete the configuration for the cgroup before the container is
           restored there: CRIU resumes the restored processes before it
           returns, so the device filter and the limits must already be in
           place.  There is no process to move, as CRIU is run in the cgroup,
           and the controllers were enabled by precreate_cgroup.  */
        cg.joined = true;

        ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
        اگر (UNLIKELY (ret < 0))
          برو fail;

        /* Restore the container directly in the desired cgroup.  */
        status.cgroup_path = cgroup_path;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        اگر (UNLIKELY (ret < 0))
          برو fail;

        /* Now that the container is restored, keep any signal for it, so that
           it is forwarded to the init by wait_for_process, rather than acted
           upon by crun.  This cannot be done earlier, as CRIU, which is run as
           a child process, would inherit the blocked signals.  */
        ret = block_signals (err);
        اگر (UNLIKELY (ret < 0))
          برو fail;

        /* Use the container first process PID to finalize the cgroup.  */
        cg.pid = status.pid;
      }
    وگرنه
      {
        /* When using systemd, we need a process first that must exist before a cgroup is created.
           Create a dummy process that sits and waits until the container is restored.  */
        ret = pipe2 (proxy_pid_pipe, O_CLOEXEC);
        اگر (UNLIKELY (ret < 0))
          {
            ret = crun_make_error (err, errno, "pipe");
            برو fail;
          }

        /* Only used for auto cleanup.  */
        proxy_pid_pipe0 = proxy_pid_pipe[0];
        proxy_pid_pipe1 = proxy_pid_pipe[1];

        proxy_pid = fork ();
        اگر (UNLIKELY (proxy_pid < 0))
          {
            ret = crun_make_error (err, errno, "fork");
            برو fail;
          }

        اگر (proxy_pid == 0)
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
        اگر (UNLIKELY (ret < 0))
          برو fail;
      }

    ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
    اگر (UNLIKELY (ret < 0))
      برو fail;

    /* When using a dummy process, the container is restored here once the cgroup is configured.  */
    اگر (needs_proxy_process)
      {
        cleanup_free نویسه *target_cgroup = پوچ;
        cleanup_free نویسه *proxy_cgroup = پوچ;

        ret = libcrun_get_cgroup_process (proxy_pid, &target_cgroup, false, err);
        اگر (UNLIKELY (ret < 0))
          برو fail;

        /* Move the dummy process to a sub-cgroup.  */
        ret = append_paths (&proxy_cgroup, err, target_cgroup, ".proxy-crun", پوچ);
        اگر (UNLIKELY (ret < 0))
          برو fail;

        ret = libcrun_move_process_to_cgroup (proxy_pid, 0, proxy_cgroup, true, err);
        اگر (UNLIKELY (ret < 0))
          برو fail;

        /* Restore the container in the same cgroup where the dummy process was.  */
        status.cgroup_path = target_cgroup;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        اگر (UNLIKELY (ret < 0))
          برو fail;

        /* See the comment for the other libcrun_container_restore_linux call.  */
        ret = block_signals (err);
        اگر (UNLIKELY (ret < 0))
          برو fail;

        /* The container is running, so do not fail because of the dummy process.  */
        ret = restore_terminate_proxy_process (proxy_pid, proxy_cgroup, status.pid, cgroup_manager, err);
        اگر (UNLIKELY (ret < 0))
          crun_error_release (err);
        proxy_pid = -1;

        close_and_reset (&proxy_pid_pipe1);
      }
  }

  context->detach = cr_options->detach;
  ret = write_container_status (container, context, status.pid, cgroup_status, err);
  اگر (UNLIKELY (ret < 0))
    برو fail;

  {
    /* Wait the same way run and exec do: apart from waiting for the container
       init, this writes the pid file, forwards any signal to the init, and
       reaps every child, not only the init.  */
    ساختار wait_for_process_args args = {
      .pid = status.pid,
      .context = context,
      .terminal_fd = -1,
      .notify_socket = -1,
      .container_ready_fd = پوچ,
      .seccomp_notify_fd = -1,
      .seccomp_notify_plugins = پوچ,
    };

    بازگشت wait_for_process (&args, err);
  }

fail:
  /* Once restored, the container is running.  Without a status file nothing
     can reach it anymore, so do not leave it behind.  */
  اگر (status.pid > 0)
    {
      kill (status.pid, SIGKILL);
      waitpid_ignore_stopped (status.pid, پوچ, 0);
    }
  اگر (cgroup_status)
    {
      libcrun_error_t tmp_err = پوچ;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  {
    libcrun_error_t tmp_err = پوچ;

    اگر (libcrun_container_delete_status (context->state_ریشه, context->id, &tmp_err) < 0)
      crun_error_release (&tmp_err);
  }
  بازگشت ret;
}

صحیح
libcrun_container_read_pids (libcrun_context_t *context, ثابت نویسه *id, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  cleanup_cgroup_status ساختار libcrun_cgroup_status *cgroup_status = پوچ;
  cleanup_container_status libcrun_container_status_t status = {};
  صحیح ret;

  ret = libcrun_read_container_status (&status, context->state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (status.cgroup_path == پوچ || status.cgroup_path[0] == '\0')
    بازگشت crun_make_error (err, 0, "the container is not using cgroups");

  cgroup_status = libcrun_cgroup_make_status (&status);

  بازگشت libcrun_cgroup_read_pids (cgroup_status, recurse, pids, err);
}

صحیح
libcrun_container_list_json (libcrun_context_t *context, نویسه **out, libcrun_error_t *err)
{
  libcrun_container_list_t *list = پوچ, *it;
  ثابت نویسه *content = پوچ;
  json_gen_ctx *gen = پوچ;
  size_t len;
  صحیح ret;

  *out = پوچ;

  ret = libcrun_get_containers_list (&list, context->state_ریشه, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (! json_gen_init (&gen, پوچ))
    {
      ret = crun_make_error (err, 0, "cannot allocate json generator");
      برو exit;
    }

  json_gen_config (gen, json_gen_beautify, 1);
  json_gen_array_open (gen);

  برای (it = list; it; it = it->next)
    {
      libcrun_container_status_t status;
      صحیح running = 0;
      صحیح pid;
      ثابت نویسه *container_status = پوچ;

      ret = libcrun_read_container_status (&status, context->state_ریشه, it->name, err);
      اگر (UNLIKELY (ret < 0))
        برو exit;

      pid = status.pid;

      ret = libcrun_get_container_state_string (it->name, &status, context->state_ریشه, &container_status,
                                                &running, err);
      اگر (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          ادامه;
        }

      اگر (! running)
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
  اگر (json_gen_get_buf (gen, &content, &len) != json_gen_status_ok)
    {
      ret = libcrun_make_error (err, 0, "cannot generate json list");
      برو exit;
    }

  *out = xstrdup (content);
  ret = 0;

exit:
  اگر (list)
    libcrun_free_containers_list (list);
  اگر (gen)
    json_gen_free (gen);

  بازگشت ret;
}

صحیح
libcrun_write_json_containers_list (libcrun_context_t *context, FILE *out, libcrun_error_t *err)
{
  cleanup_free نویسه *content = پوچ;
  size_t len;
  صحیح ret;

  ret = libcrun_container_list_json (context, &content, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  len = strlen (content);
  اگر (len && fwrite (content, 1, len, out) != len)
    بازگشت crun_make_error (err, errno, "error writing to file");

  بازگشت 0;
}

صحیح
libcrun_container_update_intel_rdt (libcrun_context_t *context, ثابت نویسه *id, ساختار libcrun_intel_rdt_update *update, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = پوچ;
  صحیح ret;

  ret = read_container_config_from_state (&container, context->state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت libcrun_update_intel_rdt (id, container, update->l3_cache_schema, update->mem_bw_schema, update->schemata, err);
}

ایستا صحیح
libcrun_container_add_or_remove_mounts_from_file (libcrun_context_t *context, ثابت نویسه *id, ثابت نویسه *file, bool add, libcrun_error_t *err)
{
  cleanup_custom_handler_instance ساختار custom_handler_instance_s *custom_handler = پوچ;
  cleanup_container libcrun_container_t *container = پوچ;
  cleanup_free runtime_spec_schema_defs_mount **mounts = پوچ;
  ثابت نویسه *state_ریشه = context->state_ریشه;
  cleanup_free parser_error parser_err = پوچ;
  ساختار parser_context pctx = { 0, stderr };
  libcrun_container_status_t status = {};
  cleanup_free نویسه *content = پوچ;
  json_object *doc = پوچ;
  size_t n_mounts = 0, len, i;
  صحیح ret = 1;

  ret = read_all_file (file, &content, &len, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_read_container_status (&status, state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = read_container_config_from_state (&container, state_ریشه, id, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = parse_json_file (&doc, content, &pctx, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (! json_object_is_type (doc, json_type_array))
    {
      ret = crun_make_error (err, 0, "mounts must be an array");
      برو cleanup;
    }
  وگرنه
    {
      n_mounts = json_object_array_length (doc);

      mounts = xmalloc0 ((n_mounts + 1) * اندازه (*mounts));
      برای (i = 0; i < n_mounts; i++)
        {
          mounts[i] = make_runtime_spec_schema_defs_mount (json_object_array_get_idx (doc, i), &pctx, &parser_err);
          اگر (mounts[i] == پوچ)
            {
              ret = crun_make_error (err, 0, "cannot parse mount: %s", parser_err);
              برو cleanup;
            }
        }
    }

  اگر (add)
    ret = libcrun_make_runtime_mounts (container, &status, mounts, n_mounts, err);
  وگرنه
    ret = libcrun_destroy_runtime_mounts (container, &status, mounts, n_mounts, err);

cleanup:
  اگر (doc)
    json_object_put (doc);
  اگر (mounts)
    برای (i = 0; i < n_mounts; i++)
      free_runtime_spec_schema_defs_mount (mounts[i]);
  بازگشت ret;
}

صحیح
libcrun_container_add_mounts_from_file (libcrun_context_t *context, ثابت نویسه *id, ثابت نویسه *file, libcrun_error_t *err)
{
  بازگشت libcrun_container_add_or_remove_mounts_from_file (context, id, file, true, err);
}

صحیح
libcrun_container_remove_mounts_from_file (libcrun_context_t *context, ثابت نویسه *id, ثابت نویسه *file, libcrun_error_t *err)
{
  بازگشت libcrun_container_add_or_remove_mounts_from_file (context, id, file, false, err);
}
