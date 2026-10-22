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
#घोषणा _GNU_SOURCE

#समावेश <config.h>
#समावेश <ocispec/runtime_spec_schema_config_schema.h>
#समावेश <stdbool.h>
#समावेश "container.h"
#समावेश "utils.h"
#समावेश "json_gen_utils.h"
#समावेश "seccomp.h"
#समावेश "mempolicy.h"
#अगर_घोषित HAVE_SECCOMP
#  include <seccomp.h>
#पूर्ण_अगर
#समावेश "scheduler.h"
#समावेश "seccomp_notify.h"
#समावेश "custom-handler.h"
#समावेश <stdbool.h>
#समावेश <argp.h>
#समावेश <unistd.h>
#समावेश <sys/stat.h>
#समावेश <sys/types.h>
#समावेश <sched.h>
#समावेश <sys/wait.h>
#समावेश <string.h>
#समावेश <fcntl.h>
#समावेश "status.h"
#समावेश "mount_flags.h"
#समावेश "linux.h"
#समावेश "terminal.h"
#समावेश "io_priority.h"
#समावेश "cgroup.h"
#समावेश "cgroup-utils.h"
#समावेश <sys/prctl.h>
#समावेश <sys/auxv.h>
#समावेश <sys/signalfd.h>
#समावेश <sys/epoll.h>
#समावेश <sys/socket.h>
#अगर_घोषित HAVE_CAP
#  include <sys/capability.h>
#पूर्ण_अगर
#समावेश <sys/ioctl.h>
#समावेश <termios.h>
#समावेश <grp.h>
#समावेश <git-version.h>

#अगर_घोषित HAVE_SYSTEMD
#  include <systemd/sd-daemon.h>
#पूर्ण_अगर

#समावेश <json-c/json.h>
#समावेश <ocispec/json_common.h>

क्रमागत
{
  SYNC_SOCKET_SYNC_MESSAGE,
  SYNC_SOCKET_ERROR_MESSAGE,
  SYNC_SOCKET_WARNING_MESSAGE,
  SYNC_SOCKET_DEBUG_MESSAGE,
};

काष्ठा container_entrypoint_s
{
  libcrun_container_t *container;
  libcrun_context_t *context;
  पूर्णांक has_terminal_socket_pair;
  पूर्णांक terminal_socketpair[2];

  /* Used by log_write_to_sync_socket.  */
  पूर्णांक sync_socket;

  पूर्णांक seccomp_fd;
  पूर्णांक seccomp_receiver_fd;
  पूर्णांक console_socket_fd;

  पूर्णांक hooks_out_fd;
  पूर्णांक hooks_err_fd;

  /* Absolute bundle path, to be reported to hooks run in the container.  */
  स्थिर अक्षर *bundle;

  काष्ठा custom_handler_instance_s *custom_handler;
};

काष्ठा sync_socket_message_s
{
  पूर्णांक type;
  पूर्णांक error_मूल्य;
  अक्षर message[512];
};

प्रकार runtime_spec_schema_defs_hook hook;

// linux hooks
अक्षर *hooks[] = {
  "prestart",
  "createRuntime",
  "createContainer",
  "startContainer",
  "poststart",
  "poststop"
};

// linux namespaces
अटल अक्षर *namespaces[] = {
  "cgroup",
  "ipc",
  "mount",
  "network",
  "pid",
  "user",
  "uts"
};

अटल अक्षर *actions[] = {
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

अटल अक्षर *operators[] = {
  "SCMP_CMP_NE",
  "SCMP_CMP_LT",
  "SCMP_CMP_LE",
  "SCMP_CMP_EQ",
  "SCMP_CMP_GE",
  "SCMP_CMP_GT",
  "SCMP_CMP_MASKED_EQ",
};

अटल अक्षर *archs[] = {
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

अटल अक्षर *mempolicy_modes[] = {
  "MPOL_DEFAULT",
  "MPOL_PREFERRED",
  "MPOL_BIND",
  "MPOL_INTERLEAVE",
  "MPOL_LOCAL",
  "MPOL_PREFERRED_MANY",
  "MPOL_WEIGHTED_INTERLEAVE"
};

अटल अक्षर *mempolicy_flags[] = {
  "MPOL_F_NUMA_BALANCING",
  "MPOL_F_RELATIVE_NODES",
  "MPOL_F_STATIC_NODES"
};

अटल अक्षर *potentially_unsafe_annotations[] = {
  "module.wasm.image/variant",
  "io.kubernetes.cri.container-type",
  "run.oci.",
  "org.criu.",
  "krun.",
};

#घोषणा SYNC_SOCKET_MESSAGE_LEN(x, l) (दुरत्व (काष्ठा sync_socket_message_s, message) + l)

अटल पूर्णांक
sync_socket_write_संदेश (पूर्णांक fd, पूर्णांक verbosity, पूर्णांक err_मूल्य, स्थिर अक्षर *log_संदेश)
{
  पूर्णांक ret;
  माप_प्रकार err_len;
  काष्ठा sync_socket_message_s संदेश;
  चयन (verbosity)
    {
    हाल LIBCRUN_VERBOSITY_DEBUG:
      संदेश.type = SYNC_SOCKET_DEBUG_MESSAGE;
      अवरोध;
    हाल LIBCRUN_VERBOSITY_WARNING:
      संदेश.type = SYNC_SOCKET_WARNING_MESSAGE;
      अवरोध;
    हाल LIBCRUN_VERBOSITY_ERROR:
      संदेश.type = SYNC_SOCKET_ERROR_MESSAGE;
      अवरोध;
    }
  संदेश.error_मूल्य = err_मूल्य;

  अगर (fd < 0)
    वापस 0;

  err_len = म_माप (log_संदेश);
  अगर (err_len >= माप (संदेश.message))
    err_len = माप (संदेश.message) - 1;

  स_नकल (संदेश.message, log_संदेश, err_len);
  संदेश.message[err_len] = '\0';

  ret = अस्थायी_FAILURE_RETRY (ग_लिखो (fd, &संदेश, SYNC_SOCKET_MESSAGE_LEN (संदेश, err_len + 1)));
  अगर (UNLIKELY (ret < 0))
    वापस -1;

  वापस 0;
}

अटल पूर्णांक
sync_socket_write_error (पूर्णांक fd, libcrun_error_t *out_err)
{
  अगर (fd < 0)
    वापस 0;
  वापस sync_socket_write_संदेश (fd, false, (*out_err)->status, (*out_err)->संदेश);
}

अटल व्योम
log_write_to_sync_socket (पूर्णांक errno_, स्थिर अक्षर *संदेश, पूर्णांक verbosity, व्योम *arg)
{
  काष्ठा container_entrypoint_s *entrypoint_args = arg;
  पूर्णांक fd = entrypoint_args->sync_socket;

  अगर (fd < 0)
    वापस;

  अगर (sync_socket_write_संदेश (fd, verbosity, errno_, संदेश) < 0)
    log_write_to_stderr (errno_, संदेश, verbosity, arg);
}

अटल bool
is_memory_limit_too_निचला (runtime_spec_schema_config_schema *def)
{
  स्थिर दीर्घ memory_limit_too_निचला = 1024 * 1024;

  अगर (def->linux == शून्य || def->linux->resources == शून्य)
    वापस false;

  अगर (def->linux->resources->memory
      && def->linux->resources->memory->limit_present
      && def->linux->resources->memory->limit < memory_limit_too_निचला)
    वापस true;

  अगर (def->linux->resources->unified)
    {
      माप_प्रकार i;

      क्रम (i = 0; i < def->linux->resources->unified->len; i++)
        अगर (म_भेद (def->linux->resources->unified->कुंजियाँ[i], "memory.max") == 0)
          {
            दीर्घ limit;

            त्रुटि_सं = 0;
            limit = म_से_दीर्घ (def->linux->resources->unified->मूल्यावली[i], शून्य, 10);
            अगर (त्रुटि_सं != 0)
              वापस false;
            अगर (limit < memory_limit_too_निचला)
              वापस true;
          }
    }

  वापस false;
}

अटल पूर्णांक
sync_socket_wait_sync (libcrun_context_t *context, पूर्णांक fd, bool flush, libcrun_error_t *err)
{
  काष्ठा sync_socket_message_s संदेश;

  अगर (fd < 0)
    वापस 0;

  जबतक (true)
    {
      पूर्णांक ret;

      ret = अस्थायी_FAILURE_RETRY (पढ़ो (fd, &संदेश, माप (संदेश)));
      अगर (UNLIKELY (ret < 0))
        {
          अगर (flush)
            वापस 0;
          वापस crun_make_error (err, त्रुटि_सं, "read from sync socket");
        }

      अगर (ret == 0)
        {
          अगर (flush)
            वापस 0;

          वापस crun_make_error (err, 0, "read from the init process");
        }

      अगर (! flush && संदेश.type == SYNC_SOCKET_SYNC_MESSAGE)
        वापस 0;
      अन्यथा अगर (संदेश.type == SYNC_SOCKET_DEBUG_MESSAGE)
        {
          अगर (context)
            context->output_handler (संदेश.error_मूल्य, संदेश.message, LIBCRUN_VERBOSITY_DEBUG, context->output_handler_arg);
          जारी;
        }
      अन्यथा अगर (संदेश.type == SYNC_SOCKET_WARNING_MESSAGE)
        {
          अगर (context)
            context->output_handler (संदेश.error_मूल्य, संदेश.message, LIBCRUN_VERBOSITY_WARNING, context->output_handler_arg);
          जारी;
        }
      अन्यथा अगर (संदेश.type == SYNC_SOCKET_ERROR_MESSAGE)
        वापस crun_make_error (err, संदेश.error_मूल्य, "%s", संदेश.message);
    }
}

अटल पूर्णांक
sync_socket_send_sync (पूर्णांक fd, bool flush_errors, libcrun_error_t *err)
{
  पूर्णांक ret;
  काष्ठा sync_socket_message_s संदेश = {
    0,
  };
  संदेश.type = SYNC_SOCKET_SYNC_MESSAGE;

  अगर (fd < 0)
    वापस 0;

  ret = अस्थायी_FAILURE_RETRY (ग_लिखो (fd, &संदेश, SYNC_SOCKET_MESSAGE_LEN (संदेश, 0)));
  अगर (UNLIKELY (ret < 0))
    {
      अगर (flush_errors)
        {
          पूर्णांक saved_errno = त्रुटि_सं;
          ret = अस्थायी_FAILURE_RETRY (पढ़ो (fd, &संदेश, माप (संदेश)));
          अगर (ret >= 0 && संदेश.type == SYNC_SOCKET_ERROR_MESSAGE)
            वापस crun_make_error (err, संदेश.error_मूल्य, "%s", संदेश.message);
          त्रुटि_सं = saved_errno;
        }
      वापस crun_make_error (err, त्रुटि_सं, "write to sync socket");
    }

  वापस 0;
}

अटल bool
has_cap_sys_ptrace ()
{
#अगर_घोषित HAVE_CAP
  काष्ठा __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  काष्ठा __user_cap_data_struct तथ्य[2] = { { 0 } };

  अगर (UNLIKELY (capget (&hdr, तथ्य) < 0))
    वापस true;

  वापस (तथ्य[0].effective & (1U << CAP_SYS_PTRACE)) != 0;
#अन्यथा
  वापस geteuid () == 0;
#पूर्ण_अगर
}

अटल libcrun_container_t *
make_container (runtime_spec_schema_config_schema *container_def, स्थिर अक्षर *path, स्थिर अक्षर *config)
{
  libcrun_container_t *container = xmalloc0 (माप (*container));
  container->container_def = container_def;

  container->host_uid = geteuid ();
  container->host_gid = getegid ();
  container->host_has_cap_sys_ptrace = has_cap_sys_ptrace ();
  container->proc_fd = -1;

  container->annotations = make_string_map_from_json (container_def->annotations);

  /* An empty namespace path means that a new namespace is to be created,
     the same as if it is not specified.  */
  अगर (container_def->linux)
    {
      माप_प्रकार i;

      क्रम (i = 0; i < container_def->linux->namespaces_len; i++)
        अगर (container_def->linux->namespaces[i]->path && container_def->linux->namespaces[i]->path[0] == '\0')
          {
            मुक्त (container_def->linux->namespaces[i]->path);
            container_def->linux->namespaces[i]->path = शून्य;
          }
    }

  अगर (path)
    container->config_file = xstrdup (path);
  अगर (config)
    container->config_file_content = xstrdup (config);

  वापस container;
}

libcrun_container_t *
libcrun_container_load_from_memory (स्थिर अक्षर *json, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free अक्षर *oci_error = शून्य;
  container_def = runtime_spec_schema_config_schema_parse_तथ्य (json, शून्य, &oci_error);
  अगर (container_def == शून्य)
    {
      crun_make_error (err, 0, "load: `%s`", oci_error);
      वापस शून्य;
    }
  वापस make_container (container_def, शून्य, json);
}

libcrun_container_t *
libcrun_container_load_from_file (स्थिर अक्षर *path, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free अक्षर *oci_error = शून्य;
  cleanup_free अक्षर *content = शून्य;
  माप_प्रकार len;
  पूर्णांक ret;

  libcrun_debug ("Loading container from config file: `%s`", path);
  ret = read_all_file (path, &content, &len, err);
  अगर (UNLIKELY (ret < 0))
    वापस शून्य;

  container_def = runtime_spec_schema_config_schema_parse_तथ्य (content, शून्य, &oci_error);
  अगर (container_def == शून्य)
    {
      crun_make_error (err, 0, "load `%s`: %s", path, oci_error);
      वापस शून्य;
    }
  वापस make_container (container_def, शून्य, content);
}

स्थिर अक्षर *
libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err)
{
  काष्ठा parser_context ctx = { 0, मानक_त्रुटि };
  parser_error gen_err = शून्य;

  अगर (container == शून्य)
    {
      crun_make_error (err, EINVAL, "no container specified");
      वापस शून्य;
    }

  अगर (container->config_file_content == शून्य && container->container_def)
    {
      container->config_file_content = runtime_spec_schema_config_schema_generate_json (container->container_def, &ctx, &gen_err);
      अगर (container->config_file_content == शून्य)
        {
          crun_make_error (err, 0, "cannot generate the configuration JSON: %s", gen_err ?: "unknown error");
          मुक्त (gen_err);
          वापस शून्य;
        }
      मुक्त (gen_err);
    }

  अगर (container->config_file_content == शून्य)
    {
      crun_make_error (err, 0, "the container has no configuration");
      वापस शून्य;
    }

  वापस container->config_file_content;
}

स्थिर अक्षर *
libcrun_container_get_annotation (libcrun_container_t *container, स्थिर अक्षर *कुंजी)
{
  अगर (container == शून्य)
    वापस शून्य;
  वापस find_string_map_मूल्य (container->annotations, कुंजी);
}

माप_प्रकार
libcrun_container_get_annotations_len (libcrun_container_t *container)
{
  अगर (container == शून्य || container->annotations == शून्य)
    वापस 0;
  वापस string_map_आकार (container->annotations);
}

पूर्णांक
libcrun_container_get_annotation_at (libcrun_container_t *container, माप_प्रकार सूचकांक, स्थिर अक्षर **कुंजी,
                                     स्थिर अक्षर **मूल्य)
{
  अगर (container == शून्य)
    {
      त्रुटि_सं = दुस्फल;
      वापस -1;
    }
  वापस string_map_get_at (container->annotations, सूचकांक, कुंजी, मूल्य);
}

uid_t
libcrun_container_get_uid (libcrun_container_t *container)
{
  अगर (container == शून्य)
    वापस (uid_t) -1;
  वापस container->host_uid;
}

gid_t
libcrun_container_get_gid (libcrun_container_t *container)
{
  अगर (container == शून्य)
    वापस (gid_t) -1;
  वापस container->host_gid;
}

व्योम
libcrun_container_free (libcrun_container_t *ctr)
{
  अगर (ctr == शून्य)
    वापस;

  अगर (ctr->cleanup_private_तथ्य)
    ctr->cleanup_private_तथ्य (ctr->private_तथ्य);

  अगर (ctr->container_def)
    free_runtime_spec_schema_config_schema (ctr->container_def);

  free_string_map (ctr->annotations);

  अगर (ctr->proc_fd >= 0)
    बंद (ctr->proc_fd);

  मुक्त (ctr->config_file_content);
  मुक्त (ctr->config_file);
  मुक्त (ctr);
}

libcrun_context_t *
libcrun_context_new (स्थिर अक्षर *id, स्थिर अक्षर *state_मूल, libcrun_error_t *err)
{
  libcrun_context_t *ctx = xmalloc0 (माप (*ctx));

  /* Go through the setters so that the owned_* copies and the borrowed
     pointers stay in sync and are released by libcrun_context_free.  */
  libcrun_context_set_id (ctx, id);
  libcrun_context_set_state_मूल (ctx, state_मूल);
  libcrun_context_set_bundle (ctx, ".");
  ctx->fifo_exec_wait_fd = -1;

  अगर (libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, id, शून्य, err) < 0)
    {
      libcrun_context_free (ctx);
      वापस शून्य;
    }

  ctx->handler_manager = libcrun_handler_manager_create (err);
  अगर (ctx->handler_manager == शून्य)
    {
      libcrun_context_free (ctx);
      वापस शून्य;
    }

#अगर_घोषित CRUN_LIBDIR
  अगर (access (CRUN_LIBDIR "/handlers", F_OK) == 0)
    {
      अगर (libcrun_handler_manager_load_directory (ctx->handler_manager, CRUN_LIBDIR "/handlers", err) < 0)
        {
          libcrun_context_free (ctx);
          वापस शून्य;
        }
    }
#पूर्ण_अगर

  वापस ctx;
}

व्योम
libcrun_context_free (libcrun_context_t *ctx)
{
  अगर (ctx == शून्य)
    वापस;

  मुक्त (ctx->owned_state_मूल);
  मुक्त (ctx->owned_id);
  मुक्त (ctx->owned_bundle);
  मुक्त (ctx->owned_console_socket);
  मुक्त (ctx->owned_pid_file);
  मुक्त (ctx->owned_notify_socket);
  मुक्त (ctx->owned_handler);

  अगर (ctx->handler_manager)
    handler_manager_free (ctx->handler_manager);

  मुक्त (ctx);
}

#घोषणा LIBCRUN_CONTEXT_SET_STRING(field)                  \
  करो                                                       \
    {                                                      \
      मुक्त (ctx->owned_##field);                           \
      ctx->owned_##field = मूल्य ? xstrdup (मूल्य) : शून्य; \
      ctx->field = ctx->owned_##field;                     \
  } जबतक (0)

व्योम
libcrun_context_set_id (libcrun_context_t *ctx, स्थिर अक्षर *मूल्य)
{
  LIBCRUN_CONTEXT_SET_STRING (id);
}

व्योम
libcrun_context_set_state_मूल (libcrun_context_t *ctx, स्थिर अक्षर *मूल्य)
{
  LIBCRUN_CONTEXT_SET_STRING (state_मूल);
}

व्योम
libcrun_context_set_bundle (libcrun_context_t *ctx, स्थिर अक्षर *मूल्य)
{
  LIBCRUN_CONTEXT_SET_STRING (bundle);
}

व्योम
libcrun_context_set_console_socket (libcrun_context_t *ctx, स्थिर अक्षर *मूल्य)
{
  LIBCRUN_CONTEXT_SET_STRING (console_socket);
}

व्योम
libcrun_context_set_pid_file (libcrun_context_t *ctx, स्थिर अक्षर *मूल्य)
{
  LIBCRUN_CONTEXT_SET_STRING (pid_file);
}

व्योम
libcrun_context_set_notify_socket (libcrun_context_t *ctx, स्थिर अक्षर *मूल्य)
{
  LIBCRUN_CONTEXT_SET_STRING (notify_socket);
}

व्योम
libcrun_context_set_handler (libcrun_context_t *ctx, स्थिर अक्षर *मूल्य)
{
  LIBCRUN_CONTEXT_SET_STRING (handler);
}

#अघोषित LIBCRUN_CONTEXT_SET_STRING

स्थिर अक्षर *
libcrun_context_get_pid_file (libcrun_context_t *ctx)
{
  वापस ctx->pid_file;
}

स्थिर अक्षर *
libcrun_context_get_state_मूल (libcrun_context_t *ctx)
{
  अगर (ctx == शून्य)
    वापस शून्य;
  वापस ctx->state_मूल;
}

व्योम
libcrun_context_set_preserve_fds (libcrun_context_t *ctx, पूर्णांक preserve_fds)
{
  ctx->preserve_fds = preserve_fds;
}

पूर्णांक
libcrun_context_get_preserve_fds (libcrun_context_t *ctx)
{
  वापस ctx->preserve_fds;
}

व्योम
libcrun_context_set_listen_fds (libcrun_context_t *ctx, पूर्णांक listen_fds)
{
  ctx->listen_fds = listen_fds;
}

व्योम
libcrun_context_set_args (libcrun_context_t *ctx, पूर्णांक argc, अक्षर **argv)
{
  ctx->argc = argc;
  ctx->argv = argv;
}

व्योम
libcrun_context_set_systemd_cgroup (libcrun_context_t *ctx, bool मूल्य)
{
  ctx->systemd_cgroup = मूल्य;
}

व्योम
libcrun_context_set_detach (libcrun_context_t *ctx, bool मूल्य)
{
  ctx->detach = मूल्य;
}

व्योम
libcrun_context_set_no_new_keyring (libcrun_context_t *ctx, bool मूल्य)
{
  ctx->no_new_keyring = मूल्य;
}

व्योम
libcrun_context_set_no_pivot (libcrun_context_t *ctx, bool मूल्य)
{
  ctx->no_pivot = मूल्य;
}

व्योम
libcrun_context_set_force_no_cgroup (libcrun_context_t *ctx, bool मूल्य)
{
  ctx->force_no_cgroup = मूल्य;
}

व्योम
libcrun_context_set_output_handler (libcrun_context_t *ctx, crun_output_handler handler, व्योम *arg)
{
  ctx->output_handler = handler;
  ctx->output_handler_arg = arg;
}

पूर्णांक
libcrun_context_init_logging (libcrun_context_t *ctx, स्थिर अक्षर *log, libcrun_error_t *err)
{
  वापस libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, ctx->id, log, err);
}

अटल पूर्णांक
block_signals (libcrun_error_t *err)
{
  पूर्णांक ret;
  sigset_t mask;
  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "sigprocmask");
  वापस 0;
}

अटल पूर्णांक
unblock_signals (libcrun_error_t *err)
{
  पूर्णांक i;
  पूर्णांक ret;
  sigset_t mask;
  काष्ठा sigaction act = {};

  sigfillset (&mask);
  ret = sigprocmask (SIG_UNBLOCK, &mask, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "sigprocmask");

  act.sa_handler = संक_पूर्व;
  क्रम (i = 0; i < NSIG; i++)
    {
      ret = sigaction (i, &act, शून्य);
      अगर (ret < 0 && त्रुटि_सं != EINVAL)
        वापस crun_make_error (err, त्रुटि_सं, "sigaction");
    }

  वापस 0;
}

/* must be used on the host before pivot_root(2).  */
अटल पूर्णांक
initialize_security (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = libcrun_init_caps (container, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (UNLIKELY (proc == शून्य))
    वापस 0;

  अगर (proc->apparmor_profile)
    {
      ret = libcrun_initialize_apparmor (err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  ret = libcrun_initialize_selinux (container, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस 0;
}

/* Emit the "annotations" map.  Returns a json_gen_status; callers can wrap
   the call with GEN_OR_FAIL.  */
अटल पूर्णांक
gen_annotations (json_gen_ctx *gen, json_map_string_string *annotations)
{
  माप_प्रकार i;
  पूर्णांक r;

  GEN_कुंजी (gen, "annotations");
  GEN_OR_FAIL (json_gen_map_open (gen));

  क्रम (i = 0; i < annotations->len; i++)
    {
      /* An annotation whose value is null in the document is parsed into a
         NULL, which cannot be emitted as a JSON string; leave it out.  */
      अगर (annotations->कुंजियाँ[i] == शून्य || annotations->मूल्यावली[i] == शून्य)
        जारी;

      GEN_STR (gen, annotations->कुंजियाँ[i]);
      GEN_STR (gen, annotations->मूल्यावली[i]);
    }

  वापस json_gen_map_close (gen);

gen_error:
  वापस r;
}

/* Return the bundle path to report in the container state.  crun always
   chdir()s into the bundle, so fall back to getcwd() when the caller has no
   absolute bundle path, as can happen when libcrun is used as a library.
   *ALLOCATED must be freed by the caller.  */
अटल स्थिर अक्षर *
get_bundle_path (स्थिर अक्षर *bundle, अक्षर **allocated)
{
  अगर (bundle != शून्य && bundle[0] == '/')
    वापस bundle;

  *allocated = getcwd (शून्य, 0);
  अगर (*allocated == शून्य)
    OOM ();
  वापस *allocated;
}

अटल पूर्णांक
do_hooks (runtime_spec_schema_config_schema *def, pid_t pid, स्थिर अक्षर *id, bool keep_going, स्थिर अक्षर *bundle,
          स्थिर अक्षर *cwd, स्थिर अक्षर *status, स्थिर अक्षर *name, hook **hooks, माप_प्रकार hooks_len, पूर्णांक out_fd,
          पूर्णांक err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  माप_प्रकार i, stdin_len;
  पूर्णांक r, ret;
  अक्षर *मानक_निवेश = शून्य;
  cleanup_free अक्षर *bundle_allocated = शून्य;
  स्थिर अक्षर *rootfs = def->मूल ? def->मूल->path : "";
  cleanup_json_gen json_gen_ctx *gen = शून्य;

  bundle = get_bundle_path (bundle, &bundle_allocated);

  अगर (! json_gen_init (&gen, शून्य))
    वापस crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 0);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_कुंजी (gen, "ociVersion");
  GEN_STR (gen, "1.0");

  GEN_कुंजी (gen, "id");
  GEN_STR (gen, id);

  GEN_कुंजी (gen, "pid");
  GEN_OR_FAIL (map_int (gen, pid));

  GEN_कुंजी (gen, "root");
  GEN_STR (gen, rootfs);

  GEN_कुंजी (gen, "bundle");
  GEN_STR (gen, bundle);

  GEN_कुंजी (gen, "status");
  GEN_STR (gen, status);

  अगर (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, (स्थिर अक्षर **) &मानक_निवेश, &stdin_len));

  ret = 0;

  bool error_created = false;
  क्रम (i = 0; i < hooks_len; i++)
    {
      अक्षर **env = environ;

      अगर (hooks[i]->env)
        env = hooks[i]->env;

      /* Release the error from the previous iteration, if any.  */
      अगर (error_created)
        {
          crun_error_release (err);
          error_created = false;
        }

      ret = run_process_with_stdin_timeout_envp (hooks[i]->path, hooks[i]->args, cwd, hooks[i]->timeout, env,
                                                 मानक_निवेश, stdin_len, out_fd, err_fd, can_ignore_chdir_errors, err);
      अगर (UNLIKELY (ret < 0))
        error_created = true;

      अगर (UNLIKELY (ret != 0))
        {
          अगर (keep_going)
            libcrun_warning ("error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
          अन्यथा
            {
              libcrun_error (0, "error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
              अवरोध;
            }
        }
    }

  वापस ret;

gen_error:
  वापस json_gen_error_to_crun_error (r, err);
}

अटल पूर्णांक
get_json_gen_परिणाम (json_gen_ctx *gen, अक्षर **out, माप_प्रकार *out_len)
{
  स्थिर अक्षर *buf = शून्य;
  माप_प्रकार buf_len = 0;
  पूर्णांक r;

  r = json_gen_get_buf (gen, &buf, &buf_len);
  अगर (UNLIKELY (r != json_gen_status_ok))
    वापस r;

  *out_len = buf_len;

  *out = xmalloc (buf_len + 1);
  स_नकल (*out, buf, buf_len);
  (*out)[buf_len] = '\0';

  वापस json_gen_status_ok;
}

अटल पूर्णांक
get_seccomp_receiver_fd_payload (libcrun_container_t *container, स्थिर अक्षर *status, pid_t own_pid,
                                 अक्षर **seccomp_fd_payload, माप_प्रकार *seccomp_fd_payload_len, libcrun_error_t *err)
{
  पूर्णांक r;
  json_gen_ctx *gen = शून्य;
  runtime_spec_schema_config_schema *def = container->container_def;
  स्थिर अक्षर *स्थिर OCI_VERSION = "0.2.0";

  अगर (! json_gen_init (&gen, शून्य))
    वापस crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_कुंजी (gen, "ociVersion");
  GEN_STR (gen, OCI_VERSION);

  GEN_कुंजी (gen, "fds");
  GEN_OR_FAIL (json_gen_array_open (gen));

  GEN_कुंजी (gen, "seccompFd");
  GEN_OR_FAIL (json_gen_array_close (gen));

  GEN_कुंजी (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  अगर (def && def->linux && def->linux->seccomp)
    {
      स्थिर अक्षर *metadata = def->linux->seccomp->listener_metadata;

      अगर (metadata)
        {
          GEN_कुंजी (gen, "metadata");
          GEN_STR (gen, metadata);
        }
    }

  /* State.  */
  GEN_कुंजी (gen, "state");
  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_कुंजी (gen, "ociVersion");
  GEN_STR (gen, OCI_VERSION);

  अगर (container->context && container->context->id)
    {
      GEN_कुंजी (gen, "id");
      GEN_STR (gen, container->context->id);
    }

  GEN_कुंजी (gen, "status");
  GEN_STR (gen, status);

  GEN_कुंजी (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  अगर (container->context && container->context->bundle)
    {
      GEN_कुंजी (gen, "bundle");
      GEN_STR (gen, container->context->bundle);
    }

  अगर (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));
  /* End state.  */

  GEN_OR_FAIL (json_gen_map_close (gen));

  r = get_json_gen_परिणाम (gen, seccomp_fd_payload, seccomp_fd_payload_len);

gen_error:
  json_gen_free (gen);

  वापस json_gen_error_to_crun_error (r, err);
}

अटल पूर्णांक
send_sync_cb (व्योम *तथ्य, libcrun_error_t *err)
{
  पूर्णांक sync_socket_fd = *((पूर्णांक *) तथ्य);
  पूर्णांक ret;

  /* sync 2.  */
  ret = sync_socket_send_sync (sync_socket_fd, false, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  /* sync 3.  */
  वापस sync_socket_wait_sync (शून्य, sync_socket_fd, false, err);
}

अटल पूर्णांक
maybe_chown_std_streams (uid_t container_uid, gid_t container_gid, libcrun_error_t *err)
{
  पूर्णांक ret, i;

  क्रम (i = 0; i < 3; i++)
    {
      अगर (! isatty (i))
        {
          काष्ठा stat statbuf;
          ret = ख_स्थिति (i, &statbuf);
          अगर (UNLIKELY (ret < 0))
            {
              अगर (त्रुटि_सं == EBADF)
                जारी;
              वापस crun_make_error (err, त्रुटि_सं, "fstat fd `%d`", i);
            }

          /* Skip chown for device files */
          अगर (S_ISCHR (statbuf.st_mode) || S_ISBLK (statbuf.st_mode))
            जारी;

          ret = fchown (i, container_uid, container_gid);
          अगर (UNLIKELY (ret < 0))
            {
              /* EINVAL means the user is not mapped in the current userns.
                 Ignore EPERM and EROFS as well as there is no reason to fail
                 so early, and let the container payload deal with it.
                 EBADF means fd is closed.
              */
              अगर (त्रुटि_सं == EINVAL || त्रुटि_सं == EPERM || त्रुटि_सं == EROFS || त्रुटि_सं == EBADF)
                जारी;

              वापस crun_make_error (err, त्रुटि_सं, "fchown std stream `%i`", i);
            }
        }
    }
  वापस 0;
}

पूर्णांक
libcrun_container_notify_handler (काष्ठा container_entrypoint_s *args,
                                  क्रमागत handler_configure_phase phase,
                                  libcrun_container_t *container, स्थिर अक्षर *rootfs,
                                  libcrun_error_t *err)
{
  काष्ठा custom_handler_s *h;

  अगर (args->custom_handler == शून्य)
    वापस 0;

  h = args->custom_handler->vtable;
  अगर (h == शून्य || h->configure_container == शून्य)
    वापस 0;

  वापस h->configure_container (args->custom_handler->cookie, phase,
                                 args->context, container,
                                 rootfs, err);
}

/* Resolve and normalize the container rootfs path.  */
अटल पूर्णांक
resolve_rootfs_path (libcrun_container_t *container, अक्षर **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  अगर (def->मूल && def->मूल->path)
    {
      *rootfs = realpath (def->मूल->path, शून्य);
      अगर (UNLIKELY (*rootfs == शून्य))
        {
          पूर्णांक saved_errno = त्रुटि_सं;

          /* If realpath failed for any reason, try the relative directory.  */
          अगर (def->मूल->path[0] == '/')
            {
              cleanup_free अक्षर *cwd = शून्य;
              libcrun_error_t tmp_err = शून्य;
              ssize_t len;
              पूर्णांक proc_fd;

              proc_fd = libcrun_get_cached_proc_fd (container, &tmp_err);
              अगर (LIKELY (proc_fd >= 0))
                {
                  len = safe_readlinkat (proc_fd, "self/cwd", &cwd, 0, &tmp_err);
                  अगर (LIKELY (len > 0))
                    {
                      /* If the rootfs is under the current working directory, just use its relative path.  */
                      अगर (has_dir_prefix (def->मूल->path, cwd))
                        {
                          स्थिर अक्षर *it = consume_slashes (def->मूल->path + len);
                          अगर (LIKELY (*it))
                            {
                              पूर्णांक ret = faccessat (AT_FDCWD, it, F_OK, 0);
                              अगर (LIKELY (ret == 0))
                                *rootfs = xstrdup (it);
                            }
                        }
                    }
                }
              crun_error_release (&tmp_err);
            }

          अगर (*rootfs == शून्य)
            {
              अगर (saved_errno == ENOENT)
                वापस crun_make_error (err, ENOENT, "the rootfs path `%s` does not exist", def->मूल->path);

              *rootfs = xstrdup (def->मूल->path);
            }
        }
    }
  वापस 0;
}

/* Initialize the environment variables.  */
अटल पूर्णांक
setup_environment (runtime_spec_schema_config_schema *def, uid_t container_uid, libcrun_error_t *err)
{
  पूर्णांक ret;

  अगर (clearenv ())
    वापस crun_make_error (err, त्रुटि_सं, "clearenv");

  अगर (def->process)
    {
      माप_प्रकार i;

      क्रम (i = 0; i < def->process->env_len; i++)
        {
          /* A null element of the array of variables is parsed into a NULL,
             which putenv(3) cannot be handed.  */
          अगर (def->process->env[i] == शून्य)
            वापस crun_make_error (err, EINVAL, "environment variable is not specified");

          अगर (putenv (def->process->env[i]) < 0)
            वापस crun_make_error (err, त्रुटि_सं, "putenv `%s`", def->process->env[i]);
        }
    }

  अगर (is_empty_string (दो_पर्या ("HOME")))
    {
      ret = set_home_env (container_uid);
      अगर (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          अगर (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            वापस crun_make_error (err, त्रुटि_सं, "setenv HOME");
        }
    }

  वापस 0;
}

/* Configure terminal and console for interactive containers.  */
अटल पूर्णांक
setup_terminal (काष्ठा container_entrypoint_s *entrypoint_args, libcrun_container_t *container,
                पूर्णांक console_socket, पूर्णांक console_socketpair, पूर्णांक has_terminal, libcrun_error_t *err)
{
  cleanup_close पूर्णांक terminal_fd = -1;
  पूर्णांक ret;

  अगर (! has_terminal)
    वापस 0;

  ख_साफ (मानक_त्रुटि);

  अगर (console_socket >= 0 || (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0))
    {
      terminal_fd = libcrun_set_terminal (container, err);
      अगर (UNLIKELY (terminal_fd < 0))
        वापस terminal_fd;
    }

  अगर (console_socket >= 0)
    {
      ret = send_fd_to_socket (console_socket, terminal_fd, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  अन्यथा अगर (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0)
    {
      ret = send_fd_to_socket (console_socketpair, terminal_fd, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  वापस 0;
}

/* Resolve and validate the container's executable path.  */
अटल पूर्णांक
setup_executable_path (काष्ठा container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                       अक्षर **exec_path, libcrun_error_t *err)
{
  पूर्णांक ret;

  अगर (def->process && def->process->args)
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      अगर (UNLIKELY (ret < 0))
        {
          अगर (entrypoint_args->custom_handler == शून्य && crun_error_get_errno (err) == ENOENT)
            वापस ret;

          /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
             once the process switched to the use that runs in the container.  This might be necessary
             when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
             is not honored.  */
          crun_error_release (err);
        }
    }

  वापस 0;
}

/* Apply security settings for the container process.  */
अटल पूर्णांक
apply_security_settings (काष्ठा container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                         libcrun_container_t *container, pid_t own_pid, libcrun_error_t *err)
{
  पूर्णांक ret;
  runtime_spec_schema_config_schema_process_capabilities *capabilities;
  पूर्णांक no_new_privs;

  अगर (def->process && ! def->process->no_new_privileges)
    {
      अक्षर **seccomp_flags = शून्य;
      माप_प्रकार seccomp_flags_len = 0;
      cleanup_free अक्षर *seccomp_fd_payload = शून्य;
      माप_प्रकार seccomp_fd_payload_len = 0;

      अगर (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      अगर (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  capabilities = def->process ? def->process->capabilities : शून्य;
  no_new_privs = def->process ? def->process->no_new_privileges : 1;
  ret = libcrun_set_caps (capabilities, container->container_uid, container->container_gid, no_new_privs,
                          entrypoint_args->custom_handler != शून्य, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस 0;
}

/* Create the process that joins the existing PID namespace now that the
   host mounts are not visible anymore, and continue the setup from there.  */
अटल पूर्णांक
do_delayed_pidns_fork (पूर्णांक sync_socket, pid_t *own_pid, libcrun_error_t *err)
{
  pid_t pid;
  पूर्णांक ret;

  pid = विभाजन ();
  अगर (UNLIKELY (pid < 0))
    वापस crun_make_error (err, त्रुटि_सं, "fork");

  अगर (pid)
    {
      /* Report the new PID to the parent and exit immediately.  */
      ret = sync_socket_send_sync (sync_socket, false, err);
      अगर (LIKELY (ret == 0))
        {
          ret = अस्थायी_FAILURE_RETRY (ग_लिखो (sync_socket, &pid, माप (pid)));
          अगर (LIKELY (ret == माप (pid)))
            _safe_exit (निकास_सफल);
        }

      समाप्त (pid, SIGKILL);
      _safe_exit (निकास_त्रुटि);
    }

  /* sync receive own pid.  */
  ret = अस्थायी_FAILURE_RETRY (पढ़ो (sync_socket, own_pid, माप (*own_pid)));
  अगर (UNLIKELY (ret != माप (*own_pid)))
    {
      अगर (ret >= 0)
        त्रुटि_सं = 0;
      वापस crun_make_error (err, त्रुटि_सं, "read from sync socket");
    }

  वापस 0;
}

काष्ठा delayed_pidns_fork_s
{
  libcrun_container_t *container;
  पूर्णांक sync_socket;
  pid_t *own_pid;
};

अटल पूर्णांक
delayed_pidns_fork_cb (व्योम *तथ्य, libcrun_error_t *err)
{
  काष्ठा delayed_pidns_fork_s *d = तथ्य;

  अगर (! libcrun_container_delayed_pidns_fork (d->container))
    वापस 0;

  वापस do_delayed_pidns_fork (d->sync_socket, d->own_pid, err);
}

अटल पूर्णांक
container_init_setup (व्योम *args, pid_t *own_pid, अक्षर *notify_socket,
                      पूर्णांक sync_socket, अक्षर **exec_path, libcrun_error_t *err)
{
  काष्ठा container_entrypoint_s *entrypoint_args = args;
  libcrun_container_t *container = entrypoint_args->container;
  bool chdir_done = false;
  पूर्णांक ret;
  पूर्णांक has_terminal;
  cleanup_close पूर्णांक console_socket = -1;
  cleanup_close पूर्णांक console_socketpair = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free अक्षर *rootfs = शून्य;
  काष्ठा delayed_pidns_fork_s delayed_pidns_fork = {
    .container = container,
    .sync_socket = sync_socket,
    .own_pid = own_pid,
  };

  ret = initialize_security (container, def->process, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_configure_network (container, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = resolve_rootfs_path (container, &rootfs, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  /* Configure terminal socket pair for container communication.  */
  अगर (entrypoint_args->terminal_socketpair[0] >= 0)
    {
      close_and_reset (&entrypoint_args->terminal_socketpair[0]);
      console_socketpair = entrypoint_args->terminal_socketpair[1];
    }

  /* sync 1.  */
  ret = sync_socket_wait_sync (शून्य, sync_socket, false, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  has_terminal = container->container_def->process && container->container_def->process->terminal;
  अगर (has_terminal && entrypoint_args->context->console_socket)
    console_socket = entrypoint_args->console_socket_fd;

  ret = libcrun_set_sysctl (container, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_BEFORE_MOUNTS, container, rootfs, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_do_pivot_मूल (container, entrypoint_args->context->no_pivot, &rootfs, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  /* sync 2 and 3 are sent as part of libcrun_set_mounts.  */
  ret = libcrun_set_mounts (entrypoint_args, container, rootfs, send_sync_cb, &sync_socket, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  /* The createContainer hooks MUST run after the mounts are set up but before
     the pivot_root.  When these hooks are present can_use_open_tree_namespace()
     forces the deferred-pivot path, so the actual pivot happens later in
     libcrun_finalize_mounts().  */
  अगर (def->hooks && def->hooks->create_container_len)
    {
      libcrun_error_t tmp_err = शून्य;
      पूर्णांक in_userns = check_running_in_user_namespace (&tmp_err);
      अगर (tmp_err)
        crun_error_release (&tmp_err);
      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, शून्य, "created", "createContainer",
                      (hook **) def->hooks->create_container,
                      def->hooks->create_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      अगर (UNLIKELY (ret != 0))
        वापस ret;
    }

  ret = libcrun_finalize_mounts (entrypoint_args, container, rootfs, delayed_pidns_fork_cb, &delayed_pidns_fork, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (def->process)
    {
      ret = libcrun_set_selinux_label (container, def->process, false, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = libcrun_set_apparmor_profile (container, def->process, false, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  ret = mark_or_close_fds_ge_than (container, entrypoint_args->context->preserve_fds + 3, false, err);
  अगर (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ret = libcrun_reopen_dev_null (err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = maybe_chown_std_streams (container->container_uid, container->container_gid, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = setup_environment (def, container->container_uid, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  /* Set primary process to 1 explicitly if nothing is configured and LISTEN_FD is not set.  */
  अगर (entrypoint_args->context->listen_fds > 0 && दो_पर्या ("LISTEN_PID") == शून्य)
    {
      libcrun_warning ("setting LISTEN_PID=1 since no previous configuration was found");
      अगर (UNLIKELY (setenv ("LISTEN_PID", "1", 1) < 0))
        वापस crun_make_error (err, त्रुटि_सं, "setenv LISTEN_PID");
    }

  /* Attempt to chdir immediately here, before doing the setresuid.  If we fail here, let's
     try again later once the process switched to the user that runs in the container.  */
  अगर (def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      अगर (LIKELY (ret == 0))
        chdir_done = true;
      अन्यथा
        crun_error_release (err);
    }

  ret = setsid ();
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "setsid");

  ret = setup_terminal (entrypoint_args, container, console_socket, console_socketpair, has_terminal, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  close_and_reset (&console_socket);
  close_and_reset (&console_socketpair);

  ret = setup_executable_path (entrypoint_args, def, exec_path, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_set_hostname (container, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_set_domainname (container, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (def->process && def->process->user)
    umask (def->process->user->umask_present ? def->process->user->umask : 0022);

  ret = apply_security_settings (entrypoint_args, def, container, *own_pid, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (UNLIKELY (def->process && def->process->args && *exec_path == शून्य))
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      अगर (UNLIKELY (ret < 0))
        {
          अगर (entrypoint_args->custom_handler == शून्य || is_empty_string (def->process->args[0]))
            वापस ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          *exec_path = xstrdup (def->process->args[0]);
        }
    }

  /* The chdir was not already performed, so try again now after switching to the UID/GID in the container.  */
  अगर (! chdir_done && def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (notify_socket && putenv (notify_socket) < 0)
    वापस crun_make_error (err, त्रुटि_सं, "putenv `%s`", notify_socket);

  वापस 0;
}

अटल पूर्णांक
open_hooks_output (libcrun_container_t *container, पूर्णांक *out_fd, पूर्णांक *err_fd, libcrun_error_t *err)
{
  स्थिर अक्षर *annotation;

  *err_fd = *out_fd = -1;

  libcrun_debug ("Opening hooks output");
  annotation = find_annotation (container, "run.oci.hooks.stdout");
  अगर (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stdout` annotation");
      *out_fd = अस्थायी_FAILURE_RETRY (खोलो (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      अगर (UNLIKELY (*out_fd < 0))
        वापस crun_make_error (err, त्रुटि_सं, "open `%s`", annotation);
    }

  annotation = find_annotation (container, "run.oci.hooks.stderr");
  अगर (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stderr` annotation");
      *err_fd = अस्थायी_FAILURE_RETRY (खोलो (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      अगर (UNLIKELY (*err_fd < 0))
        वापस crun_make_error (err, त्रुटि_सं, "open `%s`", annotation);
    }

  वापस 0;
}

/* The environment the process was started with lives on the initial stack, right after the
   argv strings and before the string AT_EXECFN points to.  clearenv(3) only drops the pointers
   to it, so when there is no execv to replace the stack, erase it or the container process can
   still read it, e.g. through /proc/self/environ.  */
अटल व्योम
erase_initial_environment (अक्षर **argv, पूर्णांक argc)
{
  अक्षर *start = argv[argc - 1] + म_माप (argv[argc - 1]) + 1;
  अक्षर *end = (अक्षर *) getauxval (AT_EXECFN);

  अगर (end > start)
    स_रखो (start, 0, end - start);
}

अटल व्योम
rewrite_argv (अक्षर **argv, पूर्णांक argc, स्थिर अक्षर *name, अक्षर **args, माप_प्रकार args_len)
{
  cleanup_free अक्षर *decorated_name = शून्य;
  माप_प्रकार available_len = argv[argc - 1] - argv[0] + म_माप (argv[argc - 1]) + 1;
  cleanup_free अक्षर *new_argv = शून्य;
  माप_प्रकार i, so_far = 0, needed = 0;

  needed = xasprintf (&decorated_name, "[libcrun:%s]", name) + 1;
  क्रम (i = 0; i < args_len; i++)
    needed += म_माप (args[i]) + 1;

  new_argv = xmalloc0 (needed + 1);
  म_नकल (new_argv, decorated_name);
  so_far = म_माप (decorated_name) + 1;

  अगर (available_len >= needed)
    {
      क्रम (i = 0; i < args_len; i++)
        {
          म_नकल (new_argv + so_far, args[i]);
          so_far += म_माप (args[i]) + 1;
        }
    }

  अगर (so_far >= available_len)
    so_far = available_len - 1;

  स_रखो (argv[0], 0, available_len);
  स_नकल (argv[0], new_argv, so_far);
}

/* Entrypoint to the container.  */
अटल पूर्णांक
container_init (व्योम *args, अक्षर *notify_socket, पूर्णांक sync_socket, libcrun_error_t *err)
{
  काष्ठा container_entrypoint_s *entrypoint_args = args;
  पूर्णांक ret;
  runtime_spec_schema_config_schema *def = entrypoint_args->container->container_def;
  cleanup_free अक्षर *exec_path = शून्य;
  __attribute__ ((unused)) cleanup_free अक्षर *notify_socket_cleanup = notify_socket;
  pid_t own_pid = 0;

  entrypoint_args->sync_socket = sync_socket;

  crun_set_output_handler (log_write_to_sync_socket, args);

  /* sync receive own pid.  */
  ret = अस्थायी_FAILURE_RETRY (पढ़ो (sync_socket, &own_pid, माप (own_pid)));
  अगर (UNLIKELY (ret != माप (own_pid)))
    {
      अगर (ret >= 0)
        त्रुटि_सं = 0;
      वापस crun_make_error (err, त्रुटि_सं, "read from sync socket");
    }

  ret = container_init_setup (args, &own_pid, notify_socket, sync_socket, &exec_path, err);
  अगर (UNLIKELY (ret < 0))
    {
      /* If it fails to write the error using the sync socket, then fallback
         to stderr.  */
      अगर (sync_socket_write_error (sync_socket, err) == 0)
        {
          crun_error_release (err);
          /* A return value of > 0 means to fail with an error without err being set so to avoid
             printing it twice.  */
          वापस 1;
        }
      वापस ret;
    }

  entrypoint_args->sync_socket = -1;

  ret = unblock_signals (err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  /* sync 4.  */
  ret = sync_socket_send_sync (sync_socket, false, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  close_and_reset (&sync_socket);

  अगर (entrypoint_args->context->fifo_exec_wait_fd >= 0)
    {
      अक्षर बफर[1];
      fd_set read_set;
      cleanup_close पूर्णांक fd = entrypoint_args->context->fifo_exec_wait_fd;
      entrypoint_args->context->fifo_exec_wait_fd = -1;

      FD_ZERO (&read_set);
      FD_SET (fd, &read_set);
      करो
        {
          ret = select (fd + 1, &read_set, शून्य, शून्य, शून्य);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "select");

          ret = अस्थायी_FAILURE_RETRY (पढ़ो (fd, बफर, माप (बफर)));
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "read from the exec fifo");
      } जबतक (ret == 0);

      close_and_reset (&entrypoint_args->context->fifo_exec_wait_fd);
    }

  crun_set_output_handler (log_write_to_stderr, शून्य);

  अगर (def->process && def->process->no_new_privileges)
    {
      अक्षर **seccomp_flags = शून्य;
      माप_प्रकार seccomp_flags_len = 0;
      cleanup_free अक्षर *seccomp_fd_payload = शून्य;
      माप_प्रकार seccomp_fd_payload_len = 0;

      अगर (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      अगर (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (entrypoint_args->container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags,
                                   seccomp_flags_len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  अगर (UNLIKELY (def->process == शून्य))
    वापस crun_make_error (err, 0, "block `process` not found");

  अगर (UNLIKELY (exec_path == शून्य))
    वापस crun_make_error (err, 0, "executable path not specified");

  अगर (def->hooks && def->hooks->start_container_len)
    {
      libcrun_container_t *container = entrypoint_args->container;
      libcrun_error_t tmp_err = शून्य;
      पूर्णांक in_userns = check_running_in_user_namespace (&tmp_err);
      अगर (tmp_err)
        crun_error_release (&tmp_err);

      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, शून्य, "created", "startContainer",
                      (hook **) def->hooks->start_container,
                      def->hooks->start_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      अगर (UNLIKELY (ret != 0))
        वापस ret;

      /* Seek stdout/stderr to the end.  If the hooks were using the same files,
         the container process overwrites what was previously written.  */
      (व्योम) lseek (1, 0, अंत_से);
      (व्योम) lseek (2, 0, अंत_से);
    }

  अगर (entrypoint_args->custom_handler)
    {
      /* Files marked with O_CLOEXEC are closed at execv time, so make sure they are closed now.
         This is a best effort operation, because the seccomp filter is already in place and it could
         stop some syscalls used by mark_or_close_fds_ge_than.
      */
      अगर (entrypoint_args->custom_handler->vtable->close_fds)
        {
          ret = entrypoint_args->custom_handler->vtable->close_fds (entrypoint_args->custom_handler->cookie,
                                                                    entrypoint_args->container,
                                                                    entrypoint_args->context->preserve_fds,
                                                                    err);
        }
      अन्यथा
        {
          ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
        }
      अगर (UNLIKELY (ret < 0))
        crun_error_release (err);

      prctl (PR_SET_NAME, entrypoint_args->custom_handler->vtable->name);

      अगर (entrypoint_args->context->argv)
        {
          erase_initial_environment (entrypoint_args->context->argv, entrypoint_args->context->argc);
          rewrite_argv (entrypoint_args->context->argv, entrypoint_args->context->argc,
                        entrypoint_args->custom_handler->vtable->name, def->process->args,
                        def->process->args_len);

          /* It is a quite destructive operation as we might be referencing data from the old
             argv, so make sure the context is not reused.  */
          entrypoint_args->context->argv = शून्य;
          entrypoint_args->context = शून्य;
        }

      ret = libcrun_set_selinux_label (entrypoint_args->container, def->process, true, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = libcrun_set_apparmor_profile (entrypoint_args->container, def->process, true, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = entrypoint_args->custom_handler->vtable->run_func (entrypoint_args->custom_handler->cookie,
                                                               entrypoint_args->container,
                                                               exec_path,
                                                               def->process->args);
      अगर (ret != 0)
        वापस crun_make_error (err, 0, "exec container process failed with handler as `%s`", entrypoint_args->custom_handler->vtable->name);

      वापस ret;
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation since the seccomp profile is already in place now and might block
     some of the syscalls needed by mark_or_close_fds_ge_than.  */
  ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
  अगर (UNLIKELY (ret < 0))
    crun_error_release (err);

  अस्थायी_FAILURE_RETRY (execv (exec_path, def->process->args));

  अगर (त्रुटि_सं == ENOENT)
    वापस crun_make_error (err, त्रुटि_सं, "exec container process (missing dynamic library?) `%s`", exec_path);

  वापस crun_make_error (err, त्रुटि_सं, "exec container process `%s`", exec_path);
}

अटल पूर्णांक
read_container_config_from_state (libcrun_container_t **container, स्थिर अक्षर *state_मूल, स्थिर अक्षर *id,
                                  libcrun_error_t *err)
{
  cleanup_free अक्षर *config_file = शून्य;
  cleanup_free अक्षर *dir = शून्य;
  पूर्णांक ret;

  *container = शून्य;

  ret = libcrun_get_state_directory (&dir, state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = append_paths (&config_file, err, dir, "config.json", शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  *container = libcrun_container_load_from_file (config_file, err);
  अगर (*container == शून्य)
    वापस -1;

  वापस 0;
}

अटल पूर्णांक
run_poststop_hooks (arg_unused libcrun_context_t *context, libcrun_container_t *container,
                    runtime_spec_schema_config_schema *def,
                    libcrun_container_status_t *status, स्थिर अक्षर *state_मूल, स्थिर अक्षर *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container_cleanup = शून्य;
  पूर्णांक ret;

  अगर (def == शून्य)
    {
      अगर (container == शून्य)
        {
          ret = read_container_config_from_state (&container_cleanup, state_मूल, id, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
          container = container_cleanup;
        }

      def = container->container_def;
    }

  अगर (def->hooks && def->hooks->poststop_len)
    {
      cleanup_close पूर्णांक hooks_out_fd = -1;
      cleanup_close पूर्णांक hooks_err_fd = -1;

      अगर (container == शून्य)
        {
          ret = read_container_config_from_state (&container_cleanup, state_मूल, id, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
          container = container_cleanup;
        }

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = do_hooks (def, 0, id, true, status->bundle, status->bundle, "stopped", "poststop", (hook **) def->hooks->poststop,
                      def->hooks->poststop_len, hooks_out_fd, hooks_err_fd, false, err);
      अगर (UNLIKELY (ret != 0))
        {
          अगर (ret < 0)
            libcrun_error_report_and_release (err);
          अन्यथा
            libcrun_error (0, "poststop hook failed with exit code: %d", ret);
        }
    }
  वापस 0;
}

अटल bool
has_new_pid_namespace (runtime_spec_schema_config_schema *def)
{
  माप_प्रकार i;

  अगर (def->linux == शून्य || def->linux->namespaces == शून्य)
    वापस false;

  क्रम (i = 0; i < def->linux->namespaces_len; i++)
    {
      अगर (म_भेद (def->linux->namespaces[i]->type, "pid") == 0 && def->linux->namespaces[i]->path == शून्य)
        वापस true;
    }
  वापस false;
}

/* Delete the container state.  CONTAINER is the container the caller already has
   available, or NULL.  When it is not NULL, the poststop hooks are run even if the
   status file was never written, e.g. when the container creation failed.  */
अटल पूर्णांक
container_delete_internal (libcrun_context_t *context, libcrun_container_t *container,
                           runtime_spec_schema_config_schema *def, स्थिर अक्षर *id, bool force, bool killall,
                           libcrun_error_t *err)
{
  cleanup_cgroup_status काष्ठा libcrun_cgroup_status *cgroup_status = शून्य;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_container libcrun_container_t *container_cleanup = शून्य;
  स्थिर अक्षर *state_मूल = context->state_मूल;
  पूर्णांक ret;

  ret = libcrun_read_container_status (&status, state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    {
      अगर (force && crun_error_get_errno (err) == ENOENT)
        {
          libcrun_error_t tmp_err = शून्य;

          crun_error_release (err);

          अगर (container)
            {
              libcrun_container_status_t partial_status = {
                .bundle = (अक्षर *) context->bundle,
              };

              ret = run_poststop_hooks (context, container, def, &partial_status, state_मूल, id, err);
              अगर (UNLIKELY (ret < 0))
                crun_error_write_warning_and_release (context->output_handler_arg, &err);
            }

          libcrun_container_delete_status (state_मूल, id, &tmp_err);
          crun_error_release (&tmp_err);
          वापस 0;
        }
      crun_error_release (err);
      वापस libcrun_container_delete_status (state_मूल, id, err);
    }

  cgroup_status = libcrun_cgroup_make_status (&status);

  अगर (! force)
    {
      ret = libcrun_is_container_running (&status, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
      अगर (ret == 1)
        {
          ret = libcrun_status_has_read_exec_fifo (state_मूल, id, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
          अगर (ret == 0)
            वापस crun_make_error (err, 0, "the container `%s` is not in `created` or `stopped` state", id);

          /* If the container is in "created" state, then do the equivalent of delete --force.  */
          killall = force = true;
        }
    }

  अगर (def == शून्य)
    {
      अगर (container == शून्य)
        {
          ret = read_container_config_from_state (&container_cleanup, state_मूल, id, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;

          container = container_cleanup;
        }

      def = container->container_def;
    }

  अगर (killall && force)
    {
      /* If the container has a pid namespace, it is enough to kill the first
         process (pid=1 in the namespace).
      */
      अगर (has_new_pid_namespace (def))
        {
          ret = libcrun_kill_linux (&status, SIGKILL, err);
          अगर (UNLIKELY (ret < 0))
            {
              अगर (crun_error_get_errno (err) != ESRCH)
                वापस ret;

              crun_error_release (err);
            }
        }
      अन्यथा अगर (status.cgroup_path)
        {
          ret = libcrun_cgroup_killall (cgroup_status, SIGKILL, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }
    }

  अगर (def->linux && def->linux->intel_rdt)
    {
      ret = libcrun_destroy_intelrdt (id, def, err);
      अगर (UNLIKELY (ret < 0))
        libcrun_error_report_and_release (err);
    }

  अगर (status.cgroup_path)
    {
      /* If the cgroup cannot be destroyed, there are still processes in it.
         Keep the container status, it is the only record of the cgroup, so
         that the delete can be retried.  */
      ret = libcrun_cgroup_destroy (cgroup_status, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  ret = run_poststop_hooks (context, container, def, &status, state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  वापस libcrun_container_delete_status (state_मूल, id, err);
}

पूर्णांक
libcrun_container_delete (libcrun_context_t *context, स्थिर अक्षर *id, bool force, libcrun_error_t *err)
{
  वापस container_delete_internal (context, शून्य, शून्य, id, force, true, err);
}

पूर्णांक
libcrun_container_kill (libcrun_context_t *context, स्थिर अक्षर *id, स्थिर अक्षर *संकेत, libcrun_error_t *err)
{
  पूर्णांक sig, ret;
  स्थिर अक्षर *state_मूल = context->state_मूल;
  cleanup_container_status libcrun_container_status_t status = {};

  sig = str2sig (संकेत);
  अगर (UNLIKELY (sig < 0))
    वापस crun_make_error (err, 0, "unknown signal `%s`", संकेत);

  ret = libcrun_read_container_status (&status, state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस libcrun_kill_linux (&status, sig, err);
}

पूर्णांक
libcrun_container_killall (libcrun_context_t *context, स्थिर अक्षर *id, स्थिर अक्षर *संकेत, libcrun_error_t *err)
{
  पूर्णांक sig, ret;
  स्थिर अक्षर *state_मूल = context->state_मूल;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_cgroup_status काष्ठा libcrun_cgroup_status *cgroup_status = शून्य;

  sig = str2sig (संकेत);
  अगर (UNLIKELY (sig < 0))
    वापस crun_make_error (err, 0, "unknown signal `%s`", संकेत);

  ret = libcrun_read_container_status (&status, state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  cgroup_status = libcrun_cgroup_make_status (&status);

  ret = libcrun_cgroup_killall (cgroup_status, sig, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;
  वापस 0;
}

अटल पूर्णांक
write_container_status (libcrun_container_t *container, libcrun_context_t *context,
                        pid_t pid, काष्ठा libcrun_cgroup_status *cgroup_status,
                        libcrun_error_t *err)
{
  cleanup_free अक्षर *bundle_allocated = शून्य;
  स्थिर अक्षर *bundle = get_bundle_path (context->bundle, &bundle_allocated);
  cleanup_free अक्षर *owner = get_user_name (geteuid ());
  अक्षर *external_descriptors = libcrun_get_external_descriptors (container);
  अक्षर *rootfs = container->container_def->मूल ? container->container_def->मूल->path : "";
  अक्षर created[35];

  libcrun_container_status_t status = {
    .pid = pid,
    .rootfs = rootfs,
    .bundle = (अक्षर *) bundle,
    .created = created,
    .owner = owner,
    .systemd_cgroup = context->systemd_cgroup,
    .detached = context->detach,
    .no_new_keyring = context->no_new_keyring,
    .external_descriptors = external_descriptors,
    .cgroup_path = शून्य,
    .scope = शून्य,
  };

  get_current_timestamp (created, माप (created));

  अगर (cgroup_status)
    {
      पूर्णांक ret;

      ret = libcrun_cgroup_get_status (cgroup_status, &status, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (external_descriptors == शून्य)
    वापस crun_make_error (err, 0, "invalid internal state.  No external descriptors found");
  वापस libcrun_write_container_status (context->state_मूल, context->id, &status, err);
}

अटल पूर्णांक
reap_subprocesses (pid_t *main_process, पूर्णांक *main_process_exit, पूर्णांक *last_process, libcrun_error_t *err)
{
  *last_process = 0;
  जबतक (1)
    {
      पूर्णांक status;
      पूर्णांक r = waitpid_ignore_stopped (-1, &status, WNOHANG);
      अगर (r < 0)
        {
          अगर (त्रुटि_सं == EINTR)
            जारी;
          अगर (त्रुटि_सं == ECHILD)
            {
              *last_process = 1;
              वापस 0;
            }
          वापस crun_make_error (err, त्रुटि_सं, "waitpid");
        }
      अगर (r == 0)
        अवरोध;
      अगर (r != *main_process)
        जारी;

      *main_process_exit = get_process_exit_status (status);

      /* Once reaped, the pid can be reused by an unrelated process.  */
      *main_process = 0;
    }
  वापस 0;
}

अटल पूर्णांक
send_sd_notify (स्थिर अक्षर *ready_str, libcrun_error_t *err)
{
#अगर_घोषित HAVE_SYSTEMD
  पूर्णांक ret = sd_notify (0, ready_str);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, -ret, "sd_notify");

#  अगर HAVE_SD_NOTIFY_BARRIER
  /* Hard-code a 30 seconds timeout.  Ignore errors.  */
  sd_notify_barrier (0, 30 * 1000000);
#  endif
#अन्यथा
  (व्योम) ready_str;
  (व्योम) err;
#पूर्ण_अगर
  वापस 0;
}

अटल पूर्णांक
handle_notify_socket (पूर्णांक notify_socketfd, libcrun_error_t *err)
{
#अगर_घोषित HAVE_SYSTEMD
  पूर्णांक ret;
  अक्षर buf[256];
  स्थिर अक्षर *ready_str = "READY=1";

  ret = recvfrom (notify_socketfd, buf, माप (buf) - 1, 0, शून्य, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "recvfrom notify socket");

  buf[ret] = '\0';
  अगर (म_माला (buf, ready_str))
    {
      ret = send_sd_notify (ready_str, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      वापस 1;
    }
  वापस 0;
#अन्यथा
  (व्योम) notify_socketfd;
  (व्योम) err;
  वापस 1;
#पूर्ण_अगर
}

काष्ठा wait_for_process_args
{
  pid_t pid;
  libcrun_context_t *context;
  पूर्णांक terminal_fd;
  पूर्णांक notify_socket;
  पूर्णांक *container_ready_fd;
  पूर्णांक seccomp_notify_fd;
  स्थिर अक्षर *seccomp_notify_plugins;
};

अटल पूर्णांक
write_pid_file (स्थिर अक्षर *pid_file, pid_t pid, libcrun_error_t *err)
{
  अक्षर buf[32];
  पूर्णांक buf_len = snprintf (buf, माप (buf), "%d", pid);
  अगर (UNLIKELY (buf_len >= (पूर्णांक) माप (buf)))
    वापस crun_make_error (err, 0, "internal error: static buffer too small");
  वापस write_file_at_with_flags (AT_FDCWD, O_CREAT | O_TRUNC, 0700, pid_file, buf, buf_len, err);
}

#घोषणा MAX_EVENTS 10

अटल bool
is_fd_pollable (पूर्णांक fd)
{
  काष्ठा epoll_event ev = { .events = EPOLLIN };
  cleanup_close पूर्णांक epollfd = epoll_create1 (EPOLL_CLOEXEC);

  अगर (epollfd < 0)
    वापस true;

  वापस epoll_ctl (epollfd, EPOLL_CTL_ADD, fd, &ev) == 0 || त्रुटि_सं != EPERM;
}

अटल पूर्णांक
wait_for_process (काष्ठा wait_for_process_args *args, libcrun_error_t *err)
{
  cleanup_channel_fd_pair काष्ठा channel_fd_pair *from_terminal = शून्य;
  cleanup_channel_fd_pair काष्ठा channel_fd_pair *to_terminal = शून्य;
  पूर्णांक ret, container_exit_code = 0, last_process;
  cleanup_close पूर्णांक terminal_fd_from = -1;
  cleanup_close पूर्णांक terminal_fd_to = -1;
  bool stdin_pollable = false, stdout_pollable = false;
  cleanup_close पूर्णांक epollfd = -1;
  cleanup_close पूर्णांक signalfd = -1;
  sigset_t mask;
  पूर्णांक in_fds[MAX_EVENTS];
  पूर्णांक in_fds_len = 0;
  पूर्णांक out_fds[MAX_EVENTS];
  पूर्णांक out_fds_len = 0;
  माप_प्रकार i;

  cleanup_seccomp_notify_context काष्ठा seccomp_notify_context_s *seccomp_notify_ctx = शून्य;

  container_exit_code = 0;

  अगर (args == शून्य || args->context == शून्य)
    वापस crun_make_error (err, 0, "internal error: context is empty");

  क्रम (i = 0; i < MAX_EVENTS; i++)
    {
      in_fds[i] = -1;
      out_fds[i] = -1;
    }

  अगर (args->context->pid_file)
    {
      ret = write_pid_file (args->context->pid_file, args->pid, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  /* Also exit if there is nothing more to wait for.  */
  अगर (args->context->detach && args->notify_socket < 0)
    वापस 0;

  अगर (args->container_ready_fd)
    {
      ret = 0;
      अस्थायी_FAILURE_RETRY (ग_लिखो (*args->container_ready_fd, &ret, माप (ret)));
      close_and_reset (args->container_ready_fd);
    }

  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "sigprocmask");

  signalfd = create_signalfd (&mask, err);
  अगर (UNLIKELY (signalfd < 0))
    वापस signalfd;

  ret = reap_subprocesses (&args->pid, &container_exit_code, &last_process, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (last_process)
    वापस container_exit_code;

  अगर (args->seccomp_notify_fd >= 0)
    {
      cleanup_free अक्षर *state_मूल = शून्य;
      cleanup_free अक्षर *oci_config_path = शून्य;

      काष्ठा libcrun_load_seccomp_notify_conf_s conf;
      स_रखो (&conf, 0, माप conf);

      ret = libcrun_get_state_directory (&state_मूल, args->context->state_मूल, args->context->id, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = append_paths (&oci_config_path, err, state_मूल, "config.json", शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      conf.runtime_मूल_path = state_मूल;
      conf.name = args->context->id;
      conf.bundle_path = args->context->bundle;
      conf.oci_config_path = oci_config_path;

      ret = set_blocking_fd (args->seccomp_notify_fd, false, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = libcrun_load_seccomp_notify_plugins (&seccomp_notify_ctx,
                                                 args->seccomp_notify_plugins,
                                                 &conf, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      in_fds[in_fds_len++] = args->seccomp_notify_fd;
    }

  अगर (args->terminal_fd >= 0)
    {
      /* The terminal_fd is dup()ed so that it can be registered with
         epoll multiple times using different masks.  */
      terminal_fd_from = dup (args->terminal_fd);
      अगर (UNLIKELY (terminal_fd_from < 0))
        वापस crun_make_error (err, त्रुटि_सं, "dup terminal fd");
      terminal_fd_to = dup (args->terminal_fd);
      अगर (UNLIKELY (terminal_fd_to < 0))
        वापस crun_make_error (err, त्रुटि_सं, "dup terminal fd");

      पूर्णांक i, non_blocking_fds[] = { terminal_fd_from, terminal_fd_to, 0, 1, -1 };
      क्रम (i = 0; non_blocking_fds[i] >= 0; i++)
        {
          ret = set_blocking_fd (non_blocking_fds[i], false, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }

      /* stdin and stdout can be something epoll does not support, such as
         /dev/null or a regular file.  Such a stdin is not forwarded, and
         such a stdout is written to without waiting for it to be ready.  */
      stdin_pollable = is_fd_pollable (0);
      stdout_pollable = is_fd_pollable (1);

      from_terminal = channel_fd_pair_new (terminal_fd_from, 1, बफ_मान);
      अगर (stdin_pollable)
        to_terminal = channel_fd_pair_new (0, terminal_fd_to, बफ_मान);
    }

  in_fds[in_fds_len++] = signalfd;
  अगर (args->notify_socket >= 0)
    in_fds[in_fds_len++] = args->notify_socket;
  अगर (args->terminal_fd >= 0)
    {
      अगर (stdin_pollable)
        {
          in_fds[in_fds_len++] = 0;
          out_fds[out_fds_len++] = terminal_fd_to;
        }

      in_fds[in_fds_len++] = terminal_fd_from;
      अगर (stdout_pollable)
        out_fds[out_fds_len++] = 1;
    }

  epollfd = epoll_helper (in_fds, शून्य, out_fds, शून्य, err);
  अगर (UNLIKELY (epollfd < 0))
    वापस epollfd;

  जबतक (1)
    {
      काष्ठा epoll_event events[MAX_EVENTS];
      काष्ठा signalfd_siginfo si;
      काष्ठा winsize ws;
      पूर्णांक i, nr_events;
      ssize_t res;

      nr_events = अस्थायी_FAILURE_RETRY (epoll_wait (epollfd, events, MAX_EVENTS, -1));
      अगर (UNLIKELY (nr_events < 0))
        वापस crun_make_error (err, त्रुटि_सं, "epoll_wait");

      क्रम (i = 0; i < nr_events; i++)
        {
          अगर (events[i].तथ्य.fd == 0 || events[i].तथ्य.fd == terminal_fd_to)
            {
              ret = channel_fd_pair_process (to_terminal, epollfd, err);
              अगर (UNLIKELY (ret < 0))
                वापस crun_error_wrap (err, "copy to terminal fd");
            }
          अन्यथा अगर (events[i].तथ्य.fd == 1 || events[i].तथ्य.fd == terminal_fd_from)
            {
              ret = channel_fd_pair_process (from_terminal, epollfd, err);
              अगर (UNLIKELY (ret < 0))
                वापस crun_error_wrap (err, "copy from terminal fd");
            }
          अन्यथा अगर (events[i].तथ्य.fd == args->seccomp_notify_fd)
            {
              ret = libcrun_seccomp_notify_plugins (seccomp_notify_ctx,
                                                    args->seccomp_notify_fd, err);
              अगर (UNLIKELY (ret < 0))
                वापस ret;
            }
          अन्यथा अगर (events[i].तथ्य.fd == args->notify_socket)
            {
              ret = handle_notify_socket (args->notify_socket, err);
              अगर (UNLIKELY (ret < 0))
                वापस ret;
              अगर (ret && args->context->detach)
                वापस 0;
            }
          अन्यथा अगर (events[i].तथ्य.fd == signalfd)
            {
              res = अस्थायी_FAILURE_RETRY (पढ़ो (signalfd, &si, माप (si)));
              अगर (UNLIKELY (res < 0))
                वापस crun_make_error (err, त्रुटि_सं, "read from signalfd");
              अगर (si.ssi_signo == SIGCHLD)
                {
                  ret = reap_subprocesses (&args->pid, &container_exit_code,
                                           &last_process, err);
                  अगर (UNLIKELY (ret < 0))
                    वापस ret;
                  अगर (last_process)
                    वापस container_exit_code;
                }
              अन्यथा अगर (si.ssi_signo == SIGWINCH)
                {
                  /* Ignore the signal if the terminal is not available.  */
                  अगर (args->terminal_fd > 0)
                    {
                      ret = ioctl (0, TIOCGWINSZ, &ws);
                      अगर (UNLIKELY (ret < 0))
                        वापस crun_make_error (err, त्रुटि_सं, "ioctl TIOCGWINSZ copy terminal size from stdin");

                      ret = ioctl (args->terminal_fd, TIOCSWINSZ, &ws);
                      अगर (UNLIKELY (ret < 0))
                        वापस crun_make_error (err, त्रुटि_सं, "ioctl TIOCSWINSZ copy terminal size to pty");
                    }
                }
              अन्यथा अगर (args->pid > 0)
                {
                  /* Send any other signal to the child process, if it was not reaped yet.  */
                  ret = समाप्त (args->pid, si.ssi_signo);
                }
            }
          अन्यथा
            {
              वापस crun_make_error (err, 0, "internal error: unknown fd from epoll_wait");
            }
        }
    }

  वापस 0;
}

अटल व्योम
flush_fd_to_err (libcrun_context_t *context, पूर्णांक terminal_fd)
{
  अक्षर buf[256];
  पूर्णांक flags;

  अगर (terminal_fd < 0 || मानक_त्रुटि == शून्य)
    वापस;

  flags = fcntl (terminal_fd, F_GETFL, 0);
  अगर (flags == -1)
    वापस;
  अगर (fcntl (terminal_fd, F_SETFL, flags | O_NONBLOCK) < 0)
    वापस;

  क्रम (;;)
    {
      पूर्णांक ret = अस्थायी_FAILURE_RETRY (पढ़ो (terminal_fd, buf, माप (buf) - 1));
      अगर (ret <= 0)
        अवरोध;
      buf[ret] = '\0';
      अगर (context->output_handler)
        context->output_handler (0, buf, LIBCRUN_VERBOSITY_ERROR, context->output_handler_arg);
    }
  (व्योम) fcntl (terminal_fd, F_SETFL, flags);
  ख_साफ (मानक_त्रुटि);
  fsync (1);
  fsync (2);
}

अटल पूर्णांक
cleanup_watch (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
               काष्ठा libcrun_cgroup_status *cgroup_status, pid_t init_pid, पूर्णांक sync_socket,
               पूर्णांक terminal_fd, libcrun_error_t *err)
{
  स्थिर अक्षर *oom_message = शून्य;
  libcrun_error_t tmp_err = शून्य;
  पूर्णांक ret;

  अगर (init_pid)
    {
      /* Try to detect whether the cgroup has a OOM.  */
      अगर (cgroup_status)
        {
          पूर्णांक has_oom;

          has_oom = libcrun_cgroup_has_oom (cgroup_status, &tmp_err);
          अगर (has_oom > 0)
            oom_message = "OOM: the memory limit could be too low";
          अन्यथा अगर (has_oom < 0)
            {
              /* If the detection has failed for any reason, e.g. the cgroup was
                 already deleted by the time it was checked, just ignore the
                 failure.  */
              crun_error_release (&tmp_err);
            }
        }
      /* If the OOM wasn't detected, look into the static configuration.  */
      अगर (oom_message == शून्य && is_memory_limit_too_निचला (def))
        oom_message = "the memory limit could be too low";

      समाप्त (init_pid, SIGKILL);
      waitpid_ignore_stopped (init_pid, शून्य, 0);
    }

  ret = sync_socket_wait_sync (context, sync_socket, true, &tmp_err);
  अगर (UNLIKELY (ret < 0))
    {
      crun_error_release (err);
      *err = tmp_err;
    }

  अगर (terminal_fd >= 0)
    flush_fd_to_err (context, terminal_fd);

  अगर (oom_message)
    वापस crun_error_wrap (err, "%s", oom_message);

  वापस -1;
}

/* Find the uid:gid that is mapped to root inside the container user namespace.  */
व्योम
get_मूल_in_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid, uid_t *uid,
                        gid_t *gid)
{
  *uid = -1;
  *gid = -1;

  /* Not root in the namespace.  */
  अगर (host_uid)
    वापस;

  अगर (def->linux && def->linux->uid_mappings)
    {
      माप_प्रकार i;

      क्रम (i = 0; i < def->linux->uid_mappings_len; i++)
        अगर (def->linux->uid_mappings[i]->container_id == 0)
          {
            *uid = def->linux->uid_mappings[i]->host_id;
            अवरोध;
          }
    }
  अगर (def->linux && def->linux->gid_mappings)
    {
      माप_प्रकार i;

      क्रम (i = 0; i < def->linux->gid_mappings_len; i++)
        अगर (def->linux->gid_mappings[i]->container_id == 0)
          {
            *gid = def->linux->gid_mappings[i]->host_id;
            अवरोध;
          }
    }

  /* If the uid and the gid are not changed, do not attempt any chown.  */
  अगर (*uid == host_uid && *gid == host_gid)
    *uid = *gid = -1;
}

अटल पूर्णांक
get_seccomp_receiver_fd (libcrun_container_t *container, पूर्णांक *fd, पूर्णांक *self_receiver_fd, स्थिर अक्षर **plugins,
                         libcrun_error_t *err)
{
  स्थिर अक्षर *tmp;
  runtime_spec_schema_config_schema *def = container->container_def;

  *fd = -1;
  *self_receiver_fd = -1;

  tmp = find_annotation (container, "run.oci.seccomp.plugins");
  अगर (tmp)
    {
      पूर्णांक fds[2];
      पूर्णांक ret;

      ret = create_socket_pair (fds, err);
      अगर (UNLIKELY (ret < 0))
        वापस crun_error_wrap (err, "create socket pair");

      *fd = fds[0];
      *self_receiver_fd = fds[1];
      *plugins = tmp;
      वापस 0;
    }

  अगर (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    tmp = def->linux->seccomp->listener_path;
  अन्यथा
    tmp = find_annotation (container, "run.oci.seccomp.receiver");
  अगर (tmp == शून्य)
    tmp = दो_पर्या ("RUN_OCI_SECCOMP_RECEIVER");
  अगर (tmp)
    {
      अगर (tmp[0] != '/')
        वापस crun_make_error (err, 0, "the seccomp receiver `%s` is not an absolute path", tmp);

      *fd = open_unix_domain_client_socket (tmp, 0, err);
      अगर (UNLIKELY (*fd < 0))
        वापस crun_error_wrap (err, "open seccomp receiver");
    }

  वापस 0;
}

अटल bool
has_seccomp_receiver (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;

  अगर (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    वापस true;

  अगर (find_annotation (container, "run.oci.seccomp.receiver") != शून्य || दो_पर्या ("RUN_OCI_SECCOMP_RECEIVER") != शून्य)
    वापस true;

  वापस false;
}

अटल पूर्णांक
setup_container_hooks_output (libcrun_container_t *container, runtime_spec_schema_config_schema *def,
                              काष्ठा container_entrypoint_s *container_args, पूर्णांक *hooks_out_fd,
                              पूर्णांक *hooks_err_fd, libcrun_error_t *err)
{
  पूर्णांक ret;

  अगर (def->hooks
      && (def->hooks->prestart_len || def->hooks->poststart_len || def->hooks->create_runtime_len
          || def->hooks->create_container_len || def->hooks->start_container_len))
    {
      ret = open_hooks_output (container, hooks_out_fd, hooks_err_fd, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
      container_args->hooks_out_fd = *hooks_out_fd;
      container_args->hooks_err_fd = *hooks_err_fd;
    }
  वापस 0;
}

अटल पूर्णांक
setup_container_keyring (libcrun_container_t *container, libcrun_context_t *context, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णांक ret;

  अगर (! context->no_new_keyring)
    {
      स्थिर अक्षर *label = शून्य;

      libcrun_debug ("Creating new keyring");

      अगर (def->process)
        {
          label = def->process->selinux_label;
          अगर (label)
            libcrun_debug ("Using SELinux process label: `%s`", label);
        }

      ret = libcrun_create_keyring (container, container->context->id, label, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  वापस 0;
}

अटल पूर्णांक
setup_terminal_socket_pair (libcrun_container_t *container, libcrun_context_t *context,
                            काष्ठा container_entrypoint_s *container_args, पूर्णांक *socket_pair_0,
                            पूर्णांक *socket_pair_1, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णांक ret;
  पूर्णांक detach = context->detach;

  अगर (def->process && def->process->terminal && ! detach && context->console_socket == शून्य)
    {
      libcrun_debug ("Creating terminal socket pair");
      container_args->has_terminal_socket_pair = 1;
      ret = create_socket_pair (container_args->terminal_socketpair, err);
      अगर (UNLIKELY (ret < 0))
        वापस crun_error_wrap (err, "create terminal socket");

      *socket_pair_0 = container_args->terminal_socketpair[0];
      *socket_pair_1 = container_args->terminal_socketpair[1];
    }
  वापस 0;
}

अटल पूर्णांक
setup_seccomp (libcrun_container_t *container, स्थिर अक्षर *seccomp_bpf_तथ्य,
               काष्ठा libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, पूर्णांक *seccomp_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णांक ret;

  अगर (find_annotation (container, "run.oci.seccomp.plugins") != शून्य && has_seccomp_receiver (container))
    {
      वापस crun_make_error (err, 0, "seccomp plugins and seccomp receivers cannot be declared at the same time");
    }

  अगर (def->linux && (def->linux->seccomp || seccomp_bpf_तथ्य))
    {
      अचिन्हित पूर्णांक seccomp_gen_options = 0;
      स्थिर अक्षर *annotation;

      libcrun_debug ("Initializing seccomp");
      annotation = find_annotation (container, "run.oci.seccomp_fail_unknown_syscall");
      अगर (annotation && म_भेद (annotation, "0") != 0)
        seccomp_gen_options = LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL;

      अगर (seccomp_bpf_तथ्य)
        seccomp_gen_options |= LIBCRUN_SECCOMP_SKIP_CACHE;

      libcrun_seccomp_gen_ctx_init (seccomp_gen_ctx, container, true, seccomp_gen_options);

      ret = libcrun_open_seccomp_bpf (seccomp_gen_ctx, seccomp_fd, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  वापस 0;
}

अटल पूर्णांक
setup_console_socket (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
                      काष्ठा container_entrypoint_s *container_args, पूर्णांक *console_socket_fd,
                      libcrun_error_t *err)
{
  अगर (context->console_socket && def->process && def->process->terminal)
    {
      *console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
      अगर (UNLIKELY (*console_socket_fd < 0))
        वापस crun_error_wrap (err, "open console socket");
      container_args->console_socket_fd = *console_socket_fd;
    }
  वापस 0;
}

अटल पूर्णांक
setup_cgroup_manager (libcrun_context_t *context, libcrun_container_t *container,
                      काष्ठा libcrun_cgroup_args *cg, पूर्णांक *cgroup_dirfd,
                      काष्ठा libcrun_dirfd_s *cgroup_dirfd_s, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णांक cgroup_manager;
  uid_t मूल_uid = -1;
  gid_t मूल_gid = -1;
  पूर्णांक ret;

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  अगर (context->systemd_cgroup)
    {
      libcrun_debug ("Using systemd cgroup manager");
      cgroup_manager = CGROUP_MANAGER_SYSTEMD;
    }
  अन्यथा अगर (context->force_no_cgroup)
    {
      libcrun_debug ("Disabling cgroup manager");
      cgroup_manager = CGROUP_MANAGER_DISABLED;
    }
  अन्यथा
    libcrun_debug ("Using cgroupfs cgroup manager");

  /* If we are root (either on the host or in a namespace), then chown the cgroup to root
     in the container user namespace.  */
  get_मूल_in_the_userns (def, container->host_uid, container->host_gid, &मूल_uid, &मूल_gid);
  libcrun_debug ("Using container host UID `%d` and GID `%d`", container->host_uid, container->host_gid);

  स_रखो (cg, 0, माप (*cg));

  cg->cgroup_path = def->linux ? def->linux->cgroups_path : "";
  cg->manager = cgroup_manager;
  cg->id = context->id;
  cg->resources = def->linux ? def->linux->resources : शून्य;
  cg->annotations = container->annotations;
  cg->मूल_uid = मूल_uid;
  cg->मूल_gid = मूल_gid;
  cg->state_मूल = context->state_मूल;
  cg->container = container;

  ret = libcrun_cgroup_preenter (cg, cgroup_dirfd, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  cgroup_dirfd_s->dirfd = cgroup_dirfd;
  cgroup_dirfd_s->joined = false;

  वापस 0;
}

अटल पूर्णांक
set_scheduler (pid_t pid, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = libcrun_set_scheduler (pid, def->process, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_reset_cpu_affinity_mask (pid, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_set_io_priority (pid, def->process, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस 0;
}

अटल पूर्णांक
seccomp_generation (पूर्णांक seccomp_fd, स्थिर अक्षर *seccomp_bpf_तथ्य,
                    काष्ठा libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, libcrun_error_t *err)
{
  पूर्णांक ret;

  अगर (seccomp_fd >= 0)
    {
      अगर (seccomp_bpf_तथ्य != शून्य)
        {
          ret = libcrun_copy_seccomp (seccomp_gen_ctx, seccomp_bpf_तथ्य, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }
      अन्यथा
        {
          ret = libcrun_generate_seccomp (seccomp_gen_ctx, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }
    }
  वापस 0;
}

अटल पूर्णांक
terminal_setup (runtime_spec_schema_config_schema *def, libcrun_context_t *context,
                पूर्णांक socket_pair_0, पूर्णांक *terminal_fd, व्योम **orig_terminal, libcrun_error_t *err)
{
  पूर्णांक ret;
  पूर्णांक detach = context->detach;

  अगर (def->process && def->process->terminal && ! detach && context->console_socket == शून्य)
    {
      libcrun_debug ("Receiving console socket fd");
      *terminal_fd = receive_fd_from_socket (socket_pair_0, err);
      अगर (UNLIKELY (*terminal_fd < 0))
        वापस -1;

      ret = libcrun_clear_onlcr (*terminal_fd, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = libcrun_set_raw (0, orig_terminal, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  वापस 0;
}

अटल पूर्णांक
libcrun_container_run_internal (libcrun_container_t *container, libcrun_context_t *context,
                                पूर्णांक *container_ready_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णांक ret;
  pid_t pid;
  पूर्णांक detach = context->detach;
  cleanup_cgroup_status काष्ठा libcrun_cgroup_status *cgroup_status = शून्य;
  cleanup_close पूर्णांक terminal_fd = -1;
  cleanup_terminal व्योम *orig_terminal = शून्य;
  cleanup_close पूर्णांक sync_socket = -1;
  cleanup_close पूर्णांक notify_socket = -1;
  cleanup_close पूर्णांक socket_pair_0 = -1;
  cleanup_close पूर्णांक socket_pair_1 = -1;
  cleanup_close पूर्णांक seccomp_fd = -1;
  cleanup_close पूर्णांक console_socket_fd = -1;
  cleanup_close पूर्णांक hooks_out_fd = -1;
  cleanup_close पूर्णांक hooks_err_fd = -1;
  cleanup_close पूर्णांक own_seccomp_receiver_fd = -1;
  cleanup_close पूर्णांक seccomp_notify_fd = -1;
  स्थिर अक्षर *seccomp_notify_plugins = शून्य;
  cleanup_cgroup_preenter काष्ठा libcrun_cgroup_args cg = {};
  cleanup_free अक्षर *bundle_allocated = शून्य;
  काष्ठा container_entrypoint_s container_args = {
    .container = container,
    .context = context,
    .bundle = get_bundle_path (context->bundle, &bundle_allocated),
    .terminal_socketpair = { -1, -1 },
    .console_socket_fd = -1,
    .hooks_out_fd = -1,
    .hooks_err_fd = -1,
    .seccomp_receiver_fd = -1,
    .custom_handler = शून्य,
  };
  cleanup_close पूर्णांक cgroup_dirfd = -1;
  काष्ठा libcrun_dirfd_s cgroup_dirfd_s;
  काष्ठा libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  स्थिर अक्षर *seccomp_bpf_तथ्य = find_annotation (container, "run.oci.seccomp_bpf_data");
  पूर्णांक cgroup_mode;

  अगर (! context->force_no_cgroup)
    {
      cgroup_mode = libcrun_get_cgroup_mode (err);
      अगर (UNLIKELY (cgroup_mode < 0))
        वापस cgroup_mode;

      अगर (cgroup_mode != CGROUP_MODE_UNIFIED)
        libcrun_warning ("cgroup v1 is deprecated and will be removed in a future release.  Use cgroup v2");
    }

  ret = setup_container_hooks_output (container, def, &container_args, &hooks_out_fd, &hooks_err_fd, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  container->context = context;

  अगर (! detach || context->notify_socket)
    {
      libcrun_debug ("Setting child subreaper");
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "prctl set child subreaper");
    }

  ret = setup_container_keyring (container, context, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = setup_terminal_socket_pair (container, context, &container_args, &socket_pair_0, &socket_pair_1, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = block_signals (err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  umask (0);

  ret = libcrun_set_mempolicy (def, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = setup_seccomp (container, seccomp_bpf_तथ्य, &seccomp_gen_ctx, &seccomp_fd, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;
  container_args.seccomp_fd = seccomp_fd;

  अगर (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &container_args.seccomp_receiver_fd, &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  ret = setup_console_socket (context, def, &container_args, &console_socket_fd, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = setup_cgroup_manager (context, container, &cg, &cgroup_dirfd, &cgroup_dirfd_s, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_configure_handler (container_args.context->handler_manager,
                                   container_args.context,
                                   container,
                                   &(container_args.custom_handler),
                                   err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (container_args.custom_handler && container_args.custom_handler->vtable->modify_oci_configuration)
    {
      libcrun_debug ("Using custom handler to modify OCI configuration");
      ret = container_args.custom_handler->vtable->modify_oci_configuration (container_args.custom_handler->cookie,
                                                                             container_args.context,
                                                                             container->container_def,
                                                                             err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  pid = libcrun_run_linux_container (container, container_init, &container_args, &sync_socket, &cgroup_dirfd_s, err);
  अगर (UNLIKELY (pid < 0))
    वापस pid;

  cg.pid = pid;
  cg.joined = cgroup_dirfd_s.joined;
  libcrun_debug ("Running container on PID: `%d`", pid);

  अगर (context->fifo_exec_wait_fd < 0 && context->notify_socket)
    {
      libcrun_debug ("Using notify socket: `%s`", context->notify_socket);
      /* Do not open the notify socket here on "create".  "start" will take care of it.  */
      ret = get_notify_fd (context, container, &notify_socket, err);
      अगर (UNLIKELY (ret < 0))
        जाओ fail;
    }

  अगर (container_args.terminal_socketpair[1] >= 0)
    close_and_reset (&socket_pair_1);

  ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
  अगर (UNLIKELY (ret < 0))
    जाओ fail;

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE, err);
  अगर (UNLIKELY (ret < 0))
    जाओ fail;

  ret = libcrun_move_network_devices (container, pid, err);
  अगर (UNLIKELY (ret < 0))
    जाओ fail;

  /* sync send own pid.  */
  ret = अस्थायी_FAILURE_RETRY (ग_लिखो (sync_socket, &pid, माप (pid)));
  अगर (UNLIKELY (ret != माप (pid)))
    {
      अगर (ret >= 0)
        त्रुटि_सं = 0;
      crun_make_error (err, त्रुटि_सं, "write to sync socket");
      जाओ fail;
    }

  /* sync 1.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  अगर (UNLIKELY (ret < 0))
    जाओ fail;

  /* sync 2.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  अगर (UNLIKELY (ret < 0))
    जाओ fail;

  ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
  अगर (UNLIKELY (ret < 0))
    जाओ fail;

  /* If the container process is created later, the scheduler is set on it then.  */
  अगर (! libcrun_container_delayed_pidns_fork (container))
    {
      ret = set_scheduler (pid, def, err);
      अगर (UNLIKELY (ret < 0))
        जाओ fail;
    }

  /* The container is waiting that we write back.  In this phase we can launch the
     prestart hooks.  */
  अगर (def->hooks && def->hooks->prestart_len)
    {
      libcrun_debug ("Running `prestart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "prestart", (hook **) def->hooks->prestart,
                      def->hooks->prestart_len, hooks_out_fd, hooks_err_fd, false, err);
      अगर (UNLIKELY (ret != 0))
        जाओ fail;
    }
  अगर (def->hooks && def->hooks->create_runtime_len)
    {
      libcrun_debug ("Running `create` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "createRuntime", (hook **) def->hooks->create_runtime,
                      def->hooks->create_runtime_len, hooks_out_fd, hooks_err_fd, false, err);
      अगर (UNLIKELY (ret != 0))
        जाओ fail;
    }

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_तथ्य, &seccomp_gen_ctx, err);
  अगर (UNLIKELY (ret < 0))
    जाओ fail;
  close_and_reset (&seccomp_fd);

  /* sync 3.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  अगर (UNLIKELY (ret < 0))
    जाओ fail;

  /* The process joining an existing PID namespace is created once the container root is in place.  */
  अगर (libcrun_container_delayed_pidns_fork (container))
    {
      pid_t new_pid = 0;

      ret = sync_socket_wait_sync (context, sync_socket, false, err);
      अगर (UNLIKELY (ret < 0))
        जाओ fail;

      ret = अस्थायी_FAILURE_RETRY (पढ़ो (sync_socket, &new_pid, माप (new_pid)));
      अगर (UNLIKELY (ret != माप (new_pid)))
        {
          अगर (ret >= 0)
            त्रुटि_सं = 0;
          crun_make_error (err, त्रुटि_सं, "read pid from sync socket");
          जाओ fail;
        }

      /* Cleanup the first process.  */
      waitpid_ignore_stopped (pid, शून्य, 0);

      pid = new_pid;

      ret = set_scheduler (pid, def, err);
      अगर (UNLIKELY (ret < 0))
        जाओ fail;

      ret = libcrun_handle_pidfd_receiver (pid, container, err);
      अगर (UNLIKELY (ret < 0))
        जाओ fail;

      /* sync send own pid.  */
      ret = अस्थायी_FAILURE_RETRY (ग_लिखो (sync_socket, &pid, माप (pid)));
      अगर (UNLIKELY (ret != माप (pid)))
        {
          अगर (ret >= 0)
            त्रुटि_सं = 0;
          crun_make_error (err, त्रुटि_सं, "write to sync socket");
          जाओ fail;
        }
    }

  ret = terminal_setup (def, context, socket_pair_0, &terminal_fd, &orig_terminal, err);
  अगर (UNLIKELY (ret < 0))
    जाओ fail;

  close_and_reset (&socket_pair_0);

  /* sync 4.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  अगर (UNLIKELY (ret < 0))
    जाओ fail;

  ret = close_and_reset (&sync_socket);
  अगर (UNLIKELY (ret < 0))
    {
      crun_make_error (err, त्रुटि_सं, "close sync_socket failed");
      जाओ fail;
    }

  libcrun_debug ("Writing container status");
  ret = write_container_status (container, context, pid, cgroup_status, err);
  अगर (UNLIKELY (ret < 0))
    जाओ fail;

  /* Run poststart hooks here only if the container is created using "run".  For create+start, the
     hooks will be executed as part of the start command.  */
  अगर (context->fifo_exec_wait_fd < 0 && def->hooks && def->hooks->poststart_len)
    {
      libcrun_debug ("Running `poststart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      अगर (UNLIKELY (ret != 0))
        जाओ fail;
    }

  /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
  अगर (own_seccomp_receiver_fd >= 0)
    {
      libcrun_debug ("Receiving seccomp fd");
      seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
      अगर (UNLIKELY (seccomp_notify_fd < 0))
        जाओ fail;

      ret = close_and_reset (&own_seccomp_receiver_fd);
      अगर (UNLIKELY (ret < 0))
        {
          crun_make_error (err, त्रुटि_सं, "close seccomp receiver fd failed");
          जाओ fail;
        }
    }

  {
    काष्ठा wait_for_process_args args = {
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
  अगर (! context->detach)
    {
      libcrun_error_t tmp_err = शून्य;
      cleanup_watch (context, def, cgroup_status, 0, sync_socket, terminal_fd, &tmp_err);
      crun_error_release (&tmp_err);
    }

  वापस ret;

fail:
  ret = cleanup_watch (context, def, cgroup_status, pid, sync_socket, terminal_fd, err);
  अगर (cgroup_status)
    {
      libcrun_error_t tmp_err = शून्य;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  वापस ret;
}

अटल पूर्णांक
check_config_file (runtime_spec_schema_config_schema *def, libcrun_context_t *context, libcrun_error_t *err)
{
  अगर (UNLIKELY (def->linux == शून्य))
    वापस crun_make_error (err, 0, "invalid config file, no `linux` block specified");

  अगर (context->handler == शून्य)
    {
      अगर (UNLIKELY (def->मूल == शून्य))
        वापस crun_make_error (err, 0, "invalid config file, no `root` block specified");
      अगर (UNLIKELY (def->mounts == शून्य))
        वापस crun_make_error (err, 0, "invalid config file, no `mounts` block specified");
    }
  वापस 0;
}

अटल पूर्णांक
libcrun_copy_config_file (स्थिर अक्षर *id, स्थिर अक्षर *state_मूल, libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णांक ret;
  cleanup_free अक्षर *dest_path = शून्य;
  cleanup_free अक्षर *dir = शून्य;
  cleanup_free अक्षर *बफर = शून्य;
  माप_प्रकार len;

  ret = libcrun_get_state_directory (&dir, state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = append_paths (&dest_path, err, dir, "config.json", शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (container->config_file == शून्य && container->config_file_content == शून्य)
    वापस crun_make_error (err, 0, "config file not specified");

  अगर (container->config_file == शून्य)
    {
      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, container->config_file_content, म_माप (container->config_file_content), err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  अन्यथा
    {
      libcrun_debug ("Reading config file: `%s`", container->config_file);
      ret = read_all_file (container->config_file, &बफर, &len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, बफर, len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  वापस 0;
}

अटल व्योम
force_delete_container_status (libcrun_context_t *context, libcrun_container_t *container,
                               runtime_spec_schema_config_schema *def)
{
  libcrun_error_t tmp_err = शून्य;
  container_delete_internal (context, container, def, context->id, true, false, &tmp_err);
  libcrun_error_report_and_release (&tmp_err);
}

/* Serialize ERR to FD as an int errno followed by a NUL-terminated message.
   Used by forked children to report a failure to the parent process.  */
अटल व्योम
write_error_to_pipe (पूर्णांक fd, libcrun_error_t *err)
{
  स्थिर अक्षर *संदेश = (*err)->संदेश;
  पूर्णांक errcode = crun_error_get_errno (err);

  अस्थायी_FAILURE_RETRY (ग_लिखो (fd, &errcode, माप (errcode)));
  अस्थायी_FAILURE_RETRY (ग_लिखो (fd, संदेश, म_माप (संदेश) + 1));
}

पूर्णांक
libcrun_container_run (libcrun_context_t *context, libcrun_container_t *container, अचिन्हित पूर्णांक options,
                       libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णांक ret;
  पूर्णांक detach = context->detach;
  पूर्णांक container_ret_status[2];
  cleanup_close पूर्णांक pipefd0 = -1;
  cleanup_close पूर्णांक pipefd1 = -1;
  libcrun_error_t tmp_err = शून्य;

  container->context = context;

  ret = validate_options (options, LIBCRUN_RUN_OPTIONS_PREFORK | LIBCRUN_RUN_OPTIONS_KEEP, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = check_config_file (def, context, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (def->process && def->process->terminal && detach && context->console_socket == शून्य)
    वापस crun_make_error (err, 0, "use --console-socket with --detach when a terminal is used");

  ret = libcrun_status_check_directories (context->state_मूल, context->id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (! detach && (options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      ret = libcrun_copy_config_file (context->id, context->state_मूल, container, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = libcrun_container_run_internal (container, context, शून्य, err);
      अगर (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
        force_delete_container_status (context, container, def);
      वापस ret;
    }

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  ret = विभाजन ();
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "fork");
  अगर (ret)
    {
      पूर्णांक status;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, शून्य, 0);

      ret = अस्थायी_FAILURE_RETRY (पढ़ो (pipefd0, &status, माप (status)));
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, त्रुटि_सं, "read from sync pipe");

      अगर (status < 0)
        {
          पूर्णांक errno_;
          अक्षर buf[512];
          ret = अस्थायी_FAILURE_RETRY (पढ़ो (pipefd0, &errno_, माप (errno_)));
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "read from sync pipe");

          ret = अस्थायी_FAILURE_RETRY (पढ़ो (pipefd0, buf, माप (buf) - 1));
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "read from sync pipe");
          buf[ret] = '\0';

          वापस crun_make_error (err, errno_, "%s", buf);
        }

      वापस status;
    }

  close_and_reset (&pipefd0);

  /* forked process.  */
  ret = detach_process ();
  अगर (UNLIKELY (ret < 0))
    libcrun_fail_with_error (त्रुटि_सं, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_मूल, container, &tmp_err);
  अगर (UNLIKELY (ret < 0))
    जाओ fail;

  ret = libcrun_container_run_internal (container, context, शून्य, &tmp_err);
  अस्थायी_FAILURE_RETRY (ग_लिखो (pipefd1, &ret, माप (ret)));
  अगर (UNLIKELY (ret < 0))
    जाओ fail;

  निकास (निकास_सफल);
fail:

  अगर (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
    force_delete_container_status (context, container, def);
  अगर (tmp_err)
    {
      write_error_to_pipe (pipefd1, &tmp_err);
      crun_error_release (&tmp_err);
    }

  निकास (निकास_त्रुटि);
}

पूर्णांक
libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container, अचिन्हित पूर्णांक options,
                          libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णांक ret;
  पूर्णांक container_ready_pipe[2];
  cleanup_close पूर्णांक pipefd0 = -1;
  cleanup_close पूर्णांक pipefd1 = -1;
  cleanup_close पूर्णांक exec_fifo_fd = -1;
  context->detach = 1;

  libcrun_debug ("Creating container: `%s`", context->id);
  container->context = context;

  ret = validate_options (options, LIBCRUN_CREATE_OPTIONS_PREFORK, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = check_config_file (def, context, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (def->process && def->process->terminal && context->console_socket == शून्य)
    वापस crun_make_error (err, 0, "use --console-socket with create when a terminal is used");

  ret = libcrun_status_check_directories (context->state_मूल, context->id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  exec_fifo_fd = libcrun_status_create_exec_fifo (context->state_मूल, context->id, err);
  अगर (UNLIKELY (exec_fifo_fd < 0))
    वापस exec_fifo_fd;

  context->fifo_exec_wait_fd = exec_fifo_fd;
  exec_fifo_fd = -1;

  अगर ((options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      libcrun_debug ("Running with prefork enabled");
      ret = libcrun_copy_config_file (context->id, context->state_मूल, container, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
      ret = libcrun_container_run_internal (container, context, शून्य, err);
      अगर (UNLIKELY (ret < 0))
        force_delete_container_status (context, container, def);
      वापस ret;
    }

  ret = pipe2 (container_ready_pipe, O_CLOEXEC);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "pipe");
  pipefd0 = container_ready_pipe[0];
  pipefd1 = container_ready_pipe[1];

  ret = विभाजन ();
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "fork");
  अगर (ret)
    {
      पूर्णांक exit_code;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, शून्य, 0);

      ret = अस्थायी_FAILURE_RETRY (पढ़ो (pipefd0, &exit_code, माप (exit_code)));
      अगर (UNLIKELY (ret <= 0))
        वापस crun_make_error (err, ret < 0 ? त्रुटि_सं : 0, "waiting for container to be ready");

      अगर (ret != माप (exit_code) || exit_code < 0)
        वापस crun_make_error (err, 0, "internal error: read invalid exit code");

      अगर (exit_code == 0)
        वापस 0;

      libcrun_debug ("Exit code is `%d`, deleting container", exit_code);
      libcrun_error_t tmp_err = शून्य;
      container_delete_internal (context, शून्य, def, context->id, true, true, &tmp_err);
      libcrun_error_report_and_release (&tmp_err);
      वापस crun_make_error (err, 0, "error creating container");
    }

  /* forked process.  */
  ret = detach_process ();
  अगर (UNLIKELY (ret < 0))
    libcrun_fail_with_error (त्रुटि_सं, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_मूल, container, err);
  अगर (UNLIKELY (ret < 0))
    {
      पूर्णांक errcode = crun_error_get_errno (err);
      crun_error_release (err);
      libcrun_fail_with_error (errcode, "copy config file");
    }

  ret = libcrun_container_run_internal (container, context, &pipefd1, err);
  अगर (UNLIKELY (ret < 0))
    {
      force_delete_container_status (context, container, def);
      libcrun_error ((*err)->status, "%s", (*err)->संदेश);
      crun_error_release (err);
      crun_set_output_handler (log_write_to_stderr, शून्य);
    }

  अगर (pipefd1 >= 0)
    अस्थायी_FAILURE_RETRY (ग_लिखो (pipefd1, &ret, माप (ret)));
  निकास (ret ? निकास_त्रुटि : 0);
}

पूर्णांक
libcrun_container_start (libcrun_context_t *context, स्थिर अक्षर *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = शून्य;
  स्थिर अक्षर *state_मूल = context->state_मूल;
  runtime_spec_schema_config_schema *def;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_close पूर्णांक fd = -1;
  पूर्णांक ret;

  ret = libcrun_read_container_status (&status, state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_is_container_running (&status, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (! ret)
    वापस crun_make_error (err, 0, "container `%s` is not running", id);

  ret = read_container_config_from_state (&container, state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (context->notify_socket)
    {
      ret = get_notify_fd (context, container, &fd, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  ret = libcrun_status_write_exec_fifo (context->state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  def = container->container_def;

  अगर (context->notify_socket)
    {
      अगर (fd >= 0)
        {
          fd_set read_set;

          जबतक (1)
            {
              काष्ठा timeval timeout = {
                .tv_sec = 0,
                .tv_usec = 10000,
              };
              FD_ZERO (&read_set);
              FD_SET (fd, &read_set);

              ret = select (fd + 1, &read_set, शून्य, शून्य, &timeout);
              अगर (UNLIKELY (ret < 0))
                वापस crun_make_error (err, त्रुटि_सं, "select failed");
              अगर (ret)
                {
                  ret = handle_notify_socket (fd, err);
                  अगर (UNLIKELY (ret < 0))
                    वापस ret;
                  अगर (ret)
                    अवरोध;
                }
              अन्यथा
                {
                  ret = libcrun_is_container_running (&status, err);
                  अगर (UNLIKELY (ret < 0))
                    वापस ret;
                  अगर (! ret)
                    वापस 0;
                }
            }
        }
    }

  /* The container is considered running only after we got the notification from the
     notify_socket, if any.  */
  अगर (def->hooks && def->hooks->poststart_len)
    {
      cleanup_close पूर्णांक hooks_out_fd = -1;
      cleanup_close पूर्णांक hooks_err_fd = -1;

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = do_hooks (def, status.pid, context->id, false, status.bundle, status.bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      अगर (UNLIKELY (ret != 0))
        {
          libcrun_error_t tmp_err = शून्य;
          container_delete_internal (context, container, def, id, true, true, &tmp_err);
          libcrun_error_report_and_release (&tmp_err);
          वापस ret;
        }
    }

  वापस 0;
}

पूर्णांक
libcrun_get_container_state_string (स्थिर अक्षर *id, libcrun_container_status_t *status, स्थिर अक्षर *state_मूल,
                                    स्थिर अक्षर **container_status, पूर्णांक *running, libcrun_error_t *err)
{
  पूर्णांक ret, has_fifo = 0;
  bool paused = false;

  ret = libcrun_is_container_running (status, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;
  *running = ret;

  अगर (*running)
    {
      ret = libcrun_status_has_read_exec_fifo (state_मूल, id, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
      has_fifo = ret;
    }

  अगर (*running && ! has_fifo)
    {
      cleanup_cgroup_status काष्ठा libcrun_cgroup_status *cgroup_status = शून्य;

      cgroup_status = libcrun_cgroup_make_status (status);

      ret = libcrun_cgroup_is_container_paused (cgroup_status, &paused, err);
      अगर (UNLIKELY (ret < 0))
        {
          /*
            The cgroup might have been cleaned up by the time we try to read it, ignore both
            ENOENT and ENODEV:
            - ENOENT: if the open(CGROUP_PATH) fails because the cgroup was deleted.
            - ENODEV: if the cgroup is deleted between the open and reading the freeze status.
          */

          त्रुटि_सं = crun_error_get_errno (err);
          अगर (त्रुटि_सं == ENOENT || त्रुटि_सं == ENODEV)
            {
              crun_error_release (err);
              *container_status = "stopped";
              वापस 0;
            }

          वापस ret;
        }
    }

  अगर (! *running)
    *container_status = "stopped";
  अन्यथा अगर (has_fifo)
    *container_status = "created";
  अन्यथा अगर (paused)
    *container_status = "paused";
  अन्यथा
    *container_status = "running";

  वापस 0;
}

पूर्णांक
libcrun_container_state_json (libcrun_context_t *context, स्थिर अक्षर *id, अक्षर **out, libcrun_error_t *err)
{
  अक्षर *बफर = शून्य;
  माप_प्रकार len = 0;
  खाता *f;
  पूर्णांक ret;

  f = open_memstream (&बफर, &len);
  अगर (f == शून्य)
    वापस crun_make_error (err, त्रुटि_सं, "open_memstream");

  ret = libcrun_container_state (context, id, f, err);
  ख_बंद (f);
  अगर (UNLIKELY (ret < 0))
    {
      मुक्त (बफर);
      वापस ret;
    }

  *out = बफर;
  वापस 0;
}

पूर्णांक
libcrun_container_state (libcrun_context_t *context, स्थिर अक्षर *id, खाता *out, libcrun_error_t *err)
{
  स्थिर अक्षर *स्थिर OCI_CONFIG_VERSION = "1.0.0";
  libcrun_container_status_t status = {};
  स्थिर अक्षर *state_मूल = context->state_मूल;
  स्थिर अक्षर *container_status = शून्य;
  json_gen_ctx *gen = शून्य;
  स्थिर अक्षर *buf;
  पूर्णांक ret = 0;
  पूर्णांक r = json_gen_status_ok;
  पूर्णांक running;
  माप_प्रकार len;

  ret = libcrun_read_container_status (&status, state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_get_container_state_string (id, &status, state_मूल, &container_status, &running, err);
  अगर (UNLIKELY (ret < 0))
    जाओ निकास;

  ret = 0;
  अगर (! json_gen_init (&gen, शून्य))
    वापस crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));
  GEN_कुंजी (gen, "ociVersion");
  GEN_STR (gen, OCI_CONFIG_VERSION);

  GEN_कुंजी (gen, "id");
  GEN_STR (gen, id);

  GEN_कुंजी (gen, "pid");
  GEN_OR_FAIL (map_int (gen, running ? status.pid : 0));

  GEN_कुंजी (gen, "status");
  GEN_STR (gen, container_status);

  GEN_कुंजी (gen, "bundle");
  GEN_STR (gen, status.bundle);

  GEN_कुंजी (gen, "rootfs");
  GEN_STR (gen, status.rootfs);

  GEN_कुंजी (gen, "created");
  GEN_STR (gen, status.created);

  अगर (status.scope)
    {
      GEN_कुंजी (gen, "systemd-scope");
      GEN_STR (gen, status.scope);
    }

  अगर (status.owner)
    {
      GEN_कुंजी (gen, "owner");
      GEN_STR (gen, status.owner);
    }

  {
    cleanup_container libcrun_container_t *container = शून्य;

    ret = read_container_config_from_state (&container, state_मूल, id, err);
    अगर (UNLIKELY (ret < 0))
      जाओ निकास;

    अगर (container->container_def->annotations && container->container_def->annotations->len)
      GEN_OR_FAIL (gen_annotations (gen, container->container_def->annotations));
  }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  ख_लिखो (out, "%s\n", buf);

निकास:
  अगर (gen)
    json_gen_free (gen);
  libcrun_free_container_status (&status);
  वापस ret;

gen_error:
  ret = json_gen_error_to_crun_error (r, err);
  जाओ निकास;
}

पूर्णांक
libcrun_container_exec (libcrun_context_t *context, स्थिर अक्षर *id, runtime_spec_schema_config_schema_process *process,
                        libcrun_error_t *err)
{
  काष्ठा libcrun_container_exec_options_s opts;
  स_रखो (&opts, 0, माप (opts));

  opts.struct_आकार = माप (opts);
  opts.process = process;

  वापस libcrun_container_exec_with_options (context, id, &opts, err);
}

पूर्णांक
libcrun_container_exec_process_file (libcrun_context_t *context, स्थिर अक्षर *id, स्थिर अक्षर *path, libcrun_error_t *err)
{
  काष्ठा libcrun_container_exec_options_s opts;
  स_रखो (&opts, 0, माप (opts));

  opts.struct_आकार = माप (opts);
  opts.path = path;

  वापस libcrun_container_exec_with_options (context, id, &opts, err);
}

अटल पूर्णांक
exec_from_process_json (libcrun_context_t *context, स्थिर अक्षर *id, स्थिर अक्षर *process_json, स्थिर अक्षर *cgroup,
                        bool merge_env, libcrun_error_t *err)
{
  काष्ठा parser_context parser_ctx = { .options = 0, .errfile = मानक_त्रुटि };
  काष्ठा libcrun_container_exec_options_s opts;
  runtime_spec_schema_config_schema_process *process;
  parser_error parser_err = शून्य;
  json_object *doc = शून्य;
  पूर्णांक ret;

  ret = parse_json_file (&doc, process_json, &parser_ctx, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  process = make_runtime_spec_schema_config_schema_process (doc, &parser_ctx, &parser_err);
  json_object_put (doc);
  अगर (process == शून्य)
    {
      ret = crun_make_error (err, 0, "cannot parse process: `%s`", parser_err ? parser_err : "unknown error");
      मुक्त (parser_err);
      वापस ret;
    }

  स_रखो (&opts, 0, माप (opts));
  opts.struct_आकार = माप (opts);
  opts.process = process;
  opts.cgroup = cgroup;
  opts.merge_env = merge_env;

  ret = libcrun_container_exec_with_options (context, id, &opts, err);
  free_runtime_spec_schema_config_schema_process (process);
  वापस ret;
}

पूर्णांक
libcrun_container_exec_json (libcrun_context_t *context, स्थिर अक्षर *id, स्थिर अक्षर *process_json, libcrun_error_t *err)
{
  वापस exec_from_process_json (context, id, process_json, शून्य, false, err);
}

पूर्णांक
libcrun_container_exec_with_options_json (libcrun_context_t *context, स्थिर अक्षर *id,
                                          स्थिर काष्ठा libcrun_exec_options_s *opts, libcrun_error_t *err)
{
  अगर (opts == शून्य || opts->struct_आकार == 0)
    वापस crun_make_error (err, EINVAL, "invalid exec options");

  वापस exec_from_process_json (context, id, opts->process_json, opts->cgroup, opts->merge_env, err);
}

#घोषणा cleanup_process_schema __attribute__ ((cleanup (cleanup_process_schemap)))

अटल अंतरभूत व्योम
cleanup_process_schemap (runtime_spec_schema_config_schema_process **p)
{
  runtime_spec_schema_config_schema_process *process = *p;
  अगर (process)
    (व्योम) free_runtime_spec_schema_config_schema_process (process);
}

/* Build the seccomp receiver payload (when needed), apply the seccomp profile and
   close the seccomp file descriptors.  Used from exec_process_entrypoint both before
   and after the capabilities are set, depending on process->no_new_privileges.  */
अटल पूर्णांक
apply_seccomp_for_exec (libcrun_container_t *container, pid_t own_pid, पूर्णांक seccomp_fd,
                        पूर्णांक seccomp_receiver_fd, अक्षर **seccomp_flags, माप_प्रकार seccomp_flags_len,
                        libcrun_error_t *err)
{
  cleanup_free अक्षर *seccomp_fd_payload = शून्य;
  माप_प्रकार seccomp_fd_payload_len = 0;
  पूर्णांक ret;

  अगर (seccomp_receiver_fd >= 0)
    {
      ret = get_seccomp_receiver_fd_payload (container, "running", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  ret = libcrun_apply_seccomp (seccomp_fd, seccomp_receiver_fd, seccomp_fd_payload,
                               seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  close_and_reset (&seccomp_fd);
  close_and_reset (&seccomp_receiver_fd);
  वापस 0;
}

अटल पूर्णांक
exec_process_entrypoint (libcrun_context_t *context,
                         libcrun_container_t *container,
                         runtime_spec_schema_config_schema_process *process,
                         पूर्णांक *pipefd1,
                         पूर्णांक seccomp_fd,
                         पूर्णांक seccomp_receiver_fd,
                         काष्ठा custom_handler_instance_s *custom_handler,
                         bool merge_env,
                         libcrun_error_t *err)
{
  runtime_spec_schema_config_schema_process_capabilities *capabilities = शून्य;
  cleanup_free अक्षर *exec_path = शून्य;
  uid_t container_uid;
  gid_t container_gid;
  स्थिर अक्षर *cwd;
  bool chdir_done = false;
  माप_प्रकार seccomp_flags_len = 0;
  अक्षर **seccomp_flags = शून्य;
  pid_t own_pid = 0;
  माप_प्रकार i;
  पूर्णांक ret;

  container_uid = process->user ? process->user->uid : 0;
  container_gid = process->user ? process->user->gid : 0;

  अस्थायी_FAILURE_RETRY (पढ़ो (*pipefd1, &own_pid, माप (own_pid)));

  cwd = process->cwd ? process->cwd : "/";
  अगर (LIKELY (libcrun_safe_chdir (cwd, err) == 0))
    chdir_done = true;
  अन्यथा
    crun_error_release (err);

  ret = unblock_signals (err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = clearenv ();
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "clearenv");

  अगर ((merge_env || ! process->env_len) && container->container_def->process && container->container_def->process->env_len)
    {
      अक्षर *e;

      क्रम (i = 0; i < container->container_def->process->env_len; i++)
        {
          e = container->container_def->process->env[i];
          अगर (putenv (e) < 0)
            वापस crun_make_error (err, त्रुटि_सं, "putenv `%s`", e);
        }
    }
  अगर (process->env_len)
    {
      क्रम (i = 0; i < process->env_len; i++)
        अगर (putenv (process->env[i]) < 0)
          वापस crun_make_error (err, त्रुटि_सं, "putenv `%s`", process->env[i]);
    }

  अगर (is_empty_string (दो_पर्या ("HOME")))
    {
      ret = set_home_env (container_uid);
      अगर (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          अगर (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            वापस crun_make_error (err, त्रुटि_सं, "setenv HOME");
        }
    }

  ret = libcrun_set_selinux_label (container, process, false, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_set_apparmor_profile (container, process, false, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (container->container_def->linux && container->container_def->linux->seccomp)
    {
      seccomp_flags = container->container_def->linux->seccomp->flags;
      seccomp_flags_len = container->container_def->linux->seccomp->flags_len;
    }

  ret = find_executable (&exec_path, process->args[0], process->cwd, err);
  अगर (UNLIKELY (ret < 0))
    {
      अगर (crun_error_get_errno (err) == ENOENT)
        वापस ret;

      /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
         once the process switched to the use that runs in the container.  This might be necessary
         when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
         is not honored.  */
      crun_error_release (err);
    }

  अगर (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, false, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_reopen_dev_null (err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (! process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  ret = libcrun_container_setgroups (container, process, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = maybe_chown_std_streams (container_uid, container_gid, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (process->capabilities)
    capabilities = process->capabilities;
  अन्यथा अगर (container->container_def->process)
    capabilities = container->container_def->process->capabilities;

  ret = libcrun_set_caps (capabilities, container_uid, container_gid, process->no_new_privileges,
                          custom_handler != शून्य, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (UNLIKELY (exec_path == शून्य))
    {
      ret = find_executable (&exec_path, process->args[0], process->cwd, err);
      अगर (UNLIKELY (ret < 0))
        {
          अगर (custom_handler == शून्य || is_empty_string (process->args[0]))
            वापस ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          exec_path = xstrdup (process->args[0]);
        }
    }

  अगर (! chdir_done)
    {
      ret = libcrun_safe_chdir (cwd, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (process->user)
    umask (process->user->umask_present ? process->user->umask : 0022);

  ret = 0;
  अस्थायी_FAILURE_RETRY (ग_लिखो (*pipefd1, &ret, माप (ret)));
  अस्थायी_FAILURE_RETRY (बंद (*pipefd1));
  *pipefd1 = -1;

  अगर (custom_handler)
    {
      अगर (custom_handler->vtable->exec_func == शून्य)
        वापस crun_make_error (err, 0, "the handler does not support exec");

      अगर (context->argv)
        erase_initial_environment (context->argv, context->argc);

      ret = custom_handler->vtable->exec_func (custom_handler->cookie,
                                               container,
                                               exec_path,
                                               process->args);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, -ret, "exec container process failed with handler as `%s`", custom_handler->vtable->name);

      _safe_exit (निकास_त्रुटि);
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation, because the seccomp filter is already in place and it could
     stop some syscalls used by mark_or_close_fds_ge_than.
  */
  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, true, err);
  अगर (UNLIKELY (ret < 0))
    crun_error_release (err);

  अस्थायी_FAILURE_RETRY (execv (exec_path, process->args));
  libcrun_fail_with_error (त्रुटि_सं, "exec `%s`", exec_path);
}

/* Close the file descriptors inherited from the caller, other than those
   to be passed to the container process.  */
पूर्णांक
libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err)
{
  वापस mark_or_close_fds_ge_than (शून्य, context->preserve_fds + 3, true, err);
}

पूर्णांक
libcrun_load_process_from_file (स्थिर अक्षर *path, runtime_spec_schema_config_schema_process **process,
                                libcrun_error_t *err)
{
  काष्ठा parser_context ctx = { 0, मानक_त्रुटि };
  cleanup_free अक्षर *content = शून्य;
  parser_error parser_err = शून्य;
  json_object *doc = शून्य;
  माप_प्रकार len;
  पूर्णांक ret;

  ret = read_all_file (path, &content, &len, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  *process = make_runtime_spec_schema_config_schema_process (doc, &ctx, &parser_err);
  अगर (UNLIKELY (*process == शून्य))
    {
      ret = crun_make_error (err, 0, "cannot parse process file: `%s`", parser_err);
      मुक्त (parser_err);
      अगर (doc)
        json_object_put (doc);
      वापस ret;
    }

  मुक्त (parser_err);
  अगर (doc)
    json_object_put (doc);

  वापस 0;
}

पूर्णांक
libcrun_container_exec_with_options (libcrun_context_t *context, स्थिर अक्षर *id,
                                     काष्ठा libcrun_container_exec_options_s *opts,
                                     libcrun_error_t *err)
{
  cleanup_custom_handler_instance काष्ठा custom_handler_instance_s *custom_handler = शून्य;
  पूर्णांक container_status, ret;
  bool container_paused = false;
  pid_t pid;
  libcrun_container_status_t status = {};
  स्थिर अक्षर *state_मूल = context->state_मूल;
  cleanup_close पूर्णांक terminal_fd = -1;
  cleanup_close पूर्णांक seccomp_fd = -1;
  cleanup_terminal व्योम *orig_terminal = शून्य;
  cleanup_container libcrun_container_t *container = शून्य;
  पूर्णांक container_ret_status[2];
  cleanup_close पूर्णांक pipefd0 = -1;
  cleanup_close पूर्णांक pipefd1 = -1;
  cleanup_close पूर्णांक seccomp_receiver_fd = -1;
  cleanup_close पूर्णांक own_seccomp_receiver_fd = -1;
  cleanup_close पूर्णांक seccomp_notify_fd = -1;
  स्थिर अक्षर *seccomp_notify_plugins = शून्य;
  __attribute__ ((unused)) cleanup_process_schema runtime_spec_schema_config_schema_process *process_cleanup = शून्य;
  runtime_spec_schema_config_schema_process *process = opts->process;
  काष्ठा libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  पूर्णांक ret_from_child = 0;

  ret = libcrun_read_container_status (&status, state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_is_container_running (&status, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;
  container_status = ret;

  ret = read_container_config_from_state (&container, state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  container->context = context;

  अगर (container_status == 0)
    वापस crun_make_error (err, 0, "the container `%s` is not running", id);

  {
    cleanup_cgroup_status काष्ठा libcrun_cgroup_status *cgroup_status = शून्य;

    cgroup_status = libcrun_cgroup_make_status (&status);

    ret = libcrun_cgroup_is_container_paused (cgroup_status, &container_paused, err);
    अगर (UNLIKELY (ret < 0))
      वापस ret;
  }

  अगर (UNLIKELY (container_paused))
    वापस crun_make_error (err, 0, "cannot exec in a paused container `%s`", id);

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = block_signals (err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  libcrun_seccomp_gen_ctx_init (&seccomp_gen_ctx, container, false, 0);

  ret = libcrun_open_seccomp_bpf (&seccomp_gen_ctx, &seccomp_fd, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &seccomp_receiver_fd,
                                     &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  अगर (माप (*opts) != opts->struct_आकार)
    वापस crun_make_error (err, EINVAL, "invalid libcrun_container_exec_options_s struct");

  अगर (opts->path)
    {
      अगर (process)
        वापस crun_make_error (err, EINVAL, "cannot specify both exec file and options");

      ret = libcrun_load_process_from_file (opts->path, &process, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      process_cleanup = process;
    }

  /* This must be done before we enter a user namespace.  If the process does
     not specify any rlimits, use the ones from the container configuration.  */
  अगर (process->rlimits_len == 0 && container->container_def->process)
    ret = libcrun_set_rlimits (container->container_def->process->rlimits,
                               container->container_def->process->rlimits_len, err);
  अन्यथा
    ret = libcrun_set_rlimits (process->rlimits, process->rlimits_len, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_set_mempolicy (container->container_def, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  /* If the new process block doesn't specify a SELinux label, AppArmor profile, user,
     I/O priority or scheduler, then use the configuration from the original config file.  */
  अगर (container->container_def->process)
    {
      अगर (process->io_priority == शून्य && container->container_def->process->io_priority)
        {
          process->io_priority = clone_runtime_spec_schema_config_schema_process_io_priority (container->container_def->process->io_priority);
          अगर (process->io_priority == शून्य)
            OOM ();
        }

      अगर (process->scheduler == शून्य && container->container_def->process->scheduler)
        {
          process->scheduler = clone_runtime_spec_schema_config_schema_process_scheduler (container->container_def->process->scheduler);
          अगर (process->scheduler == शून्य)
            OOM ();
        }

      अगर (process->selinux_label == शून्य && container->container_def->process->selinux_label)
        process->selinux_label = xstrdup (container->container_def->process->selinux_label);

      अगर (process->apparmor_profile == शून्य && container->container_def->process->apparmor_profile)
        process->apparmor_profile = xstrdup (container->container_def->process->apparmor_profile);

      अगर (process->user == शून्य && container->container_def->process->user)
        {
          process->user = clone_runtime_spec_schema_config_schema_process_user (container->container_def->process->user);
          अगर (process->user == शून्य)
            OOM ();
        }
    }

  ret = initialize_security (container, process, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  /* Do not leak the session keyring of the caller into the container.  The
     keyring created for the container cannot be joined, so use an anonymous one.  */
  अगर (! context->no_new_keyring && ! status.no_new_keyring)
    {
      libcrun_debug ("Creating new keyring for the exec process");

      ret = libcrun_create_keyring (container, शून्य, process->selinux_label, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "prctl unset dumpable");

  pid = libcrun_join_process (context, container, status.pid, &status, opts->cgroup, context->detach,
                              process, process->terminal ? &terminal_fd : शून्य, err);
  अगर (UNLIKELY (pid < 0))
    वापस pid;

  /* Process to exec.  */
  अगर (pid == 0)
    {
      अस्थायी_FAILURE_RETRY (बंद (pipefd0));
      pipefd0 = -1;

      exec_process_entrypoint (context, container, process, &pipefd1, seccomp_fd, seccomp_receiver_fd, custom_handler, opts->merge_env, err);
      /* It gets here only on errors.  */
      अगर (*err)
        {
          अगर (pipefd1 < 0)
            libcrun_fail_with_error ((*err)->status, "%s", (*err)->संदेश);
          अन्यथा
            {
              write_error_to_pipe (pipefd1, err);
              अस्थायी_FAILURE_RETRY (बंद (pipefd1));
              pipefd1 = -1;
            }
        }
      _safe_exit (निकास_त्रुटि);
    }

  अस्थायी_FAILURE_RETRY (बंद (pipefd1));
  pipefd1 = -1;

  अस्थायी_FAILURE_RETRY (ग_लिखो (pipefd0, &pid, माप (pid)));

  अगर (seccomp_fd >= 0)
    close_and_reset (&seccomp_fd);

  अगर (terminal_fd >= 0)
    {
      अचिन्हित लघु rows = 0, cols = 0;

      अगर (process->console_आकार)
        {
          cols = process->console_आकार->width;
          rows = process->console_आकार->height;
        }

      ret = libcrun_terminal_setup_आकार (terminal_fd, rows, cols, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      अगर (context->console_socket)
        {
          पूर्णांक ret;
          cleanup_close पूर्णांक console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
          अगर (UNLIKELY (console_socket_fd < 0))
            वापस console_socket_fd;
          ret = send_fd_to_socket (console_socket_fd, terminal_fd, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
          close_and_reset (&terminal_fd);
        }
      अन्यथा
        {
          ret = libcrun_clear_onlcr (terminal_fd, err);
          अगर (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              वापस ret;
            }

          ret = libcrun_set_raw (0, &orig_terminal, err);
          अगर (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              वापस ret;
            }
        }
    }

  ret = अस्थायी_FAILURE_RETRY (पढ़ो (pipefd0, &ret_from_child, माप (ret_from_child)));
  अगर (ret != माप (ret_from_child))
    ret = crun_make_error (err, 0, "read pipe failed");
  अन्यथा अगर (ret_from_child != 0)
    {
      cleanup_free अक्षर *संदेश = शून्य;
      माप_प्रकार len = 0;

      ret = read_all_fd (pipefd0, "error stream", &संदेश, &len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
      /* the string from read_all_fd is always NUL terminated.  */
      ret = crun_make_error (err, ret_from_child, "%s", संदेश);
    }
  अन्यथा
    {
      /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
      अगर (own_seccomp_receiver_fd >= 0)
        {
          seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
          अगर (UNLIKELY (seccomp_notify_fd < 0))
            वापस seccomp_notify_fd;

          ret = close_and_reset (&own_seccomp_receiver_fd);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "close seccomp receiver fd failed");
        }

      {
        काष्ठा wait_for_process_args args = {
          .pid = pid,
          .context = context,
          .terminal_fd = terminal_fd,
          .notify_socket = -1,
          .container_ready_fd = शून्य,
          .seccomp_notify_fd = seccomp_notify_fd,
          .seccomp_notify_plugins = seccomp_notify_plugins,
        };

        ret = wait_for_process (&args, err);
      }
    }

  flush_fd_to_err (context, terminal_fd);
  वापस ret;
}

पूर्णांक
libcrun_container_update (libcrun_context_t *context, स्थिर अक्षर *id, स्थिर अक्षर *content, माप_प्रकार len arg_unused,
                          libcrun_error_t *err)
{
  cleanup_custom_handler_instance काष्ठा custom_handler_instance_s *custom_handler = शून्य;
  runtime_spec_schema_config_linux_resources *resources = शून्य;
  cleanup_container libcrun_container_t *container = शून्य;
  स्थिर अक्षर *state_मूल = context->state_मूल;
  काष्ठा parser_context ctx = { 0, मानक_त्रुटि };
  libcrun_container_status_t status = {};
  parser_error parser_err = शून्य;
  json_object *doc = शून्य;
  पूर्णांक ret;

  ret = libcrun_read_container_status (&status, state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = read_container_config_from_state (&container, state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  resources = make_runtime_spec_schema_config_linux_resources (doc, &ctx, &parser_err);
  अगर (UNLIKELY (resources == शून्य))
    {
      ret = crun_make_error (err, 0, "cannot parse resources: %s", parser_err);
      जाओ cleanup;
    }

  अगर (custom_handler && custom_handler->vtable->modify_oci_configuration)
    {
      /* Adapt RESOURCES to be used from the modify_oci_configuration hook.  */
      cleanup_free runtime_spec_schema_config_linux *linux = xmalloc0 (माप (*linux));
      cleanup_free runtime_spec_schema_config_schema *def = xmalloc0 (माप (*def));

      def->linux = linux;
      linux->resources = resources;

      ret = custom_handler->vtable->modify_oci_configuration (custom_handler->cookie,
                                                              context,
                                                              def,
                                                              err);
      अगर (UNLIKELY (ret < 0))
        जाओ cleanup;
    }

  ret = libcrun_linux_container_update (&status, state_मूल, resources, err);

cleanup:
  अगर (doc)
    json_object_put (doc);
  मुक्त (parser_err);
  अगर (resources)
    free_runtime_spec_schema_config_linux_resources (resources);

  वापस ret;
}

पूर्णांक
libcrun_container_update_from_file (libcrun_context_t *context, स्थिर अक्षर *id, स्थिर अक्षर *file, libcrun_error_t *err)
{
  cleanup_free अक्षर *content = शून्य;
  माप_प्रकार len;
  पूर्णांक ret;

  ret = read_all_file (file, &content, &len, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस libcrun_container_update (context, id, content, len, err);
}

अटल पूर्णांक
compare_update_मूल्यावली (स्थिर व्योम *a, स्थिर व्योम *b)
{
  स्थिर काष्ठा libcrun_update_मूल्य_s *aa = a;
  स्थिर काष्ठा libcrun_update_मूल्य_s *bb = b;
  पूर्णांक ret;

  ret = म_भेद (aa->section, bb->section);
  अगर (ret)
    वापस ret;
  वापस म_भेद (aa->name, bb->name);
}

पूर्णांक
libcrun_container_update_from_मूल्यावली (libcrun_context_t *context, स्थिर अक्षर *id,
                                      काष्ठा libcrun_update_मूल्य_s *मूल्यावली, माप_प्रकार len,
                                      libcrun_error_t *err)
{
  स्थिर अक्षर *current_section = शून्य;
  स्थिर अक्षर *buf;
  json_gen_ctx *gen = शून्य;
  माप_प्रकार i, buf_len;
  पूर्णांक ret;

  अगर (! json_gen_init (&gen, शून्य))
    वापस crun_make_error (err, 0, "json_gen_init failed");
  json_gen_map_open (gen);

  क्विक (मूल्यावली, len, माप (काष्ठा libcrun_update_मूल्य_s), compare_update_मूल्यावली);

  क्रम (i = 0; i < len; i++)
    {
      अगर (current_section == शून्य || म_भेद (मूल्यावली[i].section, current_section))
        {
          अगर (i > 0)
            json_gen_map_close (gen);

          current_section = मूल्यावली[i].section;
          json_gen_string (gen, current_section, म_माप (current_section));
          json_gen_map_open (gen);
        }

      json_gen_string (gen, मूल्यावली[i].name, म_माप (मूल्यावली[i].name));

      अगर (मूल्यावली[i].numeric)
        json_gen_संख्या (gen, मूल्यावली[i].मूल्य, म_माप (मूल्यावली[i].मूल्य));
      अन्यथा
        json_gen_string (gen, मूल्यावली[i].मूल्य, म_माप (मूल्यावली[i].मूल्य));
    }
  अगर (len)
    json_gen_map_close (gen);

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);

  ret = libcrun_container_update (context, id, buf, buf_len, err);

  json_gen_free (gen);

  वापस ret;
}

अटल व्योम
populate_array_field (अक्षर ***field, अक्षर *array[], माप_प्रकार अंक_elements)
{
  माप_प्रकार i;

  *field = xmalloc0 ((अंक_elements + 1) * माप (अक्षर *));
  क्रम (i = 0; i < अंक_elements; i++)
    (*field)[i] = xstrdup (array[i]);

  (*field)[i] = शून्य;
}

#अगर_घोषित HAVE_CAP
अटल व्योम
populate_capabilities (काष्ठा features_info_s *info, अक्षर ***capabilities, माप_प्रकार *अंक_capabilities)
{
  माप_प्रकार सूचकांक = 0;
  cap_मूल्य_t i;
  अक्षर *endptr;
  पूर्णांक j;

  *अंक_capabilities = 0;
  क्रम (i = 0;; i++)
    {
      अक्षर *v = cap_to_name (i);
      अगर (v == शून्य)
        अवरोध;
      म_से_दीर्घ (v, &endptr, 10);
      अगर (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          अवरोध;
        }
      (*अंक_capabilities)++;
    }

  *capabilities = xmalloc0 ((*अंक_capabilities + 1) * माप (स्थिर अक्षर *));
  क्रम (i = 0; i < (cap_मूल्य_t) *अंक_capabilities; i++)
    {
      अक्षर *v = cap_to_name (i);
      अगर (v == शून्य)
        अवरोध;
      म_से_दीर्घ (v, &endptr, 10);
      अगर (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          अवरोध;
        }

      // Convert capability name to uppercase
      क्रम (j = 0; v[j] != '\0'; j++)
        v[j] = बड़े (v[j]);

      (*capabilities)[सूचकांक] = v;
      सूचकांक++;
    }

  (*capabilities)[सूचकांक] = शून्य; // Terminate the array with NULL
  populate_array_field (&(info->linux.capabilities), *capabilities, *अंक_capabilities);
}
#पूर्ण_अगर

अटल व्योम
retrieve_mount_options (काष्ठा features_info_s **info)
{
  cleanup_free स्थिर काष्ठा propagation_flags_s *mount_options_list = शून्य;
  माप_प्रकार अंक_mount_options = 0;

  // Retrieve mount options from wordlist
  mount_options_list = get_mount_flags_from_wordlist ();

  // Calculate the number of mount options
  जबतक (mount_options_list[अंक_mount_options].name != शून्य)
    अंक_mount_options++;

  // Allocate memory for mount options in info struct
  (*info)->mount_options = xmalloc0 ((अंक_mount_options + 1) * माप (अक्षर *));

  // Copy mount options to info struct
  क्रम (माप_प्रकार i = 0; i < अंक_mount_options; i++)
    (*info)->mount_options[i] = xstrdup (mount_options_list[i].name);
}

पूर्णांक
libcrun_container_get_features (libcrun_context_t *context, काष्ठा features_info_s **info, libcrun_error_t *err arg_unused)
{
  // Allocate memory for the features_info_s structure
  माप_प्रकार अंक_namspaces = माप (namespaces) / माप (namespaces[0]);
  माप_प्रकार अंक_operators = माप (operators) / माप (operators[0]);
  माप_प्रकार अंक_actions = माप (actions) / माप (actions[0]);
  माप_प्रकार अंक_hooks = माप (hooks) / माप (hooks[0]);
  माप_प्रकार अंक_archs = माप (archs) / माप (archs[0]);
  माप_प्रकार अंक_unsafe_annotations = माप (potentially_unsafe_annotations) / माप (potentially_unsafe_annotations[0]);
  cleanup_free अक्षर **capabilities = शून्य;
  माप_प्रकार अंक_capabilities = 0;
  माप_प्रकार अंक_mempolicy_modes = माप (mempolicy_modes) / माप (mempolicy_modes[0]);
  माप_प्रकार अंक_mempolicy_flags = माप (mempolicy_flags) / माप (mempolicy_flags[0]);

  *info = xmalloc0 (माप (काष्ठा features_info_s));

  // Hardcoded feature information
  (*info)->oci_version_min = xstrdup ("1.0.0");
  (*info)->oci_version_max = xstrdup ("1.1.0+dev");

  // Populate hooks
  populate_array_field (&((*info)->hooks), hooks, अंक_hooks);

  // Populate mount_options
  retrieve_mount_options (info);

  // Populate namespaces
  populate_array_field (&((*info)->linux.namespaces), namespaces, अंक_namspaces);

#अगर_घोषित HAVE_CAP
  // Populate capabilities
  populate_capabilities (*info, &capabilities, &अंक_capabilities);
#पूर्ण_अगर

  // Hardcode the values for cgroup
  (*info)->linux.cgroup.v1 = true;
  (*info)->linux.cgroup.v2 = true;
#अगर_घोषित HAVE_SYSTEMD
  (*info)->linux.cgroup.systemd = true;
  (*info)->linux.cgroup.systemd_user = true;
#पूर्ण_अगर

  // Put seccomp values
#अगर_घोषित HAVE_SECCOMP
  (*info)->linux.seccomp.enabled = true;
  // Populate actions
  populate_array_field (&((*info)->linux.seccomp.actions), actions, अंक_actions);
  // Populate operators
  populate_array_field (&((*info)->linux.seccomp.operators), operators, अंक_operators);
  // Populate archs
  populate_array_field (&((*info)->linux.seccomp.archs), archs, अंक_archs);
#अन्यथा
  (*info)->linux.seccomp.enabled = false;
#पूर्ण_अगर

  // Put values for apparmor and selinux
  (*info)->linux.apparmor.enabled = true;
  (*info)->linux.selinux.enabled = true;

  (*info)->linux.intel_rdt.enabled = true;

  (*info)->linux.net_devices.enabled = true;

  populate_array_field (&((*info)->linux.memory_policy.mode), mempolicy_modes, अंक_mempolicy_modes);
  populate_array_field (&((*info)->linux.memory_policy.flags), mempolicy_flags, अंक_mempolicy_flags);

  // Put the values for mount extensions
  (*info)->linux.mount_ext.idmap.enabled = true;

  // Populate the values for annotations
#अगर_घोषित HAVE_SECCOMP
  {
    स्थिर काष्ठा scmp_version *version = seccomp_version ();
    अक्षर *version_string = शून्य;

    xasprintf (&version_string, "%u.%u.%u", version->major, version->minor, version->micro);
    (*info)->annotations.io_github_seccomp_libseccomp_version = version_string;
  }
#पूर्ण_अगर

  अगर (context->handler_manager && handler_by_name (context->handler_manager, "wasm"))
    (*info)->annotations.run_oci_crun_wasm = true;

#अगर HAVE_CRIU
  (*info)->annotations.run_oci_crun_checkpoint_enabled = true;
#पूर्ण_अगर
  (*info)->annotations.run_oci_crun_commit = GIT_VERSION;
  (*info)->annotations.run_oci_crun_version = PACKAGE_VERSION;

  populate_array_field (&((*info)->potentially_unsafe_annotations), potentially_unsafe_annotations, अंक_unsafe_annotations);

  वापस 0;
}

अटल व्योम
features_add_string (json_gen_ctx *json_gen, स्थिर अक्षर *कुंजी, स्थिर अक्षर *मूल्य)
{
  json_gen_string (json_gen, कुंजी, म_माप (कुंजी));
  json_gen_string (json_gen, मूल्य, म_माप (मूल्य));
}

अटल व्योम
features_add_bool (json_gen_ctx *json_gen, स्थिर अक्षर *कुंजी, पूर्णांक मूल्य)
{
  json_gen_string (json_gen, कुंजी, म_माप (कुंजी));
  json_gen_bool (json_gen, मूल्य);
}

अटल व्योम
features_add_bool_str (json_gen_ctx *json_gen, स्थिर अक्षर *कुंजी, पूर्णांक मूल्य)
{
  features_add_string (json_gen, कुंजी, मूल्य ? "true" : "false");
}

अटल व्योम
features_add_array (json_gen_ctx *json_gen, स्थिर अक्षर *कुंजी, अक्षर **array)
{
  माप_प्रकार i;

  json_gen_string (json_gen, कुंजी, म_माप (कुंजी));
  json_gen_array_open (json_gen);
  अगर (array)
    क्रम (i = 0; array[i] != शून्य; i++)
      json_gen_string (json_gen, array[i], म_माप (array[i]));
  json_gen_array_close (json_gen);
}

अटल व्योम
features_add_linux_info (json_gen_ctx *json_gen, स्थिर काष्ठा linux_info_s *linux)
{
  json_gen_string (json_gen, "linux", म_माप ("linux"));
  json_gen_map_open (json_gen);

  features_add_array (json_gen, "namespaces", linux->namespaces);
  features_add_array (json_gen, "capabilities", linux->capabilities);

  json_gen_string (json_gen, "cgroup", म_माप ("cgroup"));
  json_gen_map_open (json_gen);
  features_add_bool (json_gen, "v1", linux->cgroup.v1);
  features_add_bool (json_gen, "v2", linux->cgroup.v2);
  features_add_bool (json_gen, "systemd", linux->cgroup.systemd);
  features_add_bool (json_gen, "systemdUser", linux->cgroup.systemd_user);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "seccomp", म_माप ("seccomp"));
  json_gen_map_open (json_gen);
  features_add_bool (json_gen, "enabled", linux->seccomp.enabled);
  अगर (linux->seccomp.actions)
    features_add_array (json_gen, "actions", linux->seccomp.actions);
  अगर (linux->seccomp.operators)
    features_add_array (json_gen, "operators", linux->seccomp.operators);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "apparmor", म_माप ("apparmor"));
  json_gen_map_open (json_gen);
  features_add_bool (json_gen, "enabled", linux->apparmor.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "selinux", म_माप ("selinux"));
  json_gen_map_open (json_gen);
  features_add_bool (json_gen, "enabled", linux->selinux.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "mountExtensions", म_माप ("mountExtensions"));
  json_gen_map_open (json_gen);
  json_gen_string (json_gen, "idmap", म_माप ("idmap"));
  json_gen_map_open (json_gen);
  features_add_bool (json_gen, "enabled", linux->mount_ext.idmap.enabled);
  json_gen_map_close (json_gen);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "intelRdt", म_माप ("intelRdt"));
  json_gen_map_open (json_gen);
  features_add_bool (json_gen, "enabled", linux->intel_rdt.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "netDevices", म_माप ("netDevices"));
  json_gen_map_open (json_gen);
  features_add_bool (json_gen, "enabled", linux->net_devices.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "memoryPolicy", म_माप ("memoryPolicy"));
  json_gen_map_open (json_gen);
  अगर (linux->memory_policy.mode)
    features_add_array (json_gen, "modes", linux->memory_policy.mode);
  अगर (linux->memory_policy.flags)
    features_add_array (json_gen, "flags", linux->memory_policy.flags);
  json_gen_map_close (json_gen);

  json_gen_map_close (json_gen);
}

अटल व्योम
features_add_annotations_info (json_gen_ctx *json_gen, स्थिर काष्ठा annotations_info_s *annotation)
{
  json_gen_string (json_gen, "annotations", म_माप ("annotations"));
  json_gen_map_open (json_gen);

  अगर (! is_empty_string (annotation->io_github_seccomp_libseccomp_version))
    features_add_string (json_gen, "io.github.seccomp.libseccomp.version", annotation->io_github_seccomp_libseccomp_version);

  features_add_bool_str (json_gen, "org.opencontainers.runc.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);
  features_add_bool_str (json_gen, "run.oci.crun.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);

  features_add_string (json_gen, "run.oci.crun.commit", annotation->run_oci_crun_commit);
  features_add_string (json_gen, "run.oci.crun.version", annotation->run_oci_crun_version);

  features_add_bool_str (json_gen, "run.oci.crun.wasm", annotation->run_oci_crun_wasm);

  json_gen_map_close (json_gen);
}

पूर्णांक
libcrun_container_get_features_json (libcrun_context_t *context, अक्षर **out, libcrun_error_t *err)
{
  cleanup_struct_features काष्ठा features_info_s *info = शून्य;
  स्थिर अक्षर *बफर = शून्य;
  json_gen_ctx *json_gen;
  माप_प्रकार बफर_len;
  पूर्णांक ret;

  *out = शून्य;

  ret = libcrun_container_get_features (context, &info, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (! json_gen_init (&json_gen, शून्य))
    वापस crun_make_error (err, 0, "cannot initialize JSON generator");

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

  json_gen_get_buf (json_gen, &बफर, &बफर_len);
  *out = xstrdup (बफर);
  json_gen_free (json_gen);

  वापस 0;
}

/* Read the container status and fail if the container is not running.  */
अटल पूर्णांक
read_status_require_running (libcrun_context_t *context, स्थिर अक्षर *id,
                             libcrun_container_status_t *status, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = libcrun_read_container_status (status, context->state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_is_container_running (status, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;
  अगर (ret == 0)
    वापस crun_make_error (err, 0, "the container `%s` is not running", id);

  वापस 0;
}

पूर्णांक
libcrun_container_pause (libcrun_context_t *context, स्थिर अक्षर *id, libcrun_error_t *err)
{
  पूर्णांक ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस libcrun_container_pause_linux (&status, err);
}

पूर्णांक
libcrun_container_unpause (libcrun_context_t *context, स्थिर अक्षर *id, libcrun_error_t *err)
{
  पूर्णांक ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस libcrun_container_unpause_linux (&status, err);
}

/* Translate the public options struct into the internal representation used by
   the linux/CRIU layer.  Strings are borrowed (not copied).  */
अटल व्योम
cr_options_from_public (libcrun_checkpoint_restore_t *dst, काष्ठा libcrun_checkpoint_restore_options_s *src)
{
  स_रखो (dst, 0, माप (*dst));
  dst->image_path = (अक्षर *) src->image_path;
  dst->work_path = (अक्षर *) src->work_path;
  dst->parent_path = (अक्षर *) src->parent_path;
  dst->console_socket = src->console_socket;
  dst->lsm_profile = (अक्षर *) src->lsm_profile;
  dst->lsm_mount_context = (अक्षर *) src->lsm_mount_context;
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

पूर्णांक
libcrun_container_checkpoint (libcrun_context_t *context, स्थिर अक्षर *id,
                              काष्ठा libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  पूर्णांक ret;
  स्थिर अक्षर *state_मूल = context->state_मूल;
  libcrun_container_status_t status = {};
  libcrun_checkpoint_restore_t cr;
  cleanup_container libcrun_container_t *container = शून्य;

  cr_options_from_public (&cr, cr_options);

  ret = read_status_require_running (context, id, &status, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = read_container_config_from_state (&container, state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  /* Needed to use the same paths (e.g. the shared empty directory used for
     masked paths) as the container runtime and restore do.  */
  container->context = context;

  ret = libcrun_container_checkpoint_linux (&status, container, &cr, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (! (cr_options->leave_running || cr_options->pre_dump))
    वापस container_delete_internal (context, शून्य, शून्य, id, true, true, err);

  वापस 0;
}

/* The dummy process only keeps the cgroup populated until the container is restored there.
   Its cgroup is below the container cgroup, so once the container runs it can freeze or kill
   the dummy process: never depend on it after that point.  */
अटल व्योम
restore_proxy_process (पूर्णांक proxy_pid_pipe)
{
  अक्षर c;

  /* Wait to be killed by the parent, or for the parent to go away.  */
  अस्थायी_FAILURE_RETRY (पढ़ो (proxy_pid_pipe, &c, माप (c)));
}

अटल पूर्णांक
restore_terminate_proxy_process (pid_t proxy_pid, स्थिर अक्षर *proxy_cgroup, pid_t container_pid, पूर्णांक cgroup_manager, libcrun_error_t *err)
{
  पूर्णांक mode;
  पूर्णांक ret;

  अगर (cgroup_manager == CGROUP_MANAGER_SYSTEMD)
    {
      अक्षर ready_str[64];

      ret = snprintf (ready_str, माप (ready_str), "MAINPID=%d", container_pid);
      अगर (UNLIKELY (ret >= (पूर्णांक) माप (ready_str)))
        वापस crun_make_error (err, 0, "internal error: static buffer too small");

      ret = send_sd_notify (ready_str, err);
      /* Do not fail on errors.  */
      अगर (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  mode = libcrun_get_cgroup_mode (err);
  अगर (UNLIKELY (mode < 0))
    वापस mode;

  /* SIGKILL terminates a process frozen with cgroup v2, while the cgroup v1 freezer does not
     deliver it until the cgroup is thawed, so do not wait for it there.  */
  समाप्त (proxy_pid, SIGKILL);
  अस्थायी_FAILURE_RETRY (waitpid (proxy_pid, शून्य, mode == CGROUP_MODE_UNIFIED ? 0 : WNOHANG));

  वापस destroy_cgroup_path (proxy_cgroup, mode, err);
}

पूर्णांक
libcrun_container_restore (libcrun_context_t *context, स्थिर अक्षर *id arg_unused,
                           काष्ठा libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  cleanup_cgroup_status काष्ठा libcrun_cgroup_status *cgroup_status = शून्य;
  cleanup_container libcrun_container_t *container = शून्य;
  cleanup_close पूर्णांक proxy_pid_pipe0 = -1;
  cleanup_close पूर्णांक proxy_pid_pipe1 = -1;
  runtime_spec_schema_config_schema *def;
  libcrun_container_status_t status = {};
  काष्ठा libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  libcrun_checkpoint_restore_t cr;
  cleanup_pid pid_t proxy_pid = -1;
  cleanup_close पूर्णांक seccomp_fd = -1;
  स्थिर अक्षर *seccomp_bpf_तथ्य;
  पूर्णांक proxy_pid_pipe[2];
  पूर्णांक cgroup_manager;
  uid_t मूल_uid = -1;
  gid_t मूल_gid = -1;
  पूर्णांक ret;

  cr_options_from_public (&cr, cr_options);

  container = libcrun_container_load_from_file ("config.json", err);
  अगर (container == शून्य)
    वापस -1;

  container->context = context;
  def = container->container_def;

  ret = check_config_file (def, context, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_status_check_directories (context->state_मूल, context->id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_copy_config_file (context->id, context->state_मूल, container, err);
  अगर (UNLIKELY (ret < 0))
    जाओ fail;

  /* CRIU restores the seccomp filters of the checkpointed processes, but
     exec loads the filter from the seccomp.bpf file in the state directory,
     so it must be generated again for the restored container.  */
  seccomp_bpf_तथ्य = find_annotation (container, "run.oci.seccomp_bpf_data");

  ret = setup_seccomp (container, seccomp_bpf_तथ्य, &seccomp_gen_ctx, &seccomp_fd, err);
  अगर (UNLIKELY (ret < 0))
    जाओ fail;

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_तथ्य, &seccomp_gen_ctx, err);
  अगर (UNLIKELY (ret < 0))
    जाओ fail;
  close_and_reset (&seccomp_fd);

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  अगर (context->systemd_cgroup)
    cgroup_manager = CGROUP_MANAGER_SYSTEMD;
  अन्यथा अगर (context->force_no_cgroup)
    cgroup_manager = CGROUP_MANAGER_DISABLED;

  def = container->container_def;

  /* If we are root (either on the host or in a namespace),
   * then chown the cgroup to root in the container user namespace. */
  get_मूल_in_the_userns (def, container->host_uid, container->host_gid, &मूल_uid, &मूल_gid);

  /* If the root in the container is different than the current root user, attempt to chown
     the std streams before entering the user namespace.  Otherwise we might lose access
     to the user (as it is not mapped in the user namespace) and cannot chown them.  */
  अगर (मूल_uid > 0 || मूल_gid > 0)
    {
      ret = maybe_chown_std_streams (मूल_uid, मूल_gid, err);
      अगर (UNLIKELY (ret < 0))
        जाओ fail;
    }

  /* CRIU runs as a child process, so the restored processes would inherit the session keyring.  */
  ret = setup_container_keyring (container, context, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  {
    cleanup_close पूर्णांक cgroup_dirfd = -1;
    bool needs_proxy_process;
    cleanup_cgroup_preenter काष्ठा libcrun_cgroup_args cg = {
      .resources = def->linux ? def->linux->resources : शून्य,
      .annotations = container->annotations,
      .cgroup_path = def->linux ? def->linux->cgroups_path : "",
      .manager = cgroup_manager,
      .मूल_uid = मूल_uid,
      .मूल_gid = मूल_gid,
      .id = context->id,
      .state_मूल = context->state_मूल,
      .container = container,
    };

    /* The CRIU restore code uses bundle, rootfs and cgroup_path of status.   The cgroup_path is set later.  */
    status.bundle = (अक्षर *) context->bundle;
    status.rootfs = def->मूल->path;

    ret = libcrun_cgroup_preenter (&cg, &cgroup_dirfd, err);
    अगर (UNLIKELY (ret < 0))
      जाओ fail;

    needs_proxy_process = cgroup_dirfd < 0;

    /* If the target cgroup was already created (with cgroupfs), we can restore the container directly there.  */
    अगर (! needs_proxy_process)
      {
        cleanup_free अक्षर *cgroup_path = शून्य;

        ret = get_cgroup_dirfd_path (cgroup_dirfd, &cgroup_path, err);
        अगर (UNLIKELY (ret < 0))
          जाओ fail;

        /* Complete the configuration for the cgroup before the container is
           restored there: CRIU resumes the restored processes before it
           returns, so the device filter and the limits must already be in
           place.  There is no process to move, as CRIU is run in the cgroup,
           and the controllers were enabled by precreate_cgroup.  */
        cg.joined = true;

        ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
        अगर (UNLIKELY (ret < 0))
          जाओ fail;

        /* Restore the container directly in the desired cgroup.  */
        status.cgroup_path = cgroup_path;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        अगर (UNLIKELY (ret < 0))
          जाओ fail;

        /* Now that the container is restored, keep any signal for it, so that
           it is forwarded to the init by wait_for_process, rather than acted
           upon by crun.  This cannot be done earlier, as CRIU, which is run as
           a child process, would inherit the blocked signals.  */
        ret = block_signals (err);
        अगर (UNLIKELY (ret < 0))
          जाओ fail;

        /* Use the container first process PID to finalize the cgroup.  */
        cg.pid = status.pid;
      }
    अन्यथा
      {
        /* When using systemd, we need a process first that must exist before a cgroup is created.
           Create a dummy process that sits and waits until the container is restored.  */
        ret = pipe2 (proxy_pid_pipe, O_CLOEXEC);
        अगर (UNLIKELY (ret < 0))
          {
            ret = crun_make_error (err, त्रुटि_सं, "pipe");
            जाओ fail;
          }

        /* Only used for auto cleanup.  */
        proxy_pid_pipe0 = proxy_pid_pipe[0];
        proxy_pid_pipe1 = proxy_pid_pipe[1];

        proxy_pid = विभाजन ();
        अगर (UNLIKELY (proxy_pid < 0))
          {
            ret = crun_make_error (err, त्रुटि_सं, "fork");
            जाओ fail;
          }

        अगर (proxy_pid == 0)
          {
            close_and_reset (&proxy_pid_pipe1);

            restore_proxy_process (proxy_pid_pipe0);
            _safe_exit (निकास_सफल);
          }

        close_and_reset (&proxy_pid_pipe0);

        /* Use the dummy process PID as the PID to setup the cgroup.  */
        cg.pid = proxy_pid;

        /* Complete the configuration for the cgroup.  It contains only the dummy process.  */
        ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
        अगर (UNLIKELY (ret < 0))
          जाओ fail;
      }

    ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
    अगर (UNLIKELY (ret < 0))
      जाओ fail;

    /* When using a dummy process, the container is restored here once the cgroup is configured.  */
    अगर (needs_proxy_process)
      {
        cleanup_free अक्षर *target_cgroup = शून्य;
        cleanup_free अक्षर *proxy_cgroup = शून्य;

        ret = libcrun_get_cgroup_process (proxy_pid, &target_cgroup, false, err);
        अगर (UNLIKELY (ret < 0))
          जाओ fail;

        /* Move the dummy process to a sub-cgroup.  */
        ret = append_paths (&proxy_cgroup, err, target_cgroup, ".proxy-crun", शून्य);
        अगर (UNLIKELY (ret < 0))
          जाओ fail;

        ret = libcrun_move_process_to_cgroup (proxy_pid, 0, proxy_cgroup, true, err);
        अगर (UNLIKELY (ret < 0))
          जाओ fail;

        /* Restore the container in the same cgroup where the dummy process was.  */
        status.cgroup_path = target_cgroup;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        अगर (UNLIKELY (ret < 0))
          जाओ fail;

        /* See the comment for the other libcrun_container_restore_linux call.  */
        ret = block_signals (err);
        अगर (UNLIKELY (ret < 0))
          जाओ fail;

        /* The container is running, so do not fail because of the dummy process.  */
        ret = restore_terminate_proxy_process (proxy_pid, proxy_cgroup, status.pid, cgroup_manager, err);
        अगर (UNLIKELY (ret < 0))
          crun_error_release (err);
        proxy_pid = -1;

        close_and_reset (&proxy_pid_pipe1);
      }
  }

  context->detach = cr_options->detach;
  ret = write_container_status (container, context, status.pid, cgroup_status, err);
  अगर (UNLIKELY (ret < 0))
    जाओ fail;

  {
    /* Wait the same way run and exec do: apart from waiting for the container
       init, this writes the pid file, forwards any signal to the init, and
       reaps every child, not only the init.  */
    काष्ठा wait_for_process_args args = {
      .pid = status.pid,
      .context = context,
      .terminal_fd = -1,
      .notify_socket = -1,
      .container_ready_fd = शून्य,
      .seccomp_notify_fd = -1,
      .seccomp_notify_plugins = शून्य,
    };

    वापस wait_for_process (&args, err);
  }

fail:
  /* Once restored, the container is running.  Without a status file nothing
     can reach it anymore, so do not leave it behind.  */
  अगर (status.pid > 0)
    {
      समाप्त (status.pid, SIGKILL);
      waitpid_ignore_stopped (status.pid, शून्य, 0);
    }
  अगर (cgroup_status)
    {
      libcrun_error_t tmp_err = शून्य;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  {
    libcrun_error_t tmp_err = शून्य;

    अगर (libcrun_container_delete_status (context->state_मूल, context->id, &tmp_err) < 0)
      crun_error_release (&tmp_err);
  }
  वापस ret;
}

पूर्णांक
libcrun_container_read_pids (libcrun_context_t *context, स्थिर अक्षर *id, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  cleanup_cgroup_status काष्ठा libcrun_cgroup_status *cgroup_status = शून्य;
  cleanup_container_status libcrun_container_status_t status = {};
  पूर्णांक ret;

  ret = libcrun_read_container_status (&status, context->state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (status.cgroup_path == शून्य || status.cgroup_path[0] == '\0')
    वापस crun_make_error (err, 0, "the container is not using cgroups");

  cgroup_status = libcrun_cgroup_make_status (&status);

  वापस libcrun_cgroup_read_pids (cgroup_status, recurse, pids, err);
}

पूर्णांक
libcrun_container_list_json (libcrun_context_t *context, अक्षर **out, libcrun_error_t *err)
{
  libcrun_container_list_t *list = शून्य, *it;
  स्थिर अक्षर *content = शून्य;
  json_gen_ctx *gen = शून्य;
  माप_प्रकार len;
  पूर्णांक ret;

  *out = शून्य;

  ret = libcrun_get_containers_list (&list, context->state_मूल, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (! json_gen_init (&gen, शून्य))
    {
      ret = crun_make_error (err, 0, "cannot allocate json generator");
      जाओ निकास;
    }

  json_gen_config (gen, json_gen_beautify, 1);
  json_gen_array_open (gen);

  क्रम (it = list; it; it = it->अग्र)
    {
      libcrun_container_status_t status;
      पूर्णांक running = 0;
      पूर्णांक pid;
      स्थिर अक्षर *container_status = शून्य;

      ret = libcrun_read_container_status (&status, context->state_मूल, it->name, err);
      अगर (UNLIKELY (ret < 0))
        जाओ निकास;

      pid = status.pid;

      ret = libcrun_get_container_state_string (it->name, &status, context->state_मूल, &container_status,
                                                &running, err);
      अगर (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          जारी;
        }

      अगर (! running)
        pid = 0;

      json_gen_map_open (gen);
      json_gen_string (gen, "id", म_माप ("id"));
      json_gen_string (gen, it->name, म_माप (it->name));
      json_gen_string (gen, "pid", म_माप ("pid"));
      map_int (gen, pid);
      json_gen_string (gen, "status", म_माप ("status"));
      json_gen_string (gen, container_status, म_माप (container_status));
      json_gen_string (gen, "bundle", म_माप ("bundle"));
      json_gen_string (gen, status.bundle, म_माप (status.bundle));
      json_gen_string (gen, "created", म_माप ("created"));
      json_gen_string (gen, status.created, म_माप (status.created));
      json_gen_string (gen, "owner", म_माप ("owner"));
      json_gen_string (gen, status.owner, safe_strlen (status.owner));
      json_gen_map_close (gen);

      libcrun_free_container_status (&status);
    }

  json_gen_array_close (gen);
  अगर (json_gen_get_buf (gen, &content, &len) != json_gen_status_ok)
    {
      ret = libcrun_make_error (err, 0, "cannot generate json list");
      जाओ निकास;
    }

  *out = xstrdup (content);
  ret = 0;

निकास:
  अगर (list)
    libcrun_free_containers_list (list);
  अगर (gen)
    json_gen_free (gen);

  वापस ret;
}

पूर्णांक
libcrun_write_json_containers_list (libcrun_context_t *context, खाता *out, libcrun_error_t *err)
{
  cleanup_free अक्षर *content = शून्य;
  माप_प्रकार len;
  पूर्णांक ret;

  ret = libcrun_container_list_json (context, &content, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  len = म_माप (content);
  अगर (len && ख_डालो (content, 1, len, out) != len)
    वापस crun_make_error (err, त्रुटि_सं, "error writing to file");

  वापस 0;
}

पूर्णांक
libcrun_container_update_intel_rdt (libcrun_context_t *context, स्थिर अक्षर *id, काष्ठा libcrun_intel_rdt_update *update, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = शून्य;
  पूर्णांक ret;

  ret = read_container_config_from_state (&container, context->state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस libcrun_update_intel_rdt (id, container, update->l3_cache_schema, update->mem_bw_schema, update->schemata, err);
}

अटल पूर्णांक
libcrun_container_add_or_remove_mounts_from_file (libcrun_context_t *context, स्थिर अक्षर *id, स्थिर अक्षर *file, bool add, libcrun_error_t *err)
{
  cleanup_custom_handler_instance काष्ठा custom_handler_instance_s *custom_handler = शून्य;
  cleanup_container libcrun_container_t *container = शून्य;
  cleanup_free runtime_spec_schema_defs_mount **mounts = शून्य;
  स्थिर अक्षर *state_मूल = context->state_मूल;
  cleanup_free parser_error parser_err = शून्य;
  काष्ठा parser_context pctx = { 0, मानक_त्रुटि };
  libcrun_container_status_t status = {};
  cleanup_free अक्षर *content = शून्य;
  json_object *doc = शून्य;
  माप_प्रकार n_mounts = 0, len, i;
  पूर्णांक ret = 1;

  ret = read_all_file (file, &content, &len, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_read_container_status (&status, state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = read_container_config_from_state (&container, state_मूल, id, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = parse_json_file (&doc, content, &pctx, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (! json_object_is_type (doc, json_type_array))
    {
      ret = crun_make_error (err, 0, "mounts must be an array");
      जाओ cleanup;
    }
  अन्यथा
    {
      n_mounts = json_object_array_लंबाई (doc);

      mounts = xmalloc0 ((n_mounts + 1) * माप (*mounts));
      क्रम (i = 0; i < n_mounts; i++)
        {
          mounts[i] = make_runtime_spec_schema_defs_mount (json_object_array_get_idx (doc, i), &pctx, &parser_err);
          अगर (mounts[i] == शून्य)
            {
              ret = crun_make_error (err, 0, "cannot parse mount: %s", parser_err);
              जाओ cleanup;
            }
        }
    }

  अगर (add)
    ret = libcrun_make_runtime_mounts (container, &status, mounts, n_mounts, err);
  अन्यथा
    ret = libcrun_destroy_runtime_mounts (container, &status, mounts, n_mounts, err);

cleanup:
  अगर (doc)
    json_object_put (doc);
  अगर (mounts)
    क्रम (i = 0; i < n_mounts; i++)
      free_runtime_spec_schema_defs_mount (mounts[i]);
  वापस ret;
}

पूर्णांक
libcrun_container_add_mounts_from_file (libcrun_context_t *context, स्थिर अक्षर *id, स्थिर अक्षर *file, libcrun_error_t *err)
{
  वापस libcrun_container_add_or_remove_mounts_from_file (context, id, file, true, err);
}

पूर्णांक
libcrun_container_remove_mounts_from_file (libcrun_context_t *context, स्थिर अक्षर *id, स्थिर अक्षर *file, libcrun_error_t *err)
{
  वापस libcrun_container_add_or_remove_mounts_from_file (context, id, file, false, err);
}
