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

ایستا نویسه doc[] = "OCI runtime";

شمارش
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

ساختار list_options_s
{
  bool quiet;
  صحیح format;
};

شمارش
{
  LIST_TABLE = 100,
  LIST_JSON,
};

ایستا ساختار list_options_s list_options;

ایستا ساختار argp_option options[]
    = { { "quiet", 'q', 0, 0, "show only IDs", 0 },
        { "format", 'f', "FORMAT", 0, "select one of: table or json (default: \"table\")", 0 },
        {
            0,
        } };

ایستا نویسه args_doc[] = "list";

ایستا error_t
parse_opt (صحیح کلید, نویسه *arg, ساختار argp_state *state arg_unused)
{
  انتخاب (کلید)
    {
    حالت 'q':
      list_options.quiet = true;
      بشکن;
    حالت 'f':
      اگر (strcmp (arg, "table") == 0)
        list_options.format = LIST_TABLE;
      وگرنه اگر (strcmp (arg, "json") == 0)
        list_options.format = LIST_JSON;
      وگرنه
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      بشکن;

    پیشفرض:
      بازگشت ARGP_ERR_UNKNOWN;
    }

  بازگشت 0;
}

ایستا ساختار argp run_argp = { options, parse_opt, args_doc, doc, پوچ, پوچ, پوچ };

صحیح
crun_command_list (ساختار crun_global_arguments *global_args, صحیح argc, نویسه **argv, libcrun_error_t *err)
{
  صحیح first_arg;
  صحیح ret, max_length = 4;
  cleanup_context libcrun_context_t *crun_context = پوچ;
  libcrun_container_list_t *list = پوچ;
  libcrun_container_iter_t *it;
  ثابت نویسه *name;

  list_options.format = LIST_TABLE;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &list_options);
  crun_assert_n_args (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (list_options.format == LIST_JSON)
    {
      cleanup_free نویسه *json = پوچ;

      ret = libcrun_container_list_json (crun_context, &json, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      fputs (json, stdout);
      بازگشت 0;
    }

  ret = libcrun_container_list (crun_context, &list, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  برای (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
    {
      صحیح l = strlen (name);
      اگر (l > max_length)
        max_length = l;
    }
  libcrun_container_iter_free (it);

  max_length++;

  اگر (! list_options.quiet)
    printf ("%-*s%-10s%-8s %-39s %-30s %s\n", max_length, "NAME", "PID", "STATUS", "BUNDLE PATH", "CREATED", "OWNER");

  برای (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
    {
      libcrun_status_t *status = پوچ;

      ret = libcrun_container_status_load (crun_context, name, &status, err);
      اگر (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          ادامه;
        }
      اگر (list_options.quiet)
        printf ("%s\n", name);
      وگرنه
        {
          libcrun_container_state_t state;
          صحیح running = 0;
          صحیح pid = libcrun_status_get_pid (status);

          ret = libcrun_status_get_state (status, &state, &running, err);
          اگر (UNLIKELY (ret < 0))
            {
              libcrun_error_report_and_release (err);
              libcrun_container_status_free (status);
              ادامه;
            }

          اگر (! running)
            pid = 0;

          printf ("%-*s%-10d%-8s %-39s %-30s %s\n", max_length, name, pid,
                  libcrun_container_state_to_string (state), libcrun_status_get_bundle (status),
                  libcrun_status_get_created (status), libcrun_status_get_owner (status));
        }

      libcrun_container_status_free (status);
    }
  libcrun_container_iter_free (it);

  libcrun_container_list_free (list);
  بازگشت ret >= 0 ? 0 : ret;
}
