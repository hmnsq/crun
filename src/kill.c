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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <argp.h>
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <unistd.h>
#অন্তর্ভাৱ <errno.h>
#অন্তর্ভাৱ <signal.h>
#অন্তর্ভাৱ <regex.h>

#অন্তর্ভাৱ "crun.h"

স্থির ৱর্ণ doc[] = "OCI runtime";

গণনা
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

সংরচনা kill_options_s
{
  bool all;
  bool regex;
};

স্থির সংরচনা kill_options_s kill_options;

স্থির সংরচনা argp_option options[]
    = { { "all", 'a', 0, 0, "kill all the processes", 0 },
        { "regex", 'r', 0, 0, "the specified CONTAINER is a regular expression (kill multiple containers)", 0 },
        {
            0,
        } };

স্থির ৱর্ণ args_doc[] = "kill CONTAINER [SIGNAL]";

স্থির error_t
parse_opt (পূর্ণাঙ্ক চাবি, ৱর্ণ *arg arg_unused, সংরচনা argp_state *state arg_unused)
{
  ৱিকল্প (চাবি)
    {
    পক্ষ 'a':
      kill_options.all = true;
      ভঙ্গ;

    পক্ষ 'r':
      kill_options.regex = true;
      ভঙ্গ;

    পক্ষ ARGP_চাবি_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    স্ৱভাৱ:
      ঘূৰাই ARGP_ERR_UNKNOWN;
    }

  ঘূৰাই 0;
}

স্থির সংরচনা argp run_argp = { options, parse_opt, args_doc, doc, রিক্ত, রিক্ত, রিক্ত };

পূর্ণাঙ্ক
crun_command_kill (সংরচনা crun_global_arguments *global_args, পূর্ণাঙ্ক argc, ৱর্ণ **argv, libcrun_error_t *err)
{
  পূর্ণাঙ্ক first_arg = 0, ret;
  ধ্রুৱ ৱর্ণ *সঙ্কেত;
  cleanup_context libcrun_context_t *crun_context = রিক্ত;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &kill_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  সঙ্কেত = "SIGTERM";
  যদি (argc - first_arg > 1)
    সঙ্কেত = argv[first_arg + 1];

  যদি (kill_options.regex)
    {
      regex_t re;
      libcrun_container_list_t *list;
      libcrun_container_iter_t *it;
      ধ্রুৱ ৱর্ণ *name;

      ret = regcomp (&re, argv[first_arg], REG_EXTENDED | REG_NOSUB);
      যদি (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "invalid regular expression %s", argv[first_arg]);

      ret = libcrun_container_list (crun_context, &list, err);
      যদি (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "cannot read containers list");

      বাবে (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
        যদি (regexec (&re, name, 0, রিক্ত, 0) == 0)
          {
            ret = libcrun_container_kill (crun_context, name, সঙ্কেত, err);
            যদি (UNLIKELY (ret < 0))
              libcrun_error_report_and_release (err);
          }

      libcrun_container_iter_free (it);
      libcrun_container_list_free (list);
      regfree (&re);
      ঘূৰাই 0;
    }

  যদি (kill_options.all)
    ঘূৰাই libcrun_container_killall (crun_context, argv[first_arg], সঙ্কেত, err);

  ঘূৰাই libcrun_container_kill (crun_context, argv[first_arg], সঙ্কেত, err);
}
