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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <argp.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <errno.h>

#அந்தர்பாவ "crun.h"

நிலையான எழுத்து doc[] = "OCI runtime";

எண்ணிக்கை
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

கட்டமைப்பு list_options_s
{
  bool quiet;
  முழு format;
};

எண்ணிக்கை
{
  LIST_TABLE = 100,
  LIST_JSON,
};

நிலையான கட்டமைப்பு list_options_s list_options;

நிலையான கட்டமைப்பு argp_option options[]
    = { { "quiet", 'q', 0, 0, "show only IDs", 0 },
        { "format", 'f', "FORMAT", 0, "select one of: table or json (default: \"table\")", 0 },
        {
            0,
        } };

நிலையான எழுத்து args_doc[] = "list";

நிலையான error_t
parse_opt (முழு சாவி, எழுத்து *arg, கட்டமைப்பு argp_state *state arg_unused)
{
  தேர்வு (சாவி)
    {
    நிலை 'q':
      list_options.quiet = true;
      நிறுத்து;
    நிலை 'f':
      எனில் (strcmp (arg, "table") == 0)
        list_options.format = LIST_TABLE;
      இல்லையெனில் எனில் (strcmp (arg, "json") == 0)
        list_options.format = LIST_JSON;
      இல்லையெனில்
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      நிறுத்து;

    இயல்பு:
      திரும்பு ARGP_ERR_UNKNOWN;
    }

  திரும்பு 0;
}

நிலையான கட்டமைப்பு argp run_argp = { options, parse_opt, args_doc, doc, வெற்று, வெற்று, வெற்று };

முழு
crun_command_list (கட்டமைப்பு crun_global_arguments *global_args, முழு argc, எழுத்து **argv, libcrun_error_t *err)
{
  முழு first_arg;
  முழு ret, max_length = 4;
  cleanup_context libcrun_context_t *crun_context = வெற்று;
  libcrun_container_list_t *list = வெற்று;
  libcrun_container_iter_t *it;
  மாறிலி எழுத்து *name;

  list_options.format = LIST_TABLE;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &list_options);
  crun_assert_n_args (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (list_options.format == LIST_JSON)
    {
      cleanup_free எழுத்து *json = வெற்று;

      ret = libcrun_container_list_json (crun_context, &json, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      fputs (json, stdout);
      திரும்பு 0;
    }

  ret = libcrun_container_list (crun_context, &list, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  க்கு (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
    {
      முழு l = strlen (name);
      எனில் (l > max_length)
        max_length = l;
    }
  libcrun_container_iter_free (it);

  max_length++;

  எனில் (! list_options.quiet)
    printf ("%-*s%-10s%-8s %-39s %-30s %s\n", max_length, "NAME", "PID", "STATUS", "BUNDLE PATH", "CREATED", "OWNER");

  க்கு (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
    {
      libcrun_status_t *status = வெற்று;

      ret = libcrun_container_status_load (crun_context, name, &status, err);
      எனில் (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          தொடர்;
        }
      எனில் (list_options.quiet)
        printf ("%s\n", name);
      இல்லையெனில்
        {
          libcrun_container_state_t state;
          முழு running = 0;
          முழு pid = libcrun_status_get_pid (status);

          ret = libcrun_status_get_state (status, &state, &running, err);
          எனில் (UNLIKELY (ret < 0))
            {
              libcrun_error_report_and_release (err);
              libcrun_container_status_free (status);
              தொடர்;
            }

          எனில் (! running)
            pid = 0;

          printf ("%-*s%-10d%-8s %-39s %-30s %s\n", max_length, name, pid,
                  libcrun_container_state_to_string (state), libcrun_status_get_bundle (status),
                  libcrun_status_get_created (status), libcrun_status_get_owner (status));
        }

      libcrun_container_status_free (status);
    }
  libcrun_container_iter_free (it);

  libcrun_container_list_free (list);
  திரும்பு ret >= 0 ? 0 : ret;
}
