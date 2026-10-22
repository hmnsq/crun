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

ಸ್ಥಿರ ಅಕ್ಷರ doc[] = "OCI runtime";

ಎಣಿಕೆ
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

ರಚನೆ ps_options_s
{
  ಪೂರ್ಣಾಂಕ format;
};

ಎಣಿಕೆ
{
  PS_ಕೋಷ್ಟಕ = 100,
  PS_JSON,
};

ಸ್ಥಿರ ರಚನೆ ps_options_s ps_options;

ಸ್ಥಿರ ರಚನೆ argp_option options[] = { { "format", 'f', "FORMAT", 0, "select the output format", 0 },
                                        {
                                            0,
                                        } };

ಸ್ಥಿರ ಅಕ್ಷರ ವಾದಗಳು_doc[] = "ps";

ಸ್ಥಿರ error_t
parse_opt (ಪೂರ್ಣಾಂಕ ಕೀಲಿ, ಅಕ್ಷರ *arg, ರಚನೆ argp_state *state arg_ಬಳಸದ)
{
  ಆಯ್ಕೆ (ಕೀಲಿ)
    {
    ಸ್ಥಿತಿ 'f':
      ಆದರೆ (strcmp (arg, "table") == 0)
        ps_options.format = PS_ಕೋಷ್ಟಕ;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (arg, "json") == 0)
        ps_options.format = PS_JSON;
      ಇಲ್ಲದಿದ್ದರೆ
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ ARGP_ERR_UNKNOWN;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ರಚನೆ argp ಸತತ_argp = { options, parse_opt, ವಾದಗಳು_doc, doc, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ };

ಪೂರ್ಣಾಂಕ
crun_command_ps (ರಚನೆ crun_global_arguments *global_ವಾದಗಳು, ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ಮೊದಲ_arg;
  ಪೂರ್ಣಾಂಕ ret;
  cleanup_free pid_t *pids = ಖಾಲಿ;
  cleanup_context libcrun_context_t *crun_context = ಖಾಲಿ;
  size_t i;

  ps_options.format = PS_ಕೋಷ್ಟಕ;

  argp_parse (&ಸತತ_argp, argc, argv, ARGP_ಒಳ_ಕ್ರಮ, &ಮೊದಲ_arg, &ps_options);
  crun_assert_n_ವಾದಗಳು (argc - ಮೊದಲ_arg, 1, 1);

  crun_context = new_libcrun_context (global_ವಾದಗಳು);

  ret = init_libcrun_context (crun_context, argv[ಮೊದಲ_arg], global_ವಾದಗಳು, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_container_read_pids (crun_context, argv[ಮೊದಲ_arg], true, &pids, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      libcrun_error_report_and_release (err);
      ಹಿಂತಿರುಗಿ ret;
    }

  ಆಯ್ಕೆ (ps_options.format)
    {
    ಸ್ಥಿತಿ PS_JSON:
      ಬರೆ ("[\n");
      ಗಾಗಿ (i = 0; pids && pids[i]; i++)
        ಬರೆ ("  %d%s\n", pids[i], pids[i + 1] ? "," : "");
      ಬರೆ ("]\n");
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ PS_ಕೋಷ್ಟಕ:
      ಬರೆ ("PID\n");
      ಗಾಗಿ (i = 0; pids && pids[i]; i++)
        ಬರೆ ("%d\n", pids[i]);
      ನಿಲ್ಲಿಸು;
    }

  ಹಿಂತಿರುಗಿ 0;
}
