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

#شامل <config.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <argp.h>
#شامل <string.h>
#شامل <unistd.h>
#شامل <errno.h>

#شامل "crun.h"

ایستا نویسه doc[] = "OCI runtime";

ایستا ساختار argp_option options[] = { { 0 } };

ایستا نویسه args_doc[] = "features";

ایستا error_t
parse_opt (صحیح کلید, نویسه *arg arg_unused, ساختار argp_state *state arg_unused)
{
  اگر (کلید != ARGP_کلید_NO_ARGS)
    {
      بازگشت ARGP_ERR_UNKNOWN;
    }

  بازگشت 0;
}

ایستا ساختار argp run_argp = { options, parse_opt, args_doc, doc, پوچ, پوچ, پوچ };

صحیح
crun_command_features (ساختار crun_global_arguments *global_args, صحیح argc, نویسه **argv, libcrun_error_t *err)
{
  cleanup_free نویسه *json = پوچ;
  صحیح first_arg = 0, ret = 0;
  cleanup_context libcrun_context_t *crun_context = پوچ;

  argp_parse (&run_argp, argc, argv, 0, 0, &options);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = libcrun_container_get_features_json (crun_context, &json, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  printf ("%s", json);

  بازگشت 0;
}
