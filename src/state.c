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

স্থির বর্ণ doc[] = "OCI runtime";

গণনা
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

সংরচনা state_options_s
{
};

স্থির সংরচনা state_options_s state_options;

স্থির সংরচনা argp_option options[] = { {
    0,
} };

স্থির বর্ণ args_doc[] = "state CONTAINER";

স্থির error_t
parse_opt (পূর্ণাঙ্ক চাবি, বর্ণ *arg arg_unused, সংরচনা argp_state *state arg_unused)
{
  বিকল্প (চাবি)
    {
    পক্ষ ARGP_চাবি_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    স্বভাব:
      প্রত্যাগম ARGP_ERR_UNKNOWN;
    }

  প্রত্যাগম 0;
}

স্থির সংরচনা argp run_argp = { options, parse_opt, args_doc, doc, রিক্ত, রিক্ত, রিক্ত };

পূর্ণাঙ্ক
crun_command_state (সংরচনা crun_global_arguments *global_args, পূর্ণাঙ্ক argc, বর্ণ **argv, libcrun_error_t *err)
{
  পূর্ণাঙ্ক first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = রিক্ত;
  cleanup_free বর্ণ *state = রিক্ত;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &state_options);
  crun_assert_n_args (argc - first_arg, 1, 1);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = libcrun_container_state_json (crun_context, argv[first_arg], &state, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  fputs (state, stdout);
  প্রত্যাগম 0;
}
