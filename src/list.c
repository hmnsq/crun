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

ಸ್ಥಿರ ಅಕ್ಷರ doc[] = "OCI runtime";

ಎಣಿಕೆ
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

ರಚನೆ list_options_s
{
  bool quiet;
  ಪೂರ್ಣಾಂಕ format;
};

ಎಣಿಕೆ
{
  LIST_ಕೋಷ್ಟಕ = 100,
  LIST_JSON,
};

ಸ್ಥಿರ ರಚನೆ list_options_s list_options;

ಸ್ಥಿರ ರಚನೆ argp_option options[]
    = { { "quiet", 'q', 0, 0, "show only IDs", 0 },
        { "format", 'f', "FORMAT", 0, "select one of: table or json (default: \"table\")", 0 },
        {
            0,
        } };

ಸ್ಥಿರ ಅಕ್ಷರ ವಾದಗಳು_doc[] = "list";

ಸ್ಥಿರ error_t
parse_opt (ಪೂರ್ಣಾಂಕ ಕೀಲಿ, ಅಕ್ಷರ *arg, ರಚನೆ argp_state *state arg_ಬಳಸದ)
{
  ಆಯ್ಕೆ (ಕೀಲಿ)
    {
    ಸ್ಥಿತಿ 'q':
      list_options.quiet = true;
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'f':
      ಆದರೆ (strcmp (arg, "table") == 0)
        list_options.format = LIST_ಕೋಷ್ಟಕ;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (arg, "json") == 0)
        list_options.format = LIST_JSON;
      ಇಲ್ಲದಿದ್ದರೆ
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ ARGP_ERR_UNKNOWN;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ರಚನೆ argp ಸತತ_argp = { options, parse_opt, ವಾದಗಳು_doc, doc, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ };

ಪೂರ್ಣಾಂಕ
crun_command_list (ರಚನೆ crun_global_arguments *global_ವಾದಗಳು, ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ಮೊದಲ_arg;
  ಪೂರ್ಣಾಂಕ ret, max_ಉದ್ದಳತೆ = 4;
  cleanup_context libcrun_context_t *crun_context = ಖಾಲಿ;
  libcrun_container_list_t *list = ಖಾಲಿ;
  libcrun_container_iter_t *it;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;

  list_options.format = LIST_ಕೋಷ್ಟಕ;

  argp_parse (&ಸತತ_argp, argc, argv, ARGP_ಒಳ_ಕ್ರಮ, &ಮೊದಲ_arg, &list_options);
  crun_assert_n_ವಾದಗಳು (argc - ಮೊದಲ_arg, 0, 0);

  crun_context = new_libcrun_context (global_ವಾದಗಳು);

  ret = init_libcrun_context (crun_context, argv[ಮೊದಲ_arg], global_ವಾದಗಳು, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (list_options.format == LIST_JSON)
    {
      cleanup_free ಅಕ್ಷರ *json = ಖಾಲಿ;

      ret = libcrun_container_list_json (crun_context, &json, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      fputs (json, stdout);
      ಹಿಂತಿರುಗಿ 0;
    }

  ret = libcrun_container_list (crun_context, &list, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಗಾಗಿ (it = libcrun_container_list_iter (list); libcrun_container_iter_ಮುಂದಿನ (it, &name);)
    {
      ಪೂರ್ಣಾಂಕ l = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (name);
      ಆದರೆ (l > max_ಉದ್ದಳತೆ)
        max_ಉದ್ದಳತೆ = l;
    }
  libcrun_container_iter_free (it);

  max_ಉದ್ದಳತೆ++;

  ಆದರೆ (! list_options.quiet)
    ಬರೆ ("%-*s%-10s%-8s %-39s %-30s %s\n", max_ಉದ್ದಳತೆ, "NAME", "PID", "STATUS", "BUNDLE PATH", "CREATED", "OWNER");

  ಗಾಗಿ (it = libcrun_container_list_iter (list); libcrun_container_iter_ಮುಂದಿನ (it, &name);)
    {
      libcrun_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2 = ಖಾಲಿ;

      ret = libcrun_container_ಸ್ಥಿತಿ_ತುಂಬು (crun_context, name, &ಸ್ಥಿತಿ_2, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          ಮುಂದುವರಿ;
        }
      ಆದರೆ (list_options.quiet)
        ಬರೆ ("%s\n", name);
      ಇಲ್ಲದಿದ್ದರೆ
        {
          libcrun_container_state_t state;
          ಪೂರ್ಣಾಂಕ running = 0;
          ಪೂರ್ಣಾಂಕ pid = libcrun_ಸ್ಥಿತಿ_get_pid (ಸ್ಥಿತಿ_2);

          ret = libcrun_ಸ್ಥಿತಿ_get_state (ಸ್ಥಿತಿ_2, &state, &running, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            {
              libcrun_error_report_and_release (err);
              libcrun_container_ಸ್ಥಿತಿ_free (ಸ್ಥಿತಿ_2);
              ಮುಂದುವರಿ;
            }

          ಆದರೆ (! running)
            pid = 0;

          ಬರೆ ("%-*s%-10d%-8s %-39s %-30s %s\n", max_ಉದ್ದಳತೆ, name, pid,
                  libcrun_container_state_ಗಮ್ಯ_string (state), libcrun_ಸ್ಥಿತಿ_get_bundle (ಸ್ಥಿತಿ_2),
                  libcrun_ಸ್ಥಿತಿ_get_created (ಸ್ಥಿತಿ_2), libcrun_ಸ್ಥಿತಿ_get_owner (ಸ್ಥಿತಿ_2));
        }

      libcrun_container_ಸ್ಥಿತಿ_free (ಸ್ಥಿತಿ_2);
    }
  libcrun_container_iter_free (it);

  libcrun_container_list_free (list);
  ಹಿಂತಿರುಗಿ ret >= 0 ? 0 : ret;
}
