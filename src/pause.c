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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश <argp.h>
#समावेश <string.h>
#समावेश <unistd.h>
#समावेश <errno.h>
#समावेश <regex.h>

#समावेश "crun.h"

अटल अक्षर doc[] = "OCI runtime";

काष्ठा pause_options_s
{
};

अटल काष्ठा pause_options_s pause_options;

अटल काष्ठा argp_option options[] = { {
    0,
} };

अटल अक्षर args_doc[] = "pause CONTAINER";

अटल error_t
parse_opt (पूर्णांक कुंजी, अक्षर *arg arg_unused, काष्ठा argp_state *state arg_unused)
{
  चयन (कुंजी)
    {
    हाल ARGP_कुंजी_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    शेष:
      वापस ARGP_ERR_UNKNOWN;
    }

  वापस 0;
}

अटल काष्ठा argp run_argp = { options, parse_opt, args_doc, doc, शून्य, शून्य, शून्य };

पूर्णांक
crun_command_pause (काष्ठा crun_global_arguments *global_args, पूर्णांक argc, अक्षर **argv, libcrun_error_t *err)
{
  पूर्णांक first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = शून्य;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &pause_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस libcrun_container_pause (crun_context, argv[first_arg], err);
}
