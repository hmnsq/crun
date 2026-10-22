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

स्थिर वर्ण doc[] = "OCI runtime";

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
  पूर्णाङ्क format;
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

स्थिर वर्ण args_doc[] = "ps";

स्थिर error_t
parse_opt (पूर्णाङ्क कुंजी, वर्ण *arg, संरचना argp_state *state arg_unused)
{
  विकल्प (कुंजी)
    {
    पक्ष 'f':
      यदि (म_भेद (arg, "table") == 0)
        ps_options.format = PS_TABLE;
      अन्यथा यदि (म_भेद (arg, "json") == 0)
        ps_options.format = PS_JSON;
      अन्यथा
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      भङ्ग;

    स्वभाव:
      प्रत्यागम ARGP_ERR_UNKNOWN;
    }

  प्रत्यागम 0;
}

स्थिर संरचना argp run_argp = { options, parse_opt, args_doc, doc, रिक्त, रिक्त, रिक्त };

पूर्णाङ्क
crun_command_ps (संरचना crun_global_arguments *global_args, पूर्णाङ्क argc, वर्ण **argv, libcrun_error_t *err)
{
  पूर्णाङ्क first_arg;
  पूर्णाङ्क ret;
  cleanup_free pid_t *pids = रिक्त;
  cleanup_context libcrun_context_t *crun_context = रिक्त;
  माप_प्रकार i;

  ps_options.format = PS_TABLE;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &ps_options);
  crun_assert_n_args (argc - first_arg, 1, 1);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_container_read_pids (crun_context, argv[first_arg], true, &pids, err);
  यदि (UNLIKELY (ret < 0))
    {
      libcrun_error_report_and_release (err);
      प्रत्यागम ret;
    }

  विकल्प (ps_options.format)
    {
    पक्ष PS_JSON:
      म_लिखो ("[\n");
      प्रति (i = 0; pids && pids[i]; i++)
        म_लिखो ("  %d%s\n", pids[i], pids[i + 1] ? "," : "");
      म_लिखो ("]\n");
      भङ्ग;

    पक्ष PS_TABLE:
      म_लिखो ("PID\n");
      प्रति (i = 0; pids && pids[i]; i++)
        म_लिखो ("%d\n", pids[i]);
      भङ्ग;
    }

  प्रत्यागम 0;
}
