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

ساکن حرف doc[] = "OCI runtime";

ساکن ساخت argp_option options[] = {
  0,
};

ساکن حرف دلائل_doc[] = "mounts [add|remove] CONTAINER FILE";

ساکن error_t
parse_opt (صحیح کلید, حرف *arg arg_غیر_مستعمل, ساخت argp_state *state arg_غیر_مستعمل)
{
  انتخاب (کلید)
    {
    طےشدہ:
      واپس ARGP_ERR_UNKNOWN;
    }

  واپس 0;
}

ساکن ساخت argp تسلسل_argp = { options, parse_opt, دلائل_doc, doc, تہی, تہی, تہی };

صحیح
crun_command_mounts (ساخت crun_global_arguments *global_دلائل, صحیح argc, حرف **argv, libcrun_error_t *err)
{
  صحیح پہلا_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = تہی;

  argp_parse (&تسلسل_argp, argc, argv, ARGP_اندر_ترتیب, &پہلا_arg, تہی);
  crun_assert_n_دلائل (argc - پہلا_arg, 3, 3);

  crun_context = new_libcrun_context (global_دلائل);

  ret = init_libcrun_context (crun_context, argv[پہلا_arg], global_دلائل, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (strcmp (argv[پہلا_arg], "add") == 0)
    واپس libcrun_container_جوڑو_mounts_from_file (crun_context, argv[پہلا_arg + 1], argv[پہلا_arg + 2], err);
  ورنہ اگر (strcmp (argv[پہلا_arg], "remove") == 0)
    واپس libcrun_container_remove_mounts_from_file (crun_context, argv[پہلا_arg + 1], argv[پہلا_arg + 2], err);

  واپس crun_make_error (err, 0, "unknown command %s", argv[پہلا_arg]);
}
