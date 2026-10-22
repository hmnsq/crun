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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <argp.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <regex.h>

#अन्तर्भाव "crun.h"

गणना
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

स्थिर अक्षर doc[] = "OCI runtime";

स्थिर संरचना libcrun_checkpoint_restore_options_s cr_options;

स्थिर संरचना argp_option options[]
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

स्थिर अक्षर args_doc[] = "checkpoint CONTAINER";

/* Map the CLI names onto the public LIBCRUN_CR_* enums; libcrun translates
   these to CRIU's own values internally so CRIU headers never reach here.  */
पूर्णांक
crun_parse_network_lock_method (अक्षर *param)
{
  जर (म_भेद (param, "iptables") == 0)
    परत LIBCRUN_CR_NETWORK_LOCK_IPTABLES;
  अन्यथा जर (म_भेद (param, "nftables") == 0)
    परत LIBCRUN_CR_NETWORK_LOCK_NFTABLES;
  अन्यथा जर (म_भेद (param, "skip") == 0)
    परत LIBCRUN_CR_NETWORK_LOCK_SKIP;
  अन्यथा
    libcrun_fail_with_error (0, "unknown network lock method specified");
}

पूर्णांक
crun_parse_manage_cgroups_mode (अक्षर *param)
{
  जर (म_भेद (param, "soft") == 0)
    परत LIBCRUN_CR_CG_MODE_SOFT;
  अन्यथा जर (म_भेद (param, "ignore") == 0)
    परत LIBCRUN_CR_CG_MODE_IGNORE;
  अन्यथा जर (म_भेद (param, "full") == 0)
    परत LIBCRUN_CR_CG_MODE_FULL;
  अन्यथा जर (म_भेद (param, "strict") == 0)
    परत LIBCRUN_CR_CG_MODE_STRICT;
  अन्यथा
    libcrun_fail_with_error (0, "unknown cgroup mode specified");
}

स्थिर error_t
parse_opt (पूर्णांक किल्ली, अक्षर *arg, संरचना argp_state *state)
{
  निवड (किल्ली)
    {
    स्थिती ARGP_किल्ली_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    स्थिती OPTION_IMAGE_PATH:
      cr_options.image_path = argp_mandatory_argument (arg, state);
      थांबा;

    स्थिती OPTION_WORK_PATH:
      cr_options.work_path = argp_mandatory_argument (arg, state);
      थांबा;

    स्थिती OPTION_PARENT_PATH:
      cr_options.parent_path = argp_mandatory_argument (arg, state);
      थांबा;

    स्थिती OPTION_PRE_DUMP:
      cr_options.pre_dump = true;
      थांबा;

    स्थिती OPTION_LEAVE_RUNNING:
      cr_options.leave_running = true;
      थांबा;

    स्थिती OPTION_TCP_ESTABLISHED:
      cr_options.tcp_established = true;
      थांबा;

    स्थिती OPTION_EXT_UNIX_SK:
      cr_options.ext_unix_sk = true;
      थांबा;

    स्थिती OPTION_SHELL_JOB:
      cr_options.shell_job = true;
      थांबा;

    स्थिती OPTION_FILE_LOCKS:
      cr_options.file_locks = true;
      थांबा;

    स्थिती OPTION_MANAGE_CGROUPS_MODE:
      cr_options.manage_cgroups_mode = crun_parse_manage_cgroups_mode (argp_mandatory_argument (arg, state));
      थांबा;

    स्थिती OPTION_NETWORK_LOCK_METHOD:
      cr_options.network_lock_method = crun_parse_network_lock_method (argp_mandatory_argument (arg, state));
      थांबा;

    पूर्वनिर्धारित:
      परत ARGP_ERR_UNKNOWN;
    }

  परत 0;
}

स्थिर संरचना argp run_argp = { options, parse_opt, args_doc, doc, रिक्त, रिक्त, रिक्त };

पूर्णांक
crun_command_checkpoint (संरचना crun_global_arguments *global_args, पूर्णांक argc, अक्षर **argv, libcrun_error_t *err)
{
  cleanup_free अक्षर *cr_path = रिक्त;
  पूर्णांक first_arg;
  पूर्णांक ret;
  cleanup_context libcrun_context_t *crun_context = रिक्त;

  cr_options.struct_size = आकार (cr_options);

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &cr_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (cr_options.image_path == रिक्त)
    {
      cleanup_free अक्षर *path = रिक्त;

      path = getcwd (रिक्त, 0);
      जर (UNLIKELY (path == रिक्त))
        libcrun_fail_with_error (त्रुटि_सं, "getcwd failed");

      ret = asprintf (&cr_path, "%s/checkpoint", path);
      जर (UNLIKELY (ret < 0))
        OOM ();
      cr_options.image_path = cr_path;
    }

  परत libcrun_container_checkpoint (crun_context, argv[first_arg], &cr_options, err);
}
