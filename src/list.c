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

संरचना list_options_s
{
  bool quiet;
  पूर्णांक format;
};

गणना
{
  LIST_TABLE = 100,
  LIST_JSON,
};

स्थिर संरचना list_options_s list_options;

स्थिर संरचना argp_option options[]
    = { { "quiet", 'q', 0, 0, "show only IDs", 0 },
        { "format", 'f', "FORMAT", 0, "select one of: table or json (default: \"table\")", 0 },
        {
            0,
        } };

स्थिर अक्षर args_doc[] = "list";

स्थिर error_t
parse_opt (पूर्णांक किल्ली, अक्षर *arg, संरचना argp_state *state arg_unused)
{
  निवड (किल्ली)
    {
    स्थिती 'q':
      list_options.quiet = true;
      थांबा;
    स्थिती 'f':
      जर (म_भेद (arg, "table") == 0)
        list_options.format = LIST_TABLE;
      अन्यथा जर (म_भेद (arg, "json") == 0)
        list_options.format = LIST_JSON;
      अन्यथा
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      थांबा;

    पूर्वनिर्धारित:
      परत ARGP_ERR_UNKNOWN;
    }

  परत 0;
}

स्थिर संरचना argp run_argp = { options, parse_opt, args_doc, doc, रिक्त, रिक्त, रिक्त };

पूर्णांक
crun_command_list (संरचना crun_global_arguments *global_args, पूर्णांक argc, अक्षर **argv, libcrun_error_t *err)
{
  पूर्णांक first_arg;
  पूर्णांक ret, max_length = 4;
  cleanup_context libcrun_context_t *crun_context = रिक्त;
  libcrun_container_list_t *list = रिक्त;
  libcrun_container_iter_t *it;
  स्थिरांक अक्षर *name;

  list_options.format = LIST_TABLE;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &list_options);
  crun_assert_n_args (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (list_options.format == LIST_JSON)
    {
      cleanup_free अक्षर *json = रिक्त;

      ret = libcrun_container_list_json (crun_context, &json, err);
      जर (UNLIKELY (ret < 0))
        परत ret;

      ख_माला_दो (json, मानक_निकास);
      परत 0;
    }

  ret = libcrun_container_list (crun_context, &list, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  साठी (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
    {
      पूर्णांक l = म_माप (name);
      जर (l > max_length)
        max_length = l;
    }
  libcrun_container_iter_free (it);

  max_length++;

  जर (! list_options.quiet)
    म_लिखो ("%-*s%-10s%-8s %-39s %-30s %s\n", max_length, "NAME", "PID", "STATUS", "BUNDLE PATH", "CREATED", "OWNER");

  साठी (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
    {
      libcrun_status_t *status = रिक्त;

      ret = libcrun_container_status_load (crun_context, name, &status, err);
      जर (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          सुरूठेवा;
        }
      जर (list_options.quiet)
        म_लिखो ("%s\n", name);
      अन्यथा
        {
          libcrun_container_state_t state;
          पूर्णांक running = 0;
          पूर्णांक pid = libcrun_status_get_pid (status);

          ret = libcrun_status_get_state (status, &state, &running, err);
          जर (UNLIKELY (ret < 0))
            {
              libcrun_error_report_and_release (err);
              libcrun_container_status_free (status);
              सुरूठेवा;
            }

          जर (! running)
            pid = 0;

          म_लिखो ("%-*s%-10d%-8s %-39s %-30s %s\n", max_length, name, pid,
                  libcrun_container_state_to_string (state), libcrun_status_get_bundle (status),
                  libcrun_status_get_created (status), libcrun_status_get_owner (status));
        }

      libcrun_container_status_free (status);
    }
  libcrun_container_iter_free (it);

  libcrun_container_list_free (list);
  परत ret >= 0 ? 0 : ret;
}
