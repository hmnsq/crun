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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <argp.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <errno.h>

#अन्तर्भाव "crun.h"

स्थिर वर्ण doc[] = "OCI runtime";

गणना
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

संरचना list_options_s
{
  bool quiet;
  पूर्णाङ्क format;
};

गणना
{
  LIST_सारणी = 100,
  LIST_JSON,
};

स्थिर संरचना list_options_s list_options;

स्थिर संरचना argp_option options[]
    = { { "quiet", 'q', 0, 0, "show only IDs", 0 },
        { "format", 'f', "FORMAT", 0, "select one of: table or json (default: \"table\")", 0 },
        {
            0,
        } };

स्थिर वर्ण तर्काः_doc[] = "list";

स्थिर error_t
parse_opt (पूर्णाङ्क कुञ्चिका, वर्ण *arg, संरचना argp_state *state arg_अप्रयुक्तम्)
{
  विकल्प (कुञ्चिका)
    {
    पक्ष 'q':
      list_options.quiet = true;
      भङ्ग;
    पक्ष 'f':
      यदि (वाक्यं_तुलय (arg, "table") == 0)
        list_options.format = LIST_सारणी;
      अन्यथा यदि (वाक्यं_तुलय (arg, "json") == 0)
        list_options.format = LIST_JSON;
      अन्यथा
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      भङ्ग;

    स्वभाव:
      प्रत्यागम ARGP_ERR_UNKNOWN;
    }

  प्रत्यागम 0;
}

स्थिर संरचना argp सततम्_argp = { options, parse_opt, तर्काः_doc, doc, रिक्त, रिक्त, रिक्त };

पूर्णाङ्क
crun_command_list (संरचना crun_global_arguments *global_तर्काः, पूर्णाङ्क argc, वर्ण **argv, libcrun_error_t *err)
{
  पूर्णाङ्क first_arg;
  पूर्णाङ्क ret, max_दीर्घता = 4;
  cleanup_context libcrun_context_t *crun_context = रिक्त;
  libcrun_container_list_t *list = रिक्त;
  libcrun_container_iter_t *it;
  ध्रुव वर्ण *name;

  list_options.format = LIST_सारणी;

  argp_parse (&सततम्_argp, argc, argv, ARGP_अन्तर्_क्रमः, &first_arg, &list_options);
  crun_assert_n_तर्काः (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_तर्काः);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_तर्काः, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (list_options.format == LIST_JSON)
    {
      cleanup_free वर्ण *json = रिक्त;

      ret = libcrun_container_list_json (crun_context, &json, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      fputs (json, stdout);
      प्रत्यागम 0;
    }

  ret = libcrun_container_list (crun_context, &list, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रति (it = libcrun_container_list_iter (list); libcrun_container_iter_अग्र (it, &name);)
    {
      पूर्णाङ्क l = वाक्यदैर्घ्यम् (name);
      यदि (l > max_दीर्घता)
        max_दीर्घता = l;
    }
  libcrun_container_iter_free (it);

  max_दीर्घता++;

  यदि (! list_options.quiet)
    प्रारूप_लिख ("%-*s%-10s%-8s %-39s %-30s %s\n", max_दीर्घता, "NAME", "PID", "STATUS", "BUNDLE PATH", "CREATED", "OWNER");

  प्रति (it = libcrun_container_list_iter (list); libcrun_container_iter_अग्र (it, &name);)
    {
      libcrun_स्थितिः_t *स्थितिः = रिक्त;

      ret = libcrun_container_स्थितिः_आरोपय (crun_context, name, &स्थितिः, err);
      यदि (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          अनुवर्त;
        }
      यदि (list_options.quiet)
        प्रारूप_लिख ("%s\n", name);
      अन्यथा
        {
          libcrun_container_state_t state;
          पूर्णाङ्क running = 0;
          पूर्णाङ्क pid = libcrun_स्थितिः_get_pid (स्थितिः);

          ret = libcrun_स्थितिः_get_state (स्थितिः, &state, &running, err);
          यदि (UNLIKELY (ret < 0))
            {
              libcrun_error_report_and_release (err);
              libcrun_container_स्थितिः_free (स्थितिः);
              अनुवर्त;
            }

          यदि (! running)
            pid = 0;

          प्रारूप_लिख ("%-*s%-10d%-8s %-39s %-30s %s\n", max_दीर्घता, name, pid,
                  libcrun_container_state_गन्तव्यम्_string (state), libcrun_स्थितिः_get_bundle (स्थितिः),
                  libcrun_स्थितिः_get_created (स्थितिः), libcrun_स्थितिः_get_owner (स्थितिः));
        }

      libcrun_container_स्थितिः_free (स्थितिः);
    }
  libcrun_container_iter_free (it);

  libcrun_container_list_free (list);
  प्रत्यागम ret >= 0 ? 0 : ret;
}
