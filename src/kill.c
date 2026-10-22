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

ساکن حرف doc[] = "OCI runtime";

شمار
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

ساخت kill_options_s
{
  bool all;
  bool regex;
};

ساکن ساخت kill_options_s kill_options;

ساکن ساخت argp_option options[]
    = { { "all", 'a', 0, 0, "kill all the processes", 0 },
        { "regex", 'r', 0, 0, "the specified CONTAINER is a regular expression (kill multiple containers)", 0 },
        {
            0,
        } };

ساکن حرف دلائل_doc[] = "kill CONTAINER [SIGNAL]";

ساکن error_t
parse_opt (صحیح کلید, حرف *arg arg_غیر_مستعمل, ساخت argp_state *state arg_غیر_مستعمل)
{
  انتخاب (کلید)
    {
    حالت 'a':
      kill_options.all = true;
      توڑو;

    حالت 'r':
      kill_options.regex = true;
      توڑو;

    حالت ARGP_کلید_NO_دلائل:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    طےشدہ:
      واپس ARGP_ERR_UNKNOWN;
    }

  واپس 0;
}

ساکن ساخت argp تسلسل_argp = { options, parse_opt, دلائل_doc, doc, تہی, تہی, تہی };

صحیح
crun_command_kill (ساخت crun_global_arguments *global_دلائل, صحیح argc, حرف **argv, libcrun_error_t *err)
{
  صحیح پہلا_arg = 0, ret;
  ثابت حرف *اشارہ;
  cleanup_context libcrun_context_t *crun_context = تہی;

  argp_parse (&تسلسل_argp, argc, argv, ARGP_اندر_ترتیب, &پہلا_arg, &kill_options);
  crun_assert_n_دلائل (argc - پہلا_arg, 1, 2);

  crun_context = new_libcrun_context (global_دلائل);

  ret = init_libcrun_context (crun_context, argv[پہلا_arg], global_دلائل, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اشارہ = "SIGTERM";
  اگر (argc - پہلا_arg > 1)
    اشارہ = argv[پہلا_arg + 1];

  اگر (kill_options.regex)
    {
      regex_t re;
      libcrun_container_list_t *list;
      libcrun_container_iter_t *it;
      ثابت حرف *name;

      ret = regcomp (&re, argv[پہلا_arg], REG_EXTENDED | REG_NOSUB);
      اگر (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "invalid regular expression %s", argv[پہلا_arg]);

      ret = libcrun_container_list (crun_context, &list, err);
      اگر (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "cannot read containers list");

      برائے (it = libcrun_container_list_iter (list); libcrun_container_iter_اگلا (it, &name);)
        اگر (regexec (&re, name, 0, تہی, 0) == 0)
          {
            ret = libcrun_container_kill (crun_context, name, اشارہ, err);
            اگر (UNLIKELY (ret < 0))
              libcrun_error_report_and_release (err);
          }

      libcrun_container_iter_free (it);
      libcrun_container_list_free (list);
      regfree (&re);
      واپس 0;
    }

  اگر (kill_options.all)
    واپس libcrun_container_killall (crun_context, argv[پہلا_arg], اشارہ, err);

  واپس libcrun_container_kill (crun_context, argv[پہلا_arg], اشارہ, err);
}
