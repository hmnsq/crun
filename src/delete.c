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
#अन्तर्भाव <regex.h>

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

संरचना delete_options_s
{
  पूर्णाङ्क regex;
  bool force;
};

स्थिर संरचना delete_options_s delete_options;

स्थिर संरचना argp_option options[]
    = { { "force", 'f', 0, 0, "delete the container even if it is still running", 0 },
        { "regex", 'r', 0, 0, "the specified CONTAINER is a regular expression (delete multiple containers)", 0 },
        {
            0,
        } };

स्थिर वर्ण args_doc[] = "delete CONTAINER";

स्थिर error_t
parse_opt (पूर्णाङ्क साँचो, वर्ण *arg arg_unused, संरचना argp_state *state arg_unused)
{
  विकल्प (साँचो)
    {
    पक्ष 'f':
      delete_options.force = true;
      रोक;

    पक्ष 'r':
      delete_options.regex = true;
      रोक;

    पक्ष ARGP_साँचो_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    स्वभाव:
      फर्काऊ ARGP_ERR_UNKNOWN;
    }

  फर्काऊ 0;
}

स्थिर संरचना argp run_argp = { options, parse_opt, args_doc, doc, रिक्त, रिक्त, रिक्त };

पूर्णाङ्क
crun_command_delete (संरचना crun_global_arguments *global_args, पूर्णाङ्क argc, वर्ण **argv, libcrun_error_t *err)
{
  पूर्णाङ्क first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = रिक्त;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &delete_options);
  crun_assert_n_args (argc - first_arg, 1, 1);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  यदि (delete_options.regex)
    {
      regex_t re;
      libcrun_container_list_t *list;
      libcrun_container_iter_t *it;
      अचर वर्ण *name;

      ret = regcomp (&re, argv[first_arg], REG_EXTENDED | REG_NOSUB);
      यदि (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "invalid regular expression %s", argv[first_arg]);

      ret = libcrun_container_list (crun_context, &list, err);
      यदि (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "cannot read containers list");

      लागि (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
        यदि (regexec (&re, name, 0, रिक्त, 0) == 0)
          {
            ret = libcrun_container_delete (crun_context, name, delete_options.force, err);
            यदि (UNLIKELY (ret < 0))
              libcrun_error_report_and_release (err);
          }

      libcrun_container_iter_free (it);
      libcrun_container_list_free (list);
      regfree (&re);
      फर्काऊ 0;
    }

  फर्काऊ libcrun_container_delete (crun_context, argv[first_arg], delete_options.force, err);
}
