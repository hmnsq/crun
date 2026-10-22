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

સ્થિર સંરચના argp_option options[] = { {
    0,
} };

સ્થિર અક્ષર args_doc[] = "start CONTAINER";

સ્થિર error_t
parse_opt (પૂર્ણાંક ચાવી, અક્ષર *arg arg_unused, સંરચના argp_state *state arg_unused)
{
  પસંદગી (ચાવી)
    {
    સ્થિતિ ARGP_ચાવી_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    મૂળભૂત:
      પાછા ARGP_ERR_UNKNOWN;
    }

  પાછા 0;
}

સ્થિર સંરચના argp run_argp = { options, parse_opt, args_doc, doc, ખાલી, ખાલી, ખાલી };

પૂર્ણાંક
crun_command_start (સંરચના crun_global_arguments *global_args, પૂર્ણાંક argc, અક્ષર **argv, libcrun_error_t *err)
{
  પૂર્ણાંક first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = ખાલી;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, ખાલી);
  crun_assert_n_args (argc - first_arg, 1, 1);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા libcrun_container_start (crun_context, argv[first_arg], err);
}
