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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <argp.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <errno.h>

#অন্তর্ভাব "crun.h"

স্থির অক্ষর doc[] = "OCI runtime";

গণনা
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

কাঠামো list_options_s
{
  bool quiet;
  পূর্ণসংখ্যা format;
};

গণনা
{
  LIST_TABLE = 100,
  LIST_JSON,
};

স্থির কাঠামো list_options_s list_options;

স্থির কাঠামো argp_option options[]
    = { { "quiet", 'q', 0, 0, "show only IDs", 0 },
        { "format", 'f', "FORMAT", 0, "select one of: table or json (default: \"table\")", 0 },
        {
            0,
        } };

স্থির অক্ষর args_doc[] = "list";

স্থির error_t
parse_opt (পূর্ণসংখ্যা চাবি, অক্ষর *arg, কাঠামো argp_state *state arg_unused)
{
  নির্বাচন (চাবি)
    {
    ক্ষেত্র 'q':
      list_options.quiet = true;
      বিরতি;
    ক্ষেত্র 'f':
      যদি (strcmp (arg, "table") == 0)
        list_options.format = LIST_TABLE;
      অন্যথা যদি (strcmp (arg, "json") == 0)
        list_options.format = LIST_JSON;
      অন্যথা
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      বিরতি;

    পূর্বনির্ধারিত:
      ফেরত ARGP_ERR_UNKNOWN;
    }

  ফেরত 0;
}

স্থির কাঠামো argp run_argp = { options, parse_opt, args_doc, doc, ফাঁকা, ফাঁকা, ফাঁকা };

পূর্ণসংখ্যা
crun_command_list (কাঠামো crun_global_arguments *global_args, পূর্ণসংখ্যা argc, অক্ষর **argv, libcrun_error_t *err)
{
  পূর্ণসংখ্যা first_arg;
  পূর্ণসংখ্যা ret, max_দৈর্ঘ্য = 4;
  cleanup_context libcrun_context_t *crun_context = ফাঁকা;
  libcrun_container_list_t *list = ফাঁকা;
  libcrun_container_iter_t *it;
  ধ্রুবক অক্ষর *name;

  list_options.format = LIST_TABLE;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &list_options);
  crun_assert_n_args (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  যদি (list_options.format == LIST_JSON)
    {
      cleanup_free অক্ষর *json = ফাঁকা;

      ret = libcrun_container_list_json (crun_context, &json, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      fputs (json, stdout);
      ফেরত 0;
    }

  ret = libcrun_container_list (crun_context, &list, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  জন্য (it = libcrun_container_list_iter (list); libcrun_container_iter_পরবর্তী (it, &name);)
    {
      পূর্ণসংখ্যা l = strlen (name);
      যদি (l > max_দৈর্ঘ্য)
        max_দৈর্ঘ্য = l;
    }
  libcrun_container_iter_free (it);

  max_দৈর্ঘ্য++;

  যদি (! list_options.quiet)
    printf ("%-*s%-10s%-8s %-39s %-30s %s\n", max_দৈর্ঘ্য, "NAME", "PID", "STATUS", "BUNDLE PATH", "CREATED", "OWNER");

  জন্য (it = libcrun_container_list_iter (list); libcrun_container_iter_পরবর্তী (it, &name);)
    {
      libcrun_status_t *status = ফাঁকা;

      ret = libcrun_container_status_load (crun_context, name, &status, err);
      যদি (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          চালু;
        }
      যদি (list_options.quiet)
        printf ("%s\n", name);
      অন্যথা
        {
          libcrun_container_state_t state;
          পূর্ণসংখ্যা running = 0;
          পূর্ণসংখ্যা pid = libcrun_status_get_pid (status);

          ret = libcrun_status_get_state (status, &state, &running, err);
          যদি (UNLIKELY (ret < 0))
            {
              libcrun_error_report_and_release (err);
              libcrun_container_status_free (status);
              চালু;
            }

          যদি (! running)
            pid = 0;

          printf ("%-*s%-10d%-8s %-39s %-30s %s\n", max_দৈর্ঘ্য, name, pid,
                  libcrun_container_state_to_string (state), libcrun_status_get_bundle (status),
                  libcrun_status_get_created (status), libcrun_status_get_owner (status));
        }

      libcrun_container_status_free (status);
    }
  libcrun_container_iter_free (it);

  libcrun_container_list_free (list);
  ফেরত ret >= 0 ? 0 : ret;
}
