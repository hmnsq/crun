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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <argp.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <signal.h>
#অন্তর্ভাব <regex.h>

#অন্তর্ভাব "crun.h"

স্থির অক্ষর doc[] = "OCI runtime";

গণনা
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

কাঠামো kill_options_s
{
  bool all;
  bool regex;
};

স্থির কাঠামো kill_options_s kill_options;

স্থির কাঠামো argp_option options[]
    = { { "all", 'a', 0, 0, "kill all the processes", 0 },
        { "regex", 'r', 0, 0, "the specified CONTAINER is a regular expression (kill multiple containers)", 0 },
        {
            0,
        } };

স্থির অক্ষর args_doc[] = "kill CONTAINER [SIGNAL]";

স্থির error_t
parse_opt (পূর্ণসংখ্যা চাবি, অক্ষর *arg arg_unused, কাঠামো argp_state *state arg_unused)
{
  নির্বাচন (চাবি)
    {
    ক্ষেত্র 'a':
      kill_options.all = true;
      বিরতি;

    ক্ষেত্র 'r':
      kill_options.regex = true;
      বিরতি;

    ক্ষেত্র ARGP_চাবি_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    পূর্বনির্ধারিত:
      ফেরত ARGP_ERR_UNKNOWN;
    }

  ফেরত 0;
}

স্থির কাঠামো argp run_argp = { options, parse_opt, args_doc, doc, ফাঁকা, ফাঁকা, ফাঁকা };

পূর্ণসংখ্যা
crun_command_kill (কাঠামো crun_global_arguments *global_args, পূর্ণসংখ্যা argc, অক্ষর **argv, libcrun_error_t *err)
{
  পূর্ণসংখ্যা first_arg = 0, ret;
  ধ্রুবক অক্ষর *সংকেত;
  cleanup_context libcrun_context_t *crun_context = ফাঁকা;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &kill_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  সংকেত = "SIGTERM";
  যদি (argc - first_arg > 1)
    সংকেত = argv[first_arg + 1];

  যদি (kill_options.regex)
    {
      regex_t re;
      libcrun_container_list_t *list;
      libcrun_container_iter_t *it;
      ধ্রুবক অক্ষর *name;

      ret = regcomp (&re, argv[first_arg], REG_EXTENDED | REG_NOSUB);
      যদি (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "invalid regular expression %s", argv[first_arg]);

      ret = libcrun_container_list (crun_context, &list, err);
      যদি (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "cannot read containers list");

      জন্য (it = libcrun_container_list_iter (list); libcrun_container_iter_পরবর্তী (it, &name);)
        যদি (regexec (&re, name, 0, ফাঁকা, 0) == 0)
          {
            ret = libcrun_container_kill (crun_context, name, সংকেত, err);
            যদি (UNLIKELY (ret < 0))
              libcrun_error_report_and_release (err);
          }

      libcrun_container_iter_free (it);
      libcrun_container_list_free (list);
      regfree (&re);
      ফেরত 0;
    }

  যদি (kill_options.all)
    ফেরত libcrun_container_killall (crun_context, argv[first_arg], সংকেত, err);

  ফেরত libcrun_container_kill (crun_context, argv[first_arg], সংকেত, err);
}
