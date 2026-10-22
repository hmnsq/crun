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

ಸ್ಥಿರ ವರ್ಣ doc[] = "OCI runtime";

ಗಣನಾ
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

ಸಂರಚನಾ kill_options_s
{
  bool all;
  bool regex;
};

ಸ್ಥಿರ ಸಂರಚನಾ kill_options_s kill_options;

ಸ್ಥಿರ ಸಂರಚನಾ argp_option options[]
    = { { "all", 'a', 0, 0, "kill all the processes", 0 },
        { "regex", 'r', 0, 0, "the specified CONTAINER is a regular expression (kill multiple containers)", 0 },
        {
            0,
        } };

ಸ್ಥಿರ ವರ್ಣ args_doc[] = "kill CONTAINER [SIGNAL]";

ಸ್ಥಿರ error_t
parse_opt (ಪೂರ್ಣಾಙ್ಕ ಕೀಲಿ, ವರ್ಣ *arg arg_unused, ಸಂರಚನಾ argp_state *state arg_unused)
{
  ವಿಕಲ್ಪ (ಕೀಲಿ)
    {
    ಪಕ್ಷ 'a':
      kill_options.all = true;
      ಭಙ್ಗ;

    ಪಕ್ಷ 'r':
      kill_options.regex = true;
      ಭಙ್ಗ;

    ಪಕ್ಷ ARGP_ಕೀಲಿ_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ ARGP_ERR_UNKNOWN;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಸಂರಚನಾ argp run_argp = { options, parse_opt, args_doc, doc, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ };

ಪೂರ್ಣಾಙ್ಕ
crun_command_kill (ಸಂರಚನಾ crun_global_arguments *global_args, ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ first_arg = 0, ret;
  ಧ್ರುವ ವರ್ಣ *ಸಙ್ಕೇತ;
  cleanup_context libcrun_context_t *crun_context = ರಿಕ್ತ;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &kill_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಸಙ್ಕೇತ = "SIGTERM";
  ಯದಿ (argc - first_arg > 1)
    ಸಙ್ಕೇತ = argv[first_arg + 1];

  ಯದಿ (kill_options.regex)
    {
      regex_t re;
      libcrun_container_list_t *list;
      libcrun_container_iter_t *it;
      ಧ್ರುವ ವರ್ಣ *name;

      ret = regcomp (&re, argv[first_arg], REG_EXTENDED | REG_NOSUB);
      ಯದಿ (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "invalid regular expression %s", argv[first_arg]);

      ret = libcrun_container_list (crun_context, &list, err);
      ಯದಿ (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "cannot read containers list");

      ಪ್ರತಿ (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
        ಯದಿ (regexec (&re, name, 0, ರಿಕ್ತ, 0) == 0)
          {
            ret = libcrun_container_kill (crun_context, name, ಸಙ್ಕೇತ, err);
            ಯದಿ (UNLIKELY (ret < 0))
              libcrun_error_report_and_release (err);
          }

      libcrun_container_iter_free (it);
      libcrun_container_list_free (list);
      regfree (&re);
      ಪ್ರತ್ಯಾಗಮ 0;
    }

  ಯದಿ (kill_options.all)
    ಪ್ರತ್ಯಾಗಮ libcrun_container_killall (crun_context, argv[first_arg], ಸಙ್ಕೇತ, err);

  ಪ್ರತ್ಯಾಗಮ libcrun_container_kill (crun_context, argv[first_arg], ಸಙ್ಕೇತ, err);
}
