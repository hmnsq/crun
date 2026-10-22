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
#నిర్వచించు GNU_ఆధారం

#అన్తర్భావ <config.h>
#అన్తర్భావ <ocispec/runtime_spec_schema_config_schema.h>
#అన్తర్భావ <stdbool.h>
#అన్తర్భావ "container.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "json_gen_utils.h"
#అన్తర్భావ "seccomp.h"
#అన్తర్భావ "mempolicy.h"
#నిర్వచితమైతే HAVE_SECCOMP
#  include <seccomp.h>
#అయితే_ముగింపు
#అన్తర్భావ "scheduler.h"
#అన్తర్భావ "seccomp_notify.h"
#అన్తర్భావ "custom-handler.h"
#అన్తర్భావ <stdbool.h>
#అన్తర్భావ <argp.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <sched.h>
#అన్తర్భావ <sys/wait.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ "status.h"
#అన్తర్భావ "mount_flags.h"
#అన్తర్భావ "linux.h"
#అన్తర్భావ "terminal.h"
#అన్తర్భావ "io_priority.h"
#అన్తర్భావ "cgroup.h"
#అన్తర్భావ "cgroup-utils.h"
#అన్తర్భావ <sys/prctl.h>
#అన్తర్భావ <sys/auxv.h>
#అన్తర్భావ <sys/signalfd.h>
#అన్తర్భావ <sys/epoll.h>
#అన్తర్భావ <sys/socket.h>
#నిర్వచితమైతే HAVE_CAP
#  include <sys/capability.h>
#అయితే_ముగింపు
#అన్తర్భావ <sys/ioctl.h>
#అన్తర్భావ <termios.h>
#అన్తర్భావ <grp.h>
#అన్తర్భావ <git-version.h>

#నిర్వచితమైతే HAVE_SYSTEMD
#  include <systemd/sd-daemon.h>
#అయితే_ముగింపు

#అన్తర్భావ <json-c/json.h>
#అన్తర్భావ <ocispec/json_common.h>

గణన
{
  SYNC_SOCKET_SYNC_సందేశం,
  SYNC_SOCKET_ERROR_సందేశం,
  SYNC_SOCKET_WARNING_సందేశం,
  SYNC_SOCKET_DEBUG_సందేశం,
};

నిర్మాణం container_entrypoint_s
{
  libcrun_container_t *container;
  libcrun_context_t *context;
  పూర్ణాంకం has_terminal_socket_pair;
  పూర్ణాంకం terminal_socketpair[2];

  /* Used by log_write_to_sync_socket.  */
  పూర్ణాంకం sync_socket;

  పూర్ణాంకం seccomp_fd;
  పూర్ణాంకం seccomp_receiver_fd;
  పూర్ణాంకం console_socket_fd;

  పూర్ణాంకం hooks_బహిర్గతం_fd;
  పూర్ణాంకం hooks_err_fd;

  /* Absolute bundle path, to be reported to hooks run in the container.  */
  స్థిరరాశి అక్షరం *bundle;

  నిర్మాణం custom_handler_instance_s *custom_handler;
};

నిర్మాణం sync_socket_సందేశం_s
{
  పూర్ణాంకం type;
  పూర్ణాంకం error_విలువ;
  అక్షరం సందేశం_2[512];
};

రకనిర్వచనం runtime_spec_schema_defs_hook hook;

// linux hooks
అక్షరం *hooks[] = {
  "prestart",
  "createRuntime",
  "createContainer",
  "startContainer",
  "poststart",
  "poststop"
};

// linux namespaces
స్థిర అక్షరం *namespaces[] = {
  "cgroup",
  "ipc",
  "mount",
  "network",
  "pid",
  "user",
  "uts"
};

స్థిర అక్షరం *actions[] = {
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

స్థిర అక్షరం *operators[] = {
  "SCMP_CMP_NE",
  "SCMP_CMP_LT",
  "SCMP_CMP_LE",
  "SCMP_CMP_EQ",
  "SCMP_CMP_GE",
  "SCMP_CMP_GT",
  "SCMP_CMP_MASKED_EQ",
};

స్థిర అక్షరం *archs[] = {
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

స్థిర అక్షరం *mempolicy_modes[] = {
  "MPOL_DEFAULT",
  "MPOL_PREFERRED",
  "MPOL_BIND",
  "MPOL_INTERLEAVE",
  "MPOL_LOCAL",
  "MPOL_PREFERRED_MANY",
  "MPOL_WEIGHTED_INTERLEAVE"
};

స్థిర అక్షరం *mempolicy_flags[] = {
  "MPOL_F_NUMA_BALANCING",
  "MPOL_F_RELATIVE_NODES",
  "MPOL_F_STATIC_NODES"
};

స్థిర అక్షరం *potentially_unsafe_annotations[] = {
  "module.wasm.image/variant",
  "io.kubernetes.cri.container-type",
  "run.oci.",
  "org.criu.",
  "krun.",
};

#నిర్వచించు SYNC_SOCKET_సందేశం_LEN(x, l) (offsetof (నిర్మాణం sync_socket_సందేశం_s, సందేశం_2) + l)

స్థిర పూర్ణాంకం
sync_socket_write_సందేశం (పూర్ణాంకం fd, పూర్ణాంకం verbosity, పూర్ణాంకం err_విలువ, స్థిరరాశి అక్షరం *log_సందేశం)
{
  పూర్ణాంకం ret;
  size_t err_len;
  నిర్మాణం sync_socket_సందేశం_s సందేశం;
  ఎంపిక (verbosity)
    {
    స్థితి LIBCRUN_VERBOSITY_DEBUG:
      సందేశం.type = SYNC_SOCKET_DEBUG_సందేశం;
      ఆపు;
    స్థితి LIBCRUN_VERBOSITY_WARNING:
      సందేశం.type = SYNC_SOCKET_WARNING_సందేశం;
      ఆపు;
    స్థితి LIBCRUN_VERBOSITY_ERROR:
      సందేశం.type = SYNC_SOCKET_ERROR_సందేశం;
      ఆపు;
    }
  సందేశం.error_విలువ = err_విలువ;

  ఒకవేళ (fd < 0)
    తిరిగి 0;

  err_len = అక్షరశ్రేణి_పొడవు (log_సందేశం);
  ఒకవేళ (err_len >= పరిమాణం (సందేశం.సందేశం_2))
    err_len = పరిమాణం (సందేశం.సందేశం_2) - 1;

  జ్ఞాపకం_నకలు (సందేశం.సందేశం_2, log_సందేశం, err_len);
  సందేశం.సందేశం_2[err_len] = '\0';

  ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (fd, &సందేశం, SYNC_SOCKET_సందేశం_LEN (సందేశం, err_len + 1)));
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి -1;

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
sync_socket_write_error (పూర్ణాంకం fd, libcrun_error_t *బహిర్గతం_err)
{
  ఒకవేళ (fd < 0)
    తిరిగి 0;
  తిరిగి sync_socket_write_సందేశం (fd, false, (*బహిర్గతం_err)->స్థితి_2, (*బహిర్గతం_err)->సందేశం);
}

స్థిర శూన్య
log_write_గమ్యం_sync_socket (పూర్ణాంకం errno_, స్థిరరాశి అక్షరం *సందేశం, పూర్ణాంకం verbosity, శూన్య *arg)
{
  నిర్మాణం container_entrypoint_s *entrypoint_వాదనలు = arg;
  పూర్ణాంకం fd = entrypoint_వాదనలు->sync_socket;

  ఒకవేళ (fd < 0)
    తిరిగి;

  ఒకవేళ (sync_socket_write_సందేశం (fd, verbosity, errno_, సందేశం) < 0)
    log_write_గమ్యం_stderr (errno_, సందేశం, verbosity, arg);
}

స్థిర bool
is_జ్ఞాపకం_హద్దు_too_దిగువ (runtime_spec_schema_config_schema *def)
{
  స్థిరరాశి దీర్ఘ జ్ఞాపకం_హద్దు_too_దిగువ = 1024 * 1024;

  ఒకవేళ (def->linux == ఖాళీ || def->linux->resources == ఖాళీ)
    తిరిగి false;

  ఒకవేళ (def->linux->resources->జ్ఞాపకం
      && def->linux->resources->జ్ఞాపకం->హద్దు_present
      && def->linux->resources->జ్ఞాపకం->హద్దు < జ్ఞాపకం_హద్దు_too_దిగువ)
    తిరిగి true;

  ఒకవేళ (def->linux->resources->unified)
    {
      size_t i;

      కొరకు (i = 0; i < def->linux->resources->unified->len; i++)
        ఒకవేళ (strcmp (def->linux->resources->unified->కీలకాలు[i], "memory.max") == 0)
          {
            దీర్ఘ హద్దు;

            errno = 0;
            హద్దు = strtol (def->linux->resources->unified->విలువలు[i], ఖాళీ, 10);
            ఒకవేళ (errno != 0)
              తిరిగి false;
            ఒకవేళ (హద్దు < జ్ఞాపకం_హద్దు_too_దిగువ)
              తిరిగి true;
          }
    }

  తిరిగి false;
}

స్థిర పూర్ణాంకం
sync_socket_wait_sync (libcrun_context_t *context, పూర్ణాంకం fd, bool flush, libcrun_error_t *err)
{
  నిర్మాణం sync_socket_సందేశం_s సందేశం;

  ఒకవేళ (fd < 0)
    తిరిగి 0;

  అయితే (true)
    {
      పూర్ణాంకం ret;

      ret = తాత్కాలిక_FAILURE_RETRY (పఠ (fd, &సందేశం, పరిమాణం (సందేశం)));
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          ఒకవేళ (flush)
            తిరిగి 0;
          తిరిగి crun_make_error (err, errno, "read from sync socket");
        }

      ఒకవేళ (ret == 0)
        {
          ఒకవేళ (flush)
            తిరిగి 0;

          తిరిగి crun_make_error (err, 0, "read from the init process");
        }

      ఒకవేళ (! flush && సందేశం.type == SYNC_SOCKET_SYNC_సందేశం)
        తిరిగి 0;
      లేకపోతే ఒకవేళ (సందేశం.type == SYNC_SOCKET_DEBUG_సందేశం)
        {
          ఒకవేళ (context)
            context->output_handler (సందేశం.error_విలువ, సందేశం.సందేశం_2, LIBCRUN_VERBOSITY_DEBUG, context->output_handler_arg);
          కొనసాగు;
        }
      లేకపోతే ఒకవేళ (సందేశం.type == SYNC_SOCKET_WARNING_సందేశం)
        {
          ఒకవేళ (context)
            context->output_handler (సందేశం.error_విలువ, సందేశం.సందేశం_2, LIBCRUN_VERBOSITY_WARNING, context->output_handler_arg);
          కొనసాగు;
        }
      లేకపోతే ఒకవేళ (సందేశం.type == SYNC_SOCKET_ERROR_సందేశం)
        తిరిగి crun_make_error (err, సందేశం.error_విలువ, "%s", సందేశం.సందేశం_2);
    }
}

స్థిర పూర్ణాంకం
sync_socket_send_sync (పూర్ణాంకం fd, bool flush_errors, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  నిర్మాణం sync_socket_సందేశం_s సందేశం = {
    0,
  };
  సందేశం.type = SYNC_SOCKET_SYNC_సందేశం;

  ఒకవేళ (fd < 0)
    తిరిగి 0;

  ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (fd, &సందేశం, SYNC_SOCKET_సందేశం_LEN (సందేశం, 0)));
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ఒకవేళ (flush_errors)
        {
          పూర్ణాంకం saved_errno = errno;
          ret = తాత్కాలిక_FAILURE_RETRY (పఠ (fd, &సందేశం, పరిమాణం (సందేశం)));
          ఒకవేళ (ret >= 0 && సందేశం.type == SYNC_SOCKET_ERROR_సందేశం)
            తిరిగి crun_make_error (err, సందేశం.error_విలువ, "%s", సందేశం.సందేశం_2);
          errno = saved_errno;
        }
      తిరిగి crun_make_error (err, errno, "write to sync socket");
    }

  తిరిగి 0;
}

స్థిర bool
has_cap_sys_ptrace ()
{
#నిర్వచితమైతే HAVE_CAP
  నిర్మాణం __user_cap_header_struct hdr = { LINUX_CAPABILITY_సంస్కరణ_3, 0 };
  నిర్మాణం __user_cap_data_struct దత్తాంశం[2] = { { 0 } };

  ఒకవేళ (UNLIKELY (capget (&hdr, దత్తాంశం) < 0))
    తిరిగి true;

  తిరిగి (దత్తాంశం[0].effective & (1U << CAP_SYS_PTRACE)) != 0;
#లేకపోతే
  తిరిగి geteuid () == 0;
#అయితే_ముగింపు
}

స్థిర libcrun_container_t *
make_container (runtime_spec_schema_config_schema *container_def, స్థిరరాశి అక్షరం *మార్గం, స్థిరరాశి అక్షరం *config)
{
  libcrun_container_t *container = xmalloc0 (పరిమాణం (*container));
  container->container_def = container_def;

  container->host_uid = geteuid ();
  container->host_gid = getegid ();
  container->host_has_cap_sys_ptrace = has_cap_sys_ptrace ();
  container->proc_fd = -1;

  container->annotations = make_string_map_from_json (container_def->annotations);

  /* An empty namespace path means that a new namespace is to be created,
     the same as if it is not specified.  */
  ఒకవేళ (container_def->linux)
    {
      size_t i;

      కొరకు (i = 0; i < container_def->linux->namespaces_len; i++)
        ఒకవేళ (container_def->linux->namespaces[i]->మార్గం && container_def->linux->namespaces[i]->మార్గం[0] == '\0')
          {
            విడుదల (container_def->linux->namespaces[i]->మార్గం);
            container_def->linux->namespaces[i]->మార్గం = ఖాళీ;
          }
    }

  ఒకవేళ (మార్గం)
    container->config_file = xstrdup (మార్గం);
  ఒకవేళ (config)
    container->config_file_content = xstrdup (config);

  తిరిగి container;
}

libcrun_container_t *
libcrun_container_ఎక్కించు_from_జ్ఞాపకం (స్థిరరాశి అక్షరం *json, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free అక్షరం *oci_error = ఖాళీ;
  container_def = runtime_spec_schema_config_schema_parse_దత్తాంశం (json, ఖాళీ, &oci_error);
  ఒకవేళ (container_def == ఖాళీ)
    {
      crun_make_error (err, 0, "load: `%s`", oci_error);
      తిరిగి ఖాళీ;
    }
  తిరిగి make_container (container_def, ఖాళీ, json);
}

libcrun_container_t *
libcrun_container_ఎక్కించు_from_file (స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free అక్షరం *oci_error = ఖాళీ;
  cleanup_free అక్షరం *content = ఖాళీ;
  size_t len;
  పూర్ణాంకం ret;

  libcrun_debug ("Loading container from config file: `%s`", మార్గం);
  ret = read_all_file (మార్గం, &content, &len, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ఖాళీ;

  container_def = runtime_spec_schema_config_schema_parse_దత్తాంశం (content, ఖాళీ, &oci_error);
  ఒకవేళ (container_def == ఖాళీ)
    {
      crun_make_error (err, 0, "load `%s`: %s", మార్గం, oci_error);
      తిరిగి ఖాళీ;
    }
  తిరిగి make_container (container_def, ఖాళీ, content);
}

స్థిరరాశి అక్షరం *
libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err)
{
  నిర్మాణం parser_context ctx = { 0, stderr };
  parser_error gen_err = ఖాళీ;

  ఒకవేళ (container == ఖాళీ)
    {
      crun_make_error (err, EINVAL, "no container specified");
      తిరిగి ఖాళీ;
    }

  ఒకవేళ (container->config_file_content == ఖాళీ && container->container_def)
    {
      container->config_file_content = runtime_spec_schema_config_schema_generate_json (container->container_def, &ctx, &gen_err);
      ఒకవేళ (container->config_file_content == ఖాళీ)
        {
          crun_make_error (err, 0, "cannot generate the configuration JSON: %s", gen_err ?: "unknown error");
          విడుదల (gen_err);
          తిరిగి ఖాళీ;
        }
      విడుదల (gen_err);
    }

  ఒకవేళ (container->config_file_content == ఖాళీ)
    {
      crun_make_error (err, 0, "the container has no configuration");
      తిరిగి ఖాళీ;
    }

  తిరిగి container->config_file_content;
}

స్థిరరాశి అక్షరం *
libcrun_container_get_annotation (libcrun_container_t *container, స్థిరరాశి అక్షరం *కీలకం)
{
  ఒకవేళ (container == ఖాళీ)
    తిరిగి ఖాళీ;
  తిరిగి కనుగొను_string_map_విలువ (container->annotations, కీలకం);
}

size_t
libcrun_container_get_annotations_len (libcrun_container_t *container)
{
  ఒకవేళ (container == ఖాళీ || container->annotations == ఖాళీ)
    తిరిగి 0;
  తిరిగి string_map_కొలత (container->annotations);
}

పూర్ణాంకం
libcrun_container_get_annotation_at (libcrun_container_t *container, size_t సూచీ, స్థిరరాశి అక్షరం **కీలకం,
                                     స్థిరరాశి అక్షరం **విలువ)
{
  ఒకవేళ (container == ఖాళీ)
    {
      errno = ERANGE;
      తిరిగి -1;
    }
  తిరిగి string_map_get_at (container->annotations, సూచీ, కీలకం, విలువ);
}

uid_t
libcrun_container_get_uid (libcrun_container_t *container)
{
  ఒకవేళ (container == ఖాళీ)
    తిరిగి (uid_t) -1;
  తిరిగి container->host_uid;
}

gid_t
libcrun_container_get_gid (libcrun_container_t *container)
{
  ఒకవేళ (container == ఖాళీ)
    తిరిగి (gid_t) -1;
  తిరిగి container->host_gid;
}

శూన్య
libcrun_container_free (libcrun_container_t *ctr)
{
  ఒకవేళ (ctr == ఖాళీ)
    తిరిగి;

  ఒకవేళ (ctr->cleanup_వ్యక్తిగత_దత్తాంశం)
    ctr->cleanup_వ్యక్తిగత_దత్తాంశం (ctr->వ్యక్తిగత_దత్తాంశం);

  ఒకవేళ (ctr->container_def)
    free_runtime_spec_schema_config_schema (ctr->container_def);

  free_string_map (ctr->annotations);

  ఒకవేళ (ctr->proc_fd >= 0)
    close (ctr->proc_fd);

  విడుదల (ctr->config_file_content);
  విడుదల (ctr->config_file);
  విడుదల (ctr);
}

libcrun_context_t *
libcrun_context_new (స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *state_మూలం, libcrun_error_t *err)
{
  libcrun_context_t *ctx = xmalloc0 (పరిమాణం (*ctx));

  /* Go through the setters so that the owned_* copies and the borrowed
     pointers stay in sync and are released by libcrun_context_free.  */
  libcrun_context_సమితి_id (ctx, id);
  libcrun_context_సమితి_state_మూలం (ctx, state_మూలం);
  libcrun_context_సమితి_bundle (ctx, ".");
  ctx->fifo_exec_wait_fd = -1;

  ఒకవేళ (libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, id, ఖాళీ, err) < 0)
    {
      libcrun_context_free (ctx);
      తిరిగి ఖాళీ;
    }

  ctx->handler_manager = libcrun_handler_manager_create (err);
  ఒకవేళ (ctx->handler_manager == ఖాళీ)
    {
      libcrun_context_free (ctx);
      తిరిగి ఖాళీ;
    }

#నిర్వచితమైతే CRUN_LIBDIR
  ఒకవేళ (access (CRUN_LIBDIR "/handlers", F_OK) == 0)
    {
      ఒకవేళ (libcrun_handler_manager_ఎక్కించు_directory (ctx->handler_manager, CRUN_LIBDIR "/handlers", err) < 0)
        {
          libcrun_context_free (ctx);
          తిరిగి ఖాళీ;
        }
    }
#అయితే_ముగింపు

  తిరిగి ctx;
}

శూన్య
libcrun_context_free (libcrun_context_t *ctx)
{
  ఒకవేళ (ctx == ఖాళీ)
    తిరిగి;

  విడుదల (ctx->owned_state_మూలం);
  విడుదల (ctx->owned_id);
  విడుదల (ctx->owned_bundle);
  విడుదల (ctx->owned_console_socket);
  విడుదల (ctx->owned_pid_file);
  విడుదల (ctx->owned_notify_socket);
  విడుదల (ctx->owned_handler);

  ఒకవేళ (ctx->handler_manager)
    handler_manager_free (ctx->handler_manager);

  విడుదల (ctx);
}

#నిర్వచించు LIBCRUN_CONTEXT_సమితి_STRING(క్షేత్రం)                  \
  చేయి                                                       \
    {                                                      \
      విడుదల (ctx->owned_##field);                           \
      ctx->owned_##field = విలువ ? xstrdup (విలువ) : ఖాళీ; \
      ctx->క్షేత్రం = ctx->owned_##field;                     \
  } అయితే (0)

శూన్య
libcrun_context_సమితి_id (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *విలువ)
{
  LIBCRUN_CONTEXT_సమితి_STRING (id);
}

శూన్య
libcrun_context_సమితి_state_మూలం (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *విలువ)
{
  LIBCRUN_CONTEXT_సమితి_STRING (state_మూలం);
}

శూన్య
libcrun_context_సమితి_bundle (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *విలువ)
{
  LIBCRUN_CONTEXT_సమితి_STRING (bundle);
}

శూన్య
libcrun_context_సమితి_console_socket (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *విలువ)
{
  LIBCRUN_CONTEXT_సమితి_STRING (console_socket);
}

శూన్య
libcrun_context_సమితి_pid_file (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *విలువ)
{
  LIBCRUN_CONTEXT_సమితి_STRING (pid_file);
}

శూన్య
libcrun_context_సమితి_notify_socket (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *విలువ)
{
  LIBCRUN_CONTEXT_సమితి_STRING (notify_socket);
}

శూన్య
libcrun_context_సమితి_handler (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *విలువ)
{
  LIBCRUN_CONTEXT_సమితి_STRING (handler);
}

#నిర్వచనం_రద్దు LIBCRUN_CONTEXT_సమితి_STRING

స్థిరరాశి అక్షరం *
libcrun_context_get_pid_file (libcrun_context_t *ctx)
{
  తిరిగి ctx->pid_file;
}

స్థిరరాశి అక్షరం *
libcrun_context_get_state_మూలం (libcrun_context_t *ctx)
{
  ఒకవేళ (ctx == ఖాళీ)
    తిరిగి ఖాళీ;
  తిరిగి ctx->state_మూలం;
}

శూన్య
libcrun_context_సమితి_preserve_fds (libcrun_context_t *ctx, పూర్ణాంకం preserve_fds)
{
  ctx->preserve_fds = preserve_fds;
}

పూర్ణాంకం
libcrun_context_get_preserve_fds (libcrun_context_t *ctx)
{
  తిరిగి ctx->preserve_fds;
}

శూన్య
libcrun_context_సమితి_listen_fds (libcrun_context_t *ctx, పూర్ణాంకం listen_fds)
{
  ctx->listen_fds = listen_fds;
}

శూన్య
libcrun_context_సమితి_వాదనలు (libcrun_context_t *ctx, పూర్ణాంకం argc, అక్షరం **argv)
{
  ctx->argc = argc;
  ctx->argv = argv;
}

శూన్య
libcrun_context_సమితి_systemd_cgroup (libcrun_context_t *ctx, bool విలువ)
{
  ctx->systemd_cgroup = విలువ;
}

శూన్య
libcrun_context_సమితి_detach (libcrun_context_t *ctx, bool విలువ)
{
  ctx->detach = విలువ;
}

శూన్య
libcrun_context_సమితి_no_new_keyring (libcrun_context_t *ctx, bool విలువ)
{
  ctx->no_new_keyring = విలువ;
}

శూన్య
libcrun_context_సమితి_no_ఇరుసు (libcrun_context_t *ctx, bool విలువ)
{
  ctx->no_ఇరుసు = విలువ;
}

శూన్య
libcrun_context_సమితి_force_no_cgroup (libcrun_context_t *ctx, bool విలువ)
{
  ctx->force_no_cgroup = విలువ;
}

శూన్య
libcrun_context_సమితి_output_handler (libcrun_context_t *ctx, crun_output_handler handler, శూన్య *arg)
{
  ctx->output_handler = handler;
  ctx->output_handler_arg = arg;
}

పూర్ణాంకం
libcrun_context_init_logging (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *సంవర్గమానం, libcrun_error_t *err)
{
  తిరిగి libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, ctx->id, సంవర్గమానం, err);
}

స్థిర పూర్ణాంకం
ఖండం_సంకేతాలు (libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  sigset_t ముసుగు;
  sigfillset (&ముసుగు);
  ret = sigprocmask (SIG_ఖండం, &ముసుగు, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "sigprocmask");
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
unblock_సంకేతాలు (libcrun_error_t *err)
{
  పూర్ణాంకం i;
  పూర్ణాంకం ret;
  sigset_t ముసుగు;
  నిర్మాణం sigaction act = {};

  sigfillset (&ముసుగు);
  ret = sigprocmask (SIG_UNBLOCK, &ముసుగు, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "sigprocmask");

  act.sa_handler = SIG_DFL;
  కొరకు (i = 0; i < NSIG; i++)
    {
      ret = sigaction (i, &act, ఖాళీ);
      ఒకవేళ (ret < 0 && errno != EINVAL)
        తిరిగి crun_make_error (err, errno, "sigaction");
    }

  తిరిగి 0;
}

/* must be used on the host before pivot_root(2).  */
స్థిర పూర్ణాంకం
initialize_security (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ret = libcrun_init_caps (container, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (UNLIKELY (proc == ఖాళీ))
    తిరిగి 0;

  ఒకవేళ (proc->apparmor_profile)
    {
      ret = libcrun_initialize_apparmor (err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ret = libcrun_initialize_selinux (container, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి 0;
}

/* Emit the "annotations" map.  Returns a json_gen_status; callers can wrap
   the call with GEN_OR_FAIL.  */
స్థిర పూర్ణాంకం
gen_annotations (json_gen_ctx *gen, json_map_string_string *annotations)
{
  size_t i;
  పూర్ణాంకం r;

  GEN_కీలకం (gen, "annotations");
  GEN_OR_FAIL (json_gen_map_open (gen));

  కొరకు (i = 0; i < annotations->len; i++)
    {
      /* An annotation whose value is null in the document is parsed into a
         NULL, which cannot be emitted as a JSON string; leave it out.  */
      ఒకవేళ (annotations->కీలకాలు[i] == ఖాళీ || annotations->విలువలు[i] == ఖాళీ)
        కొనసాగు;

      GEN_STR (gen, annotations->కీలకాలు[i]);
      GEN_STR (gen, annotations->విలువలు[i]);
    }

  తిరిగి json_gen_map_close (gen);

gen_error:
  తిరిగి r;
}

/* Return the bundle path to report in the container state.  crun always
   chdir()s into the bundle, so fall back to getcwd() when the caller has no
   absolute bundle path, as can happen when libcrun is used as a library.
   *ALLOCATED must be freed by the caller.  */
స్థిర స్థిరరాశి అక్షరం *
get_bundle_మార్గం (స్థిరరాశి అక్షరం *bundle, అక్షరం **allocated)
{
  ఒకవేళ (bundle != ఖాళీ && bundle[0] == '/')
    తిరిగి bundle;

  *allocated = getcwd (ఖాళీ, 0);
  ఒకవేళ (*allocated == ఖాళీ)
    OOM ();
  తిరిగి *allocated;
}

స్థిర పూర్ణాంకం
do_hooks (runtime_spec_schema_config_schema *def, pid_t pid, స్థిరరాశి అక్షరం *id, bool keep_going, స్థిరరాశి అక్షరం *bundle,
          స్థిరరాశి అక్షరం *cwd, స్థిరరాశి అక్షరం *స్థితి_2, స్థిరరాశి అక్షరం *name, hook **hooks, size_t hooks_len, పూర్ణాంకం బహిర్గతం_fd,
          పూర్ణాంకం err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  size_t i, stdin_len;
  పూర్ణాంకం r, ret;
  అక్షరం *stdin = ఖాళీ;
  cleanup_free అక్షరం *bundle_allocated = ఖాళీ;
  స్థిరరాశి అక్షరం *rootfs = def->మూలం ? def->మూలం->మార్గం : "";
  cleanup_json_gen json_gen_ctx *gen = ఖాళీ;

  bundle = get_bundle_మార్గం (bundle, &bundle_allocated);

  ఒకవేళ (! json_gen_init (&gen, ఖాళీ))
    తిరిగి crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 0);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_కీలకం (gen, "ociVersion");
  GEN_STR (gen, "1.0");

  GEN_కీలకం (gen, "id");
  GEN_STR (gen, id);

  GEN_కీలకం (gen, "pid");
  GEN_OR_FAIL (map_int (gen, pid));

  GEN_కీలకం (gen, "root");
  GEN_STR (gen, rootfs);

  GEN_కీలకం (gen, "bundle");
  GEN_STR (gen, bundle);

  GEN_కీలకం (gen, "status");
  GEN_STR (gen, స్థితి_2);

  ఒకవేళ (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, (స్థిరరాశి అక్షరం **) &stdin, &stdin_len));

  ret = 0;

  bool error_created = false;
  కొరకు (i = 0; i < hooks_len; i++)
    {
      అక్షరం **env = environ;

      ఒకవేళ (hooks[i]->env)
        env = hooks[i]->env;

      /* Release the error from the previous iteration, if any.  */
      ఒకవేళ (error_created)
        {
          crun_error_release (err);
          error_created = false;
        }

      ret = నిరంతరం_process_with_stdin_timeout_envp (hooks[i]->మార్గం, hooks[i]->వాదనలు, cwd, hooks[i]->timeout, env,
                                                 stdin, stdin_len, బహిర్గతం_fd, err_fd, can_ignore_chdir_errors, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        error_created = true;

      ఒకవేళ (UNLIKELY (ret != 0))
        {
          ఒకవేళ (keep_going)
            libcrun_warning ("error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->మార్గం, ret);
          లేకపోతే
            {
              libcrun_error (0, "error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->మార్గం, ret);
              ఆపు;
            }
        }
    }

  తిరిగి ret;

gen_error:
  తిరిగి json_gen_error_గమ్యం_crun_error (r, err);
}

స్థిర పూర్ణాంకం
get_json_gen_ఫలితం (json_gen_ctx *gen, అక్షరం **బహిర్గతం, size_t *బహిర్గతం_len)
{
  స్థిరరాశి అక్షరం *buf = ఖాళీ;
  size_t buf_len = 0;
  పూర్ణాంకం r;

  r = json_gen_get_buf (gen, &buf, &buf_len);
  ఒకవేళ (UNLIKELY (r != json_gen_స్థితి_ok))
    తిరిగి r;

  *బహిర్గతం_len = buf_len;

  *బహిర్గతం = xmalloc (buf_len + 1);
  జ్ఞాపకం_నకలు (*బహిర్గతం, buf, buf_len);
  (*బహిర్గతం)[buf_len] = '\0';

  తిరిగి json_gen_స్థితి_ok;
}

స్థిర పూర్ణాంకం
get_seccomp_receiver_fd_payload (libcrun_container_t *container, స్థిరరాశి అక్షరం *స్థితి_2, pid_t own_pid,
                                 అక్షరం **seccomp_fd_payload, size_t *seccomp_fd_payload_len, libcrun_error_t *err)
{
  పూర్ణాంకం r;
  json_gen_ctx *gen = ఖాళీ;
  runtime_spec_schema_config_schema *def = container->container_def;
  స్థిరరాశి అక్షరం *స్థిరరాశి OCI_సంస్కరణ = "0.2.0";

  ఒకవేళ (! json_gen_init (&gen, ఖాళీ))
    తిరిగి crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_కీలకం (gen, "ociVersion");
  GEN_STR (gen, OCI_సంస్కరణ);

  GEN_కీలకం (gen, "fds");
  GEN_OR_FAIL (json_gen_array_open (gen));

  GEN_కీలకం (gen, "seccompFd");
  GEN_OR_FAIL (json_gen_array_close (gen));

  GEN_కీలకం (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  ఒకవేళ (def && def->linux && def->linux->seccomp)
    {
      స్థిరరాశి అక్షరం *metadata = def->linux->seccomp->listener_metadata;

      ఒకవేళ (metadata)
        {
          GEN_కీలకం (gen, "metadata");
          GEN_STR (gen, metadata);
        }
    }

  /* State.  */
  GEN_కీలకం (gen, "state");
  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_కీలకం (gen, "ociVersion");
  GEN_STR (gen, OCI_సంస్కరణ);

  ఒకవేళ (container->context && container->context->id)
    {
      GEN_కీలకం (gen, "id");
      GEN_STR (gen, container->context->id);
    }

  GEN_కీలకం (gen, "status");
  GEN_STR (gen, స్థితి_2);

  GEN_కీలకం (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  ఒకవేళ (container->context && container->context->bundle)
    {
      GEN_కీలకం (gen, "bundle");
      GEN_STR (gen, container->context->bundle);
    }

  ఒకవేళ (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));
  /* End state.  */

  GEN_OR_FAIL (json_gen_map_close (gen));

  r = get_json_gen_ఫలితం (gen, seccomp_fd_payload, seccomp_fd_payload_len);

gen_error:
  json_gen_free (gen);

  తిరిగి json_gen_error_గమ్యం_crun_error (r, err);
}

స్థిర పూర్ణాంకం
send_sync_cb (శూన్య *దత్తాంశం, libcrun_error_t *err)
{
  పూర్ణాంకం sync_socket_fd = *((పూర్ణాంకం *) దత్తాంశం);
  పూర్ణాంకం ret;

  /* sync 2.  */
  ret = sync_socket_send_sync (sync_socket_fd, false, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  /* sync 3.  */
  తిరిగి sync_socket_wait_sync (ఖాళీ, sync_socket_fd, false, err);
}

స్థిర పూర్ణాంకం
maybe_chown_std_streams (uid_t container_uid, gid_t container_gid, libcrun_error_t *err)
{
  పూర్ణాంకం ret, i;

  కొరకు (i = 0; i < 3; i++)
    {
      ఒకవేళ (! isatty (i))
        {
          నిర్మాణం stat statbuf;
          ret = fstat (i, &statbuf);
          ఒకవేళ (UNLIKELY (ret < 0))
            {
              ఒకవేళ (errno == EBADF)
                కొనసాగు;
              తిరిగి crun_make_error (err, errno, "fstat fd `%d`", i);
            }

          /* Skip chown for device files */
          ఒకవేళ (S_ISCHR (statbuf.st_mode) || S_ISBLK (statbuf.st_mode))
            కొనసాగు;

          ret = fchown (i, container_uid, container_gid);
          ఒకవేళ (UNLIKELY (ret < 0))
            {
              /* EINVAL means the user is not mapped in the current userns.
                 Ignore EPERM and EROFS as well as there is no reason to fail
                 so early, and let the container payload deal with it.
                 EBADF means fd is closed.
              */
              ఒకవేళ (errno == EINVAL || errno == EPERM || errno == EROFS || errno == EBADF)
                కొనసాగు;

              తిరిగి crun_make_error (err, errno, "fchown std stream `%i`", i);
            }
        }
    }
  తిరిగి 0;
}

పూర్ణాంకం
libcrun_container_notify_handler (నిర్మాణం container_entrypoint_s *వాదనలు,
                                  గణన handler_configure_phase phase,
                                  libcrun_container_t *container, స్థిరరాశి అక్షరం *rootfs,
                                  libcrun_error_t *err)
{
  నిర్మాణం custom_handler_s *h;

  ఒకవేళ (వాదనలు->custom_handler == ఖాళీ)
    తిరిగి 0;

  h = వాదనలు->custom_handler->vtable;
  ఒకవేళ (h == ఖాళీ || h->configure_container == ఖాళీ)
    తిరిగి 0;

  తిరిగి h->configure_container (వాదనలు->custom_handler->cookie, phase,
                                 వాదనలు->context, container,
                                 rootfs, err);
}

/* Resolve and normalize the container rootfs path.  */
స్థిర పూర్ణాంకం
resolve_rootfs_మార్గం (libcrun_container_t *container, అక్షరం **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ఒకవేళ (def->మూలం && def->మూలం->మార్గం)
    {
      *rootfs = realpath (def->మూలం->మార్గం, ఖాళీ);
      ఒకవేళ (UNLIKELY (*rootfs == ఖాళీ))
        {
          పూర్ణాంకం saved_errno = errno;

          /* If realpath failed for any reason, try the relative directory.  */
          ఒకవేళ (def->మూలం->మార్గం[0] == '/')
            {
              cleanup_free అక్షరం *cwd = ఖాళీ;
              libcrun_error_t tmp_err = ఖాళీ;
              ssize_t len;
              పూర్ణాంకం proc_fd;

              proc_fd = libcrun_get_cached_proc_fd (container, &tmp_err);
              ఒకవేళ (LIKELY (proc_fd >= 0))
                {
                  len = సురక్షిత_readlinkat (proc_fd, "self/cwd", &cwd, 0, &tmp_err);
                  ఒకవేళ (LIKELY (len > 0))
                    {
                      /* If the rootfs is under the current working directory, just use its relative path.  */
                      ఒకవేళ (has_dir_ఉపసర్గ (def->మూలం->మార్గం, cwd))
                        {
                          స్థిరరాశి అక్షరం *it = consume_slashes (def->మూలం->మార్గం + len);
                          ఒకవేళ (LIKELY (*it))
                            {
                              పూర్ణాంకం ret = faccessat (AT_FDCWD, it, F_OK, 0);
                              ఒకవేళ (LIKELY (ret == 0))
                                *rootfs = xstrdup (it);
                            }
                        }
                    }
                }
              crun_error_release (&tmp_err);
            }

          ఒకవేళ (*rootfs == ఖాళీ)
            {
              ఒకవేళ (saved_errno == ENOENT)
                తిరిగి crun_make_error (err, ENOENT, "the rootfs path `%s` does not exist", def->మూలం->మార్గం);

              *rootfs = xstrdup (def->మూలం->మార్గం);
            }
        }
    }
  తిరిగి 0;
}

/* Initialize the environment variables.  */
స్థిర పూర్ణాంకం
అమరిక_environment (runtime_spec_schema_config_schema *def, uid_t container_uid, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ఒకవేళ (clearenv ())
    తిరిగి crun_make_error (err, errno, "clearenv");

  ఒకవేళ (def->process)
    {
      size_t i;

      కొరకు (i = 0; i < def->process->env_len; i++)
        {
          /* A null element of the array of variables is parsed into a NULL,
             which putenv(3) cannot be handed.  */
          ఒకవేళ (def->process->env[i] == ఖాళీ)
            తిరిగి crun_make_error (err, EINVAL, "environment variable is not specified");

          ఒకవేళ (putenv (def->process->env[i]) < 0)
            తిరిగి crun_make_error (err, errno, "putenv `%s`", def->process->env[i]);
        }
    }

  ఒకవేళ (is_ఖాళీలు_string (getenv ("HOME")))
    {
      ret = సమితి_home_env (container_uid);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          ఒకవేళ (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            తిరిగి crun_make_error (err, errno, "setenv HOME");
        }
    }

  తిరిగి 0;
}

/* Configure terminal and console for interactive containers.  */
స్థిర పూర్ణాంకం
అమరిక_terminal (నిర్మాణం container_entrypoint_s *entrypoint_వాదనలు, libcrun_container_t *container,
                పూర్ణాంకం console_socket, పూర్ణాంకం console_socketpair, పూర్ణాంకం has_terminal, libcrun_error_t *err)
{
  cleanup_close పూర్ణాంకం terminal_fd = -1;
  పూర్ణాంకం ret;

  ఒకవేళ (! has_terminal)
    తిరిగి 0;

  fflush (stderr);

  ఒకవేళ (console_socket >= 0 || (entrypoint_వాదనలు->has_terminal_socket_pair && console_socketpair >= 0))
    {
      terminal_fd = libcrun_సమితి_terminal (container, err);
      ఒకవేళ (UNLIKELY (terminal_fd < 0))
        తిరిగి terminal_fd;
    }

  ఒకవేళ (console_socket >= 0)
    {
      ret = send_fd_గమ్యం_socket (console_socket, terminal_fd, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  లేకపోతే ఒకవేళ (entrypoint_వాదనలు->has_terminal_socket_pair && console_socketpair >= 0)
    {
      ret = send_fd_గమ్యం_socket (console_socketpair, terminal_fd, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  తిరిగి 0;
}

/* Resolve and validate the container's executable path.  */
స్థిర పూర్ణాంకం
అమరిక_executable_మార్గం (నిర్మాణం container_entrypoint_s *entrypoint_వాదనలు, runtime_spec_schema_config_schema *def,
                       అక్షరం **exec_మార్గం, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ఒకవేళ (def->process && def->process->వాదనలు)
    {
      ret = కనుగొను_executable (exec_మార్గం, def->process->వాదనలు[0], def->process->cwd, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          ఒకవేళ (entrypoint_వాదనలు->custom_handler == ఖాళీ && crun_error_get_errno (err) == ENOENT)
            తిరిగి ret;

          /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
             once the process switched to the use that runs in the container.  This might be necessary
             when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
             is not honored.  */
          crun_error_release (err);
        }
    }

  తిరిగి 0;
}

/* Apply security settings for the container process.  */
స్థిర పూర్ణాంకం
apply_security_settings (నిర్మాణం container_entrypoint_s *entrypoint_వాదనలు, runtime_spec_schema_config_schema *def,
                         libcrun_container_t *container, pid_t own_pid, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  runtime_spec_schema_config_schema_process_capabilities *capabilities;
  పూర్ణాంకం no_new_privs;

  ఒకవేళ (def->process && ! def->process->no_new_privileges)
    {
      అక్షరం **seccomp_flags = ఖాళీ;
      size_t seccomp_flags_len = 0;
      cleanup_free అక్షరం *seccomp_fd_payload = ఖాళీ;
      size_t seccomp_fd_payload_len = 0;

      ఒకవేళ (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      ఒకవేళ (entrypoint_వాదనలు->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_వాదనలు->seccomp_fd, entrypoint_వాదనలు->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      close_and_reset (&entrypoint_వాదనలు->seccomp_fd);
      close_and_reset (&entrypoint_వాదనలు->seccomp_receiver_fd);
    }

  capabilities = def->process ? def->process->capabilities : ఖాళీ;
  no_new_privs = def->process ? def->process->no_new_privileges : 1;
  ret = libcrun_సమితి_caps (capabilities, container->container_uid, container->container_gid, no_new_privs,
                          entrypoint_వాదనలు->custom_handler != ఖాళీ, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి 0;
}

/* Create the process that joins the existing PID namespace now that the
   host mounts are not visible anymore, and continue the setup from there.  */
స్థిర పూర్ణాంకం
do_delayed_pidns_fork (పూర్ణాంకం sync_socket, pid_t *own_pid, libcrun_error_t *err)
{
  pid_t pid;
  పూర్ణాంకం ret;

  pid = fork ();
  ఒకవేళ (UNLIKELY (pid < 0))
    తిరిగి crun_make_error (err, errno, "fork");

  ఒకవేళ (pid)
    {
      /* Report the new PID to the parent and exit immediately.  */
      ret = sync_socket_send_sync (sync_socket, false, err);
      ఒకవేళ (LIKELY (ret == 0))
        {
          ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (sync_socket, &pid, పరిమాణం (pid)));
          ఒకవేళ (LIKELY (ret == పరిమాణం (pid)))
            సురక్షిత_exit (EXIT_SUCCESS);
        }

      kill (pid, SIGKILL);
      సురక్షిత_exit (EXIT_FAILURE);
    }

  /* sync receive own pid.  */
  ret = తాత్కాలిక_FAILURE_RETRY (పఠ (sync_socket, own_pid, పరిమాణం (*own_pid)));
  ఒకవేళ (UNLIKELY (ret != పరిమాణం (*own_pid)))
    {
      ఒకవేళ (ret >= 0)
        errno = 0;
      తిరిగి crun_make_error (err, errno, "read from sync socket");
    }

  తిరిగి 0;
}

నిర్మాణం delayed_pidns_fork_s
{
  libcrun_container_t *container;
  పూర్ణాంకం sync_socket;
  pid_t *own_pid;
};

స్థిర పూర్ణాంకం
delayed_pidns_fork_cb (శూన్య *దత్తాంశం, libcrun_error_t *err)
{
  నిర్మాణం delayed_pidns_fork_s *d = దత్తాంశం;

  ఒకవేళ (! libcrun_container_delayed_pidns_fork (d->container))
    తిరిగి 0;

  తిరిగి do_delayed_pidns_fork (d->sync_socket, d->own_pid, err);
}

స్థిర పూర్ణాంకం
container_init_అమరిక (శూన్య *వాదనలు, pid_t *own_pid, అక్షరం *notify_socket,
                      పూర్ణాంకం sync_socket, అక్షరం **exec_మార్గం, libcrun_error_t *err)
{
  నిర్మాణం container_entrypoint_s *entrypoint_వాదనలు = వాదనలు;
  libcrun_container_t *container = entrypoint_వాదనలు->container;
  bool chdir_పూర్తి = false;
  పూర్ణాంకం ret;
  పూర్ణాంకం has_terminal;
  cleanup_close పూర్ణాంకం console_socket = -1;
  cleanup_close పూర్ణాంకం console_socketpair = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free అక్షరం *rootfs = ఖాళీ;
  నిర్మాణం delayed_pidns_fork_s delayed_pidns_fork = {
    .container = container,
    .sync_socket = sync_socket,
    .own_pid = own_pid,
  };

  ret = initialize_security (container, def->process, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_configure_జాలం (container, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = resolve_rootfs_మార్గం (container, &rootfs, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  /* Configure terminal socket pair for container communication.  */
  ఒకవేళ (entrypoint_వాదనలు->terminal_socketpair[0] >= 0)
    {
      close_and_reset (&entrypoint_వాదనలు->terminal_socketpair[0]);
      console_socketpair = entrypoint_వాదనలు->terminal_socketpair[1];
    }

  /* sync 1.  */
  ret = sync_socket_wait_sync (ఖాళీ, sync_socket, false, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  has_terminal = container->container_def->process && container->container_def->process->terminal;
  ఒకవేళ (has_terminal && entrypoint_వాదనలు->context->console_socket)
    console_socket = entrypoint_వాదనలు->console_socket_fd;

  ret = libcrun_సమితి_sysctl (container, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_container_notify_handler (entrypoint_వాదనలు, HANDLER_CONFIGURE_BEFORE_MOUNTS, container, rootfs, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_do_ఇరుసు_మూలం (container, entrypoint_వాదనలు->context->no_ఇరుసు, &rootfs, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  /* sync 2 and 3 are sent as part of libcrun_set_mounts.  */
  ret = libcrun_సమితి_mounts (entrypoint_వాదనలు, container, rootfs, send_sync_cb, &sync_socket, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  /* The createContainer hooks MUST run after the mounts are set up but before
     the pivot_root.  When these hooks are present can_use_open_tree_namespace()
     forces the deferred-pivot path, so the actual pivot happens later in
     libcrun_finalize_mounts().  */
  ఒకవేళ (def->hooks && def->hooks->create_container_len)
    {
      libcrun_error_t tmp_err = ఖాళీ;
      పూర్ణాంకం లోపల_userns = check_running_లోపల_user_namespace (&tmp_err);
      ఒకవేళ (tmp_err)
        crun_error_release (&tmp_err);
      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_వాదనలు->bundle, ఖాళీ, "created", "createContainer",
                      (hook **) def->hooks->create_container,
                      def->hooks->create_container_len, entrypoint_వాదనలు->hooks_బహిర్గతం_fd, entrypoint_వాదనలు->hooks_err_fd,
                      లోపల_userns > 0, err);
      ఒకవేళ (UNLIKELY (ret != 0))
        తిరిగి ret;
    }

  ret = libcrun_finalize_mounts (entrypoint_వాదనలు, container, rootfs, delayed_pidns_fork_cb, &delayed_pidns_fork, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (def->process)
    {
      ret = libcrun_సమితి_selinux_label (container, def->process, false, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = libcrun_సమితి_apparmor_profile (container, def->process, false, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ret = mark_or_close_fds_ge_than (container, entrypoint_వాదనలు->context->preserve_fds + 3, false, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ret = libcrun_reopen_dev_null (err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = maybe_chown_std_streams (container->container_uid, container->container_gid, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = అమరిక_environment (def, container->container_uid, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  /* Set primary process to 1 explicitly if nothing is configured and LISTEN_FD is not set.  */
  ఒకవేళ (entrypoint_వాదనలు->context->listen_fds > 0 && getenv ("LISTEN_PID") == ఖాళీ)
    {
      libcrun_warning ("setting LISTEN_PID=1 since no previous configuration was found");
      ఒకవేళ (UNLIKELY (setenv ("LISTEN_PID", "1", 1) < 0))
        తిరిగి crun_make_error (err, errno, "setenv LISTEN_PID");
    }

  /* Attempt to chdir immediately here, before doing the setresuid.  If we fail here, let's
     try again later once the process switched to the user that runs in the container.  */
  ఒకవేళ (def->process && def->process->cwd)
    {
      ret = libcrun_సురక్షిత_chdir (def->process->cwd, err);
      ఒకవేళ (LIKELY (ret == 0))
        chdir_పూర్తి = true;
      లేకపోతే
        crun_error_release (err);
    }

  ret = setsid ();
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "setsid");

  ret = అమరిక_terminal (entrypoint_వాదనలు, container, console_socket, console_socketpair, has_terminal, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  close_and_reset (&console_socket);
  close_and_reset (&console_socketpair);

  ret = అమరిక_executable_మార్గం (entrypoint_వాదనలు, def, exec_మార్గం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_సమితి_hostname (container, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_సమితి_domainname (container, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_సమితి_personality (container->container_def->linux->personality, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (def->process && def->process->user)
    umask (def->process->user->umask_present ? def->process->user->umask : 0022);

  ret = apply_security_settings (entrypoint_వాదనలు, def, container, *own_pid, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (UNLIKELY (def->process && def->process->వాదనలు && *exec_మార్గం == ఖాళీ))
    {
      ret = కనుగొను_executable (exec_మార్గం, def->process->వాదనలు[0], def->process->cwd, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          ఒకవేళ (entrypoint_వాదనలు->custom_handler == ఖాళీ || is_ఖాళీలు_string (def->process->వాదనలు[0]))
            తిరిగి ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          *exec_మార్గం = xstrdup (def->process->వాదనలు[0]);
        }
    }

  /* The chdir was not already performed, so try again now after switching to the UID/GID in the container.  */
  ఒకవేళ (! chdir_పూర్తి && def->process && def->process->cwd)
    {
      ret = libcrun_సురక్షిత_chdir (def->process->cwd, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (notify_socket && putenv (notify_socket) < 0)
    తిరిగి crun_make_error (err, errno, "putenv `%s`", notify_socket);

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
open_hooks_output (libcrun_container_t *container, పూర్ణాంకం *బహిర్గతం_fd, పూర్ణాంకం *err_fd, libcrun_error_t *err)
{
  స్థిరరాశి అక్షరం *annotation;

  *err_fd = *బహిర్గతం_fd = -1;

  libcrun_debug ("Opening hooks output");
  annotation = కనుగొను_annotation (container, "run.oci.hooks.stdout");
  ఒకవేళ (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stdout` annotation");
      *బహిర్గతం_fd = తాత్కాలిక_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      ఒకవేళ (UNLIKELY (*బహిర్గతం_fd < 0))
        తిరిగి crun_make_error (err, errno, "open `%s`", annotation);
    }

  annotation = కనుగొను_annotation (container, "run.oci.hooks.stderr");
  ఒకవేళ (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stderr` annotation");
      *err_fd = తాత్కాలిక_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      ఒకవేళ (UNLIKELY (*err_fd < 0))
        తిరిగి crun_make_error (err, errno, "open `%s`", annotation);
    }

  తిరిగి 0;
}

/* The environment the process was started with lives on the initial stack, right after the
   argv strings and before the string AT_EXECFN points to.  clearenv(3) only drops the pointers
   to it, so when there is no execv to replace the stack, erase it or the container process can
   still read it, e.g. through /proc/self/environ.  */
స్థిర శూన్య
erase_initial_environment (అక్షరం **argv, పూర్ణాంకం argc)
{
  అక్షరం *ప్రారంభం = argv[argc - 1] + అక్షరశ్రేణి_పొడవు (argv[argc - 1]) + 1;
  అక్షరం *చివర = (అక్షరం *) getauxval (AT_EXECFN);

  ఒకవేళ (చివర > ప్రారంభం)
    memset (ప్రారంభం, 0, చివర - ప్రారంభం);
}

స్థిర శూన్య
rewrite_argv (అక్షరం **argv, పూర్ణాంకం argc, స్థిరరాశి అక్షరం *name, అక్షరం **వాదనలు, size_t వాదనలు_len)
{
  cleanup_free అక్షరం *decorated_name = ఖాళీ;
  size_t అందుబాటు_len = argv[argc - 1] - argv[0] + అక్షరశ్రేణి_పొడవు (argv[argc - 1]) + 1;
  cleanup_free అక్షరం *new_argv = ఖాళీ;
  size_t i, so_far = 0, needed = 0;

  needed = xasprintf (&decorated_name, "[libcrun:%s]", name) + 1;
  కొరకు (i = 0; i < వాదనలు_len; i++)
    needed += అక్షరశ్రేణి_పొడవు (వాదనలు[i]) + 1;

  new_argv = xmalloc0 (needed + 1);
  strcpy (new_argv, decorated_name);
  so_far = అక్షరశ్రేణి_పొడవు (decorated_name) + 1;

  ఒకవేళ (అందుబాటు_len >= needed)
    {
      కొరకు (i = 0; i < వాదనలు_len; i++)
        {
          strcpy (new_argv + so_far, వాదనలు[i]);
          so_far += అక్షరశ్రేణి_పొడవు (వాదనలు[i]) + 1;
        }
    }

  ఒకవేళ (so_far >= అందుబాటు_len)
    so_far = అందుబాటు_len - 1;

  memset (argv[0], 0, అందుబాటు_len);
  జ్ఞాపకం_నకలు (argv[0], new_argv, so_far);
}

/* Entrypoint to the container.  */
స్థిర పూర్ణాంకం
container_init (శూన్య *వాదనలు, అక్షరం *notify_socket, పూర్ణాంకం sync_socket, libcrun_error_t *err)
{
  నిర్మాణం container_entrypoint_s *entrypoint_వాదనలు = వాదనలు;
  పూర్ణాంకం ret;
  runtime_spec_schema_config_schema *def = entrypoint_వాదనలు->container->container_def;
  cleanup_free అక్షరం *exec_మార్గం = ఖాళీ;
  __attribute__ ((వాడని)) cleanup_free అక్షరం *notify_socket_cleanup = notify_socket;
  pid_t own_pid = 0;

  entrypoint_వాదనలు->sync_socket = sync_socket;

  crun_సమితి_output_handler (log_write_గమ్యం_sync_socket, వాదనలు);

  /* sync receive own pid.  */
  ret = తాత్కాలిక_FAILURE_RETRY (పఠ (sync_socket, &own_pid, పరిమాణం (own_pid)));
  ఒకవేళ (UNLIKELY (ret != పరిమాణం (own_pid)))
    {
      ఒకవేళ (ret >= 0)
        errno = 0;
      తిరిగి crun_make_error (err, errno, "read from sync socket");
    }

  ret = container_init_అమరిక (వాదనలు, &own_pid, notify_socket, sync_socket, &exec_మార్గం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      /* If it fails to write the error using the sync socket, then fallback
         to stderr.  */
      ఒకవేళ (sync_socket_write_error (sync_socket, err) == 0)
        {
          crun_error_release (err);
          /* A return value of > 0 means to fail with an error without err being set so to avoid
             printing it twice.  */
          తిరిగి 1;
        }
      తిరిగి ret;
    }

  entrypoint_వాదనలు->sync_socket = -1;

  ret = unblock_సంకేతాలు (err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  /* sync 4.  */
  ret = sync_socket_send_sync (sync_socket, false, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  close_and_reset (&sync_socket);

  ఒకవేళ (entrypoint_వాదనలు->context->fifo_exec_wait_fd >= 0)
    {
      అక్షరం బఫరు[1];
      fd_set read_సమితి;
      cleanup_close పూర్ణాంకం fd = entrypoint_వాదనలు->context->fifo_exec_wait_fd;
      entrypoint_వాదనలు->context->fifo_exec_wait_fd = -1;

      FD_ZERO (&read_సమితి);
      FD_SET (fd, &read_సమితి);
      చేయి
        {
          ret = select (fd + 1, &read_సమితి, ఖాళీ, ఖాళీ, ఖాళీ);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "select");

          ret = తాత్కాలిక_FAILURE_RETRY (పఠ (fd, బఫరు, పరిమాణం (బఫరు)));
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "read from the exec fifo");
      } అయితే (ret == 0);

      close_and_reset (&entrypoint_వాదనలు->context->fifo_exec_wait_fd);
    }

  crun_సమితి_output_handler (log_write_గమ్యం_stderr, ఖాళీ);

  ఒకవేళ (def->process && def->process->no_new_privileges)
    {
      అక్షరం **seccomp_flags = ఖాళీ;
      size_t seccomp_flags_len = 0;
      cleanup_free అక్షరం *seccomp_fd_payload = ఖాళీ;
      size_t seccomp_fd_payload_len = 0;

      ఒకవేళ (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      ఒకవేళ (entrypoint_వాదనలు->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (entrypoint_వాదనలు->container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_వాదనలు->seccomp_fd, entrypoint_వాదనలు->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags,
                                   seccomp_flags_len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
      close_and_reset (&entrypoint_వాదనలు->seccomp_fd);
      close_and_reset (&entrypoint_వాదనలు->seccomp_receiver_fd);
    }

  ఒకవేళ (UNLIKELY (def->process == ఖాళీ))
    తిరిగి crun_make_error (err, 0, "block `process` not found");

  ఒకవేళ (UNLIKELY (exec_మార్గం == ఖాళీ))
    తిరిగి crun_make_error (err, 0, "executable path not specified");

  ఒకవేళ (def->hooks && def->hooks->ప్రారంభం_container_len)
    {
      libcrun_container_t *container = entrypoint_వాదనలు->container;
      libcrun_error_t tmp_err = ఖాళీ;
      పూర్ణాంకం లోపల_userns = check_running_లోపల_user_namespace (&tmp_err);
      ఒకవేళ (tmp_err)
        crun_error_release (&tmp_err);

      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_వాదనలు->bundle, ఖాళీ, "created", "startContainer",
                      (hook **) def->hooks->ప్రారంభం_container,
                      def->hooks->ప్రారంభం_container_len, entrypoint_వాదనలు->hooks_బహిర్గతం_fd, entrypoint_వాదనలు->hooks_err_fd,
                      లోపల_userns > 0, err);
      ఒకవేళ (UNLIKELY (ret != 0))
        తిరిగి ret;

      /* Seek stdout/stderr to the end.  If the hooks were using the same files,
         the container process overwrites what was previously written.  */
      (శూన్య) lseek (1, 0, SEEK_END);
      (శూన్య) lseek (2, 0, SEEK_END);
    }

  ఒకవేళ (entrypoint_వాదనలు->custom_handler)
    {
      /* Files marked with O_CLOEXEC are closed at execv time, so make sure they are closed now.
         This is a best effort operation, because the seccomp filter is already in place and it could
         stop some syscalls used by mark_or_close_fds_ge_than.
      */
      ఒకవేళ (entrypoint_వాదనలు->custom_handler->vtable->close_fds)
        {
          ret = entrypoint_వాదనలు->custom_handler->vtable->close_fds (entrypoint_వాదనలు->custom_handler->cookie,
                                                                    entrypoint_వాదనలు->container,
                                                                    entrypoint_వాదనలు->context->preserve_fds,
                                                                    err);
        }
      లేకపోతే
        {
          ret = mark_or_close_fds_ge_than (entrypoint_వాదనలు->container, entrypoint_వాదనలు->context->preserve_fds + 3, true, err);
        }
      ఒకవేళ (UNLIKELY (ret < 0))
        crun_error_release (err);

      prctl (PR_సమితి_NAME, entrypoint_వాదనలు->custom_handler->vtable->name);

      ఒకవేళ (entrypoint_వాదనలు->context->argv)
        {
          erase_initial_environment (entrypoint_వాదనలు->context->argv, entrypoint_వాదనలు->context->argc);
          rewrite_argv (entrypoint_వాదనలు->context->argv, entrypoint_వాదనలు->context->argc,
                        entrypoint_వాదనలు->custom_handler->vtable->name, def->process->వాదనలు,
                        def->process->వాదనలు_len);

          /* It is a quite destructive operation as we might be referencing data from the old
             argv, so make sure the context is not reused.  */
          entrypoint_వాదనలు->context->argv = ఖాళీ;
          entrypoint_వాదనలు->context = ఖాళీ;
        }

      ret = libcrun_సమితి_selinux_label (entrypoint_వాదనలు->container, def->process, true, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = libcrun_సమితి_apparmor_profile (entrypoint_వాదనలు->container, def->process, true, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = entrypoint_వాదనలు->custom_handler->vtable->నిరంతరం_func (entrypoint_వాదనలు->custom_handler->cookie,
                                                               entrypoint_వాదనలు->container,
                                                               exec_మార్గం,
                                                               def->process->వాదనలు);
      ఒకవేళ (ret != 0)
        తిరిగి crun_make_error (err, 0, "exec container process failed with handler as `%s`", entrypoint_వాదనలు->custom_handler->vtable->name);

      తిరిగి ret;
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation since the seccomp profile is already in place now and might block
     some of the syscalls needed by mark_or_close_fds_ge_than.  */
  ret = mark_or_close_fds_ge_than (entrypoint_వాదనలు->container, entrypoint_వాదనలు->context->preserve_fds + 3, true, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    crun_error_release (err);

  తాత్కాలిక_FAILURE_RETRY (execv (exec_మార్గం, def->process->వాదనలు));

  ఒకవేళ (errno == ENOENT)
    తిరిగి crun_make_error (err, errno, "exec container process (missing dynamic library?) `%s`", exec_మార్గం);

  తిరిగి crun_make_error (err, errno, "exec container process `%s`", exec_మార్గం);
}

స్థిర పూర్ణాంకం
read_container_config_from_state (libcrun_container_t **container, స్థిరరాశి అక్షరం *state_మూలం, స్థిరరాశి అక్షరం *id,
                                  libcrun_error_t *err)
{
  cleanup_free అక్షరం *config_file = ఖాళీ;
  cleanup_free అక్షరం *dir = ఖాళీ;
  పూర్ణాంకం ret;

  *container = ఖాళీ;

  ret = libcrun_get_state_directory (&dir, state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = append_paths (&config_file, err, dir, "config.json", ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  *container = libcrun_container_ఎక్కించు_from_file (config_file, err);
  ఒకవేళ (*container == ఖాళీ)
    తిరిగి -1;

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
నిరంతరం_poststop_hooks (arg_వాడని libcrun_context_t *context, libcrun_container_t *container,
                    runtime_spec_schema_config_schema *def,
                    libcrun_container_స్థితి_t *స్థితి_2, స్థిరరాశి అక్షరం *state_మూలం, స్థిరరాశి అక్షరం *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container_cleanup = ఖాళీ;
  పూర్ణాంకం ret;

  ఒకవేళ (def == ఖాళీ)
    {
      ఒకవేళ (container == ఖాళీ)
        {
          ret = read_container_config_from_state (&container_cleanup, state_మూలం, id, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
          container = container_cleanup;
        }

      def = container->container_def;
    }

  ఒకవేళ (def->hooks && def->hooks->poststop_len)
    {
      cleanup_close పూర్ణాంకం hooks_బహిర్గతం_fd = -1;
      cleanup_close పూర్ణాంకం hooks_err_fd = -1;

      ఒకవేళ (container == ఖాళీ)
        {
          ret = read_container_config_from_state (&container_cleanup, state_మూలం, id, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
          container = container_cleanup;
        }

      ret = open_hooks_output (container, &hooks_బహిర్గతం_fd, &hooks_err_fd, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = do_hooks (def, 0, id, true, స్థితి_2->bundle, స్థితి_2->bundle, "stopped", "poststop", (hook **) def->hooks->poststop,
                      def->hooks->poststop_len, hooks_బహిర్గతం_fd, hooks_err_fd, false, err);
      ఒకవేళ (UNLIKELY (ret != 0))
        {
          ఒకవేళ (ret < 0)
            libcrun_error_report_and_release (err);
          లేకపోతే
            libcrun_error (0, "poststop hook failed with exit code: %d", ret);
        }
    }
  తిరిగి 0;
}

స్థిర bool
has_new_pid_namespace (runtime_spec_schema_config_schema *def)
{
  size_t i;

  ఒకవేళ (def->linux == ఖాళీ || def->linux->namespaces == ఖాళీ)
    తిరిగి false;

  కొరకు (i = 0; i < def->linux->namespaces_len; i++)
    {
      ఒకవేళ (strcmp (def->linux->namespaces[i]->type, "pid") == 0 && def->linux->namespaces[i]->మార్గం == ఖాళీ)
        తిరిగి true;
    }
  తిరిగి false;
}

/* Delete the container state.  CONTAINER is the container the caller already has
   available, or NULL.  When it is not NULL, the poststop hooks are run even if the
   status file was never written, e.g. when the container creation failed.  */
స్థిర పూర్ణాంకం
container_delete_అంతర్గత (libcrun_context_t *context, libcrun_container_t *container,
                           runtime_spec_schema_config_schema *def, స్థిరరాశి అక్షరం *id, bool force, bool killall,
                           libcrun_error_t *err)
{
  cleanup_cgroup_స్థితి నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి = ఖాళీ;
  cleanup_container_స్థితి libcrun_container_స్థితి_t స్థితి_2 = {};
  cleanup_container libcrun_container_t *container_cleanup = ఖాళీ;
  స్థిరరాశి అక్షరం *state_మూలం = context->state_మూలం;
  పూర్ణాంకం ret;

  ret = libcrun_read_container_స్థితి (&స్థితి_2, state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ఒకవేళ (force && crun_error_get_errno (err) == ENOENT)
        {
          libcrun_error_t tmp_err = ఖాళీ;

          crun_error_release (err);

          ఒకవేళ (container)
            {
              libcrun_container_స్థితి_t partial_స్థితి = {
                .bundle = (అక్షరం *) context->bundle,
              };

              ret = నిరంతరం_poststop_hooks (context, container, def, &partial_స్థితి, state_మూలం, id, err);
              ఒకవేళ (UNLIKELY (ret < 0))
                crun_error_write_warning_and_release (context->output_handler_arg, &err);
            }

          libcrun_container_delete_స్థితి (state_మూలం, id, &tmp_err);
          crun_error_release (&tmp_err);
          తిరిగి 0;
        }
      crun_error_release (err);
      తిరిగి libcrun_container_delete_స్థితి (state_మూలం, id, err);
    }

  cgroup_స్థితి = libcrun_cgroup_make_స్థితి (&స్థితి_2);

  ఒకవేళ (! force)
    {
      ret = libcrun_is_container_running (&స్థితి_2, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
      ఒకవేళ (ret == 1)
        {
          ret = libcrun_స్థితి_has_read_exec_fifo (state_మూలం, id, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
          ఒకవేళ (ret == 0)
            తిరిగి crun_make_error (err, 0, "the container `%s` is not in `created` or `stopped` state", id);

          /* If the container is in "created" state, then do the equivalent of delete --force.  */
          killall = force = true;
        }
    }

  ఒకవేళ (def == ఖాళీ)
    {
      ఒకవేళ (container == ఖాళీ)
        {
          ret = read_container_config_from_state (&container_cleanup, state_మూలం, id, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;

          container = container_cleanup;
        }

      def = container->container_def;
    }

  ఒకవేళ (killall && force)
    {
      /* If the container has a pid namespace, it is enough to kill the first
         process (pid=1 in the namespace).
      */
      ఒకవేళ (has_new_pid_namespace (def))
        {
          ret = libcrun_kill_linux (&స్థితి_2, SIGKILL, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            {
              ఒకవేళ (crun_error_get_errno (err) != ESRCH)
                తిరిగి ret;

              crun_error_release (err);
            }
        }
      లేకపోతే ఒకవేళ (స్థితి_2.cgroup_మార్గం)
        {
          ret = libcrun_cgroup_killall (cgroup_స్థితి, SIGKILL, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }
    }

  ఒకవేళ (def->linux && def->linux->intel_rdt)
    {
      ret = libcrun_destroy_intelrdt (id, def, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        libcrun_error_report_and_release (err);
    }

  ఒకవేళ (స్థితి_2.cgroup_మార్గం)
    {
      /* If the cgroup cannot be destroyed, there are still processes in it.
         Keep the container status, it is the only record of the cgroup, so
         that the delete can be retried.  */
      ret = libcrun_cgroup_destroy (cgroup_స్థితి, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ret = నిరంతరం_poststop_hooks (context, container, def, &స్థితి_2, state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  తిరిగి libcrun_container_delete_స్థితి (state_మూలం, id, err);
}

పూర్ణాంకం
libcrun_container_delete (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, bool force, libcrun_error_t *err)
{
  తిరిగి container_delete_అంతర్గత (context, ఖాళీ, ఖాళీ, id, force, true, err);
}

పూర్ణాంకం
libcrun_container_kill (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *సంకేతం, libcrun_error_t *err)
{
  పూర్ణాంకం sig, ret;
  స్థిరరాశి అక్షరం *state_మూలం = context->state_మూలం;
  cleanup_container_స్థితి libcrun_container_స్థితి_t స్థితి_2 = {};

  sig = str2sig (సంకేతం);
  ఒకవేళ (UNLIKELY (sig < 0))
    తిరిగి crun_make_error (err, 0, "unknown signal `%s`", సంకేతం);

  ret = libcrun_read_container_స్థితి (&స్థితి_2, state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి libcrun_kill_linux (&స్థితి_2, sig, err);
}

పూర్ణాంకం
libcrun_container_killall (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *సంకేతం, libcrun_error_t *err)
{
  పూర్ణాంకం sig, ret;
  స్థిరరాశి అక్షరం *state_మూలం = context->state_మూలం;
  cleanup_container_స్థితి libcrun_container_స్థితి_t స్థితి_2 = {};
  cleanup_cgroup_స్థితి నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి = ఖాళీ;

  sig = str2sig (సంకేతం);
  ఒకవేళ (UNLIKELY (sig < 0))
    తిరిగి crun_make_error (err, 0, "unknown signal `%s`", సంకేతం);

  ret = libcrun_read_container_స్థితి (&స్థితి_2, state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  cgroup_స్థితి = libcrun_cgroup_make_స్థితి (&స్థితి_2);

  ret = libcrun_cgroup_killall (cgroup_స్థితి, sig, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
write_container_స్థితి (libcrun_container_t *container, libcrun_context_t *context,
                        pid_t pid, నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి,
                        libcrun_error_t *err)
{
  cleanup_free అక్షరం *bundle_allocated = ఖాళీ;
  స్థిరరాశి అక్షరం *bundle = get_bundle_మార్గం (context->bundle, &bundle_allocated);
  cleanup_free అక్షరం *owner = get_user_name (geteuid ());
  అక్షరం *external_descriptors = libcrun_get_external_descriptors (container);
  అక్షరం *rootfs = container->container_def->మూలం ? container->container_def->మూలం->మార్గం : "";
  అక్షరం created[35];

  libcrun_container_స్థితి_t స్థితి_2 = {
    .pid = pid,
    .rootfs = rootfs,
    .bundle = (అక్షరం *) bundle,
    .created = created,
    .owner = owner,
    .systemd_cgroup = context->systemd_cgroup,
    .detached = context->detach,
    .no_new_keyring = context->no_new_keyring,
    .external_descriptors = external_descriptors,
    .cgroup_మార్గం = ఖాళీ,
    .scope = ఖాళీ,
  };

  get_current_timestamp (created, పరిమాణం (created));

  ఒకవేళ (cgroup_స్థితి)
    {
      పూర్ణాంకం ret;

      ret = libcrun_cgroup_get_స్థితి (cgroup_స్థితి, &స్థితి_2, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (external_descriptors == ఖాళీ)
    తిరిగి crun_make_error (err, 0, "invalid internal state.  No external descriptors found");
  తిరిగి libcrun_write_container_స్థితి (context->state_మూలం, context->id, &స్థితి_2, err);
}

స్థిర పూర్ణాంకం
reap_subprocesses (pid_t *main_process, పూర్ణాంకం *main_process_exit, పూర్ణాంకం *చివరి_process, libcrun_error_t *err)
{
  *చివరి_process = 0;
  అయితే (1)
    {
      పూర్ణాంకం స్థితి_2;
      పూర్ణాంకం r = waitpid_ignore_stopped (-1, &స్థితి_2, WNOHANG);
      ఒకవేళ (r < 0)
        {
          ఒకవేళ (errno == EINTR)
            కొనసాగు;
          ఒకవేళ (errno == ECHILD)
            {
              *చివరి_process = 1;
              తిరిగి 0;
            }
          తిరిగి crun_make_error (err, errno, "waitpid");
        }
      ఒకవేళ (r == 0)
        ఆపు;
      ఒకవేళ (r != *main_process)
        కొనసాగు;

      *main_process_exit = get_process_exit_స్థితి (స్థితి_2);

      /* Once reaped, the pid can be reused by an unrelated process.  */
      *main_process = 0;
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
send_sd_notify (స్థిరరాశి అక్షరం *ready_str, libcrun_error_t *err)
{
#నిర్వచితమైతే HAVE_SYSTEMD
  పూర్ణాంకం ret = sd_notify (0, ready_str);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, -ret, "sd_notify");

#  ఒకవేళ HAVE_SD_NOTIFY_BARRIER
  /* Hard-code a 30 seconds timeout.  Ignore errors.  */
  sd_notify_barrier (0, 30 * 1000000);
#  endif
#లేకపోతే
  (శూన్య) ready_str;
  (శూన్య) err;
#అయితే_ముగింపు
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
handle_notify_socket (పూర్ణాంకం notify_socketfd, libcrun_error_t *err)
{
#నిర్వచితమైతే HAVE_SYSTEMD
  పూర్ణాంకం ret;
  అక్షరం buf[256];
  స్థిరరాశి అక్షరం *ready_str = "READY=1";

  ret = recvfrom (notify_socketfd, buf, పరిమాణం (buf) - 1, 0, ఖాళీ, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "recvfrom notify socket");

  buf[ret] = '\0';
  ఒకవేళ (అక్షరశ్రేణి_శోధన (buf, ready_str))
    {
      ret = send_sd_notify (ready_str, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      తిరిగి 1;
    }
  తిరిగి 0;
#లేకపోతే
  (శూన్య) notify_socketfd;
  (శూన్య) err;
  తిరిగి 1;
#అయితే_ముగింపు
}

నిర్మాణం wait_for_process_వాదనలు
{
  pid_t pid;
  libcrun_context_t *context;
  పూర్ణాంకం terminal_fd;
  పూర్ణాంకం notify_socket;
  పూర్ణాంకం *container_ready_fd;
  పూర్ణాంకం seccomp_notify_fd;
  స్థిరరాశి అక్షరం *seccomp_notify_plugins;
};

స్థిర పూర్ణాంకం
write_pid_file (స్థిరరాశి అక్షరం *pid_file, pid_t pid, libcrun_error_t *err)
{
  అక్షరం buf[32];
  పూర్ణాంకం buf_len = snprintf (buf, పరిమాణం (buf), "%d", pid);
  ఒకవేళ (UNLIKELY (buf_len >= (పూర్ణాంకం) పరిమాణం (buf)))
    తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");
  తిరిగి write_file_at_with_flags (AT_FDCWD, O_CREAT | O_TRUNC, 0700, pid_file, buf, buf_len, err);
}

#నిర్వచించు MAX_EVENTS 10

స్థిర bool
is_fd_pollable (పూర్ణాంకం fd)
{
  నిర్మాణం epoll_event ev = { .events = EPOLLIN };
  cleanup_close పూర్ణాంకం epollfd = epoll_create1 (EPOLL_CLOEXEC);

  ఒకవేళ (epollfd < 0)
    తిరిగి true;

  తిరిగి epoll_ctl (epollfd, EPOLL_CTL_కూడు, fd, &ev) == 0 || errno != EPERM;
}

స్థిర పూర్ణాంకం
wait_for_process (నిర్మాణం wait_for_process_వాదనలు *వాదనలు, libcrun_error_t *err)
{
  cleanup_channel_fd_pair నిర్మాణం channel_fd_pair *from_terminal = ఖాళీ;
  cleanup_channel_fd_pair నిర్మాణం channel_fd_pair *గమ్యం_terminal = ఖాళీ;
  పూర్ణాంకం ret, container_exit_సంకేతం = 0, చివరి_process;
  cleanup_close పూర్ణాంకం terminal_fd_from = -1;
  cleanup_close పూర్ణాంకం terminal_fd_గమ్యం = -1;
  bool stdin_pollable = false, stdout_pollable = false;
  cleanup_close పూర్ణాంకం epollfd = -1;
  cleanup_close పూర్ణాంకం signalfd = -1;
  sigset_t ముసుగు;
  పూర్ణాంకం లోపల_fds[MAX_EVENTS];
  పూర్ణాంకం లోపల_fds_len = 0;
  పూర్ణాంకం బహిర్గతం_fds[MAX_EVENTS];
  పూర్ణాంకం బహిర్గతం_fds_len = 0;
  size_t i;

  cleanup_seccomp_notify_context నిర్మాణం seccomp_notify_context_s *seccomp_notify_ctx = ఖాళీ;

  container_exit_సంకేతం = 0;

  ఒకవేళ (వాదనలు == ఖాళీ || వాదనలు->context == ఖాళీ)
    తిరిగి crun_make_error (err, 0, "internal error: context is empty");

  కొరకు (i = 0; i < MAX_EVENTS; i++)
    {
      లోపల_fds[i] = -1;
      బహిర్గతం_fds[i] = -1;
    }

  ఒకవేళ (వాదనలు->context->pid_file)
    {
      ret = write_pid_file (వాదనలు->context->pid_file, వాదనలు->pid, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  /* Also exit if there is nothing more to wait for.  */
  ఒకవేళ (వాదనలు->context->detach && వాదనలు->notify_socket < 0)
    తిరిగి 0;

  ఒకవేళ (వాదనలు->container_ready_fd)
    {
      ret = 0;
      తాత్కాలిక_FAILURE_RETRY (లిఖ (*వాదనలు->container_ready_fd, &ret, పరిమాణం (ret)));
      close_and_reset (వాదనలు->container_ready_fd);
    }

  sigfillset (&ముసుగు);
  ret = sigprocmask (SIG_ఖండం, &ముసుగు, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "sigprocmask");

  signalfd = create_signalfd (&ముసుగు, err);
  ఒకవేళ (UNLIKELY (signalfd < 0))
    తిరిగి signalfd;

  ret = reap_subprocesses (&వాదనలు->pid, &container_exit_సంకేతం, &చివరి_process, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (చివరి_process)
    తిరిగి container_exit_సంకేతం;

  ఒకవేళ (వాదనలు->seccomp_notify_fd >= 0)
    {
      cleanup_free అక్షరం *state_మూలం = ఖాళీ;
      cleanup_free అక్షరం *oci_config_మార్గం = ఖాళీ;

      నిర్మాణం libcrun_ఎక్కించు_seccomp_notify_conf_s conf;
      memset (&conf, 0, పరిమాణం conf);

      ret = libcrun_get_state_directory (&state_మూలం, వాదనలు->context->state_మూలం, వాదనలు->context->id, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = append_paths (&oci_config_మార్గం, err, state_మూలం, "config.json", ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      conf.runtime_మూలం_మార్గం = state_మూలం;
      conf.name = వాదనలు->context->id;
      conf.bundle_మార్గం = వాదనలు->context->bundle;
      conf.oci_config_మార్గం = oci_config_మార్గం;

      ret = సమితి_blocking_fd (వాదనలు->seccomp_notify_fd, false, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = libcrun_ఎక్కించు_seccomp_notify_plugins (&seccomp_notify_ctx,
                                                 వాదనలు->seccomp_notify_plugins,
                                                 &conf, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      లోపల_fds[లోపల_fds_len++] = వాదనలు->seccomp_notify_fd;
    }

  ఒకవేళ (వాదనలు->terminal_fd >= 0)
    {
      /* The terminal_fd is dup()ed so that it can be registered with
         epoll multiple times using different masks.  */
      terminal_fd_from = dup (వాదనలు->terminal_fd);
      ఒకవేళ (UNLIKELY (terminal_fd_from < 0))
        తిరిగి crun_make_error (err, errno, "dup terminal fd");
      terminal_fd_గమ్యం = dup (వాదనలు->terminal_fd);
      ఒకవేళ (UNLIKELY (terminal_fd_గమ్యం < 0))
        తిరిగి crun_make_error (err, errno, "dup terminal fd");

      పూర్ణాంకం i, non_blocking_fds[] = { terminal_fd_from, terminal_fd_గమ్యం, 0, 1, -1 };
      కొరకు (i = 0; non_blocking_fds[i] >= 0; i++)
        {
          ret = సమితి_blocking_fd (non_blocking_fds[i], false, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }

      /* stdin and stdout can be something epoll does not support, such as
         /dev/null or a regular file.  Such a stdin is not forwarded, and
         such a stdout is written to without waiting for it to be ready.  */
      stdin_pollable = is_fd_pollable (0);
      stdout_pollable = is_fd_pollable (1);

      from_terminal = channel_fd_pair_new (terminal_fd_from, 1, BUFSIZ);
      ఒకవేళ (stdin_pollable)
        గమ్యం_terminal = channel_fd_pair_new (0, terminal_fd_గమ్యం, BUFSIZ);
    }

  లోపల_fds[లోపల_fds_len++] = signalfd;
  ఒకవేళ (వాదనలు->notify_socket >= 0)
    లోపల_fds[లోపల_fds_len++] = వాదనలు->notify_socket;
  ఒకవేళ (వాదనలు->terminal_fd >= 0)
    {
      ఒకవేళ (stdin_pollable)
        {
          లోపల_fds[లోపల_fds_len++] = 0;
          బహిర్గతం_fds[బహిర్గతం_fds_len++] = terminal_fd_గమ్యం;
        }

      లోపల_fds[లోపల_fds_len++] = terminal_fd_from;
      ఒకవేళ (stdout_pollable)
        బహిర్గతం_fds[బహిర్గతం_fds_len++] = 1;
    }

  epollfd = epoll_helper (లోపల_fds, ఖాళీ, బహిర్గతం_fds, ఖాళీ, err);
  ఒకవేళ (UNLIKELY (epollfd < 0))
    తిరిగి epollfd;

  అయితే (1)
    {
      నిర్మాణం epoll_event events[MAX_EVENTS];
      నిర్మాణం signalfd_siginfo si;
      నిర్మాణం winsize ws;
      పూర్ణాంకం i, nr_events;
      ssize_t res;

      nr_events = తాత్కాలిక_FAILURE_RETRY (epoll_wait (epollfd, events, MAX_EVENTS, -1));
      ఒకవేళ (UNLIKELY (nr_events < 0))
        తిరిగి crun_make_error (err, errno, "epoll_wait");

      కొరకు (i = 0; i < nr_events; i++)
        {
          ఒకవేళ (events[i].దత్తాంశం.fd == 0 || events[i].దత్తాంశం.fd == terminal_fd_గమ్యం)
            {
              ret = channel_fd_pair_process (గమ్యం_terminal, epollfd, err);
              ఒకవేళ (UNLIKELY (ret < 0))
                తిరిగి crun_error_wrap (err, "copy to terminal fd");
            }
          లేకపోతే ఒకవేళ (events[i].దత్తాంశం.fd == 1 || events[i].దత్తాంశం.fd == terminal_fd_from)
            {
              ret = channel_fd_pair_process (from_terminal, epollfd, err);
              ఒకవేళ (UNLIKELY (ret < 0))
                తిరిగి crun_error_wrap (err, "copy from terminal fd");
            }
          లేకపోతే ఒకవేళ (events[i].దత్తాంశం.fd == వాదనలు->seccomp_notify_fd)
            {
              ret = libcrun_seccomp_notify_plugins (seccomp_notify_ctx,
                                                    వాదనలు->seccomp_notify_fd, err);
              ఒకవేళ (UNLIKELY (ret < 0))
                తిరిగి ret;
            }
          లేకపోతే ఒకవేళ (events[i].దత్తాంశం.fd == వాదనలు->notify_socket)
            {
              ret = handle_notify_socket (వాదనలు->notify_socket, err);
              ఒకవేళ (UNLIKELY (ret < 0))
                తిరిగి ret;
              ఒకవేళ (ret && వాదనలు->context->detach)
                తిరిగి 0;
            }
          లేకపోతే ఒకవేళ (events[i].దత్తాంశం.fd == signalfd)
            {
              res = తాత్కాలిక_FAILURE_RETRY (పఠ (signalfd, &si, పరిమాణం (si)));
              ఒకవేళ (UNLIKELY (res < 0))
                తిరిగి crun_make_error (err, errno, "read from signalfd");
              ఒకవేళ (si.ssi_signo == SIGCHLD)
                {
                  ret = reap_subprocesses (&వాదనలు->pid, &container_exit_సంకేతం,
                                           &చివరి_process, err);
                  ఒకవేళ (UNLIKELY (ret < 0))
                    తిరిగి ret;
                  ఒకవేళ (చివరి_process)
                    తిరిగి container_exit_సంకేతం;
                }
              లేకపోతే ఒకవేళ (si.ssi_signo == SIGWINCH)
                {
                  /* Ignore the signal if the terminal is not available.  */
                  ఒకవేళ (వాదనలు->terminal_fd > 0)
                    {
                      ret = ioctl (0, TIOCGWINSZ, &ws);
                      ఒకవేళ (UNLIKELY (ret < 0))
                        తిరిగి crun_make_error (err, errno, "ioctl TIOCGWINSZ copy terminal size from stdin");

                      ret = ioctl (వాదనలు->terminal_fd, TIOCSWINSZ, &ws);
                      ఒకవేళ (UNLIKELY (ret < 0))
                        తిరిగి crun_make_error (err, errno, "ioctl TIOCSWINSZ copy terminal size to pty");
                    }
                }
              లేకపోతే ఒకవేళ (వాదనలు->pid > 0)
                {
                  /* Send any other signal to the child process, if it was not reaped yet.  */
                  ret = kill (వాదనలు->pid, si.ssi_signo);
                }
            }
          లేకపోతే
            {
              తిరిగి crun_make_error (err, 0, "internal error: unknown fd from epoll_wait");
            }
        }
    }

  తిరిగి 0;
}

స్థిర శూన్య
flush_fd_గమ్యం_err (libcrun_context_t *context, పూర్ణాంకం terminal_fd)
{
  అక్షరం buf[256];
  పూర్ణాంకం flags;

  ఒకవేళ (terminal_fd < 0 || stderr == ఖాళీ)
    తిరిగి;

  flags = fcntl (terminal_fd, F_GETFL, 0);
  ఒకవేళ (flags == -1)
    తిరిగి;
  ఒకవేళ (fcntl (terminal_fd, F_SETFL, flags | O_NONBLOCK) < 0)
    తిరిగి;

  కొరకు (;;)
    {
      పూర్ణాంకం ret = తాత్కాలిక_FAILURE_RETRY (పఠ (terminal_fd, buf, పరిమాణం (buf) - 1));
      ఒకవేళ (ret <= 0)
        ఆపు;
      buf[ret] = '\0';
      ఒకవేళ (context->output_handler)
        context->output_handler (0, buf, LIBCRUN_VERBOSITY_ERROR, context->output_handler_arg);
    }
  (శూన్య) fcntl (terminal_fd, F_SETFL, flags);
  fflush (stderr);
  fsync (1);
  fsync (2);
}

స్థిర పూర్ణాంకం
cleanup_watch (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
               నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి, pid_t init_pid, పూర్ణాంకం sync_socket,
               పూర్ణాంకం terminal_fd, libcrun_error_t *err)
{
  స్థిరరాశి అక్షరం *oom_సందేశం = ఖాళీ;
  libcrun_error_t tmp_err = ఖాళీ;
  పూర్ణాంకం ret;

  ఒకవేళ (init_pid)
    {
      /* Try to detect whether the cgroup has a OOM.  */
      ఒకవేళ (cgroup_స్థితి)
        {
          పూర్ణాంకం has_oom;

          has_oom = libcrun_cgroup_has_oom (cgroup_స్థితి, &tmp_err);
          ఒకవేళ (has_oom > 0)
            oom_సందేశం = "OOM: the memory limit could be too low";
          లేకపోతే ఒకవేళ (has_oom < 0)
            {
              /* If the detection has failed for any reason, e.g. the cgroup was
                 already deleted by the time it was checked, just ignore the
                 failure.  */
              crun_error_release (&tmp_err);
            }
        }
      /* If the OOM wasn't detected, look into the static configuration.  */
      ఒకవేళ (oom_సందేశం == ఖాళీ && is_జ్ఞాపకం_హద్దు_too_దిగువ (def))
        oom_సందేశం = "the memory limit could be too low";

      kill (init_pid, SIGKILL);
      waitpid_ignore_stopped (init_pid, ఖాళీ, 0);
    }

  ret = sync_socket_wait_sync (context, sync_socket, true, &tmp_err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      crun_error_release (err);
      *err = tmp_err;
    }

  ఒకవేళ (terminal_fd >= 0)
    flush_fd_గమ్యం_err (context, terminal_fd);

  ఒకవేళ (oom_సందేశం)
    తిరిగి crun_error_wrap (err, "%s", oom_సందేశం);

  తిరిగి -1;
}

/* Find the uid:gid that is mapped to root inside the container user namespace.  */
శూన్య
get_మూలం_లోపల_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid, uid_t *uid,
                        gid_t *gid)
{
  *uid = -1;
  *gid = -1;

  /* Not root in the namespace.  */
  ఒకవేళ (host_uid)
    తిరిగి;

  ఒకవేళ (def->linux && def->linux->uid_mappings)
    {
      size_t i;

      కొరకు (i = 0; i < def->linux->uid_mappings_len; i++)
        ఒకవేళ (def->linux->uid_mappings[i]->container_id == 0)
          {
            *uid = def->linux->uid_mappings[i]->host_id;
            ఆపు;
          }
    }
  ఒకవేళ (def->linux && def->linux->gid_mappings)
    {
      size_t i;

      కొరకు (i = 0; i < def->linux->gid_mappings_len; i++)
        ఒకవేళ (def->linux->gid_mappings[i]->container_id == 0)
          {
            *gid = def->linux->gid_mappings[i]->host_id;
            ఆపు;
          }
    }

  /* If the uid and the gid are not changed, do not attempt any chown.  */
  ఒకవేళ (*uid == host_uid && *gid == host_gid)
    *uid = *gid = -1;
}

స్థిర పూర్ణాంకం
get_seccomp_receiver_fd (libcrun_container_t *container, పూర్ణాంకం *fd, పూర్ణాంకం *self_receiver_fd, స్థిరరాశి అక్షరం **plugins,
                         libcrun_error_t *err)
{
  స్థిరరాశి అక్షరం *tmp;
  runtime_spec_schema_config_schema *def = container->container_def;

  *fd = -1;
  *self_receiver_fd = -1;

  tmp = కనుగొను_annotation (container, "run.oci.seccomp.plugins");
  ఒకవేళ (tmp)
    {
      పూర్ణాంకం fds[2];
      పూర్ణాంకం ret;

      ret = create_socket_pair (fds, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_error_wrap (err, "create socket pair");

      *fd = fds[0];
      *self_receiver_fd = fds[1];
      *plugins = tmp;
      తిరిగి 0;
    }

  ఒకవేళ (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_మార్గం)
    tmp = def->linux->seccomp->listener_మార్గం;
  లేకపోతే
    tmp = కనుగొను_annotation (container, "run.oci.seccomp.receiver");
  ఒకవేళ (tmp == ఖాళీ)
    tmp = getenv ("RUN_OCI_SECCOMP_RECEIVER");
  ఒకవేళ (tmp)
    {
      ఒకవేళ (tmp[0] != '/')
        తిరిగి crun_make_error (err, 0, "the seccomp receiver `%s` is not an absolute path", tmp);

      *fd = open_unix_domain_client_socket (tmp, 0, err);
      ఒకవేళ (UNLIKELY (*fd < 0))
        తిరిగి crun_error_wrap (err, "open seccomp receiver");
    }

  తిరిగి 0;
}

స్థిర bool
has_seccomp_receiver (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;

  ఒకవేళ (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_మార్గం)
    తిరిగి true;

  ఒకవేళ (కనుగొను_annotation (container, "run.oci.seccomp.receiver") != ఖాళీ || getenv ("RUN_OCI_SECCOMP_RECEIVER") != ఖాళీ)
    తిరిగి true;

  తిరిగి false;
}

స్థిర పూర్ణాంకం
అమరిక_container_hooks_output (libcrun_container_t *container, runtime_spec_schema_config_schema *def,
                              నిర్మాణం container_entrypoint_s *container_వాదనలు, పూర్ణాంకం *hooks_బహిర్గతం_fd,
                              పూర్ణాంకం *hooks_err_fd, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ఒకవేళ (def->hooks
      && (def->hooks->prestart_len || def->hooks->poststart_len || def->hooks->create_runtime_len
          || def->hooks->create_container_len || def->hooks->ప్రారంభం_container_len))
    {
      ret = open_hooks_output (container, hooks_బహిర్గతం_fd, hooks_err_fd, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
      container_వాదనలు->hooks_బహిర్గతం_fd = *hooks_బహిర్గతం_fd;
      container_వాదనలు->hooks_err_fd = *hooks_err_fd;
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
అమరిక_container_keyring (libcrun_container_t *container, libcrun_context_t *context, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  పూర్ణాంకం ret;

  ఒకవేళ (! context->no_new_keyring)
    {
      స్థిరరాశి అక్షరం *label = ఖాళీ;

      libcrun_debug ("Creating new keyring");

      ఒకవేళ (def->process)
        {
          label = def->process->selinux_label;
          ఒకవేళ (label)
            libcrun_debug ("Using SELinux process label: `%s`", label);
        }

      ret = libcrun_create_keyring (container, container->context->id, label, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
అమరిక_terminal_socket_pair (libcrun_container_t *container, libcrun_context_t *context,
                            నిర్మాణం container_entrypoint_s *container_వాదనలు, పూర్ణాంకం *socket_pair_0,
                            పూర్ణాంకం *socket_pair_1, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  పూర్ణాంకం ret;
  పూర్ణాంకం detach = context->detach;

  ఒకవేళ (def->process && def->process->terminal && ! detach && context->console_socket == ఖాళీ)
    {
      libcrun_debug ("Creating terminal socket pair");
      container_వాదనలు->has_terminal_socket_pair = 1;
      ret = create_socket_pair (container_వాదనలు->terminal_socketpair, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_error_wrap (err, "create terminal socket");

      *socket_pair_0 = container_వాదనలు->terminal_socketpair[0];
      *socket_pair_1 = container_వాదనలు->terminal_socketpair[1];
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
అమరిక_seccomp (libcrun_container_t *container, స్థిరరాశి అక్షరం *seccomp_bpf_దత్తాంశం,
               నిర్మాణం libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, పూర్ణాంకం *seccomp_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  పూర్ణాంకం ret;

  ఒకవేళ (కనుగొను_annotation (container, "run.oci.seccomp.plugins") != ఖాళీ && has_seccomp_receiver (container))
    {
      తిరిగి crun_make_error (err, 0, "seccomp plugins and seccomp receivers cannot be declared at the same time");
    }

  ఒకవేళ (def->linux && (def->linux->seccomp || seccomp_bpf_దత్తాంశం))
    {
      గుర్తులేని పూర్ణాంకం seccomp_gen_options = 0;
      స్థిరరాశి అక్షరం *annotation;

      libcrun_debug ("Initializing seccomp");
      annotation = కనుగొను_annotation (container, "run.oci.seccomp_fail_unknown_syscall");
      ఒకవేళ (annotation && strcmp (annotation, "0") != 0)
        seccomp_gen_options = LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL;

      ఒకవేళ (seccomp_bpf_దత్తాంశం)
        seccomp_gen_options |= LIBCRUN_SECCOMP_SKIP_CACHE;

      libcrun_seccomp_gen_ctx_init (seccomp_gen_ctx, container, true, seccomp_gen_options);

      ret = libcrun_open_seccomp_bpf (seccomp_gen_ctx, seccomp_fd, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
అమరిక_console_socket (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
                      నిర్మాణం container_entrypoint_s *container_వాదనలు, పూర్ణాంకం *console_socket_fd,
                      libcrun_error_t *err)
{
  ఒకవేళ (context->console_socket && def->process && def->process->terminal)
    {
      *console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
      ఒకవేళ (UNLIKELY (*console_socket_fd < 0))
        తిరిగి crun_error_wrap (err, "open console socket");
      container_వాదనలు->console_socket_fd = *console_socket_fd;
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
అమరిక_cgroup_manager (libcrun_context_t *context, libcrun_container_t *container,
                      నిర్మాణం libcrun_cgroup_వాదనలు *cg, పూర్ణాంకం *cgroup_dirfd,
                      నిర్మాణం libcrun_dirfd_s *cgroup_dirfd_s, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  పూర్ణాంకం cgroup_manager;
  uid_t మూలం_uid = -1;
  gid_t మూలం_gid = -1;
  పూర్ణాంకం ret;

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  ఒకవేళ (context->systemd_cgroup)
    {
      libcrun_debug ("Using systemd cgroup manager");
      cgroup_manager = CGROUP_MANAGER_SYSTEMD;
    }
  లేకపోతే ఒకవేళ (context->force_no_cgroup)
    {
      libcrun_debug ("Disabling cgroup manager");
      cgroup_manager = CGROUP_MANAGER_DISABLED;
    }
  లేకపోతే
    libcrun_debug ("Using cgroupfs cgroup manager");

  /* If we are root (either on the host or in a namespace), then chown the cgroup to root
     in the container user namespace.  */
  get_మూలం_లోపల_the_userns (def, container->host_uid, container->host_gid, &మూలం_uid, &మూలం_gid);
  libcrun_debug ("Using container host UID `%d` and GID `%d`", container->host_uid, container->host_gid);

  memset (cg, 0, పరిమాణం (*cg));

  cg->cgroup_మార్గం = def->linux ? def->linux->cgroups_మార్గం : "";
  cg->manager = cgroup_manager;
  cg->id = context->id;
  cg->resources = def->linux ? def->linux->resources : ఖాళీ;
  cg->annotations = container->annotations;
  cg->మూలం_uid = మూలం_uid;
  cg->మూలం_gid = మూలం_gid;
  cg->state_మూలం = context->state_మూలం;
  cg->container = container;

  ret = libcrun_cgroup_preenter (cg, cgroup_dirfd, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  cgroup_dirfd_s->dirfd = cgroup_dirfd;
  cgroup_dirfd_s->joined = false;

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
సమితి_scheduler (pid_t pid, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ret = libcrun_సమితి_scheduler (pid, def->process, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_reset_cpu_affinity_ముసుగు (pid, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_సమితి_io_ప్రాధాన్యత (pid, def->process, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
seccomp_generation (పూర్ణాంకం seccomp_fd, స్థిరరాశి అక్షరం *seccomp_bpf_దత్తాంశం,
                    నిర్మాణం libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ఒకవేళ (seccomp_fd >= 0)
    {
      ఒకవేళ (seccomp_bpf_దత్తాంశం != ఖాళీ)
        {
          ret = libcrun_copy_seccomp (seccomp_gen_ctx, seccomp_bpf_దత్తాంశం, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }
      లేకపోతే
        {
          ret = libcrun_generate_seccomp (seccomp_gen_ctx, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
terminal_అమరిక (runtime_spec_schema_config_schema *def, libcrun_context_t *context,
                పూర్ణాంకం socket_pair_0, పూర్ణాంకం *terminal_fd, శూన్య **orig_terminal, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  పూర్ణాంకం detach = context->detach;

  ఒకవేళ (def->process && def->process->terminal && ! detach && context->console_socket == ఖాళీ)
    {
      libcrun_debug ("Receiving console socket fd");
      *terminal_fd = receive_fd_from_socket (socket_pair_0, err);
      ఒకవేళ (UNLIKELY (*terminal_fd < 0))
        తిరిగి -1;

      ret = libcrun_clear_onlcr (*terminal_fd, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = libcrun_సమితి_raw (0, orig_terminal, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
libcrun_container_నిరంతరం_అంతర్గత (libcrun_container_t *container, libcrun_context_t *context,
                                పూర్ణాంకం *container_ready_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  పూర్ణాంకం ret;
  pid_t pid;
  పూర్ణాంకం detach = context->detach;
  cleanup_cgroup_స్థితి నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి = ఖాళీ;
  cleanup_close పూర్ణాంకం terminal_fd = -1;
  cleanup_terminal శూన్య *orig_terminal = ఖాళీ;
  cleanup_close పూర్ణాంకం sync_socket = -1;
  cleanup_close పూర్ణాంకం notify_socket = -1;
  cleanup_close పూర్ణాంకం socket_pair_0 = -1;
  cleanup_close పూర్ణాంకం socket_pair_1 = -1;
  cleanup_close పూర్ణాంకం seccomp_fd = -1;
  cleanup_close పూర్ణాంకం console_socket_fd = -1;
  cleanup_close పూర్ణాంకం hooks_బహిర్గతం_fd = -1;
  cleanup_close పూర్ణాంకం hooks_err_fd = -1;
  cleanup_close పూర్ణాంకం own_seccomp_receiver_fd = -1;
  cleanup_close పూర్ణాంకం seccomp_notify_fd = -1;
  స్థిరరాశి అక్షరం *seccomp_notify_plugins = ఖాళీ;
  cleanup_cgroup_preenter నిర్మాణం libcrun_cgroup_వాదనలు cg = {};
  cleanup_free అక్షరం *bundle_allocated = ఖాళీ;
  నిర్మాణం container_entrypoint_s container_వాదనలు = {
    .container = container,
    .context = context,
    .bundle = get_bundle_మార్గం (context->bundle, &bundle_allocated),
    .terminal_socketpair = { -1, -1 },
    .console_socket_fd = -1,
    .hooks_బహిర్గతం_fd = -1,
    .hooks_err_fd = -1,
    .seccomp_receiver_fd = -1,
    .custom_handler = ఖాళీ,
  };
  cleanup_close పూర్ణాంకం cgroup_dirfd = -1;
  నిర్మాణం libcrun_dirfd_s cgroup_dirfd_s;
  నిర్మాణం libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  స్థిరరాశి అక్షరం *seccomp_bpf_దత్తాంశం = కనుగొను_annotation (container, "run.oci.seccomp_bpf_data");
  పూర్ణాంకం cgroup_బహుళకం;

  ఒకవేళ (! context->force_no_cgroup)
    {
      cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
      ఒకవేళ (UNLIKELY (cgroup_బహుళకం < 0))
        తిరిగి cgroup_బహుళకం;

      ఒకవేళ (cgroup_బహుళకం != CGROUP_బహుళకం_UNIFIED)
        libcrun_warning ("cgroup v1 is deprecated and will be removed in a future release.  Use cgroup v2");
    }

  ret = అమరిక_container_hooks_output (container, def, &container_వాదనలు, &hooks_బహిర్గతం_fd, &hooks_err_fd, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  container->context = context;

  ఒకవేళ (! detach || context->notify_socket)
    {
      libcrun_debug ("Setting child subreaper");
      ret = prctl (PR_సమితి_బిడ్డ_SUBREAPER, 1, 0, 0, 0);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "prctl set child subreaper");
    }

  ret = అమరిక_container_keyring (container, context, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = అమరిక_terminal_socket_pair (container, context, &container_వాదనలు, &socket_pair_0, &socket_pair_1, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = ఖండం_సంకేతాలు (err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  umask (0);

  ret = libcrun_సమితి_mempolicy (def, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = అమరిక_seccomp (container, seccomp_bpf_దత్తాంశం, &seccomp_gen_ctx, &seccomp_fd, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;
  container_వాదనలు.seccomp_fd = seccomp_fd;

  ఒకవేళ (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &container_వాదనలు.seccomp_receiver_fd, &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ret = అమరిక_console_socket (context, def, &container_వాదనలు, &console_socket_fd, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = అమరిక_cgroup_manager (context, container, &cg, &cgroup_dirfd, &cgroup_dirfd_s, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_configure_handler (container_వాదనలు.context->handler_manager,
                                   container_వాదనలు.context,
                                   container,
                                   &(container_వాదనలు.custom_handler),
                                   err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (container_వాదనలు.custom_handler && container_వాదనలు.custom_handler->vtable->modify_oci_configuration)
    {
      libcrun_debug ("Using custom handler to modify OCI configuration");
      ret = container_వాదనలు.custom_handler->vtable->modify_oci_configuration (container_వాదనలు.custom_handler->cookie,
                                                                             container_వాదనలు.context,
                                                                             container->container_def,
                                                                             err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  pid = libcrun_నిరంతరం_linux_container (container, container_init, &container_వాదనలు, &sync_socket, &cgroup_dirfd_s, err);
  ఒకవేళ (UNLIKELY (pid < 0))
    తిరిగి pid;

  cg.pid = pid;
  cg.joined = cgroup_dirfd_s.joined;
  libcrun_debug ("Running container on PID: `%d`", pid);

  ఒకవేళ (context->fifo_exec_wait_fd < 0 && context->notify_socket)
    {
      libcrun_debug ("Using notify socket: `%s`", context->notify_socket);
      /* Do not open the notify socket here on "create".  "start" will take care of it.  */
      ret = get_notify_fd (context, container, &notify_socket, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        వెళ్ళు fail;
    }

  ఒకవేళ (container_వాదనలు.terminal_socketpair[1] >= 0)
    close_and_reset (&socket_pair_1);

  ret = libcrun_cgroup_enter (&cg, &cgroup_స్థితి, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు fail;

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు fail;

  ret = libcrun_move_జాలం_devices (container, pid, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు fail;

  /* sync send own pid.  */
  ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (sync_socket, &pid, పరిమాణం (pid)));
  ఒకవేళ (UNLIKELY (ret != పరిమాణం (pid)))
    {
      ఒకవేళ (ret >= 0)
        errno = 0;
      crun_make_error (err, errno, "write to sync socket");
      వెళ్ళు fail;
    }

  /* sync 1.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు fail;

  /* sync 2.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు fail;

  ret = libcrun_cgroup_enter_finalize (&cg, cgroup_స్థితి, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు fail;

  /* If the container process is created later, the scheduler is set on it then.  */
  ఒకవేళ (! libcrun_container_delayed_pidns_fork (container))
    {
      ret = సమితి_scheduler (pid, def, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        వెళ్ళు fail;
    }

  /* The container is waiting that we write back.  In this phase we can launch the
     prestart hooks.  */
  ఒకవేళ (def->hooks && def->hooks->prestart_len)
    {
      libcrun_debug ("Running `prestart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "prestart", (hook **) def->hooks->prestart,
                      def->hooks->prestart_len, hooks_బహిర్గతం_fd, hooks_err_fd, false, err);
      ఒకవేళ (UNLIKELY (ret != 0))
        వెళ్ళు fail;
    }
  ఒకవేళ (def->hooks && def->hooks->create_runtime_len)
    {
      libcrun_debug ("Running `create` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "createRuntime", (hook **) def->hooks->create_runtime,
                      def->hooks->create_runtime_len, hooks_బహిర్గతం_fd, hooks_err_fd, false, err);
      ఒకవేళ (UNLIKELY (ret != 0))
        వెళ్ళు fail;
    }

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_దత్తాంశం, &seccomp_gen_ctx, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు fail;
  close_and_reset (&seccomp_fd);

  /* sync 3.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు fail;

  /* The process joining an existing PID namespace is created once the container root is in place.  */
  ఒకవేళ (libcrun_container_delayed_pidns_fork (container))
    {
      pid_t new_pid = 0;

      ret = sync_socket_wait_sync (context, sync_socket, false, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        వెళ్ళు fail;

      ret = తాత్కాలిక_FAILURE_RETRY (పఠ (sync_socket, &new_pid, పరిమాణం (new_pid)));
      ఒకవేళ (UNLIKELY (ret != పరిమాణం (new_pid)))
        {
          ఒకవేళ (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "read pid from sync socket");
          వెళ్ళు fail;
        }

      /* Cleanup the first process.  */
      waitpid_ignore_stopped (pid, ఖాళీ, 0);

      pid = new_pid;

      ret = సమితి_scheduler (pid, def, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        వెళ్ళు fail;

      ret = libcrun_handle_pidfd_receiver (pid, container, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        వెళ్ళు fail;

      /* sync send own pid.  */
      ret = తాత్కాలిక_FAILURE_RETRY (లిఖ (sync_socket, &pid, పరిమాణం (pid)));
      ఒకవేళ (UNLIKELY (ret != పరిమాణం (pid)))
        {
          ఒకవేళ (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "write to sync socket");
          వెళ్ళు fail;
        }
    }

  ret = terminal_అమరిక (def, context, socket_pair_0, &terminal_fd, &orig_terminal, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు fail;

  close_and_reset (&socket_pair_0);

  /* sync 4.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు fail;

  ret = close_and_reset (&sync_socket);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      crun_make_error (err, errno, "close sync_socket failed");
      వెళ్ళు fail;
    }

  libcrun_debug ("Writing container status");
  ret = write_container_స్థితి (container, context, pid, cgroup_స్థితి, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు fail;

  /* Run poststart hooks here only if the container is created using "run".  For create+start, the
     hooks will be executed as part of the start command.  */
  ఒకవేళ (context->fifo_exec_wait_fd < 0 && def->hooks && def->hooks->poststart_len)
    {
      libcrun_debug ("Running `poststart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_బహిర్గతం_fd, hooks_err_fd, false, err);
      ఒకవేళ (UNLIKELY (ret != 0))
        వెళ్ళు fail;
    }

  /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
  ఒకవేళ (own_seccomp_receiver_fd >= 0)
    {
      libcrun_debug ("Receiving seccomp fd");
      seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
      ఒకవేళ (UNLIKELY (seccomp_notify_fd < 0))
        వెళ్ళు fail;

      ret = close_and_reset (&own_seccomp_receiver_fd);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          crun_make_error (err, errno, "close seccomp receiver fd failed");
          వెళ్ళు fail;
        }
    }

  {
    నిర్మాణం wait_for_process_వాదనలు వాదనలు = {
      .pid = pid,
      .context = context,
      .terminal_fd = terminal_fd,
      .notify_socket = notify_socket,
      .container_ready_fd = container_ready_fd,
      .seccomp_notify_fd = seccomp_notify_fd,
      .seccomp_notify_plugins = seccomp_notify_plugins,
    };
    ret = wait_for_process (&వాదనలు, err);
  }
  ఒకవేళ (! context->detach)
    {
      libcrun_error_t tmp_err = ఖాళీ;
      cleanup_watch (context, def, cgroup_స్థితి, 0, sync_socket, terminal_fd, &tmp_err);
      crun_error_release (&tmp_err);
    }

  తిరిగి ret;

fail:
  ret = cleanup_watch (context, def, cgroup_స్థితి, pid, sync_socket, terminal_fd, err);
  ఒకవేళ (cgroup_స్థితి)
    {
      libcrun_error_t tmp_err = ఖాళీ;
      libcrun_cgroup_destroy (cgroup_స్థితి, &tmp_err);
      crun_error_release (&tmp_err);
    }
  తిరిగి ret;
}

స్థిర పూర్ణాంకం
check_config_file (runtime_spec_schema_config_schema *def, libcrun_context_t *context, libcrun_error_t *err)
{
  ఒకవేళ (UNLIKELY (def->linux == ఖాళీ))
    తిరిగి crun_make_error (err, 0, "invalid config file, no `linux` block specified");

  ఒకవేళ (context->handler == ఖాళీ)
    {
      ఒకవేళ (UNLIKELY (def->మూలం == ఖాళీ))
        తిరిగి crun_make_error (err, 0, "invalid config file, no `root` block specified");
      ఒకవేళ (UNLIKELY (def->mounts == ఖాళీ))
        తిరిగి crun_make_error (err, 0, "invalid config file, no `mounts` block specified");
    }
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
libcrun_copy_config_file (స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *state_మూలం, libcrun_container_t *container, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  cleanup_free అక్షరం *dest_మార్గం = ఖాళీ;
  cleanup_free అక్షరం *dir = ఖాళీ;
  cleanup_free అక్షరం *బఫరు = ఖాళీ;
  size_t len;

  ret = libcrun_get_state_directory (&dir, state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = append_paths (&dest_మార్గం, err, dir, "config.json", ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (container->config_file == ఖాళీ && container->config_file_content == ఖాళీ)
    తిరిగి crun_make_error (err, 0, "config file not specified");

  ఒకవేళ (container->config_file == ఖాళీ)
    {
      libcrun_debug ("Writing config file to: `%s`", dest_మార్గం);
      ret = write_file (dest_మార్గం, container->config_file_content, అక్షరశ్రేణి_పొడవు (container->config_file_content), err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  లేకపోతే
    {
      libcrun_debug ("Reading config file: `%s`", container->config_file);
      ret = read_all_file (container->config_file, &బఫరు, &len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      libcrun_debug ("Writing config file to: `%s`", dest_మార్గం);
      ret = write_file (dest_మార్గం, బఫరు, len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  తిరిగి 0;
}

స్థిర శూన్య
force_delete_container_స్థితి (libcrun_context_t *context, libcrun_container_t *container,
                               runtime_spec_schema_config_schema *def)
{
  libcrun_error_t tmp_err = ఖాళీ;
  container_delete_అంతర్గత (context, container, def, context->id, true, false, &tmp_err);
  libcrun_error_report_and_release (&tmp_err);
}

/* Serialize ERR to FD as an int errno followed by a NUL-terminated message.
   Used by forked children to report a failure to the parent process.  */
స్థిర శూన్య
write_error_గమ్యం_pipe (పూర్ణాంకం fd, libcrun_error_t *err)
{
  స్థిరరాశి అక్షరం *సందేశం = (*err)->సందేశం;
  పూర్ణాంకం errcode = crun_error_get_errno (err);

  తాత్కాలిక_FAILURE_RETRY (లిఖ (fd, &errcode, పరిమాణం (errcode)));
  తాత్కాలిక_FAILURE_RETRY (లిఖ (fd, సందేశం, అక్షరశ్రేణి_పొడవు (సందేశం) + 1));
}

పూర్ణాంకం
libcrun_container_నిరంతరం (libcrun_context_t *context, libcrun_container_t *container, గుర్తులేని పూర్ణాంకం options,
                       libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  పూర్ణాంకం ret;
  పూర్ణాంకం detach = context->detach;
  పూర్ణాంకం container_ret_స్థితి[2];
  cleanup_close పూర్ణాంకం pipefd0 = -1;
  cleanup_close పూర్ణాంకం pipefd1 = -1;
  libcrun_error_t tmp_err = ఖాళీ;

  container->context = context;

  ret = validate_options (options, LIBCRUN_నిరంతరం_OPTIONS_PREFORK | LIBCRUN_నిరంతరం_OPTIONS_KEEP, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = check_config_file (def, context, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (def->process && def->process->terminal && detach && context->console_socket == ఖాళీ)
    తిరిగి crun_make_error (err, 0, "use --console-socket with --detach when a terminal is used");

  ret = libcrun_స్థితి_check_directories (context->state_మూలం, context->id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (! detach && (options & LIBCRUN_నిరంతరం_OPTIONS_PREFORK) == 0)
    {
      ret = libcrun_copy_config_file (context->id, context->state_మూలం, container, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = libcrun_container_నిరంతరం_అంతర్గత (container, context, ఖాళీ, err);
      ఒకవేళ (! (options & LIBCRUN_నిరంతరం_OPTIONS_KEEP))
        force_delete_container_స్థితి (context, container, def);
      తిరిగి ret;
    }

  ret = pipe2 (container_ret_స్థితి, O_CLOEXEC);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_స్థితి[0];
  pipefd1 = container_ret_స్థితి[1];

  ret = fork ();
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "fork");
  ఒకవేళ (ret)
    {
      పూర్ణాంకం స్థితి_2;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, ఖాళీ, 0);

      ret = తాత్కాలిక_FAILURE_RETRY (పఠ (pipefd0, &స్థితి_2, పరిమాణం (స్థితి_2)));
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, errno, "read from sync pipe");

      ఒకవేళ (స్థితి_2 < 0)
        {
          పూర్ణాంకం errno_;
          అక్షరం buf[512];
          ret = తాత్కాలిక_FAILURE_RETRY (పఠ (pipefd0, &errno_, పరిమాణం (errno_)));
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "read from sync pipe");

          ret = తాత్కాలిక_FAILURE_RETRY (పఠ (pipefd0, buf, పరిమాణం (buf) - 1));
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "read from sync pipe");
          buf[ret] = '\0';

          తిరిగి crun_make_error (err, errno_, "%s", buf);
        }

      తిరిగి స్థితి_2;
    }

  close_and_reset (&pipefd0);

  /* forked process.  */
  ret = detach_process ();
  ఒకవేళ (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_మూలం, container, &tmp_err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు fail;

  ret = libcrun_container_నిరంతరం_అంతర్గత (container, context, ఖాళీ, &tmp_err);
  తాత్కాలిక_FAILURE_RETRY (లిఖ (pipefd1, &ret, పరిమాణం (ret)));
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు fail;

  నిష్క్రమించు (EXIT_SUCCESS);
fail:

  ఒకవేళ (! (options & LIBCRUN_నిరంతరం_OPTIONS_KEEP))
    force_delete_container_స్థితి (context, container, def);
  ఒకవేళ (tmp_err)
    {
      write_error_గమ్యం_pipe (pipefd1, &tmp_err);
      crun_error_release (&tmp_err);
    }

  నిష్క్రమించు (EXIT_FAILURE);
}

పూర్ణాంకం
libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container, గుర్తులేని పూర్ణాంకం options,
                          libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  పూర్ణాంకం ret;
  పూర్ణాంకం container_ready_pipe[2];
  cleanup_close పూర్ణాంకం pipefd0 = -1;
  cleanup_close పూర్ణాంకం pipefd1 = -1;
  cleanup_close పూర్ణాంకం exec_fifo_fd = -1;
  context->detach = 1;

  libcrun_debug ("Creating container: `%s`", context->id);
  container->context = context;

  ret = validate_options (options, LIBCRUN_CREATE_OPTIONS_PREFORK, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = check_config_file (def, context, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (def->process && def->process->terminal && context->console_socket == ఖాళీ)
    తిరిగి crun_make_error (err, 0, "use --console-socket with create when a terminal is used");

  ret = libcrun_స్థితి_check_directories (context->state_మూలం, context->id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  exec_fifo_fd = libcrun_స్థితి_create_exec_fifo (context->state_మూలం, context->id, err);
  ఒకవేళ (UNLIKELY (exec_fifo_fd < 0))
    తిరిగి exec_fifo_fd;

  context->fifo_exec_wait_fd = exec_fifo_fd;
  exec_fifo_fd = -1;

  ఒకవేళ ((options & LIBCRUN_నిరంతరం_OPTIONS_PREFORK) == 0)
    {
      libcrun_debug ("Running with prefork enabled");
      ret = libcrun_copy_config_file (context->id, context->state_మూలం, container, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
      ret = libcrun_container_నిరంతరం_అంతర్గత (container, context, ఖాళీ, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        force_delete_container_స్థితి (context, container, def);
      తిరిగి ret;
    }

  ret = pipe2 (container_ready_pipe, O_CLOEXEC);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "pipe");
  pipefd0 = container_ready_pipe[0];
  pipefd1 = container_ready_pipe[1];

  ret = fork ();
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "fork");
  ఒకవేళ (ret)
    {
      పూర్ణాంకం exit_సంకేతం;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, ఖాళీ, 0);

      ret = తాత్కాలిక_FAILURE_RETRY (పఠ (pipefd0, &exit_సంకేతం, పరిమాణం (exit_సంకేతం)));
      ఒకవేళ (UNLIKELY (ret <= 0))
        తిరిగి crun_make_error (err, ret < 0 ? errno : 0, "waiting for container to be ready");

      ఒకవేళ (ret != పరిమాణం (exit_సంకేతం) || exit_సంకేతం < 0)
        తిరిగి crun_make_error (err, 0, "internal error: read invalid exit code");

      ఒకవేళ (exit_సంకేతం == 0)
        తిరిగి 0;

      libcrun_debug ("Exit code is `%d`, deleting container", exit_సంకేతం);
      libcrun_error_t tmp_err = ఖాళీ;
      container_delete_అంతర్గత (context, ఖాళీ, def, context->id, true, true, &tmp_err);
      libcrun_error_report_and_release (&tmp_err);
      తిరిగి crun_make_error (err, 0, "error creating container");
    }

  /* forked process.  */
  ret = detach_process ();
  ఒకవేళ (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_మూలం, container, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      పూర్ణాంకం errcode = crun_error_get_errno (err);
      crun_error_release (err);
      libcrun_fail_with_error (errcode, "copy config file");
    }

  ret = libcrun_container_నిరంతరం_అంతర్గత (container, context, &pipefd1, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      force_delete_container_స్థితి (context, container, def);
      libcrun_error ((*err)->స్థితి_2, "%s", (*err)->సందేశం);
      crun_error_release (err);
      crun_సమితి_output_handler (log_write_గమ్యం_stderr, ఖాళీ);
    }

  ఒకవేళ (pipefd1 >= 0)
    తాత్కాలిక_FAILURE_RETRY (లిఖ (pipefd1, &ret, పరిమాణం (ret)));
  నిష్క్రమించు (ret ? EXIT_FAILURE : 0);
}

పూర్ణాంకం
libcrun_container_ప్రారంభం (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = ఖాళీ;
  స్థిరరాశి అక్షరం *state_మూలం = context->state_మూలం;
  runtime_spec_schema_config_schema *def;
  cleanup_container_స్థితి libcrun_container_స్థితి_t స్థితి_2 = {};
  cleanup_close పూర్ణాంకం fd = -1;
  పూర్ణాంకం ret;

  ret = libcrun_read_container_స్థితి (&స్థితి_2, state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_is_container_running (&స్థితి_2, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (! ret)
    తిరిగి crun_make_error (err, 0, "container `%s` is not running", id);

  ret = read_container_config_from_state (&container, state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (context->notify_socket)
    {
      ret = get_notify_fd (context, container, &fd, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ret = libcrun_స్థితి_write_exec_fifo (context->state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  def = container->container_def;

  ఒకవేళ (context->notify_socket)
    {
      ఒకవేళ (fd >= 0)
        {
          fd_set read_సమితి;

          అయితే (1)
            {
              నిర్మాణం timeval timeout = {
                .tv_sec = 0,
                .tv_usec = 10000,
              };
              FD_ZERO (&read_సమితి);
              FD_SET (fd, &read_సమితి);

              ret = select (fd + 1, &read_సమితి, ఖాళీ, ఖాళీ, &timeout);
              ఒకవేళ (UNLIKELY (ret < 0))
                తిరిగి crun_make_error (err, errno, "select failed");
              ఒకవేళ (ret)
                {
                  ret = handle_notify_socket (fd, err);
                  ఒకవేళ (UNLIKELY (ret < 0))
                    తిరిగి ret;
                  ఒకవేళ (ret)
                    ఆపు;
                }
              లేకపోతే
                {
                  ret = libcrun_is_container_running (&స్థితి_2, err);
                  ఒకవేళ (UNLIKELY (ret < 0))
                    తిరిగి ret;
                  ఒకవేళ (! ret)
                    తిరిగి 0;
                }
            }
        }
    }

  /* The container is considered running only after we got the notification from the
     notify_socket, if any.  */
  ఒకవేళ (def->hooks && def->hooks->poststart_len)
    {
      cleanup_close పూర్ణాంకం hooks_బహిర్గతం_fd = -1;
      cleanup_close పూర్ణాంకం hooks_err_fd = -1;

      ret = open_hooks_output (container, &hooks_బహిర్గతం_fd, &hooks_err_fd, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = do_hooks (def, స్థితి_2.pid, context->id, false, స్థితి_2.bundle, స్థితి_2.bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_బహిర్గతం_fd, hooks_err_fd, false, err);
      ఒకవేళ (UNLIKELY (ret != 0))
        {
          libcrun_error_t tmp_err = ఖాళీ;
          container_delete_అంతర్గత (context, container, def, id, true, true, &tmp_err);
          libcrun_error_report_and_release (&tmp_err);
          తిరిగి ret;
        }
    }

  తిరిగి 0;
}

పూర్ణాంకం
libcrun_get_container_state_string (స్థిరరాశి అక్షరం *id, libcrun_container_స్థితి_t *స్థితి_2, స్థిరరాశి అక్షరం *state_మూలం,
                                    స్థిరరాశి అక్షరం **container_స్థితి, పూర్ణాంకం *running, libcrun_error_t *err)
{
  పూర్ణాంకం ret, has_fifo = 0;
  bool paused = false;

  ret = libcrun_is_container_running (స్థితి_2, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;
  *running = ret;

  ఒకవేళ (*running)
    {
      ret = libcrun_స్థితి_has_read_exec_fifo (state_మూలం, id, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
      has_fifo = ret;
    }

  ఒకవేళ (*running && ! has_fifo)
    {
      cleanup_cgroup_స్థితి నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి = ఖాళీ;

      cgroup_స్థితి = libcrun_cgroup_make_స్థితి (స్థితి_2);

      ret = libcrun_cgroup_is_container_paused (cgroup_స్థితి, &paused, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          /*
            The cgroup might have been cleaned up by the time we try to read it, ignore both
            ENOENT and ENODEV:
            - ENOENT: if the open(CGROUP_PATH) fails because the cgroup was deleted.
            - ENODEV: if the cgroup is deleted between the open and reading the freeze status.
          */

          errno = crun_error_get_errno (err);
          ఒకవేళ (errno == ENOENT || errno == ENODEV)
            {
              crun_error_release (err);
              *container_స్థితి = "stopped";
              తిరిగి 0;
            }

          తిరిగి ret;
        }
    }

  ఒకవేళ (! *running)
    *container_స్థితి = "stopped";
  లేకపోతే ఒకవేళ (has_fifo)
    *container_స్థితి = "created";
  లేకపోతే ఒకవేళ (paused)
    *container_స్థితి = "paused";
  లేకపోతే
    *container_స్థితి = "running";

  తిరిగి 0;
}

పూర్ణాంకం
libcrun_container_state_json (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, అక్షరం **బహిర్గతం, libcrun_error_t *err)
{
  అక్షరం *బఫరు = ఖాళీ;
  size_t len = 0;
  FILE *f;
  పూర్ణాంకం ret;

  f = open_memstream (&బఫరు, &len);
  ఒకవేళ (f == ఖాళీ)
    తిరిగి crun_make_error (err, errno, "open_memstream");

  ret = libcrun_container_state (context, id, f, err);
  fclose (f);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      విడుదల (బఫరు);
      తిరిగి ret;
    }

  *బహిర్గతం = బఫరు;
  తిరిగి 0;
}

పూర్ణాంకం
libcrun_container_state (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, FILE *బహిర్గతం, libcrun_error_t *err)
{
  స్థిరరాశి అక్షరం *స్థిరరాశి OCI_CONFIG_సంస్కరణ = "1.0.0";
  libcrun_container_స్థితి_t స్థితి_2 = {};
  స్థిరరాశి అక్షరం *state_మూలం = context->state_మూలం;
  స్థిరరాశి అక్షరం *container_స్థితి = ఖాళీ;
  json_gen_ctx *gen = ఖాళీ;
  స్థిరరాశి అక్షరం *buf;
  పూర్ణాంకం ret = 0;
  పూర్ణాంకం r = json_gen_స్థితి_ok;
  పూర్ణాంకం running;
  size_t len;

  ret = libcrun_read_container_స్థితి (&స్థితి_2, state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_get_container_state_string (id, &స్థితి_2, state_మూలం, &container_స్థితి, &running, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు నిష్క్రమించు;

  ret = 0;
  ఒకవేళ (! json_gen_init (&gen, ఖాళీ))
    తిరిగి crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));
  GEN_కీలకం (gen, "ociVersion");
  GEN_STR (gen, OCI_CONFIG_సంస్కరణ);

  GEN_కీలకం (gen, "id");
  GEN_STR (gen, id);

  GEN_కీలకం (gen, "pid");
  GEN_OR_FAIL (map_int (gen, running ? స్థితి_2.pid : 0));

  GEN_కీలకం (gen, "status");
  GEN_STR (gen, container_స్థితి);

  GEN_కీలకం (gen, "bundle");
  GEN_STR (gen, స్థితి_2.bundle);

  GEN_కీలకం (gen, "rootfs");
  GEN_STR (gen, స్థితి_2.rootfs);

  GEN_కీలకం (gen, "created");
  GEN_STR (gen, స్థితి_2.created);

  ఒకవేళ (స్థితి_2.scope)
    {
      GEN_కీలకం (gen, "systemd-scope");
      GEN_STR (gen, స్థితి_2.scope);
    }

  ఒకవేళ (స్థితి_2.owner)
    {
      GEN_కీలకం (gen, "owner");
      GEN_STR (gen, స్థితి_2.owner);
    }

  {
    cleanup_container libcrun_container_t *container = ఖాళీ;

    ret = read_container_config_from_state (&container, state_మూలం, id, err);
    ఒకవేళ (UNLIKELY (ret < 0))
      వెళ్ళు నిష్క్రమించు;

    ఒకవేళ (container->container_def->annotations && container->container_def->annotations->len)
      GEN_OR_FAIL (gen_annotations (gen, container->container_def->annotations));
  }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  fprintf (బహిర్గతం, "%s\n", buf);

నిష్క్రమించు:
  ఒకవేళ (gen)
    json_gen_free (gen);
  libcrun_free_container_స్థితి (&స్థితి_2);
  తిరిగి ret;

gen_error:
  ret = json_gen_error_గమ్యం_crun_error (r, err);
  వెళ్ళు నిష్క్రమించు;
}

పూర్ణాంకం
libcrun_container_exec (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, runtime_spec_schema_config_schema_process *process,
                        libcrun_error_t *err)
{
  నిర్మాణం libcrun_container_exec_options_s opts;
  memset (&opts, 0, పరిమాణం (opts));

  opts.struct_కొలత = పరిమాణం (opts);
  opts.process = process;

  తిరిగి libcrun_container_exec_with_options (context, id, &opts, err);
}

పూర్ణాంకం
libcrun_container_exec_process_file (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err)
{
  నిర్మాణం libcrun_container_exec_options_s opts;
  memset (&opts, 0, పరిమాణం (opts));

  opts.struct_కొలత = పరిమాణం (opts);
  opts.మార్గం = మార్గం;

  తిరిగి libcrun_container_exec_with_options (context, id, &opts, err);
}

స్థిర పూర్ణాంకం
exec_from_process_json (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *process_json, స్థిరరాశి అక్షరం *cgroup,
                        bool విలీనం_env, libcrun_error_t *err)
{
  నిర్మాణం parser_context parser_ctx = { .options = 0, .errfile = stderr };
  నిర్మాణం libcrun_container_exec_options_s opts;
  runtime_spec_schema_config_schema_process *process;
  parser_error parser_err = ఖాళీ;
  json_object *doc = ఖాళీ;
  పూర్ణాంకం ret;

  ret = parse_json_file (&doc, process_json, &parser_ctx, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  process = make_runtime_spec_schema_config_schema_process (doc, &parser_ctx, &parser_err);
  json_object_put (doc);
  ఒకవేళ (process == ఖాళీ)
    {
      ret = crun_make_error (err, 0, "cannot parse process: `%s`", parser_err ? parser_err : "unknown error");
      విడుదల (parser_err);
      తిరిగి ret;
    }

  memset (&opts, 0, పరిమాణం (opts));
  opts.struct_కొలత = పరిమాణం (opts);
  opts.process = process;
  opts.cgroup = cgroup;
  opts.విలీనం_env = విలీనం_env;

  ret = libcrun_container_exec_with_options (context, id, &opts, err);
  free_runtime_spec_schema_config_schema_process (process);
  తిరిగి ret;
}

పూర్ణాంకం
libcrun_container_exec_json (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *process_json, libcrun_error_t *err)
{
  తిరిగి exec_from_process_json (context, id, process_json, ఖాళీ, false, err);
}

పూర్ణాంకం
libcrun_container_exec_with_options_json (libcrun_context_t *context, స్థిరరాశి అక్షరం *id,
                                          స్థిరరాశి నిర్మాణం libcrun_exec_options_s *opts, libcrun_error_t *err)
{
  ఒకవేళ (opts == ఖాళీ || opts->struct_కొలత == 0)
    తిరిగి crun_make_error (err, EINVAL, "invalid exec options");

  తిరిగి exec_from_process_json (context, id, opts->process_json, opts->cgroup, opts->విలీనం_env, err);
}

#నిర్వచించు cleanup_process_schema __attribute__ ((cleanup (cleanup_process_schemap)))

స్థిర అంతఃవరుస శూన్య
cleanup_process_schemap (runtime_spec_schema_config_schema_process **p)
{
  runtime_spec_schema_config_schema_process *process = *p;
  ఒకవేళ (process)
    (శూన్య) free_runtime_spec_schema_config_schema_process (process);
}

/* Build the seccomp receiver payload (when needed), apply the seccomp profile and
   close the seccomp file descriptors.  Used from exec_process_entrypoint both before
   and after the capabilities are set, depending on process->no_new_privileges.  */
స్థిర పూర్ణాంకం
apply_seccomp_for_exec (libcrun_container_t *container, pid_t own_pid, పూర్ణాంకం seccomp_fd,
                        పూర్ణాంకం seccomp_receiver_fd, అక్షరం **seccomp_flags, size_t seccomp_flags_len,
                        libcrun_error_t *err)
{
  cleanup_free అక్షరం *seccomp_fd_payload = ఖాళీ;
  size_t seccomp_fd_payload_len = 0;
  పూర్ణాంకం ret;

  ఒకవేళ (seccomp_receiver_fd >= 0)
    {
      ret = get_seccomp_receiver_fd_payload (container, "running", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ret = libcrun_apply_seccomp (seccomp_fd, seccomp_receiver_fd, seccomp_fd_payload,
                               seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  close_and_reset (&seccomp_fd);
  close_and_reset (&seccomp_receiver_fd);
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
exec_process_entrypoint (libcrun_context_t *context,
                         libcrun_container_t *container,
                         runtime_spec_schema_config_schema_process *process,
                         పూర్ణాంకం *pipefd1,
                         పూర్ణాంకం seccomp_fd,
                         పూర్ణాంకం seccomp_receiver_fd,
                         నిర్మాణం custom_handler_instance_s *custom_handler,
                         bool విలీనం_env,
                         libcrun_error_t *err)
{
  runtime_spec_schema_config_schema_process_capabilities *capabilities = ఖాళీ;
  cleanup_free అక్షరం *exec_మార్గం = ఖాళీ;
  uid_t container_uid;
  gid_t container_gid;
  స్థిరరాశి అక్షరం *cwd;
  bool chdir_పూర్తి = false;
  size_t seccomp_flags_len = 0;
  అక్షరం **seccomp_flags = ఖాళీ;
  pid_t own_pid = 0;
  size_t i;
  పూర్ణాంకం ret;

  container_uid = process->user ? process->user->uid : 0;
  container_gid = process->user ? process->user->gid : 0;

  తాత్కాలిక_FAILURE_RETRY (పఠ (*pipefd1, &own_pid, పరిమాణం (own_pid)));

  cwd = process->cwd ? process->cwd : "/";
  ఒకవేళ (LIKELY (libcrun_సురక్షిత_chdir (cwd, err) == 0))
    chdir_పూర్తి = true;
  లేకపోతే
    crun_error_release (err);

  ret = unblock_సంకేతాలు (err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = clearenv ();
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "clearenv");

  ఒకవేళ ((విలీనం_env || ! process->env_len) && container->container_def->process && container->container_def->process->env_len)
    {
      అక్షరం *e;

      కొరకు (i = 0; i < container->container_def->process->env_len; i++)
        {
          e = container->container_def->process->env[i];
          ఒకవేళ (putenv (e) < 0)
            తిరిగి crun_make_error (err, errno, "putenv `%s`", e);
        }
    }
  ఒకవేళ (process->env_len)
    {
      కొరకు (i = 0; i < process->env_len; i++)
        ఒకవేళ (putenv (process->env[i]) < 0)
          తిరిగి crun_make_error (err, errno, "putenv `%s`", process->env[i]);
    }

  ఒకవేళ (is_ఖాళీలు_string (getenv ("HOME")))
    {
      ret = సమితి_home_env (container_uid);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          ఒకవేళ (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            తిరిగి crun_make_error (err, errno, "setenv HOME");
        }
    }

  ret = libcrun_సమితి_selinux_label (container, process, false, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_సమితి_apparmor_profile (container, process, false, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (container->container_def->linux && container->container_def->linux->seccomp)
    {
      seccomp_flags = container->container_def->linux->seccomp->flags;
      seccomp_flags_len = container->container_def->linux->seccomp->flags_len;
    }

  ret = కనుగొను_executable (&exec_మార్గం, process->వాదనలు[0], process->cwd, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ఒకవేళ (crun_error_get_errno (err) == ENOENT)
        తిరిగి ret;

      /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
         once the process switched to the use that runs in the container.  This might be necessary
         when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
         is not honored.  */
      crun_error_release (err);
    }

  ఒకవేళ (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_సమితి_personality (container->container_def->linux->personality, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, false, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_reopen_dev_null (err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (! process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ret = libcrun_container_setgroups (container, process, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = maybe_chown_std_streams (container_uid, container_gid, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (process->capabilities)
    capabilities = process->capabilities;
  లేకపోతే ఒకవేళ (container->container_def->process)
    capabilities = container->container_def->process->capabilities;

  ret = libcrun_సమితి_caps (capabilities, container_uid, container_gid, process->no_new_privileges,
                          custom_handler != ఖాళీ, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (UNLIKELY (exec_మార్గం == ఖాళీ))
    {
      ret = కనుగొను_executable (&exec_మార్గం, process->వాదనలు[0], process->cwd, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          ఒకవేళ (custom_handler == ఖాళీ || is_ఖాళీలు_string (process->వాదనలు[0]))
            తిరిగి ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          exec_మార్గం = xstrdup (process->వాదనలు[0]);
        }
    }

  ఒకవేళ (! chdir_పూర్తి)
    {
      ret = libcrun_సురక్షిత_chdir (cwd, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (process->user)
    umask (process->user->umask_present ? process->user->umask : 0022);

  ret = 0;
  తాత్కాలిక_FAILURE_RETRY (లిఖ (*pipefd1, &ret, పరిమాణం (ret)));
  తాత్కాలిక_FAILURE_RETRY (close (*pipefd1));
  *pipefd1 = -1;

  ఒకవేళ (custom_handler)
    {
      ఒకవేళ (custom_handler->vtable->exec_func == ఖాళీ)
        తిరిగి crun_make_error (err, 0, "the handler does not support exec");

      ఒకవేళ (context->argv)
        erase_initial_environment (context->argv, context->argc);

      ret = custom_handler->vtable->exec_func (custom_handler->cookie,
                                               container,
                                               exec_మార్గం,
                                               process->వాదనలు);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, -ret, "exec container process failed with handler as `%s`", custom_handler->vtable->name);

      సురక్షిత_exit (EXIT_FAILURE);
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation, because the seccomp filter is already in place and it could
     stop some syscalls used by mark_or_close_fds_ge_than.
  */
  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, true, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    crun_error_release (err);

  తాత్కాలిక_FAILURE_RETRY (execv (exec_మార్గం, process->వాదనలు));
  libcrun_fail_with_error (errno, "exec `%s`", exec_మార్గం);
}

/* Close the file descriptors inherited from the caller, other than those
   to be passed to the container process.  */
పూర్ణాంకం
libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err)
{
  తిరిగి mark_or_close_fds_ge_than (ఖాళీ, context->preserve_fds + 3, true, err);
}

పూర్ణాంకం
libcrun_ఎక్కించు_process_from_file (స్థిరరాశి అక్షరం *మార్గం, runtime_spec_schema_config_schema_process **process,
                                libcrun_error_t *err)
{
  నిర్మాణం parser_context ctx = { 0, stderr };
  cleanup_free అక్షరం *content = ఖాళీ;
  parser_error parser_err = ఖాళీ;
  json_object *doc = ఖాళీ;
  size_t len;
  పూర్ణాంకం ret;

  ret = read_all_file (మార్గం, &content, &len, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  *process = make_runtime_spec_schema_config_schema_process (doc, &ctx, &parser_err);
  ఒకవేళ (UNLIKELY (*process == ఖాళీ))
    {
      ret = crun_make_error (err, 0, "cannot parse process file: `%s`", parser_err);
      విడుదల (parser_err);
      ఒకవేళ (doc)
        json_object_put (doc);
      తిరిగి ret;
    }

  విడుదల (parser_err);
  ఒకవేళ (doc)
    json_object_put (doc);

  తిరిగి 0;
}

పూర్ణాంకం
libcrun_container_exec_with_options (libcrun_context_t *context, స్థిరరాశి అక్షరం *id,
                                     నిర్మాణం libcrun_container_exec_options_s *opts,
                                     libcrun_error_t *err)
{
  cleanup_custom_handler_instance నిర్మాణం custom_handler_instance_s *custom_handler = ఖాళీ;
  పూర్ణాంకం container_స్థితి, ret;
  bool container_paused = false;
  pid_t pid;
  libcrun_container_స్థితి_t స్థితి_2 = {};
  స్థిరరాశి అక్షరం *state_మూలం = context->state_మూలం;
  cleanup_close పూర్ణాంకం terminal_fd = -1;
  cleanup_close పూర్ణాంకం seccomp_fd = -1;
  cleanup_terminal శూన్య *orig_terminal = ఖాళీ;
  cleanup_container libcrun_container_t *container = ఖాళీ;
  పూర్ణాంకం container_ret_స్థితి[2];
  cleanup_close పూర్ణాంకం pipefd0 = -1;
  cleanup_close పూర్ణాంకం pipefd1 = -1;
  cleanup_close పూర్ణాంకం seccomp_receiver_fd = -1;
  cleanup_close పూర్ణాంకం own_seccomp_receiver_fd = -1;
  cleanup_close పూర్ణాంకం seccomp_notify_fd = -1;
  స్థిరరాశి అక్షరం *seccomp_notify_plugins = ఖాళీ;
  __attribute__ ((వాడని)) cleanup_process_schema runtime_spec_schema_config_schema_process *process_cleanup = ఖాళీ;
  runtime_spec_schema_config_schema_process *process = opts->process;
  నిర్మాణం libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  పూర్ణాంకం ret_from_బిడ్డ = 0;

  ret = libcrun_read_container_స్థితి (&స్థితి_2, state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_is_container_running (&స్థితి_2, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;
  container_స్థితి = ret;

  ret = read_container_config_from_state (&container, state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  container->context = context;

  ఒకవేళ (container_స్థితి == 0)
    తిరిగి crun_make_error (err, 0, "the container `%s` is not running", id);

  {
    cleanup_cgroup_స్థితి నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి = ఖాళీ;

    cgroup_స్థితి = libcrun_cgroup_make_స్థితి (&స్థితి_2);

    ret = libcrun_cgroup_is_container_paused (cgroup_స్థితి, &container_paused, err);
    ఒకవేళ (UNLIKELY (ret < 0))
      తిరిగి ret;
  }

  ఒకవేళ (UNLIKELY (container_paused))
    తిరిగి crun_make_error (err, 0, "cannot exec in a paused container `%s`", id);

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = ఖండం_సంకేతాలు (err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  libcrun_seccomp_gen_ctx_init (&seccomp_gen_ctx, container, false, 0);

  ret = libcrun_open_seccomp_bpf (&seccomp_gen_ctx, &seccomp_fd, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &seccomp_receiver_fd,
                                     &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ఒకవేళ (పరిమాణం (*opts) != opts->struct_కొలత)
    తిరిగి crun_make_error (err, EINVAL, "invalid libcrun_container_exec_options_s struct");

  ఒకవేళ (opts->మార్గం)
    {
      ఒకవేళ (process)
        తిరిగి crun_make_error (err, EINVAL, "cannot specify both exec file and options");

      ret = libcrun_ఎక్కించు_process_from_file (opts->మార్గం, &process, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      process_cleanup = process;
    }

  /* This must be done before we enter a user namespace.  If the process does
     not specify any rlimits, use the ones from the container configuration.  */
  ఒకవేళ (process->rlimits_len == 0 && container->container_def->process)
    ret = libcrun_సమితి_rlimits (container->container_def->process->rlimits,
                               container->container_def->process->rlimits_len, err);
  లేకపోతే
    ret = libcrun_సమితి_rlimits (process->rlimits, process->rlimits_len, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_సమితి_mempolicy (container->container_def, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = pipe2 (container_ret_స్థితి, O_CLOEXEC);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_స్థితి[0];
  pipefd1 = container_ret_స్థితి[1];

  /* If the new process block doesn't specify a SELinux label, AppArmor profile, user,
     I/O priority or scheduler, then use the configuration from the original config file.  */
  ఒకవేళ (container->container_def->process)
    {
      ఒకవేళ (process->io_ప్రాధాన్యత == ఖాళీ && container->container_def->process->io_ప్రాధాన్యత)
        {
          process->io_ప్రాధాన్యత = clone_runtime_spec_schema_config_schema_process_io_ప్రాధాన్యత (container->container_def->process->io_ప్రాధాన్యత);
          ఒకవేళ (process->io_ప్రాధాన్యత == ఖాళీ)
            OOM ();
        }

      ఒకవేళ (process->scheduler == ఖాళీ && container->container_def->process->scheduler)
        {
          process->scheduler = clone_runtime_spec_schema_config_schema_process_scheduler (container->container_def->process->scheduler);
          ఒకవేళ (process->scheduler == ఖాళీ)
            OOM ();
        }

      ఒకవేళ (process->selinux_label == ఖాళీ && container->container_def->process->selinux_label)
        process->selinux_label = xstrdup (container->container_def->process->selinux_label);

      ఒకవేళ (process->apparmor_profile == ఖాళీ && container->container_def->process->apparmor_profile)
        process->apparmor_profile = xstrdup (container->container_def->process->apparmor_profile);

      ఒకవేళ (process->user == ఖాళీ && container->container_def->process->user)
        {
          process->user = clone_runtime_spec_schema_config_schema_process_user (container->container_def->process->user);
          ఒకవేళ (process->user == ఖాళీ)
            OOM ();
        }
    }

  ret = initialize_security (container, process, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  /* Do not leak the session keyring of the caller into the container.  The
     keyring created for the container cannot be joined, so use an anonymous one.  */
  ఒకవేళ (! context->no_new_keyring && ! స్థితి_2.no_new_keyring)
    {
      libcrun_debug ("Creating new keyring for the exec process");

      ret = libcrun_create_keyring (container, ఖాళీ, process->selinux_label, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ret = prctl (PR_సమితి_DUMPABLE, 0, 0, 0, 0);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "prctl unset dumpable");

  pid = libcrun_join_process (context, container, స్థితి_2.pid, &స్థితి_2, opts->cgroup, context->detach,
                              process, process->terminal ? &terminal_fd : ఖాళీ, err);
  ఒకవేళ (UNLIKELY (pid < 0))
    తిరిగి pid;

  /* Process to exec.  */
  ఒకవేళ (pid == 0)
    {
      తాత్కాలిక_FAILURE_RETRY (close (pipefd0));
      pipefd0 = -1;

      exec_process_entrypoint (context, container, process, &pipefd1, seccomp_fd, seccomp_receiver_fd, custom_handler, opts->విలీనం_env, err);
      /* It gets here only on errors.  */
      ఒకవేళ (*err)
        {
          ఒకవేళ (pipefd1 < 0)
            libcrun_fail_with_error ((*err)->స్థితి_2, "%s", (*err)->సందేశం);
          లేకపోతే
            {
              write_error_గమ్యం_pipe (pipefd1, err);
              తాత్కాలిక_FAILURE_RETRY (close (pipefd1));
              pipefd1 = -1;
            }
        }
      సురక్షిత_exit (EXIT_FAILURE);
    }

  తాత్కాలిక_FAILURE_RETRY (close (pipefd1));
  pipefd1 = -1;

  తాత్కాలిక_FAILURE_RETRY (లిఖ (pipefd0, &pid, పరిమాణం (pid)));

  ఒకవేళ (seccomp_fd >= 0)
    close_and_reset (&seccomp_fd);

  ఒకవేళ (terminal_fd >= 0)
    {
      గుర్తులేని పొట్టి అడ్డువరుసలు = 0, నిలువుల_సంఖ్య = 0;

      ఒకవేళ (process->console_కొలత)
        {
          నిలువుల_సంఖ్య = process->console_కొలత->వెడల్పు;
          అడ్డువరుసలు = process->console_కొలత->ఎత్తు;
        }

      ret = libcrun_terminal_అమరిక_కొలత (terminal_fd, అడ్డువరుసలు, నిలువుల_సంఖ్య, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ఒకవేళ (context->console_socket)
        {
          పూర్ణాంకం ret;
          cleanup_close పూర్ణాంకం console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
          ఒకవేళ (UNLIKELY (console_socket_fd < 0))
            తిరిగి console_socket_fd;
          ret = send_fd_గమ్యం_socket (console_socket_fd, terminal_fd, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
          close_and_reset (&terminal_fd);
        }
      లేకపోతే
        {
          ret = libcrun_clear_onlcr (terminal_fd, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            {
              flush_fd_గమ్యం_err (context, terminal_fd);
              తిరిగి ret;
            }

          ret = libcrun_సమితి_raw (0, &orig_terminal, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            {
              flush_fd_గమ్యం_err (context, terminal_fd);
              తిరిగి ret;
            }
        }
    }

  ret = తాత్కాలిక_FAILURE_RETRY (పఠ (pipefd0, &ret_from_బిడ్డ, పరిమాణం (ret_from_బిడ్డ)));
  ఒకవేళ (ret != పరిమాణం (ret_from_బిడ్డ))
    ret = crun_make_error (err, 0, "read pipe failed");
  లేకపోతే ఒకవేళ (ret_from_బిడ్డ != 0)
    {
      cleanup_free అక్షరం *సందేశం = ఖాళీ;
      size_t len = 0;

      ret = read_all_fd (pipefd0, "error stream", &సందేశం, &len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
      /* the string from read_all_fd is always NUL terminated.  */
      ret = crun_make_error (err, ret_from_బిడ్డ, "%s", సందేశం);
    }
  లేకపోతే
    {
      /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
      ఒకవేళ (own_seccomp_receiver_fd >= 0)
        {
          seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
          ఒకవేళ (UNLIKELY (seccomp_notify_fd < 0))
            తిరిగి seccomp_notify_fd;

          ret = close_and_reset (&own_seccomp_receiver_fd);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "close seccomp receiver fd failed");
        }

      {
        నిర్మాణం wait_for_process_వాదనలు వాదనలు = {
          .pid = pid,
          .context = context,
          .terminal_fd = terminal_fd,
          .notify_socket = -1,
          .container_ready_fd = ఖాళీ,
          .seccomp_notify_fd = seccomp_notify_fd,
          .seccomp_notify_plugins = seccomp_notify_plugins,
        };

        ret = wait_for_process (&వాదనలు, err);
      }
    }

  flush_fd_గమ్యం_err (context, terminal_fd);
  తిరిగి ret;
}

పూర్ణాంకం
libcrun_container_update (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *content, size_t len arg_వాడని,
                          libcrun_error_t *err)
{
  cleanup_custom_handler_instance నిర్మాణం custom_handler_instance_s *custom_handler = ఖాళీ;
  runtime_spec_schema_config_linux_resources *resources = ఖాళీ;
  cleanup_container libcrun_container_t *container = ఖాళీ;
  స్థిరరాశి అక్షరం *state_మూలం = context->state_మూలం;
  నిర్మాణం parser_context ctx = { 0, stderr };
  libcrun_container_స్థితి_t స్థితి_2 = {};
  parser_error parser_err = ఖాళీ;
  json_object *doc = ఖాళీ;
  పూర్ణాంకం ret;

  ret = libcrun_read_container_స్థితి (&స్థితి_2, state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = read_container_config_from_state (&container, state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  resources = make_runtime_spec_schema_config_linux_resources (doc, &ctx, &parser_err);
  ఒకవేళ (UNLIKELY (resources == ఖాళీ))
    {
      ret = crun_make_error (err, 0, "cannot parse resources: %s", parser_err);
      వెళ్ళు cleanup;
    }

  ఒకవేళ (custom_handler && custom_handler->vtable->modify_oci_configuration)
    {
      /* Adapt RESOURCES to be used from the modify_oci_configuration hook.  */
      cleanup_free runtime_spec_schema_config_linux *linux = xmalloc0 (పరిమాణం (*linux));
      cleanup_free runtime_spec_schema_config_schema *def = xmalloc0 (పరిమాణం (*def));

      def->linux = linux;
      linux->resources = resources;

      ret = custom_handler->vtable->modify_oci_configuration (custom_handler->cookie,
                                                              context,
                                                              def,
                                                              err);
      ఒకవేళ (UNLIKELY (ret < 0))
        వెళ్ళు cleanup;
    }

  ret = libcrun_linux_container_update (&స్థితి_2, state_మూలం, resources, err);

cleanup:
  ఒకవేళ (doc)
    json_object_put (doc);
  విడుదల (parser_err);
  ఒకవేళ (resources)
    free_runtime_spec_schema_config_linux_resources (resources);

  తిరిగి ret;
}

పూర్ణాంకం
libcrun_container_update_from_file (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *file, libcrun_error_t *err)
{
  cleanup_free అక్షరం *content = ఖాళీ;
  size_t len;
  పూర్ణాంకం ret;

  ret = read_all_file (file, &content, &len, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి libcrun_container_update (context, id, content, len, err);
}

స్థిర పూర్ణాంకం
compare_update_విలువలు (స్థిరరాశి శూన్య *a, స్థిరరాశి శూన్య *b)
{
  స్థిరరాశి నిర్మాణం libcrun_update_విలువ_s *aa = a;
  స్థిరరాశి నిర్మాణం libcrun_update_విలువ_s *bb = b;
  పూర్ణాంకం ret;

  ret = strcmp (aa->section, bb->section);
  ఒకవేళ (ret)
    తిరిగి ret;
  తిరిగి strcmp (aa->name, bb->name);
}

పూర్ణాంకం
libcrun_container_update_from_విలువలు (libcrun_context_t *context, స్థిరరాశి అక్షరం *id,
                                      నిర్మాణం libcrun_update_విలువ_s *విలువలు, size_t len,
                                      libcrun_error_t *err)
{
  స్థిరరాశి అక్షరం *current_section = ఖాళీ;
  స్థిరరాశి అక్షరం *buf;
  json_gen_ctx *gen = ఖాళీ;
  size_t i, buf_len;
  పూర్ణాంకం ret;

  ఒకవేళ (! json_gen_init (&gen, ఖాళీ))
    తిరిగి crun_make_error (err, 0, "json_gen_init failed");
  json_gen_map_open (gen);

  qsort (విలువలు, len, పరిమాణం (నిర్మాణం libcrun_update_విలువ_s), compare_update_విలువలు);

  కొరకు (i = 0; i < len; i++)
    {
      ఒకవేళ (current_section == ఖాళీ || strcmp (విలువలు[i].section, current_section))
        {
          ఒకవేళ (i > 0)
            json_gen_map_close (gen);

          current_section = విలువలు[i].section;
          json_gen_string (gen, current_section, అక్షరశ్రేణి_పొడవు (current_section));
          json_gen_map_open (gen);
        }

      json_gen_string (gen, విలువలు[i].name, అక్షరశ్రేణి_పొడవు (విలువలు[i].name));

      ఒకవేళ (విలువలు[i].numeric)
        json_gen_సంఖ్య (gen, విలువలు[i].విలువ, అక్షరశ్రేణి_పొడవు (విలువలు[i].విలువ));
      లేకపోతే
        json_gen_string (gen, విలువలు[i].విలువ, అక్షరశ్రేణి_పొడవు (విలువలు[i].విలువ));
    }
  ఒకవేళ (len)
    json_gen_map_close (gen);

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);

  ret = libcrun_container_update (context, id, buf, buf_len, err);

  json_gen_free (gen);

  తిరిగి ret;
}

స్థిర శూన్య
populate_array_క్షేత్రం (అక్షరం ***క్షేత్రం, అక్షరం *array[], size_t అంకె_elements)
{
  size_t i;

  *క్షేత్రం = xmalloc0 ((అంకె_elements + 1) * పరిమాణం (అక్షరం *));
  కొరకు (i = 0; i < అంకె_elements; i++)
    (*క్షేత్రం)[i] = xstrdup (array[i]);

  (*క్షేత్రం)[i] = ఖాళీ;
}

#నిర్వచితమైతే HAVE_CAP
స్థిర శూన్య
populate_capabilities (నిర్మాణం features_info_s *info, అక్షరం ***capabilities, size_t *అంకె_capabilities)
{
  size_t సూచీ = 0;
  cap_విలువ_t i;
  అక్షరం *endptr;
  పూర్ణాంకం j;

  *అంకె_capabilities = 0;
  కొరకు (i = 0;; i++)
    {
      అక్షరం *v = cap_గమ్యం_name (i);
      ఒకవేళ (v == ఖాళీ)
        ఆపు;
      strtol (v, &endptr, 10);
      ఒకవేళ (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          ఆపు;
        }
      (*అంకె_capabilities)++;
    }

  *capabilities = xmalloc0 ((*అంకె_capabilities + 1) * పరిమాణం (స్థిరరాశి అక్షరం *));
  కొరకు (i = 0; i < (cap_విలువ_t) *అంకె_capabilities; i++)
    {
      అక్షరం *v = cap_గమ్యం_name (i);
      ఒకవేళ (v == ఖాళీ)
        ఆపు;
      strtol (v, &endptr, 10);
      ఒకవేళ (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          ఆపు;
        }

      // Convert capability name to uppercase
      కొరకు (j = 0; v[j] != '\0'; j++)
        v[j] = పెద్ద_అక్షరం (v[j]);

      (*capabilities)[సూచీ] = v;
      సూచీ++;
    }

  (*capabilities)[సూచీ] = ఖాళీ; // Terminate the array with NULL
  populate_array_క్షేత్రం (&(info->linux.capabilities), *capabilities, *అంకె_capabilities);
}
#అయితే_ముగింపు

స్థిర శూన్య
retrieve_mount_options (నిర్మాణం features_info_s **info)
{
  cleanup_free స్థిరరాశి నిర్మాణం ప్రసరణ_flags_s *mount_options_list = ఖాళీ;
  size_t అంకె_mount_options = 0;

  // Retrieve mount options from wordlist
  mount_options_list = get_mount_flags_from_wordlist ();

  // Calculate the number of mount options
  అయితే (mount_options_list[అంకె_mount_options].name != ఖాళీ)
    అంకె_mount_options++;

  // Allocate memory for mount options in info struct
  (*info)->mount_options = xmalloc0 ((అంకె_mount_options + 1) * పరిమాణం (అక్షరం *));

  // Copy mount options to info struct
  కొరకు (size_t i = 0; i < అంకె_mount_options; i++)
    (*info)->mount_options[i] = xstrdup (mount_options_list[i].name);
}

పూర్ణాంకం
libcrun_container_get_features (libcrun_context_t *context, నిర్మాణం features_info_s **info, libcrun_error_t *err arg_వాడని)
{
  // Allocate memory for the features_info_s structure
  size_t అంకె_namspaces = పరిమాణం (namespaces) / పరిమాణం (namespaces[0]);
  size_t అంకె_operators = పరిమాణం (operators) / పరిమాణం (operators[0]);
  size_t అంకె_actions = పరిమాణం (actions) / పరిమాణం (actions[0]);
  size_t అంకె_hooks = పరిమాణం (hooks) / పరిమాణం (hooks[0]);
  size_t అంకె_archs = పరిమాణం (archs) / పరిమాణం (archs[0]);
  size_t అంకె_unsafe_annotations = పరిమాణం (potentially_unsafe_annotations) / పరిమాణం (potentially_unsafe_annotations[0]);
  cleanup_free అక్షరం **capabilities = ఖాళీ;
  size_t అంకె_capabilities = 0;
  size_t అంకె_mempolicy_modes = పరిమాణం (mempolicy_modes) / పరిమాణం (mempolicy_modes[0]);
  size_t అంకె_mempolicy_flags = పరిమాణం (mempolicy_flags) / పరిమాణం (mempolicy_flags[0]);

  *info = xmalloc0 (పరిమాణం (నిర్మాణం features_info_s));

  // Hardcoded feature information
  (*info)->oci_సంస్కరణ_min = xstrdup ("1.0.0");
  (*info)->oci_సంస్కరణ_max = xstrdup ("1.1.0+dev");

  // Populate hooks
  populate_array_క్షేత్రం (&((*info)->hooks), hooks, అంకె_hooks);

  // Populate mount_options
  retrieve_mount_options (info);

  // Populate namespaces
  populate_array_క్షేత్రం (&((*info)->linux.namespaces), namespaces, అంకె_namspaces);

#నిర్వచితమైతే HAVE_CAP
  // Populate capabilities
  populate_capabilities (*info, &capabilities, &అంకె_capabilities);
#అయితే_ముగింపు

  // Hardcode the values for cgroup
  (*info)->linux.cgroup.v1 = true;
  (*info)->linux.cgroup.v2 = true;
#నిర్వచితమైతే HAVE_SYSTEMD
  (*info)->linux.cgroup.systemd = true;
  (*info)->linux.cgroup.systemd_user = true;
#అయితే_ముగింపు

  // Put seccomp values
#నిర్వచితమైతే HAVE_SECCOMP
  (*info)->linux.seccomp.enabled = true;
  // Populate actions
  populate_array_క్షేత్రం (&((*info)->linux.seccomp.actions), actions, అంకె_actions);
  // Populate operators
  populate_array_క్షేత్రం (&((*info)->linux.seccomp.operators), operators, అంకె_operators);
  // Populate archs
  populate_array_క్షేత్రం (&((*info)->linux.seccomp.archs), archs, అంకె_archs);
#లేకపోతే
  (*info)->linux.seccomp.enabled = false;
#అయితే_ముగింపు

  // Put values for apparmor and selinux
  (*info)->linux.apparmor.enabled = true;
  (*info)->linux.selinux.enabled = true;

  (*info)->linux.intel_rdt.enabled = true;

  (*info)->linux.net_devices.enabled = true;

  populate_array_క్షేత్రం (&((*info)->linux.జ్ఞాపకం_policy.బహుళకం), mempolicy_modes, అంకె_mempolicy_modes);
  populate_array_క్షేత్రం (&((*info)->linux.జ్ఞాపకం_policy.flags), mempolicy_flags, అంకె_mempolicy_flags);

  // Put the values for mount extensions
  (*info)->linux.mount_ext.idmap.enabled = true;

  // Populate the values for annotations
#నిర్వచితమైతే HAVE_SECCOMP
  {
    స్థిరరాశి నిర్మాణం scmp_సంస్కరణ *సంస్కరణ = seccomp_సంస్కరణ ();
    అక్షరం *సంస్కరణ_string = ఖాళీ;

    xasprintf (&సంస్కరణ_string, "%u.%u.%u", సంస్కరణ->major, సంస్కరణ->minor, సంస్కరణ->micro);
    (*info)->annotations.io_github_seccomp_libseccomp_సంస్కరణ = సంస్కరణ_string;
  }
#అయితే_ముగింపు

  ఒకవేళ (context->handler_manager && handler_by_name (context->handler_manager, "wasm"))
    (*info)->annotations.నిరంతరం_oci_crun_wasm = true;

#ఒకవేళ HAVE_CRIU
  (*info)->annotations.నిరంతరం_oci_crun_checkpoint_enabled = true;
#అయితే_ముగింపు
  (*info)->annotations.నిరంతరం_oci_crun_commit = GIT_సంస్కరణ;
  (*info)->annotations.నిరంతరం_oci_crun_సంస్కరణ = PACKAGE_సంస్కరణ;

  populate_array_క్షేత్రం (&((*info)->potentially_unsafe_annotations), potentially_unsafe_annotations, అంకె_unsafe_annotations);

  తిరిగి 0;
}

స్థిర శూన్య
features_కూడు_string (json_gen_ctx *json_gen, స్థిరరాశి అక్షరం *కీలకం, స్థిరరాశి అక్షరం *విలువ)
{
  json_gen_string (json_gen, కీలకం, అక్షరశ్రేణి_పొడవు (కీలకం));
  json_gen_string (json_gen, విలువ, అక్షరశ్రేణి_పొడవు (విలువ));
}

స్థిర శూన్య
features_కూడు_bool (json_gen_ctx *json_gen, స్థిరరాశి అక్షరం *కీలకం, పూర్ణాంకం విలువ)
{
  json_gen_string (json_gen, కీలకం, అక్షరశ్రేణి_పొడవు (కీలకం));
  json_gen_bool (json_gen, విలువ);
}

స్థిర శూన్య
features_కూడు_bool_str (json_gen_ctx *json_gen, స్థిరరాశి అక్షరం *కీలకం, పూర్ణాంకం విలువ)
{
  features_కూడు_string (json_gen, కీలకం, విలువ ? "true" : "false");
}

స్థిర శూన్య
features_కూడు_array (json_gen_ctx *json_gen, స్థిరరాశి అక్షరం *కీలకం, అక్షరం **array)
{
  size_t i;

  json_gen_string (json_gen, కీలకం, అక్షరశ్రేణి_పొడవు (కీలకం));
  json_gen_array_open (json_gen);
  ఒకవేళ (array)
    కొరకు (i = 0; array[i] != ఖాళీ; i++)
      json_gen_string (json_gen, array[i], అక్షరశ్రేణి_పొడవు (array[i]));
  json_gen_array_close (json_gen);
}

స్థిర శూన్య
features_కూడు_linux_info (json_gen_ctx *json_gen, స్థిరరాశి నిర్మాణం linux_info_s *linux)
{
  json_gen_string (json_gen, "linux", అక్షరశ్రేణి_పొడవు ("linux"));
  json_gen_map_open (json_gen);

  features_కూడు_array (json_gen, "namespaces", linux->namespaces);
  features_కూడు_array (json_gen, "capabilities", linux->capabilities);

  json_gen_string (json_gen, "cgroup", అక్షరశ్రేణి_పొడవు ("cgroup"));
  json_gen_map_open (json_gen);
  features_కూడు_bool (json_gen, "v1", linux->cgroup.v1);
  features_కూడు_bool (json_gen, "v2", linux->cgroup.v2);
  features_కూడు_bool (json_gen, "systemd", linux->cgroup.systemd);
  features_కూడు_bool (json_gen, "systemdUser", linux->cgroup.systemd_user);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "seccomp", అక్షరశ్రేణి_పొడవు ("seccomp"));
  json_gen_map_open (json_gen);
  features_కూడు_bool (json_gen, "enabled", linux->seccomp.enabled);
  ఒకవేళ (linux->seccomp.actions)
    features_కూడు_array (json_gen, "actions", linux->seccomp.actions);
  ఒకవేళ (linux->seccomp.operators)
    features_కూడు_array (json_gen, "operators", linux->seccomp.operators);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "apparmor", అక్షరశ్రేణి_పొడవు ("apparmor"));
  json_gen_map_open (json_gen);
  features_కూడు_bool (json_gen, "enabled", linux->apparmor.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "selinux", అక్షరశ్రేణి_పొడవు ("selinux"));
  json_gen_map_open (json_gen);
  features_కూడు_bool (json_gen, "enabled", linux->selinux.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "mountExtensions", అక్షరశ్రేణి_పొడవు ("mountExtensions"));
  json_gen_map_open (json_gen);
  json_gen_string (json_gen, "idmap", అక్షరశ్రేణి_పొడవు ("idmap"));
  json_gen_map_open (json_gen);
  features_కూడు_bool (json_gen, "enabled", linux->mount_ext.idmap.enabled);
  json_gen_map_close (json_gen);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "intelRdt", అక్షరశ్రేణి_పొడవు ("intelRdt"));
  json_gen_map_open (json_gen);
  features_కూడు_bool (json_gen, "enabled", linux->intel_rdt.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "netDevices", అక్షరశ్రేణి_పొడవు ("netDevices"));
  json_gen_map_open (json_gen);
  features_కూడు_bool (json_gen, "enabled", linux->net_devices.enabled);
  json_gen_map_close (json_gen);

  json_gen_string (json_gen, "memoryPolicy", అక్షరశ్రేణి_పొడవు ("memoryPolicy"));
  json_gen_map_open (json_gen);
  ఒకవేళ (linux->జ్ఞాపకం_policy.బహుళకం)
    features_కూడు_array (json_gen, "modes", linux->జ్ఞాపకం_policy.బహుళకం);
  ఒకవేళ (linux->జ్ఞాపకం_policy.flags)
    features_కూడు_array (json_gen, "flags", linux->జ్ఞాపకం_policy.flags);
  json_gen_map_close (json_gen);

  json_gen_map_close (json_gen);
}

స్థిర శూన్య
features_కూడు_annotations_info (json_gen_ctx *json_gen, స్థిరరాశి నిర్మాణం annotations_info_s *annotation)
{
  json_gen_string (json_gen, "annotations", అక్షరశ్రేణి_పొడవు ("annotations"));
  json_gen_map_open (json_gen);

  ఒకవేళ (! is_ఖాళీలు_string (annotation->io_github_seccomp_libseccomp_సంస్కరణ))
    features_కూడు_string (json_gen, "io.github.seccomp.libseccomp.version", annotation->io_github_seccomp_libseccomp_సంస్కరణ);

  features_కూడు_bool_str (json_gen, "org.opencontainers.runc.checkpoint.enabled", annotation->నిరంతరం_oci_crun_checkpoint_enabled);
  features_కూడు_bool_str (json_gen, "run.oci.crun.checkpoint.enabled", annotation->నిరంతరం_oci_crun_checkpoint_enabled);

  features_కూడు_string (json_gen, "run.oci.crun.commit", annotation->నిరంతరం_oci_crun_commit);
  features_కూడు_string (json_gen, "run.oci.crun.version", annotation->నిరంతరం_oci_crun_సంస్కరణ);

  features_కూడు_bool_str (json_gen, "run.oci.crun.wasm", annotation->నిరంతరం_oci_crun_wasm);

  json_gen_map_close (json_gen);
}

పూర్ణాంకం
libcrun_container_get_features_json (libcrun_context_t *context, అక్షరం **బహిర్గతం, libcrun_error_t *err)
{
  cleanup_struct_features నిర్మాణం features_info_s *info = ఖాళీ;
  స్థిరరాశి అక్షరం *బఫరు = ఖాళీ;
  json_gen_ctx *json_gen;
  size_t బఫరు_len;
  పూర్ణాంకం ret;

  *బహిర్గతం = ఖాళీ;

  ret = libcrun_container_get_features (context, &info, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (! json_gen_init (&json_gen, ఖాళీ))
    తిరిగి crun_make_error (err, 0, "cannot initialize JSON generator");

  json_gen_config (json_gen, json_gen_beautify, 1);

  json_gen_map_open (json_gen);

  features_కూడు_string (json_gen, "ociVersionMin", info->oci_సంస్కరణ_min);
  features_కూడు_string (json_gen, "ociVersionMax", info->oci_సంస్కరణ_max);
  features_కూడు_array (json_gen, "hooks", info->hooks);
  features_కూడు_array (json_gen, "mountOptions", info->mount_options);
  features_కూడు_linux_info (json_gen, &info->linux);
  features_కూడు_annotations_info (json_gen, &info->annotations);
  features_కూడు_array (json_gen, "potentiallyUnsafeConfigAnnotations", info->potentially_unsafe_annotations);

  json_gen_map_close (json_gen);

  json_gen_get_buf (json_gen, &బఫరు, &బఫరు_len);
  *బహిర్గతం = xstrdup (బఫరు);
  json_gen_free (json_gen);

  తిరిగి 0;
}

/* Read the container status and fail if the container is not running.  */
స్థిర పూర్ణాంకం
read_స్థితి_require_running (libcrun_context_t *context, స్థిరరాశి అక్షరం *id,
                             libcrun_container_స్థితి_t *స్థితి_2, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ret = libcrun_read_container_స్థితి (స్థితి_2, context->state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_is_container_running (స్థితి_2, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;
  ఒకవేళ (ret == 0)
    తిరిగి crun_make_error (err, 0, "the container `%s` is not running", id);

  తిరిగి 0;
}

పూర్ణాంకం
libcrun_container_pause (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  libcrun_container_స్థితి_t స్థితి_2 = {};

  ret = read_స్థితి_require_running (context, id, &స్థితి_2, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి libcrun_container_pause_linux (&స్థితి_2, err);
}

పూర్ణాంకం
libcrun_container_unpause (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  libcrun_container_స్థితి_t స్థితి_2 = {};

  ret = read_స్థితి_require_running (context, id, &స్థితి_2, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి libcrun_container_unpause_linux (&స్థితి_2, err);
}

/* Translate the public options struct into the internal representation used by
   the linux/CRIU layer.  Strings are borrowed (not copied).  */
స్థిర శూన్య
cr_options_from_ప్రజా (libcrun_checkpoint_restore_t *dst, నిర్మాణం libcrun_checkpoint_restore_options_s *src)
{
  memset (dst, 0, పరిమాణం (*dst));
  dst->image_మార్గం = (అక్షరం *) src->image_మార్గం;
  dst->work_మార్గం = (అక్షరం *) src->work_మార్గం;
  dst->మాతృ_మార్గం = (అక్షరం *) src->మాతృ_మార్గం;
  dst->console_socket = src->console_socket;
  dst->lsm_profile = (అక్షరం *) src->lsm_profile;
  dst->lsm_mount_context = (అక్షరం *) src->lsm_mount_context;
  dst->leave_running = src->leave_running;
  dst->tcp_established = src->tcp_established;
  dst->tcp_close = src->tcp_close;
  dst->ext_unix_sk = src->ext_unix_sk;
  dst->shell_పని = src->shell_పని;
  dst->file_locks = src->file_locks;
  dst->pre_dump = src->pre_dump;
  dst->detach = src->detach;
  dst->manage_cgroups_బహుళకం = src->manage_cgroups_బహుళకం;
  dst->జాలం_తాళం_పద్ధతి = src->జాలం_తాళం_పద్ధతి;
}

పూర్ణాంకం
libcrun_container_checkpoint (libcrun_context_t *context, స్థిరరాశి అక్షరం *id,
                              నిర్మాణం libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  స్థిరరాశి అక్షరం *state_మూలం = context->state_మూలం;
  libcrun_container_స్థితి_t స్థితి_2 = {};
  libcrun_checkpoint_restore_t cr;
  cleanup_container libcrun_container_t *container = ఖాళీ;

  cr_options_from_ప్రజా (&cr, cr_options);

  ret = read_స్థితి_require_running (context, id, &స్థితి_2, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = read_container_config_from_state (&container, state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  /* Needed to use the same paths (e.g. the shared empty directory used for
     masked paths) as the container runtime and restore do.  */
  container->context = context;

  ret = libcrun_container_checkpoint_linux (&స్థితి_2, container, &cr, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (! (cr_options->leave_running || cr_options->pre_dump))
    తిరిగి container_delete_అంతర్గత (context, ఖాళీ, ఖాళీ, id, true, true, err);

  తిరిగి 0;
}

/* The dummy process only keeps the cgroup populated until the container is restored there.
   Its cgroup is below the container cgroup, so once the container runs it can freeze or kill
   the dummy process: never depend on it after that point.  */
స్థిర శూన్య
restore_proxy_process (పూర్ణాంకం proxy_pid_pipe)
{
  అక్షరం c;

  /* Wait to be killed by the parent, or for the parent to go away.  */
  తాత్కాలిక_FAILURE_RETRY (పఠ (proxy_pid_pipe, &c, పరిమాణం (c)));
}

స్థిర పూర్ణాంకం
restore_terminate_proxy_process (pid_t proxy_pid, స్థిరరాశి అక్షరం *proxy_cgroup, pid_t container_pid, పూర్ణాంకం cgroup_manager, libcrun_error_t *err)
{
  పూర్ణాంకం బహుళకం;
  పూర్ణాంకం ret;

  ఒకవేళ (cgroup_manager == CGROUP_MANAGER_SYSTEMD)
    {
      అక్షరం ready_str[64];

      ret = snprintf (ready_str, పరిమాణం (ready_str), "MAINPID=%d", container_pid);
      ఒకవేళ (UNLIKELY (ret >= (పూర్ణాంకం) పరిమాణం (ready_str)))
        తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

      ret = send_sd_notify (ready_str, err);
      /* Do not fail on errors.  */
      ఒకవేళ (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (బహుళకం < 0))
    తిరిగి బహుళకం;

  /* SIGKILL terminates a process frozen with cgroup v2, while the cgroup v1 freezer does not
     deliver it until the cgroup is thawed, so do not wait for it there.  */
  kill (proxy_pid, SIGKILL);
  తాత్కాలిక_FAILURE_RETRY (waitpid (proxy_pid, ఖాళీ, బహుళకం == CGROUP_బహుళకం_UNIFIED ? 0 : WNOHANG));

  తిరిగి destroy_cgroup_మార్గం (proxy_cgroup, బహుళకం, err);
}

పూర్ణాంకం
libcrun_container_restore (libcrun_context_t *context, స్థిరరాశి అక్షరం *id arg_వాడని,
                           నిర్మాణం libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  cleanup_cgroup_స్థితి నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి = ఖాళీ;
  cleanup_container libcrun_container_t *container = ఖాళీ;
  cleanup_close పూర్ణాంకం proxy_pid_pipe0 = -1;
  cleanup_close పూర్ణాంకం proxy_pid_pipe1 = -1;
  runtime_spec_schema_config_schema *def;
  libcrun_container_స్థితి_t స్థితి_2 = {};
  నిర్మాణం libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  libcrun_checkpoint_restore_t cr;
  cleanup_pid pid_t proxy_pid = -1;
  cleanup_close పూర్ణాంకం seccomp_fd = -1;
  స్థిరరాశి అక్షరం *seccomp_bpf_దత్తాంశం;
  పూర్ణాంకం proxy_pid_pipe[2];
  పూర్ణాంకం cgroup_manager;
  uid_t మూలం_uid = -1;
  gid_t మూలం_gid = -1;
  పూర్ణాంకం ret;

  cr_options_from_ప్రజా (&cr, cr_options);

  container = libcrun_container_ఎక్కించు_from_file ("config.json", err);
  ఒకవేళ (container == ఖాళీ)
    తిరిగి -1;

  container->context = context;
  def = container->container_def;

  ret = check_config_file (def, context, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_స్థితి_check_directories (context->state_మూలం, context->id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_copy_config_file (context->id, context->state_మూలం, container, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు fail;

  /* CRIU restores the seccomp filters of the checkpointed processes, but
     exec loads the filter from the seccomp.bpf file in the state directory,
     so it must be generated again for the restored container.  */
  seccomp_bpf_దత్తాంశం = కనుగొను_annotation (container, "run.oci.seccomp_bpf_data");

  ret = అమరిక_seccomp (container, seccomp_bpf_దత్తాంశం, &seccomp_gen_ctx, &seccomp_fd, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు fail;

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_దత్తాంశం, &seccomp_gen_ctx, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు fail;
  close_and_reset (&seccomp_fd);

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  ఒకవేళ (context->systemd_cgroup)
    cgroup_manager = CGROUP_MANAGER_SYSTEMD;
  లేకపోతే ఒకవేళ (context->force_no_cgroup)
    cgroup_manager = CGROUP_MANAGER_DISABLED;

  def = container->container_def;

  /* If we are root (either on the host or in a namespace),
   * then chown the cgroup to root in the container user namespace. */
  get_మూలం_లోపల_the_userns (def, container->host_uid, container->host_gid, &మూలం_uid, &మూలం_gid);

  /* If the root in the container is different than the current root user, attempt to chown
     the std streams before entering the user namespace.  Otherwise we might lose access
     to the user (as it is not mapped in the user namespace) and cannot chown them.  */
  ఒకవేళ (మూలం_uid > 0 || మూలం_gid > 0)
    {
      ret = maybe_chown_std_streams (మూలం_uid, మూలం_gid, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        వెళ్ళు fail;
    }

  /* CRIU runs as a child process, so the restored processes would inherit the session keyring.  */
  ret = అమరిక_container_keyring (container, context, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  {
    cleanup_close పూర్ణాంకం cgroup_dirfd = -1;
    bool needs_proxy_process;
    cleanup_cgroup_preenter నిర్మాణం libcrun_cgroup_వాదనలు cg = {
      .resources = def->linux ? def->linux->resources : ఖాళీ,
      .annotations = container->annotations,
      .cgroup_మార్గం = def->linux ? def->linux->cgroups_మార్గం : "",
      .manager = cgroup_manager,
      .మూలం_uid = మూలం_uid,
      .మూలం_gid = మూలం_gid,
      .id = context->id,
      .state_మూలం = context->state_మూలం,
      .container = container,
    };

    /* The CRIU restore code uses bundle, rootfs and cgroup_path of status.   The cgroup_path is set later.  */
    స్థితి_2.bundle = (అక్షరం *) context->bundle;
    స్థితి_2.rootfs = def->మూలం->మార్గం;

    ret = libcrun_cgroup_preenter (&cg, &cgroup_dirfd, err);
    ఒకవేళ (UNLIKELY (ret < 0))
      వెళ్ళు fail;

    needs_proxy_process = cgroup_dirfd < 0;

    /* If the target cgroup was already created (with cgroupfs), we can restore the container directly there.  */
    ఒకవేళ (! needs_proxy_process)
      {
        cleanup_free అక్షరం *cgroup_మార్గం = ఖాళీ;

        ret = get_cgroup_dirfd_మార్గం (cgroup_dirfd, &cgroup_మార్గం, err);
        ఒకవేళ (UNLIKELY (ret < 0))
          వెళ్ళు fail;

        /* Complete the configuration for the cgroup before the container is
           restored there: CRIU resumes the restored processes before it
           returns, so the device filter and the limits must already be in
           place.  There is no process to move, as CRIU is run in the cgroup,
           and the controllers were enabled by precreate_cgroup.  */
        cg.joined = true;

        ret = libcrun_cgroup_enter (&cg, &cgroup_స్థితి, err);
        ఒకవేళ (UNLIKELY (ret < 0))
          వెళ్ళు fail;

        /* Restore the container directly in the desired cgroup.  */
        స్థితి_2.cgroup_మార్గం = cgroup_మార్గం;

        ret = libcrun_container_restore_linux (&స్థితి_2, container, &cr, err);
        ఒకవేళ (UNLIKELY (ret < 0))
          వెళ్ళు fail;

        /* Now that the container is restored, keep any signal for it, so that
           it is forwarded to the init by wait_for_process, rather than acted
           upon by crun.  This cannot be done earlier, as CRIU, which is run as
           a child process, would inherit the blocked signals.  */
        ret = ఖండం_సంకేతాలు (err);
        ఒకవేళ (UNLIKELY (ret < 0))
          వెళ్ళు fail;

        /* Use the container first process PID to finalize the cgroup.  */
        cg.pid = స్థితి_2.pid;
      }
    లేకపోతే
      {
        /* When using systemd, we need a process first that must exist before a cgroup is created.
           Create a dummy process that sits and waits until the container is restored.  */
        ret = pipe2 (proxy_pid_pipe, O_CLOEXEC);
        ఒకవేళ (UNLIKELY (ret < 0))
          {
            ret = crun_make_error (err, errno, "pipe");
            వెళ్ళు fail;
          }

        /* Only used for auto cleanup.  */
        proxy_pid_pipe0 = proxy_pid_pipe[0];
        proxy_pid_pipe1 = proxy_pid_pipe[1];

        proxy_pid = fork ();
        ఒకవేళ (UNLIKELY (proxy_pid < 0))
          {
            ret = crun_make_error (err, errno, "fork");
            వెళ్ళు fail;
          }

        ఒకవేళ (proxy_pid == 0)
          {
            close_and_reset (&proxy_pid_pipe1);

            restore_proxy_process (proxy_pid_pipe0);
            సురక్షిత_exit (EXIT_SUCCESS);
          }

        close_and_reset (&proxy_pid_pipe0);

        /* Use the dummy process PID as the PID to setup the cgroup.  */
        cg.pid = proxy_pid;

        /* Complete the configuration for the cgroup.  It contains only the dummy process.  */
        ret = libcrun_cgroup_enter (&cg, &cgroup_స్థితి, err);
        ఒకవేళ (UNLIKELY (ret < 0))
          వెళ్ళు fail;
      }

    ret = libcrun_cgroup_enter_finalize (&cg, cgroup_స్థితి, err);
    ఒకవేళ (UNLIKELY (ret < 0))
      వెళ్ళు fail;

    /* When using a dummy process, the container is restored here once the cgroup is configured.  */
    ఒకవేళ (needs_proxy_process)
      {
        cleanup_free అక్షరం *లక్ష్యం_cgroup = ఖాళీ;
        cleanup_free అక్షరం *proxy_cgroup = ఖాళీ;

        ret = libcrun_get_cgroup_process (proxy_pid, &లక్ష్యం_cgroup, false, err);
        ఒకవేళ (UNLIKELY (ret < 0))
          వెళ్ళు fail;

        /* Move the dummy process to a sub-cgroup.  */
        ret = append_paths (&proxy_cgroup, err, లక్ష్యం_cgroup, ".proxy-crun", ఖాళీ);
        ఒకవేళ (UNLIKELY (ret < 0))
          వెళ్ళు fail;

        ret = libcrun_move_process_గమ్యం_cgroup (proxy_pid, 0, proxy_cgroup, true, err);
        ఒకవేళ (UNLIKELY (ret < 0))
          వెళ్ళు fail;

        /* Restore the container in the same cgroup where the dummy process was.  */
        స్థితి_2.cgroup_మార్గం = లక్ష్యం_cgroup;

        ret = libcrun_container_restore_linux (&స్థితి_2, container, &cr, err);
        ఒకవేళ (UNLIKELY (ret < 0))
          వెళ్ళు fail;

        /* See the comment for the other libcrun_container_restore_linux call.  */
        ret = ఖండం_సంకేతాలు (err);
        ఒకవేళ (UNLIKELY (ret < 0))
          వెళ్ళు fail;

        /* The container is running, so do not fail because of the dummy process.  */
        ret = restore_terminate_proxy_process (proxy_pid, proxy_cgroup, స్థితి_2.pid, cgroup_manager, err);
        ఒకవేళ (UNLIKELY (ret < 0))
          crun_error_release (err);
        proxy_pid = -1;

        close_and_reset (&proxy_pid_pipe1);
      }
  }

  context->detach = cr_options->detach;
  ret = write_container_స్థితి (container, context, స్థితి_2.pid, cgroup_స్థితి, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు fail;

  {
    /* Wait the same way run and exec do: apart from waiting for the container
       init, this writes the pid file, forwards any signal to the init, and
       reaps every child, not only the init.  */
    నిర్మాణం wait_for_process_వాదనలు వాదనలు = {
      .pid = స్థితి_2.pid,
      .context = context,
      .terminal_fd = -1,
      .notify_socket = -1,
      .container_ready_fd = ఖాళీ,
      .seccomp_notify_fd = -1,
      .seccomp_notify_plugins = ఖాళీ,
    };

    తిరిగి wait_for_process (&వాదనలు, err);
  }

fail:
  /* Once restored, the container is running.  Without a status file nothing
     can reach it anymore, so do not leave it behind.  */
  ఒకవేళ (స్థితి_2.pid > 0)
    {
      kill (స్థితి_2.pid, SIGKILL);
      waitpid_ignore_stopped (స్థితి_2.pid, ఖాళీ, 0);
    }
  ఒకవేళ (cgroup_స్థితి)
    {
      libcrun_error_t tmp_err = ఖాళీ;
      libcrun_cgroup_destroy (cgroup_స్థితి, &tmp_err);
      crun_error_release (&tmp_err);
    }
  {
    libcrun_error_t tmp_err = ఖాళీ;

    ఒకవేళ (libcrun_container_delete_స్థితి (context->state_మూలం, context->id, &tmp_err) < 0)
      crun_error_release (&tmp_err);
  }
  తిరిగి ret;
}

పూర్ణాంకం
libcrun_container_read_pids (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  cleanup_cgroup_స్థితి నిర్మాణం libcrun_cgroup_స్థితి *cgroup_స్థితి = ఖాళీ;
  cleanup_container_స్థితి libcrun_container_స్థితి_t స్థితి_2 = {};
  పూర్ణాంకం ret;

  ret = libcrun_read_container_స్థితి (&స్థితి_2, context->state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (స్థితి_2.cgroup_మార్గం == ఖాళీ || స్థితి_2.cgroup_మార్గం[0] == '\0')
    తిరిగి crun_make_error (err, 0, "the container is not using cgroups");

  cgroup_స్థితి = libcrun_cgroup_make_స్థితి (&స్థితి_2);

  తిరిగి libcrun_cgroup_read_pids (cgroup_స్థితి, recurse, pids, err);
}

పూర్ణాంకం
libcrun_container_list_json (libcrun_context_t *context, అక్షరం **బహిర్గతం, libcrun_error_t *err)
{
  libcrun_container_list_t *list = ఖాళీ, *it;
  స్థిరరాశి అక్షరం *content = ఖాళీ;
  json_gen_ctx *gen = ఖాళీ;
  size_t len;
  పూర్ణాంకం ret;

  *బహిర్గతం = ఖాళీ;

  ret = libcrun_get_containers_list (&list, context->state_మూలం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (! json_gen_init (&gen, ఖాళీ))
    {
      ret = crun_make_error (err, 0, "cannot allocate json generator");
      వెళ్ళు నిష్క్రమించు;
    }

  json_gen_config (gen, json_gen_beautify, 1);
  json_gen_array_open (gen);

  కొరకు (it = list; it; it = it->తదుపరి)
    {
      libcrun_container_స్థితి_t స్థితి_2;
      పూర్ణాంకం running = 0;
      పూర్ణాంకం pid;
      స్థిరరాశి అక్షరం *container_స్థితి = ఖాళీ;

      ret = libcrun_read_container_స్థితి (&స్థితి_2, context->state_మూలం, it->name, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        వెళ్ళు నిష్క్రమించు;

      pid = స్థితి_2.pid;

      ret = libcrun_get_container_state_string (it->name, &స్థితి_2, context->state_మూలం, &container_స్థితి,
                                                &running, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          కొనసాగు;
        }

      ఒకవేళ (! running)
        pid = 0;

      json_gen_map_open (gen);
      json_gen_string (gen, "id", అక్షరశ్రేణి_పొడవు ("id"));
      json_gen_string (gen, it->name, అక్షరశ్రేణి_పొడవు (it->name));
      json_gen_string (gen, "pid", అక్షరశ్రేణి_పొడవు ("pid"));
      map_int (gen, pid);
      json_gen_string (gen, "status", అక్షరశ్రేణి_పొడవు ("status"));
      json_gen_string (gen, container_స్థితి, అక్షరశ్రేణి_పొడవు (container_స్థితి));
      json_gen_string (gen, "bundle", అక్షరశ్రేణి_పొడవు ("bundle"));
      json_gen_string (gen, స్థితి_2.bundle, అక్షరశ్రేణి_పొడవు (స్థితి_2.bundle));
      json_gen_string (gen, "created", అక్షరశ్రేణి_పొడవు ("created"));
      json_gen_string (gen, స్థితి_2.created, అక్షరశ్రేణి_పొడవు (స్థితి_2.created));
      json_gen_string (gen, "owner", అక్షరశ్రేణి_పొడవు ("owner"));
      json_gen_string (gen, స్థితి_2.owner, సురక్షిత_strlen (స్థితి_2.owner));
      json_gen_map_close (gen);

      libcrun_free_container_స్థితి (&స్థితి_2);
    }

  json_gen_array_close (gen);
  ఒకవేళ (json_gen_get_buf (gen, &content, &len) != json_gen_స్థితి_ok)
    {
      ret = libcrun_make_error (err, 0, "cannot generate json list");
      వెళ్ళు నిష్క్రమించు;
    }

  *బహిర్గతం = xstrdup (content);
  ret = 0;

నిష్క్రమించు:
  ఒకవేళ (list)
    libcrun_free_containers_list (list);
  ఒకవేళ (gen)
    json_gen_free (gen);

  తిరిగి ret;
}

పూర్ణాంకం
libcrun_write_json_containers_list (libcrun_context_t *context, FILE *బహిర్గతం, libcrun_error_t *err)
{
  cleanup_free అక్షరం *content = ఖాళీ;
  size_t len;
  పూర్ణాంకం ret;

  ret = libcrun_container_list_json (context, &content, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  len = అక్షరశ్రేణి_పొడవు (content);
  ఒకవేళ (len && fwrite (content, 1, len, బహిర్గతం) != len)
    తిరిగి crun_make_error (err, errno, "error writing to file");

  తిరిగి 0;
}

పూర్ణాంకం
libcrun_container_update_intel_rdt (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, నిర్మాణం libcrun_intel_rdt_update *update, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = ఖాళీ;
  పూర్ణాంకం ret;

  ret = read_container_config_from_state (&container, context->state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి libcrun_update_intel_rdt (id, container, update->l3_cache_schema, update->mem_bw_schema, update->schemata, err);
}

స్థిర పూర్ణాంకం
libcrun_container_కూడు_or_remove_mounts_from_file (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *file, bool కూడు, libcrun_error_t *err)
{
  cleanup_custom_handler_instance నిర్మాణం custom_handler_instance_s *custom_handler = ఖాళీ;
  cleanup_container libcrun_container_t *container = ఖాళీ;
  cleanup_free runtime_spec_schema_defs_mount **mounts = ఖాళీ;
  స్థిరరాశి అక్షరం *state_మూలం = context->state_మూలం;
  cleanup_free parser_error parser_err = ఖాళీ;
  నిర్మాణం parser_context pctx = { 0, stderr };
  libcrun_container_స్థితి_t స్థితి_2 = {};
  cleanup_free అక్షరం *content = ఖాళీ;
  json_object *doc = ఖాళీ;
  size_t n_mounts = 0, len, i;
  పూర్ణాంకం ret = 1;

  ret = read_all_file (file, &content, &len, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_read_container_స్థితి (&స్థితి_2, state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = read_container_config_from_state (&container, state_మూలం, id, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = parse_json_file (&doc, content, &pctx, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (! json_object_is_type (doc, json_type_array))
    {
      ret = crun_make_error (err, 0, "mounts must be an array");
      వెళ్ళు cleanup;
    }
  లేకపోతే
    {
      n_mounts = json_object_array_నిడివి (doc);

      mounts = xmalloc0 ((n_mounts + 1) * పరిమాణం (*mounts));
      కొరకు (i = 0; i < n_mounts; i++)
        {
          mounts[i] = make_runtime_spec_schema_defs_mount (json_object_array_get_idx (doc, i), &pctx, &parser_err);
          ఒకవేళ (mounts[i] == ఖాళీ)
            {
              ret = crun_make_error (err, 0, "cannot parse mount: %s", parser_err);
              వెళ్ళు cleanup;
            }
        }
    }

  ఒకవేళ (కూడు)
    ret = libcrun_make_runtime_mounts (container, &స్థితి_2, mounts, n_mounts, err);
  లేకపోతే
    ret = libcrun_destroy_runtime_mounts (container, &స్థితి_2, mounts, n_mounts, err);

cleanup:
  ఒకవేళ (doc)
    json_object_put (doc);
  ఒకవేళ (mounts)
    కొరకు (i = 0; i < n_mounts; i++)
      free_runtime_spec_schema_defs_mount (mounts[i]);
  తిరిగి ret;
}

పూర్ణాంకం
libcrun_container_కూడు_mounts_from_file (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *file, libcrun_error_t *err)
{
  తిరిగి libcrun_container_కూడు_or_remove_mounts_from_file (context, id, file, true, err);
}

పూర్ణాంకం
libcrun_container_remove_mounts_from_file (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *file, libcrun_error_t *err)
{
  తిరిగి libcrun_container_కూడు_or_remove_mounts_from_file (context, id, file, false, err);
}
