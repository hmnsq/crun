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

#అన్తర్భావ <config.h>
#అన్తర్భావ <argp.h>

#అన్తర్భావ "crun.h"
#అన్తర్భావ "run_create.h"

గణన
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS,
  OPTION_NO_ఇరుసు
};

స్థిర స్థిరరాశి అక్షరం *bundle = ఖాళీ;

స్థిర libcrun_context_t *crun_context;

స్థిర నిర్మాణం argp_option options[]
    = { { "bundle", 'b', "DIR", 0, "container bundle (default \".\")", 0 },
        { "config", 'f', "FILE", 0, "override the config file name", 0 },
        { "console-socket", OPTION_CONSOLE_SOCKET, "SOCK", 0,
          "path to a socket that will receive the ptmx end of the tty", 0 },
        { "preserve-fds", OPTION_PRESERVE_FDS, "N", 0, "pass additional FDs to the container", 0 },
        { "no-pivot", OPTION_NO_ఇరుసు, 0, 0, "do not use pivot_root", 0 },
        { "pid-file", OPTION_PID_FILE, "FILE", 0, "where to write the PID of the container", 0 },
        { "no-subreaper", OPTION_NO_SUBREAPER, 0, 0, "do not create a subreaper process (ignored)", 0 },
        { "no-new-keyring", OPTION_NO_NEW_KEYRING, 0, 0, "keep the same session key", 0 },
        {
            0,
        } };

స్థిర అక్షరం doc[] = "OCI runtime";

స్థిర అక్షరం వాదనలు_doc[] = "create [OPTION]... CONTAINER";

స్థిర స్థిరరాశి అక్షరం *config_file = "config.json";

స్థిర error_t
parse_opt (పూర్ణాంకం కీలకం, అక్షరం *arg, నిర్మాణం argp_state *state)
{
  ఎంపిక (కీలకం)
    {
    స్థితి 'b':
      bundle = argp_mandatory_వాదం (arg, state);
      ఆపు;

    స్థితి 'f':
      config_file = argp_mandatory_వాదం (arg, state);
      ఆపు;

    స్థితి OPTION_CONSOLE_SOCKET:
      libcrun_context_సమితి_console_socket (crun_context, argp_mandatory_వాదం (arg, state));
      ఆపు;

    స్థితి OPTION_PRESERVE_FDS:
      libcrun_context_సమితి_preserve_fds (crun_context, parse_id_or_fail (argp_mandatory_వాదం (arg, state), ఖాళీ, "preserve-fds"));
      ఆపు;

    స్థితి OPTION_NO_SUBREAPER:
      ఆపు;

    స్థితి OPTION_NO_ఇరుసు:
      libcrun_context_సమితి_no_ఇరుసు (crun_context, true);
      ఆపు;

    స్థితి OPTION_NO_NEW_KEYRING:
      libcrun_context_సమితి_no_new_keyring (crun_context, true);
      ఆపు;

    స్థితి OPTION_PID_FILE:
      libcrun_context_సమితి_pid_file (crun_context, argp_mandatory_వాదం (arg, state));
      ఆపు;

    స్థితి ARGP_కీలకం_NO_వాదనలు:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    అప్రమేయ:
      తిరిగి ARGP_ERR_UNKNOWN;
    }

  తిరిగి 0;
}

స్థిర నిర్మాణం argp నిరంతరం_argp = { options, parse_opt, వాదనలు_doc, doc, ఖాళీ, ఖాళీ, ఖాళీ };

స్థిర గుర్తులేని పూర్ణాంకం
get_options ()
{
  తిరిగి 0;
}

పూర్ణాంకం
crun_command_create (నిర్మాణం crun_global_arguments *global_వాదనలు, పూర్ణాంకం argc, అక్షరం **argv, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  crun_context = new_libcrun_context (global_వాదనలు);
  ret = crun_నిరంతరం_create_అంతర్గత (global_వాదనలు, argc, argv, libcrun_container_create, get_options, crun_context, &నిరంతరం_argp, &config_file, &bundle, err);
  libcrun_context_free (crun_context);
  తిరిగి ret;
}
