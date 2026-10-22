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

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "crun.h"

static var_na doc[] = "OCI runtime";

ga_nanaa
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

sa_nracanaa ps_options_s
{
  පූර්ණ_සංඛ්‍යා format;
};

ga_nanaa
{
  PS_TABLE = 100,
  PS_JSON,
};

static sa_nracanaa ps_options_s ps_options;

static sa_nracanaa argp_option options[] = { { "format", 'f', "FORMAT", 0, "select the output format", 0 },
                                        {
                                            0,
                                        } };

static var_na args_doc[] = "ps";

static error_t
parse_opt (පූර්ණ_සංඛ්‍යා key, var_na *arg, sa_nracanaa argp_state *state arg_unused)
{
  switch (key)
    {
    pak_sa 'f':
      නම් (strcmp (arg, "table") == 0)
        ps_options.format = PS_TABLE;
      නැතහොත් නම් (strcmp (arg, "json") == 0)
        ps_options.format = PS_JSON;
      නැතහොත්
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      bha_nga;

    default:
      ආපසු ARGP_ERR_UNKNOWN;
    }

  ආපසු 0;
}

static sa_nracanaa argp run_argp = { options, parse_opt, args_doc, doc, NULL, NULL, NULL };

පූර්ණ_සංඛ්‍යා
crun_command_ps (sa_nracanaa crun_global_arguments *global_args, පූර්ණ_සංඛ්‍යා argc, var_na **argv, libcrun_error_t *err)
{
  පූර්ණ_සංඛ්‍යා first_arg;
  පූර්ණ_සංඛ්‍යා ret;
  cleanup_free pid_t *pids = NULL;
  cleanup_context libcrun_context_t *crun_context = NULL;
  size_t i;

  ps_options.format = PS_TABLE;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &ps_options);
  crun_assert_n_args (argc - first_arg, 1, 1);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  ret = libcrun_container_read_pids (crun_context, argv[first_arg], true, &pids, err);
  නම් (UNLIKELY (ret < 0))
    {
      libcrun_error_report_and_release (err);
      ආපසු ret;
    }

  switch (ps_options.format)
    {
    pak_sa PS_JSON:
      printf ("[\n");
      සඳහා (i = 0; pids && pids[i]; i++)
        printf ("  %d%s\n", pids[i], pids[i + 1] ? "," : "");
      printf ("]\n");
      bha_nga;

    pak_sa PS_TABLE:
      printf ("PID\n");
      සඳහා (i = 0; pids && pids[i]; i++)
        printf ("%d\n", pids[i]);
      bha_nga;
    }

  ආපසු 0;
}
