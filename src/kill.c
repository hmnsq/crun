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
#ಅನ್ತರ್ಭಾವ <signal.h>
#ಅನ್ತರ್ಭಾವ <regex.h>

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

ರಚನೆ kill_options_s
{
  bool all;
  bool regex;
};

ಸ್ಥಿರ ರಚನೆ kill_options_s kill_options;

ಸ್ಥಿರ ರಚನೆ argp_option options[]
    = { { "all", 'a', 0, 0, "kill all the processes", 0 },
        { "regex", 'r', 0, 0, "the specified CONTAINER is a regular expression (kill multiple containers)", 0 },
        {
            0,
        } };

ಸ್ಥಿರ ಅಕ್ಷರ ವಾದಗಳು_doc[] = "kill CONTAINER [SIGNAL]";

ಸ್ಥಿರ error_t
parse_opt (ಪೂರ್ಣಾಂಕ ಕೀಲಿ, ಅಕ್ಷರ *arg arg_ಬಳಸದ, ರಚನೆ argp_state *state arg_ಬಳಸದ)
{
  ಆಯ್ಕೆ (ಕೀಲಿ)
    {
    ಸ್ಥಿತಿ 'a':
      kill_options.all = true;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'r':
      kill_options.regex = true;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ARGP_ಕೀಲಿ_NO_ವಾದಗಳು:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ ARGP_ERR_UNKNOWN;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ರಚನೆ argp ಸತತ_argp = { options, parse_opt, ವಾದಗಳು_doc, doc, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ };

ಪೂರ್ಣಾಂಕ
crun_command_kill (ರಚನೆ crun_global_arguments *global_ವಾದಗಳು, ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ಮೊದಲ_arg = 0, ret;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂಕೇತ;
  cleanup_context libcrun_context_t *crun_context = ಖಾಲಿ;

  argp_parse (&ಸತತ_argp, argc, argv, ARGP_ಒಳ_ಕ್ರಮ, &ಮೊದಲ_arg, &kill_options);
  crun_assert_n_ವಾದಗಳು (argc - ಮೊದಲ_arg, 1, 2);

  crun_context = new_libcrun_context (global_ವಾದಗಳು);

  ret = init_libcrun_context (crun_context, argv[ಮೊದಲ_arg], global_ವಾದಗಳು, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಸಂಕೇತ = "SIGTERM";
  ಆದರೆ (argc - ಮೊದಲ_arg > 1)
    ಸಂಕೇತ = argv[ಮೊದಲ_arg + 1];

  ಆದರೆ (kill_options.regex)
    {
      regex_t re;
      libcrun_container_list_t *list;
      libcrun_container_iter_t *it;
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;

      ret = regcomp (&re, argv[ಮೊದಲ_arg], REG_EXTENDED | REG_NOSUB);
      ಆದರೆ (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "invalid regular expression %s", argv[ಮೊದಲ_arg]);

      ret = libcrun_container_list (crun_context, &list, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "cannot read containers list");

      ಗಾಗಿ (it = libcrun_container_list_iter (list); libcrun_container_iter_ಮುಂದಿನ (it, &name);)
        ಆದರೆ (regexec (&re, name, 0, ಖಾಲಿ, 0) == 0)
          {
            ret = libcrun_container_kill (crun_context, name, ಸಂಕೇತ, err);
            ಆದರೆ (UNLIKELY (ret < 0))
              libcrun_error_report_and_release (err);
          }

      libcrun_container_iter_free (it);
      libcrun_container_list_free (list);
      regfree (&re);
      ಹಿಂತಿರುಗಿ 0;
    }

  ಆದರೆ (kill_options.all)
    ಹಿಂತಿರುಗಿ libcrun_container_killall (crun_context, argv[ಮೊದಲ_arg], ಸಂಕೇತ, err);

  ಹಿಂತಿರುಗಿ libcrun_container_kill (crun_context, argv[ಮೊದಲ_arg], ಸಂಕೇತ, err);
}
