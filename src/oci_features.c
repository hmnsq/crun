/*
 * crun - OCI runtime written in C
 *
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

static struct argp_option options[] = { { 0 } };

static char args_doc[] = "features";

static error_t
parse_opt (số_nguyên key, char *arg arg_unused, struct argp_state *state arg_unused)
{
  nếu (key != ARGP_KEY_NO_ARGS)
    {
      trả_về ARGP_ERR_UNKNOWN;
    }

  trả_về 0;
}

static struct argp run_argp = { options, parse_opt, args_doc, doc, NULL, NULL, NULL };

số_nguyên
crun_command_features (struct crun_global_arguments *global_args, số_nguyên argc, char **argv, libcrun_error_t *err)
{
  cleanup_free char *json = NULL;
  số_nguyên first_arg = 0, ret = 0;
  cleanup_context libcrun_context_t *crun_context = NULL;

  argp_parse (&run_argp, argc, argv, 0, 0, &options);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  ret = libcrun_container_get_features_json (crun_context, &json, err);
  nếu (UNLIKELY (ret < 0))
    trả_về ret;

  printf ("%s", json);

  trả_về 0;
}
