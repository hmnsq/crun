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

#شامل "crun.h"

ایستا نویسه doc[] = "OCI runtime";

ایستا ساختار argp_option options[] = {
  0,
};

ایستا نویسه args_doc[] = "mounts [add|remove] CONTAINER FILE";

ایستا error_t
parse_opt (صحیح کلید, نویسه *arg arg_unused, ساختار argp_state *state arg_unused)
{
  انتخاب (کلید)
    {
    پیشفرض:
      بازگشت ARGP_ERR_UNKNOWN;
    }

  بازگشت 0;
}

ایستا ساختار argp run_argp = { options, parse_opt, args_doc, doc, پوچ, پوچ, پوچ };

صحیح
crun_command_mounts (ساختار crun_global_arguments *global_args, صحیح argc, نویسه **argv, libcrun_error_t *err)
{
  صحیح first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = پوچ;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, پوچ);
  crun_assert_n_args (argc - first_arg, 3, 3);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (strcmp (argv[first_arg], "add") == 0)
    بازگشت libcrun_container_add_mounts_from_file (crun_context, argv[first_arg + 1], argv[first_arg + 2], err);
  وگرنه اگر (strcmp (argv[first_arg], "remove") == 0)
    بازگشت libcrun_container_remove_mounts_from_file (crun_context, argv[first_arg + 1], argv[first_arg + 2], err);

  بازگشت crun_make_error (err, 0, "unknown command %s", argv[first_arg]);
}
