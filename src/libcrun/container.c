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

enum
{
  SYNC_SOCKET_SYNC_MESSAGE,
  SYNC_SOCKET_ERROR_MESSAGE,
  SYNC_SOCKET_WARNING_MESSAGE,
  SYNC_SOCKET_DEBUG_MESSAGE,
};

struct container_entrypoint_s
{
  libcrun_container_t *container;
  libcrun_context_t *context;
  целое has_terminal_socket_pair;
  целое terminal_socketpair[2];

  /* Used by log_write_to_sync_socket.  */
  целое sync_socket;

  целое seccomp_fd;
  целое seccomp_receiver_fd;
  целое console_socket_fd;

  целое hooks_out_fd;
  целое hooks_err_fd;

  /* Absolute bundle path, to be reported to hooks run in the container.  */
  const char *bundle;

  struct custom_handler_instance_s *custom_handler;
};

struct sync_socket_message_s
{
  целое type;
  целое error_value;
  char message[512];
};

typedef runtime_spec_schema_defs_hook hook;

// linux hooks
char *hooks[] = {
  "prestart",
  "createRuntime",
  "createContainer",
  "startContainer",
  "poststart",
  "poststop"
};

// linux namespaces
static char *namespaces[] = {
  "cgroup",
  "ipc",
  "mount",
  "network",
  "pid",
  "user",
  "uts"
};

static char *actions[] = {
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

static char *operators[] = {
  "SCMP_CMP_NE",
  "SCMP_CMP_LT",
  "SCMP_CMP_LE",
  "SCMP_CMP_EQ",
  "SCMP_CMP_GE",
  "SCMP_CMP_GT",
  "SCMP_CMP_MASKED_EQ",
};

static char *archs[] = {
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

static char *mempolicy_modes[] = {
  "MPOL_DEFAULT",
  "MPOL_PREFERRED",
  "MPOL_BIND",
  "MPOL_INTERLEAVE",
  "MPOL_LOCAL",
  "MPOL_PREFERRED_MANY",
  "MPOL_WEIGHTED_INTERLEAVE"
};

static char *mempolicy_flags[] = {
  "MPOL_F_NUMA_BALANCING",
  "MPOL_F_RELATIVE_NODES",
  "MPOL_F_STATIC_NODES"
};

static char *potentially_unsafe_annotations[] = {
  "module.wasm.image/variant",
  "io.kubernetes.cri.container-type",
  "run.oci.",
  "org.criu.",
  "krun.",
};

#define SYNC_SOCKET_MESSAGE_LEN(x, l) (offsetof (struct sync_socket_message_s, message) + l)

static целое
sync_socket_write_msg (целое fd, целое verbosity, целое err_value, const char *log_msg)
{
  целое ret;
  size_t err_len;
  struct sync_socket_message_s msg;
  switch (verbosity)
    {
    case LIBCRUN_VERBOSITY_DEBUG:
      msg.type = SYNC_SOCKET_DEBUG_MESSAGE;
      break;
    case LIBCRUN_VERBOSITY_WARNING:
      msg.type = SYNC_SOCKET_WARNING_MESSAGE;
      break;
    case LIBCRUN_VERBOSITY_ERROR:
      msg.type = SYNC_SOCKET_ERROR_MESSAGE;
      break;
    }
  msg.error_value = err_value;

  если (fd < 0)
    вернуть 0;

  err_len = strlen (log_msg);
  если (err_len >= sizeof (msg.message))
    err_len = sizeof (msg.message) - 1;

  memcpy (msg.message, log_msg, err_len);
  msg.message[err_len] = '\0';

  ret = TEMP_FAILURE_RETRY (write (fd, &msg, SYNC_SOCKET_MESSAGE_LEN (msg, err_len + 1)));
  если (UNLIKELY (ret < 0))
    вернуть -1;

  вернуть 0;
}

static целое
sync_socket_write_error (целое fd, libcrun_error_t *out_err)
{
  если (fd < 0)
    вернуть 0;
  вернуть sync_socket_write_msg (fd, false, (*out_err)->status, (*out_err)->msg);
}

static void
log_write_to_sync_socket (целое errno_, const char *msg, целое verbosity, void *arg)
{
  struct container_entrypoint_s *entrypoint_args = arg;
  целое fd = entrypoint_args->sync_socket;

  если (fd < 0)
    вернуть;

  если (sync_socket_write_msg (fd, verbosity, errno_, msg) < 0)
    log_write_to_stderr (errno_, msg, verbosity, arg);
}

static bool
is_memory_limit_too_low (runtime_spec_schema_config_schema *def)
{
  const long memory_limit_too_low = 1024 * 1024;

  если (def->linux == NULL || def->linux->resources == NULL)
    вернуть false;

  если (def->linux->resources->memory
      && def->linux->resources->memory->limit_present
      && def->linux->resources->memory->limit < memory_limit_too_low)
    вернуть true;

  если (def->linux->resources->unified)
    {
      size_t i;

      для (i = 0; i < def->linux->resources->unified->len; i++)
        если (strcmp (def->linux->resources->unified->keys[i], "memory.max") == 0)
          {
            long limit;

            errno = 0;
            limit = strtol (def->linux->resources->unified->values[i], NULL, 10);
            если (errno != 0)
              вернуть false;
            если (limit < memory_limit_too_low)
              вернуть true;
          }
    }

  вернуть false;
}

static целое
sync_socket_wait_sync (libcrun_context_t *context, целое fd, bool flush, libcrun_error_t *err)
{
  struct sync_socket_message_s msg;

  если (fd < 0)
    вернуть 0;

  пока (true)
    {
      целое ret;

      ret = TEMP_FAILURE_RETRY (read (fd, &msg, sizeof (msg)));
      если (UNLIKELY (ret < 0))
        {
          если (flush)
            вернуть 0;
          вернуть crun_make_error (err, errno, "read from sync socket");
        }

      если (ret == 0)
        {
          если (flush)
            вернуть 0;

          вернуть crun_make_error (err, 0, "read from the init process");
        }

      если (! flush && msg.type == SYNC_SOCKET_SYNC_MESSAGE)
        вернуть 0;
      иначе если (msg.type == SYNC_SOCKET_DEBUG_MESSAGE)
        {
          если (context)
            context->output_handler (msg.error_value, msg.message, LIBCRUN_VERBOSITY_DEBUG, context->output_handler_arg);
          continue;
        }
      иначе если (msg.type == SYNC_SOCKET_WARNING_MESSAGE)
        {
          если (context)
            context->output_handler (msg.error_value, msg.message, LIBCRUN_VERBOSITY_WARNING, context->output_handler_arg);
          continue;
        }
      иначе если (msg.type == SYNC_SOCKET_ERROR_MESSAGE)
        вернуть crun_make_error (err, msg.error_value, "%s", msg.message);
    }
}

static целое
sync_socket_send_sync (целое fd, bool flush_errors, libcrun_error_t *err)
{
  целое ret;
  struct sync_socket_message_s msg = {
    0,
  };
  msg.type = SYNC_SOCKET_SYNC_MESSAGE;

  если (fd < 0)
    вернуть 0;

  ret = TEMP_FAILURE_RETRY (write (fd, &msg, SYNC_SOCKET_MESSAGE_LEN (msg, 0)));
  если (UNLIKELY (ret < 0))
    {
      если (flush_errors)
        {
          целое saved_errno = errno;
          ret = TEMP_FAILURE_RETRY (read (fd, &msg, sizeof (msg)));
          если (ret >= 0 && msg.type == SYNC_SOCKET_ERROR_MESSAGE)
            вернуть crun_make_error (err, msg.error_value, "%s", msg.message);
          errno = saved_errno;
        }
      вернуть crun_make_error (err, errno, "write to sync socket");
    }

  вернуть 0;
}

static bool
has_cap_sys_ptrace ()
{
#ifdef HAVE_CAP
  struct __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  struct __user_cap_data_struct data[2] = { { 0 } };

  если (UNLIKELY (capget (&hdr, data) < 0))
    вернуть true;

  вернуть (data[0].effective & (1U << CAP_SYS_PTRACE)) != 0;
#иначе
  вернуть geteuid () == 0;
#endif
}

static libcrun_container_t *
make_container (runtime_spec_schema_config_schema *container_def, const char *path, const char *config)
{
  libcrun_container_t *container = xmalloc0 (sizeof (*container));
  container->container_def = container_def;

  container->host_uid = geteuid ();
  container->host_gid = getegid ();
  container->host_has_cap_sys_ptrace = has_cap_sys_ptrace ();
  container->proc_fd = -1;

  container->annotations = make_string_map_from_json (container_def->annotations);

  /* An empty namespace path means that a new namespace is to be created,
     the same as if it is not specified.  */
  если (container_def->linux)
    {
      size_t i;

      для (i = 0; i < container_def->linux->namespaces_len; i++)
        если (container_def->linux->namespaces[i]->path && container_def->linux->namespaces[i]->path[0] == '\0')
          {
            free (container_def->linux->namespaces[i]->path);
            container_def->linux->namespaces[i]->path = NULL;
          }
    }

  если (path)
    container->config_file = xstrdup (path);
  если (config)
    container->config_file_content = xstrdup (config);

  вернуть container;
}

libcrun_container_t *
libcrun_container_load_from_memory (const char *json, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free char *oci_error = NULL;
  container_def = runtime_spec_schema_config_schema_parse_data (json, NULL, &oci_error);
  если (container_def == NULL)
    {
      crun_make_error (err, 0, "load: `%s`", oci_error);
      вернуть NULL;
    }
  вернуть make_container (container_def, NULL, json);
}

libcrun_container_t *
libcrun_container_load_from_file (const char *path, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free char *oci_error = NULL;
  cleanup_free char *content = NULL;
  size_t len;
  целое ret;

  libcrun_debug ("Loading container from config file: `%s`", path);
  ret = read_all_file (path, &content, &len, err);
  если (UNLIKELY (ret < 0))
    вернуть NULL;

  container_def = runtime_spec_schema_config_schema_parse_data (content, NULL, &oci_error);
  если (container_def == NULL)
    {
      crun_make_error (err, 0, "load `%s`: %s", path, oci_error);
      вернуть NULL;
    }
  вернуть make_container (container_def, NULL, content);
}

const char *
libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err)
{
  struct parser_context ctx = { 0, stderr };
  parser_error gen_err = NULL;

  если (container == NULL)
    {
      crun_make_error (err, EINVAL, "no container specified");
      вернуть NULL;
    }

  если (container->config_file_content == NULL && container->container_def)
    {
      container->config_file_content = runtime_spec_schema_config_schema_generate_json (container->container_def, &ctx, &gen_err);
      если (container->config_file_content == NULL)
        {
          crun_make_error (err, 0, "cannot generate the configuration JSON: %s", gen_err ?: "unknown error");
          free (gen_err);
          вернуть NULL;
        }
      free (gen_err);
    }

  если (container->config_file_content == NULL)
    {
      crun_make_error (err, 0, "the container has no configuration");
      вернуть NULL;
    }

  вернуть container->config_file_content;
}

const char *
libcrun_container_get_annotation (libcrun_container_t *container, const char *key)
{
  если (container == NULL)
    вернуть NULL;
  вернуть find_string_map_value (container->annotations, key);
}

size_t
libcrun_container_get_annotations_len (libcrun_container_t *container)
{
  если (container == NULL || container->annotations == NULL)
    вернуть 0;
  вернуть string_map_size (container->annotations);
}

целое
libcrun_container_get_annotation_at (libcrun_container_t *container, size_t index, const char **key,
                                     const char **value)
{
  если (container == NULL)
    {
      errno = ERANGE;
      вернуть -1;
    }
  вернуть string_map_get_at (container->annotations, index, key, value);
}

uid_t
libcrun_container_get_uid (libcrun_container_t *container)
{
  если (container == NULL)
    вернуть (uid_t) -1;
  вернуть container->host_uid;
}

gid_t
libcrun_container_get_gid (libcrun_container_t *container)
{
  если (container == NULL)
    вернуть (gid_t) -1;
  вернуть container->host_gid;
}

void
libcrun_container_free (libcrun_container_t *ctr)
{
  если (ctr == NULL)
    вернуть;

  если (ctr->cleanup_private_data)
    ctr->cleanup_private_data (ctr->private_data);

  если (ctr->container_def)
    free_runtime_spec_schema_config_schema (ctr->container_def);

  free_string_map (ctr->annotations);

  если (ctr->proc_fd >= 0)
    close (ctr->proc_fd);

  free (ctr->config_file_content);
  free (ctr->config_file);
  free (ctr);
}

libcrun_context_t *
libcrun_context_new (const char *id, const char *state_root, libcrun_error_t *err)
{
  libcrun_context_t *ctx = xmalloc0 (sizeof (*ctx));

  /* Go through the setters so that the owned_* copies and the borrowed
     pointers stay in sync and are released by libcrun_context_free.  */
  libcrun_context_set_id (ctx, id);
  libcrun_context_set_state_root (ctx, state_root);
  libcrun_context_set_bundle (ctx, ".");
  ctx->fifo_exec_wait_fd = -1;

  если (libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, id, NULL, err) < 0)
    {
      libcrun_context_free (ctx);
      вернуть NULL;
    }

  ctx->handler_manager = libcrun_handler_manager_create (err);
  если (ctx->handler_manager == NULL)
    {
      libcrun_context_free (ctx);
      вернуть NULL;
    }

#ifdef CRUN_LIBDIR
  если (access (CRUN_LIBDIR "/handlers", F_OK) == 0)
    {
      если (libcrun_handler_manager_load_directory (ctx->handler_manager, CRUN_LIBDIR "/handlers", err) < 0)
        {
          libcrun_context_free (ctx);
          вернуть NULL;
        }
    }
#endif

  вернуть ctx;
}

void
libcrun_context_free (libcrun_context_t *ctx)
{
  если (ctx == NULL)
    вернуть;

  free (ctx->owned_state_root);
  free (ctx->owned_id);
  free (ctx->owned_bundle);
  free (ctx->owned_console_socket);
  free (ctx->owned_pid_file);
  free (ctx->owned_notify_socket);
  free (ctx->owned_handler);

  если (ctx->handler_manager)
    handler_manager_free (ctx->handler_manager);

  free (ctx);
}

#define LIBCRUN_CONTEXT_SET_STRING(field)                  \
  do                                                       \
    {                                                      \
      free (ctx->owned_##field);                           \
      ctx->owned_##field = value ? xstrdup (value) : NULL; \
      ctx->field = ctx->owned_##field;                     \
  } пока (0)

void
libcrun_context_set_id (libcrun_context_t *ctx, const char *value)
{
  LIBCRUN_CONTEXT_SET_STRING (id);
}

void
libcrun_context_set_state_root (libcrun_context_t *ctx, const char *value)
{
  LIBCRUN_CONTEXT_SET_STRING (state_root);
}

void
libcrun_context_set_bundle (libcrun_context_t *ctx, const char *value)
{
  LIBCRUN_CONTEXT_SET_STRING (bundle);
}

void
libcrun_context_set_console_socket (libcrun_context_t *ctx, const char *value)
{
  LIBCRUN_CONTEXT_SET_STRING (console_socket);
}

void
libcrun_context_set_pid_file (libcrun_context_t *ctx, const char *value)
{
  LIBCRUN_CONTEXT_SET_STRING (pid_file);
}

void
libcrun_context_set_notify_socket (libcrun_context_t *ctx, const char *value)
{
  LIBCRUN_CONTEXT_SET_STRING (notify_socket);
}

void
libcrun_context_set_handler (libcrun_context_t *ctx, const char *value)
{
  LIBCRUN_CONTEXT_SET_STRING (handler);
}

#undef LIBCRUN_CONTEXT_SET_STRING

const char *
libcrun_context_get_pid_file (libcrun_context_t *ctx)
{
  вернуть ctx->pid_file;
}

const char *
libcrun_context_get_state_root (libcrun_context_t *ctx)
{
  если (ctx == NULL)
    вернуть NULL;
  вернуть ctx->state_root;
}

void
libcrun_context_set_preserve_fds (libcrun_context_t *ctx, целое preserve_fds)
{
  ctx->preserve_fds = preserve_fds;
}

целое
libcrun_context_get_preserve_fds (libcrun_context_t *ctx)
{
  вернуть ctx->preserve_fds;
}

void
libcrun_context_set_listen_fds (libcrun_context_t *ctx, целое listen_fds)
{
  ctx->listen_fds = listen_fds;
}

void
libcrun_context_set_args (libcrun_context_t *ctx, целое argc, char **argv)
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

целое
libcrun_context_init_logging (libcrun_context_t *ctx, const char *log, libcrun_error_t *err)
{
  вернуть libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, ctx->id, log, err);
}

static целое
block_signals (libcrun_error_t *err)
{
  целое ret;
  sigset_t mask;
  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, NULL);
  если (UNLIKELY (ret < 0))
    вернуть crun_make_error (err, errno, "sigprocmask");
  вернуть 0;
}

static целое
unblock_signals (libcrun_error_t *err)
{
  целое i;
  целое ret;
  sigset_t mask;
  struct sigaction act = {};

  sigfillset (&mask);
  ret = sigprocmask (SIG_UNBLOCK, &mask, NULL);
  если (UNLIKELY (ret < 0))
    вернуть crun_make_error (err, errno, "sigprocmask");

  act.sa_handler = SIG_DFL;
  для (i = 0; i < NSIG; i++)
    {
      ret = sigaction (i, &act, NULL);
      если (ret < 0 && errno != EINVAL)
        вернуть crun_make_error (err, errno, "sigaction");
    }

  вернуть 0;
}

/* must be used on the host before pivot_root(2).  */
static целое
initialize_security (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, libcrun_error_t *err)
{
  целое ret;

  ret = libcrun_init_caps (container, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (UNLIKELY (proc == NULL))
    вернуть 0;

  если (proc->apparmor_profile)
    {
      ret = libcrun_initialize_apparmor (err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  ret = libcrun_initialize_selinux (container, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  вернуть 0;
}

/* Emit the "annotations" map.  Returns a json_gen_status; callers can wrap
   the call with GEN_OR_FAIL.  */
static целое
gen_annotations (json_gen_ctx *gen, json_map_string_string *annotations)
{
  size_t i;
  целое r;

  GEN_KEY (gen, "annotations");
  GEN_OR_FAIL (json_gen_map_open (gen));

  для (i = 0; i < annotations->len; i++)
    {
      /* An annotation whose value is null in the document is parsed into a
         NULL, which cannot be emitted as a JSON string; leave it out.  */
      если (annotations->keys[i] == NULL || annotations->values[i] == NULL)
        continue;

      GEN_STR (gen, annotations->keys[i]);
      GEN_STR (gen, annotations->values[i]);
    }

  вернуть json_gen_map_close (gen);

gen_error:
  вернуть r;
}

/* Return the bundle path to report in the container state.  crun always
   chdir()s into the bundle, so fall back to getcwd() when the caller has no
   absolute bundle path, as can happen when libcrun is used as a library.
   *ALLOCATED must be freed by the caller.  */
static const char *
get_bundle_path (const char *bundle, char **allocated)
{
  если (bundle != NULL && bundle[0] == '/')
    вернуть bundle;

  *allocated = getcwd (NULL, 0);
  если (*allocated == NULL)
    OOM ();
  вернуть *allocated;
}

static целое
do_hooks (runtime_spec_schema_config_schema *def, pid_t pid, const char *id, bool keep_going, const char *bundle,
          const char *cwd, const char *status, const char *name, hook **hooks, size_t hooks_len, целое out_fd,
          целое err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  size_t i, stdin_len;
  целое r, ret;
  char *stdin = NULL;
  cleanup_free char *bundle_allocated = NULL;
  const char *rootfs = def->root ? def->root->path : "";
  cleanup_json_gen json_gen_ctx *gen = NULL;

  bundle = get_bundle_path (bundle, &bundle_allocated);

  если (! json_gen_init (&gen, NULL))
    вернуть crun_make_error (err, 0, "json_gen_init failed");

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

  если (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, (const char **) &stdin, &stdin_len));

  ret = 0;

  bool error_created = false;
  для (i = 0; i < hooks_len; i++)
    {
      char **env = environ;

      если (hooks[i]->env)
        env = hooks[i]->env;

      /* Release the error from the previous iteration, if any.  */
      если (error_created)
        {
          crun_error_release (err);
          error_created = false;
        }

      ret = run_process_with_stdin_timeout_envp (hooks[i]->path, hooks[i]->args, cwd, hooks[i]->timeout, env,
                                                 stdin, stdin_len, out_fd, err_fd, can_ignore_chdir_errors, err);
      если (UNLIKELY (ret < 0))
        error_created = true;

      если (UNLIKELY (ret != 0))
        {
          если (keep_going)
            libcrun_warning ("error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
          иначе
            {
              libcrun_error (0, "error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
              break;
            }
        }
    }

  вернуть ret;

gen_error:
  вернуть json_gen_error_to_crun_error (r, err);
}

static целое
get_json_gen_result (json_gen_ctx *gen, char **out, size_t *out_len)
{
  const char *buf = NULL;
  size_t buf_len = 0;
  целое r;

  r = json_gen_get_buf (gen, &buf, &buf_len);
  если (UNLIKELY (r != json_gen_status_ok))
    вернуть r;

  *out_len = buf_len;

  *out = xmalloc (buf_len + 1);
  memcpy (*out, buf, buf_len);
  (*out)[buf_len] = '\0';

  вернуть json_gen_status_ok;
}

static целое
get_seccomp_receiver_fd_payload (libcrun_container_t *container, const char *status, pid_t own_pid,
                                 char **seccomp_fd_payload, size_t *seccomp_fd_payload_len, libcrun_error_t *err)
{
  целое r;
  json_gen_ctx *gen = NULL;
  runtime_spec_schema_config_schema *def = container->container_def;
  const char *const OCI_VERSION = "0.2.0";

  если (! json_gen_init (&gen, NULL))
    вернуть crun_make_error (err, 0, "json_gen_init failed");

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

  если (def && def->linux && def->linux->seccomp)
    {
      const char *metadata = def->linux->seccomp->listener_metadata;

      если (metadata)
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

  если (container->context && container->context->id)
    {
      GEN_KEY (gen, "id");
      GEN_STR (gen, container->context->id);
    }

  GEN_KEY (gen, "status");
  GEN_STR (gen, status);

  GEN_KEY (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  если (container->context && container->context->bundle)
    {
      GEN_KEY (gen, "bundle");
      GEN_STR (gen, container->context->bundle);
    }

  если (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));
  /* End state.  */

  GEN_OR_FAIL (json_gen_map_close (gen));

  r = get_json_gen_result (gen, seccomp_fd_payload, seccomp_fd_payload_len);

gen_error:
  json_gen_free (gen);

  вернуть json_gen_error_to_crun_error (r, err);
}

static целое
send_sync_cb (void *data, libcrun_error_t *err)
{
  целое sync_socket_fd = *((целое *) data);
  целое ret;

  /* sync 2.  */
  ret = sync_socket_send_sync (sync_socket_fd, false, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  /* sync 3.  */
  вернуть sync_socket_wait_sync (NULL, sync_socket_fd, false, err);
}

static целое
maybe_chown_std_streams (uid_t container_uid, gid_t container_gid, libcrun_error_t *err)
{
  целое ret, i;

  для (i = 0; i < 3; i++)
    {
      если (! isatty (i))
        {
          struct stat statbuf;
          ret = fstat (i, &statbuf);
          если (UNLIKELY (ret < 0))
            {
              если (errno == EBADF)
                continue;
              вернуть crun_make_error (err, errno, "fstat fd `%d`", i);
            }

          /* Skip chown for device files */
          если (S_ISCHR (statbuf.st_mode) || S_ISBLK (statbuf.st_mode))
            continue;

          ret = fchown (i, container_uid, container_gid);
          если (UNLIKELY (ret < 0))
            {
              /* EINVAL means the user is not mapped in the current userns.
                 Ignore EPERM and EROFS as well as there is no reason to fail
                 so early, and let the container payload deal with it.
                 EBADF means fd is closed.
              */
              если (errno == EINVAL || errno == EPERM || errno == EROFS || errno == EBADF)
                continue;

              вернуть crun_make_error (err, errno, "fchown std stream `%i`", i);
            }
        }
    }
  вернуть 0;
}

целое
libcrun_container_notify_handler (struct container_entrypoint_s *args,
                                  enum handler_configure_phase phase,
                                  libcrun_container_t *container, const char *rootfs,
                                  libcrun_error_t *err)
{
  struct custom_handler_s *h;

  если (args->custom_handler == NULL)
    вернуть 0;

  h = args->custom_handler->vtable;
  если (h == NULL || h->configure_container == NULL)
    вернуть 0;

  вернуть h->configure_container (args->custom_handler->cookie, phase,
                                 args->context, container,
                                 rootfs, err);
}

/* Resolve and normalize the container rootfs path.  */
static целое
resolve_rootfs_path (libcrun_container_t *container, char **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  если (def->root && def->root->path)
    {
      *rootfs = realpath (def->root->path, NULL);
      если (UNLIKELY (*rootfs == NULL))
        {
          целое saved_errno = errno;

          /* If realpath failed for any reason, try the relative directory.  */
          если (def->root->path[0] == '/')
            {
              cleanup_free char *cwd = NULL;
              libcrun_error_t tmp_err = NULL;
              ssize_t len;
              целое proc_fd;

              proc_fd = libcrun_get_cached_proc_fd (container, &tmp_err);
              если (LIKELY (proc_fd >= 0))
                {
                  len = safe_readlinkat (proc_fd, "self/cwd", &cwd, 0, &tmp_err);
                  если (LIKELY (len > 0))
                    {
                      /* If the rootfs is under the current working directory, just use its relative path.  */
                      если (has_dir_prefix (def->root->path, cwd))
                        {
                          const char *it = consume_slashes (def->root->path + len);
                          если (LIKELY (*it))
                            {
                              целое ret = faccessat (AT_FDCWD, it, F_OK, 0);
                              если (LIKELY (ret == 0))
                                *rootfs = xstrdup (it);
                            }
                        }
                    }
                }
              crun_error_release (&tmp_err);
            }

          если (*rootfs == NULL)
            {
              если (saved_errno == ENOENT)
                вернуть crun_make_error (err, ENOENT, "the rootfs path `%s` does not exist", def->root->path);

              *rootfs = xstrdup (def->root->path);
            }
        }
    }
  вернуть 0;
}

/* Initialize the environment variables.  */
static целое
setup_environment (runtime_spec_schema_config_schema *def, uid_t container_uid, libcrun_error_t *err)
{
  целое ret;

  если (clearenv ())
    вернуть crun_make_error (err, errno, "clearenv");

  если (def->process)
    {
      size_t i;

      для (i = 0; i < def->process->env_len; i++)
        {
          /* A null element of the array of variables is parsed into a NULL,
             which putenv(3) cannot be handed.  */
          если (def->process->env[i] == NULL)
            вернуть crun_make_error (err, EINVAL, "environment variable is not specified");

          если (putenv (def->process->env[i]) < 0)
            вернуть crun_make_error (err, errno, "putenv `%s`", def->process->env[i]);
        }
    }

  если (is_empty_string (getenv ("HOME")))
    {
      ret = set_home_env (container_uid);
      если (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          если (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            вернуть crun_make_error (err, errno, "setenv HOME");
        }
    }

  вернуть 0;
}

/* Configure terminal and console for interactive containers.  */
static целое
setup_terminal (struct container_entrypoint_s *entrypoint_args, libcrun_container_t *container,
                целое console_socket, целое console_socketpair, целое has_terminal, libcrun_error_t *err)
{
  cleanup_close целое terminal_fd = -1;
  целое ret;

  если (! has_terminal)
    вернуть 0;

  fflush (stderr);

  если (console_socket >= 0 || (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0))
    {
      terminal_fd = libcrun_set_terminal (container, err);
      если (UNLIKELY (terminal_fd < 0))
        вернуть terminal_fd;
    }

  если (console_socket >= 0)
    {
      ret = send_fd_to_socket (console_socket, terminal_fd, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }
  иначе если (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0)
    {
      ret = send_fd_to_socket (console_socketpair, terminal_fd, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  вернуть 0;
}

/* Resolve and validate the container's executable path.  */
static целое
setup_executable_path (struct container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                       char **exec_path, libcrun_error_t *err)
{
  целое ret;

  если (def->process && def->process->args)
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      если (UNLIKELY (ret < 0))
        {
          если (entrypoint_args->custom_handler == NULL && crun_error_get_errno (err) == ENOENT)
            вернуть ret;

          /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
             once the process switched to the use that runs in the container.  This might be necessary
             when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
             is not honored.  */
          crun_error_release (err);
        }
    }

  вернуть 0;
}

/* Apply security settings for the container process.  */
static целое
apply_security_settings (struct container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                         libcrun_container_t *container, pid_t own_pid, libcrun_error_t *err)
{
  целое ret;
  runtime_spec_schema_config_schema_process_capabilities *capabilities;
  целое no_new_privs;

  если (def->process && ! def->process->no_new_privileges)
    {
      char **seccomp_flags = NULL;
      size_t seccomp_flags_len = 0;
      cleanup_free char *seccomp_fd_payload = NULL;
      size_t seccomp_fd_payload_len = 0;

      если (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      если (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          если (UNLIKELY (ret < 0))
            вернуть ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  capabilities = def->process ? def->process->capabilities : NULL;
  no_new_privs = def->process ? def->process->no_new_privileges : 1;
  ret = libcrun_set_caps (capabilities, container->container_uid, container->container_gid, no_new_privs,
                          entrypoint_args->custom_handler != NULL, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  вернуть 0;
}

/* Create the process that joins the existing PID namespace now that the
   host mounts are not visible anymore, and continue the setup from there.  */
static целое
do_delayed_pidns_fork (целое sync_socket, pid_t *own_pid, libcrun_error_t *err)
{
  pid_t pid;
  целое ret;

  pid = fork ();
  если (UNLIKELY (pid < 0))
    вернуть crun_make_error (err, errno, "fork");

  если (pid)
    {
      /* Report the new PID to the parent and exit immediately.  */
      ret = sync_socket_send_sync (sync_socket, false, err);
      если (LIKELY (ret == 0))
        {
          ret = TEMP_FAILURE_RETRY (write (sync_socket, &pid, sizeof (pid)));
          если (LIKELY (ret == sizeof (pid)))
            _safe_exit (EXIT_SUCCESS);
        }

      kill (pid, SIGKILL);
      _safe_exit (EXIT_FAILURE);
    }

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (read (sync_socket, own_pid, sizeof (*own_pid)));
  если (UNLIKELY (ret != sizeof (*own_pid)))
    {
      если (ret >= 0)
        errno = 0;
      вернуть crun_make_error (err, errno, "read from sync socket");
    }

  вернуть 0;
}

struct delayed_pidns_fork_s
{
  libcrun_container_t *container;
  целое sync_socket;
  pid_t *own_pid;
};

static целое
delayed_pidns_fork_cb (void *data, libcrun_error_t *err)
{
  struct delayed_pidns_fork_s *d = data;

  если (! libcrun_container_delayed_pidns_fork (d->container))
    вернуть 0;

  вернуть do_delayed_pidns_fork (d->sync_socket, d->own_pid, err);
}

static целое
container_init_setup (void *args, pid_t *own_pid, char *notify_socket,
                      целое sync_socket, char **exec_path, libcrun_error_t *err)
{
  struct container_entrypoint_s *entrypoint_args = args;
  libcrun_container_t *container = entrypoint_args->container;
  bool chdir_done = false;
  целое ret;
  целое has_terminal;
  cleanup_close целое console_socket = -1;
  cleanup_close целое console_socketpair = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free char *rootfs = NULL;
  struct delayed_pidns_fork_s delayed_pidns_fork = {
    .container = container,
    .sync_socket = sync_socket,
    .own_pid = own_pid,
  };

  ret = initialize_security (container, def->process, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = libcrun_configure_network (container, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = resolve_rootfs_path (container, &rootfs, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  /* Configure terminal socket pair for container communication.  */
  если (entrypoint_args->terminal_socketpair[0] >= 0)
    {
      close_and_reset (&entrypoint_args->terminal_socketpair[0]);
      console_socketpair = entrypoint_args->terminal_socketpair[1];
    }

  /* sync 1.  */
  ret = sync_socket_wait_sync (NULL, sync_socket, false, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  has_terminal = container->container_def->process && container->container_def->process->terminal;
  если (has_terminal && entrypoint_args->context->console_socket)
    console_socket = entrypoint_args->console_socket_fd;

  ret = libcrun_set_sysctl (container, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_BEFORE_MOUNTS, container, rootfs, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = libcrun_do_pivot_root (container, entrypoint_args->context->no_pivot, &rootfs, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  /* sync 2 and 3 are sent as part of libcrun_set_mounts.  */
  ret = libcrun_set_mounts (entrypoint_args, container, rootfs, send_sync_cb, &sync_socket, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  /* The createContainer hooks MUST run after the mounts are set up but before
     the pivot_root.  When these hooks are present can_use_open_tree_namespace()
     forces the deferred-pivot path, so the actual pivot happens later in
     libcrun_finalize_mounts().  */
  если (def->hooks && def->hooks->create_container_len)
    {
      libcrun_error_t tmp_err = NULL;
      целое in_userns = check_running_in_user_namespace (&tmp_err);
      если (tmp_err)
        crun_error_release (&tmp_err);
      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, NULL, "created", "createContainer",
                      (hook **) def->hooks->create_container,
                      def->hooks->create_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      если (UNLIKELY (ret != 0))
        вернуть ret;
    }

  ret = libcrun_finalize_mounts (entrypoint_args, container, rootfs, delayed_pidns_fork_cb, &delayed_pidns_fork, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (def->process)
    {
      ret = libcrun_set_selinux_label (container, def->process, false, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      ret = libcrun_set_apparmor_profile (container, def->process, false, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  ret = mark_or_close_fds_ge_than (container, entrypoint_args->context->preserve_fds + 3, false, err);
  если (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ret = libcrun_reopen_dev_null (err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = maybe_chown_std_streams (container->container_uid, container->container_gid, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = setup_environment (def, container->container_uid, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  /* Set primary process to 1 explicitly if nothing is configured and LISTEN_FD is not set.  */
  если (entrypoint_args->context->listen_fds > 0 && getenv ("LISTEN_PID") == NULL)
    {
      libcrun_warning ("setting LISTEN_PID=1 since no previous configuration was found");
      если (UNLIKELY (setenv ("LISTEN_PID", "1", 1) < 0))
        вернуть crun_make_error (err, errno, "setenv LISTEN_PID");
    }

  /* Attempt to chdir immediately here, before doing the setresuid.  If we fail here, let's
     try again later once the process switched to the user that runs in the container.  */
  если (def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      если (LIKELY (ret == 0))
        chdir_done = true;
      иначе
        crun_error_release (err);
    }

  ret = setsid ();
  если (UNLIKELY (ret < 0))
    вернуть crun_make_error (err, errno, "setsid");

  ret = setup_terminal (entrypoint_args, container, console_socket, console_socketpair, has_terminal, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  close_and_reset (&console_socket);
  close_and_reset (&console_socketpair);

  ret = setup_executable_path (entrypoint_args, def, exec_path, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = libcrun_set_hostname (container, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = libcrun_set_domainname (container, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  если (def->process && def->process->user)
    umask (def->process->user->umask_present ? def->process->user->umask : 0022);

  ret = apply_security_settings (entrypoint_args, def, container, *own_pid, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (UNLIKELY (def->process && def->process->args && *exec_path == NULL))
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      если (UNLIKELY (ret < 0))
        {
          если (entrypoint_args->custom_handler == NULL || is_empty_string (def->process->args[0]))
            вернуть ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          *exec_path = xstrdup (def->process->args[0]);
        }
    }

  /* The chdir was not already performed, so try again now after switching to the UID/GID in the container.  */
  если (! chdir_done && def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  если (notify_socket && putenv (notify_socket) < 0)
    вернуть crun_make_error (err, errno, "putenv `%s`", notify_socket);

  вернуть 0;
}

static целое
open_hooks_output (libcrun_container_t *container, целое *out_fd, целое *err_fd, libcrun_error_t *err)
{
  const char *annotation;

  *err_fd = *out_fd = -1;

  libcrun_debug ("Opening hooks output");
  annotation = find_annotation (container, "run.oci.hooks.stdout");
  если (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stdout` annotation");
      *out_fd = TEMP_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      если (UNLIKELY (*out_fd < 0))
        вернуть crun_make_error (err, errno, "open `%s`", annotation);
    }

  annotation = find_annotation (container, "run.oci.hooks.stderr");
  если (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stderr` annotation");
      *err_fd = TEMP_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      если (UNLIKELY (*err_fd < 0))
        вернуть crun_make_error (err, errno, "open `%s`", annotation);
    }

  вернуть 0;
}

/* The environment the process was started with lives on the initial stack, right after the
   argv strings and before the string AT_EXECFN points to.  clearenv(3) only drops the pointers
   to it, so when there is no execv to replace the stack, erase it or the container process can
   still read it, e.g. through /proc/self/environ.  */
static void
erase_initial_environment (char **argv, целое argc)
{
  char *start = argv[argc - 1] + strlen (argv[argc - 1]) + 1;
  char *end = (char *) getauxval (AT_EXECFN);

  если (end > start)
    memset (start, 0, end - start);
}

static void
rewrite_argv (char **argv, целое argc, const char *name, char **args, size_t args_len)
{
  cleanup_free char *decorated_name = NULL;
  size_t available_len = argv[argc - 1] - argv[0] + strlen (argv[argc - 1]) + 1;
  cleanup_free char *new_argv = NULL;
  size_t i, so_far = 0, needed = 0;

  needed = xasprintf (&decorated_name, "[libcrun:%s]", name) + 1;
  для (i = 0; i < args_len; i++)
    needed += strlen (args[i]) + 1;

  new_argv = xmalloc0 (needed + 1);
  strcpy (new_argv, decorated_name);
  so_far = strlen (decorated_name) + 1;

  если (available_len >= needed)
    {
      для (i = 0; i < args_len; i++)
        {
          strcpy (new_argv + so_far, args[i]);
          so_far += strlen (args[i]) + 1;
        }
    }

  если (so_far >= available_len)
    so_far = available_len - 1;

  memset (argv[0], 0, available_len);
  memcpy (argv[0], new_argv, so_far);
}

/* Entrypoint to the container.  */
static целое
container_init (void *args, char *notify_socket, целое sync_socket, libcrun_error_t *err)
{
  struct container_entrypoint_s *entrypoint_args = args;
  целое ret;
  runtime_spec_schema_config_schema *def = entrypoint_args->container->container_def;
  cleanup_free char *exec_path = NULL;
  __attribute__ ((unused)) cleanup_free char *notify_socket_cleanup = notify_socket;
  pid_t own_pid = 0;

  entrypoint_args->sync_socket = sync_socket;

  crun_set_output_handler (log_write_to_sync_socket, args);

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (read (sync_socket, &own_pid, sizeof (own_pid)));
  если (UNLIKELY (ret != sizeof (own_pid)))
    {
      если (ret >= 0)
        errno = 0;
      вернуть crun_make_error (err, errno, "read from sync socket");
    }

  ret = container_init_setup (args, &own_pid, notify_socket, sync_socket, &exec_path, err);
  если (UNLIKELY (ret < 0))
    {
      /* If it fails to write the error using the sync socket, then fallback
         to stderr.  */
      если (sync_socket_write_error (sync_socket, err) == 0)
        {
          crun_error_release (err);
          /* A return value of > 0 means to fail with an error without err being set so to avoid
             printing it twice.  */
          вернуть 1;
        }
      вернуть ret;
    }

  entrypoint_args->sync_socket = -1;

  ret = unblock_signals (err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  /* sync 4.  */
  ret = sync_socket_send_sync (sync_socket, false, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  close_and_reset (&sync_socket);

  если (entrypoint_args->context->fifo_exec_wait_fd >= 0)
    {
      char buffer[1];
      fd_set read_set;
      cleanup_close целое fd = entrypoint_args->context->fifo_exec_wait_fd;
      entrypoint_args->context->fifo_exec_wait_fd = -1;

      FD_ZERO (&read_set);
      FD_SET (fd, &read_set);
      do
        {
          ret = select (fd + 1, &read_set, NULL, NULL, NULL);
          если (UNLIKELY (ret < 0))
            вернуть crun_make_error (err, errno, "select");

          ret = TEMP_FAILURE_RETRY (read (fd, buffer, sizeof (buffer)));
          если (UNLIKELY (ret < 0))
            вернуть crun_make_error (err, errno, "read from the exec fifo");
      } пока (ret == 0);

      close_and_reset (&entrypoint_args->context->fifo_exec_wait_fd);
    }

  crun_set_output_handler (log_write_to_stderr, NULL);

  если (def->process && def->process->no_new_privileges)
    {
      char **seccomp_flags = NULL;
      size_t seccomp_flags_len = 0;
      cleanup_free char *seccomp_fd_payload = NULL;
      size_t seccomp_fd_payload_len = 0;

      если (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      если (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (entrypoint_args->container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          если (UNLIKELY (ret < 0))
            вернуть ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags,
                                   seccomp_flags_len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  если (UNLIKELY (def->process == NULL))
    вернуть crun_make_error (err, 0, "block `process` not found");

  если (UNLIKELY (exec_path == NULL))
    вернуть crun_make_error (err, 0, "executable path not specified");

  если (def->hooks && def->hooks->start_container_len)
    {
      libcrun_container_t *container = entrypoint_args->container;
      libcrun_error_t tmp_err = NULL;
      целое in_userns = check_running_in_user_namespace (&tmp_err);
      если (tmp_err)
        crun_error_release (&tmp_err);

      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, NULL, "created", "startContainer",
                      (hook **) def->hooks->start_container,
                      def->hooks->start_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      если (UNLIKELY (ret != 0))
        вернуть ret;

      /* Seek stdout/stderr to the end.  If the hooks were using the same files,
         the container process overwrites what was previously written.  */
      (void) lseek (1, 0, SEEK_END);
      (void) lseek (2, 0, SEEK_END);
    }

  если (entrypoint_args->custom_handler)
    {
      /* Files marked with O_CLOEXEC are closed at execv time, so make sure they are closed now.
         This is a best effort operation, because the seccomp filter is already in place and it could
         stop some syscalls used by mark_or_close_fds_ge_than.
      */
      если (entrypoint_args->custom_handler->vtable->close_fds)
        {
          ret = entrypoint_args->custom_handler->vtable->close_fds (entrypoint_args->custom_handler->cookie,
                                                                    entrypoint_args->container,
                                                                    entrypoint_args->context->preserve_fds,
                                                                    err);
        }
      иначе
        {
          ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
        }
      если (UNLIKELY (ret < 0))
        crun_error_release (err);

      prctl (PR_SET_NAME, entrypoint_args->custom_handler->vtable->name);

      если (entrypoint_args->context->argv)
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
      если (UNLIKELY (ret < 0))
        вернуть ret;

      ret = libcrun_set_apparmor_profile (entrypoint_args->container, def->process, true, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      ret = entrypoint_args->custom_handler->vtable->run_func (entrypoint_args->custom_handler->cookie,
                                                               entrypoint_args->container,
                                                               exec_path,
                                                               def->process->args);
      если (ret != 0)
        вернуть crun_make_error (err, 0, "exec container process failed with handler as `%s`", entrypoint_args->custom_handler->vtable->name);

      вернуть ret;
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation since the seccomp profile is already in place now and might block
     some of the syscalls needed by mark_or_close_fds_ge_than.  */
  ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
  если (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, def->process->args));

  если (errno == ENOENT)
    вернуть crun_make_error (err, errno, "exec container process (missing dynamic library?) `%s`", exec_path);

  вернуть crun_make_error (err, errno, "exec container process `%s`", exec_path);
}

static целое
read_container_config_from_state (libcrun_container_t **container, const char *state_root, const char *id,
                                  libcrun_error_t *err)
{
  cleanup_free char *config_file = NULL;
  cleanup_free char *dir = NULL;
  целое ret;

  *container = NULL;

  ret = libcrun_get_state_directory (&dir, state_root, id, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = append_paths (&config_file, err, dir, "config.json", NULL);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  *container = libcrun_container_load_from_file (config_file, err);
  если (*container == NULL)
    вернуть -1;

  вернуть 0;
}

static целое
run_poststop_hooks (arg_unused libcrun_context_t *context, libcrun_container_t *container,
                    runtime_spec_schema_config_schema *def,
                    libcrun_container_status_t *status, const char *state_root, const char *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container_cleanup = NULL;
  целое ret;

  если (def == NULL)
    {
      если (container == NULL)
        {
          ret = read_container_config_from_state (&container_cleanup, state_root, id, err);
          если (UNLIKELY (ret < 0))
            вернуть ret;
          container = container_cleanup;
        }

      def = container->container_def;
    }

  если (def->hooks && def->hooks->poststop_len)
    {
      cleanup_close целое hooks_out_fd = -1;
      cleanup_close целое hooks_err_fd = -1;

      если (container == NULL)
        {
          ret = read_container_config_from_state (&container_cleanup, state_root, id, err);
          если (UNLIKELY (ret < 0))
            вернуть ret;
          container = container_cleanup;
        }

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      ret = do_hooks (def, 0, id, true, status->bundle, status->bundle, "stopped", "poststop", (hook **) def->hooks->poststop,
                      def->hooks->poststop_len, hooks_out_fd, hooks_err_fd, false, err);
      если (UNLIKELY (ret != 0))
        {
          если (ret < 0)
            libcrun_error_report_and_release (err);
          иначе
            libcrun_error (0, "poststop hook failed with exit code: %d", ret);
        }
    }
  вернуть 0;
}

static bool
has_new_pid_namespace (runtime_spec_schema_config_schema *def)
{
  size_t i;

  если (def->linux == NULL || def->linux->namespaces == NULL)
    вернуть false;

  для (i = 0; i < def->linux->namespaces_len; i++)
    {
      если (strcmp (def->linux->namespaces[i]->type, "pid") == 0 && def->linux->namespaces[i]->path == NULL)
        вернуть true;
    }
  вернуть false;
}

/* Delete the container state.  CONTAINER is the container the caller already has
   available, or NULL.  When it is not NULL, the poststop hooks are run even if the
   status file was never written, e.g. when the container creation failed.  */
static целое
container_delete_internal (libcrun_context_t *context, libcrun_container_t *container,
                           runtime_spec_schema_config_schema *def, const char *id, bool force, bool killall,
                           libcrun_error_t *err)
{
  cleanup_cgroup_status struct libcrun_cgroup_status *cgroup_status = NULL;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_container libcrun_container_t *container_cleanup = NULL;
  const char *state_root = context->state_root;
  целое ret;

  ret = libcrun_read_container_status (&status, state_root, id, err);
  если (UNLIKELY (ret < 0))
    {
      если (force && crun_error_get_errno (err) == ENOENT)
        {
          libcrun_error_t tmp_err = NULL;

          crun_error_release (err);

          если (container)
            {
              libcrun_container_status_t partial_status = {
                .bundle = (char *) context->bundle,
              };

              ret = run_poststop_hooks (context, container, def, &partial_status, state_root, id, err);
              если (UNLIKELY (ret < 0))
                crun_error_write_warning_and_release (context->output_handler_arg, &err);
            }

          libcrun_container_delete_status (state_root, id, &tmp_err);
          crun_error_release (&tmp_err);
          вернуть 0;
        }
      crun_error_release (err);
      вернуть libcrun_container_delete_status (state_root, id, err);
    }

  cgroup_status = libcrun_cgroup_make_status (&status);

  если (! force)
    {
      ret = libcrun_is_container_running (&status, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
      если (ret == 1)
        {
          ret = libcrun_status_has_read_exec_fifo (state_root, id, err);
          если (UNLIKELY (ret < 0))
            вернуть ret;
          если (ret == 0)
            вернуть crun_make_error (err, 0, "the container `%s` is not in `created` or `stopped` state", id);

          /* If the container is in "created" state, then do the equivalent of delete --force.  */
          killall = force = true;
        }
    }

  если (def == NULL)
    {
      если (container == NULL)
        {
          ret = read_container_config_from_state (&container_cleanup, state_root, id, err);
          если (UNLIKELY (ret < 0))
            вернуть ret;

          container = container_cleanup;
        }

      def = container->container_def;
    }

  если (killall && force)
    {
      /* If the container has a pid namespace, it is enough to kill the first
         process (pid=1 in the namespace).
      */
      если (has_new_pid_namespace (def))
        {
          ret = libcrun_kill_linux (&status, SIGKILL, err);
          если (UNLIKELY (ret < 0))
            {
              если (crun_error_get_errno (err) != ESRCH)
                вернуть ret;

              crun_error_release (err);
            }
        }
      иначе если (status.cgroup_path)
        {
          ret = libcrun_cgroup_killall (cgroup_status, SIGKILL, err);
          если (UNLIKELY (ret < 0))
            вернуть ret;
        }
    }

  если (def->linux && def->linux->intel_rdt)
    {
      ret = libcrun_destroy_intelrdt (id, def, err);
      если (UNLIKELY (ret < 0))
        libcrun_error_report_and_release (err);
    }

  если (status.cgroup_path)
    {
      /* If the cgroup cannot be destroyed, there are still processes in it.
         Keep the container status, it is the only record of the cgroup, so
         that the delete can be retried.  */
      ret = libcrun_cgroup_destroy (cgroup_status, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  ret = run_poststop_hooks (context, container, def, &status, state_root, id, err);
  если (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  вернуть libcrun_container_delete_status (state_root, id, err);
}

целое
libcrun_container_delete (libcrun_context_t *context, const char *id, bool force, libcrun_error_t *err)
{
  вернуть container_delete_internal (context, NULL, NULL, id, force, true, err);
}

целое
libcrun_container_kill (libcrun_context_t *context, const char *id, const char *signal, libcrun_error_t *err)
{
  целое sig, ret;
  const char *state_root = context->state_root;
  cleanup_container_status libcrun_container_status_t status = {};

  sig = str2sig (signal);
  если (UNLIKELY (sig < 0))
    вернуть crun_make_error (err, 0, "unknown signal `%s`", signal);

  ret = libcrun_read_container_status (&status, state_root, id, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  вернуть libcrun_kill_linux (&status, sig, err);
}

целое
libcrun_container_killall (libcrun_context_t *context, const char *id, const char *signal, libcrun_error_t *err)
{
  целое sig, ret;
  const char *state_root = context->state_root;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_cgroup_status struct libcrun_cgroup_status *cgroup_status = NULL;

  sig = str2sig (signal);
  если (UNLIKELY (sig < 0))
    вернуть crun_make_error (err, 0, "unknown signal `%s`", signal);

  ret = libcrun_read_container_status (&status, state_root, id, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  cgroup_status = libcrun_cgroup_make_status (&status);

  ret = libcrun_cgroup_killall (cgroup_status, sig, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;
  вернуть 0;
}

static целое
write_container_status (libcrun_container_t *container, libcrun_context_t *context,
                        pid_t pid, struct libcrun_cgroup_status *cgroup_status,
                        libcrun_error_t *err)
{
  cleanup_free char *bundle_allocated = NULL;
  const char *bundle = get_bundle_path (context->bundle, &bundle_allocated);
  cleanup_free char *owner = get_user_name (geteuid ());
  char *external_descriptors = libcrun_get_external_descriptors (container);
  char *rootfs = container->container_def->root ? container->container_def->root->path : "";
  char created[35];

  libcrun_container_status_t status = {
    .pid = pid,
    .rootfs = rootfs,
    .bundle = (char *) bundle,
    .created = created,
    .owner = owner,
    .systemd_cgroup = context->systemd_cgroup,
    .detached = context->detach,
    .no_new_keyring = context->no_new_keyring,
    .external_descriptors = external_descriptors,
    .cgroup_path = NULL,
    .scope = NULL,
  };

  get_current_timestamp (created, sizeof (created));

  если (cgroup_status)
    {
      целое ret;

      ret = libcrun_cgroup_get_status (cgroup_status, &status, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  если (external_descriptors == NULL)
    вернуть crun_make_error (err, 0, "invalid internal state.  No external descriptors found");
  вернуть libcrun_write_container_status (context->state_root, context->id, &status, err);
}

static целое
reap_subprocesses (pid_t *main_process, целое *main_process_exit, целое *last_process, libcrun_error_t *err)
{
  *last_process = 0;
  пока (1)
    {
      целое status;
      целое r = waitpid_ignore_stopped (-1, &status, WNOHANG);
      если (r < 0)
        {
          если (errno == EINTR)
            continue;
          если (errno == ECHILD)
            {
              *last_process = 1;
              вернуть 0;
            }
          вернуть crun_make_error (err, errno, "waitpid");
        }
      если (r == 0)
        break;
      если (r != *main_process)
        continue;

      *main_process_exit = get_process_exit_status (status);

      /* Once reaped, the pid can be reused by an unrelated process.  */
      *main_process = 0;
    }
  вернуть 0;
}

static целое
send_sd_notify (const char *ready_str, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  целое ret = sd_notify (0, ready_str);
  если (UNLIKELY (ret < 0))
    вернуть crun_make_error (err, -ret, "sd_notify");

#  если HAVE_SD_NOTIFY_BARRIER
  /* Hard-code a 30 seconds timeout.  Ignore errors.  */
  sd_notify_barrier (0, 30 * 1000000);
#  endif
#иначе
  (void) ready_str;
  (void) err;
#endif
  вернуть 0;
}

static целое
handle_notify_socket (целое notify_socketfd, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  целое ret;
  char buf[256];
  const char *ready_str = "READY=1";

  ret = recvfrom (notify_socketfd, buf, sizeof (buf) - 1, 0, NULL, NULL);
  если (UNLIKELY (ret < 0))
    вернуть crun_make_error (err, errno, "recvfrom notify socket");

  buf[ret] = '\0';
  если (strstr (buf, ready_str))
    {
      ret = send_sd_notify (ready_str, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      вернуть 1;
    }
  вернуть 0;
#иначе
  (void) notify_socketfd;
  (void) err;
  вернуть 1;
#endif
}

struct wait_for_process_args
{
  pid_t pid;
  libcrun_context_t *context;
  целое terminal_fd;
  целое notify_socket;
  целое *container_ready_fd;
  целое seccomp_notify_fd;
  const char *seccomp_notify_plugins;
};

static целое
write_pid_file (const char *pid_file, pid_t pid, libcrun_error_t *err)
{
  char buf[32];
  целое buf_len = snprintf (buf, sizeof (buf), "%d", pid);
  если (UNLIKELY (buf_len >= (целое) sizeof (buf)))
    вернуть crun_make_error (err, 0, "internal error: static buffer too small");
  вернуть write_file_at_with_flags (AT_FDCWD, O_CREAT | O_TRUNC, 0700, pid_file, buf, buf_len, err);
}

#define MAX_EVENTS 10

static bool
is_fd_pollable (целое fd)
{
  struct epoll_event ev = { .events = EPOLLIN };
  cleanup_close целое epollfd = epoll_create1 (EPOLL_CLOEXEC);

  если (epollfd < 0)
    вернуть true;

  вернуть epoll_ctl (epollfd, EPOLL_CTL_ADD, fd, &ev) == 0 || errno != EPERM;
}

static целое
wait_for_process (struct wait_for_process_args *args, libcrun_error_t *err)
{
  cleanup_channel_fd_pair struct channel_fd_pair *from_terminal = NULL;
  cleanup_channel_fd_pair struct channel_fd_pair *to_terminal = NULL;
  целое ret, container_exit_code = 0, last_process;
  cleanup_close целое terminal_fd_from = -1;
  cleanup_close целое terminal_fd_to = -1;
  bool stdin_pollable = false, stdout_pollable = false;
  cleanup_close целое epollfd = -1;
  cleanup_close целое signalfd = -1;
  sigset_t mask;
  целое in_fds[MAX_EVENTS];
  целое in_fds_len = 0;
  целое out_fds[MAX_EVENTS];
  целое out_fds_len = 0;
  size_t i;

  cleanup_seccomp_notify_context struct seccomp_notify_context_s *seccomp_notify_ctx = NULL;

  container_exit_code = 0;

  если (args == NULL || args->context == NULL)
    вернуть crun_make_error (err, 0, "internal error: context is empty");

  для (i = 0; i < MAX_EVENTS; i++)
    {
      in_fds[i] = -1;
      out_fds[i] = -1;
    }

  если (args->context->pid_file)
    {
      ret = write_pid_file (args->context->pid_file, args->pid, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  /* Also exit if there is nothing more to wait for.  */
  если (args->context->detach && args->notify_socket < 0)
    вернуть 0;

  если (args->container_ready_fd)
    {
      ret = 0;
      TEMP_FAILURE_RETRY (write (*args->container_ready_fd, &ret, sizeof (ret)));
      close_and_reset (args->container_ready_fd);
    }

  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, NULL);
  если (UNLIKELY (ret < 0))
    вернуть crun_make_error (err, errno, "sigprocmask");

  signalfd = create_signalfd (&mask, err);
  если (UNLIKELY (signalfd < 0))
    вернуть signalfd;

  ret = reap_subprocesses (&args->pid, &container_exit_code, &last_process, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (last_process)
    вернуть container_exit_code;

  если (args->seccomp_notify_fd >= 0)
    {
      cleanup_free char *state_root = NULL;
      cleanup_free char *oci_config_path = NULL;

      struct libcrun_load_seccomp_notify_conf_s conf;
      memset (&conf, 0, sizeof conf);

      ret = libcrun_get_state_directory (&state_root, args->context->state_root, args->context->id, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      ret = append_paths (&oci_config_path, err, state_root, "config.json", NULL);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      conf.runtime_root_path = state_root;
      conf.name = args->context->id;
      conf.bundle_path = args->context->bundle;
      conf.oci_config_path = oci_config_path;

      ret = set_blocking_fd (args->seccomp_notify_fd, false, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      ret = libcrun_load_seccomp_notify_plugins (&seccomp_notify_ctx,
                                                 args->seccomp_notify_plugins,
                                                 &conf, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      in_fds[in_fds_len++] = args->seccomp_notify_fd;
    }

  если (args->terminal_fd >= 0)
    {
      /* The terminal_fd is dup()ed so that it can be registered with
         epoll multiple times using different masks.  */
      terminal_fd_from = dup (args->terminal_fd);
      если (UNLIKELY (terminal_fd_from < 0))
        вернуть crun_make_error (err, errno, "dup terminal fd");
      terminal_fd_to = dup (args->terminal_fd);
      если (UNLIKELY (terminal_fd_to < 0))
        вернуть crun_make_error (err, errno, "dup terminal fd");

      целое i, non_blocking_fds[] = { terminal_fd_from, terminal_fd_to, 0, 1, -1 };
      для (i = 0; non_blocking_fds[i] >= 0; i++)
        {
          ret = set_blocking_fd (non_blocking_fds[i], false, err);
          если (UNLIKELY (ret < 0))
            вернуть ret;
        }

      /* stdin and stdout can be something epoll does not support, such as
         /dev/null or a regular file.  Such a stdin is not forwarded, and
         such a stdout is written to without waiting for it to be ready.  */
      stdin_pollable = is_fd_pollable (0);
      stdout_pollable = is_fd_pollable (1);

      from_terminal = channel_fd_pair_new (terminal_fd_from, 1, BUFSIZ);
      если (stdin_pollable)
        to_terminal = channel_fd_pair_new (0, terminal_fd_to, BUFSIZ);
    }

  in_fds[in_fds_len++] = signalfd;
  если (args->notify_socket >= 0)
    in_fds[in_fds_len++] = args->notify_socket;
  если (args->terminal_fd >= 0)
    {
      если (stdin_pollable)
        {
          in_fds[in_fds_len++] = 0;
          out_fds[out_fds_len++] = terminal_fd_to;
        }

      in_fds[in_fds_len++] = terminal_fd_from;
      если (stdout_pollable)
        out_fds[out_fds_len++] = 1;
    }

  epollfd = epoll_helper (in_fds, NULL, out_fds, NULL, err);
  если (UNLIKELY (epollfd < 0))
    вернуть epollfd;

  пока (1)
    {
      struct epoll_event events[MAX_EVENTS];
      struct signalfd_siginfo si;
      struct winsize ws;
      целое i, nr_events;
      ssize_t res;

      nr_events = TEMP_FAILURE_RETRY (epoll_wait (epollfd, events, MAX_EVENTS, -1));
      если (UNLIKELY (nr_events < 0))
        вернуть crun_make_error (err, errno, "epoll_wait");

      для (i = 0; i < nr_events; i++)
        {
          если (events[i].data.fd == 0 || events[i].data.fd == terminal_fd_to)
            {
              ret = channel_fd_pair_process (to_terminal, epollfd, err);
              если (UNLIKELY (ret < 0))
                вернуть crun_error_wrap (err, "copy to terminal fd");
            }
          иначе если (events[i].data.fd == 1 || events[i].data.fd == terminal_fd_from)
            {
              ret = channel_fd_pair_process (from_terminal, epollfd, err);
              если (UNLIKELY (ret < 0))
                вернуть crun_error_wrap (err, "copy from terminal fd");
            }
          иначе если (events[i].data.fd == args->seccomp_notify_fd)
            {
              ret = libcrun_seccomp_notify_plugins (seccomp_notify_ctx,
                                                    args->seccomp_notify_fd, err);
              если (UNLIKELY (ret < 0))
                вернуть ret;
            }
          иначе если (events[i].data.fd == args->notify_socket)
            {
              ret = handle_notify_socket (args->notify_socket, err);
              если (UNLIKELY (ret < 0))
                вернуть ret;
              если (ret && args->context->detach)
                вернуть 0;
            }
          иначе если (events[i].data.fd == signalfd)
            {
              res = TEMP_FAILURE_RETRY (read (signalfd, &si, sizeof (si)));
              если (UNLIKELY (res < 0))
                вернуть crun_make_error (err, errno, "read from signalfd");
              если (si.ssi_signo == SIGCHLD)
                {
                  ret = reap_subprocesses (&args->pid, &container_exit_code,
                                           &last_process, err);
                  если (UNLIKELY (ret < 0))
                    вернуть ret;
                  если (last_process)
                    вернуть container_exit_code;
                }
              иначе если (si.ssi_signo == SIGWINCH)
                {
                  /* Ignore the signal if the terminal is not available.  */
                  если (args->terminal_fd > 0)
                    {
                      ret = ioctl (0, TIOCGWINSZ, &ws);
                      если (UNLIKELY (ret < 0))
                        вернуть crun_make_error (err, errno, "ioctl TIOCGWINSZ copy terminal size from stdin");

                      ret = ioctl (args->terminal_fd, TIOCSWINSZ, &ws);
                      если (UNLIKELY (ret < 0))
                        вернуть crun_make_error (err, errno, "ioctl TIOCSWINSZ copy terminal size to pty");
                    }
                }
              иначе если (args->pid > 0)
                {
                  /* Send any other signal to the child process, if it was not reaped yet.  */
                  ret = kill (args->pid, si.ssi_signo);
                }
            }
          иначе
            {
              вернуть crun_make_error (err, 0, "internal error: unknown fd from epoll_wait");
            }
        }
    }

  вернуть 0;
}

static void
flush_fd_to_err (libcrun_context_t *context, целое terminal_fd)
{
  char buf[256];
  целое flags;

  если (terminal_fd < 0 || stderr == NULL)
    вернуть;

  flags = fcntl (terminal_fd, F_GETFL, 0);
  если (flags == -1)
    вернуть;
  если (fcntl (terminal_fd, F_SETFL, flags | O_NONBLOCK) < 0)
    вернуть;

  для (;;)
    {
      целое ret = TEMP_FAILURE_RETRY (read (terminal_fd, buf, sizeof (buf) - 1));
      если (ret <= 0)
        break;
      buf[ret] = '\0';
      если (context->output_handler)
        context->output_handler (0, buf, LIBCRUN_VERBOSITY_ERROR, context->output_handler_arg);
    }
  (void) fcntl (terminal_fd, F_SETFL, flags);
  fflush (stderr);
  fsync (1);
  fsync (2);
}

static целое
cleanup_watch (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
               struct libcrun_cgroup_status *cgroup_status, pid_t init_pid, целое sync_socket,
               целое terminal_fd, libcrun_error_t *err)
{
  const char *oom_message = NULL;
  libcrun_error_t tmp_err = NULL;
  целое ret;

  если (init_pid)
    {
      /* Try to detect whether the cgroup has a OOM.  */
      если (cgroup_status)
        {
          целое has_oom;

          has_oom = libcrun_cgroup_has_oom (cgroup_status, &tmp_err);
          если (has_oom > 0)
            oom_message = "OOM: the memory limit could be too low";
          иначе если (has_oom < 0)
            {
              /* If the detection has failed for any reason, e.g. the cgroup was
                 already deleted by the time it was checked, just ignore the
                 failure.  */
              crun_error_release (&tmp_err);
            }
        }
      /* If the OOM wasn't detected, look into the static configuration.  */
      если (oom_message == NULL && is_memory_limit_too_low (def))
        oom_message = "the memory limit could be too low";

      kill (init_pid, SIGKILL);
      waitpid_ignore_stopped (init_pid, NULL, 0);
    }

  ret = sync_socket_wait_sync (context, sync_socket, true, &tmp_err);
  если (UNLIKELY (ret < 0))
    {
      crun_error_release (err);
      *err = tmp_err;
    }

  если (terminal_fd >= 0)
    flush_fd_to_err (context, terminal_fd);

  если (oom_message)
    вернуть crun_error_wrap (err, "%s", oom_message);

  вернуть -1;
}

/* Find the uid:gid that is mapped to root inside the container user namespace.  */
void
get_root_in_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid, uid_t *uid,
                        gid_t *gid)
{
  *uid = -1;
  *gid = -1;

  /* Not root in the namespace.  */
  если (host_uid)
    вернуть;

  если (def->linux && def->linux->uid_mappings)
    {
      size_t i;

      для (i = 0; i < def->linux->uid_mappings_len; i++)
        если (def->linux->uid_mappings[i]->container_id == 0)
          {
            *uid = def->linux->uid_mappings[i]->host_id;
            break;
          }
    }
  если (def->linux && def->linux->gid_mappings)
    {
      size_t i;

      для (i = 0; i < def->linux->gid_mappings_len; i++)
        если (def->linux->gid_mappings[i]->container_id == 0)
          {
            *gid = def->linux->gid_mappings[i]->host_id;
            break;
          }
    }

  /* If the uid and the gid are not changed, do not attempt any chown.  */
  если (*uid == host_uid && *gid == host_gid)
    *uid = *gid = -1;
}

static целое
get_seccomp_receiver_fd (libcrun_container_t *container, целое *fd, целое *self_receiver_fd, const char **plugins,
                         libcrun_error_t *err)
{
  const char *tmp;
  runtime_spec_schema_config_schema *def = container->container_def;

  *fd = -1;
  *self_receiver_fd = -1;

  tmp = find_annotation (container, "run.oci.seccomp.plugins");
  если (tmp)
    {
      целое fds[2];
      целое ret;

      ret = create_socket_pair (fds, err);
      если (UNLIKELY (ret < 0))
        вернуть crun_error_wrap (err, "create socket pair");

      *fd = fds[0];
      *self_receiver_fd = fds[1];
      *plugins = tmp;
      вернуть 0;
    }

  если (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    tmp = def->linux->seccomp->listener_path;
  иначе
    tmp = find_annotation (container, "run.oci.seccomp.receiver");
  если (tmp == NULL)
    tmp = getenv ("RUN_OCI_SECCOMP_RECEIVER");
  если (tmp)
    {
      если (tmp[0] != '/')
        вернуть crun_make_error (err, 0, "the seccomp receiver `%s` is not an absolute path", tmp);

      *fd = open_unix_domain_client_socket (tmp, 0, err);
      если (UNLIKELY (*fd < 0))
        вернуть crun_error_wrap (err, "open seccomp receiver");
    }

  вернуть 0;
}

static bool
has_seccomp_receiver (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;

  если (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    вернуть true;

  если (find_annotation (container, "run.oci.seccomp.receiver") != NULL || getenv ("RUN_OCI_SECCOMP_RECEIVER") != NULL)
    вернуть true;

  вернуть false;
}

static целое
setup_container_hooks_output (libcrun_container_t *container, runtime_spec_schema_config_schema *def,
                              struct container_entrypoint_s *container_args, целое *hooks_out_fd,
                              целое *hooks_err_fd, libcrun_error_t *err)
{
  целое ret;

  если (def->hooks
      && (def->hooks->prestart_len || def->hooks->poststart_len || def->hooks->create_runtime_len
          || def->hooks->create_container_len || def->hooks->start_container_len))
    {
      ret = open_hooks_output (container, hooks_out_fd, hooks_err_fd, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
      container_args->hooks_out_fd = *hooks_out_fd;
      container_args->hooks_err_fd = *hooks_err_fd;
    }
  вернуть 0;
}

static целое
setup_container_keyring (libcrun_container_t *container, libcrun_context_t *context, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  целое ret;

  если (! context->no_new_keyring)
    {
      const char *label = NULL;

      libcrun_debug ("Creating new keyring");

      если (def->process)
        {
          label = def->process->selinux_label;
          если (label)
            libcrun_debug ("Using SELinux process label: `%s`", label);
        }

      ret = libcrun_create_keyring (container, container->context->id, label, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }
  вернуть 0;
}

static целое
setup_terminal_socket_pair (libcrun_container_t *container, libcrun_context_t *context,
                            struct container_entrypoint_s *container_args, целое *socket_pair_0,
                            целое *socket_pair_1, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  целое ret;
  целое detach = context->detach;

  если (def->process && def->process->terminal && ! detach && context->console_socket == NULL)
    {
      libcrun_debug ("Creating terminal socket pair");
      container_args->has_terminal_socket_pair = 1;
      ret = create_socket_pair (container_args->terminal_socketpair, err);
      если (UNLIKELY (ret < 0))
        вернуть crun_error_wrap (err, "create terminal socket");

      *socket_pair_0 = container_args->terminal_socketpair[0];
      *socket_pair_1 = container_args->terminal_socketpair[1];
    }
  вернуть 0;
}

static целое
setup_seccomp (libcrun_container_t *container, const char *seccomp_bpf_data,
               struct libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, целое *seccomp_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  целое ret;

  если (find_annotation (container, "run.oci.seccomp.plugins") != NULL && has_seccomp_receiver (container))
    {
      вернуть crun_make_error (err, 0, "seccomp plugins and seccomp receivers cannot be declared at the same time");
    }

  если (def->linux && (def->linux->seccomp || seccomp_bpf_data))
    {
      unsigned целое seccomp_gen_options = 0;
      const char *annotation;

      libcrun_debug ("Initializing seccomp");
      annotation = find_annotation (container, "run.oci.seccomp_fail_unknown_syscall");
      если (annotation && strcmp (annotation, "0") != 0)
        seccomp_gen_options = LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL;

      если (seccomp_bpf_data)
        seccomp_gen_options |= LIBCRUN_SECCOMP_SKIP_CACHE;

      libcrun_seccomp_gen_ctx_init (seccomp_gen_ctx, container, true, seccomp_gen_options);

      ret = libcrun_open_seccomp_bpf (seccomp_gen_ctx, seccomp_fd, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }
  вернуть 0;
}

static целое
setup_console_socket (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
                      struct container_entrypoint_s *container_args, целое *console_socket_fd,
                      libcrun_error_t *err)
{
  если (context->console_socket && def->process && def->process->terminal)
    {
      *console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
      если (UNLIKELY (*console_socket_fd < 0))
        вернуть crun_error_wrap (err, "open console socket");
      container_args->console_socket_fd = *console_socket_fd;
    }
  вернуть 0;
}

static целое
setup_cgroup_manager (libcrun_context_t *context, libcrun_container_t *container,
                      struct libcrun_cgroup_args *cg, целое *cgroup_dirfd,
                      struct libcrun_dirfd_s *cgroup_dirfd_s, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  целое cgroup_manager;
  uid_t root_uid = -1;
  gid_t root_gid = -1;
  целое ret;

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  если (context->systemd_cgroup)
    {
      libcrun_debug ("Using systemd cgroup manager");
      cgroup_manager = CGROUP_MANAGER_SYSTEMD;
    }
  иначе если (context->force_no_cgroup)
    {
      libcrun_debug ("Disabling cgroup manager");
      cgroup_manager = CGROUP_MANAGER_DISABLED;
    }
  иначе
    libcrun_debug ("Using cgroupfs cgroup manager");

  /* If we are root (either on the host or in a namespace), then chown the cgroup to root
     in the container user namespace.  */
  get_root_in_the_userns (def, container->host_uid, container->host_gid, &root_uid, &root_gid);
  libcrun_debug ("Using container host UID `%d` and GID `%d`", container->host_uid, container->host_gid);

  memset (cg, 0, sizeof (*cg));

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
  если (UNLIKELY (ret < 0))
    вернуть ret;

  cgroup_dirfd_s->dirfd = cgroup_dirfd;
  cgroup_dirfd_s->joined = false;

  вернуть 0;
}

static целое
set_scheduler (pid_t pid, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  целое ret;

  ret = libcrun_set_scheduler (pid, def->process, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = libcrun_reset_cpu_affinity_mask (pid, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = libcrun_set_io_priority (pid, def->process, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  вернуть 0;
}

static целое
seccomp_generation (целое seccomp_fd, const char *seccomp_bpf_data,
                    struct libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, libcrun_error_t *err)
{
  целое ret;

  если (seccomp_fd >= 0)
    {
      если (seccomp_bpf_data != NULL)
        {
          ret = libcrun_copy_seccomp (seccomp_gen_ctx, seccomp_bpf_data, err);
          если (UNLIKELY (ret < 0))
            вернуть ret;
        }
      иначе
        {
          ret = libcrun_generate_seccomp (seccomp_gen_ctx, err);
          если (UNLIKELY (ret < 0))
            вернуть ret;
        }
    }
  вернуть 0;
}

static целое
terminal_setup (runtime_spec_schema_config_schema *def, libcrun_context_t *context,
                целое socket_pair_0, целое *terminal_fd, void **orig_terminal, libcrun_error_t *err)
{
  целое ret;
  целое detach = context->detach;

  если (def->process && def->process->terminal && ! detach && context->console_socket == NULL)
    {
      libcrun_debug ("Receiving console socket fd");
      *terminal_fd = receive_fd_from_socket (socket_pair_0, err);
      если (UNLIKELY (*terminal_fd < 0))
        вернуть -1;

      ret = libcrun_clear_onlcr (*terminal_fd, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      ret = libcrun_set_raw (0, orig_terminal, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }
  вернуть 0;
}

static целое
libcrun_container_run_internal (libcrun_container_t *container, libcrun_context_t *context,
                                целое *container_ready_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  целое ret;
  pid_t pid;
  целое detach = context->detach;
  cleanup_cgroup_status struct libcrun_cgroup_status *cgroup_status = NULL;
  cleanup_close целое terminal_fd = -1;
  cleanup_terminal void *orig_terminal = NULL;
  cleanup_close целое sync_socket = -1;
  cleanup_close целое notify_socket = -1;
  cleanup_close целое socket_pair_0 = -1;
  cleanup_close целое socket_pair_1 = -1;
  cleanup_close целое seccomp_fd = -1;
  cleanup_close целое console_socket_fd = -1;
  cleanup_close целое hooks_out_fd = -1;
  cleanup_close целое hooks_err_fd = -1;
  cleanup_close целое own_seccomp_receiver_fd = -1;
  cleanup_close целое seccomp_notify_fd = -1;
  const char *seccomp_notify_plugins = NULL;
  cleanup_cgroup_preenter struct libcrun_cgroup_args cg = {};
  cleanup_free char *bundle_allocated = NULL;
  struct container_entrypoint_s container_args = {
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
  cleanup_close целое cgroup_dirfd = -1;
  struct libcrun_dirfd_s cgroup_dirfd_s;
  struct libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  const char *seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");
  целое cgroup_mode;

  если (! context->force_no_cgroup)
    {
      cgroup_mode = libcrun_get_cgroup_mode (err);
      если (UNLIKELY (cgroup_mode < 0))
        вернуть cgroup_mode;

      если (cgroup_mode != CGROUP_MODE_UNIFIED)
        libcrun_warning ("cgroup v1 is deprecated and will be removed in a future release.  Use cgroup v2");
    }

  ret = setup_container_hooks_output (container, def, &container_args, &hooks_out_fd, &hooks_err_fd, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  container->context = context;

  если (! detach || context->notify_socket)
    {
      libcrun_debug ("Setting child subreaper");
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      если (UNLIKELY (ret < 0))
        вернуть crun_make_error (err, errno, "prctl set child subreaper");
    }

  ret = setup_container_keyring (container, context, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = setup_terminal_socket_pair (container, context, &container_args, &socket_pair_0, &socket_pair_1, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = block_signals (err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  umask (0);

  ret = libcrun_set_mempolicy (def, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;
  container_args.seccomp_fd = seccomp_fd;

  если (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &container_args.seccomp_receiver_fd, &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  ret = setup_console_socket (context, def, &container_args, &console_socket_fd, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = setup_cgroup_manager (context, container, &cg, &cgroup_dirfd, &cgroup_dirfd_s, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = libcrun_configure_handler (container_args.context->handler_manager,
                                   container_args.context,
                                   container,
                                   &(container_args.custom_handler),
                                   err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (container_args.custom_handler && container_args.custom_handler->vtable->modify_oci_configuration)
    {
      libcrun_debug ("Using custom handler to modify OCI configuration");
      ret = container_args.custom_handler->vtable->modify_oci_configuration (container_args.custom_handler->cookie,
                                                                             container_args.context,
                                                                             container->container_def,
                                                                             err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  pid = libcrun_run_linux_container (container, container_init, &container_args, &sync_socket, &cgroup_dirfd_s, err);
  если (UNLIKELY (pid < 0))
    вернуть pid;

  cg.pid = pid;
  cg.joined = cgroup_dirfd_s.joined;
  libcrun_debug ("Running container on PID: `%d`", pid);

  если (context->fifo_exec_wait_fd < 0 && context->notify_socket)
    {
      libcrun_debug ("Using notify socket: `%s`", context->notify_socket);
      /* Do not open the notify socket here on "create".  "start" will take care of it.  */
      ret = get_notify_fd (context, container, &notify_socket, err);
      если (UNLIKELY (ret < 0))
        goto fail;
    }

  если (container_args.terminal_socketpair[1] >= 0)
    close_and_reset (&socket_pair_1);

  ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
  если (UNLIKELY (ret < 0))
    goto fail;

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE, err);
  если (UNLIKELY (ret < 0))
    goto fail;

  ret = libcrun_move_network_devices (container, pid, err);
  если (UNLIKELY (ret < 0))
    goto fail;

  /* sync send own pid.  */
  ret = TEMP_FAILURE_RETRY (write (sync_socket, &pid, sizeof (pid)));
  если (UNLIKELY (ret != sizeof (pid)))
    {
      если (ret >= 0)
        errno = 0;
      crun_make_error (err, errno, "write to sync socket");
      goto fail;
    }

  /* sync 1.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  если (UNLIKELY (ret < 0))
    goto fail;

  /* sync 2.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  если (UNLIKELY (ret < 0))
    goto fail;

  ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
  если (UNLIKELY (ret < 0))
    goto fail;

  /* If the container process is created later, the scheduler is set on it then.  */
  если (! libcrun_container_delayed_pidns_fork (container))
    {
      ret = set_scheduler (pid, def, err);
      если (UNLIKELY (ret < 0))
        goto fail;
    }

  /* The container is waiting that we write back.  In this phase we can launch the
     prestart hooks.  */
  если (def->hooks && def->hooks->prestart_len)
    {
      libcrun_debug ("Running `prestart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "prestart", (hook **) def->hooks->prestart,
                      def->hooks->prestart_len, hooks_out_fd, hooks_err_fd, false, err);
      если (UNLIKELY (ret != 0))
        goto fail;
    }
  если (def->hooks && def->hooks->create_runtime_len)
    {
      libcrun_debug ("Running `create` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "createRuntime", (hook **) def->hooks->create_runtime,
                      def->hooks->create_runtime_len, hooks_out_fd, hooks_err_fd, false, err);
      если (UNLIKELY (ret != 0))
        goto fail;
    }

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  если (UNLIKELY (ret < 0))
    goto fail;
  close_and_reset (&seccomp_fd);

  /* sync 3.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  если (UNLIKELY (ret < 0))
    goto fail;

  /* The process joining an existing PID namespace is created once the container root is in place.  */
  если (libcrun_container_delayed_pidns_fork (container))
    {
      pid_t new_pid = 0;

      ret = sync_socket_wait_sync (context, sync_socket, false, err);
      если (UNLIKELY (ret < 0))
        goto fail;

      ret = TEMP_FAILURE_RETRY (read (sync_socket, &new_pid, sizeof (new_pid)));
      если (UNLIKELY (ret != sizeof (new_pid)))
        {
          если (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "read pid from sync socket");
          goto fail;
        }

      /* Cleanup the first process.  */
      waitpid_ignore_stopped (pid, NULL, 0);

      pid = new_pid;

      ret = set_scheduler (pid, def, err);
      если (UNLIKELY (ret < 0))
        goto fail;

      ret = libcrun_handle_pidfd_receiver (pid, container, err);
      если (UNLIKELY (ret < 0))
        goto fail;

      /* sync send own pid.  */
      ret = TEMP_FAILURE_RETRY (write (sync_socket, &pid, sizeof (pid)));
      если (UNLIKELY (ret != sizeof (pid)))
        {
          если (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "write to sync socket");
          goto fail;
        }
    }

  ret = terminal_setup (def, context, socket_pair_0, &terminal_fd, &orig_terminal, err);
  если (UNLIKELY (ret < 0))
    goto fail;

  close_and_reset (&socket_pair_0);

  /* sync 4.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  если (UNLIKELY (ret < 0))
    goto fail;

  ret = close_and_reset (&sync_socket);
  если (UNLIKELY (ret < 0))
    {
      crun_make_error (err, errno, "close sync_socket failed");
      goto fail;
    }

  libcrun_debug ("Writing container status");
  ret = write_container_status (container, context, pid, cgroup_status, err);
  если (UNLIKELY (ret < 0))
    goto fail;

  /* Run poststart hooks here only if the container is created using "run".  For create+start, the
     hooks will be executed as part of the start command.  */
  если (context->fifo_exec_wait_fd < 0 && def->hooks && def->hooks->poststart_len)
    {
      libcrun_debug ("Running `poststart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      если (UNLIKELY (ret != 0))
        goto fail;
    }

  /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
  если (own_seccomp_receiver_fd >= 0)
    {
      libcrun_debug ("Receiving seccomp fd");
      seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
      если (UNLIKELY (seccomp_notify_fd < 0))
        goto fail;

      ret = close_and_reset (&own_seccomp_receiver_fd);
      если (UNLIKELY (ret < 0))
        {
          crun_make_error (err, errno, "close seccomp receiver fd failed");
          goto fail;
        }
    }

  {
    struct wait_for_process_args args = {
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
  если (! context->detach)
    {
      libcrun_error_t tmp_err = NULL;
      cleanup_watch (context, def, cgroup_status, 0, sync_socket, terminal_fd, &tmp_err);
      crun_error_release (&tmp_err);
    }

  вернуть ret;

fail:
  ret = cleanup_watch (context, def, cgroup_status, pid, sync_socket, terminal_fd, err);
  если (cgroup_status)
    {
      libcrun_error_t tmp_err = NULL;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  вернуть ret;
}

static целое
check_config_file (runtime_spec_schema_config_schema *def, libcrun_context_t *context, libcrun_error_t *err)
{
  если (UNLIKELY (def->linux == NULL))
    вернуть crun_make_error (err, 0, "invalid config file, no `linux` block specified");

  если (context->handler == NULL)
    {
      если (UNLIKELY (def->root == NULL))
        вернуть crun_make_error (err, 0, "invalid config file, no `root` block specified");
      если (UNLIKELY (def->mounts == NULL))
        вернуть crun_make_error (err, 0, "invalid config file, no `mounts` block specified");
    }
  вернуть 0;
}

static целое
libcrun_copy_config_file (const char *id, const char *state_root, libcrun_container_t *container, libcrun_error_t *err)
{
  целое ret;
  cleanup_free char *dest_path = NULL;
  cleanup_free char *dir = NULL;
  cleanup_free char *buffer = NULL;
  size_t len;

  ret = libcrun_get_state_directory (&dir, state_root, id, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = append_paths (&dest_path, err, dir, "config.json", NULL);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (container->config_file == NULL && container->config_file_content == NULL)
    вернуть crun_make_error (err, 0, "config file not specified");

  если (container->config_file == NULL)
    {
      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, container->config_file_content, strlen (container->config_file_content), err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }
  иначе
    {
      libcrun_debug ("Reading config file: `%s`", container->config_file);
      ret = read_all_file (container->config_file, &buffer, &len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, buffer, len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  вернуть 0;
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
write_error_to_pipe (целое fd, libcrun_error_t *err)
{
  const char *msg = (*err)->msg;
  целое errcode = crun_error_get_errno (err);

  TEMP_FAILURE_RETRY (write (fd, &errcode, sizeof (errcode)));
  TEMP_FAILURE_RETRY (write (fd, msg, strlen (msg) + 1));
}

целое
libcrun_container_run (libcrun_context_t *context, libcrun_container_t *container, unsigned целое options,
                       libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  целое ret;
  целое detach = context->detach;
  целое container_ret_status[2];
  cleanup_close целое pipefd0 = -1;
  cleanup_close целое pipefd1 = -1;
  libcrun_error_t tmp_err = NULL;

  container->context = context;

  ret = validate_options (options, LIBCRUN_RUN_OPTIONS_PREFORK | LIBCRUN_RUN_OPTIONS_KEEP, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = check_config_file (def, context, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (def->process && def->process->terminal && detach && context->console_socket == NULL)
    вернуть crun_make_error (err, 0, "use --console-socket with --detach when a terminal is used");

  ret = libcrun_status_check_directories (context->state_root, context->id, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (! detach && (options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      ret = libcrun_copy_config_file (context->id, context->state_root, container, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      ret = libcrun_container_run_internal (container, context, NULL, err);
      если (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
        force_delete_container_status (context, container, def);
      вернуть ret;
    }

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  если (UNLIKELY (ret < 0))
    вернуть crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  ret = fork ();
  если (UNLIKELY (ret < 0))
    вернуть crun_make_error (err, errno, "fork");
  если (ret)
    {
      целое status;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, NULL, 0);

      ret = TEMP_FAILURE_RETRY (read (pipefd0, &status, sizeof (status)));
      если (UNLIKELY (ret < 0))
        вернуть crun_make_error (err, errno, "read from sync pipe");

      если (status < 0)
        {
          целое errno_;
          char buf[512];
          ret = TEMP_FAILURE_RETRY (read (pipefd0, &errno_, sizeof (errno_)));
          если (UNLIKELY (ret < 0))
            вернуть crun_make_error (err, errno, "read from sync pipe");

          ret = TEMP_FAILURE_RETRY (read (pipefd0, buf, sizeof (buf) - 1));
          если (UNLIKELY (ret < 0))
            вернуть crun_make_error (err, errno, "read from sync pipe");
          buf[ret] = '\0';

          вернуть crun_make_error (err, errno_, "%s", buf);
        }

      вернуть status;
    }

  close_and_reset (&pipefd0);

  /* forked process.  */
  ret = detach_process ();
  если (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_root, container, &tmp_err);
  если (UNLIKELY (ret < 0))
    goto fail;

  ret = libcrun_container_run_internal (container, context, NULL, &tmp_err);
  TEMP_FAILURE_RETRY (write (pipefd1, &ret, sizeof (ret)));
  если (UNLIKELY (ret < 0))
    goto fail;

  exit (EXIT_SUCCESS);
fail:

  если (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
    force_delete_container_status (context, container, def);
  если (tmp_err)
    {
      write_error_to_pipe (pipefd1, &tmp_err);
      crun_error_release (&tmp_err);
    }

  exit (EXIT_FAILURE);
}

целое
libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container, unsigned целое options,
                          libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  целое ret;
  целое container_ready_pipe[2];
  cleanup_close целое pipefd0 = -1;
  cleanup_close целое pipefd1 = -1;
  cleanup_close целое exec_fifo_fd = -1;
  context->detach = 1;

  libcrun_debug ("Creating container: `%s`", context->id);
  container->context = context;

  ret = validate_options (options, LIBCRUN_CREATE_OPTIONS_PREFORK, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = check_config_file (def, context, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (def->process && def->process->terminal && context->console_socket == NULL)
    вернуть crun_make_error (err, 0, "use --console-socket with create when a terminal is used");

  ret = libcrun_status_check_directories (context->state_root, context->id, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  exec_fifo_fd = libcrun_status_create_exec_fifo (context->state_root, context->id, err);
  если (UNLIKELY (exec_fifo_fd < 0))
    вернуть exec_fifo_fd;

  context->fifo_exec_wait_fd = exec_fifo_fd;
  exec_fifo_fd = -1;

  если ((options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      libcrun_debug ("Running with prefork enabled");
      ret = libcrun_copy_config_file (context->id, context->state_root, container, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
      ret = libcrun_container_run_internal (container, context, NULL, err);
      если (UNLIKELY (ret < 0))
        force_delete_container_status (context, container, def);
      вернуть ret;
    }

  ret = pipe2 (container_ready_pipe, O_CLOEXEC);
  если (UNLIKELY (ret < 0))
    вернуть crun_make_error (err, errno, "pipe");
  pipefd0 = container_ready_pipe[0];
  pipefd1 = container_ready_pipe[1];

  ret = fork ();
  если (UNLIKELY (ret < 0))
    вернуть crun_make_error (err, errno, "fork");
  если (ret)
    {
      целое exit_code;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, NULL, 0);

      ret = TEMP_FAILURE_RETRY (read (pipefd0, &exit_code, sizeof (exit_code)));
      если (UNLIKELY (ret <= 0))
        вернуть crun_make_error (err, ret < 0 ? errno : 0, "waiting for container to be ready");

      если (ret != sizeof (exit_code) || exit_code < 0)
        вернуть crun_make_error (err, 0, "internal error: read invalid exit code");

      если (exit_code == 0)
        вернуть 0;

      libcrun_debug ("Exit code is `%d`, deleting container", exit_code);
      libcrun_error_t tmp_err = NULL;
      container_delete_internal (context, NULL, def, context->id, true, true, &tmp_err);
      libcrun_error_report_and_release (&tmp_err);
      вернуть crun_make_error (err, 0, "error creating container");
    }

  /* forked process.  */
  ret = detach_process ();
  если (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_root, container, err);
  если (UNLIKELY (ret < 0))
    {
      целое errcode = crun_error_get_errno (err);
      crun_error_release (err);
      libcrun_fail_with_error (errcode, "copy config file");
    }

  ret = libcrun_container_run_internal (container, context, &pipefd1, err);
  если (UNLIKELY (ret < 0))
    {
      force_delete_container_status (context, container, def);
      libcrun_error ((*err)->status, "%s", (*err)->msg);
      crun_error_release (err);
      crun_set_output_handler (log_write_to_stderr, NULL);
    }

  если (pipefd1 >= 0)
    TEMP_FAILURE_RETRY (write (pipefd1, &ret, sizeof (ret)));
  exit (ret ? EXIT_FAILURE : 0);
}

целое
libcrun_container_start (libcrun_context_t *context, const char *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = NULL;
  const char *state_root = context->state_root;
  runtime_spec_schema_config_schema *def;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_close целое fd = -1;
  целое ret;

  ret = libcrun_read_container_status (&status, state_root, id, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = libcrun_is_container_running (&status, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (! ret)
    вернуть crun_make_error (err, 0, "container `%s` is not running", id);

  ret = read_container_config_from_state (&container, state_root, id, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (context->notify_socket)
    {
      ret = get_notify_fd (context, container, &fd, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  ret = libcrun_status_write_exec_fifo (context->state_root, id, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  def = container->container_def;

  если (context->notify_socket)
    {
      если (fd >= 0)
        {
          fd_set read_set;

          пока (1)
            {
              struct timeval timeout = {
                .tv_sec = 0,
                .tv_usec = 10000,
              };
              FD_ZERO (&read_set);
              FD_SET (fd, &read_set);

              ret = select (fd + 1, &read_set, NULL, NULL, &timeout);
              если (UNLIKELY (ret < 0))
                вернуть crun_make_error (err, errno, "select failed");
              если (ret)
                {
                  ret = handle_notify_socket (fd, err);
                  если (UNLIKELY (ret < 0))
                    вернуть ret;
                  если (ret)
                    break;
                }
              иначе
                {
                  ret = libcrun_is_container_running (&status, err);
                  если (UNLIKELY (ret < 0))
                    вернуть ret;
                  если (! ret)
                    вернуть 0;
                }
            }
        }
    }

  /* The container is considered running only after we got the notification from the
     notify_socket, if any.  */
  если (def->hooks && def->hooks->poststart_len)
    {
      cleanup_close целое hooks_out_fd = -1;
      cleanup_close целое hooks_err_fd = -1;

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      ret = do_hooks (def, status.pid, context->id, false, status.bundle, status.bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      если (UNLIKELY (ret != 0))
        {
          libcrun_error_t tmp_err = NULL;
          container_delete_internal (context, container, def, id, true, true, &tmp_err);
          libcrun_error_report_and_release (&tmp_err);
          вернуть ret;
        }
    }

  вернуть 0;
}

целое
libcrun_get_container_state_string (const char *id, libcrun_container_status_t *status, const char *state_root,
                                    const char **container_status, целое *running, libcrun_error_t *err)
{
  целое ret, has_fifo = 0;
  bool paused = false;

  ret = libcrun_is_container_running (status, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;
  *running = ret;

  если (*running)
    {
      ret = libcrun_status_has_read_exec_fifo (state_root, id, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
      has_fifo = ret;
    }

  если (*running && ! has_fifo)
    {
      cleanup_cgroup_status struct libcrun_cgroup_status *cgroup_status = NULL;

      cgroup_status = libcrun_cgroup_make_status (status);

      ret = libcrun_cgroup_is_container_paused (cgroup_status, &paused, err);
      если (UNLIKELY (ret < 0))
        {
          /*
            The cgroup might have been cleaned up by the time we try to read it, ignore both
            ENOENT and ENODEV:
            - ENOENT: if the open(CGROUP_PATH) fails because the cgroup was deleted.
            - ENODEV: if the cgroup is deleted between the open and reading the freeze status.
          */

          errno = crun_error_get_errno (err);
          если (errno == ENOENT || errno == ENODEV)
            {
              crun_error_release (err);
              *container_status = "stopped";
              вернуть 0;
            }

          вернуть ret;
        }
    }

  если (! *running)
    *container_status = "stopped";
  иначе если (has_fifo)
    *container_status = "created";
  иначе если (paused)
    *container_status = "paused";
  иначе
    *container_status = "running";

  вернуть 0;
}

целое
libcrun_container_state_json (libcrun_context_t *context, const char *id, char **out, libcrun_error_t *err)
{
  char *buffer = NULL;
  size_t len = 0;
  FILE *f;
  целое ret;

  f = open_memstream (&buffer, &len);
  если (f == NULL)
    вернуть crun_make_error (err, errno, "open_memstream");

  ret = libcrun_container_state (context, id, f, err);
  fclose (f);
  если (UNLIKELY (ret < 0))
    {
      free (buffer);
      вернуть ret;
    }

  *out = buffer;
  вернуть 0;
}

целое
libcrun_container_state (libcrun_context_t *context, const char *id, FILE *out, libcrun_error_t *err)
{
  const char *const OCI_CONFIG_VERSION = "1.0.0";
  libcrun_container_status_t status = {};
  const char *state_root = context->state_root;
  const char *container_status = NULL;
  json_gen_ctx *gen = NULL;
  const char *buf;
  целое ret = 0;
  целое r = json_gen_status_ok;
  целое running;
  size_t len;

  ret = libcrun_read_container_status (&status, state_root, id, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = libcrun_get_container_state_string (id, &status, state_root, &container_status, &running, err);
  если (UNLIKELY (ret < 0))
    goto exit;

  ret = 0;
  если (! json_gen_init (&gen, NULL))
    вернуть crun_make_error (err, 0, "json_gen_init failed");

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

  если (status.scope)
    {
      GEN_KEY (gen, "systemd-scope");
      GEN_STR (gen, status.scope);
    }

  если (status.owner)
    {
      GEN_KEY (gen, "owner");
      GEN_STR (gen, status.owner);
    }

  {
    cleanup_container libcrun_container_t *container = NULL;

    ret = read_container_config_from_state (&container, state_root, id, err);
    если (UNLIKELY (ret < 0))
      goto exit;

    если (container->container_def->annotations && container->container_def->annotations->len)
      GEN_OR_FAIL (gen_annotations (gen, container->container_def->annotations));
  }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  fprintf (out, "%s\n", buf);

exit:
  если (gen)
    json_gen_free (gen);
  libcrun_free_container_status (&status);
  вернуть ret;

gen_error:
  ret = json_gen_error_to_crun_error (r, err);
  goto exit;
}

целое
libcrun_container_exec (libcrun_context_t *context, const char *id, runtime_spec_schema_config_schema_process *process,
                        libcrun_error_t *err)
{
  struct libcrun_container_exec_options_s opts;
  memset (&opts, 0, sizeof (opts));

  opts.struct_size = sizeof (opts);
  opts.process = process;

  вернуть libcrun_container_exec_with_options (context, id, &opts, err);
}

целое
libcrun_container_exec_process_file (libcrun_context_t *context, const char *id, const char *path, libcrun_error_t *err)
{
  struct libcrun_container_exec_options_s opts;
  memset (&opts, 0, sizeof (opts));

  opts.struct_size = sizeof (opts);
  opts.path = path;

  вернуть libcrun_container_exec_with_options (context, id, &opts, err);
}

static целое
exec_from_process_json (libcrun_context_t *context, const char *id, const char *process_json, const char *cgroup,
                        bool merge_env, libcrun_error_t *err)
{
  struct parser_context parser_ctx = { .options = 0, .errfile = stderr };
  struct libcrun_container_exec_options_s opts;
  runtime_spec_schema_config_schema_process *process;
  parser_error parser_err = NULL;
  json_object *doc = NULL;
  целое ret;

  ret = parse_json_file (&doc, process_json, &parser_ctx, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  process = make_runtime_spec_schema_config_schema_process (doc, &parser_ctx, &parser_err);
  json_object_put (doc);
  если (process == NULL)
    {
      ret = crun_make_error (err, 0, "cannot parse process: `%s`", parser_err ? parser_err : "unknown error");
      free (parser_err);
      вернуть ret;
    }

  memset (&opts, 0, sizeof (opts));
  opts.struct_size = sizeof (opts);
  opts.process = process;
  opts.cgroup = cgroup;
  opts.merge_env = merge_env;

  ret = libcrun_container_exec_with_options (context, id, &opts, err);
  free_runtime_spec_schema_config_schema_process (process);
  вернуть ret;
}

целое
libcrun_container_exec_json (libcrun_context_t *context, const char *id, const char *process_json, libcrun_error_t *err)
{
  вернуть exec_from_process_json (context, id, process_json, NULL, false, err);
}

целое
libcrun_container_exec_with_options_json (libcrun_context_t *context, const char *id,
                                          const struct libcrun_exec_options_s *opts, libcrun_error_t *err)
{
  если (opts == NULL || opts->struct_size == 0)
    вернуть crun_make_error (err, EINVAL, "invalid exec options");

  вернуть exec_from_process_json (context, id, opts->process_json, opts->cgroup, opts->merge_env, err);
}

#define cleanup_process_schema __attribute__ ((cleanup (cleanup_process_schemap)))

static inline void
cleanup_process_schemap (runtime_spec_schema_config_schema_process **p)
{
  runtime_spec_schema_config_schema_process *process = *p;
  если (process)
    (void) free_runtime_spec_schema_config_schema_process (process);
}

/* Build the seccomp receiver payload (when needed), apply the seccomp profile and
   close the seccomp file descriptors.  Used from exec_process_entrypoint both before
   and after the capabilities are set, depending on process->no_new_privileges.  */
static целое
apply_seccomp_for_exec (libcrun_container_t *container, pid_t own_pid, целое seccomp_fd,
                        целое seccomp_receiver_fd, char **seccomp_flags, size_t seccomp_flags_len,
                        libcrun_error_t *err)
{
  cleanup_free char *seccomp_fd_payload = NULL;
  size_t seccomp_fd_payload_len = 0;
  целое ret;

  если (seccomp_receiver_fd >= 0)
    {
      ret = get_seccomp_receiver_fd_payload (container, "running", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  ret = libcrun_apply_seccomp (seccomp_fd, seccomp_receiver_fd, seccomp_fd_payload,
                               seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  close_and_reset (&seccomp_fd);
  close_and_reset (&seccomp_receiver_fd);
  вернуть 0;
}

static целое
exec_process_entrypoint (libcrun_context_t *context,
                         libcrun_container_t *container,
                         runtime_spec_schema_config_schema_process *process,
                         целое *pipefd1,
                         целое seccomp_fd,
                         целое seccomp_receiver_fd,
                         struct custom_handler_instance_s *custom_handler,
                         bool merge_env,
                         libcrun_error_t *err)
{
  runtime_spec_schema_config_schema_process_capabilities *capabilities = NULL;
  cleanup_free char *exec_path = NULL;
  uid_t container_uid;
  gid_t container_gid;
  const char *cwd;
  bool chdir_done = false;
  size_t seccomp_flags_len = 0;
  char **seccomp_flags = NULL;
  pid_t own_pid = 0;
  size_t i;
  целое ret;

  container_uid = process->user ? process->user->uid : 0;
  container_gid = process->user ? process->user->gid : 0;

  TEMP_FAILURE_RETRY (read (*pipefd1, &own_pid, sizeof (own_pid)));

  cwd = process->cwd ? process->cwd : "/";
  если (LIKELY (libcrun_safe_chdir (cwd, err) == 0))
    chdir_done = true;
  иначе
    crun_error_release (err);

  ret = unblock_signals (err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = clearenv ();
  если (UNLIKELY (ret < 0))
    вернуть crun_make_error (err, errno, "clearenv");

  если ((merge_env || ! process->env_len) && container->container_def->process && container->container_def->process->env_len)
    {
      char *e;

      для (i = 0; i < container->container_def->process->env_len; i++)
        {
          e = container->container_def->process->env[i];
          если (putenv (e) < 0)
            вернуть crun_make_error (err, errno, "putenv `%s`", e);
        }
    }
  если (process->env_len)
    {
      для (i = 0; i < process->env_len; i++)
        если (putenv (process->env[i]) < 0)
          вернуть crun_make_error (err, errno, "putenv `%s`", process->env[i]);
    }

  если (is_empty_string (getenv ("HOME")))
    {
      ret = set_home_env (container_uid);
      если (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          если (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            вернуть crun_make_error (err, errno, "setenv HOME");
        }
    }

  ret = libcrun_set_selinux_label (container, process, false, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = libcrun_set_apparmor_profile (container, process, false, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (container->container_def->linux && container->container_def->linux->seccomp)
    {
      seccomp_flags = container->container_def->linux->seccomp->flags;
      seccomp_flags_len = container->container_def->linux->seccomp->flags_len;
    }

  ret = find_executable (&exec_path, process->args[0], process->cwd, err);
  если (UNLIKELY (ret < 0))
    {
      если (crun_error_get_errno (err) == ENOENT)
        вернуть ret;

      /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
         once the process switched to the use that runs in the container.  This might be necessary
         when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
         is not honored.  */
      crun_error_release (err);
    }

  если (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, false, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = libcrun_reopen_dev_null (err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (! process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  ret = libcrun_container_setgroups (container, process, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = maybe_chown_std_streams (container_uid, container_gid, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (process->capabilities)
    capabilities = process->capabilities;
  иначе если (container->container_def->process)
    capabilities = container->container_def->process->capabilities;

  ret = libcrun_set_caps (capabilities, container_uid, container_gid, process->no_new_privileges,
                          custom_handler != NULL, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (UNLIKELY (exec_path == NULL))
    {
      ret = find_executable (&exec_path, process->args[0], process->cwd, err);
      если (UNLIKELY (ret < 0))
        {
          если (custom_handler == NULL || is_empty_string (process->args[0]))
            вернуть ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          exec_path = xstrdup (process->args[0]);
        }
    }

  если (! chdir_done)
    {
      ret = libcrun_safe_chdir (cwd, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  если (process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  если (process->user)
    umask (process->user->umask_present ? process->user->umask : 0022);

  ret = 0;
  TEMP_FAILURE_RETRY (write (*pipefd1, &ret, sizeof (ret)));
  TEMP_FAILURE_RETRY (close (*pipefd1));
  *pipefd1 = -1;

  если (custom_handler)
    {
      если (custom_handler->vtable->exec_func == NULL)
        вернуть crun_make_error (err, 0, "the handler does not support exec");

      если (context->argv)
        erase_initial_environment (context->argv, context->argc);

      ret = custom_handler->vtable->exec_func (custom_handler->cookie,
                                               container,
                                               exec_path,
                                               process->args);
      если (UNLIKELY (ret < 0))
        вернуть crun_make_error (err, -ret, "exec container process failed with handler as `%s`", custom_handler->vtable->name);

      _safe_exit (EXIT_FAILURE);
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation, because the seccomp filter is already in place and it could
     stop some syscalls used by mark_or_close_fds_ge_than.
  */
  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, true, err);
  если (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, process->args));
  libcrun_fail_with_error (errno, "exec `%s`", exec_path);
}

/* Close the file descriptors inherited from the caller, other than those
   to be passed to the container process.  */
целое
libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err)
{
  вернуть mark_or_close_fds_ge_than (NULL, context->preserve_fds + 3, true, err);
}

целое
libcrun_load_process_from_file (const char *path, runtime_spec_schema_config_schema_process **process,
                                libcrun_error_t *err)
{
  struct parser_context ctx = { 0, stderr };
  cleanup_free char *content = NULL;
  parser_error parser_err = NULL;
  json_object *doc = NULL;
  size_t len;
  целое ret;

  ret = read_all_file (path, &content, &len, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  *process = make_runtime_spec_schema_config_schema_process (doc, &ctx, &parser_err);
  если (UNLIKELY (*process == NULL))
    {
      ret = crun_make_error (err, 0, "cannot parse process file: `%s`", parser_err);
      free (parser_err);
      если (doc)
        json_object_put (doc);
      вернуть ret;
    }

  free (parser_err);
  если (doc)
    json_object_put (doc);

  вернуть 0;
}

целое
libcrun_container_exec_with_options (libcrun_context_t *context, const char *id,
                                     struct libcrun_container_exec_options_s *opts,
                                     libcrun_error_t *err)
{
  cleanup_custom_handler_instance struct custom_handler_instance_s *custom_handler = NULL;
  целое container_status, ret;
  bool container_paused = false;
  pid_t pid;
  libcrun_container_status_t status = {};
  const char *state_root = context->state_root;
  cleanup_close целое terminal_fd = -1;
  cleanup_close целое seccomp_fd = -1;
  cleanup_terminal void *orig_terminal = NULL;
  cleanup_container libcrun_container_t *container = NULL;
  целое container_ret_status[2];
  cleanup_close целое pipefd0 = -1;
  cleanup_close целое pipefd1 = -1;
  cleanup_close целое seccomp_receiver_fd = -1;
  cleanup_close целое own_seccomp_receiver_fd = -1;
  cleanup_close целое seccomp_notify_fd = -1;
  const char *seccomp_notify_plugins = NULL;
  __attribute__ ((unused)) cleanup_process_schema runtime_spec_schema_config_schema_process *process_cleanup = NULL;
  runtime_spec_schema_config_schema_process *process = opts->process;
  struct libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  целое ret_from_child = 0;

  ret = libcrun_read_container_status (&status, state_root, id, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = libcrun_is_container_running (&status, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;
  container_status = ret;

  ret = read_container_config_from_state (&container, state_root, id, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  container->context = context;

  если (container_status == 0)
    вернуть crun_make_error (err, 0, "the container `%s` is not running", id);

  {
    cleanup_cgroup_status struct libcrun_cgroup_status *cgroup_status = NULL;

    cgroup_status = libcrun_cgroup_make_status (&status);

    ret = libcrun_cgroup_is_container_paused (cgroup_status, &container_paused, err);
    если (UNLIKELY (ret < 0))
      вернуть ret;
  }

  если (UNLIKELY (container_paused))
    вернуть crun_make_error (err, 0, "cannot exec in a paused container `%s`", id);

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = block_signals (err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  libcrun_seccomp_gen_ctx_init (&seccomp_gen_ctx, container, false, 0);

  ret = libcrun_open_seccomp_bpf (&seccomp_gen_ctx, &seccomp_fd, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &seccomp_receiver_fd,
                                     &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  если (sizeof (*opts) != opts->struct_size)
    вернуть crun_make_error (err, EINVAL, "invalid libcrun_container_exec_options_s struct");

  если (opts->path)
    {
      если (process)
        вернуть crun_make_error (err, EINVAL, "cannot specify both exec file and options");

      ret = libcrun_load_process_from_file (opts->path, &process, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      process_cleanup = process;
    }

  /* This must be done before we enter a user namespace.  If the process does
     not specify any rlimits, use the ones from the container configuration.  */
  если (process->rlimits_len == 0 && container->container_def->process)
    ret = libcrun_set_rlimits (container->container_def->process->rlimits,
                               container->container_def->process->rlimits_len, err);
  иначе
    ret = libcrun_set_rlimits (process->rlimits, process->rlimits_len, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = libcrun_set_mempolicy (container->container_def, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  если (UNLIKELY (ret < 0))
    вернуть crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  /* If the new process block doesn't specify a SELinux label, AppArmor profile, user,
     I/O priority or scheduler, then use the configuration from the original config file.  */
  если (container->container_def->process)
    {
      если (process->io_priority == NULL && container->container_def->process->io_priority)
        {
          process->io_priority = clone_runtime_spec_schema_config_schema_process_io_priority (container->container_def->process->io_priority);
          если (process->io_priority == NULL)
            OOM ();
        }

      если (process->scheduler == NULL && container->container_def->process->scheduler)
        {
          process->scheduler = clone_runtime_spec_schema_config_schema_process_scheduler (container->container_def->process->scheduler);
          если (process->scheduler == NULL)
            OOM ();
        }

      если (process->selinux_label == NULL && container->container_def->process->selinux_label)
        process->selinux_label = xstrdup (container->container_def->process->selinux_label);

      если (process->apparmor_profile == NULL && container->container_def->process->apparmor_profile)
        process->apparmor_profile = xstrdup (container->container_def->process->apparmor_profile);

      если (process->user == NULL && container->container_def->process->user)
        {
          process->user = clone_runtime_spec_schema_config_schema_process_user (container->container_def->process->user);
          если (process->user == NULL)
            OOM ();
        }
    }

  ret = initialize_security (container, process, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  /* Do not leak the session keyring of the caller into the container.  The
     keyring created for the container cannot be joined, so use an anonymous one.  */
  если (! context->no_new_keyring && ! status.no_new_keyring)
    {
      libcrun_debug ("Creating new keyring for the exec process");

      ret = libcrun_create_keyring (container, NULL, process->selinux_label, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
    }

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  если (UNLIKELY (ret < 0))
    вернуть crun_make_error (err, errno, "prctl unset dumpable");

  pid = libcrun_join_process (context, container, status.pid, &status, opts->cgroup, context->detach,
                              process, process->terminal ? &terminal_fd : NULL, err);
  если (UNLIKELY (pid < 0))
    вернуть pid;

  /* Process to exec.  */
  если (pid == 0)
    {
      TEMP_FAILURE_RETRY (close (pipefd0));
      pipefd0 = -1;

      exec_process_entrypoint (context, container, process, &pipefd1, seccomp_fd, seccomp_receiver_fd, custom_handler, opts->merge_env, err);
      /* It gets here only on errors.  */
      если (*err)
        {
          если (pipefd1 < 0)
            libcrun_fail_with_error ((*err)->status, "%s", (*err)->msg);
          иначе
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

  TEMP_FAILURE_RETRY (write (pipefd0, &pid, sizeof (pid)));

  если (seccomp_fd >= 0)
    close_and_reset (&seccomp_fd);

  если (terminal_fd >= 0)
    {
      unsigned short rows = 0, cols = 0;

      если (process->console_size)
        {
          cols = process->console_size->width;
          rows = process->console_size->height;
        }

      ret = libcrun_terminal_setup_size (terminal_fd, rows, cols, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;

      если (context->console_socket)
        {
          целое ret;
          cleanup_close целое console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
          если (UNLIKELY (console_socket_fd < 0))
            вернуть console_socket_fd;
          ret = send_fd_to_socket (console_socket_fd, terminal_fd, err);
          если (UNLIKELY (ret < 0))
            вернуть ret;
          close_and_reset (&terminal_fd);
        }
      иначе
        {
          ret = libcrun_clear_onlcr (terminal_fd, err);
          если (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              вернуть ret;
            }

          ret = libcrun_set_raw (0, &orig_terminal, err);
          если (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              вернуть ret;
            }
        }
    }

  ret = TEMP_FAILURE_RETRY (read (pipefd0, &ret_from_child, sizeof (ret_from_child)));
  если (ret != sizeof (ret_from_child))
    ret = crun_make_error (err, 0, "read pipe failed");
  иначе если (ret_from_child != 0)
    {
      cleanup_free char *msg = NULL;
      size_t len = 0;

      ret = read_all_fd (pipefd0, "error stream", &msg, &len, err);
      если (UNLIKELY (ret < 0))
        вернуть ret;
      /* the string from read_all_fd is always NUL terminated.  */
      ret = crun_make_error (err, ret_from_child, "%s", msg);
    }
  иначе
    {
      /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
      если (own_seccomp_receiver_fd >= 0)
        {
          seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
          если (UNLIKELY (seccomp_notify_fd < 0))
            вернуть seccomp_notify_fd;

          ret = close_and_reset (&own_seccomp_receiver_fd);
          если (UNLIKELY (ret < 0))
            вернуть crun_make_error (err, errno, "close seccomp receiver fd failed");
        }

      {
        struct wait_for_process_args args = {
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
  вернуть ret;
}

целое
libcrun_container_update (libcrun_context_t *context, const char *id, const char *content, size_t len arg_unused,
                          libcrun_error_t *err)
{
  cleanup_custom_handler_instance struct custom_handler_instance_s *custom_handler = NULL;
  runtime_spec_schema_config_linux_resources *resources = NULL;
  cleanup_container libcrun_container_t *container = NULL;
  const char *state_root = context->state_root;
  struct parser_context ctx = { 0, stderr };
  libcrun_container_status_t status = {};
  parser_error parser_err = NULL;
  json_object *doc = NULL;
  целое ret;

  ret = libcrun_read_container_status (&status, state_root, id, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = read_container_config_from_state (&container, state_root, id, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  resources = make_runtime_spec_schema_config_linux_resources (doc, &ctx, &parser_err);
  если (UNLIKELY (resources == NULL))
    {
      ret = crun_make_error (err, 0, "cannot parse resources: %s", parser_err);
      goto cleanup;
    }

  если (custom_handler && custom_handler->vtable->modify_oci_configuration)
    {
      /* Adapt RESOURCES to be used from the modify_oci_configuration hook.  */
      cleanup_free runtime_spec_schema_config_linux *linux = xmalloc0 (sizeof (*linux));
      cleanup_free runtime_spec_schema_config_schema *def = xmalloc0 (sizeof (*def));

      def->linux = linux;
      linux->resources = resources;

      ret = custom_handler->vtable->modify_oci_configuration (custom_handler->cookie,
                                                              context,
                                                              def,
                                                              err);
      если (UNLIKELY (ret < 0))
        goto cleanup;
    }

  ret = libcrun_linux_container_update (&status, state_root, resources, err);

cleanup:
  если (doc)
    json_object_put (doc);
  free (parser_err);
  если (resources)
    free_runtime_spec_schema_config_linux_resources (resources);

  вернуть ret;
}

целое
libcrun_container_update_from_file (libcrun_context_t *context, const char *id, const char *file, libcrun_error_t *err)
{
  cleanup_free char *content = NULL;
  size_t len;
  целое ret;

  ret = read_all_file (file, &content, &len, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  вернуть libcrun_container_update (context, id, content, len, err);
}

static целое
compare_update_values (const void *a, const void *b)
{
  const struct libcrun_update_value_s *aa = a;
  const struct libcrun_update_value_s *bb = b;
  целое ret;

  ret = strcmp (aa->section, bb->section);
  если (ret)
    вернуть ret;
  вернуть strcmp (aa->name, bb->name);
}

целое
libcrun_container_update_from_values (libcrun_context_t *context, const char *id,
                                      struct libcrun_update_value_s *values, size_t len,
                                      libcrun_error_t *err)
{
  const char *current_section = NULL;
  const char *buf;
  json_gen_ctx *gen = NULL;
  size_t i, buf_len;
  целое ret;

  если (! json_gen_init (&gen, NULL))
    вернуть crun_make_error (err, 0, "json_gen_init failed");
  json_gen_map_open (gen);

  qsort (values, len, sizeof (struct libcrun_update_value_s), compare_update_values);

  для (i = 0; i < len; i++)
    {
      если (current_section == NULL || strcmp (values[i].section, current_section))
        {
          если (i > 0)
            json_gen_map_close (gen);

          current_section = values[i].section;
          json_gen_string (gen, current_section, strlen (current_section));
          json_gen_map_open (gen);
        }

      json_gen_string (gen, values[i].name, strlen (values[i].name));

      если (values[i].numeric)
        json_gen_number (gen, values[i].value, strlen (values[i].value));
      иначе
        json_gen_string (gen, values[i].value, strlen (values[i].value));
    }
  если (len)
    json_gen_map_close (gen);

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);

  ret = libcrun_container_update (context, id, buf, buf_len, err);

  json_gen_free (gen);

  вернуть ret;
}

static void
populate_array_field (char ***field, char *array[], size_t num_elements)
{
  size_t i;

  *field = xmalloc0 ((num_elements + 1) * sizeof (char *));
  для (i = 0; i < num_elements; i++)
    (*field)[i] = xstrdup (array[i]);

  (*field)[i] = NULL;
}

#ifdef HAVE_CAP
static void
populate_capabilities (struct features_info_s *info, char ***capabilities, size_t *num_capabilities)
{
  size_t index = 0;
  cap_value_t i;
  char *endptr;
  целое j;

  *num_capabilities = 0;
  для (i = 0;; i++)
    {
      char *v = cap_to_name (i);
      если (v == NULL)
        break;
      strtol (v, &endptr, 10);
      если (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          break;
        }
      (*num_capabilities)++;
    }

  *capabilities = xmalloc0 ((*num_capabilities + 1) * sizeof (const char *));
  для (i = 0; i < (cap_value_t) *num_capabilities; i++)
    {
      char *v = cap_to_name (i);
      если (v == NULL)
        break;
      strtol (v, &endptr, 10);
      если (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          break;
        }

      // Convert capability name to uppercase
      для (j = 0; v[j] != '\0'; j++)
        v[j] = toupper (v[j]);

      (*capabilities)[index] = v;
      index++;
    }

  (*capabilities)[index] = NULL; // Terminate the array with NULL
  populate_array_field (&(info->linux.capabilities), *capabilities, *num_capabilities);
}
#endif

static void
retrieve_mount_options (struct features_info_s **info)
{
  cleanup_free const struct propagation_flags_s *mount_options_list = NULL;
  size_t num_mount_options = 0;

  // Retrieve mount options from wordlist
  mount_options_list = get_mount_flags_from_wordlist ();

  // Calculate the number of mount options
  пока (mount_options_list[num_mount_options].name != NULL)
    num_mount_options++;

  // Allocate memory for mount options in info struct
  (*info)->mount_options = xmalloc0 ((num_mount_options + 1) * sizeof (char *));

  // Copy mount options to info struct
  для (size_t i = 0; i < num_mount_options; i++)
    (*info)->mount_options[i] = xstrdup (mount_options_list[i].name);
}

целое
libcrun_container_get_features (libcrun_context_t *context, struct features_info_s **info, libcrun_error_t *err arg_unused)
{
  // Allocate memory for the features_info_s structure
  size_t num_namspaces = sizeof (namespaces) / sizeof (namespaces[0]);
  size_t num_operators = sizeof (operators) / sizeof (operators[0]);
  size_t num_actions = sizeof (actions) / sizeof (actions[0]);
  size_t num_hooks = sizeof (hooks) / sizeof (hooks[0]);
  size_t num_archs = sizeof (archs) / sizeof (archs[0]);
  size_t num_unsafe_annotations = sizeof (potentially_unsafe_annotations) / sizeof (potentially_unsafe_annotations[0]);
  cleanup_free char **capabilities = NULL;
  size_t num_capabilities = 0;
  size_t num_mempolicy_modes = sizeof (mempolicy_modes) / sizeof (mempolicy_modes[0]);
  size_t num_mempolicy_flags = sizeof (mempolicy_flags) / sizeof (mempolicy_flags[0]);

  *info = xmalloc0 (sizeof (struct features_info_s));

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
#иначе
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
    const struct scmp_version *version = seccomp_version ();
    char *version_string = NULL;

    xasprintf (&version_string, "%u.%u.%u", version->major, version->minor, version->micro);
    (*info)->annotations.io_github_seccomp_libseccomp_version = version_string;
  }
#endif

  если (context->handler_manager && handler_by_name (context->handler_manager, "wasm"))
    (*info)->annotations.run_oci_crun_wasm = true;

#если HAVE_CRIU
  (*info)->annotations.run_oci_crun_checkpoint_enabled = true;
#endif
  (*info)->annotations.run_oci_crun_commit = GIT_VERSION;
  (*info)->annotations.run_oci_crun_version = PACKAGE_VERSION;

  populate_array_field (&((*info)->potentially_unsafe_annotations), potentially_unsafe_annotations, num_unsafe_annotations);

  вернуть 0;
}

static void
features_add_string (json_gen_ctx *json_gen, const char *key, const char *value)
{
  json_gen_string (json_gen, key, strlen (key));
  json_gen_string (json_gen, value, strlen (value));
}

static void
features_add_bool (json_gen_ctx *json_gen, const char *key, целое value)
{
  json_gen_string (json_gen, key, strlen (key));
  json_gen_bool (json_gen, value);
}

static void
features_add_bool_str (json_gen_ctx *json_gen, const char *key, целое value)
{
  features_add_string (json_gen, key, value ? "true" : "false");
}

static void
features_add_array (json_gen_ctx *json_gen, const char *key, char **array)
{
  size_t i;

  json_gen_string (json_gen, key, strlen (key));
  json_gen_array_open (json_gen);
  если (array)
    для (i = 0; array[i] != NULL; i++)
      json_gen_string (json_gen, array[i], strlen (array[i]));
  json_gen_array_close (json_gen);
}

static void
features_add_linux_info (json_gen_ctx *json_gen, const struct linux_info_s *linux)
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
  если (linux->seccomp.actions)
    features_add_array (json_gen, "actions", linux->seccomp.actions);
  если (linux->seccomp.operators)
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
  если (linux->memory_policy.mode)
    features_add_array (json_gen, "modes", linux->memory_policy.mode);
  если (linux->memory_policy.flags)
    features_add_array (json_gen, "flags", linux->memory_policy.flags);
  json_gen_map_close (json_gen);

  json_gen_map_close (json_gen);
}

static void
features_add_annotations_info (json_gen_ctx *json_gen, const struct annotations_info_s *annotation)
{
  json_gen_string (json_gen, "annotations", strlen ("annotations"));
  json_gen_map_open (json_gen);

  если (! is_empty_string (annotation->io_github_seccomp_libseccomp_version))
    features_add_string (json_gen, "io.github.seccomp.libseccomp.version", annotation->io_github_seccomp_libseccomp_version);

  features_add_bool_str (json_gen, "org.opencontainers.runc.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);
  features_add_bool_str (json_gen, "run.oci.crun.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);

  features_add_string (json_gen, "run.oci.crun.commit", annotation->run_oci_crun_commit);
  features_add_string (json_gen, "run.oci.crun.version", annotation->run_oci_crun_version);

  features_add_bool_str (json_gen, "run.oci.crun.wasm", annotation->run_oci_crun_wasm);

  json_gen_map_close (json_gen);
}

целое
libcrun_container_get_features_json (libcrun_context_t *context, char **out, libcrun_error_t *err)
{
  cleanup_struct_features struct features_info_s *info = NULL;
  const char *buffer = NULL;
  json_gen_ctx *json_gen;
  size_t buffer_len;
  целое ret;

  *out = NULL;

  ret = libcrun_container_get_features (context, &info, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (! json_gen_init (&json_gen, NULL))
    вернуть crun_make_error (err, 0, "cannot initialize JSON generator");

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

  вернуть 0;
}

/* Read the container status and fail if the container is not running.  */
static целое
read_status_require_running (libcrun_context_t *context, const char *id,
                             libcrun_container_status_t *status, libcrun_error_t *err)
{
  целое ret;

  ret = libcrun_read_container_status (status, context->state_root, id, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = libcrun_is_container_running (status, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;
  если (ret == 0)
    вернуть crun_make_error (err, 0, "the container `%s` is not running", id);

  вернуть 0;
}

целое
libcrun_container_pause (libcrun_context_t *context, const char *id, libcrun_error_t *err)
{
  целое ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  вернуть libcrun_container_pause_linux (&status, err);
}

целое
libcrun_container_unpause (libcrun_context_t *context, const char *id, libcrun_error_t *err)
{
  целое ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  вернуть libcrun_container_unpause_linux (&status, err);
}

/* Translate the public options struct into the internal representation used by
   the linux/CRIU layer.  Strings are borrowed (not copied).  */
static void
cr_options_from_public (libcrun_checkpoint_restore_t *dst, struct libcrun_checkpoint_restore_options_s *src)
{
  memset (dst, 0, sizeof (*dst));
  dst->image_path = (char *) src->image_path;
  dst->work_path = (char *) src->work_path;
  dst->parent_path = (char *) src->parent_path;
  dst->console_socket = src->console_socket;
  dst->lsm_profile = (char *) src->lsm_profile;
  dst->lsm_mount_context = (char *) src->lsm_mount_context;
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

целое
libcrun_container_checkpoint (libcrun_context_t *context, const char *id,
                              struct libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  целое ret;
  const char *state_root = context->state_root;
  libcrun_container_status_t status = {};
  libcrun_checkpoint_restore_t cr;
  cleanup_container libcrun_container_t *container = NULL;

  cr_options_from_public (&cr, cr_options);

  ret = read_status_require_running (context, id, &status, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = read_container_config_from_state (&container, state_root, id, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  /* Needed to use the same paths (e.g. the shared empty directory used for
     masked paths) as the container runtime and restore do.  */
  container->context = context;

  ret = libcrun_container_checkpoint_linux (&status, container, &cr, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (! (cr_options->leave_running || cr_options->pre_dump))
    вернуть container_delete_internal (context, NULL, NULL, id, true, true, err);

  вернуть 0;
}

/* The dummy process only keeps the cgroup populated until the container is restored there.
   Its cgroup is below the container cgroup, so once the container runs it can freeze or kill
   the dummy process: never depend on it after that point.  */
static void
restore_proxy_process (целое proxy_pid_pipe)
{
  char c;

  /* Wait to be killed by the parent, or for the parent to go away.  */
  TEMP_FAILURE_RETRY (read (proxy_pid_pipe, &c, sizeof (c)));
}

static целое
restore_terminate_proxy_process (pid_t proxy_pid, const char *proxy_cgroup, pid_t container_pid, целое cgroup_manager, libcrun_error_t *err)
{
  целое mode;
  целое ret;

  если (cgroup_manager == CGROUP_MANAGER_SYSTEMD)
    {
      char ready_str[64];

      ret = snprintf (ready_str, sizeof (ready_str), "MAINPID=%d", container_pid);
      если (UNLIKELY (ret >= (целое) sizeof (ready_str)))
        вернуть crun_make_error (err, 0, "internal error: static buffer too small");

      ret = send_sd_notify (ready_str, err);
      /* Do not fail on errors.  */
      если (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  mode = libcrun_get_cgroup_mode (err);
  если (UNLIKELY (mode < 0))
    вернуть mode;

  /* SIGKILL terminates a process frozen with cgroup v2, while the cgroup v1 freezer does not
     deliver it until the cgroup is thawed, so do not wait for it there.  */
  kill (proxy_pid, SIGKILL);
  TEMP_FAILURE_RETRY (waitpid (proxy_pid, NULL, mode == CGROUP_MODE_UNIFIED ? 0 : WNOHANG));

  вернуть destroy_cgroup_path (proxy_cgroup, mode, err);
}

целое
libcrun_container_restore (libcrun_context_t *context, const char *id arg_unused,
                           struct libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  cleanup_cgroup_status struct libcrun_cgroup_status *cgroup_status = NULL;
  cleanup_container libcrun_container_t *container = NULL;
  cleanup_close целое proxy_pid_pipe0 = -1;
  cleanup_close целое proxy_pid_pipe1 = -1;
  runtime_spec_schema_config_schema *def;
  libcrun_container_status_t status = {};
  struct libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  libcrun_checkpoint_restore_t cr;
  cleanup_pid pid_t proxy_pid = -1;
  cleanup_close целое seccomp_fd = -1;
  const char *seccomp_bpf_data;
  целое proxy_pid_pipe[2];
  целое cgroup_manager;
  uid_t root_uid = -1;
  gid_t root_gid = -1;
  целое ret;

  cr_options_from_public (&cr, cr_options);

  container = libcrun_container_load_from_file ("config.json", err);
  если (container == NULL)
    вернуть -1;

  container->context = context;
  def = container->container_def;

  ret = check_config_file (def, context, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = libcrun_status_check_directories (context->state_root, context->id, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = libcrun_copy_config_file (context->id, context->state_root, container, err);
  если (UNLIKELY (ret < 0))
    goto fail;

  /* CRIU restores the seccomp filters of the checkpointed processes, but
     exec loads the filter from the seccomp.bpf file in the state directory,
     so it must be generated again for the restored container.  */
  seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  если (UNLIKELY (ret < 0))
    goto fail;

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  если (UNLIKELY (ret < 0))
    goto fail;
  close_and_reset (&seccomp_fd);

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  если (context->systemd_cgroup)
    cgroup_manager = CGROUP_MANAGER_SYSTEMD;
  иначе если (context->force_no_cgroup)
    cgroup_manager = CGROUP_MANAGER_DISABLED;

  def = container->container_def;

  /* If we are root (either on the host or in a namespace),
   * then chown the cgroup to root in the container user namespace. */
  get_root_in_the_userns (def, container->host_uid, container->host_gid, &root_uid, &root_gid);

  /* If the root in the container is different than the current root user, attempt to chown
     the std streams before entering the user namespace.  Otherwise we might lose access
     to the user (as it is not mapped in the user namespace) and cannot chown them.  */
  если (root_uid > 0 || root_gid > 0)
    {
      ret = maybe_chown_std_streams (root_uid, root_gid, err);
      если (UNLIKELY (ret < 0))
        goto fail;
    }

  /* CRIU runs as a child process, so the restored processes would inherit the session keyring.  */
  ret = setup_container_keyring (container, context, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  {
    cleanup_close целое cgroup_dirfd = -1;
    bool needs_proxy_process;
    cleanup_cgroup_preenter struct libcrun_cgroup_args cg = {
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
    status.bundle = (char *) context->bundle;
    status.rootfs = def->root->path;

    ret = libcrun_cgroup_preenter (&cg, &cgroup_dirfd, err);
    если (UNLIKELY (ret < 0))
      goto fail;

    needs_proxy_process = cgroup_dirfd < 0;

    /* If the target cgroup was already created (with cgroupfs), we can restore the container directly there.  */
    если (! needs_proxy_process)
      {
        cleanup_free char *cgroup_path = NULL;

        ret = get_cgroup_dirfd_path (cgroup_dirfd, &cgroup_path, err);
        если (UNLIKELY (ret < 0))
          goto fail;

        /* Complete the configuration for the cgroup before the container is
           restored there: CRIU resumes the restored processes before it
           returns, so the device filter and the limits must already be in
           place.  There is no process to move, as CRIU is run in the cgroup,
           and the controllers were enabled by precreate_cgroup.  */
        cg.joined = true;

        ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
        если (UNLIKELY (ret < 0))
          goto fail;

        /* Restore the container directly in the desired cgroup.  */
        status.cgroup_path = cgroup_path;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        если (UNLIKELY (ret < 0))
          goto fail;

        /* Now that the container is restored, keep any signal for it, so that
           it is forwarded to the init by wait_for_process, rather than acted
           upon by crun.  This cannot be done earlier, as CRIU, which is run as
           a child process, would inherit the blocked signals.  */
        ret = block_signals (err);
        если (UNLIKELY (ret < 0))
          goto fail;

        /* Use the container first process PID to finalize the cgroup.  */
        cg.pid = status.pid;
      }
    иначе
      {
        /* When using systemd, we need a process first that must exist before a cgroup is created.
           Create a dummy process that sits and waits until the container is restored.  */
        ret = pipe2 (proxy_pid_pipe, O_CLOEXEC);
        если (UNLIKELY (ret < 0))
          {
            ret = crun_make_error (err, errno, "pipe");
            goto fail;
          }

        /* Only used for auto cleanup.  */
        proxy_pid_pipe0 = proxy_pid_pipe[0];
        proxy_pid_pipe1 = proxy_pid_pipe[1];

        proxy_pid = fork ();
        если (UNLIKELY (proxy_pid < 0))
          {
            ret = crun_make_error (err, errno, "fork");
            goto fail;
          }

        если (proxy_pid == 0)
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
        если (UNLIKELY (ret < 0))
          goto fail;
      }

    ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
    если (UNLIKELY (ret < 0))
      goto fail;

    /* When using a dummy process, the container is restored here once the cgroup is configured.  */
    если (needs_proxy_process)
      {
        cleanup_free char *target_cgroup = NULL;
        cleanup_free char *proxy_cgroup = NULL;

        ret = libcrun_get_cgroup_process (proxy_pid, &target_cgroup, false, err);
        если (UNLIKELY (ret < 0))
          goto fail;

        /* Move the dummy process to a sub-cgroup.  */
        ret = append_paths (&proxy_cgroup, err, target_cgroup, ".proxy-crun", NULL);
        если (UNLIKELY (ret < 0))
          goto fail;

        ret = libcrun_move_process_to_cgroup (proxy_pid, 0, proxy_cgroup, true, err);
        если (UNLIKELY (ret < 0))
          goto fail;

        /* Restore the container in the same cgroup where the dummy process was.  */
        status.cgroup_path = target_cgroup;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        если (UNLIKELY (ret < 0))
          goto fail;

        /* See the comment for the other libcrun_container_restore_linux call.  */
        ret = block_signals (err);
        если (UNLIKELY (ret < 0))
          goto fail;

        /* The container is running, so do not fail because of the dummy process.  */
        ret = restore_terminate_proxy_process (proxy_pid, proxy_cgroup, status.pid, cgroup_manager, err);
        если (UNLIKELY (ret < 0))
          crun_error_release (err);
        proxy_pid = -1;

        close_and_reset (&proxy_pid_pipe1);
      }
  }

  context->detach = cr_options->detach;
  ret = write_container_status (container, context, status.pid, cgroup_status, err);
  если (UNLIKELY (ret < 0))
    goto fail;

  {
    /* Wait the same way run and exec do: apart from waiting for the container
       init, this writes the pid file, forwards any signal to the init, and
       reaps every child, not only the init.  */
    struct wait_for_process_args args = {
      .pid = status.pid,
      .context = context,
      .terminal_fd = -1,
      .notify_socket = -1,
      .container_ready_fd = NULL,
      .seccomp_notify_fd = -1,
      .seccomp_notify_plugins = NULL,
    };

    вернуть wait_for_process (&args, err);
  }

fail:
  /* Once restored, the container is running.  Without a status file nothing
     can reach it anymore, so do not leave it behind.  */
  если (status.pid > 0)
    {
      kill (status.pid, SIGKILL);
      waitpid_ignore_stopped (status.pid, NULL, 0);
    }
  если (cgroup_status)
    {
      libcrun_error_t tmp_err = NULL;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  {
    libcrun_error_t tmp_err = NULL;

    если (libcrun_container_delete_status (context->state_root, context->id, &tmp_err) < 0)
      crun_error_release (&tmp_err);
  }
  вернуть ret;
}

целое
libcrun_container_read_pids (libcrun_context_t *context, const char *id, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  cleanup_cgroup_status struct libcrun_cgroup_status *cgroup_status = NULL;
  cleanup_container_status libcrun_container_status_t status = {};
  целое ret;

  ret = libcrun_read_container_status (&status, context->state_root, id, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (status.cgroup_path == NULL || status.cgroup_path[0] == '\0')
    вернуть crun_make_error (err, 0, "the container is not using cgroups");

  cgroup_status = libcrun_cgroup_make_status (&status);

  вернуть libcrun_cgroup_read_pids (cgroup_status, recurse, pids, err);
}

целое
libcrun_container_list_json (libcrun_context_t *context, char **out, libcrun_error_t *err)
{
  libcrun_container_list_t *list = NULL, *it;
  const char *content = NULL;
  json_gen_ctx *gen = NULL;
  size_t len;
  целое ret;

  *out = NULL;

  ret = libcrun_get_containers_list (&list, context->state_root, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (! json_gen_init (&gen, NULL))
    {
      ret = crun_make_error (err, 0, "cannot allocate json generator");
      goto exit;
    }

  json_gen_config (gen, json_gen_beautify, 1);
  json_gen_array_open (gen);

  для (it = list; it; it = it->next)
    {
      libcrun_container_status_t status;
      целое running = 0;
      целое pid;
      const char *container_status = NULL;

      ret = libcrun_read_container_status (&status, context->state_root, it->name, err);
      если (UNLIKELY (ret < 0))
        goto exit;

      pid = status.pid;

      ret = libcrun_get_container_state_string (it->name, &status, context->state_root, &container_status,
                                                &running, err);
      если (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          continue;
        }

      если (! running)
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
  если (json_gen_get_buf (gen, &content, &len) != json_gen_status_ok)
    {
      ret = libcrun_make_error (err, 0, "cannot generate json list");
      goto exit;
    }

  *out = xstrdup (content);
  ret = 0;

exit:
  если (list)
    libcrun_free_containers_list (list);
  если (gen)
    json_gen_free (gen);

  вернуть ret;
}

целое
libcrun_write_json_containers_list (libcrun_context_t *context, FILE *out, libcrun_error_t *err)
{
  cleanup_free char *content = NULL;
  size_t len;
  целое ret;

  ret = libcrun_container_list_json (context, &content, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  len = strlen (content);
  если (len && fwrite (content, 1, len, out) != len)
    вернуть crun_make_error (err, errno, "error writing to file");

  вернуть 0;
}

целое
libcrun_container_update_intel_rdt (libcrun_context_t *context, const char *id, struct libcrun_intel_rdt_update *update, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = NULL;
  целое ret;

  ret = read_container_config_from_state (&container, context->state_root, id, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  вернуть libcrun_update_intel_rdt (id, container, update->l3_cache_schema, update->mem_bw_schema, update->schemata, err);
}

static целое
libcrun_container_add_or_remove_mounts_from_file (libcrun_context_t *context, const char *id, const char *file, bool add, libcrun_error_t *err)
{
  cleanup_custom_handler_instance struct custom_handler_instance_s *custom_handler = NULL;
  cleanup_container libcrun_container_t *container = NULL;
  cleanup_free runtime_spec_schema_defs_mount **mounts = NULL;
  const char *state_root = context->state_root;
  cleanup_free parser_error parser_err = NULL;
  struct parser_context pctx = { 0, stderr };
  libcrun_container_status_t status = {};
  cleanup_free char *content = NULL;
  json_object *doc = NULL;
  size_t n_mounts = 0, len, i;
  целое ret = 1;

  ret = read_all_file (file, &content, &len, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = libcrun_read_container_status (&status, state_root, id, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = read_container_config_from_state (&container, state_root, id, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  ret = parse_json_file (&doc, content, &pctx, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  если (! json_object_is_type (doc, json_type_array))
    {
      ret = crun_make_error (err, 0, "mounts must be an array");
      goto cleanup;
    }
  иначе
    {
      n_mounts = json_object_array_length (doc);

      mounts = xmalloc0 ((n_mounts + 1) * sizeof (*mounts));
      для (i = 0; i < n_mounts; i++)
        {
          mounts[i] = make_runtime_spec_schema_defs_mount (json_object_array_get_idx (doc, i), &pctx, &parser_err);
          если (mounts[i] == NULL)
            {
              ret = crun_make_error (err, 0, "cannot parse mount: %s", parser_err);
              goto cleanup;
            }
        }
    }

  если (add)
    ret = libcrun_make_runtime_mounts (container, &status, mounts, n_mounts, err);
  иначе
    ret = libcrun_destroy_runtime_mounts (container, &status, mounts, n_mounts, err);

cleanup:
  если (doc)
    json_object_put (doc);
  если (mounts)
    для (i = 0; i < n_mounts; i++)
      free_runtime_spec_schema_defs_mount (mounts[i]);
  вернуть ret;
}

целое
libcrun_container_add_mounts_from_file (libcrun_context_t *context, const char *id, const char *file, libcrun_error_t *err)
{
  вернуть libcrun_container_add_or_remove_mounts_from_file (context, id, file, true, err);
}

целое
libcrun_container_remove_mounts_from_file (libcrun_context_t *context, const char *id, const char *file, libcrun_error_t *err)
{
  вернуть libcrun_container_add_or_remove_mounts_from_file (context, id, file, false, err);
}
