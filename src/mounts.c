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

ساكن بنية argp_option options[] = {
  0,
};

ساكن حرف دليلون_doc[] = "mounts [add|remove] CONTAINER FILE";

ساكن error_t
parse_opt (صحيح ڪنجي, حرف *arg arg_اڻ_استعمال_ٿيل, بنية argp_state *state arg_اڻ_استعمال_ٿيل)
{
  بدل (ڪنجي)
    {
    افتراضي:
      أرجع ARGP_ERR_UNKNOWN;
    }

  أرجع 0;
}

ساكن بنية argp لاڳيتو_argp = { options, parse_opt, دليلون_doc, doc, عدم, عدم, عدم };

صحيح
crun_command_mounts (بنية crun_global_arguments *global_دليلون, صحيح argc, حرف **argv, libcrun_error_t *err)
{
  صحيح پهريون_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = عدم;

  argp_parse (&لاڳيتو_argp, argc, argv, ARGP_اندريون_ترتيب, &پهريون_arg, عدم);
  crun_assert_n_دليلون (argc - پهريون_arg, 3, 3);

  crun_context = new_libcrun_context (global_دليلون);

  ret = init_libcrun_context (crun_context, argv[پهريون_arg], global_دليلون, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (strcmp (argv[پهريون_arg], "add") == 0)
    أرجع libcrun_container_جوڙيو_mounts_from_file (crun_context, argv[پهريون_arg + 1], argv[پهريون_arg + 2], err);
  وإلا إذا (strcmp (argv[پهريون_arg], "remove") == 0)
    أرجع libcrun_container_remove_mounts_from_file (crun_context, argv[پهريون_arg + 1], argv[پهريون_arg + 2], err);

  أرجع crun_make_error (err, 0, "unknown command %s", argv[پهريون_arg]);
}
