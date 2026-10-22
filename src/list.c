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

تعداد
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

بنية list_options_s
{
  bool quiet;
  صحيح format;
};

تعداد
{
  LIST_TABLE = 100,
  LIST_JSON,
};

ساكن بنية list_options_s list_options;

ساكن بنية argp_option options[]
    = { { "quiet", 'q', 0, 0, "show only IDs", 0 },
        { "format", 'f', "FORMAT", 0, "select one of: table or json (default: \"table\")", 0 },
        {
            0,
        } };

ساكن حرف args_doc[] = "list";

ساكن error_t
parse_opt (صحيح کلیل, حرف *arg, بنية argp_state *state arg_unused)
{
  بدل (کلیل)
    {
    حالة 'q':
      list_options.quiet = true;
      اكسر;
    حالة 'f':
      إذا (strcmp (arg, "table") == 0)
        list_options.format = LIST_TABLE;
      وإلا إذا (strcmp (arg, "json") == 0)
        list_options.format = LIST_JSON;
      وإلا
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      اكسر;

    افتراضي:
      أرجع ARGP_ERR_UNKNOWN;
    }

  أرجع 0;
}

ساكن بنية argp run_argp = { options, parse_opt, args_doc, doc, عدم, عدم, عدم };

صحيح
crun_command_list (بنية crun_global_arguments *global_args, صحيح argc, حرف **argv, libcrun_error_t *err)
{
  صحيح first_arg;
  صحيح ret, max_length = 4;
  cleanup_context libcrun_context_t *crun_context = عدم;
  libcrun_container_list_t *list = عدم;
  libcrun_container_iter_t *it;
  ثابت حرف *name;

  list_options.format = LIST_TABLE;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &list_options);
  crun_assert_n_args (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (list_options.format == LIST_JSON)
    {
      cleanup_free حرف *json = عدم;

      ret = libcrun_container_list_json (crun_context, &json, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      fputs (json, stdout);
      أرجع 0;
    }

  ret = libcrun_container_list (crun_context, &list, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  لكل (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
    {
      صحيح l = strlen (name);
      إذا (l > max_length)
        max_length = l;
    }
  libcrun_container_iter_free (it);

  max_length++;

  إذا (! list_options.quiet)
    printf ("%-*s%-10s%-8s %-39s %-30s %s\n", max_length, "NAME", "PID", "STATUS", "BUNDLE PATH", "CREATED", "OWNER");

  لكل (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
    {
      libcrun_status_t *status = عدم;

      ret = libcrun_container_status_load (crun_context, name, &status, err);
      إذا (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          استمر;
        }
      إذا (list_options.quiet)
        printf ("%s\n", name);
      وإلا
        {
          libcrun_container_state_t state;
          صحيح running = 0;
          صحيح pid = libcrun_status_get_pid (status);

          ret = libcrun_status_get_state (status, &state, &running, err);
          إذا (UNLIKELY (ret < 0))
            {
              libcrun_error_report_and_release (err);
              libcrun_container_status_free (status);
              استمر;
            }

          إذا (! running)
            pid = 0;

          printf ("%-*s%-10d%-8s %-39s %-30s %s\n", max_length, name, pid,
                  libcrun_container_state_to_string (state), libcrun_status_get_bundle (status),
                  libcrun_status_get_created (status), libcrun_status_get_owner (status));
        }

      libcrun_container_status_free (status);
    }
  libcrun_container_iter_free (it);

  libcrun_container_list_free (list);
  أرجع ret >= 0 ? 0 : ret;
}
