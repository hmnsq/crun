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

#شامل <config.h>
#شامل <argp.h>

#شامل "crun.h"
#شامل "run_create.h"

شمار
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS,
  OPTION_NO_محور
};

ساکن ثابت حرف *bundle = تہی;

ساکن libcrun_context_t *crun_context;

ساکن ساخت argp_option options[]
    = { { "bundle", 'b', "DIR", 0, "container bundle (default \".\")", 0 },
        { "config", 'f', "FILE", 0, "override the config file name", 0 },
        { "console-socket", OPTION_CONSOLE_SOCKET, "SOCK", 0,
          "path to a socket that will receive the ptmx end of the tty", 0 },
        { "preserve-fds", OPTION_PRESERVE_FDS, "N", 0, "pass additional FDs to the container", 0 },
        { "no-pivot", OPTION_NO_محور, 0, 0, "do not use pivot_root", 0 },
        { "pid-file", OPTION_PID_FILE, "FILE", 0, "where to write the PID of the container", 0 },
        { "no-subreaper", OPTION_NO_SUBREAPER, 0, 0, "do not create a subreaper process (ignored)", 0 },
        { "no-new-keyring", OPTION_NO_NEW_KEYRING, 0, 0, "keep the same session key", 0 },
        {
            0,
        } };

ساکن حرف doc[] = "OCI runtime";

ساکن حرف دلائل_doc[] = "create [OPTION]... CONTAINER";

ساکن ثابت حرف *config_file = "config.json";

ساکن error_t
parse_opt (صحیح کلید, حرف *arg, ساخت argp_state *state)
{
  انتخاب (کلید)
    {
    حالت 'b':
      bundle = argp_mandatory_دلیل (arg, state);
      توڑو;

    حالت 'f':
      config_file = argp_mandatory_دلیل (arg, state);
      توڑو;

    حالت OPTION_CONSOLE_SOCKET:
      libcrun_context_سیٹ_console_socket (crun_context, argp_mandatory_دلیل (arg, state));
      توڑو;

    حالت OPTION_PRESERVE_FDS:
      libcrun_context_سیٹ_preserve_fds (crun_context, parse_id_or_fail (argp_mandatory_دلیل (arg, state), تہی, "preserve-fds"));
      توڑو;

    حالت OPTION_NO_SUBREAPER:
      توڑو;

    حالت OPTION_NO_محور:
      libcrun_context_سیٹ_no_محور (crun_context, true);
      توڑو;

    حالت OPTION_NO_NEW_KEYRING:
      libcrun_context_سیٹ_no_new_keyring (crun_context, true);
      توڑو;

    حالت OPTION_PID_FILE:
      libcrun_context_سیٹ_pid_file (crun_context, argp_mandatory_دلیل (arg, state));
      توڑو;

    حالت ARGP_کلید_NO_دلائل:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    طےشدہ:
      واپس ARGP_ERR_UNKNOWN;
    }

  واپس 0;
}

ساکن ساخت argp تسلسل_argp = { options, parse_opt, دلائل_doc, doc, تہی, تہی, تہی };

ساکن غیرعلامتی صحیح
get_options ()
{
  واپس 0;
}

صحیح
crun_command_create (ساخت crun_global_arguments *global_دلائل, صحیح argc, حرف **argv, libcrun_error_t *err)
{
  صحیح ret;

  crun_context = new_libcrun_context (global_دلائل);
  ret = crun_تسلسل_create_اندرونی (global_دلائل, argc, argv, libcrun_container_create, get_options, crun_context, &تسلسل_argp, &config_file, &bundle, err);
  libcrun_context_free (crun_context);
  واپس ret;
}
