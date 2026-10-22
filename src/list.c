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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <argp.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <errno.h>

#અન્તર્ભાવ "crun.h"

સ્થિર અક્ષર doc[] = "OCI runtime";

ગણના
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

સંરચના list_options_s
{
  bool quiet;
  પૂર્ણાંક format;
};

ગણના
{
  LIST_TABLE = 100,
  LIST_JSON,
};

સ્થિર સંરચના list_options_s list_options;

સ્થિર સંરચના argp_option options[]
    = { { "quiet", 'q', 0, 0, "show only IDs", 0 },
        { "format", 'f', "FORMAT", 0, "select one of: table or json (default: \"table\")", 0 },
        {
            0,
        } };

સ્થિર અક્ષર args_doc[] = "list";

સ્થિર error_t
parse_opt (પૂર્ણાંક ચાવી, અક્ષર *arg, સંરચના argp_state *state arg_unused)
{
  પસંદગી (ચાવી)
    {
    સ્થિતિ 'q':
      list_options.quiet = true;
      તોડો;
    સ્થિતિ 'f':
      જો (strcmp (arg, "table") == 0)
        list_options.format = LIST_TABLE;
      નહીંતર જો (strcmp (arg, "json") == 0)
        list_options.format = LIST_JSON;
      નહીંતર
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      તોડો;

    મૂળભૂત:
      પાછા ARGP_ERR_UNKNOWN;
    }

  પાછા 0;
}

સ્થિર સંરચના argp run_argp = { options, parse_opt, args_doc, doc, ખાલી, ખાલી, ખાલી };

પૂર્ણાંક
crun_command_list (સંરચના crun_global_arguments *global_args, પૂર્ણાંક argc, અક્ષર **argv, libcrun_error_t *err)
{
  પૂર્ણાંક first_arg;
  પૂર્ણાંક ret, max_length = 4;
  cleanup_context libcrun_context_t *crun_context = ખાલી;
  libcrun_container_list_t *list = ખાલી;
  libcrun_container_iter_t *it;
  અચલ અક્ષર *name;

  list_options.format = LIST_TABLE;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &list_options);
  crun_assert_n_args (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (list_options.format == LIST_JSON)
    {
      cleanup_free અક્ષર *json = ખાલી;

      ret = libcrun_container_list_json (crun_context, &json, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      fputs (json, stdout);
      પાછા 0;
    }

  ret = libcrun_container_list (crun_context, &list, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  માટે (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
    {
      પૂર્ણાંક l = strlen (name);
      જો (l > max_length)
        max_length = l;
    }
  libcrun_container_iter_free (it);

  max_length++;

  જો (! list_options.quiet)
    printf ("%-*s%-10s%-8s %-39s %-30s %s\n", max_length, "NAME", "PID", "STATUS", "BUNDLE PATH", "CREATED", "OWNER");

  માટે (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
    {
      libcrun_status_t *status = ખાલી;

      ret = libcrun_container_status_load (crun_context, name, &status, err);
      જો (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          ચાલુ;
        }
      જો (list_options.quiet)
        printf ("%s\n", name);
      નહીંતર
        {
          libcrun_container_state_t state;
          પૂર્ણાંક running = 0;
          પૂર્ણાંક pid = libcrun_status_get_pid (status);

          ret = libcrun_status_get_state (status, &state, &running, err);
          જો (UNLIKELY (ret < 0))
            {
              libcrun_error_report_and_release (err);
              libcrun_container_status_free (status);
              ચાલુ;
            }

          જો (! running)
            pid = 0;

          printf ("%-*s%-10d%-8s %-39s %-30s %s\n", max_length, name, pid,
                  libcrun_container_state_to_string (state), libcrun_status_get_bundle (status),
                  libcrun_status_get_created (status), libcrun_status_get_owner (status));
        }

      libcrun_container_status_free (status);
    }
  libcrun_container_iter_free (it);

  libcrun_container_list_free (list);
  પાછા ret >= 0 ? 0 : ret;
}
