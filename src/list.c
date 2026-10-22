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

sa_nracanaa list_options_s
{
  bool quiet;
  ຈຳນວນເຕັມ format;
};

ga_nanaa
{
  LIST_TABLE = 100,
  LIST_JSON,
};

static sa_nracanaa list_options_s list_options;

static sa_nracanaa argp_option options[]
    = { { "quiet", 'q', 0, 0, "show only IDs", 0 },
        { "format", 'f', "FORMAT", 0, "select one of: table or json (default: \"table\")", 0 },
        {
            0,
        } };

static var_na args_doc[] = "list";

static error_t
parse_opt (ຈຳນວນເຕັມ key, var_na *arg, sa_nracanaa argp_state *state arg_unused)
{
  switch (key)
    {
    pak_sa 'q':
      list_options.quiet = true;
      bha_nga;
    pak_sa 'f':
      ຖ້າ (strcmp (arg, "table") == 0)
        list_options.format = LIST_TABLE;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (strcmp (arg, "json") == 0)
        list_options.format = LIST_JSON;
      ບໍ່ດັ່ງນັ້ນ
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      bha_nga;

    default:
      ສົ່ງຄືນ ARGP_ERR_UNKNOWN;
    }

  ສົ່ງຄືນ 0;
}

static sa_nracanaa argp run_argp = { options, parse_opt, args_doc, doc, NULL, NULL, NULL };

ຈຳນວນເຕັມ
crun_command_list (sa_nracanaa crun_global_arguments *global_args, ຈຳນວນເຕັມ argc, var_na **argv, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ first_arg;
  ຈຳນວນເຕັມ ret, max_length = 4;
  cleanup_context libcrun_context_t *crun_context = NULL;
  libcrun_container_list_t *list = NULL;
  libcrun_container_iter_t *it;
  const var_na *name;

  list_options.format = LIST_TABLE;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &list_options);
  crun_assert_n_args (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ຖ້າ (list_options.format == LIST_JSON)
    {
      cleanup_free var_na *json = NULL;

      ret = libcrun_container_list_json (crun_context, &json, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      fputs (json, stdout);
      ສົ່ງຄືນ 0;
    }

  ret = libcrun_container_list (crun_context, &list, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ສຳລັບ (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
    {
      ຈຳນວນເຕັມ l = strlen (name);
      ຖ້າ (l > max_length)
        max_length = l;
    }
  libcrun_container_iter_free (it);

  max_length++;

  ຖ້າ (! list_options.quiet)
    printf ("%-*s%-10s%-8s %-39s %-30s %s\n", max_length, "NAME", "PID", "STATUS", "BUNDLE PATH", "CREATED", "OWNER");

  ສຳລັບ (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
    {
      libcrun_status_t *status = NULL;

      ret = libcrun_container_status_load (crun_context, name, &status, err);
      ຖ້າ (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          continue;
        }
      ຖ້າ (list_options.quiet)
        printf ("%s\n", name);
      ບໍ່ດັ່ງນັ້ນ
        {
          libcrun_container_state_t state;
          ຈຳນວນເຕັມ running = 0;
          ຈຳນວນເຕັມ pid = libcrun_status_get_pid (status);

          ret = libcrun_status_get_state (status, &state, &running, err);
          ຖ້າ (UNLIKELY (ret < 0))
            {
              libcrun_error_report_and_release (err);
              libcrun_container_status_free (status);
              continue;
            }

          ຖ້າ (! running)
            pid = 0;

          printf ("%-*s%-10d%-8s %-39s %-30s %s\n", max_length, name, pid,
                  libcrun_container_state_to_string (state), libcrun_status_get_bundle (status),
                  libcrun_status_get_created (status), libcrun_status_get_owner (status));
        }

      libcrun_container_status_free (status);
    }
  libcrun_container_iter_free (it);

  libcrun_container_list_free (list);
  ສົ່ງຄືນ ret >= 0 ? 0 : ret;
}
