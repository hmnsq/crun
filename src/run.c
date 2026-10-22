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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <argp.h>

#અન્તર્ભાવ "crun.h"
#અન્તર્ભાવ "run_create.h"

સ્થિર અક્ષર doc[] = "OCI runtime";

ગણના
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS,
  OPTION_NO_PIVOT,
  OPTION_KEEP,
};

સ્થિર અચલ અક્ષર *bundle = ખાલી;

સ્થિર bool keep = false;

સ્થિર libcrun_context_t *crun_context;

સ્થિર સંરચના argp_option options[]
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
        { "no-pivot", OPTION_NO_PIVOT, 0, 0, "do not use pivot_root", 0 },
        {
            0,
        } };

સ્થિર અક્ષર args_doc[] = "run [OPTION]... CONTAINER";

સ્થિર અચલ અક્ષર *config_file = "config.json";

સ્થિર error_t
parse_opt (પૂર્ણાંક ચાવી, અક્ષર *arg, સંરચના argp_state *state)
{
  પસંદગી (ચાવી)
    {
    સ્થિતિ 'd':
      libcrun_context_set_detach (crun_context, true);
      તોડો;

    સ્થિતિ 'f':
      config_file = argp_mandatory_argument (arg, state);
      તોડો;

    સ્થિતિ 'b':
      bundle = argp_mandatory_argument (arg, state);
      તોડો;

    સ્થિતિ OPTION_KEEP:
      keep = true;
      તોડો;

    સ્થિતિ OPTION_CONSOLE_SOCKET:
      libcrun_context_set_console_socket (crun_context, argp_mandatory_argument (arg, state));
      તોડો;

    સ્થિતિ OPTION_PRESERVE_FDS:
      libcrun_context_set_preserve_fds (crun_context, parse_id_or_fail (argp_mandatory_argument (arg, state), ખાલી, "preserve-fds"));
      તોડો;

    સ્થિતિ OPTION_NO_SUBREAPER:
      તોડો;

    સ્થિતિ OPTION_NO_NEW_KEYRING:
      libcrun_context_set_no_new_keyring (crun_context, true);
      તોડો;

    સ્થિતિ OPTION_PID_FILE:
      libcrun_context_set_pid_file (crun_context, argp_mandatory_argument (arg, state));
      તોડો;

    સ્થિતિ OPTION_NO_PIVOT:
      libcrun_context_set_no_pivot (crun_context, true);
      તોડો;

    સ્થિતિ ARGP_ચાવી_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    મૂળભૂત:
      પાછા ARGP_ERR_UNKNOWN;
    }

  પાછા 0;
}

સ્થિર સંરચના argp run_argp = { options, parse_opt, args_doc, doc, ખાલી, ખાલી, ખાલી };

સ્થિર unsigned પૂર્ણાંક
get_options ()
{
  પાછા keep ? LIBCRUN_RUN_OPTIONS_KEEP : 0;
}

પૂર્ણાંક
crun_command_run (સંરચના crun_global_arguments *global_args, પૂર્ણાંક argc, અક્ષર **argv, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  crun_context = new_libcrun_context (global_args);
  ret = crun_run_create_internal (global_args, argc, argv, libcrun_container_run, get_options, crun_context, &run_argp, &config_file, &bundle, err);
  libcrun_context_free (crun_context);
  પાછા ret;
}
