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

ساکن حرف doc[] = "OCI runtime";

شمار
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

ساخت ps_options_s
{
  صحیح format;
};

شمار
{
  PS_جدول = 100,
  PS_JSON,
};

ساکن ساخت ps_options_s ps_options;

ساکن ساخت argp_option options[] = { { "format", 'f', "FORMAT", 0, "select the output format", 0 },
                                        {
                                            0,
                                        } };

ساکن حرف دلائل_doc[] = "ps";

ساکن error_t
parse_opt (صحیح کلید, حرف *arg, ساخت argp_state *state arg_غیر_مستعمل)
{
  انتخاب (کلید)
    {
    حالت 'f':
      اگر (strcmp (arg, "table") == 0)
        ps_options.format = PS_جدول;
      ورنہ اگر (strcmp (arg, "json") == 0)
        ps_options.format = PS_JSON;
      ورنہ
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      توڑو;

    طےشدہ:
      واپس ARGP_ERR_UNKNOWN;
    }

  واپس 0;
}

ساکن ساخت argp تسلسل_argp = { options, parse_opt, دلائل_doc, doc, تہی, تہی, تہی };

صحیح
crun_command_ps (ساخت crun_global_arguments *global_دلائل, صحیح argc, حرف **argv, libcrun_error_t *err)
{
  صحیح پہلا_arg;
  صحیح ret;
  cleanup_free pid_t *pids = تہی;
  cleanup_context libcrun_context_t *crun_context = تہی;
  size_t i;

  ps_options.format = PS_جدول;

  argp_parse (&تسلسل_argp, argc, argv, ARGP_اندر_ترتیب, &پہلا_arg, &ps_options);
  crun_assert_n_دلائل (argc - پہلا_arg, 1, 1);

  crun_context = new_libcrun_context (global_دلائل);

  ret = init_libcrun_context (crun_context, argv[پہلا_arg], global_دلائل, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcrun_container_read_pids (crun_context, argv[پہلا_arg], true, &pids, err);
  اگر (UNLIKELY (ret < 0))
    {
      libcrun_error_report_and_release (err);
      واپس ret;
    }

  انتخاب (ps_options.format)
    {
    حالت PS_JSON:
      لکھو ("[\n");
      برائے (i = 0; pids && pids[i]; i++)
        لکھو ("  %d%s\n", pids[i], pids[i + 1] ? "," : "");
      لکھو ("]\n");
      توڑو;

    حالت PS_جدول:
      لکھو ("PID\n");
      برائے (i = 0; pids && pids[i]; i++)
        لکھو ("%d\n", pids[i]);
      توڑو;
    }

  واپس 0;
}
