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

स्थिर अक्षर doc[] = "OCI runtime";

गणना
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

संरचना state_options_s
{
};

स्थिर संरचना state_options_s state_options;

स्थिर संरचना argp_option options[] = { {
    0,
} };

स्थिर अक्षर args_doc[] = "state CONTAINER";

स्थिर error_t
parse_opt (पूर्णांक किल्ली, अक्षर *arg arg_unused, संरचना argp_state *state arg_unused)
{
  निवड (किल्ली)
    {
    स्थिती ARGP_किल्ली_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    पूर्वनिर्धारित:
      परत ARGP_ERR_UNKNOWN;
    }

  परत 0;
}

स्थिर संरचना argp run_argp = { options, parse_opt, args_doc, doc, रिक्त, रिक्त, रिक्त };

पूर्णांक
crun_command_state (संरचना crun_global_arguments *global_args, पूर्णांक argc, अक्षर **argv, libcrun_error_t *err)
{
  पूर्णांक first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = रिक्त;
  cleanup_free अक्षर *state = रिक्त;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &state_options);
  crun_assert_n_args (argc - first_arg, 1, 1);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = libcrun_container_state_json (crun_context, argv[first_arg], &state, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ख_माला_दो (state, मानक_निकास);
  परत 0;
}
