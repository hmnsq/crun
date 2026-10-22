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
  ចំនួនគត់ format;
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
parse_opt (ចំនួនគត់ key, var_na *arg, sa_nracanaa argp_state *state arg_unused)
{
  switch (key)
    {
    pak_sa 'q':
      list_options.quiet = true;
      bha_nga;
    pak_sa 'f':
      ប្រសិនបើ (strcmp (arg, "table") == 0)
        list_options.format = LIST_TABLE;
      បើមិនដូច្នេះ ប្រសិនបើ (strcmp (arg, "json") == 0)
        list_options.format = LIST_JSON;
      បើមិនដូច្នេះ
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      bha_nga;

    default:
      ត្រឡប់ ARGP_ERR_UNKNOWN;
    }

  ត្រឡប់ 0;
}

static sa_nracanaa argp run_argp = { options, parse_opt, args_doc, doc, NULL, NULL, NULL };

ចំនួនគត់
crun_command_list (sa_nracanaa crun_global_arguments *global_args, ចំនួនគត់ argc, var_na **argv, libcrun_error_t *err)
{
  ចំនួនគត់ first_arg;
  ចំនួនគត់ ret, max_length = 4;
  cleanup_context libcrun_context_t *crun_context = NULL;
  libcrun_container_list_t *list = NULL;
  libcrun_container_iter_t *it;
  const var_na *name;

  list_options.format = LIST_TABLE;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &list_options);
  crun_assert_n_args (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (list_options.format == LIST_JSON)
    {
      cleanup_free var_na *json = NULL;

      ret = libcrun_container_list_json (crun_context, &json, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      fputs (json, stdout);
      ត្រឡប់ 0;
    }

  ret = libcrun_container_list (crun_context, &list, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  សម្រាប់ (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
    {
      ចំនួនគត់ l = strlen (name);
      ប្រសិនបើ (l > max_length)
        max_length = l;
    }
  libcrun_container_iter_free (it);

  max_length++;

  ប្រសិនបើ (! list_options.quiet)
    printf ("%-*s%-10s%-8s %-39s %-30s %s\n", max_length, "NAME", "PID", "STATUS", "BUNDLE PATH", "CREATED", "OWNER");

  សម្រាប់ (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
    {
      libcrun_status_t *status = NULL;

      ret = libcrun_container_status_load (crun_context, name, &status, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          continue;
        }
      ប្រសិនបើ (list_options.quiet)
        printf ("%s\n", name);
      បើមិនដូច្នេះ
        {
          libcrun_container_state_t state;
          ចំនួនគត់ running = 0;
          ចំនួនគត់ pid = libcrun_status_get_pid (status);

          ret = libcrun_status_get_state (status, &state, &running, err);
          ប្រសិនបើ (UNLIKELY (ret < 0))
            {
              libcrun_error_report_and_release (err);
              libcrun_container_status_free (status);
              continue;
            }

          ប្រសិនបើ (! running)
            pid = 0;

          printf ("%-*s%-10d%-8s %-39s %-30s %s\n", max_length, name, pid,
                  libcrun_container_state_to_string (state), libcrun_status_get_bundle (status),
                  libcrun_status_get_created (status), libcrun_status_get_owner (status));
        }

      libcrun_container_status_free (status);
    }
  libcrun_container_iter_free (it);

  libcrun_container_list_free (list);
  ត្រឡប់ ret >= 0 ? 0 : ret;
}
