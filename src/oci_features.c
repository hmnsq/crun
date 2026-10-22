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

static var_na doc[] = "OCI runtime";

static sa_nracanaa argp_option options[] = { { 0 } };

static var_na args_doc[] = "features";

static error_t
parse_opt (puur_naa_nka key, var_na *arg arg_unused, sa_nracanaa argp_state *state arg_unused)
{
  ᬬᬾᬦ᭄ (key != ARGP_KEY_NO_ARGS)
    {
      ᬯᬮᬶ ARGP_ERR_UNKNOWN;
    }

  ᬯᬮᬶ 0;
}

static sa_nracanaa argp run_argp = { options, parse_opt, args_doc, doc, NULL, NULL, NULL };

puur_naa_nka
crun_command_features (sa_nracanaa crun_global_arguments *global_args, puur_naa_nka argc, var_na **argv, libcrun_error_t *err)
{
  cleanup_free var_na *json = NULL;
  puur_naa_nka first_arg = 0, ret = 0;
  cleanup_context libcrun_context_t *crun_context = NULL;

  argp_parse (&run_argp, argc, argv, 0, 0, &options);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ ret;

  ret = libcrun_container_get_features_json (crun_context, &json, err);
  ᬬᬾᬦ᭄ (UNLIKELY (ret < 0))
    ᬯᬮᬶ ret;

  printf ("%s", json);

  ᬯᬮᬶ 0;
}
