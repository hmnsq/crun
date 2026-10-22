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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <argp.h>
#ضمن <string.h>
#ضمن <unistd.h>
#ضمن <errno.h>
#ضمن <signal.h>
#ضمن <regex.h>

#ضمن "crun.h"

ساكن حرف doc[] = "OCI runtime";

تعداد
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

بنية kill_options_s
{
  bool all;
  bool regex;
};

ساكن بنية kill_options_s kill_options;

ساكن بنية argp_option options[]
    = { { "all", 'a', 0, 0, "kill all the processes", 0 },
        { "regex", 'r', 0, 0, "the specified CONTAINER is a regular expression (kill multiple containers)", 0 },
        {
            0,
        } };

ساكن حرف دليلون_doc[] = "kill CONTAINER [SIGNAL]";

ساكن error_t
parse_opt (صحيح ڪنجي, حرف *arg arg_اڻ_استعمال_ٿيل, بنية argp_state *state arg_اڻ_استعمال_ٿيل)
{
  بدل (ڪنجي)
    {
    حالة 'a':
      kill_options.all = true;
      اكسر;

    حالة 'r':
      kill_options.regex = true;
      اكسر;

    حالة ARGP_ڪنجي_NO_دليلون:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    افتراضي:
      أرجع ARGP_ERR_UNKNOWN;
    }

  أرجع 0;
}

ساكن بنية argp لاڳيتو_argp = { options, parse_opt, دليلون_doc, doc, عدم, عدم, عدم };

صحيح
crun_command_kill (بنية crun_global_arguments *global_دليلون, صحيح argc, حرف **argv, libcrun_error_t *err)
{
  صحيح پهريون_arg = 0, ret;
  ثابت حرف *إشارة;
  cleanup_context libcrun_context_t *crun_context = عدم;

  argp_parse (&لاڳيتو_argp, argc, argv, ARGP_اندريون_ترتيب, &پهريون_arg, &kill_options);
  crun_assert_n_دليلون (argc - پهريون_arg, 1, 2);

  crun_context = new_libcrun_context (global_دليلون);

  ret = init_libcrun_context (crun_context, argv[پهريون_arg], global_دليلون, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إشارة = "SIGTERM";
  إذا (argc - پهريون_arg > 1)
    إشارة = argv[پهريون_arg + 1];

  إذا (kill_options.regex)
    {
      regex_t re;
      libcrun_container_list_t *list;
      libcrun_container_iter_t *it;
      ثابت حرف *name;

      ret = regcomp (&re, argv[پهريون_arg], REG_EXTENDED | REG_NOSUB);
      إذا (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "invalid regular expression %s", argv[پهريون_arg]);

      ret = libcrun_container_list (crun_context, &list, err);
      إذا (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "cannot read containers list");

      لكل (it = libcrun_container_list_iter (list); libcrun_container_iter_ايندڙ (it, &name);)
        إذا (regexec (&re, name, 0, عدم, 0) == 0)
          {
            ret = libcrun_container_kill (crun_context, name, إشارة, err);
            إذا (UNLIKELY (ret < 0))
              libcrun_error_report_and_release (err);
          }

      libcrun_container_iter_free (it);
      libcrun_container_list_free (list);
      regfree (&re);
      أرجع 0;
    }

  إذا (kill_options.all)
    أرجع libcrun_container_killall (crun_context, argv[پهريون_arg], إشارة, err);

  أرجع libcrun_container_kill (crun_context, argv[پهريون_arg], إشارة, err);
}
