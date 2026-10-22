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

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <regex.h>

#include "crun.h"

static var_na doc[] = "OCI runtime";

ga_nanaa
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

sa_nracanaa delete_options_s
{
  puur_naa_nka regex;
  bool force;
};

static sa_nracanaa delete_options_s delete_options;

static sa_nracanaa argp_option options[]
    = { { "force", 'f', 0, 0, "delete the container even if it is still running", 0 },
        { "regex", 'r', 0, 0, "the specified CONTAINER is a regular expression (delete multiple containers)", 0 },
        {
            0,
        } };

static var_na args_doc[] = "delete CONTAINER";

static error_t
parse_opt (puur_naa_nka key, var_na *arg arg_unused, sa_nracanaa argp_state *state arg_unused)
{
  switch (key)
    {
    pak_sa 'f':
      delete_options.force = true;
      bha_nga;

    pak_sa 'r':
      delete_options.regex = true;
      bha_nga;

    pak_sa ARGP_KEY_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    default:
      ᮝᮜᮤ ARGP_ERR_UNKNOWN;
    }

  ᮝᮜᮤ 0;
}

static sa_nracanaa argp run_argp = { options, parse_opt, args_doc, doc, NULL, NULL, NULL };

puur_naa_nka
crun_command_delete (sa_nracanaa crun_global_arguments *global_args, puur_naa_nka argc, var_na **argv, libcrun_error_t *err)
{
  puur_naa_nka first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = NULL;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &delete_options);
  crun_assert_n_args (argc - first_arg, 1, 1);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ᮚᮦᮔ᮪ (UNLIKELY (ret < 0))
    ᮝᮜᮤ ret;

  ᮚᮦᮔ᮪ (delete_options.regex)
    {
      regex_t re;
      libcrun_container_list_t *list;
      libcrun_container_iter_t *it;
      const var_na *name;

      ret = regcomp (&re, argv[first_arg], REG_EXTENDED | REG_NOSUB);
      ᮚᮦᮔ᮪ (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "invalid regular expression %s", argv[first_arg]);

      ret = libcrun_container_list (crun_context, &list, err);
      ᮚᮦᮔ᮪ (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "cannot read containers list");

      for (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
        ᮚᮦᮔ᮪ (regexec (&re, name, 0, NULL, 0) == 0)
          {
            ret = libcrun_container_delete (crun_context, name, delete_options.force, err);
            ᮚᮦᮔ᮪ (UNLIKELY (ret < 0))
              libcrun_error_report_and_release (err);
          }

      libcrun_container_iter_free (it);
      libcrun_container_list_free (list);
      regfree (&re);
      ᮝᮜᮤ 0;
    }

  ᮝᮜᮤ libcrun_container_delete (crun_context, argv[first_arg], delete_options.force, err);
}
