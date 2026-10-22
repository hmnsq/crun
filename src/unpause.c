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

ساکن حرف doc[] = "OCI runtime";

ساخت unpause_options_s
{
};

ساکن ساخت unpause_options_s unpause_options;

ساکن ساخت argp_option options[] = { {
    0,
} };

ساکن حرف دلائل_doc[] = "resume CONTAINER";

ساکن error_t
parse_opt (صحیح کلید, حرف *arg arg_غیر_مستعمل, ساخت argp_state *state arg_غیر_مستعمل)
{
  انتخاب (کلید)
    {
    حالت ARGP_کلید_NO_دلائل:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    طےشدہ:
      واپس ARGP_ERR_UNKNOWN;
    }

  واپس 0;
}

ساکن ساخت argp تسلسل_argp = { options, parse_opt, دلائل_doc, doc, تہی, تہی, تہی };

صحیح
crun_command_unpause (ساخت crun_global_arguments *global_دلائل, صحیح argc, حرف **argv, libcrun_error_t *err)
{
  صحیح پہلا_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = تہی;

  argp_parse (&تسلسل_argp, argc, argv, ARGP_اندر_ترتیب, &پہلا_arg, &unpause_options);
  crun_assert_n_دلائل (argc - پہلا_arg, 1, 2);

  crun_context = new_libcrun_context (global_دلائل);

  ret = init_libcrun_context (crun_context, argv[پہلا_arg], global_دلائل, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس libcrun_container_unpause (crun_context, argv[پہلا_arg], err);
}
