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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <argp.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <errno.h>

#অন্তর্ভাব "crun.h"

স্থির অক্ষর doc[] = "OCI runtime";

গণনা
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

কাঠামো ps_options_s
{
  পূর্ণসংখ্যা format;
};

গণনা
{
  PS_TABLE = 100,
  PS_JSON,
};

স্থির কাঠামো ps_options_s ps_options;

স্থির কাঠামো argp_option options[] = { { "format", 'f', "FORMAT", 0, "select the output format", 0 },
                                        {
                                            0,
                                        } };

স্থির অক্ষর args_doc[] = "ps";

স্থির error_t
parse_opt (পূর্ণসংখ্যা চাবি, অক্ষর *arg, কাঠামো argp_state *state arg_unused)
{
  নির্বাচন (চাবি)
    {
    ক্ষেত্র 'f':
      যদি (strcmp (arg, "table") == 0)
        ps_options.format = PS_TABLE;
      অন্যথা যদি (strcmp (arg, "json") == 0)
        ps_options.format = PS_JSON;
      অন্যথা
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      বিরতি;

    পূর্বনির্ধারিত:
      ফেরত ARGP_ERR_UNKNOWN;
    }

  ফেরত 0;
}

স্থির কাঠামো argp run_argp = { options, parse_opt, args_doc, doc, ফাঁকা, ফাঁকা, ফাঁকা };

পূর্ণসংখ্যা
crun_command_ps (কাঠামো crun_global_arguments *global_args, পূর্ণসংখ্যা argc, অক্ষর **argv, libcrun_error_t *err)
{
  পূর্ণসংখ্যা first_arg;
  পূর্ণসংখ্যা ret;
  cleanup_free pid_t *pids = ফাঁকা;
  cleanup_context libcrun_context_t *crun_context = ফাঁকা;
  size_t i;

  ps_options.format = PS_TABLE;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &ps_options);
  crun_assert_n_args (argc - first_arg, 1, 1);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = libcrun_container_read_pids (crun_context, argv[first_arg], true, &pids, err);
  যদি (UNLIKELY (ret < 0))
    {
      libcrun_error_report_and_release (err);
      ফেরত ret;
    }

  নির্বাচন (ps_options.format)
    {
    ক্ষেত্র PS_JSON:
      printf ("[\n");
      জন্য (i = 0; pids && pids[i]; i++)
        printf ("  %d%s\n", pids[i], pids[i + 1] ? "," : "");
      printf ("]\n");
      বিরতি;

    ক্ষেত্র PS_TABLE:
      printf ("PID\n");
      জন্য (i = 0; pids && pids[i]; i++)
        printf ("%d\n", pids[i]);
      বিরতি;
    }

  ফেরত 0;
}
