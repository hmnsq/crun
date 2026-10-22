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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <ocispec/runtime_spec_schema_config_schema.h>
#അന്തര്ഭാവ <stdbool.h>
#അന്തര്ഭാവ "container.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "json_gen_utils.h"
#അന്തര്ഭാവ "seccomp.h"
#അന്തര്ഭാവ "mempolicy.h"
#ifdef HAVE_SECCOMP
#  include <seccomp.h>
#endif
#അന്തര്ഭാവ "scheduler.h"
#അന്തര്ഭാവ "seccomp_notify.h"
#അന്തര്ഭാവ "custom-handler.h"
#അന്തര്ഭാവ <stdbool.h>
#അന്തര്ഭാവ <argp.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <sched.h>
#അന്തര്ഭാവ <sys/wait.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ "status.h"
#അന്തര്ഭാവ "mount_flags.h"
#അന്തര്ഭാവ "linux.h"
#അന്തര്ഭാവ "terminal.h"
#അന്തര്ഭാവ "io_priority.h"
#അന്തര്ഭാവ "cgroup.h"
#അന്തര്ഭാവ "cgroup-utils.h"
#അന്തര്ഭാവ <sys/prctl.h>
#അന്തര്ഭാവ <sys/auxv.h>
#അന്തര്ഭാവ <sys/signalfd.h>
#അന്തര്ഭാവ <sys/epoll.h>
#അന്തര്ഭാവ <sys/socket.h>
#ifdef HAVE_CAP
#  include <sys/capability.h>
#endif
#അന്തര്ഭാവ <sys/ioctl.h>
#അന്തര്ഭാവ <termios.h>
#അന്തര്ഭാവ <grp.h>
#അന്തര്ഭാവ <git-version.h>

#ifdef HAVE_SYSTEMD
#  include <systemd/sd-daemon.h>
#endif

#അന്തര്ഭാവ <json-c/json.h>
#അന്തര്ഭാവ <ocispec/json_common.h>

എണ്ണൽ
{
  SYNC_SOCKET_SYNC_MESSAGE,
  SYNC_SOCKET_ERROR_MESSAGE,
  SYNC_SOCKET_WARNING_MESSAGE,
  SYNC_SOCKET_DEBUG_MESSAGE,
};

ഘടന container_entrypoint_s
{
  libcrun_container_t *container;
  libcrun_context_t *context;
  പൂർണ്ണസംഖ്യ has_terminal_socket_pair;
  പൂർണ്ണസംഖ്യ terminal_socketpair[2];

  /* Used by log_write_to_sync_socket.  */
  പൂർണ്ണസംഖ്യ sync_socket;

  പൂർണ്ണസംഖ്യ seccomp_fd;
  പൂർണ്ണസംഖ്യ seccomp_receiver_fd;
  പൂർണ്ണസംഖ്യ console_socket_fd;

  പൂർണ്ണസംഖ്യ hooks_out_fd;
  പൂർണ്ണസംഖ്യ hooks_err_fd;

  /* Absolute bundle path, to be reported to hooks run in the container.  */
  സ്ഥിരം അക്ഷരം *bundle;

  ഘടന custom_handler_instance_s *custom_handler;
};

ഘടന sync_socket_message_s
{
  പൂർണ്ണസംഖ്യ type;
  പൂർണ്ണസംഖ്യ error_മൂല്യം;
  അക്ഷരം message[512];
};

തരനിർവചനം runtime_spec_schema_defs_hook hook;

// linux hooks
അക്ഷരം *hooks[] = {
  "prestart",
  "createRuntime",
  "createContainer",
  "startContainer",
  "poststart",
  "poststop"
};

// linux namespaces
സ്ഥിതം അക്ഷരം *namespaces[] = {
  "cgroup",
  "ipc",
  "mount",
  "network",
  "pid",
  "user",
  "uts"
};

സ്ഥിതം അക്ഷരം *actions[] = {
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

സ്ഥിതം അക്ഷരം *operators[] = {
  "SCMP_CMP_NE",
  "SCMP_CMP_LT",
  "SCMP_CMP_LE",
  "SCMP_CMP_EQ",
  "SCMP_CMP_GE",
  "SCMP_CMP_GT",
  "SCMP_CMP_MASKED_EQ",
};

സ്ഥിതം അക്ഷരം *archs[] = {
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

സ്ഥിതം അക്ഷരം *mempolicy_modes[] = {
  "MPOL_DEFAULT",
  "MPOL_PREFERRED",
  "MPOL_BIND",
  "MPOL_INTERLEAVE",
  "MPOL_LOCAL",
  "MPOL_PREFERRED_MANY",
  "MPOL_WEIGHTED_INTERLEAVE"
};

സ്ഥിതം അക്ഷരം *mempolicy_flags[] = {
  "MPOL_F_NUMA_BALANCING",
  "MPOL_F_RELATIVE_NODES",
  "MPOL_F_STATIC_NODES"
};

സ്ഥിതം അക്ഷരം *potentially_unsafe_annotations[] = {
  "module.wasm.image/variant",
  "io.kubernetes.cri.container-type",
  "run.oci.",
  "org.criu.",
  "krun.",
};

#define SYNC_SOCKET_MESSAGE_LEN(x, l) (offsetof (ഘടന sync_socket_message_s, message) + l)

സ്ഥിതം പൂർണ്ണസംഖ്യ
sync_socket_write_സന്ദേശം (പൂർണ്ണസംഖ്യ fd, പൂർണ്ണസംഖ്യ verbosity, പൂർണ്ണസംഖ്യ err_മൂല്യം, സ്ഥിരം അക്ഷരം *log_സന്ദേശം)
{
  പൂർണ്ണസംഖ്യ ret;
  size_t err_len;
  ഘടന sync_socket_message_s സന്ദേശം;
  തിരഞ്ഞെടുപ്പ് (verbosity)
    {
    അവസ്ഥ LIBCRUN_VERBOSITY_DEBUG:
      സന്ദേശം.type = SYNC_SOCKET_DEBUG_MESSAGE;
      നിർത്തുക;
    അവസ്ഥ LIBCRUN_VERBOSITY_WARNING:
      സന്ദേശം.type = SYNC_SOCKET_WARNING_MESSAGE;
      നിർത്തുക;
    അവസ്ഥ LIBCRUN_VERBOSITY_ERROR:
      സന്ദേശം.type = SYNC_SOCKET_ERROR_MESSAGE;
      നിർത്തുക;
    }
  സന്ദേശം.error_മൂല്യം = err_മൂല്യം;

  എങ്കിൽ (fd < 0)
    തിരികെ 0;

  err_len = strlen (log_സന്ദേശം);
  എങ്കിൽ (err_len >= വലുപ്പം (സന്ദേശം.message))
    err_len = വലുപ്പം (സന്ദേശം.message) - 1;

  memcpy (സന്ദേശം.message, log_സന്ദേശം, err_len);
  സന്ദേശം.message[err_len] = '\0';

  ret = TEMP_FAILURE_RETRY (ലിഖ (fd, &സന്ദേശം, SYNC_SOCKET_MESSAGE_LEN (സന്ദേശം, err_len + 1)));
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ -1;

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
sync_socket_write_error (പൂർണ്ണസംഖ്യ fd, libcrun_error_t *out_err)
{
  എങ്കിൽ (fd < 0)
    തിരികെ 0;
  തിരികെ sync_socket_write_സന്ദേശം (fd, false, (*out_err)->status, (*out_err)->സന്ദേശം);
}

സ്ഥിതം ശൂന്യം
log_write_to_sync_socket (പൂർണ്ണസംഖ്യ errno_, സ്ഥിരം അക്ഷരം *സന്ദേശം, പൂർണ്ണസംഖ്യ verbosity, ശൂന്യം *arg)
{
  ഘടന container_entrypoint_s *entrypoint_args = arg;
  പൂർണ്ണസംഖ്യ fd = entrypoint_args->sync_socket;

  എങ്കിൽ (fd < 0)
    തിരികെ;

  എങ്കിൽ (sync_socket_write_സന്ദേശം (fd, verbosity, errno_, സന്ദേശം) < 0)
    log_write_to_stderr (errno_, സന്ദേശം, verbosity, arg);
}

സ്ഥിതം bool
is_memory_limit_too_താഴെയുള്ള (runtime_spec_schema_config_schema *def)
{
  സ്ഥിരം long memory_limit_too_താഴെയുള്ള = 1024 * 1024;

  എങ്കിൽ (def->linux == ശൂന്യ || def->linux->resources == ശൂന്യ)
    തിരികെ false;

  എങ്കിൽ (def->linux->resources->memory
      && def->linux->resources->memory->limit_present
      && def->linux->resources->memory->limit < memory_limit_too_താഴെയുള്ള)
    തിരികെ true;

  എങ്കിൽ (def->linux->resources->unified)
    {
      size_t i;

      വേണ്ടി (i = 0; i < def->linux->resources->unified->len; i++)
        എങ്കിൽ (strcmp (def->linux->resources->unified->താക്കോലുകൾ[i], "memory.max") == 0)
          {
            long limit;

            errno = 0;
            limit = strtol (def->linux->resources->unified->മൂല്യങ്ങൾ[i], ശൂന്യ, 10);
            എങ്കിൽ (errno != 0)
              തിരികെ false;
            എങ്കിൽ (limit < memory_limit_too_താഴെയുള്ള)
              തിരികെ true;
          }
    }

  തിരികെ false;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
sync_socket_wait_sync (libcrun_context_t *context, പൂർണ്ണസംഖ്യ fd, bool flush, libcrun_error_t *err)
{
  ഘടന sync_socket_message_s സന്ദേശം;

  എങ്കിൽ (fd < 0)
    തിരികെ 0;

  ആകുമ്പോൾ (true)
    {
      പൂർണ്ണസംഖ്യ ret;

      ret = TEMP_FAILURE_RETRY (പഠ (fd, &സന്ദേശം, വലുപ്പം (സന്ദേശം)));
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          എങ്കിൽ (flush)
            തിരികെ 0;
          തിരികെ crun_make_error (err, errno, "read from sync socket");
        }

      എങ്കിൽ (ret == 0)
        {
          എങ്കിൽ (flush)
            തിരികെ 0;

          തിരികെ crun_make_error (err, 0, "read from the init process");
        }

      എങ്കിൽ (! flush && സന്ദേശം.type == SYNC_SOCKET_SYNC_MESSAGE)
        തിരികെ 0;
      അല്ലെങ്കിൽ എങ്കിൽ (സന്ദേശം.type == SYNC_SOCKET_DEBUG_MESSAGE)
        {
          എങ്കിൽ (context)
            context->output_handler (സന്ദേശം.error_മൂല്യം, സന്ദേശം.message, LIBCRUN_VERBOSITY_DEBUG, context->output_handler_arg);
          തുടരുക;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (സന്ദേശം.type == SYNC_SOCKET_WARNING_MESSAGE)
        {
          എങ്കിൽ (context)
            context->output_handler (സന്ദേശം.error_മൂല്യം, സന്ദേശം.message, LIBCRUN_VERBOSITY_WARNING, context->output_handler_arg);
          തുടരുക;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (സന്ദേശം.type == SYNC_SOCKET_ERROR_MESSAGE)
        തിരികെ crun_make_error (err, സന്ദേശം.error_മൂല്യം, "%s", സന്ദേശം.message);
    }
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
sync_socket_send_sync (പൂർണ്ണസംഖ്യ fd, bool flush_errors, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  ഘടന sync_socket_message_s സന്ദേശം = {
    0,
  };
  സന്ദേശം.type = SYNC_SOCKET_SYNC_MESSAGE;

  എങ്കിൽ (fd < 0)
    തിരികെ 0;

  ret = TEMP_FAILURE_RETRY (ലിഖ (fd, &സന്ദേശം, SYNC_SOCKET_MESSAGE_LEN (സന്ദേശം, 0)));
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      എങ്കിൽ (flush_errors)
        {
          പൂർണ്ണസംഖ്യ saved_errno = errno;
          ret = TEMP_FAILURE_RETRY (പഠ (fd, &സന്ദേശം, വലുപ്പം (സന്ദേശം)));
          എങ്കിൽ (ret >= 0 && സന്ദേശം.type == SYNC_SOCKET_ERROR_MESSAGE)
            തിരികെ crun_make_error (err, സന്ദേശം.error_മൂല്യം, "%s", സന്ദേശം.message);
          errno = saved_errno;
        }
      തിരികെ crun_make_error (err, errno, "write to sync socket");
    }

  തിരികെ 0;
}

സ്ഥിതം bool
has_cap_sys_ptrace ()
{
#ifdef HAVE_CAP
  ഘടന __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  ഘടന __user_cap_data_struct data[2] = { { 0 } };

  എങ്കിൽ (UNLIKELY (capget (&hdr, data) < 0))
    തിരികെ true;

  തിരികെ (data[0].effective & (1U << CAP_SYS_PTRACE)) != 0;
#അല്ലെങ്കിൽ
  തിരികെ geteuid () == 0;
#endif
}

സ്ഥിതം libcrun_container_t *
make_container (runtime_spec_schema_config_schema *container_def, സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *config)
{
  libcrun_container_t *container = xmalloc0 (വലുപ്പം (*container));
  container->container_def = container_def;

  container->host_uid = geteuid ();
  container->host_gid = getegid ();
  container->host_has_cap_sys_ptrace = has_cap_sys_ptrace ();
  container->proc_fd = -1;

  container->annotations = make_string_map_from_json (container_def->annotations);

  /* An empty namespace path means that a new namespace is to be created,
     the same as if it is not specified.  */
  എങ്കിൽ (container_def->linux)
    {
      size_t i;

      വേണ്ടി (i = 0; i < container_def->linux->namespaces_len; i++)
        എങ്കിൽ (container_def->linux->namespaces[i]->path && container_def->linux->namespaces[i]->path[0] == '\0')
          {
            സ്വതന്ത്രം (container_def->linux->namespaces[i]->path);
            container_def->linux->namespaces[i]->path = ശൂന്യ;
          }
    }

  എങ്കിൽ (path)
    container->config_file = xstrdup (path);
  എങ്കിൽ (config)
    container->config_file_content = xstrdup (config);

  തിരികെ container;
}

libcrun_container_t *
libcrun_container_load_from_memory (സ്ഥിരം അക്ഷരം *json, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free അക്ഷരം *oci_error = ശൂന്യ;
  container_def = runtime_spec_schema_config_schema_parse_data (json, ശൂന്യ, &oci_error);
  എങ്കിൽ (container_def == ശൂന്യ)
    {
      crun_make_error (err, 0, "load: `%s`", oci_error);
      തിരികെ ശൂന്യ;
    }
  തിരികെ make_container (container_def, ശൂന്യ, json);
}

libcrun_container_t *
libcrun_container_load_from_file (സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free അക്ഷരം *oci_error = ശൂന്യ;
  cleanup_free അക്ഷരം *content = ശൂന്യ;
  size_t len;
  പൂർണ്ണസംഖ്യ ret;

  libcrun_debug ("Loading container from config file: `%s`", path);
  ret = read_all_file (path, &content, &len, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ശൂന്യ;

  container_def = runtime_spec_schema_config_schema_parse_data (content, ശൂന്യ, &oci_error);
  എങ്കിൽ (container_def == ശൂന്യ)
    {
      crun_make_error (err, 0, "load `%s`: %s", path, oci_error);
      തിരികെ ശൂന്യ;
    }
  തിരികെ make_container (container_def, ശൂന്യ, content);
}

സ്ഥിരം അക്ഷരം *
libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err)
{
  ഘടന parser_context ctx = { 0, stderr };
  parser_error gen_err = ശൂന്യ;

  എങ്കിൽ (container == ശൂന്യ)
    {
      crun_make_error (err, EINVAL, "no container specified");
      തിരികെ ശൂന്യ;
    }

  എങ്കിൽ (container->config_file_content == ശൂന്യ && container->container_def)
    {
      container->config_file_content = runtime_spec_schema_config_schema_generate_json (container->container_def, &ctx, &gen_err);
      എങ്കിൽ (container->config_file_content == ശൂന്യ)
        {
          crun_make_error (err, 0, "cannot generate the configuration JSON: %s", gen_err ?: "unknown error");
          സ്വതന്ത്രം (gen_err);
          തിരികെ ശൂന്യ;
        }
      സ്വതന്ത്രം (gen_err);
    }

  എങ്കിൽ (container->config_file_content == ശൂന്യ)
    {
      crun_make_error (err, 0, "the container has no configuration");
      തിരികെ ശൂന്യ;
    }

  തിരികെ container->config_file_content;
}

സ്ഥിരം അക്ഷരം *
libcrun_container_get_annotation (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *താക്കോൽ)
{
  എങ്കിൽ (container == ശൂന്യ)
    തിരികെ ശൂന്യ;
  തിരികെ find_string_map_മൂല്യം (container->annotations, താക്കോൽ);
}

size_t
libcrun_container_get_annotations_len (libcrun_container_t *container)
{
  എങ്കിൽ (container == ശൂന്യ || container->annotations == ശൂന്യ)
    തിരികെ 0;
  തിരികെ string_map_size (container->annotations);
}

പൂർണ്ണസംഖ്യ
libcrun_container_get_annotation_at (libcrun_container_t *container, size_t index, സ്ഥിരം അക്ഷരം **താക്കോൽ,
                                     സ്ഥിരം അക്ഷരം **മൂല്യം)
{
  എങ്കിൽ (container == ശൂന്യ)
    {
      errno = ERANGE;
      തിരികെ -1;
    }
  തിരികെ string_map_get_at (container->annotations, index, താക്കോൽ, മൂല്യം);
}

uid_t
libcrun_container_get_uid (libcrun_container_t *container)
{
  എങ്കിൽ (container == ശൂന്യ)
    തിരികെ (uid_t) -1;
  തിരികെ container->host_uid;
}

gid_t
libcrun_container_get_gid (libcrun_container_t *container)
{
  എങ്കിൽ (container == ശൂന്യ)
    തിരികെ (gid_t) -1;
  തിരികെ container->host_gid;
}

ശൂന്യം
libcrun_container_free (libcrun_container_t *ctr)
{
  എങ്കിൽ (ctr == ശൂന്യ)
    തിരികെ;

  എങ്കിൽ (ctr->cleanup_private_data)
    ctr->cleanup_private_data (ctr->private_data);

  എങ്കിൽ (ctr->container_def)
    free_runtime_spec_schema_config_schema (ctr->container_def);

  free_string_map (ctr->annotations);

  എങ്കിൽ (ctr->proc_fd >= 0)
    close (ctr->proc_fd);

  സ്വതന്ത്രം (ctr->config_file_content);
  സ്വതന്ത്രം (ctr->config_file);
  സ്വതന്ത്രം (ctr);
}

libcrun_context_t *
libcrun_context_new (സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *state_വേര്, libcrun_error_t *err)
{
  libcrun_context_t *ctx = xmalloc0 (വലുപ്പം (*ctx));

  /* Go through the setters so that the owned_* copies and the borrowed
     pointers stay in sync and are released by libcrun_context_free.  */
  libcrun_context_set_id (ctx, id);
  libcrun_context_set_state_വേര് (ctx, state_വേര്);
  libcrun_context_set_bundle (ctx, ".");
  ctx->fifo_exec_wait_fd = -1;

  എങ്കിൽ (libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, id, ശൂന്യ, err) < 0)
    {
      libcrun_context_free (ctx);
      തിരികെ ശൂന്യ;
    }

  ctx->handler_manager = libcrun_handler_manager_create (err);
  എങ്കിൽ (ctx->handler_manager == ശൂന്യ)
    {
      libcrun_context_free (ctx);
      തിരികെ ശൂന്യ;
    }

#ifdef CRUN_LIBDIR
  എങ്കിൽ (access (CRUN_LIBDIR "/handlers", F_OK) == 0)
    {
      എങ്കിൽ (libcrun_handler_manager_load_directory (ctx->handler_manager, CRUN_LIBDIR "/handlers", err) < 0)
        {
          libcrun_context_free (ctx);
          തിരികെ ശൂന്യ;
        }
    }
#endif

  തിരികെ ctx;
}

ശൂന്യം
libcrun_context_free (libcrun_context_t *ctx)
{
  എങ്കിൽ (ctx == ശൂന്യ)
    തിരികെ;

  സ്വതന്ത്രം (ctx->owned_state_വേര്);
  സ്വതന്ത്രം (ctx->owned_id);
  സ്വതന്ത്രം (ctx->owned_bundle);
  സ്വതന്ത്രം (ctx->owned_console_socket);
  സ്വതന്ത്രം (ctx->owned_pid_file);
  സ്വതന്ത്രം (ctx->owned_notify_socket);
  സ്വതന്ത്രം (ctx->owned_handler);

  എങ്കിൽ (ctx->handler_manager)
    handler_manager_free (ctx->handler_manager);

  സ്വതന്ത്രം (ctx);
}

#define LIBCRUN_CONTEXT_SET_STRING(field)                  \
  ചെയ്യുക                                                       \
    {                                                      \
      സ്വതന്ത്രം (ctx->owned_##field);                           \
      ctx->owned_##field = മൂല്യം ? xstrdup (മൂല്യം) : ശൂന്യ; \
      ctx->field = ctx->owned_##field;                     \
  } ആകുമ്പോൾ (0)

ശൂന്യം
libcrun_context_set_id (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *മൂല്യം)
{
  LIBCRUN_CONTEXT_SET_STRING (id);
}

ശൂന്യം
libcrun_context_set_state_വേര് (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *മൂല്യം)
{
  LIBCRUN_CONTEXT_SET_STRING (state_വേര്);
}

ശൂന്യം
libcrun_context_set_bundle (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *മൂല്യം)
{
  LIBCRUN_CONTEXT_SET_STRING (bundle);
}

ശൂന്യം
libcrun_context_set_console_socket (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *മൂല്യം)
{
  LIBCRUN_CONTEXT_SET_STRING (console_socket);
}

ശൂന്യം
libcrun_context_set_pid_file (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *മൂല്യം)
{
  LIBCRUN_CONTEXT_SET_STRING (pid_file);
}

ശൂന്യം
libcrun_context_set_notify_socket (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *മൂല്യം)
{
  LIBCRUN_CONTEXT_SET_STRING (notify_socket);
}

ശൂന്യം
libcrun_context_set_handler (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *മൂല്യം)
{
  LIBCRUN_CONTEXT_SET_STRING (handler);
}

#undef LIBCRUN_CONTEXT_SET_STRING

സ്ഥിരം അക്ഷരം *
libcrun_context_get_pid_file (libcrun_context_t *ctx)
{
  തിരികെ ctx->pid_file;
}

സ്ഥിരം അക്ഷരം *
libcrun_context_get_state_വേര് (libcrun_context_t *ctx)
{
  എങ്കിൽ (ctx == ശൂന്യ)
    തിരികെ ശൂന്യ;
  തിരികെ ctx->state_വേര്;
}

ശൂന്യം
libcrun_context_set_preserve_fds (libcrun_context_t *ctx, പൂർണ്ണസംഖ്യ preserve_fds)
{
  ctx->preserve_fds = preserve_fds;
}

പൂർണ്ണസംഖ്യ
libcrun_context_get_preserve_fds (libcrun_context_t *ctx)
{
  തിരികെ ctx->preserve_fds;
}

ശൂന്യം
libcrun_context_set_listen_fds (libcrun_context_t *ctx, പൂർണ്ണസംഖ്യ listen_fds)
{
  ctx->listen_fds = listen_fds;
}

ശൂന്യം
libcrun_context_set_args (libcrun_context_t *ctx, പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  ctx->argc = argc;
  ctx->argv = argv;
}

ശൂന്യം
libcrun_context_set_systemd_cgroup (libcrun_context_t *ctx, bool മൂല്യം)
{
  ctx->systemd_cgroup = മൂല്യം;
}

ശൂന്യം
libcrun_context_set_detach (libcrun_context_t *ctx, bool മൂല്യം)
{
  ctx->detach = മൂല്യം;
}

ശൂന്യം
libcrun_context_set_no_new_keyring (libcrun_context_t *ctx, bool മൂല്യം)
{
  ctx->no_new_keyring = മൂല്യം;
}

ശൂന്യം
libcrun_context_set_no_pivot (libcrun_context_t *ctx, bool മൂല്യം)
{
  ctx->no_pivot = മൂല്യം;
}

ശൂന്യം
libcrun_context_set_force_no_cgroup (libcrun_context_t *ctx, bool മൂല്യം)
{
  ctx->force_no_cgroup = മൂല്യം;
}

ശൂന്യം
libcrun_context_set_output_handler (libcrun_context_t *ctx, crun_output_handler handler, ശൂന്യം *arg)
{
  ctx->output_handler = handler;
  ctx->output_handler_arg = arg;
}

പൂർണ്ണസംഖ്യ
libcrun_context_init_logging (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *log, libcrun_error_t *err)
{
  തിരികെ libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, ctx->id, log, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
block_signals (libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  sigset_t mask;
  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "sigprocmask");
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
unblock_signals (libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ i;
  പൂർണ്ണസംഖ്യ ret;
  sigset_t mask;
  ഘടന sigaction act = {};

  sigfillset (&mask);
  ret = sigprocmask (SIG_UNBLOCK, &mask, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "sigprocmask");

  act.sa_handler = SIG_DFL;
  വേണ്ടി (i = 0; i < NSIG; i++)
    {
      ret = sigaction (i, &act, ശൂന്യ);
      എങ്കിൽ (ret < 0 && errno != EINVAL)
        തിരികെ crun_make_error (err, errno, "sigaction");
    }

  തിരികെ 0;
}

/* must be used on the host before pivot_root(2).  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
initialize_security (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  ret = libcrun_init_caps (container, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (UNLIKELY (proc == ശൂന്യ))
    തിരികെ 0;

  എങ്കിൽ (proc->apparmor_profile)
    {
      ret = libcrun_initialize_apparmor (err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  ret = libcrun_initialize_selinux (container, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ 0;
}

/* Emit the "annotations" map.  Returns a json_gen_status; callers can wrap
   the call with GEN_OR_FAIL.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
gen_annotations (json_gen_ctx *gen, json_map_string_string *annotations)
{
  size_t i;
  പൂർണ്ണസംഖ്യ r;

  GEN_താക്കോൽ (gen, "annotations");
  GEN_OR_FAIL (json_gen_map_open (gen));

  വേണ്ടി (i = 0; i < annotations->len; i++)
    {
      /* An annotation whose value is null in the document is parsed into a
         NULL, which cannot be emitted as a JSON string; leave it out.  */
      എങ്കിൽ (annotations->താക്കോലുകൾ[i] == ശൂന്യ || annotations->മൂല്യങ്ങൾ[i] == ശൂന്യ)
        തുടരുക;

      GEN_STR (gen, annotations->താക്കോലുകൾ[i]);
      GEN_STR (gen, annotations->മൂല്യങ്ങൾ[i]);
    }

  തിരികെ json_gen_map_close (gen);

gen_error:
  തിരികെ r;
}

/* Return the bundle path to report in the container state.  crun always
   chdir()s into the bundle, so fall back to getcwd() when the caller has no
   absolute bundle path, as can happen when libcrun is used as a library.
   *ALLOCATED must be freed by the caller.  */
സ്ഥിതം സ്ഥിരം അക്ഷരം *
get_bundle_path (സ്ഥിരം അക്ഷരം *bundle, അക്ഷരം **allocated)
{
  എങ്കിൽ (bundle != ശൂന്യ && bundle[0] == '/')
    തിരികെ bundle;

  *allocated = getcwd (ശൂന്യ, 0);
  എങ്കിൽ (*allocated == ശൂന്യ)
    OOM ();
  തിരികെ *allocated;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
do_hooks (runtime_spec_schema_config_schema *def, pid_t pid, സ്ഥിരം അക്ഷരം *id, bool keep_going, സ്ഥിരം അക്ഷരം *bundle,
          സ്ഥിരം അക്ഷരം *cwd, സ്ഥിരം അക്ഷരം *status, സ്ഥിരം അക്ഷരം *name, hook **hooks, size_t hooks_len, പൂർണ്ണസംഖ്യ out_fd,
          പൂർണ്ണസംഖ്യ err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  size_t i, stdin_len;
  പൂർണ്ണസംഖ്യ r, ret;
  അക്ഷരം *stdin = ശൂന്യ;
  cleanup_free അക്ഷരം *bundle_allocated = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *rootfs = def->വേര് ? def->വേര്->path : "";
  cleanup_json_gen json_gen_ctx *gen = ശൂന്യ;

  bundle = get_bundle_path (bundle, &bundle_allocated);

  എങ്കിൽ (! json_gen_init (&gen, ശൂന്യ))
    തിരികെ crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 0);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_താക്കോൽ (gen, "ociVersion");
  GEN_STR (gen, "1.0");

  GEN_താക്കോൽ (gen, "id");
  GEN_STR (gen, id);

  GEN_താക്കോൽ (gen, "pid");
  GEN_OR_FAIL (map_int (gen, pid));

  GEN_താക്കോൽ (gen, "root");
  GEN_STR (gen, rootfs);

  GEN_താക്കോൽ (gen, "bundle");
  GEN_STR (gen, bundle);

  GEN_താക്കോൽ (gen, "status");
  GEN_STR (gen, status);

  എങ്കിൽ (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, (സ്ഥിരം അക്ഷരം **) &stdin, &stdin_len));

  ret = 0;

  bool error_created = false;
  വേണ്ടി (i = 0; i < hooks_len; i++)
    {
      അക്ഷരം **env = environ;

      എങ്കിൽ (hooks[i]->env)
        env = hooks[i]->env;

      /* Release the error from the previous iteration, if any.  */
      എങ്കിൽ (error_created)
        {
          crun_error_release (err);
          error_created = false;
        }

      ret = run_process_with_stdin_timeout_envp (hooks[i]->path, hooks[i]->args, cwd, hooks[i]->timeout, env,
                                                 stdin, stdin_len, out_fd, err_fd, can_ignore_chdir_errors, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        error_created = true;

      എങ്കിൽ (UNLIKELY (ret != 0))
        {
          എങ്കിൽ (keep_going)
            libcrun_warning ("error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
          അല്ലെങ്കിൽ
            {
              libcrun_error (0, "error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
              നിർത്തുക;
            }
        }
    }

  തിരികെ ret;

gen_error:
  തിരികെ json_gen_error_to_crun_error (r, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
get_json_gen_result (json_gen_ctx *gen, അക്ഷരം **out, size_t *out_len)
{
  സ്ഥിരം അക്ഷരം *buf = ശൂന്യ;
  size_t buf_len = 0;
  പൂർണ്ണസംഖ്യ r;

  r = json_gen_get_buf (gen, &buf, &buf_len);
  എങ്കിൽ (UNLIKELY (r != json_gen_status_ok))
    തിരികെ r;

  *out_len = buf_len;

  *out = xmalloc (buf_len + 1);
  memcpy (*out, buf, buf_len);
  (*out)[buf_len] = '\0';

  തിരികെ json_gen_status_ok;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
get_seccomp_receiver_fd_payload (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *status, pid_t own_pid,
                                 അക്ഷരം **seccomp_fd_payload, size_t *seccomp_fd_payload_len, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ r;
  json_gen_ctx *gen = ശൂന്യ;
  runtime_spec_schema_config_schema *def = container->container_def;
  സ്ഥിരം അക്ഷരം *സ്ഥിരം OCI_VERSION = "0.2.0";

  എങ്കിൽ (! json_gen_init (&gen, ശൂന്യ))
    തിരികെ crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_താക്കോൽ (gen, "ociVersion");
  GEN_STR (gen, OCI_VERSION);

  GEN_താക്കോൽ (gen, "fds");
  GEN_OR_FAIL (json_gen_array_open (gen));

  GEN_താക്കോൽ (gen, "seccompFd");
  GEN_OR_FAIL (json_gen_array_close (gen));

  GEN_താക്കോൽ (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  എങ്കിൽ (def && def->linux && def->linux->seccomp)
    {
      സ്ഥിരം അക്ഷരം *metadata = def->linux->seccomp->listener_metadata;

      എങ്കിൽ (metadata)
        {
          GEN_താക്കോൽ (gen, "metadata");
          GEN_STR (gen, metadata);
        }
    }

  /* State.  */
  GEN_താക്കോൽ (gen, "state");
  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_താക്കോൽ (gen, "ociVersion");
  GEN_STR (gen, OCI_VERSION);

  എങ്കിൽ (container->context && container->context->id)
    {
      GEN_താക്കോൽ (gen, "id");
      GEN_STR (gen, container->context->id);
    }

  GEN_താക്കോൽ (gen, "status");
  GEN_STR (gen, status);

  GEN_താക്കോൽ (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  എങ്കിൽ (container->context && container->context->bundle)
    {
      GEN_താക്കോൽ (gen, "bundle");
      GEN_STR (gen, container->context->bundle);
    }

  എങ്കിൽ (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));
  /* End state.  */

  GEN_OR_FAIL (json_gen_map_close (gen));

  r = get_json_gen_result (gen, seccomp_fd_payload, seccomp_fd_payload_len);

gen_error:
  json_gen_free (gen);

  തിരികെ json_gen_error_to_crun_error (r, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
send_sync_cb (ശൂന്യം *data, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ sync_socket_fd = *((പൂർണ്ണസംഖ്യ *) data);
  പൂർണ്ണസംഖ്യ ret;

  /* sync 2.  */
  ret = sync_socket_send_sync (sync_socket_fd, false, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  /* sync 3.  */
  തിരികെ sync_socket_wait_sync (ശൂന്യ, sync_socket_fd, false, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
maybe_chown_std_streams (uid_t container_uid, gid_t container_gid, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret, i;

  വേണ്ടി (i = 0; i < 3; i++)
    {
      എങ്കിൽ (! isatty (i))
        {
          ഘടന stat statbuf;
          ret = fstat (i, &statbuf);
          എങ്കിൽ (UNLIKELY (ret < 0))
            {
              എങ്കിൽ (errno == EBADF)
                തുടരുക;
              തിരികെ crun_make_error (err, errno, "fstat fd `%d`", i);
            }

          /* Skip chown for device files */
          എങ്കിൽ (S_ISCHR (statbuf.st_mode) || S_ISBLK (statbuf.st_mode))
            തുടരുക;

          ret = fchown (i, container_uid, container_gid);
          എങ്കിൽ (UNLIKELY (ret < 0))
            {
              /* EINVAL means the user is not mapped in the current userns.
                 Ignore EPERM and EROFS as well as there is no reason to fail
                 so early, and let the container payload deal with it.
                 EBADF means fd is closed.
              */
              എങ്കിൽ (errno == EINVAL || errno == EPERM || errno == EROFS || errno == EBADF)
                തുടരുക;

              തിരികെ crun_make_error (err, errno, "fchown std stream `%i`", i);
            }
        }
    }
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_container_notify_handler (ഘടന container_entrypoint_s *args,
                                  എണ്ണൽ handler_configure_phase phase,
                                  libcrun_container_t *container, സ്ഥിരം അക്ഷരം *rootfs,
                                  libcrun_error_t *err)
{
  ഘടന custom_handler_s *h;

  എങ്കിൽ (args->custom_handler == ശൂന്യ)
    തിരികെ 0;

  h = args->custom_handler->vtable;
  എങ്കിൽ (h == ശൂന്യ || h->configure_container == ശൂന്യ)
    തിരികെ 0;

  തിരികെ h->configure_container (args->custom_handler->cookie, phase,
                                 args->context, container,
                                 rootfs, err);
}

/* Resolve and normalize the container rootfs path.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
resolve_rootfs_path (libcrun_container_t *container, അക്ഷരം **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  എങ്കിൽ (def->വേര് && def->വേര്->path)
    {
      *rootfs = realpath (def->വേര്->path, ശൂന്യ);
      എങ്കിൽ (UNLIKELY (*rootfs == ശൂന്യ))
        {
          പൂർണ്ണസംഖ്യ saved_errno = errno;

          /* If realpath failed for any reason, try the relative directory.  */
          എങ്കിൽ (def->വേര്->path[0] == '/')
            {
              cleanup_free അക്ഷരം *cwd = ശൂന്യ;
              libcrun_error_t tmp_err = ശൂന്യ;
              ssize_t len;
              പൂർണ്ണസംഖ്യ proc_fd;

              proc_fd = libcrun_get_cached_proc_fd (container, &tmp_err);
              എങ്കിൽ (LIKELY (proc_fd >= 0))
                {
                  len = safe_readlinkat (proc_fd, "self/cwd", &cwd, 0, &tmp_err);
                  എങ്കിൽ (LIKELY (len > 0))
                    {
                      /* If the rootfs is under the current working directory, just use its relative path.  */
                      എങ്കിൽ (has_dir_prefix (def->വേര്->path, cwd))
                        {
                          സ്ഥിരം അക്ഷരം *it = consume_slashes (def->വേര്->path + len);
                          എങ്കിൽ (LIKELY (*it))
                            {
                              പൂർണ്ണസംഖ്യ ret = faccessat (AT_FDCWD, it, F_OK, 0);
                              എങ്കിൽ (LIKELY (ret == 0))
                                *rootfs = xstrdup (it);
                            }
                        }
                    }
                }
              crun_error_release (&tmp_err);
            }

          എങ്കിൽ (*rootfs == ശൂന്യ)
            {
              എങ്കിൽ (saved_errno == ENOENT)
                തിരികെ crun_make_error (err, ENOENT, "the rootfs path `%s` does not exist", def->വേര്->path);

              *rootfs = xstrdup (def->വേര്->path);
            }
        }
    }
  തിരികെ 0;
}

/* Initialize the environment variables.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
setup_environment (runtime_spec_schema_config_schema *def, uid_t container_uid, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (clearenv ())
    തിരികെ crun_make_error (err, errno, "clearenv");

  എങ്കിൽ (def->process)
    {
      size_t i;

      വേണ്ടി (i = 0; i < def->process->env_len; i++)
        {
          /* A null element of the array of variables is parsed into a NULL,
             which putenv(3) cannot be handed.  */
          എങ്കിൽ (def->process->env[i] == ശൂന്യ)
            തിരികെ crun_make_error (err, EINVAL, "environment variable is not specified");

          എങ്കിൽ (putenv (def->process->env[i]) < 0)
            തിരികെ crun_make_error (err, errno, "putenv `%s`", def->process->env[i]);
        }
    }

  എങ്കിൽ (is_empty_string (getenv ("HOME")))
    {
      ret = set_home_env (container_uid);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          എങ്കിൽ (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            തിരികെ crun_make_error (err, errno, "setenv HOME");
        }
    }

  തിരികെ 0;
}

/* Configure terminal and console for interactive containers.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
setup_terminal (ഘടന container_entrypoint_s *entrypoint_args, libcrun_container_t *container,
                പൂർണ്ണസംഖ്യ console_socket, പൂർണ്ണസംഖ്യ console_socketpair, പൂർണ്ണസംഖ്യ has_terminal, libcrun_error_t *err)
{
  cleanup_close പൂർണ്ണസംഖ്യ terminal_fd = -1;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (! has_terminal)
    തിരികെ 0;

  fflush (stderr);

  എങ്കിൽ (console_socket >= 0 || (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0))
    {
      terminal_fd = libcrun_set_terminal (container, err);
      എങ്കിൽ (UNLIKELY (terminal_fd < 0))
        തിരികെ terminal_fd;
    }

  എങ്കിൽ (console_socket >= 0)
    {
      ret = send_fd_to_socket (console_socket, terminal_fd, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0)
    {
      ret = send_fd_to_socket (console_socketpair, terminal_fd, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  തിരികെ 0;
}

/* Resolve and validate the container's executable path.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
setup_executable_path (ഘടന container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                       അക്ഷരം **exec_path, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (def->process && def->process->args)
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          എങ്കിൽ (entrypoint_args->custom_handler == ശൂന്യ && crun_error_get_errno (err) == ENOENT)
            തിരികെ ret;

          /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
             once the process switched to the use that runs in the container.  This might be necessary
             when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
             is not honored.  */
          crun_error_release (err);
        }
    }

  തിരികെ 0;
}

/* Apply security settings for the container process.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
apply_security_settings (ഘടന container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                         libcrun_container_t *container, pid_t own_pid, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  runtime_spec_schema_config_schema_process_capabilities *capabilities;
  പൂർണ്ണസംഖ്യ no_new_privs;

  എങ്കിൽ (def->process && ! def->process->no_new_privileges)
    {
      അക്ഷരം **seccomp_flags = ശൂന്യ;
      size_t seccomp_flags_len = 0;
      cleanup_free അക്ഷരം *seccomp_fd_payload = ശൂന്യ;
      size_t seccomp_fd_payload_len = 0;

      എങ്കിൽ (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      എങ്കിൽ (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  capabilities = def->process ? def->process->capabilities : ശൂന്യ;
  no_new_privs = def->process ? def->process->no_new_privileges : 1;
  ret = libcrun_set_caps (capabilities, container->container_uid, container->container_gid, no_new_privs,
                          entrypoint_args->custom_handler != ശൂന്യ, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ 0;
}

/* Create the process that joins the existing PID namespace now that the
   host mounts are not visible anymore, and continue the setup from there.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
do_delayed_pidns_fork (പൂർണ്ണസംഖ്യ sync_socket, pid_t *own_pid, libcrun_error_t *err)
{
  pid_t pid;
  പൂർണ്ണസംഖ്യ ret;

  pid = fork ();
  എങ്കിൽ (UNLIKELY (pid < 0))
    തിരികെ crun_make_error (err, errno, "fork");

  എങ്കിൽ (pid)
    {
      /* Report the new PID to the parent and exit immediately.  */
      ret = sync_socket_send_sync (sync_socket, false, err);
      എങ്കിൽ (LIKELY (ret == 0))
        {
          ret = TEMP_FAILURE_RETRY (ലിഖ (sync_socket, &pid, വലുപ്പം (pid)));
          എങ്കിൽ (LIKELY (ret == വലുപ്പം (pid)))
            _safe_exit (EXIT_SUCCESS);
        }

      kill (pid, SIGKILL);
      _safe_exit (EXIT_FAILURE);
    }

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (പഠ (sync_socket, own_pid, വലുപ്പം (*own_pid)));
  എങ്കിൽ (UNLIKELY (ret != വലുപ്പം (*own_pid)))
    {
      എങ്കിൽ (ret >= 0)
        errno = 0;
      തിരികെ crun_make_error (err, errno, "read from sync socket");
    }

  തിരികെ 0;
}

ഘടന delayed_pidns_fork_s
{
  libcrun_container_t *container;
  പൂർണ്ണസംഖ്യ sync_socket;
  pid_t *own_pid;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ
delayed_pidns_fork_cb (ശൂന്യം *data, libcrun_error_t *err)
{
  ഘടന delayed_pidns_fork_s *d = data;

  എങ്കിൽ (! libcrun_container_delayed_pidns_fork (d->container))
    തിരികെ 0;

  തിരികെ do_delayed_pidns_fork (d->sync_socket, d->own_pid, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
container_init_setup (ശൂന്യം *args, pid_t *own_pid, അക്ഷരം *notify_socket,
                      പൂർണ്ണസംഖ്യ sync_socket, അക്ഷരം **exec_path, libcrun_error_t *err)
{
  ഘടന container_entrypoint_s *entrypoint_args = args;
  libcrun_container_t *container = entrypoint_args->container;
  bool chdir_done = false;
  പൂർണ്ണസംഖ്യ ret;
  പൂർണ്ണസംഖ്യ has_terminal;
  cleanup_close പൂർണ്ണസംഖ്യ console_socket = -1;
  cleanup_close പൂർണ്ണസംഖ്യ console_socketpair = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free അക്ഷരം *rootfs = ശൂന്യ;
  ഘടന delayed_pidns_fork_s delayed_pidns_fork = {
    .container = container,
    .sync_socket = sync_socket,
    .own_pid = own_pid,
  };

  ret = initialize_security (container, def->process, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_configure_network (container, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = resolve_rootfs_path (container, &rootfs, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  /* Configure terminal socket pair for container communication.  */
  എങ്കിൽ (entrypoint_args->terminal_socketpair[0] >= 0)
    {
      close_and_reset (&entrypoint_args->terminal_socketpair[0]);
      console_socketpair = entrypoint_args->terminal_socketpair[1];
    }

  /* sync 1.  */
  ret = sync_socket_wait_sync (ശൂന്യ, sync_socket, false, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  has_terminal = container->container_def->process && container->container_def->process->terminal;
  എങ്കിൽ (has_terminal && entrypoint_args->context->console_socket)
    console_socket = entrypoint_args->console_socket_fd;

  ret = libcrun_set_sysctl (container, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_BEFORE_MOUNTS, container, rootfs, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_do_pivot_വേര് (container, entrypoint_args->context->no_pivot, &rootfs, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  /* sync 2 and 3 are sent as part of libcrun_set_mounts.  */
  ret = libcrun_set_mounts (entrypoint_args, container, rootfs, send_sync_cb, &sync_socket, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  /* The createContainer hooks MUST run after the mounts are set up but before
     the pivot_root.  When these hooks are present can_use_open_tree_namespace()
     forces the deferred-pivot path, so the actual pivot happens later in
     libcrun_finalize_mounts().  */
  എങ്കിൽ (def->hooks && def->hooks->create_container_len)
    {
      libcrun_error_t tmp_err = ശൂന്യ;
      പൂർണ്ണസംഖ്യ in_userns = check_running_in_user_namespace (&tmp_err);
      എങ്കിൽ (tmp_err)
        crun_error_release (&tmp_err);
      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, ശൂന്യ, "created", "createContainer",
                      (hook **) def->hooks->create_container,
                      def->hooks->create_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      എങ്കിൽ (UNLIKELY (ret != 0))
        തിരികെ ret;
    }

  ret = libcrun_finalize_mounts (entrypoint_args, container, rootfs, delayed_pidns_fork_cb, &delayed_pidns_fork, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (def->process)
    {
      ret = libcrun_set_selinux_label (container, def->process, false, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = libcrun_set_apparmor_profile (container, def->process, false, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  ret = mark_or_close_fds_ge_than (container, entrypoint_args->context->preserve_fds + 3, false, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ret = libcrun_reopen_dev_null (err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = maybe_chown_std_streams (container->container_uid, container->container_gid, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = setup_environment (def, container->container_uid, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  /* Set primary process to 1 explicitly if nothing is configured and LISTEN_FD is not set.  */
  എങ്കിൽ (entrypoint_args->context->listen_fds > 0 && getenv ("LISTEN_PID") == ശൂന്യ)
    {
      libcrun_warning ("setting LISTEN_PID=1 since no previous configuration was found");
      എങ്കിൽ (UNLIKELY (setenv ("LISTEN_PID", "1", 1) < 0))
        തിരികെ crun_make_error (err, errno, "setenv LISTEN_PID");
    }

  /* Attempt to chdir immediately here, before doing the setresuid.  If we fail here, let's
     try again later once the process switched to the user that runs in the container.  */
  എങ്കിൽ (def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      എങ്കിൽ (LIKELY (ret == 0))
        chdir_done = true;
      അല്ലെങ്കിൽ
        crun_error_release (err);
    }

  ret = setsid ();
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "setsid");

  ret = setup_terminal (entrypoint_args, container, console_socket, console_socketpair, has_terminal, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  close_and_reset (&console_socket);
  close_and_reset (&console_socketpair);

  ret = setup_executable_path (entrypoint_args, def, exec_path, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_set_hostname (container, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_set_domainname (container, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (def->process && def->process->user)
    umask (def->process->user->umask_present ? def->process->user->umask : 0022);

  ret = apply_security_settings (entrypoint_args, def, container, *own_pid, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (UNLIKELY (def->process && def->process->args && *exec_path == ശൂന്യ))
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          എങ്കിൽ (entrypoint_args->custom_handler == ശൂന്യ || is_empty_string (def->process->args[0]))
            തിരികെ ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          *exec_path = xstrdup (def->process->args[0]);
        }
    }

  /* The chdir was not already performed, so try again now after switching to the UID/GID in the container.  */
  എങ്കിൽ (! chdir_done && def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (notify_socket && putenv (notify_socket) < 0)
    തിരികെ crun_make_error (err, errno, "putenv `%s`", notify_socket);

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
open_hooks_output (libcrun_container_t *container, പൂർണ്ണസംഖ്യ *out_fd, പൂർണ്ണസംഖ്യ *err_fd, libcrun_error_t *err)
{
  സ്ഥിരം അക്ഷരം *annotation;

  *err_fd = *out_fd = -1;

  libcrun_debug ("Opening hooks output");
  annotation = find_annotation (container, "run.oci.hooks.stdout");
  എങ്കിൽ (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stdout` annotation");
      *out_fd = TEMP_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      എങ്കിൽ (UNLIKELY (*out_fd < 0))
        തിരികെ crun_make_error (err, errno, "open `%s`", annotation);
    }

  annotation = find_annotation (container, "run.oci.hooks.stderr");
  എങ്കിൽ (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stderr` annotation");
      *err_fd = TEMP_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      എങ്കിൽ (UNLIKELY (*err_fd < 0))
        തിരികെ crun_make_error (err, errno, "open `%s`", annotation);
    }

  തിരികെ 0;
}

/* The environment the process was started with lives on the initial stack, right after the
   argv strings and before the string AT_EXECFN points to.  clearenv(3) only drops the pointers
   to it, so when there is no execv to replace the stack, erase it or the container process can
   still read it, e.g. through /proc/self/environ.  */
സ്ഥിതം ശൂന്യം
erase_initial_environment (അക്ഷരം **argv, പൂർണ്ണസംഖ്യ argc)
{
  അക്ഷരം *start = argv[argc - 1] + strlen (argv[argc - 1]) + 1;
  അക്ഷരം *end = (അക്ഷരം *) getauxval (AT_EXECFN);

  എങ്കിൽ (end > start)
    memset (start, 0, end - start);
}

സ്ഥിതം ശൂന്യം
rewrite_argv (അക്ഷരം **argv, പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം *name, അക്ഷരം **args, size_t args_len)
{
  cleanup_free അക്ഷരം *decorated_name = ശൂന്യ;
  size_t available_len = argv[argc - 1] - argv[0] + strlen (argv[argc - 1]) + 1;
  cleanup_free അക്ഷരം *new_argv = ശൂന്യ;
  size_t i, so_far = 0, needed = 0;

  needed = xasprintf (&decorated_name, "[libcrun:%s]", name) + 1;
  വേണ്ടി (i = 0; i < args_len; i++)
    needed += strlen (args[i]) + 1;

  new_argv = xmalloc0 (needed + 1);
  strcpy (new_argv, decorated_name);
  so_far = strlen (decorated_name) + 1;

  എങ്കിൽ (available_len >= needed)
    {
      വേണ്ടി (i = 0; i < args_len; i++)
        {
          strcpy (new_argv + so_far, args[i]);
          so_far += strlen (args[i]) + 1;
        }
    }

  എങ്കിൽ (so_far >= available_len)
    so_far = available_len - 1;

  memset (argv[0], 0, available_len);
  memcpy (argv[0], new_argv, so_far);
}

/* Entrypoint to the container.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
container_init (ശൂന്യം *args, അക്ഷരം *notify_socket, പൂർണ്ണസംഖ്യ sync_socket, libcrun_error_t *err)
{
  ഘടന container_entrypoint_s *entrypoint_args = args;
  പൂർണ്ണസംഖ്യ ret;
  runtime_spec_schema_config_schema *def = entrypoint_args->container->container_def;
  cleanup_free അക്ഷരം *exec_path = ശൂന്യ;
  __attribute__ ((unused)) cleanup_free അക്ഷരം *notify_socket_cleanup = notify_socket;
  pid_t own_pid = 0;

  entrypoint_args->sync_socket = sync_socket;

  crun_set_output_handler (log_write_to_sync_socket, args);

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (പഠ (sync_socket, &own_pid, വലുപ്പം (own_pid)));
  എങ്കിൽ (UNLIKELY (ret != വലുപ്പം (own_pid)))
    {
      എങ്കിൽ (ret >= 0)
        errno = 0;
      തിരികെ crun_make_error (err, errno, "read from sync socket");
    }

  ret = container_init_setup (args, &own_pid, notify_socket, sync_socket, &exec_path, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      /* If it fails to write the error using the sync socket, then fallback
         to stderr.  */
      എങ്കിൽ (sync_socket_write_error (sync_socket, err) == 0)
        {
          crun_error_release (err);
          /* A return value of > 0 means to fail with an error without err being set so to avoid
             printing it twice.  */
          തിരികെ 1;
        }
      തിരികെ ret;
    }

  entrypoint_args->sync_socket = -1;

  ret = unblock_signals (err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  /* sync 4.  */
  ret = sync_socket_send_sync (sync_socket, false, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  close_and_reset (&sync_socket);

  എങ്കിൽ (entrypoint_args->context->fifo_exec_wait_fd >= 0)
    {
      അക്ഷരം buffer[1];
      fd_set read_set;
      cleanup_close പൂർണ്ണസംഖ്യ fd = entrypoint_args->context->fifo_exec_wait_fd;
      entrypoint_args->context->fifo_exec_wait_fd = -1;

      FD_ZERO (&read_set);
      FD_SET (fd, &read_set);
      ചെയ്യുക
        {
          ret = select (fd + 1, &read_set, ശൂന്യ, ശൂന്യ, ശൂന്യ);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "select");

          ret = TEMP_FAILURE_RETRY (പഠ (fd, buffer, വലുപ്പം (buffer)));
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "read from the exec fifo");
      } ആകുമ്പോൾ (ret == 0);

      close_and_reset (&entrypoint_args->context->fifo_exec_wait_fd);
    }

  crun_set_output_handler (log_write_to_stderr, ശൂന്യ);

  എങ്കിൽ (def->process && def->process->no_new_privileges)
    {
      അക്ഷരം **seccomp_flags = ശൂന്യ;
      size_t seccomp_flags_len = 0;
      cleanup_free അക്ഷരം *seccomp_fd_payload = ശൂന്യ;
      size_t seccomp_fd_payload_len = 0;

      എങ്കിൽ (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      എങ്കിൽ (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (entrypoint_args->container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags,
                                   seccomp_flags_len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  എങ്കിൽ (UNLIKELY (def->process == ശൂന്യ))
    തിരികെ crun_make_error (err, 0, "block `process` not found");

  എങ്കിൽ (UNLIKELY (exec_path == ശൂന്യ))
    തിരികെ crun_make_error (err, 0, "executable path not specified");

  എങ്കിൽ (def->hooks && def->hooks->start_container_len)
    {
      libcrun_container_t *container = entrypoint_args->container;
      libcrun_error_t tmp_err = ശൂന്യ;
      പൂർണ്ണസംഖ്യ in_userns = check_running_in_user_namespace (&tmp_err);
      എങ്കിൽ (tmp_err)
        crun_error_release (&tmp_err);

      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, ശൂന്യ, "created", "startContainer",
                      (hook **) def->hooks->start_container,
                      def->hooks->start_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      എങ്കിൽ (UNLIKELY (ret != 0))
        തിരികെ ret;

      /* Seek stdout/stderr to the end.  If the hooks were using the same files,
         the container process overwrites what was previously written.  */
      (ശൂന്യം) lseek (1, 0, SEEK_END);
      (ശൂന്യം) lseek (2, 0, SEEK_END);
    }

  എങ്കിൽ (entrypoint_args->custom_handler)
    {
      /* Files marked with O_CLOEXEC are closed at execv time, so make sure they are closed now.
         This is a best effort operation, because the seccomp filter is already in place and it could
         stop some syscalls used by mark_or_close_fds_ge_than.
      */
      എങ്കിൽ (entrypoint_args->custom_handler->vtable->close_fds)
        {
          ret = entrypoint_args->custom_handler->vtable->close_fds (entrypoint_args->custom_handler->cookie,
                                                                    entrypoint_args->container,
                                                                    entrypoint_args->context->preserve_fds,
                                                                    err);
        }
      അല്ലെങ്കിൽ
        {
          ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
        }
      എങ്കിൽ (UNLIKELY (ret < 0))
        crun_error_release (err);

      prctl (PR_SET_NAME, entrypoint_args->custom_handler->vtable->name);

      എങ്കിൽ (entrypoint_args->context->argv)
        {
          erase_initial_environment (entrypoint_args->context->argv, entrypoint_args->context->argc);
          rewrite_argv (entrypoint_args->context->argv, entrypoint_args->context->argc,
                        entrypoint_args->custom_handler->vtable->name, def->process->args,
                        def->process->args_len);

          /* It is a quite destructive operation as we might be referencing data from the old
             argv, so make sure the context is not reused.  */
          entrypoint_args->context->argv = ശൂന്യ;
          entrypoint_args->context = ശൂന്യ;
        }

      ret = libcrun_set_selinux_label (entrypoint_args->container, def->process, true, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = libcrun_set_apparmor_profile (entrypoint_args->container, def->process, true, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = entrypoint_args->custom_handler->vtable->run_func (entrypoint_args->custom_handler->cookie,
                                                               entrypoint_args->container,
                                                               exec_path,
                                                               def->process->args);
      എങ്കിൽ (ret != 0)
        തിരികെ crun_make_error (err, 0, "exec container process failed with handler as `%s`", entrypoint_args->custom_handler->vtable->name);

      തിരികെ ret;
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation since the seccomp profile is already in place now and might block
     some of the syscalls needed by mark_or_close_fds_ge_than.  */
  ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, def->process->args));

  എങ്കിൽ (errno == ENOENT)
    തിരികെ crun_make_error (err, errno, "exec container process (missing dynamic library?) `%s`", exec_path);

  തിരികെ crun_make_error (err, errno, "exec container process `%s`", exec_path);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
read_container_config_from_state (libcrun_container_t **container, സ്ഥിരം അക്ഷരം *state_വേര്, സ്ഥിരം അക്ഷരം *id,
                                  libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *config_file = ശൂന്യ;
  cleanup_free അക്ഷരം *dir = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;

  *container = ശൂന്യ;

  ret = libcrun_get_state_directory (&dir, state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = append_paths (&config_file, err, dir, "config.json", ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  *container = libcrun_container_load_from_file (config_file, err);
  എങ്കിൽ (*container == ശൂന്യ)
    തിരികെ -1;

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
run_poststop_hooks (arg_unused libcrun_context_t *context, libcrun_container_t *container,
                    runtime_spec_schema_config_schema *def,
                    libcrun_container_status_t *status, സ്ഥിരം അക്ഷരം *state_വേര്, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container_cleanup = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (def == ശൂന്യ)
    {
      എങ്കിൽ (container == ശൂന്യ)
        {
          ret = read_container_config_from_state (&container_cleanup, state_വേര്, id, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
          container = container_cleanup;
        }

      def = container->container_def;
    }

  എങ്കിൽ (def->hooks && def->hooks->poststop_len)
    {
      cleanup_close പൂർണ്ണസംഖ്യ hooks_out_fd = -1;
      cleanup_close പൂർണ്ണസംഖ്യ hooks_err_fd = -1;

      എങ്കിൽ (container == ശൂന്യ)
        {
          ret = read_container_config_from_state (&container_cleanup, state_വേര്, id, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
          container = container_cleanup;
        }

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = do_hooks (def, 0, id, true, status->bundle, status->bundle, "stopped", "poststop", (hook **) def->hooks->poststop,
                      def->hooks->poststop_len, hooks_out_fd, hooks_err_fd, false, err);
      എങ്കിൽ (UNLIKELY (ret != 0))
        {
          എങ്കിൽ (ret < 0)
            libcrun_error_report_and_release (err);
          അല്ലെങ്കിൽ
            libcrun_error (0, "poststop hook failed with exit code: %d", ret);
        }
    }
  തിരികെ 0;
}

സ്ഥിതം bool
has_new_pid_namespace (runtime_spec_schema_config_schema *def)
{
  size_t i;

  എങ്കിൽ (def->linux == ശൂന്യ || def->linux->namespaces == ശൂന്യ)
    തിരികെ false;

  വേണ്ടി (i = 0; i < def->linux->namespaces_len; i++)
    {
      എങ്കിൽ (strcmp (def->linux->namespaces[i]->type, "pid") == 0 && def->linux->namespaces[i]->path == ശൂന്യ)
        തിരികെ true;
    }
  തിരികെ false;
}

/* Delete the container state.  CONTAINER is the container the caller already has
   available, or NULL.  When it is not NULL, the poststop hooks are run even if the
   status file was never written, e.g. when the container creation failed.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
container_delete_internal (libcrun_context_t *context, libcrun_container_t *container,
                           runtime_spec_schema_config_schema *def, സ്ഥിരം അക്ഷരം *id, bool force, bool killall,
                           libcrun_error_t *err)
{
  cleanup_cgroup_status ഘടന libcrun_cgroup_status *cgroup_status = ശൂന്യ;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_container libcrun_container_t *container_cleanup = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *state_വേര് = context->state_വേര്;
  പൂർണ്ണസംഖ്യ ret;

  ret = libcrun_read_container_status (&status, state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      എങ്കിൽ (force && crun_error_get_errno (err) == ENOENT)
        {
          libcrun_error_t tmp_err = ശൂന്യ;

          crun_error_release (err);

          എങ്കിൽ (container)
            {
              libcrun_container_status_t partial_status = {
                .bundle = (അക്ഷരം *) context->bundle,
              };

              ret = run_poststop_hooks (context, container, def, &partial_status, state_വേര്, id, err);
              എങ്കിൽ (UNLIKELY (ret < 0))
                crun_error_write_warning_and_release (context->output_handler_arg, &err);
            }

          libcrun_container_delete_status (state_വേര്, id, &tmp_err);
          crun_error_release (&tmp_err);
          തിരികെ 0;
        }
      crun_error_release (err);
      തിരികെ libcrun_container_delete_status (state_വേര്, id, err);
    }

  cgroup_status = libcrun_cgroup_make_status (&status);

  എങ്കിൽ (! force)
    {
      ret = libcrun_is_container_running (&status, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
      എങ്കിൽ (ret == 1)
        {
          ret = libcrun_status_has_read_exec_fifo (state_വേര്, id, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
          എങ്കിൽ (ret == 0)
            തിരികെ crun_make_error (err, 0, "the container `%s` is not in `created` or `stopped` state", id);

          /* If the container is in "created" state, then do the equivalent of delete --force.  */
          killall = force = true;
        }
    }

  എങ്കിൽ (def == ശൂന്യ)
    {
      എങ്കിൽ (container == ശൂന്യ)
        {
          ret = read_container_config_from_state (&container_cleanup, state_വേര്, id, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;

          container = container_cleanup;
        }

      def = container->container_def;
    }

  എങ്കിൽ (killall && force)
    {
      /* If the container has a pid namespace, it is enough to kill the first
         process (pid=1 in the namespace).
      */
      എങ്കിൽ (has_new_pid_namespace (def))
        {
          ret = libcrun_kill_linux (&status, SIGKILL, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            {
              എങ്കിൽ (crun_error_get_errno (err) != ESRCH)
                തിരികെ ret;

              crun_error_release (err);
            }
        }
      അല്ലെങ്കിൽ എങ്കിൽ (status.cgroup_path)
        {
          ret = libcrun_cgroup_killall (cgroup_status, SIGKILL, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }
    }

  എങ്കിൽ (def->linux && def->linux->intel_rdt)
    {
      ret = libcrun_destroy_intelrdt (id, def, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        libcrun_error_report_and_release (err);
    }

  എങ്കിൽ (status.cgroup_path)
    {
      /* If the cgroup cannot be destroyed, there are still processes in it.
         Keep the container status, it is the only record of the cgroup, so
         that the delete can be retried.  */
      ret = libcrun_cgroup_destroy (cgroup_status, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  ret = run_poststop_hooks (context, container, def, &status, state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  തിരികെ libcrun_container_delete_status (state_വേര്, id, err);
}

പൂർണ്ണസംഖ്യ
libcrun_container_delete (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, bool force, libcrun_error_t *err)
{
  തിരികെ container_delete_internal (context, ശൂന്യ, ശൂന്യ, id, force, true, err);
}

പൂർണ്ണസംഖ്യ
libcrun_container_kill (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *സൂചന, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ sig, ret;
  സ്ഥിരം അക്ഷരം *state_വേര് = context->state_വേര്;
  cleanup_container_status libcrun_container_status_t status = {};

  sig = str2sig (സൂചന);
  എങ്കിൽ (UNLIKELY (sig < 0))
    തിരികെ crun_make_error (err, 0, "unknown signal `%s`", സൂചന);

  ret = libcrun_read_container_status (&status, state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ libcrun_kill_linux (&status, sig, err);
}

പൂർണ്ണസംഖ്യ
libcrun_container_killall (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *സൂചന, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ sig, ret;
  സ്ഥിരം അക്ഷരം *state_വേര് = context->state_വേര്;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_cgroup_status ഘടന libcrun_cgroup_status *cgroup_status = ശൂന്യ;

  sig = str2sig (സൂചന);
  എങ്കിൽ (UNLIKELY (sig < 0))
    തിരികെ crun_make_error (err, 0, "unknown signal `%s`", സൂചന);

  ret = libcrun_read_container_status (&status, state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  cgroup_status = libcrun_cgroup_make_status (&status);

  ret = libcrun_cgroup_killall (cgroup_status, sig, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
write_container_status (libcrun_container_t *container, libcrun_context_t *context,
                        pid_t pid, ഘടന libcrun_cgroup_status *cgroup_status,
                        libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *bundle_allocated = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *bundle = get_bundle_path (context->bundle, &bundle_allocated);
  cleanup_free അക്ഷരം *owner = get_user_name (geteuid ());
  അക്ഷരം *external_descriptors = libcrun_get_external_descriptors (container);
  അക്ഷരം *rootfs = container->container_def->വേര് ? container->container_def->വേര്->path : "";
  അക്ഷരം created[35];

  libcrun_container_status_t status = {
    .pid = pid,
    .rootfs = rootfs,
    .bundle = (അക്ഷരം *) bundle,
    .created = created,
    .owner = owner,
    .systemd_cgroup = context->systemd_cgroup,
    .detached = context->detach,
    .no_new_keyring = context->no_new_keyring,
    .external_descriptors = external_descriptors,
    .cgroup_path = ശൂന്യ,
    .scope = ശൂന്യ,
  };

  get_current_timestamp (created, വലുപ്പം (created));

  എങ്കിൽ (cgroup_status)
    {
      പൂർണ്ണസംഖ്യ ret;

      ret = libcrun_cgroup_get_status (cgroup_status, &status, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (external_descriptors == ശൂന്യ)
    തിരികെ crun_make_error (err, 0, "invalid internal state.  No external descriptors found");
  തിരികെ libcrun_write_container_status (context->state_വേര്, context->id, &status, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
reap_subprocesses (pid_t *main_process, പൂർണ്ണസംഖ്യ *main_process_exit, പൂർണ്ണസംഖ്യ *last_process, libcrun_error_t *err)
{
  *last_process = 0;
  ആകുമ്പോൾ (1)
    {
      പൂർണ്ണസംഖ്യ status;
      പൂർണ്ണസംഖ്യ r = waitpid_ignore_stopped (-1, &status, WNOHANG);
      എങ്കിൽ (r < 0)
        {
          എങ്കിൽ (errno == EINTR)
            തുടരുക;
          എങ്കിൽ (errno == ECHILD)
            {
              *last_process = 1;
              തിരികെ 0;
            }
          തിരികെ crun_make_error (err, errno, "waitpid");
        }
      എങ്കിൽ (r == 0)
        നിർത്തുക;
      എങ്കിൽ (r != *main_process)
        തുടരുക;

      *main_process_exit = get_process_exit_status (status);

      /* Once reaped, the pid can be reused by an unrelated process.  */
      *main_process = 0;
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
send_sd_notify (സ്ഥിരം അക്ഷരം *ready_str, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  പൂർണ്ണസംഖ്യ ret = sd_notify (0, ready_str);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, -ret, "sd_notify");

#  എങ്കിൽ HAVE_SD_NOTIFY_BARRIER
  /* Hard-code a 30 seconds timeout.  Ignore errors.  */
  sd_notify_barrier (0, 30 * 1000000);
#  endif
#അല്ലെങ്കിൽ
  (ശൂന്യം) ready_str;
  (ശൂന്യം) err;
#endif
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
handle_notify_socket (പൂർണ്ണസംഖ്യ notify_socketfd, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  പൂർണ്ണസംഖ്യ ret;
  അക്ഷരം buf[256];
  സ്ഥിരം അക്ഷരം *ready_str = "READY=1";

  ret = recvfrom (notify_socketfd, buf, വലുപ്പം (buf) - 1, 0, ശൂന്യ, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "recvfrom notify socket");

  buf[ret] = '\0';
  എങ്കിൽ (strstr (buf, ready_str))
    {
      ret = send_sd_notify (ready_str, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      തിരികെ 1;
    }
  തിരികെ 0;
#അല്ലെങ്കിൽ
  (ശൂന്യം) notify_socketfd;
  (ശൂന്യം) err;
  തിരികെ 1;
#endif
}

ഘടന wait_for_process_args
{
  pid_t pid;
  libcrun_context_t *context;
  പൂർണ്ണസംഖ്യ terminal_fd;
  പൂർണ്ണസംഖ്യ notify_socket;
  പൂർണ്ണസംഖ്യ *container_ready_fd;
  പൂർണ്ണസംഖ്യ seccomp_notify_fd;
  സ്ഥിരം അക്ഷരം *seccomp_notify_plugins;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ
write_pid_file (സ്ഥിരം അക്ഷരം *pid_file, pid_t pid, libcrun_error_t *err)
{
  അക്ഷരം buf[32];
  പൂർണ്ണസംഖ്യ buf_len = snprintf (buf, വലുപ്പം (buf), "%d", pid);
  എങ്കിൽ (UNLIKELY (buf_len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (buf)))
    തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");
  തിരികെ write_file_at_with_flags (AT_FDCWD, O_CREAT | O_TRUNC, 0700, pid_file, buf, buf_len, err);
}

#define MAX_EVENTS 10

സ്ഥിതം bool
is_fd_pollable (പൂർണ്ണസംഖ്യ fd)
{
  ഘടന epoll_event ev = { .events = EPOLLIN };
  cleanup_close പൂർണ്ണസംഖ്യ epollfd = epoll_create1 (EPOLL_CLOEXEC);

  എങ്കിൽ (epollfd < 0)
    തിരികെ true;

  തിരികെ epoll_ctl (epollfd, EPOLL_CTL_ADD, fd, &ev) == 0 || errno != EPERM;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
wait_for_process (ഘടന wait_for_process_args *args, libcrun_error_t *err)
{
  cleanup_channel_fd_pair ഘടന channel_fd_pair *from_terminal = ശൂന്യ;
  cleanup_channel_fd_pair ഘടന channel_fd_pair *to_terminal = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret, container_exit_code = 0, last_process;
  cleanup_close പൂർണ്ണസംഖ്യ terminal_fd_from = -1;
  cleanup_close പൂർണ്ണസംഖ്യ terminal_fd_to = -1;
  bool stdin_pollable = false, stdout_pollable = false;
  cleanup_close പൂർണ്ണസംഖ്യ epollfd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ signalfd = -1;
  sigset_t mask;
  പൂർണ്ണസംഖ്യ in_fds[MAX_EVENTS];
  പൂർണ്ണസംഖ്യ in_fds_len = 0;
  പൂർണ്ണസംഖ്യ out_fds[MAX_EVENTS];
  പൂർണ്ണസംഖ്യ out_fds_len = 0;
  size_t i;

  cleanup_seccomp_notify_context ഘടന seccomp_notify_context_s *seccomp_notify_ctx = ശൂന്യ;

  container_exit_code = 0;

  എങ്കിൽ (args == ശൂന്യ || args->context == ശൂന്യ)
    തിരികെ crun_make_error (err, 0, "internal error: context is empty");

  വേണ്ടി (i = 0; i < MAX_EVENTS; i++)
    {
      in_fds[i] = -1;
      out_fds[i] = -1;
    }

  എങ്കിൽ (args->context->pid_file)
    {
      ret = write_pid_file (args->context->pid_file, args->pid, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  /* Also exit if there is nothing more to wait for.  */
  എങ്കിൽ (args->context->detach && args->notify_socket < 0)
    തിരികെ 0;

  എങ്കിൽ (args->container_ready_fd)
    {
      ret = 0;
      TEMP_FAILURE_RETRY (ലിഖ (*args->container_ready_fd, &ret, വലുപ്പം (ret)));
      close_and_reset (args->container_ready_fd);
    }

  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "sigprocmask");

  signalfd = create_signalfd (&mask, err);
  എങ്കിൽ (UNLIKELY (signalfd < 0))
    തിരികെ signalfd;

  ret = reap_subprocesses (&args->pid, &container_exit_code, &last_process, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (last_process)
    തിരികെ container_exit_code;

  എങ്കിൽ (args->seccomp_notify_fd >= 0)
    {
      cleanup_free അക്ഷരം *state_വേര് = ശൂന്യ;
      cleanup_free അക്ഷരം *oci_config_path = ശൂന്യ;

      ഘടന libcrun_load_seccomp_notify_conf_s conf;
      memset (&conf, 0, വലുപ്പം conf);

      ret = libcrun_get_state_directory (&state_വേര്, args->context->state_വേര്, args->context->id, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = append_paths (&oci_config_path, err, state_വേര്, "config.json", ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      conf.runtime_വേര്_path = state_വേര്;
      conf.name = args->context->id;
      conf.bundle_path = args->context->bundle;
      conf.oci_config_path = oci_config_path;

      ret = set_blocking_fd (args->seccomp_notify_fd, false, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = libcrun_load_seccomp_notify_plugins (&seccomp_notify_ctx,
                                                 args->seccomp_notify_plugins,
                                                 &conf, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      in_fds[in_fds_len++] = args->seccomp_notify_fd;
    }

  എങ്കിൽ (args->terminal_fd >= 0)
    {
      /* The terminal_fd is dup()ed so that it can be registered with
         epoll multiple times using different masks.  */
      terminal_fd_from = dup (args->terminal_fd);
      എങ്കിൽ (UNLIKELY (terminal_fd_from < 0))
        തിരികെ crun_make_error (err, errno, "dup terminal fd");
      terminal_fd_to = dup (args->terminal_fd);
      എങ്കിൽ (UNLIKELY (terminal_fd_to < 0))
        തിരികെ crun_make_error (err, errno, "dup terminal fd");

      പൂർണ്ണസംഖ്യ i, non_blocking_fds[] = { terminal_fd_from, terminal_fd_to, 0, 1, -1 };
      വേണ്ടി (i = 0; non_blocking_fds[i] >= 0; i++)
        {
          ret = set_blocking_fd (non_blocking_fds[i], false, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }

      /* stdin and stdout can be something epoll does not support, such as
         /dev/null or a regular file.  Such a stdin is not forwarded, and
         such a stdout is written to without waiting for it to be ready.  */
      stdin_pollable = is_fd_pollable (0);
      stdout_pollable = is_fd_pollable (1);

      from_terminal = channel_fd_pair_new (terminal_fd_from, 1, BUFSIZ);
      എങ്കിൽ (stdin_pollable)
        to_terminal = channel_fd_pair_new (0, terminal_fd_to, BUFSIZ);
    }

  in_fds[in_fds_len++] = signalfd;
  എങ്കിൽ (args->notify_socket >= 0)
    in_fds[in_fds_len++] = args->notify_socket;
  എങ്കിൽ (args->terminal_fd >= 0)
    {
      എങ്കിൽ (stdin_pollable)
        {
          in_fds[in_fds_len++] = 0;
          out_fds[out_fds_len++] = terminal_fd_to;
        }

      in_fds[in_fds_len++] = terminal_fd_from;
      എങ്കിൽ (stdout_pollable)
        out_fds[out_fds_len++] = 1;
    }

  epollfd = epoll_helper (in_fds, ശൂന്യ, out_fds, ശൂന്യ, err);
  എങ്കിൽ (UNLIKELY (epollfd < 0))
    തിരികെ epollfd;

  ആകുമ്പോൾ (1)
    {
      ഘടന epoll_event events[MAX_EVENTS];
      ഘടന signalfd_siginfo si;
      ഘടന winsize ws;
      പൂർണ്ണസംഖ്യ i, nr_events;
      ssize_t res;

      nr_events = TEMP_FAILURE_RETRY (epoll_wait (epollfd, events, MAX_EVENTS, -1));
      എങ്കിൽ (UNLIKELY (nr_events < 0))
        തിരികെ crun_make_error (err, errno, "epoll_wait");

      വേണ്ടി (i = 0; i < nr_events; i++)
        {
          എങ്കിൽ (events[i].data.fd == 0 || events[i].data.fd == terminal_fd_to)
            {
              ret = channel_fd_pair_process (to_terminal, epollfd, err);
              എങ്കിൽ (UNLIKELY (ret < 0))
                തിരികെ crun_error_wrap (err, "copy to terminal fd");
            }
          അല്ലെങ്കിൽ എങ്കിൽ (events[i].data.fd == 1 || events[i].data.fd == terminal_fd_from)
            {
              ret = channel_fd_pair_process (from_terminal, epollfd, err);
              എങ്കിൽ (UNLIKELY (ret < 0))
                തിരികെ crun_error_wrap (err, "copy from terminal fd");
            }
          അല്ലെങ്കിൽ എങ്കിൽ (events[i].data.fd == args->seccomp_notify_fd)
            {
              ret = libcrun_seccomp_notify_plugins (seccomp_notify_ctx,
                                                    args->seccomp_notify_fd, err);
              എങ്കിൽ (UNLIKELY (ret < 0))
                തിരികെ ret;
            }
          അല്ലെങ്കിൽ എങ്കിൽ (events[i].data.fd == args->notify_socket)
            {
              ret = handle_notify_socket (args->notify_socket, err);
              എങ്കിൽ (UNLIKELY (ret < 0))
                തിരികെ ret;
              എങ്കിൽ (ret && args->context->detach)
                തിരികെ 0;
            }
          അല്ലെങ്കിൽ എങ്കിൽ (events[i].data.fd == signalfd)
            {
              res = TEMP_FAILURE_RETRY (പഠ (signalfd, &si, വലുപ്പം (si)));
              എങ്കിൽ (UNLIKELY (res < 0))
                തിരികെ crun_make_error (err, errno, "read from signalfd");
              എങ്കിൽ (si.ssi_signo == SIGCHLD)
                {
                  ret = reap_subprocesses (&args->pid, &container_exit_code,
                                           &last_process, err);
                  എങ്കിൽ (UNLIKELY (ret < 0))
                    തിരികെ ret;
                  എങ്കിൽ (last_process)
                    തിരികെ container_exit_code;
                }
              അല്ലെങ്കിൽ എങ്കിൽ (si.ssi_signo == SIGWINCH)
                {
                  /* Ignore the signal if the terminal is not available.  */
                  എങ്കിൽ (args->terminal_fd > 0)
                    {
                      ret = ioctl (0, TIOCGWINSZ, &ws);
                      എങ്കിൽ (UNLIKELY (ret < 0))
                        തിരികെ crun_make_error (err, errno, "ioctl TIOCGWINSZ copy terminal size from stdin");

                      ret = ioctl (args->terminal_fd, TIOCSWINSZ, &ws);
                      എങ്കിൽ (UNLIKELY (ret < 0))
                        തിരികെ crun_make_error (err, errno, "ioctl TIOCSWINSZ copy terminal size to pty");
                    }
                }
              അല്ലെങ്കിൽ എങ്കിൽ (args->pid > 0)
                {
                  /* Send any other signal to the child process, if it was not reaped yet.  */
                  ret = kill (args->pid, si.ssi_signo);
                }
            }
          അല്ലെങ്കിൽ
            {
              തിരികെ crun_make_error (err, 0, "internal error: unknown fd from epoll_wait");
            }
        }
    }

  തിരികെ 0;
}

സ്ഥിതം ശൂന്യം
flush_fd_to_err (libcrun_context_t *context, പൂർണ്ണസംഖ്യ terminal_fd)
{
  അക്ഷരം buf[256];
  പൂർണ്ണസംഖ്യ flags;

  എങ്കിൽ (terminal_fd < 0 || stderr == ശൂന്യ)
    തിരികെ;

  flags = fcntl (terminal_fd, F_GETFL, 0);
  എങ്കിൽ (flags == -1)
    തിരികെ;
  എങ്കിൽ (fcntl (terminal_fd, F_SETFL, flags | O_NONBLOCK) < 0)
    തിരികെ;

  വേണ്ടി (;;)
    {
      പൂർണ്ണസംഖ്യ ret = TEMP_FAILURE_RETRY (പഠ (terminal_fd, buf, വലുപ്പം (buf) - 1));
      എങ്കിൽ (ret <= 0)
        നിർത്തുക;
      buf[ret] = '\0';
      എങ്കിൽ (context->output_handler)
        context->output_handler (0, buf, LIBCRUN_VERBOSITY_ERROR, context->output_handler_arg);
    }
  (ശൂന്യം) fcntl (terminal_fd, F_SETFL, flags);
  fflush (stderr);
  fsync (1);
  fsync (2);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
cleanup_watch (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
               ഘടന libcrun_cgroup_status *cgroup_status, pid_t init_pid, പൂർണ്ണസംഖ്യ sync_socket,
               പൂർണ്ണസംഖ്യ terminal_fd, libcrun_error_t *err)
{
  സ്ഥിരം അക്ഷരം *oom_message = ശൂന്യ;
  libcrun_error_t tmp_err = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (init_pid)
    {
      /* Try to detect whether the cgroup has a OOM.  */
      എങ്കിൽ (cgroup_status)
        {
          പൂർണ്ണസംഖ്യ has_oom;

          has_oom = libcrun_cgroup_has_oom (cgroup_status, &tmp_err);
          എങ്കിൽ (has_oom > 0)
            oom_message = "OOM: the memory limit could be too low";
          അല്ലെങ്കിൽ എങ്കിൽ (has_oom < 0)
            {
              /* If the detection has failed for any reason, e.g. the cgroup was
                 already deleted by the time it was checked, just ignore the
                 failure.  */
              crun_error_release (&tmp_err);
            }
        }
      /* If the OOM wasn't detected, look into the static configuration.  */
      എങ്കിൽ (oom_message == ശൂന്യ && is_memory_limit_too_താഴെയുള്ള (def))
        oom_message = "the memory limit could be too low";

      kill (init_pid, SIGKILL);
      waitpid_ignore_stopped (init_pid, ശൂന്യ, 0);
    }

  ret = sync_socket_wait_sync (context, sync_socket, true, &tmp_err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      crun_error_release (err);
      *err = tmp_err;
    }

  എങ്കിൽ (terminal_fd >= 0)
    flush_fd_to_err (context, terminal_fd);

  എങ്കിൽ (oom_message)
    തിരികെ crun_error_wrap (err, "%s", oom_message);

  തിരികെ -1;
}

/* Find the uid:gid that is mapped to root inside the container user namespace.  */
ശൂന്യം
get_വേര്_in_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid, uid_t *uid,
                        gid_t *gid)
{
  *uid = -1;
  *gid = -1;

  /* Not root in the namespace.  */
  എങ്കിൽ (host_uid)
    തിരികെ;

  എങ്കിൽ (def->linux && def->linux->uid_mappings)
    {
      size_t i;

      വേണ്ടി (i = 0; i < def->linux->uid_mappings_len; i++)
        എങ്കിൽ (def->linux->uid_mappings[i]->container_id == 0)
          {
            *uid = def->linux->uid_mappings[i]->host_id;
            നിർത്തുക;
          }
    }
  എങ്കിൽ (def->linux && def->linux->gid_mappings)
    {
      size_t i;

      വേണ്ടി (i = 0; i < def->linux->gid_mappings_len; i++)
        എങ്കിൽ (def->linux->gid_mappings[i]->container_id == 0)
          {
            *gid = def->linux->gid_mappings[i]->host_id;
            നിർത്തുക;
          }
    }

  /* If the uid and the gid are not changed, do not attempt any chown.  */
  എങ്കിൽ (*uid == host_uid && *gid == host_gid)
    *uid = *gid = -1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
get_seccomp_receiver_fd (libcrun_container_t *container, പൂർണ്ണസംഖ്യ *fd, പൂർണ്ണസംഖ്യ *self_receiver_fd, സ്ഥിരം അക്ഷരം **plugins,
                         libcrun_error_t *err)
{
  സ്ഥിരം അക്ഷരം *tmp;
  runtime_spec_schema_config_schema *def = container->container_def;

  *fd = -1;
  *self_receiver_fd = -1;

  tmp = find_annotation (container, "run.oci.seccomp.plugins");
  എങ്കിൽ (tmp)
    {
      പൂർണ്ണസംഖ്യ fds[2];
      പൂർണ്ണസംഖ്യ ret;

      ret = create_socket_pair (fds, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_error_wrap (err, "create socket pair");

      *fd = fds[0];
      *self_receiver_fd = fds[1];
      *plugins = tmp;
      തിരികെ 0;
    }

  എങ്കിൽ (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    tmp = def->linux->seccomp->listener_path;
  അല്ലെങ്കിൽ
    tmp = find_annotation (container, "run.oci.seccomp.receiver");
  എങ്കിൽ (tmp == ശൂന്യ)
    tmp = getenv ("RUN_OCI_SECCOMP_RECEIVER");
  എങ്കിൽ (tmp)
    {
      എങ്കിൽ (tmp[0] != '/')
        തിരികെ crun_make_error (err, 0, "the seccomp receiver `%s` is not an absolute path", tmp);

      *fd = open_unix_domain_client_socket (tmp, 0, err);
      എങ്കിൽ (UNLIKELY (*fd < 0))
        തിരികെ crun_error_wrap (err, "open seccomp receiver");
    }

  തിരികെ 0;
}

സ്ഥിതം bool
has_seccomp_receiver (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;

  എങ്കിൽ (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    തിരികെ true;

  എങ്കിൽ (find_annotation (container, "run.oci.seccomp.receiver") != ശൂന്യ || getenv ("RUN_OCI_SECCOMP_RECEIVER") != ശൂന്യ)
    തിരികെ true;

  തിരികെ false;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
setup_container_hooks_output (libcrun_container_t *container, runtime_spec_schema_config_schema *def,
                              ഘടന container_entrypoint_s *container_args, പൂർണ്ണസംഖ്യ *hooks_out_fd,
                              പൂർണ്ണസംഖ്യ *hooks_err_fd, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (def->hooks
      && (def->hooks->prestart_len || def->hooks->poststart_len || def->hooks->create_runtime_len
          || def->hooks->create_container_len || def->hooks->start_container_len))
    {
      ret = open_hooks_output (container, hooks_out_fd, hooks_err_fd, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
      container_args->hooks_out_fd = *hooks_out_fd;
      container_args->hooks_err_fd = *hooks_err_fd;
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
setup_container_keyring (libcrun_container_t *container, libcrun_context_t *context, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (! context->no_new_keyring)
    {
      സ്ഥിരം അക്ഷരം *label = ശൂന്യ;

      libcrun_debug ("Creating new keyring");

      എങ്കിൽ (def->process)
        {
          label = def->process->selinux_label;
          എങ്കിൽ (label)
            libcrun_debug ("Using SELinux process label: `%s`", label);
        }

      ret = libcrun_create_keyring (container, container->context->id, label, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
setup_terminal_socket_pair (libcrun_container_t *container, libcrun_context_t *context,
                            ഘടന container_entrypoint_s *container_args, പൂർണ്ണസംഖ്യ *socket_pair_0,
                            പൂർണ്ണസംഖ്യ *socket_pair_1, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  പൂർണ്ണസംഖ്യ ret;
  പൂർണ്ണസംഖ്യ detach = context->detach;

  എങ്കിൽ (def->process && def->process->terminal && ! detach && context->console_socket == ശൂന്യ)
    {
      libcrun_debug ("Creating terminal socket pair");
      container_args->has_terminal_socket_pair = 1;
      ret = create_socket_pair (container_args->terminal_socketpair, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_error_wrap (err, "create terminal socket");

      *socket_pair_0 = container_args->terminal_socketpair[0];
      *socket_pair_1 = container_args->terminal_socketpair[1];
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
setup_seccomp (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *seccomp_bpf_data,
               ഘടന libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, പൂർണ്ണസംഖ്യ *seccomp_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (find_annotation (container, "run.oci.seccomp.plugins") != ശൂന്യ && has_seccomp_receiver (container))
    {
      തിരികെ crun_make_error (err, 0, "seccomp plugins and seccomp receivers cannot be declared at the same time");
    }

  എങ്കിൽ (def->linux && (def->linux->seccomp || seccomp_bpf_data))
    {
      unsigned പൂർണ്ണസംഖ്യ seccomp_gen_options = 0;
      സ്ഥിരം അക്ഷരം *annotation;

      libcrun_debug ("Initializing seccomp");
      annotation = find_annotation (container, "run.oci.seccomp_fail_unknown_syscall");
      എങ്കിൽ (annotation && strcmp (annotation, "0") != 0)
        seccomp_gen_options = LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL;

      എങ്കിൽ (seccomp_bpf_data)
        seccomp_gen_options |= LIBCRUN_SECCOMP_SKIP_CACHE;

      libcrun_seccomp_gen_ctx_init (seccomp_gen_ctx, container, true, seccomp_gen_options);

      ret = libcrun_open_seccomp_bpf (seccomp_gen_ctx, seccomp_fd, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
setup_console_socket (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
                      ഘടന container_entrypoint_s *container_args, പൂർണ്ണസംഖ്യ *console_socket_fd,
                      libcrun_error_t *err)
{
  എങ്കിൽ (context->console_socket && def->process && def->process->terminal)
    {
      *console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
      എങ്കിൽ (UNLIKELY (*console_socket_fd < 0))
        തിരികെ crun_error_wrap (err, "open console socket");
      container_args->console_socket_fd = *console_socket_fd;
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
setup_cgroup_manager (libcrun_context_t *context, libcrun_container_t *container,
                      ഘടന libcrun_cgroup_args *cg, പൂർണ്ണസംഖ്യ *cgroup_dirfd,
                      ഘടന libcrun_dirfd_s *cgroup_dirfd_s, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  പൂർണ്ണസംഖ്യ cgroup_manager;
  uid_t വേര്_uid = -1;
  gid_t വേര്_gid = -1;
  പൂർണ്ണസംഖ്യ ret;

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  എങ്കിൽ (context->systemd_cgroup)
    {
      libcrun_debug ("Using systemd cgroup manager");
      cgroup_manager = CGROUP_MANAGER_SYSTEMD;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (context->force_no_cgroup)
    {
      libcrun_debug ("Disabling cgroup manager");
      cgroup_manager = CGROUP_MANAGER_DISABLED;
    }
  അല്ലെങ്കിൽ
    libcrun_debug ("Using cgroupfs cgroup manager");

  /* If we are root (either on the host or in a namespace), then chown the cgroup to root
     in the container user namespace.  */
  get_വേര്_in_the_userns (def, container->host_uid, container->host_gid, &വേര്_uid, &വേര്_gid);
  libcrun_debug ("Using container host UID `%d` and GID `%d`", container->host_uid, container->host_gid);

  memset (cg, 0, വലുപ്പം (*cg));

  cg->cgroup_path = def->linux ? def->linux->cgroups_path : "";
  cg->manager = cgroup_manager;
  cg->id = context->id;
  cg->resources = def->linux ? def->linux->resources : ശൂന്യ;
  cg->annotations = container->annotations;
  cg->വേര്_uid = വേര്_uid;
  cg->വേര്_gid = വേര്_gid;
  cg->state_വേര് = context->state_വേര്;
  cg->container = container;

  ret = libcrun_cgroup_preenter (cg, cgroup_dirfd, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  cgroup_dirfd_s->dirfd = cgroup_dirfd;
  cgroup_dirfd_s->joined = false;

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
set_scheduler (pid_t pid, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  ret = libcrun_set_scheduler (pid, def->process, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_reset_cpu_affinity_mask (pid, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_set_io_priority (pid, def->process, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
seccomp_generation (പൂർണ്ണസംഖ്യ seccomp_fd, സ്ഥിരം അക്ഷരം *seccomp_bpf_data,
                    ഘടന libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (seccomp_fd >= 0)
    {
      എങ്കിൽ (seccomp_bpf_data != ശൂന്യ)
        {
          ret = libcrun_copy_seccomp (seccomp_gen_ctx, seccomp_bpf_data, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }
      അല്ലെങ്കിൽ
        {
          ret = libcrun_generate_seccomp (seccomp_gen_ctx, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
terminal_setup (runtime_spec_schema_config_schema *def, libcrun_context_t *context,
                പൂർണ്ണസംഖ്യ socket_pair_0, പൂർണ്ണസംഖ്യ *terminal_fd, ശൂന്യം **orig_terminal, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  പൂർണ്ണസംഖ്യ detach = context->detach;

  എങ്കിൽ (def->process && def->process->terminal && ! detach && context->console_socket == ശൂന്യ)
    {
      libcrun_debug ("Receiving console socket fd");
      *terminal_fd = receive_fd_from_socket (socket_pair_0, err);
      എങ്കിൽ (UNLIKELY (*terminal_fd < 0))
        തിരികെ -1;

      ret = libcrun_clear_onlcr (*terminal_fd, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = libcrun_set_raw (0, orig_terminal, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libcrun_container_run_internal (libcrun_container_t *container, libcrun_context_t *context,
                                പൂർണ്ണസംഖ്യ *container_ready_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  പൂർണ്ണസംഖ്യ ret;
  pid_t pid;
  പൂർണ്ണസംഖ്യ detach = context->detach;
  cleanup_cgroup_status ഘടന libcrun_cgroup_status *cgroup_status = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ terminal_fd = -1;
  cleanup_terminal ശൂന്യം *orig_terminal = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ sync_socket = -1;
  cleanup_close പൂർണ്ണസംഖ്യ notify_socket = -1;
  cleanup_close പൂർണ്ണസംഖ്യ socket_pair_0 = -1;
  cleanup_close പൂർണ്ണസംഖ്യ socket_pair_1 = -1;
  cleanup_close പൂർണ്ണസംഖ്യ seccomp_fd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ console_socket_fd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ hooks_out_fd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ hooks_err_fd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ own_seccomp_receiver_fd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ seccomp_notify_fd = -1;
  സ്ഥിരം അക്ഷരം *seccomp_notify_plugins = ശൂന്യ;
  cleanup_cgroup_preenter ഘടന libcrun_cgroup_args cg = {};
  cleanup_free അക്ഷരം *bundle_allocated = ശൂന്യ;
  ഘടന container_entrypoint_s container_args = {
    .container = container,
    .context = context,
    .bundle = get_bundle_path (context->bundle, &bundle_allocated),
    .terminal_socketpair = { -1, -1 },
    .console_socket_fd = -1,
    .hooks_out_fd = -1,
    .hooks_err_fd = -1,
    .seccomp_receiver_fd = -1,
    .custom_handler = ശൂന്യ,
  };
  cleanup_close പൂർണ്ണസംഖ്യ cgroup_dirfd = -1;
  ഘടന libcrun_dirfd_s cgroup_dirfd_s;
  ഘടന libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  സ്ഥിരം അക്ഷരം *seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");
  പൂർണ്ണസംഖ്യ cgroup_mode;

  എങ്കിൽ (! context->force_no_cgroup)
    {
      cgroup_mode = libcrun_get_cgroup_mode (err);
      എങ്കിൽ (UNLIKELY (cgroup_mode < 0))
        തിരികെ cgroup_mode;

      എങ്കിൽ (cgroup_mode != CGROUP_MODE_UNIFIED)
        libcrun_warning ("cgroup v1 is deprecated and will be removed in a future release.  Use cgroup v2");
    }

  ret = setup_container_hooks_output (container, def, &container_args, &hooks_out_fd, &hooks_err_fd, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  container->context = context;

  എങ്കിൽ (! detach || context->notify_socket)
    {
      libcrun_debug ("Setting child subreaper");
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "prctl set child subreaper");
    }

  ret = setup_container_keyring (container, context, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = setup_terminal_socket_pair (container, context, &container_args, &socket_pair_0, &socket_pair_1, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = block_signals (err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  umask (0);

  ret = libcrun_set_mempolicy (def, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;
  container_args.seccomp_fd = seccomp_fd;

  എങ്കിൽ (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &container_args.seccomp_receiver_fd, &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  ret = setup_console_socket (context, def, &container_args, &console_socket_fd, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = setup_cgroup_manager (context, container, &cg, &cgroup_dirfd, &cgroup_dirfd_s, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_configure_handler (container_args.context->handler_manager,
                                   container_args.context,
                                   container,
                                   &(container_args.custom_handler),
                                   err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (container_args.custom_handler && container_args.custom_handler->vtable->modify_oci_configuration)
    {
      libcrun_debug ("Using custom handler to modify OCI configuration");
      ret = container_args.custom_handler->vtable->modify_oci_configuration (container_args.custom_handler->cookie,
                                                                             container_args.context,
                                                                             container->container_def,
                                                                             err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  pid = libcrun_run_linux_container (container, container_init, &container_args, &sync_socket, &cgroup_dirfd_s, err);
  എങ്കിൽ (UNLIKELY (pid < 0))
    തിരികെ pid;

  cg.pid = pid;
  cg.joined = cgroup_dirfd_s.joined;
  libcrun_debug ("Running container on PID: `%d`", pid);

  എങ്കിൽ (context->fifo_exec_wait_fd < 0 && context->notify_socket)
    {
      libcrun_debug ("Using notify socket: `%s`", context->notify_socket);
      /* Do not open the notify socket here on "create".  "start" will take care of it.  */
      ret = get_notify_fd (context, container, &notify_socket, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        പോകുക fail;
    }

  എങ്കിൽ (container_args.terminal_socketpair[1] >= 0)
    close_and_reset (&socket_pair_1);

  ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക fail;

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക fail;

  ret = libcrun_move_network_devices (container, pid, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക fail;

  /* sync send own pid.  */
  ret = TEMP_FAILURE_RETRY (ലിഖ (sync_socket, &pid, വലുപ്പം (pid)));
  എങ്കിൽ (UNLIKELY (ret != വലുപ്പം (pid)))
    {
      എങ്കിൽ (ret >= 0)
        errno = 0;
      crun_make_error (err, errno, "write to sync socket");
      പോകുക fail;
    }

  /* sync 1.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക fail;

  /* sync 2.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക fail;

  ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക fail;

  /* If the container process is created later, the scheduler is set on it then.  */
  എങ്കിൽ (! libcrun_container_delayed_pidns_fork (container))
    {
      ret = set_scheduler (pid, def, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        പോകുക fail;
    }

  /* The container is waiting that we write back.  In this phase we can launch the
     prestart hooks.  */
  എങ്കിൽ (def->hooks && def->hooks->prestart_len)
    {
      libcrun_debug ("Running `prestart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "prestart", (hook **) def->hooks->prestart,
                      def->hooks->prestart_len, hooks_out_fd, hooks_err_fd, false, err);
      എങ്കിൽ (UNLIKELY (ret != 0))
        പോകുക fail;
    }
  എങ്കിൽ (def->hooks && def->hooks->create_runtime_len)
    {
      libcrun_debug ("Running `create` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "createRuntime", (hook **) def->hooks->create_runtime,
                      def->hooks->create_runtime_len, hooks_out_fd, hooks_err_fd, false, err);
      എങ്കിൽ (UNLIKELY (ret != 0))
        പോകുക fail;
    }

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക fail;
  close_and_reset (&seccomp_fd);

  /* sync 3.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക fail;

  /* The process joining an existing PID namespace is created once the container root is in place.  */
  എങ്കിൽ (libcrun_container_delayed_pidns_fork (container))
    {
      pid_t new_pid = 0;

      ret = sync_socket_wait_sync (context, sync_socket, false, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        പോകുക fail;

      ret = TEMP_FAILURE_RETRY (പഠ (sync_socket, &new_pid, വലുപ്പം (new_pid)));
      എങ്കിൽ (UNLIKELY (ret != വലുപ്പം (new_pid)))
        {
          എങ്കിൽ (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "read pid from sync socket");
          പോകുക fail;
        }

      /* Cleanup the first process.  */
      waitpid_ignore_stopped (pid, ശൂന്യ, 0);

      pid = new_pid;

      ret = set_scheduler (pid, def, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        പോകുക fail;

      ret = libcrun_handle_pidfd_receiver (pid, container, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        പോകുക fail;

      /* sync send own pid.  */
      ret = TEMP_FAILURE_RETRY (ലിഖ (sync_socket, &pid, വലുപ്പം (pid)));
      എങ്കിൽ (UNLIKELY (ret != വലുപ്പം (pid)))
        {
          എങ്കിൽ (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "write to sync socket");
          പോകുക fail;
        }
    }

  ret = terminal_setup (def, context, socket_pair_0, &terminal_fd, &orig_terminal, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക fail;

  close_and_reset (&socket_pair_0);

  /* sync 4.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക fail;

  ret = close_and_reset (&sync_socket);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      crun_make_error (err, errno, "close sync_socket failed");
      പോകുക fail;
    }

  libcrun_debug ("Writing container status");
  ret = write_container_status (container, context, pid, cgroup_status, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക fail;

  /* Run poststart hooks here only if the container is created using "run".  For create+start, the
     hooks will be executed as part of the start command.  */
  എങ്കിൽ (context->fifo_exec_wait_fd < 0 && def->hooks && def->hooks->poststart_len)
    {
      libcrun_debug ("Running `poststart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      എങ്കിൽ (UNLIKELY (ret != 0))
        പോകുക fail;
    }

  /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
  എങ്കിൽ (own_seccomp_receiver_fd >= 0)
    {
      libcrun_debug ("Receiving seccomp fd");
      seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
      എങ്കിൽ (UNLIKELY (seccomp_notify_fd < 0))
        പോകുക fail;

      ret = close_and_reset (&own_seccomp_receiver_fd);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          crun_make_error (err, errno, "close seccomp receiver fd failed");
          പോകുക fail;
        }
    }

  {
    ഘടന wait_for_process_args args = {
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
  എങ്കിൽ (! context->detach)
    {
      libcrun_error_t tmp_err = ശൂന്യ;
      cleanup_watch (context, def, cgroup_status, 0, sync_socket, terminal_fd, &tmp_err);
      crun_error_release (&tmp_err);
    }

  തിരികെ ret;

fail:
  ret = cleanup_watch (context, def, cgroup_status, pid, sync_socket, terminal_fd, err);
  എങ്കിൽ (cgroup_status)
    {
      libcrun_error_t tmp_err = ശൂന്യ;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
check_config_file (runtime_spec_schema_config_schema *def, libcrun_context_t *context, libcrun_error_t *err)
{
  എങ്കിൽ (UNLIKELY (def->linux == ശൂന്യ))
    തിരികെ crun_make_error (err, 0, "invalid config file, no `linux` block specified");

  എങ്കിൽ (context->handler == ശൂന്യ)
    {
      എങ്കിൽ (UNLIKELY (def->വേര് == ശൂന്യ))
        തിരികെ crun_make_error (err, 0, "invalid config file, no `root` block specified");
      എങ്കിൽ (UNLIKELY (def->mounts == ശൂന്യ))
        തിരികെ crun_make_error (err, 0, "invalid config file, no `mounts` block specified");
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libcrun_copy_config_file (സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *state_വേര്, libcrun_container_t *container, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  cleanup_free അക്ഷരം *dest_path = ശൂന്യ;
  cleanup_free അക്ഷരം *dir = ശൂന്യ;
  cleanup_free അക്ഷരം *buffer = ശൂന്യ;
  size_t len;

  ret = libcrun_get_state_directory (&dir, state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = append_paths (&dest_path, err, dir, "config.json", ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (container->config_file == ശൂന്യ && container->config_file_content == ശൂന്യ)
    തിരികെ crun_make_error (err, 0, "config file not specified");

  എങ്കിൽ (container->config_file == ശൂന്യ)
    {
      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, container->config_file_content, strlen (container->config_file_content), err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  അല്ലെങ്കിൽ
    {
      libcrun_debug ("Reading config file: `%s`", container->config_file);
      ret = read_all_file (container->config_file, &buffer, &len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, buffer, len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  തിരികെ 0;
}

സ്ഥിതം ശൂന്യം
force_delete_container_status (libcrun_context_t *context, libcrun_container_t *container,
                               runtime_spec_schema_config_schema *def)
{
  libcrun_error_t tmp_err = ശൂന്യ;
  container_delete_internal (context, container, def, context->id, true, false, &tmp_err);
  libcrun_error_report_and_release (&tmp_err);
}

/* Serialize ERR to FD as an int errno followed by a NUL-terminated message.
   Used by forked children to report a failure to the parent process.  */
സ്ഥിതം ശൂന്യം
write_error_to_pipe (പൂർണ്ണസംഖ്യ fd, libcrun_error_t *err)
{
  സ്ഥിരം അക്ഷരം *സന്ദേശം = (*err)->സന്ദേശം;
  പൂർണ്ണസംഖ്യ errcode = crun_error_get_errno (err);

  TEMP_FAILURE_RETRY (ലിഖ (fd, &errcode, വലുപ്പം (errcode)));
  TEMP_FAILURE_RETRY (ലിഖ (fd, സന്ദേശം, strlen (സന്ദേശം) + 1));
}

പൂർണ്ണസംഖ്യ
libcrun_container_run (libcrun_context_t *context, libcrun_container_t *container, unsigned പൂർണ്ണസംഖ്യ options,
                       libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  പൂർണ്ണസംഖ്യ ret;
  പൂർണ്ണസംഖ്യ detach = context->detach;
  പൂർണ്ണസംഖ്യ container_ret_status[2];
  cleanup_close പൂർണ്ണസംഖ്യ pipefd0 = -1;
  cleanup_close പൂർണ്ണസംഖ്യ pipefd1 = -1;
  libcrun_error_t tmp_err = ശൂന്യ;

  container->context = context;

  ret = validate_options (options, LIBCRUN_RUN_OPTIONS_PREFORK | LIBCRUN_RUN_OPTIONS_KEEP, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = check_config_file (def, context, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (def->process && def->process->terminal && detach && context->console_socket == ശൂന്യ)
    തിരികെ crun_make_error (err, 0, "use --console-socket with --detach when a terminal is used");

  ret = libcrun_status_check_directories (context->state_വേര്, context->id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (! detach && (options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      ret = libcrun_copy_config_file (context->id, context->state_വേര്, container, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = libcrun_container_run_internal (container, context, ശൂന്യ, err);
      എങ്കിൽ (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
        force_delete_container_status (context, container, def);
      തിരികെ ret;
    }

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  ret = fork ();
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "fork");
  എങ്കിൽ (ret)
    {
      പൂർണ്ണസംഖ്യ status;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, ശൂന്യ, 0);

      ret = TEMP_FAILURE_RETRY (പഠ (pipefd0, &status, വലുപ്പം (status)));
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, errno, "read from sync pipe");

      എങ്കിൽ (status < 0)
        {
          പൂർണ്ണസംഖ്യ errno_;
          അക്ഷരം buf[512];
          ret = TEMP_FAILURE_RETRY (പഠ (pipefd0, &errno_, വലുപ്പം (errno_)));
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "read from sync pipe");

          ret = TEMP_FAILURE_RETRY (പഠ (pipefd0, buf, വലുപ്പം (buf) - 1));
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "read from sync pipe");
          buf[ret] = '\0';

          തിരികെ crun_make_error (err, errno_, "%s", buf);
        }

      തിരികെ status;
    }

  close_and_reset (&pipefd0);

  /* forked process.  */
  ret = detach_process ();
  എങ്കിൽ (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_വേര്, container, &tmp_err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക fail;

  ret = libcrun_container_run_internal (container, context, ശൂന്യ, &tmp_err);
  TEMP_FAILURE_RETRY (ലിഖ (pipefd1, &ret, വലുപ്പം (ret)));
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക fail;

  exit (EXIT_SUCCESS);
fail:

  എങ്കിൽ (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
    force_delete_container_status (context, container, def);
  എങ്കിൽ (tmp_err)
    {
      write_error_to_pipe (pipefd1, &tmp_err);
      crun_error_release (&tmp_err);
    }

  exit (EXIT_FAILURE);
}

പൂർണ്ണസംഖ്യ
libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container, unsigned പൂർണ്ണസംഖ്യ options,
                          libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  പൂർണ്ണസംഖ്യ ret;
  പൂർണ്ണസംഖ്യ container_ready_pipe[2];
  cleanup_close പൂർണ്ണസംഖ്യ pipefd0 = -1;
  cleanup_close പൂർണ്ണസംഖ്യ pipefd1 = -1;
  cleanup_close പൂർണ്ണസംഖ്യ exec_fifo_fd = -1;
  context->detach = 1;

  libcrun_debug ("Creating container: `%s`", context->id);
  container->context = context;

  ret = validate_options (options, LIBCRUN_CREATE_OPTIONS_PREFORK, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = check_config_file (def, context, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (def->process && def->process->terminal && context->console_socket == ശൂന്യ)
    തിരികെ crun_make_error (err, 0, "use --console-socket with create when a terminal is used");

  ret = libcrun_status_check_directories (context->state_വേര്, context->id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  exec_fifo_fd = libcrun_status_create_exec_fifo (context->state_വേര്, context->id, err);
  എങ്കിൽ (UNLIKELY (exec_fifo_fd < 0))
    തിരികെ exec_fifo_fd;

  context->fifo_exec_wait_fd = exec_fifo_fd;
  exec_fifo_fd = -1;

  എങ്കിൽ ((options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      libcrun_debug ("Running with prefork enabled");
      ret = libcrun_copy_config_file (context->id, context->state_വേര്, container, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
      ret = libcrun_container_run_internal (container, context, ശൂന്യ, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        force_delete_container_status (context, container, def);
      തിരികെ ret;
    }

  ret = pipe2 (container_ready_pipe, O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "pipe");
  pipefd0 = container_ready_pipe[0];
  pipefd1 = container_ready_pipe[1];

  ret = fork ();
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "fork");
  എങ്കിൽ (ret)
    {
      പൂർണ്ണസംഖ്യ exit_code;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, ശൂന്യ, 0);

      ret = TEMP_FAILURE_RETRY (പഠ (pipefd0, &exit_code, വലുപ്പം (exit_code)));
      എങ്കിൽ (UNLIKELY (ret <= 0))
        തിരികെ crun_make_error (err, ret < 0 ? errno : 0, "waiting for container to be ready");

      എങ്കിൽ (ret != വലുപ്പം (exit_code) || exit_code < 0)
        തിരികെ crun_make_error (err, 0, "internal error: read invalid exit code");

      എങ്കിൽ (exit_code == 0)
        തിരികെ 0;

      libcrun_debug ("Exit code is `%d`, deleting container", exit_code);
      libcrun_error_t tmp_err = ശൂന്യ;
      container_delete_internal (context, ശൂന്യ, def, context->id, true, true, &tmp_err);
      libcrun_error_report_and_release (&tmp_err);
      തിരികെ crun_make_error (err, 0, "error creating container");
    }

  /* forked process.  */
  ret = detach_process ();
  എങ്കിൽ (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_വേര്, container, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      പൂർണ്ണസംഖ്യ errcode = crun_error_get_errno (err);
      crun_error_release (err);
      libcrun_fail_with_error (errcode, "copy config file");
    }

  ret = libcrun_container_run_internal (container, context, &pipefd1, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      force_delete_container_status (context, container, def);
      libcrun_error ((*err)->status, "%s", (*err)->സന്ദേശം);
      crun_error_release (err);
      crun_set_output_handler (log_write_to_stderr, ശൂന്യ);
    }

  എങ്കിൽ (pipefd1 >= 0)
    TEMP_FAILURE_RETRY (ലിഖ (pipefd1, &ret, വലുപ്പം (ret)));
  exit (ret ? EXIT_FAILURE : 0);
}

പൂർണ്ണസംഖ്യ
libcrun_container_start (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *state_വേര് = context->state_വേര്;
  runtime_spec_schema_config_schema *def;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
  പൂർണ്ണസംഖ്യ ret;

  ret = libcrun_read_container_status (&status, state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_is_container_running (&status, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (! ret)
    തിരികെ crun_make_error (err, 0, "container `%s` is not running", id);

  ret = read_container_config_from_state (&container, state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (context->notify_socket)
    {
      ret = get_notify_fd (context, container, &fd, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  ret = libcrun_status_write_exec_fifo (context->state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  def = container->container_def;

  എങ്കിൽ (context->notify_socket)
    {
      എങ്കിൽ (fd >= 0)
        {
          fd_set read_set;

          ആകുമ്പോൾ (1)
            {
              ഘടന timeval timeout = {
                .tv_sec = 0,
                .tv_usec = 10000,
              };
              FD_ZERO (&read_set);
              FD_SET (fd, &read_set);

              ret = select (fd + 1, &read_set, ശൂന്യ, ശൂന്യ, &timeout);
              എങ്കിൽ (UNLIKELY (ret < 0))
                തിരികെ crun_make_error (err, errno, "select failed");
              എങ്കിൽ (ret)
                {
                  ret = handle_notify_socket (fd, err);
                  എങ്കിൽ (UNLIKELY (ret < 0))
                    തിരികെ ret;
                  എങ്കിൽ (ret)
                    നിർത്തുക;
                }
              അല്ലെങ്കിൽ
                {
                  ret = libcrun_is_container_running (&status, err);
                  എങ്കിൽ (UNLIKELY (ret < 0))
                    തിരികെ ret;
                  എങ്കിൽ (! ret)
                    തിരികെ 0;
                }
            }
        }
    }

  /* The container is considered running only after we got the notification from the
     notify_socket, if any.  */
  എങ്കിൽ (def->hooks && def->hooks->poststart_len)
    {
      cleanup_close പൂർണ്ണസംഖ്യ hooks_out_fd = -1;
      cleanup_close പൂർണ്ണസംഖ്യ hooks_err_fd = -1;

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = do_hooks (def, status.pid, context->id, false, status.bundle, status.bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      എങ്കിൽ (UNLIKELY (ret != 0))
        {
          libcrun_error_t tmp_err = ശൂന്യ;
          container_delete_internal (context, container, def, id, true, true, &tmp_err);
          libcrun_error_report_and_release (&tmp_err);
          തിരികെ ret;
        }
    }

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_get_container_state_string (സ്ഥിരം അക്ഷരം *id, libcrun_container_status_t *status, സ്ഥിരം അക്ഷരം *state_വേര്,
                                    സ്ഥിരം അക്ഷരം **container_status, പൂർണ്ണസംഖ്യ *running, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret, has_fifo = 0;
  bool paused = false;

  ret = libcrun_is_container_running (status, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;
  *running = ret;

  എങ്കിൽ (*running)
    {
      ret = libcrun_status_has_read_exec_fifo (state_വേര്, id, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
      has_fifo = ret;
    }

  എങ്കിൽ (*running && ! has_fifo)
    {
      cleanup_cgroup_status ഘടന libcrun_cgroup_status *cgroup_status = ശൂന്യ;

      cgroup_status = libcrun_cgroup_make_status (status);

      ret = libcrun_cgroup_is_container_paused (cgroup_status, &paused, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          /*
            The cgroup might have been cleaned up by the time we try to read it, ignore both
            ENOENT and ENODEV:
            - ENOENT: if the open(CGROUP_PATH) fails because the cgroup was deleted.
            - ENODEV: if the cgroup is deleted between the open and reading the freeze status.
          */

          errno = crun_error_get_errno (err);
          എങ്കിൽ (errno == ENOENT || errno == ENODEV)
            {
              crun_error_release (err);
              *container_status = "stopped";
              തിരികെ 0;
            }

          തിരികെ ret;
        }
    }

  എങ്കിൽ (! *running)
    *container_status = "stopped";
  അല്ലെങ്കിൽ എങ്കിൽ (has_fifo)
    *container_status = "created";
  അല്ലെങ്കിൽ എങ്കിൽ (paused)
    *container_status = "paused";
  അല്ലെങ്കിൽ
    *container_status = "running";

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_container_state_json (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, അക്ഷരം **out, libcrun_error_t *err)
{
  അക്ഷരം *buffer = ശൂന്യ;
  size_t len = 0;
  FILE *f;
  പൂർണ്ണസംഖ്യ ret;

  f = open_memstream (&buffer, &len);
  എങ്കിൽ (f == ശൂന്യ)
    തിരികെ crun_make_error (err, errno, "open_memstream");

  ret = libcrun_container_state (context, id, f, err);
  fclose (f);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      സ്വതന്ത്രം (buffer);
      തിരികെ ret;
    }

  *out = buffer;
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_container_state (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, FILE *out, libcrun_error_t *err)
{
  സ്ഥിരം അക്ഷരം *സ്ഥിരം OCI_CONFIG_VERSION = "1.0.0";
  libcrun_container_status_t status = {};
  സ്ഥിരം അക്ഷരം *state_വേര് = context->state_വേര്;
  സ്ഥിരം അക്ഷരം *container_status = ശൂന്യ;
  json_gen_ctx *gen = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *buf;
  പൂർണ്ണസംഖ്യ ret = 0;
  പൂർണ്ണസംഖ്യ r = json_gen_status_ok;
  പൂർണ്ണസംഖ്യ running;
  size_t len;

  ret = libcrun_read_container_status (&status, state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_get_container_state_string (id, &status, state_വേര്, &container_status, &running, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക exit;

  ret = 0;
  എങ്കിൽ (! json_gen_init (&gen, ശൂന്യ))
    തിരികെ crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));
  GEN_താക്കോൽ (gen, "ociVersion");
  GEN_STR (gen, OCI_CONFIG_VERSION);

  GEN_താക്കോൽ (gen, "id");
  GEN_STR (gen, id);

  GEN_താക്കോൽ (gen, "pid");
  GEN_OR_FAIL (map_int (gen, running ? status.pid : 0));

  GEN_താക്കോൽ (gen, "status");
  GEN_STR (gen, container_status);

  GEN_താക്കോൽ (gen, "bundle");
  GEN_STR (gen, status.bundle);

  GEN_താക്കോൽ (gen, "rootfs");
  GEN_STR (gen, status.rootfs);

  GEN_താക്കോൽ (gen, "created");
  GEN_STR (gen, status.created);

  എങ്കിൽ (status.scope)
    {
      GEN_താക്കോൽ (gen, "systemd-scope");
      GEN_STR (gen, status.scope);
    }

  എങ്കിൽ (status.owner)
    {
      GEN_താക്കോൽ (gen, "owner");
      GEN_STR (gen, status.owner);
    }

  {
    cleanup_container libcrun_container_t *container = ശൂന്യ;

    ret = read_container_config_from_state (&container, state_വേര്, id, err);
    എങ്കിൽ (UNLIKELY (ret < 0))
      പോകുക exit;

    എങ്കിൽ (container->container_def->annotations && container->container_def->annotations->len)
      GEN_OR_FAIL (gen_annotations (gen, container->container_def->annotations));
  }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  fprintf (out, "%s\n", buf);

exit:
  എങ്കിൽ (gen)
    json_gen_free (gen);
  libcrun_free_container_status (&status);
  തിരികെ ret;

gen_error:
  ret = json_gen_error_to_crun_error (r, err);
  പോകുക exit;
}

പൂർണ്ണസംഖ്യ
libcrun_container_exec (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, runtime_spec_schema_config_schema_process *process,
                        libcrun_error_t *err)
{
  ഘടന libcrun_container_exec_options_s opts;
  memset (&opts, 0, വലുപ്പം (opts));

  opts.struct_size = വലുപ്പം (opts);
  opts.process = process;

  തിരികെ libcrun_container_exec_with_options (context, id, &opts, err);
}

പൂർണ്ണസംഖ്യ
libcrun_container_exec_process_file (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err)
{
  ഘടന libcrun_container_exec_options_s opts;
  memset (&opts, 0, വലുപ്പം (opts));

  opts.struct_size = വലുപ്പം (opts);
  opts.path = path;

  തിരികെ libcrun_container_exec_with_options (context, id, &opts, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
exec_from_process_json (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *process_json, സ്ഥിരം അക്ഷരം *cgroup,
                        bool merge_env, libcrun_error_t *err)
{
  ഘടന parser_context parser_ctx = { .options = 0, .errfile = stderr };
  ഘടന libcrun_container_exec_options_s opts;
  runtime_spec_schema_config_schema_process *process;
  parser_error parser_err = ശൂന്യ;
  json_object *doc = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;

  ret = parse_json_file (&doc, process_json, &parser_ctx, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  process = make_runtime_spec_schema_config_schema_process (doc, &parser_ctx, &parser_err);
  json_object_put (doc);
  എങ്കിൽ (process == ശൂന്യ)
    {
      ret = crun_make_error (err, 0, "cannot parse process: `%s`", parser_err ? parser_err : "unknown error");
      സ്വതന്ത്രം (parser_err);
      തിരികെ ret;
    }

  memset (&opts, 0, വലുപ്പം (opts));
  opts.struct_size = വലുപ്പം (opts);
  opts.process = process;
  opts.cgroup = cgroup;
  opts.merge_env = merge_env;

  ret = libcrun_container_exec_with_options (context, id, &opts, err);
  free_runtime_spec_schema_config_schema_process (process);
  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
libcrun_container_exec_json (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *process_json, libcrun_error_t *err)
{
  തിരികെ exec_from_process_json (context, id, process_json, ശൂന്യ, false, err);
}

പൂർണ്ണസംഖ്യ
libcrun_container_exec_with_options_json (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id,
                                          സ്ഥിരം ഘടന libcrun_exec_options_s *opts, libcrun_error_t *err)
{
  എങ്കിൽ (opts == ശൂന്യ || opts->struct_size == 0)
    തിരികെ crun_make_error (err, EINVAL, "invalid exec options");

  തിരികെ exec_from_process_json (context, id, opts->process_json, opts->cgroup, opts->merge_env, err);
}

#define cleanup_process_schema __attribute__ ((cleanup (cleanup_process_schemap)))

സ്ഥിതം വരിയിൽ ശൂന്യം
cleanup_process_schemap (runtime_spec_schema_config_schema_process **p)
{
  runtime_spec_schema_config_schema_process *process = *p;
  എങ്കിൽ (process)
    (ശൂന്യം) free_runtime_spec_schema_config_schema_process (process);
}

/* Build the seccomp receiver payload (when needed), apply the seccomp profile and
   close the seccomp file descriptors.  Used from exec_process_entrypoint both before
   and after the capabilities are set, depending on process->no_new_privileges.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
apply_seccomp_for_exec (libcrun_container_t *container, pid_t own_pid, പൂർണ്ണസംഖ്യ seccomp_fd,
                        പൂർണ്ണസംഖ്യ seccomp_receiver_fd, അക്ഷരം **seccomp_flags, size_t seccomp_flags_len,
                        libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *seccomp_fd_payload = ശൂന്യ;
  size_t seccomp_fd_payload_len = 0;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (seccomp_receiver_fd >= 0)
    {
      ret = get_seccomp_receiver_fd_payload (container, "running", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  ret = libcrun_apply_seccomp (seccomp_fd, seccomp_receiver_fd, seccomp_fd_payload,
                               seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  close_and_reset (&seccomp_fd);
  close_and_reset (&seccomp_receiver_fd);
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
exec_process_entrypoint (libcrun_context_t *context,
                         libcrun_container_t *container,
                         runtime_spec_schema_config_schema_process *process,
                         പൂർണ്ണസംഖ്യ *pipefd1,
                         പൂർണ്ണസംഖ്യ seccomp_fd,
                         പൂർണ്ണസംഖ്യ seccomp_receiver_fd,
                         ഘടന custom_handler_instance_s *custom_handler,
                         bool merge_env,
                         libcrun_error_t *err)
{
  runtime_spec_schema_config_schema_process_capabilities *capabilities = ശൂന്യ;
  cleanup_free അക്ഷരം *exec_path = ശൂന്യ;
  uid_t container_uid;
  gid_t container_gid;
  സ്ഥിരം അക്ഷരം *cwd;
  bool chdir_done = false;
  size_t seccomp_flags_len = 0;
  അക്ഷരം **seccomp_flags = ശൂന്യ;
  pid_t own_pid = 0;
  size_t i;
  പൂർണ്ണസംഖ്യ ret;

  container_uid = process->user ? process->user->uid : 0;
  container_gid = process->user ? process->user->gid : 0;

  TEMP_FAILURE_RETRY (പഠ (*pipefd1, &own_pid, വലുപ്പം (own_pid)));

  cwd = process->cwd ? process->cwd : "/";
  എങ്കിൽ (LIKELY (libcrun_safe_chdir (cwd, err) == 0))
    chdir_done = true;
  അല്ലെങ്കിൽ
    crun_error_release (err);

  ret = unblock_signals (err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = clearenv ();
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "clearenv");

  എങ്കിൽ ((merge_env || ! process->env_len) && container->container_def->process && container->container_def->process->env_len)
    {
      അക്ഷരം *e;

      വേണ്ടി (i = 0; i < container->container_def->process->env_len; i++)
        {
          e = container->container_def->process->env[i];
          എങ്കിൽ (putenv (e) < 0)
            തിരികെ crun_make_error (err, errno, "putenv `%s`", e);
        }
    }
  എങ്കിൽ (process->env_len)
    {
      വേണ്ടി (i = 0; i < process->env_len; i++)
        എങ്കിൽ (putenv (process->env[i]) < 0)
          തിരികെ crun_make_error (err, errno, "putenv `%s`", process->env[i]);
    }

  എങ്കിൽ (is_empty_string (getenv ("HOME")))
    {
      ret = set_home_env (container_uid);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          എങ്കിൽ (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            തിരികെ crun_make_error (err, errno, "setenv HOME");
        }
    }

  ret = libcrun_set_selinux_label (container, process, false, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_set_apparmor_profile (container, process, false, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (container->container_def->linux && container->container_def->linux->seccomp)
    {
      seccomp_flags = container->container_def->linux->seccomp->flags;
      seccomp_flags_len = container->container_def->linux->seccomp->flags_len;
    }

  ret = find_executable (&exec_path, process->args[0], process->cwd, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      എങ്കിൽ (crun_error_get_errno (err) == ENOENT)
        തിരികെ ret;

      /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
         once the process switched to the use that runs in the container.  This might be necessary
         when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
         is not honored.  */
      crun_error_release (err);
    }

  എങ്കിൽ (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, false, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_reopen_dev_null (err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (! process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  ret = libcrun_container_setgroups (container, process, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = maybe_chown_std_streams (container_uid, container_gid, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (process->capabilities)
    capabilities = process->capabilities;
  അല്ലെങ്കിൽ എങ്കിൽ (container->container_def->process)
    capabilities = container->container_def->process->capabilities;

  ret = libcrun_set_caps (capabilities, container_uid, container_gid, process->no_new_privileges,
                          custom_handler != ശൂന്യ, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (UNLIKELY (exec_path == ശൂന്യ))
    {
      ret = find_executable (&exec_path, process->args[0], process->cwd, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          എങ്കിൽ (custom_handler == ശൂന്യ || is_empty_string (process->args[0]))
            തിരികെ ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          exec_path = xstrdup (process->args[0]);
        }
    }

  എങ്കിൽ (! chdir_done)
    {
      ret = libcrun_safe_chdir (cwd, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (process->user)
    umask (process->user->umask_present ? process->user->umask : 0022);

  ret = 0;
  TEMP_FAILURE_RETRY (ലിഖ (*pipefd1, &ret, വലുപ്പം (ret)));
  TEMP_FAILURE_RETRY (close (*pipefd1));
  *pipefd1 = -1;

  എങ്കിൽ (custom_handler)
    {
      എങ്കിൽ (custom_handler->vtable->exec_func == ശൂന്യ)
        തിരികെ crun_make_error (err, 0, "the handler does not support exec");

      എങ്കിൽ (context->argv)
        erase_initial_environment (context->argv, context->argc);

      ret = custom_handler->vtable->exec_func (custom_handler->cookie,
                                               container,
                                               exec_path,
                                               process->args);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ crun_make_error (err, -ret, "exec container process failed with handler as `%s`", custom_handler->vtable->name);

      _safe_exit (EXIT_FAILURE);
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation, because the seccomp filter is already in place and it could
     stop some syscalls used by mark_or_close_fds_ge_than.
  */
  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, true, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, process->args));
  libcrun_fail_with_error (errno, "exec `%s`", exec_path);
}

/* Close the file descriptors inherited from the caller, other than those
   to be passed to the container process.  */
പൂർണ്ണസംഖ്യ
libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err)
{
  തിരികെ mark_or_close_fds_ge_than (ശൂന്യ, context->preserve_fds + 3, true, err);
}

പൂർണ്ണസംഖ്യ
libcrun_load_process_from_file (സ്ഥിരം അക്ഷരം *path, runtime_spec_schema_config_schema_process **process,
                                libcrun_error_t *err)
{
  ഘടന parser_context ctx = { 0, stderr };
  cleanup_free അക്ഷരം *content = ശൂന്യ;
  parser_error parser_err = ശൂന്യ;
  json_object *doc = ശൂന്യ;
  size_t len;
  പൂർണ്ണസംഖ്യ ret;

  ret = read_all_file (path, &content, &len, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  *process = make_runtime_spec_schema_config_schema_process (doc, &ctx, &parser_err);
  എങ്കിൽ (UNLIKELY (*process == ശൂന്യ))
    {
      ret = crun_make_error (err, 0, "cannot parse process file: `%s`", parser_err);
      സ്വതന്ത്രം (parser_err);
      എങ്കിൽ (doc)
        json_object_put (doc);
      തിരികെ ret;
    }

  സ്വതന്ത്രം (parser_err);
  എങ്കിൽ (doc)
    json_object_put (doc);

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_container_exec_with_options (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id,
                                     ഘടന libcrun_container_exec_options_s *opts,
                                     libcrun_error_t *err)
{
  cleanup_custom_handler_instance ഘടന custom_handler_instance_s *custom_handler = ശൂന്യ;
  പൂർണ്ണസംഖ്യ container_status, ret;
  bool container_paused = false;
  pid_t pid;
  libcrun_container_status_t status = {};
  സ്ഥിരം അക്ഷരം *state_വേര് = context->state_വേര്;
  cleanup_close പൂർണ്ണസംഖ്യ terminal_fd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ seccomp_fd = -1;
  cleanup_terminal ശൂന്യം *orig_terminal = ശൂന്യ;
  cleanup_container libcrun_container_t *container = ശൂന്യ;
  പൂർണ്ണസംഖ്യ container_ret_status[2];
  cleanup_close പൂർണ്ണസംഖ്യ pipefd0 = -1;
  cleanup_close പൂർണ്ണസംഖ്യ pipefd1 = -1;
  cleanup_close പൂർണ്ണസംഖ്യ seccomp_receiver_fd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ own_seccomp_receiver_fd = -1;
  cleanup_close പൂർണ്ണസംഖ്യ seccomp_notify_fd = -1;
  സ്ഥിരം അക്ഷരം *seccomp_notify_plugins = ശൂന്യ;
  __attribute__ ((unused)) cleanup_process_schema runtime_spec_schema_config_schema_process *process_cleanup = ശൂന്യ;
  runtime_spec_schema_config_schema_process *process = opts->process;
  ഘടന libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  പൂർണ്ണസംഖ്യ ret_from_child = 0;

  ret = libcrun_read_container_status (&status, state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_is_container_running (&status, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;
  container_status = ret;

  ret = read_container_config_from_state (&container, state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  container->context = context;

  എങ്കിൽ (container_status == 0)
    തിരികെ crun_make_error (err, 0, "the container `%s` is not running", id);

  {
    cleanup_cgroup_status ഘടന libcrun_cgroup_status *cgroup_status = ശൂന്യ;

    cgroup_status = libcrun_cgroup_make_status (&status);

    ret = libcrun_cgroup_is_container_paused (cgroup_status, &container_paused, err);
    എങ്കിൽ (UNLIKELY (ret < 0))
      തിരികെ ret;
  }

  എങ്കിൽ (UNLIKELY (container_paused))
    തിരികെ crun_make_error (err, 0, "cannot exec in a paused container `%s`", id);

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = block_signals (err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  libcrun_seccomp_gen_ctx_init (&seccomp_gen_ctx, container, false, 0);

  ret = libcrun_open_seccomp_bpf (&seccomp_gen_ctx, &seccomp_fd, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &seccomp_receiver_fd,
                                     &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (വലുപ്പം (*opts) != opts->struct_size)
    തിരികെ crun_make_error (err, EINVAL, "invalid libcrun_container_exec_options_s struct");

  എങ്കിൽ (opts->path)
    {
      എങ്കിൽ (process)
        തിരികെ crun_make_error (err, EINVAL, "cannot specify both exec file and options");

      ret = libcrun_load_process_from_file (opts->path, &process, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      process_cleanup = process;
    }

  /* This must be done before we enter a user namespace.  If the process does
     not specify any rlimits, use the ones from the container configuration.  */
  എങ്കിൽ (process->rlimits_len == 0 && container->container_def->process)
    ret = libcrun_set_rlimits (container->container_def->process->rlimits,
                               container->container_def->process->rlimits_len, err);
  അല്ലെങ്കിൽ
    ret = libcrun_set_rlimits (process->rlimits, process->rlimits_len, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_set_mempolicy (container->container_def, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  /* If the new process block doesn't specify a SELinux label, AppArmor profile, user,
     I/O priority or scheduler, then use the configuration from the original config file.  */
  എങ്കിൽ (container->container_def->process)
    {
      എങ്കിൽ (process->io_priority == ശൂന്യ && container->container_def->process->io_priority)
        {
          process->io_priority = clone_runtime_spec_schema_config_schema_process_io_priority (container->container_def->process->io_priority);
          എങ്കിൽ (process->io_priority == ശൂന്യ)
            OOM ();
        }

      എങ്കിൽ (process->scheduler == ശൂന്യ && container->container_def->process->scheduler)
        {
          process->scheduler = clone_runtime_spec_schema_config_schema_process_scheduler (container->container_def->process->scheduler);
          എങ്കിൽ (process->scheduler == ശൂന്യ)
            OOM ();
        }

      എങ്കിൽ (process->selinux_label == ശൂന്യ && container->container_def->process->selinux_label)
        process->selinux_label = xstrdup (container->container_def->process->selinux_label);

      എങ്കിൽ (process->apparmor_profile == ശൂന്യ && container->container_def->process->apparmor_profile)
        process->apparmor_profile = xstrdup (container->container_def->process->apparmor_profile);

      എങ്കിൽ (process->user == ശൂന്യ && container->container_def->process->user)
        {
          process->user = clone_runtime_spec_schema_config_schema_process_user (container->container_def->process->user);
          എങ്കിൽ (process->user == ശൂന്യ)
            OOM ();
        }
    }

  ret = initialize_security (container, process, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  /* Do not leak the session keyring of the caller into the container.  The
     keyring created for the container cannot be joined, so use an anonymous one.  */
  എങ്കിൽ (! context->no_new_keyring && ! status.no_new_keyring)
    {
      libcrun_debug ("Creating new keyring for the exec process");

      ret = libcrun_create_keyring (container, ശൂന്യ, process->selinux_label, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "prctl unset dumpable");

  pid = libcrun_join_process (context, container, status.pid, &status, opts->cgroup, context->detach,
                              process, process->terminal ? &terminal_fd : ശൂന്യ, err);
  എങ്കിൽ (UNLIKELY (pid < 0))
    തിരികെ pid;

  /* Process to exec.  */
  എങ്കിൽ (pid == 0)
    {
      TEMP_FAILURE_RETRY (close (pipefd0));
      pipefd0 = -1;

      exec_process_entrypoint (context, container, process, &pipefd1, seccomp_fd, seccomp_receiver_fd, custom_handler, opts->merge_env, err);
      /* It gets here only on errors.  */
      എങ്കിൽ (*err)
        {
          എങ്കിൽ (pipefd1 < 0)
            libcrun_fail_with_error ((*err)->status, "%s", (*err)->സന്ദേശം);
          അല്ലെങ്കിൽ
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

  TEMP_FAILURE_RETRY (ലിഖ (pipefd0, &pid, വലുപ്പം (pid)));

  എങ്കിൽ (seccomp_fd >= 0)
    close_and_reset (&seccomp_fd);

  എങ്കിൽ (terminal_fd >= 0)
    {
      unsigned short rows = 0, cols = 0;

      എങ്കിൽ (process->console_size)
        {
          cols = process->console_size->width;
          rows = process->console_size->height;
        }

      ret = libcrun_terminal_setup_size (terminal_fd, rows, cols, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      എങ്കിൽ (context->console_socket)
        {
          പൂർണ്ണസംഖ്യ ret;
          cleanup_close പൂർണ്ണസംഖ്യ console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
          എങ്കിൽ (UNLIKELY (console_socket_fd < 0))
            തിരികെ console_socket_fd;
          ret = send_fd_to_socket (console_socket_fd, terminal_fd, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
          close_and_reset (&terminal_fd);
        }
      അല്ലെങ്കിൽ
        {
          ret = libcrun_clear_onlcr (terminal_fd, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              തിരികെ ret;
            }

          ret = libcrun_set_raw (0, &orig_terminal, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              തിരികെ ret;
            }
        }
    }

  ret = TEMP_FAILURE_RETRY (പഠ (pipefd0, &ret_from_child, വലുപ്പം (ret_from_child)));
  എങ്കിൽ (ret != വലുപ്പം (ret_from_child))
    ret = crun_make_error (err, 0, "read pipe failed");
  അല്ലെങ്കിൽ എങ്കിൽ (ret_from_child != 0)
    {
      cleanup_free അക്ഷരം *സന്ദേശം = ശൂന്യ;
      size_t len = 0;

      ret = read_all_fd (pipefd0, "error stream", &സന്ദേശം, &len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
      /* the string from read_all_fd is always NUL terminated.  */
      ret = crun_make_error (err, ret_from_child, "%s", സന്ദേശം);
    }
  അല്ലെങ്കിൽ
    {
      /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
      എങ്കിൽ (own_seccomp_receiver_fd >= 0)
        {
          seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
          എങ്കിൽ (UNLIKELY (seccomp_notify_fd < 0))
            തിരികെ seccomp_notify_fd;

          ret = close_and_reset (&own_seccomp_receiver_fd);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ crun_make_error (err, errno, "close seccomp receiver fd failed");
        }

      {
        ഘടന wait_for_process_args args = {
          .pid = pid,
          .context = context,
          .terminal_fd = terminal_fd,
          .notify_socket = -1,
          .container_ready_fd = ശൂന്യ,
          .seccomp_notify_fd = seccomp_notify_fd,
          .seccomp_notify_plugins = seccomp_notify_plugins,
        };

        ret = wait_for_process (&args, err);
      }
    }

  flush_fd_to_err (context, terminal_fd);
  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
libcrun_container_update (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *content, size_t len arg_unused,
                          libcrun_error_t *err)
{
  cleanup_custom_handler_instance ഘടന custom_handler_instance_s *custom_handler = ശൂന്യ;
  runtime_spec_schema_config_linux_resources *resources = ശൂന്യ;
  cleanup_container libcrun_container_t *container = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *state_വേര് = context->state_വേര്;
  ഘടന parser_context ctx = { 0, stderr };
  libcrun_container_status_t status = {};
  parser_error parser_err = ശൂന്യ;
  json_object *doc = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;

  ret = libcrun_read_container_status (&status, state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = read_container_config_from_state (&container, state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  resources = make_runtime_spec_schema_config_linux_resources (doc, &ctx, &parser_err);
  എങ്കിൽ (UNLIKELY (resources == ശൂന്യ))
    {
      ret = crun_make_error (err, 0, "cannot parse resources: %s", parser_err);
      പോകുക cleanup;
    }

  എങ്കിൽ (custom_handler && custom_handler->vtable->modify_oci_configuration)
    {
      /* Adapt RESOURCES to be used from the modify_oci_configuration hook.  */
      cleanup_free runtime_spec_schema_config_linux *linux = xmalloc0 (വലുപ്പം (*linux));
      cleanup_free runtime_spec_schema_config_schema *def = xmalloc0 (വലുപ്പം (*def));

      def->linux = linux;
      linux->resources = resources;

      ret = custom_handler->vtable->modify_oci_configuration (custom_handler->cookie,
                                                              context,
                                                              def,
                                                              err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        പോകുക cleanup;
    }

  ret = libcrun_linux_container_update (&status, state_വേര്, resources, err);

cleanup:
  എങ്കിൽ (doc)
    json_object_put (doc);
  സ്വതന്ത്രം (parser_err);
  എങ്കിൽ (resources)
    free_runtime_spec_schema_config_linux_resources (resources);

  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
libcrun_container_update_from_file (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *file, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *content = ശൂന്യ;
  size_t len;
  പൂർണ്ണസംഖ്യ ret;

  ret = read_all_file (file, &content, &len, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ libcrun_container_update (context, id, content, len, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
compare_update_മൂല്യങ്ങൾ (സ്ഥിരം ശൂന്യം *a, സ്ഥിരം ശൂന്യം *b)
{
  സ്ഥിരം ഘടന libcrun_update_മൂല്യം_s *aa = a;
  സ്ഥിരം ഘടന libcrun_update_മൂല്യം_s *bb = b;
  പൂർണ്ണസംഖ്യ ret;

  ret = strcmp (aa->section, bb->section);
  എങ്കിൽ (ret)
    തിരികെ ret;
  തിരികെ strcmp (aa->name, bb->name);
}

പൂർണ്ണസംഖ്യ
libcrun_container_update_from_മൂല്യങ്ങൾ (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id,
                                      ഘടന libcrun_update_മൂല്യം_s *മൂല്യങ്ങൾ, size_t len,
                                      libcrun_error_t *err)
{
  സ്ഥിരം അക്ഷരം *current_section = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *buf;
  json_gen_ctx *gen = ശൂന്യ;
  size_t i, buf_len;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (! json_gen_init (&gen, ശൂന്യ))
    തിരികെ crun_make_error (err, 0, "json_gen_init failed");
  json_gen_map_open (gen);

  qsort (മൂല്യങ്ങൾ, len, വലുപ്പം (ഘടന libcrun_update_മൂല്യം_s), compare_update_മൂല്യങ്ങൾ);

  വേണ്ടി (i = 0; i < len; i++)
    {
      എങ്കിൽ (current_section == ശൂന്യ || strcmp (മൂല്യങ്ങൾ[i].section, current_section))
        {
          എങ്കിൽ (i > 0)
            json_gen_map_close (gen);

          current_section = മൂല്യങ്ങൾ[i].section;
          json_gen_string (gen, current_section, strlen (current_section));
          json_gen_map_open (gen);
        }

      json_gen_string (gen, മൂല്യങ്ങൾ[i].name, strlen (മൂല്യങ്ങൾ[i].name));

      എങ്കിൽ (മൂല്യങ്ങൾ[i].numeric)
        json_gen_സംഖ്യ (gen, മൂല്യങ്ങൾ[i].മൂല്യം, strlen (മൂല്യങ്ങൾ[i].മൂല്യം));
      അല്ലെങ്കിൽ
        json_gen_string (gen, മൂല്യങ്ങൾ[i].മൂല്യം, strlen (മൂല്യങ്ങൾ[i].മൂല്യം));
    }
  എങ്കിൽ (len)
    json_gen_map_close (gen);

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);

  ret = libcrun_container_update (context, id, buf, buf_len, err);

  json_gen_free (gen);

  തിരികെ ret;
}

സ്ഥിതം ശൂന്യം
populate_array_field (അക്ഷരം ***field, അക്ഷരം *array[], size_t അക്കം_elements)
{
  size_t i;

  *field = xmalloc0 ((അക്കം_elements + 1) * വലുപ്പം (അക്ഷരം *));
  വേണ്ടി (i = 0; i < അക്കം_elements; i++)
    (*field)[i] = xstrdup (array[i]);

  (*field)[i] = ശൂന്യ;
}

#ifdef HAVE_CAP
സ്ഥിതം ശൂന്യം
populate_capabilities (ഘടന features_info_s *info, അക്ഷരം ***capabilities, size_t *അക്കം_capabilities)
{
  size_t index = 0;
  cap_മൂല്യം_t i;
  അക്ഷരം *endptr;
  പൂർണ്ണസംഖ്യ j;

  *അക്കം_capabilities = 0;
  വേണ്ടി (i = 0;; i++)
    {
      അക്ഷരം *v = cap_to_name (i);
      എങ്കിൽ (v == ശൂന്യ)
        നിർത്തുക;
      strtol (v, &endptr, 10);
      എങ്കിൽ (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          നിർത്തുക;
        }
      (*അക്കം_capabilities)++;
    }

  *capabilities = xmalloc0 ((*അക്കം_capabilities + 1) * വലുപ്പം (സ്ഥിരം അക്ഷരം *));
  വേണ്ടി (i = 0; i < (cap_മൂല്യം_t) *അക്കം_capabilities; i++)
    {
      അക്ഷരം *v = cap_to_name (i);
      എങ്കിൽ (v == ശൂന്യ)
        നിർത്തുക;
      strtol (v, &endptr, 10);
      എങ്കിൽ (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          നിർത്തുക;
        }

      // Convert capability name to uppercase
      വേണ്ടി (j = 0; v[j] != '\0'; j++)
        v[j] = toupper (v[j]);

      (*capabilities)[index] = v;
      index++;
    }

  (*capabilities)[index] = ശൂന്യ; // Terminate the array with NULL
  populate_array_field (&(info->linux.capabilities), *capabilities, *അക്കം_capabilities);
}
#endif

സ്ഥിതം ശൂന്യം
retrieve_mount_options (ഘടന features_info_s **info)
{
  cleanup_free സ്ഥിരം ഘടന propagation_flags_s *mount_options_list = ശൂന്യ;
  size_t അക്കം_mount_options = 0;

  // Retrieve mount options from wordlist
  mount_options_list = get_mount_flags_from_wordlist ();

  // Calculate the number of mount options
  ആകുമ്പോൾ (mount_options_list[അക്കം_mount_options].name != ശൂന്യ)
    അക്കം_mount_options++;

  // Allocate memory for mount options in info struct
  (*info)->mount_options = xmalloc0 ((അക്കം_mount_options + 1) * വലുപ്പം (അക്ഷരം *));

  // Copy mount options to info struct
  വേണ്ടി (size_t i = 0; i < അക്കം_mount_options; i++)
    (*info)->mount_options[i] = xstrdup (mount_options_list[i].name);
}

പൂർണ്ണസംഖ്യ
libcrun_container_get_features (libcrun_context_t *context, ഘടന features_info_s **info, libcrun_error_t *err arg_unused)
{
  // Allocate memory for the features_info_s structure
  size_t അക്കം_namspaces = വലുപ്പം (namespaces) / വലുപ്പം (namespaces[0]);
  size_t അക്കം_operators = വലുപ്പം (operators) / വലുപ്പം (operators[0]);
  size_t അക്കം_actions = വലുപ്പം (actions) / വലുപ്പം (actions[0]);
  size_t അക്കം_hooks = വലുപ്പം (hooks) / വലുപ്പം (hooks[0]);
  size_t അക്കം_archs = വലുപ്പം (archs) / വലുപ്പം (archs[0]);
  size_t അക്കം_unsafe_annotations = വലുപ്പം (potentially_unsafe_annotations) / വലുപ്പം (potentially_unsafe_annotations[0]);
  cleanup_free അക്ഷരം **capabilities = ശൂന്യ;
  size_t അക്കം_capabilities = 0;
  size_t അക്കം_mempolicy_modes = വലുപ്പം (mempolicy_modes) / വലുപ്പം (mempolicy_modes[0]);
  size_t അക്കം_mempolicy_flags = വലുപ്പം (mempolicy_flags) / വലുപ്പം (mempolicy_flags[0]);

  *info = xmalloc0 (വലുപ്പം (ഘടന features_info_s));

  // Hardcoded feature information
  (*info)->oci_version_min = xstrdup ("1.0.0");
  (*info)->oci_version_max = xstrdup ("1.1.0+dev");

  // Populate hooks
  populate_array_field (&((*info)->hooks), hooks, അക്കം_hooks);

  // Populate mount_options
  retrieve_mount_options (info);

  // Populate namespaces
  populate_array_field (&((*info)->linux.namespaces), namespaces, അക്കം_namspaces);

#ifdef HAVE_CAP
  // Populate capabilities
  populate_capabilities (*info, &capabilities, &അക്കം_capabilities);
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
  populate_array_field (&((*info)->linux.seccomp.actions), actions, അക്കം_actions);
  // Populate operators
  populate_array_field (&((*info)->linux.seccomp.operators), operators, അക്കം_operators);
  // Populate archs
  populate_array_field (&((*info)->linux.seccomp.archs), archs, അക്കം_archs);
#അല്ലെങ്കിൽ
  (*info)->linux.seccomp.enabled = false;
#endif

  // Put values for apparmor and selinux
  (*info)->linux.apparmor.enabled = true;
  (*info)->linux.selinux.enabled = true;

  (*info)->linux.intel_rdt.enabled = true;

  (*info)->linux.net_devices.enabled = true;

  populate_array_field (&((*info)->linux.memory_policy.mode), mempolicy_modes, അക്കം_mempolicy_modes);
  populate_array_field (&((*info)->linux.memory_policy.flags), mempolicy_flags, അക്കം_mempolicy_flags);

  // Put the values for mount extensions
  (*info)->linux.mount_ext.idmap.enabled = true;

  // Populate the values for annotations
#ifdef HAVE_SECCOMP
  {
    സ്ഥിരം ഘടന scmp_version *version = seccomp_version ();
    അക്ഷരം *version_string = ശൂന്യ;

    xasprintf (&version_string, "%u.%u.%u", version->major, version->minor, version->micro);
    (*info)->annotations.io_github_seccomp_libseccomp_version = version_string;
  }
#endif

  എങ്കിൽ (context->handler_manager && handler_by_name (context->handler_manager, "wasm"))
    (*info)->annotations.run_oci_crun_wasm = true;

#എങ്കിൽ HAVE_CRIU
  (*info)->annotations.run_oci_crun_checkpoint_enabled = true;
#endif
  (*info)->annotations.run_oci_crun_commit = GIT_VERSION;
  (*info)->annotations.run_oci_crun_version = PACKAGE_VERSION;

  populate_array_field (&((*info)->potentially_unsafe_annotations), potentially_unsafe_annotations, അക്കം_unsafe_annotations);

  തിരികെ 0;
}

സ്ഥിതം ശൂന്യം
features_add_string (json_gen_ctx *json_gen, സ്ഥിരം അക്ഷരം *താക്കോൽ, സ്ഥിരം അക്ഷരം *മൂല്യം)
{
  json_gen_string (json_gen, താക്കോൽ, strlen (താക്കോൽ));
  json_gen_string (json_gen, മൂല്യം, strlen (മൂല്യം));
}

സ്ഥിതം ശൂന്യം
features_add_bool (json_gen_ctx *json_gen, സ്ഥിരം അക്ഷരം *താക്കോൽ, പൂർണ്ണസംഖ്യ മൂല്യം)
{
  json_gen_string (json_gen, താക്കോൽ, strlen (താക്കോൽ));
  json_gen_bool (json_gen, മൂല്യം);
}

സ്ഥിതം ശൂന്യം
features_add_bool_str (json_gen_ctx *json_gen, സ്ഥിരം അക്ഷരം *താക്കോൽ, പൂർണ്ണസംഖ്യ മൂല്യം)
{
  features_add_string (json_gen, താക്കോൽ, മൂല്യം ? "true" : "false");
}

സ്ഥിതം ശൂന്യം
features_add_array (json_gen_ctx *json_gen, സ്ഥിരം അക്ഷരം *താക്കോൽ, അക്ഷരം **array)
{
  size_t i;

  json_gen_string (json_gen, താക്കോൽ, strlen (താക്കോൽ));
  json_gen_array_open (json_gen);
  എങ്കിൽ (array)
    വേണ്ടി (i = 0; array[i] != ശൂന്യ; i++)
      json_gen_string (json_gen, array[i], strlen (array[i]));
  json_gen_array_close (json_gen);
}

സ്ഥിതം ശൂന്യം
features_add_linux_info (json_gen_ctx *json_gen, സ്ഥിരം ഘടന linux_info_s *linux)
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
  എങ്കിൽ (linux->seccomp.actions)
    features_add_array (json_gen, "actions", linux->seccomp.actions);
  എങ്കിൽ (linux->seccomp.operators)
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
  എങ്കിൽ (linux->memory_policy.mode)
    features_add_array (json_gen, "modes", linux->memory_policy.mode);
  എങ്കിൽ (linux->memory_policy.flags)
    features_add_array (json_gen, "flags", linux->memory_policy.flags);
  json_gen_map_close (json_gen);

  json_gen_map_close (json_gen);
}

സ്ഥിതം ശൂന്യം
features_add_annotations_info (json_gen_ctx *json_gen, സ്ഥിരം ഘടന annotations_info_s *annotation)
{
  json_gen_string (json_gen, "annotations", strlen ("annotations"));
  json_gen_map_open (json_gen);

  എങ്കിൽ (! is_empty_string (annotation->io_github_seccomp_libseccomp_version))
    features_add_string (json_gen, "io.github.seccomp.libseccomp.version", annotation->io_github_seccomp_libseccomp_version);

  features_add_bool_str (json_gen, "org.opencontainers.runc.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);
  features_add_bool_str (json_gen, "run.oci.crun.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);

  features_add_string (json_gen, "run.oci.crun.commit", annotation->run_oci_crun_commit);
  features_add_string (json_gen, "run.oci.crun.version", annotation->run_oci_crun_version);

  features_add_bool_str (json_gen, "run.oci.crun.wasm", annotation->run_oci_crun_wasm);

  json_gen_map_close (json_gen);
}

പൂർണ്ണസംഖ്യ
libcrun_container_get_features_json (libcrun_context_t *context, അക്ഷരം **out, libcrun_error_t *err)
{
  cleanup_struct_features ഘടന features_info_s *info = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *buffer = ശൂന്യ;
  json_gen_ctx *json_gen;
  size_t buffer_len;
  പൂർണ്ണസംഖ്യ ret;

  *out = ശൂന്യ;

  ret = libcrun_container_get_features (context, &info, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (! json_gen_init (&json_gen, ശൂന്യ))
    തിരികെ crun_make_error (err, 0, "cannot initialize JSON generator");

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

  തിരികെ 0;
}

/* Read the container status and fail if the container is not running.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
read_status_require_running (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id,
                             libcrun_container_status_t *status, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  ret = libcrun_read_container_status (status, context->state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_is_container_running (status, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;
  എങ്കിൽ (ret == 0)
    തിരികെ crun_make_error (err, 0, "the container `%s` is not running", id);

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_container_pause (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ libcrun_container_pause_linux (&status, err);
}

പൂർണ്ണസംഖ്യ
libcrun_container_unpause (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ libcrun_container_unpause_linux (&status, err);
}

/* Translate the public options struct into the internal representation used by
   the linux/CRIU layer.  Strings are borrowed (not copied).  */
സ്ഥിതം ശൂന്യം
cr_options_from_public (libcrun_checkpoint_restore_t *dst, ഘടന libcrun_checkpoint_restore_options_s *src)
{
  memset (dst, 0, വലുപ്പം (*dst));
  dst->image_path = (അക്ഷരം *) src->image_path;
  dst->work_path = (അക്ഷരം *) src->work_path;
  dst->parent_path = (അക്ഷരം *) src->parent_path;
  dst->console_socket = src->console_socket;
  dst->lsm_profile = (അക്ഷരം *) src->lsm_profile;
  dst->lsm_mount_context = (അക്ഷരം *) src->lsm_mount_context;
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

പൂർണ്ണസംഖ്യ
libcrun_container_checkpoint (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id,
                              ഘടന libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;
  സ്ഥിരം അക്ഷരം *state_വേര് = context->state_വേര്;
  libcrun_container_status_t status = {};
  libcrun_checkpoint_restore_t cr;
  cleanup_container libcrun_container_t *container = ശൂന്യ;

  cr_options_from_public (&cr, cr_options);

  ret = read_status_require_running (context, id, &status, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = read_container_config_from_state (&container, state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  /* Needed to use the same paths (e.g. the shared empty directory used for
     masked paths) as the container runtime and restore do.  */
  container->context = context;

  ret = libcrun_container_checkpoint_linux (&status, container, &cr, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (! (cr_options->leave_running || cr_options->pre_dump))
    തിരികെ container_delete_internal (context, ശൂന്യ, ശൂന്യ, id, true, true, err);

  തിരികെ 0;
}

/* The dummy process only keeps the cgroup populated until the container is restored there.
   Its cgroup is below the container cgroup, so once the container runs it can freeze or kill
   the dummy process: never depend on it after that point.  */
സ്ഥിതം ശൂന്യം
restore_proxy_process (പൂർണ്ണസംഖ്യ proxy_pid_pipe)
{
  അക്ഷരം c;

  /* Wait to be killed by the parent, or for the parent to go away.  */
  TEMP_FAILURE_RETRY (പഠ (proxy_pid_pipe, &c, വലുപ്പം (c)));
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
restore_terminate_proxy_process (pid_t proxy_pid, സ്ഥിരം അക്ഷരം *proxy_cgroup, pid_t container_pid, പൂർണ്ണസംഖ്യ cgroup_manager, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ mode;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (cgroup_manager == CGROUP_MANAGER_SYSTEMD)
    {
      അക്ഷരം ready_str[64];

      ret = snprintf (ready_str, വലുപ്പം (ready_str), "MAINPID=%d", container_pid);
      എങ്കിൽ (UNLIKELY (ret >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (ready_str)))
        തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = send_sd_notify (ready_str, err);
      /* Do not fail on errors.  */
      എങ്കിൽ (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  mode = libcrun_get_cgroup_mode (err);
  എങ്കിൽ (UNLIKELY (mode < 0))
    തിരികെ mode;

  /* SIGKILL terminates a process frozen with cgroup v2, while the cgroup v1 freezer does not
     deliver it until the cgroup is thawed, so do not wait for it there.  */
  kill (proxy_pid, SIGKILL);
  TEMP_FAILURE_RETRY (waitpid (proxy_pid, ശൂന്യ, mode == CGROUP_MODE_UNIFIED ? 0 : WNOHANG));

  തിരികെ destroy_cgroup_path (proxy_cgroup, mode, err);
}

പൂർണ്ണസംഖ്യ
libcrun_container_restore (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id arg_unused,
                           ഘടന libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  cleanup_cgroup_status ഘടന libcrun_cgroup_status *cgroup_status = ശൂന്യ;
  cleanup_container libcrun_container_t *container = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ proxy_pid_pipe0 = -1;
  cleanup_close പൂർണ്ണസംഖ്യ proxy_pid_pipe1 = -1;
  runtime_spec_schema_config_schema *def;
  libcrun_container_status_t status = {};
  ഘടന libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  libcrun_checkpoint_restore_t cr;
  cleanup_pid pid_t proxy_pid = -1;
  cleanup_close പൂർണ്ണസംഖ്യ seccomp_fd = -1;
  സ്ഥിരം അക്ഷരം *seccomp_bpf_data;
  പൂർണ്ണസംഖ്യ proxy_pid_pipe[2];
  പൂർണ്ണസംഖ്യ cgroup_manager;
  uid_t വേര്_uid = -1;
  gid_t വേര്_gid = -1;
  പൂർണ്ണസംഖ്യ ret;

  cr_options_from_public (&cr, cr_options);

  container = libcrun_container_load_from_file ("config.json", err);
  എങ്കിൽ (container == ശൂന്യ)
    തിരികെ -1;

  container->context = context;
  def = container->container_def;

  ret = check_config_file (def, context, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_status_check_directories (context->state_വേര്, context->id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_copy_config_file (context->id, context->state_വേര്, container, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക fail;

  /* CRIU restores the seccomp filters of the checkpointed processes, but
     exec loads the filter from the seccomp.bpf file in the state directory,
     so it must be generated again for the restored container.  */
  seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക fail;

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക fail;
  close_and_reset (&seccomp_fd);

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  എങ്കിൽ (context->systemd_cgroup)
    cgroup_manager = CGROUP_MANAGER_SYSTEMD;
  അല്ലെങ്കിൽ എങ്കിൽ (context->force_no_cgroup)
    cgroup_manager = CGROUP_MANAGER_DISABLED;

  def = container->container_def;

  /* If we are root (either on the host or in a namespace),
   * then chown the cgroup to root in the container user namespace. */
  get_വേര്_in_the_userns (def, container->host_uid, container->host_gid, &വേര്_uid, &വേര്_gid);

  /* If the root in the container is different than the current root user, attempt to chown
     the std streams before entering the user namespace.  Otherwise we might lose access
     to the user (as it is not mapped in the user namespace) and cannot chown them.  */
  എങ്കിൽ (വേര്_uid > 0 || വേര്_gid > 0)
    {
      ret = maybe_chown_std_streams (വേര്_uid, വേര്_gid, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        പോകുക fail;
    }

  /* CRIU runs as a child process, so the restored processes would inherit the session keyring.  */
  ret = setup_container_keyring (container, context, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  {
    cleanup_close പൂർണ്ണസംഖ്യ cgroup_dirfd = -1;
    bool needs_proxy_process;
    cleanup_cgroup_preenter ഘടന libcrun_cgroup_args cg = {
      .resources = def->linux ? def->linux->resources : ശൂന്യ,
      .annotations = container->annotations,
      .cgroup_path = def->linux ? def->linux->cgroups_path : "",
      .manager = cgroup_manager,
      .വേര്_uid = വേര്_uid,
      .വേര്_gid = വേര്_gid,
      .id = context->id,
      .state_വേര് = context->state_വേര്,
      .container = container,
    };

    /* The CRIU restore code uses bundle, rootfs and cgroup_path of status.   The cgroup_path is set later.  */
    status.bundle = (അക്ഷരം *) context->bundle;
    status.rootfs = def->വേര്->path;

    ret = libcrun_cgroup_preenter (&cg, &cgroup_dirfd, err);
    എങ്കിൽ (UNLIKELY (ret < 0))
      പോകുക fail;

    needs_proxy_process = cgroup_dirfd < 0;

    /* If the target cgroup was already created (with cgroupfs), we can restore the container directly there.  */
    എങ്കിൽ (! needs_proxy_process)
      {
        cleanup_free അക്ഷരം *cgroup_path = ശൂന്യ;

        ret = get_cgroup_dirfd_path (cgroup_dirfd, &cgroup_path, err);
        എങ്കിൽ (UNLIKELY (ret < 0))
          പോകുക fail;

        /* Complete the configuration for the cgroup before the container is
           restored there: CRIU resumes the restored processes before it
           returns, so the device filter and the limits must already be in
           place.  There is no process to move, as CRIU is run in the cgroup,
           and the controllers were enabled by precreate_cgroup.  */
        cg.joined = true;

        ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
        എങ്കിൽ (UNLIKELY (ret < 0))
          പോകുക fail;

        /* Restore the container directly in the desired cgroup.  */
        status.cgroup_path = cgroup_path;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        എങ്കിൽ (UNLIKELY (ret < 0))
          പോകുക fail;

        /* Now that the container is restored, keep any signal for it, so that
           it is forwarded to the init by wait_for_process, rather than acted
           upon by crun.  This cannot be done earlier, as CRIU, which is run as
           a child process, would inherit the blocked signals.  */
        ret = block_signals (err);
        എങ്കിൽ (UNLIKELY (ret < 0))
          പോകുക fail;

        /* Use the container first process PID to finalize the cgroup.  */
        cg.pid = status.pid;
      }
    അല്ലെങ്കിൽ
      {
        /* When using systemd, we need a process first that must exist before a cgroup is created.
           Create a dummy process that sits and waits until the container is restored.  */
        ret = pipe2 (proxy_pid_pipe, O_CLOEXEC);
        എങ്കിൽ (UNLIKELY (ret < 0))
          {
            ret = crun_make_error (err, errno, "pipe");
            പോകുക fail;
          }

        /* Only used for auto cleanup.  */
        proxy_pid_pipe0 = proxy_pid_pipe[0];
        proxy_pid_pipe1 = proxy_pid_pipe[1];

        proxy_pid = fork ();
        എങ്കിൽ (UNLIKELY (proxy_pid < 0))
          {
            ret = crun_make_error (err, errno, "fork");
            പോകുക fail;
          }

        എങ്കിൽ (proxy_pid == 0)
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
        എങ്കിൽ (UNLIKELY (ret < 0))
          പോകുക fail;
      }

    ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
    എങ്കിൽ (UNLIKELY (ret < 0))
      പോകുക fail;

    /* When using a dummy process, the container is restored here once the cgroup is configured.  */
    എങ്കിൽ (needs_proxy_process)
      {
        cleanup_free അക്ഷരം *target_cgroup = ശൂന്യ;
        cleanup_free അക്ഷരം *proxy_cgroup = ശൂന്യ;

        ret = libcrun_get_cgroup_process (proxy_pid, &target_cgroup, false, err);
        എങ്കിൽ (UNLIKELY (ret < 0))
          പോകുക fail;

        /* Move the dummy process to a sub-cgroup.  */
        ret = append_paths (&proxy_cgroup, err, target_cgroup, ".proxy-crun", ശൂന്യ);
        എങ്കിൽ (UNLIKELY (ret < 0))
          പോകുക fail;

        ret = libcrun_move_process_to_cgroup (proxy_pid, 0, proxy_cgroup, true, err);
        എങ്കിൽ (UNLIKELY (ret < 0))
          പോകുക fail;

        /* Restore the container in the same cgroup where the dummy process was.  */
        status.cgroup_path = target_cgroup;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        എങ്കിൽ (UNLIKELY (ret < 0))
          പോകുക fail;

        /* See the comment for the other libcrun_container_restore_linux call.  */
        ret = block_signals (err);
        എങ്കിൽ (UNLIKELY (ret < 0))
          പോകുക fail;

        /* The container is running, so do not fail because of the dummy process.  */
        ret = restore_terminate_proxy_process (proxy_pid, proxy_cgroup, status.pid, cgroup_manager, err);
        എങ്കിൽ (UNLIKELY (ret < 0))
          crun_error_release (err);
        proxy_pid = -1;

        close_and_reset (&proxy_pid_pipe1);
      }
  }

  context->detach = cr_options->detach;
  ret = write_container_status (container, context, status.pid, cgroup_status, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക fail;

  {
    /* Wait the same way run and exec do: apart from waiting for the container
       init, this writes the pid file, forwards any signal to the init, and
       reaps every child, not only the init.  */
    ഘടന wait_for_process_args args = {
      .pid = status.pid,
      .context = context,
      .terminal_fd = -1,
      .notify_socket = -1,
      .container_ready_fd = ശൂന്യ,
      .seccomp_notify_fd = -1,
      .seccomp_notify_plugins = ശൂന്യ,
    };

    തിരികെ wait_for_process (&args, err);
  }

fail:
  /* Once restored, the container is running.  Without a status file nothing
     can reach it anymore, so do not leave it behind.  */
  എങ്കിൽ (status.pid > 0)
    {
      kill (status.pid, SIGKILL);
      waitpid_ignore_stopped (status.pid, ശൂന്യ, 0);
    }
  എങ്കിൽ (cgroup_status)
    {
      libcrun_error_t tmp_err = ശൂന്യ;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  {
    libcrun_error_t tmp_err = ശൂന്യ;

    എങ്കിൽ (libcrun_container_delete_status (context->state_വേര്, context->id, &tmp_err) < 0)
      crun_error_release (&tmp_err);
  }
  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
libcrun_container_read_pids (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  cleanup_cgroup_status ഘടന libcrun_cgroup_status *cgroup_status = ശൂന്യ;
  cleanup_container_status libcrun_container_status_t status = {};
  പൂർണ്ണസംഖ്യ ret;

  ret = libcrun_read_container_status (&status, context->state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (status.cgroup_path == ശൂന്യ || status.cgroup_path[0] == '\0')
    തിരികെ crun_make_error (err, 0, "the container is not using cgroups");

  cgroup_status = libcrun_cgroup_make_status (&status);

  തിരികെ libcrun_cgroup_read_pids (cgroup_status, recurse, pids, err);
}

പൂർണ്ണസംഖ്യ
libcrun_container_list_json (libcrun_context_t *context, അക്ഷരം **out, libcrun_error_t *err)
{
  libcrun_container_list_t *list = ശൂന്യ, *it;
  സ്ഥിരം അക്ഷരം *content = ശൂന്യ;
  json_gen_ctx *gen = ശൂന്യ;
  size_t len;
  പൂർണ്ണസംഖ്യ ret;

  *out = ശൂന്യ;

  ret = libcrun_get_containers_list (&list, context->state_വേര്, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (! json_gen_init (&gen, ശൂന്യ))
    {
      ret = crun_make_error (err, 0, "cannot allocate json generator");
      പോകുക exit;
    }

  json_gen_config (gen, json_gen_beautify, 1);
  json_gen_array_open (gen);

  വേണ്ടി (it = list; it; it = it->next)
    {
      libcrun_container_status_t status;
      പൂർണ്ണസംഖ്യ running = 0;
      പൂർണ്ണസംഖ്യ pid;
      സ്ഥിരം അക്ഷരം *container_status = ശൂന്യ;

      ret = libcrun_read_container_status (&status, context->state_വേര്, it->name, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        പോകുക exit;

      pid = status.pid;

      ret = libcrun_get_container_state_string (it->name, &status, context->state_വേര്, &container_status,
                                                &running, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          തുടരുക;
        }

      എങ്കിൽ (! running)
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
  എങ്കിൽ (json_gen_get_buf (gen, &content, &len) != json_gen_status_ok)
    {
      ret = libcrun_make_error (err, 0, "cannot generate json list");
      പോകുക exit;
    }

  *out = xstrdup (content);
  ret = 0;

exit:
  എങ്കിൽ (list)
    libcrun_free_containers_list (list);
  എങ്കിൽ (gen)
    json_gen_free (gen);

  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
libcrun_write_json_containers_list (libcrun_context_t *context, FILE *out, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *content = ശൂന്യ;
  size_t len;
  പൂർണ്ണസംഖ്യ ret;

  ret = libcrun_container_list_json (context, &content, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  len = strlen (content);
  എങ്കിൽ (len && fwrite (content, 1, len, out) != len)
    തിരികെ crun_make_error (err, errno, "error writing to file");

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_container_update_intel_rdt (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, ഘടന libcrun_intel_rdt_update *update, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;

  ret = read_container_config_from_state (&container, context->state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ libcrun_update_intel_rdt (id, container, update->l3_cache_schema, update->mem_bw_schema, update->schemata, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libcrun_container_add_or_remove_mounts_from_file (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *file, bool add, libcrun_error_t *err)
{
  cleanup_custom_handler_instance ഘടന custom_handler_instance_s *custom_handler = ശൂന്യ;
  cleanup_container libcrun_container_t *container = ശൂന്യ;
  cleanup_free runtime_spec_schema_defs_mount **mounts = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *state_വേര് = context->state_വേര്;
  cleanup_free parser_error parser_err = ശൂന്യ;
  ഘടന parser_context pctx = { 0, stderr };
  libcrun_container_status_t status = {};
  cleanup_free അക്ഷരം *content = ശൂന്യ;
  json_object *doc = ശൂന്യ;
  size_t n_mounts = 0, len, i;
  പൂർണ്ണസംഖ്യ ret = 1;

  ret = read_all_file (file, &content, &len, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_read_container_status (&status, state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = read_container_config_from_state (&container, state_വേര്, id, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = parse_json_file (&doc, content, &pctx, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (! json_object_is_type (doc, json_type_array))
    {
      ret = crun_make_error (err, 0, "mounts must be an array");
      പോകുക cleanup;
    }
  അല്ലെങ്കിൽ
    {
      n_mounts = json_object_array_length (doc);

      mounts = xmalloc0 ((n_mounts + 1) * വലുപ്പം (*mounts));
      വേണ്ടി (i = 0; i < n_mounts; i++)
        {
          mounts[i] = make_runtime_spec_schema_defs_mount (json_object_array_get_idx (doc, i), &pctx, &parser_err);
          എങ്കിൽ (mounts[i] == ശൂന്യ)
            {
              ret = crun_make_error (err, 0, "cannot parse mount: %s", parser_err);
              പോകുക cleanup;
            }
        }
    }

  എങ്കിൽ (add)
    ret = libcrun_make_runtime_mounts (container, &status, mounts, n_mounts, err);
  അല്ലെങ്കിൽ
    ret = libcrun_destroy_runtime_mounts (container, &status, mounts, n_mounts, err);

cleanup:
  എങ്കിൽ (doc)
    json_object_put (doc);
  എങ്കിൽ (mounts)
    വേണ്ടി (i = 0; i < n_mounts; i++)
      free_runtime_spec_schema_defs_mount (mounts[i]);
  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
libcrun_container_add_mounts_from_file (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *file, libcrun_error_t *err)
{
  തിരികെ libcrun_container_add_or_remove_mounts_from_file (context, id, file, true, err);
}

പൂർണ്ണസംഖ്യ
libcrun_container_remove_mounts_from_file (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *file, libcrun_error_t *err)
{
  തിരികെ libcrun_container_add_or_remove_mounts_from_file (context, id, file, false, err);
}
