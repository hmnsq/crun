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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <argp.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <regex.h>

#அந்தர்பாவ "crun.h"

நிலையான எழுத்து doc[] = "OCI runtime";

எண்ணிக்கை
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

கட்டமைப்பு delete_options_s
{
  முழு regex;
  bool force;
};

நிலையான கட்டமைப்பு delete_options_s delete_options;

நிலையான கட்டமைப்பு argp_option options[]
    = { { "force", 'f', 0, 0, "delete the container even if it is still running", 0 },
        { "regex", 'r', 0, 0, "the specified CONTAINER is a regular expression (delete multiple containers)", 0 },
        {
            0,
        } };

நிலையான எழுத்து args_doc[] = "delete CONTAINER";

நிலையான error_t
parse_opt (முழு சாவி, எழுத்து *arg arg_unused, கட்டமைப்பு argp_state *state arg_unused)
{
  தேர்வு (சாவி)
    {
    நிலை 'f':
      delete_options.force = true;
      நிறுத்து;

    நிலை 'r':
      delete_options.regex = true;
      நிறுத்து;

    நிலை ARGP_சாவி_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    இயல்பு:
      திரும்பு ARGP_ERR_UNKNOWN;
    }

  திரும்பு 0;
}

நிலையான கட்டமைப்பு argp run_argp = { options, parse_opt, args_doc, doc, வெற்று, வெற்று, வெற்று };

முழு
crun_command_delete (கட்டமைப்பு crun_global_arguments *global_args, முழு argc, எழுத்து **argv, libcrun_error_t *err)
{
  முழு first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = வெற்று;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &delete_options);
  crun_assert_n_args (argc - first_arg, 1, 1);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (delete_options.regex)
    {
      regex_t re;
      libcrun_container_list_t *list;
      libcrun_container_iter_t *it;
      மாறிலி எழுத்து *name;

      ret = regcomp (&re, argv[first_arg], REG_EXTENDED | REG_NOSUB);
      எனில் (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "invalid regular expression %s", argv[first_arg]);

      ret = libcrun_container_list (crun_context, &list, err);
      எனில் (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "cannot read containers list");

      க்கு (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
        எனில் (regexec (&re, name, 0, வெற்று, 0) == 0)
          {
            ret = libcrun_container_delete (crun_context, name, delete_options.force, err);
            எனில் (UNLIKELY (ret < 0))
              libcrun_error_report_and_release (err);
          }

      libcrun_container_iter_free (it);
      libcrun_container_list_free (list);
      regfree (&re);
      திரும்பு 0;
    }

  திரும்பு libcrun_container_delete (crun_context, argv[first_arg], delete_options.force, err);
}
