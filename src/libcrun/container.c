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
#وصف_ڪريو GNU_ذريعو

#ضمن <config.h>
#ضمن <ocispec/runtime_spec_schema_config_schema.h>
#ضمن <stdbool.h>
#ضمن "container.h"
#ضمن "utils.h"
#ضمن "json_gen_utils.h"
#ضمن "seccomp.h"
#ضمن "mempolicy.h"
#جيڪڏهن_وصف_ٿيل HAVE_SECCOMP
#  include <seccomp.h>
#جيڪڏهن_پورو
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
#جيڪڏهن_وصف_ٿيل HAVE_CAP
#  include <sys/capability.h>
#جيڪڏهن_پورو
#ضمن <sys/ioctl.h>
#ضمن <termios.h>
#ضمن <grp.h>
#ضمن <git-version.h>

#جيڪڏهن_وصف_ٿيل HAVE_SYSTEMD
#  include <systemd/sd-daemon.h>
#جيڪڏهن_پورو

#ضمن <json-c/json.h>
#ضمن <ocispec/json_common.h>

تعداد
{
  SYNC_SOCKET_SYNC_پيغام,
  SYNC_SOCKET_ERROR_پيغام,
  SYNC_SOCKET_WARNING_پيغام,
  SYNC_SOCKET_DEBUG_پيغام,
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

  صحيح hooks_ٻاهريون_fd;
  صحيح hooks_err_fd;

  /* Absolute bundle path, to be reported to hooks run in the container.  */
  ثابت حرف *bundle;

  بنية custom_handler_instance_s *custom_handler;
};

بنية sync_socket_پيغام_s
{
  صحيح type;
  صحيح error_قدر;
  حرف پيغام[512];
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

#وصف_ڪريو SYNC_SOCKET_پيغام_LEN(x, l) (offsetof (بنية sync_socket_پيغام_s, پيغام) + l)

ساكن صحيح
sync_socket_write_نياپو (صحيح fd, صحيح verbosity, صحيح err_قدر, ثابت حرف *log_نياپو)
{
  صحيح ret;
  size_t err_len;
  بنية sync_socket_پيغام_s نياپو;
  بدل (verbosity)
    {
    حالة LIBCRUN_VERBOSITY_DEBUG:
      نياپو.type = SYNC_SOCKET_DEBUG_پيغام;
      اكسر;
    حالة LIBCRUN_VERBOSITY_WARNING:
      نياپو.type = SYNC_SOCKET_WARNING_پيغام;
      اكسر;
    حالة LIBCRUN_VERBOSITY_ERROR:
      نياپو.type = SYNC_SOCKET_ERROR_پيغام;
      اكسر;
    }
  نياپو.error_قدر = err_قدر;

  إذا (fd < 0)
    أرجع 0;

  err_len = لڙي_ڊيگهه (log_نياپو);
  إذا (err_len >= حجم (نياپو.پيغام))
    err_len = حجم (نياپو.پيغام) - 1;

  يادگيري_نقل (نياپو.پيغام, log_نياپو, err_len);
  نياپو.پيغام[err_len] = '\0';

  ret = عارضي_FAILURE_RETRY (اكتب (fd, &نياپو, SYNC_SOCKET_پيغام_LEN (نياپو, err_len + 1)));
  إذا (UNLIKELY (ret < 0))
    أرجع -1;

  أرجع 0;
}

ساكن صحيح
sync_socket_write_error (صحيح fd, libcrun_error_t *ٻاهريون_err)
{
  إذا (fd < 0)
    أرجع 0;
  أرجع sync_socket_write_نياپو (fd, false, (*ٻاهريون_err)->حالت, (*ٻاهريون_err)->نياپو);
}

ساكن فراغ
log_write_منزل_sync_socket (صحيح errno_, ثابت حرف *نياپو, صحيح verbosity, فراغ *arg)
{
  بنية container_entrypoint_s *entrypoint_دليلون = arg;
  صحيح fd = entrypoint_دليلون->sync_socket;

  إذا (fd < 0)
    أرجع;

  إذا (sync_socket_write_نياپو (fd, verbosity, errno_, نياپو) < 0)
    log_write_منزل_stderr (errno_, نياپو, verbosity, arg);
}

ساكن bool
is_يادگيري_حد_بندي_too_هيٺيون (runtime_spec_schema_config_schema *def)
{
  ثابت ڊگهو يادگيري_حد_بندي_too_هيٺيون = 1024 * 1024;

  إذا (def->linux == عدم || def->linux->resources == عدم)
    أرجع false;

  إذا (def->linux->resources->يادگيري
      && def->linux->resources->يادگيري->حد_بندي_present
      && def->linux->resources->يادگيري->حد_بندي < يادگيري_حد_بندي_too_هيٺيون)
    أرجع true;

  إذا (def->linux->resources->unified)
    {
      size_t i;

      لكل (i = 0; i < def->linux->resources->unified->len; i++)
        إذا (strcmp (def->linux->resources->unified->ڪنجيون[i], "memory.max") == 0)
          {
            ڊگهو حد_بندي;

            errno = 0;
            حد_بندي = strtol (def->linux->resources->unified->قدرون[i], عدم, 10);
            إذا (errno != 0)
              أرجع false;
            إذا (حد_بندي < يادگيري_حد_بندي_too_هيٺيون)
              أرجع true;
          }
    }

  أرجع false;
}

ساكن صحيح
sync_socket_wait_sync (libcrun_context_t *context, صحيح fd, bool flush, libcrun_error_t *err)
{
  بنية sync_socket_پيغام_s نياپو;

  إذا (fd < 0)
    أرجع 0;

  طالما (true)
    {
      صحيح ret;

      ret = عارضي_FAILURE_RETRY (اقرأ (fd, &نياپو, حجم (نياپو)));
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

      إذا (! flush && نياپو.type == SYNC_SOCKET_SYNC_پيغام)
        أرجع 0;
      وإلا إذا (نياپو.type == SYNC_SOCKET_DEBUG_پيغام)
        {
          إذا (context)
            context->output_handler (نياپو.error_قدر, نياپو.پيغام, LIBCRUN_VERBOSITY_DEBUG, context->output_handler_arg);
          استمر;
        }
      وإلا إذا (نياپو.type == SYNC_SOCKET_WARNING_پيغام)
        {
          إذا (context)
            context->output_handler (نياپو.error_قدر, نياپو.پيغام, LIBCRUN_VERBOSITY_WARNING, context->output_handler_arg);
          استمر;
        }
      وإلا إذا (نياپو.type == SYNC_SOCKET_ERROR_پيغام)
        أرجع crun_make_error (err, نياپو.error_قدر, "%s", نياپو.پيغام);
    }
}

ساكن صحيح
sync_socket_send_sync (صحيح fd, bool flush_errors, libcrun_error_t *err)
{
  صحيح ret;
  بنية sync_socket_پيغام_s نياپو = {
    0,
  };
  نياپو.type = SYNC_SOCKET_SYNC_پيغام;

  إذا (fd < 0)
    أرجع 0;

  ret = عارضي_FAILURE_RETRY (اكتب (fd, &نياپو, SYNC_SOCKET_پيغام_LEN (نياپو, 0)));
  إذا (UNLIKELY (ret < 0))
    {
      إذا (flush_errors)
        {
          صحيح saved_errno = errno;
          ret = عارضي_FAILURE_RETRY (اقرأ (fd, &نياپو, حجم (نياپو)));
          إذا (ret >= 0 && نياپو.type == SYNC_SOCKET_ERROR_پيغام)
            أرجع crun_make_error (err, نياپو.error_قدر, "%s", نياپو.پيغام);
          errno = saved_errno;
        }
      أرجع crun_make_error (err, errno, "write to sync socket");
    }

  أرجع 0;
}

ساكن bool
has_cap_sys_ptrace ()
{
#جيڪڏهن_وصف_ٿيل HAVE_CAP
  بنية __user_cap_header_struct hdr = { LINUX_CAPABILITY_نسخو_3, 0 };
  بنية __user_cap_data_struct معلومات[2] = { { 0 } };

  إذا (UNLIKELY (capget (&hdr, معلومات) < 0))
    أرجع true;

  أرجع (معلومات[0].effective & (1U << CAP_SYS_PTRACE)) != 0;
#وإلا
  أرجع geteuid () == 0;
#جيڪڏهن_پورو
}

ساكن libcrun_container_t *
make_container (runtime_spec_schema_config_schema *container_def, ثابت حرف *رستو, ثابت حرف *config)
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
        إذا (container_def->linux->namespaces[i]->رستو && container_def->linux->namespaces[i]->رستو[0] == '\0')
          {
            حرر (container_def->linux->namespaces[i]->رستو);
            container_def->linux->namespaces[i]->رستو = عدم;
          }
    }

  إذا (رستو)
    container->config_file = xstrdup (رستو);
  إذا (config)
    container->config_file_content = xstrdup (config);

  أرجع container;
}

libcrun_container_t *
libcrun_container_ڀريو_from_يادگيري (ثابت حرف *json, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free حرف *oci_error = عدم;
  container_def = runtime_spec_schema_config_schema_parse_معلومات (json, عدم, &oci_error);
  إذا (container_def == عدم)
    {
      crun_make_error (err, 0, "load: `%s`", oci_error);
      أرجع عدم;
    }
  أرجع make_container (container_def, عدم, json);
}

libcrun_container_t *
libcrun_container_ڀريو_from_file (ثابت حرف *رستو, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free حرف *oci_error = عدم;
  cleanup_free حرف *content = عدم;
  size_t len;
  صحيح ret;

  libcrun_debug ("Loading container from config file: `%s`", رستو);
  ret = read_all_file (رستو, &content, &len, err);
  إذا (UNLIKELY (ret < 0))
    أرجع عدم;

  container_def = runtime_spec_schema_config_schema_parse_معلومات (content, عدم, &oci_error);
  إذا (container_def == عدم)
    {
      crun_make_error (err, 0, "load `%s`: %s", رستو, oci_error);
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
libcrun_container_get_annotation (libcrun_container_t *container, ثابت حرف *ڪنجي)
{
  إذا (container == عدم)
    أرجع عدم;
  أرجع ڳوليو_string_map_قدر (container->annotations, ڪنجي);
}

size_t
libcrun_container_get_annotations_len (libcrun_container_t *container)
{
  إذا (container == عدم || container->annotations == عدم)
    أرجع 0;
  أرجع string_map_سائيز (container->annotations);
}

صحيح
libcrun_container_get_annotation_at (libcrun_container_t *container, size_t اشاريو, ثابت حرف **ڪنجي,
                                     ثابت حرف **قدر)
{
  إذا (container == عدم)
    {
      errno = ERANGE;
      أرجع -1;
    }
  أرجع string_map_get_at (container->annotations, اشاريو, ڪنجي, قدر);
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

  إذا (ctr->cleanup_ذاتي_معلومات)
    ctr->cleanup_ذاتي_معلومات (ctr->ذاتي_معلومات);

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
libcrun_context_new (ثابت حرف *id, ثابت حرف *state_پاڙ, libcrun_error_t *err)
{
  libcrun_context_t *ctx = xmalloc0 (حجم (*ctx));

  /* Go through the setters so that the owned_* copies and the borrowed
     pointers stay in sync and are released by libcrun_context_free.  */
  libcrun_context_سيٽ_id (ctx, id);
  libcrun_context_سيٽ_state_پاڙ (ctx, state_پاڙ);
  libcrun_context_سيٽ_bundle (ctx, ".");
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

#جيڪڏهن_وصف_ٿيل CRUN_LIBDIR
  إذا (access (CRUN_LIBDIR "/handlers", F_OK) == 0)
    {
      إذا (libcrun_handler_manager_ڀريو_directory (ctx->handler_manager, CRUN_LIBDIR "/handlers", err) < 0)
        {
          libcrun_context_free (ctx);
          أرجع عدم;
        }
    }
#جيڪڏهن_پورو

  أرجع ctx;
}

فراغ
libcrun_context_free (libcrun_context_t *ctx)
{
  إذا (ctx == عدم)
    أرجع;

  حرر (ctx->owned_state_پاڙ);
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

#وصف_ڪريو LIBCRUN_CONTEXT_سيٽ_STRING(ميدان)                  \
  افعل                                                       \
    {                                                      \
      حرر (ctx->owned_##field);                           \
      ctx->owned_##field = قدر ? xstrdup (قدر) : عدم; \
      ctx->ميدان = ctx->owned_##field;                     \
  } طالما (0)

فراغ
libcrun_context_سيٽ_id (libcrun_context_t *ctx, ثابت حرف *قدر)
{
  LIBCRUN_CONTEXT_سيٽ_STRING (id);
}

فراغ
libcrun_context_سيٽ_state_پاڙ (libcrun_context_t *ctx, ثابت حرف *قدر)
{
  LIBCRUN_CONTEXT_سيٽ_STRING (state_پاڙ);
}

فراغ
libcrun_context_سيٽ_bundle (libcrun_context_t *ctx, ثابت حرف *قدر)
{
  LIBCRUN_CONTEXT_سيٽ_STRING (bundle);
}

فراغ
libcrun_context_سيٽ_console_socket (libcrun_context_t *ctx, ثابت حرف *قدر)
{
  LIBCRUN_CONTEXT_سيٽ_STRING (console_socket);
}

فراغ
libcrun_context_سيٽ_pid_file (libcrun_context_t *ctx, ثابت حرف *قدر)
{
  LIBCRUN_CONTEXT_سيٽ_STRING (pid_file);
}

فراغ
libcrun_context_سيٽ_notify_socket (libcrun_context_t *ctx, ثابت حرف *قدر)
{
  LIBCRUN_CONTEXT_سيٽ_STRING (notify_socket);
}

فراغ
libcrun_context_سيٽ_handler (libcrun_context_t *ctx, ثابت حرف *قدر)
{
  LIBCRUN_CONTEXT_سيٽ_STRING (handler);
}

#وصف_رد LIBCRUN_CONTEXT_سيٽ_STRING

ثابت حرف *
libcrun_context_get_pid_file (libcrun_context_t *ctx)
{
  أرجع ctx->pid_file;
}

ثابت حرف *
libcrun_context_get_state_پاڙ (libcrun_context_t *ctx)
{
  إذا (ctx == عدم)
    أرجع عدم;
  أرجع ctx->state_پاڙ;
}

فراغ
libcrun_context_سيٽ_preserve_fds (libcrun_context_t *ctx, صحيح preserve_fds)
{
  ctx->preserve_fds = preserve_fds;
}

صحيح
libcrun_context_get_preserve_fds (libcrun_context_t *ctx)
{
  أرجع ctx->preserve_fds;
}

فراغ
libcrun_context_سيٽ_listen_fds (libcrun_context_t *ctx, صحيح listen_fds)
{
  ctx->listen_fds = listen_fds;
}

فراغ
libcrun_context_سيٽ_دليلون (libcrun_context_t *ctx, صحيح argc, حرف **argv)
{
  ctx->argc = argc;
  ctx->argv = argv;
}

فراغ
libcrun_context_سيٽ_systemd_cgroup (libcrun_context_t *ctx, bool قدر)
{
  ctx->systemd_cgroup = قدر;
}

فراغ
libcrun_context_سيٽ_detach (libcrun_context_t *ctx, bool قدر)
{
  ctx->detach = قدر;
}

فراغ
libcrun_context_سيٽ_no_new_keyring (libcrun_context_t *ctx, bool قدر)
{
  ctx->no_new_keyring = قدر;
}

فراغ
libcrun_context_سيٽ_no_محور (libcrun_context_t *ctx, bool قدر)
{
  ctx->no_محور = قدر;
}

فراغ
libcrun_context_سيٽ_force_no_cgroup (libcrun_context_t *ctx, bool قدر)
{
  ctx->force_no_cgroup = قدر;
}

فراغ
libcrun_context_سيٽ_output_handler (libcrun_context_t *ctx, crun_output_handler handler, فراغ *arg)
{
  ctx->output_handler = handler;
  ctx->output_handler_arg = arg;
}

صحيح
libcrun_context_init_logging (libcrun_context_t *ctx, ثابت حرف *لاگرٿم, libcrun_error_t *err)
{
  أرجع libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, ctx->id, لاگرٿم, err);
}

ساكن صحيح
بلاڪ_سگنل (libcrun_error_t *err)
{
  صحيح ret;
  sigset_t نقاب;
  sigfillset (&نقاب);
  ret = sigprocmask (SIG_بلاڪ, &نقاب, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "sigprocmask");
  أرجع 0;
}

ساكن صحيح
unblock_سگنل (libcrun_error_t *err)
{
  صحيح i;
  صحيح ret;
  sigset_t نقاب;
  بنية sigaction act = {};

  sigfillset (&نقاب);
  ret = sigprocmask (SIG_UNBLOCK, &نقاب, عدم);
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

  GEN_ڪنجي (gen, "annotations");
  GEN_OR_FAIL (json_gen_map_open (gen));

  لكل (i = 0; i < annotations->len; i++)
    {
      /* An annotation whose value is null in the document is parsed into a
         NULL, which cannot be emitted as a JSON string; leave it out.  */
      إذا (annotations->ڪنجيون[i] == عدم || annotations->قدرون[i] == عدم)
        استمر;

      GEN_STR (gen, annotations->ڪنجيون[i]);
      GEN_STR (gen, annotations->قدرون[i]);
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
get_bundle_رستو (ثابت حرف *bundle, حرف **allocated)
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
          ثابت حرف *cwd, ثابت حرف *حالت, ثابت حرف *name, hook **hooks, size_t hooks_len, صحيح ٻاهريون_fd,
          صحيح err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  size_t i, stdin_len;
  صحيح r, ret;
  حرف *stdin = عدم;
  cleanup_free حرف *bundle_allocated = عدم;
  ثابت حرف *rootfs = def->پاڙ ? def->پاڙ->رستو : "";
  cleanup_json_gen json_gen_ctx *gen = عدم;

  bundle = get_bundle_رستو (bundle, &bundle_allocated);

  إذا (! json_gen_init (&gen, عدم))
    أرجع crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 0);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_ڪنجي (gen, "ociVersion");
  GEN_STR (gen, "1.0");

  GEN_ڪنجي (gen, "id");
  GEN_STR (gen, id);

  GEN_ڪنجي (gen, "pid");
  GEN_OR_FAIL (map_int (gen, pid));

  GEN_ڪنجي (gen, "root");
  GEN_STR (gen, rootfs);

  GEN_ڪنجي (gen, "bundle");
  GEN_STR (gen, bundle);

  GEN_ڪنجي (gen, "status");
  GEN_STR (gen, حالت);

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

      ret = لاڳيتو_process_with_stdin_timeout_envp (hooks[i]->رستو, hooks[i]->دليلون, cwd, hooks[i]->timeout, env,
                                                 stdin, stdin_len, ٻاهريون_fd, err_fd, can_ignore_chdir_errors, err);
      إذا (UNLIKELY (ret < 0))
        error_created = true;

      إذا (UNLIKELY (ret != 0))
        {
          إذا (keep_going)
            libcrun_warning ("error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->رستو, ret);
          وإلا
            {
              libcrun_error (0, "error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->رستو, ret);
              اكسر;
            }
        }
    }

  أرجع ret;

gen_error:
  أرجع json_gen_error_منزل_crun_error (r, err);
}

ساكن صحيح
get_json_gen_نتيجو (json_gen_ctx *gen, حرف **ٻاهريون, size_t *ٻاهريون_len)
{
  ثابت حرف *buf = عدم;
  size_t buf_len = 0;
  صحيح r;

  r = json_gen_get_buf (gen, &buf, &buf_len);
  إذا (UNLIKELY (r != json_gen_حالت_ok))
    أرجع r;

  *ٻاهريون_len = buf_len;

  *ٻاهريون = xmalloc (buf_len + 1);
  يادگيري_نقل (*ٻاهريون, buf, buf_len);
  (*ٻاهريون)[buf_len] = '\0';

  أرجع json_gen_حالت_ok;
}

ساكن صحيح
get_seccomp_receiver_fd_payload (libcrun_container_t *container, ثابت حرف *حالت, pid_t own_pid,
                                 حرف **seccomp_fd_payload, size_t *seccomp_fd_payload_len, libcrun_error_t *err)
{
  صحيح r;
  json_gen_ctx *gen = عدم;
  runtime_spec_schema_config_schema *def = container->container_def;
  ثابت حرف *ثابت OCI_نسخو = "0.2.0";

  إذا (! json_gen_init (&gen, عدم))
    أرجع crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_ڪنجي (gen, "ociVersion");
  GEN_STR (gen, OCI_نسخو);

  GEN_ڪنجي (gen, "fds");
  GEN_OR_FAIL (json_gen_array_open (gen));

  GEN_ڪنجي (gen, "seccompFd");
  GEN_OR_FAIL (json_gen_array_close (gen));

  GEN_ڪنجي (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  إذا (def && def->linux && def->linux->seccomp)
    {
      ثابت حرف *metadata = def->linux->seccomp->listener_metadata;

      إذا (metadata)
        {
          GEN_ڪنجي (gen, "metadata");
          GEN_STR (gen, metadata);
        }
    }

  /* State.  */
  GEN_ڪنجي (gen, "state");
  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_ڪنجي (gen, "ociVersion");
  GEN_STR (gen, OCI_نسخو);

  إذا (container->context && container->context->id)
    {
      GEN_ڪنجي (gen, "id");
      GEN_STR (gen, container->context->id);
    }

  GEN_ڪنجي (gen, "status");
  GEN_STR (gen, حالت);

  GEN_ڪنجي (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  إذا (container->context && container->context->bundle)
    {
      GEN_ڪنجي (gen, "bundle");
      GEN_STR (gen, container->context->bundle);
    }

  إذا (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));
  /* End state.  */

  GEN_OR_FAIL (json_gen_map_close (gen));

  r = get_json_gen_نتيجو (gen, seccomp_fd_payload, seccomp_fd_payload_len);

gen_error:
  json_gen_free (gen);

  أرجع json_gen_error_منزل_crun_error (r, err);
}

ساكن صحيح
send_sync_cb (فراغ *معلومات, libcrun_error_t *err)
{
  صحيح sync_socket_fd = *((صحيح *) معلومات);
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
libcrun_container_notify_handler (بنية container_entrypoint_s *دليلون,
                                  تعداد handler_configure_phase phase,
                                  libcrun_container_t *container, ثابت حرف *rootfs,
                                  libcrun_error_t *err)
{
  بنية custom_handler_s *h;

  إذا (دليلون->custom_handler == عدم)
    أرجع 0;

  h = دليلون->custom_handler->vtable;
  إذا (h == عدم || h->configure_container == عدم)
    أرجع 0;

  أرجع h->configure_container (دليلون->custom_handler->cookie, phase,
                                 دليلون->context, container,
                                 rootfs, err);
}

/* Resolve and normalize the container rootfs path.  */
ساكن صحيح
resolve_rootfs_رستو (libcrun_container_t *container, حرف **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  إذا (def->پاڙ && def->پاڙ->رستو)
    {
      *rootfs = realpath (def->پاڙ->رستو, عدم);
      إذا (UNLIKELY (*rootfs == عدم))
        {
          صحيح saved_errno = errno;

          /* If realpath failed for any reason, try the relative directory.  */
          إذا (def->پاڙ->رستو[0] == '/')
            {
              cleanup_free حرف *cwd = عدم;
              libcrun_error_t tmp_err = عدم;
              ssize_t len;
              صحيح proc_fd;

              proc_fd = libcrun_get_cached_proc_fd (container, &tmp_err);
              إذا (LIKELY (proc_fd >= 0))
                {
                  len = محفوظ_readlinkat (proc_fd, "self/cwd", &cwd, 0, &tmp_err);
                  إذا (LIKELY (len > 0))
                    {
                      /* If the rootfs is under the current working directory, just use its relative path.  */
                      إذا (has_dir_اڳياڙي (def->پاڙ->رستو, cwd))
                        {
                          ثابت حرف *it = consume_slashes (def->پاڙ->رستو + len);
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
                أرجع crun_make_error (err, ENOENT, "the rootfs path `%s` does not exist", def->پاڙ->رستو);

              *rootfs = xstrdup (def->پاڙ->رستو);
            }
        }
    }
  أرجع 0;
}

/* Initialize the environment variables.  */
ساكن صحيح
تياري_environment (runtime_spec_schema_config_schema *def, uid_t container_uid, libcrun_error_t *err)
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

  إذا (is_خالي_جايون_string (getenv ("HOME")))
    {
      ret = سيٽ_home_env (container_uid);
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
تياري_terminal (بنية container_entrypoint_s *entrypoint_دليلون, libcrun_container_t *container,
                صحيح console_socket, صحيح console_socketpair, صحيح has_terminal, libcrun_error_t *err)
{
  cleanup_close صحيح terminal_fd = -1;
  صحيح ret;

  إذا (! has_terminal)
    أرجع 0;

  fflush (stderr);

  إذا (console_socket >= 0 || (entrypoint_دليلون->has_terminal_socket_pair && console_socketpair >= 0))
    {
      terminal_fd = libcrun_سيٽ_terminal (container, err);
      إذا (UNLIKELY (terminal_fd < 0))
        أرجع terminal_fd;
    }

  إذا (console_socket >= 0)
    {
      ret = send_fd_منزل_socket (console_socket, terminal_fd, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  وإلا إذا (entrypoint_دليلون->has_terminal_socket_pair && console_socketpair >= 0)
    {
      ret = send_fd_منزل_socket (console_socketpair, terminal_fd, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  أرجع 0;
}

/* Resolve and validate the container's executable path.  */
ساكن صحيح
تياري_executable_رستو (بنية container_entrypoint_s *entrypoint_دليلون, runtime_spec_schema_config_schema *def,
                       حرف **exec_رستو, libcrun_error_t *err)
{
  صحيح ret;

  إذا (def->process && def->process->دليلون)
    {
      ret = ڳوليو_executable (exec_رستو, def->process->دليلون[0], def->process->cwd, err);
      إذا (UNLIKELY (ret < 0))
        {
          إذا (entrypoint_دليلون->custom_handler == عدم && crun_error_get_errno (err) == ENOENT)
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
apply_security_settings (بنية container_entrypoint_s *entrypoint_دليلون, runtime_spec_schema_config_schema *def,
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

      إذا (entrypoint_دليلون->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_دليلون->seccomp_fd, entrypoint_دليلون->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      close_and_reset (&entrypoint_دليلون->seccomp_fd);
      close_and_reset (&entrypoint_دليلون->seccomp_receiver_fd);
    }

  capabilities = def->process ? def->process->capabilities : عدم;
  no_new_privs = def->process ? def->process->no_new_privileges : 1;
  ret = libcrun_سيٽ_caps (capabilities, container->container_uid, container->container_gid, no_new_privs,
                          entrypoint_دليلون->custom_handler != عدم, err);
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
          ret = عارضي_FAILURE_RETRY (اكتب (sync_socket, &pid, حجم (pid)));
          إذا (LIKELY (ret == حجم (pid)))
            محفوظ_exit (EXIT_SUCCESS);
        }

      kill (pid, SIGKILL);
      محفوظ_exit (EXIT_FAILURE);
    }

  /* sync receive own pid.  */
  ret = عارضي_FAILURE_RETRY (اقرأ (sync_socket, own_pid, حجم (*own_pid)));
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
delayed_pidns_fork_cb (فراغ *معلومات, libcrun_error_t *err)
{
  بنية delayed_pidns_fork_s *d = معلومات;

  إذا (! libcrun_container_delayed_pidns_fork (d->container))
    أرجع 0;

  أرجع do_delayed_pidns_fork (d->sync_socket, d->own_pid, err);
}

ساكن صحيح
container_init_تياري (فراغ *دليلون, pid_t *own_pid, حرف *notify_socket,
                      صحيح sync_socket, حرف **exec_رستو, libcrun_error_t *err)
{
  بنية container_entrypoint_s *entrypoint_دليلون = دليلون;
  libcrun_container_t *container = entrypoint_دليلون->container;
  bool chdir_پورو = false;
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

  ret = libcrun_configure_ڄار (container, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = resolve_rootfs_رستو (container, &rootfs, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* Configure terminal socket pair for container communication.  */
  إذا (entrypoint_دليلون->terminal_socketpair[0] >= 0)
    {
      close_and_reset (&entrypoint_دليلون->terminal_socketpair[0]);
      console_socketpair = entrypoint_دليلون->terminal_socketpair[1];
    }

  /* sync 1.  */
  ret = sync_socket_wait_sync (عدم, sync_socket, false, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  has_terminal = container->container_def->process && container->container_def->process->terminal;
  إذا (has_terminal && entrypoint_دليلون->context->console_socket)
    console_socket = entrypoint_دليلون->console_socket_fd;

  ret = libcrun_سيٽ_sysctl (container, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_container_notify_handler (entrypoint_دليلون, HANDLER_CONFIGURE_BEFORE_MOUNTS, container, rootfs, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_do_محور_پاڙ (container, entrypoint_دليلون->context->no_محور, &rootfs, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* sync 2 and 3 are sent as part of libcrun_set_mounts.  */
  ret = libcrun_سيٽ_mounts (entrypoint_دليلون, container, rootfs, send_sync_cb, &sync_socket, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* The createContainer hooks MUST run after the mounts are set up but before
     the pivot_root.  When these hooks are present can_use_open_tree_namespace()
     forces the deferred-pivot path, so the actual pivot happens later in
     libcrun_finalize_mounts().  */
  إذا (def->hooks && def->hooks->create_container_len)
    {
      libcrun_error_t tmp_err = عدم;
      صحيح اندريون_userns = check_running_اندريون_user_namespace (&tmp_err);
      إذا (tmp_err)
        crun_error_release (&tmp_err);
      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_دليلون->bundle, عدم, "created", "createContainer",
                      (hook **) def->hooks->create_container,
                      def->hooks->create_container_len, entrypoint_دليلون->hooks_ٻاهريون_fd, entrypoint_دليلون->hooks_err_fd,
                      اندريون_userns > 0, err);
      إذا (UNLIKELY (ret != 0))
        أرجع ret;
    }

  ret = libcrun_finalize_mounts (entrypoint_دليلون, container, rootfs, delayed_pidns_fork_cb, &delayed_pidns_fork, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (def->process)
    {
      ret = libcrun_سيٽ_selinux_label (container, def->process, false, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = libcrun_سيٽ_apparmor_profile (container, def->process, false, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = mark_or_close_fds_ge_than (container, entrypoint_دليلون->context->preserve_fds + 3, false, err);
  إذا (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ret = libcrun_reopen_dev_null (err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = maybe_chown_std_streams (container->container_uid, container->container_gid, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = تياري_environment (def, container->container_uid, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* Set primary process to 1 explicitly if nothing is configured and LISTEN_FD is not set.  */
  إذا (entrypoint_دليلون->context->listen_fds > 0 && getenv ("LISTEN_PID") == عدم)
    {
      libcrun_warning ("setting LISTEN_PID=1 since no previous configuration was found");
      إذا (UNLIKELY (setenv ("LISTEN_PID", "1", 1) < 0))
        أرجع crun_make_error (err, errno, "setenv LISTEN_PID");
    }

  /* Attempt to chdir immediately here, before doing the setresuid.  If we fail here, let's
     try again later once the process switched to the user that runs in the container.  */
  إذا (def->process && def->process->cwd)
    {
      ret = libcrun_محفوظ_chdir (def->process->cwd, err);
      إذا (LIKELY (ret == 0))
        chdir_پورو = true;
      وإلا
        crun_error_release (err);
    }

  ret = setsid ();
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "setsid");

  ret = تياري_terminal (entrypoint_دليلون, container, console_socket, console_socketpair, has_terminal, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  close_and_reset (&console_socket);
  close_and_reset (&console_socketpair);

  ret = تياري_executable_رستو (entrypoint_دليلون, def, exec_رستو, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_سيٽ_hostname (container, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_سيٽ_domainname (container, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_سيٽ_personality (container->container_def->linux->personality, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (def->process && def->process->user)
    umask (def->process->user->umask_present ? def->process->user->umask : 0022);

  ret = apply_security_settings (entrypoint_دليلون, def, container, *own_pid, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (UNLIKELY (def->process && def->process->دليلون && *exec_رستو == عدم))
    {
      ret = ڳوليو_executable (exec_رستو, def->process->دليلون[0], def->process->cwd, err);
      إذا (UNLIKELY (ret < 0))
        {
          إذا (entrypoint_دليلون->custom_handler == عدم || is_خالي_جايون_string (def->process->دليلون[0]))
            أرجع ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          *exec_رستو = xstrdup (def->process->دليلون[0]);
        }
    }

  /* The chdir was not already performed, so try again now after switching to the UID/GID in the container.  */
  إذا (! chdir_پورو && def->process && def->process->cwd)
    {
      ret = libcrun_محفوظ_chdir (def->process->cwd, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (notify_socket && putenv (notify_socket) < 0)
    أرجع crun_make_error (err, errno, "putenv `%s`", notify_socket);

  أرجع 0;
}

ساكن صحيح
open_hooks_output (libcrun_container_t *container, صحيح *ٻاهريون_fd, صحيح *err_fd, libcrun_error_t *err)
{
  ثابت حرف *annotation;

  *err_fd = *ٻاهريون_fd = -1;

  libcrun_debug ("Opening hooks output");
  annotation = ڳوليو_annotation (container, "run.oci.hooks.stdout");
  إذا (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stdout` annotation");
      *ٻاهريون_fd = عارضي_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      إذا (UNLIKELY (*ٻاهريون_fd < 0))
        أرجع crun_make_error (err, errno, "open `%s`", annotation);
    }

  annotation = ڳوليو_annotation (container, "run.oci.hooks.stderr");
  إذا (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stderr` annotation");
      *err_fd = عارضي_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
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
  حرف *شروعات = argv[argc - 1] + لڙي_ڊيگهه (argv[argc - 1]) + 1;
  حرف *پڇاڙي = (حرف *) getauxval (AT_EXECFN);

  إذا (پڇاڙي > شروعات)
    memset (شروعات, 0, پڇاڙي - شروعات);
}

ساكن فراغ
rewrite_argv (حرف **argv, صحيح argc, ثابت حرف *name, حرف **دليلون, size_t دليلون_len)
{
  cleanup_free حرف *decorated_name = عدم;
  size_t موجود_len = argv[argc - 1] - argv[0] + لڙي_ڊيگهه (argv[argc - 1]) + 1;
  cleanup_free حرف *new_argv = عدم;
  size_t i, so_far = 0, needed = 0;

  needed = xasprintf (&decorated_name, "[libcrun:%s]", name) + 1;
  لكل (i = 0; i < دليلون_len; i++)
    needed += لڙي_ڊيگهه (دليلون[i]) + 1;

  new_argv = xmalloc0 (needed + 1);
  strcpy (new_argv, decorated_name);
  so_far = لڙي_ڊيگهه (decorated_name) + 1;

  إذا (موجود_len >= needed)
    {
      لكل (i = 0; i < دليلون_len; i++)
        {
          strcpy (new_argv + so_far, دليلون[i]);
          so_far += لڙي_ڊيگهه (دليلون[i]) + 1;
        }
    }

  إذا (so_far >= موجود_len)
    so_far = موجود_len - 1;

  memset (argv[0], 0, موجود_len);
  يادگيري_نقل (argv[0], new_argv, so_far);
}

/* Entrypoint to the container.  */
ساكن صحيح
container_init (فراغ *دليلون, حرف *notify_socket, صحيح sync_socket, libcrun_error_t *err)
{
  بنية container_entrypoint_s *entrypoint_دليلون = دليلون;
  صحيح ret;
  runtime_spec_schema_config_schema *def = entrypoint_دليلون->container->container_def;
  cleanup_free حرف *exec_رستو = عدم;
  __attribute__ ((اڻ_استعمال_ٿيل)) cleanup_free حرف *notify_socket_cleanup = notify_socket;
  pid_t own_pid = 0;

  entrypoint_دليلون->sync_socket = sync_socket;

  crun_سيٽ_output_handler (log_write_منزل_sync_socket, دليلون);

  /* sync receive own pid.  */
  ret = عارضي_FAILURE_RETRY (اقرأ (sync_socket, &own_pid, حجم (own_pid)));
  إذا (UNLIKELY (ret != حجم (own_pid)))
    {
      إذا (ret >= 0)
        errno = 0;
      أرجع crun_make_error (err, errno, "read from sync socket");
    }

  ret = container_init_تياري (دليلون, &own_pid, notify_socket, sync_socket, &exec_رستو, err);
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

  entrypoint_دليلون->sync_socket = -1;

  ret = unblock_سگنل (err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* sync 4.  */
  ret = sync_socket_send_sync (sync_socket, false, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  close_and_reset (&sync_socket);

  إذا (entrypoint_دليلون->context->fifo_exec_wait_fd >= 0)
    {
      حرف بفر[1];
      fd_set read_سيٽ;
      cleanup_close صحيح fd = entrypoint_دليلون->context->fifo_exec_wait_fd;
      entrypoint_دليلون->context->fifo_exec_wait_fd = -1;

      FD_ZERO (&read_سيٽ);
      FD_SET (fd, &read_سيٽ);
      افعل
        {
          ret = select (fd + 1, &read_سيٽ, عدم, عدم, عدم);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "select");

          ret = عارضي_FAILURE_RETRY (اقرأ (fd, بفر, حجم (بفر)));
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "read from the exec fifo");
      } طالما (ret == 0);

      close_and_reset (&entrypoint_دليلون->context->fifo_exec_wait_fd);
    }

  crun_سيٽ_output_handler (log_write_منزل_stderr, عدم);

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

      إذا (entrypoint_دليلون->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (entrypoint_دليلون->container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_دليلون->seccomp_fd, entrypoint_دليلون->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags,
                                   seccomp_flags_len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      close_and_reset (&entrypoint_دليلون->seccomp_fd);
      close_and_reset (&entrypoint_دليلون->seccomp_receiver_fd);
    }

  إذا (UNLIKELY (def->process == عدم))
    أرجع crun_make_error (err, 0, "block `process` not found");

  إذا (UNLIKELY (exec_رستو == عدم))
    أرجع crun_make_error (err, 0, "executable path not specified");

  إذا (def->hooks && def->hooks->شروعات_container_len)
    {
      libcrun_container_t *container = entrypoint_دليلون->container;
      libcrun_error_t tmp_err = عدم;
      صحيح اندريون_userns = check_running_اندريون_user_namespace (&tmp_err);
      إذا (tmp_err)
        crun_error_release (&tmp_err);

      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_دليلون->bundle, عدم, "created", "startContainer",
                      (hook **) def->hooks->شروعات_container,
                      def->hooks->شروعات_container_len, entrypoint_دليلون->hooks_ٻاهريون_fd, entrypoint_دليلون->hooks_err_fd,
                      اندريون_userns > 0, err);
      إذا (UNLIKELY (ret != 0))
        أرجع ret;

      /* Seek stdout/stderr to the end.  If the hooks were using the same files,
         the container process overwrites what was previously written.  */
      (فراغ) lseek (1, 0, SEEK_END);
      (فراغ) lseek (2, 0, SEEK_END);
    }

  إذا (entrypoint_دليلون->custom_handler)
    {
      /* Files marked with O_CLOEXEC are closed at execv time, so make sure they are closed now.
         This is a best effort operation, because the seccomp filter is already in place and it could
         stop some syscalls used by mark_or_close_fds_ge_than.
      */
      إذا (entrypoint_دليلون->custom_handler->vtable->close_fds)
        {
          ret = entrypoint_دليلون->custom_handler->vtable->close_fds (entrypoint_دليلون->custom_handler->cookie,
                                                                    entrypoint_دليلون->container,
                                                                    entrypoint_دليلون->context->preserve_fds,
                                                                    err);
        }
      وإلا
        {
          ret = mark_or_close_fds_ge_than (entrypoint_دليلون->container, entrypoint_دليلون->context->preserve_fds + 3, true, err);
        }
      إذا (UNLIKELY (ret < 0))
        crun_error_release (err);

      prctl (PR_سيٽ_NAME, entrypoint_دليلون->custom_handler->vtable->name);

      إذا (entrypoint_دليلون->context->argv)
        {
          erase_initial_environment (entrypoint_دليلون->context->argv, entrypoint_دليلون->context->argc);
          rewrite_argv (entrypoint_دليلون->context->argv, entrypoint_دليلون->context->argc,
                        entrypoint_دليلون->custom_handler->vtable->name, def->process->دليلون,
                        def->process->دليلون_len);

          /* It is a quite destructive operation as we might be referencing data from the old
             argv, so make sure the context is not reused.  */
          entrypoint_دليلون->context->argv = عدم;
          entrypoint_دليلون->context = عدم;
        }

      ret = libcrun_سيٽ_selinux_label (entrypoint_دليلون->container, def->process, true, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = libcrun_سيٽ_apparmor_profile (entrypoint_دليلون->container, def->process, true, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = entrypoint_دليلون->custom_handler->vtable->لاڳيتو_func (entrypoint_دليلون->custom_handler->cookie,
                                                               entrypoint_دليلون->container,
                                                               exec_رستو,
                                                               def->process->دليلون);
      إذا (ret != 0)
        أرجع crun_make_error (err, 0, "exec container process failed with handler as `%s`", entrypoint_دليلون->custom_handler->vtable->name);

      أرجع ret;
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation since the seccomp profile is already in place now and might block
     some of the syscalls needed by mark_or_close_fds_ge_than.  */
  ret = mark_or_close_fds_ge_than (entrypoint_دليلون->container, entrypoint_دليلون->context->preserve_fds + 3, true, err);
  إذا (UNLIKELY (ret < 0))
    crun_error_release (err);

  عارضي_FAILURE_RETRY (execv (exec_رستو, def->process->دليلون));

  إذا (errno == ENOENT)
    أرجع crun_make_error (err, errno, "exec container process (missing dynamic library?) `%s`", exec_رستو);

  أرجع crun_make_error (err, errno, "exec container process `%s`", exec_رستو);
}

ساكن صحيح
read_container_config_from_state (libcrun_container_t **container, ثابت حرف *state_پاڙ, ثابت حرف *id,
                                  libcrun_error_t *err)
{
  cleanup_free حرف *config_file = عدم;
  cleanup_free حرف *dir = عدم;
  صحيح ret;

  *container = عدم;

  ret = libcrun_get_state_directory (&dir, state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = append_paths (&config_file, err, dir, "config.json", عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  *container = libcrun_container_ڀريو_from_file (config_file, err);
  إذا (*container == عدم)
    أرجع -1;

  أرجع 0;
}

ساكن صحيح
لاڳيتو_poststop_hooks (arg_اڻ_استعمال_ٿيل libcrun_context_t *context, libcrun_container_t *container,
                    runtime_spec_schema_config_schema *def,
                    libcrun_container_حالت_t *حالت, ثابت حرف *state_پاڙ, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container_cleanup = عدم;
  صحيح ret;

  إذا (def == عدم)
    {
      إذا (container == عدم)
        {
          ret = read_container_config_from_state (&container_cleanup, state_پاڙ, id, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          container = container_cleanup;
        }

      def = container->container_def;
    }

  إذا (def->hooks && def->hooks->poststop_len)
    {
      cleanup_close صحيح hooks_ٻاهريون_fd = -1;
      cleanup_close صحيح hooks_err_fd = -1;

      إذا (container == عدم)
        {
          ret = read_container_config_from_state (&container_cleanup, state_پاڙ, id, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          container = container_cleanup;
        }

      ret = open_hooks_output (container, &hooks_ٻاهريون_fd, &hooks_err_fd, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = do_hooks (def, 0, id, true, حالت->bundle, حالت->bundle, "stopped", "poststop", (hook **) def->hooks->poststop,
                      def->hooks->poststop_len, hooks_ٻاهريون_fd, hooks_err_fd, false, err);
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
      إذا (strcmp (def->linux->namespaces[i]->type, "pid") == 0 && def->linux->namespaces[i]->رستو == عدم)
        أرجع true;
    }
  أرجع false;
}

/* Delete the container state.  CONTAINER is the container the caller already has
   available, or NULL.  When it is not NULL, the poststop hooks are run even if the
   status file was never written, e.g. when the container creation failed.  */
ساكن صحيح
container_delete_اندروني (libcrun_context_t *context, libcrun_container_t *container,
                           runtime_spec_schema_config_schema *def, ثابت حرف *id, bool force, bool killall,
                           libcrun_error_t *err)
{
  cleanup_cgroup_حالت بنية libcrun_cgroup_حالت *cgroup_حالت = عدم;
  cleanup_container_حالت libcrun_container_حالت_t حالت = {};
  cleanup_container libcrun_container_t *container_cleanup = عدم;
  ثابت حرف *state_پاڙ = context->state_پاڙ;
  صحيح ret;

  ret = libcrun_read_container_حالت (&حالت, state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (force && crun_error_get_errno (err) == ENOENT)
        {
          libcrun_error_t tmp_err = عدم;

          crun_error_release (err);

          إذا (container)
            {
              libcrun_container_حالت_t partial_حالت = {
                .bundle = (حرف *) context->bundle,
              };

              ret = لاڳيتو_poststop_hooks (context, container, def, &partial_حالت, state_پاڙ, id, err);
              إذا (UNLIKELY (ret < 0))
                crun_error_write_warning_and_release (context->output_handler_arg, &err);
            }

          libcrun_container_delete_حالت (state_پاڙ, id, &tmp_err);
          crun_error_release (&tmp_err);
          أرجع 0;
        }
      crun_error_release (err);
      أرجع libcrun_container_delete_حالت (state_پاڙ, id, err);
    }

  cgroup_حالت = libcrun_cgroup_make_حالت (&حالت);

  إذا (! force)
    {
      ret = libcrun_is_container_running (&حالت, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      إذا (ret == 1)
        {
          ret = libcrun_حالت_has_read_exec_fifo (state_پاڙ, id, err);
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
          ret = read_container_config_from_state (&container_cleanup, state_پاڙ, id, err);
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
          ret = libcrun_kill_linux (&حالت, SIGKILL, err);
          إذا (UNLIKELY (ret < 0))
            {
              إذا (crun_error_get_errno (err) != ESRCH)
                أرجع ret;

              crun_error_release (err);
            }
        }
      وإلا إذا (حالت.cgroup_رستو)
        {
          ret = libcrun_cgroup_killall (cgroup_حالت, SIGKILL, err);
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

  إذا (حالت.cgroup_رستو)
    {
      /* If the cgroup cannot be destroyed, there are still processes in it.
         Keep the container status, it is the only record of the cgroup, so
         that the delete can be retried.  */
      ret = libcrun_cgroup_destroy (cgroup_حالت, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = لاڳيتو_poststop_hooks (context, container, def, &حالت, state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  أرجع libcrun_container_delete_حالت (state_پاڙ, id, err);
}

صحيح
libcrun_container_delete (libcrun_context_t *context, ثابت حرف *id, bool force, libcrun_error_t *err)
{
  أرجع container_delete_اندروني (context, عدم, عدم, id, force, true, err);
}

صحيح
libcrun_container_kill (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *إشارة, libcrun_error_t *err)
{
  صحيح sig, ret;
  ثابت حرف *state_پاڙ = context->state_پاڙ;
  cleanup_container_حالت libcrun_container_حالت_t حالت = {};

  sig = str2sig (إشارة);
  إذا (UNLIKELY (sig < 0))
    أرجع crun_make_error (err, 0, "unknown signal `%s`", إشارة);

  ret = libcrun_read_container_حالت (&حالت, state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع libcrun_kill_linux (&حالت, sig, err);
}

صحيح
libcrun_container_killall (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *إشارة, libcrun_error_t *err)
{
  صحيح sig, ret;
  ثابت حرف *state_پاڙ = context->state_پاڙ;
  cleanup_container_حالت libcrun_container_حالت_t حالت = {};
  cleanup_cgroup_حالت بنية libcrun_cgroup_حالت *cgroup_حالت = عدم;

  sig = str2sig (إشارة);
  إذا (UNLIKELY (sig < 0))
    أرجع crun_make_error (err, 0, "unknown signal `%s`", إشارة);

  ret = libcrun_read_container_حالت (&حالت, state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  cgroup_حالت = libcrun_cgroup_make_حالت (&حالت);

  ret = libcrun_cgroup_killall (cgroup_حالت, sig, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;
  أرجع 0;
}

ساكن صحيح
write_container_حالت (libcrun_container_t *container, libcrun_context_t *context,
                        pid_t pid, بنية libcrun_cgroup_حالت *cgroup_حالت,
                        libcrun_error_t *err)
{
  cleanup_free حرف *bundle_allocated = عدم;
  ثابت حرف *bundle = get_bundle_رستو (context->bundle, &bundle_allocated);
  cleanup_free حرف *owner = get_user_name (geteuid ());
  حرف *external_descriptors = libcrun_get_external_descriptors (container);
  حرف *rootfs = container->container_def->پاڙ ? container->container_def->پاڙ->رستو : "";
  حرف created[35];

  libcrun_container_حالت_t حالت = {
    .pid = pid,
    .rootfs = rootfs,
    .bundle = (حرف *) bundle,
    .created = created,
    .owner = owner,
    .systemd_cgroup = context->systemd_cgroup,
    .detached = context->detach,
    .no_new_keyring = context->no_new_keyring,
    .external_descriptors = external_descriptors,
    .cgroup_رستو = عدم,
    .scope = عدم,
  };

  get_current_timestamp (created, حجم (created));

  إذا (cgroup_حالت)
    {
      صحيح ret;

      ret = libcrun_cgroup_get_حالت (cgroup_حالت, &حالت, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (external_descriptors == عدم)
    أرجع crun_make_error (err, 0, "invalid internal state.  No external descriptors found");
  أرجع libcrun_write_container_حالت (context->state_پاڙ, context->id, &حالت, err);
}

ساكن صحيح
reap_subprocesses (pid_t *main_process, صحيح *main_process_exit, صحيح *آخري_process, libcrun_error_t *err)
{
  *آخري_process = 0;
  طالما (1)
    {
      صحيح حالت;
      صحيح r = waitpid_ignore_stopped (-1, &حالت, WNOHANG);
      إذا (r < 0)
        {
          إذا (errno == EINTR)
            استمر;
          إذا (errno == ECHILD)
            {
              *آخري_process = 1;
              أرجع 0;
            }
          أرجع crun_make_error (err, errno, "waitpid");
        }
      إذا (r == 0)
        اكسر;
      إذا (r != *main_process)
        استمر;

      *main_process_exit = get_process_exit_حالت (حالت);

      /* Once reaped, the pid can be reused by an unrelated process.  */
      *main_process = 0;
    }
  أرجع 0;
}

ساكن صحيح
send_sd_notify (ثابت حرف *ready_str, libcrun_error_t *err)
{
#جيڪڏهن_وصف_ٿيل HAVE_SYSTEMD
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
#جيڪڏهن_پورو
  أرجع 0;
}

ساكن صحيح
handle_notify_socket (صحيح notify_socketfd, libcrun_error_t *err)
{
#جيڪڏهن_وصف_ٿيل HAVE_SYSTEMD
  صحيح ret;
  حرف buf[256];
  ثابت حرف *ready_str = "READY=1";

  ret = recvfrom (notify_socketfd, buf, حجم (buf) - 1, 0, عدم, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "recvfrom notify socket");

  buf[ret] = '\0';
  إذا (لڙي_ڳولا (buf, ready_str))
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
#جيڪڏهن_پورو
}

بنية wait_for_process_دليلون
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

#وصف_ڪريو MAX_EVENTS 10

ساكن bool
is_fd_pollable (صحيح fd)
{
  بنية epoll_event ev = { .events = EPOLLIN };
  cleanup_close صحيح epollfd = epoll_create1 (EPOLL_CLOEXEC);

  إذا (epollfd < 0)
    أرجع true;

  أرجع epoll_ctl (epollfd, EPOLL_CTL_جوڙيو, fd, &ev) == 0 || errno != EPERM;
}

ساكن صحيح
wait_for_process (بنية wait_for_process_دليلون *دليلون, libcrun_error_t *err)
{
  cleanup_channel_fd_pair بنية channel_fd_pair *from_terminal = عدم;
  cleanup_channel_fd_pair بنية channel_fd_pair *منزل_terminal = عدم;
  صحيح ret, container_exit_ڪوڊ = 0, آخري_process;
  cleanup_close صحيح terminal_fd_from = -1;
  cleanup_close صحيح terminal_fd_منزل = -1;
  bool stdin_pollable = false, stdout_pollable = false;
  cleanup_close صحيح epollfd = -1;
  cleanup_close صحيح signalfd = -1;
  sigset_t نقاب;
  صحيح اندريون_fds[MAX_EVENTS];
  صحيح اندريون_fds_len = 0;
  صحيح ٻاهريون_fds[MAX_EVENTS];
  صحيح ٻاهريون_fds_len = 0;
  size_t i;

  cleanup_seccomp_notify_context بنية seccomp_notify_context_s *seccomp_notify_ctx = عدم;

  container_exit_ڪوڊ = 0;

  إذا (دليلون == عدم || دليلون->context == عدم)
    أرجع crun_make_error (err, 0, "internal error: context is empty");

  لكل (i = 0; i < MAX_EVENTS; i++)
    {
      اندريون_fds[i] = -1;
      ٻاهريون_fds[i] = -1;
    }

  إذا (دليلون->context->pid_file)
    {
      ret = write_pid_file (دليلون->context->pid_file, دليلون->pid, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  /* Also exit if there is nothing more to wait for.  */
  إذا (دليلون->context->detach && دليلون->notify_socket < 0)
    أرجع 0;

  إذا (دليلون->container_ready_fd)
    {
      ret = 0;
      عارضي_FAILURE_RETRY (اكتب (*دليلون->container_ready_fd, &ret, حجم (ret)));
      close_and_reset (دليلون->container_ready_fd);
    }

  sigfillset (&نقاب);
  ret = sigprocmask (SIG_بلاڪ, &نقاب, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "sigprocmask");

  signalfd = create_signalfd (&نقاب, err);
  إذا (UNLIKELY (signalfd < 0))
    أرجع signalfd;

  ret = reap_subprocesses (&دليلون->pid, &container_exit_ڪوڊ, &آخري_process, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (آخري_process)
    أرجع container_exit_ڪوڊ;

  إذا (دليلون->seccomp_notify_fd >= 0)
    {
      cleanup_free حرف *state_پاڙ = عدم;
      cleanup_free حرف *oci_config_رستو = عدم;

      بنية libcrun_ڀريو_seccomp_notify_conf_s conf;
      memset (&conf, 0, حجم conf);

      ret = libcrun_get_state_directory (&state_پاڙ, دليلون->context->state_پاڙ, دليلون->context->id, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = append_paths (&oci_config_رستو, err, state_پاڙ, "config.json", عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      conf.runtime_پاڙ_رستو = state_پاڙ;
      conf.name = دليلون->context->id;
      conf.bundle_رستو = دليلون->context->bundle;
      conf.oci_config_رستو = oci_config_رستو;

      ret = سيٽ_blocking_fd (دليلون->seccomp_notify_fd, false, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = libcrun_ڀريو_seccomp_notify_plugins (&seccomp_notify_ctx,
                                                 دليلون->seccomp_notify_plugins,
                                                 &conf, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      اندريون_fds[اندريون_fds_len++] = دليلون->seccomp_notify_fd;
    }

  إذا (دليلون->terminal_fd >= 0)
    {
      /* The terminal_fd is dup()ed so that it can be registered with
         epoll multiple times using different masks.  */
      terminal_fd_from = dup (دليلون->terminal_fd);
      إذا (UNLIKELY (terminal_fd_from < 0))
        أرجع crun_make_error (err, errno, "dup terminal fd");
      terminal_fd_منزل = dup (دليلون->terminal_fd);
      إذا (UNLIKELY (terminal_fd_منزل < 0))
        أرجع crun_make_error (err, errno, "dup terminal fd");

      صحيح i, non_blocking_fds[] = { terminal_fd_from, terminal_fd_منزل, 0, 1, -1 };
      لكل (i = 0; non_blocking_fds[i] >= 0; i++)
        {
          ret = سيٽ_blocking_fd (non_blocking_fds[i], false, err);
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
        منزل_terminal = channel_fd_pair_new (0, terminal_fd_منزل, BUFSIZ);
    }

  اندريون_fds[اندريون_fds_len++] = signalfd;
  إذا (دليلون->notify_socket >= 0)
    اندريون_fds[اندريون_fds_len++] = دليلون->notify_socket;
  إذا (دليلون->terminal_fd >= 0)
    {
      إذا (stdin_pollable)
        {
          اندريون_fds[اندريون_fds_len++] = 0;
          ٻاهريون_fds[ٻاهريون_fds_len++] = terminal_fd_منزل;
        }

      اندريون_fds[اندريون_fds_len++] = terminal_fd_from;
      إذا (stdout_pollable)
        ٻاهريون_fds[ٻاهريون_fds_len++] = 1;
    }

  epollfd = epoll_helper (اندريون_fds, عدم, ٻاهريون_fds, عدم, err);
  إذا (UNLIKELY (epollfd < 0))
    أرجع epollfd;

  طالما (1)
    {
      بنية epoll_event events[MAX_EVENTS];
      بنية signalfd_siginfo si;
      بنية winsize ws;
      صحيح i, nr_events;
      ssize_t res;

      nr_events = عارضي_FAILURE_RETRY (epoll_wait (epollfd, events, MAX_EVENTS, -1));
      إذا (UNLIKELY (nr_events < 0))
        أرجع crun_make_error (err, errno, "epoll_wait");

      لكل (i = 0; i < nr_events; i++)
        {
          إذا (events[i].معلومات.fd == 0 || events[i].معلومات.fd == terminal_fd_منزل)
            {
              ret = channel_fd_pair_process (منزل_terminal, epollfd, err);
              إذا (UNLIKELY (ret < 0))
                أرجع crun_error_wrap (err, "copy to terminal fd");
            }
          وإلا إذا (events[i].معلومات.fd == 1 || events[i].معلومات.fd == terminal_fd_from)
            {
              ret = channel_fd_pair_process (from_terminal, epollfd, err);
              إذا (UNLIKELY (ret < 0))
                أرجع crun_error_wrap (err, "copy from terminal fd");
            }
          وإلا إذا (events[i].معلومات.fd == دليلون->seccomp_notify_fd)
            {
              ret = libcrun_seccomp_notify_plugins (seccomp_notify_ctx,
                                                    دليلون->seccomp_notify_fd, err);
              إذا (UNLIKELY (ret < 0))
                أرجع ret;
            }
          وإلا إذا (events[i].معلومات.fd == دليلون->notify_socket)
            {
              ret = handle_notify_socket (دليلون->notify_socket, err);
              إذا (UNLIKELY (ret < 0))
                أرجع ret;
              إذا (ret && دليلون->context->detach)
                أرجع 0;
            }
          وإلا إذا (events[i].معلومات.fd == signalfd)
            {
              res = عارضي_FAILURE_RETRY (اقرأ (signalfd, &si, حجم (si)));
              إذا (UNLIKELY (res < 0))
                أرجع crun_make_error (err, errno, "read from signalfd");
              إذا (si.ssi_signo == SIGCHLD)
                {
                  ret = reap_subprocesses (&دليلون->pid, &container_exit_ڪوڊ,
                                           &آخري_process, err);
                  إذا (UNLIKELY (ret < 0))
                    أرجع ret;
                  إذا (آخري_process)
                    أرجع container_exit_ڪوڊ;
                }
              وإلا إذا (si.ssi_signo == SIGWINCH)
                {
                  /* Ignore the signal if the terminal is not available.  */
                  إذا (دليلون->terminal_fd > 0)
                    {
                      ret = ioctl (0, TIOCGWINSZ, &ws);
                      إذا (UNLIKELY (ret < 0))
                        أرجع crun_make_error (err, errno, "ioctl TIOCGWINSZ copy terminal size from stdin");

                      ret = ioctl (دليلون->terminal_fd, TIOCSWINSZ, &ws);
                      إذا (UNLIKELY (ret < 0))
                        أرجع crun_make_error (err, errno, "ioctl TIOCSWINSZ copy terminal size to pty");
                    }
                }
              وإلا إذا (دليلون->pid > 0)
                {
                  /* Send any other signal to the child process, if it was not reaped yet.  */
                  ret = kill (دليلون->pid, si.ssi_signo);
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
flush_fd_منزل_err (libcrun_context_t *context, صحيح terminal_fd)
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
      صحيح ret = عارضي_FAILURE_RETRY (اقرأ (terminal_fd, buf, حجم (buf) - 1));
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
               بنية libcrun_cgroup_حالت *cgroup_حالت, pid_t init_pid, صحيح sync_socket,
               صحيح terminal_fd, libcrun_error_t *err)
{
  ثابت حرف *oom_پيغام = عدم;
  libcrun_error_t tmp_err = عدم;
  صحيح ret;

  إذا (init_pid)
    {
      /* Try to detect whether the cgroup has a OOM.  */
      إذا (cgroup_حالت)
        {
          صحيح has_oom;

          has_oom = libcrun_cgroup_has_oom (cgroup_حالت, &tmp_err);
          إذا (has_oom > 0)
            oom_پيغام = "OOM: the memory limit could be too low";
          وإلا إذا (has_oom < 0)
            {
              /* If the detection has failed for any reason, e.g. the cgroup was
                 already deleted by the time it was checked, just ignore the
                 failure.  */
              crun_error_release (&tmp_err);
            }
        }
      /* If the OOM wasn't detected, look into the static configuration.  */
      إذا (oom_پيغام == عدم && is_يادگيري_حد_بندي_too_هيٺيون (def))
        oom_پيغام = "the memory limit could be too low";

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
    flush_fd_منزل_err (context, terminal_fd);

  إذا (oom_پيغام)
    أرجع crun_error_wrap (err, "%s", oom_پيغام);

  أرجع -1;
}

/* Find the uid:gid that is mapped to root inside the container user namespace.  */
فراغ
get_پاڙ_اندريون_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid, uid_t *uid,
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

  tmp = ڳوليو_annotation (container, "run.oci.seccomp.plugins");
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

  إذا (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_رستو)
    tmp = def->linux->seccomp->listener_رستو;
  وإلا
    tmp = ڳوليو_annotation (container, "run.oci.seccomp.receiver");
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

  إذا (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_رستو)
    أرجع true;

  إذا (ڳوليو_annotation (container, "run.oci.seccomp.receiver") != عدم || getenv ("RUN_OCI_SECCOMP_RECEIVER") != عدم)
    أرجع true;

  أرجع false;
}

ساكن صحيح
تياري_container_hooks_output (libcrun_container_t *container, runtime_spec_schema_config_schema *def,
                              بنية container_entrypoint_s *container_دليلون, صحيح *hooks_ٻاهريون_fd,
                              صحيح *hooks_err_fd, libcrun_error_t *err)
{
  صحيح ret;

  إذا (def->hooks
      && (def->hooks->prestart_len || def->hooks->poststart_len || def->hooks->create_runtime_len
          || def->hooks->create_container_len || def->hooks->شروعات_container_len))
    {
      ret = open_hooks_output (container, hooks_ٻاهريون_fd, hooks_err_fd, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      container_دليلون->hooks_ٻاهريون_fd = *hooks_ٻاهريون_fd;
      container_دليلون->hooks_err_fd = *hooks_err_fd;
    }
  أرجع 0;
}

ساكن صحيح
تياري_container_keyring (libcrun_container_t *container, libcrun_context_t *context, libcrun_error_t *err)
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
تياري_terminal_socket_pair (libcrun_container_t *container, libcrun_context_t *context,
                            بنية container_entrypoint_s *container_دليلون, صحيح *socket_pair_0,
                            صحيح *socket_pair_1, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح ret;
  صحيح detach = context->detach;

  إذا (def->process && def->process->terminal && ! detach && context->console_socket == عدم)
    {
      libcrun_debug ("Creating terminal socket pair");
      container_دليلون->has_terminal_socket_pair = 1;
      ret = create_socket_pair (container_دليلون->terminal_socketpair, err);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_error_wrap (err, "create terminal socket");

      *socket_pair_0 = container_دليلون->terminal_socketpair[0];
      *socket_pair_1 = container_دليلون->terminal_socketpair[1];
    }
  أرجع 0;
}

ساكن صحيح
تياري_seccomp (libcrun_container_t *container, ثابت حرف *seccomp_bpf_معلومات,
               بنية libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, صحيح *seccomp_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح ret;

  إذا (ڳوليو_annotation (container, "run.oci.seccomp.plugins") != عدم && has_seccomp_receiver (container))
    {
      أرجع crun_make_error (err, 0, "seccomp plugins and seccomp receivers cannot be declared at the same time");
    }

  إذا (def->linux && (def->linux->seccomp || seccomp_bpf_معلومات))
    {
      بنانشان صحيح seccomp_gen_options = 0;
      ثابت حرف *annotation;

      libcrun_debug ("Initializing seccomp");
      annotation = ڳوليو_annotation (container, "run.oci.seccomp_fail_unknown_syscall");
      إذا (annotation && strcmp (annotation, "0") != 0)
        seccomp_gen_options = LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL;

      إذا (seccomp_bpf_معلومات)
        seccomp_gen_options |= LIBCRUN_SECCOMP_SKIP_CACHE;

      libcrun_seccomp_gen_ctx_init (seccomp_gen_ctx, container, true, seccomp_gen_options);

      ret = libcrun_open_seccomp_bpf (seccomp_gen_ctx, seccomp_fd, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  أرجع 0;
}

ساكن صحيح
تياري_console_socket (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
                      بنية container_entrypoint_s *container_دليلون, صحيح *console_socket_fd,
                      libcrun_error_t *err)
{
  إذا (context->console_socket && def->process && def->process->terminal)
    {
      *console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
      إذا (UNLIKELY (*console_socket_fd < 0))
        أرجع crun_error_wrap (err, "open console socket");
      container_دليلون->console_socket_fd = *console_socket_fd;
    }
  أرجع 0;
}

ساكن صحيح
تياري_cgroup_manager (libcrun_context_t *context, libcrun_container_t *container,
                      بنية libcrun_cgroup_دليلون *cg, صحيح *cgroup_dirfd,
                      بنية libcrun_dirfd_s *cgroup_dirfd_s, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح cgroup_manager;
  uid_t پاڙ_uid = -1;
  gid_t پاڙ_gid = -1;
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
  get_پاڙ_اندريون_the_userns (def, container->host_uid, container->host_gid, &پاڙ_uid, &پاڙ_gid);
  libcrun_debug ("Using container host UID `%d` and GID `%d`", container->host_uid, container->host_gid);

  memset (cg, 0, حجم (*cg));

  cg->cgroup_رستو = def->linux ? def->linux->cgroups_رستو : "";
  cg->manager = cgroup_manager;
  cg->id = context->id;
  cg->resources = def->linux ? def->linux->resources : عدم;
  cg->annotations = container->annotations;
  cg->پاڙ_uid = پاڙ_uid;
  cg->پاڙ_gid = پاڙ_gid;
  cg->state_پاڙ = context->state_پاڙ;
  cg->container = container;

  ret = libcrun_cgroup_preenter (cg, cgroup_dirfd, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  cgroup_dirfd_s->dirfd = cgroup_dirfd;
  cgroup_dirfd_s->joined = false;

  أرجع 0;
}

ساكن صحيح
سيٽ_scheduler (pid_t pid, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  صحيح ret;

  ret = libcrun_سيٽ_scheduler (pid, def->process, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_reset_cpu_affinity_نقاب (pid, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_سيٽ_io_ترجيح (pid, def->process, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع 0;
}

ساكن صحيح
seccomp_generation (صحيح seccomp_fd, ثابت حرف *seccomp_bpf_معلومات,
                    بنية libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, libcrun_error_t *err)
{
  صحيح ret;

  إذا (seccomp_fd >= 0)
    {
      إذا (seccomp_bpf_معلومات != عدم)
        {
          ret = libcrun_copy_seccomp (seccomp_gen_ctx, seccomp_bpf_معلومات, err);
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
terminal_تياري (runtime_spec_schema_config_schema *def, libcrun_context_t *context,
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

      ret = libcrun_سيٽ_raw (0, orig_terminal, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  أرجع 0;
}

ساكن صحيح
libcrun_container_لاڳيتو_اندروني (libcrun_container_t *container, libcrun_context_t *context,
                                صحيح *container_ready_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح ret;
  pid_t pid;
  صحيح detach = context->detach;
  cleanup_cgroup_حالت بنية libcrun_cgroup_حالت *cgroup_حالت = عدم;
  cleanup_close صحيح terminal_fd = -1;
  cleanup_terminal فراغ *orig_terminal = عدم;
  cleanup_close صحيح sync_socket = -1;
  cleanup_close صحيح notify_socket = -1;
  cleanup_close صحيح socket_pair_0 = -1;
  cleanup_close صحيح socket_pair_1 = -1;
  cleanup_close صحيح seccomp_fd = -1;
  cleanup_close صحيح console_socket_fd = -1;
  cleanup_close صحيح hooks_ٻاهريون_fd = -1;
  cleanup_close صحيح hooks_err_fd = -1;
  cleanup_close صحيح own_seccomp_receiver_fd = -1;
  cleanup_close صحيح seccomp_notify_fd = -1;
  ثابت حرف *seccomp_notify_plugins = عدم;
  cleanup_cgroup_preenter بنية libcrun_cgroup_دليلون cg = {};
  cleanup_free حرف *bundle_allocated = عدم;
  بنية container_entrypoint_s container_دليلون = {
    .container = container,
    .context = context,
    .bundle = get_bundle_رستو (context->bundle, &bundle_allocated),
    .terminal_socketpair = { -1, -1 },
    .console_socket_fd = -1,
    .hooks_ٻاهريون_fd = -1,
    .hooks_err_fd = -1,
    .seccomp_receiver_fd = -1,
    .custom_handler = عدم,
  };
  cleanup_close صحيح cgroup_dirfd = -1;
  بنية libcrun_dirfd_s cgroup_dirfd_s;
  بنية libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  ثابت حرف *seccomp_bpf_معلومات = ڳوليو_annotation (container, "run.oci.seccomp_bpf_data");
  صحيح cgroup_گهڻائي;

  إذا (! context->force_no_cgroup)
    {
      cgroup_گهڻائي = libcrun_get_cgroup_گهڻائي (err);
      إذا (UNLIKELY (cgroup_گهڻائي < 0))
        أرجع cgroup_گهڻائي;

      إذا (cgroup_گهڻائي != CGROUP_گهڻائي_UNIFIED)
        libcrun_warning ("cgroup v1 is deprecated and will be removed in a future release.  Use cgroup v2");
    }

  ret = تياري_container_hooks_output (container, def, &container_دليلون, &hooks_ٻاهريون_fd, &hooks_err_fd, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  container->context = context;

  إذا (! detach || context->notify_socket)
    {
      libcrun_debug ("Setting child subreaper");
      ret = prctl (PR_سيٽ_ٻار_SUBREAPER, 1, 0, 0, 0);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "prctl set child subreaper");
    }

  ret = تياري_container_keyring (container, context, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = تياري_terminal_socket_pair (container, context, &container_دليلون, &socket_pair_0, &socket_pair_1, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = بلاڪ_سگنل (err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  umask (0);

  ret = libcrun_سيٽ_mempolicy (def, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = تياري_seccomp (container, seccomp_bpf_معلومات, &seccomp_gen_ctx, &seccomp_fd, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;
  container_دليلون.seccomp_fd = seccomp_fd;

  إذا (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &container_دليلون.seccomp_receiver_fd, &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = تياري_console_socket (context, def, &container_دليلون, &console_socket_fd, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = تياري_cgroup_manager (context, container, &cg, &cgroup_dirfd, &cgroup_dirfd_s, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_configure_handler (container_دليلون.context->handler_manager,
                                   container_دليلون.context,
                                   container,
                                   &(container_دليلون.custom_handler),
                                   err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (container_دليلون.custom_handler && container_دليلون.custom_handler->vtable->modify_oci_configuration)
    {
      libcrun_debug ("Using custom handler to modify OCI configuration");
      ret = container_دليلون.custom_handler->vtable->modify_oci_configuration (container_دليلون.custom_handler->cookie,
                                                                             container_دليلون.context,
                                                                             container->container_def,
                                                                             err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  pid = libcrun_لاڳيتو_linux_container (container, container_init, &container_دليلون, &sync_socket, &cgroup_dirfd_s, err);
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

  إذا (container_دليلون.terminal_socketpair[1] >= 0)
    close_and_reset (&socket_pair_1);

  ret = libcrun_cgroup_enter (&cg, &cgroup_حالت, err);
  إذا (UNLIKELY (ret < 0))
    اذهب fail;

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE, err);
  إذا (UNLIKELY (ret < 0))
    اذهب fail;

  ret = libcrun_move_ڄار_devices (container, pid, err);
  إذا (UNLIKELY (ret < 0))
    اذهب fail;

  /* sync send own pid.  */
  ret = عارضي_FAILURE_RETRY (اكتب (sync_socket, &pid, حجم (pid)));
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

  ret = libcrun_cgroup_enter_finalize (&cg, cgroup_حالت, err);
  إذا (UNLIKELY (ret < 0))
    اذهب fail;

  /* If the container process is created later, the scheduler is set on it then.  */
  إذا (! libcrun_container_delayed_pidns_fork (container))
    {
      ret = سيٽ_scheduler (pid, def, err);
      إذا (UNLIKELY (ret < 0))
        اذهب fail;
    }

  /* The container is waiting that we write back.  In this phase we can launch the
     prestart hooks.  */
  إذا (def->hooks && def->hooks->prestart_len)
    {
      libcrun_debug ("Running `prestart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "prestart", (hook **) def->hooks->prestart,
                      def->hooks->prestart_len, hooks_ٻاهريون_fd, hooks_err_fd, false, err);
      إذا (UNLIKELY (ret != 0))
        اذهب fail;
    }
  إذا (def->hooks && def->hooks->create_runtime_len)
    {
      libcrun_debug ("Running `create` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "createRuntime", (hook **) def->hooks->create_runtime,
                      def->hooks->create_runtime_len, hooks_ٻاهريون_fd, hooks_err_fd, false, err);
      إذا (UNLIKELY (ret != 0))
        اذهب fail;
    }

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_معلومات, &seccomp_gen_ctx, err);
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

      ret = عارضي_FAILURE_RETRY (اقرأ (sync_socket, &new_pid, حجم (new_pid)));
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

      ret = سيٽ_scheduler (pid, def, err);
      إذا (UNLIKELY (ret < 0))
        اذهب fail;

      ret = libcrun_handle_pidfd_receiver (pid, container, err);
      إذا (UNLIKELY (ret < 0))
        اذهب fail;

      /* sync send own pid.  */
      ret = عارضي_FAILURE_RETRY (اكتب (sync_socket, &pid, حجم (pid)));
      إذا (UNLIKELY (ret != حجم (pid)))
        {
          إذا (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "write to sync socket");
          اذهب fail;
        }
    }

  ret = terminal_تياري (def, context, socket_pair_0, &terminal_fd, &orig_terminal, err);
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
  ret = write_container_حالت (container, context, pid, cgroup_حالت, err);
  إذا (UNLIKELY (ret < 0))
    اذهب fail;

  /* Run poststart hooks here only if the container is created using "run".  For create+start, the
     hooks will be executed as part of the start command.  */
  إذا (context->fifo_exec_wait_fd < 0 && def->hooks && def->hooks->poststart_len)
    {
      libcrun_debug ("Running `poststart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_ٻاهريون_fd, hooks_err_fd, false, err);
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
    بنية wait_for_process_دليلون دليلون = {
      .pid = pid,
      .context = context,
      .terminal_fd = terminal_fd,
      .notify_socket = notify_socket,
      .container_ready_fd = container_ready_fd,
      .seccomp_notify_fd = seccomp_notify_fd,
      .seccomp_notify_plugins = seccomp_notify_plugins,
    };
    ret = wait_for_process (&دليلون, err);
  }
  إذا (! context->detach)
    {
      libcrun_error_t tmp_err = عدم;
      cleanup_watch (context, def, cgroup_حالت, 0, sync_socket, terminal_fd, &tmp_err);
      crun_error_release (&tmp_err);
    }

  أرجع ret;

fail:
  ret = cleanup_watch (context, def, cgroup_حالت, pid, sync_socket, terminal_fd, err);
  إذا (cgroup_حالت)
    {
      libcrun_error_t tmp_err = عدم;
      libcrun_cgroup_destroy (cgroup_حالت, &tmp_err);
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
      إذا (UNLIKELY (def->پاڙ == عدم))
        أرجع crun_make_error (err, 0, "invalid config file, no `root` block specified");
      إذا (UNLIKELY (def->mounts == عدم))
        أرجع crun_make_error (err, 0, "invalid config file, no `mounts` block specified");
    }
  أرجع 0;
}

ساكن صحيح
libcrun_copy_config_file (ثابت حرف *id, ثابت حرف *state_پاڙ, libcrun_container_t *container, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_free حرف *dest_رستو = عدم;
  cleanup_free حرف *dir = عدم;
  cleanup_free حرف *بفر = عدم;
  size_t len;

  ret = libcrun_get_state_directory (&dir, state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = append_paths (&dest_رستو, err, dir, "config.json", عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (container->config_file == عدم && container->config_file_content == عدم)
    أرجع crun_make_error (err, 0, "config file not specified");

  إذا (container->config_file == عدم)
    {
      libcrun_debug ("Writing config file to: `%s`", dest_رستو);
      ret = write_file (dest_رستو, container->config_file_content, لڙي_ڊيگهه (container->config_file_content), err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  وإلا
    {
      libcrun_debug ("Reading config file: `%s`", container->config_file);
      ret = read_all_file (container->config_file, &بفر, &len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      libcrun_debug ("Writing config file to: `%s`", dest_رستو);
      ret = write_file (dest_رستو, بفر, len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  أرجع 0;
}

ساكن فراغ
force_delete_container_حالت (libcrun_context_t *context, libcrun_container_t *container,
                               runtime_spec_schema_config_schema *def)
{
  libcrun_error_t tmp_err = عدم;
  container_delete_اندروني (context, container, def, context->id, true, false, &tmp_err);
  libcrun_error_report_and_release (&tmp_err);
}

/* Serialize ERR to FD as an int errno followed by a NUL-terminated message.
   Used by forked children to report a failure to the parent process.  */
ساكن فراغ
write_error_منزل_pipe (صحيح fd, libcrun_error_t *err)
{
  ثابت حرف *نياپو = (*err)->نياپو;
  صحيح errcode = crun_error_get_errno (err);

  عارضي_FAILURE_RETRY (اكتب (fd, &errcode, حجم (errcode)));
  عارضي_FAILURE_RETRY (اكتب (fd, نياپو, لڙي_ڊيگهه (نياپو) + 1));
}

صحيح
libcrun_container_لاڳيتو (libcrun_context_t *context, libcrun_container_t *container, بنانشان صحيح options,
                       libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح ret;
  صحيح detach = context->detach;
  صحيح container_ret_حالت[2];
  cleanup_close صحيح pipefd0 = -1;
  cleanup_close صحيح pipefd1 = -1;
  libcrun_error_t tmp_err = عدم;

  container->context = context;

  ret = validate_options (options, LIBCRUN_لاڳيتو_OPTIONS_PREFORK | LIBCRUN_لاڳيتو_OPTIONS_KEEP, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = check_config_file (def, context, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (def->process && def->process->terminal && detach && context->console_socket == عدم)
    أرجع crun_make_error (err, 0, "use --console-socket with --detach when a terminal is used");

  ret = libcrun_حالت_check_directories (context->state_پاڙ, context->id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (! detach && (options & LIBCRUN_لاڳيتو_OPTIONS_PREFORK) == 0)
    {
      ret = libcrun_copy_config_file (context->id, context->state_پاڙ, container, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = libcrun_container_لاڳيتو_اندروني (container, context, عدم, err);
      إذا (! (options & LIBCRUN_لاڳيتو_OPTIONS_KEEP))
        force_delete_container_حالت (context, container, def);
      أرجع ret;
    }

  ret = pipe2 (container_ret_حالت, O_CLOEXEC);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_حالت[0];
  pipefd1 = container_ret_حالت[1];

  ret = fork ();
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "fork");
  إذا (ret)
    {
      صحيح حالت;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, عدم, 0);

      ret = عارضي_FAILURE_RETRY (اقرأ (pipefd0, &حالت, حجم (حالت)));
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, errno, "read from sync pipe");

      إذا (حالت < 0)
        {
          صحيح errno_;
          حرف buf[512];
          ret = عارضي_FAILURE_RETRY (اقرأ (pipefd0, &errno_, حجم (errno_)));
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "read from sync pipe");

          ret = عارضي_FAILURE_RETRY (اقرأ (pipefd0, buf, حجم (buf) - 1));
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "read from sync pipe");
          buf[ret] = '\0';

          أرجع crun_make_error (err, errno_, "%s", buf);
        }

      أرجع حالت;
    }

  close_and_reset (&pipefd0);

  /* forked process.  */
  ret = detach_process ();
  إذا (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_پاڙ, container, &tmp_err);
  إذا (UNLIKELY (ret < 0))
    اذهب fail;

  ret = libcrun_container_لاڳيتو_اندروني (container, context, عدم, &tmp_err);
  عارضي_FAILURE_RETRY (اكتب (pipefd1, &ret, حجم (ret)));
  إذا (UNLIKELY (ret < 0))
    اذهب fail;

  ٻاهر_نڪرو (EXIT_SUCCESS);
fail:

  إذا (! (options & LIBCRUN_لاڳيتو_OPTIONS_KEEP))
    force_delete_container_حالت (context, container, def);
  إذا (tmp_err)
    {
      write_error_منزل_pipe (pipefd1, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ٻاهر_نڪرو (EXIT_FAILURE);
}

صحيح
libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container, بنانشان صحيح options,
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

  ret = libcrun_حالت_check_directories (context->state_پاڙ, context->id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  exec_fifo_fd = libcrun_حالت_create_exec_fifo (context->state_پاڙ, context->id, err);
  إذا (UNLIKELY (exec_fifo_fd < 0))
    أرجع exec_fifo_fd;

  context->fifo_exec_wait_fd = exec_fifo_fd;
  exec_fifo_fd = -1;

  إذا ((options & LIBCRUN_لاڳيتو_OPTIONS_PREFORK) == 0)
    {
      libcrun_debug ("Running with prefork enabled");
      ret = libcrun_copy_config_file (context->id, context->state_پاڙ, container, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      ret = libcrun_container_لاڳيتو_اندروني (container, context, عدم, err);
      إذا (UNLIKELY (ret < 0))
        force_delete_container_حالت (context, container, def);
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
      صحيح exit_ڪوڊ;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, عدم, 0);

      ret = عارضي_FAILURE_RETRY (اقرأ (pipefd0, &exit_ڪوڊ, حجم (exit_ڪوڊ)));
      إذا (UNLIKELY (ret <= 0))
        أرجع crun_make_error (err, ret < 0 ? errno : 0, "waiting for container to be ready");

      إذا (ret != حجم (exit_ڪوڊ) || exit_ڪوڊ < 0)
        أرجع crun_make_error (err, 0, "internal error: read invalid exit code");

      إذا (exit_ڪوڊ == 0)
        أرجع 0;

      libcrun_debug ("Exit code is `%d`, deleting container", exit_ڪوڊ);
      libcrun_error_t tmp_err = عدم;
      container_delete_اندروني (context, عدم, def, context->id, true, true, &tmp_err);
      libcrun_error_report_and_release (&tmp_err);
      أرجع crun_make_error (err, 0, "error creating container");
    }

  /* forked process.  */
  ret = detach_process ();
  إذا (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_پاڙ, container, err);
  إذا (UNLIKELY (ret < 0))
    {
      صحيح errcode = crun_error_get_errno (err);
      crun_error_release (err);
      libcrun_fail_with_error (errcode, "copy config file");
    }

  ret = libcrun_container_لاڳيتو_اندروني (container, context, &pipefd1, err);
  إذا (UNLIKELY (ret < 0))
    {
      force_delete_container_حالت (context, container, def);
      libcrun_error ((*err)->حالت, "%s", (*err)->نياپو);
      crun_error_release (err);
      crun_سيٽ_output_handler (log_write_منزل_stderr, عدم);
    }

  إذا (pipefd1 >= 0)
    عارضي_FAILURE_RETRY (اكتب (pipefd1, &ret, حجم (ret)));
  ٻاهر_نڪرو (ret ? EXIT_FAILURE : 0);
}

صحيح
libcrun_container_شروعات (libcrun_context_t *context, ثابت حرف *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = عدم;
  ثابت حرف *state_پاڙ = context->state_پاڙ;
  runtime_spec_schema_config_schema *def;
  cleanup_container_حالت libcrun_container_حالت_t حالت = {};
  cleanup_close صحيح fd = -1;
  صحيح ret;

  ret = libcrun_read_container_حالت (&حالت, state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_is_container_running (&حالت, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (! ret)
    أرجع crun_make_error (err, 0, "container `%s` is not running", id);

  ret = read_container_config_from_state (&container, state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (context->notify_socket)
    {
      ret = get_notify_fd (context, container, &fd, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = libcrun_حالت_write_exec_fifo (context->state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  def = container->container_def;

  إذا (context->notify_socket)
    {
      إذا (fd >= 0)
        {
          fd_set read_سيٽ;

          طالما (1)
            {
              بنية timeval timeout = {
                .tv_sec = 0,
                .tv_usec = 10000,
              };
              FD_ZERO (&read_سيٽ);
              FD_SET (fd, &read_سيٽ);

              ret = select (fd + 1, &read_سيٽ, عدم, عدم, &timeout);
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
                  ret = libcrun_is_container_running (&حالت, err);
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
      cleanup_close صحيح hooks_ٻاهريون_fd = -1;
      cleanup_close صحيح hooks_err_fd = -1;

      ret = open_hooks_output (container, &hooks_ٻاهريون_fd, &hooks_err_fd, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = do_hooks (def, حالت.pid, context->id, false, حالت.bundle, حالت.bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_ٻاهريون_fd, hooks_err_fd, false, err);
      إذا (UNLIKELY (ret != 0))
        {
          libcrun_error_t tmp_err = عدم;
          container_delete_اندروني (context, container, def, id, true, true, &tmp_err);
          libcrun_error_report_and_release (&tmp_err);
          أرجع ret;
        }
    }

  أرجع 0;
}

صحيح
libcrun_get_container_state_string (ثابت حرف *id, libcrun_container_حالت_t *حالت, ثابت حرف *state_پاڙ,
                                    ثابت حرف **container_حالت, صحيح *running, libcrun_error_t *err)
{
  صحيح ret, has_fifo = 0;
  bool paused = false;

  ret = libcrun_is_container_running (حالت, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;
  *running = ret;

  إذا (*running)
    {
      ret = libcrun_حالت_has_read_exec_fifo (state_پاڙ, id, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      has_fifo = ret;
    }

  إذا (*running && ! has_fifo)
    {
      cleanup_cgroup_حالت بنية libcrun_cgroup_حالت *cgroup_حالت = عدم;

      cgroup_حالت = libcrun_cgroup_make_حالت (حالت);

      ret = libcrun_cgroup_is_container_paused (cgroup_حالت, &paused, err);
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
              *container_حالت = "stopped";
              أرجع 0;
            }

          أرجع ret;
        }
    }

  إذا (! *running)
    *container_حالت = "stopped";
  وإلا إذا (has_fifo)
    *container_حالت = "created";
  وإلا إذا (paused)
    *container_حالت = "paused";
  وإلا
    *container_حالت = "running";

  أرجع 0;
}

صحيح
libcrun_container_state_json (libcrun_context_t *context, ثابت حرف *id, حرف **ٻاهريون, libcrun_error_t *err)
{
  حرف *بفر = عدم;
  size_t len = 0;
  FILE *f;
  صحيح ret;

  f = open_memstream (&بفر, &len);
  إذا (f == عدم)
    أرجع crun_make_error (err, errno, "open_memstream");

  ret = libcrun_container_state (context, id, f, err);
  fclose (f);
  إذا (UNLIKELY (ret < 0))
    {
      حرر (بفر);
      أرجع ret;
    }

  *ٻاهريون = بفر;
  أرجع 0;
}

صحيح
libcrun_container_state (libcrun_context_t *context, ثابت حرف *id, FILE *ٻاهريون, libcrun_error_t *err)
{
  ثابت حرف *ثابت OCI_CONFIG_نسخو = "1.0.0";
  libcrun_container_حالت_t حالت = {};
  ثابت حرف *state_پاڙ = context->state_پاڙ;
  ثابت حرف *container_حالت = عدم;
  json_gen_ctx *gen = عدم;
  ثابت حرف *buf;
  صحيح ret = 0;
  صحيح r = json_gen_حالت_ok;
  صحيح running;
  size_t len;

  ret = libcrun_read_container_حالت (&حالت, state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_get_container_state_string (id, &حالت, state_پاڙ, &container_حالت, &running, err);
  إذا (UNLIKELY (ret < 0))
    اذهب ٻاهر_نڪرو;

  ret = 0;
  إذا (! json_gen_init (&gen, عدم))
    أرجع crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));
  GEN_ڪنجي (gen, "ociVersion");
  GEN_STR (gen, OCI_CONFIG_نسخو);

  GEN_ڪنجي (gen, "id");
  GEN_STR (gen, id);

  GEN_ڪنجي (gen, "pid");
  GEN_OR_FAIL (map_int (gen, running ? حالت.pid : 0));

  GEN_ڪنجي (gen, "status");
  GEN_STR (gen, container_حالت);

  GEN_ڪنجي (gen, "bundle");
  GEN_STR (gen, حالت.bundle);

  GEN_ڪنجي (gen, "rootfs");
  GEN_STR (gen, حالت.rootfs);

  GEN_ڪنجي (gen, "created");
  GEN_STR (gen, حالت.created);

  إذا (حالت.scope)
    {
      GEN_ڪنجي (gen, "systemd-scope");
      GEN_STR (gen, حالت.scope);
    }

  إذا (حالت.owner)
    {
      GEN_ڪنجي (gen, "owner");
      GEN_STR (gen, حالت.owner);
    }

  {
    cleanup_container libcrun_container_t *container = عدم;

    ret = read_container_config_from_state (&container, state_پاڙ, id, err);
    إذا (UNLIKELY (ret < 0))
      اذهب ٻاهر_نڪرو;

    إذا (container->container_def->annotations && container->container_def->annotations->len)
      GEN_OR_FAIL (gen_annotations (gen, container->container_def->annotations));
  }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  fprintf (ٻاهريون, "%s\n", buf);

ٻاهر_نڪرو:
  إذا (gen)
    json_gen_free (gen);
  libcrun_free_container_حالت (&حالت);
  أرجع ret;

gen_error:
  ret = json_gen_error_منزل_crun_error (r, err);
  اذهب ٻاهر_نڪرو;
}

صحيح
libcrun_container_exec (libcrun_context_t *context, ثابت حرف *id, runtime_spec_schema_config_schema_process *process,
                        libcrun_error_t *err)
{
  بنية libcrun_container_exec_options_s opts;
  memset (&opts, 0, حجم (opts));

  opts.struct_سائيز = حجم (opts);
  opts.process = process;

  أرجع libcrun_container_exec_with_options (context, id, &opts, err);
}

صحيح
libcrun_container_exec_process_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *رستو, libcrun_error_t *err)
{
  بنية libcrun_container_exec_options_s opts;
  memset (&opts, 0, حجم (opts));

  opts.struct_سائيز = حجم (opts);
  opts.رستو = رستو;

  أرجع libcrun_container_exec_with_options (context, id, &opts, err);
}

ساكن صحيح
exec_from_process_json (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *process_json, ثابت حرف *cgroup,
                        bool ملايو_env, libcrun_error_t *err)
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
  opts.struct_سائيز = حجم (opts);
  opts.process = process;
  opts.cgroup = cgroup;
  opts.ملايو_env = ملايو_env;

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
  إذا (opts == عدم || opts->struct_سائيز == 0)
    أرجع crun_make_error (err, EINVAL, "invalid exec options");

  أرجع exec_from_process_json (context, id, opts->process_json, opts->cgroup, opts->ملايو_env, err);
}

#وصف_ڪريو cleanup_process_schema __attribute__ ((cleanup (cleanup_process_schemap)))

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
                         bool ملايو_env,
                         libcrun_error_t *err)
{
  runtime_spec_schema_config_schema_process_capabilities *capabilities = عدم;
  cleanup_free حرف *exec_رستو = عدم;
  uid_t container_uid;
  gid_t container_gid;
  ثابت حرف *cwd;
  bool chdir_پورو = false;
  size_t seccomp_flags_len = 0;
  حرف **seccomp_flags = عدم;
  pid_t own_pid = 0;
  size_t i;
  صحيح ret;

  container_uid = process->user ? process->user->uid : 0;
  container_gid = process->user ? process->user->gid : 0;

  عارضي_FAILURE_RETRY (اقرأ (*pipefd1, &own_pid, حجم (own_pid)));

  cwd = process->cwd ? process->cwd : "/";
  إذا (LIKELY (libcrun_محفوظ_chdir (cwd, err) == 0))
    chdir_پورو = true;
  وإلا
    crun_error_release (err);

  ret = unblock_سگنل (err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = clearenv ();
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "clearenv");

  إذا ((ملايو_env || ! process->env_len) && container->container_def->process && container->container_def->process->env_len)
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

  إذا (is_خالي_جايون_string (getenv ("HOME")))
    {
      ret = سيٽ_home_env (container_uid);
      إذا (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          إذا (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            أرجع crun_make_error (err, errno, "setenv HOME");
        }
    }

  ret = libcrun_سيٽ_selinux_label (container, process, false, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_سيٽ_apparmor_profile (container, process, false, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (container->container_def->linux && container->container_def->linux->seccomp)
    {
      seccomp_flags = container->container_def->linux->seccomp->flags;
      seccomp_flags_len = container->container_def->linux->seccomp->flags_len;
    }

  ret = ڳوليو_executable (&exec_رستو, process->دليلون[0], process->cwd, err);
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
      ret = libcrun_سيٽ_personality (container->container_def->linux->personality, err);
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

  ret = libcrun_سيٽ_caps (capabilities, container_uid, container_gid, process->no_new_privileges,
                          custom_handler != عدم, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (UNLIKELY (exec_رستو == عدم))
    {
      ret = ڳوليو_executable (&exec_رستو, process->دليلون[0], process->cwd, err);
      إذا (UNLIKELY (ret < 0))
        {
          إذا (custom_handler == عدم || is_خالي_جايون_string (process->دليلون[0]))
            أرجع ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          exec_رستو = xstrdup (process->دليلون[0]);
        }
    }

  إذا (! chdir_پورو)
    {
      ret = libcrun_محفوظ_chdir (cwd, err);
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
  عارضي_FAILURE_RETRY (اكتب (*pipefd1, &ret, حجم (ret)));
  عارضي_FAILURE_RETRY (close (*pipefd1));
  *pipefd1 = -1;

  إذا (custom_handler)
    {
      إذا (custom_handler->vtable->exec_func == عدم)
        أرجع crun_make_error (err, 0, "the handler does not support exec");

      إذا (context->argv)
        erase_initial_environment (context->argv, context->argc);

      ret = custom_handler->vtable->exec_func (custom_handler->cookie,
                                               container,
                                               exec_رستو,
                                               process->دليلون);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, -ret, "exec container process failed with handler as `%s`", custom_handler->vtable->name);

      محفوظ_exit (EXIT_FAILURE);
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation, because the seccomp filter is already in place and it could
     stop some syscalls used by mark_or_close_fds_ge_than.
  */
  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, true, err);
  إذا (UNLIKELY (ret < 0))
    crun_error_release (err);

  عارضي_FAILURE_RETRY (execv (exec_رستو, process->دليلون));
  libcrun_fail_with_error (errno, "exec `%s`", exec_رستو);
}

/* Close the file descriptors inherited from the caller, other than those
   to be passed to the container process.  */
صحيح
libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err)
{
  أرجع mark_or_close_fds_ge_than (عدم, context->preserve_fds + 3, true, err);
}

صحيح
libcrun_ڀريو_process_from_file (ثابت حرف *رستو, runtime_spec_schema_config_schema_process **process,
                                libcrun_error_t *err)
{
  بنية parser_context ctx = { 0, stderr };
  cleanup_free حرف *content = عدم;
  parser_error parser_err = عدم;
  json_object *doc = عدم;
  size_t len;
  صحيح ret;

  ret = read_all_file (رستو, &content, &len, err);
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
  صحيح container_حالت, ret;
  bool container_paused = false;
  pid_t pid;
  libcrun_container_حالت_t حالت = {};
  ثابت حرف *state_پاڙ = context->state_پاڙ;
  cleanup_close صحيح terminal_fd = -1;
  cleanup_close صحيح seccomp_fd = -1;
  cleanup_terminal فراغ *orig_terminal = عدم;
  cleanup_container libcrun_container_t *container = عدم;
  صحيح container_ret_حالت[2];
  cleanup_close صحيح pipefd0 = -1;
  cleanup_close صحيح pipefd1 = -1;
  cleanup_close صحيح seccomp_receiver_fd = -1;
  cleanup_close صحيح own_seccomp_receiver_fd = -1;
  cleanup_close صحيح seccomp_notify_fd = -1;
  ثابت حرف *seccomp_notify_plugins = عدم;
  __attribute__ ((اڻ_استعمال_ٿيل)) cleanup_process_schema runtime_spec_schema_config_schema_process *process_cleanup = عدم;
  runtime_spec_schema_config_schema_process *process = opts->process;
  بنية libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  صحيح ret_from_ٻار = 0;

  ret = libcrun_read_container_حالت (&حالت, state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_is_container_running (&حالت, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;
  container_حالت = ret;

  ret = read_container_config_from_state (&container, state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  container->context = context;

  إذا (container_حالت == 0)
    أرجع crun_make_error (err, 0, "the container `%s` is not running", id);

  {
    cleanup_cgroup_حالت بنية libcrun_cgroup_حالت *cgroup_حالت = عدم;

    cgroup_حالت = libcrun_cgroup_make_حالت (&حالت);

    ret = libcrun_cgroup_is_container_paused (cgroup_حالت, &container_paused, err);
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

  ret = بلاڪ_سگنل (err);
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

  إذا (حجم (*opts) != opts->struct_سائيز)
    أرجع crun_make_error (err, EINVAL, "invalid libcrun_container_exec_options_s struct");

  إذا (opts->رستو)
    {
      إذا (process)
        أرجع crun_make_error (err, EINVAL, "cannot specify both exec file and options");

      ret = libcrun_ڀريو_process_from_file (opts->رستو, &process, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      process_cleanup = process;
    }

  /* This must be done before we enter a user namespace.  If the process does
     not specify any rlimits, use the ones from the container configuration.  */
  إذا (process->rlimits_len == 0 && container->container_def->process)
    ret = libcrun_سيٽ_rlimits (container->container_def->process->rlimits,
                               container->container_def->process->rlimits_len, err);
  وإلا
    ret = libcrun_سيٽ_rlimits (process->rlimits, process->rlimits_len, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_سيٽ_mempolicy (container->container_def, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = pipe2 (container_ret_حالت, O_CLOEXEC);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_حالت[0];
  pipefd1 = container_ret_حالت[1];

  /* If the new process block doesn't specify a SELinux label, AppArmor profile, user,
     I/O priority or scheduler, then use the configuration from the original config file.  */
  إذا (container->container_def->process)
    {
      إذا (process->io_ترجيح == عدم && container->container_def->process->io_ترجيح)
        {
          process->io_ترجيح = clone_runtime_spec_schema_config_schema_process_io_ترجيح (container->container_def->process->io_ترجيح);
          إذا (process->io_ترجيح == عدم)
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
  إذا (! context->no_new_keyring && ! حالت.no_new_keyring)
    {
      libcrun_debug ("Creating new keyring for the exec process");

      ret = libcrun_create_keyring (container, عدم, process->selinux_label, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = prctl (PR_سيٽ_DUMPABLE, 0, 0, 0, 0);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "prctl unset dumpable");

  pid = libcrun_join_process (context, container, حالت.pid, &حالت, opts->cgroup, context->detach,
                              process, process->terminal ? &terminal_fd : عدم, err);
  إذا (UNLIKELY (pid < 0))
    أرجع pid;

  /* Process to exec.  */
  إذا (pid == 0)
    {
      عارضي_FAILURE_RETRY (close (pipefd0));
      pipefd0 = -1;

      exec_process_entrypoint (context, container, process, &pipefd1, seccomp_fd, seccomp_receiver_fd, custom_handler, opts->ملايو_env, err);
      /* It gets here only on errors.  */
      إذا (*err)
        {
          إذا (pipefd1 < 0)
            libcrun_fail_with_error ((*err)->حالت, "%s", (*err)->نياپو);
          وإلا
            {
              write_error_منزل_pipe (pipefd1, err);
              عارضي_FAILURE_RETRY (close (pipefd1));
              pipefd1 = -1;
            }
        }
      محفوظ_exit (EXIT_FAILURE);
    }

  عارضي_FAILURE_RETRY (close (pipefd1));
  pipefd1 = -1;

  عارضي_FAILURE_RETRY (اكتب (pipefd0, &pid, حجم (pid)));

  إذا (seccomp_fd >= 0)
    close_and_reset (&seccomp_fd);

  إذا (terminal_fd >= 0)
    {
      بنانشان ننڍو صفون = 0, کانن_ڳڻپ = 0;

      إذا (process->console_سائيز)
        {
          کانن_ڳڻپ = process->console_سائيز->ويڪر;
          صفون = process->console_سائيز->اوچائي;
        }

      ret = libcrun_terminal_تياري_سائيز (terminal_fd, صفون, کانن_ڳڻپ, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      إذا (context->console_socket)
        {
          صحيح ret;
          cleanup_close صحيح console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
          إذا (UNLIKELY (console_socket_fd < 0))
            أرجع console_socket_fd;
          ret = send_fd_منزل_socket (console_socket_fd, terminal_fd, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          close_and_reset (&terminal_fd);
        }
      وإلا
        {
          ret = libcrun_clear_onlcr (terminal_fd, err);
          إذا (UNLIKELY (ret < 0))
            {
              flush_fd_منزل_err (context, terminal_fd);
              أرجع ret;
            }

          ret = libcrun_سيٽ_raw (0, &orig_terminal, err);
          إذا (UNLIKELY (ret < 0))
            {
              flush_fd_منزل_err (context, terminal_fd);
              أرجع ret;
            }
        }
    }

  ret = عارضي_FAILURE_RETRY (اقرأ (pipefd0, &ret_from_ٻار, حجم (ret_from_ٻار)));
  إذا (ret != حجم (ret_from_ٻار))
    ret = crun_make_error (err, 0, "read pipe failed");
  وإلا إذا (ret_from_ٻار != 0)
    {
      cleanup_free حرف *نياپو = عدم;
      size_t len = 0;

      ret = read_all_fd (pipefd0, "error stream", &نياپو, &len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      /* the string from read_all_fd is always NUL terminated.  */
      ret = crun_make_error (err, ret_from_ٻار, "%s", نياپو);
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
        بنية wait_for_process_دليلون دليلون = {
          .pid = pid,
          .context = context,
          .terminal_fd = terminal_fd,
          .notify_socket = -1,
          .container_ready_fd = عدم,
          .seccomp_notify_fd = seccomp_notify_fd,
          .seccomp_notify_plugins = seccomp_notify_plugins,
        };

        ret = wait_for_process (&دليلون, err);
      }
    }

  flush_fd_منزل_err (context, terminal_fd);
  أرجع ret;
}

صحيح
libcrun_container_update (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *content, size_t len arg_اڻ_استعمال_ٿيل,
                          libcrun_error_t *err)
{
  cleanup_custom_handler_instance بنية custom_handler_instance_s *custom_handler = عدم;
  runtime_spec_schema_config_linux_resources *resources = عدم;
  cleanup_container libcrun_container_t *container = عدم;
  ثابت حرف *state_پاڙ = context->state_پاڙ;
  بنية parser_context ctx = { 0, stderr };
  libcrun_container_حالت_t حالت = {};
  parser_error parser_err = عدم;
  json_object *doc = عدم;
  صحيح ret;

  ret = libcrun_read_container_حالت (&حالت, state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = read_container_config_from_state (&container, state_پاڙ, id, err);
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

  ret = libcrun_linux_container_update (&حالت, state_پاڙ, resources, err);

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
compare_update_قدرون (ثابت فراغ *a, ثابت فراغ *b)
{
  ثابت بنية libcrun_update_قدر_s *aa = a;
  ثابت بنية libcrun_update_قدر_s *bb = b;
  صحيح ret;

  ret = strcmp (aa->section, bb->section);
  إذا (ret)
    أرجع ret;
  أرجع strcmp (aa->name, bb->name);
}

صحيح
libcrun_container_update_from_قدرون (libcrun_context_t *context, ثابت حرف *id,
                                      بنية libcrun_update_قدر_s *قدرون, size_t len,
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

  qsort (قدرون, len, حجم (بنية libcrun_update_قدر_s), compare_update_قدرون);

  لكل (i = 0; i < len; i++)
    {
      إذا (current_section == عدم || strcmp (قدرون[i].section, current_section))
        {
          إذا (i > 0)
            json_gen_map_close (gen);

          current_section = قدرون[i].section;
          json_gen_string (gen, current_section, لڙي_ڊيگهه (current_section));
          json_gen_map_open (gen);
        }

      json_gen_string (gen, قدرون[i].name, لڙي_ڊيگهه (قدرون[i].name));

      إذا (قدرون[i].numeric)
        json_gen_عدد (gen, قدرون[i].قدر, لڙي_ڊيگهه (قدرون[i].قدر));
      وإلا
        json_gen_string (gen, قدرون[i].قدر, لڙي_ڊيگهه (قدرون[i].قدر));
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
populate_array_ميدان (حرف ***ميدان, حرف *array[], size_t انگ_elements)
{
  size_t i;

  *ميدان = xmalloc0 ((انگ_elements + 1) * حجم (حرف *));
  لكل (i = 0; i < انگ_elements; i++)
    (*ميدان)[i] = xstrdup (array[i]);

  (*ميدان)[i] = عدم;
}

#جيڪڏهن_وصف_ٿيل HAVE_CAP
ساكن فراغ
populate_capabilities (بنية features_info_s *info, حرف ***capabilities, size_t *انگ_capabilities)
{
  size_t اشاريو = 0;
  cap_قدر_t i;
  حرف *endptr;
  صحيح j;

  *انگ_capabilities = 0;
  لكل (i = 0;; i++)
    {
      حرف *v = cap_منزل_name (i);
      إذا (v == عدم)
        اكسر;
      strtol (v, &endptr, 10);
      إذا (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          اكسر;
        }
      (*انگ_capabilities)++;
    }

  *capabilities = xmalloc0 ((*انگ_capabilities + 1) * حجم (ثابت حرف *));
  لكل (i = 0; i < (cap_قدر_t) *انگ_capabilities; i++)
    {
      حرف *v = cap_منزل_name (i);
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
        v[j] = وڏو_اکر (v[j]);

      (*capabilities)[اشاريو] = v;
      اشاريو++;
    }

  (*capabilities)[اشاريو] = عدم; // Terminate the array with NULL
  populate_array_ميدان (&(info->linux.capabilities), *capabilities, *انگ_capabilities);
}
#جيڪڏهن_پورو

ساكن فراغ
retrieve_mount_options (بنية features_info_s **info)
{
  cleanup_free ثابت بنية ڦهلاء_flags_s *mount_options_list = عدم;
  size_t انگ_mount_options = 0;

  // Retrieve mount options from wordlist
  mount_options_list = get_mount_flags_from_wordlist ();

  // Calculate the number of mount options
  طالما (mount_options_list[انگ_mount_options].name != عدم)
    انگ_mount_options++;

  // Allocate memory for mount options in info struct
  (*info)->mount_options = xmalloc0 ((انگ_mount_options + 1) * حجم (حرف *));

  // Copy mount options to info struct
  لكل (size_t i = 0; i < انگ_mount_options; i++)
    (*info)->mount_options[i] = xstrdup (mount_options_list[i].name);
}

صحيح
libcrun_container_get_features (libcrun_context_t *context, بنية features_info_s **info, libcrun_error_t *err arg_اڻ_استعمال_ٿيل)
{
  // Allocate memory for the features_info_s structure
  size_t انگ_namspaces = حجم (namespaces) / حجم (namespaces[0]);
  size_t انگ_operators = حجم (operators) / حجم (operators[0]);
  size_t انگ_actions = حجم (actions) / حجم (actions[0]);
  size_t انگ_hooks = حجم (hooks) / حجم (hooks[0]);
  size_t انگ_archs = حجم (archs) / حجم (archs[0]);
  size_t انگ_unsafe_annotations = حجم (potentially_unsafe_annotations) / حجم (potentially_unsafe_annotations[0]);
  cleanup_free حرف **capabilities = عدم;
  size_t انگ_capabilities = 0;
  size_t انگ_mempolicy_modes = حجم (mempolicy_modes) / حجم (mempolicy_modes[0]);
  size_t انگ_mempolicy_flags = حجم (mempolicy_flags) / حجم (mempolicy_flags[0]);

  *info = xmalloc0 (حجم (بنية features_info_s));

  // Hardcoded feature information
  (*info)->oci_نسخو_min = xstrdup ("1.0.0");
  (*info)->oci_نسخو_max = xstrdup ("1.1.0+dev");

  // Populate hooks
  populate_array_ميدان (&((*info)->hooks), hooks, انگ_hooks);

  // Populate mount_options
  retrieve_mount_options (info);

  // Populate namespaces
  populate_array_ميدان (&((*info)->linux.namespaces), namespaces, انگ_namspaces);

#جيڪڏهن_وصف_ٿيل HAVE_CAP
  // Populate capabilities
  populate_capabilities (*info, &capabilities, &انگ_capabilities);
#جيڪڏهن_پورو

  // Hardcode the values for cgroup
  (*info)->linux.cgroup.v1 = true;
  (*info)->linux.cgroup.v2 = true;
#جيڪڏهن_وصف_ٿيل HAVE_SYSTEMD
  (*info)->linux.cgroup.systemd = true;
  (*info)->linux.cgroup.systemd_user = true;
#جيڪڏهن_پورو

  // Put seccomp values
#جيڪڏهن_وصف_ٿيل HAVE_SECCOMP
  (*info)->linux.seccomp.enabled = true;
  // Populate actions
  populate_array_ميدان (&((*info)->linux.seccomp.actions), actions, انگ_actions);
  // Populate operators
  populate_array_ميدان (&((*info)->linux.seccomp.operators), operators, انگ_operators);
  // Populate archs
  populate_array_ميدان (&((*info)->linux.seccomp.archs), archs, انگ_archs);
#وإلا
  (*info)->linux.seccomp.enabled = false;
#جيڪڏهن_پورو

  // Put values for apparmor and selinux
  (*info)->linux.apparmor.enabled = true;
  (*info)->linux.selinux.enabled = true;

  (*info)->linux.intel_rdt.enabled = true;

  (*info)->linux.net_devices.enabled = true;

  populate_array_ميدان (&((*info)->linux.يادگيري_policy.گهڻائي), mempolicy_modes, انگ_mempolicy_modes);
  populate_array_ميدان (&((*info)->linux.يادگيري_policy.flags), mempolicy_flags, انگ_mempolicy_flags);

  // Put the values for mount extensions
  (*info)->linux.mount_ext.idmap.enabled = true;

  // Populate the values for annotations
#جيڪڏهن_وصف_ٿيل HAVE_SECCOMP
  {
    ثابت بنية scmp_نسخو *نسخو = seccomp_نسخو ();
    حرف *نسخو_string = عدم;

    xasprintf (&نسخو_string, "%u.%u.%u", نسخو->major, نسخو->minor, نسخو->micro);
    (*info)->annotations.io_github_seccomp_libseccomp_نسخو = نسخو_string;
  }
#جيڪڏهن_پورو

  إذا (context->handler_manager && handler_by_name (context->handler_manager, "wasm"))
    (*info)->annotations.لاڳيتو_oci_crun_wasm = true;

#إذا HAVE_CRIU
  (*info)->annotations.لاڳيتو_oci_crun_checkpoint_enabled = true;
#جيڪڏهن_پورو
  (*info)->annotations.لاڳيتو_oci_crun_commit = GIT_نسخو;
  (*info)->annotations.لاڳيتو_oci_crun_نسخو = PACKAGE_نسخو;

  populate_array_ميدان (&((*info)->potentially_unsafe_annotations), potentially_unsafe_annotations, انگ_unsafe_annotations);

  أرجع 0;
}

ساكن فراغ
features_جوڙيو_string (json_gen_ctx *json_gen, ثابت حرف *ڪنجي, ثابت حرف *قدر)
{
  json_gen_string (json_gen, ڪنجي, لڙي_ڊيگهه (ڪنجي));
  json_gen_string (json_gen, قدر, لڙي_ڊيگهه (قدر));
}

ساكن فراغ
features_جوڙيو_bool (json_gen_ctx *json_gen, ثابت حرف *ڪنجي, صحيح قدر)
{
  json_gen_string (json_gen, ڪنجي, لڙي_ڊيگهه (ڪنجي));
  json_gen_bool (json_gen, قدر);
}

ساكن فراغ
features_جوڙيو_bool_str (json_gen_ctx *json_gen, ثابت حرف *ڪنجي, صحيح قدر)
{
  features_جوڙيو_string (json_gen, ڪنجي, قدر ? "true" : "false");
}

ساكن فراغ
features_جوڙيو_array (json_gen_ctx *json_gen, ثابت حرف *ڪنجي, حرف **array)
{
  size_t i;

  json_gen_string (json_gen, ڪنجي, لڙي_ڊيگهه (ڪنجي));
  json_gen_array_open (json_gen);
  إذا (array)
    لكل (i = 0; array[i] != عدم; i++)
      json_gen_string (json_gen, array[i], لڙي_ڊيگهه (array[i]));
  json_gen_array_close (json_gen);
}

ساكن فراغ
features_جوڙيو_linux_info (json_gen_ctx *json_gen, ثابت بنية linux_info_s *linux)
{
  json_gen_string (json_gen, "linux", لڙي_ڊيگهه ("linux"));
  json_gen_map_open (json_gen);

  features_جوڙيو_array (json_gen, "namespaces", linux->namespaces);
  features_جوڙيو_array (json_gen, "capabilities", linux->capabilities);

  json_gen_string (json_gen, "cgroup", لڙي_ڊيگهه ("cgroup"));
  json_gen_map_open (json_gen);
  features_جوڙيو_bool (json_gen, "v1", linux->cgroup.v1);
  features_جوڙيو_bool (json_gen, "v2", linux->cgroup.v2);
  features_جوڙيو_bool (json_gen, "systemd", linux->cgroup.systemd);
  features_جوڙيو_bool (json_gen, "systemdUser", linux->cgroup.systemd_user);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "seccomp", لڙي_ڊيگهه ("seccomp"));
  json_gen_map_open (json_gen);
  features_جوڙيو_bool (json_gen, "enabled", linux->seccomp.enabled);
  إذا (linux->seccomp.actions)
    features_جوڙيو_array (json_gen, "actions", linux->seccomp.actions);
  إذا (linux->seccomp.operators)
    features_جوڙيو_array (json_gen, "operators", linux->seccomp.operators);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "apparmor", لڙي_ڊيگهه ("apparmor"));
  json_gen_map_open (json_gen);
  features_جوڙيو_bool (json_gen, "enabled", linux->apparmor.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "selinux", لڙي_ڊيگهه ("selinux"));
  json_gen_map_open (json_gen);
  features_جوڙيو_bool (json_gen, "enabled", linux->selinux.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "mountExtensions", لڙي_ڊيگهه ("mountExtensions"));
  json_gen_map_open (json_gen);
  json_gen_string (json_gen, "idmap", لڙي_ڊيگهه ("idmap"));
  json_gen_map_open (json_gen);
  features_جوڙيو_bool (json_gen, "enabled", linux->mount_ext.idmap.enabled);
  json_gen_map_close (json_gen);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "intelRdt", لڙي_ڊيگهه ("intelRdt"));
  json_gen_map_open (json_gen);
  features_جوڙيو_bool (json_gen, "enabled", linux->intel_rdt.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "netDevices", لڙي_ڊيگهه ("netDevices"));
  json_gen_map_open (json_gen);
  features_جوڙيو_bool (json_gen, "enabled", linux->net_devices.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "memoryPolicy", لڙي_ڊيگهه ("memoryPolicy"));
  json_gen_map_open (json_gen);
  إذا (linux->يادگيري_policy.گهڻائي)
    features_جوڙيو_array (json_gen, "modes", linux->يادگيري_policy.گهڻائي);
  إذا (linux->يادگيري_policy.flags)
    features_جوڙيو_array (json_gen, "flags", linux->يادگيري_policy.flags);
  json_gen_map_close (json_gen);

  json_gen_map_close (json_gen);
}

ساكن فراغ
features_جوڙيو_annotations_info (json_gen_ctx *json_gen, ثابت بنية annotations_info_s *annotation)
{
  json_gen_string (json_gen, "annotations", لڙي_ڊيگهه ("annotations"));
  json_gen_map_open (json_gen);

  إذا (! is_خالي_جايون_string (annotation->io_github_seccomp_libseccomp_نسخو))
    features_جوڙيو_string (json_gen, "io.github.seccomp.libseccomp.version", annotation->io_github_seccomp_libseccomp_نسخو);

  features_جوڙيو_bool_str (json_gen, "org.opencontainers.runc.checkpoint.enabled", annotation->لاڳيتو_oci_crun_checkpoint_enabled);
  features_جوڙيو_bool_str (json_gen, "run.oci.crun.checkpoint.enabled", annotation->لاڳيتو_oci_crun_checkpoint_enabled);

  features_جوڙيو_string (json_gen, "run.oci.crun.commit", annotation->لاڳيتو_oci_crun_commit);
  features_جوڙيو_string (json_gen, "run.oci.crun.version", annotation->لاڳيتو_oci_crun_نسخو);

  features_جوڙيو_bool_str (json_gen, "run.oci.crun.wasm", annotation->لاڳيتو_oci_crun_wasm);

  json_gen_map_close (json_gen);
}

صحيح
libcrun_container_get_features_json (libcrun_context_t *context, حرف **ٻاهريون, libcrun_error_t *err)
{
  cleanup_struct_features بنية features_info_s *info = عدم;
  ثابت حرف *بفر = عدم;
  json_gen_ctx *json_gen;
  size_t بفر_len;
  صحيح ret;

  *ٻاهريون = عدم;

  ret = libcrun_container_get_features (context, &info, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (! json_gen_init (&json_gen, عدم))
    أرجع crun_make_error (err, 0, "cannot initialize JSON generator");

  json_gen_config (json_gen, json_gen_beautify, 1);

  json_gen_map_open (json_gen);

  features_جوڙيو_string (json_gen, "ociVersionMin", info->oci_نسخو_min);
  features_جوڙيو_string (json_gen, "ociVersionMax", info->oci_نسخو_max);
  features_جوڙيو_array (json_gen, "hooks", info->hooks);
  features_جوڙيو_array (json_gen, "mountOptions", info->mount_options);
  features_جوڙيو_linux_info (json_gen, &info->linux);
  features_جوڙيو_annotations_info (json_gen, &info->annotations);
  features_جوڙيو_array (json_gen, "potentiallyUnsafeConfigAnnotations", info->potentially_unsafe_annotations);

  json_gen_map_close (json_gen);

  json_gen_get_buf (json_gen, &بفر, &بفر_len);
  *ٻاهريون = xstrdup (بفر);
  json_gen_free (json_gen);

  أرجع 0;
}

/* Read the container status and fail if the container is not running.  */
ساكن صحيح
read_حالت_require_running (libcrun_context_t *context, ثابت حرف *id,
                             libcrun_container_حالت_t *حالت, libcrun_error_t *err)
{
  صحيح ret;

  ret = libcrun_read_container_حالت (حالت, context->state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_is_container_running (حالت, err);
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
  libcrun_container_حالت_t حالت = {};

  ret = read_حالت_require_running (context, id, &حالت, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع libcrun_container_pause_linux (&حالت, err);
}

صحيح
libcrun_container_unpause (libcrun_context_t *context, ثابت حرف *id, libcrun_error_t *err)
{
  صحيح ret;
  libcrun_container_حالت_t حالت = {};

  ret = read_حالت_require_running (context, id, &حالت, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع libcrun_container_unpause_linux (&حالت, err);
}

/* Translate the public options struct into the internal representation used by
   the linux/CRIU layer.  Strings are borrowed (not copied).  */
ساكن فراغ
cr_options_from_عوامي (libcrun_checkpoint_restore_t *dst, بنية libcrun_checkpoint_restore_options_s *src)
{
  memset (dst, 0, حجم (*dst));
  dst->image_رستو = (حرف *) src->image_رستو;
  dst->work_رستو = (حرف *) src->work_رستو;
  dst->والد_رستو = (حرف *) src->والد_رستو;
  dst->console_socket = src->console_socket;
  dst->lsm_profile = (حرف *) src->lsm_profile;
  dst->lsm_mount_context = (حرف *) src->lsm_mount_context;
  dst->leave_running = src->leave_running;
  dst->tcp_established = src->tcp_established;
  dst->tcp_close = src->tcp_close;
  dst->ext_unix_sk = src->ext_unix_sk;
  dst->shell_ڪم = src->shell_ڪم;
  dst->file_locks = src->file_locks;
  dst->pre_dump = src->pre_dump;
  dst->detach = src->detach;
  dst->manage_cgroups_گهڻائي = src->manage_cgroups_گهڻائي;
  dst->ڄار_تالو_طريقو = src->ڄار_تالو_طريقو;
}

صحيح
libcrun_container_checkpoint (libcrun_context_t *context, ثابت حرف *id,
                              بنية libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  صحيح ret;
  ثابت حرف *state_پاڙ = context->state_پاڙ;
  libcrun_container_حالت_t حالت = {};
  libcrun_checkpoint_restore_t cr;
  cleanup_container libcrun_container_t *container = عدم;

  cr_options_from_عوامي (&cr, cr_options);

  ret = read_حالت_require_running (context, id, &حالت, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = read_container_config_from_state (&container, state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* Needed to use the same paths (e.g. the shared empty directory used for
     masked paths) as the container runtime and restore do.  */
  container->context = context;

  ret = libcrun_container_checkpoint_linux (&حالت, container, &cr, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (! (cr_options->leave_running || cr_options->pre_dump))
    أرجع container_delete_اندروني (context, عدم, عدم, id, true, true, err);

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
  عارضي_FAILURE_RETRY (اقرأ (proxy_pid_pipe, &c, حجم (c)));
}

ساكن صحيح
restore_terminate_proxy_process (pid_t proxy_pid, ثابت حرف *proxy_cgroup, pid_t container_pid, صحيح cgroup_manager, libcrun_error_t *err)
{
  صحيح گهڻائي;
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

  گهڻائي = libcrun_get_cgroup_گهڻائي (err);
  إذا (UNLIKELY (گهڻائي < 0))
    أرجع گهڻائي;

  /* SIGKILL terminates a process frozen with cgroup v2, while the cgroup v1 freezer does not
     deliver it until the cgroup is thawed, so do not wait for it there.  */
  kill (proxy_pid, SIGKILL);
  عارضي_FAILURE_RETRY (waitpid (proxy_pid, عدم, گهڻائي == CGROUP_گهڻائي_UNIFIED ? 0 : WNOHANG));

  أرجع destroy_cgroup_رستو (proxy_cgroup, گهڻائي, err);
}

صحيح
libcrun_container_restore (libcrun_context_t *context, ثابت حرف *id arg_اڻ_استعمال_ٿيل,
                           بنية libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  cleanup_cgroup_حالت بنية libcrun_cgroup_حالت *cgroup_حالت = عدم;
  cleanup_container libcrun_container_t *container = عدم;
  cleanup_close صحيح proxy_pid_pipe0 = -1;
  cleanup_close صحيح proxy_pid_pipe1 = -1;
  runtime_spec_schema_config_schema *def;
  libcrun_container_حالت_t حالت = {};
  بنية libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  libcrun_checkpoint_restore_t cr;
  cleanup_pid pid_t proxy_pid = -1;
  cleanup_close صحيح seccomp_fd = -1;
  ثابت حرف *seccomp_bpf_معلومات;
  صحيح proxy_pid_pipe[2];
  صحيح cgroup_manager;
  uid_t پاڙ_uid = -1;
  gid_t پاڙ_gid = -1;
  صحيح ret;

  cr_options_from_عوامي (&cr, cr_options);

  container = libcrun_container_ڀريو_from_file ("config.json", err);
  إذا (container == عدم)
    أرجع -1;

  container->context = context;
  def = container->container_def;

  ret = check_config_file (def, context, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_حالت_check_directories (context->state_پاڙ, context->id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_copy_config_file (context->id, context->state_پاڙ, container, err);
  إذا (UNLIKELY (ret < 0))
    اذهب fail;

  /* CRIU restores the seccomp filters of the checkpointed processes, but
     exec loads the filter from the seccomp.bpf file in the state directory,
     so it must be generated again for the restored container.  */
  seccomp_bpf_معلومات = ڳوليو_annotation (container, "run.oci.seccomp_bpf_data");

  ret = تياري_seccomp (container, seccomp_bpf_معلومات, &seccomp_gen_ctx, &seccomp_fd, err);
  إذا (UNLIKELY (ret < 0))
    اذهب fail;

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_معلومات, &seccomp_gen_ctx, err);
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
  get_پاڙ_اندريون_the_userns (def, container->host_uid, container->host_gid, &پاڙ_uid, &پاڙ_gid);

  /* If the root in the container is different than the current root user, attempt to chown
     the std streams before entering the user namespace.  Otherwise we might lose access
     to the user (as it is not mapped in the user namespace) and cannot chown them.  */
  إذا (پاڙ_uid > 0 || پاڙ_gid > 0)
    {
      ret = maybe_chown_std_streams (پاڙ_uid, پاڙ_gid, err);
      إذا (UNLIKELY (ret < 0))
        اذهب fail;
    }

  /* CRIU runs as a child process, so the restored processes would inherit the session keyring.  */
  ret = تياري_container_keyring (container, context, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  {
    cleanup_close صحيح cgroup_dirfd = -1;
    bool needs_proxy_process;
    cleanup_cgroup_preenter بنية libcrun_cgroup_دليلون cg = {
      .resources = def->linux ? def->linux->resources : عدم,
      .annotations = container->annotations,
      .cgroup_رستو = def->linux ? def->linux->cgroups_رستو : "",
      .manager = cgroup_manager,
      .پاڙ_uid = پاڙ_uid,
      .پاڙ_gid = پاڙ_gid,
      .id = context->id,
      .state_پاڙ = context->state_پاڙ,
      .container = container,
    };

    /* The CRIU restore code uses bundle, rootfs and cgroup_path of status.   The cgroup_path is set later.  */
    حالت.bundle = (حرف *) context->bundle;
    حالت.rootfs = def->پاڙ->رستو;

    ret = libcrun_cgroup_preenter (&cg, &cgroup_dirfd, err);
    إذا (UNLIKELY (ret < 0))
      اذهب fail;

    needs_proxy_process = cgroup_dirfd < 0;

    /* If the target cgroup was already created (with cgroupfs), we can restore the container directly there.  */
    إذا (! needs_proxy_process)
      {
        cleanup_free حرف *cgroup_رستو = عدم;

        ret = get_cgroup_dirfd_رستو (cgroup_dirfd, &cgroup_رستو, err);
        إذا (UNLIKELY (ret < 0))
          اذهب fail;

        /* Complete the configuration for the cgroup before the container is
           restored there: CRIU resumes the restored processes before it
           returns, so the device filter and the limits must already be in
           place.  There is no process to move, as CRIU is run in the cgroup,
           and the controllers were enabled by precreate_cgroup.  */
        cg.joined = true;

        ret = libcrun_cgroup_enter (&cg, &cgroup_حالت, err);
        إذا (UNLIKELY (ret < 0))
          اذهب fail;

        /* Restore the container directly in the desired cgroup.  */
        حالت.cgroup_رستو = cgroup_رستو;

        ret = libcrun_container_restore_linux (&حالت, container, &cr, err);
        إذا (UNLIKELY (ret < 0))
          اذهب fail;

        /* Now that the container is restored, keep any signal for it, so that
           it is forwarded to the init by wait_for_process, rather than acted
           upon by crun.  This cannot be done earlier, as CRIU, which is run as
           a child process, would inherit the blocked signals.  */
        ret = بلاڪ_سگنل (err);
        إذا (UNLIKELY (ret < 0))
          اذهب fail;

        /* Use the container first process PID to finalize the cgroup.  */
        cg.pid = حالت.pid;
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
            محفوظ_exit (EXIT_SUCCESS);
          }

        close_and_reset (&proxy_pid_pipe0);

        /* Use the dummy process PID as the PID to setup the cgroup.  */
        cg.pid = proxy_pid;

        /* Complete the configuration for the cgroup.  It contains only the dummy process.  */
        ret = libcrun_cgroup_enter (&cg, &cgroup_حالت, err);
        إذا (UNLIKELY (ret < 0))
          اذهب fail;
      }

    ret = libcrun_cgroup_enter_finalize (&cg, cgroup_حالت, err);
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

        ret = libcrun_move_process_منزل_cgroup (proxy_pid, 0, proxy_cgroup, true, err);
        إذا (UNLIKELY (ret < 0))
          اذهب fail;

        /* Restore the container in the same cgroup where the dummy process was.  */
        حالت.cgroup_رستو = هدف_cgroup;

        ret = libcrun_container_restore_linux (&حالت, container, &cr, err);
        إذا (UNLIKELY (ret < 0))
          اذهب fail;

        /* See the comment for the other libcrun_container_restore_linux call.  */
        ret = بلاڪ_سگنل (err);
        إذا (UNLIKELY (ret < 0))
          اذهب fail;

        /* The container is running, so do not fail because of the dummy process.  */
        ret = restore_terminate_proxy_process (proxy_pid, proxy_cgroup, حالت.pid, cgroup_manager, err);
        إذا (UNLIKELY (ret < 0))
          crun_error_release (err);
        proxy_pid = -1;

        close_and_reset (&proxy_pid_pipe1);
      }
  }

  context->detach = cr_options->detach;
  ret = write_container_حالت (container, context, حالت.pid, cgroup_حالت, err);
  إذا (UNLIKELY (ret < 0))
    اذهب fail;

  {
    /* Wait the same way run and exec do: apart from waiting for the container
       init, this writes the pid file, forwards any signal to the init, and
       reaps every child, not only the init.  */
    بنية wait_for_process_دليلون دليلون = {
      .pid = حالت.pid,
      .context = context,
      .terminal_fd = -1,
      .notify_socket = -1,
      .container_ready_fd = عدم,
      .seccomp_notify_fd = -1,
      .seccomp_notify_plugins = عدم,
    };

    أرجع wait_for_process (&دليلون, err);
  }

fail:
  /* Once restored, the container is running.  Without a status file nothing
     can reach it anymore, so do not leave it behind.  */
  إذا (حالت.pid > 0)
    {
      kill (حالت.pid, SIGKILL);
      waitpid_ignore_stopped (حالت.pid, عدم, 0);
    }
  إذا (cgroup_حالت)
    {
      libcrun_error_t tmp_err = عدم;
      libcrun_cgroup_destroy (cgroup_حالت, &tmp_err);
      crun_error_release (&tmp_err);
    }
  {
    libcrun_error_t tmp_err = عدم;

    إذا (libcrun_container_delete_حالت (context->state_پاڙ, context->id, &tmp_err) < 0)
      crun_error_release (&tmp_err);
  }
  أرجع ret;
}

صحيح
libcrun_container_read_pids (libcrun_context_t *context, ثابت حرف *id, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  cleanup_cgroup_حالت بنية libcrun_cgroup_حالت *cgroup_حالت = عدم;
  cleanup_container_حالت libcrun_container_حالت_t حالت = {};
  صحيح ret;

  ret = libcrun_read_container_حالت (&حالت, context->state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (حالت.cgroup_رستو == عدم || حالت.cgroup_رستو[0] == '\0')
    أرجع crun_make_error (err, 0, "the container is not using cgroups");

  cgroup_حالت = libcrun_cgroup_make_حالت (&حالت);

  أرجع libcrun_cgroup_read_pids (cgroup_حالت, recurse, pids, err);
}

صحيح
libcrun_container_list_json (libcrun_context_t *context, حرف **ٻاهريون, libcrun_error_t *err)
{
  libcrun_container_list_t *list = عدم, *it;
  ثابت حرف *content = عدم;
  json_gen_ctx *gen = عدم;
  size_t len;
  صحيح ret;

  *ٻاهريون = عدم;

  ret = libcrun_get_containers_list (&list, context->state_پاڙ, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (! json_gen_init (&gen, عدم))
    {
      ret = crun_make_error (err, 0, "cannot allocate json generator");
      اذهب ٻاهر_نڪرو;
    }

  json_gen_config (gen, json_gen_beautify, 1);
  json_gen_array_open (gen);

  لكل (it = list; it; it = it->ايندڙ)
    {
      libcrun_container_حالت_t حالت;
      صحيح running = 0;
      صحيح pid;
      ثابت حرف *container_حالت = عدم;

      ret = libcrun_read_container_حالت (&حالت, context->state_پاڙ, it->name, err);
      إذا (UNLIKELY (ret < 0))
        اذهب ٻاهر_نڪرو;

      pid = حالت.pid;

      ret = libcrun_get_container_state_string (it->name, &حالت, context->state_پاڙ, &container_حالت,
                                                &running, err);
      إذا (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          استمر;
        }

      إذا (! running)
        pid = 0;

      json_gen_map_open (gen);
      json_gen_string (gen, "id", لڙي_ڊيگهه ("id"));
      json_gen_string (gen, it->name, لڙي_ڊيگهه (it->name));
      json_gen_string (gen, "pid", لڙي_ڊيگهه ("pid"));
      map_int (gen, pid);
      json_gen_string (gen, "status", لڙي_ڊيگهه ("status"));
      json_gen_string (gen, container_حالت, لڙي_ڊيگهه (container_حالت));
      json_gen_string (gen, "bundle", لڙي_ڊيگهه ("bundle"));
      json_gen_string (gen, حالت.bundle, لڙي_ڊيگهه (حالت.bundle));
      json_gen_string (gen, "created", لڙي_ڊيگهه ("created"));
      json_gen_string (gen, حالت.created, لڙي_ڊيگهه (حالت.created));
      json_gen_string (gen, "owner", لڙي_ڊيگهه ("owner"));
      json_gen_string (gen, حالت.owner, محفوظ_strlen (حالت.owner));
      json_gen_map_close (gen);

      libcrun_free_container_حالت (&حالت);
    }

  json_gen_array_close (gen);
  إذا (json_gen_get_buf (gen, &content, &len) != json_gen_حالت_ok)
    {
      ret = libcrun_make_error (err, 0, "cannot generate json list");
      اذهب ٻاهر_نڪرو;
    }

  *ٻاهريون = xstrdup (content);
  ret = 0;

ٻاهر_نڪرو:
  إذا (list)
    libcrun_free_containers_list (list);
  إذا (gen)
    json_gen_free (gen);

  أرجع ret;
}

صحيح
libcrun_write_json_containers_list (libcrun_context_t *context, FILE *ٻاهريون, libcrun_error_t *err)
{
  cleanup_free حرف *content = عدم;
  size_t len;
  صحيح ret;

  ret = libcrun_container_list_json (context, &content, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  len = لڙي_ڊيگهه (content);
  إذا (len && fwrite (content, 1, len, ٻاهريون) != len)
    أرجع crun_make_error (err, errno, "error writing to file");

  أرجع 0;
}

صحيح
libcrun_container_update_intel_rdt (libcrun_context_t *context, ثابت حرف *id, بنية libcrun_intel_rdt_update *update, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = عدم;
  صحيح ret;

  ret = read_container_config_from_state (&container, context->state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع libcrun_update_intel_rdt (id, container, update->l3_cache_schema, update->mem_bw_schema, update->schemata, err);
}

ساكن صحيح
libcrun_container_جوڙيو_or_remove_mounts_from_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *file, bool جوڙيو, libcrun_error_t *err)
{
  cleanup_custom_handler_instance بنية custom_handler_instance_s *custom_handler = عدم;
  cleanup_container libcrun_container_t *container = عدم;
  cleanup_free runtime_spec_schema_defs_mount **mounts = عدم;
  ثابت حرف *state_پاڙ = context->state_پاڙ;
  cleanup_free parser_error parser_err = عدم;
  بنية parser_context pctx = { 0, stderr };
  libcrun_container_حالت_t حالت = {};
  cleanup_free حرف *content = عدم;
  json_object *doc = عدم;
  size_t n_mounts = 0, len, i;
  صحيح ret = 1;

  ret = read_all_file (file, &content, &len, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_read_container_حالت (&حالت, state_پاڙ, id, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = read_container_config_from_state (&container, state_پاڙ, id, err);
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
      n_mounts = json_object_array_ڊيگهائي (doc);

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

  إذا (جوڙيو)
    ret = libcrun_make_runtime_mounts (container, &حالت, mounts, n_mounts, err);
  وإلا
    ret = libcrun_destroy_runtime_mounts (container, &حالت, mounts, n_mounts, err);

cleanup:
  إذا (doc)
    json_object_put (doc);
  إذا (mounts)
    لكل (i = 0; i < n_mounts; i++)
      free_runtime_spec_schema_defs_mount (mounts[i]);
  أرجع ret;
}

صحيح
libcrun_container_جوڙيو_mounts_from_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *file, libcrun_error_t *err)
{
  أرجع libcrun_container_جوڙيو_or_remove_mounts_from_file (context, id, file, true, err);
}

صحيح
libcrun_container_remove_mounts_from_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *file, libcrun_error_t *err)
{
  أرجع libcrun_container_جوڙيو_or_remove_mounts_from_file (context, id, file, false, err);
}
