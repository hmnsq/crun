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
#include <signal.h>
#include <regex.h>

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

sa_nracanaa kill_options_s
{
  bool all;
  bool regex;
};

static sa_nracanaa kill_options_s kill_options;

static sa_nracanaa argp_option options[]
    = { { "all", 'a', 0, 0, "kill all the processes", 0 },
        { "regex", 'r', 0, 0, "the specified CONTAINER is a regular expression (kill multiple containers)", 0 },
        {
            0,
        } };

static var_na args_doc[] = "kill CONTAINER [SIGNAL]";

static error_t
parse_opt (puur_naa_nka key, var_na *arg arg_unused, sa_nracanaa argp_state *state arg_unused)
{
  switch (key)
    {
    pak_sa 'a':
      kill_options.all = true;
      bha_nga;

    pak_sa 'r':
      kill_options.regex = true;
      bha_nga;

    pak_sa ARGP_KEY_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    default:
      ꦮꦭꦶ ARGP_ERR_UNKNOWN;
    }

  ꦮꦭꦶ 0;
}

static sa_nracanaa argp run_argp = { options, parse_opt, args_doc, doc, NULL, NULL, NULL };

puur_naa_nka
crun_command_kill (sa_nracanaa crun_global_arguments *global_args, puur_naa_nka argc, var_na **argv, libcrun_error_t *err)
{
  puur_naa_nka first_arg = 0, ret;
  const var_na *sa_nkeeta;
  cleanup_context libcrun_context_t *crun_context = NULL;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &kill_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
    ꦮꦭꦶ ret;

  sa_nkeeta = "SIGTERM";
  ꦪꦼꦤ꧀ (argc - first_arg > 1)
    sa_nkeeta = argv[first_arg + 1];

  ꦪꦼꦤ꧀ (kill_options.regex)
    {
      regex_t re;
      libcrun_container_list_t *list;
      libcrun_container_iter_t *it;
      const var_na *name;

      ret = regcomp (&re, argv[first_arg], REG_EXTENDED | REG_NOSUB);
      ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "invalid regular expression %s", argv[first_arg]);

      ret = libcrun_container_list (crun_context, &list, err);
      ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "cannot read containers list");

      ꦏꦁꦒꦺꦴ (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
        ꦪꦼꦤ꧀ (regexec (&re, name, 0, NULL, 0) == 0)
          {
            ret = libcrun_container_kill (crun_context, name, sa_nkeeta, err);
            ꦪꦼꦤ꧀ (UNLIKELY (ret < 0))
              libcrun_error_report_and_release (err);
          }

      libcrun_container_iter_free (it);
      libcrun_container_list_free (list);
      regfree (&re);
      ꦮꦭꦶ 0;
    }

  ꦪꦼꦤ꧀ (kill_options.all)
    ꦮꦭꦶ libcrun_container_killall (crun_context, argv[first_arg], sa_nkeeta, err);

  ꦮꦭꦶ libcrun_container_kill (crun_context, argv[first_arg], sa_nkeeta, err);
}
