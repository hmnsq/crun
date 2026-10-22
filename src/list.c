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

static char doc[] = "OCI runtime";

enum
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

struct list_options_s
{
  bool quiet;
  ціле format;
};

enum
{
  LIST_TABLE = 100,
  LIST_JSON,
};

static struct list_options_s list_options;

static struct argp_option options[]
    = { { "quiet", 'q', 0, 0, "show only IDs", 0 },
        { "format", 'f', "FORMAT", 0, "select one of: table or json (default: \"table\")", 0 },
        {
            0,
        } };

static char args_doc[] = "list";

static error_t
parse_opt (ціле key, char *arg, struct argp_state *state arg_unused)
{
  switch (key)
    {
    case 'q':
      list_options.quiet = true;
      break;
    case 'f':
      якщо (strcmp (arg, "table") == 0)
        list_options.format = LIST_TABLE;
      інакше якщо (strcmp (arg, "json") == 0)
        list_options.format = LIST_JSON;
      інакше
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      break;

    default:
      повернути ARGP_ERR_UNKNOWN;
    }

  повернути 0;
}

static struct argp run_argp = { options, parse_opt, args_doc, doc, NULL, NULL, NULL };

ціле
crun_command_list (struct crun_global_arguments *global_args, ціле argc, char **argv, libcrun_error_t *err)
{
  ціле first_arg;
  ціле ret, max_length = 4;
  cleanup_context libcrun_context_t *crun_context = NULL;
  libcrun_container_list_t *list = NULL;
  libcrun_container_iter_t *it;
  const char *name;

  list_options.format = LIST_TABLE;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &list_options);
  crun_assert_n_args (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  якщо (list_options.format == LIST_JSON)
    {
      cleanup_free char *json = NULL;

      ret = libcrun_container_list_json (crun_context, &json, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;

      fputs (json, stdout);
      повернути 0;
    }

  ret = libcrun_container_list (crun_context, &list, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  для (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
    {
      ціле l = strlen (name);
      якщо (l > max_length)
        max_length = l;
    }
  libcrun_container_iter_free (it);

  max_length++;

  якщо (! list_options.quiet)
    printf ("%-*s%-10s%-8s %-39s %-30s %s\n", max_length, "NAME", "PID", "STATUS", "BUNDLE PATH", "CREATED", "OWNER");

  для (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
    {
      libcrun_status_t *status = NULL;

      ret = libcrun_container_status_load (crun_context, name, &status, err);
      якщо (UNLIKELY (ret < 0))
        {
          libcrun_error_report_and_release (err);
          continue;
        }
      якщо (list_options.quiet)
        printf ("%s\n", name);
      інакше
        {
          libcrun_container_state_t state;
          ціле running = 0;
          ціле pid = libcrun_status_get_pid (status);

          ret = libcrun_status_get_state (status, &state, &running, err);
          якщо (UNLIKELY (ret < 0))
            {
              libcrun_error_report_and_release (err);
              libcrun_container_status_free (status);
              continue;
            }

          якщо (! running)
            pid = 0;

          printf ("%-*s%-10d%-8s %-39s %-30s %s\n", max_length, name, pid,
                  libcrun_container_state_to_string (state), libcrun_status_get_bundle (status),
                  libcrun_status_get_created (status), libcrun_status_get_owner (status));
        }

      libcrun_container_status_free (status);
    }
  libcrun_container_iter_free (it);

  libcrun_container_list_free (list);
  повернути ret >= 0 ? 0 : ret;
}
