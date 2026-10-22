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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <ocispec/runtime_spec_schema_config_schema.h>
#அந்தர்பாவ <stdbool.h>
#அந்தர்பாவ "container.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "json_gen_utils.h"
#அந்தர்பாவ "seccomp.h"
#அந்தர்பாவ "mempolicy.h"
#ifdef HAVE_SECCOMP
#  include <seccomp.h>
#endif
#அந்தர்பாவ "scheduler.h"
#அந்தர்பாவ "seccomp_notify.h"
#அந்தர்பாவ "custom-handler.h"
#அந்தர்பாவ <stdbool.h>
#அந்தர்பாவ <argp.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <sched.h>
#அந்தர்பாவ <sys/wait.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ "status.h"
#அந்தர்பாவ "mount_flags.h"
#அந்தர்பாவ "linux.h"
#அந்தர்பாவ "terminal.h"
#அந்தர்பாவ "io_priority.h"
#அந்தர்பாவ "cgroup.h"
#அந்தர்பாவ "cgroup-utils.h"
#அந்தர்பாவ <sys/prctl.h>
#அந்தர்பாவ <sys/auxv.h>
#அந்தர்பாவ <sys/signalfd.h>
#அந்தர்பாவ <sys/epoll.h>
#அந்தர்பாவ <sys/socket.h>
#ifdef HAVE_CAP
#  include <sys/capability.h>
#endif
#அந்தர்பாவ <sys/ioctl.h>
#அந்தர்பாவ <termios.h>
#அந்தர்பாவ <grp.h>
#அந்தர்பாவ <git-version.h>

#ifdef HAVE_SYSTEMD
#  include <systemd/sd-daemon.h>
#endif

#அந்தர்பாவ <json-c/json.h>
#அந்தர்பாவ <ocispec/json_common.h>

எண்ணிக்கை
{
  SYNC_SOCKET_SYNC_MESSAGE,
  SYNC_SOCKET_ERROR_MESSAGE,
  SYNC_SOCKET_WARNING_MESSAGE,
  SYNC_SOCKET_DEBUG_MESSAGE,
};

கட்டமைப்பு container_entrypoint_s
{
  libcrun_container_t *container;
  libcrun_context_t *context;
  முழு has_terminal_socket_pair;
  முழு terminal_socketpair[2];

  /* Used by log_write_to_sync_socket.  */
  முழு sync_socket;

  முழு seccomp_fd;
  முழு seccomp_receiver_fd;
  முழு console_socket_fd;

  முழு hooks_out_fd;
  முழு hooks_err_fd;

  /* Absolute bundle path, to be reported to hooks run in the container.  */
  மாறிலி எழுத்து *bundle;

  கட்டமைப்பு custom_handler_instance_s *custom_handler;
};

கட்டமைப்பு sync_socket_message_s
{
  முழு type;
  முழு error_மதிப்பு;
  எழுத்து message[512];
};

வகைவரை runtime_spec_schema_defs_hook hook;

// linux hooks
எழுத்து *hooks[] = {
  "prestart",
  "createRuntime",
  "createContainer",
  "startContainer",
  "poststart",
  "poststop"
};

// linux namespaces
நிலையான எழுத்து *namespaces[] = {
  "cgroup",
  "ipc",
  "mount",
  "network",
  "pid",
  "user",
  "uts"
};

நிலையான எழுத்து *actions[] = {
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

நிலையான எழுத்து *operators[] = {
  "SCMP_CMP_NE",
  "SCMP_CMP_LT",
  "SCMP_CMP_LE",
  "SCMP_CMP_EQ",
  "SCMP_CMP_GE",
  "SCMP_CMP_GT",
  "SCMP_CMP_MASKED_EQ",
};

நிலையான எழுத்து *archs[] = {
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

நிலையான எழுத்து *mempolicy_modes[] = {
  "MPOL_DEFAULT",
  "MPOL_PREFERRED",
  "MPOL_BIND",
  "MPOL_INTERLEAVE",
  "MPOL_LOCAL",
  "MPOL_PREFERRED_MANY",
  "MPOL_WEIGHTED_INTERLEAVE"
};

நிலையான எழுத்து *mempolicy_flags[] = {
  "MPOL_F_NUMA_BALANCING",
  "MPOL_F_RELATIVE_NODES",
  "MPOL_F_STATIC_NODES"
};

நிலையான எழுத்து *potentially_unsafe_annotations[] = {
  "module.wasm.image/variant",
  "io.kubernetes.cri.container-type",
  "run.oci.",
  "org.criu.",
  "krun.",
};

#define SYNC_SOCKET_MESSAGE_LEN(x, l) (offsetof (கட்டமைப்பு sync_socket_message_s, message) + l)

நிலையான முழு
sync_socket_write_செய்தி (முழு fd, முழு verbosity, முழு err_மதிப்பு, மாறிலி எழுத்து *log_செய்தி)
{
  முழு ret;
  size_t err_len;
  கட்டமைப்பு sync_socket_message_s செய்தி;
  தேர்வு (verbosity)
    {
    நிலை LIBCRUN_VERBOSITY_DEBUG:
      செய்தி.type = SYNC_SOCKET_DEBUG_MESSAGE;
      நிறுத்து;
    நிலை LIBCRUN_VERBOSITY_WARNING:
      செய்தி.type = SYNC_SOCKET_WARNING_MESSAGE;
      நிறுத்து;
    நிலை LIBCRUN_VERBOSITY_ERROR:
      செய்தி.type = SYNC_SOCKET_ERROR_MESSAGE;
      நிறுத்து;
    }
  செய்தி.error_மதிப்பு = err_மதிப்பு;

  எனில் (fd < 0)
    திரும்பு 0;

  err_len = strlen (log_செய்தி);
  எனில் (err_len >= அளவு (செய்தி.message))
    err_len = அளவு (செய்தி.message) - 1;

  memcpy (செய்தி.message, log_செய்தி, err_len);
  செய்தி.message[err_len] = '\0';

  ret = TEMP_FAILURE_RETRY (லிக (fd, &செய்தி, SYNC_SOCKET_MESSAGE_LEN (செய்தி, err_len + 1)));
  எனில் (UNLIKELY (ret < 0))
    திரும்பு -1;

  திரும்பு 0;
}

நிலையான முழு
sync_socket_write_error (முழு fd, libcrun_error_t *out_err)
{
  எனில் (fd < 0)
    திரும்பு 0;
  திரும்பு sync_socket_write_செய்தி (fd, false, (*out_err)->status, (*out_err)->செய்தி);
}

நிலையான வெறுமை
log_write_to_sync_socket (முழு errno_, மாறிலி எழுத்து *செய்தி, முழு verbosity, வெறுமை *arg)
{
  கட்டமைப்பு container_entrypoint_s *entrypoint_args = arg;
  முழு fd = entrypoint_args->sync_socket;

  எனில் (fd < 0)
    திரும்பு;

  எனில் (sync_socket_write_செய்தி (fd, verbosity, errno_, செய்தி) < 0)
    log_write_to_stderr (errno_, செய்தி, verbosity, arg);
}

நிலையான bool
is_memory_limit_too_கீழ் (runtime_spec_schema_config_schema *def)
{
  மாறிலி long memory_limit_too_கீழ் = 1024 * 1024;

  எனில் (def->linux == வெற்று || def->linux->resources == வெற்று)
    திரும்பு false;

  எனில் (def->linux->resources->memory
      && def->linux->resources->memory->limit_present
      && def->linux->resources->memory->limit < memory_limit_too_கீழ்)
    திரும்பு true;

  எனில் (def->linux->resources->unified)
    {
      size_t i;

      க்கு (i = 0; i < def->linux->resources->unified->len; i++)
        எனில் (strcmp (def->linux->resources->unified->சாவிகள்[i], "memory.max") == 0)
          {
            long limit;

            errno = 0;
            limit = strtol (def->linux->resources->unified->மதிப்புகள்[i], வெற்று, 10);
            எனில் (errno != 0)
              திரும்பு false;
            எனில் (limit < memory_limit_too_கீழ்)
              திரும்பு true;
          }
    }

  திரும்பு false;
}

நிலையான முழு
sync_socket_wait_sync (libcrun_context_t *context, முழு fd, bool flush, libcrun_error_t *err)
{
  கட்டமைப்பு sync_socket_message_s செய்தி;

  எனில் (fd < 0)
    திரும்பு 0;

  போது (true)
    {
      முழு ret;

      ret = TEMP_FAILURE_RETRY (பட (fd, &செய்தி, அளவு (செய்தி)));
      எனில் (UNLIKELY (ret < 0))
        {
          எனில் (flush)
            திரும்பு 0;
          திரும்பு crun_make_error (err, errno, "read from sync socket");
        }

      எனில் (ret == 0)
        {
          எனில் (flush)
            திரும்பு 0;

          திரும்பு crun_make_error (err, 0, "read from the init process");
        }

      எனில் (! flush && செய்தி.type == SYNC_SOCKET_SYNC_MESSAGE)
        திரும்பு 0;
      இல்லையெனில் எனில் (செய்தி.type == SYNC_SOCKET_DEBUG_MESSAGE)
        {
          எனில் (context)
            context->output_handler (செய்தி.error_மதிப்பு, செய்தி.message, LIBCRUN_VERBOSITY_DEBUG, context->output_handler_arg);
          தொடர்;
        }
      இல்லையெனில் எனில் (செய்தி.type == SYNC_SOCKET_WARNING_MESSAGE)
        {
          எனில் (context)
            context->output_handler (செய்தி.error_மதிப்பு, செய்தி.message, LIBCRUN_VERBOSITY_WARNING, context->output_handler_arg);
          தொடர்;
        }
      இல்லையெனில் எனில் (செய்தி.type == SYNC_SOCKET_ERROR_MESSAGE)
        திரும்பு crun_make_error (err, செய்தி.error_மதிப்பு, "%s", செய்தி.message);
    }
}

நிலையான முழு
sync_socket_send_sync (முழு fd, bool flush_errors, libcrun_error_t *err)
{
  முழு ret;
  கட்டமைப்பு sync_socket_message_s செய்தி = {
    0,
  };
  செய்தி.type = SYNC_SOCKET_SYNC_MESSAGE;

  எனில் (fd < 0)
    திரும்பு 0;

  ret = TEMP_FAILURE_RETRY (லிக (fd, &செய்தி, SYNC_SOCKET_MESSAGE_LEN (செய்தி, 0)));
  எனில் (UNLIKELY (ret < 0))
    {
      எனில் (flush_errors)
        {
          முழு saved_errno = errno;
          ret = TEMP_FAILURE_RETRY (பட (fd, &செய்தி, அளவு (செய்தி)));
          எனில் (ret >= 0 && செய்தி.type == SYNC_SOCKET_ERROR_MESSAGE)
            திரும்பு crun_make_error (err, செய்தி.error_மதிப்பு, "%s", செய்தி.message);
          errno = saved_errno;
        }
      திரும்பு crun_make_error (err, errno, "write to sync socket");
    }

  திரும்பு 0;
}

நிலையான bool
has_cap_sys_ptrace ()
{
#ifdef HAVE_CAP
  கட்டமைப்பு __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  கட்டமைப்பு __user_cap_data_struct data[2] = { { 0 } };

  எனில் (UNLIKELY (capget (&hdr, data) < 0))
    திரும்பு true;

  திரும்பு (data[0].effective & (1U << CAP_SYS_PTRACE)) != 0;
#இல்லையெனில்
  திரும்பு geteuid () == 0;
#endif
}

நிலையான libcrun_container_t *
make_container (runtime_spec_schema_config_schema *container_def, மாறிலி எழுத்து *path, மாறிலி எழுத்து *config)
{
  libcrun_container_t *container = xmalloc0 (அளவு (*container));
  container->container_def = container_def;

  container->host_uid = geteuid ();
  container->host_gid = getegid ();
  container->host_has_cap_sys_ptrace = has_cap_sys_ptrace ();
  container->proc_fd = -1;

  container->annotations = make_string_map_from_json (container_def->annotations);

  /* An empty namespace path means that a new namespace is to be created,
     the same as if it is not specified.  */
  எனில் (container_def->linux)
    {
      size_t i;

      க்கு (i = 0; i < container_def->linux->namespaces_len; i++)
        எனில் (container_def->linux->namespaces[i]->path && container_def->linux->namespaces[i]->path[0] == '\0')
          {
            விடுவி (container_def->linux->namespaces[i]->path);
            container_def->linux->namespaces[i]->path = வெற்று;
          }
    }

  எனில் (path)
    container->config_file = xstrdup (path);
  எனில் (config)
    container->config_file_content = xstrdup (config);

  திரும்பு container;
}

libcrun_container_t *
libcrun_container_load_from_memory (மாறிலி எழுத்து *json, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free எழுத்து *oci_error = வெற்று;
  container_def = runtime_spec_schema_config_schema_parse_data (json, வெற்று, &oci_error);
  எனில் (container_def == வெற்று)
    {
      crun_make_error (err, 0, "load: `%s`", oci_error);
      திரும்பு வெற்று;
    }
  திரும்பு make_container (container_def, வெற்று, json);
}

libcrun_container_t *
libcrun_container_load_from_file (மாறிலி எழுத்து *path, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free எழுத்து *oci_error = வெற்று;
  cleanup_free எழுத்து *content = வெற்று;
  size_t len;
  முழு ret;

  libcrun_debug ("Loading container from config file: `%s`", path);
  ret = read_all_file (path, &content, &len, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு வெற்று;

  container_def = runtime_spec_schema_config_schema_parse_data (content, வெற்று, &oci_error);
  எனில் (container_def == வெற்று)
    {
      crun_make_error (err, 0, "load `%s`: %s", path, oci_error);
      திரும்பு வெற்று;
    }
  திரும்பு make_container (container_def, வெற்று, content);
}

மாறிலி எழுத்து *
libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err)
{
  கட்டமைப்பு parser_context ctx = { 0, stderr };
  parser_error gen_err = வெற்று;

  எனில் (container == வெற்று)
    {
      crun_make_error (err, EINVAL, "no container specified");
      திரும்பு வெற்று;
    }

  எனில் (container->config_file_content == வெற்று && container->container_def)
    {
      container->config_file_content = runtime_spec_schema_config_schema_generate_json (container->container_def, &ctx, &gen_err);
      எனில் (container->config_file_content == வெற்று)
        {
          crun_make_error (err, 0, "cannot generate the configuration JSON: %s", gen_err ?: "unknown error");
          விடுவி (gen_err);
          திரும்பு வெற்று;
        }
      விடுவி (gen_err);
    }

  எனில் (container->config_file_content == வெற்று)
    {
      crun_make_error (err, 0, "the container has no configuration");
      திரும்பு வெற்று;
    }

  திரும்பு container->config_file_content;
}

மாறிலி எழுத்து *
libcrun_container_get_annotation (libcrun_container_t *container, மாறிலி எழுத்து *சாவி)
{
  எனில் (container == வெற்று)
    திரும்பு வெற்று;
  திரும்பு find_string_map_மதிப்பு (container->annotations, சாவி);
}

size_t
libcrun_container_get_annotations_len (libcrun_container_t *container)
{
  எனில் (container == வெற்று || container->annotations == வெற்று)
    திரும்பு 0;
  திரும்பு string_map_size (container->annotations);
}

முழு
libcrun_container_get_annotation_at (libcrun_container_t *container, size_t index, மாறிலி எழுத்து **சாவி,
                                     மாறிலி எழுத்து **மதிப்பு)
{
  எனில் (container == வெற்று)
    {
      errno = ERANGE;
      திரும்பு -1;
    }
  திரும்பு string_map_get_at (container->annotations, index, சாவி, மதிப்பு);
}

uid_t
libcrun_container_get_uid (libcrun_container_t *container)
{
  எனில் (container == வெற்று)
    திரும்பு (uid_t) -1;
  திரும்பு container->host_uid;
}

gid_t
libcrun_container_get_gid (libcrun_container_t *container)
{
  எனில் (container == வெற்று)
    திரும்பு (gid_t) -1;
  திரும்பு container->host_gid;
}

வெறுமை
libcrun_container_free (libcrun_container_t *ctr)
{
  எனில் (ctr == வெற்று)
    திரும்பு;

  எனில் (ctr->cleanup_private_data)
    ctr->cleanup_private_data (ctr->private_data);

  எனில் (ctr->container_def)
    free_runtime_spec_schema_config_schema (ctr->container_def);

  free_string_map (ctr->annotations);

  எனில் (ctr->proc_fd >= 0)
    close (ctr->proc_fd);

  விடுவி (ctr->config_file_content);
  விடுவி (ctr->config_file);
  விடுவி (ctr);
}

libcrun_context_t *
libcrun_context_new (மாறிலி எழுத்து *id, மாறிலி எழுத்து *state_வேர், libcrun_error_t *err)
{
  libcrun_context_t *ctx = xmalloc0 (அளவு (*ctx));

  /* Go through the setters so that the owned_* copies and the borrowed
     pointers stay in sync and are released by libcrun_context_free.  */
  libcrun_context_set_id (ctx, id);
  libcrun_context_set_state_வேர் (ctx, state_வேர்);
  libcrun_context_set_bundle (ctx, ".");
  ctx->fifo_exec_wait_fd = -1;

  எனில் (libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, id, வெற்று, err) < 0)
    {
      libcrun_context_free (ctx);
      திரும்பு வெற்று;
    }

  ctx->handler_manager = libcrun_handler_manager_create (err);
  எனில் (ctx->handler_manager == வெற்று)
    {
      libcrun_context_free (ctx);
      திரும்பு வெற்று;
    }

#ifdef CRUN_LIBDIR
  எனில் (access (CRUN_LIBDIR "/handlers", F_OK) == 0)
    {
      எனில் (libcrun_handler_manager_load_directory (ctx->handler_manager, CRUN_LIBDIR "/handlers", err) < 0)
        {
          libcrun_context_free (ctx);
          திரும்பு வெற்று;
        }
    }
#endif

  திரும்பு ctx;
}

வெறுமை
libcrun_context_free (libcrun_context_t *ctx)
{
  எனில் (ctx == வெற்று)
    திரும்பு;

  விடுவி (ctx->owned_state_வேர்);
  விடுவி (ctx->owned_id);
  விடுவி (ctx->owned_bundle);
  விடுவி (ctx->owned_console_socket);
  விடுவி (ctx->owned_pid_file);
  விடுவி (ctx->owned_notify_socket);
  விடுவி (ctx->owned_handler);

  எனில் (ctx->handler_manager)
    handler_manager_free (ctx->handler_manager);

  விடுவி (ctx);
}

#define LIBCRUN_CONTEXT_SET_STRING(field)                  \
  செய்                                                       \
    {                                                      \
      விடுவி (ctx->owned_##field);                           \
      ctx->owned_##field = மதிப்பு ? xstrdup (மதிப்பு) : வெற்று; \
      ctx->field = ctx->owned_##field;                     \
  } போது (0)

வெறுமை
libcrun_context_set_id (libcrun_context_t *ctx, மாறிலி எழுத்து *மதிப்பு)
{
  LIBCRUN_CONTEXT_SET_STRING (id);
}

வெறுமை
libcrun_context_set_state_வேர் (libcrun_context_t *ctx, மாறிலி எழுத்து *மதிப்பு)
{
  LIBCRUN_CONTEXT_SET_STRING (state_வேர்);
}

வெறுமை
libcrun_context_set_bundle (libcrun_context_t *ctx, மாறிலி எழுத்து *மதிப்பு)
{
  LIBCRUN_CONTEXT_SET_STRING (bundle);
}

வெறுமை
libcrun_context_set_console_socket (libcrun_context_t *ctx, மாறிலி எழுத்து *மதிப்பு)
{
  LIBCRUN_CONTEXT_SET_STRING (console_socket);
}

வெறுமை
libcrun_context_set_pid_file (libcrun_context_t *ctx, மாறிலி எழுத்து *மதிப்பு)
{
  LIBCRUN_CONTEXT_SET_STRING (pid_file);
}

வெறுமை
libcrun_context_set_notify_socket (libcrun_context_t *ctx, மாறிலி எழுத்து *மதிப்பு)
{
  LIBCRUN_CONTEXT_SET_STRING (notify_socket);
}

வெறுமை
libcrun_context_set_handler (libcrun_context_t *ctx, மாறிலி எழுத்து *மதிப்பு)
{
  LIBCRUN_CONTEXT_SET_STRING (handler);
}

#undef LIBCRUN_CONTEXT_SET_STRING

மாறிலி எழுத்து *
libcrun_context_get_pid_file (libcrun_context_t *ctx)
{
  திரும்பு ctx->pid_file;
}

மாறிலி எழுத்து *
libcrun_context_get_state_வேர் (libcrun_context_t *ctx)
{
  எனில் (ctx == வெற்று)
    திரும்பு வெற்று;
  திரும்பு ctx->state_வேர்;
}

வெறுமை
libcrun_context_set_preserve_fds (libcrun_context_t *ctx, முழு preserve_fds)
{
  ctx->preserve_fds = preserve_fds;
}

முழு
libcrun_context_get_preserve_fds (libcrun_context_t *ctx)
{
  திரும்பு ctx->preserve_fds;
}

வெறுமை
libcrun_context_set_listen_fds (libcrun_context_t *ctx, முழு listen_fds)
{
  ctx->listen_fds = listen_fds;
}

வெறுமை
libcrun_context_set_args (libcrun_context_t *ctx, முழு argc, எழுத்து **argv)
{
  ctx->argc = argc;
  ctx->argv = argv;
}

வெறுமை
libcrun_context_set_systemd_cgroup (libcrun_context_t *ctx, bool மதிப்பு)
{
  ctx->systemd_cgroup = மதிப்பு;
}

வெறுமை
libcrun_context_set_detach (libcrun_context_t *ctx, bool மதிப்பு)
{
  ctx->detach = மதிப்பு;
}

வெறுமை
libcrun_context_set_no_new_keyring (libcrun_context_t *ctx, bool மதிப்பு)
{
  ctx->no_new_keyring = மதிப்பு;
}

வெறுமை
libcrun_context_set_no_pivot (libcrun_context_t *ctx, bool மதிப்பு)
{
  ctx->no_pivot = மதிப்பு;
}

வெறுமை
libcrun_context_set_force_no_cgroup (libcrun_context_t *ctx, bool மதிப்பு)
{
  ctx->force_no_cgroup = மதிப்பு;
}

வெறுமை
libcrun_context_set_output_handler (libcrun_context_t *ctx, crun_output_handler handler, வெறுமை *arg)
{
  ctx->output_handler = handler;
  ctx->output_handler_arg = arg;
}

முழு
libcrun_context_init_logging (libcrun_context_t *ctx, மாறிலி எழுத்து *log, libcrun_error_t *err)
{
  திரும்பு libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, ctx->id, log, err);
}

நிலையான முழு
block_signals (libcrun_error_t *err)
{
  முழு ret;
  sigset_t mask;
  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "sigprocmask");
  திரும்பு 0;
}

நிலையான முழு
unblock_signals (libcrun_error_t *err)
{
  முழு i;
  முழு ret;
  sigset_t mask;
  கட்டமைப்பு sigaction act = {};

  sigfillset (&mask);
  ret = sigprocmask (SIG_UNBLOCK, &mask, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "sigprocmask");

  act.sa_handler = SIG_DFL;
  க்கு (i = 0; i < NSIG; i++)
    {
      ret = sigaction (i, &act, வெற்று);
      எனில் (ret < 0 && errno != EINVAL)
        திரும்பு crun_make_error (err, errno, "sigaction");
    }

  திரும்பு 0;
}

/* must be used on the host before pivot_root(2).  */
நிலையான முழு
initialize_security (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, libcrun_error_t *err)
{
  முழு ret;

  ret = libcrun_init_caps (container, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (UNLIKELY (proc == வெற்று))
    திரும்பு 0;

  எனில் (proc->apparmor_profile)
    {
      ret = libcrun_initialize_apparmor (err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  ret = libcrun_initialize_selinux (container, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு 0;
}

/* Emit the "annotations" map.  Returns a json_gen_status; callers can wrap
   the call with GEN_OR_FAIL.  */
நிலையான முழு
gen_annotations (json_gen_ctx *gen, json_map_string_string *annotations)
{
  size_t i;
  முழு r;

  GEN_சாவி (gen, "annotations");
  GEN_OR_FAIL (json_gen_map_open (gen));

  க்கு (i = 0; i < annotations->len; i++)
    {
      /* An annotation whose value is null in the document is parsed into a
         NULL, which cannot be emitted as a JSON string; leave it out.  */
      எனில் (annotations->சாவிகள்[i] == வெற்று || annotations->மதிப்புகள்[i] == வெற்று)
        தொடர்;

      GEN_STR (gen, annotations->சாவிகள்[i]);
      GEN_STR (gen, annotations->மதிப்புகள்[i]);
    }

  திரும்பு json_gen_map_close (gen);

gen_error:
  திரும்பு r;
}

/* Return the bundle path to report in the container state.  crun always
   chdir()s into the bundle, so fall back to getcwd() when the caller has no
   absolute bundle path, as can happen when libcrun is used as a library.
   *ALLOCATED must be freed by the caller.  */
நிலையான மாறிலி எழுத்து *
get_bundle_path (மாறிலி எழுத்து *bundle, எழுத்து **allocated)
{
  எனில் (bundle != வெற்று && bundle[0] == '/')
    திரும்பு bundle;

  *allocated = getcwd (வெற்று, 0);
  எனில் (*allocated == வெற்று)
    OOM ();
  திரும்பு *allocated;
}

நிலையான முழு
do_hooks (runtime_spec_schema_config_schema *def, pid_t pid, மாறிலி எழுத்து *id, bool keep_going, மாறிலி எழுத்து *bundle,
          மாறிலி எழுத்து *cwd, மாறிலி எழுத்து *status, மாறிலி எழுத்து *name, hook **hooks, size_t hooks_len, முழு out_fd,
          முழு err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  size_t i, stdin_len;
  முழு r, ret;
  எழுத்து *stdin = வெற்று;
  cleanup_free எழுத்து *bundle_allocated = வெற்று;
  மாறிலி எழுத்து *rootfs = def->வேர் ? def->வேர்->path : "";
  cleanup_json_gen json_gen_ctx *gen = வெற்று;

  bundle = get_bundle_path (bundle, &bundle_allocated);

  எனில் (! json_gen_init (&gen, வெற்று))
    திரும்பு crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 0);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_சாவி (gen, "ociVersion");
  GEN_STR (gen, "1.0");

  GEN_சாவி (gen, "id");
  GEN_STR (gen, id);

  GEN_சாவி (gen, "pid");
  GEN_OR_FAIL (map_int (gen, pid));

  GEN_சாவி (gen, "root");
  GEN_STR (gen, rootfs);

  GEN_சாவி (gen, "bundle");
  GEN_STR (gen, bundle);

  GEN_சாவி (gen, "status");
  GEN_STR (gen, status);

  எனில் (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, (மாறிலி எழுத்து **) &stdin, &stdin_len));

  ret = 0;

  bool error_created = false;
  க்கு (i = 0; i < hooks_len; i++)
    {
      எழுத்து **env = environ;

      எனில் (hooks[i]->env)
        env = hooks[i]->env;

      /* Release the error from the previous iteration, if any.  */
      எனில் (error_created)
        {
          crun_error_release (err);
          error_created = false;
        }

      ret = run_process_with_stdin_timeout_envp (hooks[i]->path, hooks[i]->args, cwd, hooks[i]->timeout, env,
                                                 stdin, stdin_len, out_fd, err_fd, can_ignore_chdir_errors, err);
      எனில் (UNLIKELY (ret < 0))
        error_created = true;

      எனில் (UNLIKELY (ret != 0))
        {
          எனில் (keep_going)
            libcrun_warning ("error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
          இல்லையெனில்
            {
              libcrun_error (0, "error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
              நிறுத்து;
            }
        }
    }

  திரும்பு ret;

gen_error:
  திரும்பு json_gen_error_to_crun_error (r, err);
}

நிலையான முழு
get_json_gen_result (json_gen_ctx *gen, எழுத்து **out, size_t *out_len)
{
  மாறிலி எழுத்து *buf = வெற்று;
  size_t buf_len = 0;
  முழு r;

  r = json_gen_get_buf (gen, &buf, &buf_len);
  எனில் (UNLIKELY (r != json_gen_status_ok))
    திரும்பு r;

  *out_len = buf_len;

  *out = xmalloc (buf_len + 1);
  memcpy (*out, buf, buf_len);
  (*out)[buf_len] = '\0';

  திரும்பு json_gen_status_ok;
}

நிலையான முழு
get_seccomp_receiver_fd_payload (libcrun_container_t *container, மாறிலி எழுத்து *status, pid_t own_pid,
                                 எழுத்து **seccomp_fd_payload, size_t *seccomp_fd_payload_len, libcrun_error_t *err)
{
  முழு r;
  json_gen_ctx *gen = வெற்று;
  runtime_spec_schema_config_schema *def = container->container_def;
  மாறிலி எழுத்து *மாறிலி OCI_VERSION = "0.2.0";

  எனில் (! json_gen_init (&gen, வெற்று))
    திரும்பு crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_சாவி (gen, "ociVersion");
  GEN_STR (gen, OCI_VERSION);

  GEN_சாவி (gen, "fds");
  GEN_OR_FAIL (json_gen_array_open (gen));

  GEN_சாவி (gen, "seccompFd");
  GEN_OR_FAIL (json_gen_array_close (gen));

  GEN_சாவி (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  எனில் (def && def->linux && def->linux->seccomp)
    {
      மாறிலி எழுத்து *metadata = def->linux->seccomp->listener_metadata;

      எனில் (metadata)
        {
          GEN_சாவி (gen, "metadata");
          GEN_STR (gen, metadata);
        }
    }

  /* State.  */
  GEN_சாவி (gen, "state");
  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_சாவி (gen, "ociVersion");
  GEN_STR (gen, OCI_VERSION);

  எனில் (container->context && container->context->id)
    {
      GEN_சாவி (gen, "id");
      GEN_STR (gen, container->context->id);
    }

  GEN_சாவி (gen, "status");
  GEN_STR (gen, status);

  GEN_சாவி (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  எனில் (container->context && container->context->bundle)
    {
      GEN_சாவி (gen, "bundle");
      GEN_STR (gen, container->context->bundle);
    }

  எனில் (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));
  /* End state.  */

  GEN_OR_FAIL (json_gen_map_close (gen));

  r = get_json_gen_result (gen, seccomp_fd_payload, seccomp_fd_payload_len);

gen_error:
  json_gen_free (gen);

  திரும்பு json_gen_error_to_crun_error (r, err);
}

நிலையான முழு
send_sync_cb (வெறுமை *data, libcrun_error_t *err)
{
  முழு sync_socket_fd = *((முழு *) data);
  முழு ret;

  /* sync 2.  */
  ret = sync_socket_send_sync (sync_socket_fd, false, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  /* sync 3.  */
  திரும்பு sync_socket_wait_sync (வெற்று, sync_socket_fd, false, err);
}

நிலையான முழு
maybe_chown_std_streams (uid_t container_uid, gid_t container_gid, libcrun_error_t *err)
{
  முழு ret, i;

  க்கு (i = 0; i < 3; i++)
    {
      எனில் (! isatty (i))
        {
          கட்டமைப்பு stat statbuf;
          ret = fstat (i, &statbuf);
          எனில் (UNLIKELY (ret < 0))
            {
              எனில் (errno == EBADF)
                தொடர்;
              திரும்பு crun_make_error (err, errno, "fstat fd `%d`", i);
            }

          /* Skip chown for device files */
          எனில் (S_ISCHR (statbuf.st_mode) || S_ISBLK (statbuf.st_mode))
            தொடர்;

          ret = fchown (i, container_uid, container_gid);
          எனில் (UNLIKELY (ret < 0))
            {
              /* EINVAL means the user is not mapped in the current userns.
                 Ignore EPERM and EROFS as well as there is no reason to fail
                 so early, and let the container payload deal with it.
                 EBADF means fd is closed.
              */
              எனில் (errno == EINVAL || errno == EPERM || errno == EROFS || errno == EBADF)
                தொடர்;

              திரும்பு crun_make_error (err, errno, "fchown std stream `%i`", i);
            }
        }
    }
  திரும்பு 0;
}

முழு
libcrun_container_notify_handler (கட்டமைப்பு container_entrypoint_s *args,
                                  எண்ணிக்கை handler_configure_phase phase,
                                  libcrun_container_t *container, மாறிலி எழுத்து *rootfs,
                                  libcrun_error_t *err)
{
  கட்டமைப்பு custom_handler_s *h;

  எனில் (args->custom_handler == வெற்று)
    திரும்பு 0;

  h = args->custom_handler->vtable;
  எனில் (h == வெற்று || h->configure_container == வெற்று)
    திரும்பு 0;

  திரும்பு h->configure_container (args->custom_handler->cookie, phase,
                                 args->context, container,
                                 rootfs, err);
}

/* Resolve and normalize the container rootfs path.  */
நிலையான முழு
resolve_rootfs_path (libcrun_container_t *container, எழுத்து **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  எனில் (def->வேர் && def->வேர்->path)
    {
      *rootfs = realpath (def->வேர்->path, வெற்று);
      எனில் (UNLIKELY (*rootfs == வெற்று))
        {
          முழு saved_errno = errno;

          /* If realpath failed for any reason, try the relative directory.  */
          எனில் (def->வேர்->path[0] == '/')
            {
              cleanup_free எழுத்து *cwd = வெற்று;
              libcrun_error_t tmp_err = வெற்று;
              ssize_t len;
              முழு proc_fd;

              proc_fd = libcrun_get_cached_proc_fd (container, &tmp_err);
              எனில் (LIKELY (proc_fd >= 0))
                {
                  len = safe_readlinkat (proc_fd, "self/cwd", &cwd, 0, &tmp_err);
                  எனில் (LIKELY (len > 0))
                    {
                      /* If the rootfs is under the current working directory, just use its relative path.  */
                      எனில் (has_dir_prefix (def->வேர்->path, cwd))
                        {
                          மாறிலி எழுத்து *it = consume_slashes (def->வேர்->path + len);
                          எனில் (LIKELY (*it))
                            {
                              முழு ret = faccessat (AT_FDCWD, it, F_OK, 0);
                              எனில் (LIKELY (ret == 0))
                                *rootfs = xstrdup (it);
                            }
                        }
                    }
                }
              crun_error_release (&tmp_err);
            }

          எனில் (*rootfs == வெற்று)
            {
              எனில் (saved_errno == ENOENT)
                திரும்பு crun_make_error (err, ENOENT, "the rootfs path `%s` does not exist", def->வேர்->path);

              *rootfs = xstrdup (def->வேர்->path);
            }
        }
    }
  திரும்பு 0;
}

/* Initialize the environment variables.  */
நிலையான முழு
setup_environment (runtime_spec_schema_config_schema *def, uid_t container_uid, libcrun_error_t *err)
{
  முழு ret;

  எனில் (clearenv ())
    திரும்பு crun_make_error (err, errno, "clearenv");

  எனில் (def->process)
    {
      size_t i;

      க்கு (i = 0; i < def->process->env_len; i++)
        {
          /* A null element of the array of variables is parsed into a NULL,
             which putenv(3) cannot be handed.  */
          எனில் (def->process->env[i] == வெற்று)
            திரும்பு crun_make_error (err, EINVAL, "environment variable is not specified");

          எனில் (putenv (def->process->env[i]) < 0)
            திரும்பு crun_make_error (err, errno, "putenv `%s`", def->process->env[i]);
        }
    }

  எனில் (is_empty_string (getenv ("HOME")))
    {
      ret = set_home_env (container_uid);
      எனில் (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          எனில் (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            திரும்பு crun_make_error (err, errno, "setenv HOME");
        }
    }

  திரும்பு 0;
}

/* Configure terminal and console for interactive containers.  */
நிலையான முழு
setup_terminal (கட்டமைப்பு container_entrypoint_s *entrypoint_args, libcrun_container_t *container,
                முழு console_socket, முழு console_socketpair, முழு has_terminal, libcrun_error_t *err)
{
  cleanup_close முழு terminal_fd = -1;
  முழு ret;

  எனில் (! has_terminal)
    திரும்பு 0;

  fflush (stderr);

  எனில் (console_socket >= 0 || (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0))
    {
      terminal_fd = libcrun_set_terminal (container, err);
      எனில் (UNLIKELY (terminal_fd < 0))
        திரும்பு terminal_fd;
    }

  எனில் (console_socket >= 0)
    {
      ret = send_fd_to_socket (console_socket, terminal_fd, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  இல்லையெனில் எனில் (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0)
    {
      ret = send_fd_to_socket (console_socketpair, terminal_fd, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  திரும்பு 0;
}

/* Resolve and validate the container's executable path.  */
நிலையான முழு
setup_executable_path (கட்டமைப்பு container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                       எழுத்து **exec_path, libcrun_error_t *err)
{
  முழு ret;

  எனில் (def->process && def->process->args)
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      எனில் (UNLIKELY (ret < 0))
        {
          எனில் (entrypoint_args->custom_handler == வெற்று && crun_error_get_errno (err) == ENOENT)
            திரும்பு ret;

          /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
             once the process switched to the use that runs in the container.  This might be necessary
             when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
             is not honored.  */
          crun_error_release (err);
        }
    }

  திரும்பு 0;
}

/* Apply security settings for the container process.  */
நிலையான முழு
apply_security_settings (கட்டமைப்பு container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                         libcrun_container_t *container, pid_t own_pid, libcrun_error_t *err)
{
  முழு ret;
  runtime_spec_schema_config_schema_process_capabilities *capabilities;
  முழு no_new_privs;

  எனில் (def->process && ! def->process->no_new_privileges)
    {
      எழுத்து **seccomp_flags = வெற்று;
      size_t seccomp_flags_len = 0;
      cleanup_free எழுத்து *seccomp_fd_payload = வெற்று;
      size_t seccomp_fd_payload_len = 0;

      எனில் (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      எனில் (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  capabilities = def->process ? def->process->capabilities : வெற்று;
  no_new_privs = def->process ? def->process->no_new_privileges : 1;
  ret = libcrun_set_caps (capabilities, container->container_uid, container->container_gid, no_new_privs,
                          entrypoint_args->custom_handler != வெற்று, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு 0;
}

/* Create the process that joins the existing PID namespace now that the
   host mounts are not visible anymore, and continue the setup from there.  */
நிலையான முழு
do_delayed_pidns_fork (முழு sync_socket, pid_t *own_pid, libcrun_error_t *err)
{
  pid_t pid;
  முழு ret;

  pid = fork ();
  எனில் (UNLIKELY (pid < 0))
    திரும்பு crun_make_error (err, errno, "fork");

  எனில் (pid)
    {
      /* Report the new PID to the parent and exit immediately.  */
      ret = sync_socket_send_sync (sync_socket, false, err);
      எனில் (LIKELY (ret == 0))
        {
          ret = TEMP_FAILURE_RETRY (லிக (sync_socket, &pid, அளவு (pid)));
          எனில் (LIKELY (ret == அளவு (pid)))
            _safe_exit (EXIT_SUCCESS);
        }

      kill (pid, SIGKILL);
      _safe_exit (EXIT_FAILURE);
    }

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (பட (sync_socket, own_pid, அளவு (*own_pid)));
  எனில் (UNLIKELY (ret != அளவு (*own_pid)))
    {
      எனில் (ret >= 0)
        errno = 0;
      திரும்பு crun_make_error (err, errno, "read from sync socket");
    }

  திரும்பு 0;
}

கட்டமைப்பு delayed_pidns_fork_s
{
  libcrun_container_t *container;
  முழு sync_socket;
  pid_t *own_pid;
};

நிலையான முழு
delayed_pidns_fork_cb (வெறுமை *data, libcrun_error_t *err)
{
  கட்டமைப்பு delayed_pidns_fork_s *d = data;

  எனில் (! libcrun_container_delayed_pidns_fork (d->container))
    திரும்பு 0;

  திரும்பு do_delayed_pidns_fork (d->sync_socket, d->own_pid, err);
}

நிலையான முழு
container_init_setup (வெறுமை *args, pid_t *own_pid, எழுத்து *notify_socket,
                      முழு sync_socket, எழுத்து **exec_path, libcrun_error_t *err)
{
  கட்டமைப்பு container_entrypoint_s *entrypoint_args = args;
  libcrun_container_t *container = entrypoint_args->container;
  bool chdir_done = false;
  முழு ret;
  முழு has_terminal;
  cleanup_close முழு console_socket = -1;
  cleanup_close முழு console_socketpair = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free எழுத்து *rootfs = வெற்று;
  கட்டமைப்பு delayed_pidns_fork_s delayed_pidns_fork = {
    .container = container,
    .sync_socket = sync_socket,
    .own_pid = own_pid,
  };

  ret = initialize_security (container, def->process, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_configure_network (container, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = resolve_rootfs_path (container, &rootfs, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  /* Configure terminal socket pair for container communication.  */
  எனில் (entrypoint_args->terminal_socketpair[0] >= 0)
    {
      close_and_reset (&entrypoint_args->terminal_socketpair[0]);
      console_socketpair = entrypoint_args->terminal_socketpair[1];
    }

  /* sync 1.  */
  ret = sync_socket_wait_sync (வெற்று, sync_socket, false, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  has_terminal = container->container_def->process && container->container_def->process->terminal;
  எனில் (has_terminal && entrypoint_args->context->console_socket)
    console_socket = entrypoint_args->console_socket_fd;

  ret = libcrun_set_sysctl (container, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_BEFORE_MOUNTS, container, rootfs, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_do_pivot_வேர் (container, entrypoint_args->context->no_pivot, &rootfs, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  /* sync 2 and 3 are sent as part of libcrun_set_mounts.  */
  ret = libcrun_set_mounts (entrypoint_args, container, rootfs, send_sync_cb, &sync_socket, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  /* The createContainer hooks MUST run after the mounts are set up but before
     the pivot_root.  When these hooks are present can_use_open_tree_namespace()
     forces the deferred-pivot path, so the actual pivot happens later in
     libcrun_finalize_mounts().  */
  எனில் (def->hooks && def->hooks->create_container_len)
    {
      libcrun_error_t tmp_err = வெற்று;
      முழு in_userns = check_running_in_user_namespace (&tmp_err);
      எனில் (tmp_err)
        crun_error_release (&tmp_err);
      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, வெற்று, "created", "createContainer",
                      (hook **) def->hooks->create_container,
                      def->hooks->create_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      எனில் (UNLIKELY (ret != 0))
        திரும்பு ret;
    }

  ret = libcrun_finalize_mounts (entrypoint_args, container, rootfs, delayed_pidns_fork_cb, &delayed_pidns_fork, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (def->process)
    {
      ret = libcrun_set_selinux_label (container, def->process, false, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = libcrun_set_apparmor_profile (container, def->process, false, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  ret = mark_or_close_fds_ge_than (container, entrypoint_args->context->preserve_fds + 3, false, err);
  எனில் (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ret = libcrun_reopen_dev_null (err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = maybe_chown_std_streams (container->container_uid, container->container_gid, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = setup_environment (def, container->container_uid, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  /* Set primary process to 1 explicitly if nothing is configured and LISTEN_FD is not set.  */
  எனில் (entrypoint_args->context->listen_fds > 0 && getenv ("LISTEN_PID") == வெற்று)
    {
      libcrun_warning ("setting LISTEN_PID=1 since no previous configuration was found");
      எனில் (UNLIKELY (setenv ("LISTEN_PID", "1", 1) < 0))
        திரும்பு crun_make_error (err, errno, "setenv LISTEN_PID");
    }

  /* Attempt to chdir immediately here, before doing the setresuid.  If we fail here, let's
     try again later once the process switched to the user that runs in the container.  */
  எனில் (def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      எனில் (LIKELY (ret == 0))
        chdir_done = true;
      இல்லையெனில்
        crun_error_release (err);
    }

  ret = setsid ();
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "setsid");

  ret = setup_terminal (entrypoint_args, container, console_socket, console_socketpair, has_terminal, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  close_and_reset (&console_socket);
  close_and_reset (&console_socketpair);

  ret = setup_executable_path (entrypoint_args, def, exec_path, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_set_hostname (container, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_set_domainname (container, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (def->process && def->process->user)
    umask (def->process->user->umask_present ? def->process->user->umask : 0022);

  ret = apply_security_settings (entrypoint_args, def, container, *own_pid, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (UNLIKELY (def->process && def->process->args && *exec_path == வெற்று))
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      எனில் (UNLIKELY (ret < 0))
        {
          எனில் (entrypoint_args->custom_handler == வெற்று || is_empty_string (def->process->args[0]))
            திரும்பு ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          *exec_path = xstrdup (def->process->args[0]);
        }
    }

  /* The chdir was not already performed, so try again now after switching to the UID/GID in the container.  */
  எனில் (! chdir_done && def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (notify_socket && putenv (notify_socket) < 0)
    திரும்பு crun_make_error (err, errno, "putenv `%s`", notify_socket);

  திரும்பு 0;
}

நிலையான முழு
open_hooks_output (libcrun_container_t *container, முழு *out_fd, முழு *err_fd, libcrun_error_t *err)
{
  மாறிலி எழுத்து *annotation;

  *err_fd = *out_fd = -1;

  libcrun_debug ("Opening hooks output");
  annotation = find_annotation (container, "run.oci.hooks.stdout");
  எனில் (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stdout` annotation");
      *out_fd = TEMP_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      எனில் (UNLIKELY (*out_fd < 0))
        திரும்பு crun_make_error (err, errno, "open `%s`", annotation);
    }

  annotation = find_annotation (container, "run.oci.hooks.stderr");
  எனில் (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stderr` annotation");
      *err_fd = TEMP_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      எனில் (UNLIKELY (*err_fd < 0))
        திரும்பு crun_make_error (err, errno, "open `%s`", annotation);
    }

  திரும்பு 0;
}

/* The environment the process was started with lives on the initial stack, right after the
   argv strings and before the string AT_EXECFN points to.  clearenv(3) only drops the pointers
   to it, so when there is no execv to replace the stack, erase it or the container process can
   still read it, e.g. through /proc/self/environ.  */
நிலையான வெறுமை
erase_initial_environment (எழுத்து **argv, முழு argc)
{
  எழுத்து *start = argv[argc - 1] + strlen (argv[argc - 1]) + 1;
  எழுத்து *end = (எழுத்து *) getauxval (AT_EXECFN);

  எனில் (end > start)
    memset (start, 0, end - start);
}

நிலையான வெறுமை
rewrite_argv (எழுத்து **argv, முழு argc, மாறிலி எழுத்து *name, எழுத்து **args, size_t args_len)
{
  cleanup_free எழுத்து *decorated_name = வெற்று;
  size_t available_len = argv[argc - 1] - argv[0] + strlen (argv[argc - 1]) + 1;
  cleanup_free எழுத்து *new_argv = வெற்று;
  size_t i, so_far = 0, needed = 0;

  needed = xasprintf (&decorated_name, "[libcrun:%s]", name) + 1;
  க்கு (i = 0; i < args_len; i++)
    needed += strlen (args[i]) + 1;

  new_argv = xmalloc0 (needed + 1);
  strcpy (new_argv, decorated_name);
  so_far = strlen (decorated_name) + 1;

  எனில் (available_len >= needed)
    {
      க்கு (i = 0; i < args_len; i++)
        {
          strcpy (new_argv + so_far, args[i]);
          so_far += strlen (args[i]) + 1;
        }
    }

  எனில் (so_far >= available_len)
    so_far = available_len - 1;

  memset (argv[0], 0, available_len);
  memcpy (argv[0], new_argv, so_far);
}

/* Entrypoint to the container.  */
நிலையான முழு
container_init (வெறுமை *args, எழுத்து *notify_socket, முழு sync_socket, libcrun_error_t *err)
{
  கட்டமைப்பு container_entrypoint_s *entrypoint_args = args;
  முழு ret;
  runtime_spec_schema_config_schema *def = entrypoint_args->container->container_def;
  cleanup_free எழுத்து *exec_path = வெற்று;
  __attribute__ ((unused)) cleanup_free எழுத்து *notify_socket_cleanup = notify_socket;
  pid_t own_pid = 0;

  entrypoint_args->sync_socket = sync_socket;

  crun_set_output_handler (log_write_to_sync_socket, args);

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (பட (sync_socket, &own_pid, அளவு (own_pid)));
  எனில் (UNLIKELY (ret != அளவு (own_pid)))
    {
      எனில் (ret >= 0)
        errno = 0;
      திரும்பு crun_make_error (err, errno, "read from sync socket");
    }

  ret = container_init_setup (args, &own_pid, notify_socket, sync_socket, &exec_path, err);
  எனில் (UNLIKELY (ret < 0))
    {
      /* If it fails to write the error using the sync socket, then fallback
         to stderr.  */
      எனில் (sync_socket_write_error (sync_socket, err) == 0)
        {
          crun_error_release (err);
          /* A return value of > 0 means to fail with an error without err being set so to avoid
             printing it twice.  */
          திரும்பு 1;
        }
      திரும்பு ret;
    }

  entrypoint_args->sync_socket = -1;

  ret = unblock_signals (err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  /* sync 4.  */
  ret = sync_socket_send_sync (sync_socket, false, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  close_and_reset (&sync_socket);

  எனில் (entrypoint_args->context->fifo_exec_wait_fd >= 0)
    {
      எழுத்து buffer[1];
      fd_set read_set;
      cleanup_close முழு fd = entrypoint_args->context->fifo_exec_wait_fd;
      entrypoint_args->context->fifo_exec_wait_fd = -1;

      FD_ZERO (&read_set);
      FD_SET (fd, &read_set);
      செய்
        {
          ret = select (fd + 1, &read_set, வெற்று, வெற்று, வெற்று);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "select");

          ret = TEMP_FAILURE_RETRY (பட (fd, buffer, அளவு (buffer)));
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "read from the exec fifo");
      } போது (ret == 0);

      close_and_reset (&entrypoint_args->context->fifo_exec_wait_fd);
    }

  crun_set_output_handler (log_write_to_stderr, வெற்று);

  எனில் (def->process && def->process->no_new_privileges)
    {
      எழுத்து **seccomp_flags = வெற்று;
      size_t seccomp_flags_len = 0;
      cleanup_free எழுத்து *seccomp_fd_payload = வெற்று;
      size_t seccomp_fd_payload_len = 0;

      எனில் (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      எனில் (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (entrypoint_args->container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags,
                                   seccomp_flags_len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  எனில் (UNLIKELY (def->process == வெற்று))
    திரும்பு crun_make_error (err, 0, "block `process` not found");

  எனில் (UNLIKELY (exec_path == வெற்று))
    திரும்பு crun_make_error (err, 0, "executable path not specified");

  எனில் (def->hooks && def->hooks->start_container_len)
    {
      libcrun_container_t *container = entrypoint_args->container;
      libcrun_error_t tmp_err = வெற்று;
      முழு in_userns = check_running_in_user_namespace (&tmp_err);
      எனில் (tmp_err)
        crun_error_release (&tmp_err);

      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, வெற்று, "created", "startContainer",
                      (hook **) def->hooks->start_container,
                      def->hooks->start_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      எனில் (UNLIKELY (ret != 0))
        திரும்பு ret;

      /* Seek stdout/stderr to the end.  If the hooks were using the same files,
         the container process overwrites what was previously written.  */
      (வெறுமை) lseek (1, 0, SEEK_END);
      (வெறுமை) lseek (2, 0, SEEK_END);
    }

  எனில் (entrypoint_args->custom_handler)
    {
      /* Files marked with O_CLOEXEC are closed at execv time, so make sure they are closed now.
         This is a best effort operation, because the seccomp filter is already in place and it could
         stop some syscalls used by mark_or_close_fds_ge_than.
      */
      எனில் (entrypoint_args->custom_handler->vtable->close_fds)
        {
          ret = entrypoint_args->custom_handler->vtable->close_fds (entrypoint_args->custom_handler->cookie,
                                                                    entrypoint_args->container,
                                                                    entrypoint_args->context->preserve_fds,
                                                                    err);
        }
      இல்லையெனில்
        {
          ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
        }
      எனில் (UNLIKELY (ret < 0))
        crun_error_release (err);

      prctl (PR_SET_NAME, entrypoint_args->custom_handler->vtable->name);

      எனில் (entrypoint_args->context->argv)
        {
          erase_initial_environment (entrypoint_args->context->argv, entrypoint_args->context->argc);
          rewrite_argv (entrypoint_args->context->argv, entrypoint_args->context->argc,
                        entrypoint_args->custom_handler->vtable->name, def->process->args,
                        def->process->args_len);

          /* It is a quite destructive operation as we might be referencing data from the old
             argv, so make sure the context is not reused.  */
          entrypoint_args->context->argv = வெற்று;
          entrypoint_args->context = வெற்று;
        }

      ret = libcrun_set_selinux_label (entrypoint_args->container, def->process, true, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = libcrun_set_apparmor_profile (entrypoint_args->container, def->process, true, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = entrypoint_args->custom_handler->vtable->run_func (entrypoint_args->custom_handler->cookie,
                                                               entrypoint_args->container,
                                                               exec_path,
                                                               def->process->args);
      எனில் (ret != 0)
        திரும்பு crun_make_error (err, 0, "exec container process failed with handler as `%s`", entrypoint_args->custom_handler->vtable->name);

      திரும்பு ret;
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation since the seccomp profile is already in place now and might block
     some of the syscalls needed by mark_or_close_fds_ge_than.  */
  ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
  எனில் (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, def->process->args));

  எனில் (errno == ENOENT)
    திரும்பு crun_make_error (err, errno, "exec container process (missing dynamic library?) `%s`", exec_path);

  திரும்பு crun_make_error (err, errno, "exec container process `%s`", exec_path);
}

நிலையான முழு
read_container_config_from_state (libcrun_container_t **container, மாறிலி எழுத்து *state_வேர், மாறிலி எழுத்து *id,
                                  libcrun_error_t *err)
{
  cleanup_free எழுத்து *config_file = வெற்று;
  cleanup_free எழுத்து *dir = வெற்று;
  முழு ret;

  *container = வெற்று;

  ret = libcrun_get_state_directory (&dir, state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = append_paths (&config_file, err, dir, "config.json", வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  *container = libcrun_container_load_from_file (config_file, err);
  எனில் (*container == வெற்று)
    திரும்பு -1;

  திரும்பு 0;
}

நிலையான முழு
run_poststop_hooks (arg_unused libcrun_context_t *context, libcrun_container_t *container,
                    runtime_spec_schema_config_schema *def,
                    libcrun_container_status_t *status, மாறிலி எழுத்து *state_வேர், மாறிலி எழுத்து *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container_cleanup = வெற்று;
  முழு ret;

  எனில் (def == வெற்று)
    {
      எனில் (container == வெற்று)
        {
          ret = read_container_config_from_state (&container_cleanup, state_வேர், id, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
          container = container_cleanup;
        }

      def = container->container_def;
    }

  எனில் (def->hooks && def->hooks->poststop_len)
    {
      cleanup_close முழு hooks_out_fd = -1;
      cleanup_close முழு hooks_err_fd = -1;

      எனில் (container == வெற்று)
        {
          ret = read_container_config_from_state (&container_cleanup, state_வேர், id, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
          container = container_cleanup;
        }

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = do_hooks (def, 0, id, true, status->bundle, status->bundle, "stopped", "poststop", (hook **) def->hooks->poststop,
                      def->hooks->poststop_len, hooks_out_fd, hooks_err_fd, false, err);
      எனில் (UNLIKELY (ret != 0))
        {
          எனில் (ret < 0)
            libcrun_error_report_and_release (err);
          இல்லையெனில்
            libcrun_error (0, "poststop hook failed with exit code: %d", ret);
        }
    }
  திரும்பு 0;
}

நிலையான bool
has_new_pid_namespace (runtime_spec_schema_config_schema *def)
{
  size_t i;

  எனில் (def->linux == வெற்று || def->linux->namespaces == வெற்று)
    திரும்பு false;

  க்கு (i = 0; i < def->linux->namespaces_len; i++)
    {
      எனில் (strcmp (def->linux->namespaces[i]->type, "pid") == 0 && def->linux->namespaces[i]->path == வெற்று)
        திரும்பு true;
    }
  திரும்பு false;
}

/* Delete the container state.  CONTAINER is the container the caller already has
   available, or NULL.  When it is not NULL, the poststop hooks are run even if the
   status file was never written, e.g. when the container creation failed.  */
நிலையான முழு
container_delete_internal (libcrun_context_t *context, libcrun_container_t *container,
                           runtime_spec_schema_config_schema *def, மாறிலி எழுத்து *id, bool force, bool killall,
                           libcrun_error_t *err)
{
  cleanup_cgroup_status கட்டமைப்பு libcrun_cgroup_status *cgroup_status = வெற்று;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_container libcrun_container_t *container_cleanup = வெற்று;
  மாறிலி எழுத்து *state_வேர் = context->state_வேர்;
  முழு ret;

  ret = libcrun_read_container_status (&status, state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    {
      எனில் (force && crun_error_get_errno (err) == ENOENT)
        {
          libcrun_error_t tmp_err = வெற்று;

          crun_error_release (err);

          எனில் (container)
            {
              libcrun_container_status_t partial_status = {
                .bundle = (எழுத்து *) context->bundle,
              };

              ret = run_poststop_hooks (context, container, def, &partial_status, state_வேர், id, err);
              எனில் (UNLIKELY (ret < 0))
                crun_error_write_warning_and_release (context->output_handler_arg, &err);
            }

          libcrun_container_delete_status (state_வேர், id, &tmp_err);
          crun_error_release (&tmp_err);
          திரும்பு 0;
        }
      crun_error_release (err);
      திரும்பு libcrun_container_delete_status (state_வேர், id, err);
    }

  cgroup_status = libcrun_cgroup_make_status (&status);

  எனில் (! force)
    {
      ret = libcrun_is_container_running (&status, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
      எனில் (ret == 1)
        {
          ret = libcrun_status_has_read_exec_fifo (state_வேர், id, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
          எனில் (ret == 0)
            திரும்பு crun_make_error (err, 0, "the container `%s` is not in `created` or `stopped` state", id);

          /* If the container is in "created" state, then do the equivalent of delete --force.  */
          killall = force = true;
        }
    }

  எனில் (def == வெற்று)
    {
      எனில் (container == வெற்று)
        {
          ret = read_container_config_from_state (&container_cleanup, state_வேர், id, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;

          container = container_cleanup;
        }

      def = container->container_def;
    }

  எனில் (killall && force)
    {
      /* If the container has a pid namespace, it is enough to kill the first
         process (pid=1 in the namespace).
      */
      எனில் (has_new_pid_namespace (def))
        {
          ret = libcrun_kill_linux (&status, SIGKILL, err);
          எனில் (UNLIKELY (ret < 0))
            {
              எனில் (crun_error_get_errno (err) != ESRCH)
                திரும்பு ret;

              crun_error_release (err);
            }
        }
      இல்லையெனில் எனில் (status.cgroup_path)
        {
          ret = libcrun_cgroup_killall (cgroup_status, SIGKILL, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }
    }

  எனில் (def->linux && def->linux->intel_rdt)
    {
      ret = libcrun_destroy_intelrdt (id, def, err);
      எனில் (UNLIKELY (ret < 0))
        libcrun_error_report_and_release (err);
    }

  எனில் (status.cgroup_path)
    {
      /* If the cgroup cannot be destroyed, there are still processes in it.
         Keep the container status, it is the only record of the cgroup, so
         that the delete can be retried.  */
      ret = libcrun_cgroup_destroy (cgroup_status, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  ret = run_poststop_hooks (context, container, def, &status, state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  திரும்பு libcrun_container_delete_status (state_வேர், id, err);
}

முழு
libcrun_container_delete (libcrun_context_t *context, மாறிலி எழுத்து *id, bool force, libcrun_error_t *err)
{
  திரும்பு container_delete_internal (context, வெற்று, வெற்று, id, force, true, err);
}

முழு
libcrun_container_kill (libcrun_context_t *context, மாறிலி எழுத்து *id, மாறிலி எழுத்து *சமிக்ஞை, libcrun_error_t *err)
{
  முழு sig, ret;
  மாறிலி எழுத்து *state_வேர் = context->state_வேர்;
  cleanup_container_status libcrun_container_status_t status = {};

  sig = str2sig (சமிக்ஞை);
  எனில் (UNLIKELY (sig < 0))
    திரும்பு crun_make_error (err, 0, "unknown signal `%s`", சமிக்ஞை);

  ret = libcrun_read_container_status (&status, state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு libcrun_kill_linux (&status, sig, err);
}

முழு
libcrun_container_killall (libcrun_context_t *context, மாறிலி எழுத்து *id, மாறிலி எழுத்து *சமிக்ஞை, libcrun_error_t *err)
{
  முழு sig, ret;
  மாறிலி எழுத்து *state_வேர் = context->state_வேர்;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_cgroup_status கட்டமைப்பு libcrun_cgroup_status *cgroup_status = வெற்று;

  sig = str2sig (சமிக்ஞை);
  எனில் (UNLIKELY (sig < 0))
    திரும்பு crun_make_error (err, 0, "unknown signal `%s`", சமிக்ஞை);

  ret = libcrun_read_container_status (&status, state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  cgroup_status = libcrun_cgroup_make_status (&status);

  ret = libcrun_cgroup_killall (cgroup_status, sig, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;
  திரும்பு 0;
}

நிலையான முழு
write_container_status (libcrun_container_t *container, libcrun_context_t *context,
                        pid_t pid, கட்டமைப்பு libcrun_cgroup_status *cgroup_status,
                        libcrun_error_t *err)
{
  cleanup_free எழுத்து *bundle_allocated = வெற்று;
  மாறிலி எழுத்து *bundle = get_bundle_path (context->bundle, &bundle_allocated);
  cleanup_free எழுத்து *owner = get_user_name (geteuid ());
  எழுத்து *external_descriptors = libcrun_get_external_descriptors (container);
  எழுத்து *rootfs = container->container_def->வேர் ? container->container_def->வேர்->path : "";
  எழுத்து created[35];

  libcrun_container_status_t status = {
    .pid = pid,
    .rootfs = rootfs,
    .bundle = (எழுத்து *) bundle,
    .created = created,
    .owner = owner,
    .systemd_cgroup = context->systemd_cgroup,
    .detached = context->detach,
    .no_new_keyring = context->no_new_keyring,
    .external_descriptors = external_descriptors,
    .cgroup_path = வெற்று,
    .scope = வெற்று,
  };

  get_current_timestamp (created, அளவு (created));

  எனில் (cgroup_status)
    {
      முழு ret;

      ret = libcrun_cgroup_get_status (cgroup_status, &status, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (external_descriptors == வெற்று)
    திரும்பு crun_make_error (err, 0, "invalid internal state.  No external descriptors found");
  திரும்பு libcrun_write_container_status (context->state_வேர், context->id, &status, err);
}

நிலையான முழு
reap_subprocesses (pid_t *main_process, முழு *main_process_exit, முழு *last_process, libcrun_error_t *err)
{
  *last_process = 0;
  போது (1)
    {
      முழு status;
      முழு r = waitpid_ignore_stopped (-1, &status, WNOHANG);
      எனில் (r < 0)
        {
          எனில் (errno == EINTR)
            தொடர்;
          எனில் (errno == ECHILD)
            {
              *last_process = 1;
              திரும்பு 0;
            }
          திரும்பு crun_make_error (err, errno, "waitpid");
        }
      எனில் (r == 0)
        நிறுத்து;
      எனில் (r != *main_process)
        தொடர்;

      *main_process_exit = get_process_exit_status (status);

      /* Once reaped, the pid can be reused by an unrelated process.  */
      *main_process = 0;
    }
  திரும்பு 0;
}

நிலையான முழு
send_sd_notify (மாறிலி எழுத்து *ready_str, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  முழு ret = sd_notify (0, ready_str);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, -ret, "sd_notify");

#  எனில் HAVE_SD_NOTIFY_BARRIER
  /* Hard-code a 30 seconds timeout.  Ignore errors.  */
  sd_notify_barrier (0, 30 * 1000000);
#  endif
#இல்லையெனில்
  (வெறுமை) ready_str;
  (வெறுமை) err;
#endif
  திரும்பு 0;
}

நிலையான முழு
handle_notify_socket (முழு notify_socketfd, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  முழு ret;
  எழுத்து buf[256];
  மாறிலி எழுத்து *ready_str = "READY=1";

  ret = recvfrom (notify_socketfd, buf, அளவு (buf) - 1, 0, வெற்று, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "recvfrom notify socket");

  buf[ret] = '\0';
  எனில் (strstr (buf, ready_str))
    {
      ret = send_sd_notify (ready_str, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      திரும்பு 1;
    }
  திரும்பு 0;
#இல்லையெனில்
  (வெறுமை) notify_socketfd;
  (வெறுமை) err;
  திரும்பு 1;
#endif
}

கட்டமைப்பு wait_for_process_args
{
  pid_t pid;
  libcrun_context_t *context;
  முழு terminal_fd;
  முழு notify_socket;
  முழு *container_ready_fd;
  முழு seccomp_notify_fd;
  மாறிலி எழுத்து *seccomp_notify_plugins;
};

நிலையான முழு
write_pid_file (மாறிலி எழுத்து *pid_file, pid_t pid, libcrun_error_t *err)
{
  எழுத்து buf[32];
  முழு buf_len = snprintf (buf, அளவு (buf), "%d", pid);
  எனில் (UNLIKELY (buf_len >= (முழு) அளவு (buf)))
    திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");
  திரும்பு write_file_at_with_flags (AT_FDCWD, O_CREAT | O_TRUNC, 0700, pid_file, buf, buf_len, err);
}

#define MAX_EVENTS 10

நிலையான bool
is_fd_pollable (முழு fd)
{
  கட்டமைப்பு epoll_event ev = { .events = EPOLLIN };
  cleanup_close முழு epollfd = epoll_create1 (EPOLL_CLOEXEC);

  எனில் (epollfd < 0)
    திரும்பு true;

  திரும்பு epoll_ctl (epollfd, EPOLL_CTL_ADD, fd, &ev) == 0 || errno != EPERM;
}

நிலையான முழு
wait_for_process (கட்டமைப்பு wait_for_process_args *args, libcrun_error_t *err)
{
  cleanup_channel_fd_pair கட்டமைப்பு channel_fd_pair *from_terminal = வெற்று;
  cleanup_channel_fd_pair கட்டமைப்பு channel_fd_pair *to_terminal = வெற்று;
  முழு ret, container_exit_code = 0, last_process;
  cleanup_close முழு terminal_fd_from = -1;
  cleanup_close முழு terminal_fd_to = -1;
  bool stdin_pollable = false, stdout_pollable = false;
  cleanup_close முழு epollfd = -1;
  cleanup_close முழு signalfd = -1;
  sigset_t mask;
  முழு in_fds[MAX_EVENTS];
  முழு in_fds_len = 0;
  முழு out_fds[MAX_EVENTS];
  முழு out_fds_len = 0;
  size_t i;

  cleanup_seccomp_notify_context கட்டமைப்பு seccomp_notify_context_s *seccomp_notify_ctx = வெற்று;

  container_exit_code = 0;

  எனில் (args == வெற்று || args->context == வெற்று)
    திரும்பு crun_make_error (err, 0, "internal error: context is empty");

  க்கு (i = 0; i < MAX_EVENTS; i++)
    {
      in_fds[i] = -1;
      out_fds[i] = -1;
    }

  எனில் (args->context->pid_file)
    {
      ret = write_pid_file (args->context->pid_file, args->pid, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  /* Also exit if there is nothing more to wait for.  */
  எனில் (args->context->detach && args->notify_socket < 0)
    திரும்பு 0;

  எனில் (args->container_ready_fd)
    {
      ret = 0;
      TEMP_FAILURE_RETRY (லிக (*args->container_ready_fd, &ret, அளவு (ret)));
      close_and_reset (args->container_ready_fd);
    }

  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "sigprocmask");

  signalfd = create_signalfd (&mask, err);
  எனில் (UNLIKELY (signalfd < 0))
    திரும்பு signalfd;

  ret = reap_subprocesses (&args->pid, &container_exit_code, &last_process, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (last_process)
    திரும்பு container_exit_code;

  எனில் (args->seccomp_notify_fd >= 0)
    {
      cleanup_free எழுத்து *state_வேர் = வெற்று;
      cleanup_free எழுத்து *oci_config_path = வெற்று;

      கட்டமைப்பு libcrun_load_seccomp_notify_conf_s conf;
      memset (&conf, 0, அளவு conf);

      ret = libcrun_get_state_directory (&state_வேர், args->context->state_வேர், args->context->id, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = append_paths (&oci_config_path, err, state_வேர், "config.json", வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      conf.runtime_வேர்_path = state_வேர்;
      conf.name = args->context->id;
      conf.bundle_path = args->context->bundle;
      conf.oci_config_path = oci_config_path;

      ret = set_blocking_fd (args->seccomp_notify_fd, false, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = libcrun_load_seccomp_notify_plugins (&seccomp_notify_ctx,
                                                 args->seccomp_notify_plugins,
                                                 &conf, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      in_fds[in_fds_len++] = args->seccomp_notify_fd;
    }

  எனில் (args->terminal_fd >= 0)
    {
      /* The terminal_fd is dup()ed so that it can be registered with
         epoll multiple times using different masks.  */
      terminal_fd_from = dup (args->terminal_fd);
      எனில் (UNLIKELY (terminal_fd_from < 0))
        திரும்பு crun_make_error (err, errno, "dup terminal fd");
      terminal_fd_to = dup (args->terminal_fd);
      எனில் (UNLIKELY (terminal_fd_to < 0))
        திரும்பு crun_make_error (err, errno, "dup terminal fd");

      முழு i, non_blocking_fds[] = { terminal_fd_from, terminal_fd_to, 0, 1, -1 };
      க்கு (i = 0; non_blocking_fds[i] >= 0; i++)
        {
          ret = set_blocking_fd (non_blocking_fds[i], false, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }

      /* stdin and stdout can be something epoll does not support, such as
         /dev/null or a regular file.  Such a stdin is not forwarded, and
         such a stdout is written to without waiting for it to be ready.  */
      stdin_pollable = is_fd_pollable (0);
      stdout_pollable = is_fd_pollable (1);

      from_terminal = channel_fd_pair_new (terminal_fd_from, 1, BUFSIZ);
      எனில் (stdin_pollable)
        to_terminal = channel_fd_pair_new (0, terminal_fd_to, BUFSIZ);
    }

  in_fds[in_fds_len++] = signalfd;
  எனில் (args->notify_socket >= 0)
    in_fds[in_fds_len++] = args->notify_socket;
  எனில் (args->terminal_fd >= 0)
    {
      எனில் (stdin_pollable)
        {
          in_fds[in_fds_len++] = 0;
          out_fds[out_fds_len++] = terminal_fd_to;
        }

      in_fds[in_fds_len++] = terminal_fd_from;
      எனில் (stdout_pollable)
        out_fds[out_fds_len++] = 1;
    }

  epollfd = epoll_helper (in_fds, வெற்று, out_fds, வெற்று, err);
  எனில் (UNLIKELY (epollfd < 0))
    திரும்பு epollfd;

  போது (1)
    {
      கட்டமைப்பு epoll_event events[MAX_EVENTS];
      கட்டமைப்பு signalfd_siginfo si;
      கட்டமைப்பு winsize ws;
      முழு i, nr_events;
      ssize_t res;

      nr_events = TEMP_FAILURE_RETRY (epoll_wait (epollfd, events, MAX_EVENTS, -1));
      எனில் (UNLIKELY (nr_events < 0))
        திரும்பு crun_make_error (err, errno, "epoll_wait");

      க்கு (i = 0; i < nr_events; i++)
        {
          எனில் (events[i].data.fd == 0 || events[i].data.fd == terminal_fd_to)
            {
              ret = channel_fd_pair_process (to_terminal, epollfd, err);
              எனில் (UNLIKELY (ret < 0))
                திரும்பு crun_error_wrap (err, "copy to terminal fd");
            }
          இல்லையெனில் எனில் (events[i].data.fd == 1 || events[i].data.fd == terminal_fd_from)
            {
              ret = channel_fd_pair_process (from_terminal, epollfd, err);
              எனில் (UNLIKELY (ret < 0))
                திரும்பு crun_error_wrap (err, "copy from terminal fd");
            }
          இல்லையெனில் எனில் (events[i].data.fd == args->seccomp_notify_fd)
            {
              ret = libcrun_seccomp_notify_plugins (seccomp_notify_ctx,
                                                    args->seccomp_notify_fd, err);
              எனில் (UNLIKELY (ret < 0))
                திரும்பு ret;
            }
          இல்லையெனில் எனில் (events[i].data.fd == args->notify_socket)
            {
              ret = handle_notify_socket (args->notify_socket, err);
              எனில் (UNLIKELY (ret < 0))
                திரும்பு ret;
              எனில் (ret && args->context->detach)
                திரும்பு 0;
            }
          இல்லையெனில் எனில் (events[i].data.fd == signalfd)
            {
              res = TEMP_FAILURE_RETRY (பட (signalfd, &si, அளவு (si)));
              எனில் (UNLIKELY (res < 0))
                திரும்பு crun_make_error (err, errno, "read from signalfd");
              எனில் (si.ssi_signo == SIGCHLD)
                {
                  ret = reap_subprocesses (&args->pid, &container_exit_code,
                                           &last_process, err);
                  எனில் (UNLIKELY (ret < 0))
                    திரும்பு ret;
                  எனில் (last_process)
                    திரும்பு container_exit_code;
                }
              இல்லையெனில் எனில் (si.ssi_signo == SIGWINCH)
                {
                  /* Ignore the signal if the terminal is not available.  */
                  எனில் (args->terminal_fd > 0)
                    {
                      ret = ioctl (0, TIOCGWINSZ, &ws);
                      எனில் (UNLIKELY (ret < 0))
                        திரும்பு crun_make_error (err, errno, "ioctl TIOCGWINSZ copy terminal size from stdin");

                      ret = ioctl (args->terminal_fd, TIOCSWINSZ, &ws);
                      எனில் (UNLIKELY (ret < 0))
                        திரும்பு crun_make_error (err, errno, "ioctl TIOCSWINSZ copy terminal size to pty");
                    }
                }
              இல்லையெனில் எனில் (args->pid > 0)
                {
                  /* Send any other signal to the child process, if it was not reaped yet.  */
                  ret = kill (args->pid, si.ssi_signo);
                }
            }
          இல்லையெனில்
            {
              திரும்பு crun_make_error (err, 0, "internal error: unknown fd from epoll_wait");
            }
        }
    }

  திரும்பு 0;
}

நிலையான வெறுமை
flush_fd_to_err (libcrun_context_t *context, முழு terminal_fd)
{
  எழுத்து buf[256];
  முழு flags;

  எனில் (terminal_fd < 0 || stderr == வெற்று)
    திரும்பு;

  flags = fcntl (terminal_fd, F_GETFL, 0);
  எனில் (flags == -1)
    திரும்பு;
  எனில் (fcntl (terminal_fd, F_SETFL, flags | O_NONBLOCK) < 0)
    திரும்பு;

  க்கு (;;)
    {
      முழு ret = TEMP_FAILURE_RETRY (பட (terminal_fd, buf, அளவு (buf) - 1));
      எனில் (ret <= 0)
        நிறுத்து;
      buf[ret] = '\0';
      எனில் (context->output_handler)
        context->output_handler (0, buf, LIBCRUN_VERBOSITY_ERROR, context->output_handler_arg);
    }
  (வெறுமை) fcntl (terminal_fd, F_SETFL, flags);
  fflush (stderr);
  fsync (1);
  fsync (2);
}

நிலையான முழு
cleanup_watch (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
               கட்டமைப்பு libcrun_cgroup_status *cgroup_status, pid_t init_pid, முழு sync_socket,
               முழு terminal_fd, libcrun_error_t *err)
{
  மாறிலி எழுத்து *oom_message = வெற்று;
  libcrun_error_t tmp_err = வெற்று;
  முழு ret;

  எனில் (init_pid)
    {
      /* Try to detect whether the cgroup has a OOM.  */
      எனில் (cgroup_status)
        {
          முழு has_oom;

          has_oom = libcrun_cgroup_has_oom (cgroup_status, &tmp_err);
          எனில் (has_oom > 0)
            oom_message = "OOM: the memory limit could be too low";
          இல்லையெனில் எனில் (has_oom < 0)
            {
              /* If the detection has failed for any reason, e.g. the cgroup was
                 already deleted by the time it was checked, just ignore the
                 failure.  */
              crun_error_release (&tmp_err);
            }
        }
      /* If the OOM wasn't detected, look into the static configuration.  */
      எனில் (oom_message == வெற்று && is_memory_limit_too_கீழ் (def))
        oom_message = "the memory limit could be too low";

      kill (init_pid, SIGKILL);
      waitpid_ignore_stopped (init_pid, வெற்று, 0);
    }

  ret = sync_socket_wait_sync (context, sync_socket, true, &tmp_err);
  எனில் (UNLIKELY (ret < 0))
    {
      crun_error_release (err);
      *err = tmp_err;
    }

  எனில் (terminal_fd >= 0)
    flush_fd_to_err (context, terminal_fd);

  எனில் (oom_message)
    திரும்பு crun_error_wrap (err, "%s", oom_message);

  திரும்பு -1;
}

/* Find the uid:gid that is mapped to root inside the container user namespace.  */
வெறுமை
get_வேர்_in_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid, uid_t *uid,
                        gid_t *gid)
{
  *uid = -1;
  *gid = -1;

  /* Not root in the namespace.  */
  எனில் (host_uid)
    திரும்பு;

  எனில் (def->linux && def->linux->uid_mappings)
    {
      size_t i;

      க்கு (i = 0; i < def->linux->uid_mappings_len; i++)
        எனில் (def->linux->uid_mappings[i]->container_id == 0)
          {
            *uid = def->linux->uid_mappings[i]->host_id;
            நிறுத்து;
          }
    }
  எனில் (def->linux && def->linux->gid_mappings)
    {
      size_t i;

      க்கு (i = 0; i < def->linux->gid_mappings_len; i++)
        எனில் (def->linux->gid_mappings[i]->container_id == 0)
          {
            *gid = def->linux->gid_mappings[i]->host_id;
            நிறுத்து;
          }
    }

  /* If the uid and the gid are not changed, do not attempt any chown.  */
  எனில் (*uid == host_uid && *gid == host_gid)
    *uid = *gid = -1;
}

நிலையான முழு
get_seccomp_receiver_fd (libcrun_container_t *container, முழு *fd, முழு *self_receiver_fd, மாறிலி எழுத்து **plugins,
                         libcrun_error_t *err)
{
  மாறிலி எழுத்து *tmp;
  runtime_spec_schema_config_schema *def = container->container_def;

  *fd = -1;
  *self_receiver_fd = -1;

  tmp = find_annotation (container, "run.oci.seccomp.plugins");
  எனில் (tmp)
    {
      முழு fds[2];
      முழு ret;

      ret = create_socket_pair (fds, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_error_wrap (err, "create socket pair");

      *fd = fds[0];
      *self_receiver_fd = fds[1];
      *plugins = tmp;
      திரும்பு 0;
    }

  எனில் (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    tmp = def->linux->seccomp->listener_path;
  இல்லையெனில்
    tmp = find_annotation (container, "run.oci.seccomp.receiver");
  எனில் (tmp == வெற்று)
    tmp = getenv ("RUN_OCI_SECCOMP_RECEIVER");
  எனில் (tmp)
    {
      எனில் (tmp[0] != '/')
        திரும்பு crun_make_error (err, 0, "the seccomp receiver `%s` is not an absolute path", tmp);

      *fd = open_unix_domain_client_socket (tmp, 0, err);
      எனில் (UNLIKELY (*fd < 0))
        திரும்பு crun_error_wrap (err, "open seccomp receiver");
    }

  திரும்பு 0;
}

நிலையான bool
has_seccomp_receiver (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;

  எனில் (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    திரும்பு true;

  எனில் (find_annotation (container, "run.oci.seccomp.receiver") != வெற்று || getenv ("RUN_OCI_SECCOMP_RECEIVER") != வெற்று)
    திரும்பு true;

  திரும்பு false;
}

நிலையான முழு
setup_container_hooks_output (libcrun_container_t *container, runtime_spec_schema_config_schema *def,
                              கட்டமைப்பு container_entrypoint_s *container_args, முழு *hooks_out_fd,
                              முழு *hooks_err_fd, libcrun_error_t *err)
{
  முழு ret;

  எனில் (def->hooks
      && (def->hooks->prestart_len || def->hooks->poststart_len || def->hooks->create_runtime_len
          || def->hooks->create_container_len || def->hooks->start_container_len))
    {
      ret = open_hooks_output (container, hooks_out_fd, hooks_err_fd, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
      container_args->hooks_out_fd = *hooks_out_fd;
      container_args->hooks_err_fd = *hooks_err_fd;
    }
  திரும்பு 0;
}

நிலையான முழு
setup_container_keyring (libcrun_container_t *container, libcrun_context_t *context, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  முழு ret;

  எனில் (! context->no_new_keyring)
    {
      மாறிலி எழுத்து *label = வெற்று;

      libcrun_debug ("Creating new keyring");

      எனில் (def->process)
        {
          label = def->process->selinux_label;
          எனில் (label)
            libcrun_debug ("Using SELinux process label: `%s`", label);
        }

      ret = libcrun_create_keyring (container, container->context->id, label, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  திரும்பு 0;
}

நிலையான முழு
setup_terminal_socket_pair (libcrun_container_t *container, libcrun_context_t *context,
                            கட்டமைப்பு container_entrypoint_s *container_args, முழு *socket_pair_0,
                            முழு *socket_pair_1, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  முழு ret;
  முழு detach = context->detach;

  எனில் (def->process && def->process->terminal && ! detach && context->console_socket == வெற்று)
    {
      libcrun_debug ("Creating terminal socket pair");
      container_args->has_terminal_socket_pair = 1;
      ret = create_socket_pair (container_args->terminal_socketpair, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_error_wrap (err, "create terminal socket");

      *socket_pair_0 = container_args->terminal_socketpair[0];
      *socket_pair_1 = container_args->terminal_socketpair[1];
    }
  திரும்பு 0;
}

நிலையான முழு
setup_seccomp (libcrun_container_t *container, மாறிலி எழுத்து *seccomp_bpf_data,
               கட்டமைப்பு libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, முழு *seccomp_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  முழு ret;

  எனில் (find_annotation (container, "run.oci.seccomp.plugins") != வெற்று && has_seccomp_receiver (container))
    {
      திரும்பு crun_make_error (err, 0, "seccomp plugins and seccomp receivers cannot be declared at the same time");
    }

  எனில் (def->linux && (def->linux->seccomp || seccomp_bpf_data))
    {
      unsigned முழு seccomp_gen_options = 0;
      மாறிலி எழுத்து *annotation;

      libcrun_debug ("Initializing seccomp");
      annotation = find_annotation (container, "run.oci.seccomp_fail_unknown_syscall");
      எனில் (annotation && strcmp (annotation, "0") != 0)
        seccomp_gen_options = LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL;

      எனில் (seccomp_bpf_data)
        seccomp_gen_options |= LIBCRUN_SECCOMP_SKIP_CACHE;

      libcrun_seccomp_gen_ctx_init (seccomp_gen_ctx, container, true, seccomp_gen_options);

      ret = libcrun_open_seccomp_bpf (seccomp_gen_ctx, seccomp_fd, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  திரும்பு 0;
}

நிலையான முழு
setup_console_socket (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
                      கட்டமைப்பு container_entrypoint_s *container_args, முழு *console_socket_fd,
                      libcrun_error_t *err)
{
  எனில் (context->console_socket && def->process && def->process->terminal)
    {
      *console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
      எனில் (UNLIKELY (*console_socket_fd < 0))
        திரும்பு crun_error_wrap (err, "open console socket");
      container_args->console_socket_fd = *console_socket_fd;
    }
  திரும்பு 0;
}

நிலையான முழு
setup_cgroup_manager (libcrun_context_t *context, libcrun_container_t *container,
                      கட்டமைப்பு libcrun_cgroup_args *cg, முழு *cgroup_dirfd,
                      கட்டமைப்பு libcrun_dirfd_s *cgroup_dirfd_s, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  முழு cgroup_manager;
  uid_t வேர்_uid = -1;
  gid_t வேர்_gid = -1;
  முழு ret;

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  எனில் (context->systemd_cgroup)
    {
      libcrun_debug ("Using systemd cgroup manager");
      cgroup_manager = CGROUP_MANAGER_SYSTEMD;
    }
  இல்லையெனில் எனில் (context->force_no_cgroup)
    {
      libcrun_debug ("Disabling cgroup manager");
      cgroup_manager = CGROUP_MANAGER_DISABLED;
    }
  இல்லையெனில்
    libcrun_debug ("Using cgroupfs cgroup manager");

  /* If we are root (either on the host or in a namespace), then chown the cgroup to root
     in the container user namespace.  */
  get_வேர்_in_the_userns (def, container->host_uid, container->host_gid, &வேர்_uid, &வேர்_gid);
  libcrun_debug ("Using container host UID `%d` and GID `%d`", container->host_uid, container->host_gid);

  memset (cg, 0, அளவு (*cg));

  cg->cgroup_path = def->linux ? def->linux->cgroups_path : "";
  cg->manager = cgroup_manager;
  cg->id = context->id;
  cg->resources = def->linux ? def->linux->resources : வெற்று;
  cg->annotations = container->annotations;
  cg->வேர்_uid = வேர்_uid;
  cg->வேர்_gid = வேர்_gid;
  cg->state_வேர் = context->state_வேர்;
  cg->container = container;

  ret = libcrun_cgroup_preenter (cg, cgroup_dirfd, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  cgroup_dirfd_s->dirfd = cgroup_dirfd;
  cgroup_dirfd_s->joined = false;

  திரும்பு 0;
}

நிலையான முழு
set_scheduler (pid_t pid, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  முழு ret;

  ret = libcrun_set_scheduler (pid, def->process, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_reset_cpu_affinity_mask (pid, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_set_io_priority (pid, def->process, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு 0;
}

நிலையான முழு
seccomp_generation (முழு seccomp_fd, மாறிலி எழுத்து *seccomp_bpf_data,
                    கட்டமைப்பு libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, libcrun_error_t *err)
{
  முழு ret;

  எனில் (seccomp_fd >= 0)
    {
      எனில் (seccomp_bpf_data != வெற்று)
        {
          ret = libcrun_copy_seccomp (seccomp_gen_ctx, seccomp_bpf_data, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }
      இல்லையெனில்
        {
          ret = libcrun_generate_seccomp (seccomp_gen_ctx, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }
    }
  திரும்பு 0;
}

நிலையான முழு
terminal_setup (runtime_spec_schema_config_schema *def, libcrun_context_t *context,
                முழு socket_pair_0, முழு *terminal_fd, வெறுமை **orig_terminal, libcrun_error_t *err)
{
  முழு ret;
  முழு detach = context->detach;

  எனில் (def->process && def->process->terminal && ! detach && context->console_socket == வெற்று)
    {
      libcrun_debug ("Receiving console socket fd");
      *terminal_fd = receive_fd_from_socket (socket_pair_0, err);
      எனில் (UNLIKELY (*terminal_fd < 0))
        திரும்பு -1;

      ret = libcrun_clear_onlcr (*terminal_fd, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = libcrun_set_raw (0, orig_terminal, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  திரும்பு 0;
}

நிலையான முழு
libcrun_container_run_internal (libcrun_container_t *container, libcrun_context_t *context,
                                முழு *container_ready_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  முழு ret;
  pid_t pid;
  முழு detach = context->detach;
  cleanup_cgroup_status கட்டமைப்பு libcrun_cgroup_status *cgroup_status = வெற்று;
  cleanup_close முழு terminal_fd = -1;
  cleanup_terminal வெறுமை *orig_terminal = வெற்று;
  cleanup_close முழு sync_socket = -1;
  cleanup_close முழு notify_socket = -1;
  cleanup_close முழு socket_pair_0 = -1;
  cleanup_close முழு socket_pair_1 = -1;
  cleanup_close முழு seccomp_fd = -1;
  cleanup_close முழு console_socket_fd = -1;
  cleanup_close முழு hooks_out_fd = -1;
  cleanup_close முழு hooks_err_fd = -1;
  cleanup_close முழு own_seccomp_receiver_fd = -1;
  cleanup_close முழு seccomp_notify_fd = -1;
  மாறிலி எழுத்து *seccomp_notify_plugins = வெற்று;
  cleanup_cgroup_preenter கட்டமைப்பு libcrun_cgroup_args cg = {};
  cleanup_free எழுத்து *bundle_allocated = வெற்று;
  கட்டமைப்பு container_entrypoint_s container_args = {
    .container = container,
    .context = context,
    .bundle = get_bundle_path (context->bundle, &bundle_allocated),
    .terminal_socketpair = { -1, -1 },
    .console_socket_fd = -1,
    .hooks_out_fd = -1,
    .hooks_err_fd = -1,
    .seccomp_receiver_fd = -1,
    .custom_handler = வெற்று,
  };
  cleanup_close முழு cgroup_dirfd = -1;
  கட்டமைப்பு libcrun_dirfd_s cgroup_dirfd_s;
  கட்டமைப்பு libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  மாறிலி எழுத்து *seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");
  முழு cgroup_mode;

  எனில் (! context->force_no_cgroup)
    {
      cgroup_mode = libcrun_get_cgroup_mode (err);
      எனில் (UNLIKELY (cgroup_mode < 0))
        திரும்பு cgroup_mode;

      எனில் (cgroup_mode != CGROUP_MODE_UNIFIED)
        libcrun_warning ("cgroup v1 is deprecated and will be removed in a future release.  Use cgroup v2");
    }

  ret = setup_container_hooks_output (container, def, &container_args, &hooks_out_fd, &hooks_err_fd, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  container->context = context;

  எனில் (! detach || context->notify_socket)
    {
      libcrun_debug ("Setting child subreaper");
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "prctl set child subreaper");
    }

  ret = setup_container_keyring (container, context, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = setup_terminal_socket_pair (container, context, &container_args, &socket_pair_0, &socket_pair_1, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = block_signals (err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  umask (0);

  ret = libcrun_set_mempolicy (def, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;
  container_args.seccomp_fd = seccomp_fd;

  எனில் (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &container_args.seccomp_receiver_fd, &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  ret = setup_console_socket (context, def, &container_args, &console_socket_fd, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = setup_cgroup_manager (context, container, &cg, &cgroup_dirfd, &cgroup_dirfd_s, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_configure_handler (container_args.context->handler_manager,
                                   container_args.context,
                                   container,
                                   &(container_args.custom_handler),
                                   err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (container_args.custom_handler && container_args.custom_handler->vtable->modify_oci_configuration)
    {
      libcrun_debug ("Using custom handler to modify OCI configuration");
      ret = container_args.custom_handler->vtable->modify_oci_configuration (container_args.custom_handler->cookie,
                                                                             container_args.context,
                                                                             container->container_def,
                                                                             err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  pid = libcrun_run_linux_container (container, container_init, &container_args, &sync_socket, &cgroup_dirfd_s, err);
  எனில் (UNLIKELY (pid < 0))
    திரும்பு pid;

  cg.pid = pid;
  cg.joined = cgroup_dirfd_s.joined;
  libcrun_debug ("Running container on PID: `%d`", pid);

  எனில் (context->fifo_exec_wait_fd < 0 && context->notify_socket)
    {
      libcrun_debug ("Using notify socket: `%s`", context->notify_socket);
      /* Do not open the notify socket here on "create".  "start" will take care of it.  */
      ret = get_notify_fd (context, container, &notify_socket, err);
      எனில் (UNLIKELY (ret < 0))
        செல் fail;
    }

  எனில் (container_args.terminal_socketpair[1] >= 0)
    close_and_reset (&socket_pair_1);

  ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
  எனில் (UNLIKELY (ret < 0))
    செல் fail;

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE, err);
  எனில் (UNLIKELY (ret < 0))
    செல் fail;

  ret = libcrun_move_network_devices (container, pid, err);
  எனில் (UNLIKELY (ret < 0))
    செல் fail;

  /* sync send own pid.  */
  ret = TEMP_FAILURE_RETRY (லிக (sync_socket, &pid, அளவு (pid)));
  எனில் (UNLIKELY (ret != அளவு (pid)))
    {
      எனில் (ret >= 0)
        errno = 0;
      crun_make_error (err, errno, "write to sync socket");
      செல் fail;
    }

  /* sync 1.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  எனில் (UNLIKELY (ret < 0))
    செல் fail;

  /* sync 2.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  எனில் (UNLIKELY (ret < 0))
    செல் fail;

  ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
  எனில் (UNLIKELY (ret < 0))
    செல் fail;

  /* If the container process is created later, the scheduler is set on it then.  */
  எனில் (! libcrun_container_delayed_pidns_fork (container))
    {
      ret = set_scheduler (pid, def, err);
      எனில் (UNLIKELY (ret < 0))
        செல் fail;
    }

  /* The container is waiting that we write back.  In this phase we can launch the
     prestart hooks.  */
  எனில் (def->hooks && def->hooks->prestart_len)
    {
      libcrun_debug ("Running `prestart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "prestart", (hook **) def->hooks->prestart,
                      def->hooks->prestart_len, hooks_out_fd, hooks_err_fd, false, err);
      எனில் (UNLIKELY (ret != 0))
        செல் fail;
    }
  எனில் (def->hooks && def->hooks->create_runtime_len)
    {
      libcrun_debug ("Running `create` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "createRuntime", (hook **) def->hooks->create_runtime,
                      def->hooks->create_runtime_len, hooks_out_fd, hooks_err_fd, false, err);
      எனில் (UNLIKELY (ret != 0))
        செல் fail;
    }

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  எனில் (UNLIKELY (ret < 0))
    செல் fail;
  close_and_reset (&seccomp_fd);

  /* sync 3.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  எனில் (UNLIKELY (ret < 0))
    செல் fail;

  /* The process joining an existing PID namespace is created once the container root is in place.  */
  எனில் (libcrun_container_delayed_pidns_fork (container))
    {
      pid_t new_pid = 0;

      ret = sync_socket_wait_sync (context, sync_socket, false, err);
      எனில் (UNLIKELY (ret < 0))
        செல் fail;

      ret = TEMP_FAILURE_RETRY (பட (sync_socket, &new_pid, அளவு (new_pid)));
      எனில் (UNLIKELY (ret != அளவு (new_pid)))
        {
          எனில் (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "read pid from sync socket");
          செல் fail;
        }

      /* Cleanup the first process.  */
      waitpid_ignore_stopped (pid, வெற்று, 0);

      pid = new_pid;

      ret = set_scheduler (pid, def, err);
      எனில் (UNLIKELY (ret < 0))
        செல் fail;

      ret = libcrun_handle_pidfd_receiver (pid, container, err);
      எனில் (UNLIKELY (ret < 0))
        செல் fail;

      /* sync send own pid.  */
      ret = TEMP_FAILURE_RETRY (லிக (sync_socket, &pid, அளவு (pid)));
      எனில் (UNLIKELY (ret != அளவு (pid)))
        {
          எனில் (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "write to sync socket");
          செல் fail;
        }
    }

  ret = terminal_setup (def, context, socket_pair_0, &terminal_fd, &orig_terminal, err);
  எனில் (UNLIKELY (ret < 0))
    செல் fail;

  close_and_reset (&socket_pair_0);

  /* sync 4.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  எனில் (UNLIKELY (ret < 0))
    செல் fail;

  ret = close_and_reset (&sync_socket);
  எனில் (UNLIKELY (ret < 0))
    {
      crun_make_error (err, errno, "close sync_socket failed");
      செல் fail;
    }

  libcrun_debug ("Writing container status");
  ret = write_container_status (container, context, pid, cgroup_status, err);
  எனில் (UNLIKELY (ret < 0))
    செல் fail;

  /* Run poststart hooks here only if the container is created using "run".  For create+start, the
     hooks will be executed as part of the start command.  */
  எனில் (context->fifo_exec_wait_fd < 0 && def->hooks && def->hooks->poststart_len)
    {
      libcrun_debug ("Running `poststart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      எனில் (UNLIKELY (ret != 0))
        செல் fail;
    }

  /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
  எனில் (own_seccomp_receiver_fd >= 0)
    {
      libcrun_debug ("Receiving seccomp fd");
      seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
      எனில் (UNLIKELY (seccomp_notify_fd < 0))
        செல் fail;

      ret = close_and_reset (&own_seccomp_receiver_fd);
      எனில் (UNLIKELY (ret < 0))
        {
          crun_make_error (err, errno, "close seccomp receiver fd failed");
          செல் fail;
        }
    }

  {
    கட்டமைப்பு wait_for_process_args args = {
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
  எனில் (! context->detach)
    {
      libcrun_error_t tmp_err = வெற்று;
      cleanup_watch (context, def, cgroup_status, 0, sync_socket, terminal_fd, &tmp_err);
      crun_error_release (&tmp_err);
    }

  திரும்பு ret;

fail:
  ret = cleanup_watch (context, def, cgroup_status, pid, sync_socket, terminal_fd, err);
  எனில் (cgroup_status)
    {
      libcrun_error_t tmp_err = வெற்று;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  திரும்பு ret;
}

நிலையான முழு
check_config_file (runtime_spec_schema_config_schema *def, libcrun_context_t *context, libcrun_error_t *err)
{
  எனில் (UNLIKELY (def->linux == வெற்று))
    திரும்பு crun_make_error (err, 0, "invalid config file, no `linux` block specified");

  எனில் (context->handler == வெற்று)
    {
      எனில் (UNLIKELY (def->வேர் == வெற்று))
        திரும்பு crun_make_error (err, 0, "invalid config file, no `root` block specified");
      எனில் (UNLIKELY (def->mounts == வெற்று))
        திரும்பு crun_make_error (err, 0, "invalid config file, no `mounts` block specified");
    }
  திரும்பு 0;
}

நிலையான முழு
libcrun_copy_config_file (மாறிலி எழுத்து *id, மாறிலி எழுத்து *state_வேர், libcrun_container_t *container, libcrun_error_t *err)
{
  முழு ret;
  cleanup_free எழுத்து *dest_path = வெற்று;
  cleanup_free எழுத்து *dir = வெற்று;
  cleanup_free எழுத்து *buffer = வெற்று;
  size_t len;

  ret = libcrun_get_state_directory (&dir, state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = append_paths (&dest_path, err, dir, "config.json", வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (container->config_file == வெற்று && container->config_file_content == வெற்று)
    திரும்பு crun_make_error (err, 0, "config file not specified");

  எனில் (container->config_file == வெற்று)
    {
      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, container->config_file_content, strlen (container->config_file_content), err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  இல்லையெனில்
    {
      libcrun_debug ("Reading config file: `%s`", container->config_file);
      ret = read_all_file (container->config_file, &buffer, &len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, buffer, len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  திரும்பு 0;
}

நிலையான வெறுமை
force_delete_container_status (libcrun_context_t *context, libcrun_container_t *container,
                               runtime_spec_schema_config_schema *def)
{
  libcrun_error_t tmp_err = வெற்று;
  container_delete_internal (context, container, def, context->id, true, false, &tmp_err);
  libcrun_error_report_and_release (&tmp_err);
}

/* Serialize ERR to FD as an int errno followed by a NUL-terminated message.
   Used by forked children to report a failure to the parent process.  */
நிலையான வெறுமை
write_error_to_pipe (முழு fd, libcrun_error_t *err)
{
  மாறிலி எழுத்து *செய்தி = (*err)->செய்தி;
  முழு errcode = crun_error_get_errno (err);

  TEMP_FAILURE_RETRY (லிக (fd, &errcode, அளவு (errcode)));
  TEMP_FAILURE_RETRY (லிக (fd, செய்தி, strlen (செய்தி) + 1));
}

முழு
libcrun_container_run (libcrun_context_t *context, libcrun_container_t *container, unsigned முழு options,
                       libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  முழு ret;
  முழு detach = context->detach;
  முழு container_ret_status[2];
  cleanup_close முழு pipefd0 = -1;
  cleanup_close முழு pipefd1 = -1;
  libcrun_error_t tmp_err = வெற்று;

  container->context = context;

  ret = validate_options (options, LIBCRUN_RUN_OPTIONS_PREFORK | LIBCRUN_RUN_OPTIONS_KEEP, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = check_config_file (def, context, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (def->process && def->process->terminal && detach && context->console_socket == வெற்று)
    திரும்பு crun_make_error (err, 0, "use --console-socket with --detach when a terminal is used");

  ret = libcrun_status_check_directories (context->state_வேர், context->id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (! detach && (options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      ret = libcrun_copy_config_file (context->id, context->state_வேர், container, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = libcrun_container_run_internal (container, context, வெற்று, err);
      எனில் (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
        force_delete_container_status (context, container, def);
      திரும்பு ret;
    }

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  ret = fork ();
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "fork");
  எனில் (ret)
    {
      முழு status;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, வெற்று, 0);

      ret = TEMP_FAILURE_RETRY (பட (pipefd0, &status, அளவு (status)));
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, errno, "read from sync pipe");

      எனில் (status < 0)
        {
          முழு errno_;
          எழுத்து buf[512];
          ret = TEMP_FAILURE_RETRY (பட (pipefd0, &errno_, அளவு (errno_)));
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "read from sync pipe");

          ret = TEMP_FAILURE_RETRY (பட (pipefd0, buf, அளவு (buf) - 1));
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "read from sync pipe");
          buf[ret] = '\0';

          திரும்பு crun_make_error (err, errno_, "%s", buf);
        }

      திரும்பு status;
    }

  close_and_reset (&pipefd0);

  /* forked process.  */
  ret = detach_process ();
  எனில் (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_வேர், container, &tmp_err);
  எனில் (UNLIKELY (ret < 0))
    செல் fail;

  ret = libcrun_container_run_internal (container, context, வெற்று, &tmp_err);
  TEMP_FAILURE_RETRY (லிக (pipefd1, &ret, அளவு (ret)));
  எனில் (UNLIKELY (ret < 0))
    செல் fail;

  exit (EXIT_SUCCESS);
fail:

  எனில் (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
    force_delete_container_status (context, container, def);
  எனில் (tmp_err)
    {
      write_error_to_pipe (pipefd1, &tmp_err);
      crun_error_release (&tmp_err);
    }

  exit (EXIT_FAILURE);
}

முழு
libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container, unsigned முழு options,
                          libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  முழு ret;
  முழு container_ready_pipe[2];
  cleanup_close முழு pipefd0 = -1;
  cleanup_close முழு pipefd1 = -1;
  cleanup_close முழு exec_fifo_fd = -1;
  context->detach = 1;

  libcrun_debug ("Creating container: `%s`", context->id);
  container->context = context;

  ret = validate_options (options, LIBCRUN_CREATE_OPTIONS_PREFORK, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = check_config_file (def, context, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (def->process && def->process->terminal && context->console_socket == வெற்று)
    திரும்பு crun_make_error (err, 0, "use --console-socket with create when a terminal is used");

  ret = libcrun_status_check_directories (context->state_வேர், context->id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  exec_fifo_fd = libcrun_status_create_exec_fifo (context->state_வேர், context->id, err);
  எனில் (UNLIKELY (exec_fifo_fd < 0))
    திரும்பு exec_fifo_fd;

  context->fifo_exec_wait_fd = exec_fifo_fd;
  exec_fifo_fd = -1;

  எனில் ((options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      libcrun_debug ("Running with prefork enabled");
      ret = libcrun_copy_config_file (context->id, context->state_வேர், container, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
      ret = libcrun_container_run_internal (container, context, வெற்று, err);
      எனில் (UNLIKELY (ret < 0))
        force_delete_container_status (context, container, def);
      திரும்பு ret;
    }

  ret = pipe2 (container_ready_pipe, O_CLOEXEC);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "pipe");
  pipefd0 = container_ready_pipe[0];
  pipefd1 = container_ready_pipe[1];

  ret = fork ();
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "fork");
  எனில் (ret)
    {
      முழு exit_code;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, வெற்று, 0);

      ret = TEMP_FAILURE_RETRY (பட (pipefd0, &exit_code, அளவு (exit_code)));
      எனில் (UNLIKELY (ret <= 0))
        திரும்பு crun_make_error (err, ret < 0 ? errno : 0, "waiting for container to be ready");

      எனில் (ret != அளவு (exit_code) || exit_code < 0)
        திரும்பு crun_make_error (err, 0, "internal error: read invalid exit code");

      எனில் (exit_code == 0)
        திரும்பு 0;

      libcrun_debug ("Exit code is `%d`, deleting container", exit_code);
      libcrun_error_t tmp_err = வெற்று;
      container_delete_internal (context, வெற்று, def, context->id, true, true, &tmp_err);
      libcrun_error_report_and_release (&tmp_err);
      திரும்பு crun_make_error (err, 0, "error creating container");
    }

  /* forked process.  */
  ret = detach_process ();
  எனில் (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_வேர், container, err);
  எனில் (UNLIKELY (ret < 0))
    {
      முழு errcode = crun_error_get_errno (err);
      crun_error_release (err);
      libcrun_fail_with_error (errcode, "copy config file");
    }

  ret = libcrun_container_run_internal (container, context, &pipefd1, err);
  எனில் (UNLIKELY (ret < 0))
    {
      force_delete_container_status (context, container, def);
      libcrun_error ((*err)->status, "%s", (*err)->செய்தி);
      crun_error_release (err);
      crun_set_output_handler (log_write_to_stderr, வெற்று);
    }

  எனில் (pipefd1 >= 0)
    TEMP_FAILURE_RETRY (லிக (pipefd1, &ret, அளவு (ret)));
  exit (ret ? EXIT_FAILURE : 0);
}

முழு
libcrun_container_start (libcrun_context_t *context, மாறிலி எழுத்து *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = வெற்று;
  மாறிலி எழுத்து *state_வேர் = context->state_வேர்;
  runtime_spec_schema_config_schema *def;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_close முழு fd = -1;
  முழு ret;

  ret = libcrun_read_container_status (&status, state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_is_container_running (&status, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (! ret)
    திரும்பு crun_make_error (err, 0, "container `%s` is not running", id);

  ret = read_container_config_from_state (&container, state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (context->notify_socket)
    {
      ret = get_notify_fd (context, container, &fd, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  ret = libcrun_status_write_exec_fifo (context->state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  def = container->container_def;

  எனில் (context->notify_socket)
    {
      எனில் (fd >= 0)
        {
          fd_set read_set;

          போது (1)
            {
              கட்டமைப்பு timeval timeout = {
                .tv_sec = 0,
                .tv_usec = 10000,
              };
              FD_ZERO (&read_set);
              FD_SET (fd, &read_set);

              ret = select (fd + 1, &read_set, வெற்று, வெற்று, &timeout);
              எனில் (UNLIKELY (ret < 0))
                திரும்பு crun_make_error (err, errno, "select failed");
              எனில் (ret)
                {
                  ret = handle_notify_socket (fd, err);
                  எனில் (UNLIKELY (ret < 0))
                    திரும்பு ret;
                  எனில் (ret)
                    நிறுத்து;
                }
              இல்லையெனில்
                {
                  ret = libcrun_is_container_running (&status, err);
                  எனில் (UNLIKELY (ret < 0))
                    திரும்பு ret;
                  எனில் (! ret)
                    திரும்பு 0;
                }
            }
        }
    }

  /* The container is considered running only after we got the notification from the
     notify_socket, if any.  */
  எனில் (def->hooks && def->hooks->poststart_len)
    {
      cleanup_close முழு hooks_out_fd = -1;
      cleanup_close முழு hooks_err_fd = -1;

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = do_hooks (def, status.pid, context->id, false, status.bundle, status.bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      எனில் (UNLIKELY (ret != 0))
        {
          libcrun_error_t tmp_err = வெற்று;
          container_delete_internal (context, container, def, id, true, true, &tmp_err);
          libcrun_error_report_and_release (&tmp_err);
          திரும்பு ret;
        }
    }

  திரும்பு 0;
}

முழு
libcrun_get_container_state_string (மாறிலி எழுத்து *id, libcrun_container_status_t *status, மாறிலி எழுத்து *state_வேர்,
                                    மாறிலி எழுத்து **container_status, முழு *running, libcrun_error_t *err)
{
  முழு ret, has_fifo = 0;
  bool paused = false;

  ret = libcrun_is_container_running (status, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;
  *running = ret;

  எனில் (*running)
    {
      ret = libcrun_status_has_read_exec_fifo (state_வேர், id, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
      has_fifo = ret;
    }

  எனில் (*running && ! has_fifo)
    {
      cleanup_cgroup_status கட்டமைப்பு libcrun_cgroup_status *cgroup_status = வெற்று;

      cgroup_status = libcrun_cgroup_make_status (status);

      ret = libcrun_cgroup_is_container_paused (cgroup_status, &paused, err);
      எனில் (UNLIKELY (ret < 0))
        {
          /*
            The cgroup might have been cleaned up by the time we try to read it, ignore both
            ENOENT and ENODEV:
            - ENOENT: if the open(CGROUP_PATH) fails because the cgroup was deleted.
            - ENODEV: if the cgroup is deleted between the open and reading the freeze status.
          */

          errno = crun_error_get_errno (err);
          எனில் (errno == ENOENT || errno == ENODEV)
            {
              crun_error_release (err);
              *container_status = "stopped";
              திரும்பு 0;
            }

          திரும்பு ret;
        }
    }

  எனில் (! *running)
    *container_status = "stopped";
  இல்லையெனில் எனில் (has_fifo)
    *container_status = "created";
  இல்லையெனில் எனில் (paused)
    *container_status = "paused";
  இல்லையெனில்
    *container_status = "running";

  திரும்பு 0;
}

முழு
libcrun_container_state_json (libcrun_context_t *context, மாறிலி எழுத்து *id, எழுத்து **out, libcrun_error_t *err)
{
  எழுத்து *buffer = வெற்று;
  size_t len = 0;
  FILE *f;
  முழு ret;

  f = open_memstream (&buffer, &len);
  எனில் (f == வெற்று)
    திரும்பு crun_make_error (err, errno, "open_memstream");

  ret = libcrun_container_state (context, id, f, err);
  fclose (f);
  எனில் (UNLIKELY (ret < 0))
    {
      விடுவி (buffer);
      திரும்பு ret;
    }

  *out = buffer;
  திரும்பு 0;
}

முழு
libcrun_container_state (libcrun_context_t *context, மாறிலி எழுத்து *id, FILE *out, libcrun_error_t *err)
{
  மாறிலி எழுத்து *மாறிலி OCI_CONFIG_VERSION = "1.0.0";
  libcrun_container_status_t status = {};
  மாறிலி எழுத்து *state_வேர் = context->state_வேர்;
  மாறிலி எழுத்து *container_status = வெற்று;
  json_gen_ctx *gen = வெற்று;
  மாறிலி எழுத்து *buf;
  முழு ret = 0;
  முழு r = json_gen_status_ok;
  முழு running;
  size_t len;

  ret = libcrun_read_container_status (&status, state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_get_container_state_string (id, &status, state_வேர், &container_status, &running, err);
  எனில் (UNLIKELY (ret < 0))
    செல் exit;

  ret = 0;
  எனில் (! json_gen_init (&gen, வெற்று))
    திரும்பு crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));
  GEN_சாவி (gen, "ociVersion");
  GEN_STR (gen, OCI_CONFIG_VERSION);

  GEN_சாவி (gen, "id");
  GEN_STR (gen, id);

  GEN_சாவி (gen, "pid");
  GEN_OR_FAIL (map_int (gen, running ? status.pid : 0));

  GEN_சாவி (gen, "status");
  GEN_STR (gen, container_status);

  GEN_சாவி (gen, "bundle");
  GEN_STR (gen, status.bundle);

  GEN_சாவி (gen, "rootfs");
  GEN_STR (gen, status.rootfs);

  GEN_சாவி (gen, "created");
  GEN_STR (gen, status.created);

  எனில் (status.scope)
    {
      GEN_சாவி (gen, "systemd-scope");
      GEN_STR (gen, status.scope);
    }

  எனில் (status.owner)
    {
      GEN_சாவி (gen, "owner");
      GEN_STR (gen, status.owner);
    }

  {
    cleanup_container libcrun_container_t *container = வெற்று;

    ret = read_container_config_from_state (&container, state_வேர், id, err);
    எனில் (UNLIKELY (ret < 0))
      செல் exit;

    எனில் (container->container_def->annotations && container->container_def->annotations->len)
      GEN_OR_FAIL (gen_annotations (gen, container->container_def->annotations));
  }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  fprintf (out, "%s\n", buf);

exit:
  எனில் (gen)
    json_gen_free (gen);
  libcrun_free_container_status (&status);
  திரும்பு ret;

gen_error:
  ret = json_gen_error_to_crun_error (r, err);
  செல் exit;
}

முழு
libcrun_container_exec (libcrun_context_t *context, மாறிலி எழுத்து *id, runtime_spec_schema_config_schema_process *process,
                        libcrun_error_t *err)
{
  கட்டமைப்பு libcrun_container_exec_options_s opts;
  memset (&opts, 0, அளவு (opts));

  opts.struct_size = அளவு (opts);
  opts.process = process;

  திரும்பு libcrun_container_exec_with_options (context, id, &opts, err);
}

முழு
libcrun_container_exec_process_file (libcrun_context_t *context, மாறிலி எழுத்து *id, மாறிலி எழுத்து *path, libcrun_error_t *err)
{
  கட்டமைப்பு libcrun_container_exec_options_s opts;
  memset (&opts, 0, அளவு (opts));

  opts.struct_size = அளவு (opts);
  opts.path = path;

  திரும்பு libcrun_container_exec_with_options (context, id, &opts, err);
}

நிலையான முழு
exec_from_process_json (libcrun_context_t *context, மாறிலி எழுத்து *id, மாறிலி எழுத்து *process_json, மாறிலி எழுத்து *cgroup,
                        bool merge_env, libcrun_error_t *err)
{
  கட்டமைப்பு parser_context parser_ctx = { .options = 0, .errfile = stderr };
  கட்டமைப்பு libcrun_container_exec_options_s opts;
  runtime_spec_schema_config_schema_process *process;
  parser_error parser_err = வெற்று;
  json_object *doc = வெற்று;
  முழு ret;

  ret = parse_json_file (&doc, process_json, &parser_ctx, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  process = make_runtime_spec_schema_config_schema_process (doc, &parser_ctx, &parser_err);
  json_object_put (doc);
  எனில் (process == வெற்று)
    {
      ret = crun_make_error (err, 0, "cannot parse process: `%s`", parser_err ? parser_err : "unknown error");
      விடுவி (parser_err);
      திரும்பு ret;
    }

  memset (&opts, 0, அளவு (opts));
  opts.struct_size = அளவு (opts);
  opts.process = process;
  opts.cgroup = cgroup;
  opts.merge_env = merge_env;

  ret = libcrun_container_exec_with_options (context, id, &opts, err);
  free_runtime_spec_schema_config_schema_process (process);
  திரும்பு ret;
}

முழு
libcrun_container_exec_json (libcrun_context_t *context, மாறிலி எழுத்து *id, மாறிலி எழுத்து *process_json, libcrun_error_t *err)
{
  திரும்பு exec_from_process_json (context, id, process_json, வெற்று, false, err);
}

முழு
libcrun_container_exec_with_options_json (libcrun_context_t *context, மாறிலி எழுத்து *id,
                                          மாறிலி கட்டமைப்பு libcrun_exec_options_s *opts, libcrun_error_t *err)
{
  எனில் (opts == வெற்று || opts->struct_size == 0)
    திரும்பு crun_make_error (err, EINVAL, "invalid exec options");

  திரும்பு exec_from_process_json (context, id, opts->process_json, opts->cgroup, opts->merge_env, err);
}

#define cleanup_process_schema __attribute__ ((cleanup (cleanup_process_schemap)))

நிலையான உள்வரி வெறுமை
cleanup_process_schemap (runtime_spec_schema_config_schema_process **p)
{
  runtime_spec_schema_config_schema_process *process = *p;
  எனில் (process)
    (வெறுமை) free_runtime_spec_schema_config_schema_process (process);
}

/* Build the seccomp receiver payload (when needed), apply the seccomp profile and
   close the seccomp file descriptors.  Used from exec_process_entrypoint both before
   and after the capabilities are set, depending on process->no_new_privileges.  */
நிலையான முழு
apply_seccomp_for_exec (libcrun_container_t *container, pid_t own_pid, முழு seccomp_fd,
                        முழு seccomp_receiver_fd, எழுத்து **seccomp_flags, size_t seccomp_flags_len,
                        libcrun_error_t *err)
{
  cleanup_free எழுத்து *seccomp_fd_payload = வெற்று;
  size_t seccomp_fd_payload_len = 0;
  முழு ret;

  எனில் (seccomp_receiver_fd >= 0)
    {
      ret = get_seccomp_receiver_fd_payload (container, "running", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  ret = libcrun_apply_seccomp (seccomp_fd, seccomp_receiver_fd, seccomp_fd_payload,
                               seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  close_and_reset (&seccomp_fd);
  close_and_reset (&seccomp_receiver_fd);
  திரும்பு 0;
}

நிலையான முழு
exec_process_entrypoint (libcrun_context_t *context,
                         libcrun_container_t *container,
                         runtime_spec_schema_config_schema_process *process,
                         முழு *pipefd1,
                         முழு seccomp_fd,
                         முழு seccomp_receiver_fd,
                         கட்டமைப்பு custom_handler_instance_s *custom_handler,
                         bool merge_env,
                         libcrun_error_t *err)
{
  runtime_spec_schema_config_schema_process_capabilities *capabilities = வெற்று;
  cleanup_free எழுத்து *exec_path = வெற்று;
  uid_t container_uid;
  gid_t container_gid;
  மாறிலி எழுத்து *cwd;
  bool chdir_done = false;
  size_t seccomp_flags_len = 0;
  எழுத்து **seccomp_flags = வெற்று;
  pid_t own_pid = 0;
  size_t i;
  முழு ret;

  container_uid = process->user ? process->user->uid : 0;
  container_gid = process->user ? process->user->gid : 0;

  TEMP_FAILURE_RETRY (பட (*pipefd1, &own_pid, அளவு (own_pid)));

  cwd = process->cwd ? process->cwd : "/";
  எனில் (LIKELY (libcrun_safe_chdir (cwd, err) == 0))
    chdir_done = true;
  இல்லையெனில்
    crun_error_release (err);

  ret = unblock_signals (err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = clearenv ();
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "clearenv");

  எனில் ((merge_env || ! process->env_len) && container->container_def->process && container->container_def->process->env_len)
    {
      எழுத்து *e;

      க்கு (i = 0; i < container->container_def->process->env_len; i++)
        {
          e = container->container_def->process->env[i];
          எனில் (putenv (e) < 0)
            திரும்பு crun_make_error (err, errno, "putenv `%s`", e);
        }
    }
  எனில் (process->env_len)
    {
      க்கு (i = 0; i < process->env_len; i++)
        எனில் (putenv (process->env[i]) < 0)
          திரும்பு crun_make_error (err, errno, "putenv `%s`", process->env[i]);
    }

  எனில் (is_empty_string (getenv ("HOME")))
    {
      ret = set_home_env (container_uid);
      எனில் (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          எனில் (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            திரும்பு crun_make_error (err, errno, "setenv HOME");
        }
    }

  ret = libcrun_set_selinux_label (container, process, false, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_set_apparmor_profile (container, process, false, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (container->container_def->linux && container->container_def->linux->seccomp)
    {
      seccomp_flags = container->container_def->linux->seccomp->flags;
      seccomp_flags_len = container->container_def->linux->seccomp->flags_len;
    }

  ret = find_executable (&exec_path, process->args[0], process->cwd, err);
  எனில் (UNLIKELY (ret < 0))
    {
      எனில் (crun_error_get_errno (err) == ENOENT)
        திரும்பு ret;

      /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
         once the process switched to the use that runs in the container.  This might be necessary
         when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
         is not honored.  */
      crun_error_release (err);
    }

  எனில் (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, false, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_reopen_dev_null (err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (! process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  ret = libcrun_container_setgroups (container, process, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = maybe_chown_std_streams (container_uid, container_gid, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (process->capabilities)
    capabilities = process->capabilities;
  இல்லையெனில் எனில் (container->container_def->process)
    capabilities = container->container_def->process->capabilities;

  ret = libcrun_set_caps (capabilities, container_uid, container_gid, process->no_new_privileges,
                          custom_handler != வெற்று, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (UNLIKELY (exec_path == வெற்று))
    {
      ret = find_executable (&exec_path, process->args[0], process->cwd, err);
      எனில் (UNLIKELY (ret < 0))
        {
          எனில் (custom_handler == வெற்று || is_empty_string (process->args[0]))
            திரும்பு ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          exec_path = xstrdup (process->args[0]);
        }
    }

  எனில் (! chdir_done)
    {
      ret = libcrun_safe_chdir (cwd, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (process->user)
    umask (process->user->umask_present ? process->user->umask : 0022);

  ret = 0;
  TEMP_FAILURE_RETRY (லிக (*pipefd1, &ret, அளவு (ret)));
  TEMP_FAILURE_RETRY (close (*pipefd1));
  *pipefd1 = -1;

  எனில் (custom_handler)
    {
      எனில் (custom_handler->vtable->exec_func == வெற்று)
        திரும்பு crun_make_error (err, 0, "the handler does not support exec");

      எனில் (context->argv)
        erase_initial_environment (context->argv, context->argc);

      ret = custom_handler->vtable->exec_func (custom_handler->cookie,
                                               container,
                                               exec_path,
                                               process->args);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, -ret, "exec container process failed with handler as `%s`", custom_handler->vtable->name);

      _safe_exit (EXIT_FAILURE);
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation, because the seccomp filter is already in place and it could
     stop some syscalls used by mark_or_close_fds_ge_than.
  */
  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, true, err);
  எனில் (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, process->args));
  libcrun_fail_with_error (errno, "exec `%s`", exec_path);
}

/* Close the file descriptors inherited from the caller, other than those
   to be passed to the container process.  */
முழு
libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err)
{
  திரும்பு mark_or_close_fds_ge_than (வெற்று, context->preserve_fds + 3, true, err);
}

முழு
libcrun_load_process_from_file (மாறிலி எழுத்து *path, runtime_spec_schema_config_schema_process **process,
                                libcrun_error_t *err)
{
  கட்டமைப்பு parser_context ctx = { 0, stderr };
  cleanup_free எழுத்து *content = வெற்று;
  parser_error parser_err = வெற்று;
  json_object *doc = வெற்று;
  size_t len;
  முழு ret;

  ret = read_all_file (path, &content, &len, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  *process = make_runtime_spec_schema_config_schema_process (doc, &ctx, &parser_err);
  எனில் (UNLIKELY (*process == வெற்று))
    {
      ret = crun_make_error (err, 0, "cannot parse process file: `%s`", parser_err);
      விடுவி (parser_err);
      எனில் (doc)
        json_object_put (doc);
      திரும்பு ret;
    }

  விடுவி (parser_err);
  எனில் (doc)
    json_object_put (doc);

  திரும்பு 0;
}

முழு
libcrun_container_exec_with_options (libcrun_context_t *context, மாறிலி எழுத்து *id,
                                     கட்டமைப்பு libcrun_container_exec_options_s *opts,
                                     libcrun_error_t *err)
{
  cleanup_custom_handler_instance கட்டமைப்பு custom_handler_instance_s *custom_handler = வெற்று;
  முழு container_status, ret;
  bool container_paused = false;
  pid_t pid;
  libcrun_container_status_t status = {};
  மாறிலி எழுத்து *state_வேர் = context->state_வேர்;
  cleanup_close முழு terminal_fd = -1;
  cleanup_close முழு seccomp_fd = -1;
  cleanup_terminal வெறுமை *orig_terminal = வெற்று;
  cleanup_container libcrun_container_t *container = வெற்று;
  முழு container_ret_status[2];
  cleanup_close முழு pipefd0 = -1;
  cleanup_close முழு pipefd1 = -1;
  cleanup_close முழு seccomp_receiver_fd = -1;
  cleanup_close முழு own_seccomp_receiver_fd = -1;
  cleanup_close முழு seccomp_notify_fd = -1;
  மாறிலி எழுத்து *seccomp_notify_plugins = வெற்று;
  __attribute__ ((unused)) cleanup_process_schema runtime_spec_schema_config_schema_process *process_cleanup = வெற்று;
  runtime_spec_schema_config_schema_process *process = opts->process;
  கட்டமைப்பு libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  முழு ret_from_child = 0;

  ret = libcrun_read_container_status (&status, state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_is_container_running (&status, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;
  container_status = ret;

  ret = read_container_config_from_state (&container, state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  container->context = context;

  எனில் (container_status == 0)
    திரும்பு crun_make_error (err, 0, "the container `%s` is not running", id);

  {
    cleanup_cgroup_status கட்டமைப்பு libcrun_cgroup_status *cgroup_status = வெற்று;

    cgroup_status = libcrun_cgroup_make_status (&status);

    ret = libcrun_cgroup_is_container_paused (cgroup_status, &container_paused, err);
    எனில் (UNLIKELY (ret < 0))
      திரும்பு ret;
  }

  எனில் (UNLIKELY (container_paused))
    திரும்பு crun_make_error (err, 0, "cannot exec in a paused container `%s`", id);

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = block_signals (err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  libcrun_seccomp_gen_ctx_init (&seccomp_gen_ctx, container, false, 0);

  ret = libcrun_open_seccomp_bpf (&seccomp_gen_ctx, &seccomp_fd, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &seccomp_receiver_fd,
                                     &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  எனில் (அளவு (*opts) != opts->struct_size)
    திரும்பு crun_make_error (err, EINVAL, "invalid libcrun_container_exec_options_s struct");

  எனில் (opts->path)
    {
      எனில் (process)
        திரும்பு crun_make_error (err, EINVAL, "cannot specify both exec file and options");

      ret = libcrun_load_process_from_file (opts->path, &process, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      process_cleanup = process;
    }

  /* This must be done before we enter a user namespace.  If the process does
     not specify any rlimits, use the ones from the container configuration.  */
  எனில் (process->rlimits_len == 0 && container->container_def->process)
    ret = libcrun_set_rlimits (container->container_def->process->rlimits,
                               container->container_def->process->rlimits_len, err);
  இல்லையெனில்
    ret = libcrun_set_rlimits (process->rlimits, process->rlimits_len, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_set_mempolicy (container->container_def, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  /* If the new process block doesn't specify a SELinux label, AppArmor profile, user,
     I/O priority or scheduler, then use the configuration from the original config file.  */
  எனில் (container->container_def->process)
    {
      எனில் (process->io_priority == வெற்று && container->container_def->process->io_priority)
        {
          process->io_priority = clone_runtime_spec_schema_config_schema_process_io_priority (container->container_def->process->io_priority);
          எனில் (process->io_priority == வெற்று)
            OOM ();
        }

      எனில் (process->scheduler == வெற்று && container->container_def->process->scheduler)
        {
          process->scheduler = clone_runtime_spec_schema_config_schema_process_scheduler (container->container_def->process->scheduler);
          எனில் (process->scheduler == வெற்று)
            OOM ();
        }

      எனில் (process->selinux_label == வெற்று && container->container_def->process->selinux_label)
        process->selinux_label = xstrdup (container->container_def->process->selinux_label);

      எனில் (process->apparmor_profile == வெற்று && container->container_def->process->apparmor_profile)
        process->apparmor_profile = xstrdup (container->container_def->process->apparmor_profile);

      எனில் (process->user == வெற்று && container->container_def->process->user)
        {
          process->user = clone_runtime_spec_schema_config_schema_process_user (container->container_def->process->user);
          எனில் (process->user == வெற்று)
            OOM ();
        }
    }

  ret = initialize_security (container, process, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  /* Do not leak the session keyring of the caller into the container.  The
     keyring created for the container cannot be joined, so use an anonymous one.  */
  எனில் (! context->no_new_keyring && ! status.no_new_keyring)
    {
      libcrun_debug ("Creating new keyring for the exec process");

      ret = libcrun_create_keyring (container, வெற்று, process->selinux_label, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "prctl unset dumpable");

  pid = libcrun_join_process (context, container, status.pid, &status, opts->cgroup, context->detach,
                              process, process->terminal ? &terminal_fd : வெற்று, err);
  எனில் (UNLIKELY (pid < 0))
    திரும்பு pid;

  /* Process to exec.  */
  எனில் (pid == 0)
    {
      TEMP_FAILURE_RETRY (close (pipefd0));
      pipefd0 = -1;

      exec_process_entrypoint (context, container, process, &pipefd1, seccomp_fd, seccomp_receiver_fd, custom_handler, opts->merge_env, err);
      /* It gets here only on errors.  */
      எனில் (*err)
        {
          எனில் (pipefd1 < 0)
            libcrun_fail_with_error ((*err)->status, "%s", (*err)->செய்தி);
          இல்லையெனில்
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

  TEMP_FAILURE_RETRY (லிக (pipefd0, &pid, அளவு (pid)));

  எனில் (seccomp_fd >= 0)
    close_and_reset (&seccomp_fd);

  எனில் (terminal_fd >= 0)
    {
      unsigned short rows = 0, cols = 0;

      எனில் (process->console_size)
        {
          cols = process->console_size->width;
          rows = process->console_size->height;
        }

      ret = libcrun_terminal_setup_size (terminal_fd, rows, cols, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      எனில் (context->console_socket)
        {
          முழு ret;
          cleanup_close முழு console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
          எனில் (UNLIKELY (console_socket_fd < 0))
            திரும்பு console_socket_fd;
          ret = send_fd_to_socket (console_socket_fd, terminal_fd, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
          close_and_reset (&terminal_fd);
        }
      இல்லையெனில்
        {
          ret = libcrun_clear_onlcr (terminal_fd, err);
          எனில் (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              திரும்பு ret;
            }

          ret = libcrun_set_raw (0, &orig_terminal, err);
          எனில் (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              திரும்பு ret;
            }
        }
    }

  ret = TEMP_FAILURE_RETRY (பட (pipefd0, &ret_from_child, அளவு (ret_from_child)));
  எனில் (ret != அளவு (ret_from_child))
    ret = crun_make_error (err, 0, "read pipe failed");
  இல்லையெனில் எனில் (ret_from_child != 0)
    {
      cleanup_free எழுத்து *செய்தி = வெற்று;
      size_t len = 0;

      ret = read_all_fd (pipefd0, "error stream", &செய்தி, &len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
      /* the string from read_all_fd is always NUL terminated.  */
      ret = crun_make_error (err, ret_from_child, "%s", செய்தி);
    }
  இல்லையெனில்
    {
      /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
      எனில் (own_seccomp_receiver_fd >= 0)
        {
          seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
          எனில் (UNLIKELY (seccomp_notify_fd < 0))
            திரும்பு seccomp_notify_fd;

          ret = close_and_reset (&own_seccomp_receiver_fd);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "close seccomp receiver fd failed");
        }

      {
        கட்டமைப்பு wait_for_process_args args = {
          .pid = pid,
          .context = context,
          .terminal_fd = terminal_fd,
          .notify_socket = -1,
          .container_ready_fd = வெற்று,
          .seccomp_notify_fd = seccomp_notify_fd,
          .seccomp_notify_plugins = seccomp_notify_plugins,
        };

        ret = wait_for_process (&args, err);
      }
    }

  flush_fd_to_err (context, terminal_fd);
  திரும்பு ret;
}

முழு
libcrun_container_update (libcrun_context_t *context, மாறிலி எழுத்து *id, மாறிலி எழுத்து *content, size_t len arg_unused,
                          libcrun_error_t *err)
{
  cleanup_custom_handler_instance கட்டமைப்பு custom_handler_instance_s *custom_handler = வெற்று;
  runtime_spec_schema_config_linux_resources *resources = வெற்று;
  cleanup_container libcrun_container_t *container = வெற்று;
  மாறிலி எழுத்து *state_வேர் = context->state_வேர்;
  கட்டமைப்பு parser_context ctx = { 0, stderr };
  libcrun_container_status_t status = {};
  parser_error parser_err = வெற்று;
  json_object *doc = வெற்று;
  முழு ret;

  ret = libcrun_read_container_status (&status, state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = read_container_config_from_state (&container, state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  resources = make_runtime_spec_schema_config_linux_resources (doc, &ctx, &parser_err);
  எனில் (UNLIKELY (resources == வெற்று))
    {
      ret = crun_make_error (err, 0, "cannot parse resources: %s", parser_err);
      செல் cleanup;
    }

  எனில் (custom_handler && custom_handler->vtable->modify_oci_configuration)
    {
      /* Adapt RESOURCES to be used from the modify_oci_configuration hook.  */
      cleanup_free runtime_spec_schema_config_linux *linux = xmalloc0 (அளவு (*linux));
      cleanup_free runtime_spec_schema_config_schema *def = xmalloc0 (அளவு (*def));

      def->linux = linux;
      linux->resources = resources;

      ret = custom_handler->vtable->modify_oci_configuration (custom_handler->cookie,
                                                              context,
                                                              def,
                                                              err);
      எனில் (UNLIKELY (ret < 0))
        செல் cleanup;
    }

  ret = libcrun_linux_container_update (&status, state_வேர், resources, err);

cleanup:
  எனில் (doc)
    json_object_put (doc);
  விடுவி (parser_err);
  எனில் (resources)
    free_runtime_spec_schema_config_linux_resources (resources);

  திரும்பு ret;
}

முழு
libcrun_container_update_from_file (libcrun_context_t *context, மாறிலி எழுத்து *id, மாறிலி எழுத்து *file, libcrun_error_t *err)
{
  cleanup_free எழுத்து *content = வெற்று;
  size_t len;
  முழு ret;

  ret = read_all_file (file, &content, &len, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு libcrun_container_update (context, id, content, len, err);
}

நிலையான முழு
compare_update_மதிப்புகள் (மாறிலி வெறுமை *a, மாறிலி வெறுமை *b)
{
  மாறிலி கட்டமைப்பு libcrun_update_மதிப்பு_s *aa = a;
  மாறிலி கட்டமைப்பு libcrun_update_மதிப்பு_s *bb = b;
  முழு ret;

  ret = strcmp (aa->section, bb->section);
  எனில் (ret)
    திரும்பு ret;
  திரும்பு strcmp (aa->name, bb->name);
}

முழு
libcrun_container_update_from_மதிப்புகள் (libcrun_context_t *context, மாறிலி எழுத்து *id,
                                      கட்டமைப்பு libcrun_update_மதிப்பு_s *மதிப்புகள், size_t len,
                                      libcrun_error_t *err)
{
  மாறிலி எழுத்து *current_section = வெற்று;
  மாறிலி எழுத்து *buf;
  json_gen_ctx *gen = வெற்று;
  size_t i, buf_len;
  முழு ret;

  எனில் (! json_gen_init (&gen, வெற்று))
    திரும்பு crun_make_error (err, 0, "json_gen_init failed");
  json_gen_map_open (gen);

  qsort (மதிப்புகள், len, அளவு (கட்டமைப்பு libcrun_update_மதிப்பு_s), compare_update_மதிப்புகள்);

  க்கு (i = 0; i < len; i++)
    {
      எனில் (current_section == வெற்று || strcmp (மதிப்புகள்[i].section, current_section))
        {
          எனில் (i > 0)
            json_gen_map_close (gen);

          current_section = மதிப்புகள்[i].section;
          json_gen_string (gen, current_section, strlen (current_section));
          json_gen_map_open (gen);
        }

      json_gen_string (gen, மதிப்புகள்[i].name, strlen (மதிப்புகள்[i].name));

      எனில் (மதிப்புகள்[i].numeric)
        json_gen_எண் (gen, மதிப்புகள்[i].மதிப்பு, strlen (மதிப்புகள்[i].மதிப்பு));
      இல்லையெனில்
        json_gen_string (gen, மதிப்புகள்[i].மதிப்பு, strlen (மதிப்புகள்[i].மதிப்பு));
    }
  எனில் (len)
    json_gen_map_close (gen);

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);

  ret = libcrun_container_update (context, id, buf, buf_len, err);

  json_gen_free (gen);

  திரும்பு ret;
}

நிலையான வெறுமை
populate_array_field (எழுத்து ***field, எழுத்து *array[], size_t இலக்கம்_elements)
{
  size_t i;

  *field = xmalloc0 ((இலக்கம்_elements + 1) * அளவு (எழுத்து *));
  க்கு (i = 0; i < இலக்கம்_elements; i++)
    (*field)[i] = xstrdup (array[i]);

  (*field)[i] = வெற்று;
}

#ifdef HAVE_CAP
நிலையான வெறுமை
populate_capabilities (கட்டமைப்பு features_info_s *info, எழுத்து ***capabilities, size_t *இலக்கம்_capabilities)
{
  size_t index = 0;
  cap_மதிப்பு_t i;
  எழுத்து *endptr;
  முழு j;

  *இலக்கம்_capabilities = 0;
  க்கு (i = 0;; i++)
    {
      எழுத்து *v = cap_to_name (i);
      எனில் (v == வெற்று)
        நிறுத்து;
      strtol (v, &endptr, 10);
      எனில் (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          நிறுத்து;
        }
      (*இலக்கம்_capabilities)++;
    }

  *capabilities = xmalloc0 ((*இலக்கம்_capabilities + 1) * அளவு (மாறிலி எழுத்து *));
  க்கு (i = 0; i < (cap_மதிப்பு_t) *இலக்கம்_capabilities; i++)
    {
      எழுத்து *v = cap_to_name (i);
      எனில் (v == வெற்று)
        நிறுத்து;
      strtol (v, &endptr, 10);
      எனில் (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          நிறுத்து;
        }

      // Convert capability name to uppercase
      க்கு (j = 0; v[j] != '\0'; j++)
        v[j] = toupper (v[j]);

      (*capabilities)[index] = v;
      index++;
    }

  (*capabilities)[index] = வெற்று; // Terminate the array with NULL
  populate_array_field (&(info->linux.capabilities), *capabilities, *இலக்கம்_capabilities);
}
#endif

நிலையான வெறுமை
retrieve_mount_options (கட்டமைப்பு features_info_s **info)
{
  cleanup_free மாறிலி கட்டமைப்பு propagation_flags_s *mount_options_list = வெற்று;
  size_t இலக்கம்_mount_options = 0;

  // Retrieve mount options from wordlist
  mount_options_list = get_mount_flags_from_wordlist ();

  // Calculate the number of mount options
  போது (mount_options_list[இலக்கம்_mount_options].name != வெற்று)
    இலக்கம்_mount_options++;

  // Allocate memory for mount options in info struct
  (*info)->mount_options = xmalloc0 ((இலக்கம்_mount_options + 1) * அளவு (எழுத்து *));

  // Copy mount options to info struct
  க்கு (size_t i = 0; i < இலக்கம்_mount_options; i++)
    (*info)->mount_options[i] = xstrdup (mount_options_list[i].name);
}

முழு
libcrun_container_get_features (libcrun_context_t *context, கட்டமைப்பு features_info_s **info, libcrun_error_t *err arg_unused)
{
  // Allocate memory for the features_info_s structure
  size_t இலக்கம்_namspaces = அளவு (namespaces) / அளவு (namespaces[0]);
  size_t இலக்கம்_operators = அளவு (operators) / அளவு (operators[0]);
  size_t இலக்கம்_actions = அளவு (actions) / அளவு (actions[0]);
  size_t இலக்கம்_hooks = அளவு (hooks) / அளவு (hooks[0]);
  size_t இலக்கம்_archs = அளவு (archs) / அளவு (archs[0]);
  size_t இலக்கம்_unsafe_annotations = அளவு (potentially_unsafe_annotations) / அளவு (potentially_unsafe_annotations[0]);
  cleanup_free எழுத்து **capabilities = வெற்று;
  size_t இலக்கம்_capabilities = 0;
  size_t இலக்கம்_mempolicy_modes = அளவு (mempolicy_modes) / அளவு (mempolicy_modes[0]);
  size_t இலக்கம்_mempolicy_flags = அளவு (mempolicy_flags) / அளவு (mempolicy_flags[0]);

  *info = xmalloc0 (அளவு (கட்டமைப்பு features_info_s));

  // Hardcoded feature information
  (*info)->oci_version_min = xstrdup ("1.0.0");
  (*info)->oci_version_max = xstrdup ("1.1.0+dev");

  // Populate hooks
  populate_array_field (&((*info)->hooks), hooks, இலக்கம்_hooks);

  // Populate mount_options
  retrieve_mount_options (info);

  // Populate namespaces
  populate_array_field (&((*info)->linux.namespaces), namespaces, இலக்கம்_namspaces);

#ifdef HAVE_CAP
  // Populate capabilities
  populate_capabilities (*info, &capabilities, &இலக்கம்_capabilities);
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
  populate_array_field (&((*info)->linux.seccomp.actions), actions, இலக்கம்_actions);
  // Populate operators
  populate_array_field (&((*info)->linux.seccomp.operators), operators, இலக்கம்_operators);
  // Populate archs
  populate_array_field (&((*info)->linux.seccomp.archs), archs, இலக்கம்_archs);
#இல்லையெனில்
  (*info)->linux.seccomp.enabled = false;
#endif

  // Put values for apparmor and selinux
  (*info)->linux.apparmor.enabled = true;
  (*info)->linux.selinux.enabled = true;

  (*info)->linux.intel_rdt.enabled = true;

  (*info)->linux.net_devices.enabled = true;

  populate_array_field (&((*info)->linux.memory_policy.mode), mempolicy_modes, இலக்கம்_mempolicy_modes);
  populate_array_field (&((*info)->linux.memory_policy.flags), mempolicy_flags, இலக்கம்_mempolicy_flags);

  // Put the values for mount extensions
  (*info)->linux.mount_ext.idmap.enabled = true;

  // Populate the values for annotations
#ifdef HAVE_SECCOMP
  {
    மாறிலி கட்டமைப்பு scmp_version *version = seccomp_version ();
    எழுத்து *version_string = வெற்று;

    xasprintf (&version_string, "%u.%u.%u", version->major, version->minor, version->micro);
    (*info)->annotations.io_github_seccomp_libseccomp_version = version_string;
  }
#endif

  எனில் (context->handler_manager && handler_by_name (context->handler_manager, "wasm"))
    (*info)->annotations.run_oci_crun_wasm = true;

#எனில் HAVE_CRIU
  (*info)->annotations.run_oci_crun_checkpoint_enabled = true;
#endif
  (*info)->annotations.run_oci_crun_commit = GIT_VERSION;
  (*info)->annotations.run_oci_crun_version = PACKAGE_VERSION;

  populate_array_field (&((*info)->potentially_unsafe_annotations), potentially_unsafe_annotations, இலக்கம்_unsafe_annotations);

  திரும்பு 0;
}

நிலையான வெறுமை
features_add_string (json_gen_ctx *json_gen, மாறிலி எழுத்து *சாவி, மாறிலி எழுத்து *மதிப்பு)
{
  json_gen_string (json_gen, சாவி, strlen (சாவி));
  json_gen_string (json_gen, மதிப்பு, strlen (மதிப்பு));
}

நிலையான வெறுமை
features_add_bool (json_gen_ctx *json_gen, மாறிலி எழுத்து *சாவி, முழு மதிப்பு)
{
  json_gen_string (json_gen, சாவி, strlen (சாவி));
  json_gen_bool (json_gen, மதிப்பு);
}

நிலையான வெறுமை
features_add_bool_str (json_gen_ctx *json_gen, மாறிலி எழுத்து *சாவி, முழு மதிப்பு)
{
  features_add_string (json_gen, சாவி, மதிப்பு ? "true" : "false");
}

நிலையான வெறுமை
features_add_array (json_gen_ctx *json_gen, மாறிலி எழுத்து *சாவி, எழுத்து **array)
{
  size_t i;

  json_gen_string (json_gen, சாவி, strlen (சாவி));
  json_gen_array_open (json_gen);
  எனில் (array)
    க்கு (i = 0; array[i] != வெற்று; i++)
      json_gen_string (json_gen, array[i], strlen (array[i]));
  json_gen_array_close (json_gen);
}

நிலையான வெறுமை
features_add_linux_info (json_gen_ctx *json_gen, மாறிலி கட்டமைப்பு linux_info_s *linux)
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
  எனில் (linux->seccomp.actions)
    features_add_array (json_gen, "actions", linux->seccomp.actions);
  எனில் (linux->seccomp.operators)
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
  எனில் (linux->memory_policy.mode)
    features_add_array (json_gen, "modes", linux->memory_policy.mode);
  எனில் (linux->memory_policy.flags)
    features_add_array (json_gen, "flags", linux->memory_policy.flags);
  json_gen_map_close (json_gen);

  json_gen_map_close (json_gen);
}

நிலையான வெறுமை
features_add_annotations_info (json_gen_ctx *json_gen, மாறிலி கட்டமைப்பு annotations_info_s *annotation)
{
  json_gen_string (json_gen, "annotations", strlen ("annotations"));
  json_gen_map_open (json_gen);

  எனில் (! is_empty_string (annotation->io_github_seccomp_libseccomp_version))
    features_add_string (json_gen, "io.github.seccomp.libseccomp.version", annotation->io_github_seccomp_libseccomp_version);

  features_add_bool_str (json_gen, "org.opencontainers.runc.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);
  features_add_bool_str (json_gen, "run.oci.crun.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);

  features_add_string (json_gen, "run.oci.crun.commit", annotation->run_oci_crun_commit);
  features_add_string (json_gen, "run.oci.crun.version", annotation->run_oci_crun_version);

  features_add_bool_str (json_gen, "run.oci.crun.wasm", annotation->run_oci_crun_wasm);

  json_gen_map_close (json_gen);
}

முழு
libcrun_container_get_features_json (libcrun_context_t *context, எழுத்து **out, libcrun_error_t *err)
{
  cleanup_struct_features கட்டமைப்பு features_info_s *info = வெற்று;
  மாறிலி எழுத்து *buffer = வெற்று;
  json_gen_ctx *json_gen;
  size_t buffer_len;
  முழு ret;

  *out = வெற்று;

  ret = libcrun_container_get_features (context, &info, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (! json_gen_init (&json_gen, வெற்று))
    திரும்பு crun_make_error (err, 0, "cannot initialize JSON generator");

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

  திரும்பு 0;
}

/* Read the container status and fail if the container is not running.  */
நிலையான முழு
read_status_require_running (libcrun_context_t *context, மாறிலி எழுத்து *id,
                             libcrun_container_status_t *status, libcrun_error_t *err)
{
  முழு ret;

  ret = libcrun_read_container_status (status, context->state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_is_container_running (status, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;
  எனில் (ret == 0)
    திரும்பு crun_make_error (err, 0, "the container `%s` is not running", id);

  திரும்பு 0;
}

முழு
libcrun_container_pause (libcrun_context_t *context, மாறிலி எழுத்து *id, libcrun_error_t *err)
{
  முழு ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு libcrun_container_pause_linux (&status, err);
}

முழு
libcrun_container_unpause (libcrun_context_t *context, மாறிலி எழுத்து *id, libcrun_error_t *err)
{
  முழு ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு libcrun_container_unpause_linux (&status, err);
}

/* Translate the public options struct into the internal representation used by
   the linux/CRIU layer.  Strings are borrowed (not copied).  */
நிலையான வெறுமை
cr_options_from_public (libcrun_checkpoint_restore_t *dst, கட்டமைப்பு libcrun_checkpoint_restore_options_s *src)
{
  memset (dst, 0, அளவு (*dst));
  dst->image_path = (எழுத்து *) src->image_path;
  dst->work_path = (எழுத்து *) src->work_path;
  dst->parent_path = (எழுத்து *) src->parent_path;
  dst->console_socket = src->console_socket;
  dst->lsm_profile = (எழுத்து *) src->lsm_profile;
  dst->lsm_mount_context = (எழுத்து *) src->lsm_mount_context;
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

முழு
libcrun_container_checkpoint (libcrun_context_t *context, மாறிலி எழுத்து *id,
                              கட்டமைப்பு libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  முழு ret;
  மாறிலி எழுத்து *state_வேர் = context->state_வேர்;
  libcrun_container_status_t status = {};
  libcrun_checkpoint_restore_t cr;
  cleanup_container libcrun_container_t *container = வெற்று;

  cr_options_from_public (&cr, cr_options);

  ret = read_status_require_running (context, id, &status, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = read_container_config_from_state (&container, state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  /* Needed to use the same paths (e.g. the shared empty directory used for
     masked paths) as the container runtime and restore do.  */
  container->context = context;

  ret = libcrun_container_checkpoint_linux (&status, container, &cr, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (! (cr_options->leave_running || cr_options->pre_dump))
    திரும்பு container_delete_internal (context, வெற்று, வெற்று, id, true, true, err);

  திரும்பு 0;
}

/* The dummy process only keeps the cgroup populated until the container is restored there.
   Its cgroup is below the container cgroup, so once the container runs it can freeze or kill
   the dummy process: never depend on it after that point.  */
நிலையான வெறுமை
restore_proxy_process (முழு proxy_pid_pipe)
{
  எழுத்து c;

  /* Wait to be killed by the parent, or for the parent to go away.  */
  TEMP_FAILURE_RETRY (பட (proxy_pid_pipe, &c, அளவு (c)));
}

நிலையான முழு
restore_terminate_proxy_process (pid_t proxy_pid, மாறிலி எழுத்து *proxy_cgroup, pid_t container_pid, முழு cgroup_manager, libcrun_error_t *err)
{
  முழு mode;
  முழு ret;

  எனில் (cgroup_manager == CGROUP_MANAGER_SYSTEMD)
    {
      எழுத்து ready_str[64];

      ret = snprintf (ready_str, அளவு (ready_str), "MAINPID=%d", container_pid);
      எனில் (UNLIKELY (ret >= (முழு) அளவு (ready_str)))
        திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

      ret = send_sd_notify (ready_str, err);
      /* Do not fail on errors.  */
      எனில் (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  mode = libcrun_get_cgroup_mode (err);
  எனில் (UNLIKELY (mode < 0))
    திரும்பு mode;

  /* SIGKILL terminates a process frozen with cgroup v2, while the cgroup v1 freezer does not
     deliver it until the cgroup is thawed, so do not wait for it there.  */
  kill (proxy_pid, SIGKILL);
  TEMP_FAILURE_RETRY (waitpid (proxy_pid, வெற்று, mode == CGROUP_MODE_UNIFIED ? 0 : WNOHANG));

  திரும்பு destroy_cgroup_path (proxy_cgroup, mode, err);
}

முழு
libcrun_container_restore (libcrun_context_t *context, மாறிலி எழுத்து *id arg_unused,
                           கட்டமைப்பு libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  cleanup_cgroup_status கட்டமைப்பு libcrun_cgroup_status *cgroup_status = வெற்று;
  cleanup_container libcrun_container_t *container = வெற்று;
  cleanup_close முழு proxy_pid_pipe0 = -1;
  cleanup_close முழு proxy_pid_pipe1 = -1;
  runtime_spec_schema_config_schema *def;
  libcrun_container_status_t status = {};
  கட்டமைப்பு libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  libcrun_checkpoint_restore_t cr;
  cleanup_pid pid_t proxy_pid = -1;
  cleanup_close முழு seccomp_fd = -1;
  மாறிலி எழுத்து *seccomp_bpf_data;
  முழு proxy_pid_pipe[2];
  முழு cgroup_manager;
  uid_t வேர்_uid = -1;
  gid_t வேர்_gid = -1;
  முழு ret;

  cr_options_from_public (&cr, cr_options);

  container = libcrun_container_load_from_file ("config.json", err);
  எனில் (container == வெற்று)
    திரும்பு -1;

  container->context = context;
  def = container->container_def;

  ret = check_config_file (def, context, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_status_check_directories (context->state_வேர், context->id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_copy_config_file (context->id, context->state_வேர், container, err);
  எனில் (UNLIKELY (ret < 0))
    செல் fail;

  /* CRIU restores the seccomp filters of the checkpointed processes, but
     exec loads the filter from the seccomp.bpf file in the state directory,
     so it must be generated again for the restored container.  */
  seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  எனில் (UNLIKELY (ret < 0))
    செல் fail;

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  எனில் (UNLIKELY (ret < 0))
    செல் fail;
  close_and_reset (&seccomp_fd);

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  எனில் (context->systemd_cgroup)
    cgroup_manager = CGROUP_MANAGER_SYSTEMD;
  இல்லையெனில் எனில் (context->force_no_cgroup)
    cgroup_manager = CGROUP_MANAGER_DISABLED;

  def = container->container_def;

  /* If we are root (either on the host or in a namespace),
   * then chown the cgroup to root in the container user namespace. */
  get_வேர்_in_the_userns (def, container->host_uid, container->host_gid, &வேர்_uid, &வேர்_gid);

  /* If the root in the container is different than the current root user, attempt to chown
     the std streams before entering the user namespace.  Otherwise we might lose access
     to the user (as it is not mapped in the user namespace) and cannot chown them.  */
  எனில் (வேர்_uid > 0 || வேர்_gid > 0)
    {
      ret = maybe_chown_std_streams (வேர்_uid, வேர்_gid, err);
      எனில் (UNLIKELY (ret < 0))
        செல் fail;
    }

  /* CRIU runs as a child process, so the restored processes would inherit the session keyring.  */
  ret = setup_container_keyring (container, context, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  {
    cleanup_close முழு cgroup_dirfd = -1;
    bool needs_proxy_process;
    cleanup_cgroup_preenter கட்டமைப்பு libcrun_cgroup_args cg = {
      .resources = def->linux ? def->linux->resources : வெற்று,
      .annotations = container->annotations,
      .cgroup_path = def->linux ? def->linux->cgroups_path : "",
      .manager = cgroup_manager,
      .வேர்_uid = வேர்_uid,
      .வேர்_gid = வேர்_gid,
      .id = context->id,
      .state_வேர் = context->state_வேர்,
      .container = container,
    };

    /* The CRIU restore code uses bundle, rootfs and cgroup_path of status.   The cgroup_path is set later.  */
    status.bundle = (எழுத்து *) context->bundle;
    status.rootfs = def->வேர்->path;

    ret = libcrun_cgroup_preenter (&cg, &cgroup_dirfd, err);
    எனில் (UNLIKELY (ret < 0))
      செல் fail;

    needs_proxy_process = cgroup_dirfd < 0;

    /* If the target cgroup was already created (with cgroupfs), we can restore the container directly there.  */
    எனில் (! needs_proxy_process)
      {
        cleanup_free எழுத்து *cgroup_path = வெற்று;

        ret = get_cgroup_dirfd_path (cgroup_dirfd, &cgroup_path, err);
        எனில் (UNLIKELY (ret < 0))
          செல் fail;

        /* Complete the configuration for the cgroup before the container is
           restored there: CRIU resumes the restored processes before it
           returns, so the device filter and the limits must already be in
           place.  There is no process to move, as CRIU is run in the cgroup,
           and the controllers were enabled by precreate_cgroup.  */
        cg.joined = true;

        ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
        எனில் (UNLIKELY (ret < 0))
          செல் fail;

        /* Restore the container directly in the desired cgroup.  */
        status.cgroup_path = cgroup_path;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        எனில் (UNLIKELY (ret < 0))
          செல் fail;

        /* Now that the container is restored, keep any signal for it, so that
           it is forwarded to the init by wait_for_process, rather than acted
           upon by crun.  This cannot be done earlier, as CRIU, which is run as
           a child process, would inherit the blocked signals.  */
        ret = block_signals (err);
        எனில் (UNLIKELY (ret < 0))
          செல் fail;

        /* Use the container first process PID to finalize the cgroup.  */
        cg.pid = status.pid;
      }
    இல்லையெனில்
      {
        /* When using systemd, we need a process first that must exist before a cgroup is created.
           Create a dummy process that sits and waits until the container is restored.  */
        ret = pipe2 (proxy_pid_pipe, O_CLOEXEC);
        எனில் (UNLIKELY (ret < 0))
          {
            ret = crun_make_error (err, errno, "pipe");
            செல் fail;
          }

        /* Only used for auto cleanup.  */
        proxy_pid_pipe0 = proxy_pid_pipe[0];
        proxy_pid_pipe1 = proxy_pid_pipe[1];

        proxy_pid = fork ();
        எனில் (UNLIKELY (proxy_pid < 0))
          {
            ret = crun_make_error (err, errno, "fork");
            செல் fail;
          }

        எனில் (proxy_pid == 0)
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
        எனில் (UNLIKELY (ret < 0))
          செல் fail;
      }

    ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
    எனில் (UNLIKELY (ret < 0))
      செல் fail;

    /* When using a dummy process, the container is restored here once the cgroup is configured.  */
    எனில் (needs_proxy_process)
      {
        cleanup_free எழுத்து *target_cgroup = வெற்று;
        cleanup_free எழுத்து *proxy_cgroup = வெற்று;

        ret = libcrun_get_cgroup_process (proxy_pid, &target_cgroup, false, err);
        எனில் (UNLIKELY (ret < 0))
          செல் fail;

        /* Move the dummy process to a sub-cgroup.  */
        ret = append_paths (&proxy_cgroup, err, target_cgroup, ".proxy-crun", வெற்று);
        எனில் (UNLIKELY (ret < 0))
          செல் fail;

        ret = libcrun_move_process_to_cgroup (proxy_pid, 0, proxy_cgroup, true, err);
        எனில் (UNLIKELY (ret < 0))
          செல் fail;

        /* Restore the container in the same cgroup where the dummy process was.  */
        status.cgroup_path = target_cgroup;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        எனில் (UNLIKELY (ret < 0))
          செல் fail;

        /* See the comment for the other libcrun_container_restore_linux call.  */
        ret = block_signals (err);
        எனில் (UNLIKELY (ret < 0))
          செல் fail;

        /* The container is running, so do not fail because of the dummy process.  */
        ret = restore_terminate_proxy_process (proxy_pid, proxy_cgroup, status.pid, cgroup_manager, err);
        எனில் (UNLIKELY (ret < 0))
          crun_error_release (err);
        proxy_pid = -1;

        close_and_reset (&proxy_pid_pipe1);
      }
  }

  context->detach = cr_options->detach;
  ret = write_container_status (container, context, status.pid, cgroup_status, err);
  எனில் (UNLIKELY (ret < 0))
    செல் fail;

  {
    /* Wait the same way run and exec do: apart from waiting for the container
       init, this writes the pid file, forwards any signal to the init, and
       reaps every child, not only the init.  */
    கட்டமைப்பு wait_for_process_args args = {
      .pid = status.pid,
      .context = context,
      .terminal_fd = -1,
      .notify_socket = -1,
      .container_ready_fd = வெற்று,
      .seccomp_notify_fd = -1,
      .seccomp_notify_plugins = வெற்று,
    };

    திரும்பு wait_for_process (&args, err);
  }

fail:
  /* Once restored, the container is running.  Without a status file nothing
     can reach it anymore, so do not leave it behind.  */
  எனில் (status.pid > 0)
    {
      kill (status.pid, SIGKILL);
      waitpid_ignore_stopped (status.pid, வெற்று, 0);
    }
  எனில் (cgroup_status)
    {
      libcrun_error_t tmp_err = வெற்று;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  {
    libcrun_error_t tmp_err = வெற்று;

    எனில் (libcrun_container_delete_status (context->state_வேர், context->id, &tmp_err) < 0)
      crun_error_release (&tmp_err);
  }
  திரும்பு ret;
}

முழு
libcrun_container_read_pids (libcrun_context_t *context, மாறிலி எழுத்து *id, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  cleanup_cgroup_status கட்டமைப்பு libcrun_cgroup_status *cgroup_status = வெற்று;
  cleanup_container_status libcrun_container_status_t status = {};
  முழு ret;

  ret = libcrun_read_container_status (&status, context->state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (status.cgroup_path == வெற்று || status.cgroup_path[0] == '\0')
    திரும்பு crun_make_error (err, 0, "the container is not using cgroups");

  cgroup_status = libcrun_cgroup_make_status (&status);

  திரும்பு libcrun_cgroup_read_pids (cgroup_status, recurse, pids, err);
}

முழு
libcrun_container_list_json (libcrun_context_t *context, எழுத்து **out, libcrun_error_t *err)
{
  libcrun_container_list_t *list = வெற்று, *it;
  மாறிலி எழுத்து *content = வெற்று;
  json_gen_ctx *gen = வெற்று;
  size_t len;
  முழு ret;

  *out = வெற்று;

  ret = libcrun_get_containers_list (&list, context->state_வேர், err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (! json_gen_init (&gen, வெற்று))
    {
      ret = crun_make_error (err, 0, "cannot allocate json generator");
      செல் exit;
    }

  json_gen_config (gen, json_gen_beautify, 1);
  json_gen_array_open (gen);

  க்கு (it = list; it; it = it->next)
    {
      libcrun_container_status_t status;
      முழு running = 0;
      முழு pid;
      மாறிலி எழுத்து *container_status = வெற்று;

      ret = libcrun_read_container_status (&status, context->state_வேர், it->name, err);
      எனில் (UNLIKELY (ret < 0))
        செல் exit;

      pid = status.pid;

      ret = libcrun_get_container_state_string (it->name, &status, context->state_வேர், &container_status,
                                                &running, err);
      எனில் (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          தொடர்;
        }

      எனில் (! running)
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
  எனில் (json_gen_get_buf (gen, &content, &len) != json_gen_status_ok)
    {
      ret = libcrun_make_error (err, 0, "cannot generate json list");
      செல் exit;
    }

  *out = xstrdup (content);
  ret = 0;

exit:
  எனில் (list)
    libcrun_free_containers_list (list);
  எனில் (gen)
    json_gen_free (gen);

  திரும்பு ret;
}

முழு
libcrun_write_json_containers_list (libcrun_context_t *context, FILE *out, libcrun_error_t *err)
{
  cleanup_free எழுத்து *content = வெற்று;
  size_t len;
  முழு ret;

  ret = libcrun_container_list_json (context, &content, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  len = strlen (content);
  எனில் (len && fwrite (content, 1, len, out) != len)
    திரும்பு crun_make_error (err, errno, "error writing to file");

  திரும்பு 0;
}

முழு
libcrun_container_update_intel_rdt (libcrun_context_t *context, மாறிலி எழுத்து *id, கட்டமைப்பு libcrun_intel_rdt_update *update, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = வெற்று;
  முழு ret;

  ret = read_container_config_from_state (&container, context->state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு libcrun_update_intel_rdt (id, container, update->l3_cache_schema, update->mem_bw_schema, update->schemata, err);
}

நிலையான முழு
libcrun_container_add_or_remove_mounts_from_file (libcrun_context_t *context, மாறிலி எழுத்து *id, மாறிலி எழுத்து *file, bool add, libcrun_error_t *err)
{
  cleanup_custom_handler_instance கட்டமைப்பு custom_handler_instance_s *custom_handler = வெற்று;
  cleanup_container libcrun_container_t *container = வெற்று;
  cleanup_free runtime_spec_schema_defs_mount **mounts = வெற்று;
  மாறிலி எழுத்து *state_வேர் = context->state_வேர்;
  cleanup_free parser_error parser_err = வெற்று;
  கட்டமைப்பு parser_context pctx = { 0, stderr };
  libcrun_container_status_t status = {};
  cleanup_free எழுத்து *content = வெற்று;
  json_object *doc = வெற்று;
  size_t n_mounts = 0, len, i;
  முழு ret = 1;

  ret = read_all_file (file, &content, &len, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_read_container_status (&status, state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = read_container_config_from_state (&container, state_வேர், id, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = parse_json_file (&doc, content, &pctx, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (! json_object_is_type (doc, json_type_array))
    {
      ret = crun_make_error (err, 0, "mounts must be an array");
      செல் cleanup;
    }
  இல்லையெனில்
    {
      n_mounts = json_object_array_length (doc);

      mounts = xmalloc0 ((n_mounts + 1) * அளவு (*mounts));
      க்கு (i = 0; i < n_mounts; i++)
        {
          mounts[i] = make_runtime_spec_schema_defs_mount (json_object_array_get_idx (doc, i), &pctx, &parser_err);
          எனில் (mounts[i] == வெற்று)
            {
              ret = crun_make_error (err, 0, "cannot parse mount: %s", parser_err);
              செல் cleanup;
            }
        }
    }

  எனில் (add)
    ret = libcrun_make_runtime_mounts (container, &status, mounts, n_mounts, err);
  இல்லையெனில்
    ret = libcrun_destroy_runtime_mounts (container, &status, mounts, n_mounts, err);

cleanup:
  எனில் (doc)
    json_object_put (doc);
  எனில் (mounts)
    க்கு (i = 0; i < n_mounts; i++)
      free_runtime_spec_schema_defs_mount (mounts[i]);
  திரும்பு ret;
}

முழு
libcrun_container_add_mounts_from_file (libcrun_context_t *context, மாறிலி எழுத்து *id, மாறிலி எழுத்து *file, libcrun_error_t *err)
{
  திரும்பு libcrun_container_add_or_remove_mounts_from_file (context, id, file, true, err);
}

முழு
libcrun_container_remove_mounts_from_file (libcrun_context_t *context, மாறிலி எழுத்து *id, மாறிலி எழுத்து *file, libcrun_error_t *err)
{
  திரும்பு libcrun_container_add_or_remove_mounts_from_file (context, id, file, false, err);
}
