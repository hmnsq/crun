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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <argp.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <regex.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "crun.h"

ਸਥਿਰ ਅੱਖਰ doc[] = "OCI runtime";

ਗਿਣਤੀ
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

ਬਣਤਰ delete_options_s
{
  ਪੂਰਨ regex;
  bool force;
};

ਸਥਿਰ ਬਣਤਰ delete_options_s delete_options;

ਸਥਿਰ ਬਣਤਰ argp_option options[]
    = { { "force", 'f', 0, 0, "delete the container even if it is still running", 0 },
        { "regex", 'r', 0, 0, "the specified CONTAINER is a regular expression (delete multiple containers)", 0 },
        {
            0,
        } };

ਸਥਿਰ ਅੱਖਰ ਦਲੀਲਾਂ_doc[] = "delete CONTAINER";

ਸਥਿਰ error_t
parse_opt (ਪੂਰਨ ਕੁੰਜੀ, ਅੱਖਰ *arg arg_ਅਣਵਰਤਿਆ, ਬਣਤਰ argp_state *state arg_ਅਣਵਰਤਿਆ)
{
  ਚੋਣ (ਕੁੰਜੀ)
    {
    ਹਾਲਤ 'f':
      delete_options.force = true;
      ਰੋਕੋ;

    ਹਾਲਤ 'r':
      delete_options.regex = true;
      ਰੋਕੋ;

    ਹਾਲਤ ARGP_ਕੁੰਜੀ_NO_ਦਲੀਲਾਂ:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    ਮੂਲ:
      ਵਾਪਸ ARGP_ERR_UNKNOWN;
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਬਣਤਰ argp ਲੜੀਵਾਰ_argp = { options, parse_opt, ਦਲੀਲਾਂ_doc, doc, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ };

ਪੂਰਨ
crun_command_delete (ਬਣਤਰ crun_global_arguments *global_ਦਲੀਲਾਂ, ਪੂਰਨ argc, ਅੱਖਰ **argv, libcrun_error_t *err)
{
  ਪੂਰਨ ਪਹਿਲਾ_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = ਨਿਰਾ;

  argp_parse (&ਲੜੀਵਾਰ_argp, argc, argv, ARGP_ਵਿੱਚ_ਕ੍ਰਮ, &ਪਹਿਲਾ_arg, &delete_options);
  crun_assert_n_ਦਲੀਲਾਂ (argc - ਪਹਿਲਾ_arg, 1, 1);

  crun_context = new_libcrun_context (global_ਦਲੀਲਾਂ);

  ret = init_libcrun_context (crun_context, argv[ਪਹਿਲਾ_arg], global_ਦਲੀਲਾਂ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (delete_options.regex)
    {
      regex_t re;
      libcrun_container_list_t *list;
      libcrun_container_iter_t *it;
      ਅਚਲ ਅੱਖਰ *name;

      ret = regcomp (&re, argv[ਪਹਿਲਾ_arg], REG_EXTENDED | REG_NOSUB);
      ਜੇ (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "invalid regular expression %s", argv[ਪਹਿਲਾ_arg]);

      ret = libcrun_container_list (crun_context, &list, err);
      ਜੇ (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "cannot read containers list");

      ਲਈ (it = libcrun_container_list_iter (list); libcrun_container_iter_ਅਗਲਾ (it, &name);)
        ਜੇ (regexec (&re, name, 0, ਨਿਰਾ, 0) == 0)
          {
            ret = libcrun_container_delete (crun_context, name, delete_options.force, err);
            ਜੇ (UNLIKELY (ret < 0))
              libcrun_error_report_and_release (err);
          }

      libcrun_container_iter_free (it);
      libcrun_container_list_free (list);
      regfree (&re);
      ਵਾਪਸ 0;
    }

  ਵਾਪਸ libcrun_container_delete (crun_context, argv[ਪਹਿਲਾ_arg], delete_options.force, err);
}
