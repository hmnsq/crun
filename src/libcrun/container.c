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
#تعریف_کن GNU_منبع

#ضمن <config.h>
#ضمن <ocispec/runtime_spec_schema_config_schema.h>
#ضمن <stdbool.h>
#ضمن "container.h"
#ضمن "utils.h"
#ضمن "json_gen_utils.h"
#ضمن "seccomp.h"
#ضمن "mempolicy.h"
#اگر_تعریف_شده HAVE_SECCOMP
#  include <seccomp.h>
#پایان_اگر
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
#اگر_تعریف_شده HAVE_CAP
#  include <sys/capability.h>
#پایان_اگر
#ضمن <sys/ioctl.h>
#ضمن <termios.h>
#ضمن <grp.h>
#ضمن <git-version.h>

#اگر_تعریف_شده HAVE_SYSTEMD
#  include <systemd/sd-daemon.h>
#پایان_اگر

#ضمن <json-c/json.h>
#ضمن <ocispec/json_common.h>

تعداد
{
  SYNC_SOCKET_SYNC_پیام,
  SYNC_SOCKET_ERROR_پیام,
  SYNC_SOCKET_WARNING_پیام,
  SYNC_SOCKET_DEBUG_پیام,
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

  صحيح hooks_بیرون_fd;
  صحيح hooks_err_fd;

  /* Absolute bundle path, to be reported to hooks run in the container.  */
  ثابت حرف *bundle;

  بنية custom_handler_instance_s *custom_handler;
};

بنية sync_socket_پیام_s
{
  صحيح type;
  صحيح error_مقدار;
  حرف پیام_2[512];
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

#تعریف_کن SYNC_SOCKET_پیام_LEN(x, l) (offsetof (بنية sync_socket_پیام_s, پیام_2) + l)

ساكن صحيح
sync_socket_write_پیام (صحيح fd, صحيح verbosity, صحيح err_مقدار, ثابت حرف *log_پیام)
{
  صحيح ret;
  size_t err_len;
  بنية sync_socket_پیام_s پیام;
  بدل (verbosity)
    {
    حالة LIBCRUN_VERBOSITY_DEBUG:
      پیام.type = SYNC_SOCKET_DEBUG_پیام;
      اكسر;
    حالة LIBCRUN_VERBOSITY_WARNING:
      پیام.type = SYNC_SOCKET_WARNING_پیام;
      اكسر;
    حالة LIBCRUN_VERBOSITY_ERROR:
      پیام.type = SYNC_SOCKET_ERROR_پیام;
      اكسر;
    }
  پیام.error_مقدار = err_مقدار;

  إذا (fd < 0)
    أرجع 0;

  err_len = طول_رشته (log_پیام);
  إذا (err_len >= حجم (پیام.پیام_2))
    err_len = حجم (پیام.پیام_2) - 1;

  کپی_حافظه (پیام.پیام_2, log_پیام, err_len);
  پیام.پیام_2[err_len] = '\0';

  ret = موقت_FAILURE_RETRY (اكتب (fd, &پیام, SYNC_SOCKET_پیام_LEN (پیام, err_len + 1)));
  إذا (UNLIKELY (ret < 0))
    أرجع -1;

  أرجع 0;
}

ساكن صحيح
sync_socket_write_error (صحيح fd, libcrun_error_t *بیرون_err)
{
  إذا (fd < 0)
    أرجع 0;
  أرجع sync_socket_write_پیام (fd, false, (*بیرون_err)->وضعیت, (*بیرون_err)->پیام);
}

ساكن فراغ
log_write_مقصد_sync_socket (صحيح errno_, ثابت حرف *پیام, صحيح verbosity, فراغ *arg)
{
  بنية container_entrypoint_s *entrypoint_آرگومانها = arg;
  صحيح fd = entrypoint_آرگومانها->sync_socket;

  إذا (fd < 0)
    أرجع;

  إذا (sync_socket_write_پیام (fd, verbosity, errno_, پیام) < 0)
    log_write_مقصد_stderr (errno_, پیام, verbosity, arg);
}

ساكن bool
is_حافظه_حد_too_کم (runtime_spec_schema_config_schema *def)
{
  ثابت دراز حافظه_حد_too_کم = 1024 * 1024;

  إذا (def->linux == عدم || def->linux->resources == عدم)
    أرجع false;

  إذا (def->linux->resources->حافظه
      && def->linux->resources->حافظه->حد_present
      && def->linux->resources->حافظه->حد < حافظه_حد_too_کم)
    أرجع true;

  إذا (def->linux->resources->unified)
    {
      size_t i;

      لكل (i = 0; i < def->linux->resources->unified->len; i++)
        إذا (strcmp (def->linux->resources->unified->کلیدها[i], "memory.max") == 0)
          {
            دراز حد;

            errno = 0;
            حد = strtol (def->linux->resources->unified->مقادیر[i], عدم, 10);
            إذا (errno != 0)
              أرجع false;
            إذا (حد < حافظه_حد_too_کم)
              أرجع true;
          }
    }

  أرجع false;
}

ساكن صحيح
sync_socket_wait_sync (libcrun_context_t *context, صحيح fd, bool flush, libcrun_error_t *err)
{
  بنية sync_socket_پیام_s پیام;

  إذا (fd < 0)
    أرجع 0;

  طالما (true)
    {
      صحيح ret;

      ret = موقت_FAILURE_RETRY (اقرأ (fd, &پیام, حجم (پیام)));
      إذا (UNLIKELY (ret < 0))
        {
          إذا (flush)
            أرجع 0;
          أرجع crun_make_error (err, errno, "read from sync socket");
        }

      إذا (ret == 0)
        {
          إذا (flush)
            أرجع 0;

          أرجع crun_make_error (err, 0, "read from the init process");
        }

      إذا (! flush && پیام.type == SYNC_SOCKET_SYNC_پیام)
        أرجع 0;
      وإلا إذا (پیام.type == SYNC_SOCKET_DEBUG_پیام)
        {
          إذا (context)
            context->output_handler (پیام.error_مقدار, پیام.پیام_2, LIBCRUN_VERBOSITY_DEBUG, context->output_handler_arg);
          استمر;
        }
      وإلا إذا (پیام.type == SYNC_SOCKET_WARNING_پیام)
        {
          إذا (context)
            context->output_handler (پیام.error_مقدار, پیام.پیام_2, LIBCRUN_VERBOSITY_WARNING, context->output_handler_arg);
          استمر;
        }
      وإلا إذا (پیام.type == SYNC_SOCKET_ERROR_پیام)
        أرجع crun_make_error (err, پیام.error_مقدار, "%s", پیام.پیام_2);
    }
}

ساكن صحيح
sync_socket_send_sync (صحيح fd, bool flush_errors, libcrun_error_t *err)
{
  صحيح ret;
  بنية sync_socket_پیام_s پیام = {
    0,
  };
  پیام.type = SYNC_SOCKET_SYNC_پیام;

  إذا (fd < 0)
    أرجع 0;

  ret = موقت_FAILURE_RETRY (اكتب (fd, &پیام, SYNC_SOCKET_پیام_LEN (پیام, 0)));
  إذا (UNLIKELY (ret < 0))
    {
      إذا (flush_errors)
        {
          صحيح saved_errno = errno;
          ret = موقت_FAILURE_RETRY (اقرأ (fd, &پیام, حجم (پیام)));
          إذا (ret >= 0 && پیام.type == SYNC_SOCKET_ERROR_پیام)
            أرجع crun_make_error (err, پیام.error_مقدار, "%s", پیام.پیام_2);
          errno = saved_errno;
        }
      أرجع crun_make_error (err, errno, "write to sync socket");
    }

  أرجع 0;
}

ساكن bool
has_cap_sys_ptrace ()
{
#اگر_تعریف_شده HAVE_CAP
  بنية __user_cap_header_struct hdr = { LINUX_CAPABILITY_نسخه_3, 0 };
  بنية __user_cap_data_struct داده[2] = { { 0 } };

  إذا (UNLIKELY (capget (&hdr, داده) < 0))
    أرجع true;

  أرجع (داده[0].effective & (1U << CAP_SYS_PTRACE)) != 0;
#وإلا
  أرجع geteuid () == 0;
#پایان_اگر
}

ساكن libcrun_container_t *
make_container (runtime_spec_schema_config_schema *container_def, ثابت حرف *مسیر, ثابت حرف *config)
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
  إذا (container_def->linux)
    {
      size_t i;

      لكل (i = 0; i < container_def->linux->namespaces_len; i++)
        إذا (container_def->linux->namespaces[i]->مسیر && container_def->linux->namespaces[i]->مسیر[0] == '\0')
          {
            حرر (container_def->linux->namespaces[i]->مسیر);
            container_def->linux->namespaces[i]->مسیر = عدم;
          }
    }

  إذا (مسیر)
    container->config_file = xstrdup (مسیر);
  إذا (config)
    container->config_file_content = xstrdup (config);

  أرجع container;
}

libcrun_container_t *
libcrun_container_بارگیری_from_حافظه (ثابت حرف *json, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free حرف *oci_error = عدم;
  container_def = runtime_spec_schema_config_schema_parse_داده (json, عدم, &oci_error);
  إذا (container_def == عدم)
    {
      crun_make_error (err, 0, "load: `%s`", oci_error);
      أرجع عدم;
    }
  أرجع make_container (container_def, عدم, json);
}

libcrun_container_t *
libcrun_container_بارگیری_from_file (ثابت حرف *مسیر, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free حرف *oci_error = عدم;
  cleanup_free حرف *content = عدم;
  size_t len;
  صحيح ret;

  libcrun_debug ("Loading container from config file: `%s`", مسیر);
  ret = read_all_file (مسیر, &content, &len, err);
  إذا (UNLIKELY (ret < 0))
    أرجع عدم;

  container_def = runtime_spec_schema_config_schema_parse_داده (content, عدم, &oci_error);
  إذا (container_def == عدم)
    {
      crun_make_error (err, 0, "load `%s`: %s", مسیر, oci_error);
      أرجع عدم;
    }
  أرجع make_container (container_def, عدم, content);
}

ثابت حرف *
libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err)
{
  بنية parser_context ctx = { 0, stderr };
  parser_error gen_err = عدم;

  إذا (container == عدم)
    {
      crun_make_error (err, EINVAL, "no container specified");
      أرجع عدم;
    }

  إذا (container->config_file_content == عدم && container->container_def)
    {
      container->config_file_content = runtime_spec_schema_config_schema_generate_json (container->container_def, &ctx, &gen_err);
      إذا (container->config_file_content == عدم)
        {
          crun_make_error (err, 0, "cannot generate the configuration JSON: %s", gen_err ?: "unknown error");
          حرر (gen_err);
          أرجع عدم;
        }
      حرر (gen_err);
    }

  إذا (container->config_file_content == عدم)
    {
      crun_make_error (err, 0, "the container has no configuration");
      أرجع عدم;
    }

  أرجع container->config_file_content;
}

ثابت حرف *
libcrun_container_get_annotation (libcrun_container_t *container, ثابت حرف *کلید)
{
  إذا (container == عدم)
    أرجع عدم;
  أرجع پیدا_کن_string_map_مقدار (container->annotations, کلید);
}

size_t
libcrun_container_get_annotations_len (libcrun_container_t *container)
{
  إذا (container == عدم || container->annotations == عدم)
    أرجع 0;
  أرجع string_map_اندازه (container->annotations);
}

صحيح
libcrun_container_get_annotation_at (libcrun_container_t *container, size_t شاخص, ثابت حرف **کلید,
                                     ثابت حرف **مقدار)
{
  إذا (container == عدم)
    {
      errno = ERANGE;
      أرجع -1;
    }
  أرجع string_map_get_at (container->annotations, شاخص, کلید, مقدار);
}

uid_t
libcrun_container_get_uid (libcrun_container_t *container)
{
  إذا (container == عدم)
    أرجع (uid_t) -1;
  أرجع container->host_uid;
}

gid_t
libcrun_container_get_gid (libcrun_container_t *container)
{
  إذا (container == عدم)
    أرجع (gid_t) -1;
  أرجع container->host_gid;
}

فراغ
libcrun_container_free (libcrun_container_t *ctr)
{
  إذا (ctr == عدم)
    أرجع;

  إذا (ctr->cleanup_خصوصی_داده)
    ctr->cleanup_خصوصی_داده (ctr->خصوصی_داده);

  إذا (ctr->container_def)
    free_runtime_spec_schema_config_schema (ctr->container_def);

  free_string_map (ctr->annotations);

  إذا (ctr->proc_fd >= 0)
    close (ctr->proc_fd);

  حرر (ctr->config_file_content);
  حرر (ctr->config_file);
  حرر (ctr);
}

libcrun_context_t *
libcrun_context_new (ثابت حرف *id, ثابت حرف *state_ریشه, libcrun_error_t *err)
{
  libcrun_context_t *ctx = xmalloc0 (حجم (*ctx));

  /* Go through the setters so that the owned_* copies and the borrowed
     pointers stay in sync and are released by libcrun_context_free.  */
  libcrun_context_مجموعه_id (ctx, id);
  libcrun_context_مجموعه_state_ریشه (ctx, state_ریشه);
  libcrun_context_مجموعه_bundle (ctx, ".");
  ctx->fifo_exec_wait_fd = -1;

  إذا (libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, id, عدم, err) < 0)
    {
      libcrun_context_free (ctx);
      أرجع عدم;
    }

  ctx->handler_manager = libcrun_handler_manager_create (err);
  إذا (ctx->handler_manager == عدم)
    {
      libcrun_context_free (ctx);
      أرجع عدم;
    }

#اگر_تعریف_شده CRUN_LIBDIR
  إذا (access (CRUN_LIBDIR "/handlers", F_OK) == 0)
    {
      إذا (libcrun_handler_manager_بارگیری_directory (ctx->handler_manager, CRUN_LIBDIR "/handlers", err) < 0)
        {
          libcrun_context_free (ctx);
          أرجع عدم;
        }
    }
#پایان_اگر

  أرجع ctx;
}

فراغ
libcrun_context_free (libcrun_context_t *ctx)
{
  إذا (ctx == عدم)
    أرجع;

  حرر (ctx->owned_state_ریشه);
  حرر (ctx->owned_id);
  حرر (ctx->owned_bundle);
  حرر (ctx->owned_console_socket);
  حرر (ctx->owned_pid_file);
  حرر (ctx->owned_notify_socket);
  حرر (ctx->owned_handler);

  إذا (ctx->handler_manager)
    handler_manager_free (ctx->handler_manager);

  حرر (ctx);
}

#تعریف_کن LIBCRUN_CONTEXT_مجموعه_STRING(فیلد)                  \
  افعل                                                       \
    {                                                      \
      حرر (ctx->owned_##field);                           \
      ctx->owned_##field = مقدار ? xstrdup (مقدار) : عدم; \
      ctx->فیلد = ctx->owned_##field;                     \
  } طالما (0)

فراغ
libcrun_context_مجموعه_id (libcrun_context_t *ctx, ثابت حرف *مقدار)
{
  LIBCRUN_CONTEXT_مجموعه_STRING (id);
}

فراغ
libcrun_context_مجموعه_state_ریشه (libcrun_context_t *ctx, ثابت حرف *مقدار)
{
  LIBCRUN_CONTEXT_مجموعه_STRING (state_ریشه);
}

فراغ
libcrun_context_مجموعه_bundle (libcrun_context_t *ctx, ثابت حرف *مقدار)
{
  LIBCRUN_CONTEXT_مجموعه_STRING (bundle);
}

فراغ
libcrun_context_مجموعه_console_socket (libcrun_context_t *ctx, ثابت حرف *مقدار)
{
  LIBCRUN_CONTEXT_مجموعه_STRING (console_socket);
}

فراغ
libcrun_context_مجموعه_pid_file (libcrun_context_t *ctx, ثابت حرف *مقدار)
{
  LIBCRUN_CONTEXT_مجموعه_STRING (pid_file);
}

فراغ
libcrun_context_مجموعه_notify_socket (libcrun_context_t *ctx, ثابت حرف *مقدار)
{
  LIBCRUN_CONTEXT_مجموعه_STRING (notify_socket);
}

فراغ
libcrun_context_مجموعه_handler (libcrun_context_t *ctx, ثابت حرف *مقدار)
{
  LIBCRUN_CONTEXT_مجموعه_STRING (handler);
}

#لغو_تعریف LIBCRUN_CONTEXT_مجموعه_STRING

ثابت حرف *
libcrun_context_get_pid_file (libcrun_context_t *ctx)
{
  أرجع ctx->pid_file;
}

ثابت حرف *
libcrun_context_get_state_ریشه (libcrun_context_t *ctx)
{
  إذا (ctx == عدم)
    أرجع عدم;
  أرجع ctx->state_ریشه;
}

فراغ
libcrun_context_مجموعه_preserve_fds (libcrun_context_t *ctx, صحيح preserve_fds)
{
  ctx->preserve_fds = preserve_fds;
}

صحيح
libcrun_context_get_preserve_fds (libcrun_context_t *ctx)
{
  أرجع ctx->preserve_fds;
}

فراغ
libcrun_context_مجموعه_listen_fds (libcrun_context_t *ctx, صحيح listen_fds)
{
  ctx->listen_fds = listen_fds;
}

فراغ
libcrun_context_مجموعه_آرگومانها (libcrun_context_t *ctx, صحيح argc, حرف **argv)
{
  ctx->argc = argc;
  ctx->argv = argv;
}

فراغ
libcrun_context_مجموعه_systemd_cgroup (libcrun_context_t *ctx, bool مقدار)
{
  ctx->systemd_cgroup = مقدار;
}

فراغ
libcrun_context_مجموعه_detach (libcrun_context_t *ctx, bool مقدار)
{
  ctx->detach = مقدار;
}

فراغ
libcrun_context_مجموعه_no_new_keyring (libcrun_context_t *ctx, bool مقدار)
{
  ctx->no_new_keyring = مقدار;
}

فراغ
libcrun_context_مجموعه_no_محور (libcrun_context_t *ctx, bool مقدار)
{
  ctx->no_محور = مقدار;
}

فراغ
libcrun_context_مجموعه_force_no_cgroup (libcrun_context_t *ctx, bool مقدار)
{
  ctx->force_no_cgroup = مقدار;
}

فراغ
libcrun_context_مجموعه_output_handler (libcrun_context_t *ctx, crun_output_handler handler, فراغ *arg)
{
  ctx->output_handler = handler;
  ctx->output_handler_arg = arg;
}

صحيح
libcrun_context_init_logging (libcrun_context_t *ctx, ثابت حرف *لگاریتم, libcrun_error_t *err)
{
  أرجع libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, ctx->id, لگاریتم, err);
}

ساكن صحيح
بلوک_سیگنالها (libcrun_error_t *err)
{
  صحيح ret;
  sigset_t ماسک;
  sigfillset (&ماسک);
  ret = sigprocmask (SIG_بلوک, &ماسک, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "sigprocmask");
  أرجع 0;
}

ساكن صحيح
unblock_سیگنالها (libcrun_error_t *err)
{
  صحيح i;
  صحيح ret;
  sigset_t ماسک;
  بنية sigaction act = {};

  sigfillset (&ماسک);
  ret = sigprocmask (SIG_UNBLOCK, &ماسک, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "sigprocmask");

  act.sa_handler = SIG_DFL;
  لكل (i = 0; i < NSIG; i++)
    {
      ret = sigaction (i, &act, عدم);
      إذا (ret < 0 && errno != EINVAL)
        أرجع crun_make_error (err, errno, "sigaction");
    }

  أرجع 0;
}

/* must be used on the host before pivot_root(2).  */
ساكن صحيح
initialize_security (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, libcrun_error_t *err)
{
  صحيح ret;

  ret = libcrun_init_caps (container, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (UNLIKELY (proc == عدم))
    أرجع 0;

  إذا (proc->apparmor_profile)
    {
      ret = libcrun_initialize_apparmor (err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = libcrun_initialize_selinux (container, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع 0;
}

/* Emit the "annotations" map.  Returns a json_gen_status; callers can wrap
   the call with GEN_OR_FAIL.  */
ساكن صحيح
gen_annotations (json_gen_ctx *gen, json_map_string_string *annotations)
{
  size_t i;
  صحيح r;

  GEN_کلید (gen, "annotations");
  GEN_OR_FAIL (json_gen_map_open (gen));

  لكل (i = 0; i < annotations->len; i++)
    {
      /* An annotation whose value is null in the document is parsed into a
         NULL, which cannot be emitted as a JSON string; leave it out.  */
      إذا (annotations->کلیدها[i] == عدم || annotations->مقادیر[i] == عدم)
        استمر;

      GEN_STR (gen, annotations->کلیدها[i]);
      GEN_STR (gen, annotations->مقادیر[i]);
    }

  أرجع json_gen_map_close (gen);

gen_error:
  أرجع r;
}

/* Return the bundle path to report in the container state.  crun always
   chdir()s into the bundle, so fall back to getcwd() when the caller has no
   absolute bundle path, as can happen when libcrun is used as a library.
   *ALLOCATED must be freed by the caller.  */
ساكن ثابت حرف *
get_bundle_مسیر (ثابت حرف *bundle, حرف **allocated)
{
  إذا (bundle != عدم && bundle[0] == '/')
    أرجع bundle;

  *allocated = getcwd (عدم, 0);
  إذا (*allocated == عدم)
    OOM ();
  أرجع *allocated;
}

ساكن صحيح
do_hooks (runtime_spec_schema_config_schema *def, pid_t pid, ثابت حرف *id, bool keep_going, ثابت حرف *bundle,
          ثابت حرف *cwd, ثابت حرف *وضعیت, ثابت حرف *name, hook **hooks, size_t hooks_len, صحيح بیرون_fd,
          صحيح err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  size_t i, stdin_len;
  صحيح r, ret;
  حرف *stdin = عدم;
  cleanup_free حرف *bundle_allocated = عدم;
  ثابت حرف *rootfs = def->ریشه ? def->ریشه->مسیر : "";
  cleanup_json_gen json_gen_ctx *gen = عدم;

  bundle = get_bundle_مسیر (bundle, &bundle_allocated);

  إذا (! json_gen_init (&gen, عدم))
    أرجع crun_make_error (err, 0, "json_gen_init failed");

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
  GEN_STR (gen, وضعیت);

  إذا (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, (ثابت حرف **) &stdin, &stdin_len));

  ret = 0;

  bool error_created = false;
  لكل (i = 0; i < hooks_len; i++)
    {
      حرف **env = environ;

      إذا (hooks[i]->env)
        env = hooks[i]->env;

      /* Release the error from the previous iteration, if any.  */
      إذا (error_created)
        {
          crun_error_release (err);
          error_created = false;
        }

      ret = پیاپی_process_with_stdin_timeout_envp (hooks[i]->مسیر, hooks[i]->آرگومانها, cwd, hooks[i]->timeout, env,
                                                 stdin, stdin_len, بیرون_fd, err_fd, can_ignore_chdir_errors, err);
      إذا (UNLIKELY (ret < 0))
        error_created = true;

      إذا (UNLIKELY (ret != 0))
        {
          إذا (keep_going)
            libcrun_warning ("error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->مسیر, ret);
          وإلا
            {
              libcrun_error (0, "error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->مسیر, ret);
              اكسر;
            }
        }
    }

  أرجع ret;

gen_error:
  أرجع json_gen_error_مقصد_crun_error (r, err);
}

ساكن صحيح
get_json_gen_نتیجه (json_gen_ctx *gen, حرف **بیرون, size_t *بیرون_len)
{
  ثابت حرف *buf = عدم;
  size_t buf_len = 0;
  صحيح r;

  r = json_gen_get_buf (gen, &buf, &buf_len);
  إذا (UNLIKELY (r != json_gen_وضعیت_ok))
    أرجع r;

  *بیرون_len = buf_len;

  *بیرون = xmalloc (buf_len + 1);
  کپی_حافظه (*بیرون, buf, buf_len);
  (*بیرون)[buf_len] = '\0';

  أرجع json_gen_وضعیت_ok;
}

ساكن صحيح
get_seccomp_receiver_fd_payload (libcrun_container_t *container, ثابت حرف *وضعیت, pid_t own_pid,
                                 حرف **seccomp_fd_payload, size_t *seccomp_fd_payload_len, libcrun_error_t *err)
{
  صحيح r;
  json_gen_ctx *gen = عدم;
  runtime_spec_schema_config_schema *def = container->container_def;
  ثابت حرف *ثابت OCI_نسخه = "0.2.0";

  إذا (! json_gen_init (&gen, عدم))
    أرجع crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_کلید (gen, "ociVersion");
  GEN_STR (gen, OCI_نسخه);

  GEN_کلید (gen, "fds");
  GEN_OR_FAIL (json_gen_array_open (gen));

  GEN_کلید (gen, "seccompFd");
  GEN_OR_FAIL (json_gen_array_close (gen));

  GEN_کلید (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  إذا (def && def->linux && def->linux->seccomp)
    {
      ثابت حرف *metadata = def->linux->seccomp->listener_metadata;

      إذا (metadata)
        {
          GEN_کلید (gen, "metadata");
          GEN_STR (gen, metadata);
        }
    }

  /* State.  */
  GEN_کلید (gen, "state");
  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_کلید (gen, "ociVersion");
  GEN_STR (gen, OCI_نسخه);

  إذا (container->context && container->context->id)
    {
      GEN_کلید (gen, "id");
      GEN_STR (gen, container->context->id);
    }

  GEN_کلید (gen, "status");
  GEN_STR (gen, وضعیت);

  GEN_کلید (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  إذا (container->context && container->context->bundle)
    {
      GEN_کلید (gen, "bundle");
      GEN_STR (gen, container->context->bundle);
    }

  إذا (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));
  /* End state.  */

  GEN_OR_FAIL (json_gen_map_close (gen));

  r = get_json_gen_نتیجه (gen, seccomp_fd_payload, seccomp_fd_payload_len);

gen_error:
  json_gen_free (gen);

  أرجع json_gen_error_مقصد_crun_error (r, err);
}

ساكن صحيح
send_sync_cb (فراغ *داده, libcrun_error_t *err)
{
  صحيح sync_socket_fd = *((صحيح *) داده);
  صحيح ret;

  /* sync 2.  */
  ret = sync_socket_send_sync (sync_socket_fd, false, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* sync 3.  */
  أرجع sync_socket_wait_sync (عدم, sync_socket_fd, false, err);
}

ساكن صحيح
maybe_chown_std_streams (uid_t container_uid, gid_t container_gid, libcrun_error_t *err)
{
  صحيح ret, i;

  لكل (i = 0; i < 3; i++)
    {
      إذا (! isatty (i))
        {
          بنية stat statbuf;
          ret = fstat (i, &statbuf);
          إذا (UNLIKELY (ret < 0))
            {
              إذا (errno == EBADF)
                استمر;
              أرجع crun_make_error (err, errno, "fstat fd `%d`", i);
            }

          /* Skip chown for device files */
          إذا (S_ISCHR (statbuf.st_mode) || S_ISBLK (statbuf.st_mode))
            استمر;

          ret = fchown (i, container_uid, container_gid);
          إذا (UNLIKELY (ret < 0))
            {
              /* EINVAL means the user is not mapped in the current userns.
                 Ignore EPERM and EROFS as well as there is no reason to fail
                 so early, and let the container payload deal with it.
                 EBADF means fd is closed.
              */
              إذا (errno == EINVAL || errno == EPERM || errno == EROFS || errno == EBADF)
                استمر;

              أرجع crun_make_error (err, errno, "fchown std stream `%i`", i);
            }
        }
    }
  أرجع 0;
}

صحيح
libcrun_container_notify_handler (بنية container_entrypoint_s *آرگومانها,
                                  تعداد handler_configure_phase phase,
                                  libcrun_container_t *container, ثابت حرف *rootfs,
                                  libcrun_error_t *err)
{
  بنية custom_handler_s *h;

  إذا (آرگومانها->custom_handler == عدم)
    أرجع 0;

  h = آرگومانها->custom_handler->vtable;
  إذا (h == عدم || h->configure_container == عدم)
    أرجع 0;

  أرجع h->configure_container (آرگومانها->custom_handler->cookie, phase,
                                 آرگومانها->context, container,
                                 rootfs, err);
}

/* Resolve and normalize the container rootfs path.  */
ساكن صحيح
resolve_rootfs_مسیر (libcrun_container_t *container, حرف **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  إذا (def->ریشه && def->ریشه->مسیر)
    {
      *rootfs = realpath (def->ریشه->مسیر, عدم);
      إذا (UNLIKELY (*rootfs == عدم))
        {
          صحيح saved_errno = errno;

          /* If realpath failed for any reason, try the relative directory.  */
          إذا (def->ریشه->مسیر[0] == '/')
            {
              cleanup_free حرف *cwd = عدم;
              libcrun_error_t tmp_err = عدم;
              ssize_t len;
              صحيح proc_fd;

              proc_fd = libcrun_get_cached_proc_fd (container, &tmp_err);
              إذا (LIKELY (proc_fd >= 0))
                {
                  len = امن_readlinkat (proc_fd, "self/cwd", &cwd, 0, &tmp_err);
                  إذا (LIKELY (len > 0))
                    {
                      /* If the rootfs is under the current working directory, just use its relative path.  */
                      إذا (has_dir_پیشوند (def->ریشه->مسیر, cwd))
                        {
                          ثابت حرف *it = consume_slashes (def->ریشه->مسیر + len);
                          إذا (LIKELY (*it))
                            {
                              صحيح ret = faccessat (AT_FDCWD, it, F_OK, 0);
                              إذا (LIKELY (ret == 0))
                                *rootfs = xstrdup (it);
                            }
                        }
                    }
                }
              crun_error_release (&tmp_err);
            }

          إذا (*rootfs == عدم)
            {
              إذا (saved_errno == ENOENT)
                أرجع crun_make_error (err, ENOENT, "the rootfs path `%s` does not exist", def->ریشه->مسیر);

              *rootfs = xstrdup (def->ریشه->مسیر);
            }
        }
    }
  أرجع 0;
}

/* Initialize the environment variables.  */
ساكن صحيح
راهاندازی_environment (runtime_spec_schema_config_schema *def, uid_t container_uid, libcrun_error_t *err)
{
  صحيح ret;

  إذا (clearenv ())
    أرجع crun_make_error (err, errno, "clearenv");

  إذا (def->process)
    {
      size_t i;

      لكل (i = 0; i < def->process->env_len; i++)
        {
          /* A null element of the array of variables is parsed into a NULL,
             which putenv(3) cannot be handed.  */
          إذا (def->process->env[i] == عدم)
            أرجع crun_make_error (err, EINVAL, "environment variable is not specified");

          إذا (putenv (def->process->env[i]) < 0)
            أرجع crun_make_error (err, errno, "putenv `%s`", def->process->env[i]);
        }
    }

  إذا (is_خالی_string (getenv ("HOME")))
    {
      ret = مجموعه_home_env (container_uid);
      إذا (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          إذا (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            أرجع crun_make_error (err, errno, "setenv HOME");
        }
    }

  أرجع 0;
}

/* Configure terminal and console for interactive containers.  */
ساكن صحيح
راهاندازی_terminal (بنية container_entrypoint_s *entrypoint_آرگومانها, libcrun_container_t *container,
                صحيح console_socket, صحيح console_socketpair, صحيح has_terminal, libcrun_error_t *err)
{
  cleanup_close صحيح terminal_fd = -1;
  صحيح ret;

  إذا (! has_terminal)
    أرجع 0;

  fflush (stderr);

  إذا (console_socket >= 0 || (entrypoint_آرگومانها->has_terminal_socket_pair && console_socketpair >= 0))
    {
      terminal_fd = libcrun_مجموعه_terminal (container, err);
      إذا (UNLIKELY (terminal_fd < 0))
        أرجع terminal_fd;
    }

  إذا (console_socket >= 0)
    {
      ret = send_fd_مقصد_socket (console_socket, terminal_fd, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  وإلا إذا (entrypoint_آرگومانها->has_terminal_socket_pair && console_socketpair >= 0)
    {
      ret = send_fd_مقصد_socket (console_socketpair, terminal_fd, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  أرجع 0;
}

/* Resolve and validate the container's executable path.  */
ساكن صحيح
راهاندازی_executable_مسیر (بنية container_entrypoint_s *entrypoint_آرگومانها, runtime_spec_schema_config_schema *def,
                       حرف **exec_مسیر, libcrun_error_t *err)
{
  صحيح ret;

  إذا (def->process && def->process->آرگومانها)
    {
      ret = پیدا_کن_executable (exec_مسیر, def->process->آرگومانها[0], def->process->cwd, err);
      إذا (UNLIKELY (ret < 0))
        {
          إذا (entrypoint_آرگومانها->custom_handler == عدم && crun_error_get_errno (err) == ENOENT)
            أرجع ret;

          /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
             once the process switched to the use that runs in the container.  This might be necessary
             when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
             is not honored.  */
          crun_error_release (err);
        }
    }

  أرجع 0;
}

/* Apply security settings for the container process.  */
ساكن صحيح
apply_security_settings (بنية container_entrypoint_s *entrypoint_آرگومانها, runtime_spec_schema_config_schema *def,
                         libcrun_container_t *container, pid_t own_pid, libcrun_error_t *err)
{
  صحيح ret;
  runtime_spec_schema_config_schema_process_capabilities *capabilities;
  صحيح no_new_privs;

  إذا (def->process && ! def->process->no_new_privileges)
    {
      حرف **seccomp_flags = عدم;
      size_t seccomp_flags_len = 0;
      cleanup_free حرف *seccomp_fd_payload = عدم;
      size_t seccomp_fd_payload_len = 0;

      إذا (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      إذا (entrypoint_آرگومانها->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_آرگومانها->seccomp_fd, entrypoint_آرگومانها->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      close_and_reset (&entrypoint_آرگومانها->seccomp_fd);
      close_and_reset (&entrypoint_آرگومانها->seccomp_receiver_fd);
    }

  capabilities = def->process ? def->process->capabilities : عدم;
  no_new_privs = def->process ? def->process->no_new_privileges : 1;
  ret = libcrun_مجموعه_caps (capabilities, container->container_uid, container->container_gid, no_new_privs,
                          entrypoint_آرگومانها->custom_handler != عدم, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع 0;
}

/* Create the process that joins the existing PID namespace now that the
   host mounts are not visible anymore, and continue the setup from there.  */
ساكن صحيح
do_delayed_pidns_fork (صحيح sync_socket, pid_t *own_pid, libcrun_error_t *err)
{
  pid_t pid;
  صحيح ret;

  pid = fork ();
  إذا (UNLIKELY (pid < 0))
    أرجع crun_make_error (err, errno, "fork");

  إذا (pid)
    {
      /* Report the new PID to the parent and exit immediately.  */
      ret = sync_socket_send_sync (sync_socket, false, err);
      إذا (LIKELY (ret == 0))
        {
          ret = موقت_FAILURE_RETRY (اكتب (sync_socket, &pid, حجم (pid)));
          إذا (LIKELY (ret == حجم (pid)))
            امن_exit (EXIT_SUCCESS);
        }

      kill (pid, SIGKILL);
      امن_exit (EXIT_FAILURE);
    }

  /* sync receive own pid.  */
  ret = موقت_FAILURE_RETRY (اقرأ (sync_socket, own_pid, حجم (*own_pid)));
  إذا (UNLIKELY (ret != حجم (*own_pid)))
    {
      إذا (ret >= 0)
        errno = 0;
      أرجع crun_make_error (err, errno, "read from sync socket");
    }

  أرجع 0;
}

بنية delayed_pidns_fork_s
{
  libcrun_container_t *container;
  صحيح sync_socket;
  pid_t *own_pid;
};

ساكن صحيح
delayed_pidns_fork_cb (فراغ *داده, libcrun_error_t *err)
{
  بنية delayed_pidns_fork_s *d = داده;

  إذا (! libcrun_container_delayed_pidns_fork (d->container))
    أرجع 0;

  أرجع do_delayed_pidns_fork (d->sync_socket, d->own_pid, err);
}

ساكن صحيح
container_init_راهاندازی (فراغ *آرگومانها, pid_t *own_pid, حرف *notify_socket,
                      صحيح sync_socket, حرف **exec_مسیر, libcrun_error_t *err)
{
  بنية container_entrypoint_s *entrypoint_آرگومانها = آرگومانها;
  libcrun_container_t *container = entrypoint_آرگومانها->container;
  bool chdir_انجامشده = false;
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
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_configure_شبکه (container, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = resolve_rootfs_مسیر (container, &rootfs, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* Configure terminal socket pair for container communication.  */
  إذا (entrypoint_آرگومانها->terminal_socketpair[0] >= 0)
    {
      close_and_reset (&entrypoint_آرگومانها->terminal_socketpair[0]);
      console_socketpair = entrypoint_آرگومانها->terminal_socketpair[1];
    }

  /* sync 1.  */
  ret = sync_socket_wait_sync (عدم, sync_socket, false, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  has_terminal = container->container_def->process && container->container_def->process->terminal;
  إذا (has_terminal && entrypoint_آرگومانها->context->console_socket)
    console_socket = entrypoint_آرگومانها->console_socket_fd;

  ret = libcrun_مجموعه_sysctl (container, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_container_notify_handler (entrypoint_آرگومانها, HANDLER_CONFIGURE_BEFORE_MOUNTS, container, rootfs, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_do_محور_ریشه (container, entrypoint_آرگومانها->context->no_محور, &rootfs, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* sync 2 and 3 are sent as part of libcrun_set_mounts.  */
  ret = libcrun_مجموعه_mounts (entrypoint_آرگومانها, container, rootfs, send_sync_cb, &sync_socket, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* The createContainer hooks MUST run after the mounts are set up but before
     the pivot_root.  When these hooks are present can_use_open_tree_namespace()
     forces the deferred-pivot path, so the actual pivot happens later in
     libcrun_finalize_mounts().  */
  إذا (def->hooks && def->hooks->create_container_len)
    {
      libcrun_error_t tmp_err = عدم;
      صحيح درون_userns = check_running_درون_user_namespace (&tmp_err);
      إذا (tmp_err)
        crun_error_release (&tmp_err);
      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_آرگومانها->bundle, عدم, "created", "createContainer",
                      (hook **) def->hooks->create_container,
                      def->hooks->create_container_len, entrypoint_آرگومانها->hooks_بیرون_fd, entrypoint_آرگومانها->hooks_err_fd,
                      درون_userns > 0, err);
      إذا (UNLIKELY (ret != 0))
        أرجع ret;
    }

  ret = libcrun_finalize_mounts (entrypoint_آرگومانها, container, rootfs, delayed_pidns_fork_cb, &delayed_pidns_fork, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (def->process)
    {
      ret = libcrun_مجموعه_selinux_label (container, def->process, false, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = libcrun_مجموعه_apparmor_profile (container, def->process, false, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = mark_or_close_fds_ge_than (container, entrypoint_آرگومانها->context->preserve_fds + 3, false, err);
  إذا (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ret = libcrun_reopen_dev_null (err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = maybe_chown_std_streams (container->container_uid, container->container_gid, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = راهاندازی_environment (def, container->container_uid, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* Set primary process to 1 explicitly if nothing is configured and LISTEN_FD is not set.  */
  إذا (entrypoint_آرگومانها->context->listen_fds > 0 && getenv ("LISTEN_PID") == عدم)
    {
      libcrun_warning ("setting LISTEN_PID=1 since no previous configuration was found");
      إذا (UNLIKELY (setenv ("LISTEN_PID", "1", 1) < 0))
        أرجع crun_make_error (err, errno, "setenv LISTEN_PID");
    }

  /* Attempt to chdir immediately here, before doing the setresuid.  If we fail here, let's
     try again later once the process switched to the user that runs in the container.  */
  إذا (def->process && def->process->cwd)
    {
      ret = libcrun_امن_chdir (def->process->cwd, err);
      إذا (LIKELY (ret == 0))
        chdir_انجامشده = true;
      وإلا
        crun_error_release (err);
    }

  ret = setsid ();
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "setsid");

  ret = راهاندازی_terminal (entrypoint_آرگومانها, container, console_socket, console_socketpair, has_terminal, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  close_and_reset (&console_socket);
  close_and_reset (&console_socketpair);

  ret = راهاندازی_executable_مسیر (entrypoint_آرگومانها, def, exec_مسیر, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_مجموعه_hostname (container, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_مجموعه_domainname (container, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_مجموعه_personality (container->container_def->linux->personality, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (def->process && def->process->user)
    umask (def->process->user->umask_present ? def->process->user->umask : 0022);

  ret = apply_security_settings (entrypoint_آرگومانها, def, container, *own_pid, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (UNLIKELY (def->process && def->process->آرگومانها && *exec_مسیر == عدم))
    {
      ret = پیدا_کن_executable (exec_مسیر, def->process->آرگومانها[0], def->process->cwd, err);
      إذا (UNLIKELY (ret < 0))
        {
          إذا (entrypoint_آرگومانها->custom_handler == عدم || is_خالی_string (def->process->آرگومانها[0]))
            أرجع ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          *exec_مسیر = xstrdup (def->process->آرگومانها[0]);
        }
    }

  /* The chdir was not already performed, so try again now after switching to the UID/GID in the container.  */
  إذا (! chdir_انجامشده && def->process && def->process->cwd)
    {
      ret = libcrun_امن_chdir (def->process->cwd, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (notify_socket && putenv (notify_socket) < 0)
    أرجع crun_make_error (err, errno, "putenv `%s`", notify_socket);

  أرجع 0;
}

ساكن صحيح
open_hooks_output (libcrun_container_t *container, صحيح *بیرون_fd, صحيح *err_fd, libcrun_error_t *err)
{
  ثابت حرف *annotation;

  *err_fd = *بیرون_fd = -1;

  libcrun_debug ("Opening hooks output");
  annotation = پیدا_کن_annotation (container, "run.oci.hooks.stdout");
  إذا (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stdout` annotation");
      *بیرون_fd = موقت_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      إذا (UNLIKELY (*بیرون_fd < 0))
        أرجع crun_make_error (err, errno, "open `%s`", annotation);
    }

  annotation = پیدا_کن_annotation (container, "run.oci.hooks.stderr");
  إذا (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stderr` annotation");
      *err_fd = موقت_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      إذا (UNLIKELY (*err_fd < 0))
        أرجع crun_make_error (err, errno, "open `%s`", annotation);
    }

  أرجع 0;
}

/* The environment the process was started with lives on the initial stack, right after the
   argv strings and before the string AT_EXECFN points to.  clearenv(3) only drops the pointers
   to it, so when there is no execv to replace the stack, erase it or the container process can
   still read it, e.g. through /proc/self/environ.  */
ساكن فراغ
erase_initial_environment (حرف **argv, صحيح argc)
{
  حرف *شروع = argv[argc - 1] + طول_رشته (argv[argc - 1]) + 1;
  حرف *انتها = (حرف *) getauxval (AT_EXECFN);

  إذا (انتها > شروع)
    memset (شروع, 0, انتها - شروع);
}

ساكن فراغ
rewrite_argv (حرف **argv, صحيح argc, ثابت حرف *name, حرف **آرگومانها, size_t آرگومانها_len)
{
  cleanup_free حرف *decorated_name = عدم;
  size_t موجود_len = argv[argc - 1] - argv[0] + طول_رشته (argv[argc - 1]) + 1;
  cleanup_free حرف *new_argv = عدم;
  size_t i, so_far = 0, needed = 0;

  needed = xasprintf (&decorated_name, "[libcrun:%s]", name) + 1;
  لكل (i = 0; i < آرگومانها_len; i++)
    needed += طول_رشته (آرگومانها[i]) + 1;

  new_argv = xmalloc0 (needed + 1);
  strcpy (new_argv, decorated_name);
  so_far = طول_رشته (decorated_name) + 1;

  إذا (موجود_len >= needed)
    {
      لكل (i = 0; i < آرگومانها_len; i++)
        {
          strcpy (new_argv + so_far, آرگومانها[i]);
          so_far += طول_رشته (آرگومانها[i]) + 1;
        }
    }

  إذا (so_far >= موجود_len)
    so_far = موجود_len - 1;

  memset (argv[0], 0, موجود_len);
  کپی_حافظه (argv[0], new_argv, so_far);
}

/* Entrypoint to the container.  */
ساكن صحيح
container_init (فراغ *آرگومانها, حرف *notify_socket, صحيح sync_socket, libcrun_error_t *err)
{
  بنية container_entrypoint_s *entrypoint_آرگومانها = آرگومانها;
  صحيح ret;
  runtime_spec_schema_config_schema *def = entrypoint_آرگومانها->container->container_def;
  cleanup_free حرف *exec_مسیر = عدم;
  __attribute__ ((استفادهنشده)) cleanup_free حرف *notify_socket_cleanup = notify_socket;
  pid_t own_pid = 0;

  entrypoint_آرگومانها->sync_socket = sync_socket;

  crun_مجموعه_output_handler (log_write_مقصد_sync_socket, آرگومانها);

  /* sync receive own pid.  */
  ret = موقت_FAILURE_RETRY (اقرأ (sync_socket, &own_pid, حجم (own_pid)));
  إذا (UNLIKELY (ret != حجم (own_pid)))
    {
      إذا (ret >= 0)
        errno = 0;
      أرجع crun_make_error (err, errno, "read from sync socket");
    }

  ret = container_init_راهاندازی (آرگومانها, &own_pid, notify_socket, sync_socket, &exec_مسیر, err);
  إذا (UNLIKELY (ret < 0))
    {
      /* If it fails to write the error using the sync socket, then fallback
         to stderr.  */
      إذا (sync_socket_write_error (sync_socket, err) == 0)
        {
          crun_error_release (err);
          /* A return value of > 0 means to fail with an error without err being set so to avoid
             printing it twice.  */
          أرجع 1;
        }
      أرجع ret;
    }

  entrypoint_آرگومانها->sync_socket = -1;

  ret = unblock_سیگنالها (err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* sync 4.  */
  ret = sync_socket_send_sync (sync_socket, false, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  close_and_reset (&sync_socket);

  إذا (entrypoint_آرگومانها->context->fifo_exec_wait_fd >= 0)
    {
      حرف میانگیر[1];
      fd_set read_مجموعه;
      cleanup_close صحيح fd = entrypoint_آرگومانها->context->fifo_exec_wait_fd;
      entrypoint_آرگومانها->context->fifo_exec_wait_fd = -1;

      FD_ZERO (&read_مجموعه);
      FD_SET (fd, &read_مجموعه);
      افعل
        {
          ret = select (fd + 1, &read_مجموعه, عدم, عدم, عدم);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "select");

          ret = موقت_FAILURE_RETRY (اقرأ (fd, میانگیر, حجم (میانگیر)));
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "read from the exec fifo");
      } طالما (ret == 0);

      close_and_reset (&entrypoint_آرگومانها->context->fifo_exec_wait_fd);
    }

  crun_مجموعه_output_handler (log_write_مقصد_stderr, عدم);

  إذا (def->process && def->process->no_new_privileges)
    {
      حرف **seccomp_flags = عدم;
      size_t seccomp_flags_len = 0;
      cleanup_free حرف *seccomp_fd_payload = عدم;
      size_t seccomp_fd_payload_len = 0;

      إذا (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      إذا (entrypoint_آرگومانها->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (entrypoint_آرگومانها->container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_آرگومانها->seccomp_fd, entrypoint_آرگومانها->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags,
                                   seccomp_flags_len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      close_and_reset (&entrypoint_آرگومانها->seccomp_fd);
      close_and_reset (&entrypoint_آرگومانها->seccomp_receiver_fd);
    }

  إذا (UNLIKELY (def->process == عدم))
    أرجع crun_make_error (err, 0, "block `process` not found");

  إذا (UNLIKELY (exec_مسیر == عدم))
    أرجع crun_make_error (err, 0, "executable path not specified");

  إذا (def->hooks && def->hooks->شروع_container_len)
    {
      libcrun_container_t *container = entrypoint_آرگومانها->container;
      libcrun_error_t tmp_err = عدم;
      صحيح درون_userns = check_running_درون_user_namespace (&tmp_err);
      إذا (tmp_err)
        crun_error_release (&tmp_err);

      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_آرگومانها->bundle, عدم, "created", "startContainer",
                      (hook **) def->hooks->شروع_container,
                      def->hooks->شروع_container_len, entrypoint_آرگومانها->hooks_بیرون_fd, entrypoint_آرگومانها->hooks_err_fd,
                      درون_userns > 0, err);
      إذا (UNLIKELY (ret != 0))
        أرجع ret;

      /* Seek stdout/stderr to the end.  If the hooks were using the same files,
         the container process overwrites what was previously written.  */
      (فراغ) lseek (1, 0, SEEK_END);
      (فراغ) lseek (2, 0, SEEK_END);
    }

  إذا (entrypoint_آرگومانها->custom_handler)
    {
      /* Files marked with O_CLOEXEC are closed at execv time, so make sure they are closed now.
         This is a best effort operation, because the seccomp filter is already in place and it could
         stop some syscalls used by mark_or_close_fds_ge_than.
      */
      إذا (entrypoint_آرگومانها->custom_handler->vtable->close_fds)
        {
          ret = entrypoint_آرگومانها->custom_handler->vtable->close_fds (entrypoint_آرگومانها->custom_handler->cookie,
                                                                    entrypoint_آرگومانها->container,
                                                                    entrypoint_آرگومانها->context->preserve_fds,
                                                                    err);
        }
      وإلا
        {
          ret = mark_or_close_fds_ge_than (entrypoint_آرگومانها->container, entrypoint_آرگومانها->context->preserve_fds + 3, true, err);
        }
      إذا (UNLIKELY (ret < 0))
        crun_error_release (err);

      prctl (PR_مجموعه_NAME, entrypoint_آرگومانها->custom_handler->vtable->name);

      إذا (entrypoint_آرگومانها->context->argv)
        {
          erase_initial_environment (entrypoint_آرگومانها->context->argv, entrypoint_آرگومانها->context->argc);
          rewrite_argv (entrypoint_آرگومانها->context->argv, entrypoint_آرگومانها->context->argc,
                        entrypoint_آرگومانها->custom_handler->vtable->name, def->process->آرگومانها,
                        def->process->آرگومانها_len);

          /* It is a quite destructive operation as we might be referencing data from the old
             argv, so make sure the context is not reused.  */
          entrypoint_آرگومانها->context->argv = عدم;
          entrypoint_آرگومانها->context = عدم;
        }

      ret = libcrun_مجموعه_selinux_label (entrypoint_آرگومانها->container, def->process, true, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = libcrun_مجموعه_apparmor_profile (entrypoint_آرگومانها->container, def->process, true, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = entrypoint_آرگومانها->custom_handler->vtable->پیاپی_func (entrypoint_آرگومانها->custom_handler->cookie,
                                                               entrypoint_آرگومانها->container,
                                                               exec_مسیر,
                                                               def->process->آرگومانها);
      إذا (ret != 0)
        أرجع crun_make_error (err, 0, "exec container process failed with handler as `%s`", entrypoint_آرگومانها->custom_handler->vtable->name);

      أرجع ret;
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation since the seccomp profile is already in place now and might block
     some of the syscalls needed by mark_or_close_fds_ge_than.  */
  ret = mark_or_close_fds_ge_than (entrypoint_آرگومانها->container, entrypoint_آرگومانها->context->preserve_fds + 3, true, err);
  إذا (UNLIKELY (ret < 0))
    crun_error_release (err);

  موقت_FAILURE_RETRY (execv (exec_مسیر, def->process->آرگومانها));

  إذا (errno == ENOENT)
    أرجع crun_make_error (err, errno, "exec container process (missing dynamic library?) `%s`", exec_مسیر);

  أرجع crun_make_error (err, errno, "exec container process `%s`", exec_مسیر);
}

ساكن صحيح
read_container_config_from_state (libcrun_container_t **container, ثابت حرف *state_ریشه, ثابت حرف *id,
                                  libcrun_error_t *err)
{
  cleanup_free حرف *config_file = عدم;
  cleanup_free حرف *dir = عدم;
  صحيح ret;

  *container = عدم;

  ret = libcrun_get_state_directory (&dir, state_ریشه, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = append_paths (&config_file, err, dir, "config.json", عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  *container = libcrun_container_بارگیری_from_file (config_file, err);
  إذا (*container == عدم)
    أرجع -1;

  أرجع 0;
}

ساكن صحيح
پیاپی_poststop_hooks (arg_استفادهنشده libcrun_context_t *context, libcrun_container_t *container,
                    runtime_spec_schema_config_schema *def,
                    libcrun_container_وضعیت_t *وضعیت, ثابت حرف *state_ریشه, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container_cleanup = عدم;
  صحيح ret;

  إذا (def == عدم)
    {
      إذا (container == عدم)
        {
          ret = read_container_config_from_state (&container_cleanup, state_ریشه, id, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          container = container_cleanup;
        }

      def = container->container_def;
    }

  إذا (def->hooks && def->hooks->poststop_len)
    {
      cleanup_close صحيح hooks_بیرون_fd = -1;
      cleanup_close صحيح hooks_err_fd = -1;

      إذا (container == عدم)
        {
          ret = read_container_config_from_state (&container_cleanup, state_ریشه, id, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          container = container_cleanup;
        }

      ret = open_hooks_output (container, &hooks_بیرون_fd, &hooks_err_fd, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = do_hooks (def, 0, id, true, وضعیت->bundle, وضعیت->bundle, "stopped", "poststop", (hook **) def->hooks->poststop,
                      def->hooks->poststop_len, hooks_بیرون_fd, hooks_err_fd, false, err);
      إذا (UNLIKELY (ret != 0))
        {
          إذا (ret < 0)
            libcrun_error_report_and_release (err);
          وإلا
            libcrun_error (0, "poststop hook failed with exit code: %d", ret);
        }
    }
  أرجع 0;
}

ساكن bool
has_new_pid_namespace (runtime_spec_schema_config_schema *def)
{
  size_t i;

  إذا (def->linux == عدم || def->linux->namespaces == عدم)
    أرجع false;

  لكل (i = 0; i < def->linux->namespaces_len; i++)
    {
      إذا (strcmp (def->linux->namespaces[i]->type, "pid") == 0 && def->linux->namespaces[i]->مسیر == عدم)
        أرجع true;
    }
  أرجع false;
}

/* Delete the container state.  CONTAINER is the container the caller already has
   available, or NULL.  When it is not NULL, the poststop hooks are run even if the
   status file was never written, e.g. when the container creation failed.  */
ساكن صحيح
container_delete_داخلی (libcrun_context_t *context, libcrun_container_t *container,
                           runtime_spec_schema_config_schema *def, ثابت حرف *id, bool force, bool killall,
                           libcrun_error_t *err)
{
  cleanup_cgroup_وضعیت بنية libcrun_cgroup_وضعیت *cgroup_وضعیت = عدم;
  cleanup_container_وضعیت libcrun_container_وضعیت_t وضعیت = {};
  cleanup_container libcrun_container_t *container_cleanup = عدم;
  ثابت حرف *state_ریشه = context->state_ریشه;
  صحيح ret;

  ret = libcrun_read_container_وضعیت (&وضعیت, state_ریشه, id, err);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (force && crun_error_get_errno (err) == ENOENT)
        {
          libcrun_error_t tmp_err = عدم;

          crun_error_release (err);

          إذا (container)
            {
              libcrun_container_وضعیت_t partial_وضعیت = {
                .bundle = (حرف *) context->bundle,
              };

              ret = پیاپی_poststop_hooks (context, container, def, &partial_وضعیت, state_ریشه, id, err);
              إذا (UNLIKELY (ret < 0))
                crun_error_write_warning_and_release (context->output_handler_arg, &err);
            }

          libcrun_container_delete_وضعیت (state_ریشه, id, &tmp_err);
          crun_error_release (&tmp_err);
          أرجع 0;
        }
      crun_error_release (err);
      أرجع libcrun_container_delete_وضعیت (state_ریشه, id, err);
    }

  cgroup_وضعیت = libcrun_cgroup_make_وضعیت (&وضعیت);

  إذا (! force)
    {
      ret = libcrun_is_container_running (&وضعیت, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      إذا (ret == 1)
        {
          ret = libcrun_وضعیت_has_read_exec_fifo (state_ریشه, id, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          إذا (ret == 0)
            أرجع crun_make_error (err, 0, "the container `%s` is not in `created` or `stopped` state", id);

          /* If the container is in "created" state, then do the equivalent of delete --force.  */
          killall = force = true;
        }
    }

  إذا (def == عدم)
    {
      إذا (container == عدم)
        {
          ret = read_container_config_from_state (&container_cleanup, state_ریشه, id, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

          container = container_cleanup;
        }

      def = container->container_def;
    }

  إذا (killall && force)
    {
      /* If the container has a pid namespace, it is enough to kill the first
         process (pid=1 in the namespace).
      */
      إذا (has_new_pid_namespace (def))
        {
          ret = libcrun_kill_linux (&وضعیت, SIGKILL, err);
          إذا (UNLIKELY (ret < 0))
            {
              إذا (crun_error_get_errno (err) != ESRCH)
                أرجع ret;

              crun_error_release (err);
            }
        }
      وإلا إذا (وضعیت.cgroup_مسیر)
        {
          ret = libcrun_cgroup_killall (cgroup_وضعیت, SIGKILL, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
    }

  إذا (def->linux && def->linux->intel_rdt)
    {
      ret = libcrun_destroy_intelrdt (id, def, err);
      إذا (UNLIKELY (ret < 0))
        libcrun_error_report_and_release (err);
    }

  إذا (وضعیت.cgroup_مسیر)
    {
      /* If the cgroup cannot be destroyed, there are still processes in it.
         Keep the container status, it is the only record of the cgroup, so
         that the delete can be retried.  */
      ret = libcrun_cgroup_destroy (cgroup_وضعیت, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = پیاپی_poststop_hooks (context, container, def, &وضعیت, state_ریشه, id, err);
  إذا (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  أرجع libcrun_container_delete_وضعیت (state_ریشه, id, err);
}

صحيح
libcrun_container_delete (libcrun_context_t *context, ثابت حرف *id, bool force, libcrun_error_t *err)
{
  أرجع container_delete_داخلی (context, عدم, عدم, id, force, true, err);
}

صحيح
libcrun_container_kill (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *إشارة, libcrun_error_t *err)
{
  صحيح sig, ret;
  ثابت حرف *state_ریشه = context->state_ریشه;
  cleanup_container_وضعیت libcrun_container_وضعیت_t وضعیت = {};

  sig = str2sig (إشارة);
  إذا (UNLIKELY (sig < 0))
    أرجع crun_make_error (err, 0, "unknown signal `%s`", إشارة);

  ret = libcrun_read_container_وضعیت (&وضعیت, state_ریشه, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع libcrun_kill_linux (&وضعیت, sig, err);
}

صحيح
libcrun_container_killall (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *إشارة, libcrun_error_t *err)
{
  صحيح sig, ret;
  ثابت حرف *state_ریشه = context->state_ریشه;
  cleanup_container_وضعیت libcrun_container_وضعیت_t وضعیت = {};
  cleanup_cgroup_وضعیت بنية libcrun_cgroup_وضعیت *cgroup_وضعیت = عدم;

  sig = str2sig (إشارة);
  إذا (UNLIKELY (sig < 0))
    أرجع crun_make_error (err, 0, "unknown signal `%s`", إشارة);

  ret = libcrun_read_container_وضعیت (&وضعیت, state_ریشه, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  cgroup_وضعیت = libcrun_cgroup_make_وضعیت (&وضعیت);

  ret = libcrun_cgroup_killall (cgroup_وضعیت, sig, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;
  أرجع 0;
}

ساكن صحيح
write_container_وضعیت (libcrun_container_t *container, libcrun_context_t *context,
                        pid_t pid, بنية libcrun_cgroup_وضعیت *cgroup_وضعیت,
                        libcrun_error_t *err)
{
  cleanup_free حرف *bundle_allocated = عدم;
  ثابت حرف *bundle = get_bundle_مسیر (context->bundle, &bundle_allocated);
  cleanup_free حرف *owner = get_user_name (geteuid ());
  حرف *external_descriptors = libcrun_get_external_descriptors (container);
  حرف *rootfs = container->container_def->ریشه ? container->container_def->ریشه->مسیر : "";
  حرف created[35];

  libcrun_container_وضعیت_t وضعیت = {
    .pid = pid,
    .rootfs = rootfs,
    .bundle = (حرف *) bundle,
    .created = created,
    .owner = owner,
    .systemd_cgroup = context->systemd_cgroup,
    .detached = context->detach,
    .no_new_keyring = context->no_new_keyring,
    .external_descriptors = external_descriptors,
    .cgroup_مسیر = عدم,
    .scope = عدم,
  };

  get_current_timestamp (created, حجم (created));

  إذا (cgroup_وضعیت)
    {
      صحيح ret;

      ret = libcrun_cgroup_get_وضعیت (cgroup_وضعیت, &وضعیت, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (external_descriptors == عدم)
    أرجع crun_make_error (err, 0, "invalid internal state.  No external descriptors found");
  أرجع libcrun_write_container_وضعیت (context->state_ریشه, context->id, &وضعیت, err);
}

ساكن صحيح
reap_subprocesses (pid_t *main_process, صحيح *main_process_exit, صحيح *آخرین_process, libcrun_error_t *err)
{
  *آخرین_process = 0;
  طالما (1)
    {
      صحيح وضعیت;
      صحيح r = waitpid_ignore_stopped (-1, &وضعیت, WNOHANG);
      إذا (r < 0)
        {
          إذا (errno == EINTR)
            استمر;
          إذا (errno == ECHILD)
            {
              *آخرین_process = 1;
              أرجع 0;
            }
          أرجع crun_make_error (err, errno, "waitpid");
        }
      إذا (r == 0)
        اكسر;
      إذا (r != *main_process)
        استمر;

      *main_process_exit = get_process_exit_وضعیت (وضعیت);

      /* Once reaped, the pid can be reused by an unrelated process.  */
      *main_process = 0;
    }
  أرجع 0;
}

ساكن صحيح
send_sd_notify (ثابت حرف *ready_str, libcrun_error_t *err)
{
#اگر_تعریف_شده HAVE_SYSTEMD
  صحيح ret = sd_notify (0, ready_str);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, -ret, "sd_notify");

#  إذا HAVE_SD_NOTIFY_BARRIER
  /* Hard-code a 30 seconds timeout.  Ignore errors.  */
  sd_notify_barrier (0, 30 * 1000000);
#  endif
#وإلا
  (فراغ) ready_str;
  (فراغ) err;
#پایان_اگر
  أرجع 0;
}

ساكن صحيح
handle_notify_socket (صحيح notify_socketfd, libcrun_error_t *err)
{
#اگر_تعریف_شده HAVE_SYSTEMD
  صحيح ret;
  حرف buf[256];
  ثابت حرف *ready_str = "READY=1";

  ret = recvfrom (notify_socketfd, buf, حجم (buf) - 1, 0, عدم, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "recvfrom notify socket");

  buf[ret] = '\0';
  إذا (جستجوی_رشته (buf, ready_str))
    {
      ret = send_sd_notify (ready_str, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      أرجع 1;
    }
  أرجع 0;
#وإلا
  (فراغ) notify_socketfd;
  (فراغ) err;
  أرجع 1;
#پایان_اگر
}

بنية wait_for_process_آرگومانها
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
  إذا (UNLIKELY (buf_len >= (صحيح) حجم (buf)))
    أرجع crun_make_error (err, 0, "internal error: static buffer too small");
  أرجع write_file_at_with_flags (AT_FDCWD, O_CREAT | O_TRUNC, 0700, pid_file, buf, buf_len, err);
}

#تعریف_کن MAX_EVENTS 10

ساكن bool
is_fd_pollable (صحيح fd)
{
  بنية epoll_event ev = { .events = EPOLLIN };
  cleanup_close صحيح epollfd = epoll_create1 (EPOLL_CLOEXEC);

  إذا (epollfd < 0)
    أرجع true;

  أرجع epoll_ctl (epollfd, EPOLL_CTL_جمع_کن, fd, &ev) == 0 || errno != EPERM;
}

ساكن صحيح
wait_for_process (بنية wait_for_process_آرگومانها *آرگومانها, libcrun_error_t *err)
{
  cleanup_channel_fd_pair بنية channel_fd_pair *from_terminal = عدم;
  cleanup_channel_fd_pair بنية channel_fd_pair *مقصد_terminal = عدم;
  صحيح ret, container_exit_کد = 0, آخرین_process;
  cleanup_close صحيح terminal_fd_from = -1;
  cleanup_close صحيح terminal_fd_مقصد = -1;
  bool stdin_pollable = false, stdout_pollable = false;
  cleanup_close صحيح epollfd = -1;
  cleanup_close صحيح signalfd = -1;
  sigset_t ماسک;
  صحيح درون_fds[MAX_EVENTS];
  صحيح درون_fds_len = 0;
  صحيح بیرون_fds[MAX_EVENTS];
  صحيح بیرون_fds_len = 0;
  size_t i;

  cleanup_seccomp_notify_context بنية seccomp_notify_context_s *seccomp_notify_ctx = عدم;

  container_exit_کد = 0;

  إذا (آرگومانها == عدم || آرگومانها->context == عدم)
    أرجع crun_make_error (err, 0, "internal error: context is empty");

  لكل (i = 0; i < MAX_EVENTS; i++)
    {
      درون_fds[i] = -1;
      بیرون_fds[i] = -1;
    }

  إذا (آرگومانها->context->pid_file)
    {
      ret = write_pid_file (آرگومانها->context->pid_file, آرگومانها->pid, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  /* Also exit if there is nothing more to wait for.  */
  إذا (آرگومانها->context->detach && آرگومانها->notify_socket < 0)
    أرجع 0;

  إذا (آرگومانها->container_ready_fd)
    {
      ret = 0;
      موقت_FAILURE_RETRY (اكتب (*آرگومانها->container_ready_fd, &ret, حجم (ret)));
      close_and_reset (آرگومانها->container_ready_fd);
    }

  sigfillset (&ماسک);
  ret = sigprocmask (SIG_بلوک, &ماسک, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "sigprocmask");

  signalfd = create_signalfd (&ماسک, err);
  إذا (UNLIKELY (signalfd < 0))
    أرجع signalfd;

  ret = reap_subprocesses (&آرگومانها->pid, &container_exit_کد, &آخرین_process, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (آخرین_process)
    أرجع container_exit_کد;

  إذا (آرگومانها->seccomp_notify_fd >= 0)
    {
      cleanup_free حرف *state_ریشه = عدم;
      cleanup_free حرف *oci_config_مسیر = عدم;

      بنية libcrun_بارگیری_seccomp_notify_conf_s conf;
      memset (&conf, 0, حجم conf);

      ret = libcrun_get_state_directory (&state_ریشه, آرگومانها->context->state_ریشه, آرگومانها->context->id, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = append_paths (&oci_config_مسیر, err, state_ریشه, "config.json", عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      conf.runtime_ریشه_مسیر = state_ریشه;
      conf.name = آرگومانها->context->id;
      conf.bundle_مسیر = آرگومانها->context->bundle;
      conf.oci_config_مسیر = oci_config_مسیر;

      ret = مجموعه_blocking_fd (آرگومانها->seccomp_notify_fd, false, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = libcrun_بارگیری_seccomp_notify_plugins (&seccomp_notify_ctx,
                                                 آرگومانها->seccomp_notify_plugins,
                                                 &conf, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      درون_fds[درون_fds_len++] = آرگومانها->seccomp_notify_fd;
    }

  إذا (آرگومانها->terminal_fd >= 0)
    {
      /* The terminal_fd is dup()ed so that it can be registered with
         epoll multiple times using different masks.  */
      terminal_fd_from = dup (آرگومانها->terminal_fd);
      إذا (UNLIKELY (terminal_fd_from < 0))
        أرجع crun_make_error (err, errno, "dup terminal fd");
      terminal_fd_مقصد = dup (آرگومانها->terminal_fd);
      إذا (UNLIKELY (terminal_fd_مقصد < 0))
        أرجع crun_make_error (err, errno, "dup terminal fd");

      صحيح i, non_blocking_fds[] = { terminal_fd_from, terminal_fd_مقصد, 0, 1, -1 };
      لكل (i = 0; non_blocking_fds[i] >= 0; i++)
        {
          ret = مجموعه_blocking_fd (non_blocking_fds[i], false, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }

      /* stdin and stdout can be something epoll does not support, such as
         /dev/null or a regular file.  Such a stdin is not forwarded, and
         such a stdout is written to without waiting for it to be ready.  */
      stdin_pollable = is_fd_pollable (0);
      stdout_pollable = is_fd_pollable (1);

      from_terminal = channel_fd_pair_new (terminal_fd_from, 1, BUFSIZ);
      إذا (stdin_pollable)
        مقصد_terminal = channel_fd_pair_new (0, terminal_fd_مقصد, BUFSIZ);
    }

  درون_fds[درون_fds_len++] = signalfd;
  إذا (آرگومانها->notify_socket >= 0)
    درون_fds[درون_fds_len++] = آرگومانها->notify_socket;
  إذا (آرگومانها->terminal_fd >= 0)
    {
      إذا (stdin_pollable)
        {
          درون_fds[درون_fds_len++] = 0;
          بیرون_fds[بیرون_fds_len++] = terminal_fd_مقصد;
        }

      درون_fds[درون_fds_len++] = terminal_fd_from;
      إذا (stdout_pollable)
        بیرون_fds[بیرون_fds_len++] = 1;
    }

  epollfd = epoll_helper (درون_fds, عدم, بیرون_fds, عدم, err);
  إذا (UNLIKELY (epollfd < 0))
    أرجع epollfd;

  طالما (1)
    {
      بنية epoll_event events[MAX_EVENTS];
      بنية signalfd_siginfo si;
      بنية winsize ws;
      صحيح i, nr_events;
      ssize_t res;

      nr_events = موقت_FAILURE_RETRY (epoll_wait (epollfd, events, MAX_EVENTS, -1));
      إذا (UNLIKELY (nr_events < 0))
        أرجع crun_make_error (err, errno, "epoll_wait");

      لكل (i = 0; i < nr_events; i++)
        {
          إذا (events[i].داده.fd == 0 || events[i].داده.fd == terminal_fd_مقصد)
            {
              ret = channel_fd_pair_process (مقصد_terminal, epollfd, err);
              إذا (UNLIKELY (ret < 0))
                أرجع crun_error_wrap (err, "copy to terminal fd");
            }
          وإلا إذا (events[i].داده.fd == 1 || events[i].داده.fd == terminal_fd_from)
            {
              ret = channel_fd_pair_process (from_terminal, epollfd, err);
              إذا (UNLIKELY (ret < 0))
                أرجع crun_error_wrap (err, "copy from terminal fd");
            }
          وإلا إذا (events[i].داده.fd == آرگومانها->seccomp_notify_fd)
            {
              ret = libcrun_seccomp_notify_plugins (seccomp_notify_ctx,
                                                    آرگومانها->seccomp_notify_fd, err);
              إذا (UNLIKELY (ret < 0))
                أرجع ret;
            }
          وإلا إذا (events[i].داده.fd == آرگومانها->notify_socket)
            {
              ret = handle_notify_socket (آرگومانها->notify_socket, err);
              إذا (UNLIKELY (ret < 0))
                أرجع ret;
              إذا (ret && آرگومانها->context->detach)
                أرجع 0;
            }
          وإلا إذا (events[i].داده.fd == signalfd)
            {
              res = موقت_FAILURE_RETRY (اقرأ (signalfd, &si, حجم (si)));
              إذا (UNLIKELY (res < 0))
                أرجع crun_make_error (err, errno, "read from signalfd");
              إذا (si.ssi_signo == SIGCHLD)
                {
                  ret = reap_subprocesses (&آرگومانها->pid, &container_exit_کد,
                                           &آخرین_process, err);
                  إذا (UNLIKELY (ret < 0))
                    أرجع ret;
                  إذا (آخرین_process)
                    أرجع container_exit_کد;
                }
              وإلا إذا (si.ssi_signo == SIGWINCH)
                {
                  /* Ignore the signal if the terminal is not available.  */
                  إذا (آرگومانها->terminal_fd > 0)
                    {
                      ret = ioctl (0, TIOCGWINSZ, &ws);
                      إذا (UNLIKELY (ret < 0))
                        أرجع crun_make_error (err, errno, "ioctl TIOCGWINSZ copy terminal size from stdin");

                      ret = ioctl (آرگومانها->terminal_fd, TIOCSWINSZ, &ws);
                      إذا (UNLIKELY (ret < 0))
                        أرجع crun_make_error (err, errno, "ioctl TIOCSWINSZ copy terminal size to pty");
                    }
                }
              وإلا إذا (آرگومانها->pid > 0)
                {
                  /* Send any other signal to the child process, if it was not reaped yet.  */
                  ret = kill (آرگومانها->pid, si.ssi_signo);
                }
            }
          وإلا
            {
              أرجع crun_make_error (err, 0, "internal error: unknown fd from epoll_wait");
            }
        }
    }

  أرجع 0;
}

ساكن فراغ
flush_fd_مقصد_err (libcrun_context_t *context, صحيح terminal_fd)
{
  حرف buf[256];
  صحيح flags;

  إذا (terminal_fd < 0 || stderr == عدم)
    أرجع;

  flags = fcntl (terminal_fd, F_GETFL, 0);
  إذا (flags == -1)
    أرجع;
  إذا (fcntl (terminal_fd, F_SETFL, flags | O_NONBLOCK) < 0)
    أرجع;

  لكل (;;)
    {
      صحيح ret = موقت_FAILURE_RETRY (اقرأ (terminal_fd, buf, حجم (buf) - 1));
      إذا (ret <= 0)
        اكسر;
      buf[ret] = '\0';
      إذا (context->output_handler)
        context->output_handler (0, buf, LIBCRUN_VERBOSITY_ERROR, context->output_handler_arg);
    }
  (فراغ) fcntl (terminal_fd, F_SETFL, flags);
  fflush (stderr);
  fsync (1);
  fsync (2);
}

ساكن صحيح
cleanup_watch (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
               بنية libcrun_cgroup_وضعیت *cgroup_وضعیت, pid_t init_pid, صحيح sync_socket,
               صحيح terminal_fd, libcrun_error_t *err)
{
  ثابت حرف *oom_پیام = عدم;
  libcrun_error_t tmp_err = عدم;
  صحيح ret;

  إذا (init_pid)
    {
      /* Try to detect whether the cgroup has a OOM.  */
      إذا (cgroup_وضعیت)
        {
          صحيح has_oom;

          has_oom = libcrun_cgroup_has_oom (cgroup_وضعیت, &tmp_err);
          إذا (has_oom > 0)
            oom_پیام = "OOM: the memory limit could be too low";
          وإلا إذا (has_oom < 0)
            {
              /* If the detection has failed for any reason, e.g. the cgroup was
                 already deleted by the time it was checked, just ignore the
                 failure.  */
              crun_error_release (&tmp_err);
            }
        }
      /* If the OOM wasn't detected, look into the static configuration.  */
      إذا (oom_پیام == عدم && is_حافظه_حد_too_کم (def))
        oom_پیام = "the memory limit could be too low";

      kill (init_pid, SIGKILL);
      waitpid_ignore_stopped (init_pid, عدم, 0);
    }

  ret = sync_socket_wait_sync (context, sync_socket, true, &tmp_err);
  إذا (UNLIKELY (ret < 0))
    {
      crun_error_release (err);
      *err = tmp_err;
    }

  إذا (terminal_fd >= 0)
    flush_fd_مقصد_err (context, terminal_fd);

  إذا (oom_پیام)
    أرجع crun_error_wrap (err, "%s", oom_پیام);

  أرجع -1;
}

/* Find the uid:gid that is mapped to root inside the container user namespace.  */
فراغ
get_ریشه_درون_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid, uid_t *uid,
                        gid_t *gid)
{
  *uid = -1;
  *gid = -1;

  /* Not root in the namespace.  */
  إذا (host_uid)
    أرجع;

  إذا (def->linux && def->linux->uid_mappings)
    {
      size_t i;

      لكل (i = 0; i < def->linux->uid_mappings_len; i++)
        إذا (def->linux->uid_mappings[i]->container_id == 0)
          {
            *uid = def->linux->uid_mappings[i]->host_id;
            اكسر;
          }
    }
  إذا (def->linux && def->linux->gid_mappings)
    {
      size_t i;

      لكل (i = 0; i < def->linux->gid_mappings_len; i++)
        إذا (def->linux->gid_mappings[i]->container_id == 0)
          {
            *gid = def->linux->gid_mappings[i]->host_id;
            اكسر;
          }
    }

  /* If the uid and the gid are not changed, do not attempt any chown.  */
  إذا (*uid == host_uid && *gid == host_gid)
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

  tmp = پیدا_کن_annotation (container, "run.oci.seccomp.plugins");
  إذا (tmp)
    {
      صحيح fds[2];
      صحيح ret;

      ret = create_socket_pair (fds, err);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_error_wrap (err, "create socket pair");

      *fd = fds[0];
      *self_receiver_fd = fds[1];
      *plugins = tmp;
      أرجع 0;
    }

  إذا (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_مسیر)
    tmp = def->linux->seccomp->listener_مسیر;
  وإلا
    tmp = پیدا_کن_annotation (container, "run.oci.seccomp.receiver");
  إذا (tmp == عدم)
    tmp = getenv ("RUN_OCI_SECCOMP_RECEIVER");
  إذا (tmp)
    {
      إذا (tmp[0] != '/')
        أرجع crun_make_error (err, 0, "the seccomp receiver `%s` is not an absolute path", tmp);

      *fd = open_unix_domain_client_socket (tmp, 0, err);
      إذا (UNLIKELY (*fd < 0))
        أرجع crun_error_wrap (err, "open seccomp receiver");
    }

  أرجع 0;
}

ساكن bool
has_seccomp_receiver (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;

  إذا (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_مسیر)
    أرجع true;

  إذا (پیدا_کن_annotation (container, "run.oci.seccomp.receiver") != عدم || getenv ("RUN_OCI_SECCOMP_RECEIVER") != عدم)
    أرجع true;

  أرجع false;
}

ساكن صحيح
راهاندازی_container_hooks_output (libcrun_container_t *container, runtime_spec_schema_config_schema *def,
                              بنية container_entrypoint_s *container_آرگومانها, صحيح *hooks_بیرون_fd,
                              صحيح *hooks_err_fd, libcrun_error_t *err)
{
  صحيح ret;

  إذا (def->hooks
      && (def->hooks->prestart_len || def->hooks->poststart_len || def->hooks->create_runtime_len
          || def->hooks->create_container_len || def->hooks->شروع_container_len))
    {
      ret = open_hooks_output (container, hooks_بیرون_fd, hooks_err_fd, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      container_آرگومانها->hooks_بیرون_fd = *hooks_بیرون_fd;
      container_آرگومانها->hooks_err_fd = *hooks_err_fd;
    }
  أرجع 0;
}

ساكن صحيح
راهاندازی_container_keyring (libcrun_container_t *container, libcrun_context_t *context, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح ret;

  إذا (! context->no_new_keyring)
    {
      ثابت حرف *label = عدم;

      libcrun_debug ("Creating new keyring");

      إذا (def->process)
        {
          label = def->process->selinux_label;
          إذا (label)
            libcrun_debug ("Using SELinux process label: `%s`", label);
        }

      ret = libcrun_create_keyring (container, container->context->id, label, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  أرجع 0;
}

ساكن صحيح
راهاندازی_terminal_socket_pair (libcrun_container_t *container, libcrun_context_t *context,
                            بنية container_entrypoint_s *container_آرگومانها, صحيح *socket_pair_0,
                            صحيح *socket_pair_1, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح ret;
  صحيح detach = context->detach;

  إذا (def->process && def->process->terminal && ! detach && context->console_socket == عدم)
    {
      libcrun_debug ("Creating terminal socket pair");
      container_آرگومانها->has_terminal_socket_pair = 1;
      ret = create_socket_pair (container_آرگومانها->terminal_socketpair, err);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_error_wrap (err, "create terminal socket");

      *socket_pair_0 = container_آرگومانها->terminal_socketpair[0];
      *socket_pair_1 = container_آرگومانها->terminal_socketpair[1];
    }
  أرجع 0;
}

ساكن صحيح
راهاندازی_seccomp (libcrun_container_t *container, ثابت حرف *seccomp_bpf_داده,
               بنية libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, صحيح *seccomp_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح ret;

  إذا (پیدا_کن_annotation (container, "run.oci.seccomp.plugins") != عدم && has_seccomp_receiver (container))
    {
      أرجع crun_make_error (err, 0, "seccomp plugins and seccomp receivers cannot be declared at the same time");
    }

  إذا (def->linux && (def->linux->seccomp || seccomp_bpf_داده))
    {
      بیعلامت صحيح seccomp_gen_options = 0;
      ثابت حرف *annotation;

      libcrun_debug ("Initializing seccomp");
      annotation = پیدا_کن_annotation (container, "run.oci.seccomp_fail_unknown_syscall");
      إذا (annotation && strcmp (annotation, "0") != 0)
        seccomp_gen_options = LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL;

      إذا (seccomp_bpf_داده)
        seccomp_gen_options |= LIBCRUN_SECCOMP_SKIP_CACHE;

      libcrun_seccomp_gen_ctx_init (seccomp_gen_ctx, container, true, seccomp_gen_options);

      ret = libcrun_open_seccomp_bpf (seccomp_gen_ctx, seccomp_fd, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  أرجع 0;
}

ساكن صحيح
راهاندازی_console_socket (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
                      بنية container_entrypoint_s *container_آرگومانها, صحيح *console_socket_fd,
                      libcrun_error_t *err)
{
  إذا (context->console_socket && def->process && def->process->terminal)
    {
      *console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
      إذا (UNLIKELY (*console_socket_fd < 0))
        أرجع crun_error_wrap (err, "open console socket");
      container_آرگومانها->console_socket_fd = *console_socket_fd;
    }
  أرجع 0;
}

ساكن صحيح
راهاندازی_cgroup_manager (libcrun_context_t *context, libcrun_container_t *container,
                      بنية libcrun_cgroup_آرگومانها *cg, صحيح *cgroup_dirfd,
                      بنية libcrun_dirfd_s *cgroup_dirfd_s, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح cgroup_manager;
  uid_t ریشه_uid = -1;
  gid_t ریشه_gid = -1;
  صحيح ret;

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  إذا (context->systemd_cgroup)
    {
      libcrun_debug ("Using systemd cgroup manager");
      cgroup_manager = CGROUP_MANAGER_SYSTEMD;
    }
  وإلا إذا (context->force_no_cgroup)
    {
      libcrun_debug ("Disabling cgroup manager");
      cgroup_manager = CGROUP_MANAGER_DISABLED;
    }
  وإلا
    libcrun_debug ("Using cgroupfs cgroup manager");

  /* If we are root (either on the host or in a namespace), then chown the cgroup to root
     in the container user namespace.  */
  get_ریشه_درون_the_userns (def, container->host_uid, container->host_gid, &ریشه_uid, &ریشه_gid);
  libcrun_debug ("Using container host UID `%d` and GID `%d`", container->host_uid, container->host_gid);

  memset (cg, 0, حجم (*cg));

  cg->cgroup_مسیر = def->linux ? def->linux->cgroups_مسیر : "";
  cg->manager = cgroup_manager;
  cg->id = context->id;
  cg->resources = def->linux ? def->linux->resources : عدم;
  cg->annotations = container->annotations;
  cg->ریشه_uid = ریشه_uid;
  cg->ریشه_gid = ریشه_gid;
  cg->state_ریشه = context->state_ریشه;
  cg->container = container;

  ret = libcrun_cgroup_preenter (cg, cgroup_dirfd, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  cgroup_dirfd_s->dirfd = cgroup_dirfd;
  cgroup_dirfd_s->joined = false;

  أرجع 0;
}

ساكن صحيح
مجموعه_scheduler (pid_t pid, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  صحيح ret;

  ret = libcrun_مجموعه_scheduler (pid, def->process, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_reset_cpu_affinity_ماسک (pid, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_مجموعه_io_اولویت (pid, def->process, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع 0;
}

ساكن صحيح
seccomp_generation (صحيح seccomp_fd, ثابت حرف *seccomp_bpf_داده,
                    بنية libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, libcrun_error_t *err)
{
  صحيح ret;

  إذا (seccomp_fd >= 0)
    {
      إذا (seccomp_bpf_داده != عدم)
        {
          ret = libcrun_copy_seccomp (seccomp_gen_ctx, seccomp_bpf_داده, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
      وإلا
        {
          ret = libcrun_generate_seccomp (seccomp_gen_ctx, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
    }
  أرجع 0;
}

ساكن صحيح
terminal_راهاندازی (runtime_spec_schema_config_schema *def, libcrun_context_t *context,
                صحيح socket_pair_0, صحيح *terminal_fd, فراغ **orig_terminal, libcrun_error_t *err)
{
  صحيح ret;
  صحيح detach = context->detach;

  إذا (def->process && def->process->terminal && ! detach && context->console_socket == عدم)
    {
      libcrun_debug ("Receiving console socket fd");
      *terminal_fd = receive_fd_from_socket (socket_pair_0, err);
      إذا (UNLIKELY (*terminal_fd < 0))
        أرجع -1;

      ret = libcrun_clear_onlcr (*terminal_fd, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = libcrun_مجموعه_raw (0, orig_terminal, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  أرجع 0;
}

ساكن صحيح
libcrun_container_پیاپی_داخلی (libcrun_container_t *container, libcrun_context_t *context,
                                صحيح *container_ready_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح ret;
  pid_t pid;
  صحيح detach = context->detach;
  cleanup_cgroup_وضعیت بنية libcrun_cgroup_وضعیت *cgroup_وضعیت = عدم;
  cleanup_close صحيح terminal_fd = -1;
  cleanup_terminal فراغ *orig_terminal = عدم;
  cleanup_close صحيح sync_socket = -1;
  cleanup_close صحيح notify_socket = -1;
  cleanup_close صحيح socket_pair_0 = -1;
  cleanup_close صحيح socket_pair_1 = -1;
  cleanup_close صحيح seccomp_fd = -1;
  cleanup_close صحيح console_socket_fd = -1;
  cleanup_close صحيح hooks_بیرون_fd = -1;
  cleanup_close صحيح hooks_err_fd = -1;
  cleanup_close صحيح own_seccomp_receiver_fd = -1;
  cleanup_close صحيح seccomp_notify_fd = -1;
  ثابت حرف *seccomp_notify_plugins = عدم;
  cleanup_cgroup_preenter بنية libcrun_cgroup_آرگومانها cg = {};
  cleanup_free حرف *bundle_allocated = عدم;
  بنية container_entrypoint_s container_آرگومانها = {
    .container = container,
    .context = context,
    .bundle = get_bundle_مسیر (context->bundle, &bundle_allocated),
    .terminal_socketpair = { -1, -1 },
    .console_socket_fd = -1,
    .hooks_بیرون_fd = -1,
    .hooks_err_fd = -1,
    .seccomp_receiver_fd = -1,
    .custom_handler = عدم,
  };
  cleanup_close صحيح cgroup_dirfd = -1;
  بنية libcrun_dirfd_s cgroup_dirfd_s;
  بنية libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  ثابت حرف *seccomp_bpf_داده = پیدا_کن_annotation (container, "run.oci.seccomp_bpf_data");
  صحيح cgroup_نما;

  إذا (! context->force_no_cgroup)
    {
      cgroup_نما = libcrun_get_cgroup_نما (err);
      إذا (UNLIKELY (cgroup_نما < 0))
        أرجع cgroup_نما;

      إذا (cgroup_نما != CGROUP_نما_UNIFIED)
        libcrun_warning ("cgroup v1 is deprecated and will be removed in a future release.  Use cgroup v2");
    }

  ret = راهاندازی_container_hooks_output (container, def, &container_آرگومانها, &hooks_بیرون_fd, &hooks_err_fd, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  container->context = context;

  إذا (! detach || context->notify_socket)
    {
      libcrun_debug ("Setting child subreaper");
      ret = prctl (PR_مجموعه_فرزند_SUBREAPER, 1, 0, 0, 0);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "prctl set child subreaper");
    }

  ret = راهاندازی_container_keyring (container, context, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = راهاندازی_terminal_socket_pair (container, context, &container_آرگومانها, &socket_pair_0, &socket_pair_1, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = بلوک_سیگنالها (err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  umask (0);

  ret = libcrun_مجموعه_mempolicy (def, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = راهاندازی_seccomp (container, seccomp_bpf_داده, &seccomp_gen_ctx, &seccomp_fd, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;
  container_آرگومانها.seccomp_fd = seccomp_fd;

  إذا (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &container_آرگومانها.seccomp_receiver_fd, &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = راهاندازی_console_socket (context, def, &container_آرگومانها, &console_socket_fd, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = راهاندازی_cgroup_manager (context, container, &cg, &cgroup_dirfd, &cgroup_dirfd_s, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_configure_handler (container_آرگومانها.context->handler_manager,
                                   container_آرگومانها.context,
                                   container,
                                   &(container_آرگومانها.custom_handler),
                                   err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (container_آرگومانها.custom_handler && container_آرگومانها.custom_handler->vtable->modify_oci_configuration)
    {
      libcrun_debug ("Using custom handler to modify OCI configuration");
      ret = container_آرگومانها.custom_handler->vtable->modify_oci_configuration (container_آرگومانها.custom_handler->cookie,
                                                                             container_آرگومانها.context,
                                                                             container->container_def,
                                                                             err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  pid = libcrun_پیاپی_linux_container (container, container_init, &container_آرگومانها, &sync_socket, &cgroup_dirfd_s, err);
  إذا (UNLIKELY (pid < 0))
    أرجع pid;

  cg.pid = pid;
  cg.joined = cgroup_dirfd_s.joined;
  libcrun_debug ("Running container on PID: `%d`", pid);

  إذا (context->fifo_exec_wait_fd < 0 && context->notify_socket)
    {
      libcrun_debug ("Using notify socket: `%s`", context->notify_socket);
      /* Do not open the notify socket here on "create".  "start" will take care of it.  */
      ret = get_notify_fd (context, container, &notify_socket, err);
      إذا (UNLIKELY (ret < 0))
        اذهب fail;
    }

  إذا (container_آرگومانها.terminal_socketpair[1] >= 0)
    close_and_reset (&socket_pair_1);

  ret = libcrun_cgroup_enter (&cg, &cgroup_وضعیت, err);
  إذا (UNLIKELY (ret < 0))
    اذهب fail;

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE, err);
  إذا (UNLIKELY (ret < 0))
    اذهب fail;

  ret = libcrun_move_شبکه_devices (container, pid, err);
  إذا (UNLIKELY (ret < 0))
    اذهب fail;

  /* sync send own pid.  */
  ret = موقت_FAILURE_RETRY (اكتب (sync_socket, &pid, حجم (pid)));
  إذا (UNLIKELY (ret != حجم (pid)))
    {
      إذا (ret >= 0)
        errno = 0;
      crun_make_error (err, errno, "write to sync socket");
      اذهب fail;
    }

  /* sync 1.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  إذا (UNLIKELY (ret < 0))
    اذهب fail;

  /* sync 2.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  إذا (UNLIKELY (ret < 0))
    اذهب fail;

  ret = libcrun_cgroup_enter_finalize (&cg, cgroup_وضعیت, err);
  إذا (UNLIKELY (ret < 0))
    اذهب fail;

  /* If the container process is created later, the scheduler is set on it then.  */
  إذا (! libcrun_container_delayed_pidns_fork (container))
    {
      ret = مجموعه_scheduler (pid, def, err);
      إذا (UNLIKELY (ret < 0))
        اذهب fail;
    }

  /* The container is waiting that we write back.  In this phase we can launch the
     prestart hooks.  */
  إذا (def->hooks && def->hooks->prestart_len)
    {
      libcrun_debug ("Running `prestart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "prestart", (hook **) def->hooks->prestart,
                      def->hooks->prestart_len, hooks_بیرون_fd, hooks_err_fd, false, err);
      إذا (UNLIKELY (ret != 0))
        اذهب fail;
    }
  إذا (def->hooks && def->hooks->create_runtime_len)
    {
      libcrun_debug ("Running `create` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "createRuntime", (hook **) def->hooks->create_runtime,
                      def->hooks->create_runtime_len, hooks_بیرون_fd, hooks_err_fd, false, err);
      إذا (UNLIKELY (ret != 0))
        اذهب fail;
    }

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_داده, &seccomp_gen_ctx, err);
  إذا (UNLIKELY (ret < 0))
    اذهب fail;
  close_and_reset (&seccomp_fd);

  /* sync 3.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  إذا (UNLIKELY (ret < 0))
    اذهب fail;

  /* The process joining an existing PID namespace is created once the container root is in place.  */
  إذا (libcrun_container_delayed_pidns_fork (container))
    {
      pid_t new_pid = 0;

      ret = sync_socket_wait_sync (context, sync_socket, false, err);
      إذا (UNLIKELY (ret < 0))
        اذهب fail;

      ret = موقت_FAILURE_RETRY (اقرأ (sync_socket, &new_pid, حجم (new_pid)));
      إذا (UNLIKELY (ret != حجم (new_pid)))
        {
          إذا (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "read pid from sync socket");
          اذهب fail;
        }

      /* Cleanup the first process.  */
      waitpid_ignore_stopped (pid, عدم, 0);

      pid = new_pid;

      ret = مجموعه_scheduler (pid, def, err);
      إذا (UNLIKELY (ret < 0))
        اذهب fail;

      ret = libcrun_handle_pidfd_receiver (pid, container, err);
      إذا (UNLIKELY (ret < 0))
        اذهب fail;

      /* sync send own pid.  */
      ret = موقت_FAILURE_RETRY (اكتب (sync_socket, &pid, حجم (pid)));
      إذا (UNLIKELY (ret != حجم (pid)))
        {
          إذا (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "write to sync socket");
          اذهب fail;
        }
    }

  ret = terminal_راهاندازی (def, context, socket_pair_0, &terminal_fd, &orig_terminal, err);
  إذا (UNLIKELY (ret < 0))
    اذهب fail;

  close_and_reset (&socket_pair_0);

  /* sync 4.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  إذا (UNLIKELY (ret < 0))
    اذهب fail;

  ret = close_and_reset (&sync_socket);
  إذا (UNLIKELY (ret < 0))
    {
      crun_make_error (err, errno, "close sync_socket failed");
      اذهب fail;
    }

  libcrun_debug ("Writing container status");
  ret = write_container_وضعیت (container, context, pid, cgroup_وضعیت, err);
  إذا (UNLIKELY (ret < 0))
    اذهب fail;

  /* Run poststart hooks here only if the container is created using "run".  For create+start, the
     hooks will be executed as part of the start command.  */
  إذا (context->fifo_exec_wait_fd < 0 && def->hooks && def->hooks->poststart_len)
    {
      libcrun_debug ("Running `poststart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_بیرون_fd, hooks_err_fd, false, err);
      إذا (UNLIKELY (ret != 0))
        اذهب fail;
    }

  /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
  إذا (own_seccomp_receiver_fd >= 0)
    {
      libcrun_debug ("Receiving seccomp fd");
      seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
      إذا (UNLIKELY (seccomp_notify_fd < 0))
        اذهب fail;

      ret = close_and_reset (&own_seccomp_receiver_fd);
      إذا (UNLIKELY (ret < 0))
        {
          crun_make_error (err, errno, "close seccomp receiver fd failed");
          اذهب fail;
        }
    }

  {
    بنية wait_for_process_آرگومانها آرگومانها = {
      .pid = pid,
      .context = context,
      .terminal_fd = terminal_fd,
      .notify_socket = notify_socket,
      .container_ready_fd = container_ready_fd,
      .seccomp_notify_fd = seccomp_notify_fd,
      .seccomp_notify_plugins = seccomp_notify_plugins,
    };
    ret = wait_for_process (&آرگومانها, err);
  }
  إذا (! context->detach)
    {
      libcrun_error_t tmp_err = عدم;
      cleanup_watch (context, def, cgroup_وضعیت, 0, sync_socket, terminal_fd, &tmp_err);
      crun_error_release (&tmp_err);
    }

  أرجع ret;

fail:
  ret = cleanup_watch (context, def, cgroup_وضعیت, pid, sync_socket, terminal_fd, err);
  إذا (cgroup_وضعیت)
    {
      libcrun_error_t tmp_err = عدم;
      libcrun_cgroup_destroy (cgroup_وضعیت, &tmp_err);
      crun_error_release (&tmp_err);
    }
  أرجع ret;
}

ساكن صحيح
check_config_file (runtime_spec_schema_config_schema *def, libcrun_context_t *context, libcrun_error_t *err)
{
  إذا (UNLIKELY (def->linux == عدم))
    أرجع crun_make_error (err, 0, "invalid config file, no `linux` block specified");

  إذا (context->handler == عدم)
    {
      إذا (UNLIKELY (def->ریشه == عدم))
        أرجع crun_make_error (err, 0, "invalid config file, no `root` block specified");
      إذا (UNLIKELY (def->mounts == عدم))
        أرجع crun_make_error (err, 0, "invalid config file, no `mounts` block specified");
    }
  أرجع 0;
}

ساكن صحيح
libcrun_copy_config_file (ثابت حرف *id, ثابت حرف *state_ریشه, libcrun_container_t *container, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_free حرف *dest_مسیر = عدم;
  cleanup_free حرف *dir = عدم;
  cleanup_free حرف *میانگیر = عدم;
  size_t len;

  ret = libcrun_get_state_directory (&dir, state_ریشه, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = append_paths (&dest_مسیر, err, dir, "config.json", عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (container->config_file == عدم && container->config_file_content == عدم)
    أرجع crun_make_error (err, 0, "config file not specified");

  إذا (container->config_file == عدم)
    {
      libcrun_debug ("Writing config file to: `%s`", dest_مسیر);
      ret = write_file (dest_مسیر, container->config_file_content, طول_رشته (container->config_file_content), err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  وإلا
    {
      libcrun_debug ("Reading config file: `%s`", container->config_file);
      ret = read_all_file (container->config_file, &میانگیر, &len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      libcrun_debug ("Writing config file to: `%s`", dest_مسیر);
      ret = write_file (dest_مسیر, میانگیر, len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  أرجع 0;
}

ساكن فراغ
force_delete_container_وضعیت (libcrun_context_t *context, libcrun_container_t *container,
                               runtime_spec_schema_config_schema *def)
{
  libcrun_error_t tmp_err = عدم;
  container_delete_داخلی (context, container, def, context->id, true, false, &tmp_err);
  libcrun_error_report_and_release (&tmp_err);
}

/* Serialize ERR to FD as an int errno followed by a NUL-terminated message.
   Used by forked children to report a failure to the parent process.  */
ساكن فراغ
write_error_مقصد_pipe (صحيح fd, libcrun_error_t *err)
{
  ثابت حرف *پیام = (*err)->پیام;
  صحيح errcode = crun_error_get_errno (err);

  موقت_FAILURE_RETRY (اكتب (fd, &errcode, حجم (errcode)));
  موقت_FAILURE_RETRY (اكتب (fd, پیام, طول_رشته (پیام) + 1));
}

صحيح
libcrun_container_پیاپی (libcrun_context_t *context, libcrun_container_t *container, بیعلامت صحيح options,
                       libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح ret;
  صحيح detach = context->detach;
  صحيح container_ret_وضعیت[2];
  cleanup_close صحيح pipefd0 = -1;
  cleanup_close صحيح pipefd1 = -1;
  libcrun_error_t tmp_err = عدم;

  container->context = context;

  ret = validate_options (options, LIBCRUN_پیاپی_OPTIONS_PREFORK | LIBCRUN_پیاپی_OPTIONS_KEEP, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = check_config_file (def, context, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (def->process && def->process->terminal && detach && context->console_socket == عدم)
    أرجع crun_make_error (err, 0, "use --console-socket with --detach when a terminal is used");

  ret = libcrun_وضعیت_check_directories (context->state_ریشه, context->id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (! detach && (options & LIBCRUN_پیاپی_OPTIONS_PREFORK) == 0)
    {
      ret = libcrun_copy_config_file (context->id, context->state_ریشه, container, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = libcrun_container_پیاپی_داخلی (container, context, عدم, err);
      إذا (! (options & LIBCRUN_پیاپی_OPTIONS_KEEP))
        force_delete_container_وضعیت (context, container, def);
      أرجع ret;
    }

  ret = pipe2 (container_ret_وضعیت, O_CLOEXEC);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_وضعیت[0];
  pipefd1 = container_ret_وضعیت[1];

  ret = fork ();
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "fork");
  إذا (ret)
    {
      صحيح وضعیت;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, عدم, 0);

      ret = موقت_FAILURE_RETRY (اقرأ (pipefd0, &وضعیت, حجم (وضعیت)));
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "read from sync pipe");

      إذا (وضعیت < 0)
        {
          صحيح errno_;
          حرف buf[512];
          ret = موقت_FAILURE_RETRY (اقرأ (pipefd0, &errno_, حجم (errno_)));
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "read from sync pipe");

          ret = موقت_FAILURE_RETRY (اقرأ (pipefd0, buf, حجم (buf) - 1));
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "read from sync pipe");
          buf[ret] = '\0';

          أرجع crun_make_error (err, errno_, "%s", buf);
        }

      أرجع وضعیت;
    }

  close_and_reset (&pipefd0);

  /* forked process.  */
  ret = detach_process ();
  إذا (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_ریشه, container, &tmp_err);
  إذا (UNLIKELY (ret < 0))
    اذهب fail;

  ret = libcrun_container_پیاپی_داخلی (container, context, عدم, &tmp_err);
  موقت_FAILURE_RETRY (اكتب (pipefd1, &ret, حجم (ret)));
  إذا (UNLIKELY (ret < 0))
    اذهب fail;

  خروج (EXIT_SUCCESS);
fail:

  إذا (! (options & LIBCRUN_پیاپی_OPTIONS_KEEP))
    force_delete_container_وضعیت (context, container, def);
  إذا (tmp_err)
    {
      write_error_مقصد_pipe (pipefd1, &tmp_err);
      crun_error_release (&tmp_err);
    }

  خروج (EXIT_FAILURE);
}

صحيح
libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container, بیعلامت صحيح options,
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
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = check_config_file (def, context, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (def->process && def->process->terminal && context->console_socket == عدم)
    أرجع crun_make_error (err, 0, "use --console-socket with create when a terminal is used");

  ret = libcrun_وضعیت_check_directories (context->state_ریشه, context->id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  exec_fifo_fd = libcrun_وضعیت_create_exec_fifo (context->state_ریشه, context->id, err);
  إذا (UNLIKELY (exec_fifo_fd < 0))
    أرجع exec_fifo_fd;

  context->fifo_exec_wait_fd = exec_fifo_fd;
  exec_fifo_fd = -1;

  إذا ((options & LIBCRUN_پیاپی_OPTIONS_PREFORK) == 0)
    {
      libcrun_debug ("Running with prefork enabled");
      ret = libcrun_copy_config_file (context->id, context->state_ریشه, container, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      ret = libcrun_container_پیاپی_داخلی (container, context, عدم, err);
      إذا (UNLIKELY (ret < 0))
        force_delete_container_وضعیت (context, container, def);
      أرجع ret;
    }

  ret = pipe2 (container_ready_pipe, O_CLOEXEC);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "pipe");
  pipefd0 = container_ready_pipe[0];
  pipefd1 = container_ready_pipe[1];

  ret = fork ();
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "fork");
  إذا (ret)
    {
      صحيح exit_کد;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, عدم, 0);

      ret = موقت_FAILURE_RETRY (اقرأ (pipefd0, &exit_کد, حجم (exit_کد)));
      إذا (UNLIKELY (ret <= 0))
        أرجع crun_make_error (err, ret < 0 ? errno : 0, "waiting for container to be ready");

      إذا (ret != حجم (exit_کد) || exit_کد < 0)
        أرجع crun_make_error (err, 0, "internal error: read invalid exit code");

      إذا (exit_کد == 0)
        أرجع 0;

      libcrun_debug ("Exit code is `%d`, deleting container", exit_کد);
      libcrun_error_t tmp_err = عدم;
      container_delete_داخلی (context, عدم, def, context->id, true, true, &tmp_err);
      libcrun_error_report_and_release (&tmp_err);
      أرجع crun_make_error (err, 0, "error creating container");
    }

  /* forked process.  */
  ret = detach_process ();
  إذا (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_ریشه, container, err);
  إذا (UNLIKELY (ret < 0))
    {
      صحيح errcode = crun_error_get_errno (err);
      crun_error_release (err);
      libcrun_fail_with_error (errcode, "copy config file");
    }

  ret = libcrun_container_پیاپی_داخلی (container, context, &pipefd1, err);
  إذا (UNLIKELY (ret < 0))
    {
      force_delete_container_وضعیت (context, container, def);
      libcrun_error ((*err)->وضعیت, "%s", (*err)->پیام);
      crun_error_release (err);
      crun_مجموعه_output_handler (log_write_مقصد_stderr, عدم);
    }

  إذا (pipefd1 >= 0)
    موقت_FAILURE_RETRY (اكتب (pipefd1, &ret, حجم (ret)));
  خروج (ret ? EXIT_FAILURE : 0);
}

صحيح
libcrun_container_شروع (libcrun_context_t *context, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = عدم;
  ثابت حرف *state_ریشه = context->state_ریشه;
  runtime_spec_schema_config_schema *def;
  cleanup_container_وضعیت libcrun_container_وضعیت_t وضعیت = {};
  cleanup_close صحيح fd = -1;
  صحيح ret;

  ret = libcrun_read_container_وضعیت (&وضعیت, state_ریشه, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_is_container_running (&وضعیت, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (! ret)
    أرجع crun_make_error (err, 0, "container `%s` is not running", id);

  ret = read_container_config_from_state (&container, state_ریشه, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (context->notify_socket)
    {
      ret = get_notify_fd (context, container, &fd, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = libcrun_وضعیت_write_exec_fifo (context->state_ریشه, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  def = container->container_def;

  إذا (context->notify_socket)
    {
      إذا (fd >= 0)
        {
          fd_set read_مجموعه;

          طالما (1)
            {
              بنية timeval timeout = {
                .tv_sec = 0,
                .tv_usec = 10000,
              };
              FD_ZERO (&read_مجموعه);
              FD_SET (fd, &read_مجموعه);

              ret = select (fd + 1, &read_مجموعه, عدم, عدم, &timeout);
              إذا (UNLIKELY (ret < 0))
                أرجع crun_make_error (err, errno, "select failed");
              إذا (ret)
                {
                  ret = handle_notify_socket (fd, err);
                  إذا (UNLIKELY (ret < 0))
                    أرجع ret;
                  إذا (ret)
                    اكسر;
                }
              وإلا
                {
                  ret = libcrun_is_container_running (&وضعیت, err);
                  إذا (UNLIKELY (ret < 0))
                    أرجع ret;
                  إذا (! ret)
                    أرجع 0;
                }
            }
        }
    }

  /* The container is considered running only after we got the notification from the
     notify_socket, if any.  */
  إذا (def->hooks && def->hooks->poststart_len)
    {
      cleanup_close صحيح hooks_بیرون_fd = -1;
      cleanup_close صحيح hooks_err_fd = -1;

      ret = open_hooks_output (container, &hooks_بیرون_fd, &hooks_err_fd, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = do_hooks (def, وضعیت.pid, context->id, false, وضعیت.bundle, وضعیت.bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_بیرون_fd, hooks_err_fd, false, err);
      إذا (UNLIKELY (ret != 0))
        {
          libcrun_error_t tmp_err = عدم;
          container_delete_داخلی (context, container, def, id, true, true, &tmp_err);
          libcrun_error_report_and_release (&tmp_err);
          أرجع ret;
        }
    }

  أرجع 0;
}

صحيح
libcrun_get_container_state_string (ثابت حرف *id, libcrun_container_وضعیت_t *وضعیت, ثابت حرف *state_ریشه,
                                    ثابت حرف **container_وضعیت, صحيح *running, libcrun_error_t *err)
{
  صحيح ret, has_fifo = 0;
  bool paused = false;

  ret = libcrun_is_container_running (وضعیت, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;
  *running = ret;

  إذا (*running)
    {
      ret = libcrun_وضعیت_has_read_exec_fifo (state_ریشه, id, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      has_fifo = ret;
    }

  إذا (*running && ! has_fifo)
    {
      cleanup_cgroup_وضعیت بنية libcrun_cgroup_وضعیت *cgroup_وضعیت = عدم;

      cgroup_وضعیت = libcrun_cgroup_make_وضعیت (وضعیت);

      ret = libcrun_cgroup_is_container_paused (cgroup_وضعیت, &paused, err);
      إذا (UNLIKELY (ret < 0))
        {
          /*
            The cgroup might have been cleaned up by the time we try to read it, ignore both
            ENOENT and ENODEV:
            - ENOENT: if the open(CGROUP_PATH) fails because the cgroup was deleted.
            - ENODEV: if the cgroup is deleted between the open and reading the freeze status.
          */

          errno = crun_error_get_errno (err);
          إذا (errno == ENOENT || errno == ENODEV)
            {
              crun_error_release (err);
              *container_وضعیت = "stopped";
              أرجع 0;
            }

          أرجع ret;
        }
    }

  إذا (! *running)
    *container_وضعیت = "stopped";
  وإلا إذا (has_fifo)
    *container_وضعیت = "created";
  وإلا إذا (paused)
    *container_وضعیت = "paused";
  وإلا
    *container_وضعیت = "running";

  أرجع 0;
}

صحيح
libcrun_container_state_json (libcrun_context_t *context, ثابت حرف *id, حرف **بیرون, libcrun_error_t *err)
{
  حرف *میانگیر = عدم;
  size_t len = 0;
  FILE *f;
  صحيح ret;

  f = open_memstream (&میانگیر, &len);
  إذا (f == عدم)
    أرجع crun_make_error (err, errno, "open_memstream");

  ret = libcrun_container_state (context, id, f, err);
  fclose (f);
  إذا (UNLIKELY (ret < 0))
    {
      حرر (میانگیر);
      أرجع ret;
    }

  *بیرون = میانگیر;
  أرجع 0;
}

صحيح
libcrun_container_state (libcrun_context_t *context, ثابت حرف *id, FILE *بیرون, libcrun_error_t *err)
{
  ثابت حرف *ثابت OCI_CONFIG_نسخه = "1.0.0";
  libcrun_container_وضعیت_t وضعیت = {};
  ثابت حرف *state_ریشه = context->state_ریشه;
  ثابت حرف *container_وضعیت = عدم;
  json_gen_ctx *gen = عدم;
  ثابت حرف *buf;
  صحيح ret = 0;
  صحيح r = json_gen_وضعیت_ok;
  صحيح running;
  size_t len;

  ret = libcrun_read_container_وضعیت (&وضعیت, state_ریشه, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_get_container_state_string (id, &وضعیت, state_ریشه, &container_وضعیت, &running, err);
  إذا (UNLIKELY (ret < 0))
    اذهب خروج;

  ret = 0;
  إذا (! json_gen_init (&gen, عدم))
    أرجع crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));
  GEN_کلید (gen, "ociVersion");
  GEN_STR (gen, OCI_CONFIG_نسخه);

  GEN_کلید (gen, "id");
  GEN_STR (gen, id);

  GEN_کلید (gen, "pid");
  GEN_OR_FAIL (map_int (gen, running ? وضعیت.pid : 0));

  GEN_کلید (gen, "status");
  GEN_STR (gen, container_وضعیت);

  GEN_کلید (gen, "bundle");
  GEN_STR (gen, وضعیت.bundle);

  GEN_کلید (gen, "rootfs");
  GEN_STR (gen, وضعیت.rootfs);

  GEN_کلید (gen, "created");
  GEN_STR (gen, وضعیت.created);

  إذا (وضعیت.scope)
    {
      GEN_کلید (gen, "systemd-scope");
      GEN_STR (gen, وضعیت.scope);
    }

  إذا (وضعیت.owner)
    {
      GEN_کلید (gen, "owner");
      GEN_STR (gen, وضعیت.owner);
    }

  {
    cleanup_container libcrun_container_t *container = عدم;

    ret = read_container_config_from_state (&container, state_ریشه, id, err);
    إذا (UNLIKELY (ret < 0))
      اذهب خروج;

    إذا (container->container_def->annotations && container->container_def->annotations->len)
      GEN_OR_FAIL (gen_annotations (gen, container->container_def->annotations));
  }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  fprintf (بیرون, "%s\n", buf);

خروج:
  إذا (gen)
    json_gen_free (gen);
  libcrun_free_container_وضعیت (&وضعیت);
  أرجع ret;

gen_error:
  ret = json_gen_error_مقصد_crun_error (r, err);
  اذهب خروج;
}

صحيح
libcrun_container_exec (libcrun_context_t *context, ثابت حرف *id, runtime_spec_schema_config_schema_process *process,
                        libcrun_error_t *err)
{
  بنية libcrun_container_exec_options_s opts;
  memset (&opts, 0, حجم (opts));

  opts.struct_اندازه = حجم (opts);
  opts.process = process;

  أرجع libcrun_container_exec_with_options (context, id, &opts, err);
}

صحيح
libcrun_container_exec_process_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *مسیر, libcrun_error_t *err)
{
  بنية libcrun_container_exec_options_s opts;
  memset (&opts, 0, حجم (opts));

  opts.struct_اندازه = حجم (opts);
  opts.مسیر = مسیر;

  أرجع libcrun_container_exec_with_options (context, id, &opts, err);
}

ساكن صحيح
exec_from_process_json (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *process_json, ثابت حرف *cgroup,
                        bool ادغام_env, libcrun_error_t *err)
{
  بنية parser_context parser_ctx = { .options = 0, .errfile = stderr };
  بنية libcrun_container_exec_options_s opts;
  runtime_spec_schema_config_schema_process *process;
  parser_error parser_err = عدم;
  json_object *doc = عدم;
  صحيح ret;

  ret = parse_json_file (&doc, process_json, &parser_ctx, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  process = make_runtime_spec_schema_config_schema_process (doc, &parser_ctx, &parser_err);
  json_object_put (doc);
  إذا (process == عدم)
    {
      ret = crun_make_error (err, 0, "cannot parse process: `%s`", parser_err ? parser_err : "unknown error");
      حرر (parser_err);
      أرجع ret;
    }

  memset (&opts, 0, حجم (opts));
  opts.struct_اندازه = حجم (opts);
  opts.process = process;
  opts.cgroup = cgroup;
  opts.ادغام_env = ادغام_env;

  ret = libcrun_container_exec_with_options (context, id, &opts, err);
  free_runtime_spec_schema_config_schema_process (process);
  أرجع ret;
}

صحيح
libcrun_container_exec_json (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *process_json, libcrun_error_t *err)
{
  أرجع exec_from_process_json (context, id, process_json, عدم, false, err);
}

صحيح
libcrun_container_exec_with_options_json (libcrun_context_t *context, ثابت حرف *id,
                                          ثابت بنية libcrun_exec_options_s *opts, libcrun_error_t *err)
{
  إذا (opts == عدم || opts->struct_اندازه == 0)
    أرجع crun_make_error (err, EINVAL, "invalid exec options");

  أرجع exec_from_process_json (context, id, opts->process_json, opts->cgroup, opts->ادغام_env, err);
}

#تعریف_کن cleanup_process_schema __attribute__ ((cleanup (cleanup_process_schemap)))

ساكن مضمن فراغ
cleanup_process_schemap (runtime_spec_schema_config_schema_process **p)
{
  runtime_spec_schema_config_schema_process *process = *p;
  إذا (process)
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

  إذا (seccomp_receiver_fd >= 0)
    {
      ret = get_seccomp_receiver_fd_payload (container, "running", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = libcrun_apply_seccomp (seccomp_fd, seccomp_receiver_fd, seccomp_fd_payload,
                               seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  close_and_reset (&seccomp_fd);
  close_and_reset (&seccomp_receiver_fd);
  أرجع 0;
}

ساكن صحيح
exec_process_entrypoint (libcrun_context_t *context,
                         libcrun_container_t *container,
                         runtime_spec_schema_config_schema_process *process,
                         صحيح *pipefd1,
                         صحيح seccomp_fd,
                         صحيح seccomp_receiver_fd,
                         بنية custom_handler_instance_s *custom_handler,
                         bool ادغام_env,
                         libcrun_error_t *err)
{
  runtime_spec_schema_config_schema_process_capabilities *capabilities = عدم;
  cleanup_free حرف *exec_مسیر = عدم;
  uid_t container_uid;
  gid_t container_gid;
  ثابت حرف *cwd;
  bool chdir_انجامشده = false;
  size_t seccomp_flags_len = 0;
  حرف **seccomp_flags = عدم;
  pid_t own_pid = 0;
  size_t i;
  صحيح ret;

  container_uid = process->user ? process->user->uid : 0;
  container_gid = process->user ? process->user->gid : 0;

  موقت_FAILURE_RETRY (اقرأ (*pipefd1, &own_pid, حجم (own_pid)));

  cwd = process->cwd ? process->cwd : "/";
  إذا (LIKELY (libcrun_امن_chdir (cwd, err) == 0))
    chdir_انجامشده = true;
  وإلا
    crun_error_release (err);

  ret = unblock_سیگنالها (err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = clearenv ();
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "clearenv");

  إذا ((ادغام_env || ! process->env_len) && container->container_def->process && container->container_def->process->env_len)
    {
      حرف *e;

      لكل (i = 0; i < container->container_def->process->env_len; i++)
        {
          e = container->container_def->process->env[i];
          إذا (putenv (e) < 0)
            أرجع crun_make_error (err, errno, "putenv `%s`", e);
        }
    }
  إذا (process->env_len)
    {
      لكل (i = 0; i < process->env_len; i++)
        إذا (putenv (process->env[i]) < 0)
          أرجع crun_make_error (err, errno, "putenv `%s`", process->env[i]);
    }

  إذا (is_خالی_string (getenv ("HOME")))
    {
      ret = مجموعه_home_env (container_uid);
      إذا (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          إذا (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            أرجع crun_make_error (err, errno, "setenv HOME");
        }
    }

  ret = libcrun_مجموعه_selinux_label (container, process, false, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_مجموعه_apparmor_profile (container, process, false, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (container->container_def->linux && container->container_def->linux->seccomp)
    {
      seccomp_flags = container->container_def->linux->seccomp->flags;
      seccomp_flags_len = container->container_def->linux->seccomp->flags_len;
    }

  ret = پیدا_کن_executable (&exec_مسیر, process->آرگومانها[0], process->cwd, err);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (crun_error_get_errno (err) == ENOENT)
        أرجع ret;

      /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
         once the process switched to the use that runs in the container.  This might be necessary
         when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
         is not honored.  */
      crun_error_release (err);
    }

  إذا (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_مجموعه_personality (container->container_def->linux->personality, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, false, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_reopen_dev_null (err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (! process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = libcrun_container_setgroups (container, process, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = maybe_chown_std_streams (container_uid, container_gid, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (process->capabilities)
    capabilities = process->capabilities;
  وإلا إذا (container->container_def->process)
    capabilities = container->container_def->process->capabilities;

  ret = libcrun_مجموعه_caps (capabilities, container_uid, container_gid, process->no_new_privileges,
                          custom_handler != عدم, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (UNLIKELY (exec_مسیر == عدم))
    {
      ret = پیدا_کن_executable (&exec_مسیر, process->آرگومانها[0], process->cwd, err);
      إذا (UNLIKELY (ret < 0))
        {
          إذا (custom_handler == عدم || is_خالی_string (process->آرگومانها[0]))
            أرجع ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          exec_مسیر = xstrdup (process->آرگومانها[0]);
        }
    }

  إذا (! chdir_انجامشده)
    {
      ret = libcrun_امن_chdir (cwd, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (process->user)
    umask (process->user->umask_present ? process->user->umask : 0022);

  ret = 0;
  موقت_FAILURE_RETRY (اكتب (*pipefd1, &ret, حجم (ret)));
  موقت_FAILURE_RETRY (close (*pipefd1));
  *pipefd1 = -1;

  إذا (custom_handler)
    {
      إذا (custom_handler->vtable->exec_func == عدم)
        أرجع crun_make_error (err, 0, "the handler does not support exec");

      إذا (context->argv)
        erase_initial_environment (context->argv, context->argc);

      ret = custom_handler->vtable->exec_func (custom_handler->cookie,
                                               container,
                                               exec_مسیر,
                                               process->آرگومانها);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, -ret, "exec container process failed with handler as `%s`", custom_handler->vtable->name);

      امن_exit (EXIT_FAILURE);
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation, because the seccomp filter is already in place and it could
     stop some syscalls used by mark_or_close_fds_ge_than.
  */
  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, true, err);
  إذا (UNLIKELY (ret < 0))
    crun_error_release (err);

  موقت_FAILURE_RETRY (execv (exec_مسیر, process->آرگومانها));
  libcrun_fail_with_error (errno, "exec `%s`", exec_مسیر);
}

/* Close the file descriptors inherited from the caller, other than those
   to be passed to the container process.  */
صحيح
libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err)
{
  أرجع mark_or_close_fds_ge_than (عدم, context->preserve_fds + 3, true, err);
}

صحيح
libcrun_بارگیری_process_from_file (ثابت حرف *مسیر, runtime_spec_schema_config_schema_process **process,
                                libcrun_error_t *err)
{
  بنية parser_context ctx = { 0, stderr };
  cleanup_free حرف *content = عدم;
  parser_error parser_err = عدم;
  json_object *doc = عدم;
  size_t len;
  صحيح ret;

  ret = read_all_file (مسیر, &content, &len, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  *process = make_runtime_spec_schema_config_schema_process (doc, &ctx, &parser_err);
  إذا (UNLIKELY (*process == عدم))
    {
      ret = crun_make_error (err, 0, "cannot parse process file: `%s`", parser_err);
      حرر (parser_err);
      إذا (doc)
        json_object_put (doc);
      أرجع ret;
    }

  حرر (parser_err);
  إذا (doc)
    json_object_put (doc);

  أرجع 0;
}

صحيح
libcrun_container_exec_with_options (libcrun_context_t *context, ثابت حرف *id,
                                     بنية libcrun_container_exec_options_s *opts,
                                     libcrun_error_t *err)
{
  cleanup_custom_handler_instance بنية custom_handler_instance_s *custom_handler = عدم;
  صحيح container_وضعیت, ret;
  bool container_paused = false;
  pid_t pid;
  libcrun_container_وضعیت_t وضعیت = {};
  ثابت حرف *state_ریشه = context->state_ریشه;
  cleanup_close صحيح terminal_fd = -1;
  cleanup_close صحيح seccomp_fd = -1;
  cleanup_terminal فراغ *orig_terminal = عدم;
  cleanup_container libcrun_container_t *container = عدم;
  صحيح container_ret_وضعیت[2];
  cleanup_close صحيح pipefd0 = -1;
  cleanup_close صحيح pipefd1 = -1;
  cleanup_close صحيح seccomp_receiver_fd = -1;
  cleanup_close صحيح own_seccomp_receiver_fd = -1;
  cleanup_close صحيح seccomp_notify_fd = -1;
  ثابت حرف *seccomp_notify_plugins = عدم;
  __attribute__ ((استفادهنشده)) cleanup_process_schema runtime_spec_schema_config_schema_process *process_cleanup = عدم;
  runtime_spec_schema_config_schema_process *process = opts->process;
  بنية libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  صحيح ret_from_فرزند = 0;

  ret = libcrun_read_container_وضعیت (&وضعیت, state_ریشه, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_is_container_running (&وضعیت, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;
  container_وضعیت = ret;

  ret = read_container_config_from_state (&container, state_ریشه, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  container->context = context;

  إذا (container_وضعیت == 0)
    أرجع crun_make_error (err, 0, "the container `%s` is not running", id);

  {
    cleanup_cgroup_وضعیت بنية libcrun_cgroup_وضعیت *cgroup_وضعیت = عدم;

    cgroup_وضعیت = libcrun_cgroup_make_وضعیت (&وضعیت);

    ret = libcrun_cgroup_is_container_paused (cgroup_وضعیت, &container_paused, err);
    إذا (UNLIKELY (ret < 0))
      أرجع ret;
  }

  إذا (UNLIKELY (container_paused))
    أرجع crun_make_error (err, 0, "cannot exec in a paused container `%s`", id);

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = بلوک_سیگنالها (err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  libcrun_seccomp_gen_ctx_init (&seccomp_gen_ctx, container, false, 0);

  ret = libcrun_open_seccomp_bpf (&seccomp_gen_ctx, &seccomp_fd, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &seccomp_receiver_fd,
                                     &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (حجم (*opts) != opts->struct_اندازه)
    أرجع crun_make_error (err, EINVAL, "invalid libcrun_container_exec_options_s struct");

  إذا (opts->مسیر)
    {
      إذا (process)
        أرجع crun_make_error (err, EINVAL, "cannot specify both exec file and options");

      ret = libcrun_بارگیری_process_from_file (opts->مسیر, &process, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      process_cleanup = process;
    }

  /* This must be done before we enter a user namespace.  If the process does
     not specify any rlimits, use the ones from the container configuration.  */
  إذا (process->rlimits_len == 0 && container->container_def->process)
    ret = libcrun_مجموعه_rlimits (container->container_def->process->rlimits,
                               container->container_def->process->rlimits_len, err);
  وإلا
    ret = libcrun_مجموعه_rlimits (process->rlimits, process->rlimits_len, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_مجموعه_mempolicy (container->container_def, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = pipe2 (container_ret_وضعیت, O_CLOEXEC);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_وضعیت[0];
  pipefd1 = container_ret_وضعیت[1];

  /* If the new process block doesn't specify a SELinux label, AppArmor profile, user,
     I/O priority or scheduler, then use the configuration from the original config file.  */
  إذا (container->container_def->process)
    {
      إذا (process->io_اولویت == عدم && container->container_def->process->io_اولویت)
        {
          process->io_اولویت = clone_runtime_spec_schema_config_schema_process_io_اولویت (container->container_def->process->io_اولویت);
          إذا (process->io_اولویت == عدم)
            OOM ();
        }

      إذا (process->scheduler == عدم && container->container_def->process->scheduler)
        {
          process->scheduler = clone_runtime_spec_schema_config_schema_process_scheduler (container->container_def->process->scheduler);
          إذا (process->scheduler == عدم)
            OOM ();
        }

      إذا (process->selinux_label == عدم && container->container_def->process->selinux_label)
        process->selinux_label = xstrdup (container->container_def->process->selinux_label);

      إذا (process->apparmor_profile == عدم && container->container_def->process->apparmor_profile)
        process->apparmor_profile = xstrdup (container->container_def->process->apparmor_profile);

      إذا (process->user == عدم && container->container_def->process->user)
        {
          process->user = clone_runtime_spec_schema_config_schema_process_user (container->container_def->process->user);
          إذا (process->user == عدم)
            OOM ();
        }
    }

  ret = initialize_security (container, process, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* Do not leak the session keyring of the caller into the container.  The
     keyring created for the container cannot be joined, so use an anonymous one.  */
  إذا (! context->no_new_keyring && ! وضعیت.no_new_keyring)
    {
      libcrun_debug ("Creating new keyring for the exec process");

      ret = libcrun_create_keyring (container, عدم, process->selinux_label, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = prctl (PR_مجموعه_DUMPABLE, 0, 0, 0, 0);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "prctl unset dumpable");

  pid = libcrun_join_process (context, container, وضعیت.pid, &وضعیت, opts->cgroup, context->detach,
                              process, process->terminal ? &terminal_fd : عدم, err);
  إذا (UNLIKELY (pid < 0))
    أرجع pid;

  /* Process to exec.  */
  إذا (pid == 0)
    {
      موقت_FAILURE_RETRY (close (pipefd0));
      pipefd0 = -1;

      exec_process_entrypoint (context, container, process, &pipefd1, seccomp_fd, seccomp_receiver_fd, custom_handler, opts->ادغام_env, err);
      /* It gets here only on errors.  */
      إذا (*err)
        {
          إذا (pipefd1 < 0)
            libcrun_fail_with_error ((*err)->وضعیت, "%s", (*err)->پیام);
          وإلا
            {
              write_error_مقصد_pipe (pipefd1, err);
              موقت_FAILURE_RETRY (close (pipefd1));
              pipefd1 = -1;
            }
        }
      امن_exit (EXIT_FAILURE);
    }

  موقت_FAILURE_RETRY (close (pipefd1));
  pipefd1 = -1;

  موقت_FAILURE_RETRY (اكتب (pipefd0, &pid, حجم (pid)));

  إذا (seccomp_fd >= 0)
    close_and_reset (&seccomp_fd);

  إذا (terminal_fd >= 0)
    {
      بیعلامت کوتاه ردیفها = 0, تعداد_ستون = 0;

      إذا (process->console_اندازه)
        {
          تعداد_ستون = process->console_اندازه->عرض;
          ردیفها = process->console_اندازه->ارتفاع;
        }

      ret = libcrun_terminal_راهاندازی_اندازه (terminal_fd, ردیفها, تعداد_ستون, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      إذا (context->console_socket)
        {
          صحيح ret;
          cleanup_close صحيح console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
          إذا (UNLIKELY (console_socket_fd < 0))
            أرجع console_socket_fd;
          ret = send_fd_مقصد_socket (console_socket_fd, terminal_fd, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          close_and_reset (&terminal_fd);
        }
      وإلا
        {
          ret = libcrun_clear_onlcr (terminal_fd, err);
          إذا (UNLIKELY (ret < 0))
            {
              flush_fd_مقصد_err (context, terminal_fd);
              أرجع ret;
            }

          ret = libcrun_مجموعه_raw (0, &orig_terminal, err);
          إذا (UNLIKELY (ret < 0))
            {
              flush_fd_مقصد_err (context, terminal_fd);
              أرجع ret;
            }
        }
    }

  ret = موقت_FAILURE_RETRY (اقرأ (pipefd0, &ret_from_فرزند, حجم (ret_from_فرزند)));
  إذا (ret != حجم (ret_from_فرزند))
    ret = crun_make_error (err, 0, "read pipe failed");
  وإلا إذا (ret_from_فرزند != 0)
    {
      cleanup_free حرف *پیام = عدم;
      size_t len = 0;

      ret = read_all_fd (pipefd0, "error stream", &پیام, &len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      /* the string from read_all_fd is always NUL terminated.  */
      ret = crun_make_error (err, ret_from_فرزند, "%s", پیام);
    }
  وإلا
    {
      /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
      إذا (own_seccomp_receiver_fd >= 0)
        {
          seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
          إذا (UNLIKELY (seccomp_notify_fd < 0))
            أرجع seccomp_notify_fd;

          ret = close_and_reset (&own_seccomp_receiver_fd);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "close seccomp receiver fd failed");
        }

      {
        بنية wait_for_process_آرگومانها آرگومانها = {
          .pid = pid,
          .context = context,
          .terminal_fd = terminal_fd,
          .notify_socket = -1,
          .container_ready_fd = عدم,
          .seccomp_notify_fd = seccomp_notify_fd,
          .seccomp_notify_plugins = seccomp_notify_plugins,
        };

        ret = wait_for_process (&آرگومانها, err);
      }
    }

  flush_fd_مقصد_err (context, terminal_fd);
  أرجع ret;
}

صحيح
libcrun_container_update (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *content, size_t len arg_استفادهنشده,
                          libcrun_error_t *err)
{
  cleanup_custom_handler_instance بنية custom_handler_instance_s *custom_handler = عدم;
  runtime_spec_schema_config_linux_resources *resources = عدم;
  cleanup_container libcrun_container_t *container = عدم;
  ثابت حرف *state_ریشه = context->state_ریشه;
  بنية parser_context ctx = { 0, stderr };
  libcrun_container_وضعیت_t وضعیت = {};
  parser_error parser_err = عدم;
  json_object *doc = عدم;
  صحيح ret;

  ret = libcrun_read_container_وضعیت (&وضعیت, state_ریشه, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = read_container_config_from_state (&container, state_ریشه, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  resources = make_runtime_spec_schema_config_linux_resources (doc, &ctx, &parser_err);
  إذا (UNLIKELY (resources == عدم))
    {
      ret = crun_make_error (err, 0, "cannot parse resources: %s", parser_err);
      اذهب cleanup;
    }

  إذا (custom_handler && custom_handler->vtable->modify_oci_configuration)
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
      إذا (UNLIKELY (ret < 0))
        اذهب cleanup;
    }

  ret = libcrun_linux_container_update (&وضعیت, state_ریشه, resources, err);

cleanup:
  إذا (doc)
    json_object_put (doc);
  حرر (parser_err);
  إذا (resources)
    free_runtime_spec_schema_config_linux_resources (resources);

  أرجع ret;
}

صحيح
libcrun_container_update_from_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *file, libcrun_error_t *err)
{
  cleanup_free حرف *content = عدم;
  size_t len;
  صحيح ret;

  ret = read_all_file (file, &content, &len, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع libcrun_container_update (context, id, content, len, err);
}

ساكن صحيح
compare_update_مقادیر (ثابت فراغ *a, ثابت فراغ *b)
{
  ثابت بنية libcrun_update_مقدار_s *aa = a;
  ثابت بنية libcrun_update_مقدار_s *bb = b;
  صحيح ret;

  ret = strcmp (aa->section, bb->section);
  إذا (ret)
    أرجع ret;
  أرجع strcmp (aa->name, bb->name);
}

صحيح
libcrun_container_update_from_مقادیر (libcrun_context_t *context, ثابت حرف *id,
                                      بنية libcrun_update_مقدار_s *مقادیر, size_t len,
                                      libcrun_error_t *err)
{
  ثابت حرف *current_section = عدم;
  ثابت حرف *buf;
  json_gen_ctx *gen = عدم;
  size_t i, buf_len;
  صحيح ret;

  إذا (! json_gen_init (&gen, عدم))
    أرجع crun_make_error (err, 0, "json_gen_init failed");
  json_gen_map_open (gen);

  qsort (مقادیر, len, حجم (بنية libcrun_update_مقدار_s), compare_update_مقادیر);

  لكل (i = 0; i < len; i++)
    {
      إذا (current_section == عدم || strcmp (مقادیر[i].section, current_section))
        {
          إذا (i > 0)
            json_gen_map_close (gen);

          current_section = مقادیر[i].section;
          json_gen_string (gen, current_section, طول_رشته (current_section));
          json_gen_map_open (gen);
        }

      json_gen_string (gen, مقادیر[i].name, طول_رشته (مقادیر[i].name));

      إذا (مقادیر[i].numeric)
        json_gen_عدد (gen, مقادیر[i].مقدار, طول_رشته (مقادیر[i].مقدار));
      وإلا
        json_gen_string (gen, مقادیر[i].مقدار, طول_رشته (مقادیر[i].مقدار));
    }
  إذا (len)
    json_gen_map_close (gen);

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);

  ret = libcrun_container_update (context, id, buf, buf_len, err);

  json_gen_free (gen);

  أرجع ret;
}

ساكن فراغ
populate_array_فیلد (حرف ***فیلد, حرف *array[], size_t رقم_elements)
{
  size_t i;

  *فیلد = xmalloc0 ((رقم_elements + 1) * حجم (حرف *));
  لكل (i = 0; i < رقم_elements; i++)
    (*فیلد)[i] = xstrdup (array[i]);

  (*فیلد)[i] = عدم;
}

#اگر_تعریف_شده HAVE_CAP
ساكن فراغ
populate_capabilities (بنية features_info_s *info, حرف ***capabilities, size_t *رقم_capabilities)
{
  size_t شاخص = 0;
  cap_مقدار_t i;
  حرف *endptr;
  صحيح j;

  *رقم_capabilities = 0;
  لكل (i = 0;; i++)
    {
      حرف *v = cap_مقصد_name (i);
      إذا (v == عدم)
        اكسر;
      strtol (v, &endptr, 10);
      إذا (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          اكسر;
        }
      (*رقم_capabilities)++;
    }

  *capabilities = xmalloc0 ((*رقم_capabilities + 1) * حجم (ثابت حرف *));
  لكل (i = 0; i < (cap_مقدار_t) *رقم_capabilities; i++)
    {
      حرف *v = cap_مقصد_name (i);
      إذا (v == عدم)
        اكسر;
      strtol (v, &endptr, 10);
      إذا (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          اكسر;
        }

      // Convert capability name to uppercase
      لكل (j = 0; v[j] != '\0'; j++)
        v[j] = به_بزرگ (v[j]);

      (*capabilities)[شاخص] = v;
      شاخص++;
    }

  (*capabilities)[شاخص] = عدم; // Terminate the array with NULL
  populate_array_فیلد (&(info->linux.capabilities), *capabilities, *رقم_capabilities);
}
#پایان_اگر

ساكن فراغ
retrieve_mount_options (بنية features_info_s **info)
{
  cleanup_free ثابت بنية انتشار_flags_s *mount_options_list = عدم;
  size_t رقم_mount_options = 0;

  // Retrieve mount options from wordlist
  mount_options_list = get_mount_flags_from_wordlist ();

  // Calculate the number of mount options
  طالما (mount_options_list[رقم_mount_options].name != عدم)
    رقم_mount_options++;

  // Allocate memory for mount options in info struct
  (*info)->mount_options = xmalloc0 ((رقم_mount_options + 1) * حجم (حرف *));

  // Copy mount options to info struct
  لكل (size_t i = 0; i < رقم_mount_options; i++)
    (*info)->mount_options[i] = xstrdup (mount_options_list[i].name);
}

صحيح
libcrun_container_get_features (libcrun_context_t *context, بنية features_info_s **info, libcrun_error_t *err arg_استفادهنشده)
{
  // Allocate memory for the features_info_s structure
  size_t رقم_namspaces = حجم (namespaces) / حجم (namespaces[0]);
  size_t رقم_operators = حجم (operators) / حجم (operators[0]);
  size_t رقم_actions = حجم (actions) / حجم (actions[0]);
  size_t رقم_hooks = حجم (hooks) / حجم (hooks[0]);
  size_t رقم_archs = حجم (archs) / حجم (archs[0]);
  size_t رقم_unsafe_annotations = حجم (potentially_unsafe_annotations) / حجم (potentially_unsafe_annotations[0]);
  cleanup_free حرف **capabilities = عدم;
  size_t رقم_capabilities = 0;
  size_t رقم_mempolicy_modes = حجم (mempolicy_modes) / حجم (mempolicy_modes[0]);
  size_t رقم_mempolicy_flags = حجم (mempolicy_flags) / حجم (mempolicy_flags[0]);

  *info = xmalloc0 (حجم (بنية features_info_s));

  // Hardcoded feature information
  (*info)->oci_نسخه_min = xstrdup ("1.0.0");
  (*info)->oci_نسخه_max = xstrdup ("1.1.0+dev");

  // Populate hooks
  populate_array_فیلد (&((*info)->hooks), hooks, رقم_hooks);

  // Populate mount_options
  retrieve_mount_options (info);

  // Populate namespaces
  populate_array_فیلد (&((*info)->linux.namespaces), namespaces, رقم_namspaces);

#اگر_تعریف_شده HAVE_CAP
  // Populate capabilities
  populate_capabilities (*info, &capabilities, &رقم_capabilities);
#پایان_اگر

  // Hardcode the values for cgroup
  (*info)->linux.cgroup.v1 = true;
  (*info)->linux.cgroup.v2 = true;
#اگر_تعریف_شده HAVE_SYSTEMD
  (*info)->linux.cgroup.systemd = true;
  (*info)->linux.cgroup.systemd_user = true;
#پایان_اگر

  // Put seccomp values
#اگر_تعریف_شده HAVE_SECCOMP
  (*info)->linux.seccomp.enabled = true;
  // Populate actions
  populate_array_فیلد (&((*info)->linux.seccomp.actions), actions, رقم_actions);
  // Populate operators
  populate_array_فیلد (&((*info)->linux.seccomp.operators), operators, رقم_operators);
  // Populate archs
  populate_array_فیلد (&((*info)->linux.seccomp.archs), archs, رقم_archs);
#وإلا
  (*info)->linux.seccomp.enabled = false;
#پایان_اگر

  // Put values for apparmor and selinux
  (*info)->linux.apparmor.enabled = true;
  (*info)->linux.selinux.enabled = true;

  (*info)->linux.intel_rdt.enabled = true;

  (*info)->linux.net_devices.enabled = true;

  populate_array_فیلد (&((*info)->linux.حافظه_policy.نما), mempolicy_modes, رقم_mempolicy_modes);
  populate_array_فیلد (&((*info)->linux.حافظه_policy.flags), mempolicy_flags, رقم_mempolicy_flags);

  // Put the values for mount extensions
  (*info)->linux.mount_ext.idmap.enabled = true;

  // Populate the values for annotations
#اگر_تعریف_شده HAVE_SECCOMP
  {
    ثابت بنية scmp_نسخه *نسخه = seccomp_نسخه ();
    حرف *نسخه_string = عدم;

    xasprintf (&نسخه_string, "%u.%u.%u", نسخه->major, نسخه->minor, نسخه->micro);
    (*info)->annotations.io_github_seccomp_libseccomp_نسخه = نسخه_string;
  }
#پایان_اگر

  إذا (context->handler_manager && handler_by_name (context->handler_manager, "wasm"))
    (*info)->annotations.پیاپی_oci_crun_wasm = true;

#إذا HAVE_CRIU
  (*info)->annotations.پیاپی_oci_crun_checkpoint_enabled = true;
#پایان_اگر
  (*info)->annotations.پیاپی_oci_crun_commit = GIT_نسخه;
  (*info)->annotations.پیاپی_oci_crun_نسخه = PACKAGE_نسخه;

  populate_array_فیلد (&((*info)->potentially_unsafe_annotations), potentially_unsafe_annotations, رقم_unsafe_annotations);

  أرجع 0;
}

ساكن فراغ
features_جمع_کن_string (json_gen_ctx *json_gen, ثابت حرف *کلید, ثابت حرف *مقدار)
{
  json_gen_string (json_gen, کلید, طول_رشته (کلید));
  json_gen_string (json_gen, مقدار, طول_رشته (مقدار));
}

ساكن فراغ
features_جمع_کن_bool (json_gen_ctx *json_gen, ثابت حرف *کلید, صحيح مقدار)
{
  json_gen_string (json_gen, کلید, طول_رشته (کلید));
  json_gen_bool (json_gen, مقدار);
}

ساكن فراغ
features_جمع_کن_bool_str (json_gen_ctx *json_gen, ثابت حرف *کلید, صحيح مقدار)
{
  features_جمع_کن_string (json_gen, کلید, مقدار ? "true" : "false");
}

ساكن فراغ
features_جمع_کن_array (json_gen_ctx *json_gen, ثابت حرف *کلید, حرف **array)
{
  size_t i;

  json_gen_string (json_gen, کلید, طول_رشته (کلید));
  json_gen_array_open (json_gen);
  إذا (array)
    لكل (i = 0; array[i] != عدم; i++)
      json_gen_string (json_gen, array[i], طول_رشته (array[i]));
  json_gen_array_close (json_gen);
}

ساكن فراغ
features_جمع_کن_linux_info (json_gen_ctx *json_gen, ثابت بنية linux_info_s *linux)
{
  json_gen_string (json_gen, "linux", طول_رشته ("linux"));
  json_gen_map_open (json_gen);

  features_جمع_کن_array (json_gen, "namespaces", linux->namespaces);
  features_جمع_کن_array (json_gen, "capabilities", linux->capabilities);

  json_gen_string (json_gen, "cgroup", طول_رشته ("cgroup"));
  json_gen_map_open (json_gen);
  features_جمع_کن_bool (json_gen, "v1", linux->cgroup.v1);
  features_جمع_کن_bool (json_gen, "v2", linux->cgroup.v2);
  features_جمع_کن_bool (json_gen, "systemd", linux->cgroup.systemd);
  features_جمع_کن_bool (json_gen, "systemdUser", linux->cgroup.systemd_user);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "seccomp", طول_رشته ("seccomp"));
  json_gen_map_open (json_gen);
  features_جمع_کن_bool (json_gen, "enabled", linux->seccomp.enabled);
  إذا (linux->seccomp.actions)
    features_جمع_کن_array (json_gen, "actions", linux->seccomp.actions);
  إذا (linux->seccomp.operators)
    features_جمع_کن_array (json_gen, "operators", linux->seccomp.operators);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "apparmor", طول_رشته ("apparmor"));
  json_gen_map_open (json_gen);
  features_جمع_کن_bool (json_gen, "enabled", linux->apparmor.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "selinux", طول_رشته ("selinux"));
  json_gen_map_open (json_gen);
  features_جمع_کن_bool (json_gen, "enabled", linux->selinux.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "mountExtensions", طول_رشته ("mountExtensions"));
  json_gen_map_open (json_gen);
  json_gen_string (json_gen, "idmap", طول_رشته ("idmap"));
  json_gen_map_open (json_gen);
  features_جمع_کن_bool (json_gen, "enabled", linux->mount_ext.idmap.enabled);
  json_gen_map_close (json_gen);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "intelRdt", طول_رشته ("intelRdt"));
  json_gen_map_open (json_gen);
  features_جمع_کن_bool (json_gen, "enabled", linux->intel_rdt.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "netDevices", طول_رشته ("netDevices"));
  json_gen_map_open (json_gen);
  features_جمع_کن_bool (json_gen, "enabled", linux->net_devices.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "memoryPolicy", طول_رشته ("memoryPolicy"));
  json_gen_map_open (json_gen);
  إذا (linux->حافظه_policy.نما)
    features_جمع_کن_array (json_gen, "modes", linux->حافظه_policy.نما);
  إذا (linux->حافظه_policy.flags)
    features_جمع_کن_array (json_gen, "flags", linux->حافظه_policy.flags);
  json_gen_map_close (json_gen);

  json_gen_map_close (json_gen);
}

ساكن فراغ
features_جمع_کن_annotations_info (json_gen_ctx *json_gen, ثابت بنية annotations_info_s *annotation)
{
  json_gen_string (json_gen, "annotations", طول_رشته ("annotations"));
  json_gen_map_open (json_gen);

  إذا (! is_خالی_string (annotation->io_github_seccomp_libseccomp_نسخه))
    features_جمع_کن_string (json_gen, "io.github.seccomp.libseccomp.version", annotation->io_github_seccomp_libseccomp_نسخه);

  features_جمع_کن_bool_str (json_gen, "org.opencontainers.runc.checkpoint.enabled", annotation->پیاپی_oci_crun_checkpoint_enabled);
  features_جمع_کن_bool_str (json_gen, "run.oci.crun.checkpoint.enabled", annotation->پیاپی_oci_crun_checkpoint_enabled);

  features_جمع_کن_string (json_gen, "run.oci.crun.commit", annotation->پیاپی_oci_crun_commit);
  features_جمع_کن_string (json_gen, "run.oci.crun.version", annotation->پیاپی_oci_crun_نسخه);

  features_جمع_کن_bool_str (json_gen, "run.oci.crun.wasm", annotation->پیاپی_oci_crun_wasm);

  json_gen_map_close (json_gen);
}

صحيح
libcrun_container_get_features_json (libcrun_context_t *context, حرف **بیرون, libcrun_error_t *err)
{
  cleanup_struct_features بنية features_info_s *info = عدم;
  ثابت حرف *میانگیر = عدم;
  json_gen_ctx *json_gen;
  size_t میانگیر_len;
  صحيح ret;

  *بیرون = عدم;

  ret = libcrun_container_get_features (context, &info, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (! json_gen_init (&json_gen, عدم))
    أرجع crun_make_error (err, 0, "cannot initialize JSON generator");

  json_gen_config (json_gen, json_gen_beautify, 1);

  json_gen_map_open (json_gen);

  features_جمع_کن_string (json_gen, "ociVersionMin", info->oci_نسخه_min);
  features_جمع_کن_string (json_gen, "ociVersionMax", info->oci_نسخه_max);
  features_جمع_کن_array (json_gen, "hooks", info->hooks);
  features_جمع_کن_array (json_gen, "mountOptions", info->mount_options);
  features_جمع_کن_linux_info (json_gen, &info->linux);
  features_جمع_کن_annotations_info (json_gen, &info->annotations);
  features_جمع_کن_array (json_gen, "potentiallyUnsafeConfigAnnotations", info->potentially_unsafe_annotations);

  json_gen_map_close (json_gen);

  json_gen_get_buf (json_gen, &میانگیر, &میانگیر_len);
  *بیرون = xstrdup (میانگیر);
  json_gen_free (json_gen);

  أرجع 0;
}

/* Read the container status and fail if the container is not running.  */
ساكن صحيح
read_وضعیت_require_running (libcrun_context_t *context, ثابت حرف *id,
                             libcrun_container_وضعیت_t *وضعیت, libcrun_error_t *err)
{
  صحيح ret;

  ret = libcrun_read_container_وضعیت (وضعیت, context->state_ریشه, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_is_container_running (وضعیت, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;
  إذا (ret == 0)
    أرجع crun_make_error (err, 0, "the container `%s` is not running", id);

  أرجع 0;
}

صحيح
libcrun_container_pause (libcrun_context_t *context, ثابت حرف *id, libcrun_error_t *err)
{
  صحيح ret;
  libcrun_container_وضعیت_t وضعیت = {};

  ret = read_وضعیت_require_running (context, id, &وضعیت, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع libcrun_container_pause_linux (&وضعیت, err);
}

صحيح
libcrun_container_unpause (libcrun_context_t *context, ثابت حرف *id, libcrun_error_t *err)
{
  صحيح ret;
  libcrun_container_وضعیت_t وضعیت = {};

  ret = read_وضعیت_require_running (context, id, &وضعیت, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع libcrun_container_unpause_linux (&وضعیت, err);
}

/* Translate the public options struct into the internal representation used by
   the linux/CRIU layer.  Strings are borrowed (not copied).  */
ساكن فراغ
cr_options_from_عمومی (libcrun_checkpoint_restore_t *dst, بنية libcrun_checkpoint_restore_options_s *src)
{
  memset (dst, 0, حجم (*dst));
  dst->image_مسیر = (حرف *) src->image_مسیر;
  dst->work_مسیر = (حرف *) src->work_مسیر;
  dst->والد_مسیر = (حرف *) src->والد_مسیر;
  dst->console_socket = src->console_socket;
  dst->lsm_profile = (حرف *) src->lsm_profile;
  dst->lsm_mount_context = (حرف *) src->lsm_mount_context;
  dst->leave_running = src->leave_running;
  dst->tcp_established = src->tcp_established;
  dst->tcp_close = src->tcp_close;
  dst->ext_unix_sk = src->ext_unix_sk;
  dst->shell_کار = src->shell_کار;
  dst->file_locks = src->file_locks;
  dst->pre_dump = src->pre_dump;
  dst->detach = src->detach;
  dst->manage_cgroups_نما = src->manage_cgroups_نما;
  dst->شبکه_قفل_روش = src->شبکه_قفل_روش;
}

صحيح
libcrun_container_checkpoint (libcrun_context_t *context, ثابت حرف *id,
                              بنية libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  صحيح ret;
  ثابت حرف *state_ریشه = context->state_ریشه;
  libcrun_container_وضعیت_t وضعیت = {};
  libcrun_checkpoint_restore_t cr;
  cleanup_container libcrun_container_t *container = عدم;

  cr_options_from_عمومی (&cr, cr_options);

  ret = read_وضعیت_require_running (context, id, &وضعیت, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = read_container_config_from_state (&container, state_ریشه, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* Needed to use the same paths (e.g. the shared empty directory used for
     masked paths) as the container runtime and restore do.  */
  container->context = context;

  ret = libcrun_container_checkpoint_linux (&وضعیت, container, &cr, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (! (cr_options->leave_running || cr_options->pre_dump))
    أرجع container_delete_داخلی (context, عدم, عدم, id, true, true, err);

  أرجع 0;
}

/* The dummy process only keeps the cgroup populated until the container is restored there.
   Its cgroup is below the container cgroup, so once the container runs it can freeze or kill
   the dummy process: never depend on it after that point.  */
ساكن فراغ
restore_proxy_process (صحيح proxy_pid_pipe)
{
  حرف c;

  /* Wait to be killed by the parent, or for the parent to go away.  */
  موقت_FAILURE_RETRY (اقرأ (proxy_pid_pipe, &c, حجم (c)));
}

ساكن صحيح
restore_terminate_proxy_process (pid_t proxy_pid, ثابت حرف *proxy_cgroup, pid_t container_pid, صحيح cgroup_manager, libcrun_error_t *err)
{
  صحيح نما;
  صحيح ret;

  إذا (cgroup_manager == CGROUP_MANAGER_SYSTEMD)
    {
      حرف ready_str[64];

      ret = snprintf (ready_str, حجم (ready_str), "MAINPID=%d", container_pid);
      إذا (UNLIKELY (ret >= (صحيح) حجم (ready_str)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");

      ret = send_sd_notify (ready_str, err);
      /* Do not fail on errors.  */
      إذا (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  نما = libcrun_get_cgroup_نما (err);
  إذا (UNLIKELY (نما < 0))
    أرجع نما;

  /* SIGKILL terminates a process frozen with cgroup v2, while the cgroup v1 freezer does not
     deliver it until the cgroup is thawed, so do not wait for it there.  */
  kill (proxy_pid, SIGKILL);
  موقت_FAILURE_RETRY (waitpid (proxy_pid, عدم, نما == CGROUP_نما_UNIFIED ? 0 : WNOHANG));

  أرجع destroy_cgroup_مسیر (proxy_cgroup, نما, err);
}

صحيح
libcrun_container_restore (libcrun_context_t *context, ثابت حرف *id arg_استفادهنشده,
                           بنية libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  cleanup_cgroup_وضعیت بنية libcrun_cgroup_وضعیت *cgroup_وضعیت = عدم;
  cleanup_container libcrun_container_t *container = عدم;
  cleanup_close صحيح proxy_pid_pipe0 = -1;
  cleanup_close صحيح proxy_pid_pipe1 = -1;
  runtime_spec_schema_config_schema *def;
  libcrun_container_وضعیت_t وضعیت = {};
  بنية libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  libcrun_checkpoint_restore_t cr;
  cleanup_pid pid_t proxy_pid = -1;
  cleanup_close صحيح seccomp_fd = -1;
  ثابت حرف *seccomp_bpf_داده;
  صحيح proxy_pid_pipe[2];
  صحيح cgroup_manager;
  uid_t ریشه_uid = -1;
  gid_t ریشه_gid = -1;
  صحيح ret;

  cr_options_from_عمومی (&cr, cr_options);

  container = libcrun_container_بارگیری_from_file ("config.json", err);
  إذا (container == عدم)
    أرجع -1;

  container->context = context;
  def = container->container_def;

  ret = check_config_file (def, context, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_وضعیت_check_directories (context->state_ریشه, context->id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_copy_config_file (context->id, context->state_ریشه, container, err);
  إذا (UNLIKELY (ret < 0))
    اذهب fail;

  /* CRIU restores the seccomp filters of the checkpointed processes, but
     exec loads the filter from the seccomp.bpf file in the state directory,
     so it must be generated again for the restored container.  */
  seccomp_bpf_داده = پیدا_کن_annotation (container, "run.oci.seccomp_bpf_data");

  ret = راهاندازی_seccomp (container, seccomp_bpf_داده, &seccomp_gen_ctx, &seccomp_fd, err);
  إذا (UNLIKELY (ret < 0))
    اذهب fail;

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_داده, &seccomp_gen_ctx, err);
  إذا (UNLIKELY (ret < 0))
    اذهب fail;
  close_and_reset (&seccomp_fd);

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  إذا (context->systemd_cgroup)
    cgroup_manager = CGROUP_MANAGER_SYSTEMD;
  وإلا إذا (context->force_no_cgroup)
    cgroup_manager = CGROUP_MANAGER_DISABLED;

  def = container->container_def;

  /* If we are root (either on the host or in a namespace),
   * then chown the cgroup to root in the container user namespace. */
  get_ریشه_درون_the_userns (def, container->host_uid, container->host_gid, &ریشه_uid, &ریشه_gid);

  /* If the root in the container is different than the current root user, attempt to chown
     the std streams before entering the user namespace.  Otherwise we might lose access
     to the user (as it is not mapped in the user namespace) and cannot chown them.  */
  إذا (ریشه_uid > 0 || ریشه_gid > 0)
    {
      ret = maybe_chown_std_streams (ریشه_uid, ریشه_gid, err);
      إذا (UNLIKELY (ret < 0))
        اذهب fail;
    }

  /* CRIU runs as a child process, so the restored processes would inherit the session keyring.  */
  ret = راهاندازی_container_keyring (container, context, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  {
    cleanup_close صحيح cgroup_dirfd = -1;
    bool needs_proxy_process;
    cleanup_cgroup_preenter بنية libcrun_cgroup_آرگومانها cg = {
      .resources = def->linux ? def->linux->resources : عدم,
      .annotations = container->annotations,
      .cgroup_مسیر = def->linux ? def->linux->cgroups_مسیر : "",
      .manager = cgroup_manager,
      .ریشه_uid = ریشه_uid,
      .ریشه_gid = ریشه_gid,
      .id = context->id,
      .state_ریشه = context->state_ریشه,
      .container = container,
    };

    /* The CRIU restore code uses bundle, rootfs and cgroup_path of status.   The cgroup_path is set later.  */
    وضعیت.bundle = (حرف *) context->bundle;
    وضعیت.rootfs = def->ریشه->مسیر;

    ret = libcrun_cgroup_preenter (&cg, &cgroup_dirfd, err);
    إذا (UNLIKELY (ret < 0))
      اذهب fail;

    needs_proxy_process = cgroup_dirfd < 0;

    /* If the target cgroup was already created (with cgroupfs), we can restore the container directly there.  */
    إذا (! needs_proxy_process)
      {
        cleanup_free حرف *cgroup_مسیر = عدم;

        ret = get_cgroup_dirfd_مسیر (cgroup_dirfd, &cgroup_مسیر, err);
        إذا (UNLIKELY (ret < 0))
          اذهب fail;

        /* Complete the configuration for the cgroup before the container is
           restored there: CRIU resumes the restored processes before it
           returns, so the device filter and the limits must already be in
           place.  There is no process to move, as CRIU is run in the cgroup,
           and the controllers were enabled by precreate_cgroup.  */
        cg.joined = true;

        ret = libcrun_cgroup_enter (&cg, &cgroup_وضعیت, err);
        إذا (UNLIKELY (ret < 0))
          اذهب fail;

        /* Restore the container directly in the desired cgroup.  */
        وضعیت.cgroup_مسیر = cgroup_مسیر;

        ret = libcrun_container_restore_linux (&وضعیت, container, &cr, err);
        إذا (UNLIKELY (ret < 0))
          اذهب fail;

        /* Now that the container is restored, keep any signal for it, so that
           it is forwarded to the init by wait_for_process, rather than acted
           upon by crun.  This cannot be done earlier, as CRIU, which is run as
           a child process, would inherit the blocked signals.  */
        ret = بلوک_سیگنالها (err);
        إذا (UNLIKELY (ret < 0))
          اذهب fail;

        /* Use the container first process PID to finalize the cgroup.  */
        cg.pid = وضعیت.pid;
      }
    وإلا
      {
        /* When using systemd, we need a process first that must exist before a cgroup is created.
           Create a dummy process that sits and waits until the container is restored.  */
        ret = pipe2 (proxy_pid_pipe, O_CLOEXEC);
        إذا (UNLIKELY (ret < 0))
          {
            ret = crun_make_error (err, errno, "pipe");
            اذهب fail;
          }

        /* Only used for auto cleanup.  */
        proxy_pid_pipe0 = proxy_pid_pipe[0];
        proxy_pid_pipe1 = proxy_pid_pipe[1];

        proxy_pid = fork ();
        إذا (UNLIKELY (proxy_pid < 0))
          {
            ret = crun_make_error (err, errno, "fork");
            اذهب fail;
          }

        إذا (proxy_pid == 0)
          {
            close_and_reset (&proxy_pid_pipe1);

            restore_proxy_process (proxy_pid_pipe0);
            امن_exit (EXIT_SUCCESS);
          }

        close_and_reset (&proxy_pid_pipe0);

        /* Use the dummy process PID as the PID to setup the cgroup.  */
        cg.pid = proxy_pid;

        /* Complete the configuration for the cgroup.  It contains only the dummy process.  */
        ret = libcrun_cgroup_enter (&cg, &cgroup_وضعیت, err);
        إذا (UNLIKELY (ret < 0))
          اذهب fail;
      }

    ret = libcrun_cgroup_enter_finalize (&cg, cgroup_وضعیت, err);
    إذا (UNLIKELY (ret < 0))
      اذهب fail;

    /* When using a dummy process, the container is restored here once the cgroup is configured.  */
    إذا (needs_proxy_process)
      {
        cleanup_free حرف *هدف_cgroup = عدم;
        cleanup_free حرف *proxy_cgroup = عدم;

        ret = libcrun_get_cgroup_process (proxy_pid, &هدف_cgroup, false, err);
        إذا (UNLIKELY (ret < 0))
          اذهب fail;

        /* Move the dummy process to a sub-cgroup.  */
        ret = append_paths (&proxy_cgroup, err, هدف_cgroup, ".proxy-crun", عدم);
        إذا (UNLIKELY (ret < 0))
          اذهب fail;

        ret = libcrun_move_process_مقصد_cgroup (proxy_pid, 0, proxy_cgroup, true, err);
        إذا (UNLIKELY (ret < 0))
          اذهب fail;

        /* Restore the container in the same cgroup where the dummy process was.  */
        وضعیت.cgroup_مسیر = هدف_cgroup;

        ret = libcrun_container_restore_linux (&وضعیت, container, &cr, err);
        إذا (UNLIKELY (ret < 0))
          اذهب fail;

        /* See the comment for the other libcrun_container_restore_linux call.  */
        ret = بلوک_سیگنالها (err);
        إذا (UNLIKELY (ret < 0))
          اذهب fail;

        /* The container is running, so do not fail because of the dummy process.  */
        ret = restore_terminate_proxy_process (proxy_pid, proxy_cgroup, وضعیت.pid, cgroup_manager, err);
        إذا (UNLIKELY (ret < 0))
          crun_error_release (err);
        proxy_pid = -1;

        close_and_reset (&proxy_pid_pipe1);
      }
  }

  context->detach = cr_options->detach;
  ret = write_container_وضعیت (container, context, وضعیت.pid, cgroup_وضعیت, err);
  إذا (UNLIKELY (ret < 0))
    اذهب fail;

  {
    /* Wait the same way run and exec do: apart from waiting for the container
       init, this writes the pid file, forwards any signal to the init, and
       reaps every child, not only the init.  */
    بنية wait_for_process_آرگومانها آرگومانها = {
      .pid = وضعیت.pid,
      .context = context,
      .terminal_fd = -1,
      .notify_socket = -1,
      .container_ready_fd = عدم,
      .seccomp_notify_fd = -1,
      .seccomp_notify_plugins = عدم,
    };

    أرجع wait_for_process (&آرگومانها, err);
  }

fail:
  /* Once restored, the container is running.  Without a status file nothing
     can reach it anymore, so do not leave it behind.  */
  إذا (وضعیت.pid > 0)
    {
      kill (وضعیت.pid, SIGKILL);
      waitpid_ignore_stopped (وضعیت.pid, عدم, 0);
    }
  إذا (cgroup_وضعیت)
    {
      libcrun_error_t tmp_err = عدم;
      libcrun_cgroup_destroy (cgroup_وضعیت, &tmp_err);
      crun_error_release (&tmp_err);
    }
  {
    libcrun_error_t tmp_err = عدم;

    إذا (libcrun_container_delete_وضعیت (context->state_ریشه, context->id, &tmp_err) < 0)
      crun_error_release (&tmp_err);
  }
  أرجع ret;
}

صحيح
libcrun_container_read_pids (libcrun_context_t *context, ثابت حرف *id, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  cleanup_cgroup_وضعیت بنية libcrun_cgroup_وضعیت *cgroup_وضعیت = عدم;
  cleanup_container_وضعیت libcrun_container_وضعیت_t وضعیت = {};
  صحيح ret;

  ret = libcrun_read_container_وضعیت (&وضعیت, context->state_ریشه, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (وضعیت.cgroup_مسیر == عدم || وضعیت.cgroup_مسیر[0] == '\0')
    أرجع crun_make_error (err, 0, "the container is not using cgroups");

  cgroup_وضعیت = libcrun_cgroup_make_وضعیت (&وضعیت);

  أرجع libcrun_cgroup_read_pids (cgroup_وضعیت, recurse, pids, err);
}

صحيح
libcrun_container_list_json (libcrun_context_t *context, حرف **بیرون, libcrun_error_t *err)
{
  libcrun_container_list_t *list = عدم, *it;
  ثابت حرف *content = عدم;
  json_gen_ctx *gen = عدم;
  size_t len;
  صحيح ret;

  *بیرون = عدم;

  ret = libcrun_get_containers_list (&list, context->state_ریشه, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (! json_gen_init (&gen, عدم))
    {
      ret = crun_make_error (err, 0, "cannot allocate json generator");
      اذهب خروج;
    }

  json_gen_config (gen, json_gen_beautify, 1);
  json_gen_array_open (gen);

  لكل (it = list; it; it = it->بعدی)
    {
      libcrun_container_وضعیت_t وضعیت;
      صحيح running = 0;
      صحيح pid;
      ثابت حرف *container_وضعیت = عدم;

      ret = libcrun_read_container_وضعیت (&وضعیت, context->state_ریشه, it->name, err);
      إذا (UNLIKELY (ret < 0))
        اذهب خروج;

      pid = وضعیت.pid;

      ret = libcrun_get_container_state_string (it->name, &وضعیت, context->state_ریشه, &container_وضعیت,
                                                &running, err);
      إذا (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          استمر;
        }

      إذا (! running)
        pid = 0;

      json_gen_map_open (gen);
      json_gen_string (gen, "id", طول_رشته ("id"));
      json_gen_string (gen, it->name, طول_رشته (it->name));
      json_gen_string (gen, "pid", طول_رشته ("pid"));
      map_int (gen, pid);
      json_gen_string (gen, "status", طول_رشته ("status"));
      json_gen_string (gen, container_وضعیت, طول_رشته (container_وضعیت));
      json_gen_string (gen, "bundle", طول_رشته ("bundle"));
      json_gen_string (gen, وضعیت.bundle, طول_رشته (وضعیت.bundle));
      json_gen_string (gen, "created", طول_رشته ("created"));
      json_gen_string (gen, وضعیت.created, طول_رشته (وضعیت.created));
      json_gen_string (gen, "owner", طول_رشته ("owner"));
      json_gen_string (gen, وضعیت.owner, امن_strlen (وضعیت.owner));
      json_gen_map_close (gen);

      libcrun_free_container_وضعیت (&وضعیت);
    }

  json_gen_array_close (gen);
  إذا (json_gen_get_buf (gen, &content, &len) != json_gen_وضعیت_ok)
    {
      ret = libcrun_make_error (err, 0, "cannot generate json list");
      اذهب خروج;
    }

  *بیرون = xstrdup (content);
  ret = 0;

خروج:
  إذا (list)
    libcrun_free_containers_list (list);
  إذا (gen)
    json_gen_free (gen);

  أرجع ret;
}

صحيح
libcrun_write_json_containers_list (libcrun_context_t *context, FILE *بیرون, libcrun_error_t *err)
{
  cleanup_free حرف *content = عدم;
  size_t len;
  صحيح ret;

  ret = libcrun_container_list_json (context, &content, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  len = طول_رشته (content);
  إذا (len && fwrite (content, 1, len, بیرون) != len)
    أرجع crun_make_error (err, errno, "error writing to file");

  أرجع 0;
}

صحيح
libcrun_container_update_intel_rdt (libcrun_context_t *context, ثابت حرف *id, بنية libcrun_intel_rdt_update *update, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = عدم;
  صحيح ret;

  ret = read_container_config_from_state (&container, context->state_ریشه, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع libcrun_update_intel_rdt (id, container, update->l3_cache_schema, update->mem_bw_schema, update->schemata, err);
}

ساكن صحيح
libcrun_container_جمع_کن_or_remove_mounts_from_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *file, bool جمع_کن, libcrun_error_t *err)
{
  cleanup_custom_handler_instance بنية custom_handler_instance_s *custom_handler = عدم;
  cleanup_container libcrun_container_t *container = عدم;
  cleanup_free runtime_spec_schema_defs_mount **mounts = عدم;
  ثابت حرف *state_ریشه = context->state_ریشه;
  cleanup_free parser_error parser_err = عدم;
  بنية parser_context pctx = { 0, stderr };
  libcrun_container_وضعیت_t وضعیت = {};
  cleanup_free حرف *content = عدم;
  json_object *doc = عدم;
  size_t n_mounts = 0, len, i;
  صحيح ret = 1;

  ret = read_all_file (file, &content, &len, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_read_container_وضعیت (&وضعیت, state_ریشه, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = read_container_config_from_state (&container, state_ریشه, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = parse_json_file (&doc, content, &pctx, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (! json_object_is_type (doc, json_type_array))
    {
      ret = crun_make_error (err, 0, "mounts must be an array");
      اذهب cleanup;
    }
  وإلا
    {
      n_mounts = json_object_array_درازی (doc);

      mounts = xmalloc0 ((n_mounts + 1) * حجم (*mounts));
      لكل (i = 0; i < n_mounts; i++)
        {
          mounts[i] = make_runtime_spec_schema_defs_mount (json_object_array_get_idx (doc, i), &pctx, &parser_err);
          إذا (mounts[i] == عدم)
            {
              ret = crun_make_error (err, 0, "cannot parse mount: %s", parser_err);
              اذهب cleanup;
            }
        }
    }

  إذا (جمع_کن)
    ret = libcrun_make_runtime_mounts (container, &وضعیت, mounts, n_mounts, err);
  وإلا
    ret = libcrun_destroy_runtime_mounts (container, &وضعیت, mounts, n_mounts, err);

cleanup:
  إذا (doc)
    json_object_put (doc);
  إذا (mounts)
    لكل (i = 0; i < n_mounts; i++)
      free_runtime_spec_schema_defs_mount (mounts[i]);
  أرجع ret;
}

صحيح
libcrun_container_جمع_کن_mounts_from_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *file, libcrun_error_t *err)
{
  أرجع libcrun_container_جمع_کن_or_remove_mounts_from_file (context, id, file, true, err);
}

صحيح
libcrun_container_remove_mounts_from_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *file, libcrun_error_t *err)
{
  أرجع libcrun_container_جمع_کن_or_remove_mounts_from_file (context, id, file, false, err);
}
