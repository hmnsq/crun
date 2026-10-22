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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <argp.h>

#ಅನ್ತರ್ಭಾವ "crun.h"
#ಅನ್ತರ್ಭಾವ "run_create.h"

ಗಣನಾ
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS,
  OPTION_NO_PIVOT
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *bundle = ರಿಕ್ತ;

ಸ್ಥಿರ libcrun_context_t *crun_context;

ಸ್ಥಿರ ಸಂರಚನಾ argp_option options[]
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

ಸ್ಥಿರ ವರ್ಣ doc[] = "OCI runtime";

ಸ್ಥಿರ ವರ್ಣ args_doc[] = "create [OPTION]... CONTAINER";

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *config_file = "config.json";

ಸ್ಥಿರ error_t
parse_opt (ಪೂರ್ಣಾಙ್ಕ ಕೀಲಿ, ವರ್ಣ *arg, ಸಂರಚನಾ argp_state *state)
{
  ವಿಕಲ್ಪ (ಕೀಲಿ)
    {
    ಪಕ್ಷ 'b':
      bundle = argp_mandatory_argument (arg, state);
      ಭಙ್ಗ;

    ಪಕ್ಷ 'f':
      config_file = argp_mandatory_argument (arg, state);
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_CONSOLE_SOCKET:
      libcrun_context_set_console_socket (crun_context, argp_mandatory_argument (arg, state));
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_PRESERVE_FDS:
      libcrun_context_set_preserve_fds (crun_context, parse_id_or_fail (argp_mandatory_argument (arg, state), ರಿಕ್ತ, "preserve-fds"));
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_NO_SUBREAPER:
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_NO_PIVOT:
      libcrun_context_set_no_pivot (crun_context, true);
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_NO_NEW_KEYRING:
      libcrun_context_set_no_new_keyring (crun_context, true);
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_PID_FILE:
      libcrun_context_set_pid_file (crun_context, argp_mandatory_argument (arg, state));
      ಭಙ್ಗ;

    ಪಕ್ಷ ARGP_ಕೀಲಿ_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ ARGP_ERR_UNKNOWN;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಸಂರಚನಾ argp run_argp = { options, parse_opt, args_doc, doc, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ };

ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ
get_options ()
{
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
crun_command_create (ಸಂರಚನಾ crun_global_arguments *global_args, ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  crun_context = new_libcrun_context (global_args);
  ret = crun_run_create_internal (global_args, argc, argv, libcrun_container_create, get_options, crun_context, &run_argp, &config_file, &bundle, err);
  libcrun_context_free (crun_context);
  ಪ್ರತ್ಯಾಗಮ ret;
}
