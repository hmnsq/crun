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
  ចំនួនគត់ has_terminal_socket_pair;
  ចំនួនគត់ terminal_socketpair[2];

  /* Used by log_write_to_sync_socket.  */
  ចំនួនគត់ sync_socket;

  ចំនួនគត់ seccomp_fd;
  ចំនួនគត់ seccomp_receiver_fd;
  ចំនួនគត់ console_socket_fd;

  ចំនួនគត់ hooks_out_fd;
  ចំនួនគត់ hooks_err_fd;

  /* Absolute bundle path, to be reported to hooks run in the container.  */
  const var_na *bundle;

  sa_nracanaa custom_handler_instance_s *custom_handler;
};

sa_nracanaa sync_socket_message_s
{
  ចំនួនគត់ type;
  ចំនួនគត់ error_value;
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

static ចំនួនគត់
sync_socket_write_msg (ចំនួនគត់ fd, ចំនួនគត់ verbosity, ចំនួនគត់ err_value, const var_na *log_msg)
{
  ចំនួនគត់ ret;
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

  ប្រសិនបើ (fd < 0)
    ត្រឡប់ 0;

  err_len = strlen (log_msg);
  ប្រសិនបើ (err_len >= parimaa_na (msg.message))
    err_len = parimaa_na (msg.message) - 1;

  memcpy (msg.message, log_msg, err_len);
  msg.message[err_len] = '\0';

  ret = TEMP_FAILURE_RETRY (write (fd, &msg, SYNC_SOCKET_MESSAGE_LEN (msg, err_len + 1)));
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ -1;

  ត្រឡប់ 0;
}

static ចំនួនគត់
sync_socket_write_error (ចំនួនគត់ fd, libcrun_error_t *out_err)
{
  ប្រសិនបើ (fd < 0)
    ត្រឡប់ 0;
  ត្រឡប់ sync_socket_write_msg (fd, false, (*out_err)->status, (*out_err)->msg);
}

static void
log_write_to_sync_socket (ចំនួនគត់ errno_, const var_na *msg, ចំនួនគត់ verbosity, void *arg)
{
  sa_nracanaa container_entrypoint_s *entrypoint_args = arg;
  ចំនួនគត់ fd = entrypoint_args->sync_socket;

  ប្រសិនបើ (fd < 0)
    ត្រឡប់;

  ប្រសិនបើ (sync_socket_write_msg (fd, verbosity, errno_, msg) < 0)
    log_write_to_stderr (errno_, msg, verbosity, arg);
}

static bool
is_memory_limit_too_low (runtime_spec_schema_config_schema *def)
{
  const long memory_limit_too_low = 1024 * 1024;

  ប្រសិនបើ (def->linux == NULL || def->linux->resources == NULL)
    ត្រឡប់ false;

  ប្រសិនបើ (def->linux->resources->memory
      && def->linux->resources->memory->limit_present
      && def->linux->resources->memory->limit < memory_limit_too_low)
    ត្រឡប់ true;

  ប្រសិនបើ (def->linux->resources->unified)
    {
      size_t i;

      សម្រាប់ (i = 0; i < def->linux->resources->unified->len; i++)
        ប្រសិនបើ (strcmp (def->linux->resources->unified->keys[i], "memory.max") == 0)
          {
            long limit;

            errno = 0;
            limit = strtol (def->linux->resources->unified->values[i], NULL, 10);
            ប្រសិនបើ (errno != 0)
              ត្រឡប់ false;
            ប្រសិនបើ (limit < memory_limit_too_low)
              ត្រឡប់ true;
          }
    }

  ត្រឡប់ false;
}

static ចំនួនគត់
sync_socket_wait_sync (libcrun_context_t *context, ចំនួនគត់ fd, bool flush, libcrun_error_t *err)
{
  sa_nracanaa sync_socket_message_s msg;

  ប្រសិនបើ (fd < 0)
    ត្រឡប់ 0;

  ខណៈពេល (true)
    {
      ចំនួនគត់ ret;

      ret = TEMP_FAILURE_RETRY (read (fd, &msg, parimaa_na (msg)));
      ប្រសិនបើ (UNLIKELY (ret < 0))
        {
          ប្រសិនបើ (flush)
            ត្រឡប់ 0;
          ត្រឡប់ crun_make_error (err, errno, "read from sync socket");
        }

      ប្រសិនបើ (ret == 0)
        {
          ប្រសិនបើ (flush)
            ត្រឡប់ 0;

          ត្រឡប់ crun_make_error (err, 0, "read from the init process");
        }

      ប្រសិនបើ (! flush && msg.type == SYNC_SOCKET_SYNC_MESSAGE)
        ត្រឡប់ 0;
      បើមិនដូច្នេះ ប្រសិនបើ (msg.type == SYNC_SOCKET_DEBUG_MESSAGE)
        {
          ប្រសិនបើ (context)
            context->output_handler (msg.error_value, msg.message, LIBCRUN_VERBOSITY_DEBUG, context->output_handler_arg);
          continue;
        }
      បើមិនដូច្នេះ ប្រសិនបើ (msg.type == SYNC_SOCKET_WARNING_MESSAGE)
        {
          ប្រសិនបើ (context)
            context->output_handler (msg.error_value, msg.message, LIBCRUN_VERBOSITY_WARNING, context->output_handler_arg);
          continue;
        }
      បើមិនដូច្នេះ ប្រសិនបើ (msg.type == SYNC_SOCKET_ERROR_MESSAGE)
        ត្រឡប់ crun_make_error (err, msg.error_value, "%s", msg.message);
    }
}

static ចំនួនគត់
sync_socket_send_sync (ចំនួនគត់ fd, bool flush_errors, libcrun_error_t *err)
{
  ចំនួនគត់ ret;
  sa_nracanaa sync_socket_message_s msg = {
    0,
  };
  msg.type = SYNC_SOCKET_SYNC_MESSAGE;

  ប្រសិនបើ (fd < 0)
    ត្រឡប់ 0;

  ret = TEMP_FAILURE_RETRY (write (fd, &msg, SYNC_SOCKET_MESSAGE_LEN (msg, 0)));
  ប្រសិនបើ (UNLIKELY (ret < 0))
    {
      ប្រសិនបើ (flush_errors)
        {
          ចំនួនគត់ saved_errno = errno;
          ret = TEMP_FAILURE_RETRY (read (fd, &msg, parimaa_na (msg)));
          ប្រសិនបើ (ret >= 0 && msg.type == SYNC_SOCKET_ERROR_MESSAGE)
            ត្រឡប់ crun_make_error (err, msg.error_value, "%s", msg.message);
          errno = saved_errno;
        }
      ត្រឡប់ crun_make_error (err, errno, "write to sync socket");
    }

  ត្រឡប់ 0;
}

static bool
has_cap_sys_ptrace ()
{
#ifdef HAVE_CAP
  sa_nracanaa __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  sa_nracanaa __user_cap_data_struct data[2] = { { 0 } };

  ប្រសិនបើ (UNLIKELY (capget (&hdr, data) < 0))
    ត្រឡប់ true;

  ត្រឡប់ (data[0].effective & (1U << CAP_SYS_PTRACE)) != 0;
#បើមិនដូច្នេះ
  ត្រឡប់ geteuid () == 0;
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
  ប្រសិនបើ (container_def->linux)
    {
      size_t i;

      សម្រាប់ (i = 0; i < container_def->linux->namespaces_len; i++)
        ប្រសិនបើ (container_def->linux->namespaces[i]->path && container_def->linux->namespaces[i]->path[0] == '\0')
          {
            free (container_def->linux->namespaces[i]->path);
            container_def->linux->namespaces[i]->path = NULL;
          }
    }

  ប្រសិនបើ (path)
    container->config_file = xstrdup (path);
  ប្រសិនបើ (config)
    container->config_file_content = xstrdup (config);

  ត្រឡប់ container;
}

libcrun_container_t *
libcrun_container_load_from_memory (const var_na *json, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free var_na *oci_error = NULL;
  container_def = runtime_spec_schema_config_schema_parse_data (json, NULL, &oci_error);
  ប្រសិនបើ (container_def == NULL)
    {
      crun_make_error (err, 0, "load: `%s`", oci_error);
      ត្រឡប់ NULL;
    }
  ត្រឡប់ make_container (container_def, NULL, json);
}

libcrun_container_t *
libcrun_container_load_from_file (const var_na *path, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free var_na *oci_error = NULL;
  cleanup_free var_na *content = NULL;
  size_t len;
  ចំនួនគត់ ret;

  libcrun_debug ("Loading container from config file: `%s`", path);
  ret = read_all_file (path, &content, &len, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ NULL;

  container_def = runtime_spec_schema_config_schema_parse_data (content, NULL, &oci_error);
  ប្រសិនបើ (container_def == NULL)
    {
      crun_make_error (err, 0, "load `%s`: %s", path, oci_error);
      ត្រឡប់ NULL;
    }
  ត្រឡប់ make_container (container_def, NULL, content);
}

const var_na *
libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err)
{
  sa_nracanaa parser_context ctx = { 0, stderr };
  parser_error gen_err = NULL;

  ប្រសិនបើ (container == NULL)
    {
      crun_make_error (err, EINVAL, "no container specified");
      ត្រឡប់ NULL;
    }

  ប្រសិនបើ (container->config_file_content == NULL && container->container_def)
    {
      container->config_file_content = runtime_spec_schema_config_schema_generate_json (container->container_def, &ctx, &gen_err);
      ប្រសិនបើ (container->config_file_content == NULL)
        {
          crun_make_error (err, 0, "cannot generate the configuration JSON: %s", gen_err ?: "unknown error");
          free (gen_err);
          ត្រឡប់ NULL;
        }
      free (gen_err);
    }

  ប្រសិនបើ (container->config_file_content == NULL)
    {
      crun_make_error (err, 0, "the container has no configuration");
      ត្រឡប់ NULL;
    }

  ត្រឡប់ container->config_file_content;
}

const var_na *
libcrun_container_get_annotation (libcrun_container_t *container, const var_na *key)
{
  ប្រសិនបើ (container == NULL)
    ត្រឡប់ NULL;
  ត្រឡប់ find_string_map_value (container->annotations, key);
}

size_t
libcrun_container_get_annotations_len (libcrun_container_t *container)
{
  ប្រសិនបើ (container == NULL || container->annotations == NULL)
    ត្រឡប់ 0;
  ត្រឡប់ string_map_size (container->annotations);
}

ចំនួនគត់
libcrun_container_get_annotation_at (libcrun_container_t *container, size_t index, const var_na **key,
                                     const var_na **value)
{
  ប្រសិនបើ (container == NULL)
    {
      errno = ERANGE;
      ត្រឡប់ -1;
    }
  ត្រឡប់ string_map_get_at (container->annotations, index, key, value);
}

uid_t
libcrun_container_get_uid (libcrun_container_t *container)
{
  ប្រសិនបើ (container == NULL)
    ត្រឡប់ (uid_t) -1;
  ត្រឡប់ container->host_uid;
}

gid_t
libcrun_container_get_gid (libcrun_container_t *container)
{
  ប្រសិនបើ (container == NULL)
    ត្រឡប់ (gid_t) -1;
  ត្រឡប់ container->host_gid;
}

void
libcrun_container_free (libcrun_container_t *ctr)
{
  ប្រសិនបើ (ctr == NULL)
    ត្រឡប់;

  ប្រសិនបើ (ctr->cleanup_private_data)
    ctr->cleanup_private_data (ctr->private_data);

  ប្រសិនបើ (ctr->container_def)
    free_runtime_spec_schema_config_schema (ctr->container_def);

  free_string_map (ctr->annotations);

  ប្រសិនបើ (ctr->proc_fd >= 0)
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

  ប្រសិនបើ (libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, id, NULL, err) < 0)
    {
      libcrun_context_free (ctx);
      ត្រឡប់ NULL;
    }

  ctx->handler_manager = libcrun_handler_manager_create (err);
  ប្រសិនបើ (ctx->handler_manager == NULL)
    {
      libcrun_context_free (ctx);
      ត្រឡប់ NULL;
    }

#ifdef CRUN_LIBDIR
  ប្រសិនបើ (access (CRUN_LIBDIR "/handlers", F_OK) == 0)
    {
      ប្រសិនបើ (libcrun_handler_manager_load_directory (ctx->handler_manager, CRUN_LIBDIR "/handlers", err) < 0)
        {
          libcrun_context_free (ctx);
          ត្រឡប់ NULL;
        }
    }
#endif

  ត្រឡប់ ctx;
}

void
libcrun_context_free (libcrun_context_t *ctx)
{
  ប្រសិនបើ (ctx == NULL)
    ត្រឡប់;

  free (ctx->owned_state_root);
  free (ctx->owned_id);
  free (ctx->owned_bundle);
  free (ctx->owned_console_socket);
  free (ctx->owned_pid_file);
  free (ctx->owned_notify_socket);
  free (ctx->owned_handler);

  ប្រសិនបើ (ctx->handler_manager)
    handler_manager_free (ctx->handler_manager);

  free (ctx);
}

#define LIBCRUN_CONTEXT_SET_STRING(field)                  \
  do                                                       \
    {                                                      \
      free (ctx->owned_##field);                           \
      ctx->owned_##field = value ? xstrdup (value) : NULL; \
      ctx->field = ctx->owned_##field;                     \
  } ខណៈពេល (0)

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
  ត្រឡប់ ctx->pid_file;
}

const var_na *
libcrun_context_get_state_root (libcrun_context_t *ctx)
{
  ប្រសិនបើ (ctx == NULL)
    ត្រឡប់ NULL;
  ត្រឡប់ ctx->state_root;
}

void
libcrun_context_set_preserve_fds (libcrun_context_t *ctx, ចំនួនគត់ preserve_fds)
{
  ctx->preserve_fds = preserve_fds;
}

ចំនួនគត់
libcrun_context_get_preserve_fds (libcrun_context_t *ctx)
{
  ត្រឡប់ ctx->preserve_fds;
}

void
libcrun_context_set_listen_fds (libcrun_context_t *ctx, ចំនួនគត់ listen_fds)
{
  ctx->listen_fds = listen_fds;
}

void
libcrun_context_set_args (libcrun_context_t *ctx, ចំនួនគត់ argc, var_na **argv)
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

ចំនួនគត់
libcrun_context_init_logging (libcrun_context_t *ctx, const var_na *log, libcrun_error_t *err)
{
  ត្រឡប់ libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, ctx->id, log, err);
}

static ចំនួនគត់
block_signals (libcrun_error_t *err)
{
  ចំនួនគត់ ret;
  sigset_t mask;
  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, NULL);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "sigprocmask");
  ត្រឡប់ 0;
}

static ចំនួនគត់
unblock_signals (libcrun_error_t *err)
{
  ចំនួនគត់ i;
  ចំនួនគត់ ret;
  sigset_t mask;
  sa_nracanaa sigaction act = {};

  sigfillset (&mask);
  ret = sigprocmask (SIG_UNBLOCK, &mask, NULL);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "sigprocmask");

  act.sa_handler = SIG_DFL;
  សម្រាប់ (i = 0; i < NSIG; i++)
    {
      ret = sigaction (i, &act, NULL);
      ប្រសិនបើ (ret < 0 && errno != EINVAL)
        ត្រឡប់ crun_make_error (err, errno, "sigaction");
    }

  ត្រឡប់ 0;
}

/* must be used on the host before pivot_root(2).  */
static ចំនួនគត់
initialize_security (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, libcrun_error_t *err)
{
  ចំនួនគត់ ret;

  ret = libcrun_init_caps (container, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (UNLIKELY (proc == NULL))
    ត្រឡប់ 0;

  ប្រសិនបើ (proc->apparmor_profile)
    {
      ret = libcrun_initialize_apparmor (err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }

  ret = libcrun_initialize_selinux (container, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ត្រឡប់ 0;
}

/* Emit the "annotations" map.  Returns a json_gen_status; callers can wrap
   the call with GEN_OR_FAIL.  */
static ចំនួនគត់
gen_annotations (json_gen_ctx *gen, json_map_string_string *annotations)
{
  size_t i;
  ចំនួនគត់ r;

  GEN_KEY (gen, "annotations");
  GEN_OR_FAIL (json_gen_map_open (gen));

  សម្រាប់ (i = 0; i < annotations->len; i++)
    {
      /* An annotation whose value is null in the document is parsed into a
         NULL, which cannot be emitted as a JSON string; leave it out.  */
      ប្រសិនបើ (annotations->keys[i] == NULL || annotations->values[i] == NULL)
        continue;

      GEN_STR (gen, annotations->keys[i]);
      GEN_STR (gen, annotations->values[i]);
    }

  ត្រឡប់ json_gen_map_close (gen);

gen_error:
  ត្រឡប់ r;
}

/* Return the bundle path to report in the container state.  crun always
   chdir()s into the bundle, so fall back to getcwd() when the caller has no
   absolute bundle path, as can happen when libcrun is used as a library.
   *ALLOCATED must be freed by the caller.  */
static const var_na *
get_bundle_path (const var_na *bundle, var_na **allocated)
{
  ប្រសិនបើ (bundle != NULL && bundle[0] == '/')
    ត្រឡប់ bundle;

  *allocated = getcwd (NULL, 0);
  ប្រសិនបើ (*allocated == NULL)
    OOM ();
  ត្រឡប់ *allocated;
}

static ចំនួនគត់
do_hooks (runtime_spec_schema_config_schema *def, pid_t pid, const var_na *id, bool keep_going, const var_na *bundle,
          const var_na *cwd, const var_na *status, const var_na *name, hook **hooks, size_t hooks_len, ចំនួនគត់ out_fd,
          ចំនួនគត់ err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  size_t i, stdin_len;
  ចំនួនគត់ r, ret;
  var_na *stdin = NULL;
  cleanup_free var_na *bundle_allocated = NULL;
  const var_na *rootfs = def->root ? def->root->path : "";
  cleanup_json_gen json_gen_ctx *gen = NULL;

  bundle = get_bundle_path (bundle, &bundle_allocated);

  ប្រសិនបើ (! json_gen_init (&gen, NULL))
    ត្រឡប់ crun_make_error (err, 0, "json_gen_init failed");

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

  ប្រសិនបើ (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, (const var_na **) &stdin, &stdin_len));

  ret = 0;

  bool error_created = false;
  សម្រាប់ (i = 0; i < hooks_len; i++)
    {
      var_na **env = environ;

      ប្រសិនបើ (hooks[i]->env)
        env = hooks[i]->env;

      /* Release the error from the previous iteration, if any.  */
      ប្រសិនបើ (error_created)
        {
          crun_error_release (err);
          error_created = false;
        }

      ret = run_process_with_stdin_timeout_envp (hooks[i]->path, hooks[i]->args, cwd, hooks[i]->timeout, env,
                                                 stdin, stdin_len, out_fd, err_fd, can_ignore_chdir_errors, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        error_created = true;

      ប្រសិនបើ (UNLIKELY (ret != 0))
        {
          ប្រសិនបើ (keep_going)
            libcrun_warning ("error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
          បើមិនដូច្នេះ
            {
              libcrun_error (0, "error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
              bha_nga;
            }
        }
    }

  ត្រឡប់ ret;

gen_error:
  ត្រឡប់ json_gen_error_to_crun_error (r, err);
}

static ចំនួនគត់
get_json_gen_result (json_gen_ctx *gen, var_na **out, size_t *out_len)
{
  const var_na *buf = NULL;
  size_t buf_len = 0;
  ចំនួនគត់ r;

  r = json_gen_get_buf (gen, &buf, &buf_len);
  ប្រសិនបើ (UNLIKELY (r != json_gen_status_ok))
    ត្រឡប់ r;

  *out_len = buf_len;

  *out = xmalloc (buf_len + 1);
  memcpy (*out, buf, buf_len);
  (*out)[buf_len] = '\0';

  ត្រឡប់ json_gen_status_ok;
}

static ចំនួនគត់
get_seccomp_receiver_fd_payload (libcrun_container_t *container, const var_na *status, pid_t own_pid,
                                 var_na **seccomp_fd_payload, size_t *seccomp_fd_payload_len, libcrun_error_t *err)
{
  ចំនួនគត់ r;
  json_gen_ctx *gen = NULL;
  runtime_spec_schema_config_schema *def = container->container_def;
  const var_na *const OCI_VERSION = "0.2.0";

  ប្រសិនបើ (! json_gen_init (&gen, NULL))
    ត្រឡប់ crun_make_error (err, 0, "json_gen_init failed");

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

  ប្រសិនបើ (def && def->linux && def->linux->seccomp)
    {
      const var_na *metadata = def->linux->seccomp->listener_metadata;

      ប្រសិនបើ (metadata)
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

  ប្រសិនបើ (container->context && container->context->id)
    {
      GEN_KEY (gen, "id");
      GEN_STR (gen, container->context->id);
    }

  GEN_KEY (gen, "status");
  GEN_STR (gen, status);

  GEN_KEY (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  ប្រសិនបើ (container->context && container->context->bundle)
    {
      GEN_KEY (gen, "bundle");
      GEN_STR (gen, container->context->bundle);
    }

  ប្រសិនបើ (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));
  /* End state.  */

  GEN_OR_FAIL (json_gen_map_close (gen));

  r = get_json_gen_result (gen, seccomp_fd_payload, seccomp_fd_payload_len);

gen_error:
  json_gen_free (gen);

  ត្រឡប់ json_gen_error_to_crun_error (r, err);
}

static ចំនួនគត់
send_sync_cb (void *data, libcrun_error_t *err)
{
  ចំនួនគត់ sync_socket_fd = *((ចំនួនគត់ *) data);
  ចំនួនគត់ ret;

  /* sync 2.  */
  ret = sync_socket_send_sync (sync_socket_fd, false, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  /* sync 3.  */
  ត្រឡប់ sync_socket_wait_sync (NULL, sync_socket_fd, false, err);
}

static ចំនួនគត់
maybe_chown_std_streams (uid_t container_uid, gid_t container_gid, libcrun_error_t *err)
{
  ចំនួនគត់ ret, i;

  សម្រាប់ (i = 0; i < 3; i++)
    {
      ប្រសិនបើ (! isatty (i))
        {
          sa_nracanaa stat statbuf;
          ret = fstat (i, &statbuf);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            {
              ប្រសិនបើ (errno == EBADF)
                continue;
              ត្រឡប់ crun_make_error (err, errno, "fstat fd `%d`", i);
            }

          /* Skip chown for device files */
          ប្រសិនបើ (S_ISCHR (statbuf.st_mode) || S_ISBLK (statbuf.st_mode))
            continue;

          ret = fchown (i, container_uid, container_gid);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            {
              /* EINVAL means the user is not mapped in the current userns.
                 Ignore EPERM and EROFS as well as there is no reason to fail
                 so early, and let the container payload deal with it.
                 EBADF means fd is closed.
              */
              ប្រសិនបើ (errno == EINVAL || errno == EPERM || errno == EROFS || errno == EBADF)
                continue;

              ត្រឡប់ crun_make_error (err, errno, "fchown std stream `%i`", i);
            }
        }
    }
  ត្រឡប់ 0;
}

ចំនួនគត់
libcrun_container_notify_handler (sa_nracanaa container_entrypoint_s *args,
                                  ga_nanaa handler_configure_phase phase,
                                  libcrun_container_t *container, const var_na *rootfs,
                                  libcrun_error_t *err)
{
  sa_nracanaa custom_handler_s *h;

  ប្រសិនបើ (args->custom_handler == NULL)
    ត្រឡប់ 0;

  h = args->custom_handler->vtable;
  ប្រសិនបើ (h == NULL || h->configure_container == NULL)
    ត្រឡប់ 0;

  ត្រឡប់ h->configure_container (args->custom_handler->cookie, phase,
                                 args->context, container,
                                 rootfs, err);
}

/* Resolve and normalize the container rootfs path.  */
static ចំនួនគត់
resolve_rootfs_path (libcrun_container_t *container, var_na **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ប្រសិនបើ (def->root && def->root->path)
    {
      *rootfs = realpath (def->root->path, NULL);
      ប្រសិនបើ (UNLIKELY (*rootfs == NULL))
        {
          ចំនួនគត់ saved_errno = errno;

          /* If realpath failed for any reason, try the relative directory.  */
          ប្រសិនបើ (def->root->path[0] == '/')
            {
              cleanup_free var_na *cwd = NULL;
              libcrun_error_t tmp_err = NULL;
              ssize_t len;
              ចំនួនគត់ proc_fd;

              proc_fd = libcrun_get_cached_proc_fd (container, &tmp_err);
              ប្រសិនបើ (LIKELY (proc_fd >= 0))
                {
                  len = safe_readlinkat (proc_fd, "self/cwd", &cwd, 0, &tmp_err);
                  ប្រសិនបើ (LIKELY (len > 0))
                    {
                      /* If the rootfs is under the current working directory, just use its relative path.  */
                      ប្រសិនបើ (has_dir_prefix (def->root->path, cwd))
                        {
                          const var_na *it = consume_slashes (def->root->path + len);
                          ប្រសិនបើ (LIKELY (*it))
                            {
                              ចំនួនគត់ ret = faccessat (AT_FDCWD, it, F_OK, 0);
                              ប្រសិនបើ (LIKELY (ret == 0))
                                *rootfs = xstrdup (it);
                            }
                        }
                    }
                }
              crun_error_release (&tmp_err);
            }

          ប្រសិនបើ (*rootfs == NULL)
            {
              ប្រសិនបើ (saved_errno == ENOENT)
                ត្រឡប់ crun_make_error (err, ENOENT, "the rootfs path `%s` does not exist", def->root->path);

              *rootfs = xstrdup (def->root->path);
            }
        }
    }
  ត្រឡប់ 0;
}

/* Initialize the environment variables.  */
static ចំនួនគត់
setup_environment (runtime_spec_schema_config_schema *def, uid_t container_uid, libcrun_error_t *err)
{
  ចំនួនគត់ ret;

  ប្រសិនបើ (clearenv ())
    ត្រឡប់ crun_make_error (err, errno, "clearenv");

  ប្រសិនបើ (def->process)
    {
      size_t i;

      សម្រាប់ (i = 0; i < def->process->env_len; i++)
        {
          /* A null element of the array of variables is parsed into a NULL,
             which putenv(3) cannot be handed.  */
          ប្រសិនបើ (def->process->env[i] == NULL)
            ត្រឡប់ crun_make_error (err, EINVAL, "environment variable is not specified");

          ប្រសិនបើ (putenv (def->process->env[i]) < 0)
            ត្រឡប់ crun_make_error (err, errno, "putenv `%s`", def->process->env[i]);
        }
    }

  ប្រសិនបើ (is_empty_string (getenv ("HOME")))
    {
      ret = set_home_env (container_uid);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          ប្រសិនបើ (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            ត្រឡប់ crun_make_error (err, errno, "setenv HOME");
        }
    }

  ត្រឡប់ 0;
}

/* Configure terminal and console for interactive containers.  */
static ចំនួនគត់
setup_terminal (sa_nracanaa container_entrypoint_s *entrypoint_args, libcrun_container_t *container,
                ចំនួនគត់ console_socket, ចំនួនគត់ console_socketpair, ចំនួនគត់ has_terminal, libcrun_error_t *err)
{
  cleanup_close ចំនួនគត់ terminal_fd = -1;
  ចំនួនគត់ ret;

  ប្រសិនបើ (! has_terminal)
    ត្រឡប់ 0;

  fflush (stderr);

  ប្រសិនបើ (console_socket >= 0 || (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0))
    {
      terminal_fd = libcrun_set_terminal (container, err);
      ប្រសិនបើ (UNLIKELY (terminal_fd < 0))
        ត្រឡប់ terminal_fd;
    }

  ប្រសិនបើ (console_socket >= 0)
    {
      ret = send_fd_to_socket (console_socket, terminal_fd, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }
  បើមិនដូច្នេះ ប្រសិនបើ (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0)
    {
      ret = send_fd_to_socket (console_socketpair, terminal_fd, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }

  ត្រឡប់ 0;
}

/* Resolve and validate the container's executable path.  */
static ចំនួនគត់
setup_executable_path (sa_nracanaa container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                       var_na **exec_path, libcrun_error_t *err)
{
  ចំនួនគត់ ret;

  ប្រសិនបើ (def->process && def->process->args)
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        {
          ប្រសិនបើ (entrypoint_args->custom_handler == NULL && crun_error_get_errno (err) == ENOENT)
            ត្រឡប់ ret;

          /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
             once the process switched to the use that runs in the container.  This might be necessary
             when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
             is not honored.  */
          crun_error_release (err);
        }
    }

  ត្រឡប់ 0;
}

/* Apply security settings for the container process.  */
static ចំនួនគត់
apply_security_settings (sa_nracanaa container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                         libcrun_container_t *container, pid_t own_pid, libcrun_error_t *err)
{
  ចំនួនគត់ ret;
  runtime_spec_schema_config_schema_process_capabilities *capabilities;
  ចំនួនគត់ no_new_privs;

  ប្រសិនបើ (def->process && ! def->process->no_new_privileges)
    {
      var_na **seccomp_flags = NULL;
      size_t seccomp_flags_len = 0;
      cleanup_free var_na *seccomp_fd_payload = NULL;
      size_t seccomp_fd_payload_len = 0;

      ប្រសិនបើ (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      ប្រសិនបើ (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  capabilities = def->process ? def->process->capabilities : NULL;
  no_new_privs = def->process ? def->process->no_new_privileges : 1;
  ret = libcrun_set_caps (capabilities, container->container_uid, container->container_gid, no_new_privs,
                          entrypoint_args->custom_handler != NULL, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ត្រឡប់ 0;
}

/* Create the process that joins the existing PID namespace now that the
   host mounts are not visible anymore, and continue the setup from there.  */
static ចំនួនគត់
do_delayed_pidns_fork (ចំនួនគត់ sync_socket, pid_t *own_pid, libcrun_error_t *err)
{
  pid_t pid;
  ចំនួនគត់ ret;

  pid = fork ();
  ប្រសិនបើ (UNLIKELY (pid < 0))
    ត្រឡប់ crun_make_error (err, errno, "fork");

  ប្រសិនបើ (pid)
    {
      /* Report the new PID to the parent and exit immediately.  */
      ret = sync_socket_send_sync (sync_socket, false, err);
      ប្រសិនបើ (LIKELY (ret == 0))
        {
          ret = TEMP_FAILURE_RETRY (write (sync_socket, &pid, parimaa_na (pid)));
          ប្រសិនបើ (LIKELY (ret == parimaa_na (pid)))
            _safe_exit (EXIT_SUCCESS);
        }

      kill (pid, SIGKILL);
      _safe_exit (EXIT_FAILURE);
    }

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (read (sync_socket, own_pid, parimaa_na (*own_pid)));
  ប្រសិនបើ (UNLIKELY (ret != parimaa_na (*own_pid)))
    {
      ប្រសិនបើ (ret >= 0)
        errno = 0;
      ត្រឡប់ crun_make_error (err, errno, "read from sync socket");
    }

  ត្រឡប់ 0;
}

sa_nracanaa delayed_pidns_fork_s
{
  libcrun_container_t *container;
  ចំនួនគត់ sync_socket;
  pid_t *own_pid;
};

static ចំនួនគត់
delayed_pidns_fork_cb (void *data, libcrun_error_t *err)
{
  sa_nracanaa delayed_pidns_fork_s *d = data;

  ប្រសិនបើ (! libcrun_container_delayed_pidns_fork (d->container))
    ត្រឡប់ 0;

  ត្រឡប់ do_delayed_pidns_fork (d->sync_socket, d->own_pid, err);
}

static ចំនួនគត់
container_init_setup (void *args, pid_t *own_pid, var_na *notify_socket,
                      ចំនួនគត់ sync_socket, var_na **exec_path, libcrun_error_t *err)
{
  sa_nracanaa container_entrypoint_s *entrypoint_args = args;
  libcrun_container_t *container = entrypoint_args->container;
  bool chdir_done = false;
  ចំនួនគត់ ret;
  ចំនួនគត់ has_terminal;
  cleanup_close ចំនួនគត់ console_socket = -1;
  cleanup_close ចំនួនគត់ console_socketpair = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free var_na *rootfs = NULL;
  sa_nracanaa delayed_pidns_fork_s delayed_pidns_fork = {
    .container = container,
    .sync_socket = sync_socket,
    .own_pid = own_pid,
  };

  ret = initialize_security (container, def->process, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = libcrun_configure_network (container, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = resolve_rootfs_path (container, &rootfs, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  /* Configure terminal socket pair for container communication.  */
  ប្រសិនបើ (entrypoint_args->terminal_socketpair[0] >= 0)
    {
      close_and_reset (&entrypoint_args->terminal_socketpair[0]);
      console_socketpair = entrypoint_args->terminal_socketpair[1];
    }

  /* sync 1.  */
  ret = sync_socket_wait_sync (NULL, sync_socket, false, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  has_terminal = container->container_def->process && container->container_def->process->terminal;
  ប្រសិនបើ (has_terminal && entrypoint_args->context->console_socket)
    console_socket = entrypoint_args->console_socket_fd;

  ret = libcrun_set_sysctl (container, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_BEFORE_MOUNTS, container, rootfs, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = libcrun_do_pivot_root (container, entrypoint_args->context->no_pivot, &rootfs, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  /* sync 2 and 3 are sent as part of libcrun_set_mounts.  */
  ret = libcrun_set_mounts (entrypoint_args, container, rootfs, send_sync_cb, &sync_socket, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  /* The createContainer hooks MUST run after the mounts are set up but before
     the pivot_root.  When these hooks are present can_use_open_tree_namespace()
     forces the deferred-pivot path, so the actual pivot happens later in
     libcrun_finalize_mounts().  */
  ប្រសិនបើ (def->hooks && def->hooks->create_container_len)
    {
      libcrun_error_t tmp_err = NULL;
      ចំនួនគត់ in_userns = check_running_in_user_namespace (&tmp_err);
      ប្រសិនបើ (tmp_err)
        crun_error_release (&tmp_err);
      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, NULL, "created", "createContainer",
                      (hook **) def->hooks->create_container,
                      def->hooks->create_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      ប្រសិនបើ (UNLIKELY (ret != 0))
        ត្រឡប់ ret;
    }

  ret = libcrun_finalize_mounts (entrypoint_args, container, rootfs, delayed_pidns_fork_cb, &delayed_pidns_fork, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (def->process)
    {
      ret = libcrun_set_selinux_label (container, def->process, false, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      ret = libcrun_set_apparmor_profile (container, def->process, false, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }

  ret = mark_or_close_fds_ge_than (container, entrypoint_args->context->preserve_fds + 3, false, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ret = libcrun_reopen_dev_null (err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = maybe_chown_std_streams (container->container_uid, container->container_gid, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = setup_environment (def, container->container_uid, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  /* Set primary process to 1 explicitly if nothing is configured and LISTEN_FD is not set.  */
  ប្រសិនបើ (entrypoint_args->context->listen_fds > 0 && getenv ("LISTEN_PID") == NULL)
    {
      libcrun_warning ("setting LISTEN_PID=1 since no previous configuration was found");
      ប្រសិនបើ (UNLIKELY (setenv ("LISTEN_PID", "1", 1) < 0))
        ត្រឡប់ crun_make_error (err, errno, "setenv LISTEN_PID");
    }

  /* Attempt to chdir immediately here, before doing the setresuid.  If we fail here, let's
     try again later once the process switched to the user that runs in the container.  */
  ប្រសិនបើ (def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      ប្រសិនបើ (LIKELY (ret == 0))
        chdir_done = true;
      បើមិនដូច្នេះ
        crun_error_release (err);
    }

  ret = setsid ();
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "setsid");

  ret = setup_terminal (entrypoint_args, container, console_socket, console_socketpair, has_terminal, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  close_and_reset (&console_socket);
  close_and_reset (&console_socketpair);

  ret = setup_executable_path (entrypoint_args, def, exec_path, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = libcrun_set_hostname (container, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = libcrun_set_domainname (container, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }

  ប្រសិនបើ (def->process && def->process->user)
    umask (def->process->user->umask_present ? def->process->user->umask : 0022);

  ret = apply_security_settings (entrypoint_args, def, container, *own_pid, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (UNLIKELY (def->process && def->process->args && *exec_path == NULL))
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        {
          ប្រសិនបើ (entrypoint_args->custom_handler == NULL || is_empty_string (def->process->args[0]))
            ត្រឡប់ ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          *exec_path = xstrdup (def->process->args[0]);
        }
    }

  /* The chdir was not already performed, so try again now after switching to the UID/GID in the container.  */
  ប្រសិនបើ (! chdir_done && def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }

  ប្រសិនបើ (notify_socket && putenv (notify_socket) < 0)
    ត្រឡប់ crun_make_error (err, errno, "putenv `%s`", notify_socket);

  ត្រឡប់ 0;
}

static ចំនួនគត់
open_hooks_output (libcrun_container_t *container, ចំនួនគត់ *out_fd, ចំនួនគត់ *err_fd, libcrun_error_t *err)
{
  const var_na *annotation;

  *err_fd = *out_fd = -1;

  libcrun_debug ("Opening hooks output");
  annotation = find_annotation (container, "run.oci.hooks.stdout");
  ប្រសិនបើ (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stdout` annotation");
      *out_fd = TEMP_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      ប្រសិនបើ (UNLIKELY (*out_fd < 0))
        ត្រឡប់ crun_make_error (err, errno, "open `%s`", annotation);
    }

  annotation = find_annotation (container, "run.oci.hooks.stderr");
  ប្រសិនបើ (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stderr` annotation");
      *err_fd = TEMP_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      ប្រសិនបើ (UNLIKELY (*err_fd < 0))
        ត្រឡប់ crun_make_error (err, errno, "open `%s`", annotation);
    }

  ត្រឡប់ 0;
}

/* The environment the process was started with lives on the initial stack, right after the
   argv strings and before the string AT_EXECFN points to.  clearenv(3) only drops the pointers
   to it, so when there is no execv to replace the stack, erase it or the container process can
   still read it, e.g. through /proc/self/environ.  */
static void
erase_initial_environment (var_na **argv, ចំនួនគត់ argc)
{
  var_na *start = argv[argc - 1] + strlen (argv[argc - 1]) + 1;
  var_na *end = (var_na *) getauxval (AT_EXECFN);

  ប្រសិនបើ (end > start)
    memset (start, 0, end - start);
}

static void
rewrite_argv (var_na **argv, ចំនួនគត់ argc, const var_na *name, var_na **args, size_t args_len)
{
  cleanup_free var_na *decorated_name = NULL;
  size_t available_len = argv[argc - 1] - argv[0] + strlen (argv[argc - 1]) + 1;
  cleanup_free var_na *new_argv = NULL;
  size_t i, so_far = 0, needed = 0;

  needed = xasprintf (&decorated_name, "[libcrun:%s]", name) + 1;
  សម្រាប់ (i = 0; i < args_len; i++)
    needed += strlen (args[i]) + 1;

  new_argv = xmalloc0 (needed + 1);
  strcpy (new_argv, decorated_name);
  so_far = strlen (decorated_name) + 1;

  ប្រសិនបើ (available_len >= needed)
    {
      សម្រាប់ (i = 0; i < args_len; i++)
        {
          strcpy (new_argv + so_far, args[i]);
          so_far += strlen (args[i]) + 1;
        }
    }

  ប្រសិនបើ (so_far >= available_len)
    so_far = available_len - 1;

  memset (argv[0], 0, available_len);
  memcpy (argv[0], new_argv, so_far);
}

/* Entrypoint to the container.  */
static ចំនួនគត់
container_init (void *args, var_na *notify_socket, ចំនួនគត់ sync_socket, libcrun_error_t *err)
{
  sa_nracanaa container_entrypoint_s *entrypoint_args = args;
  ចំនួនគត់ ret;
  runtime_spec_schema_config_schema *def = entrypoint_args->container->container_def;
  cleanup_free var_na *exec_path = NULL;
  __attribute__ ((unused)) cleanup_free var_na *notify_socket_cleanup = notify_socket;
  pid_t own_pid = 0;

  entrypoint_args->sync_socket = sync_socket;

  crun_set_output_handler (log_write_to_sync_socket, args);

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (read (sync_socket, &own_pid, parimaa_na (own_pid)));
  ប្រសិនបើ (UNLIKELY (ret != parimaa_na (own_pid)))
    {
      ប្រសិនបើ (ret >= 0)
        errno = 0;
      ត្រឡប់ crun_make_error (err, errno, "read from sync socket");
    }

  ret = container_init_setup (args, &own_pid, notify_socket, sync_socket, &exec_path, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    {
      /* If it fails to write the error using the sync socket, then fallback
         to stderr.  */
      ប្រសិនបើ (sync_socket_write_error (sync_socket, err) == 0)
        {
          crun_error_release (err);
          /* A return value of > 0 means to fail with an error without err being set so to avoid
             printing it twice.  */
          ត្រឡប់ 1;
        }
      ត្រឡប់ ret;
    }

  entrypoint_args->sync_socket = -1;

  ret = unblock_signals (err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  /* sync 4.  */
  ret = sync_socket_send_sync (sync_socket, false, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  close_and_reset (&sync_socket);

  ប្រសិនបើ (entrypoint_args->context->fifo_exec_wait_fd >= 0)
    {
      var_na buffer[1];
      fd_set read_set;
      cleanup_close ចំនួនគត់ fd = entrypoint_args->context->fifo_exec_wait_fd;
      entrypoint_args->context->fifo_exec_wait_fd = -1;

      FD_ZERO (&read_set);
      FD_SET (fd, &read_set);
      do
        {
          ret = select (fd + 1, &read_set, NULL, NULL, NULL);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ crun_make_error (err, errno, "select");

          ret = TEMP_FAILURE_RETRY (read (fd, buffer, parimaa_na (buffer)));
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ crun_make_error (err, errno, "read from the exec fifo");
      } ខណៈពេល (ret == 0);

      close_and_reset (&entrypoint_args->context->fifo_exec_wait_fd);
    }

  crun_set_output_handler (log_write_to_stderr, NULL);

  ប្រសិនបើ (def->process && def->process->no_new_privileges)
    {
      var_na **seccomp_flags = NULL;
      size_t seccomp_flags_len = 0;
      cleanup_free var_na *seccomp_fd_payload = NULL;
      size_t seccomp_fd_payload_len = 0;

      ប្រសិនបើ (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      ប្រសិនបើ (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (entrypoint_args->container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags,
                                   seccomp_flags_len, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  ប្រសិនបើ (UNLIKELY (def->process == NULL))
    ត្រឡប់ crun_make_error (err, 0, "block `process` not found");

  ប្រសិនបើ (UNLIKELY (exec_path == NULL))
    ត្រឡប់ crun_make_error (err, 0, "executable path not specified");

  ប្រសិនបើ (def->hooks && def->hooks->start_container_len)
    {
      libcrun_container_t *container = entrypoint_args->container;
      libcrun_error_t tmp_err = NULL;
      ចំនួនគត់ in_userns = check_running_in_user_namespace (&tmp_err);
      ប្រសិនបើ (tmp_err)
        crun_error_release (&tmp_err);

      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, NULL, "created", "startContainer",
                      (hook **) def->hooks->start_container,
                      def->hooks->start_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      ប្រសិនបើ (UNLIKELY (ret != 0))
        ត្រឡប់ ret;

      /* Seek stdout/stderr to the end.  If the hooks were using the same files,
         the container process overwrites what was previously written.  */
      (void) lseek (1, 0, SEEK_END);
      (void) lseek (2, 0, SEEK_END);
    }

  ប្រសិនបើ (entrypoint_args->custom_handler)
    {
      /* Files marked with O_CLOEXEC are closed at execv time, so make sure they are closed now.
         This is a best effort operation, because the seccomp filter is already in place and it could
         stop some syscalls used by mark_or_close_fds_ge_than.
      */
      ប្រសិនបើ (entrypoint_args->custom_handler->vtable->close_fds)
        {
          ret = entrypoint_args->custom_handler->vtable->close_fds (entrypoint_args->custom_handler->cookie,
                                                                    entrypoint_args->container,
                                                                    entrypoint_args->context->preserve_fds,
                                                                    err);
        }
      បើមិនដូច្នេះ
        {
          ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
        }
      ប្រសិនបើ (UNLIKELY (ret < 0))
        crun_error_release (err);

      prctl (PR_SET_NAME, entrypoint_args->custom_handler->vtable->name);

      ប្រសិនបើ (entrypoint_args->context->argv)
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
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      ret = libcrun_set_apparmor_profile (entrypoint_args->container, def->process, true, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      ret = entrypoint_args->custom_handler->vtable->run_func (entrypoint_args->custom_handler->cookie,
                                                               entrypoint_args->container,
                                                               exec_path,
                                                               def->process->args);
      ប្រសិនបើ (ret != 0)
        ត្រឡប់ crun_make_error (err, 0, "exec container process failed with handler as `%s`", entrypoint_args->custom_handler->vtable->name);

      ត្រឡប់ ret;
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation since the seccomp profile is already in place now and might block
     some of the syscalls needed by mark_or_close_fds_ge_than.  */
  ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, def->process->args));

  ប្រសិនបើ (errno == ENOENT)
    ត្រឡប់ crun_make_error (err, errno, "exec container process (missing dynamic library?) `%s`", exec_path);

  ត្រឡប់ crun_make_error (err, errno, "exec container process `%s`", exec_path);
}

static ចំនួនគត់
read_container_config_from_state (libcrun_container_t **container, const var_na *state_root, const var_na *id,
                                  libcrun_error_t *err)
{
  cleanup_free var_na *config_file = NULL;
  cleanup_free var_na *dir = NULL;
  ចំនួនគត់ ret;

  *container = NULL;

  ret = libcrun_get_state_directory (&dir, state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = append_paths (&config_file, err, dir, "config.json", NULL);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  *container = libcrun_container_load_from_file (config_file, err);
  ប្រសិនបើ (*container == NULL)
    ត្រឡប់ -1;

  ត្រឡប់ 0;
}

static ចំនួនគត់
run_poststop_hooks (arg_unused libcrun_context_t *context, libcrun_container_t *container,
                    runtime_spec_schema_config_schema *def,
                    libcrun_container_status_t *status, const var_na *state_root, const var_na *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container_cleanup = NULL;
  ចំនួនគត់ ret;

  ប្រសិនបើ (def == NULL)
    {
      ប្រសិនបើ (container == NULL)
        {
          ret = read_container_config_from_state (&container_cleanup, state_root, id, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;
          container = container_cleanup;
        }

      def = container->container_def;
    }

  ប្រសិនបើ (def->hooks && def->hooks->poststop_len)
    {
      cleanup_close ចំនួនគត់ hooks_out_fd = -1;
      cleanup_close ចំនួនគត់ hooks_err_fd = -1;

      ប្រសិនបើ (container == NULL)
        {
          ret = read_container_config_from_state (&container_cleanup, state_root, id, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;
          container = container_cleanup;
        }

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      ret = do_hooks (def, 0, id, true, status->bundle, status->bundle, "stopped", "poststop", (hook **) def->hooks->poststop,
                      def->hooks->poststop_len, hooks_out_fd, hooks_err_fd, false, err);
      ប្រសិនបើ (UNLIKELY (ret != 0))
        {
          ប្រសិនបើ (ret < 0)
            libcrun_error_report_and_release (err);
          បើមិនដូច្នេះ
            libcrun_error (0, "poststop hook failed with exit code: %d", ret);
        }
    }
  ត្រឡប់ 0;
}

static bool
has_new_pid_namespace (runtime_spec_schema_config_schema *def)
{
  size_t i;

  ប្រសិនបើ (def->linux == NULL || def->linux->namespaces == NULL)
    ត្រឡប់ false;

  សម្រាប់ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ប្រសិនបើ (strcmp (def->linux->namespaces[i]->type, "pid") == 0 && def->linux->namespaces[i]->path == NULL)
        ត្រឡប់ true;
    }
  ត្រឡប់ false;
}

/* Delete the container state.  CONTAINER is the container the caller already has
   available, or NULL.  When it is not NULL, the poststop hooks are run even if the
   status file was never written, e.g. when the container creation failed.  */
static ចំនួនគត់
container_delete_internal (libcrun_context_t *context, libcrun_container_t *container,
                           runtime_spec_schema_config_schema *def, const var_na *id, bool force, bool killall,
                           libcrun_error_t *err)
{
  cleanup_cgroup_status sa_nracanaa libcrun_cgroup_status *cgroup_status = NULL;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_container libcrun_container_t *container_cleanup = NULL;
  const var_na *state_root = context->state_root;
  ចំនួនគត់ ret;

  ret = libcrun_read_container_status (&status, state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    {
      ប្រសិនបើ (force && crun_error_get_errno (err) == ENOENT)
        {
          libcrun_error_t tmp_err = NULL;

          crun_error_release (err);

          ប្រសិនបើ (container)
            {
              libcrun_container_status_t partial_status = {
                .bundle = (var_na *) context->bundle,
              };

              ret = run_poststop_hooks (context, container, def, &partial_status, state_root, id, err);
              ប្រសិនបើ (UNLIKELY (ret < 0))
                crun_error_write_warning_and_release (context->output_handler_arg, &err);
            }

          libcrun_container_delete_status (state_root, id, &tmp_err);
          crun_error_release (&tmp_err);
          ត្រឡប់ 0;
        }
      crun_error_release (err);
      ត្រឡប់ libcrun_container_delete_status (state_root, id, err);
    }

  cgroup_status = libcrun_cgroup_make_status (&status);

  ប្រសិនបើ (! force)
    {
      ret = libcrun_is_container_running (&status, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
      ប្រសិនបើ (ret == 1)
        {
          ret = libcrun_status_has_read_exec_fifo (state_root, id, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;
          ប្រសិនបើ (ret == 0)
            ត្រឡប់ crun_make_error (err, 0, "the container `%s` is not in `created` or `stopped` state", id);

          /* If the container is in "created" state, then do the equivalent of delete --force.  */
          killall = force = true;
        }
    }

  ប្រសិនបើ (def == NULL)
    {
      ប្រសិនបើ (container == NULL)
        {
          ret = read_container_config_from_state (&container_cleanup, state_root, id, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;

          container = container_cleanup;
        }

      def = container->container_def;
    }

  ប្រសិនបើ (killall && force)
    {
      /* If the container has a pid namespace, it is enough to kill the first
         process (pid=1 in the namespace).
      */
      ប្រសិនបើ (has_new_pid_namespace (def))
        {
          ret = libcrun_kill_linux (&status, SIGKILL, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            {
              ប្រសិនបើ (crun_error_get_errno (err) != ESRCH)
                ត្រឡប់ ret;

              crun_error_release (err);
            }
        }
      បើមិនដូច្នេះ ប្រសិនបើ (status.cgroup_path)
        {
          ret = libcrun_cgroup_killall (cgroup_status, SIGKILL, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;
        }
    }

  ប្រសិនបើ (def->linux && def->linux->intel_rdt)
    {
      ret = libcrun_destroy_intelrdt (id, def, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        libcrun_error_report_and_release (err);
    }

  ប្រសិនបើ (status.cgroup_path)
    {
      /* If the cgroup cannot be destroyed, there are still processes in it.
         Keep the container status, it is the only record of the cgroup, so
         that the delete can be retried.  */
      ret = libcrun_cgroup_destroy (cgroup_status, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }

  ret = run_poststop_hooks (context, container, def, &status, state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ត្រឡប់ libcrun_container_delete_status (state_root, id, err);
}

ចំនួនគត់
libcrun_container_delete (libcrun_context_t *context, const var_na *id, bool force, libcrun_error_t *err)
{
  ត្រឡប់ container_delete_internal (context, NULL, NULL, id, force, true, err);
}

ចំនួនគត់
libcrun_container_kill (libcrun_context_t *context, const var_na *id, const var_na *sa_nkeeta, libcrun_error_t *err)
{
  ចំនួនគត់ sig, ret;
  const var_na *state_root = context->state_root;
  cleanup_container_status libcrun_container_status_t status = {};

  sig = str2sig (sa_nkeeta);
  ប្រសិនបើ (UNLIKELY (sig < 0))
    ត្រឡប់ crun_make_error (err, 0, "unknown signal `%s`", sa_nkeeta);

  ret = libcrun_read_container_status (&status, state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ត្រឡប់ libcrun_kill_linux (&status, sig, err);
}

ចំនួនគត់
libcrun_container_killall (libcrun_context_t *context, const var_na *id, const var_na *sa_nkeeta, libcrun_error_t *err)
{
  ចំនួនគត់ sig, ret;
  const var_na *state_root = context->state_root;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_cgroup_status sa_nracanaa libcrun_cgroup_status *cgroup_status = NULL;

  sig = str2sig (sa_nkeeta);
  ប្រសិនបើ (UNLIKELY (sig < 0))
    ត្រឡប់ crun_make_error (err, 0, "unknown signal `%s`", sa_nkeeta);

  ret = libcrun_read_container_status (&status, state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  cgroup_status = libcrun_cgroup_make_status (&status);

  ret = libcrun_cgroup_killall (cgroup_status, sig, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;
  ត្រឡប់ 0;
}

static ចំនួនគត់
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

  ប្រសិនបើ (cgroup_status)
    {
      ចំនួនគត់ ret;

      ret = libcrun_cgroup_get_status (cgroup_status, &status, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }

  ប្រសិនបើ (external_descriptors == NULL)
    ត្រឡប់ crun_make_error (err, 0, "invalid internal state.  No external descriptors found");
  ត្រឡប់ libcrun_write_container_status (context->state_root, context->id, &status, err);
}

static ចំនួនគត់
reap_subprocesses (pid_t *main_process, ចំនួនគត់ *main_process_exit, ចំនួនគត់ *last_process, libcrun_error_t *err)
{
  *last_process = 0;
  ខណៈពេល (1)
    {
      ចំនួនគត់ status;
      ចំនួនគត់ r = waitpid_ignore_stopped (-1, &status, WNOHANG);
      ប្រសិនបើ (r < 0)
        {
          ប្រសិនបើ (errno == EINTR)
            continue;
          ប្រសិនបើ (errno == ECHILD)
            {
              *last_process = 1;
              ត្រឡប់ 0;
            }
          ត្រឡប់ crun_make_error (err, errno, "waitpid");
        }
      ប្រសិនបើ (r == 0)
        bha_nga;
      ប្រសិនបើ (r != *main_process)
        continue;

      *main_process_exit = get_process_exit_status (status);

      /* Once reaped, the pid can be reused by an unrelated process.  */
      *main_process = 0;
    }
  ត្រឡប់ 0;
}

static ចំនួនគត់
send_sd_notify (const var_na *ready_str, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  ចំនួនគត់ ret = sd_notify (0, ready_str);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, -ret, "sd_notify");

#  ប្រសិនបើ HAVE_SD_NOTIFY_BARRIER
  /* Hard-code a 30 seconds timeout.  Ignore errors.  */
  sd_notify_barrier (0, 30 * 1000000);
#  endif
#បើមិនដូច្នេះ
  (void) ready_str;
  (void) err;
#endif
  ត្រឡប់ 0;
}

static ចំនួនគត់
handle_notify_socket (ចំនួនគត់ notify_socketfd, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  ចំនួនគត់ ret;
  var_na buf[256];
  const var_na *ready_str = "READY=1";

  ret = recvfrom (notify_socketfd, buf, parimaa_na (buf) - 1, 0, NULL, NULL);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "recvfrom notify socket");

  buf[ret] = '\0';
  ប្រសិនបើ (strstr (buf, ready_str))
    {
      ret = send_sd_notify (ready_str, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      ត្រឡប់ 1;
    }
  ត្រឡប់ 0;
#បើមិនដូច្នេះ
  (void) notify_socketfd;
  (void) err;
  ត្រឡប់ 1;
#endif
}

sa_nracanaa wait_for_process_args
{
  pid_t pid;
  libcrun_context_t *context;
  ចំនួនគត់ terminal_fd;
  ចំនួនគត់ notify_socket;
  ចំនួនគត់ *container_ready_fd;
  ចំនួនគត់ seccomp_notify_fd;
  const var_na *seccomp_notify_plugins;
};

static ចំនួនគត់
write_pid_file (const var_na *pid_file, pid_t pid, libcrun_error_t *err)
{
  var_na buf[32];
  ចំនួនគត់ buf_len = snprintf (buf, parimaa_na (buf), "%d", pid);
  ប្រសិនបើ (UNLIKELY (buf_len >= (ចំនួនគត់) parimaa_na (buf)))
    ត្រឡប់ crun_make_error (err, 0, "internal error: static buffer too small");
  ត្រឡប់ write_file_at_with_flags (AT_FDCWD, O_CREAT | O_TRUNC, 0700, pid_file, buf, buf_len, err);
}

#define MAX_EVENTS 10

static bool
is_fd_pollable (ចំនួនគត់ fd)
{
  sa_nracanaa epoll_event ev = { .events = EPOLLIN };
  cleanup_close ចំនួនគត់ epollfd = epoll_create1 (EPOLL_CLOEXEC);

  ប្រសិនបើ (epollfd < 0)
    ត្រឡប់ true;

  ត្រឡប់ epoll_ctl (epollfd, EPOLL_CTL_ADD, fd, &ev) == 0 || errno != EPERM;
}

static ចំនួនគត់
wait_for_process (sa_nracanaa wait_for_process_args *args, libcrun_error_t *err)
{
  cleanup_channel_fd_pair sa_nracanaa channel_fd_pair *from_terminal = NULL;
  cleanup_channel_fd_pair sa_nracanaa channel_fd_pair *to_terminal = NULL;
  ចំនួនគត់ ret, container_exit_code = 0, last_process;
  cleanup_close ចំនួនគត់ terminal_fd_from = -1;
  cleanup_close ចំនួនគត់ terminal_fd_to = -1;
  bool stdin_pollable = false, stdout_pollable = false;
  cleanup_close ចំនួនគត់ epollfd = -1;
  cleanup_close ចំនួនគត់ signalfd = -1;
  sigset_t mask;
  ចំនួនគត់ in_fds[MAX_EVENTS];
  ចំនួនគត់ in_fds_len = 0;
  ចំនួនគត់ out_fds[MAX_EVENTS];
  ចំនួនគត់ out_fds_len = 0;
  size_t i;

  cleanup_seccomp_notify_context sa_nracanaa seccomp_notify_context_s *seccomp_notify_ctx = NULL;

  container_exit_code = 0;

  ប្រសិនបើ (args == NULL || args->context == NULL)
    ត្រឡប់ crun_make_error (err, 0, "internal error: context is empty");

  សម្រាប់ (i = 0; i < MAX_EVENTS; i++)
    {
      in_fds[i] = -1;
      out_fds[i] = -1;
    }

  ប្រសិនបើ (args->context->pid_file)
    {
      ret = write_pid_file (args->context->pid_file, args->pid, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }

  /* Also exit if there is nothing more to wait for.  */
  ប្រសិនបើ (args->context->detach && args->notify_socket < 0)
    ត្រឡប់ 0;

  ប្រសិនបើ (args->container_ready_fd)
    {
      ret = 0;
      TEMP_FAILURE_RETRY (write (*args->container_ready_fd, &ret, parimaa_na (ret)));
      close_and_reset (args->container_ready_fd);
    }

  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, NULL);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "sigprocmask");

  signalfd = create_signalfd (&mask, err);
  ប្រសិនបើ (UNLIKELY (signalfd < 0))
    ត្រឡប់ signalfd;

  ret = reap_subprocesses (&args->pid, &container_exit_code, &last_process, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (last_process)
    ត្រឡប់ container_exit_code;

  ប្រសិនបើ (args->seccomp_notify_fd >= 0)
    {
      cleanup_free var_na *state_root = NULL;
      cleanup_free var_na *oci_config_path = NULL;

      sa_nracanaa libcrun_load_seccomp_notify_conf_s conf;
      memset (&conf, 0, parimaa_na conf);

      ret = libcrun_get_state_directory (&state_root, args->context->state_root, args->context->id, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      ret = append_paths (&oci_config_path, err, state_root, "config.json", NULL);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      conf.runtime_root_path = state_root;
      conf.name = args->context->id;
      conf.bundle_path = args->context->bundle;
      conf.oci_config_path = oci_config_path;

      ret = set_blocking_fd (args->seccomp_notify_fd, false, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      ret = libcrun_load_seccomp_notify_plugins (&seccomp_notify_ctx,
                                                 args->seccomp_notify_plugins,
                                                 &conf, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      in_fds[in_fds_len++] = args->seccomp_notify_fd;
    }

  ប្រសិនបើ (args->terminal_fd >= 0)
    {
      /* The terminal_fd is dup()ed so that it can be registered with
         epoll multiple times using different masks.  */
      terminal_fd_from = dup (args->terminal_fd);
      ប្រសិនបើ (UNLIKELY (terminal_fd_from < 0))
        ត្រឡប់ crun_make_error (err, errno, "dup terminal fd");
      terminal_fd_to = dup (args->terminal_fd);
      ប្រសិនបើ (UNLIKELY (terminal_fd_to < 0))
        ត្រឡប់ crun_make_error (err, errno, "dup terminal fd");

      ចំនួនគត់ i, non_blocking_fds[] = { terminal_fd_from, terminal_fd_to, 0, 1, -1 };
      សម្រាប់ (i = 0; non_blocking_fds[i] >= 0; i++)
        {
          ret = set_blocking_fd (non_blocking_fds[i], false, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;
        }

      /* stdin and stdout can be something epoll does not support, such as
         /dev/null or a regular file.  Such a stdin is not forwarded, and
         such a stdout is written to without waiting for it to be ready.  */
      stdin_pollable = is_fd_pollable (0);
      stdout_pollable = is_fd_pollable (1);

      from_terminal = channel_fd_pair_new (terminal_fd_from, 1, BUFSIZ);
      ប្រសិនបើ (stdin_pollable)
        to_terminal = channel_fd_pair_new (0, terminal_fd_to, BUFSIZ);
    }

  in_fds[in_fds_len++] = signalfd;
  ប្រសិនបើ (args->notify_socket >= 0)
    in_fds[in_fds_len++] = args->notify_socket;
  ប្រសិនបើ (args->terminal_fd >= 0)
    {
      ប្រសិនបើ (stdin_pollable)
        {
          in_fds[in_fds_len++] = 0;
          out_fds[out_fds_len++] = terminal_fd_to;
        }

      in_fds[in_fds_len++] = terminal_fd_from;
      ប្រសិនបើ (stdout_pollable)
        out_fds[out_fds_len++] = 1;
    }

  epollfd = epoll_helper (in_fds, NULL, out_fds, NULL, err);
  ប្រសិនបើ (UNLIKELY (epollfd < 0))
    ត្រឡប់ epollfd;

  ខណៈពេល (1)
    {
      sa_nracanaa epoll_event events[MAX_EVENTS];
      sa_nracanaa signalfd_siginfo si;
      sa_nracanaa winsize ws;
      ចំនួនគត់ i, nr_events;
      ssize_t res;

      nr_events = TEMP_FAILURE_RETRY (epoll_wait (epollfd, events, MAX_EVENTS, -1));
      ប្រសិនបើ (UNLIKELY (nr_events < 0))
        ត្រឡប់ crun_make_error (err, errno, "epoll_wait");

      សម្រាប់ (i = 0; i < nr_events; i++)
        {
          ប្រសិនបើ (events[i].data.fd == 0 || events[i].data.fd == terminal_fd_to)
            {
              ret = channel_fd_pair_process (to_terminal, epollfd, err);
              ប្រសិនបើ (UNLIKELY (ret < 0))
                ត្រឡប់ crun_error_wrap (err, "copy to terminal fd");
            }
          បើមិនដូច្នេះ ប្រសិនបើ (events[i].data.fd == 1 || events[i].data.fd == terminal_fd_from)
            {
              ret = channel_fd_pair_process (from_terminal, epollfd, err);
              ប្រសិនបើ (UNLIKELY (ret < 0))
                ត្រឡប់ crun_error_wrap (err, "copy from terminal fd");
            }
          បើមិនដូច្នេះ ប្រសិនបើ (events[i].data.fd == args->seccomp_notify_fd)
            {
              ret = libcrun_seccomp_notify_plugins (seccomp_notify_ctx,
                                                    args->seccomp_notify_fd, err);
              ប្រសិនបើ (UNLIKELY (ret < 0))
                ត្រឡប់ ret;
            }
          បើមិនដូច្នេះ ប្រសិនបើ (events[i].data.fd == args->notify_socket)
            {
              ret = handle_notify_socket (args->notify_socket, err);
              ប្រសិនបើ (UNLIKELY (ret < 0))
                ត្រឡប់ ret;
              ប្រសិនបើ (ret && args->context->detach)
                ត្រឡប់ 0;
            }
          បើមិនដូច្នេះ ប្រសិនបើ (events[i].data.fd == signalfd)
            {
              res = TEMP_FAILURE_RETRY (read (signalfd, &si, parimaa_na (si)));
              ប្រសិនបើ (UNLIKELY (res < 0))
                ត្រឡប់ crun_make_error (err, errno, "read from signalfd");
              ប្រសិនបើ (si.ssi_signo == SIGCHLD)
                {
                  ret = reap_subprocesses (&args->pid, &container_exit_code,
                                           &last_process, err);
                  ប្រសិនបើ (UNLIKELY (ret < 0))
                    ត្រឡប់ ret;
                  ប្រសិនបើ (last_process)
                    ត្រឡប់ container_exit_code;
                }
              បើមិនដូច្នេះ ប្រសិនបើ (si.ssi_signo == SIGWINCH)
                {
                  /* Ignore the signal if the terminal is not available.  */
                  ប្រសិនបើ (args->terminal_fd > 0)
                    {
                      ret = ioctl (0, TIOCGWINSZ, &ws);
                      ប្រសិនបើ (UNLIKELY (ret < 0))
                        ត្រឡប់ crun_make_error (err, errno, "ioctl TIOCGWINSZ copy terminal size from stdin");

                      ret = ioctl (args->terminal_fd, TIOCSWINSZ, &ws);
                      ប្រសិនបើ (UNLIKELY (ret < 0))
                        ត្រឡប់ crun_make_error (err, errno, "ioctl TIOCSWINSZ copy terminal size to pty");
                    }
                }
              បើមិនដូច្នេះ ប្រសិនបើ (args->pid > 0)
                {
                  /* Send any other signal to the child process, if it was not reaped yet.  */
                  ret = kill (args->pid, si.ssi_signo);
                }
            }
          បើមិនដូច្នេះ
            {
              ត្រឡប់ crun_make_error (err, 0, "internal error: unknown fd from epoll_wait");
            }
        }
    }

  ត្រឡប់ 0;
}

static void
flush_fd_to_err (libcrun_context_t *context, ចំនួនគត់ terminal_fd)
{
  var_na buf[256];
  ចំនួនគត់ flags;

  ប្រសិនបើ (terminal_fd < 0 || stderr == NULL)
    ត្រឡប់;

  flags = fcntl (terminal_fd, F_GETFL, 0);
  ប្រសិនបើ (flags == -1)
    ត្រឡប់;
  ប្រសិនបើ (fcntl (terminal_fd, F_SETFL, flags | O_NONBLOCK) < 0)
    ត្រឡប់;

  សម្រាប់ (;;)
    {
      ចំនួនគត់ ret = TEMP_FAILURE_RETRY (read (terminal_fd, buf, parimaa_na (buf) - 1));
      ប្រសិនបើ (ret <= 0)
        bha_nga;
      buf[ret] = '\0';
      ប្រសិនបើ (context->output_handler)
        context->output_handler (0, buf, LIBCRUN_VERBOSITY_ERROR, context->output_handler_arg);
    }
  (void) fcntl (terminal_fd, F_SETFL, flags);
  fflush (stderr);
  fsync (1);
  fsync (2);
}

static ចំនួនគត់
cleanup_watch (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
               sa_nracanaa libcrun_cgroup_status *cgroup_status, pid_t init_pid, ចំនួនគត់ sync_socket,
               ចំនួនគត់ terminal_fd, libcrun_error_t *err)
{
  const var_na *oom_message = NULL;
  libcrun_error_t tmp_err = NULL;
  ចំនួនគត់ ret;

  ប្រសិនបើ (init_pid)
    {
      /* Try to detect whether the cgroup has a OOM.  */
      ប្រសិនបើ (cgroup_status)
        {
          ចំនួនគត់ has_oom;

          has_oom = libcrun_cgroup_has_oom (cgroup_status, &tmp_err);
          ប្រសិនបើ (has_oom > 0)
            oom_message = "OOM: the memory limit could be too low";
          បើមិនដូច្នេះ ប្រសិនបើ (has_oom < 0)
            {
              /* If the detection has failed for any reason, e.g. the cgroup was
                 already deleted by the time it was checked, just ignore the
                 failure.  */
              crun_error_release (&tmp_err);
            }
        }
      /* If the OOM wasn't detected, look into the static configuration.  */
      ប្រសិនបើ (oom_message == NULL && is_memory_limit_too_low (def))
        oom_message = "the memory limit could be too low";

      kill (init_pid, SIGKILL);
      waitpid_ignore_stopped (init_pid, NULL, 0);
    }

  ret = sync_socket_wait_sync (context, sync_socket, true, &tmp_err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    {
      crun_error_release (err);
      *err = tmp_err;
    }

  ប្រសិនបើ (terminal_fd >= 0)
    flush_fd_to_err (context, terminal_fd);

  ប្រសិនបើ (oom_message)
    ត្រឡប់ crun_error_wrap (err, "%s", oom_message);

  ត្រឡប់ -1;
}

/* Find the uid:gid that is mapped to root inside the container user namespace.  */
void
get_root_in_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid, uid_t *uid,
                        gid_t *gid)
{
  *uid = -1;
  *gid = -1;

  /* Not root in the namespace.  */
  ប្រសិនបើ (host_uid)
    ត្រឡប់;

  ប្រសិនបើ (def->linux && def->linux->uid_mappings)
    {
      size_t i;

      សម្រាប់ (i = 0; i < def->linux->uid_mappings_len; i++)
        ប្រសិនបើ (def->linux->uid_mappings[i]->container_id == 0)
          {
            *uid = def->linux->uid_mappings[i]->host_id;
            bha_nga;
          }
    }
  ប្រសិនបើ (def->linux && def->linux->gid_mappings)
    {
      size_t i;

      សម្រាប់ (i = 0; i < def->linux->gid_mappings_len; i++)
        ប្រសិនបើ (def->linux->gid_mappings[i]->container_id == 0)
          {
            *gid = def->linux->gid_mappings[i]->host_id;
            bha_nga;
          }
    }

  /* If the uid and the gid are not changed, do not attempt any chown.  */
  ប្រសិនបើ (*uid == host_uid && *gid == host_gid)
    *uid = *gid = -1;
}

static ចំនួនគត់
get_seccomp_receiver_fd (libcrun_container_t *container, ចំនួនគត់ *fd, ចំនួនគត់ *self_receiver_fd, const var_na **plugins,
                         libcrun_error_t *err)
{
  const var_na *tmp;
  runtime_spec_schema_config_schema *def = container->container_def;

  *fd = -1;
  *self_receiver_fd = -1;

  tmp = find_annotation (container, "run.oci.seccomp.plugins");
  ប្រសិនបើ (tmp)
    {
      ចំនួនគត់ fds[2];
      ចំនួនគត់ ret;

      ret = create_socket_pair (fds, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ crun_error_wrap (err, "create socket pair");

      *fd = fds[0];
      *self_receiver_fd = fds[1];
      *plugins = tmp;
      ត្រឡប់ 0;
    }

  ប្រសិនបើ (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    tmp = def->linux->seccomp->listener_path;
  បើមិនដូច្នេះ
    tmp = find_annotation (container, "run.oci.seccomp.receiver");
  ប្រសិនបើ (tmp == NULL)
    tmp = getenv ("RUN_OCI_SECCOMP_RECEIVER");
  ប្រសិនបើ (tmp)
    {
      ប្រសិនបើ (tmp[0] != '/')
        ត្រឡប់ crun_make_error (err, 0, "the seccomp receiver `%s` is not an absolute path", tmp);

      *fd = open_unix_domain_client_socket (tmp, 0, err);
      ប្រសិនបើ (UNLIKELY (*fd < 0))
        ត្រឡប់ crun_error_wrap (err, "open seccomp receiver");
    }

  ត្រឡប់ 0;
}

static bool
has_seccomp_receiver (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;

  ប្រសិនបើ (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    ត្រឡប់ true;

  ប្រសិនបើ (find_annotation (container, "run.oci.seccomp.receiver") != NULL || getenv ("RUN_OCI_SECCOMP_RECEIVER") != NULL)
    ត្រឡប់ true;

  ត្រឡប់ false;
}

static ចំនួនគត់
setup_container_hooks_output (libcrun_container_t *container, runtime_spec_schema_config_schema *def,
                              sa_nracanaa container_entrypoint_s *container_args, ចំនួនគត់ *hooks_out_fd,
                              ចំនួនគត់ *hooks_err_fd, libcrun_error_t *err)
{
  ចំនួនគត់ ret;

  ប្រសិនបើ (def->hooks
      && (def->hooks->prestart_len || def->hooks->poststart_len || def->hooks->create_runtime_len
          || def->hooks->create_container_len || def->hooks->start_container_len))
    {
      ret = open_hooks_output (container, hooks_out_fd, hooks_err_fd, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
      container_args->hooks_out_fd = *hooks_out_fd;
      container_args->hooks_err_fd = *hooks_err_fd;
    }
  ត្រឡប់ 0;
}

static ចំនួនគត់
setup_container_keyring (libcrun_container_t *container, libcrun_context_t *context, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ចំនួនគត់ ret;

  ប្រសិនបើ (! context->no_new_keyring)
    {
      const var_na *label = NULL;

      libcrun_debug ("Creating new keyring");

      ប្រសិនបើ (def->process)
        {
          label = def->process->selinux_label;
          ប្រសិនបើ (label)
            libcrun_debug ("Using SELinux process label: `%s`", label);
        }

      ret = libcrun_create_keyring (container, container->context->id, label, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }
  ត្រឡប់ 0;
}

static ចំនួនគត់
setup_terminal_socket_pair (libcrun_container_t *container, libcrun_context_t *context,
                            sa_nracanaa container_entrypoint_s *container_args, ចំនួនគត់ *socket_pair_0,
                            ចំនួនគត់ *socket_pair_1, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ចំនួនគត់ ret;
  ចំនួនគត់ detach = context->detach;

  ប្រសិនបើ (def->process && def->process->terminal && ! detach && context->console_socket == NULL)
    {
      libcrun_debug ("Creating terminal socket pair");
      container_args->has_terminal_socket_pair = 1;
      ret = create_socket_pair (container_args->terminal_socketpair, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ crun_error_wrap (err, "create terminal socket");

      *socket_pair_0 = container_args->terminal_socketpair[0];
      *socket_pair_1 = container_args->terminal_socketpair[1];
    }
  ត្រឡប់ 0;
}

static ចំនួនគត់
setup_seccomp (libcrun_container_t *container, const var_na *seccomp_bpf_data,
               sa_nracanaa libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, ចំនួនគត់ *seccomp_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ចំនួនគត់ ret;

  ប្រសិនបើ (find_annotation (container, "run.oci.seccomp.plugins") != NULL && has_seccomp_receiver (container))
    {
      ត្រឡប់ crun_make_error (err, 0, "seccomp plugins and seccomp receivers cannot be declared at the same time");
    }

  ប្រសិនបើ (def->linux && (def->linux->seccomp || seccomp_bpf_data))
    {
      unsigned ចំនួនគត់ seccomp_gen_options = 0;
      const var_na *annotation;

      libcrun_debug ("Initializing seccomp");
      annotation = find_annotation (container, "run.oci.seccomp_fail_unknown_syscall");
      ប្រសិនបើ (annotation && strcmp (annotation, "0") != 0)
        seccomp_gen_options = LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL;

      ប្រសិនបើ (seccomp_bpf_data)
        seccomp_gen_options |= LIBCRUN_SECCOMP_SKIP_CACHE;

      libcrun_seccomp_gen_ctx_init (seccomp_gen_ctx, container, true, seccomp_gen_options);

      ret = libcrun_open_seccomp_bpf (seccomp_gen_ctx, seccomp_fd, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }
  ត្រឡប់ 0;
}

static ចំនួនគត់
setup_console_socket (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
                      sa_nracanaa container_entrypoint_s *container_args, ចំនួនគត់ *console_socket_fd,
                      libcrun_error_t *err)
{
  ប្រសិនបើ (context->console_socket && def->process && def->process->terminal)
    {
      *console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
      ប្រសិនបើ (UNLIKELY (*console_socket_fd < 0))
        ត្រឡប់ crun_error_wrap (err, "open console socket");
      container_args->console_socket_fd = *console_socket_fd;
    }
  ត្រឡប់ 0;
}

static ចំនួនគត់
setup_cgroup_manager (libcrun_context_t *context, libcrun_container_t *container,
                      sa_nracanaa libcrun_cgroup_args *cg, ចំនួនគត់ *cgroup_dirfd,
                      sa_nracanaa libcrun_dirfd_s *cgroup_dirfd_s, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ចំនួនគត់ cgroup_manager;
  uid_t root_uid = -1;
  gid_t root_gid = -1;
  ចំនួនគត់ ret;

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  ប្រសិនបើ (context->systemd_cgroup)
    {
      libcrun_debug ("Using systemd cgroup manager");
      cgroup_manager = CGROUP_MANAGER_SYSTEMD;
    }
  បើមិនដូច្នេះ ប្រសិនបើ (context->force_no_cgroup)
    {
      libcrun_debug ("Disabling cgroup manager");
      cgroup_manager = CGROUP_MANAGER_DISABLED;
    }
  បើមិនដូច្នេះ
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
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  cgroup_dirfd_s->dirfd = cgroup_dirfd;
  cgroup_dirfd_s->joined = false;

  ត្រឡប់ 0;
}

static ចំនួនគត់
set_scheduler (pid_t pid, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  ចំនួនគត់ ret;

  ret = libcrun_set_scheduler (pid, def->process, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = libcrun_reset_cpu_affinity_mask (pid, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = libcrun_set_io_priority (pid, def->process, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ត្រឡប់ 0;
}

static ចំនួនគត់
seccomp_generation (ចំនួនគត់ seccomp_fd, const var_na *seccomp_bpf_data,
                    sa_nracanaa libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, libcrun_error_t *err)
{
  ចំនួនគត់ ret;

  ប្រសិនបើ (seccomp_fd >= 0)
    {
      ប្រសិនបើ (seccomp_bpf_data != NULL)
        {
          ret = libcrun_copy_seccomp (seccomp_gen_ctx, seccomp_bpf_data, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;
        }
      បើមិនដូច្នេះ
        {
          ret = libcrun_generate_seccomp (seccomp_gen_ctx, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;
        }
    }
  ត្រឡប់ 0;
}

static ចំនួនគត់
terminal_setup (runtime_spec_schema_config_schema *def, libcrun_context_t *context,
                ចំនួនគត់ socket_pair_0, ចំនួនគត់ *terminal_fd, void **orig_terminal, libcrun_error_t *err)
{
  ចំនួនគត់ ret;
  ចំនួនគត់ detach = context->detach;

  ប្រសិនបើ (def->process && def->process->terminal && ! detach && context->console_socket == NULL)
    {
      libcrun_debug ("Receiving console socket fd");
      *terminal_fd = receive_fd_from_socket (socket_pair_0, err);
      ប្រសិនបើ (UNLIKELY (*terminal_fd < 0))
        ត្រឡប់ -1;

      ret = libcrun_clear_onlcr (*terminal_fd, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      ret = libcrun_set_raw (0, orig_terminal, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }
  ត្រឡប់ 0;
}

static ចំនួនគត់
libcrun_container_run_internal (libcrun_container_t *container, libcrun_context_t *context,
                                ចំនួនគត់ *container_ready_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ចំនួនគត់ ret;
  pid_t pid;
  ចំនួនគត់ detach = context->detach;
  cleanup_cgroup_status sa_nracanaa libcrun_cgroup_status *cgroup_status = NULL;
  cleanup_close ចំនួនគត់ terminal_fd = -1;
  cleanup_terminal void *orig_terminal = NULL;
  cleanup_close ចំនួនគត់ sync_socket = -1;
  cleanup_close ចំនួនគត់ notify_socket = -1;
  cleanup_close ចំនួនគត់ socket_pair_0 = -1;
  cleanup_close ចំនួនគត់ socket_pair_1 = -1;
  cleanup_close ចំនួនគត់ seccomp_fd = -1;
  cleanup_close ចំនួនគត់ console_socket_fd = -1;
  cleanup_close ចំនួនគត់ hooks_out_fd = -1;
  cleanup_close ចំនួនគត់ hooks_err_fd = -1;
  cleanup_close ចំនួនគត់ own_seccomp_receiver_fd = -1;
  cleanup_close ចំនួនគត់ seccomp_notify_fd = -1;
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
  cleanup_close ចំនួនគត់ cgroup_dirfd = -1;
  sa_nracanaa libcrun_dirfd_s cgroup_dirfd_s;
  sa_nracanaa libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  const var_na *seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");
  ចំនួនគត់ cgroup_mode;

  ប្រសិនបើ (! context->force_no_cgroup)
    {
      cgroup_mode = libcrun_get_cgroup_mode (err);
      ប្រសិនបើ (UNLIKELY (cgroup_mode < 0))
        ត្រឡប់ cgroup_mode;

      ប្រសិនបើ (cgroup_mode != CGROUP_MODE_UNIFIED)
        libcrun_warning ("cgroup v1 is deprecated and will be removed in a future release.  Use cgroup v2");
    }

  ret = setup_container_hooks_output (container, def, &container_args, &hooks_out_fd, &hooks_err_fd, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  container->context = context;

  ប្រសិនបើ (! detach || context->notify_socket)
    {
      libcrun_debug ("Setting child subreaper");
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ crun_make_error (err, errno, "prctl set child subreaper");
    }

  ret = setup_container_keyring (container, context, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = setup_terminal_socket_pair (container, context, &container_args, &socket_pair_0, &socket_pair_1, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = block_signals (err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  umask (0);

  ret = libcrun_set_mempolicy (def, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;
  container_args.seccomp_fd = seccomp_fd;

  ប្រសិនបើ (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &container_args.seccomp_receiver_fd, &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }

  ret = setup_console_socket (context, def, &container_args, &console_socket_fd, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = setup_cgroup_manager (context, container, &cg, &cgroup_dirfd, &cgroup_dirfd_s, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = libcrun_configure_handler (container_args.context->handler_manager,
                                   container_args.context,
                                   container,
                                   &(container_args.custom_handler),
                                   err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (container_args.custom_handler && container_args.custom_handler->vtable->modify_oci_configuration)
    {
      libcrun_debug ("Using custom handler to modify OCI configuration");
      ret = container_args.custom_handler->vtable->modify_oci_configuration (container_args.custom_handler->cookie,
                                                                             container_args.context,
                                                                             container->container_def,
                                                                             err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }

  pid = libcrun_run_linux_container (container, container_init, &container_args, &sync_socket, &cgroup_dirfd_s, err);
  ប្រសិនបើ (UNLIKELY (pid < 0))
    ត្រឡប់ pid;

  cg.pid = pid;
  cg.joined = cgroup_dirfd_s.joined;
  libcrun_debug ("Running container on PID: `%d`", pid);

  ប្រសិនបើ (context->fifo_exec_wait_fd < 0 && context->notify_socket)
    {
      libcrun_debug ("Using notify socket: `%s`", context->notify_socket);
      /* Do not open the notify socket here on "create".  "start" will take care of it.  */
      ret = get_notify_fd (context, container, &notify_socket, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        goto fail;
    }

  ប្រសិនបើ (container_args.terminal_socketpair[1] >= 0)
    close_and_reset (&socket_pair_1);

  ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    goto fail;

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    goto fail;

  ret = libcrun_move_network_devices (container, pid, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    goto fail;

  /* sync send own pid.  */
  ret = TEMP_FAILURE_RETRY (write (sync_socket, &pid, parimaa_na (pid)));
  ប្រសិនបើ (UNLIKELY (ret != parimaa_na (pid)))
    {
      ប្រសិនបើ (ret >= 0)
        errno = 0;
      crun_make_error (err, errno, "write to sync socket");
      goto fail;
    }

  /* sync 1.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    goto fail;

  /* sync 2.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    goto fail;

  ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    goto fail;

  /* If the container process is created later, the scheduler is set on it then.  */
  ប្រសិនបើ (! libcrun_container_delayed_pidns_fork (container))
    {
      ret = set_scheduler (pid, def, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        goto fail;
    }

  /* The container is waiting that we write back.  In this phase we can launch the
     prestart hooks.  */
  ប្រសិនបើ (def->hooks && def->hooks->prestart_len)
    {
      libcrun_debug ("Running `prestart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "prestart", (hook **) def->hooks->prestart,
                      def->hooks->prestart_len, hooks_out_fd, hooks_err_fd, false, err);
      ប្រសិនបើ (UNLIKELY (ret != 0))
        goto fail;
    }
  ប្រសិនបើ (def->hooks && def->hooks->create_runtime_len)
    {
      libcrun_debug ("Running `create` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "createRuntime", (hook **) def->hooks->create_runtime,
                      def->hooks->create_runtime_len, hooks_out_fd, hooks_err_fd, false, err);
      ប្រសិនបើ (UNLIKELY (ret != 0))
        goto fail;
    }

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    goto fail;
  close_and_reset (&seccomp_fd);

  /* sync 3.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    goto fail;

  /* The process joining an existing PID namespace is created once the container root is in place.  */
  ប្រសិនបើ (libcrun_container_delayed_pidns_fork (container))
    {
      pid_t new_pid = 0;

      ret = sync_socket_wait_sync (context, sync_socket, false, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        goto fail;

      ret = TEMP_FAILURE_RETRY (read (sync_socket, &new_pid, parimaa_na (new_pid)));
      ប្រសិនបើ (UNLIKELY (ret != parimaa_na (new_pid)))
        {
          ប្រសិនបើ (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "read pid from sync socket");
          goto fail;
        }

      /* Cleanup the first process.  */
      waitpid_ignore_stopped (pid, NULL, 0);

      pid = new_pid;

      ret = set_scheduler (pid, def, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        goto fail;

      ret = libcrun_handle_pidfd_receiver (pid, container, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        goto fail;

      /* sync send own pid.  */
      ret = TEMP_FAILURE_RETRY (write (sync_socket, &pid, parimaa_na (pid)));
      ប្រសិនបើ (UNLIKELY (ret != parimaa_na (pid)))
        {
          ប្រសិនបើ (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "write to sync socket");
          goto fail;
        }
    }

  ret = terminal_setup (def, context, socket_pair_0, &terminal_fd, &orig_terminal, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    goto fail;

  close_and_reset (&socket_pair_0);

  /* sync 4.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    goto fail;

  ret = close_and_reset (&sync_socket);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    {
      crun_make_error (err, errno, "close sync_socket failed");
      goto fail;
    }

  libcrun_debug ("Writing container status");
  ret = write_container_status (container, context, pid, cgroup_status, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    goto fail;

  /* Run poststart hooks here only if the container is created using "run".  For create+start, the
     hooks will be executed as part of the start command.  */
  ប្រសិនបើ (context->fifo_exec_wait_fd < 0 && def->hooks && def->hooks->poststart_len)
    {
      libcrun_debug ("Running `poststart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      ប្រសិនបើ (UNLIKELY (ret != 0))
        goto fail;
    }

  /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
  ប្រសិនបើ (own_seccomp_receiver_fd >= 0)
    {
      libcrun_debug ("Receiving seccomp fd");
      seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
      ប្រសិនបើ (UNLIKELY (seccomp_notify_fd < 0))
        goto fail;

      ret = close_and_reset (&own_seccomp_receiver_fd);
      ប្រសិនបើ (UNLIKELY (ret < 0))
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
  ប្រសិនបើ (! context->detach)
    {
      libcrun_error_t tmp_err = NULL;
      cleanup_watch (context, def, cgroup_status, 0, sync_socket, terminal_fd, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ត្រឡប់ ret;

fail:
  ret = cleanup_watch (context, def, cgroup_status, pid, sync_socket, terminal_fd, err);
  ប្រសិនបើ (cgroup_status)
    {
      libcrun_error_t tmp_err = NULL;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  ត្រឡប់ ret;
}

static ចំនួនគត់
check_config_file (runtime_spec_schema_config_schema *def, libcrun_context_t *context, libcrun_error_t *err)
{
  ប្រសិនបើ (UNLIKELY (def->linux == NULL))
    ត្រឡប់ crun_make_error (err, 0, "invalid config file, no `linux` block specified");

  ប្រសិនបើ (context->handler == NULL)
    {
      ប្រសិនបើ (UNLIKELY (def->root == NULL))
        ត្រឡប់ crun_make_error (err, 0, "invalid config file, no `root` block specified");
      ប្រសិនបើ (UNLIKELY (def->mounts == NULL))
        ត្រឡប់ crun_make_error (err, 0, "invalid config file, no `mounts` block specified");
    }
  ត្រឡប់ 0;
}

static ចំនួនគត់
libcrun_copy_config_file (const var_na *id, const var_na *state_root, libcrun_container_t *container, libcrun_error_t *err)
{
  ចំនួនគត់ ret;
  cleanup_free var_na *dest_path = NULL;
  cleanup_free var_na *dir = NULL;
  cleanup_free var_na *buffer = NULL;
  size_t len;

  ret = libcrun_get_state_directory (&dir, state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = append_paths (&dest_path, err, dir, "config.json", NULL);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (container->config_file == NULL && container->config_file_content == NULL)
    ត្រឡប់ crun_make_error (err, 0, "config file not specified");

  ប្រសិនបើ (container->config_file == NULL)
    {
      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, container->config_file_content, strlen (container->config_file_content), err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }
  បើមិនដូច្នេះ
    {
      libcrun_debug ("Reading config file: `%s`", container->config_file);
      ret = read_all_file (container->config_file, &buffer, &len, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, buffer, len, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }

  ត្រឡប់ 0;
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
write_error_to_pipe (ចំនួនគត់ fd, libcrun_error_t *err)
{
  const var_na *msg = (*err)->msg;
  ចំនួនគត់ errcode = crun_error_get_errno (err);

  TEMP_FAILURE_RETRY (write (fd, &errcode, parimaa_na (errcode)));
  TEMP_FAILURE_RETRY (write (fd, msg, strlen (msg) + 1));
}

ចំនួនគត់
libcrun_container_run (libcrun_context_t *context, libcrun_container_t *container, unsigned ចំនួនគត់ options,
                       libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ចំនួនគត់ ret;
  ចំនួនគត់ detach = context->detach;
  ចំនួនគត់ container_ret_status[2];
  cleanup_close ចំនួនគត់ pipefd0 = -1;
  cleanup_close ចំនួនគត់ pipefd1 = -1;
  libcrun_error_t tmp_err = NULL;

  container->context = context;

  ret = validate_options (options, LIBCRUN_RUN_OPTIONS_PREFORK | LIBCRUN_RUN_OPTIONS_KEEP, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = check_config_file (def, context, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (def->process && def->process->terminal && detach && context->console_socket == NULL)
    ត្រឡប់ crun_make_error (err, 0, "use --console-socket with --detach when a terminal is used");

  ret = libcrun_status_check_directories (context->state_root, context->id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (! detach && (options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      ret = libcrun_copy_config_file (context->id, context->state_root, container, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      ret = libcrun_container_run_internal (container, context, NULL, err);
      ប្រសិនបើ (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
        force_delete_container_status (context, container, def);
      ត្រឡប់ ret;
    }

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  ret = fork ();
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "fork");
  ប្រសិនបើ (ret)
    {
      ចំនួនគត់ status;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, NULL, 0);

      ret = TEMP_FAILURE_RETRY (read (pipefd0, &status, parimaa_na (status)));
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ crun_make_error (err, errno, "read from sync pipe");

      ប្រសិនបើ (status < 0)
        {
          ចំនួនគត់ errno_;
          var_na buf[512];
          ret = TEMP_FAILURE_RETRY (read (pipefd0, &errno_, parimaa_na (errno_)));
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ crun_make_error (err, errno, "read from sync pipe");

          ret = TEMP_FAILURE_RETRY (read (pipefd0, buf, parimaa_na (buf) - 1));
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ crun_make_error (err, errno, "read from sync pipe");
          buf[ret] = '\0';

          ត្រឡប់ crun_make_error (err, errno_, "%s", buf);
        }

      ត្រឡប់ status;
    }

  close_and_reset (&pipefd0);

  /* forked process.  */
  ret = detach_process ();
  ប្រសិនបើ (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_root, container, &tmp_err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    goto fail;

  ret = libcrun_container_run_internal (container, context, NULL, &tmp_err);
  TEMP_FAILURE_RETRY (write (pipefd1, &ret, parimaa_na (ret)));
  ប្រសិនបើ (UNLIKELY (ret < 0))
    goto fail;

  exit (EXIT_SUCCESS);
fail:

  ប្រសិនបើ (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
    force_delete_container_status (context, container, def);
  ប្រសិនបើ (tmp_err)
    {
      write_error_to_pipe (pipefd1, &tmp_err);
      crun_error_release (&tmp_err);
    }

  exit (EXIT_FAILURE);
}

ចំនួនគត់
libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container, unsigned ចំនួនគត់ options,
                          libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ចំនួនគត់ ret;
  ចំនួនគត់ container_ready_pipe[2];
  cleanup_close ចំនួនគត់ pipefd0 = -1;
  cleanup_close ចំនួនគត់ pipefd1 = -1;
  cleanup_close ចំនួនគត់ exec_fifo_fd = -1;
  context->detach = 1;

  libcrun_debug ("Creating container: `%s`", context->id);
  container->context = context;

  ret = validate_options (options, LIBCRUN_CREATE_OPTIONS_PREFORK, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = check_config_file (def, context, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (def->process && def->process->terminal && context->console_socket == NULL)
    ត្រឡប់ crun_make_error (err, 0, "use --console-socket with create when a terminal is used");

  ret = libcrun_status_check_directories (context->state_root, context->id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  exec_fifo_fd = libcrun_status_create_exec_fifo (context->state_root, context->id, err);
  ប្រសិនបើ (UNLIKELY (exec_fifo_fd < 0))
    ត្រឡប់ exec_fifo_fd;

  context->fifo_exec_wait_fd = exec_fifo_fd;
  exec_fifo_fd = -1;

  ប្រសិនបើ ((options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      libcrun_debug ("Running with prefork enabled");
      ret = libcrun_copy_config_file (context->id, context->state_root, container, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
      ret = libcrun_container_run_internal (container, context, NULL, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        force_delete_container_status (context, container, def);
      ត្រឡប់ ret;
    }

  ret = pipe2 (container_ready_pipe, O_CLOEXEC);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "pipe");
  pipefd0 = container_ready_pipe[0];
  pipefd1 = container_ready_pipe[1];

  ret = fork ();
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "fork");
  ប្រសិនបើ (ret)
    {
      ចំនួនគត់ exit_code;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, NULL, 0);

      ret = TEMP_FAILURE_RETRY (read (pipefd0, &exit_code, parimaa_na (exit_code)));
      ប្រសិនបើ (UNLIKELY (ret <= 0))
        ត្រឡប់ crun_make_error (err, ret < 0 ? errno : 0, "waiting for container to be ready");

      ប្រសិនបើ (ret != parimaa_na (exit_code) || exit_code < 0)
        ត្រឡប់ crun_make_error (err, 0, "internal error: read invalid exit code");

      ប្រសិនបើ (exit_code == 0)
        ត្រឡប់ 0;

      libcrun_debug ("Exit code is `%d`, deleting container", exit_code);
      libcrun_error_t tmp_err = NULL;
      container_delete_internal (context, NULL, def, context->id, true, true, &tmp_err);
      libcrun_error_report_and_release (&tmp_err);
      ត្រឡប់ crun_make_error (err, 0, "error creating container");
    }

  /* forked process.  */
  ret = detach_process ();
  ប្រសិនបើ (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_root, container, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    {
      ចំនួនគត់ errcode = crun_error_get_errno (err);
      crun_error_release (err);
      libcrun_fail_with_error (errcode, "copy config file");
    }

  ret = libcrun_container_run_internal (container, context, &pipefd1, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    {
      force_delete_container_status (context, container, def);
      libcrun_error ((*err)->status, "%s", (*err)->msg);
      crun_error_release (err);
      crun_set_output_handler (log_write_to_stderr, NULL);
    }

  ប្រសិនបើ (pipefd1 >= 0)
    TEMP_FAILURE_RETRY (write (pipefd1, &ret, parimaa_na (ret)));
  exit (ret ? EXIT_FAILURE : 0);
}

ចំនួនគត់
libcrun_container_start (libcrun_context_t *context, const var_na *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = NULL;
  const var_na *state_root = context->state_root;
  runtime_spec_schema_config_schema *def;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_close ចំនួនគត់ fd = -1;
  ចំនួនគត់ ret;

  ret = libcrun_read_container_status (&status, state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = libcrun_is_container_running (&status, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (! ret)
    ត្រឡប់ crun_make_error (err, 0, "container `%s` is not running", id);

  ret = read_container_config_from_state (&container, state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (context->notify_socket)
    {
      ret = get_notify_fd (context, container, &fd, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }

  ret = libcrun_status_write_exec_fifo (context->state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  def = container->container_def;

  ប្រសិនបើ (context->notify_socket)
    {
      ប្រសិនបើ (fd >= 0)
        {
          fd_set read_set;

          ខណៈពេល (1)
            {
              sa_nracanaa timeval timeout = {
                .tv_sec = 0,
                .tv_usec = 10000,
              };
              FD_ZERO (&read_set);
              FD_SET (fd, &read_set);

              ret = select (fd + 1, &read_set, NULL, NULL, &timeout);
              ប្រសិនបើ (UNLIKELY (ret < 0))
                ត្រឡប់ crun_make_error (err, errno, "select failed");
              ប្រសិនបើ (ret)
                {
                  ret = handle_notify_socket (fd, err);
                  ប្រសិនបើ (UNLIKELY (ret < 0))
                    ត្រឡប់ ret;
                  ប្រសិនបើ (ret)
                    bha_nga;
                }
              បើមិនដូច្នេះ
                {
                  ret = libcrun_is_container_running (&status, err);
                  ប្រសិនបើ (UNLIKELY (ret < 0))
                    ត្រឡប់ ret;
                  ប្រសិនបើ (! ret)
                    ត្រឡប់ 0;
                }
            }
        }
    }

  /* The container is considered running only after we got the notification from the
     notify_socket, if any.  */
  ប្រសិនបើ (def->hooks && def->hooks->poststart_len)
    {
      cleanup_close ចំនួនគត់ hooks_out_fd = -1;
      cleanup_close ចំនួនគត់ hooks_err_fd = -1;

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      ret = do_hooks (def, status.pid, context->id, false, status.bundle, status.bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      ប្រសិនបើ (UNLIKELY (ret != 0))
        {
          libcrun_error_t tmp_err = NULL;
          container_delete_internal (context, container, def, id, true, true, &tmp_err);
          libcrun_error_report_and_release (&tmp_err);
          ត្រឡប់ ret;
        }
    }

  ត្រឡប់ 0;
}

ចំនួនគត់
libcrun_get_container_state_string (const var_na *id, libcrun_container_status_t *status, const var_na *state_root,
                                    const var_na **container_status, ចំនួនគត់ *running, libcrun_error_t *err)
{
  ចំនួនគត់ ret, has_fifo = 0;
  bool paused = false;

  ret = libcrun_is_container_running (status, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;
  *running = ret;

  ប្រសិនបើ (*running)
    {
      ret = libcrun_status_has_read_exec_fifo (state_root, id, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
      has_fifo = ret;
    }

  ប្រសិនបើ (*running && ! has_fifo)
    {
      cleanup_cgroup_status sa_nracanaa libcrun_cgroup_status *cgroup_status = NULL;

      cgroup_status = libcrun_cgroup_make_status (status);

      ret = libcrun_cgroup_is_container_paused (cgroup_status, &paused, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        {
          /*
            The cgroup might have been cleaned up by the time we try to read it, ignore both
            ENOENT and ENODEV:
            - ENOENT: if the open(CGROUP_PATH) fails because the cgroup was deleted.
            - ENODEV: if the cgroup is deleted between the open and reading the freeze status.
          */

          errno = crun_error_get_errno (err);
          ប្រសិនបើ (errno == ENOENT || errno == ENODEV)
            {
              crun_error_release (err);
              *container_status = "stopped";
              ត្រឡប់ 0;
            }

          ត្រឡប់ ret;
        }
    }

  ប្រសិនបើ (! *running)
    *container_status = "stopped";
  បើមិនដូច្នេះ ប្រសិនបើ (has_fifo)
    *container_status = "created";
  បើមិនដូច្នេះ ប្រសិនបើ (paused)
    *container_status = "paused";
  បើមិនដូច្នេះ
    *container_status = "running";

  ត្រឡប់ 0;
}

ចំនួនគត់
libcrun_container_state_json (libcrun_context_t *context, const var_na *id, var_na **out, libcrun_error_t *err)
{
  var_na *buffer = NULL;
  size_t len = 0;
  FILE *f;
  ចំនួនគត់ ret;

  f = open_memstream (&buffer, &len);
  ប្រសិនបើ (f == NULL)
    ត្រឡប់ crun_make_error (err, errno, "open_memstream");

  ret = libcrun_container_state (context, id, f, err);
  fclose (f);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    {
      free (buffer);
      ត្រឡប់ ret;
    }

  *out = buffer;
  ត្រឡប់ 0;
}

ចំនួនគត់
libcrun_container_state (libcrun_context_t *context, const var_na *id, FILE *out, libcrun_error_t *err)
{
  const var_na *const OCI_CONFIG_VERSION = "1.0.0";
  libcrun_container_status_t status = {};
  const var_na *state_root = context->state_root;
  const var_na *container_status = NULL;
  json_gen_ctx *gen = NULL;
  const var_na *buf;
  ចំនួនគត់ ret = 0;
  ចំនួនគត់ r = json_gen_status_ok;
  ចំនួនគត់ running;
  size_t len;

  ret = libcrun_read_container_status (&status, state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = libcrun_get_container_state_string (id, &status, state_root, &container_status, &running, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    goto exit;

  ret = 0;
  ប្រសិនបើ (! json_gen_init (&gen, NULL))
    ត្រឡប់ crun_make_error (err, 0, "json_gen_init failed");

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

  ប្រសិនបើ (status.scope)
    {
      GEN_KEY (gen, "systemd-scope");
      GEN_STR (gen, status.scope);
    }

  ប្រសិនបើ (status.owner)
    {
      GEN_KEY (gen, "owner");
      GEN_STR (gen, status.owner);
    }

  {
    cleanup_container libcrun_container_t *container = NULL;

    ret = read_container_config_from_state (&container, state_root, id, err);
    ប្រសិនបើ (UNLIKELY (ret < 0))
      goto exit;

    ប្រសិនបើ (container->container_def->annotations && container->container_def->annotations->len)
      GEN_OR_FAIL (gen_annotations (gen, container->container_def->annotations));
  }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  fprintf (out, "%s\n", buf);

exit:
  ប្រសិនបើ (gen)
    json_gen_free (gen);
  libcrun_free_container_status (&status);
  ត្រឡប់ ret;

gen_error:
  ret = json_gen_error_to_crun_error (r, err);
  goto exit;
}

ចំនួនគត់
libcrun_container_exec (libcrun_context_t *context, const var_na *id, runtime_spec_schema_config_schema_process *process,
                        libcrun_error_t *err)
{
  sa_nracanaa libcrun_container_exec_options_s opts;
  memset (&opts, 0, parimaa_na (opts));

  opts.struct_size = parimaa_na (opts);
  opts.process = process;

  ត្រឡប់ libcrun_container_exec_with_options (context, id, &opts, err);
}

ចំនួនគត់
libcrun_container_exec_process_file (libcrun_context_t *context, const var_na *id, const var_na *path, libcrun_error_t *err)
{
  sa_nracanaa libcrun_container_exec_options_s opts;
  memset (&opts, 0, parimaa_na (opts));

  opts.struct_size = parimaa_na (opts);
  opts.path = path;

  ត្រឡប់ libcrun_container_exec_with_options (context, id, &opts, err);
}

static ចំនួនគត់
exec_from_process_json (libcrun_context_t *context, const var_na *id, const var_na *process_json, const var_na *cgroup,
                        bool merge_env, libcrun_error_t *err)
{
  sa_nracanaa parser_context parser_ctx = { .options = 0, .errfile = stderr };
  sa_nracanaa libcrun_container_exec_options_s opts;
  runtime_spec_schema_config_schema_process *process;
  parser_error parser_err = NULL;
  json_object *doc = NULL;
  ចំនួនគត់ ret;

  ret = parse_json_file (&doc, process_json, &parser_ctx, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  process = make_runtime_spec_schema_config_schema_process (doc, &parser_ctx, &parser_err);
  json_object_put (doc);
  ប្រសិនបើ (process == NULL)
    {
      ret = crun_make_error (err, 0, "cannot parse process: `%s`", parser_err ? parser_err : "unknown error");
      free (parser_err);
      ត្រឡប់ ret;
    }

  memset (&opts, 0, parimaa_na (opts));
  opts.struct_size = parimaa_na (opts);
  opts.process = process;
  opts.cgroup = cgroup;
  opts.merge_env = merge_env;

  ret = libcrun_container_exec_with_options (context, id, &opts, err);
  free_runtime_spec_schema_config_schema_process (process);
  ត្រឡប់ ret;
}

ចំនួនគត់
libcrun_container_exec_json (libcrun_context_t *context, const var_na *id, const var_na *process_json, libcrun_error_t *err)
{
  ត្រឡប់ exec_from_process_json (context, id, process_json, NULL, false, err);
}

ចំនួនគត់
libcrun_container_exec_with_options_json (libcrun_context_t *context, const var_na *id,
                                          const sa_nracanaa libcrun_exec_options_s *opts, libcrun_error_t *err)
{
  ប្រសិនបើ (opts == NULL || opts->struct_size == 0)
    ត្រឡប់ crun_make_error (err, EINVAL, "invalid exec options");

  ត្រឡប់ exec_from_process_json (context, id, opts->process_json, opts->cgroup, opts->merge_env, err);
}

#define cleanup_process_schema __attribute__ ((cleanup (cleanup_process_schemap)))

static anta_hstha void
cleanup_process_schemap (runtime_spec_schema_config_schema_process **p)
{
  runtime_spec_schema_config_schema_process *process = *p;
  ប្រសិនបើ (process)
    (void) free_runtime_spec_schema_config_schema_process (process);
}

/* Build the seccomp receiver payload (when needed), apply the seccomp profile and
   close the seccomp file descriptors.  Used from exec_process_entrypoint both before
   and after the capabilities are set, depending on process->no_new_privileges.  */
static ចំនួនគត់
apply_seccomp_for_exec (libcrun_container_t *container, pid_t own_pid, ចំនួនគត់ seccomp_fd,
                        ចំនួនគត់ seccomp_receiver_fd, var_na **seccomp_flags, size_t seccomp_flags_len,
                        libcrun_error_t *err)
{
  cleanup_free var_na *seccomp_fd_payload = NULL;
  size_t seccomp_fd_payload_len = 0;
  ចំនួនគត់ ret;

  ប្រសិនបើ (seccomp_receiver_fd >= 0)
    {
      ret = get_seccomp_receiver_fd_payload (container, "running", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }

  ret = libcrun_apply_seccomp (seccomp_fd, seccomp_receiver_fd, seccomp_fd_payload,
                               seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  close_and_reset (&seccomp_fd);
  close_and_reset (&seccomp_receiver_fd);
  ត្រឡប់ 0;
}

static ចំនួនគត់
exec_process_entrypoint (libcrun_context_t *context,
                         libcrun_container_t *container,
                         runtime_spec_schema_config_schema_process *process,
                         ចំនួនគត់ *pipefd1,
                         ចំនួនគត់ seccomp_fd,
                         ចំនួនគត់ seccomp_receiver_fd,
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
  ចំនួនគត់ ret;

  container_uid = process->user ? process->user->uid : 0;
  container_gid = process->user ? process->user->gid : 0;

  TEMP_FAILURE_RETRY (read (*pipefd1, &own_pid, parimaa_na (own_pid)));

  cwd = process->cwd ? process->cwd : "/";
  ប្រសិនបើ (LIKELY (libcrun_safe_chdir (cwd, err) == 0))
    chdir_done = true;
  បើមិនដូច្នេះ
    crun_error_release (err);

  ret = unblock_signals (err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = clearenv ();
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "clearenv");

  ប្រសិនបើ ((merge_env || ! process->env_len) && container->container_def->process && container->container_def->process->env_len)
    {
      var_na *e;

      សម្រាប់ (i = 0; i < container->container_def->process->env_len; i++)
        {
          e = container->container_def->process->env[i];
          ប្រសិនបើ (putenv (e) < 0)
            ត្រឡប់ crun_make_error (err, errno, "putenv `%s`", e);
        }
    }
  ប្រសិនបើ (process->env_len)
    {
      សម្រាប់ (i = 0; i < process->env_len; i++)
        ប្រសិនបើ (putenv (process->env[i]) < 0)
          ត្រឡប់ crun_make_error (err, errno, "putenv `%s`", process->env[i]);
    }

  ប្រសិនបើ (is_empty_string (getenv ("HOME")))
    {
      ret = set_home_env (container_uid);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          ប្រសិនបើ (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            ត្រឡប់ crun_make_error (err, errno, "setenv HOME");
        }
    }

  ret = libcrun_set_selinux_label (container, process, false, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = libcrun_set_apparmor_profile (container, process, false, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (container->container_def->linux && container->container_def->linux->seccomp)
    {
      seccomp_flags = container->container_def->linux->seccomp->flags;
      seccomp_flags_len = container->container_def->linux->seccomp->flags_len;
    }

  ret = find_executable (&exec_path, process->args[0], process->cwd, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    {
      ប្រសិនបើ (crun_error_get_errno (err) == ENOENT)
        ត្រឡប់ ret;

      /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
         once the process switched to the use that runs in the container.  This might be necessary
         when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
         is not honored.  */
      crun_error_release (err);
    }

  ប្រសិនបើ (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }

  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, false, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = libcrun_reopen_dev_null (err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (! process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }

  ret = libcrun_container_setgroups (container, process, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = maybe_chown_std_streams (container_uid, container_gid, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (process->capabilities)
    capabilities = process->capabilities;
  បើមិនដូច្នេះ ប្រសិនបើ (container->container_def->process)
    capabilities = container->container_def->process->capabilities;

  ret = libcrun_set_caps (capabilities, container_uid, container_gid, process->no_new_privileges,
                          custom_handler != NULL, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (UNLIKELY (exec_path == NULL))
    {
      ret = find_executable (&exec_path, process->args[0], process->cwd, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        {
          ប្រសិនបើ (custom_handler == NULL || is_empty_string (process->args[0]))
            ត្រឡប់ ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          exec_path = xstrdup (process->args[0]);
        }
    }

  ប្រសិនបើ (! chdir_done)
    {
      ret = libcrun_safe_chdir (cwd, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }

  ប្រសិនបើ (process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }

  ប្រសិនបើ (process->user)
    umask (process->user->umask_present ? process->user->umask : 0022);

  ret = 0;
  TEMP_FAILURE_RETRY (write (*pipefd1, &ret, parimaa_na (ret)));
  TEMP_FAILURE_RETRY (close (*pipefd1));
  *pipefd1 = -1;

  ប្រសិនបើ (custom_handler)
    {
      ប្រសិនបើ (custom_handler->vtable->exec_func == NULL)
        ត្រឡប់ crun_make_error (err, 0, "the handler does not support exec");

      ប្រសិនបើ (context->argv)
        erase_initial_environment (context->argv, context->argc);

      ret = custom_handler->vtable->exec_func (custom_handler->cookie,
                                               container,
                                               exec_path,
                                               process->args);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ crun_make_error (err, -ret, "exec container process failed with handler as `%s`", custom_handler->vtable->name);

      _safe_exit (EXIT_FAILURE);
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation, because the seccomp filter is already in place and it could
     stop some syscalls used by mark_or_close_fds_ge_than.
  */
  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, true, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, process->args));
  libcrun_fail_with_error (errno, "exec `%s`", exec_path);
}

/* Close the file descriptors inherited from the caller, other than those
   to be passed to the container process.  */
ចំនួនគត់
libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err)
{
  ត្រឡប់ mark_or_close_fds_ge_than (NULL, context->preserve_fds + 3, true, err);
}

ចំនួនគត់
libcrun_load_process_from_file (const var_na *path, runtime_spec_schema_config_schema_process **process,
                                libcrun_error_t *err)
{
  sa_nracanaa parser_context ctx = { 0, stderr };
  cleanup_free var_na *content = NULL;
  parser_error parser_err = NULL;
  json_object *doc = NULL;
  size_t len;
  ចំនួនគត់ ret;

  ret = read_all_file (path, &content, &len, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  *process = make_runtime_spec_schema_config_schema_process (doc, &ctx, &parser_err);
  ប្រសិនបើ (UNLIKELY (*process == NULL))
    {
      ret = crun_make_error (err, 0, "cannot parse process file: `%s`", parser_err);
      free (parser_err);
      ប្រសិនបើ (doc)
        json_object_put (doc);
      ត្រឡប់ ret;
    }

  free (parser_err);
  ប្រសិនបើ (doc)
    json_object_put (doc);

  ត្រឡប់ 0;
}

ចំនួនគត់
libcrun_container_exec_with_options (libcrun_context_t *context, const var_na *id,
                                     sa_nracanaa libcrun_container_exec_options_s *opts,
                                     libcrun_error_t *err)
{
  cleanup_custom_handler_instance sa_nracanaa custom_handler_instance_s *custom_handler = NULL;
  ចំនួនគត់ container_status, ret;
  bool container_paused = false;
  pid_t pid;
  libcrun_container_status_t status = {};
  const var_na *state_root = context->state_root;
  cleanup_close ចំនួនគត់ terminal_fd = -1;
  cleanup_close ចំនួនគត់ seccomp_fd = -1;
  cleanup_terminal void *orig_terminal = NULL;
  cleanup_container libcrun_container_t *container = NULL;
  ចំនួនគត់ container_ret_status[2];
  cleanup_close ចំនួនគត់ pipefd0 = -1;
  cleanup_close ចំនួនគត់ pipefd1 = -1;
  cleanup_close ចំនួនគត់ seccomp_receiver_fd = -1;
  cleanup_close ចំនួនគត់ own_seccomp_receiver_fd = -1;
  cleanup_close ចំនួនគត់ seccomp_notify_fd = -1;
  const var_na *seccomp_notify_plugins = NULL;
  __attribute__ ((unused)) cleanup_process_schema runtime_spec_schema_config_schema_process *process_cleanup = NULL;
  runtime_spec_schema_config_schema_process *process = opts->process;
  sa_nracanaa libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  ចំនួនគត់ ret_from_child = 0;

  ret = libcrun_read_container_status (&status, state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = libcrun_is_container_running (&status, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;
  container_status = ret;

  ret = read_container_config_from_state (&container, state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  container->context = context;

  ប្រសិនបើ (container_status == 0)
    ត្រឡប់ crun_make_error (err, 0, "the container `%s` is not running", id);

  {
    cleanup_cgroup_status sa_nracanaa libcrun_cgroup_status *cgroup_status = NULL;

    cgroup_status = libcrun_cgroup_make_status (&status);

    ret = libcrun_cgroup_is_container_paused (cgroup_status, &container_paused, err);
    ប្រសិនបើ (UNLIKELY (ret < 0))
      ត្រឡប់ ret;
  }

  ប្រសិនបើ (UNLIKELY (container_paused))
    ត្រឡប់ crun_make_error (err, 0, "cannot exec in a paused container `%s`", id);

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = block_signals (err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  libcrun_seccomp_gen_ctx_init (&seccomp_gen_ctx, container, false, 0);

  ret = libcrun_open_seccomp_bpf (&seccomp_gen_ctx, &seccomp_fd, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &seccomp_receiver_fd,
                                     &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }

  ប្រសិនបើ (parimaa_na (*opts) != opts->struct_size)
    ត្រឡប់ crun_make_error (err, EINVAL, "invalid libcrun_container_exec_options_s struct");

  ប្រសិនបើ (opts->path)
    {
      ប្រសិនបើ (process)
        ត្រឡប់ crun_make_error (err, EINVAL, "cannot specify both exec file and options");

      ret = libcrun_load_process_from_file (opts->path, &process, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      process_cleanup = process;
    }

  /* This must be done before we enter a user namespace.  If the process does
     not specify any rlimits, use the ones from the container configuration.  */
  ប្រសិនបើ (process->rlimits_len == 0 && container->container_def->process)
    ret = libcrun_set_rlimits (container->container_def->process->rlimits,
                               container->container_def->process->rlimits_len, err);
  បើមិនដូច្នេះ
    ret = libcrun_set_rlimits (process->rlimits, process->rlimits_len, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = libcrun_set_mempolicy (container->container_def, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  /* If the new process block doesn't specify a SELinux label, AppArmor profile, user,
     I/O priority or scheduler, then use the configuration from the original config file.  */
  ប្រសិនបើ (container->container_def->process)
    {
      ប្រសិនបើ (process->io_priority == NULL && container->container_def->process->io_priority)
        {
          process->io_priority = clone_runtime_spec_schema_config_schema_process_io_priority (container->container_def->process->io_priority);
          ប្រសិនបើ (process->io_priority == NULL)
            OOM ();
        }

      ប្រសិនបើ (process->scheduler == NULL && container->container_def->process->scheduler)
        {
          process->scheduler = clone_runtime_spec_schema_config_schema_process_scheduler (container->container_def->process->scheduler);
          ប្រសិនបើ (process->scheduler == NULL)
            OOM ();
        }

      ប្រសិនបើ (process->selinux_label == NULL && container->container_def->process->selinux_label)
        process->selinux_label = xstrdup (container->container_def->process->selinux_label);

      ប្រសិនបើ (process->apparmor_profile == NULL && container->container_def->process->apparmor_profile)
        process->apparmor_profile = xstrdup (container->container_def->process->apparmor_profile);

      ប្រសិនបើ (process->user == NULL && container->container_def->process->user)
        {
          process->user = clone_runtime_spec_schema_config_schema_process_user (container->container_def->process->user);
          ប្រសិនបើ (process->user == NULL)
            OOM ();
        }
    }

  ret = initialize_security (container, process, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  /* Do not leak the session keyring of the caller into the container.  The
     keyring created for the container cannot be joined, so use an anonymous one.  */
  ប្រសិនបើ (! context->no_new_keyring && ! status.no_new_keyring)
    {
      libcrun_debug ("Creating new keyring for the exec process");

      ret = libcrun_create_keyring (container, NULL, process->selinux_label, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "prctl unset dumpable");

  pid = libcrun_join_process (context, container, status.pid, &status, opts->cgroup, context->detach,
                              process, process->terminal ? &terminal_fd : NULL, err);
  ប្រសិនបើ (UNLIKELY (pid < 0))
    ត្រឡប់ pid;

  /* Process to exec.  */
  ប្រសិនបើ (pid == 0)
    {
      TEMP_FAILURE_RETRY (close (pipefd0));
      pipefd0 = -1;

      exec_process_entrypoint (context, container, process, &pipefd1, seccomp_fd, seccomp_receiver_fd, custom_handler, opts->merge_env, err);
      /* It gets here only on errors.  */
      ប្រសិនបើ (*err)
        {
          ប្រសិនបើ (pipefd1 < 0)
            libcrun_fail_with_error ((*err)->status, "%s", (*err)->msg);
          បើមិនដូច្នេះ
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

  ប្រសិនបើ (seccomp_fd >= 0)
    close_and_reset (&seccomp_fd);

  ប្រសិនបើ (terminal_fd >= 0)
    {
      unsigned short rows = 0, cols = 0;

      ប្រសិនបើ (process->console_size)
        {
          cols = process->console_size->width;
          rows = process->console_size->height;
        }

      ret = libcrun_terminal_setup_size (terminal_fd, rows, cols, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      ប្រសិនបើ (context->console_socket)
        {
          ចំនួនគត់ ret;
          cleanup_close ចំនួនគត់ console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
          ប្រសិនបើ (UNLIKELY (console_socket_fd < 0))
            ត្រឡប់ console_socket_fd;
          ret = send_fd_to_socket (console_socket_fd, terminal_fd, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ ret;
          close_and_reset (&terminal_fd);
        }
      បើមិនដូច្នេះ
        {
          ret = libcrun_clear_onlcr (terminal_fd, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              ត្រឡប់ ret;
            }

          ret = libcrun_set_raw (0, &orig_terminal, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              ត្រឡប់ ret;
            }
        }
    }

  ret = TEMP_FAILURE_RETRY (read (pipefd0, &ret_from_child, parimaa_na (ret_from_child)));
  ប្រសិនបើ (ret != parimaa_na (ret_from_child))
    ret = crun_make_error (err, 0, "read pipe failed");
  បើមិនដូច្នេះ ប្រសិនបើ (ret_from_child != 0)
    {
      cleanup_free var_na *msg = NULL;
      size_t len = 0;

      ret = read_all_fd (pipefd0, "error stream", &msg, &len, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
      /* the string from read_all_fd is always NUL terminated.  */
      ret = crun_make_error (err, ret_from_child, "%s", msg);
    }
  បើមិនដូច្នេះ
    {
      /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
      ប្រសិនបើ (own_seccomp_receiver_fd >= 0)
        {
          seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
          ប្រសិនបើ (UNLIKELY (seccomp_notify_fd < 0))
            ត្រឡប់ seccomp_notify_fd;

          ret = close_and_reset (&own_seccomp_receiver_fd);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            ត្រឡប់ crun_make_error (err, errno, "close seccomp receiver fd failed");
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
  ត្រឡប់ ret;
}

ចំនួនគត់
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
  ចំនួនគត់ ret;

  ret = libcrun_read_container_status (&status, state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = read_container_config_from_state (&container, state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  resources = make_runtime_spec_schema_config_linux_resources (doc, &ctx, &parser_err);
  ប្រសិនបើ (UNLIKELY (resources == NULL))
    {
      ret = crun_make_error (err, 0, "cannot parse resources: %s", parser_err);
      goto cleanup;
    }

  ប្រសិនបើ (custom_handler && custom_handler->vtable->modify_oci_configuration)
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
      ប្រសិនបើ (UNLIKELY (ret < 0))
        goto cleanup;
    }

  ret = libcrun_linux_container_update (&status, state_root, resources, err);

cleanup:
  ប្រសិនបើ (doc)
    json_object_put (doc);
  free (parser_err);
  ប្រសិនបើ (resources)
    free_runtime_spec_schema_config_linux_resources (resources);

  ត្រឡប់ ret;
}

ចំនួនគត់
libcrun_container_update_from_file (libcrun_context_t *context, const var_na *id, const var_na *file, libcrun_error_t *err)
{
  cleanup_free var_na *content = NULL;
  size_t len;
  ចំនួនគត់ ret;

  ret = read_all_file (file, &content, &len, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ត្រឡប់ libcrun_container_update (context, id, content, len, err);
}

static ចំនួនគត់
compare_update_values (const void *a, const void *b)
{
  const sa_nracanaa libcrun_update_value_s *aa = a;
  const sa_nracanaa libcrun_update_value_s *bb = b;
  ចំនួនគត់ ret;

  ret = strcmp (aa->section, bb->section);
  ប្រសិនបើ (ret)
    ត្រឡប់ ret;
  ត្រឡប់ strcmp (aa->name, bb->name);
}

ចំនួនគត់
libcrun_container_update_from_values (libcrun_context_t *context, const var_na *id,
                                      sa_nracanaa libcrun_update_value_s *values, size_t len,
                                      libcrun_error_t *err)
{
  const var_na *current_section = NULL;
  const var_na *buf;
  json_gen_ctx *gen = NULL;
  size_t i, buf_len;
  ចំនួនគត់ ret;

  ប្រសិនបើ (! json_gen_init (&gen, NULL))
    ត្រឡប់ crun_make_error (err, 0, "json_gen_init failed");
  json_gen_map_open (gen);

  qsort (values, len, parimaa_na (sa_nracanaa libcrun_update_value_s), compare_update_values);

  សម្រាប់ (i = 0; i < len; i++)
    {
      ប្រសិនបើ (current_section == NULL || strcmp (values[i].section, current_section))
        {
          ប្រសិនបើ (i > 0)
            json_gen_map_close (gen);

          current_section = values[i].section;
          json_gen_string (gen, current_section, strlen (current_section));
          json_gen_map_open (gen);
        }

      json_gen_string (gen, values[i].name, strlen (values[i].name));

      ប្រសិនបើ (values[i].numeric)
        json_gen_number (gen, values[i].value, strlen (values[i].value));
      បើមិនដូច្នេះ
        json_gen_string (gen, values[i].value, strlen (values[i].value));
    }
  ប្រសិនបើ (len)
    json_gen_map_close (gen);

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);

  ret = libcrun_container_update (context, id, buf, buf_len, err);

  json_gen_free (gen);

  ត្រឡប់ ret;
}

static void
populate_array_field (var_na ***field, var_na *array[], size_t num_elements)
{
  size_t i;

  *field = xmalloc0 ((num_elements + 1) * parimaa_na (var_na *));
  សម្រាប់ (i = 0; i < num_elements; i++)
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
  ចំនួនគត់ j;

  *num_capabilities = 0;
  សម្រាប់ (i = 0;; i++)
    {
      var_na *v = cap_to_name (i);
      ប្រសិនបើ (v == NULL)
        bha_nga;
      strtol (v, &endptr, 10);
      ប្រសិនបើ (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          bha_nga;
        }
      (*num_capabilities)++;
    }

  *capabilities = xmalloc0 ((*num_capabilities + 1) * parimaa_na (const var_na *));
  សម្រាប់ (i = 0; i < (cap_value_t) *num_capabilities; i++)
    {
      var_na *v = cap_to_name (i);
      ប្រសិនបើ (v == NULL)
        bha_nga;
      strtol (v, &endptr, 10);
      ប្រសិនបើ (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          bha_nga;
        }

      // Convert capability name to uppercase
      សម្រាប់ (j = 0; v[j] != '\0'; j++)
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
  ខណៈពេល (mount_options_list[num_mount_options].name != NULL)
    num_mount_options++;

  // Allocate memory for mount options in info struct
  (*info)->mount_options = xmalloc0 ((num_mount_options + 1) * parimaa_na (var_na *));

  // Copy mount options to info struct
  សម្រាប់ (size_t i = 0; i < num_mount_options; i++)
    (*info)->mount_options[i] = xstrdup (mount_options_list[i].name);
}

ចំនួនគត់
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
#បើមិនដូច្នេះ
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

  ប្រសិនបើ (context->handler_manager && handler_by_name (context->handler_manager, "wasm"))
    (*info)->annotations.run_oci_crun_wasm = true;

#ប្រសិនបើ HAVE_CRIU
  (*info)->annotations.run_oci_crun_checkpoint_enabled = true;
#endif
  (*info)->annotations.run_oci_crun_commit = GIT_VERSION;
  (*info)->annotations.run_oci_crun_version = PACKAGE_VERSION;

  populate_array_field (&((*info)->potentially_unsafe_annotations), potentially_unsafe_annotations, num_unsafe_annotations);

  ត្រឡប់ 0;
}

static void
features_add_string (json_gen_ctx *json_gen, const var_na *key, const var_na *value)
{
  json_gen_string (json_gen, key, strlen (key));
  json_gen_string (json_gen, value, strlen (value));
}

static void
features_add_bool (json_gen_ctx *json_gen, const var_na *key, ចំនួនគត់ value)
{
  json_gen_string (json_gen, key, strlen (key));
  json_gen_bool (json_gen, value);
}

static void
features_add_bool_str (json_gen_ctx *json_gen, const var_na *key, ចំនួនគត់ value)
{
  features_add_string (json_gen, key, value ? "true" : "false");
}

static void
features_add_array (json_gen_ctx *json_gen, const var_na *key, var_na **array)
{
  size_t i;

  json_gen_string (json_gen, key, strlen (key));
  json_gen_array_open (json_gen);
  ប្រសិនបើ (array)
    សម្រាប់ (i = 0; array[i] != NULL; i++)
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
  ប្រសិនបើ (linux->seccomp.actions)
    features_add_array (json_gen, "actions", linux->seccomp.actions);
  ប្រសិនបើ (linux->seccomp.operators)
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
  ប្រសិនបើ (linux->memory_policy.mode)
    features_add_array (json_gen, "modes", linux->memory_policy.mode);
  ប្រសិនបើ (linux->memory_policy.flags)
    features_add_array (json_gen, "flags", linux->memory_policy.flags);
  json_gen_map_close (json_gen);

  json_gen_map_close (json_gen);
}

static void
features_add_annotations_info (json_gen_ctx *json_gen, const sa_nracanaa annotations_info_s *annotation)
{
  json_gen_string (json_gen, "annotations", strlen ("annotations"));
  json_gen_map_open (json_gen);

  ប្រសិនបើ (! is_empty_string (annotation->io_github_seccomp_libseccomp_version))
    features_add_string (json_gen, "io.github.seccomp.libseccomp.version", annotation->io_github_seccomp_libseccomp_version);

  features_add_bool_str (json_gen, "org.opencontainers.runc.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);
  features_add_bool_str (json_gen, "run.oci.crun.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);

  features_add_string (json_gen, "run.oci.crun.commit", annotation->run_oci_crun_commit);
  features_add_string (json_gen, "run.oci.crun.version", annotation->run_oci_crun_version);

  features_add_bool_str (json_gen, "run.oci.crun.wasm", annotation->run_oci_crun_wasm);

  json_gen_map_close (json_gen);
}

ចំនួនគត់
libcrun_container_get_features_json (libcrun_context_t *context, var_na **out, libcrun_error_t *err)
{
  cleanup_struct_features sa_nracanaa features_info_s *info = NULL;
  const var_na *buffer = NULL;
  json_gen_ctx *json_gen;
  size_t buffer_len;
  ចំនួនគត់ ret;

  *out = NULL;

  ret = libcrun_container_get_features (context, &info, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (! json_gen_init (&json_gen, NULL))
    ត្រឡប់ crun_make_error (err, 0, "cannot initialize JSON generator");

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

  ត្រឡប់ 0;
}

/* Read the container status and fail if the container is not running.  */
static ចំនួនគត់
read_status_require_running (libcrun_context_t *context, const var_na *id,
                             libcrun_container_status_t *status, libcrun_error_t *err)
{
  ចំនួនគត់ ret;

  ret = libcrun_read_container_status (status, context->state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = libcrun_is_container_running (status, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;
  ប្រសិនបើ (ret == 0)
    ត្រឡប់ crun_make_error (err, 0, "the container `%s` is not running", id);

  ត្រឡប់ 0;
}

ចំនួនគត់
libcrun_container_pause (libcrun_context_t *context, const var_na *id, libcrun_error_t *err)
{
  ចំនួនគត់ ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ត្រឡប់ libcrun_container_pause_linux (&status, err);
}

ចំនួនគត់
libcrun_container_unpause (libcrun_context_t *context, const var_na *id, libcrun_error_t *err)
{
  ចំនួនគត់ ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ត្រឡប់ libcrun_container_unpause_linux (&status, err);
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

ចំនួនគត់
libcrun_container_checkpoint (libcrun_context_t *context, const var_na *id,
                              sa_nracanaa libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  ចំនួនគត់ ret;
  const var_na *state_root = context->state_root;
  libcrun_container_status_t status = {};
  libcrun_checkpoint_restore_t cr;
  cleanup_container libcrun_container_t *container = NULL;

  cr_options_from_public (&cr, cr_options);

  ret = read_status_require_running (context, id, &status, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = read_container_config_from_state (&container, state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  /* Needed to use the same paths (e.g. the shared empty directory used for
     masked paths) as the container runtime and restore do.  */
  container->context = context;

  ret = libcrun_container_checkpoint_linux (&status, container, &cr, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (! (cr_options->leave_running || cr_options->pre_dump))
    ត្រឡប់ container_delete_internal (context, NULL, NULL, id, true, true, err);

  ត្រឡប់ 0;
}

/* The dummy process only keeps the cgroup populated until the container is restored there.
   Its cgroup is below the container cgroup, so once the container runs it can freeze or kill
   the dummy process: never depend on it after that point.  */
static void
restore_proxy_process (ចំនួនគត់ proxy_pid_pipe)
{
  var_na c;

  /* Wait to be killed by the parent, or for the parent to go away.  */
  TEMP_FAILURE_RETRY (read (proxy_pid_pipe, &c, parimaa_na (c)));
}

static ចំនួនគត់
restore_terminate_proxy_process (pid_t proxy_pid, const var_na *proxy_cgroup, pid_t container_pid, ចំនួនគត់ cgroup_manager, libcrun_error_t *err)
{
  ចំនួនគត់ mode;
  ចំនួនគត់ ret;

  ប្រសិនបើ (cgroup_manager == CGROUP_MANAGER_SYSTEMD)
    {
      var_na ready_str[64];

      ret = snprintf (ready_str, parimaa_na (ready_str), "MAINPID=%d", container_pid);
      ប្រសិនបើ (UNLIKELY (ret >= (ចំនួនគត់) parimaa_na (ready_str)))
        ត្រឡប់ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = send_sd_notify (ready_str, err);
      /* Do not fail on errors.  */
      ប្រសិនបើ (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  mode = libcrun_get_cgroup_mode (err);
  ប្រសិនបើ (UNLIKELY (mode < 0))
    ត្រឡប់ mode;

  /* SIGKILL terminates a process frozen with cgroup v2, while the cgroup v1 freezer does not
     deliver it until the cgroup is thawed, so do not wait for it there.  */
  kill (proxy_pid, SIGKILL);
  TEMP_FAILURE_RETRY (waitpid (proxy_pid, NULL, mode == CGROUP_MODE_UNIFIED ? 0 : WNOHANG));

  ត្រឡប់ destroy_cgroup_path (proxy_cgroup, mode, err);
}

ចំនួនគត់
libcrun_container_restore (libcrun_context_t *context, const var_na *id arg_unused,
                           sa_nracanaa libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  cleanup_cgroup_status sa_nracanaa libcrun_cgroup_status *cgroup_status = NULL;
  cleanup_container libcrun_container_t *container = NULL;
  cleanup_close ចំនួនគត់ proxy_pid_pipe0 = -1;
  cleanup_close ចំនួនគត់ proxy_pid_pipe1 = -1;
  runtime_spec_schema_config_schema *def;
  libcrun_container_status_t status = {};
  sa_nracanaa libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  libcrun_checkpoint_restore_t cr;
  cleanup_pid pid_t proxy_pid = -1;
  cleanup_close ចំនួនគត់ seccomp_fd = -1;
  const var_na *seccomp_bpf_data;
  ចំនួនគត់ proxy_pid_pipe[2];
  ចំនួនគត់ cgroup_manager;
  uid_t root_uid = -1;
  gid_t root_gid = -1;
  ចំនួនគត់ ret;

  cr_options_from_public (&cr, cr_options);

  container = libcrun_container_load_from_file ("config.json", err);
  ប្រសិនបើ (container == NULL)
    ត្រឡប់ -1;

  container->context = context;
  def = container->container_def;

  ret = check_config_file (def, context, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = libcrun_status_check_directories (context->state_root, context->id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = libcrun_copy_config_file (context->id, context->state_root, container, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    goto fail;

  /* CRIU restores the seccomp filters of the checkpointed processes, but
     exec loads the filter from the seccomp.bpf file in the state directory,
     so it must be generated again for the restored container.  */
  seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    goto fail;

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    goto fail;
  close_and_reset (&seccomp_fd);

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  ប្រសិនបើ (context->systemd_cgroup)
    cgroup_manager = CGROUP_MANAGER_SYSTEMD;
  បើមិនដូច្នេះ ប្រសិនបើ (context->force_no_cgroup)
    cgroup_manager = CGROUP_MANAGER_DISABLED;

  def = container->container_def;

  /* If we are root (either on the host or in a namespace),
   * then chown the cgroup to root in the container user namespace. */
  get_root_in_the_userns (def, container->host_uid, container->host_gid, &root_uid, &root_gid);

  /* If the root in the container is different than the current root user, attempt to chown
     the std streams before entering the user namespace.  Otherwise we might lose access
     to the user (as it is not mapped in the user namespace) and cannot chown them.  */
  ប្រសិនបើ (root_uid > 0 || root_gid > 0)
    {
      ret = maybe_chown_std_streams (root_uid, root_gid, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        goto fail;
    }

  /* CRIU runs as a child process, so the restored processes would inherit the session keyring.  */
  ret = setup_container_keyring (container, context, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  {
    cleanup_close ចំនួនគត់ cgroup_dirfd = -1;
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
    ប្រសិនបើ (UNLIKELY (ret < 0))
      goto fail;

    needs_proxy_process = cgroup_dirfd < 0;

    /* If the target cgroup was already created (with cgroupfs), we can restore the container directly there.  */
    ប្រសិនបើ (! needs_proxy_process)
      {
        cleanup_free var_na *cgroup_path = NULL;

        ret = get_cgroup_dirfd_path (cgroup_dirfd, &cgroup_path, err);
        ប្រសិនបើ (UNLIKELY (ret < 0))
          goto fail;

        /* Complete the configuration for the cgroup before the container is
           restored there: CRIU resumes the restored processes before it
           returns, so the device filter and the limits must already be in
           place.  There is no process to move, as CRIU is run in the cgroup,
           and the controllers were enabled by precreate_cgroup.  */
        cg.joined = true;

        ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
        ប្រសិនបើ (UNLIKELY (ret < 0))
          goto fail;

        /* Restore the container directly in the desired cgroup.  */
        status.cgroup_path = cgroup_path;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        ប្រសិនបើ (UNLIKELY (ret < 0))
          goto fail;

        /* Now that the container is restored, keep any signal for it, so that
           it is forwarded to the init by wait_for_process, rather than acted
           upon by crun.  This cannot be done earlier, as CRIU, which is run as
           a child process, would inherit the blocked signals.  */
        ret = block_signals (err);
        ប្រសិនបើ (UNLIKELY (ret < 0))
          goto fail;

        /* Use the container first process PID to finalize the cgroup.  */
        cg.pid = status.pid;
      }
    បើមិនដូច្នេះ
      {
        /* When using systemd, we need a process first that must exist before a cgroup is created.
           Create a dummy process that sits and waits until the container is restored.  */
        ret = pipe2 (proxy_pid_pipe, O_CLOEXEC);
        ប្រសិនបើ (UNLIKELY (ret < 0))
          {
            ret = crun_make_error (err, errno, "pipe");
            goto fail;
          }

        /* Only used for auto cleanup.  */
        proxy_pid_pipe0 = proxy_pid_pipe[0];
        proxy_pid_pipe1 = proxy_pid_pipe[1];

        proxy_pid = fork ();
        ប្រសិនបើ (UNLIKELY (proxy_pid < 0))
          {
            ret = crun_make_error (err, errno, "fork");
            goto fail;
          }

        ប្រសិនបើ (proxy_pid == 0)
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
        ប្រសិនបើ (UNLIKELY (ret < 0))
          goto fail;
      }

    ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
    ប្រសិនបើ (UNLIKELY (ret < 0))
      goto fail;

    /* When using a dummy process, the container is restored here once the cgroup is configured.  */
    ប្រសិនបើ (needs_proxy_process)
      {
        cleanup_free var_na *target_cgroup = NULL;
        cleanup_free var_na *proxy_cgroup = NULL;

        ret = libcrun_get_cgroup_process (proxy_pid, &target_cgroup, false, err);
        ប្រសិនបើ (UNLIKELY (ret < 0))
          goto fail;

        /* Move the dummy process to a sub-cgroup.  */
        ret = append_paths (&proxy_cgroup, err, target_cgroup, ".proxy-crun", NULL);
        ប្រសិនបើ (UNLIKELY (ret < 0))
          goto fail;

        ret = libcrun_move_process_to_cgroup (proxy_pid, 0, proxy_cgroup, true, err);
        ប្រសិនបើ (UNLIKELY (ret < 0))
          goto fail;

        /* Restore the container in the same cgroup where the dummy process was.  */
        status.cgroup_path = target_cgroup;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        ប្រសិនបើ (UNLIKELY (ret < 0))
          goto fail;

        /* See the comment for the other libcrun_container_restore_linux call.  */
        ret = block_signals (err);
        ប្រសិនបើ (UNLIKELY (ret < 0))
          goto fail;

        /* The container is running, so do not fail because of the dummy process.  */
        ret = restore_terminate_proxy_process (proxy_pid, proxy_cgroup, status.pid, cgroup_manager, err);
        ប្រសិនបើ (UNLIKELY (ret < 0))
          crun_error_release (err);
        proxy_pid = -1;

        close_and_reset (&proxy_pid_pipe1);
      }
  }

  context->detach = cr_options->detach;
  ret = write_container_status (container, context, status.pid, cgroup_status, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
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

    ត្រឡប់ wait_for_process (&args, err);
  }

fail:
  /* Once restored, the container is running.  Without a status file nothing
     can reach it anymore, so do not leave it behind.  */
  ប្រសិនបើ (status.pid > 0)
    {
      kill (status.pid, SIGKILL);
      waitpid_ignore_stopped (status.pid, NULL, 0);
    }
  ប្រសិនបើ (cgroup_status)
    {
      libcrun_error_t tmp_err = NULL;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  {
    libcrun_error_t tmp_err = NULL;

    ប្រសិនបើ (libcrun_container_delete_status (context->state_root, context->id, &tmp_err) < 0)
      crun_error_release (&tmp_err);
  }
  ត្រឡប់ ret;
}

ចំនួនគត់
libcrun_container_read_pids (libcrun_context_t *context, const var_na *id, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  cleanup_cgroup_status sa_nracanaa libcrun_cgroup_status *cgroup_status = NULL;
  cleanup_container_status libcrun_container_status_t status = {};
  ចំនួនគត់ ret;

  ret = libcrun_read_container_status (&status, context->state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (status.cgroup_path == NULL || status.cgroup_path[0] == '\0')
    ត្រឡប់ crun_make_error (err, 0, "the container is not using cgroups");

  cgroup_status = libcrun_cgroup_make_status (&status);

  ត្រឡប់ libcrun_cgroup_read_pids (cgroup_status, recurse, pids, err);
}

ចំនួនគត់
libcrun_container_list_json (libcrun_context_t *context, var_na **out, libcrun_error_t *err)
{
  libcrun_container_list_t *list = NULL, *it;
  const var_na *content = NULL;
  json_gen_ctx *gen = NULL;
  size_t len;
  ចំនួនគត់ ret;

  *out = NULL;

  ret = libcrun_get_containers_list (&list, context->state_root, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (! json_gen_init (&gen, NULL))
    {
      ret = crun_make_error (err, 0, "cannot allocate json generator");
      goto exit;
    }

  json_gen_config (gen, json_gen_beautify, 1);
  json_gen_array_open (gen);

  សម្រាប់ (it = list; it; it = it->next)
    {
      libcrun_container_status_t status;
      ចំនួនគត់ running = 0;
      ចំនួនគត់ pid;
      const var_na *container_status = NULL;

      ret = libcrun_read_container_status (&status, context->state_root, it->name, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        goto exit;

      pid = status.pid;

      ret = libcrun_get_container_state_string (it->name, &status, context->state_root, &container_status,
                                                &running, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          continue;
        }

      ប្រសិនបើ (! running)
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
  ប្រសិនបើ (json_gen_get_buf (gen, &content, &len) != json_gen_status_ok)
    {
      ret = libcrun_make_error (err, 0, "cannot generate json list");
      goto exit;
    }

  *out = xstrdup (content);
  ret = 0;

exit:
  ប្រសិនបើ (list)
    libcrun_free_containers_list (list);
  ប្រសិនបើ (gen)
    json_gen_free (gen);

  ត្រឡប់ ret;
}

ចំនួនគត់
libcrun_write_json_containers_list (libcrun_context_t *context, FILE *out, libcrun_error_t *err)
{
  cleanup_free var_na *content = NULL;
  size_t len;
  ចំនួនគត់ ret;

  ret = libcrun_container_list_json (context, &content, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  len = strlen (content);
  ប្រសិនបើ (len && fwrite (content, 1, len, out) != len)
    ត្រឡប់ crun_make_error (err, errno, "error writing to file");

  ត្រឡប់ 0;
}

ចំនួនគត់
libcrun_container_update_intel_rdt (libcrun_context_t *context, const var_na *id, sa_nracanaa libcrun_intel_rdt_update *update, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = NULL;
  ចំនួនគត់ ret;

  ret = read_container_config_from_state (&container, context->state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ត្រឡប់ libcrun_update_intel_rdt (id, container, update->l3_cache_schema, update->mem_bw_schema, update->schemata, err);
}

static ចំនួនគត់
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
  ចំនួនគត់ ret = 1;

  ret = read_all_file (file, &content, &len, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = libcrun_read_container_status (&status, state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = read_container_config_from_state (&container, state_root, id, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = parse_json_file (&doc, content, &pctx, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (! json_object_is_type (doc, json_type_array))
    {
      ret = crun_make_error (err, 0, "mounts must be an array");
      goto cleanup;
    }
  បើមិនដូច្នេះ
    {
      n_mounts = json_object_array_length (doc);

      mounts = xmalloc0 ((n_mounts + 1) * parimaa_na (*mounts));
      សម្រាប់ (i = 0; i < n_mounts; i++)
        {
          mounts[i] = make_runtime_spec_schema_defs_mount (json_object_array_get_idx (doc, i), &pctx, &parser_err);
          ប្រសិនបើ (mounts[i] == NULL)
            {
              ret = crun_make_error (err, 0, "cannot parse mount: %s", parser_err);
              goto cleanup;
            }
        }
    }

  ប្រសិនបើ (add)
    ret = libcrun_make_runtime_mounts (container, &status, mounts, n_mounts, err);
  បើមិនដូច្នេះ
    ret = libcrun_destroy_runtime_mounts (container, &status, mounts, n_mounts, err);

cleanup:
  ប្រសិនបើ (doc)
    json_object_put (doc);
  ប្រសិនបើ (mounts)
    សម្រាប់ (i = 0; i < n_mounts; i++)
      free_runtime_spec_schema_defs_mount (mounts[i]);
  ត្រឡប់ ret;
}

ចំនួនគត់
libcrun_container_add_mounts_from_file (libcrun_context_t *context, const var_na *id, const var_na *file, libcrun_error_t *err)
{
  ត្រឡប់ libcrun_container_add_or_remove_mounts_from_file (context, id, file, true, err);
}

ចំនួនគត់
libcrun_container_remove_mounts_from_file (libcrun_context_t *context, const var_na *id, const var_na *file, libcrun_error_t *err)
{
  ត្រឡប់ libcrun_container_add_or_remove_mounts_from_file (context, id, file, false, err);
}
