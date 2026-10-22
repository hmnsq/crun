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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <argp.h>
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <unistd.h>
#অন্তর্ভাৱ <errno.h>
#অন্তর্ভাৱ <regex.h>

#অন্তর্ভাৱ "crun.h"

স্থির ৱর্ণ doc[] = "OCI runtime";

সংরচনা pause_options_s
{
};

স্থির সংরচনা pause_options_s pause_options;

স্থির সংরচনা argp_option options[] = { {
    0,
} };

স্থির ৱর্ণ args_doc[] = "pause CONTAINER";

স্থির error_t
parse_opt (পূর্ণাঙ্ক চাবি, ৱর্ণ *arg arg_unused, সংরচনা argp_state *state arg_unused)
{
  ৱিকল্প (চাবি)
    {
    পক্ষ ARGP_চাবি_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    স্ৱভাৱ:
      ঘূৰাই ARGP_ERR_UNKNOWN;
    }

  ঘূৰাই 0;
}

স্থির সংরচনা argp run_argp = { options, parse_opt, args_doc, doc, রিক্ত, রিক্ত, রিক্ত };

পূর্ণাঙ্ক
crun_command_pause (সংরচনা crun_global_arguments *global_args, পূর্ণাঙ্ক argc, ৱর্ণ **argv, libcrun_error_t *err)
{
  পূর্ণাঙ্ক first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = রিক্ত;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &pause_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ঘূৰাই libcrun_container_pause (crun_context, argv[first_arg], err);
}
