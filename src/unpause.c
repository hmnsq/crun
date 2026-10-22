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
#include <regex.h>

#include "crun.h"

static var_na doc[] = "OCI runtime";

sa_nracanaa unpause_options_s
{
};

static sa_nracanaa unpause_options_s unpause_options;

static sa_nracanaa argp_option options[] = { {
    0,
} };

static var_na args_doc[] = "resume CONTAINER";

static error_t
parse_opt (ចំនួនគត់ key, var_na *arg arg_unused, sa_nracanaa argp_state *state arg_unused)
{
  switch (key)
    {
    pak_sa ARGP_KEY_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    default:
      ត្រឡប់ ARGP_ERR_UNKNOWN;
    }

  ត្រឡប់ 0;
}

static sa_nracanaa argp run_argp = { options, parse_opt, args_doc, doc, NULL, NULL, NULL };

ចំនួនគត់
crun_command_unpause (sa_nracanaa crun_global_arguments *global_args, ចំនួនគត់ argc, var_na **argv, libcrun_error_t *err)
{
  ចំនួនគត់ first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = NULL;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &unpause_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ត្រឡប់ libcrun_container_unpause (crun_context, argv[first_arg], err);
}
