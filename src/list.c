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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <argp.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <errno.h>

#ಅನ್ತರ್ಭಾವ "crun.h"

ಸ್ಥಿರ ವರ್ಣ doc[] = "OCI runtime";

ಗಣನಾ
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

ಸಂರಚನಾ list_options_s
{
  bool quiet;
  ಪೂರ್ಣಾಙ್ಕ format;
};

ಗಣನಾ
{
  LIST_TABLE = 100,
  LIST_JSON,
};

ಸ್ಥಿರ ಸಂರಚನಾ list_options_s list_options;

ಸ್ಥಿರ ಸಂರಚನಾ argp_option options[]
    = { { "quiet", 'q', 0, 0, "show only IDs", 0 },
        { "format", 'f', "FORMAT", 0, "select one of: table or json (default: \"table\")", 0 },
        {
            0,
        } };

ಸ್ಥಿರ ವರ್ಣ args_doc[] = "list";

ಸ್ಥಿರ error_t
parse_opt (ಪೂರ್ಣಾಙ್ಕ ಕೀಲಿ, ವರ್ಣ *arg, ಸಂರಚನಾ argp_state *state arg_unused)
{
  ವಿಕಲ್ಪ (ಕೀಲಿ)
    {
    ಪಕ್ಷ 'q':
      list_options.quiet = true;
      ಭಙ್ಗ;
    ಪಕ್ಷ 'f':
      ಯದಿ (strcmp (arg, "table") == 0)
        list_options.format = LIST_TABLE;
      ಅನ್ಯಥಾ ಯದಿ (strcmp (arg, "json") == 0)
        list_options.format = LIST_JSON;
      ಅನ್ಯಥಾ
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      ಭಙ್ಗ;

    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ ARGP_ERR_UNKNOWN;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಸಂರಚನಾ argp run_argp = { options, parse_opt, args_doc, doc, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ };

ಪೂರ್ಣಾಙ್ಕ
crun_command_list (ಸಂರಚನಾ crun_global_arguments *global_args, ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ first_arg;
  ಪೂರ್ಣಾಙ್ಕ ret, max_length = 4;
  cleanup_context libcrun_context_t *crun_context = ರಿಕ್ತ;
  libcrun_container_list_t *list = ರಿಕ್ತ;
  libcrun_container_iter_t *it;
  ಧ್ರುವ ವರ್ಣ *name;

  list_options.format = LIST_TABLE;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &list_options);
  crun_assert_n_args (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (list_options.format == LIST_JSON)
    {
      cleanup_free ವರ್ಣ *json = ರಿಕ್ತ;

      ret = libcrun_container_list_json (crun_context, &json, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      fputs (json, stdout);
      ಪ್ರತ್ಯಾಗಮ 0;
    }

  ret = libcrun_container_list (crun_context, &list, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತಿ (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
    {
      ಪೂರ್ಣಾಙ್ಕ l = strlen (name);
      ಯದಿ (l > max_length)
        max_length = l;
    }
  libcrun_container_iter_free (it);

  max_length++;

  ಯದಿ (! list_options.quiet)
    printf ("%-*s%-10s%-8s %-39s %-30s %s\n", max_length, "NAME", "PID", "STATUS", "BUNDLE PATH", "CREATED", "OWNER");

  ಪ್ರತಿ (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
    {
      libcrun_status_t *status = ರಿಕ್ತ;

      ret = libcrun_container_status_load (crun_context, name, &status, err);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          ಅನುವರ್ತ;
        }
      ಯದಿ (list_options.quiet)
        printf ("%s\n", name);
      ಅನ್ಯಥಾ
        {
          libcrun_container_state_t state;
          ಪೂರ್ಣಾಙ್ಕ running = 0;
          ಪೂರ್ಣಾಙ್ಕ pid = libcrun_status_get_pid (status);

          ret = libcrun_status_get_state (status, &state, &running, err);
          ಯದಿ (UNLIKELY (ret < 0))
            {
              libcrun_error_report_and_release (err);
              libcrun_container_status_free (status);
              ಅನುವರ್ತ;
            }

          ಯದಿ (! running)
            pid = 0;

          printf ("%-*s%-10d%-8s %-39s %-30s %s\n", max_length, name, pid,
                  libcrun_container_state_to_string (state), libcrun_status_get_bundle (status),
                  libcrun_status_get_created (status), libcrun_status_get_owner (status));
        }

      libcrun_container_status_free (status);
    }
  libcrun_container_iter_free (it);

  libcrun_container_list_free (list);
  ಪ್ರತ್ಯಾಗಮ ret >= 0 ? 0 : ret;
}
