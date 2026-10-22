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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <argp.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <errno.h>

#അന്തര്ഭാവ "crun.h"

സ്ഥിതം അക്ഷരം doc[] = "OCI runtime";

എണ്ണൽ
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

ഘടന ps_options_s
{
  പൂർണ്ണസംഖ്യ format;
};

എണ്ണൽ
{
  PS_TABLE = 100,
  PS_JSON,
};

സ്ഥിതം ഘടന ps_options_s ps_options;

സ്ഥിതം ഘടന argp_option options[] = { { "format", 'f', "FORMAT", 0, "select the output format", 0 },
                                        {
                                            0,
                                        } };

സ്ഥിതം അക്ഷരം args_doc[] = "ps";

സ്ഥിതം error_t
parse_opt (പൂർണ്ണസംഖ്യ താക്കോൽ, അക്ഷരം *arg, ഘടന argp_state *state arg_unused)
{
  തിരഞ്ഞെടുപ്പ് (താക്കോൽ)
    {
    അവസ്ഥ 'f':
      എങ്കിൽ (strcmp (arg, "table") == 0)
        ps_options.format = PS_TABLE;
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (arg, "json") == 0)
        ps_options.format = PS_JSON;
      അല്ലെങ്കിൽ
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
      തിരികെ ARGP_ERR_UNKNOWN;
    }

  തിരികെ 0;
}

സ്ഥിതം ഘടന argp run_argp = { options, parse_opt, args_doc, doc, ശൂന്യ, ശൂന്യ, ശൂന്യ };

പൂർണ്ണസംഖ്യ
crun_command_ps (ഘടന crun_global_arguments *global_args, പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ first_arg;
  പൂർണ്ണസംഖ്യ ret;
  cleanup_free pid_t *pids = ശൂന്യ;
  cleanup_context libcrun_context_t *crun_context = ശൂന്യ;
  size_t i;

  ps_options.format = PS_TABLE;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &ps_options);
  crun_assert_n_args (argc - first_arg, 1, 1);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_container_read_pids (crun_context, argv[first_arg], true, &pids, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      libcrun_error_report_and_release (err);
      തിരികെ ret;
    }

  തിരഞ്ഞെടുപ്പ് (ps_options.format)
    {
    അവസ്ഥ PS_JSON:
      printf ("[\n");
      വേണ്ടി (i = 0; pids && pids[i]; i++)
        printf ("  %d%s\n", pids[i], pids[i + 1] ? "," : "");
      printf ("]\n");
      നിർത്തുക;

    അവസ്ഥ PS_TABLE:
      printf ("PID\n");
      വേണ്ടി (i = 0; pids && pids[i]; i++)
        printf ("%d\n", pids[i]);
      നിർത്തുക;
    }

  തിരികെ 0;
}
