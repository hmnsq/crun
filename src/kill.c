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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <argp.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <signal.h>
#अन्तर्भाव <regex.h>

#अन्तर्भाव "crun.h"

स्थिर वर्ण doc[] = "OCI runtime";

गणना
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

संरचना kill_options_s
{
  bool all;
  bool regex;
};

स्थिर संरचना kill_options_s kill_options;

स्थिर संरचना argp_option options[]
    = { { "all", 'a', 0, 0, "kill all the processes", 0 },
        { "regex", 'r', 0, 0, "the specified CONTAINER is a regular expression (kill multiple containers)", 0 },
        {
            0,
        } };

स्थिर वर्ण args_doc[] = "kill CONTAINER [SIGNAL]";

स्थिर error_t
parse_opt (पूर्णाङ्क कुञ्चिका, वर्ण *arg arg_unused, संरचना argp_state *state arg_unused)
{
  विकल्प (कुञ्चिका)
    {
    पक्ष 'a':
      kill_options.all = true;
      भङ्ग;

    पक्ष 'r':
      kill_options.regex = true;
      भङ्ग;

    पक्ष ARGP_कुञ्चिका_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    स्वभाव:
      पटिनिवत्त ARGP_ERR_UNKNOWN;
    }

  पटिनिवत्त 0;
}

स्थिर संरचना argp run_argp = { options, parse_opt, args_doc, doc, रिक्त, रिक्त, रिक्त };

पूर्णाङ्क
crun_command_kill (संरचना crun_global_arguments *global_args, पूर्णाङ्क argc, वर्ण **argv, libcrun_error_t *err)
{
  पूर्णाङ्क first_arg = 0, ret;
  ध्रुव वर्ण *सङ्केत;
  cleanup_context libcrun_context_t *crun_context = रिक्त;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &kill_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सङ्केत = "SIGTERM";
  सचे (argc - first_arg > 1)
    सङ्केत = argv[first_arg + 1];

  सचे (kill_options.regex)
    {
      regex_t re;
      libcrun_container_list_t *list;
      libcrun_container_iter_t *it;
      ध्रुव वर्ण *name;

      ret = regcomp (&re, argv[first_arg], REG_EXTENDED | REG_NOSUB);
      सचे (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "invalid regular expression %s", argv[first_arg]);

      ret = libcrun_container_list (crun_context, &list, err);
      सचे (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "cannot read containers list");

      अत्थाय (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
        सचे (regexec (&re, name, 0, रिक्त, 0) == 0)
          {
            ret = libcrun_container_kill (crun_context, name, सङ्केत, err);
            सचे (UNLIKELY (ret < 0))
              libcrun_error_report_and_release (err);
          }

      libcrun_container_iter_free (it);
      libcrun_container_list_free (list);
      regfree (&re);
      पटिनिवत्त 0;
    }

  सचे (kill_options.all)
    पटिनिवत्त libcrun_container_killall (crun_context, argv[first_arg], सङ्केत, err);

  पटिनिवत्त libcrun_container_kill (crun_context, argv[first_arg], सङ्केत, err);
}
