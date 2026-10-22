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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <ocispec/runtime_spec_schema_config_schema.h>
#অন্তর্ভাব <stdbool.h>
#অন্তর্ভাব "container.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "json_gen_utils.h"
#অন্তর্ভাব "seccomp.h"
#অন্তর্ভাব "mempolicy.h"
#ifdef HAVE_SECCOMP
#  include <seccomp.h>
#endif
#অন্তর্ভাব "scheduler.h"
#অন্তর্ভাব "seccomp_notify.h"
#অন্তর্ভাব "custom-handler.h"
#অন্তর্ভাব <stdbool.h>
#অন্তর্ভাব <argp.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <sched.h>
#অন্তর্ভাব <sys/wait.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব "status.h"
#অন্তর্ভাব "mount_flags.h"
#অন্তর্ভাব "linux.h"
#অন্তর্ভাব "terminal.h"
#অন্তর্ভাব "io_priority.h"
#অন্তর্ভাব "cgroup.h"
#অন্তর্ভাব "cgroup-utils.h"
#অন্তর্ভাব <sys/prctl.h>
#অন্তর্ভাব <sys/auxv.h>
#অন্তর্ভাব <sys/signalfd.h>
#অন্তর্ভাব <sys/epoll.h>
#অন্তর্ভাব <sys/socket.h>
#ifdef HAVE_CAP
#  include <sys/capability.h>
#endif
#অন্তর্ভাব <sys/ioctl.h>
#অন্তর্ভাব <termios.h>
#অন্তর্ভাব <grp.h>
#অন্তর্ভাব <git-version.h>

#ifdef HAVE_SYSTEMD
#  include <systemd/sd-daemon.h>
#endif

#অন্তর্ভাব <json-c/json.h>
#অন্তর্ভাব <ocispec/json_common.h>

গণনা
{
  SYNC_SOCKET_SYNC_MESSAGE,
  SYNC_SOCKET_ERROR_MESSAGE,
  SYNC_SOCKET_WARNING_MESSAGE,
  SYNC_SOCKET_DEBUG_MESSAGE,
};

সংরচনা container_entrypoint_s
{
  libcrun_container_t *container;
  libcrun_context_t *context;
  পূর্ণাঙ্ক has_terminal_socket_pair;
  পূর্ণাঙ্ক terminal_socketpair[2];

  /* Used by log_write_to_sync_socket.  */
  পূর্ণাঙ্ক sync_socket;

  পূর্ণাঙ্ক seccomp_fd;
  পূর্ণাঙ্ক seccomp_receiver_fd;
  পূর্ণাঙ্ক console_socket_fd;

  পূর্ণাঙ্ক hooks_out_fd;
  পূর্ণাঙ্ক hooks_err_fd;

  /* Absolute bundle path, to be reported to hooks run in the container.  */
  ধ্রুব বর্ণ *bundle;

  সংরচনা custom_handler_instance_s *custom_handler;
};

সংরচনা sync_socket_message_s
{
  পূর্ণাঙ্ক type;
  পূর্ণাঙ্ক error_মান;
  বর্ণ message[512];
};

প্রকারনির্দেশ runtime_spec_schema_defs_hook hook;

// linux hooks
বর্ণ *hooks[] = {
  "prestart",
  "createRuntime",
  "createContainer",
  "startContainer",
  "poststart",
  "poststop"
};

// linux namespaces
স্থির বর্ণ *namespaces[] = {
  "cgroup",
  "ipc",
  "mount",
  "network",
  "pid",
  "user",
  "uts"
};

স্থির বর্ণ *actions[] = {
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

স্থির বর্ণ *operators[] = {
  "SCMP_CMP_NE",
  "SCMP_CMP_LT",
  "SCMP_CMP_LE",
  "SCMP_CMP_EQ",
  "SCMP_CMP_GE",
  "SCMP_CMP_GT",
  "SCMP_CMP_MASKED_EQ",
};

স্থির বর্ণ *archs[] = {
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

স্থির বর্ণ *mempolicy_modes[] = {
  "MPOL_DEFAULT",
  "MPOL_PREFERRED",
  "MPOL_BIND",
  "MPOL_INTERLEAVE",
  "MPOL_LOCAL",
  "MPOL_PREFERRED_MANY",
  "MPOL_WEIGHTED_INTERLEAVE"
};

স্থির বর্ণ *mempolicy_flags[] = {
  "MPOL_F_NUMA_BALANCING",
  "MPOL_F_RELATIVE_NODES",
  "MPOL_F_STATIC_NODES"
};

স্থির বর্ণ *potentially_unsafe_annotations[] = {
  "module.wasm.image/variant",
  "io.kubernetes.cri.container-type",
  "run.oci.",
  "org.criu.",
  "krun.",
};

#define SYNC_SOCKET_MESSAGE_LEN(x, l) (offsetof (সংরচনা sync_socket_message_s, message) + l)

স্থির পূর্ণাঙ্ক
sync_socket_write_বার্তা (পূর্ণাঙ্ক fd, পূর্ণাঙ্ক verbosity, পূর্ণাঙ্ক err_মান, ধ্রুব বর্ণ *log_বার্তা)
{
  পূর্ণাঙ্ক ret;
  size_t err_len;
  সংরচনা sync_socket_message_s বার্তা;
  বিকল্প (verbosity)
    {
    পক্ষ LIBCRUN_VERBOSITY_DEBUG:
      বার্তা.type = SYNC_SOCKET_DEBUG_MESSAGE;
      ভঙ্গ;
    পক্ষ LIBCRUN_VERBOSITY_WARNING:
      বার্তা.type = SYNC_SOCKET_WARNING_MESSAGE;
      ভঙ্গ;
    পক্ষ LIBCRUN_VERBOSITY_ERROR:
      বার্তা.type = SYNC_SOCKET_ERROR_MESSAGE;
      ভঙ্গ;
    }
  বার্তা.error_মান = err_মান;

  যদি (fd < 0)
    প্রত্যাগম 0;

  err_len = strlen (log_বার্তা);
  যদি (err_len >= পরিমাণ (বার্তা.message))
    err_len = পরিমাণ (বার্তা.message) - 1;

  memcpy (বার্তা.message, log_বার্তা, err_len);
  বার্তা.message[err_len] = '\0';

  ret = TEMP_FAILURE_RETRY (লিখ (fd, &বার্তা, SYNC_SOCKET_MESSAGE_LEN (বার্তা, err_len + 1)));
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম -1;

  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
sync_socket_write_error (পূর্ণাঙ্ক fd, libcrun_error_t *out_err)
{
  যদি (fd < 0)
    প্রত্যাগম 0;
  প্রত্যাগম sync_socket_write_বার্তা (fd, false, (*out_err)->status, (*out_err)->বার্তা);
}

স্থির শূন্য
log_write_to_sync_socket (পূর্ণাঙ্ক errno_, ধ্রুব বর্ণ *বার্তা, পূর্ণাঙ্ক verbosity, শূন্য *arg)
{
  সংরচনা container_entrypoint_s *entrypoint_args = arg;
  পূর্ণাঙ্ক fd = entrypoint_args->sync_socket;

  যদি (fd < 0)
    প্রত্যাগম;

  যদি (sync_socket_write_বার্তা (fd, verbosity, errno_, বার্তা) < 0)
    log_write_to_stderr (errno_, বার্তা, verbosity, arg);
}

স্থির bool
is_memory_limit_too_নিচের (runtime_spec_schema_config_schema *def)
{
  ধ্রুব long memory_limit_too_নিচের = 1024 * 1024;

  যদি (def->linux == রিক্ত || def->linux->resources == রিক্ত)
    প্রত্যাগম false;

  যদি (def->linux->resources->memory
      && def->linux->resources->memory->limit_present
      && def->linux->resources->memory->limit < memory_limit_too_নিচের)
    প্রত্যাগম true;

  যদি (def->linux->resources->unified)
    {
      size_t i;

      প্রতি (i = 0; i < def->linux->resources->unified->len; i++)
        যদি (strcmp (def->linux->resources->unified->চাবিগুলি[i], "memory.max") == 0)
          {
            long limit;

            errno = 0;
            limit = strtol (def->linux->resources->unified->মানসমূহ[i], রিক্ত, 10);
            যদি (errno != 0)
              প্রত্যাগম false;
            যদি (limit < memory_limit_too_নিচের)
              প্রত্যাগম true;
          }
    }

  প্রত্যাগম false;
}

স্থির পূর্ণাঙ্ক
sync_socket_wait_sync (libcrun_context_t *context, পূর্ণাঙ্ক fd, bool flush, libcrun_error_t *err)
{
  সংরচনা sync_socket_message_s বার্তা;

  যদি (fd < 0)
    প্রত্যাগম 0;

  যাবত্ (true)
    {
      পূর্ণাঙ্ক ret;

      ret = TEMP_FAILURE_RETRY (পঠ (fd, &বার্তা, পরিমাণ (বার্তা)));
      যদি (UNLIKELY (ret < 0))
        {
          যদি (flush)
            প্রত্যাগম 0;
          প্রত্যাগম crun_make_error (err, errno, "read from sync socket");
        }

      যদি (ret == 0)
        {
          যদি (flush)
            প্রত্যাগম 0;

          প্রত্যাগম crun_make_error (err, 0, "read from the init process");
        }

      যদি (! flush && বার্তা.type == SYNC_SOCKET_SYNC_MESSAGE)
        প্রত্যাগম 0;
      অন্যথা যদি (বার্তা.type == SYNC_SOCKET_DEBUG_MESSAGE)
        {
          যদি (context)
            context->output_handler (বার্তা.error_মান, বার্তা.message, LIBCRUN_VERBOSITY_DEBUG, context->output_handler_arg);
          অনুবর্ত;
        }
      অন্যথা যদি (বার্তা.type == SYNC_SOCKET_WARNING_MESSAGE)
        {
          যদি (context)
            context->output_handler (বার্তা.error_মান, বার্তা.message, LIBCRUN_VERBOSITY_WARNING, context->output_handler_arg);
          অনুবর্ত;
        }
      অন্যথা যদি (বার্তা.type == SYNC_SOCKET_ERROR_MESSAGE)
        প্রত্যাগম crun_make_error (err, বার্তা.error_মান, "%s", বার্তা.message);
    }
}

স্থির পূর্ণাঙ্ক
sync_socket_send_sync (পূর্ণাঙ্ক fd, bool flush_errors, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  সংরচনা sync_socket_message_s বার্তা = {
    0,
  };
  বার্তা.type = SYNC_SOCKET_SYNC_MESSAGE;

  যদি (fd < 0)
    প্রত্যাগম 0;

  ret = TEMP_FAILURE_RETRY (লিখ (fd, &বার্তা, SYNC_SOCKET_MESSAGE_LEN (বার্তা, 0)));
  যদি (UNLIKELY (ret < 0))
    {
      যদি (flush_errors)
        {
          পূর্ণাঙ্ক saved_errno = errno;
          ret = TEMP_FAILURE_RETRY (পঠ (fd, &বার্তা, পরিমাণ (বার্তা)));
          যদি (ret >= 0 && বার্তা.type == SYNC_SOCKET_ERROR_MESSAGE)
            প্রত্যাগম crun_make_error (err, বার্তা.error_মান, "%s", বার্তা.message);
          errno = saved_errno;
        }
      প্রত্যাগম crun_make_error (err, errno, "write to sync socket");
    }

  প্রত্যাগম 0;
}

স্থির bool
has_cap_sys_ptrace ()
{
#ifdef HAVE_CAP
  সংরচনা __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  সংরচনা __user_cap_data_struct data[2] = { { 0 } };

  যদি (UNLIKELY (capget (&hdr, data) < 0))
    প্রত্যাগম true;

  প্রত্যাগম (data[0].effective & (1U << CAP_SYS_PTRACE)) != 0;
#অন্যথা
  প্রত্যাগম geteuid () == 0;
#endif
}

স্থির libcrun_container_t *
make_container (runtime_spec_schema_config_schema *container_def, ধ্রুব বর্ণ *path, ধ্রুব বর্ণ *config)
{
  libcrun_container_t *container = xmalloc0 (পরিমাণ (*container));
  container->container_def = container_def;

  container->host_uid = geteuid ();
  container->host_gid = getegid ();
  container->host_has_cap_sys_ptrace = has_cap_sys_ptrace ();
  container->proc_fd = -1;

  container->annotations = make_string_map_from_json (container_def->annotations);

  /* An empty namespace path means that a new namespace is to be created,
     the same as if it is not specified.  */
  যদি (container_def->linux)
    {
      size_t i;

      প্রতি (i = 0; i < container_def->linux->namespaces_len; i++)
        যদি (container_def->linux->namespaces[i]->path && container_def->linux->namespaces[i]->path[0] == '\0')
          {
            মুক্ত (container_def->linux->namespaces[i]->path);
            container_def->linux->namespaces[i]->path = রিক্ত;
          }
    }

  যদি (path)
    container->config_file = xstrdup (path);
  যদি (config)
    container->config_file_content = xstrdup (config);

  প্রত্যাগম container;
}

libcrun_container_t *
libcrun_container_load_from_memory (ধ্রুব বর্ণ *json, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free বর্ণ *oci_error = রিক্ত;
  container_def = runtime_spec_schema_config_schema_parse_data (json, রিক্ত, &oci_error);
  যদি (container_def == রিক্ত)
    {
      crun_make_error (err, 0, "load: `%s`", oci_error);
      প্রত্যাগম রিক্ত;
    }
  প্রত্যাগম make_container (container_def, রিক্ত, json);
}

libcrun_container_t *
libcrun_container_load_from_file (ধ্রুব বর্ণ *path, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free বর্ণ *oci_error = রিক্ত;
  cleanup_free বর্ণ *content = রিক্ত;
  size_t len;
  পূর্ণাঙ্ক ret;

  libcrun_debug ("Loading container from config file: `%s`", path);
  ret = read_all_file (path, &content, &len, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম রিক্ত;

  container_def = runtime_spec_schema_config_schema_parse_data (content, রিক্ত, &oci_error);
  যদি (container_def == রিক্ত)
    {
      crun_make_error (err, 0, "load `%s`: %s", path, oci_error);
      প্রত্যাগম রিক্ত;
    }
  প্রত্যাগম make_container (container_def, রিক্ত, content);
}

ধ্রুব বর্ণ *
libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err)
{
  সংরচনা parser_context ctx = { 0, stderr };
  parser_error gen_err = রিক্ত;

  যদি (container == রিক্ত)
    {
      crun_make_error (err, EINVAL, "no container specified");
      প্রত্যাগম রিক্ত;
    }

  যদি (container->config_file_content == রিক্ত && container->container_def)
    {
      container->config_file_content = runtime_spec_schema_config_schema_generate_json (container->container_def, &ctx, &gen_err);
      যদি (container->config_file_content == রিক্ত)
        {
          crun_make_error (err, 0, "cannot generate the configuration JSON: %s", gen_err ?: "unknown error");
          মুক্ত (gen_err);
          প্রত্যাগম রিক্ত;
        }
      মুক্ত (gen_err);
    }

  যদি (container->config_file_content == রিক্ত)
    {
      crun_make_error (err, 0, "the container has no configuration");
      প্রত্যাগম রিক্ত;
    }

  প্রত্যাগম container->config_file_content;
}

ধ্রুব বর্ণ *
libcrun_container_get_annotation (libcrun_container_t *container, ধ্রুব বর্ণ *চাবি)
{
  যদি (container == রিক্ত)
    প্রত্যাগম রিক্ত;
  প্রত্যাগম find_string_map_মান (container->annotations, চাবি);
}

size_t
libcrun_container_get_annotations_len (libcrun_container_t *container)
{
  যদি (container == রিক্ত || container->annotations == রিক্ত)
    প্রত্যাগম 0;
  প্রত্যাগম string_map_size (container->annotations);
}

পূর্ণাঙ্ক
libcrun_container_get_annotation_at (libcrun_container_t *container, size_t index, ধ্রুব বর্ণ **চাবি,
                                     ধ্রুব বর্ণ **মান)
{
  যদি (container == রিক্ত)
    {
      errno = ERANGE;
      প্রত্যাগম -1;
    }
  প্রত্যাগম string_map_get_at (container->annotations, index, চাবি, মান);
}

uid_t
libcrun_container_get_uid (libcrun_container_t *container)
{
  যদি (container == রিক্ত)
    প্রত্যাগম (uid_t) -1;
  প্রত্যাগম container->host_uid;
}

gid_t
libcrun_container_get_gid (libcrun_container_t *container)
{
  যদি (container == রিক্ত)
    প্রত্যাগম (gid_t) -1;
  প্রত্যাগম container->host_gid;
}

শূন্য
libcrun_container_free (libcrun_container_t *ctr)
{
  যদি (ctr == রিক্ত)
    প্রত্যাগম;

  যদি (ctr->cleanup_private_data)
    ctr->cleanup_private_data (ctr->private_data);

  যদি (ctr->container_def)
    free_runtime_spec_schema_config_schema (ctr->container_def);

  free_string_map (ctr->annotations);

  যদি (ctr->proc_fd >= 0)
    close (ctr->proc_fd);

  মুক্ত (ctr->config_file_content);
  মুক্ত (ctr->config_file);
  মুক্ত (ctr);
}

libcrun_context_t *
libcrun_context_new (ধ্রুব বর্ণ *id, ধ্রুব বর্ণ *state_মূল, libcrun_error_t *err)
{
  libcrun_context_t *ctx = xmalloc0 (পরিমাণ (*ctx));

  /* Go through the setters so that the owned_* copies and the borrowed
     pointers stay in sync and are released by libcrun_context_free.  */
  libcrun_context_set_id (ctx, id);
  libcrun_context_set_state_মূল (ctx, state_মূল);
  libcrun_context_set_bundle (ctx, ".");
  ctx->fifo_exec_wait_fd = -1;

  যদি (libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, id, রিক্ত, err) < 0)
    {
      libcrun_context_free (ctx);
      প্রত্যাগম রিক্ত;
    }

  ctx->handler_manager = libcrun_handler_manager_create (err);
  যদি (ctx->handler_manager == রিক্ত)
    {
      libcrun_context_free (ctx);
      প্রত্যাগম রিক্ত;
    }

#ifdef CRUN_LIBDIR
  যদি (access (CRUN_LIBDIR "/handlers", F_OK) == 0)
    {
      যদি (libcrun_handler_manager_load_directory (ctx->handler_manager, CRUN_LIBDIR "/handlers", err) < 0)
        {
          libcrun_context_free (ctx);
          প্রত্যাগম রিক্ত;
        }
    }
#endif

  প্রত্যাগম ctx;
}

শূন্য
libcrun_context_free (libcrun_context_t *ctx)
{
  যদি (ctx == রিক্ত)
    প্রত্যাগম;

  মুক্ত (ctx->owned_state_মূল);
  মুক্ত (ctx->owned_id);
  মুক্ত (ctx->owned_bundle);
  মুক্ত (ctx->owned_console_socket);
  মুক্ত (ctx->owned_pid_file);
  মুক্ত (ctx->owned_notify_socket);
  মুক্ত (ctx->owned_handler);

  যদি (ctx->handler_manager)
    handler_manager_free (ctx->handler_manager);

  মুক্ত (ctx);
}

#define LIBCRUN_CONTEXT_SET_STRING(field)                  \
  কুরু                                                       \
    {                                                      \
      মুক্ত (ctx->owned_##field);                           \
      ctx->owned_##field = মান ? xstrdup (মান) : রিক্ত; \
      ctx->field = ctx->owned_##field;                     \
  } যাবত্ (0)

শূন্য
libcrun_context_set_id (libcrun_context_t *ctx, ধ্রুব বর্ণ *মান)
{
  LIBCRUN_CONTEXT_SET_STRING (id);
}

শূন্য
libcrun_context_set_state_মূল (libcrun_context_t *ctx, ধ্রুব বর্ণ *মান)
{
  LIBCRUN_CONTEXT_SET_STRING (state_মূল);
}

শূন্য
libcrun_context_set_bundle (libcrun_context_t *ctx, ধ্রুব বর্ণ *মান)
{
  LIBCRUN_CONTEXT_SET_STRING (bundle);
}

শূন্য
libcrun_context_set_console_socket (libcrun_context_t *ctx, ধ্রুব বর্ণ *মান)
{
  LIBCRUN_CONTEXT_SET_STRING (console_socket);
}

শূন্য
libcrun_context_set_pid_file (libcrun_context_t *ctx, ধ্রুব বর্ণ *মান)
{
  LIBCRUN_CONTEXT_SET_STRING (pid_file);
}

শূন্য
libcrun_context_set_notify_socket (libcrun_context_t *ctx, ধ্রুব বর্ণ *মান)
{
  LIBCRUN_CONTEXT_SET_STRING (notify_socket);
}

শূন্য
libcrun_context_set_handler (libcrun_context_t *ctx, ধ্রুব বর্ণ *মান)
{
  LIBCRUN_CONTEXT_SET_STRING (handler);
}

#undef LIBCRUN_CONTEXT_SET_STRING

ধ্রুব বর্ণ *
libcrun_context_get_pid_file (libcrun_context_t *ctx)
{
  প্রত্যাগম ctx->pid_file;
}

ধ্রুব বর্ণ *
libcrun_context_get_state_মূল (libcrun_context_t *ctx)
{
  যদি (ctx == রিক্ত)
    প্রত্যাগম রিক্ত;
  প্রত্যাগম ctx->state_মূল;
}

শূন্য
libcrun_context_set_preserve_fds (libcrun_context_t *ctx, পূর্ণাঙ্ক preserve_fds)
{
  ctx->preserve_fds = preserve_fds;
}

পূর্ণাঙ্ক
libcrun_context_get_preserve_fds (libcrun_context_t *ctx)
{
  প্রত্যাগম ctx->preserve_fds;
}

শূন্য
libcrun_context_set_listen_fds (libcrun_context_t *ctx, পূর্ণাঙ্ক listen_fds)
{
  ctx->listen_fds = listen_fds;
}

শূন্য
libcrun_context_set_args (libcrun_context_t *ctx, পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  ctx->argc = argc;
  ctx->argv = argv;
}

শূন্য
libcrun_context_set_systemd_cgroup (libcrun_context_t *ctx, bool মান)
{
  ctx->systemd_cgroup = মান;
}

শূন্য
libcrun_context_set_detach (libcrun_context_t *ctx, bool মান)
{
  ctx->detach = মান;
}

শূন্য
libcrun_context_set_no_new_keyring (libcrun_context_t *ctx, bool মান)
{
  ctx->no_new_keyring = মান;
}

শূন্য
libcrun_context_set_no_pivot (libcrun_context_t *ctx, bool মান)
{
  ctx->no_pivot = মান;
}

শূন্য
libcrun_context_set_force_no_cgroup (libcrun_context_t *ctx, bool মান)
{
  ctx->force_no_cgroup = মান;
}

শূন্য
libcrun_context_set_output_handler (libcrun_context_t *ctx, crun_output_handler handler, শূন্য *arg)
{
  ctx->output_handler = handler;
  ctx->output_handler_arg = arg;
}

পূর্ণাঙ্ক
libcrun_context_init_logging (libcrun_context_t *ctx, ধ্রুব বর্ণ *log, libcrun_error_t *err)
{
  প্রত্যাগম libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, ctx->id, log, err);
}

স্থির পূর্ণাঙ্ক
block_signals (libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  sigset_t mask;
  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "sigprocmask");
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
unblock_signals (libcrun_error_t *err)
{
  পূর্ণাঙ্ক i;
  পূর্ণাঙ্ক ret;
  sigset_t mask;
  সংরচনা sigaction act = {};

  sigfillset (&mask);
  ret = sigprocmask (SIG_UNBLOCK, &mask, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "sigprocmask");

  act.sa_handler = SIG_DFL;
  প্রতি (i = 0; i < NSIG; i++)
    {
      ret = sigaction (i, &act, রিক্ত);
      যদি (ret < 0 && errno != EINVAL)
        প্রত্যাগম crun_make_error (err, errno, "sigaction");
    }

  প্রত্যাগম 0;
}

/* must be used on the host before pivot_root(2).  */
স্থির পূর্ণাঙ্ক
initialize_security (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  ret = libcrun_init_caps (container, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (UNLIKELY (proc == রিক্ত))
    প্রত্যাগম 0;

  যদি (proc->apparmor_profile)
    {
      ret = libcrun_initialize_apparmor (err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  ret = libcrun_initialize_selinux (container, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  প্রত্যাগম 0;
}

/* Emit the "annotations" map.  Returns a json_gen_status; callers can wrap
   the call with GEN_OR_FAIL.  */
স্থির পূর্ণাঙ্ক
gen_annotations (json_gen_ctx *gen, json_map_string_string *annotations)
{
  size_t i;
  পূর্ণাঙ্ক r;

  GEN_চাবি (gen, "annotations");
  GEN_OR_FAIL (json_gen_map_open (gen));

  প্রতি (i = 0; i < annotations->len; i++)
    {
      /* An annotation whose value is null in the document is parsed into a
         NULL, which cannot be emitted as a JSON string; leave it out.  */
      যদি (annotations->চাবিগুলি[i] == রিক্ত || annotations->মানসমূহ[i] == রিক্ত)
        অনুবর্ত;

      GEN_STR (gen, annotations->চাবিগুলি[i]);
      GEN_STR (gen, annotations->মানসমূহ[i]);
    }

  প্রত্যাগম json_gen_map_close (gen);

gen_error:
  প্রত্যাগম r;
}

/* Return the bundle path to report in the container state.  crun always
   chdir()s into the bundle, so fall back to getcwd() when the caller has no
   absolute bundle path, as can happen when libcrun is used as a library.
   *ALLOCATED must be freed by the caller.  */
স্থির ধ্রুব বর্ণ *
get_bundle_path (ধ্রুব বর্ণ *bundle, বর্ণ **allocated)
{
  যদি (bundle != রিক্ত && bundle[0] == '/')
    প্রত্যাগম bundle;

  *allocated = getcwd (রিক্ত, 0);
  যদি (*allocated == রিক্ত)
    OOM ();
  প্রত্যাগম *allocated;
}

স্থির পূর্ণাঙ্ক
do_hooks (runtime_spec_schema_config_schema *def, pid_t pid, ধ্রুব বর্ণ *id, bool keep_going, ধ্রুব বর্ণ *bundle,
          ধ্রুব বর্ণ *cwd, ধ্রুব বর্ণ *status, ধ্রুব বর্ণ *name, hook **hooks, size_t hooks_len, পূর্ণাঙ্ক out_fd,
          পূর্ণাঙ্ক err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  size_t i, stdin_len;
  পূর্ণাঙ্ক r, ret;
  বর্ণ *stdin = রিক্ত;
  cleanup_free বর্ণ *bundle_allocated = রিক্ত;
  ধ্রুব বর্ণ *rootfs = def->মূল ? def->মূল->path : "";
  cleanup_json_gen json_gen_ctx *gen = রিক্ত;

  bundle = get_bundle_path (bundle, &bundle_allocated);

  যদি (! json_gen_init (&gen, রিক্ত))
    প্রত্যাগম crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 0);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_চাবি (gen, "ociVersion");
  GEN_STR (gen, "1.0");

  GEN_চাবি (gen, "id");
  GEN_STR (gen, id);

  GEN_চাবি (gen, "pid");
  GEN_OR_FAIL (map_int (gen, pid));

  GEN_চাবি (gen, "root");
  GEN_STR (gen, rootfs);

  GEN_চাবি (gen, "bundle");
  GEN_STR (gen, bundle);

  GEN_চাবি (gen, "status");
  GEN_STR (gen, status);

  যদি (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, (ধ্রুব বর্ণ **) &stdin, &stdin_len));

  ret = 0;

  bool error_created = false;
  প্রতি (i = 0; i < hooks_len; i++)
    {
      বর্ণ **env = environ;

      যদি (hooks[i]->env)
        env = hooks[i]->env;

      /* Release the error from the previous iteration, if any.  */
      যদি (error_created)
        {
          crun_error_release (err);
          error_created = false;
        }

      ret = run_process_with_stdin_timeout_envp (hooks[i]->path, hooks[i]->args, cwd, hooks[i]->timeout, env,
                                                 stdin, stdin_len, out_fd, err_fd, can_ignore_chdir_errors, err);
      যদি (UNLIKELY (ret < 0))
        error_created = true;

      যদি (UNLIKELY (ret != 0))
        {
          যদি (keep_going)
            libcrun_warning ("error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
          অন্যথা
            {
              libcrun_error (0, "error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
              ভঙ্গ;
            }
        }
    }

  প্রত্যাগম ret;

gen_error:
  প্রত্যাগম json_gen_error_to_crun_error (r, err);
}

স্থির পূর্ণাঙ্ক
get_json_gen_result (json_gen_ctx *gen, বর্ণ **out, size_t *out_len)
{
  ধ্রুব বর্ণ *buf = রিক্ত;
  size_t buf_len = 0;
  পূর্ণাঙ্ক r;

  r = json_gen_get_buf (gen, &buf, &buf_len);
  যদি (UNLIKELY (r != json_gen_status_ok))
    প্রত্যাগম r;

  *out_len = buf_len;

  *out = xmalloc (buf_len + 1);
  memcpy (*out, buf, buf_len);
  (*out)[buf_len] = '\0';

  প্রত্যাগম json_gen_status_ok;
}

স্থির পূর্ণাঙ্ক
get_seccomp_receiver_fd_payload (libcrun_container_t *container, ধ্রুব বর্ণ *status, pid_t own_pid,
                                 বর্ণ **seccomp_fd_payload, size_t *seccomp_fd_payload_len, libcrun_error_t *err)
{
  পূর্ণাঙ্ক r;
  json_gen_ctx *gen = রিক্ত;
  runtime_spec_schema_config_schema *def = container->container_def;
  ধ্রুব বর্ণ *ধ্রুব OCI_VERSION = "0.2.0";

  যদি (! json_gen_init (&gen, রিক্ত))
    প্রত্যাগম crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_চাবি (gen, "ociVersion");
  GEN_STR (gen, OCI_VERSION);

  GEN_চাবি (gen, "fds");
  GEN_OR_FAIL (json_gen_array_open (gen));

  GEN_চাবি (gen, "seccompFd");
  GEN_OR_FAIL (json_gen_array_close (gen));

  GEN_চাবি (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  যদি (def && def->linux && def->linux->seccomp)
    {
      ধ্রুব বর্ণ *metadata = def->linux->seccomp->listener_metadata;

      যদি (metadata)
        {
          GEN_চাবি (gen, "metadata");
          GEN_STR (gen, metadata);
        }
    }

  /* State.  */
  GEN_চাবি (gen, "state");
  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_চাবি (gen, "ociVersion");
  GEN_STR (gen, OCI_VERSION);

  যদি (container->context && container->context->id)
    {
      GEN_চাবি (gen, "id");
      GEN_STR (gen, container->context->id);
    }

  GEN_চাবি (gen, "status");
  GEN_STR (gen, status);

  GEN_চাবি (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  যদি (container->context && container->context->bundle)
    {
      GEN_চাবি (gen, "bundle");
      GEN_STR (gen, container->context->bundle);
    }

  যদি (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));
  /* End state.  */

  GEN_OR_FAIL (json_gen_map_close (gen));

  r = get_json_gen_result (gen, seccomp_fd_payload, seccomp_fd_payload_len);

gen_error:
  json_gen_free (gen);

  প্রত্যাগম json_gen_error_to_crun_error (r, err);
}

স্থির পূর্ণাঙ্ক
send_sync_cb (শূন্য *data, libcrun_error_t *err)
{
  পূর্ণাঙ্ক sync_socket_fd = *((পূর্ণাঙ্ক *) data);
  পূর্ণাঙ্ক ret;

  /* sync 2.  */
  ret = sync_socket_send_sync (sync_socket_fd, false, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  /* sync 3.  */
  প্রত্যাগম sync_socket_wait_sync (রিক্ত, sync_socket_fd, false, err);
}

স্থির পূর্ণাঙ্ক
maybe_chown_std_streams (uid_t container_uid, gid_t container_gid, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret, i;

  প্রতি (i = 0; i < 3; i++)
    {
      যদি (! isatty (i))
        {
          সংরচনা stat statbuf;
          ret = fstat (i, &statbuf);
          যদি (UNLIKELY (ret < 0))
            {
              যদি (errno == EBADF)
                অনুবর্ত;
              প্রত্যাগম crun_make_error (err, errno, "fstat fd `%d`", i);
            }

          /* Skip chown for device files */
          যদি (S_ISCHR (statbuf.st_mode) || S_ISBLK (statbuf.st_mode))
            অনুবর্ত;

          ret = fchown (i, container_uid, container_gid);
          যদি (UNLIKELY (ret < 0))
            {
              /* EINVAL means the user is not mapped in the current userns.
                 Ignore EPERM and EROFS as well as there is no reason to fail
                 so early, and let the container payload deal with it.
                 EBADF means fd is closed.
              */
              যদি (errno == EINVAL || errno == EPERM || errno == EROFS || errno == EBADF)
                অনুবর্ত;

              প্রত্যাগম crun_make_error (err, errno, "fchown std stream `%i`", i);
            }
        }
    }
  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
libcrun_container_notify_handler (সংরচনা container_entrypoint_s *args,
                                  গণনা handler_configure_phase phase,
                                  libcrun_container_t *container, ধ্রুব বর্ণ *rootfs,
                                  libcrun_error_t *err)
{
  সংরচনা custom_handler_s *h;

  যদি (args->custom_handler == রিক্ত)
    প্রত্যাগম 0;

  h = args->custom_handler->vtable;
  যদি (h == রিক্ত || h->configure_container == রিক্ত)
    প্রত্যাগম 0;

  প্রত্যাগম h->configure_container (args->custom_handler->cookie, phase,
                                 args->context, container,
                                 rootfs, err);
}

/* Resolve and normalize the container rootfs path.  */
স্থির পূর্ণাঙ্ক
resolve_rootfs_path (libcrun_container_t *container, বর্ণ **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  যদি (def->মূল && def->মূল->path)
    {
      *rootfs = realpath (def->মূল->path, রিক্ত);
      যদি (UNLIKELY (*rootfs == রিক্ত))
        {
          পূর্ণাঙ্ক saved_errno = errno;

          /* If realpath failed for any reason, try the relative directory.  */
          যদি (def->মূল->path[0] == '/')
            {
              cleanup_free বর্ণ *cwd = রিক্ত;
              libcrun_error_t tmp_err = রিক্ত;
              ssize_t len;
              পূর্ণাঙ্ক proc_fd;

              proc_fd = libcrun_get_cached_proc_fd (container, &tmp_err);
              যদি (LIKELY (proc_fd >= 0))
                {
                  len = safe_readlinkat (proc_fd, "self/cwd", &cwd, 0, &tmp_err);
                  যদি (LIKELY (len > 0))
                    {
                      /* If the rootfs is under the current working directory, just use its relative path.  */
                      যদি (has_dir_prefix (def->মূল->path, cwd))
                        {
                          ধ্রুব বর্ণ *it = consume_slashes (def->মূল->path + len);
                          যদি (LIKELY (*it))
                            {
                              পূর্ণাঙ্ক ret = faccessat (AT_FDCWD, it, F_OK, 0);
                              যদি (LIKELY (ret == 0))
                                *rootfs = xstrdup (it);
                            }
                        }
                    }
                }
              crun_error_release (&tmp_err);
            }

          যদি (*rootfs == রিক্ত)
            {
              যদি (saved_errno == ENOENT)
                প্রত্যাগম crun_make_error (err, ENOENT, "the rootfs path `%s` does not exist", def->মূল->path);

              *rootfs = xstrdup (def->মূল->path);
            }
        }
    }
  প্রত্যাগম 0;
}

/* Initialize the environment variables.  */
স্থির পূর্ণাঙ্ক
setup_environment (runtime_spec_schema_config_schema *def, uid_t container_uid, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  যদি (clearenv ())
    প্রত্যাগম crun_make_error (err, errno, "clearenv");

  যদি (def->process)
    {
      size_t i;

      প্রতি (i = 0; i < def->process->env_len; i++)
        {
          /* A null element of the array of variables is parsed into a NULL,
             which putenv(3) cannot be handed.  */
          যদি (def->process->env[i] == রিক্ত)
            প্রত্যাগম crun_make_error (err, EINVAL, "environment variable is not specified");

          যদি (putenv (def->process->env[i]) < 0)
            প্রত্যাগম crun_make_error (err, errno, "putenv `%s`", def->process->env[i]);
        }
    }

  যদি (is_empty_string (getenv ("HOME")))
    {
      ret = set_home_env (container_uid);
      যদি (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          যদি (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            প্রত্যাগম crun_make_error (err, errno, "setenv HOME");
        }
    }

  প্রত্যাগম 0;
}

/* Configure terminal and console for interactive containers.  */
স্থির পূর্ণাঙ্ক
setup_terminal (সংরচনা container_entrypoint_s *entrypoint_args, libcrun_container_t *container,
                পূর্ণাঙ্ক console_socket, পূর্ণাঙ্ক console_socketpair, পূর্ণাঙ্ক has_terminal, libcrun_error_t *err)
{
  cleanup_close পূর্ণাঙ্ক terminal_fd = -1;
  পূর্ণাঙ্ক ret;

  যদি (! has_terminal)
    প্রত্যাগম 0;

  fflush (stderr);

  যদি (console_socket >= 0 || (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0))
    {
      terminal_fd = libcrun_set_terminal (container, err);
      যদি (UNLIKELY (terminal_fd < 0))
        প্রত্যাগম terminal_fd;
    }

  যদি (console_socket >= 0)
    {
      ret = send_fd_to_socket (console_socket, terminal_fd, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }
  অন্যথা যদি (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0)
    {
      ret = send_fd_to_socket (console_socketpair, terminal_fd, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  প্রত্যাগম 0;
}

/* Resolve and validate the container's executable path.  */
স্থির পূর্ণাঙ্ক
setup_executable_path (সংরচনা container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                       বর্ণ **exec_path, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  যদি (def->process && def->process->args)
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      যদি (UNLIKELY (ret < 0))
        {
          যদি (entrypoint_args->custom_handler == রিক্ত && crun_error_get_errno (err) == ENOENT)
            প্রত্যাগম ret;

          /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
             once the process switched to the use that runs in the container.  This might be necessary
             when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
             is not honored.  */
          crun_error_release (err);
        }
    }

  প্রত্যাগম 0;
}

/* Apply security settings for the container process.  */
স্থির পূর্ণাঙ্ক
apply_security_settings (সংরচনা container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                         libcrun_container_t *container, pid_t own_pid, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  runtime_spec_schema_config_schema_process_capabilities *capabilities;
  পূর্ণাঙ্ক no_new_privs;

  যদি (def->process && ! def->process->no_new_privileges)
    {
      বর্ণ **seccomp_flags = রিক্ত;
      size_t seccomp_flags_len = 0;
      cleanup_free বর্ণ *seccomp_fd_payload = রিক্ত;
      size_t seccomp_fd_payload_len = 0;

      যদি (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      যদি (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  capabilities = def->process ? def->process->capabilities : রিক্ত;
  no_new_privs = def->process ? def->process->no_new_privileges : 1;
  ret = libcrun_set_caps (capabilities, container->container_uid, container->container_gid, no_new_privs,
                          entrypoint_args->custom_handler != রিক্ত, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  প্রত্যাগম 0;
}

/* Create the process that joins the existing PID namespace now that the
   host mounts are not visible anymore, and continue the setup from there.  */
স্থির পূর্ণাঙ্ক
do_delayed_pidns_fork (পূর্ণাঙ্ক sync_socket, pid_t *own_pid, libcrun_error_t *err)
{
  pid_t pid;
  পূর্ণাঙ্ক ret;

  pid = fork ();
  যদি (UNLIKELY (pid < 0))
    প্রত্যাগম crun_make_error (err, errno, "fork");

  যদি (pid)
    {
      /* Report the new PID to the parent and exit immediately.  */
      ret = sync_socket_send_sync (sync_socket, false, err);
      যদি (LIKELY (ret == 0))
        {
          ret = TEMP_FAILURE_RETRY (লিখ (sync_socket, &pid, পরিমাণ (pid)));
          যদি (LIKELY (ret == পরিমাণ (pid)))
            _safe_exit (EXIT_SUCCESS);
        }

      kill (pid, SIGKILL);
      _safe_exit (EXIT_FAILURE);
    }

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (পঠ (sync_socket, own_pid, পরিমাণ (*own_pid)));
  যদি (UNLIKELY (ret != পরিমাণ (*own_pid)))
    {
      যদি (ret >= 0)
        errno = 0;
      প্রত্যাগম crun_make_error (err, errno, "read from sync socket");
    }

  প্রত্যাগম 0;
}

সংরচনা delayed_pidns_fork_s
{
  libcrun_container_t *container;
  পূর্ণাঙ্ক sync_socket;
  pid_t *own_pid;
};

স্থির পূর্ণাঙ্ক
delayed_pidns_fork_cb (শূন্য *data, libcrun_error_t *err)
{
  সংরচনা delayed_pidns_fork_s *d = data;

  যদি (! libcrun_container_delayed_pidns_fork (d->container))
    প্রত্যাগম 0;

  প্রত্যাগম do_delayed_pidns_fork (d->sync_socket, d->own_pid, err);
}

স্থির পূর্ণাঙ্ক
container_init_setup (শূন্য *args, pid_t *own_pid, বর্ণ *notify_socket,
                      পূর্ণাঙ্ক sync_socket, বর্ণ **exec_path, libcrun_error_t *err)
{
  সংরচনা container_entrypoint_s *entrypoint_args = args;
  libcrun_container_t *container = entrypoint_args->container;
  bool chdir_done = false;
  পূর্ণাঙ্ক ret;
  পূর্ণাঙ্ক has_terminal;
  cleanup_close পূর্ণাঙ্ক console_socket = -1;
  cleanup_close পূর্ণাঙ্ক console_socketpair = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free বর্ণ *rootfs = রিক্ত;
  সংরচনা delayed_pidns_fork_s delayed_pidns_fork = {
    .container = container,
    .sync_socket = sync_socket,
    .own_pid = own_pid,
  };

  ret = initialize_security (container, def->process, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = libcrun_configure_network (container, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = resolve_rootfs_path (container, &rootfs, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  /* Configure terminal socket pair for container communication.  */
  যদি (entrypoint_args->terminal_socketpair[0] >= 0)
    {
      close_and_reset (&entrypoint_args->terminal_socketpair[0]);
      console_socketpair = entrypoint_args->terminal_socketpair[1];
    }

  /* sync 1.  */
  ret = sync_socket_wait_sync (রিক্ত, sync_socket, false, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  has_terminal = container->container_def->process && container->container_def->process->terminal;
  যদি (has_terminal && entrypoint_args->context->console_socket)
    console_socket = entrypoint_args->console_socket_fd;

  ret = libcrun_set_sysctl (container, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_BEFORE_MOUNTS, container, rootfs, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = libcrun_do_pivot_মূল (container, entrypoint_args->context->no_pivot, &rootfs, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  /* sync 2 and 3 are sent as part of libcrun_set_mounts.  */
  ret = libcrun_set_mounts (entrypoint_args, container, rootfs, send_sync_cb, &sync_socket, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  /* The createContainer hooks MUST run after the mounts are set up but before
     the pivot_root.  When these hooks are present can_use_open_tree_namespace()
     forces the deferred-pivot path, so the actual pivot happens later in
     libcrun_finalize_mounts().  */
  যদি (def->hooks && def->hooks->create_container_len)
    {
      libcrun_error_t tmp_err = রিক্ত;
      পূর্ণাঙ্ক in_userns = check_running_in_user_namespace (&tmp_err);
      যদি (tmp_err)
        crun_error_release (&tmp_err);
      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, রিক্ত, "created", "createContainer",
                      (hook **) def->hooks->create_container,
                      def->hooks->create_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      যদি (UNLIKELY (ret != 0))
        প্রত্যাগম ret;
    }

  ret = libcrun_finalize_mounts (entrypoint_args, container, rootfs, delayed_pidns_fork_cb, &delayed_pidns_fork, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (def->process)
    {
      ret = libcrun_set_selinux_label (container, def->process, false, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      ret = libcrun_set_apparmor_profile (container, def->process, false, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  ret = mark_or_close_fds_ge_than (container, entrypoint_args->context->preserve_fds + 3, false, err);
  যদি (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ret = libcrun_reopen_dev_null (err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = maybe_chown_std_streams (container->container_uid, container->container_gid, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = setup_environment (def, container->container_uid, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  /* Set primary process to 1 explicitly if nothing is configured and LISTEN_FD is not set.  */
  যদি (entrypoint_args->context->listen_fds > 0 && getenv ("LISTEN_PID") == রিক্ত)
    {
      libcrun_warning ("setting LISTEN_PID=1 since no previous configuration was found");
      যদি (UNLIKELY (setenv ("LISTEN_PID", "1", 1) < 0))
        প্রত্যাগম crun_make_error (err, errno, "setenv LISTEN_PID");
    }

  /* Attempt to chdir immediately here, before doing the setresuid.  If we fail here, let's
     try again later once the process switched to the user that runs in the container.  */
  যদি (def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      যদি (LIKELY (ret == 0))
        chdir_done = true;
      অন্যথা
        crun_error_release (err);
    }

  ret = setsid ();
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "setsid");

  ret = setup_terminal (entrypoint_args, container, console_socket, console_socketpair, has_terminal, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  close_and_reset (&console_socket);
  close_and_reset (&console_socketpair);

  ret = setup_executable_path (entrypoint_args, def, exec_path, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = libcrun_set_hostname (container, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = libcrun_set_domainname (container, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  যদি (def->process && def->process->user)
    umask (def->process->user->umask_present ? def->process->user->umask : 0022);

  ret = apply_security_settings (entrypoint_args, def, container, *own_pid, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (UNLIKELY (def->process && def->process->args && *exec_path == রিক্ত))
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      যদি (UNLIKELY (ret < 0))
        {
          যদি (entrypoint_args->custom_handler == রিক্ত || is_empty_string (def->process->args[0]))
            প্রত্যাগম ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          *exec_path = xstrdup (def->process->args[0]);
        }
    }

  /* The chdir was not already performed, so try again now after switching to the UID/GID in the container.  */
  যদি (! chdir_done && def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  যদি (notify_socket && putenv (notify_socket) < 0)
    প্রত্যাগম crun_make_error (err, errno, "putenv `%s`", notify_socket);

  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
open_hooks_output (libcrun_container_t *container, পূর্ণাঙ্ক *out_fd, পূর্ণাঙ্ক *err_fd, libcrun_error_t *err)
{
  ধ্রুব বর্ণ *annotation;

  *err_fd = *out_fd = -1;

  libcrun_debug ("Opening hooks output");
  annotation = find_annotation (container, "run.oci.hooks.stdout");
  যদি (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stdout` annotation");
      *out_fd = TEMP_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      যদি (UNLIKELY (*out_fd < 0))
        প্রত্যাগম crun_make_error (err, errno, "open `%s`", annotation);
    }

  annotation = find_annotation (container, "run.oci.hooks.stderr");
  যদি (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stderr` annotation");
      *err_fd = TEMP_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      যদি (UNLIKELY (*err_fd < 0))
        প্রত্যাগম crun_make_error (err, errno, "open `%s`", annotation);
    }

  প্রত্যাগম 0;
}

/* The environment the process was started with lives on the initial stack, right after the
   argv strings and before the string AT_EXECFN points to.  clearenv(3) only drops the pointers
   to it, so when there is no execv to replace the stack, erase it or the container process can
   still read it, e.g. through /proc/self/environ.  */
স্থির শূন্য
erase_initial_environment (বর্ণ **argv, পূর্ণাঙ্ক argc)
{
  বর্ণ *start = argv[argc - 1] + strlen (argv[argc - 1]) + 1;
  বর্ণ *end = (বর্ণ *) getauxval (AT_EXECFN);

  যদি (end > start)
    memset (start, 0, end - start);
}

স্থির শূন্য
rewrite_argv (বর্ণ **argv, পূর্ণাঙ্ক argc, ধ্রুব বর্ণ *name, বর্ণ **args, size_t args_len)
{
  cleanup_free বর্ণ *decorated_name = রিক্ত;
  size_t available_len = argv[argc - 1] - argv[0] + strlen (argv[argc - 1]) + 1;
  cleanup_free বর্ণ *new_argv = রিক্ত;
  size_t i, so_far = 0, needed = 0;

  needed = xasprintf (&decorated_name, "[libcrun:%s]", name) + 1;
  প্রতি (i = 0; i < args_len; i++)
    needed += strlen (args[i]) + 1;

  new_argv = xmalloc0 (needed + 1);
  strcpy (new_argv, decorated_name);
  so_far = strlen (decorated_name) + 1;

  যদি (available_len >= needed)
    {
      প্রতি (i = 0; i < args_len; i++)
        {
          strcpy (new_argv + so_far, args[i]);
          so_far += strlen (args[i]) + 1;
        }
    }

  যদি (so_far >= available_len)
    so_far = available_len - 1;

  memset (argv[0], 0, available_len);
  memcpy (argv[0], new_argv, so_far);
}

/* Entrypoint to the container.  */
স্থির পূর্ণাঙ্ক
container_init (শূন্য *args, বর্ণ *notify_socket, পূর্ণাঙ্ক sync_socket, libcrun_error_t *err)
{
  সংরচনা container_entrypoint_s *entrypoint_args = args;
  পূর্ণাঙ্ক ret;
  runtime_spec_schema_config_schema *def = entrypoint_args->container->container_def;
  cleanup_free বর্ণ *exec_path = রিক্ত;
  __attribute__ ((unused)) cleanup_free বর্ণ *notify_socket_cleanup = notify_socket;
  pid_t own_pid = 0;

  entrypoint_args->sync_socket = sync_socket;

  crun_set_output_handler (log_write_to_sync_socket, args);

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (পঠ (sync_socket, &own_pid, পরিমাণ (own_pid)));
  যদি (UNLIKELY (ret != পরিমাণ (own_pid)))
    {
      যদি (ret >= 0)
        errno = 0;
      প্রত্যাগম crun_make_error (err, errno, "read from sync socket");
    }

  ret = container_init_setup (args, &own_pid, notify_socket, sync_socket, &exec_path, err);
  যদি (UNLIKELY (ret < 0))
    {
      /* If it fails to write the error using the sync socket, then fallback
         to stderr.  */
      যদি (sync_socket_write_error (sync_socket, err) == 0)
        {
          crun_error_release (err);
          /* A return value of > 0 means to fail with an error without err being set so to avoid
             printing it twice.  */
          প্রত্যাগম 1;
        }
      প্রত্যাগম ret;
    }

  entrypoint_args->sync_socket = -1;

  ret = unblock_signals (err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  /* sync 4.  */
  ret = sync_socket_send_sync (sync_socket, false, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  close_and_reset (&sync_socket);

  যদি (entrypoint_args->context->fifo_exec_wait_fd >= 0)
    {
      বর্ণ buffer[1];
      fd_set read_set;
      cleanup_close পূর্ণাঙ্ক fd = entrypoint_args->context->fifo_exec_wait_fd;
      entrypoint_args->context->fifo_exec_wait_fd = -1;

      FD_ZERO (&read_set);
      FD_SET (fd, &read_set);
      কুরু
        {
          ret = select (fd + 1, &read_set, রিক্ত, রিক্ত, রিক্ত);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম crun_make_error (err, errno, "select");

          ret = TEMP_FAILURE_RETRY (পঠ (fd, buffer, পরিমাণ (buffer)));
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম crun_make_error (err, errno, "read from the exec fifo");
      } যাবত্ (ret == 0);

      close_and_reset (&entrypoint_args->context->fifo_exec_wait_fd);
    }

  crun_set_output_handler (log_write_to_stderr, রিক্ত);

  যদি (def->process && def->process->no_new_privileges)
    {
      বর্ণ **seccomp_flags = রিক্ত;
      size_t seccomp_flags_len = 0;
      cleanup_free বর্ণ *seccomp_fd_payload = রিক্ত;
      size_t seccomp_fd_payload_len = 0;

      যদি (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      যদি (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (entrypoint_args->container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags,
                                   seccomp_flags_len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  যদি (UNLIKELY (def->process == রিক্ত))
    প্রত্যাগম crun_make_error (err, 0, "block `process` not found");

  যদি (UNLIKELY (exec_path == রিক্ত))
    প্রত্যাগম crun_make_error (err, 0, "executable path not specified");

  যদি (def->hooks && def->hooks->start_container_len)
    {
      libcrun_container_t *container = entrypoint_args->container;
      libcrun_error_t tmp_err = রিক্ত;
      পূর্ণাঙ্ক in_userns = check_running_in_user_namespace (&tmp_err);
      যদি (tmp_err)
        crun_error_release (&tmp_err);

      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, রিক্ত, "created", "startContainer",
                      (hook **) def->hooks->start_container,
                      def->hooks->start_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      যদি (UNLIKELY (ret != 0))
        প্রত্যাগম ret;

      /* Seek stdout/stderr to the end.  If the hooks were using the same files,
         the container process overwrites what was previously written.  */
      (শূন্য) lseek (1, 0, SEEK_END);
      (শূন্য) lseek (2, 0, SEEK_END);
    }

  যদি (entrypoint_args->custom_handler)
    {
      /* Files marked with O_CLOEXEC are closed at execv time, so make sure they are closed now.
         This is a best effort operation, because the seccomp filter is already in place and it could
         stop some syscalls used by mark_or_close_fds_ge_than.
      */
      যদি (entrypoint_args->custom_handler->vtable->close_fds)
        {
          ret = entrypoint_args->custom_handler->vtable->close_fds (entrypoint_args->custom_handler->cookie,
                                                                    entrypoint_args->container,
                                                                    entrypoint_args->context->preserve_fds,
                                                                    err);
        }
      অন্যথা
        {
          ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
        }
      যদি (UNLIKELY (ret < 0))
        crun_error_release (err);

      prctl (PR_SET_NAME, entrypoint_args->custom_handler->vtable->name);

      যদি (entrypoint_args->context->argv)
        {
          erase_initial_environment (entrypoint_args->context->argv, entrypoint_args->context->argc);
          rewrite_argv (entrypoint_args->context->argv, entrypoint_args->context->argc,
                        entrypoint_args->custom_handler->vtable->name, def->process->args,
                        def->process->args_len);

          /* It is a quite destructive operation as we might be referencing data from the old
             argv, so make sure the context is not reused.  */
          entrypoint_args->context->argv = রিক্ত;
          entrypoint_args->context = রিক্ত;
        }

      ret = libcrun_set_selinux_label (entrypoint_args->container, def->process, true, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      ret = libcrun_set_apparmor_profile (entrypoint_args->container, def->process, true, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      ret = entrypoint_args->custom_handler->vtable->run_func (entrypoint_args->custom_handler->cookie,
                                                               entrypoint_args->container,
                                                               exec_path,
                                                               def->process->args);
      যদি (ret != 0)
        প্রত্যাগম crun_make_error (err, 0, "exec container process failed with handler as `%s`", entrypoint_args->custom_handler->vtable->name);

      প্রত্যাগম ret;
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation since the seccomp profile is already in place now and might block
     some of the syscalls needed by mark_or_close_fds_ge_than.  */
  ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
  যদি (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, def->process->args));

  যদি (errno == ENOENT)
    প্রত্যাগম crun_make_error (err, errno, "exec container process (missing dynamic library?) `%s`", exec_path);

  প্রত্যাগম crun_make_error (err, errno, "exec container process `%s`", exec_path);
}

স্থির পূর্ণাঙ্ক
read_container_config_from_state (libcrun_container_t **container, ধ্রুব বর্ণ *state_মূল, ধ্রুব বর্ণ *id,
                                  libcrun_error_t *err)
{
  cleanup_free বর্ণ *config_file = রিক্ত;
  cleanup_free বর্ণ *dir = রিক্ত;
  পূর্ণাঙ্ক ret;

  *container = রিক্ত;

  ret = libcrun_get_state_directory (&dir, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = append_paths (&config_file, err, dir, "config.json", রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  *container = libcrun_container_load_from_file (config_file, err);
  যদি (*container == রিক্ত)
    প্রত্যাগম -1;

  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
run_poststop_hooks (arg_unused libcrun_context_t *context, libcrun_container_t *container,
                    runtime_spec_schema_config_schema *def,
                    libcrun_container_status_t *status, ধ্রুব বর্ণ *state_মূল, ধ্রুব বর্ণ *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container_cleanup = রিক্ত;
  পূর্ণাঙ্ক ret;

  যদি (def == রিক্ত)
    {
      যদি (container == রিক্ত)
        {
          ret = read_container_config_from_state (&container_cleanup, state_মূল, id, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
          container = container_cleanup;
        }

      def = container->container_def;
    }

  যদি (def->hooks && def->hooks->poststop_len)
    {
      cleanup_close পূর্ণাঙ্ক hooks_out_fd = -1;
      cleanup_close পূর্ণাঙ্ক hooks_err_fd = -1;

      যদি (container == রিক্ত)
        {
          ret = read_container_config_from_state (&container_cleanup, state_মূল, id, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
          container = container_cleanup;
        }

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      ret = do_hooks (def, 0, id, true, status->bundle, status->bundle, "stopped", "poststop", (hook **) def->hooks->poststop,
                      def->hooks->poststop_len, hooks_out_fd, hooks_err_fd, false, err);
      যদি (UNLIKELY (ret != 0))
        {
          যদি (ret < 0)
            libcrun_error_report_and_release (err);
          অন্যথা
            libcrun_error (0, "poststop hook failed with exit code: %d", ret);
        }
    }
  প্রত্যাগম 0;
}

স্থির bool
has_new_pid_namespace (runtime_spec_schema_config_schema *def)
{
  size_t i;

  যদি (def->linux == রিক্ত || def->linux->namespaces == রিক্ত)
    প্রত্যাগম false;

  প্রতি (i = 0; i < def->linux->namespaces_len; i++)
    {
      যদি (strcmp (def->linux->namespaces[i]->type, "pid") == 0 && def->linux->namespaces[i]->path == রিক্ত)
        প্রত্যাগম true;
    }
  প্রত্যাগম false;
}

/* Delete the container state.  CONTAINER is the container the caller already has
   available, or NULL.  When it is not NULL, the poststop hooks are run even if the
   status file was never written, e.g. when the container creation failed.  */
স্থির পূর্ণাঙ্ক
container_delete_internal (libcrun_context_t *context, libcrun_container_t *container,
                           runtime_spec_schema_config_schema *def, ধ্রুব বর্ণ *id, bool force, bool killall,
                           libcrun_error_t *err)
{
  cleanup_cgroup_status সংরচনা libcrun_cgroup_status *cgroup_status = রিক্ত;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_container libcrun_container_t *container_cleanup = রিক্ত;
  ধ্রুব বর্ণ *state_মূল = context->state_মূল;
  পূর্ণাঙ্ক ret;

  ret = libcrun_read_container_status (&status, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (force && crun_error_get_errno (err) == ENOENT)
        {
          libcrun_error_t tmp_err = রিক্ত;

          crun_error_release (err);

          যদি (container)
            {
              libcrun_container_status_t partial_status = {
                .bundle = (বর্ণ *) context->bundle,
              };

              ret = run_poststop_hooks (context, container, def, &partial_status, state_মূল, id, err);
              যদি (UNLIKELY (ret < 0))
                crun_error_write_warning_and_release (context->output_handler_arg, &err);
            }

          libcrun_container_delete_status (state_মূল, id, &tmp_err);
          crun_error_release (&tmp_err);
          প্রত্যাগম 0;
        }
      crun_error_release (err);
      প্রত্যাগম libcrun_container_delete_status (state_মূল, id, err);
    }

  cgroup_status = libcrun_cgroup_make_status (&status);

  যদি (! force)
    {
      ret = libcrun_is_container_running (&status, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
      যদি (ret == 1)
        {
          ret = libcrun_status_has_read_exec_fifo (state_মূল, id, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
          যদি (ret == 0)
            প্রত্যাগম crun_make_error (err, 0, "the container `%s` is not in `created` or `stopped` state", id);

          /* If the container is in "created" state, then do the equivalent of delete --force.  */
          killall = force = true;
        }
    }

  যদি (def == রিক্ত)
    {
      যদি (container == রিক্ত)
        {
          ret = read_container_config_from_state (&container_cleanup, state_মূল, id, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;

          container = container_cleanup;
        }

      def = container->container_def;
    }

  যদি (killall && force)
    {
      /* If the container has a pid namespace, it is enough to kill the first
         process (pid=1 in the namespace).
      */
      যদি (has_new_pid_namespace (def))
        {
          ret = libcrun_kill_linux (&status, SIGKILL, err);
          যদি (UNLIKELY (ret < 0))
            {
              যদি (crun_error_get_errno (err) != ESRCH)
                প্রত্যাগম ret;

              crun_error_release (err);
            }
        }
      অন্যথা যদি (status.cgroup_path)
        {
          ret = libcrun_cgroup_killall (cgroup_status, SIGKILL, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
        }
    }

  যদি (def->linux && def->linux->intel_rdt)
    {
      ret = libcrun_destroy_intelrdt (id, def, err);
      যদি (UNLIKELY (ret < 0))
        libcrun_error_report_and_release (err);
    }

  যদি (status.cgroup_path)
    {
      /* If the cgroup cannot be destroyed, there are still processes in it.
         Keep the container status, it is the only record of the cgroup, so
         that the delete can be retried.  */
      ret = libcrun_cgroup_destroy (cgroup_status, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  ret = run_poststop_hooks (context, container, def, &status, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  প্রত্যাগম libcrun_container_delete_status (state_মূল, id, err);
}

পূর্ণাঙ্ক
libcrun_container_delete (libcrun_context_t *context, ধ্রুব বর্ণ *id, bool force, libcrun_error_t *err)
{
  প্রত্যাগম container_delete_internal (context, রিক্ত, রিক্ত, id, force, true, err);
}

পূর্ণাঙ্ক
libcrun_container_kill (libcrun_context_t *context, ধ্রুব বর্ণ *id, ধ্রুব বর্ণ *সঙ্কেত, libcrun_error_t *err)
{
  পূর্ণাঙ্ক sig, ret;
  ধ্রুব বর্ণ *state_মূল = context->state_মূল;
  cleanup_container_status libcrun_container_status_t status = {};

  sig = str2sig (সঙ্কেত);
  যদি (UNLIKELY (sig < 0))
    প্রত্যাগম crun_make_error (err, 0, "unknown signal `%s`", সঙ্কেত);

  ret = libcrun_read_container_status (&status, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  প্রত্যাগম libcrun_kill_linux (&status, sig, err);
}

পূর্ণাঙ্ক
libcrun_container_killall (libcrun_context_t *context, ধ্রুব বর্ণ *id, ধ্রুব বর্ণ *সঙ্কেত, libcrun_error_t *err)
{
  পূর্ণাঙ্ক sig, ret;
  ধ্রুব বর্ণ *state_মূল = context->state_মূল;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_cgroup_status সংরচনা libcrun_cgroup_status *cgroup_status = রিক্ত;

  sig = str2sig (সঙ্কেত);
  যদি (UNLIKELY (sig < 0))
    প্রত্যাগম crun_make_error (err, 0, "unknown signal `%s`", সঙ্কেত);

  ret = libcrun_read_container_status (&status, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  cgroup_status = libcrun_cgroup_make_status (&status);

  ret = libcrun_cgroup_killall (cgroup_status, sig, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
write_container_status (libcrun_container_t *container, libcrun_context_t *context,
                        pid_t pid, সংরচনা libcrun_cgroup_status *cgroup_status,
                        libcrun_error_t *err)
{
  cleanup_free বর্ণ *bundle_allocated = রিক্ত;
  ধ্রুব বর্ণ *bundle = get_bundle_path (context->bundle, &bundle_allocated);
  cleanup_free বর্ণ *owner = get_user_name (geteuid ());
  বর্ণ *external_descriptors = libcrun_get_external_descriptors (container);
  বর্ণ *rootfs = container->container_def->মূল ? container->container_def->মূল->path : "";
  বর্ণ created[35];

  libcrun_container_status_t status = {
    .pid = pid,
    .rootfs = rootfs,
    .bundle = (বর্ণ *) bundle,
    .created = created,
    .owner = owner,
    .systemd_cgroup = context->systemd_cgroup,
    .detached = context->detach,
    .no_new_keyring = context->no_new_keyring,
    .external_descriptors = external_descriptors,
    .cgroup_path = রিক্ত,
    .scope = রিক্ত,
  };

  get_current_timestamp (created, পরিমাণ (created));

  যদি (cgroup_status)
    {
      পূর্ণাঙ্ক ret;

      ret = libcrun_cgroup_get_status (cgroup_status, &status, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  যদি (external_descriptors == রিক্ত)
    প্রত্যাগম crun_make_error (err, 0, "invalid internal state.  No external descriptors found");
  প্রত্যাগম libcrun_write_container_status (context->state_মূল, context->id, &status, err);
}

স্থির পূর্ণাঙ্ক
reap_subprocesses (pid_t *main_process, পূর্ণাঙ্ক *main_process_exit, পূর্ণাঙ্ক *last_process, libcrun_error_t *err)
{
  *last_process = 0;
  যাবত্ (1)
    {
      পূর্ণাঙ্ক status;
      পূর্ণাঙ্ক r = waitpid_ignore_stopped (-1, &status, WNOHANG);
      যদি (r < 0)
        {
          যদি (errno == EINTR)
            অনুবর্ত;
          যদি (errno == ECHILD)
            {
              *last_process = 1;
              প্রত্যাগম 0;
            }
          প্রত্যাগম crun_make_error (err, errno, "waitpid");
        }
      যদি (r == 0)
        ভঙ্গ;
      যদি (r != *main_process)
        অনুবর্ত;

      *main_process_exit = get_process_exit_status (status);

      /* Once reaped, the pid can be reused by an unrelated process.  */
      *main_process = 0;
    }
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
send_sd_notify (ধ্রুব বর্ণ *ready_str, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  পূর্ণাঙ্ক ret = sd_notify (0, ready_str);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, -ret, "sd_notify");

#  যদি HAVE_SD_NOTIFY_BARRIER
  /* Hard-code a 30 seconds timeout.  Ignore errors.  */
  sd_notify_barrier (0, 30 * 1000000);
#  endif
#অন্যথা
  (শূন্য) ready_str;
  (শূন্য) err;
#endif
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
handle_notify_socket (পূর্ণাঙ্ক notify_socketfd, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  পূর্ণাঙ্ক ret;
  বর্ণ buf[256];
  ধ্রুব বর্ণ *ready_str = "READY=1";

  ret = recvfrom (notify_socketfd, buf, পরিমাণ (buf) - 1, 0, রিক্ত, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "recvfrom notify socket");

  buf[ret] = '\0';
  যদি (strstr (buf, ready_str))
    {
      ret = send_sd_notify (ready_str, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      প্রত্যাগম 1;
    }
  প্রত্যাগম 0;
#অন্যথা
  (শূন্য) notify_socketfd;
  (শূন্য) err;
  প্রত্যাগম 1;
#endif
}

সংরচনা wait_for_process_args
{
  pid_t pid;
  libcrun_context_t *context;
  পূর্ণাঙ্ক terminal_fd;
  পূর্ণাঙ্ক notify_socket;
  পূর্ণাঙ্ক *container_ready_fd;
  পূর্ণাঙ্ক seccomp_notify_fd;
  ধ্রুব বর্ণ *seccomp_notify_plugins;
};

স্থির পূর্ণাঙ্ক
write_pid_file (ধ্রুব বর্ণ *pid_file, pid_t pid, libcrun_error_t *err)
{
  বর্ণ buf[32];
  পূর্ণাঙ্ক buf_len = snprintf (buf, পরিমাণ (buf), "%d", pid);
  যদি (UNLIKELY (buf_len >= (পূর্ণাঙ্ক) পরিমাণ (buf)))
    প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");
  প্রত্যাগম write_file_at_with_flags (AT_FDCWD, O_CREAT | O_TRUNC, 0700, pid_file, buf, buf_len, err);
}

#define MAX_EVENTS 10

স্থির bool
is_fd_pollable (পূর্ণাঙ্ক fd)
{
  সংরচনা epoll_event ev = { .events = EPOLLIN };
  cleanup_close পূর্ণাঙ্ক epollfd = epoll_create1 (EPOLL_CLOEXEC);

  যদি (epollfd < 0)
    প্রত্যাগম true;

  প্রত্যাগম epoll_ctl (epollfd, EPOLL_CTL_ADD, fd, &ev) == 0 || errno != EPERM;
}

স্থির পূর্ণাঙ্ক
wait_for_process (সংরচনা wait_for_process_args *args, libcrun_error_t *err)
{
  cleanup_channel_fd_pair সংরচনা channel_fd_pair *from_terminal = রিক্ত;
  cleanup_channel_fd_pair সংরচনা channel_fd_pair *to_terminal = রিক্ত;
  পূর্ণাঙ্ক ret, container_exit_code = 0, last_process;
  cleanup_close পূর্ণাঙ্ক terminal_fd_from = -1;
  cleanup_close পূর্ণাঙ্ক terminal_fd_to = -1;
  bool stdin_pollable = false, stdout_pollable = false;
  cleanup_close পূর্ণাঙ্ক epollfd = -1;
  cleanup_close পূর্ণাঙ্ক signalfd = -1;
  sigset_t mask;
  পূর্ণাঙ্ক in_fds[MAX_EVENTS];
  পূর্ণাঙ্ক in_fds_len = 0;
  পূর্ণাঙ্ক out_fds[MAX_EVENTS];
  পূর্ণাঙ্ক out_fds_len = 0;
  size_t i;

  cleanup_seccomp_notify_context সংরচনা seccomp_notify_context_s *seccomp_notify_ctx = রিক্ত;

  container_exit_code = 0;

  যদি (args == রিক্ত || args->context == রিক্ত)
    প্রত্যাগম crun_make_error (err, 0, "internal error: context is empty");

  প্রতি (i = 0; i < MAX_EVENTS; i++)
    {
      in_fds[i] = -1;
      out_fds[i] = -1;
    }

  যদি (args->context->pid_file)
    {
      ret = write_pid_file (args->context->pid_file, args->pid, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  /* Also exit if there is nothing more to wait for.  */
  যদি (args->context->detach && args->notify_socket < 0)
    প্রত্যাগম 0;

  যদি (args->container_ready_fd)
    {
      ret = 0;
      TEMP_FAILURE_RETRY (লিখ (*args->container_ready_fd, &ret, পরিমাণ (ret)));
      close_and_reset (args->container_ready_fd);
    }

  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "sigprocmask");

  signalfd = create_signalfd (&mask, err);
  যদি (UNLIKELY (signalfd < 0))
    প্রত্যাগম signalfd;

  ret = reap_subprocesses (&args->pid, &container_exit_code, &last_process, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (last_process)
    প্রত্যাগম container_exit_code;

  যদি (args->seccomp_notify_fd >= 0)
    {
      cleanup_free বর্ণ *state_মূল = রিক্ত;
      cleanup_free বর্ণ *oci_config_path = রিক্ত;

      সংরচনা libcrun_load_seccomp_notify_conf_s conf;
      memset (&conf, 0, পরিমাণ conf);

      ret = libcrun_get_state_directory (&state_মূল, args->context->state_মূল, args->context->id, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      ret = append_paths (&oci_config_path, err, state_মূল, "config.json", রিক্ত);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      conf.runtime_মূল_path = state_মূল;
      conf.name = args->context->id;
      conf.bundle_path = args->context->bundle;
      conf.oci_config_path = oci_config_path;

      ret = set_blocking_fd (args->seccomp_notify_fd, false, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      ret = libcrun_load_seccomp_notify_plugins (&seccomp_notify_ctx,
                                                 args->seccomp_notify_plugins,
                                                 &conf, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      in_fds[in_fds_len++] = args->seccomp_notify_fd;
    }

  যদি (args->terminal_fd >= 0)
    {
      /* The terminal_fd is dup()ed so that it can be registered with
         epoll multiple times using different masks.  */
      terminal_fd_from = dup (args->terminal_fd);
      যদি (UNLIKELY (terminal_fd_from < 0))
        প্রত্যাগম crun_make_error (err, errno, "dup terminal fd");
      terminal_fd_to = dup (args->terminal_fd);
      যদি (UNLIKELY (terminal_fd_to < 0))
        প্রত্যাগম crun_make_error (err, errno, "dup terminal fd");

      পূর্ণাঙ্ক i, non_blocking_fds[] = { terminal_fd_from, terminal_fd_to, 0, 1, -1 };
      প্রতি (i = 0; non_blocking_fds[i] >= 0; i++)
        {
          ret = set_blocking_fd (non_blocking_fds[i], false, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
        }

      /* stdin and stdout can be something epoll does not support, such as
         /dev/null or a regular file.  Such a stdin is not forwarded, and
         such a stdout is written to without waiting for it to be ready.  */
      stdin_pollable = is_fd_pollable (0);
      stdout_pollable = is_fd_pollable (1);

      from_terminal = channel_fd_pair_new (terminal_fd_from, 1, BUFSIZ);
      যদি (stdin_pollable)
        to_terminal = channel_fd_pair_new (0, terminal_fd_to, BUFSIZ);
    }

  in_fds[in_fds_len++] = signalfd;
  যদি (args->notify_socket >= 0)
    in_fds[in_fds_len++] = args->notify_socket;
  যদি (args->terminal_fd >= 0)
    {
      যদি (stdin_pollable)
        {
          in_fds[in_fds_len++] = 0;
          out_fds[out_fds_len++] = terminal_fd_to;
        }

      in_fds[in_fds_len++] = terminal_fd_from;
      যদি (stdout_pollable)
        out_fds[out_fds_len++] = 1;
    }

  epollfd = epoll_helper (in_fds, রিক্ত, out_fds, রিক্ত, err);
  যদি (UNLIKELY (epollfd < 0))
    প্রত্যাগম epollfd;

  যাবত্ (1)
    {
      সংরচনা epoll_event events[MAX_EVENTS];
      সংরচনা signalfd_siginfo si;
      সংরচনা winsize ws;
      পূর্ণাঙ্ক i, nr_events;
      ssize_t res;

      nr_events = TEMP_FAILURE_RETRY (epoll_wait (epollfd, events, MAX_EVENTS, -1));
      যদি (UNLIKELY (nr_events < 0))
        প্রত্যাগম crun_make_error (err, errno, "epoll_wait");

      প্রতি (i = 0; i < nr_events; i++)
        {
          যদি (events[i].data.fd == 0 || events[i].data.fd == terminal_fd_to)
            {
              ret = channel_fd_pair_process (to_terminal, epollfd, err);
              যদি (UNLIKELY (ret < 0))
                প্রত্যাগম crun_error_wrap (err, "copy to terminal fd");
            }
          অন্যথা যদি (events[i].data.fd == 1 || events[i].data.fd == terminal_fd_from)
            {
              ret = channel_fd_pair_process (from_terminal, epollfd, err);
              যদি (UNLIKELY (ret < 0))
                প্রত্যাগম crun_error_wrap (err, "copy from terminal fd");
            }
          অন্যথা যদি (events[i].data.fd == args->seccomp_notify_fd)
            {
              ret = libcrun_seccomp_notify_plugins (seccomp_notify_ctx,
                                                    args->seccomp_notify_fd, err);
              যদি (UNLIKELY (ret < 0))
                প্রত্যাগম ret;
            }
          অন্যথা যদি (events[i].data.fd == args->notify_socket)
            {
              ret = handle_notify_socket (args->notify_socket, err);
              যদি (UNLIKELY (ret < 0))
                প্রত্যাগম ret;
              যদি (ret && args->context->detach)
                প্রত্যাগম 0;
            }
          অন্যথা যদি (events[i].data.fd == signalfd)
            {
              res = TEMP_FAILURE_RETRY (পঠ (signalfd, &si, পরিমাণ (si)));
              যদি (UNLIKELY (res < 0))
                প্রত্যাগম crun_make_error (err, errno, "read from signalfd");
              যদি (si.ssi_signo == SIGCHLD)
                {
                  ret = reap_subprocesses (&args->pid, &container_exit_code,
                                           &last_process, err);
                  যদি (UNLIKELY (ret < 0))
                    প্রত্যাগম ret;
                  যদি (last_process)
                    প্রত্যাগম container_exit_code;
                }
              অন্যথা যদি (si.ssi_signo == SIGWINCH)
                {
                  /* Ignore the signal if the terminal is not available.  */
                  যদি (args->terminal_fd > 0)
                    {
                      ret = ioctl (0, TIOCGWINSZ, &ws);
                      যদি (UNLIKELY (ret < 0))
                        প্রত্যাগম crun_make_error (err, errno, "ioctl TIOCGWINSZ copy terminal size from stdin");

                      ret = ioctl (args->terminal_fd, TIOCSWINSZ, &ws);
                      যদি (UNLIKELY (ret < 0))
                        প্রত্যাগম crun_make_error (err, errno, "ioctl TIOCSWINSZ copy terminal size to pty");
                    }
                }
              অন্যথা যদি (args->pid > 0)
                {
                  /* Send any other signal to the child process, if it was not reaped yet.  */
                  ret = kill (args->pid, si.ssi_signo);
                }
            }
          অন্যথা
            {
              প্রত্যাগম crun_make_error (err, 0, "internal error: unknown fd from epoll_wait");
            }
        }
    }

  প্রত্যাগম 0;
}

স্থির শূন্য
flush_fd_to_err (libcrun_context_t *context, পূর্ণাঙ্ক terminal_fd)
{
  বর্ণ buf[256];
  পূর্ণাঙ্ক flags;

  যদি (terminal_fd < 0 || stderr == রিক্ত)
    প্রত্যাগম;

  flags = fcntl (terminal_fd, F_GETFL, 0);
  যদি (flags == -1)
    প্রত্যাগম;
  যদি (fcntl (terminal_fd, F_SETFL, flags | O_NONBLOCK) < 0)
    প্রত্যাগম;

  প্রতি (;;)
    {
      পূর্ণাঙ্ক ret = TEMP_FAILURE_RETRY (পঠ (terminal_fd, buf, পরিমাণ (buf) - 1));
      যদি (ret <= 0)
        ভঙ্গ;
      buf[ret] = '\0';
      যদি (context->output_handler)
        context->output_handler (0, buf, LIBCRUN_VERBOSITY_ERROR, context->output_handler_arg);
    }
  (শূন্য) fcntl (terminal_fd, F_SETFL, flags);
  fflush (stderr);
  fsync (1);
  fsync (2);
}

স্থির পূর্ণাঙ্ক
cleanup_watch (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
               সংরচনা libcrun_cgroup_status *cgroup_status, pid_t init_pid, পূর্ণাঙ্ক sync_socket,
               পূর্ণাঙ্ক terminal_fd, libcrun_error_t *err)
{
  ধ্রুব বর্ণ *oom_message = রিক্ত;
  libcrun_error_t tmp_err = রিক্ত;
  পূর্ণাঙ্ক ret;

  যদি (init_pid)
    {
      /* Try to detect whether the cgroup has a OOM.  */
      যদি (cgroup_status)
        {
          পূর্ণাঙ্ক has_oom;

          has_oom = libcrun_cgroup_has_oom (cgroup_status, &tmp_err);
          যদি (has_oom > 0)
            oom_message = "OOM: the memory limit could be too low";
          অন্যথা যদি (has_oom < 0)
            {
              /* If the detection has failed for any reason, e.g. the cgroup was
                 already deleted by the time it was checked, just ignore the
                 failure.  */
              crun_error_release (&tmp_err);
            }
        }
      /* If the OOM wasn't detected, look into the static configuration.  */
      যদি (oom_message == রিক্ত && is_memory_limit_too_নিচের (def))
        oom_message = "the memory limit could be too low";

      kill (init_pid, SIGKILL);
      waitpid_ignore_stopped (init_pid, রিক্ত, 0);
    }

  ret = sync_socket_wait_sync (context, sync_socket, true, &tmp_err);
  যদি (UNLIKELY (ret < 0))
    {
      crun_error_release (err);
      *err = tmp_err;
    }

  যদি (terminal_fd >= 0)
    flush_fd_to_err (context, terminal_fd);

  যদি (oom_message)
    প্রত্যাগম crun_error_wrap (err, "%s", oom_message);

  প্রত্যাগম -1;
}

/* Find the uid:gid that is mapped to root inside the container user namespace.  */
শূন্য
get_মূল_in_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid, uid_t *uid,
                        gid_t *gid)
{
  *uid = -1;
  *gid = -1;

  /* Not root in the namespace.  */
  যদি (host_uid)
    প্রত্যাগম;

  যদি (def->linux && def->linux->uid_mappings)
    {
      size_t i;

      প্রতি (i = 0; i < def->linux->uid_mappings_len; i++)
        যদি (def->linux->uid_mappings[i]->container_id == 0)
          {
            *uid = def->linux->uid_mappings[i]->host_id;
            ভঙ্গ;
          }
    }
  যদি (def->linux && def->linux->gid_mappings)
    {
      size_t i;

      প্রতি (i = 0; i < def->linux->gid_mappings_len; i++)
        যদি (def->linux->gid_mappings[i]->container_id == 0)
          {
            *gid = def->linux->gid_mappings[i]->host_id;
            ভঙ্গ;
          }
    }

  /* If the uid and the gid are not changed, do not attempt any chown.  */
  যদি (*uid == host_uid && *gid == host_gid)
    *uid = *gid = -1;
}

স্থির পূর্ণাঙ্ক
get_seccomp_receiver_fd (libcrun_container_t *container, পূর্ণাঙ্ক *fd, পূর্ণাঙ্ক *self_receiver_fd, ধ্রুব বর্ণ **plugins,
                         libcrun_error_t *err)
{
  ধ্রুব বর্ণ *tmp;
  runtime_spec_schema_config_schema *def = container->container_def;

  *fd = -1;
  *self_receiver_fd = -1;

  tmp = find_annotation (container, "run.oci.seccomp.plugins");
  যদি (tmp)
    {
      পূর্ণাঙ্ক fds[2];
      পূর্ণাঙ্ক ret;

      ret = create_socket_pair (fds, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম crun_error_wrap (err, "create socket pair");

      *fd = fds[0];
      *self_receiver_fd = fds[1];
      *plugins = tmp;
      প্রত্যাগম 0;
    }

  যদি (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    tmp = def->linux->seccomp->listener_path;
  অন্যথা
    tmp = find_annotation (container, "run.oci.seccomp.receiver");
  যদি (tmp == রিক্ত)
    tmp = getenv ("RUN_OCI_SECCOMP_RECEIVER");
  যদি (tmp)
    {
      যদি (tmp[0] != '/')
        প্রত্যাগম crun_make_error (err, 0, "the seccomp receiver `%s` is not an absolute path", tmp);

      *fd = open_unix_domain_client_socket (tmp, 0, err);
      যদি (UNLIKELY (*fd < 0))
        প্রত্যাগম crun_error_wrap (err, "open seccomp receiver");
    }

  প্রত্যাগম 0;
}

স্থির bool
has_seccomp_receiver (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;

  যদি (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    প্রত্যাগম true;

  যদি (find_annotation (container, "run.oci.seccomp.receiver") != রিক্ত || getenv ("RUN_OCI_SECCOMP_RECEIVER") != রিক্ত)
    প্রত্যাগম true;

  প্রত্যাগম false;
}

স্থির পূর্ণাঙ্ক
setup_container_hooks_output (libcrun_container_t *container, runtime_spec_schema_config_schema *def,
                              সংরচনা container_entrypoint_s *container_args, পূর্ণাঙ্ক *hooks_out_fd,
                              পূর্ণাঙ্ক *hooks_err_fd, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  যদি (def->hooks
      && (def->hooks->prestart_len || def->hooks->poststart_len || def->hooks->create_runtime_len
          || def->hooks->create_container_len || def->hooks->start_container_len))
    {
      ret = open_hooks_output (container, hooks_out_fd, hooks_err_fd, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
      container_args->hooks_out_fd = *hooks_out_fd;
      container_args->hooks_err_fd = *hooks_err_fd;
    }
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
setup_container_keyring (libcrun_container_t *container, libcrun_context_t *context, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  পূর্ণাঙ্ক ret;

  যদি (! context->no_new_keyring)
    {
      ধ্রুব বর্ণ *label = রিক্ত;

      libcrun_debug ("Creating new keyring");

      যদি (def->process)
        {
          label = def->process->selinux_label;
          যদি (label)
            libcrun_debug ("Using SELinux process label: `%s`", label);
        }

      ret = libcrun_create_keyring (container, container->context->id, label, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
setup_terminal_socket_pair (libcrun_container_t *container, libcrun_context_t *context,
                            সংরচনা container_entrypoint_s *container_args, পূর্ণাঙ্ক *socket_pair_0,
                            পূর্ণাঙ্ক *socket_pair_1, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  পূর্ণাঙ্ক ret;
  পূর্ণাঙ্ক detach = context->detach;

  যদি (def->process && def->process->terminal && ! detach && context->console_socket == রিক্ত)
    {
      libcrun_debug ("Creating terminal socket pair");
      container_args->has_terminal_socket_pair = 1;
      ret = create_socket_pair (container_args->terminal_socketpair, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম crun_error_wrap (err, "create terminal socket");

      *socket_pair_0 = container_args->terminal_socketpair[0];
      *socket_pair_1 = container_args->terminal_socketpair[1];
    }
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
setup_seccomp (libcrun_container_t *container, ধ্রুব বর্ণ *seccomp_bpf_data,
               সংরচনা libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, পূর্ণাঙ্ক *seccomp_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  পূর্ণাঙ্ক ret;

  যদি (find_annotation (container, "run.oci.seccomp.plugins") != রিক্ত && has_seccomp_receiver (container))
    {
      প্রত্যাগম crun_make_error (err, 0, "seccomp plugins and seccomp receivers cannot be declared at the same time");
    }

  যদি (def->linux && (def->linux->seccomp || seccomp_bpf_data))
    {
      unsigned পূর্ণাঙ্ক seccomp_gen_options = 0;
      ধ্রুব বর্ণ *annotation;

      libcrun_debug ("Initializing seccomp");
      annotation = find_annotation (container, "run.oci.seccomp_fail_unknown_syscall");
      যদি (annotation && strcmp (annotation, "0") != 0)
        seccomp_gen_options = LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL;

      যদি (seccomp_bpf_data)
        seccomp_gen_options |= LIBCRUN_SECCOMP_SKIP_CACHE;

      libcrun_seccomp_gen_ctx_init (seccomp_gen_ctx, container, true, seccomp_gen_options);

      ret = libcrun_open_seccomp_bpf (seccomp_gen_ctx, seccomp_fd, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
setup_console_socket (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
                      সংরচনা container_entrypoint_s *container_args, পূর্ণাঙ্ক *console_socket_fd,
                      libcrun_error_t *err)
{
  যদি (context->console_socket && def->process && def->process->terminal)
    {
      *console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
      যদি (UNLIKELY (*console_socket_fd < 0))
        প্রত্যাগম crun_error_wrap (err, "open console socket");
      container_args->console_socket_fd = *console_socket_fd;
    }
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
setup_cgroup_manager (libcrun_context_t *context, libcrun_container_t *container,
                      সংরচনা libcrun_cgroup_args *cg, পূর্ণাঙ্ক *cgroup_dirfd,
                      সংরচনা libcrun_dirfd_s *cgroup_dirfd_s, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  পূর্ণাঙ্ক cgroup_manager;
  uid_t মূল_uid = -1;
  gid_t মূল_gid = -1;
  পূর্ণাঙ্ক ret;

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  যদি (context->systemd_cgroup)
    {
      libcrun_debug ("Using systemd cgroup manager");
      cgroup_manager = CGROUP_MANAGER_SYSTEMD;
    }
  অন্যথা যদি (context->force_no_cgroup)
    {
      libcrun_debug ("Disabling cgroup manager");
      cgroup_manager = CGROUP_MANAGER_DISABLED;
    }
  অন্যথা
    libcrun_debug ("Using cgroupfs cgroup manager");

  /* If we are root (either on the host or in a namespace), then chown the cgroup to root
     in the container user namespace.  */
  get_মূল_in_the_userns (def, container->host_uid, container->host_gid, &মূল_uid, &মূল_gid);
  libcrun_debug ("Using container host UID `%d` and GID `%d`", container->host_uid, container->host_gid);

  memset (cg, 0, পরিমাণ (*cg));

  cg->cgroup_path = def->linux ? def->linux->cgroups_path : "";
  cg->manager = cgroup_manager;
  cg->id = context->id;
  cg->resources = def->linux ? def->linux->resources : রিক্ত;
  cg->annotations = container->annotations;
  cg->মূল_uid = মূল_uid;
  cg->মূল_gid = মূল_gid;
  cg->state_মূল = context->state_মূল;
  cg->container = container;

  ret = libcrun_cgroup_preenter (cg, cgroup_dirfd, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  cgroup_dirfd_s->dirfd = cgroup_dirfd;
  cgroup_dirfd_s->joined = false;

  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
set_scheduler (pid_t pid, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  ret = libcrun_set_scheduler (pid, def->process, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = libcrun_reset_cpu_affinity_mask (pid, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = libcrun_set_io_priority (pid, def->process, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
seccomp_generation (পূর্ণাঙ্ক seccomp_fd, ধ্রুব বর্ণ *seccomp_bpf_data,
                    সংরচনা libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  যদি (seccomp_fd >= 0)
    {
      যদি (seccomp_bpf_data != রিক্ত)
        {
          ret = libcrun_copy_seccomp (seccomp_gen_ctx, seccomp_bpf_data, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
        }
      অন্যথা
        {
          ret = libcrun_generate_seccomp (seccomp_gen_ctx, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
        }
    }
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
terminal_setup (runtime_spec_schema_config_schema *def, libcrun_context_t *context,
                পূর্ণাঙ্ক socket_pair_0, পূর্ণাঙ্ক *terminal_fd, শূন্য **orig_terminal, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  পূর্ণাঙ্ক detach = context->detach;

  যদি (def->process && def->process->terminal && ! detach && context->console_socket == রিক্ত)
    {
      libcrun_debug ("Receiving console socket fd");
      *terminal_fd = receive_fd_from_socket (socket_pair_0, err);
      যদি (UNLIKELY (*terminal_fd < 0))
        প্রত্যাগম -1;

      ret = libcrun_clear_onlcr (*terminal_fd, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      ret = libcrun_set_raw (0, orig_terminal, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
libcrun_container_run_internal (libcrun_container_t *container, libcrun_context_t *context,
                                পূর্ণাঙ্ক *container_ready_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  পূর্ণাঙ্ক ret;
  pid_t pid;
  পূর্ণাঙ্ক detach = context->detach;
  cleanup_cgroup_status সংরচনা libcrun_cgroup_status *cgroup_status = রিক্ত;
  cleanup_close পূর্ণাঙ্ক terminal_fd = -1;
  cleanup_terminal শূন্য *orig_terminal = রিক্ত;
  cleanup_close পূর্ণাঙ্ক sync_socket = -1;
  cleanup_close পূর্ণাঙ্ক notify_socket = -1;
  cleanup_close পূর্ণাঙ্ক socket_pair_0 = -1;
  cleanup_close পূর্ণাঙ্ক socket_pair_1 = -1;
  cleanup_close পূর্ণাঙ্ক seccomp_fd = -1;
  cleanup_close পূর্ণাঙ্ক console_socket_fd = -1;
  cleanup_close পূর্ণাঙ্ক hooks_out_fd = -1;
  cleanup_close পূর্ণাঙ্ক hooks_err_fd = -1;
  cleanup_close পূর্ণাঙ্ক own_seccomp_receiver_fd = -1;
  cleanup_close পূর্ণাঙ্ক seccomp_notify_fd = -1;
  ধ্রুব বর্ণ *seccomp_notify_plugins = রিক্ত;
  cleanup_cgroup_preenter সংরচনা libcrun_cgroup_args cg = {};
  cleanup_free বর্ণ *bundle_allocated = রিক্ত;
  সংরচনা container_entrypoint_s container_args = {
    .container = container,
    .context = context,
    .bundle = get_bundle_path (context->bundle, &bundle_allocated),
    .terminal_socketpair = { -1, -1 },
    .console_socket_fd = -1,
    .hooks_out_fd = -1,
    .hooks_err_fd = -1,
    .seccomp_receiver_fd = -1,
    .custom_handler = রিক্ত,
  };
  cleanup_close পূর্ণাঙ্ক cgroup_dirfd = -1;
  সংরচনা libcrun_dirfd_s cgroup_dirfd_s;
  সংরচনা libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  ধ্রুব বর্ণ *seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");
  পূর্ণাঙ্ক cgroup_mode;

  যদি (! context->force_no_cgroup)
    {
      cgroup_mode = libcrun_get_cgroup_mode (err);
      যদি (UNLIKELY (cgroup_mode < 0))
        প্রত্যাগম cgroup_mode;

      যদি (cgroup_mode != CGROUP_MODE_UNIFIED)
        libcrun_warning ("cgroup v1 is deprecated and will be removed in a future release.  Use cgroup v2");
    }

  ret = setup_container_hooks_output (container, def, &container_args, &hooks_out_fd, &hooks_err_fd, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  container->context = context;

  যদি (! detach || context->notify_socket)
    {
      libcrun_debug ("Setting child subreaper");
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম crun_make_error (err, errno, "prctl set child subreaper");
    }

  ret = setup_container_keyring (container, context, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = setup_terminal_socket_pair (container, context, &container_args, &socket_pair_0, &socket_pair_1, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = block_signals (err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  umask (0);

  ret = libcrun_set_mempolicy (def, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;
  container_args.seccomp_fd = seccomp_fd;

  যদি (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &container_args.seccomp_receiver_fd, &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  ret = setup_console_socket (context, def, &container_args, &console_socket_fd, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = setup_cgroup_manager (context, container, &cg, &cgroup_dirfd, &cgroup_dirfd_s, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = libcrun_configure_handler (container_args.context->handler_manager,
                                   container_args.context,
                                   container,
                                   &(container_args.custom_handler),
                                   err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (container_args.custom_handler && container_args.custom_handler->vtable->modify_oci_configuration)
    {
      libcrun_debug ("Using custom handler to modify OCI configuration");
      ret = container_args.custom_handler->vtable->modify_oci_configuration (container_args.custom_handler->cookie,
                                                                             container_args.context,
                                                                             container->container_def,
                                                                             err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  pid = libcrun_run_linux_container (container, container_init, &container_args, &sync_socket, &cgroup_dirfd_s, err);
  যদি (UNLIKELY (pid < 0))
    প্রত্যাগম pid;

  cg.pid = pid;
  cg.joined = cgroup_dirfd_s.joined;
  libcrun_debug ("Running container on PID: `%d`", pid);

  যদি (context->fifo_exec_wait_fd < 0 && context->notify_socket)
    {
      libcrun_debug ("Using notify socket: `%s`", context->notify_socket);
      /* Do not open the notify socket here on "create".  "start" will take care of it.  */
      ret = get_notify_fd (context, container, &notify_socket, err);
      যদি (UNLIKELY (ret < 0))
        গচ্ছ fail;
    }

  যদি (container_args.terminal_socketpair[1] >= 0)
    close_and_reset (&socket_pair_1);

  ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ fail;

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ fail;

  ret = libcrun_move_network_devices (container, pid, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ fail;

  /* sync send own pid.  */
  ret = TEMP_FAILURE_RETRY (লিখ (sync_socket, &pid, পরিমাণ (pid)));
  যদি (UNLIKELY (ret != পরিমাণ (pid)))
    {
      যদি (ret >= 0)
        errno = 0;
      crun_make_error (err, errno, "write to sync socket");
      গচ্ছ fail;
    }

  /* sync 1.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ fail;

  /* sync 2.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ fail;

  ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ fail;

  /* If the container process is created later, the scheduler is set on it then.  */
  যদি (! libcrun_container_delayed_pidns_fork (container))
    {
      ret = set_scheduler (pid, def, err);
      যদি (UNLIKELY (ret < 0))
        গচ্ছ fail;
    }

  /* The container is waiting that we write back.  In this phase we can launch the
     prestart hooks.  */
  যদি (def->hooks && def->hooks->prestart_len)
    {
      libcrun_debug ("Running `prestart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "prestart", (hook **) def->hooks->prestart,
                      def->hooks->prestart_len, hooks_out_fd, hooks_err_fd, false, err);
      যদি (UNLIKELY (ret != 0))
        গচ্ছ fail;
    }
  যদি (def->hooks && def->hooks->create_runtime_len)
    {
      libcrun_debug ("Running `create` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "createRuntime", (hook **) def->hooks->create_runtime,
                      def->hooks->create_runtime_len, hooks_out_fd, hooks_err_fd, false, err);
      যদি (UNLIKELY (ret != 0))
        গচ্ছ fail;
    }

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ fail;
  close_and_reset (&seccomp_fd);

  /* sync 3.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ fail;

  /* The process joining an existing PID namespace is created once the container root is in place.  */
  যদি (libcrun_container_delayed_pidns_fork (container))
    {
      pid_t new_pid = 0;

      ret = sync_socket_wait_sync (context, sync_socket, false, err);
      যদি (UNLIKELY (ret < 0))
        গচ্ছ fail;

      ret = TEMP_FAILURE_RETRY (পঠ (sync_socket, &new_pid, পরিমাণ (new_pid)));
      যদি (UNLIKELY (ret != পরিমাণ (new_pid)))
        {
          যদি (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "read pid from sync socket");
          গচ্ছ fail;
        }

      /* Cleanup the first process.  */
      waitpid_ignore_stopped (pid, রিক্ত, 0);

      pid = new_pid;

      ret = set_scheduler (pid, def, err);
      যদি (UNLIKELY (ret < 0))
        গচ্ছ fail;

      ret = libcrun_handle_pidfd_receiver (pid, container, err);
      যদি (UNLIKELY (ret < 0))
        গচ্ছ fail;

      /* sync send own pid.  */
      ret = TEMP_FAILURE_RETRY (লিখ (sync_socket, &pid, পরিমাণ (pid)));
      যদি (UNLIKELY (ret != পরিমাণ (pid)))
        {
          যদি (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "write to sync socket");
          গচ্ছ fail;
        }
    }

  ret = terminal_setup (def, context, socket_pair_0, &terminal_fd, &orig_terminal, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ fail;

  close_and_reset (&socket_pair_0);

  /* sync 4.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ fail;

  ret = close_and_reset (&sync_socket);
  যদি (UNLIKELY (ret < 0))
    {
      crun_make_error (err, errno, "close sync_socket failed");
      গচ্ছ fail;
    }

  libcrun_debug ("Writing container status");
  ret = write_container_status (container, context, pid, cgroup_status, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ fail;

  /* Run poststart hooks here only if the container is created using "run".  For create+start, the
     hooks will be executed as part of the start command.  */
  যদি (context->fifo_exec_wait_fd < 0 && def->hooks && def->hooks->poststart_len)
    {
      libcrun_debug ("Running `poststart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      যদি (UNLIKELY (ret != 0))
        গচ্ছ fail;
    }

  /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
  যদি (own_seccomp_receiver_fd >= 0)
    {
      libcrun_debug ("Receiving seccomp fd");
      seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
      যদি (UNLIKELY (seccomp_notify_fd < 0))
        গচ্ছ fail;

      ret = close_and_reset (&own_seccomp_receiver_fd);
      যদি (UNLIKELY (ret < 0))
        {
          crun_make_error (err, errno, "close seccomp receiver fd failed");
          গচ্ছ fail;
        }
    }

  {
    সংরচনা wait_for_process_args args = {
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
  যদি (! context->detach)
    {
      libcrun_error_t tmp_err = রিক্ত;
      cleanup_watch (context, def, cgroup_status, 0, sync_socket, terminal_fd, &tmp_err);
      crun_error_release (&tmp_err);
    }

  প্রত্যাগম ret;

fail:
  ret = cleanup_watch (context, def, cgroup_status, pid, sync_socket, terminal_fd, err);
  যদি (cgroup_status)
    {
      libcrun_error_t tmp_err = রিক্ত;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  প্রত্যাগম ret;
}

স্থির পূর্ণাঙ্ক
check_config_file (runtime_spec_schema_config_schema *def, libcrun_context_t *context, libcrun_error_t *err)
{
  যদি (UNLIKELY (def->linux == রিক্ত))
    প্রত্যাগম crun_make_error (err, 0, "invalid config file, no `linux` block specified");

  যদি (context->handler == রিক্ত)
    {
      যদি (UNLIKELY (def->মূল == রিক্ত))
        প্রত্যাগম crun_make_error (err, 0, "invalid config file, no `root` block specified");
      যদি (UNLIKELY (def->mounts == রিক্ত))
        প্রত্যাগম crun_make_error (err, 0, "invalid config file, no `mounts` block specified");
    }
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
libcrun_copy_config_file (ধ্রুব বর্ণ *id, ধ্রুব বর্ণ *state_মূল, libcrun_container_t *container, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  cleanup_free বর্ণ *dest_path = রিক্ত;
  cleanup_free বর্ণ *dir = রিক্ত;
  cleanup_free বর্ণ *buffer = রিক্ত;
  size_t len;

  ret = libcrun_get_state_directory (&dir, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = append_paths (&dest_path, err, dir, "config.json", রিক্ত);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (container->config_file == রিক্ত && container->config_file_content == রিক্ত)
    প্রত্যাগম crun_make_error (err, 0, "config file not specified");

  যদি (container->config_file == রিক্ত)
    {
      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, container->config_file_content, strlen (container->config_file_content), err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }
  অন্যথা
    {
      libcrun_debug ("Reading config file: `%s`", container->config_file);
      ret = read_all_file (container->config_file, &buffer, &len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, buffer, len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  প্রত্যাগম 0;
}

স্থির শূন্য
force_delete_container_status (libcrun_context_t *context, libcrun_container_t *container,
                               runtime_spec_schema_config_schema *def)
{
  libcrun_error_t tmp_err = রিক্ত;
  container_delete_internal (context, container, def, context->id, true, false, &tmp_err);
  libcrun_error_report_and_release (&tmp_err);
}

/* Serialize ERR to FD as an int errno followed by a NUL-terminated message.
   Used by forked children to report a failure to the parent process.  */
স্থির শূন্য
write_error_to_pipe (পূর্ণাঙ্ক fd, libcrun_error_t *err)
{
  ধ্রুব বর্ণ *বার্তা = (*err)->বার্তা;
  পূর্ণাঙ্ক errcode = crun_error_get_errno (err);

  TEMP_FAILURE_RETRY (লিখ (fd, &errcode, পরিমাণ (errcode)));
  TEMP_FAILURE_RETRY (লিখ (fd, বার্তা, strlen (বার্তা) + 1));
}

পূর্ণাঙ্ক
libcrun_container_run (libcrun_context_t *context, libcrun_container_t *container, unsigned পূর্ণাঙ্ক options,
                       libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  পূর্ণাঙ্ক ret;
  পূর্ণাঙ্ক detach = context->detach;
  পূর্ণাঙ্ক container_ret_status[2];
  cleanup_close পূর্ণাঙ্ক pipefd0 = -1;
  cleanup_close পূর্ণাঙ্ক pipefd1 = -1;
  libcrun_error_t tmp_err = রিক্ত;

  container->context = context;

  ret = validate_options (options, LIBCRUN_RUN_OPTIONS_PREFORK | LIBCRUN_RUN_OPTIONS_KEEP, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = check_config_file (def, context, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (def->process && def->process->terminal && detach && context->console_socket == রিক্ত)
    প্রত্যাগম crun_make_error (err, 0, "use --console-socket with --detach when a terminal is used");

  ret = libcrun_status_check_directories (context->state_মূল, context->id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (! detach && (options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      ret = libcrun_copy_config_file (context->id, context->state_মূল, container, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      ret = libcrun_container_run_internal (container, context, রিক্ত, err);
      যদি (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
        force_delete_container_status (context, container, def);
      প্রত্যাগম ret;
    }

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  ret = fork ();
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "fork");
  যদি (ret)
    {
      পূর্ণাঙ্ক status;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, রিক্ত, 0);

      ret = TEMP_FAILURE_RETRY (পঠ (pipefd0, &status, পরিমাণ (status)));
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম crun_make_error (err, errno, "read from sync pipe");

      যদি (status < 0)
        {
          পূর্ণাঙ্ক errno_;
          বর্ণ buf[512];
          ret = TEMP_FAILURE_RETRY (পঠ (pipefd0, &errno_, পরিমাণ (errno_)));
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম crun_make_error (err, errno, "read from sync pipe");

          ret = TEMP_FAILURE_RETRY (পঠ (pipefd0, buf, পরিমাণ (buf) - 1));
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম crun_make_error (err, errno, "read from sync pipe");
          buf[ret] = '\0';

          প্রত্যাগম crun_make_error (err, errno_, "%s", buf);
        }

      প্রত্যাগম status;
    }

  close_and_reset (&pipefd0);

  /* forked process.  */
  ret = detach_process ();
  যদি (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_মূল, container, &tmp_err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ fail;

  ret = libcrun_container_run_internal (container, context, রিক্ত, &tmp_err);
  TEMP_FAILURE_RETRY (লিখ (pipefd1, &ret, পরিমাণ (ret)));
  যদি (UNLIKELY (ret < 0))
    গচ্ছ fail;

  exit (EXIT_SUCCESS);
fail:

  যদি (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
    force_delete_container_status (context, container, def);
  যদি (tmp_err)
    {
      write_error_to_pipe (pipefd1, &tmp_err);
      crun_error_release (&tmp_err);
    }

  exit (EXIT_FAILURE);
}

পূর্ণাঙ্ক
libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container, unsigned পূর্ণাঙ্ক options,
                          libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  পূর্ণাঙ্ক ret;
  পূর্ণাঙ্ক container_ready_pipe[2];
  cleanup_close পূর্ণাঙ্ক pipefd0 = -1;
  cleanup_close পূর্ণাঙ্ক pipefd1 = -1;
  cleanup_close পূর্ণাঙ্ক exec_fifo_fd = -1;
  context->detach = 1;

  libcrun_debug ("Creating container: `%s`", context->id);
  container->context = context;

  ret = validate_options (options, LIBCRUN_CREATE_OPTIONS_PREFORK, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = check_config_file (def, context, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (def->process && def->process->terminal && context->console_socket == রিক্ত)
    প্রত্যাগম crun_make_error (err, 0, "use --console-socket with create when a terminal is used");

  ret = libcrun_status_check_directories (context->state_মূল, context->id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  exec_fifo_fd = libcrun_status_create_exec_fifo (context->state_মূল, context->id, err);
  যদি (UNLIKELY (exec_fifo_fd < 0))
    প্রত্যাগম exec_fifo_fd;

  context->fifo_exec_wait_fd = exec_fifo_fd;
  exec_fifo_fd = -1;

  যদি ((options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      libcrun_debug ("Running with prefork enabled");
      ret = libcrun_copy_config_file (context->id, context->state_মূল, container, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
      ret = libcrun_container_run_internal (container, context, রিক্ত, err);
      যদি (UNLIKELY (ret < 0))
        force_delete_container_status (context, container, def);
      প্রত্যাগম ret;
    }

  ret = pipe2 (container_ready_pipe, O_CLOEXEC);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "pipe");
  pipefd0 = container_ready_pipe[0];
  pipefd1 = container_ready_pipe[1];

  ret = fork ();
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "fork");
  যদি (ret)
    {
      পূর্ণাঙ্ক exit_code;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, রিক্ত, 0);

      ret = TEMP_FAILURE_RETRY (পঠ (pipefd0, &exit_code, পরিমাণ (exit_code)));
      যদি (UNLIKELY (ret <= 0))
        প্রত্যাগম crun_make_error (err, ret < 0 ? errno : 0, "waiting for container to be ready");

      যদি (ret != পরিমাণ (exit_code) || exit_code < 0)
        প্রত্যাগম crun_make_error (err, 0, "internal error: read invalid exit code");

      যদি (exit_code == 0)
        প্রত্যাগম 0;

      libcrun_debug ("Exit code is `%d`, deleting container", exit_code);
      libcrun_error_t tmp_err = রিক্ত;
      container_delete_internal (context, রিক্ত, def, context->id, true, true, &tmp_err);
      libcrun_error_report_and_release (&tmp_err);
      প্রত্যাগম crun_make_error (err, 0, "error creating container");
    }

  /* forked process.  */
  ret = detach_process ();
  যদি (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_মূল, container, err);
  যদি (UNLIKELY (ret < 0))
    {
      পূর্ণাঙ্ক errcode = crun_error_get_errno (err);
      crun_error_release (err);
      libcrun_fail_with_error (errcode, "copy config file");
    }

  ret = libcrun_container_run_internal (container, context, &pipefd1, err);
  যদি (UNLIKELY (ret < 0))
    {
      force_delete_container_status (context, container, def);
      libcrun_error ((*err)->status, "%s", (*err)->বার্তা);
      crun_error_release (err);
      crun_set_output_handler (log_write_to_stderr, রিক্ত);
    }

  যদি (pipefd1 >= 0)
    TEMP_FAILURE_RETRY (লিখ (pipefd1, &ret, পরিমাণ (ret)));
  exit (ret ? EXIT_FAILURE : 0);
}

পূর্ণাঙ্ক
libcrun_container_start (libcrun_context_t *context, ধ্রুব বর্ণ *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = রিক্ত;
  ধ্রুব বর্ণ *state_মূল = context->state_মূল;
  runtime_spec_schema_config_schema *def;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_close পূর্ণাঙ্ক fd = -1;
  পূর্ণাঙ্ক ret;

  ret = libcrun_read_container_status (&status, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = libcrun_is_container_running (&status, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (! ret)
    প্রত্যাগম crun_make_error (err, 0, "container `%s` is not running", id);

  ret = read_container_config_from_state (&container, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (context->notify_socket)
    {
      ret = get_notify_fd (context, container, &fd, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  ret = libcrun_status_write_exec_fifo (context->state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  def = container->container_def;

  যদি (context->notify_socket)
    {
      যদি (fd >= 0)
        {
          fd_set read_set;

          যাবত্ (1)
            {
              সংরচনা timeval timeout = {
                .tv_sec = 0,
                .tv_usec = 10000,
              };
              FD_ZERO (&read_set);
              FD_SET (fd, &read_set);

              ret = select (fd + 1, &read_set, রিক্ত, রিক্ত, &timeout);
              যদি (UNLIKELY (ret < 0))
                প্রত্যাগম crun_make_error (err, errno, "select failed");
              যদি (ret)
                {
                  ret = handle_notify_socket (fd, err);
                  যদি (UNLIKELY (ret < 0))
                    প্রত্যাগম ret;
                  যদি (ret)
                    ভঙ্গ;
                }
              অন্যথা
                {
                  ret = libcrun_is_container_running (&status, err);
                  যদি (UNLIKELY (ret < 0))
                    প্রত্যাগম ret;
                  যদি (! ret)
                    প্রত্যাগম 0;
                }
            }
        }
    }

  /* The container is considered running only after we got the notification from the
     notify_socket, if any.  */
  যদি (def->hooks && def->hooks->poststart_len)
    {
      cleanup_close পূর্ণাঙ্ক hooks_out_fd = -1;
      cleanup_close পূর্ণাঙ্ক hooks_err_fd = -1;

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      ret = do_hooks (def, status.pid, context->id, false, status.bundle, status.bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      যদি (UNLIKELY (ret != 0))
        {
          libcrun_error_t tmp_err = রিক্ত;
          container_delete_internal (context, container, def, id, true, true, &tmp_err);
          libcrun_error_report_and_release (&tmp_err);
          প্রত্যাগম ret;
        }
    }

  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
libcrun_get_container_state_string (ধ্রুব বর্ণ *id, libcrun_container_status_t *status, ধ্রুব বর্ণ *state_মূল,
                                    ধ্রুব বর্ণ **container_status, পূর্ণাঙ্ক *running, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret, has_fifo = 0;
  bool paused = false;

  ret = libcrun_is_container_running (status, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;
  *running = ret;

  যদি (*running)
    {
      ret = libcrun_status_has_read_exec_fifo (state_মূল, id, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
      has_fifo = ret;
    }

  যদি (*running && ! has_fifo)
    {
      cleanup_cgroup_status সংরচনা libcrun_cgroup_status *cgroup_status = রিক্ত;

      cgroup_status = libcrun_cgroup_make_status (status);

      ret = libcrun_cgroup_is_container_paused (cgroup_status, &paused, err);
      যদি (UNLIKELY (ret < 0))
        {
          /*
            The cgroup might have been cleaned up by the time we try to read it, ignore both
            ENOENT and ENODEV:
            - ENOENT: if the open(CGROUP_PATH) fails because the cgroup was deleted.
            - ENODEV: if the cgroup is deleted between the open and reading the freeze status.
          */

          errno = crun_error_get_errno (err);
          যদি (errno == ENOENT || errno == ENODEV)
            {
              crun_error_release (err);
              *container_status = "stopped";
              প্রত্যাগম 0;
            }

          প্রত্যাগম ret;
        }
    }

  যদি (! *running)
    *container_status = "stopped";
  অন্যথা যদি (has_fifo)
    *container_status = "created";
  অন্যথা যদি (paused)
    *container_status = "paused";
  অন্যথা
    *container_status = "running";

  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
libcrun_container_state_json (libcrun_context_t *context, ধ্রুব বর্ণ *id, বর্ণ **out, libcrun_error_t *err)
{
  বর্ণ *buffer = রিক্ত;
  size_t len = 0;
  FILE *f;
  পূর্ণাঙ্ক ret;

  f = open_memstream (&buffer, &len);
  যদি (f == রিক্ত)
    প্রত্যাগম crun_make_error (err, errno, "open_memstream");

  ret = libcrun_container_state (context, id, f, err);
  fclose (f);
  যদি (UNLIKELY (ret < 0))
    {
      মুক্ত (buffer);
      প্রত্যাগম ret;
    }

  *out = buffer;
  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
libcrun_container_state (libcrun_context_t *context, ধ্রুব বর্ণ *id, FILE *out, libcrun_error_t *err)
{
  ধ্রুব বর্ণ *ধ্রুব OCI_CONFIG_VERSION = "1.0.0";
  libcrun_container_status_t status = {};
  ধ্রুব বর্ণ *state_মূল = context->state_মূল;
  ধ্রুব বর্ণ *container_status = রিক্ত;
  json_gen_ctx *gen = রিক্ত;
  ধ্রুব বর্ণ *buf;
  পূর্ণাঙ্ক ret = 0;
  পূর্ণাঙ্ক r = json_gen_status_ok;
  পূর্ণাঙ্ক running;
  size_t len;

  ret = libcrun_read_container_status (&status, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = libcrun_get_container_state_string (id, &status, state_মূল, &container_status, &running, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ exit;

  ret = 0;
  যদি (! json_gen_init (&gen, রিক্ত))
    প্রত্যাগম crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));
  GEN_চাবি (gen, "ociVersion");
  GEN_STR (gen, OCI_CONFIG_VERSION);

  GEN_চাবি (gen, "id");
  GEN_STR (gen, id);

  GEN_চাবি (gen, "pid");
  GEN_OR_FAIL (map_int (gen, running ? status.pid : 0));

  GEN_চাবি (gen, "status");
  GEN_STR (gen, container_status);

  GEN_চাবি (gen, "bundle");
  GEN_STR (gen, status.bundle);

  GEN_চাবি (gen, "rootfs");
  GEN_STR (gen, status.rootfs);

  GEN_চাবি (gen, "created");
  GEN_STR (gen, status.created);

  যদি (status.scope)
    {
      GEN_চাবি (gen, "systemd-scope");
      GEN_STR (gen, status.scope);
    }

  যদি (status.owner)
    {
      GEN_চাবি (gen, "owner");
      GEN_STR (gen, status.owner);
    }

  {
    cleanup_container libcrun_container_t *container = রিক্ত;

    ret = read_container_config_from_state (&container, state_মূল, id, err);
    যদি (UNLIKELY (ret < 0))
      গচ্ছ exit;

    যদি (container->container_def->annotations && container->container_def->annotations->len)
      GEN_OR_FAIL (gen_annotations (gen, container->container_def->annotations));
  }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  fprintf (out, "%s\n", buf);

exit:
  যদি (gen)
    json_gen_free (gen);
  libcrun_free_container_status (&status);
  প্রত্যাগম ret;

gen_error:
  ret = json_gen_error_to_crun_error (r, err);
  গচ্ছ exit;
}

পূর্ণাঙ্ক
libcrun_container_exec (libcrun_context_t *context, ধ্রুব বর্ণ *id, runtime_spec_schema_config_schema_process *process,
                        libcrun_error_t *err)
{
  সংরচনা libcrun_container_exec_options_s opts;
  memset (&opts, 0, পরিমাণ (opts));

  opts.struct_size = পরিমাণ (opts);
  opts.process = process;

  প্রত্যাগম libcrun_container_exec_with_options (context, id, &opts, err);
}

পূর্ণাঙ্ক
libcrun_container_exec_process_file (libcrun_context_t *context, ধ্রুব বর্ণ *id, ধ্রুব বর্ণ *path, libcrun_error_t *err)
{
  সংরচনা libcrun_container_exec_options_s opts;
  memset (&opts, 0, পরিমাণ (opts));

  opts.struct_size = পরিমাণ (opts);
  opts.path = path;

  প্রত্যাগম libcrun_container_exec_with_options (context, id, &opts, err);
}

স্থির পূর্ণাঙ্ক
exec_from_process_json (libcrun_context_t *context, ধ্রুব বর্ণ *id, ধ্রুব বর্ণ *process_json, ধ্রুব বর্ণ *cgroup,
                        bool merge_env, libcrun_error_t *err)
{
  সংরচনা parser_context parser_ctx = { .options = 0, .errfile = stderr };
  সংরচনা libcrun_container_exec_options_s opts;
  runtime_spec_schema_config_schema_process *process;
  parser_error parser_err = রিক্ত;
  json_object *doc = রিক্ত;
  পূর্ণাঙ্ক ret;

  ret = parse_json_file (&doc, process_json, &parser_ctx, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  process = make_runtime_spec_schema_config_schema_process (doc, &parser_ctx, &parser_err);
  json_object_put (doc);
  যদি (process == রিক্ত)
    {
      ret = crun_make_error (err, 0, "cannot parse process: `%s`", parser_err ? parser_err : "unknown error");
      মুক্ত (parser_err);
      প্রত্যাগম ret;
    }

  memset (&opts, 0, পরিমাণ (opts));
  opts.struct_size = পরিমাণ (opts);
  opts.process = process;
  opts.cgroup = cgroup;
  opts.merge_env = merge_env;

  ret = libcrun_container_exec_with_options (context, id, &opts, err);
  free_runtime_spec_schema_config_schema_process (process);
  প্রত্যাগম ret;
}

পূর্ণাঙ্ক
libcrun_container_exec_json (libcrun_context_t *context, ধ্রুব বর্ণ *id, ধ্রুব বর্ণ *process_json, libcrun_error_t *err)
{
  প্রত্যাগম exec_from_process_json (context, id, process_json, রিক্ত, false, err);
}

পূর্ণাঙ্ক
libcrun_container_exec_with_options_json (libcrun_context_t *context, ধ্রুব বর্ণ *id,
                                          ধ্রুব সংরচনা libcrun_exec_options_s *opts, libcrun_error_t *err)
{
  যদি (opts == রিক্ত || opts->struct_size == 0)
    প্রত্যাগম crun_make_error (err, EINVAL, "invalid exec options");

  প্রত্যাগম exec_from_process_json (context, id, opts->process_json, opts->cgroup, opts->merge_env, err);
}

#define cleanup_process_schema __attribute__ ((cleanup (cleanup_process_schemap)))

স্থির অন্তঃস্থ শূন্য
cleanup_process_schemap (runtime_spec_schema_config_schema_process **p)
{
  runtime_spec_schema_config_schema_process *process = *p;
  যদি (process)
    (শূন্য) free_runtime_spec_schema_config_schema_process (process);
}

/* Build the seccomp receiver payload (when needed), apply the seccomp profile and
   close the seccomp file descriptors.  Used from exec_process_entrypoint both before
   and after the capabilities are set, depending on process->no_new_privileges.  */
স্থির পূর্ণাঙ্ক
apply_seccomp_for_exec (libcrun_container_t *container, pid_t own_pid, পূর্ণাঙ্ক seccomp_fd,
                        পূর্ণাঙ্ক seccomp_receiver_fd, বর্ণ **seccomp_flags, size_t seccomp_flags_len,
                        libcrun_error_t *err)
{
  cleanup_free বর্ণ *seccomp_fd_payload = রিক্ত;
  size_t seccomp_fd_payload_len = 0;
  পূর্ণাঙ্ক ret;

  যদি (seccomp_receiver_fd >= 0)
    {
      ret = get_seccomp_receiver_fd_payload (container, "running", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  ret = libcrun_apply_seccomp (seccomp_fd, seccomp_receiver_fd, seccomp_fd_payload,
                               seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  close_and_reset (&seccomp_fd);
  close_and_reset (&seccomp_receiver_fd);
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
exec_process_entrypoint (libcrun_context_t *context,
                         libcrun_container_t *container,
                         runtime_spec_schema_config_schema_process *process,
                         পূর্ণাঙ্ক *pipefd1,
                         পূর্ণাঙ্ক seccomp_fd,
                         পূর্ণাঙ্ক seccomp_receiver_fd,
                         সংরচনা custom_handler_instance_s *custom_handler,
                         bool merge_env,
                         libcrun_error_t *err)
{
  runtime_spec_schema_config_schema_process_capabilities *capabilities = রিক্ত;
  cleanup_free বর্ণ *exec_path = রিক্ত;
  uid_t container_uid;
  gid_t container_gid;
  ধ্রুব বর্ণ *cwd;
  bool chdir_done = false;
  size_t seccomp_flags_len = 0;
  বর্ণ **seccomp_flags = রিক্ত;
  pid_t own_pid = 0;
  size_t i;
  পূর্ণাঙ্ক ret;

  container_uid = process->user ? process->user->uid : 0;
  container_gid = process->user ? process->user->gid : 0;

  TEMP_FAILURE_RETRY (পঠ (*pipefd1, &own_pid, পরিমাণ (own_pid)));

  cwd = process->cwd ? process->cwd : "/";
  যদি (LIKELY (libcrun_safe_chdir (cwd, err) == 0))
    chdir_done = true;
  অন্যথা
    crun_error_release (err);

  ret = unblock_signals (err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = clearenv ();
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "clearenv");

  যদি ((merge_env || ! process->env_len) && container->container_def->process && container->container_def->process->env_len)
    {
      বর্ণ *e;

      প্রতি (i = 0; i < container->container_def->process->env_len; i++)
        {
          e = container->container_def->process->env[i];
          যদি (putenv (e) < 0)
            প্রত্যাগম crun_make_error (err, errno, "putenv `%s`", e);
        }
    }
  যদি (process->env_len)
    {
      প্রতি (i = 0; i < process->env_len; i++)
        যদি (putenv (process->env[i]) < 0)
          প্রত্যাগম crun_make_error (err, errno, "putenv `%s`", process->env[i]);
    }

  যদি (is_empty_string (getenv ("HOME")))
    {
      ret = set_home_env (container_uid);
      যদি (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          যদি (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            প্রত্যাগম crun_make_error (err, errno, "setenv HOME");
        }
    }

  ret = libcrun_set_selinux_label (container, process, false, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = libcrun_set_apparmor_profile (container, process, false, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (container->container_def->linux && container->container_def->linux->seccomp)
    {
      seccomp_flags = container->container_def->linux->seccomp->flags;
      seccomp_flags_len = container->container_def->linux->seccomp->flags_len;
    }

  ret = find_executable (&exec_path, process->args[0], process->cwd, err);
  যদি (UNLIKELY (ret < 0))
    {
      যদি (crun_error_get_errno (err) == ENOENT)
        প্রত্যাগম ret;

      /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
         once the process switched to the use that runs in the container.  This might be necessary
         when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
         is not honored.  */
      crun_error_release (err);
    }

  যদি (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, false, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = libcrun_reopen_dev_null (err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (! process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  ret = libcrun_container_setgroups (container, process, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = maybe_chown_std_streams (container_uid, container_gid, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (process->capabilities)
    capabilities = process->capabilities;
  অন্যথা যদি (container->container_def->process)
    capabilities = container->container_def->process->capabilities;

  ret = libcrun_set_caps (capabilities, container_uid, container_gid, process->no_new_privileges,
                          custom_handler != রিক্ত, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (UNLIKELY (exec_path == রিক্ত))
    {
      ret = find_executable (&exec_path, process->args[0], process->cwd, err);
      যদি (UNLIKELY (ret < 0))
        {
          যদি (custom_handler == রিক্ত || is_empty_string (process->args[0]))
            প্রত্যাগম ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          exec_path = xstrdup (process->args[0]);
        }
    }

  যদি (! chdir_done)
    {
      ret = libcrun_safe_chdir (cwd, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  যদি (process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  যদি (process->user)
    umask (process->user->umask_present ? process->user->umask : 0022);

  ret = 0;
  TEMP_FAILURE_RETRY (লিখ (*pipefd1, &ret, পরিমাণ (ret)));
  TEMP_FAILURE_RETRY (close (*pipefd1));
  *pipefd1 = -1;

  যদি (custom_handler)
    {
      যদি (custom_handler->vtable->exec_func == রিক্ত)
        প্রত্যাগম crun_make_error (err, 0, "the handler does not support exec");

      যদি (context->argv)
        erase_initial_environment (context->argv, context->argc);

      ret = custom_handler->vtable->exec_func (custom_handler->cookie,
                                               container,
                                               exec_path,
                                               process->args);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম crun_make_error (err, -ret, "exec container process failed with handler as `%s`", custom_handler->vtable->name);

      _safe_exit (EXIT_FAILURE);
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation, because the seccomp filter is already in place and it could
     stop some syscalls used by mark_or_close_fds_ge_than.
  */
  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, true, err);
  যদি (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, process->args));
  libcrun_fail_with_error (errno, "exec `%s`", exec_path);
}

/* Close the file descriptors inherited from the caller, other than those
   to be passed to the container process.  */
পূর্ণাঙ্ক
libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err)
{
  প্রত্যাগম mark_or_close_fds_ge_than (রিক্ত, context->preserve_fds + 3, true, err);
}

পূর্ণাঙ্ক
libcrun_load_process_from_file (ধ্রুব বর্ণ *path, runtime_spec_schema_config_schema_process **process,
                                libcrun_error_t *err)
{
  সংরচনা parser_context ctx = { 0, stderr };
  cleanup_free বর্ণ *content = রিক্ত;
  parser_error parser_err = রিক্ত;
  json_object *doc = রিক্ত;
  size_t len;
  পূর্ণাঙ্ক ret;

  ret = read_all_file (path, &content, &len, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  *process = make_runtime_spec_schema_config_schema_process (doc, &ctx, &parser_err);
  যদি (UNLIKELY (*process == রিক্ত))
    {
      ret = crun_make_error (err, 0, "cannot parse process file: `%s`", parser_err);
      মুক্ত (parser_err);
      যদি (doc)
        json_object_put (doc);
      প্রত্যাগম ret;
    }

  মুক্ত (parser_err);
  যদি (doc)
    json_object_put (doc);

  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
libcrun_container_exec_with_options (libcrun_context_t *context, ধ্রুব বর্ণ *id,
                                     সংরচনা libcrun_container_exec_options_s *opts,
                                     libcrun_error_t *err)
{
  cleanup_custom_handler_instance সংরচনা custom_handler_instance_s *custom_handler = রিক্ত;
  পূর্ণাঙ্ক container_status, ret;
  bool container_paused = false;
  pid_t pid;
  libcrun_container_status_t status = {};
  ধ্রুব বর্ণ *state_মূল = context->state_মূল;
  cleanup_close পূর্ণাঙ্ক terminal_fd = -1;
  cleanup_close পূর্ণাঙ্ক seccomp_fd = -1;
  cleanup_terminal শূন্য *orig_terminal = রিক্ত;
  cleanup_container libcrun_container_t *container = রিক্ত;
  পূর্ণাঙ্ক container_ret_status[2];
  cleanup_close পূর্ণাঙ্ক pipefd0 = -1;
  cleanup_close পূর্ণাঙ্ক pipefd1 = -1;
  cleanup_close পূর্ণাঙ্ক seccomp_receiver_fd = -1;
  cleanup_close পূর্ণাঙ্ক own_seccomp_receiver_fd = -1;
  cleanup_close পূর্ণাঙ্ক seccomp_notify_fd = -1;
  ধ্রুব বর্ণ *seccomp_notify_plugins = রিক্ত;
  __attribute__ ((unused)) cleanup_process_schema runtime_spec_schema_config_schema_process *process_cleanup = রিক্ত;
  runtime_spec_schema_config_schema_process *process = opts->process;
  সংরচনা libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  পূর্ণাঙ্ক ret_from_child = 0;

  ret = libcrun_read_container_status (&status, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = libcrun_is_container_running (&status, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;
  container_status = ret;

  ret = read_container_config_from_state (&container, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  container->context = context;

  যদি (container_status == 0)
    প্রত্যাগম crun_make_error (err, 0, "the container `%s` is not running", id);

  {
    cleanup_cgroup_status সংরচনা libcrun_cgroup_status *cgroup_status = রিক্ত;

    cgroup_status = libcrun_cgroup_make_status (&status);

    ret = libcrun_cgroup_is_container_paused (cgroup_status, &container_paused, err);
    যদি (UNLIKELY (ret < 0))
      প্রত্যাগম ret;
  }

  যদি (UNLIKELY (container_paused))
    প্রত্যাগম crun_make_error (err, 0, "cannot exec in a paused container `%s`", id);

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = block_signals (err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  libcrun_seccomp_gen_ctx_init (&seccomp_gen_ctx, container, false, 0);

  ret = libcrun_open_seccomp_bpf (&seccomp_gen_ctx, &seccomp_fd, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &seccomp_receiver_fd,
                                     &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  যদি (পরিমাণ (*opts) != opts->struct_size)
    প্রত্যাগম crun_make_error (err, EINVAL, "invalid libcrun_container_exec_options_s struct");

  যদি (opts->path)
    {
      যদি (process)
        প্রত্যাগম crun_make_error (err, EINVAL, "cannot specify both exec file and options");

      ret = libcrun_load_process_from_file (opts->path, &process, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      process_cleanup = process;
    }

  /* This must be done before we enter a user namespace.  If the process does
     not specify any rlimits, use the ones from the container configuration.  */
  যদি (process->rlimits_len == 0 && container->container_def->process)
    ret = libcrun_set_rlimits (container->container_def->process->rlimits,
                               container->container_def->process->rlimits_len, err);
  অন্যথা
    ret = libcrun_set_rlimits (process->rlimits, process->rlimits_len, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = libcrun_set_mempolicy (container->container_def, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  /* If the new process block doesn't specify a SELinux label, AppArmor profile, user,
     I/O priority or scheduler, then use the configuration from the original config file.  */
  যদি (container->container_def->process)
    {
      যদি (process->io_priority == রিক্ত && container->container_def->process->io_priority)
        {
          process->io_priority = clone_runtime_spec_schema_config_schema_process_io_priority (container->container_def->process->io_priority);
          যদি (process->io_priority == রিক্ত)
            OOM ();
        }

      যদি (process->scheduler == রিক্ত && container->container_def->process->scheduler)
        {
          process->scheduler = clone_runtime_spec_schema_config_schema_process_scheduler (container->container_def->process->scheduler);
          যদি (process->scheduler == রিক্ত)
            OOM ();
        }

      যদি (process->selinux_label == রিক্ত && container->container_def->process->selinux_label)
        process->selinux_label = xstrdup (container->container_def->process->selinux_label);

      যদি (process->apparmor_profile == রিক্ত && container->container_def->process->apparmor_profile)
        process->apparmor_profile = xstrdup (container->container_def->process->apparmor_profile);

      যদি (process->user == রিক্ত && container->container_def->process->user)
        {
          process->user = clone_runtime_spec_schema_config_schema_process_user (container->container_def->process->user);
          যদি (process->user == রিক্ত)
            OOM ();
        }
    }

  ret = initialize_security (container, process, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  /* Do not leak the session keyring of the caller into the container.  The
     keyring created for the container cannot be joined, so use an anonymous one.  */
  যদি (! context->no_new_keyring && ! status.no_new_keyring)
    {
      libcrun_debug ("Creating new keyring for the exec process");

      ret = libcrun_create_keyring (container, রিক্ত, process->selinux_label, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
    }

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "prctl unset dumpable");

  pid = libcrun_join_process (context, container, status.pid, &status, opts->cgroup, context->detach,
                              process, process->terminal ? &terminal_fd : রিক্ত, err);
  যদি (UNLIKELY (pid < 0))
    প্রত্যাগম pid;

  /* Process to exec.  */
  যদি (pid == 0)
    {
      TEMP_FAILURE_RETRY (close (pipefd0));
      pipefd0 = -1;

      exec_process_entrypoint (context, container, process, &pipefd1, seccomp_fd, seccomp_receiver_fd, custom_handler, opts->merge_env, err);
      /* It gets here only on errors.  */
      যদি (*err)
        {
          যদি (pipefd1 < 0)
            libcrun_fail_with_error ((*err)->status, "%s", (*err)->বার্তা);
          অন্যথা
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

  TEMP_FAILURE_RETRY (লিখ (pipefd0, &pid, পরিমাণ (pid)));

  যদি (seccomp_fd >= 0)
    close_and_reset (&seccomp_fd);

  যদি (terminal_fd >= 0)
    {
      unsigned short rows = 0, cols = 0;

      যদি (process->console_size)
        {
          cols = process->console_size->width;
          rows = process->console_size->height;
        }

      ret = libcrun_terminal_setup_size (terminal_fd, rows, cols, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;

      যদি (context->console_socket)
        {
          পূর্ণাঙ্ক ret;
          cleanup_close পূর্ণাঙ্ক console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
          যদি (UNLIKELY (console_socket_fd < 0))
            প্রত্যাগম console_socket_fd;
          ret = send_fd_to_socket (console_socket_fd, terminal_fd, err);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম ret;
          close_and_reset (&terminal_fd);
        }
      অন্যথা
        {
          ret = libcrun_clear_onlcr (terminal_fd, err);
          যদি (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              প্রত্যাগম ret;
            }

          ret = libcrun_set_raw (0, &orig_terminal, err);
          যদি (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              প্রত্যাগম ret;
            }
        }
    }

  ret = TEMP_FAILURE_RETRY (পঠ (pipefd0, &ret_from_child, পরিমাণ (ret_from_child)));
  যদি (ret != পরিমাণ (ret_from_child))
    ret = crun_make_error (err, 0, "read pipe failed");
  অন্যথা যদি (ret_from_child != 0)
    {
      cleanup_free বর্ণ *বার্তা = রিক্ত;
      size_t len = 0;

      ret = read_all_fd (pipefd0, "error stream", &বার্তা, &len, err);
      যদি (UNLIKELY (ret < 0))
        প্রত্যাগম ret;
      /* the string from read_all_fd is always NUL terminated.  */
      ret = crun_make_error (err, ret_from_child, "%s", বার্তা);
    }
  অন্যথা
    {
      /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
      যদি (own_seccomp_receiver_fd >= 0)
        {
          seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
          যদি (UNLIKELY (seccomp_notify_fd < 0))
            প্রত্যাগম seccomp_notify_fd;

          ret = close_and_reset (&own_seccomp_receiver_fd);
          যদি (UNLIKELY (ret < 0))
            প্রত্যাগম crun_make_error (err, errno, "close seccomp receiver fd failed");
        }

      {
        সংরচনা wait_for_process_args args = {
          .pid = pid,
          .context = context,
          .terminal_fd = terminal_fd,
          .notify_socket = -1,
          .container_ready_fd = রিক্ত,
          .seccomp_notify_fd = seccomp_notify_fd,
          .seccomp_notify_plugins = seccomp_notify_plugins,
        };

        ret = wait_for_process (&args, err);
      }
    }

  flush_fd_to_err (context, terminal_fd);
  প্রত্যাগম ret;
}

পূর্ণাঙ্ক
libcrun_container_update (libcrun_context_t *context, ধ্রুব বর্ণ *id, ধ্রুব বর্ণ *content, size_t len arg_unused,
                          libcrun_error_t *err)
{
  cleanup_custom_handler_instance সংরচনা custom_handler_instance_s *custom_handler = রিক্ত;
  runtime_spec_schema_config_linux_resources *resources = রিক্ত;
  cleanup_container libcrun_container_t *container = রিক্ত;
  ধ্রুব বর্ণ *state_মূল = context->state_মূল;
  সংরচনা parser_context ctx = { 0, stderr };
  libcrun_container_status_t status = {};
  parser_error parser_err = রিক্ত;
  json_object *doc = রিক্ত;
  পূর্ণাঙ্ক ret;

  ret = libcrun_read_container_status (&status, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = read_container_config_from_state (&container, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  resources = make_runtime_spec_schema_config_linux_resources (doc, &ctx, &parser_err);
  যদি (UNLIKELY (resources == রিক্ত))
    {
      ret = crun_make_error (err, 0, "cannot parse resources: %s", parser_err);
      গচ্ছ cleanup;
    }

  যদি (custom_handler && custom_handler->vtable->modify_oci_configuration)
    {
      /* Adapt RESOURCES to be used from the modify_oci_configuration hook.  */
      cleanup_free runtime_spec_schema_config_linux *linux = xmalloc0 (পরিমাণ (*linux));
      cleanup_free runtime_spec_schema_config_schema *def = xmalloc0 (পরিমাণ (*def));

      def->linux = linux;
      linux->resources = resources;

      ret = custom_handler->vtable->modify_oci_configuration (custom_handler->cookie,
                                                              context,
                                                              def,
                                                              err);
      যদি (UNLIKELY (ret < 0))
        গচ্ছ cleanup;
    }

  ret = libcrun_linux_container_update (&status, state_মূল, resources, err);

cleanup:
  যদি (doc)
    json_object_put (doc);
  মুক্ত (parser_err);
  যদি (resources)
    free_runtime_spec_schema_config_linux_resources (resources);

  প্রত্যাগম ret;
}

পূর্ণাঙ্ক
libcrun_container_update_from_file (libcrun_context_t *context, ধ্রুব বর্ণ *id, ধ্রুব বর্ণ *file, libcrun_error_t *err)
{
  cleanup_free বর্ণ *content = রিক্ত;
  size_t len;
  পূর্ণাঙ্ক ret;

  ret = read_all_file (file, &content, &len, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  প্রত্যাগম libcrun_container_update (context, id, content, len, err);
}

স্থির পূর্ণাঙ্ক
compare_update_মানসমূহ (ধ্রুব শূন্য *a, ধ্রুব শূন্য *b)
{
  ধ্রুব সংরচনা libcrun_update_মান_s *aa = a;
  ধ্রুব সংরচনা libcrun_update_মান_s *bb = b;
  পূর্ণাঙ্ক ret;

  ret = strcmp (aa->section, bb->section);
  যদি (ret)
    প্রত্যাগম ret;
  প্রত্যাগম strcmp (aa->name, bb->name);
}

পূর্ণাঙ্ক
libcrun_container_update_from_মানসমূহ (libcrun_context_t *context, ধ্রুব বর্ণ *id,
                                      সংরচনা libcrun_update_মান_s *মানসমূহ, size_t len,
                                      libcrun_error_t *err)
{
  ধ্রুব বর্ণ *current_section = রিক্ত;
  ধ্রুব বর্ণ *buf;
  json_gen_ctx *gen = রিক্ত;
  size_t i, buf_len;
  পূর্ণাঙ্ক ret;

  যদি (! json_gen_init (&gen, রিক্ত))
    প্রত্যাগম crun_make_error (err, 0, "json_gen_init failed");
  json_gen_map_open (gen);

  qsort (মানসমূহ, len, পরিমাণ (সংরচনা libcrun_update_মান_s), compare_update_মানসমূহ);

  প্রতি (i = 0; i < len; i++)
    {
      যদি (current_section == রিক্ত || strcmp (মানসমূহ[i].section, current_section))
        {
          যদি (i > 0)
            json_gen_map_close (gen);

          current_section = মানসমূহ[i].section;
          json_gen_string (gen, current_section, strlen (current_section));
          json_gen_map_open (gen);
        }

      json_gen_string (gen, মানসমূহ[i].name, strlen (মানসমূহ[i].name));

      যদি (মানসমূহ[i].numeric)
        json_gen_সংখ্যা (gen, মানসমূহ[i].মান, strlen (মানসমূহ[i].মান));
      অন্যথা
        json_gen_string (gen, মানসমূহ[i].মান, strlen (মানসমূহ[i].মান));
    }
  যদি (len)
    json_gen_map_close (gen);

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);

  ret = libcrun_container_update (context, id, buf, buf_len, err);

  json_gen_free (gen);

  প্রত্যাগম ret;
}

স্থির শূন্য
populate_array_field (বর্ণ ***field, বর্ণ *array[], size_t অঙ্ক_elements)
{
  size_t i;

  *field = xmalloc0 ((অঙ্ক_elements + 1) * পরিমাণ (বর্ণ *));
  প্রতি (i = 0; i < অঙ্ক_elements; i++)
    (*field)[i] = xstrdup (array[i]);

  (*field)[i] = রিক্ত;
}

#ifdef HAVE_CAP
স্থির শূন্য
populate_capabilities (সংরচনা features_info_s *info, বর্ণ ***capabilities, size_t *অঙ্ক_capabilities)
{
  size_t index = 0;
  cap_মান_t i;
  বর্ণ *endptr;
  পূর্ণাঙ্ক j;

  *অঙ্ক_capabilities = 0;
  প্রতি (i = 0;; i++)
    {
      বর্ণ *v = cap_to_name (i);
      যদি (v == রিক্ত)
        ভঙ্গ;
      strtol (v, &endptr, 10);
      যদি (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          ভঙ্গ;
        }
      (*অঙ্ক_capabilities)++;
    }

  *capabilities = xmalloc0 ((*অঙ্ক_capabilities + 1) * পরিমাণ (ধ্রুব বর্ণ *));
  প্রতি (i = 0; i < (cap_মান_t) *অঙ্ক_capabilities; i++)
    {
      বর্ণ *v = cap_to_name (i);
      যদি (v == রিক্ত)
        ভঙ্গ;
      strtol (v, &endptr, 10);
      যদি (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          ভঙ্গ;
        }

      // Convert capability name to uppercase
      প্রতি (j = 0; v[j] != '\0'; j++)
        v[j] = toupper (v[j]);

      (*capabilities)[index] = v;
      index++;
    }

  (*capabilities)[index] = রিক্ত; // Terminate the array with NULL
  populate_array_field (&(info->linux.capabilities), *capabilities, *অঙ্ক_capabilities);
}
#endif

স্থির শূন্য
retrieve_mount_options (সংরচনা features_info_s **info)
{
  cleanup_free ধ্রুব সংরচনা propagation_flags_s *mount_options_list = রিক্ত;
  size_t অঙ্ক_mount_options = 0;

  // Retrieve mount options from wordlist
  mount_options_list = get_mount_flags_from_wordlist ();

  // Calculate the number of mount options
  যাবত্ (mount_options_list[অঙ্ক_mount_options].name != রিক্ত)
    অঙ্ক_mount_options++;

  // Allocate memory for mount options in info struct
  (*info)->mount_options = xmalloc0 ((অঙ্ক_mount_options + 1) * পরিমাণ (বর্ণ *));

  // Copy mount options to info struct
  প্রতি (size_t i = 0; i < অঙ্ক_mount_options; i++)
    (*info)->mount_options[i] = xstrdup (mount_options_list[i].name);
}

পূর্ণাঙ্ক
libcrun_container_get_features (libcrun_context_t *context, সংরচনা features_info_s **info, libcrun_error_t *err arg_unused)
{
  // Allocate memory for the features_info_s structure
  size_t অঙ্ক_namspaces = পরিমাণ (namespaces) / পরিমাণ (namespaces[0]);
  size_t অঙ্ক_operators = পরিমাণ (operators) / পরিমাণ (operators[0]);
  size_t অঙ্ক_actions = পরিমাণ (actions) / পরিমাণ (actions[0]);
  size_t অঙ্ক_hooks = পরিমাণ (hooks) / পরিমাণ (hooks[0]);
  size_t অঙ্ক_archs = পরিমাণ (archs) / পরিমাণ (archs[0]);
  size_t অঙ্ক_unsafe_annotations = পরিমাণ (potentially_unsafe_annotations) / পরিমাণ (potentially_unsafe_annotations[0]);
  cleanup_free বর্ণ **capabilities = রিক্ত;
  size_t অঙ্ক_capabilities = 0;
  size_t অঙ্ক_mempolicy_modes = পরিমাণ (mempolicy_modes) / পরিমাণ (mempolicy_modes[0]);
  size_t অঙ্ক_mempolicy_flags = পরিমাণ (mempolicy_flags) / পরিমাণ (mempolicy_flags[0]);

  *info = xmalloc0 (পরিমাণ (সংরচনা features_info_s));

  // Hardcoded feature information
  (*info)->oci_version_min = xstrdup ("1.0.0");
  (*info)->oci_version_max = xstrdup ("1.1.0+dev");

  // Populate hooks
  populate_array_field (&((*info)->hooks), hooks, অঙ্ক_hooks);

  // Populate mount_options
  retrieve_mount_options (info);

  // Populate namespaces
  populate_array_field (&((*info)->linux.namespaces), namespaces, অঙ্ক_namspaces);

#ifdef HAVE_CAP
  // Populate capabilities
  populate_capabilities (*info, &capabilities, &অঙ্ক_capabilities);
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
  populate_array_field (&((*info)->linux.seccomp.actions), actions, অঙ্ক_actions);
  // Populate operators
  populate_array_field (&((*info)->linux.seccomp.operators), operators, অঙ্ক_operators);
  // Populate archs
  populate_array_field (&((*info)->linux.seccomp.archs), archs, অঙ্ক_archs);
#অন্যথা
  (*info)->linux.seccomp.enabled = false;
#endif

  // Put values for apparmor and selinux
  (*info)->linux.apparmor.enabled = true;
  (*info)->linux.selinux.enabled = true;

  (*info)->linux.intel_rdt.enabled = true;

  (*info)->linux.net_devices.enabled = true;

  populate_array_field (&((*info)->linux.memory_policy.mode), mempolicy_modes, অঙ্ক_mempolicy_modes);
  populate_array_field (&((*info)->linux.memory_policy.flags), mempolicy_flags, অঙ্ক_mempolicy_flags);

  // Put the values for mount extensions
  (*info)->linux.mount_ext.idmap.enabled = true;

  // Populate the values for annotations
#ifdef HAVE_SECCOMP
  {
    ধ্রুব সংরচনা scmp_version *version = seccomp_version ();
    বর্ণ *version_string = রিক্ত;

    xasprintf (&version_string, "%u.%u.%u", version->major, version->minor, version->micro);
    (*info)->annotations.io_github_seccomp_libseccomp_version = version_string;
  }
#endif

  যদি (context->handler_manager && handler_by_name (context->handler_manager, "wasm"))
    (*info)->annotations.run_oci_crun_wasm = true;

#যদি HAVE_CRIU
  (*info)->annotations.run_oci_crun_checkpoint_enabled = true;
#endif
  (*info)->annotations.run_oci_crun_commit = GIT_VERSION;
  (*info)->annotations.run_oci_crun_version = PACKAGE_VERSION;

  populate_array_field (&((*info)->potentially_unsafe_annotations), potentially_unsafe_annotations, অঙ্ক_unsafe_annotations);

  প্রত্যাগম 0;
}

স্থির শূন্য
features_add_string (json_gen_ctx *json_gen, ধ্রুব বর্ণ *চাবি, ধ্রুব বর্ণ *মান)
{
  json_gen_string (json_gen, চাবি, strlen (চাবি));
  json_gen_string (json_gen, মান, strlen (মান));
}

স্থির শূন্য
features_add_bool (json_gen_ctx *json_gen, ধ্রুব বর্ণ *চাবি, পূর্ণাঙ্ক মান)
{
  json_gen_string (json_gen, চাবি, strlen (চাবি));
  json_gen_bool (json_gen, মান);
}

স্থির শূন্য
features_add_bool_str (json_gen_ctx *json_gen, ধ্রুব বর্ণ *চাবি, পূর্ণাঙ্ক মান)
{
  features_add_string (json_gen, চাবি, মান ? "true" : "false");
}

স্থির শূন্য
features_add_array (json_gen_ctx *json_gen, ধ্রুব বর্ণ *চাবি, বর্ণ **array)
{
  size_t i;

  json_gen_string (json_gen, চাবি, strlen (চাবি));
  json_gen_array_open (json_gen);
  যদি (array)
    প্রতি (i = 0; array[i] != রিক্ত; i++)
      json_gen_string (json_gen, array[i], strlen (array[i]));
  json_gen_array_close (json_gen);
}

স্থির শূন্য
features_add_linux_info (json_gen_ctx *json_gen, ধ্রুব সংরচনা linux_info_s *linux)
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
  যদি (linux->seccomp.actions)
    features_add_array (json_gen, "actions", linux->seccomp.actions);
  যদি (linux->seccomp.operators)
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
  যদি (linux->memory_policy.mode)
    features_add_array (json_gen, "modes", linux->memory_policy.mode);
  যদি (linux->memory_policy.flags)
    features_add_array (json_gen, "flags", linux->memory_policy.flags);
  json_gen_map_close (json_gen);

  json_gen_map_close (json_gen);
}

স্থির শূন্য
features_add_annotations_info (json_gen_ctx *json_gen, ধ্রুব সংরচনা annotations_info_s *annotation)
{
  json_gen_string (json_gen, "annotations", strlen ("annotations"));
  json_gen_map_open (json_gen);

  যদি (! is_empty_string (annotation->io_github_seccomp_libseccomp_version))
    features_add_string (json_gen, "io.github.seccomp.libseccomp.version", annotation->io_github_seccomp_libseccomp_version);

  features_add_bool_str (json_gen, "org.opencontainers.runc.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);
  features_add_bool_str (json_gen, "run.oci.crun.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);

  features_add_string (json_gen, "run.oci.crun.commit", annotation->run_oci_crun_commit);
  features_add_string (json_gen, "run.oci.crun.version", annotation->run_oci_crun_version);

  features_add_bool_str (json_gen, "run.oci.crun.wasm", annotation->run_oci_crun_wasm);

  json_gen_map_close (json_gen);
}

পূর্ণাঙ্ক
libcrun_container_get_features_json (libcrun_context_t *context, বর্ণ **out, libcrun_error_t *err)
{
  cleanup_struct_features সংরচনা features_info_s *info = রিক্ত;
  ধ্রুব বর্ণ *buffer = রিক্ত;
  json_gen_ctx *json_gen;
  size_t buffer_len;
  পূর্ণাঙ্ক ret;

  *out = রিক্ত;

  ret = libcrun_container_get_features (context, &info, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (! json_gen_init (&json_gen, রিক্ত))
    প্রত্যাগম crun_make_error (err, 0, "cannot initialize JSON generator");

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

  প্রত্যাগম 0;
}

/* Read the container status and fail if the container is not running.  */
স্থির পূর্ণাঙ্ক
read_status_require_running (libcrun_context_t *context, ধ্রুব বর্ণ *id,
                             libcrun_container_status_t *status, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  ret = libcrun_read_container_status (status, context->state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = libcrun_is_container_running (status, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;
  যদি (ret == 0)
    প্রত্যাগম crun_make_error (err, 0, "the container `%s` is not running", id);

  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
libcrun_container_pause (libcrun_context_t *context, ধ্রুব বর্ণ *id, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  প্রত্যাগম libcrun_container_pause_linux (&status, err);
}

পূর্ণাঙ্ক
libcrun_container_unpause (libcrun_context_t *context, ধ্রুব বর্ণ *id, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  প্রত্যাগম libcrun_container_unpause_linux (&status, err);
}

/* Translate the public options struct into the internal representation used by
   the linux/CRIU layer.  Strings are borrowed (not copied).  */
স্থির শূন্য
cr_options_from_public (libcrun_checkpoint_restore_t *dst, সংরচনা libcrun_checkpoint_restore_options_s *src)
{
  memset (dst, 0, পরিমাণ (*dst));
  dst->image_path = (বর্ণ *) src->image_path;
  dst->work_path = (বর্ণ *) src->work_path;
  dst->parent_path = (বর্ণ *) src->parent_path;
  dst->console_socket = src->console_socket;
  dst->lsm_profile = (বর্ণ *) src->lsm_profile;
  dst->lsm_mount_context = (বর্ণ *) src->lsm_mount_context;
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

পূর্ণাঙ্ক
libcrun_container_checkpoint (libcrun_context_t *context, ধ্রুব বর্ণ *id,
                              সংরচনা libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  ধ্রুব বর্ণ *state_মূল = context->state_মূল;
  libcrun_container_status_t status = {};
  libcrun_checkpoint_restore_t cr;
  cleanup_container libcrun_container_t *container = রিক্ত;

  cr_options_from_public (&cr, cr_options);

  ret = read_status_require_running (context, id, &status, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = read_container_config_from_state (&container, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  /* Needed to use the same paths (e.g. the shared empty directory used for
     masked paths) as the container runtime and restore do.  */
  container->context = context;

  ret = libcrun_container_checkpoint_linux (&status, container, &cr, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (! (cr_options->leave_running || cr_options->pre_dump))
    প্রত্যাগম container_delete_internal (context, রিক্ত, রিক্ত, id, true, true, err);

  প্রত্যাগম 0;
}

/* The dummy process only keeps the cgroup populated until the container is restored there.
   Its cgroup is below the container cgroup, so once the container runs it can freeze or kill
   the dummy process: never depend on it after that point.  */
স্থির শূন্য
restore_proxy_process (পূর্ণাঙ্ক proxy_pid_pipe)
{
  বর্ণ c;

  /* Wait to be killed by the parent, or for the parent to go away.  */
  TEMP_FAILURE_RETRY (পঠ (proxy_pid_pipe, &c, পরিমাণ (c)));
}

স্থির পূর্ণাঙ্ক
restore_terminate_proxy_process (pid_t proxy_pid, ধ্রুব বর্ণ *proxy_cgroup, pid_t container_pid, পূর্ণাঙ্ক cgroup_manager, libcrun_error_t *err)
{
  পূর্ণাঙ্ক mode;
  পূর্ণাঙ্ক ret;

  যদি (cgroup_manager == CGROUP_MANAGER_SYSTEMD)
    {
      বর্ণ ready_str[64];

      ret = snprintf (ready_str, পরিমাণ (ready_str), "MAINPID=%d", container_pid);
      যদি (UNLIKELY (ret >= (পূর্ণাঙ্ক) পরিমাণ (ready_str)))
        প্রত্যাগম crun_make_error (err, 0, "internal error: static buffer too small");

      ret = send_sd_notify (ready_str, err);
      /* Do not fail on errors.  */
      যদি (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (mode < 0))
    প্রত্যাগম mode;

  /* SIGKILL terminates a process frozen with cgroup v2, while the cgroup v1 freezer does not
     deliver it until the cgroup is thawed, so do not wait for it there.  */
  kill (proxy_pid, SIGKILL);
  TEMP_FAILURE_RETRY (waitpid (proxy_pid, রিক্ত, mode == CGROUP_MODE_UNIFIED ? 0 : WNOHANG));

  প্রত্যাগম destroy_cgroup_path (proxy_cgroup, mode, err);
}

পূর্ণাঙ্ক
libcrun_container_restore (libcrun_context_t *context, ধ্রুব বর্ণ *id arg_unused,
                           সংরচনা libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  cleanup_cgroup_status সংরচনা libcrun_cgroup_status *cgroup_status = রিক্ত;
  cleanup_container libcrun_container_t *container = রিক্ত;
  cleanup_close পূর্ণাঙ্ক proxy_pid_pipe0 = -1;
  cleanup_close পূর্ণাঙ্ক proxy_pid_pipe1 = -1;
  runtime_spec_schema_config_schema *def;
  libcrun_container_status_t status = {};
  সংরচনা libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  libcrun_checkpoint_restore_t cr;
  cleanup_pid pid_t proxy_pid = -1;
  cleanup_close পূর্ণাঙ্ক seccomp_fd = -1;
  ধ্রুব বর্ণ *seccomp_bpf_data;
  পূর্ণাঙ্ক proxy_pid_pipe[2];
  পূর্ণাঙ্ক cgroup_manager;
  uid_t মূল_uid = -1;
  gid_t মূল_gid = -1;
  পূর্ণাঙ্ক ret;

  cr_options_from_public (&cr, cr_options);

  container = libcrun_container_load_from_file ("config.json", err);
  যদি (container == রিক্ত)
    প্রত্যাগম -1;

  container->context = context;
  def = container->container_def;

  ret = check_config_file (def, context, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = libcrun_status_check_directories (context->state_মূল, context->id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = libcrun_copy_config_file (context->id, context->state_মূল, container, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ fail;

  /* CRIU restores the seccomp filters of the checkpointed processes, but
     exec loads the filter from the seccomp.bpf file in the state directory,
     so it must be generated again for the restored container.  */
  seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ fail;

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ fail;
  close_and_reset (&seccomp_fd);

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  যদি (context->systemd_cgroup)
    cgroup_manager = CGROUP_MANAGER_SYSTEMD;
  অন্যথা যদি (context->force_no_cgroup)
    cgroup_manager = CGROUP_MANAGER_DISABLED;

  def = container->container_def;

  /* If we are root (either on the host or in a namespace),
   * then chown the cgroup to root in the container user namespace. */
  get_মূল_in_the_userns (def, container->host_uid, container->host_gid, &মূল_uid, &মূল_gid);

  /* If the root in the container is different than the current root user, attempt to chown
     the std streams before entering the user namespace.  Otherwise we might lose access
     to the user (as it is not mapped in the user namespace) and cannot chown them.  */
  যদি (মূল_uid > 0 || মূল_gid > 0)
    {
      ret = maybe_chown_std_streams (মূল_uid, মূল_gid, err);
      যদি (UNLIKELY (ret < 0))
        গচ্ছ fail;
    }

  /* CRIU runs as a child process, so the restored processes would inherit the session keyring.  */
  ret = setup_container_keyring (container, context, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  {
    cleanup_close পূর্ণাঙ্ক cgroup_dirfd = -1;
    bool needs_proxy_process;
    cleanup_cgroup_preenter সংরচনা libcrun_cgroup_args cg = {
      .resources = def->linux ? def->linux->resources : রিক্ত,
      .annotations = container->annotations,
      .cgroup_path = def->linux ? def->linux->cgroups_path : "",
      .manager = cgroup_manager,
      .মূল_uid = মূল_uid,
      .মূল_gid = মূল_gid,
      .id = context->id,
      .state_মূল = context->state_মূল,
      .container = container,
    };

    /* The CRIU restore code uses bundle, rootfs and cgroup_path of status.   The cgroup_path is set later.  */
    status.bundle = (বর্ণ *) context->bundle;
    status.rootfs = def->মূল->path;

    ret = libcrun_cgroup_preenter (&cg, &cgroup_dirfd, err);
    যদি (UNLIKELY (ret < 0))
      গচ্ছ fail;

    needs_proxy_process = cgroup_dirfd < 0;

    /* If the target cgroup was already created (with cgroupfs), we can restore the container directly there.  */
    যদি (! needs_proxy_process)
      {
        cleanup_free বর্ণ *cgroup_path = রিক্ত;

        ret = get_cgroup_dirfd_path (cgroup_dirfd, &cgroup_path, err);
        যদি (UNLIKELY (ret < 0))
          গচ্ছ fail;

        /* Complete the configuration for the cgroup before the container is
           restored there: CRIU resumes the restored processes before it
           returns, so the device filter and the limits must already be in
           place.  There is no process to move, as CRIU is run in the cgroup,
           and the controllers were enabled by precreate_cgroup.  */
        cg.joined = true;

        ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
        যদি (UNLIKELY (ret < 0))
          গচ্ছ fail;

        /* Restore the container directly in the desired cgroup.  */
        status.cgroup_path = cgroup_path;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        যদি (UNLIKELY (ret < 0))
          গচ্ছ fail;

        /* Now that the container is restored, keep any signal for it, so that
           it is forwarded to the init by wait_for_process, rather than acted
           upon by crun.  This cannot be done earlier, as CRIU, which is run as
           a child process, would inherit the blocked signals.  */
        ret = block_signals (err);
        যদি (UNLIKELY (ret < 0))
          গচ্ছ fail;

        /* Use the container first process PID to finalize the cgroup.  */
        cg.pid = status.pid;
      }
    অন্যথা
      {
        /* When using systemd, we need a process first that must exist before a cgroup is created.
           Create a dummy process that sits and waits until the container is restored.  */
        ret = pipe2 (proxy_pid_pipe, O_CLOEXEC);
        যদি (UNLIKELY (ret < 0))
          {
            ret = crun_make_error (err, errno, "pipe");
            গচ্ছ fail;
          }

        /* Only used for auto cleanup.  */
        proxy_pid_pipe0 = proxy_pid_pipe[0];
        proxy_pid_pipe1 = proxy_pid_pipe[1];

        proxy_pid = fork ();
        যদি (UNLIKELY (proxy_pid < 0))
          {
            ret = crun_make_error (err, errno, "fork");
            গচ্ছ fail;
          }

        যদি (proxy_pid == 0)
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
        যদি (UNLIKELY (ret < 0))
          গচ্ছ fail;
      }

    ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
    যদি (UNLIKELY (ret < 0))
      গচ্ছ fail;

    /* When using a dummy process, the container is restored here once the cgroup is configured.  */
    যদি (needs_proxy_process)
      {
        cleanup_free বর্ণ *target_cgroup = রিক্ত;
        cleanup_free বর্ণ *proxy_cgroup = রিক্ত;

        ret = libcrun_get_cgroup_process (proxy_pid, &target_cgroup, false, err);
        যদি (UNLIKELY (ret < 0))
          গচ্ছ fail;

        /* Move the dummy process to a sub-cgroup.  */
        ret = append_paths (&proxy_cgroup, err, target_cgroup, ".proxy-crun", রিক্ত);
        যদি (UNLIKELY (ret < 0))
          গচ্ছ fail;

        ret = libcrun_move_process_to_cgroup (proxy_pid, 0, proxy_cgroup, true, err);
        যদি (UNLIKELY (ret < 0))
          গচ্ছ fail;

        /* Restore the container in the same cgroup where the dummy process was.  */
        status.cgroup_path = target_cgroup;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        যদি (UNLIKELY (ret < 0))
          গচ্ছ fail;

        /* See the comment for the other libcrun_container_restore_linux call.  */
        ret = block_signals (err);
        যদি (UNLIKELY (ret < 0))
          গচ্ছ fail;

        /* The container is running, so do not fail because of the dummy process.  */
        ret = restore_terminate_proxy_process (proxy_pid, proxy_cgroup, status.pid, cgroup_manager, err);
        যদি (UNLIKELY (ret < 0))
          crun_error_release (err);
        proxy_pid = -1;

        close_and_reset (&proxy_pid_pipe1);
      }
  }

  context->detach = cr_options->detach;
  ret = write_container_status (container, context, status.pid, cgroup_status, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ fail;

  {
    /* Wait the same way run and exec do: apart from waiting for the container
       init, this writes the pid file, forwards any signal to the init, and
       reaps every child, not only the init.  */
    সংরচনা wait_for_process_args args = {
      .pid = status.pid,
      .context = context,
      .terminal_fd = -1,
      .notify_socket = -1,
      .container_ready_fd = রিক্ত,
      .seccomp_notify_fd = -1,
      .seccomp_notify_plugins = রিক্ত,
    };

    প্রত্যাগম wait_for_process (&args, err);
  }

fail:
  /* Once restored, the container is running.  Without a status file nothing
     can reach it anymore, so do not leave it behind.  */
  যদি (status.pid > 0)
    {
      kill (status.pid, SIGKILL);
      waitpid_ignore_stopped (status.pid, রিক্ত, 0);
    }
  যদি (cgroup_status)
    {
      libcrun_error_t tmp_err = রিক্ত;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  {
    libcrun_error_t tmp_err = রিক্ত;

    যদি (libcrun_container_delete_status (context->state_মূল, context->id, &tmp_err) < 0)
      crun_error_release (&tmp_err);
  }
  প্রত্যাগম ret;
}

পূর্ণাঙ্ক
libcrun_container_read_pids (libcrun_context_t *context, ধ্রুব বর্ণ *id, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  cleanup_cgroup_status সংরচনা libcrun_cgroup_status *cgroup_status = রিক্ত;
  cleanup_container_status libcrun_container_status_t status = {};
  পূর্ণাঙ্ক ret;

  ret = libcrun_read_container_status (&status, context->state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (status.cgroup_path == রিক্ত || status.cgroup_path[0] == '\0')
    প্রত্যাগম crun_make_error (err, 0, "the container is not using cgroups");

  cgroup_status = libcrun_cgroup_make_status (&status);

  প্রত্যাগম libcrun_cgroup_read_pids (cgroup_status, recurse, pids, err);
}

পূর্ণাঙ্ক
libcrun_container_list_json (libcrun_context_t *context, বর্ণ **out, libcrun_error_t *err)
{
  libcrun_container_list_t *list = রিক্ত, *it;
  ধ্রুব বর্ণ *content = রিক্ত;
  json_gen_ctx *gen = রিক্ত;
  size_t len;
  পূর্ণাঙ্ক ret;

  *out = রিক্ত;

  ret = libcrun_get_containers_list (&list, context->state_মূল, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (! json_gen_init (&gen, রিক্ত))
    {
      ret = crun_make_error (err, 0, "cannot allocate json generator");
      গচ্ছ exit;
    }

  json_gen_config (gen, json_gen_beautify, 1);
  json_gen_array_open (gen);

  প্রতি (it = list; it; it = it->next)
    {
      libcrun_container_status_t status;
      পূর্ণাঙ্ক running = 0;
      পূর্ণাঙ্ক pid;
      ধ্রুব বর্ণ *container_status = রিক্ত;

      ret = libcrun_read_container_status (&status, context->state_মূল, it->name, err);
      যদি (UNLIKELY (ret < 0))
        গচ্ছ exit;

      pid = status.pid;

      ret = libcrun_get_container_state_string (it->name, &status, context->state_মূল, &container_status,
                                                &running, err);
      যদি (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          অনুবর্ত;
        }

      যদি (! running)
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
  যদি (json_gen_get_buf (gen, &content, &len) != json_gen_status_ok)
    {
      ret = libcrun_make_error (err, 0, "cannot generate json list");
      গচ্ছ exit;
    }

  *out = xstrdup (content);
  ret = 0;

exit:
  যদি (list)
    libcrun_free_containers_list (list);
  যদি (gen)
    json_gen_free (gen);

  প্রত্যাগম ret;
}

পূর্ণাঙ্ক
libcrun_write_json_containers_list (libcrun_context_t *context, FILE *out, libcrun_error_t *err)
{
  cleanup_free বর্ণ *content = রিক্ত;
  size_t len;
  পূর্ণাঙ্ক ret;

  ret = libcrun_container_list_json (context, &content, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  len = strlen (content);
  যদি (len && fwrite (content, 1, len, out) != len)
    প্রত্যাগম crun_make_error (err, errno, "error writing to file");

  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
libcrun_container_update_intel_rdt (libcrun_context_t *context, ধ্রুব বর্ণ *id, সংরচনা libcrun_intel_rdt_update *update, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = রিক্ত;
  পূর্ণাঙ্ক ret;

  ret = read_container_config_from_state (&container, context->state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  প্রত্যাগম libcrun_update_intel_rdt (id, container, update->l3_cache_schema, update->mem_bw_schema, update->schemata, err);
}

স্থির পূর্ণাঙ্ক
libcrun_container_add_or_remove_mounts_from_file (libcrun_context_t *context, ধ্রুব বর্ণ *id, ধ্রুব বর্ণ *file, bool add, libcrun_error_t *err)
{
  cleanup_custom_handler_instance সংরচনা custom_handler_instance_s *custom_handler = রিক্ত;
  cleanup_container libcrun_container_t *container = রিক্ত;
  cleanup_free runtime_spec_schema_defs_mount **mounts = রিক্ত;
  ধ্রুব বর্ণ *state_মূল = context->state_মূল;
  cleanup_free parser_error parser_err = রিক্ত;
  সংরচনা parser_context pctx = { 0, stderr };
  libcrun_container_status_t status = {};
  cleanup_free বর্ণ *content = রিক্ত;
  json_object *doc = রিক্ত;
  size_t n_mounts = 0, len, i;
  পূর্ণাঙ্ক ret = 1;

  ret = read_all_file (file, &content, &len, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = libcrun_read_container_status (&status, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = read_container_config_from_state (&container, state_মূল, id, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = parse_json_file (&doc, content, &pctx, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (! json_object_is_type (doc, json_type_array))
    {
      ret = crun_make_error (err, 0, "mounts must be an array");
      গচ্ছ cleanup;
    }
  অন্যথা
    {
      n_mounts = json_object_array_length (doc);

      mounts = xmalloc0 ((n_mounts + 1) * পরিমাণ (*mounts));
      প্রতি (i = 0; i < n_mounts; i++)
        {
          mounts[i] = make_runtime_spec_schema_defs_mount (json_object_array_get_idx (doc, i), &pctx, &parser_err);
          যদি (mounts[i] == রিক্ত)
            {
              ret = crun_make_error (err, 0, "cannot parse mount: %s", parser_err);
              গচ্ছ cleanup;
            }
        }
    }

  যদি (add)
    ret = libcrun_make_runtime_mounts (container, &status, mounts, n_mounts, err);
  অন্যথা
    ret = libcrun_destroy_runtime_mounts (container, &status, mounts, n_mounts, err);

cleanup:
  যদি (doc)
    json_object_put (doc);
  যদি (mounts)
    প্রতি (i = 0; i < n_mounts; i++)
      free_runtime_spec_schema_defs_mount (mounts[i]);
  প্রত্যাগম ret;
}

পূর্ণাঙ্ক
libcrun_container_add_mounts_from_file (libcrun_context_t *context, ধ্রুব বর্ণ *id, ধ্রুব বর্ণ *file, libcrun_error_t *err)
{
  প্রত্যাগম libcrun_container_add_or_remove_mounts_from_file (context, id, file, true, err);
}

পূর্ণাঙ্ক
libcrun_container_remove_mounts_from_file (libcrun_context_t *context, ধ্রুব বর্ণ *id, ধ্রুব বর্ণ *file, libcrun_error_t *err)
{
  প্রত্যাগম libcrun_container_add_or_remove_mounts_from_file (context, id, file, false, err);
}
