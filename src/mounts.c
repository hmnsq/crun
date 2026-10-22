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

ساكن حرف وسائط_doc[] = "mounts [add|remove] CONTAINER FILE";

ساكن error_t
parse_opt (صحيح مفتاح, حرف *arg arg_unused, بنية argp_state *state arg_unused)
{
  بدل (مفتاح)
    {
    افتراضي:
      أرجع ARGP_ERR_UNKNOWN;
    }

  أرجع 0;
}

ساكن بنية argp run_argp = { options, parse_opt, وسائط_doc, doc, عدم, عدم, عدم };

صحيح
crun_command_mounts (بنية crun_global_arguments *global_وسائط, صحيح argc, حرف **argv, libcrun_error_t *err)
{
  صحيح first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = عدم;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, عدم);
  crun_assert_n_وسائط (argc - first_arg, 3, 3);

  crun_context = new_libcrun_context (global_وسائط);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_وسائط, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (strcmp (argv[first_arg], "add") == 0)
    أرجع libcrun_container_add_mounts_from_file (crun_context, argv[first_arg + 1], argv[first_arg + 2], err);
  وإلا إذا (strcmp (argv[first_arg], "remove") == 0)
    أرجع libcrun_container_remove_mounts_from_file (crun_context, argv[first_arg + 1], argv[first_arg + 2], err);

  أرجع crun_make_error (err, 0, "unknown command %s", argv[first_arg]);
}
