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

#ضمن <config.h>
#ضمن <argp.h>

#ضمن "crun.h"
#ضمن "run_create.h"

ساكن حرف doc[] = "OCI runtime";

تعداد
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS,
  OPTION_NO_PIVOT,
  OPTION_KEEP,
};

ساكن ثابت حرف *bundle = عدم;

ساكن bool keep = false;

ساكن libcrun_context_t *crun_context;

ساكن بنية argp_option options[]
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

ساكن حرف args_doc[] = "run [OPTION]... CONTAINER";

ساكن ثابت حرف *config_file = "config.json";

ساكن error_t
parse_opt (صحيح کونچي, حرف *arg, بنية argp_state *state)
{
  بدل (کونچي)
    {
    حالة 'd':
      libcrun_context_set_detach (crun_context, true);
      اكسر;

    حالة 'f':
      config_file = argp_mandatory_argument (arg, state);
      اكسر;

    حالة 'b':
      bundle = argp_mandatory_argument (arg, state);
      اكسر;

    حالة OPTION_KEEP:
      keep = true;
      اكسر;

    حالة OPTION_CONSOLE_SOCKET:
      libcrun_context_set_console_socket (crun_context, argp_mandatory_argument (arg, state));
      اكسر;

    حالة OPTION_PRESERVE_FDS:
      libcrun_context_set_preserve_fds (crun_context, parse_id_or_fail (argp_mandatory_argument (arg, state), عدم, "preserve-fds"));
      اكسر;

    حالة OPTION_NO_SUBREAPER:
      اكسر;

    حالة OPTION_NO_NEW_KEYRING:
      libcrun_context_set_no_new_keyring (crun_context, true);
      اكسر;

    حالة OPTION_PID_FILE:
      libcrun_context_set_pid_file (crun_context, argp_mandatory_argument (arg, state));
      اكسر;

    حالة OPTION_NO_PIVOT:
      libcrun_context_set_no_pivot (crun_context, true);
      اكسر;

    حالة ARGP_کونچي_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    افتراضي:
      أرجع ARGP_ERR_UNKNOWN;
    }

  أرجع 0;
}

ساكن بنية argp run_argp = { options, parse_opt, args_doc, doc, عدم, عدم, عدم };

ساكن unsigned صحيح
get_options ()
{
  أرجع keep ? LIBCRUN_RUN_OPTIONS_KEEP : 0;
}

صحيح
crun_command_run (بنية crun_global_arguments *global_args, صحيح argc, حرف **argv, libcrun_error_t *err)
{
  صحيح ret;

  crun_context = new_libcrun_context (global_args);
  ret = crun_run_create_internal (global_args, argc, argv, libcrun_container_run, get_options, crun_context, &run_argp, &config_file, &bundle, err);
  libcrun_context_free (crun_context);
  أرجع ret;
}
