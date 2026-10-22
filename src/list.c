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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <argp.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>

#ଅନ୍ତର୍ଭାଵ "crun.h"

ସ୍ଥିର ଅକ୍ଷର doc[] = "OCI runtime";

ଗଣନା
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

ସଂରଚନା list_options_s
{
  bool quiet;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା format;
};

ଗଣନା
{
  LIST_TABLE = 100,
  LIST_JSON,
};

ସ୍ଥିର ସଂରଚନା list_options_s list_options;

ସ୍ଥିର ସଂରଚନା argp_option options[]
    = { { "quiet", 'q', 0, 0, "show only IDs", 0 },
        { "format", 'f', "FORMAT", 0, "select one of: table or json (default: \"table\")", 0 },
        {
            0,
        } };

ସ୍ଥିର ଅକ୍ଷର args_doc[] = "list";

ସ୍ଥିର error_t
parse_opt (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଚାବି, ଅକ୍ଷର *arg, ସଂରଚନା argp_state *state arg_unused)
{
  ବାଛ (ଚାବି)
    {
    ସ୍ଥିତି 'q':
      list_options.quiet = true;
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'f':
      ଯଦି (strcmp (arg, "table") == 0)
        list_options.format = LIST_TABLE;
      ଅନ୍ୟଥା ଯଦି (strcmp (arg, "json") == 0)
        list_options.format = LIST_JSON;
      ଅନ୍ୟଥା
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ ARGP_ERR_UNKNOWN;
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ସଂରଚନା argp run_argp = { options, parse_opt, args_doc, doc, ଖାଲି, ଖାଲି, ଖାଲି };

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
crun_command_list (ସଂରଚନା crun_global_arguments *global_args, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_arg;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret, max_length = 4;
  cleanup_context libcrun_context_t *crun_context = ଖାଲି;
  libcrun_container_list_t *list = ଖାଲି;
  libcrun_container_iter_t *it;
  ଅଚଳ ଅକ୍ଷର *name;

  list_options.format = LIST_TABLE;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &list_options);
  crun_assert_n_args (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (list_options.format == LIST_JSON)
    {
      cleanup_free ଅକ୍ଷର *json = ଖାଲି;

      ret = libcrun_container_list_json (crun_context, &json, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      fputs (json, stdout);
      ଫେରନ୍ତୁ 0;
    }

  ret = libcrun_container_list (crun_context, &list, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ପାଇଁ (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା l = strlen (name);
      ଯଦି (l > max_length)
        max_length = l;
    }
  libcrun_container_iter_free (it);

  max_length++;

  ଯଦି (! list_options.quiet)
    printf ("%-*s%-10s%-8s %-39s %-30s %s\n", max_length, "NAME", "PID", "STATUS", "BUNDLE PATH", "CREATED", "OWNER");

  ପାଇଁ (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
    {
      libcrun_status_t *status = ଖାଲି;

      ret = libcrun_container_status_load (crun_context, name, &status, err);
      ଯଦି (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          ଜାରି;
        }
      ଯଦି (list_options.quiet)
        printf ("%s\n", name);
      ଅନ୍ୟଥା
        {
          libcrun_container_state_t state;
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା running = 0;
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା pid = libcrun_status_get_pid (status);

          ret = libcrun_status_get_state (status, &state, &running, err);
          ଯଦି (UNLIKELY (ret < 0))
            {
              libcrun_error_report_and_release (err);
              libcrun_container_status_free (status);
              ଜାରି;
            }

          ଯଦି (! running)
            pid = 0;

          printf ("%-*s%-10d%-8s %-39s %-30s %s\n", max_length, name, pid,
                  libcrun_container_state_to_string (state), libcrun_status_get_bundle (status),
                  libcrun_status_get_created (status), libcrun_status_get_owner (status));
        }

      libcrun_container_status_free (status);
    }
  libcrun_container_iter_free (it);

  libcrun_container_list_free (list);
  ଫେରନ୍ତୁ ret >= 0 ? 0 : ret;
}
