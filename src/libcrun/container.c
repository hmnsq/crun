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
#تعریف_کرو GNU_ماخذ

#شامل <config.h>
#شامل <ocispec/runtime_spec_schema_config_schema.h>
#شامل <stdbool.h>
#شامل "container.h"
#شامل "utils.h"
#شامل "json_gen_utils.h"
#شامل "seccomp.h"
#شامل "mempolicy.h"
#اگر_تعریف_شدہ HAVE_SECCOMP
#  include <seccomp.h>
#اگر_ختم
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
#اگر_تعریف_شدہ HAVE_CAP
#  include <sys/capability.h>
#اگر_ختم
#شامل <sys/ioctl.h>
#شامل <termios.h>
#شامل <grp.h>
#شامل <git-version.h>

#اگر_تعریف_شدہ HAVE_SYSTEMD
#  include <systemd/sd-daemon.h>
#اگر_ختم

#شامل <json-c/json.h>
#شامل <ocispec/json_common.h>

شمار
{
  SYNC_SOCKET_SYNC_پیغام,
  SYNC_SOCKET_ERROR_پیغام,
  SYNC_SOCKET_WARNING_پیغام,
  SYNC_SOCKET_DEBUG_پیغام,
};

ساخت container_entrypoint_s
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

  صحیح hooks_برآمد_fd;
  صحیح hooks_err_fd;

  /* Absolute bundle path, to be reported to hooks run in the container.  */
  ثابت حرف *bundle;

  ساخت custom_handler_instance_s *custom_handler;
};

ساخت sync_socket_پیغام_s
{
  صحیح type;
  صحیح error_قدر;
  حرف پیغام_2[512];
};

قسمتعریف runtime_spec_schema_defs_hook hook;

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
ساکن حرف *namespaces[] = {
  "cgroup",
  "ipc",
  "mount",
  "network",
  "pid",
  "user",
  "uts"
};

ساکن حرف *actions[] = {
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

ساکن حرف *operators[] = {
  "SCMP_CMP_NE",
  "SCMP_CMP_LT",
  "SCMP_CMP_LE",
  "SCMP_CMP_EQ",
  "SCMP_CMP_GE",
  "SCMP_CMP_GT",
  "SCMP_CMP_MASKED_EQ",
};

ساکن حرف *archs[] = {
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

ساکن حرف *mempolicy_modes[] = {
  "MPOL_DEFAULT",
  "MPOL_PREFERRED",
  "MPOL_BIND",
  "MPOL_INTERLEAVE",
  "MPOL_LOCAL",
  "MPOL_PREFERRED_MANY",
  "MPOL_WEIGHTED_INTERLEAVE"
};

ساکن حرف *mempolicy_flags[] = {
  "MPOL_F_NUMA_BALANCING",
  "MPOL_F_RELATIVE_NODES",
  "MPOL_F_STATIC_NODES"
};

ساکن حرف *potentially_unsafe_annotations[] = {
  "module.wasm.image/variant",
  "io.kubernetes.cri.container-type",
  "run.oci.",
  "org.criu.",
  "krun.",
};

#تعریف_کرو SYNC_SOCKET_پیغام_LEN(x, l) (offsetof (ساخت sync_socket_پیغام_s, پیغام_2) + l)

ساکن صحیح
sync_socket_write_پیغام (صحیح fd, صحیح verbosity, صحیح err_قدر, ثابت حرف *log_پیغام)
{
  صحیح ret;
  size_t err_len;
  ساخت sync_socket_پیغام_s پیغام;
  انتخاب (verbosity)
    {
    حالت LIBCRUN_VERBOSITY_DEBUG:
      پیغام.type = SYNC_SOCKET_DEBUG_پیغام;
      توڑو;
    حالت LIBCRUN_VERBOSITY_WARNING:
      پیغام.type = SYNC_SOCKET_WARNING_پیغام;
      توڑو;
    حالت LIBCRUN_VERBOSITY_ERROR:
      پیغام.type = SYNC_SOCKET_ERROR_پیغام;
      توڑو;
    }
  پیغام.error_قدر = err_قدر;

  اگر (fd < 0)
    واپس 0;

  err_len = لڑی_لمبائی (log_پیغام);
  اگر (err_len >= حجم (پیغام.پیغام_2))
    err_len = حجم (پیغام.پیغام_2) - 1;

  حافظہ_نقل (پیغام.پیغام_2, log_پیغام, err_len);
  پیغام.پیغام_2[err_len] = '\0';

  ret = عارضی_FAILURE_RETRY (اكتب (fd, &پیغام, SYNC_SOCKET_پیغام_LEN (پیغام, err_len + 1)));
  اگر (UNLIKELY (ret < 0))
    واپس -1;

  واپس 0;
}

ساکن صحیح
sync_socket_write_error (صحیح fd, libcrun_error_t *برآمد_err)
{
  اگر (fd < 0)
    واپس 0;
  واپس sync_socket_write_پیغام (fd, false, (*برآمد_err)->حالت_2, (*برآمد_err)->پیغام);
}

ساکن خالی
log_write_مقصد_sync_socket (صحیح errno_, ثابت حرف *پیغام, صحیح verbosity, خالی *arg)
{
  ساخت container_entrypoint_s *entrypoint_دلائل = arg;
  صحیح fd = entrypoint_دلائل->sync_socket;

  اگر (fd < 0)
    واپس;

  اگر (sync_socket_write_پیغام (fd, verbosity, errno_, پیغام) < 0)
    log_write_مقصد_stderr (errno_, پیغام, verbosity, arg);
}

ساکن bool
is_حافظہ_حد_بندی_too_کم (runtime_spec_schema_config_schema *def)
{
  ثابت طویل حافظہ_حد_بندی_too_کم = 1024 * 1024;

  اگر (def->linux == تہی || def->linux->resources == تہی)
    واپس false;

  اگر (def->linux->resources->حافظہ
      && def->linux->resources->حافظہ->حد_بندی_present
      && def->linux->resources->حافظہ->حد_بندی < حافظہ_حد_بندی_too_کم)
    واپس true;

  اگر (def->linux->resources->unified)
    {
      size_t i;

      برائے (i = 0; i < def->linux->resources->unified->len; i++)
        اگر (strcmp (def->linux->resources->unified->کلیدیں[i], "memory.max") == 0)
          {
            طویل حد_بندی;

            errno = 0;
            حد_بندی = strtol (def->linux->resources->unified->اقدار[i], تہی, 10);
            اگر (errno != 0)
              واپس false;
            اگر (حد_بندی < حافظہ_حد_بندی_too_کم)
              واپس true;
          }
    }

  واپس false;
}

ساکن صحیح
sync_socket_wait_sync (libcrun_context_t *context, صحیح fd, bool flush, libcrun_error_t *err)
{
  ساخت sync_socket_پیغام_s پیغام;

  اگر (fd < 0)
    واپس 0;

  جب (true)
    {
      صحیح ret;

      ret = عارضی_FAILURE_RETRY (اقرأ (fd, &پیغام, حجم (پیغام)));
      اگر (UNLIKELY (ret < 0))
        {
          اگر (flush)
            واپس 0;
          واپس crun_make_error (err, errno, "read from sync socket");
        }

      اگر (ret == 0)
        {
          اگر (flush)
            واپس 0;

          واپس crun_make_error (err, 0, "read from the init process");
        }

      اگر (! flush && پیغام.type == SYNC_SOCKET_SYNC_پیغام)
        واپس 0;
      ورنہ اگر (پیغام.type == SYNC_SOCKET_DEBUG_پیغام)
        {
          اگر (context)
            context->output_handler (پیغام.error_قدر, پیغام.پیغام_2, LIBCRUN_VERBOSITY_DEBUG, context->output_handler_arg);
          جاری;
        }
      ورنہ اگر (پیغام.type == SYNC_SOCKET_WARNING_پیغام)
        {
          اگر (context)
            context->output_handler (پیغام.error_قدر, پیغام.پیغام_2, LIBCRUN_VERBOSITY_WARNING, context->output_handler_arg);
          جاری;
        }
      ورنہ اگر (پیغام.type == SYNC_SOCKET_ERROR_پیغام)
        واپس crun_make_error (err, پیغام.error_قدر, "%s", پیغام.پیغام_2);
    }
}

ساکن صحیح
sync_socket_send_sync (صحیح fd, bool flush_errors, libcrun_error_t *err)
{
  صحیح ret;
  ساخت sync_socket_پیغام_s پیغام = {
    0,
  };
  پیغام.type = SYNC_SOCKET_SYNC_پیغام;

  اگر (fd < 0)
    واپس 0;

  ret = عارضی_FAILURE_RETRY (اكتب (fd, &پیغام, SYNC_SOCKET_پیغام_LEN (پیغام, 0)));
  اگر (UNLIKELY (ret < 0))
    {
      اگر (flush_errors)
        {
          صحیح saved_errno = errno;
          ret = عارضی_FAILURE_RETRY (اقرأ (fd, &پیغام, حجم (پیغام)));
          اگر (ret >= 0 && پیغام.type == SYNC_SOCKET_ERROR_پیغام)
            واپس crun_make_error (err, پیغام.error_قدر, "%s", پیغام.پیغام_2);
          errno = saved_errno;
        }
      واپس crun_make_error (err, errno, "write to sync socket");
    }

  واپس 0;
}

ساکن bool
has_cap_sys_ptrace ()
{
#اگر_تعریف_شدہ HAVE_CAP
  ساخت __user_cap_header_struct hdr = { LINUX_CAPABILITY_نسخہ_3, 0 };
  ساخت __user_cap_data_struct ڈیٹا[2] = { { 0 } };

  اگر (UNLIKELY (capget (&hdr, ڈیٹا) < 0))
    واپس true;

  واپس (ڈیٹا[0].effective & (1U << CAP_SYS_PTRACE)) != 0;
#ورنہ
  واپس geteuid () == 0;
#اگر_ختم
}

ساکن libcrun_container_t *
make_container (runtime_spec_schema_config_schema *container_def, ثابت حرف *راستہ, ثابت حرف *config)
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
  اگر (container_def->linux)
    {
      size_t i;

      برائے (i = 0; i < container_def->linux->namespaces_len; i++)
        اگر (container_def->linux->namespaces[i]->راستہ && container_def->linux->namespaces[i]->راستہ[0] == '\0')
          {
            آزاد (container_def->linux->namespaces[i]->راستہ);
            container_def->linux->namespaces[i]->راستہ = تہی;
          }
    }

  اگر (راستہ)
    container->config_file = xstrdup (راستہ);
  اگر (config)
    container->config_file_content = xstrdup (config);

  واپس container;
}

libcrun_container_t *
libcrun_container_بھرو_from_حافظہ (ثابت حرف *json, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free حرف *oci_error = تہی;
  container_def = runtime_spec_schema_config_schema_parse_ڈیٹا (json, تہی, &oci_error);
  اگر (container_def == تہی)
    {
      crun_make_error (err, 0, "load: `%s`", oci_error);
      واپس تہی;
    }
  واپس make_container (container_def, تہی, json);
}

libcrun_container_t *
libcrun_container_بھرو_from_file (ثابت حرف *راستہ, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free حرف *oci_error = تہی;
  cleanup_free حرف *content = تہی;
  size_t len;
  صحیح ret;

  libcrun_debug ("Loading container from config file: `%s`", راستہ);
  ret = read_all_file (راستہ, &content, &len, err);
  اگر (UNLIKELY (ret < 0))
    واپس تہی;

  container_def = runtime_spec_schema_config_schema_parse_ڈیٹا (content, تہی, &oci_error);
  اگر (container_def == تہی)
    {
      crun_make_error (err, 0, "load `%s`: %s", راستہ, oci_error);
      واپس تہی;
    }
  واپس make_container (container_def, تہی, content);
}

ثابت حرف *
libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err)
{
  ساخت parser_context ctx = { 0, stderr };
  parser_error gen_err = تہی;

  اگر (container == تہی)
    {
      crun_make_error (err, EINVAL, "no container specified");
      واپس تہی;
    }

  اگر (container->config_file_content == تہی && container->container_def)
    {
      container->config_file_content = runtime_spec_schema_config_schema_generate_json (container->container_def, &ctx, &gen_err);
      اگر (container->config_file_content == تہی)
        {
          crun_make_error (err, 0, "cannot generate the configuration JSON: %s", gen_err ?: "unknown error");
          آزاد (gen_err);
          واپس تہی;
        }
      آزاد (gen_err);
    }

  اگر (container->config_file_content == تہی)
    {
      crun_make_error (err, 0, "the container has no configuration");
      واپس تہی;
    }

  واپس container->config_file_content;
}

ثابت حرف *
libcrun_container_get_annotation (libcrun_container_t *container, ثابت حرف *کلید)
{
  اگر (container == تہی)
    واپس تہی;
  واپس ڈھونڈو_string_map_قدر (container->annotations, کلید);
}

size_t
libcrun_container_get_annotations_len (libcrun_container_t *container)
{
  اگر (container == تہی || container->annotations == تہی)
    واپس 0;
  واپس string_map_سائز (container->annotations);
}

صحیح
libcrun_container_get_annotation_at (libcrun_container_t *container, size_t اشاریہ, ثابت حرف **کلید,
                                     ثابت حرف **قدر)
{
  اگر (container == تہی)
    {
      errno = ERANGE;
      واپس -1;
    }
  واپس string_map_get_at (container->annotations, اشاریہ, کلید, قدر);
}

uid_t
libcrun_container_get_uid (libcrun_container_t *container)
{
  اگر (container == تہی)
    واپس (uid_t) -1;
  واپس container->host_uid;
}

gid_t
libcrun_container_get_gid (libcrun_container_t *container)
{
  اگر (container == تہی)
    واپس (gid_t) -1;
  واپس container->host_gid;
}

خالی
libcrun_container_free (libcrun_container_t *ctr)
{
  اگر (ctr == تہی)
    واپس;

  اگر (ctr->cleanup_نجی_ڈیٹا)
    ctr->cleanup_نجی_ڈیٹا (ctr->نجی_ڈیٹا);

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
libcrun_context_new (ثابت حرف *id, ثابت حرف *state_جڑ, libcrun_error_t *err)
{
  libcrun_context_t *ctx = xmalloc0 (حجم (*ctx));

  /* Go through the setters so that the owned_* copies and the borrowed
     pointers stay in sync and are released by libcrun_context_free.  */
  libcrun_context_سیٹ_id (ctx, id);
  libcrun_context_سیٹ_state_جڑ (ctx, state_جڑ);
  libcrun_context_سیٹ_bundle (ctx, ".");
  ctx->fifo_exec_wait_fd = -1;

  اگر (libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, id, تہی, err) < 0)
    {
      libcrun_context_free (ctx);
      واپس تہی;
    }

  ctx->handler_manager = libcrun_handler_manager_create (err);
  اگر (ctx->handler_manager == تہی)
    {
      libcrun_context_free (ctx);
      واپس تہی;
    }

#اگر_تعریف_شدہ CRUN_LIBDIR
  اگر (access (CRUN_LIBDIR "/handlers", F_OK) == 0)
    {
      اگر (libcrun_handler_manager_بھرو_directory (ctx->handler_manager, CRUN_LIBDIR "/handlers", err) < 0)
        {
          libcrun_context_free (ctx);
          واپس تہی;
        }
    }
#اگر_ختم

  واپس ctx;
}

خالی
libcrun_context_free (libcrun_context_t *ctx)
{
  اگر (ctx == تہی)
    واپس;

  آزاد (ctx->owned_state_جڑ);
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

#تعریف_کرو LIBCRUN_CONTEXT_سیٹ_STRING(میدان)                  \
  کرو                                                       \
    {                                                      \
      آزاد (ctx->owned_##field);                           \
      ctx->owned_##field = قدر ? xstrdup (قدر) : تہی; \
      ctx->میدان = ctx->owned_##field;                     \
  } جب (0)

خالی
libcrun_context_سیٹ_id (libcrun_context_t *ctx, ثابت حرف *قدر)
{
  LIBCRUN_CONTEXT_سیٹ_STRING (id);
}

خالی
libcrun_context_سیٹ_state_جڑ (libcrun_context_t *ctx, ثابت حرف *قدر)
{
  LIBCRUN_CONTEXT_سیٹ_STRING (state_جڑ);
}

خالی
libcrun_context_سیٹ_bundle (libcrun_context_t *ctx, ثابت حرف *قدر)
{
  LIBCRUN_CONTEXT_سیٹ_STRING (bundle);
}

خالی
libcrun_context_سیٹ_console_socket (libcrun_context_t *ctx, ثابت حرف *قدر)
{
  LIBCRUN_CONTEXT_سیٹ_STRING (console_socket);
}

خالی
libcrun_context_سیٹ_pid_file (libcrun_context_t *ctx, ثابت حرف *قدر)
{
  LIBCRUN_CONTEXT_سیٹ_STRING (pid_file);
}

خالی
libcrun_context_سیٹ_notify_socket (libcrun_context_t *ctx, ثابت حرف *قدر)
{
  LIBCRUN_CONTEXT_سیٹ_STRING (notify_socket);
}

خالی
libcrun_context_سیٹ_handler (libcrun_context_t *ctx, ثابت حرف *قدر)
{
  LIBCRUN_CONTEXT_سیٹ_STRING (handler);
}

#تعریف_منسوخ LIBCRUN_CONTEXT_سیٹ_STRING

ثابت حرف *
libcrun_context_get_pid_file (libcrun_context_t *ctx)
{
  واپس ctx->pid_file;
}

ثابت حرف *
libcrun_context_get_state_جڑ (libcrun_context_t *ctx)
{
  اگر (ctx == تہی)
    واپس تہی;
  واپس ctx->state_جڑ;
}

خالی
libcrun_context_سیٹ_preserve_fds (libcrun_context_t *ctx, صحیح preserve_fds)
{
  ctx->preserve_fds = preserve_fds;
}

صحیح
libcrun_context_get_preserve_fds (libcrun_context_t *ctx)
{
  واپس ctx->preserve_fds;
}

خالی
libcrun_context_سیٹ_listen_fds (libcrun_context_t *ctx, صحیح listen_fds)
{
  ctx->listen_fds = listen_fds;
}

خالی
libcrun_context_سیٹ_دلائل (libcrun_context_t *ctx, صحیح argc, حرف **argv)
{
  ctx->argc = argc;
  ctx->argv = argv;
}

خالی
libcrun_context_سیٹ_systemd_cgroup (libcrun_context_t *ctx, bool قدر)
{
  ctx->systemd_cgroup = قدر;
}

خالی
libcrun_context_سیٹ_detach (libcrun_context_t *ctx, bool قدر)
{
  ctx->detach = قدر;
}

خالی
libcrun_context_سیٹ_no_new_keyring (libcrun_context_t *ctx, bool قدر)
{
  ctx->no_new_keyring = قدر;
}

خالی
libcrun_context_سیٹ_no_محور (libcrun_context_t *ctx, bool قدر)
{
  ctx->no_محور = قدر;
}

خالی
libcrun_context_سیٹ_force_no_cgroup (libcrun_context_t *ctx, bool قدر)
{
  ctx->force_no_cgroup = قدر;
}

خالی
libcrun_context_سیٹ_output_handler (libcrun_context_t *ctx, crun_output_handler handler, خالی *arg)
{
  ctx->output_handler = handler;
  ctx->output_handler_arg = arg;
}

صحیح
libcrun_context_init_logging (libcrun_context_t *ctx, ثابت حرف *لوگارتھم, libcrun_error_t *err)
{
  واپس libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, ctx->id, لوگارتھم, err);
}

ساکن صحیح
بلاک_اشارات (libcrun_error_t *err)
{
  صحیح ret;
  sigset_t نقاب;
  sigfillset (&نقاب);
  ret = sigprocmask (SIG_بلاک, &نقاب, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "sigprocmask");
  واپس 0;
}

ساکن صحیح
unblock_اشارات (libcrun_error_t *err)
{
  صحیح i;
  صحیح ret;
  sigset_t نقاب;
  ساخت sigaction act = {};

  sigfillset (&نقاب);
  ret = sigprocmask (SIG_UNBLOCK, &نقاب, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "sigprocmask");

  act.sa_handler = SIG_DFL;
  برائے (i = 0; i < NSIG; i++)
    {
      ret = sigaction (i, &act, تہی);
      اگر (ret < 0 && errno != EINVAL)
        واپس crun_make_error (err, errno, "sigaction");
    }

  واپس 0;
}

/* must be used on the host before pivot_root(2).  */
ساکن صحیح
initialize_security (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, libcrun_error_t *err)
{
  صحیح ret;

  ret = libcrun_init_caps (container, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (UNLIKELY (proc == تہی))
    واپس 0;

  اگر (proc->apparmor_profile)
    {
      ret = libcrun_initialize_apparmor (err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  ret = libcrun_initialize_selinux (container, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس 0;
}

/* Emit the "annotations" map.  Returns a json_gen_status; callers can wrap
   the call with GEN_OR_FAIL.  */
ساکن صحیح
gen_annotations (json_gen_ctx *gen, json_map_string_string *annotations)
{
  size_t i;
  صحیح r;

  GEN_کلید (gen, "annotations");
  GEN_OR_FAIL (json_gen_map_open (gen));

  برائے (i = 0; i < annotations->len; i++)
    {
      /* An annotation whose value is null in the document is parsed into a
         NULL, which cannot be emitted as a JSON string; leave it out.  */
      اگر (annotations->کلیدیں[i] == تہی || annotations->اقدار[i] == تہی)
        جاری;

      GEN_STR (gen, annotations->کلیدیں[i]);
      GEN_STR (gen, annotations->اقدار[i]);
    }

  واپس json_gen_map_close (gen);

gen_error:
  واپس r;
}

/* Return the bundle path to report in the container state.  crun always
   chdir()s into the bundle, so fall back to getcwd() when the caller has no
   absolute bundle path, as can happen when libcrun is used as a library.
   *ALLOCATED must be freed by the caller.  */
ساکن ثابت حرف *
get_bundle_راستہ (ثابت حرف *bundle, حرف **allocated)
{
  اگر (bundle != تہی && bundle[0] == '/')
    واپس bundle;

  *allocated = getcwd (تہی, 0);
  اگر (*allocated == تہی)
    OOM ();
  واپس *allocated;
}

ساکن صحیح
do_hooks (runtime_spec_schema_config_schema *def, pid_t pid, ثابت حرف *id, bool keep_going, ثابت حرف *bundle,
          ثابت حرف *cwd, ثابت حرف *حالت_2, ثابت حرف *name, hook **hooks, size_t hooks_len, صحیح برآمد_fd,
          صحیح err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  size_t i, stdin_len;
  صحیح r, ret;
  حرف *stdin = تہی;
  cleanup_free حرف *bundle_allocated = تہی;
  ثابت حرف *rootfs = def->جڑ ? def->جڑ->راستہ : "";
  cleanup_json_gen json_gen_ctx *gen = تہی;

  bundle = get_bundle_راستہ (bundle, &bundle_allocated);

  اگر (! json_gen_init (&gen, تہی))
    واپس crun_make_error (err, 0, "json_gen_init failed");

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
  GEN_STR (gen, حالت_2);

  اگر (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, (ثابت حرف **) &stdin, &stdin_len));

  ret = 0;

  bool error_created = false;
  برائے (i = 0; i < hooks_len; i++)
    {
      حرف **env = environ;

      اگر (hooks[i]->env)
        env = hooks[i]->env;

      /* Release the error from the previous iteration, if any.  */
      اگر (error_created)
        {
          crun_error_release (err);
          error_created = false;
        }

      ret = تسلسل_process_with_stdin_timeout_envp (hooks[i]->راستہ, hooks[i]->دلائل, cwd, hooks[i]->timeout, env,
                                                 stdin, stdin_len, برآمد_fd, err_fd, can_ignore_chdir_errors, err);
      اگر (UNLIKELY (ret < 0))
        error_created = true;

      اگر (UNLIKELY (ret != 0))
        {
          اگر (keep_going)
            libcrun_warning ("error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->راستہ, ret);
          ورنہ
            {
              libcrun_error (0, "error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->راستہ, ret);
              توڑو;
            }
        }
    }

  واپس ret;

gen_error:
  واپس json_gen_error_مقصد_crun_error (r, err);
}

ساکن صحیح
get_json_gen_نتیجہ (json_gen_ctx *gen, حرف **برآمد, size_t *برآمد_len)
{
  ثابت حرف *buf = تہی;
  size_t buf_len = 0;
  صحیح r;

  r = json_gen_get_buf (gen, &buf, &buf_len);
  اگر (UNLIKELY (r != json_gen_حالت_ok))
    واپس r;

  *برآمد_len = buf_len;

  *برآمد = xmalloc (buf_len + 1);
  حافظہ_نقل (*برآمد, buf, buf_len);
  (*برآمد)[buf_len] = '\0';

  واپس json_gen_حالت_ok;
}

ساکن صحیح
get_seccomp_receiver_fd_payload (libcrun_container_t *container, ثابت حرف *حالت_2, pid_t own_pid,
                                 حرف **seccomp_fd_payload, size_t *seccomp_fd_payload_len, libcrun_error_t *err)
{
  صحیح r;
  json_gen_ctx *gen = تہی;
  runtime_spec_schema_config_schema *def = container->container_def;
  ثابت حرف *ثابت OCI_نسخہ = "0.2.0";

  اگر (! json_gen_init (&gen, تہی))
    واپس crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_کلید (gen, "ociVersion");
  GEN_STR (gen, OCI_نسخہ);

  GEN_کلید (gen, "fds");
  GEN_OR_FAIL (json_gen_array_open (gen));

  GEN_کلید (gen, "seccompFd");
  GEN_OR_FAIL (json_gen_array_close (gen));

  GEN_کلید (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  اگر (def && def->linux && def->linux->seccomp)
    {
      ثابت حرف *metadata = def->linux->seccomp->listener_metadata;

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
  GEN_STR (gen, OCI_نسخہ);

  اگر (container->context && container->context->id)
    {
      GEN_کلید (gen, "id");
      GEN_STR (gen, container->context->id);
    }

  GEN_کلید (gen, "status");
  GEN_STR (gen, حالت_2);

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

  r = get_json_gen_نتیجہ (gen, seccomp_fd_payload, seccomp_fd_payload_len);

gen_error:
  json_gen_free (gen);

  واپس json_gen_error_مقصد_crun_error (r, err);
}

ساکن صحیح
send_sync_cb (خالی *ڈیٹا, libcrun_error_t *err)
{
  صحیح sync_socket_fd = *((صحیح *) ڈیٹا);
  صحیح ret;

  /* sync 2.  */
  ret = sync_socket_send_sync (sync_socket_fd, false, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  /* sync 3.  */
  واپس sync_socket_wait_sync (تہی, sync_socket_fd, false, err);
}

ساکن صحیح
maybe_chown_std_streams (uid_t container_uid, gid_t container_gid, libcrun_error_t *err)
{
  صحیح ret, i;

  برائے (i = 0; i < 3; i++)
    {
      اگر (! isatty (i))
        {
          ساخت stat statbuf;
          ret = fstat (i, &statbuf);
          اگر (UNLIKELY (ret < 0))
            {
              اگر (errno == EBADF)
                جاری;
              واپس crun_make_error (err, errno, "fstat fd `%d`", i);
            }

          /* Skip chown for device files */
          اگر (S_ISCHR (statbuf.st_mode) || S_ISBLK (statbuf.st_mode))
            جاری;

          ret = fchown (i, container_uid, container_gid);
          اگر (UNLIKELY (ret < 0))
            {
              /* EINVAL means the user is not mapped in the current userns.
                 Ignore EPERM and EROFS as well as there is no reason to fail
                 so early, and let the container payload deal with it.
                 EBADF means fd is closed.
              */
              اگر (errno == EINVAL || errno == EPERM || errno == EROFS || errno == EBADF)
                جاری;

              واپس crun_make_error (err, errno, "fchown std stream `%i`", i);
            }
        }
    }
  واپس 0;
}

صحیح
libcrun_container_notify_handler (ساخت container_entrypoint_s *دلائل,
                                  شمار handler_configure_phase phase,
                                  libcrun_container_t *container, ثابت حرف *rootfs,
                                  libcrun_error_t *err)
{
  ساخت custom_handler_s *h;

  اگر (دلائل->custom_handler == تہی)
    واپس 0;

  h = دلائل->custom_handler->vtable;
  اگر (h == تہی || h->configure_container == تہی)
    واپس 0;

  واپس h->configure_container (دلائل->custom_handler->cookie, phase,
                                 دلائل->context, container,
                                 rootfs, err);
}

/* Resolve and normalize the container rootfs path.  */
ساکن صحیح
resolve_rootfs_راستہ (libcrun_container_t *container, حرف **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  اگر (def->جڑ && def->جڑ->راستہ)
    {
      *rootfs = realpath (def->جڑ->راستہ, تہی);
      اگر (UNLIKELY (*rootfs == تہی))
        {
          صحیح saved_errno = errno;

          /* If realpath failed for any reason, try the relative directory.  */
          اگر (def->جڑ->راستہ[0] == '/')
            {
              cleanup_free حرف *cwd = تہی;
              libcrun_error_t tmp_err = تہی;
              ssize_t len;
              صحیح proc_fd;

              proc_fd = libcrun_get_cached_proc_fd (container, &tmp_err);
              اگر (LIKELY (proc_fd >= 0))
                {
                  len = محفوظ_readlinkat (proc_fd, "self/cwd", &cwd, 0, &tmp_err);
                  اگر (LIKELY (len > 0))
                    {
                      /* If the rootfs is under the current working directory, just use its relative path.  */
                      اگر (has_dir_سابقہ (def->جڑ->راستہ, cwd))
                        {
                          ثابت حرف *it = consume_slashes (def->جڑ->راستہ + len);
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

          اگر (*rootfs == تہی)
            {
              اگر (saved_errno == ENOENT)
                واپس crun_make_error (err, ENOENT, "the rootfs path `%s` does not exist", def->جڑ->راستہ);

              *rootfs = xstrdup (def->جڑ->راستہ);
            }
        }
    }
  واپس 0;
}

/* Initialize the environment variables.  */
ساکن صحیح
تیاری_environment (runtime_spec_schema_config_schema *def, uid_t container_uid, libcrun_error_t *err)
{
  صحیح ret;

  اگر (clearenv ())
    واپس crun_make_error (err, errno, "clearenv");

  اگر (def->process)
    {
      size_t i;

      برائے (i = 0; i < def->process->env_len; i++)
        {
          /* A null element of the array of variables is parsed into a NULL,
             which putenv(3) cannot be handed.  */
          اگر (def->process->env[i] == تہی)
            واپس crun_make_error (err, EINVAL, "environment variable is not specified");

          اگر (putenv (def->process->env[i]) < 0)
            واپس crun_make_error (err, errno, "putenv `%s`", def->process->env[i]);
        }
    }

  اگر (is_خالی_خانے_string (getenv ("HOME")))
    {
      ret = سیٹ_home_env (container_uid);
      اگر (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          اگر (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            واپس crun_make_error (err, errno, "setenv HOME");
        }
    }

  واپس 0;
}

/* Configure terminal and console for interactive containers.  */
ساکن صحیح
تیاری_terminal (ساخت container_entrypoint_s *entrypoint_دلائل, libcrun_container_t *container,
                صحیح console_socket, صحیح console_socketpair, صحیح has_terminal, libcrun_error_t *err)
{
  cleanup_close صحیح terminal_fd = -1;
  صحیح ret;

  اگر (! has_terminal)
    واپس 0;

  fflush (stderr);

  اگر (console_socket >= 0 || (entrypoint_دلائل->has_terminal_socket_pair && console_socketpair >= 0))
    {
      terminal_fd = libcrun_سیٹ_terminal (container, err);
      اگر (UNLIKELY (terminal_fd < 0))
        واپس terminal_fd;
    }

  اگر (console_socket >= 0)
    {
      ret = send_fd_مقصد_socket (console_socket, terminal_fd, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  ورنہ اگر (entrypoint_دلائل->has_terminal_socket_pair && console_socketpair >= 0)
    {
      ret = send_fd_مقصد_socket (console_socketpair, terminal_fd, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  واپس 0;
}

/* Resolve and validate the container's executable path.  */
ساکن صحیح
تیاری_executable_راستہ (ساخت container_entrypoint_s *entrypoint_دلائل, runtime_spec_schema_config_schema *def,
                       حرف **exec_راستہ, libcrun_error_t *err)
{
  صحیح ret;

  اگر (def->process && def->process->دلائل)
    {
      ret = ڈھونڈو_executable (exec_راستہ, def->process->دلائل[0], def->process->cwd, err);
      اگر (UNLIKELY (ret < 0))
        {
          اگر (entrypoint_دلائل->custom_handler == تہی && crun_error_get_errno (err) == ENOENT)
            واپس ret;

          /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
             once the process switched to the use that runs in the container.  This might be necessary
             when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
             is not honored.  */
          crun_error_release (err);
        }
    }

  واپس 0;
}

/* Apply security settings for the container process.  */
ساکن صحیح
apply_security_settings (ساخت container_entrypoint_s *entrypoint_دلائل, runtime_spec_schema_config_schema *def,
                         libcrun_container_t *container, pid_t own_pid, libcrun_error_t *err)
{
  صحیح ret;
  runtime_spec_schema_config_schema_process_capabilities *capabilities;
  صحیح no_new_privs;

  اگر (def->process && ! def->process->no_new_privileges)
    {
      حرف **seccomp_flags = تہی;
      size_t seccomp_flags_len = 0;
      cleanup_free حرف *seccomp_fd_payload = تہی;
      size_t seccomp_fd_payload_len = 0;

      اگر (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      اگر (entrypoint_دلائل->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_دلائل->seccomp_fd, entrypoint_دلائل->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      close_and_reset (&entrypoint_دلائل->seccomp_fd);
      close_and_reset (&entrypoint_دلائل->seccomp_receiver_fd);
    }

  capabilities = def->process ? def->process->capabilities : تہی;
  no_new_privs = def->process ? def->process->no_new_privileges : 1;
  ret = libcrun_سیٹ_caps (capabilities, container->container_uid, container->container_gid, no_new_privs,
                          entrypoint_دلائل->custom_handler != تہی, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس 0;
}

/* Create the process that joins the existing PID namespace now that the
   host mounts are not visible anymore, and continue the setup from there.  */
ساکن صحیح
do_delayed_pidns_fork (صحیح sync_socket, pid_t *own_pid, libcrun_error_t *err)
{
  pid_t pid;
  صحیح ret;

  pid = fork ();
  اگر (UNLIKELY (pid < 0))
    واپس crun_make_error (err, errno, "fork");

  اگر (pid)
    {
      /* Report the new PID to the parent and exit immediately.  */
      ret = sync_socket_send_sync (sync_socket, false, err);
      اگر (LIKELY (ret == 0))
        {
          ret = عارضی_FAILURE_RETRY (اكتب (sync_socket, &pid, حجم (pid)));
          اگر (LIKELY (ret == حجم (pid)))
            محفوظ_exit (EXIT_SUCCESS);
        }

      kill (pid, SIGKILL);
      محفوظ_exit (EXIT_FAILURE);
    }

  /* sync receive own pid.  */
  ret = عارضی_FAILURE_RETRY (اقرأ (sync_socket, own_pid, حجم (*own_pid)));
  اگر (UNLIKELY (ret != حجم (*own_pid)))
    {
      اگر (ret >= 0)
        errno = 0;
      واپس crun_make_error (err, errno, "read from sync socket");
    }

  واپس 0;
}

ساخت delayed_pidns_fork_s
{
  libcrun_container_t *container;
  صحیح sync_socket;
  pid_t *own_pid;
};

ساکن صحیح
delayed_pidns_fork_cb (خالی *ڈیٹا, libcrun_error_t *err)
{
  ساخت delayed_pidns_fork_s *d = ڈیٹا;

  اگر (! libcrun_container_delayed_pidns_fork (d->container))
    واپس 0;

  واپس do_delayed_pidns_fork (d->sync_socket, d->own_pid, err);
}

ساکن صحیح
container_init_تیاری (خالی *دلائل, pid_t *own_pid, حرف *notify_socket,
                      صحیح sync_socket, حرف **exec_راستہ, libcrun_error_t *err)
{
  ساخت container_entrypoint_s *entrypoint_دلائل = دلائل;
  libcrun_container_t *container = entrypoint_دلائل->container;
  bool chdir_مکمل = false;
  صحیح ret;
  صحیح has_terminal;
  cleanup_close صحیح console_socket = -1;
  cleanup_close صحیح console_socketpair = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free حرف *rootfs = تہی;
  ساخت delayed_pidns_fork_s delayed_pidns_fork = {
    .container = container,
    .sync_socket = sync_socket,
    .own_pid = own_pid,
  };

  ret = initialize_security (container, def->process, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_configure_جال (container, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = resolve_rootfs_راستہ (container, &rootfs, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  /* Configure terminal socket pair for container communication.  */
  اگر (entrypoint_دلائل->terminal_socketpair[0] >= 0)
    {
      close_and_reset (&entrypoint_دلائل->terminal_socketpair[0]);
      console_socketpair = entrypoint_دلائل->terminal_socketpair[1];
    }

  /* sync 1.  */
  ret = sync_socket_wait_sync (تہی, sync_socket, false, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  has_terminal = container->container_def->process && container->container_def->process->terminal;
  اگر (has_terminal && entrypoint_دلائل->context->console_socket)
    console_socket = entrypoint_دلائل->console_socket_fd;

  ret = libcrun_سیٹ_sysctl (container, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_container_notify_handler (entrypoint_دلائل, HANDLER_CONFIGURE_BEFORE_MOUNTS, container, rootfs, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_do_محور_جڑ (container, entrypoint_دلائل->context->no_محور, &rootfs, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  /* sync 2 and 3 are sent as part of libcrun_set_mounts.  */
  ret = libcrun_سیٹ_mounts (entrypoint_دلائل, container, rootfs, send_sync_cb, &sync_socket, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  /* The createContainer hooks MUST run after the mounts are set up but before
     the pivot_root.  When these hooks are present can_use_open_tree_namespace()
     forces the deferred-pivot path, so the actual pivot happens later in
     libcrun_finalize_mounts().  */
  اگر (def->hooks && def->hooks->create_container_len)
    {
      libcrun_error_t tmp_err = تہی;
      صحیح اندر_userns = check_running_اندر_user_namespace (&tmp_err);
      اگر (tmp_err)
        crun_error_release (&tmp_err);
      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_دلائل->bundle, تہی, "created", "createContainer",
                      (hook **) def->hooks->create_container,
                      def->hooks->create_container_len, entrypoint_دلائل->hooks_برآمد_fd, entrypoint_دلائل->hooks_err_fd,
                      اندر_userns > 0, err);
      اگر (UNLIKELY (ret != 0))
        واپس ret;
    }

  ret = libcrun_finalize_mounts (entrypoint_دلائل, container, rootfs, delayed_pidns_fork_cb, &delayed_pidns_fork, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (def->process)
    {
      ret = libcrun_سیٹ_selinux_label (container, def->process, false, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = libcrun_سیٹ_apparmor_profile (container, def->process, false, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  ret = mark_or_close_fds_ge_than (container, entrypoint_دلائل->context->preserve_fds + 3, false, err);
  اگر (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ret = libcrun_reopen_dev_null (err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = maybe_chown_std_streams (container->container_uid, container->container_gid, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = تیاری_environment (def, container->container_uid, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  /* Set primary process to 1 explicitly if nothing is configured and LISTEN_FD is not set.  */
  اگر (entrypoint_دلائل->context->listen_fds > 0 && getenv ("LISTEN_PID") == تہی)
    {
      libcrun_warning ("setting LISTEN_PID=1 since no previous configuration was found");
      اگر (UNLIKELY (setenv ("LISTEN_PID", "1", 1) < 0))
        واپس crun_make_error (err, errno, "setenv LISTEN_PID");
    }

  /* Attempt to chdir immediately here, before doing the setresuid.  If we fail here, let's
     try again later once the process switched to the user that runs in the container.  */
  اگر (def->process && def->process->cwd)
    {
      ret = libcrun_محفوظ_chdir (def->process->cwd, err);
      اگر (LIKELY (ret == 0))
        chdir_مکمل = true;
      ورنہ
        crun_error_release (err);
    }

  ret = setsid ();
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "setsid");

  ret = تیاری_terminal (entrypoint_دلائل, container, console_socket, console_socketpair, has_terminal, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  close_and_reset (&console_socket);
  close_and_reset (&console_socketpair);

  ret = تیاری_executable_راستہ (entrypoint_دلائل, def, exec_راستہ, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_سیٹ_hostname (container, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_سیٹ_domainname (container, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_سیٹ_personality (container->container_def->linux->personality, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (def->process && def->process->user)
    umask (def->process->user->umask_present ? def->process->user->umask : 0022);

  ret = apply_security_settings (entrypoint_دلائل, def, container, *own_pid, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (UNLIKELY (def->process && def->process->دلائل && *exec_راستہ == تہی))
    {
      ret = ڈھونڈو_executable (exec_راستہ, def->process->دلائل[0], def->process->cwd, err);
      اگر (UNLIKELY (ret < 0))
        {
          اگر (entrypoint_دلائل->custom_handler == تہی || is_خالی_خانے_string (def->process->دلائل[0]))
            واپس ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          *exec_راستہ = xstrdup (def->process->دلائل[0]);
        }
    }

  /* The chdir was not already performed, so try again now after switching to the UID/GID in the container.  */
  اگر (! chdir_مکمل && def->process && def->process->cwd)
    {
      ret = libcrun_محفوظ_chdir (def->process->cwd, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (notify_socket && putenv (notify_socket) < 0)
    واپس crun_make_error (err, errno, "putenv `%s`", notify_socket);

  واپس 0;
}

ساکن صحیح
open_hooks_output (libcrun_container_t *container, صحیح *برآمد_fd, صحیح *err_fd, libcrun_error_t *err)
{
  ثابت حرف *annotation;

  *err_fd = *برآمد_fd = -1;

  libcrun_debug ("Opening hooks output");
  annotation = ڈھونڈو_annotation (container, "run.oci.hooks.stdout");
  اگر (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stdout` annotation");
      *برآمد_fd = عارضی_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      اگر (UNLIKELY (*برآمد_fd < 0))
        واپس crun_make_error (err, errno, "open `%s`", annotation);
    }

  annotation = ڈھونڈو_annotation (container, "run.oci.hooks.stderr");
  اگر (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stderr` annotation");
      *err_fd = عارضی_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      اگر (UNLIKELY (*err_fd < 0))
        واپس crun_make_error (err, errno, "open `%s`", annotation);
    }

  واپس 0;
}

/* The environment the process was started with lives on the initial stack, right after the
   argv strings and before the string AT_EXECFN points to.  clearenv(3) only drops the pointers
   to it, so when there is no execv to replace the stack, erase it or the container process can
   still read it, e.g. through /proc/self/environ.  */
ساکن خالی
erase_initial_environment (حرف **argv, صحیح argc)
{
  حرف *آغاز = argv[argc - 1] + لڑی_لمبائی (argv[argc - 1]) + 1;
  حرف *انتہا = (حرف *) getauxval (AT_EXECFN);

  اگر (انتہا > آغاز)
    memset (آغاز, 0, انتہا - آغاز);
}

ساکن خالی
rewrite_argv (حرف **argv, صحیح argc, ثابت حرف *name, حرف **دلائل, size_t دلائل_len)
{
  cleanup_free حرف *decorated_name = تہی;
  size_t دستیاب_len = argv[argc - 1] - argv[0] + لڑی_لمبائی (argv[argc - 1]) + 1;
  cleanup_free حرف *new_argv = تہی;
  size_t i, so_far = 0, needed = 0;

  needed = xasprintf (&decorated_name, "[libcrun:%s]", name) + 1;
  برائے (i = 0; i < دلائل_len; i++)
    needed += لڑی_لمبائی (دلائل[i]) + 1;

  new_argv = xmalloc0 (needed + 1);
  strcpy (new_argv, decorated_name);
  so_far = لڑی_لمبائی (decorated_name) + 1;

  اگر (دستیاب_len >= needed)
    {
      برائے (i = 0; i < دلائل_len; i++)
        {
          strcpy (new_argv + so_far, دلائل[i]);
          so_far += لڑی_لمبائی (دلائل[i]) + 1;
        }
    }

  اگر (so_far >= دستیاب_len)
    so_far = دستیاب_len - 1;

  memset (argv[0], 0, دستیاب_len);
  حافظہ_نقل (argv[0], new_argv, so_far);
}

/* Entrypoint to the container.  */
ساکن صحیح
container_init (خالی *دلائل, حرف *notify_socket, صحیح sync_socket, libcrun_error_t *err)
{
  ساخت container_entrypoint_s *entrypoint_دلائل = دلائل;
  صحیح ret;
  runtime_spec_schema_config_schema *def = entrypoint_دلائل->container->container_def;
  cleanup_free حرف *exec_راستہ = تہی;
  __attribute__ ((غیر_مستعمل)) cleanup_free حرف *notify_socket_cleanup = notify_socket;
  pid_t own_pid = 0;

  entrypoint_دلائل->sync_socket = sync_socket;

  crun_سیٹ_output_handler (log_write_مقصد_sync_socket, دلائل);

  /* sync receive own pid.  */
  ret = عارضی_FAILURE_RETRY (اقرأ (sync_socket, &own_pid, حجم (own_pid)));
  اگر (UNLIKELY (ret != حجم (own_pid)))
    {
      اگر (ret >= 0)
        errno = 0;
      واپس crun_make_error (err, errno, "read from sync socket");
    }

  ret = container_init_تیاری (دلائل, &own_pid, notify_socket, sync_socket, &exec_راستہ, err);
  اگر (UNLIKELY (ret < 0))
    {
      /* If it fails to write the error using the sync socket, then fallback
         to stderr.  */
      اگر (sync_socket_write_error (sync_socket, err) == 0)
        {
          crun_error_release (err);
          /* A return value of > 0 means to fail with an error without err being set so to avoid
             printing it twice.  */
          واپس 1;
        }
      واپس ret;
    }

  entrypoint_دلائل->sync_socket = -1;

  ret = unblock_اشارات (err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  /* sync 4.  */
  ret = sync_socket_send_sync (sync_socket, false, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  close_and_reset (&sync_socket);

  اگر (entrypoint_دلائل->context->fifo_exec_wait_fd >= 0)
    {
      حرف بفر[1];
      fd_set read_سیٹ;
      cleanup_close صحیح fd = entrypoint_دلائل->context->fifo_exec_wait_fd;
      entrypoint_دلائل->context->fifo_exec_wait_fd = -1;

      FD_ZERO (&read_سیٹ);
      FD_SET (fd, &read_سیٹ);
      کرو
        {
          ret = select (fd + 1, &read_سیٹ, تہی, تہی, تہی);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "select");

          ret = عارضی_FAILURE_RETRY (اقرأ (fd, بفر, حجم (بفر)));
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "read from the exec fifo");
      } جب (ret == 0);

      close_and_reset (&entrypoint_دلائل->context->fifo_exec_wait_fd);
    }

  crun_سیٹ_output_handler (log_write_مقصد_stderr, تہی);

  اگر (def->process && def->process->no_new_privileges)
    {
      حرف **seccomp_flags = تہی;
      size_t seccomp_flags_len = 0;
      cleanup_free حرف *seccomp_fd_payload = تہی;
      size_t seccomp_fd_payload_len = 0;

      اگر (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      اگر (entrypoint_دلائل->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (entrypoint_دلائل->container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_دلائل->seccomp_fd, entrypoint_دلائل->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags,
                                   seccomp_flags_len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
      close_and_reset (&entrypoint_دلائل->seccomp_fd);
      close_and_reset (&entrypoint_دلائل->seccomp_receiver_fd);
    }

  اگر (UNLIKELY (def->process == تہی))
    واپس crun_make_error (err, 0, "block `process` not found");

  اگر (UNLIKELY (exec_راستہ == تہی))
    واپس crun_make_error (err, 0, "executable path not specified");

  اگر (def->hooks && def->hooks->آغاز_container_len)
    {
      libcrun_container_t *container = entrypoint_دلائل->container;
      libcrun_error_t tmp_err = تہی;
      صحیح اندر_userns = check_running_اندر_user_namespace (&tmp_err);
      اگر (tmp_err)
        crun_error_release (&tmp_err);

      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_دلائل->bundle, تہی, "created", "startContainer",
                      (hook **) def->hooks->آغاز_container,
                      def->hooks->آغاز_container_len, entrypoint_دلائل->hooks_برآمد_fd, entrypoint_دلائل->hooks_err_fd,
                      اندر_userns > 0, err);
      اگر (UNLIKELY (ret != 0))
        واپس ret;

      /* Seek stdout/stderr to the end.  If the hooks were using the same files,
         the container process overwrites what was previously written.  */
      (خالی) lseek (1, 0, SEEK_END);
      (خالی) lseek (2, 0, SEEK_END);
    }

  اگر (entrypoint_دلائل->custom_handler)
    {
      /* Files marked with O_CLOEXEC are closed at execv time, so make sure they are closed now.
         This is a best effort operation, because the seccomp filter is already in place and it could
         stop some syscalls used by mark_or_close_fds_ge_than.
      */
      اگر (entrypoint_دلائل->custom_handler->vtable->close_fds)
        {
          ret = entrypoint_دلائل->custom_handler->vtable->close_fds (entrypoint_دلائل->custom_handler->cookie,
                                                                    entrypoint_دلائل->container,
                                                                    entrypoint_دلائل->context->preserve_fds,
                                                                    err);
        }
      ورنہ
        {
          ret = mark_or_close_fds_ge_than (entrypoint_دلائل->container, entrypoint_دلائل->context->preserve_fds + 3, true, err);
        }
      اگر (UNLIKELY (ret < 0))
        crun_error_release (err);

      prctl (PR_سیٹ_NAME, entrypoint_دلائل->custom_handler->vtable->name);

      اگر (entrypoint_دلائل->context->argv)
        {
          erase_initial_environment (entrypoint_دلائل->context->argv, entrypoint_دلائل->context->argc);
          rewrite_argv (entrypoint_دلائل->context->argv, entrypoint_دلائل->context->argc,
                        entrypoint_دلائل->custom_handler->vtable->name, def->process->دلائل,
                        def->process->دلائل_len);

          /* It is a quite destructive operation as we might be referencing data from the old
             argv, so make sure the context is not reused.  */
          entrypoint_دلائل->context->argv = تہی;
          entrypoint_دلائل->context = تہی;
        }

      ret = libcrun_سیٹ_selinux_label (entrypoint_دلائل->container, def->process, true, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = libcrun_سیٹ_apparmor_profile (entrypoint_دلائل->container, def->process, true, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = entrypoint_دلائل->custom_handler->vtable->تسلسل_func (entrypoint_دلائل->custom_handler->cookie,
                                                               entrypoint_دلائل->container,
                                                               exec_راستہ,
                                                               def->process->دلائل);
      اگر (ret != 0)
        واپس crun_make_error (err, 0, "exec container process failed with handler as `%s`", entrypoint_دلائل->custom_handler->vtable->name);

      واپس ret;
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation since the seccomp profile is already in place now and might block
     some of the syscalls needed by mark_or_close_fds_ge_than.  */
  ret = mark_or_close_fds_ge_than (entrypoint_دلائل->container, entrypoint_دلائل->context->preserve_fds + 3, true, err);
  اگر (UNLIKELY (ret < 0))
    crun_error_release (err);

  عارضی_FAILURE_RETRY (execv (exec_راستہ, def->process->دلائل));

  اگر (errno == ENOENT)
    واپس crun_make_error (err, errno, "exec container process (missing dynamic library?) `%s`", exec_راستہ);

  واپس crun_make_error (err, errno, "exec container process `%s`", exec_راستہ);
}

ساکن صحیح
read_container_config_from_state (libcrun_container_t **container, ثابت حرف *state_جڑ, ثابت حرف *id,
                                  libcrun_error_t *err)
{
  cleanup_free حرف *config_file = تہی;
  cleanup_free حرف *dir = تہی;
  صحیح ret;

  *container = تہی;

  ret = libcrun_get_state_directory (&dir, state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = append_paths (&config_file, err, dir, "config.json", تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  *container = libcrun_container_بھرو_from_file (config_file, err);
  اگر (*container == تہی)
    واپس -1;

  واپس 0;
}

ساکن صحیح
تسلسل_poststop_hooks (arg_غیر_مستعمل libcrun_context_t *context, libcrun_container_t *container,
                    runtime_spec_schema_config_schema *def,
                    libcrun_container_حالت_t *حالت_2, ثابت حرف *state_جڑ, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container_cleanup = تہی;
  صحیح ret;

  اگر (def == تہی)
    {
      اگر (container == تہی)
        {
          ret = read_container_config_from_state (&container_cleanup, state_جڑ, id, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
          container = container_cleanup;
        }

      def = container->container_def;
    }

  اگر (def->hooks && def->hooks->poststop_len)
    {
      cleanup_close صحیح hooks_برآمد_fd = -1;
      cleanup_close صحیح hooks_err_fd = -1;

      اگر (container == تہی)
        {
          ret = read_container_config_from_state (&container_cleanup, state_جڑ, id, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
          container = container_cleanup;
        }

      ret = open_hooks_output (container, &hooks_برآمد_fd, &hooks_err_fd, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = do_hooks (def, 0, id, true, حالت_2->bundle, حالت_2->bundle, "stopped", "poststop", (hook **) def->hooks->poststop,
                      def->hooks->poststop_len, hooks_برآمد_fd, hooks_err_fd, false, err);
      اگر (UNLIKELY (ret != 0))
        {
          اگر (ret < 0)
            libcrun_error_report_and_release (err);
          ورنہ
            libcrun_error (0, "poststop hook failed with exit code: %d", ret);
        }
    }
  واپس 0;
}

ساکن bool
has_new_pid_namespace (runtime_spec_schema_config_schema *def)
{
  size_t i;

  اگر (def->linux == تہی || def->linux->namespaces == تہی)
    واپس false;

  برائے (i = 0; i < def->linux->namespaces_len; i++)
    {
      اگر (strcmp (def->linux->namespaces[i]->type, "pid") == 0 && def->linux->namespaces[i]->راستہ == تہی)
        واپس true;
    }
  واپس false;
}

/* Delete the container state.  CONTAINER is the container the caller already has
   available, or NULL.  When it is not NULL, the poststop hooks are run even if the
   status file was never written, e.g. when the container creation failed.  */
ساکن صحیح
container_delete_اندرونی (libcrun_context_t *context, libcrun_container_t *container,
                           runtime_spec_schema_config_schema *def, ثابت حرف *id, bool force, bool killall,
                           libcrun_error_t *err)
{
  cleanup_cgroup_حالت ساخت libcrun_cgroup_حالت *cgroup_حالت = تہی;
  cleanup_container_حالت libcrun_container_حالت_t حالت_2 = {};
  cleanup_container libcrun_container_t *container_cleanup = تہی;
  ثابت حرف *state_جڑ = context->state_جڑ;
  صحیح ret;

  ret = libcrun_read_container_حالت (&حالت_2, state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (force && crun_error_get_errno (err) == ENOENT)
        {
          libcrun_error_t tmp_err = تہی;

          crun_error_release (err);

          اگر (container)
            {
              libcrun_container_حالت_t partial_حالت = {
                .bundle = (حرف *) context->bundle,
              };

              ret = تسلسل_poststop_hooks (context, container, def, &partial_حالت, state_جڑ, id, err);
              اگر (UNLIKELY (ret < 0))
                crun_error_write_warning_and_release (context->output_handler_arg, &err);
            }

          libcrun_container_delete_حالت (state_جڑ, id, &tmp_err);
          crun_error_release (&tmp_err);
          واپس 0;
        }
      crun_error_release (err);
      واپس libcrun_container_delete_حالت (state_جڑ, id, err);
    }

  cgroup_حالت = libcrun_cgroup_make_حالت (&حالت_2);

  اگر (! force)
    {
      ret = libcrun_is_container_running (&حالت_2, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
      اگر (ret == 1)
        {
          ret = libcrun_حالت_has_read_exec_fifo (state_جڑ, id, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
          اگر (ret == 0)
            واپس crun_make_error (err, 0, "the container `%s` is not in `created` or `stopped` state", id);

          /* If the container is in "created" state, then do the equivalent of delete --force.  */
          killall = force = true;
        }
    }

  اگر (def == تہی)
    {
      اگر (container == تہی)
        {
          ret = read_container_config_from_state (&container_cleanup, state_جڑ, id, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;

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
          ret = libcrun_kill_linux (&حالت_2, SIGKILL, err);
          اگر (UNLIKELY (ret < 0))
            {
              اگر (crun_error_get_errno (err) != ESRCH)
                واپس ret;

              crun_error_release (err);
            }
        }
      ورنہ اگر (حالت_2.cgroup_راستہ)
        {
          ret = libcrun_cgroup_killall (cgroup_حالت, SIGKILL, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }
    }

  اگر (def->linux && def->linux->intel_rdt)
    {
      ret = libcrun_destroy_intelrdt (id, def, err);
      اگر (UNLIKELY (ret < 0))
        libcrun_error_report_and_release (err);
    }

  اگر (حالت_2.cgroup_راستہ)
    {
      /* If the cgroup cannot be destroyed, there are still processes in it.
         Keep the container status, it is the only record of the cgroup, so
         that the delete can be retried.  */
      ret = libcrun_cgroup_destroy (cgroup_حالت, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  ret = تسلسل_poststop_hooks (context, container, def, &حالت_2, state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  واپس libcrun_container_delete_حالت (state_جڑ, id, err);
}

صحیح
libcrun_container_delete (libcrun_context_t *context, ثابت حرف *id, bool force, libcrun_error_t *err)
{
  واپس container_delete_اندرونی (context, تہی, تہی, id, force, true, err);
}

صحیح
libcrun_container_kill (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *اشارہ, libcrun_error_t *err)
{
  صحیح sig, ret;
  ثابت حرف *state_جڑ = context->state_جڑ;
  cleanup_container_حالت libcrun_container_حالت_t حالت_2 = {};

  sig = str2sig (اشارہ);
  اگر (UNLIKELY (sig < 0))
    واپس crun_make_error (err, 0, "unknown signal `%s`", اشارہ);

  ret = libcrun_read_container_حالت (&حالت_2, state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس libcrun_kill_linux (&حالت_2, sig, err);
}

صحیح
libcrun_container_killall (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *اشارہ, libcrun_error_t *err)
{
  صحیح sig, ret;
  ثابت حرف *state_جڑ = context->state_جڑ;
  cleanup_container_حالت libcrun_container_حالت_t حالت_2 = {};
  cleanup_cgroup_حالت ساخت libcrun_cgroup_حالت *cgroup_حالت = تہی;

  sig = str2sig (اشارہ);
  اگر (UNLIKELY (sig < 0))
    واپس crun_make_error (err, 0, "unknown signal `%s`", اشارہ);

  ret = libcrun_read_container_حالت (&حالت_2, state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  cgroup_حالت = libcrun_cgroup_make_حالت (&حالت_2);

  ret = libcrun_cgroup_killall (cgroup_حالت, sig, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;
  واپس 0;
}

ساکن صحیح
write_container_حالت (libcrun_container_t *container, libcrun_context_t *context,
                        pid_t pid, ساخت libcrun_cgroup_حالت *cgroup_حالت,
                        libcrun_error_t *err)
{
  cleanup_free حرف *bundle_allocated = تہی;
  ثابت حرف *bundle = get_bundle_راستہ (context->bundle, &bundle_allocated);
  cleanup_free حرف *owner = get_user_name (geteuid ());
  حرف *external_descriptors = libcrun_get_external_descriptors (container);
  حرف *rootfs = container->container_def->جڑ ? container->container_def->جڑ->راستہ : "";
  حرف created[35];

  libcrun_container_حالت_t حالت_2 = {
    .pid = pid,
    .rootfs = rootfs,
    .bundle = (حرف *) bundle,
    .created = created,
    .owner = owner,
    .systemd_cgroup = context->systemd_cgroup,
    .detached = context->detach,
    .no_new_keyring = context->no_new_keyring,
    .external_descriptors = external_descriptors,
    .cgroup_راستہ = تہی,
    .scope = تہی,
  };

  get_current_timestamp (created, حجم (created));

  اگر (cgroup_حالت)
    {
      صحیح ret;

      ret = libcrun_cgroup_get_حالت (cgroup_حالت, &حالت_2, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (external_descriptors == تہی)
    واپس crun_make_error (err, 0, "invalid internal state.  No external descriptors found");
  واپس libcrun_write_container_حالت (context->state_جڑ, context->id, &حالت_2, err);
}

ساکن صحیح
reap_subprocesses (pid_t *main_process, صحیح *main_process_exit, صحیح *آخری_process, libcrun_error_t *err)
{
  *آخری_process = 0;
  جب (1)
    {
      صحیح حالت_2;
      صحیح r = waitpid_ignore_stopped (-1, &حالت_2, WNOHANG);
      اگر (r < 0)
        {
          اگر (errno == EINTR)
            جاری;
          اگر (errno == ECHILD)
            {
              *آخری_process = 1;
              واپس 0;
            }
          واپس crun_make_error (err, errno, "waitpid");
        }
      اگر (r == 0)
        توڑو;
      اگر (r != *main_process)
        جاری;

      *main_process_exit = get_process_exit_حالت (حالت_2);

      /* Once reaped, the pid can be reused by an unrelated process.  */
      *main_process = 0;
    }
  واپس 0;
}

ساکن صحیح
send_sd_notify (ثابت حرف *ready_str, libcrun_error_t *err)
{
#اگر_تعریف_شدہ HAVE_SYSTEMD
  صحیح ret = sd_notify (0, ready_str);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, -ret, "sd_notify");

#  اگر HAVE_SD_NOTIFY_BARRIER
  /* Hard-code a 30 seconds timeout.  Ignore errors.  */
  sd_notify_barrier (0, 30 * 1000000);
#  endif
#ورنہ
  (خالی) ready_str;
  (خالی) err;
#اگر_ختم
  واپس 0;
}

ساکن صحیح
handle_notify_socket (صحیح notify_socketfd, libcrun_error_t *err)
{
#اگر_تعریف_شدہ HAVE_SYSTEMD
  صحیح ret;
  حرف buf[256];
  ثابت حرف *ready_str = "READY=1";

  ret = recvfrom (notify_socketfd, buf, حجم (buf) - 1, 0, تہی, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "recvfrom notify socket");

  buf[ret] = '\0';
  اگر (لڑی_تلاش (buf, ready_str))
    {
      ret = send_sd_notify (ready_str, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      واپس 1;
    }
  واپس 0;
#ورنہ
  (خالی) notify_socketfd;
  (خالی) err;
  واپس 1;
#اگر_ختم
}

ساخت wait_for_process_دلائل
{
  pid_t pid;
  libcrun_context_t *context;
  صحیح terminal_fd;
  صحیح notify_socket;
  صحیح *container_ready_fd;
  صحیح seccomp_notify_fd;
  ثابت حرف *seccomp_notify_plugins;
};

ساکن صحیح
write_pid_file (ثابت حرف *pid_file, pid_t pid, libcrun_error_t *err)
{
  حرف buf[32];
  صحیح buf_len = snprintf (buf, حجم (buf), "%d", pid);
  اگر (UNLIKELY (buf_len >= (صحیح) حجم (buf)))
    واپس crun_make_error (err, 0, "internal error: static buffer too small");
  واپس write_file_at_with_flags (AT_FDCWD, O_CREAT | O_TRUNC, 0700, pid_file, buf, buf_len, err);
}

#تعریف_کرو MAX_EVENTS 10

ساکن bool
is_fd_pollable (صحیح fd)
{
  ساخت epoll_event ev = { .events = EPOLLIN };
  cleanup_close صحیح epollfd = epoll_create1 (EPOLL_CLOEXEC);

  اگر (epollfd < 0)
    واپس true;

  واپس epoll_ctl (epollfd, EPOLL_CTL_جوڑو, fd, &ev) == 0 || errno != EPERM;
}

ساکن صحیح
wait_for_process (ساخت wait_for_process_دلائل *دلائل, libcrun_error_t *err)
{
  cleanup_channel_fd_pair ساخت channel_fd_pair *from_terminal = تہی;
  cleanup_channel_fd_pair ساخت channel_fd_pair *مقصد_terminal = تہی;
  صحیح ret, container_exit_رمز = 0, آخری_process;
  cleanup_close صحیح terminal_fd_from = -1;
  cleanup_close صحیح terminal_fd_مقصد = -1;
  bool stdin_pollable = false, stdout_pollable = false;
  cleanup_close صحیح epollfd = -1;
  cleanup_close صحیح signalfd = -1;
  sigset_t نقاب;
  صحیح اندر_fds[MAX_EVENTS];
  صحیح اندر_fds_len = 0;
  صحیح برآمد_fds[MAX_EVENTS];
  صحیح برآمد_fds_len = 0;
  size_t i;

  cleanup_seccomp_notify_context ساخت seccomp_notify_context_s *seccomp_notify_ctx = تہی;

  container_exit_رمز = 0;

  اگر (دلائل == تہی || دلائل->context == تہی)
    واپس crun_make_error (err, 0, "internal error: context is empty");

  برائے (i = 0; i < MAX_EVENTS; i++)
    {
      اندر_fds[i] = -1;
      برآمد_fds[i] = -1;
    }

  اگر (دلائل->context->pid_file)
    {
      ret = write_pid_file (دلائل->context->pid_file, دلائل->pid, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  /* Also exit if there is nothing more to wait for.  */
  اگر (دلائل->context->detach && دلائل->notify_socket < 0)
    واپس 0;

  اگر (دلائل->container_ready_fd)
    {
      ret = 0;
      عارضی_FAILURE_RETRY (اكتب (*دلائل->container_ready_fd, &ret, حجم (ret)));
      close_and_reset (دلائل->container_ready_fd);
    }

  sigfillset (&نقاب);
  ret = sigprocmask (SIG_بلاک, &نقاب, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "sigprocmask");

  signalfd = create_signalfd (&نقاب, err);
  اگر (UNLIKELY (signalfd < 0))
    واپس signalfd;

  ret = reap_subprocesses (&دلائل->pid, &container_exit_رمز, &آخری_process, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (آخری_process)
    واپس container_exit_رمز;

  اگر (دلائل->seccomp_notify_fd >= 0)
    {
      cleanup_free حرف *state_جڑ = تہی;
      cleanup_free حرف *oci_config_راستہ = تہی;

      ساخت libcrun_بھرو_seccomp_notify_conf_s conf;
      memset (&conf, 0, حجم conf);

      ret = libcrun_get_state_directory (&state_جڑ, دلائل->context->state_جڑ, دلائل->context->id, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = append_paths (&oci_config_راستہ, err, state_جڑ, "config.json", تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      conf.runtime_جڑ_راستہ = state_جڑ;
      conf.name = دلائل->context->id;
      conf.bundle_راستہ = دلائل->context->bundle;
      conf.oci_config_راستہ = oci_config_راستہ;

      ret = سیٹ_blocking_fd (دلائل->seccomp_notify_fd, false, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = libcrun_بھرو_seccomp_notify_plugins (&seccomp_notify_ctx,
                                                 دلائل->seccomp_notify_plugins,
                                                 &conf, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      اندر_fds[اندر_fds_len++] = دلائل->seccomp_notify_fd;
    }

  اگر (دلائل->terminal_fd >= 0)
    {
      /* The terminal_fd is dup()ed so that it can be registered with
         epoll multiple times using different masks.  */
      terminal_fd_from = dup (دلائل->terminal_fd);
      اگر (UNLIKELY (terminal_fd_from < 0))
        واپس crun_make_error (err, errno, "dup terminal fd");
      terminal_fd_مقصد = dup (دلائل->terminal_fd);
      اگر (UNLIKELY (terminal_fd_مقصد < 0))
        واپس crun_make_error (err, errno, "dup terminal fd");

      صحیح i, non_blocking_fds[] = { terminal_fd_from, terminal_fd_مقصد, 0, 1, -1 };
      برائے (i = 0; non_blocking_fds[i] >= 0; i++)
        {
          ret = سیٹ_blocking_fd (non_blocking_fds[i], false, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }

      /* stdin and stdout can be something epoll does not support, such as
         /dev/null or a regular file.  Such a stdin is not forwarded, and
         such a stdout is written to without waiting for it to be ready.  */
      stdin_pollable = is_fd_pollable (0);
      stdout_pollable = is_fd_pollable (1);

      from_terminal = channel_fd_pair_new (terminal_fd_from, 1, BUFSIZ);
      اگر (stdin_pollable)
        مقصد_terminal = channel_fd_pair_new (0, terminal_fd_مقصد, BUFSIZ);
    }

  اندر_fds[اندر_fds_len++] = signalfd;
  اگر (دلائل->notify_socket >= 0)
    اندر_fds[اندر_fds_len++] = دلائل->notify_socket;
  اگر (دلائل->terminal_fd >= 0)
    {
      اگر (stdin_pollable)
        {
          اندر_fds[اندر_fds_len++] = 0;
          برآمد_fds[برآمد_fds_len++] = terminal_fd_مقصد;
        }

      اندر_fds[اندر_fds_len++] = terminal_fd_from;
      اگر (stdout_pollable)
        برآمد_fds[برآمد_fds_len++] = 1;
    }

  epollfd = epoll_helper (اندر_fds, تہی, برآمد_fds, تہی, err);
  اگر (UNLIKELY (epollfd < 0))
    واپس epollfd;

  جب (1)
    {
      ساخت epoll_event events[MAX_EVENTS];
      ساخت signalfd_siginfo si;
      ساخت winsize ws;
      صحیح i, nr_events;
      ssize_t res;

      nr_events = عارضی_FAILURE_RETRY (epoll_wait (epollfd, events, MAX_EVENTS, -1));
      اگر (UNLIKELY (nr_events < 0))
        واپس crun_make_error (err, errno, "epoll_wait");

      برائے (i = 0; i < nr_events; i++)
        {
          اگر (events[i].ڈیٹا.fd == 0 || events[i].ڈیٹا.fd == terminal_fd_مقصد)
            {
              ret = channel_fd_pair_process (مقصد_terminal, epollfd, err);
              اگر (UNLIKELY (ret < 0))
                واپس crun_error_wrap (err, "copy to terminal fd");
            }
          ورنہ اگر (events[i].ڈیٹا.fd == 1 || events[i].ڈیٹا.fd == terminal_fd_from)
            {
              ret = channel_fd_pair_process (from_terminal, epollfd, err);
              اگر (UNLIKELY (ret < 0))
                واپس crun_error_wrap (err, "copy from terminal fd");
            }
          ورنہ اگر (events[i].ڈیٹا.fd == دلائل->seccomp_notify_fd)
            {
              ret = libcrun_seccomp_notify_plugins (seccomp_notify_ctx,
                                                    دلائل->seccomp_notify_fd, err);
              اگر (UNLIKELY (ret < 0))
                واپس ret;
            }
          ورنہ اگر (events[i].ڈیٹا.fd == دلائل->notify_socket)
            {
              ret = handle_notify_socket (دلائل->notify_socket, err);
              اگر (UNLIKELY (ret < 0))
                واپس ret;
              اگر (ret && دلائل->context->detach)
                واپس 0;
            }
          ورنہ اگر (events[i].ڈیٹا.fd == signalfd)
            {
              res = عارضی_FAILURE_RETRY (اقرأ (signalfd, &si, حجم (si)));
              اگر (UNLIKELY (res < 0))
                واپس crun_make_error (err, errno, "read from signalfd");
              اگر (si.ssi_signo == SIGCHLD)
                {
                  ret = reap_subprocesses (&دلائل->pid, &container_exit_رمز,
                                           &آخری_process, err);
                  اگر (UNLIKELY (ret < 0))
                    واپس ret;
                  اگر (آخری_process)
                    واپس container_exit_رمز;
                }
              ورنہ اگر (si.ssi_signo == SIGWINCH)
                {
                  /* Ignore the signal if the terminal is not available.  */
                  اگر (دلائل->terminal_fd > 0)
                    {
                      ret = ioctl (0, TIOCGWINSZ, &ws);
                      اگر (UNLIKELY (ret < 0))
                        واپس crun_make_error (err, errno, "ioctl TIOCGWINSZ copy terminal size from stdin");

                      ret = ioctl (دلائل->terminal_fd, TIOCSWINSZ, &ws);
                      اگر (UNLIKELY (ret < 0))
                        واپس crun_make_error (err, errno, "ioctl TIOCSWINSZ copy terminal size to pty");
                    }
                }
              ورنہ اگر (دلائل->pid > 0)
                {
                  /* Send any other signal to the child process, if it was not reaped yet.  */
                  ret = kill (دلائل->pid, si.ssi_signo);
                }
            }
          ورنہ
            {
              واپس crun_make_error (err, 0, "internal error: unknown fd from epoll_wait");
            }
        }
    }

  واپس 0;
}

ساکن خالی
flush_fd_مقصد_err (libcrun_context_t *context, صحیح terminal_fd)
{
  حرف buf[256];
  صحیح flags;

  اگر (terminal_fd < 0 || stderr == تہی)
    واپس;

  flags = fcntl (terminal_fd, F_GETFL, 0);
  اگر (flags == -1)
    واپس;
  اگر (fcntl (terminal_fd, F_SETFL, flags | O_NONBLOCK) < 0)
    واپس;

  برائے (;;)
    {
      صحیح ret = عارضی_FAILURE_RETRY (اقرأ (terminal_fd, buf, حجم (buf) - 1));
      اگر (ret <= 0)
        توڑو;
      buf[ret] = '\0';
      اگر (context->output_handler)
        context->output_handler (0, buf, LIBCRUN_VERBOSITY_ERROR, context->output_handler_arg);
    }
  (خالی) fcntl (terminal_fd, F_SETFL, flags);
  fflush (stderr);
  fsync (1);
  fsync (2);
}

ساکن صحیح
cleanup_watch (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
               ساخت libcrun_cgroup_حالت *cgroup_حالت, pid_t init_pid, صحیح sync_socket,
               صحیح terminal_fd, libcrun_error_t *err)
{
  ثابت حرف *oom_پیغام = تہی;
  libcrun_error_t tmp_err = تہی;
  صحیح ret;

  اگر (init_pid)
    {
      /* Try to detect whether the cgroup has a OOM.  */
      اگر (cgroup_حالت)
        {
          صحیح has_oom;

          has_oom = libcrun_cgroup_has_oom (cgroup_حالت, &tmp_err);
          اگر (has_oom > 0)
            oom_پیغام = "OOM: the memory limit could be too low";
          ورنہ اگر (has_oom < 0)
            {
              /* If the detection has failed for any reason, e.g. the cgroup was
                 already deleted by the time it was checked, just ignore the
                 failure.  */
              crun_error_release (&tmp_err);
            }
        }
      /* If the OOM wasn't detected, look into the static configuration.  */
      اگر (oom_پیغام == تہی && is_حافظہ_حد_بندی_too_کم (def))
        oom_پیغام = "the memory limit could be too low";

      kill (init_pid, SIGKILL);
      waitpid_ignore_stopped (init_pid, تہی, 0);
    }

  ret = sync_socket_wait_sync (context, sync_socket, true, &tmp_err);
  اگر (UNLIKELY (ret < 0))
    {
      crun_error_release (err);
      *err = tmp_err;
    }

  اگر (terminal_fd >= 0)
    flush_fd_مقصد_err (context, terminal_fd);

  اگر (oom_پیغام)
    واپس crun_error_wrap (err, "%s", oom_پیغام);

  واپس -1;
}

/* Find the uid:gid that is mapped to root inside the container user namespace.  */
خالی
get_جڑ_اندر_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid, uid_t *uid,
                        gid_t *gid)
{
  *uid = -1;
  *gid = -1;

  /* Not root in the namespace.  */
  اگر (host_uid)
    واپس;

  اگر (def->linux && def->linux->uid_mappings)
    {
      size_t i;

      برائے (i = 0; i < def->linux->uid_mappings_len; i++)
        اگر (def->linux->uid_mappings[i]->container_id == 0)
          {
            *uid = def->linux->uid_mappings[i]->host_id;
            توڑو;
          }
    }
  اگر (def->linux && def->linux->gid_mappings)
    {
      size_t i;

      برائے (i = 0; i < def->linux->gid_mappings_len; i++)
        اگر (def->linux->gid_mappings[i]->container_id == 0)
          {
            *gid = def->linux->gid_mappings[i]->host_id;
            توڑو;
          }
    }

  /* If the uid and the gid are not changed, do not attempt any chown.  */
  اگر (*uid == host_uid && *gid == host_gid)
    *uid = *gid = -1;
}

ساکن صحیح
get_seccomp_receiver_fd (libcrun_container_t *container, صحیح *fd, صحیح *self_receiver_fd, ثابت حرف **plugins,
                         libcrun_error_t *err)
{
  ثابت حرف *tmp;
  runtime_spec_schema_config_schema *def = container->container_def;

  *fd = -1;
  *self_receiver_fd = -1;

  tmp = ڈھونڈو_annotation (container, "run.oci.seccomp.plugins");
  اگر (tmp)
    {
      صحیح fds[2];
      صحیح ret;

      ret = create_socket_pair (fds, err);
      اگر (UNLIKELY (ret < 0))
        واپس crun_error_wrap (err, "create socket pair");

      *fd = fds[0];
      *self_receiver_fd = fds[1];
      *plugins = tmp;
      واپس 0;
    }

  اگر (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_راستہ)
    tmp = def->linux->seccomp->listener_راستہ;
  ورنہ
    tmp = ڈھونڈو_annotation (container, "run.oci.seccomp.receiver");
  اگر (tmp == تہی)
    tmp = getenv ("RUN_OCI_SECCOMP_RECEIVER");
  اگر (tmp)
    {
      اگر (tmp[0] != '/')
        واپس crun_make_error (err, 0, "the seccomp receiver `%s` is not an absolute path", tmp);

      *fd = open_unix_domain_client_socket (tmp, 0, err);
      اگر (UNLIKELY (*fd < 0))
        واپس crun_error_wrap (err, "open seccomp receiver");
    }

  واپس 0;
}

ساکن bool
has_seccomp_receiver (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;

  اگر (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_راستہ)
    واپس true;

  اگر (ڈھونڈو_annotation (container, "run.oci.seccomp.receiver") != تہی || getenv ("RUN_OCI_SECCOMP_RECEIVER") != تہی)
    واپس true;

  واپس false;
}

ساکن صحیح
تیاری_container_hooks_output (libcrun_container_t *container, runtime_spec_schema_config_schema *def,
                              ساخت container_entrypoint_s *container_دلائل, صحیح *hooks_برآمد_fd,
                              صحیح *hooks_err_fd, libcrun_error_t *err)
{
  صحیح ret;

  اگر (def->hooks
      && (def->hooks->prestart_len || def->hooks->poststart_len || def->hooks->create_runtime_len
          || def->hooks->create_container_len || def->hooks->آغاز_container_len))
    {
      ret = open_hooks_output (container, hooks_برآمد_fd, hooks_err_fd, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
      container_دلائل->hooks_برآمد_fd = *hooks_برآمد_fd;
      container_دلائل->hooks_err_fd = *hooks_err_fd;
    }
  واپس 0;
}

ساکن صحیح
تیاری_container_keyring (libcrun_container_t *container, libcrun_context_t *context, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحیح ret;

  اگر (! context->no_new_keyring)
    {
      ثابت حرف *label = تہی;

      libcrun_debug ("Creating new keyring");

      اگر (def->process)
        {
          label = def->process->selinux_label;
          اگر (label)
            libcrun_debug ("Using SELinux process label: `%s`", label);
        }

      ret = libcrun_create_keyring (container, container->context->id, label, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  واپس 0;
}

ساکن صحیح
تیاری_terminal_socket_pair (libcrun_container_t *container, libcrun_context_t *context,
                            ساخت container_entrypoint_s *container_دلائل, صحیح *socket_pair_0,
                            صحیح *socket_pair_1, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحیح ret;
  صحیح detach = context->detach;

  اگر (def->process && def->process->terminal && ! detach && context->console_socket == تہی)
    {
      libcrun_debug ("Creating terminal socket pair");
      container_دلائل->has_terminal_socket_pair = 1;
      ret = create_socket_pair (container_دلائل->terminal_socketpair, err);
      اگر (UNLIKELY (ret < 0))
        واپس crun_error_wrap (err, "create terminal socket");

      *socket_pair_0 = container_دلائل->terminal_socketpair[0];
      *socket_pair_1 = container_دلائل->terminal_socketpair[1];
    }
  واپس 0;
}

ساکن صحیح
تیاری_seccomp (libcrun_container_t *container, ثابت حرف *seccomp_bpf_ڈیٹا,
               ساخت libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, صحیح *seccomp_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحیح ret;

  اگر (ڈھونڈو_annotation (container, "run.oci.seccomp.plugins") != تہی && has_seccomp_receiver (container))
    {
      واپس crun_make_error (err, 0, "seccomp plugins and seccomp receivers cannot be declared at the same time");
    }

  اگر (def->linux && (def->linux->seccomp || seccomp_bpf_ڈیٹا))
    {
      غیرعلامتی صحیح seccomp_gen_options = 0;
      ثابت حرف *annotation;

      libcrun_debug ("Initializing seccomp");
      annotation = ڈھونڈو_annotation (container, "run.oci.seccomp_fail_unknown_syscall");
      اگر (annotation && strcmp (annotation, "0") != 0)
        seccomp_gen_options = LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL;

      اگر (seccomp_bpf_ڈیٹا)
        seccomp_gen_options |= LIBCRUN_SECCOMP_SKIP_CACHE;

      libcrun_seccomp_gen_ctx_init (seccomp_gen_ctx, container, true, seccomp_gen_options);

      ret = libcrun_open_seccomp_bpf (seccomp_gen_ctx, seccomp_fd, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  واپس 0;
}

ساکن صحیح
تیاری_console_socket (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
                      ساخت container_entrypoint_s *container_دلائل, صحیح *console_socket_fd,
                      libcrun_error_t *err)
{
  اگر (context->console_socket && def->process && def->process->terminal)
    {
      *console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
      اگر (UNLIKELY (*console_socket_fd < 0))
        واپس crun_error_wrap (err, "open console socket");
      container_دلائل->console_socket_fd = *console_socket_fd;
    }
  واپس 0;
}

ساکن صحیح
تیاری_cgroup_manager (libcrun_context_t *context, libcrun_container_t *container,
                      ساخت libcrun_cgroup_دلائل *cg, صحیح *cgroup_dirfd,
                      ساخت libcrun_dirfd_s *cgroup_dirfd_s, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحیح cgroup_manager;
  uid_t جڑ_uid = -1;
  gid_t جڑ_gid = -1;
  صحیح ret;

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  اگر (context->systemd_cgroup)
    {
      libcrun_debug ("Using systemd cgroup manager");
      cgroup_manager = CGROUP_MANAGER_SYSTEMD;
    }
  ورنہ اگر (context->force_no_cgroup)
    {
      libcrun_debug ("Disabling cgroup manager");
      cgroup_manager = CGROUP_MANAGER_DISABLED;
    }
  ورنہ
    libcrun_debug ("Using cgroupfs cgroup manager");

  /* If we are root (either on the host or in a namespace), then chown the cgroup to root
     in the container user namespace.  */
  get_جڑ_اندر_the_userns (def, container->host_uid, container->host_gid, &جڑ_uid, &جڑ_gid);
  libcrun_debug ("Using container host UID `%d` and GID `%d`", container->host_uid, container->host_gid);

  memset (cg, 0, حجم (*cg));

  cg->cgroup_راستہ = def->linux ? def->linux->cgroups_راستہ : "";
  cg->manager = cgroup_manager;
  cg->id = context->id;
  cg->resources = def->linux ? def->linux->resources : تہی;
  cg->annotations = container->annotations;
  cg->جڑ_uid = جڑ_uid;
  cg->جڑ_gid = جڑ_gid;
  cg->state_جڑ = context->state_جڑ;
  cg->container = container;

  ret = libcrun_cgroup_preenter (cg, cgroup_dirfd, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  cgroup_dirfd_s->dirfd = cgroup_dirfd;
  cgroup_dirfd_s->joined = false;

  واپس 0;
}

ساکن صحیح
سیٹ_scheduler (pid_t pid, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  صحیح ret;

  ret = libcrun_سیٹ_scheduler (pid, def->process, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_reset_cpu_affinity_نقاب (pid, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_سیٹ_io_ترجیح (pid, def->process, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس 0;
}

ساکن صحیح
seccomp_generation (صحیح seccomp_fd, ثابت حرف *seccomp_bpf_ڈیٹا,
                    ساخت libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, libcrun_error_t *err)
{
  صحیح ret;

  اگر (seccomp_fd >= 0)
    {
      اگر (seccomp_bpf_ڈیٹا != تہی)
        {
          ret = libcrun_copy_seccomp (seccomp_gen_ctx, seccomp_bpf_ڈیٹا, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }
      ورنہ
        {
          ret = libcrun_generate_seccomp (seccomp_gen_ctx, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }
    }
  واپس 0;
}

ساکن صحیح
terminal_تیاری (runtime_spec_schema_config_schema *def, libcrun_context_t *context,
                صحیح socket_pair_0, صحیح *terminal_fd, خالی **orig_terminal, libcrun_error_t *err)
{
  صحیح ret;
  صحیح detach = context->detach;

  اگر (def->process && def->process->terminal && ! detach && context->console_socket == تہی)
    {
      libcrun_debug ("Receiving console socket fd");
      *terminal_fd = receive_fd_from_socket (socket_pair_0, err);
      اگر (UNLIKELY (*terminal_fd < 0))
        واپس -1;

      ret = libcrun_clear_onlcr (*terminal_fd, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = libcrun_سیٹ_raw (0, orig_terminal, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  واپس 0;
}

ساکن صحیح
libcrun_container_تسلسل_اندرونی (libcrun_container_t *container, libcrun_context_t *context,
                                صحیح *container_ready_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحیح ret;
  pid_t pid;
  صحیح detach = context->detach;
  cleanup_cgroup_حالت ساخت libcrun_cgroup_حالت *cgroup_حالت = تہی;
  cleanup_close صحیح terminal_fd = -1;
  cleanup_terminal خالی *orig_terminal = تہی;
  cleanup_close صحیح sync_socket = -1;
  cleanup_close صحیح notify_socket = -1;
  cleanup_close صحیح socket_pair_0 = -1;
  cleanup_close صحیح socket_pair_1 = -1;
  cleanup_close صحیح seccomp_fd = -1;
  cleanup_close صحیح console_socket_fd = -1;
  cleanup_close صحیح hooks_برآمد_fd = -1;
  cleanup_close صحیح hooks_err_fd = -1;
  cleanup_close صحیح own_seccomp_receiver_fd = -1;
  cleanup_close صحیح seccomp_notify_fd = -1;
  ثابت حرف *seccomp_notify_plugins = تہی;
  cleanup_cgroup_preenter ساخت libcrun_cgroup_دلائل cg = {};
  cleanup_free حرف *bundle_allocated = تہی;
  ساخت container_entrypoint_s container_دلائل = {
    .container = container,
    .context = context,
    .bundle = get_bundle_راستہ (context->bundle, &bundle_allocated),
    .terminal_socketpair = { -1, -1 },
    .console_socket_fd = -1,
    .hooks_برآمد_fd = -1,
    .hooks_err_fd = -1,
    .seccomp_receiver_fd = -1,
    .custom_handler = تہی,
  };
  cleanup_close صحیح cgroup_dirfd = -1;
  ساخت libcrun_dirfd_s cgroup_dirfd_s;
  ساخت libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  ثابت حرف *seccomp_bpf_ڈیٹا = ڈھونڈو_annotation (container, "run.oci.seccomp_bpf_data");
  صحیح cgroup_منوال;

  اگر (! context->force_no_cgroup)
    {
      cgroup_منوال = libcrun_get_cgroup_منوال (err);
      اگر (UNLIKELY (cgroup_منوال < 0))
        واپس cgroup_منوال;

      اگر (cgroup_منوال != CGROUP_منوال_UNIFIED)
        libcrun_warning ("cgroup v1 is deprecated and will be removed in a future release.  Use cgroup v2");
    }

  ret = تیاری_container_hooks_output (container, def, &container_دلائل, &hooks_برآمد_fd, &hooks_err_fd, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  container->context = context;

  اگر (! detach || context->notify_socket)
    {
      libcrun_debug ("Setting child subreaper");
      ret = prctl (PR_سیٹ_بچہ_SUBREAPER, 1, 0, 0, 0);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "prctl set child subreaper");
    }

  ret = تیاری_container_keyring (container, context, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = تیاری_terminal_socket_pair (container, context, &container_دلائل, &socket_pair_0, &socket_pair_1, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = بلاک_اشارات (err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  umask (0);

  ret = libcrun_سیٹ_mempolicy (def, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = تیاری_seccomp (container, seccomp_bpf_ڈیٹا, &seccomp_gen_ctx, &seccomp_fd, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;
  container_دلائل.seccomp_fd = seccomp_fd;

  اگر (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &container_دلائل.seccomp_receiver_fd, &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  ret = تیاری_console_socket (context, def, &container_دلائل, &console_socket_fd, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = تیاری_cgroup_manager (context, container, &cg, &cgroup_dirfd, &cgroup_dirfd_s, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_configure_handler (container_دلائل.context->handler_manager,
                                   container_دلائل.context,
                                   container,
                                   &(container_دلائل.custom_handler),
                                   err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (container_دلائل.custom_handler && container_دلائل.custom_handler->vtable->modify_oci_configuration)
    {
      libcrun_debug ("Using custom handler to modify OCI configuration");
      ret = container_دلائل.custom_handler->vtable->modify_oci_configuration (container_دلائل.custom_handler->cookie,
                                                                             container_دلائل.context,
                                                                             container->container_def,
                                                                             err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  pid = libcrun_تسلسل_linux_container (container, container_init, &container_دلائل, &sync_socket, &cgroup_dirfd_s, err);
  اگر (UNLIKELY (pid < 0))
    واپس pid;

  cg.pid = pid;
  cg.joined = cgroup_dirfd_s.joined;
  libcrun_debug ("Running container on PID: `%d`", pid);

  اگر (context->fifo_exec_wait_fd < 0 && context->notify_socket)
    {
      libcrun_debug ("Using notify socket: `%s`", context->notify_socket);
      /* Do not open the notify socket here on "create".  "start" will take care of it.  */
      ret = get_notify_fd (context, container, &notify_socket, err);
      اگر (UNLIKELY (ret < 0))
        جاؤ fail;
    }

  اگر (container_دلائل.terminal_socketpair[1] >= 0)
    close_and_reset (&socket_pair_1);

  ret = libcrun_cgroup_enter (&cg, &cgroup_حالت, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ fail;

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ fail;

  ret = libcrun_move_جال_devices (container, pid, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ fail;

  /* sync send own pid.  */
  ret = عارضی_FAILURE_RETRY (اكتب (sync_socket, &pid, حجم (pid)));
  اگر (UNLIKELY (ret != حجم (pid)))
    {
      اگر (ret >= 0)
        errno = 0;
      crun_make_error (err, errno, "write to sync socket");
      جاؤ fail;
    }

  /* sync 1.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ fail;

  /* sync 2.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ fail;

  ret = libcrun_cgroup_enter_finalize (&cg, cgroup_حالت, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ fail;

  /* If the container process is created later, the scheduler is set on it then.  */
  اگر (! libcrun_container_delayed_pidns_fork (container))
    {
      ret = سیٹ_scheduler (pid, def, err);
      اگر (UNLIKELY (ret < 0))
        جاؤ fail;
    }

  /* The container is waiting that we write back.  In this phase we can launch the
     prestart hooks.  */
  اگر (def->hooks && def->hooks->prestart_len)
    {
      libcrun_debug ("Running `prestart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "prestart", (hook **) def->hooks->prestart,
                      def->hooks->prestart_len, hooks_برآمد_fd, hooks_err_fd, false, err);
      اگر (UNLIKELY (ret != 0))
        جاؤ fail;
    }
  اگر (def->hooks && def->hooks->create_runtime_len)
    {
      libcrun_debug ("Running `create` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "createRuntime", (hook **) def->hooks->create_runtime,
                      def->hooks->create_runtime_len, hooks_برآمد_fd, hooks_err_fd, false, err);
      اگر (UNLIKELY (ret != 0))
        جاؤ fail;
    }

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_ڈیٹا, &seccomp_gen_ctx, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ fail;
  close_and_reset (&seccomp_fd);

  /* sync 3.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ fail;

  /* The process joining an existing PID namespace is created once the container root is in place.  */
  اگر (libcrun_container_delayed_pidns_fork (container))
    {
      pid_t new_pid = 0;

      ret = sync_socket_wait_sync (context, sync_socket, false, err);
      اگر (UNLIKELY (ret < 0))
        جاؤ fail;

      ret = عارضی_FAILURE_RETRY (اقرأ (sync_socket, &new_pid, حجم (new_pid)));
      اگر (UNLIKELY (ret != حجم (new_pid)))
        {
          اگر (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "read pid from sync socket");
          جاؤ fail;
        }

      /* Cleanup the first process.  */
      waitpid_ignore_stopped (pid, تہی, 0);

      pid = new_pid;

      ret = سیٹ_scheduler (pid, def, err);
      اگر (UNLIKELY (ret < 0))
        جاؤ fail;

      ret = libcrun_handle_pidfd_receiver (pid, container, err);
      اگر (UNLIKELY (ret < 0))
        جاؤ fail;

      /* sync send own pid.  */
      ret = عارضی_FAILURE_RETRY (اكتب (sync_socket, &pid, حجم (pid)));
      اگر (UNLIKELY (ret != حجم (pid)))
        {
          اگر (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "write to sync socket");
          جاؤ fail;
        }
    }

  ret = terminal_تیاری (def, context, socket_pair_0, &terminal_fd, &orig_terminal, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ fail;

  close_and_reset (&socket_pair_0);

  /* sync 4.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ fail;

  ret = close_and_reset (&sync_socket);
  اگر (UNLIKELY (ret < 0))
    {
      crun_make_error (err, errno, "close sync_socket failed");
      جاؤ fail;
    }

  libcrun_debug ("Writing container status");
  ret = write_container_حالت (container, context, pid, cgroup_حالت, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ fail;

  /* Run poststart hooks here only if the container is created using "run".  For create+start, the
     hooks will be executed as part of the start command.  */
  اگر (context->fifo_exec_wait_fd < 0 && def->hooks && def->hooks->poststart_len)
    {
      libcrun_debug ("Running `poststart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_برآمد_fd, hooks_err_fd, false, err);
      اگر (UNLIKELY (ret != 0))
        جاؤ fail;
    }

  /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
  اگر (own_seccomp_receiver_fd >= 0)
    {
      libcrun_debug ("Receiving seccomp fd");
      seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
      اگر (UNLIKELY (seccomp_notify_fd < 0))
        جاؤ fail;

      ret = close_and_reset (&own_seccomp_receiver_fd);
      اگر (UNLIKELY (ret < 0))
        {
          crun_make_error (err, errno, "close seccomp receiver fd failed");
          جاؤ fail;
        }
    }

  {
    ساخت wait_for_process_دلائل دلائل = {
      .pid = pid,
      .context = context,
      .terminal_fd = terminal_fd,
      .notify_socket = notify_socket,
      .container_ready_fd = container_ready_fd,
      .seccomp_notify_fd = seccomp_notify_fd,
      .seccomp_notify_plugins = seccomp_notify_plugins,
    };
    ret = wait_for_process (&دلائل, err);
  }
  اگر (! context->detach)
    {
      libcrun_error_t tmp_err = تہی;
      cleanup_watch (context, def, cgroup_حالت, 0, sync_socket, terminal_fd, &tmp_err);
      crun_error_release (&tmp_err);
    }

  واپس ret;

fail:
  ret = cleanup_watch (context, def, cgroup_حالت, pid, sync_socket, terminal_fd, err);
  اگر (cgroup_حالت)
    {
      libcrun_error_t tmp_err = تہی;
      libcrun_cgroup_destroy (cgroup_حالت, &tmp_err);
      crun_error_release (&tmp_err);
    }
  واپس ret;
}

ساکن صحیح
check_config_file (runtime_spec_schema_config_schema *def, libcrun_context_t *context, libcrun_error_t *err)
{
  اگر (UNLIKELY (def->linux == تہی))
    واپس crun_make_error (err, 0, "invalid config file, no `linux` block specified");

  اگر (context->handler == تہی)
    {
      اگر (UNLIKELY (def->جڑ == تہی))
        واپس crun_make_error (err, 0, "invalid config file, no `root` block specified");
      اگر (UNLIKELY (def->mounts == تہی))
        واپس crun_make_error (err, 0, "invalid config file, no `mounts` block specified");
    }
  واپس 0;
}

ساکن صحیح
libcrun_copy_config_file (ثابت حرف *id, ثابت حرف *state_جڑ, libcrun_container_t *container, libcrun_error_t *err)
{
  صحیح ret;
  cleanup_free حرف *dest_راستہ = تہی;
  cleanup_free حرف *dir = تہی;
  cleanup_free حرف *بفر = تہی;
  size_t len;

  ret = libcrun_get_state_directory (&dir, state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = append_paths (&dest_راستہ, err, dir, "config.json", تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (container->config_file == تہی && container->config_file_content == تہی)
    واپس crun_make_error (err, 0, "config file not specified");

  اگر (container->config_file == تہی)
    {
      libcrun_debug ("Writing config file to: `%s`", dest_راستہ);
      ret = write_file (dest_راستہ, container->config_file_content, لڑی_لمبائی (container->config_file_content), err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  ورنہ
    {
      libcrun_debug ("Reading config file: `%s`", container->config_file);
      ret = read_all_file (container->config_file, &بفر, &len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      libcrun_debug ("Writing config file to: `%s`", dest_راستہ);
      ret = write_file (dest_راستہ, بفر, len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  واپس 0;
}

ساکن خالی
force_delete_container_حالت (libcrun_context_t *context, libcrun_container_t *container,
                               runtime_spec_schema_config_schema *def)
{
  libcrun_error_t tmp_err = تہی;
  container_delete_اندرونی (context, container, def, context->id, true, false, &tmp_err);
  libcrun_error_report_and_release (&tmp_err);
}

/* Serialize ERR to FD as an int errno followed by a NUL-terminated message.
   Used by forked children to report a failure to the parent process.  */
ساکن خالی
write_error_مقصد_pipe (صحیح fd, libcrun_error_t *err)
{
  ثابت حرف *پیغام = (*err)->پیغام;
  صحیح errcode = crun_error_get_errno (err);

  عارضی_FAILURE_RETRY (اكتب (fd, &errcode, حجم (errcode)));
  عارضی_FAILURE_RETRY (اكتب (fd, پیغام, لڑی_لمبائی (پیغام) + 1));
}

صحیح
libcrun_container_تسلسل (libcrun_context_t *context, libcrun_container_t *container, غیرعلامتی صحیح options,
                       libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحیح ret;
  صحیح detach = context->detach;
  صحیح container_ret_حالت[2];
  cleanup_close صحیح pipefd0 = -1;
  cleanup_close صحیح pipefd1 = -1;
  libcrun_error_t tmp_err = تہی;

  container->context = context;

  ret = validate_options (options, LIBCRUN_تسلسل_OPTIONS_PREFORK | LIBCRUN_تسلسل_OPTIONS_KEEP, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = check_config_file (def, context, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (def->process && def->process->terminal && detach && context->console_socket == تہی)
    واپس crun_make_error (err, 0, "use --console-socket with --detach when a terminal is used");

  ret = libcrun_حالت_check_directories (context->state_جڑ, context->id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (! detach && (options & LIBCRUN_تسلسل_OPTIONS_PREFORK) == 0)
    {
      ret = libcrun_copy_config_file (context->id, context->state_جڑ, container, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = libcrun_container_تسلسل_اندرونی (container, context, تہی, err);
      اگر (! (options & LIBCRUN_تسلسل_OPTIONS_KEEP))
        force_delete_container_حالت (context, container, def);
      واپس ret;
    }

  ret = pipe2 (container_ret_حالت, O_CLOEXEC);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_حالت[0];
  pipefd1 = container_ret_حالت[1];

  ret = fork ();
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "fork");
  اگر (ret)
    {
      صحیح حالت_2;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, تہی, 0);

      ret = عارضی_FAILURE_RETRY (اقرأ (pipefd0, &حالت_2, حجم (حالت_2)));
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, errno, "read from sync pipe");

      اگر (حالت_2 < 0)
        {
          صحیح errno_;
          حرف buf[512];
          ret = عارضی_FAILURE_RETRY (اقرأ (pipefd0, &errno_, حجم (errno_)));
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "read from sync pipe");

          ret = عارضی_FAILURE_RETRY (اقرأ (pipefd0, buf, حجم (buf) - 1));
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "read from sync pipe");
          buf[ret] = '\0';

          واپس crun_make_error (err, errno_, "%s", buf);
        }

      واپس حالت_2;
    }

  close_and_reset (&pipefd0);

  /* forked process.  */
  ret = detach_process ();
  اگر (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_جڑ, container, &tmp_err);
  اگر (UNLIKELY (ret < 0))
    جاؤ fail;

  ret = libcrun_container_تسلسل_اندرونی (container, context, تہی, &tmp_err);
  عارضی_FAILURE_RETRY (اكتب (pipefd1, &ret, حجم (ret)));
  اگر (UNLIKELY (ret < 0))
    جاؤ fail;

  باہر_نکلو (EXIT_SUCCESS);
fail:

  اگر (! (options & LIBCRUN_تسلسل_OPTIONS_KEEP))
    force_delete_container_حالت (context, container, def);
  اگر (tmp_err)
    {
      write_error_مقصد_pipe (pipefd1, &tmp_err);
      crun_error_release (&tmp_err);
    }

  باہر_نکلو (EXIT_FAILURE);
}

صحیح
libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container, غیرعلامتی صحیح options,
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
    واپس ret;

  ret = check_config_file (def, context, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (def->process && def->process->terminal && context->console_socket == تہی)
    واپس crun_make_error (err, 0, "use --console-socket with create when a terminal is used");

  ret = libcrun_حالت_check_directories (context->state_جڑ, context->id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  exec_fifo_fd = libcrun_حالت_create_exec_fifo (context->state_جڑ, context->id, err);
  اگر (UNLIKELY (exec_fifo_fd < 0))
    واپس exec_fifo_fd;

  context->fifo_exec_wait_fd = exec_fifo_fd;
  exec_fifo_fd = -1;

  اگر ((options & LIBCRUN_تسلسل_OPTIONS_PREFORK) == 0)
    {
      libcrun_debug ("Running with prefork enabled");
      ret = libcrun_copy_config_file (context->id, context->state_جڑ, container, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
      ret = libcrun_container_تسلسل_اندرونی (container, context, تہی, err);
      اگر (UNLIKELY (ret < 0))
        force_delete_container_حالت (context, container, def);
      واپس ret;
    }

  ret = pipe2 (container_ready_pipe, O_CLOEXEC);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "pipe");
  pipefd0 = container_ready_pipe[0];
  pipefd1 = container_ready_pipe[1];

  ret = fork ();
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "fork");
  اگر (ret)
    {
      صحیح exit_رمز;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, تہی, 0);

      ret = عارضی_FAILURE_RETRY (اقرأ (pipefd0, &exit_رمز, حجم (exit_رمز)));
      اگر (UNLIKELY (ret <= 0))
        واپس crun_make_error (err, ret < 0 ? errno : 0, "waiting for container to be ready");

      اگر (ret != حجم (exit_رمز) || exit_رمز < 0)
        واپس crun_make_error (err, 0, "internal error: read invalid exit code");

      اگر (exit_رمز == 0)
        واپس 0;

      libcrun_debug ("Exit code is `%d`, deleting container", exit_رمز);
      libcrun_error_t tmp_err = تہی;
      container_delete_اندرونی (context, تہی, def, context->id, true, true, &tmp_err);
      libcrun_error_report_and_release (&tmp_err);
      واپس crun_make_error (err, 0, "error creating container");
    }

  /* forked process.  */
  ret = detach_process ();
  اگر (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_جڑ, container, err);
  اگر (UNLIKELY (ret < 0))
    {
      صحیح errcode = crun_error_get_errno (err);
      crun_error_release (err);
      libcrun_fail_with_error (errcode, "copy config file");
    }

  ret = libcrun_container_تسلسل_اندرونی (container, context, &pipefd1, err);
  اگر (UNLIKELY (ret < 0))
    {
      force_delete_container_حالت (context, container, def);
      libcrun_error ((*err)->حالت_2, "%s", (*err)->پیغام);
      crun_error_release (err);
      crun_سیٹ_output_handler (log_write_مقصد_stderr, تہی);
    }

  اگر (pipefd1 >= 0)
    عارضی_FAILURE_RETRY (اكتب (pipefd1, &ret, حجم (ret)));
  باہر_نکلو (ret ? EXIT_FAILURE : 0);
}

صحیح
libcrun_container_آغاز (libcrun_context_t *context, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = تہی;
  ثابت حرف *state_جڑ = context->state_جڑ;
  runtime_spec_schema_config_schema *def;
  cleanup_container_حالت libcrun_container_حالت_t حالت_2 = {};
  cleanup_close صحیح fd = -1;
  صحیح ret;

  ret = libcrun_read_container_حالت (&حالت_2, state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_is_container_running (&حالت_2, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (! ret)
    واپس crun_make_error (err, 0, "container `%s` is not running", id);

  ret = read_container_config_from_state (&container, state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (context->notify_socket)
    {
      ret = get_notify_fd (context, container, &fd, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  ret = libcrun_حالت_write_exec_fifo (context->state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  def = container->container_def;

  اگر (context->notify_socket)
    {
      اگر (fd >= 0)
        {
          fd_set read_سیٹ;

          جب (1)
            {
              ساخت timeval timeout = {
                .tv_sec = 0,
                .tv_usec = 10000,
              };
              FD_ZERO (&read_سیٹ);
              FD_SET (fd, &read_سیٹ);

              ret = select (fd + 1, &read_سیٹ, تہی, تہی, &timeout);
              اگر (UNLIKELY (ret < 0))
                واپس crun_make_error (err, errno, "select failed");
              اگر (ret)
                {
                  ret = handle_notify_socket (fd, err);
                  اگر (UNLIKELY (ret < 0))
                    واپس ret;
                  اگر (ret)
                    توڑو;
                }
              ورنہ
                {
                  ret = libcrun_is_container_running (&حالت_2, err);
                  اگر (UNLIKELY (ret < 0))
                    واپس ret;
                  اگر (! ret)
                    واپس 0;
                }
            }
        }
    }

  /* The container is considered running only after we got the notification from the
     notify_socket, if any.  */
  اگر (def->hooks && def->hooks->poststart_len)
    {
      cleanup_close صحیح hooks_برآمد_fd = -1;
      cleanup_close صحیح hooks_err_fd = -1;

      ret = open_hooks_output (container, &hooks_برآمد_fd, &hooks_err_fd, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = do_hooks (def, حالت_2.pid, context->id, false, حالت_2.bundle, حالت_2.bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_برآمد_fd, hooks_err_fd, false, err);
      اگر (UNLIKELY (ret != 0))
        {
          libcrun_error_t tmp_err = تہی;
          container_delete_اندرونی (context, container, def, id, true, true, &tmp_err);
          libcrun_error_report_and_release (&tmp_err);
          واپس ret;
        }
    }

  واپس 0;
}

صحیح
libcrun_get_container_state_string (ثابت حرف *id, libcrun_container_حالت_t *حالت_2, ثابت حرف *state_جڑ,
                                    ثابت حرف **container_حالت, صحیح *running, libcrun_error_t *err)
{
  صحیح ret, has_fifo = 0;
  bool paused = false;

  ret = libcrun_is_container_running (حالت_2, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;
  *running = ret;

  اگر (*running)
    {
      ret = libcrun_حالت_has_read_exec_fifo (state_جڑ, id, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
      has_fifo = ret;
    }

  اگر (*running && ! has_fifo)
    {
      cleanup_cgroup_حالت ساخت libcrun_cgroup_حالت *cgroup_حالت = تہی;

      cgroup_حالت = libcrun_cgroup_make_حالت (حالت_2);

      ret = libcrun_cgroup_is_container_paused (cgroup_حالت, &paused, err);
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
              *container_حالت = "stopped";
              واپس 0;
            }

          واپس ret;
        }
    }

  اگر (! *running)
    *container_حالت = "stopped";
  ورنہ اگر (has_fifo)
    *container_حالت = "created";
  ورنہ اگر (paused)
    *container_حالت = "paused";
  ورنہ
    *container_حالت = "running";

  واپس 0;
}

صحیح
libcrun_container_state_json (libcrun_context_t *context, ثابت حرف *id, حرف **برآمد, libcrun_error_t *err)
{
  حرف *بفر = تہی;
  size_t len = 0;
  FILE *f;
  صحیح ret;

  f = open_memstream (&بفر, &len);
  اگر (f == تہی)
    واپس crun_make_error (err, errno, "open_memstream");

  ret = libcrun_container_state (context, id, f, err);
  fclose (f);
  اگر (UNLIKELY (ret < 0))
    {
      آزاد (بفر);
      واپس ret;
    }

  *برآمد = بفر;
  واپس 0;
}

صحیح
libcrun_container_state (libcrun_context_t *context, ثابت حرف *id, FILE *برآمد, libcrun_error_t *err)
{
  ثابت حرف *ثابت OCI_CONFIG_نسخہ = "1.0.0";
  libcrun_container_حالت_t حالت_2 = {};
  ثابت حرف *state_جڑ = context->state_جڑ;
  ثابت حرف *container_حالت = تہی;
  json_gen_ctx *gen = تہی;
  ثابت حرف *buf;
  صحیح ret = 0;
  صحیح r = json_gen_حالت_ok;
  صحیح running;
  size_t len;

  ret = libcrun_read_container_حالت (&حالت_2, state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_get_container_state_string (id, &حالت_2, state_جڑ, &container_حالت, &running, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ باہر_نکلو;

  ret = 0;
  اگر (! json_gen_init (&gen, تہی))
    واپس crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));
  GEN_کلید (gen, "ociVersion");
  GEN_STR (gen, OCI_CONFIG_نسخہ);

  GEN_کلید (gen, "id");
  GEN_STR (gen, id);

  GEN_کلید (gen, "pid");
  GEN_OR_FAIL (map_int (gen, running ? حالت_2.pid : 0));

  GEN_کلید (gen, "status");
  GEN_STR (gen, container_حالت);

  GEN_کلید (gen, "bundle");
  GEN_STR (gen, حالت_2.bundle);

  GEN_کلید (gen, "rootfs");
  GEN_STR (gen, حالت_2.rootfs);

  GEN_کلید (gen, "created");
  GEN_STR (gen, حالت_2.created);

  اگر (حالت_2.scope)
    {
      GEN_کلید (gen, "systemd-scope");
      GEN_STR (gen, حالت_2.scope);
    }

  اگر (حالت_2.owner)
    {
      GEN_کلید (gen, "owner");
      GEN_STR (gen, حالت_2.owner);
    }

  {
    cleanup_container libcrun_container_t *container = تہی;

    ret = read_container_config_from_state (&container, state_جڑ, id, err);
    اگر (UNLIKELY (ret < 0))
      جاؤ باہر_نکلو;

    اگر (container->container_def->annotations && container->container_def->annotations->len)
      GEN_OR_FAIL (gen_annotations (gen, container->container_def->annotations));
  }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  fprintf (برآمد, "%s\n", buf);

باہر_نکلو:
  اگر (gen)
    json_gen_free (gen);
  libcrun_free_container_حالت (&حالت_2);
  واپس ret;

gen_error:
  ret = json_gen_error_مقصد_crun_error (r, err);
  جاؤ باہر_نکلو;
}

صحیح
libcrun_container_exec (libcrun_context_t *context, ثابت حرف *id, runtime_spec_schema_config_schema_process *process,
                        libcrun_error_t *err)
{
  ساخت libcrun_container_exec_options_s opts;
  memset (&opts, 0, حجم (opts));

  opts.struct_سائز = حجم (opts);
  opts.process = process;

  واپس libcrun_container_exec_with_options (context, id, &opts, err);
}

صحیح
libcrun_container_exec_process_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *راستہ, libcrun_error_t *err)
{
  ساخت libcrun_container_exec_options_s opts;
  memset (&opts, 0, حجم (opts));

  opts.struct_سائز = حجم (opts);
  opts.راستہ = راستہ;

  واپس libcrun_container_exec_with_options (context, id, &opts, err);
}

ساکن صحیح
exec_from_process_json (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *process_json, ثابت حرف *cgroup,
                        bool ملاؤ_env, libcrun_error_t *err)
{
  ساخت parser_context parser_ctx = { .options = 0, .errfile = stderr };
  ساخت libcrun_container_exec_options_s opts;
  runtime_spec_schema_config_schema_process *process;
  parser_error parser_err = تہی;
  json_object *doc = تہی;
  صحیح ret;

  ret = parse_json_file (&doc, process_json, &parser_ctx, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  process = make_runtime_spec_schema_config_schema_process (doc, &parser_ctx, &parser_err);
  json_object_put (doc);
  اگر (process == تہی)
    {
      ret = crun_make_error (err, 0, "cannot parse process: `%s`", parser_err ? parser_err : "unknown error");
      آزاد (parser_err);
      واپس ret;
    }

  memset (&opts, 0, حجم (opts));
  opts.struct_سائز = حجم (opts);
  opts.process = process;
  opts.cgroup = cgroup;
  opts.ملاؤ_env = ملاؤ_env;

  ret = libcrun_container_exec_with_options (context, id, &opts, err);
  free_runtime_spec_schema_config_schema_process (process);
  واپس ret;
}

صحیح
libcrun_container_exec_json (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *process_json, libcrun_error_t *err)
{
  واپس exec_from_process_json (context, id, process_json, تہی, false, err);
}

صحیح
libcrun_container_exec_with_options_json (libcrun_context_t *context, ثابت حرف *id,
                                          ثابت ساخت libcrun_exec_options_s *opts, libcrun_error_t *err)
{
  اگر (opts == تہی || opts->struct_سائز == 0)
    واپس crun_make_error (err, EINVAL, "invalid exec options");

  واپس exec_from_process_json (context, id, opts->process_json, opts->cgroup, opts->ملاؤ_env, err);
}

#تعریف_کرو cleanup_process_schema __attribute__ ((cleanup (cleanup_process_schemap)))

ساکن اندرونی خالی
cleanup_process_schemap (runtime_spec_schema_config_schema_process **p)
{
  runtime_spec_schema_config_schema_process *process = *p;
  اگر (process)
    (خالی) free_runtime_spec_schema_config_schema_process (process);
}

/* Build the seccomp receiver payload (when needed), apply the seccomp profile and
   close the seccomp file descriptors.  Used from exec_process_entrypoint both before
   and after the capabilities are set, depending on process->no_new_privileges.  */
ساکن صحیح
apply_seccomp_for_exec (libcrun_container_t *container, pid_t own_pid, صحیح seccomp_fd,
                        صحیح seccomp_receiver_fd, حرف **seccomp_flags, size_t seccomp_flags_len,
                        libcrun_error_t *err)
{
  cleanup_free حرف *seccomp_fd_payload = تہی;
  size_t seccomp_fd_payload_len = 0;
  صحیح ret;

  اگر (seccomp_receiver_fd >= 0)
    {
      ret = get_seccomp_receiver_fd_payload (container, "running", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  ret = libcrun_apply_seccomp (seccomp_fd, seccomp_receiver_fd, seccomp_fd_payload,
                               seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  close_and_reset (&seccomp_fd);
  close_and_reset (&seccomp_receiver_fd);
  واپس 0;
}

ساکن صحیح
exec_process_entrypoint (libcrun_context_t *context,
                         libcrun_container_t *container,
                         runtime_spec_schema_config_schema_process *process,
                         صحیح *pipefd1,
                         صحیح seccomp_fd,
                         صحیح seccomp_receiver_fd,
                         ساخت custom_handler_instance_s *custom_handler,
                         bool ملاؤ_env,
                         libcrun_error_t *err)
{
  runtime_spec_schema_config_schema_process_capabilities *capabilities = تہی;
  cleanup_free حرف *exec_راستہ = تہی;
  uid_t container_uid;
  gid_t container_gid;
  ثابت حرف *cwd;
  bool chdir_مکمل = false;
  size_t seccomp_flags_len = 0;
  حرف **seccomp_flags = تہی;
  pid_t own_pid = 0;
  size_t i;
  صحیح ret;

  container_uid = process->user ? process->user->uid : 0;
  container_gid = process->user ? process->user->gid : 0;

  عارضی_FAILURE_RETRY (اقرأ (*pipefd1, &own_pid, حجم (own_pid)));

  cwd = process->cwd ? process->cwd : "/";
  اگر (LIKELY (libcrun_محفوظ_chdir (cwd, err) == 0))
    chdir_مکمل = true;
  ورنہ
    crun_error_release (err);

  ret = unblock_اشارات (err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = clearenv ();
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "clearenv");

  اگر ((ملاؤ_env || ! process->env_len) && container->container_def->process && container->container_def->process->env_len)
    {
      حرف *e;

      برائے (i = 0; i < container->container_def->process->env_len; i++)
        {
          e = container->container_def->process->env[i];
          اگر (putenv (e) < 0)
            واپس crun_make_error (err, errno, "putenv `%s`", e);
        }
    }
  اگر (process->env_len)
    {
      برائے (i = 0; i < process->env_len; i++)
        اگر (putenv (process->env[i]) < 0)
          واپس crun_make_error (err, errno, "putenv `%s`", process->env[i]);
    }

  اگر (is_خالی_خانے_string (getenv ("HOME")))
    {
      ret = سیٹ_home_env (container_uid);
      اگر (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          اگر (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            واپس crun_make_error (err, errno, "setenv HOME");
        }
    }

  ret = libcrun_سیٹ_selinux_label (container, process, false, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_سیٹ_apparmor_profile (container, process, false, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (container->container_def->linux && container->container_def->linux->seccomp)
    {
      seccomp_flags = container->container_def->linux->seccomp->flags;
      seccomp_flags_len = container->container_def->linux->seccomp->flags_len;
    }

  ret = ڈھونڈو_executable (&exec_راستہ, process->دلائل[0], process->cwd, err);
  اگر (UNLIKELY (ret < 0))
    {
      اگر (crun_error_get_errno (err) == ENOENT)
        واپس ret;

      /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
         once the process switched to the use that runs in the container.  This might be necessary
         when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
         is not honored.  */
      crun_error_release (err);
    }

  اگر (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_سیٹ_personality (container->container_def->linux->personality, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, false, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_reopen_dev_null (err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (! process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  ret = libcrun_container_setgroups (container, process, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = maybe_chown_std_streams (container_uid, container_gid, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (process->capabilities)
    capabilities = process->capabilities;
  ورنہ اگر (container->container_def->process)
    capabilities = container->container_def->process->capabilities;

  ret = libcrun_سیٹ_caps (capabilities, container_uid, container_gid, process->no_new_privileges,
                          custom_handler != تہی, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (UNLIKELY (exec_راستہ == تہی))
    {
      ret = ڈھونڈو_executable (&exec_راستہ, process->دلائل[0], process->cwd, err);
      اگر (UNLIKELY (ret < 0))
        {
          اگر (custom_handler == تہی || is_خالی_خانے_string (process->دلائل[0]))
            واپس ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          exec_راستہ = xstrdup (process->دلائل[0]);
        }
    }

  اگر (! chdir_مکمل)
    {
      ret = libcrun_محفوظ_chdir (cwd, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (process->user)
    umask (process->user->umask_present ? process->user->umask : 0022);

  ret = 0;
  عارضی_FAILURE_RETRY (اكتب (*pipefd1, &ret, حجم (ret)));
  عارضی_FAILURE_RETRY (close (*pipefd1));
  *pipefd1 = -1;

  اگر (custom_handler)
    {
      اگر (custom_handler->vtable->exec_func == تہی)
        واپس crun_make_error (err, 0, "the handler does not support exec");

      اگر (context->argv)
        erase_initial_environment (context->argv, context->argc);

      ret = custom_handler->vtable->exec_func (custom_handler->cookie,
                                               container,
                                               exec_راستہ,
                                               process->دلائل);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, -ret, "exec container process failed with handler as `%s`", custom_handler->vtable->name);

      محفوظ_exit (EXIT_FAILURE);
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation, because the seccomp filter is already in place and it could
     stop some syscalls used by mark_or_close_fds_ge_than.
  */
  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, true, err);
  اگر (UNLIKELY (ret < 0))
    crun_error_release (err);

  عارضی_FAILURE_RETRY (execv (exec_راستہ, process->دلائل));
  libcrun_fail_with_error (errno, "exec `%s`", exec_راستہ);
}

/* Close the file descriptors inherited from the caller, other than those
   to be passed to the container process.  */
صحیح
libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err)
{
  واپس mark_or_close_fds_ge_than (تہی, context->preserve_fds + 3, true, err);
}

صحیح
libcrun_بھرو_process_from_file (ثابت حرف *راستہ, runtime_spec_schema_config_schema_process **process,
                                libcrun_error_t *err)
{
  ساخت parser_context ctx = { 0, stderr };
  cleanup_free حرف *content = تہی;
  parser_error parser_err = تہی;
  json_object *doc = تہی;
  size_t len;
  صحیح ret;

  ret = read_all_file (راستہ, &content, &len, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  *process = make_runtime_spec_schema_config_schema_process (doc, &ctx, &parser_err);
  اگر (UNLIKELY (*process == تہی))
    {
      ret = crun_make_error (err, 0, "cannot parse process file: `%s`", parser_err);
      آزاد (parser_err);
      اگر (doc)
        json_object_put (doc);
      واپس ret;
    }

  آزاد (parser_err);
  اگر (doc)
    json_object_put (doc);

  واپس 0;
}

صحیح
libcrun_container_exec_with_options (libcrun_context_t *context, ثابت حرف *id,
                                     ساخت libcrun_container_exec_options_s *opts,
                                     libcrun_error_t *err)
{
  cleanup_custom_handler_instance ساخت custom_handler_instance_s *custom_handler = تہی;
  صحیح container_حالت, ret;
  bool container_paused = false;
  pid_t pid;
  libcrun_container_حالت_t حالت_2 = {};
  ثابت حرف *state_جڑ = context->state_جڑ;
  cleanup_close صحیح terminal_fd = -1;
  cleanup_close صحیح seccomp_fd = -1;
  cleanup_terminal خالی *orig_terminal = تہی;
  cleanup_container libcrun_container_t *container = تہی;
  صحیح container_ret_حالت[2];
  cleanup_close صحیح pipefd0 = -1;
  cleanup_close صحیح pipefd1 = -1;
  cleanup_close صحیح seccomp_receiver_fd = -1;
  cleanup_close صحیح own_seccomp_receiver_fd = -1;
  cleanup_close صحیح seccomp_notify_fd = -1;
  ثابت حرف *seccomp_notify_plugins = تہی;
  __attribute__ ((غیر_مستعمل)) cleanup_process_schema runtime_spec_schema_config_schema_process *process_cleanup = تہی;
  runtime_spec_schema_config_schema_process *process = opts->process;
  ساخت libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  صحیح ret_from_بچہ = 0;

  ret = libcrun_read_container_حالت (&حالت_2, state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_is_container_running (&حالت_2, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;
  container_حالت = ret;

  ret = read_container_config_from_state (&container, state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  container->context = context;

  اگر (container_حالت == 0)
    واپس crun_make_error (err, 0, "the container `%s` is not running", id);

  {
    cleanup_cgroup_حالت ساخت libcrun_cgroup_حالت *cgroup_حالت = تہی;

    cgroup_حالت = libcrun_cgroup_make_حالت (&حالت_2);

    ret = libcrun_cgroup_is_container_paused (cgroup_حالت, &container_paused, err);
    اگر (UNLIKELY (ret < 0))
      واپس ret;
  }

  اگر (UNLIKELY (container_paused))
    واپس crun_make_error (err, 0, "cannot exec in a paused container `%s`", id);

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = بلاک_اشارات (err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  libcrun_seccomp_gen_ctx_init (&seccomp_gen_ctx, container, false, 0);

  ret = libcrun_open_seccomp_bpf (&seccomp_gen_ctx, &seccomp_fd, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &seccomp_receiver_fd,
                                     &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  اگر (حجم (*opts) != opts->struct_سائز)
    واپس crun_make_error (err, EINVAL, "invalid libcrun_container_exec_options_s struct");

  اگر (opts->راستہ)
    {
      اگر (process)
        واپس crun_make_error (err, EINVAL, "cannot specify both exec file and options");

      ret = libcrun_بھرو_process_from_file (opts->راستہ, &process, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      process_cleanup = process;
    }

  /* This must be done before we enter a user namespace.  If the process does
     not specify any rlimits, use the ones from the container configuration.  */
  اگر (process->rlimits_len == 0 && container->container_def->process)
    ret = libcrun_سیٹ_rlimits (container->container_def->process->rlimits,
                               container->container_def->process->rlimits_len, err);
  ورنہ
    ret = libcrun_سیٹ_rlimits (process->rlimits, process->rlimits_len, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_سیٹ_mempolicy (container->container_def, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = pipe2 (container_ret_حالت, O_CLOEXEC);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_حالت[0];
  pipefd1 = container_ret_حالت[1];

  /* If the new process block doesn't specify a SELinux label, AppArmor profile, user,
     I/O priority or scheduler, then use the configuration from the original config file.  */
  اگر (container->container_def->process)
    {
      اگر (process->io_ترجیح == تہی && container->container_def->process->io_ترجیح)
        {
          process->io_ترجیح = clone_runtime_spec_schema_config_schema_process_io_ترجیح (container->container_def->process->io_ترجیح);
          اگر (process->io_ترجیح == تہی)
            OOM ();
        }

      اگر (process->scheduler == تہی && container->container_def->process->scheduler)
        {
          process->scheduler = clone_runtime_spec_schema_config_schema_process_scheduler (container->container_def->process->scheduler);
          اگر (process->scheduler == تہی)
            OOM ();
        }

      اگر (process->selinux_label == تہی && container->container_def->process->selinux_label)
        process->selinux_label = xstrdup (container->container_def->process->selinux_label);

      اگر (process->apparmor_profile == تہی && container->container_def->process->apparmor_profile)
        process->apparmor_profile = xstrdup (container->container_def->process->apparmor_profile);

      اگر (process->user == تہی && container->container_def->process->user)
        {
          process->user = clone_runtime_spec_schema_config_schema_process_user (container->container_def->process->user);
          اگر (process->user == تہی)
            OOM ();
        }
    }

  ret = initialize_security (container, process, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  /* Do not leak the session keyring of the caller into the container.  The
     keyring created for the container cannot be joined, so use an anonymous one.  */
  اگر (! context->no_new_keyring && ! حالت_2.no_new_keyring)
    {
      libcrun_debug ("Creating new keyring for the exec process");

      ret = libcrun_create_keyring (container, تہی, process->selinux_label, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  ret = prctl (PR_سیٹ_DUMPABLE, 0, 0, 0, 0);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "prctl unset dumpable");

  pid = libcrun_join_process (context, container, حالت_2.pid, &حالت_2, opts->cgroup, context->detach,
                              process, process->terminal ? &terminal_fd : تہی, err);
  اگر (UNLIKELY (pid < 0))
    واپس pid;

  /* Process to exec.  */
  اگر (pid == 0)
    {
      عارضی_FAILURE_RETRY (close (pipefd0));
      pipefd0 = -1;

      exec_process_entrypoint (context, container, process, &pipefd1, seccomp_fd, seccomp_receiver_fd, custom_handler, opts->ملاؤ_env, err);
      /* It gets here only on errors.  */
      اگر (*err)
        {
          اگر (pipefd1 < 0)
            libcrun_fail_with_error ((*err)->حالت_2, "%s", (*err)->پیغام);
          ورنہ
            {
              write_error_مقصد_pipe (pipefd1, err);
              عارضی_FAILURE_RETRY (close (pipefd1));
              pipefd1 = -1;
            }
        }
      محفوظ_exit (EXIT_FAILURE);
    }

  عارضی_FAILURE_RETRY (close (pipefd1));
  pipefd1 = -1;

  عارضی_FAILURE_RETRY (اكتب (pipefd0, &pid, حجم (pid)));

  اگر (seccomp_fd >= 0)
    close_and_reset (&seccomp_fd);

  اگر (terminal_fd >= 0)
    {
      غیرعلامتی مختصر صفیں = 0, ستون_تعداد = 0;

      اگر (process->console_سائز)
        {
          ستون_تعداد = process->console_سائز->چوڑائی;
          صفیں = process->console_سائز->اونچائی;
        }

      ret = libcrun_terminal_تیاری_سائز (terminal_fd, صفیں, ستون_تعداد, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      اگر (context->console_socket)
        {
          صحیح ret;
          cleanup_close صحیح console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
          اگر (UNLIKELY (console_socket_fd < 0))
            واپس console_socket_fd;
          ret = send_fd_مقصد_socket (console_socket_fd, terminal_fd, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
          close_and_reset (&terminal_fd);
        }
      ورنہ
        {
          ret = libcrun_clear_onlcr (terminal_fd, err);
          اگر (UNLIKELY (ret < 0))
            {
              flush_fd_مقصد_err (context, terminal_fd);
              واپس ret;
            }

          ret = libcrun_سیٹ_raw (0, &orig_terminal, err);
          اگر (UNLIKELY (ret < 0))
            {
              flush_fd_مقصد_err (context, terminal_fd);
              واپس ret;
            }
        }
    }

  ret = عارضی_FAILURE_RETRY (اقرأ (pipefd0, &ret_from_بچہ, حجم (ret_from_بچہ)));
  اگر (ret != حجم (ret_from_بچہ))
    ret = crun_make_error (err, 0, "read pipe failed");
  ورنہ اگر (ret_from_بچہ != 0)
    {
      cleanup_free حرف *پیغام = تہی;
      size_t len = 0;

      ret = read_all_fd (pipefd0, "error stream", &پیغام, &len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
      /* the string from read_all_fd is always NUL terminated.  */
      ret = crun_make_error (err, ret_from_بچہ, "%s", پیغام);
    }
  ورنہ
    {
      /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
      اگر (own_seccomp_receiver_fd >= 0)
        {
          seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
          اگر (UNLIKELY (seccomp_notify_fd < 0))
            واپس seccomp_notify_fd;

          ret = close_and_reset (&own_seccomp_receiver_fd);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "close seccomp receiver fd failed");
        }

      {
        ساخت wait_for_process_دلائل دلائل = {
          .pid = pid,
          .context = context,
          .terminal_fd = terminal_fd,
          .notify_socket = -1,
          .container_ready_fd = تہی,
          .seccomp_notify_fd = seccomp_notify_fd,
          .seccomp_notify_plugins = seccomp_notify_plugins,
        };

        ret = wait_for_process (&دلائل, err);
      }
    }

  flush_fd_مقصد_err (context, terminal_fd);
  واپس ret;
}

صحیح
libcrun_container_update (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *content, size_t len arg_غیر_مستعمل,
                          libcrun_error_t *err)
{
  cleanup_custom_handler_instance ساخت custom_handler_instance_s *custom_handler = تہی;
  runtime_spec_schema_config_linux_resources *resources = تہی;
  cleanup_container libcrun_container_t *container = تہی;
  ثابت حرف *state_جڑ = context->state_جڑ;
  ساخت parser_context ctx = { 0, stderr };
  libcrun_container_حالت_t حالت_2 = {};
  parser_error parser_err = تہی;
  json_object *doc = تہی;
  صحیح ret;

  ret = libcrun_read_container_حالت (&حالت_2, state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = read_container_config_from_state (&container, state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  resources = make_runtime_spec_schema_config_linux_resources (doc, &ctx, &parser_err);
  اگر (UNLIKELY (resources == تہی))
    {
      ret = crun_make_error (err, 0, "cannot parse resources: %s", parser_err);
      جاؤ cleanup;
    }

  اگر (custom_handler && custom_handler->vtable->modify_oci_configuration)
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
      اگر (UNLIKELY (ret < 0))
        جاؤ cleanup;
    }

  ret = libcrun_linux_container_update (&حالت_2, state_جڑ, resources, err);

cleanup:
  اگر (doc)
    json_object_put (doc);
  آزاد (parser_err);
  اگر (resources)
    free_runtime_spec_schema_config_linux_resources (resources);

  واپس ret;
}

صحیح
libcrun_container_update_from_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *file, libcrun_error_t *err)
{
  cleanup_free حرف *content = تہی;
  size_t len;
  صحیح ret;

  ret = read_all_file (file, &content, &len, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس libcrun_container_update (context, id, content, len, err);
}

ساکن صحیح
compare_update_اقدار (ثابت خالی *a, ثابت خالی *b)
{
  ثابت ساخت libcrun_update_قدر_s *aa = a;
  ثابت ساخت libcrun_update_قدر_s *bb = b;
  صحیح ret;

  ret = strcmp (aa->section, bb->section);
  اگر (ret)
    واپس ret;
  واپس strcmp (aa->name, bb->name);
}

صحیح
libcrun_container_update_from_اقدار (libcrun_context_t *context, ثابت حرف *id,
                                      ساخت libcrun_update_قدر_s *اقدار, size_t len,
                                      libcrun_error_t *err)
{
  ثابت حرف *current_section = تہی;
  ثابت حرف *buf;
  json_gen_ctx *gen = تہی;
  size_t i, buf_len;
  صحیح ret;

  اگر (! json_gen_init (&gen, تہی))
    واپس crun_make_error (err, 0, "json_gen_init failed");
  json_gen_map_open (gen);

  qsort (اقدار, len, حجم (ساخت libcrun_update_قدر_s), compare_update_اقدار);

  برائے (i = 0; i < len; i++)
    {
      اگر (current_section == تہی || strcmp (اقدار[i].section, current_section))
        {
          اگر (i > 0)
            json_gen_map_close (gen);

          current_section = اقدار[i].section;
          json_gen_string (gen, current_section, لڑی_لمبائی (current_section));
          json_gen_map_open (gen);
        }

      json_gen_string (gen, اقدار[i].name, لڑی_لمبائی (اقدار[i].name));

      اگر (اقدار[i].numeric)
        json_gen_نمبر (gen, اقدار[i].قدر, لڑی_لمبائی (اقدار[i].قدر));
      ورنہ
        json_gen_string (gen, اقدار[i].قدر, لڑی_لمبائی (اقدار[i].قدر));
    }
  اگر (len)
    json_gen_map_close (gen);

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);

  ret = libcrun_container_update (context, id, buf, buf_len, err);

  json_gen_free (gen);

  واپس ret;
}

ساکن خالی
populate_array_میدان (حرف ***میدان, حرف *array[], size_t عدد_elements)
{
  size_t i;

  *میدان = xmalloc0 ((عدد_elements + 1) * حجم (حرف *));
  برائے (i = 0; i < عدد_elements; i++)
    (*میدان)[i] = xstrdup (array[i]);

  (*میدان)[i] = تہی;
}

#اگر_تعریف_شدہ HAVE_CAP
ساکن خالی
populate_capabilities (ساخت features_info_s *info, حرف ***capabilities, size_t *عدد_capabilities)
{
  size_t اشاریہ = 0;
  cap_قدر_t i;
  حرف *endptr;
  صحیح j;

  *عدد_capabilities = 0;
  برائے (i = 0;; i++)
    {
      حرف *v = cap_مقصد_name (i);
      اگر (v == تہی)
        توڑو;
      strtol (v, &endptr, 10);
      اگر (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          توڑو;
        }
      (*عدد_capabilities)++;
    }

  *capabilities = xmalloc0 ((*عدد_capabilities + 1) * حجم (ثابت حرف *));
  برائے (i = 0; i < (cap_قدر_t) *عدد_capabilities; i++)
    {
      حرف *v = cap_مقصد_name (i);
      اگر (v == تہی)
        توڑو;
      strtol (v, &endptr, 10);
      اگر (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          توڑو;
        }

      // Convert capability name to uppercase
      برائے (j = 0; v[j] != '\0'; j++)
        v[j] = بڑا_حرف (v[j]);

      (*capabilities)[اشاریہ] = v;
      اشاریہ++;
    }

  (*capabilities)[اشاریہ] = تہی; // Terminate the array with NULL
  populate_array_میدان (&(info->linux.capabilities), *capabilities, *عدد_capabilities);
}
#اگر_ختم

ساکن خالی
retrieve_mount_options (ساخت features_info_s **info)
{
  cleanup_free ثابت ساخت اشاعت_flags_s *mount_options_list = تہی;
  size_t عدد_mount_options = 0;

  // Retrieve mount options from wordlist
  mount_options_list = get_mount_flags_from_wordlist ();

  // Calculate the number of mount options
  جب (mount_options_list[عدد_mount_options].name != تہی)
    عدد_mount_options++;

  // Allocate memory for mount options in info struct
  (*info)->mount_options = xmalloc0 ((عدد_mount_options + 1) * حجم (حرف *));

  // Copy mount options to info struct
  برائے (size_t i = 0; i < عدد_mount_options; i++)
    (*info)->mount_options[i] = xstrdup (mount_options_list[i].name);
}

صحیح
libcrun_container_get_features (libcrun_context_t *context, ساخت features_info_s **info, libcrun_error_t *err arg_غیر_مستعمل)
{
  // Allocate memory for the features_info_s structure
  size_t عدد_namspaces = حجم (namespaces) / حجم (namespaces[0]);
  size_t عدد_operators = حجم (operators) / حجم (operators[0]);
  size_t عدد_actions = حجم (actions) / حجم (actions[0]);
  size_t عدد_hooks = حجم (hooks) / حجم (hooks[0]);
  size_t عدد_archs = حجم (archs) / حجم (archs[0]);
  size_t عدد_unsafe_annotations = حجم (potentially_unsafe_annotations) / حجم (potentially_unsafe_annotations[0]);
  cleanup_free حرف **capabilities = تہی;
  size_t عدد_capabilities = 0;
  size_t عدد_mempolicy_modes = حجم (mempolicy_modes) / حجم (mempolicy_modes[0]);
  size_t عدد_mempolicy_flags = حجم (mempolicy_flags) / حجم (mempolicy_flags[0]);

  *info = xmalloc0 (حجم (ساخت features_info_s));

  // Hardcoded feature information
  (*info)->oci_نسخہ_min = xstrdup ("1.0.0");
  (*info)->oci_نسخہ_max = xstrdup ("1.1.0+dev");

  // Populate hooks
  populate_array_میدان (&((*info)->hooks), hooks, عدد_hooks);

  // Populate mount_options
  retrieve_mount_options (info);

  // Populate namespaces
  populate_array_میدان (&((*info)->linux.namespaces), namespaces, عدد_namspaces);

#اگر_تعریف_شدہ HAVE_CAP
  // Populate capabilities
  populate_capabilities (*info, &capabilities, &عدد_capabilities);
#اگر_ختم

  // Hardcode the values for cgroup
  (*info)->linux.cgroup.v1 = true;
  (*info)->linux.cgroup.v2 = true;
#اگر_تعریف_شدہ HAVE_SYSTEMD
  (*info)->linux.cgroup.systemd = true;
  (*info)->linux.cgroup.systemd_user = true;
#اگر_ختم

  // Put seccomp values
#اگر_تعریف_شدہ HAVE_SECCOMP
  (*info)->linux.seccomp.enabled = true;
  // Populate actions
  populate_array_میدان (&((*info)->linux.seccomp.actions), actions, عدد_actions);
  // Populate operators
  populate_array_میدان (&((*info)->linux.seccomp.operators), operators, عدد_operators);
  // Populate archs
  populate_array_میدان (&((*info)->linux.seccomp.archs), archs, عدد_archs);
#ورنہ
  (*info)->linux.seccomp.enabled = false;
#اگر_ختم

  // Put values for apparmor and selinux
  (*info)->linux.apparmor.enabled = true;
  (*info)->linux.selinux.enabled = true;

  (*info)->linux.intel_rdt.enabled = true;

  (*info)->linux.net_devices.enabled = true;

  populate_array_میدان (&((*info)->linux.حافظہ_policy.منوال), mempolicy_modes, عدد_mempolicy_modes);
  populate_array_میدان (&((*info)->linux.حافظہ_policy.flags), mempolicy_flags, عدد_mempolicy_flags);

  // Put the values for mount extensions
  (*info)->linux.mount_ext.idmap.enabled = true;

  // Populate the values for annotations
#اگر_تعریف_شدہ HAVE_SECCOMP
  {
    ثابت ساخت scmp_نسخہ *نسخہ = seccomp_نسخہ ();
    حرف *نسخہ_string = تہی;

    xasprintf (&نسخہ_string, "%u.%u.%u", نسخہ->major, نسخہ->minor, نسخہ->micro);
    (*info)->annotations.io_github_seccomp_libseccomp_نسخہ = نسخہ_string;
  }
#اگر_ختم

  اگر (context->handler_manager && handler_by_name (context->handler_manager, "wasm"))
    (*info)->annotations.تسلسل_oci_crun_wasm = true;

#اگر HAVE_CRIU
  (*info)->annotations.تسلسل_oci_crun_checkpoint_enabled = true;
#اگر_ختم
  (*info)->annotations.تسلسل_oci_crun_commit = GIT_نسخہ;
  (*info)->annotations.تسلسل_oci_crun_نسخہ = PACKAGE_نسخہ;

  populate_array_میدان (&((*info)->potentially_unsafe_annotations), potentially_unsafe_annotations, عدد_unsafe_annotations);

  واپس 0;
}

ساکن خالی
features_جوڑو_string (json_gen_ctx *json_gen, ثابت حرف *کلید, ثابت حرف *قدر)
{
  json_gen_string (json_gen, کلید, لڑی_لمبائی (کلید));
  json_gen_string (json_gen, قدر, لڑی_لمبائی (قدر));
}

ساکن خالی
features_جوڑو_bool (json_gen_ctx *json_gen, ثابت حرف *کلید, صحیح قدر)
{
  json_gen_string (json_gen, کلید, لڑی_لمبائی (کلید));
  json_gen_bool (json_gen, قدر);
}

ساکن خالی
features_جوڑو_bool_str (json_gen_ctx *json_gen, ثابت حرف *کلید, صحیح قدر)
{
  features_جوڑو_string (json_gen, کلید, قدر ? "true" : "false");
}

ساکن خالی
features_جوڑو_array (json_gen_ctx *json_gen, ثابت حرف *کلید, حرف **array)
{
  size_t i;

  json_gen_string (json_gen, کلید, لڑی_لمبائی (کلید));
  json_gen_array_open (json_gen);
  اگر (array)
    برائے (i = 0; array[i] != تہی; i++)
      json_gen_string (json_gen, array[i], لڑی_لمبائی (array[i]));
  json_gen_array_close (json_gen);
}

ساکن خالی
features_جوڑو_linux_info (json_gen_ctx *json_gen, ثابت ساخت linux_info_s *linux)
{
  json_gen_string (json_gen, "linux", لڑی_لمبائی ("linux"));
  json_gen_map_open (json_gen);

  features_جوڑو_array (json_gen, "namespaces", linux->namespaces);
  features_جوڑو_array (json_gen, "capabilities", linux->capabilities);

  json_gen_string (json_gen, "cgroup", لڑی_لمبائی ("cgroup"));
  json_gen_map_open (json_gen);
  features_جوڑو_bool (json_gen, "v1", linux->cgroup.v1);
  features_جوڑو_bool (json_gen, "v2", linux->cgroup.v2);
  features_جوڑو_bool (json_gen, "systemd", linux->cgroup.systemd);
  features_جوڑو_bool (json_gen, "systemdUser", linux->cgroup.systemd_user);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "seccomp", لڑی_لمبائی ("seccomp"));
  json_gen_map_open (json_gen);
  features_جوڑو_bool (json_gen, "enabled", linux->seccomp.enabled);
  اگر (linux->seccomp.actions)
    features_جوڑو_array (json_gen, "actions", linux->seccomp.actions);
  اگر (linux->seccomp.operators)
    features_جوڑو_array (json_gen, "operators", linux->seccomp.operators);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "apparmor", لڑی_لمبائی ("apparmor"));
  json_gen_map_open (json_gen);
  features_جوڑو_bool (json_gen, "enabled", linux->apparmor.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "selinux", لڑی_لمبائی ("selinux"));
  json_gen_map_open (json_gen);
  features_جوڑو_bool (json_gen, "enabled", linux->selinux.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "mountExtensions", لڑی_لمبائی ("mountExtensions"));
  json_gen_map_open (json_gen);
  json_gen_string (json_gen, "idmap", لڑی_لمبائی ("idmap"));
  json_gen_map_open (json_gen);
  features_جوڑو_bool (json_gen, "enabled", linux->mount_ext.idmap.enabled);
  json_gen_map_close (json_gen);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "intelRdt", لڑی_لمبائی ("intelRdt"));
  json_gen_map_open (json_gen);
  features_جوڑو_bool (json_gen, "enabled", linux->intel_rdt.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "netDevices", لڑی_لمبائی ("netDevices"));
  json_gen_map_open (json_gen);
  features_جوڑو_bool (json_gen, "enabled", linux->net_devices.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "memoryPolicy", لڑی_لمبائی ("memoryPolicy"));
  json_gen_map_open (json_gen);
  اگر (linux->حافظہ_policy.منوال)
    features_جوڑو_array (json_gen, "modes", linux->حافظہ_policy.منوال);
  اگر (linux->حافظہ_policy.flags)
    features_جوڑو_array (json_gen, "flags", linux->حافظہ_policy.flags);
  json_gen_map_close (json_gen);

  json_gen_map_close (json_gen);
}

ساکن خالی
features_جوڑو_annotations_info (json_gen_ctx *json_gen, ثابت ساخت annotations_info_s *annotation)
{
  json_gen_string (json_gen, "annotations", لڑی_لمبائی ("annotations"));
  json_gen_map_open (json_gen);

  اگر (! is_خالی_خانے_string (annotation->io_github_seccomp_libseccomp_نسخہ))
    features_جوڑو_string (json_gen, "io.github.seccomp.libseccomp.version", annotation->io_github_seccomp_libseccomp_نسخہ);

  features_جوڑو_bool_str (json_gen, "org.opencontainers.runc.checkpoint.enabled", annotation->تسلسل_oci_crun_checkpoint_enabled);
  features_جوڑو_bool_str (json_gen, "run.oci.crun.checkpoint.enabled", annotation->تسلسل_oci_crun_checkpoint_enabled);

  features_جوڑو_string (json_gen, "run.oci.crun.commit", annotation->تسلسل_oci_crun_commit);
  features_جوڑو_string (json_gen, "run.oci.crun.version", annotation->تسلسل_oci_crun_نسخہ);

  features_جوڑو_bool_str (json_gen, "run.oci.crun.wasm", annotation->تسلسل_oci_crun_wasm);

  json_gen_map_close (json_gen);
}

صحیح
libcrun_container_get_features_json (libcrun_context_t *context, حرف **برآمد, libcrun_error_t *err)
{
  cleanup_struct_features ساخت features_info_s *info = تہی;
  ثابت حرف *بفر = تہی;
  json_gen_ctx *json_gen;
  size_t بفر_len;
  صحیح ret;

  *برآمد = تہی;

  ret = libcrun_container_get_features (context, &info, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (! json_gen_init (&json_gen, تہی))
    واپس crun_make_error (err, 0, "cannot initialize JSON generator");

  json_gen_config (json_gen, json_gen_beautify, 1);

  json_gen_map_open (json_gen);

  features_جوڑو_string (json_gen, "ociVersionMin", info->oci_نسخہ_min);
  features_جوڑو_string (json_gen, "ociVersionMax", info->oci_نسخہ_max);
  features_جوڑو_array (json_gen, "hooks", info->hooks);
  features_جوڑو_array (json_gen, "mountOptions", info->mount_options);
  features_جوڑو_linux_info (json_gen, &info->linux);
  features_جوڑو_annotations_info (json_gen, &info->annotations);
  features_جوڑو_array (json_gen, "potentiallyUnsafeConfigAnnotations", info->potentially_unsafe_annotations);

  json_gen_map_close (json_gen);

  json_gen_get_buf (json_gen, &بفر, &بفر_len);
  *برآمد = xstrdup (بفر);
  json_gen_free (json_gen);

  واپس 0;
}

/* Read the container status and fail if the container is not running.  */
ساکن صحیح
read_حالت_require_running (libcrun_context_t *context, ثابت حرف *id,
                             libcrun_container_حالت_t *حالت_2, libcrun_error_t *err)
{
  صحیح ret;

  ret = libcrun_read_container_حالت (حالت_2, context->state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_is_container_running (حالت_2, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;
  اگر (ret == 0)
    واپس crun_make_error (err, 0, "the container `%s` is not running", id);

  واپس 0;
}

صحیح
libcrun_container_pause (libcrun_context_t *context, ثابت حرف *id, libcrun_error_t *err)
{
  صحیح ret;
  libcrun_container_حالت_t حالت_2 = {};

  ret = read_حالت_require_running (context, id, &حالت_2, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس libcrun_container_pause_linux (&حالت_2, err);
}

صحیح
libcrun_container_unpause (libcrun_context_t *context, ثابت حرف *id, libcrun_error_t *err)
{
  صحیح ret;
  libcrun_container_حالت_t حالت_2 = {};

  ret = read_حالت_require_running (context, id, &حالت_2, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس libcrun_container_unpause_linux (&حالت_2, err);
}

/* Translate the public options struct into the internal representation used by
   the linux/CRIU layer.  Strings are borrowed (not copied).  */
ساکن خالی
cr_options_from_عوامی (libcrun_checkpoint_restore_t *dst, ساخت libcrun_checkpoint_restore_options_s *src)
{
  memset (dst, 0, حجم (*dst));
  dst->image_راستہ = (حرف *) src->image_راستہ;
  dst->work_راستہ = (حرف *) src->work_راستہ;
  dst->والد_راستہ = (حرف *) src->والد_راستہ;
  dst->console_socket = src->console_socket;
  dst->lsm_profile = (حرف *) src->lsm_profile;
  dst->lsm_mount_context = (حرف *) src->lsm_mount_context;
  dst->leave_running = src->leave_running;
  dst->tcp_established = src->tcp_established;
  dst->tcp_close = src->tcp_close;
  dst->ext_unix_sk = src->ext_unix_sk;
  dst->shell_کام = src->shell_کام;
  dst->file_locks = src->file_locks;
  dst->pre_dump = src->pre_dump;
  dst->detach = src->detach;
  dst->manage_cgroups_منوال = src->manage_cgroups_منوال;
  dst->جال_تالا_طریقہ = src->جال_تالا_طریقہ;
}

صحیح
libcrun_container_checkpoint (libcrun_context_t *context, ثابت حرف *id,
                              ساخت libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  صحیح ret;
  ثابت حرف *state_جڑ = context->state_جڑ;
  libcrun_container_حالت_t حالت_2 = {};
  libcrun_checkpoint_restore_t cr;
  cleanup_container libcrun_container_t *container = تہی;

  cr_options_from_عوامی (&cr, cr_options);

  ret = read_حالت_require_running (context, id, &حالت_2, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = read_container_config_from_state (&container, state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  /* Needed to use the same paths (e.g. the shared empty directory used for
     masked paths) as the container runtime and restore do.  */
  container->context = context;

  ret = libcrun_container_checkpoint_linux (&حالت_2, container, &cr, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (! (cr_options->leave_running || cr_options->pre_dump))
    واپس container_delete_اندرونی (context, تہی, تہی, id, true, true, err);

  واپس 0;
}

/* The dummy process only keeps the cgroup populated until the container is restored there.
   Its cgroup is below the container cgroup, so once the container runs it can freeze or kill
   the dummy process: never depend on it after that point.  */
ساکن خالی
restore_proxy_process (صحیح proxy_pid_pipe)
{
  حرف c;

  /* Wait to be killed by the parent, or for the parent to go away.  */
  عارضی_FAILURE_RETRY (اقرأ (proxy_pid_pipe, &c, حجم (c)));
}

ساکن صحیح
restore_terminate_proxy_process (pid_t proxy_pid, ثابت حرف *proxy_cgroup, pid_t container_pid, صحیح cgroup_manager, libcrun_error_t *err)
{
  صحیح منوال;
  صحیح ret;

  اگر (cgroup_manager == CGROUP_MANAGER_SYSTEMD)
    {
      حرف ready_str[64];

      ret = snprintf (ready_str, حجم (ready_str), "MAINPID=%d", container_pid);
      اگر (UNLIKELY (ret >= (صحیح) حجم (ready_str)))
        واپس crun_make_error (err, 0, "internal error: static buffer too small");

      ret = send_sd_notify (ready_str, err);
      /* Do not fail on errors.  */
      اگر (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  منوال = libcrun_get_cgroup_منوال (err);
  اگر (UNLIKELY (منوال < 0))
    واپس منوال;

  /* SIGKILL terminates a process frozen with cgroup v2, while the cgroup v1 freezer does not
     deliver it until the cgroup is thawed, so do not wait for it there.  */
  kill (proxy_pid, SIGKILL);
  عارضی_FAILURE_RETRY (waitpid (proxy_pid, تہی, منوال == CGROUP_منوال_UNIFIED ? 0 : WNOHANG));

  واپس destroy_cgroup_راستہ (proxy_cgroup, منوال, err);
}

صحیح
libcrun_container_restore (libcrun_context_t *context, ثابت حرف *id arg_غیر_مستعمل,
                           ساخت libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  cleanup_cgroup_حالت ساخت libcrun_cgroup_حالت *cgroup_حالت = تہی;
  cleanup_container libcrun_container_t *container = تہی;
  cleanup_close صحیح proxy_pid_pipe0 = -1;
  cleanup_close صحیح proxy_pid_pipe1 = -1;
  runtime_spec_schema_config_schema *def;
  libcrun_container_حالت_t حالت_2 = {};
  ساخت libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  libcrun_checkpoint_restore_t cr;
  cleanup_pid pid_t proxy_pid = -1;
  cleanup_close صحیح seccomp_fd = -1;
  ثابت حرف *seccomp_bpf_ڈیٹا;
  صحیح proxy_pid_pipe[2];
  صحیح cgroup_manager;
  uid_t جڑ_uid = -1;
  gid_t جڑ_gid = -1;
  صحیح ret;

  cr_options_from_عوامی (&cr, cr_options);

  container = libcrun_container_بھرو_from_file ("config.json", err);
  اگر (container == تہی)
    واپس -1;

  container->context = context;
  def = container->container_def;

  ret = check_config_file (def, context, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_حالت_check_directories (context->state_جڑ, context->id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_copy_config_file (context->id, context->state_جڑ, container, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ fail;

  /* CRIU restores the seccomp filters of the checkpointed processes, but
     exec loads the filter from the seccomp.bpf file in the state directory,
     so it must be generated again for the restored container.  */
  seccomp_bpf_ڈیٹا = ڈھونڈو_annotation (container, "run.oci.seccomp_bpf_data");

  ret = تیاری_seccomp (container, seccomp_bpf_ڈیٹا, &seccomp_gen_ctx, &seccomp_fd, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ fail;

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_ڈیٹا, &seccomp_gen_ctx, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ fail;
  close_and_reset (&seccomp_fd);

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  اگر (context->systemd_cgroup)
    cgroup_manager = CGROUP_MANAGER_SYSTEMD;
  ورنہ اگر (context->force_no_cgroup)
    cgroup_manager = CGROUP_MANAGER_DISABLED;

  def = container->container_def;

  /* If we are root (either on the host or in a namespace),
   * then chown the cgroup to root in the container user namespace. */
  get_جڑ_اندر_the_userns (def, container->host_uid, container->host_gid, &جڑ_uid, &جڑ_gid);

  /* If the root in the container is different than the current root user, attempt to chown
     the std streams before entering the user namespace.  Otherwise we might lose access
     to the user (as it is not mapped in the user namespace) and cannot chown them.  */
  اگر (جڑ_uid > 0 || جڑ_gid > 0)
    {
      ret = maybe_chown_std_streams (جڑ_uid, جڑ_gid, err);
      اگر (UNLIKELY (ret < 0))
        جاؤ fail;
    }

  /* CRIU runs as a child process, so the restored processes would inherit the session keyring.  */
  ret = تیاری_container_keyring (container, context, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  {
    cleanup_close صحیح cgroup_dirfd = -1;
    bool needs_proxy_process;
    cleanup_cgroup_preenter ساخت libcrun_cgroup_دلائل cg = {
      .resources = def->linux ? def->linux->resources : تہی,
      .annotations = container->annotations,
      .cgroup_راستہ = def->linux ? def->linux->cgroups_راستہ : "",
      .manager = cgroup_manager,
      .جڑ_uid = جڑ_uid,
      .جڑ_gid = جڑ_gid,
      .id = context->id,
      .state_جڑ = context->state_جڑ,
      .container = container,
    };

    /* The CRIU restore code uses bundle, rootfs and cgroup_path of status.   The cgroup_path is set later.  */
    حالت_2.bundle = (حرف *) context->bundle;
    حالت_2.rootfs = def->جڑ->راستہ;

    ret = libcrun_cgroup_preenter (&cg, &cgroup_dirfd, err);
    اگر (UNLIKELY (ret < 0))
      جاؤ fail;

    needs_proxy_process = cgroup_dirfd < 0;

    /* If the target cgroup was already created (with cgroupfs), we can restore the container directly there.  */
    اگر (! needs_proxy_process)
      {
        cleanup_free حرف *cgroup_راستہ = تہی;

        ret = get_cgroup_dirfd_راستہ (cgroup_dirfd, &cgroup_راستہ, err);
        اگر (UNLIKELY (ret < 0))
          جاؤ fail;

        /* Complete the configuration for the cgroup before the container is
           restored there: CRIU resumes the restored processes before it
           returns, so the device filter and the limits must already be in
           place.  There is no process to move, as CRIU is run in the cgroup,
           and the controllers were enabled by precreate_cgroup.  */
        cg.joined = true;

        ret = libcrun_cgroup_enter (&cg, &cgroup_حالت, err);
        اگر (UNLIKELY (ret < 0))
          جاؤ fail;

        /* Restore the container directly in the desired cgroup.  */
        حالت_2.cgroup_راستہ = cgroup_راستہ;

        ret = libcrun_container_restore_linux (&حالت_2, container, &cr, err);
        اگر (UNLIKELY (ret < 0))
          جاؤ fail;

        /* Now that the container is restored, keep any signal for it, so that
           it is forwarded to the init by wait_for_process, rather than acted
           upon by crun.  This cannot be done earlier, as CRIU, which is run as
           a child process, would inherit the blocked signals.  */
        ret = بلاک_اشارات (err);
        اگر (UNLIKELY (ret < 0))
          جاؤ fail;

        /* Use the container first process PID to finalize the cgroup.  */
        cg.pid = حالت_2.pid;
      }
    ورنہ
      {
        /* When using systemd, we need a process first that must exist before a cgroup is created.
           Create a dummy process that sits and waits until the container is restored.  */
        ret = pipe2 (proxy_pid_pipe, O_CLOEXEC);
        اگر (UNLIKELY (ret < 0))
          {
            ret = crun_make_error (err, errno, "pipe");
            جاؤ fail;
          }

        /* Only used for auto cleanup.  */
        proxy_pid_pipe0 = proxy_pid_pipe[0];
        proxy_pid_pipe1 = proxy_pid_pipe[1];

        proxy_pid = fork ();
        اگر (UNLIKELY (proxy_pid < 0))
          {
            ret = crun_make_error (err, errno, "fork");
            جاؤ fail;
          }

        اگر (proxy_pid == 0)
          {
            close_and_reset (&proxy_pid_pipe1);

            restore_proxy_process (proxy_pid_pipe0);
            محفوظ_exit (EXIT_SUCCESS);
          }

        close_and_reset (&proxy_pid_pipe0);

        /* Use the dummy process PID as the PID to setup the cgroup.  */
        cg.pid = proxy_pid;

        /* Complete the configuration for the cgroup.  It contains only the dummy process.  */
        ret = libcrun_cgroup_enter (&cg, &cgroup_حالت, err);
        اگر (UNLIKELY (ret < 0))
          جاؤ fail;
      }

    ret = libcrun_cgroup_enter_finalize (&cg, cgroup_حالت, err);
    اگر (UNLIKELY (ret < 0))
      جاؤ fail;

    /* When using a dummy process, the container is restored here once the cgroup is configured.  */
    اگر (needs_proxy_process)
      {
        cleanup_free حرف *ہدف_cgroup = تہی;
        cleanup_free حرف *proxy_cgroup = تہی;

        ret = libcrun_get_cgroup_process (proxy_pid, &ہدف_cgroup, false, err);
        اگر (UNLIKELY (ret < 0))
          جاؤ fail;

        /* Move the dummy process to a sub-cgroup.  */
        ret = append_paths (&proxy_cgroup, err, ہدف_cgroup, ".proxy-crun", تہی);
        اگر (UNLIKELY (ret < 0))
          جاؤ fail;

        ret = libcrun_move_process_مقصد_cgroup (proxy_pid, 0, proxy_cgroup, true, err);
        اگر (UNLIKELY (ret < 0))
          جاؤ fail;

        /* Restore the container in the same cgroup where the dummy process was.  */
        حالت_2.cgroup_راستہ = ہدف_cgroup;

        ret = libcrun_container_restore_linux (&حالت_2, container, &cr, err);
        اگر (UNLIKELY (ret < 0))
          جاؤ fail;

        /* See the comment for the other libcrun_container_restore_linux call.  */
        ret = بلاک_اشارات (err);
        اگر (UNLIKELY (ret < 0))
          جاؤ fail;

        /* The container is running, so do not fail because of the dummy process.  */
        ret = restore_terminate_proxy_process (proxy_pid, proxy_cgroup, حالت_2.pid, cgroup_manager, err);
        اگر (UNLIKELY (ret < 0))
          crun_error_release (err);
        proxy_pid = -1;

        close_and_reset (&proxy_pid_pipe1);
      }
  }

  context->detach = cr_options->detach;
  ret = write_container_حالت (container, context, حالت_2.pid, cgroup_حالت, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ fail;

  {
    /* Wait the same way run and exec do: apart from waiting for the container
       init, this writes the pid file, forwards any signal to the init, and
       reaps every child, not only the init.  */
    ساخت wait_for_process_دلائل دلائل = {
      .pid = حالت_2.pid,
      .context = context,
      .terminal_fd = -1,
      .notify_socket = -1,
      .container_ready_fd = تہی,
      .seccomp_notify_fd = -1,
      .seccomp_notify_plugins = تہی,
    };

    واپس wait_for_process (&دلائل, err);
  }

fail:
  /* Once restored, the container is running.  Without a status file nothing
     can reach it anymore, so do not leave it behind.  */
  اگر (حالت_2.pid > 0)
    {
      kill (حالت_2.pid, SIGKILL);
      waitpid_ignore_stopped (حالت_2.pid, تہی, 0);
    }
  اگر (cgroup_حالت)
    {
      libcrun_error_t tmp_err = تہی;
      libcrun_cgroup_destroy (cgroup_حالت, &tmp_err);
      crun_error_release (&tmp_err);
    }
  {
    libcrun_error_t tmp_err = تہی;

    اگر (libcrun_container_delete_حالت (context->state_جڑ, context->id, &tmp_err) < 0)
      crun_error_release (&tmp_err);
  }
  واپس ret;
}

صحیح
libcrun_container_read_pids (libcrun_context_t *context, ثابت حرف *id, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  cleanup_cgroup_حالت ساخت libcrun_cgroup_حالت *cgroup_حالت = تہی;
  cleanup_container_حالت libcrun_container_حالت_t حالت_2 = {};
  صحیح ret;

  ret = libcrun_read_container_حالت (&حالت_2, context->state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (حالت_2.cgroup_راستہ == تہی || حالت_2.cgroup_راستہ[0] == '\0')
    واپس crun_make_error (err, 0, "the container is not using cgroups");

  cgroup_حالت = libcrun_cgroup_make_حالت (&حالت_2);

  واپس libcrun_cgroup_read_pids (cgroup_حالت, recurse, pids, err);
}

صحیح
libcrun_container_list_json (libcrun_context_t *context, حرف **برآمد, libcrun_error_t *err)
{
  libcrun_container_list_t *list = تہی, *it;
  ثابت حرف *content = تہی;
  json_gen_ctx *gen = تہی;
  size_t len;
  صحیح ret;

  *برآمد = تہی;

  ret = libcrun_get_containers_list (&list, context->state_جڑ, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (! json_gen_init (&gen, تہی))
    {
      ret = crun_make_error (err, 0, "cannot allocate json generator");
      جاؤ باہر_نکلو;
    }

  json_gen_config (gen, json_gen_beautify, 1);
  json_gen_array_open (gen);

  برائے (it = list; it; it = it->اگلا)
    {
      libcrun_container_حالت_t حالت_2;
      صحیح running = 0;
      صحیح pid;
      ثابت حرف *container_حالت = تہی;

      ret = libcrun_read_container_حالت (&حالت_2, context->state_جڑ, it->name, err);
      اگر (UNLIKELY (ret < 0))
        جاؤ باہر_نکلو;

      pid = حالت_2.pid;

      ret = libcrun_get_container_state_string (it->name, &حالت_2, context->state_جڑ, &container_حالت,
                                                &running, err);
      اگر (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          جاری;
        }

      اگر (! running)
        pid = 0;

      json_gen_map_open (gen);
      json_gen_string (gen, "id", لڑی_لمبائی ("id"));
      json_gen_string (gen, it->name, لڑی_لمبائی (it->name));
      json_gen_string (gen, "pid", لڑی_لمبائی ("pid"));
      map_int (gen, pid);
      json_gen_string (gen, "status", لڑی_لمبائی ("status"));
      json_gen_string (gen, container_حالت, لڑی_لمبائی (container_حالت));
      json_gen_string (gen, "bundle", لڑی_لمبائی ("bundle"));
      json_gen_string (gen, حالت_2.bundle, لڑی_لمبائی (حالت_2.bundle));
      json_gen_string (gen, "created", لڑی_لمبائی ("created"));
      json_gen_string (gen, حالت_2.created, لڑی_لمبائی (حالت_2.created));
      json_gen_string (gen, "owner", لڑی_لمبائی ("owner"));
      json_gen_string (gen, حالت_2.owner, محفوظ_strlen (حالت_2.owner));
      json_gen_map_close (gen);

      libcrun_free_container_حالت (&حالت_2);
    }

  json_gen_array_close (gen);
  اگر (json_gen_get_buf (gen, &content, &len) != json_gen_حالت_ok)
    {
      ret = libcrun_make_error (err, 0, "cannot generate json list");
      جاؤ باہر_نکلو;
    }

  *برآمد = xstrdup (content);
  ret = 0;

باہر_نکلو:
  اگر (list)
    libcrun_free_containers_list (list);
  اگر (gen)
    json_gen_free (gen);

  واپس ret;
}

صحیح
libcrun_write_json_containers_list (libcrun_context_t *context, FILE *برآمد, libcrun_error_t *err)
{
  cleanup_free حرف *content = تہی;
  size_t len;
  صحیح ret;

  ret = libcrun_container_list_json (context, &content, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  len = لڑی_لمبائی (content);
  اگر (len && fwrite (content, 1, len, برآمد) != len)
    واپس crun_make_error (err, errno, "error writing to file");

  واپس 0;
}

صحیح
libcrun_container_update_intel_rdt (libcrun_context_t *context, ثابت حرف *id, ساخت libcrun_intel_rdt_update *update, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = تہی;
  صحیح ret;

  ret = read_container_config_from_state (&container, context->state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس libcrun_update_intel_rdt (id, container, update->l3_cache_schema, update->mem_bw_schema, update->schemata, err);
}

ساکن صحیح
libcrun_container_جوڑو_or_remove_mounts_from_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *file, bool جوڑو, libcrun_error_t *err)
{
  cleanup_custom_handler_instance ساخت custom_handler_instance_s *custom_handler = تہی;
  cleanup_container libcrun_container_t *container = تہی;
  cleanup_free runtime_spec_schema_defs_mount **mounts = تہی;
  ثابت حرف *state_جڑ = context->state_جڑ;
  cleanup_free parser_error parser_err = تہی;
  ساخت parser_context pctx = { 0, stderr };
  libcrun_container_حالت_t حالت_2 = {};
  cleanup_free حرف *content = تہی;
  json_object *doc = تہی;
  size_t n_mounts = 0, len, i;
  صحیح ret = 1;

  ret = read_all_file (file, &content, &len, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_read_container_حالت (&حالت_2, state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = read_container_config_from_state (&container, state_جڑ, id, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = parse_json_file (&doc, content, &pctx, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (! json_object_is_type (doc, json_type_array))
    {
      ret = crun_make_error (err, 0, "mounts must be an array");
      جاؤ cleanup;
    }
  ورنہ
    {
      n_mounts = json_object_array_لمبائی (doc);

      mounts = xmalloc0 ((n_mounts + 1) * حجم (*mounts));
      برائے (i = 0; i < n_mounts; i++)
        {
          mounts[i] = make_runtime_spec_schema_defs_mount (json_object_array_get_idx (doc, i), &pctx, &parser_err);
          اگر (mounts[i] == تہی)
            {
              ret = crun_make_error (err, 0, "cannot parse mount: %s", parser_err);
              جاؤ cleanup;
            }
        }
    }

  اگر (جوڑو)
    ret = libcrun_make_runtime_mounts (container, &حالت_2, mounts, n_mounts, err);
  ورنہ
    ret = libcrun_destroy_runtime_mounts (container, &حالت_2, mounts, n_mounts, err);

cleanup:
  اگر (doc)
    json_object_put (doc);
  اگر (mounts)
    برائے (i = 0; i < n_mounts; i++)
      free_runtime_spec_schema_defs_mount (mounts[i]);
  واپس ret;
}

صحیح
libcrun_container_جوڑو_mounts_from_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *file, libcrun_error_t *err)
{
  واپس libcrun_container_جوڑو_or_remove_mounts_from_file (context, id, file, true, err);
}

صحیح
libcrun_container_remove_mounts_from_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *file, libcrun_error_t *err)
{
  واپس libcrun_container_جوڑو_or_remove_mounts_from_file (context, id, file, false, err);
}
