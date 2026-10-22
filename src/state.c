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

بنية state_options_s
{
};

ساكن بنية state_options_s state_options;

ساكن بنية argp_option options[] = { {
    0,
} };

ساكن حرف دلیلونه_doc[] = "state CONTAINER";

ساكن error_t
parse_opt (صحيح کلی, حرف *arg arg_ناکارول_شوی, بنية argp_state *state arg_ناکارول_شوی)
{
  بدل (کلی)
    {
    حالة ARGP_کلی_NO_دلیلونه:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    افتراضي:
      أرجع ARGP_ERR_UNKNOWN;
    }

  أرجع 0;
}

ساكن بنية argp پرلپسې_argp = { options, parse_opt, دلیلونه_doc, doc, عدم, عدم, عدم };

صحيح
crun_command_state (بنية crun_global_arguments *global_دلیلونه, صحيح argc, حرف **argv, libcrun_error_t *err)
{
  صحيح لومړی_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = عدم;
  cleanup_free حرف *state = عدم;

  argp_parse (&پرلپسې_argp, argc, argv, ARGP_دننه_ترتیب, &لومړی_arg, &state_options);
  crun_assert_n_دلیلونه (argc - لومړی_arg, 1, 1);

  crun_context = new_libcrun_context (global_دلیلونه);

  ret = init_libcrun_context (crun_context, argv[لومړی_arg], global_دلیلونه, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_container_state_json (crun_context, argv[لومړی_arg], &state, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  fputs (state, stdout);
  أرجع 0;
}
