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

#شامل <config.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <argp.h>
#شامل <string.h>
#شامل <unistd.h>
#شامل <errno.h>
#شامل <regex.h>

#شامل "crun.h"

شمارش
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

ایستا نویسه doc[] = "OCI runtime";

ایستا ساختار libcrun_checkpoint_restore_options_s cr_options;

ایستا ساختار argp_option options[]
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

ایستا نویسه args_doc[] = "checkpoint CONTAINER";

/* Map the CLI names onto the public LIBCRUN_CR_* enums; libcrun translates
   these to CRIU's own values internally so CRIU headers never reach here.  */
صحیح
crun_parse_network_lock_method (نویسه *param)
{
  اگر (strcmp (param, "iptables") == 0)
    بازگشت LIBCRUN_CR_NETWORK_LOCK_IPTABLES;
  وگرنه اگر (strcmp (param, "nftables") == 0)
    بازگشت LIBCRUN_CR_NETWORK_LOCK_NFTABLES;
  وگرنه اگر (strcmp (param, "skip") == 0)
    بازگشت LIBCRUN_CR_NETWORK_LOCK_SKIP;
  وگرنه
    libcrun_fail_with_error (0, "unknown network lock method specified");
}

صحیح
crun_parse_manage_cgroups_mode (نویسه *param)
{
  اگر (strcmp (param, "soft") == 0)
    بازگشت LIBCRUN_CR_CG_MODE_SOFT;
  وگرنه اگر (strcmp (param, "ignore") == 0)
    بازگشت LIBCRUN_CR_CG_MODE_IGNORE;
  وگرنه اگر (strcmp (param, "full") == 0)
    بازگشت LIBCRUN_CR_CG_MODE_FULL;
  وگرنه اگر (strcmp (param, "strict") == 0)
    بازگشت LIBCRUN_CR_CG_MODE_STRICT;
  وگرنه
    libcrun_fail_with_error (0, "unknown cgroup mode specified");
}

ایستا error_t
parse_opt (صحیح کلید, نویسه *arg, ساختار argp_state *state)
{
  انتخاب (کلید)
    {
    حالت ARGP_کلید_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    حالت OPTION_IMAGE_PATH:
      cr_options.image_path = argp_mandatory_argument (arg, state);
      بشکن;

    حالت OPTION_WORK_PATH:
      cr_options.work_path = argp_mandatory_argument (arg, state);
      بشکن;

    حالت OPTION_PARENT_PATH:
      cr_options.parent_path = argp_mandatory_argument (arg, state);
      بشکن;

    حالت OPTION_PRE_DUMP:
      cr_options.pre_dump = true;
      بشکن;

    حالت OPTION_LEAVE_RUNNING:
      cr_options.leave_running = true;
      بشکن;

    حالت OPTION_TCP_ESTABLISHED:
      cr_options.tcp_established = true;
      بشکن;

    حالت OPTION_EXT_UNIX_SK:
      cr_options.ext_unix_sk = true;
      بشکن;

    حالت OPTION_SHELL_JOB:
      cr_options.shell_job = true;
      بشکن;

    حالت OPTION_FILE_LOCKS:
      cr_options.file_locks = true;
      بشکن;

    حالت OPTION_MANAGE_CGROUPS_MODE:
      cr_options.manage_cgroups_mode = crun_parse_manage_cgroups_mode (argp_mandatory_argument (arg, state));
      بشکن;

    حالت OPTION_NETWORK_LOCK_METHOD:
      cr_options.network_lock_method = crun_parse_network_lock_method (argp_mandatory_argument (arg, state));
      بشکن;

    پیشفرض:
      بازگشت ARGP_ERR_UNKNOWN;
    }

  بازگشت 0;
}

ایستا ساختار argp run_argp = { options, parse_opt, args_doc, doc, پوچ, پوچ, پوچ };

صحیح
crun_command_checkpoint (ساختار crun_global_arguments *global_args, صحیح argc, نویسه **argv, libcrun_error_t *err)
{
  cleanup_free نویسه *cr_path = پوچ;
  صحیح first_arg;
  صحیح ret;
  cleanup_context libcrun_context_t *crun_context = پوچ;

  cr_options.struct_size = اندازه (cr_options);

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &cr_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (cr_options.image_path == پوچ)
    {
      cleanup_free نویسه *path = پوچ;

      path = getcwd (پوچ, 0);
      اگر (UNLIKELY (path == پوچ))
        libcrun_fail_with_error (errno, "getcwd failed");

      ret = asprintf (&cr_path, "%s/checkpoint", path);
      اگر (UNLIKELY (ret < 0))
        OOM ();
      cr_options.image_path = cr_path;
    }

  بازگشت libcrun_container_checkpoint (crun_context, argv[first_arg], &cr_options, err);
}
