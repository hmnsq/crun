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
#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <ocispec/runtime_spec_schema_config_schema.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ "container.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "json_gen_utils.h"
#ಅನ್ತರ್ಭಾವ "seccomp.h"
#ಅನ್ತರ್ಭಾವ "mempolicy.h"
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SECCOMP
#  include <seccomp.h>
#ಆದರೆ_ಅಂತ್ಯ
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
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CAP
#  include <sys/capability.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <sys/ioctl.h>
#ಅನ್ತರ್ಭಾವ <termios.h>
#ಅನ್ತರ್ಭಾವ <grp.h>
#ಅನ್ತರ್ಭಾವ <git-version.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYSTEMD
#  include <systemd/sd-daemon.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <json-c/json.h>
#ಅನ್ತರ್ಭಾವ <ocispec/json_common.h>

ಎಣಿಕೆ
{
  SYNC_SOCKET_SYNC_ಸಂದೇಶ,
  SYNC_SOCKET_ERROR_ಸಂದೇಶ,
  SYNC_SOCKET_WARNING_ಸಂದೇಶ,
  SYNC_SOCKET_DEBUG_ಸಂದೇಶ,
};

ರಚನೆ container_entrypoint_s
{
  libcrun_container_t *container;
  libcrun_context_t *context;
  ಪೂರ್ಣಾಂಕ has_terminal_socket_pair;
  ಪೂರ್ಣಾಂಕ terminal_socketpair[2];

  /* Used by log_write_to_sync_socket.  */
  ಪೂರ್ಣಾಂಕ sync_socket;

  ಪೂರ್ಣಾಂಕ seccomp_fd;
  ಪೂರ್ಣಾಂಕ seccomp_receiver_fd;
  ಪೂರ್ಣಾಂಕ console_socket_fd;

  ಪೂರ್ಣಾಂಕ hooks_ಹೊರಹೋಗು_fd;
  ಪೂರ್ಣಾಂಕ hooks_err_fd;

  /* Absolute bundle path, to be reported to hooks run in the container.  */
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *bundle;

  ರಚನೆ custom_handler_instance_s *custom_handler;
};

ರಚನೆ sync_socket_ಸಂದೇಶ_s
{
  ಪೂರ್ಣಾಂಕ type;
  ಪೂರ್ಣಾಂಕ error_ಮೌಲ್ಯ;
  ಅಕ್ಷರ ಸಂದೇಶ_2[512];
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ runtime_spec_schema_defs_hook hook;

// linux hooks
ಅಕ್ಷರ *hooks[] = {
  "prestart",
  "createRuntime",
  "createContainer",
  "startContainer",
  "poststart",
  "poststop"
};

// linux namespaces
ಸ್ಥಿರ ಅಕ್ಷರ *namespaces[] = {
  "cgroup",
  "ipc",
  "mount",
  "network",
  "pid",
  "user",
  "uts"
};

ಸ್ಥಿರ ಅಕ್ಷರ *actions[] = {
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

ಸ್ಥಿರ ಅಕ್ಷರ *operators[] = {
  "SCMP_CMP_NE",
  "SCMP_CMP_LT",
  "SCMP_CMP_LE",
  "SCMP_CMP_EQ",
  "SCMP_CMP_GE",
  "SCMP_CMP_GT",
  "SCMP_CMP_MASKED_EQ",
};

ಸ್ಥಿರ ಅಕ್ಷರ *archs[] = {
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

ಸ್ಥಿರ ಅಕ್ಷರ *mempolicy_modes[] = {
  "MPOL_DEFAULT",
  "MPOL_PREFERRED",
  "MPOL_BIND",
  "MPOL_INTERLEAVE",
  "MPOL_LOCAL",
  "MPOL_PREFERRED_MANY",
  "MPOL_WEIGHTED_INTERLEAVE"
};

ಸ್ಥಿರ ಅಕ್ಷರ *mempolicy_flags[] = {
  "MPOL_F_NUMA_BALANCING",
  "MPOL_F_RELATIVE_NODES",
  "MPOL_F_STATIC_NODES"
};

ಸ್ಥಿರ ಅಕ್ಷರ *potentially_unsafe_annotations[] = {
  "module.wasm.image/variant",
  "io.kubernetes.cri.container-type",
  "run.oci.",
  "org.criu.",
  "krun.",
};

#ವ್ಯಾಖ್ಯಾನಿಸು SYNC_SOCKET_ಸಂದೇಶ_LEN(x, l) (offsetof (ರಚನೆ sync_socket_ಸಂದೇಶ_s, ಸಂದೇಶ_2) + l)

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
sync_socket_write_ಸಂದೇಶ (ಪೂರ್ಣಾಂಕ fd, ಪೂರ್ಣಾಂಕ verbosity, ಪೂರ್ಣಾಂಕ err_ಮೌಲ್ಯ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *log_ಸಂದೇಶ)
{
  ಪೂರ್ಣಾಂಕ ret;
  size_t err_len;
  ರಚನೆ sync_socket_ಸಂದೇಶ_s ಸಂದೇಶ;
  ಆಯ್ಕೆ (verbosity)
    {
    ಸ್ಥಿತಿ LIBCRUN_VERBOSITY_DEBUG:
      ಸಂದೇಶ.type = SYNC_SOCKET_DEBUG_ಸಂದೇಶ;
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ LIBCRUN_VERBOSITY_WARNING:
      ಸಂದೇಶ.type = SYNC_SOCKET_WARNING_ಸಂದೇಶ;
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ LIBCRUN_VERBOSITY_ERROR:
      ಸಂದೇಶ.type = SYNC_SOCKET_ERROR_ಸಂದೇಶ;
      ನಿಲ್ಲಿಸು;
    }
  ಸಂದೇಶ.error_ಮೌಲ್ಯ = err_ಮೌಲ್ಯ;

  ಆದರೆ (fd < 0)
    ಹಿಂತಿರುಗಿ 0;

  err_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (log_ಸಂದೇಶ);
  ಆದರೆ (err_len >= ಗಾತ್ರ (ಸಂದೇಶ.ಸಂದೇಶ_2))
    err_len = ಗಾತ್ರ (ಸಂದೇಶ.ಸಂದೇಶ_2) - 1;

  ಸ್ಮರಣೆ_ನಕಲು (ಸಂದೇಶ.ಸಂದೇಶ_2, log_ಸಂದೇಶ, err_len);
  ಸಂದೇಶ.ಸಂದೇಶ_2[err_len] = '\0';

  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (fd, &ಸಂದೇಶ, SYNC_SOCKET_ಸಂದೇಶ_LEN (ಸಂದೇಶ, err_len + 1)));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ -1;

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
sync_socket_write_error (ಪೂರ್ಣಾಂಕ fd, libcrun_error_t *ಹೊರಹೋಗು_err)
{
  ಆದರೆ (fd < 0)
    ಹಿಂತಿರುಗಿ 0;
  ಹಿಂತಿರುಗಿ sync_socket_write_ಸಂದೇಶ (fd, false, (*ಹೊರಹೋಗು_err)->ಸ್ಥಿತಿ_2, (*ಹೊರಹೋಗು_err)->ಸಂದೇಶ);
}

ಸ್ಥಿರ ಶೂನ್ಯ
log_write_ಗಮ್ಯ_sync_socket (ಪೂರ್ಣಾಂಕ errno_, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, ಪೂರ್ಣಾಂಕ verbosity, ಶೂನ್ಯ *arg)
{
  ರಚನೆ container_entrypoint_s *entrypoint_ವಾದಗಳು = arg;
  ಪೂರ್ಣಾಂಕ fd = entrypoint_ವಾದಗಳು->sync_socket;

  ಆದರೆ (fd < 0)
    ಹಿಂತಿರುಗಿ;

  ಆದರೆ (sync_socket_write_ಸಂದೇಶ (fd, verbosity, errno_, ಸಂದೇಶ) < 0)
    log_write_ಗಮ್ಯ_stderr (errno_, ಸಂದೇಶ, verbosity, arg);
}

ಸ್ಥಿರ bool
is_ಸ್ಮರಣೆ_ಎಲ್ಲೆ_too_ಕೆಳಗಿನ (runtime_spec_schema_config_schema *def)
{
  ಸ್ಥಿರಾಂಕ ದೀರ್ಘ ಸ್ಮರಣೆ_ಎಲ್ಲೆ_too_ಕೆಳಗಿನ = 1024 * 1024;

  ಆದರೆ (def->linux == ಖಾಲಿ || def->linux->resources == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ false;

  ಆದರೆ (def->linux->resources->ಸ್ಮರಣೆ
      && def->linux->resources->ಸ್ಮರಣೆ->ಎಲ್ಲೆ_present
      && def->linux->resources->ಸ್ಮರಣೆ->ಎಲ್ಲೆ < ಸ್ಮರಣೆ_ಎಲ್ಲೆ_too_ಕೆಳಗಿನ)
    ಹಿಂತಿರುಗಿ true;

  ಆದರೆ (def->linux->resources->unified)
    {
      size_t i;

      ಗಾಗಿ (i = 0; i < def->linux->resources->unified->len; i++)
        ಆದರೆ (strcmp (def->linux->resources->unified->ಕೀಲಿಗಳು[i], "memory.max") == 0)
          {
            ದೀರ್ಘ ಎಲ್ಲೆ;

            errno = 0;
            ಎಲ್ಲೆ = strtol (def->linux->resources->unified->ಮೌಲ್ಯಗಳು[i], ಖಾಲಿ, 10);
            ಆದರೆ (errno != 0)
              ಹಿಂತಿರುಗಿ false;
            ಆದರೆ (ಎಲ್ಲೆ < ಸ್ಮರಣೆ_ಎಲ್ಲೆ_too_ಕೆಳಗಿನ)
              ಹಿಂತಿರುಗಿ true;
          }
    }

  ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
sync_socket_wait_sync (libcrun_context_t *context, ಪೂರ್ಣಾಂಕ fd, bool flush, libcrun_error_t *err)
{
  ರಚನೆ sync_socket_ಸಂದೇಶ_s ಸಂದೇಶ;

  ಆದರೆ (fd < 0)
    ಹಿಂತಿರುಗಿ 0;

  ಆಗ (true)
    {
      ಪೂರ್ಣಾಂಕ ret;

      ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (fd, &ಸಂದೇಶ, ಗಾತ್ರ (ಸಂದೇಶ)));
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          ಆದರೆ (flush)
            ಹಿಂತಿರುಗಿ 0;
          ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "read from sync socket");
        }

      ಆದರೆ (ret == 0)
        {
          ಆದರೆ (flush)
            ಹಿಂತಿರುಗಿ 0;

          ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "read from the init process");
        }

      ಆದರೆ (! flush && ಸಂದೇಶ.type == SYNC_SOCKET_SYNC_ಸಂದೇಶ)
        ಹಿಂತಿರುಗಿ 0;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಸಂದೇಶ.type == SYNC_SOCKET_DEBUG_ಸಂದೇಶ)
        {
          ಆದರೆ (context)
            context->output_handler (ಸಂದೇಶ.error_ಮೌಲ್ಯ, ಸಂದೇಶ.ಸಂದೇಶ_2, LIBCRUN_VERBOSITY_DEBUG, context->output_handler_arg);
          ಮುಂದುವರಿ;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಸಂದೇಶ.type == SYNC_SOCKET_WARNING_ಸಂದೇಶ)
        {
          ಆದರೆ (context)
            context->output_handler (ಸಂದೇಶ.error_ಮೌಲ್ಯ, ಸಂದೇಶ.ಸಂದೇಶ_2, LIBCRUN_VERBOSITY_WARNING, context->output_handler_arg);
          ಮುಂದುವರಿ;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಸಂದೇಶ.type == SYNC_SOCKET_ERROR_ಸಂದೇಶ)
        ಹಿಂತಿರುಗಿ crun_make_error (err, ಸಂದೇಶ.error_ಮೌಲ್ಯ, "%s", ಸಂದೇಶ.ಸಂದೇಶ_2);
    }
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
sync_socket_send_sync (ಪೂರ್ಣಾಂಕ fd, bool flush_errors, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  ರಚನೆ sync_socket_ಸಂದೇಶ_s ಸಂದೇಶ = {
    0,
  };
  ಸಂದೇಶ.type = SYNC_SOCKET_SYNC_ಸಂದೇಶ;

  ಆದರೆ (fd < 0)
    ಹಿಂತಿರುಗಿ 0;

  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (fd, &ಸಂದೇಶ, SYNC_SOCKET_ಸಂದೇಶ_LEN (ಸಂದೇಶ, 0)));
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಆದರೆ (flush_errors)
        {
          ಪೂರ್ಣಾಂಕ saved_errno = errno;
          ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (fd, &ಸಂದೇಶ, ಗಾತ್ರ (ಸಂದೇಶ)));
          ಆದರೆ (ret >= 0 && ಸಂದೇಶ.type == SYNC_SOCKET_ERROR_ಸಂದೇಶ)
            ಹಿಂತಿರುಗಿ crun_make_error (err, ಸಂದೇಶ.error_ಮೌಲ್ಯ, "%s", ಸಂದೇಶ.ಸಂದೇಶ_2);
          errno = saved_errno;
        }
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "write to sync socket");
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ bool
has_cap_sys_ptrace ()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CAP
  ರಚನೆ __user_cap_header_struct hdr = { LINUX_CAPABILITY_ಆವೃತ್ತಿ_3, 0 };
  ರಚನೆ __user_cap_data_struct ದತ್ತಾಂಶ[2] = { { 0 } };

  ಆದರೆ (UNLIKELY (capget (&hdr, ದತ್ತಾಂಶ) < 0))
    ಹಿಂತಿರುಗಿ true;

  ಹಿಂತಿರುಗಿ (ದತ್ತಾಂಶ[0].effective & (1U << CAP_SYS_PTRACE)) != 0;
#ಇಲ್ಲದಿದ್ದರೆ
  ಹಿಂತಿರುಗಿ geteuid () == 0;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ libcrun_container_t *
make_container (runtime_spec_schema_config_schema *container_def, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *config)
{
  libcrun_container_t *container = xmalloc0 (ಗಾತ್ರ (*container));
  container->container_def = container_def;

  container->host_uid = geteuid ();
  container->host_gid = getegid ();
  container->host_has_cap_sys_ptrace = has_cap_sys_ptrace ();
  container->proc_fd = -1;

  container->annotations = make_string_map_from_json (container_def->annotations);

  /* An empty namespace path means that a new namespace is to be created,
     the same as if it is not specified.  */
  ಆದರೆ (container_def->linux)
    {
      size_t i;

      ಗಾಗಿ (i = 0; i < container_def->linux->namespaces_len; i++)
        ಆದರೆ (container_def->linux->namespaces[i]->ಮಾರ್ಗ && container_def->linux->namespaces[i]->ಮಾರ್ಗ[0] == '\0')
          {
            ಬಿಡುಗಡೆ (container_def->linux->namespaces[i]->ಮಾರ್ಗ);
            container_def->linux->namespaces[i]->ಮಾರ್ಗ = ಖಾಲಿ;
          }
    }

  ಆದರೆ (ಮಾರ್ಗ)
    container->config_file = xstrdup (ಮಾರ್ಗ);
  ಆದರೆ (config)
    container->config_file_content = xstrdup (config);

  ಹಿಂತಿರುಗಿ container;
}

libcrun_container_t *
libcrun_container_ತುಂಬು_from_ಸ್ಮರಣೆ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *json, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free ಅಕ್ಷರ *oci_error = ಖಾಲಿ;
  container_def = runtime_spec_schema_config_schema_parse_ದತ್ತಾಂಶ (json, ಖಾಲಿ, &oci_error);
  ಆದರೆ (container_def == ಖಾಲಿ)
    {
      crun_make_error (err, 0, "load: `%s`", oci_error);
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }
  ಹಿಂತಿರುಗಿ make_container (container_def, ಖಾಲಿ, json);
}

libcrun_container_t *
libcrun_container_ತುಂಬು_from_file (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free ಅಕ್ಷರ *oci_error = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *content = ಖಾಲಿ;
  size_t len;
  ಪೂರ್ಣಾಂಕ ret;

  libcrun_debug ("Loading container from config file: `%s`", ಮಾರ್ಗ);
  ret = read_all_file (ಮಾರ್ಗ, &content, &len, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

  container_def = runtime_spec_schema_config_schema_parse_ದತ್ತಾಂಶ (content, ಖಾಲಿ, &oci_error);
  ಆದರೆ (container_def == ಖಾಲಿ)
    {
      crun_make_error (err, 0, "load `%s`: %s", ಮಾರ್ಗ, oci_error);
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }
  ಹಿಂತಿರುಗಿ make_container (container_def, ಖಾಲಿ, content);
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err)
{
  ರಚನೆ parser_context ctx = { 0, stderr };
  parser_error gen_err = ಖಾಲಿ;

  ಆದರೆ (container == ಖಾಲಿ)
    {
      crun_make_error (err, EINVAL, "no container specified");
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

  ಆದರೆ (container->config_file_content == ಖಾಲಿ && container->container_def)
    {
      container->config_file_content = runtime_spec_schema_config_schema_generate_json (container->container_def, &ctx, &gen_err);
      ಆದರೆ (container->config_file_content == ಖಾಲಿ)
        {
          crun_make_error (err, 0, "cannot generate the configuration JSON: %s", gen_err ?: "unknown error");
          ಬಿಡುಗಡೆ (gen_err);
          ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }
      ಬಿಡುಗಡೆ (gen_err);
    }

  ಆದರೆ (container->config_file_content == ಖಾಲಿ)
    {
      crun_make_error (err, 0, "the container has no configuration");
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

  ಹಿಂತಿರುಗಿ container->config_file_content;
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
libcrun_container_get_annotation (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ)
{
  ಆದರೆ (container == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
  ಹಿಂತಿರುಗಿ ಕಂಡುಹಿಡಿ_string_map_ಮೌಲ್ಯ (container->annotations, ಕೀಲಿ);
}

size_t
libcrun_container_get_annotations_len (libcrun_container_t *container)
{
  ಆದರೆ (container == ಖಾಲಿ || container->annotations == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;
  ಹಿಂತಿರುಗಿ string_map_ಗಾತ್ರ (container->annotations);
}

ಪೂರ್ಣಾಂಕ
libcrun_container_get_annotation_at (libcrun_container_t *container, size_t ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಕೀಲಿ,
                                     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಮೌಲ್ಯ)
{
  ಆದರೆ (container == ಖಾಲಿ)
    {
      errno = ERANGE;
      ಹಿಂತಿರುಗಿ -1;
    }
  ಹಿಂತಿರುಗಿ string_map_get_at (container->annotations, ಸೂಚ್ಯಂಕ, ಕೀಲಿ, ಮೌಲ್ಯ);
}

uid_t
libcrun_container_get_uid (libcrun_container_t *container)
{
  ಆದರೆ (container == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ (uid_t) -1;
  ಹಿಂತಿರುಗಿ container->host_uid;
}

gid_t
libcrun_container_get_gid (libcrun_container_t *container)
{
  ಆದರೆ (container == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ (gid_t) -1;
  ಹಿಂತಿರುಗಿ container->host_gid;
}

ಶೂನ್ಯ
libcrun_container_free (libcrun_container_t *ctr)
{
  ಆದರೆ (ctr == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;

  ಆದರೆ (ctr->cleanup_ಖಾಸಗಿ_ದತ್ತಾಂಶ)
    ctr->cleanup_ಖಾಸಗಿ_ದತ್ತಾಂಶ (ctr->ಖಾಸಗಿ_ದತ್ತಾಂಶ);

  ಆದರೆ (ctr->container_def)
    free_runtime_spec_schema_config_schema (ctr->container_def);

  free_string_map (ctr->annotations);

  ಆದರೆ (ctr->proc_fd >= 0)
    close (ctr->proc_fd);

  ಬಿಡುಗಡೆ (ctr->config_file_content);
  ಬಿಡುಗಡೆ (ctr->config_file);
  ಬಿಡುಗಡೆ (ctr);
}

libcrun_context_t *
libcrun_context_new (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, libcrun_error_t *err)
{
  libcrun_context_t *ctx = xmalloc0 (ಗಾತ್ರ (*ctx));

  /* Go through the setters so that the owned_* copies and the borrowed
     pointers stay in sync and are released by libcrun_context_free.  */
  libcrun_context_ಗಣ_id (ctx, id);
  libcrun_context_ಗಣ_state_ಬೇರು (ctx, state_ಬೇರು);
  libcrun_context_ಗಣ_bundle (ctx, ".");
  ctx->fifo_exec_wait_fd = -1;

  ಆದರೆ (libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, id, ಖಾಲಿ, err) < 0)
    {
      libcrun_context_free (ctx);
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

  ctx->handler_manager = libcrun_handler_manager_create (err);
  ಆದರೆ (ctx->handler_manager == ಖಾಲಿ)
    {
      libcrun_context_free (ctx);
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CRUN_LIBDIR
  ಆದರೆ (access (CRUN_LIBDIR "/handlers", F_OK) == 0)
    {
      ಆದರೆ (libcrun_handler_manager_ತುಂಬು_directory (ctx->handler_manager, CRUN_LIBDIR "/handlers", err) < 0)
        {
          libcrun_context_free (ctx);
          ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }
    }
#ಆದರೆ_ಅಂತ್ಯ

  ಹಿಂತಿರುಗಿ ctx;
}

ಶೂನ್ಯ
libcrun_context_free (libcrun_context_t *ctx)
{
  ಆದರೆ (ctx == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;

  ಬಿಡುಗಡೆ (ctx->owned_state_ಬೇರು);
  ಬಿಡುಗಡೆ (ctx->owned_id);
  ಬಿಡುಗಡೆ (ctx->owned_bundle);
  ಬಿಡುಗಡೆ (ctx->owned_console_socket);
  ಬಿಡುಗಡೆ (ctx->owned_pid_file);
  ಬಿಡುಗಡೆ (ctx->owned_notify_socket);
  ಬಿಡುಗಡೆ (ctx->owned_handler);

  ಆದರೆ (ctx->handler_manager)
    handler_manager_free (ctx->handler_manager);

  ಬಿಡುಗಡೆ (ctx);
}

#ವ್ಯಾಖ್ಯಾನಿಸು LIBCRUN_CONTEXT_ಗಣ_STRING(ಕ್ಷೇತ್ರ)                  \
  ಮಾಡು                                                       \
    {                                                      \
      ಬಿಡುಗಡೆ (ctx->owned_##field);                           \
      ctx->owned_##field = ಮೌಲ್ಯ ? xstrdup (ಮೌಲ್ಯ) : ಖಾಲಿ; \
      ctx->ಕ್ಷೇತ್ರ = ctx->owned_##field;                     \
  } ಆಗ (0)

ಶೂನ್ಯ
libcrun_context_ಗಣ_id (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
  LIBCRUN_CONTEXT_ಗಣ_STRING (id);
}

ಶೂನ್ಯ
libcrun_context_ಗಣ_state_ಬೇರು (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
  LIBCRUN_CONTEXT_ಗಣ_STRING (state_ಬೇರು);
}

ಶೂನ್ಯ
libcrun_context_ಗಣ_bundle (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
  LIBCRUN_CONTEXT_ಗಣ_STRING (bundle);
}

ಶೂನ್ಯ
libcrun_context_ಗಣ_console_socket (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
  LIBCRUN_CONTEXT_ಗಣ_STRING (console_socket);
}

ಶೂನ್ಯ
libcrun_context_ಗಣ_pid_file (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
  LIBCRUN_CONTEXT_ಗಣ_STRING (pid_file);
}

ಶೂನ್ಯ
libcrun_context_ಗಣ_notify_socket (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
  LIBCRUN_CONTEXT_ಗಣ_STRING (notify_socket);
}

ಶೂನ್ಯ
libcrun_context_ಗಣ_handler (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
  LIBCRUN_CONTEXT_ಗಣ_STRING (handler);
}

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು LIBCRUN_CONTEXT_ಗಣ_STRING

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
libcrun_context_get_pid_file (libcrun_context_t *ctx)
{
  ಹಿಂತಿರುಗಿ ctx->pid_file;
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
libcrun_context_get_state_ಬೇರು (libcrun_context_t *ctx)
{
  ಆದರೆ (ctx == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
  ಹಿಂತಿರುಗಿ ctx->state_ಬೇರು;
}

ಶೂನ್ಯ
libcrun_context_ಗಣ_preserve_fds (libcrun_context_t *ctx, ಪೂರ್ಣಾಂಕ preserve_fds)
{
  ctx->preserve_fds = preserve_fds;
}

ಪೂರ್ಣಾಂಕ
libcrun_context_get_preserve_fds (libcrun_context_t *ctx)
{
  ಹಿಂತಿರುಗಿ ctx->preserve_fds;
}

ಶೂನ್ಯ
libcrun_context_ಗಣ_listen_fds (libcrun_context_t *ctx, ಪೂರ್ಣಾಂಕ listen_fds)
{
  ctx->listen_fds = listen_fds;
}

ಶೂನ್ಯ
libcrun_context_ಗಣ_ವಾದಗಳು (libcrun_context_t *ctx, ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ctx->argc = argc;
  ctx->argv = argv;
}

ಶೂನ್ಯ
libcrun_context_ಗಣ_systemd_cgroup (libcrun_context_t *ctx, bool ಮೌಲ್ಯ)
{
  ctx->systemd_cgroup = ಮೌಲ್ಯ;
}

ಶೂನ್ಯ
libcrun_context_ಗಣ_detach (libcrun_context_t *ctx, bool ಮೌಲ್ಯ)
{
  ctx->detach = ಮೌಲ್ಯ;
}

ಶೂನ್ಯ
libcrun_context_ಗಣ_no_new_keyring (libcrun_context_t *ctx, bool ಮೌಲ್ಯ)
{
  ctx->no_new_keyring = ಮೌಲ್ಯ;
}

ಶೂನ್ಯ
libcrun_context_ಗಣ_no_ಅಚ್ಚು (libcrun_context_t *ctx, bool ಮೌಲ್ಯ)
{
  ctx->no_ಅಚ್ಚು = ಮೌಲ್ಯ;
}

ಶೂನ್ಯ
libcrun_context_ಗಣ_force_no_cgroup (libcrun_context_t *ctx, bool ಮೌಲ್ಯ)
{
  ctx->force_no_cgroup = ಮೌಲ್ಯ;
}

ಶೂನ್ಯ
libcrun_context_ಗಣ_output_handler (libcrun_context_t *ctx, crun_output_handler handler, ಶೂನ್ಯ *arg)
{
  ctx->output_handler = handler;
  ctx->output_handler_arg = arg;
}

ಪೂರ್ಣಾಂಕ
libcrun_context_init_logging (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಲಘುಗಣಕ, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, ctx->id, ಲಘುಗಣಕ, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಖಂಡ_ಸಂಕೇತಗಳು (libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  sigset_t ಮುಖವಾಡ;
  sigfillset (&ಮುಖವಾಡ);
  ret = sigprocmask (SIG_ಖಂಡ, &ಮುಖವಾಡ, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "sigprocmask");
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
unblock_ಸಂಕೇತಗಳು (libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ i;
  ಪೂರ್ಣಾಂಕ ret;
  sigset_t ಮುಖವಾಡ;
  ರಚನೆ sigaction act = {};

  sigfillset (&ಮುಖವಾಡ);
  ret = sigprocmask (SIG_UNBLOCK, &ಮುಖವಾಡ, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "sigprocmask");

  act.sa_handler = SIG_DFL;
  ಗಾಗಿ (i = 0; i < NSIG; i++)
    {
      ret = sigaction (i, &act, ಖಾಲಿ);
      ಆದರೆ (ret < 0 && errno != EINVAL)
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "sigaction");
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* must be used on the host before pivot_root(2).  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
initialize_security (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ret = libcrun_init_caps (container, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (UNLIKELY (proc == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ 0;

  ಆದರೆ (proc->apparmor_profile)
    {
      ret = libcrun_initialize_apparmor (err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ret = libcrun_initialize_selinux (container, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ 0;
}

/* Emit the "annotations" map.  Returns a json_gen_status; callers can wrap
   the call with GEN_OR_FAIL.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
gen_annotations (json_gen_ctx *gen, json_map_string_string *annotations)
{
  size_t i;
  ಪೂರ್ಣಾಂಕ r;

  GEN_ಕೀಲಿ (gen, "annotations");
  GEN_OR_FAIL (json_gen_map_open (gen));

  ಗಾಗಿ (i = 0; i < annotations->len; i++)
    {
      /* An annotation whose value is null in the document is parsed into a
         NULL, which cannot be emitted as a JSON string; leave it out.  */
      ಆದರೆ (annotations->ಕೀಲಿಗಳು[i] == ಖಾಲಿ || annotations->ಮೌಲ್ಯಗಳು[i] == ಖಾಲಿ)
        ಮುಂದುವರಿ;

      GEN_STR (gen, annotations->ಕೀಲಿಗಳು[i]);
      GEN_STR (gen, annotations->ಮೌಲ್ಯಗಳು[i]);
    }

  ಹಿಂತಿರುಗಿ json_gen_map_close (gen);

gen_error:
  ಹಿಂತಿರುಗಿ r;
}

/* Return the bundle path to report in the container state.  crun always
   chdir()s into the bundle, so fall back to getcwd() when the caller has no
   absolute bundle path, as can happen when libcrun is used as a library.
   *ALLOCATED must be freed by the caller.  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
get_bundle_ಮಾರ್ಗ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *bundle, ಅಕ್ಷರ **allocated)
{
  ಆದರೆ (bundle != ಖಾಲಿ && bundle[0] == '/')
    ಹಿಂತಿರುಗಿ bundle;

  *allocated = getcwd (ಖಾಲಿ, 0);
  ಆದರೆ (*allocated == ಖಾಲಿ)
    OOM ();
  ಹಿಂತಿರುಗಿ *allocated;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_hooks (runtime_spec_schema_config_schema *def, pid_t pid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, bool keep_going, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *bundle,
          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cwd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿತಿ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, hook **hooks, size_t hooks_len, ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_fd,
          ಪೂರ್ಣಾಂಕ err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  size_t i, stdin_len;
  ಪೂರ್ಣಾಂಕ r, ret;
  ಅಕ್ಷರ *stdin = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *bundle_allocated = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs = def->ಬೇರು ? def->ಬೇರು->ಮಾರ್ಗ : "";
  cleanup_json_gen json_gen_ctx *gen = ಖಾಲಿ;

  bundle = get_bundle_ಮಾರ್ಗ (bundle, &bundle_allocated);

  ಆದರೆ (! json_gen_init (&gen, ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "json_gen_init failed");

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
  GEN_STR (gen, ಸ್ಥಿತಿ_2);

  ಆದರೆ (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **) &stdin, &stdin_len));

  ret = 0;

  bool error_created = false;
  ಗಾಗಿ (i = 0; i < hooks_len; i++)
    {
      ಅಕ್ಷರ **env = environ;

      ಆದರೆ (hooks[i]->env)
        env = hooks[i]->env;

      /* Release the error from the previous iteration, if any.  */
      ಆದರೆ (error_created)
        {
          crun_error_release (err);
          error_created = false;
        }

      ret = ಸತತ_process_with_stdin_timeout_envp (hooks[i]->ಮಾರ್ಗ, hooks[i]->ವಾದಗಳು, cwd, hooks[i]->timeout, env,
                                                 stdin, stdin_len, ಹೊರಹೋಗು_fd, err_fd, can_ignore_chdir_errors, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        error_created = true;

      ಆದರೆ (UNLIKELY (ret != 0))
        {
          ಆದರೆ (keep_going)
            libcrun_warning ("error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->ಮಾರ್ಗ, ret);
          ಇಲ್ಲದಿದ್ದರೆ
            {
              libcrun_error (0, "error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->ಮಾರ್ಗ, ret);
              ನಿಲ್ಲಿಸು;
            }
        }
    }

  ಹಿಂತಿರುಗಿ ret;

gen_error:
  ಹಿಂತಿರುಗಿ json_gen_error_ಗಮ್ಯ_crun_error (r, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_json_gen_ಫಲಿತಾಂಶ (json_gen_ctx *gen, ಅಕ್ಷರ **ಹೊರಹೋಗು, size_t *ಹೊರಹೋಗು_len)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *buf = ಖಾಲಿ;
  size_t buf_len = 0;
  ಪೂರ್ಣಾಂಕ r;

  r = json_gen_get_buf (gen, &buf, &buf_len);
  ಆದರೆ (UNLIKELY (r != json_gen_ಸ್ಥಿತಿ_ok))
    ಹಿಂತಿರುಗಿ r;

  *ಹೊರಹೋಗು_len = buf_len;

  *ಹೊರಹೋಗು = xmalloc (buf_len + 1);
  ಸ್ಮರಣೆ_ನಕಲು (*ಹೊರಹೋಗು, buf, buf_len);
  (*ಹೊರಹೋಗು)[buf_len] = '\0';

  ಹಿಂತಿರುಗಿ json_gen_ಸ್ಥಿತಿ_ok;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_seccomp_receiver_fd_payload (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿತಿ_2, pid_t own_pid,
                                 ಅಕ್ಷರ **seccomp_fd_payload, size_t *seccomp_fd_payload_len, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ r;
  json_gen_ctx *gen = ಖಾಲಿ;
  runtime_spec_schema_config_schema *def = container->container_def;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ OCI_ಆವೃತ್ತಿ = "0.2.0";

  ಆದರೆ (! json_gen_init (&gen, ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_ಕೀಲಿ (gen, "ociVersion");
  GEN_STR (gen, OCI_ಆವೃತ್ತಿ);

  GEN_ಕೀಲಿ (gen, "fds");
  GEN_OR_FAIL (json_gen_array_open (gen));

  GEN_ಕೀಲಿ (gen, "seccompFd");
  GEN_OR_FAIL (json_gen_array_close (gen));

  GEN_ಕೀಲಿ (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  ಆದರೆ (def && def->linux && def->linux->seccomp)
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *metadata = def->linux->seccomp->listener_metadata;

      ಆದರೆ (metadata)
        {
          GEN_ಕೀಲಿ (gen, "metadata");
          GEN_STR (gen, metadata);
        }
    }

  /* State.  */
  GEN_ಕೀಲಿ (gen, "state");
  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_ಕೀಲಿ (gen, "ociVersion");
  GEN_STR (gen, OCI_ಆವೃತ್ತಿ);

  ಆದರೆ (container->context && container->context->id)
    {
      GEN_ಕೀಲಿ (gen, "id");
      GEN_STR (gen, container->context->id);
    }

  GEN_ಕೀಲಿ (gen, "status");
  GEN_STR (gen, ಸ್ಥಿತಿ_2);

  GEN_ಕೀಲಿ (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  ಆದರೆ (container->context && container->context->bundle)
    {
      GEN_ಕೀಲಿ (gen, "bundle");
      GEN_STR (gen, container->context->bundle);
    }

  ಆದರೆ (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));
  /* End state.  */

  GEN_OR_FAIL (json_gen_map_close (gen));

  r = get_json_gen_ಫಲಿತಾಂಶ (gen, seccomp_fd_payload, seccomp_fd_payload_len);

gen_error:
  json_gen_free (gen);

  ಹಿಂತಿರುಗಿ json_gen_error_ಗಮ್ಯ_crun_error (r, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
send_sync_cb (ಶೂನ್ಯ *ದತ್ತಾಂಶ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ sync_socket_fd = *((ಪೂರ್ಣಾಂಕ *) ದತ್ತಾಂಶ);
  ಪೂರ್ಣಾಂಕ ret;

  /* sync 2.  */
  ret = sync_socket_send_sync (sync_socket_fd, false, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  /* sync 3.  */
  ಹಿಂತಿರುಗಿ sync_socket_wait_sync (ಖಾಲಿ, sync_socket_fd, false, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
maybe_chown_std_streams (uid_t container_uid, gid_t container_gid, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret, i;

  ಗಾಗಿ (i = 0; i < 3; i++)
    {
      ಆದರೆ (! isatty (i))
        {
          ರಚನೆ stat statbuf;
          ret = fstat (i, &statbuf);
          ಆದರೆ (UNLIKELY (ret < 0))
            {
              ಆದರೆ (errno == EBADF)
                ಮುಂದುವರಿ;
              ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fstat fd `%d`", i);
            }

          /* Skip chown for device files */
          ಆದರೆ (S_ISCHR (statbuf.st_mode) || S_ISBLK (statbuf.st_mode))
            ಮುಂದುವರಿ;

          ret = fchown (i, container_uid, container_gid);
          ಆದರೆ (UNLIKELY (ret < 0))
            {
              /* EINVAL means the user is not mapped in the current userns.
                 Ignore EPERM and EROFS as well as there is no reason to fail
                 so early, and let the container payload deal with it.
                 EBADF means fd is closed.
              */
              ಆದರೆ (errno == EINVAL || errno == EPERM || errno == EROFS || errno == EBADF)
                ಮುಂದುವರಿ;

              ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fchown std stream `%i`", i);
            }
        }
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_container_notify_handler (ರಚನೆ container_entrypoint_s *ವಾದಗಳು,
                                  ಎಣಿಕೆ handler_configure_phase phase,
                                  libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs,
                                  libcrun_error_t *err)
{
  ರಚನೆ custom_handler_s *h;

  ಆದರೆ (ವಾದಗಳು->custom_handler == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  h = ವಾದಗಳು->custom_handler->vtable;
  ಆದರೆ (h == ಖಾಲಿ || h->configure_container == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  ಹಿಂತಿರುಗಿ h->configure_container (ವಾದಗಳು->custom_handler->cookie, phase,
                                 ವಾದಗಳು->context, container,
                                 rootfs, err);
}

/* Resolve and normalize the container rootfs path.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
resolve_rootfs_ಮಾರ್ಗ (libcrun_container_t *container, ಅಕ್ಷರ **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಆದರೆ (def->ಬೇರು && def->ಬೇರು->ಮಾರ್ಗ)
    {
      *rootfs = realpath (def->ಬೇರು->ಮಾರ್ಗ, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (*rootfs == ಖಾಲಿ))
        {
          ಪೂರ್ಣಾಂಕ saved_errno = errno;

          /* If realpath failed for any reason, try the relative directory.  */
          ಆದರೆ (def->ಬೇರು->ಮಾರ್ಗ[0] == '/')
            {
              cleanup_free ಅಕ್ಷರ *cwd = ಖಾಲಿ;
              libcrun_error_t tmp_err = ಖಾಲಿ;
              ssize_t len;
              ಪೂರ್ಣಾಂಕ proc_fd;

              proc_fd = libcrun_get_cached_proc_fd (container, &tmp_err);
              ಆದರೆ (LIKELY (proc_fd >= 0))
                {
                  len = ಸುರಕ್ಷಿತ_readlinkat (proc_fd, "self/cwd", &cwd, 0, &tmp_err);
                  ಆದರೆ (LIKELY (len > 0))
                    {
                      /* If the rootfs is under the current working directory, just use its relative path.  */
                      ಆದರೆ (has_dir_ಪೂರ್ವಪ್ರತ್ಯಯ (def->ಬೇರು->ಮಾರ್ಗ, cwd))
                        {
                          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *it = consume_slashes (def->ಬೇರು->ಮಾರ್ಗ + len);
                          ಆದರೆ (LIKELY (*it))
                            {
                              ಪೂರ್ಣಾಂಕ ret = faccessat (AT_FDCWD, it, F_OK, 0);
                              ಆದರೆ (LIKELY (ret == 0))
                                *rootfs = xstrdup (it);
                            }
                        }
                    }
                }
              crun_error_release (&tmp_err);
            }

          ಆದರೆ (*rootfs == ಖಾಲಿ)
            {
              ಆದರೆ (saved_errno == ENOENT)
                ಹಿಂತಿರುಗಿ crun_make_error (err, ENOENT, "the rootfs path `%s` does not exist", def->ಬೇರು->ಮಾರ್ಗ);

              *rootfs = xstrdup (def->ಬೇರು->ಮಾರ್ಗ);
            }
        }
    }
  ಹಿಂತಿರುಗಿ 0;
}

/* Initialize the environment variables.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಸಿದ್ಧತೆ_environment (runtime_spec_schema_config_schema *def, uid_t container_uid, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (clearenv ())
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "clearenv");

  ಆದರೆ (def->process)
    {
      size_t i;

      ಗಾಗಿ (i = 0; i < def->process->env_len; i++)
        {
          /* A null element of the array of variables is parsed into a NULL,
             which putenv(3) cannot be handed.  */
          ಆದರೆ (def->process->env[i] == ಖಾಲಿ)
            ಹಿಂತಿರುಗಿ crun_make_error (err, EINVAL, "environment variable is not specified");

          ಆದರೆ (putenv (def->process->env[i]) < 0)
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "putenv `%s`", def->process->env[i]);
        }
    }

  ಆದರೆ (is_ಖಾಲಿಗಳು_string (getenv ("HOME")))
    {
      ret = ಗಣ_home_env (container_uid);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          ಆದರೆ (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "setenv HOME");
        }
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* Configure terminal and console for interactive containers.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಸಿದ್ಧತೆ_terminal (ರಚನೆ container_entrypoint_s *entrypoint_ವಾದಗಳು, libcrun_container_t *container,
                ಪೂರ್ಣಾಂಕ console_socket, ಪೂರ್ಣಾಂಕ console_socketpair, ಪೂರ್ಣಾಂಕ has_terminal, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ terminal_fd = -1;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (! has_terminal)
    ಹಿಂತಿರುಗಿ 0;

  fflush (stderr);

  ಆದರೆ (console_socket >= 0 || (entrypoint_ವಾದಗಳು->has_terminal_socket_pair && console_socketpair >= 0))
    {
      terminal_fd = libcrun_ಗಣ_terminal (container, err);
      ಆದರೆ (UNLIKELY (terminal_fd < 0))
        ಹಿಂತಿರುಗಿ terminal_fd;
    }

  ಆದರೆ (console_socket >= 0)
    {
      ret = send_fd_ಗಮ್ಯ_socket (console_socket, terminal_fd, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (entrypoint_ವಾದಗಳು->has_terminal_socket_pair && console_socketpair >= 0)
    {
      ret = send_fd_ಗಮ್ಯ_socket (console_socketpair, terminal_fd, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* Resolve and validate the container's executable path.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಸಿದ್ಧತೆ_executable_ಮಾರ್ಗ (ರಚನೆ container_entrypoint_s *entrypoint_ವಾದಗಳು, runtime_spec_schema_config_schema *def,
                       ಅಕ್ಷರ **exec_ಮಾರ್ಗ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (def->process && def->process->ವಾದಗಳು)
    {
      ret = ಕಂಡುಹಿಡಿ_executable (exec_ಮಾರ್ಗ, def->process->ವಾದಗಳು[0], def->process->cwd, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          ಆದರೆ (entrypoint_ವಾದಗಳು->custom_handler == ಖಾಲಿ && crun_error_get_errno (err) == ENOENT)
            ಹಿಂತಿರುಗಿ ret;

          /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
             once the process switched to the use that runs in the container.  This might be necessary
             when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
             is not honored.  */
          crun_error_release (err);
        }
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* Apply security settings for the container process.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
apply_security_settings (ರಚನೆ container_entrypoint_s *entrypoint_ವಾದಗಳು, runtime_spec_schema_config_schema *def,
                         libcrun_container_t *container, pid_t own_pid, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  runtime_spec_schema_config_schema_process_capabilities *capabilities;
  ಪೂರ್ಣಾಂಕ no_new_privs;

  ಆದರೆ (def->process && ! def->process->no_new_privileges)
    {
      ಅಕ್ಷರ **seccomp_flags = ಖಾಲಿ;
      size_t seccomp_flags_len = 0;
      cleanup_free ಅಕ್ಷರ *seccomp_fd_payload = ಖಾಲಿ;
      size_t seccomp_fd_payload_len = 0;

      ಆದರೆ (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      ಆದರೆ (entrypoint_ವಾದಗಳು->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_ವಾದಗಳು->seccomp_fd, entrypoint_ವಾದಗಳು->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      close_and_reset (&entrypoint_ವಾದಗಳು->seccomp_fd);
      close_and_reset (&entrypoint_ವಾದಗಳು->seccomp_receiver_fd);
    }

  capabilities = def->process ? def->process->capabilities : ಖಾಲಿ;
  no_new_privs = def->process ? def->process->no_new_privileges : 1;
  ret = libcrun_ಗಣ_caps (capabilities, container->container_uid, container->container_gid, no_new_privs,
                          entrypoint_ವಾದಗಳು->custom_handler != ಖಾಲಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ 0;
}

/* Create the process that joins the existing PID namespace now that the
   host mounts are not visible anymore, and continue the setup from there.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_delayed_pidns_fork (ಪೂರ್ಣಾಂಕ sync_socket, pid_t *own_pid, libcrun_error_t *err)
{
  pid_t pid;
  ಪೂರ್ಣಾಂಕ ret;

  pid = fork ();
  ಆದರೆ (UNLIKELY (pid < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fork");

  ಆದರೆ (pid)
    {
      /* Report the new PID to the parent and exit immediately.  */
      ret = sync_socket_send_sync (sync_socket, false, err);
      ಆದರೆ (LIKELY (ret == 0))
        {
          ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (sync_socket, &pid, ಗಾತ್ರ (pid)));
          ಆದರೆ (LIKELY (ret == ಗಾತ್ರ (pid)))
            ಸುರಕ್ಷಿತ_exit (EXIT_SUCCESS);
        }

      kill (pid, SIGKILL);
      ಸುರಕ್ಷಿತ_exit (EXIT_FAILURE);
    }

  /* sync receive own pid.  */
  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (sync_socket, own_pid, ಗಾತ್ರ (*own_pid)));
  ಆದರೆ (UNLIKELY (ret != ಗಾತ್ರ (*own_pid)))
    {
      ಆದರೆ (ret >= 0)
        errno = 0;
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "read from sync socket");
    }

  ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ delayed_pidns_fork_s
{
  libcrun_container_t *container;
  ಪೂರ್ಣಾಂಕ sync_socket;
  pid_t *own_pid;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
delayed_pidns_fork_cb (ಶೂನ್ಯ *ದತ್ತಾಂಶ, libcrun_error_t *err)
{
  ರಚನೆ delayed_pidns_fork_s *d = ದತ್ತಾಂಶ;

  ಆದರೆ (! libcrun_container_delayed_pidns_fork (d->container))
    ಹಿಂತಿರುಗಿ 0;

  ಹಿಂತಿರುಗಿ do_delayed_pidns_fork (d->sync_socket, d->own_pid, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
container_init_ಸಿದ್ಧತೆ (ಶೂನ್ಯ *ವಾದಗಳು, pid_t *own_pid, ಅಕ್ಷರ *notify_socket,
                      ಪೂರ್ಣಾಂಕ sync_socket, ಅಕ್ಷರ **exec_ಮಾರ್ಗ, libcrun_error_t *err)
{
  ರಚನೆ container_entrypoint_s *entrypoint_ವಾದಗಳು = ವಾದಗಳು;
  libcrun_container_t *container = entrypoint_ವಾದಗಳು->container;
  bool chdir_ಮುಗಿದ = false;
  ಪೂರ್ಣಾಂಕ ret;
  ಪೂರ್ಣಾಂಕ has_terminal;
  cleanup_close ಪೂರ್ಣಾಂಕ console_socket = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ console_socketpair = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free ಅಕ್ಷರ *rootfs = ಖಾಲಿ;
  ರಚನೆ delayed_pidns_fork_s delayed_pidns_fork = {
    .container = container,
    .sync_socket = sync_socket,
    .own_pid = own_pid,
  };

  ret = initialize_security (container, def->process, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_configure_ಜಾಲ (container, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = resolve_rootfs_ಮಾರ್ಗ (container, &rootfs, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  /* Configure terminal socket pair for container communication.  */
  ಆದರೆ (entrypoint_ವಾದಗಳು->terminal_socketpair[0] >= 0)
    {
      close_and_reset (&entrypoint_ವಾದಗಳು->terminal_socketpair[0]);
      console_socketpair = entrypoint_ವಾದಗಳು->terminal_socketpair[1];
    }

  /* sync 1.  */
  ret = sync_socket_wait_sync (ಖಾಲಿ, sync_socket, false, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  has_terminal = container->container_def->process && container->container_def->process->terminal;
  ಆದರೆ (has_terminal && entrypoint_ವಾದಗಳು->context->console_socket)
    console_socket = entrypoint_ವಾದಗಳು->console_socket_fd;

  ret = libcrun_ಗಣ_sysctl (container, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_container_notify_handler (entrypoint_ವಾದಗಳು, HANDLER_CONFIGURE_BEFORE_MOUNTS, container, rootfs, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_do_ಅಚ್ಚು_ಬೇರು (container, entrypoint_ವಾದಗಳು->context->no_ಅಚ್ಚು, &rootfs, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  /* sync 2 and 3 are sent as part of libcrun_set_mounts.  */
  ret = libcrun_ಗಣ_mounts (entrypoint_ವಾದಗಳು, container, rootfs, send_sync_cb, &sync_socket, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  /* The createContainer hooks MUST run after the mounts are set up but before
     the pivot_root.  When these hooks are present can_use_open_tree_namespace()
     forces the deferred-pivot path, so the actual pivot happens later in
     libcrun_finalize_mounts().  */
  ಆದರೆ (def->hooks && def->hooks->create_container_len)
    {
      libcrun_error_t tmp_err = ಖಾಲಿ;
      ಪೂರ್ಣಾಂಕ ಒಳ_userns = check_running_ಒಳ_user_namespace (&tmp_err);
      ಆದರೆ (tmp_err)
        crun_error_release (&tmp_err);
      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_ವಾದಗಳು->bundle, ಖಾಲಿ, "created", "createContainer",
                      (hook **) def->hooks->create_container,
                      def->hooks->create_container_len, entrypoint_ವಾದಗಳು->hooks_ಹೊರಹೋಗು_fd, entrypoint_ವಾದಗಳು->hooks_err_fd,
                      ಒಳ_userns > 0, err);
      ಆದರೆ (UNLIKELY (ret != 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ret = libcrun_finalize_mounts (entrypoint_ವಾದಗಳು, container, rootfs, delayed_pidns_fork_cb, &delayed_pidns_fork, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (def->process)
    {
      ret = libcrun_ಗಣ_selinux_label (container, def->process, false, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = libcrun_ಗಣ_apparmor_profile (container, def->process, false, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ret = mark_or_close_fds_ge_than (container, entrypoint_ವಾದಗಳು->context->preserve_fds + 3, false, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ret = libcrun_reopen_dev_null (err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = maybe_chown_std_streams (container->container_uid, container->container_gid, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = ಸಿದ್ಧತೆ_environment (def, container->container_uid, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  /* Set primary process to 1 explicitly if nothing is configured and LISTEN_FD is not set.  */
  ಆದರೆ (entrypoint_ವಾದಗಳು->context->listen_fds > 0 && getenv ("LISTEN_PID") == ಖಾಲಿ)
    {
      libcrun_warning ("setting LISTEN_PID=1 since no previous configuration was found");
      ಆದರೆ (UNLIKELY (setenv ("LISTEN_PID", "1", 1) < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "setenv LISTEN_PID");
    }

  /* Attempt to chdir immediately here, before doing the setresuid.  If we fail here, let's
     try again later once the process switched to the user that runs in the container.  */
  ಆದರೆ (def->process && def->process->cwd)
    {
      ret = libcrun_ಸುರಕ್ಷಿತ_chdir (def->process->cwd, err);
      ಆದರೆ (LIKELY (ret == 0))
        chdir_ಮುಗಿದ = true;
      ಇಲ್ಲದಿದ್ದರೆ
        crun_error_release (err);
    }

  ret = setsid ();
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "setsid");

  ret = ಸಿದ್ಧತೆ_terminal (entrypoint_ವಾದಗಳು, container, console_socket, console_socketpair, has_terminal, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  close_and_reset (&console_socket);
  close_and_reset (&console_socketpair);

  ret = ಸಿದ್ಧತೆ_executable_ಮಾರ್ಗ (entrypoint_ವಾದಗಳು, def, exec_ಮಾರ್ಗ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_ಗಣ_hostname (container, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_ಗಣ_domainname (container, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_ಗಣ_personality (container->container_def->linux->personality, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (def->process && def->process->user)
    umask (def->process->user->umask_present ? def->process->user->umask : 0022);

  ret = apply_security_settings (entrypoint_ವಾದಗಳು, def, container, *own_pid, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (UNLIKELY (def->process && def->process->ವಾದಗಳು && *exec_ಮಾರ್ಗ == ಖಾಲಿ))
    {
      ret = ಕಂಡುಹಿಡಿ_executable (exec_ಮಾರ್ಗ, def->process->ವಾದಗಳು[0], def->process->cwd, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          ಆದರೆ (entrypoint_ವಾದಗಳು->custom_handler == ಖಾಲಿ || is_ಖಾಲಿಗಳು_string (def->process->ವಾದಗಳು[0]))
            ಹಿಂತಿರುಗಿ ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          *exec_ಮಾರ್ಗ = xstrdup (def->process->ವಾದಗಳು[0]);
        }
    }

  /* The chdir was not already performed, so try again now after switching to the UID/GID in the container.  */
  ಆದರೆ (! chdir_ಮುಗಿದ && def->process && def->process->cwd)
    {
      ret = libcrun_ಸುರಕ್ಷಿತ_chdir (def->process->cwd, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (notify_socket && putenv (notify_socket) < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "putenv `%s`", notify_socket);

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
open_hooks_output (libcrun_container_t *container, ಪೂರ್ಣಾಂಕ *ಹೊರಹೋಗು_fd, ಪೂರ್ಣಾಂಕ *err_fd, libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *annotation;

  *err_fd = *ಹೊರಹೋಗು_fd = -1;

  libcrun_debug ("Opening hooks output");
  annotation = ಕಂಡುಹಿಡಿ_annotation (container, "run.oci.hooks.stdout");
  ಆದರೆ (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stdout` annotation");
      *ಹೊರಹೋಗು_fd = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      ಆದರೆ (UNLIKELY (*ಹೊರಹೋಗು_fd < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", annotation);
    }

  annotation = ಕಂಡುಹಿಡಿ_annotation (container, "run.oci.hooks.stderr");
  ಆದರೆ (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stderr` annotation");
      *err_fd = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      ಆದರೆ (UNLIKELY (*err_fd < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", annotation);
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* The environment the process was started with lives on the initial stack, right after the
   argv strings and before the string AT_EXECFN points to.  clearenv(3) only drops the pointers
   to it, so when there is no execv to replace the stack, erase it or the container process can
   still read it, e.g. through /proc/self/environ.  */
ಸ್ಥಿರ ಶೂನ್ಯ
erase_initial_environment (ಅಕ್ಷರ **argv, ಪೂರ್ಣಾಂಕ argc)
{
  ಅಕ್ಷರ *ಆರಂಭ = argv[argc - 1] + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (argv[argc - 1]) + 1;
  ಅಕ್ಷರ *ಕೊನೆ = (ಅಕ್ಷರ *) getauxval (AT_EXECFN);

  ಆದರೆ (ಕೊನೆ > ಆರಂಭ)
    memset (ಆರಂಭ, 0, ಕೊನೆ - ಆರಂಭ);
}

ಸ್ಥಿರ ಶೂನ್ಯ
rewrite_argv (ಅಕ್ಷರ **argv, ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಅಕ್ಷರ **ವಾದಗಳು, size_t ವಾದಗಳು_len)
{
  cleanup_free ಅಕ್ಷರ *decorated_name = ಖಾಲಿ;
  size_t ಲಭ್ಯ_len = argv[argc - 1] - argv[0] + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (argv[argc - 1]) + 1;
  cleanup_free ಅಕ್ಷರ *new_argv = ಖಾಲಿ;
  size_t i, so_far = 0, needed = 0;

  needed = xasprintf (&decorated_name, "[libcrun:%s]", name) + 1;
  ಗಾಗಿ (i = 0; i < ವಾದಗಳು_len; i++)
    needed += ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ವಾದಗಳು[i]) + 1;

  new_argv = xmalloc0 (needed + 1);
  strcpy (new_argv, decorated_name);
  so_far = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (decorated_name) + 1;

  ಆದರೆ (ಲಭ್ಯ_len >= needed)
    {
      ಗಾಗಿ (i = 0; i < ವಾದಗಳು_len; i++)
        {
          strcpy (new_argv + so_far, ವಾದಗಳು[i]);
          so_far += ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ವಾದಗಳು[i]) + 1;
        }
    }

  ಆದರೆ (so_far >= ಲಭ್ಯ_len)
    so_far = ಲಭ್ಯ_len - 1;

  memset (argv[0], 0, ಲಭ್ಯ_len);
  ಸ್ಮರಣೆ_ನಕಲು (argv[0], new_argv, so_far);
}

/* Entrypoint to the container.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
container_init (ಶೂನ್ಯ *ವಾದಗಳು, ಅಕ್ಷರ *notify_socket, ಪೂರ್ಣಾಂಕ sync_socket, libcrun_error_t *err)
{
  ರಚನೆ container_entrypoint_s *entrypoint_ವಾದಗಳು = ವಾದಗಳು;
  ಪೂರ್ಣಾಂಕ ret;
  runtime_spec_schema_config_schema *def = entrypoint_ವಾದಗಳು->container->container_def;
  cleanup_free ಅಕ್ಷರ *exec_ಮಾರ್ಗ = ಖಾಲಿ;
  __attribute__ ((ಬಳಸದ)) cleanup_free ಅಕ್ಷರ *notify_socket_cleanup = notify_socket;
  pid_t own_pid = 0;

  entrypoint_ವಾದಗಳು->sync_socket = sync_socket;

  crun_ಗಣ_output_handler (log_write_ಗಮ್ಯ_sync_socket, ವಾದಗಳು);

  /* sync receive own pid.  */
  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (sync_socket, &own_pid, ಗಾತ್ರ (own_pid)));
  ಆದರೆ (UNLIKELY (ret != ಗಾತ್ರ (own_pid)))
    {
      ಆದರೆ (ret >= 0)
        errno = 0;
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "read from sync socket");
    }

  ret = container_init_ಸಿದ್ಧತೆ (ವಾದಗಳು, &own_pid, notify_socket, sync_socket, &exec_ಮಾರ್ಗ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      /* If it fails to write the error using the sync socket, then fallback
         to stderr.  */
      ಆದರೆ (sync_socket_write_error (sync_socket, err) == 0)
        {
          crun_error_release (err);
          /* A return value of > 0 means to fail with an error without err being set so to avoid
             printing it twice.  */
          ಹಿಂತಿರುಗಿ 1;
        }
      ಹಿಂತಿರುಗಿ ret;
    }

  entrypoint_ವಾದಗಳು->sync_socket = -1;

  ret = unblock_ಸಂಕೇತಗಳು (err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  /* sync 4.  */
  ret = sync_socket_send_sync (sync_socket, false, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  close_and_reset (&sync_socket);

  ಆದರೆ (entrypoint_ವಾದಗಳು->context->fifo_exec_wait_fd >= 0)
    {
      ಅಕ್ಷರ ಬಫರ್[1];
      fd_set read_ಗಣ;
      cleanup_close ಪೂರ್ಣಾಂಕ fd = entrypoint_ವಾದಗಳು->context->fifo_exec_wait_fd;
      entrypoint_ವಾದಗಳು->context->fifo_exec_wait_fd = -1;

      FD_ZERO (&read_ಗಣ);
      FD_SET (fd, &read_ಗಣ);
      ಮಾಡು
        {
          ret = select (fd + 1, &read_ಗಣ, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "select");

          ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (fd, ಬಫರ್, ಗಾತ್ರ (ಬಫರ್)));
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "read from the exec fifo");
      } ಆಗ (ret == 0);

      close_and_reset (&entrypoint_ವಾದಗಳು->context->fifo_exec_wait_fd);
    }

  crun_ಗಣ_output_handler (log_write_ಗಮ್ಯ_stderr, ಖಾಲಿ);

  ಆದರೆ (def->process && def->process->no_new_privileges)
    {
      ಅಕ್ಷರ **seccomp_flags = ಖಾಲಿ;
      size_t seccomp_flags_len = 0;
      cleanup_free ಅಕ್ಷರ *seccomp_fd_payload = ಖಾಲಿ;
      size_t seccomp_fd_payload_len = 0;

      ಆದರೆ (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      ಆದರೆ (entrypoint_ವಾದಗಳು->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (entrypoint_ವಾದಗಳು->container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_ವಾದಗಳು->seccomp_fd, entrypoint_ವಾದಗಳು->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags,
                                   seccomp_flags_len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
      close_and_reset (&entrypoint_ವಾದಗಳು->seccomp_fd);
      close_and_reset (&entrypoint_ವಾದಗಳು->seccomp_receiver_fd);
    }

  ಆದರೆ (UNLIKELY (def->process == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "block `process` not found");

  ಆದರೆ (UNLIKELY (exec_ಮಾರ್ಗ == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "executable path not specified");

  ಆದರೆ (def->hooks && def->hooks->ಆರಂಭ_container_len)
    {
      libcrun_container_t *container = entrypoint_ವಾದಗಳು->container;
      libcrun_error_t tmp_err = ಖಾಲಿ;
      ಪೂರ್ಣಾಂಕ ಒಳ_userns = check_running_ಒಳ_user_namespace (&tmp_err);
      ಆದರೆ (tmp_err)
        crun_error_release (&tmp_err);

      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_ವಾದಗಳು->bundle, ಖಾಲಿ, "created", "startContainer",
                      (hook **) def->hooks->ಆರಂಭ_container,
                      def->hooks->ಆರಂಭ_container_len, entrypoint_ವಾದಗಳು->hooks_ಹೊರಹೋಗು_fd, entrypoint_ವಾದಗಳು->hooks_err_fd,
                      ಒಳ_userns > 0, err);
      ಆದರೆ (UNLIKELY (ret != 0))
        ಹಿಂತಿರುಗಿ ret;

      /* Seek stdout/stderr to the end.  If the hooks were using the same files,
         the container process overwrites what was previously written.  */
      (ಶೂನ್ಯ) lseek (1, 0, SEEK_END);
      (ಶೂನ್ಯ) lseek (2, 0, SEEK_END);
    }

  ಆದರೆ (entrypoint_ವಾದಗಳು->custom_handler)
    {
      /* Files marked with O_CLOEXEC are closed at execv time, so make sure they are closed now.
         This is a best effort operation, because the seccomp filter is already in place and it could
         stop some syscalls used by mark_or_close_fds_ge_than.
      */
      ಆದರೆ (entrypoint_ವಾದಗಳು->custom_handler->vtable->close_fds)
        {
          ret = entrypoint_ವಾದಗಳು->custom_handler->vtable->close_fds (entrypoint_ವಾದಗಳು->custom_handler->cookie,
                                                                    entrypoint_ವಾದಗಳು->container,
                                                                    entrypoint_ವಾದಗಳು->context->preserve_fds,
                                                                    err);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ret = mark_or_close_fds_ge_than (entrypoint_ವಾದಗಳು->container, entrypoint_ವಾದಗಳು->context->preserve_fds + 3, true, err);
        }
      ಆದರೆ (UNLIKELY (ret < 0))
        crun_error_release (err);

      prctl (PR_ಗಣ_NAME, entrypoint_ವಾದಗಳು->custom_handler->vtable->name);

      ಆದರೆ (entrypoint_ವಾದಗಳು->context->argv)
        {
          erase_initial_environment (entrypoint_ವಾದಗಳು->context->argv, entrypoint_ವಾದಗಳು->context->argc);
          rewrite_argv (entrypoint_ವಾದಗಳು->context->argv, entrypoint_ವಾದಗಳು->context->argc,
                        entrypoint_ವಾದಗಳು->custom_handler->vtable->name, def->process->ವಾದಗಳು,
                        def->process->ವಾದಗಳು_len);

          /* It is a quite destructive operation as we might be referencing data from the old
             argv, so make sure the context is not reused.  */
          entrypoint_ವಾದಗಳು->context->argv = ಖಾಲಿ;
          entrypoint_ವಾದಗಳು->context = ಖಾಲಿ;
        }

      ret = libcrun_ಗಣ_selinux_label (entrypoint_ವಾದಗಳು->container, def->process, true, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = libcrun_ಗಣ_apparmor_profile (entrypoint_ವಾದಗಳು->container, def->process, true, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = entrypoint_ವಾದಗಳು->custom_handler->vtable->ಸತತ_func (entrypoint_ವಾದಗಳು->custom_handler->cookie,
                                                               entrypoint_ವಾದಗಳು->container,
                                                               exec_ಮಾರ್ಗ,
                                                               def->process->ವಾದಗಳು);
      ಆದರೆ (ret != 0)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "exec container process failed with handler as `%s`", entrypoint_ವಾದಗಳು->custom_handler->vtable->name);

      ಹಿಂತಿರುಗಿ ret;
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation since the seccomp profile is already in place now and might block
     some of the syscalls needed by mark_or_close_fds_ge_than.  */
  ret = mark_or_close_fds_ge_than (entrypoint_ವಾದಗಳು->container, entrypoint_ವಾದಗಳು->context->preserve_fds + 3, true, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    crun_error_release (err);

  ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (execv (exec_ಮಾರ್ಗ, def->process->ವಾದಗಳು));

  ಆದರೆ (errno == ENOENT)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "exec container process (missing dynamic library?) `%s`", exec_ಮಾರ್ಗ);

  ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "exec container process `%s`", exec_ಮಾರ್ಗ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
read_container_config_from_state (libcrun_container_t **container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id,
                                  libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *config_file = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *dir = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;

  *container = ಖಾಲಿ;

  ret = libcrun_get_state_directory (&dir, state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = append_paths (&config_file, err, dir, "config.json", ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  *container = libcrun_container_ತುಂಬು_from_file (config_file, err);
  ಆದರೆ (*container == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ -1;

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಸತತ_poststop_hooks (arg_ಬಳಸದ libcrun_context_t *context, libcrun_container_t *container,
                    runtime_spec_schema_config_schema *def,
                    libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container_cleanup = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (def == ಖಾಲಿ)
    {
      ಆದರೆ (container == ಖಾಲಿ)
        {
          ret = read_container_config_from_state (&container_cleanup, state_ಬೇರು, id, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
          container = container_cleanup;
        }

      def = container->container_def;
    }

  ಆದರೆ (def->hooks && def->hooks->poststop_len)
    {
      cleanup_close ಪೂರ್ಣಾಂಕ hooks_ಹೊರಹೋಗು_fd = -1;
      cleanup_close ಪೂರ್ಣಾಂಕ hooks_err_fd = -1;

      ಆದರೆ (container == ಖಾಲಿ)
        {
          ret = read_container_config_from_state (&container_cleanup, state_ಬೇರು, id, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
          container = container_cleanup;
        }

      ret = open_hooks_output (container, &hooks_ಹೊರಹೋಗು_fd, &hooks_err_fd, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = do_hooks (def, 0, id, true, ಸ್ಥಿತಿ_2->bundle, ಸ್ಥಿತಿ_2->bundle, "stopped", "poststop", (hook **) def->hooks->poststop,
                      def->hooks->poststop_len, hooks_ಹೊರಹೋಗು_fd, hooks_err_fd, false, err);
      ಆದರೆ (UNLIKELY (ret != 0))
        {
          ಆದರೆ (ret < 0)
            libcrun_error_report_and_release (err);
          ಇಲ್ಲದಿದ್ದರೆ
            libcrun_error (0, "poststop hook failed with exit code: %d", ret);
        }
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ bool
has_new_pid_namespace (runtime_spec_schema_config_schema *def)
{
  size_t i;

  ಆದರೆ (def->linux == ಖಾಲಿ || def->linux->namespaces == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ false;

  ಗಾಗಿ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ಆದರೆ (strcmp (def->linux->namespaces[i]->type, "pid") == 0 && def->linux->namespaces[i]->ಮಾರ್ಗ == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ true;
    }
  ಹಿಂತಿರುಗಿ false;
}

/* Delete the container state.  CONTAINER is the container the caller already has
   available, or NULL.  When it is not NULL, the poststop hooks are run even if the
   status file was never written, e.g. when the container creation failed.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
container_delete_ಆಂತರಿಕ (libcrun_context_t *context, libcrun_container_t *container,
                           runtime_spec_schema_config_schema *def, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, bool force, bool killall,
                           libcrun_error_t *err)
{
  cleanup_cgroup_ಸ್ಥಿತಿ ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ = ಖಾಲಿ;
  cleanup_container_ಸ್ಥಿತಿ libcrun_container_ಸ್ಥಿತಿ_t ಸ್ಥಿತಿ_2 = {};
  cleanup_container libcrun_container_t *container_cleanup = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು = context->state_ಬೇರು;
  ಪೂರ್ಣಾಂಕ ret;

  ret = libcrun_read_container_ಸ್ಥಿತಿ (&ಸ್ಥಿತಿ_2, state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಆದರೆ (force && crun_error_get_errno (err) == ENOENT)
        {
          libcrun_error_t tmp_err = ಖಾಲಿ;

          crun_error_release (err);

          ಆದರೆ (container)
            {
              libcrun_container_ಸ್ಥಿತಿ_t partial_ಸ್ಥಿತಿ = {
                .bundle = (ಅಕ್ಷರ *) context->bundle,
              };

              ret = ಸತತ_poststop_hooks (context, container, def, &partial_ಸ್ಥಿತಿ, state_ಬೇರು, id, err);
              ಆದರೆ (UNLIKELY (ret < 0))
                crun_error_write_warning_and_release (context->output_handler_arg, &err);
            }

          libcrun_container_delete_ಸ್ಥಿತಿ (state_ಬೇರು, id, &tmp_err);
          crun_error_release (&tmp_err);
          ಹಿಂತಿರುಗಿ 0;
        }
      crun_error_release (err);
      ಹಿಂತಿರುಗಿ libcrun_container_delete_ಸ್ಥಿತಿ (state_ಬೇರು, id, err);
    }

  cgroup_ಸ್ಥಿತಿ = libcrun_cgroup_make_ಸ್ಥಿತಿ (&ಸ್ಥಿತಿ_2);

  ಆದರೆ (! force)
    {
      ret = libcrun_is_container_running (&ಸ್ಥಿತಿ_2, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
      ಆದರೆ (ret == 1)
        {
          ret = libcrun_ಸ್ಥಿತಿ_has_read_exec_fifo (state_ಬೇರು, id, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
          ಆದರೆ (ret == 0)
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "the container `%s` is not in `created` or `stopped` state", id);

          /* If the container is in "created" state, then do the equivalent of delete --force.  */
          killall = force = true;
        }
    }

  ಆದರೆ (def == ಖಾಲಿ)
    {
      ಆದರೆ (container == ಖಾಲಿ)
        {
          ret = read_container_config_from_state (&container_cleanup, state_ಬೇರು, id, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;

          container = container_cleanup;
        }

      def = container->container_def;
    }

  ಆದರೆ (killall && force)
    {
      /* If the container has a pid namespace, it is enough to kill the first
         process (pid=1 in the namespace).
      */
      ಆದರೆ (has_new_pid_namespace (def))
        {
          ret = libcrun_kill_linux (&ಸ್ಥಿತಿ_2, SIGKILL, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            {
              ಆದರೆ (crun_error_get_errno (err) != ESRCH)
                ಹಿಂತಿರುಗಿ ret;

              crun_error_release (err);
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಸ್ಥಿತಿ_2.cgroup_ಮಾರ್ಗ)
        {
          ret = libcrun_cgroup_killall (cgroup_ಸ್ಥಿತಿ, SIGKILL, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }
    }

  ಆದರೆ (def->linux && def->linux->intel_rdt)
    {
      ret = libcrun_destroy_intelrdt (id, def, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        libcrun_error_report_and_release (err);
    }

  ಆದರೆ (ಸ್ಥಿತಿ_2.cgroup_ಮಾರ್ಗ)
    {
      /* If the cgroup cannot be destroyed, there are still processes in it.
         Keep the container status, it is the only record of the cgroup, so
         that the delete can be retried.  */
      ret = libcrun_cgroup_destroy (cgroup_ಸ್ಥಿತಿ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ret = ಸತತ_poststop_hooks (context, container, def, &ಸ್ಥಿತಿ_2, state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ಹಿಂತಿರುಗಿ libcrun_container_delete_ಸ್ಥಿತಿ (state_ಬೇರು, id, err);
}

ಪೂರ್ಣಾಂಕ
libcrun_container_delete (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, bool force, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ container_delete_ಆಂತರಿಕ (context, ಖಾಲಿ, ಖಾಲಿ, id, force, true, err);
}

ಪೂರ್ಣಾಂಕ
libcrun_container_kill (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂಕೇತ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ sig, ret;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು = context->state_ಬೇರು;
  cleanup_container_ಸ್ಥಿತಿ libcrun_container_ಸ್ಥಿತಿ_t ಸ್ಥಿತಿ_2 = {};

  sig = str2sig (ಸಂಕೇತ);
  ಆದರೆ (UNLIKELY (sig < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "unknown signal `%s`", ಸಂಕೇತ);

  ret = libcrun_read_container_ಸ್ಥಿತಿ (&ಸ್ಥಿತಿ_2, state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ libcrun_kill_linux (&ಸ್ಥಿತಿ_2, sig, err);
}

ಪೂರ್ಣಾಂಕ
libcrun_container_killall (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂಕೇತ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ sig, ret;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು = context->state_ಬೇರು;
  cleanup_container_ಸ್ಥಿತಿ libcrun_container_ಸ್ಥಿತಿ_t ಸ್ಥಿತಿ_2 = {};
  cleanup_cgroup_ಸ್ಥಿತಿ ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ = ಖಾಲಿ;

  sig = str2sig (ಸಂಕೇತ);
  ಆದರೆ (UNLIKELY (sig < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "unknown signal `%s`", ಸಂಕೇತ);

  ret = libcrun_read_container_ಸ್ಥಿತಿ (&ಸ್ಥಿತಿ_2, state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  cgroup_ಸ್ಥಿತಿ = libcrun_cgroup_make_ಸ್ಥಿತಿ (&ಸ್ಥಿತಿ_2);

  ret = libcrun_cgroup_killall (cgroup_ಸ್ಥಿತಿ, sig, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_container_ಸ್ಥಿತಿ (libcrun_container_t *container, libcrun_context_t *context,
                        pid_t pid, ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ,
                        libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *bundle_allocated = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *bundle = get_bundle_ಮಾರ್ಗ (context->bundle, &bundle_allocated);
  cleanup_free ಅಕ್ಷರ *owner = get_user_name (geteuid ());
  ಅಕ್ಷರ *external_descriptors = libcrun_get_external_descriptors (container);
  ಅಕ್ಷರ *rootfs = container->container_def->ಬೇರು ? container->container_def->ಬೇರು->ಮಾರ್ಗ : "";
  ಅಕ್ಷರ created[35];

  libcrun_container_ಸ್ಥಿತಿ_t ಸ್ಥಿತಿ_2 = {
    .pid = pid,
    .rootfs = rootfs,
    .bundle = (ಅಕ್ಷರ *) bundle,
    .created = created,
    .owner = owner,
    .systemd_cgroup = context->systemd_cgroup,
    .detached = context->detach,
    .no_new_keyring = context->no_new_keyring,
    .external_descriptors = external_descriptors,
    .cgroup_ಮಾರ್ಗ = ಖಾಲಿ,
    .scope = ಖಾಲಿ,
  };

  get_current_timestamp (created, ಗಾತ್ರ (created));

  ಆದರೆ (cgroup_ಸ್ಥಿತಿ)
    {
      ಪೂರ್ಣಾಂಕ ret;

      ret = libcrun_cgroup_get_ಸ್ಥಿತಿ (cgroup_ಸ್ಥಿತಿ, &ಸ್ಥಿತಿ_2, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (external_descriptors == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid internal state.  No external descriptors found");
  ಹಿಂತಿರುಗಿ libcrun_write_container_ಸ್ಥಿತಿ (context->state_ಬೇರು, context->id, &ಸ್ಥಿತಿ_2, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
reap_subprocesses (pid_t *main_process, ಪೂರ್ಣಾಂಕ *main_process_exit, ಪೂರ್ಣಾಂಕ *ಕೊನೆಯ_process, libcrun_error_t *err)
{
  *ಕೊನೆಯ_process = 0;
  ಆಗ (1)
    {
      ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2;
      ಪೂರ್ಣಾಂಕ r = waitpid_ignore_stopped (-1, &ಸ್ಥಿತಿ_2, WNOHANG);
      ಆದರೆ (r < 0)
        {
          ಆದರೆ (errno == EINTR)
            ಮುಂದುವರಿ;
          ಆದರೆ (errno == ECHILD)
            {
              *ಕೊನೆಯ_process = 1;
              ಹಿಂತಿರುಗಿ 0;
            }
          ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "waitpid");
        }
      ಆದರೆ (r == 0)
        ನಿಲ್ಲಿಸು;
      ಆದರೆ (r != *main_process)
        ಮುಂದುವರಿ;

      *main_process_exit = get_process_exit_ಸ್ಥಿತಿ (ಸ್ಥಿತಿ_2);

      /* Once reaped, the pid can be reused by an unrelated process.  */
      *main_process = 0;
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
send_sd_notify (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ready_str, libcrun_error_t *err)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYSTEMD
  ಪೂರ್ಣಾಂಕ ret = sd_notify (0, ready_str);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "sd_notify");

#  ಆದರೆ HAVE_SD_NOTIFY_BARRIER
  /* Hard-code a 30 seconds timeout.  Ignore errors.  */
  sd_notify_barrier (0, 30 * 1000000);
#  endif
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) ready_str;
  (ಶೂನ್ಯ) err;
#ಆದರೆ_ಅಂತ್ಯ
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
handle_notify_socket (ಪೂರ್ಣಾಂಕ notify_socketfd, libcrun_error_t *err)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYSTEMD
  ಪೂರ್ಣಾಂಕ ret;
  ಅಕ್ಷರ buf[256];
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ready_str = "READY=1";

  ret = recvfrom (notify_socketfd, buf, ಗಾತ್ರ (buf) - 1, 0, ಖಾಲಿ, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "recvfrom notify socket");

  buf[ret] = '\0';
  ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು (buf, ready_str))
    {
      ret = send_sd_notify (ready_str, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ಹಿಂತಿರುಗಿ 1;
    }
  ಹಿಂತಿರುಗಿ 0;
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) notify_socketfd;
  (ಶೂನ್ಯ) err;
  ಹಿಂತಿರುಗಿ 1;
#ಆದರೆ_ಅಂತ್ಯ
}

ರಚನೆ wait_for_process_ವಾದಗಳು
{
  pid_t pid;
  libcrun_context_t *context;
  ಪೂರ್ಣಾಂಕ terminal_fd;
  ಪೂರ್ಣಾಂಕ notify_socket;
  ಪೂರ್ಣಾಂಕ *container_ready_fd;
  ಪೂರ್ಣಾಂಕ seccomp_notify_fd;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *seccomp_notify_plugins;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_pid_file (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pid_file, pid_t pid, libcrun_error_t *err)
{
  ಅಕ್ಷರ buf[32];
  ಪೂರ್ಣಾಂಕ buf_len = snprintf (buf, ಗಾತ್ರ (buf), "%d", pid);
  ಆದರೆ (UNLIKELY (buf_len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (buf)))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");
  ಹಿಂತಿರುಗಿ write_file_at_with_flags (AT_FDCWD, O_CREAT | O_TRUNC, 0700, pid_file, buf, buf_len, err);
}

#ವ್ಯಾಖ್ಯಾನಿಸು MAX_EVENTS 10

ಸ್ಥಿರ bool
is_fd_pollable (ಪೂರ್ಣಾಂಕ fd)
{
  ರಚನೆ epoll_event ev = { .events = EPOLLIN };
  cleanup_close ಪೂರ್ಣಾಂಕ epollfd = epoll_create1 (EPOLL_CLOEXEC);

  ಆದರೆ (epollfd < 0)
    ಹಿಂತಿರುಗಿ true;

  ಹಿಂತಿರುಗಿ epoll_ctl (epollfd, EPOLL_CTL_ಕೂಡಿಸು, fd, &ev) == 0 || errno != EPERM;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
wait_for_process (ರಚನೆ wait_for_process_ವಾದಗಳು *ವಾದಗಳು, libcrun_error_t *err)
{
  cleanup_channel_fd_pair ರಚನೆ channel_fd_pair *from_terminal = ಖಾಲಿ;
  cleanup_channel_fd_pair ರಚನೆ channel_fd_pair *ಗಮ್ಯ_terminal = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret, container_exit_ಸಂಕೇತ = 0, ಕೊನೆಯ_process;
  cleanup_close ಪೂರ್ಣಾಂಕ terminal_fd_from = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ terminal_fd_ಗಮ್ಯ = -1;
  bool stdin_pollable = false, stdout_pollable = false;
  cleanup_close ಪೂರ್ಣಾಂಕ epollfd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ signalfd = -1;
  sigset_t ಮುಖವಾಡ;
  ಪೂರ್ಣಾಂಕ ಒಳ_fds[MAX_EVENTS];
  ಪೂರ್ಣಾಂಕ ಒಳ_fds_len = 0;
  ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_fds[MAX_EVENTS];
  ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_fds_len = 0;
  size_t i;

  cleanup_seccomp_notify_context ರಚನೆ seccomp_notify_context_s *seccomp_notify_ctx = ಖಾಲಿ;

  container_exit_ಸಂಕೇತ = 0;

  ಆದರೆ (ವಾದಗಳು == ಖಾಲಿ || ವಾದಗಳು->context == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: context is empty");

  ಗಾಗಿ (i = 0; i < MAX_EVENTS; i++)
    {
      ಒಳ_fds[i] = -1;
      ಹೊರಹೋಗು_fds[i] = -1;
    }

  ಆದರೆ (ವಾದಗಳು->context->pid_file)
    {
      ret = write_pid_file (ವಾದಗಳು->context->pid_file, ವಾದಗಳು->pid, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  /* Also exit if there is nothing more to wait for.  */
  ಆದರೆ (ವಾದಗಳು->context->detach && ವಾದಗಳು->notify_socket < 0)
    ಹಿಂತಿರುಗಿ 0;

  ಆದರೆ (ವಾದಗಳು->container_ready_fd)
    {
      ret = 0;
      ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (*ವಾದಗಳು->container_ready_fd, &ret, ಗಾತ್ರ (ret)));
      close_and_reset (ವಾದಗಳು->container_ready_fd);
    }

  sigfillset (&ಮುಖವಾಡ);
  ret = sigprocmask (SIG_ಖಂಡ, &ಮುಖವಾಡ, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "sigprocmask");

  signalfd = create_signalfd (&ಮುಖವಾಡ, err);
  ಆದರೆ (UNLIKELY (signalfd < 0))
    ಹಿಂತಿರುಗಿ signalfd;

  ret = reap_subprocesses (&ವಾದಗಳು->pid, &container_exit_ಸಂಕೇತ, &ಕೊನೆಯ_process, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (ಕೊನೆಯ_process)
    ಹಿಂತಿರುಗಿ container_exit_ಸಂಕೇತ;

  ಆದರೆ (ವಾದಗಳು->seccomp_notify_fd >= 0)
    {
      cleanup_free ಅಕ್ಷರ *state_ಬೇರು = ಖಾಲಿ;
      cleanup_free ಅಕ್ಷರ *oci_config_ಮಾರ್ಗ = ಖಾಲಿ;

      ರಚನೆ libcrun_ತುಂಬು_seccomp_notify_conf_s conf;
      memset (&conf, 0, ಗಾತ್ರ conf);

      ret = libcrun_get_state_directory (&state_ಬೇರು, ವಾದಗಳು->context->state_ಬೇರು, ವಾದಗಳು->context->id, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = append_paths (&oci_config_ಮಾರ್ಗ, err, state_ಬೇರು, "config.json", ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      conf.runtime_ಬೇರು_ಮಾರ್ಗ = state_ಬೇರು;
      conf.name = ವಾದಗಳು->context->id;
      conf.bundle_ಮಾರ್ಗ = ವಾದಗಳು->context->bundle;
      conf.oci_config_ಮಾರ್ಗ = oci_config_ಮಾರ್ಗ;

      ret = ಗಣ_blocking_fd (ವಾದಗಳು->seccomp_notify_fd, false, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = libcrun_ತುಂಬು_seccomp_notify_plugins (&seccomp_notify_ctx,
                                                 ವಾದಗಳು->seccomp_notify_plugins,
                                                 &conf, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ಒಳ_fds[ಒಳ_fds_len++] = ವಾದಗಳು->seccomp_notify_fd;
    }

  ಆದರೆ (ವಾದಗಳು->terminal_fd >= 0)
    {
      /* The terminal_fd is dup()ed so that it can be registered with
         epoll multiple times using different masks.  */
      terminal_fd_from = dup (ವಾದಗಳು->terminal_fd);
      ಆದರೆ (UNLIKELY (terminal_fd_from < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "dup terminal fd");
      terminal_fd_ಗಮ್ಯ = dup (ವಾದಗಳು->terminal_fd);
      ಆದರೆ (UNLIKELY (terminal_fd_ಗಮ್ಯ < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "dup terminal fd");

      ಪೂರ್ಣಾಂಕ i, non_blocking_fds[] = { terminal_fd_from, terminal_fd_ಗಮ್ಯ, 0, 1, -1 };
      ಗಾಗಿ (i = 0; non_blocking_fds[i] >= 0; i++)
        {
          ret = ಗಣ_blocking_fd (non_blocking_fds[i], false, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }

      /* stdin and stdout can be something epoll does not support, such as
         /dev/null or a regular file.  Such a stdin is not forwarded, and
         such a stdout is written to without waiting for it to be ready.  */
      stdin_pollable = is_fd_pollable (0);
      stdout_pollable = is_fd_pollable (1);

      from_terminal = channel_fd_pair_new (terminal_fd_from, 1, BUFSIZ);
      ಆದರೆ (stdin_pollable)
        ಗಮ್ಯ_terminal = channel_fd_pair_new (0, terminal_fd_ಗಮ್ಯ, BUFSIZ);
    }

  ಒಳ_fds[ಒಳ_fds_len++] = signalfd;
  ಆದರೆ (ವಾದಗಳು->notify_socket >= 0)
    ಒಳ_fds[ಒಳ_fds_len++] = ವಾದಗಳು->notify_socket;
  ಆದರೆ (ವಾದಗಳು->terminal_fd >= 0)
    {
      ಆದರೆ (stdin_pollable)
        {
          ಒಳ_fds[ಒಳ_fds_len++] = 0;
          ಹೊರಹೋಗು_fds[ಹೊರಹೋಗು_fds_len++] = terminal_fd_ಗಮ್ಯ;
        }

      ಒಳ_fds[ಒಳ_fds_len++] = terminal_fd_from;
      ಆದರೆ (stdout_pollable)
        ಹೊರಹೋಗು_fds[ಹೊರಹೋಗು_fds_len++] = 1;
    }

  epollfd = epoll_helper (ಒಳ_fds, ಖಾಲಿ, ಹೊರಹೋಗು_fds, ಖಾಲಿ, err);
  ಆದರೆ (UNLIKELY (epollfd < 0))
    ಹಿಂತಿರುಗಿ epollfd;

  ಆಗ (1)
    {
      ರಚನೆ epoll_event events[MAX_EVENTS];
      ರಚನೆ signalfd_siginfo si;
      ರಚನೆ winsize ws;
      ಪೂರ್ಣಾಂಕ i, nr_events;
      ssize_t res;

      nr_events = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (epoll_wait (epollfd, events, MAX_EVENTS, -1));
      ಆದರೆ (UNLIKELY (nr_events < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "epoll_wait");

      ಗಾಗಿ (i = 0; i < nr_events; i++)
        {
          ಆದರೆ (events[i].ದತ್ತಾಂಶ.fd == 0 || events[i].ದತ್ತಾಂಶ.fd == terminal_fd_ಗಮ್ಯ)
            {
              ret = channel_fd_pair_process (ಗಮ್ಯ_terminal, epollfd, err);
              ಆದರೆ (UNLIKELY (ret < 0))
                ಹಿಂತಿರುಗಿ crun_error_wrap (err, "copy to terminal fd");
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (events[i].ದತ್ತಾಂಶ.fd == 1 || events[i].ದತ್ತಾಂಶ.fd == terminal_fd_from)
            {
              ret = channel_fd_pair_process (from_terminal, epollfd, err);
              ಆದರೆ (UNLIKELY (ret < 0))
                ಹಿಂತಿರುಗಿ crun_error_wrap (err, "copy from terminal fd");
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (events[i].ದತ್ತಾಂಶ.fd == ವಾದಗಳು->seccomp_notify_fd)
            {
              ret = libcrun_seccomp_notify_plugins (seccomp_notify_ctx,
                                                    ವಾದಗಳು->seccomp_notify_fd, err);
              ಆದರೆ (UNLIKELY (ret < 0))
                ಹಿಂತಿರುಗಿ ret;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (events[i].ದತ್ತಾಂಶ.fd == ವಾದಗಳು->notify_socket)
            {
              ret = handle_notify_socket (ವಾದಗಳು->notify_socket, err);
              ಆದರೆ (UNLIKELY (ret < 0))
                ಹಿಂತಿರುಗಿ ret;
              ಆದರೆ (ret && ವಾದಗಳು->context->detach)
                ಹಿಂತಿರುಗಿ 0;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (events[i].ದತ್ತಾಂಶ.fd == signalfd)
            {
              res = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (signalfd, &si, ಗಾತ್ರ (si)));
              ಆದರೆ (UNLIKELY (res < 0))
                ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "read from signalfd");
              ಆದರೆ (si.ssi_signo == SIGCHLD)
                {
                  ret = reap_subprocesses (&ವಾದಗಳು->pid, &container_exit_ಸಂಕೇತ,
                                           &ಕೊನೆಯ_process, err);
                  ಆದರೆ (UNLIKELY (ret < 0))
                    ಹಿಂತಿರುಗಿ ret;
                  ಆದರೆ (ಕೊನೆಯ_process)
                    ಹಿಂತಿರುಗಿ container_exit_ಸಂಕೇತ;
                }
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (si.ssi_signo == SIGWINCH)
                {
                  /* Ignore the signal if the terminal is not available.  */
                  ಆದರೆ (ವಾದಗಳು->terminal_fd > 0)
                    {
                      ret = ioctl (0, TIOCGWINSZ, &ws);
                      ಆದರೆ (UNLIKELY (ret < 0))
                        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "ioctl TIOCGWINSZ copy terminal size from stdin");

                      ret = ioctl (ವಾದಗಳು->terminal_fd, TIOCSWINSZ, &ws);
                      ಆದರೆ (UNLIKELY (ret < 0))
                        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "ioctl TIOCSWINSZ copy terminal size to pty");
                    }
                }
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ವಾದಗಳು->pid > 0)
                {
                  /* Send any other signal to the child process, if it was not reaped yet.  */
                  ret = kill (ವಾದಗಳು->pid, si.ssi_signo);
                }
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: unknown fd from epoll_wait");
            }
        }
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ
flush_fd_ಗಮ್ಯ_err (libcrun_context_t *context, ಪೂರ್ಣಾಂಕ terminal_fd)
{
  ಅಕ್ಷರ buf[256];
  ಪೂರ್ಣಾಂಕ flags;

  ಆದರೆ (terminal_fd < 0 || stderr == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;

  flags = fcntl (terminal_fd, F_GETFL, 0);
  ಆದರೆ (flags == -1)
    ಹಿಂತಿರುಗಿ;
  ಆದರೆ (fcntl (terminal_fd, F_SETFL, flags | O_NONBLOCK) < 0)
    ಹಿಂತಿರುಗಿ;

  ಗಾಗಿ (;;)
    {
      ಪೂರ್ಣಾಂಕ ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (terminal_fd, buf, ಗಾತ್ರ (buf) - 1));
      ಆದರೆ (ret <= 0)
        ನಿಲ್ಲಿಸು;
      buf[ret] = '\0';
      ಆದರೆ (context->output_handler)
        context->output_handler (0, buf, LIBCRUN_VERBOSITY_ERROR, context->output_handler_arg);
    }
  (ಶೂನ್ಯ) fcntl (terminal_fd, F_SETFL, flags);
  fflush (stderr);
  fsync (1);
  fsync (2);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
cleanup_watch (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
               ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ, pid_t init_pid, ಪೂರ್ಣಾಂಕ sync_socket,
               ಪೂರ್ಣಾಂಕ terminal_fd, libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *oom_ಸಂದೇಶ = ಖಾಲಿ;
  libcrun_error_t tmp_err = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (init_pid)
    {
      /* Try to detect whether the cgroup has a OOM.  */
      ಆದರೆ (cgroup_ಸ್ಥಿತಿ)
        {
          ಪೂರ್ಣಾಂಕ has_oom;

          has_oom = libcrun_cgroup_has_oom (cgroup_ಸ್ಥಿತಿ, &tmp_err);
          ಆದರೆ (has_oom > 0)
            oom_ಸಂದೇಶ = "OOM: the memory limit could be too low";
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (has_oom < 0)
            {
              /* If the detection has failed for any reason, e.g. the cgroup was
                 already deleted by the time it was checked, just ignore the
                 failure.  */
              crun_error_release (&tmp_err);
            }
        }
      /* If the OOM wasn't detected, look into the static configuration.  */
      ಆದರೆ (oom_ಸಂದೇಶ == ಖಾಲಿ && is_ಸ್ಮರಣೆ_ಎಲ್ಲೆ_too_ಕೆಳಗಿನ (def))
        oom_ಸಂದೇಶ = "the memory limit could be too low";

      kill (init_pid, SIGKILL);
      waitpid_ignore_stopped (init_pid, ಖಾಲಿ, 0);
    }

  ret = sync_socket_wait_sync (context, sync_socket, true, &tmp_err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      crun_error_release (err);
      *err = tmp_err;
    }

  ಆದರೆ (terminal_fd >= 0)
    flush_fd_ಗಮ್ಯ_err (context, terminal_fd);

  ಆದರೆ (oom_ಸಂದೇಶ)
    ಹಿಂತಿರುಗಿ crun_error_wrap (err, "%s", oom_ಸಂದೇಶ);

  ಹಿಂತಿರುಗಿ -1;
}

/* Find the uid:gid that is mapped to root inside the container user namespace.  */
ಶೂನ್ಯ
get_ಬೇರು_ಒಳ_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid, uid_t *uid,
                        gid_t *gid)
{
  *uid = -1;
  *gid = -1;

  /* Not root in the namespace.  */
  ಆದರೆ (host_uid)
    ಹಿಂತಿರುಗಿ;

  ಆದರೆ (def->linux && def->linux->uid_mappings)
    {
      size_t i;

      ಗಾಗಿ (i = 0; i < def->linux->uid_mappings_len; i++)
        ಆದರೆ (def->linux->uid_mappings[i]->container_id == 0)
          {
            *uid = def->linux->uid_mappings[i]->host_id;
            ನಿಲ್ಲಿಸು;
          }
    }
  ಆದರೆ (def->linux && def->linux->gid_mappings)
    {
      size_t i;

      ಗಾಗಿ (i = 0; i < def->linux->gid_mappings_len; i++)
        ಆದರೆ (def->linux->gid_mappings[i]->container_id == 0)
          {
            *gid = def->linux->gid_mappings[i]->host_id;
            ನಿಲ್ಲಿಸು;
          }
    }

  /* If the uid and the gid are not changed, do not attempt any chown.  */
  ಆದರೆ (*uid == host_uid && *gid == host_gid)
    *uid = *gid = -1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_seccomp_receiver_fd (libcrun_container_t *container, ಪೂರ್ಣಾಂಕ *fd, ಪೂರ್ಣಾಂಕ *self_receiver_fd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **plugins,
                         libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *tmp;
  runtime_spec_schema_config_schema *def = container->container_def;

  *fd = -1;
  *self_receiver_fd = -1;

  tmp = ಕಂಡುಹಿಡಿ_annotation (container, "run.oci.seccomp.plugins");
  ಆದರೆ (tmp)
    {
      ಪೂರ್ಣಾಂಕ fds[2];
      ಪೂರ್ಣಾಂಕ ret;

      ret = create_socket_pair (fds, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_error_wrap (err, "create socket pair");

      *fd = fds[0];
      *self_receiver_fd = fds[1];
      *plugins = tmp;
      ಹಿಂತಿರುಗಿ 0;
    }

  ಆದರೆ (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_ಮಾರ್ಗ)
    tmp = def->linux->seccomp->listener_ಮಾರ್ಗ;
  ಇಲ್ಲದಿದ್ದರೆ
    tmp = ಕಂಡುಹಿಡಿ_annotation (container, "run.oci.seccomp.receiver");
  ಆದರೆ (tmp == ಖಾಲಿ)
    tmp = getenv ("RUN_OCI_SECCOMP_RECEIVER");
  ಆದರೆ (tmp)
    {
      ಆದರೆ (tmp[0] != '/')
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "the seccomp receiver `%s` is not an absolute path", tmp);

      *fd = open_unix_domain_client_socket (tmp, 0, err);
      ಆದರೆ (UNLIKELY (*fd < 0))
        ಹಿಂತಿರುಗಿ crun_error_wrap (err, "open seccomp receiver");
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ bool
has_seccomp_receiver (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;

  ಆದರೆ (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_ಮಾರ್ಗ)
    ಹಿಂತಿರುಗಿ true;

  ಆದರೆ (ಕಂಡುಹಿಡಿ_annotation (container, "run.oci.seccomp.receiver") != ಖಾಲಿ || getenv ("RUN_OCI_SECCOMP_RECEIVER") != ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ true;

  ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಸಿದ್ಧತೆ_container_hooks_output (libcrun_container_t *container, runtime_spec_schema_config_schema *def,
                              ರಚನೆ container_entrypoint_s *container_ವಾದಗಳು, ಪೂರ್ಣಾಂಕ *hooks_ಹೊರಹೋಗು_fd,
                              ಪೂರ್ಣಾಂಕ *hooks_err_fd, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (def->hooks
      && (def->hooks->prestart_len || def->hooks->poststart_len || def->hooks->create_runtime_len
          || def->hooks->create_container_len || def->hooks->ಆರಂಭ_container_len))
    {
      ret = open_hooks_output (container, hooks_ಹೊರಹೋಗು_fd, hooks_err_fd, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
      container_ವಾದಗಳು->hooks_ಹೊರಹೋಗು_fd = *hooks_ಹೊರಹೋಗು_fd;
      container_ವಾದಗಳು->hooks_err_fd = *hooks_err_fd;
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಸಿದ್ಧತೆ_container_keyring (libcrun_container_t *container, libcrun_context_t *context, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (! context->no_new_keyring)
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *label = ಖಾಲಿ;

      libcrun_debug ("Creating new keyring");

      ಆದರೆ (def->process)
        {
          label = def->process->selinux_label;
          ಆದರೆ (label)
            libcrun_debug ("Using SELinux process label: `%s`", label);
        }

      ret = libcrun_create_keyring (container, container->context->id, label, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಸಿದ್ಧತೆ_terminal_socket_pair (libcrun_container_t *container, libcrun_context_t *context,
                            ರಚನೆ container_entrypoint_s *container_ವಾದಗಳು, ಪೂರ್ಣಾಂಕ *socket_pair_0,
                            ಪೂರ್ಣಾಂಕ *socket_pair_1, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಪೂರ್ಣಾಂಕ ret;
  ಪೂರ್ಣಾಂಕ detach = context->detach;

  ಆದರೆ (def->process && def->process->terminal && ! detach && context->console_socket == ಖಾಲಿ)
    {
      libcrun_debug ("Creating terminal socket pair");
      container_ವಾದಗಳು->has_terminal_socket_pair = 1;
      ret = create_socket_pair (container_ವಾದಗಳು->terminal_socketpair, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_error_wrap (err, "create terminal socket");

      *socket_pair_0 = container_ವಾದಗಳು->terminal_socketpair[0];
      *socket_pair_1 = container_ವಾದಗಳು->terminal_socketpair[1];
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಸಿದ್ಧತೆ_seccomp (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *seccomp_bpf_ದತ್ತಾಂಶ,
               ರಚನೆ libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, ಪೂರ್ಣಾಂಕ *seccomp_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (ಕಂಡುಹಿಡಿ_annotation (container, "run.oci.seccomp.plugins") != ಖಾಲಿ && has_seccomp_receiver (container))
    {
      ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "seccomp plugins and seccomp receivers cannot be declared at the same time");
    }

  ಆದರೆ (def->linux && (def->linux->seccomp || seccomp_bpf_ದತ್ತಾಂಶ))
    {
      ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ seccomp_gen_options = 0;
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *annotation;

      libcrun_debug ("Initializing seccomp");
      annotation = ಕಂಡುಹಿಡಿ_annotation (container, "run.oci.seccomp_fail_unknown_syscall");
      ಆದರೆ (annotation && strcmp (annotation, "0") != 0)
        seccomp_gen_options = LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL;

      ಆದರೆ (seccomp_bpf_ದತ್ತಾಂಶ)
        seccomp_gen_options |= LIBCRUN_SECCOMP_SKIP_CACHE;

      libcrun_seccomp_gen_ctx_init (seccomp_gen_ctx, container, true, seccomp_gen_options);

      ret = libcrun_open_seccomp_bpf (seccomp_gen_ctx, seccomp_fd, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಸಿದ್ಧತೆ_console_socket (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
                      ರಚನೆ container_entrypoint_s *container_ವಾದಗಳು, ಪೂರ್ಣಾಂಕ *console_socket_fd,
                      libcrun_error_t *err)
{
  ಆದರೆ (context->console_socket && def->process && def->process->terminal)
    {
      *console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
      ಆದರೆ (UNLIKELY (*console_socket_fd < 0))
        ಹಿಂತಿರುಗಿ crun_error_wrap (err, "open console socket");
      container_ವಾದಗಳು->console_socket_fd = *console_socket_fd;
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಸಿದ್ಧತೆ_cgroup_manager (libcrun_context_t *context, libcrun_container_t *container,
                      ರಚನೆ libcrun_cgroup_ವಾದಗಳು *cg, ಪೂರ್ಣಾಂಕ *cgroup_dirfd,
                      ರಚನೆ libcrun_dirfd_s *cgroup_dirfd_s, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಪೂರ್ಣಾಂಕ cgroup_manager;
  uid_t ಬೇರು_uid = -1;
  gid_t ಬೇರು_gid = -1;
  ಪೂರ್ಣಾಂಕ ret;

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  ಆದರೆ (context->systemd_cgroup)
    {
      libcrun_debug ("Using systemd cgroup manager");
      cgroup_manager = CGROUP_MANAGER_SYSTEMD;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (context->force_no_cgroup)
    {
      libcrun_debug ("Disabling cgroup manager");
      cgroup_manager = CGROUP_MANAGER_DISABLED;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    libcrun_debug ("Using cgroupfs cgroup manager");

  /* If we are root (either on the host or in a namespace), then chown the cgroup to root
     in the container user namespace.  */
  get_ಬೇರು_ಒಳ_the_userns (def, container->host_uid, container->host_gid, &ಬೇರು_uid, &ಬೇರು_gid);
  libcrun_debug ("Using container host UID `%d` and GID `%d`", container->host_uid, container->host_gid);

  memset (cg, 0, ಗಾತ್ರ (*cg));

  cg->cgroup_ಮಾರ್ಗ = def->linux ? def->linux->cgroups_ಮಾರ್ಗ : "";
  cg->manager = cgroup_manager;
  cg->id = context->id;
  cg->resources = def->linux ? def->linux->resources : ಖಾಲಿ;
  cg->annotations = container->annotations;
  cg->ಬೇರು_uid = ಬೇರು_uid;
  cg->ಬೇರು_gid = ಬೇರು_gid;
  cg->state_ಬೇರು = context->state_ಬೇರು;
  cg->container = container;

  ret = libcrun_cgroup_preenter (cg, cgroup_dirfd, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  cgroup_dirfd_s->dirfd = cgroup_dirfd;
  cgroup_dirfd_s->joined = false;

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಗಣ_scheduler (pid_t pid, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ret = libcrun_ಗಣ_scheduler (pid, def->process, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_reset_cpu_affinity_ಮುಖವಾಡ (pid, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_ಗಣ_io_ಆದ್ಯತೆ (pid, def->process, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
seccomp_generation (ಪೂರ್ಣಾಂಕ seccomp_fd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *seccomp_bpf_ದತ್ತಾಂಶ,
                    ರಚನೆ libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (seccomp_fd >= 0)
    {
      ಆದರೆ (seccomp_bpf_ದತ್ತಾಂಶ != ಖಾಲಿ)
        {
          ret = libcrun_copy_seccomp (seccomp_gen_ctx, seccomp_bpf_ದತ್ತಾಂಶ, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ret = libcrun_generate_seccomp (seccomp_gen_ctx, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
terminal_ಸಿದ್ಧತೆ (runtime_spec_schema_config_schema *def, libcrun_context_t *context,
                ಪೂರ್ಣಾಂಕ socket_pair_0, ಪೂರ್ಣಾಂಕ *terminal_fd, ಶೂನ್ಯ **orig_terminal, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  ಪೂರ್ಣಾಂಕ detach = context->detach;

  ಆದರೆ (def->process && def->process->terminal && ! detach && context->console_socket == ಖಾಲಿ)
    {
      libcrun_debug ("Receiving console socket fd");
      *terminal_fd = receive_fd_from_socket (socket_pair_0, err);
      ಆದರೆ (UNLIKELY (*terminal_fd < 0))
        ಹಿಂತಿರುಗಿ -1;

      ret = libcrun_clear_onlcr (*terminal_fd, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = libcrun_ಗಣ_raw (0, orig_terminal, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libcrun_container_ಸತತ_ಆಂತರಿಕ (libcrun_container_t *container, libcrun_context_t *context,
                                ಪೂರ್ಣಾಂಕ *container_ready_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಪೂರ್ಣಾಂಕ ret;
  pid_t pid;
  ಪೂರ್ಣಾಂಕ detach = context->detach;
  cleanup_cgroup_ಸ್ಥಿತಿ ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ terminal_fd = -1;
  cleanup_terminal ಶೂನ್ಯ *orig_terminal = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ sync_socket = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ notify_socket = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ socket_pair_0 = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ socket_pair_1 = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ seccomp_fd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ console_socket_fd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ hooks_ಹೊರಹೋಗು_fd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ hooks_err_fd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ own_seccomp_receiver_fd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ seccomp_notify_fd = -1;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *seccomp_notify_plugins = ಖಾಲಿ;
  cleanup_cgroup_preenter ರಚನೆ libcrun_cgroup_ವಾದಗಳು cg = {};
  cleanup_free ಅಕ್ಷರ *bundle_allocated = ಖಾಲಿ;
  ರಚನೆ container_entrypoint_s container_ವಾದಗಳು = {
    .container = container,
    .context = context,
    .bundle = get_bundle_ಮಾರ್ಗ (context->bundle, &bundle_allocated),
    .terminal_socketpair = { -1, -1 },
    .console_socket_fd = -1,
    .hooks_ಹೊರಹೋಗು_fd = -1,
    .hooks_err_fd = -1,
    .seccomp_receiver_fd = -1,
    .custom_handler = ಖಾಲಿ,
  };
  cleanup_close ಪೂರ್ಣಾಂಕ cgroup_dirfd = -1;
  ರಚನೆ libcrun_dirfd_s cgroup_dirfd_s;
  ರಚನೆ libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *seccomp_bpf_ದತ್ತಾಂಶ = ಕಂಡುಹಿಡಿ_annotation (container, "run.oci.seccomp_bpf_data");
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ;

  ಆದರೆ (! context->force_no_cgroup)
    {
      cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
      ಆದರೆ (UNLIKELY (cgroup_ಬಹುಲಕ < 0))
        ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

      ಆದರೆ (cgroup_ಬಹುಲಕ != CGROUP_ಬಹುಲಕ_UNIFIED)
        libcrun_warning ("cgroup v1 is deprecated and will be removed in a future release.  Use cgroup v2");
    }

  ret = ಸಿದ್ಧತೆ_container_hooks_output (container, def, &container_ವಾದಗಳು, &hooks_ಹೊರಹೋಗು_fd, &hooks_err_fd, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  container->context = context;

  ಆದರೆ (! detach || context->notify_socket)
    {
      libcrun_debug ("Setting child subreaper");
      ret = prctl (PR_ಗಣ_ಮಗು_SUBREAPER, 1, 0, 0, 0);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "prctl set child subreaper");
    }

  ret = ಸಿದ್ಧತೆ_container_keyring (container, context, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = ಸಿದ್ಧತೆ_terminal_socket_pair (container, context, &container_ವಾದಗಳು, &socket_pair_0, &socket_pair_1, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = ಖಂಡ_ಸಂಕೇತಗಳು (err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  umask (0);

  ret = libcrun_ಗಣ_mempolicy (def, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = ಸಿದ್ಧತೆ_seccomp (container, seccomp_bpf_ದತ್ತಾಂಶ, &seccomp_gen_ctx, &seccomp_fd, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;
  container_ವಾದಗಳು.seccomp_fd = seccomp_fd;

  ಆದರೆ (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &container_ವಾದಗಳು.seccomp_receiver_fd, &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ret = ಸಿದ್ಧತೆ_console_socket (context, def, &container_ವಾದಗಳು, &console_socket_fd, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = ಸಿದ್ಧತೆ_cgroup_manager (context, container, &cg, &cgroup_dirfd, &cgroup_dirfd_s, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_configure_handler (container_ವಾದಗಳು.context->handler_manager,
                                   container_ವಾದಗಳು.context,
                                   container,
                                   &(container_ವಾದಗಳು.custom_handler),
                                   err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (container_ವಾದಗಳು.custom_handler && container_ವಾದಗಳು.custom_handler->vtable->modify_oci_configuration)
    {
      libcrun_debug ("Using custom handler to modify OCI configuration");
      ret = container_ವಾದಗಳು.custom_handler->vtable->modify_oci_configuration (container_ವಾದಗಳು.custom_handler->cookie,
                                                                             container_ವಾದಗಳು.context,
                                                                             container->container_def,
                                                                             err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  pid = libcrun_ಸತತ_linux_container (container, container_init, &container_ವಾದಗಳು, &sync_socket, &cgroup_dirfd_s, err);
  ಆದರೆ (UNLIKELY (pid < 0))
    ಹಿಂತಿರುಗಿ pid;

  cg.pid = pid;
  cg.joined = cgroup_dirfd_s.joined;
  libcrun_debug ("Running container on PID: `%d`", pid);

  ಆದರೆ (context->fifo_exec_wait_fd < 0 && context->notify_socket)
    {
      libcrun_debug ("Using notify socket: `%s`", context->notify_socket);
      /* Do not open the notify socket here on "create".  "start" will take care of it.  */
      ret = get_notify_fd (context, container, &notify_socket, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹೋಗು fail;
    }

  ಆದರೆ (container_ವಾದಗಳು.terminal_socketpair[1] >= 0)
    close_and_reset (&socket_pair_1);

  ret = libcrun_cgroup_enter (&cg, &cgroup_ಸ್ಥಿತಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು fail;

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು fail;

  ret = libcrun_move_ಜಾಲ_devices (container, pid, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು fail;

  /* sync send own pid.  */
  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (sync_socket, &pid, ಗಾತ್ರ (pid)));
  ಆದರೆ (UNLIKELY (ret != ಗಾತ್ರ (pid)))
    {
      ಆದರೆ (ret >= 0)
        errno = 0;
      crun_make_error (err, errno, "write to sync socket");
      ಹೋಗು fail;
    }

  /* sync 1.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು fail;

  /* sync 2.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು fail;

  ret = libcrun_cgroup_enter_finalize (&cg, cgroup_ಸ್ಥಿತಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು fail;

  /* If the container process is created later, the scheduler is set on it then.  */
  ಆದರೆ (! libcrun_container_delayed_pidns_fork (container))
    {
      ret = ಗಣ_scheduler (pid, def, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹೋಗು fail;
    }

  /* The container is waiting that we write back.  In this phase we can launch the
     prestart hooks.  */
  ಆದರೆ (def->hooks && def->hooks->prestart_len)
    {
      libcrun_debug ("Running `prestart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "prestart", (hook **) def->hooks->prestart,
                      def->hooks->prestart_len, hooks_ಹೊರಹೋಗು_fd, hooks_err_fd, false, err);
      ಆದರೆ (UNLIKELY (ret != 0))
        ಹೋಗು fail;
    }
  ಆದರೆ (def->hooks && def->hooks->create_runtime_len)
    {
      libcrun_debug ("Running `create` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "createRuntime", (hook **) def->hooks->create_runtime,
                      def->hooks->create_runtime_len, hooks_ಹೊರಹೋಗು_fd, hooks_err_fd, false, err);
      ಆದರೆ (UNLIKELY (ret != 0))
        ಹೋಗು fail;
    }

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_ದತ್ತಾಂಶ, &seccomp_gen_ctx, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು fail;
  close_and_reset (&seccomp_fd);

  /* sync 3.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು fail;

  /* The process joining an existing PID namespace is created once the container root is in place.  */
  ಆದರೆ (libcrun_container_delayed_pidns_fork (container))
    {
      pid_t new_pid = 0;

      ret = sync_socket_wait_sync (context, sync_socket, false, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹೋಗು fail;

      ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (sync_socket, &new_pid, ಗಾತ್ರ (new_pid)));
      ಆದರೆ (UNLIKELY (ret != ಗಾತ್ರ (new_pid)))
        {
          ಆದರೆ (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "read pid from sync socket");
          ಹೋಗು fail;
        }

      /* Cleanup the first process.  */
      waitpid_ignore_stopped (pid, ಖಾಲಿ, 0);

      pid = new_pid;

      ret = ಗಣ_scheduler (pid, def, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹೋಗು fail;

      ret = libcrun_handle_pidfd_receiver (pid, container, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹೋಗು fail;

      /* sync send own pid.  */
      ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (sync_socket, &pid, ಗಾತ್ರ (pid)));
      ಆದರೆ (UNLIKELY (ret != ಗಾತ್ರ (pid)))
        {
          ಆದರೆ (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "write to sync socket");
          ಹೋಗು fail;
        }
    }

  ret = terminal_ಸಿದ್ಧತೆ (def, context, socket_pair_0, &terminal_fd, &orig_terminal, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು fail;

  close_and_reset (&socket_pair_0);

  /* sync 4.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು fail;

  ret = close_and_reset (&sync_socket);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      crun_make_error (err, errno, "close sync_socket failed");
      ಹೋಗು fail;
    }

  libcrun_debug ("Writing container status");
  ret = write_container_ಸ್ಥಿತಿ (container, context, pid, cgroup_ಸ್ಥಿತಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು fail;

  /* Run poststart hooks here only if the container is created using "run".  For create+start, the
     hooks will be executed as part of the start command.  */
  ಆದರೆ (context->fifo_exec_wait_fd < 0 && def->hooks && def->hooks->poststart_len)
    {
      libcrun_debug ("Running `poststart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_ಹೊರಹೋಗು_fd, hooks_err_fd, false, err);
      ಆದರೆ (UNLIKELY (ret != 0))
        ಹೋಗು fail;
    }

  /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
  ಆದರೆ (own_seccomp_receiver_fd >= 0)
    {
      libcrun_debug ("Receiving seccomp fd");
      seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
      ಆದರೆ (UNLIKELY (seccomp_notify_fd < 0))
        ಹೋಗು fail;

      ret = close_and_reset (&own_seccomp_receiver_fd);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          crun_make_error (err, errno, "close seccomp receiver fd failed");
          ಹೋಗು fail;
        }
    }

  {
    ರಚನೆ wait_for_process_ವಾದಗಳು ವಾದಗಳು = {
      .pid = pid,
      .context = context,
      .terminal_fd = terminal_fd,
      .notify_socket = notify_socket,
      .container_ready_fd = container_ready_fd,
      .seccomp_notify_fd = seccomp_notify_fd,
      .seccomp_notify_plugins = seccomp_notify_plugins,
    };
    ret = wait_for_process (&ವಾದಗಳು, err);
  }
  ಆದರೆ (! context->detach)
    {
      libcrun_error_t tmp_err = ಖಾಲಿ;
      cleanup_watch (context, def, cgroup_ಸ್ಥಿತಿ, 0, sync_socket, terminal_fd, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ಹಿಂತಿರುಗಿ ret;

fail:
  ret = cleanup_watch (context, def, cgroup_ಸ್ಥಿತಿ, pid, sync_socket, terminal_fd, err);
  ಆದರೆ (cgroup_ಸ್ಥಿತಿ)
    {
      libcrun_error_t tmp_err = ಖಾಲಿ;
      libcrun_cgroup_destroy (cgroup_ಸ್ಥಿತಿ, &tmp_err);
      crun_error_release (&tmp_err);
    }
  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
check_config_file (runtime_spec_schema_config_schema *def, libcrun_context_t *context, libcrun_error_t *err)
{
  ಆದರೆ (UNLIKELY (def->linux == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid config file, no `linux` block specified");

  ಆದರೆ (context->handler == ಖಾಲಿ)
    {
      ಆದರೆ (UNLIKELY (def->ಬೇರು == ಖಾಲಿ))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid config file, no `root` block specified");
      ಆದರೆ (UNLIKELY (def->mounts == ಖಾಲಿ))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid config file, no `mounts` block specified");
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libcrun_copy_config_file (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, libcrun_container_t *container, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  cleanup_free ಅಕ್ಷರ *dest_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *dir = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *ಬಫರ್ = ಖಾಲಿ;
  size_t len;

  ret = libcrun_get_state_directory (&dir, state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = append_paths (&dest_ಮಾರ್ಗ, err, dir, "config.json", ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (container->config_file == ಖಾಲಿ && container->config_file_content == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "config file not specified");

  ಆದರೆ (container->config_file == ಖಾಲಿ)
    {
      libcrun_debug ("Writing config file to: `%s`", dest_ಮಾರ್ಗ);
      ret = write_file (dest_ಮಾರ್ಗ, container->config_file_content, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (container->config_file_content), err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      libcrun_debug ("Reading config file: `%s`", container->config_file);
      ret = read_all_file (container->config_file, &ಬಫರ್, &len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      libcrun_debug ("Writing config file to: `%s`", dest_ಮಾರ್ಗ);
      ret = write_file (dest_ಮಾರ್ಗ, ಬಫರ್, len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ
force_delete_container_ಸ್ಥಿತಿ (libcrun_context_t *context, libcrun_container_t *container,
                               runtime_spec_schema_config_schema *def)
{
  libcrun_error_t tmp_err = ಖಾಲಿ;
  container_delete_ಆಂತರಿಕ (context, container, def, context->id, true, false, &tmp_err);
  libcrun_error_report_and_release (&tmp_err);
}

/* Serialize ERR to FD as an int errno followed by a NUL-terminated message.
   Used by forked children to report a failure to the parent process.  */
ಸ್ಥಿರ ಶೂನ್ಯ
write_error_ಗಮ್ಯ_pipe (ಪೂರ್ಣಾಂಕ fd, libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ = (*err)->ಸಂದೇಶ;
  ಪೂರ್ಣಾಂಕ errcode = crun_error_get_errno (err);

  ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (fd, &errcode, ಗಾತ್ರ (errcode)));
  ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (fd, ಸಂದೇಶ, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಸಂದೇಶ) + 1));
}

ಪೂರ್ಣಾಂಕ
libcrun_container_ಸತತ (libcrun_context_t *context, libcrun_container_t *container, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ options,
                       libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಪೂರ್ಣಾಂಕ ret;
  ಪೂರ್ಣಾಂಕ detach = context->detach;
  ಪೂರ್ಣಾಂಕ container_ret_ಸ್ಥಿತಿ[2];
  cleanup_close ಪೂರ್ಣಾಂಕ pipefd0 = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ pipefd1 = -1;
  libcrun_error_t tmp_err = ಖಾಲಿ;

  container->context = context;

  ret = validate_options (options, LIBCRUN_ಸತತ_OPTIONS_PREFORK | LIBCRUN_ಸತತ_OPTIONS_KEEP, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = check_config_file (def, context, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (def->process && def->process->terminal && detach && context->console_socket == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "use --console-socket with --detach when a terminal is used");

  ret = libcrun_ಸ್ಥಿತಿ_check_directories (context->state_ಬೇರು, context->id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (! detach && (options & LIBCRUN_ಸತತ_OPTIONS_PREFORK) == 0)
    {
      ret = libcrun_copy_config_file (context->id, context->state_ಬೇರು, container, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = libcrun_container_ಸತತ_ಆಂತರಿಕ (container, context, ಖಾಲಿ, err);
      ಆದರೆ (! (options & LIBCRUN_ಸತತ_OPTIONS_KEEP))
        force_delete_container_ಸ್ಥಿತಿ (context, container, def);
      ಹಿಂತಿರುಗಿ ret;
    }

  ret = pipe2 (container_ret_ಸ್ಥಿತಿ, O_CLOEXEC);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_ಸ್ಥಿತಿ[0];
  pipefd1 = container_ret_ಸ್ಥಿತಿ[1];

  ret = fork ();
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fork");
  ಆದರೆ (ret)
    {
      ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, ಖಾಲಿ, 0);

      ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (pipefd0, &ಸ್ಥಿತಿ_2, ಗಾತ್ರ (ಸ್ಥಿತಿ_2)));
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "read from sync pipe");

      ಆದರೆ (ಸ್ಥಿತಿ_2 < 0)
        {
          ಪೂರ್ಣಾಂಕ errno_;
          ಅಕ್ಷರ buf[512];
          ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (pipefd0, &errno_, ಗಾತ್ರ (errno_)));
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "read from sync pipe");

          ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (pipefd0, buf, ಗಾತ್ರ (buf) - 1));
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "read from sync pipe");
          buf[ret] = '\0';

          ಹಿಂತಿರುಗಿ crun_make_error (err, errno_, "%s", buf);
        }

      ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
    }

  close_and_reset (&pipefd0);

  /* forked process.  */
  ret = detach_process ();
  ಆದರೆ (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_ಬೇರು, container, &tmp_err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು fail;

  ret = libcrun_container_ಸತತ_ಆಂತರಿಕ (container, context, ಖಾಲಿ, &tmp_err);
  ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (pipefd1, &ret, ಗಾತ್ರ (ret)));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು fail;

  ನಿರ್ಗಮಿಸು (EXIT_SUCCESS);
fail:

  ಆದರೆ (! (options & LIBCRUN_ಸತತ_OPTIONS_KEEP))
    force_delete_container_ಸ್ಥಿತಿ (context, container, def);
  ಆದರೆ (tmp_err)
    {
      write_error_ಗಮ್ಯ_pipe (pipefd1, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ನಿರ್ಗಮಿಸು (EXIT_FAILURE);
}

ಪೂರ್ಣಾಂಕ
libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ options,
                          libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಪೂರ್ಣಾಂಕ ret;
  ಪೂರ್ಣಾಂಕ container_ready_pipe[2];
  cleanup_close ಪೂರ್ಣಾಂಕ pipefd0 = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ pipefd1 = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ exec_fifo_fd = -1;
  context->detach = 1;

  libcrun_debug ("Creating container: `%s`", context->id);
  container->context = context;

  ret = validate_options (options, LIBCRUN_CREATE_OPTIONS_PREFORK, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = check_config_file (def, context, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (def->process && def->process->terminal && context->console_socket == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "use --console-socket with create when a terminal is used");

  ret = libcrun_ಸ್ಥಿತಿ_check_directories (context->state_ಬೇರು, context->id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  exec_fifo_fd = libcrun_ಸ್ಥಿತಿ_create_exec_fifo (context->state_ಬೇರು, context->id, err);
  ಆದರೆ (UNLIKELY (exec_fifo_fd < 0))
    ಹಿಂತಿರುಗಿ exec_fifo_fd;

  context->fifo_exec_wait_fd = exec_fifo_fd;
  exec_fifo_fd = -1;

  ಆದರೆ ((options & LIBCRUN_ಸತತ_OPTIONS_PREFORK) == 0)
    {
      libcrun_debug ("Running with prefork enabled");
      ret = libcrun_copy_config_file (context->id, context->state_ಬೇರು, container, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
      ret = libcrun_container_ಸತತ_ಆಂತರಿಕ (container, context, ಖಾಲಿ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        force_delete_container_ಸ್ಥಿತಿ (context, container, def);
      ಹಿಂತಿರುಗಿ ret;
    }

  ret = pipe2 (container_ready_pipe, O_CLOEXEC);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "pipe");
  pipefd0 = container_ready_pipe[0];
  pipefd1 = container_ready_pipe[1];

  ret = fork ();
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fork");
  ಆದರೆ (ret)
    {
      ಪೂರ್ಣಾಂಕ exit_ಸಂಕೇತ;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, ಖಾಲಿ, 0);

      ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (pipefd0, &exit_ಸಂಕೇತ, ಗಾತ್ರ (exit_ಸಂಕೇತ)));
      ಆದರೆ (UNLIKELY (ret <= 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, ret < 0 ? errno : 0, "waiting for container to be ready");

      ಆದರೆ (ret != ಗಾತ್ರ (exit_ಸಂಕೇತ) || exit_ಸಂಕೇತ < 0)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: read invalid exit code");

      ಆದರೆ (exit_ಸಂಕೇತ == 0)
        ಹಿಂತಿರುಗಿ 0;

      libcrun_debug ("Exit code is `%d`, deleting container", exit_ಸಂಕೇತ);
      libcrun_error_t tmp_err = ಖಾಲಿ;
      container_delete_ಆಂತರಿಕ (context, ಖಾಲಿ, def, context->id, true, true, &tmp_err);
      libcrun_error_report_and_release (&tmp_err);
      ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "error creating container");
    }

  /* forked process.  */
  ret = detach_process ();
  ಆದರೆ (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_ಬೇರು, container, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಪೂರ್ಣಾಂಕ errcode = crun_error_get_errno (err);
      crun_error_release (err);
      libcrun_fail_with_error (errcode, "copy config file");
    }

  ret = libcrun_container_ಸತತ_ಆಂತರಿಕ (container, context, &pipefd1, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      force_delete_container_ಸ್ಥಿತಿ (context, container, def);
      libcrun_error ((*err)->ಸ್ಥಿತಿ_2, "%s", (*err)->ಸಂದೇಶ);
      crun_error_release (err);
      crun_ಗಣ_output_handler (log_write_ಗಮ್ಯ_stderr, ಖಾಲಿ);
    }

  ಆದರೆ (pipefd1 >= 0)
    ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (pipefd1, &ret, ಗಾತ್ರ (ret)));
  ನಿರ್ಗಮಿಸು (ret ? EXIT_FAILURE : 0);
}

ಪೂರ್ಣಾಂಕ
libcrun_container_ಆರಂಭ (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು = context->state_ಬೇರು;
  runtime_spec_schema_config_schema *def;
  cleanup_container_ಸ್ಥಿತಿ libcrun_container_ಸ್ಥಿತಿ_t ಸ್ಥಿತಿ_2 = {};
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
  ಪೂರ್ಣಾಂಕ ret;

  ret = libcrun_read_container_ಸ್ಥಿತಿ (&ಸ್ಥಿತಿ_2, state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_is_container_running (&ಸ್ಥಿತಿ_2, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (! ret)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "container `%s` is not running", id);

  ret = read_container_config_from_state (&container, state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (context->notify_socket)
    {
      ret = get_notify_fd (context, container, &fd, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ret = libcrun_ಸ್ಥಿತಿ_write_exec_fifo (context->state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  def = container->container_def;

  ಆದರೆ (context->notify_socket)
    {
      ಆದರೆ (fd >= 0)
        {
          fd_set read_ಗಣ;

          ಆಗ (1)
            {
              ರಚನೆ timeval timeout = {
                .tv_sec = 0,
                .tv_usec = 10000,
              };
              FD_ZERO (&read_ಗಣ);
              FD_SET (fd, &read_ಗಣ);

              ret = select (fd + 1, &read_ಗಣ, ಖಾಲಿ, ಖಾಲಿ, &timeout);
              ಆದರೆ (UNLIKELY (ret < 0))
                ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "select failed");
              ಆದರೆ (ret)
                {
                  ret = handle_notify_socket (fd, err);
                  ಆದರೆ (UNLIKELY (ret < 0))
                    ಹಿಂತಿರುಗಿ ret;
                  ಆದರೆ (ret)
                    ನಿಲ್ಲಿಸು;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  ret = libcrun_is_container_running (&ಸ್ಥಿತಿ_2, err);
                  ಆದರೆ (UNLIKELY (ret < 0))
                    ಹಿಂತಿರುಗಿ ret;
                  ಆದರೆ (! ret)
                    ಹಿಂತಿರುಗಿ 0;
                }
            }
        }
    }

  /* The container is considered running only after we got the notification from the
     notify_socket, if any.  */
  ಆದರೆ (def->hooks && def->hooks->poststart_len)
    {
      cleanup_close ಪೂರ್ಣಾಂಕ hooks_ಹೊರಹೋಗು_fd = -1;
      cleanup_close ಪೂರ್ಣಾಂಕ hooks_err_fd = -1;

      ret = open_hooks_output (container, &hooks_ಹೊರಹೋಗು_fd, &hooks_err_fd, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = do_hooks (def, ಸ್ಥಿತಿ_2.pid, context->id, false, ಸ್ಥಿತಿ_2.bundle, ಸ್ಥಿತಿ_2.bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_ಹೊರಹೋಗು_fd, hooks_err_fd, false, err);
      ಆದರೆ (UNLIKELY (ret != 0))
        {
          libcrun_error_t tmp_err = ಖಾಲಿ;
          container_delete_ಆಂತರಿಕ (context, container, def, id, true, true, &tmp_err);
          libcrun_error_report_and_release (&tmp_err);
          ಹಿಂತಿರುಗಿ ret;
        }
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_get_container_state_string (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು,
                                    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **container_ಸ್ಥಿತಿ, ಪೂರ್ಣಾಂಕ *running, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret, has_fifo = 0;
  bool paused = false;

  ret = libcrun_is_container_running (ಸ್ಥಿತಿ_2, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;
  *running = ret;

  ಆದರೆ (*running)
    {
      ret = libcrun_ಸ್ಥಿತಿ_has_read_exec_fifo (state_ಬೇರು, id, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
      has_fifo = ret;
    }

  ಆದರೆ (*running && ! has_fifo)
    {
      cleanup_cgroup_ಸ್ಥಿತಿ ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ = ಖಾಲಿ;

      cgroup_ಸ್ಥಿತಿ = libcrun_cgroup_make_ಸ್ಥಿತಿ (ಸ್ಥಿತಿ_2);

      ret = libcrun_cgroup_is_container_paused (cgroup_ಸ್ಥಿತಿ, &paused, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          /*
            The cgroup might have been cleaned up by the time we try to read it, ignore both
            ENOENT and ENODEV:
            - ENOENT: if the open(CGROUP_PATH) fails because the cgroup was deleted.
            - ENODEV: if the cgroup is deleted between the open and reading the freeze status.
          */

          errno = crun_error_get_errno (err);
          ಆದರೆ (errno == ENOENT || errno == ENODEV)
            {
              crun_error_release (err);
              *container_ಸ್ಥಿತಿ = "stopped";
              ಹಿಂತಿರುಗಿ 0;
            }

          ಹಿಂತಿರುಗಿ ret;
        }
    }

  ಆದರೆ (! *running)
    *container_ಸ್ಥಿತಿ = "stopped";
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (has_fifo)
    *container_ಸ್ಥಿತಿ = "created";
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (paused)
    *container_ಸ್ಥಿತಿ = "paused";
  ಇಲ್ಲದಿದ್ದರೆ
    *container_ಸ್ಥಿತಿ = "running";

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_container_state_json (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಅಕ್ಷರ **ಹೊರಹೋಗು, libcrun_error_t *err)
{
  ಅಕ್ಷರ *ಬಫರ್ = ಖಾಲಿ;
  size_t len = 0;
  FILE *f;
  ಪೂರ್ಣಾಂಕ ret;

  f = open_memstream (&ಬಫರ್, &len);
  ಆದರೆ (f == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open_memstream");

  ret = libcrun_container_state (context, id, f, err);
  fclose (f);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಬಿಡುಗಡೆ (ಬಫರ್);
      ಹಿಂತಿರುಗಿ ret;
    }

  *ಹೊರಹೋಗು = ಬಫರ್;
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_container_state (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, FILE *ಹೊರಹೋಗು, libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ OCI_CONFIG_ಆವೃತ್ತಿ = "1.0.0";
  libcrun_container_ಸ್ಥಿತಿ_t ಸ್ಥಿತಿ_2 = {};
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು = context->state_ಬೇರು;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *container_ಸ್ಥಿತಿ = ಖಾಲಿ;
  json_gen_ctx *gen = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *buf;
  ಪೂರ್ಣಾಂಕ ret = 0;
  ಪೂರ್ಣಾಂಕ r = json_gen_ಸ್ಥಿತಿ_ok;
  ಪೂರ್ಣಾಂಕ running;
  size_t len;

  ret = libcrun_read_container_ಸ್ಥಿತಿ (&ಸ್ಥಿತಿ_2, state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_get_container_state_string (id, &ಸ್ಥಿತಿ_2, state_ಬೇರು, &container_ಸ್ಥಿತಿ, &running, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು ನಿರ್ಗಮಿಸು;

  ret = 0;
  ಆದರೆ (! json_gen_init (&gen, ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));
  GEN_ಕೀಲಿ (gen, "ociVersion");
  GEN_STR (gen, OCI_CONFIG_ಆವೃತ್ತಿ);

  GEN_ಕೀಲಿ (gen, "id");
  GEN_STR (gen, id);

  GEN_ಕೀಲಿ (gen, "pid");
  GEN_OR_FAIL (map_int (gen, running ? ಸ್ಥಿತಿ_2.pid : 0));

  GEN_ಕೀಲಿ (gen, "status");
  GEN_STR (gen, container_ಸ್ಥಿತಿ);

  GEN_ಕೀಲಿ (gen, "bundle");
  GEN_STR (gen, ಸ್ಥಿತಿ_2.bundle);

  GEN_ಕೀಲಿ (gen, "rootfs");
  GEN_STR (gen, ಸ್ಥಿತಿ_2.rootfs);

  GEN_ಕೀಲಿ (gen, "created");
  GEN_STR (gen, ಸ್ಥಿತಿ_2.created);

  ಆದರೆ (ಸ್ಥಿತಿ_2.scope)
    {
      GEN_ಕೀಲಿ (gen, "systemd-scope");
      GEN_STR (gen, ಸ್ಥಿತಿ_2.scope);
    }

  ಆದರೆ (ಸ್ಥಿತಿ_2.owner)
    {
      GEN_ಕೀಲಿ (gen, "owner");
      GEN_STR (gen, ಸ್ಥಿತಿ_2.owner);
    }

  {
    cleanup_container libcrun_container_t *container = ಖಾಲಿ;

    ret = read_container_config_from_state (&container, state_ಬೇರು, id, err);
    ಆದರೆ (UNLIKELY (ret < 0))
      ಹೋಗು ನಿರ್ಗಮಿಸು;

    ಆದರೆ (container->container_def->annotations && container->container_def->annotations->len)
      GEN_OR_FAIL (gen_annotations (gen, container->container_def->annotations));
  }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  fprintf (ಹೊರಹೋಗು, "%s\n", buf);

ನಿರ್ಗಮಿಸು:
  ಆದರೆ (gen)
    json_gen_free (gen);
  libcrun_free_container_ಸ್ಥಿತಿ (&ಸ್ಥಿತಿ_2);
  ಹಿಂತಿರುಗಿ ret;

gen_error:
  ret = json_gen_error_ಗಮ್ಯ_crun_error (r, err);
  ಹೋಗು ನಿರ್ಗಮಿಸು;
}

ಪೂರ್ಣಾಂಕ
libcrun_container_exec (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, runtime_spec_schema_config_schema_process *process,
                        libcrun_error_t *err)
{
  ರಚನೆ libcrun_container_exec_options_s opts;
  memset (&opts, 0, ಗಾತ್ರ (opts));

  opts.struct_ಗಾತ್ರ = ಗಾತ್ರ (opts);
  opts.process = process;

  ಹಿಂತಿರುಗಿ libcrun_container_exec_with_options (context, id, &opts, err);
}

ಪೂರ್ಣಾಂಕ
libcrun_container_exec_process_file (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err)
{
  ರಚನೆ libcrun_container_exec_options_s opts;
  memset (&opts, 0, ಗಾತ್ರ (opts));

  opts.struct_ಗಾತ್ರ = ಗಾತ್ರ (opts);
  opts.ಮಾರ್ಗ = ಮಾರ್ಗ;

  ಹಿಂತಿರುಗಿ libcrun_container_exec_with_options (context, id, &opts, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
exec_from_process_json (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *process_json, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cgroup,
                        bool ವಿಲೀನ_env, libcrun_error_t *err)
{
  ರಚನೆ parser_context parser_ctx = { .options = 0, .errfile = stderr };
  ರಚನೆ libcrun_container_exec_options_s opts;
  runtime_spec_schema_config_schema_process *process;
  parser_error parser_err = ಖಾಲಿ;
  json_object *doc = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;

  ret = parse_json_file (&doc, process_json, &parser_ctx, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  process = make_runtime_spec_schema_config_schema_process (doc, &parser_ctx, &parser_err);
  json_object_put (doc);
  ಆದರೆ (process == ಖಾಲಿ)
    {
      ret = crun_make_error (err, 0, "cannot parse process: `%s`", parser_err ? parser_err : "unknown error");
      ಬಿಡುಗಡೆ (parser_err);
      ಹಿಂತಿರುಗಿ ret;
    }

  memset (&opts, 0, ಗಾತ್ರ (opts));
  opts.struct_ಗಾತ್ರ = ಗಾತ್ರ (opts);
  opts.process = process;
  opts.cgroup = cgroup;
  opts.ವಿಲೀನ_env = ವಿಲೀನ_env;

  ret = libcrun_container_exec_with_options (context, id, &opts, err);
  free_runtime_spec_schema_config_schema_process (process);
  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
libcrun_container_exec_json (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *process_json, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ exec_from_process_json (context, id, process_json, ಖಾಲಿ, false, err);
}

ಪೂರ್ಣಾಂಕ
libcrun_container_exec_with_options_json (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id,
                                          ಸ್ಥಿರಾಂಕ ರಚನೆ libcrun_exec_options_s *opts, libcrun_error_t *err)
{
  ಆದರೆ (opts == ಖಾಲಿ || opts->struct_ಗಾತ್ರ == 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, EINVAL, "invalid exec options");

  ಹಿಂತಿರುಗಿ exec_from_process_json (context, id, opts->process_json, opts->cgroup, opts->ವಿಲೀನ_env, err);
}

#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_process_schema __attribute__ ((cleanup (cleanup_process_schemap)))

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cleanup_process_schemap (runtime_spec_schema_config_schema_process **p)
{
  runtime_spec_schema_config_schema_process *process = *p;
  ಆದರೆ (process)
    (ಶೂನ್ಯ) free_runtime_spec_schema_config_schema_process (process);
}

/* Build the seccomp receiver payload (when needed), apply the seccomp profile and
   close the seccomp file descriptors.  Used from exec_process_entrypoint both before
   and after the capabilities are set, depending on process->no_new_privileges.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
apply_seccomp_for_exec (libcrun_container_t *container, pid_t own_pid, ಪೂರ್ಣಾಂಕ seccomp_fd,
                        ಪೂರ್ಣಾಂಕ seccomp_receiver_fd, ಅಕ್ಷರ **seccomp_flags, size_t seccomp_flags_len,
                        libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *seccomp_fd_payload = ಖಾಲಿ;
  size_t seccomp_fd_payload_len = 0;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (seccomp_receiver_fd >= 0)
    {
      ret = get_seccomp_receiver_fd_payload (container, "running", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ret = libcrun_apply_seccomp (seccomp_fd, seccomp_receiver_fd, seccomp_fd_payload,
                               seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  close_and_reset (&seccomp_fd);
  close_and_reset (&seccomp_receiver_fd);
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
exec_process_entrypoint (libcrun_context_t *context,
                         libcrun_container_t *container,
                         runtime_spec_schema_config_schema_process *process,
                         ಪೂರ್ಣಾಂಕ *pipefd1,
                         ಪೂರ್ಣಾಂಕ seccomp_fd,
                         ಪೂರ್ಣಾಂಕ seccomp_receiver_fd,
                         ರಚನೆ custom_handler_instance_s *custom_handler,
                         bool ವಿಲೀನ_env,
                         libcrun_error_t *err)
{
  runtime_spec_schema_config_schema_process_capabilities *capabilities = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *exec_ಮಾರ್ಗ = ಖಾಲಿ;
  uid_t container_uid;
  gid_t container_gid;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cwd;
  bool chdir_ಮುಗಿದ = false;
  size_t seccomp_flags_len = 0;
  ಅಕ್ಷರ **seccomp_flags = ಖಾಲಿ;
  pid_t own_pid = 0;
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  container_uid = process->user ? process->user->uid : 0;
  container_gid = process->user ? process->user->gid : 0;

  ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (*pipefd1, &own_pid, ಗಾತ್ರ (own_pid)));

  cwd = process->cwd ? process->cwd : "/";
  ಆದರೆ (LIKELY (libcrun_ಸುರಕ್ಷಿತ_chdir (cwd, err) == 0))
    chdir_ಮುಗಿದ = true;
  ಇಲ್ಲದಿದ್ದರೆ
    crun_error_release (err);

  ret = unblock_ಸಂಕೇತಗಳು (err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = clearenv ();
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "clearenv");

  ಆದರೆ ((ವಿಲೀನ_env || ! process->env_len) && container->container_def->process && container->container_def->process->env_len)
    {
      ಅಕ್ಷರ *e;

      ಗಾಗಿ (i = 0; i < container->container_def->process->env_len; i++)
        {
          e = container->container_def->process->env[i];
          ಆದರೆ (putenv (e) < 0)
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "putenv `%s`", e);
        }
    }
  ಆದರೆ (process->env_len)
    {
      ಗಾಗಿ (i = 0; i < process->env_len; i++)
        ಆದರೆ (putenv (process->env[i]) < 0)
          ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "putenv `%s`", process->env[i]);
    }

  ಆದರೆ (is_ಖಾಲಿಗಳು_string (getenv ("HOME")))
    {
      ret = ಗಣ_home_env (container_uid);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          ಆದರೆ (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "setenv HOME");
        }
    }

  ret = libcrun_ಗಣ_selinux_label (container, process, false, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_ಗಣ_apparmor_profile (container, process, false, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (container->container_def->linux && container->container_def->linux->seccomp)
    {
      seccomp_flags = container->container_def->linux->seccomp->flags;
      seccomp_flags_len = container->container_def->linux->seccomp->flags_len;
    }

  ret = ಕಂಡುಹಿಡಿ_executable (&exec_ಮಾರ್ಗ, process->ವಾದಗಳು[0], process->cwd, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಆದರೆ (crun_error_get_errno (err) == ENOENT)
        ಹಿಂತಿರುಗಿ ret;

      /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
         once the process switched to the use that runs in the container.  This might be necessary
         when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
         is not honored.  */
      crun_error_release (err);
    }

  ಆದರೆ (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_ಗಣ_personality (container->container_def->linux->personality, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, false, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_reopen_dev_null (err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (! process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ret = libcrun_container_setgroups (container, process, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = maybe_chown_std_streams (container_uid, container_gid, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (process->capabilities)
    capabilities = process->capabilities;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (container->container_def->process)
    capabilities = container->container_def->process->capabilities;

  ret = libcrun_ಗಣ_caps (capabilities, container_uid, container_gid, process->no_new_privileges,
                          custom_handler != ಖಾಲಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (UNLIKELY (exec_ಮಾರ್ಗ == ಖಾಲಿ))
    {
      ret = ಕಂಡುಹಿಡಿ_executable (&exec_ಮಾರ್ಗ, process->ವಾದಗಳು[0], process->cwd, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          ಆದರೆ (custom_handler == ಖಾಲಿ || is_ಖಾಲಿಗಳು_string (process->ವಾದಗಳು[0]))
            ಹಿಂತಿರುಗಿ ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          exec_ಮಾರ್ಗ = xstrdup (process->ವಾದಗಳು[0]);
        }
    }

  ಆದರೆ (! chdir_ಮುಗಿದ)
    {
      ret = libcrun_ಸುರಕ್ಷಿತ_chdir (cwd, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (process->user)
    umask (process->user->umask_present ? process->user->umask : 0022);

  ret = 0;
  ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (*pipefd1, &ret, ಗಾತ್ರ (ret)));
  ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (*pipefd1));
  *pipefd1 = -1;

  ಆದರೆ (custom_handler)
    {
      ಆದರೆ (custom_handler->vtable->exec_func == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "the handler does not support exec");

      ಆದರೆ (context->argv)
        erase_initial_environment (context->argv, context->argc);

      ret = custom_handler->vtable->exec_func (custom_handler->cookie,
                                               container,
                                               exec_ಮಾರ್ಗ,
                                               process->ವಾದಗಳು);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "exec container process failed with handler as `%s`", custom_handler->vtable->name);

      ಸುರಕ್ಷಿತ_exit (EXIT_FAILURE);
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation, because the seccomp filter is already in place and it could
     stop some syscalls used by mark_or_close_fds_ge_than.
  */
  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, true, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    crun_error_release (err);

  ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (execv (exec_ಮಾರ್ಗ, process->ವಾದಗಳು));
  libcrun_fail_with_error (errno, "exec `%s`", exec_ಮಾರ್ಗ);
}

/* Close the file descriptors inherited from the caller, other than those
   to be passed to the container process.  */
ಪೂರ್ಣಾಂಕ
libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ mark_or_close_fds_ge_than (ಖಾಲಿ, context->preserve_fds + 3, true, err);
}

ಪೂರ್ಣಾಂಕ
libcrun_ತುಂಬು_process_from_file (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, runtime_spec_schema_config_schema_process **process,
                                libcrun_error_t *err)
{
  ರಚನೆ parser_context ctx = { 0, stderr };
  cleanup_free ಅಕ್ಷರ *content = ಖಾಲಿ;
  parser_error parser_err = ಖಾಲಿ;
  json_object *doc = ಖಾಲಿ;
  size_t len;
  ಪೂರ್ಣಾಂಕ ret;

  ret = read_all_file (ಮಾರ್ಗ, &content, &len, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  *process = make_runtime_spec_schema_config_schema_process (doc, &ctx, &parser_err);
  ಆದರೆ (UNLIKELY (*process == ಖಾಲಿ))
    {
      ret = crun_make_error (err, 0, "cannot parse process file: `%s`", parser_err);
      ಬಿಡುಗಡೆ (parser_err);
      ಆದರೆ (doc)
        json_object_put (doc);
      ಹಿಂತಿರುಗಿ ret;
    }

  ಬಿಡುಗಡೆ (parser_err);
  ಆದರೆ (doc)
    json_object_put (doc);

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_container_exec_with_options (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id,
                                     ರಚನೆ libcrun_container_exec_options_s *opts,
                                     libcrun_error_t *err)
{
  cleanup_custom_handler_instance ರಚನೆ custom_handler_instance_s *custom_handler = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ container_ಸ್ಥಿತಿ, ret;
  bool container_paused = false;
  pid_t pid;
  libcrun_container_ಸ್ಥಿತಿ_t ಸ್ಥಿತಿ_2 = {};
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು = context->state_ಬೇರು;
  cleanup_close ಪೂರ್ಣಾಂಕ terminal_fd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ seccomp_fd = -1;
  cleanup_terminal ಶೂನ್ಯ *orig_terminal = ಖಾಲಿ;
  cleanup_container libcrun_container_t *container = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ container_ret_ಸ್ಥಿತಿ[2];
  cleanup_close ಪೂರ್ಣಾಂಕ pipefd0 = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ pipefd1 = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ seccomp_receiver_fd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ own_seccomp_receiver_fd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ seccomp_notify_fd = -1;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *seccomp_notify_plugins = ಖಾಲಿ;
  __attribute__ ((ಬಳಸದ)) cleanup_process_schema runtime_spec_schema_config_schema_process *process_cleanup = ಖಾಲಿ;
  runtime_spec_schema_config_schema_process *process = opts->process;
  ರಚನೆ libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  ಪೂರ್ಣಾಂಕ ret_from_ಮಗು = 0;

  ret = libcrun_read_container_ಸ್ಥಿತಿ (&ಸ್ಥಿತಿ_2, state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_is_container_running (&ಸ್ಥಿತಿ_2, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;
  container_ಸ್ಥಿತಿ = ret;

  ret = read_container_config_from_state (&container, state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  container->context = context;

  ಆದರೆ (container_ಸ್ಥಿತಿ == 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "the container `%s` is not running", id);

  {
    cleanup_cgroup_ಸ್ಥಿತಿ ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ = ಖಾಲಿ;

    cgroup_ಸ್ಥಿತಿ = libcrun_cgroup_make_ಸ್ಥಿತಿ (&ಸ್ಥಿತಿ_2);

    ret = libcrun_cgroup_is_container_paused (cgroup_ಸ್ಥಿತಿ, &container_paused, err);
    ಆದರೆ (UNLIKELY (ret < 0))
      ಹಿಂತಿರುಗಿ ret;
  }

  ಆದರೆ (UNLIKELY (container_paused))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot exec in a paused container `%s`", id);

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = ಖಂಡ_ಸಂಕೇತಗಳು (err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  libcrun_seccomp_gen_ctx_init (&seccomp_gen_ctx, container, false, 0);

  ret = libcrun_open_seccomp_bpf (&seccomp_gen_ctx, &seccomp_fd, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &seccomp_receiver_fd,
                                     &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (ಗಾತ್ರ (*opts) != opts->struct_ಗಾತ್ರ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, EINVAL, "invalid libcrun_container_exec_options_s struct");

  ಆದರೆ (opts->ಮಾರ್ಗ)
    {
      ಆದರೆ (process)
        ಹಿಂತಿರುಗಿ crun_make_error (err, EINVAL, "cannot specify both exec file and options");

      ret = libcrun_ತುಂಬು_process_from_file (opts->ಮಾರ್ಗ, &process, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      process_cleanup = process;
    }

  /* This must be done before we enter a user namespace.  If the process does
     not specify any rlimits, use the ones from the container configuration.  */
  ಆದರೆ (process->rlimits_len == 0 && container->container_def->process)
    ret = libcrun_ಗಣ_rlimits (container->container_def->process->rlimits,
                               container->container_def->process->rlimits_len, err);
  ಇಲ್ಲದಿದ್ದರೆ
    ret = libcrun_ಗಣ_rlimits (process->rlimits, process->rlimits_len, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_ಗಣ_mempolicy (container->container_def, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = pipe2 (container_ret_ಸ್ಥಿತಿ, O_CLOEXEC);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_ಸ್ಥಿತಿ[0];
  pipefd1 = container_ret_ಸ್ಥಿತಿ[1];

  /* If the new process block doesn't specify a SELinux label, AppArmor profile, user,
     I/O priority or scheduler, then use the configuration from the original config file.  */
  ಆದರೆ (container->container_def->process)
    {
      ಆದರೆ (process->io_ಆದ್ಯತೆ == ಖಾಲಿ && container->container_def->process->io_ಆದ್ಯತೆ)
        {
          process->io_ಆದ್ಯತೆ = clone_runtime_spec_schema_config_schema_process_io_ಆದ್ಯತೆ (container->container_def->process->io_ಆದ್ಯತೆ);
          ಆದರೆ (process->io_ಆದ್ಯತೆ == ಖಾಲಿ)
            OOM ();
        }

      ಆದರೆ (process->scheduler == ಖಾಲಿ && container->container_def->process->scheduler)
        {
          process->scheduler = clone_runtime_spec_schema_config_schema_process_scheduler (container->container_def->process->scheduler);
          ಆದರೆ (process->scheduler == ಖಾಲಿ)
            OOM ();
        }

      ಆದರೆ (process->selinux_label == ಖಾಲಿ && container->container_def->process->selinux_label)
        process->selinux_label = xstrdup (container->container_def->process->selinux_label);

      ಆದರೆ (process->apparmor_profile == ಖಾಲಿ && container->container_def->process->apparmor_profile)
        process->apparmor_profile = xstrdup (container->container_def->process->apparmor_profile);

      ಆದರೆ (process->user == ಖಾಲಿ && container->container_def->process->user)
        {
          process->user = clone_runtime_spec_schema_config_schema_process_user (container->container_def->process->user);
          ಆದರೆ (process->user == ಖಾಲಿ)
            OOM ();
        }
    }

  ret = initialize_security (container, process, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  /* Do not leak the session keyring of the caller into the container.  The
     keyring created for the container cannot be joined, so use an anonymous one.  */
  ಆದರೆ (! context->no_new_keyring && ! ಸ್ಥಿತಿ_2.no_new_keyring)
    {
      libcrun_debug ("Creating new keyring for the exec process");

      ret = libcrun_create_keyring (container, ಖಾಲಿ, process->selinux_label, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ret = prctl (PR_ಗಣ_DUMPABLE, 0, 0, 0, 0);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "prctl unset dumpable");

  pid = libcrun_join_process (context, container, ಸ್ಥಿತಿ_2.pid, &ಸ್ಥಿತಿ_2, opts->cgroup, context->detach,
                              process, process->terminal ? &terminal_fd : ಖಾಲಿ, err);
  ಆದರೆ (UNLIKELY (pid < 0))
    ಹಿಂತಿರುಗಿ pid;

  /* Process to exec.  */
  ಆದರೆ (pid == 0)
    {
      ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (pipefd0));
      pipefd0 = -1;

      exec_process_entrypoint (context, container, process, &pipefd1, seccomp_fd, seccomp_receiver_fd, custom_handler, opts->ವಿಲೀನ_env, err);
      /* It gets here only on errors.  */
      ಆದರೆ (*err)
        {
          ಆದರೆ (pipefd1 < 0)
            libcrun_fail_with_error ((*err)->ಸ್ಥಿತಿ_2, "%s", (*err)->ಸಂದೇಶ);
          ಇಲ್ಲದಿದ್ದರೆ
            {
              write_error_ಗಮ್ಯ_pipe (pipefd1, err);
              ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (pipefd1));
              pipefd1 = -1;
            }
        }
      ಸುರಕ್ಷಿತ_exit (EXIT_FAILURE);
    }

  ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (pipefd1));
  pipefd1 = -1;

  ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (pipefd0, &pid, ಗಾತ್ರ (pid)));

  ಆದರೆ (seccomp_fd >= 0)
    close_and_reset (&seccomp_fd);

  ಆದರೆ (terminal_fd >= 0)
    {
      ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ ಅಡ್ಡಸಾಲುಗಳು = 0, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = 0;

      ಆದರೆ (process->console_ಗಾತ್ರ)
        {
          ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = process->console_ಗಾತ್ರ->ಅಗಲ;
          ಅಡ್ಡಸಾಲುಗಳು = process->console_ಗಾತ್ರ->ಎತ್ತರ;
        }

      ret = libcrun_terminal_ಸಿದ್ಧತೆ_ಗಾತ್ರ (terminal_fd, ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ಆದರೆ (context->console_socket)
        {
          ಪೂರ್ಣಾಂಕ ret;
          cleanup_close ಪೂರ್ಣಾಂಕ console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
          ಆದರೆ (UNLIKELY (console_socket_fd < 0))
            ಹಿಂತಿರುಗಿ console_socket_fd;
          ret = send_fd_ಗಮ್ಯ_socket (console_socket_fd, terminal_fd, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
          close_and_reset (&terminal_fd);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ret = libcrun_clear_onlcr (terminal_fd, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            {
              flush_fd_ಗಮ್ಯ_err (context, terminal_fd);
              ಹಿಂತಿರುಗಿ ret;
            }

          ret = libcrun_ಗಣ_raw (0, &orig_terminal, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            {
              flush_fd_ಗಮ್ಯ_err (context, terminal_fd);
              ಹಿಂತಿರುಗಿ ret;
            }
        }
    }

  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (pipefd0, &ret_from_ಮಗು, ಗಾತ್ರ (ret_from_ಮಗು)));
  ಆದರೆ (ret != ಗಾತ್ರ (ret_from_ಮಗು))
    ret = crun_make_error (err, 0, "read pipe failed");
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ret_from_ಮಗು != 0)
    {
      cleanup_free ಅಕ್ಷರ *ಸಂದೇಶ = ಖಾಲಿ;
      size_t len = 0;

      ret = read_all_fd (pipefd0, "error stream", &ಸಂದೇಶ, &len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
      /* the string from read_all_fd is always NUL terminated.  */
      ret = crun_make_error (err, ret_from_ಮಗು, "%s", ಸಂದೇಶ);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
      ಆದರೆ (own_seccomp_receiver_fd >= 0)
        {
          seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
          ಆದರೆ (UNLIKELY (seccomp_notify_fd < 0))
            ಹಿಂತಿರುಗಿ seccomp_notify_fd;

          ret = close_and_reset (&own_seccomp_receiver_fd);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "close seccomp receiver fd failed");
        }

      {
        ರಚನೆ wait_for_process_ವಾದಗಳು ವಾದಗಳು = {
          .pid = pid,
          .context = context,
          .terminal_fd = terminal_fd,
          .notify_socket = -1,
          .container_ready_fd = ಖಾಲಿ,
          .seccomp_notify_fd = seccomp_notify_fd,
          .seccomp_notify_plugins = seccomp_notify_plugins,
        };

        ret = wait_for_process (&ವಾದಗಳು, err);
      }
    }

  flush_fd_ಗಮ್ಯ_err (context, terminal_fd);
  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
libcrun_container_update (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *content, size_t len arg_ಬಳಸದ,
                          libcrun_error_t *err)
{
  cleanup_custom_handler_instance ರಚನೆ custom_handler_instance_s *custom_handler = ಖಾಲಿ;
  runtime_spec_schema_config_linux_resources *resources = ಖಾಲಿ;
  cleanup_container libcrun_container_t *container = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು = context->state_ಬೇರು;
  ರಚನೆ parser_context ctx = { 0, stderr };
  libcrun_container_ಸ್ಥಿತಿ_t ಸ್ಥಿತಿ_2 = {};
  parser_error parser_err = ಖಾಲಿ;
  json_object *doc = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;

  ret = libcrun_read_container_ಸ್ಥಿತಿ (&ಸ್ಥಿತಿ_2, state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = read_container_config_from_state (&container, state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  resources = make_runtime_spec_schema_config_linux_resources (doc, &ctx, &parser_err);
  ಆದರೆ (UNLIKELY (resources == ಖಾಲಿ))
    {
      ret = crun_make_error (err, 0, "cannot parse resources: %s", parser_err);
      ಹೋಗು cleanup;
    }

  ಆದರೆ (custom_handler && custom_handler->vtable->modify_oci_configuration)
    {
      /* Adapt RESOURCES to be used from the modify_oci_configuration hook.  */
      cleanup_free runtime_spec_schema_config_linux *linux = xmalloc0 (ಗಾತ್ರ (*linux));
      cleanup_free runtime_spec_schema_config_schema *def = xmalloc0 (ಗಾತ್ರ (*def));

      def->linux = linux;
      linux->resources = resources;

      ret = custom_handler->vtable->modify_oci_configuration (custom_handler->cookie,
                                                              context,
                                                              def,
                                                              err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹೋಗು cleanup;
    }

  ret = libcrun_linux_container_update (&ಸ್ಥಿತಿ_2, state_ಬೇರು, resources, err);

cleanup:
  ಆದರೆ (doc)
    json_object_put (doc);
  ಬಿಡುಗಡೆ (parser_err);
  ಆದರೆ (resources)
    free_runtime_spec_schema_config_linux_resources (resources);

  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
libcrun_container_update_from_file (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *content = ಖಾಲಿ;
  size_t len;
  ಪೂರ್ಣಾಂಕ ret;

  ret = read_all_file (file, &content, &len, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ libcrun_container_update (context, id, content, len, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
compare_update_ಮೌಲ್ಯಗಳು (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b)
{
  ಸ್ಥಿರಾಂಕ ರಚನೆ libcrun_update_ಮೌಲ್ಯ_s *aa = a;
  ಸ್ಥಿರಾಂಕ ರಚನೆ libcrun_update_ಮೌಲ್ಯ_s *bb = b;
  ಪೂರ್ಣಾಂಕ ret;

  ret = strcmp (aa->section, bb->section);
  ಆದರೆ (ret)
    ಹಿಂತಿರುಗಿ ret;
  ಹಿಂತಿರುಗಿ strcmp (aa->name, bb->name);
}

ಪೂರ್ಣಾಂಕ
libcrun_container_update_from_ಮೌಲ್ಯಗಳು (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id,
                                      ರಚನೆ libcrun_update_ಮೌಲ್ಯ_s *ಮೌಲ್ಯಗಳು, size_t len,
                                      libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *current_section = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *buf;
  json_gen_ctx *gen = ಖಾಲಿ;
  size_t i, buf_len;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (! json_gen_init (&gen, ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "json_gen_init failed");
  json_gen_map_open (gen);

  qsort (ಮೌಲ್ಯಗಳು, len, ಗಾತ್ರ (ರಚನೆ libcrun_update_ಮೌಲ್ಯ_s), compare_update_ಮೌಲ್ಯಗಳು);

  ಗಾಗಿ (i = 0; i < len; i++)
    {
      ಆದರೆ (current_section == ಖಾಲಿ || strcmp (ಮೌಲ್ಯಗಳು[i].section, current_section))
        {
          ಆದರೆ (i > 0)
            json_gen_map_close (gen);

          current_section = ಮೌಲ್ಯಗಳು[i].section;
          json_gen_string (gen, current_section, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (current_section));
          json_gen_map_open (gen);
        }

      json_gen_string (gen, ಮೌಲ್ಯಗಳು[i].name, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಮೌಲ್ಯಗಳು[i].name));

      ಆದರೆ (ಮೌಲ್ಯಗಳು[i].numeric)
        json_gen_ಸಂಖ್ಯೆ (gen, ಮೌಲ್ಯಗಳು[i].ಮೌಲ್ಯ, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಮೌಲ್ಯಗಳು[i].ಮೌಲ್ಯ));
      ಇಲ್ಲದಿದ್ದರೆ
        json_gen_string (gen, ಮೌಲ್ಯಗಳು[i].ಮೌಲ್ಯ, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಮೌಲ್ಯಗಳು[i].ಮೌಲ್ಯ));
    }
  ಆದರೆ (len)
    json_gen_map_close (gen);

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);

  ret = libcrun_container_update (context, id, buf, buf_len, err);

  json_gen_free (gen);

  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ
populate_array_ಕ್ಷೇತ್ರ (ಅಕ್ಷರ ***ಕ್ಷೇತ್ರ, ಅಕ್ಷರ *array[], size_t ಅಂಕೆ_elements)
{
  size_t i;

  *ಕ್ಷೇತ್ರ = xmalloc0 ((ಅಂಕೆ_elements + 1) * ಗಾತ್ರ (ಅಕ್ಷರ *));
  ಗಾಗಿ (i = 0; i < ಅಂಕೆ_elements; i++)
    (*ಕ್ಷೇತ್ರ)[i] = xstrdup (array[i]);

  (*ಕ್ಷೇತ್ರ)[i] = ಖಾಲಿ;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CAP
ಸ್ಥಿರ ಶೂನ್ಯ
populate_capabilities (ರಚನೆ features_info_s *info, ಅಕ್ಷರ ***capabilities, size_t *ಅಂಕೆ_capabilities)
{
  size_t ಸೂಚ್ಯಂಕ = 0;
  cap_ಮೌಲ್ಯ_t i;
  ಅಕ್ಷರ *endptr;
  ಪೂರ್ಣಾಂಕ j;

  *ಅಂಕೆ_capabilities = 0;
  ಗಾಗಿ (i = 0;; i++)
    {
      ಅಕ್ಷರ *v = cap_ಗಮ್ಯ_name (i);
      ಆದರೆ (v == ಖಾಲಿ)
        ನಿಲ್ಲಿಸು;
      strtol (v, &endptr, 10);
      ಆದರೆ (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          ನಿಲ್ಲಿಸು;
        }
      (*ಅಂಕೆ_capabilities)++;
    }

  *capabilities = xmalloc0 ((*ಅಂಕೆ_capabilities + 1) * ಗಾತ್ರ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *));
  ಗಾಗಿ (i = 0; i < (cap_ಮೌಲ್ಯ_t) *ಅಂಕೆ_capabilities; i++)
    {
      ಅಕ್ಷರ *v = cap_ಗಮ್ಯ_name (i);
      ಆದರೆ (v == ಖಾಲಿ)
        ನಿಲ್ಲಿಸು;
      strtol (v, &endptr, 10);
      ಆದರೆ (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          ನಿಲ್ಲಿಸು;
        }

      // Convert capability name to uppercase
      ಗಾಗಿ (j = 0; v[j] != '\0'; j++)
        v[j] = ದೊಡ್ಡ_ಅಕ್ಷರ (v[j]);

      (*capabilities)[ಸೂಚ್ಯಂಕ] = v;
      ಸೂಚ್ಯಂಕ++;
    }

  (*capabilities)[ಸೂಚ್ಯಂಕ] = ಖಾಲಿ; // Terminate the array with NULL
  populate_array_ಕ್ಷೇತ್ರ (&(info->linux.capabilities), *capabilities, *ಅಂಕೆ_capabilities);
}
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಶೂನ್ಯ
retrieve_mount_options (ರಚನೆ features_info_s **info)
{
  cleanup_free ಸ್ಥಿರಾಂಕ ರಚನೆ ಪ್ರಸರಣ_flags_s *mount_options_list = ಖಾಲಿ;
  size_t ಅಂಕೆ_mount_options = 0;

  // Retrieve mount options from wordlist
  mount_options_list = get_mount_flags_from_wordlist ();

  // Calculate the number of mount options
  ಆಗ (mount_options_list[ಅಂಕೆ_mount_options].name != ಖಾಲಿ)
    ಅಂಕೆ_mount_options++;

  // Allocate memory for mount options in info struct
  (*info)->mount_options = xmalloc0 ((ಅಂಕೆ_mount_options + 1) * ಗಾತ್ರ (ಅಕ್ಷರ *));

  // Copy mount options to info struct
  ಗಾಗಿ (size_t i = 0; i < ಅಂಕೆ_mount_options; i++)
    (*info)->mount_options[i] = xstrdup (mount_options_list[i].name);
}

ಪೂರ್ಣಾಂಕ
libcrun_container_get_features (libcrun_context_t *context, ರಚನೆ features_info_s **info, libcrun_error_t *err arg_ಬಳಸದ)
{
  // Allocate memory for the features_info_s structure
  size_t ಅಂಕೆ_namspaces = ಗಾತ್ರ (namespaces) / ಗಾತ್ರ (namespaces[0]);
  size_t ಅಂಕೆ_operators = ಗಾತ್ರ (operators) / ಗಾತ್ರ (operators[0]);
  size_t ಅಂಕೆ_actions = ಗಾತ್ರ (actions) / ಗಾತ್ರ (actions[0]);
  size_t ಅಂಕೆ_hooks = ಗಾತ್ರ (hooks) / ಗಾತ್ರ (hooks[0]);
  size_t ಅಂಕೆ_archs = ಗಾತ್ರ (archs) / ಗಾತ್ರ (archs[0]);
  size_t ಅಂಕೆ_unsafe_annotations = ಗಾತ್ರ (potentially_unsafe_annotations) / ಗಾತ್ರ (potentially_unsafe_annotations[0]);
  cleanup_free ಅಕ್ಷರ **capabilities = ಖಾಲಿ;
  size_t ಅಂಕೆ_capabilities = 0;
  size_t ಅಂಕೆ_mempolicy_modes = ಗಾತ್ರ (mempolicy_modes) / ಗಾತ್ರ (mempolicy_modes[0]);
  size_t ಅಂಕೆ_mempolicy_flags = ಗಾತ್ರ (mempolicy_flags) / ಗಾತ್ರ (mempolicy_flags[0]);

  *info = xmalloc0 (ಗಾತ್ರ (ರಚನೆ features_info_s));

  // Hardcoded feature information
  (*info)->oci_ಆವೃತ್ತಿ_min = xstrdup ("1.0.0");
  (*info)->oci_ಆವೃತ್ತಿ_max = xstrdup ("1.1.0+dev");

  // Populate hooks
  populate_array_ಕ್ಷೇತ್ರ (&((*info)->hooks), hooks, ಅಂಕೆ_hooks);

  // Populate mount_options
  retrieve_mount_options (info);

  // Populate namespaces
  populate_array_ಕ್ಷೇತ್ರ (&((*info)->linux.namespaces), namespaces, ಅಂಕೆ_namspaces);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CAP
  // Populate capabilities
  populate_capabilities (*info, &capabilities, &ಅಂಕೆ_capabilities);
#ಆದರೆ_ಅಂತ್ಯ

  // Hardcode the values for cgroup
  (*info)->linux.cgroup.v1 = true;
  (*info)->linux.cgroup.v2 = true;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYSTEMD
  (*info)->linux.cgroup.systemd = true;
  (*info)->linux.cgroup.systemd_user = true;
#ಆದರೆ_ಅಂತ್ಯ

  // Put seccomp values
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SECCOMP
  (*info)->linux.seccomp.enabled = true;
  // Populate actions
  populate_array_ಕ್ಷೇತ್ರ (&((*info)->linux.seccomp.actions), actions, ಅಂಕೆ_actions);
  // Populate operators
  populate_array_ಕ್ಷೇತ್ರ (&((*info)->linux.seccomp.operators), operators, ಅಂಕೆ_operators);
  // Populate archs
  populate_array_ಕ್ಷೇತ್ರ (&((*info)->linux.seccomp.archs), archs, ಅಂಕೆ_archs);
#ಇಲ್ಲದಿದ್ದರೆ
  (*info)->linux.seccomp.enabled = false;
#ಆದರೆ_ಅಂತ್ಯ

  // Put values for apparmor and selinux
  (*info)->linux.apparmor.enabled = true;
  (*info)->linux.selinux.enabled = true;

  (*info)->linux.intel_rdt.enabled = true;

  (*info)->linux.net_devices.enabled = true;

  populate_array_ಕ್ಷೇತ್ರ (&((*info)->linux.ಸ್ಮರಣೆ_policy.ಬಹುಲಕ), mempolicy_modes, ಅಂಕೆ_mempolicy_modes);
  populate_array_ಕ್ಷೇತ್ರ (&((*info)->linux.ಸ್ಮರಣೆ_policy.flags), mempolicy_flags, ಅಂಕೆ_mempolicy_flags);

  // Put the values for mount extensions
  (*info)->linux.mount_ext.idmap.enabled = true;

  // Populate the values for annotations
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SECCOMP
  {
    ಸ್ಥಿರಾಂಕ ರಚನೆ scmp_ಆವೃತ್ತಿ *ಆವೃತ್ತಿ = seccomp_ಆವೃತ್ತಿ ();
    ಅಕ್ಷರ *ಆವೃತ್ತಿ_string = ಖಾಲಿ;

    xasprintf (&ಆವೃತ್ತಿ_string, "%u.%u.%u", ಆವೃತ್ತಿ->major, ಆವೃತ್ತಿ->minor, ಆವೃತ್ತಿ->micro);
    (*info)->annotations.io_github_seccomp_libseccomp_ಆವೃತ್ತಿ = ಆವೃತ್ತಿ_string;
  }
#ಆದರೆ_ಅಂತ್ಯ

  ಆದರೆ (context->handler_manager && handler_by_name (context->handler_manager, "wasm"))
    (*info)->annotations.ಸತತ_oci_crun_wasm = true;

#ಆದರೆ HAVE_CRIU
  (*info)->annotations.ಸತತ_oci_crun_checkpoint_enabled = true;
#ಆದರೆ_ಅಂತ್ಯ
  (*info)->annotations.ಸತತ_oci_crun_commit = GIT_ಆವೃತ್ತಿ;
  (*info)->annotations.ಸತತ_oci_crun_ಆವೃತ್ತಿ = PACKAGE_ಆವೃತ್ತಿ;

  populate_array_ಕ್ಷೇತ್ರ (&((*info)->potentially_unsafe_annotations), potentially_unsafe_annotations, ಅಂಕೆ_unsafe_annotations);

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ
features_ಕೂಡಿಸು_string (json_gen_ctx *json_gen, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
  json_gen_string (json_gen, ಕೀಲಿ, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಕೀಲಿ));
  json_gen_string (json_gen, ಮೌಲ್ಯ, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಮೌಲ್ಯ));
}

ಸ್ಥಿರ ಶೂನ್ಯ
features_ಕೂಡಿಸು_bool (json_gen_ctx *json_gen, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ, ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ)
{
  json_gen_string (json_gen, ಕೀಲಿ, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಕೀಲಿ));
  json_gen_bool (json_gen, ಮೌಲ್ಯ);
}

ಸ್ಥಿರ ಶೂನ್ಯ
features_ಕೂಡಿಸು_bool_str (json_gen_ctx *json_gen, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ, ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ)
{
  features_ಕೂಡಿಸು_string (json_gen, ಕೀಲಿ, ಮೌಲ್ಯ ? "true" : "false");
}

ಸ್ಥಿರ ಶೂನ್ಯ
features_ಕೂಡಿಸು_array (json_gen_ctx *json_gen, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ, ಅಕ್ಷರ **array)
{
  size_t i;

  json_gen_string (json_gen, ಕೀಲಿ, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಕೀಲಿ));
  json_gen_array_open (json_gen);
  ಆದರೆ (array)
    ಗಾಗಿ (i = 0; array[i] != ಖಾಲಿ; i++)
      json_gen_string (json_gen, array[i], ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (array[i]));
  json_gen_array_close (json_gen);
}

ಸ್ಥಿರ ಶೂನ್ಯ
features_ಕೂಡಿಸು_linux_info (json_gen_ctx *json_gen, ಸ್ಥಿರಾಂಕ ರಚನೆ linux_info_s *linux)
{
  json_gen_string (json_gen, "linux", ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("linux"));
  json_gen_map_open (json_gen);

  features_ಕೂಡಿಸು_array (json_gen, "namespaces", linux->namespaces);
  features_ಕೂಡಿಸು_array (json_gen, "capabilities", linux->capabilities);

  json_gen_string (json_gen, "cgroup", ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("cgroup"));
  json_gen_map_open (json_gen);
  features_ಕೂಡಿಸು_bool (json_gen, "v1", linux->cgroup.v1);
  features_ಕೂಡಿಸು_bool (json_gen, "v2", linux->cgroup.v2);
  features_ಕೂಡಿಸು_bool (json_gen, "systemd", linux->cgroup.systemd);
  features_ಕೂಡಿಸು_bool (json_gen, "systemdUser", linux->cgroup.systemd_user);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "seccomp", ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("seccomp"));
  json_gen_map_open (json_gen);
  features_ಕೂಡಿಸು_bool (json_gen, "enabled", linux->seccomp.enabled);
  ಆದರೆ (linux->seccomp.actions)
    features_ಕೂಡಿಸು_array (json_gen, "actions", linux->seccomp.actions);
  ಆದರೆ (linux->seccomp.operators)
    features_ಕೂಡಿಸು_array (json_gen, "operators", linux->seccomp.operators);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "apparmor", ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("apparmor"));
  json_gen_map_open (json_gen);
  features_ಕೂಡಿಸು_bool (json_gen, "enabled", linux->apparmor.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "selinux", ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("selinux"));
  json_gen_map_open (json_gen);
  features_ಕೂಡಿಸು_bool (json_gen, "enabled", linux->selinux.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "mountExtensions", ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("mountExtensions"));
  json_gen_map_open (json_gen);
  json_gen_string (json_gen, "idmap", ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("idmap"));
  json_gen_map_open (json_gen);
  features_ಕೂಡಿಸು_bool (json_gen, "enabled", linux->mount_ext.idmap.enabled);
  json_gen_map_close (json_gen);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "intelRdt", ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("intelRdt"));
  json_gen_map_open (json_gen);
  features_ಕೂಡಿಸು_bool (json_gen, "enabled", linux->intel_rdt.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "netDevices", ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("netDevices"));
  json_gen_map_open (json_gen);
  features_ಕೂಡಿಸು_bool (json_gen, "enabled", linux->net_devices.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "memoryPolicy", ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("memoryPolicy"));
  json_gen_map_open (json_gen);
  ಆದರೆ (linux->ಸ್ಮರಣೆ_policy.ಬಹುಲಕ)
    features_ಕೂಡಿಸು_array (json_gen, "modes", linux->ಸ್ಮರಣೆ_policy.ಬಹುಲಕ);
  ಆದರೆ (linux->ಸ್ಮರಣೆ_policy.flags)
    features_ಕೂಡಿಸು_array (json_gen, "flags", linux->ಸ್ಮರಣೆ_policy.flags);
  json_gen_map_close (json_gen);

  json_gen_map_close (json_gen);
}

ಸ್ಥಿರ ಶೂನ್ಯ
features_ಕೂಡಿಸು_annotations_info (json_gen_ctx *json_gen, ಸ್ಥಿರಾಂಕ ರಚನೆ annotations_info_s *annotation)
{
  json_gen_string (json_gen, "annotations", ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("annotations"));
  json_gen_map_open (json_gen);

  ಆದರೆ (! is_ಖಾಲಿಗಳು_string (annotation->io_github_seccomp_libseccomp_ಆವೃತ್ತಿ))
    features_ಕೂಡಿಸು_string (json_gen, "io.github.seccomp.libseccomp.version", annotation->io_github_seccomp_libseccomp_ಆವೃತ್ತಿ);

  features_ಕೂಡಿಸು_bool_str (json_gen, "org.opencontainers.runc.checkpoint.enabled", annotation->ಸತತ_oci_crun_checkpoint_enabled);
  features_ಕೂಡಿಸು_bool_str (json_gen, "run.oci.crun.checkpoint.enabled", annotation->ಸತತ_oci_crun_checkpoint_enabled);

  features_ಕೂಡಿಸು_string (json_gen, "run.oci.crun.commit", annotation->ಸತತ_oci_crun_commit);
  features_ಕೂಡಿಸು_string (json_gen, "run.oci.crun.version", annotation->ಸತತ_oci_crun_ಆವೃತ್ತಿ);

  features_ಕೂಡಿಸು_bool_str (json_gen, "run.oci.crun.wasm", annotation->ಸತತ_oci_crun_wasm);

  json_gen_map_close (json_gen);
}

ಪೂರ್ಣಾಂಕ
libcrun_container_get_features_json (libcrun_context_t *context, ಅಕ್ಷರ **ಹೊರಹೋಗು, libcrun_error_t *err)
{
  cleanup_struct_features ರಚನೆ features_info_s *info = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಫರ್ = ಖಾಲಿ;
  json_gen_ctx *json_gen;
  size_t ಬಫರ್_len;
  ಪೂರ್ಣಾಂಕ ret;

  *ಹೊರಹೋಗು = ಖಾಲಿ;

  ret = libcrun_container_get_features (context, &info, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (! json_gen_init (&json_gen, ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "cannot initialize JSON generator");

  json_gen_config (json_gen, json_gen_beautify, 1);

  json_gen_map_open (json_gen);

  features_ಕೂಡಿಸು_string (json_gen, "ociVersionMin", info->oci_ಆವೃತ್ತಿ_min);
  features_ಕೂಡಿಸು_string (json_gen, "ociVersionMax", info->oci_ಆವೃತ್ತಿ_max);
  features_ಕೂಡಿಸು_array (json_gen, "hooks", info->hooks);
  features_ಕೂಡಿಸು_array (json_gen, "mountOptions", info->mount_options);
  features_ಕೂಡಿಸು_linux_info (json_gen, &info->linux);
  features_ಕೂಡಿಸು_annotations_info (json_gen, &info->annotations);
  features_ಕೂಡಿಸು_array (json_gen, "potentiallyUnsafeConfigAnnotations", info->potentially_unsafe_annotations);

  json_gen_map_close (json_gen);

  json_gen_get_buf (json_gen, &ಬಫರ್, &ಬಫರ್_len);
  *ಹೊರಹೋಗು = xstrdup (ಬಫರ್);
  json_gen_free (json_gen);

  ಹಿಂತಿರುಗಿ 0;
}

/* Read the container status and fail if the container is not running.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
read_ಸ್ಥಿತಿ_require_running (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id,
                             libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ret = libcrun_read_container_ಸ್ಥಿತಿ (ಸ್ಥಿತಿ_2, context->state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_is_container_running (ಸ್ಥಿತಿ_2, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;
  ಆದರೆ (ret == 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "the container `%s` is not running", id);

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_container_pause (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  libcrun_container_ಸ್ಥಿತಿ_t ಸ್ಥಿತಿ_2 = {};

  ret = read_ಸ್ಥಿತಿ_require_running (context, id, &ಸ್ಥಿತಿ_2, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ libcrun_container_pause_linux (&ಸ್ಥಿತಿ_2, err);
}

ಪೂರ್ಣಾಂಕ
libcrun_container_unpause (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  libcrun_container_ಸ್ಥಿತಿ_t ಸ್ಥಿತಿ_2 = {};

  ret = read_ಸ್ಥಿತಿ_require_running (context, id, &ಸ್ಥಿತಿ_2, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ libcrun_container_unpause_linux (&ಸ್ಥಿತಿ_2, err);
}

/* Translate the public options struct into the internal representation used by
   the linux/CRIU layer.  Strings are borrowed (not copied).  */
ಸ್ಥಿರ ಶೂನ್ಯ
cr_options_from_ಬಹಿರಂಗ (libcrun_checkpoint_restore_t *dst, ರಚನೆ libcrun_checkpoint_restore_options_s *src)
{
  memset (dst, 0, ಗಾತ್ರ (*dst));
  dst->image_ಮಾರ್ಗ = (ಅಕ್ಷರ *) src->image_ಮಾರ್ಗ;
  dst->work_ಮಾರ್ಗ = (ಅಕ್ಷರ *) src->work_ಮಾರ್ಗ;
  dst->ಪೋಷಕ_ಮಾರ್ಗ = (ಅಕ್ಷರ *) src->ಪೋಷಕ_ಮಾರ್ಗ;
  dst->console_socket = src->console_socket;
  dst->lsm_profile = (ಅಕ್ಷರ *) src->lsm_profile;
  dst->lsm_mount_context = (ಅಕ್ಷರ *) src->lsm_mount_context;
  dst->leave_running = src->leave_running;
  dst->tcp_established = src->tcp_established;
  dst->tcp_close = src->tcp_close;
  dst->ext_unix_sk = src->ext_unix_sk;
  dst->shell_ಕೆಲಸ = src->shell_ಕೆಲಸ;
  dst->file_locks = src->file_locks;
  dst->pre_dump = src->pre_dump;
  dst->detach = src->detach;
  dst->manage_cgroups_ಬಹುಲಕ = src->manage_cgroups_ಬಹುಲಕ;
  dst->ಜಾಲ_ಬೀಗ_ವಿಧಾನ = src->ಜಾಲ_ಬೀಗ_ವಿಧಾನ;
}

ಪೂರ್ಣಾಂಕ
libcrun_container_checkpoint (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id,
                              ರಚನೆ libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು = context->state_ಬೇರು;
  libcrun_container_ಸ್ಥಿತಿ_t ಸ್ಥಿತಿ_2 = {};
  libcrun_checkpoint_restore_t cr;
  cleanup_container libcrun_container_t *container = ಖಾಲಿ;

  cr_options_from_ಬಹಿರಂಗ (&cr, cr_options);

  ret = read_ಸ್ಥಿತಿ_require_running (context, id, &ಸ್ಥಿತಿ_2, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = read_container_config_from_state (&container, state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  /* Needed to use the same paths (e.g. the shared empty directory used for
     masked paths) as the container runtime and restore do.  */
  container->context = context;

  ret = libcrun_container_checkpoint_linux (&ಸ್ಥಿತಿ_2, container, &cr, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (! (cr_options->leave_running || cr_options->pre_dump))
    ಹಿಂತಿರುಗಿ container_delete_ಆಂತರಿಕ (context, ಖಾಲಿ, ಖಾಲಿ, id, true, true, err);

  ಹಿಂತಿರುಗಿ 0;
}

/* The dummy process only keeps the cgroup populated until the container is restored there.
   Its cgroup is below the container cgroup, so once the container runs it can freeze or kill
   the dummy process: never depend on it after that point.  */
ಸ್ಥಿರ ಶೂನ್ಯ
restore_proxy_process (ಪೂರ್ಣಾಂಕ proxy_pid_pipe)
{
  ಅಕ್ಷರ c;

  /* Wait to be killed by the parent, or for the parent to go away.  */
  ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (proxy_pid_pipe, &c, ಗಾತ್ರ (c)));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
restore_terminate_proxy_process (pid_t proxy_pid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *proxy_cgroup, pid_t container_pid, ಪೂರ್ಣಾಂಕ cgroup_manager, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ಬಹುಲಕ;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (cgroup_manager == CGROUP_MANAGER_SYSTEMD)
    {
      ಅಕ್ಷರ ready_str[64];

      ret = snprintf (ready_str, ಗಾತ್ರ (ready_str), "MAINPID=%d", container_pid);
      ಆದರೆ (UNLIKELY (ret >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (ready_str)))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = send_sd_notify (ready_str, err);
      /* Do not fail on errors.  */
      ಆದರೆ (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ ಬಹುಲಕ;

  /* SIGKILL terminates a process frozen with cgroup v2, while the cgroup v1 freezer does not
     deliver it until the cgroup is thawed, so do not wait for it there.  */
  kill (proxy_pid, SIGKILL);
  ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (waitpid (proxy_pid, ಖಾಲಿ, ಬಹುಲಕ == CGROUP_ಬಹುಲಕ_UNIFIED ? 0 : WNOHANG));

  ಹಿಂತಿರುಗಿ destroy_cgroup_ಮಾರ್ಗ (proxy_cgroup, ಬಹುಲಕ, err);
}

ಪೂರ್ಣಾಂಕ
libcrun_container_restore (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id arg_ಬಳಸದ,
                           ರಚನೆ libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  cleanup_cgroup_ಸ್ಥಿತಿ ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ = ಖಾಲಿ;
  cleanup_container libcrun_container_t *container = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ proxy_pid_pipe0 = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ proxy_pid_pipe1 = -1;
  runtime_spec_schema_config_schema *def;
  libcrun_container_ಸ್ಥಿತಿ_t ಸ್ಥಿತಿ_2 = {};
  ರಚನೆ libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  libcrun_checkpoint_restore_t cr;
  cleanup_pid pid_t proxy_pid = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ seccomp_fd = -1;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *seccomp_bpf_ದತ್ತಾಂಶ;
  ಪೂರ್ಣಾಂಕ proxy_pid_pipe[2];
  ಪೂರ್ಣಾಂಕ cgroup_manager;
  uid_t ಬೇರು_uid = -1;
  gid_t ಬೇರು_gid = -1;
  ಪೂರ್ಣಾಂಕ ret;

  cr_options_from_ಬಹಿರಂಗ (&cr, cr_options);

  container = libcrun_container_ತುಂಬು_from_file ("config.json", err);
  ಆದರೆ (container == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ -1;

  container->context = context;
  def = container->container_def;

  ret = check_config_file (def, context, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_ಸ್ಥಿತಿ_check_directories (context->state_ಬೇರು, context->id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_copy_config_file (context->id, context->state_ಬೇರು, container, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು fail;

  /* CRIU restores the seccomp filters of the checkpointed processes, but
     exec loads the filter from the seccomp.bpf file in the state directory,
     so it must be generated again for the restored container.  */
  seccomp_bpf_ದತ್ತಾಂಶ = ಕಂಡುಹಿಡಿ_annotation (container, "run.oci.seccomp_bpf_data");

  ret = ಸಿದ್ಧತೆ_seccomp (container, seccomp_bpf_ದತ್ತಾಂಶ, &seccomp_gen_ctx, &seccomp_fd, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು fail;

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_ದತ್ತಾಂಶ, &seccomp_gen_ctx, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು fail;
  close_and_reset (&seccomp_fd);

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  ಆದರೆ (context->systemd_cgroup)
    cgroup_manager = CGROUP_MANAGER_SYSTEMD;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (context->force_no_cgroup)
    cgroup_manager = CGROUP_MANAGER_DISABLED;

  def = container->container_def;

  /* If we are root (either on the host or in a namespace),
   * then chown the cgroup to root in the container user namespace. */
  get_ಬೇರು_ಒಳ_the_userns (def, container->host_uid, container->host_gid, &ಬೇರು_uid, &ಬೇರು_gid);

  /* If the root in the container is different than the current root user, attempt to chown
     the std streams before entering the user namespace.  Otherwise we might lose access
     to the user (as it is not mapped in the user namespace) and cannot chown them.  */
  ಆದರೆ (ಬೇರು_uid > 0 || ಬೇರು_gid > 0)
    {
      ret = maybe_chown_std_streams (ಬೇರು_uid, ಬೇರು_gid, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹೋಗು fail;
    }

  /* CRIU runs as a child process, so the restored processes would inherit the session keyring.  */
  ret = ಸಿದ್ಧತೆ_container_keyring (container, context, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  {
    cleanup_close ಪೂರ್ಣಾಂಕ cgroup_dirfd = -1;
    bool needs_proxy_process;
    cleanup_cgroup_preenter ರಚನೆ libcrun_cgroup_ವಾದಗಳು cg = {
      .resources = def->linux ? def->linux->resources : ಖಾಲಿ,
      .annotations = container->annotations,
      .cgroup_ಮಾರ್ಗ = def->linux ? def->linux->cgroups_ಮಾರ್ಗ : "",
      .manager = cgroup_manager,
      .ಬೇರು_uid = ಬೇರು_uid,
      .ಬೇರು_gid = ಬೇರು_gid,
      .id = context->id,
      .state_ಬೇರು = context->state_ಬೇರು,
      .container = container,
    };

    /* The CRIU restore code uses bundle, rootfs and cgroup_path of status.   The cgroup_path is set later.  */
    ಸ್ಥಿತಿ_2.bundle = (ಅಕ್ಷರ *) context->bundle;
    ಸ್ಥಿತಿ_2.rootfs = def->ಬೇರು->ಮಾರ್ಗ;

    ret = libcrun_cgroup_preenter (&cg, &cgroup_dirfd, err);
    ಆದರೆ (UNLIKELY (ret < 0))
      ಹೋಗು fail;

    needs_proxy_process = cgroup_dirfd < 0;

    /* If the target cgroup was already created (with cgroupfs), we can restore the container directly there.  */
    ಆದರೆ (! needs_proxy_process)
      {
        cleanup_free ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ = ಖಾಲಿ;

        ret = get_cgroup_dirfd_ಮಾರ್ಗ (cgroup_dirfd, &cgroup_ಮಾರ್ಗ, err);
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹೋಗು fail;

        /* Complete the configuration for the cgroup before the container is
           restored there: CRIU resumes the restored processes before it
           returns, so the device filter and the limits must already be in
           place.  There is no process to move, as CRIU is run in the cgroup,
           and the controllers were enabled by precreate_cgroup.  */
        cg.joined = true;

        ret = libcrun_cgroup_enter (&cg, &cgroup_ಸ್ಥಿತಿ, err);
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹೋಗು fail;

        /* Restore the container directly in the desired cgroup.  */
        ಸ್ಥಿತಿ_2.cgroup_ಮಾರ್ಗ = cgroup_ಮಾರ್ಗ;

        ret = libcrun_container_restore_linux (&ಸ್ಥಿತಿ_2, container, &cr, err);
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹೋಗು fail;

        /* Now that the container is restored, keep any signal for it, so that
           it is forwarded to the init by wait_for_process, rather than acted
           upon by crun.  This cannot be done earlier, as CRIU, which is run as
           a child process, would inherit the blocked signals.  */
        ret = ಖಂಡ_ಸಂಕೇತಗಳು (err);
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹೋಗು fail;

        /* Use the container first process PID to finalize the cgroup.  */
        cg.pid = ಸ್ಥಿತಿ_2.pid;
      }
    ಇಲ್ಲದಿದ್ದರೆ
      {
        /* When using systemd, we need a process first that must exist before a cgroup is created.
           Create a dummy process that sits and waits until the container is restored.  */
        ret = pipe2 (proxy_pid_pipe, O_CLOEXEC);
        ಆದರೆ (UNLIKELY (ret < 0))
          {
            ret = crun_make_error (err, errno, "pipe");
            ಹೋಗು fail;
          }

        /* Only used for auto cleanup.  */
        proxy_pid_pipe0 = proxy_pid_pipe[0];
        proxy_pid_pipe1 = proxy_pid_pipe[1];

        proxy_pid = fork ();
        ಆದರೆ (UNLIKELY (proxy_pid < 0))
          {
            ret = crun_make_error (err, errno, "fork");
            ಹೋಗು fail;
          }

        ಆದರೆ (proxy_pid == 0)
          {
            close_and_reset (&proxy_pid_pipe1);

            restore_proxy_process (proxy_pid_pipe0);
            ಸುರಕ್ಷಿತ_exit (EXIT_SUCCESS);
          }

        close_and_reset (&proxy_pid_pipe0);

        /* Use the dummy process PID as the PID to setup the cgroup.  */
        cg.pid = proxy_pid;

        /* Complete the configuration for the cgroup.  It contains only the dummy process.  */
        ret = libcrun_cgroup_enter (&cg, &cgroup_ಸ್ಥಿತಿ, err);
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹೋಗು fail;
      }

    ret = libcrun_cgroup_enter_finalize (&cg, cgroup_ಸ್ಥಿತಿ, err);
    ಆದರೆ (UNLIKELY (ret < 0))
      ಹೋಗು fail;

    /* When using a dummy process, the container is restored here once the cgroup is configured.  */
    ಆದರೆ (needs_proxy_process)
      {
        cleanup_free ಅಕ್ಷರ *ಗುರಿ_cgroup = ಖಾಲಿ;
        cleanup_free ಅಕ್ಷರ *proxy_cgroup = ಖಾಲಿ;

        ret = libcrun_get_cgroup_process (proxy_pid, &ಗುರಿ_cgroup, false, err);
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹೋಗು fail;

        /* Move the dummy process to a sub-cgroup.  */
        ret = append_paths (&proxy_cgroup, err, ಗುರಿ_cgroup, ".proxy-crun", ಖಾಲಿ);
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹೋಗು fail;

        ret = libcrun_move_process_ಗಮ್ಯ_cgroup (proxy_pid, 0, proxy_cgroup, true, err);
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹೋಗು fail;

        /* Restore the container in the same cgroup where the dummy process was.  */
        ಸ್ಥಿತಿ_2.cgroup_ಮಾರ್ಗ = ಗುರಿ_cgroup;

        ret = libcrun_container_restore_linux (&ಸ್ಥಿತಿ_2, container, &cr, err);
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹೋಗು fail;

        /* See the comment for the other libcrun_container_restore_linux call.  */
        ret = ಖಂಡ_ಸಂಕೇತಗಳು (err);
        ಆದರೆ (UNLIKELY (ret < 0))
          ಹೋಗು fail;

        /* The container is running, so do not fail because of the dummy process.  */
        ret = restore_terminate_proxy_process (proxy_pid, proxy_cgroup, ಸ್ಥಿತಿ_2.pid, cgroup_manager, err);
        ಆದರೆ (UNLIKELY (ret < 0))
          crun_error_release (err);
        proxy_pid = -1;

        close_and_reset (&proxy_pid_pipe1);
      }
  }

  context->detach = cr_options->detach;
  ret = write_container_ಸ್ಥಿತಿ (container, context, ಸ್ಥಿತಿ_2.pid, cgroup_ಸ್ಥಿತಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು fail;

  {
    /* Wait the same way run and exec do: apart from waiting for the container
       init, this writes the pid file, forwards any signal to the init, and
       reaps every child, not only the init.  */
    ರಚನೆ wait_for_process_ವಾದಗಳು ವಾದಗಳು = {
      .pid = ಸ್ಥಿತಿ_2.pid,
      .context = context,
      .terminal_fd = -1,
      .notify_socket = -1,
      .container_ready_fd = ಖಾಲಿ,
      .seccomp_notify_fd = -1,
      .seccomp_notify_plugins = ಖಾಲಿ,
    };

    ಹಿಂತಿರುಗಿ wait_for_process (&ವಾದಗಳು, err);
  }

fail:
  /* Once restored, the container is running.  Without a status file nothing
     can reach it anymore, so do not leave it behind.  */
  ಆದರೆ (ಸ್ಥಿತಿ_2.pid > 0)
    {
      kill (ಸ್ಥಿತಿ_2.pid, SIGKILL);
      waitpid_ignore_stopped (ಸ್ಥಿತಿ_2.pid, ಖಾಲಿ, 0);
    }
  ಆದರೆ (cgroup_ಸ್ಥಿತಿ)
    {
      libcrun_error_t tmp_err = ಖಾಲಿ;
      libcrun_cgroup_destroy (cgroup_ಸ್ಥಿತಿ, &tmp_err);
      crun_error_release (&tmp_err);
    }
  {
    libcrun_error_t tmp_err = ಖಾಲಿ;

    ಆದರೆ (libcrun_container_delete_ಸ್ಥಿತಿ (context->state_ಬೇರು, context->id, &tmp_err) < 0)
      crun_error_release (&tmp_err);
  }
  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
libcrun_container_read_pids (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  cleanup_cgroup_ಸ್ಥಿತಿ ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ = ಖಾಲಿ;
  cleanup_container_ಸ್ಥಿತಿ libcrun_container_ಸ್ಥಿತಿ_t ಸ್ಥಿತಿ_2 = {};
  ಪೂರ್ಣಾಂಕ ret;

  ret = libcrun_read_container_ಸ್ಥಿತಿ (&ಸ್ಥಿತಿ_2, context->state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (ಸ್ಥಿತಿ_2.cgroup_ಮಾರ್ಗ == ಖಾಲಿ || ಸ್ಥಿತಿ_2.cgroup_ಮಾರ್ಗ[0] == '\0')
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "the container is not using cgroups");

  cgroup_ಸ್ಥಿತಿ = libcrun_cgroup_make_ಸ್ಥಿತಿ (&ಸ್ಥಿತಿ_2);

  ಹಿಂತಿರುಗಿ libcrun_cgroup_read_pids (cgroup_ಸ್ಥಿತಿ, recurse, pids, err);
}

ಪೂರ್ಣಾಂಕ
libcrun_container_list_json (libcrun_context_t *context, ಅಕ್ಷರ **ಹೊರಹೋಗು, libcrun_error_t *err)
{
  libcrun_container_list_t *list = ಖಾಲಿ, *it;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *content = ಖಾಲಿ;
  json_gen_ctx *gen = ಖಾಲಿ;
  size_t len;
  ಪೂರ್ಣಾಂಕ ret;

  *ಹೊರಹೋಗು = ಖಾಲಿ;

  ret = libcrun_get_containers_list (&list, context->state_ಬೇರು, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (! json_gen_init (&gen, ಖಾಲಿ))
    {
      ret = crun_make_error (err, 0, "cannot allocate json generator");
      ಹೋಗು ನಿರ್ಗಮಿಸು;
    }

  json_gen_config (gen, json_gen_beautify, 1);
  json_gen_array_open (gen);

  ಗಾಗಿ (it = list; it; it = it->ಮುಂದಿನ)
    {
      libcrun_container_ಸ್ಥಿತಿ_t ಸ್ಥಿತಿ_2;
      ಪೂರ್ಣಾಂಕ running = 0;
      ಪೂರ್ಣಾಂಕ pid;
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *container_ಸ್ಥಿತಿ = ಖಾಲಿ;

      ret = libcrun_read_container_ಸ್ಥಿತಿ (&ಸ್ಥಿತಿ_2, context->state_ಬೇರು, it->name, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹೋಗು ನಿರ್ಗಮಿಸು;

      pid = ಸ್ಥಿತಿ_2.pid;

      ret = libcrun_get_container_state_string (it->name, &ಸ್ಥಿತಿ_2, context->state_ಬೇರು, &container_ಸ್ಥಿತಿ,
                                                &running, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          ಮುಂದುವರಿ;
        }

      ಆದರೆ (! running)
        pid = 0;

      json_gen_map_open (gen);
      json_gen_string (gen, "id", ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("id"));
      json_gen_string (gen, it->name, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (it->name));
      json_gen_string (gen, "pid", ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("pid"));
      map_int (gen, pid);
      json_gen_string (gen, "status", ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("status"));
      json_gen_string (gen, container_ಸ್ಥಿತಿ, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (container_ಸ್ಥಿತಿ));
      json_gen_string (gen, "bundle", ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("bundle"));
      json_gen_string (gen, ಸ್ಥಿತಿ_2.bundle, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಸ್ಥಿತಿ_2.bundle));
      json_gen_string (gen, "created", ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("created"));
      json_gen_string (gen, ಸ್ಥಿತಿ_2.created, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಸ್ಥಿತಿ_2.created));
      json_gen_string (gen, "owner", ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("owner"));
      json_gen_string (gen, ಸ್ಥಿತಿ_2.owner, ಸುರಕ್ಷಿತ_strlen (ಸ್ಥಿತಿ_2.owner));
      json_gen_map_close (gen);

      libcrun_free_container_ಸ್ಥಿತಿ (&ಸ್ಥಿತಿ_2);
    }

  json_gen_array_close (gen);
  ಆದರೆ (json_gen_get_buf (gen, &content, &len) != json_gen_ಸ್ಥಿತಿ_ok)
    {
      ret = libcrun_make_error (err, 0, "cannot generate json list");
      ಹೋಗು ನಿರ್ಗಮಿಸು;
    }

  *ಹೊರಹೋಗು = xstrdup (content);
  ret = 0;

ನಿರ್ಗಮಿಸು:
  ಆದರೆ (list)
    libcrun_free_containers_list (list);
  ಆದರೆ (gen)
    json_gen_free (gen);

  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
libcrun_write_json_containers_list (libcrun_context_t *context, FILE *ಹೊರಹೋಗು, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *content = ಖಾಲಿ;
  size_t len;
  ಪೂರ್ಣಾಂಕ ret;

  ret = libcrun_container_list_json (context, &content, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (content);
  ಆದರೆ (len && fwrite (content, 1, len, ಹೊರಹೋಗು) != len)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "error writing to file");

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_container_update_intel_rdt (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ರಚನೆ libcrun_intel_rdt_update *update, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;

  ret = read_container_config_from_state (&container, context->state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ libcrun_update_intel_rdt (id, container, update->l3_cache_schema, update->mem_bw_schema, update->schemata, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libcrun_container_ಕೂಡಿಸು_or_remove_mounts_from_file (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, bool ಕೂಡಿಸು, libcrun_error_t *err)
{
  cleanup_custom_handler_instance ರಚನೆ custom_handler_instance_s *custom_handler = ಖಾಲಿ;
  cleanup_container libcrun_container_t *container = ಖಾಲಿ;
  cleanup_free runtime_spec_schema_defs_mount **mounts = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು = context->state_ಬೇರು;
  cleanup_free parser_error parser_err = ಖಾಲಿ;
  ರಚನೆ parser_context pctx = { 0, stderr };
  libcrun_container_ಸ್ಥಿತಿ_t ಸ್ಥಿತಿ_2 = {};
  cleanup_free ಅಕ್ಷರ *content = ಖಾಲಿ;
  json_object *doc = ಖಾಲಿ;
  size_t n_mounts = 0, len, i;
  ಪೂರ್ಣಾಂಕ ret = 1;

  ret = read_all_file (file, &content, &len, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_read_container_ಸ್ಥಿತಿ (&ಸ್ಥಿತಿ_2, state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = read_container_config_from_state (&container, state_ಬೇರು, id, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = parse_json_file (&doc, content, &pctx, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (! json_object_is_type (doc, json_type_array))
    {
      ret = crun_make_error (err, 0, "mounts must be an array");
      ಹೋಗು cleanup;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      n_mounts = json_object_array_ಉದ್ದಳತೆ (doc);

      mounts = xmalloc0 ((n_mounts + 1) * ಗಾತ್ರ (*mounts));
      ಗಾಗಿ (i = 0; i < n_mounts; i++)
        {
          mounts[i] = make_runtime_spec_schema_defs_mount (json_object_array_get_idx (doc, i), &pctx, &parser_err);
          ಆದರೆ (mounts[i] == ಖಾಲಿ)
            {
              ret = crun_make_error (err, 0, "cannot parse mount: %s", parser_err);
              ಹೋಗು cleanup;
            }
        }
    }

  ಆದರೆ (ಕೂಡಿಸು)
    ret = libcrun_make_runtime_mounts (container, &ಸ್ಥಿತಿ_2, mounts, n_mounts, err);
  ಇಲ್ಲದಿದ್ದರೆ
    ret = libcrun_destroy_runtime_mounts (container, &ಸ್ಥಿತಿ_2, mounts, n_mounts, err);

cleanup:
  ಆದರೆ (doc)
    json_object_put (doc);
  ಆದರೆ (mounts)
    ಗಾಗಿ (i = 0; i < n_mounts; i++)
      free_runtime_spec_schema_defs_mount (mounts[i]);
  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
libcrun_container_ಕೂಡಿಸು_mounts_from_file (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ libcrun_container_ಕೂಡಿಸು_or_remove_mounts_from_file (context, id, file, true, err);
}

ಪೂರ್ಣಾಂಕ
libcrun_container_remove_mounts_from_file (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ libcrun_container_ಕೂಡಿಸು_or_remove_mounts_from_file (context, id, file, false, err);
}
