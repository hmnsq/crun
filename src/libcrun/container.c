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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <ocispec/runtime_spec_schema_config_schema.h>
#ଅନ୍ତର୍ଭାଵ <stdbool.h>
#ଅନ୍ତର୍ଭାଵ "container.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "json_gen_utils.h"
#ଅନ୍ତର୍ଭାଵ "seccomp.h"
#ଅନ୍ତର୍ଭାଵ "mempolicy.h"
#ifdef HAVE_SECCOMP
#  include <seccomp.h>
#endif
#ଅନ୍ତର୍ଭାଵ "scheduler.h"
#ଅନ୍ତର୍ଭାଵ "seccomp_notify.h"
#ଅନ୍ତର୍ଭାଵ "custom-handler.h"
#ଅନ୍ତର୍ଭାଵ <stdbool.h>
#ଅନ୍ତର୍ଭାଵ <argp.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <sched.h>
#ଅନ୍ତର୍ଭାଵ <sys/wait.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ "status.h"
#ଅନ୍ତର୍ଭାଵ "mount_flags.h"
#ଅନ୍ତର୍ଭାଵ "linux.h"
#ଅନ୍ତର୍ଭାଵ "terminal.h"
#ଅନ୍ତର୍ଭାଵ "io_priority.h"
#ଅନ୍ତର୍ଭାଵ "cgroup.h"
#ଅନ୍ତର୍ଭାଵ "cgroup-utils.h"
#ଅନ୍ତର୍ଭାଵ <sys/prctl.h>
#ଅନ୍ତର୍ଭାଵ <sys/auxv.h>
#ଅନ୍ତର୍ଭାଵ <sys/signalfd.h>
#ଅନ୍ତର୍ଭାଵ <sys/epoll.h>
#ଅନ୍ତର୍ଭାଵ <sys/socket.h>
#ifdef HAVE_CAP
#  include <sys/capability.h>
#endif
#ଅନ୍ତର୍ଭାଵ <sys/ioctl.h>
#ଅନ୍ତର୍ଭାଵ <termios.h>
#ଅନ୍ତର୍ଭାଵ <grp.h>
#ଅନ୍ତର୍ଭାଵ <git-version.h>

#ifdef HAVE_SYSTEMD
#  include <systemd/sd-daemon.h>
#endif

#ଅନ୍ତର୍ଭାଵ <json-c/json.h>
#ଅନ୍ତର୍ଭାଵ <ocispec/json_common.h>

ଗଣନା
{
  SYNC_SOCKET_SYNC_MESSAGE,
  SYNC_SOCKET_ERROR_MESSAGE,
  SYNC_SOCKET_WARNING_MESSAGE,
  SYNC_SOCKET_DEBUG_MESSAGE,
};

ସଂରଚନା container_entrypoint_s
{
  libcrun_container_t *container;
  libcrun_context_t *context;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_terminal_socket_pair;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା terminal_socketpair[2];

  /* Used by log_write_to_sync_socket.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା seccomp_fd;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା seccomp_receiver_fd;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା console_socket_fd;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା hooks_out_fd;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା hooks_err_fd;

  /* Absolute bundle path, to be reported to hooks run in the container.  */
  ଅଚଳ ଅକ୍ଷର *bundle;

  ସଂରଚନା custom_handler_instance_s *custom_handler;
};

ସଂରଚନା sync_socket_message_s
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା type;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା error_ମୂଲ୍ୟ;
  ଅକ୍ଷର message[512];
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା runtime_spec_schema_defs_hook hook;

// linux hooks
ଅକ୍ଷର *hooks[] = {
  "prestart",
  "createRuntime",
  "createContainer",
  "startContainer",
  "poststart",
  "poststop"
};

// linux namespaces
ସ୍ଥିର ଅକ୍ଷର *namespaces[] = {
  "cgroup",
  "ipc",
  "mount",
  "network",
  "pid",
  "user",
  "uts"
};

ସ୍ଥିର ଅକ୍ଷର *actions[] = {
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

ସ୍ଥିର ଅକ୍ଷର *operators[] = {
  "SCMP_CMP_NE",
  "SCMP_CMP_LT",
  "SCMP_CMP_LE",
  "SCMP_CMP_EQ",
  "SCMP_CMP_GE",
  "SCMP_CMP_GT",
  "SCMP_CMP_MASKED_EQ",
};

ସ୍ଥିର ଅକ୍ଷର *archs[] = {
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

ସ୍ଥିର ଅକ୍ଷର *mempolicy_modes[] = {
  "MPOL_DEFAULT",
  "MPOL_PREFERRED",
  "MPOL_BIND",
  "MPOL_INTERLEAVE",
  "MPOL_LOCAL",
  "MPOL_PREFERRED_MANY",
  "MPOL_WEIGHTED_INTERLEAVE"
};

ସ୍ଥିର ଅକ୍ଷର *mempolicy_flags[] = {
  "MPOL_F_NUMA_BALANCING",
  "MPOL_F_RELATIVE_NODES",
  "MPOL_F_STATIC_NODES"
};

ସ୍ଥିର ଅକ୍ଷର *potentially_unsafe_annotations[] = {
  "module.wasm.image/variant",
  "io.kubernetes.cri.container-type",
  "run.oci.",
  "org.criu.",
  "krun.",
};

#define SYNC_SOCKET_MESSAGE_LEN(x, l) (offsetof (ସଂରଚନା sync_socket_message_s, message) + l)

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
sync_socket_write_ସନ୍ଦେଶ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity, ପୂର୍ଣ୍ଣସଂଖ୍ୟା err_ମୂଲ୍ୟ, ଅଚଳ ଅକ୍ଷର *log_ସନ୍ଦେଶ)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  size_t err_len;
  ସଂରଚନା sync_socket_message_s ସନ୍ଦେଶ;
  ବାଛ (verbosity)
    {
    ସ୍ଥିତି LIBCRUN_VERBOSITY_DEBUG:
      ସନ୍ଦେଶ.type = SYNC_SOCKET_DEBUG_MESSAGE;
      ଭାଙ୍ଗ;
    ସ୍ଥିତି LIBCRUN_VERBOSITY_WARNING:
      ସନ୍ଦେଶ.type = SYNC_SOCKET_WARNING_MESSAGE;
      ଭାଙ୍ଗ;
    ସ୍ଥିତି LIBCRUN_VERBOSITY_ERROR:
      ସନ୍ଦେଶ.type = SYNC_SOCKET_ERROR_MESSAGE;
      ଭାଙ୍ଗ;
    }
  ସନ୍ଦେଶ.error_ମୂଲ୍ୟ = err_ମୂଲ୍ୟ;

  ଯଦି (fd < 0)
    ଫେରନ୍ତୁ 0;

  err_len = strlen (log_ସନ୍ଦେଶ);
  ଯଦି (err_len >= ଆକାର (ସନ୍ଦେଶ.message))
    err_len = ଆକାର (ସନ୍ଦେଶ.message) - 1;

  memcpy (ସନ୍ଦେଶ.message, log_ସନ୍ଦେଶ, err_len);
  ସନ୍ଦେଶ.message[err_len] = '\0';

  ret = TEMP_FAILURE_RETRY (ଲିଖ (fd, &ସନ୍ଦେଶ, SYNC_SOCKET_MESSAGE_LEN (ସନ୍ଦେଶ, err_len + 1)));
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ -1;

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
sync_socket_write_error (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, libcrun_error_t *out_err)
{
  ଯଦି (fd < 0)
    ଫେରନ୍ତୁ 0;
  ଫେରନ୍ତୁ sync_socket_write_ସନ୍ଦେଶ (fd, false, (*out_err)->status, (*out_err)->ସନ୍ଦେଶ);
}

ସ୍ଥିର ଶୂନ୍ୟ
log_write_to_sync_socket (ପୂର୍ଣ୍ଣସଂଖ୍ୟା errno_, ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity, ଶୂନ୍ୟ *arg)
{
  ସଂରଚନା container_entrypoint_s *entrypoint_args = arg;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = entrypoint_args->sync_socket;

  ଯଦି (fd < 0)
    ଫେରନ୍ତୁ;

  ଯଦି (sync_socket_write_ସନ୍ଦେଶ (fd, verbosity, errno_, ସନ୍ଦେଶ) < 0)
    log_write_to_stderr (errno_, ସନ୍ଦେଶ, verbosity, arg);
}

ସ୍ଥିର bool
is_memory_limit_too_ତଳ (runtime_spec_schema_config_schema *def)
{
  ଅଚଳ long memory_limit_too_ତଳ = 1024 * 1024;

  ଯଦି (def->linux == ଖାଲି || def->linux->resources == ଖାଲି)
    ଫେରନ୍ତୁ false;

  ଯଦି (def->linux->resources->memory
      && def->linux->resources->memory->limit_present
      && def->linux->resources->memory->limit < memory_limit_too_ତଳ)
    ଫେରନ୍ତୁ true;

  ଯଦି (def->linux->resources->unified)
    {
      size_t i;

      ପାଇଁ (i = 0; i < def->linux->resources->unified->len; i++)
        ଯଦି (strcmp (def->linux->resources->unified->ଚାବିଗୁଡ଼ିକ[i], "memory.max") == 0)
          {
            long limit;

            errno = 0;
            limit = strtol (def->linux->resources->unified->ମୂଲ୍ୟଗୁଡ଼ିକ[i], ଖାଲି, 10);
            ଯଦି (errno != 0)
              ଫେରନ୍ତୁ false;
            ଯଦି (limit < memory_limit_too_ତଳ)
              ଫେରନ୍ତୁ true;
          }
    }

  ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
sync_socket_wait_sync (libcrun_context_t *context, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, bool flush, libcrun_error_t *err)
{
  ସଂରଚନା sync_socket_message_s ସନ୍ଦେଶ;

  ଯଦି (fd < 0)
    ଫେରନ୍ତୁ 0;

  ଯେତେବେଳେ (true)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

      ret = TEMP_FAILURE_RETRY (ପଠ (fd, &ସନ୍ଦେଶ, ଆକାର (ସନ୍ଦେଶ)));
      ଯଦି (UNLIKELY (ret < 0))
        {
          ଯଦି (flush)
            ଫେରନ୍ତୁ 0;
          ଫେରନ୍ତୁ crun_make_error (err, errno, "read from sync socket");
        }

      ଯଦି (ret == 0)
        {
          ଯଦି (flush)
            ଫେରନ୍ତୁ 0;

          ଫେରନ୍ତୁ crun_make_error (err, 0, "read from the init process");
        }

      ଯଦି (! flush && ସନ୍ଦେଶ.type == SYNC_SOCKET_SYNC_MESSAGE)
        ଫେରନ୍ତୁ 0;
      ଅନ୍ୟଥା ଯଦି (ସନ୍ଦେଶ.type == SYNC_SOCKET_DEBUG_MESSAGE)
        {
          ଯଦି (context)
            context->output_handler (ସନ୍ଦେଶ.error_ମୂଲ୍ୟ, ସନ୍ଦେଶ.message, LIBCRUN_VERBOSITY_DEBUG, context->output_handler_arg);
          ଜାରି;
        }
      ଅନ୍ୟଥା ଯଦି (ସନ୍ଦେଶ.type == SYNC_SOCKET_WARNING_MESSAGE)
        {
          ଯଦି (context)
            context->output_handler (ସନ୍ଦେଶ.error_ମୂଲ୍ୟ, ସନ୍ଦେଶ.message, LIBCRUN_VERBOSITY_WARNING, context->output_handler_arg);
          ଜାରି;
        }
      ଅନ୍ୟଥା ଯଦି (ସନ୍ଦେଶ.type == SYNC_SOCKET_ERROR_MESSAGE)
        ଫେରନ୍ତୁ crun_make_error (err, ସନ୍ଦେଶ.error_ମୂଲ୍ୟ, "%s", ସନ୍ଦେଶ.message);
    }
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
sync_socket_send_sync (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, bool flush_errors, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ସଂରଚନା sync_socket_message_s ସନ୍ଦେଶ = {
    0,
  };
  ସନ୍ଦେଶ.type = SYNC_SOCKET_SYNC_MESSAGE;

  ଯଦି (fd < 0)
    ଫେରନ୍ତୁ 0;

  ret = TEMP_FAILURE_RETRY (ଲିଖ (fd, &ସନ୍ଦେଶ, SYNC_SOCKET_MESSAGE_LEN (ସନ୍ଦେଶ, 0)));
  ଯଦି (UNLIKELY (ret < 0))
    {
      ଯଦି (flush_errors)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;
          ret = TEMP_FAILURE_RETRY (ପଠ (fd, &ସନ୍ଦେଶ, ଆକାର (ସନ୍ଦେଶ)));
          ଯଦି (ret >= 0 && ସନ୍ଦେଶ.type == SYNC_SOCKET_ERROR_MESSAGE)
            ଫେରନ୍ତୁ crun_make_error (err, ସନ୍ଦେଶ.error_ମୂଲ୍ୟ, "%s", ସନ୍ଦେଶ.message);
          errno = saved_errno;
        }
      ଫେରନ୍ତୁ crun_make_error (err, errno, "write to sync socket");
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର bool
has_cap_sys_ptrace ()
{
#ifdef HAVE_CAP
  ସଂରଚନା __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  ସଂରଚନା __user_cap_data_struct data[2] = { { 0 } };

  ଯଦି (UNLIKELY (capget (&hdr, data) < 0))
    ଫେରନ୍ତୁ true;

  ଫେରନ୍ତୁ (data[0].effective & (1U << CAP_SYS_PTRACE)) != 0;
#ଅନ୍ୟଥା
  ଫେରନ୍ତୁ geteuid () == 0;
#endif
}

ସ୍ଥିର libcrun_container_t *
make_container (runtime_spec_schema_config_schema *container_def, ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର *config)
{
  libcrun_container_t *container = xmalloc0 (ଆକାର (*container));
  container->container_def = container_def;

  container->host_uid = geteuid ();
  container->host_gid = getegid ();
  container->host_has_cap_sys_ptrace = has_cap_sys_ptrace ();
  container->proc_fd = -1;

  container->annotations = make_string_map_from_json (container_def->annotations);

  /* An empty namespace path means that a new namespace is to be created,
     the same as if it is not specified.  */
  ଯଦି (container_def->linux)
    {
      size_t i;

      ପାଇଁ (i = 0; i < container_def->linux->namespaces_len; i++)
        ଯଦି (container_def->linux->namespaces[i]->path && container_def->linux->namespaces[i]->path[0] == '\0')
          {
            ମୁକ୍ତ (container_def->linux->namespaces[i]->path);
            container_def->linux->namespaces[i]->path = ଖାଲି;
          }
    }

  ଯଦି (path)
    container->config_file = xstrdup (path);
  ଯଦି (config)
    container->config_file_content = xstrdup (config);

  ଫେରନ୍ତୁ container;
}

libcrun_container_t *
libcrun_container_load_from_memory (ଅଚଳ ଅକ୍ଷର *json, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free ଅକ୍ଷର *oci_error = ଖାଲି;
  container_def = runtime_spec_schema_config_schema_parse_data (json, ଖାଲି, &oci_error);
  ଯଦି (container_def == ଖାଲି)
    {
      crun_make_error (err, 0, "load: `%s`", oci_error);
      ଫେରନ୍ତୁ ଖାଲି;
    }
  ଫେରନ୍ତୁ make_container (container_def, ଖାଲି, json);
}

libcrun_container_t *
libcrun_container_load_from_file (ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *container_def;
  cleanup_free ଅକ୍ଷର *oci_error = ଖାଲି;
  cleanup_free ଅକ୍ଷର *content = ଖାଲି;
  size_t len;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  libcrun_debug ("Loading container from config file: `%s`", path);
  ret = read_all_file (path, &content, &len, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ଖାଲି;

  container_def = runtime_spec_schema_config_schema_parse_data (content, ଖାଲି, &oci_error);
  ଯଦି (container_def == ଖାଲି)
    {
      crun_make_error (err, 0, "load `%s`: %s", path, oci_error);
      ଫେରନ୍ତୁ ଖାଲି;
    }
  ଫେରନ୍ତୁ make_container (container_def, ଖାଲି, content);
}

ଅଚଳ ଅକ୍ଷର *
libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err)
{
  ସଂରଚନା parser_context ctx = { 0, stderr };
  parser_error gen_err = ଖାଲି;

  ଯଦି (container == ଖାଲି)
    {
      crun_make_error (err, EINVAL, "no container specified");
      ଫେରନ୍ତୁ ଖାଲି;
    }

  ଯଦି (container->config_file_content == ଖାଲି && container->container_def)
    {
      container->config_file_content = runtime_spec_schema_config_schema_generate_json (container->container_def, &ctx, &gen_err);
      ଯଦି (container->config_file_content == ଖାଲି)
        {
          crun_make_error (err, 0, "cannot generate the configuration JSON: %s", gen_err ?: "unknown error");
          ମୁକ୍ତ (gen_err);
          ଫେରନ୍ତୁ ଖାଲି;
        }
      ମୁକ୍ତ (gen_err);
    }

  ଯଦି (container->config_file_content == ଖାଲି)
    {
      crun_make_error (err, 0, "the container has no configuration");
      ଫେରନ୍ତୁ ଖାଲି;
    }

  ଫେରନ୍ତୁ container->config_file_content;
}

ଅଚଳ ଅକ୍ଷର *
libcrun_container_get_annotation (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *ଚାବି)
{
  ଯଦି (container == ଖାଲି)
    ଫେରନ୍ତୁ ଖାଲି;
  ଫେରନ୍ତୁ find_string_map_ମୂଲ୍ୟ (container->annotations, ଚାବି);
}

size_t
libcrun_container_get_annotations_len (libcrun_container_t *container)
{
  ଯଦି (container == ଖାଲି || container->annotations == ଖାଲି)
    ଫେରନ୍ତୁ 0;
  ଫେରନ୍ତୁ string_map_size (container->annotations);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_get_annotation_at (libcrun_container_t *container, size_t index, ଅଚଳ ଅକ୍ଷର **ଚାବି,
                                     ଅଚଳ ଅକ୍ଷର **ମୂଲ୍ୟ)
{
  ଯଦି (container == ଖାଲି)
    {
      errno = ERANGE;
      ଫେରନ୍ତୁ -1;
    }
  ଫେରନ୍ତୁ string_map_get_at (container->annotations, index, ଚାବି, ମୂଲ୍ୟ);
}

uid_t
libcrun_container_get_uid (libcrun_container_t *container)
{
  ଯଦି (container == ଖାଲି)
    ଫେରନ୍ତୁ (uid_t) -1;
  ଫେରନ୍ତୁ container->host_uid;
}

gid_t
libcrun_container_get_gid (libcrun_container_t *container)
{
  ଯଦି (container == ଖାଲି)
    ଫେରନ୍ତୁ (gid_t) -1;
  ଫେରନ୍ତୁ container->host_gid;
}

ଶୂନ୍ୟ
libcrun_container_free (libcrun_container_t *ctr)
{
  ଯଦି (ctr == ଖାଲି)
    ଫେରନ୍ତୁ;

  ଯଦି (ctr->cleanup_private_data)
    ctr->cleanup_private_data (ctr->private_data);

  ଯଦି (ctr->container_def)
    free_runtime_spec_schema_config_schema (ctr->container_def);

  free_string_map (ctr->annotations);

  ଯଦି (ctr->proc_fd >= 0)
    close (ctr->proc_fd);

  ମୁକ୍ତ (ctr->config_file_content);
  ମୁକ୍ତ (ctr->config_file);
  ମୁକ୍ତ (ctr);
}

libcrun_context_t *
libcrun_context_new (ଅଚଳ ଅକ୍ଷର *id, ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, libcrun_error_t *err)
{
  libcrun_context_t *ctx = xmalloc0 (ଆକାର (*ctx));

  /* Go through the setters so that the owned_* copies and the borrowed
     pointers stay in sync and are released by libcrun_context_free.  */
  libcrun_context_set_id (ctx, id);
  libcrun_context_set_state_ମୂଳ (ctx, state_ମୂଳ);
  libcrun_context_set_bundle (ctx, ".");
  ctx->fifo_exec_wait_fd = -1;

  ଯଦି (libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, id, ଖାଲି, err) < 0)
    {
      libcrun_context_free (ctx);
      ଫେରନ୍ତୁ ଖାଲି;
    }

  ctx->handler_manager = libcrun_handler_manager_create (err);
  ଯଦି (ctx->handler_manager == ଖାଲି)
    {
      libcrun_context_free (ctx);
      ଫେରନ୍ତୁ ଖାଲି;
    }

#ifdef CRUN_LIBDIR
  ଯଦି (access (CRUN_LIBDIR "/handlers", F_OK) == 0)
    {
      ଯଦି (libcrun_handler_manager_load_directory (ctx->handler_manager, CRUN_LIBDIR "/handlers", err) < 0)
        {
          libcrun_context_free (ctx);
          ଫେରନ୍ତୁ ଖାଲି;
        }
    }
#endif

  ଫେରନ୍ତୁ ctx;
}

ଶୂନ୍ୟ
libcrun_context_free (libcrun_context_t *ctx)
{
  ଯଦି (ctx == ଖାଲି)
    ଫେରନ୍ତୁ;

  ମୁକ୍ତ (ctx->owned_state_ମୂଳ);
  ମୁକ୍ତ (ctx->owned_id);
  ମୁକ୍ତ (ctx->owned_bundle);
  ମୁକ୍ତ (ctx->owned_console_socket);
  ମୁକ୍ତ (ctx->owned_pid_file);
  ମୁକ୍ତ (ctx->owned_notify_socket);
  ମୁକ୍ତ (ctx->owned_handler);

  ଯଦି (ctx->handler_manager)
    handler_manager_free (ctx->handler_manager);

  ମୁକ୍ତ (ctx);
}

#define LIBCRUN_CONTEXT_SET_STRING(field)                  \
  କର                                                       \
    {                                                      \
      ମୁକ୍ତ (ctx->owned_##field);                           \
      ctx->owned_##field = ମୂଲ୍ୟ ? xstrdup (ମୂଲ୍ୟ) : ଖାଲି; \
      ctx->field = ctx->owned_##field;                     \
  } ଯେତେବେଳେ (0)

ଶୂନ୍ୟ
libcrun_context_set_id (libcrun_context_t *ctx, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ)
{
  LIBCRUN_CONTEXT_SET_STRING (id);
}

ଶୂନ୍ୟ
libcrun_context_set_state_ମୂଳ (libcrun_context_t *ctx, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ)
{
  LIBCRUN_CONTEXT_SET_STRING (state_ମୂଳ);
}

ଶୂନ୍ୟ
libcrun_context_set_bundle (libcrun_context_t *ctx, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ)
{
  LIBCRUN_CONTEXT_SET_STRING (bundle);
}

ଶୂନ୍ୟ
libcrun_context_set_console_socket (libcrun_context_t *ctx, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ)
{
  LIBCRUN_CONTEXT_SET_STRING (console_socket);
}

ଶୂନ୍ୟ
libcrun_context_set_pid_file (libcrun_context_t *ctx, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ)
{
  LIBCRUN_CONTEXT_SET_STRING (pid_file);
}

ଶୂନ୍ୟ
libcrun_context_set_notify_socket (libcrun_context_t *ctx, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ)
{
  LIBCRUN_CONTEXT_SET_STRING (notify_socket);
}

ଶୂନ୍ୟ
libcrun_context_set_handler (libcrun_context_t *ctx, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ)
{
  LIBCRUN_CONTEXT_SET_STRING (handler);
}

#undef LIBCRUN_CONTEXT_SET_STRING

ଅଚଳ ଅକ୍ଷର *
libcrun_context_get_pid_file (libcrun_context_t *ctx)
{
  ଫେରନ୍ତୁ ctx->pid_file;
}

ଅଚଳ ଅକ୍ଷର *
libcrun_context_get_state_ମୂଳ (libcrun_context_t *ctx)
{
  ଯଦି (ctx == ଖାଲି)
    ଫେରନ୍ତୁ ଖାଲି;
  ଫେରନ୍ତୁ ctx->state_ମୂଳ;
}

ଶୂନ୍ୟ
libcrun_context_set_preserve_fds (libcrun_context_t *ctx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା preserve_fds)
{
  ctx->preserve_fds = preserve_fds;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_context_get_preserve_fds (libcrun_context_t *ctx)
{
  ଫେରନ୍ତୁ ctx->preserve_fds;
}

ଶୂନ୍ୟ
libcrun_context_set_listen_fds (libcrun_context_t *ctx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା listen_fds)
{
  ctx->listen_fds = listen_fds;
}

ଶୂନ୍ୟ
libcrun_context_set_args (libcrun_context_t *ctx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ctx->argc = argc;
  ctx->argv = argv;
}

ଶୂନ୍ୟ
libcrun_context_set_systemd_cgroup (libcrun_context_t *ctx, bool ମୂଲ୍ୟ)
{
  ctx->systemd_cgroup = ମୂଲ୍ୟ;
}

ଶୂନ୍ୟ
libcrun_context_set_detach (libcrun_context_t *ctx, bool ମୂଲ୍ୟ)
{
  ctx->detach = ମୂଲ୍ୟ;
}

ଶୂନ୍ୟ
libcrun_context_set_no_new_keyring (libcrun_context_t *ctx, bool ମୂଲ୍ୟ)
{
  ctx->no_new_keyring = ମୂଲ୍ୟ;
}

ଶୂନ୍ୟ
libcrun_context_set_no_pivot (libcrun_context_t *ctx, bool ମୂଲ୍ୟ)
{
  ctx->no_pivot = ମୂଲ୍ୟ;
}

ଶୂନ୍ୟ
libcrun_context_set_force_no_cgroup (libcrun_context_t *ctx, bool ମୂଲ୍ୟ)
{
  ctx->force_no_cgroup = ମୂଲ୍ୟ;
}

ଶୂନ୍ୟ
libcrun_context_set_output_handler (libcrun_context_t *ctx, crun_output_handler handler, ଶୂନ୍ୟ *arg)
{
  ctx->output_handler = handler;
  ctx->output_handler_arg = arg;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_context_init_logging (libcrun_context_t *ctx, ଅଚଳ ଅକ୍ଷର *log, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ libcrun_init_logging (&ctx->output_handler, &ctx->output_handler_arg, ctx->id, log, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
block_signals (libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  sigset_t mask;
  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "sigprocmask");
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
unblock_signals (libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  sigset_t mask;
  ସଂରଚନା sigaction act = {};

  sigfillset (&mask);
  ret = sigprocmask (SIG_UNBLOCK, &mask, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "sigprocmask");

  act.sa_handler = SIG_DFL;
  ପାଇଁ (i = 0; i < NSIG; i++)
    {
      ret = sigaction (i, &act, ଖାଲି);
      ଯଦି (ret < 0 && errno != EINVAL)
        ଫେରନ୍ତୁ crun_make_error (err, errno, "sigaction");
    }

  ଫେରନ୍ତୁ 0;
}

/* must be used on the host before pivot_root(2).  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
initialize_security (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = libcrun_init_caps (container, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (UNLIKELY (proc == ଖାଲି))
    ଫେରନ୍ତୁ 0;

  ଯଦି (proc->apparmor_profile)
    {
      ret = libcrun_initialize_apparmor (err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ret = libcrun_initialize_selinux (container, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ 0;
}

/* Emit the "annotations" map.  Returns a json_gen_status; callers can wrap
   the call with GEN_OR_FAIL.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
gen_annotations (json_gen_ctx *gen, json_map_string_string *annotations)
{
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା r;

  GEN_ଚାବି (gen, "annotations");
  GEN_OR_FAIL (json_gen_map_open (gen));

  ପାଇଁ (i = 0; i < annotations->len; i++)
    {
      /* An annotation whose value is null in the document is parsed into a
         NULL, which cannot be emitted as a JSON string; leave it out.  */
      ଯଦି (annotations->ଚାବିଗୁଡ଼ିକ[i] == ଖାଲି || annotations->ମୂଲ୍ୟଗୁଡ଼ିକ[i] == ଖାଲି)
        ଜାରି;

      GEN_STR (gen, annotations->ଚାବିଗୁଡ଼ିକ[i]);
      GEN_STR (gen, annotations->ମୂଲ୍ୟଗୁଡ଼ିକ[i]);
    }

  ଫେରନ୍ତୁ json_gen_map_close (gen);

gen_error:
  ଫେରନ୍ତୁ r;
}

/* Return the bundle path to report in the container state.  crun always
   chdir()s into the bundle, so fall back to getcwd() when the caller has no
   absolute bundle path, as can happen when libcrun is used as a library.
   *ALLOCATED must be freed by the caller.  */
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
get_bundle_path (ଅଚଳ ଅକ୍ଷର *bundle, ଅକ୍ଷର **allocated)
{
  ଯଦି (bundle != ଖାଲି && bundle[0] == '/')
    ଫେରନ୍ତୁ bundle;

  *allocated = getcwd (ଖାଲି, 0);
  ଯଦି (*allocated == ଖାଲି)
    OOM ();
  ଫେରନ୍ତୁ *allocated;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
do_hooks (runtime_spec_schema_config_schema *def, pid_t pid, ଅଚଳ ଅକ୍ଷର *id, bool keep_going, ଅଚଳ ଅକ୍ଷର *bundle,
          ଅଚଳ ଅକ୍ଷର *cwd, ଅଚଳ ଅକ୍ଷର *status, ଅଚଳ ଅକ୍ଷର *name, hook **hooks, size_t hooks_len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_fd,
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err)
{
  size_t i, stdin_len;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା r, ret;
  ଅକ୍ଷର *stdin = ଖାଲି;
  cleanup_free ଅକ୍ଷର *bundle_allocated = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *rootfs = def->ମୂଳ ? def->ମୂଳ->path : "";
  cleanup_json_gen json_gen_ctx *gen = ଖାଲି;

  bundle = get_bundle_path (bundle, &bundle_allocated);

  ଯଦି (! json_gen_init (&gen, ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 0);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_ଚାବି (gen, "ociVersion");
  GEN_STR (gen, "1.0");

  GEN_ଚାବି (gen, "id");
  GEN_STR (gen, id);

  GEN_ଚାବି (gen, "pid");
  GEN_OR_FAIL (map_int (gen, pid));

  GEN_ଚାବି (gen, "root");
  GEN_STR (gen, rootfs);

  GEN_ଚାବି (gen, "bundle");
  GEN_STR (gen, bundle);

  GEN_ଚାବି (gen, "status");
  GEN_STR (gen, status);

  ଯଦି (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, (ଅଚଳ ଅକ୍ଷର **) &stdin, &stdin_len));

  ret = 0;

  bool error_created = false;
  ପାଇଁ (i = 0; i < hooks_len; i++)
    {
      ଅକ୍ଷର **env = environ;

      ଯଦି (hooks[i]->env)
        env = hooks[i]->env;

      /* Release the error from the previous iteration, if any.  */
      ଯଦି (error_created)
        {
          crun_error_release (err);
          error_created = false;
        }

      ret = run_process_with_stdin_timeout_envp (hooks[i]->path, hooks[i]->args, cwd, hooks[i]->timeout, env,
                                                 stdin, stdin_len, out_fd, err_fd, can_ignore_chdir_errors, err);
      ଯଦି (UNLIKELY (ret < 0))
        error_created = true;

      ଯଦି (UNLIKELY (ret != 0))
        {
          ଯଦି (keep_going)
            libcrun_warning ("error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
          ଅନ୍ୟଥା
            {
              libcrun_error (0, "error running %s hook #%zu: `%s` (exit code: %d)", name, i, hooks[i]->path, ret);
              ଭାଙ୍ଗ;
            }
        }
    }

  ଫେରନ୍ତୁ ret;

gen_error:
  ଫେରନ୍ତୁ json_gen_error_to_crun_error (r, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_json_gen_result (json_gen_ctx *gen, ଅକ୍ଷର **out, size_t *out_len)
{
  ଅଚଳ ଅକ୍ଷର *buf = ଖାଲି;
  size_t buf_len = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା r;

  r = json_gen_get_buf (gen, &buf, &buf_len);
  ଯଦି (UNLIKELY (r != json_gen_status_ok))
    ଫେରନ୍ତୁ r;

  *out_len = buf_len;

  *out = xmalloc (buf_len + 1);
  memcpy (*out, buf, buf_len);
  (*out)[buf_len] = '\0';

  ଫେରନ୍ତୁ json_gen_status_ok;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_seccomp_receiver_fd_payload (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *status, pid_t own_pid,
                                 ଅକ୍ଷର **seccomp_fd_payload, size_t *seccomp_fd_payload_len, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା r;
  json_gen_ctx *gen = ଖାଲି;
  runtime_spec_schema_config_schema *def = container->container_def;
  ଅଚଳ ଅକ୍ଷର *ଅଚଳ OCI_VERSION = "0.2.0";

  ଯଦି (! json_gen_init (&gen, ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_ଚାବି (gen, "ociVersion");
  GEN_STR (gen, OCI_VERSION);

  GEN_ଚାବି (gen, "fds");
  GEN_OR_FAIL (json_gen_array_open (gen));

  GEN_ଚାବି (gen, "seccompFd");
  GEN_OR_FAIL (json_gen_array_close (gen));

  GEN_ଚାବି (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  ଯଦି (def && def->linux && def->linux->seccomp)
    {
      ଅଚଳ ଅକ୍ଷର *metadata = def->linux->seccomp->listener_metadata;

      ଯଦି (metadata)
        {
          GEN_ଚାବି (gen, "metadata");
          GEN_STR (gen, metadata);
        }
    }

  /* State.  */
  GEN_ଚାବି (gen, "state");
  GEN_OR_FAIL (json_gen_map_open (gen));

  GEN_ଚାବି (gen, "ociVersion");
  GEN_STR (gen, OCI_VERSION);

  ଯଦି (container->context && container->context->id)
    {
      GEN_ଚାବି (gen, "id");
      GEN_STR (gen, container->context->id);
    }

  GEN_ଚାବି (gen, "status");
  GEN_STR (gen, status);

  GEN_ଚାବି (gen, "pid");
  GEN_OR_FAIL (map_int (gen, own_pid));

  ଯଦି (container->context && container->context->bundle)
    {
      GEN_ଚାବି (gen, "bundle");
      GEN_STR (gen, container->context->bundle);
    }

  ଯଦି (def && def->annotations && def->annotations->len)
    GEN_OR_FAIL (gen_annotations (gen, def->annotations));

  GEN_OR_FAIL (json_gen_map_close (gen));
  /* End state.  */

  GEN_OR_FAIL (json_gen_map_close (gen));

  r = get_json_gen_result (gen, seccomp_fd_payload, seccomp_fd_payload_len);

gen_error:
  json_gen_free (gen);

  ଫେରନ୍ତୁ json_gen_error_to_crun_error (r, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
send_sync_cb (ଶୂନ୍ୟ *data, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket_fd = *((ପୂର୍ଣ୍ଣସଂଖ୍ୟା *) data);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  /* sync 2.  */
  ret = sync_socket_send_sync (sync_socket_fd, false, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  /* sync 3.  */
  ଫେରନ୍ତୁ sync_socket_wait_sync (ଖାଲି, sync_socket_fd, false, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
maybe_chown_std_streams (uid_t container_uid, gid_t container_gid, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret, i;

  ପାଇଁ (i = 0; i < 3; i++)
    {
      ଯଦି (! isatty (i))
        {
          ସଂରଚନା stat statbuf;
          ret = fstat (i, &statbuf);
          ଯଦି (UNLIKELY (ret < 0))
            {
              ଯଦି (errno == EBADF)
                ଜାରି;
              ଫେରନ୍ତୁ crun_make_error (err, errno, "fstat fd `%d`", i);
            }

          /* Skip chown for device files */
          ଯଦି (S_ISCHR (statbuf.st_mode) || S_ISBLK (statbuf.st_mode))
            ଜାରି;

          ret = fchown (i, container_uid, container_gid);
          ଯଦି (UNLIKELY (ret < 0))
            {
              /* EINVAL means the user is not mapped in the current userns.
                 Ignore EPERM and EROFS as well as there is no reason to fail
                 so early, and let the container payload deal with it.
                 EBADF means fd is closed.
              */
              ଯଦି (errno == EINVAL || errno == EPERM || errno == EROFS || errno == EBADF)
                ଜାରି;

              ଫେରନ୍ତୁ crun_make_error (err, errno, "fchown std stream `%i`", i);
            }
        }
    }
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_notify_handler (ସଂରଚନା container_entrypoint_s *args,
                                  ଗଣନା handler_configure_phase phase,
                                  libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *rootfs,
                                  libcrun_error_t *err)
{
  ସଂରଚନା custom_handler_s *h;

  ଯଦି (args->custom_handler == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  h = args->custom_handler->vtable;
  ଯଦି (h == ଖାଲି || h->configure_container == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  ଫେରନ୍ତୁ h->configure_container (args->custom_handler->cookie, phase,
                                 args->context, container,
                                 rootfs, err);
}

/* Resolve and normalize the container rootfs path.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
resolve_rootfs_path (libcrun_container_t *container, ଅକ୍ଷର **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ଯଦି (def->ମୂଳ && def->ମୂଳ->path)
    {
      *rootfs = realpath (def->ମୂଳ->path, ଖାଲି);
      ଯଦି (UNLIKELY (*rootfs == ଖାଲି))
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;

          /* If realpath failed for any reason, try the relative directory.  */
          ଯଦି (def->ମୂଳ->path[0] == '/')
            {
              cleanup_free ଅକ୍ଷର *cwd = ଖାଲି;
              libcrun_error_t tmp_err = ଖାଲି;
              ssize_t len;
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା proc_fd;

              proc_fd = libcrun_get_cached_proc_fd (container, &tmp_err);
              ଯଦି (LIKELY (proc_fd >= 0))
                {
                  len = safe_readlinkat (proc_fd, "self/cwd", &cwd, 0, &tmp_err);
                  ଯଦି (LIKELY (len > 0))
                    {
                      /* If the rootfs is under the current working directory, just use its relative path.  */
                      ଯଦି (has_dir_prefix (def->ମୂଳ->path, cwd))
                        {
                          ଅଚଳ ଅକ୍ଷର *it = consume_slashes (def->ମୂଳ->path + len);
                          ଯଦି (LIKELY (*it))
                            {
                              ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = faccessat (AT_FDCWD, it, F_OK, 0);
                              ଯଦି (LIKELY (ret == 0))
                                *rootfs = xstrdup (it);
                            }
                        }
                    }
                }
              crun_error_release (&tmp_err);
            }

          ଯଦି (*rootfs == ଖାଲି)
            {
              ଯଦି (saved_errno == ENOENT)
                ଫେରନ୍ତୁ crun_make_error (err, ENOENT, "the rootfs path `%s` does not exist", def->ମୂଳ->path);

              *rootfs = xstrdup (def->ମୂଳ->path);
            }
        }
    }
  ଫେରନ୍ତୁ 0;
}

/* Initialize the environment variables.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
setup_environment (runtime_spec_schema_config_schema *def, uid_t container_uid, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (clearenv ())
    ଫେରନ୍ତୁ crun_make_error (err, errno, "clearenv");

  ଯଦି (def->process)
    {
      size_t i;

      ପାଇଁ (i = 0; i < def->process->env_len; i++)
        {
          /* A null element of the array of variables is parsed into a NULL,
             which putenv(3) cannot be handed.  */
          ଯଦି (def->process->env[i] == ଖାଲି)
            ଫେରନ୍ତୁ crun_make_error (err, EINVAL, "environment variable is not specified");

          ଯଦି (putenv (def->process->env[i]) < 0)
            ଫେରନ୍ତୁ crun_make_error (err, errno, "putenv `%s`", def->process->env[i]);
        }
    }

  ଯଦି (is_empty_string (getenv ("HOME")))
    {
      ret = set_home_env (container_uid);
      ଯଦି (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          ଯଦି (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "setenv HOME");
        }
    }

  ଫେରନ୍ତୁ 0;
}

/* Configure terminal and console for interactive containers.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
setup_terminal (ସଂରଚନା container_entrypoint_s *entrypoint_args, libcrun_container_t *container,
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା console_socket, ପୂର୍ଣ୍ଣସଂଖ୍ୟା console_socketpair, ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_terminal, libcrun_error_t *err)
{
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା terminal_fd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (! has_terminal)
    ଫେରନ୍ତୁ 0;

  fflush (stderr);

  ଯଦି (console_socket >= 0 || (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0))
    {
      terminal_fd = libcrun_set_terminal (container, err);
      ଯଦି (UNLIKELY (terminal_fd < 0))
        ଫେରନ୍ତୁ terminal_fd;
    }

  ଯଦି (console_socket >= 0)
    {
      ret = send_fd_to_socket (console_socket, terminal_fd, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଅନ୍ୟଥା ଯଦି (entrypoint_args->has_terminal_socket_pair && console_socketpair >= 0)
    {
      ret = send_fd_to_socket (console_socketpair, terminal_fd, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଫେରନ୍ତୁ 0;
}

/* Resolve and validate the container's executable path.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
setup_executable_path (ସଂରଚନା container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                       ଅକ୍ଷର **exec_path, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (def->process && def->process->args)
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      ଯଦି (UNLIKELY (ret < 0))
        {
          ଯଦି (entrypoint_args->custom_handler == ଖାଲି && crun_error_get_errno (err) == ENOENT)
            ଫେରନ୍ତୁ ret;

          /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
             once the process switched to the use that runs in the container.  This might be necessary
             when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
             is not honored.  */
          crun_error_release (err);
        }
    }

  ଫେରନ୍ତୁ 0;
}

/* Apply security settings for the container process.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
apply_security_settings (ସଂରଚନା container_entrypoint_s *entrypoint_args, runtime_spec_schema_config_schema *def,
                         libcrun_container_t *container, pid_t own_pid, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  runtime_spec_schema_config_schema_process_capabilities *capabilities;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା no_new_privs;

  ଯଦି (def->process && ! def->process->no_new_privileges)
    {
      ଅକ୍ଷର **seccomp_flags = ଖାଲି;
      size_t seccomp_flags_len = 0;
      cleanup_free ଅକ୍ଷର *seccomp_fd_payload = ଖାଲି;
      size_t seccomp_fd_payload_len = 0;

      ଯଦି (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      ଯଦି (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  capabilities = def->process ? def->process->capabilities : ଖାଲି;
  no_new_privs = def->process ? def->process->no_new_privileges : 1;
  ret = libcrun_set_caps (capabilities, container->container_uid, container->container_gid, no_new_privs,
                          entrypoint_args->custom_handler != ଖାଲି, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ 0;
}

/* Create the process that joins the existing PID namespace now that the
   host mounts are not visible anymore, and continue the setup from there.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
do_delayed_pidns_fork (ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket, pid_t *own_pid, libcrun_error_t *err)
{
  pid_t pid;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  pid = fork ();
  ଯଦି (UNLIKELY (pid < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "fork");

  ଯଦି (pid)
    {
      /* Report the new PID to the parent and exit immediately.  */
      ret = sync_socket_send_sync (sync_socket, false, err);
      ଯଦି (LIKELY (ret == 0))
        {
          ret = TEMP_FAILURE_RETRY (ଲିଖ (sync_socket, &pid, ଆକାର (pid)));
          ଯଦି (LIKELY (ret == ଆକାର (pid)))
            _safe_exit (EXIT_SUCCESS);
        }

      kill (pid, SIGKILL);
      _safe_exit (EXIT_FAILURE);
    }

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (ପଠ (sync_socket, own_pid, ଆକାର (*own_pid)));
  ଯଦି (UNLIKELY (ret != ଆକାର (*own_pid)))
    {
      ଯଦି (ret >= 0)
        errno = 0;
      ଫେରନ୍ତୁ crun_make_error (err, errno, "read from sync socket");
    }

  ଫେରନ୍ତୁ 0;
}

ସଂରଚନା delayed_pidns_fork_s
{
  libcrun_container_t *container;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket;
  pid_t *own_pid;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
delayed_pidns_fork_cb (ଶୂନ୍ୟ *data, libcrun_error_t *err)
{
  ସଂରଚନା delayed_pidns_fork_s *d = data;

  ଯଦି (! libcrun_container_delayed_pidns_fork (d->container))
    ଫେରନ୍ତୁ 0;

  ଫେରନ୍ତୁ do_delayed_pidns_fork (d->sync_socket, d->own_pid, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
container_init_setup (ଶୂନ୍ୟ *args, pid_t *own_pid, ଅକ୍ଷର *notify_socket,
                      ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket, ଅକ୍ଷର **exec_path, libcrun_error_t *err)
{
  ସଂରଚନା container_entrypoint_s *entrypoint_args = args;
  libcrun_container_t *container = entrypoint_args->container;
  bool chdir_done = false;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_terminal;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା console_socket = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା console_socketpair = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free ଅକ୍ଷର *rootfs = ଖାଲି;
  ସଂରଚନା delayed_pidns_fork_s delayed_pidns_fork = {
    .container = container,
    .sync_socket = sync_socket,
    .own_pid = own_pid,
  };

  ret = initialize_security (container, def->process, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_configure_network (container, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = resolve_rootfs_path (container, &rootfs, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  /* Configure terminal socket pair for container communication.  */
  ଯଦି (entrypoint_args->terminal_socketpair[0] >= 0)
    {
      close_and_reset (&entrypoint_args->terminal_socketpair[0]);
      console_socketpair = entrypoint_args->terminal_socketpair[1];
    }

  /* sync 1.  */
  ret = sync_socket_wait_sync (ଖାଲି, sync_socket, false, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  has_terminal = container->container_def->process && container->container_def->process->terminal;
  ଯଦି (has_terminal && entrypoint_args->context->console_socket)
    console_socket = entrypoint_args->console_socket_fd;

  ret = libcrun_set_sysctl (container, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_BEFORE_MOUNTS, container, rootfs, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_do_pivot_ମୂଳ (container, entrypoint_args->context->no_pivot, &rootfs, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  /* sync 2 and 3 are sent as part of libcrun_set_mounts.  */
  ret = libcrun_set_mounts (entrypoint_args, container, rootfs, send_sync_cb, &sync_socket, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  /* The createContainer hooks MUST run after the mounts are set up but before
     the pivot_root.  When these hooks are present can_use_open_tree_namespace()
     forces the deferred-pivot path, so the actual pivot happens later in
     libcrun_finalize_mounts().  */
  ଯଦି (def->hooks && def->hooks->create_container_len)
    {
      libcrun_error_t tmp_err = ଖାଲି;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା in_userns = check_running_in_user_namespace (&tmp_err);
      ଯଦି (tmp_err)
        crun_error_release (&tmp_err);
      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, ଖାଲି, "created", "createContainer",
                      (hook **) def->hooks->create_container,
                      def->hooks->create_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      ଯଦି (UNLIKELY (ret != 0))
        ଫେରନ୍ତୁ ret;
    }

  ret = libcrun_finalize_mounts (entrypoint_args, container, rootfs, delayed_pidns_fork_cb, &delayed_pidns_fork, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (def->process)
    {
      ret = libcrun_set_selinux_label (container, def->process, false, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = libcrun_set_apparmor_profile (container, def->process, false, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ret = mark_or_close_fds_ge_than (container, entrypoint_args->context->preserve_fds + 3, false, err);
  ଯଦି (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ret = libcrun_reopen_dev_null (err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = maybe_chown_std_streams (container->container_uid, container->container_gid, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = setup_environment (def, container->container_uid, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  /* Set primary process to 1 explicitly if nothing is configured and LISTEN_FD is not set.  */
  ଯଦି (entrypoint_args->context->listen_fds > 0 && getenv ("LISTEN_PID") == ଖାଲି)
    {
      libcrun_warning ("setting LISTEN_PID=1 since no previous configuration was found");
      ଯଦି (UNLIKELY (setenv ("LISTEN_PID", "1", 1) < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "setenv LISTEN_PID");
    }

  /* Attempt to chdir immediately here, before doing the setresuid.  If we fail here, let's
     try again later once the process switched to the user that runs in the container.  */
  ଯଦି (def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      ଯଦି (LIKELY (ret == 0))
        chdir_done = true;
      ଅନ୍ୟଥା
        crun_error_release (err);
    }

  ret = setsid ();
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "setsid");

  ret = setup_terminal (entrypoint_args, container, console_socket, console_socketpair, has_terminal, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  close_and_reset (&console_socket);
  close_and_reset (&console_socketpair);

  ret = setup_executable_path (entrypoint_args, def, exec_path, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_set_hostname (container, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_set_domainname (container, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (def->process && def->process->user)
    umask (def->process->user->umask_present ? def->process->user->umask : 0022);

  ret = apply_security_settings (entrypoint_args, def, container, *own_pid, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (UNLIKELY (def->process && def->process->args && *exec_path == ଖାଲି))
    {
      ret = find_executable (exec_path, def->process->args[0], def->process->cwd, err);
      ଯଦି (UNLIKELY (ret < 0))
        {
          ଯଦି (entrypoint_args->custom_handler == ଖାଲି || is_empty_string (def->process->args[0]))
            ଫେରନ୍ତୁ ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          *exec_path = xstrdup (def->process->args[0]);
        }
    }

  /* The chdir was not already performed, so try again now after switching to the UID/GID in the container.  */
  ଯଦି (! chdir_done && def->process && def->process->cwd)
    {
      ret = libcrun_safe_chdir (def->process->cwd, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (notify_socket && putenv (notify_socket) < 0)
    ଫେରନ୍ତୁ crun_make_error (err, errno, "putenv `%s`", notify_socket);

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
open_hooks_output (libcrun_container_t *container, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *out_fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *err_fd, libcrun_error_t *err)
{
  ଅଚଳ ଅକ୍ଷର *annotation;

  *err_fd = *out_fd = -1;

  libcrun_debug ("Opening hooks output");
  annotation = find_annotation (container, "run.oci.hooks.stdout");
  ଯଦି (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stdout` annotation");
      *out_fd = TEMP_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      ଯଦି (UNLIKELY (*out_fd < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", annotation);
    }

  annotation = find_annotation (container, "run.oci.hooks.stderr");
  ଯଦି (annotation)
    {
      libcrun_debug ("Found `run.oci.hooks.stderr` annotation");
      *err_fd = TEMP_FAILURE_RETRY (open (annotation, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC, 0700));
      ଯଦି (UNLIKELY (*err_fd < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", annotation);
    }

  ଫେରନ୍ତୁ 0;
}

/* The environment the process was started with lives on the initial stack, right after the
   argv strings and before the string AT_EXECFN points to.  clearenv(3) only drops the pointers
   to it, so when there is no execv to replace the stack, erase it or the container process can
   still read it, e.g. through /proc/self/environ.  */
ସ୍ଥିର ଶୂନ୍ୟ
erase_initial_environment (ଅକ୍ଷର **argv, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc)
{
  ଅକ୍ଷର *start = argv[argc - 1] + strlen (argv[argc - 1]) + 1;
  ଅକ୍ଷର *end = (ଅକ୍ଷର *) getauxval (AT_EXECFN);

  ଯଦି (end > start)
    memset (start, 0, end - start);
}

ସ୍ଥିର ଶୂନ୍ୟ
rewrite_argv (ଅକ୍ଷର **argv, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର *name, ଅକ୍ଷର **args, size_t args_len)
{
  cleanup_free ଅକ୍ଷର *decorated_name = ଖାଲି;
  size_t available_len = argv[argc - 1] - argv[0] + strlen (argv[argc - 1]) + 1;
  cleanup_free ଅକ୍ଷର *new_argv = ଖାଲି;
  size_t i, so_far = 0, needed = 0;

  needed = xasprintf (&decorated_name, "[libcrun:%s]", name) + 1;
  ପାଇଁ (i = 0; i < args_len; i++)
    needed += strlen (args[i]) + 1;

  new_argv = xmalloc0 (needed + 1);
  strcpy (new_argv, decorated_name);
  so_far = strlen (decorated_name) + 1;

  ଯଦି (available_len >= needed)
    {
      ପାଇଁ (i = 0; i < args_len; i++)
        {
          strcpy (new_argv + so_far, args[i]);
          so_far += strlen (args[i]) + 1;
        }
    }

  ଯଦି (so_far >= available_len)
    so_far = available_len - 1;

  memset (argv[0], 0, available_len);
  memcpy (argv[0], new_argv, so_far);
}

/* Entrypoint to the container.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
container_init (ଶୂନ୍ୟ *args, ଅକ୍ଷର *notify_socket, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket, libcrun_error_t *err)
{
  ସଂରଚନା container_entrypoint_s *entrypoint_args = args;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  runtime_spec_schema_config_schema *def = entrypoint_args->container->container_def;
  cleanup_free ଅକ୍ଷର *exec_path = ଖାଲି;
  __attribute__ ((unused)) cleanup_free ଅକ୍ଷର *notify_socket_cleanup = notify_socket;
  pid_t own_pid = 0;

  entrypoint_args->sync_socket = sync_socket;

  crun_set_output_handler (log_write_to_sync_socket, args);

  /* sync receive own pid.  */
  ret = TEMP_FAILURE_RETRY (ପଠ (sync_socket, &own_pid, ଆକାର (own_pid)));
  ଯଦି (UNLIKELY (ret != ଆକାର (own_pid)))
    {
      ଯଦି (ret >= 0)
        errno = 0;
      ଫେରନ୍ତୁ crun_make_error (err, errno, "read from sync socket");
    }

  ret = container_init_setup (args, &own_pid, notify_socket, sync_socket, &exec_path, err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      /* If it fails to write the error using the sync socket, then fallback
         to stderr.  */
      ଯଦି (sync_socket_write_error (sync_socket, err) == 0)
        {
          crun_error_release (err);
          /* A return value of > 0 means to fail with an error without err being set so to avoid
             printing it twice.  */
          ଫେରନ୍ତୁ 1;
        }
      ଫେରନ୍ତୁ ret;
    }

  entrypoint_args->sync_socket = -1;

  ret = unblock_signals (err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  /* sync 4.  */
  ret = sync_socket_send_sync (sync_socket, false, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  close_and_reset (&sync_socket);

  ଯଦି (entrypoint_args->context->fifo_exec_wait_fd >= 0)
    {
      ଅକ୍ଷର buffer[1];
      fd_set read_set;
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = entrypoint_args->context->fifo_exec_wait_fd;
      entrypoint_args->context->fifo_exec_wait_fd = -1;

      FD_ZERO (&read_set);
      FD_SET (fd, &read_set);
      କର
        {
          ret = select (fd + 1, &read_set, ଖାଲି, ଖାଲି, ଖାଲି);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "select");

          ret = TEMP_FAILURE_RETRY (ପଠ (fd, buffer, ଆକାର (buffer)));
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "read from the exec fifo");
      } ଯେତେବେଳେ (ret == 0);

      close_and_reset (&entrypoint_args->context->fifo_exec_wait_fd);
    }

  crun_set_output_handler (log_write_to_stderr, ଖାଲି);

  ଯଦି (def->process && def->process->no_new_privileges)
    {
      ଅକ୍ଷର **seccomp_flags = ଖାଲି;
      size_t seccomp_flags_len = 0;
      cleanup_free ଅକ୍ଷର *seccomp_fd_payload = ଖାଲି;
      size_t seccomp_fd_payload_len = 0;

      ଯଦି (def->linux && def->linux->seccomp)
        {
          seccomp_flags = def->linux->seccomp->flags;
          seccomp_flags_len = def->linux->seccomp->flags_len;
        }

      ଯଦି (entrypoint_args->seccomp_receiver_fd >= 0)
        {
          ret = get_seccomp_receiver_fd_payload (entrypoint_args->container, "creating", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }

      ret = libcrun_apply_seccomp (entrypoint_args->seccomp_fd, entrypoint_args->seccomp_receiver_fd,
                                   seccomp_fd_payload, seccomp_fd_payload_len, seccomp_flags,
                                   seccomp_flags_len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
      close_and_reset (&entrypoint_args->seccomp_fd);
      close_and_reset (&entrypoint_args->seccomp_receiver_fd);
    }

  ଯଦି (UNLIKELY (def->process == ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "block `process` not found");

  ଯଦି (UNLIKELY (exec_path == ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "executable path not specified");

  ଯଦି (def->hooks && def->hooks->start_container_len)
    {
      libcrun_container_t *container = entrypoint_args->container;
      libcrun_error_t tmp_err = ଖାଲି;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା in_userns = check_running_in_user_namespace (&tmp_err);
      ଯଦି (tmp_err)
        crun_error_release (&tmp_err);

      ret = do_hooks (def, getpid (), container->context->id, false, entrypoint_args->bundle, ଖାଲି, "created", "startContainer",
                      (hook **) def->hooks->start_container,
                      def->hooks->start_container_len, entrypoint_args->hooks_out_fd, entrypoint_args->hooks_err_fd,
                      in_userns > 0, err);
      ଯଦି (UNLIKELY (ret != 0))
        ଫେରନ୍ତୁ ret;

      /* Seek stdout/stderr to the end.  If the hooks were using the same files,
         the container process overwrites what was previously written.  */
      (ଶୂନ୍ୟ) lseek (1, 0, SEEK_END);
      (ଶୂନ୍ୟ) lseek (2, 0, SEEK_END);
    }

  ଯଦି (entrypoint_args->custom_handler)
    {
      /* Files marked with O_CLOEXEC are closed at execv time, so make sure they are closed now.
         This is a best effort operation, because the seccomp filter is already in place and it could
         stop some syscalls used by mark_or_close_fds_ge_than.
      */
      ଯଦି (entrypoint_args->custom_handler->vtable->close_fds)
        {
          ret = entrypoint_args->custom_handler->vtable->close_fds (entrypoint_args->custom_handler->cookie,
                                                                    entrypoint_args->container,
                                                                    entrypoint_args->context->preserve_fds,
                                                                    err);
        }
      ଅନ୍ୟଥା
        {
          ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
        }
      ଯଦି (UNLIKELY (ret < 0))
        crun_error_release (err);

      prctl (PR_SET_NAME, entrypoint_args->custom_handler->vtable->name);

      ଯଦି (entrypoint_args->context->argv)
        {
          erase_initial_environment (entrypoint_args->context->argv, entrypoint_args->context->argc);
          rewrite_argv (entrypoint_args->context->argv, entrypoint_args->context->argc,
                        entrypoint_args->custom_handler->vtable->name, def->process->args,
                        def->process->args_len);

          /* It is a quite destructive operation as we might be referencing data from the old
             argv, so make sure the context is not reused.  */
          entrypoint_args->context->argv = ଖାଲି;
          entrypoint_args->context = ଖାଲି;
        }

      ret = libcrun_set_selinux_label (entrypoint_args->container, def->process, true, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = libcrun_set_apparmor_profile (entrypoint_args->container, def->process, true, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = entrypoint_args->custom_handler->vtable->run_func (entrypoint_args->custom_handler->cookie,
                                                               entrypoint_args->container,
                                                               exec_path,
                                                               def->process->args);
      ଯଦି (ret != 0)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "exec container process failed with handler as `%s`", entrypoint_args->custom_handler->vtable->name);

      ଫେରନ୍ତୁ ret;
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation since the seccomp profile is already in place now and might block
     some of the syscalls needed by mark_or_close_fds_ge_than.  */
  ret = mark_or_close_fds_ge_than (entrypoint_args->container, entrypoint_args->context->preserve_fds + 3, true, err);
  ଯଦି (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, def->process->args));

  ଯଦି (errno == ENOENT)
    ଫେରନ୍ତୁ crun_make_error (err, errno, "exec container process (missing dynamic library?) `%s`", exec_path);

  ଫେରନ୍ତୁ crun_make_error (err, errno, "exec container process `%s`", exec_path);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
read_container_config_from_state (libcrun_container_t **container, ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, ଅଚଳ ଅକ୍ଷର *id,
                                  libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *config_file = ଖାଲି;
  cleanup_free ଅକ୍ଷର *dir = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  *container = ଖାଲି;

  ret = libcrun_get_state_directory (&dir, state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = append_paths (&config_file, err, dir, "config.json", ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  *container = libcrun_container_load_from_file (config_file, err);
  ଯଦି (*container == ଖାଲି)
    ଫେରନ୍ତୁ -1;

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
run_poststop_hooks (arg_unused libcrun_context_t *context, libcrun_container_t *container,
                    runtime_spec_schema_config_schema *def,
                    libcrun_container_status_t *status, ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container_cleanup = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (def == ଖାଲି)
    {
      ଯଦି (container == ଖାଲି)
        {
          ret = read_container_config_from_state (&container_cleanup, state_ମୂଳ, id, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
          container = container_cleanup;
        }

      def = container->container_def;
    }

  ଯଦି (def->hooks && def->hooks->poststop_len)
    {
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା hooks_out_fd = -1;
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା hooks_err_fd = -1;

      ଯଦି (container == ଖାଲି)
        {
          ret = read_container_config_from_state (&container_cleanup, state_ମୂଳ, id, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
          container = container_cleanup;
        }

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = do_hooks (def, 0, id, true, status->bundle, status->bundle, "stopped", "poststop", (hook **) def->hooks->poststop,
                      def->hooks->poststop_len, hooks_out_fd, hooks_err_fd, false, err);
      ଯଦି (UNLIKELY (ret != 0))
        {
          ଯଦି (ret < 0)
            libcrun_error_report_and_release (err);
          ଅନ୍ୟଥା
            libcrun_error (0, "poststop hook failed with exit code: %d", ret);
        }
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର bool
has_new_pid_namespace (runtime_spec_schema_config_schema *def)
{
  size_t i;

  ଯଦି (def->linux == ଖାଲି || def->linux->namespaces == ଖାଲି)
    ଫେରନ୍ତୁ false;

  ପାଇଁ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ଯଦି (strcmp (def->linux->namespaces[i]->type, "pid") == 0 && def->linux->namespaces[i]->path == ଖାଲି)
        ଫେରନ୍ତୁ true;
    }
  ଫେରନ୍ତୁ false;
}

/* Delete the container state.  CONTAINER is the container the caller already has
   available, or NULL.  When it is not NULL, the poststop hooks are run even if the
   status file was never written, e.g. when the container creation failed.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
container_delete_internal (libcrun_context_t *context, libcrun_container_t *container,
                           runtime_spec_schema_config_schema *def, ଅଚଳ ଅକ୍ଷର *id, bool force, bool killall,
                           libcrun_error_t *err)
{
  cleanup_cgroup_status ସଂରଚନା libcrun_cgroup_status *cgroup_status = ଖାଲି;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_container libcrun_container_t *container_cleanup = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *state_ମୂଳ = context->state_ମୂଳ;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = libcrun_read_container_status (&status, state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ଯଦି (force && crun_error_get_errno (err) == ENOENT)
        {
          libcrun_error_t tmp_err = ଖାଲି;

          crun_error_release (err);

          ଯଦି (container)
            {
              libcrun_container_status_t partial_status = {
                .bundle = (ଅକ୍ଷର *) context->bundle,
              };

              ret = run_poststop_hooks (context, container, def, &partial_status, state_ମୂଳ, id, err);
              ଯଦି (UNLIKELY (ret < 0))
                crun_error_write_warning_and_release (context->output_handler_arg, &err);
            }

          libcrun_container_delete_status (state_ମୂଳ, id, &tmp_err);
          crun_error_release (&tmp_err);
          ଫେରନ୍ତୁ 0;
        }
      crun_error_release (err);
      ଫେରନ୍ତୁ libcrun_container_delete_status (state_ମୂଳ, id, err);
    }

  cgroup_status = libcrun_cgroup_make_status (&status);

  ଯଦି (! force)
    {
      ret = libcrun_is_container_running (&status, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
      ଯଦି (ret == 1)
        {
          ret = libcrun_status_has_read_exec_fifo (state_ମୂଳ, id, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
          ଯଦି (ret == 0)
            ଫେରନ୍ତୁ crun_make_error (err, 0, "the container `%s` is not in `created` or `stopped` state", id);

          /* If the container is in "created" state, then do the equivalent of delete --force.  */
          killall = force = true;
        }
    }

  ଯଦି (def == ଖାଲି)
    {
      ଯଦି (container == ଖାଲି)
        {
          ret = read_container_config_from_state (&container_cleanup, state_ମୂଳ, id, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;

          container = container_cleanup;
        }

      def = container->container_def;
    }

  ଯଦି (killall && force)
    {
      /* If the container has a pid namespace, it is enough to kill the first
         process (pid=1 in the namespace).
      */
      ଯଦି (has_new_pid_namespace (def))
        {
          ret = libcrun_kill_linux (&status, SIGKILL, err);
          ଯଦି (UNLIKELY (ret < 0))
            {
              ଯଦି (crun_error_get_errno (err) != ESRCH)
                ଫେରନ୍ତୁ ret;

              crun_error_release (err);
            }
        }
      ଅନ୍ୟଥା ଯଦି (status.cgroup_path)
        {
          ret = libcrun_cgroup_killall (cgroup_status, SIGKILL, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }
    }

  ଯଦି (def->linux && def->linux->intel_rdt)
    {
      ret = libcrun_destroy_intelrdt (id, def, err);
      ଯଦି (UNLIKELY (ret < 0))
        libcrun_error_report_and_release (err);
    }

  ଯଦି (status.cgroup_path)
    {
      /* If the cgroup cannot be destroyed, there are still processes in it.
         Keep the container status, it is the only record of the cgroup, so
         that the delete can be retried.  */
      ret = libcrun_cgroup_destroy (cgroup_status, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ret = run_poststop_hooks (context, container, def, &status, state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    libcrun_error_report_and_release (err);

  ଫେରନ୍ତୁ libcrun_container_delete_status (state_ମୂଳ, id, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_delete (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, bool force, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ container_delete_internal (context, ଖାଲି, ଖାଲି, id, force, true, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_kill (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, ଅଚଳ ଅକ୍ଷର *ସଙ୍କେତ, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା sig, ret;
  ଅଚଳ ଅକ୍ଷର *state_ମୂଳ = context->state_ମୂଳ;
  cleanup_container_status libcrun_container_status_t status = {};

  sig = str2sig (ସଙ୍କେତ);
  ଯଦି (UNLIKELY (sig < 0))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "unknown signal `%s`", ସଙ୍କେତ);

  ret = libcrun_read_container_status (&status, state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ libcrun_kill_linux (&status, sig, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_killall (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, ଅଚଳ ଅକ୍ଷର *ସଙ୍କେତ, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା sig, ret;
  ଅଚଳ ଅକ୍ଷର *state_ମୂଳ = context->state_ମୂଳ;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_cgroup_status ସଂରଚନା libcrun_cgroup_status *cgroup_status = ଖାଲି;

  sig = str2sig (ସଙ୍କେତ);
  ଯଦି (UNLIKELY (sig < 0))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "unknown signal `%s`", ସଙ୍କେତ);

  ret = libcrun_read_container_status (&status, state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  cgroup_status = libcrun_cgroup_make_status (&status);

  ret = libcrun_cgroup_killall (cgroup_status, sig, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_container_status (libcrun_container_t *container, libcrun_context_t *context,
                        pid_t pid, ସଂରଚନା libcrun_cgroup_status *cgroup_status,
                        libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *bundle_allocated = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *bundle = get_bundle_path (context->bundle, &bundle_allocated);
  cleanup_free ଅକ୍ଷର *owner = get_user_name (geteuid ());
  ଅକ୍ଷର *external_descriptors = libcrun_get_external_descriptors (container);
  ଅକ୍ଷର *rootfs = container->container_def->ମୂଳ ? container->container_def->ମୂଳ->path : "";
  ଅକ୍ଷର created[35];

  libcrun_container_status_t status = {
    .pid = pid,
    .rootfs = rootfs,
    .bundle = (ଅକ୍ଷର *) bundle,
    .created = created,
    .owner = owner,
    .systemd_cgroup = context->systemd_cgroup,
    .detached = context->detach,
    .no_new_keyring = context->no_new_keyring,
    .external_descriptors = external_descriptors,
    .cgroup_path = ଖାଲି,
    .scope = ଖାଲି,
  };

  get_current_timestamp (created, ଆକାର (created));

  ଯଦି (cgroup_status)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

      ret = libcrun_cgroup_get_status (cgroup_status, &status, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (external_descriptors == ଖାଲି)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid internal state.  No external descriptors found");
  ଫେରନ୍ତୁ libcrun_write_container_status (context->state_ମୂଳ, context->id, &status, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
reap_subprocesses (pid_t *main_process, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *main_process_exit, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *last_process, libcrun_error_t *err)
{
  *last_process = 0;
  ଯେତେବେଳେ (1)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା status;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = waitpid_ignore_stopped (-1, &status, WNOHANG);
      ଯଦି (r < 0)
        {
          ଯଦି (errno == EINTR)
            ଜାରି;
          ଯଦି (errno == ECHILD)
            {
              *last_process = 1;
              ଫେରନ୍ତୁ 0;
            }
          ଫେରନ୍ତୁ crun_make_error (err, errno, "waitpid");
        }
      ଯଦି (r == 0)
        ଭାଙ୍ଗ;
      ଯଦି (r != *main_process)
        ଜାରି;

      *main_process_exit = get_process_exit_status (status);

      /* Once reaped, the pid can be reused by an unrelated process.  */
      *main_process = 0;
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
send_sd_notify (ଅଚଳ ଅକ୍ଷର *ready_str, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = sd_notify (0, ready_str);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, -ret, "sd_notify");

#  ଯଦି HAVE_SD_NOTIFY_BARRIER
  /* Hard-code a 30 seconds timeout.  Ignore errors.  */
  sd_notify_barrier (0, 30 * 1000000);
#  endif
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) ready_str;
  (ଶୂନ୍ୟ) err;
#endif
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
handle_notify_socket (ପୂର୍ଣ୍ଣସଂଖ୍ୟା notify_socketfd, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ଅକ୍ଷର buf[256];
  ଅଚଳ ଅକ୍ଷର *ready_str = "READY=1";

  ret = recvfrom (notify_socketfd, buf, ଆକାର (buf) - 1, 0, ଖାଲି, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "recvfrom notify socket");

  buf[ret] = '\0';
  ଯଦି (strstr (buf, ready_str))
    {
      ret = send_sd_notify (ready_str, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ଫେରନ୍ତୁ 1;
    }
  ଫେରନ୍ତୁ 0;
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) notify_socketfd;
  (ଶୂନ୍ୟ) err;
  ଫେରନ୍ତୁ 1;
#endif
}

ସଂରଚନା wait_for_process_args
{
  pid_t pid;
  libcrun_context_t *context;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା terminal_fd;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା notify_socket;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *container_ready_fd;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା seccomp_notify_fd;
  ଅଚଳ ଅକ୍ଷର *seccomp_notify_plugins;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_pid_file (ଅଚଳ ଅକ୍ଷର *pid_file, pid_t pid, libcrun_error_t *err)
{
  ଅକ୍ଷର buf[32];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା buf_len = snprintf (buf, ଆକାର (buf), "%d", pid);
  ଯଦି (UNLIKELY (buf_len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (buf)))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");
  ଫେରନ୍ତୁ write_file_at_with_flags (AT_FDCWD, O_CREAT | O_TRUNC, 0700, pid_file, buf, buf_len, err);
}

#define MAX_EVENTS 10

ସ୍ଥିର bool
is_fd_pollable (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd)
{
  ସଂରଚନା epoll_event ev = { .events = EPOLLIN };
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା epollfd = epoll_create1 (EPOLL_CLOEXEC);

  ଯଦି (epollfd < 0)
    ଫେରନ୍ତୁ true;

  ଫେରନ୍ତୁ epoll_ctl (epollfd, EPOLL_CTL_ADD, fd, &ev) == 0 || errno != EPERM;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
wait_for_process (ସଂରଚନା wait_for_process_args *args, libcrun_error_t *err)
{
  cleanup_channel_fd_pair ସଂରଚନା channel_fd_pair *from_terminal = ଖାଲି;
  cleanup_channel_fd_pair ସଂରଚନା channel_fd_pair *to_terminal = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret, container_exit_code = 0, last_process;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା terminal_fd_from = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା terminal_fd_to = -1;
  bool stdin_pollable = false, stdout_pollable = false;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା epollfd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା signalfd = -1;
  sigset_t mask;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା in_fds[MAX_EVENTS];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା in_fds_len = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_fds[MAX_EVENTS];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_fds_len = 0;
  size_t i;

  cleanup_seccomp_notify_context ସଂରଚନା seccomp_notify_context_s *seccomp_notify_ctx = ଖାଲି;

  container_exit_code = 0;

  ଯଦି (args == ଖାଲି || args->context == ଖାଲି)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: context is empty");

  ପାଇଁ (i = 0; i < MAX_EVENTS; i++)
    {
      in_fds[i] = -1;
      out_fds[i] = -1;
    }

  ଯଦି (args->context->pid_file)
    {
      ret = write_pid_file (args->context->pid_file, args->pid, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  /* Also exit if there is nothing more to wait for.  */
  ଯଦି (args->context->detach && args->notify_socket < 0)
    ଫେରନ୍ତୁ 0;

  ଯଦି (args->container_ready_fd)
    {
      ret = 0;
      TEMP_FAILURE_RETRY (ଲିଖ (*args->container_ready_fd, &ret, ଆକାର (ret)));
      close_and_reset (args->container_ready_fd);
    }

  sigfillset (&mask);
  ret = sigprocmask (SIG_BLOCK, &mask, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "sigprocmask");

  signalfd = create_signalfd (&mask, err);
  ଯଦି (UNLIKELY (signalfd < 0))
    ଫେରନ୍ତୁ signalfd;

  ret = reap_subprocesses (&args->pid, &container_exit_code, &last_process, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (last_process)
    ଫେରନ୍ତୁ container_exit_code;

  ଯଦି (args->seccomp_notify_fd >= 0)
    {
      cleanup_free ଅକ୍ଷର *state_ମୂଳ = ଖାଲି;
      cleanup_free ଅକ୍ଷର *oci_config_path = ଖାଲି;

      ସଂରଚନା libcrun_load_seccomp_notify_conf_s conf;
      memset (&conf, 0, ଆକାର conf);

      ret = libcrun_get_state_directory (&state_ମୂଳ, args->context->state_ମୂଳ, args->context->id, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = append_paths (&oci_config_path, err, state_ମୂଳ, "config.json", ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      conf.runtime_ମୂଳ_path = state_ମୂଳ;
      conf.name = args->context->id;
      conf.bundle_path = args->context->bundle;
      conf.oci_config_path = oci_config_path;

      ret = set_blocking_fd (args->seccomp_notify_fd, false, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = libcrun_load_seccomp_notify_plugins (&seccomp_notify_ctx,
                                                 args->seccomp_notify_plugins,
                                                 &conf, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      in_fds[in_fds_len++] = args->seccomp_notify_fd;
    }

  ଯଦି (args->terminal_fd >= 0)
    {
      /* The terminal_fd is dup()ed so that it can be registered with
         epoll multiple times using different masks.  */
      terminal_fd_from = dup (args->terminal_fd);
      ଯଦି (UNLIKELY (terminal_fd_from < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "dup terminal fd");
      terminal_fd_to = dup (args->terminal_fd);
      ଯଦି (UNLIKELY (terminal_fd_to < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "dup terminal fd");

      ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, non_blocking_fds[] = { terminal_fd_from, terminal_fd_to, 0, 1, -1 };
      ପାଇଁ (i = 0; non_blocking_fds[i] >= 0; i++)
        {
          ret = set_blocking_fd (non_blocking_fds[i], false, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }

      /* stdin and stdout can be something epoll does not support, such as
         /dev/null or a regular file.  Such a stdin is not forwarded, and
         such a stdout is written to without waiting for it to be ready.  */
      stdin_pollable = is_fd_pollable (0);
      stdout_pollable = is_fd_pollable (1);

      from_terminal = channel_fd_pair_new (terminal_fd_from, 1, BUFSIZ);
      ଯଦି (stdin_pollable)
        to_terminal = channel_fd_pair_new (0, terminal_fd_to, BUFSIZ);
    }

  in_fds[in_fds_len++] = signalfd;
  ଯଦି (args->notify_socket >= 0)
    in_fds[in_fds_len++] = args->notify_socket;
  ଯଦି (args->terminal_fd >= 0)
    {
      ଯଦି (stdin_pollable)
        {
          in_fds[in_fds_len++] = 0;
          out_fds[out_fds_len++] = terminal_fd_to;
        }

      in_fds[in_fds_len++] = terminal_fd_from;
      ଯଦି (stdout_pollable)
        out_fds[out_fds_len++] = 1;
    }

  epollfd = epoll_helper (in_fds, ଖାଲି, out_fds, ଖାଲି, err);
  ଯଦି (UNLIKELY (epollfd < 0))
    ଫେରନ୍ତୁ epollfd;

  ଯେତେବେଳେ (1)
    {
      ସଂରଚନା epoll_event events[MAX_EVENTS];
      ସଂରଚନା signalfd_siginfo si;
      ସଂରଚନା winsize ws;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, nr_events;
      ssize_t res;

      nr_events = TEMP_FAILURE_RETRY (epoll_wait (epollfd, events, MAX_EVENTS, -1));
      ଯଦି (UNLIKELY (nr_events < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "epoll_wait");

      ପାଇଁ (i = 0; i < nr_events; i++)
        {
          ଯଦି (events[i].data.fd == 0 || events[i].data.fd == terminal_fd_to)
            {
              ret = channel_fd_pair_process (to_terminal, epollfd, err);
              ଯଦି (UNLIKELY (ret < 0))
                ଫେରନ୍ତୁ crun_error_wrap (err, "copy to terminal fd");
            }
          ଅନ୍ୟଥା ଯଦି (events[i].data.fd == 1 || events[i].data.fd == terminal_fd_from)
            {
              ret = channel_fd_pair_process (from_terminal, epollfd, err);
              ଯଦି (UNLIKELY (ret < 0))
                ଫେରନ୍ତୁ crun_error_wrap (err, "copy from terminal fd");
            }
          ଅନ୍ୟଥା ଯଦି (events[i].data.fd == args->seccomp_notify_fd)
            {
              ret = libcrun_seccomp_notify_plugins (seccomp_notify_ctx,
                                                    args->seccomp_notify_fd, err);
              ଯଦି (UNLIKELY (ret < 0))
                ଫେରନ୍ତୁ ret;
            }
          ଅନ୍ୟଥା ଯଦି (events[i].data.fd == args->notify_socket)
            {
              ret = handle_notify_socket (args->notify_socket, err);
              ଯଦି (UNLIKELY (ret < 0))
                ଫେରନ୍ତୁ ret;
              ଯଦି (ret && args->context->detach)
                ଫେରନ୍ତୁ 0;
            }
          ଅନ୍ୟଥା ଯଦି (events[i].data.fd == signalfd)
            {
              res = TEMP_FAILURE_RETRY (ପଠ (signalfd, &si, ଆକାର (si)));
              ଯଦି (UNLIKELY (res < 0))
                ଫେରନ୍ତୁ crun_make_error (err, errno, "read from signalfd");
              ଯଦି (si.ssi_signo == SIGCHLD)
                {
                  ret = reap_subprocesses (&args->pid, &container_exit_code,
                                           &last_process, err);
                  ଯଦି (UNLIKELY (ret < 0))
                    ଫେରନ୍ତୁ ret;
                  ଯଦି (last_process)
                    ଫେରନ୍ତୁ container_exit_code;
                }
              ଅନ୍ୟଥା ଯଦି (si.ssi_signo == SIGWINCH)
                {
                  /* Ignore the signal if the terminal is not available.  */
                  ଯଦି (args->terminal_fd > 0)
                    {
                      ret = ioctl (0, TIOCGWINSZ, &ws);
                      ଯଦି (UNLIKELY (ret < 0))
                        ଫେରନ୍ତୁ crun_make_error (err, errno, "ioctl TIOCGWINSZ copy terminal size from stdin");

                      ret = ioctl (args->terminal_fd, TIOCSWINSZ, &ws);
                      ଯଦି (UNLIKELY (ret < 0))
                        ଫେରନ୍ତୁ crun_make_error (err, errno, "ioctl TIOCSWINSZ copy terminal size to pty");
                    }
                }
              ଅନ୍ୟଥା ଯଦି (args->pid > 0)
                {
                  /* Send any other signal to the child process, if it was not reaped yet.  */
                  ret = kill (args->pid, si.ssi_signo);
                }
            }
          ଅନ୍ୟଥା
            {
              ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: unknown fd from epoll_wait");
            }
        }
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ
flush_fd_to_err (libcrun_context_t *context, ପୂର୍ଣ୍ଣସଂଖ୍ୟା terminal_fd)
{
  ଅକ୍ଷର buf[256];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags;

  ଯଦି (terminal_fd < 0 || stderr == ଖାଲି)
    ଫେରନ୍ତୁ;

  flags = fcntl (terminal_fd, F_GETFL, 0);
  ଯଦି (flags == -1)
    ଫେରନ୍ତୁ;
  ଯଦି (fcntl (terminal_fd, F_SETFL, flags | O_NONBLOCK) < 0)
    ଫେରନ୍ତୁ;

  ପାଇଁ (;;)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = TEMP_FAILURE_RETRY (ପଠ (terminal_fd, buf, ଆକାର (buf) - 1));
      ଯଦି (ret <= 0)
        ଭାଙ୍ଗ;
      buf[ret] = '\0';
      ଯଦି (context->output_handler)
        context->output_handler (0, buf, LIBCRUN_VERBOSITY_ERROR, context->output_handler_arg);
    }
  (ଶୂନ୍ୟ) fcntl (terminal_fd, F_SETFL, flags);
  fflush (stderr);
  fsync (1);
  fsync (2);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
cleanup_watch (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
               ସଂରଚନା libcrun_cgroup_status *cgroup_status, pid_t init_pid, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket,
               ପୂର୍ଣ୍ଣସଂଖ୍ୟା terminal_fd, libcrun_error_t *err)
{
  ଅଚଳ ଅକ୍ଷର *oom_message = ଖାଲି;
  libcrun_error_t tmp_err = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (init_pid)
    {
      /* Try to detect whether the cgroup has a OOM.  */
      ଯଦି (cgroup_status)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_oom;

          has_oom = libcrun_cgroup_has_oom (cgroup_status, &tmp_err);
          ଯଦି (has_oom > 0)
            oom_message = "OOM: the memory limit could be too low";
          ଅନ୍ୟଥା ଯଦି (has_oom < 0)
            {
              /* If the detection has failed for any reason, e.g. the cgroup was
                 already deleted by the time it was checked, just ignore the
                 failure.  */
              crun_error_release (&tmp_err);
            }
        }
      /* If the OOM wasn't detected, look into the static configuration.  */
      ଯଦି (oom_message == ଖାଲି && is_memory_limit_too_ତଳ (def))
        oom_message = "the memory limit could be too low";

      kill (init_pid, SIGKILL);
      waitpid_ignore_stopped (init_pid, ଖାଲି, 0);
    }

  ret = sync_socket_wait_sync (context, sync_socket, true, &tmp_err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      crun_error_release (err);
      *err = tmp_err;
    }

  ଯଦି (terminal_fd >= 0)
    flush_fd_to_err (context, terminal_fd);

  ଯଦି (oom_message)
    ଫେରନ୍ତୁ crun_error_wrap (err, "%s", oom_message);

  ଫେରନ୍ତୁ -1;
}

/* Find the uid:gid that is mapped to root inside the container user namespace.  */
ଶୂନ୍ୟ
get_ମୂଳ_in_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid, uid_t *uid,
                        gid_t *gid)
{
  *uid = -1;
  *gid = -1;

  /* Not root in the namespace.  */
  ଯଦି (host_uid)
    ଫେରନ୍ତୁ;

  ଯଦି (def->linux && def->linux->uid_mappings)
    {
      size_t i;

      ପାଇଁ (i = 0; i < def->linux->uid_mappings_len; i++)
        ଯଦି (def->linux->uid_mappings[i]->container_id == 0)
          {
            *uid = def->linux->uid_mappings[i]->host_id;
            ଭାଙ୍ଗ;
          }
    }
  ଯଦି (def->linux && def->linux->gid_mappings)
    {
      size_t i;

      ପାଇଁ (i = 0; i < def->linux->gid_mappings_len; i++)
        ଯଦି (def->linux->gid_mappings[i]->container_id == 0)
          {
            *gid = def->linux->gid_mappings[i]->host_id;
            ଭାଙ୍ଗ;
          }
    }

  /* If the uid and the gid are not changed, do not attempt any chown.  */
  ଯଦି (*uid == host_uid && *gid == host_gid)
    *uid = *gid = -1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_seccomp_receiver_fd (libcrun_container_t *container, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *self_receiver_fd, ଅଚଳ ଅକ୍ଷର **plugins,
                         libcrun_error_t *err)
{
  ଅଚଳ ଅକ୍ଷର *tmp;
  runtime_spec_schema_config_schema *def = container->container_def;

  *fd = -1;
  *self_receiver_fd = -1;

  tmp = find_annotation (container, "run.oci.seccomp.plugins");
  ଯଦି (tmp)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା fds[2];
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

      ret = create_socket_pair (fds, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_error_wrap (err, "create socket pair");

      *fd = fds[0];
      *self_receiver_fd = fds[1];
      *plugins = tmp;
      ଫେରନ୍ତୁ 0;
    }

  ଯଦି (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    tmp = def->linux->seccomp->listener_path;
  ଅନ୍ୟଥା
    tmp = find_annotation (container, "run.oci.seccomp.receiver");
  ଯଦି (tmp == ଖାଲି)
    tmp = getenv ("RUN_OCI_SECCOMP_RECEIVER");
  ଯଦି (tmp)
    {
      ଯଦି (tmp[0] != '/')
        ଫେରନ୍ତୁ crun_make_error (err, 0, "the seccomp receiver `%s` is not an absolute path", tmp);

      *fd = open_unix_domain_client_socket (tmp, 0, err);
      ଯଦି (UNLIKELY (*fd < 0))
        ଫେରନ୍ତୁ crun_error_wrap (err, "open seccomp receiver");
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର bool
has_seccomp_receiver (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;

  ଯଦି (def && def->linux && def->linux->seccomp && def->linux->seccomp->listener_path)
    ଫେରନ୍ତୁ true;

  ଯଦି (find_annotation (container, "run.oci.seccomp.receiver") != ଖାଲି || getenv ("RUN_OCI_SECCOMP_RECEIVER") != ଖାଲି)
    ଫେରନ୍ତୁ true;

  ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
setup_container_hooks_output (libcrun_container_t *container, runtime_spec_schema_config_schema *def,
                              ସଂରଚନା container_entrypoint_s *container_args, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *hooks_out_fd,
                              ପୂର୍ଣ୍ଣସଂଖ୍ୟା *hooks_err_fd, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (def->hooks
      && (def->hooks->prestart_len || def->hooks->poststart_len || def->hooks->create_runtime_len
          || def->hooks->create_container_len || def->hooks->start_container_len))
    {
      ret = open_hooks_output (container, hooks_out_fd, hooks_err_fd, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
      container_args->hooks_out_fd = *hooks_out_fd;
      container_args->hooks_err_fd = *hooks_err_fd;
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
setup_container_keyring (libcrun_container_t *container, libcrun_context_t *context, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (! context->no_new_keyring)
    {
      ଅଚଳ ଅକ୍ଷର *label = ଖାଲି;

      libcrun_debug ("Creating new keyring");

      ଯଦି (def->process)
        {
          label = def->process->selinux_label;
          ଯଦି (label)
            libcrun_debug ("Using SELinux process label: `%s`", label);
        }

      ret = libcrun_create_keyring (container, container->context->id, label, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
setup_terminal_socket_pair (libcrun_container_t *container, libcrun_context_t *context,
                            ସଂରଚନା container_entrypoint_s *container_args, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *socket_pair_0,
                            ପୂର୍ଣ୍ଣସଂଖ୍ୟା *socket_pair_1, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା detach = context->detach;

  ଯଦି (def->process && def->process->terminal && ! detach && context->console_socket == ଖାଲି)
    {
      libcrun_debug ("Creating terminal socket pair");
      container_args->has_terminal_socket_pair = 1;
      ret = create_socket_pair (container_args->terminal_socketpair, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_error_wrap (err, "create terminal socket");

      *socket_pair_0 = container_args->terminal_socketpair[0];
      *socket_pair_1 = container_args->terminal_socketpair[1];
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
setup_seccomp (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *seccomp_bpf_data,
               ସଂରଚନା libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *seccomp_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (find_annotation (container, "run.oci.seccomp.plugins") != ଖାଲି && has_seccomp_receiver (container))
    {
      ଫେରନ୍ତୁ crun_make_error (err, 0, "seccomp plugins and seccomp receivers cannot be declared at the same time");
    }

  ଯଦି (def->linux && (def->linux->seccomp || seccomp_bpf_data))
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା seccomp_gen_options = 0;
      ଅଚଳ ଅକ୍ଷର *annotation;

      libcrun_debug ("Initializing seccomp");
      annotation = find_annotation (container, "run.oci.seccomp_fail_unknown_syscall");
      ଯଦି (annotation && strcmp (annotation, "0") != 0)
        seccomp_gen_options = LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL;

      ଯଦି (seccomp_bpf_data)
        seccomp_gen_options |= LIBCRUN_SECCOMP_SKIP_CACHE;

      libcrun_seccomp_gen_ctx_init (seccomp_gen_ctx, container, true, seccomp_gen_options);

      ret = libcrun_open_seccomp_bpf (seccomp_gen_ctx, seccomp_fd, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
setup_console_socket (libcrun_context_t *context, runtime_spec_schema_config_schema *def,
                      ସଂରଚନା container_entrypoint_s *container_args, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *console_socket_fd,
                      libcrun_error_t *err)
{
  ଯଦି (context->console_socket && def->process && def->process->terminal)
    {
      *console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
      ଯଦି (UNLIKELY (*console_socket_fd < 0))
        ଫେରନ୍ତୁ crun_error_wrap (err, "open console socket");
      container_args->console_socket_fd = *console_socket_fd;
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
setup_cgroup_manager (libcrun_context_t *context, libcrun_container_t *container,
                      ସଂରଚନା libcrun_cgroup_args *cg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *cgroup_dirfd,
                      ସଂରଚନା libcrun_dirfd_s *cgroup_dirfd_s, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_manager;
  uid_t ମୂଳ_uid = -1;
  gid_t ମୂଳ_gid = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  ଯଦି (context->systemd_cgroup)
    {
      libcrun_debug ("Using systemd cgroup manager");
      cgroup_manager = CGROUP_MANAGER_SYSTEMD;
    }
  ଅନ୍ୟଥା ଯଦି (context->force_no_cgroup)
    {
      libcrun_debug ("Disabling cgroup manager");
      cgroup_manager = CGROUP_MANAGER_DISABLED;
    }
  ଅନ୍ୟଥା
    libcrun_debug ("Using cgroupfs cgroup manager");

  /* If we are root (either on the host or in a namespace), then chown the cgroup to root
     in the container user namespace.  */
  get_ମୂଳ_in_the_userns (def, container->host_uid, container->host_gid, &ମୂଳ_uid, &ମୂଳ_gid);
  libcrun_debug ("Using container host UID `%d` and GID `%d`", container->host_uid, container->host_gid);

  memset (cg, 0, ଆକାର (*cg));

  cg->cgroup_path = def->linux ? def->linux->cgroups_path : "";
  cg->manager = cgroup_manager;
  cg->id = context->id;
  cg->resources = def->linux ? def->linux->resources : ଖାଲି;
  cg->annotations = container->annotations;
  cg->ମୂଳ_uid = ମୂଳ_uid;
  cg->ମୂଳ_gid = ମୂଳ_gid;
  cg->state_ମୂଳ = context->state_ମୂଳ;
  cg->container = container;

  ret = libcrun_cgroup_preenter (cg, cgroup_dirfd, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  cgroup_dirfd_s->dirfd = cgroup_dirfd;
  cgroup_dirfd_s->joined = false;

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
set_scheduler (pid_t pid, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = libcrun_set_scheduler (pid, def->process, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_reset_cpu_affinity_mask (pid, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_set_io_priority (pid, def->process, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
seccomp_generation (ପୂର୍ଣ୍ଣସଂଖ୍ୟା seccomp_fd, ଅଚଳ ଅକ୍ଷର *seccomp_bpf_data,
                    ସଂରଚନା libcrun_seccomp_gen_ctx_s *seccomp_gen_ctx, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (seccomp_fd >= 0)
    {
      ଯଦି (seccomp_bpf_data != ଖାଲି)
        {
          ret = libcrun_copy_seccomp (seccomp_gen_ctx, seccomp_bpf_data, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }
      ଅନ୍ୟଥା
        {
          ret = libcrun_generate_seccomp (seccomp_gen_ctx, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
terminal_setup (runtime_spec_schema_config_schema *def, libcrun_context_t *context,
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା socket_pair_0, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *terminal_fd, ଶୂନ୍ୟ **orig_terminal, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା detach = context->detach;

  ଯଦି (def->process && def->process->terminal && ! detach && context->console_socket == ଖାଲି)
    {
      libcrun_debug ("Receiving console socket fd");
      *terminal_fd = receive_fd_from_socket (socket_pair_0, err);
      ଯଦି (UNLIKELY (*terminal_fd < 0))
        ଫେରନ୍ତୁ -1;

      ret = libcrun_clear_onlcr (*terminal_fd, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = libcrun_set_raw (0, orig_terminal, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_run_internal (libcrun_container_t *container, libcrun_context_t *context,
                                ପୂର୍ଣ୍ଣସଂଖ୍ୟା *container_ready_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  pid_t pid;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା detach = context->detach;
  cleanup_cgroup_status ସଂରଚନା libcrun_cgroup_status *cgroup_status = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା terminal_fd = -1;
  cleanup_terminal ଶୂନ୍ୟ *orig_terminal = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା notify_socket = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା socket_pair_0 = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା socket_pair_1 = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା seccomp_fd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା console_socket_fd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା hooks_out_fd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା hooks_err_fd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା own_seccomp_receiver_fd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା seccomp_notify_fd = -1;
  ଅଚଳ ଅକ୍ଷର *seccomp_notify_plugins = ଖାଲି;
  cleanup_cgroup_preenter ସଂରଚନା libcrun_cgroup_args cg = {};
  cleanup_free ଅକ୍ଷର *bundle_allocated = ଖାଲି;
  ସଂରଚନା container_entrypoint_s container_args = {
    .container = container,
    .context = context,
    .bundle = get_bundle_path (context->bundle, &bundle_allocated),
    .terminal_socketpair = { -1, -1 },
    .console_socket_fd = -1,
    .hooks_out_fd = -1,
    .hooks_err_fd = -1,
    .seccomp_receiver_fd = -1,
    .custom_handler = ଖାଲି,
  };
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_dirfd = -1;
  ସଂରଚନା libcrun_dirfd_s cgroup_dirfd_s;
  ସଂରଚନା libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  ଅଚଳ ଅକ୍ଷର *seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode;

  ଯଦି (! context->force_no_cgroup)
    {
      cgroup_mode = libcrun_get_cgroup_mode (err);
      ଯଦି (UNLIKELY (cgroup_mode < 0))
        ଫେରନ୍ତୁ cgroup_mode;

      ଯଦି (cgroup_mode != CGROUP_MODE_UNIFIED)
        libcrun_warning ("cgroup v1 is deprecated and will be removed in a future release.  Use cgroup v2");
    }

  ret = setup_container_hooks_output (container, def, &container_args, &hooks_out_fd, &hooks_err_fd, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  container->context = context;

  ଯଦି (! detach || context->notify_socket)
    {
      libcrun_debug ("Setting child subreaper");
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "prctl set child subreaper");
    }

  ret = setup_container_keyring (container, context, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = setup_terminal_socket_pair (container, context, &container_args, &socket_pair_0, &socket_pair_1, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = block_signals (err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  umask (0);

  ret = libcrun_set_mempolicy (def, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;
  container_args.seccomp_fd = seccomp_fd;

  ଯଦି (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &container_args.seccomp_receiver_fd, &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ret = setup_console_socket (context, def, &container_args, &console_socket_fd, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = setup_cgroup_manager (context, container, &cg, &cgroup_dirfd, &cgroup_dirfd_s, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_configure_handler (container_args.context->handler_manager,
                                   container_args.context,
                                   container,
                                   &(container_args.custom_handler),
                                   err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (container_args.custom_handler && container_args.custom_handler->vtable->modify_oci_configuration)
    {
      libcrun_debug ("Using custom handler to modify OCI configuration");
      ret = container_args.custom_handler->vtable->modify_oci_configuration (container_args.custom_handler->cookie,
                                                                             container_args.context,
                                                                             container->container_def,
                                                                             err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  pid = libcrun_run_linux_container (container, container_init, &container_args, &sync_socket, &cgroup_dirfd_s, err);
  ଯଦି (UNLIKELY (pid < 0))
    ଫେରନ୍ତୁ pid;

  cg.pid = pid;
  cg.joined = cgroup_dirfd_s.joined;
  libcrun_debug ("Running container on PID: `%d`", pid);

  ଯଦି (context->fifo_exec_wait_fd < 0 && context->notify_socket)
    {
      libcrun_debug ("Using notify socket: `%s`", context->notify_socket);
      /* Do not open the notify socket here on "create".  "start" will take care of it.  */
      ret = get_notify_fd (context, container, &notify_socket, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଯାଅ fail;
    }

  ଯଦି (container_args.terminal_socketpair[1] >= 0)
    close_and_reset (&socket_pair_1);

  ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ fail;

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ fail;

  ret = libcrun_move_network_devices (container, pid, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ fail;

  /* sync send own pid.  */
  ret = TEMP_FAILURE_RETRY (ଲିଖ (sync_socket, &pid, ଆକାର (pid)));
  ଯଦି (UNLIKELY (ret != ଆକାର (pid)))
    {
      ଯଦି (ret >= 0)
        errno = 0;
      crun_make_error (err, errno, "write to sync socket");
      ଯାଅ fail;
    }

  /* sync 1.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ fail;

  /* sync 2.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ fail;

  ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ fail;

  /* If the container process is created later, the scheduler is set on it then.  */
  ଯଦି (! libcrun_container_delayed_pidns_fork (container))
    {
      ret = set_scheduler (pid, def, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଯାଅ fail;
    }

  /* The container is waiting that we write back.  In this phase we can launch the
     prestart hooks.  */
  ଯଦି (def->hooks && def->hooks->prestart_len)
    {
      libcrun_debug ("Running `prestart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "prestart", (hook **) def->hooks->prestart,
                      def->hooks->prestart_len, hooks_out_fd, hooks_err_fd, false, err);
      ଯଦି (UNLIKELY (ret != 0))
        ଯାଅ fail;
    }
  ଯଦି (def->hooks && def->hooks->create_runtime_len)
    {
      libcrun_debug ("Running `create` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "created", "createRuntime", (hook **) def->hooks->create_runtime,
                      def->hooks->create_runtime_len, hooks_out_fd, hooks_err_fd, false, err);
      ଯଦି (UNLIKELY (ret != 0))
        ଯାଅ fail;
    }

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ fail;
  close_and_reset (&seccomp_fd);

  /* sync 3.  */
  ret = sync_socket_send_sync (sync_socket, true, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ fail;

  /* The process joining an existing PID namespace is created once the container root is in place.  */
  ଯଦି (libcrun_container_delayed_pidns_fork (container))
    {
      pid_t new_pid = 0;

      ret = sync_socket_wait_sync (context, sync_socket, false, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଯାଅ fail;

      ret = TEMP_FAILURE_RETRY (ପଠ (sync_socket, &new_pid, ଆକାର (new_pid)));
      ଯଦି (UNLIKELY (ret != ଆକାର (new_pid)))
        {
          ଯଦି (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "read pid from sync socket");
          ଯାଅ fail;
        }

      /* Cleanup the first process.  */
      waitpid_ignore_stopped (pid, ଖାଲି, 0);

      pid = new_pid;

      ret = set_scheduler (pid, def, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଯାଅ fail;

      ret = libcrun_handle_pidfd_receiver (pid, container, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଯାଅ fail;

      /* sync send own pid.  */
      ret = TEMP_FAILURE_RETRY (ଲିଖ (sync_socket, &pid, ଆକାର (pid)));
      ଯଦି (UNLIKELY (ret != ଆକାର (pid)))
        {
          ଯଦି (ret >= 0)
            errno = 0;
          crun_make_error (err, errno, "write to sync socket");
          ଯାଅ fail;
        }
    }

  ret = terminal_setup (def, context, socket_pair_0, &terminal_fd, &orig_terminal, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ fail;

  close_and_reset (&socket_pair_0);

  /* sync 4.  */
  ret = sync_socket_wait_sync (context, sync_socket, false, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ fail;

  ret = close_and_reset (&sync_socket);
  ଯଦି (UNLIKELY (ret < 0))
    {
      crun_make_error (err, errno, "close sync_socket failed");
      ଯାଅ fail;
    }

  libcrun_debug ("Writing container status");
  ret = write_container_status (container, context, pid, cgroup_status, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ fail;

  /* Run poststart hooks here only if the container is created using "run".  For create+start, the
     hooks will be executed as part of the start command.  */
  ଯଦି (context->fifo_exec_wait_fd < 0 && def->hooks && def->hooks->poststart_len)
    {
      libcrun_debug ("Running `poststart` hooks");
      ret = do_hooks (def, pid, context->id, false, context->bundle, context->bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      ଯଦି (UNLIKELY (ret != 0))
        ଯାଅ fail;
    }

  /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
  ଯଦି (own_seccomp_receiver_fd >= 0)
    {
      libcrun_debug ("Receiving seccomp fd");
      seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
      ଯଦି (UNLIKELY (seccomp_notify_fd < 0))
        ଯାଅ fail;

      ret = close_and_reset (&own_seccomp_receiver_fd);
      ଯଦି (UNLIKELY (ret < 0))
        {
          crun_make_error (err, errno, "close seccomp receiver fd failed");
          ଯାଅ fail;
        }
    }

  {
    ସଂରଚନା wait_for_process_args args = {
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
  ଯଦି (! context->detach)
    {
      libcrun_error_t tmp_err = ଖାଲି;
      cleanup_watch (context, def, cgroup_status, 0, sync_socket, terminal_fd, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ଫେରନ୍ତୁ ret;

fail:
  ret = cleanup_watch (context, def, cgroup_status, pid, sync_socket, terminal_fd, err);
  ଯଦି (cgroup_status)
    {
      libcrun_error_t tmp_err = ଖାଲି;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
check_config_file (runtime_spec_schema_config_schema *def, libcrun_context_t *context, libcrun_error_t *err)
{
  ଯଦି (UNLIKELY (def->linux == ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid config file, no `linux` block specified");

  ଯଦି (context->handler == ଖାଲି)
    {
      ଯଦି (UNLIKELY (def->ମୂଳ == ଖାଲି))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid config file, no `root` block specified");
      ଯଦି (UNLIKELY (def->mounts == ଖାଲି))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid config file, no `mounts` block specified");
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_copy_config_file (ଅଚଳ ଅକ୍ଷର *id, ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, libcrun_container_t *container, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  cleanup_free ଅକ୍ଷର *dest_path = ଖାଲି;
  cleanup_free ଅକ୍ଷର *dir = ଖାଲି;
  cleanup_free ଅକ୍ଷର *buffer = ଖାଲି;
  size_t len;

  ret = libcrun_get_state_directory (&dir, state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = append_paths (&dest_path, err, dir, "config.json", ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (container->config_file == ଖାଲି && container->config_file_content == ଖାଲି)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "config file not specified");

  ଯଦି (container->config_file == ଖାଲି)
    {
      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, container->config_file_content, strlen (container->config_file_content), err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଅନ୍ୟଥା
    {
      libcrun_debug ("Reading config file: `%s`", container->config_file);
      ret = read_all_file (container->config_file, &buffer, &len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      libcrun_debug ("Writing config file to: `%s`", dest_path);
      ret = write_file (dest_path, buffer, len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ
force_delete_container_status (libcrun_context_t *context, libcrun_container_t *container,
                               runtime_spec_schema_config_schema *def)
{
  libcrun_error_t tmp_err = ଖାଲି;
  container_delete_internal (context, container, def, context->id, true, false, &tmp_err);
  libcrun_error_report_and_release (&tmp_err);
}

/* Serialize ERR to FD as an int errno followed by a NUL-terminated message.
   Used by forked children to report a failure to the parent process.  */
ସ୍ଥିର ଶୂନ୍ୟ
write_error_to_pipe (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, libcrun_error_t *err)
{
  ଅଚଳ ଅକ୍ଷର *ସନ୍ଦେଶ = (*err)->ସନ୍ଦେଶ;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା errcode = crun_error_get_errno (err);

  TEMP_FAILURE_RETRY (ଲିଖ (fd, &errcode, ଆକାର (errcode)));
  TEMP_FAILURE_RETRY (ଲିଖ (fd, ସନ୍ଦେଶ, strlen (ସନ୍ଦେଶ) + 1));
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_run (libcrun_context_t *context, libcrun_container_t *container, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା options,
                       libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା detach = context->detach;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା container_ret_status[2];
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା pipefd0 = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା pipefd1 = -1;
  libcrun_error_t tmp_err = ଖାଲି;

  container->context = context;

  ret = validate_options (options, LIBCRUN_RUN_OPTIONS_PREFORK | LIBCRUN_RUN_OPTIONS_KEEP, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = check_config_file (def, context, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (def->process && def->process->terminal && detach && context->console_socket == ଖାଲି)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "use --console-socket with --detach when a terminal is used");

  ret = libcrun_status_check_directories (context->state_ମୂଳ, context->id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (! detach && (options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      ret = libcrun_copy_config_file (context->id, context->state_ମୂଳ, container, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = libcrun_container_run_internal (container, context, ଖାଲି, err);
      ଯଦି (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
        force_delete_container_status (context, container, def);
      ଫେରନ୍ତୁ ret;
    }

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  ret = fork ();
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "fork");
  ଯଦି (ret)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା status;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, ଖାଲି, 0);

      ret = TEMP_FAILURE_RETRY (ପଠ (pipefd0, &status, ଆକାର (status)));
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "read from sync pipe");

      ଯଦି (status < 0)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା errno_;
          ଅକ୍ଷର buf[512];
          ret = TEMP_FAILURE_RETRY (ପଠ (pipefd0, &errno_, ଆକାର (errno_)));
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "read from sync pipe");

          ret = TEMP_FAILURE_RETRY (ପଠ (pipefd0, buf, ଆକାର (buf) - 1));
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "read from sync pipe");
          buf[ret] = '\0';

          ଫେରନ୍ତୁ crun_make_error (err, errno_, "%s", buf);
        }

      ଫେରନ୍ତୁ status;
    }

  close_and_reset (&pipefd0);

  /* forked process.  */
  ret = detach_process ();
  ଯଦି (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_ମୂଳ, container, &tmp_err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ fail;

  ret = libcrun_container_run_internal (container, context, ଖାଲି, &tmp_err);
  TEMP_FAILURE_RETRY (ଲିଖ (pipefd1, &ret, ଆକାର (ret)));
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ fail;

  exit (EXIT_SUCCESS);
fail:

  ଯଦି (! (options & LIBCRUN_RUN_OPTIONS_KEEP))
    force_delete_container_status (context, container, def);
  ଯଦି (tmp_err)
    {
      write_error_to_pipe (pipefd1, &tmp_err);
      crun_error_release (&tmp_err);
    }

  exit (EXIT_FAILURE);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା options,
                          libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା container_ready_pipe[2];
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା pipefd0 = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା pipefd1 = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା exec_fifo_fd = -1;
  context->detach = 1;

  libcrun_debug ("Creating container: `%s`", context->id);
  container->context = context;

  ret = validate_options (options, LIBCRUN_CREATE_OPTIONS_PREFORK, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = check_config_file (def, context, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (def->process && def->process->terminal && context->console_socket == ଖାଲି)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "use --console-socket with create when a terminal is used");

  ret = libcrun_status_check_directories (context->state_ମୂଳ, context->id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  exec_fifo_fd = libcrun_status_create_exec_fifo (context->state_ମୂଳ, context->id, err);
  ଯଦି (UNLIKELY (exec_fifo_fd < 0))
    ଫେରନ୍ତୁ exec_fifo_fd;

  context->fifo_exec_wait_fd = exec_fifo_fd;
  exec_fifo_fd = -1;

  ଯଦି ((options & LIBCRUN_RUN_OPTIONS_PREFORK) == 0)
    {
      libcrun_debug ("Running with prefork enabled");
      ret = libcrun_copy_config_file (context->id, context->state_ମୂଳ, container, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
      ret = libcrun_container_run_internal (container, context, ଖାଲି, err);
      ଯଦି (UNLIKELY (ret < 0))
        force_delete_container_status (context, container, def);
      ଫେରନ୍ତୁ ret;
    }

  ret = pipe2 (container_ready_pipe, O_CLOEXEC);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "pipe");
  pipefd0 = container_ready_pipe[0];
  pipefd1 = container_ready_pipe[1];

  ret = fork ();
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "fork");
  ଯଦି (ret)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା exit_code;
      close_and_reset (&pipefd1);

      waitpid_ignore_stopped (ret, ଖାଲି, 0);

      ret = TEMP_FAILURE_RETRY (ପଠ (pipefd0, &exit_code, ଆକାର (exit_code)));
      ଯଦି (UNLIKELY (ret <= 0))
        ଫେରନ୍ତୁ crun_make_error (err, ret < 0 ? errno : 0, "waiting for container to be ready");

      ଯଦି (ret != ଆକାର (exit_code) || exit_code < 0)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: read invalid exit code");

      ଯଦି (exit_code == 0)
        ଫେରନ୍ତୁ 0;

      libcrun_debug ("Exit code is `%d`, deleting container", exit_code);
      libcrun_error_t tmp_err = ଖାଲି;
      container_delete_internal (context, ଖାଲି, def, context->id, true, true, &tmp_err);
      libcrun_error_report_and_release (&tmp_err);
      ଫେରନ୍ତୁ crun_make_error (err, 0, "error creating container");
    }

  /* forked process.  */
  ret = detach_process ();
  ଯଦି (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "detach process");

  ret = libcrun_copy_config_file (context->id, context->state_ମୂଳ, container, err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା errcode = crun_error_get_errno (err);
      crun_error_release (err);
      libcrun_fail_with_error (errcode, "copy config file");
    }

  ret = libcrun_container_run_internal (container, context, &pipefd1, err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      force_delete_container_status (context, container, def);
      libcrun_error ((*err)->status, "%s", (*err)->ସନ୍ଦେଶ);
      crun_error_release (err);
      crun_set_output_handler (log_write_to_stderr, ଖାଲି);
    }

  ଯଦି (pipefd1 >= 0)
    TEMP_FAILURE_RETRY (ଲିଖ (pipefd1, &ret, ଆକାର (ret)));
  exit (ret ? EXIT_FAILURE : 0);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_start (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *state_ମୂଳ = context->state_ମୂଳ;
  runtime_spec_schema_config_schema *def;
  cleanup_container_status libcrun_container_status_t status = {};
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = libcrun_read_container_status (&status, state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_is_container_running (&status, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (! ret)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "container `%s` is not running", id);

  ret = read_container_config_from_state (&container, state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (context->notify_socket)
    {
      ret = get_notify_fd (context, container, &fd, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ret = libcrun_status_write_exec_fifo (context->state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  def = container->container_def;

  ଯଦି (context->notify_socket)
    {
      ଯଦି (fd >= 0)
        {
          fd_set read_set;

          ଯେତେବେଳେ (1)
            {
              ସଂରଚନା timeval timeout = {
                .tv_sec = 0,
                .tv_usec = 10000,
              };
              FD_ZERO (&read_set);
              FD_SET (fd, &read_set);

              ret = select (fd + 1, &read_set, ଖାଲି, ଖାଲି, &timeout);
              ଯଦି (UNLIKELY (ret < 0))
                ଫେରନ୍ତୁ crun_make_error (err, errno, "select failed");
              ଯଦି (ret)
                {
                  ret = handle_notify_socket (fd, err);
                  ଯଦି (UNLIKELY (ret < 0))
                    ଫେରନ୍ତୁ ret;
                  ଯଦି (ret)
                    ଭାଙ୍ଗ;
                }
              ଅନ୍ୟଥା
                {
                  ret = libcrun_is_container_running (&status, err);
                  ଯଦି (UNLIKELY (ret < 0))
                    ଫେରନ୍ତୁ ret;
                  ଯଦି (! ret)
                    ଫେରନ୍ତୁ 0;
                }
            }
        }
    }

  /* The container is considered running only after we got the notification from the
     notify_socket, if any.  */
  ଯଦି (def->hooks && def->hooks->poststart_len)
    {
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା hooks_out_fd = -1;
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା hooks_err_fd = -1;

      ret = open_hooks_output (container, &hooks_out_fd, &hooks_err_fd, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = do_hooks (def, status.pid, context->id, false, status.bundle, status.bundle, "running", "poststart", (hook **) def->hooks->poststart,
                      def->hooks->poststart_len, hooks_out_fd, hooks_err_fd, false, err);
      ଯଦି (UNLIKELY (ret != 0))
        {
          libcrun_error_t tmp_err = ଖାଲି;
          container_delete_internal (context, container, def, id, true, true, &tmp_err);
          libcrun_error_report_and_release (&tmp_err);
          ଫେରନ୍ତୁ ret;
        }
    }

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_get_container_state_string (ଅଚଳ ଅକ୍ଷର *id, libcrun_container_status_t *status, ଅଚଳ ଅକ୍ଷର *state_ମୂଳ,
                                    ଅଚଳ ଅକ୍ଷର **container_status, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *running, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret, has_fifo = 0;
  bool paused = false;

  ret = libcrun_is_container_running (status, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;
  *running = ret;

  ଯଦି (*running)
    {
      ret = libcrun_status_has_read_exec_fifo (state_ମୂଳ, id, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
      has_fifo = ret;
    }

  ଯଦି (*running && ! has_fifo)
    {
      cleanup_cgroup_status ସଂରଚନା libcrun_cgroup_status *cgroup_status = ଖାଲି;

      cgroup_status = libcrun_cgroup_make_status (status);

      ret = libcrun_cgroup_is_container_paused (cgroup_status, &paused, err);
      ଯଦି (UNLIKELY (ret < 0))
        {
          /*
            The cgroup might have been cleaned up by the time we try to read it, ignore both
            ENOENT and ENODEV:
            - ENOENT: if the open(CGROUP_PATH) fails because the cgroup was deleted.
            - ENODEV: if the cgroup is deleted between the open and reading the freeze status.
          */

          errno = crun_error_get_errno (err);
          ଯଦି (errno == ENOENT || errno == ENODEV)
            {
              crun_error_release (err);
              *container_status = "stopped";
              ଫେରନ୍ତୁ 0;
            }

          ଫେରନ୍ତୁ ret;
        }
    }

  ଯଦି (! *running)
    *container_status = "stopped";
  ଅନ୍ୟଥା ଯଦି (has_fifo)
    *container_status = "created";
  ଅନ୍ୟଥା ଯଦି (paused)
    *container_status = "paused";
  ଅନ୍ୟଥା
    *container_status = "running";

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_state_json (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, ଅକ୍ଷର **out, libcrun_error_t *err)
{
  ଅକ୍ଷର *buffer = ଖାଲି;
  size_t len = 0;
  FILE *f;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  f = open_memstream (&buffer, &len);
  ଯଦି (f == ଖାଲି)
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open_memstream");

  ret = libcrun_container_state (context, id, f, err);
  fclose (f);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ମୁକ୍ତ (buffer);
      ଫେରନ୍ତୁ ret;
    }

  *out = buffer;
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_state (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, FILE *out, libcrun_error_t *err)
{
  ଅଚଳ ଅକ୍ଷର *ଅଚଳ OCI_CONFIG_VERSION = "1.0.0";
  libcrun_container_status_t status = {};
  ଅଚଳ ଅକ୍ଷର *state_ମୂଳ = context->state_ମୂଳ;
  ଅଚଳ ଅକ୍ଷର *container_status = ଖାଲି;
  json_gen_ctx *gen = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *buf;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = json_gen_status_ok;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା running;
  size_t len;

  ret = libcrun_read_container_status (&status, state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_get_container_state_string (id, &status, state_ମୂଳ, &container_status, &running, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ exit;

  ret = 0;
  ଯଦି (! json_gen_init (&gen, ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "json_gen_init failed");

  json_gen_config (gen, json_gen_beautify, 1);

  GEN_OR_FAIL (json_gen_map_open (gen));
  GEN_ଚାବି (gen, "ociVersion");
  GEN_STR (gen, OCI_CONFIG_VERSION);

  GEN_ଚାବି (gen, "id");
  GEN_STR (gen, id);

  GEN_ଚାବି (gen, "pid");
  GEN_OR_FAIL (map_int (gen, running ? status.pid : 0));

  GEN_ଚାବି (gen, "status");
  GEN_STR (gen, container_status);

  GEN_ଚାବି (gen, "bundle");
  GEN_STR (gen, status.bundle);

  GEN_ଚାବି (gen, "rootfs");
  GEN_STR (gen, status.rootfs);

  GEN_ଚାବି (gen, "created");
  GEN_STR (gen, status.created);

  ଯଦି (status.scope)
    {
      GEN_ଚାବି (gen, "systemd-scope");
      GEN_STR (gen, status.scope);
    }

  ଯଦି (status.owner)
    {
      GEN_ଚାବି (gen, "owner");
      GEN_STR (gen, status.owner);
    }

  {
    cleanup_container libcrun_container_t *container = ଖାଲି;

    ret = read_container_config_from_state (&container, state_ମୂଳ, id, err);
    ଯଦି (UNLIKELY (ret < 0))
      ଯାଅ exit;

    ଯଦି (container->container_def->annotations && container->container_def->annotations->len)
      GEN_OR_FAIL (gen_annotations (gen, container->container_def->annotations));
  }

  GEN_OR_FAIL (json_gen_map_close (gen));

  GEN_OR_FAIL (json_gen_get_buf (gen, &buf, &len));

  fprintf (out, "%s\n", buf);

exit:
  ଯଦି (gen)
    json_gen_free (gen);
  libcrun_free_container_status (&status);
  ଫେରନ୍ତୁ ret;

gen_error:
  ret = json_gen_error_to_crun_error (r, err);
  ଯାଅ exit;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_exec (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, runtime_spec_schema_config_schema_process *process,
                        libcrun_error_t *err)
{
  ସଂରଚନା libcrun_container_exec_options_s opts;
  memset (&opts, 0, ଆକାର (opts));

  opts.struct_size = ଆକାର (opts);
  opts.process = process;

  ଫେରନ୍ତୁ libcrun_container_exec_with_options (context, id, &opts, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_exec_process_file (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err)
{
  ସଂରଚନା libcrun_container_exec_options_s opts;
  memset (&opts, 0, ଆକାର (opts));

  opts.struct_size = ଆକାର (opts);
  opts.path = path;

  ଫେରନ୍ତୁ libcrun_container_exec_with_options (context, id, &opts, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
exec_from_process_json (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, ଅଚଳ ଅକ୍ଷର *process_json, ଅଚଳ ଅକ୍ଷର *cgroup,
                        bool merge_env, libcrun_error_t *err)
{
  ସଂରଚନା parser_context parser_ctx = { .options = 0, .errfile = stderr };
  ସଂରଚନା libcrun_container_exec_options_s opts;
  runtime_spec_schema_config_schema_process *process;
  parser_error parser_err = ଖାଲି;
  json_object *doc = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = parse_json_file (&doc, process_json, &parser_ctx, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  process = make_runtime_spec_schema_config_schema_process (doc, &parser_ctx, &parser_err);
  json_object_put (doc);
  ଯଦି (process == ଖାଲି)
    {
      ret = crun_make_error (err, 0, "cannot parse process: `%s`", parser_err ? parser_err : "unknown error");
      ମୁକ୍ତ (parser_err);
      ଫେରନ୍ତୁ ret;
    }

  memset (&opts, 0, ଆକାର (opts));
  opts.struct_size = ଆକାର (opts);
  opts.process = process;
  opts.cgroup = cgroup;
  opts.merge_env = merge_env;

  ret = libcrun_container_exec_with_options (context, id, &opts, err);
  free_runtime_spec_schema_config_schema_process (process);
  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_exec_json (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, ଅଚଳ ଅକ୍ଷର *process_json, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ exec_from_process_json (context, id, process_json, ଖାଲି, false, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_exec_with_options_json (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id,
                                          ଅଚଳ ସଂରଚନା libcrun_exec_options_s *opts, libcrun_error_t *err)
{
  ଯଦି (opts == ଖାଲି || opts->struct_size == 0)
    ଫେରନ୍ତୁ crun_make_error (err, EINVAL, "invalid exec options");

  ଫେରନ୍ତୁ exec_from_process_json (context, id, opts->process_json, opts->cgroup, opts->merge_env, err);
}

#define cleanup_process_schema __attribute__ ((cleanup (cleanup_process_schemap)))

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
cleanup_process_schemap (runtime_spec_schema_config_schema_process **p)
{
  runtime_spec_schema_config_schema_process *process = *p;
  ଯଦି (process)
    (ଶୂନ୍ୟ) free_runtime_spec_schema_config_schema_process (process);
}

/* Build the seccomp receiver payload (when needed), apply the seccomp profile and
   close the seccomp file descriptors.  Used from exec_process_entrypoint both before
   and after the capabilities are set, depending on process->no_new_privileges.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
apply_seccomp_for_exec (libcrun_container_t *container, pid_t own_pid, ପୂର୍ଣ୍ଣସଂଖ୍ୟା seccomp_fd,
                        ପୂର୍ଣ୍ଣସଂଖ୍ୟା seccomp_receiver_fd, ଅକ୍ଷର **seccomp_flags, size_t seccomp_flags_len,
                        libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *seccomp_fd_payload = ଖାଲି;
  size_t seccomp_fd_payload_len = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (seccomp_receiver_fd >= 0)
    {
      ret = get_seccomp_receiver_fd_payload (container, "running", own_pid, &seccomp_fd_payload, &seccomp_fd_payload_len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ret = libcrun_apply_seccomp (seccomp_fd, seccomp_receiver_fd, seccomp_fd_payload,
                               seccomp_fd_payload_len, seccomp_flags, seccomp_flags_len, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  close_and_reset (&seccomp_fd);
  close_and_reset (&seccomp_receiver_fd);
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
exec_process_entrypoint (libcrun_context_t *context,
                         libcrun_container_t *container,
                         runtime_spec_schema_config_schema_process *process,
                         ପୂର୍ଣ୍ଣସଂଖ୍ୟା *pipefd1,
                         ପୂର୍ଣ୍ଣସଂଖ୍ୟା seccomp_fd,
                         ପୂର୍ଣ୍ଣସଂଖ୍ୟା seccomp_receiver_fd,
                         ସଂରଚନା custom_handler_instance_s *custom_handler,
                         bool merge_env,
                         libcrun_error_t *err)
{
  runtime_spec_schema_config_schema_process_capabilities *capabilities = ଖାଲି;
  cleanup_free ଅକ୍ଷର *exec_path = ଖାଲି;
  uid_t container_uid;
  gid_t container_gid;
  ଅଚଳ ଅକ୍ଷର *cwd;
  bool chdir_done = false;
  size_t seccomp_flags_len = 0;
  ଅକ୍ଷର **seccomp_flags = ଖାଲି;
  pid_t own_pid = 0;
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  container_uid = process->user ? process->user->uid : 0;
  container_gid = process->user ? process->user->gid : 0;

  TEMP_FAILURE_RETRY (ପଠ (*pipefd1, &own_pid, ଆକାର (own_pid)));

  cwd = process->cwd ? process->cwd : "/";
  ଯଦି (LIKELY (libcrun_safe_chdir (cwd, err) == 0))
    chdir_done = true;
  ଅନ୍ୟଥା
    crun_error_release (err);

  ret = unblock_signals (err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = clearenv ();
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "clearenv");

  ଯଦି ((merge_env || ! process->env_len) && container->container_def->process && container->container_def->process->env_len)
    {
      ଅକ୍ଷର *e;

      ପାଇଁ (i = 0; i < container->container_def->process->env_len; i++)
        {
          e = container->container_def->process->env[i];
          ଯଦି (putenv (e) < 0)
            ଫେରନ୍ତୁ crun_make_error (err, errno, "putenv `%s`", e);
        }
    }
  ଯଦି (process->env_len)
    {
      ପାଇଁ (i = 0; i < process->env_len; i++)
        ଯଦି (putenv (process->env[i]) < 0)
          ଫେରନ୍ତୁ crun_make_error (err, errno, "putenv `%s`", process->env[i]);
    }

  ଯଦି (is_empty_string (getenv ("HOME")))
    {
      ret = set_home_env (container_uid);
      ଯଦି (UNLIKELY (ret < 0))
        {
          libcrun_warning ("cannot detect HOME environment variable, setting default");
          ଯଦି (UNLIKELY (setenv ("HOME", "/", 1) < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "setenv HOME");
        }
    }

  ret = libcrun_set_selinux_label (container, process, false, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_set_apparmor_profile (container, process, false, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (container->container_def->linux && container->container_def->linux->seccomp)
    {
      seccomp_flags = container->container_def->linux->seccomp->flags;
      seccomp_flags_len = container->container_def->linux->seccomp->flags_len;
    }

  ret = find_executable (&exec_path, process->args[0], process->cwd, err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ଯଦି (crun_error_get_errno (err) == ENOENT)
        ଫେରନ୍ତୁ ret;

      /* If it fails for any other reason, ignore the failure.  We'll try again the lookup
         once the process switched to the use that runs in the container.  This might be necessary
         when opening a file that is on a network file system like NFS, where CAP_DAC_OVERRIDE
         is not honored.  */
      crun_error_release (err);
    }

  ଯଦି (container->container_def->linux && container->container_def->linux->personality)
    {
      ret = libcrun_set_personality (container->container_def->linux->personality, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, false, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_reopen_dev_null (err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (! process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ret = libcrun_container_setgroups (container, process, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = maybe_chown_std_streams (container_uid, container_gid, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (process->capabilities)
    capabilities = process->capabilities;
  ଅନ୍ୟଥା ଯଦି (container->container_def->process)
    capabilities = container->container_def->process->capabilities;

  ret = libcrun_set_caps (capabilities, container_uid, container_gid, process->no_new_privileges,
                          custom_handler != ଖାଲି, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (UNLIKELY (exec_path == ଖାଲି))
    {
      ret = find_executable (&exec_path, process->args[0], process->cwd, err);
      ଯଦି (UNLIKELY (ret < 0))
        {
          ଯଦି (custom_handler == ଖାଲି || is_empty_string (process->args[0]))
            ଫେରନ୍ତୁ ret;

          /* If a custom handler is used, pass argv0 as specified.  e.g. with wasm the file could miss the +x bit.  */
          crun_error_release (err);
          exec_path = xstrdup (process->args[0]);
        }
    }

  ଯଦି (! chdir_done)
    {
      ret = libcrun_safe_chdir (cwd, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (process->no_new_privileges)
    {
      ret = apply_seccomp_for_exec (container, own_pid, seccomp_fd, seccomp_receiver_fd,
                                    seccomp_flags, seccomp_flags_len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (process->user)
    umask (process->user->umask_present ? process->user->umask : 0022);

  ret = 0;
  TEMP_FAILURE_RETRY (ଲିଖ (*pipefd1, &ret, ଆକାର (ret)));
  TEMP_FAILURE_RETRY (close (*pipefd1));
  *pipefd1 = -1;

  ଯଦି (custom_handler)
    {
      ଯଦି (custom_handler->vtable->exec_func == ଖାଲି)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "the handler does not support exec");

      ଯଦି (context->argv)
        erase_initial_environment (context->argv, context->argc);

      ret = custom_handler->vtable->exec_func (custom_handler->cookie,
                                               container,
                                               exec_path,
                                               process->args);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, -ret, "exec container process failed with handler as `%s`", custom_handler->vtable->name);

      _safe_exit (EXIT_FAILURE);
    }

  /* Attempt to close all the files that are not needed to prevent execv to have access to them.
     This is a best effort operation, because the seccomp filter is already in place and it could
     stop some syscalls used by mark_or_close_fds_ge_than.
  */
  ret = mark_or_close_fds_ge_than (container, context->preserve_fds + 3, true, err);
  ଯଦି (UNLIKELY (ret < 0))
    crun_error_release (err);

  TEMP_FAILURE_RETRY (execv (exec_path, process->args));
  libcrun_fail_with_error (errno, "exec `%s`", exec_path);
}

/* Close the file descriptors inherited from the caller, other than those
   to be passed to the container process.  */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ mark_or_close_fds_ge_than (ଖାଲି, context->preserve_fds + 3, true, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_load_process_from_file (ଅଚଳ ଅକ୍ଷର *path, runtime_spec_schema_config_schema_process **process,
                                libcrun_error_t *err)
{
  ସଂରଚନା parser_context ctx = { 0, stderr };
  cleanup_free ଅକ୍ଷର *content = ଖାଲି;
  parser_error parser_err = ଖାଲି;
  json_object *doc = ଖାଲି;
  size_t len;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = read_all_file (path, &content, &len, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  *process = make_runtime_spec_schema_config_schema_process (doc, &ctx, &parser_err);
  ଯଦି (UNLIKELY (*process == ଖାଲି))
    {
      ret = crun_make_error (err, 0, "cannot parse process file: `%s`", parser_err);
      ମୁକ୍ତ (parser_err);
      ଯଦି (doc)
        json_object_put (doc);
      ଫେରନ୍ତୁ ret;
    }

  ମୁକ୍ତ (parser_err);
  ଯଦି (doc)
    json_object_put (doc);

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_exec_with_options (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id,
                                     ସଂରଚନା libcrun_container_exec_options_s *opts,
                                     libcrun_error_t *err)
{
  cleanup_custom_handler_instance ସଂରଚନା custom_handler_instance_s *custom_handler = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା container_status, ret;
  bool container_paused = false;
  pid_t pid;
  libcrun_container_status_t status = {};
  ଅଚଳ ଅକ୍ଷର *state_ମୂଳ = context->state_ମୂଳ;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା terminal_fd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା seccomp_fd = -1;
  cleanup_terminal ଶୂନ୍ୟ *orig_terminal = ଖାଲି;
  cleanup_container libcrun_container_t *container = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା container_ret_status[2];
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା pipefd0 = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା pipefd1 = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା seccomp_receiver_fd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା own_seccomp_receiver_fd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା seccomp_notify_fd = -1;
  ଅଚଳ ଅକ୍ଷର *seccomp_notify_plugins = ଖାଲି;
  __attribute__ ((unused)) cleanup_process_schema runtime_spec_schema_config_schema_process *process_cleanup = ଖାଲି;
  runtime_spec_schema_config_schema_process *process = opts->process;
  ସଂରଚନା libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret_from_child = 0;

  ret = libcrun_read_container_status (&status, state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_is_container_running (&status, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;
  container_status = ret;

  ret = read_container_config_from_state (&container, state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  container->context = context;

  ଯଦି (container_status == 0)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "the container `%s` is not running", id);

  {
    cleanup_cgroup_status ସଂରଚନା libcrun_cgroup_status *cgroup_status = ଖାଲି;

    cgroup_status = libcrun_cgroup_make_status (&status);

    ret = libcrun_cgroup_is_container_paused (cgroup_status, &container_paused, err);
    ଯଦି (UNLIKELY (ret < 0))
      ଫେରନ୍ତୁ ret;
  }

  ଯଦି (UNLIKELY (container_paused))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot exec in a paused container `%s`", id);

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = block_signals (err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  libcrun_seccomp_gen_ctx_init (&seccomp_gen_ctx, container, false, 0);

  ret = libcrun_open_seccomp_bpf (&seccomp_gen_ctx, &seccomp_fd, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (seccomp_fd >= 0)
    {
      ret = get_seccomp_receiver_fd (container, &seccomp_receiver_fd,
                                     &own_seccomp_receiver_fd,
                                     &seccomp_notify_plugins, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (ଆକାର (*opts) != opts->struct_size)
    ଫେରନ୍ତୁ crun_make_error (err, EINVAL, "invalid libcrun_container_exec_options_s struct");

  ଯଦି (opts->path)
    {
      ଯଦି (process)
        ଫେରନ୍ତୁ crun_make_error (err, EINVAL, "cannot specify both exec file and options");

      ret = libcrun_load_process_from_file (opts->path, &process, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      process_cleanup = process;
    }

  /* This must be done before we enter a user namespace.  If the process does
     not specify any rlimits, use the ones from the container configuration.  */
  ଯଦି (process->rlimits_len == 0 && container->container_def->process)
    ret = libcrun_set_rlimits (container->container_def->process->rlimits,
                               container->container_def->process->rlimits_len, err);
  ଅନ୍ୟଥା
    ret = libcrun_set_rlimits (process->rlimits, process->rlimits_len, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_set_mempolicy (container->container_def, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = pipe2 (container_ret_status, O_CLOEXEC);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "pipe");
  pipefd0 = container_ret_status[0];
  pipefd1 = container_ret_status[1];

  /* If the new process block doesn't specify a SELinux label, AppArmor profile, user,
     I/O priority or scheduler, then use the configuration from the original config file.  */
  ଯଦି (container->container_def->process)
    {
      ଯଦି (process->io_priority == ଖାଲି && container->container_def->process->io_priority)
        {
          process->io_priority = clone_runtime_spec_schema_config_schema_process_io_priority (container->container_def->process->io_priority);
          ଯଦି (process->io_priority == ଖାଲି)
            OOM ();
        }

      ଯଦି (process->scheduler == ଖାଲି && container->container_def->process->scheduler)
        {
          process->scheduler = clone_runtime_spec_schema_config_schema_process_scheduler (container->container_def->process->scheduler);
          ଯଦି (process->scheduler == ଖାଲି)
            OOM ();
        }

      ଯଦି (process->selinux_label == ଖାଲି && container->container_def->process->selinux_label)
        process->selinux_label = xstrdup (container->container_def->process->selinux_label);

      ଯଦି (process->apparmor_profile == ଖାଲି && container->container_def->process->apparmor_profile)
        process->apparmor_profile = xstrdup (container->container_def->process->apparmor_profile);

      ଯଦି (process->user == ଖାଲି && container->container_def->process->user)
        {
          process->user = clone_runtime_spec_schema_config_schema_process_user (container->container_def->process->user);
          ଯଦି (process->user == ଖାଲି)
            OOM ();
        }
    }

  ret = initialize_security (container, process, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  /* Do not leak the session keyring of the caller into the container.  The
     keyring created for the container cannot be joined, so use an anonymous one.  */
  ଯଦି (! context->no_new_keyring && ! status.no_new_keyring)
    {
      libcrun_debug ("Creating new keyring for the exec process");

      ret = libcrun_create_keyring (container, ଖାଲି, process->selinux_label, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "prctl unset dumpable");

  pid = libcrun_join_process (context, container, status.pid, &status, opts->cgroup, context->detach,
                              process, process->terminal ? &terminal_fd : ଖାଲି, err);
  ଯଦି (UNLIKELY (pid < 0))
    ଫେରନ୍ତୁ pid;

  /* Process to exec.  */
  ଯଦି (pid == 0)
    {
      TEMP_FAILURE_RETRY (close (pipefd0));
      pipefd0 = -1;

      exec_process_entrypoint (context, container, process, &pipefd1, seccomp_fd, seccomp_receiver_fd, custom_handler, opts->merge_env, err);
      /* It gets here only on errors.  */
      ଯଦି (*err)
        {
          ଯଦି (pipefd1 < 0)
            libcrun_fail_with_error ((*err)->status, "%s", (*err)->ସନ୍ଦେଶ);
          ଅନ୍ୟଥା
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

  TEMP_FAILURE_RETRY (ଲିଖ (pipefd0, &pid, ଆକାର (pid)));

  ଯଦି (seccomp_fd >= 0)
    close_and_reset (&seccomp_fd);

  ଯଦି (terminal_fd >= 0)
    {
      unsigned short rows = 0, cols = 0;

      ଯଦି (process->console_size)
        {
          cols = process->console_size->width;
          rows = process->console_size->height;
        }

      ret = libcrun_terminal_setup_size (terminal_fd, rows, cols, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ଯଦି (context->console_socket)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
          cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା console_socket_fd = open_unix_domain_client_socket (context->console_socket, 0, err);
          ଯଦି (UNLIKELY (console_socket_fd < 0))
            ଫେରନ୍ତୁ console_socket_fd;
          ret = send_fd_to_socket (console_socket_fd, terminal_fd, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
          close_and_reset (&terminal_fd);
        }
      ଅନ୍ୟଥା
        {
          ret = libcrun_clear_onlcr (terminal_fd, err);
          ଯଦି (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              ଫେରନ୍ତୁ ret;
            }

          ret = libcrun_set_raw (0, &orig_terminal, err);
          ଯଦି (UNLIKELY (ret < 0))
            {
              flush_fd_to_err (context, terminal_fd);
              ଫେରନ୍ତୁ ret;
            }
        }
    }

  ret = TEMP_FAILURE_RETRY (ପଠ (pipefd0, &ret_from_child, ଆକାର (ret_from_child)));
  ଯଦି (ret != ଆକାର (ret_from_child))
    ret = crun_make_error (err, 0, "read pipe failed");
  ଅନ୍ୟଥା ଯଦି (ret_from_child != 0)
    {
      cleanup_free ଅକ୍ଷର *ସନ୍ଦେଶ = ଖାଲି;
      size_t len = 0;

      ret = read_all_fd (pipefd0, "error stream", &ସନ୍ଦେଶ, &len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
      /* the string from read_all_fd is always NUL terminated.  */
      ret = crun_make_error (err, ret_from_child, "%s", ସନ୍ଦେଶ);
    }
  ଅନ୍ୟଥା
    {
      /* Let's receive the seccomp notify fd and handle it as part of wait_for_process().  */
      ଯଦି (own_seccomp_receiver_fd >= 0)
        {
          seccomp_notify_fd = receive_fd_from_socket (own_seccomp_receiver_fd, err);
          ଯଦି (UNLIKELY (seccomp_notify_fd < 0))
            ଫେରନ୍ତୁ seccomp_notify_fd;

          ret = close_and_reset (&own_seccomp_receiver_fd);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "close seccomp receiver fd failed");
        }

      {
        ସଂରଚନା wait_for_process_args args = {
          .pid = pid,
          .context = context,
          .terminal_fd = terminal_fd,
          .notify_socket = -1,
          .container_ready_fd = ଖାଲି,
          .seccomp_notify_fd = seccomp_notify_fd,
          .seccomp_notify_plugins = seccomp_notify_plugins,
        };

        ret = wait_for_process (&args, err);
      }
    }

  flush_fd_to_err (context, terminal_fd);
  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_update (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, ଅଚଳ ଅକ୍ଷର *content, size_t len arg_unused,
                          libcrun_error_t *err)
{
  cleanup_custom_handler_instance ସଂରଚନା custom_handler_instance_s *custom_handler = ଖାଲି;
  runtime_spec_schema_config_linux_resources *resources = ଖାଲି;
  cleanup_container libcrun_container_t *container = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *state_ମୂଳ = context->state_ମୂଳ;
  ସଂରଚନା parser_context ctx = { 0, stderr };
  libcrun_container_status_t status = {};
  parser_error parser_err = ଖାଲି;
  json_object *doc = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = libcrun_read_container_status (&status, state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = read_container_config_from_state (&container, state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_configure_handler (context->handler_manager,
                                   context,
                                   container,
                                   &custom_handler,
                                   err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = parse_json_file (&doc, content, &ctx, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  resources = make_runtime_spec_schema_config_linux_resources (doc, &ctx, &parser_err);
  ଯଦି (UNLIKELY (resources == ଖାଲି))
    {
      ret = crun_make_error (err, 0, "cannot parse resources: %s", parser_err);
      ଯାଅ cleanup;
    }

  ଯଦି (custom_handler && custom_handler->vtable->modify_oci_configuration)
    {
      /* Adapt RESOURCES to be used from the modify_oci_configuration hook.  */
      cleanup_free runtime_spec_schema_config_linux *linux = xmalloc0 (ଆକାର (*linux));
      cleanup_free runtime_spec_schema_config_schema *def = xmalloc0 (ଆକାର (*def));

      def->linux = linux;
      linux->resources = resources;

      ret = custom_handler->vtable->modify_oci_configuration (custom_handler->cookie,
                                                              context,
                                                              def,
                                                              err);
      ଯଦି (UNLIKELY (ret < 0))
        ଯାଅ cleanup;
    }

  ret = libcrun_linux_container_update (&status, state_ମୂଳ, resources, err);

cleanup:
  ଯଦି (doc)
    json_object_put (doc);
  ମୁକ୍ତ (parser_err);
  ଯଦି (resources)
    free_runtime_spec_schema_config_linux_resources (resources);

  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_update_from_file (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, ଅଚଳ ଅକ୍ଷର *file, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *content = ଖାଲି;
  size_t len;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = read_all_file (file, &content, &len, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ libcrun_container_update (context, id, content, len, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
compare_update_ମୂଲ୍ୟଗୁଡ଼ିକ (ଅଚଳ ଶୂନ୍ୟ *a, ଅଚଳ ଶୂନ୍ୟ *b)
{
  ଅଚଳ ସଂରଚନା libcrun_update_ମୂଲ୍ୟ_s *aa = a;
  ଅଚଳ ସଂରଚନା libcrun_update_ମୂଲ୍ୟ_s *bb = b;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = strcmp (aa->section, bb->section);
  ଯଦି (ret)
    ଫେରନ୍ତୁ ret;
  ଫେରନ୍ତୁ strcmp (aa->name, bb->name);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_update_from_ମୂଲ୍ୟଗୁଡ଼ିକ (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id,
                                      ସଂରଚନା libcrun_update_ମୂଲ୍ୟ_s *ମୂଲ୍ୟଗୁଡ଼ିକ, size_t len,
                                      libcrun_error_t *err)
{
  ଅଚଳ ଅକ୍ଷର *current_section = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *buf;
  json_gen_ctx *gen = ଖାଲି;
  size_t i, buf_len;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (! json_gen_init (&gen, ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "json_gen_init failed");
  json_gen_map_open (gen);

  qsort (ମୂଲ୍ୟଗୁଡ଼ିକ, len, ଆକାର (ସଂରଚନା libcrun_update_ମୂଲ୍ୟ_s), compare_update_ମୂଲ୍ୟଗୁଡ଼ିକ);

  ପାଇଁ (i = 0; i < len; i++)
    {
      ଯଦି (current_section == ଖାଲି || strcmp (ମୂଲ୍ୟଗୁଡ଼ିକ[i].section, current_section))
        {
          ଯଦି (i > 0)
            json_gen_map_close (gen);

          current_section = ମୂଲ୍ୟଗୁଡ଼ିକ[i].section;
          json_gen_string (gen, current_section, strlen (current_section));
          json_gen_map_open (gen);
        }

      json_gen_string (gen, ମୂଲ୍ୟଗୁଡ଼ିକ[i].name, strlen (ମୂଲ୍ୟଗୁଡ଼ିକ[i].name));

      ଯଦି (ମୂଲ୍ୟଗୁଡ଼ିକ[i].numeric)
        json_gen_ସଂଖ୍ୟା (gen, ମୂଲ୍ୟଗୁଡ଼ିକ[i].ମୂଲ୍ୟ, strlen (ମୂଲ୍ୟଗୁଡ଼ିକ[i].ମୂଲ୍ୟ));
      ଅନ୍ୟଥା
        json_gen_string (gen, ମୂଲ୍ୟଗୁଡ଼ିକ[i].ମୂଲ୍ୟ, strlen (ମୂଲ୍ୟଗୁଡ଼ିକ[i].ମୂଲ୍ୟ));
    }
  ଯଦି (len)
    json_gen_map_close (gen);

  json_gen_map_close (gen);

  json_gen_get_buf (gen, &buf, &buf_len);

  ret = libcrun_container_update (context, id, buf, buf_len, err);

  json_gen_free (gen);

  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ
populate_array_field (ଅକ୍ଷର ***field, ଅକ୍ଷର *array[], size_t ଅଙ୍କ_elements)
{
  size_t i;

  *field = xmalloc0 ((ଅଙ୍କ_elements + 1) * ଆକାର (ଅକ୍ଷର *));
  ପାଇଁ (i = 0; i < ଅଙ୍କ_elements; i++)
    (*field)[i] = xstrdup (array[i]);

  (*field)[i] = ଖାଲି;
}

#ifdef HAVE_CAP
ସ୍ଥିର ଶୂନ୍ୟ
populate_capabilities (ସଂରଚନା features_info_s *info, ଅକ୍ଷର ***capabilities, size_t *ଅଙ୍କ_capabilities)
{
  size_t index = 0;
  cap_ମୂଲ୍ୟ_t i;
  ଅକ୍ଷର *endptr;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;

  *ଅଙ୍କ_capabilities = 0;
  ପାଇଁ (i = 0;; i++)
    {
      ଅକ୍ଷର *v = cap_to_name (i);
      ଯଦି (v == ଖାଲି)
        ଭାଙ୍ଗ;
      strtol (v, &endptr, 10);
      ଯଦି (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          ଭାଙ୍ଗ;
        }
      (*ଅଙ୍କ_capabilities)++;
    }

  *capabilities = xmalloc0 ((*ଅଙ୍କ_capabilities + 1) * ଆକାର (ଅଚଳ ଅକ୍ଷର *));
  ପାଇଁ (i = 0; i < (cap_ମୂଲ୍ୟ_t) *ଅଙ୍କ_capabilities; i++)
    {
      ଅକ୍ଷର *v = cap_to_name (i);
      ଯଦି (v == ଖାଲି)
        ଭାଙ୍ଗ;
      strtol (v, &endptr, 10);
      ଯଦି (endptr != v)
        {
          // Non-numeric or non-zero value encountered, break the loop
          ଭାଙ୍ଗ;
        }

      // Convert capability name to uppercase
      ପାଇଁ (j = 0; v[j] != '\0'; j++)
        v[j] = toupper (v[j]);

      (*capabilities)[index] = v;
      index++;
    }

  (*capabilities)[index] = ଖାଲି; // Terminate the array with NULL
  populate_array_field (&(info->linux.capabilities), *capabilities, *ଅଙ୍କ_capabilities);
}
#endif

ସ୍ଥିର ଶୂନ୍ୟ
retrieve_mount_options (ସଂରଚନା features_info_s **info)
{
  cleanup_free ଅଚଳ ସଂରଚନା propagation_flags_s *mount_options_list = ଖାଲି;
  size_t ଅଙ୍କ_mount_options = 0;

  // Retrieve mount options from wordlist
  mount_options_list = get_mount_flags_from_wordlist ();

  // Calculate the number of mount options
  ଯେତେବେଳେ (mount_options_list[ଅଙ୍କ_mount_options].name != ଖାଲି)
    ଅଙ୍କ_mount_options++;

  // Allocate memory for mount options in info struct
  (*info)->mount_options = xmalloc0 ((ଅଙ୍କ_mount_options + 1) * ଆକାର (ଅକ୍ଷର *));

  // Copy mount options to info struct
  ପାଇଁ (size_t i = 0; i < ଅଙ୍କ_mount_options; i++)
    (*info)->mount_options[i] = xstrdup (mount_options_list[i].name);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_get_features (libcrun_context_t *context, ସଂରଚନା features_info_s **info, libcrun_error_t *err arg_unused)
{
  // Allocate memory for the features_info_s structure
  size_t ଅଙ୍କ_namspaces = ଆକାର (namespaces) / ଆକାର (namespaces[0]);
  size_t ଅଙ୍କ_operators = ଆକାର (operators) / ଆକାର (operators[0]);
  size_t ଅଙ୍କ_actions = ଆକାର (actions) / ଆକାର (actions[0]);
  size_t ଅଙ୍କ_hooks = ଆକାର (hooks) / ଆକାର (hooks[0]);
  size_t ଅଙ୍କ_archs = ଆକାର (archs) / ଆକାର (archs[0]);
  size_t ଅଙ୍କ_unsafe_annotations = ଆକାର (potentially_unsafe_annotations) / ଆକାର (potentially_unsafe_annotations[0]);
  cleanup_free ଅକ୍ଷର **capabilities = ଖାଲି;
  size_t ଅଙ୍କ_capabilities = 0;
  size_t ଅଙ୍କ_mempolicy_modes = ଆକାର (mempolicy_modes) / ଆକାର (mempolicy_modes[0]);
  size_t ଅଙ୍କ_mempolicy_flags = ଆକାର (mempolicy_flags) / ଆକାର (mempolicy_flags[0]);

  *info = xmalloc0 (ଆକାର (ସଂରଚନା features_info_s));

  // Hardcoded feature information
  (*info)->oci_version_min = xstrdup ("1.0.0");
  (*info)->oci_version_max = xstrdup ("1.1.0+dev");

  // Populate hooks
  populate_array_field (&((*info)->hooks), hooks, ଅଙ୍କ_hooks);

  // Populate mount_options
  retrieve_mount_options (info);

  // Populate namespaces
  populate_array_field (&((*info)->linux.namespaces), namespaces, ଅଙ୍କ_namspaces);

#ifdef HAVE_CAP
  // Populate capabilities
  populate_capabilities (*info, &capabilities, &ଅଙ୍କ_capabilities);
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
  populate_array_field (&((*info)->linux.seccomp.actions), actions, ଅଙ୍କ_actions);
  // Populate operators
  populate_array_field (&((*info)->linux.seccomp.operators), operators, ଅଙ୍କ_operators);
  // Populate archs
  populate_array_field (&((*info)->linux.seccomp.archs), archs, ଅଙ୍କ_archs);
#ଅନ୍ୟଥା
  (*info)->linux.seccomp.enabled = false;
#endif

  // Put values for apparmor and selinux
  (*info)->linux.apparmor.enabled = true;
  (*info)->linux.selinux.enabled = true;

  (*info)->linux.intel_rdt.enabled = true;

  (*info)->linux.net_devices.enabled = true;

  populate_array_field (&((*info)->linux.memory_policy.mode), mempolicy_modes, ଅଙ୍କ_mempolicy_modes);
  populate_array_field (&((*info)->linux.memory_policy.flags), mempolicy_flags, ଅଙ୍କ_mempolicy_flags);

  // Put the values for mount extensions
  (*info)->linux.mount_ext.idmap.enabled = true;

  // Populate the values for annotations
#ifdef HAVE_SECCOMP
  {
    ଅଚଳ ସଂରଚନା scmp_version *version = seccomp_version ();
    ଅକ୍ଷର *version_string = ଖାଲି;

    xasprintf (&version_string, "%u.%u.%u", version->major, version->minor, version->micro);
    (*info)->annotations.io_github_seccomp_libseccomp_version = version_string;
  }
#endif

  ଯଦି (context->handler_manager && handler_by_name (context->handler_manager, "wasm"))
    (*info)->annotations.run_oci_crun_wasm = true;

#ଯଦି HAVE_CRIU
  (*info)->annotations.run_oci_crun_checkpoint_enabled = true;
#endif
  (*info)->annotations.run_oci_crun_commit = GIT_VERSION;
  (*info)->annotations.run_oci_crun_version = PACKAGE_VERSION;

  populate_array_field (&((*info)->potentially_unsafe_annotations), potentially_unsafe_annotations, ଅଙ୍କ_unsafe_annotations);

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ
features_add_string (json_gen_ctx *json_gen, ଅଚଳ ଅକ୍ଷର *ଚାବି, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ)
{
  json_gen_string (json_gen, ଚାବି, strlen (ଚାବି));
  json_gen_string (json_gen, ମୂଲ୍ୟ, strlen (ମୂଲ୍ୟ));
}

ସ୍ଥିର ଶୂନ୍ୟ
features_add_bool (json_gen_ctx *json_gen, ଅଚଳ ଅକ୍ଷର *ଚାବି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ)
{
  json_gen_string (json_gen, ଚାବି, strlen (ଚାବି));
  json_gen_bool (json_gen, ମୂଲ୍ୟ);
}

ସ୍ଥିର ଶୂନ୍ୟ
features_add_bool_str (json_gen_ctx *json_gen, ଅଚଳ ଅକ୍ଷର *ଚାବି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ)
{
  features_add_string (json_gen, ଚାବି, ମୂଲ୍ୟ ? "true" : "false");
}

ସ୍ଥିର ଶୂନ୍ୟ
features_add_array (json_gen_ctx *json_gen, ଅଚଳ ଅକ୍ଷର *ଚାବି, ଅକ୍ଷର **array)
{
  size_t i;

  json_gen_string (json_gen, ଚାବି, strlen (ଚାବି));
  json_gen_array_open (json_gen);
  ଯଦି (array)
    ପାଇଁ (i = 0; array[i] != ଖାଲି; i++)
      json_gen_string (json_gen, array[i], strlen (array[i]));
  json_gen_array_close (json_gen);
}

ସ୍ଥିର ଶୂନ୍ୟ
features_add_linux_info (json_gen_ctx *json_gen, ଅଚଳ ସଂରଚନା linux_info_s *linux)
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
  ଯଦି (linux->seccomp.actions)
    features_add_array (json_gen, "actions", linux->seccomp.actions);
  ଯଦି (linux->seccomp.operators)
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
  ଯଦି (linux->memory_policy.mode)
    features_add_array (json_gen, "modes", linux->memory_policy.mode);
  ଯଦି (linux->memory_policy.flags)
    features_add_array (json_gen, "flags", linux->memory_policy.flags);
  json_gen_map_close (json_gen);

  json_gen_map_close (json_gen);
}

ସ୍ଥିର ଶୂନ୍ୟ
features_add_annotations_info (json_gen_ctx *json_gen, ଅଚଳ ସଂରଚନା annotations_info_s *annotation)
{
  json_gen_string (json_gen, "annotations", strlen ("annotations"));
  json_gen_map_open (json_gen);

  ଯଦି (! is_empty_string (annotation->io_github_seccomp_libseccomp_version))
    features_add_string (json_gen, "io.github.seccomp.libseccomp.version", annotation->io_github_seccomp_libseccomp_version);

  features_add_bool_str (json_gen, "org.opencontainers.runc.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);
  features_add_bool_str (json_gen, "run.oci.crun.checkpoint.enabled", annotation->run_oci_crun_checkpoint_enabled);

  features_add_string (json_gen, "run.oci.crun.commit", annotation->run_oci_crun_commit);
  features_add_string (json_gen, "run.oci.crun.version", annotation->run_oci_crun_version);

  features_add_bool_str (json_gen, "run.oci.crun.wasm", annotation->run_oci_crun_wasm);

  json_gen_map_close (json_gen);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_get_features_json (libcrun_context_t *context, ଅକ୍ଷର **out, libcrun_error_t *err)
{
  cleanup_struct_features ସଂରଚନା features_info_s *info = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *buffer = ଖାଲି;
  json_gen_ctx *json_gen;
  size_t buffer_len;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  *out = ଖାଲି;

  ret = libcrun_container_get_features (context, &info, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (! json_gen_init (&json_gen, ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot initialize JSON generator");

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

  ଫେରନ୍ତୁ 0;
}

/* Read the container status and fail if the container is not running.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
read_status_require_running (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id,
                             libcrun_container_status_t *status, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = libcrun_read_container_status (status, context->state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_is_container_running (status, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;
  ଯଦି (ret == 0)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "the container `%s` is not running", id);

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_pause (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ libcrun_container_pause_linux (&status, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_unpause (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  libcrun_container_status_t status = {};

  ret = read_status_require_running (context, id, &status, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ libcrun_container_unpause_linux (&status, err);
}

/* Translate the public options struct into the internal representation used by
   the linux/CRIU layer.  Strings are borrowed (not copied).  */
ସ୍ଥିର ଶୂନ୍ୟ
cr_options_from_public (libcrun_checkpoint_restore_t *dst, ସଂରଚନା libcrun_checkpoint_restore_options_s *src)
{
  memset (dst, 0, ଆକାର (*dst));
  dst->image_path = (ଅକ୍ଷର *) src->image_path;
  dst->work_path = (ଅକ୍ଷର *) src->work_path;
  dst->parent_path = (ଅକ୍ଷର *) src->parent_path;
  dst->console_socket = src->console_socket;
  dst->lsm_profile = (ଅକ୍ଷର *) src->lsm_profile;
  dst->lsm_mount_context = (ଅକ୍ଷର *) src->lsm_mount_context;
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

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_checkpoint (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id,
                              ସଂରଚନା libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ଅଚଳ ଅକ୍ଷର *state_ମୂଳ = context->state_ମୂଳ;
  libcrun_container_status_t status = {};
  libcrun_checkpoint_restore_t cr;
  cleanup_container libcrun_container_t *container = ଖାଲି;

  cr_options_from_public (&cr, cr_options);

  ret = read_status_require_running (context, id, &status, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = read_container_config_from_state (&container, state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  /* Needed to use the same paths (e.g. the shared empty directory used for
     masked paths) as the container runtime and restore do.  */
  container->context = context;

  ret = libcrun_container_checkpoint_linux (&status, container, &cr, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (! (cr_options->leave_running || cr_options->pre_dump))
    ଫେରନ୍ତୁ container_delete_internal (context, ଖାଲି, ଖାଲି, id, true, true, err);

  ଫେରନ୍ତୁ 0;
}

/* The dummy process only keeps the cgroup populated until the container is restored there.
   Its cgroup is below the container cgroup, so once the container runs it can freeze or kill
   the dummy process: never depend on it after that point.  */
ସ୍ଥିର ଶୂନ୍ୟ
restore_proxy_process (ପୂର୍ଣ୍ଣସଂଖ୍ୟା proxy_pid_pipe)
{
  ଅକ୍ଷର c;

  /* Wait to be killed by the parent, or for the parent to go away.  */
  TEMP_FAILURE_RETRY (ପଠ (proxy_pid_pipe, &c, ଆକାର (c)));
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
restore_terminate_proxy_process (pid_t proxy_pid, ଅଚଳ ଅକ୍ଷର *proxy_cgroup, pid_t container_pid, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_manager, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (cgroup_manager == CGROUP_MANAGER_SYSTEMD)
    {
      ଅକ୍ଷର ready_str[64];

      ret = snprintf (ready_str, ଆକାର (ready_str), "MAINPID=%d", container_pid);
      ଯଦି (UNLIKELY (ret >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (ready_str)))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

      ret = send_sd_notify (ready_str, err);
      /* Do not fail on errors.  */
      ଯଦି (UNLIKELY (ret < 0))
        crun_error_release (err);
    }

  mode = libcrun_get_cgroup_mode (err);
  ଯଦି (UNLIKELY (mode < 0))
    ଫେରନ୍ତୁ mode;

  /* SIGKILL terminates a process frozen with cgroup v2, while the cgroup v1 freezer does not
     deliver it until the cgroup is thawed, so do not wait for it there.  */
  kill (proxy_pid, SIGKILL);
  TEMP_FAILURE_RETRY (waitpid (proxy_pid, ଖାଲି, mode == CGROUP_MODE_UNIFIED ? 0 : WNOHANG));

  ଫେରନ୍ତୁ destroy_cgroup_path (proxy_cgroup, mode, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_restore (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id arg_unused,
                           ସଂରଚନା libcrun_checkpoint_restore_options_s *cr_options, libcrun_error_t *err)
{
  cleanup_cgroup_status ସଂରଚନା libcrun_cgroup_status *cgroup_status = ଖାଲି;
  cleanup_container libcrun_container_t *container = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା proxy_pid_pipe0 = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା proxy_pid_pipe1 = -1;
  runtime_spec_schema_config_schema *def;
  libcrun_container_status_t status = {};
  ସଂରଚନା libcrun_seccomp_gen_ctx_s seccomp_gen_ctx;
  libcrun_checkpoint_restore_t cr;
  cleanup_pid pid_t proxy_pid = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା seccomp_fd = -1;
  ଅଚଳ ଅକ୍ଷର *seccomp_bpf_data;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା proxy_pid_pipe[2];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_manager;
  uid_t ମୂଳ_uid = -1;
  gid_t ମୂଳ_gid = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  cr_options_from_public (&cr, cr_options);

  container = libcrun_container_load_from_file ("config.json", err);
  ଯଦି (container == ଖାଲି)
    ଫେରନ୍ତୁ -1;

  container->context = context;
  def = container->container_def;

  ret = check_config_file (def, context, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_status_check_directories (context->state_ମୂଳ, context->id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_copy_config_file (context->id, context->state_ମୂଳ, container, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ fail;

  /* CRIU restores the seccomp filters of the checkpointed processes, but
     exec loads the filter from the seccomp.bpf file in the state directory,
     so it must be generated again for the restored container.  */
  seccomp_bpf_data = find_annotation (container, "run.oci.seccomp_bpf_data");

  ret = setup_seccomp (container, seccomp_bpf_data, &seccomp_gen_ctx, &seccomp_fd, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ fail;

  ret = seccomp_generation (seccomp_fd, seccomp_bpf_data, &seccomp_gen_ctx, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ fail;
  close_and_reset (&seccomp_fd);

  cgroup_manager = CGROUP_MANAGER_CGROUPFS;
  ଯଦି (context->systemd_cgroup)
    cgroup_manager = CGROUP_MANAGER_SYSTEMD;
  ଅନ୍ୟଥା ଯଦି (context->force_no_cgroup)
    cgroup_manager = CGROUP_MANAGER_DISABLED;

  def = container->container_def;

  /* If we are root (either on the host or in a namespace),
   * then chown the cgroup to root in the container user namespace. */
  get_ମୂଳ_in_the_userns (def, container->host_uid, container->host_gid, &ମୂଳ_uid, &ମୂଳ_gid);

  /* If the root in the container is different than the current root user, attempt to chown
     the std streams before entering the user namespace.  Otherwise we might lose access
     to the user (as it is not mapped in the user namespace) and cannot chown them.  */
  ଯଦି (ମୂଳ_uid > 0 || ମୂଳ_gid > 0)
    {
      ret = maybe_chown_std_streams (ମୂଳ_uid, ମୂଳ_gid, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଯାଅ fail;
    }

  /* CRIU runs as a child process, so the restored processes would inherit the session keyring.  */
  ret = setup_container_keyring (container, context, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  {
    cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_dirfd = -1;
    bool needs_proxy_process;
    cleanup_cgroup_preenter ସଂରଚନା libcrun_cgroup_args cg = {
      .resources = def->linux ? def->linux->resources : ଖାଲି,
      .annotations = container->annotations,
      .cgroup_path = def->linux ? def->linux->cgroups_path : "",
      .manager = cgroup_manager,
      .ମୂଳ_uid = ମୂଳ_uid,
      .ମୂଳ_gid = ମୂଳ_gid,
      .id = context->id,
      .state_ମୂଳ = context->state_ମୂଳ,
      .container = container,
    };

    /* The CRIU restore code uses bundle, rootfs and cgroup_path of status.   The cgroup_path is set later.  */
    status.bundle = (ଅକ୍ଷର *) context->bundle;
    status.rootfs = def->ମୂଳ->path;

    ret = libcrun_cgroup_preenter (&cg, &cgroup_dirfd, err);
    ଯଦି (UNLIKELY (ret < 0))
      ଯାଅ fail;

    needs_proxy_process = cgroup_dirfd < 0;

    /* If the target cgroup was already created (with cgroupfs), we can restore the container directly there.  */
    ଯଦି (! needs_proxy_process)
      {
        cleanup_free ଅକ୍ଷର *cgroup_path = ଖାଲି;

        ret = get_cgroup_dirfd_path (cgroup_dirfd, &cgroup_path, err);
        ଯଦି (UNLIKELY (ret < 0))
          ଯାଅ fail;

        /* Complete the configuration for the cgroup before the container is
           restored there: CRIU resumes the restored processes before it
           returns, so the device filter and the limits must already be in
           place.  There is no process to move, as CRIU is run in the cgroup,
           and the controllers were enabled by precreate_cgroup.  */
        cg.joined = true;

        ret = libcrun_cgroup_enter (&cg, &cgroup_status, err);
        ଯଦି (UNLIKELY (ret < 0))
          ଯାଅ fail;

        /* Restore the container directly in the desired cgroup.  */
        status.cgroup_path = cgroup_path;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        ଯଦି (UNLIKELY (ret < 0))
          ଯାଅ fail;

        /* Now that the container is restored, keep any signal for it, so that
           it is forwarded to the init by wait_for_process, rather than acted
           upon by crun.  This cannot be done earlier, as CRIU, which is run as
           a child process, would inherit the blocked signals.  */
        ret = block_signals (err);
        ଯଦି (UNLIKELY (ret < 0))
          ଯାଅ fail;

        /* Use the container first process PID to finalize the cgroup.  */
        cg.pid = status.pid;
      }
    ଅନ୍ୟଥା
      {
        /* When using systemd, we need a process first that must exist before a cgroup is created.
           Create a dummy process that sits and waits until the container is restored.  */
        ret = pipe2 (proxy_pid_pipe, O_CLOEXEC);
        ଯଦି (UNLIKELY (ret < 0))
          {
            ret = crun_make_error (err, errno, "pipe");
            ଯାଅ fail;
          }

        /* Only used for auto cleanup.  */
        proxy_pid_pipe0 = proxy_pid_pipe[0];
        proxy_pid_pipe1 = proxy_pid_pipe[1];

        proxy_pid = fork ();
        ଯଦି (UNLIKELY (proxy_pid < 0))
          {
            ret = crun_make_error (err, errno, "fork");
            ଯାଅ fail;
          }

        ଯଦି (proxy_pid == 0)
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
        ଯଦି (UNLIKELY (ret < 0))
          ଯାଅ fail;
      }

    ret = libcrun_cgroup_enter_finalize (&cg, cgroup_status, err);
    ଯଦି (UNLIKELY (ret < 0))
      ଯାଅ fail;

    /* When using a dummy process, the container is restored here once the cgroup is configured.  */
    ଯଦି (needs_proxy_process)
      {
        cleanup_free ଅକ୍ଷର *target_cgroup = ଖାଲି;
        cleanup_free ଅକ୍ଷର *proxy_cgroup = ଖାଲି;

        ret = libcrun_get_cgroup_process (proxy_pid, &target_cgroup, false, err);
        ଯଦି (UNLIKELY (ret < 0))
          ଯାଅ fail;

        /* Move the dummy process to a sub-cgroup.  */
        ret = append_paths (&proxy_cgroup, err, target_cgroup, ".proxy-crun", ଖାଲି);
        ଯଦି (UNLIKELY (ret < 0))
          ଯାଅ fail;

        ret = libcrun_move_process_to_cgroup (proxy_pid, 0, proxy_cgroup, true, err);
        ଯଦି (UNLIKELY (ret < 0))
          ଯାଅ fail;

        /* Restore the container in the same cgroup where the dummy process was.  */
        status.cgroup_path = target_cgroup;

        ret = libcrun_container_restore_linux (&status, container, &cr, err);
        ଯଦି (UNLIKELY (ret < 0))
          ଯାଅ fail;

        /* See the comment for the other libcrun_container_restore_linux call.  */
        ret = block_signals (err);
        ଯଦି (UNLIKELY (ret < 0))
          ଯାଅ fail;

        /* The container is running, so do not fail because of the dummy process.  */
        ret = restore_terminate_proxy_process (proxy_pid, proxy_cgroup, status.pid, cgroup_manager, err);
        ଯଦି (UNLIKELY (ret < 0))
          crun_error_release (err);
        proxy_pid = -1;

        close_and_reset (&proxy_pid_pipe1);
      }
  }

  context->detach = cr_options->detach;
  ret = write_container_status (container, context, status.pid, cgroup_status, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ fail;

  {
    /* Wait the same way run and exec do: apart from waiting for the container
       init, this writes the pid file, forwards any signal to the init, and
       reaps every child, not only the init.  */
    ସଂରଚନା wait_for_process_args args = {
      .pid = status.pid,
      .context = context,
      .terminal_fd = -1,
      .notify_socket = -1,
      .container_ready_fd = ଖାଲି,
      .seccomp_notify_fd = -1,
      .seccomp_notify_plugins = ଖାଲି,
    };

    ଫେରନ୍ତୁ wait_for_process (&args, err);
  }

fail:
  /* Once restored, the container is running.  Without a status file nothing
     can reach it anymore, so do not leave it behind.  */
  ଯଦି (status.pid > 0)
    {
      kill (status.pid, SIGKILL);
      waitpid_ignore_stopped (status.pid, ଖାଲି, 0);
    }
  ଯଦି (cgroup_status)
    {
      libcrun_error_t tmp_err = ଖାଲି;
      libcrun_cgroup_destroy (cgroup_status, &tmp_err);
      crun_error_release (&tmp_err);
    }
  {
    libcrun_error_t tmp_err = ଖାଲି;

    ଯଦି (libcrun_container_delete_status (context->state_ମୂଳ, context->id, &tmp_err) < 0)
      crun_error_release (&tmp_err);
  }
  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_read_pids (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  cleanup_cgroup_status ସଂରଚନା libcrun_cgroup_status *cgroup_status = ଖାଲି;
  cleanup_container_status libcrun_container_status_t status = {};
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = libcrun_read_container_status (&status, context->state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (status.cgroup_path == ଖାଲି || status.cgroup_path[0] == '\0')
    ଫେରନ୍ତୁ crun_make_error (err, 0, "the container is not using cgroups");

  cgroup_status = libcrun_cgroup_make_status (&status);

  ଫେରନ୍ତୁ libcrun_cgroup_read_pids (cgroup_status, recurse, pids, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_list_json (libcrun_context_t *context, ଅକ୍ଷର **out, libcrun_error_t *err)
{
  libcrun_container_list_t *list = ଖାଲି, *it;
  ଅଚଳ ଅକ୍ଷର *content = ଖାଲି;
  json_gen_ctx *gen = ଖାଲି;
  size_t len;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  *out = ଖାଲି;

  ret = libcrun_get_containers_list (&list, context->state_ମୂଳ, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (! json_gen_init (&gen, ଖାଲି))
    {
      ret = crun_make_error (err, 0, "cannot allocate json generator");
      ଯାଅ exit;
    }

  json_gen_config (gen, json_gen_beautify, 1);
  json_gen_array_open (gen);

  ପାଇଁ (it = list; it; it = it->next)
    {
      libcrun_container_status_t status;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା running = 0;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା pid;
      ଅଚଳ ଅକ୍ଷର *container_status = ଖାଲି;

      ret = libcrun_read_container_status (&status, context->state_ମୂଳ, it->name, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଯାଅ exit;

      pid = status.pid;

      ret = libcrun_get_container_state_string (it->name, &status, context->state_ମୂଳ, &container_status,
                                                &running, err);
      ଯଦି (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          ଜାରି;
        }

      ଯଦି (! running)
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
  ଯଦି (json_gen_get_buf (gen, &content, &len) != json_gen_status_ok)
    {
      ret = libcrun_make_error (err, 0, "cannot generate json list");
      ଯାଅ exit;
    }

  *out = xstrdup (content);
  ret = 0;

exit:
  ଯଦି (list)
    libcrun_free_containers_list (list);
  ଯଦି (gen)
    json_gen_free (gen);

  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_write_json_containers_list (libcrun_context_t *context, FILE *out, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *content = ଖାଲି;
  size_t len;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = libcrun_container_list_json (context, &content, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  len = strlen (content);
  ଯଦି (len && fwrite (content, 1, len, out) != len)
    ଫେରନ୍ତୁ crun_make_error (err, errno, "error writing to file");

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_update_intel_rdt (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, ସଂରଚନା libcrun_intel_rdt_update *update, libcrun_error_t *err)
{
  cleanup_container libcrun_container_t *container = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = read_container_config_from_state (&container, context->state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ libcrun_update_intel_rdt (id, container, update->l3_cache_schema, update->mem_bw_schema, update->schemata, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_add_or_remove_mounts_from_file (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, ଅଚଳ ଅକ୍ଷର *file, bool add, libcrun_error_t *err)
{
  cleanup_custom_handler_instance ସଂରଚନା custom_handler_instance_s *custom_handler = ଖାଲି;
  cleanup_container libcrun_container_t *container = ଖାଲି;
  cleanup_free runtime_spec_schema_defs_mount **mounts = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *state_ମୂଳ = context->state_ମୂଳ;
  cleanup_free parser_error parser_err = ଖାଲି;
  ସଂରଚନା parser_context pctx = { 0, stderr };
  libcrun_container_status_t status = {};
  cleanup_free ଅକ୍ଷର *content = ଖାଲି;
  json_object *doc = ଖାଲି;
  size_t n_mounts = 0, len, i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 1;

  ret = read_all_file (file, &content, &len, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_read_container_status (&status, state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = read_container_config_from_state (&container, state_ମୂଳ, id, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = parse_json_file (&doc, content, &pctx, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (! json_object_is_type (doc, json_type_array))
    {
      ret = crun_make_error (err, 0, "mounts must be an array");
      ଯାଅ cleanup;
    }
  ଅନ୍ୟଥା
    {
      n_mounts = json_object_array_length (doc);

      mounts = xmalloc0 ((n_mounts + 1) * ଆକାର (*mounts));
      ପାଇଁ (i = 0; i < n_mounts; i++)
        {
          mounts[i] = make_runtime_spec_schema_defs_mount (json_object_array_get_idx (doc, i), &pctx, &parser_err);
          ଯଦି (mounts[i] == ଖାଲି)
            {
              ret = crun_make_error (err, 0, "cannot parse mount: %s", parser_err);
              ଯାଅ cleanup;
            }
        }
    }

  ଯଦି (add)
    ret = libcrun_make_runtime_mounts (container, &status, mounts, n_mounts, err);
  ଅନ୍ୟଥା
    ret = libcrun_destroy_runtime_mounts (container, &status, mounts, n_mounts, err);

cleanup:
  ଯଦି (doc)
    json_object_put (doc);
  ଯଦି (mounts)
    ପାଇଁ (i = 0; i < n_mounts; i++)
      free_runtime_spec_schema_defs_mount (mounts[i]);
  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_add_mounts_from_file (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, ଅଚଳ ଅକ୍ଷର *file, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ libcrun_container_add_or_remove_mounts_from_file (context, id, file, true, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_remove_mounts_from_file (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, ଅଚଳ ଅକ୍ଷର *file, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ libcrun_container_add_or_remove_mounts_from_file (context, id, file, false, err);
}
