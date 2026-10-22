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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <argp.h>
#ضمن <string.h>
#ضمن <unistd.h>
#ضمن <errno.h>

#ضمن "crun.h"

ساكن حرف doc[] = "OCI runtime";

تعداد
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

بنية ps_options_s
{
  صحيح format;
};

تعداد
{
  PS_جدول = 100,
  PS_JSON,
};

ساكن بنية ps_options_s ps_options;

ساكن بنية argp_option options[] = { { "format", 'f', "FORMAT", 0, "select the output format", 0 },
                                        {
                                            0,
                                        } };

ساكن حرف دليلون_doc[] = "ps";

ساكن error_t
parse_opt (صحيح ڪنجي, حرف *arg, بنية argp_state *state arg_اڻ_استعمال_ٿيل)
{
  بدل (ڪنجي)
    {
    حالة 'f':
      إذا (strcmp (arg, "table") == 0)
        ps_options.format = PS_جدول;
      وإلا إذا (strcmp (arg, "json") == 0)
        ps_options.format = PS_JSON;
      وإلا
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      اكسر;

    افتراضي:
      أرجع ARGP_ERR_UNKNOWN;
    }

  أرجع 0;
}

ساكن بنية argp لاڳيتو_argp = { options, parse_opt, دليلون_doc, doc, عدم, عدم, عدم };

صحيح
crun_command_ps (بنية crun_global_arguments *global_دليلون, صحيح argc, حرف **argv, libcrun_error_t *err)
{
  صحيح پهريون_arg;
  صحيح ret;
  cleanup_free pid_t *pids = عدم;
  cleanup_context libcrun_context_t *crun_context = عدم;
  size_t i;

  ps_options.format = PS_جدول;

  argp_parse (&لاڳيتو_argp, argc, argv, ARGP_اندريون_ترتيب, &پهريون_arg, &ps_options);
  crun_assert_n_دليلون (argc - پهريون_arg, 1, 1);

  crun_context = new_libcrun_context (global_دليلون);

  ret = init_libcrun_context (crun_context, argv[پهريون_arg], global_دليلون, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_container_read_pids (crun_context, argv[پهريون_arg], true, &pids, err);
  إذا (UNLIKELY (ret < 0))
    {
      libcrun_error_report_and_release (err);
      أرجع ret;
    }

  بدل (ps_options.format)
    {
    حالة PS_JSON:
      لکو ("[\n");
      لكل (i = 0; pids && pids[i]; i++)
        لکو ("  %d%s\n", pids[i], pids[i + 1] ? "," : "");
      لکو ("]\n");
      اكسر;

    حالة PS_جدول:
      لکو ("PID\n");
      لكل (i = 0; pids && pids[i]; i++)
        لکو ("%d\n", pids[i]);
      اكسر;
    }

  أرجع 0;
}
