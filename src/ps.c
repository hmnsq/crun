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

#شامل <config.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <argp.h>
#شامل <string.h>
#شامل <unistd.h>
#شامل <errno.h>

#شامل "crun.h"

ایستا نویسه doc[] = "OCI runtime";

شمارش
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

ساختار ps_options_s
{
  صحیح format;
};

شمارش
{
  PS_TABLE = 100,
  PS_JSON,
};

ایستا ساختار ps_options_s ps_options;

ایستا ساختار argp_option options[] = { { "format", 'f', "FORMAT", 0, "select the output format", 0 },
                                        {
                                            0,
                                        } };

ایستا نویسه args_doc[] = "ps";

ایستا error_t
parse_opt (صحیح کلید, نویسه *arg, ساختار argp_state *state arg_unused)
{
  انتخاب (کلید)
    {
    حالت 'f':
      اگر (strcmp (arg, "table") == 0)
        ps_options.format = PS_TABLE;
      وگرنه اگر (strcmp (arg, "json") == 0)
        ps_options.format = PS_JSON;
      وگرنه
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      بشکن;

    پیشفرض:
      بازگشت ARGP_ERR_UNKNOWN;
    }

  بازگشت 0;
}

ایستا ساختار argp run_argp = { options, parse_opt, args_doc, doc, پوچ, پوچ, پوچ };

صحیح
crun_command_ps (ساختار crun_global_arguments *global_args, صحیح argc, نویسه **argv, libcrun_error_t *err)
{
  صحیح first_arg;
  صحیح ret;
  cleanup_free pid_t *pids = پوچ;
  cleanup_context libcrun_context_t *crun_context = پوچ;
  size_t i;

  ps_options.format = PS_TABLE;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &ps_options);
  crun_assert_n_args (argc - first_arg, 1, 1);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_container_read_pids (crun_context, argv[first_arg], true, &pids, err);
  اگر (UNLIKELY (ret < 0))
    {
      libcrun_error_report_and_release (err);
      بازگشت ret;
    }

  انتخاب (ps_options.format)
    {
    حالت PS_JSON:
      printf ("[\n");
      برای (i = 0; pids && pids[i]; i++)
        printf ("  %d%s\n", pids[i], pids[i + 1] ? "," : "");
      printf ("]\n");
      بشکن;

    حالت PS_TABLE:
      printf ("PID\n");
      برای (i = 0; pids && pids[i]; i++)
        printf ("%d\n", pids[i]);
      بشکن;
    }

  بازگشت 0;
}
