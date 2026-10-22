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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <argp.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "crun.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run_create.h"

ਸਥਿਰ ਅੱਖਰ doc[] = "OCI runtime";

ਗਿਣਤੀ
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS,
  OPTION_NO_ਧੁਰਾ,
  OPTION_KEEP,
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *bundle = ਨਿਰਾ;

ਸਥਿਰ bool keep = false;

ਸਥਿਰ libcrun_context_t *crun_context;

ਸਥਿਰ ਬਣਤਰ argp_option options[]
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
        { "no-pivot", OPTION_NO_ਧੁਰਾ, 0, 0, "do not use pivot_root", 0 },
        {
            0,
        } };

ਸਥਿਰ ਅੱਖਰ ਦਲੀਲਾਂ_doc[] = "run [OPTION]... CONTAINER";

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *config_file = "config.json";

ਸਥਿਰ error_t
parse_opt (ਪੂਰਨ ਕੁੰਜੀ, ਅੱਖਰ *arg, ਬਣਤਰ argp_state *state)
{
  ਚੋਣ (ਕੁੰਜੀ)
    {
    ਹਾਲਤ 'd':
      libcrun_context_ਸੈੱਟ_detach (crun_context, true);
      ਰੋਕੋ;

    ਹਾਲਤ 'f':
      config_file = argp_mandatory_ਦਲੀਲ (arg, state);
      ਰੋਕੋ;

    ਹਾਲਤ 'b':
      bundle = argp_mandatory_ਦਲੀਲ (arg, state);
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_KEEP:
      keep = true;
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_CONSOLE_SOCKET:
      libcrun_context_ਸੈੱਟ_console_socket (crun_context, argp_mandatory_ਦਲੀਲ (arg, state));
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_PRESERVE_FDS:
      libcrun_context_ਸੈੱਟ_preserve_fds (crun_context, parse_id_or_fail (argp_mandatory_ਦਲੀਲ (arg, state), ਨਿਰਾ, "preserve-fds"));
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_NO_SUBREAPER:
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_NO_NEW_KEYRING:
      libcrun_context_ਸੈੱਟ_no_new_keyring (crun_context, true);
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_PID_FILE:
      libcrun_context_ਸੈੱਟ_pid_file (crun_context, argp_mandatory_ਦਲੀਲ (arg, state));
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_NO_ਧੁਰਾ:
      libcrun_context_ਸੈੱਟ_no_ਧੁਰਾ (crun_context, true);
      ਰੋਕੋ;

    ਹਾਲਤ ARGP_ਕੁੰਜੀ_NO_ਦਲੀਲਾਂ:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    ਮੂਲ:
      ਵਾਪਸ ARGP_ERR_UNKNOWN;
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਬਣਤਰ argp ਲੜੀਵਾਰ_argp = { options, parse_opt, ਦਲੀਲਾਂ_doc, doc, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ };

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
get_options ()
{
  ਵਾਪਸ keep ? LIBCRUN_ਲੜੀਵਾਰ_OPTIONS_KEEP : 0;
}

ਪੂਰਨ
crun_command_ਲੜੀਵਾਰ (ਬਣਤਰ crun_global_arguments *global_ਦਲੀਲਾਂ, ਪੂਰਨ argc, ਅੱਖਰ **argv, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  crun_context = new_libcrun_context (global_ਦਲੀਲਾਂ);
  ret = crun_ਲੜੀਵਾਰ_create_ਅੰਦਰੂਨੀ (global_ਦਲੀਲਾਂ, argc, argv, libcrun_container_ਲੜੀਵਾਰ, get_options, crun_context, &ਲੜੀਵਾਰ_argp, &config_file, &bundle, err);
  libcrun_context_free (crun_context);
  ਵਾਪਸ ret;
}
