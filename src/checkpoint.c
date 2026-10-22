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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <argp.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <regex.h>

#অন্তর্ভাব "crun.h"

গণনা
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

স্থির বর্ণ doc[] = "OCI runtime";

স্থির সংরচনা libcrun_checkpoint_restore_options_s cr_options;

স্থির সংরচনা argp_option options[]
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

স্থির বর্ণ args_doc[] = "checkpoint CONTAINER";

/* Map the CLI names onto the public LIBCRUN_CR_* enums; libcrun translates
   these to CRIU's own values internally so CRIU headers never reach here.  */
পূর্ণাঙ্ক
crun_parse_network_lock_method (বর্ণ *param)
{
  যদি (strcmp (param, "iptables") == 0)
    প্রত্যাগম LIBCRUN_CR_NETWORK_LOCK_IPTABLES;
  অন্যথা যদি (strcmp (param, "nftables") == 0)
    প্রত্যাগম LIBCRUN_CR_NETWORK_LOCK_NFTABLES;
  অন্যথা যদি (strcmp (param, "skip") == 0)
    প্রত্যাগম LIBCRUN_CR_NETWORK_LOCK_SKIP;
  অন্যথা
    libcrun_fail_with_error (0, "unknown network lock method specified");
}

পূর্ণাঙ্ক
crun_parse_manage_cgroups_mode (বর্ণ *param)
{
  যদি (strcmp (param, "soft") == 0)
    প্রত্যাগম LIBCRUN_CR_CG_MODE_SOFT;
  অন্যথা যদি (strcmp (param, "ignore") == 0)
    প্রত্যাগম LIBCRUN_CR_CG_MODE_IGNORE;
  অন্যথা যদি (strcmp (param, "full") == 0)
    প্রত্যাগম LIBCRUN_CR_CG_MODE_FULL;
  অন্যথা যদি (strcmp (param, "strict") == 0)
    প্রত্যাগম LIBCRUN_CR_CG_MODE_STRICT;
  অন্যথা
    libcrun_fail_with_error (0, "unknown cgroup mode specified");
}

স্থির error_t
parse_opt (পূর্ণাঙ্ক চাবি, বর্ণ *arg, সংরচনা argp_state *state)
{
  বিকল্প (চাবি)
    {
    পক্ষ ARGP_চাবি_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    পক্ষ OPTION_IMAGE_PATH:
      cr_options.image_path = argp_mandatory_argument (arg, state);
      ভঙ্গ;

    পক্ষ OPTION_WORK_PATH:
      cr_options.work_path = argp_mandatory_argument (arg, state);
      ভঙ্গ;

    পক্ষ OPTION_PARENT_PATH:
      cr_options.parent_path = argp_mandatory_argument (arg, state);
      ভঙ্গ;

    পক্ষ OPTION_PRE_DUMP:
      cr_options.pre_dump = true;
      ভঙ্গ;

    পক্ষ OPTION_LEAVE_RUNNING:
      cr_options.leave_running = true;
      ভঙ্গ;

    পক্ষ OPTION_TCP_ESTABLISHED:
      cr_options.tcp_established = true;
      ভঙ্গ;

    পক্ষ OPTION_EXT_UNIX_SK:
      cr_options.ext_unix_sk = true;
      ভঙ্গ;

    পক্ষ OPTION_SHELL_JOB:
      cr_options.shell_job = true;
      ভঙ্গ;

    পক্ষ OPTION_FILE_LOCKS:
      cr_options.file_locks = true;
      ভঙ্গ;

    পক্ষ OPTION_MANAGE_CGROUPS_MODE:
      cr_options.manage_cgroups_mode = crun_parse_manage_cgroups_mode (argp_mandatory_argument (arg, state));
      ভঙ্গ;

    পক্ষ OPTION_NETWORK_LOCK_METHOD:
      cr_options.network_lock_method = crun_parse_network_lock_method (argp_mandatory_argument (arg, state));
      ভঙ্গ;

    স্বভাব:
      প্রত্যাগম ARGP_ERR_UNKNOWN;
    }

  প্রত্যাগম 0;
}

স্থির সংরচনা argp run_argp = { options, parse_opt, args_doc, doc, রিক্ত, রিক্ত, রিক্ত };

পূর্ণাঙ্ক
crun_command_checkpoint (সংরচনা crun_global_arguments *global_args, পূর্ণাঙ্ক argc, বর্ণ **argv, libcrun_error_t *err)
{
  cleanup_free বর্ণ *cr_path = রিক্ত;
  পূর্ণাঙ্ক first_arg;
  পূর্ণাঙ্ক ret;
  cleanup_context libcrun_context_t *crun_context = রিক্ত;

  cr_options.struct_size = পরিমাণ (cr_options);

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &cr_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  যদি (cr_options.image_path == রিক্ত)
    {
      cleanup_free বর্ণ *path = রিক্ত;

      path = getcwd (রিক্ত, 0);
      যদি (UNLIKELY (path == রিক্ত))
        libcrun_fail_with_error (errno, "getcwd failed");

      ret = asprintf (&cr_path, "%s/checkpoint", path);
      যদি (UNLIKELY (ret < 0))
        OOM ();
      cr_options.image_path = cr_path;
    }

  প্রত্যাগম libcrun_container_checkpoint (crun_context, argv[first_arg], &cr_options, err);
}
