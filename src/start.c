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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <argp.h>
#ضمن <string.h>
#ضمن <unistd.h>
#ضمن <errno.h>

#ضمن "crun.h"

ساكن حرف doc[] = "OCI runtime";

تعداد
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

ساكن بنية argp_option options[] = { {
    0,
} };

ساكن حرف args_doc[] = "start CONTAINER";

ساكن error_t
parse_opt (صحيح key, حرف *arg arg_unused, بنية argp_state *state arg_unused)
{
  بدل (key)
    {
    حالة ARGP_KEY_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    افتراضي:
      ܦܢܐ ARGP_ERR_UNKNOWN;
    }

  ܦܢܐ 0;
}

ساكن بنية argp run_argp = { options, parse_opt, args_doc, doc, عدم, عدم, عدم };

صحيح
crun_command_start (بنية crun_global_arguments *global_args, صحيح argc, حرف **argv, libcrun_error_t *err)
{
  صحيح first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = عدم;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, عدم);
  crun_assert_n_args (argc - first_arg, 1, 1);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ܐܢ (UNLIKELY (ret < 0))
    ܦܢܐ ret;

  ܦܢܐ libcrun_container_start (crun_context, argv[first_arg], err);
}
