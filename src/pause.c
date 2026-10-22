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

#شامل <config.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <argp.h>
#شامل <string.h>
#شامل <unistd.h>
#شامل <errno.h>
#شامل <regex.h>

#شامل "crun.h"

ایستا نویسه doc[] = "OCI runtime";

ساختار pause_options_s
{
};

ایستا ساختار pause_options_s pause_options;

ایستا ساختار argp_option options[] = { {
    0,
} };

ایستا نویسه args_doc[] = "pause CONTAINER";

ایستا error_t
parse_opt (صحیح کلید, نویسه *arg arg_unused, ساختار argp_state *state arg_unused)
{
  انتخاب (کلید)
    {
    حالت ARGP_کلید_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    پیشفرض:
      بازگشت ARGP_ERR_UNKNOWN;
    }

  بازگشت 0;
}

ایستا ساختار argp run_argp = { options, parse_opt, args_doc, doc, پوچ, پوچ, پوچ };

صحیح
crun_command_pause (ساختار crun_global_arguments *global_args, صحیح argc, نویسه **argv, libcrun_error_t *err)
{
  صحیح first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = پوچ;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &pause_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت libcrun_container_pause (crun_context, argv[first_arg], err);
}
