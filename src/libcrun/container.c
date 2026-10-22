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

#include <config.h>
#include <ocispec/runtime_spec_schema_config_schema.h>
#include <stdbool.h>
#include "container.h"
#include "utils.h"
#include "json_gen_utils.h"
#include "seccomp.h"
#include "mempolicy.h"
#ifdef HAVE_SECCOMP
#  include <seccomp.h>
#endif
#include "scheduler.h"
#include "seccomp_notify.h"
#include "custom-handler.h"
#include <stdbool.h>
#include <argp.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sched.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include "status.h"
#include "mount_flags.h"
#include "linux.h"
#include "terminal.h"
#include "io_priority.h"
#include "cgroup.h"
#include "cgroup-utils.h"
#include <sys/prctl.h>
#include <sys/auxv.h>
#include <sys/signalfd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#ifdef HAVE_CAP
#  include <sys/capability.h>
#endif
#include <sys/ioctl.h>
#include <termios.h>
#include <grp.h>
#include <git-version.h>

#ifdef HAVE_SYSTEMD
#  include <systemd/sd-daemon.h>
#endif

#include <json-c/json.h>
#include <ocispec/json_common.h>

ga_nanaa
{
  SYNC_SOCKET_SYNC_MESSAGE,
  SYNC_SOCKET_ERROR_MESSAGE,
  SYNC_SOCKET_WARNING_MESSAGE,
  SYNC_SOCKET_DEBUG_MESSAGE,
};

sa_nracanaa container_entrypoint_s
{
  libcrun_container_t *container;
  libcrun_context_t *context;
  puur_naa_nka has_terminal_socket_pair;
  puur_naa_nka terminal_socketpair[2];

  /* Used by log_write_to_sync_socket.  */
  puur_naa_nka sync_socket;

  puur_naa_nka seccomp_fd;
  puur_naa_nka seccomp_receiver_fd;
  puur_naa_nka console_socket_fd;

  puur_naa_nka hooks_out_fd;
  puur_naa_nka hooks_err_fd;

  /* Absolute bundle path, to be reported to hooks run in the container.  */
  const var_na *bundle;

  sa_nracanaa custom_handler_instance_s *custom_handler;
};

sa_nracanaa sync_socket_message_s
{
  puur_naa_nka type;
  puur_naa_nka error_value;
  var_na message[512];
};

typedef runtime_spec_schema_defs_hook hook;

// linux hooks
var_na *hooks[] = {
  "prestart",
  "createRuntime",
  "createContainer",
  "startContainer",
  "poststart",
  "poststop"
};

// linux namespaces
static var_na *namespaces[] = {
  "cgroup",
  "ipc",
  "mount",
  "network",
  "pid",
  "user",
  "uts"
};

static var_na *actions[] = {
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

static var_na *operators[] = {
  "SCMP_CMP_NE",
  "SCMP_CMP_LT",
  "SCMP_CMP_LE",
  "SCMP_CMP_EQ",
  "SCMP_CMP_GE",
  "SCMP_CMP_GT",
  "SCMP_CMP_MASKED_EQ",
};

static var_na *archs[] = {
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

static var_na *mempolicy_modes[] = {
  "MPOL_DEFAULT",
  "MPOL_PREFERRED",
  "MPOL_BIND",
  "MPOL_INTERLEAVE",
  "MPOL_LOCAL",
  "MPOL_PREFERRED_MANY",
  "MPOL_WEIGHTED_INTERLEAVE"
};

static var_na *mempolicy_flags[] = {
  "MPOL_F_NUMA_BALANCING",
  "MPOL_F_RELATIVE_NODES",
  "MPOL_F_STATIC_NODES"
};

static var_na *potentially_unsafe_annotations[] = {
  "module.wasm.image/variant",
  "io.kubernetes.cri.container-type",
  "run.oci.",
  "org.criu.",
  "krun.",
};

#define SYNC_SOCKET_MESSAGE_LEN(x, l) (offsetof (sa_nracanaa sync_socket_message_s, message) + l)

static puur_naa_nka
sync_socket_write_msg (puur_naa_nka fd, puur_naa_nka verbosity, puur_naa_nka err_value, const var_na *log_msg)
{
  puur_naa_nka ret;
  size_t err_len;
  sa_nracanaa sync_socket_message_s msg;
  switch (verbosity)
    {
    pak_sa LIBCRUN_VERBOSITY_DEBUG:
      msg.type = SYNC_SOCKET_DEBUG_MESSAGE;
      bha_nga;
    pak_sa LIBCRUN_VERBOSITY_WARNING:
      msg.type = SYNC_SOCKET_WARNING_MESSAGE;
      bha_nga;
    pak_sa LIBCRUN_VERBOSITY_ERROR:
      msg.type = SYNC_SOCKET_ERROR_MESSAGE;
      bha_nga;
    }
  msg.error_value = err_value;

  ᱡᱩᱫᱤ (fd < 0)
    ᱨᱩᱣᱟᱹᱲ 0;

  err_len = strlen (log_msg);
  ᱡᱩᱫᱤ (err_len >= parimaa_na (msg.message))
    err_len = parimaa_na (msg.message) - 1;

  memcpy (msg.message, log_msg, err_len);
  msg.message[err_len] = '\0';

  ret = TEMP_FAILURE_RETRY (write (fd, &msg, SYNC_SOCKET_MESSAGE_LEN (msg, err_len + 1)));
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ -1;

  ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka
sync_socket_write_error (puur_naa_nka fd, libcrun_error_t *out_err)
{
  ᱡᱩᱫᱤ (fd < 0)
    ᱨᱩᱣᱟᱹᱲ 0;
  ᱨᱩᱣᱟᱹᱲ sync_socket_write_msg (fd, false, (*out_err)->status, (*out_err)->msg);
}

static void
log_write_to_sync_socket (puur_naa_nka errno_, const var_na *msg, puur_naa_nka verbosity, void *arg)
{
  sa_nracanaa container_entrypoint_s *entrypoint_args = arg;
  puur_naa_nka fd = entrypoint_args->sync_socket;

  ᱡᱩᱫᱤ (fd < 0)
    ᱨᱩᱣᱟᱹᱲ;

  ᱡᱩᱫᱤ (sync_socket_write_msg (fd, verbosity, errno_, msg) < 0)
    log_write_to_stderr (errno_, msg, verbosity, arg);
}

static bool
is_memory_limit_too_low (runtime_spec_schema_config_schema *def)
{
  const long memory_limit_too_low = 1024 * 1024;

  ᱡᱩᱫᱤ (def->linux == NULL || def->linux->resources == NULL)
    ᱨᱩᱣᱟᱹᱲ false;

  ᱡᱩᱫᱤ (def->linux->resources->memory
      && def->linux->resources->memory->limit_present
      && def->linux->resources->memory->limit < memory_limit_too_low)
    ᱨᱩᱣᱟᱹᱲ true;

  ᱡᱩᱫᱤ (def->linux->resources->unified)
    {
      size_t i;

      for (i = 0; i < def->linux->resources->unified->len; i++)
        ᱡᱩᱫᱤ (strcmp (def->linux->resources->unified->keys[i], "memory.max") == 0)
          {
            long limit;

            errno = 0;
            limit = strtol (def->linux->resources->unified->values[i], NULL, 10);
            ᱡᱩᱫᱤ (errno != 0)
              ᱨᱩᱣᱟᱹᱲ false;
            ᱡᱩᱫᱤ (limit < memory_limit_too_low)
              ᱨᱩᱣᱟᱹᱲ true;
          }
    }

  ᱨᱩᱣᱟᱹᱲ false;
}

static puur_naa_nka
sync_socket_wait_sync (libcrun_context_t *context, puur_naa_nka fd, bool flush, libcrun_error_t *err)
{
  sa_nracanaa sync_socket_message_s msg;

  ᱡᱩᱫᱤ (fd < 0)
    ᱨᱩᱣᱟᱹᱲ 0;

  ᱡᱚᱠᱷᱚᱱ (true)
    {
      puur_naa_nka ret;

      ret = TEMP_FAILURE_RETRY (read (fd, &msg, parimaa_na (msg)));
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        {
          ᱡᱩᱫᱤ (flush)
            ᱨᱩᱣᱟᱹᱲ 0;
          ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "read from sync socket");
        }

      ᱡᱩᱫᱤ (ret == 0)
        {
          ᱡᱩᱫᱤ (flush)
            ᱨᱩᱣᱟᱹᱲ 0;

          ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "read from the init process");
        }

      ᱡᱩᱫᱤ (! flush && msg.type == SYNC_SOCKET_SYNC_MESSAGE)
        ᱨᱩᱣᱟᱹᱲ 0;
      ᱵᱟᱝ ᱡᱩᱫᱤ (msg.type == SYNC_SOCKET_DEBUG_MESSAGE)
        {
          ᱡᱩᱫᱤ (context)
            context->output_handler (msg.error_value, msg.message, LIBCRUN_VERBOSITY_DEBUG, context->output_handler_arg);
          continue;
        }
      ᱵᱟᱝ ᱡᱩᱫᱤ (msg.type == SYNC_SOCKET_WARNING_MESSAGE)
        {
          ᱡᱩᱫᱤ (context)
            context->output_handler (msg.error_value, msg.message, LIBCRUN_VERBOSITY_WARNING, context->output_handler_arg);
          continue;
        }
      ᱵᱟᱝ ᱡᱩᱫᱤ (msg.type == SYNC_SOCKET_ERROR_MESSAGE)
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, msg.error_value, "%s", msg.message);
    }
}

static puur_naa_nka
sync_socket_send_sync (puur_naa_nka fd, bool flush_errors, libcrun_error_t *err)
{
  puur_naa_nka ret;
  sa_nracanaa sync_socket_message_s msg = {
    0,
  };
  msg.type = SYNC_SOCKET_SYNC_MESSAGE;

  ᱡᱩᱫᱤ (fd < 0)
    ᱨᱩᱣᱟᱹᱲ 0;

  ret = TEMP_FAILURE_RETRY (write (fd, &msg, SYNC_SOCKET_MESSAGE_LEN (msg, 0)));
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    {
      ᱡᱩᱫᱤ (flush_errors)
        {
          puur_naa_nka saved_errno = errno;
          ret = TEMP_FAILURE_RETRY (read (fd, &msg, parimaa_na (msg)));
          ᱡᱩᱫᱤ (ret >= 0 && msg.type == SYNC_SOCKET_ERROR_MESSAGE)
            ᱨᱩᱣᱟᱹᱲ crun_make_error (err, msg.error_value, "%s", msg.message);
          errno = saved_errno;
        }
      ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "write to sync socket");
    }

  ᱨᱩᱣᱟᱹᱲ 0;
}

static bool
has_cap_sys_ptrace ()
{
#ifdef HAVE_CAP
  sa_nracanaa __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  sa_nracanaa __user_cap_data_struct data[2] = { { 0 } };

  ᱡᱩᱫᱤ (UNLIKELY (capget (&hdr, data) < 0))
    ᱨᱩᱣᱟᱹᱲ true;

  ᱨᱩᱣᱟᱹᱲ (data[0].effective & (1U << CAP_SYS_PTRACE)) != 0;
#ᱵᱟᱝ
  ᱨᱩᱣᱟᱹᱲ geteuid () == 0;
#endif
}

static libcrun_container_t *
make_container (runtime_spec_schema_config_schema *container_def, const var_na *path, const var_na *config)
{
  libcrun_container_t *container = xmalloc0 (parimaa_na (*container));
  container->container_def = container_def;

  container->host_uid = geteuid ();
  container->host_gid = getegid ();
  container->host_has_cap_sys_ptrace = has_cap_sys_ptrace ();
  container->proc_fd = -1;

  container->annotations = make_string_map_from_json (container_def->annotations);

  /* An empty namespace path means that a new namespace is to be created,
     the same as if it is not specified.  */
  ᱡᱩᱫᱤ (container_def->linux)
    {
      size_t i;

      for (i = 0; i < container_def->linux->namespaces_len; i++)
        ᱡᱩᱫᱤ (container_def->linux->namespaces[i]->path && container_def->linux->namespaces[i]->path[0] == '\0')
          {
            free (container_def->linux->namespaces[i]->path);
            container_def->linux->namespaces[i]->path = NULL;
          }
    }

  ᱡᱩᱫᱤ (path)
    container->config_file = xstrdup (path);
  ᱡᱩᱫᱤ (config)
    container->config_file_content = xstrdup (config);

  ᱨᱩᱣᱟᱹᱲ container;
}

libcrun_container_t *
libcrun_container_load_from_memory (const var_na *json, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free var_na *oci_error = NULL;
  container_def = runtime_spec_schema_config_schema_parse_data (json, NULL, &oci_error);
  ᱡᱩᱫᱤ (container_def == NULL)
    {
      crun_make_error (err, 0, "load: `%s`", oci_error);
      ᱨᱩᱣᱟᱹᱲ NULL;
    }
  ᱨᱩᱣᱟᱹᱲ make_container (container_def, NULL, json);
}

libcrun_container_t *
libcrun_container_load_from_file (const var_na *path, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free var_na *oci_error = NULL;
  cleanup_free var_na *content = NULL;
  size_t len;
  puur_naa_nka ret;

  libcrun_debug ("Loading container from config file: `%s`", path);
  ret = read_all_file (path, &content, &len, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ NULL;

  container_def = runtime_spec_schema_config_schema_parse_data (content, NULL, &oci_error);
  ᱡᱩᱫᱤ (container_def == NULL)
    {
      crun_make_error (err, 0, "load `%s`: %s", path, oci_error);
      ᱨᱩᱣᱟᱹᱲ NULL;
    }
  ᱨᱩᱣᱟᱹᱲ make_container (container_def, NULL, content);
}

const var_na *
libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err)
{
  sa_nracanaa parser_context ctx = { 0, stderr };
  parser_error gen_err = NULL;

  ᱡᱩᱫᱤ (container == NULL)
    {
      crun_make_error (err, EINVAL, "no container specified");
      ᱨᱩᱣᱟᱹᱲ NULL;
    }

  ᱡᱩᱫᱤ (container->config_file_content == NULL && container->container_def)
    {
      container->config_file_content = runtime_spec_schema_config_schema_generate_json (container->container_def, &ctx, &gen_err);
      ᱡᱩᱫᱤ (container->config_file_content == NULL)
        {
          crun_make_error (err, 0, "cannot generate the configuration JSON: %s", gen_err ?: "unknown error");
          free (gen_err);
          ᱨᱩᱣᱟᱹᱲ NULL;
        }
      free (gen_err);
    }

  ᱡᱩᱫᱤ (container->config_file_content == NULL)
    {
      crun_make_error (err, 0, "the container has no configuration");
      ᱨᱩᱣᱟᱹᱲ NULL;
    }

  ᱨᱩᱣᱟᱹᱲ container->config_file_content;
}

const var_na *
libcrun_container_get_annotation (libcrun_container_t *container, const var_na *key)
{
  ᱡᱩᱫᱤ (container == NULL)
    ᱨᱩᱣᱟᱹᱲ NULL;
  ᱨᱩᱣᱟᱹᱲ find_string_map_value (container->annotations, key);
}

size_t
libcrun_container_get_annotations_len (libcrun_container_t *container)
{
  ᱡᱩᱫᱤ (container == NULL || container->annotations == NULL)
    ᱨᱩᱣᱟᱹᱲ 0;
  ᱨᱩᱣᱟᱹᱲ string_map_size (container->annotations);
}

puur_naa_nka
libcrun_container_get_annotation_at (libcrun_container_t *container, size_t index, const var_na **key,
                                     const var_na **value)
{
  ᱡᱩᱫᱤ (container == NULL)
    {
      errno = ERANGE;
      ᱨᱩᱣᱟᱹᱲ -1;
    }
  ᱨᱩᱣᱟᱹᱲ string_map_get_at (container->annotations, index, key, value);
}

uid_t
libcrun_container_get_uid (libcrun_container_t *container)
{
  ᱡᱩᱫᱤ (container == NULL)
    ᱨᱩᱣᱟᱹᱲ (uid_t) -1;
  ᱨᱩᱣᱟᱹᱲ container->host_uid;
}

gid_t
libcrun_container_get_gid (libcrun_container_t *container)
{
  ᱡᱩᱫᱤ (container == NULL)
    ᱨᱩᱣᱟᱹᱲ (gid_t) -1;
  ᱨᱩᱣᱟᱹᱲ container->host_gid;
}

void
libcrun_container_free (libcrun_container_t *ctr)
{
  ᱡᱩᱫᱤ (ctr == NULL)
    ᱨᱩᱣᱟᱹᱲ;

  ᱡᱩᱫᱤ (ctr->cleanup_private_data)
    ctr->cleanup_private_data (ctr->private_data);

  ᱡᱩᱫᱤ (ctr->container_def)
    free_runtime_spec_schema_config_schema (ctr->container_def);

  free_string_map (ctr->annotations);

  ᱡᱩᱫᱤ (ctr->proc_fd >= 0)
    close (ctr->proc_fd);

  free (ctr->config_file_content);
  free (ctr->config_file);
  free (ctr);
}

libcrun_context_t *
libcrun_context_new (const var_na *id, const var_na *state_root, libcrun_error_t *err)
{
  libcrun_context_t *ctx = xmalloc0 (parimaa_na (*ctx));

  /* Go through the setters so that the owned_* copies and the borrowed
     pointers stay in sync and are released by libcrun_context_free.  */
  libcrun_context_set_id (ctx, id);
  libcrun_context_set_state_root (ctx, state_root);
  libcrun_context_set_bundle (ctx, ".");
  ctx->fifo_exec_wait_fd = -1;

  ᱡᱩᱫᱤ (libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, id, NULL, err) < 0)
    {
      libcrun_context_free (ctx);
      ᱨᱩᱣᱟᱹᱲ NULL;
    }

  ctx->handler_manager = libcrun_handler_manager_create (err);
  ᱡᱩᱫᱤ (ctx->handler_manager == NULL)
    {
      libcrun_context_free (ctx);
      ᱨᱩᱣᱟᱹᱲ NULL;
    }

#ifdef CRUN_LIBDIR
  ᱡᱩᱫᱤ (access (CRUN_LIBDIR "/handlers", F_OK) == 0)
    {
      ᱡᱩᱫᱤ (libcrun_handler_manager_load_directory (ctx->handler_manager, CRUN_LIBDIR "/handlers", err) < 0)
        {
          libcrun_context_free (ctx);
          ᱨᱩᱣᱟᱹᱲ NULL;
        }
    }
#endif

  ᱨᱩᱣᱟᱹᱲ ctx;
}

void
libcrun_context_free (libcrun_context_t *ctx)
{
  ᱡᱩᱫᱤ (ctx == NULL)
    ᱨᱩᱣᱟᱹᱲ;

  free (ctx->owned_state_root);
  free (ctx->owned_id);
  free (ctx->owned_bundle);
  free (ctx->owned_console_socket);
  free (ctx->owned_pid_file);
  free (ctx->owned_notify_socket);
  free (ctx->owned_handler);

  ᱡᱩᱫᱤ (ctx->handler_manager)
    handler_manager_free (ctx->handler_manager);

  free (ctx);
}

#define LIBCRUN_CONTEXT_SET_STRING(field)                  \
  do                                                       \
    {                                                      \
      free (ctx->owned_##field);                           \
      ctx->owned_##field = value ? xstrdup (value) : NULL; \
      ctx->field = ctx->owned_##field;                     \
  } ᱡᱚᱠᱷᱚᱱ (0)

void
libcrun_context_set_id (libcrun_context_t *ctx, const var_na *value)
{
  LIBCRUN_CONTEXT_SET_STRING (id);
}

void
libcrun_context_set_state_root (libcrun_context_t *ctx, const var_na *value)
{
  LIBCRUN_CONTEXT_SET_STRING (state_root);
}

void
libcrun_context_set_bundle (libcrun_context_t *ctx, const var_na *value)
{
  LIBCRUN_CONTEXT_SET_STRING (bundle);
}

void
libcrun_context_set_console_socket (libcrun_context_t *ctx, const var_na *value)
{
  LIBCRUN_CONTEXT_SET_STRING (console_socket);
}

void
libcrun_context_set_pid_file (libcrun_context_t *ctx, const var_na *value)
{
  LIBCRUN_CONTEXT_SET_STRING (pid_file);
}

void
libcrun_context_set_notify_socket (libcrun_context_t *ctx, const var_na *value)
{
  LIBCRUN_CONTEXT_SET_STRING (notify_socket);
}

void
libcrun_context_set_handler (libcrun_context_t *ctx, const var_na *value)
{
  LIBCRUN_CONTEXT_SET_STRING (handler);
}

#undef LIBCRUN_CONTEXT_SET_STRING

const var_na *
libcrun_context_get_pid_file (libcrun_context_t *ctx)
{
  ᱨᱩᱣᱟᱹᱲ ctx->pid_file;
}

const var_na *
libcrun_context_get_state_root (libcrun_context_t *ctx)
{
  ᱡᱩᱫᱤ (ctx == NULL)
    ᱨᱩᱣᱟᱹᱲ NULL;
  ᱨᱩᱣᱟᱹᱲ ctx->state_root;
}

void
libcrun_context_set_preserve_fds (libcrun_context_t *ctx, puur_naa_nka preserve_fds)
{
  ctx->preserve_fds = preserve_fds;
}

puur_naa_nka
libcrun_context_get_preserve_fds (libcrun_context_t *ctx)
{
  ᱨᱩᱣᱟᱹᱲ ctx->preserve_fds;
}

void
libcrun_context_set_listen_fds (libcrun_context_t *ctx, puur_naa_nka listen_fds)
{
  ctx->listen_fds = listen_fds;
}

void
libcrun_context_set_args (libcrun_context_t *ctx, puur_naa_nka argc, var_na **argv)
{
  ctx->argc = argc;
  ctx->argv = argv;
}

void
libcrun_context_set_systemd_cgroup (libcrun_context_t *ctx, bool value)
{
  ctx->systemd_cgroup = value;
}

void
libcrun_context_set_detach (libcrun_context_t *ctx, bool value)
{
  ctx->detach = value;
}

void
libcrun_context_set_no_new_keyring (libcrun_context_t *ctx, bool value)
{
  ctx->no_new_keyring = value;
}

void
libcrun_context_set_no_pivot (libcrun_context_t *ctx, bool value)
{
  ctx->no_pivot = value;
}

void
libcrun_context_set_force_no_cgroup (libcrun_context_t *ctx, bool value)
{
  ctx->force_no_cgroup = value;
}

void
libcrun_context_set_output_handler (libcrun_context_t *ctx, crun_output_handler handler, void *arg)
{
  ctx->output_handler = handler;
  ctx->output_handler_arg = arg;
}

puur_naa_nka
libcrun_context_init_logging (libcrun_context_t *ctx, const var_na *log, libcrun_error_t *err)
{
  ᱨᱩᱣᱟᱹᱲ libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, ctx->id, log, err);
}

static puur_naa_nka
block_signals (libcrun_error_t *err)
{
  puur_naa_nka ret;
  sigset_t mask;
  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, NULL);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "sigprocmask");
  ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka
unblock_signals (libcrun_error_t *err)
{
  puur_naa_nka i;
  puur_naa_nka ret;
  sigset_t mask;
  sa_nracanaa sigaction act = {};

  sigfillset (&mask);
  ret = sigprocmask (SIG_UNBLOCK, &mask, NULL);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "sigprocmask");

  act.sa_handler = SIG_DFL;
  for (i = 0; i < NSIG; i++)
    {
      ret = sigaction (i, &act, NULL);
      ᱡᱩᱫᱤ (ret < 0 && errno != EINVAL)
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "sigaction");
    }

  ᱨᱩᱣᱟᱹᱲ 0;
}

/* must be used on the host before pivot_root(2).  */
static puur_naa_nka
initialize_security (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, libcrun_error_t *err)
{
  puur_naa_nka ret;

  ret = libcrun_init_caps (container, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱡᱩᱫᱤ (UNLIKELY (proc == NULL))
    ᱨᱩᱣᱟᱹᱲ 0;

  ᱡᱩᱫᱤ (proc->apparmor_profile)
    {
      ret = libcrun_initialize_apparmor (err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
    }

  ret = libcrun_initialize_selinux (container, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱨᱩᱣᱟᱹᱲ 0;
}

/* Emit the "annotations" map.  Returns a json_gen_status; callers can wrap
   the call with GEN_OR_FAIL.  */
static puur_naa_nka
gen_annotations (json_gen_ctx *gen, json_map_string_string *annotations)
{
  size_t i;
  puur_naa_nka r;

  GEN_KEY (gen, "annotations");
  GEN_OR_FAIL (json_gen_map_open (gen));

  for (i = 0; i < annotations->len; i++)
    {
      /* An annotation whose value is null in the document is parsed into a
         NULL, which cannot be emitted as a JSON string; leave it out.  */
      ᱡᱩᱫᱤ (annotations->keys[i] == NULL || annotations->values[i] == NULL)
        continue;

      GEN_STR (gen, annotations->keys[i]);
      GEN_STR (gen, annotations->values[i]);
    }

  ᱨᱩᱣᱟᱹᱲ json_gen_map_close (gen);

gen_error:
  ᱨᱩᱣᱟᱹᱲ r;
}

/* Return the bundle path to report in the container state.  crun always
   chdir()s into the bundle, so fall back to getcwd() when the caller has no
   absolute bundle path, as can happen when libcrun is used as a library.
   *ALLOCATED must be freed by the caller.  */
static const var_na *
get_bundle_path (const var_na *bundle, var_na **allocated)
{
  ᱡᱩᱫᱤ (bundle != NULL && bundle[0] == '/')
    ᱨᱩᱣᱟᱹᱲ bundle;

  *allocated = getcwd (NULL, 0);
  ᱡᱩᱫᱤ (*allocated == NULL)
    OOM ();
  ᱨᱩᱣᱟᱹᱲ *allocated;
}

static puur_naa_nka
do_hooks (runtime_spec_schema_config_schema *def, pid_t pid, const var_na *id, bool keep_going, const var_na *bundle,
          const var_na *cwd, const var_na *status, const var_na *name, hook **hooks, size_t hooks_len, puur_naa_nka out_fd,
          puur_naa_nka err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  size_t i, stdin_len;
  puur_naa_nka r, ret;
  var_na *stdin = NULL;
  cleanup_free var_na *bundle_allocated = NULL;
  const var_na *rootfs = def->root ? def->root->path : "";
  cleanup_json_gen json_gen_ctx *gen = NULL;

  bundle = get_bundle_path (bundle, &bundle_allocated);

  ᱡᱩᱫᱤ (! json_gen_init (&gen, NULL))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "json_gen_init failed");

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

  ᱡᱩᱫᱤ (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, (const var_na **) &stdin, &stdin_len));

  ret = 0;

  bool error_created = false;
  for (i = 0; i < hooks_len; i++)
    {
      var_na **env = environ;

      ᱡᱩᱫᱤ (hooks[i]->env)
        env = hooks[i]->env;

      /* Release the error from the previous iteration, if any.  */
      ᱡᱩᱫᱤ (error_created)
        {
          crun_error_release (err);
          error_created = false;
        }

      ret = run_process_with_stdin_timeout_envp (hooks[i]->path, hooks[i]->args, cwd, hooks[i]->timeout, env,
                                                 stdin, stdin_len, out_fd, err_fd, can_ignore_chdir_errors, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        error_created = true;

      ᱡᱩᱫᱤ (UNLIKELY (ret != 0))
        {
          ᱡᱩᱫᱤ (keep_going)
            libcrun_warning ("error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
          ᱵᱟᱝ
            {
              libcrun_error (0, "error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
              bha_nga;
            }
        }
    }

  ᱨᱩᱣᱟᱹᱲ ret;

gen_error:
  ᱨᱩᱣᱟᱹᱲ json_gen_error_to_crun_error (r, err);
}

static puur_naa_nka
get_json_gen_result (json_gen_ctx *gen, var_na **out, size_t *out_len)
{
  const var_na *buf = NULL;
  size_t buf_len = 0;
  puur_naa_nka r;

  r = json_gen_get_buf (gen, &buf, &buf_len);
  ᱡᱩᱫᱤ (UNLIKELY (r != json_gen_status_ok))
    ᱨᱩᱣᱟᱹᱲ r;

  *out_len = buf_len;

  *out = xmalloc (buf_len + 1);
  memcpy (*out, buf, buf_len);
  (*out)[buf_len] = '\0';

  ᱨᱩᱣᱟᱹᱲ json_gen_status_ok;
}

static puur_naa_nka
get_seccomp_receiver_fd_payload (libcrun_container_t *container, const var_na *status, pid_t own_pid,
                                 var_na **seccomp_fd_payload, size_t *seccomp_fd_payload_len, libcrun_error_t *err)
{
  puur_naa_nka r;
  json_gen_ctx *gen = NULL;
  runtime_spec_schema_config_schema *def = container->container_def;
  const var_na *const OCI_VERSION = "0.2.0";

  ᱡᱩᱫᱤ (! json_gen_init (&gen, NULL))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "json_gen_init failed");

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

  ᱡᱩᱫᱤ (def && def->linux && def->linux->seccomp)
    {
      const var_na *metadata = def->linux->seccomp->listener_metadata;

      ᱡᱩᱫᱤ (metadata)
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

  ᱡᱩᱫᱤ (container->context && container->context->id)
    {
      GEN_KEY (gen, "id");
      GEN_STR (gen, container->context->id);
    }

  GEN_KEY (gen, "status");
  GEN_STR (gen, status);

  GEN_KEY (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  ᱡᱩᱫᱤ (container->context && container->context->bundle)
    {
      GEN_KEY (gen, "bundle");
      GEN_STR (gen, container->context->bundle);
    }

  ᱡᱩᱫᱤ (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));
  /* End state.  */

  GEN_OR_FAIL (json_gen_map_close (gen));

  r = get_json_gen_result (gen, seccomp_fd_payload, seccomp_fd_payload_len);

gen_error:
  json_gen_free (gen);

  ᱨᱩᱣᱟᱹᱲ json_gen_error_to_crun_error (r, err);
}

static puur_naa_nka
send_sync_cb (void *data, libcrun_error_t *err)
{
  puur_naa_nka sync_socket_fd = *((puur_naa_nka *) data);
  puur_naa_nka ret;

  /* sync 2.  */
  ret = sync_socket_send_sync (sync_socket_fd, false, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  /* sync 3.  */
  ᱨᱩᱣᱟᱹᱲ sync_socket_wait_sync (NULL, sync_socket_fd, false, err);
}

static puur_naa_nka
maybe_chown_std_streams (uid_t container_uid, gid_t container_gid, libcrun_error_t *err)
{
  puur_naa_nka ret, i;

  for (i = 0; i < 3; i++)
    {
      ᱡᱩᱫᱤ (! isatty (i))
        {
          sa_nracanaa stat statbuf;
          ret = fstat (i, &statbuf);
          ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
            {
              ᱡᱩᱫᱤ (errno == EBADF)
                continue;
              ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "fstat fd `%d`", i);
            }

          /* Skip chown for device files */
          ᱡᱩᱫᱤ (S_ISCHR (statbuf.st_mode) || S_ISBLK (statbuf.st_mode))
            continue;

          ret = fchown (i, container_uid, container_gid);
          ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
            {
              /* EINVAL means the user is not mapped in the current userns.
                 Ignore EPERM and EROFS as well as there is no reason to fail
                 so early, and let the container payload deal with it.
                 EBADF means fd is closed.
              */
              ᱡᱩᱫᱤ (errno == EINVAL || errno == EPERM || errno == EROFS || errno == EBADF)
                continue;

              ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "fchown std stream `%i`", i);
            }
        }
    }
  ᱨᱩᱣᱟᱹᱲ 0;
}

puur_naa_nka
libcrun_container_notify_handler (sa_nracanaa container_entrypoint_s *args,
                                  ga_nanaa handler_configure_phase phase,
                                  libcrun_container_t *container, const var_na *rootfs,
                                  libcrun_error_t *err)
{
  sa_nracanaa custom_handler_s *h;

  ᱡᱩᱫᱤ (args->custom_handler == NULL)
    ᱨᱩᱣᱟᱹᱲ 0;

  h = args->custom_handler->vtable;
  ᱡᱩᱫᱤ (h == NULL || h->configure_container == NULL)
    ᱨᱩᱣᱟᱹᱲ 0;

  ᱨᱩᱣᱟᱹᱲ h->configure_container (args->custom_handler->cookie, phase,
                                 args->context, container,
                                 rootfs, err);
}

/* Resolve and normalize the container rootfs path.  */
static puur_naa_nka
resolve_rootfs_path (libcrun_container_t *container, var_na **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ᱡᱩᱫᱤ (def->root && def->root->path)
    {
      *rootfs = realpath (def->root->path, NULL);
      ᱡᱩᱫᱤ (UNLIKELY (*rootfs == NULL))
        {
          puur_naa_nka saved_errno = errno;

          /* If realpath failed for any reason, try the relative directory.  */
          ᱡᱩᱫᱤ (def->root->path[0] == '/')
            {
              cleanup_free var_na *cwd = NULL;
              libcrun_error_t tmp_err = NULL;
              ssize_t len;
              puur_naa_nka proc_fd;

              proc_fd = libcrun_get_cached_proc_fd (container, &tmp_err);
              ᱡᱩᱫᱤ (LIKELY (proc_fd >= 0))
                {
                  len = safe_readlinkat (proc_fd, "self/cwd", &cwd, 0, &tmp_err);
                  ᱡᱩᱫᱤ (LIKELY (len > 0))
                    {
                      /* If the rootfs is under the current working directory, just use its relative path.  */
                      ᱡᱩᱫᱤ (has_dir_prefix (def->root->path, cwd))
                        {
                          const var_na *it = consume_slashes (def->root->path + len);
                          ᱡᱩᱫᱤ (LIKELY (*it))
                            {
                              puur_naa_nka ret = faccessat (AT_FDCWD, it, F_OK, 0);
                              ᱡᱩᱫᱤ (LIKELY (ret == 0))
                                *rootfs = xstrdup (it);
                            }
                        }
                    }
                }
              crun_error_release (&tmp_err);
            }

          ᱡᱩᱫᱤ (*rootfs == NULL)
            {
              ᱡᱩᱫᱤ (saved_errno == ENOENT)
                ᱨᱩᱣᱟᱹᱲ crun_make_error (err, ENOENT, "the rootfs path `%s` does not exist", def->root->path);

              *rootfs = xstrdup (def->root->path);
            }
        }
    }
  ᱨᱩᱣᱟᱹᱲ 0;
}

/* Initialize the environment variables.  */
static puur_naa_nka
setup_environment (runtime_spec_schema_config_schema *def, uid_t container_uid, libcrun_error_t *err)
{
  puur_naa_nka ret;

  ᱡᱩᱫᱤ (clearenv ())
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "clearenv");

  ᱡᱩᱫᱤ (def->process)
    {
      size_t i;

      for (i = 0; i < def->process->env_len; i++)
        {
          /* A null element of the array of variables is parsed into a NULL,
             which putenv(3) cannot be handed.  */
          ᱡᱩᱫᱤ (def->process->env[i] == NULL)
            ᱨᱩᱣᱟᱹᱲ crun_make_error (err, EINVAL, "environment variable is not specified");

          ᱡᱩᱫᱤ (putenv (def->process->env[i]) < 0)
            ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "putenv `%s`", def->process->env[i]);
        }
    }

  ᱡᱩᱫᱤ (is_empty_string (getenv ("HOME")))
    {
      ret = set_home_env (container_uid);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          ᱡᱩᱫᱤ (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "setenv HOME");
        }
    }

  ᱨᱩᱣᱟᱹᱲ 0;
}

/* Configure terminal and console for interactive containers.  */
static puur_naa_nka
setup_terminal (sa_nracanaa container_entrypoint_s *entrypoint_args, libcrun_container_t *container,
                puur_naa_nka console_socket, puur_naa_nka console_socketpair, puur_naa_nka has_terminal, libcrun_error_t *err)
{
  cleanup_close puur_naa_nka terminal_fd = -1;
  puur_naa_nka ret;

  ᱡᱩᱫᱤ (! has_terminal)
    ᱨᱩᱣᱟᱹᱲ 0;

  fflush (stderr);

  ᱡᱩᱫᱤ (console_socket >= 0 || (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0))
    {
      terminal_fd = libcrun_set_terminal (container, err);
      ᱡᱩᱫᱤ (UNLIKELY (terminal_fd < 0))
        ᱨᱩᱣᱟᱹᱲ terminal_fd;
    }

  ᱡᱩᱫᱤ (console_socket >= 0)
    {
      ret = send_fd_to_socket (console_socket, terminal_fd, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
    }
  ᱵᱟᱝ ᱡᱩᱫᱤ (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0)
    {
      ret = send_fd_to_socket (console_socketpair, terminal_fd, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
    }

  ᱨᱩᱣᱟᱹᱲ 0;
}

/* Resolve and validate the container's executable path.  */
static puur_naa_nka
setup_executable_path (sa_nracanaa container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                       var_na **exec_path, libcrun_error_t *err)
{
  puur_naa_nka ret;

  ᱡᱩᱫᱤ (def->process && def->process->args)
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        {
          ᱡᱩᱫᱤ (entrypoint_args->custom_handler == NULL && crun_error_get_errno (err) == ENOENT)
            ᱨᱩᱣᱟᱹᱲ ret;

          /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
             once the process switched to the use that runs in the container.  This might be necessary
             when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
             is not honored.  */
          crun_error_release (err);
        }
    }

  ᱨᱩᱣᱟᱹᱲ 0;
}

/* Apply security settings for the container process.  */
static puur_naa_nka
apply_security_settings (sa_nracanaa container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                         libcrun_container_t *container, pid_t own_pid, libcrun_error_t *err)
{
  puur_naa_nka ret;
  runtime_spec_schema_config_schema_process_capabilities *capabilities;
  puur_naa_nka no_new_privs;

  ᱡᱩᱫᱤ (def->process && ! def->process->no_new_privileges)
    {
      var_na **seccomp_flags = NULL;
      size_t seccomp_flags_len = 0;
      cleanup_free var_na *seccomp_fd_payload = NULL;
      size_t seccomp_fd_payload_len = 0;

      ᱡᱩᱫᱤ (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      ᱡᱩᱫᱤ (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
            ᱨᱩᱣᱟᱹᱲ ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;

      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  capabilities = def->process ? def->process->capabilities : NULL;
  no_new_privs = def->process ? def->process->no_new_privileges : 1;
  ret = libcrun_set_caps (capabilities, container->container_uid, container->container_gid, no_new_privs,
                          entrypoint_args->custom_handler != NULL, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱨᱩᱣᱟᱹᱲ 0;
}

/* Create the process that joins the existing PID namespace now that the
   host mounts are not visible anymore, and continue the setup from there.  */
static puur_naa_nka
do_delayed_pidns_fork (puur_naa_nka sync_socket, pid_t *own_pid, libcrun_error_t *err)
{
  pid_t pid;
  puur_naa_nka ret;

  pid = fork ();
  ᱡᱩᱫᱤ (UNLIKELY (pid < 0))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "fork");

  ᱡᱩᱫᱤ (pid)
    {
      /* Report the new PID to the parent and exit immediately.  */
      ret = sync_socket_send_sync (sync_socket, false, err);
      ᱡᱩᱫᱤ (LIKELY (ret == 0))
        {
          ret = TEMP_FAILURE_RETRY (write (sync_socket, &pid, parimaa_na (pid)));
          ᱡᱩᱫᱤ (LIKELY (ret == parimaa_na (pid)))
            _safe_exit (EXIT_SUCCESS);
        }

      kill (pid, SIGKILL);
      _safe_exit (EXIT_FAILURE);
    }

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (read (sync_socket, own_pid, parimaa_na (*own_pid)));
  ᱡᱩᱫᱤ (UNLIKELY (ret != parimaa_na (*own_pid)))
    {
      ᱡᱩᱫᱤ (ret >= 0)
        errno = 0;
      ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "read from sync socket");
    }

  ᱨᱩᱣᱟᱹᱲ 0;
}

sa_nracanaa delayed_pidns_fork_s
{
  libcrun_container_t *container;
  puur_naa_nka sync_socket;
  pid_t *own_pid;
};

static puur_naa_nka
delayed_pidns_fork_cb (void *data, libcrun_error_t *err)
{
  sa_nracanaa delayed_pidns_fork_s *d = data;

  ᱡᱩᱫᱤ (! libcrun_container_delayed_pidns_fork (d->container))
    ᱨᱩᱣᱟᱹᱲ 0;

  ᱨᱩᱣᱟᱹᱲ do_delayed_pidns_fork (d->sync_socket, d->own_pid, err);
}

static puur_naa_nka
container_init_setup (void *args, pid_t *own_pid, var_na *notify_socket,
                      puur_naa_nka sync_socket, var_na **exec_path, libcrun_error_t *err)
{
  sa_nracanaa container_entrypoint_s *entrypoint_args = args;
  libcrun_container_t *container = entrypoint_args->container;
  bool chdir_done = false;
  puur_naa_nka ret;
  puur_naa_nka has_terminal;
  cleanup_close puur_naa_nka console_socket = -1;
  cleanup_close puur_naa_nka console_socketpair = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free var_na *rootfs = NULL;
  sa_nracanaa delayed_pidns_fork_s delayed_pidns_fork = {
    .container = container,
    .sync_socket = sync_socket,
    .own_pid = own_pid,
  };

  ret = initialize_security (container, def->process, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = libcrun_configure_network (container, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = resolve_rootfs_path (container, &rootfs, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  /* Configure terminal socket pair for container communication.  */
  ᱡᱩᱫᱤ (entrypoint_args->terminal_socketpair[0] >= 0)
    {
      close_and_reset (&entrypoint_args->terminal_socketpair[0]);
      console_socketpair = entrypoint_args->terminal_socketpair[1];
    }

  /* sync 1.  */
  ret = sync_socket_wait_sync (NULL, sync_socket, false, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  has_terminal = container->container_def->process && container->container_def->process->terminal;
  ᱡᱩᱫᱤ (has_terminal && entrypoint_args->context->console_socket)
    console_socket = entrypoint_args->console_socket_fd;

  ret = libcrun_set_sysctl (container, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_BEFORE_MOUNTS, container, rootfs, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = libcrun_do_pivot_root (container, entrypoint_args->context->no_pivot, &rootfs, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  /* sync 2 and 3 are sent as part of libcrun_set_mounts.  */
  ret = libcrun_set_mounts (entrypoint_args, container, rootfs, send_sync_cb, &sync_socket, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  /* The createContainer hooks MUST run after the mounts are set up but before
     the pivot_root.  When these hooks are present can_use_open_tree_namespace()
     forces the deferred-pivot path, so the actual pivot happens later in
     libcrun_finalize_mounts().  */
  ᱡᱩᱫᱤ (def->hooks && def->hooks->create_container_len)
    {
      libcrun_error_t tmp_err = NULL;
      puur_naa_nka in_userns = check_running_in_user_namespace (&tmp_err);
      ᱡᱩᱫᱤ (tmp_err)
        crun_error_release (&tmp_err);
      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, NULL, "created", "createContainer",
                      (hook **) def->hooks->create_container,
                      def->hooks->create_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret != 0))
        ᱨᱩᱣᱟᱹᱲ ret;
    }

  ret = libcrun_finalize_mounts (entrypoint_args, container, rootfs, delayed_pidns_fork_cb, &delayed_pidns_fork, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱡᱩᱫᱤ (def->process)
    {
      ret = libcrun_set_selinux_label (container, def->process, false, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;

      ret = libcrun_set_apparmor_profile (container, def->process, false, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
    }

  ret = mark_or_close_fds_ge_than (container, entrypoint_args->context->preserve_fds + 3, false, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ret = libcrun_reopen_dev_null (err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = maybe_chown_std_streams (container->container_uid, container->container_gid, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = setup_environment (def, container->container_uid, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  /* Set primary process to 1 explicitly if nothing is configured and LISTEN_FD is not set.  */
  ᱡᱩᱫᱤ (entrypoint_args->context->listen_fds > 0 && getenv ("LISTEN_PID") == NULL)
    {
      libcrun_warning ("setting LISTEN_PID=1 since no previous configuration was found");
      ᱡᱩᱫᱤ (UNLIKELY (setenv ("LISTEN_PID", "1", 1) < 0))
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "setenv LISTEN_PID");
    }

  /* Attempt to chdir immediately here, before doing the setresuid.  If we fail here, let's
     try again later once the process switched to the user that runs in the container.  */
  ᱡᱩᱫᱤ (def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      ᱡᱩᱫᱤ (LIKELY (ret == 0))
        chdir_done = true;
      ᱵᱟᱝ
        crun_error_release (err);
    }

  ret = setsid ();
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "setsid");

  ret = setup_terminal (entrypoint_args, container, console_socket, console_socketpair, has_terminal, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  close_and_reset (&console_socket);
  close_and_reset (&console_socketpair);

  ret = setup_executable_path (entrypoint_args, def, exec_path, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = libcrun_set_hostname (container, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = libcrun_set_domainname (container, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱡᱩᱫᱤ (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
    }

  ᱡᱩᱫᱤ (def->process && def->process->user)
    umask (def->process->user->umask_present ? def->process->user->umask : 0022);

  ret = apply_security_settings (entrypoint_args, def, container, *own_pid, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱡᱩᱫᱤ (UNLIKELY (def->process && def->process->args && *exec_path == NULL))
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        {
          ᱡᱩᱫᱤ (entrypoint_args->custom_handler == NULL || is_empty_string (def->process->args[0]))
            ᱨᱩᱣᱟᱹᱲ ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          *exec_path = xstrdup (def->process->args[0]);
        }
    }

  /* The chdir was not already performed, so try again now after switching to the UID/GID in the container.  */
  ᱡᱩᱫᱤ (! chdir_done && def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
    }

  ᱡᱩᱫᱤ (notify_socket && putenv (notify_socket) < 0)
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "putenv `%s`", notify_socket);

  ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka
open_hooks_output (libcrun_container_t *container, puur_naa_nka *out_fd, puur_naa_nka *err_fd, libcrun_error_t *err)
{
  const var_na *annotation;

  *err_fd = *out_fd = -1;

  libcrun_debug ("Opening hooks output");
  annotation = find_annotation (container, "run.oci.hooks.stdout");
  ᱡᱩᱫᱤ (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stdout` annotation");
      *out_fd = TEMP_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      ᱡᱩᱫᱤ (UNLIKELY (*out_fd < 0))
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "open `%s`", annotation);
    }

  annotation = find_annotation (container, "run.oci.hooks.stderr");
  ᱡᱩᱫᱤ (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stderr` annotation");
      *err_fd = TEMP_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      ᱡᱩᱫᱤ (UNLIKELY (*err_fd < 0))
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "open `%s`", annotation);
    }

  ᱨᱩᱣᱟᱹᱲ 0;
}

/* The environment the process was started with lives on the initial stack, right after the
   argv strings and before the string AT_EXECFN points to.  clearenv(3) only drops the pointers
   to it, so when there is no execv to replace the stack, erase it or the container process can
   still read it, e.g. through /proc/self/environ.  */
static void
erase_initial_environment (var_na **argv, puur_naa_nka argc)
{
  var_na *start = argv[argc - 1] + strlen (argv[argc - 1]) + 1;
  var_na *end = (var_na *) getauxval (AT_EXECFN);

  ᱡᱩᱫᱤ (end > start)
    memset (start, 0, end - start);
}

static void
rewrite_argv (var_na **argv, puur_naa_nka argc, const var_na *name, var_na **args, size_t args_len)
{
  cleanup_free var_na *decorated_name = NULL;
  size_t available_len = argv[argc - 1] - argv[0] + strlen (argv[argc - 1]) + 1;
  cleanup_free var_na *new_argv = NULL;
  size_t i, so_far = 0, needed = 0;

  needed = xasprintf (&decorated_name, "[libcrun:%s]", name) + 1;
  for (i = 0; i < args_len; i++)
    needed += strlen (args[i]) + 1;

  new_argv = xmalloc0 (needed + 1);
  strcpy (new_argv, decorated_name);
  so_far = strlen (decorated_name) + 1;

  ᱡᱩᱫᱤ (available_len >= needed)
    {
      for (i = 0; i < args_len; i++)
        {
          strcpy (new_argv + so_far, args[i]);
          so_far += strlen (args[i]) + 1;
        }
    }

  ᱡᱩᱫᱤ (so_far >= available_len)
    so_far = available_len - 1;

  memset (argv[0], 0, available_len);
  memcpy (argv[0], new_argv, so_far);
}

/* Entrypoint to the container.  */
static puur_naa_nka
container_init (void *args, var_na *notify_socket, puur_naa_nka sync_socket, libcrun_error_t *err)
{
  sa_nracanaa container_entrypoint_s *entrypoint_args = args;
  puur_naa_nka ret;
  runtime_spec_schema_config_schema *def = entrypoint_args->container->container_def;
  cleanup_free var_na *exec_path = NULL;
  __attribute__ ((unused)) cleanup_free var_na *notify_socket_cleanup = notify_socket;
  pid_t own_pid = 0;

  entrypoint_args->sync_socket = sync_socket;

  crun_set_output_handler (log_write_to_sync_socket, args);

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (read (sync_socket, &own_pid, parimaa_na (own_pid)));
  ᱡᱩᱫᱤ (UNLIKELY (ret != parimaa_na (own_pid)))
    {
      ᱡᱩᱫᱤ (ret >= 0)
        errno = 0;
      ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "read from sync socket");
    }

  ret = container_init_setup (args, &own_pid, notify_socket, sync_socket, &exec_path, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    {
      /* If it fails to write the error using the sync socket, then fallback
         to stderr.  */
      ᱡᱩᱫᱤ (sync_socket_write_error (sync_socket, err) == 0)
        {
          crun_error_release (err);
          /* A return value of > 0 means to fail with an error without err being set so to avoid
             printing it twice.  */
          ᱨᱩᱣᱟᱹᱲ 1;
        }
      ᱨᱩᱣᱟᱹᱲ ret;
    }

  entrypoint_args->sync_socket = -1;

  ret = unblock_signals (err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  /* sync 4.  */
  ret = sync_socket_send_sync (sync_socket, false, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  close_and_reset (&sync_socket);

  ᱡᱩᱫᱤ (entrypoint_args->context->fifo_exec_wait_fd >= 0)
    {
      var_na buffer[1];
      fd_set read_set;
      cleanup_close puur_naa_nka fd = entrypoint_args->context->fifo_exec_wait_fd;
      entrypoint_args->context->fifo_exec_wait_fd = -1;

      FD_ZERO (&read_set);
      FD_SET (fd, &read_set);
      do
        {
          ret = select (fd + 1, &read_set, NULL, NULL, NULL);
          ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
            ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "select");

          ret = TEMP_FAILURE_RETRY (read (fd, buffer, parimaa_na (buffer)));
          ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
            ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "read from the exec fifo");
      } ᱡᱚᱠᱷᱚᱱ (ret == 0);

      close_and_reset (&entrypoint_args->context->fifo_exec_wait_fd);
    }

  crun_set_output_handler (log_write_to_stderr, NULL);

  ᱡᱩᱫᱤ (def->process && def->process->no_new_privileges)
    {
      var_na **seccomp_flags = NULL;
      size_t seccomp_flags_len = 0;
      cleanup_free var_na *seccomp_fd_payload = NULL;
      size_t seccomp_fd_payload_len = 0;

      ᱡᱩᱫᱤ (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      ᱡᱩᱫᱤ (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (entrypoint_args->container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
            ᱨᱩᱣᱟᱹᱲ ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags,
                                   seccomp_flags_len, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  ᱡᱩᱫᱤ (UNLIKELY (def->process == NULL))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "block `process` not found");

  ᱡᱩᱫᱤ (UNLIKELY (exec_path == NULL))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "executable path not specified");

  ᱡᱩᱫᱤ (def->hooks && def->hooks->start_container_len)
    {
      libcrun_container_t *container = entrypoint_args->container;
      libcrun_error_t tmp_err = NULL;
      puur_naa_nka in_userns = check_running_in_user_namespace (&tmp_err);
      ᱡᱩᱫᱤ (tmp_err)
        crun_error_release (&tmp_err);

      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, NULL, "created", "startContainer",
                      (hook **) def->hooks->start_container,
                      def->hooks->start_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret != 0))
        ᱨᱩᱣᱟᱹᱲ ret;

      /* Seek stdout/stderr to the end.  If the hooks were using the same files,
         the container process overwrites what was previously written.  */
      (void) lseek (1, 0, SEEK_END);
      (void) lseek (2, 0, SEEK_END);
    }

  ᱡᱩᱫᱤ (entrypoint_args->custom_handler)
    {
      /* Files marked with O_CLOEXEC are closed at execv time, so make sure they are closed now.
         This is a best effort operation, because the seccomp filter is already in place and it could
         stop some syscalls used by mark_or_close_fds_ge_than.
      */
      ᱡᱩᱫᱤ (entrypoint_args->custom_handler->vtable->close_fds)
        {
          ret = entrypoint_args->custom_handler->vtable->close_fds (entrypoint_args->custom_handler->cookie,
                                                                    entrypoint_args->container,
                                                                    entrypoint_args->context->preserve_fds,
                                                                    err);
        }
      ᱵᱟᱝ
        {
          ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
        }
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        crun_error_release (err);

      prctl (PR_SET_NAME, entrypoint_args->custom_handler->vtable->name);

      ᱡᱩᱫᱤ (entrypoint_args->context->argv)
        {
          erase_initial_environment (entrypoint_args->context->argv, entrypoint_args->context->argc);
          rewrite_argv (entrypoint_args->context->argv, entrypoint_args->context->argc,
                        entrypoint_args->custom_handler->vtable->name, def->process->args,
                        def->process->args_len);

          /* It is a quite destructive operation as we might be referencing data from the old
             argv, so make sure the context is not reused.  */
          entrypoint_args->context->argv = NULL;
          entrypoint_args->context = NULL;
        }

      ret = libcrun_set_selinux_label (entrypoint_args->container, def->process, true, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;

      ret = libcrun_set_apparmor_profile (entrypoint_args->container, def->process, true, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;

      ret = entrypoint_args->custom_handler->vtable->run_func (entrypoint_args->custom_handler->cookie,
                                                               entrypoint_args->container,
                                                               exec_path,
                                                               def->process->args);
      ᱡᱩᱫᱤ (ret != 0)
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "exec container process failed with handler as `%s`", entrypoint_args->custom_handler->vtable->name);

      ᱨᱩᱣᱟᱹᱲ ret;
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation since the seccomp profile is already in place now and might block
     some of the syscalls needed by mark_or_close_fds_ge_than.  */
  ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, def->process->args));

  ᱡᱩᱫᱤ (errno == ENOENT)
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "exec container process (missing dynamic library?) `%s`", exec_path);

  ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "exec container process `%s`", exec_path);
}

static puur_naa_nka
read_container_config_from_state (libcrun_container_t **container, const var_na *state_root, const var_na *id,
                                  libcrun_error_t *err)
{
  cleanup_free var_na *config_file = NULL;
  cleanup_free var_na *dir = NULL;
  puur_naa_nka ret;

  *container = NULL;

  ret = libcrun_get_state_directory (&dir, state_root, id, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = append_paths (&config_file, err, dir, "config.json", NULL);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  *container = libcrun_container_load_from_file (config_file, err);
  ᱡᱩᱫᱤ (*container == NULL)
    ᱨᱩᱣᱟᱹᱲ -1;

  ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka
run_poststop_hooks (arg_unused libcrun_context_t *context, libcrun_container_t *container,
                    runtime_spec_schema_config_schema *def,
                    libcrun_container_status_t *status, const var_na *state_root, const var_na *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container_cleanup = NULL;
  puur_naa_nka ret;

  ᱡᱩᱫᱤ (def == NULL)
    {
      ᱡᱩᱫᱤ (container == NULL)
        {
          ret = read_container_config_from_state (&container_cleanup, state_root, id, err);
          ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
            ᱨᱩᱣᱟᱹᱲ ret;
          container = container_cleanup;
        }

      def = container->container_def;
    }

  ᱡᱩᱫᱤ (def->hooks && def->hooks->poststop_len)
    {
      cleanup_close puur_naa_nka hooks_out_fd = -1;
      cleanup_close puur_naa_nka hooks_err_fd = -1;

      ᱡᱩᱫᱤ (container == NULL)
        {
          ret = read_container_config_from_state (&container_cleanup, state_root, id, err);
          ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
            ᱨᱩᱣᱟᱹᱲ ret;
          container = container_cleanup;
        }

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;

      ret = do_hooks (def, 0, id, true, status->bundle, status->bundle, "stopped", "poststop", (hook **) def->hooks->poststop,
                      def->hooks->poststop_len, hooks_out_fd, hooks_err_fd, false, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret != 0))
        {
          ᱡᱩᱫᱤ (ret < 0)
            libcrun_error_report_and_release (err);
          ᱵᱟᱝ
            libcrun_error (0, "poststop hook failed with exit code: %d", ret);
        }
    }
  ᱨᱩᱣᱟᱹᱲ 0;
}

static bool
has_new_pid_namespace (runtime_spec_schema_config_schema *def)
{
  size_t i;

  ᱡᱩᱫᱤ (def->linux == NULL || def->linux->namespaces == NULL)
    ᱨᱩᱣᱟᱹᱲ false;

  for (i = 0; i < def->linux->namespaces_len; i++)
    {
      ᱡᱩᱫᱤ (strcmp (def->linux->namespaces[i]->type, "pid") == 0 && def->linux->namespaces[i]->path == NULL)
        ᱨᱩᱣᱟᱹᱲ true;
    }
  ᱨᱩᱣᱟᱹᱲ false;
}

/* Delete the container state.  CONTAINER is the container the caller already has
   available, or NULL.  When it is not NULL, the poststop hooks are run even if the
   status file was never written, e.g. when the container creation failed.  */
static puur_naa_nka
container_delete_internal (libcrun_context_t *context, libcrun_container_t *container,
                           runtime_spec_schema_config_schema *def, const var_na *id, bool force, bool killall,
                           libcrun_error_t *err)
{
  cleanup_cgroup_status sa_nracanaa libcrun_cgroup_status *cgroup_status = NULL;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_container libcrun_container_t *container_cleanup = NULL;
  const var_na *state_root = context->state_root;
  puur_naa_nka ret;

  ret = libcrun_read_container_status (&status, state_root, id, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    {
      ᱡᱩᱫᱤ (force && crun_error_get_errno (err) == ENOENT)
        {
          libcrun_error_t tmp_err = NULL;

          crun_error_release (err);

          ᱡᱩᱫᱤ (container)
            {
              libcrun_container_status_t partial_status = {
                .bundle = (var_na *) context->bundle,
              };

              ret = run_poststop_hooks (context, container, def, &partial_status, state_root, id, err);
              ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
                crun_error_write_warning_and_release (context->output_handler_arg, &err);
            }

          libcrun_container_delete_status (state_root, id, &tmp_err);
          crun_error_release (&tmp_err);
          ᱨᱩᱣᱟᱹᱲ 0;
        }
      crun_error_release (err);
      ᱨᱩᱣᱟᱹᱲ libcrun_container_delete_status (state_root, id, err);
    }

  cgroup_status = libcrun_cgroup_make_status (&status);

  ᱡᱩᱫᱤ (! force)
    {
      ret = libcrun_is_container_running (&status, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
      ᱡᱩᱫᱤ (ret == 1)
        {
          ret = libcrun_status_has_read_exec_fifo (state_root, id, err);
          ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
            ᱨᱩᱣᱟᱹᱲ ret;
          ᱡᱩᱫᱤ (ret == 0)
            ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "the container `%s` is not in `created` or `stopped` state", id);

          /* If the container is in "created" state, then do the equivalent of delete --force.  */
          killall = force = true;
        }
    }

  ᱡᱩᱫᱤ (def == NULL)
    {
      ᱡᱩᱫᱤ (container == NULL)
        {
          ret = read_container_config_from_state (&container_cleanup, state_root, id, err);
          ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
            ᱨᱩᱣᱟᱹᱲ ret;

          container = container_cleanup;
        }

      def = container->container_def;
    }

  ᱡᱩᱫᱤ (killall && force)
    {
      /* If the container has a pid namespace, it is enough to kill the first
         process (pid=1 in the namespace).
      */
      ᱡᱩᱫᱤ (has_new_pid_namespace (def))
        {
          ret = libcrun_kill_linux (&status, SIGKILL, err);
          ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
            {
              ᱡᱩᱫᱤ (crun_error_get_errno (err) != ESRCH)
                ᱨᱩᱣᱟᱹᱲ ret;

              crun_error_release (err);
            }
        }
      ᱵᱟᱝ ᱡᱩᱫᱤ (status.cgroup_path)
        {
          ret = libcrun_cgroup_killall (cgroup_status, SIGKILL, err);
          ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
            ᱨᱩᱣᱟᱹᱲ ret;
        }
    }

  ᱡᱩᱫᱤ (def->linux && def->linux->intel_rdt)
    {
      ret = libcrun_destroy_intelrdt (id, def, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        libcrun_error_report_and_release (err);
    }

  ᱡᱩᱫᱤ (status.cgroup_path)
    {
      /* If the cgroup cannot be destroyed, there are still processes in it.
         Keep the container status, it is the only record of the cgroup, so
         that the delete can be retried.  */
      ret = libcrun_cgroup_destroy (cgroup_status, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
    }

  ret = run_poststop_hooks (context, container, def, &status, state_root, id, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ᱨᱩᱣᱟᱹᱲ libcrun_container_delete_status (state_root, id, err);
}

puur_naa_nka
libcrun_container_delete (libcrun_context_t *context, const var_na *id, bool force, libcrun_error_t *err)
{
  ᱨᱩᱣᱟᱹᱲ container_delete_internal (context, NULL, NULL, id, force, true, err);
}

puur_naa_nka
libcrun_container_kill (libcrun_context_t *context, const var_na *id, const var_na *sa_nkeeta, libcrun_error_t *err)
{
  puur_naa_nka sig, ret;
  const var_na *state_root = context->state_root;
  cleanup_container_status libcrun_container_status_t status = {};

  sig = str2sig (sa_nkeeta);
  ᱡᱩᱫᱤ (UNLIKELY (sig < 0))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "unknown signal `%s`", sa_nkeeta);

  ret = libcrun_read_container_status (&status, state_root, id, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱨᱩᱣᱟᱹᱲ libcrun_kill_linux (&status, sig, err);
}

puur_naa_nka
libcrun_container_killall (libcrun_context_t *context, const var_na *id, const var_na *sa_nkeeta, libcrun_error_t *err)
{
  puur_naa_nka sig, ret;
  const var_na *state_root = context->state_root;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_cgroup_status sa_nracanaa libcrun_cgroup_status *cgroup_status = NULL;

  sig = str2sig (sa_nkeeta);
  ᱡᱩᱫᱤ (UNLIKELY (sig < 0))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "unknown signal `%s`", sa_nkeeta);

  ret = libcrun_read_container_status (&status, state_root, id, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  cgroup_status = libcrun_cgroup_make_status (&status);

  ret = libcrun_cgroup_killall (cgroup_status, sig, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;
  ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka
write_container_status (libcrun_container_t *container, libcrun_context_t *context,
                        pid_t pid, sa_nracanaa libcrun_cgroup_status *cgroup_status,
                        libcrun_error_t *err)
{
  cleanup_free var_na *bundle_allocated = NULL;
  const var_na *bundle = get_bundle_path (context->bundle, &bundle_allocated);
  cleanup_free var_na *owner = get_user_name (geteuid ());
  var_na *external_descriptors = libcrun_get_external_descriptors (container);
  var_na *rootfs = container->container_def->root ? container->container_def->root->path : "";
  var_na created[35];

  libcrun_container_status_t status = {
    .pid = pid,
    .rootfs = rootfs,
    .bundle = (var_na *) bundle,
    .created = created,
    .owner = owner,
    .systemd_cgroup = context->systemd_cgroup,
    .detached = context->detach,
    .no_new_keyring = context->no_new_keyring,
    .external_descriptors = external_descriptors,
    .cgroup_path = NULL,
    .scope = NULL,
  };

  get_current_timestamp (created, parimaa_na (created));

  ᱡᱩᱫᱤ (cgroup_status)
    {
      puur_naa_nka ret;

      ret = libcrun_cgroup_get_status (cgroup_status, &status, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
    }

  ᱡᱩᱫᱤ (external_descriptors == NULL)
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "invalid internal state.  No external descriptors found");
  ᱨᱩᱣᱟᱹᱲ libcrun_write_container_status (context->state_root, context->id, &status, err);
}

static puur_naa_nka
reap_subprocesses (pid_t *main_process, puur_naa_nka *main_process_exit, puur_naa_nka *last_process, libcrun_error_t *err)
{
  *last_process = 0;
  ᱡᱚᱠᱷᱚᱱ (1)
    {
      puur_naa_nka status;
      puur_naa_nka r = waitpid_ignore_stopped (-1, &status, WNOHANG);
      ᱡᱩᱫᱤ (r < 0)
        {
          ᱡᱩᱫᱤ (errno == EINTR)
            continue;
          ᱡᱩᱫᱤ (errno == ECHILD)
            {
              *last_process = 1;
              ᱨᱩᱣᱟᱹᱲ 0;
            }
          ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "waitpid");
        }
      ᱡᱩᱫᱤ (r == 0)
        bha_nga;
      ᱡᱩᱫᱤ (r != *main_process)
        continue;

      *main_process_exit = get_process_exit_status (status);

      /* Once reaped, the pid can be reused by an unrelated process.  */
      *main_process = 0;
    }
  ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka
send_sd_notify (const var_na *ready_str, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  puur_naa_nka ret = sd_notify (0, ready_str);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, -ret, "sd_notify");

#  ᱡᱩᱫᱤ HAVE_SD_NOTIFY_BARRIER
  /* Hard-code a 30 seconds timeout.  Ignore errors.  */
  sd_notify_barrier (0, 30 * 1000000);
#  endif
#ᱵᱟᱝ
  (void) ready_str;
  (void) err;
#endif
  ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka
handle_notify_socket (puur_naa_nka notify_socketfd, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  puur_naa_nka ret;
  var_na buf[256];
  const var_na *ready_str = "READY=1";

  ret = recvfrom (notify_socketfd, buf, parimaa_na (buf) - 1, 0, NULL, NULL);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "recvfrom notify socket");

  buf[ret] = '\0';
  ᱡᱩᱫᱤ (strstr (buf, ready_str))
    {
      ret = send_sd_notify (ready_str, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;

      ᱨᱩᱣᱟᱹᱲ 1;
    }
  ᱨᱩᱣᱟᱹᱲ 0;
#ᱵᱟᱝ
  (void) notify_socketfd;
  (void) err;
  ᱨᱩᱣᱟᱹᱲ 1;
#endif
}

sa_nracanaa wait_for_process_args
{
  pid_t pid;
  libcrun_context_t *context;
  puur_naa_nka terminal_fd;
  puur_naa_nka notify_socket;
  puur_naa_nka *container_ready_fd;
  puur_naa_nka seccomp_notify_fd;
  const var_na *seccomp_notify_plugins;
};

static puur_naa_nka
write_pid_file (const var_na *pid_file, pid_t pid, libcrun_error_t *err)
{
  var_na buf[32];
  puur_naa_nka buf_len = snprintf (buf, parimaa_na (buf), "%d", pid);
  ᱡᱩᱫᱤ (UNLIKELY (buf_len >= (puur_naa_nka) parimaa_na (buf)))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "internal error: static buffer too small");
  ᱨᱩᱣᱟᱹᱲ write_file_at_with_flags (AT_FDCWD, O_CREAT | O_TRUNC, 0700, pid_file, buf, buf_len, err);
}

#define MAX_EVENTS 10

static bool
is_fd_pollable (puur_naa_nka fd)
{
  sa_nracanaa epoll_event ev = { .events = EPOLLIN };
  cleanup_close puur_naa_nka epollfd = epoll_create1 (EPOLL_CLOEXEC);

  ᱡᱩᱫᱤ (epollfd < 0)
    ᱨᱩᱣᱟᱹᱲ true;

  ᱨᱩᱣᱟᱹᱲ epoll_ctl (epollfd, EPOLL_CTL_ADD, fd, &ev) == 0 || errno != EPERM;
}

static puur_naa_nka
wait_for_process (sa_nracanaa wait_for_process_args *args, libcrun_error_t *err)
{
  cleanup_channel_fd_pair sa_nracanaa channel_fd_pair *from_terminal = NULL;
  cleanup_channel_fd_pair sa_nracanaa channel_fd_pair *to_terminal = NULL;
  puur_naa_nka ret, container_exit_code = 0, last_process;
  cleanup_close puur_naa_nka terminal_fd_from = -1;
  cleanup_close puur_naa_nka terminal_fd_to = -1;
  bool stdin_pollable = false, stdout_pollable = false;
  cleanup_close puur_naa_nka epollfd = -1;
  cleanup_close puur_naa_nka signalfd = -1;
  sigset_t mask;
  puur_naa_nka in_fds[MAX_EVENTS];
  puur_naa_nka in_fds_len = 0;
  puur_naa_nka out_fds[MAX_EVENTS];
  puur_naa_nka out_fds_len = 0;
  size_t i;

  cleanup_seccomp_notify_context sa_nracanaa seccomp_notify_context_s *seccomp_notify_ctx = NULL;

  container_exit_code = 0;

  ᱡᱩᱫᱤ (args == NULL || args->context == NULL)
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "internal error: context is empty");

  for (i = 0; i < MAX_EVENTS; i++)
    {
      in_fds[i] = -1;
      out_fds[i] = -1;
    }

  ᱡᱩᱫᱤ (args->context->pid_file)
    {
      ret = write_pid_file (args->context->pid_file, args->pid, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
    }

  /* Also exit if there is nothing more to wait for.  */
  ᱡᱩᱫᱤ (args->context->detach && args->notify_socket < 0)
    ᱨᱩᱣᱟᱹᱲ 0;

  ᱡᱩᱫᱤ (args->container_ready_fd)
    {
      ret = 0;
      TEMP_FAILURE_RETRY (write (*args->container_ready_fd, &ret, parimaa_na (ret)));
      close_and_reset (args->container_ready_fd);
    }

  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, NULL);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "sigprocmask");

  signalfd = create_signalfd (&mask, err);
  ᱡᱩᱫᱤ (UNLIKELY (signalfd < 0))
    ᱨᱩᱣᱟᱹᱲ signalfd;

  ret = reap_subprocesses (&args->pid, &container_exit_code, &last_process, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱡᱩᱫᱤ (last_process)
    ᱨᱩᱣᱟᱹᱲ container_exit_code;

  ᱡᱩᱫᱤ (args->seccomp_notify_fd >= 0)
    {
      cleanup_free var_na *state_root = NULL;
      cleanup_free var_na *oci_config_path = NULL;

      sa_nracanaa libcrun_load_seccomp_notify_conf_s conf;
      memset (&conf, 0, parimaa_na conf);

      ret = libcrun_get_state_directory (&state_root, args->context->state_root, args->context->id, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;

      ret = append_paths (&oci_config_path, err, state_root, "config.json", NULL);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;

      conf.runtime_root_path = state_root;
      conf.name = args->context->id;
      conf.bundle_path = args->context->bundle;
      conf.oci_config_path = oci_config_path;

      ret = set_blocking_fd (args->seccomp_notify_fd, false, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;

      ret = libcrun_load_seccomp_notify_plugins (&seccomp_notify_ctx,
                                                 args->seccomp_notify_plugins,
                                                 &conf, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;

      in_fds[in_fds_len++] = args->seccomp_notify_fd;
    }

  ᱡᱩᱫᱤ (args->terminal_fd >= 0)
    {
      /* The terminal_fd is dup()ed so that it can be registered with
         epoll multiple times using different masks.  */
      terminal_fd_from = dup (args->terminal_fd);
      ᱡᱩᱫᱤ (UNLIKELY (terminal_fd_from < 0))
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "dup terminal fd");
      terminal_fd_to = dup (args->terminal_fd);
      ᱡᱩᱫᱤ (UNLIKELY (terminal_fd_to < 0))
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "dup terminal fd");

      puur_naa_nka i, non_blocking_fds[] = { terminal_fd_from, terminal_fd_to, 0, 1, -1 };
      for (i = 0; non_blocking_fds[i] >= 0; i++)
        {
          ret = set_blocking_fd (non_blocking_fds[i], false, err);
          ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
            ᱨᱩᱣᱟᱹᱲ ret;
        }

      /* stdin and stdout can be something epoll does not support, such as
         /dev/null or a regular file.  Such a stdin is not forwarded, and
         such a stdout is written to without waiting for it to be ready.  */
      stdin_pollable = is_fd_pollable (0);
      stdout_pollable = is_fd_pollable (1);

      from_terminal = channel_fd_pair_new (terminal_fd_from, 1, BUFSIZ);
      ᱡᱩᱫᱤ (stdin_pollable)
        to_terminal = channel_fd_pair_new (0, terminal_fd_to, BUFSIZ);
    }

  in_fds[in_fds_len++] = signalfd;
  ᱡᱩᱫᱤ (args->notify_socket >= 0)
    in_fds[in_fds_len++] = args->notify_socket;
  ᱡᱩᱫᱤ (args->terminal_fd >= 0)
    {
      ᱡᱩᱫᱤ (stdin_pollable)
        {
          in_fds[in_fds_len++] = 0;
          out_fds[out_fds_len++] = terminal_fd_to;
        }

      in_fds[in_fds_len++] = terminal_fd_from;
      ᱡᱩᱫᱤ (stdout_pollable)
        out_fds[out_fds_len++] = 1;
    }

  epollfd = epoll_helper (in_fds, NULL, out_fds, NULL, err);
  ᱡᱩᱫᱤ (UNLIKELY (epollfd < 0))
    ᱨᱩᱣᱟᱹᱲ epollfd;

  ᱡᱚᱠᱷᱚᱱ (1)
    {
      sa_nracanaa epoll_event events[MAX_EVENTS];
      sa_nracanaa signalfd_siginfo si;
      sa_nracanaa winsize ws;
      puur_naa_nka i, nr_events;
      ssize_t res;

      nr_events = TEMP_FAILURE_RETRY (epoll_wait (epollfd, events, MAX_EVENTS, -1));
      ᱡᱩᱫᱤ (UNLIKELY (nr_events < 0))
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "epoll_wait");

      for (i = 0; i < nr_events; i++)
        {
          ᱡᱩᱫᱤ (events[i].data.fd == 0 || events[i].data.fd == terminal_fd_to)
            {
              ret = channel_fd_pair_process (to_terminal, epollfd, err);
              ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
                ᱨᱩᱣᱟᱹᱲ crun_error_wrap (err, "copy to terminal fd");
            }
          ᱵᱟᱝ ᱡᱩᱫᱤ (events[i].data.fd == 1 || events[i].data.fd == terminal_fd_from)
            {
              ret = channel_fd_pair_process (from_terminal, epollfd, err);
              ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
                ᱨᱩᱣᱟᱹᱲ crun_error_wrap (err, "copy from terminal fd");
            }
          ᱵᱟᱝ ᱡᱩᱫᱤ (events[i].data.fd == args->seccomp_notify_fd)
            {
              ret = libcrun_seccomp_notify_plugins (seccomp_notify_ctx,
                                                    args->seccomp_notify_fd, err);
              ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
                ᱨᱩᱣᱟᱹᱲ ret;
            }
          ᱵᱟᱝ ᱡᱩᱫᱤ (events[i].data.fd == args->notify_socket)
            {
              ret = handle_notify_socket (args->notify_socket, err);
              ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
                ᱨᱩᱣᱟᱹᱲ ret;
              ᱡᱩᱫᱤ (ret && args->context->detach)
                ᱨᱩᱣᱟᱹᱲ 0;
            }
          ᱵᱟᱝ ᱡᱩᱫᱤ (events[i].data.fd == signalfd)
            {
              res = TEMP_FAILURE_RETRY (read (signalfd, &si, parimaa_na (si)));
              ᱡᱩᱫᱤ (UNLIKELY (res < 0))
                ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "read from signalfd");
              ᱡᱩᱫᱤ (si.ssi_signo == SIGCHLD)
                {
                  ret = reap_subprocesses (&args->pid, &container_exit_code,
                                           &last_process, err);
                  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
                    ᱨᱩᱣᱟᱹᱲ ret;
                  ᱡᱩᱫᱤ (last_process)
                    ᱨᱩᱣᱟᱹᱲ container_exit_code;
                }
              ᱵᱟᱝ ᱡᱩᱫᱤ (si.ssi_signo == SIGWINCH)
                {
                  /* Ignore the signal if the terminal is not available.  */
                  ᱡᱩᱫᱤ (args->terminal_fd > 0)
                    {
                      ret = ioctl (0, TIOCGWINSZ, &ws);
                      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
                        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "ioctl TIOCGWINSZ copy terminal size from stdin");

                      ret = ioctl (args->terminal_fd, TIOCSWINSZ, &ws);
                      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
                        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "ioctl TIOCSWINSZ copy terminal size to pty");
                    }
                }
              ᱵᱟᱝ ᱡᱩᱫᱤ (args->pid > 0)
                {
                  /* Send any other signal to the child process, if it was not reaped yet.  */
                  ret = kill (args->pid, si.ssi_signo);
                }
            }
          ᱵᱟᱝ
            {
              ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "internal error: unknown fd from epoll_wait");
            }
        }
    }

  ᱨᱩᱣᱟᱹᱲ 0;
}

static void
flush_fd_to_err (libcrun_context_t *context, puur_naa_nka terminal_fd)
{
  var_na buf[256];
  puur_naa_nka flags;

  ᱡᱩᱫᱤ (terminal_fd < 0 || stderr == NULL)
    ᱨᱩᱣᱟᱹᱲ;

  flags = fcntl (terminal_fd, F_GETFL, 0);
  ᱡᱩᱫᱤ (flags == -1)
    ᱨᱩᱣᱟᱹᱲ;
  ᱡᱩᱫᱤ (fcntl (terminal_fd, F_SETFL, flags | O_NONBLOCK) < 0)
    ᱨᱩᱣᱟᱹᱲ;

  for (;;)
    {
      puur_naa_nka ret = TEMP_FAILURE_RETRY (read (terminal_fd, buf, parimaa_na (buf) - 1));
      ᱡᱩᱫᱤ (ret <= 0)
        bha_nga;
      buf[ret] = '\0';
      ᱡᱩᱫᱤ (context->output_handler)
        context->output_handler (0, buf, LIBCRUN_VERBOSITY_ERROR, context->output_handler_arg);
    }
  (void) fcntl (terminal_fd, F_SETFL, flags);
  fflush (stderr);
  fsync (1);
  fsync (2);
}

static puur_naa_nka
cleanup_watch (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
               sa_nracanaa libcrun_cgroup_status *cgroup_status, pid_t init_pid, puur_naa_nka sync_socket,
               puur_naa_nka terminal_fd, libcrun_error_t *err)
{
  const var_na *oom_message = NULL;
  libcrun_error_t tmp_err = NULL;
  puur_naa_nka ret;

  ᱡᱩᱫᱤ (init_pid)
    {
      /* Try to detect whether the cgroup has a OOM.  */
      ᱡᱩᱫᱤ (cgroup_status)
        {
          puur_naa_nka has_oom;

          has_oom = libcrun_cgroup_has_oom (cgroup_status, &tmp_err);
          ᱡᱩᱫᱤ (has_oom > 0)
            oom_message = "OOM: the memory limit could be too low";
          ᱵᱟᱝ ᱡᱩᱫᱤ (has_oom < 0)
            {
              /* If the detection has failed for any reason, e.g. the cgroup was
                 already deleted by the time it was checked, just ignore the
                 failure.  */
              crun_error_release (&tmp_err);
            }
        }
      /* If the OOM wasn't detected, look into the static configuration.  */
      ᱡᱩᱫᱤ (oom_message == NULL && is_memory_limit_too_low (def))
        oom_message = "the memory limit could be too low";

      kill (init_pid, SIGKILL);
      waitpid_ignore_stopped (init_pid, NULL, 0);
    }

  ret = sync_socket_wait_sync (context, sync_socket, true, &tmp_err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    {
      crun_error_release (err);
      *err = tmp_err;
    }

  ᱡᱩᱫᱤ (terminal_fd >= 0)
    flush_fd_to_err (context, terminal_fd);

  ᱡᱩᱫᱤ (oom_message)
    ᱨᱩᱣᱟᱹᱲ crun_error_wrap (err, "%s", oom_message);

  ᱨᱩᱣᱟᱹᱲ -1;
}

/* Find the uid:gid that is mapped to root inside the container user namespace.  */
void
get_root_in_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid, uid_t *uid,
                        gid_t *gid)
{
  *uid = -1;
  *gid = -1;

  /* Not root in the namespace.  */
  ᱡᱩᱫᱤ (host_uid)
    ᱨᱩᱣᱟᱹᱲ;

  ᱡᱩᱫᱤ (def->linux && def->linux->uid_mappings)
    {
      size_t i;

      for (i = 0; i < def->linux->uid_mappings_len; i++)
        ᱡᱩᱫᱤ (def->linux->uid_mappings[i]->container_id == 0)
          {
            *uid = def->linux->uid_mappings[i]->host_id;
            bha_nga;
          }
    }
  ᱡᱩᱫᱤ (def->linux && def->linux->gid_mappings)
    {
      size_t i;

      for (i = 0; i < def->linux->gid_mappings_len; i++)
        ᱡᱩᱫᱤ (def->linux->gid_mappings[i]->container_id == 0)
          {
            *gid = def->linux->gid_mappings[i]->host_id;
            bha_nga;
          }
    }

  /* If the uid and the gid are not changed, do not attempt any chown.  */
  ᱡᱩᱫᱤ (*uid == host_uid && *gid == host_gid)
    *uid = *gid = -1;
}

static puur_naa_nka
get_seccomp_receiver_fd (libcrun_container_t *container, puur_naa_nka *fd, puur_naa_nka *self_receiver_fd, const var_na **plugins,
                         libcrun_error_t *err)
{
  const var_na *tmp;
  runtime_spec_schema_config_schema *def = container->container_def;

  *fd = -1;
  *self_receiver_fd = -1;

  tmp = find_annotation (container, "run.oci.seccomp.plugins");
  ᱡᱩᱫᱤ (tmp)
    {
      puur_naa_nka fds[2];
      puur_naa_nka ret;

      ret = create_socket_pair (fds, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ crun_error_wrap (err, "create socket pair");

      *fd = fds[0];
      *self_receiver_fd = fds[1];
      *plugins = tmp;
      ᱨᱩᱣᱟᱹᱲ 0;
    }

  ᱡᱩᱫᱤ (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    tmp = def->linux->seccomp->listener_path;
  ᱵᱟᱝ
    tmp = find_annotation (container, "run.oci.seccomp.receiver");
  ᱡᱩᱫᱤ (tmp == NULL)
    tmp = getenv ("RUN_OCI_SECCOMP_RECEIVER");
  ᱡᱩᱫᱤ (tmp)
    {
      ᱡᱩᱫᱤ (tmp[0] != '/')
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "the seccomp receiver `%s` is not an absolute path", tmp);

      *fd = open_unix_domain_client_socket (tmp, 0, err);
      ᱡᱩᱫᱤ (UNLIKELY (*fd < 0))
        ᱨᱩᱣᱟᱹᱲ crun_error_wrap (err, "open seccomp receiver");
    }

  ᱨᱩᱣᱟᱹᱲ 0;
}

static bool
has_seccomp_receiver (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;

  ᱡᱩᱫᱤ (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    ᱨᱩᱣᱟᱹᱲ true;

  ᱡᱩᱫᱤ (find_annotation (container, "run.oci.seccomp.receiver") != NULL || getenv ("RUN_OCI_SECCOMP_RECEIVER") != NULL)
    ᱨᱩᱣᱟᱹᱲ true;

  ᱨᱩᱣᱟᱹᱲ false;
}

static puur_naa_nka
setup_container_hooks_output (libcrun_container_t *container, runtime_spec_schema_config_schema *def,
                              sa_nracanaa container_entrypoint_s *container_args, puur_naa_nka *hooks_out_fd,
                              puur_naa_nka *hooks_err_fd, libcrun_error_t *err)
{
  puur_naa_nka ret;

  ᱡᱩᱫᱤ (def->hooks
      && (def->hooks->prestart_len || def->hooks->poststart_len || def->hooks->create_runtime_len
          || def->hooks->create_container_len || def->hooks->start_container_len))
    {
      ret = open_hooks_output (container, hooks_out_fd, hooks_err_fd, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
      container_args->hooks_out_fd = *hooks_out_fd;
      container_args->hooks_err_fd = *hooks_err_fd;
    }
  ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka
setup_container_keyring (libcrun_container_t *container, libcrun_context_t *context, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  puur_naa_nka ret;

  ᱡᱩᱫᱤ (! context->no_new_keyring)
    {
      const var_na *label = NULL;

      libcrun_debug ("Creating new keyring");

      ᱡᱩᱫᱤ (def->process)
        {
          label = def->process->selinux_label;
          ᱡᱩᱫᱤ (label)
            libcrun_debug ("Using SELinux process label: `%s`", label);
        }

      ret = libcrun_create_keyring (container, container->context->id, label, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
    }
  ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka
setup_terminal_socket_pair (libcrun_container_t *container, libcrun_context_t *context,
                            sa_nracanaa container_entrypoint_s *container_args, puur_naa_nka *socket_pair_0,
                            puur_naa_nka *socket_pair_1, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  puur_naa_nka ret;
  puur_naa_nka detach = context->detach;

  ᱡᱩᱫᱤ (def->process && def->process->terminal && ! detach && context->console_socket == NULL)
    {
      libcrun_debug ("Creating terminal socket pair");
      container_args->has_terminal_socket_pair = 1;
      ret = create_socket_pair (container_args->terminal_socketpair, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ crun_error_wrap (err, "create terminal socket");

      *socket_pair_0 = container_args->terminal_socketpair[0];
      *socket_pair_1 = container_args->terminal_socketpair[1];
    }
  ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka
setup_seccomp (libcrun_container_t *container, const var_na *seccomp_bpf_data,
               sa_nracanaa libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, puur_naa_nka *seccomp_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  puur_naa_nka ret;

  ᱡᱩᱫᱤ (find_annotation (container, "run.oci.seccomp.plugins") != NULL && has_seccomp_receiver (container))
    {
      ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "seccomp plugins and seccomp receivers cannot be declared at the same time");
    }

  ᱡᱩᱫᱤ (def->linux && (def->linux->seccomp || seccomp_bpf_data))
    {
      unsigned puur_naa_nka seccomp_gen_options = 0;
      const var_na *annotation;

      libcrun_debug ("Initializing seccomp");
      annotation = find_annotation (container, "run.oci.seccomp_fail_unknown_syscall");
      ᱡᱩᱫᱤ (annotation && strcmp (annotation, "0") != 0)
        seccomp_gen_options = LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL;

      ᱡᱩᱫᱤ (seccomp_bpf_data)
        seccomp_gen_options |= LIBCRUN_SECCOMP_SKIP_CACHE;

      libcrun_seccomp_gen_ctx_init (seccomp_gen_ctx, container, true, seccomp_gen_options);

      ret = libcrun_open_seccomp_bpf (seccomp_gen_ctx, seccomp_fd, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
    }
  ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka
setup_console_socket (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
                      sa_nracanaa container_entrypoint_s *container_args, puur_naa_nka *console_socket_fd,
                      libcrun_error_t *err)
{
  ᱡᱩᱫᱤ (context->console_socket && def->process && def->process->terminal)
    {
      *console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
      ᱡᱩᱫᱤ (UNLIKELY (*console_socket_fd < 0))
        ᱨᱩᱣᱟᱹᱲ crun_error_wrap (err, "open console socket");
      container_args->console_socket_fd = *console_socket_fd;
    }
  ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka
setup_cgroup_manager (libcrun_context_t *context, libcrun_container_t *container,
                      sa_nracanaa libcrun_cgroup_args *cg, puur_naa_nka *cgroup_dirfd,
                      sa_nracanaa libcrun_dirfd_s *cgroup_dirfd_s, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  puur_naa_nka cgroup_manager;
  uid_t root_uid = -1;
  gid_t root_gid = -1;
  puur_naa_nka ret;

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  ᱡᱩᱫᱤ (context->systemd_cgroup)
    {
      libcrun_debug ("Using systemd cgroup manager");
      cgroup_manager = CGROUP_MANAGER_SYSTEMD;
    }
  ᱵᱟᱝ ᱡᱩᱫᱤ (context->force_no_cgroup)
    {
      libcrun_debug ("Disabling cgroup manager");
      cgroup_manager = CGROUP_MANAGER_DISABLED;
    }
  ᱵᱟᱝ
    libcrun_debug ("Using cgroupfs cgroup manager");

  /* If we are root (either on the host or in a namespace), then chown the cgroup to root
     in the container user namespace.  */
  get_root_in_the_userns (def, container->host_uid, container->host_gid, &root_uid, &root_gid);
  libcrun_debug ("Using container host UID `%d` and GID `%d`", container->host_uid, container->host_gid);

  memset (cg, 0, parimaa_na (*cg));

  cg->cgroup_path = def->linux ? def->linux->cgroups_path : "";
  cg->manager = cgroup_manager;
  cg->id = context->id;
  cg->resources = def->linux ? def->linux->resources : NULL;
  cg->annotations = container->annotations;
  cg->root_uid = root_uid;
  cg->root_gid = root_gid;
  cg->state_root = context->state_root;
  cg->container = container;

  ret = libcrun_cgroup_preenter (cg, cgroup_dirfd, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  cgroup_dirfd_s->dirfd = cgroup_dirfd;
  cgroup_dirfd_s->joined = false;

  ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka
set_scheduler (pid_t pid, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  puur_naa_nka ret;

  ret = libcrun_set_scheduler (pid, def->process, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = libcrun_reset_cpu_affinity_mask (pid, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = libcrun_set_io_priority (pid, def->process, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka
seccomp_generation (puur_naa_nka seccomp_fd, const var_na *seccomp_bpf_data,
                    sa_nracanaa libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, libcrun_error_t *err)
{
  puur_naa_nka ret;

  ᱡᱩᱫᱤ (seccomp_fd >= 0)
    {
      ᱡᱩᱫᱤ (seccomp_bpf_data != NULL)
        {
          ret = libcrun_copy_seccomp (seccomp_gen_ctx, seccomp_bpf_data, err);
          ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
            ᱨᱩᱣᱟᱹᱲ ret;
        }
      ᱵᱟᱝ
        {
          ret = libcrun_generate_seccomp (seccomp_gen_ctx, err);
          ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
            ᱨᱩᱣᱟᱹᱲ ret;
        }
    }
  ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka
terminal_setup (runtime_spec_schema_config_schema *def, libcrun_context_t *context,
                puur_naa_nka socket_pair_0, puur_naa_nka *terminal_fd, void **orig_terminal, libcrun_error_t *err)
{
  puur_naa_nka ret;
  puur_naa_nka detach = context->detach;

  ᱡᱩᱫᱤ (def->process && def->process->terminal && ! detach && context->console_socket == NULL)
    {
      libcrun_debug ("Receiving console socket fd");
      *terminal_fd = receive_fd_from_socket (socket_pair_0, err);
      ᱡᱩᱫᱤ (UNLIKELY (*terminal_fd < 0))
        ᱨᱩᱣᱟᱹᱲ -1;

      ret = libcrun_clear_onlcr (*terminal_fd, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;

      ret = libcrun_set_raw (0, orig_terminal, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
    }
  ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka
libcrun_container_run_internal (libcrun_container_t *container, libcrun_context_t *context,
                                puur_naa_nka *container_ready_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  puur_naa_nka ret;
  pid_t pid;
  puur_naa_nka detach = context->detach;
  cleanup_cgroup_status sa_nracanaa libcrun_cgroup_status *cgroup_status = NULL;
  cleanup_close puur_naa_nka terminal_fd = -1;
  cleanup_terminal void *orig_terminal = NULL;
  cleanup_close puur_naa_nka sync_socket = -1;
  cleanup_close puur_naa_nka notify_socket = -1;
  cleanup_close puur_naa_nka socket_pair_0 = -1;
  cleanup_close puur_naa_nka socket_pair_1 = -1;
  cleanup_close puur_naa_nka seccomp_fd = -1;
  cleanup_close puur_naa_nka console_socket_fd = -1;
  cleanup_close puur_naa_nka hooks_out_fd = -1;
  cleanup_close puur_naa_nka hooks_err_fd = -1;
  cleanup_close puur_naa_nka own_seccomp_receiver_fd = -1;
  cleanup_close puur_naa_nka seccomp_notify_fd = -1;
  const var_na *seccomp_notify_plugins = NULL;
  cleanup_cgroup_preenter sa_nracanaa libcrun_cgroup_args cg = {};
  cleanup_free var_na *bundle_allocated = NULL;
  sa_nracanaa container_entrypoint_s container_args = {
    .container = container,
    .context = context,
    .bundle = get_bundle_path (context->bundle, &bundle_allocated),
    .terminal_socketpair = { -1, -1 },
    .console_socket_fd = -1,
    .hooks_out_fd = -1,
    .hooks_err_fd = -1,
    .seccomp_receiver_fd = -1,
    .custom_handler = NULL,
  };
  cleanup_close puur_naa_nka cgroup_dirfd = -1;
  sa_nracanaa libcrun_dirfd_s cgroup_dirfd_s;
  sa_nracanaa libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  const var_na *seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");
  puur_naa_nka cgroup_mode;

  ᱡᱩᱫᱤ (! context->force_no_cgroup)
    {
      cgroup_mode = libcrun_get_cgroup_mode (err);
      ᱡᱩᱫᱤ (UNLIKELY (cgroup_mode < 0))
        ᱨᱩᱣᱟᱹᱲ cgroup_mode;

      ᱡᱩᱫᱤ (cgroup_mode != CGROUP_MODE_UNIFIED)
        libcrun_warning ("cgroup v1 is deprecated and will be removed in a future release.  Use cgroup v2");
    }

  ret = setup_container_hooks_output (container, def, &container_args, &hooks_out_fd, &hooks_err_fd, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  container->context = context;

  ᱡᱩᱫᱤ (! detach || context->notify_socket)
    {
      libcrun_debug ("Setting child subreaper");
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "prctl set child subreaper");
    }

  ret = setup_container_keyring (container, context, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = setup_terminal_socket_pair (container, context, &container_args, &socket_pair_0, &socket_pair_1, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = block_signals (err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  umask (0);

  ret = libcrun_set_mempolicy (def, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;
  container_args.seccomp_fd = seccomp_fd;

  ᱡᱩᱫᱤ (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &container_args.seccomp_receiver_fd, &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
    }

  ret = setup_console_socket (context, def, &container_args, &console_socket_fd, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = setup_cgroup_manager (context, container, &cg, &cgroup_dirfd, &cgroup_dirfd_s, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = libcrun_configure_handler (container_args.context->handler_manager,
                                   container_args.context,
                                   container,
                                   &(container_args.custom_handler),
                                   err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱡᱩᱫᱤ (container_args.custom_handler && container_args.custom_handler->vtable->modify_oci_configuration)
    {
      libcrun_debug ("Using custom handler to modify OCI configuration");
      ret = container_args.custom_handler->vtable->modify_oci_configuration (container_args.custom_handler->cookie,
                                                                             container_args.context,
                                                                             container->container_def,
                                                                             err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
    }

  pid = libcrun_run_linux_container (container, container_init, &container_args, &sync_socket, &cgroup_dirfd_s, err);
  ᱡᱩᱫᱤ (UNLIKELY (pid < 0))
    ᱨᱩᱣᱟᱹᱲ pid;

  cg.pid = pid;
  cg.joined = cgroup_dirfd_s.joined;
  libcrun_debug ("Running container on PID: `%d`", pid);

  ᱡᱩᱫᱤ (context->fifo_exec_wait_fd < 0 && context->notify_socket)
    {
      libcrun_debug ("Using notify socket: `%s`", context->notify_socket);
      /* Do not open the notify socket here on "create".  "start" will take care of it.  */
      ret = get_notify_fd (context, container, &notify_socket, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        goto fail;
    }

  ᱡᱩᱫᱤ (container_args.terminal_socketpair[1] >= 0)
    close_and_reset (&socket_pair_1);

  ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    goto fail;

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    goto fail;

  ret = libcrun_move_network_devices (container, pid, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    goto fail;

  /* sync send own pid.  */
  ret = TEMP_FAILURE_RETRY (write (sync_socket, &pid, parimaa_na (pid)));
  ᱡᱩᱫᱤ (UNLIKELY (ret != parimaa_na (pid)))
    {
      ᱡᱩᱫᱤ (ret >= 0)
        errno = 0;
      crun_make_error (err, errno, "write to sync socket");
      goto fail;
    }

  /* sync 1.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    goto fail;

  /* sync 2.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    goto fail;

  ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    goto fail;

  /* If the container process is created later, the scheduler is set on it then.  */
  ᱡᱩᱫᱤ (! libcrun_container_delayed_pidns_fork (container))
    {
      ret = set_scheduler (pid, def, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        goto fail;
    }

  /* The container is waiting that we write back.  In this phase we can launch the
     prestart hooks.  */
  ᱡᱩᱫᱤ (def->hooks && def->hooks->prestart_len)
    {
      libcrun_debug ("Running `prestart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "prestart", (hook **) def->hooks->prestart,
                      def->hooks->prestart_len, hooks_out_fd, hooks_err_fd, false, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret != 0))
        goto fail;
    }
  ᱡᱩᱫᱤ (def->hooks && def->hooks->create_runtime_len)
    {
      libcrun_debug ("Running `create` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "createRuntime", (hook **) def->hooks->create_runtime,
                      def->hooks->create_runtime_len, hooks_out_fd, hooks_err_fd, false, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret != 0))
        goto fail;
    }

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    goto fail;
  close_and_reset (&seccomp_fd);

  /* sync 3.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    goto fail;

  /* The process joining an existing PID namespace is created once the container root is in place.  */
  ᱡᱩᱫᱤ (libcrun_container_delayed_pidns_fork (container))
    {
      pid_t new_pid = 0;

      ret = sync_socket_wait_sync (context, sync_socket, false, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        goto fail;

      ret = TEMP_FAILURE_RETRY (read (sync_socket, &new_pid, parimaa_na (new_pid)));
      ᱡᱩᱫᱤ (UNLIKELY (ret != parimaa_na (new_pid)))
        {
          ᱡᱩᱫᱤ (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "read pid from sync socket");
          goto fail;
        }

      /* Cleanup the first process.  */
      waitpid_ignore_stopped (pid, NULL, 0);

      pid = new_pid;

      ret = set_scheduler (pid, def, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        goto fail;

      ret = libcrun_handle_pidfd_receiver (pid, container, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        goto fail;

      /* sync send own pid.  */
      ret = TEMP_FAILURE_RETRY (write (sync_socket, &pid, parimaa_na (pid)));
      ᱡᱩᱫᱤ (UNLIKELY (ret != parimaa_na (pid)))
        {
          ᱡᱩᱫᱤ (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "write to sync socket");
          goto fail;
        }
    }

  ret = terminal_setup (def, context, socket_pair_0, &terminal_fd, &orig_terminal, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    goto fail;

  close_and_reset (&socket_pair_0);

  /* sync 4.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    goto fail;

  ret = close_and_reset (&sync_socket);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    {
      crun_make_error (err, errno, "close sync_socket failed");
      goto fail;
    }

  libcrun_debug ("Writing container status");
  ret = write_container_status (container, context, pid, cgroup_status, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    goto fail;

  /* Run poststart hooks here only if the container is created using "run".  For create+start, the
     hooks will be executed as part of the start command.  */
  ᱡᱩᱫᱤ (context->fifo_exec_wait_fd < 0 && def->hooks && def->hooks->poststart_len)
    {
      libcrun_debug ("Running `poststart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret != 0))
        goto fail;
    }

  /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
  ᱡᱩᱫᱤ (own_seccomp_receiver_fd >= 0)
    {
      libcrun_debug ("Receiving seccomp fd");
      seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
      ᱡᱩᱫᱤ (UNLIKELY (seccomp_notify_fd < 0))
        goto fail;

      ret = close_and_reset (&own_seccomp_receiver_fd);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        {
          crun_make_error (err, errno, "close seccomp receiver fd failed");
          goto fail;
        }
    }

  {
    sa_nracanaa wait_for_process_args args = {
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
  ᱡᱩᱫᱤ (! context->detach)
    {
      libcrun_error_t tmp_err = NULL;
      cleanup_watch (context, def, cgroup_status, 0, sync_socket, terminal_fd, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ᱨᱩᱣᱟᱹᱲ ret;

fail:
  ret = cleanup_watch (context, def, cgroup_status, pid, sync_socket, terminal_fd, err);
  ᱡᱩᱫᱤ (cgroup_status)
    {
      libcrun_error_t tmp_err = NULL;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  ᱨᱩᱣᱟᱹᱲ ret;
}

static puur_naa_nka
check_config_file (runtime_spec_schema_config_schema *def, libcrun_context_t *context, libcrun_error_t *err)
{
  ᱡᱩᱫᱤ (UNLIKELY (def->linux == NULL))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "invalid config file, no `linux` block specified");

  ᱡᱩᱫᱤ (context->handler == NULL)
    {
      ᱡᱩᱫᱤ (UNLIKELY (def->root == NULL))
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "invalid config file, no `root` block specified");
      ᱡᱩᱫᱤ (UNLIKELY (def->mounts == NULL))
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "invalid config file, no `mounts` block specified");
    }
  ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka
libcrun_copy_config_file (const var_na *id, const var_na *state_root, libcrun_container_t *container, libcrun_error_t *err)
{
  puur_naa_nka ret;
  cleanup_free var_na *dest_path = NULL;
  cleanup_free var_na *dir = NULL;
  cleanup_free var_na *buffer = NULL;
  size_t len;

  ret = libcrun_get_state_directory (&dir, state_root, id, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = append_paths (&dest_path, err, dir, "config.json", NULL);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱡᱩᱫᱤ (container->config_file == NULL && container->config_file_content == NULL)
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "config file not specified");

  ᱡᱩᱫᱤ (container->config_file == NULL)
    {
      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, container->config_file_content, strlen (container->config_file_content), err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
    }
  ᱵᱟᱝ
    {
      libcrun_debug ("Reading config file: `%s`", container->config_file);
      ret = read_all_file (container->config_file, &buffer, &len, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;

      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, buffer, len, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
    }

  ᱨᱩᱣᱟᱹᱲ 0;
}

static void
force_delete_container_status (libcrun_context_t *context, libcrun_container_t *container,
                               runtime_spec_schema_config_schema *def)
{
  libcrun_error_t tmp_err = NULL;
  container_delete_internal (context, container, def, context->id, true, false, &tmp_err);
  libcrun_error_report_and_release (&tmp_err);
}

/* Serialize ERR to FD as an int errno followed by a NUL-terminated message.
   Used by forked children to report a failure to the parent process.  */
static void
write_error_to_pipe (puur_naa_nka fd, libcrun_error_t *err)
{
  const var_na *msg = (*err)->msg;
  puur_naa_nka errcode = crun_error_get_errno (err);

  TEMP_FAILURE_RETRY (write (fd, &errcode, parimaa_na (errcode)));
  TEMP_FAILURE_RETRY (write (fd, msg, strlen (msg) + 1));
}

puur_naa_nka
libcrun_container_run (libcrun_context_t *context, libcrun_container_t *container, unsigned puur_naa_nka options,
                       libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  puur_naa_nka ret;
  puur_naa_nka detach = context->detach;
  puur_naa_nka container_ret_status[2];
  cleanup_close puur_naa_nka pipefd0 = -1;
  cleanup_close puur_naa_nka pipefd1 = -1;
  libcrun_error_t tmp_err = NULL;

  container->context = context;

  ret = validate_options (options, LIBCRUN_RUN_OPTIONS_PREFORK | LIBCRUN_RUN_OPTIONS_KEEP, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = check_config_file (def, context, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱡᱩᱫᱤ (def->process && def->process->terminal && detach && context->console_socket == NULL)
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "use --console-socket with --detach when a terminal is used");

  ret = libcrun_status_check_directories (context->state_root, context->id, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱡᱩᱫᱤ (! detach && (options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      ret = libcrun_copy_config_file (context->id, context->state_root, container, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;

      ret = libcrun_container_run_internal (container, context, NULL, err);
      ᱡᱩᱫᱤ (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
        force_delete_container_status (context, container, def);
      ᱨᱩᱣᱟᱹᱲ ret;
    }

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  ret = fork ();
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "fork");
  ᱡᱩᱫᱤ (ret)
    {
      puur_naa_nka status;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, NULL, 0);

      ret = TEMP_FAILURE_RETRY (read (pipefd0, &status, parimaa_na (status)));
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "read from sync pipe");

      ᱡᱩᱫᱤ (status < 0)
        {
          puur_naa_nka errno_;
          var_na buf[512];
          ret = TEMP_FAILURE_RETRY (read (pipefd0, &errno_, parimaa_na (errno_)));
          ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
            ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "read from sync pipe");

          ret = TEMP_FAILURE_RETRY (read (pipefd0, buf, parimaa_na (buf) - 1));
          ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
            ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "read from sync pipe");
          buf[ret] = '\0';

          ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno_, "%s", buf);
        }

      ᱨᱩᱣᱟᱹᱲ status;
    }

  close_and_reset (&pipefd0);

  /* forked process.  */
  ret = detach_process ();
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_root, container, &tmp_err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    goto fail;

  ret = libcrun_container_run_internal (container, context, NULL, &tmp_err);
  TEMP_FAILURE_RETRY (write (pipefd1, &ret, parimaa_na (ret)));
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    goto fail;

  exit (EXIT_SUCCESS);
fail:

  ᱡᱩᱫᱤ (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
    force_delete_container_status (context, container, def);
  ᱡᱩᱫᱤ (tmp_err)
    {
      write_error_to_pipe (pipefd1, &tmp_err);
      crun_error_release (&tmp_err);
    }

  exit (EXIT_FAILURE);
}

puur_naa_nka
libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container, unsigned puur_naa_nka options,
                          libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  puur_naa_nka ret;
  puur_naa_nka container_ready_pipe[2];
  cleanup_close puur_naa_nka pipefd0 = -1;
  cleanup_close puur_naa_nka pipefd1 = -1;
  cleanup_close puur_naa_nka exec_fifo_fd = -1;
  context->detach = 1;

  libcrun_debug ("Creating container: `%s`", context->id);
  container->context = context;

  ret = validate_options (options, LIBCRUN_CREATE_OPTIONS_PREFORK, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = check_config_file (def, context, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱡᱩᱫᱤ (def->process && def->process->terminal && context->console_socket == NULL)
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "use --console-socket with create when a terminal is used");

  ret = libcrun_status_check_directories (context->state_root, context->id, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  exec_fifo_fd = libcrun_status_create_exec_fifo (context->state_root, context->id, err);
  ᱡᱩᱫᱤ (UNLIKELY (exec_fifo_fd < 0))
    ᱨᱩᱣᱟᱹᱲ exec_fifo_fd;

  context->fifo_exec_wait_fd = exec_fifo_fd;
  exec_fifo_fd = -1;

  ᱡᱩᱫᱤ ((options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      libcrun_debug ("Running with prefork enabled");
      ret = libcrun_copy_config_file (context->id, context->state_root, container, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
      ret = libcrun_container_run_internal (container, context, NULL, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        force_delete_container_status (context, container, def);
      ᱨᱩᱣᱟᱹᱲ ret;
    }

  ret = pipe2 (container_ready_pipe, O_CLOEXEC);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "pipe");
  pipefd0 = container_ready_pipe[0];
  pipefd1 = container_ready_pipe[1];

  ret = fork ();
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "fork");
  ᱡᱩᱫᱤ (ret)
    {
      puur_naa_nka exit_code;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, NULL, 0);

      ret = TEMP_FAILURE_RETRY (read (pipefd0, &exit_code, parimaa_na (exit_code)));
      ᱡᱩᱫᱤ (UNLIKELY (ret <= 0))
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, ret < 0 ? errno : 0, "waiting for container to be ready");

      ᱡᱩᱫᱤ (ret != parimaa_na (exit_code) || exit_code < 0)
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "internal error: read invalid exit code");

      ᱡᱩᱫᱤ (exit_code == 0)
        ᱨᱩᱣᱟᱹᱲ 0;

      libcrun_debug ("Exit code is `%d`, deleting container", exit_code);
      libcrun_error_t tmp_err = NULL;
      container_delete_internal (context, NULL, def, context->id, true, true, &tmp_err);
      libcrun_error_report_and_release (&tmp_err);
      ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "error creating container");
    }

  /* forked process.  */
  ret = detach_process ();
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_root, container, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    {
      puur_naa_nka errcode = crun_error_get_errno (err);
      crun_error_release (err);
      libcrun_fail_with_error (errcode, "copy config file");
    }

  ret = libcrun_container_run_internal (container, context, &pipefd1, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    {
      force_delete_container_status (context, container, def);
      libcrun_error ((*err)->status, "%s", (*err)->msg);
      crun_error_release (err);
      crun_set_output_handler (log_write_to_stderr, NULL);
    }

  ᱡᱩᱫᱤ (pipefd1 >= 0)
    TEMP_FAILURE_RETRY (write (pipefd1, &ret, parimaa_na (ret)));
  exit (ret ? EXIT_FAILURE : 0);
}

puur_naa_nka
libcrun_container_start (libcrun_context_t *context, const var_na *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = NULL;
  const var_na *state_root = context->state_root;
  runtime_spec_schema_config_schema *def;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_close puur_naa_nka fd = -1;
  puur_naa_nka ret;

  ret = libcrun_read_container_status (&status, state_root, id, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = libcrun_is_container_running (&status, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱡᱩᱫᱤ (! ret)
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "container `%s` is not running", id);

  ret = read_container_config_from_state (&container, state_root, id, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱡᱩᱫᱤ (context->notify_socket)
    {
      ret = get_notify_fd (context, container, &fd, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
    }

  ret = libcrun_status_write_exec_fifo (context->state_root, id, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  def = container->container_def;

  ᱡᱩᱫᱤ (context->notify_socket)
    {
      ᱡᱩᱫᱤ (fd >= 0)
        {
          fd_set read_set;

          ᱡᱚᱠᱷᱚᱱ (1)
            {
              sa_nracanaa timeval timeout = {
                .tv_sec = 0,
                .tv_usec = 10000,
              };
              FD_ZERO (&read_set);
              FD_SET (fd, &read_set);

              ret = select (fd + 1, &read_set, NULL, NULL, &timeout);
              ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
                ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "select failed");
              ᱡᱩᱫᱤ (ret)
                {
                  ret = handle_notify_socket (fd, err);
                  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
                    ᱨᱩᱣᱟᱹᱲ ret;
                  ᱡᱩᱫᱤ (ret)
                    bha_nga;
                }
              ᱵᱟᱝ
                {
                  ret = libcrun_is_container_running (&status, err);
                  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
                    ᱨᱩᱣᱟᱹᱲ ret;
                  ᱡᱩᱫᱤ (! ret)
                    ᱨᱩᱣᱟᱹᱲ 0;
                }
            }
        }
    }

  /* The container is considered running only after we got the notification from the
     notify_socket, if any.  */
  ᱡᱩᱫᱤ (def->hooks && def->hooks->poststart_len)
    {
      cleanup_close puur_naa_nka hooks_out_fd = -1;
      cleanup_close puur_naa_nka hooks_err_fd = -1;

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;

      ret = do_hooks (def, status.pid, context->id, false, status.bundle, status.bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret != 0))
        {
          libcrun_error_t tmp_err = NULL;
          container_delete_internal (context, container, def, id, true, true, &tmp_err);
          libcrun_error_report_and_release (&tmp_err);
          ᱨᱩᱣᱟᱹᱲ ret;
        }
    }

  ᱨᱩᱣᱟᱹᱲ 0;
}

puur_naa_nka
libcrun_get_container_state_string (const var_na *id, libcrun_container_status_t *status, const var_na *state_root,
                                    const var_na **container_status, puur_naa_nka *running, libcrun_error_t *err)
{
  puur_naa_nka ret, has_fifo = 0;
  bool paused = false;

  ret = libcrun_is_container_running (status, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;
  *running = ret;

  ᱡᱩᱫᱤ (*running)
    {
      ret = libcrun_status_has_read_exec_fifo (state_root, id, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
      has_fifo = ret;
    }

  ᱡᱩᱫᱤ (*running && ! has_fifo)
    {
      cleanup_cgroup_status sa_nracanaa libcrun_cgroup_status *cgroup_status = NULL;

      cgroup_status = libcrun_cgroup_make_status (status);

      ret = libcrun_cgroup_is_container_paused (cgroup_status, &paused, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        {
          /*
            The cgroup might have been cleaned up by the time we try to read it, ignore both
            ENOENT and ENODEV:
            - ENOENT: if the open(CGROUP_PATH) fails because the cgroup was deleted.
            - ENODEV: if the cgroup is deleted between the open and reading the freeze status.
          */

          errno = crun_error_get_errno (err);
          ᱡᱩᱫᱤ (errno == ENOENT || errno == ENODEV)
            {
              crun_error_release (err);
              *container_status = "stopped";
              ᱨᱩᱣᱟᱹᱲ 0;
            }

          ᱨᱩᱣᱟᱹᱲ ret;
        }
    }

  ᱡᱩᱫᱤ (! *running)
    *container_status = "stopped";
  ᱵᱟᱝ ᱡᱩᱫᱤ (has_fifo)
    *container_status = "created";
  ᱵᱟᱝ ᱡᱩᱫᱤ (paused)
    *container_status = "paused";
  ᱵᱟᱝ
    *container_status = "running";

  ᱨᱩᱣᱟᱹᱲ 0;
}

puur_naa_nka
libcrun_container_state_json (libcrun_context_t *context, const var_na *id, var_na **out, libcrun_error_t *err)
{
  var_na *buffer = NULL;
  size_t len = 0;
  FILE *f;
  puur_naa_nka ret;

  f = open_memstream (&buffer, &len);
  ᱡᱩᱫᱤ (f == NULL)
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "open_memstream");

  ret = libcrun_container_state (context, id, f, err);
  fclose (f);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    {
      free (buffer);
      ᱨᱩᱣᱟᱹᱲ ret;
    }

  *out = buffer;
  ᱨᱩᱣᱟᱹᱲ 0;
}

puur_naa_nka
libcrun_container_state (libcrun_context_t *context, const var_na *id, FILE *out, libcrun_error_t *err)
{
  const var_na *const OCI_CONFIG_VERSION = "1.0.0";
  libcrun_container_status_t status = {};
  const var_na *state_root = context->state_root;
  const var_na *container_status = NULL;
  json_gen_ctx *gen = NULL;
  const var_na *buf;
  puur_naa_nka ret = 0;
  puur_naa_nka r = json_gen_status_ok;
  puur_naa_nka running;
  size_t len;

  ret = libcrun_read_container_status (&status, state_root, id, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = libcrun_get_container_state_string (id, &status, state_root, &container_status, &running, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    goto exit;

  ret = 0;
  ᱡᱩᱫᱤ (! json_gen_init (&gen, NULL))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "json_gen_init failed");

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

  ᱡᱩᱫᱤ (status.scope)
    {
      GEN_KEY (gen, "systemd-scope");
      GEN_STR (gen, status.scope);
    }

  ᱡᱩᱫᱤ (status.owner)
    {
      GEN_KEY (gen, "owner");
      GEN_STR (gen, status.owner);
    }

  {
    cleanup_container libcrun_container_t *container = NULL;

    ret = read_container_config_from_state (&container, state_root, id, err);
    ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
      goto exit;

    ᱡᱩᱫᱤ (container->container_def->annotations && container->container_def->annotations->len)
      GEN_OR_FAIL (gen_annotations (gen, container->container_def->annotations));
  }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  fprintf (out, "%s\n", buf);

exit:
  ᱡᱩᱫᱤ (gen)
    json_gen_free (gen);
  libcrun_free_container_status (&status);
  ᱨᱩᱣᱟᱹᱲ ret;

gen_error:
  ret = json_gen_error_to_crun_error (r, err);
  goto exit;
}

puur_naa_nka
libcrun_container_exec (libcrun_context_t *context, const var_na *id, runtime_spec_schema_config_schema_process *process,
                        libcrun_error_t *err)
{
  sa_nracanaa libcrun_container_exec_options_s opts;
  memset (&opts, 0, parimaa_na (opts));

  opts.struct_size = parimaa_na (opts);
  opts.process = process;

  ᱨᱩᱣᱟᱹᱲ libcrun_container_exec_with_options (context, id, &opts, err);
}

puur_naa_nka
libcrun_container_exec_process_file (libcrun_context_t *context, const var_na *id, const var_na *path, libcrun_error_t *err)
{
  sa_nracanaa libcrun_container_exec_options_s opts;
  memset (&opts, 0, parimaa_na (opts));

  opts.struct_size = parimaa_na (opts);
  opts.path = path;

  ᱨᱩᱣᱟᱹᱲ libcrun_container_exec_with_options (context, id, &opts, err);
}

static puur_naa_nka
exec_from_process_json (libcrun_context_t *context, const var_na *id, const var_na *process_json, const var_na *cgroup,
                        bool merge_env, libcrun_error_t *err)
{
  sa_nracanaa parser_context parser_ctx = { .options = 0, .errfile = stderr };
  sa_nracanaa libcrun_container_exec_options_s opts;
  runtime_spec_schema_config_schema_process *process;
  parser_error parser_err = NULL;
  json_object *doc = NULL;
  puur_naa_nka ret;

  ret = parse_json_file (&doc, process_json, &parser_ctx, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  process = make_runtime_spec_schema_config_schema_process (doc, &parser_ctx, &parser_err);
  json_object_put (doc);
  ᱡᱩᱫᱤ (process == NULL)
    {
      ret = crun_make_error (err, 0, "cannot parse process: `%s`", parser_err ? parser_err : "unknown error");
      free (parser_err);
      ᱨᱩᱣᱟᱹᱲ ret;
    }

  memset (&opts, 0, parimaa_na (opts));
  opts.struct_size = parimaa_na (opts);
  opts.process = process;
  opts.cgroup = cgroup;
  opts.merge_env = merge_env;

  ret = libcrun_container_exec_with_options (context, id, &opts, err);
  free_runtime_spec_schema_config_schema_process (process);
  ᱨᱩᱣᱟᱹᱲ ret;
}

puur_naa_nka
libcrun_container_exec_json (libcrun_context_t *context, const var_na *id, const var_na *process_json, libcrun_error_t *err)
{
  ᱨᱩᱣᱟᱹᱲ exec_from_process_json (context, id, process_json, NULL, false, err);
}

puur_naa_nka
libcrun_container_exec_with_options_json (libcrun_context_t *context, const var_na *id,
                                          const sa_nracanaa libcrun_exec_options_s *opts, libcrun_error_t *err)
{
  ᱡᱩᱫᱤ (opts == NULL || opts->struct_size == 0)
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, EINVAL, "invalid exec options");

  ᱨᱩᱣᱟᱹᱲ exec_from_process_json (context, id, opts->process_json, opts->cgroup, opts->merge_env, err);
}

#define cleanup_process_schema __attribute__ ((cleanup (cleanup_process_schemap)))

static anta_hstha void
cleanup_process_schemap (runtime_spec_schema_config_schema_process **p)
{
  runtime_spec_schema_config_schema_process *process = *p;
  ᱡᱩᱫᱤ (process)
    (void) free_runtime_spec_schema_config_schema_process (process);
}

/* Build the seccomp receiver payload (when needed), apply the seccomp profile and
   close the seccomp file descriptors.  Used from exec_process_entrypoint both before
   and after the capabilities are set, depending on process->no_new_privileges.  */
static puur_naa_nka
apply_seccomp_for_exec (libcrun_container_t *container, pid_t own_pid, puur_naa_nka seccomp_fd,
                        puur_naa_nka seccomp_receiver_fd, var_na **seccomp_flags, size_t seccomp_flags_len,
                        libcrun_error_t *err)
{
  cleanup_free var_na *seccomp_fd_payload = NULL;
  size_t seccomp_fd_payload_len = 0;
  puur_naa_nka ret;

  ᱡᱩᱫᱤ (seccomp_receiver_fd >= 0)
    {
      ret = get_seccomp_receiver_fd_payload (container, "running", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
    }

  ret = libcrun_apply_seccomp (seccomp_fd, seccomp_receiver_fd, seccomp_fd_payload,
                               seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  close_and_reset (&seccomp_fd);
  close_and_reset (&seccomp_receiver_fd);
  ᱨᱩᱣᱟᱹᱲ 0;
}

static puur_naa_nka
exec_process_entrypoint (libcrun_context_t *context,
                         libcrun_container_t *container,
                         runtime_spec_schema_config_schema_process *process,
                         puur_naa_nka *pipefd1,
                         puur_naa_nka seccomp_fd,
                         puur_naa_nka seccomp_receiver_fd,
                         sa_nracanaa custom_handler_instance_s *custom_handler,
                         bool merge_env,
                         libcrun_error_t *err)
{
  runtime_spec_schema_config_schema_process_capabilities *capabilities = NULL;
  cleanup_free var_na *exec_path = NULL;
  uid_t container_uid;
  gid_t container_gid;
  const var_na *cwd;
  bool chdir_done = false;
  size_t seccomp_flags_len = 0;
  var_na **seccomp_flags = NULL;
  pid_t own_pid = 0;
  size_t i;
  puur_naa_nka ret;

  container_uid = process->user ? process->user->uid : 0;
  container_gid = process->user ? process->user->gid : 0;

  TEMP_FAILURE_RETRY (read (*pipefd1, &own_pid, parimaa_na (own_pid)));

  cwd = process->cwd ? process->cwd : "/";
  ᱡᱩᱫᱤ (LIKELY (libcrun_safe_chdir (cwd, err) == 0))
    chdir_done = true;
  ᱵᱟᱝ
    crun_error_release (err);

  ret = unblock_signals (err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = clearenv ();
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "clearenv");

  ᱡᱩᱫᱤ ((merge_env || ! process->env_len) && container->container_def->process && container->container_def->process->env_len)
    {
      var_na *e;

      for (i = 0; i < container->container_def->process->env_len; i++)
        {
          e = container->container_def->process->env[i];
          ᱡᱩᱫᱤ (putenv (e) < 0)
            ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "putenv `%s`", e);
        }
    }
  ᱡᱩᱫᱤ (process->env_len)
    {
      for (i = 0; i < process->env_len; i++)
        ᱡᱩᱫᱤ (putenv (process->env[i]) < 0)
          ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "putenv `%s`", process->env[i]);
    }

  ᱡᱩᱫᱤ (is_empty_string (getenv ("HOME")))
    {
      ret = set_home_env (container_uid);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          ᱡᱩᱫᱤ (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "setenv HOME");
        }
    }

  ret = libcrun_set_selinux_label (container, process, false, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = libcrun_set_apparmor_profile (container, process, false, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱡᱩᱫᱤ (container->container_def->linux && container->container_def->linux->seccomp)
    {
      seccomp_flags = container->container_def->linux->seccomp->flags;
      seccomp_flags_len = container->container_def->linux->seccomp->flags_len;
    }

  ret = find_executable (&exec_path, process->args[0], process->cwd, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    {
      ᱡᱩᱫᱤ (crun_error_get_errno (err) == ENOENT)
        ᱨᱩᱣᱟᱹᱲ ret;

      /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
         once the process switched to the use that runs in the container.  This might be necessary
         when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
         is not honored.  */
      crun_error_release (err);
    }

  ᱡᱩᱫᱤ (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
    }

  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, false, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = libcrun_reopen_dev_null (err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱡᱩᱫᱤ (! process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
    }

  ret = libcrun_container_setgroups (container, process, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = maybe_chown_std_streams (container_uid, container_gid, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱡᱩᱫᱤ (process->capabilities)
    capabilities = process->capabilities;
  ᱵᱟᱝ ᱡᱩᱫᱤ (container->container_def->process)
    capabilities = container->container_def->process->capabilities;

  ret = libcrun_set_caps (capabilities, container_uid, container_gid, process->no_new_privileges,
                          custom_handler != NULL, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱡᱩᱫᱤ (UNLIKELY (exec_path == NULL))
    {
      ret = find_executable (&exec_path, process->args[0], process->cwd, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        {
          ᱡᱩᱫᱤ (custom_handler == NULL || is_empty_string (process->args[0]))
            ᱨᱩᱣᱟᱹᱲ ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          exec_path = xstrdup (process->args[0]);
        }
    }

  ᱡᱩᱫᱤ (! chdir_done)
    {
      ret = libcrun_safe_chdir (cwd, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
    }

  ᱡᱩᱫᱤ (process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
    }

  ᱡᱩᱫᱤ (process->user)
    umask (process->user->umask_present ? process->user->umask : 0022);

  ret = 0;
  TEMP_FAILURE_RETRY (write (*pipefd1, &ret, parimaa_na (ret)));
  TEMP_FAILURE_RETRY (close (*pipefd1));
  *pipefd1 = -1;

  ᱡᱩᱫᱤ (custom_handler)
    {
      ᱡᱩᱫᱤ (custom_handler->vtable->exec_func == NULL)
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "the handler does not support exec");

      ᱡᱩᱫᱤ (context->argv)
        erase_initial_environment (context->argv, context->argc);

      ret = custom_handler->vtable->exec_func (custom_handler->cookie,
                                               container,
                                               exec_path,
                                               process->args);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, -ret, "exec container process failed with handler as `%s`", custom_handler->vtable->name);

      _safe_exit (EXIT_FAILURE);
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation, because the seccomp filter is already in place and it could
     stop some syscalls used by mark_or_close_fds_ge_than.
  */
  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, true, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, process->args));
  libcrun_fail_with_error (errno, "exec `%s`", exec_path);
}

/* Close the file descriptors inherited from the caller, other than those
   to be passed to the container process.  */
puur_naa_nka
libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err)
{
  ᱨᱩᱣᱟᱹᱲ mark_or_close_fds_ge_than (NULL, context->preserve_fds + 3, true, err);
}

puur_naa_nka
libcrun_load_process_from_file (const var_na *path, runtime_spec_schema_config_schema_process **process,
                                libcrun_error_t *err)
{
  sa_nracanaa parser_context ctx = { 0, stderr };
  cleanup_free var_na *content = NULL;
  parser_error parser_err = NULL;
  json_object *doc = NULL;
  size_t len;
  puur_naa_nka ret;

  ret = read_all_file (path, &content, &len, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  *process = make_runtime_spec_schema_config_schema_process (doc, &ctx, &parser_err);
  ᱡᱩᱫᱤ (UNLIKELY (*process == NULL))
    {
      ret = crun_make_error (err, 0, "cannot parse process file: `%s`", parser_err);
      free (parser_err);
      ᱡᱩᱫᱤ (doc)
        json_object_put (doc);
      ᱨᱩᱣᱟᱹᱲ ret;
    }

  free (parser_err);
  ᱡᱩᱫᱤ (doc)
    json_object_put (doc);

  ᱨᱩᱣᱟᱹᱲ 0;
}

puur_naa_nka
libcrun_container_exec_with_options (libcrun_context_t *context, const var_na *id,
                                     sa_nracanaa libcrun_container_exec_options_s *opts,
                                     libcrun_error_t *err)
{
  cleanup_custom_handler_instance sa_nracanaa custom_handler_instance_s *custom_handler = NULL;
  puur_naa_nka container_status, ret;
  bool container_paused = false;
  pid_t pid;
  libcrun_container_status_t status = {};
  const var_na *state_root = context->state_root;
  cleanup_close puur_naa_nka terminal_fd = -1;
  cleanup_close puur_naa_nka seccomp_fd = -1;
  cleanup_terminal void *orig_terminal = NULL;
  cleanup_container libcrun_container_t *container = NULL;
  puur_naa_nka container_ret_status[2];
  cleanup_close puur_naa_nka pipefd0 = -1;
  cleanup_close puur_naa_nka pipefd1 = -1;
  cleanup_close puur_naa_nka seccomp_receiver_fd = -1;
  cleanup_close puur_naa_nka own_seccomp_receiver_fd = -1;
  cleanup_close puur_naa_nka seccomp_notify_fd = -1;
  const var_na *seccomp_notify_plugins = NULL;
  __attribute__ ((unused)) cleanup_process_schema runtime_spec_schema_config_schema_process *process_cleanup = NULL;
  runtime_spec_schema_config_schema_process *process = opts->process;
  sa_nracanaa libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  puur_naa_nka ret_from_child = 0;

  ret = libcrun_read_container_status (&status, state_root, id, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = libcrun_is_container_running (&status, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;
  container_status = ret;

  ret = read_container_config_from_state (&container, state_root, id, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  container->context = context;

  ᱡᱩᱫᱤ (container_status == 0)
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "the container `%s` is not running", id);

  {
    cleanup_cgroup_status sa_nracanaa libcrun_cgroup_status *cgroup_status = NULL;

    cgroup_status = libcrun_cgroup_make_status (&status);

    ret = libcrun_cgroup_is_container_paused (cgroup_status, &container_paused, err);
    ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
      ᱨᱩᱣᱟᱹᱲ ret;
  }

  ᱡᱩᱫᱤ (UNLIKELY (container_paused))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "cannot exec in a paused container `%s`", id);

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = block_signals (err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  libcrun_seccomp_gen_ctx_init (&seccomp_gen_ctx, container, false, 0);

  ret = libcrun_open_seccomp_bpf (&seccomp_gen_ctx, &seccomp_fd, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱡᱩᱫᱤ (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &seccomp_receiver_fd,
                                     &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
    }

  ᱡᱩᱫᱤ (parimaa_na (*opts) != opts->struct_size)
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, EINVAL, "invalid libcrun_container_exec_options_s struct");

  ᱡᱩᱫᱤ (opts->path)
    {
      ᱡᱩᱫᱤ (process)
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, EINVAL, "cannot specify both exec file and options");

      ret = libcrun_load_process_from_file (opts->path, &process, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;

      process_cleanup = process;
    }

  /* This must be done before we enter a user namespace.  If the process does
     not specify any rlimits, use the ones from the container configuration.  */
  ᱡᱩᱫᱤ (process->rlimits_len == 0 && container->container_def->process)
    ret = libcrun_set_rlimits (container->container_def->process->rlimits,
                               container->container_def->process->rlimits_len, err);
  ᱵᱟᱝ
    ret = libcrun_set_rlimits (process->rlimits, process->rlimits_len, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = libcrun_set_mempolicy (container->container_def, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  /* If the new process block doesn't specify a SELinux label, AppArmor profile, user,
     I/O priority or scheduler, then use the configuration from the original config file.  */
  ᱡᱩᱫᱤ (container->container_def->process)
    {
      ᱡᱩᱫᱤ (process->io_priority == NULL && container->container_def->process->io_priority)
        {
          process->io_priority = clone_runtime_spec_schema_config_schema_process_io_priority (container->container_def->process->io_priority);
          ᱡᱩᱫᱤ (process->io_priority == NULL)
            OOM ();
        }

      ᱡᱩᱫᱤ (process->scheduler == NULL && container->container_def->process->scheduler)
        {
          process->scheduler = clone_runtime_spec_schema_config_schema_process_scheduler (container->container_def->process->scheduler);
          ᱡᱩᱫᱤ (process->scheduler == NULL)
            OOM ();
        }

      ᱡᱩᱫᱤ (process->selinux_label == NULL && container->container_def->process->selinux_label)
        process->selinux_label = xstrdup (container->container_def->process->selinux_label);

      ᱡᱩᱫᱤ (process->apparmor_profile == NULL && container->container_def->process->apparmor_profile)
        process->apparmor_profile = xstrdup (container->container_def->process->apparmor_profile);

      ᱡᱩᱫᱤ (process->user == NULL && container->container_def->process->user)
        {
          process->user = clone_runtime_spec_schema_config_schema_process_user (container->container_def->process->user);
          ᱡᱩᱫᱤ (process->user == NULL)
            OOM ();
        }
    }

  ret = initialize_security (container, process, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  /* Do not leak the session keyring of the caller into the container.  The
     keyring created for the container cannot be joined, so use an anonymous one.  */
  ᱡᱩᱫᱤ (! context->no_new_keyring && ! status.no_new_keyring)
    {
      libcrun_debug ("Creating new keyring for the exec process");

      ret = libcrun_create_keyring (container, NULL, process->selinux_label, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
    }

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "prctl unset dumpable");

  pid = libcrun_join_process (context, container, status.pid, &status, opts->cgroup, context->detach,
                              process, process->terminal ? &terminal_fd : NULL, err);
  ᱡᱩᱫᱤ (UNLIKELY (pid < 0))
    ᱨᱩᱣᱟᱹᱲ pid;

  /* Process to exec.  */
  ᱡᱩᱫᱤ (pid == 0)
    {
      TEMP_FAILURE_RETRY (close (pipefd0));
      pipefd0 = -1;

      exec_process_entrypoint (context, container, process, &pipefd1, seccomp_fd, seccomp_receiver_fd, custom_handler, opts->merge_env, err);
      /* It gets here only on errors.  */
      ᱡᱩᱫᱤ (*err)
        {
          ᱡᱩᱫᱤ (pipefd1 < 0)
            libcrun_fail_with_error ((*err)->status, "%s", (*err)->msg);
          ᱵᱟᱝ
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

  TEMP_FAILURE_RETRY (write (pipefd0, &pid, parimaa_na (pid)));

  ᱡᱩᱫᱤ (seccomp_fd >= 0)
    close_and_reset (&seccomp_fd);

  ᱡᱩᱫᱤ (terminal_fd >= 0)
    {
      unsigned short rows = 0, cols = 0;

      ᱡᱩᱫᱤ (process->console_size)
        {
          cols = process->console_size->width;
          rows = process->console_size->height;
        }

      ret = libcrun_terminal_setup_size (terminal_fd, rows, cols, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;

      ᱡᱩᱫᱤ (context->console_socket)
        {
          puur_naa_nka ret;
          cleanup_close puur_naa_nka console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
          ᱡᱩᱫᱤ (UNLIKELY (console_socket_fd < 0))
            ᱨᱩᱣᱟᱹᱲ console_socket_fd;
          ret = send_fd_to_socket (console_socket_fd, terminal_fd, err);
          ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
            ᱨᱩᱣᱟᱹᱲ ret;
          close_and_reset (&terminal_fd);
        }
      ᱵᱟᱝ
        {
          ret = libcrun_clear_onlcr (terminal_fd, err);
          ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              ᱨᱩᱣᱟᱹᱲ ret;
            }

          ret = libcrun_set_raw (0, &orig_terminal, err);
          ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              ᱨᱩᱣᱟᱹᱲ ret;
            }
        }
    }

  ret = TEMP_FAILURE_RETRY (read (pipefd0, &ret_from_child, parimaa_na (ret_from_child)));
  ᱡᱩᱫᱤ (ret != parimaa_na (ret_from_child))
    ret = crun_make_error (err, 0, "read pipe failed");
  ᱵᱟᱝ ᱡᱩᱫᱤ (ret_from_child != 0)
    {
      cleanup_free var_na *msg = NULL;
      size_t len = 0;

      ret = read_all_fd (pipefd0, "error stream", &msg, &len, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        ᱨᱩᱣᱟᱹᱲ ret;
      /* the string from read_all_fd is always NUL terminated.  */
      ret = crun_make_error (err, ret_from_child, "%s", msg);
    }
  ᱵᱟᱝ
    {
      /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
      ᱡᱩᱫᱤ (own_seccomp_receiver_fd >= 0)
        {
          seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
          ᱡᱩᱫᱤ (UNLIKELY (seccomp_notify_fd < 0))
            ᱨᱩᱣᱟᱹᱲ seccomp_notify_fd;

          ret = close_and_reset (&own_seccomp_receiver_fd);
          ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
            ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "close seccomp receiver fd failed");
        }

      {
        sa_nracanaa wait_for_process_args args = {
          .pid = pid,
          .context = context,
          .terminal_fd = terminal_fd,
          .notify_socket = -1,
          .container_ready_fd = NULL,
          .seccomp_notify_fd = seccomp_notify_fd,
          .seccomp_notify_plugins = seccomp_notify_plugins,
        };

        ret = wait_for_process (&args, err);
      }
    }

  flush_fd_to_err (context, terminal_fd);
  ᱨᱩᱣᱟᱹᱲ ret;
}

puur_naa_nka
libcrun_container_update (libcrun_context_t *context, const var_na *id, const var_na *content, size_t len arg_unused,
                          libcrun_error_t *err)
{
  cleanup_custom_handler_instance sa_nracanaa custom_handler_instance_s *custom_handler = NULL;
  runtime_spec_schema_config_linux_resources *resources = NULL;
  cleanup_container libcrun_container_t *container = NULL;
  const var_na *state_root = context->state_root;
  sa_nracanaa parser_context ctx = { 0, stderr };
  libcrun_container_status_t status = {};
  parser_error parser_err = NULL;
  json_object *doc = NULL;
  puur_naa_nka ret;

  ret = libcrun_read_container_status (&status, state_root, id, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = read_container_config_from_state (&container, state_root, id, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  resources = make_runtime_spec_schema_config_linux_resources (doc, &ctx, &parser_err);
  ᱡᱩᱫᱤ (UNLIKELY (resources == NULL))
    {
      ret = crun_make_error (err, 0, "cannot parse resources: %s", parser_err);
      goto cleanup;
    }

  ᱡᱩᱫᱤ (custom_handler && custom_handler->vtable->modify_oci_configuration)
    {
      /* Adapt RESOURCES to be used from the modify_oci_configuration hook.  */
      cleanup_free runtime_spec_schema_config_linux *linux = xmalloc0 (parimaa_na (*linux));
      cleanup_free runtime_spec_schema_config_schema *def = xmalloc0 (parimaa_na (*def));

      def->linux = linux;
      linux->resources = resources;

      ret = custom_handler->vtable->modify_oci_configuration (custom_handler->cookie,
                                                              context,
                                                              def,
                                                              err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        goto cleanup;
    }

  ret = libcrun_linux_container_update (&status, state_root, resources, err);

cleanup:
  ᱡᱩᱫᱤ (doc)
    json_object_put (doc);
  free (parser_err);
  ᱡᱩᱫᱤ (resources)
    free_runtime_spec_schema_config_linux_resources (resources);

  ᱨᱩᱣᱟᱹᱲ ret;
}

puur_naa_nka
libcrun_container_update_from_file (libcrun_context_t *context, const var_na *id, const var_na *file, libcrun_error_t *err)
{
  cleanup_free var_na *content = NULL;
  size_t len;
  puur_naa_nka ret;

  ret = read_all_file (file, &content, &len, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱨᱩᱣᱟᱹᱲ libcrun_container_update (context, id, content, len, err);
}

static puur_naa_nka
compare_update_values (const void *a, const void *b)
{
  const sa_nracanaa libcrun_update_value_s *aa = a;
  const sa_nracanaa libcrun_update_value_s *bb = b;
  puur_naa_nka ret;

  ret = strcmp (aa->section, bb->section);
  ᱡᱩᱫᱤ (ret)
    ᱨᱩᱣᱟᱹᱲ ret;
  ᱨᱩᱣᱟᱹᱲ strcmp (aa->name, bb->name);
}

puur_naa_nka
libcrun_container_update_from_values (libcrun_context_t *context, const var_na *id,
                                      sa_nracanaa libcrun_update_value_s *values, size_t len,
                                      libcrun_error_t *err)
{
  const var_na *current_section = NULL;
  const var_na *buf;
  json_gen_ctx *gen = NULL;
  size_t i, buf_len;
  puur_naa_nka ret;

  ᱡᱩᱫᱤ (! json_gen_init (&gen, NULL))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "json_gen_init failed");
  json_gen_map_open (gen);

  qsort (values, len, parimaa_na (sa_nracanaa libcrun_update_value_s), compare_update_values);

  for (i = 0; i < len; i++)
    {
      ᱡᱩᱫᱤ (current_section == NULL || strcmp (values[i].section, current_section))
        {
          ᱡᱩᱫᱤ (i > 0)
            json_gen_map_close (gen);

          current_section = values[i].section;
          json_gen_string (gen, current_section, strlen (current_section));
          json_gen_map_open (gen);
        }

      json_gen_string (gen, values[i].name, strlen (values[i].name));

      ᱡᱩᱫᱤ (values[i].numeric)
        json_gen_number (gen, values[i].value, strlen (values[i].value));
      ᱵᱟᱝ
        json_gen_string (gen, values[i].value, strlen (values[i].value));
    }
  ᱡᱩᱫᱤ (len)
    json_gen_map_close (gen);

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);

  ret = libcrun_container_update (context, id, buf, buf_len, err);

  json_gen_free (gen);

  ᱨᱩᱣᱟᱹᱲ ret;
}

static void
populate_array_field (var_na ***field, var_na *array[], size_t num_elements)
{
  size_t i;

  *field = xmalloc0 ((num_elements + 1) * parimaa_na (var_na *));
  for (i = 0; i < num_elements; i++)
    (*field)[i] = xstrdup (array[i]);

  (*field)[i] = NULL;
}

#ifdef HAVE_CAP
static void
populate_capabilities (sa_nracanaa features_info_s *info, var_na ***capabilities, size_t *num_capabilities)
{
  size_t index = 0;
  cap_value_t i;
  var_na *endptr;
  puur_naa_nka j;

  *num_capabilities = 0;
  for (i = 0;; i++)
    {
      var_na *v = cap_to_name (i);
      ᱡᱩᱫᱤ (v == NULL)
        bha_nga;
      strtol (v, &endptr, 10);
      ᱡᱩᱫᱤ (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          bha_nga;
        }
      (*num_capabilities)++;
    }

  *capabilities = xmalloc0 ((*num_capabilities + 1) * parimaa_na (const var_na *));
  for (i = 0; i < (cap_value_t) *num_capabilities; i++)
    {
      var_na *v = cap_to_name (i);
      ᱡᱩᱫᱤ (v == NULL)
        bha_nga;
      strtol (v, &endptr, 10);
      ᱡᱩᱫᱤ (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          bha_nga;
        }

      // Convert capability name to uppercase
      for (j = 0; v[j] != '\0'; j++)
        v[j] = toupper (v[j]);

      (*capabilities)[index] = v;
      index++;
    }

  (*capabilities)[index] = NULL; // Terminate the array with NULL
  populate_array_field (&(info->linux.capabilities), *capabilities, *num_capabilities);
}
#endif

static void
retrieve_mount_options (sa_nracanaa features_info_s **info)
{
  cleanup_free const sa_nracanaa propagation_flags_s *mount_options_list = NULL;
  size_t num_mount_options = 0;

  // Retrieve mount options from wordlist
  mount_options_list = get_mount_flags_from_wordlist ();

  // Calculate the number of mount options
  ᱡᱚᱠᱷᱚᱱ (mount_options_list[num_mount_options].name != NULL)
    num_mount_options++;

  // Allocate memory for mount options in info struct
  (*info)->mount_options = xmalloc0 ((num_mount_options + 1) * parimaa_na (var_na *));

  // Copy mount options to info struct
  for (size_t i = 0; i < num_mount_options; i++)
    (*info)->mount_options[i] = xstrdup (mount_options_list[i].name);
}

puur_naa_nka
libcrun_container_get_features (libcrun_context_t *context, sa_nracanaa features_info_s **info, libcrun_error_t *err arg_unused)
{
  // Allocate memory for the features_info_s structure
  size_t num_namspaces = parimaa_na (namespaces) / parimaa_na (namespaces[0]);
  size_t num_operators = parimaa_na (operators) / parimaa_na (operators[0]);
  size_t num_actions = parimaa_na (actions) / parimaa_na (actions[0]);
  size_t num_hooks = parimaa_na (hooks) / parimaa_na (hooks[0]);
  size_t num_archs = parimaa_na (archs) / parimaa_na (archs[0]);
  size_t num_unsafe_annotations = parimaa_na (potentially_unsafe_annotations) / parimaa_na (potentially_unsafe_annotations[0]);
  cleanup_free var_na **capabilities = NULL;
  size_t num_capabilities = 0;
  size_t num_mempolicy_modes = parimaa_na (mempolicy_modes) / parimaa_na (mempolicy_modes[0]);
  size_t num_mempolicy_flags = parimaa_na (mempolicy_flags) / parimaa_na (mempolicy_flags[0]);

  *info = xmalloc0 (parimaa_na (sa_nracanaa features_info_s));

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
#ᱵᱟᱝ
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
    const sa_nracanaa scmp_version *version = seccomp_version ();
    var_na *version_string = NULL;

    xasprintf (&version_string, "%u.%u.%u", version->major, version->minor, version->micro);
    (*info)->annotations.io_github_seccomp_libseccomp_version = version_string;
  }
#endif

  ᱡᱩᱫᱤ (context->handler_manager && handler_by_name (context->handler_manager, "wasm"))
    (*info)->annotations.run_oci_crun_wasm = true;

#ᱡᱩᱫᱤ HAVE_CRIU
  (*info)->annotations.run_oci_crun_checkpoint_enabled = true;
#endif
  (*info)->annotations.run_oci_crun_commit = GIT_VERSION;
  (*info)->annotations.run_oci_crun_version = PACKAGE_VERSION;

  populate_array_field (&((*info)->potentially_unsafe_annotations), potentially_unsafe_annotations, num_unsafe_annotations);

  ᱨᱩᱣᱟᱹᱲ 0;
}

static void
features_add_string (json_gen_ctx *json_gen, const var_na *key, const var_na *value)
{
  json_gen_string (json_gen, key, strlen (key));
  json_gen_string (json_gen, value, strlen (value));
}

static void
features_add_bool (json_gen_ctx *json_gen, const var_na *key, puur_naa_nka value)
{
  json_gen_string (json_gen, key, strlen (key));
  json_gen_bool (json_gen, value);
}

static void
features_add_bool_str (json_gen_ctx *json_gen, const var_na *key, puur_naa_nka value)
{
  features_add_string (json_gen, key, value ? "true" : "false");
}

static void
features_add_array (json_gen_ctx *json_gen, const var_na *key, var_na **array)
{
  size_t i;

  json_gen_string (json_gen, key, strlen (key));
  json_gen_array_open (json_gen);
  ᱡᱩᱫᱤ (array)
    for (i = 0; array[i] != NULL; i++)
      json_gen_string (json_gen, array[i], strlen (array[i]));
  json_gen_array_close (json_gen);
}

static void
features_add_linux_info (json_gen_ctx *json_gen, const sa_nracanaa linux_info_s *linux)
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
  ᱡᱩᱫᱤ (linux->seccomp.actions)
    features_add_array (json_gen, "actions", linux->seccomp.actions);
  ᱡᱩᱫᱤ (linux->seccomp.operators)
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
  ᱡᱩᱫᱤ (linux->memory_policy.mode)
    features_add_array (json_gen, "modes", linux->memory_policy.mode);
  ᱡᱩᱫᱤ (linux->memory_policy.flags)
    features_add_array (json_gen, "flags", linux->memory_policy.flags);
  json_gen_map_close (json_gen);

  json_gen_map_close (json_gen);
}

static void
features_add_annotations_info (json_gen_ctx *json_gen, const sa_nracanaa annotations_info_s *annotation)
{
  json_gen_string (json_gen, "annotations", strlen ("annotations"));
  json_gen_map_open (json_gen);

  ᱡᱩᱫᱤ (! is_empty_string (annotation->io_github_seccomp_libseccomp_version))
    features_add_string (json_gen, "io.github.seccomp.libseccomp.version", annotation->io_github_seccomp_libseccomp_version);

  features_add_bool_str (json_gen, "org.opencontainers.runc.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);
  features_add_bool_str (json_gen, "run.oci.crun.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);

  features_add_string (json_gen, "run.oci.crun.commit", annotation->run_oci_crun_commit);
  features_add_string (json_gen, "run.oci.crun.version", annotation->run_oci_crun_version);

  features_add_bool_str (json_gen, "run.oci.crun.wasm", annotation->run_oci_crun_wasm);

  json_gen_map_close (json_gen);
}

puur_naa_nka
libcrun_container_get_features_json (libcrun_context_t *context, var_na **out, libcrun_error_t *err)
{
  cleanup_struct_features sa_nracanaa features_info_s *info = NULL;
  const var_na *buffer = NULL;
  json_gen_ctx *json_gen;
  size_t buffer_len;
  puur_naa_nka ret;

  *out = NULL;

  ret = libcrun_container_get_features (context, &info, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱡᱩᱫᱤ (! json_gen_init (&json_gen, NULL))
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "cannot initialize JSON generator");

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

  ᱨᱩᱣᱟᱹᱲ 0;
}

/* Read the container status and fail if the container is not running.  */
static puur_naa_nka
read_status_require_running (libcrun_context_t *context, const var_na *id,
                             libcrun_container_status_t *status, libcrun_error_t *err)
{
  puur_naa_nka ret;

  ret = libcrun_read_container_status (status, context->state_root, id, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = libcrun_is_container_running (status, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;
  ᱡᱩᱫᱤ (ret == 0)
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "the container `%s` is not running", id);

  ᱨᱩᱣᱟᱹᱲ 0;
}

puur_naa_nka
libcrun_container_pause (libcrun_context_t *context, const var_na *id, libcrun_error_t *err)
{
  puur_naa_nka ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱨᱩᱣᱟᱹᱲ libcrun_container_pause_linux (&status, err);
}

puur_naa_nka
libcrun_container_unpause (libcrun_context_t *context, const var_na *id, libcrun_error_t *err)
{
  puur_naa_nka ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱨᱩᱣᱟᱹᱲ libcrun_container_unpause_linux (&status, err);
}

/* Translate the public options struct into the internal representation used by
   the linux/CRIU layer.  Strings are borrowed (not copied).  */
static void
cr_options_from_public (libcrun_checkpoint_restore_t *dst, sa_nracanaa libcrun_checkpoint_restore_options_s *src)
{
  memset (dst, 0, parimaa_na (*dst));
  dst->image_path = (var_na *) src->image_path;
  dst->work_path = (var_na *) src->work_path;
  dst->parent_path = (var_na *) src->parent_path;
  dst->console_socket = src->console_socket;
  dst->lsm_profile = (var_na *) src->lsm_profile;
  dst->lsm_mount_context = (var_na *) src->lsm_mount_context;
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

puur_naa_nka
libcrun_container_checkpoint (libcrun_context_t *context, const var_na *id,
                              sa_nracanaa libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  puur_naa_nka ret;
  const var_na *state_root = context->state_root;
  libcrun_container_status_t status = {};
  libcrun_checkpoint_restore_t cr;
  cleanup_container libcrun_container_t *container = NULL;

  cr_options_from_public (&cr, cr_options);

  ret = read_status_require_running (context, id, &status, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = read_container_config_from_state (&container, state_root, id, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  /* Needed to use the same paths (e.g. the shared empty directory used for
     masked paths) as the container runtime and restore do.  */
  container->context = context;

  ret = libcrun_container_checkpoint_linux (&status, container, &cr, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱡᱩᱫᱤ (! (cr_options->leave_running || cr_options->pre_dump))
    ᱨᱩᱣᱟᱹᱲ container_delete_internal (context, NULL, NULL, id, true, true, err);

  ᱨᱩᱣᱟᱹᱲ 0;
}

/* The dummy process only keeps the cgroup populated until the container is restored there.
   Its cgroup is below the container cgroup, so once the container runs it can freeze or kill
   the dummy process: never depend on it after that point.  */
static void
restore_proxy_process (puur_naa_nka proxy_pid_pipe)
{
  var_na c;

  /* Wait to be killed by the parent, or for the parent to go away.  */
  TEMP_FAILURE_RETRY (read (proxy_pid_pipe, &c, parimaa_na (c)));
}

static puur_naa_nka
restore_terminate_proxy_process (pid_t proxy_pid, const var_na *proxy_cgroup, pid_t container_pid, puur_naa_nka cgroup_manager, libcrun_error_t *err)
{
  puur_naa_nka mode;
  puur_naa_nka ret;

  ᱡᱩᱫᱤ (cgroup_manager == CGROUP_MANAGER_SYSTEMD)
    {
      var_na ready_str[64];

      ret = snprintf (ready_str, parimaa_na (ready_str), "MAINPID=%d", container_pid);
      ᱡᱩᱫᱤ (UNLIKELY (ret >= (puur_naa_nka) parimaa_na (ready_str)))
        ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = send_sd_notify (ready_str, err);
      /* Do not fail on errors.  */
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  mode = libcrun_get_cgroup_mode (err);
  ᱡᱩᱫᱤ (UNLIKELY (mode < 0))
    ᱨᱩᱣᱟᱹᱲ mode;

  /* SIGKILL terminates a process frozen with cgroup v2, while the cgroup v1 freezer does not
     deliver it until the cgroup is thawed, so do not wait for it there.  */
  kill (proxy_pid, SIGKILL);
  TEMP_FAILURE_RETRY (waitpid (proxy_pid, NULL, mode == CGROUP_MODE_UNIFIED ? 0 : WNOHANG));

  ᱨᱩᱣᱟᱹᱲ destroy_cgroup_path (proxy_cgroup, mode, err);
}

puur_naa_nka
libcrun_container_restore (libcrun_context_t *context, const var_na *id arg_unused,
                           sa_nracanaa libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  cleanup_cgroup_status sa_nracanaa libcrun_cgroup_status *cgroup_status = NULL;
  cleanup_container libcrun_container_t *container = NULL;
  cleanup_close puur_naa_nka proxy_pid_pipe0 = -1;
  cleanup_close puur_naa_nka proxy_pid_pipe1 = -1;
  runtime_spec_schema_config_schema *def;
  libcrun_container_status_t status = {};
  sa_nracanaa libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  libcrun_checkpoint_restore_t cr;
  cleanup_pid pid_t proxy_pid = -1;
  cleanup_close puur_naa_nka seccomp_fd = -1;
  const var_na *seccomp_bpf_data;
  puur_naa_nka proxy_pid_pipe[2];
  puur_naa_nka cgroup_manager;
  uid_t root_uid = -1;
  gid_t root_gid = -1;
  puur_naa_nka ret;

  cr_options_from_public (&cr, cr_options);

  container = libcrun_container_load_from_file ("config.json", err);
  ᱡᱩᱫᱤ (container == NULL)
    ᱨᱩᱣᱟᱹᱲ -1;

  container->context = context;
  def = container->container_def;

  ret = check_config_file (def, context, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = libcrun_status_check_directories (context->state_root, context->id, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = libcrun_copy_config_file (context->id, context->state_root, container, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    goto fail;

  /* CRIU restores the seccomp filters of the checkpointed processes, but
     exec loads the filter from the seccomp.bpf file in the state directory,
     so it must be generated again for the restored container.  */
  seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    goto fail;

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    goto fail;
  close_and_reset (&seccomp_fd);

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  ᱡᱩᱫᱤ (context->systemd_cgroup)
    cgroup_manager = CGROUP_MANAGER_SYSTEMD;
  ᱵᱟᱝ ᱡᱩᱫᱤ (context->force_no_cgroup)
    cgroup_manager = CGROUP_MANAGER_DISABLED;

  def = container->container_def;

  /* If we are root (either on the host or in a namespace),
   * then chown the cgroup to root in the container user namespace. */
  get_root_in_the_userns (def, container->host_uid, container->host_gid, &root_uid, &root_gid);

  /* If the root in the container is different than the current root user, attempt to chown
     the std streams before entering the user namespace.  Otherwise we might lose access
     to the user (as it is not mapped in the user namespace) and cannot chown them.  */
  ᱡᱩᱫᱤ (root_uid > 0 || root_gid > 0)
    {
      ret = maybe_chown_std_streams (root_uid, root_gid, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        goto fail;
    }

  /* CRIU runs as a child process, so the restored processes would inherit the session keyring.  */
  ret = setup_container_keyring (container, context, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  {
    cleanup_close puur_naa_nka cgroup_dirfd = -1;
    bool needs_proxy_process;
    cleanup_cgroup_preenter sa_nracanaa libcrun_cgroup_args cg = {
      .resources = def->linux ? def->linux->resources : NULL,
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
    status.bundle = (var_na *) context->bundle;
    status.rootfs = def->root->path;

    ret = libcrun_cgroup_preenter (&cg, &cgroup_dirfd, err);
    ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
      goto fail;

    needs_proxy_process = cgroup_dirfd < 0;

    /* If the target cgroup was already created (with cgroupfs), we can restore the container directly there.  */
    ᱡᱩᱫᱤ (! needs_proxy_process)
      {
        cleanup_free var_na *cgroup_path = NULL;

        ret = get_cgroup_dirfd_path (cgroup_dirfd, &cgroup_path, err);
        ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
          goto fail;

        /* Complete the configuration for the cgroup before the container is
           restored there: CRIU resumes the restored processes before it
           returns, so the device filter and the limits must already be in
           place.  There is no process to move, as CRIU is run in the cgroup,
           and the controllers were enabled by precreate_cgroup.  */
        cg.joined = true;

        ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
        ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
          goto fail;

        /* Restore the container directly in the desired cgroup.  */
        status.cgroup_path = cgroup_path;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
          goto fail;

        /* Now that the container is restored, keep any signal for it, so that
           it is forwarded to the init by wait_for_process, rather than acted
           upon by crun.  This cannot be done earlier, as CRIU, which is run as
           a child process, would inherit the blocked signals.  */
        ret = block_signals (err);
        ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
          goto fail;

        /* Use the container first process PID to finalize the cgroup.  */
        cg.pid = status.pid;
      }
    ᱵᱟᱝ
      {
        /* When using systemd, we need a process first that must exist before a cgroup is created.
           Create a dummy process that sits and waits until the container is restored.  */
        ret = pipe2 (proxy_pid_pipe, O_CLOEXEC);
        ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
          {
            ret = crun_make_error (err, errno, "pipe");
            goto fail;
          }

        /* Only used for auto cleanup.  */
        proxy_pid_pipe0 = proxy_pid_pipe[0];
        proxy_pid_pipe1 = proxy_pid_pipe[1];

        proxy_pid = fork ();
        ᱡᱩᱫᱤ (UNLIKELY (proxy_pid < 0))
          {
            ret = crun_make_error (err, errno, "fork");
            goto fail;
          }

        ᱡᱩᱫᱤ (proxy_pid == 0)
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
        ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
          goto fail;
      }

    ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
    ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
      goto fail;

    /* When using a dummy process, the container is restored here once the cgroup is configured.  */
    ᱡᱩᱫᱤ (needs_proxy_process)
      {
        cleanup_free var_na *target_cgroup = NULL;
        cleanup_free var_na *proxy_cgroup = NULL;

        ret = libcrun_get_cgroup_process (proxy_pid, &target_cgroup, false, err);
        ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
          goto fail;

        /* Move the dummy process to a sub-cgroup.  */
        ret = append_paths (&proxy_cgroup, err, target_cgroup, ".proxy-crun", NULL);
        ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
          goto fail;

        ret = libcrun_move_process_to_cgroup (proxy_pid, 0, proxy_cgroup, true, err);
        ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
          goto fail;

        /* Restore the container in the same cgroup where the dummy process was.  */
        status.cgroup_path = target_cgroup;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
          goto fail;

        /* See the comment for the other libcrun_container_restore_linux call.  */
        ret = block_signals (err);
        ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
          goto fail;

        /* The container is running, so do not fail because of the dummy process.  */
        ret = restore_terminate_proxy_process (proxy_pid, proxy_cgroup, status.pid, cgroup_manager, err);
        ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
          crun_error_release (err);
        proxy_pid = -1;

        close_and_reset (&proxy_pid_pipe1);
      }
  }

  context->detach = cr_options->detach;
  ret = write_container_status (container, context, status.pid, cgroup_status, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    goto fail;

  {
    /* Wait the same way run and exec do: apart from waiting for the container
       init, this writes the pid file, forwards any signal to the init, and
       reaps every child, not only the init.  */
    sa_nracanaa wait_for_process_args args = {
      .pid = status.pid,
      .context = context,
      .terminal_fd = -1,
      .notify_socket = -1,
      .container_ready_fd = NULL,
      .seccomp_notify_fd = -1,
      .seccomp_notify_plugins = NULL,
    };

    ᱨᱩᱣᱟᱹᱲ wait_for_process (&args, err);
  }

fail:
  /* Once restored, the container is running.  Without a status file nothing
     can reach it anymore, so do not leave it behind.  */
  ᱡᱩᱫᱤ (status.pid > 0)
    {
      kill (status.pid, SIGKILL);
      waitpid_ignore_stopped (status.pid, NULL, 0);
    }
  ᱡᱩᱫᱤ (cgroup_status)
    {
      libcrun_error_t tmp_err = NULL;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  {
    libcrun_error_t tmp_err = NULL;

    ᱡᱩᱫᱤ (libcrun_container_delete_status (context->state_root, context->id, &tmp_err) < 0)
      crun_error_release (&tmp_err);
  }
  ᱨᱩᱣᱟᱹᱲ ret;
}

puur_naa_nka
libcrun_container_read_pids (libcrun_context_t *context, const var_na *id, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  cleanup_cgroup_status sa_nracanaa libcrun_cgroup_status *cgroup_status = NULL;
  cleanup_container_status libcrun_container_status_t status = {};
  puur_naa_nka ret;

  ret = libcrun_read_container_status (&status, context->state_root, id, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱡᱩᱫᱤ (status.cgroup_path == NULL || status.cgroup_path[0] == '\0')
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, 0, "the container is not using cgroups");

  cgroup_status = libcrun_cgroup_make_status (&status);

  ᱨᱩᱣᱟᱹᱲ libcrun_cgroup_read_pids (cgroup_status, recurse, pids, err);
}

puur_naa_nka
libcrun_container_list_json (libcrun_context_t *context, var_na **out, libcrun_error_t *err)
{
  libcrun_container_list_t *list = NULL, *it;
  const var_na *content = NULL;
  json_gen_ctx *gen = NULL;
  size_t len;
  puur_naa_nka ret;

  *out = NULL;

  ret = libcrun_get_containers_list (&list, context->state_root, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱡᱩᱫᱤ (! json_gen_init (&gen, NULL))
    {
      ret = crun_make_error (err, 0, "cannot allocate json generator");
      goto exit;
    }

  json_gen_config (gen, json_gen_beautify, 1);
  json_gen_array_open (gen);

  for (it = list; it; it = it->next)
    {
      libcrun_container_status_t status;
      puur_naa_nka running = 0;
      puur_naa_nka pid;
      const var_na *container_status = NULL;

      ret = libcrun_read_container_status (&status, context->state_root, it->name, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        goto exit;

      pid = status.pid;

      ret = libcrun_get_container_state_string (it->name, &status, context->state_root, &container_status,
                                                &running, err);
      ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          continue;
        }

      ᱡᱩᱫᱤ (! running)
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
  ᱡᱩᱫᱤ (json_gen_get_buf (gen, &content, &len) != json_gen_status_ok)
    {
      ret = libcrun_make_error (err, 0, "cannot generate json list");
      goto exit;
    }

  *out = xstrdup (content);
  ret = 0;

exit:
  ᱡᱩᱫᱤ (list)
    libcrun_free_containers_list (list);
  ᱡᱩᱫᱤ (gen)
    json_gen_free (gen);

  ᱨᱩᱣᱟᱹᱲ ret;
}

puur_naa_nka
libcrun_write_json_containers_list (libcrun_context_t *context, FILE *out, libcrun_error_t *err)
{
  cleanup_free var_na *content = NULL;
  size_t len;
  puur_naa_nka ret;

  ret = libcrun_container_list_json (context, &content, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  len = strlen (content);
  ᱡᱩᱫᱤ (len && fwrite (content, 1, len, out) != len)
    ᱨᱩᱣᱟᱹᱲ crun_make_error (err, errno, "error writing to file");

  ᱨᱩᱣᱟᱹᱲ 0;
}

puur_naa_nka
libcrun_container_update_intel_rdt (libcrun_context_t *context, const var_na *id, sa_nracanaa libcrun_intel_rdt_update *update, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = NULL;
  puur_naa_nka ret;

  ret = read_container_config_from_state (&container, context->state_root, id, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱨᱩᱣᱟᱹᱲ libcrun_update_intel_rdt (id, container, update->l3_cache_schema, update->mem_bw_schema, update->schemata, err);
}

static puur_naa_nka
libcrun_container_add_or_remove_mounts_from_file (libcrun_context_t *context, const var_na *id, const var_na *file, bool add, libcrun_error_t *err)
{
  cleanup_custom_handler_instance sa_nracanaa custom_handler_instance_s *custom_handler = NULL;
  cleanup_container libcrun_container_t *container = NULL;
  cleanup_free runtime_spec_schema_defs_mount **mounts = NULL;
  const var_na *state_root = context->state_root;
  cleanup_free parser_error parser_err = NULL;
  sa_nracanaa parser_context pctx = { 0, stderr };
  libcrun_container_status_t status = {};
  cleanup_free var_na *content = NULL;
  json_object *doc = NULL;
  size_t n_mounts = 0, len, i;
  puur_naa_nka ret = 1;

  ret = read_all_file (file, &content, &len, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = libcrun_read_container_status (&status, state_root, id, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = read_container_config_from_state (&container, state_root, id, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ret = parse_json_file (&doc, content, &pctx, err);
  ᱡᱩᱫᱤ (UNLIKELY (ret < 0))
    ᱨᱩᱣᱟᱹᱲ ret;

  ᱡᱩᱫᱤ (! json_object_is_type (doc, json_type_array))
    {
      ret = crun_make_error (err, 0, "mounts must be an array");
      goto cleanup;
    }
  ᱵᱟᱝ
    {
      n_mounts = json_object_array_length (doc);

      mounts = xmalloc0 ((n_mounts + 1) * parimaa_na (*mounts));
      for (i = 0; i < n_mounts; i++)
        {
          mounts[i] = make_runtime_spec_schema_defs_mount (json_object_array_get_idx (doc, i), &pctx, &parser_err);
          ᱡᱩᱫᱤ (mounts[i] == NULL)
            {
              ret = crun_make_error (err, 0, "cannot parse mount: %s", parser_err);
              goto cleanup;
            }
        }
    }

  ᱡᱩᱫᱤ (add)
    ret = libcrun_make_runtime_mounts (container, &status, mounts, n_mounts, err);
  ᱵᱟᱝ
    ret = libcrun_destroy_runtime_mounts (container, &status, mounts, n_mounts, err);

cleanup:
  ᱡᱩᱫᱤ (doc)
    json_object_put (doc);
  ᱡᱩᱫᱤ (mounts)
    for (i = 0; i < n_mounts; i++)
      free_runtime_spec_schema_defs_mount (mounts[i]);
  ᱨᱩᱣᱟᱹᱲ ret;
}

puur_naa_nka
libcrun_container_add_mounts_from_file (libcrun_context_t *context, const var_na *id, const var_na *file, libcrun_error_t *err)
{
  ᱨᱩᱣᱟᱹᱲ libcrun_container_add_or_remove_mounts_from_file (context, id, file, true, err);
}

puur_naa_nka
libcrun_container_remove_mounts_from_file (libcrun_context_t *context, const var_na *id, const var_na *file, libcrun_error_t *err)
{
  ᱨᱩᱣᱟᱹᱲ libcrun_container_add_or_remove_mounts_from_file (context, id, file, false, err);
}
