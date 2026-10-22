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

#घोषणा _GNU_SOURCE

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश <argp.h>
#समावेश <string.h>
#समावेश <unistd.h>
#समावेश <errno.h>
#समावेश <regex.h>

#समावेश "crun.h"

क्रमागत
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

अटल अक्षर doc[] = "OCI runtime";

अटल काष्ठा libcrun_checkpoint_restore_options_s cr_options;

अटल काष्ठा argp_option options[]
    = { { "image-path", OPTION_IMAGE_PATH, "DIR", 0, "path for saving criu image files", 0 },
        { "work-path", OPTION_WORK_PATH, "DIR", 0, "path for saving work files and logs", 0 },
        { "leave-running", OPTION_LEAVE_RUNNING, 0, 0, "leave the process running after checkpointing", 0 },
        { "tcp-established", OPTION_TCP_ESTABLISHED, 0, 0, "allow open tcp connections", 0 },
        { "ext-unix-sk", OPTION_EXT_UNIX_SK, 0, 0, "allow external unix sockets", 0 },
        { "shell-job", OPTION_SHELL_JOB, 0, 0, "allow shell jobs", 0 },
        { "file-locks", OPTION_FILE_LOCKS, 0, 0, "allow file locks", 0 },
        { "network-lock", OPTION_NETWORK_LOCK_METHOD, "METHOD", 0, "set network lock method", 0 },
#अगर_घोषित CRIU_PRE_DUMP_SUPPORT
        { "parent-path", OPTION_PARENT_PATH, "DIR", 0, "path for previous criu image files in pre-dump", 0 },
        { "pre-dump", OPTION_PRE_DUMP, 0, 0, "dump container's memory information only, leave the container running after this", 0 },
#पूर्ण_अगर
        { "manage-cgroups-mode", OPTION_MANAGE_CGROUPS_MODE, "MODE", 0, "cgroups mode: 'soft' (default), 'ignore', 'full' and 'strict'", 0 },
        {
            0,
        } };

अटल अक्षर args_doc[] = "checkpoint CONTAINER";

/* Map the CLI names onto the public LIBCRUN_CR_* enums; libcrun translates
   these to CRIU's own values internally so CRIU headers never reach here.  */
पूर्णांक
crun_parse_network_lock_method (अक्षर *param)
{
  अगर (म_भेद (param, "iptables") == 0)
    वापस LIBCRUN_CR_NETWORK_LOCK_IPTABLES;
  अन्यथा अगर (म_भेद (param, "nftables") == 0)
    वापस LIBCRUN_CR_NETWORK_LOCK_NFTABLES;
  अन्यथा अगर (म_भेद (param, "skip") == 0)
    वापस LIBCRUN_CR_NETWORK_LOCK_SKIP;
  अन्यथा
    libcrun_fail_with_error (0, "unknown network lock method specified");
}

पूर्णांक
crun_parse_manage_cgroups_mode (अक्षर *param)
{
  अगर (म_भेद (param, "soft") == 0)
    वापस LIBCRUN_CR_CG_MODE_SOFT;
  अन्यथा अगर (म_भेद (param, "ignore") == 0)
    वापस LIBCRUN_CR_CG_MODE_IGNORE;
  अन्यथा अगर (म_भेद (param, "full") == 0)
    वापस LIBCRUN_CR_CG_MODE_FULL;
  अन्यथा अगर (म_भेद (param, "strict") == 0)
    वापस LIBCRUN_CR_CG_MODE_STRICT;
  अन्यथा
    libcrun_fail_with_error (0, "unknown cgroup mode specified");
}

अटल error_t
parse_opt (पूर्णांक कुंजी, अक्षर *arg, काष्ठा argp_state *state)
{
  चयन (कुंजी)
    {
    हाल ARGP_कुंजी_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    हाल OPTION_IMAGE_PATH:
      cr_options.image_path = argp_mandatory_argument (arg, state);
      अवरोध;

    हाल OPTION_WORK_PATH:
      cr_options.work_path = argp_mandatory_argument (arg, state);
      अवरोध;

    हाल OPTION_PARENT_PATH:
      cr_options.parent_path = argp_mandatory_argument (arg, state);
      अवरोध;

    हाल OPTION_PRE_DUMP:
      cr_options.pre_dump = true;
      अवरोध;

    हाल OPTION_LEAVE_RUNNING:
      cr_options.leave_running = true;
      अवरोध;

    हाल OPTION_TCP_ESTABLISHED:
      cr_options.tcp_established = true;
      अवरोध;

    हाल OPTION_EXT_UNIX_SK:
      cr_options.ext_unix_sk = true;
      अवरोध;

    हाल OPTION_SHELL_JOB:
      cr_options.shell_job = true;
      अवरोध;

    हाल OPTION_FILE_LOCKS:
      cr_options.file_locks = true;
      अवरोध;

    हाल OPTION_MANAGE_CGROUPS_MODE:
      cr_options.manage_cgroups_mode = crun_parse_manage_cgroups_mode (argp_mandatory_argument (arg, state));
      अवरोध;

    हाल OPTION_NETWORK_LOCK_METHOD:
      cr_options.network_lock_method = crun_parse_network_lock_method (argp_mandatory_argument (arg, state));
      अवरोध;

    शेष:
      वापस ARGP_ERR_UNKNOWN;
    }

  वापस 0;
}

अटल काष्ठा argp run_argp = { options, parse_opt, args_doc, doc, शून्य, शून्य, शून्य };

पूर्णांक
crun_command_checkpoint (काष्ठा crun_global_arguments *global_args, पूर्णांक argc, अक्षर **argv, libcrun_error_t *err)
{
  cleanup_free अक्षर *cr_path = शून्य;
  पूर्णांक first_arg;
  पूर्णांक ret;
  cleanup_context libcrun_context_t *crun_context = शून्य;

  cr_options.struct_आकार = माप (cr_options);

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &cr_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (cr_options.image_path == शून्य)
    {
      cleanup_free अक्षर *path = शून्य;

      path = getcwd (शून्य, 0);
      अगर (UNLIKELY (path == शून्य))
        libcrun_fail_with_error (त्रुटि_सं, "getcwd failed");

      ret = asprintf (&cr_path, "%s/checkpoint", path);
      अगर (UNLIKELY (ret < 0))
        OOM ();
      cr_options.image_path = cr_path;
    }

  वापस libcrun_container_checkpoint (crun_context, argv[first_arg], &cr_options, err);
}
