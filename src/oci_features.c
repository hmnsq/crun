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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <argp.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <errno.h>

#अन्तर्भाव "crun.h"

स्थिर वर्ण doc[] = "OCI runtime";

स्थिर संरचना argp_option options[] = { { 0 } };

स्थिर वर्ण args_doc[] = "features";

स्थिर error_t
parse_opt (पूर्णाङ्क कुंजी, वर्ण *arg arg_unused, संरचना argp_state *state arg_unused)
{
  यदि (कुंजी != ARGP_कुंजी_NO_ARGS)
    {
      प्रत्यागम ARGP_ERR_UNKNOWN;
    }

  प्रत्यागम 0;
}

स्थिर संरचना argp run_argp = { options, parse_opt, args_doc, doc, रिक्त, रिक्त, रिक्त };

पूर्णाङ्क
crun_command_features (संरचना crun_global_arguments *global_args, पूर्णाङ्क argc, वर्ण **argv, libcrun_error_t *err)
{
  cleanup_free वर्ण *json = रिक्त;
  पूर्णाङ्क first_arg = 0, ret = 0;
  cleanup_context libcrun_context_t *crun_context = रिक्त;

  argp_parse (&run_argp, argc, argv, 0, 0, &options);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcrun_container_get_features_json (crun_context, &json, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  म_लिखो ("%s", json);

  प्रत्यागम 0;
}
