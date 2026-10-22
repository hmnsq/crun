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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <argp.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "crun.h"

ਸਥਿਰ ਅੱਖਰ doc[] = "OCI runtime";

ਗਿਣਤੀ
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

ਬਣਤਰ ps_options_s
{
  ਪੂਰਨ format;
};

ਗਿਣਤੀ
{
  PS_ਸਾਰਣੀ = 100,
  PS_JSON,
};

ਸਥਿਰ ਬਣਤਰ ps_options_s ps_options;

ਸਥਿਰ ਬਣਤਰ argp_option options[] = { { "format", 'f', "FORMAT", 0, "select the output format", 0 },
                                        {
                                            0,
                                        } };

ਸਥਿਰ ਅੱਖਰ ਦਲੀਲਾਂ_doc[] = "ps";

ਸਥਿਰ error_t
parse_opt (ਪੂਰਨ ਕੁੰਜੀ, ਅੱਖਰ *arg, ਬਣਤਰ argp_state *state arg_ਅਣਵਰਤਿਆ)
{
  ਚੋਣ (ਕੁੰਜੀ)
    {
    ਹਾਲਤ 'f':
      ਜੇ (strcmp (arg, "table") == 0)
        ps_options.format = PS_ਸਾਰਣੀ;
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (arg, "json") == 0)
        ps_options.format = PS_JSON;
      ਨਹੀਂਤਾਂ
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      ਰੋਕੋ;

    ਮੂਲ:
      ਵਾਪਸ ARGP_ERR_UNKNOWN;
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਬਣਤਰ argp ਲੜੀਵਾਰ_argp = { options, parse_opt, ਦਲੀਲਾਂ_doc, doc, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ };

ਪੂਰਨ
crun_command_ps (ਬਣਤਰ crun_global_arguments *global_ਦਲੀਲਾਂ, ਪੂਰਨ argc, ਅੱਖਰ **argv, libcrun_error_t *err)
{
  ਪੂਰਨ ਪਹਿਲਾ_arg;
  ਪੂਰਨ ret;
  cleanup_free pid_t *pids = ਨਿਰਾ;
  cleanup_context libcrun_context_t *crun_context = ਨਿਰਾ;
  size_t i;

  ps_options.format = PS_ਸਾਰਣੀ;

  argp_parse (&ਲੜੀਵਾਰ_argp, argc, argv, ARGP_ਵਿੱਚ_ਕ੍ਰਮ, &ਪਹਿਲਾ_arg, &ps_options);
  crun_assert_n_ਦਲੀਲਾਂ (argc - ਪਹਿਲਾ_arg, 1, 1);

  crun_context = new_libcrun_context (global_ਦਲੀਲਾਂ);

  ret = init_libcrun_context (crun_context, argv[ਪਹਿਲਾ_arg], global_ਦਲੀਲਾਂ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_container_read_pids (crun_context, argv[ਪਹਿਲਾ_arg], true, &pids, err);
  ਜੇ (UNLIKELY (ret < 0))
    {
      libcrun_error_report_and_release (err);
      ਵਾਪਸ ret;
    }

  ਚੋਣ (ps_options.format)
    {
    ਹਾਲਤ PS_JSON:
      ਲਿਖੋ ("[\n");
      ਲਈ (i = 0; pids && pids[i]; i++)
        ਲਿਖੋ ("  %d%s\n", pids[i], pids[i + 1] ? "," : "");
      ਲਿਖੋ ("]\n");
      ਰੋਕੋ;

    ਹਾਲਤ PS_ਸਾਰਣੀ:
      ਲਿਖੋ ("PID\n");
      ਲਈ (i = 0; pids && pids[i]; i++)
        ਲਿਖੋ ("%d\n", pids[i]);
      ਰੋਕੋ;
    }

  ਵਾਪਸ 0;
}
