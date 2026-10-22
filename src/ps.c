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

काष्ठा ps_options_s
{
  पूर्णांक format;
};

क्रमागत
{
  PS_TABLE = 100,
  PS_JSON,
};

अटल काष्ठा ps_options_s ps_options;

अटल काष्ठा argp_option options[] = { { "format", 'f', "FORMAT", 0, "select the output format", 0 },
                                        {
                                            0,
                                        } };

अटल अक्षर args_doc[] = "ps";

अटल error_t
parse_opt (पूर्णांक कुंजी, अक्षर *arg, काष्ठा argp_state *state arg_unused)
{
  चयन (कुंजी)
    {
    हाल 'f':
      अगर (म_भेद (arg, "table") == 0)
        ps_options.format = PS_TABLE;
      अन्यथा अगर (म_भेद (arg, "json") == 0)
        ps_options.format = PS_JSON;
      अन्यथा
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      अवरोध;

    शेष:
      वापस ARGP_ERR_UNKNOWN;
    }

  वापस 0;
}

अटल काष्ठा argp run_argp = { options, parse_opt, args_doc, doc, शून्य, शून्य, शून्य };

पूर्णांक
crun_command_ps (काष्ठा crun_global_arguments *global_args, पूर्णांक argc, अक्षर **argv, libcrun_error_t *err)
{
  पूर्णांक first_arg;
  पूर्णांक ret;
  cleanup_free pid_t *pids = शून्य;
  cleanup_context libcrun_context_t *crun_context = शून्य;
  माप_प्रकार i;

  ps_options.format = PS_TABLE;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &ps_options);
  crun_assert_n_args (argc - first_arg, 1, 1);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcrun_container_read_pids (crun_context, argv[first_arg], true, &pids, err);
  अगर (UNLIKELY (ret < 0))
    {
      libcrun_error_report_and_release (err);
      वापस ret;
    }

  चयन (ps_options.format)
    {
    हाल PS_JSON:
      म_लिखो ("[\n");
      क्रम (i = 0; pids && pids[i]; i++)
        म_लिखो ("  %d%s\n", pids[i], pids[i + 1] ? "," : "");
      म_लिखो ("]\n");
      अवरोध;

    हाल PS_TABLE:
      म_लिखो ("PID\n");
      क्रम (i = 0; pids && pids[i]; i++)
        म_लिखो ("%d\n", pids[i]);
      अवरोध;
    }

  वापस 0;
}
