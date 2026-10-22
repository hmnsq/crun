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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <argp.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <errno.h>

#अन्तर्भाव "crun.h"

स्थिर वर्ण doc[] = "OCI runtime";

स्थिर संरचना argp_option options[] = {
  0,
};

स्थिर वर्ण तर्काः_doc[] = "mounts [add|remove] CONTAINER FILE";

स्थिर error_t
parse_opt (पूर्णाङ्क कुञ्चिका, वर्ण *arg arg_अप्रयुक्तम्, संरचना argp_state *state arg_अप्रयुक्तम्)
{
  विकल्प (कुञ्चिका)
    {
    स्वभाव:
      प्रत्यागम ARGP_ERR_UNKNOWN;
    }

  प्रत्यागम 0;
}

स्थिर संरचना argp सततम्_argp = { options, parse_opt, तर्काः_doc, doc, रिक्त, रिक्त, रिक्त };

पूर्णाङ्क
crun_command_mounts (संरचना crun_global_arguments *global_तर्काः, पूर्णाङ्क argc, वर्ण **argv, libcrun_error_t *err)
{
  पूर्णाङ्क first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = रिक्त;

  argp_parse (&सततम्_argp, argc, argv, ARGP_अन्तर्_क्रमः, &first_arg, रिक्त);
  crun_assert_n_तर्काः (argc - first_arg, 3, 3);

  crun_context = new_libcrun_context (global_तर्काः);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_तर्काः, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (वाक्यं_तुलय (argv[first_arg], "add") == 0)
    प्रत्यागम libcrun_container_सङ्कलय_mounts_from_file (crun_context, argv[first_arg + 1], argv[first_arg + 2], err);
  अन्यथा यदि (वाक्यं_तुलय (argv[first_arg], "remove") == 0)
    प्रत्यागम libcrun_container_remove_mounts_from_file (crun_context, argv[first_arg + 1], argv[first_arg + 2], err);

  प्रत्यागम crun_make_error (err, 0, "unknown command %s", argv[first_arg]);
}
