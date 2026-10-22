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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <ocispec/runtime_spec_schema_config_schema.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ "container.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "json_gen_utils.h"
#ಅನ್ತರ್ಭಾವ "seccomp.h"
#ಅನ್ತರ್ಭಾವ "mempolicy.h"
#ifdef HAVE_SECCOMP
#  include <seccomp.h>
#endif
#ಅನ್ತರ್ಭಾವ "scheduler.h"
#ಅನ್ತರ್ಭಾವ "seccomp_notify.h"
#ಅನ್ತರ್ಭಾವ "custom-handler.h"
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <argp.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <sched.h>
#ಅನ್ತರ್ಭಾವ <sys/wait.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ "status.h"
#ಅನ್ತರ್ಭಾವ "mount_flags.h"
#ಅನ್ತರ್ಭಾವ "linux.h"
#ಅನ್ತರ್ಭಾವ "terminal.h"
#ಅನ್ತರ್ಭಾವ "io_priority.h"
#ಅನ್ತರ್ಭಾವ "cgroup.h"
#ಅನ್ತರ್ಭಾವ "cgroup-utils.h"
#ಅನ್ತರ್ಭಾವ <sys/prctl.h>
#ಅನ್ತರ್ಭಾವ <sys/auxv.h>
#ಅನ್ತರ್ಭಾವ <sys/signalfd.h>
#ಅನ್ತರ್ಭಾವ <sys/epoll.h>
#ಅನ್ತರ್ಭಾವ <sys/socket.h>
#ifdef HAVE_CAP
#  include <sys/capability.h>
#endif
#ಅನ್ತರ್ಭಾವ <sys/ioctl.h>
#ಅನ್ತರ್ಭಾವ <termios.h>
#ಅನ್ತರ್ಭಾವ <grp.h>
#ಅನ್ತರ್ಭಾವ <git-version.h>

#ifdef HAVE_SYSTEMD
#  include <systemd/sd-daemon.h>
#endif

#ಅನ್ತರ್ಭಾವ <json-c/json.h>
#ಅನ್ತರ್ಭಾವ <ocispec/json_common.h>

ಗಣನಾ
{
  SYNC_SOCKET_SYNC_MESSAGE,
  SYNC_SOCKET_ERROR_MESSAGE,
  SYNC_SOCKET_WARNING_MESSAGE,
  SYNC_SOCKET_DEBUG_MESSAGE,
};

ಸಂರಚನಾ container_entrypoint_s
{
  libcrun_container_t *container;
  libcrun_context_t *context;
  ಪೂರ್ಣಾಙ್ಕ has_terminal_socket_pair;
  ಪೂರ್ಣಾಙ್ಕ terminal_socketpair[2];

  /* Used by log_write_to_sync_socket.  */
  ಪೂರ್ಣಾಙ್ಕ sync_socket;

  ಪೂರ್ಣಾಙ್ಕ seccomp_fd;
  ಪೂರ್ಣಾಙ್ಕ seccomp_receiver_fd;
  ಪೂರ್ಣಾಙ್ಕ console_socket_fd;

  ಪೂರ್ಣಾಙ್ಕ hooks_out_fd;
  ಪೂರ್ಣಾಙ್ಕ hooks_err_fd;

  /* Absolute bundle path, to be reported to hooks run in the container.  */
  ಧ್ರುವ ವರ್ಣ *bundle;

  ಸಂರಚನಾ custom_handler_instance_s *custom_handler;
};

ಸಂರಚನಾ sync_socket_message_s
{
  ಪೂರ್ಣಾಙ್ಕ type;
  ಪೂರ್ಣಾಙ್ಕ error_ಮೌಲ್ಯ;
  ವರ್ಣ message[512];
};

ಪ್ರಕಾರನಿರ್ದೇಶ runtime_spec_schema_defs_hook hook;

// linux hooks
ವರ್ಣ *hooks[] = {
  "prestart",
  "createRuntime",
  "createContainer",
  "startContainer",
  "poststart",
  "poststop"
};

// linux namespaces
ಸ್ಥಿರ ವರ್ಣ *namespaces[] = {
  "cgroup",
  "ipc",
  "mount",
  "network",
  "pid",
  "user",
  "uts"
};

ಸ್ಥಿರ ವರ್ಣ *actions[] = {
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

ಸ್ಥಿರ ವರ್ಣ *operators[] = {
  "SCMP_CMP_NE",
  "SCMP_CMP_LT",
  "SCMP_CMP_LE",
  "SCMP_CMP_EQ",
  "SCMP_CMP_GE",
  "SCMP_CMP_GT",
  "SCMP_CMP_MASKED_EQ",
};

ಸ್ಥಿರ ವರ್ಣ *archs[] = {
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

ಸ್ಥಿರ ವರ್ಣ *mempolicy_modes[] = {
  "MPOL_DEFAULT",
  "MPOL_PREFERRED",
  "MPOL_BIND",
  "MPOL_INTERLEAVE",
  "MPOL_LOCAL",
  "MPOL_PREFERRED_MANY",
  "MPOL_WEIGHTED_INTERLEAVE"
};

ಸ್ಥಿರ ವರ್ಣ *mempolicy_flags[] = {
  "MPOL_F_NUMA_BALANCING",
  "MPOL_F_RELATIVE_NODES",
  "MPOL_F_STATIC_NODES"
};

ಸ್ಥಿರ ವರ್ಣ *potentially_unsafe_annotations[] = {
  "module.wasm.image/variant",
  "io.kubernetes.cri.container-type",
  "run.oci.",
  "org.criu.",
  "krun.",
};

#define SYNC_SOCKET_MESSAGE_LEN(x, l) (offsetof (ಸಂರಚನಾ sync_socket_message_s, message) + l)

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
sync_socket_write_ಸಂದೇಶ (ಪೂರ್ಣಾಙ್ಕ fd, ಪೂರ್ಣಾಙ್ಕ verbosity, ಪೂರ್ಣಾಙ್ಕ err_ಮೌಲ್ಯ, ಧ್ರುವ ವರ್ಣ *log_ಸಂದೇಶ)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  size_t err_len;
  ಸಂರಚನಾ sync_socket_message_s ಸಂದೇಶ;
  ವಿಕಲ್ಪ (verbosity)
    {
    ಪಕ್ಷ LIBCRUN_VERBOSITY_DEBUG:
      ಸಂದೇಶ.type = SYNC_SOCKET_DEBUG_MESSAGE;
      ಭಙ್ಗ;
    ಪಕ್ಷ LIBCRUN_VERBOSITY_WARNING:
      ಸಂದೇಶ.type = SYNC_SOCKET_WARNING_MESSAGE;
      ಭಙ್ಗ;
    ಪಕ್ಷ LIBCRUN_VERBOSITY_ERROR:
      ಸಂದೇಶ.type = SYNC_SOCKET_ERROR_MESSAGE;
      ಭಙ್ಗ;
    }
  ಸಂದೇಶ.error_ಮೌಲ್ಯ = err_ಮೌಲ್ಯ;

  ಯದಿ (fd < 0)
    ಪ್ರತ್ಯಾಗಮ 0;

  err_len = strlen (log_ಸಂದೇಶ);
  ಯದಿ (err_len >= ಪರಿಮಾಣ (ಸಂದೇಶ.message))
    err_len = ಪರಿಮಾಣ (ಸಂದೇಶ.message) - 1;

  memcpy (ಸಂದೇಶ.message, log_ಸಂದೇಶ, err_len);
  ಸಂದೇಶ.message[err_len] = '\0';

  ret = TEMP_FAILURE_RETRY (ಲಿಖ (fd, &ಸಂದೇಶ, SYNC_SOCKET_MESSAGE_LEN (ಸಂದೇಶ, err_len + 1)));
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ -1;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
sync_socket_write_error (ಪೂರ್ಣಾಙ್ಕ fd, libcrun_error_t *out_err)
{
  ಯದಿ (fd < 0)
    ಪ್ರತ್ಯಾಗಮ 0;
  ಪ್ರತ್ಯಾಗಮ sync_socket_write_ಸಂದೇಶ (fd, false, (*out_err)->status, (*out_err)->ಸಂದೇಶ);
}

ಸ್ಥಿರ ಶೂನ್ಯ
log_write_to_sync_socket (ಪೂರ್ಣಾಙ್ಕ errno_, ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ, ಪೂರ್ಣಾಙ್ಕ verbosity, ಶೂನ್ಯ *arg)
{
  ಸಂರಚನಾ container_entrypoint_s *entrypoint_args = arg;
  ಪೂರ್ಣಾಙ್ಕ fd = entrypoint_args->sync_socket;

  ಯದಿ (fd < 0)
    ಪ್ರತ್ಯಾಗಮ;

  ಯದಿ (sync_socket_write_ಸಂದೇಶ (fd, verbosity, errno_, ಸಂದೇಶ) < 0)
    log_write_to_stderr (errno_, ಸಂದೇಶ, verbosity, arg);
}

ಸ್ಥಿರ bool
is_memory_limit_too_ಕೆಳಗಿನ (runtime_spec_schema_config_schema *def)
{
  ಧ್ರುವ long memory_limit_too_ಕೆಳಗಿನ = 1024 * 1024;

  ಯದಿ (def->linux == ರಿಕ್ತ || def->linux->resources == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ false;

  ಯದಿ (def->linux->resources->memory
      && def->linux->resources->memory->limit_present
      && def->linux->resources->memory->limit < memory_limit_too_ಕೆಳಗಿನ)
    ಪ್ರತ್ಯಾಗಮ true;

  ಯದಿ (def->linux->resources->unified)
    {
      size_t i;

      ಪ್ರತಿ (i = 0; i < def->linux->resources->unified->len; i++)
        ಯದಿ (strcmp (def->linux->resources->unified->ಕೀಲಿಗಳು[i], "memory.max") == 0)
          {
            long limit;

            errno = 0;
            limit = strtol (def->linux->resources->unified->ಮೌಲ್ಯಗಳು[i], ರಿಕ್ತ, 10);
            ಯದಿ (errno != 0)
              ಪ್ರತ್ಯಾಗಮ false;
            ಯದಿ (limit < memory_limit_too_ಕೆಳಗಿನ)
              ಪ್ರತ್ಯಾಗಮ true;
          }
    }

  ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
sync_socket_wait_sync (libcrun_context_t *context, ಪೂರ್ಣಾಙ್ಕ fd, bool flush, libcrun_error_t *err)
{
  ಸಂರಚನಾ sync_socket_message_s ಸಂದೇಶ;

  ಯದಿ (fd < 0)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯಾವತ್ (true)
    {
      ಪೂರ್ಣಾಙ್ಕ ret;

      ret = TEMP_FAILURE_RETRY (ಪಠ (fd, &ಸಂದೇಶ, ಪರಿಮಾಣ (ಸಂದೇಶ)));
      ಯದಿ (UNLIKELY (ret < 0))
        {
          ಯದಿ (flush)
            ಪ್ರತ್ಯಾಗಮ 0;
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "read from sync socket");
        }

      ಯದಿ (ret == 0)
        {
          ಯದಿ (flush)
            ಪ್ರತ್ಯಾಗಮ 0;

          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "read from the init process");
        }

      ಯದಿ (! flush && ಸಂದೇಶ.type == SYNC_SOCKET_SYNC_MESSAGE)
        ಪ್ರತ್ಯಾಗಮ 0;
      ಅನ್ಯಥಾ ಯದಿ (ಸಂದೇಶ.type == SYNC_SOCKET_DEBUG_MESSAGE)
        {
          ಯದಿ (context)
            context->output_handler (ಸಂದೇಶ.error_ಮೌಲ್ಯ, ಸಂದೇಶ.message, LIBCRUN_VERBOSITY_DEBUG, context->output_handler_arg);
          ಅನುವರ್ತ;
        }
      ಅನ್ಯಥಾ ಯದಿ (ಸಂದೇಶ.type == SYNC_SOCKET_WARNING_MESSAGE)
        {
          ಯದಿ (context)
            context->output_handler (ಸಂದೇಶ.error_ಮೌಲ್ಯ, ಸಂದೇಶ.message, LIBCRUN_VERBOSITY_WARNING, context->output_handler_arg);
          ಅನುವರ್ತ;
        }
      ಅನ್ಯಥಾ ಯದಿ (ಸಂದೇಶ.type == SYNC_SOCKET_ERROR_MESSAGE)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ಸಂದೇಶ.error_ಮೌಲ್ಯ, "%s", ಸಂದೇಶ.message);
    }
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
sync_socket_send_sync (ಪೂರ್ಣಾಙ್ಕ fd, bool flush_errors, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಸಂರಚನಾ sync_socket_message_s ಸಂದೇಶ = {
    0,
  };
  ಸಂದೇಶ.type = SYNC_SOCKET_SYNC_MESSAGE;

  ಯದಿ (fd < 0)
    ಪ್ರತ್ಯಾಗಮ 0;

  ret = TEMP_FAILURE_RETRY (ಲಿಖ (fd, &ಸಂದೇಶ, SYNC_SOCKET_MESSAGE_LEN (ಸಂದೇಶ, 0)));
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ಯದಿ (flush_errors)
        {
          ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
          ret = TEMP_FAILURE_RETRY (ಪಠ (fd, &ಸಂದೇಶ, ಪರಿಮಾಣ (ಸಂದೇಶ)));
          ಯದಿ (ret >= 0 && ಸಂದೇಶ.type == SYNC_SOCKET_ERROR_MESSAGE)
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ಸಂದೇಶ.error_ಮೌಲ್ಯ, "%s", ಸಂದೇಶ.message);
          errno = saved_errno;
        }
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "write to sync socket");
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ bool
has_cap_sys_ptrace ()
{
#ifdef HAVE_CAP
  ಸಂರಚನಾ __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  ಸಂರಚನಾ __user_cap_data_struct data[2] = { { 0 } };

  ಯದಿ (UNLIKELY (capget (&hdr, data) < 0))
    ಪ್ರತ್ಯಾಗಮ true;

  ಪ್ರತ್ಯಾಗಮ (data[0].effective & (1U << CAP_SYS_PTRACE)) != 0;
#ಅನ್ಯಥಾ
  ಪ್ರತ್ಯಾಗಮ geteuid () == 0;
#endif
}

ಸ್ಥಿರ libcrun_container_t *
make_container (runtime_spec_schema_config_schema *container_def, ಧ್ರುವ ವರ್ಣ *path, ಧ್ರುವ ವರ್ಣ *config)
{
  libcrun_container_t *container = xmalloc0 (ಪರಿಮಾಣ (*container));
  container->container_def = container_def;

  container->host_uid = geteuid ();
  container->host_gid = getegid ();
  container->host_has_cap_sys_ptrace = has_cap_sys_ptrace ();
  container->proc_fd = -1;

  container->annotations = make_string_map_from_json (container_def->annotations);

  /* An empty namespace path means that a new namespace is to be created,
     the same as if it is not specified.  */
  ಯದಿ (container_def->linux)
    {
      size_t i;

      ಪ್ರತಿ (i = 0; i < container_def->linux->namespaces_len; i++)
        ಯದಿ (container_def->linux->namespaces[i]->path && container_def->linux->namespaces[i]->path[0] == '\0')
          {
            ಮುಕ್ತ (container_def->linux->namespaces[i]->path);
            container_def->linux->namespaces[i]->path = ರಿಕ್ತ;
          }
    }

  ಯದಿ (path)
    container->config_file = xstrdup (path);
  ಯದಿ (config)
    container->config_file_content = xstrdup (config);

  ಪ್ರತ್ಯಾಗಮ container;
}

libcrun_container_t *
libcrun_container_load_from_memory (ಧ್ರುವ ವರ್ಣ *json, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free ವರ್ಣ *oci_error = ರಿಕ್ತ;
  container_def = runtime_spec_schema_config_schema_parse_data (json, ರಿಕ್ತ, &oci_error);
  ಯದಿ (container_def == ರಿಕ್ತ)
    {
      crun_make_error (err, 0, "load: `%s`", oci_error);
      ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }
  ಪ್ರತ್ಯಾಗಮ make_container (container_def, ರಿಕ್ತ, json);
}

libcrun_container_t *
libcrun_container_load_from_file (ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free ವರ್ಣ *oci_error = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *content = ರಿಕ್ತ;
  size_t len;
  ಪೂರ್ಣಾಙ್ಕ ret;

  libcrun_debug ("Loading container from config file: `%s`", path);
  ret = read_all_file (path, &content, &len, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

  container_def = runtime_spec_schema_config_schema_parse_data (content, ರಿಕ್ತ, &oci_error);
  ಯದಿ (container_def == ರಿಕ್ತ)
    {
      crun_make_error (err, 0, "load `%s`: %s", path, oci_error);
      ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }
  ಪ್ರತ್ಯಾಗಮ make_container (container_def, ರಿಕ್ತ, content);
}

ಧ್ರುವ ವರ್ಣ *
libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err)
{
  ಸಂರಚನಾ parser_context ctx = { 0, stderr };
  parser_error gen_err = ರಿಕ್ತ;

  ಯದಿ (container == ರಿಕ್ತ)
    {
      crun_make_error (err, EINVAL, "no container specified");
      ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

  ಯದಿ (container->config_file_content == ರಿಕ್ತ && container->container_def)
    {
      container->config_file_content = runtime_spec_schema_config_schema_generate_json (container->container_def, &ctx, &gen_err);
      ಯದಿ (container->config_file_content == ರಿಕ್ತ)
        {
          crun_make_error (err, 0, "cannot generate the configuration JSON: %s", gen_err ?: "unknown error");
          ಮುಕ್ತ (gen_err);
          ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }
      ಮುಕ್ತ (gen_err);
    }

  ಯದಿ (container->config_file_content == ರಿಕ್ತ)
    {
      crun_make_error (err, 0, "the container has no configuration");
      ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

  ಪ್ರತ್ಯಾಗಮ container->config_file_content;
}

ಧ್ರುವ ವರ್ಣ *
libcrun_container_get_annotation (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ)
{
  ಯದಿ (container == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
  ಪ್ರತ್ಯಾಗಮ find_string_map_ಮೌಲ್ಯ (container->annotations, ಕೀಲಿ);
}

size_t
libcrun_container_get_annotations_len (libcrun_container_t *container)
{
  ಯದಿ (container == ರಿಕ್ತ || container->annotations == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;
  ಪ್ರತ್ಯಾಗಮ string_map_size (container->annotations);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_get_annotation_at (libcrun_container_t *container, size_t index, ಧ್ರುವ ವರ್ಣ **ಕೀಲಿ,
                                     ಧ್ರುವ ವರ್ಣ **ಮೌಲ್ಯ)
{
  ಯದಿ (container == ರಿಕ್ತ)
    {
      errno = ERANGE;
      ಪ್ರತ್ಯಾಗಮ -1;
    }
  ಪ್ರತ್ಯಾಗಮ string_map_get_at (container->annotations, index, ಕೀಲಿ, ಮೌಲ್ಯ);
}

uid_t
libcrun_container_get_uid (libcrun_container_t *container)
{
  ಯದಿ (container == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ (uid_t) -1;
  ಪ್ರತ್ಯಾಗಮ container->host_uid;
}

gid_t
libcrun_container_get_gid (libcrun_container_t *container)
{
  ಯದಿ (container == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ (gid_t) -1;
  ಪ್ರತ್ಯಾಗಮ container->host_gid;
}

ಶೂನ್ಯ
libcrun_container_free (libcrun_container_t *ctr)
{
  ಯದಿ (ctr == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ;

  ಯದಿ (ctr->cleanup_private_data)
    ctr->cleanup_private_data (ctr->private_data);

  ಯದಿ (ctr->container_def)
    free_runtime_spec_schema_config_schema (ctr->container_def);

  free_string_map (ctr->annotations);

  ಯದಿ (ctr->proc_fd >= 0)
    close (ctr->proc_fd);

  ಮುಕ್ತ (ctr->config_file_content);
  ಮುಕ್ತ (ctr->config_file);
  ಮುಕ್ತ (ctr);
}

libcrun_context_t *
libcrun_context_new (ಧ್ರುವ ವರ್ಣ *id, ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, libcrun_error_t *err)
{
  libcrun_context_t *ctx = xmalloc0 (ಪರಿಮಾಣ (*ctx));

  /* Go through the setters so that the owned_* copies and the borrowed
     pointers stay in sync and are released by libcrun_context_free.  */
  libcrun_context_set_id (ctx, id);
  libcrun_context_set_state_ಬೇರು (ctx, state_ಬೇರು);
  libcrun_context_set_bundle (ctx, ".");
  ctx->fifo_exec_wait_fd = -1;

  ಯದಿ (libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, id, ರಿಕ್ತ, err) < 0)
    {
      libcrun_context_free (ctx);
      ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

  ctx->handler_manager = libcrun_handler_manager_create (err);
  ಯದಿ (ctx->handler_manager == ರಿಕ್ತ)
    {
      libcrun_context_free (ctx);
      ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

#ifdef CRUN_LIBDIR
  ಯದಿ (access (CRUN_LIBDIR "/handlers", F_OK) == 0)
    {
      ಯದಿ (libcrun_handler_manager_load_directory (ctx->handler_manager, CRUN_LIBDIR "/handlers", err) < 0)
        {
          libcrun_context_free (ctx);
          ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }
    }
#endif

  ಪ್ರತ್ಯಾಗಮ ctx;
}

ಶೂನ್ಯ
libcrun_context_free (libcrun_context_t *ctx)
{
  ಯದಿ (ctx == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ;

  ಮುಕ್ತ (ctx->owned_state_ಬೇರು);
  ಮುಕ್ತ (ctx->owned_id);
  ಮುಕ್ತ (ctx->owned_bundle);
  ಮುಕ್ತ (ctx->owned_console_socket);
  ಮುಕ್ತ (ctx->owned_pid_file);
  ಮುಕ್ತ (ctx->owned_notify_socket);
  ಮುಕ್ತ (ctx->owned_handler);

  ಯದಿ (ctx->handler_manager)
    handler_manager_free (ctx->handler_manager);

  ಮುಕ್ತ (ctx);
}

#define LIBCRUN_CONTEXT_SET_STRING(field)                  \
  ಕುರು                                                       \
    {                                                      \
      ಮುಕ್ತ (ctx->owned_##field);                           \
      ctx->owned_##field = ಮೌಲ್ಯ ? xstrdup (ಮೌಲ್ಯ) : ರಿಕ್ತ; \
      ctx->field = ctx->owned_##field;                     \
  } ಯಾವತ್ (0)

ಶೂನ್ಯ
libcrun_context_set_id (libcrun_context_t *ctx, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
  LIBCRUN_CONTEXT_SET_STRING (id);
}

ಶೂನ್ಯ
libcrun_context_set_state_ಬೇರು (libcrun_context_t *ctx, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
  LIBCRUN_CONTEXT_SET_STRING (state_ಬೇರು);
}

ಶೂನ್ಯ
libcrun_context_set_bundle (libcrun_context_t *ctx, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
  LIBCRUN_CONTEXT_SET_STRING (bundle);
}

ಶೂನ್ಯ
libcrun_context_set_console_socket (libcrun_context_t *ctx, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
  LIBCRUN_CONTEXT_SET_STRING (console_socket);
}

ಶೂನ್ಯ
libcrun_context_set_pid_file (libcrun_context_t *ctx, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
  LIBCRUN_CONTEXT_SET_STRING (pid_file);
}

ಶೂನ್ಯ
libcrun_context_set_notify_socket (libcrun_context_t *ctx, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
  LIBCRUN_CONTEXT_SET_STRING (notify_socket);
}

ಶೂನ್ಯ
libcrun_context_set_handler (libcrun_context_t *ctx, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
  LIBCRUN_CONTEXT_SET_STRING (handler);
}

#undef LIBCRUN_CONTEXT_SET_STRING

ಧ್ರುವ ವರ್ಣ *
libcrun_context_get_pid_file (libcrun_context_t *ctx)
{
  ಪ್ರತ್ಯಾಗಮ ctx->pid_file;
}

ಧ್ರುವ ವರ್ಣ *
libcrun_context_get_state_ಬೇರು (libcrun_context_t *ctx)
{
  ಯದಿ (ctx == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
  ಪ್ರತ್ಯಾಗಮ ctx->state_ಬೇರು;
}

ಶೂನ್ಯ
libcrun_context_set_preserve_fds (libcrun_context_t *ctx, ಪೂರ್ಣಾಙ್ಕ preserve_fds)
{
  ctx->preserve_fds = preserve_fds;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_context_get_preserve_fds (libcrun_context_t *ctx)
{
  ಪ್ರತ್ಯಾಗಮ ctx->preserve_fds;
}

ಶೂನ್ಯ
libcrun_context_set_listen_fds (libcrun_context_t *ctx, ಪೂರ್ಣಾಙ್ಕ listen_fds)
{
  ctx->listen_fds = listen_fds;
}

ಶೂನ್ಯ
libcrun_context_set_args (libcrun_context_t *ctx, ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ctx->argc = argc;
  ctx->argv = argv;
}

ಶೂನ್ಯ
libcrun_context_set_systemd_cgroup (libcrun_context_t *ctx, bool ಮೌಲ್ಯ)
{
  ctx->systemd_cgroup = ಮೌಲ್ಯ;
}

ಶೂನ್ಯ
libcrun_context_set_detach (libcrun_context_t *ctx, bool ಮೌಲ್ಯ)
{
  ctx->detach = ಮೌಲ್ಯ;
}

ಶೂನ್ಯ
libcrun_context_set_no_new_keyring (libcrun_context_t *ctx, bool ಮೌಲ್ಯ)
{
  ctx->no_new_keyring = ಮೌಲ್ಯ;
}

ಶೂನ್ಯ
libcrun_context_set_no_pivot (libcrun_context_t *ctx, bool ಮೌಲ್ಯ)
{
  ctx->no_pivot = ಮೌಲ್ಯ;
}

ಶೂನ್ಯ
libcrun_context_set_force_no_cgroup (libcrun_context_t *ctx, bool ಮೌಲ್ಯ)
{
  ctx->force_no_cgroup = ಮೌಲ್ಯ;
}

ಶೂನ್ಯ
libcrun_context_set_output_handler (libcrun_context_t *ctx, crun_output_handler handler, ಶೂನ್ಯ *arg)
{
  ctx->output_handler = handler;
  ctx->output_handler_arg = arg;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_context_init_logging (libcrun_context_t *ctx, ಧ್ರುವ ವರ್ಣ *log, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, ctx->id, log, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
block_signals (libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  sigset_t mask;
  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "sigprocmask");
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
unblock_signals (libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ i;
  ಪೂರ್ಣಾಙ್ಕ ret;
  sigset_t mask;
  ಸಂರಚನಾ sigaction act = {};

  sigfillset (&mask);
  ret = sigprocmask (SIG_UNBLOCK, &mask, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "sigprocmask");

  act.sa_handler = SIG_DFL;
  ಪ್ರತಿ (i = 0; i < NSIG; i++)
    {
      ret = sigaction (i, &act, ರಿಕ್ತ);
      ಯದಿ (ret < 0 && errno != EINVAL)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "sigaction");
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* must be used on the host before pivot_root(2).  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
initialize_security (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = libcrun_init_caps (container, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (UNLIKELY (proc == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯದಿ (proc->apparmor_profile)
    {
      ret = libcrun_initialize_apparmor (err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = libcrun_initialize_selinux (container, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Emit the "annotations" map.  Returns a json_gen_status; callers can wrap
   the call with GEN_OR_FAIL.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
gen_annotations (json_gen_ctx *gen, json_map_string_string *annotations)
{
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ r;

  GEN_ಕೀಲಿ (gen, "annotations");
  GEN_OR_FAIL (json_gen_map_open (gen));

  ಪ್ರತಿ (i = 0; i < annotations->len; i++)
    {
      /* An annotation whose value is null in the document is parsed into a
         NULL, which cannot be emitted as a JSON string; leave it out.  */
      ಯದಿ (annotations->ಕೀಲಿಗಳು[i] == ರಿಕ್ತ || annotations->ಮೌಲ್ಯಗಳು[i] == ರಿಕ್ತ)
        ಅನುವರ್ತ;

      GEN_STR (gen, annotations->ಕೀಲಿಗಳು[i]);
      GEN_STR (gen, annotations->ಮೌಲ್ಯಗಳು[i]);
    }

  ಪ್ರತ್ಯಾಗಮ json_gen_map_close (gen);

gen_error:
  ಪ್ರತ್ಯಾಗಮ r;
}

/* Return the bundle path to report in the container state.  crun always
   chdir()s into the bundle, so fall back to getcwd() when the caller has no
   absolute bundle path, as can happen when libcrun is used as a library.
   *ALLOCATED must be freed by the caller.  */
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
get_bundle_path (ಧ್ರುವ ವರ್ಣ *bundle, ವರ್ಣ **allocated)
{
  ಯದಿ (bundle != ರಿಕ್ತ && bundle[0] == '/')
    ಪ್ರತ್ಯಾಗಮ bundle;

  *allocated = getcwd (ರಿಕ್ತ, 0);
  ಯದಿ (*allocated == ರಿಕ್ತ)
    OOM ();
  ಪ್ರತ್ಯಾಗಮ *allocated;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_hooks (runtime_spec_schema_config_schema *def, pid_t pid, ಧ್ರುವ ವರ್ಣ *id, bool keep_going, ಧ್ರುವ ವರ್ಣ *bundle,
          ಧ್ರುವ ವರ್ಣ *cwd, ಧ್ರುವ ವರ್ಣ *status, ಧ್ರುವ ವರ್ಣ *name, hook **hooks, size_t hooks_len, ಪೂರ್ಣಾಙ್ಕ out_fd,
          ಪೂರ್ಣಾಙ್ಕ err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  size_t i, stdin_len;
  ಪೂರ್ಣಾಙ್ಕ r, ret;
  ವರ್ಣ *stdin = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *bundle_allocated = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *rootfs = def->ಬೇರು ? def->ಬೇರು->path : "";
  cleanup_json_gen json_gen_ctx *gen = ರಿಕ್ತ;

  bundle = get_bundle_path (bundle, &bundle_allocated);

  ಯದಿ (! json_gen_init (&gen, ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 0);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_ಕೀಲಿ (gen, "ociVersion");
  GEN_STR (gen, "1.0");

  GEN_ಕೀಲಿ (gen, "id");
  GEN_STR (gen, id);

  GEN_ಕೀಲಿ (gen, "pid");
  GEN_OR_FAIL (map_int (gen, pid));

  GEN_ಕೀಲಿ (gen, "root");
  GEN_STR (gen, rootfs);

  GEN_ಕೀಲಿ (gen, "bundle");
  GEN_STR (gen, bundle);

  GEN_ಕೀಲಿ (gen, "status");
  GEN_STR (gen, status);

  ಯದಿ (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, (ಧ್ರುವ ವರ್ಣ **) &stdin, &stdin_len));

  ret = 0;

  bool error_created = false;
  ಪ್ರತಿ (i = 0; i < hooks_len; i++)
    {
      ವರ್ಣ **env = environ;

      ಯದಿ (hooks[i]->env)
        env = hooks[i]->env;

      /* Release the error from the previous iteration, if any.  */
      ಯದಿ (error_created)
        {
          crun_error_release (err);
          error_created = false;
        }

      ret = run_process_with_stdin_timeout_envp (hooks[i]->path, hooks[i]->args, cwd, hooks[i]->timeout, env,
                                                 stdin, stdin_len, out_fd, err_fd, can_ignore_chdir_errors, err);
      ಯದಿ (UNLIKELY (ret < 0))
        error_created = true;

      ಯದಿ (UNLIKELY (ret != 0))
        {
          ಯದಿ (keep_going)
            libcrun_warning ("error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
          ಅನ್ಯಥಾ
            {
              libcrun_error (0, "error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
              ಭಙ್ಗ;
            }
        }
    }

  ಪ್ರತ್ಯಾಗಮ ret;

gen_error:
  ಪ್ರತ್ಯಾಗಮ json_gen_error_to_crun_error (r, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_json_gen_result (json_gen_ctx *gen, ವರ್ಣ **out, size_t *out_len)
{
  ಧ್ರುವ ವರ್ಣ *buf = ರಿಕ್ತ;
  size_t buf_len = 0;
  ಪೂರ್ಣಾಙ್ಕ r;

  r = json_gen_get_buf (gen, &buf, &buf_len);
  ಯದಿ (UNLIKELY (r != json_gen_status_ok))
    ಪ್ರತ್ಯಾಗಮ r;

  *out_len = buf_len;

  *out = xmalloc (buf_len + 1);
  memcpy (*out, buf, buf_len);
  (*out)[buf_len] = '\0';

  ಪ್ರತ್ಯಾಗಮ json_gen_status_ok;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_seccomp_receiver_fd_payload (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *status, pid_t own_pid,
                                 ವರ್ಣ **seccomp_fd_payload, size_t *seccomp_fd_payload_len, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ r;
  json_gen_ctx *gen = ರಿಕ್ತ;
  runtime_spec_schema_config_schema *def = container->container_def;
  ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ OCI_VERSION = "0.2.0";

  ಯದಿ (! json_gen_init (&gen, ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_ಕೀಲಿ (gen, "ociVersion");
  GEN_STR (gen, OCI_VERSION);

  GEN_ಕೀಲಿ (gen, "fds");
  GEN_OR_FAIL (json_gen_array_open (gen));

  GEN_ಕೀಲಿ (gen, "seccompFd");
  GEN_OR_FAIL (json_gen_array_close (gen));

  GEN_ಕೀಲಿ (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  ಯದಿ (def && def->linux && def->linux->seccomp)
    {
      ಧ್ರುವ ವರ್ಣ *metadata = def->linux->seccomp->listener_metadata;

      ಯದಿ (metadata)
        {
          GEN_ಕೀಲಿ (gen, "metadata");
          GEN_STR (gen, metadata);
        }
    }

  /* State.  */
  GEN_ಕೀಲಿ (gen, "state");
  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_ಕೀಲಿ (gen, "ociVersion");
  GEN_STR (gen, OCI_VERSION);

  ಯದಿ (container->context && container->context->id)
    {
      GEN_ಕೀಲಿ (gen, "id");
      GEN_STR (gen, container->context->id);
    }

  GEN_ಕೀಲಿ (gen, "status");
  GEN_STR (gen, status);

  GEN_ಕೀಲಿ (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  ಯದಿ (container->context && container->context->bundle)
    {
      GEN_ಕೀಲಿ (gen, "bundle");
      GEN_STR (gen, container->context->bundle);
    }

  ಯದಿ (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));
  /* End state.  */

  GEN_OR_FAIL (json_gen_map_close (gen));

  r = get_json_gen_result (gen, seccomp_fd_payload, seccomp_fd_payload_len);

gen_error:
  json_gen_free (gen);

  ಪ್ರತ್ಯಾಗಮ json_gen_error_to_crun_error (r, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
send_sync_cb (ಶೂನ್ಯ *data, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ sync_socket_fd = *((ಪೂರ್ಣಾಙ್ಕ *) data);
  ಪೂರ್ಣಾಙ್ಕ ret;

  /* sync 2.  */
  ret = sync_socket_send_sync (sync_socket_fd, false, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* sync 3.  */
  ಪ್ರತ್ಯಾಗಮ sync_socket_wait_sync (ರಿಕ್ತ, sync_socket_fd, false, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
maybe_chown_std_streams (uid_t container_uid, gid_t container_gid, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret, i;

  ಪ್ರತಿ (i = 0; i < 3; i++)
    {
      ಯದಿ (! isatty (i))
        {
          ಸಂರಚನಾ stat statbuf;
          ret = fstat (i, &statbuf);
          ಯದಿ (UNLIKELY (ret < 0))
            {
              ಯದಿ (errno == EBADF)
                ಅನುವರ್ತ;
              ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fstat fd `%d`", i);
            }

          /* Skip chown for device files */
          ಯದಿ (S_ISCHR (statbuf.st_mode) || S_ISBLK (statbuf.st_mode))
            ಅನುವರ್ತ;

          ret = fchown (i, container_uid, container_gid);
          ಯದಿ (UNLIKELY (ret < 0))
            {
              /* EINVAL means the user is not mapped in the current userns.
                 Ignore EPERM and EROFS as well as there is no reason to fail
                 so early, and let the container payload deal with it.
                 EBADF means fd is closed.
              */
              ಯದಿ (errno == EINVAL || errno == EPERM || errno == EROFS || errno == EBADF)
                ಅನುವರ್ತ;

              ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fchown std stream `%i`", i);
            }
        }
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_notify_handler (ಸಂರಚನಾ container_entrypoint_s *args,
                                  ಗಣನಾ handler_configure_phase phase,
                                  libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *rootfs,
                                  libcrun_error_t *err)
{
  ಸಂರಚನಾ custom_handler_s *h;

  ಯದಿ (args->custom_handler == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  h = args->custom_handler->vtable;
  ಯದಿ (h == ರಿಕ್ತ || h->configure_container == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಪ್ರತ್ಯಾಗಮ h->configure_container (args->custom_handler->cookie, phase,
                                 args->context, container,
                                 rootfs, err);
}

/* Resolve and normalize the container rootfs path.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
resolve_rootfs_path (libcrun_container_t *container, ವರ್ಣ **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಯದಿ (def->ಬೇರು && def->ಬೇರು->path)
    {
      *rootfs = realpath (def->ಬೇರು->path, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (*rootfs == ರಿಕ್ತ))
        {
          ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;

          /* If realpath failed for any reason, try the relative directory.  */
          ಯದಿ (def->ಬೇರು->path[0] == '/')
            {
              cleanup_free ವರ್ಣ *cwd = ರಿಕ್ತ;
              libcrun_error_t tmp_err = ರಿಕ್ತ;
              ssize_t len;
              ಪೂರ್ಣಾಙ್ಕ proc_fd;

              proc_fd = libcrun_get_cached_proc_fd (container, &tmp_err);
              ಯದಿ (LIKELY (proc_fd >= 0))
                {
                  len = safe_readlinkat (proc_fd, "self/cwd", &cwd, 0, &tmp_err);
                  ಯದಿ (LIKELY (len > 0))
                    {
                      /* If the rootfs is under the current working directory, just use its relative path.  */
                      ಯದಿ (has_dir_prefix (def->ಬೇರು->path, cwd))
                        {
                          ಧ್ರುವ ವರ್ಣ *it = consume_slashes (def->ಬೇರು->path + len);
                          ಯದಿ (LIKELY (*it))
                            {
                              ಪೂರ್ಣಾಙ್ಕ ret = faccessat (AT_FDCWD, it, F_OK, 0);
                              ಯದಿ (LIKELY (ret == 0))
                                *rootfs = xstrdup (it);
                            }
                        }
                    }
                }
              crun_error_release (&tmp_err);
            }

          ಯದಿ (*rootfs == ರಿಕ್ತ)
            {
              ಯದಿ (saved_errno == ENOENT)
                ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ENOENT, "the rootfs path `%s` does not exist", def->ಬೇರು->path);

              *rootfs = xstrdup (def->ಬೇರು->path);
            }
        }
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Initialize the environment variables.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
setup_environment (runtime_spec_schema_config_schema *def, uid_t container_uid, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (clearenv ())
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "clearenv");

  ಯದಿ (def->process)
    {
      size_t i;

      ಪ್ರತಿ (i = 0; i < def->process->env_len; i++)
        {
          /* A null element of the array of variables is parsed into a NULL,
             which putenv(3) cannot be handed.  */
          ಯದಿ (def->process->env[i] == ರಿಕ್ತ)
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EINVAL, "environment variable is not specified");

          ಯದಿ (putenv (def->process->env[i]) < 0)
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "putenv `%s`", def->process->env[i]);
        }
    }

  ಯದಿ (is_empty_string (getenv ("HOME")))
    {
      ret = set_home_env (container_uid);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          ಯದಿ (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "setenv HOME");
        }
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Configure terminal and console for interactive containers.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
setup_terminal (ಸಂರಚನಾ container_entrypoint_s *entrypoint_args, libcrun_container_t *container,
                ಪೂರ್ಣಾಙ್ಕ console_socket, ಪೂರ್ಣಾಙ್ಕ console_socketpair, ಪೂರ್ಣಾಙ್ಕ has_terminal, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ terminal_fd = -1;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (! has_terminal)
    ಪ್ರತ್ಯಾಗಮ 0;

  fflush (stderr);

  ಯದಿ (console_socket >= 0 || (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0))
    {
      terminal_fd = libcrun_set_terminal (container, err);
      ಯದಿ (UNLIKELY (terminal_fd < 0))
        ಪ್ರತ್ಯಾಗಮ terminal_fd;
    }

  ಯದಿ (console_socket >= 0)
    {
      ret = send_fd_to_socket (console_socket, terminal_fd, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಅನ್ಯಥಾ ಯದಿ (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0)
    {
      ret = send_fd_to_socket (console_socketpair, terminal_fd, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Resolve and validate the container's executable path.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
setup_executable_path (ಸಂರಚನಾ container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                       ವರ್ಣ **exec_path, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (def->process && def->process->args)
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          ಯದಿ (entrypoint_args->custom_handler == ರಿಕ್ತ && crun_error_get_errno (err) == ENOENT)
            ಪ್ರತ್ಯಾಗಮ ret;

          /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
             once the process switched to the use that runs in the container.  This might be necessary
             when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
             is not honored.  */
          crun_error_release (err);
        }
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Apply security settings for the container process.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
apply_security_settings (ಸಂರಚನಾ container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                         libcrun_container_t *container, pid_t own_pid, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  runtime_spec_schema_config_schema_process_capabilities *capabilities;
  ಪೂರ್ಣಾಙ್ಕ no_new_privs;

  ಯದಿ (def->process && ! def->process->no_new_privileges)
    {
      ವರ್ಣ **seccomp_flags = ರಿಕ್ತ;
      size_t seccomp_flags_len = 0;
      cleanup_free ವರ್ಣ *seccomp_fd_payload = ರಿಕ್ತ;
      size_t seccomp_fd_payload_len = 0;

      ಯದಿ (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      ಯದಿ (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  capabilities = def->process ? def->process->capabilities : ರಿಕ್ತ;
  no_new_privs = def->process ? def->process->no_new_privileges : 1;
  ret = libcrun_set_caps (capabilities, container->container_uid, container->container_gid, no_new_privs,
                          entrypoint_args->custom_handler != ರಿಕ್ತ, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Create the process that joins the existing PID namespace now that the
   host mounts are not visible anymore, and continue the setup from there.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_delayed_pidns_fork (ಪೂರ್ಣಾಙ್ಕ sync_socket, pid_t *own_pid, libcrun_error_t *err)
{
  pid_t pid;
  ಪೂರ್ಣಾಙ್ಕ ret;

  pid = fork ();
  ಯದಿ (UNLIKELY (pid < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fork");

  ಯದಿ (pid)
    {
      /* Report the new PID to the parent and exit immediately.  */
      ret = sync_socket_send_sync (sync_socket, false, err);
      ಯದಿ (LIKELY (ret == 0))
        {
          ret = TEMP_FAILURE_RETRY (ಲಿಖ (sync_socket, &pid, ಪರಿಮಾಣ (pid)));
          ಯದಿ (LIKELY (ret == ಪರಿಮಾಣ (pid)))
            _safe_exit (EXIT_SUCCESS);
        }

      kill (pid, SIGKILL);
      _safe_exit (EXIT_FAILURE);
    }

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (ಪಠ (sync_socket, own_pid, ಪರಿಮಾಣ (*own_pid)));
  ಯದಿ (UNLIKELY (ret != ಪರಿಮಾಣ (*own_pid)))
    {
      ಯದಿ (ret >= 0)
        errno = 0;
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "read from sync socket");
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ delayed_pidns_fork_s
{
  libcrun_container_t *container;
  ಪೂರ್ಣಾಙ್ಕ sync_socket;
  pid_t *own_pid;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
delayed_pidns_fork_cb (ಶೂನ್ಯ *data, libcrun_error_t *err)
{
  ಸಂರಚನಾ delayed_pidns_fork_s *d = data;

  ಯದಿ (! libcrun_container_delayed_pidns_fork (d->container))
    ಪ್ರತ್ಯಾಗಮ 0;

  ಪ್ರತ್ಯಾಗಮ do_delayed_pidns_fork (d->sync_socket, d->own_pid, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
container_init_setup (ಶೂನ್ಯ *args, pid_t *own_pid, ವರ್ಣ *notify_socket,
                      ಪೂರ್ಣಾಙ್ಕ sync_socket, ವರ್ಣ **exec_path, libcrun_error_t *err)
{
  ಸಂರಚನಾ container_entrypoint_s *entrypoint_args = args;
  libcrun_container_t *container = entrypoint_args->container;
  bool chdir_done = false;
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಪೂರ್ಣಾಙ್ಕ has_terminal;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ console_socket = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ console_socketpair = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free ವರ್ಣ *rootfs = ರಿಕ್ತ;
  ಸಂರಚನಾ delayed_pidns_fork_s delayed_pidns_fork = {
    .container = container,
    .sync_socket = sync_socket,
    .own_pid = own_pid,
  };

  ret = initialize_security (container, def->process, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_configure_network (container, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = resolve_rootfs_path (container, &rootfs, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* Configure terminal socket pair for container communication.  */
  ಯದಿ (entrypoint_args->terminal_socketpair[0] >= 0)
    {
      close_and_reset (&entrypoint_args->terminal_socketpair[0]);
      console_socketpair = entrypoint_args->terminal_socketpair[1];
    }

  /* sync 1.  */
  ret = sync_socket_wait_sync (ರಿಕ್ತ, sync_socket, false, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  has_terminal = container->container_def->process && container->container_def->process->terminal;
  ಯದಿ (has_terminal && entrypoint_args->context->console_socket)
    console_socket = entrypoint_args->console_socket_fd;

  ret = libcrun_set_sysctl (container, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_BEFORE_MOUNTS, container, rootfs, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_do_pivot_ಬೇರು (container, entrypoint_args->context->no_pivot, &rootfs, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* sync 2 and 3 are sent as part of libcrun_set_mounts.  */
  ret = libcrun_set_mounts (entrypoint_args, container, rootfs, send_sync_cb, &sync_socket, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* The createContainer hooks MUST run after the mounts are set up but before
     the pivot_root.  When these hooks are present can_use_open_tree_namespace()
     forces the deferred-pivot path, so the actual pivot happens later in
     libcrun_finalize_mounts().  */
  ಯದಿ (def->hooks && def->hooks->create_container_len)
    {
      libcrun_error_t tmp_err = ರಿಕ್ತ;
      ಪೂರ್ಣಾಙ್ಕ in_userns = check_running_in_user_namespace (&tmp_err);
      ಯದಿ (tmp_err)
        crun_error_release (&tmp_err);
      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, ರಿಕ್ತ, "created", "createContainer",
                      (hook **) def->hooks->create_container,
                      def->hooks->create_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      ಯದಿ (UNLIKELY (ret != 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = libcrun_finalize_mounts (entrypoint_args, container, rootfs, delayed_pidns_fork_cb, &delayed_pidns_fork, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (def->process)
    {
      ret = libcrun_set_selinux_label (container, def->process, false, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = libcrun_set_apparmor_profile (container, def->process, false, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = mark_or_close_fds_ge_than (container, entrypoint_args->context->preserve_fds + 3, false, err);
  ಯದಿ (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ret = libcrun_reopen_dev_null (err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = maybe_chown_std_streams (container->container_uid, container->container_gid, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = setup_environment (def, container->container_uid, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* Set primary process to 1 explicitly if nothing is configured and LISTEN_FD is not set.  */
  ಯದಿ (entrypoint_args->context->listen_fds > 0 && getenv ("LISTEN_PID") == ರಿಕ್ತ)
    {
      libcrun_warning ("setting LISTEN_PID=1 since no previous configuration was found");
      ಯದಿ (UNLIKELY (setenv ("LISTEN_PID", "1", 1) < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "setenv LISTEN_PID");
    }

  /* Attempt to chdir immediately here, before doing the setresuid.  If we fail here, let's
     try again later once the process switched to the user that runs in the container.  */
  ಯದಿ (def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      ಯದಿ (LIKELY (ret == 0))
        chdir_done = true;
      ಅನ್ಯಥಾ
        crun_error_release (err);
    }

  ret = setsid ();
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "setsid");

  ret = setup_terminal (entrypoint_args, container, console_socket, console_socketpair, has_terminal, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  close_and_reset (&console_socket);
  close_and_reset (&console_socketpair);

  ret = setup_executable_path (entrypoint_args, def, exec_path, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_set_hostname (container, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_set_domainname (container, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (def->process && def->process->user)
    umask (def->process->user->umask_present ? def->process->user->umask : 0022);

  ret = apply_security_settings (entrypoint_args, def, container, *own_pid, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (UNLIKELY (def->process && def->process->args && *exec_path == ರಿಕ್ತ))
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          ಯದಿ (entrypoint_args->custom_handler == ರಿಕ್ತ || is_empty_string (def->process->args[0]))
            ಪ್ರತ್ಯಾಗಮ ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          *exec_path = xstrdup (def->process->args[0]);
        }
    }

  /* The chdir was not already performed, so try again now after switching to the UID/GID in the container.  */
  ಯದಿ (! chdir_done && def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (notify_socket && putenv (notify_socket) < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "putenv `%s`", notify_socket);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
open_hooks_output (libcrun_container_t *container, ಪೂರ್ಣಾಙ್ಕ *out_fd, ಪೂರ್ಣಾಙ್ಕ *err_fd, libcrun_error_t *err)
{
  ಧ್ರುವ ವರ್ಣ *annotation;

  *err_fd = *out_fd = -1;

  libcrun_debug ("Opening hooks output");
  annotation = find_annotation (container, "run.oci.hooks.stdout");
  ಯದಿ (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stdout` annotation");
      *out_fd = TEMP_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      ಯದಿ (UNLIKELY (*out_fd < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", annotation);
    }

  annotation = find_annotation (container, "run.oci.hooks.stderr");
  ಯದಿ (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stderr` annotation");
      *err_fd = TEMP_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      ಯದಿ (UNLIKELY (*err_fd < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", annotation);
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* The environment the process was started with lives on the initial stack, right after the
   argv strings and before the string AT_EXECFN points to.  clearenv(3) only drops the pointers
   to it, so when there is no execv to replace the stack, erase it or the container process can
   still read it, e.g. through /proc/self/environ.  */
ಸ್ಥಿರ ಶೂನ್ಯ
erase_initial_environment (ವರ್ಣ **argv, ಪೂರ್ಣಾಙ್ಕ argc)
{
  ವರ್ಣ *start = argv[argc - 1] + strlen (argv[argc - 1]) + 1;
  ವರ್ಣ *end = (ವರ್ಣ *) getauxval (AT_EXECFN);

  ಯದಿ (end > start)
    memset (start, 0, end - start);
}

ಸ್ಥಿರ ಶೂನ್ಯ
rewrite_argv (ವರ್ಣ **argv, ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ *name, ವರ್ಣ **args, size_t args_len)
{
  cleanup_free ವರ್ಣ *decorated_name = ರಿಕ್ತ;
  size_t available_len = argv[argc - 1] - argv[0] + strlen (argv[argc - 1]) + 1;
  cleanup_free ವರ್ಣ *new_argv = ರಿಕ್ತ;
  size_t i, so_far = 0, needed = 0;

  needed = xasprintf (&decorated_name, "[libcrun:%s]", name) + 1;
  ಪ್ರತಿ (i = 0; i < args_len; i++)
    needed += strlen (args[i]) + 1;

  new_argv = xmalloc0 (needed + 1);
  strcpy (new_argv, decorated_name);
  so_far = strlen (decorated_name) + 1;

  ಯದಿ (available_len >= needed)
    {
      ಪ್ರತಿ (i = 0; i < args_len; i++)
        {
          strcpy (new_argv + so_far, args[i]);
          so_far += strlen (args[i]) + 1;
        }
    }

  ಯದಿ (so_far >= available_len)
    so_far = available_len - 1;

  memset (argv[0], 0, available_len);
  memcpy (argv[0], new_argv, so_far);
}

/* Entrypoint to the container.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
container_init (ಶೂನ್ಯ *args, ವರ್ಣ *notify_socket, ಪೂರ್ಣಾಙ್ಕ sync_socket, libcrun_error_t *err)
{
  ಸಂರಚನಾ container_entrypoint_s *entrypoint_args = args;
  ಪೂರ್ಣಾಙ್ಕ ret;
  runtime_spec_schema_config_schema *def = entrypoint_args->container->container_def;
  cleanup_free ವರ್ಣ *exec_path = ರಿಕ್ತ;
  __attribute__ ((unused)) cleanup_free ವರ್ಣ *notify_socket_cleanup = notify_socket;
  pid_t own_pid = 0;

  entrypoint_args->sync_socket = sync_socket;

  crun_set_output_handler (log_write_to_sync_socket, args);

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (ಪಠ (sync_socket, &own_pid, ಪರಿಮಾಣ (own_pid)));
  ಯದಿ (UNLIKELY (ret != ಪರಿಮಾಣ (own_pid)))
    {
      ಯದಿ (ret >= 0)
        errno = 0;
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "read from sync socket");
    }

  ret = container_init_setup (args, &own_pid, notify_socket, sync_socket, &exec_path, err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      /* If it fails to write the error using the sync socket, then fallback
         to stderr.  */
      ಯದಿ (sync_socket_write_error (sync_socket, err) == 0)
        {
          crun_error_release (err);
          /* A return value of > 0 means to fail with an error without err being set so to avoid
             printing it twice.  */
          ಪ್ರತ್ಯಾಗಮ 1;
        }
      ಪ್ರತ್ಯಾಗಮ ret;
    }

  entrypoint_args->sync_socket = -1;

  ret = unblock_signals (err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* sync 4.  */
  ret = sync_socket_send_sync (sync_socket, false, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  close_and_reset (&sync_socket);

  ಯದಿ (entrypoint_args->context->fifo_exec_wait_fd >= 0)
    {
      ವರ್ಣ buffer[1];
      fd_set read_set;
      cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = entrypoint_args->context->fifo_exec_wait_fd;
      entrypoint_args->context->fifo_exec_wait_fd = -1;

      FD_ZERO (&read_set);
      FD_SET (fd, &read_set);
      ಕುರು
        {
          ret = select (fd + 1, &read_set, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "select");

          ret = TEMP_FAILURE_RETRY (ಪಠ (fd, buffer, ಪರಿಮಾಣ (buffer)));
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "read from the exec fifo");
      } ಯಾವತ್ (ret == 0);

      close_and_reset (&entrypoint_args->context->fifo_exec_wait_fd);
    }

  crun_set_output_handler (log_write_to_stderr, ರಿಕ್ತ);

  ಯದಿ (def->process && def->process->no_new_privileges)
    {
      ವರ್ಣ **seccomp_flags = ರಿಕ್ತ;
      size_t seccomp_flags_len = 0;
      cleanup_free ವರ್ಣ *seccomp_fd_payload = ರಿಕ್ತ;
      size_t seccomp_fd_payload_len = 0;

      ಯದಿ (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      ಯದಿ (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (entrypoint_args->container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags,
                                   seccomp_flags_len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  ಯದಿ (UNLIKELY (def->process == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "block `process` not found");

  ಯದಿ (UNLIKELY (exec_path == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "executable path not specified");

  ಯದಿ (def->hooks && def->hooks->start_container_len)
    {
      libcrun_container_t *container = entrypoint_args->container;
      libcrun_error_t tmp_err = ರಿಕ್ತ;
      ಪೂರ್ಣಾಙ್ಕ in_userns = check_running_in_user_namespace (&tmp_err);
      ಯದಿ (tmp_err)
        crun_error_release (&tmp_err);

      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, ರಿಕ್ತ, "created", "startContainer",
                      (hook **) def->hooks->start_container,
                      def->hooks->start_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      ಯದಿ (UNLIKELY (ret != 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      /* Seek stdout/stderr to the end.  If the hooks were using the same files,
         the container process overwrites what was previously written.  */
      (ಶೂನ್ಯ) lseek (1, 0, SEEK_END);
      (ಶೂನ್ಯ) lseek (2, 0, SEEK_END);
    }

  ಯದಿ (entrypoint_args->custom_handler)
    {
      /* Files marked with O_CLOEXEC are closed at execv time, so make sure they are closed now.
         This is a best effort operation, because the seccomp filter is already in place and it could
         stop some syscalls used by mark_or_close_fds_ge_than.
      */
      ಯದಿ (entrypoint_args->custom_handler->vtable->close_fds)
        {
          ret = entrypoint_args->custom_handler->vtable->close_fds (entrypoint_args->custom_handler->cookie,
                                                                    entrypoint_args->container,
                                                                    entrypoint_args->context->preserve_fds,
                                                                    err);
        }
      ಅನ್ಯಥಾ
        {
          ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
        }
      ಯದಿ (UNLIKELY (ret < 0))
        crun_error_release (err);

      prctl (PR_SET_NAME, entrypoint_args->custom_handler->vtable->name);

      ಯದಿ (entrypoint_args->context->argv)
        {
          erase_initial_environment (entrypoint_args->context->argv, entrypoint_args->context->argc);
          rewrite_argv (entrypoint_args->context->argv, entrypoint_args->context->argc,
                        entrypoint_args->custom_handler->vtable->name, def->process->args,
                        def->process->args_len);

          /* It is a quite destructive operation as we might be referencing data from the old
             argv, so make sure the context is not reused.  */
          entrypoint_args->context->argv = ರಿಕ್ತ;
          entrypoint_args->context = ರಿಕ್ತ;
        }

      ret = libcrun_set_selinux_label (entrypoint_args->container, def->process, true, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = libcrun_set_apparmor_profile (entrypoint_args->container, def->process, true, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = entrypoint_args->custom_handler->vtable->run_func (entrypoint_args->custom_handler->cookie,
                                                               entrypoint_args->container,
                                                               exec_path,
                                                               def->process->args);
      ಯದಿ (ret != 0)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "exec container process failed with handler as `%s`", entrypoint_args->custom_handler->vtable->name);

      ಪ್ರತ್ಯಾಗಮ ret;
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation since the seccomp profile is already in place now and might block
     some of the syscalls needed by mark_or_close_fds_ge_than.  */
  ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
  ಯದಿ (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, def->process->args));

  ಯದಿ (errno == ENOENT)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "exec container process (missing dynamic library?) `%s`", exec_path);

  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "exec container process `%s`", exec_path);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
read_container_config_from_state (libcrun_container_t **container, ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, ಧ್ರುವ ವರ್ಣ *id,
                                  libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *config_file = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *dir = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  *container = ರಿಕ್ತ;

  ret = libcrun_get_state_directory (&dir, state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = append_paths (&config_file, err, dir, "config.json", ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  *container = libcrun_container_load_from_file (config_file, err);
  ಯದಿ (*container == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ -1;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
run_poststop_hooks (arg_unused libcrun_context_t *context, libcrun_container_t *container,
                    runtime_spec_schema_config_schema *def,
                    libcrun_container_status_t *status, ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container_cleanup = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (def == ರಿಕ್ತ)
    {
      ಯದಿ (container == ರಿಕ್ತ)
        {
          ret = read_container_config_from_state (&container_cleanup, state_ಬೇರು, id, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
          container = container_cleanup;
        }

      def = container->container_def;
    }

  ಯದಿ (def->hooks && def->hooks->poststop_len)
    {
      cleanup_close ಪೂರ್ಣಾಙ್ಕ hooks_out_fd = -1;
      cleanup_close ಪೂರ್ಣಾಙ್ಕ hooks_err_fd = -1;

      ಯದಿ (container == ರಿಕ್ತ)
        {
          ret = read_container_config_from_state (&container_cleanup, state_ಬೇರು, id, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
          container = container_cleanup;
        }

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = do_hooks (def, 0, id, true, status->bundle, status->bundle, "stopped", "poststop", (hook **) def->hooks->poststop,
                      def->hooks->poststop_len, hooks_out_fd, hooks_err_fd, false, err);
      ಯದಿ (UNLIKELY (ret != 0))
        {
          ಯದಿ (ret < 0)
            libcrun_error_report_and_release (err);
          ಅನ್ಯಥಾ
            libcrun_error (0, "poststop hook failed with exit code: %d", ret);
        }
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ bool
has_new_pid_namespace (runtime_spec_schema_config_schema *def)
{
  size_t i;

  ಯದಿ (def->linux == ರಿಕ್ತ || def->linux->namespaces == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ false;

  ಪ್ರತಿ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ಯದಿ (strcmp (def->linux->namespaces[i]->type, "pid") == 0 && def->linux->namespaces[i]->path == ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ true;
    }
  ಪ್ರತ್ಯಾಗಮ false;
}

/* Delete the container state.  CONTAINER is the container the caller already has
   available, or NULL.  When it is not NULL, the poststop hooks are run even if the
   status file was never written, e.g. when the container creation failed.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
container_delete_internal (libcrun_context_t *context, libcrun_container_t *container,
                           runtime_spec_schema_config_schema *def, ಧ್ರುವ ವರ್ಣ *id, bool force, bool killall,
                           libcrun_error_t *err)
{
  cleanup_cgroup_status ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status = ರಿಕ್ತ;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_container libcrun_container_t *container_cleanup = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *state_ಬೇರು = context->state_ಬೇರು;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = libcrun_read_container_status (&status, state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ಯದಿ (force && crun_error_get_errno (err) == ENOENT)
        {
          libcrun_error_t tmp_err = ರಿಕ್ತ;

          crun_error_release (err);

          ಯದಿ (container)
            {
              libcrun_container_status_t partial_status = {
                .bundle = (ವರ್ಣ *) context->bundle,
              };

              ret = run_poststop_hooks (context, container, def, &partial_status, state_ಬೇರು, id, err);
              ಯದಿ (UNLIKELY (ret < 0))
                crun_error_write_warning_and_release (context->output_handler_arg, &err);
            }

          libcrun_container_delete_status (state_ಬೇರು, id, &tmp_err);
          crun_error_release (&tmp_err);
          ಪ್ರತ್ಯಾಗಮ 0;
        }
      crun_error_release (err);
      ಪ್ರತ್ಯಾಗಮ libcrun_container_delete_status (state_ಬೇರು, id, err);
    }

  cgroup_status = libcrun_cgroup_make_status (&status);

  ಯದಿ (! force)
    {
      ret = libcrun_is_container_running (&status, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
      ಯದಿ (ret == 1)
        {
          ret = libcrun_status_has_read_exec_fifo (state_ಬೇರು, id, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
          ಯದಿ (ret == 0)
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "the container `%s` is not in `created` or `stopped` state", id);

          /* If the container is in "created" state, then do the equivalent of delete --force.  */
          killall = force = true;
        }
    }

  ಯದಿ (def == ರಿಕ್ತ)
    {
      ಯದಿ (container == ರಿಕ್ತ)
        {
          ret = read_container_config_from_state (&container_cleanup, state_ಬೇರು, id, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;

          container = container_cleanup;
        }

      def = container->container_def;
    }

  ಯದಿ (killall && force)
    {
      /* If the container has a pid namespace, it is enough to kill the first
         process (pid=1 in the namespace).
      */
      ಯದಿ (has_new_pid_namespace (def))
        {
          ret = libcrun_kill_linux (&status, SIGKILL, err);
          ಯದಿ (UNLIKELY (ret < 0))
            {
              ಯದಿ (crun_error_get_errno (err) != ESRCH)
                ಪ್ರತ್ಯಾಗಮ ret;

              crun_error_release (err);
            }
        }
      ಅನ್ಯಥಾ ಯದಿ (status.cgroup_path)
        {
          ret = libcrun_cgroup_killall (cgroup_status, SIGKILL, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }
    }

  ಯದಿ (def->linux && def->linux->intel_rdt)
    {
      ret = libcrun_destroy_intelrdt (id, def, err);
      ಯದಿ (UNLIKELY (ret < 0))
        libcrun_error_report_and_release (err);
    }

  ಯದಿ (status.cgroup_path)
    {
      /* If the cgroup cannot be destroyed, there are still processes in it.
         Keep the container status, it is the only record of the cgroup, so
         that the delete can be retried.  */
      ret = libcrun_cgroup_destroy (cgroup_status, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = run_poststop_hooks (context, container, def, &status, state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ಪ್ರತ್ಯಾಗಮ libcrun_container_delete_status (state_ಬೇರು, id, err);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_delete (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, bool force, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ container_delete_internal (context, ರಿಕ್ತ, ರಿಕ್ತ, id, force, true, err);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_kill (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, ಧ್ರುವ ವರ್ಣ *ಸಙ್ಕೇತ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ sig, ret;
  ಧ್ರುವ ವರ್ಣ *state_ಬೇರು = context->state_ಬೇರು;
  cleanup_container_status libcrun_container_status_t status = {};

  sig = str2sig (ಸಙ್ಕೇತ);
  ಯದಿ (UNLIKELY (sig < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "unknown signal `%s`", ಸಙ್ಕೇತ);

  ret = libcrun_read_container_status (&status, state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ libcrun_kill_linux (&status, sig, err);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_killall (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, ಧ್ರುವ ವರ್ಣ *ಸಙ್ಕೇತ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ sig, ret;
  ಧ್ರುವ ವರ್ಣ *state_ಬೇರು = context->state_ಬೇರು;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_cgroup_status ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status = ರಿಕ್ತ;

  sig = str2sig (ಸಙ್ಕೇತ);
  ಯದಿ (UNLIKELY (sig < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "unknown signal `%s`", ಸಙ್ಕೇತ);

  ret = libcrun_read_container_status (&status, state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  cgroup_status = libcrun_cgroup_make_status (&status);

  ret = libcrun_cgroup_killall (cgroup_status, sig, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_container_status (libcrun_container_t *container, libcrun_context_t *context,
                        pid_t pid, ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status,
                        libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *bundle_allocated = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *bundle = get_bundle_path (context->bundle, &bundle_allocated);
  cleanup_free ವರ್ಣ *owner = get_user_name (geteuid ());
  ವರ್ಣ *external_descriptors = libcrun_get_external_descriptors (container);
  ವರ್ಣ *rootfs = container->container_def->ಬೇರು ? container->container_def->ಬೇರು->path : "";
  ವರ್ಣ created[35];

  libcrun_container_status_t status = {
    .pid = pid,
    .rootfs = rootfs,
    .bundle = (ವರ್ಣ *) bundle,
    .created = created,
    .owner = owner,
    .systemd_cgroup = context->systemd_cgroup,
    .detached = context->detach,
    .no_new_keyring = context->no_new_keyring,
    .external_descriptors = external_descriptors,
    .cgroup_path = ರಿಕ್ತ,
    .scope = ರಿಕ್ತ,
  };

  get_current_timestamp (created, ಪರಿಮಾಣ (created));

  ಯದಿ (cgroup_status)
    {
      ಪೂರ್ಣಾಙ್ಕ ret;

      ret = libcrun_cgroup_get_status (cgroup_status, &status, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (external_descriptors == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid internal state.  No external descriptors found");
  ಪ್ರತ್ಯಾಗಮ libcrun_write_container_status (context->state_ಬೇರು, context->id, &status, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
reap_subprocesses (pid_t *main_process, ಪೂರ್ಣಾಙ್ಕ *main_process_exit, ಪೂರ್ಣಾಙ್ಕ *last_process, libcrun_error_t *err)
{
  *last_process = 0;
  ಯಾವತ್ (1)
    {
      ಪೂರ್ಣಾಙ್ಕ status;
      ಪೂರ್ಣಾಙ್ಕ r = waitpid_ignore_stopped (-1, &status, WNOHANG);
      ಯದಿ (r < 0)
        {
          ಯದಿ (errno == EINTR)
            ಅನುವರ್ತ;
          ಯದಿ (errno == ECHILD)
            {
              *last_process = 1;
              ಪ್ರತ್ಯಾಗಮ 0;
            }
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "waitpid");
        }
      ಯದಿ (r == 0)
        ಭಙ್ಗ;
      ಯದಿ (r != *main_process)
        ಅನುವರ್ತ;

      *main_process_exit = get_process_exit_status (status);

      /* Once reaped, the pid can be reused by an unrelated process.  */
      *main_process = 0;
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
send_sd_notify (ಧ್ರುವ ವರ್ಣ *ready_str, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  ಪೂರ್ಣಾಙ್ಕ ret = sd_notify (0, ready_str);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "sd_notify");

#  ಯದಿ HAVE_SD_NOTIFY_BARRIER
  /* Hard-code a 30 seconds timeout.  Ignore errors.  */
  sd_notify_barrier (0, 30 * 1000000);
#  endif
#ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) ready_str;
  (ಶೂನ್ಯ) err;
#endif
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
handle_notify_socket (ಪೂರ್ಣಾಙ್ಕ notify_socketfd, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  ಪೂರ್ಣಾಙ್ಕ ret;
  ವರ್ಣ buf[256];
  ಧ್ರುವ ವರ್ಣ *ready_str = "READY=1";

  ret = recvfrom (notify_socketfd, buf, ಪರಿಮಾಣ (buf) - 1, 0, ರಿಕ್ತ, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "recvfrom notify socket");

  buf[ret] = '\0';
  ಯದಿ (strstr (buf, ready_str))
    {
      ret = send_sd_notify (ready_str, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ಪ್ರತ್ಯಾಗಮ 1;
    }
  ಪ್ರತ್ಯಾಗಮ 0;
#ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) notify_socketfd;
  (ಶೂನ್ಯ) err;
  ಪ್ರತ್ಯಾಗಮ 1;
#endif
}

ಸಂರಚನಾ wait_for_process_args
{
  pid_t pid;
  libcrun_context_t *context;
  ಪೂರ್ಣಾಙ್ಕ terminal_fd;
  ಪೂರ್ಣಾಙ್ಕ notify_socket;
  ಪೂರ್ಣಾಙ್ಕ *container_ready_fd;
  ಪೂರ್ಣಾಙ್ಕ seccomp_notify_fd;
  ಧ್ರುವ ವರ್ಣ *seccomp_notify_plugins;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_pid_file (ಧ್ರುವ ವರ್ಣ *pid_file, pid_t pid, libcrun_error_t *err)
{
  ವರ್ಣ buf[32];
  ಪೂರ್ಣಾಙ್ಕ buf_len = snprintf (buf, ಪರಿಮಾಣ (buf), "%d", pid);
  ಯದಿ (UNLIKELY (buf_len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (buf)))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");
  ಪ್ರತ್ಯಾಗಮ write_file_at_with_flags (AT_FDCWD, O_CREAT | O_TRUNC, 0700, pid_file, buf, buf_len, err);
}

#define MAX_EVENTS 10

ಸ್ಥಿರ bool
is_fd_pollable (ಪೂರ್ಣಾಙ್ಕ fd)
{
  ಸಂರಚನಾ epoll_event ev = { .events = EPOLLIN };
  cleanup_close ಪೂರ್ಣಾಙ್ಕ epollfd = epoll_create1 (EPOLL_CLOEXEC);

  ಯದಿ (epollfd < 0)
    ಪ್ರತ್ಯಾಗಮ true;

  ಪ್ರತ್ಯಾಗಮ epoll_ctl (epollfd, EPOLL_CTL_ADD, fd, &ev) == 0 || errno != EPERM;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
wait_for_process (ಸಂರಚನಾ wait_for_process_args *args, libcrun_error_t *err)
{
  cleanup_channel_fd_pair ಸಂರಚನಾ channel_fd_pair *from_terminal = ರಿಕ್ತ;
  cleanup_channel_fd_pair ಸಂರಚನಾ channel_fd_pair *to_terminal = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret, container_exit_code = 0, last_process;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ terminal_fd_from = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ terminal_fd_to = -1;
  bool stdin_pollable = false, stdout_pollable = false;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ epollfd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ signalfd = -1;
  sigset_t mask;
  ಪೂರ್ಣಾಙ್ಕ in_fds[MAX_EVENTS];
  ಪೂರ್ಣಾಙ್ಕ in_fds_len = 0;
  ಪೂರ್ಣಾಙ್ಕ out_fds[MAX_EVENTS];
  ಪೂರ್ಣಾಙ್ಕ out_fds_len = 0;
  size_t i;

  cleanup_seccomp_notify_context ಸಂರಚನಾ seccomp_notify_context_s *seccomp_notify_ctx = ರಿಕ್ತ;

  container_exit_code = 0;

  ಯದಿ (args == ರಿಕ್ತ || args->context == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: context is empty");

  ಪ್ರತಿ (i = 0; i < MAX_EVENTS; i++)
    {
      in_fds[i] = -1;
      out_fds[i] = -1;
    }

  ಯದಿ (args->context->pid_file)
    {
      ret = write_pid_file (args->context->pid_file, args->pid, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  /* Also exit if there is nothing more to wait for.  */
  ಯದಿ (args->context->detach && args->notify_socket < 0)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯದಿ (args->container_ready_fd)
    {
      ret = 0;
      TEMP_FAILURE_RETRY (ಲಿಖ (*args->container_ready_fd, &ret, ಪರಿಮಾಣ (ret)));
      close_and_reset (args->container_ready_fd);
    }

  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "sigprocmask");

  signalfd = create_signalfd (&mask, err);
  ಯದಿ (UNLIKELY (signalfd < 0))
    ಪ್ರತ್ಯಾಗಮ signalfd;

  ret = reap_subprocesses (&args->pid, &container_exit_code, &last_process, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (last_process)
    ಪ್ರತ್ಯಾಗಮ container_exit_code;

  ಯದಿ (args->seccomp_notify_fd >= 0)
    {
      cleanup_free ವರ್ಣ *state_ಬೇರು = ರಿಕ್ತ;
      cleanup_free ವರ್ಣ *oci_config_path = ರಿಕ್ತ;

      ಸಂರಚನಾ libcrun_load_seccomp_notify_conf_s conf;
      memset (&conf, 0, ಪರಿಮಾಣ conf);

      ret = libcrun_get_state_directory (&state_ಬೇರು, args->context->state_ಬೇರು, args->context->id, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = append_paths (&oci_config_path, err, state_ಬೇರು, "config.json", ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      conf.runtime_ಬೇರು_path = state_ಬೇರು;
      conf.name = args->context->id;
      conf.bundle_path = args->context->bundle;
      conf.oci_config_path = oci_config_path;

      ret = set_blocking_fd (args->seccomp_notify_fd, false, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = libcrun_load_seccomp_notify_plugins (&seccomp_notify_ctx,
                                                 args->seccomp_notify_plugins,
                                                 &conf, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      in_fds[in_fds_len++] = args->seccomp_notify_fd;
    }

  ಯದಿ (args->terminal_fd >= 0)
    {
      /* The terminal_fd is dup()ed so that it can be registered with
         epoll multiple times using different masks.  */
      terminal_fd_from = dup (args->terminal_fd);
      ಯದಿ (UNLIKELY (terminal_fd_from < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "dup terminal fd");
      terminal_fd_to = dup (args->terminal_fd);
      ಯದಿ (UNLIKELY (terminal_fd_to < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "dup terminal fd");

      ಪೂರ್ಣಾಙ್ಕ i, non_blocking_fds[] = { terminal_fd_from, terminal_fd_to, 0, 1, -1 };
      ಪ್ರತಿ (i = 0; non_blocking_fds[i] >= 0; i++)
        {
          ret = set_blocking_fd (non_blocking_fds[i], false, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }

      /* stdin and stdout can be something epoll does not support, such as
         /dev/null or a regular file.  Such a stdin is not forwarded, and
         such a stdout is written to without waiting for it to be ready.  */
      stdin_pollable = is_fd_pollable (0);
      stdout_pollable = is_fd_pollable (1);

      from_terminal = channel_fd_pair_new (terminal_fd_from, 1, BUFSIZ);
      ಯದಿ (stdin_pollable)
        to_terminal = channel_fd_pair_new (0, terminal_fd_to, BUFSIZ);
    }

  in_fds[in_fds_len++] = signalfd;
  ಯದಿ (args->notify_socket >= 0)
    in_fds[in_fds_len++] = args->notify_socket;
  ಯದಿ (args->terminal_fd >= 0)
    {
      ಯದಿ (stdin_pollable)
        {
          in_fds[in_fds_len++] = 0;
          out_fds[out_fds_len++] = terminal_fd_to;
        }

      in_fds[in_fds_len++] = terminal_fd_from;
      ಯದಿ (stdout_pollable)
        out_fds[out_fds_len++] = 1;
    }

  epollfd = epoll_helper (in_fds, ರಿಕ್ತ, out_fds, ರಿಕ್ತ, err);
  ಯದಿ (UNLIKELY (epollfd < 0))
    ಪ್ರತ್ಯಾಗಮ epollfd;

  ಯಾವತ್ (1)
    {
      ಸಂರಚನಾ epoll_event events[MAX_EVENTS];
      ಸಂರಚನಾ signalfd_siginfo si;
      ಸಂರಚನಾ winsize ws;
      ಪೂರ್ಣಾಙ್ಕ i, nr_events;
      ssize_t res;

      nr_events = TEMP_FAILURE_RETRY (epoll_wait (epollfd, events, MAX_EVENTS, -1));
      ಯದಿ (UNLIKELY (nr_events < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "epoll_wait");

      ಪ್ರತಿ (i = 0; i < nr_events; i++)
        {
          ಯದಿ (events[i].data.fd == 0 || events[i].data.fd == terminal_fd_to)
            {
              ret = channel_fd_pair_process (to_terminal, epollfd, err);
              ಯದಿ (UNLIKELY (ret < 0))
                ಪ್ರತ್ಯಾಗಮ crun_error_wrap (err, "copy to terminal fd");
            }
          ಅನ್ಯಥಾ ಯದಿ (events[i].data.fd == 1 || events[i].data.fd == terminal_fd_from)
            {
              ret = channel_fd_pair_process (from_terminal, epollfd, err);
              ಯದಿ (UNLIKELY (ret < 0))
                ಪ್ರತ್ಯಾಗಮ crun_error_wrap (err, "copy from terminal fd");
            }
          ಅನ್ಯಥಾ ಯದಿ (events[i].data.fd == args->seccomp_notify_fd)
            {
              ret = libcrun_seccomp_notify_plugins (seccomp_notify_ctx,
                                                    args->seccomp_notify_fd, err);
              ಯದಿ (UNLIKELY (ret < 0))
                ಪ್ರತ್ಯಾಗಮ ret;
            }
          ಅನ್ಯಥಾ ಯದಿ (events[i].data.fd == args->notify_socket)
            {
              ret = handle_notify_socket (args->notify_socket, err);
              ಯದಿ (UNLIKELY (ret < 0))
                ಪ್ರತ್ಯಾಗಮ ret;
              ಯದಿ (ret && args->context->detach)
                ಪ್ರತ್ಯಾಗಮ 0;
            }
          ಅನ್ಯಥಾ ಯದಿ (events[i].data.fd == signalfd)
            {
              res = TEMP_FAILURE_RETRY (ಪಠ (signalfd, &si, ಪರಿಮಾಣ (si)));
              ಯದಿ (UNLIKELY (res < 0))
                ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "read from signalfd");
              ಯದಿ (si.ssi_signo == SIGCHLD)
                {
                  ret = reap_subprocesses (&args->pid, &container_exit_code,
                                           &last_process, err);
                  ಯದಿ (UNLIKELY (ret < 0))
                    ಪ್ರತ್ಯಾಗಮ ret;
                  ಯದಿ (last_process)
                    ಪ್ರತ್ಯಾಗಮ container_exit_code;
                }
              ಅನ್ಯಥಾ ಯದಿ (si.ssi_signo == SIGWINCH)
                {
                  /* Ignore the signal if the terminal is not available.  */
                  ಯದಿ (args->terminal_fd > 0)
                    {
                      ret = ioctl (0, TIOCGWINSZ, &ws);
                      ಯದಿ (UNLIKELY (ret < 0))
                        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "ioctl TIOCGWINSZ copy terminal size from stdin");

                      ret = ioctl (args->terminal_fd, TIOCSWINSZ, &ws);
                      ಯದಿ (UNLIKELY (ret < 0))
                        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "ioctl TIOCSWINSZ copy terminal size to pty");
                    }
                }
              ಅನ್ಯಥಾ ಯದಿ (args->pid > 0)
                {
                  /* Send any other signal to the child process, if it was not reaped yet.  */
                  ret = kill (args->pid, si.ssi_signo);
                }
            }
          ಅನ್ಯಥಾ
            {
              ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: unknown fd from epoll_wait");
            }
        }
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ
flush_fd_to_err (libcrun_context_t *context, ಪೂರ್ಣಾಙ್ಕ terminal_fd)
{
  ವರ್ಣ buf[256];
  ಪೂರ್ಣಾಙ್ಕ flags;

  ಯದಿ (terminal_fd < 0 || stderr == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ;

  flags = fcntl (terminal_fd, F_GETFL, 0);
  ಯದಿ (flags == -1)
    ಪ್ರತ್ಯಾಗಮ;
  ಯದಿ (fcntl (terminal_fd, F_SETFL, flags | O_NONBLOCK) < 0)
    ಪ್ರತ್ಯಾಗಮ;

  ಪ್ರತಿ (;;)
    {
      ಪೂರ್ಣಾಙ್ಕ ret = TEMP_FAILURE_RETRY (ಪಠ (terminal_fd, buf, ಪರಿಮಾಣ (buf) - 1));
      ಯದಿ (ret <= 0)
        ಭಙ್ಗ;
      buf[ret] = '\0';
      ಯದಿ (context->output_handler)
        context->output_handler (0, buf, LIBCRUN_VERBOSITY_ERROR, context->output_handler_arg);
    }
  (ಶೂನ್ಯ) fcntl (terminal_fd, F_SETFL, flags);
  fflush (stderr);
  fsync (1);
  fsync (2);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
cleanup_watch (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
               ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status, pid_t init_pid, ಪೂರ್ಣಾಙ್ಕ sync_socket,
               ಪೂರ್ಣಾಙ್ಕ terminal_fd, libcrun_error_t *err)
{
  ಧ್ರುವ ವರ್ಣ *oom_message = ರಿಕ್ತ;
  libcrun_error_t tmp_err = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (init_pid)
    {
      /* Try to detect whether the cgroup has a OOM.  */
      ಯದಿ (cgroup_status)
        {
          ಪೂರ್ಣಾಙ್ಕ has_oom;

          has_oom = libcrun_cgroup_has_oom (cgroup_status, &tmp_err);
          ಯದಿ (has_oom > 0)
            oom_message = "OOM: the memory limit could be too low";
          ಅನ್ಯಥಾ ಯದಿ (has_oom < 0)
            {
              /* If the detection has failed for any reason, e.g. the cgroup was
                 already deleted by the time it was checked, just ignore the
                 failure.  */
              crun_error_release (&tmp_err);
            }
        }
      /* If the OOM wasn't detected, look into the static configuration.  */
      ಯದಿ (oom_message == ರಿಕ್ತ && is_memory_limit_too_ಕೆಳಗಿನ (def))
        oom_message = "the memory limit could be too low";

      kill (init_pid, SIGKILL);
      waitpid_ignore_stopped (init_pid, ರಿಕ್ತ, 0);
    }

  ret = sync_socket_wait_sync (context, sync_socket, true, &tmp_err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      crun_error_release (err);
      *err = tmp_err;
    }

  ಯದಿ (terminal_fd >= 0)
    flush_fd_to_err (context, terminal_fd);

  ಯದಿ (oom_message)
    ಪ್ರತ್ಯಾಗಮ crun_error_wrap (err, "%s", oom_message);

  ಪ್ರತ್ಯಾಗಮ -1;
}

/* Find the uid:gid that is mapped to root inside the container user namespace.  */
ಶೂನ್ಯ
get_ಬೇರು_in_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid, uid_t *uid,
                        gid_t *gid)
{
  *uid = -1;
  *gid = -1;

  /* Not root in the namespace.  */
  ಯದಿ (host_uid)
    ಪ್ರತ್ಯಾಗಮ;

  ಯದಿ (def->linux && def->linux->uid_mappings)
    {
      size_t i;

      ಪ್ರತಿ (i = 0; i < def->linux->uid_mappings_len; i++)
        ಯದಿ (def->linux->uid_mappings[i]->container_id == 0)
          {
            *uid = def->linux->uid_mappings[i]->host_id;
            ಭಙ್ಗ;
          }
    }
  ಯದಿ (def->linux && def->linux->gid_mappings)
    {
      size_t i;

      ಪ್ರತಿ (i = 0; i < def->linux->gid_mappings_len; i++)
        ಯದಿ (def->linux->gid_mappings[i]->container_id == 0)
          {
            *gid = def->linux->gid_mappings[i]->host_id;
            ಭಙ್ಗ;
          }
    }

  /* If the uid and the gid are not changed, do not attempt any chown.  */
  ಯದಿ (*uid == host_uid && *gid == host_gid)
    *uid = *gid = -1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_seccomp_receiver_fd (libcrun_container_t *container, ಪೂರ್ಣಾಙ್ಕ *fd, ಪೂರ್ಣಾಙ್ಕ *self_receiver_fd, ಧ್ರುವ ವರ್ಣ **plugins,
                         libcrun_error_t *err)
{
  ಧ್ರುವ ವರ್ಣ *tmp;
  runtime_spec_schema_config_schema *def = container->container_def;

  *fd = -1;
  *self_receiver_fd = -1;

  tmp = find_annotation (container, "run.oci.seccomp.plugins");
  ಯದಿ (tmp)
    {
      ಪೂರ್ಣಾಙ್ಕ fds[2];
      ಪೂರ್ಣಾಙ್ಕ ret;

      ret = create_socket_pair (fds, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_error_wrap (err, "create socket pair");

      *fd = fds[0];
      *self_receiver_fd = fds[1];
      *plugins = tmp;
      ಪ್ರತ್ಯಾಗಮ 0;
    }

  ಯದಿ (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    tmp = def->linux->seccomp->listener_path;
  ಅನ್ಯಥಾ
    tmp = find_annotation (container, "run.oci.seccomp.receiver");
  ಯದಿ (tmp == ರಿಕ್ತ)
    tmp = getenv ("RUN_OCI_SECCOMP_RECEIVER");
  ಯದಿ (tmp)
    {
      ಯದಿ (tmp[0] != '/')
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "the seccomp receiver `%s` is not an absolute path", tmp);

      *fd = open_unix_domain_client_socket (tmp, 0, err);
      ಯದಿ (UNLIKELY (*fd < 0))
        ಪ್ರತ್ಯಾಗಮ crun_error_wrap (err, "open seccomp receiver");
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ bool
has_seccomp_receiver (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;

  ಯದಿ (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    ಪ್ರತ್ಯಾಗಮ true;

  ಯದಿ (find_annotation (container, "run.oci.seccomp.receiver") != ರಿಕ್ತ || getenv ("RUN_OCI_SECCOMP_RECEIVER") != ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ true;

  ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
setup_container_hooks_output (libcrun_container_t *container, runtime_spec_schema_config_schema *def,
                              ಸಂರಚನಾ container_entrypoint_s *container_args, ಪೂರ್ಣಾಙ್ಕ *hooks_out_fd,
                              ಪೂರ್ಣಾಙ್ಕ *hooks_err_fd, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (def->hooks
      && (def->hooks->prestart_len || def->hooks->poststart_len || def->hooks->create_runtime_len
          || def->hooks->create_container_len || def->hooks->start_container_len))
    {
      ret = open_hooks_output (container, hooks_out_fd, hooks_err_fd, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
      container_args->hooks_out_fd = *hooks_out_fd;
      container_args->hooks_err_fd = *hooks_err_fd;
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
setup_container_keyring (libcrun_container_t *container, libcrun_context_t *context, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (! context->no_new_keyring)
    {
      ಧ್ರುವ ವರ್ಣ *label = ರಿಕ್ತ;

      libcrun_debug ("Creating new keyring");

      ಯದಿ (def->process)
        {
          label = def->process->selinux_label;
          ಯದಿ (label)
            libcrun_debug ("Using SELinux process label: `%s`", label);
        }

      ret = libcrun_create_keyring (container, container->context->id, label, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
setup_terminal_socket_pair (libcrun_container_t *container, libcrun_context_t *context,
                            ಸಂರಚನಾ container_entrypoint_s *container_args, ಪೂರ್ಣಾಙ್ಕ *socket_pair_0,
                            ಪೂರ್ಣಾಙ್ಕ *socket_pair_1, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಪೂರ್ಣಾಙ್ಕ detach = context->detach;

  ಯದಿ (def->process && def->process->terminal && ! detach && context->console_socket == ರಿಕ್ತ)
    {
      libcrun_debug ("Creating terminal socket pair");
      container_args->has_terminal_socket_pair = 1;
      ret = create_socket_pair (container_args->terminal_socketpair, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_error_wrap (err, "create terminal socket");

      *socket_pair_0 = container_args->terminal_socketpair[0];
      *socket_pair_1 = container_args->terminal_socketpair[1];
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
setup_seccomp (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *seccomp_bpf_data,
               ಸಂರಚನಾ libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, ಪೂರ್ಣಾಙ್ಕ *seccomp_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (find_annotation (container, "run.oci.seccomp.plugins") != ರಿಕ್ತ && has_seccomp_receiver (container))
    {
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "seccomp plugins and seccomp receivers cannot be declared at the same time");
    }

  ಯದಿ (def->linux && (def->linux->seccomp || seccomp_bpf_data))
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ seccomp_gen_options = 0;
      ಧ್ರುವ ವರ್ಣ *annotation;

      libcrun_debug ("Initializing seccomp");
      annotation = find_annotation (container, "run.oci.seccomp_fail_unknown_syscall");
      ಯದಿ (annotation && strcmp (annotation, "0") != 0)
        seccomp_gen_options = LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL;

      ಯದಿ (seccomp_bpf_data)
        seccomp_gen_options |= LIBCRUN_SECCOMP_SKIP_CACHE;

      libcrun_seccomp_gen_ctx_init (seccomp_gen_ctx, container, true, seccomp_gen_options);

      ret = libcrun_open_seccomp_bpf (seccomp_gen_ctx, seccomp_fd, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
setup_console_socket (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
                      ಸಂರಚನಾ container_entrypoint_s *container_args, ಪೂರ್ಣಾಙ್ಕ *console_socket_fd,
                      libcrun_error_t *err)
{
  ಯದಿ (context->console_socket && def->process && def->process->terminal)
    {
      *console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
      ಯದಿ (UNLIKELY (*console_socket_fd < 0))
        ಪ್ರತ್ಯಾಗಮ crun_error_wrap (err, "open console socket");
      container_args->console_socket_fd = *console_socket_fd;
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
setup_cgroup_manager (libcrun_context_t *context, libcrun_container_t *container,
                      ಸಂರಚನಾ libcrun_cgroup_args *cg, ಪೂರ್ಣಾಙ್ಕ *cgroup_dirfd,
                      ಸಂರಚನಾ libcrun_dirfd_s *cgroup_dirfd_s, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಪೂರ್ಣಾಙ್ಕ cgroup_manager;
  uid_t ಬೇರು_uid = -1;
  gid_t ಬೇರು_gid = -1;
  ಪೂರ್ಣಾಙ್ಕ ret;

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  ಯದಿ (context->systemd_cgroup)
    {
      libcrun_debug ("Using systemd cgroup manager");
      cgroup_manager = CGROUP_MANAGER_SYSTEMD;
    }
  ಅನ್ಯಥಾ ಯದಿ (context->force_no_cgroup)
    {
      libcrun_debug ("Disabling cgroup manager");
      cgroup_manager = CGROUP_MANAGER_DISABLED;
    }
  ಅನ್ಯಥಾ
    libcrun_debug ("Using cgroupfs cgroup manager");

  /* If we are root (either on the host or in a namespace), then chown the cgroup to root
     in the container user namespace.  */
  get_ಬೇರು_in_the_userns (def, container->host_uid, container->host_gid, &ಬೇರು_uid, &ಬೇರು_gid);
  libcrun_debug ("Using container host UID `%d` and GID `%d`", container->host_uid, container->host_gid);

  memset (cg, 0, ಪರಿಮಾಣ (*cg));

  cg->cgroup_path = def->linux ? def->linux->cgroups_path : "";
  cg->manager = cgroup_manager;
  cg->id = context->id;
  cg->resources = def->linux ? def->linux->resources : ರಿಕ್ತ;
  cg->annotations = container->annotations;
  cg->ಬೇರು_uid = ಬೇರು_uid;
  cg->ಬೇರು_gid = ಬೇರು_gid;
  cg->state_ಬೇರು = context->state_ಬೇರು;
  cg->container = container;

  ret = libcrun_cgroup_preenter (cg, cgroup_dirfd, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  cgroup_dirfd_s->dirfd = cgroup_dirfd;
  cgroup_dirfd_s->joined = false;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
set_scheduler (pid_t pid, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = libcrun_set_scheduler (pid, def->process, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_reset_cpu_affinity_mask (pid, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_set_io_priority (pid, def->process, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
seccomp_generation (ಪೂರ್ಣಾಙ್ಕ seccomp_fd, ಧ್ರುವ ವರ್ಣ *seccomp_bpf_data,
                    ಸಂರಚನಾ libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (seccomp_fd >= 0)
    {
      ಯದಿ (seccomp_bpf_data != ರಿಕ್ತ)
        {
          ret = libcrun_copy_seccomp (seccomp_gen_ctx, seccomp_bpf_data, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }
      ಅನ್ಯಥಾ
        {
          ret = libcrun_generate_seccomp (seccomp_gen_ctx, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
terminal_setup (runtime_spec_schema_config_schema *def, libcrun_context_t *context,
                ಪೂರ್ಣಾಙ್ಕ socket_pair_0, ಪೂರ್ಣಾಙ್ಕ *terminal_fd, ಶೂನ್ಯ **orig_terminal, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಪೂರ್ಣಾಙ್ಕ detach = context->detach;

  ಯದಿ (def->process && def->process->terminal && ! detach && context->console_socket == ರಿಕ್ತ)
    {
      libcrun_debug ("Receiving console socket fd");
      *terminal_fd = receive_fd_from_socket (socket_pair_0, err);
      ಯದಿ (UNLIKELY (*terminal_fd < 0))
        ಪ್ರತ್ಯಾಗಮ -1;

      ret = libcrun_clear_onlcr (*terminal_fd, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = libcrun_set_raw (0, orig_terminal, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libcrun_container_run_internal (libcrun_container_t *container, libcrun_context_t *context,
                                ಪೂರ್ಣಾಙ್ಕ *container_ready_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಪೂರ್ಣಾಙ್ಕ ret;
  pid_t pid;
  ಪೂರ್ಣಾಙ್ಕ detach = context->detach;
  cleanup_cgroup_status ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ terminal_fd = -1;
  cleanup_terminal ಶೂನ್ಯ *orig_terminal = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ sync_socket = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ notify_socket = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ socket_pair_0 = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ socket_pair_1 = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ seccomp_fd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ console_socket_fd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ hooks_out_fd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ hooks_err_fd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ own_seccomp_receiver_fd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ seccomp_notify_fd = -1;
  ಧ್ರುವ ವರ್ಣ *seccomp_notify_plugins = ರಿಕ್ತ;
  cleanup_cgroup_preenter ಸಂರಚನಾ libcrun_cgroup_args cg = {};
  cleanup_free ವರ್ಣ *bundle_allocated = ರಿಕ್ತ;
  ಸಂರಚನಾ container_entrypoint_s container_args = {
    .container = container,
    .context = context,
    .bundle = get_bundle_path (context->bundle, &bundle_allocated),
    .terminal_socketpair = { -1, -1 },
    .console_socket_fd = -1,
    .hooks_out_fd = -1,
    .hooks_err_fd = -1,
    .seccomp_receiver_fd = -1,
    .custom_handler = ರಿಕ್ತ,
  };
  cleanup_close ಪೂರ್ಣಾಙ್ಕ cgroup_dirfd = -1;
  ಸಂರಚನಾ libcrun_dirfd_s cgroup_dirfd_s;
  ಸಂರಚನಾ libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  ಧ್ರುವ ವರ್ಣ *seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");
  ಪೂರ್ಣಾಙ್ಕ cgroup_mode;

  ಯದಿ (! context->force_no_cgroup)
    {
      cgroup_mode = libcrun_get_cgroup_mode (err);
      ಯದಿ (UNLIKELY (cgroup_mode < 0))
        ಪ್ರತ್ಯಾಗಮ cgroup_mode;

      ಯದಿ (cgroup_mode != CGROUP_MODE_UNIFIED)
        libcrun_warning ("cgroup v1 is deprecated and will be removed in a future release.  Use cgroup v2");
    }

  ret = setup_container_hooks_output (container, def, &container_args, &hooks_out_fd, &hooks_err_fd, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  container->context = context;

  ಯದಿ (! detach || context->notify_socket)
    {
      libcrun_debug ("Setting child subreaper");
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "prctl set child subreaper");
    }

  ret = setup_container_keyring (container, context, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = setup_terminal_socket_pair (container, context, &container_args, &socket_pair_0, &socket_pair_1, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = block_signals (err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  umask (0);

  ret = libcrun_set_mempolicy (def, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;
  container_args.seccomp_fd = seccomp_fd;

  ಯದಿ (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &container_args.seccomp_receiver_fd, &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = setup_console_socket (context, def, &container_args, &console_socket_fd, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = setup_cgroup_manager (context, container, &cg, &cgroup_dirfd, &cgroup_dirfd_s, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_configure_handler (container_args.context->handler_manager,
                                   container_args.context,
                                   container,
                                   &(container_args.custom_handler),
                                   err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (container_args.custom_handler && container_args.custom_handler->vtable->modify_oci_configuration)
    {
      libcrun_debug ("Using custom handler to modify OCI configuration");
      ret = container_args.custom_handler->vtable->modify_oci_configuration (container_args.custom_handler->cookie,
                                                                             container_args.context,
                                                                             container->container_def,
                                                                             err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  pid = libcrun_run_linux_container (container, container_init, &container_args, &sync_socket, &cgroup_dirfd_s, err);
  ಯದಿ (UNLIKELY (pid < 0))
    ಪ್ರತ್ಯಾಗಮ pid;

  cg.pid = pid;
  cg.joined = cgroup_dirfd_s.joined;
  libcrun_debug ("Running container on PID: `%d`", pid);

  ಯದಿ (context->fifo_exec_wait_fd < 0 && context->notify_socket)
    {
      libcrun_debug ("Using notify socket: `%s`", context->notify_socket);
      /* Do not open the notify socket here on "create".  "start" will take care of it.  */
      ret = get_notify_fd (context, container, &notify_socket, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಗಚ್ಛ fail;
    }

  ಯದಿ (container_args.terminal_socketpair[1] >= 0)
    close_and_reset (&socket_pair_1);

  ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ fail;

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ fail;

  ret = libcrun_move_network_devices (container, pid, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ fail;

  /* sync send own pid.  */
  ret = TEMP_FAILURE_RETRY (ಲಿಖ (sync_socket, &pid, ಪರಿಮಾಣ (pid)));
  ಯದಿ (UNLIKELY (ret != ಪರಿಮಾಣ (pid)))
    {
      ಯದಿ (ret >= 0)
        errno = 0;
      crun_make_error (err, errno, "write to sync socket");
      ಗಚ್ಛ fail;
    }

  /* sync 1.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ fail;

  /* sync 2.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ fail;

  ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ fail;

  /* If the container process is created later, the scheduler is set on it then.  */
  ಯದಿ (! libcrun_container_delayed_pidns_fork (container))
    {
      ret = set_scheduler (pid, def, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಗಚ್ಛ fail;
    }

  /* The container is waiting that we write back.  In this phase we can launch the
     prestart hooks.  */
  ಯದಿ (def->hooks && def->hooks->prestart_len)
    {
      libcrun_debug ("Running `prestart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "prestart", (hook **) def->hooks->prestart,
                      def->hooks->prestart_len, hooks_out_fd, hooks_err_fd, false, err);
      ಯದಿ (UNLIKELY (ret != 0))
        ಗಚ್ಛ fail;
    }
  ಯದಿ (def->hooks && def->hooks->create_runtime_len)
    {
      libcrun_debug ("Running `create` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "createRuntime", (hook **) def->hooks->create_runtime,
                      def->hooks->create_runtime_len, hooks_out_fd, hooks_err_fd, false, err);
      ಯದಿ (UNLIKELY (ret != 0))
        ಗಚ್ಛ fail;
    }

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ fail;
  close_and_reset (&seccomp_fd);

  /* sync 3.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ fail;

  /* The process joining an existing PID namespace is created once the container root is in place.  */
  ಯದಿ (libcrun_container_delayed_pidns_fork (container))
    {
      pid_t new_pid = 0;

      ret = sync_socket_wait_sync (context, sync_socket, false, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಗಚ್ಛ fail;

      ret = TEMP_FAILURE_RETRY (ಪಠ (sync_socket, &new_pid, ಪರಿಮಾಣ (new_pid)));
      ಯದಿ (UNLIKELY (ret != ಪರಿಮಾಣ (new_pid)))
        {
          ಯದಿ (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "read pid from sync socket");
          ಗಚ್ಛ fail;
        }

      /* Cleanup the first process.  */
      waitpid_ignore_stopped (pid, ರಿಕ್ತ, 0);

      pid = new_pid;

      ret = set_scheduler (pid, def, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಗಚ್ಛ fail;

      ret = libcrun_handle_pidfd_receiver (pid, container, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಗಚ್ಛ fail;

      /* sync send own pid.  */
      ret = TEMP_FAILURE_RETRY (ಲಿಖ (sync_socket, &pid, ಪರಿಮಾಣ (pid)));
      ಯದಿ (UNLIKELY (ret != ಪರಿಮಾಣ (pid)))
        {
          ಯದಿ (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "write to sync socket");
          ಗಚ್ಛ fail;
        }
    }

  ret = terminal_setup (def, context, socket_pair_0, &terminal_fd, &orig_terminal, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ fail;

  close_and_reset (&socket_pair_0);

  /* sync 4.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ fail;

  ret = close_and_reset (&sync_socket);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      crun_make_error (err, errno, "close sync_socket failed");
      ಗಚ್ಛ fail;
    }

  libcrun_debug ("Writing container status");
  ret = write_container_status (container, context, pid, cgroup_status, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ fail;

  /* Run poststart hooks here only if the container is created using "run".  For create+start, the
     hooks will be executed as part of the start command.  */
  ಯದಿ (context->fifo_exec_wait_fd < 0 && def->hooks && def->hooks->poststart_len)
    {
      libcrun_debug ("Running `poststart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      ಯದಿ (UNLIKELY (ret != 0))
        ಗಚ್ಛ fail;
    }

  /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
  ಯದಿ (own_seccomp_receiver_fd >= 0)
    {
      libcrun_debug ("Receiving seccomp fd");
      seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
      ಯದಿ (UNLIKELY (seccomp_notify_fd < 0))
        ಗಚ್ಛ fail;

      ret = close_and_reset (&own_seccomp_receiver_fd);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          crun_make_error (err, errno, "close seccomp receiver fd failed");
          ಗಚ್ಛ fail;
        }
    }

  {
    ಸಂರಚನಾ wait_for_process_args args = {
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
  ಯದಿ (! context->detach)
    {
      libcrun_error_t tmp_err = ರಿಕ್ತ;
      cleanup_watch (context, def, cgroup_status, 0, sync_socket, terminal_fd, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ಪ್ರತ್ಯಾಗಮ ret;

fail:
  ret = cleanup_watch (context, def, cgroup_status, pid, sync_socket, terminal_fd, err);
  ಯದಿ (cgroup_status)
    {
      libcrun_error_t tmp_err = ರಿಕ್ತ;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
check_config_file (runtime_spec_schema_config_schema *def, libcrun_context_t *context, libcrun_error_t *err)
{
  ಯದಿ (UNLIKELY (def->linux == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid config file, no `linux` block specified");

  ಯದಿ (context->handler == ರಿಕ್ತ)
    {
      ಯದಿ (UNLIKELY (def->ಬೇರು == ರಿಕ್ತ))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid config file, no `root` block specified");
      ಯದಿ (UNLIKELY (def->mounts == ರಿಕ್ತ))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid config file, no `mounts` block specified");
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libcrun_copy_config_file (ಧ್ರುವ ವರ್ಣ *id, ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, libcrun_container_t *container, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  cleanup_free ವರ್ಣ *dest_path = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *dir = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *buffer = ರಿಕ್ತ;
  size_t len;

  ret = libcrun_get_state_directory (&dir, state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = append_paths (&dest_path, err, dir, "config.json", ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (container->config_file == ರಿಕ್ತ && container->config_file_content == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "config file not specified");

  ಯದಿ (container->config_file == ರಿಕ್ತ)
    {
      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, container->config_file_content, strlen (container->config_file_content), err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಅನ್ಯಥಾ
    {
      libcrun_debug ("Reading config file: `%s`", container->config_file);
      ret = read_all_file (container->config_file, &buffer, &len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, buffer, len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ
force_delete_container_status (libcrun_context_t *context, libcrun_container_t *container,
                               runtime_spec_schema_config_schema *def)
{
  libcrun_error_t tmp_err = ರಿಕ್ತ;
  container_delete_internal (context, container, def, context->id, true, false, &tmp_err);
  libcrun_error_report_and_release (&tmp_err);
}

/* Serialize ERR to FD as an int errno followed by a NUL-terminated message.
   Used by forked children to report a failure to the parent process.  */
ಸ್ಥಿರ ಶೂನ್ಯ
write_error_to_pipe (ಪೂರ್ಣಾಙ್ಕ fd, libcrun_error_t *err)
{
  ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ = (*err)->ಸಂದೇಶ;
  ಪೂರ್ಣಾಙ್ಕ errcode = crun_error_get_errno (err);

  TEMP_FAILURE_RETRY (ಲಿಖ (fd, &errcode, ಪರಿಮಾಣ (errcode)));
  TEMP_FAILURE_RETRY (ಲಿಖ (fd, ಸಂದೇಶ, strlen (ಸಂದೇಶ) + 1));
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_run (libcrun_context_t *context, libcrun_container_t *container, unsigned ಪೂರ್ಣಾಙ್ಕ options,
                       libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಪೂರ್ಣಾಙ್ಕ detach = context->detach;
  ಪೂರ್ಣಾಙ್ಕ container_ret_status[2];
  cleanup_close ಪೂರ್ಣಾಙ್ಕ pipefd0 = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ pipefd1 = -1;
  libcrun_error_t tmp_err = ರಿಕ್ತ;

  container->context = context;

  ret = validate_options (options, LIBCRUN_RUN_OPTIONS_PREFORK | LIBCRUN_RUN_OPTIONS_KEEP, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = check_config_file (def, context, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (def->process && def->process->terminal && detach && context->console_socket == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "use --console-socket with --detach when a terminal is used");

  ret = libcrun_status_check_directories (context->state_ಬೇರು, context->id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (! detach && (options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      ret = libcrun_copy_config_file (context->id, context->state_ಬೇರು, container, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = libcrun_container_run_internal (container, context, ರಿಕ್ತ, err);
      ಯದಿ (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
        force_delete_container_status (context, container, def);
      ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  ret = fork ();
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fork");
  ಯದಿ (ret)
    {
      ಪೂರ್ಣಾಙ್ಕ status;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, ರಿಕ್ತ, 0);

      ret = TEMP_FAILURE_RETRY (ಪಠ (pipefd0, &status, ಪರಿಮಾಣ (status)));
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "read from sync pipe");

      ಯದಿ (status < 0)
        {
          ಪೂರ್ಣಾಙ್ಕ errno_;
          ವರ್ಣ buf[512];
          ret = TEMP_FAILURE_RETRY (ಪಠ (pipefd0, &errno_, ಪರಿಮಾಣ (errno_)));
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "read from sync pipe");

          ret = TEMP_FAILURE_RETRY (ಪಠ (pipefd0, buf, ಪರಿಮಾಣ (buf) - 1));
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "read from sync pipe");
          buf[ret] = '\0';

          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno_, "%s", buf);
        }

      ಪ್ರತ್ಯಾಗಮ status;
    }

  close_and_reset (&pipefd0);

  /* forked process.  */
  ret = detach_process ();
  ಯದಿ (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_ಬೇರು, container, &tmp_err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ fail;

  ret = libcrun_container_run_internal (container, context, ರಿಕ್ತ, &tmp_err);
  TEMP_FAILURE_RETRY (ಲಿಖ (pipefd1, &ret, ಪರಿಮಾಣ (ret)));
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ fail;

  exit (EXIT_SUCCESS);
fail:

  ಯದಿ (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
    force_delete_container_status (context, container, def);
  ಯದಿ (tmp_err)
    {
      write_error_to_pipe (pipefd1, &tmp_err);
      crun_error_release (&tmp_err);
    }

  exit (EXIT_FAILURE);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container, unsigned ಪೂರ್ಣಾಙ್ಕ options,
                          libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಪೂರ್ಣಾಙ್ಕ container_ready_pipe[2];
  cleanup_close ಪೂರ್ಣಾಙ್ಕ pipefd0 = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ pipefd1 = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ exec_fifo_fd = -1;
  context->detach = 1;

  libcrun_debug ("Creating container: `%s`", context->id);
  container->context = context;

  ret = validate_options (options, LIBCRUN_CREATE_OPTIONS_PREFORK, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = check_config_file (def, context, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (def->process && def->process->terminal && context->console_socket == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "use --console-socket with create when a terminal is used");

  ret = libcrun_status_check_directories (context->state_ಬೇರು, context->id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  exec_fifo_fd = libcrun_status_create_exec_fifo (context->state_ಬೇರು, context->id, err);
  ಯದಿ (UNLIKELY (exec_fifo_fd < 0))
    ಪ್ರತ್ಯಾಗಮ exec_fifo_fd;

  context->fifo_exec_wait_fd = exec_fifo_fd;
  exec_fifo_fd = -1;

  ಯದಿ ((options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      libcrun_debug ("Running with prefork enabled");
      ret = libcrun_copy_config_file (context->id, context->state_ಬೇರು, container, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
      ret = libcrun_container_run_internal (container, context, ರಿಕ್ತ, err);
      ಯದಿ (UNLIKELY (ret < 0))
        force_delete_container_status (context, container, def);
      ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = pipe2 (container_ready_pipe, O_CLOEXEC);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "pipe");
  pipefd0 = container_ready_pipe[0];
  pipefd1 = container_ready_pipe[1];

  ret = fork ();
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fork");
  ಯದಿ (ret)
    {
      ಪೂರ್ಣಾಙ್ಕ exit_code;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, ರಿಕ್ತ, 0);

      ret = TEMP_FAILURE_RETRY (ಪಠ (pipefd0, &exit_code, ಪರಿಮಾಣ (exit_code)));
      ಯದಿ (UNLIKELY (ret <= 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ret < 0 ? errno : 0, "waiting for container to be ready");

      ಯದಿ (ret != ಪರಿಮಾಣ (exit_code) || exit_code < 0)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: read invalid exit code");

      ಯದಿ (exit_code == 0)
        ಪ್ರತ್ಯಾಗಮ 0;

      libcrun_debug ("Exit code is `%d`, deleting container", exit_code);
      libcrun_error_t tmp_err = ರಿಕ್ತ;
      container_delete_internal (context, ರಿಕ್ತ, def, context->id, true, true, &tmp_err);
      libcrun_error_report_and_release (&tmp_err);
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "error creating container");
    }

  /* forked process.  */
  ret = detach_process ();
  ಯದಿ (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_ಬೇರು, container, err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ಪೂರ್ಣಾಙ್ಕ errcode = crun_error_get_errno (err);
      crun_error_release (err);
      libcrun_fail_with_error (errcode, "copy config file");
    }

  ret = libcrun_container_run_internal (container, context, &pipefd1, err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      force_delete_container_status (context, container, def);
      libcrun_error ((*err)->status, "%s", (*err)->ಸಂದೇಶ);
      crun_error_release (err);
      crun_set_output_handler (log_write_to_stderr, ರಿಕ್ತ);
    }

  ಯದಿ (pipefd1 >= 0)
    TEMP_FAILURE_RETRY (ಲಿಖ (pipefd1, &ret, ಪರಿಮಾಣ (ret)));
  exit (ret ? EXIT_FAILURE : 0);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_start (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *state_ಬೇರು = context->state_ಬೇರು;
  runtime_spec_schema_config_schema *def;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = libcrun_read_container_status (&status, state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_is_container_running (&status, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (! ret)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "container `%s` is not running", id);

  ret = read_container_config_from_state (&container, state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (context->notify_socket)
    {
      ret = get_notify_fd (context, container, &fd, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = libcrun_status_write_exec_fifo (context->state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  def = container->container_def;

  ಯದಿ (context->notify_socket)
    {
      ಯದಿ (fd >= 0)
        {
          fd_set read_set;

          ಯಾವತ್ (1)
            {
              ಸಂರಚನಾ timeval timeout = {
                .tv_sec = 0,
                .tv_usec = 10000,
              };
              FD_ZERO (&read_set);
              FD_SET (fd, &read_set);

              ret = select (fd + 1, &read_set, ರಿಕ್ತ, ರಿಕ್ತ, &timeout);
              ಯದಿ (UNLIKELY (ret < 0))
                ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "select failed");
              ಯದಿ (ret)
                {
                  ret = handle_notify_socket (fd, err);
                  ಯದಿ (UNLIKELY (ret < 0))
                    ಪ್ರತ್ಯಾಗಮ ret;
                  ಯದಿ (ret)
                    ಭಙ್ಗ;
                }
              ಅನ್ಯಥಾ
                {
                  ret = libcrun_is_container_running (&status, err);
                  ಯದಿ (UNLIKELY (ret < 0))
                    ಪ್ರತ್ಯಾಗಮ ret;
                  ಯದಿ (! ret)
                    ಪ್ರತ್ಯಾಗಮ 0;
                }
            }
        }
    }

  /* The container is considered running only after we got the notification from the
     notify_socket, if any.  */
  ಯದಿ (def->hooks && def->hooks->poststart_len)
    {
      cleanup_close ಪೂರ್ಣಾಙ್ಕ hooks_out_fd = -1;
      cleanup_close ಪೂರ್ಣಾಙ್ಕ hooks_err_fd = -1;

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = do_hooks (def, status.pid, context->id, false, status.bundle, status.bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      ಯದಿ (UNLIKELY (ret != 0))
        {
          libcrun_error_t tmp_err = ರಿಕ್ತ;
          container_delete_internal (context, container, def, id, true, true, &tmp_err);
          libcrun_error_report_and_release (&tmp_err);
          ಪ್ರತ್ಯಾಗಮ ret;
        }
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_get_container_state_string (ಧ್ರುವ ವರ್ಣ *id, libcrun_container_status_t *status, ಧ್ರುವ ವರ್ಣ *state_ಬೇರು,
                                    ಧ್ರುವ ವರ್ಣ **container_status, ಪೂರ್ಣಾಙ್ಕ *running, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret, has_fifo = 0;
  bool paused = false;

  ret = libcrun_is_container_running (status, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;
  *running = ret;

  ಯದಿ (*running)
    {
      ret = libcrun_status_has_read_exec_fifo (state_ಬೇರು, id, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
      has_fifo = ret;
    }

  ಯದಿ (*running && ! has_fifo)
    {
      cleanup_cgroup_status ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status = ರಿಕ್ತ;

      cgroup_status = libcrun_cgroup_make_status (status);

      ret = libcrun_cgroup_is_container_paused (cgroup_status, &paused, err);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          /*
            The cgroup might have been cleaned up by the time we try to read it, ignore both
            ENOENT and ENODEV:
            - ENOENT: if the open(CGROUP_PATH) fails because the cgroup was deleted.
            - ENODEV: if the cgroup is deleted between the open and reading the freeze status.
          */

          errno = crun_error_get_errno (err);
          ಯದಿ (errno == ENOENT || errno == ENODEV)
            {
              crun_error_release (err);
              *container_status = "stopped";
              ಪ್ರತ್ಯಾಗಮ 0;
            }

          ಪ್ರತ್ಯಾಗಮ ret;
        }
    }

  ಯದಿ (! *running)
    *container_status = "stopped";
  ಅನ್ಯಥಾ ಯದಿ (has_fifo)
    *container_status = "created";
  ಅನ್ಯಥಾ ಯದಿ (paused)
    *container_status = "paused";
  ಅನ್ಯಥಾ
    *container_status = "running";

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_state_json (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, ವರ್ಣ **out, libcrun_error_t *err)
{
  ವರ್ಣ *buffer = ರಿಕ್ತ;
  size_t len = 0;
  FILE *f;
  ಪೂರ್ಣಾಙ್ಕ ret;

  f = open_memstream (&buffer, &len);
  ಯದಿ (f == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open_memstream");

  ret = libcrun_container_state (context, id, f, err);
  fclose (f);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ಮುಕ್ತ (buffer);
      ಪ್ರತ್ಯಾಗಮ ret;
    }

  *out = buffer;
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_state (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, FILE *out, libcrun_error_t *err)
{
  ಧ್ರುವ ವರ್ಣ *ಧ್ರುವ OCI_CONFIG_VERSION = "1.0.0";
  libcrun_container_status_t status = {};
  ಧ್ರುವ ವರ್ಣ *state_ಬೇರು = context->state_ಬೇರು;
  ಧ್ರುವ ವರ್ಣ *container_status = ರಿಕ್ತ;
  json_gen_ctx *gen = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *buf;
  ಪೂರ್ಣಾಙ್ಕ ret = 0;
  ಪೂರ್ಣಾಙ್ಕ r = json_gen_status_ok;
  ಪೂರ್ಣಾಙ್ಕ running;
  size_t len;

  ret = libcrun_read_container_status (&status, state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_get_container_state_string (id, &status, state_ಬೇರು, &container_status, &running, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ exit;

  ret = 0;
  ಯದಿ (! json_gen_init (&gen, ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));
  GEN_ಕೀಲಿ (gen, "ociVersion");
  GEN_STR (gen, OCI_CONFIG_VERSION);

  GEN_ಕೀಲಿ (gen, "id");
  GEN_STR (gen, id);

  GEN_ಕೀಲಿ (gen, "pid");
  GEN_OR_FAIL (map_int (gen, running ? status.pid : 0));

  GEN_ಕೀಲಿ (gen, "status");
  GEN_STR (gen, container_status);

  GEN_ಕೀಲಿ (gen, "bundle");
  GEN_STR (gen, status.bundle);

  GEN_ಕೀಲಿ (gen, "rootfs");
  GEN_STR (gen, status.rootfs);

  GEN_ಕೀಲಿ (gen, "created");
  GEN_STR (gen, status.created);

  ಯದಿ (status.scope)
    {
      GEN_ಕೀಲಿ (gen, "systemd-scope");
      GEN_STR (gen, status.scope);
    }

  ಯದಿ (status.owner)
    {
      GEN_ಕೀಲಿ (gen, "owner");
      GEN_STR (gen, status.owner);
    }

  {
    cleanup_container libcrun_container_t *container = ರಿಕ್ತ;

    ret = read_container_config_from_state (&container, state_ಬೇರು, id, err);
    ಯದಿ (UNLIKELY (ret < 0))
      ಗಚ್ಛ exit;

    ಯದಿ (container->container_def->annotations && container->container_def->annotations->len)
      GEN_OR_FAIL (gen_annotations (gen, container->container_def->annotations));
  }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  fprintf (out, "%s\n", buf);

exit:
  ಯದಿ (gen)
    json_gen_free (gen);
  libcrun_free_container_status (&status);
  ಪ್ರತ್ಯಾಗಮ ret;

gen_error:
  ret = json_gen_error_to_crun_error (r, err);
  ಗಚ್ಛ exit;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_exec (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, runtime_spec_schema_config_schema_process *process,
                        libcrun_error_t *err)
{
  ಸಂರಚನಾ libcrun_container_exec_options_s opts;
  memset (&opts, 0, ಪರಿಮಾಣ (opts));

  opts.struct_size = ಪರಿಮಾಣ (opts);
  opts.process = process;

  ಪ್ರತ್ಯಾಗಮ libcrun_container_exec_with_options (context, id, &opts, err);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_exec_process_file (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err)
{
  ಸಂರಚನಾ libcrun_container_exec_options_s opts;
  memset (&opts, 0, ಪರಿಮಾಣ (opts));

  opts.struct_size = ಪರಿಮಾಣ (opts);
  opts.path = path;

  ಪ್ರತ್ಯಾಗಮ libcrun_container_exec_with_options (context, id, &opts, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
exec_from_process_json (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, ಧ್ರುವ ವರ್ಣ *process_json, ಧ್ರುವ ವರ್ಣ *cgroup,
                        bool merge_env, libcrun_error_t *err)
{
  ಸಂರಚನಾ parser_context parser_ctx = { .options = 0, .errfile = stderr };
  ಸಂರಚನಾ libcrun_container_exec_options_s opts;
  runtime_spec_schema_config_schema_process *process;
  parser_error parser_err = ರಿಕ್ತ;
  json_object *doc = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = parse_json_file (&doc, process_json, &parser_ctx, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  process = make_runtime_spec_schema_config_schema_process (doc, &parser_ctx, &parser_err);
  json_object_put (doc);
  ಯದಿ (process == ರಿಕ್ತ)
    {
      ret = crun_make_error (err, 0, "cannot parse process: `%s`", parser_err ? parser_err : "unknown error");
      ಮುಕ್ತ (parser_err);
      ಪ್ರತ್ಯಾಗಮ ret;
    }

  memset (&opts, 0, ಪರಿಮಾಣ (opts));
  opts.struct_size = ಪರಿಮಾಣ (opts);
  opts.process = process;
  opts.cgroup = cgroup;
  opts.merge_env = merge_env;

  ret = libcrun_container_exec_with_options (context, id, &opts, err);
  free_runtime_spec_schema_config_schema_process (process);
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_exec_json (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, ಧ್ರುವ ವರ್ಣ *process_json, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ exec_from_process_json (context, id, process_json, ರಿಕ್ತ, false, err);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_exec_with_options_json (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id,
                                          ಧ್ರುವ ಸಂರಚನಾ libcrun_exec_options_s *opts, libcrun_error_t *err)
{
  ಯದಿ (opts == ರಿಕ್ತ || opts->struct_size == 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EINVAL, "invalid exec options");

  ಪ್ರತ್ಯಾಗಮ exec_from_process_json (context, id, opts->process_json, opts->cgroup, opts->merge_env, err);
}

#define cleanup_process_schema __attribute__ ((cleanup (cleanup_process_schemap)))

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
cleanup_process_schemap (runtime_spec_schema_config_schema_process **p)
{
  runtime_spec_schema_config_schema_process *process = *p;
  ಯದಿ (process)
    (ಶೂನ್ಯ) free_runtime_spec_schema_config_schema_process (process);
}

/* Build the seccomp receiver payload (when needed), apply the seccomp profile and
   close the seccomp file descriptors.  Used from exec_process_entrypoint both before
   and after the capabilities are set, depending on process->no_new_privileges.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
apply_seccomp_for_exec (libcrun_container_t *container, pid_t own_pid, ಪೂರ್ಣಾಙ್ಕ seccomp_fd,
                        ಪೂರ್ಣಾಙ್ಕ seccomp_receiver_fd, ವರ್ಣ **seccomp_flags, size_t seccomp_flags_len,
                        libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *seccomp_fd_payload = ರಿಕ್ತ;
  size_t seccomp_fd_payload_len = 0;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (seccomp_receiver_fd >= 0)
    {
      ret = get_seccomp_receiver_fd_payload (container, "running", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = libcrun_apply_seccomp (seccomp_fd, seccomp_receiver_fd, seccomp_fd_payload,
                               seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  close_and_reset (&seccomp_fd);
  close_and_reset (&seccomp_receiver_fd);
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
exec_process_entrypoint (libcrun_context_t *context,
                         libcrun_container_t *container,
                         runtime_spec_schema_config_schema_process *process,
                         ಪೂರ್ಣಾಙ್ಕ *pipefd1,
                         ಪೂರ್ಣಾಙ್ಕ seccomp_fd,
                         ಪೂರ್ಣಾಙ್ಕ seccomp_receiver_fd,
                         ಸಂರಚನಾ custom_handler_instance_s *custom_handler,
                         bool merge_env,
                         libcrun_error_t *err)
{
  runtime_spec_schema_config_schema_process_capabilities *capabilities = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *exec_path = ರಿಕ್ತ;
  uid_t container_uid;
  gid_t container_gid;
  ಧ್ರುವ ವರ್ಣ *cwd;
  bool chdir_done = false;
  size_t seccomp_flags_len = 0;
  ವರ್ಣ **seccomp_flags = ರಿಕ್ತ;
  pid_t own_pid = 0;
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  container_uid = process->user ? process->user->uid : 0;
  container_gid = process->user ? process->user->gid : 0;

  TEMP_FAILURE_RETRY (ಪಠ (*pipefd1, &own_pid, ಪರಿಮಾಣ (own_pid)));

  cwd = process->cwd ? process->cwd : "/";
  ಯದಿ (LIKELY (libcrun_safe_chdir (cwd, err) == 0))
    chdir_done = true;
  ಅನ್ಯಥಾ
    crun_error_release (err);

  ret = unblock_signals (err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = clearenv ();
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "clearenv");

  ಯದಿ ((merge_env || ! process->env_len) && container->container_def->process && container->container_def->process->env_len)
    {
      ವರ್ಣ *e;

      ಪ್ರತಿ (i = 0; i < container->container_def->process->env_len; i++)
        {
          e = container->container_def->process->env[i];
          ಯದಿ (putenv (e) < 0)
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "putenv `%s`", e);
        }
    }
  ಯದಿ (process->env_len)
    {
      ಪ್ರತಿ (i = 0; i < process->env_len; i++)
        ಯದಿ (putenv (process->env[i]) < 0)
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "putenv `%s`", process->env[i]);
    }

  ಯದಿ (is_empty_string (getenv ("HOME")))
    {
      ret = set_home_env (container_uid);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          ಯದಿ (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "setenv HOME");
        }
    }

  ret = libcrun_set_selinux_label (container, process, false, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_set_apparmor_profile (container, process, false, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (container->container_def->linux && container->container_def->linux->seccomp)
    {
      seccomp_flags = container->container_def->linux->seccomp->flags;
      seccomp_flags_len = container->container_def->linux->seccomp->flags_len;
    }

  ret = find_executable (&exec_path, process->args[0], process->cwd, err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ಯದಿ (crun_error_get_errno (err) == ENOENT)
        ಪ್ರತ್ಯಾಗಮ ret;

      /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
         once the process switched to the use that runs in the container.  This might be necessary
         when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
         is not honored.  */
      crun_error_release (err);
    }

  ಯದಿ (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, false, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_reopen_dev_null (err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (! process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = libcrun_container_setgroups (container, process, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = maybe_chown_std_streams (container_uid, container_gid, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (process->capabilities)
    capabilities = process->capabilities;
  ಅನ್ಯಥಾ ಯದಿ (container->container_def->process)
    capabilities = container->container_def->process->capabilities;

  ret = libcrun_set_caps (capabilities, container_uid, container_gid, process->no_new_privileges,
                          custom_handler != ರಿಕ್ತ, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (UNLIKELY (exec_path == ರಿಕ್ತ))
    {
      ret = find_executable (&exec_path, process->args[0], process->cwd, err);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          ಯದಿ (custom_handler == ರಿಕ್ತ || is_empty_string (process->args[0]))
            ಪ್ರತ್ಯಾಗಮ ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          exec_path = xstrdup (process->args[0]);
        }
    }

  ಯದಿ (! chdir_done)
    {
      ret = libcrun_safe_chdir (cwd, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (process->user)
    umask (process->user->umask_present ? process->user->umask : 0022);

  ret = 0;
  TEMP_FAILURE_RETRY (ಲಿಖ (*pipefd1, &ret, ಪರಿಮಾಣ (ret)));
  TEMP_FAILURE_RETRY (close (*pipefd1));
  *pipefd1 = -1;

  ಯದಿ (custom_handler)
    {
      ಯದಿ (custom_handler->vtable->exec_func == ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "the handler does not support exec");

      ಯದಿ (context->argv)
        erase_initial_environment (context->argv, context->argc);

      ret = custom_handler->vtable->exec_func (custom_handler->cookie,
                                               container,
                                               exec_path,
                                               process->args);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "exec container process failed with handler as `%s`", custom_handler->vtable->name);

      _safe_exit (EXIT_FAILURE);
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation, because the seccomp filter is already in place and it could
     stop some syscalls used by mark_or_close_fds_ge_than.
  */
  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, true, err);
  ಯದಿ (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, process->args));
  libcrun_fail_with_error (errno, "exec `%s`", exec_path);
}

/* Close the file descriptors inherited from the caller, other than those
   to be passed to the container process.  */
ಪೂರ್ಣಾಙ್ಕ
libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ mark_or_close_fds_ge_than (ರಿಕ್ತ, context->preserve_fds + 3, true, err);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_load_process_from_file (ಧ್ರುವ ವರ್ಣ *path, runtime_spec_schema_config_schema_process **process,
                                libcrun_error_t *err)
{
  ಸಂರಚನಾ parser_context ctx = { 0, stderr };
  cleanup_free ವರ್ಣ *content = ರಿಕ್ತ;
  parser_error parser_err = ರಿಕ್ತ;
  json_object *doc = ರಿಕ್ತ;
  size_t len;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = read_all_file (path, &content, &len, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  *process = make_runtime_spec_schema_config_schema_process (doc, &ctx, &parser_err);
  ಯದಿ (UNLIKELY (*process == ರಿಕ್ತ))
    {
      ret = crun_make_error (err, 0, "cannot parse process file: `%s`", parser_err);
      ಮುಕ್ತ (parser_err);
      ಯದಿ (doc)
        json_object_put (doc);
      ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಮುಕ್ತ (parser_err);
  ಯದಿ (doc)
    json_object_put (doc);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_exec_with_options (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id,
                                     ಸಂರಚನಾ libcrun_container_exec_options_s *opts,
                                     libcrun_error_t *err)
{
  cleanup_custom_handler_instance ಸಂರಚನಾ custom_handler_instance_s *custom_handler = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ container_status, ret;
  bool container_paused = false;
  pid_t pid;
  libcrun_container_status_t status = {};
  ಧ್ರುವ ವರ್ಣ *state_ಬೇರು = context->state_ಬೇರು;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ terminal_fd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ seccomp_fd = -1;
  cleanup_terminal ಶೂನ್ಯ *orig_terminal = ರಿಕ್ತ;
  cleanup_container libcrun_container_t *container = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ container_ret_status[2];
  cleanup_close ಪೂರ್ಣಾಙ್ಕ pipefd0 = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ pipefd1 = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ seccomp_receiver_fd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ own_seccomp_receiver_fd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ seccomp_notify_fd = -1;
  ಧ್ರುವ ವರ್ಣ *seccomp_notify_plugins = ರಿಕ್ತ;
  __attribute__ ((unused)) cleanup_process_schema runtime_spec_schema_config_schema_process *process_cleanup = ರಿಕ್ತ;
  runtime_spec_schema_config_schema_process *process = opts->process;
  ಸಂರಚನಾ libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  ಪೂರ್ಣಾಙ್ಕ ret_from_child = 0;

  ret = libcrun_read_container_status (&status, state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_is_container_running (&status, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;
  container_status = ret;

  ret = read_container_config_from_state (&container, state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  container->context = context;

  ಯದಿ (container_status == 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "the container `%s` is not running", id);

  {
    cleanup_cgroup_status ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status = ರಿಕ್ತ;

    cgroup_status = libcrun_cgroup_make_status (&status);

    ret = libcrun_cgroup_is_container_paused (cgroup_status, &container_paused, err);
    ಯದಿ (UNLIKELY (ret < 0))
      ಪ್ರತ್ಯಾಗಮ ret;
  }

  ಯದಿ (UNLIKELY (container_paused))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot exec in a paused container `%s`", id);

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = block_signals (err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  libcrun_seccomp_gen_ctx_init (&seccomp_gen_ctx, container, false, 0);

  ret = libcrun_open_seccomp_bpf (&seccomp_gen_ctx, &seccomp_fd, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &seccomp_receiver_fd,
                                     &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (ಪರಿಮಾಣ (*opts) != opts->struct_size)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EINVAL, "invalid libcrun_container_exec_options_s struct");

  ಯದಿ (opts->path)
    {
      ಯದಿ (process)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EINVAL, "cannot specify both exec file and options");

      ret = libcrun_load_process_from_file (opts->path, &process, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      process_cleanup = process;
    }

  /* This must be done before we enter a user namespace.  If the process does
     not specify any rlimits, use the ones from the container configuration.  */
  ಯದಿ (process->rlimits_len == 0 && container->container_def->process)
    ret = libcrun_set_rlimits (container->container_def->process->rlimits,
                               container->container_def->process->rlimits_len, err);
  ಅನ್ಯಥಾ
    ret = libcrun_set_rlimits (process->rlimits, process->rlimits_len, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_set_mempolicy (container->container_def, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  /* If the new process block doesn't specify a SELinux label, AppArmor profile, user,
     I/O priority or scheduler, then use the configuration from the original config file.  */
  ಯದಿ (container->container_def->process)
    {
      ಯದಿ (process->io_priority == ರಿಕ್ತ && container->container_def->process->io_priority)
        {
          process->io_priority = clone_runtime_spec_schema_config_schema_process_io_priority (container->container_def->process->io_priority);
          ಯದಿ (process->io_priority == ರಿಕ್ತ)
            OOM ();
        }

      ಯದಿ (process->scheduler == ರಿಕ್ತ && container->container_def->process->scheduler)
        {
          process->scheduler = clone_runtime_spec_schema_config_schema_process_scheduler (container->container_def->process->scheduler);
          ಯದಿ (process->scheduler == ರಿಕ್ತ)
            OOM ();
        }

      ಯದಿ (process->selinux_label == ರಿಕ್ತ && container->container_def->process->selinux_label)
        process->selinux_label = xstrdup (container->container_def->process->selinux_label);

      ಯದಿ (process->apparmor_profile == ರಿಕ್ತ && container->container_def->process->apparmor_profile)
        process->apparmor_profile = xstrdup (container->container_def->process->apparmor_profile);

      ಯದಿ (process->user == ರಿಕ್ತ && container->container_def->process->user)
        {
          process->user = clone_runtime_spec_schema_config_schema_process_user (container->container_def->process->user);
          ಯದಿ (process->user == ರಿಕ್ತ)
            OOM ();
        }
    }

  ret = initialize_security (container, process, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* Do not leak the session keyring of the caller into the container.  The
     keyring created for the container cannot be joined, so use an anonymous one.  */
  ಯದಿ (! context->no_new_keyring && ! status.no_new_keyring)
    {
      libcrun_debug ("Creating new keyring for the exec process");

      ret = libcrun_create_keyring (container, ರಿಕ್ತ, process->selinux_label, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "prctl unset dumpable");

  pid = libcrun_join_process (context, container, status.pid, &status, opts->cgroup, context->detach,
                              process, process->terminal ? &terminal_fd : ರಿಕ್ತ, err);
  ಯದಿ (UNLIKELY (pid < 0))
    ಪ್ರತ್ಯಾಗಮ pid;

  /* Process to exec.  */
  ಯದಿ (pid == 0)
    {
      TEMP_FAILURE_RETRY (close (pipefd0));
      pipefd0 = -1;

      exec_process_entrypoint (context, container, process, &pipefd1, seccomp_fd, seccomp_receiver_fd, custom_handler, opts->merge_env, err);
      /* It gets here only on errors.  */
      ಯದಿ (*err)
        {
          ಯದಿ (pipefd1 < 0)
            libcrun_fail_with_error ((*err)->status, "%s", (*err)->ಸಂದೇಶ);
          ಅನ್ಯಥಾ
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

  TEMP_FAILURE_RETRY (ಲಿಖ (pipefd0, &pid, ಪರಿಮಾಣ (pid)));

  ಯದಿ (seccomp_fd >= 0)
    close_and_reset (&seccomp_fd);

  ಯದಿ (terminal_fd >= 0)
    {
      unsigned short rows = 0, cols = 0;

      ಯದಿ (process->console_size)
        {
          cols = process->console_size->width;
          rows = process->console_size->height;
        }

      ret = libcrun_terminal_setup_size (terminal_fd, rows, cols, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ಯದಿ (context->console_socket)
        {
          ಪೂರ್ಣಾಙ್ಕ ret;
          cleanup_close ಪೂರ್ಣಾಙ್ಕ console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
          ಯದಿ (UNLIKELY (console_socket_fd < 0))
            ಪ್ರತ್ಯಾಗಮ console_socket_fd;
          ret = send_fd_to_socket (console_socket_fd, terminal_fd, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
          close_and_reset (&terminal_fd);
        }
      ಅನ್ಯಥಾ
        {
          ret = libcrun_clear_onlcr (terminal_fd, err);
          ಯದಿ (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              ಪ್ರತ್ಯಾಗಮ ret;
            }

          ret = libcrun_set_raw (0, &orig_terminal, err);
          ಯದಿ (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              ಪ್ರತ್ಯಾಗಮ ret;
            }
        }
    }

  ret = TEMP_FAILURE_RETRY (ಪಠ (pipefd0, &ret_from_child, ಪರಿಮಾಣ (ret_from_child)));
  ಯದಿ (ret != ಪರಿಮಾಣ (ret_from_child))
    ret = crun_make_error (err, 0, "read pipe failed");
  ಅನ್ಯಥಾ ಯದಿ (ret_from_child != 0)
    {
      cleanup_free ವರ್ಣ *ಸಂದೇಶ = ರಿಕ್ತ;
      size_t len = 0;

      ret = read_all_fd (pipefd0, "error stream", &ಸಂದೇಶ, &len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
      /* the string from read_all_fd is always NUL terminated.  */
      ret = crun_make_error (err, ret_from_child, "%s", ಸಂದೇಶ);
    }
  ಅನ್ಯಥಾ
    {
      /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
      ಯದಿ (own_seccomp_receiver_fd >= 0)
        {
          seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
          ಯದಿ (UNLIKELY (seccomp_notify_fd < 0))
            ಪ್ರತ್ಯಾಗಮ seccomp_notify_fd;

          ret = close_and_reset (&own_seccomp_receiver_fd);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "close seccomp receiver fd failed");
        }

      {
        ಸಂರಚನಾ wait_for_process_args args = {
          .pid = pid,
          .context = context,
          .terminal_fd = terminal_fd,
          .notify_socket = -1,
          .container_ready_fd = ರಿಕ್ತ,
          .seccomp_notify_fd = seccomp_notify_fd,
          .seccomp_notify_plugins = seccomp_notify_plugins,
        };

        ret = wait_for_process (&args, err);
      }
    }

  flush_fd_to_err (context, terminal_fd);
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_update (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, ಧ್ರುವ ವರ್ಣ *content, size_t len arg_unused,
                          libcrun_error_t *err)
{
  cleanup_custom_handler_instance ಸಂರಚನಾ custom_handler_instance_s *custom_handler = ರಿಕ್ತ;
  runtime_spec_schema_config_linux_resources *resources = ರಿಕ್ತ;
  cleanup_container libcrun_container_t *container = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *state_ಬೇರು = context->state_ಬೇರು;
  ಸಂರಚನಾ parser_context ctx = { 0, stderr };
  libcrun_container_status_t status = {};
  parser_error parser_err = ರಿಕ್ತ;
  json_object *doc = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = libcrun_read_container_status (&status, state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = read_container_config_from_state (&container, state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  resources = make_runtime_spec_schema_config_linux_resources (doc, &ctx, &parser_err);
  ಯದಿ (UNLIKELY (resources == ರಿಕ್ತ))
    {
      ret = crun_make_error (err, 0, "cannot parse resources: %s", parser_err);
      ಗಚ್ಛ cleanup;
    }

  ಯದಿ (custom_handler && custom_handler->vtable->modify_oci_configuration)
    {
      /* Adapt RESOURCES to be used from the modify_oci_configuration hook.  */
      cleanup_free runtime_spec_schema_config_linux *linux = xmalloc0 (ಪರಿಮಾಣ (*linux));
      cleanup_free runtime_spec_schema_config_schema *def = xmalloc0 (ಪರಿಮಾಣ (*def));

      def->linux = linux;
      linux->resources = resources;

      ret = custom_handler->vtable->modify_oci_configuration (custom_handler->cookie,
                                                              context,
                                                              def,
                                                              err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಗಚ್ಛ cleanup;
    }

  ret = libcrun_linux_container_update (&status, state_ಬೇರು, resources, err);

cleanup:
  ಯದಿ (doc)
    json_object_put (doc);
  ಮುಕ್ತ (parser_err);
  ಯದಿ (resources)
    free_runtime_spec_schema_config_linux_resources (resources);

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_update_from_file (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, ಧ್ರುವ ವರ್ಣ *file, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *content = ರಿಕ್ತ;
  size_t len;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = read_all_file (file, &content, &len, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ libcrun_container_update (context, id, content, len, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
compare_update_ಮೌಲ್ಯಗಳು (ಧ್ರುವ ಶೂನ್ಯ *a, ಧ್ರುವ ಶೂನ್ಯ *b)
{
  ಧ್ರುವ ಸಂರಚನಾ libcrun_update_ಮೌಲ್ಯ_s *aa = a;
  ಧ್ರುವ ಸಂರಚನಾ libcrun_update_ಮೌಲ್ಯ_s *bb = b;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = strcmp (aa->section, bb->section);
  ಯದಿ (ret)
    ಪ್ರತ್ಯಾಗಮ ret;
  ಪ್ರತ್ಯಾಗಮ strcmp (aa->name, bb->name);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_update_from_ಮೌಲ್ಯಗಳು (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id,
                                      ಸಂರಚನಾ libcrun_update_ಮೌಲ್ಯ_s *ಮೌಲ್ಯಗಳು, size_t len,
                                      libcrun_error_t *err)
{
  ಧ್ರುವ ವರ್ಣ *current_section = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *buf;
  json_gen_ctx *gen = ರಿಕ್ತ;
  size_t i, buf_len;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (! json_gen_init (&gen, ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "json_gen_init failed");
  json_gen_map_open (gen);

  qsort (ಮೌಲ್ಯಗಳು, len, ಪರಿಮಾಣ (ಸಂರಚನಾ libcrun_update_ಮೌಲ್ಯ_s), compare_update_ಮೌಲ್ಯಗಳು);

  ಪ್ರತಿ (i = 0; i < len; i++)
    {
      ಯದಿ (current_section == ರಿಕ್ತ || strcmp (ಮೌಲ್ಯಗಳು[i].section, current_section))
        {
          ಯದಿ (i > 0)
            json_gen_map_close (gen);

          current_section = ಮೌಲ್ಯಗಳು[i].section;
          json_gen_string (gen, current_section, strlen (current_section));
          json_gen_map_open (gen);
        }

      json_gen_string (gen, ಮೌಲ್ಯಗಳು[i].name, strlen (ಮೌಲ್ಯಗಳು[i].name));

      ಯದಿ (ಮೌಲ್ಯಗಳು[i].numeric)
        json_gen_ಸಂಖ್ಯೆ (gen, ಮೌಲ್ಯಗಳು[i].ಮೌಲ್ಯ, strlen (ಮೌಲ್ಯಗಳು[i].ಮೌಲ್ಯ));
      ಅನ್ಯಥಾ
        json_gen_string (gen, ಮೌಲ್ಯಗಳು[i].ಮೌಲ್ಯ, strlen (ಮೌಲ್ಯಗಳು[i].ಮೌಲ್ಯ));
    }
  ಯದಿ (len)
    json_gen_map_close (gen);

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);

  ret = libcrun_container_update (context, id, buf, buf_len, err);

  json_gen_free (gen);

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ
populate_array_field (ವರ್ಣ ***field, ವರ್ಣ *array[], size_t ಅಂಕೆ_elements)
{
  size_t i;

  *field = xmalloc0 ((ಅಂಕೆ_elements + 1) * ಪರಿಮಾಣ (ವರ್ಣ *));
  ಪ್ರತಿ (i = 0; i < ಅಂಕೆ_elements; i++)
    (*field)[i] = xstrdup (array[i]);

  (*field)[i] = ರಿಕ್ತ;
}

#ifdef HAVE_CAP
ಸ್ಥಿರ ಶೂನ್ಯ
populate_capabilities (ಸಂರಚನಾ features_info_s *info, ವರ್ಣ ***capabilities, size_t *ಅಂಕೆ_capabilities)
{
  size_t index = 0;
  cap_ಮೌಲ್ಯ_t i;
  ವರ್ಣ *endptr;
  ಪೂರ್ಣಾಙ್ಕ j;

  *ಅಂಕೆ_capabilities = 0;
  ಪ್ರತಿ (i = 0;; i++)
    {
      ವರ್ಣ *v = cap_to_name (i);
      ಯದಿ (v == ರಿಕ್ತ)
        ಭಙ್ಗ;
      strtol (v, &endptr, 10);
      ಯದಿ (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          ಭಙ್ಗ;
        }
      (*ಅಂಕೆ_capabilities)++;
    }

  *capabilities = xmalloc0 ((*ಅಂಕೆ_capabilities + 1) * ಪರಿಮಾಣ (ಧ್ರುವ ವರ್ಣ *));
  ಪ್ರತಿ (i = 0; i < (cap_ಮೌಲ್ಯ_t) *ಅಂಕೆ_capabilities; i++)
    {
      ವರ್ಣ *v = cap_to_name (i);
      ಯದಿ (v == ರಿಕ್ತ)
        ಭಙ್ಗ;
      strtol (v, &endptr, 10);
      ಯದಿ (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          ಭಙ್ಗ;
        }

      // Convert capability name to uppercase
      ಪ್ರತಿ (j = 0; v[j] != '\0'; j++)
        v[j] = toupper (v[j]);

      (*capabilities)[index] = v;
      index++;
    }

  (*capabilities)[index] = ರಿಕ್ತ; // Terminate the array with NULL
  populate_array_field (&(info->linux.capabilities), *capabilities, *ಅಂಕೆ_capabilities);
}
#endif

ಸ್ಥಿರ ಶೂನ್ಯ
retrieve_mount_options (ಸಂರಚನಾ features_info_s **info)
{
  cleanup_free ಧ್ರುವ ಸಂರಚನಾ propagation_flags_s *mount_options_list = ರಿಕ್ತ;
  size_t ಅಂಕೆ_mount_options = 0;

  // Retrieve mount options from wordlist
  mount_options_list = get_mount_flags_from_wordlist ();

  // Calculate the number of mount options
  ಯಾವತ್ (mount_options_list[ಅಂಕೆ_mount_options].name != ರಿಕ್ತ)
    ಅಂಕೆ_mount_options++;

  // Allocate memory for mount options in info struct
  (*info)->mount_options = xmalloc0 ((ಅಂಕೆ_mount_options + 1) * ಪರಿಮಾಣ (ವರ್ಣ *));

  // Copy mount options to info struct
  ಪ್ರತಿ (size_t i = 0; i < ಅಂಕೆ_mount_options; i++)
    (*info)->mount_options[i] = xstrdup (mount_options_list[i].name);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_get_features (libcrun_context_t *context, ಸಂರಚನಾ features_info_s **info, libcrun_error_t *err arg_unused)
{
  // Allocate memory for the features_info_s structure
  size_t ಅಂಕೆ_namspaces = ಪರಿಮಾಣ (namespaces) / ಪರಿಮಾಣ (namespaces[0]);
  size_t ಅಂಕೆ_operators = ಪರಿಮಾಣ (operators) / ಪರಿಮಾಣ (operators[0]);
  size_t ಅಂಕೆ_actions = ಪರಿಮಾಣ (actions) / ಪರಿಮಾಣ (actions[0]);
  size_t ಅಂಕೆ_hooks = ಪರಿಮಾಣ (hooks) / ಪರಿಮಾಣ (hooks[0]);
  size_t ಅಂಕೆ_archs = ಪರಿಮಾಣ (archs) / ಪರಿಮಾಣ (archs[0]);
  size_t ಅಂಕೆ_unsafe_annotations = ಪರಿಮಾಣ (potentially_unsafe_annotations) / ಪರಿಮಾಣ (potentially_unsafe_annotations[0]);
  cleanup_free ವರ್ಣ **capabilities = ರಿಕ್ತ;
  size_t ಅಂಕೆ_capabilities = 0;
  size_t ಅಂಕೆ_mempolicy_modes = ಪರಿಮಾಣ (mempolicy_modes) / ಪರಿಮಾಣ (mempolicy_modes[0]);
  size_t ಅಂಕೆ_mempolicy_flags = ಪರಿಮಾಣ (mempolicy_flags) / ಪರಿಮಾಣ (mempolicy_flags[0]);

  *info = xmalloc0 (ಪರಿಮಾಣ (ಸಂರಚನಾ features_info_s));

  // Hardcoded feature information
  (*info)->oci_version_min = xstrdup ("1.0.0");
  (*info)->oci_version_max = xstrdup ("1.1.0+dev");

  // Populate hooks
  populate_array_field (&((*info)->hooks), hooks, ಅಂಕೆ_hooks);

  // Populate mount_options
  retrieve_mount_options (info);

  // Populate namespaces
  populate_array_field (&((*info)->linux.namespaces), namespaces, ಅಂಕೆ_namspaces);

#ifdef HAVE_CAP
  // Populate capabilities
  populate_capabilities (*info, &capabilities, &ಅಂಕೆ_capabilities);
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
  populate_array_field (&((*info)->linux.seccomp.actions), actions, ಅಂಕೆ_actions);
  // Populate operators
  populate_array_field (&((*info)->linux.seccomp.operators), operators, ಅಂಕೆ_operators);
  // Populate archs
  populate_array_field (&((*info)->linux.seccomp.archs), archs, ಅಂಕೆ_archs);
#ಅನ್ಯಥಾ
  (*info)->linux.seccomp.enabled = false;
#endif

  // Put values for apparmor and selinux
  (*info)->linux.apparmor.enabled = true;
  (*info)->linux.selinux.enabled = true;

  (*info)->linux.intel_rdt.enabled = true;

  (*info)->linux.net_devices.enabled = true;

  populate_array_field (&((*info)->linux.memory_policy.mode), mempolicy_modes, ಅಂಕೆ_mempolicy_modes);
  populate_array_field (&((*info)->linux.memory_policy.flags), mempolicy_flags, ಅಂಕೆ_mempolicy_flags);

  // Put the values for mount extensions
  (*info)->linux.mount_ext.idmap.enabled = true;

  // Populate the values for annotations
#ifdef HAVE_SECCOMP
  {
    ಧ್ರುವ ಸಂರಚನಾ scmp_version *version = seccomp_version ();
    ವರ್ಣ *version_string = ರಿಕ್ತ;

    xasprintf (&version_string, "%u.%u.%u", version->major, version->minor, version->micro);
    (*info)->annotations.io_github_seccomp_libseccomp_version = version_string;
  }
#endif

  ಯದಿ (context->handler_manager && handler_by_name (context->handler_manager, "wasm"))
    (*info)->annotations.run_oci_crun_wasm = true;

#ಯದಿ HAVE_CRIU
  (*info)->annotations.run_oci_crun_checkpoint_enabled = true;
#endif
  (*info)->annotations.run_oci_crun_commit = GIT_VERSION;
  (*info)->annotations.run_oci_crun_version = PACKAGE_VERSION;

  populate_array_field (&((*info)->potentially_unsafe_annotations), potentially_unsafe_annotations, ಅಂಕೆ_unsafe_annotations);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ
features_add_string (json_gen_ctx *json_gen, ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
  json_gen_string (json_gen, ಕೀಲಿ, strlen (ಕೀಲಿ));
  json_gen_string (json_gen, ಮೌಲ್ಯ, strlen (ಮೌಲ್ಯ));
}

ಸ್ಥಿರ ಶೂನ್ಯ
features_add_bool (json_gen_ctx *json_gen, ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ, ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ)
{
  json_gen_string (json_gen, ಕೀಲಿ, strlen (ಕೀಲಿ));
  json_gen_bool (json_gen, ಮೌಲ್ಯ);
}

ಸ್ಥಿರ ಶೂನ್ಯ
features_add_bool_str (json_gen_ctx *json_gen, ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ, ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ)
{
  features_add_string (json_gen, ಕೀಲಿ, ಮೌಲ್ಯ ? "true" : "false");
}

ಸ್ಥಿರ ಶೂನ್ಯ
features_add_array (json_gen_ctx *json_gen, ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ, ವರ್ಣ **array)
{
  size_t i;

  json_gen_string (json_gen, ಕೀಲಿ, strlen (ಕೀಲಿ));
  json_gen_array_open (json_gen);
  ಯದಿ (array)
    ಪ್ರತಿ (i = 0; array[i] != ರಿಕ್ತ; i++)
      json_gen_string (json_gen, array[i], strlen (array[i]));
  json_gen_array_close (json_gen);
}

ಸ್ಥಿರ ಶೂನ್ಯ
features_add_linux_info (json_gen_ctx *json_gen, ಧ್ರುವ ಸಂರಚನಾ linux_info_s *linux)
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
  ಯದಿ (linux->seccomp.actions)
    features_add_array (json_gen, "actions", linux->seccomp.actions);
  ಯದಿ (linux->seccomp.operators)
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
  ಯದಿ (linux->memory_policy.mode)
    features_add_array (json_gen, "modes", linux->memory_policy.mode);
  ಯದಿ (linux->memory_policy.flags)
    features_add_array (json_gen, "flags", linux->memory_policy.flags);
  json_gen_map_close (json_gen);

  json_gen_map_close (json_gen);
}

ಸ್ಥಿರ ಶೂನ್ಯ
features_add_annotations_info (json_gen_ctx *json_gen, ಧ್ರುವ ಸಂರಚನಾ annotations_info_s *annotation)
{
  json_gen_string (json_gen, "annotations", strlen ("annotations"));
  json_gen_map_open (json_gen);

  ಯದಿ (! is_empty_string (annotation->io_github_seccomp_libseccomp_version))
    features_add_string (json_gen, "io.github.seccomp.libseccomp.version", annotation->io_github_seccomp_libseccomp_version);

  features_add_bool_str (json_gen, "org.opencontainers.runc.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);
  features_add_bool_str (json_gen, "run.oci.crun.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);

  features_add_string (json_gen, "run.oci.crun.commit", annotation->run_oci_crun_commit);
  features_add_string (json_gen, "run.oci.crun.version", annotation->run_oci_crun_version);

  features_add_bool_str (json_gen, "run.oci.crun.wasm", annotation->run_oci_crun_wasm);

  json_gen_map_close (json_gen);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_get_features_json (libcrun_context_t *context, ವರ್ಣ **out, libcrun_error_t *err)
{
  cleanup_struct_features ಸಂರಚನಾ features_info_s *info = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *buffer = ರಿಕ್ತ;
  json_gen_ctx *json_gen;
  size_t buffer_len;
  ಪೂರ್ಣಾಙ್ಕ ret;

  *out = ರಿಕ್ತ;

  ret = libcrun_container_get_features (context, &info, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (! json_gen_init (&json_gen, ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "cannot initialize JSON generator");

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

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Read the container status and fail if the container is not running.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
read_status_require_running (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id,
                             libcrun_container_status_t *status, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = libcrun_read_container_status (status, context->state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_is_container_running (status, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;
  ಯದಿ (ret == 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "the container `%s` is not running", id);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_pause (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ libcrun_container_pause_linux (&status, err);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_unpause (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ libcrun_container_unpause_linux (&status, err);
}

/* Translate the public options struct into the internal representation used by
   the linux/CRIU layer.  Strings are borrowed (not copied).  */
ಸ್ಥಿರ ಶೂನ್ಯ
cr_options_from_public (libcrun_checkpoint_restore_t *dst, ಸಂರಚನಾ libcrun_checkpoint_restore_options_s *src)
{
  memset (dst, 0, ಪರಿಮಾಣ (*dst));
  dst->image_path = (ವರ್ಣ *) src->image_path;
  dst->work_path = (ವರ್ಣ *) src->work_path;
  dst->parent_path = (ವರ್ಣ *) src->parent_path;
  dst->console_socket = src->console_socket;
  dst->lsm_profile = (ವರ್ಣ *) src->lsm_profile;
  dst->lsm_mount_context = (ವರ್ಣ *) src->lsm_mount_context;
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

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_checkpoint (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id,
                              ಸಂರಚನಾ libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಧ್ರುವ ವರ್ಣ *state_ಬೇರು = context->state_ಬೇರು;
  libcrun_container_status_t status = {};
  libcrun_checkpoint_restore_t cr;
  cleanup_container libcrun_container_t *container = ರಿಕ್ತ;

  cr_options_from_public (&cr, cr_options);

  ret = read_status_require_running (context, id, &status, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = read_container_config_from_state (&container, state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* Needed to use the same paths (e.g. the shared empty directory used for
     masked paths) as the container runtime and restore do.  */
  container->context = context;

  ret = libcrun_container_checkpoint_linux (&status, container, &cr, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (! (cr_options->leave_running || cr_options->pre_dump))
    ಪ್ರತ್ಯಾಗಮ container_delete_internal (context, ರಿಕ್ತ, ರಿಕ್ತ, id, true, true, err);

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* The dummy process only keeps the cgroup populated until the container is restored there.
   Its cgroup is below the container cgroup, so once the container runs it can freeze or kill
   the dummy process: never depend on it after that point.  */
ಸ್ಥಿರ ಶೂನ್ಯ
restore_proxy_process (ಪೂರ್ಣಾಙ್ಕ proxy_pid_pipe)
{
  ವರ್ಣ c;

  /* Wait to be killed by the parent, or for the parent to go away.  */
  TEMP_FAILURE_RETRY (ಪಠ (proxy_pid_pipe, &c, ಪರಿಮಾಣ (c)));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
restore_terminate_proxy_process (pid_t proxy_pid, ಧ್ರುವ ವರ್ಣ *proxy_cgroup, pid_t container_pid, ಪೂರ್ಣಾಙ್ಕ cgroup_manager, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ mode;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (cgroup_manager == CGROUP_MANAGER_SYSTEMD)
    {
      ವರ್ಣ ready_str[64];

      ret = snprintf (ready_str, ಪರಿಮಾಣ (ready_str), "MAINPID=%d", container_pid);
      ಯದಿ (UNLIKELY (ret >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (ready_str)))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = send_sd_notify (ready_str, err);
      /* Do not fail on errors.  */
      ಯದಿ (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (UNLIKELY (mode < 0))
    ಪ್ರತ್ಯಾಗಮ mode;

  /* SIGKILL terminates a process frozen with cgroup v2, while the cgroup v1 freezer does not
     deliver it until the cgroup is thawed, so do not wait for it there.  */
  kill (proxy_pid, SIGKILL);
  TEMP_FAILURE_RETRY (waitpid (proxy_pid, ರಿಕ್ತ, mode == CGROUP_MODE_UNIFIED ? 0 : WNOHANG));

  ಪ್ರತ್ಯಾಗಮ destroy_cgroup_path (proxy_cgroup, mode, err);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_restore (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id arg_unused,
                           ಸಂರಚನಾ libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  cleanup_cgroup_status ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status = ರಿಕ್ತ;
  cleanup_container libcrun_container_t *container = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ proxy_pid_pipe0 = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ proxy_pid_pipe1 = -1;
  runtime_spec_schema_config_schema *def;
  libcrun_container_status_t status = {};
  ಸಂರಚನಾ libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  libcrun_checkpoint_restore_t cr;
  cleanup_pid pid_t proxy_pid = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ seccomp_fd = -1;
  ಧ್ರುವ ವರ್ಣ *seccomp_bpf_data;
  ಪೂರ್ಣಾಙ್ಕ proxy_pid_pipe[2];
  ಪೂರ್ಣಾಙ್ಕ cgroup_manager;
  uid_t ಬೇರು_uid = -1;
  gid_t ಬೇರು_gid = -1;
  ಪೂರ್ಣಾಙ್ಕ ret;

  cr_options_from_public (&cr, cr_options);

  container = libcrun_container_load_from_file ("config.json", err);
  ಯದಿ (container == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ -1;

  container->context = context;
  def = container->container_def;

  ret = check_config_file (def, context, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_status_check_directories (context->state_ಬೇರು, context->id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_copy_config_file (context->id, context->state_ಬೇರು, container, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ fail;

  /* CRIU restores the seccomp filters of the checkpointed processes, but
     exec loads the filter from the seccomp.bpf file in the state directory,
     so it must be generated again for the restored container.  */
  seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ fail;

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ fail;
  close_and_reset (&seccomp_fd);

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  ಯದಿ (context->systemd_cgroup)
    cgroup_manager = CGROUP_MANAGER_SYSTEMD;
  ಅನ್ಯಥಾ ಯದಿ (context->force_no_cgroup)
    cgroup_manager = CGROUP_MANAGER_DISABLED;

  def = container->container_def;

  /* If we are root (either on the host or in a namespace),
   * then chown the cgroup to root in the container user namespace. */
  get_ಬೇರು_in_the_userns (def, container->host_uid, container->host_gid, &ಬೇರು_uid, &ಬೇರು_gid);

  /* If the root in the container is different than the current root user, attempt to chown
     the std streams before entering the user namespace.  Otherwise we might lose access
     to the user (as it is not mapped in the user namespace) and cannot chown them.  */
  ಯದಿ (ಬೇರು_uid > 0 || ಬೇರು_gid > 0)
    {
      ret = maybe_chown_std_streams (ಬೇರು_uid, ಬೇರು_gid, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಗಚ್ಛ fail;
    }

  /* CRIU runs as a child process, so the restored processes would inherit the session keyring.  */
  ret = setup_container_keyring (container, context, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  {
    cleanup_close ಪೂರ್ಣಾಙ್ಕ cgroup_dirfd = -1;
    bool needs_proxy_process;
    cleanup_cgroup_preenter ಸಂರಚನಾ libcrun_cgroup_args cg = {
      .resources = def->linux ? def->linux->resources : ರಿಕ್ತ,
      .annotations = container->annotations,
      .cgroup_path = def->linux ? def->linux->cgroups_path : "",
      .manager = cgroup_manager,
      .ಬೇರು_uid = ಬೇರು_uid,
      .ಬೇರು_gid = ಬೇರು_gid,
      .id = context->id,
      .state_ಬೇರು = context->state_ಬೇರು,
      .container = container,
    };

    /* The CRIU restore code uses bundle, rootfs and cgroup_path of status.   The cgroup_path is set later.  */
    status.bundle = (ವರ್ಣ *) context->bundle;
    status.rootfs = def->ಬೇರು->path;

    ret = libcrun_cgroup_preenter (&cg, &cgroup_dirfd, err);
    ಯದಿ (UNLIKELY (ret < 0))
      ಗಚ್ಛ fail;

    needs_proxy_process = cgroup_dirfd < 0;

    /* If the target cgroup was already created (with cgroupfs), we can restore the container directly there.  */
    ಯದಿ (! needs_proxy_process)
      {
        cleanup_free ವರ್ಣ *cgroup_path = ರಿಕ್ತ;

        ret = get_cgroup_dirfd_path (cgroup_dirfd, &cgroup_path, err);
        ಯದಿ (UNLIKELY (ret < 0))
          ಗಚ್ಛ fail;

        /* Complete the configuration for the cgroup before the container is
           restored there: CRIU resumes the restored processes before it
           returns, so the device filter and the limits must already be in
           place.  There is no process to move, as CRIU is run in the cgroup,
           and the controllers were enabled by precreate_cgroup.  */
        cg.joined = true;

        ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
        ಯದಿ (UNLIKELY (ret < 0))
          ಗಚ್ಛ fail;

        /* Restore the container directly in the desired cgroup.  */
        status.cgroup_path = cgroup_path;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        ಯದಿ (UNLIKELY (ret < 0))
          ಗಚ್ಛ fail;

        /* Now that the container is restored, keep any signal for it, so that
           it is forwarded to the init by wait_for_process, rather than acted
           upon by crun.  This cannot be done earlier, as CRIU, which is run as
           a child process, would inherit the blocked signals.  */
        ret = block_signals (err);
        ಯದಿ (UNLIKELY (ret < 0))
          ಗಚ್ಛ fail;

        /* Use the container first process PID to finalize the cgroup.  */
        cg.pid = status.pid;
      }
    ಅನ್ಯಥಾ
      {
        /* When using systemd, we need a process first that must exist before a cgroup is created.
           Create a dummy process that sits and waits until the container is restored.  */
        ret = pipe2 (proxy_pid_pipe, O_CLOEXEC);
        ಯದಿ (UNLIKELY (ret < 0))
          {
            ret = crun_make_error (err, errno, "pipe");
            ಗಚ್ಛ fail;
          }

        /* Only used for auto cleanup.  */
        proxy_pid_pipe0 = proxy_pid_pipe[0];
        proxy_pid_pipe1 = proxy_pid_pipe[1];

        proxy_pid = fork ();
        ಯದಿ (UNLIKELY (proxy_pid < 0))
          {
            ret = crun_make_error (err, errno, "fork");
            ಗಚ್ಛ fail;
          }

        ಯದಿ (proxy_pid == 0)
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
        ಯದಿ (UNLIKELY (ret < 0))
          ಗಚ್ಛ fail;
      }

    ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
    ಯದಿ (UNLIKELY (ret < 0))
      ಗಚ್ಛ fail;

    /* When using a dummy process, the container is restored here once the cgroup is configured.  */
    ಯದಿ (needs_proxy_process)
      {
        cleanup_free ವರ್ಣ *target_cgroup = ರಿಕ್ತ;
        cleanup_free ವರ್ಣ *proxy_cgroup = ರಿಕ್ತ;

        ret = libcrun_get_cgroup_process (proxy_pid, &target_cgroup, false, err);
        ಯದಿ (UNLIKELY (ret < 0))
          ಗಚ್ಛ fail;

        /* Move the dummy process to a sub-cgroup.  */
        ret = append_paths (&proxy_cgroup, err, target_cgroup, ".proxy-crun", ರಿಕ್ತ);
        ಯದಿ (UNLIKELY (ret < 0))
          ಗಚ್ಛ fail;

        ret = libcrun_move_process_to_cgroup (proxy_pid, 0, proxy_cgroup, true, err);
        ಯದಿ (UNLIKELY (ret < 0))
          ಗಚ್ಛ fail;

        /* Restore the container in the same cgroup where the dummy process was.  */
        status.cgroup_path = target_cgroup;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        ಯದಿ (UNLIKELY (ret < 0))
          ಗಚ್ಛ fail;

        /* See the comment for the other libcrun_container_restore_linux call.  */
        ret = block_signals (err);
        ಯದಿ (UNLIKELY (ret < 0))
          ಗಚ್ಛ fail;

        /* The container is running, so do not fail because of the dummy process.  */
        ret = restore_terminate_proxy_process (proxy_pid, proxy_cgroup, status.pid, cgroup_manager, err);
        ಯದಿ (UNLIKELY (ret < 0))
          crun_error_release (err);
        proxy_pid = -1;

        close_and_reset (&proxy_pid_pipe1);
      }
  }

  context->detach = cr_options->detach;
  ret = write_container_status (container, context, status.pid, cgroup_status, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ fail;

  {
    /* Wait the same way run and exec do: apart from waiting for the container
       init, this writes the pid file, forwards any signal to the init, and
       reaps every child, not only the init.  */
    ಸಂರಚನಾ wait_for_process_args args = {
      .pid = status.pid,
      .context = context,
      .terminal_fd = -1,
      .notify_socket = -1,
      .container_ready_fd = ರಿಕ್ತ,
      .seccomp_notify_fd = -1,
      .seccomp_notify_plugins = ರಿಕ್ತ,
    };

    ಪ್ರತ್ಯಾಗಮ wait_for_process (&args, err);
  }

fail:
  /* Once restored, the container is running.  Without a status file nothing
     can reach it anymore, so do not leave it behind.  */
  ಯದಿ (status.pid > 0)
    {
      kill (status.pid, SIGKILL);
      waitpid_ignore_stopped (status.pid, ರಿಕ್ತ, 0);
    }
  ಯದಿ (cgroup_status)
    {
      libcrun_error_t tmp_err = ರಿಕ್ತ;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  {
    libcrun_error_t tmp_err = ರಿಕ್ತ;

    ಯದಿ (libcrun_container_delete_status (context->state_ಬೇರು, context->id, &tmp_err) < 0)
      crun_error_release (&tmp_err);
  }
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_read_pids (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  cleanup_cgroup_status ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status = ರಿಕ್ತ;
  cleanup_container_status libcrun_container_status_t status = {};
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = libcrun_read_container_status (&status, context->state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (status.cgroup_path == ರಿಕ್ತ || status.cgroup_path[0] == '\0')
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "the container is not using cgroups");

  cgroup_status = libcrun_cgroup_make_status (&status);

  ಪ್ರತ್ಯಾಗಮ libcrun_cgroup_read_pids (cgroup_status, recurse, pids, err);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_list_json (libcrun_context_t *context, ವರ್ಣ **out, libcrun_error_t *err)
{
  libcrun_container_list_t *list = ರಿಕ್ತ, *it;
  ಧ್ರುವ ವರ್ಣ *content = ರಿಕ್ತ;
  json_gen_ctx *gen = ರಿಕ್ತ;
  size_t len;
  ಪೂರ್ಣಾಙ್ಕ ret;

  *out = ರಿಕ್ತ;

  ret = libcrun_get_containers_list (&list, context->state_ಬೇರು, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (! json_gen_init (&gen, ರಿಕ್ತ))
    {
      ret = crun_make_error (err, 0, "cannot allocate json generator");
      ಗಚ್ಛ exit;
    }

  json_gen_config (gen, json_gen_beautify, 1);
  json_gen_array_open (gen);

  ಪ್ರತಿ (it = list; it; it = it->next)
    {
      libcrun_container_status_t status;
      ಪೂರ್ಣಾಙ್ಕ running = 0;
      ಪೂರ್ಣಾಙ್ಕ pid;
      ಧ್ರುವ ವರ್ಣ *container_status = ರಿಕ್ತ;

      ret = libcrun_read_container_status (&status, context->state_ಬೇರು, it->name, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಗಚ್ಛ exit;

      pid = status.pid;

      ret = libcrun_get_container_state_string (it->name, &status, context->state_ಬೇರು, &container_status,
                                                &running, err);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          ಅನುವರ್ತ;
        }

      ಯದಿ (! running)
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
  ಯದಿ (json_gen_get_buf (gen, &content, &len) != json_gen_status_ok)
    {
      ret = libcrun_make_error (err, 0, "cannot generate json list");
      ಗಚ್ಛ exit;
    }

  *out = xstrdup (content);
  ret = 0;

exit:
  ಯದಿ (list)
    libcrun_free_containers_list (list);
  ಯದಿ (gen)
    json_gen_free (gen);

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_write_json_containers_list (libcrun_context_t *context, FILE *out, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *content = ರಿಕ್ತ;
  size_t len;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = libcrun_container_list_json (context, &content, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  len = strlen (content);
  ಯದಿ (len && fwrite (content, 1, len, out) != len)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "error writing to file");

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_update_intel_rdt (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, ಸಂರಚನಾ libcrun_intel_rdt_update *update, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = read_container_config_from_state (&container, context->state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ libcrun_update_intel_rdt (id, container, update->l3_cache_schema, update->mem_bw_schema, update->schemata, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libcrun_container_add_or_remove_mounts_from_file (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, ಧ್ರುವ ವರ್ಣ *file, bool add, libcrun_error_t *err)
{
  cleanup_custom_handler_instance ಸಂರಚನಾ custom_handler_instance_s *custom_handler = ರಿಕ್ತ;
  cleanup_container libcrun_container_t *container = ರಿಕ್ತ;
  cleanup_free runtime_spec_schema_defs_mount **mounts = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *state_ಬೇರು = context->state_ಬೇರು;
  cleanup_free parser_error parser_err = ರಿಕ್ತ;
  ಸಂರಚನಾ parser_context pctx = { 0, stderr };
  libcrun_container_status_t status = {};
  cleanup_free ವರ್ಣ *content = ರಿಕ್ತ;
  json_object *doc = ರಿಕ್ತ;
  size_t n_mounts = 0, len, i;
  ಪೂರ್ಣಾಙ್ಕ ret = 1;

  ret = read_all_file (file, &content, &len, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_read_container_status (&status, state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = read_container_config_from_state (&container, state_ಬೇರು, id, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = parse_json_file (&doc, content, &pctx, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (! json_object_is_type (doc, json_type_array))
    {
      ret = crun_make_error (err, 0, "mounts must be an array");
      ಗಚ್ಛ cleanup;
    }
  ಅನ್ಯಥಾ
    {
      n_mounts = json_object_array_length (doc);

      mounts = xmalloc0 ((n_mounts + 1) * ಪರಿಮಾಣ (*mounts));
      ಪ್ರತಿ (i = 0; i < n_mounts; i++)
        {
          mounts[i] = make_runtime_spec_schema_defs_mount (json_object_array_get_idx (doc, i), &pctx, &parser_err);
          ಯದಿ (mounts[i] == ರಿಕ್ತ)
            {
              ret = crun_make_error (err, 0, "cannot parse mount: %s", parser_err);
              ಗಚ್ಛ cleanup;
            }
        }
    }

  ಯದಿ (add)
    ret = libcrun_make_runtime_mounts (container, &status, mounts, n_mounts, err);
  ಅನ್ಯಥಾ
    ret = libcrun_destroy_runtime_mounts (container, &status, mounts, n_mounts, err);

cleanup:
  ಯದಿ (doc)
    json_object_put (doc);
  ಯದಿ (mounts)
    ಪ್ರತಿ (i = 0; i < n_mounts; i++)
      free_runtime_spec_schema_defs_mount (mounts[i]);
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_add_mounts_from_file (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, ಧ್ರುವ ವರ್ಣ *file, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ libcrun_container_add_or_remove_mounts_from_file (context, id, file, true, err);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_remove_mounts_from_file (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, ಧ್ರುವ ವರ್ಣ *file, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ libcrun_container_add_or_remove_mounts_from_file (context, id, file, false, err);
}
