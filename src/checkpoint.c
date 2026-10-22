/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2020 Adrian Reber <areber@redhat.com>
 * crun is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * crun is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with crun.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _GNU_SOURCE

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <argp.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <regex.h>

#ଅନ୍ତର୍ଭାଵ "crun.h"

ଗଣନା
{
  OPTION_IMAGE_PATH = 1000,
  OPTION_WORK_PATH,
  OPTION_LEAVE_RUNNING,
  OPTION_TCP_ESTABLISHED,
  OPTION_SHELL_JOB,
  OPTION_EXT_UNIX_SK,
  OPTION_FILE_LOCKS,
  OPTION_NETWORK_LOCK_METHOD,
  OPTION_PARENT_PATH,
  OPTION_PRE_DUMP,
  OPTION_MANAGE_CGROUPS_MODE,
};

ସ୍ଥିର ଅକ୍ଷର doc[] = "OCI runtime";

ସ୍ଥିର ସଂରଚନା libcrun_checkpoint_restore_options_s cr_options;

ସ୍ଥିର ସଂରଚନା argp_option options[]
    = { { "image-path", OPTION_IMAGE_PATH, "DIR", 0, "path for saving criu image files", 0 },
        { "work-path", OPTION_WORK_PATH, "DIR", 0, "path for saving work files and logs", 0 },
        { "leave-running", OPTION_LEAVE_RUNNING, 0, 0, "leave the process running after checkpointing", 0 },
        { "tcp-established", OPTION_TCP_ESTABLISHED, 0, 0, "allow open tcp connections", 0 },
        { "ext-unix-sk", OPTION_EXT_UNIX_SK, 0, 0, "allow external unix sockets", 0 },
        { "shell-job", OPTION_SHELL_JOB, 0, 0, "allow shell jobs", 0 },
        { "file-locks", OPTION_FILE_LOCKS, 0, 0, "allow file locks", 0 },
        { "network-lock", OPTION_NETWORK_LOCK_METHOD, "METHOD", 0, "set network lock method", 0 },
#ifdef CRIU_PRE_DUMP_SUPPORT
        { "parent-path", OPTION_PARENT_PATH, "DIR", 0, "path for previous criu image files in pre-dump", 0 },
        { "pre-dump", OPTION_PRE_DUMP, 0, 0, "dump container's memory information only, leave the container running after this", 0 },
#endif
        { "manage-cgroups-mode", OPTION_MANAGE_CGROUPS_MODE, "MODE", 0, "cgroups mode: 'soft' (default), 'ignore', 'full' and 'strict'", 0 },
        {
            0,
        } };

ସ୍ଥିର ଅକ୍ଷର args_doc[] = "checkpoint CONTAINER";

/* Map the CLI names onto the public LIBCRUN_CR_* enums; libcrun translates
   these to CRIU's own values internally so CRIU headers never reach here.  */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
crun_parse_network_lock_method (ଅକ୍ଷର *param)
{
  ଯଦି (strcmp (param, "iptables") == 0)
    ଫେରନ୍ତୁ LIBCRUN_CR_NETWORK_LOCK_IPTABLES;
  ଅନ୍ୟଥା ଯଦି (strcmp (param, "nftables") == 0)
    ଫେରନ୍ତୁ LIBCRUN_CR_NETWORK_LOCK_NFTABLES;
  ଅନ୍ୟଥା ଯଦି (strcmp (param, "skip") == 0)
    ଫେରନ୍ତୁ LIBCRUN_CR_NETWORK_LOCK_SKIP;
  ଅନ୍ୟଥା
    libcrun_fail_with_error (0, "unknown network lock method specified");
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
crun_parse_manage_cgroups_mode (ଅକ୍ଷର *param)
{
  ଯଦି (strcmp (param, "soft") == 0)
    ଫେରନ୍ତୁ LIBCRUN_CR_CG_MODE_SOFT;
  ଅନ୍ୟଥା ଯଦି (strcmp (param, "ignore") == 0)
    ଫେରନ୍ତୁ LIBCRUN_CR_CG_MODE_IGNORE;
  ଅନ୍ୟଥା ଯଦି (strcmp (param, "full") == 0)
    ଫେରନ୍ତୁ LIBCRUN_CR_CG_MODE_FULL;
  ଅନ୍ୟଥା ଯଦି (strcmp (param, "strict") == 0)
    ଫେରନ୍ତୁ LIBCRUN_CR_CG_MODE_STRICT;
  ଅନ୍ୟଥା
    libcrun_fail_with_error (0, "unknown cgroup mode specified");
}

ସ୍ଥିର error_t
parse_opt (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଚାବି, ଅକ୍ଷର *arg, ସଂରଚନା argp_state *state)
{
  ବାଛ (ଚାବି)
    {
    ସ୍ଥିତି ARGP_ଚାବି_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    ସ୍ଥିତି OPTION_IMAGE_PATH:
      cr_options.image_path = argp_mandatory_argument (arg, state);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_WORK_PATH:
      cr_options.work_path = argp_mandatory_argument (arg, state);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_PARENT_PATH:
      cr_options.parent_path = argp_mandatory_argument (arg, state);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_PRE_DUMP:
      cr_options.pre_dump = true;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_LEAVE_RUNNING:
      cr_options.leave_running = true;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_TCP_ESTABLISHED:
      cr_options.tcp_established = true;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_EXT_UNIX_SK:
      cr_options.ext_unix_sk = true;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_SHELL_JOB:
      cr_options.shell_job = true;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_FILE_LOCKS:
      cr_options.file_locks = true;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_MANAGE_CGROUPS_MODE:
      cr_options.manage_cgroups_mode = crun_parse_manage_cgroups_mode (argp_mandatory_argument (arg, state));
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_NETWORK_LOCK_METHOD:
      cr_options.network_lock_method = crun_parse_network_lock_method (argp_mandatory_argument (arg, state));
      ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ ARGP_ERR_UNKNOWN;
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ସଂରଚନା argp run_argp = { options, parse_opt, args_doc, doc, ଖାଲି, ଖାଲି, ଖାଲି };

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
crun_command_checkpoint (ସଂରଚନା crun_global_arguments *global_args, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *cr_path = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_arg;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  cleanup_context libcrun_context_t *crun_context = ଖାଲି;

  cr_options.struct_size = ଆକାର (cr_options);

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &cr_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (cr_options.image_path == ଖାଲି)
    {
      cleanup_free ଅକ୍ଷର *path = ଖାଲି;

      path = getcwd (ଖାଲି, 0);
      ଯଦି (UNLIKELY (path == ଖାଲି))
        libcrun_fail_with_error (errno, "getcwd failed");

      ret = asprintf (&cr_path, "%s/checkpoint", path);
      ଯଦି (UNLIKELY (ret < 0))
        OOM ();
      cr_options.image_path = cr_path;
    }

  ଫେରନ୍ତୁ libcrun_container_checkpoint (crun_context, argv[first_arg], &cr_options, err);
}
