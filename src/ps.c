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

#अन्तर्भाव "crun.h"

स्थिर अक्षर doc[] = "OCI runtime";

गणना
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

संरचना ps_options_s
{
  पूर्णांक format;
};

गणना
{
  PS_TABLE = 100,
  PS_JSON,
};

स्थिर संरचना ps_options_s ps_options;

स्थिर संरचना argp_option options[] = { { "format", 'f', "FORMAT", 0, "select the output format", 0 },
                                        {
                                            0,
                                        } };

स्थिर अक्षर args_doc[] = "ps";

स्थिर error_t
parse_opt (पूर्णांक किल्ली, अक्षर *arg, संरचना argp_state *state arg_unused)
{
  निवड (किल्ली)
    {
    स्थिती 'f':
      जर (म_भेद (arg, "table") == 0)
        ps_options.format = PS_TABLE;
      अन्यथा जर (म_भेद (arg, "json") == 0)
        ps_options.format = PS_JSON;
      अन्यथा
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      थांबा;

    पूर्वनिर्धारित:
      परत ARGP_ERR_UNKNOWN;
    }

  परत 0;
}

स्थिर संरचना argp run_argp = { options, parse_opt, args_doc, doc, रिक्त, रिक्त, रिक्त };

पूर्णांक
crun_command_ps (संरचना crun_global_arguments *global_args, पूर्णांक argc, अक्षर **argv, libcrun_error_t *err)
{
  पूर्णांक first_arg;
  पूर्णांक ret;
  cleanup_free pid_t *pids = रिक्त;
  cleanup_context libcrun_context_t *crun_context = रिक्त;
  माप_प्रकार i;

  ps_options.format = PS_TABLE;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &ps_options);
  crun_assert_n_args (argc - first_arg, 1, 1);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = libcrun_container_read_pids (crun_context, argv[first_arg], true, &pids, err);
  जर (UNLIKELY (ret < 0))
    {
      libcrun_error_report_and_release (err);
      परत ret;
    }

  निवड (ps_options.format)
    {
    स्थिती PS_JSON:
      म_लिखो ("[\n");
      साठी (i = 0; pids && pids[i]; i++)
        म_लिखो ("  %d%s\n", pids[i], pids[i + 1] ? "," : "");
      म_लिखो ("]\n");
      थांबा;

    स्थिती PS_TABLE:
      म_लिखो ("PID\n");
      साठी (i = 0; pids && pids[i]; i++)
        म_लिखो ("%d\n", pids[i]);
      थांबा;
    }

  परत 0;
}
