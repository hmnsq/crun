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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <argp.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <regex.h>

#ಅನ್ತರ್ಭಾವ "crun.h"

ಗಣನಾ
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

ಸ್ಥಿರ ವರ್ಣ doc[] = "OCI runtime";

ಸ್ಥಿರ ಸಂರಚನಾ libcrun_checkpoint_restore_options_s cr_options;

ಸ್ಥಿರ ಸಂರಚನಾ argp_option options[]
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

ಸ್ಥಿರ ವರ್ಣ args_doc[] = "checkpoint CONTAINER";

/* Map the CLI names onto the public LIBCRUN_CR_* enums; libcrun translates
   these to CRIU's own values internally so CRIU headers never reach here.  */
ಪೂರ್ಣಾಙ್ಕ
crun_parse_network_lock_method (ವರ್ಣ *param)
{
  ಯದಿ (strcmp (param, "iptables") == 0)
    ಪ್ರತ್ಯಾಗಮ LIBCRUN_CR_NETWORK_LOCK_IPTABLES;
  ಅನ್ಯಥಾ ಯದಿ (strcmp (param, "nftables") == 0)
    ಪ್ರತ್ಯಾಗಮ LIBCRUN_CR_NETWORK_LOCK_NFTABLES;
  ಅನ್ಯಥಾ ಯದಿ (strcmp (param, "skip") == 0)
    ಪ್ರತ್ಯಾಗಮ LIBCRUN_CR_NETWORK_LOCK_SKIP;
  ಅನ್ಯಥಾ
    libcrun_fail_with_error (0, "unknown network lock method specified");
}

ಪೂರ್ಣಾಙ್ಕ
crun_parse_manage_cgroups_mode (ವರ್ಣ *param)
{
  ಯದಿ (strcmp (param, "soft") == 0)
    ಪ್ರತ್ಯಾಗಮ LIBCRUN_CR_CG_MODE_SOFT;
  ಅನ್ಯಥಾ ಯದಿ (strcmp (param, "ignore") == 0)
    ಪ್ರತ್ಯಾಗಮ LIBCRUN_CR_CG_MODE_IGNORE;
  ಅನ್ಯಥಾ ಯದಿ (strcmp (param, "full") == 0)
    ಪ್ರತ್ಯಾಗಮ LIBCRUN_CR_CG_MODE_FULL;
  ಅನ್ಯಥಾ ಯದಿ (strcmp (param, "strict") == 0)
    ಪ್ರತ್ಯಾಗಮ LIBCRUN_CR_CG_MODE_STRICT;
  ಅನ್ಯಥಾ
    libcrun_fail_with_error (0, "unknown cgroup mode specified");
}

ಸ್ಥಿರ error_t
parse_opt (ಪೂರ್ಣಾಙ್ಕ ಕೀಲಿ, ವರ್ಣ *arg, ಸಂರಚನಾ argp_state *state)
{
  ವಿಕಲ್ಪ (ಕೀಲಿ)
    {
    ಪಕ್ಷ ARGP_ಕೀಲಿ_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    ಪಕ್ಷ OPTION_IMAGE_PATH:
      cr_options.image_path = argp_mandatory_argument (arg, state);
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_WORK_PATH:
      cr_options.work_path = argp_mandatory_argument (arg, state);
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_PARENT_PATH:
      cr_options.parent_path = argp_mandatory_argument (arg, state);
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_PRE_DUMP:
      cr_options.pre_dump = true;
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_LEAVE_RUNNING:
      cr_options.leave_running = true;
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_TCP_ESTABLISHED:
      cr_options.tcp_established = true;
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_EXT_UNIX_SK:
      cr_options.ext_unix_sk = true;
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_SHELL_JOB:
      cr_options.shell_job = true;
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_FILE_LOCKS:
      cr_options.file_locks = true;
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_MANAGE_CGROUPS_MODE:
      cr_options.manage_cgroups_mode = crun_parse_manage_cgroups_mode (argp_mandatory_argument (arg, state));
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_NETWORK_LOCK_METHOD:
      cr_options.network_lock_method = crun_parse_network_lock_method (argp_mandatory_argument (arg, state));
      ಭಙ್ಗ;

    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ ARGP_ERR_UNKNOWN;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಸಂರಚನಾ argp run_argp = { options, parse_opt, args_doc, doc, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ };

ಪೂರ್ಣಾಙ್ಕ
crun_command_checkpoint (ಸಂರಚನಾ crun_global_arguments *global_args, ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *cr_path = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ first_arg;
  ಪೂರ್ಣಾಙ್ಕ ret;
  cleanup_context libcrun_context_t *crun_context = ರಿಕ್ತ;

  cr_options.struct_size = ಪರಿಮಾಣ (cr_options);

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &cr_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (cr_options.image_path == ರಿಕ್ತ)
    {
      cleanup_free ವರ್ಣ *path = ರಿಕ್ತ;

      path = getcwd (ರಿಕ್ತ, 0);
      ಯದಿ (UNLIKELY (path == ರಿಕ್ತ))
        libcrun_fail_with_error (errno, "getcwd failed");

      ret = asprintf (&cr_path, "%s/checkpoint", path);
      ಯದಿ (UNLIKELY (ret < 0))
        OOM ();
      cr_options.image_path = cr_path;
    }

  ಪ್ರತ್ಯಾಗಮ libcrun_container_checkpoint (crun_context, argv[first_arg], &cr_options, err);
}
