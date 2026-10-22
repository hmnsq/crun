/*
 * crun - OCI runtime written in C
 *
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

ساكن بنية argp_option options[] = { { 0 } };

ساكن حرف دلیلاں_doc[] = "features";

ساكن error_t
parse_opt (صحيح کلید, حرف *arg arg_بن_ورتیا, بنية argp_state *state arg_بن_ورتیا)
{
  إذا (کلید != ARGP_کلید_NO_دلیلاں)
    {
      أرجع ARGP_ERR_UNKNOWN;
    }

  أرجع 0;
}

ساكن بنية argp لگاتار_argp = { options, parse_opt, دلیلاں_doc, doc, عدم, عدم, عدم };

صحيح
crun_command_features (بنية crun_global_arguments *global_دلیلاں, صحيح argc, حرف **argv, libcrun_error_t *err)
{
  cleanup_free حرف *json = عدم;
  صحيح پہلا_arg = 0, ret = 0;
  cleanup_context libcrun_context_t *crun_context = عدم;

  argp_parse (&لگاتار_argp, argc, argv, 0, 0, &options);

  crun_context = new_libcrun_context (global_دلیلاں);

  ret = init_libcrun_context (crun_context, argv[پہلا_arg], global_دلیلاں, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_container_get_features_json (crun_context, &json, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  لکھو ("%s", json);

  أرجع 0;
}
