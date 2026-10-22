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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <argp.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <errno.h>

#અન્તર્ભાવ "crun.h"

સ્થિર અક્ષર doc[] = "OCI runtime";

ગણના
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

સંરચના ps_options_s
{
  પૂર્ણાંક format;
};

ગણના
{
  PS_TABLE = 100,
  PS_JSON,
};

સ્થિર સંરચના ps_options_s ps_options;

સ્થિર સંરચના argp_option options[] = { { "format", 'f', "FORMAT", 0, "select the output format", 0 },
                                        {
                                            0,
                                        } };

સ્થિર અક્ષર args_doc[] = "ps";

સ્થિર error_t
parse_opt (પૂર્ણાંક ચાવી, અક્ષર *arg, સંરચના argp_state *state arg_unused)
{
  પસંદગી (ચાવી)
    {
    સ્થિતિ 'f':
      જો (strcmp (arg, "table") == 0)
        ps_options.format = PS_TABLE;
      નહીંતર જો (strcmp (arg, "json") == 0)
        ps_options.format = PS_JSON;
      નહીંતર
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      તોડો;

    મૂળભૂત:
      પાછા ARGP_ERR_UNKNOWN;
    }

  પાછા 0;
}

સ્થિર સંરચના argp run_argp = { options, parse_opt, args_doc, doc, ખાલી, ખાલી, ખાલી };

પૂર્ણાંક
crun_command_ps (સંરચના crun_global_arguments *global_args, પૂર્ણાંક argc, અક્ષર **argv, libcrun_error_t *err)
{
  પૂર્ણાંક first_arg;
  પૂર્ણાંક ret;
  cleanup_free pid_t *pids = ખાલી;
  cleanup_context libcrun_context_t *crun_context = ખાલી;
  size_t i;

  ps_options.format = PS_TABLE;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &ps_options);
  crun_assert_n_args (argc - first_arg, 1, 1);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcrun_container_read_pids (crun_context, argv[first_arg], true, &pids, err);
  જો (UNLIKELY (ret < 0))
    {
      libcrun_error_report_and_release (err);
      પાછા ret;
    }

  પસંદગી (ps_options.format)
    {
    સ્થિતિ PS_JSON:
      printf ("[\n");
      માટે (i = 0; pids && pids[i]; i++)
        printf ("  %d%s\n", pids[i], pids[i + 1] ? "," : "");
      printf ("]\n");
      તોડો;

    સ્થિતિ PS_TABLE:
      printf ("PID\n");
      માટે (i = 0; pids && pids[i]; i++)
        printf ("%d\n", pids[i]);
      તોડો;
    }

  પાછા 0;
}
