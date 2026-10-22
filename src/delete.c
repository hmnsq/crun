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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <argp.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <regex.h>

#അന്തര്ഭാവ "crun.h"

സ്ഥിതം അക്ഷരം doc[] = "OCI runtime";

എണ്ണൽ
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

ഘടന delete_options_s
{
  പൂർണ്ണസംഖ്യ regex;
  bool force;
};

സ്ഥിതം ഘടന delete_options_s delete_options;

സ്ഥിതം ഘടന argp_option options[]
    = { { "force", 'f', 0, 0, "delete the container even if it is still running", 0 },
        { "regex", 'r', 0, 0, "the specified CONTAINER is a regular expression (delete multiple containers)", 0 },
        {
            0,
        } };

സ്ഥിതം അക്ഷരം args_doc[] = "delete CONTAINER";

സ്ഥിതം error_t
parse_opt (പൂർണ്ണസംഖ്യ താക്കോൽ, അക്ഷരം *arg arg_unused, ഘടന argp_state *state arg_unused)
{
  തിരഞ്ഞെടുപ്പ് (താക്കോൽ)
    {
    അവസ്ഥ 'f':
      delete_options.force = true;
      നിർത്തുക;

    അവസ്ഥ 'r':
      delete_options.regex = true;
      നിർത്തുക;

    അവസ്ഥ ARGP_താക്കോൽ_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    സ്ഥിരസ്ഥിതി:
      തിരികെ ARGP_ERR_UNKNOWN;
    }

  തിരികെ 0;
}

സ്ഥിതം ഘടന argp run_argp = { options, parse_opt, args_doc, doc, ശൂന്യ, ശൂന്യ, ശൂന്യ };

പൂർണ്ണസംഖ്യ
crun_command_delete (ഘടന crun_global_arguments *global_args, പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = ശൂന്യ;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &delete_options);
  crun_assert_n_args (argc - first_arg, 1, 1);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (delete_options.regex)
    {
      regex_t re;
      libcrun_container_list_t *list;
      libcrun_container_iter_t *it;
      സ്ഥിരം അക്ഷരം *name;

      ret = regcomp (&re, argv[first_arg], REG_EXTENDED | REG_NOSUB);
      എങ്കിൽ (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "invalid regular expression %s", argv[first_arg]);

      ret = libcrun_container_list (crun_context, &list, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "cannot read containers list");

      വേണ്ടി (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
        എങ്കിൽ (regexec (&re, name, 0, ശൂന്യ, 0) == 0)
          {
            ret = libcrun_container_delete (crun_context, name, delete_options.force, err);
            എങ്കിൽ (UNLIKELY (ret < 0))
              libcrun_error_report_and_release (err);
          }

      libcrun_container_iter_free (it);
      libcrun_container_list_free (list);
      regfree (&re);
      തിരികെ 0;
    }

  തിരികെ libcrun_container_delete (crun_context, argv[first_arg], delete_options.force, err);
}
