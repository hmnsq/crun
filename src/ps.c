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

static char doc[] = "OCI runtime";

enum
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

struct ps_options_s
{
  整数 format;
};

enum
{
  PS_TABLE = 100,
  PS_JSON,
};

static struct ps_options_s ps_options;

static struct argp_option options[] = { { "format", 'f', "FORMAT", 0, "select the output format", 0 },
                                        {
                                            0,
                                        } };

static char args_doc[] = "ps";

static error_t
parse_opt (整数 key, char *arg, struct argp_state *state arg_unused)
{
  switch (key)
    {
    case 'f':
      もし (strcmp (arg, "table") == 0)
        ps_options.format = PS_TABLE;
      否则 もし (strcmp (arg, "json") == 0)
        ps_options.format = PS_JSON;
      否则
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      break;

    default:
      戻る ARGP_ERR_UNKNOWN;
    }

  戻る 0;
}

static struct argp run_argp = { options, parse_opt, args_doc, doc, NULL, NULL, NULL };

整数
crun_command_ps (struct crun_global_arguments *global_args, 整数 argc, char **argv, libcrun_error_t *err)
{
  整数 first_arg;
  整数 ret;
  cleanup_free pid_t *pids = NULL;
  cleanup_context libcrun_context_t *crun_context = NULL;
  size_t i;

  ps_options.format = PS_TABLE;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &ps_options);
  crun_assert_n_args (argc - first_arg, 1, 1);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ret = libcrun_container_read_pids (crun_context, argv[first_arg], true, &pids, err);
  もし (UNLIKELY (ret < 0))
    {
      libcrun_error_report_and_release (err);
      戻る ret;
    }

  switch (ps_options.format)
    {
    case PS_JSON:
      printf ("[\n");
      ために (i = 0; pids && pids[i]; i++)
        printf ("  %d%s\n", pids[i], pids[i + 1] ? "," : "");
      printf ("]\n");
      break;

    case PS_TABLE:
      printf ("PID\n");
      ために (i = 0; pids && pids[i]; i++)
        printf ("%d\n", pids[i]);
      break;
    }

  戻る 0;
}
