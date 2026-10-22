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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <argp.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <errno.h>

#അന്തര്ഭാവ "crun.h"

സ്ഥിതം അക്ഷരം doc[] = "OCI runtime";

എണ്ണൽ
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

ഘടന list_options_s
{
  bool quiet;
  പൂർണ്ണസംഖ്യ format;
};

എണ്ണൽ
{
  LIST_TABLE = 100,
  LIST_JSON,
};

സ്ഥിതം ഘടന list_options_s list_options;

സ്ഥിതം ഘടന argp_option options[]
    = { { "quiet", 'q', 0, 0, "show only IDs", 0 },
        { "format", 'f', "FORMAT", 0, "select one of: table or json (default: \"table\")", 0 },
        {
            0,
        } };

സ്ഥിതം അക്ഷരം args_doc[] = "list";

സ്ഥിതം error_t
parse_opt (പൂർണ്ണസംഖ്യ താക്കോൽ, അക്ഷരം *arg, ഘടന argp_state *state arg_unused)
{
  തിരഞ്ഞെടുപ്പ് (താക്കോൽ)
    {
    അവസ്ഥ 'q':
      list_options.quiet = true;
      നിർത്തുക;
    അവസ്ഥ 'f':
      എങ്കിൽ (strcmp (arg, "table") == 0)
        list_options.format = LIST_TABLE;
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (arg, "json") == 0)
        list_options.format = LIST_JSON;
      അല്ലെങ്കിൽ
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
      തിരികെ ARGP_ERR_UNKNOWN;
    }

  തിരികെ 0;
}

സ്ഥിതം ഘടന argp run_argp = { options, parse_opt, args_doc, doc, ശൂന്യ, ശൂന്യ, ശൂന്യ };

പൂർണ്ണസംഖ്യ
crun_command_list (ഘടന crun_global_arguments *global_args, പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ first_arg;
  പൂർണ്ണസംഖ്യ ret, max_length = 4;
  cleanup_context libcrun_context_t *crun_context = ശൂന്യ;
  libcrun_container_list_t *list = ശൂന്യ;
  libcrun_container_iter_t *it;
  സ്ഥിരം അക്ഷരം *name;

  list_options.format = LIST_TABLE;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &list_options);
  crun_assert_n_args (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (list_options.format == LIST_JSON)
    {
      cleanup_free അക്ഷരം *json = ശൂന്യ;

      ret = libcrun_container_list_json (crun_context, &json, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      fputs (json, stdout);
      തിരികെ 0;
    }

  ret = libcrun_container_list (crun_context, &list, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  വേണ്ടി (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
    {
      പൂർണ്ണസംഖ്യ l = strlen (name);
      എങ്കിൽ (l > max_length)
        max_length = l;
    }
  libcrun_container_iter_free (it);

  max_length++;

  എങ്കിൽ (! list_options.quiet)
    printf ("%-*s%-10s%-8s %-39s %-30s %s\n", max_length, "NAME", "PID", "STATUS", "BUNDLE PATH", "CREATED", "OWNER");

  വേണ്ടി (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
    {
      libcrun_status_t *status = ശൂന്യ;

      ret = libcrun_container_status_load (crun_context, name, &status, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          തുടരുക;
        }
      എങ്കിൽ (list_options.quiet)
        printf ("%s\n", name);
      അല്ലെങ്കിൽ
        {
          libcrun_container_state_t state;
          പൂർണ്ണസംഖ്യ running = 0;
          പൂർണ്ണസംഖ്യ pid = libcrun_status_get_pid (status);

          ret = libcrun_status_get_state (status, &state, &running, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            {
              libcrun_error_report_and_release (err);
              libcrun_container_status_free (status);
              തുടരുക;
            }

          എങ്കിൽ (! running)
            pid = 0;

          printf ("%-*s%-10d%-8s %-39s %-30s %s\n", max_length, name, pid,
                  libcrun_container_state_to_string (state), libcrun_status_get_bundle (status),
                  libcrun_status_get_created (status), libcrun_status_get_owner (status));
        }

      libcrun_container_status_free (status);
    }
  libcrun_container_iter_free (it);

  libcrun_container_list_free (list);
  തിരികെ ret >= 0 ? 0 : ret;
}
