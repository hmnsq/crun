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

#ضمن <config.h>
#ضمن <ocispec/runtime_spec_schema_config_schema.h>
#ضمن <stdbool.h>
#ضمن "container.h"
#ضمن "utils.h"
#ضمن "json_gen_utils.h"
#ضمن "seccomp.h"
#ضمن "mempolicy.h"
#ifdef HAVE_SECCOMP
#  include <seccomp.h>
#endif
#ضمن "scheduler.h"
#ضمن "seccomp_notify.h"
#ضمن "custom-handler.h"
#ضمن <stdbool.h>
#ضمن <argp.h>
#ضمن <unistd.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#ضمن <sched.h>
#ضمن <sys/wait.h>
#ضمن <string.h>
#ضمن <fcntl.h>
#ضمن "status.h"
#ضمن "mount_flags.h"
#ضمن "linux.h"
#ضمن "terminal.h"
#ضمن "io_priority.h"
#ضمن "cgroup.h"
#ضمن "cgroup-utils.h"
#ضمن <sys/prctl.h>
#ضمن <sys/auxv.h>
#ضمن <sys/signalfd.h>
#ضمن <sys/epoll.h>
#ضمن <sys/socket.h>
#ifdef HAVE_CAP
#  include <sys/capability.h>
#endif
#ضمن <sys/ioctl.h>
#ضمن <termios.h>
#ضمن <grp.h>
#ضمن <git-version.h>

#ifdef HAVE_SYSTEMD
#  include <systemd/sd-daemon.h>
#endif

#ضمن <json-c/json.h>
#ضمن <ocispec/json_common.h>

تعداد
{
  SYNC_SOCKET_SYNC_MESSAGE,
  SYNC_SOCKET_ERROR_MESSAGE,
  SYNC_SOCKET_WARNING_MESSAGE,
  SYNC_SOCKET_DEBUG_MESSAGE,
};

بنية container_entrypoint_s
{
  libcrun_container_t *container;
  libcrun_context_t *context;
  صحيح has_terminal_socket_pair;
  صحيح terminal_socketpair[2];

  /* Used by log_write_to_sync_socket.  */
  صحيح sync_socket;

  صحيح seccomp_fd;
  صحيح seccomp_receiver_fd;
  صحيح console_socket_fd;

  صحيح hooks_out_fd;
  صحيح hooks_err_fd;

  /* Absolute bundle path, to be reported to hooks run in the container.  */
  ثابت حرف *bundle;

  بنية custom_handler_instance_s *custom_handler;
};

بنية sync_socket_message_s
{
  صحيح type;
  صحيح error_value;
  حرف message[512];
};

تعريفنوع runtime_spec_schema_defs_hook hook;

// linux hooks
حرف *hooks[] = {
  "prestart",
  "createRuntime",
  "createContainer",
  "startContainer",
  "poststart",
  "poststop"
};

// linux namespaces
ساكن حرف *namespaces[] = {
  "cgroup",
  "ipc",
  "mount",
  "network",
  "pid",
  "user",
  "uts"
};

ساكن حرف *actions[] = {
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

ساكن حرف *operators[] = {
  "SCMP_CMP_NE",
  "SCMP_CMP_LT",
  "SCMP_CMP_LE",
  "SCMP_CMP_EQ",
  "SCMP_CMP_GE",
  "SCMP_CMP_GT",
  "SCMP_CMP_MASKED_EQ",
};

ساكن حرف *archs[] = {
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

ساكن حرف *mempolicy_modes[] = {
  "MPOL_DEFAULT",
  "MPOL_PREFERRED",
  "MPOL_BIND",
  "MPOL_INTERLEAVE",
  "MPOL_LOCAL",
  "MPOL_PREFERRED_MANY",
  "MPOL_WEIGHTED_INTERLEAVE"
};

ساكن حرف *mempolicy_flags[] = {
  "MPOL_F_NUMA_BALANCING",
  "MPOL_F_RELATIVE_NODES",
  "MPOL_F_STATIC_NODES"
};

ساكن حرف *potentially_unsafe_annotations[] = {
  "module.wasm.image/variant",
  "io.kubernetes.cri.container-type",
  "run.oci.",
  "org.criu.",
  "krun.",
};

#define SYNC_SOCKET_MESSAGE_LEN(x, l) (offsetof (بنية sync_socket_message_s, message) + l)

ساكن صحيح
sync_socket_write_msg (صحيح fd, صحيح verbosity, صحيح err_value, ثابت حرف *log_msg)
{
  صحيح ret;
  size_t err_len;
  بنية sync_socket_message_s msg;
  بدل (verbosity)
    {
    حالة LIBCRUN_VERBOSITY_DEBUG:
      msg.type = SYNC_SOCKET_DEBUG_MESSAGE;
      اكسر;
    حالة LIBCRUN_VERBOSITY_WARNING:
      msg.type = SYNC_SOCKET_WARNING_MESSAGE;
      اكسر;
    حالة LIBCRUN_VERBOSITY_ERROR:
      msg.type = SYNC_SOCKET_ERROR_MESSAGE;
      اكسر;
    }
  msg.error_value = err_value;

  אם (fd < 0)
    החזר 0;

  err_len = strlen (log_msg);
  אם (err_len >= حجم (msg.message))
    err_len = حجم (msg.message) - 1;

  memcpy (msg.message, log_msg, err_len);
  msg.message[err_len] = '\0';

  ret = TEMP_FAILURE_RETRY (اكتب (fd, &msg, SYNC_SOCKET_MESSAGE_LEN (msg, err_len + 1)));
  אם (UNLIKELY (ret < 0))
    החזר -1;

  החזר 0;
}

ساكن صحيح
sync_socket_write_error (صحيح fd, libcrun_error_t *out_err)
{
  אם (fd < 0)
    החזר 0;
  החזר sync_socket_write_msg (fd, false, (*out_err)->status, (*out_err)->msg);
}

ساكن فراغ
log_write_to_sync_socket (صحيح errno_, ثابت حرف *msg, صحيح verbosity, فراغ *arg)
{
  بنية container_entrypoint_s *entrypoint_args = arg;
  صحيح fd = entrypoint_args->sync_socket;

  אם (fd < 0)
    החזר;

  אם (sync_socket_write_msg (fd, verbosity, errno_, msg) < 0)
    log_write_to_stderr (errno_, msg, verbosity, arg);
}

ساكن bool
is_memory_limit_too_low (runtime_spec_schema_config_schema *def)
{
  ثابت long memory_limit_too_low = 1024 * 1024;

  אם (def->linux == عدم || def->linux->resources == عدم)
    החזר false;

  אם (def->linux->resources->memory
      && def->linux->resources->memory->limit_present
      && def->linux->resources->memory->limit < memory_limit_too_low)
    החזר true;

  אם (def->linux->resources->unified)
    {
      size_t i;

      עבור (i = 0; i < def->linux->resources->unified->len; i++)
        אם (strcmp (def->linux->resources->unified->keys[i], "memory.max") == 0)
          {
            long limit;

            errno = 0;
            limit = strtol (def->linux->resources->unified->values[i], عدم, 10);
            אם (errno != 0)
              החזר false;
            אם (limit < memory_limit_too_low)
              החזר true;
          }
    }

  החזר false;
}

ساكن صحيح
sync_socket_wait_sync (libcrun_context_t *context, صحيح fd, bool flush, libcrun_error_t *err)
{
  بنية sync_socket_message_s msg;

  אם (fd < 0)
    החזר 0;

  כלעוד (true)
    {
      صحيح ret;

      ret = TEMP_FAILURE_RETRY (اقرأ (fd, &msg, حجم (msg)));
      אם (UNLIKELY (ret < 0))
        {
          אם (flush)
            החזר 0;
          החזר crun_make_error (err, errno, "read from sync socket");
        }

      אם (ret == 0)
        {
          אם (flush)
            החזר 0;

          החזר crun_make_error (err, 0, "read from the init process");
        }

      אם (! flush && msg.type == SYNC_SOCKET_SYNC_MESSAGE)
        החזר 0;
      אחרת אם (msg.type == SYNC_SOCKET_DEBUG_MESSAGE)
        {
          אם (context)
            context->output_handler (msg.error_value, msg.message, LIBCRUN_VERBOSITY_DEBUG, context->output_handler_arg);
          استمر;
        }
      אחרת אם (msg.type == SYNC_SOCKET_WARNING_MESSAGE)
        {
          אם (context)
            context->output_handler (msg.error_value, msg.message, LIBCRUN_VERBOSITY_WARNING, context->output_handler_arg);
          استمر;
        }
      אחרת אם (msg.type == SYNC_SOCKET_ERROR_MESSAGE)
        החזר crun_make_error (err, msg.error_value, "%s", msg.message);
    }
}

ساكن صحيح
sync_socket_send_sync (صحيح fd, bool flush_errors, libcrun_error_t *err)
{
  صحيح ret;
  بنية sync_socket_message_s msg = {
    0,
  };
  msg.type = SYNC_SOCKET_SYNC_MESSAGE;

  אם (fd < 0)
    החזר 0;

  ret = TEMP_FAILURE_RETRY (اكتب (fd, &msg, SYNC_SOCKET_MESSAGE_LEN (msg, 0)));
  אם (UNLIKELY (ret < 0))
    {
      אם (flush_errors)
        {
          صحيح saved_errno = errno;
          ret = TEMP_FAILURE_RETRY (اقرأ (fd, &msg, حجم (msg)));
          אם (ret >= 0 && msg.type == SYNC_SOCKET_ERROR_MESSAGE)
            החזר crun_make_error (err, msg.error_value, "%s", msg.message);
          errno = saved_errno;
        }
      החזר crun_make_error (err, errno, "write to sync socket");
    }

  החזר 0;
}

ساكن bool
has_cap_sys_ptrace ()
{
#ifdef HAVE_CAP
  بنية __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  بنية __user_cap_data_struct data[2] = { { 0 } };

  אם (UNLIKELY (capget (&hdr, data) < 0))
    החזר true;

  החזר (data[0].effective & (1U << CAP_SYS_PTRACE)) != 0;
#אחרת
  החזר geteuid () == 0;
#endif
}

ساكن libcrun_container_t *
make_container (runtime_spec_schema_config_schema *container_def, ثابت حرف *path, ثابت حرف *config)
{
  libcrun_container_t *container = xmalloc0 (حجم (*container));
  container->container_def = container_def;

  container->host_uid = geteuid ();
  container->host_gid = getegid ();
  container->host_has_cap_sys_ptrace = has_cap_sys_ptrace ();
  container->proc_fd = -1;

  container->annotations = make_string_map_from_json (container_def->annotations);

  /* An empty namespace path means that a new namespace is to be created,
     the same as if it is not specified.  */
  אם (container_def->linux)
    {
      size_t i;

      עבור (i = 0; i < container_def->linux->namespaces_len; i++)
        אם (container_def->linux->namespaces[i]->path && container_def->linux->namespaces[i]->path[0] == '\0')
          {
            حرر (container_def->linux->namespaces[i]->path);
            container_def->linux->namespaces[i]->path = عدم;
          }
    }

  אם (path)
    container->config_file = xstrdup (path);
  אם (config)
    container->config_file_content = xstrdup (config);

  החזר container;
}

libcrun_container_t *
libcrun_container_load_from_memory (ثابت حرف *json, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free حرف *oci_error = عدم;
  container_def = runtime_spec_schema_config_schema_parse_data (json, عدم, &oci_error);
  אם (container_def == عدم)
    {
      crun_make_error (err, 0, "load: `%s`", oci_error);
      החזר عدم;
    }
  החזר make_container (container_def, عدم, json);
}

libcrun_container_t *
libcrun_container_load_from_file (ثابت حرف *path, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free حرف *oci_error = عدم;
  cleanup_free حرف *content = عدم;
  size_t len;
  صحيح ret;

  libcrun_debug ("Loading container from config file: `%s`", path);
  ret = read_all_file (path, &content, &len, err);
  אם (UNLIKELY (ret < 0))
    החזר عدم;

  container_def = runtime_spec_schema_config_schema_parse_data (content, عدم, &oci_error);
  אם (container_def == عدم)
    {
      crun_make_error (err, 0, "load `%s`: %s", path, oci_error);
      החזר عدم;
    }
  החזר make_container (container_def, عدم, content);
}

ثابت حرف *
libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err)
{
  بنية parser_context ctx = { 0, stderr };
  parser_error gen_err = عدم;

  אם (container == عدم)
    {
      crun_make_error (err, EINVAL, "no container specified");
      החזר عدم;
    }

  אם (container->config_file_content == عدم && container->container_def)
    {
      container->config_file_content = runtime_spec_schema_config_schema_generate_json (container->container_def, &ctx, &gen_err);
      אם (container->config_file_content == عدم)
        {
          crun_make_error (err, 0, "cannot generate the configuration JSON: %s", gen_err ?: "unknown error");
          حرر (gen_err);
          החזר عدم;
        }
      حرر (gen_err);
    }

  אם (container->config_file_content == عدم)
    {
      crun_make_error (err, 0, "the container has no configuration");
      החזר عدم;
    }

  החזר container->config_file_content;
}

ثابت حرف *
libcrun_container_get_annotation (libcrun_container_t *container, ثابت حرف *key)
{
  אם (container == عدم)
    החזר عدم;
  החזר find_string_map_value (container->annotations, key);
}

size_t
libcrun_container_get_annotations_len (libcrun_container_t *container)
{
  אם (container == عدم || container->annotations == عدم)
    החזר 0;
  החזר string_map_size (container->annotations);
}

صحيح
libcrun_container_get_annotation_at (libcrun_container_t *container, size_t index, ثابت حرف **key,
                                     ثابت حرف **value)
{
  אם (container == عدم)
    {
      errno = ERANGE;
      החזר -1;
    }
  החזר string_map_get_at (container->annotations, index, key, value);
}

uid_t
libcrun_container_get_uid (libcrun_container_t *container)
{
  אם (container == عدم)
    החזר (uid_t) -1;
  החזר container->host_uid;
}

gid_t
libcrun_container_get_gid (libcrun_container_t *container)
{
  אם (container == عدم)
    החזר (gid_t) -1;
  החזר container->host_gid;
}

فراغ
libcrun_container_free (libcrun_container_t *ctr)
{
  אם (ctr == عدم)
    החזר;

  אם (ctr->cleanup_private_data)
    ctr->cleanup_private_data (ctr->private_data);

  אם (ctr->container_def)
    free_runtime_spec_schema_config_schema (ctr->container_def);

  free_string_map (ctr->annotations);

  אם (ctr->proc_fd >= 0)
    close (ctr->proc_fd);

  حرر (ctr->config_file_content);
  حرر (ctr->config_file);
  حرر (ctr);
}

libcrun_context_t *
libcrun_context_new (ثابت حرف *id, ثابت حرف *state_root, libcrun_error_t *err)
{
  libcrun_context_t *ctx = xmalloc0 (حجم (*ctx));

  /* Go through the setters so that the owned_* copies and the borrowed
     pointers stay in sync and are released by libcrun_context_free.  */
  libcrun_context_set_id (ctx, id);
  libcrun_context_set_state_root (ctx, state_root);
  libcrun_context_set_bundle (ctx, ".");
  ctx->fifo_exec_wait_fd = -1;

  אם (libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, id, عدم, err) < 0)
    {
      libcrun_context_free (ctx);
      החזר عدم;
    }

  ctx->handler_manager = libcrun_handler_manager_create (err);
  אם (ctx->handler_manager == عدم)
    {
      libcrun_context_free (ctx);
      החזר عدم;
    }

#ifdef CRUN_LIBDIR
  אם (access (CRUN_LIBDIR "/handlers", F_OK) == 0)
    {
      אם (libcrun_handler_manager_load_directory (ctx->handler_manager, CRUN_LIBDIR "/handlers", err) < 0)
        {
          libcrun_context_free (ctx);
          החזר عدم;
        }
    }
#endif

  החזר ctx;
}

فراغ
libcrun_context_free (libcrun_context_t *ctx)
{
  אם (ctx == عدم)
    החזר;

  حرر (ctx->owned_state_root);
  حرر (ctx->owned_id);
  حرر (ctx->owned_bundle);
  حرر (ctx->owned_console_socket);
  حرر (ctx->owned_pid_file);
  حرر (ctx->owned_notify_socket);
  حرر (ctx->owned_handler);

  אם (ctx->handler_manager)
    handler_manager_free (ctx->handler_manager);

  حرر (ctx);
}

#define LIBCRUN_CONTEXT_SET_STRING(field)                  \
  עשה                                                       \
    {                                                      \
      حرر (ctx->owned_##field);                           \
      ctx->owned_##field = value ? xstrdup (value) : عدم; \
      ctx->field = ctx->owned_##field;                     \
  } כלעוד (0)

فراغ
libcrun_context_set_id (libcrun_context_t *ctx, ثابت حرف *value)
{
  LIBCRUN_CONTEXT_SET_STRING (id);
}

فراغ
libcrun_context_set_state_root (libcrun_context_t *ctx, ثابت حرف *value)
{
  LIBCRUN_CONTEXT_SET_STRING (state_root);
}

فراغ
libcrun_context_set_bundle (libcrun_context_t *ctx, ثابت حرف *value)
{
  LIBCRUN_CONTEXT_SET_STRING (bundle);
}

فراغ
libcrun_context_set_console_socket (libcrun_context_t *ctx, ثابت حرف *value)
{
  LIBCRUN_CONTEXT_SET_STRING (console_socket);
}

فراغ
libcrun_context_set_pid_file (libcrun_context_t *ctx, ثابت حرف *value)
{
  LIBCRUN_CONTEXT_SET_STRING (pid_file);
}

فراغ
libcrun_context_set_notify_socket (libcrun_context_t *ctx, ثابت حرف *value)
{
  LIBCRUN_CONTEXT_SET_STRING (notify_socket);
}

فراغ
libcrun_context_set_handler (libcrun_context_t *ctx, ثابت حرف *value)
{
  LIBCRUN_CONTEXT_SET_STRING (handler);
}

#undef LIBCRUN_CONTEXT_SET_STRING

ثابت حرف *
libcrun_context_get_pid_file (libcrun_context_t *ctx)
{
  החזר ctx->pid_file;
}

ثابت حرف *
libcrun_context_get_state_root (libcrun_context_t *ctx)
{
  אם (ctx == عدم)
    החזר عدم;
  החזר ctx->state_root;
}

فراغ
libcrun_context_set_preserve_fds (libcrun_context_t *ctx, صحيح preserve_fds)
{
  ctx->preserve_fds = preserve_fds;
}

صحيح
libcrun_context_get_preserve_fds (libcrun_context_t *ctx)
{
  החזר ctx->preserve_fds;
}

فراغ
libcrun_context_set_listen_fds (libcrun_context_t *ctx, صحيح listen_fds)
{
  ctx->listen_fds = listen_fds;
}

فراغ
libcrun_context_set_args (libcrun_context_t *ctx, صحيح argc, حرف **argv)
{
  ctx->argc = argc;
  ctx->argv = argv;
}

فراغ
libcrun_context_set_systemd_cgroup (libcrun_context_t *ctx, bool value)
{
  ctx->systemd_cgroup = value;
}

فراغ
libcrun_context_set_detach (libcrun_context_t *ctx, bool value)
{
  ctx->detach = value;
}

فراغ
libcrun_context_set_no_new_keyring (libcrun_context_t *ctx, bool value)
{
  ctx->no_new_keyring = value;
}

فراغ
libcrun_context_set_no_pivot (libcrun_context_t *ctx, bool value)
{
  ctx->no_pivot = value;
}

فراغ
libcrun_context_set_force_no_cgroup (libcrun_context_t *ctx, bool value)
{
  ctx->force_no_cgroup = value;
}

فراغ
libcrun_context_set_output_handler (libcrun_context_t *ctx, crun_output_handler handler, فراغ *arg)
{
  ctx->output_handler = handler;
  ctx->output_handler_arg = arg;
}

صحيح
libcrun_context_init_logging (libcrun_context_t *ctx, ثابت حرف *log, libcrun_error_t *err)
{
  החזר libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, ctx->id, log, err);
}

ساكن صحيح
block_signals (libcrun_error_t *err)
{
  صحيح ret;
  sigset_t mask;
  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, عدم);
  אם (UNLIKELY (ret < 0))
    החזר crun_make_error (err, errno, "sigprocmask");
  החזר 0;
}

ساكن صحيح
unblock_signals (libcrun_error_t *err)
{
  صحيح i;
  صحيح ret;
  sigset_t mask;
  بنية sigaction act = {};

  sigfillset (&mask);
  ret = sigprocmask (SIG_UNBLOCK, &mask, عدم);
  אם (UNLIKELY (ret < 0))
    החזר crun_make_error (err, errno, "sigprocmask");

  act.sa_handler = SIG_DFL;
  עבור (i = 0; i < NSIG; i++)
    {
      ret = sigaction (i, &act, عدم);
      אם (ret < 0 && errno != EINVAL)
        החזר crun_make_error (err, errno, "sigaction");
    }

  החזר 0;
}

/* must be used on the host before pivot_root(2).  */
ساكن صحيح
initialize_security (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, libcrun_error_t *err)
{
  صحيح ret;

  ret = libcrun_init_caps (container, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (UNLIKELY (proc == عدم))
    החזר 0;

  אם (proc->apparmor_profile)
    {
      ret = libcrun_initialize_apparmor (err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  ret = libcrun_initialize_selinux (container, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  החזר 0;
}

/* Emit the "annotations" map.  Returns a json_gen_status; callers can wrap
   the call with GEN_OR_FAIL.  */
ساكن صحيح
gen_annotations (json_gen_ctx *gen, json_map_string_string *annotations)
{
  size_t i;
  صحيح r;

  GEN_KEY (gen, "annotations");
  GEN_OR_FAIL (json_gen_map_open (gen));

  עבור (i = 0; i < annotations->len; i++)
    {
      /* An annotation whose value is null in the document is parsed into a
         NULL, which cannot be emitted as a JSON string; leave it out.  */
      אם (annotations->keys[i] == عدم || annotations->values[i] == عدم)
        استمر;

      GEN_STR (gen, annotations->keys[i]);
      GEN_STR (gen, annotations->values[i]);
    }

  החזר json_gen_map_close (gen);

gen_error:
  החזר r;
}

/* Return the bundle path to report in the container state.  crun always
   chdir()s into the bundle, so fall back to getcwd() when the caller has no
   absolute bundle path, as can happen when libcrun is used as a library.
   *ALLOCATED must be freed by the caller.  */
ساكن ثابت حرف *
get_bundle_path (ثابت حرف *bundle, حرف **allocated)
{
  אם (bundle != عدم && bundle[0] == '/')
    החזר bundle;

  *allocated = getcwd (عدم, 0);
  אם (*allocated == عدم)
    OOM ();
  החזר *allocated;
}

ساكن صحيح
do_hooks (runtime_spec_schema_config_schema *def, pid_t pid, ثابت حرف *id, bool keep_going, ثابت حرف *bundle,
          ثابت حرف *cwd, ثابت حرف *status, ثابت حرف *name, hook **hooks, size_t hooks_len, صحيح out_fd,
          صحيح err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  size_t i, stdin_len;
  صحيح r, ret;
  حرف *stdin = عدم;
  cleanup_free حرف *bundle_allocated = عدم;
  ثابت حرف *rootfs = def->root ? def->root->path : "";
  cleanup_json_gen json_gen_ctx *gen = عدم;

  bundle = get_bundle_path (bundle, &bundle_allocated);

  אם (! json_gen_init (&gen, عدم))
    החזר crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 0);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_KEY (gen, "ociVersion");
  GEN_STR (gen, "1.0");

  GEN_KEY (gen, "id");
  GEN_STR (gen, id);

  GEN_KEY (gen, "pid");
  GEN_OR_FAIL (map_int (gen, pid));

  GEN_KEY (gen, "root");
  GEN_STR (gen, rootfs);

  GEN_KEY (gen, "bundle");
  GEN_STR (gen, bundle);

  GEN_KEY (gen, "status");
  GEN_STR (gen, status);

  אם (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, (ثابت حرف **) &stdin, &stdin_len));

  ret = 0;

  bool error_created = false;
  עבור (i = 0; i < hooks_len; i++)
    {
      حرف **env = environ;

      אם (hooks[i]->env)
        env = hooks[i]->env;

      /* Release the error from the previous iteration, if any.  */
      אם (error_created)
        {
          crun_error_release (err);
          error_created = false;
        }

      ret = run_process_with_stdin_timeout_envp (hooks[i]->path, hooks[i]->args, cwd, hooks[i]->timeout, env,
                                                 stdin, stdin_len, out_fd, err_fd, can_ignore_chdir_errors, err);
      אם (UNLIKELY (ret < 0))
        error_created = true;

      אם (UNLIKELY (ret != 0))
        {
          אם (keep_going)
            libcrun_warning ("error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
          אחרת
            {
              libcrun_error (0, "error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
              اكسر;
            }
        }
    }

  החזר ret;

gen_error:
  החזר json_gen_error_to_crun_error (r, err);
}

ساكن صحيح
get_json_gen_result (json_gen_ctx *gen, حرف **out, size_t *out_len)
{
  ثابت حرف *buf = عدم;
  size_t buf_len = 0;
  صحيح r;

  r = json_gen_get_buf (gen, &buf, &buf_len);
  אם (UNLIKELY (r != json_gen_status_ok))
    החזר r;

  *out_len = buf_len;

  *out = xmalloc (buf_len + 1);
  memcpy (*out, buf, buf_len);
  (*out)[buf_len] = '\0';

  החזר json_gen_status_ok;
}

ساكن صحيح
get_seccomp_receiver_fd_payload (libcrun_container_t *container, ثابت حرف *status, pid_t own_pid,
                                 حرف **seccomp_fd_payload, size_t *seccomp_fd_payload_len, libcrun_error_t *err)
{
  صحيح r;
  json_gen_ctx *gen = عدم;
  runtime_spec_schema_config_schema *def = container->container_def;
  ثابت حرف *ثابت OCI_VERSION = "0.2.0";

  אם (! json_gen_init (&gen, عدم))
    החזר crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_KEY (gen, "ociVersion");
  GEN_STR (gen, OCI_VERSION);

  GEN_KEY (gen, "fds");
  GEN_OR_FAIL (json_gen_array_open (gen));

  GEN_KEY (gen, "seccompFd");
  GEN_OR_FAIL (json_gen_array_close (gen));

  GEN_KEY (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  אם (def && def->linux && def->linux->seccomp)
    {
      ثابت حرف *metadata = def->linux->seccomp->listener_metadata;

      אם (metadata)
        {
          GEN_KEY (gen, "metadata");
          GEN_STR (gen, metadata);
        }
    }

  /* State.  */
  GEN_KEY (gen, "state");
  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_KEY (gen, "ociVersion");
  GEN_STR (gen, OCI_VERSION);

  אם (container->context && container->context->id)
    {
      GEN_KEY (gen, "id");
      GEN_STR (gen, container->context->id);
    }

  GEN_KEY (gen, "status");
  GEN_STR (gen, status);

  GEN_KEY (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  אם (container->context && container->context->bundle)
    {
      GEN_KEY (gen, "bundle");
      GEN_STR (gen, container->context->bundle);
    }

  אם (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));
  /* End state.  */

  GEN_OR_FAIL (json_gen_map_close (gen));

  r = get_json_gen_result (gen, seccomp_fd_payload, seccomp_fd_payload_len);

gen_error:
  json_gen_free (gen);

  החזר json_gen_error_to_crun_error (r, err);
}

ساكن صحيح
send_sync_cb (فراغ *data, libcrun_error_t *err)
{
  صحيح sync_socket_fd = *((صحيح *) data);
  صحيح ret;

  /* sync 2.  */
  ret = sync_socket_send_sync (sync_socket_fd, false, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  /* sync 3.  */
  החזר sync_socket_wait_sync (عدم, sync_socket_fd, false, err);
}

ساكن صحيح
maybe_chown_std_streams (uid_t container_uid, gid_t container_gid, libcrun_error_t *err)
{
  صحيح ret, i;

  עבור (i = 0; i < 3; i++)
    {
      אם (! isatty (i))
        {
          بنية stat statbuf;
          ret = fstat (i, &statbuf);
          אם (UNLIKELY (ret < 0))
            {
              אם (errno == EBADF)
                استمر;
              החזר crun_make_error (err, errno, "fstat fd `%d`", i);
            }

          /* Skip chown for device files */
          אם (S_ISCHR (statbuf.st_mode) || S_ISBLK (statbuf.st_mode))
            استمر;

          ret = fchown (i, container_uid, container_gid);
          אם (UNLIKELY (ret < 0))
            {
              /* EINVAL means the user is not mapped in the current userns.
                 Ignore EPERM and EROFS as well as there is no reason to fail
                 so early, and let the container payload deal with it.
                 EBADF means fd is closed.
              */
              אם (errno == EINVAL || errno == EPERM || errno == EROFS || errno == EBADF)
                استمر;

              החזר crun_make_error (err, errno, "fchown std stream `%i`", i);
            }
        }
    }
  החזר 0;
}

صحيح
libcrun_container_notify_handler (بنية container_entrypoint_s *args,
                                  تعداد handler_configure_phase phase,
                                  libcrun_container_t *container, ثابت حرف *rootfs,
                                  libcrun_error_t *err)
{
  بنية custom_handler_s *h;

  אם (args->custom_handler == عدم)
    החזר 0;

  h = args->custom_handler->vtable;
  אם (h == عدم || h->configure_container == عدم)
    החזר 0;

  החזר h->configure_container (args->custom_handler->cookie, phase,
                                 args->context, container,
                                 rootfs, err);
}

/* Resolve and normalize the container rootfs path.  */
ساكن صحيح
resolve_rootfs_path (libcrun_container_t *container, حرف **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  אם (def->root && def->root->path)
    {
      *rootfs = realpath (def->root->path, عدم);
      אם (UNLIKELY (*rootfs == عدم))
        {
          صحيح saved_errno = errno;

          /* If realpath failed for any reason, try the relative directory.  */
          אם (def->root->path[0] == '/')
            {
              cleanup_free حرف *cwd = عدم;
              libcrun_error_t tmp_err = عدم;
              ssize_t len;
              صحيح proc_fd;

              proc_fd = libcrun_get_cached_proc_fd (container, &tmp_err);
              אם (LIKELY (proc_fd >= 0))
                {
                  len = safe_readlinkat (proc_fd, "self/cwd", &cwd, 0, &tmp_err);
                  אם (LIKELY (len > 0))
                    {
                      /* If the rootfs is under the current working directory, just use its relative path.  */
                      אם (has_dir_prefix (def->root->path, cwd))
                        {
                          ثابت حرف *it = consume_slashes (def->root->path + len);
                          אם (LIKELY (*it))
                            {
                              صحيح ret = faccessat (AT_FDCWD, it, F_OK, 0);
                              אם (LIKELY (ret == 0))
                                *rootfs = xstrdup (it);
                            }
                        }
                    }
                }
              crun_error_release (&tmp_err);
            }

          אם (*rootfs == عدم)
            {
              אם (saved_errno == ENOENT)
                החזר crun_make_error (err, ENOENT, "the rootfs path `%s` does not exist", def->root->path);

              *rootfs = xstrdup (def->root->path);
            }
        }
    }
  החזר 0;
}

/* Initialize the environment variables.  */
ساكن صحيح
setup_environment (runtime_spec_schema_config_schema *def, uid_t container_uid, libcrun_error_t *err)
{
  صحيح ret;

  אם (clearenv ())
    החזר crun_make_error (err, errno, "clearenv");

  אם (def->process)
    {
      size_t i;

      עבור (i = 0; i < def->process->env_len; i++)
        {
          /* A null element of the array of variables is parsed into a NULL,
             which putenv(3) cannot be handed.  */
          אם (def->process->env[i] == عدم)
            החזר crun_make_error (err, EINVAL, "environment variable is not specified");

          אם (putenv (def->process->env[i]) < 0)
            החזר crun_make_error (err, errno, "putenv `%s`", def->process->env[i]);
        }
    }

  אם (is_empty_string (getenv ("HOME")))
    {
      ret = set_home_env (container_uid);
      אם (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          אם (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            החזר crun_make_error (err, errno, "setenv HOME");
        }
    }

  החזר 0;
}

/* Configure terminal and console for interactive containers.  */
ساكن صحيح
setup_terminal (بنية container_entrypoint_s *entrypoint_args, libcrun_container_t *container,
                صحيح console_socket, صحيح console_socketpair, صحيح has_terminal, libcrun_error_t *err)
{
  cleanup_close صحيح terminal_fd = -1;
  صحيح ret;

  אם (! has_terminal)
    החזר 0;

  fflush (stderr);

  אם (console_socket >= 0 || (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0))
    {
      terminal_fd = libcrun_set_terminal (container, err);
      אם (UNLIKELY (terminal_fd < 0))
        החזר terminal_fd;
    }

  אם (console_socket >= 0)
    {
      ret = send_fd_to_socket (console_socket, terminal_fd, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }
  אחרת אם (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0)
    {
      ret = send_fd_to_socket (console_socketpair, terminal_fd, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  החזר 0;
}

/* Resolve and validate the container's executable path.  */
ساكن صحيح
setup_executable_path (بنية container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                       حرف **exec_path, libcrun_error_t *err)
{
  صحيح ret;

  אם (def->process && def->process->args)
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      אם (UNLIKELY (ret < 0))
        {
          אם (entrypoint_args->custom_handler == عدم && crun_error_get_errno (err) == ENOENT)
            החזר ret;

          /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
             once the process switched to the use that runs in the container.  This might be necessary
             when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
             is not honored.  */
          crun_error_release (err);
        }
    }

  החזר 0;
}

/* Apply security settings for the container process.  */
ساكن صحيح
apply_security_settings (بنية container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                         libcrun_container_t *container, pid_t own_pid, libcrun_error_t *err)
{
  صحيح ret;
  runtime_spec_schema_config_schema_process_capabilities *capabilities;
  صحيح no_new_privs;

  אם (def->process && ! def->process->no_new_privileges)
    {
      حرف **seccomp_flags = عدم;
      size_t seccomp_flags_len = 0;
      cleanup_free حرف *seccomp_fd_payload = عدم;
      size_t seccomp_fd_payload_len = 0;

      אם (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      אם (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          אם (UNLIKELY (ret < 0))
            החזר ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  capabilities = def->process ? def->process->capabilities : عدم;
  no_new_privs = def->process ? def->process->no_new_privileges : 1;
  ret = libcrun_set_caps (capabilities, container->container_uid, container->container_gid, no_new_privs,
                          entrypoint_args->custom_handler != عدم, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  החזר 0;
}

/* Create the process that joins the existing PID namespace now that the
   host mounts are not visible anymore, and continue the setup from there.  */
ساكن صحيح
do_delayed_pidns_fork (صحيح sync_socket, pid_t *own_pid, libcrun_error_t *err)
{
  pid_t pid;
  صحيح ret;

  pid = fork ();
  אם (UNLIKELY (pid < 0))
    החזר crun_make_error (err, errno, "fork");

  אם (pid)
    {
      /* Report the new PID to the parent and exit immediately.  */
      ret = sync_socket_send_sync (sync_socket, false, err);
      אם (LIKELY (ret == 0))
        {
          ret = TEMP_FAILURE_RETRY (اكتب (sync_socket, &pid, حجم (pid)));
          אם (LIKELY (ret == حجم (pid)))
            _safe_exit (EXIT_SUCCESS);
        }

      kill (pid, SIGKILL);
      _safe_exit (EXIT_FAILURE);
    }

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (اقرأ (sync_socket, own_pid, حجم (*own_pid)));
  אם (UNLIKELY (ret != حجم (*own_pid)))
    {
      אם (ret >= 0)
        errno = 0;
      החזר crun_make_error (err, errno, "read from sync socket");
    }

  החזר 0;
}

بنية delayed_pidns_fork_s
{
  libcrun_container_t *container;
  صحيح sync_socket;
  pid_t *own_pid;
};

ساكن صحيح
delayed_pidns_fork_cb (فراغ *data, libcrun_error_t *err)
{
  بنية delayed_pidns_fork_s *d = data;

  אם (! libcrun_container_delayed_pidns_fork (d->container))
    החזר 0;

  החזר do_delayed_pidns_fork (d->sync_socket, d->own_pid, err);
}

ساكن صحيح
container_init_setup (فراغ *args, pid_t *own_pid, حرف *notify_socket,
                      صحيح sync_socket, حرف **exec_path, libcrun_error_t *err)
{
  بنية container_entrypoint_s *entrypoint_args = args;
  libcrun_container_t *container = entrypoint_args->container;
  bool chdir_done = false;
  صحيح ret;
  صحيح has_terminal;
  cleanup_close صحيح console_socket = -1;
  cleanup_close صحيح console_socketpair = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free حرف *rootfs = عدم;
  بنية delayed_pidns_fork_s delayed_pidns_fork = {
    .container = container,
    .sync_socket = sync_socket,
    .own_pid = own_pid,
  };

  ret = initialize_security (container, def->process, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = libcrun_configure_network (container, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = resolve_rootfs_path (container, &rootfs, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  /* Configure terminal socket pair for container communication.  */
  אם (entrypoint_args->terminal_socketpair[0] >= 0)
    {
      close_and_reset (&entrypoint_args->terminal_socketpair[0]);
      console_socketpair = entrypoint_args->terminal_socketpair[1];
    }

  /* sync 1.  */
  ret = sync_socket_wait_sync (عدم, sync_socket, false, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  has_terminal = container->container_def->process && container->container_def->process->terminal;
  אם (has_terminal && entrypoint_args->context->console_socket)
    console_socket = entrypoint_args->console_socket_fd;

  ret = libcrun_set_sysctl (container, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_BEFORE_MOUNTS, container, rootfs, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = libcrun_do_pivot_root (container, entrypoint_args->context->no_pivot, &rootfs, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  /* sync 2 and 3 are sent as part of libcrun_set_mounts.  */
  ret = libcrun_set_mounts (entrypoint_args, container, rootfs, send_sync_cb, &sync_socket, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  /* The createContainer hooks MUST run after the mounts are set up but before
     the pivot_root.  When these hooks are present can_use_open_tree_namespace()
     forces the deferred-pivot path, so the actual pivot happens later in
     libcrun_finalize_mounts().  */
  אם (def->hooks && def->hooks->create_container_len)
    {
      libcrun_error_t tmp_err = عدم;
      صحيح in_userns = check_running_in_user_namespace (&tmp_err);
      אם (tmp_err)
        crun_error_release (&tmp_err);
      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, عدم, "created", "createContainer",
                      (hook **) def->hooks->create_container,
                      def->hooks->create_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      אם (UNLIKELY (ret != 0))
        החזר ret;
    }

  ret = libcrun_finalize_mounts (entrypoint_args, container, rootfs, delayed_pidns_fork_cb, &delayed_pidns_fork, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (def->process)
    {
      ret = libcrun_set_selinux_label (container, def->process, false, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      ret = libcrun_set_apparmor_profile (container, def->process, false, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  ret = mark_or_close_fds_ge_than (container, entrypoint_args->context->preserve_fds + 3, false, err);
  אם (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ret = libcrun_reopen_dev_null (err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = maybe_chown_std_streams (container->container_uid, container->container_gid, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = setup_environment (def, container->container_uid, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  /* Set primary process to 1 explicitly if nothing is configured and LISTEN_FD is not set.  */
  אם (entrypoint_args->context->listen_fds > 0 && getenv ("LISTEN_PID") == عدم)
    {
      libcrun_warning ("setting LISTEN_PID=1 since no previous configuration was found");
      אם (UNLIKELY (setenv ("LISTEN_PID", "1", 1) < 0))
        החזר crun_make_error (err, errno, "setenv LISTEN_PID");
    }

  /* Attempt to chdir immediately here, before doing the setresuid.  If we fail here, let's
     try again later once the process switched to the user that runs in the container.  */
  אם (def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      אם (LIKELY (ret == 0))
        chdir_done = true;
      אחרת
        crun_error_release (err);
    }

  ret = setsid ();
  אם (UNLIKELY (ret < 0))
    החזר crun_make_error (err, errno, "setsid");

  ret = setup_terminal (entrypoint_args, container, console_socket, console_socketpair, has_terminal, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  close_and_reset (&console_socket);
  close_and_reset (&console_socketpair);

  ret = setup_executable_path (entrypoint_args, def, exec_path, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = libcrun_set_hostname (container, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = libcrun_set_domainname (container, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  אם (def->process && def->process->user)
    umask (def->process->user->umask_present ? def->process->user->umask : 0022);

  ret = apply_security_settings (entrypoint_args, def, container, *own_pid, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (UNLIKELY (def->process && def->process->args && *exec_path == عدم))
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      אם (UNLIKELY (ret < 0))
        {
          אם (entrypoint_args->custom_handler == عدم || is_empty_string (def->process->args[0]))
            החזר ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          *exec_path = xstrdup (def->process->args[0]);
        }
    }

  /* The chdir was not already performed, so try again now after switching to the UID/GID in the container.  */
  אם (! chdir_done && def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  אם (notify_socket && putenv (notify_socket) < 0)
    החזר crun_make_error (err, errno, "putenv `%s`", notify_socket);

  החזר 0;
}

ساكن صحيح
open_hooks_output (libcrun_container_t *container, صحيح *out_fd, صحيح *err_fd, libcrun_error_t *err)
{
  ثابت حرف *annotation;

  *err_fd = *out_fd = -1;

  libcrun_debug ("Opening hooks output");
  annotation = find_annotation (container, "run.oci.hooks.stdout");
  אם (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stdout` annotation");
      *out_fd = TEMP_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      אם (UNLIKELY (*out_fd < 0))
        החזר crun_make_error (err, errno, "open `%s`", annotation);
    }

  annotation = find_annotation (container, "run.oci.hooks.stderr");
  אם (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stderr` annotation");
      *err_fd = TEMP_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      אם (UNLIKELY (*err_fd < 0))
        החזר crun_make_error (err, errno, "open `%s`", annotation);
    }

  החזר 0;
}

/* The environment the process was started with lives on the initial stack, right after the
   argv strings and before the string AT_EXECFN points to.  clearenv(3) only drops the pointers
   to it, so when there is no execv to replace the stack, erase it or the container process can
   still read it, e.g. through /proc/self/environ.  */
ساكن فراغ
erase_initial_environment (حرف **argv, صحيح argc)
{
  حرف *start = argv[argc - 1] + strlen (argv[argc - 1]) + 1;
  حرف *end = (حرف *) getauxval (AT_EXECFN);

  אם (end > start)
    memset (start, 0, end - start);
}

ساكن فراغ
rewrite_argv (حرف **argv, صحيح argc, ثابت حرف *name, حرف **args, size_t args_len)
{
  cleanup_free حرف *decorated_name = عدم;
  size_t available_len = argv[argc - 1] - argv[0] + strlen (argv[argc - 1]) + 1;
  cleanup_free حرف *new_argv = عدم;
  size_t i, so_far = 0, needed = 0;

  needed = xasprintf (&decorated_name, "[libcrun:%s]", name) + 1;
  עבור (i = 0; i < args_len; i++)
    needed += strlen (args[i]) + 1;

  new_argv = xmalloc0 (needed + 1);
  strcpy (new_argv, decorated_name);
  so_far = strlen (decorated_name) + 1;

  אם (available_len >= needed)
    {
      עבור (i = 0; i < args_len; i++)
        {
          strcpy (new_argv + so_far, args[i]);
          so_far += strlen (args[i]) + 1;
        }
    }

  אם (so_far >= available_len)
    so_far = available_len - 1;

  memset (argv[0], 0, available_len);
  memcpy (argv[0], new_argv, so_far);
}

/* Entrypoint to the container.  */
ساكن صحيح
container_init (فراغ *args, حرف *notify_socket, صحيح sync_socket, libcrun_error_t *err)
{
  بنية container_entrypoint_s *entrypoint_args = args;
  صحيح ret;
  runtime_spec_schema_config_schema *def = entrypoint_args->container->container_def;
  cleanup_free حرف *exec_path = عدم;
  __attribute__ ((unused)) cleanup_free حرف *notify_socket_cleanup = notify_socket;
  pid_t own_pid = 0;

  entrypoint_args->sync_socket = sync_socket;

  crun_set_output_handler (log_write_to_sync_socket, args);

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (اقرأ (sync_socket, &own_pid, حجم (own_pid)));
  אם (UNLIKELY (ret != حجم (own_pid)))
    {
      אם (ret >= 0)
        errno = 0;
      החזר crun_make_error (err, errno, "read from sync socket");
    }

  ret = container_init_setup (args, &own_pid, notify_socket, sync_socket, &exec_path, err);
  אם (UNLIKELY (ret < 0))
    {
      /* If it fails to write the error using the sync socket, then fallback
         to stderr.  */
      אם (sync_socket_write_error (sync_socket, err) == 0)
        {
          crun_error_release (err);
          /* A return value of > 0 means to fail with an error without err being set so to avoid
             printing it twice.  */
          החזר 1;
        }
      החזר ret;
    }

  entrypoint_args->sync_socket = -1;

  ret = unblock_signals (err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  /* sync 4.  */
  ret = sync_socket_send_sync (sync_socket, false, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  close_and_reset (&sync_socket);

  אם (entrypoint_args->context->fifo_exec_wait_fd >= 0)
    {
      حرف buffer[1];
      fd_set read_set;
      cleanup_close صحيح fd = entrypoint_args->context->fifo_exec_wait_fd;
      entrypoint_args->context->fifo_exec_wait_fd = -1;

      FD_ZERO (&read_set);
      FD_SET (fd, &read_set);
      עשה
        {
          ret = select (fd + 1, &read_set, عدم, عدم, عدم);
          אם (UNLIKELY (ret < 0))
            החזר crun_make_error (err, errno, "select");

          ret = TEMP_FAILURE_RETRY (اقرأ (fd, buffer, حجم (buffer)));
          אם (UNLIKELY (ret < 0))
            החזר crun_make_error (err, errno, "read from the exec fifo");
      } כלעוד (ret == 0);

      close_and_reset (&entrypoint_args->context->fifo_exec_wait_fd);
    }

  crun_set_output_handler (log_write_to_stderr, عدم);

  אם (def->process && def->process->no_new_privileges)
    {
      حرف **seccomp_flags = عدم;
      size_t seccomp_flags_len = 0;
      cleanup_free حرف *seccomp_fd_payload = عدم;
      size_t seccomp_fd_payload_len = 0;

      אם (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      אם (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (entrypoint_args->container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          אם (UNLIKELY (ret < 0))
            החזר ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags,
                                   seccomp_flags_len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  אם (UNLIKELY (def->process == عدم))
    החזר crun_make_error (err, 0, "block `process` not found");

  אם (UNLIKELY (exec_path == عدم))
    החזר crun_make_error (err, 0, "executable path not specified");

  אם (def->hooks && def->hooks->start_container_len)
    {
      libcrun_container_t *container = entrypoint_args->container;
      libcrun_error_t tmp_err = عدم;
      صحيح in_userns = check_running_in_user_namespace (&tmp_err);
      אם (tmp_err)
        crun_error_release (&tmp_err);

      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, عدم, "created", "startContainer",
                      (hook **) def->hooks->start_container,
                      def->hooks->start_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      אם (UNLIKELY (ret != 0))
        החזר ret;

      /* Seek stdout/stderr to the end.  If the hooks were using the same files,
         the container process overwrites what was previously written.  */
      (فراغ) lseek (1, 0, SEEK_END);
      (فراغ) lseek (2, 0, SEEK_END);
    }

  אם (entrypoint_args->custom_handler)
    {
      /* Files marked with O_CLOEXEC are closed at execv time, so make sure they are closed now.
         This is a best effort operation, because the seccomp filter is already in place and it could
         stop some syscalls used by mark_or_close_fds_ge_than.
      */
      אם (entrypoint_args->custom_handler->vtable->close_fds)
        {
          ret = entrypoint_args->custom_handler->vtable->close_fds (entrypoint_args->custom_handler->cookie,
                                                                    entrypoint_args->container,
                                                                    entrypoint_args->context->preserve_fds,
                                                                    err);
        }
      אחרת
        {
          ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
        }
      אם (UNLIKELY (ret < 0))
        crun_error_release (err);

      prctl (PR_SET_NAME, entrypoint_args->custom_handler->vtable->name);

      אם (entrypoint_args->context->argv)
        {
          erase_initial_environment (entrypoint_args->context->argv, entrypoint_args->context->argc);
          rewrite_argv (entrypoint_args->context->argv, entrypoint_args->context->argc,
                        entrypoint_args->custom_handler->vtable->name, def->process->args,
                        def->process->args_len);

          /* It is a quite destructive operation as we might be referencing data from the old
             argv, so make sure the context is not reused.  */
          entrypoint_args->context->argv = عدم;
          entrypoint_args->context = عدم;
        }

      ret = libcrun_set_selinux_label (entrypoint_args->container, def->process, true, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      ret = libcrun_set_apparmor_profile (entrypoint_args->container, def->process, true, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      ret = entrypoint_args->custom_handler->vtable->run_func (entrypoint_args->custom_handler->cookie,
                                                               entrypoint_args->container,
                                                               exec_path,
                                                               def->process->args);
      אם (ret != 0)
        החזר crun_make_error (err, 0, "exec container process failed with handler as `%s`", entrypoint_args->custom_handler->vtable->name);

      החזר ret;
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation since the seccomp profile is already in place now and might block
     some of the syscalls needed by mark_or_close_fds_ge_than.  */
  ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
  אם (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, def->process->args));

  אם (errno == ENOENT)
    החזר crun_make_error (err, errno, "exec container process (missing dynamic library?) `%s`", exec_path);

  החזר crun_make_error (err, errno, "exec container process `%s`", exec_path);
}

ساكن صحيح
read_container_config_from_state (libcrun_container_t **container, ثابت حرف *state_root, ثابت حرف *id,
                                  libcrun_error_t *err)
{
  cleanup_free حرف *config_file = عدم;
  cleanup_free حرف *dir = عدم;
  صحيح ret;

  *container = عدم;

  ret = libcrun_get_state_directory (&dir, state_root, id, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = append_paths (&config_file, err, dir, "config.json", عدم);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  *container = libcrun_container_load_from_file (config_file, err);
  אם (*container == عدم)
    החזר -1;

  החזר 0;
}

ساكن صحيح
run_poststop_hooks (arg_unused libcrun_context_t *context, libcrun_container_t *container,
                    runtime_spec_schema_config_schema *def,
                    libcrun_container_status_t *status, ثابت حرف *state_root, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container_cleanup = عدم;
  صحيح ret;

  אם (def == عدم)
    {
      אם (container == عدم)
        {
          ret = read_container_config_from_state (&container_cleanup, state_root, id, err);
          אם (UNLIKELY (ret < 0))
            החזר ret;
          container = container_cleanup;
        }

      def = container->container_def;
    }

  אם (def->hooks && def->hooks->poststop_len)
    {
      cleanup_close صحيح hooks_out_fd = -1;
      cleanup_close صحيح hooks_err_fd = -1;

      אם (container == عدم)
        {
          ret = read_container_config_from_state (&container_cleanup, state_root, id, err);
          אם (UNLIKELY (ret < 0))
            החזר ret;
          container = container_cleanup;
        }

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      ret = do_hooks (def, 0, id, true, status->bundle, status->bundle, "stopped", "poststop", (hook **) def->hooks->poststop,
                      def->hooks->poststop_len, hooks_out_fd, hooks_err_fd, false, err);
      אם (UNLIKELY (ret != 0))
        {
          אם (ret < 0)
            libcrun_error_report_and_release (err);
          אחרת
            libcrun_error (0, "poststop hook failed with exit code: %d", ret);
        }
    }
  החזר 0;
}

ساكن bool
has_new_pid_namespace (runtime_spec_schema_config_schema *def)
{
  size_t i;

  אם (def->linux == عدم || def->linux->namespaces == عدم)
    החזר false;

  עבור (i = 0; i < def->linux->namespaces_len; i++)
    {
      אם (strcmp (def->linux->namespaces[i]->type, "pid") == 0 && def->linux->namespaces[i]->path == عدم)
        החזר true;
    }
  החזר false;
}

/* Delete the container state.  CONTAINER is the container the caller already has
   available, or NULL.  When it is not NULL, the poststop hooks are run even if the
   status file was never written, e.g. when the container creation failed.  */
ساكن صحيح
container_delete_internal (libcrun_context_t *context, libcrun_container_t *container,
                           runtime_spec_schema_config_schema *def, ثابت حرف *id, bool force, bool killall,
                           libcrun_error_t *err)
{
  cleanup_cgroup_status بنية libcrun_cgroup_status *cgroup_status = عدم;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_container libcrun_container_t *container_cleanup = عدم;
  ثابت حرف *state_root = context->state_root;
  صحيح ret;

  ret = libcrun_read_container_status (&status, state_root, id, err);
  אם (UNLIKELY (ret < 0))
    {
      אם (force && crun_error_get_errno (err) == ENOENT)
        {
          libcrun_error_t tmp_err = عدم;

          crun_error_release (err);

          אם (container)
            {
              libcrun_container_status_t partial_status = {
                .bundle = (حرف *) context->bundle,
              };

              ret = run_poststop_hooks (context, container, def, &partial_status, state_root, id, err);
              אם (UNLIKELY (ret < 0))
                crun_error_write_warning_and_release (context->output_handler_arg, &err);
            }

          libcrun_container_delete_status (state_root, id, &tmp_err);
          crun_error_release (&tmp_err);
          החזר 0;
        }
      crun_error_release (err);
      החזר libcrun_container_delete_status (state_root, id, err);
    }

  cgroup_status = libcrun_cgroup_make_status (&status);

  אם (! force)
    {
      ret = libcrun_is_container_running (&status, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
      אם (ret == 1)
        {
          ret = libcrun_status_has_read_exec_fifo (state_root, id, err);
          אם (UNLIKELY (ret < 0))
            החזר ret;
          אם (ret == 0)
            החזר crun_make_error (err, 0, "the container `%s` is not in `created` or `stopped` state", id);

          /* If the container is in "created" state, then do the equivalent of delete --force.  */
          killall = force = true;
        }
    }

  אם (def == عدم)
    {
      אם (container == عدم)
        {
          ret = read_container_config_from_state (&container_cleanup, state_root, id, err);
          אם (UNLIKELY (ret < 0))
            החזר ret;

          container = container_cleanup;
        }

      def = container->container_def;
    }

  אם (killall && force)
    {
      /* If the container has a pid namespace, it is enough to kill the first
         process (pid=1 in the namespace).
      */
      אם (has_new_pid_namespace (def))
        {
          ret = libcrun_kill_linux (&status, SIGKILL, err);
          אם (UNLIKELY (ret < 0))
            {
              אם (crun_error_get_errno (err) != ESRCH)
                החזר ret;

              crun_error_release (err);
            }
        }
      אחרת אם (status.cgroup_path)
        {
          ret = libcrun_cgroup_killall (cgroup_status, SIGKILL, err);
          אם (UNLIKELY (ret < 0))
            החזר ret;
        }
    }

  אם (def->linux && def->linux->intel_rdt)
    {
      ret = libcrun_destroy_intelrdt (id, def, err);
      אם (UNLIKELY (ret < 0))
        libcrun_error_report_and_release (err);
    }

  אם (status.cgroup_path)
    {
      /* If the cgroup cannot be destroyed, there are still processes in it.
         Keep the container status, it is the only record of the cgroup, so
         that the delete can be retried.  */
      ret = libcrun_cgroup_destroy (cgroup_status, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  ret = run_poststop_hooks (context, container, def, &status, state_root, id, err);
  אם (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  החזר libcrun_container_delete_status (state_root, id, err);
}

صحيح
libcrun_container_delete (libcrun_context_t *context, ثابت حرف *id, bool force, libcrun_error_t *err)
{
  החזר container_delete_internal (context, عدم, عدم, id, force, true, err);
}

صحيح
libcrun_container_kill (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *إشارة, libcrun_error_t *err)
{
  صحيح sig, ret;
  ثابت حرف *state_root = context->state_root;
  cleanup_container_status libcrun_container_status_t status = {};

  sig = str2sig (إشارة);
  אם (UNLIKELY (sig < 0))
    החזר crun_make_error (err, 0, "unknown signal `%s`", إشارة);

  ret = libcrun_read_container_status (&status, state_root, id, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  החזר libcrun_kill_linux (&status, sig, err);
}

صحيح
libcrun_container_killall (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *إشارة, libcrun_error_t *err)
{
  صحيح sig, ret;
  ثابت حرف *state_root = context->state_root;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_cgroup_status بنية libcrun_cgroup_status *cgroup_status = عدم;

  sig = str2sig (إشارة);
  אם (UNLIKELY (sig < 0))
    החזר crun_make_error (err, 0, "unknown signal `%s`", إشارة);

  ret = libcrun_read_container_status (&status, state_root, id, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  cgroup_status = libcrun_cgroup_make_status (&status);

  ret = libcrun_cgroup_killall (cgroup_status, sig, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;
  החזר 0;
}

ساكن صحيح
write_container_status (libcrun_container_t *container, libcrun_context_t *context,
                        pid_t pid, بنية libcrun_cgroup_status *cgroup_status,
                        libcrun_error_t *err)
{
  cleanup_free حرف *bundle_allocated = عدم;
  ثابت حرف *bundle = get_bundle_path (context->bundle, &bundle_allocated);
  cleanup_free حرف *owner = get_user_name (geteuid ());
  حرف *external_descriptors = libcrun_get_external_descriptors (container);
  حرف *rootfs = container->container_def->root ? container->container_def->root->path : "";
  حرف created[35];

  libcrun_container_status_t status = {
    .pid = pid,
    .rootfs = rootfs,
    .bundle = (حرف *) bundle,
    .created = created,
    .owner = owner,
    .systemd_cgroup = context->systemd_cgroup,
    .detached = context->detach,
    .no_new_keyring = context->no_new_keyring,
    .external_descriptors = external_descriptors,
    .cgroup_path = عدم,
    .scope = عدم,
  };

  get_current_timestamp (created, حجم (created));

  אם (cgroup_status)
    {
      صحيح ret;

      ret = libcrun_cgroup_get_status (cgroup_status, &status, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  אם (external_descriptors == عدم)
    החזר crun_make_error (err, 0, "invalid internal state.  No external descriptors found");
  החזר libcrun_write_container_status (context->state_root, context->id, &status, err);
}

ساكن صحيح
reap_subprocesses (pid_t *main_process, صحيح *main_process_exit, صحيح *last_process, libcrun_error_t *err)
{
  *last_process = 0;
  כלעוד (1)
    {
      صحيح status;
      صحيح r = waitpid_ignore_stopped (-1, &status, WNOHANG);
      אם (r < 0)
        {
          אם (errno == EINTR)
            استمر;
          אם (errno == ECHILD)
            {
              *last_process = 1;
              החזר 0;
            }
          החזר crun_make_error (err, errno, "waitpid");
        }
      אם (r == 0)
        اكسر;
      אם (r != *main_process)
        استمر;

      *main_process_exit = get_process_exit_status (status);

      /* Once reaped, the pid can be reused by an unrelated process.  */
      *main_process = 0;
    }
  החזר 0;
}

ساكن صحيح
send_sd_notify (ثابت حرف *ready_str, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  صحيح ret = sd_notify (0, ready_str);
  אם (UNLIKELY (ret < 0))
    החזר crun_make_error (err, -ret, "sd_notify");

#  אם HAVE_SD_NOTIFY_BARRIER
  /* Hard-code a 30 seconds timeout.  Ignore errors.  */
  sd_notify_barrier (0, 30 * 1000000);
#  endif
#אחרת
  (فراغ) ready_str;
  (فراغ) err;
#endif
  החזר 0;
}

ساكن صحيح
handle_notify_socket (صحيح notify_socketfd, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  صحيح ret;
  حرف buf[256];
  ثابت حرف *ready_str = "READY=1";

  ret = recvfrom (notify_socketfd, buf, حجم (buf) - 1, 0, عدم, عدم);
  אם (UNLIKELY (ret < 0))
    החזר crun_make_error (err, errno, "recvfrom notify socket");

  buf[ret] = '\0';
  אם (strstr (buf, ready_str))
    {
      ret = send_sd_notify (ready_str, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      החזר 1;
    }
  החזר 0;
#אחרת
  (فراغ) notify_socketfd;
  (فراغ) err;
  החזר 1;
#endif
}

بنية wait_for_process_args
{
  pid_t pid;
  libcrun_context_t *context;
  صحيح terminal_fd;
  صحيح notify_socket;
  صحيح *container_ready_fd;
  صحيح seccomp_notify_fd;
  ثابت حرف *seccomp_notify_plugins;
};

ساكن صحيح
write_pid_file (ثابت حرف *pid_file, pid_t pid, libcrun_error_t *err)
{
  حرف buf[32];
  صحيح buf_len = snprintf (buf, حجم (buf), "%d", pid);
  אם (UNLIKELY (buf_len >= (صحيح) حجم (buf)))
    החזר crun_make_error (err, 0, "internal error: static buffer too small");
  החזר write_file_at_with_flags (AT_FDCWD, O_CREAT | O_TRUNC, 0700, pid_file, buf, buf_len, err);
}

#define MAX_EVENTS 10

ساكن bool
is_fd_pollable (صحيح fd)
{
  بنية epoll_event ev = { .events = EPOLLIN };
  cleanup_close صحيح epollfd = epoll_create1 (EPOLL_CLOEXEC);

  אם (epollfd < 0)
    החזר true;

  החזר epoll_ctl (epollfd, EPOLL_CTL_ADD, fd, &ev) == 0 || errno != EPERM;
}

ساكن صحيح
wait_for_process (بنية wait_for_process_args *args, libcrun_error_t *err)
{
  cleanup_channel_fd_pair بنية channel_fd_pair *from_terminal = عدم;
  cleanup_channel_fd_pair بنية channel_fd_pair *to_terminal = عدم;
  صحيح ret, container_exit_code = 0, last_process;
  cleanup_close صحيح terminal_fd_from = -1;
  cleanup_close صحيح terminal_fd_to = -1;
  bool stdin_pollable = false, stdout_pollable = false;
  cleanup_close صحيح epollfd = -1;
  cleanup_close صحيح signalfd = -1;
  sigset_t mask;
  صحيح in_fds[MAX_EVENTS];
  صحيح in_fds_len = 0;
  صحيح out_fds[MAX_EVENTS];
  صحيح out_fds_len = 0;
  size_t i;

  cleanup_seccomp_notify_context بنية seccomp_notify_context_s *seccomp_notify_ctx = عدم;

  container_exit_code = 0;

  אם (args == عدم || args->context == عدم)
    החזר crun_make_error (err, 0, "internal error: context is empty");

  עבור (i = 0; i < MAX_EVENTS; i++)
    {
      in_fds[i] = -1;
      out_fds[i] = -1;
    }

  אם (args->context->pid_file)
    {
      ret = write_pid_file (args->context->pid_file, args->pid, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  /* Also exit if there is nothing more to wait for.  */
  אם (args->context->detach && args->notify_socket < 0)
    החזר 0;

  אם (args->container_ready_fd)
    {
      ret = 0;
      TEMP_FAILURE_RETRY (اكتب (*args->container_ready_fd, &ret, حجم (ret)));
      close_and_reset (args->container_ready_fd);
    }

  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, عدم);
  אם (UNLIKELY (ret < 0))
    החזר crun_make_error (err, errno, "sigprocmask");

  signalfd = create_signalfd (&mask, err);
  אם (UNLIKELY (signalfd < 0))
    החזר signalfd;

  ret = reap_subprocesses (&args->pid, &container_exit_code, &last_process, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (last_process)
    החזר container_exit_code;

  אם (args->seccomp_notify_fd >= 0)
    {
      cleanup_free حرف *state_root = عدم;
      cleanup_free حرف *oci_config_path = عدم;

      بنية libcrun_load_seccomp_notify_conf_s conf;
      memset (&conf, 0, حجم conf);

      ret = libcrun_get_state_directory (&state_root, args->context->state_root, args->context->id, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      ret = append_paths (&oci_config_path, err, state_root, "config.json", عدم);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      conf.runtime_root_path = state_root;
      conf.name = args->context->id;
      conf.bundle_path = args->context->bundle;
      conf.oci_config_path = oci_config_path;

      ret = set_blocking_fd (args->seccomp_notify_fd, false, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      ret = libcrun_load_seccomp_notify_plugins (&seccomp_notify_ctx,
                                                 args->seccomp_notify_plugins,
                                                 &conf, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      in_fds[in_fds_len++] = args->seccomp_notify_fd;
    }

  אם (args->terminal_fd >= 0)
    {
      /* The terminal_fd is dup()ed so that it can be registered with
         epoll multiple times using different masks.  */
      terminal_fd_from = dup (args->terminal_fd);
      אם (UNLIKELY (terminal_fd_from < 0))
        החזר crun_make_error (err, errno, "dup terminal fd");
      terminal_fd_to = dup (args->terminal_fd);
      אם (UNLIKELY (terminal_fd_to < 0))
        החזר crun_make_error (err, errno, "dup terminal fd");

      صحيح i, non_blocking_fds[] = { terminal_fd_from, terminal_fd_to, 0, 1, -1 };
      עבור (i = 0; non_blocking_fds[i] >= 0; i++)
        {
          ret = set_blocking_fd (non_blocking_fds[i], false, err);
          אם (UNLIKELY (ret < 0))
            החזר ret;
        }

      /* stdin and stdout can be something epoll does not support, such as
         /dev/null or a regular file.  Such a stdin is not forwarded, and
         such a stdout is written to without waiting for it to be ready.  */
      stdin_pollable = is_fd_pollable (0);
      stdout_pollable = is_fd_pollable (1);

      from_terminal = channel_fd_pair_new (terminal_fd_from, 1, BUFSIZ);
      אם (stdin_pollable)
        to_terminal = channel_fd_pair_new (0, terminal_fd_to, BUFSIZ);
    }

  in_fds[in_fds_len++] = signalfd;
  אם (args->notify_socket >= 0)
    in_fds[in_fds_len++] = args->notify_socket;
  אם (args->terminal_fd >= 0)
    {
      אם (stdin_pollable)
        {
          in_fds[in_fds_len++] = 0;
          out_fds[out_fds_len++] = terminal_fd_to;
        }

      in_fds[in_fds_len++] = terminal_fd_from;
      אם (stdout_pollable)
        out_fds[out_fds_len++] = 1;
    }

  epollfd = epoll_helper (in_fds, عدم, out_fds, عدم, err);
  אם (UNLIKELY (epollfd < 0))
    החזר epollfd;

  כלעוד (1)
    {
      بنية epoll_event events[MAX_EVENTS];
      بنية signalfd_siginfo si;
      بنية winsize ws;
      صحيح i, nr_events;
      ssize_t res;

      nr_events = TEMP_FAILURE_RETRY (epoll_wait (epollfd, events, MAX_EVENTS, -1));
      אם (UNLIKELY (nr_events < 0))
        החזר crun_make_error (err, errno, "epoll_wait");

      עבור (i = 0; i < nr_events; i++)
        {
          אם (events[i].data.fd == 0 || events[i].data.fd == terminal_fd_to)
            {
              ret = channel_fd_pair_process (to_terminal, epollfd, err);
              אם (UNLIKELY (ret < 0))
                החזר crun_error_wrap (err, "copy to terminal fd");
            }
          אחרת אם (events[i].data.fd == 1 || events[i].data.fd == terminal_fd_from)
            {
              ret = channel_fd_pair_process (from_terminal, epollfd, err);
              אם (UNLIKELY (ret < 0))
                החזר crun_error_wrap (err, "copy from terminal fd");
            }
          אחרת אם (events[i].data.fd == args->seccomp_notify_fd)
            {
              ret = libcrun_seccomp_notify_plugins (seccomp_notify_ctx,
                                                    args->seccomp_notify_fd, err);
              אם (UNLIKELY (ret < 0))
                החזר ret;
            }
          אחרת אם (events[i].data.fd == args->notify_socket)
            {
              ret = handle_notify_socket (args->notify_socket, err);
              אם (UNLIKELY (ret < 0))
                החזר ret;
              אם (ret && args->context->detach)
                החזר 0;
            }
          אחרת אם (events[i].data.fd == signalfd)
            {
              res = TEMP_FAILURE_RETRY (اقرأ (signalfd, &si, حجم (si)));
              אם (UNLIKELY (res < 0))
                החזר crun_make_error (err, errno, "read from signalfd");
              אם (si.ssi_signo == SIGCHLD)
                {
                  ret = reap_subprocesses (&args->pid, &container_exit_code,
                                           &last_process, err);
                  אם (UNLIKELY (ret < 0))
                    החזר ret;
                  אם (last_process)
                    החזר container_exit_code;
                }
              אחרת אם (si.ssi_signo == SIGWINCH)
                {
                  /* Ignore the signal if the terminal is not available.  */
                  אם (args->terminal_fd > 0)
                    {
                      ret = ioctl (0, TIOCGWINSZ, &ws);
                      אם (UNLIKELY (ret < 0))
                        החזר crun_make_error (err, errno, "ioctl TIOCGWINSZ copy terminal size from stdin");

                      ret = ioctl (args->terminal_fd, TIOCSWINSZ, &ws);
                      אם (UNLIKELY (ret < 0))
                        החזר crun_make_error (err, errno, "ioctl TIOCSWINSZ copy terminal size to pty");
                    }
                }
              אחרת אם (args->pid > 0)
                {
                  /* Send any other signal to the child process, if it was not reaped yet.  */
                  ret = kill (args->pid, si.ssi_signo);
                }
            }
          אחרת
            {
              החזר crun_make_error (err, 0, "internal error: unknown fd from epoll_wait");
            }
        }
    }

  החזר 0;
}

ساكن فراغ
flush_fd_to_err (libcrun_context_t *context, صحيح terminal_fd)
{
  حرف buf[256];
  صحيح flags;

  אם (terminal_fd < 0 || stderr == عدم)
    החזר;

  flags = fcntl (terminal_fd, F_GETFL, 0);
  אם (flags == -1)
    החזר;
  אם (fcntl (terminal_fd, F_SETFL, flags | O_NONBLOCK) < 0)
    החזר;

  עבור (;;)
    {
      صحيح ret = TEMP_FAILURE_RETRY (اقرأ (terminal_fd, buf, حجم (buf) - 1));
      אם (ret <= 0)
        اكسر;
      buf[ret] = '\0';
      אם (context->output_handler)
        context->output_handler (0, buf, LIBCRUN_VERBOSITY_ERROR, context->output_handler_arg);
    }
  (فراغ) fcntl (terminal_fd, F_SETFL, flags);
  fflush (stderr);
  fsync (1);
  fsync (2);
}

ساكن صحيح
cleanup_watch (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
               بنية libcrun_cgroup_status *cgroup_status, pid_t init_pid, صحيح sync_socket,
               صحيح terminal_fd, libcrun_error_t *err)
{
  ثابت حرف *oom_message = عدم;
  libcrun_error_t tmp_err = عدم;
  صحيح ret;

  אם (init_pid)
    {
      /* Try to detect whether the cgroup has a OOM.  */
      אם (cgroup_status)
        {
          صحيح has_oom;

          has_oom = libcrun_cgroup_has_oom (cgroup_status, &tmp_err);
          אם (has_oom > 0)
            oom_message = "OOM: the memory limit could be too low";
          אחרת אם (has_oom < 0)
            {
              /* If the detection has failed for any reason, e.g. the cgroup was
                 already deleted by the time it was checked, just ignore the
                 failure.  */
              crun_error_release (&tmp_err);
            }
        }
      /* If the OOM wasn't detected, look into the static configuration.  */
      אם (oom_message == عدم && is_memory_limit_too_low (def))
        oom_message = "the memory limit could be too low";

      kill (init_pid, SIGKILL);
      waitpid_ignore_stopped (init_pid, عدم, 0);
    }

  ret = sync_socket_wait_sync (context, sync_socket, true, &tmp_err);
  אם (UNLIKELY (ret < 0))
    {
      crun_error_release (err);
      *err = tmp_err;
    }

  אם (terminal_fd >= 0)
    flush_fd_to_err (context, terminal_fd);

  אם (oom_message)
    החזר crun_error_wrap (err, "%s", oom_message);

  החזר -1;
}

/* Find the uid:gid that is mapped to root inside the container user namespace.  */
فراغ
get_root_in_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid, uid_t *uid,
                        gid_t *gid)
{
  *uid = -1;
  *gid = -1;

  /* Not root in the namespace.  */
  אם (host_uid)
    החזר;

  אם (def->linux && def->linux->uid_mappings)
    {
      size_t i;

      עבור (i = 0; i < def->linux->uid_mappings_len; i++)
        אם (def->linux->uid_mappings[i]->container_id == 0)
          {
            *uid = def->linux->uid_mappings[i]->host_id;
            اكسر;
          }
    }
  אם (def->linux && def->linux->gid_mappings)
    {
      size_t i;

      עבור (i = 0; i < def->linux->gid_mappings_len; i++)
        אם (def->linux->gid_mappings[i]->container_id == 0)
          {
            *gid = def->linux->gid_mappings[i]->host_id;
            اكسر;
          }
    }

  /* If the uid and the gid are not changed, do not attempt any chown.  */
  אם (*uid == host_uid && *gid == host_gid)
    *uid = *gid = -1;
}

ساكن صحيح
get_seccomp_receiver_fd (libcrun_container_t *container, صحيح *fd, صحيح *self_receiver_fd, ثابت حرف **plugins,
                         libcrun_error_t *err)
{
  ثابت حرف *tmp;
  runtime_spec_schema_config_schema *def = container->container_def;

  *fd = -1;
  *self_receiver_fd = -1;

  tmp = find_annotation (container, "run.oci.seccomp.plugins");
  אם (tmp)
    {
      صحيح fds[2];
      صحيح ret;

      ret = create_socket_pair (fds, err);
      אם (UNLIKELY (ret < 0))
        החזר crun_error_wrap (err, "create socket pair");

      *fd = fds[0];
      *self_receiver_fd = fds[1];
      *plugins = tmp;
      החזר 0;
    }

  אם (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    tmp = def->linux->seccomp->listener_path;
  אחרת
    tmp = find_annotation (container, "run.oci.seccomp.receiver");
  אם (tmp == عدم)
    tmp = getenv ("RUN_OCI_SECCOMP_RECEIVER");
  אם (tmp)
    {
      אם (tmp[0] != '/')
        החזר crun_make_error (err, 0, "the seccomp receiver `%s` is not an absolute path", tmp);

      *fd = open_unix_domain_client_socket (tmp, 0, err);
      אם (UNLIKELY (*fd < 0))
        החזר crun_error_wrap (err, "open seccomp receiver");
    }

  החזר 0;
}

ساكن bool
has_seccomp_receiver (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;

  אם (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    החזר true;

  אם (find_annotation (container, "run.oci.seccomp.receiver") != عدم || getenv ("RUN_OCI_SECCOMP_RECEIVER") != عدم)
    החזר true;

  החזר false;
}

ساكن صحيح
setup_container_hooks_output (libcrun_container_t *container, runtime_spec_schema_config_schema *def,
                              بنية container_entrypoint_s *container_args, صحيح *hooks_out_fd,
                              صحيح *hooks_err_fd, libcrun_error_t *err)
{
  صحيح ret;

  אם (def->hooks
      && (def->hooks->prestart_len || def->hooks->poststart_len || def->hooks->create_runtime_len
          || def->hooks->create_container_len || def->hooks->start_container_len))
    {
      ret = open_hooks_output (container, hooks_out_fd, hooks_err_fd, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
      container_args->hooks_out_fd = *hooks_out_fd;
      container_args->hooks_err_fd = *hooks_err_fd;
    }
  החזר 0;
}

ساكن صحيح
setup_container_keyring (libcrun_container_t *container, libcrun_context_t *context, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح ret;

  אם (! context->no_new_keyring)
    {
      ثابت حرف *label = عدم;

      libcrun_debug ("Creating new keyring");

      אם (def->process)
        {
          label = def->process->selinux_label;
          אם (label)
            libcrun_debug ("Using SELinux process label: `%s`", label);
        }

      ret = libcrun_create_keyring (container, container->context->id, label, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }
  החזר 0;
}

ساكن صحيح
setup_terminal_socket_pair (libcrun_container_t *container, libcrun_context_t *context,
                            بنية container_entrypoint_s *container_args, صحيح *socket_pair_0,
                            صحيح *socket_pair_1, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح ret;
  صحيح detach = context->detach;

  אם (def->process && def->process->terminal && ! detach && context->console_socket == عدم)
    {
      libcrun_debug ("Creating terminal socket pair");
      container_args->has_terminal_socket_pair = 1;
      ret = create_socket_pair (container_args->terminal_socketpair, err);
      אם (UNLIKELY (ret < 0))
        החזר crun_error_wrap (err, "create terminal socket");

      *socket_pair_0 = container_args->terminal_socketpair[0];
      *socket_pair_1 = container_args->terminal_socketpair[1];
    }
  החזר 0;
}

ساكن صحيح
setup_seccomp (libcrun_container_t *container, ثابت حرف *seccomp_bpf_data,
               بنية libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, صحيح *seccomp_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح ret;

  אם (find_annotation (container, "run.oci.seccomp.plugins") != عدم && has_seccomp_receiver (container))
    {
      החזר crun_make_error (err, 0, "seccomp plugins and seccomp receivers cannot be declared at the same time");
    }

  אם (def->linux && (def->linux->seccomp || seccomp_bpf_data))
    {
      unsigned صحيح seccomp_gen_options = 0;
      ثابت حرف *annotation;

      libcrun_debug ("Initializing seccomp");
      annotation = find_annotation (container, "run.oci.seccomp_fail_unknown_syscall");
      אם (annotation && strcmp (annotation, "0") != 0)
        seccomp_gen_options = LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL;

      אם (seccomp_bpf_data)
        seccomp_gen_options |= LIBCRUN_SECCOMP_SKIP_CACHE;

      libcrun_seccomp_gen_ctx_init (seccomp_gen_ctx, container, true, seccomp_gen_options);

      ret = libcrun_open_seccomp_bpf (seccomp_gen_ctx, seccomp_fd, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }
  החזר 0;
}

ساكن صحيح
setup_console_socket (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
                      بنية container_entrypoint_s *container_args, صحيح *console_socket_fd,
                      libcrun_error_t *err)
{
  אם (context->console_socket && def->process && def->process->terminal)
    {
      *console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
      אם (UNLIKELY (*console_socket_fd < 0))
        החזר crun_error_wrap (err, "open console socket");
      container_args->console_socket_fd = *console_socket_fd;
    }
  החזר 0;
}

ساكن صحيح
setup_cgroup_manager (libcrun_context_t *context, libcrun_container_t *container,
                      بنية libcrun_cgroup_args *cg, صحيح *cgroup_dirfd,
                      بنية libcrun_dirfd_s *cgroup_dirfd_s, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح cgroup_manager;
  uid_t root_uid = -1;
  gid_t root_gid = -1;
  صحيح ret;

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  אם (context->systemd_cgroup)
    {
      libcrun_debug ("Using systemd cgroup manager");
      cgroup_manager = CGROUP_MANAGER_SYSTEMD;
    }
  אחרת אם (context->force_no_cgroup)
    {
      libcrun_debug ("Disabling cgroup manager");
      cgroup_manager = CGROUP_MANAGER_DISABLED;
    }
  אחרת
    libcrun_debug ("Using cgroupfs cgroup manager");

  /* If we are root (either on the host or in a namespace), then chown the cgroup to root
     in the container user namespace.  */
  get_root_in_the_userns (def, container->host_uid, container->host_gid, &root_uid, &root_gid);
  libcrun_debug ("Using container host UID `%d` and GID `%d`", container->host_uid, container->host_gid);

  memset (cg, 0, حجم (*cg));

  cg->cgroup_path = def->linux ? def->linux->cgroups_path : "";
  cg->manager = cgroup_manager;
  cg->id = context->id;
  cg->resources = def->linux ? def->linux->resources : عدم;
  cg->annotations = container->annotations;
  cg->root_uid = root_uid;
  cg->root_gid = root_gid;
  cg->state_root = context->state_root;
  cg->container = container;

  ret = libcrun_cgroup_preenter (cg, cgroup_dirfd, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  cgroup_dirfd_s->dirfd = cgroup_dirfd;
  cgroup_dirfd_s->joined = false;

  החזר 0;
}

ساكن صحيح
set_scheduler (pid_t pid, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  صحيح ret;

  ret = libcrun_set_scheduler (pid, def->process, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = libcrun_reset_cpu_affinity_mask (pid, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = libcrun_set_io_priority (pid, def->process, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  החזר 0;
}

ساكن صحيح
seccomp_generation (صحيح seccomp_fd, ثابت حرف *seccomp_bpf_data,
                    بنية libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, libcrun_error_t *err)
{
  صحيح ret;

  אם (seccomp_fd >= 0)
    {
      אם (seccomp_bpf_data != عدم)
        {
          ret = libcrun_copy_seccomp (seccomp_gen_ctx, seccomp_bpf_data, err);
          אם (UNLIKELY (ret < 0))
            החזר ret;
        }
      אחרת
        {
          ret = libcrun_generate_seccomp (seccomp_gen_ctx, err);
          אם (UNLIKELY (ret < 0))
            החזר ret;
        }
    }
  החזר 0;
}

ساكن صحيح
terminal_setup (runtime_spec_schema_config_schema *def, libcrun_context_t *context,
                صحيح socket_pair_0, صحيح *terminal_fd, فراغ **orig_terminal, libcrun_error_t *err)
{
  صحيح ret;
  صحيح detach = context->detach;

  אם (def->process && def->process->terminal && ! detach && context->console_socket == عدم)
    {
      libcrun_debug ("Receiving console socket fd");
      *terminal_fd = receive_fd_from_socket (socket_pair_0, err);
      אם (UNLIKELY (*terminal_fd < 0))
        החזר -1;

      ret = libcrun_clear_onlcr (*terminal_fd, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      ret = libcrun_set_raw (0, orig_terminal, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }
  החזר 0;
}

ساكن صحيح
libcrun_container_run_internal (libcrun_container_t *container, libcrun_context_t *context,
                                صحيح *container_ready_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح ret;
  pid_t pid;
  صحيح detach = context->detach;
  cleanup_cgroup_status بنية libcrun_cgroup_status *cgroup_status = عدم;
  cleanup_close صحيح terminal_fd = -1;
  cleanup_terminal فراغ *orig_terminal = عدم;
  cleanup_close صحيح sync_socket = -1;
  cleanup_close صحيح notify_socket = -1;
  cleanup_close صحيح socket_pair_0 = -1;
  cleanup_close صحيح socket_pair_1 = -1;
  cleanup_close صحيح seccomp_fd = -1;
  cleanup_close صحيح console_socket_fd = -1;
  cleanup_close صحيح hooks_out_fd = -1;
  cleanup_close صحيح hooks_err_fd = -1;
  cleanup_close صحيح own_seccomp_receiver_fd = -1;
  cleanup_close صحيح seccomp_notify_fd = -1;
  ثابت حرف *seccomp_notify_plugins = عدم;
  cleanup_cgroup_preenter بنية libcrun_cgroup_args cg = {};
  cleanup_free حرف *bundle_allocated = عدم;
  بنية container_entrypoint_s container_args = {
    .container = container,
    .context = context,
    .bundle = get_bundle_path (context->bundle, &bundle_allocated),
    .terminal_socketpair = { -1, -1 },
    .console_socket_fd = -1,
    .hooks_out_fd = -1,
    .hooks_err_fd = -1,
    .seccomp_receiver_fd = -1,
    .custom_handler = عدم,
  };
  cleanup_close صحيح cgroup_dirfd = -1;
  بنية libcrun_dirfd_s cgroup_dirfd_s;
  بنية libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  ثابت حرف *seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");
  صحيح cgroup_mode;

  אם (! context->force_no_cgroup)
    {
      cgroup_mode = libcrun_get_cgroup_mode (err);
      אם (UNLIKELY (cgroup_mode < 0))
        החזר cgroup_mode;

      אם (cgroup_mode != CGROUP_MODE_UNIFIED)
        libcrun_warning ("cgroup v1 is deprecated and will be removed in a future release.  Use cgroup v2");
    }

  ret = setup_container_hooks_output (container, def, &container_args, &hooks_out_fd, &hooks_err_fd, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  container->context = context;

  אם (! detach || context->notify_socket)
    {
      libcrun_debug ("Setting child subreaper");
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      אם (UNLIKELY (ret < 0))
        החזר crun_make_error (err, errno, "prctl set child subreaper");
    }

  ret = setup_container_keyring (container, context, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = setup_terminal_socket_pair (container, context, &container_args, &socket_pair_0, &socket_pair_1, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = block_signals (err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  umask (0);

  ret = libcrun_set_mempolicy (def, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;
  container_args.seccomp_fd = seccomp_fd;

  אם (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &container_args.seccomp_receiver_fd, &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  ret = setup_console_socket (context, def, &container_args, &console_socket_fd, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = setup_cgroup_manager (context, container, &cg, &cgroup_dirfd, &cgroup_dirfd_s, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = libcrun_configure_handler (container_args.context->handler_manager,
                                   container_args.context,
                                   container,
                                   &(container_args.custom_handler),
                                   err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (container_args.custom_handler && container_args.custom_handler->vtable->modify_oci_configuration)
    {
      libcrun_debug ("Using custom handler to modify OCI configuration");
      ret = container_args.custom_handler->vtable->modify_oci_configuration (container_args.custom_handler->cookie,
                                                                             container_args.context,
                                                                             container->container_def,
                                                                             err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  pid = libcrun_run_linux_container (container, container_init, &container_args, &sync_socket, &cgroup_dirfd_s, err);
  אם (UNLIKELY (pid < 0))
    החזר pid;

  cg.pid = pid;
  cg.joined = cgroup_dirfd_s.joined;
  libcrun_debug ("Running container on PID: `%d`", pid);

  אם (context->fifo_exec_wait_fd < 0 && context->notify_socket)
    {
      libcrun_debug ("Using notify socket: `%s`", context->notify_socket);
      /* Do not open the notify socket here on "create".  "start" will take care of it.  */
      ret = get_notify_fd (context, container, &notify_socket, err);
      אם (UNLIKELY (ret < 0))
        اذهب fail;
    }

  אם (container_args.terminal_socketpair[1] >= 0)
    close_and_reset (&socket_pair_1);

  ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
  אם (UNLIKELY (ret < 0))
    اذهب fail;

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE, err);
  אם (UNLIKELY (ret < 0))
    اذهب fail;

  ret = libcrun_move_network_devices (container, pid, err);
  אם (UNLIKELY (ret < 0))
    اذهب fail;

  /* sync send own pid.  */
  ret = TEMP_FAILURE_RETRY (اكتب (sync_socket, &pid, حجم (pid)));
  אם (UNLIKELY (ret != حجم (pid)))
    {
      אם (ret >= 0)
        errno = 0;
      crun_make_error (err, errno, "write to sync socket");
      اذهب fail;
    }

  /* sync 1.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  אם (UNLIKELY (ret < 0))
    اذهب fail;

  /* sync 2.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  אם (UNLIKELY (ret < 0))
    اذهب fail;

  ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
  אם (UNLIKELY (ret < 0))
    اذهب fail;

  /* If the container process is created later, the scheduler is set on it then.  */
  אם (! libcrun_container_delayed_pidns_fork (container))
    {
      ret = set_scheduler (pid, def, err);
      אם (UNLIKELY (ret < 0))
        اذهب fail;
    }

  /* The container is waiting that we write back.  In this phase we can launch the
     prestart hooks.  */
  אם (def->hooks && def->hooks->prestart_len)
    {
      libcrun_debug ("Running `prestart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "prestart", (hook **) def->hooks->prestart,
                      def->hooks->prestart_len, hooks_out_fd, hooks_err_fd, false, err);
      אם (UNLIKELY (ret != 0))
        اذهب fail;
    }
  אם (def->hooks && def->hooks->create_runtime_len)
    {
      libcrun_debug ("Running `create` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "createRuntime", (hook **) def->hooks->create_runtime,
                      def->hooks->create_runtime_len, hooks_out_fd, hooks_err_fd, false, err);
      אם (UNLIKELY (ret != 0))
        اذهب fail;
    }

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  אם (UNLIKELY (ret < 0))
    اذهب fail;
  close_and_reset (&seccomp_fd);

  /* sync 3.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  אם (UNLIKELY (ret < 0))
    اذهب fail;

  /* The process joining an existing PID namespace is created once the container root is in place.  */
  אם (libcrun_container_delayed_pidns_fork (container))
    {
      pid_t new_pid = 0;

      ret = sync_socket_wait_sync (context, sync_socket, false, err);
      אם (UNLIKELY (ret < 0))
        اذهب fail;

      ret = TEMP_FAILURE_RETRY (اقرأ (sync_socket, &new_pid, حجم (new_pid)));
      אם (UNLIKELY (ret != حجم (new_pid)))
        {
          אם (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "read pid from sync socket");
          اذهب fail;
        }

      /* Cleanup the first process.  */
      waitpid_ignore_stopped (pid, عدم, 0);

      pid = new_pid;

      ret = set_scheduler (pid, def, err);
      אם (UNLIKELY (ret < 0))
        اذهب fail;

      ret = libcrun_handle_pidfd_receiver (pid, container, err);
      אם (UNLIKELY (ret < 0))
        اذهب fail;

      /* sync send own pid.  */
      ret = TEMP_FAILURE_RETRY (اكتب (sync_socket, &pid, حجم (pid)));
      אם (UNLIKELY (ret != حجم (pid)))
        {
          אם (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "write to sync socket");
          اذهب fail;
        }
    }

  ret = terminal_setup (def, context, socket_pair_0, &terminal_fd, &orig_terminal, err);
  אם (UNLIKELY (ret < 0))
    اذهب fail;

  close_and_reset (&socket_pair_0);

  /* sync 4.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  אם (UNLIKELY (ret < 0))
    اذهب fail;

  ret = close_and_reset (&sync_socket);
  אם (UNLIKELY (ret < 0))
    {
      crun_make_error (err, errno, "close sync_socket failed");
      اذهب fail;
    }

  libcrun_debug ("Writing container status");
  ret = write_container_status (container, context, pid, cgroup_status, err);
  אם (UNLIKELY (ret < 0))
    اذهب fail;

  /* Run poststart hooks here only if the container is created using "run".  For create+start, the
     hooks will be executed as part of the start command.  */
  אם (context->fifo_exec_wait_fd < 0 && def->hooks && def->hooks->poststart_len)
    {
      libcrun_debug ("Running `poststart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      אם (UNLIKELY (ret != 0))
        اذهب fail;
    }

  /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
  אם (own_seccomp_receiver_fd >= 0)
    {
      libcrun_debug ("Receiving seccomp fd");
      seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
      אם (UNLIKELY (seccomp_notify_fd < 0))
        اذهب fail;

      ret = close_and_reset (&own_seccomp_receiver_fd);
      אם (UNLIKELY (ret < 0))
        {
          crun_make_error (err, errno, "close seccomp receiver fd failed");
          اذهب fail;
        }
    }

  {
    بنية wait_for_process_args args = {
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
  אם (! context->detach)
    {
      libcrun_error_t tmp_err = عدم;
      cleanup_watch (context, def, cgroup_status, 0, sync_socket, terminal_fd, &tmp_err);
      crun_error_release (&tmp_err);
    }

  החזר ret;

fail:
  ret = cleanup_watch (context, def, cgroup_status, pid, sync_socket, terminal_fd, err);
  אם (cgroup_status)
    {
      libcrun_error_t tmp_err = عدم;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  החזר ret;
}

ساكن صحيح
check_config_file (runtime_spec_schema_config_schema *def, libcrun_context_t *context, libcrun_error_t *err)
{
  אם (UNLIKELY (def->linux == عدم))
    החזר crun_make_error (err, 0, "invalid config file, no `linux` block specified");

  אם (context->handler == عدم)
    {
      אם (UNLIKELY (def->root == عدم))
        החזר crun_make_error (err, 0, "invalid config file, no `root` block specified");
      אם (UNLIKELY (def->mounts == عدم))
        החזר crun_make_error (err, 0, "invalid config file, no `mounts` block specified");
    }
  החזר 0;
}

ساكن صحيح
libcrun_copy_config_file (ثابت حرف *id, ثابت حرف *state_root, libcrun_container_t *container, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_free حرف *dest_path = عدم;
  cleanup_free حرف *dir = عدم;
  cleanup_free حرف *buffer = عدم;
  size_t len;

  ret = libcrun_get_state_directory (&dir, state_root, id, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = append_paths (&dest_path, err, dir, "config.json", عدم);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (container->config_file == عدم && container->config_file_content == عدم)
    החזר crun_make_error (err, 0, "config file not specified");

  אם (container->config_file == عدم)
    {
      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, container->config_file_content, strlen (container->config_file_content), err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }
  אחרת
    {
      libcrun_debug ("Reading config file: `%s`", container->config_file);
      ret = read_all_file (container->config_file, &buffer, &len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, buffer, len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  החזר 0;
}

ساكن فراغ
force_delete_container_status (libcrun_context_t *context, libcrun_container_t *container,
                               runtime_spec_schema_config_schema *def)
{
  libcrun_error_t tmp_err = عدم;
  container_delete_internal (context, container, def, context->id, true, false, &tmp_err);
  libcrun_error_report_and_release (&tmp_err);
}

/* Serialize ERR to FD as an int errno followed by a NUL-terminated message.
   Used by forked children to report a failure to the parent process.  */
ساكن فراغ
write_error_to_pipe (صحيح fd, libcrun_error_t *err)
{
  ثابت حرف *msg = (*err)->msg;
  صحيح errcode = crun_error_get_errno (err);

  TEMP_FAILURE_RETRY (اكتب (fd, &errcode, حجم (errcode)));
  TEMP_FAILURE_RETRY (اكتب (fd, msg, strlen (msg) + 1));
}

صحيح
libcrun_container_run (libcrun_context_t *context, libcrun_container_t *container, unsigned صحيح options,
                       libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح ret;
  صحيح detach = context->detach;
  صحيح container_ret_status[2];
  cleanup_close صحيح pipefd0 = -1;
  cleanup_close صحيح pipefd1 = -1;
  libcrun_error_t tmp_err = عدم;

  container->context = context;

  ret = validate_options (options, LIBCRUN_RUN_OPTIONS_PREFORK | LIBCRUN_RUN_OPTIONS_KEEP, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = check_config_file (def, context, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (def->process && def->process->terminal && detach && context->console_socket == عدم)
    החזר crun_make_error (err, 0, "use --console-socket with --detach when a terminal is used");

  ret = libcrun_status_check_directories (context->state_root, context->id, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (! detach && (options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      ret = libcrun_copy_config_file (context->id, context->state_root, container, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      ret = libcrun_container_run_internal (container, context, عدم, err);
      אם (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
        force_delete_container_status (context, container, def);
      החזר ret;
    }

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  אם (UNLIKELY (ret < 0))
    החזר crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  ret = fork ();
  אם (UNLIKELY (ret < 0))
    החזר crun_make_error (err, errno, "fork");
  אם (ret)
    {
      صحيح status;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, عدم, 0);

      ret = TEMP_FAILURE_RETRY (اقرأ (pipefd0, &status, حجم (status)));
      אם (UNLIKELY (ret < 0))
        החזר crun_make_error (err, errno, "read from sync pipe");

      אם (status < 0)
        {
          صحيح errno_;
          حرف buf[512];
          ret = TEMP_FAILURE_RETRY (اقرأ (pipefd0, &errno_, حجم (errno_)));
          אם (UNLIKELY (ret < 0))
            החזר crun_make_error (err, errno, "read from sync pipe");

          ret = TEMP_FAILURE_RETRY (اقرأ (pipefd0, buf, حجم (buf) - 1));
          אם (UNLIKELY (ret < 0))
            החזר crun_make_error (err, errno, "read from sync pipe");
          buf[ret] = '\0';

          החזר crun_make_error (err, errno_, "%s", buf);
        }

      החזר status;
    }

  close_and_reset (&pipefd0);

  /* forked process.  */
  ret = detach_process ();
  אם (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_root, container, &tmp_err);
  אם (UNLIKELY (ret < 0))
    اذهب fail;

  ret = libcrun_container_run_internal (container, context, عدم, &tmp_err);
  TEMP_FAILURE_RETRY (اكتب (pipefd1, &ret, حجم (ret)));
  אם (UNLIKELY (ret < 0))
    اذهب fail;

  exit (EXIT_SUCCESS);
fail:

  אם (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
    force_delete_container_status (context, container, def);
  אם (tmp_err)
    {
      write_error_to_pipe (pipefd1, &tmp_err);
      crun_error_release (&tmp_err);
    }

  exit (EXIT_FAILURE);
}

صحيح
libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container, unsigned صحيح options,
                          libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح ret;
  صحيح container_ready_pipe[2];
  cleanup_close صحيح pipefd0 = -1;
  cleanup_close صحيح pipefd1 = -1;
  cleanup_close صحيح exec_fifo_fd = -1;
  context->detach = 1;

  libcrun_debug ("Creating container: `%s`", context->id);
  container->context = context;

  ret = validate_options (options, LIBCRUN_CREATE_OPTIONS_PREFORK, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = check_config_file (def, context, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (def->process && def->process->terminal && context->console_socket == عدم)
    החזר crun_make_error (err, 0, "use --console-socket with create when a terminal is used");

  ret = libcrun_status_check_directories (context->state_root, context->id, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  exec_fifo_fd = libcrun_status_create_exec_fifo (context->state_root, context->id, err);
  אם (UNLIKELY (exec_fifo_fd < 0))
    החזר exec_fifo_fd;

  context->fifo_exec_wait_fd = exec_fifo_fd;
  exec_fifo_fd = -1;

  אם ((options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      libcrun_debug ("Running with prefork enabled");
      ret = libcrun_copy_config_file (context->id, context->state_root, container, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
      ret = libcrun_container_run_internal (container, context, عدم, err);
      אם (UNLIKELY (ret < 0))
        force_delete_container_status (context, container, def);
      החזר ret;
    }

  ret = pipe2 (container_ready_pipe, O_CLOEXEC);
  אם (UNLIKELY (ret < 0))
    החזר crun_make_error (err, errno, "pipe");
  pipefd0 = container_ready_pipe[0];
  pipefd1 = container_ready_pipe[1];

  ret = fork ();
  אם (UNLIKELY (ret < 0))
    החזר crun_make_error (err, errno, "fork");
  אם (ret)
    {
      صحيح exit_code;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, عدم, 0);

      ret = TEMP_FAILURE_RETRY (اقرأ (pipefd0, &exit_code, حجم (exit_code)));
      אם (UNLIKELY (ret <= 0))
        החזר crun_make_error (err, ret < 0 ? errno : 0, "waiting for container to be ready");

      אם (ret != حجم (exit_code) || exit_code < 0)
        החזר crun_make_error (err, 0, "internal error: read invalid exit code");

      אם (exit_code == 0)
        החזר 0;

      libcrun_debug ("Exit code is `%d`, deleting container", exit_code);
      libcrun_error_t tmp_err = عدم;
      container_delete_internal (context, عدم, def, context->id, true, true, &tmp_err);
      libcrun_error_report_and_release (&tmp_err);
      החזר crun_make_error (err, 0, "error creating container");
    }

  /* forked process.  */
  ret = detach_process ();
  אם (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_root, container, err);
  אם (UNLIKELY (ret < 0))
    {
      صحيح errcode = crun_error_get_errno (err);
      crun_error_release (err);
      libcrun_fail_with_error (errcode, "copy config file");
    }

  ret = libcrun_container_run_internal (container, context, &pipefd1, err);
  אם (UNLIKELY (ret < 0))
    {
      force_delete_container_status (context, container, def);
      libcrun_error ((*err)->status, "%s", (*err)->msg);
      crun_error_release (err);
      crun_set_output_handler (log_write_to_stderr, عدم);
    }

  אם (pipefd1 >= 0)
    TEMP_FAILURE_RETRY (اكتب (pipefd1, &ret, حجم (ret)));
  exit (ret ? EXIT_FAILURE : 0);
}

صحيح
libcrun_container_start (libcrun_context_t *context, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = عدم;
  ثابت حرف *state_root = context->state_root;
  runtime_spec_schema_config_schema *def;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_close صحيح fd = -1;
  صحيح ret;

  ret = libcrun_read_container_status (&status, state_root, id, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = libcrun_is_container_running (&status, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (! ret)
    החזר crun_make_error (err, 0, "container `%s` is not running", id);

  ret = read_container_config_from_state (&container, state_root, id, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (context->notify_socket)
    {
      ret = get_notify_fd (context, container, &fd, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  ret = libcrun_status_write_exec_fifo (context->state_root, id, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  def = container->container_def;

  אם (context->notify_socket)
    {
      אם (fd >= 0)
        {
          fd_set read_set;

          כלעוד (1)
            {
              بنية timeval timeout = {
                .tv_sec = 0,
                .tv_usec = 10000,
              };
              FD_ZERO (&read_set);
              FD_SET (fd, &read_set);

              ret = select (fd + 1, &read_set, عدم, عدم, &timeout);
              אם (UNLIKELY (ret < 0))
                החזר crun_make_error (err, errno, "select failed");
              אם (ret)
                {
                  ret = handle_notify_socket (fd, err);
                  אם (UNLIKELY (ret < 0))
                    החזר ret;
                  אם (ret)
                    اكسر;
                }
              אחרת
                {
                  ret = libcrun_is_container_running (&status, err);
                  אם (UNLIKELY (ret < 0))
                    החזר ret;
                  אם (! ret)
                    החזר 0;
                }
            }
        }
    }

  /* The container is considered running only after we got the notification from the
     notify_socket, if any.  */
  אם (def->hooks && def->hooks->poststart_len)
    {
      cleanup_close صحيح hooks_out_fd = -1;
      cleanup_close صحيح hooks_err_fd = -1;

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      ret = do_hooks (def, status.pid, context->id, false, status.bundle, status.bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      אם (UNLIKELY (ret != 0))
        {
          libcrun_error_t tmp_err = عدم;
          container_delete_internal (context, container, def, id, true, true, &tmp_err);
          libcrun_error_report_and_release (&tmp_err);
          החזר ret;
        }
    }

  החזר 0;
}

صحيح
libcrun_get_container_state_string (ثابت حرف *id, libcrun_container_status_t *status, ثابت حرف *state_root,
                                    ثابت حرف **container_status, صحيح *running, libcrun_error_t *err)
{
  صحيح ret, has_fifo = 0;
  bool paused = false;

  ret = libcrun_is_container_running (status, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;
  *running = ret;

  אם (*running)
    {
      ret = libcrun_status_has_read_exec_fifo (state_root, id, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
      has_fifo = ret;
    }

  אם (*running && ! has_fifo)
    {
      cleanup_cgroup_status بنية libcrun_cgroup_status *cgroup_status = عدم;

      cgroup_status = libcrun_cgroup_make_status (status);

      ret = libcrun_cgroup_is_container_paused (cgroup_status, &paused, err);
      אם (UNLIKELY (ret < 0))
        {
          /*
            The cgroup might have been cleaned up by the time we try to read it, ignore both
            ENOENT and ENODEV:
            - ENOENT: if the open(CGROUP_PATH) fails because the cgroup was deleted.
            - ENODEV: if the cgroup is deleted between the open and reading the freeze status.
          */

          errno = crun_error_get_errno (err);
          אם (errno == ENOENT || errno == ENODEV)
            {
              crun_error_release (err);
              *container_status = "stopped";
              החזר 0;
            }

          החזר ret;
        }
    }

  אם (! *running)
    *container_status = "stopped";
  אחרת אם (has_fifo)
    *container_status = "created";
  אחרת אם (paused)
    *container_status = "paused";
  אחרת
    *container_status = "running";

  החזר 0;
}

صحيح
libcrun_container_state_json (libcrun_context_t *context, ثابت حرف *id, حرف **out, libcrun_error_t *err)
{
  حرف *buffer = عدم;
  size_t len = 0;
  FILE *f;
  صحيح ret;

  f = open_memstream (&buffer, &len);
  אם (f == عدم)
    החזר crun_make_error (err, errno, "open_memstream");

  ret = libcrun_container_state (context, id, f, err);
  fclose (f);
  אם (UNLIKELY (ret < 0))
    {
      حرر (buffer);
      החזר ret;
    }

  *out = buffer;
  החזר 0;
}

صحيح
libcrun_container_state (libcrun_context_t *context, ثابت حرف *id, FILE *out, libcrun_error_t *err)
{
  ثابت حرف *ثابت OCI_CONFIG_VERSION = "1.0.0";
  libcrun_container_status_t status = {};
  ثابت حرف *state_root = context->state_root;
  ثابت حرف *container_status = عدم;
  json_gen_ctx *gen = عدم;
  ثابت حرف *buf;
  صحيح ret = 0;
  صحيح r = json_gen_status_ok;
  صحيح running;
  size_t len;

  ret = libcrun_read_container_status (&status, state_root, id, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = libcrun_get_container_state_string (id, &status, state_root, &container_status, &running, err);
  אם (UNLIKELY (ret < 0))
    اذهب exit;

  ret = 0;
  אם (! json_gen_init (&gen, عدم))
    החזר crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));
  GEN_KEY (gen, "ociVersion");
  GEN_STR (gen, OCI_CONFIG_VERSION);

  GEN_KEY (gen, "id");
  GEN_STR (gen, id);

  GEN_KEY (gen, "pid");
  GEN_OR_FAIL (map_int (gen, running ? status.pid : 0));

  GEN_KEY (gen, "status");
  GEN_STR (gen, container_status);

  GEN_KEY (gen, "bundle");
  GEN_STR (gen, status.bundle);

  GEN_KEY (gen, "rootfs");
  GEN_STR (gen, status.rootfs);

  GEN_KEY (gen, "created");
  GEN_STR (gen, status.created);

  אם (status.scope)
    {
      GEN_KEY (gen, "systemd-scope");
      GEN_STR (gen, status.scope);
    }

  אם (status.owner)
    {
      GEN_KEY (gen, "owner");
      GEN_STR (gen, status.owner);
    }

  {
    cleanup_container libcrun_container_t *container = عدم;

    ret = read_container_config_from_state (&container, state_root, id, err);
    אם (UNLIKELY (ret < 0))
      اذهب exit;

    אם (container->container_def->annotations && container->container_def->annotations->len)
      GEN_OR_FAIL (gen_annotations (gen, container->container_def->annotations));
  }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  fprintf (out, "%s\n", buf);

exit:
  אם (gen)
    json_gen_free (gen);
  libcrun_free_container_status (&status);
  החזר ret;

gen_error:
  ret = json_gen_error_to_crun_error (r, err);
  اذهب exit;
}

صحيح
libcrun_container_exec (libcrun_context_t *context, ثابت حرف *id, runtime_spec_schema_config_schema_process *process,
                        libcrun_error_t *err)
{
  بنية libcrun_container_exec_options_s opts;
  memset (&opts, 0, حجم (opts));

  opts.struct_size = حجم (opts);
  opts.process = process;

  החזר libcrun_container_exec_with_options (context, id, &opts, err);
}

صحيح
libcrun_container_exec_process_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *path, libcrun_error_t *err)
{
  بنية libcrun_container_exec_options_s opts;
  memset (&opts, 0, حجم (opts));

  opts.struct_size = حجم (opts);
  opts.path = path;

  החזר libcrun_container_exec_with_options (context, id, &opts, err);
}

ساكن صحيح
exec_from_process_json (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *process_json, ثابت حرف *cgroup,
                        bool merge_env, libcrun_error_t *err)
{
  بنية parser_context parser_ctx = { .options = 0, .errfile = stderr };
  بنية libcrun_container_exec_options_s opts;
  runtime_spec_schema_config_schema_process *process;
  parser_error parser_err = عدم;
  json_object *doc = عدم;
  صحيح ret;

  ret = parse_json_file (&doc, process_json, &parser_ctx, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  process = make_runtime_spec_schema_config_schema_process (doc, &parser_ctx, &parser_err);
  json_object_put (doc);
  אם (process == عدم)
    {
      ret = crun_make_error (err, 0, "cannot parse process: `%s`", parser_err ? parser_err : "unknown error");
      حرر (parser_err);
      החזר ret;
    }

  memset (&opts, 0, حجم (opts));
  opts.struct_size = حجم (opts);
  opts.process = process;
  opts.cgroup = cgroup;
  opts.merge_env = merge_env;

  ret = libcrun_container_exec_with_options (context, id, &opts, err);
  free_runtime_spec_schema_config_schema_process (process);
  החזר ret;
}

صحيح
libcrun_container_exec_json (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *process_json, libcrun_error_t *err)
{
  החזר exec_from_process_json (context, id, process_json, عدم, false, err);
}

صحيح
libcrun_container_exec_with_options_json (libcrun_context_t *context, ثابت حرف *id,
                                          ثابت بنية libcrun_exec_options_s *opts, libcrun_error_t *err)
{
  אם (opts == عدم || opts->struct_size == 0)
    החזר crun_make_error (err, EINVAL, "invalid exec options");

  החזר exec_from_process_json (context, id, opts->process_json, opts->cgroup, opts->merge_env, err);
}

#define cleanup_process_schema __attribute__ ((cleanup (cleanup_process_schemap)))

ساكن مضمن فراغ
cleanup_process_schemap (runtime_spec_schema_config_schema_process **p)
{
  runtime_spec_schema_config_schema_process *process = *p;
  אם (process)
    (فراغ) free_runtime_spec_schema_config_schema_process (process);
}

/* Build the seccomp receiver payload (when needed), apply the seccomp profile and
   close the seccomp file descriptors.  Used from exec_process_entrypoint both before
   and after the capabilities are set, depending on process->no_new_privileges.  */
ساكن صحيح
apply_seccomp_for_exec (libcrun_container_t *container, pid_t own_pid, صحيح seccomp_fd,
                        صحيح seccomp_receiver_fd, حرف **seccomp_flags, size_t seccomp_flags_len,
                        libcrun_error_t *err)
{
  cleanup_free حرف *seccomp_fd_payload = عدم;
  size_t seccomp_fd_payload_len = 0;
  صحيح ret;

  אם (seccomp_receiver_fd >= 0)
    {
      ret = get_seccomp_receiver_fd_payload (container, "running", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  ret = libcrun_apply_seccomp (seccomp_fd, seccomp_receiver_fd, seccomp_fd_payload,
                               seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  close_and_reset (&seccomp_fd);
  close_and_reset (&seccomp_receiver_fd);
  החזר 0;
}

ساكن صحيح
exec_process_entrypoint (libcrun_context_t *context,
                         libcrun_container_t *container,
                         runtime_spec_schema_config_schema_process *process,
                         صحيح *pipefd1,
                         صحيح seccomp_fd,
                         صحيح seccomp_receiver_fd,
                         بنية custom_handler_instance_s *custom_handler,
                         bool merge_env,
                         libcrun_error_t *err)
{
  runtime_spec_schema_config_schema_process_capabilities *capabilities = عدم;
  cleanup_free حرف *exec_path = عدم;
  uid_t container_uid;
  gid_t container_gid;
  ثابت حرف *cwd;
  bool chdir_done = false;
  size_t seccomp_flags_len = 0;
  حرف **seccomp_flags = عدم;
  pid_t own_pid = 0;
  size_t i;
  صحيح ret;

  container_uid = process->user ? process->user->uid : 0;
  container_gid = process->user ? process->user->gid : 0;

  TEMP_FAILURE_RETRY (اقرأ (*pipefd1, &own_pid, حجم (own_pid)));

  cwd = process->cwd ? process->cwd : "/";
  אם (LIKELY (libcrun_safe_chdir (cwd, err) == 0))
    chdir_done = true;
  אחרת
    crun_error_release (err);

  ret = unblock_signals (err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = clearenv ();
  אם (UNLIKELY (ret < 0))
    החזר crun_make_error (err, errno, "clearenv");

  אם ((merge_env || ! process->env_len) && container->container_def->process && container->container_def->process->env_len)
    {
      حرف *e;

      עבור (i = 0; i < container->container_def->process->env_len; i++)
        {
          e = container->container_def->process->env[i];
          אם (putenv (e) < 0)
            החזר crun_make_error (err, errno, "putenv `%s`", e);
        }
    }
  אם (process->env_len)
    {
      עבור (i = 0; i < process->env_len; i++)
        אם (putenv (process->env[i]) < 0)
          החזר crun_make_error (err, errno, "putenv `%s`", process->env[i]);
    }

  אם (is_empty_string (getenv ("HOME")))
    {
      ret = set_home_env (container_uid);
      אם (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          אם (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            החזר crun_make_error (err, errno, "setenv HOME");
        }
    }

  ret = libcrun_set_selinux_label (container, process, false, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = libcrun_set_apparmor_profile (container, process, false, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (container->container_def->linux && container->container_def->linux->seccomp)
    {
      seccomp_flags = container->container_def->linux->seccomp->flags;
      seccomp_flags_len = container->container_def->linux->seccomp->flags_len;
    }

  ret = find_executable (&exec_path, process->args[0], process->cwd, err);
  אם (UNLIKELY (ret < 0))
    {
      אם (crun_error_get_errno (err) == ENOENT)
        החזר ret;

      /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
         once the process switched to the use that runs in the container.  This might be necessary
         when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
         is not honored.  */
      crun_error_release (err);
    }

  אם (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, false, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = libcrun_reopen_dev_null (err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (! process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  ret = libcrun_container_setgroups (container, process, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = maybe_chown_std_streams (container_uid, container_gid, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (process->capabilities)
    capabilities = process->capabilities;
  אחרת אם (container->container_def->process)
    capabilities = container->container_def->process->capabilities;

  ret = libcrun_set_caps (capabilities, container_uid, container_gid, process->no_new_privileges,
                          custom_handler != عدم, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (UNLIKELY (exec_path == عدم))
    {
      ret = find_executable (&exec_path, process->args[0], process->cwd, err);
      אם (UNLIKELY (ret < 0))
        {
          אם (custom_handler == عدم || is_empty_string (process->args[0]))
            החזר ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          exec_path = xstrdup (process->args[0]);
        }
    }

  אם (! chdir_done)
    {
      ret = libcrun_safe_chdir (cwd, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  אם (process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  אם (process->user)
    umask (process->user->umask_present ? process->user->umask : 0022);

  ret = 0;
  TEMP_FAILURE_RETRY (اكتب (*pipefd1, &ret, حجم (ret)));
  TEMP_FAILURE_RETRY (close (*pipefd1));
  *pipefd1 = -1;

  אם (custom_handler)
    {
      אם (custom_handler->vtable->exec_func == عدم)
        החזר crun_make_error (err, 0, "the handler does not support exec");

      אם (context->argv)
        erase_initial_environment (context->argv, context->argc);

      ret = custom_handler->vtable->exec_func (custom_handler->cookie,
                                               container,
                                               exec_path,
                                               process->args);
      אם (UNLIKELY (ret < 0))
        החזר crun_make_error (err, -ret, "exec container process failed with handler as `%s`", custom_handler->vtable->name);

      _safe_exit (EXIT_FAILURE);
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation, because the seccomp filter is already in place and it could
     stop some syscalls used by mark_or_close_fds_ge_than.
  */
  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, true, err);
  אם (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, process->args));
  libcrun_fail_with_error (errno, "exec `%s`", exec_path);
}

/* Close the file descriptors inherited from the caller, other than those
   to be passed to the container process.  */
صحيح
libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err)
{
  החזר mark_or_close_fds_ge_than (عدم, context->preserve_fds + 3, true, err);
}

صحيح
libcrun_load_process_from_file (ثابت حرف *path, runtime_spec_schema_config_schema_process **process,
                                libcrun_error_t *err)
{
  بنية parser_context ctx = { 0, stderr };
  cleanup_free حرف *content = عدم;
  parser_error parser_err = عدم;
  json_object *doc = عدم;
  size_t len;
  صحيح ret;

  ret = read_all_file (path, &content, &len, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  *process = make_runtime_spec_schema_config_schema_process (doc, &ctx, &parser_err);
  אם (UNLIKELY (*process == عدم))
    {
      ret = crun_make_error (err, 0, "cannot parse process file: `%s`", parser_err);
      حرر (parser_err);
      אם (doc)
        json_object_put (doc);
      החזר ret;
    }

  حرر (parser_err);
  אם (doc)
    json_object_put (doc);

  החזר 0;
}

صحيح
libcrun_container_exec_with_options (libcrun_context_t *context, ثابت حرف *id,
                                     بنية libcrun_container_exec_options_s *opts,
                                     libcrun_error_t *err)
{
  cleanup_custom_handler_instance بنية custom_handler_instance_s *custom_handler = عدم;
  صحيح container_status, ret;
  bool container_paused = false;
  pid_t pid;
  libcrun_container_status_t status = {};
  ثابت حرف *state_root = context->state_root;
  cleanup_close صحيح terminal_fd = -1;
  cleanup_close صحيح seccomp_fd = -1;
  cleanup_terminal فراغ *orig_terminal = عدم;
  cleanup_container libcrun_container_t *container = عدم;
  صحيح container_ret_status[2];
  cleanup_close صحيح pipefd0 = -1;
  cleanup_close صحيح pipefd1 = -1;
  cleanup_close صحيح seccomp_receiver_fd = -1;
  cleanup_close صحيح own_seccomp_receiver_fd = -1;
  cleanup_close صحيح seccomp_notify_fd = -1;
  ثابت حرف *seccomp_notify_plugins = عدم;
  __attribute__ ((unused)) cleanup_process_schema runtime_spec_schema_config_schema_process *process_cleanup = عدم;
  runtime_spec_schema_config_schema_process *process = opts->process;
  بنية libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  صحيح ret_from_child = 0;

  ret = libcrun_read_container_status (&status, state_root, id, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = libcrun_is_container_running (&status, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;
  container_status = ret;

  ret = read_container_config_from_state (&container, state_root, id, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  container->context = context;

  אם (container_status == 0)
    החזר crun_make_error (err, 0, "the container `%s` is not running", id);

  {
    cleanup_cgroup_status بنية libcrun_cgroup_status *cgroup_status = عدم;

    cgroup_status = libcrun_cgroup_make_status (&status);

    ret = libcrun_cgroup_is_container_paused (cgroup_status, &container_paused, err);
    אם (UNLIKELY (ret < 0))
      החזר ret;
  }

  אם (UNLIKELY (container_paused))
    החזר crun_make_error (err, 0, "cannot exec in a paused container `%s`", id);

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = block_signals (err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  libcrun_seccomp_gen_ctx_init (&seccomp_gen_ctx, container, false, 0);

  ret = libcrun_open_seccomp_bpf (&seccomp_gen_ctx, &seccomp_fd, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &seccomp_receiver_fd,
                                     &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  אם (حجم (*opts) != opts->struct_size)
    החזר crun_make_error (err, EINVAL, "invalid libcrun_container_exec_options_s struct");

  אם (opts->path)
    {
      אם (process)
        החזר crun_make_error (err, EINVAL, "cannot specify both exec file and options");

      ret = libcrun_load_process_from_file (opts->path, &process, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      process_cleanup = process;
    }

  /* This must be done before we enter a user namespace.  If the process does
     not specify any rlimits, use the ones from the container configuration.  */
  אם (process->rlimits_len == 0 && container->container_def->process)
    ret = libcrun_set_rlimits (container->container_def->process->rlimits,
                               container->container_def->process->rlimits_len, err);
  אחרת
    ret = libcrun_set_rlimits (process->rlimits, process->rlimits_len, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = libcrun_set_mempolicy (container->container_def, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  אם (UNLIKELY (ret < 0))
    החזר crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  /* If the new process block doesn't specify a SELinux label, AppArmor profile, user,
     I/O priority or scheduler, then use the configuration from the original config file.  */
  אם (container->container_def->process)
    {
      אם (process->io_priority == عدم && container->container_def->process->io_priority)
        {
          process->io_priority = clone_runtime_spec_schema_config_schema_process_io_priority (container->container_def->process->io_priority);
          אם (process->io_priority == عدم)
            OOM ();
        }

      אם (process->scheduler == عدم && container->container_def->process->scheduler)
        {
          process->scheduler = clone_runtime_spec_schema_config_schema_process_scheduler (container->container_def->process->scheduler);
          אם (process->scheduler == عدم)
            OOM ();
        }

      אם (process->selinux_label == عدم && container->container_def->process->selinux_label)
        process->selinux_label = xstrdup (container->container_def->process->selinux_label);

      אם (process->apparmor_profile == عدم && container->container_def->process->apparmor_profile)
        process->apparmor_profile = xstrdup (container->container_def->process->apparmor_profile);

      אם (process->user == عدم && container->container_def->process->user)
        {
          process->user = clone_runtime_spec_schema_config_schema_process_user (container->container_def->process->user);
          אם (process->user == عدم)
            OOM ();
        }
    }

  ret = initialize_security (container, process, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  /* Do not leak the session keyring of the caller into the container.  The
     keyring created for the container cannot be joined, so use an anonymous one.  */
  אם (! context->no_new_keyring && ! status.no_new_keyring)
    {
      libcrun_debug ("Creating new keyring for the exec process");

      ret = libcrun_create_keyring (container, عدم, process->selinux_label, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
    }

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  אם (UNLIKELY (ret < 0))
    החזר crun_make_error (err, errno, "prctl unset dumpable");

  pid = libcrun_join_process (context, container, status.pid, &status, opts->cgroup, context->detach,
                              process, process->terminal ? &terminal_fd : عدم, err);
  אם (UNLIKELY (pid < 0))
    החזר pid;

  /* Process to exec.  */
  אם (pid == 0)
    {
      TEMP_FAILURE_RETRY (close (pipefd0));
      pipefd0 = -1;

      exec_process_entrypoint (context, container, process, &pipefd1, seccomp_fd, seccomp_receiver_fd, custom_handler, opts->merge_env, err);
      /* It gets here only on errors.  */
      אם (*err)
        {
          אם (pipefd1 < 0)
            libcrun_fail_with_error ((*err)->status, "%s", (*err)->msg);
          אחרת
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

  TEMP_FAILURE_RETRY (اكتب (pipefd0, &pid, حجم (pid)));

  אם (seccomp_fd >= 0)
    close_and_reset (&seccomp_fd);

  אם (terminal_fd >= 0)
    {
      unsigned short rows = 0, cols = 0;

      אם (process->console_size)
        {
          cols = process->console_size->width;
          rows = process->console_size->height;
        }

      ret = libcrun_terminal_setup_size (terminal_fd, rows, cols, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;

      אם (context->console_socket)
        {
          صحيح ret;
          cleanup_close صحيح console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
          אם (UNLIKELY (console_socket_fd < 0))
            החזר console_socket_fd;
          ret = send_fd_to_socket (console_socket_fd, terminal_fd, err);
          אם (UNLIKELY (ret < 0))
            החזר ret;
          close_and_reset (&terminal_fd);
        }
      אחרת
        {
          ret = libcrun_clear_onlcr (terminal_fd, err);
          אם (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              החזר ret;
            }

          ret = libcrun_set_raw (0, &orig_terminal, err);
          אם (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              החזר ret;
            }
        }
    }

  ret = TEMP_FAILURE_RETRY (اقرأ (pipefd0, &ret_from_child, حجم (ret_from_child)));
  אם (ret != حجم (ret_from_child))
    ret = crun_make_error (err, 0, "read pipe failed");
  אחרת אם (ret_from_child != 0)
    {
      cleanup_free حرف *msg = عدم;
      size_t len = 0;

      ret = read_all_fd (pipefd0, "error stream", &msg, &len, err);
      אם (UNLIKELY (ret < 0))
        החזר ret;
      /* the string from read_all_fd is always NUL terminated.  */
      ret = crun_make_error (err, ret_from_child, "%s", msg);
    }
  אחרת
    {
      /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
      אם (own_seccomp_receiver_fd >= 0)
        {
          seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
          אם (UNLIKELY (seccomp_notify_fd < 0))
            החזר seccomp_notify_fd;

          ret = close_and_reset (&own_seccomp_receiver_fd);
          אם (UNLIKELY (ret < 0))
            החזר crun_make_error (err, errno, "close seccomp receiver fd failed");
        }

      {
        بنية wait_for_process_args args = {
          .pid = pid,
          .context = context,
          .terminal_fd = terminal_fd,
          .notify_socket = -1,
          .container_ready_fd = عدم,
          .seccomp_notify_fd = seccomp_notify_fd,
          .seccomp_notify_plugins = seccomp_notify_plugins,
        };

        ret = wait_for_process (&args, err);
      }
    }

  flush_fd_to_err (context, terminal_fd);
  החזר ret;
}

صحيح
libcrun_container_update (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *content, size_t len arg_unused,
                          libcrun_error_t *err)
{
  cleanup_custom_handler_instance بنية custom_handler_instance_s *custom_handler = عدم;
  runtime_spec_schema_config_linux_resources *resources = عدم;
  cleanup_container libcrun_container_t *container = عدم;
  ثابت حرف *state_root = context->state_root;
  بنية parser_context ctx = { 0, stderr };
  libcrun_container_status_t status = {};
  parser_error parser_err = عدم;
  json_object *doc = عدم;
  صحيح ret;

  ret = libcrun_read_container_status (&status, state_root, id, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = read_container_config_from_state (&container, state_root, id, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  resources = make_runtime_spec_schema_config_linux_resources (doc, &ctx, &parser_err);
  אם (UNLIKELY (resources == عدم))
    {
      ret = crun_make_error (err, 0, "cannot parse resources: %s", parser_err);
      اذهب cleanup;
    }

  אם (custom_handler && custom_handler->vtable->modify_oci_configuration)
    {
      /* Adapt RESOURCES to be used from the modify_oci_configuration hook.  */
      cleanup_free runtime_spec_schema_config_linux *linux = xmalloc0 (حجم (*linux));
      cleanup_free runtime_spec_schema_config_schema *def = xmalloc0 (حجم (*def));

      def->linux = linux;
      linux->resources = resources;

      ret = custom_handler->vtable->modify_oci_configuration (custom_handler->cookie,
                                                              context,
                                                              def,
                                                              err);
      אם (UNLIKELY (ret < 0))
        اذهب cleanup;
    }

  ret = libcrun_linux_container_update (&status, state_root, resources, err);

cleanup:
  אם (doc)
    json_object_put (doc);
  حرر (parser_err);
  אם (resources)
    free_runtime_spec_schema_config_linux_resources (resources);

  החזר ret;
}

صحيح
libcrun_container_update_from_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *file, libcrun_error_t *err)
{
  cleanup_free حرف *content = عدم;
  size_t len;
  صحيح ret;

  ret = read_all_file (file, &content, &len, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  החזר libcrun_container_update (context, id, content, len, err);
}

ساكن صحيح
compare_update_values (ثابت فراغ *a, ثابت فراغ *b)
{
  ثابت بنية libcrun_update_value_s *aa = a;
  ثابت بنية libcrun_update_value_s *bb = b;
  صحيح ret;

  ret = strcmp (aa->section, bb->section);
  אם (ret)
    החזר ret;
  החזר strcmp (aa->name, bb->name);
}

صحيح
libcrun_container_update_from_values (libcrun_context_t *context, ثابت حرف *id,
                                      بنية libcrun_update_value_s *values, size_t len,
                                      libcrun_error_t *err)
{
  ثابت حرف *current_section = عدم;
  ثابت حرف *buf;
  json_gen_ctx *gen = عدم;
  size_t i, buf_len;
  صحيح ret;

  אם (! json_gen_init (&gen, عدم))
    החזר crun_make_error (err, 0, "json_gen_init failed");
  json_gen_map_open (gen);

  qsort (values, len, حجم (بنية libcrun_update_value_s), compare_update_values);

  עבור (i = 0; i < len; i++)
    {
      אם (current_section == عدم || strcmp (values[i].section, current_section))
        {
          אם (i > 0)
            json_gen_map_close (gen);

          current_section = values[i].section;
          json_gen_string (gen, current_section, strlen (current_section));
          json_gen_map_open (gen);
        }

      json_gen_string (gen, values[i].name, strlen (values[i].name));

      אם (values[i].numeric)
        json_gen_number (gen, values[i].value, strlen (values[i].value));
      אחרת
        json_gen_string (gen, values[i].value, strlen (values[i].value));
    }
  אם (len)
    json_gen_map_close (gen);

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);

  ret = libcrun_container_update (context, id, buf, buf_len, err);

  json_gen_free (gen);

  החזר ret;
}

ساكن فراغ
populate_array_field (حرف ***field, حرف *array[], size_t num_elements)
{
  size_t i;

  *field = xmalloc0 ((num_elements + 1) * حجم (حرف *));
  עבור (i = 0; i < num_elements; i++)
    (*field)[i] = xstrdup (array[i]);

  (*field)[i] = عدم;
}

#ifdef HAVE_CAP
ساكن فراغ
populate_capabilities (بنية features_info_s *info, حرف ***capabilities, size_t *num_capabilities)
{
  size_t index = 0;
  cap_value_t i;
  حرف *endptr;
  صحيح j;

  *num_capabilities = 0;
  עבור (i = 0;; i++)
    {
      حرف *v = cap_to_name (i);
      אם (v == عدم)
        اكسر;
      strtol (v, &endptr, 10);
      אם (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          اكسر;
        }
      (*num_capabilities)++;
    }

  *capabilities = xmalloc0 ((*num_capabilities + 1) * حجم (ثابت حرف *));
  עבור (i = 0; i < (cap_value_t) *num_capabilities; i++)
    {
      حرف *v = cap_to_name (i);
      אם (v == عدم)
        اكسر;
      strtol (v, &endptr, 10);
      אם (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          اكسر;
        }

      // Convert capability name to uppercase
      עבור (j = 0; v[j] != '\0'; j++)
        v[j] = toupper (v[j]);

      (*capabilities)[index] = v;
      index++;
    }

  (*capabilities)[index] = عدم; // Terminate the array with NULL
  populate_array_field (&(info->linux.capabilities), *capabilities, *num_capabilities);
}
#endif

ساكن فراغ
retrieve_mount_options (بنية features_info_s **info)
{
  cleanup_free ثابت بنية propagation_flags_s *mount_options_list = عدم;
  size_t num_mount_options = 0;

  // Retrieve mount options from wordlist
  mount_options_list = get_mount_flags_from_wordlist ();

  // Calculate the number of mount options
  כלעוד (mount_options_list[num_mount_options].name != عدم)
    num_mount_options++;

  // Allocate memory for mount options in info struct
  (*info)->mount_options = xmalloc0 ((num_mount_options + 1) * حجم (حرف *));

  // Copy mount options to info struct
  עבור (size_t i = 0; i < num_mount_options; i++)
    (*info)->mount_options[i] = xstrdup (mount_options_list[i].name);
}

صحيح
libcrun_container_get_features (libcrun_context_t *context, بنية features_info_s **info, libcrun_error_t *err arg_unused)
{
  // Allocate memory for the features_info_s structure
  size_t num_namspaces = حجم (namespaces) / حجم (namespaces[0]);
  size_t num_operators = حجم (operators) / حجم (operators[0]);
  size_t num_actions = حجم (actions) / حجم (actions[0]);
  size_t num_hooks = حجم (hooks) / حجم (hooks[0]);
  size_t num_archs = حجم (archs) / حجم (archs[0]);
  size_t num_unsafe_annotations = حجم (potentially_unsafe_annotations) / حجم (potentially_unsafe_annotations[0]);
  cleanup_free حرف **capabilities = عدم;
  size_t num_capabilities = 0;
  size_t num_mempolicy_modes = حجم (mempolicy_modes) / حجم (mempolicy_modes[0]);
  size_t num_mempolicy_flags = حجم (mempolicy_flags) / حجم (mempolicy_flags[0]);

  *info = xmalloc0 (حجم (بنية features_info_s));

  // Hardcoded feature information
  (*info)->oci_version_min = xstrdup ("1.0.0");
  (*info)->oci_version_max = xstrdup ("1.1.0+dev");

  // Populate hooks
  populate_array_field (&((*info)->hooks), hooks, num_hooks);

  // Populate mount_options
  retrieve_mount_options (info);

  // Populate namespaces
  populate_array_field (&((*info)->linux.namespaces), namespaces, num_namspaces);

#ifdef HAVE_CAP
  // Populate capabilities
  populate_capabilities (*info, &capabilities, &num_capabilities);
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
  populate_array_field (&((*info)->linux.seccomp.actions), actions, num_actions);
  // Populate operators
  populate_array_field (&((*info)->linux.seccomp.operators), operators, num_operators);
  // Populate archs
  populate_array_field (&((*info)->linux.seccomp.archs), archs, num_archs);
#אחרת
  (*info)->linux.seccomp.enabled = false;
#endif

  // Put values for apparmor and selinux
  (*info)->linux.apparmor.enabled = true;
  (*info)->linux.selinux.enabled = true;

  (*info)->linux.intel_rdt.enabled = true;

  (*info)->linux.net_devices.enabled = true;

  populate_array_field (&((*info)->linux.memory_policy.mode), mempolicy_modes, num_mempolicy_modes);
  populate_array_field (&((*info)->linux.memory_policy.flags), mempolicy_flags, num_mempolicy_flags);

  // Put the values for mount extensions
  (*info)->linux.mount_ext.idmap.enabled = true;

  // Populate the values for annotations
#ifdef HAVE_SECCOMP
  {
    ثابت بنية scmp_version *version = seccomp_version ();
    حرف *version_string = عدم;

    xasprintf (&version_string, "%u.%u.%u", version->major, version->minor, version->micro);
    (*info)->annotations.io_github_seccomp_libseccomp_version = version_string;
  }
#endif

  אם (context->handler_manager && handler_by_name (context->handler_manager, "wasm"))
    (*info)->annotations.run_oci_crun_wasm = true;

#אם HAVE_CRIU
  (*info)->annotations.run_oci_crun_checkpoint_enabled = true;
#endif
  (*info)->annotations.run_oci_crun_commit = GIT_VERSION;
  (*info)->annotations.run_oci_crun_version = PACKAGE_VERSION;

  populate_array_field (&((*info)->potentially_unsafe_annotations), potentially_unsafe_annotations, num_unsafe_annotations);

  החזר 0;
}

ساكن فراغ
features_add_string (json_gen_ctx *json_gen, ثابت حرف *key, ثابت حرف *value)
{
  json_gen_string (json_gen, key, strlen (key));
  json_gen_string (json_gen, value, strlen (value));
}

ساكن فراغ
features_add_bool (json_gen_ctx *json_gen, ثابت حرف *key, صحيح value)
{
  json_gen_string (json_gen, key, strlen (key));
  json_gen_bool (json_gen, value);
}

ساكن فراغ
features_add_bool_str (json_gen_ctx *json_gen, ثابت حرف *key, صحيح value)
{
  features_add_string (json_gen, key, value ? "true" : "false");
}

ساكن فراغ
features_add_array (json_gen_ctx *json_gen, ثابت حرف *key, حرف **array)
{
  size_t i;

  json_gen_string (json_gen, key, strlen (key));
  json_gen_array_open (json_gen);
  אם (array)
    עבור (i = 0; array[i] != عدم; i++)
      json_gen_string (json_gen, array[i], strlen (array[i]));
  json_gen_array_close (json_gen);
}

ساكن فراغ
features_add_linux_info (json_gen_ctx *json_gen, ثابت بنية linux_info_s *linux)
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
  אם (linux->seccomp.actions)
    features_add_array (json_gen, "actions", linux->seccomp.actions);
  אם (linux->seccomp.operators)
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
  אם (linux->memory_policy.mode)
    features_add_array (json_gen, "modes", linux->memory_policy.mode);
  אם (linux->memory_policy.flags)
    features_add_array (json_gen, "flags", linux->memory_policy.flags);
  json_gen_map_close (json_gen);

  json_gen_map_close (json_gen);
}

ساكن فراغ
features_add_annotations_info (json_gen_ctx *json_gen, ثابت بنية annotations_info_s *annotation)
{
  json_gen_string (json_gen, "annotations", strlen ("annotations"));
  json_gen_map_open (json_gen);

  אם (! is_empty_string (annotation->io_github_seccomp_libseccomp_version))
    features_add_string (json_gen, "io.github.seccomp.libseccomp.version", annotation->io_github_seccomp_libseccomp_version);

  features_add_bool_str (json_gen, "org.opencontainers.runc.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);
  features_add_bool_str (json_gen, "run.oci.crun.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);

  features_add_string (json_gen, "run.oci.crun.commit", annotation->run_oci_crun_commit);
  features_add_string (json_gen, "run.oci.crun.version", annotation->run_oci_crun_version);

  features_add_bool_str (json_gen, "run.oci.crun.wasm", annotation->run_oci_crun_wasm);

  json_gen_map_close (json_gen);
}

صحيح
libcrun_container_get_features_json (libcrun_context_t *context, حرف **out, libcrun_error_t *err)
{
  cleanup_struct_features بنية features_info_s *info = عدم;
  ثابت حرف *buffer = عدم;
  json_gen_ctx *json_gen;
  size_t buffer_len;
  صحيح ret;

  *out = عدم;

  ret = libcrun_container_get_features (context, &info, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (! json_gen_init (&json_gen, عدم))
    החזר crun_make_error (err, 0, "cannot initialize JSON generator");

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

  החזר 0;
}

/* Read the container status and fail if the container is not running.  */
ساكن صحيح
read_status_require_running (libcrun_context_t *context, ثابت حرف *id,
                             libcrun_container_status_t *status, libcrun_error_t *err)
{
  صحيح ret;

  ret = libcrun_read_container_status (status, context->state_root, id, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = libcrun_is_container_running (status, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;
  אם (ret == 0)
    החזר crun_make_error (err, 0, "the container `%s` is not running", id);

  החזר 0;
}

صحيح
libcrun_container_pause (libcrun_context_t *context, ثابت حرف *id, libcrun_error_t *err)
{
  صحيح ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  החזר libcrun_container_pause_linux (&status, err);
}

صحيح
libcrun_container_unpause (libcrun_context_t *context, ثابت حرف *id, libcrun_error_t *err)
{
  صحيح ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  החזר libcrun_container_unpause_linux (&status, err);
}

/* Translate the public options struct into the internal representation used by
   the linux/CRIU layer.  Strings are borrowed (not copied).  */
ساكن فراغ
cr_options_from_public (libcrun_checkpoint_restore_t *dst, بنية libcrun_checkpoint_restore_options_s *src)
{
  memset (dst, 0, حجم (*dst));
  dst->image_path = (حرف *) src->image_path;
  dst->work_path = (حرف *) src->work_path;
  dst->parent_path = (حرف *) src->parent_path;
  dst->console_socket = src->console_socket;
  dst->lsm_profile = (حرف *) src->lsm_profile;
  dst->lsm_mount_context = (حرف *) src->lsm_mount_context;
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

صحيح
libcrun_container_checkpoint (libcrun_context_t *context, ثابت حرف *id,
                              بنية libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  صحيح ret;
  ثابت حرف *state_root = context->state_root;
  libcrun_container_status_t status = {};
  libcrun_checkpoint_restore_t cr;
  cleanup_container libcrun_container_t *container = عدم;

  cr_options_from_public (&cr, cr_options);

  ret = read_status_require_running (context, id, &status, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = read_container_config_from_state (&container, state_root, id, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  /* Needed to use the same paths (e.g. the shared empty directory used for
     masked paths) as the container runtime and restore do.  */
  container->context = context;

  ret = libcrun_container_checkpoint_linux (&status, container, &cr, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (! (cr_options->leave_running || cr_options->pre_dump))
    החזר container_delete_internal (context, عدم, عدم, id, true, true, err);

  החזר 0;
}

/* The dummy process only keeps the cgroup populated until the container is restored there.
   Its cgroup is below the container cgroup, so once the container runs it can freeze or kill
   the dummy process: never depend on it after that point.  */
ساكن فراغ
restore_proxy_process (صحيح proxy_pid_pipe)
{
  حرف c;

  /* Wait to be killed by the parent, or for the parent to go away.  */
  TEMP_FAILURE_RETRY (اقرأ (proxy_pid_pipe, &c, حجم (c)));
}

ساكن صحيح
restore_terminate_proxy_process (pid_t proxy_pid, ثابت حرف *proxy_cgroup, pid_t container_pid, صحيح cgroup_manager, libcrun_error_t *err)
{
  صحيح mode;
  صحيح ret;

  אם (cgroup_manager == CGROUP_MANAGER_SYSTEMD)
    {
      حرف ready_str[64];

      ret = snprintf (ready_str, حجم (ready_str), "MAINPID=%d", container_pid);
      אם (UNLIKELY (ret >= (صحيح) حجم (ready_str)))
        החזר crun_make_error (err, 0, "internal error: static buffer too small");

      ret = send_sd_notify (ready_str, err);
      /* Do not fail on errors.  */
      אם (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  mode = libcrun_get_cgroup_mode (err);
  אם (UNLIKELY (mode < 0))
    החזר mode;

  /* SIGKILL terminates a process frozen with cgroup v2, while the cgroup v1 freezer does not
     deliver it until the cgroup is thawed, so do not wait for it there.  */
  kill (proxy_pid, SIGKILL);
  TEMP_FAILURE_RETRY (waitpid (proxy_pid, عدم, mode == CGROUP_MODE_UNIFIED ? 0 : WNOHANG));

  החזר destroy_cgroup_path (proxy_cgroup, mode, err);
}

صحيح
libcrun_container_restore (libcrun_context_t *context, ثابت حرف *id arg_unused,
                           بنية libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  cleanup_cgroup_status بنية libcrun_cgroup_status *cgroup_status = عدم;
  cleanup_container libcrun_container_t *container = عدم;
  cleanup_close صحيح proxy_pid_pipe0 = -1;
  cleanup_close صحيح proxy_pid_pipe1 = -1;
  runtime_spec_schema_config_schema *def;
  libcrun_container_status_t status = {};
  بنية libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  libcrun_checkpoint_restore_t cr;
  cleanup_pid pid_t proxy_pid = -1;
  cleanup_close صحيح seccomp_fd = -1;
  ثابت حرف *seccomp_bpf_data;
  صحيح proxy_pid_pipe[2];
  صحيح cgroup_manager;
  uid_t root_uid = -1;
  gid_t root_gid = -1;
  صحيح ret;

  cr_options_from_public (&cr, cr_options);

  container = libcrun_container_load_from_file ("config.json", err);
  אם (container == عدم)
    החזר -1;

  container->context = context;
  def = container->container_def;

  ret = check_config_file (def, context, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = libcrun_status_check_directories (context->state_root, context->id, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = libcrun_copy_config_file (context->id, context->state_root, container, err);
  אם (UNLIKELY (ret < 0))
    اذهب fail;

  /* CRIU restores the seccomp filters of the checkpointed processes, but
     exec loads the filter from the seccomp.bpf file in the state directory,
     so it must be generated again for the restored container.  */
  seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  אם (UNLIKELY (ret < 0))
    اذهب fail;

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  אם (UNLIKELY (ret < 0))
    اذهب fail;
  close_and_reset (&seccomp_fd);

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  אם (context->systemd_cgroup)
    cgroup_manager = CGROUP_MANAGER_SYSTEMD;
  אחרת אם (context->force_no_cgroup)
    cgroup_manager = CGROUP_MANAGER_DISABLED;

  def = container->container_def;

  /* If we are root (either on the host or in a namespace),
   * then chown the cgroup to root in the container user namespace. */
  get_root_in_the_userns (def, container->host_uid, container->host_gid, &root_uid, &root_gid);

  /* If the root in the container is different than the current root user, attempt to chown
     the std streams before entering the user namespace.  Otherwise we might lose access
     to the user (as it is not mapped in the user namespace) and cannot chown them.  */
  אם (root_uid > 0 || root_gid > 0)
    {
      ret = maybe_chown_std_streams (root_uid, root_gid, err);
      אם (UNLIKELY (ret < 0))
        اذهب fail;
    }

  /* CRIU runs as a child process, so the restored processes would inherit the session keyring.  */
  ret = setup_container_keyring (container, context, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  {
    cleanup_close صحيح cgroup_dirfd = -1;
    bool needs_proxy_process;
    cleanup_cgroup_preenter بنية libcrun_cgroup_args cg = {
      .resources = def->linux ? def->linux->resources : عدم,
      .annotations = container->annotations,
      .cgroup_path = def->linux ? def->linux->cgroups_path : "",
      .manager = cgroup_manager,
      .root_uid = root_uid,
      .root_gid = root_gid,
      .id = context->id,
      .state_root = context->state_root,
      .container = container,
    };

    /* The CRIU restore code uses bundle, rootfs and cgroup_path of status.   The cgroup_path is set later.  */
    status.bundle = (حرف *) context->bundle;
    status.rootfs = def->root->path;

    ret = libcrun_cgroup_preenter (&cg, &cgroup_dirfd, err);
    אם (UNLIKELY (ret < 0))
      اذهب fail;

    needs_proxy_process = cgroup_dirfd < 0;

    /* If the target cgroup was already created (with cgroupfs), we can restore the container directly there.  */
    אם (! needs_proxy_process)
      {
        cleanup_free حرف *cgroup_path = عدم;

        ret = get_cgroup_dirfd_path (cgroup_dirfd, &cgroup_path, err);
        אם (UNLIKELY (ret < 0))
          اذهب fail;

        /* Complete the configuration for the cgroup before the container is
           restored there: CRIU resumes the restored processes before it
           returns, so the device filter and the limits must already be in
           place.  There is no process to move, as CRIU is run in the cgroup,
           and the controllers were enabled by precreate_cgroup.  */
        cg.joined = true;

        ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
        אם (UNLIKELY (ret < 0))
          اذهب fail;

        /* Restore the container directly in the desired cgroup.  */
        status.cgroup_path = cgroup_path;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        אם (UNLIKELY (ret < 0))
          اذهب fail;

        /* Now that the container is restored, keep any signal for it, so that
           it is forwarded to the init by wait_for_process, rather than acted
           upon by crun.  This cannot be done earlier, as CRIU, which is run as
           a child process, would inherit the blocked signals.  */
        ret = block_signals (err);
        אם (UNLIKELY (ret < 0))
          اذهب fail;

        /* Use the container first process PID to finalize the cgroup.  */
        cg.pid = status.pid;
      }
    אחרת
      {
        /* When using systemd, we need a process first that must exist before a cgroup is created.
           Create a dummy process that sits and waits until the container is restored.  */
        ret = pipe2 (proxy_pid_pipe, O_CLOEXEC);
        אם (UNLIKELY (ret < 0))
          {
            ret = crun_make_error (err, errno, "pipe");
            اذهب fail;
          }

        /* Only used for auto cleanup.  */
        proxy_pid_pipe0 = proxy_pid_pipe[0];
        proxy_pid_pipe1 = proxy_pid_pipe[1];

        proxy_pid = fork ();
        אם (UNLIKELY (proxy_pid < 0))
          {
            ret = crun_make_error (err, errno, "fork");
            اذهب fail;
          }

        אם (proxy_pid == 0)
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
        אם (UNLIKELY (ret < 0))
          اذهب fail;
      }

    ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
    אם (UNLIKELY (ret < 0))
      اذهب fail;

    /* When using a dummy process, the container is restored here once the cgroup is configured.  */
    אם (needs_proxy_process)
      {
        cleanup_free حرف *target_cgroup = عدم;
        cleanup_free حرف *proxy_cgroup = عدم;

        ret = libcrun_get_cgroup_process (proxy_pid, &target_cgroup, false, err);
        אם (UNLIKELY (ret < 0))
          اذهب fail;

        /* Move the dummy process to a sub-cgroup.  */
        ret = append_paths (&proxy_cgroup, err, target_cgroup, ".proxy-crun", عدم);
        אם (UNLIKELY (ret < 0))
          اذهب fail;

        ret = libcrun_move_process_to_cgroup (proxy_pid, 0, proxy_cgroup, true, err);
        אם (UNLIKELY (ret < 0))
          اذهب fail;

        /* Restore the container in the same cgroup where the dummy process was.  */
        status.cgroup_path = target_cgroup;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        אם (UNLIKELY (ret < 0))
          اذهب fail;

        /* See the comment for the other libcrun_container_restore_linux call.  */
        ret = block_signals (err);
        אם (UNLIKELY (ret < 0))
          اذهب fail;

        /* The container is running, so do not fail because of the dummy process.  */
        ret = restore_terminate_proxy_process (proxy_pid, proxy_cgroup, status.pid, cgroup_manager, err);
        אם (UNLIKELY (ret < 0))
          crun_error_release (err);
        proxy_pid = -1;

        close_and_reset (&proxy_pid_pipe1);
      }
  }

  context->detach = cr_options->detach;
  ret = write_container_status (container, context, status.pid, cgroup_status, err);
  אם (UNLIKELY (ret < 0))
    اذهب fail;

  {
    /* Wait the same way run and exec do: apart from waiting for the container
       init, this writes the pid file, forwards any signal to the init, and
       reaps every child, not only the init.  */
    بنية wait_for_process_args args = {
      .pid = status.pid,
      .context = context,
      .terminal_fd = -1,
      .notify_socket = -1,
      .container_ready_fd = عدم,
      .seccomp_notify_fd = -1,
      .seccomp_notify_plugins = عدم,
    };

    החזר wait_for_process (&args, err);
  }

fail:
  /* Once restored, the container is running.  Without a status file nothing
     can reach it anymore, so do not leave it behind.  */
  אם (status.pid > 0)
    {
      kill (status.pid, SIGKILL);
      waitpid_ignore_stopped (status.pid, عدم, 0);
    }
  אם (cgroup_status)
    {
      libcrun_error_t tmp_err = عدم;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  {
    libcrun_error_t tmp_err = عدم;

    אם (libcrun_container_delete_status (context->state_root, context->id, &tmp_err) < 0)
      crun_error_release (&tmp_err);
  }
  החזר ret;
}

صحيح
libcrun_container_read_pids (libcrun_context_t *context, ثابت حرف *id, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  cleanup_cgroup_status بنية libcrun_cgroup_status *cgroup_status = عدم;
  cleanup_container_status libcrun_container_status_t status = {};
  صحيح ret;

  ret = libcrun_read_container_status (&status, context->state_root, id, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (status.cgroup_path == عدم || status.cgroup_path[0] == '\0')
    החזר crun_make_error (err, 0, "the container is not using cgroups");

  cgroup_status = libcrun_cgroup_make_status (&status);

  החזר libcrun_cgroup_read_pids (cgroup_status, recurse, pids, err);
}

صحيح
libcrun_container_list_json (libcrun_context_t *context, حرف **out, libcrun_error_t *err)
{
  libcrun_container_list_t *list = عدم, *it;
  ثابت حرف *content = عدم;
  json_gen_ctx *gen = عدم;
  size_t len;
  صحيح ret;

  *out = عدم;

  ret = libcrun_get_containers_list (&list, context->state_root, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (! json_gen_init (&gen, عدم))
    {
      ret = crun_make_error (err, 0, "cannot allocate json generator");
      اذهب exit;
    }

  json_gen_config (gen, json_gen_beautify, 1);
  json_gen_array_open (gen);

  עבור (it = list; it; it = it->next)
    {
      libcrun_container_status_t status;
      صحيح running = 0;
      صحيح pid;
      ثابت حرف *container_status = عدم;

      ret = libcrun_read_container_status (&status, context->state_root, it->name, err);
      אם (UNLIKELY (ret < 0))
        اذهب exit;

      pid = status.pid;

      ret = libcrun_get_container_state_string (it->name, &status, context->state_root, &container_status,
                                                &running, err);
      אם (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          استمر;
        }

      אם (! running)
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
  אם (json_gen_get_buf (gen, &content, &len) != json_gen_status_ok)
    {
      ret = libcrun_make_error (err, 0, "cannot generate json list");
      اذهب exit;
    }

  *out = xstrdup (content);
  ret = 0;

exit:
  אם (list)
    libcrun_free_containers_list (list);
  אם (gen)
    json_gen_free (gen);

  החזר ret;
}

صحيح
libcrun_write_json_containers_list (libcrun_context_t *context, FILE *out, libcrun_error_t *err)
{
  cleanup_free حرف *content = عدم;
  size_t len;
  صحيح ret;

  ret = libcrun_container_list_json (context, &content, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  len = strlen (content);
  אם (len && fwrite (content, 1, len, out) != len)
    החזר crun_make_error (err, errno, "error writing to file");

  החזר 0;
}

صحيح
libcrun_container_update_intel_rdt (libcrun_context_t *context, ثابت حرف *id, بنية libcrun_intel_rdt_update *update, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = عدم;
  صحيح ret;

  ret = read_container_config_from_state (&container, context->state_root, id, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  החזר libcrun_update_intel_rdt (id, container, update->l3_cache_schema, update->mem_bw_schema, update->schemata, err);
}

ساكن صحيح
libcrun_container_add_or_remove_mounts_from_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *file, bool add, libcrun_error_t *err)
{
  cleanup_custom_handler_instance بنية custom_handler_instance_s *custom_handler = عدم;
  cleanup_container libcrun_container_t *container = عدم;
  cleanup_free runtime_spec_schema_defs_mount **mounts = عدم;
  ثابت حرف *state_root = context->state_root;
  cleanup_free parser_error parser_err = عدم;
  بنية parser_context pctx = { 0, stderr };
  libcrun_container_status_t status = {};
  cleanup_free حرف *content = عدم;
  json_object *doc = عدم;
  size_t n_mounts = 0, len, i;
  صحيح ret = 1;

  ret = read_all_file (file, &content, &len, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = libcrun_read_container_status (&status, state_root, id, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = read_container_config_from_state (&container, state_root, id, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  ret = parse_json_file (&doc, content, &pctx, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  אם (! json_object_is_type (doc, json_type_array))
    {
      ret = crun_make_error (err, 0, "mounts must be an array");
      اذهب cleanup;
    }
  אחרת
    {
      n_mounts = json_object_array_length (doc);

      mounts = xmalloc0 ((n_mounts + 1) * حجم (*mounts));
      עבור (i = 0; i < n_mounts; i++)
        {
          mounts[i] = make_runtime_spec_schema_defs_mount (json_object_array_get_idx (doc, i), &pctx, &parser_err);
          אם (mounts[i] == عدم)
            {
              ret = crun_make_error (err, 0, "cannot parse mount: %s", parser_err);
              اذهب cleanup;
            }
        }
    }

  אם (add)
    ret = libcrun_make_runtime_mounts (container, &status, mounts, n_mounts, err);
  אחרת
    ret = libcrun_destroy_runtime_mounts (container, &status, mounts, n_mounts, err);

cleanup:
  אם (doc)
    json_object_put (doc);
  אם (mounts)
    עבור (i = 0; i < n_mounts; i++)
      free_runtime_spec_schema_defs_mount (mounts[i]);
  החזר ret;
}

صحيح
libcrun_container_add_mounts_from_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *file, libcrun_error_t *err)
{
  החזר libcrun_container_add_or_remove_mounts_from_file (context, id, file, true, err);
}

صحيح
libcrun_container_remove_mounts_from_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *file, libcrun_error_t *err)
{
  החזר libcrun_container_add_or_remove_mounts_from_file (context, id, file, false, err);
}
