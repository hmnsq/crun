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

#شامل <config.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <argp.h>
#شامل <string.h>
#شامل <unistd.h>
#شامل <errno.h>
#شامل <signal.h>
#شامل <regex.h>

#شامل "crun.h"

ایستا نویسه doc[] = "OCI runtime";

شمارش
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

ساختار kill_options_s
{
  bool all;
  bool regex;
};

ایستا ساختار kill_options_s kill_options;

ایستا ساختار argp_option options[]
    = { { "all", 'a', 0, 0, "kill all the processes", 0 },
        { "regex", 'r', 0, 0, "the specified CONTAINER is a regular expression (kill multiple containers)", 0 },
        {
            0,
        } };

ایستا نویسه args_doc[] = "kill CONTAINER [SIGNAL]";

ایستا error_t
parse_opt (صحیح کلید, نویسه *arg arg_unused, ساختار argp_state *state arg_unused)
{
  انتخاب (کلید)
    {
    حالت 'a':
      kill_options.all = true;
      بشکن;

    حالت 'r':
      kill_options.regex = true;
      بشکن;

    حالت ARGP_کلید_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    پیشفرض:
      بازگشت ARGP_ERR_UNKNOWN;
    }

  بازگشت 0;
}

ایستا ساختار argp run_argp = { options, parse_opt, args_doc, doc, پوچ, پوچ, پوچ };

صحیح
crun_command_kill (ساختار crun_global_arguments *global_args, صحیح argc, نویسه **argv, libcrun_error_t *err)
{
  صحیح first_arg = 0, ret;
  ثابت نویسه *سیگنال;
  cleanup_context libcrun_context_t *crun_context = پوچ;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &kill_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  سیگنال = "SIGTERM";
  اگر (argc - first_arg > 1)
    سیگنال = argv[first_arg + 1];

  اگر (kill_options.regex)
    {
      regex_t re;
      libcrun_container_list_t *list;
      libcrun_container_iter_t *it;
      ثابت نویسه *name;

      ret = regcomp (&re, argv[first_arg], REG_EXTENDED | REG_NOSUB);
      اگر (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "invalid regular expression %s", argv[first_arg]);

      ret = libcrun_container_list (crun_context, &list, err);
      اگر (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "cannot read containers list");

      برای (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
        اگر (regexec (&re, name, 0, پوچ, 0) == 0)
          {
            ret = libcrun_container_kill (crun_context, name, سیگنال, err);
            اگر (UNLIKELY (ret < 0))
              libcrun_error_report_and_release (err);
          }

      libcrun_container_iter_free (it);
      libcrun_container_list_free (list);
      regfree (&re);
      بازگشت 0;
    }

  اگر (kill_options.all)
    بازگشت libcrun_container_killall (crun_context, argv[first_arg], سیگنال, err);

  بازگشت libcrun_container_kill (crun_context, argv[first_arg], سیگنال, err);
}
