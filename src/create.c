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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <argp.h>

#ଅନ୍ତର୍ଭାଵ "crun.h"
#ଅନ୍ତର୍ଭାଵ "run_create.h"

ଗଣନା
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS,
  OPTION_NO_PIVOT
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *bundle = ଖାଲି;

ସ୍ଥିର libcrun_context_t *crun_context;

ସ୍ଥିର ସଂରଚନା argp_option options[]
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

ସ୍ଥିର ଅକ୍ଷର doc[] = "OCI runtime";

ସ୍ଥିର ଅକ୍ଷର args_doc[] = "create [OPTION]... CONTAINER";

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *config_file = "config.json";

ସ୍ଥିର error_t
parse_opt (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଚାବି, ଅକ୍ଷର *arg, ସଂରଚନା argp_state *state)
{
  ବାଛ (ଚାବି)
    {
    ସ୍ଥିତି 'b':
      bundle = argp_mandatory_argument (arg, state);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 'f':
      config_file = argp_mandatory_argument (arg, state);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_CONSOLE_SOCKET:
      libcrun_context_set_console_socket (crun_context, argp_mandatory_argument (arg, state));
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_PRESERVE_FDS:
      libcrun_context_set_preserve_fds (crun_context, parse_id_or_fail (argp_mandatory_argument (arg, state), ଖାଲି, "preserve-fds"));
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_NO_SUBREAPER:
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_NO_PIVOT:
      libcrun_context_set_no_pivot (crun_context, true);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_NO_NEW_KEYRING:
      libcrun_context_set_no_new_keyring (crun_context, true);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_PID_FILE:
      libcrun_context_set_pid_file (crun_context, argp_mandatory_argument (arg, state));
      ଭାଙ୍ଗ;

    ସ୍ଥିତି ARGP_ଚାବି_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ ARGP_ERR_UNKNOWN;
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ସଂରଚନା argp run_argp = { options, parse_opt, args_doc, doc, ଖାଲି, ଖାଲି, ଖାଲି };

ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_options ()
{
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
crun_command_create (ସଂରଚନା crun_global_arguments *global_args, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  crun_context = new_libcrun_context (global_args);
  ret = crun_run_create_internal (global_args, argc, argv, libcrun_container_create, get_options, crun_context, &run_argp, &config_file, &bundle, err);
  libcrun_context_free (crun_context);
  ଫେରନ୍ତୁ ret;
}
