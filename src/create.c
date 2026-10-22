/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019, 2020 Giuseppe Scrivano <giuseppe@scrivano.org>
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
#അന്തര്ഭാവ <argp.h>

#അന്തര്ഭാവ "crun.h"
#അന്തര്ഭാവ "run_create.h"

എണ്ണൽ
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS,
  OPTION_NO_PIVOT
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *bundle = ശൂന്യ;

സ്ഥിതം libcrun_context_t *crun_context;

സ്ഥിതം ഘടന argp_option options[]
    = { { "bundle", 'b', "DIR", 0, "container bundle (default \".\")", 0 },
        { "config", 'f', "FILE", 0, "override the config file name", 0 },
        { "console-socket", OPTION_CONSOLE_SOCKET, "SOCK", 0,
          "path to a socket that will receive the ptmx end of the tty", 0 },
        { "preserve-fds", OPTION_PRESERVE_FDS, "N", 0, "pass additional FDs to the container", 0 },
        { "no-pivot", OPTION_NO_PIVOT, 0, 0, "do not use pivot_root", 0 },
        { "pid-file", OPTION_PID_FILE, "FILE", 0, "where to write the PID of the container", 0 },
        { "no-subreaper", OPTION_NO_SUBREAPER, 0, 0, "do not create a subreaper process (ignored)", 0 },
        { "no-new-keyring", OPTION_NO_NEW_KEYRING, 0, 0, "keep the same session key", 0 },
        {
            0,
        } };

സ്ഥിതം അക്ഷരം doc[] = "OCI runtime";

സ്ഥിതം അക്ഷരം args_doc[] = "create [OPTION]... CONTAINER";

സ്ഥിതം സ്ഥിരം അക്ഷരം *config_file = "config.json";

സ്ഥിതം error_t
parse_opt (പൂർണ്ണസംഖ്യ താക്കോൽ, അക്ഷരം *arg, ഘടന argp_state *state)
{
  തിരഞ്ഞെടുപ്പ് (താക്കോൽ)
    {
    അവസ്ഥ 'b':
      bundle = argp_mandatory_argument (arg, state);
      നിർത്തുക;

    അവസ്ഥ 'f':
      config_file = argp_mandatory_argument (arg, state);
      നിർത്തുക;

    അവസ്ഥ OPTION_CONSOLE_SOCKET:
      libcrun_context_set_console_socket (crun_context, argp_mandatory_argument (arg, state));
      നിർത്തുക;

    അവസ്ഥ OPTION_PRESERVE_FDS:
      libcrun_context_set_preserve_fds (crun_context, parse_id_or_fail (argp_mandatory_argument (arg, state), ശൂന്യ, "preserve-fds"));
      നിർത്തുക;

    അവസ്ഥ OPTION_NO_SUBREAPER:
      നിർത്തുക;

    അവസ്ഥ OPTION_NO_PIVOT:
      libcrun_context_set_no_pivot (crun_context, true);
      നിർത്തുക;

    അവസ്ഥ OPTION_NO_NEW_KEYRING:
      libcrun_context_set_no_new_keyring (crun_context, true);
      നിർത്തുക;

    അവസ്ഥ OPTION_PID_FILE:
      libcrun_context_set_pid_file (crun_context, argp_mandatory_argument (arg, state));
      നിർത്തുക;

    അവസ്ഥ ARGP_താക്കോൽ_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    സ്ഥിരസ്ഥിതി:
      തിരികെ ARGP_ERR_UNKNOWN;
    }

  തിരികെ 0;
}

സ്ഥിതം ഘടന argp run_argp = { options, parse_opt, args_doc, doc, ശൂന്യ, ശൂന്യ, ശൂന്യ };

സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
get_options ()
{
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
crun_command_create (ഘടന crun_global_arguments *global_args, പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  crun_context = new_libcrun_context (global_args);
  ret = crun_run_create_internal (global_args, argc, argv, libcrun_container_create, get_options, crun_context, &run_argp, &config_file, &bundle, err);
  libcrun_context_free (crun_context);
  തിരികെ ret;
}
