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

ಸ್ಥಿರ ಅಕ್ಷರ doc[] = "OCI runtime";

ಎಣಿಕೆ
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS,
  OPTION_NO_ಅಚ್ಚು,
  OPTION_KEEP,
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *bundle = ಖಾಲಿ;

ಸ್ಥಿರ bool keep = false;

ಸ್ಥಿರ libcrun_context_t *crun_context;

ಸ್ಥಿರ ರಚನೆ argp_option options[]
    = { { "bundle", 'b', "DIR", 0, "container bundle (default \".\")", 0 },
        { "config", 'f', "FILE", 0, "override the config file name", 0 },
        { "detach", 'd', 0, 0, "detach from the parent", 0 },
        { "console-socket", OPTION_CONSOLE_SOCKET, "SOCKET", 0,
          "path to a socket that will receive the ptmx end of the tty", 0 },
        { "preserve-fds", OPTION_PRESERVE_FDS, "N", 0, "pass additional FDs to the container", 0 },
        { "pid-file", OPTION_PID_FILE, "FILE", 0, "where to write the PID of the container", 0 },
        { "keep", OPTION_KEEP, 0, 0, "do not delete the container after it exits", 0 },
        { "no-subreaper", OPTION_NO_SUBREAPER, 0, 0, "do not create a subreaper process (ignored)", 0 },
        { "no-new-keyring", OPTION_NO_NEW_KEYRING, 0, 0, "keep the same session key", 0 },
        { "no-pivot", OPTION_NO_ಅಚ್ಚು, 0, 0, "do not use pivot_root", 0 },
        {
            0,
        } };

ಸ್ಥಿರ ಅಕ್ಷರ ವಾದಗಳು_doc[] = "run [OPTION]... CONTAINER";

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *config_file = "config.json";

ಸ್ಥಿರ error_t
parse_opt (ಪೂರ್ಣಾಂಕ ಕೀಲಿ, ಅಕ್ಷರ *arg, ರಚನೆ argp_state *state)
{
  ಆಯ್ಕೆ (ಕೀಲಿ)
    {
    ಸ್ಥಿತಿ 'd':
      libcrun_context_ಗಣ_detach (crun_context, true);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'f':
      config_file = argp_mandatory_ವಾದ (arg, state);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'b':
      bundle = argp_mandatory_ವಾದ (arg, state);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_KEEP:
      keep = true;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_CONSOLE_SOCKET:
      libcrun_context_ಗಣ_console_socket (crun_context, argp_mandatory_ವಾದ (arg, state));
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_PRESERVE_FDS:
      libcrun_context_ಗಣ_preserve_fds (crun_context, parse_id_or_fail (argp_mandatory_ವಾದ (arg, state), ಖಾಲಿ, "preserve-fds"));
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_NO_SUBREAPER:
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_NO_NEW_KEYRING:
      libcrun_context_ಗಣ_no_new_keyring (crun_context, true);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_PID_FILE:
      libcrun_context_ಗಣ_pid_file (crun_context, argp_mandatory_ವಾದ (arg, state));
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_NO_ಅಚ್ಚು:
      libcrun_context_ಗಣ_no_ಅಚ್ಚು (crun_context, true);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ARGP_ಕೀಲಿ_NO_ವಾದಗಳು:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ ARGP_ERR_UNKNOWN;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ರಚನೆ argp ಸತತ_argp = { options, parse_opt, ವಾದಗಳು_doc, doc, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ };

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ
get_options ()
{
  ಹಿಂತಿರುಗಿ keep ? LIBCRUN_ಸತತ_OPTIONS_KEEP : 0;
}

ಪೂರ್ಣಾಂಕ
crun_command_ಸತತ (ರಚನೆ crun_global_arguments *global_ವಾದಗಳು, ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  crun_context = new_libcrun_context (global_ವಾದಗಳು);
  ret = crun_ಸತತ_create_ಆಂತರಿಕ (global_ವಾದಗಳು, argc, argv, libcrun_container_ಸತತ, get_options, crun_context, &ಸತತ_argp, &config_file, &bundle, err);
  libcrun_context_free (crun_context);
  ಹಿಂತಿರುಗಿ ret;
}
