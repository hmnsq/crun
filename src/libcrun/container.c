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
#परिभाषय _GNU_SOURCE

#अन्तर्भाव <config.h>
#अन्तर्भाव <ocispec/runtime_spec_schema_config_schema.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव "container.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "json_gen_utils.h"
#अन्तर्भाव "seccomp.h"
#अन्तर्भाव "mempolicy.h"
#यदि_परिभाषितम् HAVE_SECCOMP
#  include <seccomp.h>
#यदि_अन्तः
#अन्तर्भाव "scheduler.h"
#अन्तर्भाव "seccomp_notify.h"
#अन्तर्भाव "custom-handler.h"
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <argp.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sched.h>
#अन्तर्भाव <sys/wait.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव "status.h"
#अन्तर्भाव "mount_flags.h"
#अन्तर्भाव "linux.h"
#अन्तर्भाव "terminal.h"
#अन्तर्भाव "io_priority.h"
#अन्तर्भाव "cgroup.h"
#अन्तर्भाव "cgroup-utils.h"
#अन्तर्भाव <sys/prctl.h>
#अन्तर्भाव <sys/auxv.h>
#अन्तर्भाव <sys/signalfd.h>
#अन्तर्भाव <sys/epoll.h>
#अन्तर्भाव <sys/socket.h>
#यदि_परिभाषितम् HAVE_CAP
#  include <sys/capability.h>
#यदि_अन्तः
#अन्तर्भाव <sys/ioctl.h>
#अन्तर्भाव <termios.h>
#अन्तर्भाव <grp.h>
#अन्तर्भाव <git-version.h>

#यदि_परिभाषितम् HAVE_SYSTEMD
#  include <systemd/sd-daemon.h>
#यदि_अन्तः

#अन्तर्भाव <json-c/json.h>
#अन्तर्भाव <ocispec/json_common.h>

गणना
{
  SYNC_SOCKET_SYNC_सन्देशः,
  SYNC_SOCKET_ERROR_सन्देशः,
  SYNC_SOCKET_WARNING_सन्देशः,
  SYNC_SOCKET_DEBUG_सन्देशः,
};

संरचना container_entrypoint_s
{
  libcrun_container_t *container;
  libcrun_context_t *context;
  पूर्णाङ्क has_terminal_socket_pair;
  पूर्णाङ्क terminal_socketpair[2];

  /* Used by log_write_to_sync_socket.  */
  पूर्णाङ्क sync_socket;

  पूर्णाङ्क seccomp_fd;
  पूर्णाङ्क seccomp_receiver_fd;
  पूर्णाङ्क console_socket_fd;

  पूर्णाङ्क hooks_बहिर्गमनम्_fd;
  पूर्णाङ्क hooks_err_fd;

  /* Absolute bundle path, to be reported to hooks run in the container.  */
  ध्रुव वर्ण *bundle;

  संरचना custom_handler_instance_s *custom_handler;
};

संरचना sync_socket_सन्देशः_s
{
  पूर्णाङ्क type;
  पूर्णाङ्क error_मानम्;
  वर्ण सन्देशः_2[512];
};

प्रकारनिर्देश runtime_spec_schema_defs_hook hook;

// linux hooks
वर्ण *hooks[] = {
  "prestart",
  "createRuntime",
  "createContainer",
  "startContainer",
  "poststart",
  "poststop"
};

// linux namespaces
स्थिर वर्ण *namespaces[] = {
  "cgroup",
  "ipc",
  "mount",
  "network",
  "pid",
  "user",
  "uts"
};

स्थिर वर्ण *actions[] = {
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

स्थिर वर्ण *operators[] = {
  "SCMP_CMP_NE",
  "SCMP_CMP_LT",
  "SCMP_CMP_LE",
  "SCMP_CMP_EQ",
  "SCMP_CMP_GE",
  "SCMP_CMP_GT",
  "SCMP_CMP_MASKED_EQ",
};

स्थिर वर्ण *archs[] = {
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

स्थिर वर्ण *mempolicy_modes[] = {
  "MPOL_DEFAULT",
  "MPOL_PREFERRED",
  "MPOL_BIND",
  "MPOL_INTERLEAVE",
  "MPOL_LOCAL",
  "MPOL_PREFERRED_MANY",
  "MPOL_WEIGHTED_INTERLEAVE"
};

स्थिर वर्ण *mempolicy_flags[] = {
  "MPOL_F_NUMA_BALANCING",
  "MPOL_F_RELATIVE_NODES",
  "MPOL_F_STATIC_NODES"
};

स्थिर वर्ण *potentially_unsafe_annotations[] = {
  "module.wasm.image/variant",
  "io.kubernetes.cri.container-type",
  "run.oci.",
  "org.criu.",
  "krun.",
};

#परिभाषय SYNC_SOCKET_सन्देशः_LEN(x, l) (offsetof (संरचना sync_socket_सन्देशः_s, सन्देशः_2) + l)

स्थिर पूर्णाङ्क
sync_socket_write_सन्देशः (पूर्णाङ्क fd, पूर्णाङ्क verbosity, पूर्णाङ्क err_मानम्, ध्रुव वर्ण *log_सन्देशः)
{
  पूर्णाङ्क ret;
  size_t err_len;
  संरचना sync_socket_सन्देशः_s सन्देशः;
  विकल्प (verbosity)
    {
    पक्ष LIBCRUN_VERBOSITY_DEBUG:
      सन्देशः.type = SYNC_SOCKET_DEBUG_सन्देशः;
      भङ्ग;
    पक्ष LIBCRUN_VERBOSITY_WARNING:
      सन्देशः.type = SYNC_SOCKET_WARNING_सन्देशः;
      भङ्ग;
    पक्ष LIBCRUN_VERBOSITY_ERROR:
      सन्देशः.type = SYNC_SOCKET_ERROR_सन्देशः;
      भङ्ग;
    }
  सन्देशः.error_मानम् = err_मानम्;

  यदि (fd < 0)
    प्रत्यागम 0;

  err_len = वाक्यदैर्घ्यम् (log_सन्देशः);
  यदि (err_len >= परिमाण (सन्देशः.सन्देशः_2))
    err_len = परिमाण (सन्देशः.सन्देशः_2) - 1;

  स्मृतिं_प्रतिलिख (सन्देशः.सन्देशः_2, log_सन्देशः, err_len);
  सन्देशः.सन्देशः_2[err_len] = '\0';

  ret = अस्थायी_FAILURE_RETRY (लिख (fd, &सन्देशः, SYNC_SOCKET_सन्देशः_LEN (सन्देशः, err_len + 1)));
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम -1;

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
sync_socket_write_error (पूर्णाङ्क fd, libcrun_error_t *बहिर्गमनम्_err)
{
  यदि (fd < 0)
    प्रत्यागम 0;
  प्रत्यागम sync_socket_write_सन्देशः (fd, false, (*बहिर्गमनम्_err)->स्थितिः, (*बहिर्गमनम्_err)->सन्देशः);
}

स्थिर शून्य
log_write_गन्तव्यम्_sync_socket (पूर्णाङ्क errno_, ध्रुव वर्ण *सन्देशः, पूर्णाङ्क verbosity, शून्य *arg)
{
  संरचना container_entrypoint_s *entrypoint_तर्काः = arg;
  पूर्णाङ्क fd = entrypoint_तर्काः->sync_socket;

  यदि (fd < 0)
    प्रत्यागम;

  यदि (sync_socket_write_सन्देशः (fd, verbosity, errno_, सन्देशः) < 0)
    log_write_गन्तव्यम्_stderr (errno_, सन्देशः, verbosity, arg);
}

स्थिर bool
is_स्मृतिः_सीमा_too_नीचः (runtime_spec_schema_config_schema *def)
{
  ध्रुव दीर्घ स्मृतिः_सीमा_too_नीचः = 1024 * 1024;

  यदि (def->linux == रिक्त || def->linux->resources == रिक्त)
    प्रत्यागम false;

  यदि (def->linux->resources->स्मृतिः
      && def->linux->resources->स्मृतिः->सीमा_present
      && def->linux->resources->स्मृतिः->सीमा < स्मृतिः_सीमा_too_नीचः)
    प्रत्यागम true;

  यदि (def->linux->resources->unified)
    {
      size_t i;

      प्रति (i = 0; i < def->linux->resources->unified->len; i++)
        यदि (वाक्यं_तुलय (def->linux->resources->unified->कुञ्चिकाः[i], "memory.max") == 0)
          {
            दीर्घ सीमा;

            errno = 0;
            सीमा = strtol (def->linux->resources->unified->मानानि[i], रिक्त, 10);
            यदि (errno != 0)
              प्रत्यागम false;
            यदि (सीमा < स्मृतिः_सीमा_too_नीचः)
              प्रत्यागम true;
          }
    }

  प्रत्यागम false;
}

स्थिर पूर्णाङ्क
sync_socket_wait_sync (libcrun_context_t *context, पूर्णाङ्क fd, bool flush, libcrun_error_t *err)
{
  संरचना sync_socket_सन्देशः_s सन्देशः;

  यदि (fd < 0)
    प्रत्यागम 0;

  यावत् (true)
    {
      पूर्णाङ्क ret;

      ret = अस्थायी_FAILURE_RETRY (पठ (fd, &सन्देशः, परिमाण (सन्देशः)));
      यदि (UNLIKELY (ret < 0))
        {
          यदि (flush)
            प्रत्यागम 0;
          प्रत्यागम crun_make_error (err, errno, "read from sync socket");
        }

      यदि (ret == 0)
        {
          यदि (flush)
            प्रत्यागम 0;

          प्रत्यागम crun_make_error (err, 0, "read from the init process");
        }

      यदि (! flush && सन्देशः.type == SYNC_SOCKET_SYNC_सन्देशः)
        प्रत्यागम 0;
      अन्यथा यदि (सन्देशः.type == SYNC_SOCKET_DEBUG_सन्देशः)
        {
          यदि (context)
            context->output_handler (सन्देशः.error_मानम्, सन्देशः.सन्देशः_2, LIBCRUN_VERBOSITY_DEBUG, context->output_handler_arg);
          अनुवर्त;
        }
      अन्यथा यदि (सन्देशः.type == SYNC_SOCKET_WARNING_सन्देशः)
        {
          यदि (context)
            context->output_handler (सन्देशः.error_मानम्, सन्देशः.सन्देशः_2, LIBCRUN_VERBOSITY_WARNING, context->output_handler_arg);
          अनुवर्त;
        }
      अन्यथा यदि (सन्देशः.type == SYNC_SOCKET_ERROR_सन्देशः)
        प्रत्यागम crun_make_error (err, सन्देशः.error_मानम्, "%s", सन्देशः.सन्देशः_2);
    }
}

स्थिर पूर्णाङ्क
sync_socket_send_sync (पूर्णाङ्क fd, bool flush_errors, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  संरचना sync_socket_सन्देशः_s सन्देशः = {
    0,
  };
  सन्देशः.type = SYNC_SOCKET_SYNC_सन्देशः;

  यदि (fd < 0)
    प्रत्यागम 0;

  ret = अस्थायी_FAILURE_RETRY (लिख (fd, &सन्देशः, SYNC_SOCKET_सन्देशः_LEN (सन्देशः, 0)));
  यदि (UNLIKELY (ret < 0))
    {
      यदि (flush_errors)
        {
          पूर्णाङ्क saved_errno = errno;
          ret = अस्थायी_FAILURE_RETRY (पठ (fd, &सन्देशः, परिमाण (सन्देशः)));
          यदि (ret >= 0 && सन्देशः.type == SYNC_SOCKET_ERROR_सन्देशः)
            प्रत्यागम crun_make_error (err, सन्देशः.error_मानम्, "%s", सन्देशः.सन्देशः_2);
          errno = saved_errno;
        }
      प्रत्यागम crun_make_error (err, errno, "write to sync socket");
    }

  प्रत्यागम 0;
}

स्थिर bool
has_cap_sys_ptrace ()
{
#यदि_परिभाषितम् HAVE_CAP
  संरचना __user_cap_header_struct hdr = { LINUX_CAPABILITY_संस्करणम्_3, 0 };
  संरचना __user_cap_data_struct दत्त[2] = { { 0 } };

  यदि (UNLIKELY (capget (&hdr, दत्त) < 0))
    प्रत्यागम true;

  प्रत्यागम (दत्त[0].effective & (1U << CAP_SYS_PTRACE)) != 0;
#अन्यथा
  प्रत्यागम geteuid () == 0;
#यदि_अन्तः
}

स्थिर libcrun_container_t *
make_container (runtime_spec_schema_config_schema *container_def, ध्रुव वर्ण *मार्गः, ध्रुव वर्ण *config)
{
  libcrun_container_t *container = xmalloc0 (परिमाण (*container));
  container->container_def = container_def;

  container->host_uid = geteuid ();
  container->host_gid = getegid ();
  container->host_has_cap_sys_ptrace = has_cap_sys_ptrace ();
  container->proc_fd = -1;

  container->annotations = make_string_map_from_json (container_def->annotations);

  /* An empty namespace path means that a new namespace is to be created,
     the same as if it is not specified.  */
  यदि (container_def->linux)
    {
      size_t i;

      प्रति (i = 0; i < container_def->linux->namespaces_len; i++)
        यदि (container_def->linux->namespaces[i]->मार्गः && container_def->linux->namespaces[i]->मार्गः[0] == '\0')
          {
            मुक्त (container_def->linux->namespaces[i]->मार्गः);
            container_def->linux->namespaces[i]->मार्गः = रिक्त;
          }
    }

  यदि (मार्गः)
    container->config_file = xstrdup (मार्गः);
  यदि (config)
    container->config_file_content = xstrdup (config);

  प्रत्यागम container;
}

libcrun_container_t *
libcrun_container_आरोपय_from_स्मृतिः (ध्रुव वर्ण *json, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free वर्ण *oci_error = रिक्त;
  container_def = runtime_spec_schema_config_schema_parse_दत्त (json, रिक्त, &oci_error);
  यदि (container_def == रिक्त)
    {
      crun_make_error (err, 0, "load: `%s`", oci_error);
      प्रत्यागम रिक्त;
    }
  प्रत्यागम make_container (container_def, रिक्त, json);
}

libcrun_container_t *
libcrun_container_आरोपय_from_file (ध्रुव वर्ण *मार्गः, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free वर्ण *oci_error = रिक्त;
  cleanup_free वर्ण *content = रिक्त;
  size_t len;
  पूर्णाङ्क ret;

  libcrun_debug ("Loading container from config file: `%s`", मार्गः);
  ret = read_all_file (मार्गः, &content, &len, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम रिक्त;

  container_def = runtime_spec_schema_config_schema_parse_दत्त (content, रिक्त, &oci_error);
  यदि (container_def == रिक्त)
    {
      crun_make_error (err, 0, "load `%s`: %s", मार्गः, oci_error);
      प्रत्यागम रिक्त;
    }
  प्रत्यागम make_container (container_def, रिक्त, content);
}

ध्रुव वर्ण *
libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err)
{
  संरचना parser_context ctx = { 0, stderr };
  parser_error gen_err = रिक्त;

  यदि (container == रिक्त)
    {
      crun_make_error (err, EINVAL, "no container specified");
      प्रत्यागम रिक्त;
    }

  यदि (container->config_file_content == रिक्त && container->container_def)
    {
      container->config_file_content = runtime_spec_schema_config_schema_generate_json (container->container_def, &ctx, &gen_err);
      यदि (container->config_file_content == रिक्त)
        {
          crun_make_error (err, 0, "cannot generate the configuration JSON: %s", gen_err ?: "unknown error");
          मुक्त (gen_err);
          प्रत्यागम रिक्त;
        }
      मुक्त (gen_err);
    }

  यदि (container->config_file_content == रिक्त)
    {
      crun_make_error (err, 0, "the container has no configuration");
      प्रत्यागम रिक्त;
    }

  प्रत्यागम container->config_file_content;
}

ध्रुव वर्ण *
libcrun_container_get_annotation (libcrun_container_t *container, ध्रुव वर्ण *कुञ्चिका)
{
  यदि (container == रिक्त)
    प्रत्यागम रिक्त;
  प्रत्यागम find_string_map_मानम् (container->annotations, कुञ्चिका);
}

size_t
libcrun_container_get_annotations_len (libcrun_container_t *container)
{
  यदि (container == रिक्त || container->annotations == रिक्त)
    प्रत्यागम 0;
  प्रत्यागम string_map_आकार (container->annotations);
}

पूर्णाङ्क
libcrun_container_get_annotation_at (libcrun_container_t *container, size_t सूचकांक, ध्रुव वर्ण **कुञ्चिका,
                                     ध्रुव वर्ण **मानम्)
{
  यदि (container == रिक्त)
    {
      errno = ERANGE;
      प्रत्यागम -1;
    }
  प्रत्यागम string_map_get_at (container->annotations, सूचकांक, कुञ्चिका, मानम्);
}

uid_t
libcrun_container_get_uid (libcrun_container_t *container)
{
  यदि (container == रिक्त)
    प्रत्यागम (uid_t) -1;
  प्रत्यागम container->host_uid;
}

gid_t
libcrun_container_get_gid (libcrun_container_t *container)
{
  यदि (container == रिक्त)
    प्रत्यागम (gid_t) -1;
  प्रत्यागम container->host_gid;
}

शून्य
libcrun_container_free (libcrun_container_t *ctr)
{
  यदि (ctr == रिक्त)
    प्रत्यागम;

  यदि (ctr->cleanup_वैयक्तिकम्_दत्त)
    ctr->cleanup_वैयक्तिकम्_दत्त (ctr->वैयक्तिकम्_दत्त);

  यदि (ctr->container_def)
    free_runtime_spec_schema_config_schema (ctr->container_def);

  free_string_map (ctr->annotations);

  यदि (ctr->proc_fd >= 0)
    close (ctr->proc_fd);

  मुक्त (ctr->config_file_content);
  मुक्त (ctr->config_file);
  मुक्त (ctr);
}

libcrun_context_t *
libcrun_context_new (ध्रुव वर्ण *id, ध्रुव वर्ण *state_मूलम्, libcrun_error_t *err)
{
  libcrun_context_t *ctx = xmalloc0 (परिमाण (*ctx));

  /* Go through the setters so that the owned_* copies and the borrowed
     pointers stay in sync and are released by libcrun_context_free.  */
  libcrun_context_समुच्चयः_id (ctx, id);
  libcrun_context_समुच्चयः_state_मूलम् (ctx, state_मूलम्);
  libcrun_context_समुच्चयः_bundle (ctx, ".");
  ctx->fifo_exec_wait_fd = -1;

  यदि (libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, id, रिक्त, err) < 0)
    {
      libcrun_context_free (ctx);
      प्रत्यागम रिक्त;
    }

  ctx->handler_manager = libcrun_handler_manager_create (err);
  यदि (ctx->handler_manager == रिक्त)
    {
      libcrun_context_free (ctx);
      प्रत्यागम रिक्त;
    }

#यदि_परिभाषितम् CRUN_LIBDIR
  यदि (access (CRUN_LIBDIR "/handlers", F_OK) == 0)
    {
      यदि (libcrun_handler_manager_आरोपय_directory (ctx->handler_manager, CRUN_LIBDIR "/handlers", err) < 0)
        {
          libcrun_context_free (ctx);
          प्रत्यागम रिक्त;
        }
    }
#यदि_अन्तः

  प्रत्यागम ctx;
}

शून्य
libcrun_context_free (libcrun_context_t *ctx)
{
  यदि (ctx == रिक्त)
    प्रत्यागम;

  मुक्त (ctx->owned_state_मूलम्);
  मुक्त (ctx->owned_id);
  मुक्त (ctx->owned_bundle);
  मुक्त (ctx->owned_console_socket);
  मुक्त (ctx->owned_pid_file);
  मुक्त (ctx->owned_notify_socket);
  मुक्त (ctx->owned_handler);

  यदि (ctx->handler_manager)
    handler_manager_free (ctx->handler_manager);

  मुक्त (ctx);
}

#परिभाषय LIBCRUN_CONTEXT_समुच्चयः_STRING(क्षेत्रम्)                  \
  कुरु                                                       \
    {                                                      \
      मुक्त (ctx->owned_##field);                           \
      ctx->owned_##field = मानम् ? xstrdup (मानम्) : रिक्त; \
      ctx->क्षेत्रम् = ctx->owned_##field;                     \
  } यावत् (0)

शून्य
libcrun_context_समुच्चयः_id (libcrun_context_t *ctx, ध्रुव वर्ण *मानम्)
{
  LIBCRUN_CONTEXT_समुच्चयः_STRING (id);
}

शून्य
libcrun_context_समुच्चयः_state_मूलम् (libcrun_context_t *ctx, ध्रुव वर्ण *मानम्)
{
  LIBCRUN_CONTEXT_समुच्चयः_STRING (state_मूलम्);
}

शून्य
libcrun_context_समुच्चयः_bundle (libcrun_context_t *ctx, ध्रुव वर्ण *मानम्)
{
  LIBCRUN_CONTEXT_समुच्चयः_STRING (bundle);
}

शून्य
libcrun_context_समुच्चयः_console_socket (libcrun_context_t *ctx, ध्रुव वर्ण *मानम्)
{
  LIBCRUN_CONTEXT_समुच्चयः_STRING (console_socket);
}

शून्य
libcrun_context_समुच्चयः_pid_file (libcrun_context_t *ctx, ध्रुव वर्ण *मानम्)
{
  LIBCRUN_CONTEXT_समुच्चयः_STRING (pid_file);
}

शून्य
libcrun_context_समुच्चयः_notify_socket (libcrun_context_t *ctx, ध्रुव वर्ण *मानम्)
{
  LIBCRUN_CONTEXT_समुच्चयः_STRING (notify_socket);
}

शून्य
libcrun_context_समुच्चयः_handler (libcrun_context_t *ctx, ध्रुव वर्ण *मानम्)
{
  LIBCRUN_CONTEXT_समुच्चयः_STRING (handler);
}

#अपरिभाषय LIBCRUN_CONTEXT_समुच्चयः_STRING

ध्रुव वर्ण *
libcrun_context_get_pid_file (libcrun_context_t *ctx)
{
  प्रत्यागम ctx->pid_file;
}

ध्रुव वर्ण *
libcrun_context_get_state_मूलम् (libcrun_context_t *ctx)
{
  यदि (ctx == रिक्त)
    प्रत्यागम रिक्त;
  प्रत्यागम ctx->state_मूलम्;
}

शून्य
libcrun_context_समुच्चयः_preserve_fds (libcrun_context_t *ctx, पूर्णाङ्क preserve_fds)
{
  ctx->preserve_fds = preserve_fds;
}

पूर्णाङ्क
libcrun_context_get_preserve_fds (libcrun_context_t *ctx)
{
  प्रत्यागम ctx->preserve_fds;
}

शून्य
libcrun_context_समुच्चयः_listen_fds (libcrun_context_t *ctx, पूर्णाङ्क listen_fds)
{
  ctx->listen_fds = listen_fds;
}

शून्य
libcrun_context_समुच्चयः_तर्काः (libcrun_context_t *ctx, पूर्णाङ्क argc, वर्ण **argv)
{
  ctx->argc = argc;
  ctx->argv = argv;
}

शून्य
libcrun_context_समुच्चयः_systemd_cgroup (libcrun_context_t *ctx, bool मानम्)
{
  ctx->systemd_cgroup = मानम्;
}

शून्य
libcrun_context_समुच्चयः_detach (libcrun_context_t *ctx, bool मानम्)
{
  ctx->detach = मानम्;
}

शून्य
libcrun_context_समुच्चयः_no_new_keyring (libcrun_context_t *ctx, bool मानम्)
{
  ctx->no_new_keyring = मानम्;
}

शून्य
libcrun_context_समुच्चयः_no_pivot (libcrun_context_t *ctx, bool मानम्)
{
  ctx->no_pivot = मानम्;
}

शून्य
libcrun_context_समुच्चयः_force_no_cgroup (libcrun_context_t *ctx, bool मानम्)
{
  ctx->force_no_cgroup = मानम्;
}

शून्य
libcrun_context_समुच्चयः_output_handler (libcrun_context_t *ctx, crun_output_handler handler, शून्य *arg)
{
  ctx->output_handler = handler;
  ctx->output_handler_arg = arg;
}

पूर्णाङ्क
libcrun_context_init_logging (libcrun_context_t *ctx, ध्रुव वर्ण *लघुगणकः, libcrun_error_t *err)
{
  प्रत्यागम libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, ctx->id, लघुगणकः, err);
}

स्थिर पूर्णाङ्क
खण्डम्_सङ्केताः (libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  sigset_t आवरणम्;
  sigfillset (&आवरणम्);
  ret = sigprocmask (SIG_खण्डम्, &आवरणम्, रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "sigprocmask");
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
unblock_सङ्केताः (libcrun_error_t *err)
{
  पूर्णाङ्क i;
  पूर्णाङ्क ret;
  sigset_t आवरणम्;
  संरचना sigaction act = {};

  sigfillset (&आवरणम्);
  ret = sigprocmask (SIG_UNBLOCK, &आवरणम्, रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "sigprocmask");

  act.sa_handler = SIG_DFL;
  प्रति (i = 0; i < NSIG; i++)
    {
      ret = sigaction (i, &act, रिक्त);
      यदि (ret < 0 && errno != EINVAL)
        प्रत्यागम crun_make_error (err, errno, "sigaction");
    }

  प्रत्यागम 0;
}

/* must be used on the host before pivot_root(2).  */
स्थिर पूर्णाङ्क
initialize_security (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = libcrun_init_caps (container, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (UNLIKELY (proc == रिक्त))
    प्रत्यागम 0;

  यदि (proc->apparmor_profile)
    {
      ret = libcrun_initialize_apparmor (err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  ret = libcrun_initialize_selinux (container, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम 0;
}

/* Emit the "annotations" map.  Returns a json_gen_status; callers can wrap
   the call with GEN_OR_FAIL.  */
स्थिर पूर्णाङ्क
gen_annotations (json_gen_ctx *gen, json_map_string_string *annotations)
{
  size_t i;
  पूर्णाङ्क r;

  GEN_कुञ्चिका (gen, "annotations");
  GEN_OR_FAIL (json_gen_map_open (gen));

  प्रति (i = 0; i < annotations->len; i++)
    {
      /* An annotation whose value is null in the document is parsed into a
         NULL, which cannot be emitted as a JSON string; leave it out.  */
      यदि (annotations->कुञ्चिकाः[i] == रिक्त || annotations->मानानि[i] == रिक्त)
        अनुवर्त;

      GEN_STR (gen, annotations->कुञ्चिकाः[i]);
      GEN_STR (gen, annotations->मानानि[i]);
    }

  प्रत्यागम json_gen_map_close (gen);

gen_error:
  प्रत्यागम r;
}

/* Return the bundle path to report in the container state.  crun always
   chdir()s into the bundle, so fall back to getcwd() when the caller has no
   absolute bundle path, as can happen when libcrun is used as a library.
   *ALLOCATED must be freed by the caller.  */
स्थिर ध्रुव वर्ण *
get_bundle_मार्गः (ध्रुव वर्ण *bundle, वर्ण **allocated)
{
  यदि (bundle != रिक्त && bundle[0] == '/')
    प्रत्यागम bundle;

  *allocated = getcwd (रिक्त, 0);
  यदि (*allocated == रिक्त)
    OOM ();
  प्रत्यागम *allocated;
}

स्थिर पूर्णाङ्क
do_hooks (runtime_spec_schema_config_schema *def, pid_t pid, ध्रुव वर्ण *id, bool keep_going, ध्रुव वर्ण *bundle,
          ध्रुव वर्ण *cwd, ध्रुव वर्ण *स्थितिः, ध्रुव वर्ण *name, hook **hooks, size_t hooks_len, पूर्णाङ्क बहिर्गमनम्_fd,
          पूर्णाङ्क err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  size_t i, stdin_len;
  पूर्णाङ्क r, ret;
  वर्ण *stdin = रिक्त;
  cleanup_free वर्ण *bundle_allocated = रिक्त;
  ध्रुव वर्ण *rootfs = def->मूलम् ? def->मूलम्->मार्गः : "";
  cleanup_json_gen json_gen_ctx *gen = रिक्त;

  bundle = get_bundle_मार्गः (bundle, &bundle_allocated);

  यदि (! json_gen_init (&gen, रिक्त))
    प्रत्यागम crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 0);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_कुञ्चिका (gen, "ociVersion");
  GEN_STR (gen, "1.0");

  GEN_कुञ्चिका (gen, "id");
  GEN_STR (gen, id);

  GEN_कुञ्चिका (gen, "pid");
  GEN_OR_FAIL (map_int (gen, pid));

  GEN_कुञ्चिका (gen, "root");
  GEN_STR (gen, rootfs);

  GEN_कुञ्चिका (gen, "bundle");
  GEN_STR (gen, bundle);

  GEN_कुञ्चिका (gen, "status");
  GEN_STR (gen, स्थितिः);

  यदि (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, (ध्रुव वर्ण **) &stdin, &stdin_len));

  ret = 0;

  bool error_created = false;
  प्रति (i = 0; i < hooks_len; i++)
    {
      वर्ण **env = environ;

      यदि (hooks[i]->env)
        env = hooks[i]->env;

      /* Release the error from the previous iteration, if any.  */
      यदि (error_created)
        {
          crun_error_release (err);
          error_created = false;
        }

      ret = सततम्_process_with_stdin_timeout_envp (hooks[i]->मार्गः, hooks[i]->तर्काः, cwd, hooks[i]->timeout, env,
                                                 stdin, stdin_len, बहिर्गमनम्_fd, err_fd, can_ignore_chdir_errors, err);
      यदि (UNLIKELY (ret < 0))
        error_created = true;

      यदि (UNLIKELY (ret != 0))
        {
          यदि (keep_going)
            libcrun_warning ("error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->मार्गः, ret);
          अन्यथा
            {
              libcrun_error (0, "error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->मार्गः, ret);
              भङ्ग;
            }
        }
    }

  प्रत्यागम ret;

gen_error:
  प्रत्यागम json_gen_error_गन्तव्यम्_crun_error (r, err);
}

स्थिर पूर्णाङ्क
get_json_gen_परिणाम (json_gen_ctx *gen, वर्ण **बहिर्गमनम्, size_t *बहिर्गमनम्_len)
{
  ध्रुव वर्ण *buf = रिक्त;
  size_t buf_len = 0;
  पूर्णाङ्क r;

  r = json_gen_get_buf (gen, &buf, &buf_len);
  यदि (UNLIKELY (r != json_gen_स्थितिः_ok))
    प्रत्यागम r;

  *बहिर्गमनम्_len = buf_len;

  *बहिर्गमनम् = xmalloc (buf_len + 1);
  स्मृतिं_प्रतिलिख (*बहिर्गमनम्, buf, buf_len);
  (*बहिर्गमनम्)[buf_len] = '\0';

  प्रत्यागम json_gen_स्थितिः_ok;
}

स्थिर पूर्णाङ्क
get_seccomp_receiver_fd_payload (libcrun_container_t *container, ध्रुव वर्ण *स्थितिः, pid_t own_pid,
                                 वर्ण **seccomp_fd_payload, size_t *seccomp_fd_payload_len, libcrun_error_t *err)
{
  पूर्णाङ्क r;
  json_gen_ctx *gen = रिक्त;
  runtime_spec_schema_config_schema *def = container->container_def;
  ध्रुव वर्ण *ध्रुव OCI_संस्करणम् = "0.2.0";

  यदि (! json_gen_init (&gen, रिक्त))
    प्रत्यागम crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_कुञ्चिका (gen, "ociVersion");
  GEN_STR (gen, OCI_संस्करणम्);

  GEN_कुञ्चिका (gen, "fds");
  GEN_OR_FAIL (json_gen_array_open (gen));

  GEN_कुञ्चिका (gen, "seccompFd");
  GEN_OR_FAIL (json_gen_array_close (gen));

  GEN_कुञ्चिका (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  यदि (def && def->linux && def->linux->seccomp)
    {
      ध्रुव वर्ण *metadata = def->linux->seccomp->listener_metadata;

      यदि (metadata)
        {
          GEN_कुञ्चिका (gen, "metadata");
          GEN_STR (gen, metadata);
        }
    }

  /* State.  */
  GEN_कुञ्चिका (gen, "state");
  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_कुञ्चिका (gen, "ociVersion");
  GEN_STR (gen, OCI_संस्करणम्);

  यदि (container->context && container->context->id)
    {
      GEN_कुञ्चिका (gen, "id");
      GEN_STR (gen, container->context->id);
    }

  GEN_कुञ्चिका (gen, "status");
  GEN_STR (gen, स्थितिः);

  GEN_कुञ्चिका (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  यदि (container->context && container->context->bundle)
    {
      GEN_कुञ्चिका (gen, "bundle");
      GEN_STR (gen, container->context->bundle);
    }

  यदि (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));
  /* End state.  */

  GEN_OR_FAIL (json_gen_map_close (gen));

  r = get_json_gen_परिणाम (gen, seccomp_fd_payload, seccomp_fd_payload_len);

gen_error:
  json_gen_free (gen);

  प्रत्यागम json_gen_error_गन्तव्यम्_crun_error (r, err);
}

स्थिर पूर्णाङ्क
send_sync_cb (शून्य *दत्त, libcrun_error_t *err)
{
  पूर्णाङ्क sync_socket_fd = *((पूर्णाङ्क *) दत्त);
  पूर्णाङ्क ret;

  /* sync 2.  */
  ret = sync_socket_send_sync (sync_socket_fd, false, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* sync 3.  */
  प्रत्यागम sync_socket_wait_sync (रिक्त, sync_socket_fd, false, err);
}

स्थिर पूर्णाङ्क
maybe_chown_std_streams (uid_t container_uid, gid_t container_gid, libcrun_error_t *err)
{
  पूर्णाङ्क ret, i;

  प्रति (i = 0; i < 3; i++)
    {
      यदि (! isatty (i))
        {
          संरचना stat statbuf;
          ret = fstat (i, &statbuf);
          यदि (UNLIKELY (ret < 0))
            {
              यदि (errno == EBADF)
                अनुवर्त;
              प्रत्यागम crun_make_error (err, errno, "fstat fd `%d`", i);
            }

          /* Skip chown for device files */
          यदि (S_ISCHR (statbuf.st_mode) || S_ISBLK (statbuf.st_mode))
            अनुवर्त;

          ret = fchown (i, container_uid, container_gid);
          यदि (UNLIKELY (ret < 0))
            {
              /* EINVAL means the user is not mapped in the current userns.
                 Ignore EPERM and EROFS as well as there is no reason to fail
                 so early, and let the container payload deal with it.
                 EBADF means fd is closed.
              */
              यदि (errno == EINVAL || errno == EPERM || errno == EROFS || errno == EBADF)
                अनुवर्त;

              प्रत्यागम crun_make_error (err, errno, "fchown std stream `%i`", i);
            }
        }
    }
  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_container_notify_handler (संरचना container_entrypoint_s *तर्काः,
                                  गणना handler_configure_phase phase,
                                  libcrun_container_t *container, ध्रुव वर्ण *rootfs,
                                  libcrun_error_t *err)
{
  संरचना custom_handler_s *h;

  यदि (तर्काः->custom_handler == रिक्त)
    प्रत्यागम 0;

  h = तर्काः->custom_handler->vtable;
  यदि (h == रिक्त || h->configure_container == रिक्त)
    प्रत्यागम 0;

  प्रत्यागम h->configure_container (तर्काः->custom_handler->cookie, phase,
                                 तर्काः->context, container,
                                 rootfs, err);
}

/* Resolve and normalize the container rootfs path.  */
स्थिर पूर्णाङ्क
resolve_rootfs_मार्गः (libcrun_container_t *container, वर्ण **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  यदि (def->मूलम् && def->मूलम्->मार्गः)
    {
      *rootfs = realpath (def->मूलम्->मार्गः, रिक्त);
      यदि (UNLIKELY (*rootfs == रिक्त))
        {
          पूर्णाङ्क saved_errno = errno;

          /* If realpath failed for any reason, try the relative directory.  */
          यदि (def->मूलम्->मार्गः[0] == '/')
            {
              cleanup_free वर्ण *cwd = रिक्त;
              libcrun_error_t tmp_err = रिक्त;
              ssize_t len;
              पूर्णाङ्क proc_fd;

              proc_fd = libcrun_get_cached_proc_fd (container, &tmp_err);
              यदि (LIKELY (proc_fd >= 0))
                {
                  len = safe_readlinkat (proc_fd, "self/cwd", &cwd, 0, &tmp_err);
                  यदि (LIKELY (len > 0))
                    {
                      /* If the rootfs is under the current working directory, just use its relative path.  */
                      यदि (has_dir_उपसर्गः (def->मूलम्->मार्गः, cwd))
                        {
                          ध्रुव वर्ण *it = consume_slashes (def->मूलम्->मार्गः + len);
                          यदि (LIKELY (*it))
                            {
                              पूर्णाङ्क ret = faccessat (AT_FDCWD, it, F_OK, 0);
                              यदि (LIKELY (ret == 0))
                                *rootfs = xstrdup (it);
                            }
                        }
                    }
                }
              crun_error_release (&tmp_err);
            }

          यदि (*rootfs == रिक्त)
            {
              यदि (saved_errno == ENOENT)
                प्रत्यागम crun_make_error (err, ENOENT, "the rootfs path `%s` does not exist", def->मूलम्->मार्गः);

              *rootfs = xstrdup (def->मूलम्->मार्गः);
            }
        }
    }
  प्रत्यागम 0;
}

/* Initialize the environment variables.  */
स्थिर पूर्णाङ्क
सज्जता_environment (runtime_spec_schema_config_schema *def, uid_t container_uid, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  यदि (clearenv ())
    प्रत्यागम crun_make_error (err, errno, "clearenv");

  यदि (def->process)
    {
      size_t i;

      प्रति (i = 0; i < def->process->env_len; i++)
        {
          /* A null element of the array of variables is parsed into a NULL,
             which putenv(3) cannot be handed.  */
          यदि (def->process->env[i] == रिक्त)
            प्रत्यागम crun_make_error (err, EINVAL, "environment variable is not specified");

          यदि (putenv (def->process->env[i]) < 0)
            प्रत्यागम crun_make_error (err, errno, "putenv `%s`", def->process->env[i]);
        }
    }

  यदि (is_रिक्तानि_string (getenv ("HOME")))
    {
      ret = समुच्चयः_home_env (container_uid);
      यदि (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          यदि (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            प्रत्यागम crun_make_error (err, errno, "setenv HOME");
        }
    }

  प्रत्यागम 0;
}

/* Configure terminal and console for interactive containers.  */
स्थिर पूर्णाङ्क
सज्जता_terminal (संरचना container_entrypoint_s *entrypoint_तर्काः, libcrun_container_t *container,
                पूर्णाङ्क console_socket, पूर्णाङ्क console_socketpair, पूर्णाङ्क has_terminal, libcrun_error_t *err)
{
  cleanup_close पूर्णाङ्क terminal_fd = -1;
  पूर्णाङ्क ret;

  यदि (! has_terminal)
    प्रत्यागम 0;

  fflush (stderr);

  यदि (console_socket >= 0 || (entrypoint_तर्काः->has_terminal_socket_pair && console_socketpair >= 0))
    {
      terminal_fd = libcrun_समुच्चयः_terminal (container, err);
      यदि (UNLIKELY (terminal_fd < 0))
        प्रत्यागम terminal_fd;
    }

  यदि (console_socket >= 0)
    {
      ret = send_fd_गन्तव्यम्_socket (console_socket, terminal_fd, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  अन्यथा यदि (entrypoint_तर्काः->has_terminal_socket_pair && console_socketpair >= 0)
    {
      ret = send_fd_गन्तव्यम्_socket (console_socketpair, terminal_fd, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रत्यागम 0;
}

/* Resolve and validate the container's executable path.  */
स्थिर पूर्णाङ्क
सज्जता_executable_मार्गः (संरचना container_entrypoint_s *entrypoint_तर्काः, runtime_spec_schema_config_schema *def,
                       वर्ण **exec_मार्गः, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  यदि (def->process && def->process->तर्काः)
    {
      ret = find_executable (exec_मार्गः, def->process->तर्काः[0], def->process->cwd, err);
      यदि (UNLIKELY (ret < 0))
        {
          यदि (entrypoint_तर्काः->custom_handler == रिक्त && crun_error_get_errno (err) == ENOENT)
            प्रत्यागम ret;

          /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
             once the process switched to the use that runs in the container.  This might be necessary
             when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
             is not honored.  */
          crun_error_release (err);
        }
    }

  प्रत्यागम 0;
}

/* Apply security settings for the container process.  */
स्थिर पूर्णाङ्क
apply_security_settings (संरचना container_entrypoint_s *entrypoint_तर्काः, runtime_spec_schema_config_schema *def,
                         libcrun_container_t *container, pid_t own_pid, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  runtime_spec_schema_config_schema_process_capabilities *capabilities;
  पूर्णाङ्क no_new_privs;

  यदि (def->process && ! def->process->no_new_privileges)
    {
      वर्ण **seccomp_flags = रिक्त;
      size_t seccomp_flags_len = 0;
      cleanup_free वर्ण *seccomp_fd_payload = रिक्त;
      size_t seccomp_fd_payload_len = 0;

      यदि (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      यदि (entrypoint_तर्काः->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_तर्काः->seccomp_fd, entrypoint_तर्काः->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      close_and_reset (&entrypoint_तर्काः->seccomp_fd);
      close_and_reset (&entrypoint_तर्काः->seccomp_receiver_fd);
    }

  capabilities = def->process ? def->process->capabilities : रिक्त;
  no_new_privs = def->process ? def->process->no_new_privileges : 1;
  ret = libcrun_समुच्चयः_caps (capabilities, container->container_uid, container->container_gid, no_new_privs,
                          entrypoint_तर्काः->custom_handler != रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम 0;
}

/* Create the process that joins the existing PID namespace now that the
   host mounts are not visible anymore, and continue the setup from there.  */
स्थिर पूर्णाङ्क
do_delayed_pidns_fork (पूर्णाङ्क sync_socket, pid_t *own_pid, libcrun_error_t *err)
{
  pid_t pid;
  पूर्णाङ्क ret;

  pid = fork ();
  यदि (UNLIKELY (pid < 0))
    प्रत्यागम crun_make_error (err, errno, "fork");

  यदि (pid)
    {
      /* Report the new PID to the parent and exit immediately.  */
      ret = sync_socket_send_sync (sync_socket, false, err);
      यदि (LIKELY (ret == 0))
        {
          ret = अस्थायी_FAILURE_RETRY (लिख (sync_socket, &pid, परिमाण (pid)));
          यदि (LIKELY (ret == परिमाण (pid)))
            _safe_exit (EXIT_SUCCESS);
        }

      kill (pid, SIGKILL);
      _safe_exit (EXIT_FAILURE);
    }

  /* sync receive own pid.  */
  ret = अस्थायी_FAILURE_RETRY (पठ (sync_socket, own_pid, परिमाण (*own_pid)));
  यदि (UNLIKELY (ret != परिमाण (*own_pid)))
    {
      यदि (ret >= 0)
        errno = 0;
      प्रत्यागम crun_make_error (err, errno, "read from sync socket");
    }

  प्रत्यागम 0;
}

संरचना delayed_pidns_fork_s
{
  libcrun_container_t *container;
  पूर्णाङ्क sync_socket;
  pid_t *own_pid;
};

स्थिर पूर्णाङ्क
delayed_pidns_fork_cb (शून्य *दत्त, libcrun_error_t *err)
{
  संरचना delayed_pidns_fork_s *d = दत्त;

  यदि (! libcrun_container_delayed_pidns_fork (d->container))
    प्रत्यागम 0;

  प्रत्यागम do_delayed_pidns_fork (d->sync_socket, d->own_pid, err);
}

स्थिर पूर्णाङ्क
container_init_सज्जता (शून्य *तर्काः, pid_t *own_pid, वर्ण *notify_socket,
                      पूर्णाङ्क sync_socket, वर्ण **exec_मार्गः, libcrun_error_t *err)
{
  संरचना container_entrypoint_s *entrypoint_तर्काः = तर्काः;
  libcrun_container_t *container = entrypoint_तर्काः->container;
  bool chdir_समाप्तम् = false;
  पूर्णाङ्क ret;
  पूर्णाङ्क has_terminal;
  cleanup_close पूर्णाङ्क console_socket = -1;
  cleanup_close पूर्णाङ्क console_socketpair = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free वर्ण *rootfs = रिक्त;
  संरचना delayed_pidns_fork_s delayed_pidns_fork = {
    .container = container,
    .sync_socket = sync_socket,
    .own_pid = own_pid,
  };

  ret = initialize_security (container, def->process, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_configure_जालम् (container, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = resolve_rootfs_मार्गः (container, &rootfs, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* Configure terminal socket pair for container communication.  */
  यदि (entrypoint_तर्काः->terminal_socketpair[0] >= 0)
    {
      close_and_reset (&entrypoint_तर्काः->terminal_socketpair[0]);
      console_socketpair = entrypoint_तर्काः->terminal_socketpair[1];
    }

  /* sync 1.  */
  ret = sync_socket_wait_sync (रिक्त, sync_socket, false, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  has_terminal = container->container_def->process && container->container_def->process->terminal;
  यदि (has_terminal && entrypoint_तर्काः->context->console_socket)
    console_socket = entrypoint_तर्काः->console_socket_fd;

  ret = libcrun_समुच्चयः_sysctl (container, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_container_notify_handler (entrypoint_तर्काः, HANDLER_CONFIGURE_BEFORE_MOUNTS, container, rootfs, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_do_pivot_मूलम् (container, entrypoint_तर्काः->context->no_pivot, &rootfs, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* sync 2 and 3 are sent as part of libcrun_set_mounts.  */
  ret = libcrun_समुच्चयः_mounts (entrypoint_तर्काः, container, rootfs, send_sync_cb, &sync_socket, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* The createContainer hooks MUST run after the mounts are set up but before
     the pivot_root.  When these hooks are present can_use_open_tree_namespace()
     forces the deferred-pivot path, so the actual pivot happens later in
     libcrun_finalize_mounts().  */
  यदि (def->hooks && def->hooks->create_container_len)
    {
      libcrun_error_t tmp_err = रिक्त;
      पूर्णाङ्क अन्तर्_userns = check_running_अन्तर्_user_namespace (&tmp_err);
      यदि (tmp_err)
        crun_error_release (&tmp_err);
      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_तर्काः->bundle, रिक्त, "created", "createContainer",
                      (hook **) def->hooks->create_container,
                      def->hooks->create_container_len, entrypoint_तर्काः->hooks_बहिर्गमनम्_fd, entrypoint_तर्काः->hooks_err_fd,
                      अन्तर्_userns > 0, err);
      यदि (UNLIKELY (ret != 0))
        प्रत्यागम ret;
    }

  ret = libcrun_finalize_mounts (entrypoint_तर्काः, container, rootfs, delayed_pidns_fork_cb, &delayed_pidns_fork, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (def->process)
    {
      ret = libcrun_समुच्चयः_selinux_label (container, def->process, false, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = libcrun_समुच्चयः_apparmor_profile (container, def->process, false, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  ret = mark_or_close_fds_ge_than (container, entrypoint_तर्काः->context->preserve_fds + 3, false, err);
  यदि (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ret = libcrun_reopen_dev_null (err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = maybe_chown_std_streams (container->container_uid, container->container_gid, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = सज्जता_environment (def, container->container_uid, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* Set primary process to 1 explicitly if nothing is configured and LISTEN_FD is not set.  */
  यदि (entrypoint_तर्काः->context->listen_fds > 0 && getenv ("LISTEN_PID") == रिक्त)
    {
      libcrun_warning ("setting LISTEN_PID=1 since no previous configuration was found");
      यदि (UNLIKELY (setenv ("LISTEN_PID", "1", 1) < 0))
        प्रत्यागम crun_make_error (err, errno, "setenv LISTEN_PID");
    }

  /* Attempt to chdir immediately here, before doing the setresuid.  If we fail here, let's
     try again later once the process switched to the user that runs in the container.  */
  यदि (def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      यदि (LIKELY (ret == 0))
        chdir_समाप्तम् = true;
      अन्यथा
        crun_error_release (err);
    }

  ret = setsid ();
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "setsid");

  ret = सज्जता_terminal (entrypoint_तर्काः, container, console_socket, console_socketpair, has_terminal, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  close_and_reset (&console_socket);
  close_and_reset (&console_socketpair);

  ret = सज्जता_executable_मार्गः (entrypoint_तर्काः, def, exec_मार्गः, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_समुच्चयः_hostname (container, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_समुच्चयः_domainname (container, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_समुच्चयः_personality (container->container_def->linux->personality, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  यदि (def->process && def->process->user)
    umask (def->process->user->umask_present ? def->process->user->umask : 0022);

  ret = apply_security_settings (entrypoint_तर्काः, def, container, *own_pid, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (UNLIKELY (def->process && def->process->तर्काः && *exec_मार्गः == रिक्त))
    {
      ret = find_executable (exec_मार्गः, def->process->तर्काः[0], def->process->cwd, err);
      यदि (UNLIKELY (ret < 0))
        {
          यदि (entrypoint_तर्काः->custom_handler == रिक्त || is_रिक्तानि_string (def->process->तर्काः[0]))
            प्रत्यागम ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          *exec_मार्गः = xstrdup (def->process->तर्काः[0]);
        }
    }

  /* The chdir was not already performed, so try again now after switching to the UID/GID in the container.  */
  यदि (! chdir_समाप्तम् && def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  यदि (notify_socket && putenv (notify_socket) < 0)
    प्रत्यागम crun_make_error (err, errno, "putenv `%s`", notify_socket);

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
open_hooks_output (libcrun_container_t *container, पूर्णाङ्क *बहिर्गमनम्_fd, पूर्णाङ्क *err_fd, libcrun_error_t *err)
{
  ध्रुव वर्ण *annotation;

  *err_fd = *बहिर्गमनम्_fd = -1;

  libcrun_debug ("Opening hooks output");
  annotation = find_annotation (container, "run.oci.hooks.stdout");
  यदि (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stdout` annotation");
      *बहिर्गमनम्_fd = अस्थायी_FAILURE_RETRY (उद्घाटय (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      यदि (UNLIKELY (*बहिर्गमनम्_fd < 0))
        प्रत्यागम crun_make_error (err, errno, "open `%s`", annotation);
    }

  annotation = find_annotation (container, "run.oci.hooks.stderr");
  यदि (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stderr` annotation");
      *err_fd = अस्थायी_FAILURE_RETRY (उद्घाटय (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      यदि (UNLIKELY (*err_fd < 0))
        प्रत्यागम crun_make_error (err, errno, "open `%s`", annotation);
    }

  प्रत्यागम 0;
}

/* The environment the process was started with lives on the initial stack, right after the
   argv strings and before the string AT_EXECFN points to.  clearenv(3) only drops the pointers
   to it, so when there is no execv to replace the stack, erase it or the container process can
   still read it, e.g. through /proc/self/environ.  */
स्थिर शून्य
erase_initial_environment (वर्ण **argv, पूर्णाङ्क argc)
{
  वर्ण *start = argv[argc - 1] + वाक्यदैर्घ्यम् (argv[argc - 1]) + 1;
  वर्ण *अन्तः = (वर्ण *) getauxval (AT_EXECFN);

  यदि (अन्तः > start)
    स्मृतिं_पूरय (start, 0, अन्तः - start);
}

स्थिर शून्य
rewrite_argv (वर्ण **argv, पूर्णाङ्क argc, ध्रुव वर्ण *name, वर्ण **तर्काः, size_t तर्काः_len)
{
  cleanup_free वर्ण *decorated_name = रिक्त;
  size_t उपलब्धम्_len = argv[argc - 1] - argv[0] + वाक्यदैर्घ्यम् (argv[argc - 1]) + 1;
  cleanup_free वर्ण *new_argv = रिक्त;
  size_t i, so_far = 0, needed = 0;

  needed = xasprintf (&decorated_name, "[libcrun:%s]", name) + 1;
  प्रति (i = 0; i < तर्काः_len; i++)
    needed += वाक्यदैर्घ्यम् (तर्काः[i]) + 1;

  new_argv = xmalloc0 (needed + 1);
  वाक्यं_प्रतिलिख (new_argv, decorated_name);
  so_far = वाक्यदैर्घ्यम् (decorated_name) + 1;

  यदि (उपलब्धम्_len >= needed)
    {
      प्रति (i = 0; i < तर्काः_len; i++)
        {
          वाक्यं_प्रतिलिख (new_argv + so_far, तर्काः[i]);
          so_far += वाक्यदैर्घ्यम् (तर्काः[i]) + 1;
        }
    }

  यदि (so_far >= उपलब्धम्_len)
    so_far = उपलब्धम्_len - 1;

  स्मृतिं_पूरय (argv[0], 0, उपलब्धम्_len);
  स्मृतिं_प्रतिलिख (argv[0], new_argv, so_far);
}

/* Entrypoint to the container.  */
स्थिर पूर्णाङ्क
container_init (शून्य *तर्काः, वर्ण *notify_socket, पूर्णाङ्क sync_socket, libcrun_error_t *err)
{
  संरचना container_entrypoint_s *entrypoint_तर्काः = तर्काः;
  पूर्णाङ्क ret;
  runtime_spec_schema_config_schema *def = entrypoint_तर्काः->container->container_def;
  cleanup_free वर्ण *exec_मार्गः = रिक्त;
  __attribute__ ((अप्रयुक्तम्)) cleanup_free वर्ण *notify_socket_cleanup = notify_socket;
  pid_t own_pid = 0;

  entrypoint_तर्काः->sync_socket = sync_socket;

  crun_समुच्चयः_output_handler (log_write_गन्तव्यम्_sync_socket, तर्काः);

  /* sync receive own pid.  */
  ret = अस्थायी_FAILURE_RETRY (पठ (sync_socket, &own_pid, परिमाण (own_pid)));
  यदि (UNLIKELY (ret != परिमाण (own_pid)))
    {
      यदि (ret >= 0)
        errno = 0;
      प्रत्यागम crun_make_error (err, errno, "read from sync socket");
    }

  ret = container_init_सज्जता (तर्काः, &own_pid, notify_socket, sync_socket, &exec_मार्गः, err);
  यदि (UNLIKELY (ret < 0))
    {
      /* If it fails to write the error using the sync socket, then fallback
         to stderr.  */
      यदि (sync_socket_write_error (sync_socket, err) == 0)
        {
          crun_error_release (err);
          /* A return value of > 0 means to fail with an error without err being set so to avoid
             printing it twice.  */
          प्रत्यागम 1;
        }
      प्रत्यागम ret;
    }

  entrypoint_तर्काः->sync_socket = -1;

  ret = unblock_सङ्केताः (err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* sync 4.  */
  ret = sync_socket_send_sync (sync_socket, false, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  close_and_reset (&sync_socket);

  यदि (entrypoint_तर्काः->context->fifo_exec_wait_fd >= 0)
    {
      वर्ण बफर[1];
      fd_set read_समुच्चयः;
      cleanup_close पूर्णाङ्क fd = entrypoint_तर्काः->context->fifo_exec_wait_fd;
      entrypoint_तर्काः->context->fifo_exec_wait_fd = -1;

      FD_ZERO (&read_समुच्चयः);
      FD_SET (fd, &read_समुच्चयः);
      कुरु
        {
          ret = select (fd + 1, &read_समुच्चयः, रिक्त, रिक्त, रिक्त);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "select");

          ret = अस्थायी_FAILURE_RETRY (पठ (fd, बफर, परिमाण (बफर)));
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "read from the exec fifo");
      } यावत् (ret == 0);

      close_and_reset (&entrypoint_तर्काः->context->fifo_exec_wait_fd);
    }

  crun_समुच्चयः_output_handler (log_write_गन्तव्यम्_stderr, रिक्त);

  यदि (def->process && def->process->no_new_privileges)
    {
      वर्ण **seccomp_flags = रिक्त;
      size_t seccomp_flags_len = 0;
      cleanup_free वर्ण *seccomp_fd_payload = रिक्त;
      size_t seccomp_fd_payload_len = 0;

      यदि (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      यदि (entrypoint_तर्काः->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (entrypoint_तर्काः->container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_तर्काः->seccomp_fd, entrypoint_तर्काः->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags,
                                   seccomp_flags_len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
      close_and_reset (&entrypoint_तर्काः->seccomp_fd);
      close_and_reset (&entrypoint_तर्काः->seccomp_receiver_fd);
    }

  यदि (UNLIKELY (def->process == रिक्त))
    प्रत्यागम crun_make_error (err, 0, "block `process` not found");

  यदि (UNLIKELY (exec_मार्गः == रिक्त))
    प्रत्यागम crun_make_error (err, 0, "executable path not specified");

  यदि (def->hooks && def->hooks->start_container_len)
    {
      libcrun_container_t *container = entrypoint_तर्काः->container;
      libcrun_error_t tmp_err = रिक्त;
      पूर्णाङ्क अन्तर्_userns = check_running_अन्तर्_user_namespace (&tmp_err);
      यदि (tmp_err)
        crun_error_release (&tmp_err);

      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_तर्काः->bundle, रिक्त, "created", "startContainer",
                      (hook **) def->hooks->start_container,
                      def->hooks->start_container_len, entrypoint_तर्काः->hooks_बहिर्गमनम्_fd, entrypoint_तर्काः->hooks_err_fd,
                      अन्तर्_userns > 0, err);
      यदि (UNLIKELY (ret != 0))
        प्रत्यागम ret;

      /* Seek stdout/stderr to the end.  If the hooks were using the same files,
         the container process overwrites what was previously written.  */
      (शून्य) lseek (1, 0, SEEK_END);
      (शून्य) lseek (2, 0, SEEK_END);
    }

  यदि (entrypoint_तर्काः->custom_handler)
    {
      /* Files marked with O_CLOEXEC are closed at execv time, so make sure they are closed now.
         This is a best effort operation, because the seccomp filter is already in place and it could
         stop some syscalls used by mark_or_close_fds_ge_than.
      */
      यदि (entrypoint_तर्काः->custom_handler->vtable->close_fds)
        {
          ret = entrypoint_तर्काः->custom_handler->vtable->close_fds (entrypoint_तर्काः->custom_handler->cookie,
                                                                    entrypoint_तर्काः->container,
                                                                    entrypoint_तर्काः->context->preserve_fds,
                                                                    err);
        }
      अन्यथा
        {
          ret = mark_or_close_fds_ge_than (entrypoint_तर्काः->container, entrypoint_तर्काः->context->preserve_fds + 3, true, err);
        }
      यदि (UNLIKELY (ret < 0))
        crun_error_release (err);

      prctl (PR_समुच्चयः_NAME, entrypoint_तर्काः->custom_handler->vtable->name);

      यदि (entrypoint_तर्काः->context->argv)
        {
          erase_initial_environment (entrypoint_तर्काः->context->argv, entrypoint_तर्काः->context->argc);
          rewrite_argv (entrypoint_तर्काः->context->argv, entrypoint_तर्काः->context->argc,
                        entrypoint_तर्काः->custom_handler->vtable->name, def->process->तर्काः,
                        def->process->तर्काः_len);

          /* It is a quite destructive operation as we might be referencing data from the old
             argv, so make sure the context is not reused.  */
          entrypoint_तर्काः->context->argv = रिक्त;
          entrypoint_तर्काः->context = रिक्त;
        }

      ret = libcrun_समुच्चयः_selinux_label (entrypoint_तर्काः->container, def->process, true, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = libcrun_समुच्चयः_apparmor_profile (entrypoint_तर्काः->container, def->process, true, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = entrypoint_तर्काः->custom_handler->vtable->सततम्_func (entrypoint_तर्काः->custom_handler->cookie,
                                                               entrypoint_तर्काः->container,
                                                               exec_मार्गः,
                                                               def->process->तर्काः);
      यदि (ret != 0)
        प्रत्यागम crun_make_error (err, 0, "exec container process failed with handler as `%s`", entrypoint_तर्काः->custom_handler->vtable->name);

      प्रत्यागम ret;
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation since the seccomp profile is already in place now and might block
     some of the syscalls needed by mark_or_close_fds_ge_than.  */
  ret = mark_or_close_fds_ge_than (entrypoint_तर्काः->container, entrypoint_तर्काः->context->preserve_fds + 3, true, err);
  यदि (UNLIKELY (ret < 0))
    crun_error_release (err);

  अस्थायी_FAILURE_RETRY (execv (exec_मार्गः, def->process->तर्काः));

  यदि (errno == ENOENT)
    प्रत्यागम crun_make_error (err, errno, "exec container process (missing dynamic library?) `%s`", exec_मार्गः);

  प्रत्यागम crun_make_error (err, errno, "exec container process `%s`", exec_मार्गः);
}

स्थिर पूर्णाङ्क
read_container_config_from_state (libcrun_container_t **container, ध्रुव वर्ण *state_मूलम्, ध्रुव वर्ण *id,
                                  libcrun_error_t *err)
{
  cleanup_free वर्ण *config_file = रिक्त;
  cleanup_free वर्ण *dir = रिक्त;
  पूर्णाङ्क ret;

  *container = रिक्त;

  ret = libcrun_get_state_directory (&dir, state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = append_paths (&config_file, err, dir, "config.json", रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  *container = libcrun_container_आरोपय_from_file (config_file, err);
  यदि (*container == रिक्त)
    प्रत्यागम -1;

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
सततम्_poststop_hooks (arg_अप्रयुक्तम् libcrun_context_t *context, libcrun_container_t *container,
                    runtime_spec_schema_config_schema *def,
                    libcrun_container_स्थितिः_t *स्थितिः, ध्रुव वर्ण *state_मूलम्, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container_cleanup = रिक्त;
  पूर्णाङ्क ret;

  यदि (def == रिक्त)
    {
      यदि (container == रिक्त)
        {
          ret = read_container_config_from_state (&container_cleanup, state_मूलम्, id, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
          container = container_cleanup;
        }

      def = container->container_def;
    }

  यदि (def->hooks && def->hooks->poststop_len)
    {
      cleanup_close पूर्णाङ्क hooks_बहिर्गमनम्_fd = -1;
      cleanup_close पूर्णाङ्क hooks_err_fd = -1;

      यदि (container == रिक्त)
        {
          ret = read_container_config_from_state (&container_cleanup, state_मूलम्, id, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
          container = container_cleanup;
        }

      ret = open_hooks_output (container, &hooks_बहिर्गमनम्_fd, &hooks_err_fd, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = do_hooks (def, 0, id, true, स्थितिः->bundle, स्थितिः->bundle, "stopped", "poststop", (hook **) def->hooks->poststop,
                      def->hooks->poststop_len, hooks_बहिर्गमनम्_fd, hooks_err_fd, false, err);
      यदि (UNLIKELY (ret != 0))
        {
          यदि (ret < 0)
            libcrun_error_report_and_release (err);
          अन्यथा
            libcrun_error (0, "poststop hook failed with exit code: %d", ret);
        }
    }
  प्रत्यागम 0;
}

स्थिर bool
has_new_pid_namespace (runtime_spec_schema_config_schema *def)
{
  size_t i;

  यदि (def->linux == रिक्त || def->linux->namespaces == रिक्त)
    प्रत्यागम false;

  प्रति (i = 0; i < def->linux->namespaces_len; i++)
    {
      यदि (वाक्यं_तुलय (def->linux->namespaces[i]->type, "pid") == 0 && def->linux->namespaces[i]->मार्गः == रिक्त)
        प्रत्यागम true;
    }
  प्रत्यागम false;
}

/* Delete the container state.  CONTAINER is the container the caller already has
   available, or NULL.  When it is not NULL, the poststop hooks are run even if the
   status file was never written, e.g. when the container creation failed.  */
स्थिर पूर्णाङ्क
container_delete_आन्तरिकम् (libcrun_context_t *context, libcrun_container_t *container,
                           runtime_spec_schema_config_schema *def, ध्रुव वर्ण *id, bool force, bool killall,
                           libcrun_error_t *err)
{
  cleanup_cgroup_स्थितिः संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः = रिक्त;
  cleanup_container_स्थितिः libcrun_container_स्थितिः_t स्थितिः = {};
  cleanup_container libcrun_container_t *container_cleanup = रिक्त;
  ध्रुव वर्ण *state_मूलम् = context->state_मूलम्;
  पूर्णाङ्क ret;

  ret = libcrun_read_container_स्थितिः (&स्थितिः, state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    {
      यदि (force && crun_error_get_errno (err) == ENOENT)
        {
          libcrun_error_t tmp_err = रिक्त;

          crun_error_release (err);

          यदि (container)
            {
              libcrun_container_स्थितिः_t partial_स्थितिः = {
                .bundle = (वर्ण *) context->bundle,
              };

              ret = सततम्_poststop_hooks (context, container, def, &partial_स्थितिः, state_मूलम्, id, err);
              यदि (UNLIKELY (ret < 0))
                crun_error_write_warning_and_release (context->output_handler_arg, &err);
            }

          libcrun_container_delete_स्थितिः (state_मूलम्, id, &tmp_err);
          crun_error_release (&tmp_err);
          प्रत्यागम 0;
        }
      crun_error_release (err);
      प्रत्यागम libcrun_container_delete_स्थितिः (state_मूलम्, id, err);
    }

  cgroup_स्थितिः = libcrun_cgroup_make_स्थितिः (&स्थितिः);

  यदि (! force)
    {
      ret = libcrun_is_container_running (&स्थितिः, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
      यदि (ret == 1)
        {
          ret = libcrun_स्थितिः_has_read_exec_fifo (state_मूलम्, id, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
          यदि (ret == 0)
            प्रत्यागम crun_make_error (err, 0, "the container `%s` is not in `created` or `stopped` state", id);

          /* If the container is in "created" state, then do the equivalent of delete --force.  */
          killall = force = true;
        }
    }

  यदि (def == रिक्त)
    {
      यदि (container == रिक्त)
        {
          ret = read_container_config_from_state (&container_cleanup, state_मूलम्, id, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;

          container = container_cleanup;
        }

      def = container->container_def;
    }

  यदि (killall && force)
    {
      /* If the container has a pid namespace, it is enough to kill the first
         process (pid=1 in the namespace).
      */
      यदि (has_new_pid_namespace (def))
        {
          ret = libcrun_kill_linux (&स्थितिः, SIGKILL, err);
          यदि (UNLIKELY (ret < 0))
            {
              यदि (crun_error_get_errno (err) != ESRCH)
                प्रत्यागम ret;

              crun_error_release (err);
            }
        }
      अन्यथा यदि (स्थितिः.cgroup_मार्गः)
        {
          ret = libcrun_cgroup_killall (cgroup_स्थितिः, SIGKILL, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
    }

  यदि (def->linux && def->linux->intel_rdt)
    {
      ret = libcrun_destroy_intelrdt (id, def, err);
      यदि (UNLIKELY (ret < 0))
        libcrun_error_report_and_release (err);
    }

  यदि (स्थितिः.cgroup_मार्गः)
    {
      /* If the cgroup cannot be destroyed, there are still processes in it.
         Keep the container status, it is the only record of the cgroup, so
         that the delete can be retried.  */
      ret = libcrun_cgroup_destroy (cgroup_स्थितिः, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  ret = सततम्_poststop_hooks (context, container, def, &स्थितिः, state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  प्रत्यागम libcrun_container_delete_स्थितिः (state_मूलम्, id, err);
}

पूर्णाङ्क
libcrun_container_delete (libcrun_context_t *context, ध्रुव वर्ण *id, bool force, libcrun_error_t *err)
{
  प्रत्यागम container_delete_आन्तरिकम् (context, रिक्त, रिक्त, id, force, true, err);
}

पूर्णाङ्क
libcrun_container_kill (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *सङ्केत, libcrun_error_t *err)
{
  पूर्णाङ्क sig, ret;
  ध्रुव वर्ण *state_मूलम् = context->state_मूलम्;
  cleanup_container_स्थितिः libcrun_container_स्थितिः_t स्थितिः = {};

  sig = str2sig (सङ्केत);
  यदि (UNLIKELY (sig < 0))
    प्रत्यागम crun_make_error (err, 0, "unknown signal `%s`", सङ्केत);

  ret = libcrun_read_container_स्थितिः (&स्थितिः, state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम libcrun_kill_linux (&स्थितिः, sig, err);
}

पूर्णाङ्क
libcrun_container_killall (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *सङ्केत, libcrun_error_t *err)
{
  पूर्णाङ्क sig, ret;
  ध्रुव वर्ण *state_मूलम् = context->state_मूलम्;
  cleanup_container_स्थितिः libcrun_container_स्थितिः_t स्थितिः = {};
  cleanup_cgroup_स्थितिः संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः = रिक्त;

  sig = str2sig (सङ्केत);
  यदि (UNLIKELY (sig < 0))
    प्रत्यागम crun_make_error (err, 0, "unknown signal `%s`", सङ्केत);

  ret = libcrun_read_container_स्थितिः (&स्थितिः, state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  cgroup_स्थितिः = libcrun_cgroup_make_स्थितिः (&स्थितिः);

  ret = libcrun_cgroup_killall (cgroup_स्थितिः, sig, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
write_container_स्थितिः (libcrun_container_t *container, libcrun_context_t *context,
                        pid_t pid, संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः,
                        libcrun_error_t *err)
{
  cleanup_free वर्ण *bundle_allocated = रिक्त;
  ध्रुव वर्ण *bundle = get_bundle_मार्गः (context->bundle, &bundle_allocated);
  cleanup_free वर्ण *owner = get_user_name (geteuid ());
  वर्ण *external_descriptors = libcrun_get_external_descriptors (container);
  वर्ण *rootfs = container->container_def->मूलम् ? container->container_def->मूलम्->मार्गः : "";
  वर्ण created[35];

  libcrun_container_स्थितिः_t स्थितिः = {
    .pid = pid,
    .rootfs = rootfs,
    .bundle = (वर्ण *) bundle,
    .created = created,
    .owner = owner,
    .systemd_cgroup = context->systemd_cgroup,
    .detached = context->detach,
    .no_new_keyring = context->no_new_keyring,
    .external_descriptors = external_descriptors,
    .cgroup_मार्गः = रिक्त,
    .scope = रिक्त,
  };

  get_current_timestamp (created, परिमाण (created));

  यदि (cgroup_स्थितिः)
    {
      पूर्णाङ्क ret;

      ret = libcrun_cgroup_get_स्थितिः (cgroup_स्थितिः, &स्थितिः, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  यदि (external_descriptors == रिक्त)
    प्रत्यागम crun_make_error (err, 0, "invalid internal state.  No external descriptors found");
  प्रत्यागम libcrun_write_container_स्थितिः (context->state_मूलम्, context->id, &स्थितिः, err);
}

स्थिर पूर्णाङ्क
reap_subprocesses (pid_t *main_process, पूर्णाङ्क *main_process_exit, पूर्णाङ्क *चरमः_process, libcrun_error_t *err)
{
  *चरमः_process = 0;
  यावत् (1)
    {
      पूर्णाङ्क स्थितिः;
      पूर्णाङ्क r = waitpid_ignore_stopped (-1, &स्थितिः, WNOHANG);
      यदि (r < 0)
        {
          यदि (errno == EINTR)
            अनुवर्त;
          यदि (errno == ECHILD)
            {
              *चरमः_process = 1;
              प्रत्यागम 0;
            }
          प्रत्यागम crun_make_error (err, errno, "waitpid");
        }
      यदि (r == 0)
        भङ्ग;
      यदि (r != *main_process)
        अनुवर्त;

      *main_process_exit = get_process_exit_स्थितिः (स्थितिः);

      /* Once reaped, the pid can be reused by an unrelated process.  */
      *main_process = 0;
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
send_sd_notify (ध्रुव वर्ण *ready_str, libcrun_error_t *err)
{
#यदि_परिभाषितम् HAVE_SYSTEMD
  पूर्णाङ्क ret = sd_notify (0, ready_str);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, -ret, "sd_notify");

#  यदि HAVE_SD_NOTIFY_BARRIER
  /* Hard-code a 30 seconds timeout.  Ignore errors.  */
  sd_notify_barrier (0, 30 * 1000000);
#  endif
#अन्यथा
  (शून्य) ready_str;
  (शून्य) err;
#यदि_अन्तः
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
handle_notify_socket (पूर्णाङ्क notify_socketfd, libcrun_error_t *err)
{
#यदि_परिभाषितम् HAVE_SYSTEMD
  पूर्णाङ्क ret;
  वर्ण buf[256];
  ध्रुव वर्ण *ready_str = "READY=1";

  ret = recvfrom (notify_socketfd, buf, परिमाण (buf) - 1, 0, रिक्त, रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "recvfrom notify socket");

  buf[ret] = '\0';
  यदि (वाक्ये_अन्विष (buf, ready_str))
    {
      ret = send_sd_notify (ready_str, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      प्रत्यागम 1;
    }
  प्रत्यागम 0;
#अन्यथा
  (शून्य) notify_socketfd;
  (शून्य) err;
  प्रत्यागम 1;
#यदि_अन्तः
}

संरचना wait_for_process_तर्काः
{
  pid_t pid;
  libcrun_context_t *context;
  पूर्णाङ्क terminal_fd;
  पूर्णाङ्क notify_socket;
  पूर्णाङ्क *container_ready_fd;
  पूर्णाङ्क seccomp_notify_fd;
  ध्रुव वर्ण *seccomp_notify_plugins;
};

स्थिर पूर्णाङ्क
write_pid_file (ध्रुव वर्ण *pid_file, pid_t pid, libcrun_error_t *err)
{
  वर्ण buf[32];
  पूर्णाङ्क buf_len = snprintf (buf, परिमाण (buf), "%d", pid);
  यदि (UNLIKELY (buf_len >= (पूर्णाङ्क) परिमाण (buf)))
    प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");
  प्रत्यागम write_file_at_with_flags (AT_FDCWD, O_CREAT | O_TRUNC, 0700, pid_file, buf, buf_len, err);
}

#परिभाषय MAX_EVENTS 10

स्थिर bool
is_fd_pollable (पूर्णाङ्क fd)
{
  संरचना epoll_event ev = { .events = EPOLLIN };
  cleanup_close पूर्णाङ्क epollfd = epoll_create1 (EPOLL_CLOEXEC);

  यदि (epollfd < 0)
    प्रत्यागम true;

  प्रत्यागम epoll_ctl (epollfd, EPOLL_CTL_सङ्कलय, fd, &ev) == 0 || errno != EPERM;
}

स्थिर पूर्णाङ्क
wait_for_process (संरचना wait_for_process_तर्काः *तर्काः, libcrun_error_t *err)
{
  cleanup_channel_fd_pair संरचना channel_fd_pair *from_terminal = रिक्त;
  cleanup_channel_fd_pair संरचना channel_fd_pair *गन्तव्यम्_terminal = रिक्त;
  पूर्णाङ्क ret, container_exit_संहिता = 0, चरमः_process;
  cleanup_close पूर्णाङ्क terminal_fd_from = -1;
  cleanup_close पूर्णाङ्क terminal_fd_गन्तव्यम् = -1;
  bool stdin_pollable = false, stdout_pollable = false;
  cleanup_close पूर्णाङ्क epollfd = -1;
  cleanup_close पूर्णाङ्क signalfd = -1;
  sigset_t आवरणम्;
  पूर्णाङ्क अन्तर्_fds[MAX_EVENTS];
  पूर्णाङ्क अन्तर्_fds_len = 0;
  पूर्णाङ्क बहिर्गमनम्_fds[MAX_EVENTS];
  पूर्णाङ्क बहिर्गमनम्_fds_len = 0;
  size_t i;

  cleanup_seccomp_notify_context संरचना seccomp_notify_context_s *seccomp_notify_ctx = रिक्त;

  container_exit_संहिता = 0;

  यदि (तर्काः == रिक्त || तर्काः->context == रिक्त)
    प्रत्यागम crun_make_error (err, 0, "internal error: context is empty");

  प्रति (i = 0; i < MAX_EVENTS; i++)
    {
      अन्तर्_fds[i] = -1;
      बहिर्गमनम्_fds[i] = -1;
    }

  यदि (तर्काः->context->pid_file)
    {
      ret = write_pid_file (तर्काः->context->pid_file, तर्काः->pid, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  /* Also exit if there is nothing more to wait for.  */
  यदि (तर्काः->context->detach && तर्काः->notify_socket < 0)
    प्रत्यागम 0;

  यदि (तर्काः->container_ready_fd)
    {
      ret = 0;
      अस्थायी_FAILURE_RETRY (लिख (*तर्काः->container_ready_fd, &ret, परिमाण (ret)));
      close_and_reset (तर्काः->container_ready_fd);
    }

  sigfillset (&आवरणम्);
  ret = sigprocmask (SIG_खण्डम्, &आवरणम्, रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "sigprocmask");

  signalfd = create_signalfd (&आवरणम्, err);
  यदि (UNLIKELY (signalfd < 0))
    प्रत्यागम signalfd;

  ret = reap_subprocesses (&तर्काः->pid, &container_exit_संहिता, &चरमः_process, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (चरमः_process)
    प्रत्यागम container_exit_संहिता;

  यदि (तर्काः->seccomp_notify_fd >= 0)
    {
      cleanup_free वर्ण *state_मूलम् = रिक्त;
      cleanup_free वर्ण *oci_config_मार्गः = रिक्त;

      संरचना libcrun_आरोपय_seccomp_notify_conf_s conf;
      स्मृतिं_पूरय (&conf, 0, परिमाण conf);

      ret = libcrun_get_state_directory (&state_मूलम्, तर्काः->context->state_मूलम्, तर्काः->context->id, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = append_paths (&oci_config_मार्गः, err, state_मूलम्, "config.json", रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      conf.runtime_मूलम्_मार्गः = state_मूलम्;
      conf.name = तर्काः->context->id;
      conf.bundle_मार्गः = तर्काः->context->bundle;
      conf.oci_config_मार्गः = oci_config_मार्गः;

      ret = समुच्चयः_blocking_fd (तर्काः->seccomp_notify_fd, false, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = libcrun_आरोपय_seccomp_notify_plugins (&seccomp_notify_ctx,
                                                 तर्काः->seccomp_notify_plugins,
                                                 &conf, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      अन्तर्_fds[अन्तर्_fds_len++] = तर्काः->seccomp_notify_fd;
    }

  यदि (तर्काः->terminal_fd >= 0)
    {
      /* The terminal_fd is dup()ed so that it can be registered with
         epoll multiple times using different masks.  */
      terminal_fd_from = dup (तर्काः->terminal_fd);
      यदि (UNLIKELY (terminal_fd_from < 0))
        प्रत्यागम crun_make_error (err, errno, "dup terminal fd");
      terminal_fd_गन्तव्यम् = dup (तर्काः->terminal_fd);
      यदि (UNLIKELY (terminal_fd_गन्तव्यम् < 0))
        प्रत्यागम crun_make_error (err, errno, "dup terminal fd");

      पूर्णाङ्क i, non_blocking_fds[] = { terminal_fd_from, terminal_fd_गन्तव्यम्, 0, 1, -1 };
      प्रति (i = 0; non_blocking_fds[i] >= 0; i++)
        {
          ret = समुच्चयः_blocking_fd (non_blocking_fds[i], false, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }

      /* stdin and stdout can be something epoll does not support, such as
         /dev/null or a regular file.  Such a stdin is not forwarded, and
         such a stdout is written to without waiting for it to be ready.  */
      stdin_pollable = is_fd_pollable (0);
      stdout_pollable = is_fd_pollable (1);

      from_terminal = channel_fd_pair_new (terminal_fd_from, 1, BUFSIZ);
      यदि (stdin_pollable)
        गन्तव्यम्_terminal = channel_fd_pair_new (0, terminal_fd_गन्तव्यम्, BUFSIZ);
    }

  अन्तर्_fds[अन्तर्_fds_len++] = signalfd;
  यदि (तर्काः->notify_socket >= 0)
    अन्तर्_fds[अन्तर्_fds_len++] = तर्काः->notify_socket;
  यदि (तर्काः->terminal_fd >= 0)
    {
      यदि (stdin_pollable)
        {
          अन्तर्_fds[अन्तर्_fds_len++] = 0;
          बहिर्गमनम्_fds[बहिर्गमनम्_fds_len++] = terminal_fd_गन्तव्यम्;
        }

      अन्तर्_fds[अन्तर्_fds_len++] = terminal_fd_from;
      यदि (stdout_pollable)
        बहिर्गमनम्_fds[बहिर्गमनम्_fds_len++] = 1;
    }

  epollfd = epoll_helper (अन्तर्_fds, रिक्त, बहिर्गमनम्_fds, रिक्त, err);
  यदि (UNLIKELY (epollfd < 0))
    प्रत्यागम epollfd;

  यावत् (1)
    {
      संरचना epoll_event events[MAX_EVENTS];
      संरचना signalfd_siginfo si;
      संरचना winsize ws;
      पूर्णाङ्क i, nr_events;
      ssize_t res;

      nr_events = अस्थायी_FAILURE_RETRY (epoll_wait (epollfd, events, MAX_EVENTS, -1));
      यदि (UNLIKELY (nr_events < 0))
        प्रत्यागम crun_make_error (err, errno, "epoll_wait");

      प्रति (i = 0; i < nr_events; i++)
        {
          यदि (events[i].दत्त.fd == 0 || events[i].दत्त.fd == terminal_fd_गन्तव्यम्)
            {
              ret = channel_fd_pair_process (गन्तव्यम्_terminal, epollfd, err);
              यदि (UNLIKELY (ret < 0))
                प्रत्यागम crun_error_wrap (err, "copy to terminal fd");
            }
          अन्यथा यदि (events[i].दत्त.fd == 1 || events[i].दत्त.fd == terminal_fd_from)
            {
              ret = channel_fd_pair_process (from_terminal, epollfd, err);
              यदि (UNLIKELY (ret < 0))
                प्रत्यागम crun_error_wrap (err, "copy from terminal fd");
            }
          अन्यथा यदि (events[i].दत्त.fd == तर्काः->seccomp_notify_fd)
            {
              ret = libcrun_seccomp_notify_plugins (seccomp_notify_ctx,
                                                    तर्काः->seccomp_notify_fd, err);
              यदि (UNLIKELY (ret < 0))
                प्रत्यागम ret;
            }
          अन्यथा यदि (events[i].दत्त.fd == तर्काः->notify_socket)
            {
              ret = handle_notify_socket (तर्काः->notify_socket, err);
              यदि (UNLIKELY (ret < 0))
                प्रत्यागम ret;
              यदि (ret && तर्काः->context->detach)
                प्रत्यागम 0;
            }
          अन्यथा यदि (events[i].दत्त.fd == signalfd)
            {
              res = अस्थायी_FAILURE_RETRY (पठ (signalfd, &si, परिमाण (si)));
              यदि (UNLIKELY (res < 0))
                प्रत्यागम crun_make_error (err, errno, "read from signalfd");
              यदि (si.ssi_signo == SIGCHLD)
                {
                  ret = reap_subprocesses (&तर्काः->pid, &container_exit_संहिता,
                                           &चरमः_process, err);
                  यदि (UNLIKELY (ret < 0))
                    प्रत्यागम ret;
                  यदि (चरमः_process)
                    प्रत्यागम container_exit_संहिता;
                }
              अन्यथा यदि (si.ssi_signo == SIGWINCH)
                {
                  /* Ignore the signal if the terminal is not available.  */
                  यदि (तर्काः->terminal_fd > 0)
                    {
                      ret = ioctl (0, TIOCGWINSZ, &ws);
                      यदि (UNLIKELY (ret < 0))
                        प्रत्यागम crun_make_error (err, errno, "ioctl TIOCGWINSZ copy terminal size from stdin");

                      ret = ioctl (तर्काः->terminal_fd, TIOCSWINSZ, &ws);
                      यदि (UNLIKELY (ret < 0))
                        प्रत्यागम crun_make_error (err, errno, "ioctl TIOCSWINSZ copy terminal size to pty");
                    }
                }
              अन्यथा यदि (तर्काः->pid > 0)
                {
                  /* Send any other signal to the child process, if it was not reaped yet.  */
                  ret = kill (तर्काः->pid, si.ssi_signo);
                }
            }
          अन्यथा
            {
              प्रत्यागम crun_make_error (err, 0, "internal error: unknown fd from epoll_wait");
            }
        }
    }

  प्रत्यागम 0;
}

स्थिर शून्य
flush_fd_गन्तव्यम्_err (libcrun_context_t *context, पूर्णाङ्क terminal_fd)
{
  वर्ण buf[256];
  पूर्णाङ्क flags;

  यदि (terminal_fd < 0 || stderr == रिक्त)
    प्रत्यागम;

  flags = fcntl (terminal_fd, F_GETFL, 0);
  यदि (flags == -1)
    प्रत्यागम;
  यदि (fcntl (terminal_fd, F_SETFL, flags | O_NONBLOCK) < 0)
    प्रत्यागम;

  प्रति (;;)
    {
      पूर्णाङ्क ret = अस्थायी_FAILURE_RETRY (पठ (terminal_fd, buf, परिमाण (buf) - 1));
      यदि (ret <= 0)
        भङ्ग;
      buf[ret] = '\0';
      यदि (context->output_handler)
        context->output_handler (0, buf, LIBCRUN_VERBOSITY_ERROR, context->output_handler_arg);
    }
  (शून्य) fcntl (terminal_fd, F_SETFL, flags);
  fflush (stderr);
  fsync (1);
  fsync (2);
}

स्थिर पूर्णाङ्क
cleanup_watch (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
               संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः, pid_t init_pid, पूर्णाङ्क sync_socket,
               पूर्णाङ्क terminal_fd, libcrun_error_t *err)
{
  ध्रुव वर्ण *oom_सन्देशः = रिक्त;
  libcrun_error_t tmp_err = रिक्त;
  पूर्णाङ्क ret;

  यदि (init_pid)
    {
      /* Try to detect whether the cgroup has a OOM.  */
      यदि (cgroup_स्थितिः)
        {
          पूर्णाङ्क has_oom;

          has_oom = libcrun_cgroup_has_oom (cgroup_स्थितिः, &tmp_err);
          यदि (has_oom > 0)
            oom_सन्देशः = "OOM: the memory limit could be too low";
          अन्यथा यदि (has_oom < 0)
            {
              /* If the detection has failed for any reason, e.g. the cgroup was
                 already deleted by the time it was checked, just ignore the
                 failure.  */
              crun_error_release (&tmp_err);
            }
        }
      /* If the OOM wasn't detected, look into the static configuration.  */
      यदि (oom_सन्देशः == रिक्त && is_स्मृतिः_सीमा_too_नीचः (def))
        oom_सन्देशः = "the memory limit could be too low";

      kill (init_pid, SIGKILL);
      waitpid_ignore_stopped (init_pid, रिक्त, 0);
    }

  ret = sync_socket_wait_sync (context, sync_socket, true, &tmp_err);
  यदि (UNLIKELY (ret < 0))
    {
      crun_error_release (err);
      *err = tmp_err;
    }

  यदि (terminal_fd >= 0)
    flush_fd_गन्तव्यम्_err (context, terminal_fd);

  यदि (oom_सन्देशः)
    प्रत्यागम crun_error_wrap (err, "%s", oom_सन्देशः);

  प्रत्यागम -1;
}

/* Find the uid:gid that is mapped to root inside the container user namespace.  */
शून्य
get_मूलम्_अन्तर्_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid, uid_t *uid,
                        gid_t *gid)
{
  *uid = -1;
  *gid = -1;

  /* Not root in the namespace.  */
  यदि (host_uid)
    प्रत्यागम;

  यदि (def->linux && def->linux->uid_mappings)
    {
      size_t i;

      प्रति (i = 0; i < def->linux->uid_mappings_len; i++)
        यदि (def->linux->uid_mappings[i]->container_id == 0)
          {
            *uid = def->linux->uid_mappings[i]->host_id;
            भङ्ग;
          }
    }
  यदि (def->linux && def->linux->gid_mappings)
    {
      size_t i;

      प्रति (i = 0; i < def->linux->gid_mappings_len; i++)
        यदि (def->linux->gid_mappings[i]->container_id == 0)
          {
            *gid = def->linux->gid_mappings[i]->host_id;
            भङ्ग;
          }
    }

  /* If the uid and the gid are not changed, do not attempt any chown.  */
  यदि (*uid == host_uid && *gid == host_gid)
    *uid = *gid = -1;
}

स्थिर पूर्णाङ्क
get_seccomp_receiver_fd (libcrun_container_t *container, पूर्णाङ्क *fd, पूर्णाङ्क *self_receiver_fd, ध्रुव वर्ण **plugins,
                         libcrun_error_t *err)
{
  ध्रुव वर्ण *tmp;
  runtime_spec_schema_config_schema *def = container->container_def;

  *fd = -1;
  *self_receiver_fd = -1;

  tmp = find_annotation (container, "run.oci.seccomp.plugins");
  यदि (tmp)
    {
      पूर्णाङ्क fds[2];
      पूर्णाङ्क ret;

      ret = create_socket_pair (fds, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम crun_error_wrap (err, "create socket pair");

      *fd = fds[0];
      *self_receiver_fd = fds[1];
      *plugins = tmp;
      प्रत्यागम 0;
    }

  यदि (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_मार्गः)
    tmp = def->linux->seccomp->listener_मार्गः;
  अन्यथा
    tmp = find_annotation (container, "run.oci.seccomp.receiver");
  यदि (tmp == रिक्त)
    tmp = getenv ("RUN_OCI_SECCOMP_RECEIVER");
  यदि (tmp)
    {
      यदि (tmp[0] != '/')
        प्रत्यागम crun_make_error (err, 0, "the seccomp receiver `%s` is not an absolute path", tmp);

      *fd = open_unix_domain_client_socket (tmp, 0, err);
      यदि (UNLIKELY (*fd < 0))
        प्रत्यागम crun_error_wrap (err, "open seccomp receiver");
    }

  प्रत्यागम 0;
}

स्थिर bool
has_seccomp_receiver (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;

  यदि (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_मार्गः)
    प्रत्यागम true;

  यदि (find_annotation (container, "run.oci.seccomp.receiver") != रिक्त || getenv ("RUN_OCI_SECCOMP_RECEIVER") != रिक्त)
    प्रत्यागम true;

  प्रत्यागम false;
}

स्थिर पूर्णाङ्क
सज्जता_container_hooks_output (libcrun_container_t *container, runtime_spec_schema_config_schema *def,
                              संरचना container_entrypoint_s *container_तर्काः, पूर्णाङ्क *hooks_बहिर्गमनम्_fd,
                              पूर्णाङ्क *hooks_err_fd, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  यदि (def->hooks
      && (def->hooks->prestart_len || def->hooks->poststart_len || def->hooks->create_runtime_len
          || def->hooks->create_container_len || def->hooks->start_container_len))
    {
      ret = open_hooks_output (container, hooks_बहिर्गमनम्_fd, hooks_err_fd, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
      container_तर्काः->hooks_बहिर्गमनम्_fd = *hooks_बहिर्गमनम्_fd;
      container_तर्काः->hooks_err_fd = *hooks_err_fd;
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
सज्जता_container_keyring (libcrun_container_t *container, libcrun_context_t *context, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णाङ्क ret;

  यदि (! context->no_new_keyring)
    {
      ध्रुव वर्ण *label = रिक्त;

      libcrun_debug ("Creating new keyring");

      यदि (def->process)
        {
          label = def->process->selinux_label;
          यदि (label)
            libcrun_debug ("Using SELinux process label: `%s`", label);
        }

      ret = libcrun_create_keyring (container, container->context->id, label, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
सज्जता_terminal_socket_pair (libcrun_container_t *container, libcrun_context_t *context,
                            संरचना container_entrypoint_s *container_तर्काः, पूर्णाङ्क *socket_pair_0,
                            पूर्णाङ्क *socket_pair_1, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णाङ्क ret;
  पूर्णाङ्क detach = context->detach;

  यदि (def->process && def->process->terminal && ! detach && context->console_socket == रिक्त)
    {
      libcrun_debug ("Creating terminal socket pair");
      container_तर्काः->has_terminal_socket_pair = 1;
      ret = create_socket_pair (container_तर्काः->terminal_socketpair, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम crun_error_wrap (err, "create terminal socket");

      *socket_pair_0 = container_तर्काः->terminal_socketpair[0];
      *socket_pair_1 = container_तर्काः->terminal_socketpair[1];
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
सज्जता_seccomp (libcrun_container_t *container, ध्रुव वर्ण *seccomp_bpf_दत्त,
               संरचना libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, पूर्णाङ्क *seccomp_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णाङ्क ret;

  यदि (find_annotation (container, "run.oci.seccomp.plugins") != रिक्त && has_seccomp_receiver (container))
    {
      प्रत्यागम crun_make_error (err, 0, "seccomp plugins and seccomp receivers cannot be declared at the same time");
    }

  यदि (def->linux && (def->linux->seccomp || seccomp_bpf_दत्त))
    {
      अचिह्नित पूर्णाङ्क seccomp_gen_options = 0;
      ध्रुव वर्ण *annotation;

      libcrun_debug ("Initializing seccomp");
      annotation = find_annotation (container, "run.oci.seccomp_fail_unknown_syscall");
      यदि (annotation && वाक्यं_तुलय (annotation, "0") != 0)
        seccomp_gen_options = LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL;

      यदि (seccomp_bpf_दत्त)
        seccomp_gen_options |= LIBCRUN_SECCOMP_SKIP_CACHE;

      libcrun_seccomp_gen_ctx_init (seccomp_gen_ctx, container, true, seccomp_gen_options);

      ret = libcrun_open_seccomp_bpf (seccomp_gen_ctx, seccomp_fd, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
सज्जता_console_socket (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
                      संरचना container_entrypoint_s *container_तर्काः, पूर्णाङ्क *console_socket_fd,
                      libcrun_error_t *err)
{
  यदि (context->console_socket && def->process && def->process->terminal)
    {
      *console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
      यदि (UNLIKELY (*console_socket_fd < 0))
        प्रत्यागम crun_error_wrap (err, "open console socket");
      container_तर्काः->console_socket_fd = *console_socket_fd;
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
सज्जता_cgroup_manager (libcrun_context_t *context, libcrun_container_t *container,
                      संरचना libcrun_cgroup_तर्काः *cg, पूर्णाङ्क *cgroup_dirfd,
                      संरचना libcrun_dirfd_s *cgroup_dirfd_s, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णाङ्क cgroup_manager;
  uid_t मूलम्_uid = -1;
  gid_t मूलम्_gid = -1;
  पूर्णाङ्क ret;

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  यदि (context->systemd_cgroup)
    {
      libcrun_debug ("Using systemd cgroup manager");
      cgroup_manager = CGROUP_MANAGER_SYSTEMD;
    }
  अन्यथा यदि (context->force_no_cgroup)
    {
      libcrun_debug ("Disabling cgroup manager");
      cgroup_manager = CGROUP_MANAGER_DISABLED;
    }
  अन्यथा
    libcrun_debug ("Using cgroupfs cgroup manager");

  /* If we are root (either on the host or in a namespace), then chown the cgroup to root
     in the container user namespace.  */
  get_मूलम्_अन्तर्_the_userns (def, container->host_uid, container->host_gid, &मूलम्_uid, &मूलम्_gid);
  libcrun_debug ("Using container host UID `%d` and GID `%d`", container->host_uid, container->host_gid);

  स्मृतिं_पूरय (cg, 0, परिमाण (*cg));

  cg->cgroup_मार्गः = def->linux ? def->linux->cgroups_मार्गः : "";
  cg->manager = cgroup_manager;
  cg->id = context->id;
  cg->resources = def->linux ? def->linux->resources : रिक्त;
  cg->annotations = container->annotations;
  cg->मूलम्_uid = मूलम्_uid;
  cg->मूलम्_gid = मूलम्_gid;
  cg->state_मूलम् = context->state_मूलम्;
  cg->container = container;

  ret = libcrun_cgroup_preenter (cg, cgroup_dirfd, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  cgroup_dirfd_s->dirfd = cgroup_dirfd;
  cgroup_dirfd_s->joined = false;

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
समुच्चयः_scheduler (pid_t pid, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = libcrun_समुच्चयः_scheduler (pid, def->process, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_reset_cpu_affinity_आवरणम् (pid, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_समुच्चयः_io_प्राथमिकता (pid, def->process, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
seccomp_generation (पूर्णाङ्क seccomp_fd, ध्रुव वर्ण *seccomp_bpf_दत्त,
                    संरचना libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  यदि (seccomp_fd >= 0)
    {
      यदि (seccomp_bpf_दत्त != रिक्त)
        {
          ret = libcrun_copy_seccomp (seccomp_gen_ctx, seccomp_bpf_दत्त, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
      अन्यथा
        {
          ret = libcrun_generate_seccomp (seccomp_gen_ctx, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
terminal_सज्जता (runtime_spec_schema_config_schema *def, libcrun_context_t *context,
                पूर्णाङ्क socket_pair_0, पूर्णाङ्क *terminal_fd, शून्य **orig_terminal, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  पूर्णाङ्क detach = context->detach;

  यदि (def->process && def->process->terminal && ! detach && context->console_socket == रिक्त)
    {
      libcrun_debug ("Receiving console socket fd");
      *terminal_fd = receive_fd_from_socket (socket_pair_0, err);
      यदि (UNLIKELY (*terminal_fd < 0))
        प्रत्यागम -1;

      ret = libcrun_clear_onlcr (*terminal_fd, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = libcrun_समुच्चयः_raw (0, orig_terminal, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
libcrun_container_सततम्_आन्तरिकम् (libcrun_container_t *container, libcrun_context_t *context,
                                पूर्णाङ्क *container_ready_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णाङ्क ret;
  pid_t pid;
  पूर्णाङ्क detach = context->detach;
  cleanup_cgroup_स्थितिः संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः = रिक्त;
  cleanup_close पूर्णाङ्क terminal_fd = -1;
  cleanup_terminal शून्य *orig_terminal = रिक्त;
  cleanup_close पूर्णाङ्क sync_socket = -1;
  cleanup_close पूर्णाङ्क notify_socket = -1;
  cleanup_close पूर्णाङ्क socket_pair_0 = -1;
  cleanup_close पूर्णाङ्क socket_pair_1 = -1;
  cleanup_close पूर्णाङ्क seccomp_fd = -1;
  cleanup_close पूर्णाङ्क console_socket_fd = -1;
  cleanup_close पूर्णाङ्क hooks_बहिर्गमनम्_fd = -1;
  cleanup_close पूर्णाङ्क hooks_err_fd = -1;
  cleanup_close पूर्णाङ्क own_seccomp_receiver_fd = -1;
  cleanup_close पूर्णाङ्क seccomp_notify_fd = -1;
  ध्रुव वर्ण *seccomp_notify_plugins = रिक्त;
  cleanup_cgroup_preenter संरचना libcrun_cgroup_तर्काः cg = {};
  cleanup_free वर्ण *bundle_allocated = रिक्त;
  संरचना container_entrypoint_s container_तर्काः = {
    .container = container,
    .context = context,
    .bundle = get_bundle_मार्गः (context->bundle, &bundle_allocated),
    .terminal_socketpair = { -1, -1 },
    .console_socket_fd = -1,
    .hooks_बहिर्गमनम्_fd = -1,
    .hooks_err_fd = -1,
    .seccomp_receiver_fd = -1,
    .custom_handler = रिक्त,
  };
  cleanup_close पूर्णाङ्क cgroup_dirfd = -1;
  संरचना libcrun_dirfd_s cgroup_dirfd_s;
  संरचना libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  ध्रुव वर्ण *seccomp_bpf_दत्त = find_annotation (container, "run.oci.seccomp_bpf_data");
  पूर्णाङ्क cgroup_बहुलकम्;

  यदि (! context->force_no_cgroup)
    {
      cgroup_बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
      यदि (UNLIKELY (cgroup_बहुलकम् < 0))
        प्रत्यागम cgroup_बहुलकम्;

      यदि (cgroup_बहुलकम् != CGROUP_बहुलकम्_UNIFIED)
        libcrun_warning ("cgroup v1 is deprecated and will be removed in a future release.  Use cgroup v2");
    }

  ret = सज्जता_container_hooks_output (container, def, &container_तर्काः, &hooks_बहिर्गमनम्_fd, &hooks_err_fd, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  container->context = context;

  यदि (! detach || context->notify_socket)
    {
      libcrun_debug ("Setting child subreaper");
      ret = prctl (PR_समुच्चयः_CHILD_SUBREAPER, 1, 0, 0, 0);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "prctl set child subreaper");
    }

  ret = सज्जता_container_keyring (container, context, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = सज्जता_terminal_socket_pair (container, context, &container_तर्काः, &socket_pair_0, &socket_pair_1, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = खण्डम्_सङ्केताः (err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  umask (0);

  ret = libcrun_समुच्चयः_mempolicy (def, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = सज्जता_seccomp (container, seccomp_bpf_दत्त, &seccomp_gen_ctx, &seccomp_fd, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;
  container_तर्काः.seccomp_fd = seccomp_fd;

  यदि (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &container_तर्काः.seccomp_receiver_fd, &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  ret = सज्जता_console_socket (context, def, &container_तर्काः, &console_socket_fd, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = सज्जता_cgroup_manager (context, container, &cg, &cgroup_dirfd, &cgroup_dirfd_s, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_configure_handler (container_तर्काः.context->handler_manager,
                                   container_तर्काः.context,
                                   container,
                                   &(container_तर्काः.custom_handler),
                                   err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (container_तर्काः.custom_handler && container_तर्काः.custom_handler->vtable->modify_oci_configuration)
    {
      libcrun_debug ("Using custom handler to modify OCI configuration");
      ret = container_तर्काः.custom_handler->vtable->modify_oci_configuration (container_तर्काः.custom_handler->cookie,
                                                                             container_तर्काः.context,
                                                                             container->container_def,
                                                                             err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  pid = libcrun_सततम्_linux_container (container, container_init, &container_तर्काः, &sync_socket, &cgroup_dirfd_s, err);
  यदि (UNLIKELY (pid < 0))
    प्रत्यागम pid;

  cg.pid = pid;
  cg.joined = cgroup_dirfd_s.joined;
  libcrun_debug ("Running container on PID: `%d`", pid);

  यदि (context->fifo_exec_wait_fd < 0 && context->notify_socket)
    {
      libcrun_debug ("Using notify socket: `%s`", context->notify_socket);
      /* Do not open the notify socket here on "create".  "start" will take care of it.  */
      ret = get_notify_fd (context, container, &notify_socket, err);
      यदि (UNLIKELY (ret < 0))
        गच्छ fail;
    }

  यदि (container_तर्काः.terminal_socketpair[1] >= 0)
    close_and_reset (&socket_pair_1);

  ret = libcrun_cgroup_enter (&cg, &cgroup_स्थितिः, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ fail;

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ fail;

  ret = libcrun_move_जालम्_devices (container, pid, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ fail;

  /* sync send own pid.  */
  ret = अस्थायी_FAILURE_RETRY (लिख (sync_socket, &pid, परिमाण (pid)));
  यदि (UNLIKELY (ret != परिमाण (pid)))
    {
      यदि (ret >= 0)
        errno = 0;
      crun_make_error (err, errno, "write to sync socket");
      गच्छ fail;
    }

  /* sync 1.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ fail;

  /* sync 2.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ fail;

  ret = libcrun_cgroup_enter_finalize (&cg, cgroup_स्थितिः, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ fail;

  /* If the container process is created later, the scheduler is set on it then.  */
  यदि (! libcrun_container_delayed_pidns_fork (container))
    {
      ret = समुच्चयः_scheduler (pid, def, err);
      यदि (UNLIKELY (ret < 0))
        गच्छ fail;
    }

  /* The container is waiting that we write back.  In this phase we can launch the
     prestart hooks.  */
  यदि (def->hooks && def->hooks->prestart_len)
    {
      libcrun_debug ("Running `prestart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "prestart", (hook **) def->hooks->prestart,
                      def->hooks->prestart_len, hooks_बहिर्गमनम्_fd, hooks_err_fd, false, err);
      यदि (UNLIKELY (ret != 0))
        गच्छ fail;
    }
  यदि (def->hooks && def->hooks->create_runtime_len)
    {
      libcrun_debug ("Running `create` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "createRuntime", (hook **) def->hooks->create_runtime,
                      def->hooks->create_runtime_len, hooks_बहिर्गमनम्_fd, hooks_err_fd, false, err);
      यदि (UNLIKELY (ret != 0))
        गच्छ fail;
    }

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_दत्त, &seccomp_gen_ctx, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ fail;
  close_and_reset (&seccomp_fd);

  /* sync 3.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ fail;

  /* The process joining an existing PID namespace is created once the container root is in place.  */
  यदि (libcrun_container_delayed_pidns_fork (container))
    {
      pid_t new_pid = 0;

      ret = sync_socket_wait_sync (context, sync_socket, false, err);
      यदि (UNLIKELY (ret < 0))
        गच्छ fail;

      ret = अस्थायी_FAILURE_RETRY (पठ (sync_socket, &new_pid, परिमाण (new_pid)));
      यदि (UNLIKELY (ret != परिमाण (new_pid)))
        {
          यदि (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "read pid from sync socket");
          गच्छ fail;
        }

      /* Cleanup the first process.  */
      waitpid_ignore_stopped (pid, रिक्त, 0);

      pid = new_pid;

      ret = समुच्चयः_scheduler (pid, def, err);
      यदि (UNLIKELY (ret < 0))
        गच्छ fail;

      ret = libcrun_handle_pidfd_receiver (pid, container, err);
      यदि (UNLIKELY (ret < 0))
        गच्छ fail;

      /* sync send own pid.  */
      ret = अस्थायी_FAILURE_RETRY (लिख (sync_socket, &pid, परिमाण (pid)));
      यदि (UNLIKELY (ret != परिमाण (pid)))
        {
          यदि (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "write to sync socket");
          गच्छ fail;
        }
    }

  ret = terminal_सज्जता (def, context, socket_pair_0, &terminal_fd, &orig_terminal, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ fail;

  close_and_reset (&socket_pair_0);

  /* sync 4.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ fail;

  ret = close_and_reset (&sync_socket);
  यदि (UNLIKELY (ret < 0))
    {
      crun_make_error (err, errno, "close sync_socket failed");
      गच्छ fail;
    }

  libcrun_debug ("Writing container status");
  ret = write_container_स्थितिः (container, context, pid, cgroup_स्थितिः, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ fail;

  /* Run poststart hooks here only if the container is created using "run".  For create+start, the
     hooks will be executed as part of the start command.  */
  यदि (context->fifo_exec_wait_fd < 0 && def->hooks && def->hooks->poststart_len)
    {
      libcrun_debug ("Running `poststart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_बहिर्गमनम्_fd, hooks_err_fd, false, err);
      यदि (UNLIKELY (ret != 0))
        गच्छ fail;
    }

  /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
  यदि (own_seccomp_receiver_fd >= 0)
    {
      libcrun_debug ("Receiving seccomp fd");
      seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
      यदि (UNLIKELY (seccomp_notify_fd < 0))
        गच्छ fail;

      ret = close_and_reset (&own_seccomp_receiver_fd);
      यदि (UNLIKELY (ret < 0))
        {
          crun_make_error (err, errno, "close seccomp receiver fd failed");
          गच्छ fail;
        }
    }

  {
    संरचना wait_for_process_तर्काः तर्काः = {
      .pid = pid,
      .context = context,
      .terminal_fd = terminal_fd,
      .notify_socket = notify_socket,
      .container_ready_fd = container_ready_fd,
      .seccomp_notify_fd = seccomp_notify_fd,
      .seccomp_notify_plugins = seccomp_notify_plugins,
    };
    ret = wait_for_process (&तर्काः, err);
  }
  यदि (! context->detach)
    {
      libcrun_error_t tmp_err = रिक्त;
      cleanup_watch (context, def, cgroup_स्थितिः, 0, sync_socket, terminal_fd, &tmp_err);
      crun_error_release (&tmp_err);
    }

  प्रत्यागम ret;

fail:
  ret = cleanup_watch (context, def, cgroup_स्थितिः, pid, sync_socket, terminal_fd, err);
  यदि (cgroup_स्थितिः)
    {
      libcrun_error_t tmp_err = रिक्त;
      libcrun_cgroup_destroy (cgroup_स्थितिः, &tmp_err);
      crun_error_release (&tmp_err);
    }
  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
check_config_file (runtime_spec_schema_config_schema *def, libcrun_context_t *context, libcrun_error_t *err)
{
  यदि (UNLIKELY (def->linux == रिक्त))
    प्रत्यागम crun_make_error (err, 0, "invalid config file, no `linux` block specified");

  यदि (context->handler == रिक्त)
    {
      यदि (UNLIKELY (def->मूलम् == रिक्त))
        प्रत्यागम crun_make_error (err, 0, "invalid config file, no `root` block specified");
      यदि (UNLIKELY (def->mounts == रिक्त))
        प्रत्यागम crun_make_error (err, 0, "invalid config file, no `mounts` block specified");
    }
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
libcrun_copy_config_file (ध्रुव वर्ण *id, ध्रुव वर्ण *state_मूलम्, libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  cleanup_free वर्ण *dest_मार्गः = रिक्त;
  cleanup_free वर्ण *dir = रिक्त;
  cleanup_free वर्ण *बफर = रिक्त;
  size_t len;

  ret = libcrun_get_state_directory (&dir, state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = append_paths (&dest_मार्गः, err, dir, "config.json", रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (container->config_file == रिक्त && container->config_file_content == रिक्त)
    प्रत्यागम crun_make_error (err, 0, "config file not specified");

  यदि (container->config_file == रिक्त)
    {
      libcrun_debug ("Writing config file to: `%s`", dest_मार्गः);
      ret = write_file (dest_मार्गः, container->config_file_content, वाक्यदैर्घ्यम् (container->config_file_content), err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  अन्यथा
    {
      libcrun_debug ("Reading config file: `%s`", container->config_file);
      ret = read_all_file (container->config_file, &बफर, &len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      libcrun_debug ("Writing config file to: `%s`", dest_मार्गः);
      ret = write_file (dest_मार्गः, बफर, len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रत्यागम 0;
}

स्थिर शून्य
force_delete_container_स्थितिः (libcrun_context_t *context, libcrun_container_t *container,
                               runtime_spec_schema_config_schema *def)
{
  libcrun_error_t tmp_err = रिक्त;
  container_delete_आन्तरिकम् (context, container, def, context->id, true, false, &tmp_err);
  libcrun_error_report_and_release (&tmp_err);
}

/* Serialize ERR to FD as an int errno followed by a NUL-terminated message.
   Used by forked children to report a failure to the parent process.  */
स्थिर शून्य
write_error_गन्तव्यम्_pipe (पूर्णाङ्क fd, libcrun_error_t *err)
{
  ध्रुव वर्ण *सन्देशः = (*err)->सन्देशः;
  पूर्णाङ्क errcode = crun_error_get_errno (err);

  अस्थायी_FAILURE_RETRY (लिख (fd, &errcode, परिमाण (errcode)));
  अस्थायी_FAILURE_RETRY (लिख (fd, सन्देशः, वाक्यदैर्घ्यम् (सन्देशः) + 1));
}

पूर्णाङ्क
libcrun_container_सततम् (libcrun_context_t *context, libcrun_container_t *container, अचिह्नित पूर्णाङ्क options,
                       libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णाङ्क ret;
  पूर्णाङ्क detach = context->detach;
  पूर्णाङ्क container_ret_स्थितिः[2];
  cleanup_close पूर्णाङ्क pipefd0 = -1;
  cleanup_close पूर्णाङ्क pipefd1 = -1;
  libcrun_error_t tmp_err = रिक्त;

  container->context = context;

  ret = validate_options (options, LIBCRUN_सततम्_OPTIONS_PREFORK | LIBCRUN_सततम्_OPTIONS_KEEP, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = check_config_file (def, context, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (def->process && def->process->terminal && detach && context->console_socket == रिक्त)
    प्रत्यागम crun_make_error (err, 0, "use --console-socket with --detach when a terminal is used");

  ret = libcrun_स्थितिः_check_directories (context->state_मूलम्, context->id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (! detach && (options & LIBCRUN_सततम्_OPTIONS_PREFORK) == 0)
    {
      ret = libcrun_copy_config_file (context->id, context->state_मूलम्, container, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = libcrun_container_सततम्_आन्तरिकम् (container, context, रिक्त, err);
      यदि (! (options & LIBCRUN_सततम्_OPTIONS_KEEP))
        force_delete_container_स्थितिः (context, container, def);
      प्रत्यागम ret;
    }

  ret = pipe2 (container_ret_स्थितिः, O_CLOEXEC);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_स्थितिः[0];
  pipefd1 = container_ret_स्थितिः[1];

  ret = fork ();
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "fork");
  यदि (ret)
    {
      पूर्णाङ्क स्थितिः;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, रिक्त, 0);

      ret = अस्थायी_FAILURE_RETRY (पठ (pipefd0, &स्थितिः, परिमाण (स्थितिः)));
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, errno, "read from sync pipe");

      यदि (स्थितिः < 0)
        {
          पूर्णाङ्क errno_;
          वर्ण buf[512];
          ret = अस्थायी_FAILURE_RETRY (पठ (pipefd0, &errno_, परिमाण (errno_)));
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "read from sync pipe");

          ret = अस्थायी_FAILURE_RETRY (पठ (pipefd0, buf, परिमाण (buf) - 1));
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "read from sync pipe");
          buf[ret] = '\0';

          प्रत्यागम crun_make_error (err, errno_, "%s", buf);
        }

      प्रत्यागम स्थितिः;
    }

  close_and_reset (&pipefd0);

  /* forked process.  */
  ret = detach_process ();
  यदि (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_मूलम्, container, &tmp_err);
  यदि (UNLIKELY (ret < 0))
    गच्छ fail;

  ret = libcrun_container_सततम्_आन्तरिकम् (container, context, रिक्त, &tmp_err);
  अस्थायी_FAILURE_RETRY (लिख (pipefd1, &ret, परिमाण (ret)));
  यदि (UNLIKELY (ret < 0))
    गच्छ fail;

  निर्गच्छ (EXIT_SUCCESS);
fail:

  यदि (! (options & LIBCRUN_सततम्_OPTIONS_KEEP))
    force_delete_container_स्थितिः (context, container, def);
  यदि (tmp_err)
    {
      write_error_गन्तव्यम्_pipe (pipefd1, &tmp_err);
      crun_error_release (&tmp_err);
    }

  निर्गच्छ (EXIT_FAILURE);
}

पूर्णाङ्क
libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container, अचिह्नित पूर्णाङ्क options,
                          libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  पूर्णाङ्क ret;
  पूर्णाङ्क container_ready_pipe[2];
  cleanup_close पूर्णाङ्क pipefd0 = -1;
  cleanup_close पूर्णाङ्क pipefd1 = -1;
  cleanup_close पूर्णाङ्क exec_fifo_fd = -1;
  context->detach = 1;

  libcrun_debug ("Creating container: `%s`", context->id);
  container->context = context;

  ret = validate_options (options, LIBCRUN_CREATE_OPTIONS_PREFORK, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = check_config_file (def, context, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (def->process && def->process->terminal && context->console_socket == रिक्त)
    प्रत्यागम crun_make_error (err, 0, "use --console-socket with create when a terminal is used");

  ret = libcrun_स्थितिः_check_directories (context->state_मूलम्, context->id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  exec_fifo_fd = libcrun_स्थितिः_create_exec_fifo (context->state_मूलम्, context->id, err);
  यदि (UNLIKELY (exec_fifo_fd < 0))
    प्रत्यागम exec_fifo_fd;

  context->fifo_exec_wait_fd = exec_fifo_fd;
  exec_fifo_fd = -1;

  यदि ((options & LIBCRUN_सततम्_OPTIONS_PREFORK) == 0)
    {
      libcrun_debug ("Running with prefork enabled");
      ret = libcrun_copy_config_file (context->id, context->state_मूलम्, container, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
      ret = libcrun_container_सततम्_आन्तरिकम् (container, context, रिक्त, err);
      यदि (UNLIKELY (ret < 0))
        force_delete_container_स्थितिः (context, container, def);
      प्रत्यागम ret;
    }

  ret = pipe2 (container_ready_pipe, O_CLOEXEC);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "pipe");
  pipefd0 = container_ready_pipe[0];
  pipefd1 = container_ready_pipe[1];

  ret = fork ();
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "fork");
  यदि (ret)
    {
      पूर्णाङ्क exit_संहिता;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, रिक्त, 0);

      ret = अस्थायी_FAILURE_RETRY (पठ (pipefd0, &exit_संहिता, परिमाण (exit_संहिता)));
      यदि (UNLIKELY (ret <= 0))
        प्रत्यागम crun_make_error (err, ret < 0 ? errno : 0, "waiting for container to be ready");

      यदि (ret != परिमाण (exit_संहिता) || exit_संहिता < 0)
        प्रत्यागम crun_make_error (err, 0, "internal error: read invalid exit code");

      यदि (exit_संहिता == 0)
        प्रत्यागम 0;

      libcrun_debug ("Exit code is `%d`, deleting container", exit_संहिता);
      libcrun_error_t tmp_err = रिक्त;
      container_delete_आन्तरिकम् (context, रिक्त, def, context->id, true, true, &tmp_err);
      libcrun_error_report_and_release (&tmp_err);
      प्रत्यागम crun_make_error (err, 0, "error creating container");
    }

  /* forked process.  */
  ret = detach_process ();
  यदि (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_मूलम्, container, err);
  यदि (UNLIKELY (ret < 0))
    {
      पूर्णाङ्क errcode = crun_error_get_errno (err);
      crun_error_release (err);
      libcrun_fail_with_error (errcode, "copy config file");
    }

  ret = libcrun_container_सततम्_आन्तरिकम् (container, context, &pipefd1, err);
  यदि (UNLIKELY (ret < 0))
    {
      force_delete_container_स्थितिः (context, container, def);
      libcrun_error ((*err)->स्थितिः, "%s", (*err)->सन्देशः);
      crun_error_release (err);
      crun_समुच्चयः_output_handler (log_write_गन्तव्यम्_stderr, रिक्त);
    }

  यदि (pipefd1 >= 0)
    अस्थायी_FAILURE_RETRY (लिख (pipefd1, &ret, परिमाण (ret)));
  निर्गच्छ (ret ? EXIT_FAILURE : 0);
}

पूर्णाङ्क
libcrun_container_start (libcrun_context_t *context, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = रिक्त;
  ध्रुव वर्ण *state_मूलम् = context->state_मूलम्;
  runtime_spec_schema_config_schema *def;
  cleanup_container_स्थितिः libcrun_container_स्थितिः_t स्थितिः = {};
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क ret;

  ret = libcrun_read_container_स्थितिः (&स्थितिः, state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_is_container_running (&स्थितिः, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (! ret)
    प्रत्यागम crun_make_error (err, 0, "container `%s` is not running", id);

  ret = read_container_config_from_state (&container, state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (context->notify_socket)
    {
      ret = get_notify_fd (context, container, &fd, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  ret = libcrun_स्थितिः_write_exec_fifo (context->state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  def = container->container_def;

  यदि (context->notify_socket)
    {
      यदि (fd >= 0)
        {
          fd_set read_समुच्चयः;

          यावत् (1)
            {
              संरचना timeval timeout = {
                .tv_sec = 0,
                .tv_usec = 10000,
              };
              FD_ZERO (&read_समुच्चयः);
              FD_SET (fd, &read_समुच्चयः);

              ret = select (fd + 1, &read_समुच्चयः, रिक्त, रिक्त, &timeout);
              यदि (UNLIKELY (ret < 0))
                प्रत्यागम crun_make_error (err, errno, "select failed");
              यदि (ret)
                {
                  ret = handle_notify_socket (fd, err);
                  यदि (UNLIKELY (ret < 0))
                    प्रत्यागम ret;
                  यदि (ret)
                    भङ्ग;
                }
              अन्यथा
                {
                  ret = libcrun_is_container_running (&स्थितिः, err);
                  यदि (UNLIKELY (ret < 0))
                    प्रत्यागम ret;
                  यदि (! ret)
                    प्रत्यागम 0;
                }
            }
        }
    }

  /* The container is considered running only after we got the notification from the
     notify_socket, if any.  */
  यदि (def->hooks && def->hooks->poststart_len)
    {
      cleanup_close पूर्णाङ्क hooks_बहिर्गमनम्_fd = -1;
      cleanup_close पूर्णाङ्क hooks_err_fd = -1;

      ret = open_hooks_output (container, &hooks_बहिर्गमनम्_fd, &hooks_err_fd, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = do_hooks (def, स्थितिः.pid, context->id, false, स्थितिः.bundle, स्थितिः.bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_बहिर्गमनम्_fd, hooks_err_fd, false, err);
      यदि (UNLIKELY (ret != 0))
        {
          libcrun_error_t tmp_err = रिक्त;
          container_delete_आन्तरिकम् (context, container, def, id, true, true, &tmp_err);
          libcrun_error_report_and_release (&tmp_err);
          प्रत्यागम ret;
        }
    }

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_get_container_state_string (ध्रुव वर्ण *id, libcrun_container_स्थितिः_t *स्थितिः, ध्रुव वर्ण *state_मूलम्,
                                    ध्रुव वर्ण **container_स्थितिः, पूर्णाङ्क *running, libcrun_error_t *err)
{
  पूर्णाङ्क ret, has_fifo = 0;
  bool paused = false;

  ret = libcrun_is_container_running (स्थितिः, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;
  *running = ret;

  यदि (*running)
    {
      ret = libcrun_स्थितिः_has_read_exec_fifo (state_मूलम्, id, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
      has_fifo = ret;
    }

  यदि (*running && ! has_fifo)
    {
      cleanup_cgroup_स्थितिः संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः = रिक्त;

      cgroup_स्थितिः = libcrun_cgroup_make_स्थितिः (स्थितिः);

      ret = libcrun_cgroup_is_container_paused (cgroup_स्थितिः, &paused, err);
      यदि (UNLIKELY (ret < 0))
        {
          /*
            The cgroup might have been cleaned up by the time we try to read it, ignore both
            ENOENT and ENODEV:
            - ENOENT: if the open(CGROUP_PATH) fails because the cgroup was deleted.
            - ENODEV: if the cgroup is deleted between the open and reading the freeze status.
          */

          errno = crun_error_get_errno (err);
          यदि (errno == ENOENT || errno == ENODEV)
            {
              crun_error_release (err);
              *container_स्थितिः = "stopped";
              प्रत्यागम 0;
            }

          प्रत्यागम ret;
        }
    }

  यदि (! *running)
    *container_स्थितिः = "stopped";
  अन्यथा यदि (has_fifo)
    *container_स्थितिः = "created";
  अन्यथा यदि (paused)
    *container_स्थितिः = "paused";
  अन्यथा
    *container_स्थितिः = "running";

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_container_state_json (libcrun_context_t *context, ध्रुव वर्ण *id, वर्ण **बहिर्गमनम्, libcrun_error_t *err)
{
  वर्ण *बफर = रिक्त;
  size_t len = 0;
  FILE *f;
  पूर्णाङ्क ret;

  f = open_memstream (&बफर, &len);
  यदि (f == रिक्त)
    प्रत्यागम crun_make_error (err, errno, "open_memstream");

  ret = libcrun_container_state (context, id, f, err);
  fclose (f);
  यदि (UNLIKELY (ret < 0))
    {
      मुक्त (बफर);
      प्रत्यागम ret;
    }

  *बहिर्गमनम् = बफर;
  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_container_state (libcrun_context_t *context, ध्रुव वर्ण *id, FILE *बहिर्गमनम्, libcrun_error_t *err)
{
  ध्रुव वर्ण *ध्रुव OCI_CONFIG_संस्करणम् = "1.0.0";
  libcrun_container_स्थितिः_t स्थितिः = {};
  ध्रुव वर्ण *state_मूलम् = context->state_मूलम्;
  ध्रुव वर्ण *container_स्थितिः = रिक्त;
  json_gen_ctx *gen = रिक्त;
  ध्रुव वर्ण *buf;
  पूर्णाङ्क ret = 0;
  पूर्णाङ्क r = json_gen_स्थितिः_ok;
  पूर्णाङ्क running;
  size_t len;

  ret = libcrun_read_container_स्थितिः (&स्थितिः, state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_get_container_state_string (id, &स्थितिः, state_मूलम्, &container_स्थितिः, &running, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ निर्गच्छ;

  ret = 0;
  यदि (! json_gen_init (&gen, रिक्त))
    प्रत्यागम crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));
  GEN_कुञ्चिका (gen, "ociVersion");
  GEN_STR (gen, OCI_CONFIG_संस्करणम्);

  GEN_कुञ्चिका (gen, "id");
  GEN_STR (gen, id);

  GEN_कुञ्चिका (gen, "pid");
  GEN_OR_FAIL (map_int (gen, running ? स्थितिः.pid : 0));

  GEN_कुञ्चिका (gen, "status");
  GEN_STR (gen, container_स्थितिः);

  GEN_कुञ्चिका (gen, "bundle");
  GEN_STR (gen, स्थितिः.bundle);

  GEN_कुञ्चिका (gen, "rootfs");
  GEN_STR (gen, स्थितिः.rootfs);

  GEN_कुञ्चिका (gen, "created");
  GEN_STR (gen, स्थितिः.created);

  यदि (स्थितिः.scope)
    {
      GEN_कुञ्चिका (gen, "systemd-scope");
      GEN_STR (gen, स्थितिः.scope);
    }

  यदि (स्थितिः.owner)
    {
      GEN_कुञ्चिका (gen, "owner");
      GEN_STR (gen, स्थितिः.owner);
    }

  {
    cleanup_container libcrun_container_t *container = रिक्त;

    ret = read_container_config_from_state (&container, state_मूलम्, id, err);
    यदि (UNLIKELY (ret < 0))
      गच्छ निर्गच्छ;

    यदि (container->container_def->annotations && container->container_def->annotations->len)
      GEN_OR_FAIL (gen_annotations (gen, container->container_def->annotations));
  }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  fprintf (बहिर्गमनम्, "%s\n", buf);

निर्गच्छ:
  यदि (gen)
    json_gen_free (gen);
  libcrun_free_container_स्थितिः (&स्थितिः);
  प्रत्यागम ret;

gen_error:
  ret = json_gen_error_गन्तव्यम्_crun_error (r, err);
  गच्छ निर्गच्छ;
}

पूर्णाङ्क
libcrun_container_exec (libcrun_context_t *context, ध्रुव वर्ण *id, runtime_spec_schema_config_schema_process *process,
                        libcrun_error_t *err)
{
  संरचना libcrun_container_exec_options_s opts;
  स्मृतिं_पूरय (&opts, 0, परिमाण (opts));

  opts.struct_आकार = परिमाण (opts);
  opts.process = process;

  प्रत्यागम libcrun_container_exec_with_options (context, id, &opts, err);
}

पूर्णाङ्क
libcrun_container_exec_process_file (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *मार्गः, libcrun_error_t *err)
{
  संरचना libcrun_container_exec_options_s opts;
  स्मृतिं_पूरय (&opts, 0, परिमाण (opts));

  opts.struct_आकार = परिमाण (opts);
  opts.मार्गः = मार्गः;

  प्रत्यागम libcrun_container_exec_with_options (context, id, &opts, err);
}

स्थिर पूर्णाङ्क
exec_from_process_json (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *process_json, ध्रुव वर्ण *cgroup,
                        bool merge_env, libcrun_error_t *err)
{
  संरचना parser_context parser_ctx = { .options = 0, .errfile = stderr };
  संरचना libcrun_container_exec_options_s opts;
  runtime_spec_schema_config_schema_process *process;
  parser_error parser_err = रिक्त;
  json_object *doc = रिक्त;
  पूर्णाङ्क ret;

  ret = parse_json_file (&doc, process_json, &parser_ctx, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  process = make_runtime_spec_schema_config_schema_process (doc, &parser_ctx, &parser_err);
  json_object_put (doc);
  यदि (process == रिक्त)
    {
      ret = crun_make_error (err, 0, "cannot parse process: `%s`", parser_err ? parser_err : "unknown error");
      मुक्त (parser_err);
      प्रत्यागम ret;
    }

  स्मृतिं_पूरय (&opts, 0, परिमाण (opts));
  opts.struct_आकार = परिमाण (opts);
  opts.process = process;
  opts.cgroup = cgroup;
  opts.merge_env = merge_env;

  ret = libcrun_container_exec_with_options (context, id, &opts, err);
  free_runtime_spec_schema_config_schema_process (process);
  प्रत्यागम ret;
}

पूर्णाङ्क
libcrun_container_exec_json (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *process_json, libcrun_error_t *err)
{
  प्रत्यागम exec_from_process_json (context, id, process_json, रिक्त, false, err);
}

पूर्णाङ्क
libcrun_container_exec_with_options_json (libcrun_context_t *context, ध्रुव वर्ण *id,
                                          ध्रुव संरचना libcrun_exec_options_s *opts, libcrun_error_t *err)
{
  यदि (opts == रिक्त || opts->struct_आकार == 0)
    प्रत्यागम crun_make_error (err, EINVAL, "invalid exec options");

  प्रत्यागम exec_from_process_json (context, id, opts->process_json, opts->cgroup, opts->merge_env, err);
}

#परिभाषय cleanup_process_schema __attribute__ ((cleanup (cleanup_process_schemap)))

स्थिर अन्तःस्थ शून्य
cleanup_process_schemap (runtime_spec_schema_config_schema_process **p)
{
  runtime_spec_schema_config_schema_process *process = *p;
  यदि (process)
    (शून्य) free_runtime_spec_schema_config_schema_process (process);
}

/* Build the seccomp receiver payload (when needed), apply the seccomp profile and
   close the seccomp file descriptors.  Used from exec_process_entrypoint both before
   and after the capabilities are set, depending on process->no_new_privileges.  */
स्थिर पूर्णाङ्क
apply_seccomp_for_exec (libcrun_container_t *container, pid_t own_pid, पूर्णाङ्क seccomp_fd,
                        पूर्णाङ्क seccomp_receiver_fd, वर्ण **seccomp_flags, size_t seccomp_flags_len,
                        libcrun_error_t *err)
{
  cleanup_free वर्ण *seccomp_fd_payload = रिक्त;
  size_t seccomp_fd_payload_len = 0;
  पूर्णाङ्क ret;

  यदि (seccomp_receiver_fd >= 0)
    {
      ret = get_seccomp_receiver_fd_payload (container, "running", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  ret = libcrun_apply_seccomp (seccomp_fd, seccomp_receiver_fd, seccomp_fd_payload,
                               seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  close_and_reset (&seccomp_fd);
  close_and_reset (&seccomp_receiver_fd);
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
exec_process_entrypoint (libcrun_context_t *context,
                         libcrun_container_t *container,
                         runtime_spec_schema_config_schema_process *process,
                         पूर्णाङ्क *pipefd1,
                         पूर्णाङ्क seccomp_fd,
                         पूर्णाङ्क seccomp_receiver_fd,
                         संरचना custom_handler_instance_s *custom_handler,
                         bool merge_env,
                         libcrun_error_t *err)
{
  runtime_spec_schema_config_schema_process_capabilities *capabilities = रिक्त;
  cleanup_free वर्ण *exec_मार्गः = रिक्त;
  uid_t container_uid;
  gid_t container_gid;
  ध्रुव वर्ण *cwd;
  bool chdir_समाप्तम् = false;
  size_t seccomp_flags_len = 0;
  वर्ण **seccomp_flags = रिक्त;
  pid_t own_pid = 0;
  size_t i;
  पूर्णाङ्क ret;

  container_uid = process->user ? process->user->uid : 0;
  container_gid = process->user ? process->user->gid : 0;

  अस्थायी_FAILURE_RETRY (पठ (*pipefd1, &own_pid, परिमाण (own_pid)));

  cwd = process->cwd ? process->cwd : "/";
  यदि (LIKELY (libcrun_safe_chdir (cwd, err) == 0))
    chdir_समाप्तम् = true;
  अन्यथा
    crun_error_release (err);

  ret = unblock_सङ्केताः (err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = clearenv ();
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "clearenv");

  यदि ((merge_env || ! process->env_len) && container->container_def->process && container->container_def->process->env_len)
    {
      वर्ण *e;

      प्रति (i = 0; i < container->container_def->process->env_len; i++)
        {
          e = container->container_def->process->env[i];
          यदि (putenv (e) < 0)
            प्रत्यागम crun_make_error (err, errno, "putenv `%s`", e);
        }
    }
  यदि (process->env_len)
    {
      प्रति (i = 0; i < process->env_len; i++)
        यदि (putenv (process->env[i]) < 0)
          प्रत्यागम crun_make_error (err, errno, "putenv `%s`", process->env[i]);
    }

  यदि (is_रिक्तानि_string (getenv ("HOME")))
    {
      ret = समुच्चयः_home_env (container_uid);
      यदि (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          यदि (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            प्रत्यागम crun_make_error (err, errno, "setenv HOME");
        }
    }

  ret = libcrun_समुच्चयः_selinux_label (container, process, false, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_समुच्चयः_apparmor_profile (container, process, false, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (container->container_def->linux && container->container_def->linux->seccomp)
    {
      seccomp_flags = container->container_def->linux->seccomp->flags;
      seccomp_flags_len = container->container_def->linux->seccomp->flags_len;
    }

  ret = find_executable (&exec_मार्गः, process->तर्काः[0], process->cwd, err);
  यदि (UNLIKELY (ret < 0))
    {
      यदि (crun_error_get_errno (err) == ENOENT)
        प्रत्यागम ret;

      /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
         once the process switched to the use that runs in the container.  This might be necessary
         when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
         is not honored.  */
      crun_error_release (err);
    }

  यदि (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_समुच्चयः_personality (container->container_def->linux->personality, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, false, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_reopen_dev_null (err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (! process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  ret = libcrun_container_setgroups (container, process, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = maybe_chown_std_streams (container_uid, container_gid, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (process->capabilities)
    capabilities = process->capabilities;
  अन्यथा यदि (container->container_def->process)
    capabilities = container->container_def->process->capabilities;

  ret = libcrun_समुच्चयः_caps (capabilities, container_uid, container_gid, process->no_new_privileges,
                          custom_handler != रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (UNLIKELY (exec_मार्गः == रिक्त))
    {
      ret = find_executable (&exec_मार्गः, process->तर्काः[0], process->cwd, err);
      यदि (UNLIKELY (ret < 0))
        {
          यदि (custom_handler == रिक्त || is_रिक्तानि_string (process->तर्काः[0]))
            प्रत्यागम ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          exec_मार्गः = xstrdup (process->तर्काः[0]);
        }
    }

  यदि (! chdir_समाप्तम्)
    {
      ret = libcrun_safe_chdir (cwd, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  यदि (process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  यदि (process->user)
    umask (process->user->umask_present ? process->user->umask : 0022);

  ret = 0;
  अस्थायी_FAILURE_RETRY (लिख (*pipefd1, &ret, परिमाण (ret)));
  अस्थायी_FAILURE_RETRY (close (*pipefd1));
  *pipefd1 = -1;

  यदि (custom_handler)
    {
      यदि (custom_handler->vtable->exec_func == रिक्त)
        प्रत्यागम crun_make_error (err, 0, "the handler does not support exec");

      यदि (context->argv)
        erase_initial_environment (context->argv, context->argc);

      ret = custom_handler->vtable->exec_func (custom_handler->cookie,
                                               container,
                                               exec_मार्गः,
                                               process->तर्काः);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, -ret, "exec container process failed with handler as `%s`", custom_handler->vtable->name);

      _safe_exit (EXIT_FAILURE);
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation, because the seccomp filter is already in place and it could
     stop some syscalls used by mark_or_close_fds_ge_than.
  */
  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, true, err);
  यदि (UNLIKELY (ret < 0))
    crun_error_release (err);

  अस्थायी_FAILURE_RETRY (execv (exec_मार्गः, process->तर्काः));
  libcrun_fail_with_error (errno, "exec `%s`", exec_मार्गः);
}

/* Close the file descriptors inherited from the caller, other than those
   to be passed to the container process.  */
पूर्णाङ्क
libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err)
{
  प्रत्यागम mark_or_close_fds_ge_than (रिक्त, context->preserve_fds + 3, true, err);
}

पूर्णाङ्क
libcrun_आरोपय_process_from_file (ध्रुव वर्ण *मार्गः, runtime_spec_schema_config_schema_process **process,
                                libcrun_error_t *err)
{
  संरचना parser_context ctx = { 0, stderr };
  cleanup_free वर्ण *content = रिक्त;
  parser_error parser_err = रिक्त;
  json_object *doc = रिक्त;
  size_t len;
  पूर्णाङ्क ret;

  ret = read_all_file (मार्गः, &content, &len, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  *process = make_runtime_spec_schema_config_schema_process (doc, &ctx, &parser_err);
  यदि (UNLIKELY (*process == रिक्त))
    {
      ret = crun_make_error (err, 0, "cannot parse process file: `%s`", parser_err);
      मुक्त (parser_err);
      यदि (doc)
        json_object_put (doc);
      प्रत्यागम ret;
    }

  मुक्त (parser_err);
  यदि (doc)
    json_object_put (doc);

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_container_exec_with_options (libcrun_context_t *context, ध्रुव वर्ण *id,
                                     संरचना libcrun_container_exec_options_s *opts,
                                     libcrun_error_t *err)
{
  cleanup_custom_handler_instance संरचना custom_handler_instance_s *custom_handler = रिक्त;
  पूर्णाङ्क container_स्थितिः, ret;
  bool container_paused = false;
  pid_t pid;
  libcrun_container_स्थितिः_t स्थितिः = {};
  ध्रुव वर्ण *state_मूलम् = context->state_मूलम्;
  cleanup_close पूर्णाङ्क terminal_fd = -1;
  cleanup_close पूर्णाङ्क seccomp_fd = -1;
  cleanup_terminal शून्य *orig_terminal = रिक्त;
  cleanup_container libcrun_container_t *container = रिक्त;
  पूर्णाङ्क container_ret_स्थितिः[2];
  cleanup_close पूर्णाङ्क pipefd0 = -1;
  cleanup_close पूर्णाङ्क pipefd1 = -1;
  cleanup_close पूर्णाङ्क seccomp_receiver_fd = -1;
  cleanup_close पूर्णाङ्क own_seccomp_receiver_fd = -1;
  cleanup_close पूर्णाङ्क seccomp_notify_fd = -1;
  ध्रुव वर्ण *seccomp_notify_plugins = रिक्त;
  __attribute__ ((अप्रयुक्तम्)) cleanup_process_schema runtime_spec_schema_config_schema_process *process_cleanup = रिक्त;
  runtime_spec_schema_config_schema_process *process = opts->process;
  संरचना libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  पूर्णाङ्क ret_from_child = 0;

  ret = libcrun_read_container_स्थितिः (&स्थितिः, state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_is_container_running (&स्थितिः, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;
  container_स्थितिः = ret;

  ret = read_container_config_from_state (&container, state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  container->context = context;

  यदि (container_स्थितिः == 0)
    प्रत्यागम crun_make_error (err, 0, "the container `%s` is not running", id);

  {
    cleanup_cgroup_स्थितिः संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः = रिक्त;

    cgroup_स्थितिः = libcrun_cgroup_make_स्थितिः (&स्थितिः);

    ret = libcrun_cgroup_is_container_paused (cgroup_स्थितिः, &container_paused, err);
    यदि (UNLIKELY (ret < 0))
      प्रत्यागम ret;
  }

  यदि (UNLIKELY (container_paused))
    प्रत्यागम crun_make_error (err, 0, "cannot exec in a paused container `%s`", id);

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = खण्डम्_सङ्केताः (err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  libcrun_seccomp_gen_ctx_init (&seccomp_gen_ctx, container, false, 0);

  ret = libcrun_open_seccomp_bpf (&seccomp_gen_ctx, &seccomp_fd, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &seccomp_receiver_fd,
                                     &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  यदि (परिमाण (*opts) != opts->struct_आकार)
    प्रत्यागम crun_make_error (err, EINVAL, "invalid libcrun_container_exec_options_s struct");

  यदि (opts->मार्गः)
    {
      यदि (process)
        प्रत्यागम crun_make_error (err, EINVAL, "cannot specify both exec file and options");

      ret = libcrun_आरोपय_process_from_file (opts->मार्गः, &process, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      process_cleanup = process;
    }

  /* This must be done before we enter a user namespace.  If the process does
     not specify any rlimits, use the ones from the container configuration.  */
  यदि (process->rlimits_len == 0 && container->container_def->process)
    ret = libcrun_समुच्चयः_rlimits (container->container_def->process->rlimits,
                               container->container_def->process->rlimits_len, err);
  अन्यथा
    ret = libcrun_समुच्चयः_rlimits (process->rlimits, process->rlimits_len, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_समुच्चयः_mempolicy (container->container_def, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = pipe2 (container_ret_स्थितिः, O_CLOEXEC);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_स्थितिः[0];
  pipefd1 = container_ret_स्थितिः[1];

  /* If the new process block doesn't specify a SELinux label, AppArmor profile, user,
     I/O priority or scheduler, then use the configuration from the original config file.  */
  यदि (container->container_def->process)
    {
      यदि (process->io_प्राथमिकता == रिक्त && container->container_def->process->io_प्राथमिकता)
        {
          process->io_प्राथमिकता = clone_runtime_spec_schema_config_schema_process_io_प्राथमिकता (container->container_def->process->io_प्राथमिकता);
          यदि (process->io_प्राथमिकता == रिक्त)
            OOM ();
        }

      यदि (process->scheduler == रिक्त && container->container_def->process->scheduler)
        {
          process->scheduler = clone_runtime_spec_schema_config_schema_process_scheduler (container->container_def->process->scheduler);
          यदि (process->scheduler == रिक्त)
            OOM ();
        }

      यदि (process->selinux_label == रिक्त && container->container_def->process->selinux_label)
        process->selinux_label = xstrdup (container->container_def->process->selinux_label);

      यदि (process->apparmor_profile == रिक्त && container->container_def->process->apparmor_profile)
        process->apparmor_profile = xstrdup (container->container_def->process->apparmor_profile);

      यदि (process->user == रिक्त && container->container_def->process->user)
        {
          process->user = clone_runtime_spec_schema_config_schema_process_user (container->container_def->process->user);
          यदि (process->user == रिक्त)
            OOM ();
        }
    }

  ret = initialize_security (container, process, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* Do not leak the session keyring of the caller into the container.  The
     keyring created for the container cannot be joined, so use an anonymous one.  */
  यदि (! context->no_new_keyring && ! स्थितिः.no_new_keyring)
    {
      libcrun_debug ("Creating new keyring for the exec process");

      ret = libcrun_create_keyring (container, रिक्त, process->selinux_label, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  ret = prctl (PR_समुच्चयः_DUMPABLE, 0, 0, 0, 0);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "prctl unset dumpable");

  pid = libcrun_join_process (context, container, स्थितिः.pid, &स्थितिः, opts->cgroup, context->detach,
                              process, process->terminal ? &terminal_fd : रिक्त, err);
  यदि (UNLIKELY (pid < 0))
    प्रत्यागम pid;

  /* Process to exec.  */
  यदि (pid == 0)
    {
      अस्थायी_FAILURE_RETRY (close (pipefd0));
      pipefd0 = -1;

      exec_process_entrypoint (context, container, process, &pipefd1, seccomp_fd, seccomp_receiver_fd, custom_handler, opts->merge_env, err);
      /* It gets here only on errors.  */
      यदि (*err)
        {
          यदि (pipefd1 < 0)
            libcrun_fail_with_error ((*err)->स्थितिः, "%s", (*err)->सन्देशः);
          अन्यथा
            {
              write_error_गन्तव्यम्_pipe (pipefd1, err);
              अस्थायी_FAILURE_RETRY (close (pipefd1));
              pipefd1 = -1;
            }
        }
      _safe_exit (EXIT_FAILURE);
    }

  अस्थायी_FAILURE_RETRY (close (pipefd1));
  pipefd1 = -1;

  अस्थायी_FAILURE_RETRY (लिख (pipefd0, &pid, परिमाण (pid)));

  यदि (seccomp_fd >= 0)
    close_and_reset (&seccomp_fd);

  यदि (terminal_fd >= 0)
    {
      अचिह्नित ह्रस्व पङ्क्तिकाः = 0, स्तम्भसङ्ख्या = 0;

      यदि (process->console_आकार)
        {
          स्तम्भसङ्ख्या = process->console_आकार->विस्तारः;
          पङ्क्तिकाः = process->console_आकार->height;
        }

      ret = libcrun_terminal_सज्जता_आकार (terminal_fd, पङ्क्तिकाः, स्तम्भसङ्ख्या, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      यदि (context->console_socket)
        {
          पूर्णाङ्क ret;
          cleanup_close पूर्णाङ्क console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
          यदि (UNLIKELY (console_socket_fd < 0))
            प्रत्यागम console_socket_fd;
          ret = send_fd_गन्तव्यम्_socket (console_socket_fd, terminal_fd, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
          close_and_reset (&terminal_fd);
        }
      अन्यथा
        {
          ret = libcrun_clear_onlcr (terminal_fd, err);
          यदि (UNLIKELY (ret < 0))
            {
              flush_fd_गन्तव्यम्_err (context, terminal_fd);
              प्रत्यागम ret;
            }

          ret = libcrun_समुच्चयः_raw (0, &orig_terminal, err);
          यदि (UNLIKELY (ret < 0))
            {
              flush_fd_गन्तव्यम्_err (context, terminal_fd);
              प्रत्यागम ret;
            }
        }
    }

  ret = अस्थायी_FAILURE_RETRY (पठ (pipefd0, &ret_from_child, परिमाण (ret_from_child)));
  यदि (ret != परिमाण (ret_from_child))
    ret = crun_make_error (err, 0, "read pipe failed");
  अन्यथा यदि (ret_from_child != 0)
    {
      cleanup_free वर्ण *सन्देशः = रिक्त;
      size_t len = 0;

      ret = read_all_fd (pipefd0, "error stream", &सन्देशः, &len, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
      /* the string from read_all_fd is always NUL terminated.  */
      ret = crun_make_error (err, ret_from_child, "%s", सन्देशः);
    }
  अन्यथा
    {
      /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
      यदि (own_seccomp_receiver_fd >= 0)
        {
          seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
          यदि (UNLIKELY (seccomp_notify_fd < 0))
            प्रत्यागम seccomp_notify_fd;

          ret = close_and_reset (&own_seccomp_receiver_fd);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "close seccomp receiver fd failed");
        }

      {
        संरचना wait_for_process_तर्काः तर्काः = {
          .pid = pid,
          .context = context,
          .terminal_fd = terminal_fd,
          .notify_socket = -1,
          .container_ready_fd = रिक्त,
          .seccomp_notify_fd = seccomp_notify_fd,
          .seccomp_notify_plugins = seccomp_notify_plugins,
        };

        ret = wait_for_process (&तर्काः, err);
      }
    }

  flush_fd_गन्तव्यम्_err (context, terminal_fd);
  प्रत्यागम ret;
}

पूर्णाङ्क
libcrun_container_update (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *content, size_t len arg_अप्रयुक्तम्,
                          libcrun_error_t *err)
{
  cleanup_custom_handler_instance संरचना custom_handler_instance_s *custom_handler = रिक्त;
  runtime_spec_schema_config_linux_resources *resources = रिक्त;
  cleanup_container libcrun_container_t *container = रिक्त;
  ध्रुव वर्ण *state_मूलम् = context->state_मूलम्;
  संरचना parser_context ctx = { 0, stderr };
  libcrun_container_स्थितिः_t स्थितिः = {};
  parser_error parser_err = रिक्त;
  json_object *doc = रिक्त;
  पूर्णाङ्क ret;

  ret = libcrun_read_container_स्थितिः (&स्थितिः, state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = read_container_config_from_state (&container, state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  resources = make_runtime_spec_schema_config_linux_resources (doc, &ctx, &parser_err);
  यदि (UNLIKELY (resources == रिक्त))
    {
      ret = crun_make_error (err, 0, "cannot parse resources: %s", parser_err);
      गच्छ cleanup;
    }

  यदि (custom_handler && custom_handler->vtable->modify_oci_configuration)
    {
      /* Adapt RESOURCES to be used from the modify_oci_configuration hook.  */
      cleanup_free runtime_spec_schema_config_linux *linux = xmalloc0 (परिमाण (*linux));
      cleanup_free runtime_spec_schema_config_schema *def = xmalloc0 (परिमाण (*def));

      def->linux = linux;
      linux->resources = resources;

      ret = custom_handler->vtable->modify_oci_configuration (custom_handler->cookie,
                                                              context,
                                                              def,
                                                              err);
      यदि (UNLIKELY (ret < 0))
        गच्छ cleanup;
    }

  ret = libcrun_linux_container_update (&स्थितिः, state_मूलम्, resources, err);

cleanup:
  यदि (doc)
    json_object_put (doc);
  मुक्त (parser_err);
  यदि (resources)
    free_runtime_spec_schema_config_linux_resources (resources);

  प्रत्यागम ret;
}

पूर्णाङ्क
libcrun_container_update_from_file (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *file, libcrun_error_t *err)
{
  cleanup_free वर्ण *content = रिक्त;
  size_t len;
  पूर्णाङ्क ret;

  ret = read_all_file (file, &content, &len, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम libcrun_container_update (context, id, content, len, err);
}

स्थिर पूर्णाङ्क
compare_update_मानानि (ध्रुव शून्य *a, ध्रुव शून्य *b)
{
  ध्रुव संरचना libcrun_update_मानम्_s *aa = a;
  ध्रुव संरचना libcrun_update_मानम्_s *bb = b;
  पूर्णाङ्क ret;

  ret = वाक्यं_तुलय (aa->section, bb->section);
  यदि (ret)
    प्रत्यागम ret;
  प्रत्यागम वाक्यं_तुलय (aa->name, bb->name);
}

पूर्णाङ्क
libcrun_container_update_from_मानानि (libcrun_context_t *context, ध्रुव वर्ण *id,
                                      संरचना libcrun_update_मानम्_s *मानानि, size_t len,
                                      libcrun_error_t *err)
{
  ध्रुव वर्ण *current_section = रिक्त;
  ध्रुव वर्ण *buf;
  json_gen_ctx *gen = रिक्त;
  size_t i, buf_len;
  पूर्णाङ्क ret;

  यदि (! json_gen_init (&gen, रिक्त))
    प्रत्यागम crun_make_error (err, 0, "json_gen_init failed");
  json_gen_map_open (gen);

  शीघ्रं_क्रमय (मानानि, len, परिमाण (संरचना libcrun_update_मानम्_s), compare_update_मानानि);

  प्रति (i = 0; i < len; i++)
    {
      यदि (current_section == रिक्त || वाक्यं_तुलय (मानानि[i].section, current_section))
        {
          यदि (i > 0)
            json_gen_map_close (gen);

          current_section = मानानि[i].section;
          json_gen_string (gen, current_section, वाक्यदैर्घ्यम् (current_section));
          json_gen_map_open (gen);
        }

      json_gen_string (gen, मानानि[i].name, वाक्यदैर्घ्यम् (मानानि[i].name));

      यदि (मानानि[i].numeric)
        json_gen_सङ्ख्या (gen, मानानि[i].मानम्, वाक्यदैर्घ्यम् (मानानि[i].मानम्));
      अन्यथा
        json_gen_string (gen, मानानि[i].मानम्, वाक्यदैर्घ्यम् (मानानि[i].मानम्));
    }
  यदि (len)
    json_gen_map_close (gen);

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);

  ret = libcrun_container_update (context, id, buf, buf_len, err);

  json_gen_free (gen);

  प्रत्यागम ret;
}

स्थिर शून्य
populate_array_क्षेत्रम् (वर्ण ***क्षेत्रम्, वर्ण *array[], size_t अङ्कः_elements)
{
  size_t i;

  *क्षेत्रम् = xmalloc0 ((अङ्कः_elements + 1) * परिमाण (वर्ण *));
  प्रति (i = 0; i < अङ्कः_elements; i++)
    (*क्षेत्रम्)[i] = xstrdup (array[i]);

  (*क्षेत्रम्)[i] = रिक्त;
}

#यदि_परिभाषितम् HAVE_CAP
स्थिर शून्य
populate_capabilities (संरचना features_info_s *info, वर्ण ***capabilities, size_t *अङ्कः_capabilities)
{
  size_t सूचकांक = 0;
  cap_मानम्_t i;
  वर्ण *endptr;
  पूर्णाङ्क j;

  *अङ्कः_capabilities = 0;
  प्रति (i = 0;; i++)
    {
      वर्ण *v = cap_गन्तव्यम्_name (i);
      यदि (v == रिक्त)
        भङ्ग;
      strtol (v, &endptr, 10);
      यदि (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          भङ्ग;
        }
      (*अङ्कः_capabilities)++;
    }

  *capabilities = xmalloc0 ((*अङ्कः_capabilities + 1) * परिमाण (ध्रुव वर्ण *));
  प्रति (i = 0; i < (cap_मानम्_t) *अङ्कः_capabilities; i++)
    {
      वर्ण *v = cap_गन्तव्यम्_name (i);
      यदि (v == रिक्त)
        भङ्ग;
      strtol (v, &endptr, 10);
      यदि (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          भङ्ग;
        }

      // Convert capability name to uppercase
      प्रति (j = 0; v[j] != '\0'; j++)
        v[j] = महाक्षरम् (v[j]);

      (*capabilities)[सूचकांक] = v;
      सूचकांक++;
    }

  (*capabilities)[सूचकांक] = रिक्त; // Terminate the array with NULL
  populate_array_क्षेत्रम् (&(info->linux.capabilities), *capabilities, *अङ्कः_capabilities);
}
#यदि_अन्तः

स्थिर शून्य
retrieve_mount_options (संरचना features_info_s **info)
{
  cleanup_free ध्रुव संरचना सञ्चरणम्_flags_s *mount_options_list = रिक्त;
  size_t अङ्कः_mount_options = 0;

  // Retrieve mount options from wordlist
  mount_options_list = get_mount_flags_from_wordlist ();

  // Calculate the number of mount options
  यावत् (mount_options_list[अङ्कः_mount_options].name != रिक्त)
    अङ्कः_mount_options++;

  // Allocate memory for mount options in info struct
  (*info)->mount_options = xmalloc0 ((अङ्कः_mount_options + 1) * परिमाण (वर्ण *));

  // Copy mount options to info struct
  प्रति (size_t i = 0; i < अङ्कः_mount_options; i++)
    (*info)->mount_options[i] = xstrdup (mount_options_list[i].name);
}

पूर्णाङ्क
libcrun_container_get_features (libcrun_context_t *context, संरचना features_info_s **info, libcrun_error_t *err arg_अप्रयुक्तम्)
{
  // Allocate memory for the features_info_s structure
  size_t अङ्कः_namspaces = परिमाण (namespaces) / परिमाण (namespaces[0]);
  size_t अङ्कः_operators = परिमाण (operators) / परिमाण (operators[0]);
  size_t अङ्कः_actions = परिमाण (actions) / परिमाण (actions[0]);
  size_t अङ्कः_hooks = परिमाण (hooks) / परिमाण (hooks[0]);
  size_t अङ्कः_archs = परिमाण (archs) / परिमाण (archs[0]);
  size_t अङ्कः_unsafe_annotations = परिमाण (potentially_unsafe_annotations) / परिमाण (potentially_unsafe_annotations[0]);
  cleanup_free वर्ण **capabilities = रिक्त;
  size_t अङ्कः_capabilities = 0;
  size_t अङ्कः_mempolicy_modes = परिमाण (mempolicy_modes) / परिमाण (mempolicy_modes[0]);
  size_t अङ्कः_mempolicy_flags = परिमाण (mempolicy_flags) / परिमाण (mempolicy_flags[0]);

  *info = xmalloc0 (परिमाण (संरचना features_info_s));

  // Hardcoded feature information
  (*info)->oci_संस्करणम्_min = xstrdup ("1.0.0");
  (*info)->oci_संस्करणम्_max = xstrdup ("1.1.0+dev");

  // Populate hooks
  populate_array_क्षेत्रम् (&((*info)->hooks), hooks, अङ्कः_hooks);

  // Populate mount_options
  retrieve_mount_options (info);

  // Populate namespaces
  populate_array_क्षेत्रम् (&((*info)->linux.namespaces), namespaces, अङ्कः_namspaces);

#यदि_परिभाषितम् HAVE_CAP
  // Populate capabilities
  populate_capabilities (*info, &capabilities, &अङ्कः_capabilities);
#यदि_अन्तः

  // Hardcode the values for cgroup
  (*info)->linux.cgroup.v1 = true;
  (*info)->linux.cgroup.v2 = true;
#यदि_परिभाषितम् HAVE_SYSTEMD
  (*info)->linux.cgroup.systemd = true;
  (*info)->linux.cgroup.systemd_user = true;
#यदि_अन्तः

  // Put seccomp values
#यदि_परिभाषितम् HAVE_SECCOMP
  (*info)->linux.seccomp.enabled = true;
  // Populate actions
  populate_array_क्षेत्रम् (&((*info)->linux.seccomp.actions), actions, अङ्कः_actions);
  // Populate operators
  populate_array_क्षेत्रम् (&((*info)->linux.seccomp.operators), operators, अङ्कः_operators);
  // Populate archs
  populate_array_क्षेत्रम् (&((*info)->linux.seccomp.archs), archs, अङ्कः_archs);
#अन्यथा
  (*info)->linux.seccomp.enabled = false;
#यदि_अन्तः

  // Put values for apparmor and selinux
  (*info)->linux.apparmor.enabled = true;
  (*info)->linux.selinux.enabled = true;

  (*info)->linux.intel_rdt.enabled = true;

  (*info)->linux.net_devices.enabled = true;

  populate_array_क्षेत्रम् (&((*info)->linux.स्मृतिः_policy.बहुलकम्), mempolicy_modes, अङ्कः_mempolicy_modes);
  populate_array_क्षेत्रम् (&((*info)->linux.स्मृतिः_policy.flags), mempolicy_flags, अङ्कः_mempolicy_flags);

  // Put the values for mount extensions
  (*info)->linux.mount_ext.idmap.enabled = true;

  // Populate the values for annotations
#यदि_परिभाषितम् HAVE_SECCOMP
  {
    ध्रुव संरचना scmp_संस्करणम् *संस्करणम् = seccomp_संस्करणम् ();
    वर्ण *संस्करणम्_string = रिक्त;

    xasprintf (&संस्करणम्_string, "%u.%u.%u", संस्करणम्->major, संस्करणम्->minor, संस्करणम्->micro);
    (*info)->annotations.io_github_seccomp_libseccomp_संस्करणम् = संस्करणम्_string;
  }
#यदि_अन्तः

  यदि (context->handler_manager && handler_by_name (context->handler_manager, "wasm"))
    (*info)->annotations.सततम्_oci_crun_wasm = true;

#यदि HAVE_CRIU
  (*info)->annotations.सततम्_oci_crun_checkpoint_enabled = true;
#यदि_अन्तः
  (*info)->annotations.सततम्_oci_crun_commit = GIT_संस्करणम्;
  (*info)->annotations.सततम्_oci_crun_संस्करणम् = PACKAGE_संस्करणम्;

  populate_array_क्षेत्रम् (&((*info)->potentially_unsafe_annotations), potentially_unsafe_annotations, अङ्कः_unsafe_annotations);

  प्रत्यागम 0;
}

स्थिर शून्य
features_सङ्कलय_string (json_gen_ctx *json_gen, ध्रुव वर्ण *कुञ्चिका, ध्रुव वर्ण *मानम्)
{
  json_gen_string (json_gen, कुञ्चिका, वाक्यदैर्घ्यम् (कुञ्चिका));
  json_gen_string (json_gen, मानम्, वाक्यदैर्घ्यम् (मानम्));
}

स्थिर शून्य
features_सङ्कलय_bool (json_gen_ctx *json_gen, ध्रुव वर्ण *कुञ्चिका, पूर्णाङ्क मानम्)
{
  json_gen_string (json_gen, कुञ्चिका, वाक्यदैर्घ्यम् (कुञ्चिका));
  json_gen_bool (json_gen, मानम्);
}

स्थिर शून्य
features_सङ्कलय_bool_str (json_gen_ctx *json_gen, ध्रुव वर्ण *कुञ्चिका, पूर्णाङ्क मानम्)
{
  features_सङ्कलय_string (json_gen, कुञ्चिका, मानम् ? "true" : "false");
}

स्थिर शून्य
features_सङ्कलय_array (json_gen_ctx *json_gen, ध्रुव वर्ण *कुञ्चिका, वर्ण **array)
{
  size_t i;

  json_gen_string (json_gen, कुञ्चिका, वाक्यदैर्घ्यम् (कुञ्चिका));
  json_gen_array_open (json_gen);
  यदि (array)
    प्रति (i = 0; array[i] != रिक्त; i++)
      json_gen_string (json_gen, array[i], वाक्यदैर्घ्यम् (array[i]));
  json_gen_array_close (json_gen);
}

स्थिर शून्य
features_सङ्कलय_linux_info (json_gen_ctx *json_gen, ध्रुव संरचना linux_info_s *linux)
{
  json_gen_string (json_gen, "linux", वाक्यदैर्घ्यम् ("linux"));
  json_gen_map_open (json_gen);

  features_सङ्कलय_array (json_gen, "namespaces", linux->namespaces);
  features_सङ्कलय_array (json_gen, "capabilities", linux->capabilities);

  json_gen_string (json_gen, "cgroup", वाक्यदैर्घ्यम् ("cgroup"));
  json_gen_map_open (json_gen);
  features_सङ्कलय_bool (json_gen, "v1", linux->cgroup.v1);
  features_सङ्कलय_bool (json_gen, "v2", linux->cgroup.v2);
  features_सङ्कलय_bool (json_gen, "systemd", linux->cgroup.systemd);
  features_सङ्कलय_bool (json_gen, "systemdUser", linux->cgroup.systemd_user);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "seccomp", वाक्यदैर्घ्यम् ("seccomp"));
  json_gen_map_open (json_gen);
  features_सङ्कलय_bool (json_gen, "enabled", linux->seccomp.enabled);
  यदि (linux->seccomp.actions)
    features_सङ्कलय_array (json_gen, "actions", linux->seccomp.actions);
  यदि (linux->seccomp.operators)
    features_सङ्कलय_array (json_gen, "operators", linux->seccomp.operators);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "apparmor", वाक्यदैर्घ्यम् ("apparmor"));
  json_gen_map_open (json_gen);
  features_सङ्कलय_bool (json_gen, "enabled", linux->apparmor.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "selinux", वाक्यदैर्घ्यम् ("selinux"));
  json_gen_map_open (json_gen);
  features_सङ्कलय_bool (json_gen, "enabled", linux->selinux.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "mountExtensions", वाक्यदैर्घ्यम् ("mountExtensions"));
  json_gen_map_open (json_gen);
  json_gen_string (json_gen, "idmap", वाक्यदैर्घ्यम् ("idmap"));
  json_gen_map_open (json_gen);
  features_सङ्कलय_bool (json_gen, "enabled", linux->mount_ext.idmap.enabled);
  json_gen_map_close (json_gen);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "intelRdt", वाक्यदैर्घ्यम् ("intelRdt"));
  json_gen_map_open (json_gen);
  features_सङ्कलय_bool (json_gen, "enabled", linux->intel_rdt.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "netDevices", वाक्यदैर्घ्यम् ("netDevices"));
  json_gen_map_open (json_gen);
  features_सङ्कलय_bool (json_gen, "enabled", linux->net_devices.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "memoryPolicy", वाक्यदैर्घ्यम् ("memoryPolicy"));
  json_gen_map_open (json_gen);
  यदि (linux->स्मृतिः_policy.बहुलकम्)
    features_सङ्कलय_array (json_gen, "modes", linux->स्मृतिः_policy.बहुलकम्);
  यदि (linux->स्मृतिः_policy.flags)
    features_सङ्कलय_array (json_gen, "flags", linux->स्मृतिः_policy.flags);
  json_gen_map_close (json_gen);

  json_gen_map_close (json_gen);
}

स्थिर शून्य
features_सङ्कलय_annotations_info (json_gen_ctx *json_gen, ध्रुव संरचना annotations_info_s *annotation)
{
  json_gen_string (json_gen, "annotations", वाक्यदैर्घ्यम् ("annotations"));
  json_gen_map_open (json_gen);

  यदि (! is_रिक्तानि_string (annotation->io_github_seccomp_libseccomp_संस्करणम्))
    features_सङ्कलय_string (json_gen, "io.github.seccomp.libseccomp.version", annotation->io_github_seccomp_libseccomp_संस्करणम्);

  features_सङ्कलय_bool_str (json_gen, "org.opencontainers.runc.checkpoint.enabled", annotation->सततम्_oci_crun_checkpoint_enabled);
  features_सङ्कलय_bool_str (json_gen, "run.oci.crun.checkpoint.enabled", annotation->सततम्_oci_crun_checkpoint_enabled);

  features_सङ्कलय_string (json_gen, "run.oci.crun.commit", annotation->सततम्_oci_crun_commit);
  features_सङ्कलय_string (json_gen, "run.oci.crun.version", annotation->सततम्_oci_crun_संस्करणम्);

  features_सङ्कलय_bool_str (json_gen, "run.oci.crun.wasm", annotation->सततम्_oci_crun_wasm);

  json_gen_map_close (json_gen);
}

पूर्णाङ्क
libcrun_container_get_features_json (libcrun_context_t *context, वर्ण **बहिर्गमनम्, libcrun_error_t *err)
{
  cleanup_struct_features संरचना features_info_s *info = रिक्त;
  ध्रुव वर्ण *बफर = रिक्त;
  json_gen_ctx *json_gen;
  size_t बफर_len;
  पूर्णाङ्क ret;

  *बहिर्गमनम् = रिक्त;

  ret = libcrun_container_get_features (context, &info, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (! json_gen_init (&json_gen, रिक्त))
    प्रत्यागम crun_make_error (err, 0, "cannot initialize JSON generator");

  json_gen_config (json_gen, json_gen_beautify, 1);

  json_gen_map_open (json_gen);

  features_सङ्कलय_string (json_gen, "ociVersionMin", info->oci_संस्करणम्_min);
  features_सङ्कलय_string (json_gen, "ociVersionMax", info->oci_संस्करणम्_max);
  features_सङ्कलय_array (json_gen, "hooks", info->hooks);
  features_सङ्कलय_array (json_gen, "mountOptions", info->mount_options);
  features_सङ्कलय_linux_info (json_gen, &info->linux);
  features_सङ्कलय_annotations_info (json_gen, &info->annotations);
  features_सङ्कलय_array (json_gen, "potentiallyUnsafeConfigAnnotations", info->potentially_unsafe_annotations);

  json_gen_map_close (json_gen);

  json_gen_get_buf (json_gen, &बफर, &बफर_len);
  *बहिर्गमनम् = xstrdup (बफर);
  json_gen_free (json_gen);

  प्रत्यागम 0;
}

/* Read the container status and fail if the container is not running.  */
स्थिर पूर्णाङ्क
read_स्थितिः_require_running (libcrun_context_t *context, ध्रुव वर्ण *id,
                             libcrun_container_स्थितिः_t *स्थितिः, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = libcrun_read_container_स्थितिः (स्थितिः, context->state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_is_container_running (स्थितिः, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;
  यदि (ret == 0)
    प्रत्यागम crun_make_error (err, 0, "the container `%s` is not running", id);

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_container_pause (libcrun_context_t *context, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  libcrun_container_स्थितिः_t स्थितिः = {};

  ret = read_स्थितिः_require_running (context, id, &स्थितिः, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम libcrun_container_pause_linux (&स्थितिः, err);
}

पूर्णाङ्क
libcrun_container_unpause (libcrun_context_t *context, ध्रुव वर्ण *id, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  libcrun_container_स्थितिः_t स्थितिः = {};

  ret = read_स्थितिः_require_running (context, id, &स्थितिः, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम libcrun_container_unpause_linux (&स्थितिः, err);
}

/* Translate the public options struct into the internal representation used by
   the linux/CRIU layer.  Strings are borrowed (not copied).  */
स्थिर शून्य
cr_options_from_लौकिकम् (libcrun_checkpoint_restore_t *dst, संरचना libcrun_checkpoint_restore_options_s *src)
{
  स्मृतिं_पूरय (dst, 0, परिमाण (*dst));
  dst->image_मार्गः = (वर्ण *) src->image_मार्गः;
  dst->work_मार्गः = (वर्ण *) src->work_मार्गः;
  dst->parent_मार्गः = (वर्ण *) src->parent_मार्गः;
  dst->console_socket = src->console_socket;
  dst->lsm_profile = (वर्ण *) src->lsm_profile;
  dst->lsm_mount_context = (वर्ण *) src->lsm_mount_context;
  dst->leave_running = src->leave_running;
  dst->tcp_established = src->tcp_established;
  dst->tcp_close = src->tcp_close;
  dst->ext_unix_sk = src->ext_unix_sk;
  dst->shell_job = src->shell_job;
  dst->file_locks = src->file_locks;
  dst->pre_dump = src->pre_dump;
  dst->detach = src->detach;
  dst->manage_cgroups_बहुलकम् = src->manage_cgroups_बहुलकम्;
  dst->जालम्_तालकम्_विधानम् = src->जालम्_तालकम्_विधानम्;
}

पूर्णाङ्क
libcrun_container_checkpoint (libcrun_context_t *context, ध्रुव वर्ण *id,
                              संरचना libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  ध्रुव वर्ण *state_मूलम् = context->state_मूलम्;
  libcrun_container_स्थितिः_t स्थितिः = {};
  libcrun_checkpoint_restore_t cr;
  cleanup_container libcrun_container_t *container = रिक्त;

  cr_options_from_लौकिकम् (&cr, cr_options);

  ret = read_स्थितिः_require_running (context, id, &स्थितिः, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = read_container_config_from_state (&container, state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* Needed to use the same paths (e.g. the shared empty directory used for
     masked paths) as the container runtime and restore do.  */
  container->context = context;

  ret = libcrun_container_checkpoint_linux (&स्थितिः, container, &cr, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (! (cr_options->leave_running || cr_options->pre_dump))
    प्रत्यागम container_delete_आन्तरिकम् (context, रिक्त, रिक्त, id, true, true, err);

  प्रत्यागम 0;
}

/* The dummy process only keeps the cgroup populated until the container is restored there.
   Its cgroup is below the container cgroup, so once the container runs it can freeze or kill
   the dummy process: never depend on it after that point.  */
स्थिर शून्य
restore_proxy_process (पूर्णाङ्क proxy_pid_pipe)
{
  वर्ण c;

  /* Wait to be killed by the parent, or for the parent to go away.  */
  अस्थायी_FAILURE_RETRY (पठ (proxy_pid_pipe, &c, परिमाण (c)));
}

स्थिर पूर्णाङ्क
restore_terminate_proxy_process (pid_t proxy_pid, ध्रुव वर्ण *proxy_cgroup, pid_t container_pid, पूर्णाङ्क cgroup_manager, libcrun_error_t *err)
{
  पूर्णाङ्क बहुलकम्;
  पूर्णाङ्क ret;

  यदि (cgroup_manager == CGROUP_MANAGER_SYSTEMD)
    {
      वर्ण ready_str[64];

      ret = snprintf (ready_str, परिमाण (ready_str), "MAINPID=%d", container_pid);
      यदि (UNLIKELY (ret >= (पूर्णाङ्क) परिमाण (ready_str)))
        प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

      ret = send_sd_notify (ready_str, err);
      /* Do not fail on errors.  */
      यदि (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
  यदि (UNLIKELY (बहुलकम् < 0))
    प्रत्यागम बहुलकम्;

  /* SIGKILL terminates a process frozen with cgroup v2, while the cgroup v1 freezer does not
     deliver it until the cgroup is thawed, so do not wait for it there.  */
  kill (proxy_pid, SIGKILL);
  अस्थायी_FAILURE_RETRY (waitpid (proxy_pid, रिक्त, बहुलकम् == CGROUP_बहुलकम्_UNIFIED ? 0 : WNOHANG));

  प्रत्यागम destroy_cgroup_मार्गः (proxy_cgroup, बहुलकम्, err);
}

पूर्णाङ्क
libcrun_container_restore (libcrun_context_t *context, ध्रुव वर्ण *id arg_अप्रयुक्तम्,
                           संरचना libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  cleanup_cgroup_स्थितिः संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः = रिक्त;
  cleanup_container libcrun_container_t *container = रिक्त;
  cleanup_close पूर्णाङ्क proxy_pid_pipe0 = -1;
  cleanup_close पूर्णाङ्क proxy_pid_pipe1 = -1;
  runtime_spec_schema_config_schema *def;
  libcrun_container_स्थितिः_t स्थितिः = {};
  संरचना libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  libcrun_checkpoint_restore_t cr;
  cleanup_pid pid_t proxy_pid = -1;
  cleanup_close पूर्णाङ्क seccomp_fd = -1;
  ध्रुव वर्ण *seccomp_bpf_दत्त;
  पूर्णाङ्क proxy_pid_pipe[2];
  पूर्णाङ्क cgroup_manager;
  uid_t मूलम्_uid = -1;
  gid_t मूलम्_gid = -1;
  पूर्णाङ्क ret;

  cr_options_from_लौकिकम् (&cr, cr_options);

  container = libcrun_container_आरोपय_from_file ("config.json", err);
  यदि (container == रिक्त)
    प्रत्यागम -1;

  container->context = context;
  def = container->container_def;

  ret = check_config_file (def, context, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_स्थितिः_check_directories (context->state_मूलम्, context->id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_copy_config_file (context->id, context->state_मूलम्, container, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ fail;

  /* CRIU restores the seccomp filters of the checkpointed processes, but
     exec loads the filter from the seccomp.bpf file in the state directory,
     so it must be generated again for the restored container.  */
  seccomp_bpf_दत्त = find_annotation (container, "run.oci.seccomp_bpf_data");

  ret = सज्जता_seccomp (container, seccomp_bpf_दत्त, &seccomp_gen_ctx, &seccomp_fd, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ fail;

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_दत्त, &seccomp_gen_ctx, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ fail;
  close_and_reset (&seccomp_fd);

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  यदि (context->systemd_cgroup)
    cgroup_manager = CGROUP_MANAGER_SYSTEMD;
  अन्यथा यदि (context->force_no_cgroup)
    cgroup_manager = CGROUP_MANAGER_DISABLED;

  def = container->container_def;

  /* If we are root (either on the host or in a namespace),
   * then chown the cgroup to root in the container user namespace. */
  get_मूलम्_अन्तर्_the_userns (def, container->host_uid, container->host_gid, &मूलम्_uid, &मूलम्_gid);

  /* If the root in the container is different than the current root user, attempt to chown
     the std streams before entering the user namespace.  Otherwise we might lose access
     to the user (as it is not mapped in the user namespace) and cannot chown them.  */
  यदि (मूलम्_uid > 0 || मूलम्_gid > 0)
    {
      ret = maybe_chown_std_streams (मूलम्_uid, मूलम्_gid, err);
      यदि (UNLIKELY (ret < 0))
        गच्छ fail;
    }

  /* CRIU runs as a child process, so the restored processes would inherit the session keyring.  */
  ret = सज्जता_container_keyring (container, context, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  {
    cleanup_close पूर्णाङ्क cgroup_dirfd = -1;
    bool needs_proxy_process;
    cleanup_cgroup_preenter संरचना libcrun_cgroup_तर्काः cg = {
      .resources = def->linux ? def->linux->resources : रिक्त,
      .annotations = container->annotations,
      .cgroup_मार्गः = def->linux ? def->linux->cgroups_मार्गः : "",
      .manager = cgroup_manager,
      .मूलम्_uid = मूलम्_uid,
      .मूलम्_gid = मूलम्_gid,
      .id = context->id,
      .state_मूलम् = context->state_मूलम्,
      .container = container,
    };

    /* The CRIU restore code uses bundle, rootfs and cgroup_path of status.   The cgroup_path is set later.  */
    स्थितिः.bundle = (वर्ण *) context->bundle;
    स्थितिः.rootfs = def->मूलम्->मार्गः;

    ret = libcrun_cgroup_preenter (&cg, &cgroup_dirfd, err);
    यदि (UNLIKELY (ret < 0))
      गच्छ fail;

    needs_proxy_process = cgroup_dirfd < 0;

    /* If the target cgroup was already created (with cgroupfs), we can restore the container directly there.  */
    यदि (! needs_proxy_process)
      {
        cleanup_free वर्ण *cgroup_मार्गः = रिक्त;

        ret = get_cgroup_dirfd_मार्गः (cgroup_dirfd, &cgroup_मार्गः, err);
        यदि (UNLIKELY (ret < 0))
          गच्छ fail;

        /* Complete the configuration for the cgroup before the container is
           restored there: CRIU resumes the restored processes before it
           returns, so the device filter and the limits must already be in
           place.  There is no process to move, as CRIU is run in the cgroup,
           and the controllers were enabled by precreate_cgroup.  */
        cg.joined = true;

        ret = libcrun_cgroup_enter (&cg, &cgroup_स्थितिः, err);
        यदि (UNLIKELY (ret < 0))
          गच्छ fail;

        /* Restore the container directly in the desired cgroup.  */
        स्थितिः.cgroup_मार्गः = cgroup_मार्गः;

        ret = libcrun_container_restore_linux (&स्थितिः, container, &cr, err);
        यदि (UNLIKELY (ret < 0))
          गच्छ fail;

        /* Now that the container is restored, keep any signal for it, so that
           it is forwarded to the init by wait_for_process, rather than acted
           upon by crun.  This cannot be done earlier, as CRIU, which is run as
           a child process, would inherit the blocked signals.  */
        ret = खण्डम्_सङ्केताः (err);
        यदि (UNLIKELY (ret < 0))
          गच्छ fail;

        /* Use the container first process PID to finalize the cgroup.  */
        cg.pid = स्थितिः.pid;
      }
    अन्यथा
      {
        /* When using systemd, we need a process first that must exist before a cgroup is created.
           Create a dummy process that sits and waits until the container is restored.  */
        ret = pipe2 (proxy_pid_pipe, O_CLOEXEC);
        यदि (UNLIKELY (ret < 0))
          {
            ret = crun_make_error (err, errno, "pipe");
            गच्छ fail;
          }

        /* Only used for auto cleanup.  */
        proxy_pid_pipe0 = proxy_pid_pipe[0];
        proxy_pid_pipe1 = proxy_pid_pipe[1];

        proxy_pid = fork ();
        यदि (UNLIKELY (proxy_pid < 0))
          {
            ret = crun_make_error (err, errno, "fork");
            गच्छ fail;
          }

        यदि (proxy_pid == 0)
          {
            close_and_reset (&proxy_pid_pipe1);

            restore_proxy_process (proxy_pid_pipe0);
            _safe_exit (EXIT_SUCCESS);
          }

        close_and_reset (&proxy_pid_pipe0);

        /* Use the dummy process PID as the PID to setup the cgroup.  */
        cg.pid = proxy_pid;

        /* Complete the configuration for the cgroup.  It contains only the dummy process.  */
        ret = libcrun_cgroup_enter (&cg, &cgroup_स्थितिः, err);
        यदि (UNLIKELY (ret < 0))
          गच्छ fail;
      }

    ret = libcrun_cgroup_enter_finalize (&cg, cgroup_स्थितिः, err);
    यदि (UNLIKELY (ret < 0))
      गच्छ fail;

    /* When using a dummy process, the container is restored here once the cgroup is configured.  */
    यदि (needs_proxy_process)
      {
        cleanup_free वर्ण *target_cgroup = रिक्त;
        cleanup_free वर्ण *proxy_cgroup = रिक्त;

        ret = libcrun_get_cgroup_process (proxy_pid, &target_cgroup, false, err);
        यदि (UNLIKELY (ret < 0))
          गच्छ fail;

        /* Move the dummy process to a sub-cgroup.  */
        ret = append_paths (&proxy_cgroup, err, target_cgroup, ".proxy-crun", रिक्त);
        यदि (UNLIKELY (ret < 0))
          गच्छ fail;

        ret = libcrun_move_process_गन्तव्यम्_cgroup (proxy_pid, 0, proxy_cgroup, true, err);
        यदि (UNLIKELY (ret < 0))
          गच्छ fail;

        /* Restore the container in the same cgroup where the dummy process was.  */
        स्थितिः.cgroup_मार्गः = target_cgroup;

        ret = libcrun_container_restore_linux (&स्थितिः, container, &cr, err);
        यदि (UNLIKELY (ret < 0))
          गच्छ fail;

        /* See the comment for the other libcrun_container_restore_linux call.  */
        ret = खण्डम्_सङ्केताः (err);
        यदि (UNLIKELY (ret < 0))
          गच्छ fail;

        /* The container is running, so do not fail because of the dummy process.  */
        ret = restore_terminate_proxy_process (proxy_pid, proxy_cgroup, स्थितिः.pid, cgroup_manager, err);
        यदि (UNLIKELY (ret < 0))
          crun_error_release (err);
        proxy_pid = -1;

        close_and_reset (&proxy_pid_pipe1);
      }
  }

  context->detach = cr_options->detach;
  ret = write_container_स्थितिः (container, context, स्थितिः.pid, cgroup_स्थितिः, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ fail;

  {
    /* Wait the same way run and exec do: apart from waiting for the container
       init, this writes the pid file, forwards any signal to the init, and
       reaps every child, not only the init.  */
    संरचना wait_for_process_तर्काः तर्काः = {
      .pid = स्थितिः.pid,
      .context = context,
      .terminal_fd = -1,
      .notify_socket = -1,
      .container_ready_fd = रिक्त,
      .seccomp_notify_fd = -1,
      .seccomp_notify_plugins = रिक्त,
    };

    प्रत्यागम wait_for_process (&तर्काः, err);
  }

fail:
  /* Once restored, the container is running.  Without a status file nothing
     can reach it anymore, so do not leave it behind.  */
  यदि (स्थितिः.pid > 0)
    {
      kill (स्थितिः.pid, SIGKILL);
      waitpid_ignore_stopped (स्थितिः.pid, रिक्त, 0);
    }
  यदि (cgroup_स्थितिः)
    {
      libcrun_error_t tmp_err = रिक्त;
      libcrun_cgroup_destroy (cgroup_स्थितिः, &tmp_err);
      crun_error_release (&tmp_err);
    }
  {
    libcrun_error_t tmp_err = रिक्त;

    यदि (libcrun_container_delete_स्थितिः (context->state_मूलम्, context->id, &tmp_err) < 0)
      crun_error_release (&tmp_err);
  }
  प्रत्यागम ret;
}

पूर्णाङ्क
libcrun_container_read_pids (libcrun_context_t *context, ध्रुव वर्ण *id, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  cleanup_cgroup_स्थितिः संरचना libcrun_cgroup_स्थितिः *cgroup_स्थितिः = रिक्त;
  cleanup_container_स्थितिः libcrun_container_स्थितिः_t स्थितिः = {};
  पूर्णाङ्क ret;

  ret = libcrun_read_container_स्थितिः (&स्थितिः, context->state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (स्थितिः.cgroup_मार्गः == रिक्त || स्थितिः.cgroup_मार्गः[0] == '\0')
    प्रत्यागम crun_make_error (err, 0, "the container is not using cgroups");

  cgroup_स्थितिः = libcrun_cgroup_make_स्थितिः (&स्थितिः);

  प्रत्यागम libcrun_cgroup_read_pids (cgroup_स्थितिः, recurse, pids, err);
}

पूर्णाङ्क
libcrun_container_list_json (libcrun_context_t *context, वर्ण **बहिर्गमनम्, libcrun_error_t *err)
{
  libcrun_container_list_t *list = रिक्त, *it;
  ध्रुव वर्ण *content = रिक्त;
  json_gen_ctx *gen = रिक्त;
  size_t len;
  पूर्णाङ्क ret;

  *बहिर्गमनम् = रिक्त;

  ret = libcrun_get_containers_list (&list, context->state_मूलम्, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (! json_gen_init (&gen, रिक्त))
    {
      ret = crun_make_error (err, 0, "cannot allocate json generator");
      गच्छ निर्गच्छ;
    }

  json_gen_config (gen, json_gen_beautify, 1);
  json_gen_array_open (gen);

  प्रति (it = list; it; it = it->अग्र)
    {
      libcrun_container_स्थितिः_t स्थितिः;
      पूर्णाङ्क running = 0;
      पूर्णाङ्क pid;
      ध्रुव वर्ण *container_स्थितिः = रिक्त;

      ret = libcrun_read_container_स्थितिः (&स्थितिः, context->state_मूलम्, it->name, err);
      यदि (UNLIKELY (ret < 0))
        गच्छ निर्गच्छ;

      pid = स्थितिः.pid;

      ret = libcrun_get_container_state_string (it->name, &स्थितिः, context->state_मूलम्, &container_स्थितिः,
                                                &running, err);
      यदि (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          अनुवर्त;
        }

      यदि (! running)
        pid = 0;

      json_gen_map_open (gen);
      json_gen_string (gen, "id", वाक्यदैर्घ्यम् ("id"));
      json_gen_string (gen, it->name, वाक्यदैर्घ्यम् (it->name));
      json_gen_string (gen, "pid", वाक्यदैर्घ्यम् ("pid"));
      map_int (gen, pid);
      json_gen_string (gen, "status", वाक्यदैर्घ्यम् ("status"));
      json_gen_string (gen, container_स्थितिः, वाक्यदैर्घ्यम् (container_स्थितिः));
      json_gen_string (gen, "bundle", वाक्यदैर्घ्यम् ("bundle"));
      json_gen_string (gen, स्थितिः.bundle, वाक्यदैर्घ्यम् (स्थितिः.bundle));
      json_gen_string (gen, "created", वाक्यदैर्घ्यम् ("created"));
      json_gen_string (gen, स्थितिः.created, वाक्यदैर्घ्यम् (स्थितिः.created));
      json_gen_string (gen, "owner", वाक्यदैर्घ्यम् ("owner"));
      json_gen_string (gen, स्थितिः.owner, safe_strlen (स्थितिः.owner));
      json_gen_map_close (gen);

      libcrun_free_container_स्थितिः (&स्थितिः);
    }

  json_gen_array_close (gen);
  यदि (json_gen_get_buf (gen, &content, &len) != json_gen_स्थितिः_ok)
    {
      ret = libcrun_make_error (err, 0, "cannot generate json list");
      गच्छ निर्गच्छ;
    }

  *बहिर्गमनम् = xstrdup (content);
  ret = 0;

निर्गच्छ:
  यदि (list)
    libcrun_free_containers_list (list);
  यदि (gen)
    json_gen_free (gen);

  प्रत्यागम ret;
}

पूर्णाङ्क
libcrun_write_json_containers_list (libcrun_context_t *context, FILE *बहिर्गमनम्, libcrun_error_t *err)
{
  cleanup_free वर्ण *content = रिक्त;
  size_t len;
  पूर्णाङ्क ret;

  ret = libcrun_container_list_json (context, &content, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  len = वाक्यदैर्घ्यम् (content);
  यदि (len && fwrite (content, 1, len, बहिर्गमनम्) != len)
    प्रत्यागम crun_make_error (err, errno, "error writing to file");

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_container_update_intel_rdt (libcrun_context_t *context, ध्रुव वर्ण *id, संरचना libcrun_intel_rdt_update *update, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = रिक्त;
  पूर्णाङ्क ret;

  ret = read_container_config_from_state (&container, context->state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम libcrun_update_intel_rdt (id, container, update->l3_cache_schema, update->mem_bw_schema, update->schemata, err);
}

स्थिर पूर्णाङ्क
libcrun_container_सङ्कलय_or_remove_mounts_from_file (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *file, bool सङ्कलय, libcrun_error_t *err)
{
  cleanup_custom_handler_instance संरचना custom_handler_instance_s *custom_handler = रिक्त;
  cleanup_container libcrun_container_t *container = रिक्त;
  cleanup_free runtime_spec_schema_defs_mount **mounts = रिक्त;
  ध्रुव वर्ण *state_मूलम् = context->state_मूलम्;
  cleanup_free parser_error parser_err = रिक्त;
  संरचना parser_context pctx = { 0, stderr };
  libcrun_container_स्थितिः_t स्थितिः = {};
  cleanup_free वर्ण *content = रिक्त;
  json_object *doc = रिक्त;
  size_t n_mounts = 0, len, i;
  पूर्णाङ्क ret = 1;

  ret = read_all_file (file, &content, &len, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_read_container_स्थितिः (&स्थितिः, state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = read_container_config_from_state (&container, state_मूलम्, id, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = parse_json_file (&doc, content, &pctx, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (! json_object_is_type (doc, json_type_array))
    {
      ret = crun_make_error (err, 0, "mounts must be an array");
      गच्छ cleanup;
    }
  अन्यथा
    {
      n_mounts = json_object_array_दीर्घता (doc);

      mounts = xmalloc0 ((n_mounts + 1) * परिमाण (*mounts));
      प्रति (i = 0; i < n_mounts; i++)
        {
          mounts[i] = make_runtime_spec_schema_defs_mount (json_object_array_get_idx (doc, i), &pctx, &parser_err);
          यदि (mounts[i] == रिक्त)
            {
              ret = crun_make_error (err, 0, "cannot parse mount: %s", parser_err);
              गच्छ cleanup;
            }
        }
    }

  यदि (सङ्कलय)
    ret = libcrun_make_runtime_mounts (container, &स्थितिः, mounts, n_mounts, err);
  अन्यथा
    ret = libcrun_destroy_runtime_mounts (container, &स्थितिः, mounts, n_mounts, err);

cleanup:
  यदि (doc)
    json_object_put (doc);
  यदि (mounts)
    प्रति (i = 0; i < n_mounts; i++)
      free_runtime_spec_schema_defs_mount (mounts[i]);
  प्रत्यागम ret;
}

पूर्णाङ्क
libcrun_container_सङ्कलय_mounts_from_file (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *file, libcrun_error_t *err)
{
  प्रत्यागम libcrun_container_सङ्कलय_or_remove_mounts_from_file (context, id, file, true, err);
}

पूर्णाङ्क
libcrun_container_remove_mounts_from_file (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *file, libcrun_error_t *err)
{
  प्रत्यागम libcrun_container_सङ्कलय_or_remove_mounts_from_file (context, id, file, false, err);
}
