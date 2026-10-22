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

ساخت delete_options_s
{
  صحیح regex;
  bool force;
};

ساکن ساخت delete_options_s delete_options;

ساکن ساخت argp_option options[]
    = { { "force", 'f', 0, 0, "delete the container even if it is still running", 0 },
        { "regex", 'r', 0, 0, "the specified CONTAINER is a regular expression (delete multiple containers)", 0 },
        {
            0,
        } };

ساکن حرف دلائل_doc[] = "delete CONTAINER";

ساکن error_t
parse_opt (صحیح کلید, حرف *arg arg_غیر_مستعمل, ساخت argp_state *state arg_غیر_مستعمل)
{
  انتخاب (کلید)
    {
    حالت 'f':
      delete_options.force = true;
      توڑو;

    حالت 'r':
      delete_options.regex = true;
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
crun_command_delete (ساخت crun_global_arguments *global_دلائل, صحیح argc, حرف **argv, libcrun_error_t *err)
{
  صحیح پہلا_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = تہی;

  argp_parse (&تسلسل_argp, argc, argv, ARGP_اندر_ترتیب, &پہلا_arg, &delete_options);
  crun_assert_n_دلائل (argc - پہلا_arg, 1, 1);

  crun_context = new_libcrun_context (global_دلائل);

  ret = init_libcrun_context (crun_context, argv[پہلا_arg], global_دلائل, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (delete_options.regex)
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
            ret = libcrun_container_delete (crun_context, name, delete_options.force, err);
            اگر (UNLIKELY (ret < 0))
              libcrun_error_report_and_release (err);
          }

      libcrun_container_iter_free (it);
      libcrun_container_list_free (list);
      regfree (&re);
      واپس 0;
    }

  واپس libcrun_container_delete (crun_context, argv[پہلا_arg], delete_options.force, err);
}
