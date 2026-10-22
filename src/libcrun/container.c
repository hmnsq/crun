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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ocispec/runtime_spec_schema_config_schema.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "container.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "json_gen_utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "seccomp.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mempolicy.h"
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SECCOMP
#  include <seccomp.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ "scheduler.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "seccomp_notify.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "custom-handler.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <argp.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sched.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/wait.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "status.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mount_flags.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "linux.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "terminal.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "io_priority.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup-utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/prctl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/auxv.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/signalfd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/epoll.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/socket.h>
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CAP
#  include <sys/capability.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/ioctl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <termios.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <grp.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <git-version.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYSTEMD
#  include <systemd/sd-daemon.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <json-c/json.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ocispec/json_common.h>

ਗਿਣਤੀ
{
  SYNC_SOCKET_SYNC_ਸੁਨੇਹਾ,
  SYNC_SOCKET_ERROR_ਸੁਨੇਹਾ,
  SYNC_SOCKET_WARNING_ਸੁਨੇਹਾ,
  SYNC_SOCKET_DEBUG_ਸੁਨੇਹਾ,
};

ਬਣਤਰ container_entrypoint_s
{
  libcrun_container_t *container;
  libcrun_context_t *context;
  ਪੂਰਨ has_terminal_socket_pair;
  ਪੂਰਨ terminal_socketpair[2];

  /* Used by log_write_to_sync_socket.  */
  ਪੂਰਨ sync_socket;

  ਪੂਰਨ seccomp_fd;
  ਪੂਰਨ seccomp_receiver_fd;
  ਪੂਰਨ console_socket_fd;

  ਪੂਰਨ hooks_ਬਾਹਰਲਾ_fd;
  ਪੂਰਨ hooks_err_fd;

  /* Absolute bundle path, to be reported to hooks run in the container.  */
  ਅਚਲ ਅੱਖਰ *bundle;

  ਬਣਤਰ custom_handler_instance_s *custom_handler;
};

ਬਣਤਰ sync_socket_ਸੁਨੇਹਾ_s
{
  ਪੂਰਨ type;
  ਪੂਰਨ error_ਮੁੱਲ;
  ਅੱਖਰ ਸੁਨੇਹਾ_2[512];
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ runtime_spec_schema_defs_hook hook;

// linux hooks
ਅੱਖਰ *hooks[] = {
  "prestart",
  "createRuntime",
  "createContainer",
  "startContainer",
  "poststart",
  "poststop"
};

// linux namespaces
ਸਥਿਰ ਅੱਖਰ *namespaces[] = {
  "cgroup",
  "ipc",
  "mount",
  "network",
  "pid",
  "user",
  "uts"
};

ਸਥਿਰ ਅੱਖਰ *actions[] = {
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

ਸਥਿਰ ਅੱਖਰ *operators[] = {
  "SCMP_CMP_NE",
  "SCMP_CMP_LT",
  "SCMP_CMP_LE",
  "SCMP_CMP_EQ",
  "SCMP_CMP_GE",
  "SCMP_CMP_GT",
  "SCMP_CMP_MASKED_EQ",
};

ਸਥਿਰ ਅੱਖਰ *archs[] = {
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

ਸਥਿਰ ਅੱਖਰ *mempolicy_modes[] = {
  "MPOL_DEFAULT",
  "MPOL_PREFERRED",
  "MPOL_BIND",
  "MPOL_INTERLEAVE",
  "MPOL_LOCAL",
  "MPOL_PREFERRED_MANY",
  "MPOL_WEIGHTED_INTERLEAVE"
};

ਸਥਿਰ ਅੱਖਰ *mempolicy_flags[] = {
  "MPOL_F_NUMA_BALANCING",
  "MPOL_F_RELATIVE_NODES",
  "MPOL_F_STATIC_NODES"
};

ਸਥਿਰ ਅੱਖਰ *potentially_unsafe_annotations[] = {
  "module.wasm.image/variant",
  "io.kubernetes.cri.container-type",
  "run.oci.",
  "org.criu.",
  "krun.",
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYNC_SOCKET_ਸੁਨੇਹਾ_LEN(x, l) (offsetof (ਬਣਤਰ sync_socket_ਸੁਨੇਹਾ_s, ਸੁਨੇਹਾ_2) + l)

ਸਥਿਰ ਪੂਰਨ
sync_socket_write_ਸੁਨੇਹਾ (ਪੂਰਨ fd, ਪੂਰਨ verbosity, ਪੂਰਨ err_ਮੁੱਲ, ਅਚਲ ਅੱਖਰ *log_ਸੁਨੇਹਾ)
{
  ਪੂਰਨ ret;
  size_t err_len;
  ਬਣਤਰ sync_socket_ਸੁਨੇਹਾ_s ਸੁਨੇਹਾ;
  ਚੋਣ (verbosity)
    {
    ਹਾਲਤ LIBCRUN_VERBOSITY_DEBUG:
      ਸੁਨੇਹਾ.type = SYNC_SOCKET_DEBUG_ਸੁਨੇਹਾ;
      ਰੋਕੋ;
    ਹਾਲਤ LIBCRUN_VERBOSITY_WARNING:
      ਸੁਨੇਹਾ.type = SYNC_SOCKET_WARNING_ਸੁਨੇਹਾ;
      ਰੋਕੋ;
    ਹਾਲਤ LIBCRUN_VERBOSITY_ERROR:
      ਸੁਨੇਹਾ.type = SYNC_SOCKET_ERROR_ਸੁਨੇਹਾ;
      ਰੋਕੋ;
    }
  ਸੁਨੇਹਾ.error_ਮੁੱਲ = err_ਮੁੱਲ;

  ਜੇ (fd < 0)
    ਵਾਪਸ 0;

  err_len = ਲੜੀ_ਲੰਬਾਈ (log_ਸੁਨੇਹਾ);
  ਜੇ (err_len >= ਆਕਾਰ (ਸੁਨੇਹਾ.ਸੁਨੇਹਾ_2))
    err_len = ਆਕਾਰ (ਸੁਨੇਹਾ.ਸੁਨੇਹਾ_2) - 1;

  ਸਮ੍ਰਿਤੀ_ਨਕਲ (ਸੁਨੇਹਾ.ਸੁਨੇਹਾ_2, log_ਸੁਨੇਹਾ, err_len);
  ਸੁਨੇਹਾ.ਸੁਨੇਹਾ_2[err_len] = '\0';

  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (fd, &ਸੁਨੇਹਾ, SYNC_SOCKET_ਸੁਨੇਹਾ_LEN (ਸੁਨੇਹਾ, err_len + 1)));
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ -1;

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
sync_socket_write_error (ਪੂਰਨ fd, libcrun_error_t *ਬਾਹਰਲਾ_err)
{
  ਜੇ (fd < 0)
    ਵਾਪਸ 0;
  ਵਾਪਸ sync_socket_write_ਸੁਨੇਹਾ (fd, false, (*ਬਾਹਰਲਾ_err)->ਸਥਿਤੀ, (*ਬਾਹਰਲਾ_err)->ਸੁਨੇਹਾ);
}

ਸਥਿਰ ਖਾਲੀ
log_write_ਮੰਜ਼ਿਲ_sync_socket (ਪੂਰਨ errno_, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ਪੂਰਨ verbosity, ਖਾਲੀ *arg)
{
  ਬਣਤਰ container_entrypoint_s *entrypoint_ਦਲੀਲਾਂ = arg;
  ਪੂਰਨ fd = entrypoint_ਦਲੀਲਾਂ->sync_socket;

  ਜੇ (fd < 0)
    ਵਾਪਸ;

  ਜੇ (sync_socket_write_ਸੁਨੇਹਾ (fd, verbosity, errno_, ਸੁਨੇਹਾ) < 0)
    log_write_ਮੰਜ਼ਿਲ_stderr (errno_, ਸੁਨੇਹਾ, verbosity, arg);
}

ਸਥਿਰ bool
is_ਸਮ੍ਰਿਤੀ_ਹੱਦਬੰਦੀ_too_ਨੀਵਾਂ (runtime_spec_schema_config_schema *def)
{
  ਅਚਲ ਲੰਮਾ ਸਮ੍ਰਿਤੀ_ਹੱਦਬੰਦੀ_too_ਨੀਵਾਂ = 1024 * 1024;

  ਜੇ (def->linux == ਨਿਰਾ || def->linux->resources == ਨਿਰਾ)
    ਵਾਪਸ false;

  ਜੇ (def->linux->resources->ਸਮ੍ਰਿਤੀ
      && def->linux->resources->ਸਮ੍ਰਿਤੀ->ਹੱਦਬੰਦੀ_present
      && def->linux->resources->ਸਮ੍ਰਿਤੀ->ਹੱਦਬੰਦੀ < ਸਮ੍ਰਿਤੀ_ਹੱਦਬੰਦੀ_too_ਨੀਵਾਂ)
    ਵਾਪਸ true;

  ਜੇ (def->linux->resources->unified)
    {
      size_t i;

      ਲਈ (i = 0; i < def->linux->resources->unified->len; i++)
        ਜੇ (strcmp (def->linux->resources->unified->ਕੁੰਜੀਆਂ[i], "memory.max") == 0)
          {
            ਲੰਮਾ ਹੱਦਬੰਦੀ;

            errno = 0;
            ਹੱਦਬੰਦੀ = strtol (def->linux->resources->unified->ਮੁੱਲਾਂ[i], ਨਿਰਾ, 10);
            ਜੇ (errno != 0)
              ਵਾਪਸ false;
            ਜੇ (ਹੱਦਬੰਦੀ < ਸਮ੍ਰਿਤੀ_ਹੱਦਬੰਦੀ_too_ਨੀਵਾਂ)
              ਵਾਪਸ true;
          }
    }

  ਵਾਪਸ false;
}

ਸਥਿਰ ਪੂਰਨ
sync_socket_wait_sync (libcrun_context_t *context, ਪੂਰਨ fd, bool flush, libcrun_error_t *err)
{
  ਬਣਤਰ sync_socket_ਸੁਨੇਹਾ_s ਸੁਨੇਹਾ;

  ਜੇ (fd < 0)
    ਵਾਪਸ 0;

  ਜਦੋਂ (true)
    {
      ਪੂਰਨ ret;

      ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (fd, &ਸੁਨੇਹਾ, ਆਕਾਰ (ਸੁਨੇਹਾ)));
      ਜੇ (UNLIKELY (ret < 0))
        {
          ਜੇ (flush)
            ਵਾਪਸ 0;
          ਵਾਪਸ crun_make_error (err, errno, "read from sync socket");
        }

      ਜੇ (ret == 0)
        {
          ਜੇ (flush)
            ਵਾਪਸ 0;

          ਵਾਪਸ crun_make_error (err, 0, "read from the init process");
        }

      ਜੇ (! flush && ਸੁਨੇਹਾ.type == SYNC_SOCKET_SYNC_ਸੁਨੇਹਾ)
        ਵਾਪਸ 0;
      ਨਹੀਂਤਾਂ ਜੇ (ਸੁਨੇਹਾ.type == SYNC_SOCKET_DEBUG_ਸੁਨੇਹਾ)
        {
          ਜੇ (context)
            context->output_handler (ਸੁਨੇਹਾ.error_ਮੁੱਲ, ਸੁਨੇਹਾ.ਸੁਨੇਹਾ_2, LIBCRUN_VERBOSITY_DEBUG, context->output_handler_arg);
          ਜਾਰੀ;
        }
      ਨਹੀਂਤਾਂ ਜੇ (ਸੁਨੇਹਾ.type == SYNC_SOCKET_WARNING_ਸੁਨੇਹਾ)
        {
          ਜੇ (context)
            context->output_handler (ਸੁਨੇਹਾ.error_ਮੁੱਲ, ਸੁਨੇਹਾ.ਸੁਨੇਹਾ_2, LIBCRUN_VERBOSITY_WARNING, context->output_handler_arg);
          ਜਾਰੀ;
        }
      ਨਹੀਂਤਾਂ ਜੇ (ਸੁਨੇਹਾ.type == SYNC_SOCKET_ERROR_ਸੁਨੇਹਾ)
        ਵਾਪਸ crun_make_error (err, ਸੁਨੇਹਾ.error_ਮੁੱਲ, "%s", ਸੁਨੇਹਾ.ਸੁਨੇਹਾ_2);
    }
}

ਸਥਿਰ ਪੂਰਨ
sync_socket_send_sync (ਪੂਰਨ fd, bool flush_errors, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  ਬਣਤਰ sync_socket_ਸੁਨੇਹਾ_s ਸੁਨੇਹਾ = {
    0,
  };
  ਸੁਨੇਹਾ.type = SYNC_SOCKET_SYNC_ਸੁਨੇਹਾ;

  ਜੇ (fd < 0)
    ਵਾਪਸ 0;

  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (fd, &ਸੁਨੇਹਾ, SYNC_SOCKET_ਸੁਨੇਹਾ_LEN (ਸੁਨੇਹਾ, 0)));
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਜੇ (flush_errors)
        {
          ਪੂਰਨ saved_errno = errno;
          ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (fd, &ਸੁਨੇਹਾ, ਆਕਾਰ (ਸੁਨੇਹਾ)));
          ਜੇ (ret >= 0 && ਸੁਨੇਹਾ.type == SYNC_SOCKET_ERROR_ਸੁਨੇਹਾ)
            ਵਾਪਸ crun_make_error (err, ਸੁਨੇਹਾ.error_ਮੁੱਲ, "%s", ਸੁਨੇਹਾ.ਸੁਨੇਹਾ_2);
          errno = saved_errno;
        }
      ਵਾਪਸ crun_make_error (err, errno, "write to sync socket");
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ bool
has_cap_sys_ptrace ()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CAP
  ਬਣਤਰ __user_cap_header_struct hdr = { LINUX_CAPABILITY_ਸੰਸਕਰਣ_3, 0 };
  ਬਣਤਰ __user_cap_data_struct ਅੰਕੜੇ[2] = { { 0 } };

  ਜੇ (UNLIKELY (capget (&hdr, ਅੰਕੜੇ) < 0))
    ਵਾਪਸ true;

  ਵਾਪਸ (ਅੰਕੜੇ[0].effective & (1U << CAP_SYS_PTRACE)) != 0;
#ਨਹੀਂਤਾਂ
  ਵਾਪਸ geteuid () == 0;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ libcrun_container_t *
make_container (runtime_spec_schema_config_schema *container_def, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *config)
{
  libcrun_container_t *container = xmalloc0 (ਆਕਾਰ (*container));
  container->container_def = container_def;

  container->host_uid = geteuid ();
  container->host_gid = getegid ();
  container->host_has_cap_sys_ptrace = has_cap_sys_ptrace ();
  container->proc_fd = -1;

  container->annotations = make_string_map_from_json (container_def->annotations);

  /* An empty namespace path means that a new namespace is to be created,
     the same as if it is not specified.  */
  ਜੇ (container_def->linux)
    {
      size_t i;

      ਲਈ (i = 0; i < container_def->linux->namespaces_len; i++)
        ਜੇ (container_def->linux->namespaces[i]->ਰਸਤਾ && container_def->linux->namespaces[i]->ਰਸਤਾ[0] == '\0')
          {
            ਮੁਕਤ (container_def->linux->namespaces[i]->ਰਸਤਾ);
            container_def->linux->namespaces[i]->ਰਸਤਾ = ਨਿਰਾ;
          }
    }

  ਜੇ (ਰਸਤਾ)
    container->config_file = xstrdup (ਰਸਤਾ);
  ਜੇ (config)
    container->config_file_content = xstrdup (config);

  ਵਾਪਸ container;
}

libcrun_container_t *
libcrun_container_ਲੱਦੋ_from_ਸਮ੍ਰਿਤੀ (ਅਚਲ ਅੱਖਰ *json, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free ਅੱਖਰ *oci_error = ਨਿਰਾ;
  container_def = runtime_spec_schema_config_schema_parse_ਅੰਕੜੇ (json, ਨਿਰਾ, &oci_error);
  ਜੇ (container_def == ਨਿਰਾ)
    {
      crun_make_error (err, 0, "load: `%s`", oci_error);
      ਵਾਪਸ ਨਿਰਾ;
    }
  ਵਾਪਸ make_container (container_def, ਨਿਰਾ, json);
}

libcrun_container_t *
libcrun_container_ਲੱਦੋ_from_file (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free ਅੱਖਰ *oci_error = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *content = ਨਿਰਾ;
  size_t len;
  ਪੂਰਨ ret;

  libcrun_debug ("Loading container from config file: `%s`", ਰਸਤਾ);
  ret = read_all_file (ਰਸਤਾ, &content, &len, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ਨਿਰਾ;

  container_def = runtime_spec_schema_config_schema_parse_ਅੰਕੜੇ (content, ਨਿਰਾ, &oci_error);
  ਜੇ (container_def == ਨਿਰਾ)
    {
      crun_make_error (err, 0, "load `%s`: %s", ਰਸਤਾ, oci_error);
      ਵਾਪਸ ਨਿਰਾ;
    }
  ਵਾਪਸ make_container (container_def, ਨਿਰਾ, content);
}

ਅਚਲ ਅੱਖਰ *
libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err)
{
  ਬਣਤਰ parser_context ctx = { 0, stderr };
  parser_error gen_err = ਨਿਰਾ;

  ਜੇ (container == ਨਿਰਾ)
    {
      crun_make_error (err, EINVAL, "no container specified");
      ਵਾਪਸ ਨਿਰਾ;
    }

  ਜੇ (container->config_file_content == ਨਿਰਾ && container->container_def)
    {
      container->config_file_content = runtime_spec_schema_config_schema_generate_json (container->container_def, &ctx, &gen_err);
      ਜੇ (container->config_file_content == ਨਿਰਾ)
        {
          crun_make_error (err, 0, "cannot generate the configuration JSON: %s", gen_err ?: "unknown error");
          ਮੁਕਤ (gen_err);
          ਵਾਪਸ ਨਿਰਾ;
        }
      ਮੁਕਤ (gen_err);
    }

  ਜੇ (container->config_file_content == ਨਿਰਾ)
    {
      crun_make_error (err, 0, "the container has no configuration");
      ਵਾਪਸ ਨਿਰਾ;
    }

  ਵਾਪਸ container->config_file_content;
}

ਅਚਲ ਅੱਖਰ *
libcrun_container_get_annotation (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ)
{
  ਜੇ (container == ਨਿਰਾ)
    ਵਾਪਸ ਨਿਰਾ;
  ਵਾਪਸ ਲੱਭੋ_string_map_ਮੁੱਲ (container->annotations, ਕੁੰਜੀ);
}

size_t
libcrun_container_get_annotations_len (libcrun_container_t *container)
{
  ਜੇ (container == ਨਿਰਾ || container->annotations == ਨਿਰਾ)
    ਵਾਪਸ 0;
  ਵਾਪਸ string_map_ਮਾਪ (container->annotations);
}

ਪੂਰਨ
libcrun_container_get_annotation_at (libcrun_container_t *container, size_t ਸੂਚਕਾਂਕ, ਅਚਲ ਅੱਖਰ **ਕੁੰਜੀ,
                                     ਅਚਲ ਅੱਖਰ **ਮੁੱਲ)
{
  ਜੇ (container == ਨਿਰਾ)
    {
      errno = ERANGE;
      ਵਾਪਸ -1;
    }
  ਵਾਪਸ string_map_get_at (container->annotations, ਸੂਚਕਾਂਕ, ਕੁੰਜੀ, ਮੁੱਲ);
}

uid_t
libcrun_container_get_uid (libcrun_container_t *container)
{
  ਜੇ (container == ਨਿਰਾ)
    ਵਾਪਸ (uid_t) -1;
  ਵਾਪਸ container->host_uid;
}

gid_t
libcrun_container_get_gid (libcrun_container_t *container)
{
  ਜੇ (container == ਨਿਰਾ)
    ਵਾਪਸ (gid_t) -1;
  ਵਾਪਸ container->host_gid;
}

ਖਾਲੀ
libcrun_container_free (libcrun_container_t *ctr)
{
  ਜੇ (ctr == ਨਿਰਾ)
    ਵਾਪਸ;

  ਜੇ (ctr->cleanup_ਨਿੱਜੀ_ਅੰਕੜੇ)
    ctr->cleanup_ਨਿੱਜੀ_ਅੰਕੜੇ (ctr->ਨਿੱਜੀ_ਅੰਕੜੇ);

  ਜੇ (ctr->container_def)
    free_runtime_spec_schema_config_schema (ctr->container_def);

  free_string_map (ctr->annotations);

  ਜੇ (ctr->proc_fd >= 0)
    close (ctr->proc_fd);

  ਮੁਕਤ (ctr->config_file_content);
  ਮੁਕਤ (ctr->config_file);
  ਮੁਕਤ (ctr);
}

libcrun_context_t *
libcrun_context_new (ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, libcrun_error_t *err)
{
  libcrun_context_t *ctx = xmalloc0 (ਆਕਾਰ (*ctx));

  /* Go through the setters so that the owned_* copies and the borrowed
     pointers stay in sync and are released by libcrun_context_free.  */
  libcrun_context_ਸੈੱਟ_id (ctx, id);
  libcrun_context_ਸੈੱਟ_state_ਜੜ੍ਹ (ctx, state_ਜੜ੍ਹ);
  libcrun_context_ਸੈੱਟ_bundle (ctx, ".");
  ctx->fifo_exec_wait_fd = -1;

  ਜੇ (libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, id, ਨਿਰਾ, err) < 0)
    {
      libcrun_context_free (ctx);
      ਵਾਪਸ ਨਿਰਾ;
    }

  ctx->handler_manager = libcrun_handler_manager_create (err);
  ਜੇ (ctx->handler_manager == ਨਿਰਾ)
    {
      libcrun_context_free (ctx);
      ਵਾਪਸ ਨਿਰਾ;
    }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CRUN_LIBDIR
  ਜੇ (access (CRUN_LIBDIR "/handlers", F_OK) == 0)
    {
      ਜੇ (libcrun_handler_manager_ਲੱਦੋ_directory (ctx->handler_manager, CRUN_LIBDIR "/handlers", err) < 0)
        {
          libcrun_context_free (ctx);
          ਵਾਪਸ ਨਿਰਾ;
        }
    }
#ਜੇ_ਅੰਤ

  ਵਾਪਸ ctx;
}

ਖਾਲੀ
libcrun_context_free (libcrun_context_t *ctx)
{
  ਜੇ (ctx == ਨਿਰਾ)
    ਵਾਪਸ;

  ਮੁਕਤ (ctx->owned_state_ਜੜ੍ਹ);
  ਮੁਕਤ (ctx->owned_id);
  ਮੁਕਤ (ctx->owned_bundle);
  ਮੁਕਤ (ctx->owned_console_socket);
  ਮੁਕਤ (ctx->owned_pid_file);
  ਮੁਕਤ (ctx->owned_notify_socket);
  ਮੁਕਤ (ctx->owned_handler);

  ਜੇ (ctx->handler_manager)
    handler_manager_free (ctx->handler_manager);

  ਮੁਕਤ (ctx);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LIBCRUN_CONTEXT_ਸੈੱਟ_STRING(ਖੇਤਰ)                  \
  ਕਰੋ                                                       \
    {                                                      \
      ਮੁਕਤ (ctx->owned_##field);                           \
      ctx->owned_##field = ਮੁੱਲ ? xstrdup (ਮੁੱਲ) : ਨਿਰਾ; \
      ctx->ਖੇਤਰ = ctx->owned_##field;                     \
  } ਜਦੋਂ (0)

ਖਾਲੀ
libcrun_context_ਸੈੱਟ_id (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
  LIBCRUN_CONTEXT_ਸੈੱਟ_STRING (id);
}

ਖਾਲੀ
libcrun_context_ਸੈੱਟ_state_ਜੜ੍ਹ (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
  LIBCRUN_CONTEXT_ਸੈੱਟ_STRING (state_ਜੜ੍ਹ);
}

ਖਾਲੀ
libcrun_context_ਸੈੱਟ_bundle (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
  LIBCRUN_CONTEXT_ਸੈੱਟ_STRING (bundle);
}

ਖਾਲੀ
libcrun_context_ਸੈੱਟ_console_socket (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
  LIBCRUN_CONTEXT_ਸੈੱਟ_STRING (console_socket);
}

ਖਾਲੀ
libcrun_context_ਸੈੱਟ_pid_file (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
  LIBCRUN_CONTEXT_ਸੈੱਟ_STRING (pid_file);
}

ਖਾਲੀ
libcrun_context_ਸੈੱਟ_notify_socket (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
  LIBCRUN_CONTEXT_ਸੈੱਟ_STRING (notify_socket);
}

ਖਾਲੀ
libcrun_context_ਸੈੱਟ_handler (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
  LIBCRUN_CONTEXT_ਸੈੱਟ_STRING (handler);
}

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ LIBCRUN_CONTEXT_ਸੈੱਟ_STRING

ਅਚਲ ਅੱਖਰ *
libcrun_context_get_pid_file (libcrun_context_t *ctx)
{
  ਵਾਪਸ ctx->pid_file;
}

ਅਚਲ ਅੱਖਰ *
libcrun_context_get_state_ਜੜ੍ਹ (libcrun_context_t *ctx)
{
  ਜੇ (ctx == ਨਿਰਾ)
    ਵਾਪਸ ਨਿਰਾ;
  ਵਾਪਸ ctx->state_ਜੜ੍ਹ;
}

ਖਾਲੀ
libcrun_context_ਸੈੱਟ_preserve_fds (libcrun_context_t *ctx, ਪੂਰਨ preserve_fds)
{
  ctx->preserve_fds = preserve_fds;
}

ਪੂਰਨ
libcrun_context_get_preserve_fds (libcrun_context_t *ctx)
{
  ਵਾਪਸ ctx->preserve_fds;
}

ਖਾਲੀ
libcrun_context_ਸੈੱਟ_listen_fds (libcrun_context_t *ctx, ਪੂਰਨ listen_fds)
{
  ctx->listen_fds = listen_fds;
}

ਖਾਲੀ
libcrun_context_ਸੈੱਟ_ਦਲੀਲਾਂ (libcrun_context_t *ctx, ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ctx->argc = argc;
  ctx->argv = argv;
}

ਖਾਲੀ
libcrun_context_ਸੈੱਟ_systemd_cgroup (libcrun_context_t *ctx, bool ਮੁੱਲ)
{
  ctx->systemd_cgroup = ਮੁੱਲ;
}

ਖਾਲੀ
libcrun_context_ਸੈੱਟ_detach (libcrun_context_t *ctx, bool ਮੁੱਲ)
{
  ctx->detach = ਮੁੱਲ;
}

ਖਾਲੀ
libcrun_context_ਸੈੱਟ_no_new_keyring (libcrun_context_t *ctx, bool ਮੁੱਲ)
{
  ctx->no_new_keyring = ਮੁੱਲ;
}

ਖਾਲੀ
libcrun_context_ਸੈੱਟ_no_ਧੁਰਾ (libcrun_context_t *ctx, bool ਮੁੱਲ)
{
  ctx->no_ਧੁਰਾ = ਮੁੱਲ;
}

ਖਾਲੀ
libcrun_context_ਸੈੱਟ_force_no_cgroup (libcrun_context_t *ctx, bool ਮੁੱਲ)
{
  ctx->force_no_cgroup = ਮੁੱਲ;
}

ਖਾਲੀ
libcrun_context_ਸੈੱਟ_output_handler (libcrun_context_t *ctx, crun_output_handler handler, ਖਾਲੀ *arg)
{
  ctx->output_handler = handler;
  ctx->output_handler_arg = arg;
}

ਪੂਰਨ
libcrun_context_init_logging (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *ਲਘੂਗਣਕ, libcrun_error_t *err)
{
  ਵਾਪਸ libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, ctx->id, ਲਘੂਗਣਕ, err);
}

ਸਥਿਰ ਪੂਰਨ
ਖੰਡ_ਸੰਕੇਤ (libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  sigset_t ਨਕਾਬ;
  sigfillset (&ਨਕਾਬ);
  ret = sigprocmask (SIG_ਖੰਡ, &ਨਕਾਬ, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "sigprocmask");
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
unblock_ਸੰਕੇਤ (libcrun_error_t *err)
{
  ਪੂਰਨ i;
  ਪੂਰਨ ret;
  sigset_t ਨਕਾਬ;
  ਬਣਤਰ sigaction act = {};

  sigfillset (&ਨਕਾਬ);
  ret = sigprocmask (SIG_UNBLOCK, &ਨਕਾਬ, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "sigprocmask");

  act.sa_handler = SIG_DFL;
  ਲਈ (i = 0; i < NSIG; i++)
    {
      ret = sigaction (i, &act, ਨਿਰਾ);
      ਜੇ (ret < 0 && errno != EINVAL)
        ਵਾਪਸ crun_make_error (err, errno, "sigaction");
    }

  ਵਾਪਸ 0;
}

/* must be used on the host before pivot_root(2).  */
ਸਥਿਰ ਪੂਰਨ
initialize_security (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ret = libcrun_init_caps (container, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (UNLIKELY (proc == ਨਿਰਾ))
    ਵਾਪਸ 0;

  ਜੇ (proc->apparmor_profile)
    {
      ret = libcrun_initialize_apparmor (err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ret = libcrun_initialize_selinux (container, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ 0;
}

/* Emit the "annotations" map.  Returns a json_gen_status; callers can wrap
   the call with GEN_OR_FAIL.  */
ਸਥਿਰ ਪੂਰਨ
gen_annotations (json_gen_ctx *gen, json_map_string_string *annotations)
{
  size_t i;
  ਪੂਰਨ r;

  GEN_ਕੁੰਜੀ (gen, "annotations");
  GEN_OR_FAIL (json_gen_map_open (gen));

  ਲਈ (i = 0; i < annotations->len; i++)
    {
      /* An annotation whose value is null in the document is parsed into a
         NULL, which cannot be emitted as a JSON string; leave it out.  */
      ਜੇ (annotations->ਕੁੰਜੀਆਂ[i] == ਨਿਰਾ || annotations->ਮੁੱਲਾਂ[i] == ਨਿਰਾ)
        ਜਾਰੀ;

      GEN_STR (gen, annotations->ਕੁੰਜੀਆਂ[i]);
      GEN_STR (gen, annotations->ਮੁੱਲਾਂ[i]);
    }

  ਵਾਪਸ json_gen_map_close (gen);

gen_error:
  ਵਾਪਸ r;
}

/* Return the bundle path to report in the container state.  crun always
   chdir()s into the bundle, so fall back to getcwd() when the caller has no
   absolute bundle path, as can happen when libcrun is used as a library.
   *ALLOCATED must be freed by the caller.  */
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
get_bundle_ਰਸਤਾ (ਅਚਲ ਅੱਖਰ *bundle, ਅੱਖਰ **allocated)
{
  ਜੇ (bundle != ਨਿਰਾ && bundle[0] == '/')
    ਵਾਪਸ bundle;

  *allocated = getcwd (ਨਿਰਾ, 0);
  ਜੇ (*allocated == ਨਿਰਾ)
    OOM ();
  ਵਾਪਸ *allocated;
}

ਸਥਿਰ ਪੂਰਨ
do_hooks (runtime_spec_schema_config_schema *def, pid_t pid, ਅਚਲ ਅੱਖਰ *id, bool keep_going, ਅਚਲ ਅੱਖਰ *bundle,
          ਅਚਲ ਅੱਖਰ *cwd, ਅਚਲ ਅੱਖਰ *ਸਥਿਤੀ, ਅਚਲ ਅੱਖਰ *name, hook **hooks, size_t hooks_len, ਪੂਰਨ ਬਾਹਰਲਾ_fd,
          ਪੂਰਨ err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  size_t i, stdin_len;
  ਪੂਰਨ r, ret;
  ਅੱਖਰ *stdin = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *bundle_allocated = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *rootfs = def->ਜੜ੍ਹ ? def->ਜੜ੍ਹ->ਰਸਤਾ : "";
  cleanup_json_gen json_gen_ctx *gen = ਨਿਰਾ;

  bundle = get_bundle_ਰਸਤਾ (bundle, &bundle_allocated);

  ਜੇ (! json_gen_init (&gen, ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 0);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_ਕੁੰਜੀ (gen, "ociVersion");
  GEN_STR (gen, "1.0");

  GEN_ਕੁੰਜੀ (gen, "id");
  GEN_STR (gen, id);

  GEN_ਕੁੰਜੀ (gen, "pid");
  GEN_OR_FAIL (map_int (gen, pid));

  GEN_ਕੁੰਜੀ (gen, "root");
  GEN_STR (gen, rootfs);

  GEN_ਕੁੰਜੀ (gen, "bundle");
  GEN_STR (gen, bundle);

  GEN_ਕੁੰਜੀ (gen, "status");
  GEN_STR (gen, ਸਥਿਤੀ);

  ਜੇ (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, (ਅਚਲ ਅੱਖਰ **) &stdin, &stdin_len));

  ret = 0;

  bool error_created = false;
  ਲਈ (i = 0; i < hooks_len; i++)
    {
      ਅੱਖਰ **env = environ;

      ਜੇ (hooks[i]->env)
        env = hooks[i]->env;

      /* Release the error from the previous iteration, if any.  */
      ਜੇ (error_created)
        {
          crun_error_release (err);
          error_created = false;
        }

      ret = ਲੜੀਵਾਰ_process_with_stdin_timeout_envp (hooks[i]->ਰਸਤਾ, hooks[i]->ਦਲੀਲਾਂ, cwd, hooks[i]->timeout, env,
                                                 stdin, stdin_len, ਬਾਹਰਲਾ_fd, err_fd, can_ignore_chdir_errors, err);
      ਜੇ (UNLIKELY (ret < 0))
        error_created = true;

      ਜੇ (UNLIKELY (ret != 0))
        {
          ਜੇ (keep_going)
            libcrun_warning ("error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->ਰਸਤਾ, ret);
          ਨਹੀਂਤਾਂ
            {
              libcrun_error (0, "error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->ਰਸਤਾ, ret);
              ਰੋਕੋ;
            }
        }
    }

  ਵਾਪਸ ret;

gen_error:
  ਵਾਪਸ json_gen_error_ਮੰਜ਼ਿਲ_crun_error (r, err);
}

ਸਥਿਰ ਪੂਰਨ
get_json_gen_ਨਤੀਜਾ (json_gen_ctx *gen, ਅੱਖਰ **ਬਾਹਰਲਾ, size_t *ਬਾਹਰਲਾ_len)
{
  ਅਚਲ ਅੱਖਰ *buf = ਨਿਰਾ;
  size_t buf_len = 0;
  ਪੂਰਨ r;

  r = json_gen_get_buf (gen, &buf, &buf_len);
  ਜੇ (UNLIKELY (r != json_gen_ਸਥਿਤੀ_ok))
    ਵਾਪਸ r;

  *ਬਾਹਰਲਾ_len = buf_len;

  *ਬਾਹਰਲਾ = xmalloc (buf_len + 1);
  ਸਮ੍ਰਿਤੀ_ਨਕਲ (*ਬਾਹਰਲਾ, buf, buf_len);
  (*ਬਾਹਰਲਾ)[buf_len] = '\0';

  ਵਾਪਸ json_gen_ਸਥਿਤੀ_ok;
}

ਸਥਿਰ ਪੂਰਨ
get_seccomp_receiver_fd_payload (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *ਸਥਿਤੀ, pid_t own_pid,
                                 ਅੱਖਰ **seccomp_fd_payload, size_t *seccomp_fd_payload_len, libcrun_error_t *err)
{
  ਪੂਰਨ r;
  json_gen_ctx *gen = ਨਿਰਾ;
  runtime_spec_schema_config_schema *def = container->container_def;
  ਅਚਲ ਅੱਖਰ *ਅਚਲ OCI_ਸੰਸਕਰਣ = "0.2.0";

  ਜੇ (! json_gen_init (&gen, ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_ਕੁੰਜੀ (gen, "ociVersion");
  GEN_STR (gen, OCI_ਸੰਸਕਰਣ);

  GEN_ਕੁੰਜੀ (gen, "fds");
  GEN_OR_FAIL (json_gen_array_open (gen));

  GEN_ਕੁੰਜੀ (gen, "seccompFd");
  GEN_OR_FAIL (json_gen_array_close (gen));

  GEN_ਕੁੰਜੀ (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  ਜੇ (def && def->linux && def->linux->seccomp)
    {
      ਅਚਲ ਅੱਖਰ *metadata = def->linux->seccomp->listener_metadata;

      ਜੇ (metadata)
        {
          GEN_ਕੁੰਜੀ (gen, "metadata");
          GEN_STR (gen, metadata);
        }
    }

  /* State.  */
  GEN_ਕੁੰਜੀ (gen, "state");
  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_ਕੁੰਜੀ (gen, "ociVersion");
  GEN_STR (gen, OCI_ਸੰਸਕਰਣ);

  ਜੇ (container->context && container->context->id)
    {
      GEN_ਕੁੰਜੀ (gen, "id");
      GEN_STR (gen, container->context->id);
    }

  GEN_ਕੁੰਜੀ (gen, "status");
  GEN_STR (gen, ਸਥਿਤੀ);

  GEN_ਕੁੰਜੀ (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  ਜੇ (container->context && container->context->bundle)
    {
      GEN_ਕੁੰਜੀ (gen, "bundle");
      GEN_STR (gen, container->context->bundle);
    }

  ਜੇ (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));
  /* End state.  */

  GEN_OR_FAIL (json_gen_map_close (gen));

  r = get_json_gen_ਨਤੀਜਾ (gen, seccomp_fd_payload, seccomp_fd_payload_len);

gen_error:
  json_gen_free (gen);

  ਵਾਪਸ json_gen_error_ਮੰਜ਼ਿਲ_crun_error (r, err);
}

ਸਥਿਰ ਪੂਰਨ
send_sync_cb (ਖਾਲੀ *ਅੰਕੜੇ, libcrun_error_t *err)
{
  ਪੂਰਨ sync_socket_fd = *((ਪੂਰਨ *) ਅੰਕੜੇ);
  ਪੂਰਨ ret;

  /* sync 2.  */
  ret = sync_socket_send_sync (sync_socket_fd, false, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  /* sync 3.  */
  ਵਾਪਸ sync_socket_wait_sync (ਨਿਰਾ, sync_socket_fd, false, err);
}

ਸਥਿਰ ਪੂਰਨ
maybe_chown_std_streams (uid_t container_uid, gid_t container_gid, libcrun_error_t *err)
{
  ਪੂਰਨ ret, i;

  ਲਈ (i = 0; i < 3; i++)
    {
      ਜੇ (! isatty (i))
        {
          ਬਣਤਰ stat statbuf;
          ret = fstat (i, &statbuf);
          ਜੇ (UNLIKELY (ret < 0))
            {
              ਜੇ (errno == EBADF)
                ਜਾਰੀ;
              ਵਾਪਸ crun_make_error (err, errno, "fstat fd `%d`", i);
            }

          /* Skip chown for device files */
          ਜੇ (S_ISCHR (statbuf.st_mode) || S_ISBLK (statbuf.st_mode))
            ਜਾਰੀ;

          ret = fchown (i, container_uid, container_gid);
          ਜੇ (UNLIKELY (ret < 0))
            {
              /* EINVAL means the user is not mapped in the current userns.
                 Ignore EPERM and EROFS as well as there is no reason to fail
                 so early, and let the container payload deal with it.
                 EBADF means fd is closed.
              */
              ਜੇ (errno == EINVAL || errno == EPERM || errno == EROFS || errno == EBADF)
                ਜਾਰੀ;

              ਵਾਪਸ crun_make_error (err, errno, "fchown std stream `%i`", i);
            }
        }
    }
  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_container_notify_handler (ਬਣਤਰ container_entrypoint_s *ਦਲੀਲਾਂ,
                                  ਗਿਣਤੀ handler_configure_phase phase,
                                  libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *rootfs,
                                  libcrun_error_t *err)
{
  ਬਣਤਰ custom_handler_s *h;

  ਜੇ (ਦਲੀਲਾਂ->custom_handler == ਨਿਰਾ)
    ਵਾਪਸ 0;

  h = ਦਲੀਲਾਂ->custom_handler->vtable;
  ਜੇ (h == ਨਿਰਾ || h->configure_container == ਨਿਰਾ)
    ਵਾਪਸ 0;

  ਵਾਪਸ h->configure_container (ਦਲੀਲਾਂ->custom_handler->cookie, phase,
                                 ਦਲੀਲਾਂ->context, container,
                                 rootfs, err);
}

/* Resolve and normalize the container rootfs path.  */
ਸਥਿਰ ਪੂਰਨ
resolve_rootfs_ਰਸਤਾ (libcrun_container_t *container, ਅੱਖਰ **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ਜੇ (def->ਜੜ੍ਹ && def->ਜੜ੍ਹ->ਰਸਤਾ)
    {
      *rootfs = realpath (def->ਜੜ੍ਹ->ਰਸਤਾ, ਨਿਰਾ);
      ਜੇ (UNLIKELY (*rootfs == ਨਿਰਾ))
        {
          ਪੂਰਨ saved_errno = errno;

          /* If realpath failed for any reason, try the relative directory.  */
          ਜੇ (def->ਜੜ੍ਹ->ਰਸਤਾ[0] == '/')
            {
              cleanup_free ਅੱਖਰ *cwd = ਨਿਰਾ;
              libcrun_error_t tmp_err = ਨਿਰਾ;
              ssize_t len;
              ਪੂਰਨ proc_fd;

              proc_fd = libcrun_get_cached_proc_fd (container, &tmp_err);
              ਜੇ (LIKELY (proc_fd >= 0))
                {
                  len = ਸੁਰੱਖਿਅਤ_readlinkat (proc_fd, "self/cwd", &cwd, 0, &tmp_err);
                  ਜੇ (LIKELY (len > 0))
                    {
                      /* If the rootfs is under the current working directory, just use its relative path.  */
                      ਜੇ (has_dir_ਅਗੇਤਰ (def->ਜੜ੍ਹ->ਰਸਤਾ, cwd))
                        {
                          ਅਚਲ ਅੱਖਰ *it = consume_slashes (def->ਜੜ੍ਹ->ਰਸਤਾ + len);
                          ਜੇ (LIKELY (*it))
                            {
                              ਪੂਰਨ ret = faccessat (AT_FDCWD, it, F_OK, 0);
                              ਜੇ (LIKELY (ret == 0))
                                *rootfs = xstrdup (it);
                            }
                        }
                    }
                }
              crun_error_release (&tmp_err);
            }

          ਜੇ (*rootfs == ਨਿਰਾ)
            {
              ਜੇ (saved_errno == ENOENT)
                ਵਾਪਸ crun_make_error (err, ENOENT, "the rootfs path `%s` does not exist", def->ਜੜ੍ਹ->ਰਸਤਾ);

              *rootfs = xstrdup (def->ਜੜ੍ਹ->ਰਸਤਾ);
            }
        }
    }
  ਵਾਪਸ 0;
}

/* Initialize the environment variables.  */
ਸਥਿਰ ਪੂਰਨ
ਤਿਆਰੀ_environment (runtime_spec_schema_config_schema *def, uid_t container_uid, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ਜੇ (clearenv ())
    ਵਾਪਸ crun_make_error (err, errno, "clearenv");

  ਜੇ (def->process)
    {
      size_t i;

      ਲਈ (i = 0; i < def->process->env_len; i++)
        {
          /* A null element of the array of variables is parsed into a NULL,
             which putenv(3) cannot be handed.  */
          ਜੇ (def->process->env[i] == ਨਿਰਾ)
            ਵਾਪਸ crun_make_error (err, EINVAL, "environment variable is not specified");

          ਜੇ (putenv (def->process->env[i]) < 0)
            ਵਾਪਸ crun_make_error (err, errno, "putenv `%s`", def->process->env[i]);
        }
    }

  ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_string (getenv ("HOME")))
    {
      ret = ਸੈੱਟ_home_env (container_uid);
      ਜੇ (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          ਜੇ (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            ਵਾਪਸ crun_make_error (err, errno, "setenv HOME");
        }
    }

  ਵਾਪਸ 0;
}

/* Configure terminal and console for interactive containers.  */
ਸਥਿਰ ਪੂਰਨ
ਤਿਆਰੀ_terminal (ਬਣਤਰ container_entrypoint_s *entrypoint_ਦਲੀਲਾਂ, libcrun_container_t *container,
                ਪੂਰਨ console_socket, ਪੂਰਨ console_socketpair, ਪੂਰਨ has_terminal, libcrun_error_t *err)
{
  cleanup_close ਪੂਰਨ terminal_fd = -1;
  ਪੂਰਨ ret;

  ਜੇ (! has_terminal)
    ਵਾਪਸ 0;

  fflush (stderr);

  ਜੇ (console_socket >= 0 || (entrypoint_ਦਲੀਲਾਂ->has_terminal_socket_pair && console_socketpair >= 0))
    {
      terminal_fd = libcrun_ਸੈੱਟ_terminal (container, err);
      ਜੇ (UNLIKELY (terminal_fd < 0))
        ਵਾਪਸ terminal_fd;
    }

  ਜੇ (console_socket >= 0)
    {
      ret = send_fd_ਮੰਜ਼ਿਲ_socket (console_socket, terminal_fd, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਨਹੀਂਤਾਂ ਜੇ (entrypoint_ਦਲੀਲਾਂ->has_terminal_socket_pair && console_socketpair >= 0)
    {
      ret = send_fd_ਮੰਜ਼ਿਲ_socket (console_socketpair, terminal_fd, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਵਾਪਸ 0;
}

/* Resolve and validate the container's executable path.  */
ਸਥਿਰ ਪੂਰਨ
ਤਿਆਰੀ_executable_ਰਸਤਾ (ਬਣਤਰ container_entrypoint_s *entrypoint_ਦਲੀਲਾਂ, runtime_spec_schema_config_schema *def,
                       ਅੱਖਰ **exec_ਰਸਤਾ, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ਜੇ (def->process && def->process->ਦਲੀਲਾਂ)
    {
      ret = ਲੱਭੋ_executable (exec_ਰਸਤਾ, def->process->ਦਲੀਲਾਂ[0], def->process->cwd, err);
      ਜੇ (UNLIKELY (ret < 0))
        {
          ਜੇ (entrypoint_ਦਲੀਲਾਂ->custom_handler == ਨਿਰਾ && crun_error_get_errno (err) == ENOENT)
            ਵਾਪਸ ret;

          /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
             once the process switched to the use that runs in the container.  This might be necessary
             when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
             is not honored.  */
          crun_error_release (err);
        }
    }

  ਵਾਪਸ 0;
}

/* Apply security settings for the container process.  */
ਸਥਿਰ ਪੂਰਨ
apply_security_settings (ਬਣਤਰ container_entrypoint_s *entrypoint_ਦਲੀਲਾਂ, runtime_spec_schema_config_schema *def,
                         libcrun_container_t *container, pid_t own_pid, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  runtime_spec_schema_config_schema_process_capabilities *capabilities;
  ਪੂਰਨ no_new_privs;

  ਜੇ (def->process && ! def->process->no_new_privileges)
    {
      ਅੱਖਰ **seccomp_flags = ਨਿਰਾ;
      size_t seccomp_flags_len = 0;
      cleanup_free ਅੱਖਰ *seccomp_fd_payload = ਨਿਰਾ;
      size_t seccomp_fd_payload_len = 0;

      ਜੇ (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      ਜੇ (entrypoint_ਦਲੀਲਾਂ->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_ਦਲੀਲਾਂ->seccomp_fd, entrypoint_ਦਲੀਲਾਂ->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      close_and_reset (&entrypoint_ਦਲੀਲਾਂ->seccomp_fd);
      close_and_reset (&entrypoint_ਦਲੀਲਾਂ->seccomp_receiver_fd);
    }

  capabilities = def->process ? def->process->capabilities : ਨਿਰਾ;
  no_new_privs = def->process ? def->process->no_new_privileges : 1;
  ret = libcrun_ਸੈੱਟ_caps (capabilities, container->container_uid, container->container_gid, no_new_privs,
                          entrypoint_ਦਲੀਲਾਂ->custom_handler != ਨਿਰਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ 0;
}

/* Create the process that joins the existing PID namespace now that the
   host mounts are not visible anymore, and continue the setup from there.  */
ਸਥਿਰ ਪੂਰਨ
do_delayed_pidns_fork (ਪੂਰਨ sync_socket, pid_t *own_pid, libcrun_error_t *err)
{
  pid_t pid;
  ਪੂਰਨ ret;

  pid = fork ();
  ਜੇ (UNLIKELY (pid < 0))
    ਵਾਪਸ crun_make_error (err, errno, "fork");

  ਜੇ (pid)
    {
      /* Report the new PID to the parent and exit immediately.  */
      ret = sync_socket_send_sync (sync_socket, false, err);
      ਜੇ (LIKELY (ret == 0))
        {
          ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (sync_socket, &pid, ਆਕਾਰ (pid)));
          ਜੇ (LIKELY (ret == ਆਕਾਰ (pid)))
            ਸੁਰੱਖਿਅਤ_exit (EXIT_SUCCESS);
        }

      kill (pid, SIGKILL);
      ਸੁਰੱਖਿਅਤ_exit (EXIT_FAILURE);
    }

  /* sync receive own pid.  */
  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (sync_socket, own_pid, ਆਕਾਰ (*own_pid)));
  ਜੇ (UNLIKELY (ret != ਆਕਾਰ (*own_pid)))
    {
      ਜੇ (ret >= 0)
        errno = 0;
      ਵਾਪਸ crun_make_error (err, errno, "read from sync socket");
    }

  ਵਾਪਸ 0;
}

ਬਣਤਰ delayed_pidns_fork_s
{
  libcrun_container_t *container;
  ਪੂਰਨ sync_socket;
  pid_t *own_pid;
};

ਸਥਿਰ ਪੂਰਨ
delayed_pidns_fork_cb (ਖਾਲੀ *ਅੰਕੜੇ, libcrun_error_t *err)
{
  ਬਣਤਰ delayed_pidns_fork_s *d = ਅੰਕੜੇ;

  ਜੇ (! libcrun_container_delayed_pidns_fork (d->container))
    ਵਾਪਸ 0;

  ਵਾਪਸ do_delayed_pidns_fork (d->sync_socket, d->own_pid, err);
}

ਸਥਿਰ ਪੂਰਨ
container_init_ਤਿਆਰੀ (ਖਾਲੀ *ਦਲੀਲਾਂ, pid_t *own_pid, ਅੱਖਰ *notify_socket,
                      ਪੂਰਨ sync_socket, ਅੱਖਰ **exec_ਰਸਤਾ, libcrun_error_t *err)
{
  ਬਣਤਰ container_entrypoint_s *entrypoint_ਦਲੀਲਾਂ = ਦਲੀਲਾਂ;
  libcrun_container_t *container = entrypoint_ਦਲੀਲਾਂ->container;
  bool chdir_ਪੂਰਾ = false;
  ਪੂਰਨ ret;
  ਪੂਰਨ has_terminal;
  cleanup_close ਪੂਰਨ console_socket = -1;
  cleanup_close ਪੂਰਨ console_socketpair = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free ਅੱਖਰ *rootfs = ਨਿਰਾ;
  ਬਣਤਰ delayed_pidns_fork_s delayed_pidns_fork = {
    .container = container,
    .sync_socket = sync_socket,
    .own_pid = own_pid,
  };

  ret = initialize_security (container, def->process, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_configure_ਜਾਲ (container, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = resolve_rootfs_ਰਸਤਾ (container, &rootfs, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  /* Configure terminal socket pair for container communication.  */
  ਜੇ (entrypoint_ਦਲੀਲਾਂ->terminal_socketpair[0] >= 0)
    {
      close_and_reset (&entrypoint_ਦਲੀਲਾਂ->terminal_socketpair[0]);
      console_socketpair = entrypoint_ਦਲੀਲਾਂ->terminal_socketpair[1];
    }

  /* sync 1.  */
  ret = sync_socket_wait_sync (ਨਿਰਾ, sync_socket, false, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  has_terminal = container->container_def->process && container->container_def->process->terminal;
  ਜੇ (has_terminal && entrypoint_ਦਲੀਲਾਂ->context->console_socket)
    console_socket = entrypoint_ਦਲੀਲਾਂ->console_socket_fd;

  ret = libcrun_ਸੈੱਟ_sysctl (container, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_container_notify_handler (entrypoint_ਦਲੀਲਾਂ, HANDLER_CONFIGURE_BEFORE_MOUNTS, container, rootfs, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_do_ਧੁਰਾ_ਜੜ੍ਹ (container, entrypoint_ਦਲੀਲਾਂ->context->no_ਧੁਰਾ, &rootfs, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  /* sync 2 and 3 are sent as part of libcrun_set_mounts.  */
  ret = libcrun_ਸੈੱਟ_mounts (entrypoint_ਦਲੀਲਾਂ, container, rootfs, send_sync_cb, &sync_socket, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  /* The createContainer hooks MUST run after the mounts are set up but before
     the pivot_root.  When these hooks are present can_use_open_tree_namespace()
     forces the deferred-pivot path, so the actual pivot happens later in
     libcrun_finalize_mounts().  */
  ਜੇ (def->hooks && def->hooks->create_container_len)
    {
      libcrun_error_t tmp_err = ਨਿਰਾ;
      ਪੂਰਨ ਵਿੱਚ_userns = check_running_ਵਿੱਚ_user_namespace (&tmp_err);
      ਜੇ (tmp_err)
        crun_error_release (&tmp_err);
      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_ਦਲੀਲਾਂ->bundle, ਨਿਰਾ, "created", "createContainer",
                      (hook **) def->hooks->create_container,
                      def->hooks->create_container_len, entrypoint_ਦਲੀਲਾਂ->hooks_ਬਾਹਰਲਾ_fd, entrypoint_ਦਲੀਲਾਂ->hooks_err_fd,
                      ਵਿੱਚ_userns > 0, err);
      ਜੇ (UNLIKELY (ret != 0))
        ਵਾਪਸ ret;
    }

  ret = libcrun_finalize_mounts (entrypoint_ਦਲੀਲਾਂ, container, rootfs, delayed_pidns_fork_cb, &delayed_pidns_fork, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (def->process)
    {
      ret = libcrun_ਸੈੱਟ_selinux_label (container, def->process, false, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = libcrun_ਸੈੱਟ_apparmor_profile (container, def->process, false, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ret = mark_or_close_fds_ge_than (container, entrypoint_ਦਲੀਲਾਂ->context->preserve_fds + 3, false, err);
  ਜੇ (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ret = libcrun_reopen_dev_null (err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = maybe_chown_std_streams (container->container_uid, container->container_gid, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = ਤਿਆਰੀ_environment (def, container->container_uid, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  /* Set primary process to 1 explicitly if nothing is configured and LISTEN_FD is not set.  */
  ਜੇ (entrypoint_ਦਲੀਲਾਂ->context->listen_fds > 0 && getenv ("LISTEN_PID") == ਨਿਰਾ)
    {
      libcrun_warning ("setting LISTEN_PID=1 since no previous configuration was found");
      ਜੇ (UNLIKELY (setenv ("LISTEN_PID", "1", 1) < 0))
        ਵਾਪਸ crun_make_error (err, errno, "setenv LISTEN_PID");
    }

  /* Attempt to chdir immediately here, before doing the setresuid.  If we fail here, let's
     try again later once the process switched to the user that runs in the container.  */
  ਜੇ (def->process && def->process->cwd)
    {
      ret = libcrun_ਸੁਰੱਖਿਅਤ_chdir (def->process->cwd, err);
      ਜੇ (LIKELY (ret == 0))
        chdir_ਪੂਰਾ = true;
      ਨਹੀਂਤਾਂ
        crun_error_release (err);
    }

  ret = setsid ();
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "setsid");

  ret = ਤਿਆਰੀ_terminal (entrypoint_ਦਲੀਲਾਂ, container, console_socket, console_socketpair, has_terminal, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  close_and_reset (&console_socket);
  close_and_reset (&console_socketpair);

  ret = ਤਿਆਰੀ_executable_ਰਸਤਾ (entrypoint_ਦਲੀਲਾਂ, def, exec_ਰਸਤਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_ਸੈੱਟ_hostname (container, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_ਸੈੱਟ_domainname (container, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_ਸੈੱਟ_personality (container->container_def->linux->personality, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (def->process && def->process->user)
    umask (def->process->user->umask_present ? def->process->user->umask : 0022);

  ret = apply_security_settings (entrypoint_ਦਲੀਲਾਂ, def, container, *own_pid, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (UNLIKELY (def->process && def->process->ਦਲੀਲਾਂ && *exec_ਰਸਤਾ == ਨਿਰਾ))
    {
      ret = ਲੱਭੋ_executable (exec_ਰਸਤਾ, def->process->ਦਲੀਲਾਂ[0], def->process->cwd, err);
      ਜੇ (UNLIKELY (ret < 0))
        {
          ਜੇ (entrypoint_ਦਲੀਲਾਂ->custom_handler == ਨਿਰਾ || is_ਖਾਲੀ_ਥਾਵਾਂ_string (def->process->ਦਲੀਲਾਂ[0]))
            ਵਾਪਸ ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          *exec_ਰਸਤਾ = xstrdup (def->process->ਦਲੀਲਾਂ[0]);
        }
    }

  /* The chdir was not already performed, so try again now after switching to the UID/GID in the container.  */
  ਜੇ (! chdir_ਪੂਰਾ && def->process && def->process->cwd)
    {
      ret = libcrun_ਸੁਰੱਖਿਅਤ_chdir (def->process->cwd, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (notify_socket && putenv (notify_socket) < 0)
    ਵਾਪਸ crun_make_error (err, errno, "putenv `%s`", notify_socket);

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
open_hooks_output (libcrun_container_t *container, ਪੂਰਨ *ਬਾਹਰਲਾ_fd, ਪੂਰਨ *err_fd, libcrun_error_t *err)
{
  ਅਚਲ ਅੱਖਰ *annotation;

  *err_fd = *ਬਾਹਰਲਾ_fd = -1;

  libcrun_debug ("Opening hooks output");
  annotation = ਲੱਭੋ_annotation (container, "run.oci.hooks.stdout");
  ਜੇ (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stdout` annotation");
      *ਬਾਹਰਲਾ_fd = ਆਰਜ਼ੀ_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      ਜੇ (UNLIKELY (*ਬਾਹਰਲਾ_fd < 0))
        ਵਾਪਸ crun_make_error (err, errno, "open `%s`", annotation);
    }

  annotation = ਲੱਭੋ_annotation (container, "run.oci.hooks.stderr");
  ਜੇ (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stderr` annotation");
      *err_fd = ਆਰਜ਼ੀ_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      ਜੇ (UNLIKELY (*err_fd < 0))
        ਵਾਪਸ crun_make_error (err, errno, "open `%s`", annotation);
    }

  ਵਾਪਸ 0;
}

/* The environment the process was started with lives on the initial stack, right after the
   argv strings and before the string AT_EXECFN points to.  clearenv(3) only drops the pointers
   to it, so when there is no execv to replace the stack, erase it or the container process can
   still read it, e.g. through /proc/self/environ.  */
ਸਥਿਰ ਖਾਲੀ
erase_initial_environment (ਅੱਖਰ **argv, ਪੂਰਨ argc)
{
  ਅੱਖਰ *ਸ਼ੁਰੂ = argv[argc - 1] + ਲੜੀ_ਲੰਬਾਈ (argv[argc - 1]) + 1;
  ਅੱਖਰ *ਸਿਰਾ = (ਅੱਖਰ *) getauxval (AT_EXECFN);

  ਜੇ (ਸਿਰਾ > ਸ਼ੁਰੂ)
    memset (ਸ਼ੁਰੂ, 0, ਸਿਰਾ - ਸ਼ੁਰੂ);
}

ਸਥਿਰ ਖਾਲੀ
rewrite_argv (ਅੱਖਰ **argv, ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ *name, ਅੱਖਰ **ਦਲੀਲਾਂ, size_t ਦਲੀਲਾਂ_len)
{
  cleanup_free ਅੱਖਰ *decorated_name = ਨਿਰਾ;
  size_t ਉਪਲਬਧ_len = argv[argc - 1] - argv[0] + ਲੜੀ_ਲੰਬਾਈ (argv[argc - 1]) + 1;
  cleanup_free ਅੱਖਰ *new_argv = ਨਿਰਾ;
  size_t i, so_far = 0, needed = 0;

  needed = xasprintf (&decorated_name, "[libcrun:%s]", name) + 1;
  ਲਈ (i = 0; i < ਦਲੀਲਾਂ_len; i++)
    needed += ਲੜੀ_ਲੰਬਾਈ (ਦਲੀਲਾਂ[i]) + 1;

  new_argv = xmalloc0 (needed + 1);
  strcpy (new_argv, decorated_name);
  so_far = ਲੜੀ_ਲੰਬਾਈ (decorated_name) + 1;

  ਜੇ (ਉਪਲਬਧ_len >= needed)
    {
      ਲਈ (i = 0; i < ਦਲੀਲਾਂ_len; i++)
        {
          strcpy (new_argv + so_far, ਦਲੀਲਾਂ[i]);
          so_far += ਲੜੀ_ਲੰਬਾਈ (ਦਲੀਲਾਂ[i]) + 1;
        }
    }

  ਜੇ (so_far >= ਉਪਲਬਧ_len)
    so_far = ਉਪਲਬਧ_len - 1;

  memset (argv[0], 0, ਉਪਲਬਧ_len);
  ਸਮ੍ਰਿਤੀ_ਨਕਲ (argv[0], new_argv, so_far);
}

/* Entrypoint to the container.  */
ਸਥਿਰ ਪੂਰਨ
container_init (ਖਾਲੀ *ਦਲੀਲਾਂ, ਅੱਖਰ *notify_socket, ਪੂਰਨ sync_socket, libcrun_error_t *err)
{
  ਬਣਤਰ container_entrypoint_s *entrypoint_ਦਲੀਲਾਂ = ਦਲੀਲਾਂ;
  ਪੂਰਨ ret;
  runtime_spec_schema_config_schema *def = entrypoint_ਦਲੀਲਾਂ->container->container_def;
  cleanup_free ਅੱਖਰ *exec_ਰਸਤਾ = ਨਿਰਾ;
  __attribute__ ((ਅਣਵਰਤਿਆ)) cleanup_free ਅੱਖਰ *notify_socket_cleanup = notify_socket;
  pid_t own_pid = 0;

  entrypoint_ਦਲੀਲਾਂ->sync_socket = sync_socket;

  crun_ਸੈੱਟ_output_handler (log_write_ਮੰਜ਼ਿਲ_sync_socket, ਦਲੀਲਾਂ);

  /* sync receive own pid.  */
  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (sync_socket, &own_pid, ਆਕਾਰ (own_pid)));
  ਜੇ (UNLIKELY (ret != ਆਕਾਰ (own_pid)))
    {
      ਜੇ (ret >= 0)
        errno = 0;
      ਵਾਪਸ crun_make_error (err, errno, "read from sync socket");
    }

  ret = container_init_ਤਿਆਰੀ (ਦਲੀਲਾਂ, &own_pid, notify_socket, sync_socket, &exec_ਰਸਤਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      /* If it fails to write the error using the sync socket, then fallback
         to stderr.  */
      ਜੇ (sync_socket_write_error (sync_socket, err) == 0)
        {
          crun_error_release (err);
          /* A return value of > 0 means to fail with an error without err being set so to avoid
             printing it twice.  */
          ਵਾਪਸ 1;
        }
      ਵਾਪਸ ret;
    }

  entrypoint_ਦਲੀਲਾਂ->sync_socket = -1;

  ret = unblock_ਸੰਕੇਤ (err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  /* sync 4.  */
  ret = sync_socket_send_sync (sync_socket, false, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  close_and_reset (&sync_socket);

  ਜੇ (entrypoint_ਦਲੀਲਾਂ->context->fifo_exec_wait_fd >= 0)
    {
      ਅੱਖਰ ਬਫਰ[1];
      fd_set read_ਸੈੱਟ;
      cleanup_close ਪੂਰਨ fd = entrypoint_ਦਲੀਲਾਂ->context->fifo_exec_wait_fd;
      entrypoint_ਦਲੀਲਾਂ->context->fifo_exec_wait_fd = -1;

      FD_ZERO (&read_ਸੈੱਟ);
      FD_SET (fd, &read_ਸੈੱਟ);
      ਕਰੋ
        {
          ret = select (fd + 1, &read_ਸੈੱਟ, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "select");

          ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (fd, ਬਫਰ, ਆਕਾਰ (ਬਫਰ)));
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "read from the exec fifo");
      } ਜਦੋਂ (ret == 0);

      close_and_reset (&entrypoint_ਦਲੀਲਾਂ->context->fifo_exec_wait_fd);
    }

  crun_ਸੈੱਟ_output_handler (log_write_ਮੰਜ਼ਿਲ_stderr, ਨਿਰਾ);

  ਜੇ (def->process && def->process->no_new_privileges)
    {
      ਅੱਖਰ **seccomp_flags = ਨਿਰਾ;
      size_t seccomp_flags_len = 0;
      cleanup_free ਅੱਖਰ *seccomp_fd_payload = ਨਿਰਾ;
      size_t seccomp_fd_payload_len = 0;

      ਜੇ (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      ਜੇ (entrypoint_ਦਲੀਲਾਂ->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (entrypoint_ਦਲੀਲਾਂ->container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_ਦਲੀਲਾਂ->seccomp_fd, entrypoint_ਦਲੀਲਾਂ->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags,
                                   seccomp_flags_len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
      close_and_reset (&entrypoint_ਦਲੀਲਾਂ->seccomp_fd);
      close_and_reset (&entrypoint_ਦਲੀਲਾਂ->seccomp_receiver_fd);
    }

  ਜੇ (UNLIKELY (def->process == ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, 0, "block `process` not found");

  ਜੇ (UNLIKELY (exec_ਰਸਤਾ == ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, 0, "executable path not specified");

  ਜੇ (def->hooks && def->hooks->ਸ਼ੁਰੂ_container_len)
    {
      libcrun_container_t *container = entrypoint_ਦਲੀਲਾਂ->container;
      libcrun_error_t tmp_err = ਨਿਰਾ;
      ਪੂਰਨ ਵਿੱਚ_userns = check_running_ਵਿੱਚ_user_namespace (&tmp_err);
      ਜੇ (tmp_err)
        crun_error_release (&tmp_err);

      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_ਦਲੀਲਾਂ->bundle, ਨਿਰਾ, "created", "startContainer",
                      (hook **) def->hooks->ਸ਼ੁਰੂ_container,
                      def->hooks->ਸ਼ੁਰੂ_container_len, entrypoint_ਦਲੀਲਾਂ->hooks_ਬਾਹਰਲਾ_fd, entrypoint_ਦਲੀਲਾਂ->hooks_err_fd,
                      ਵਿੱਚ_userns > 0, err);
      ਜੇ (UNLIKELY (ret != 0))
        ਵਾਪਸ ret;

      /* Seek stdout/stderr to the end.  If the hooks were using the same files,
         the container process overwrites what was previously written.  */
      (ਖਾਲੀ) lseek (1, 0, SEEK_END);
      (ਖਾਲੀ) lseek (2, 0, SEEK_END);
    }

  ਜੇ (entrypoint_ਦਲੀਲਾਂ->custom_handler)
    {
      /* Files marked with O_CLOEXEC are closed at execv time, so make sure they are closed now.
         This is a best effort operation, because the seccomp filter is already in place and it could
         stop some syscalls used by mark_or_close_fds_ge_than.
      */
      ਜੇ (entrypoint_ਦਲੀਲਾਂ->custom_handler->vtable->close_fds)
        {
          ret = entrypoint_ਦਲੀਲਾਂ->custom_handler->vtable->close_fds (entrypoint_ਦਲੀਲਾਂ->custom_handler->cookie,
                                                                    entrypoint_ਦਲੀਲਾਂ->container,
                                                                    entrypoint_ਦਲੀਲਾਂ->context->preserve_fds,
                                                                    err);
        }
      ਨਹੀਂਤਾਂ
        {
          ret = mark_or_close_fds_ge_than (entrypoint_ਦਲੀਲਾਂ->container, entrypoint_ਦਲੀਲਾਂ->context->preserve_fds + 3, true, err);
        }
      ਜੇ (UNLIKELY (ret < 0))
        crun_error_release (err);

      prctl (PR_ਸੈੱਟ_NAME, entrypoint_ਦਲੀਲਾਂ->custom_handler->vtable->name);

      ਜੇ (entrypoint_ਦਲੀਲਾਂ->context->argv)
        {
          erase_initial_environment (entrypoint_ਦਲੀਲਾਂ->context->argv, entrypoint_ਦਲੀਲਾਂ->context->argc);
          rewrite_argv (entrypoint_ਦਲੀਲਾਂ->context->argv, entrypoint_ਦਲੀਲਾਂ->context->argc,
                        entrypoint_ਦਲੀਲਾਂ->custom_handler->vtable->name, def->process->ਦਲੀਲਾਂ,
                        def->process->ਦਲੀਲਾਂ_len);

          /* It is a quite destructive operation as we might be referencing data from the old
             argv, so make sure the context is not reused.  */
          entrypoint_ਦਲੀਲਾਂ->context->argv = ਨਿਰਾ;
          entrypoint_ਦਲੀਲਾਂ->context = ਨਿਰਾ;
        }

      ret = libcrun_ਸੈੱਟ_selinux_label (entrypoint_ਦਲੀਲਾਂ->container, def->process, true, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = libcrun_ਸੈੱਟ_apparmor_profile (entrypoint_ਦਲੀਲਾਂ->container, def->process, true, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = entrypoint_ਦਲੀਲਾਂ->custom_handler->vtable->ਲੜੀਵਾਰ_func (entrypoint_ਦਲੀਲਾਂ->custom_handler->cookie,
                                                               entrypoint_ਦਲੀਲਾਂ->container,
                                                               exec_ਰਸਤਾ,
                                                               def->process->ਦਲੀਲਾਂ);
      ਜੇ (ret != 0)
        ਵਾਪਸ crun_make_error (err, 0, "exec container process failed with handler as `%s`", entrypoint_ਦਲੀਲਾਂ->custom_handler->vtable->name);

      ਵਾਪਸ ret;
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation since the seccomp profile is already in place now and might block
     some of the syscalls needed by mark_or_close_fds_ge_than.  */
  ret = mark_or_close_fds_ge_than (entrypoint_ਦਲੀਲਾਂ->container, entrypoint_ਦਲੀਲਾਂ->context->preserve_fds + 3, true, err);
  ਜੇ (UNLIKELY (ret < 0))
    crun_error_release (err);

  ਆਰਜ਼ੀ_FAILURE_RETRY (execv (exec_ਰਸਤਾ, def->process->ਦਲੀਲਾਂ));

  ਜੇ (errno == ENOENT)
    ਵਾਪਸ crun_make_error (err, errno, "exec container process (missing dynamic library?) `%s`", exec_ਰਸਤਾ);

  ਵਾਪਸ crun_make_error (err, errno, "exec container process `%s`", exec_ਰਸਤਾ);
}

ਸਥਿਰ ਪੂਰਨ
read_container_config_from_state (libcrun_container_t **container, ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ *id,
                                  libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *config_file = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *dir = ਨਿਰਾ;
  ਪੂਰਨ ret;

  *container = ਨਿਰਾ;

  ret = libcrun_get_state_directory (&dir, state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = append_paths (&config_file, err, dir, "config.json", ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  *container = libcrun_container_ਲੱਦੋ_from_file (config_file, err);
  ਜੇ (*container == ਨਿਰਾ)
    ਵਾਪਸ -1;

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
ਲੜੀਵਾਰ_poststop_hooks (arg_ਅਣਵਰਤਿਆ libcrun_context_t *context, libcrun_container_t *container,
                    runtime_spec_schema_config_schema *def,
                    libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container_cleanup = ਨਿਰਾ;
  ਪੂਰਨ ret;

  ਜੇ (def == ਨਿਰਾ)
    {
      ਜੇ (container == ਨਿਰਾ)
        {
          ret = read_container_config_from_state (&container_cleanup, state_ਜੜ੍ਹ, id, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
          container = container_cleanup;
        }

      def = container->container_def;
    }

  ਜੇ (def->hooks && def->hooks->poststop_len)
    {
      cleanup_close ਪੂਰਨ hooks_ਬਾਹਰਲਾ_fd = -1;
      cleanup_close ਪੂਰਨ hooks_err_fd = -1;

      ਜੇ (container == ਨਿਰਾ)
        {
          ret = read_container_config_from_state (&container_cleanup, state_ਜੜ੍ਹ, id, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
          container = container_cleanup;
        }

      ret = open_hooks_output (container, &hooks_ਬਾਹਰਲਾ_fd, &hooks_err_fd, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = do_hooks (def, 0, id, true, ਸਥਿਤੀ->bundle, ਸਥਿਤੀ->bundle, "stopped", "poststop", (hook **) def->hooks->poststop,
                      def->hooks->poststop_len, hooks_ਬਾਹਰਲਾ_fd, hooks_err_fd, false, err);
      ਜੇ (UNLIKELY (ret != 0))
        {
          ਜੇ (ret < 0)
            libcrun_error_report_and_release (err);
          ਨਹੀਂਤਾਂ
            libcrun_error (0, "poststop hook failed with exit code: %d", ret);
        }
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ bool
has_new_pid_namespace (runtime_spec_schema_config_schema *def)
{
  size_t i;

  ਜੇ (def->linux == ਨਿਰਾ || def->linux->namespaces == ਨਿਰਾ)
    ਵਾਪਸ false;

  ਲਈ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ਜੇ (strcmp (def->linux->namespaces[i]->type, "pid") == 0 && def->linux->namespaces[i]->ਰਸਤਾ == ਨਿਰਾ)
        ਵਾਪਸ true;
    }
  ਵਾਪਸ false;
}

/* Delete the container state.  CONTAINER is the container the caller already has
   available, or NULL.  When it is not NULL, the poststop hooks are run even if the
   status file was never written, e.g. when the container creation failed.  */
ਸਥਿਰ ਪੂਰਨ
container_delete_ਅੰਦਰੂਨੀ (libcrun_context_t *context, libcrun_container_t *container,
                           runtime_spec_schema_config_schema *def, ਅਚਲ ਅੱਖਰ *id, bool force, bool killall,
                           libcrun_error_t *err)
{
  cleanup_cgroup_ਸਥਿਤੀ ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ = ਨਿਰਾ;
  cleanup_container_ਸਥਿਤੀ libcrun_container_ਸਥਿਤੀ_t ਸਥਿਤੀ = {};
  cleanup_container libcrun_container_t *container_cleanup = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ = context->state_ਜੜ੍ਹ;
  ਪੂਰਨ ret;

  ret = libcrun_read_container_ਸਥਿਤੀ (&ਸਥਿਤੀ, state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਜੇ (force && crun_error_get_errno (err) == ENOENT)
        {
          libcrun_error_t tmp_err = ਨਿਰਾ;

          crun_error_release (err);

          ਜੇ (container)
            {
              libcrun_container_ਸਥਿਤੀ_t partial_ਸਥਿਤੀ = {
                .bundle = (ਅੱਖਰ *) context->bundle,
              };

              ret = ਲੜੀਵਾਰ_poststop_hooks (context, container, def, &partial_ਸਥਿਤੀ, state_ਜੜ੍ਹ, id, err);
              ਜੇ (UNLIKELY (ret < 0))
                crun_error_write_warning_and_release (context->output_handler_arg, &err);
            }

          libcrun_container_delete_ਸਥਿਤੀ (state_ਜੜ੍ਹ, id, &tmp_err);
          crun_error_release (&tmp_err);
          ਵਾਪਸ 0;
        }
      crun_error_release (err);
      ਵਾਪਸ libcrun_container_delete_ਸਥਿਤੀ (state_ਜੜ੍ਹ, id, err);
    }

  cgroup_ਸਥਿਤੀ = libcrun_cgroup_make_ਸਥਿਤੀ (&ਸਥਿਤੀ);

  ਜੇ (! force)
    {
      ret = libcrun_is_container_running (&ਸਥਿਤੀ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
      ਜੇ (ret == 1)
        {
          ret = libcrun_ਸਥਿਤੀ_has_read_exec_fifo (state_ਜੜ੍ਹ, id, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
          ਜੇ (ret == 0)
            ਵਾਪਸ crun_make_error (err, 0, "the container `%s` is not in `created` or `stopped` state", id);

          /* If the container is in "created" state, then do the equivalent of delete --force.  */
          killall = force = true;
        }
    }

  ਜੇ (def == ਨਿਰਾ)
    {
      ਜੇ (container == ਨਿਰਾ)
        {
          ret = read_container_config_from_state (&container_cleanup, state_ਜੜ੍ਹ, id, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;

          container = container_cleanup;
        }

      def = container->container_def;
    }

  ਜੇ (killall && force)
    {
      /* If the container has a pid namespace, it is enough to kill the first
         process (pid=1 in the namespace).
      */
      ਜੇ (has_new_pid_namespace (def))
        {
          ret = libcrun_kill_linux (&ਸਥਿਤੀ, SIGKILL, err);
          ਜੇ (UNLIKELY (ret < 0))
            {
              ਜੇ (crun_error_get_errno (err) != ESRCH)
                ਵਾਪਸ ret;

              crun_error_release (err);
            }
        }
      ਨਹੀਂਤਾਂ ਜੇ (ਸਥਿਤੀ.cgroup_ਰਸਤਾ)
        {
          ret = libcrun_cgroup_killall (cgroup_ਸਥਿਤੀ, SIGKILL, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }
    }

  ਜੇ (def->linux && def->linux->intel_rdt)
    {
      ret = libcrun_destroy_intelrdt (id, def, err);
      ਜੇ (UNLIKELY (ret < 0))
        libcrun_error_report_and_release (err);
    }

  ਜੇ (ਸਥਿਤੀ.cgroup_ਰਸਤਾ)
    {
      /* If the cgroup cannot be destroyed, there are still processes in it.
         Keep the container status, it is the only record of the cgroup, so
         that the delete can be retried.  */
      ret = libcrun_cgroup_destroy (cgroup_ਸਥਿਤੀ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ret = ਲੜੀਵਾਰ_poststop_hooks (context, container, def, &ਸਥਿਤੀ, state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ਵਾਪਸ libcrun_container_delete_ਸਥਿਤੀ (state_ਜੜ੍ਹ, id, err);
}

ਪੂਰਨ
libcrun_container_delete (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, bool force, libcrun_error_t *err)
{
  ਵਾਪਸ container_delete_ਅੰਦਰੂਨੀ (context, ਨਿਰਾ, ਨਿਰਾ, id, force, true, err);
}

ਪੂਰਨ
libcrun_container_kill (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *ਸੰਕੇਤ, libcrun_error_t *err)
{
  ਪੂਰਨ sig, ret;
  ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ = context->state_ਜੜ੍ਹ;
  cleanup_container_ਸਥਿਤੀ libcrun_container_ਸਥਿਤੀ_t ਸਥਿਤੀ = {};

  sig = str2sig (ਸੰਕੇਤ);
  ਜੇ (UNLIKELY (sig < 0))
    ਵਾਪਸ crun_make_error (err, 0, "unknown signal `%s`", ਸੰਕੇਤ);

  ret = libcrun_read_container_ਸਥਿਤੀ (&ਸਥਿਤੀ, state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ libcrun_kill_linux (&ਸਥਿਤੀ, sig, err);
}

ਪੂਰਨ
libcrun_container_killall (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *ਸੰਕੇਤ, libcrun_error_t *err)
{
  ਪੂਰਨ sig, ret;
  ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ = context->state_ਜੜ੍ਹ;
  cleanup_container_ਸਥਿਤੀ libcrun_container_ਸਥਿਤੀ_t ਸਥਿਤੀ = {};
  cleanup_cgroup_ਸਥਿਤੀ ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ = ਨਿਰਾ;

  sig = str2sig (ਸੰਕੇਤ);
  ਜੇ (UNLIKELY (sig < 0))
    ਵਾਪਸ crun_make_error (err, 0, "unknown signal `%s`", ਸੰਕੇਤ);

  ret = libcrun_read_container_ਸਥਿਤੀ (&ਸਥਿਤੀ, state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  cgroup_ਸਥਿਤੀ = libcrun_cgroup_make_ਸਥਿਤੀ (&ਸਥਿਤੀ);

  ret = libcrun_cgroup_killall (cgroup_ਸਥਿਤੀ, sig, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
write_container_ਸਥਿਤੀ (libcrun_container_t *container, libcrun_context_t *context,
                        pid_t pid, ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ,
                        libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *bundle_allocated = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *bundle = get_bundle_ਰਸਤਾ (context->bundle, &bundle_allocated);
  cleanup_free ਅੱਖਰ *owner = get_user_name (geteuid ());
  ਅੱਖਰ *external_descriptors = libcrun_get_external_descriptors (container);
  ਅੱਖਰ *rootfs = container->container_def->ਜੜ੍ਹ ? container->container_def->ਜੜ੍ਹ->ਰਸਤਾ : "";
  ਅੱਖਰ created[35];

  libcrun_container_ਸਥਿਤੀ_t ਸਥਿਤੀ = {
    .pid = pid,
    .rootfs = rootfs,
    .bundle = (ਅੱਖਰ *) bundle,
    .created = created,
    .owner = owner,
    .systemd_cgroup = context->systemd_cgroup,
    .detached = context->detach,
    .no_new_keyring = context->no_new_keyring,
    .external_descriptors = external_descriptors,
    .cgroup_ਰਸਤਾ = ਨਿਰਾ,
    .scope = ਨਿਰਾ,
  };

  get_current_timestamp (created, ਆਕਾਰ (created));

  ਜੇ (cgroup_ਸਥਿਤੀ)
    {
      ਪੂਰਨ ret;

      ret = libcrun_cgroup_get_ਸਥਿਤੀ (cgroup_ਸਥਿਤੀ, &ਸਥਿਤੀ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (external_descriptors == ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, 0, "invalid internal state.  No external descriptors found");
  ਵਾਪਸ libcrun_write_container_ਸਥਿਤੀ (context->state_ਜੜ੍ਹ, context->id, &ਸਥਿਤੀ, err);
}

ਸਥਿਰ ਪੂਰਨ
reap_subprocesses (pid_t *main_process, ਪੂਰਨ *main_process_exit, ਪੂਰਨ *ਆਖਰੀ_process, libcrun_error_t *err)
{
  *ਆਖਰੀ_process = 0;
  ਜਦੋਂ (1)
    {
      ਪੂਰਨ ਸਥਿਤੀ;
      ਪੂਰਨ r = waitpid_ignore_stopped (-1, &ਸਥਿਤੀ, WNOHANG);
      ਜੇ (r < 0)
        {
          ਜੇ (errno == EINTR)
            ਜਾਰੀ;
          ਜੇ (errno == ECHILD)
            {
              *ਆਖਰੀ_process = 1;
              ਵਾਪਸ 0;
            }
          ਵਾਪਸ crun_make_error (err, errno, "waitpid");
        }
      ਜੇ (r == 0)
        ਰੋਕੋ;
      ਜੇ (r != *main_process)
        ਜਾਰੀ;

      *main_process_exit = get_process_exit_ਸਥਿਤੀ (ਸਥਿਤੀ);

      /* Once reaped, the pid can be reused by an unrelated process.  */
      *main_process = 0;
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
send_sd_notify (ਅਚਲ ਅੱਖਰ *ready_str, libcrun_error_t *err)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYSTEMD
  ਪੂਰਨ ret = sd_notify (0, ready_str);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, -ret, "sd_notify");

#  ਜੇ HAVE_SD_NOTIFY_BARRIER
  /* Hard-code a 30 seconds timeout.  Ignore errors.  */
  sd_notify_barrier (0, 30 * 1000000);
#  endif
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) ready_str;
  (ਖਾਲੀ) err;
#ਜੇ_ਅੰਤ
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
handle_notify_socket (ਪੂਰਨ notify_socketfd, libcrun_error_t *err)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYSTEMD
  ਪੂਰਨ ret;
  ਅੱਖਰ buf[256];
  ਅਚਲ ਅੱਖਰ *ready_str = "READY=1";

  ret = recvfrom (notify_socketfd, buf, ਆਕਾਰ (buf) - 1, 0, ਨਿਰਾ, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "recvfrom notify socket");

  buf[ret] = '\0';
  ਜੇ (ਲੜੀ_ਖੋਜ (buf, ready_str))
    {
      ret = send_sd_notify (ready_str, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ਵਾਪਸ 1;
    }
  ਵਾਪਸ 0;
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) notify_socketfd;
  (ਖਾਲੀ) err;
  ਵਾਪਸ 1;
#ਜੇ_ਅੰਤ
}

ਬਣਤਰ wait_for_process_ਦਲੀਲਾਂ
{
  pid_t pid;
  libcrun_context_t *context;
  ਪੂਰਨ terminal_fd;
  ਪੂਰਨ notify_socket;
  ਪੂਰਨ *container_ready_fd;
  ਪੂਰਨ seccomp_notify_fd;
  ਅਚਲ ਅੱਖਰ *seccomp_notify_plugins;
};

ਸਥਿਰ ਪੂਰਨ
write_pid_file (ਅਚਲ ਅੱਖਰ *pid_file, pid_t pid, libcrun_error_t *err)
{
  ਅੱਖਰ buf[32];
  ਪੂਰਨ buf_len = snprintf (buf, ਆਕਾਰ (buf), "%d", pid);
  ਜੇ (UNLIKELY (buf_len >= (ਪੂਰਨ) ਆਕਾਰ (buf)))
    ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");
  ਵਾਪਸ write_file_at_with_flags (AT_FDCWD, O_CREAT | O_TRUNC, 0700, pid_file, buf, buf_len, err);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_EVENTS 10

ਸਥਿਰ bool
is_fd_pollable (ਪੂਰਨ fd)
{
  ਬਣਤਰ epoll_event ev = { .events = EPOLLIN };
  cleanup_close ਪੂਰਨ epollfd = epoll_create1 (EPOLL_CLOEXEC);

  ਜੇ (epollfd < 0)
    ਵਾਪਸ true;

  ਵਾਪਸ epoll_ctl (epollfd, EPOLL_CTL_ਜੋੜੋ, fd, &ev) == 0 || errno != EPERM;
}

ਸਥਿਰ ਪੂਰਨ
wait_for_process (ਬਣਤਰ wait_for_process_ਦਲੀਲਾਂ *ਦਲੀਲਾਂ, libcrun_error_t *err)
{
  cleanup_channel_fd_pair ਬਣਤਰ channel_fd_pair *from_terminal = ਨਿਰਾ;
  cleanup_channel_fd_pair ਬਣਤਰ channel_fd_pair *ਮੰਜ਼ਿਲ_terminal = ਨਿਰਾ;
  ਪੂਰਨ ret, container_exit_ਕੋਡ = 0, ਆਖਰੀ_process;
  cleanup_close ਪੂਰਨ terminal_fd_from = -1;
  cleanup_close ਪੂਰਨ terminal_fd_ਮੰਜ਼ਿਲ = -1;
  bool stdin_pollable = false, stdout_pollable = false;
  cleanup_close ਪੂਰਨ epollfd = -1;
  cleanup_close ਪੂਰਨ signalfd = -1;
  sigset_t ਨਕਾਬ;
  ਪੂਰਨ ਵਿੱਚ_fds[MAX_EVENTS];
  ਪੂਰਨ ਵਿੱਚ_fds_len = 0;
  ਪੂਰਨ ਬਾਹਰਲਾ_fds[MAX_EVENTS];
  ਪੂਰਨ ਬਾਹਰਲਾ_fds_len = 0;
  size_t i;

  cleanup_seccomp_notify_context ਬਣਤਰ seccomp_notify_context_s *seccomp_notify_ctx = ਨਿਰਾ;

  container_exit_ਕੋਡ = 0;

  ਜੇ (ਦਲੀਲਾਂ == ਨਿਰਾ || ਦਲੀਲਾਂ->context == ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, 0, "internal error: context is empty");

  ਲਈ (i = 0; i < MAX_EVENTS; i++)
    {
      ਵਿੱਚ_fds[i] = -1;
      ਬਾਹਰਲਾ_fds[i] = -1;
    }

  ਜੇ (ਦਲੀਲਾਂ->context->pid_file)
    {
      ret = write_pid_file (ਦਲੀਲਾਂ->context->pid_file, ਦਲੀਲਾਂ->pid, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  /* Also exit if there is nothing more to wait for.  */
  ਜੇ (ਦਲੀਲਾਂ->context->detach && ਦਲੀਲਾਂ->notify_socket < 0)
    ਵਾਪਸ 0;

  ਜੇ (ਦਲੀਲਾਂ->container_ready_fd)
    {
      ret = 0;
      ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (*ਦਲੀਲਾਂ->container_ready_fd, &ret, ਆਕਾਰ (ret)));
      close_and_reset (ਦਲੀਲਾਂ->container_ready_fd);
    }

  sigfillset (&ਨਕਾਬ);
  ret = sigprocmask (SIG_ਖੰਡ, &ਨਕਾਬ, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "sigprocmask");

  signalfd = create_signalfd (&ਨਕਾਬ, err);
  ਜੇ (UNLIKELY (signalfd < 0))
    ਵਾਪਸ signalfd;

  ret = reap_subprocesses (&ਦਲੀਲਾਂ->pid, &container_exit_ਕੋਡ, &ਆਖਰੀ_process, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (ਆਖਰੀ_process)
    ਵਾਪਸ container_exit_ਕੋਡ;

  ਜੇ (ਦਲੀਲਾਂ->seccomp_notify_fd >= 0)
    {
      cleanup_free ਅੱਖਰ *state_ਜੜ੍ਹ = ਨਿਰਾ;
      cleanup_free ਅੱਖਰ *oci_config_ਰਸਤਾ = ਨਿਰਾ;

      ਬਣਤਰ libcrun_ਲੱਦੋ_seccomp_notify_conf_s conf;
      memset (&conf, 0, ਆਕਾਰ conf);

      ret = libcrun_get_state_directory (&state_ਜੜ੍ਹ, ਦਲੀਲਾਂ->context->state_ਜੜ੍ਹ, ਦਲੀਲਾਂ->context->id, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = append_paths (&oci_config_ਰਸਤਾ, err, state_ਜੜ੍ਹ, "config.json", ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      conf.runtime_ਜੜ੍ਹ_ਰਸਤਾ = state_ਜੜ੍ਹ;
      conf.name = ਦਲੀਲਾਂ->context->id;
      conf.bundle_ਰਸਤਾ = ਦਲੀਲਾਂ->context->bundle;
      conf.oci_config_ਰਸਤਾ = oci_config_ਰਸਤਾ;

      ret = ਸੈੱਟ_blocking_fd (ਦਲੀਲਾਂ->seccomp_notify_fd, false, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = libcrun_ਲੱਦੋ_seccomp_notify_plugins (&seccomp_notify_ctx,
                                                 ਦਲੀਲਾਂ->seccomp_notify_plugins,
                                                 &conf, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ਵਿੱਚ_fds[ਵਿੱਚ_fds_len++] = ਦਲੀਲਾਂ->seccomp_notify_fd;
    }

  ਜੇ (ਦਲੀਲਾਂ->terminal_fd >= 0)
    {
      /* The terminal_fd is dup()ed so that it can be registered with
         epoll multiple times using different masks.  */
      terminal_fd_from = dup (ਦਲੀਲਾਂ->terminal_fd);
      ਜੇ (UNLIKELY (terminal_fd_from < 0))
        ਵਾਪਸ crun_make_error (err, errno, "dup terminal fd");
      terminal_fd_ਮੰਜ਼ਿਲ = dup (ਦਲੀਲਾਂ->terminal_fd);
      ਜੇ (UNLIKELY (terminal_fd_ਮੰਜ਼ਿਲ < 0))
        ਵਾਪਸ crun_make_error (err, errno, "dup terminal fd");

      ਪੂਰਨ i, non_blocking_fds[] = { terminal_fd_from, terminal_fd_ਮੰਜ਼ਿਲ, 0, 1, -1 };
      ਲਈ (i = 0; non_blocking_fds[i] >= 0; i++)
        {
          ret = ਸੈੱਟ_blocking_fd (non_blocking_fds[i], false, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }

      /* stdin and stdout can be something epoll does not support, such as
         /dev/null or a regular file.  Such a stdin is not forwarded, and
         such a stdout is written to without waiting for it to be ready.  */
      stdin_pollable = is_fd_pollable (0);
      stdout_pollable = is_fd_pollable (1);

      from_terminal = channel_fd_pair_new (terminal_fd_from, 1, BUFSIZ);
      ਜੇ (stdin_pollable)
        ਮੰਜ਼ਿਲ_terminal = channel_fd_pair_new (0, terminal_fd_ਮੰਜ਼ਿਲ, BUFSIZ);
    }

  ਵਿੱਚ_fds[ਵਿੱਚ_fds_len++] = signalfd;
  ਜੇ (ਦਲੀਲਾਂ->notify_socket >= 0)
    ਵਿੱਚ_fds[ਵਿੱਚ_fds_len++] = ਦਲੀਲਾਂ->notify_socket;
  ਜੇ (ਦਲੀਲਾਂ->terminal_fd >= 0)
    {
      ਜੇ (stdin_pollable)
        {
          ਵਿੱਚ_fds[ਵਿੱਚ_fds_len++] = 0;
          ਬਾਹਰਲਾ_fds[ਬਾਹਰਲਾ_fds_len++] = terminal_fd_ਮੰਜ਼ਿਲ;
        }

      ਵਿੱਚ_fds[ਵਿੱਚ_fds_len++] = terminal_fd_from;
      ਜੇ (stdout_pollable)
        ਬਾਹਰਲਾ_fds[ਬਾਹਰਲਾ_fds_len++] = 1;
    }

  epollfd = epoll_helper (ਵਿੱਚ_fds, ਨਿਰਾ, ਬਾਹਰਲਾ_fds, ਨਿਰਾ, err);
  ਜੇ (UNLIKELY (epollfd < 0))
    ਵਾਪਸ epollfd;

  ਜਦੋਂ (1)
    {
      ਬਣਤਰ epoll_event events[MAX_EVENTS];
      ਬਣਤਰ signalfd_siginfo si;
      ਬਣਤਰ winsize ws;
      ਪੂਰਨ i, nr_events;
      ssize_t res;

      nr_events = ਆਰਜ਼ੀ_FAILURE_RETRY (epoll_wait (epollfd, events, MAX_EVENTS, -1));
      ਜੇ (UNLIKELY (nr_events < 0))
        ਵਾਪਸ crun_make_error (err, errno, "epoll_wait");

      ਲਈ (i = 0; i < nr_events; i++)
        {
          ਜੇ (events[i].ਅੰਕੜੇ.fd == 0 || events[i].ਅੰਕੜੇ.fd == terminal_fd_ਮੰਜ਼ਿਲ)
            {
              ret = channel_fd_pair_process (ਮੰਜ਼ਿਲ_terminal, epollfd, err);
              ਜੇ (UNLIKELY (ret < 0))
                ਵਾਪਸ crun_error_wrap (err, "copy to terminal fd");
            }
          ਨਹੀਂਤਾਂ ਜੇ (events[i].ਅੰਕੜੇ.fd == 1 || events[i].ਅੰਕੜੇ.fd == terminal_fd_from)
            {
              ret = channel_fd_pair_process (from_terminal, epollfd, err);
              ਜੇ (UNLIKELY (ret < 0))
                ਵਾਪਸ crun_error_wrap (err, "copy from terminal fd");
            }
          ਨਹੀਂਤਾਂ ਜੇ (events[i].ਅੰਕੜੇ.fd == ਦਲੀਲਾਂ->seccomp_notify_fd)
            {
              ret = libcrun_seccomp_notify_plugins (seccomp_notify_ctx,
                                                    ਦਲੀਲਾਂ->seccomp_notify_fd, err);
              ਜੇ (UNLIKELY (ret < 0))
                ਵਾਪਸ ret;
            }
          ਨਹੀਂਤਾਂ ਜੇ (events[i].ਅੰਕੜੇ.fd == ਦਲੀਲਾਂ->notify_socket)
            {
              ret = handle_notify_socket (ਦਲੀਲਾਂ->notify_socket, err);
              ਜੇ (UNLIKELY (ret < 0))
                ਵਾਪਸ ret;
              ਜੇ (ret && ਦਲੀਲਾਂ->context->detach)
                ਵਾਪਸ 0;
            }
          ਨਹੀਂਤਾਂ ਜੇ (events[i].ਅੰਕੜੇ.fd == signalfd)
            {
              res = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (signalfd, &si, ਆਕਾਰ (si)));
              ਜੇ (UNLIKELY (res < 0))
                ਵਾਪਸ crun_make_error (err, errno, "read from signalfd");
              ਜੇ (si.ssi_signo == SIGCHLD)
                {
                  ret = reap_subprocesses (&ਦਲੀਲਾਂ->pid, &container_exit_ਕੋਡ,
                                           &ਆਖਰੀ_process, err);
                  ਜੇ (UNLIKELY (ret < 0))
                    ਵਾਪਸ ret;
                  ਜੇ (ਆਖਰੀ_process)
                    ਵਾਪਸ container_exit_ਕੋਡ;
                }
              ਨਹੀਂਤਾਂ ਜੇ (si.ssi_signo == SIGWINCH)
                {
                  /* Ignore the signal if the terminal is not available.  */
                  ਜੇ (ਦਲੀਲਾਂ->terminal_fd > 0)
                    {
                      ret = ioctl (0, TIOCGWINSZ, &ws);
                      ਜੇ (UNLIKELY (ret < 0))
                        ਵਾਪਸ crun_make_error (err, errno, "ioctl TIOCGWINSZ copy terminal size from stdin");

                      ret = ioctl (ਦਲੀਲਾਂ->terminal_fd, TIOCSWINSZ, &ws);
                      ਜੇ (UNLIKELY (ret < 0))
                        ਵਾਪਸ crun_make_error (err, errno, "ioctl TIOCSWINSZ copy terminal size to pty");
                    }
                }
              ਨਹੀਂਤਾਂ ਜੇ (ਦਲੀਲਾਂ->pid > 0)
                {
                  /* Send any other signal to the child process, if it was not reaped yet.  */
                  ret = kill (ਦਲੀਲਾਂ->pid, si.ssi_signo);
                }
            }
          ਨਹੀਂਤਾਂ
            {
              ਵਾਪਸ crun_make_error (err, 0, "internal error: unknown fd from epoll_wait");
            }
        }
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ
flush_fd_ਮੰਜ਼ਿਲ_err (libcrun_context_t *context, ਪੂਰਨ terminal_fd)
{
  ਅੱਖਰ buf[256];
  ਪੂਰਨ flags;

  ਜੇ (terminal_fd < 0 || stderr == ਨਿਰਾ)
    ਵਾਪਸ;

  flags = fcntl (terminal_fd, F_GETFL, 0);
  ਜੇ (flags == -1)
    ਵਾਪਸ;
  ਜੇ (fcntl (terminal_fd, F_SETFL, flags | O_NONBLOCK) < 0)
    ਵਾਪਸ;

  ਲਈ (;;)
    {
      ਪੂਰਨ ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (terminal_fd, buf, ਆਕਾਰ (buf) - 1));
      ਜੇ (ret <= 0)
        ਰੋਕੋ;
      buf[ret] = '\0';
      ਜੇ (context->output_handler)
        context->output_handler (0, buf, LIBCRUN_VERBOSITY_ERROR, context->output_handler_arg);
    }
  (ਖਾਲੀ) fcntl (terminal_fd, F_SETFL, flags);
  fflush (stderr);
  fsync (1);
  fsync (2);
}

ਸਥਿਰ ਪੂਰਨ
cleanup_watch (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
               ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ, pid_t init_pid, ਪੂਰਨ sync_socket,
               ਪੂਰਨ terminal_fd, libcrun_error_t *err)
{
  ਅਚਲ ਅੱਖਰ *oom_ਸੁਨੇਹਾ = ਨਿਰਾ;
  libcrun_error_t tmp_err = ਨਿਰਾ;
  ਪੂਰਨ ret;

  ਜੇ (init_pid)
    {
      /* Try to detect whether the cgroup has a OOM.  */
      ਜੇ (cgroup_ਸਥਿਤੀ)
        {
          ਪੂਰਨ has_oom;

          has_oom = libcrun_cgroup_has_oom (cgroup_ਸਥਿਤੀ, &tmp_err);
          ਜੇ (has_oom > 0)
            oom_ਸੁਨੇਹਾ = "OOM: the memory limit could be too low";
          ਨਹੀਂਤਾਂ ਜੇ (has_oom < 0)
            {
              /* If the detection has failed for any reason, e.g. the cgroup was
                 already deleted by the time it was checked, just ignore the
                 failure.  */
              crun_error_release (&tmp_err);
            }
        }
      /* If the OOM wasn't detected, look into the static configuration.  */
      ਜੇ (oom_ਸੁਨੇਹਾ == ਨਿਰਾ && is_ਸਮ੍ਰਿਤੀ_ਹੱਦਬੰਦੀ_too_ਨੀਵਾਂ (def))
        oom_ਸੁਨੇਹਾ = "the memory limit could be too low";

      kill (init_pid, SIGKILL);
      waitpid_ignore_stopped (init_pid, ਨਿਰਾ, 0);
    }

  ret = sync_socket_wait_sync (context, sync_socket, true, &tmp_err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      crun_error_release (err);
      *err = tmp_err;
    }

  ਜੇ (terminal_fd >= 0)
    flush_fd_ਮੰਜ਼ਿਲ_err (context, terminal_fd);

  ਜੇ (oom_ਸੁਨੇਹਾ)
    ਵਾਪਸ crun_error_wrap (err, "%s", oom_ਸੁਨੇਹਾ);

  ਵਾਪਸ -1;
}

/* Find the uid:gid that is mapped to root inside the container user namespace.  */
ਖਾਲੀ
get_ਜੜ੍ਹ_ਵਿੱਚ_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid, uid_t *uid,
                        gid_t *gid)
{
  *uid = -1;
  *gid = -1;

  /* Not root in the namespace.  */
  ਜੇ (host_uid)
    ਵਾਪਸ;

  ਜੇ (def->linux && def->linux->uid_mappings)
    {
      size_t i;

      ਲਈ (i = 0; i < def->linux->uid_mappings_len; i++)
        ਜੇ (def->linux->uid_mappings[i]->container_id == 0)
          {
            *uid = def->linux->uid_mappings[i]->host_id;
            ਰੋਕੋ;
          }
    }
  ਜੇ (def->linux && def->linux->gid_mappings)
    {
      size_t i;

      ਲਈ (i = 0; i < def->linux->gid_mappings_len; i++)
        ਜੇ (def->linux->gid_mappings[i]->container_id == 0)
          {
            *gid = def->linux->gid_mappings[i]->host_id;
            ਰੋਕੋ;
          }
    }

  /* If the uid and the gid are not changed, do not attempt any chown.  */
  ਜੇ (*uid == host_uid && *gid == host_gid)
    *uid = *gid = -1;
}

ਸਥਿਰ ਪੂਰਨ
get_seccomp_receiver_fd (libcrun_container_t *container, ਪੂਰਨ *fd, ਪੂਰਨ *self_receiver_fd, ਅਚਲ ਅੱਖਰ **plugins,
                         libcrun_error_t *err)
{
  ਅਚਲ ਅੱਖਰ *tmp;
  runtime_spec_schema_config_schema *def = container->container_def;

  *fd = -1;
  *self_receiver_fd = -1;

  tmp = ਲੱਭੋ_annotation (container, "run.oci.seccomp.plugins");
  ਜੇ (tmp)
    {
      ਪੂਰਨ fds[2];
      ਪੂਰਨ ret;

      ret = create_socket_pair (fds, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_error_wrap (err, "create socket pair");

      *fd = fds[0];
      *self_receiver_fd = fds[1];
      *plugins = tmp;
      ਵਾਪਸ 0;
    }

  ਜੇ (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_ਰਸਤਾ)
    tmp = def->linux->seccomp->listener_ਰਸਤਾ;
  ਨਹੀਂਤਾਂ
    tmp = ਲੱਭੋ_annotation (container, "run.oci.seccomp.receiver");
  ਜੇ (tmp == ਨਿਰਾ)
    tmp = getenv ("RUN_OCI_SECCOMP_RECEIVER");
  ਜੇ (tmp)
    {
      ਜੇ (tmp[0] != '/')
        ਵਾਪਸ crun_make_error (err, 0, "the seccomp receiver `%s` is not an absolute path", tmp);

      *fd = open_unix_domain_client_socket (tmp, 0, err);
      ਜੇ (UNLIKELY (*fd < 0))
        ਵਾਪਸ crun_error_wrap (err, "open seccomp receiver");
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ bool
has_seccomp_receiver (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;

  ਜੇ (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_ਰਸਤਾ)
    ਵਾਪਸ true;

  ਜੇ (ਲੱਭੋ_annotation (container, "run.oci.seccomp.receiver") != ਨਿਰਾ || getenv ("RUN_OCI_SECCOMP_RECEIVER") != ਨਿਰਾ)
    ਵਾਪਸ true;

  ਵਾਪਸ false;
}

ਸਥਿਰ ਪੂਰਨ
ਤਿਆਰੀ_container_hooks_output (libcrun_container_t *container, runtime_spec_schema_config_schema *def,
                              ਬਣਤਰ container_entrypoint_s *container_ਦਲੀਲਾਂ, ਪੂਰਨ *hooks_ਬਾਹਰਲਾ_fd,
                              ਪੂਰਨ *hooks_err_fd, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ਜੇ (def->hooks
      && (def->hooks->prestart_len || def->hooks->poststart_len || def->hooks->create_runtime_len
          || def->hooks->create_container_len || def->hooks->ਸ਼ੁਰੂ_container_len))
    {
      ret = open_hooks_output (container, hooks_ਬਾਹਰਲਾ_fd, hooks_err_fd, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
      container_ਦਲੀਲਾਂ->hooks_ਬਾਹਰਲਾ_fd = *hooks_ਬਾਹਰਲਾ_fd;
      container_ਦਲੀਲਾਂ->hooks_err_fd = *hooks_err_fd;
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
ਤਿਆਰੀ_container_keyring (libcrun_container_t *container, libcrun_context_t *context, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ਪੂਰਨ ret;

  ਜੇ (! context->no_new_keyring)
    {
      ਅਚਲ ਅੱਖਰ *label = ਨਿਰਾ;

      libcrun_debug ("Creating new keyring");

      ਜੇ (def->process)
        {
          label = def->process->selinux_label;
          ਜੇ (label)
            libcrun_debug ("Using SELinux process label: `%s`", label);
        }

      ret = libcrun_create_keyring (container, container->context->id, label, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
ਤਿਆਰੀ_terminal_socket_pair (libcrun_container_t *container, libcrun_context_t *context,
                            ਬਣਤਰ container_entrypoint_s *container_ਦਲੀਲਾਂ, ਪੂਰਨ *socket_pair_0,
                            ਪੂਰਨ *socket_pair_1, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ਪੂਰਨ ret;
  ਪੂਰਨ detach = context->detach;

  ਜੇ (def->process && def->process->terminal && ! detach && context->console_socket == ਨਿਰਾ)
    {
      libcrun_debug ("Creating terminal socket pair");
      container_ਦਲੀਲਾਂ->has_terminal_socket_pair = 1;
      ret = create_socket_pair (container_ਦਲੀਲਾਂ->terminal_socketpair, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_error_wrap (err, "create terminal socket");

      *socket_pair_0 = container_ਦਲੀਲਾਂ->terminal_socketpair[0];
      *socket_pair_1 = container_ਦਲੀਲਾਂ->terminal_socketpair[1];
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
ਤਿਆਰੀ_seccomp (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *seccomp_bpf_ਅੰਕੜੇ,
               ਬਣਤਰ libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, ਪੂਰਨ *seccomp_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ਪੂਰਨ ret;

  ਜੇ (ਲੱਭੋ_annotation (container, "run.oci.seccomp.plugins") != ਨਿਰਾ && has_seccomp_receiver (container))
    {
      ਵਾਪਸ crun_make_error (err, 0, "seccomp plugins and seccomp receivers cannot be declared at the same time");
    }

  ਜੇ (def->linux && (def->linux->seccomp || seccomp_bpf_ਅੰਕੜੇ))
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ seccomp_gen_options = 0;
      ਅਚਲ ਅੱਖਰ *annotation;

      libcrun_debug ("Initializing seccomp");
      annotation = ਲੱਭੋ_annotation (container, "run.oci.seccomp_fail_unknown_syscall");
      ਜੇ (annotation && strcmp (annotation, "0") != 0)
        seccomp_gen_options = LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL;

      ਜੇ (seccomp_bpf_ਅੰਕੜੇ)
        seccomp_gen_options |= LIBCRUN_SECCOMP_SKIP_CACHE;

      libcrun_seccomp_gen_ctx_init (seccomp_gen_ctx, container, true, seccomp_gen_options);

      ret = libcrun_open_seccomp_bpf (seccomp_gen_ctx, seccomp_fd, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
ਤਿਆਰੀ_console_socket (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
                      ਬਣਤਰ container_entrypoint_s *container_ਦਲੀਲਾਂ, ਪੂਰਨ *console_socket_fd,
                      libcrun_error_t *err)
{
  ਜੇ (context->console_socket && def->process && def->process->terminal)
    {
      *console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
      ਜੇ (UNLIKELY (*console_socket_fd < 0))
        ਵਾਪਸ crun_error_wrap (err, "open console socket");
      container_ਦਲੀਲਾਂ->console_socket_fd = *console_socket_fd;
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
ਤਿਆਰੀ_cgroup_manager (libcrun_context_t *context, libcrun_container_t *container,
                      ਬਣਤਰ libcrun_cgroup_ਦਲੀਲਾਂ *cg, ਪੂਰਨ *cgroup_dirfd,
                      ਬਣਤਰ libcrun_dirfd_s *cgroup_dirfd_s, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ਪੂਰਨ cgroup_manager;
  uid_t ਜੜ੍ਹ_uid = -1;
  gid_t ਜੜ੍ਹ_gid = -1;
  ਪੂਰਨ ret;

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  ਜੇ (context->systemd_cgroup)
    {
      libcrun_debug ("Using systemd cgroup manager");
      cgroup_manager = CGROUP_MANAGER_SYSTEMD;
    }
  ਨਹੀਂਤਾਂ ਜੇ (context->force_no_cgroup)
    {
      libcrun_debug ("Disabling cgroup manager");
      cgroup_manager = CGROUP_MANAGER_DISABLED;
    }
  ਨਹੀਂਤਾਂ
    libcrun_debug ("Using cgroupfs cgroup manager");

  /* If we are root (either on the host or in a namespace), then chown the cgroup to root
     in the container user namespace.  */
  get_ਜੜ੍ਹ_ਵਿੱਚ_the_userns (def, container->host_uid, container->host_gid, &ਜੜ੍ਹ_uid, &ਜੜ੍ਹ_gid);
  libcrun_debug ("Using container host UID `%d` and GID `%d`", container->host_uid, container->host_gid);

  memset (cg, 0, ਆਕਾਰ (*cg));

  cg->cgroup_ਰਸਤਾ = def->linux ? def->linux->cgroups_ਰਸਤਾ : "";
  cg->manager = cgroup_manager;
  cg->id = context->id;
  cg->resources = def->linux ? def->linux->resources : ਨਿਰਾ;
  cg->annotations = container->annotations;
  cg->ਜੜ੍ਹ_uid = ਜੜ੍ਹ_uid;
  cg->ਜੜ੍ਹ_gid = ਜੜ੍ਹ_gid;
  cg->state_ਜੜ੍ਹ = context->state_ਜੜ੍ਹ;
  cg->container = container;

  ret = libcrun_cgroup_preenter (cg, cgroup_dirfd, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  cgroup_dirfd_s->dirfd = cgroup_dirfd;
  cgroup_dirfd_s->joined = false;

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
ਸੈੱਟ_scheduler (pid_t pid, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ret = libcrun_ਸੈੱਟ_scheduler (pid, def->process, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_reset_cpu_affinity_ਨਕਾਬ (pid, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_ਸੈੱਟ_io_ਤਰਜੀਹ (pid, def->process, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
seccomp_generation (ਪੂਰਨ seccomp_fd, ਅਚਲ ਅੱਖਰ *seccomp_bpf_ਅੰਕੜੇ,
                    ਬਣਤਰ libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ਜੇ (seccomp_fd >= 0)
    {
      ਜੇ (seccomp_bpf_ਅੰਕੜੇ != ਨਿਰਾ)
        {
          ret = libcrun_copy_seccomp (seccomp_gen_ctx, seccomp_bpf_ਅੰਕੜੇ, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }
      ਨਹੀਂਤਾਂ
        {
          ret = libcrun_generate_seccomp (seccomp_gen_ctx, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
terminal_ਤਿਆਰੀ (runtime_spec_schema_config_schema *def, libcrun_context_t *context,
                ਪੂਰਨ socket_pair_0, ਪੂਰਨ *terminal_fd, ਖਾਲੀ **orig_terminal, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  ਪੂਰਨ detach = context->detach;

  ਜੇ (def->process && def->process->terminal && ! detach && context->console_socket == ਨਿਰਾ)
    {
      libcrun_debug ("Receiving console socket fd");
      *terminal_fd = receive_fd_from_socket (socket_pair_0, err);
      ਜੇ (UNLIKELY (*terminal_fd < 0))
        ਵਾਪਸ -1;

      ret = libcrun_clear_onlcr (*terminal_fd, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = libcrun_ਸੈੱਟ_raw (0, orig_terminal, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
libcrun_container_ਲੜੀਵਾਰ_ਅੰਦਰੂਨੀ (libcrun_container_t *container, libcrun_context_t *context,
                                ਪੂਰਨ *container_ready_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ਪੂਰਨ ret;
  pid_t pid;
  ਪੂਰਨ detach = context->detach;
  cleanup_cgroup_ਸਥਿਤੀ ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ terminal_fd = -1;
  cleanup_terminal ਖਾਲੀ *orig_terminal = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ sync_socket = -1;
  cleanup_close ਪੂਰਨ notify_socket = -1;
  cleanup_close ਪੂਰਨ socket_pair_0 = -1;
  cleanup_close ਪੂਰਨ socket_pair_1 = -1;
  cleanup_close ਪੂਰਨ seccomp_fd = -1;
  cleanup_close ਪੂਰਨ console_socket_fd = -1;
  cleanup_close ਪੂਰਨ hooks_ਬਾਹਰਲਾ_fd = -1;
  cleanup_close ਪੂਰਨ hooks_err_fd = -1;
  cleanup_close ਪੂਰਨ own_seccomp_receiver_fd = -1;
  cleanup_close ਪੂਰਨ seccomp_notify_fd = -1;
  ਅਚਲ ਅੱਖਰ *seccomp_notify_plugins = ਨਿਰਾ;
  cleanup_cgroup_preenter ਬਣਤਰ libcrun_cgroup_ਦਲੀਲਾਂ cg = {};
  cleanup_free ਅੱਖਰ *bundle_allocated = ਨਿਰਾ;
  ਬਣਤਰ container_entrypoint_s container_ਦਲੀਲਾਂ = {
    .container = container,
    .context = context,
    .bundle = get_bundle_ਰਸਤਾ (context->bundle, &bundle_allocated),
    .terminal_socketpair = { -1, -1 },
    .console_socket_fd = -1,
    .hooks_ਬਾਹਰਲਾ_fd = -1,
    .hooks_err_fd = -1,
    .seccomp_receiver_fd = -1,
    .custom_handler = ਨਿਰਾ,
  };
  cleanup_close ਪੂਰਨ cgroup_dirfd = -1;
  ਬਣਤਰ libcrun_dirfd_s cgroup_dirfd_s;
  ਬਣਤਰ libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  ਅਚਲ ਅੱਖਰ *seccomp_bpf_ਅੰਕੜੇ = ਲੱਭੋ_annotation (container, "run.oci.seccomp_bpf_data");
  ਪੂਰਨ cgroup_ਬਹੁਲਕ;

  ਜੇ (! context->force_no_cgroup)
    {
      cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
      ਜੇ (UNLIKELY (cgroup_ਬਹੁਲਕ < 0))
        ਵਾਪਸ cgroup_ਬਹੁਲਕ;

      ਜੇ (cgroup_ਬਹੁਲਕ != CGROUP_ਬਹੁਲਕ_UNIFIED)
        libcrun_warning ("cgroup v1 is deprecated and will be removed in a future release.  Use cgroup v2");
    }

  ret = ਤਿਆਰੀ_container_hooks_output (container, def, &container_ਦਲੀਲਾਂ, &hooks_ਬਾਹਰਲਾ_fd, &hooks_err_fd, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  container->context = context;

  ਜੇ (! detach || context->notify_socket)
    {
      libcrun_debug ("Setting child subreaper");
      ret = prctl (PR_ਸੈੱਟ_ਬੱਚਾ_SUBREAPER, 1, 0, 0, 0);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "prctl set child subreaper");
    }

  ret = ਤਿਆਰੀ_container_keyring (container, context, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = ਤਿਆਰੀ_terminal_socket_pair (container, context, &container_ਦਲੀਲਾਂ, &socket_pair_0, &socket_pair_1, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = ਖੰਡ_ਸੰਕੇਤ (err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  umask (0);

  ret = libcrun_ਸੈੱਟ_mempolicy (def, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = ਤਿਆਰੀ_seccomp (container, seccomp_bpf_ਅੰਕੜੇ, &seccomp_gen_ctx, &seccomp_fd, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;
  container_ਦਲੀਲਾਂ.seccomp_fd = seccomp_fd;

  ਜੇ (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &container_ਦਲੀਲਾਂ.seccomp_receiver_fd, &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ret = ਤਿਆਰੀ_console_socket (context, def, &container_ਦਲੀਲਾਂ, &console_socket_fd, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = ਤਿਆਰੀ_cgroup_manager (context, container, &cg, &cgroup_dirfd, &cgroup_dirfd_s, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_configure_handler (container_ਦਲੀਲਾਂ.context->handler_manager,
                                   container_ਦਲੀਲਾਂ.context,
                                   container,
                                   &(container_ਦਲੀਲਾਂ.custom_handler),
                                   err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (container_ਦਲੀਲਾਂ.custom_handler && container_ਦਲੀਲਾਂ.custom_handler->vtable->modify_oci_configuration)
    {
      libcrun_debug ("Using custom handler to modify OCI configuration");
      ret = container_ਦਲੀਲਾਂ.custom_handler->vtable->modify_oci_configuration (container_ਦਲੀਲਾਂ.custom_handler->cookie,
                                                                             container_ਦਲੀਲਾਂ.context,
                                                                             container->container_def,
                                                                             err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  pid = libcrun_ਲੜੀਵਾਰ_linux_container (container, container_init, &container_ਦਲੀਲਾਂ, &sync_socket, &cgroup_dirfd_s, err);
  ਜੇ (UNLIKELY (pid < 0))
    ਵਾਪਸ pid;

  cg.pid = pid;
  cg.joined = cgroup_dirfd_s.joined;
  libcrun_debug ("Running container on PID: `%d`", pid);

  ਜੇ (context->fifo_exec_wait_fd < 0 && context->notify_socket)
    {
      libcrun_debug ("Using notify socket: `%s`", context->notify_socket);
      /* Do not open the notify socket here on "create".  "start" will take care of it.  */
      ret = get_notify_fd (context, container, &notify_socket, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਜਾਓ fail;
    }

  ਜੇ (container_ਦਲੀਲਾਂ.terminal_socketpair[1] >= 0)
    close_and_reset (&socket_pair_1);

  ret = libcrun_cgroup_enter (&cg, &cgroup_ਸਥਿਤੀ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ fail;

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ fail;

  ret = libcrun_move_ਜਾਲ_devices (container, pid, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ fail;

  /* sync send own pid.  */
  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (sync_socket, &pid, ਆਕਾਰ (pid)));
  ਜੇ (UNLIKELY (ret != ਆਕਾਰ (pid)))
    {
      ਜੇ (ret >= 0)
        errno = 0;
      crun_make_error (err, errno, "write to sync socket");
      ਜਾਓ fail;
    }

  /* sync 1.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ fail;

  /* sync 2.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ fail;

  ret = libcrun_cgroup_enter_finalize (&cg, cgroup_ਸਥਿਤੀ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ fail;

  /* If the container process is created later, the scheduler is set on it then.  */
  ਜੇ (! libcrun_container_delayed_pidns_fork (container))
    {
      ret = ਸੈੱਟ_scheduler (pid, def, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਜਾਓ fail;
    }

  /* The container is waiting that we write back.  In this phase we can launch the
     prestart hooks.  */
  ਜੇ (def->hooks && def->hooks->prestart_len)
    {
      libcrun_debug ("Running `prestart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "prestart", (hook **) def->hooks->prestart,
                      def->hooks->prestart_len, hooks_ਬਾਹਰਲਾ_fd, hooks_err_fd, false, err);
      ਜੇ (UNLIKELY (ret != 0))
        ਜਾਓ fail;
    }
  ਜੇ (def->hooks && def->hooks->create_runtime_len)
    {
      libcrun_debug ("Running `create` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "createRuntime", (hook **) def->hooks->create_runtime,
                      def->hooks->create_runtime_len, hooks_ਬਾਹਰਲਾ_fd, hooks_err_fd, false, err);
      ਜੇ (UNLIKELY (ret != 0))
        ਜਾਓ fail;
    }

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_ਅੰਕੜੇ, &seccomp_gen_ctx, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ fail;
  close_and_reset (&seccomp_fd);

  /* sync 3.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ fail;

  /* The process joining an existing PID namespace is created once the container root is in place.  */
  ਜੇ (libcrun_container_delayed_pidns_fork (container))
    {
      pid_t new_pid = 0;

      ret = sync_socket_wait_sync (context, sync_socket, false, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਜਾਓ fail;

      ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (sync_socket, &new_pid, ਆਕਾਰ (new_pid)));
      ਜੇ (UNLIKELY (ret != ਆਕਾਰ (new_pid)))
        {
          ਜੇ (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "read pid from sync socket");
          ਜਾਓ fail;
        }

      /* Cleanup the first process.  */
      waitpid_ignore_stopped (pid, ਨਿਰਾ, 0);

      pid = new_pid;

      ret = ਸੈੱਟ_scheduler (pid, def, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਜਾਓ fail;

      ret = libcrun_handle_pidfd_receiver (pid, container, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਜਾਓ fail;

      /* sync send own pid.  */
      ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (sync_socket, &pid, ਆਕਾਰ (pid)));
      ਜੇ (UNLIKELY (ret != ਆਕਾਰ (pid)))
        {
          ਜੇ (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "write to sync socket");
          ਜਾਓ fail;
        }
    }

  ret = terminal_ਤਿਆਰੀ (def, context, socket_pair_0, &terminal_fd, &orig_terminal, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ fail;

  close_and_reset (&socket_pair_0);

  /* sync 4.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ fail;

  ret = close_and_reset (&sync_socket);
  ਜੇ (UNLIKELY (ret < 0))
    {
      crun_make_error (err, errno, "close sync_socket failed");
      ਜਾਓ fail;
    }

  libcrun_debug ("Writing container status");
  ret = write_container_ਸਥਿਤੀ (container, context, pid, cgroup_ਸਥਿਤੀ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ fail;

  /* Run poststart hooks here only if the container is created using "run".  For create+start, the
     hooks will be executed as part of the start command.  */
  ਜੇ (context->fifo_exec_wait_fd < 0 && def->hooks && def->hooks->poststart_len)
    {
      libcrun_debug ("Running `poststart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_ਬਾਹਰਲਾ_fd, hooks_err_fd, false, err);
      ਜੇ (UNLIKELY (ret != 0))
        ਜਾਓ fail;
    }

  /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
  ਜੇ (own_seccomp_receiver_fd >= 0)
    {
      libcrun_debug ("Receiving seccomp fd");
      seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
      ਜੇ (UNLIKELY (seccomp_notify_fd < 0))
        ਜਾਓ fail;

      ret = close_and_reset (&own_seccomp_receiver_fd);
      ਜੇ (UNLIKELY (ret < 0))
        {
          crun_make_error (err, errno, "close seccomp receiver fd failed");
          ਜਾਓ fail;
        }
    }

  {
    ਬਣਤਰ wait_for_process_ਦਲੀਲਾਂ ਦਲੀਲਾਂ = {
      .pid = pid,
      .context = context,
      .terminal_fd = terminal_fd,
      .notify_socket = notify_socket,
      .container_ready_fd = container_ready_fd,
      .seccomp_notify_fd = seccomp_notify_fd,
      .seccomp_notify_plugins = seccomp_notify_plugins,
    };
    ret = wait_for_process (&ਦਲੀਲਾਂ, err);
  }
  ਜੇ (! context->detach)
    {
      libcrun_error_t tmp_err = ਨਿਰਾ;
      cleanup_watch (context, def, cgroup_ਸਥਿਤੀ, 0, sync_socket, terminal_fd, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ਵਾਪਸ ret;

fail:
  ret = cleanup_watch (context, def, cgroup_ਸਥਿਤੀ, pid, sync_socket, terminal_fd, err);
  ਜੇ (cgroup_ਸਥਿਤੀ)
    {
      libcrun_error_t tmp_err = ਨਿਰਾ;
      libcrun_cgroup_destroy (cgroup_ਸਥਿਤੀ, &tmp_err);
      crun_error_release (&tmp_err);
    }
  ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ
check_config_file (runtime_spec_schema_config_schema *def, libcrun_context_t *context, libcrun_error_t *err)
{
  ਜੇ (UNLIKELY (def->linux == ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, 0, "invalid config file, no `linux` block specified");

  ਜੇ (context->handler == ਨਿਰਾ)
    {
      ਜੇ (UNLIKELY (def->ਜੜ੍ਹ == ਨਿਰਾ))
        ਵਾਪਸ crun_make_error (err, 0, "invalid config file, no `root` block specified");
      ਜੇ (UNLIKELY (def->mounts == ਨਿਰਾ))
        ਵਾਪਸ crun_make_error (err, 0, "invalid config file, no `mounts` block specified");
    }
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
libcrun_copy_config_file (ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, libcrun_container_t *container, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  cleanup_free ਅੱਖਰ *dest_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *dir = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *ਬਫਰ = ਨਿਰਾ;
  size_t len;

  ret = libcrun_get_state_directory (&dir, state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = append_paths (&dest_ਰਸਤਾ, err, dir, "config.json", ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (container->config_file == ਨਿਰਾ && container->config_file_content == ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, 0, "config file not specified");

  ਜੇ (container->config_file == ਨਿਰਾ)
    {
      libcrun_debug ("Writing config file to: `%s`", dest_ਰਸਤਾ);
      ret = write_file (dest_ਰਸਤਾ, container->config_file_content, ਲੜੀ_ਲੰਬਾਈ (container->config_file_content), err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਨਹੀਂਤਾਂ
    {
      libcrun_debug ("Reading config file: `%s`", container->config_file);
      ret = read_all_file (container->config_file, &ਬਫਰ, &len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      libcrun_debug ("Writing config file to: `%s`", dest_ਰਸਤਾ);
      ret = write_file (dest_ਰਸਤਾ, ਬਫਰ, len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ
force_delete_container_ਸਥਿਤੀ (libcrun_context_t *context, libcrun_container_t *container,
                               runtime_spec_schema_config_schema *def)
{
  libcrun_error_t tmp_err = ਨਿਰਾ;
  container_delete_ਅੰਦਰੂਨੀ (context, container, def, context->id, true, false, &tmp_err);
  libcrun_error_report_and_release (&tmp_err);
}

/* Serialize ERR to FD as an int errno followed by a NUL-terminated message.
   Used by forked children to report a failure to the parent process.  */
ਸਥਿਰ ਖਾਲੀ
write_error_ਮੰਜ਼ਿਲ_pipe (ਪੂਰਨ fd, libcrun_error_t *err)
{
  ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ = (*err)->ਸੁਨੇਹਾ;
  ਪੂਰਨ errcode = crun_error_get_errno (err);

  ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (fd, &errcode, ਆਕਾਰ (errcode)));
  ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (fd, ਸੁਨੇਹਾ, ਲੜੀ_ਲੰਬਾਈ (ਸੁਨੇਹਾ) + 1));
}

ਪੂਰਨ
libcrun_container_ਲੜੀਵਾਰ (libcrun_context_t *context, libcrun_container_t *container, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ options,
                       libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ਪੂਰਨ ret;
  ਪੂਰਨ detach = context->detach;
  ਪੂਰਨ container_ret_ਸਥਿਤੀ[2];
  cleanup_close ਪੂਰਨ pipefd0 = -1;
  cleanup_close ਪੂਰਨ pipefd1 = -1;
  libcrun_error_t tmp_err = ਨਿਰਾ;

  container->context = context;

  ret = validate_options (options, LIBCRUN_ਲੜੀਵਾਰ_OPTIONS_PREFORK | LIBCRUN_ਲੜੀਵਾਰ_OPTIONS_KEEP, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = check_config_file (def, context, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (def->process && def->process->terminal && detach && context->console_socket == ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, 0, "use --console-socket with --detach when a terminal is used");

  ret = libcrun_ਸਥਿਤੀ_check_directories (context->state_ਜੜ੍ਹ, context->id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (! detach && (options & LIBCRUN_ਲੜੀਵਾਰ_OPTIONS_PREFORK) == 0)
    {
      ret = libcrun_copy_config_file (context->id, context->state_ਜੜ੍ਹ, container, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = libcrun_container_ਲੜੀਵਾਰ_ਅੰਦਰੂਨੀ (container, context, ਨਿਰਾ, err);
      ਜੇ (! (options & LIBCRUN_ਲੜੀਵਾਰ_OPTIONS_KEEP))
        force_delete_container_ਸਥਿਤੀ (context, container, def);
      ਵਾਪਸ ret;
    }

  ret = pipe2 (container_ret_ਸਥਿਤੀ, O_CLOEXEC);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_ਸਥਿਤੀ[0];
  pipefd1 = container_ret_ਸਥਿਤੀ[1];

  ret = fork ();
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "fork");
  ਜੇ (ret)
    {
      ਪੂਰਨ ਸਥਿਤੀ;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, ਨਿਰਾ, 0);

      ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (pipefd0, &ਸਥਿਤੀ, ਆਕਾਰ (ਸਥਿਤੀ)));
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, errno, "read from sync pipe");

      ਜੇ (ਸਥਿਤੀ < 0)
        {
          ਪੂਰਨ errno_;
          ਅੱਖਰ buf[512];
          ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (pipefd0, &errno_, ਆਕਾਰ (errno_)));
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "read from sync pipe");

          ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (pipefd0, buf, ਆਕਾਰ (buf) - 1));
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "read from sync pipe");
          buf[ret] = '\0';

          ਵਾਪਸ crun_make_error (err, errno_, "%s", buf);
        }

      ਵਾਪਸ ਸਥਿਤੀ;
    }

  close_and_reset (&pipefd0);

  /* forked process.  */
  ret = detach_process ();
  ਜੇ (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_ਜੜ੍ਹ, container, &tmp_err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ fail;

  ret = libcrun_container_ਲੜੀਵਾਰ_ਅੰਦਰੂਨੀ (container, context, ਨਿਰਾ, &tmp_err);
  ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (pipefd1, &ret, ਆਕਾਰ (ret)));
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ fail;

  ਬਾਹਰ_ਨਿਕਲੋ (EXIT_SUCCESS);
fail:

  ਜੇ (! (options & LIBCRUN_ਲੜੀਵਾਰ_OPTIONS_KEEP))
    force_delete_container_ਸਥਿਤੀ (context, container, def);
  ਜੇ (tmp_err)
    {
      write_error_ਮੰਜ਼ਿਲ_pipe (pipefd1, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ਬਾਹਰ_ਨਿਕਲੋ (EXIT_FAILURE);
}

ਪੂਰਨ
libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ options,
                          libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ਪੂਰਨ ret;
  ਪੂਰਨ container_ready_pipe[2];
  cleanup_close ਪੂਰਨ pipefd0 = -1;
  cleanup_close ਪੂਰਨ pipefd1 = -1;
  cleanup_close ਪੂਰਨ exec_fifo_fd = -1;
  context->detach = 1;

  libcrun_debug ("Creating container: `%s`", context->id);
  container->context = context;

  ret = validate_options (options, LIBCRUN_CREATE_OPTIONS_PREFORK, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = check_config_file (def, context, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (def->process && def->process->terminal && context->console_socket == ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, 0, "use --console-socket with create when a terminal is used");

  ret = libcrun_ਸਥਿਤੀ_check_directories (context->state_ਜੜ੍ਹ, context->id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  exec_fifo_fd = libcrun_ਸਥਿਤੀ_create_exec_fifo (context->state_ਜੜ੍ਹ, context->id, err);
  ਜੇ (UNLIKELY (exec_fifo_fd < 0))
    ਵਾਪਸ exec_fifo_fd;

  context->fifo_exec_wait_fd = exec_fifo_fd;
  exec_fifo_fd = -1;

  ਜੇ ((options & LIBCRUN_ਲੜੀਵਾਰ_OPTIONS_PREFORK) == 0)
    {
      libcrun_debug ("Running with prefork enabled");
      ret = libcrun_copy_config_file (context->id, context->state_ਜੜ੍ਹ, container, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
      ret = libcrun_container_ਲੜੀਵਾਰ_ਅੰਦਰੂਨੀ (container, context, ਨਿਰਾ, err);
      ਜੇ (UNLIKELY (ret < 0))
        force_delete_container_ਸਥਿਤੀ (context, container, def);
      ਵਾਪਸ ret;
    }

  ret = pipe2 (container_ready_pipe, O_CLOEXEC);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "pipe");
  pipefd0 = container_ready_pipe[0];
  pipefd1 = container_ready_pipe[1];

  ret = fork ();
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "fork");
  ਜੇ (ret)
    {
      ਪੂਰਨ exit_ਕੋਡ;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, ਨਿਰਾ, 0);

      ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (pipefd0, &exit_ਕੋਡ, ਆਕਾਰ (exit_ਕੋਡ)));
      ਜੇ (UNLIKELY (ret <= 0))
        ਵਾਪਸ crun_make_error (err, ret < 0 ? errno : 0, "waiting for container to be ready");

      ਜੇ (ret != ਆਕਾਰ (exit_ਕੋਡ) || exit_ਕੋਡ < 0)
        ਵਾਪਸ crun_make_error (err, 0, "internal error: read invalid exit code");

      ਜੇ (exit_ਕੋਡ == 0)
        ਵਾਪਸ 0;

      libcrun_debug ("Exit code is `%d`, deleting container", exit_ਕੋਡ);
      libcrun_error_t tmp_err = ਨਿਰਾ;
      container_delete_ਅੰਦਰੂਨੀ (context, ਨਿਰਾ, def, context->id, true, true, &tmp_err);
      libcrun_error_report_and_release (&tmp_err);
      ਵਾਪਸ crun_make_error (err, 0, "error creating container");
    }

  /* forked process.  */
  ret = detach_process ();
  ਜੇ (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_ਜੜ੍ਹ, container, err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਪੂਰਨ errcode = crun_error_get_errno (err);
      crun_error_release (err);
      libcrun_fail_with_error (errcode, "copy config file");
    }

  ret = libcrun_container_ਲੜੀਵਾਰ_ਅੰਦਰੂਨੀ (container, context, &pipefd1, err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      force_delete_container_ਸਥਿਤੀ (context, container, def);
      libcrun_error ((*err)->ਸਥਿਤੀ, "%s", (*err)->ਸੁਨੇਹਾ);
      crun_error_release (err);
      crun_ਸੈੱਟ_output_handler (log_write_ਮੰਜ਼ਿਲ_stderr, ਨਿਰਾ);
    }

  ਜੇ (pipefd1 >= 0)
    ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (pipefd1, &ret, ਆਕਾਰ (ret)));
  ਬਾਹਰ_ਨਿਕਲੋ (ret ? EXIT_FAILURE : 0);
}

ਪੂਰਨ
libcrun_container_ਸ਼ੁਰੂ (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ = context->state_ਜੜ੍ਹ;
  runtime_spec_schema_config_schema *def;
  cleanup_container_ਸਥਿਤੀ libcrun_container_ਸਥਿਤੀ_t ਸਥਿਤੀ = {};
  cleanup_close ਪੂਰਨ fd = -1;
  ਪੂਰਨ ret;

  ret = libcrun_read_container_ਸਥਿਤੀ (&ਸਥਿਤੀ, state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_is_container_running (&ਸਥਿਤੀ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (! ret)
    ਵਾਪਸ crun_make_error (err, 0, "container `%s` is not running", id);

  ret = read_container_config_from_state (&container, state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (context->notify_socket)
    {
      ret = get_notify_fd (context, container, &fd, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ret = libcrun_ਸਥਿਤੀ_write_exec_fifo (context->state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  def = container->container_def;

  ਜੇ (context->notify_socket)
    {
      ਜੇ (fd >= 0)
        {
          fd_set read_ਸੈੱਟ;

          ਜਦੋਂ (1)
            {
              ਬਣਤਰ timeval timeout = {
                .tv_sec = 0,
                .tv_usec = 10000,
              };
              FD_ZERO (&read_ਸੈੱਟ);
              FD_SET (fd, &read_ਸੈੱਟ);

              ret = select (fd + 1, &read_ਸੈੱਟ, ਨਿਰਾ, ਨਿਰਾ, &timeout);
              ਜੇ (UNLIKELY (ret < 0))
                ਵਾਪਸ crun_make_error (err, errno, "select failed");
              ਜੇ (ret)
                {
                  ret = handle_notify_socket (fd, err);
                  ਜੇ (UNLIKELY (ret < 0))
                    ਵਾਪਸ ret;
                  ਜੇ (ret)
                    ਰੋਕੋ;
                }
              ਨਹੀਂਤਾਂ
                {
                  ret = libcrun_is_container_running (&ਸਥਿਤੀ, err);
                  ਜੇ (UNLIKELY (ret < 0))
                    ਵਾਪਸ ret;
                  ਜੇ (! ret)
                    ਵਾਪਸ 0;
                }
            }
        }
    }

  /* The container is considered running only after we got the notification from the
     notify_socket, if any.  */
  ਜੇ (def->hooks && def->hooks->poststart_len)
    {
      cleanup_close ਪੂਰਨ hooks_ਬਾਹਰਲਾ_fd = -1;
      cleanup_close ਪੂਰਨ hooks_err_fd = -1;

      ret = open_hooks_output (container, &hooks_ਬਾਹਰਲਾ_fd, &hooks_err_fd, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = do_hooks (def, ਸਥਿਤੀ.pid, context->id, false, ਸਥਿਤੀ.bundle, ਸਥਿਤੀ.bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_ਬਾਹਰਲਾ_fd, hooks_err_fd, false, err);
      ਜੇ (UNLIKELY (ret != 0))
        {
          libcrun_error_t tmp_err = ਨਿਰਾ;
          container_delete_ਅੰਦਰੂਨੀ (context, container, def, id, true, true, &tmp_err);
          libcrun_error_report_and_release (&tmp_err);
          ਵਾਪਸ ret;
        }
    }

  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_get_container_state_string (ਅਚਲ ਅੱਖਰ *id, libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ,
                                    ਅਚਲ ਅੱਖਰ **container_ਸਥਿਤੀ, ਪੂਰਨ *running, libcrun_error_t *err)
{
  ਪੂਰਨ ret, has_fifo = 0;
  bool paused = false;

  ret = libcrun_is_container_running (ਸਥਿਤੀ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;
  *running = ret;

  ਜੇ (*running)
    {
      ret = libcrun_ਸਥਿਤੀ_has_read_exec_fifo (state_ਜੜ੍ਹ, id, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
      has_fifo = ret;
    }

  ਜੇ (*running && ! has_fifo)
    {
      cleanup_cgroup_ਸਥਿਤੀ ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ = ਨਿਰਾ;

      cgroup_ਸਥਿਤੀ = libcrun_cgroup_make_ਸਥਿਤੀ (ਸਥਿਤੀ);

      ret = libcrun_cgroup_is_container_paused (cgroup_ਸਥਿਤੀ, &paused, err);
      ਜੇ (UNLIKELY (ret < 0))
        {
          /*
            The cgroup might have been cleaned up by the time we try to read it, ignore both
            ENOENT and ENODEV:
            - ENOENT: if the open(CGROUP_PATH) fails because the cgroup was deleted.
            - ENODEV: if the cgroup is deleted between the open and reading the freeze status.
          */

          errno = crun_error_get_errno (err);
          ਜੇ (errno == ENOENT || errno == ENODEV)
            {
              crun_error_release (err);
              *container_ਸਥਿਤੀ = "stopped";
              ਵਾਪਸ 0;
            }

          ਵਾਪਸ ret;
        }
    }

  ਜੇ (! *running)
    *container_ਸਥਿਤੀ = "stopped";
  ਨਹੀਂਤਾਂ ਜੇ (has_fifo)
    *container_ਸਥਿਤੀ = "created";
  ਨਹੀਂਤਾਂ ਜੇ (paused)
    *container_ਸਥਿਤੀ = "paused";
  ਨਹੀਂਤਾਂ
    *container_ਸਥਿਤੀ = "running";

  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_container_state_json (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, ਅੱਖਰ **ਬਾਹਰਲਾ, libcrun_error_t *err)
{
  ਅੱਖਰ *ਬਫਰ = ਨਿਰਾ;
  size_t len = 0;
  FILE *f;
  ਪੂਰਨ ret;

  f = open_memstream (&ਬਫਰ, &len);
  ਜੇ (f == ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, errno, "open_memstream");

  ret = libcrun_container_state (context, id, f, err);
  fclose (f);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਮੁਕਤ (ਬਫਰ);
      ਵਾਪਸ ret;
    }

  *ਬਾਹਰਲਾ = ਬਫਰ;
  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_container_state (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, FILE *ਬਾਹਰਲਾ, libcrun_error_t *err)
{
  ਅਚਲ ਅੱਖਰ *ਅਚਲ OCI_CONFIG_ਸੰਸਕਰਣ = "1.0.0";
  libcrun_container_ਸਥਿਤੀ_t ਸਥਿਤੀ = {};
  ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ = context->state_ਜੜ੍ਹ;
  ਅਚਲ ਅੱਖਰ *container_ਸਥਿਤੀ = ਨਿਰਾ;
  json_gen_ctx *gen = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *buf;
  ਪੂਰਨ ret = 0;
  ਪੂਰਨ r = json_gen_ਸਥਿਤੀ_ok;
  ਪੂਰਨ running;
  size_t len;

  ret = libcrun_read_container_ਸਥਿਤੀ (&ਸਥਿਤੀ, state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_get_container_state_string (id, &ਸਥਿਤੀ, state_ਜੜ੍ਹ, &container_ਸਥਿਤੀ, &running, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;

  ret = 0;
  ਜੇ (! json_gen_init (&gen, ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));
  GEN_ਕੁੰਜੀ (gen, "ociVersion");
  GEN_STR (gen, OCI_CONFIG_ਸੰਸਕਰਣ);

  GEN_ਕੁੰਜੀ (gen, "id");
  GEN_STR (gen, id);

  GEN_ਕੁੰਜੀ (gen, "pid");
  GEN_OR_FAIL (map_int (gen, running ? ਸਥਿਤੀ.pid : 0));

  GEN_ਕੁੰਜੀ (gen, "status");
  GEN_STR (gen, container_ਸਥਿਤੀ);

  GEN_ਕੁੰਜੀ (gen, "bundle");
  GEN_STR (gen, ਸਥਿਤੀ.bundle);

  GEN_ਕੁੰਜੀ (gen, "rootfs");
  GEN_STR (gen, ਸਥਿਤੀ.rootfs);

  GEN_ਕੁੰਜੀ (gen, "created");
  GEN_STR (gen, ਸਥਿਤੀ.created);

  ਜੇ (ਸਥਿਤੀ.scope)
    {
      GEN_ਕੁੰਜੀ (gen, "systemd-scope");
      GEN_STR (gen, ਸਥਿਤੀ.scope);
    }

  ਜੇ (ਸਥਿਤੀ.owner)
    {
      GEN_ਕੁੰਜੀ (gen, "owner");
      GEN_STR (gen, ਸਥਿਤੀ.owner);
    }

  {
    cleanup_container libcrun_container_t *container = ਨਿਰਾ;

    ret = read_container_config_from_state (&container, state_ਜੜ੍ਹ, id, err);
    ਜੇ (UNLIKELY (ret < 0))
      ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;

    ਜੇ (container->container_def->annotations && container->container_def->annotations->len)
      GEN_OR_FAIL (gen_annotations (gen, container->container_def->annotations));
  }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  fprintf (ਬਾਹਰਲਾ, "%s\n", buf);

ਬਾਹਰ_ਨਿਕਲੋ:
  ਜੇ (gen)
    json_gen_free (gen);
  libcrun_free_container_ਸਥਿਤੀ (&ਸਥਿਤੀ);
  ਵਾਪਸ ret;

gen_error:
  ret = json_gen_error_ਮੰਜ਼ਿਲ_crun_error (r, err);
  ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
}

ਪੂਰਨ
libcrun_container_exec (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, runtime_spec_schema_config_schema_process *process,
                        libcrun_error_t *err)
{
  ਬਣਤਰ libcrun_container_exec_options_s opts;
  memset (&opts, 0, ਆਕਾਰ (opts));

  opts.struct_ਮਾਪ = ਆਕਾਰ (opts);
  opts.process = process;

  ਵਾਪਸ libcrun_container_exec_with_options (context, id, &opts, err);
}

ਪੂਰਨ
libcrun_container_exec_process_file (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err)
{
  ਬਣਤਰ libcrun_container_exec_options_s opts;
  memset (&opts, 0, ਆਕਾਰ (opts));

  opts.struct_ਮਾਪ = ਆਕਾਰ (opts);
  opts.ਰਸਤਾ = ਰਸਤਾ;

  ਵਾਪਸ libcrun_container_exec_with_options (context, id, &opts, err);
}

ਸਥਿਰ ਪੂਰਨ
exec_from_process_json (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *process_json, ਅਚਲ ਅੱਖਰ *cgroup,
                        bool ਮਿਲਾਓ_env, libcrun_error_t *err)
{
  ਬਣਤਰ parser_context parser_ctx = { .options = 0, .errfile = stderr };
  ਬਣਤਰ libcrun_container_exec_options_s opts;
  runtime_spec_schema_config_schema_process *process;
  parser_error parser_err = ਨਿਰਾ;
  json_object *doc = ਨਿਰਾ;
  ਪੂਰਨ ret;

  ret = parse_json_file (&doc, process_json, &parser_ctx, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  process = make_runtime_spec_schema_config_schema_process (doc, &parser_ctx, &parser_err);
  json_object_put (doc);
  ਜੇ (process == ਨਿਰਾ)
    {
      ret = crun_make_error (err, 0, "cannot parse process: `%s`", parser_err ? parser_err : "unknown error");
      ਮੁਕਤ (parser_err);
      ਵਾਪਸ ret;
    }

  memset (&opts, 0, ਆਕਾਰ (opts));
  opts.struct_ਮਾਪ = ਆਕਾਰ (opts);
  opts.process = process;
  opts.cgroup = cgroup;
  opts.ਮਿਲਾਓ_env = ਮਿਲਾਓ_env;

  ret = libcrun_container_exec_with_options (context, id, &opts, err);
  free_runtime_spec_schema_config_schema_process (process);
  ਵਾਪਸ ret;
}

ਪੂਰਨ
libcrun_container_exec_json (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *process_json, libcrun_error_t *err)
{
  ਵਾਪਸ exec_from_process_json (context, id, process_json, ਨਿਰਾ, false, err);
}

ਪੂਰਨ
libcrun_container_exec_with_options_json (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id,
                                          ਅਚਲ ਬਣਤਰ libcrun_exec_options_s *opts, libcrun_error_t *err)
{
  ਜੇ (opts == ਨਿਰਾ || opts->struct_ਮਾਪ == 0)
    ਵਾਪਸ crun_make_error (err, EINVAL, "invalid exec options");

  ਵਾਪਸ exec_from_process_json (context, id, opts->process_json, opts->cgroup, opts->ਮਿਲਾਓ_env, err);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_process_schema __attribute__ ((cleanup (cleanup_process_schemap)))

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cleanup_process_schemap (runtime_spec_schema_config_schema_process **p)
{
  runtime_spec_schema_config_schema_process *process = *p;
  ਜੇ (process)
    (ਖਾਲੀ) free_runtime_spec_schema_config_schema_process (process);
}

/* Build the seccomp receiver payload (when needed), apply the seccomp profile and
   close the seccomp file descriptors.  Used from exec_process_entrypoint both before
   and after the capabilities are set, depending on process->no_new_privileges.  */
ਸਥਿਰ ਪੂਰਨ
apply_seccomp_for_exec (libcrun_container_t *container, pid_t own_pid, ਪੂਰਨ seccomp_fd,
                        ਪੂਰਨ seccomp_receiver_fd, ਅੱਖਰ **seccomp_flags, size_t seccomp_flags_len,
                        libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *seccomp_fd_payload = ਨਿਰਾ;
  size_t seccomp_fd_payload_len = 0;
  ਪੂਰਨ ret;

  ਜੇ (seccomp_receiver_fd >= 0)
    {
      ret = get_seccomp_receiver_fd_payload (container, "running", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ret = libcrun_apply_seccomp (seccomp_fd, seccomp_receiver_fd, seccomp_fd_payload,
                               seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  close_and_reset (&seccomp_fd);
  close_and_reset (&seccomp_receiver_fd);
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
exec_process_entrypoint (libcrun_context_t *context,
                         libcrun_container_t *container,
                         runtime_spec_schema_config_schema_process *process,
                         ਪੂਰਨ *pipefd1,
                         ਪੂਰਨ seccomp_fd,
                         ਪੂਰਨ seccomp_receiver_fd,
                         ਬਣਤਰ custom_handler_instance_s *custom_handler,
                         bool ਮਿਲਾਓ_env,
                         libcrun_error_t *err)
{
  runtime_spec_schema_config_schema_process_capabilities *capabilities = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *exec_ਰਸਤਾ = ਨਿਰਾ;
  uid_t container_uid;
  gid_t container_gid;
  ਅਚਲ ਅੱਖਰ *cwd;
  bool chdir_ਪੂਰਾ = false;
  size_t seccomp_flags_len = 0;
  ਅੱਖਰ **seccomp_flags = ਨਿਰਾ;
  pid_t own_pid = 0;
  size_t i;
  ਪੂਰਨ ret;

  container_uid = process->user ? process->user->uid : 0;
  container_gid = process->user ? process->user->gid : 0;

  ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (*pipefd1, &own_pid, ਆਕਾਰ (own_pid)));

  cwd = process->cwd ? process->cwd : "/";
  ਜੇ (LIKELY (libcrun_ਸੁਰੱਖਿਅਤ_chdir (cwd, err) == 0))
    chdir_ਪੂਰਾ = true;
  ਨਹੀਂਤਾਂ
    crun_error_release (err);

  ret = unblock_ਸੰਕੇਤ (err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = clearenv ();
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "clearenv");

  ਜੇ ((ਮਿਲਾਓ_env || ! process->env_len) && container->container_def->process && container->container_def->process->env_len)
    {
      ਅੱਖਰ *e;

      ਲਈ (i = 0; i < container->container_def->process->env_len; i++)
        {
          e = container->container_def->process->env[i];
          ਜੇ (putenv (e) < 0)
            ਵਾਪਸ crun_make_error (err, errno, "putenv `%s`", e);
        }
    }
  ਜੇ (process->env_len)
    {
      ਲਈ (i = 0; i < process->env_len; i++)
        ਜੇ (putenv (process->env[i]) < 0)
          ਵਾਪਸ crun_make_error (err, errno, "putenv `%s`", process->env[i]);
    }

  ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_string (getenv ("HOME")))
    {
      ret = ਸੈੱਟ_home_env (container_uid);
      ਜੇ (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          ਜੇ (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            ਵਾਪਸ crun_make_error (err, errno, "setenv HOME");
        }
    }

  ret = libcrun_ਸੈੱਟ_selinux_label (container, process, false, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_ਸੈੱਟ_apparmor_profile (container, process, false, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (container->container_def->linux && container->container_def->linux->seccomp)
    {
      seccomp_flags = container->container_def->linux->seccomp->flags;
      seccomp_flags_len = container->container_def->linux->seccomp->flags_len;
    }

  ret = ਲੱਭੋ_executable (&exec_ਰਸਤਾ, process->ਦਲੀਲਾਂ[0], process->cwd, err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ਜੇ (crun_error_get_errno (err) == ENOENT)
        ਵਾਪਸ ret;

      /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
         once the process switched to the use that runs in the container.  This might be necessary
         when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
         is not honored.  */
      crun_error_release (err);
    }

  ਜੇ (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_ਸੈੱਟ_personality (container->container_def->linux->personality, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, false, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_reopen_dev_null (err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (! process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ret = libcrun_container_setgroups (container, process, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = maybe_chown_std_streams (container_uid, container_gid, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (process->capabilities)
    capabilities = process->capabilities;
  ਨਹੀਂਤਾਂ ਜੇ (container->container_def->process)
    capabilities = container->container_def->process->capabilities;

  ret = libcrun_ਸੈੱਟ_caps (capabilities, container_uid, container_gid, process->no_new_privileges,
                          custom_handler != ਨਿਰਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (UNLIKELY (exec_ਰਸਤਾ == ਨਿਰਾ))
    {
      ret = ਲੱਭੋ_executable (&exec_ਰਸਤਾ, process->ਦਲੀਲਾਂ[0], process->cwd, err);
      ਜੇ (UNLIKELY (ret < 0))
        {
          ਜੇ (custom_handler == ਨਿਰਾ || is_ਖਾਲੀ_ਥਾਵਾਂ_string (process->ਦਲੀਲਾਂ[0]))
            ਵਾਪਸ ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          exec_ਰਸਤਾ = xstrdup (process->ਦਲੀਲਾਂ[0]);
        }
    }

  ਜੇ (! chdir_ਪੂਰਾ)
    {
      ret = libcrun_ਸੁਰੱਖਿਅਤ_chdir (cwd, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (process->user)
    umask (process->user->umask_present ? process->user->umask : 0022);

  ret = 0;
  ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (*pipefd1, &ret, ਆਕਾਰ (ret)));
  ਆਰਜ਼ੀ_FAILURE_RETRY (close (*pipefd1));
  *pipefd1 = -1;

  ਜੇ (custom_handler)
    {
      ਜੇ (custom_handler->vtable->exec_func == ਨਿਰਾ)
        ਵਾਪਸ crun_make_error (err, 0, "the handler does not support exec");

      ਜੇ (context->argv)
        erase_initial_environment (context->argv, context->argc);

      ret = custom_handler->vtable->exec_func (custom_handler->cookie,
                                               container,
                                               exec_ਰਸਤਾ,
                                               process->ਦਲੀਲਾਂ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, -ret, "exec container process failed with handler as `%s`", custom_handler->vtable->name);

      ਸੁਰੱਖਿਅਤ_exit (EXIT_FAILURE);
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation, because the seccomp filter is already in place and it could
     stop some syscalls used by mark_or_close_fds_ge_than.
  */
  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, true, err);
  ਜੇ (UNLIKELY (ret < 0))
    crun_error_release (err);

  ਆਰਜ਼ੀ_FAILURE_RETRY (execv (exec_ਰਸਤਾ, process->ਦਲੀਲਾਂ));
  libcrun_fail_with_error (errno, "exec `%s`", exec_ਰਸਤਾ);
}

/* Close the file descriptors inherited from the caller, other than those
   to be passed to the container process.  */
ਪੂਰਨ
libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err)
{
  ਵਾਪਸ mark_or_close_fds_ge_than (ਨਿਰਾ, context->preserve_fds + 3, true, err);
}

ਪੂਰਨ
libcrun_ਲੱਦੋ_process_from_file (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, runtime_spec_schema_config_schema_process **process,
                                libcrun_error_t *err)
{
  ਬਣਤਰ parser_context ctx = { 0, stderr };
  cleanup_free ਅੱਖਰ *content = ਨਿਰਾ;
  parser_error parser_err = ਨਿਰਾ;
  json_object *doc = ਨਿਰਾ;
  size_t len;
  ਪੂਰਨ ret;

  ret = read_all_file (ਰਸਤਾ, &content, &len, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  *process = make_runtime_spec_schema_config_schema_process (doc, &ctx, &parser_err);
  ਜੇ (UNLIKELY (*process == ਨਿਰਾ))
    {
      ret = crun_make_error (err, 0, "cannot parse process file: `%s`", parser_err);
      ਮੁਕਤ (parser_err);
      ਜੇ (doc)
        json_object_put (doc);
      ਵਾਪਸ ret;
    }

  ਮੁਕਤ (parser_err);
  ਜੇ (doc)
    json_object_put (doc);

  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_container_exec_with_options (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id,
                                     ਬਣਤਰ libcrun_container_exec_options_s *opts,
                                     libcrun_error_t *err)
{
  cleanup_custom_handler_instance ਬਣਤਰ custom_handler_instance_s *custom_handler = ਨਿਰਾ;
  ਪੂਰਨ container_ਸਥਿਤੀ, ret;
  bool container_paused = false;
  pid_t pid;
  libcrun_container_ਸਥਿਤੀ_t ਸਥਿਤੀ = {};
  ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ = context->state_ਜੜ੍ਹ;
  cleanup_close ਪੂਰਨ terminal_fd = -1;
  cleanup_close ਪੂਰਨ seccomp_fd = -1;
  cleanup_terminal ਖਾਲੀ *orig_terminal = ਨਿਰਾ;
  cleanup_container libcrun_container_t *container = ਨਿਰਾ;
  ਪੂਰਨ container_ret_ਸਥਿਤੀ[2];
  cleanup_close ਪੂਰਨ pipefd0 = -1;
  cleanup_close ਪੂਰਨ pipefd1 = -1;
  cleanup_close ਪੂਰਨ seccomp_receiver_fd = -1;
  cleanup_close ਪੂਰਨ own_seccomp_receiver_fd = -1;
  cleanup_close ਪੂਰਨ seccomp_notify_fd = -1;
  ਅਚਲ ਅੱਖਰ *seccomp_notify_plugins = ਨਿਰਾ;
  __attribute__ ((ਅਣਵਰਤਿਆ)) cleanup_process_schema runtime_spec_schema_config_schema_process *process_cleanup = ਨਿਰਾ;
  runtime_spec_schema_config_schema_process *process = opts->process;
  ਬਣਤਰ libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  ਪੂਰਨ ret_from_ਬੱਚਾ = 0;

  ret = libcrun_read_container_ਸਥਿਤੀ (&ਸਥਿਤੀ, state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_is_container_running (&ਸਥਿਤੀ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;
  container_ਸਥਿਤੀ = ret;

  ret = read_container_config_from_state (&container, state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  container->context = context;

  ਜੇ (container_ਸਥਿਤੀ == 0)
    ਵਾਪਸ crun_make_error (err, 0, "the container `%s` is not running", id);

  {
    cleanup_cgroup_ਸਥਿਤੀ ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ = ਨਿਰਾ;

    cgroup_ਸਥਿਤੀ = libcrun_cgroup_make_ਸਥਿਤੀ (&ਸਥਿਤੀ);

    ret = libcrun_cgroup_is_container_paused (cgroup_ਸਥਿਤੀ, &container_paused, err);
    ਜੇ (UNLIKELY (ret < 0))
      ਵਾਪਸ ret;
  }

  ਜੇ (UNLIKELY (container_paused))
    ਵਾਪਸ crun_make_error (err, 0, "cannot exec in a paused container `%s`", id);

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = ਖੰਡ_ਸੰਕੇਤ (err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  libcrun_seccomp_gen_ctx_init (&seccomp_gen_ctx, container, false, 0);

  ret = libcrun_open_seccomp_bpf (&seccomp_gen_ctx, &seccomp_fd, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &seccomp_receiver_fd,
                                     &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਜੇ (ਆਕਾਰ (*opts) != opts->struct_ਮਾਪ)
    ਵਾਪਸ crun_make_error (err, EINVAL, "invalid libcrun_container_exec_options_s struct");

  ਜੇ (opts->ਰਸਤਾ)
    {
      ਜੇ (process)
        ਵਾਪਸ crun_make_error (err, EINVAL, "cannot specify both exec file and options");

      ret = libcrun_ਲੱਦੋ_process_from_file (opts->ਰਸਤਾ, &process, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      process_cleanup = process;
    }

  /* This must be done before we enter a user namespace.  If the process does
     not specify any rlimits, use the ones from the container configuration.  */
  ਜੇ (process->rlimits_len == 0 && container->container_def->process)
    ret = libcrun_ਸੈੱਟ_rlimits (container->container_def->process->rlimits,
                               container->container_def->process->rlimits_len, err);
  ਨਹੀਂਤਾਂ
    ret = libcrun_ਸੈੱਟ_rlimits (process->rlimits, process->rlimits_len, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_ਸੈੱਟ_mempolicy (container->container_def, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = pipe2 (container_ret_ਸਥਿਤੀ, O_CLOEXEC);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_ਸਥਿਤੀ[0];
  pipefd1 = container_ret_ਸਥਿਤੀ[1];

  /* If the new process block doesn't specify a SELinux label, AppArmor profile, user,
     I/O priority or scheduler, then use the configuration from the original config file.  */
  ਜੇ (container->container_def->process)
    {
      ਜੇ (process->io_ਤਰਜੀਹ == ਨਿਰਾ && container->container_def->process->io_ਤਰਜੀਹ)
        {
          process->io_ਤਰਜੀਹ = clone_runtime_spec_schema_config_schema_process_io_ਤਰਜੀਹ (container->container_def->process->io_ਤਰਜੀਹ);
          ਜੇ (process->io_ਤਰਜੀਹ == ਨਿਰਾ)
            OOM ();
        }

      ਜੇ (process->scheduler == ਨਿਰਾ && container->container_def->process->scheduler)
        {
          process->scheduler = clone_runtime_spec_schema_config_schema_process_scheduler (container->container_def->process->scheduler);
          ਜੇ (process->scheduler == ਨਿਰਾ)
            OOM ();
        }

      ਜੇ (process->selinux_label == ਨਿਰਾ && container->container_def->process->selinux_label)
        process->selinux_label = xstrdup (container->container_def->process->selinux_label);

      ਜੇ (process->apparmor_profile == ਨਿਰਾ && container->container_def->process->apparmor_profile)
        process->apparmor_profile = xstrdup (container->container_def->process->apparmor_profile);

      ਜੇ (process->user == ਨਿਰਾ && container->container_def->process->user)
        {
          process->user = clone_runtime_spec_schema_config_schema_process_user (container->container_def->process->user);
          ਜੇ (process->user == ਨਿਰਾ)
            OOM ();
        }
    }

  ret = initialize_security (container, process, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  /* Do not leak the session keyring of the caller into the container.  The
     keyring created for the container cannot be joined, so use an anonymous one.  */
  ਜੇ (! context->no_new_keyring && ! ਸਥਿਤੀ.no_new_keyring)
    {
      libcrun_debug ("Creating new keyring for the exec process");

      ret = libcrun_create_keyring (container, ਨਿਰਾ, process->selinux_label, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ret = prctl (PR_ਸੈੱਟ_DUMPABLE, 0, 0, 0, 0);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "prctl unset dumpable");

  pid = libcrun_join_process (context, container, ਸਥਿਤੀ.pid, &ਸਥਿਤੀ, opts->cgroup, context->detach,
                              process, process->terminal ? &terminal_fd : ਨਿਰਾ, err);
  ਜੇ (UNLIKELY (pid < 0))
    ਵਾਪਸ pid;

  /* Process to exec.  */
  ਜੇ (pid == 0)
    {
      ਆਰਜ਼ੀ_FAILURE_RETRY (close (pipefd0));
      pipefd0 = -1;

      exec_process_entrypoint (context, container, process, &pipefd1, seccomp_fd, seccomp_receiver_fd, custom_handler, opts->ਮਿਲਾਓ_env, err);
      /* It gets here only on errors.  */
      ਜੇ (*err)
        {
          ਜੇ (pipefd1 < 0)
            libcrun_fail_with_error ((*err)->ਸਥਿਤੀ, "%s", (*err)->ਸੁਨੇਹਾ);
          ਨਹੀਂਤਾਂ
            {
              write_error_ਮੰਜ਼ਿਲ_pipe (pipefd1, err);
              ਆਰਜ਼ੀ_FAILURE_RETRY (close (pipefd1));
              pipefd1 = -1;
            }
        }
      ਸੁਰੱਖਿਅਤ_exit (EXIT_FAILURE);
    }

  ਆਰਜ਼ੀ_FAILURE_RETRY (close (pipefd1));
  pipefd1 = -1;

  ਆਰਜ਼ੀ_FAILURE_RETRY (ਲਿਖ (pipefd0, &pid, ਆਕਾਰ (pid)));

  ਜੇ (seccomp_fd >= 0)
    close_and_reset (&seccomp_fd);

  ਜੇ (terminal_fd >= 0)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ ਪੰਕਤੀਆਂ = 0, ਖਾਨੇ_ਗਿਣਤੀ = 0;

      ਜੇ (process->console_ਮਾਪ)
        {
          ਖਾਨੇ_ਗਿਣਤੀ = process->console_ਮਾਪ->ਚੌੜਾਈ;
          ਪੰਕਤੀਆਂ = process->console_ਮਾਪ->ਉਚਾਈ;
        }

      ret = libcrun_terminal_ਤਿਆਰੀ_ਮਾਪ (terminal_fd, ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ਜੇ (context->console_socket)
        {
          ਪੂਰਨ ret;
          cleanup_close ਪੂਰਨ console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
          ਜੇ (UNLIKELY (console_socket_fd < 0))
            ਵਾਪਸ console_socket_fd;
          ret = send_fd_ਮੰਜ਼ਿਲ_socket (console_socket_fd, terminal_fd, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
          close_and_reset (&terminal_fd);
        }
      ਨਹੀਂਤਾਂ
        {
          ret = libcrun_clear_onlcr (terminal_fd, err);
          ਜੇ (UNLIKELY (ret < 0))
            {
              flush_fd_ਮੰਜ਼ਿਲ_err (context, terminal_fd);
              ਵਾਪਸ ret;
            }

          ret = libcrun_ਸੈੱਟ_raw (0, &orig_terminal, err);
          ਜੇ (UNLIKELY (ret < 0))
            {
              flush_fd_ਮੰਜ਼ਿਲ_err (context, terminal_fd);
              ਵਾਪਸ ret;
            }
        }
    }

  ret = ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (pipefd0, &ret_from_ਬੱਚਾ, ਆਕਾਰ (ret_from_ਬੱਚਾ)));
  ਜੇ (ret != ਆਕਾਰ (ret_from_ਬੱਚਾ))
    ret = crun_make_error (err, 0, "read pipe failed");
  ਨਹੀਂਤਾਂ ਜੇ (ret_from_ਬੱਚਾ != 0)
    {
      cleanup_free ਅੱਖਰ *ਸੁਨੇਹਾ = ਨਿਰਾ;
      size_t len = 0;

      ret = read_all_fd (pipefd0, "error stream", &ਸੁਨੇਹਾ, &len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
      /* the string from read_all_fd is always NUL terminated.  */
      ret = crun_make_error (err, ret_from_ਬੱਚਾ, "%s", ਸੁਨੇਹਾ);
    }
  ਨਹੀਂਤਾਂ
    {
      /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
      ਜੇ (own_seccomp_receiver_fd >= 0)
        {
          seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
          ਜੇ (UNLIKELY (seccomp_notify_fd < 0))
            ਵਾਪਸ seccomp_notify_fd;

          ret = close_and_reset (&own_seccomp_receiver_fd);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "close seccomp receiver fd failed");
        }

      {
        ਬਣਤਰ wait_for_process_ਦਲੀਲਾਂ ਦਲੀਲਾਂ = {
          .pid = pid,
          .context = context,
          .terminal_fd = terminal_fd,
          .notify_socket = -1,
          .container_ready_fd = ਨਿਰਾ,
          .seccomp_notify_fd = seccomp_notify_fd,
          .seccomp_notify_plugins = seccomp_notify_plugins,
        };

        ret = wait_for_process (&ਦਲੀਲਾਂ, err);
      }
    }

  flush_fd_ਮੰਜ਼ਿਲ_err (context, terminal_fd);
  ਵਾਪਸ ret;
}

ਪੂਰਨ
libcrun_container_update (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *content, size_t len arg_ਅਣਵਰਤਿਆ,
                          libcrun_error_t *err)
{
  cleanup_custom_handler_instance ਬਣਤਰ custom_handler_instance_s *custom_handler = ਨਿਰਾ;
  runtime_spec_schema_config_linux_resources *resources = ਨਿਰਾ;
  cleanup_container libcrun_container_t *container = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ = context->state_ਜੜ੍ਹ;
  ਬਣਤਰ parser_context ctx = { 0, stderr };
  libcrun_container_ਸਥਿਤੀ_t ਸਥਿਤੀ = {};
  parser_error parser_err = ਨਿਰਾ;
  json_object *doc = ਨਿਰਾ;
  ਪੂਰਨ ret;

  ret = libcrun_read_container_ਸਥਿਤੀ (&ਸਥਿਤੀ, state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = read_container_config_from_state (&container, state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  resources = make_runtime_spec_schema_config_linux_resources (doc, &ctx, &parser_err);
  ਜੇ (UNLIKELY (resources == ਨਿਰਾ))
    {
      ret = crun_make_error (err, 0, "cannot parse resources: %s", parser_err);
      ਜਾਓ cleanup;
    }

  ਜੇ (custom_handler && custom_handler->vtable->modify_oci_configuration)
    {
      /* Adapt RESOURCES to be used from the modify_oci_configuration hook.  */
      cleanup_free runtime_spec_schema_config_linux *linux = xmalloc0 (ਆਕਾਰ (*linux));
      cleanup_free runtime_spec_schema_config_schema *def = xmalloc0 (ਆਕਾਰ (*def));

      def->linux = linux;
      linux->resources = resources;

      ret = custom_handler->vtable->modify_oci_configuration (custom_handler->cookie,
                                                              context,
                                                              def,
                                                              err);
      ਜੇ (UNLIKELY (ret < 0))
        ਜਾਓ cleanup;
    }

  ret = libcrun_linux_container_update (&ਸਥਿਤੀ, state_ਜੜ੍ਹ, resources, err);

cleanup:
  ਜੇ (doc)
    json_object_put (doc);
  ਮੁਕਤ (parser_err);
  ਜੇ (resources)
    free_runtime_spec_schema_config_linux_resources (resources);

  ਵਾਪਸ ret;
}

ਪੂਰਨ
libcrun_container_update_from_file (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *file, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *content = ਨਿਰਾ;
  size_t len;
  ਪੂਰਨ ret;

  ret = read_all_file (file, &content, &len, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ libcrun_container_update (context, id, content, len, err);
}

ਸਥਿਰ ਪੂਰਨ
compare_update_ਮੁੱਲਾਂ (ਅਚਲ ਖਾਲੀ *a, ਅਚਲ ਖਾਲੀ *b)
{
  ਅਚਲ ਬਣਤਰ libcrun_update_ਮੁੱਲ_s *aa = a;
  ਅਚਲ ਬਣਤਰ libcrun_update_ਮੁੱਲ_s *bb = b;
  ਪੂਰਨ ret;

  ret = strcmp (aa->section, bb->section);
  ਜੇ (ret)
    ਵਾਪਸ ret;
  ਵਾਪਸ strcmp (aa->name, bb->name);
}

ਪੂਰਨ
libcrun_container_update_from_ਮੁੱਲਾਂ (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id,
                                      ਬਣਤਰ libcrun_update_ਮੁੱਲ_s *ਮੁੱਲਾਂ, size_t len,
                                      libcrun_error_t *err)
{
  ਅਚਲ ਅੱਖਰ *current_section = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *buf;
  json_gen_ctx *gen = ਨਿਰਾ;
  size_t i, buf_len;
  ਪੂਰਨ ret;

  ਜੇ (! json_gen_init (&gen, ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, 0, "json_gen_init failed");
  json_gen_map_open (gen);

  qsort (ਮੁੱਲਾਂ, len, ਆਕਾਰ (ਬਣਤਰ libcrun_update_ਮੁੱਲ_s), compare_update_ਮੁੱਲਾਂ);

  ਲਈ (i = 0; i < len; i++)
    {
      ਜੇ (current_section == ਨਿਰਾ || strcmp (ਮੁੱਲਾਂ[i].section, current_section))
        {
          ਜੇ (i > 0)
            json_gen_map_close (gen);

          current_section = ਮੁੱਲਾਂ[i].section;
          json_gen_string (gen, current_section, ਲੜੀ_ਲੰਬਾਈ (current_section));
          json_gen_map_open (gen);
        }

      json_gen_string (gen, ਮੁੱਲਾਂ[i].name, ਲੜੀ_ਲੰਬਾਈ (ਮੁੱਲਾਂ[i].name));

      ਜੇ (ਮੁੱਲਾਂ[i].numeric)
        json_gen_ਸੰਖਿਆ (gen, ਮੁੱਲਾਂ[i].ਮੁੱਲ, ਲੜੀ_ਲੰਬਾਈ (ਮੁੱਲਾਂ[i].ਮੁੱਲ));
      ਨਹੀਂਤਾਂ
        json_gen_string (gen, ਮੁੱਲਾਂ[i].ਮੁੱਲ, ਲੜੀ_ਲੰਬਾਈ (ਮੁੱਲਾਂ[i].ਮੁੱਲ));
    }
  ਜੇ (len)
    json_gen_map_close (gen);

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);

  ret = libcrun_container_update (context, id, buf, buf_len, err);

  json_gen_free (gen);

  ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ
populate_array_ਖੇਤਰ (ਅੱਖਰ ***ਖੇਤਰ, ਅੱਖਰ *array[], size_t ਅੰਕ_elements)
{
  size_t i;

  *ਖੇਤਰ = xmalloc0 ((ਅੰਕ_elements + 1) * ਆਕਾਰ (ਅੱਖਰ *));
  ਲਈ (i = 0; i < ਅੰਕ_elements; i++)
    (*ਖੇਤਰ)[i] = xstrdup (array[i]);

  (*ਖੇਤਰ)[i] = ਨਿਰਾ;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CAP
ਸਥਿਰ ਖਾਲੀ
populate_capabilities (ਬਣਤਰ features_info_s *info, ਅੱਖਰ ***capabilities, size_t *ਅੰਕ_capabilities)
{
  size_t ਸੂਚਕਾਂਕ = 0;
  cap_ਮੁੱਲ_t i;
  ਅੱਖਰ *endptr;
  ਪੂਰਨ j;

  *ਅੰਕ_capabilities = 0;
  ਲਈ (i = 0;; i++)
    {
      ਅੱਖਰ *v = cap_ਮੰਜ਼ਿਲ_name (i);
      ਜੇ (v == ਨਿਰਾ)
        ਰੋਕੋ;
      strtol (v, &endptr, 10);
      ਜੇ (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          ਰੋਕੋ;
        }
      (*ਅੰਕ_capabilities)++;
    }

  *capabilities = xmalloc0 ((*ਅੰਕ_capabilities + 1) * ਆਕਾਰ (ਅਚਲ ਅੱਖਰ *));
  ਲਈ (i = 0; i < (cap_ਮੁੱਲ_t) *ਅੰਕ_capabilities; i++)
    {
      ਅੱਖਰ *v = cap_ਮੰਜ਼ਿਲ_name (i);
      ਜੇ (v == ਨਿਰਾ)
        ਰੋਕੋ;
      strtol (v, &endptr, 10);
      ਜੇ (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          ਰੋਕੋ;
        }

      // Convert capability name to uppercase
      ਲਈ (j = 0; v[j] != '\0'; j++)
        v[j] = ਵੱਡਾ_ਅੱਖਰ (v[j]);

      (*capabilities)[ਸੂਚਕਾਂਕ] = v;
      ਸੂਚਕਾਂਕ++;
    }

  (*capabilities)[ਸੂਚਕਾਂਕ] = ਨਿਰਾ; // Terminate the array with NULL
  populate_array_ਖੇਤਰ (&(info->linux.capabilities), *capabilities, *ਅੰਕ_capabilities);
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਖਾਲੀ
retrieve_mount_options (ਬਣਤਰ features_info_s **info)
{
  cleanup_free ਅਚਲ ਬਣਤਰ ਪ੍ਰਸਾਰ_flags_s *mount_options_list = ਨਿਰਾ;
  size_t ਅੰਕ_mount_options = 0;

  // Retrieve mount options from wordlist
  mount_options_list = get_mount_flags_from_wordlist ();

  // Calculate the number of mount options
  ਜਦੋਂ (mount_options_list[ਅੰਕ_mount_options].name != ਨਿਰਾ)
    ਅੰਕ_mount_options++;

  // Allocate memory for mount options in info struct
  (*info)->mount_options = xmalloc0 ((ਅੰਕ_mount_options + 1) * ਆਕਾਰ (ਅੱਖਰ *));

  // Copy mount options to info struct
  ਲਈ (size_t i = 0; i < ਅੰਕ_mount_options; i++)
    (*info)->mount_options[i] = xstrdup (mount_options_list[i].name);
}

ਪੂਰਨ
libcrun_container_get_features (libcrun_context_t *context, ਬਣਤਰ features_info_s **info, libcrun_error_t *err arg_ਅਣਵਰਤਿਆ)
{
  // Allocate memory for the features_info_s structure
  size_t ਅੰਕ_namspaces = ਆਕਾਰ (namespaces) / ਆਕਾਰ (namespaces[0]);
  size_t ਅੰਕ_operators = ਆਕਾਰ (operators) / ਆਕਾਰ (operators[0]);
  size_t ਅੰਕ_actions = ਆਕਾਰ (actions) / ਆਕਾਰ (actions[0]);
  size_t ਅੰਕ_hooks = ਆਕਾਰ (hooks) / ਆਕਾਰ (hooks[0]);
  size_t ਅੰਕ_archs = ਆਕਾਰ (archs) / ਆਕਾਰ (archs[0]);
  size_t ਅੰਕ_unsafe_annotations = ਆਕਾਰ (potentially_unsafe_annotations) / ਆਕਾਰ (potentially_unsafe_annotations[0]);
  cleanup_free ਅੱਖਰ **capabilities = ਨਿਰਾ;
  size_t ਅੰਕ_capabilities = 0;
  size_t ਅੰਕ_mempolicy_modes = ਆਕਾਰ (mempolicy_modes) / ਆਕਾਰ (mempolicy_modes[0]);
  size_t ਅੰਕ_mempolicy_flags = ਆਕਾਰ (mempolicy_flags) / ਆਕਾਰ (mempolicy_flags[0]);

  *info = xmalloc0 (ਆਕਾਰ (ਬਣਤਰ features_info_s));

  // Hardcoded feature information
  (*info)->oci_ਸੰਸਕਰਣ_min = xstrdup ("1.0.0");
  (*info)->oci_ਸੰਸਕਰਣ_max = xstrdup ("1.1.0+dev");

  // Populate hooks
  populate_array_ਖੇਤਰ (&((*info)->hooks), hooks, ਅੰਕ_hooks);

  // Populate mount_options
  retrieve_mount_options (info);

  // Populate namespaces
  populate_array_ਖੇਤਰ (&((*info)->linux.namespaces), namespaces, ਅੰਕ_namspaces);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CAP
  // Populate capabilities
  populate_capabilities (*info, &capabilities, &ਅੰਕ_capabilities);
#ਜੇ_ਅੰਤ

  // Hardcode the values for cgroup
  (*info)->linux.cgroup.v1 = true;
  (*info)->linux.cgroup.v2 = true;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYSTEMD
  (*info)->linux.cgroup.systemd = true;
  (*info)->linux.cgroup.systemd_user = true;
#ਜੇ_ਅੰਤ

  // Put seccomp values
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SECCOMP
  (*info)->linux.seccomp.enabled = true;
  // Populate actions
  populate_array_ਖੇਤਰ (&((*info)->linux.seccomp.actions), actions, ਅੰਕ_actions);
  // Populate operators
  populate_array_ਖੇਤਰ (&((*info)->linux.seccomp.operators), operators, ਅੰਕ_operators);
  // Populate archs
  populate_array_ਖੇਤਰ (&((*info)->linux.seccomp.archs), archs, ਅੰਕ_archs);
#ਨਹੀਂਤਾਂ
  (*info)->linux.seccomp.enabled = false;
#ਜੇ_ਅੰਤ

  // Put values for apparmor and selinux
  (*info)->linux.apparmor.enabled = true;
  (*info)->linux.selinux.enabled = true;

  (*info)->linux.intel_rdt.enabled = true;

  (*info)->linux.net_devices.enabled = true;

  populate_array_ਖੇਤਰ (&((*info)->linux.ਸਮ੍ਰਿਤੀ_policy.ਬਹੁਲਕ), mempolicy_modes, ਅੰਕ_mempolicy_modes);
  populate_array_ਖੇਤਰ (&((*info)->linux.ਸਮ੍ਰਿਤੀ_policy.flags), mempolicy_flags, ਅੰਕ_mempolicy_flags);

  // Put the values for mount extensions
  (*info)->linux.mount_ext.idmap.enabled = true;

  // Populate the values for annotations
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SECCOMP
  {
    ਅਚਲ ਬਣਤਰ scmp_ਸੰਸਕਰਣ *ਸੰਸਕਰਣ = seccomp_ਸੰਸਕਰਣ ();
    ਅੱਖਰ *ਸੰਸਕਰਣ_string = ਨਿਰਾ;

    xasprintf (&ਸੰਸਕਰਣ_string, "%u.%u.%u", ਸੰਸਕਰਣ->major, ਸੰਸਕਰਣ->minor, ਸੰਸਕਰਣ->micro);
    (*info)->annotations.io_github_seccomp_libseccomp_ਸੰਸਕਰਣ = ਸੰਸਕਰਣ_string;
  }
#ਜੇ_ਅੰਤ

  ਜੇ (context->handler_manager && handler_by_name (context->handler_manager, "wasm"))
    (*info)->annotations.ਲੜੀਵਾਰ_oci_crun_wasm = true;

#ਜੇ HAVE_CRIU
  (*info)->annotations.ਲੜੀਵਾਰ_oci_crun_checkpoint_enabled = true;
#ਜੇ_ਅੰਤ
  (*info)->annotations.ਲੜੀਵਾਰ_oci_crun_commit = GIT_ਸੰਸਕਰਣ;
  (*info)->annotations.ਲੜੀਵਾਰ_oci_crun_ਸੰਸਕਰਣ = PACKAGE_ਸੰਸਕਰਣ;

  populate_array_ਖੇਤਰ (&((*info)->potentially_unsafe_annotations), potentially_unsafe_annotations, ਅੰਕ_unsafe_annotations);

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ
features_ਜੋੜੋ_string (json_gen_ctx *json_gen, ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
  json_gen_string (json_gen, ਕੁੰਜੀ, ਲੜੀ_ਲੰਬਾਈ (ਕੁੰਜੀ));
  json_gen_string (json_gen, ਮੁੱਲ, ਲੜੀ_ਲੰਬਾਈ (ਮੁੱਲ));
}

ਸਥਿਰ ਖਾਲੀ
features_ਜੋੜੋ_bool (json_gen_ctx *json_gen, ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ, ਪੂਰਨ ਮੁੱਲ)
{
  json_gen_string (json_gen, ਕੁੰਜੀ, ਲੜੀ_ਲੰਬਾਈ (ਕੁੰਜੀ));
  json_gen_bool (json_gen, ਮੁੱਲ);
}

ਸਥਿਰ ਖਾਲੀ
features_ਜੋੜੋ_bool_str (json_gen_ctx *json_gen, ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ, ਪੂਰਨ ਮੁੱਲ)
{
  features_ਜੋੜੋ_string (json_gen, ਕੁੰਜੀ, ਮੁੱਲ ? "true" : "false");
}

ਸਥਿਰ ਖਾਲੀ
features_ਜੋੜੋ_array (json_gen_ctx *json_gen, ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ, ਅੱਖਰ **array)
{
  size_t i;

  json_gen_string (json_gen, ਕੁੰਜੀ, ਲੜੀ_ਲੰਬਾਈ (ਕੁੰਜੀ));
  json_gen_array_open (json_gen);
  ਜੇ (array)
    ਲਈ (i = 0; array[i] != ਨਿਰਾ; i++)
      json_gen_string (json_gen, array[i], ਲੜੀ_ਲੰਬਾਈ (array[i]));
  json_gen_array_close (json_gen);
}

ਸਥਿਰ ਖਾਲੀ
features_ਜੋੜੋ_linux_info (json_gen_ctx *json_gen, ਅਚਲ ਬਣਤਰ linux_info_s *linux)
{
  json_gen_string (json_gen, "linux", ਲੜੀ_ਲੰਬਾਈ ("linux"));
  json_gen_map_open (json_gen);

  features_ਜੋੜੋ_array (json_gen, "namespaces", linux->namespaces);
  features_ਜੋੜੋ_array (json_gen, "capabilities", linux->capabilities);

  json_gen_string (json_gen, "cgroup", ਲੜੀ_ਲੰਬਾਈ ("cgroup"));
  json_gen_map_open (json_gen);
  features_ਜੋੜੋ_bool (json_gen, "v1", linux->cgroup.v1);
  features_ਜੋੜੋ_bool (json_gen, "v2", linux->cgroup.v2);
  features_ਜੋੜੋ_bool (json_gen, "systemd", linux->cgroup.systemd);
  features_ਜੋੜੋ_bool (json_gen, "systemdUser", linux->cgroup.systemd_user);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "seccomp", ਲੜੀ_ਲੰਬਾਈ ("seccomp"));
  json_gen_map_open (json_gen);
  features_ਜੋੜੋ_bool (json_gen, "enabled", linux->seccomp.enabled);
  ਜੇ (linux->seccomp.actions)
    features_ਜੋੜੋ_array (json_gen, "actions", linux->seccomp.actions);
  ਜੇ (linux->seccomp.operators)
    features_ਜੋੜੋ_array (json_gen, "operators", linux->seccomp.operators);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "apparmor", ਲੜੀ_ਲੰਬਾਈ ("apparmor"));
  json_gen_map_open (json_gen);
  features_ਜੋੜੋ_bool (json_gen, "enabled", linux->apparmor.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "selinux", ਲੜੀ_ਲੰਬਾਈ ("selinux"));
  json_gen_map_open (json_gen);
  features_ਜੋੜੋ_bool (json_gen, "enabled", linux->selinux.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "mountExtensions", ਲੜੀ_ਲੰਬਾਈ ("mountExtensions"));
  json_gen_map_open (json_gen);
  json_gen_string (json_gen, "idmap", ਲੜੀ_ਲੰਬਾਈ ("idmap"));
  json_gen_map_open (json_gen);
  features_ਜੋੜੋ_bool (json_gen, "enabled", linux->mount_ext.idmap.enabled);
  json_gen_map_close (json_gen);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "intelRdt", ਲੜੀ_ਲੰਬਾਈ ("intelRdt"));
  json_gen_map_open (json_gen);
  features_ਜੋੜੋ_bool (json_gen, "enabled", linux->intel_rdt.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "netDevices", ਲੜੀ_ਲੰਬਾਈ ("netDevices"));
  json_gen_map_open (json_gen);
  features_ਜੋੜੋ_bool (json_gen, "enabled", linux->net_devices.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "memoryPolicy", ਲੜੀ_ਲੰਬਾਈ ("memoryPolicy"));
  json_gen_map_open (json_gen);
  ਜੇ (linux->ਸਮ੍ਰਿਤੀ_policy.ਬਹੁਲਕ)
    features_ਜੋੜੋ_array (json_gen, "modes", linux->ਸਮ੍ਰਿਤੀ_policy.ਬਹੁਲਕ);
  ਜੇ (linux->ਸਮ੍ਰਿਤੀ_policy.flags)
    features_ਜੋੜੋ_array (json_gen, "flags", linux->ਸਮ੍ਰਿਤੀ_policy.flags);
  json_gen_map_close (json_gen);

  json_gen_map_close (json_gen);
}

ਸਥਿਰ ਖਾਲੀ
features_ਜੋੜੋ_annotations_info (json_gen_ctx *json_gen, ਅਚਲ ਬਣਤਰ annotations_info_s *annotation)
{
  json_gen_string (json_gen, "annotations", ਲੜੀ_ਲੰਬਾਈ ("annotations"));
  json_gen_map_open (json_gen);

  ਜੇ (! is_ਖਾਲੀ_ਥਾਵਾਂ_string (annotation->io_github_seccomp_libseccomp_ਸੰਸਕਰਣ))
    features_ਜੋੜੋ_string (json_gen, "io.github.seccomp.libseccomp.version", annotation->io_github_seccomp_libseccomp_ਸੰਸਕਰਣ);

  features_ਜੋੜੋ_bool_str (json_gen, "org.opencontainers.runc.checkpoint.enabled", annotation->ਲੜੀਵਾਰ_oci_crun_checkpoint_enabled);
  features_ਜੋੜੋ_bool_str (json_gen, "run.oci.crun.checkpoint.enabled", annotation->ਲੜੀਵਾਰ_oci_crun_checkpoint_enabled);

  features_ਜੋੜੋ_string (json_gen, "run.oci.crun.commit", annotation->ਲੜੀਵਾਰ_oci_crun_commit);
  features_ਜੋੜੋ_string (json_gen, "run.oci.crun.version", annotation->ਲੜੀਵਾਰ_oci_crun_ਸੰਸਕਰਣ);

  features_ਜੋੜੋ_bool_str (json_gen, "run.oci.crun.wasm", annotation->ਲੜੀਵਾਰ_oci_crun_wasm);

  json_gen_map_close (json_gen);
}

ਪੂਰਨ
libcrun_container_get_features_json (libcrun_context_t *context, ਅੱਖਰ **ਬਾਹਰਲਾ, libcrun_error_t *err)
{
  cleanup_struct_features ਬਣਤਰ features_info_s *info = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *ਬਫਰ = ਨਿਰਾ;
  json_gen_ctx *json_gen;
  size_t ਬਫਰ_len;
  ਪੂਰਨ ret;

  *ਬਾਹਰਲਾ = ਨਿਰਾ;

  ret = libcrun_container_get_features (context, &info, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (! json_gen_init (&json_gen, ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, 0, "cannot initialize JSON generator");

  json_gen_config (json_gen, json_gen_beautify, 1);

  json_gen_map_open (json_gen);

  features_ਜੋੜੋ_string (json_gen, "ociVersionMin", info->oci_ਸੰਸਕਰਣ_min);
  features_ਜੋੜੋ_string (json_gen, "ociVersionMax", info->oci_ਸੰਸਕਰਣ_max);
  features_ਜੋੜੋ_array (json_gen, "hooks", info->hooks);
  features_ਜੋੜੋ_array (json_gen, "mountOptions", info->mount_options);
  features_ਜੋੜੋ_linux_info (json_gen, &info->linux);
  features_ਜੋੜੋ_annotations_info (json_gen, &info->annotations);
  features_ਜੋੜੋ_array (json_gen, "potentiallyUnsafeConfigAnnotations", info->potentially_unsafe_annotations);

  json_gen_map_close (json_gen);

  json_gen_get_buf (json_gen, &ਬਫਰ, &ਬਫਰ_len);
  *ਬਾਹਰਲਾ = xstrdup (ਬਫਰ);
  json_gen_free (json_gen);

  ਵਾਪਸ 0;
}

/* Read the container status and fail if the container is not running.  */
ਸਥਿਰ ਪੂਰਨ
read_ਸਥਿਤੀ_require_running (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id,
                             libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ret = libcrun_read_container_ਸਥਿਤੀ (ਸਥਿਤੀ, context->state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_is_container_running (ਸਥਿਤੀ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;
  ਜੇ (ret == 0)
    ਵਾਪਸ crun_make_error (err, 0, "the container `%s` is not running", id);

  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_container_pause (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  libcrun_container_ਸਥਿਤੀ_t ਸਥਿਤੀ = {};

  ret = read_ਸਥਿਤੀ_require_running (context, id, &ਸਥਿਤੀ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ libcrun_container_pause_linux (&ਸਥਿਤੀ, err);
}

ਪੂਰਨ
libcrun_container_unpause (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  libcrun_container_ਸਥਿਤੀ_t ਸਥਿਤੀ = {};

  ret = read_ਸਥਿਤੀ_require_running (context, id, &ਸਥਿਤੀ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ libcrun_container_unpause_linux (&ਸਥਿਤੀ, err);
}

/* Translate the public options struct into the internal representation used by
   the linux/CRIU layer.  Strings are borrowed (not copied).  */
ਸਥਿਰ ਖਾਲੀ
cr_options_from_ਜਨਤਕ (libcrun_checkpoint_restore_t *dst, ਬਣਤਰ libcrun_checkpoint_restore_options_s *src)
{
  memset (dst, 0, ਆਕਾਰ (*dst));
  dst->image_ਰਸਤਾ = (ਅੱਖਰ *) src->image_ਰਸਤਾ;
  dst->work_ਰਸਤਾ = (ਅੱਖਰ *) src->work_ਰਸਤਾ;
  dst->ਮਾਪਾ_ਰਸਤਾ = (ਅੱਖਰ *) src->ਮਾਪਾ_ਰਸਤਾ;
  dst->console_socket = src->console_socket;
  dst->lsm_profile = (ਅੱਖਰ *) src->lsm_profile;
  dst->lsm_mount_context = (ਅੱਖਰ *) src->lsm_mount_context;
  dst->leave_running = src->leave_running;
  dst->tcp_established = src->tcp_established;
  dst->tcp_close = src->tcp_close;
  dst->ext_unix_sk = src->ext_unix_sk;
  dst->shell_ਕੰਮ = src->shell_ਕੰਮ;
  dst->file_locks = src->file_locks;
  dst->pre_dump = src->pre_dump;
  dst->detach = src->detach;
  dst->manage_cgroups_ਬਹੁਲਕ = src->manage_cgroups_ਬਹੁਲਕ;
  dst->ਜਾਲ_ਤਾਲਾ_ਵਿਧੀ = src->ਜਾਲ_ਤਾਲਾ_ਵਿਧੀ;
}

ਪੂਰਨ
libcrun_container_checkpoint (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id,
                              ਬਣਤਰ libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ = context->state_ਜੜ੍ਹ;
  libcrun_container_ਸਥਿਤੀ_t ਸਥਿਤੀ = {};
  libcrun_checkpoint_restore_t cr;
  cleanup_container libcrun_container_t *container = ਨਿਰਾ;

  cr_options_from_ਜਨਤਕ (&cr, cr_options);

  ret = read_ਸਥਿਤੀ_require_running (context, id, &ਸਥਿਤੀ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = read_container_config_from_state (&container, state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  /* Needed to use the same paths (e.g. the shared empty directory used for
     masked paths) as the container runtime and restore do.  */
  container->context = context;

  ret = libcrun_container_checkpoint_linux (&ਸਥਿਤੀ, container, &cr, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (! (cr_options->leave_running || cr_options->pre_dump))
    ਵਾਪਸ container_delete_ਅੰਦਰੂਨੀ (context, ਨਿਰਾ, ਨਿਰਾ, id, true, true, err);

  ਵਾਪਸ 0;
}

/* The dummy process only keeps the cgroup populated until the container is restored there.
   Its cgroup is below the container cgroup, so once the container runs it can freeze or kill
   the dummy process: never depend on it after that point.  */
ਸਥਿਰ ਖਾਲੀ
restore_proxy_process (ਪੂਰਨ proxy_pid_pipe)
{
  ਅੱਖਰ c;

  /* Wait to be killed by the parent, or for the parent to go away.  */
  ਆਰਜ਼ੀ_FAILURE_RETRY (ਪਠ (proxy_pid_pipe, &c, ਆਕਾਰ (c)));
}

ਸਥਿਰ ਪੂਰਨ
restore_terminate_proxy_process (pid_t proxy_pid, ਅਚਲ ਅੱਖਰ *proxy_cgroup, pid_t container_pid, ਪੂਰਨ cgroup_manager, libcrun_error_t *err)
{
  ਪੂਰਨ ਬਹੁਲਕ;
  ਪੂਰਨ ret;

  ਜੇ (cgroup_manager == CGROUP_MANAGER_SYSTEMD)
    {
      ਅੱਖਰ ready_str[64];

      ret = snprintf (ready_str, ਆਕਾਰ (ready_str), "MAINPID=%d", container_pid);
      ਜੇ (UNLIKELY (ret >= (ਪੂਰਨ) ਆਕਾਰ (ready_str)))
        ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = send_sd_notify (ready_str, err);
      /* Do not fail on errors.  */
      ਜੇ (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (ਬਹੁਲਕ < 0))
    ਵਾਪਸ ਬਹੁਲਕ;

  /* SIGKILL terminates a process frozen with cgroup v2, while the cgroup v1 freezer does not
     deliver it until the cgroup is thawed, so do not wait for it there.  */
  kill (proxy_pid, SIGKILL);
  ਆਰਜ਼ੀ_FAILURE_RETRY (waitpid (proxy_pid, ਨਿਰਾ, ਬਹੁਲਕ == CGROUP_ਬਹੁਲਕ_UNIFIED ? 0 : WNOHANG));

  ਵਾਪਸ destroy_cgroup_ਰਸਤਾ (proxy_cgroup, ਬਹੁਲਕ, err);
}

ਪੂਰਨ
libcrun_container_restore (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id arg_ਅਣਵਰਤਿਆ,
                           ਬਣਤਰ libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  cleanup_cgroup_ਸਥਿਤੀ ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ = ਨਿਰਾ;
  cleanup_container libcrun_container_t *container = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ proxy_pid_pipe0 = -1;
  cleanup_close ਪੂਰਨ proxy_pid_pipe1 = -1;
  runtime_spec_schema_config_schema *def;
  libcrun_container_ਸਥਿਤੀ_t ਸਥਿਤੀ = {};
  ਬਣਤਰ libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  libcrun_checkpoint_restore_t cr;
  cleanup_pid pid_t proxy_pid = -1;
  cleanup_close ਪੂਰਨ seccomp_fd = -1;
  ਅਚਲ ਅੱਖਰ *seccomp_bpf_ਅੰਕੜੇ;
  ਪੂਰਨ proxy_pid_pipe[2];
  ਪੂਰਨ cgroup_manager;
  uid_t ਜੜ੍ਹ_uid = -1;
  gid_t ਜੜ੍ਹ_gid = -1;
  ਪੂਰਨ ret;

  cr_options_from_ਜਨਤਕ (&cr, cr_options);

  container = libcrun_container_ਲੱਦੋ_from_file ("config.json", err);
  ਜੇ (container == ਨਿਰਾ)
    ਵਾਪਸ -1;

  container->context = context;
  def = container->container_def;

  ret = check_config_file (def, context, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_ਸਥਿਤੀ_check_directories (context->state_ਜੜ੍ਹ, context->id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_copy_config_file (context->id, context->state_ਜੜ੍ਹ, container, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ fail;

  /* CRIU restores the seccomp filters of the checkpointed processes, but
     exec loads the filter from the seccomp.bpf file in the state directory,
     so it must be generated again for the restored container.  */
  seccomp_bpf_ਅੰਕੜੇ = ਲੱਭੋ_annotation (container, "run.oci.seccomp_bpf_data");

  ret = ਤਿਆਰੀ_seccomp (container, seccomp_bpf_ਅੰਕੜੇ, &seccomp_gen_ctx, &seccomp_fd, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ fail;

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_ਅੰਕੜੇ, &seccomp_gen_ctx, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ fail;
  close_and_reset (&seccomp_fd);

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  ਜੇ (context->systemd_cgroup)
    cgroup_manager = CGROUP_MANAGER_SYSTEMD;
  ਨਹੀਂਤਾਂ ਜੇ (context->force_no_cgroup)
    cgroup_manager = CGROUP_MANAGER_DISABLED;

  def = container->container_def;

  /* If we are root (either on the host or in a namespace),
   * then chown the cgroup to root in the container user namespace. */
  get_ਜੜ੍ਹ_ਵਿੱਚ_the_userns (def, container->host_uid, container->host_gid, &ਜੜ੍ਹ_uid, &ਜੜ੍ਹ_gid);

  /* If the root in the container is different than the current root user, attempt to chown
     the std streams before entering the user namespace.  Otherwise we might lose access
     to the user (as it is not mapped in the user namespace) and cannot chown them.  */
  ਜੇ (ਜੜ੍ਹ_uid > 0 || ਜੜ੍ਹ_gid > 0)
    {
      ret = maybe_chown_std_streams (ਜੜ੍ਹ_uid, ਜੜ੍ਹ_gid, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਜਾਓ fail;
    }

  /* CRIU runs as a child process, so the restored processes would inherit the session keyring.  */
  ret = ਤਿਆਰੀ_container_keyring (container, context, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  {
    cleanup_close ਪੂਰਨ cgroup_dirfd = -1;
    bool needs_proxy_process;
    cleanup_cgroup_preenter ਬਣਤਰ libcrun_cgroup_ਦਲੀਲਾਂ cg = {
      .resources = def->linux ? def->linux->resources : ਨਿਰਾ,
      .annotations = container->annotations,
      .cgroup_ਰਸਤਾ = def->linux ? def->linux->cgroups_ਰਸਤਾ : "",
      .manager = cgroup_manager,
      .ਜੜ੍ਹ_uid = ਜੜ੍ਹ_uid,
      .ਜੜ੍ਹ_gid = ਜੜ੍ਹ_gid,
      .id = context->id,
      .state_ਜੜ੍ਹ = context->state_ਜੜ੍ਹ,
      .container = container,
    };

    /* The CRIU restore code uses bundle, rootfs and cgroup_path of status.   The cgroup_path is set later.  */
    ਸਥਿਤੀ.bundle = (ਅੱਖਰ *) context->bundle;
    ਸਥਿਤੀ.rootfs = def->ਜੜ੍ਹ->ਰਸਤਾ;

    ret = libcrun_cgroup_preenter (&cg, &cgroup_dirfd, err);
    ਜੇ (UNLIKELY (ret < 0))
      ਜਾਓ fail;

    needs_proxy_process = cgroup_dirfd < 0;

    /* If the target cgroup was already created (with cgroupfs), we can restore the container directly there.  */
    ਜੇ (! needs_proxy_process)
      {
        cleanup_free ਅੱਖਰ *cgroup_ਰਸਤਾ = ਨਿਰਾ;

        ret = get_cgroup_dirfd_ਰਸਤਾ (cgroup_dirfd, &cgroup_ਰਸਤਾ, err);
        ਜੇ (UNLIKELY (ret < 0))
          ਜਾਓ fail;

        /* Complete the configuration for the cgroup before the container is
           restored there: CRIU resumes the restored processes before it
           returns, so the device filter and the limits must already be in
           place.  There is no process to move, as CRIU is run in the cgroup,
           and the controllers were enabled by precreate_cgroup.  */
        cg.joined = true;

        ret = libcrun_cgroup_enter (&cg, &cgroup_ਸਥਿਤੀ, err);
        ਜੇ (UNLIKELY (ret < 0))
          ਜਾਓ fail;

        /* Restore the container directly in the desired cgroup.  */
        ਸਥਿਤੀ.cgroup_ਰਸਤਾ = cgroup_ਰਸਤਾ;

        ret = libcrun_container_restore_linux (&ਸਥਿਤੀ, container, &cr, err);
        ਜੇ (UNLIKELY (ret < 0))
          ਜਾਓ fail;

        /* Now that the container is restored, keep any signal for it, so that
           it is forwarded to the init by wait_for_process, rather than acted
           upon by crun.  This cannot be done earlier, as CRIU, which is run as
           a child process, would inherit the blocked signals.  */
        ret = ਖੰਡ_ਸੰਕੇਤ (err);
        ਜੇ (UNLIKELY (ret < 0))
          ਜਾਓ fail;

        /* Use the container first process PID to finalize the cgroup.  */
        cg.pid = ਸਥਿਤੀ.pid;
      }
    ਨਹੀਂਤਾਂ
      {
        /* When using systemd, we need a process first that must exist before a cgroup is created.
           Create a dummy process that sits and waits until the container is restored.  */
        ret = pipe2 (proxy_pid_pipe, O_CLOEXEC);
        ਜੇ (UNLIKELY (ret < 0))
          {
            ret = crun_make_error (err, errno, "pipe");
            ਜਾਓ fail;
          }

        /* Only used for auto cleanup.  */
        proxy_pid_pipe0 = proxy_pid_pipe[0];
        proxy_pid_pipe1 = proxy_pid_pipe[1];

        proxy_pid = fork ();
        ਜੇ (UNLIKELY (proxy_pid < 0))
          {
            ret = crun_make_error (err, errno, "fork");
            ਜਾਓ fail;
          }

        ਜੇ (proxy_pid == 0)
          {
            close_and_reset (&proxy_pid_pipe1);

            restore_proxy_process (proxy_pid_pipe0);
            ਸੁਰੱਖਿਅਤ_exit (EXIT_SUCCESS);
          }

        close_and_reset (&proxy_pid_pipe0);

        /* Use the dummy process PID as the PID to setup the cgroup.  */
        cg.pid = proxy_pid;

        /* Complete the configuration for the cgroup.  It contains only the dummy process.  */
        ret = libcrun_cgroup_enter (&cg, &cgroup_ਸਥਿਤੀ, err);
        ਜੇ (UNLIKELY (ret < 0))
          ਜਾਓ fail;
      }

    ret = libcrun_cgroup_enter_finalize (&cg, cgroup_ਸਥਿਤੀ, err);
    ਜੇ (UNLIKELY (ret < 0))
      ਜਾਓ fail;

    /* When using a dummy process, the container is restored here once the cgroup is configured.  */
    ਜੇ (needs_proxy_process)
      {
        cleanup_free ਅੱਖਰ *ਨਿਸ਼ਾਨਾ_cgroup = ਨਿਰਾ;
        cleanup_free ਅੱਖਰ *proxy_cgroup = ਨਿਰਾ;

        ret = libcrun_get_cgroup_process (proxy_pid, &ਨਿਸ਼ਾਨਾ_cgroup, false, err);
        ਜੇ (UNLIKELY (ret < 0))
          ਜਾਓ fail;

        /* Move the dummy process to a sub-cgroup.  */
        ret = append_paths (&proxy_cgroup, err, ਨਿਸ਼ਾਨਾ_cgroup, ".proxy-crun", ਨਿਰਾ);
        ਜੇ (UNLIKELY (ret < 0))
          ਜਾਓ fail;

        ret = libcrun_move_process_ਮੰਜ਼ਿਲ_cgroup (proxy_pid, 0, proxy_cgroup, true, err);
        ਜੇ (UNLIKELY (ret < 0))
          ਜਾਓ fail;

        /* Restore the container in the same cgroup where the dummy process was.  */
        ਸਥਿਤੀ.cgroup_ਰਸਤਾ = ਨਿਸ਼ਾਨਾ_cgroup;

        ret = libcrun_container_restore_linux (&ਸਥਿਤੀ, container, &cr, err);
        ਜੇ (UNLIKELY (ret < 0))
          ਜਾਓ fail;

        /* See the comment for the other libcrun_container_restore_linux call.  */
        ret = ਖੰਡ_ਸੰਕੇਤ (err);
        ਜੇ (UNLIKELY (ret < 0))
          ਜਾਓ fail;

        /* The container is running, so do not fail because of the dummy process.  */
        ret = restore_terminate_proxy_process (proxy_pid, proxy_cgroup, ਸਥਿਤੀ.pid, cgroup_manager, err);
        ਜੇ (UNLIKELY (ret < 0))
          crun_error_release (err);
        proxy_pid = -1;

        close_and_reset (&proxy_pid_pipe1);
      }
  }

  context->detach = cr_options->detach;
  ret = write_container_ਸਥਿਤੀ (container, context, ਸਥਿਤੀ.pid, cgroup_ਸਥਿਤੀ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ fail;

  {
    /* Wait the same way run and exec do: apart from waiting for the container
       init, this writes the pid file, forwards any signal to the init, and
       reaps every child, not only the init.  */
    ਬਣਤਰ wait_for_process_ਦਲੀਲਾਂ ਦਲੀਲਾਂ = {
      .pid = ਸਥਿਤੀ.pid,
      .context = context,
      .terminal_fd = -1,
      .notify_socket = -1,
      .container_ready_fd = ਨਿਰਾ,
      .seccomp_notify_fd = -1,
      .seccomp_notify_plugins = ਨਿਰਾ,
    };

    ਵਾਪਸ wait_for_process (&ਦਲੀਲਾਂ, err);
  }

fail:
  /* Once restored, the container is running.  Without a status file nothing
     can reach it anymore, so do not leave it behind.  */
  ਜੇ (ਸਥਿਤੀ.pid > 0)
    {
      kill (ਸਥਿਤੀ.pid, SIGKILL);
      waitpid_ignore_stopped (ਸਥਿਤੀ.pid, ਨਿਰਾ, 0);
    }
  ਜੇ (cgroup_ਸਥਿਤੀ)
    {
      libcrun_error_t tmp_err = ਨਿਰਾ;
      libcrun_cgroup_destroy (cgroup_ਸਥਿਤੀ, &tmp_err);
      crun_error_release (&tmp_err);
    }
  {
    libcrun_error_t tmp_err = ਨਿਰਾ;

    ਜੇ (libcrun_container_delete_ਸਥਿਤੀ (context->state_ਜੜ੍ਹ, context->id, &tmp_err) < 0)
      crun_error_release (&tmp_err);
  }
  ਵਾਪਸ ret;
}

ਪੂਰਨ
libcrun_container_read_pids (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  cleanup_cgroup_ਸਥਿਤੀ ਬਣਤਰ libcrun_cgroup_ਸਥਿਤੀ *cgroup_ਸਥਿਤੀ = ਨਿਰਾ;
  cleanup_container_ਸਥਿਤੀ libcrun_container_ਸਥਿਤੀ_t ਸਥਿਤੀ = {};
  ਪੂਰਨ ret;

  ret = libcrun_read_container_ਸਥਿਤੀ (&ਸਥਿਤੀ, context->state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (ਸਥਿਤੀ.cgroup_ਰਸਤਾ == ਨਿਰਾ || ਸਥਿਤੀ.cgroup_ਰਸਤਾ[0] == '\0')
    ਵਾਪਸ crun_make_error (err, 0, "the container is not using cgroups");

  cgroup_ਸਥਿਤੀ = libcrun_cgroup_make_ਸਥਿਤੀ (&ਸਥਿਤੀ);

  ਵਾਪਸ libcrun_cgroup_read_pids (cgroup_ਸਥਿਤੀ, recurse, pids, err);
}

ਪੂਰਨ
libcrun_container_list_json (libcrun_context_t *context, ਅੱਖਰ **ਬਾਹਰਲਾ, libcrun_error_t *err)
{
  libcrun_container_list_t *list = ਨਿਰਾ, *it;
  ਅਚਲ ਅੱਖਰ *content = ਨਿਰਾ;
  json_gen_ctx *gen = ਨਿਰਾ;
  size_t len;
  ਪੂਰਨ ret;

  *ਬਾਹਰਲਾ = ਨਿਰਾ;

  ret = libcrun_get_containers_list (&list, context->state_ਜੜ੍ਹ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (! json_gen_init (&gen, ਨਿਰਾ))
    {
      ret = crun_make_error (err, 0, "cannot allocate json generator");
      ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
    }

  json_gen_config (gen, json_gen_beautify, 1);
  json_gen_array_open (gen);

  ਲਈ (it = list; it; it = it->ਅਗਲਾ)
    {
      libcrun_container_ਸਥਿਤੀ_t ਸਥਿਤੀ;
      ਪੂਰਨ running = 0;
      ਪੂਰਨ pid;
      ਅਚਲ ਅੱਖਰ *container_ਸਥਿਤੀ = ਨਿਰਾ;

      ret = libcrun_read_container_ਸਥਿਤੀ (&ਸਥਿਤੀ, context->state_ਜੜ੍ਹ, it->name, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;

      pid = ਸਥਿਤੀ.pid;

      ret = libcrun_get_container_state_string (it->name, &ਸਥਿਤੀ, context->state_ਜੜ੍ਹ, &container_ਸਥਿਤੀ,
                                                &running, err);
      ਜੇ (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          ਜਾਰੀ;
        }

      ਜੇ (! running)
        pid = 0;

      json_gen_map_open (gen);
      json_gen_string (gen, "id", ਲੜੀ_ਲੰਬਾਈ ("id"));
      json_gen_string (gen, it->name, ਲੜੀ_ਲੰਬਾਈ (it->name));
      json_gen_string (gen, "pid", ਲੜੀ_ਲੰਬਾਈ ("pid"));
      map_int (gen, pid);
      json_gen_string (gen, "status", ਲੜੀ_ਲੰਬਾਈ ("status"));
      json_gen_string (gen, container_ਸਥਿਤੀ, ਲੜੀ_ਲੰਬਾਈ (container_ਸਥਿਤੀ));
      json_gen_string (gen, "bundle", ਲੜੀ_ਲੰਬਾਈ ("bundle"));
      json_gen_string (gen, ਸਥਿਤੀ.bundle, ਲੜੀ_ਲੰਬਾਈ (ਸਥਿਤੀ.bundle));
      json_gen_string (gen, "created", ਲੜੀ_ਲੰਬਾਈ ("created"));
      json_gen_string (gen, ਸਥਿਤੀ.created, ਲੜੀ_ਲੰਬਾਈ (ਸਥਿਤੀ.created));
      json_gen_string (gen, "owner", ਲੜੀ_ਲੰਬਾਈ ("owner"));
      json_gen_string (gen, ਸਥਿਤੀ.owner, ਸੁਰੱਖਿਅਤ_strlen (ਸਥਿਤੀ.owner));
      json_gen_map_close (gen);

      libcrun_free_container_ਸਥਿਤੀ (&ਸਥਿਤੀ);
    }

  json_gen_array_close (gen);
  ਜੇ (json_gen_get_buf (gen, &content, &len) != json_gen_ਸਥਿਤੀ_ok)
    {
      ret = libcrun_make_error (err, 0, "cannot generate json list");
      ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;
    }

  *ਬਾਹਰਲਾ = xstrdup (content);
  ret = 0;

ਬਾਹਰ_ਨਿਕਲੋ:
  ਜੇ (list)
    libcrun_free_containers_list (list);
  ਜੇ (gen)
    json_gen_free (gen);

  ਵਾਪਸ ret;
}

ਪੂਰਨ
libcrun_write_json_containers_list (libcrun_context_t *context, FILE *ਬਾਹਰਲਾ, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *content = ਨਿਰਾ;
  size_t len;
  ਪੂਰਨ ret;

  ret = libcrun_container_list_json (context, &content, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  len = ਲੜੀ_ਲੰਬਾਈ (content);
  ਜੇ (len && fwrite (content, 1, len, ਬਾਹਰਲਾ) != len)
    ਵਾਪਸ crun_make_error (err, errno, "error writing to file");

  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_container_update_intel_rdt (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, ਬਣਤਰ libcrun_intel_rdt_update *update, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = ਨਿਰਾ;
  ਪੂਰਨ ret;

  ret = read_container_config_from_state (&container, context->state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ libcrun_update_intel_rdt (id, container, update->l3_cache_schema, update->mem_bw_schema, update->schemata, err);
}

ਸਥਿਰ ਪੂਰਨ
libcrun_container_ਜੋੜੋ_or_remove_mounts_from_file (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *file, bool ਜੋੜੋ, libcrun_error_t *err)
{
  cleanup_custom_handler_instance ਬਣਤਰ custom_handler_instance_s *custom_handler = ਨਿਰਾ;
  cleanup_container libcrun_container_t *container = ਨਿਰਾ;
  cleanup_free runtime_spec_schema_defs_mount **mounts = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ = context->state_ਜੜ੍ਹ;
  cleanup_free parser_error parser_err = ਨਿਰਾ;
  ਬਣਤਰ parser_context pctx = { 0, stderr };
  libcrun_container_ਸਥਿਤੀ_t ਸਥਿਤੀ = {};
  cleanup_free ਅੱਖਰ *content = ਨਿਰਾ;
  json_object *doc = ਨਿਰਾ;
  size_t n_mounts = 0, len, i;
  ਪੂਰਨ ret = 1;

  ret = read_all_file (file, &content, &len, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_read_container_ਸਥਿਤੀ (&ਸਥਿਤੀ, state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = read_container_config_from_state (&container, state_ਜੜ੍ਹ, id, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = parse_json_file (&doc, content, &pctx, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (! json_object_is_type (doc, json_type_array))
    {
      ret = crun_make_error (err, 0, "mounts must be an array");
      ਜਾਓ cleanup;
    }
  ਨਹੀਂਤਾਂ
    {
      n_mounts = json_object_array_ਲੰਮਾਈ (doc);

      mounts = xmalloc0 ((n_mounts + 1) * ਆਕਾਰ (*mounts));
      ਲਈ (i = 0; i < n_mounts; i++)
        {
          mounts[i] = make_runtime_spec_schema_defs_mount (json_object_array_get_idx (doc, i), &pctx, &parser_err);
          ਜੇ (mounts[i] == ਨਿਰਾ)
            {
              ret = crun_make_error (err, 0, "cannot parse mount: %s", parser_err);
              ਜਾਓ cleanup;
            }
        }
    }

  ਜੇ (ਜੋੜੋ)
    ret = libcrun_make_runtime_mounts (container, &ਸਥਿਤੀ, mounts, n_mounts, err);
  ਨਹੀਂਤਾਂ
    ret = libcrun_destroy_runtime_mounts (container, &ਸਥਿਤੀ, mounts, n_mounts, err);

cleanup:
  ਜੇ (doc)
    json_object_put (doc);
  ਜੇ (mounts)
    ਲਈ (i = 0; i < n_mounts; i++)
      free_runtime_spec_schema_defs_mount (mounts[i]);
  ਵਾਪਸ ret;
}

ਪੂਰਨ
libcrun_container_ਜੋੜੋ_mounts_from_file (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *file, libcrun_error_t *err)
{
  ਵਾਪਸ libcrun_container_ਜੋੜੋ_or_remove_mounts_from_file (context, id, file, true, err);
}

ਪੂਰਨ
libcrun_container_remove_mounts_from_file (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *file, libcrun_error_t *err)
{
  ਵਾਪਸ libcrun_container_ਜੋੜੋ_or_remove_mounts_from_file (context, id, file, false, err);
}
