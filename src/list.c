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

ساخت list_options_s
{
  bool quiet;
  صحیح format;
};

شمار
{
  LIST_جدول = 100,
  LIST_JSON,
};

ساکن ساخت list_options_s list_options;

ساکن ساخت argp_option options[]
    = { { "quiet", 'q', 0, 0, "show only IDs", 0 },
        { "format", 'f', "FORMAT", 0, "select one of: table or json (default: \"table\")", 0 },
        {
            0,
        } };

ساکن حرف دلائل_doc[] = "list";

ساکن error_t
parse_opt (صحیح کلید, حرف *arg, ساخت argp_state *state arg_غیر_مستعمل)
{
  انتخاب (کلید)
    {
    حالت 'q':
      list_options.quiet = true;
      توڑو;
    حالت 'f':
      اگر (strcmp (arg, "table") == 0)
        list_options.format = LIST_جدول;
      ورنہ اگر (strcmp (arg, "json") == 0)
        list_options.format = LIST_JSON;
      ورنہ
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      توڑو;

    طےشدہ:
      واپس ARGP_ERR_UNKNOWN;
    }

  واپس 0;
}

ساکن ساخت argp تسلسل_argp = { options, parse_opt, دلائل_doc, doc, تہی, تہی, تہی };

صحیح
crun_command_list (ساخت crun_global_arguments *global_دلائل, صحیح argc, حرف **argv, libcrun_error_t *err)
{
  صحیح پہلا_arg;
  صحیح ret, max_لمبائی = 4;
  cleanup_context libcrun_context_t *crun_context = تہی;
  libcrun_container_list_t *list = تہی;
  libcrun_container_iter_t *it;
  ثابت حرف *name;

  list_options.format = LIST_جدول;

  argp_parse (&تسلسل_argp, argc, argv, ARGP_اندر_ترتیب, &پہلا_arg, &list_options);
  crun_assert_n_دلائل (argc - پہلا_arg, 0, 0);

  crun_context = new_libcrun_context (global_دلائل);

  ret = init_libcrun_context (crun_context, argv[پہلا_arg], global_دلائل, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (list_options.format == LIST_JSON)
    {
      cleanup_free حرف *json = تہی;

      ret = libcrun_container_list_json (crun_context, &json, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      fputs (json, stdout);
      واپس 0;
    }

  ret = libcrun_container_list (crun_context, &list, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  برائے (it = libcrun_container_list_iter (list); libcrun_container_iter_اگلا (it, &name);)
    {
      صحیح l = لڑی_لمبائی (name);
      اگر (l > max_لمبائی)
        max_لمبائی = l;
    }
  libcrun_container_iter_free (it);

  max_لمبائی++;

  اگر (! list_options.quiet)
    لکھو ("%-*s%-10s%-8s %-39s %-30s %s\n", max_لمبائی, "NAME", "PID", "STATUS", "BUNDLE PATH", "CREATED", "OWNER");

  برائے (it = libcrun_container_list_iter (list); libcrun_container_iter_اگلا (it, &name);)
    {
      libcrun_حالت_t *حالت_2 = تہی;

      ret = libcrun_container_حالت_بھرو (crun_context, name, &حالت_2, err);
      اگر (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          جاری;
        }
      اگر (list_options.quiet)
        لکھو ("%s\n", name);
      ورنہ
        {
          libcrun_container_state_t state;
          صحیح running = 0;
          صحیح pid = libcrun_حالت_get_pid (حالت_2);

          ret = libcrun_حالت_get_state (حالت_2, &state, &running, err);
          اگر (UNLIKELY (ret < 0))
            {
              libcrun_error_report_and_release (err);
              libcrun_container_حالت_free (حالت_2);
              جاری;
            }

          اگر (! running)
            pid = 0;

          لکھو ("%-*s%-10d%-8s %-39s %-30s %s\n", max_لمبائی, name, pid,
                  libcrun_container_state_مقصد_string (state), libcrun_حالت_get_bundle (حالت_2),
                  libcrun_حالت_get_created (حالت_2), libcrun_حالت_get_owner (حالت_2));
        }

      libcrun_container_حالت_free (حالت_2);
    }
  libcrun_container_iter_free (it);

  libcrun_container_list_free (list);
  واپس ret >= 0 ? 0 : ret;
}
