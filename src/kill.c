/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019 Giuseppe Scrivano <giuseppe@scrivano.org>
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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश <argp.h>
#समावेश <string.h>
#समावेश <unistd.h>
#समावेश <errno.h>
#समावेश <signal.h>
#समावेश <regex.h>

#समावेश "crun.h"

अटल अक्षर doc[] = "OCI runtime";

क्रमागत
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

काष्ठा kill_options_s
{
  bool all;
  bool regex;
};

अटल काष्ठा kill_options_s kill_options;

अटल काष्ठा argp_option options[]
    = { { "all", 'a', 0, 0, "kill all the processes", 0 },
        { "regex", 'r', 0, 0, "the specified CONTAINER is a regular expression (kill multiple containers)", 0 },
        {
            0,
        } };

अटल अक्षर args_doc[] = "kill CONTAINER [SIGNAL]";

अटल error_t
parse_opt (पूर्णांक कुंजी, अक्षर *arg arg_unused, काष्ठा argp_state *state arg_unused)
{
  चयन (कुंजी)
    {
    हाल 'a':
      kill_options.all = true;
      अवरोध;

    हाल 'r':
      kill_options.regex = true;
      अवरोध;

    हाल ARGP_कुंजी_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    शेष:
      वापस ARGP_ERR_UNKNOWN;
    }

  वापस 0;
}

अटल काष्ठा argp run_argp = { options, parse_opt, args_doc, doc, शून्य, शून्य, शून्य };

पूर्णांक
crun_command_kill (काष्ठा crun_global_arguments *global_args, पूर्णांक argc, अक्षर **argv, libcrun_error_t *err)
{
  पूर्णांक first_arg = 0, ret;
  स्थिर अक्षर *संकेत;
  cleanup_context libcrun_context_t *crun_context = शून्य;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &kill_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  संकेत = "SIGTERM";
  अगर (argc - first_arg > 1)
    संकेत = argv[first_arg + 1];

  अगर (kill_options.regex)
    {
      regex_t re;
      libcrun_container_list_t *list;
      libcrun_container_iter_t *it;
      स्थिर अक्षर *name;

      ret = regcomp (&re, argv[first_arg], REG_EXTENDED | REG_NOSUB);
      अगर (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "invalid regular expression %s", argv[first_arg]);

      ret = libcrun_container_list (crun_context, &list, err);
      अगर (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "cannot read containers list");

      क्रम (it = libcrun_container_list_iter (list); libcrun_container_iter_अग्र (it, &name);)
        अगर (regexec (&re, name, 0, शून्य, 0) == 0)
          {
            ret = libcrun_container_kill (crun_context, name, संकेत, err);
            अगर (UNLIKELY (ret < 0))
              libcrun_error_report_and_release (err);
          }

      libcrun_container_iter_free (it);
      libcrun_container_list_free (list);
      regfree (&re);
      वापस 0;
    }

  अगर (kill_options.all)
    वापस libcrun_container_killall (crun_context, argv[first_arg], संकेत, err);

  वापस libcrun_container_kill (crun_context, argv[first_arg], संकेत, err);
}
