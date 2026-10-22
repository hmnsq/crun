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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <argp.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <errno.h>

#ಅನ್ತರ್ಭಾವ "crun.h"

ಸ್ಥಿರ ವರ್ಣ doc[] = "OCI runtime";

ಗಣನಾ
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

ಸಂರಚನಾ ps_options_s
{
  ಪೂರ್ಣಾಙ್ಕ format;
};

ಗಣನಾ
{
  PS_TABLE = 100,
  PS_JSON,
};

ಸ್ಥಿರ ಸಂರಚನಾ ps_options_s ps_options;

ಸ್ಥಿರ ಸಂರಚನಾ argp_option options[] = { { "format", 'f', "FORMAT", 0, "select the output format", 0 },
                                        {
                                            0,
                                        } };

ಸ್ಥಿರ ವರ್ಣ args_doc[] = "ps";

ಸ್ಥಿರ error_t
parse_opt (ಪೂರ್ಣಾಙ್ಕ ಕೀಲಿ, ವರ್ಣ *arg, ಸಂರಚನಾ argp_state *state arg_unused)
{
  ವಿಕಲ್ಪ (ಕೀಲಿ)
    {
    ಪಕ್ಷ 'f':
      ಯದಿ (strcmp (arg, "table") == 0)
        ps_options.format = PS_TABLE;
      ಅನ್ಯಥಾ ಯದಿ (strcmp (arg, "json") == 0)
        ps_options.format = PS_JSON;
      ಅನ್ಯಥಾ
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      ಭಙ್ಗ;

    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ ARGP_ERR_UNKNOWN;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಸಂರಚನಾ argp run_argp = { options, parse_opt, args_doc, doc, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ };

ಪೂರ್ಣಾಙ್ಕ
crun_command_ps (ಸಂರಚನಾ crun_global_arguments *global_args, ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ first_arg;
  ಪೂರ್ಣಾಙ್ಕ ret;
  cleanup_free pid_t *pids = ರಿಕ್ತ;
  cleanup_context libcrun_context_t *crun_context = ರಿಕ್ತ;
  size_t i;

  ps_options.format = PS_TABLE;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &ps_options);
  crun_assert_n_args (argc - first_arg, 1, 1);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_container_read_pids (crun_context, argv[first_arg], true, &pids, err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      libcrun_error_report_and_release (err);
      ಪ್ರತ್ಯಾಗಮ ret;
    }

  ವಿಕಲ್ಪ (ps_options.format)
    {
    ಪಕ್ಷ PS_JSON:
      printf ("[\n");
      ಪ್ರತಿ (i = 0; pids && pids[i]; i++)
        printf ("  %d%s\n", pids[i], pids[i + 1] ? "," : "");
      printf ("]\n");
      ಭಙ್ಗ;

    ಪಕ್ಷ PS_TABLE:
      printf ("PID\n");
      ಪ್ರತಿ (i = 0; pids && pids[i]; i++)
        printf ("%d\n", pids[i]);
      ಭಙ್ಗ;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}
