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

بنية list_options_s
{
  bool quiet;
  صحيح format;
};

تعداد
{
  LIST_جدول = 100,
  LIST_JSON,
};

ساكن بنية list_options_s list_options;

ساكن بنية argp_option options[]
    = { { "quiet", 'q', 0, 0, "show only IDs", 0 },
        { "format", 'f', "FORMAT", 0, "select one of: table or json (default: \"table\")", 0 },
        {
            0,
        } };

ساكن حرف آرگومانها_doc[] = "list";

ساكن error_t
parse_opt (صحيح کلید, حرف *arg, بنية argp_state *state arg_استفادهنشده)
{
  بدل (کلید)
    {
    حالة 'q':
      list_options.quiet = true;
      اكسر;
    حالة 'f':
      إذا (strcmp (arg, "table") == 0)
        list_options.format = LIST_جدول;
      وإلا إذا (strcmp (arg, "json") == 0)
        list_options.format = LIST_JSON;
      وإلا
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      اكسر;

    افتراضي:
      أرجع ARGP_ERR_UNKNOWN;
    }

  أرجع 0;
}

ساكن بنية argp پیاپی_argp = { options, parse_opt, آرگومانها_doc, doc, عدم, عدم, عدم };

صحيح
crun_command_list (بنية crun_global_arguments *global_آرگومانها, صحيح argc, حرف **argv, libcrun_error_t *err)
{
  صحيح اولی_arg;
  صحيح ret, max_درازی = 4;
  cleanup_context libcrun_context_t *crun_context = عدم;
  libcrun_container_list_t *list = عدم;
  libcrun_container_iter_t *it;
  ثابت حرف *name;

  list_options.format = LIST_جدول;

  argp_parse (&پیاپی_argp, argc, argv, ARGP_درون_ترتیب, &اولی_arg, &list_options);
  crun_assert_n_آرگومانها (argc - اولی_arg, 0, 0);

  crun_context = new_libcrun_context (global_آرگومانها);

  ret = init_libcrun_context (crun_context, argv[اولی_arg], global_آرگومانها, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (list_options.format == LIST_JSON)
    {
      cleanup_free حرف *json = عدم;

      ret = libcrun_container_list_json (crun_context, &json, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      fputs (json, stdout);
      أرجع 0;
    }

  ret = libcrun_container_list (crun_context, &list, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  لكل (it = libcrun_container_list_iter (list); libcrun_container_iter_بعدی (it, &name);)
    {
      صحيح l = طول_رشته (name);
      إذا (l > max_درازی)
        max_درازی = l;
    }
  libcrun_container_iter_free (it);

  max_درازی++;

  إذا (! list_options.quiet)
    چاپ_کن ("%-*s%-10s%-8s %-39s %-30s %s\n", max_درازی, "NAME", "PID", "STATUS", "BUNDLE PATH", "CREATED", "OWNER");

  لكل (it = libcrun_container_list_iter (list); libcrun_container_iter_بعدی (it, &name);)
    {
      libcrun_وضعیت_t *وضعیت = عدم;

      ret = libcrun_container_وضعیت_بارگیری (crun_context, name, &وضعیت, err);
      إذا (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          استمر;
        }
      إذا (list_options.quiet)
        چاپ_کن ("%s\n", name);
      وإلا
        {
          libcrun_container_state_t state;
          صحيح running = 0;
          صحيح pid = libcrun_وضعیت_get_pid (وضعیت);

          ret = libcrun_وضعیت_get_state (وضعیت, &state, &running, err);
          إذا (UNLIKELY (ret < 0))
            {
              libcrun_error_report_and_release (err);
              libcrun_container_وضعیت_free (وضعیت);
              استمر;
            }

          إذا (! running)
            pid = 0;

          چاپ_کن ("%-*s%-10d%-8s %-39s %-30s %s\n", max_درازی, name, pid,
                  libcrun_container_state_مقصد_string (state), libcrun_وضعیت_get_bundle (وضعیت),
                  libcrun_وضعیت_get_created (وضعیت), libcrun_وضعیت_get_owner (وضعیت));
        }

      libcrun_container_وضعیت_free (وضعیت);
    }
  libcrun_container_iter_free (it);

  libcrun_container_list_free (list);
  أرجع ret >= 0 ? 0 : ret;
}
