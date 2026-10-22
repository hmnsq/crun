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

sa_nracanaa spec_options_s
{
  bool rootless;
};

ga_nanaa
{
  OPTION_ROOTLESS = 1000
};

static const var_na *bundle;
static const var_na *fname;

static sa_nracanaa spec_options_s spec_options;

static sa_nracanaa argp_option options[] = { { "bundle", 'b', "DIR", 0, "path to the root of the bundle dir (default \".\")", 0 },
                                        { "file", 'f', "PATH", 0, "destination file", 0 },
                                        { "rootless", OPTION_ROOTLESS, 0, 0, "spec for the rootless case", 0 },
                                        {
                                            0,
                                        } };

static var_na args_doc[] = "spec";

static error_t
parse_opt (puur_naa_nka key, var_na *arg, sa_nracanaa argp_state *state)
{
  switch (key)
    {
    pak_sa 'b':
      bundle = argp_mandatory_argument (arg, state);
      bha_nga;

    pak_sa 'f':
      fname = argp_mandatory_argument (arg, state);
      bha_nga;

    pak_sa OPTION_ROOTLESS:
      spec_options.rootless = true;
      bha_nga;

    default:
      ပွၵ်ႈ ARGP_ERR_UNKNOWN;
    }

  ပွၵ်ႈ 0;
}

static sa_nracanaa argp run_argp = { options, parse_opt, args_doc, doc, NULL, NULL, NULL };

puur_naa_nka
crun_command_spec (sa_nracanaa crun_global_arguments *global_args, puur_naa_nka argc, var_na **argv, libcrun_error_t *err)
{
  puur_naa_nka first_arg;
  cleanup_context libcrun_context_t *crun_context = NULL;
  cleanup_file FILE *f = NULL;
  cleanup_free var_na *bundle_cleanup = NULL;
  cleanup_free var_na *spec = NULL;
  const var_na *where;
  puur_naa_nka ret;
  puur_naa_nka close_ret;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &spec_options);
  crun_assert_n_args (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ပေႃး (UNLIKELY (ret < 0))
    ပွၵ်ႈ ret;

  /* Change dir only if -b or --bundle is defined and make sure the bundle is an absolute path.  */
  ပေႃး (bundle != NULL)
    {
      ပေႃး (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, NULL);
          ပေႃး (bundle_cleanup == NULL)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      ပေႃး (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  where = fname ? fname : "config.json";

  ပေႃး (fname == NULL)
    {
      ret = access (where, F_OK);
      ပေႃး (ret == 0)
        ပွၵ်ႈ libcrun_make_error (err, 0, "`%s` already exists", where);
    }

  f = fopen (where, "w+e");
  ပေႃး (f == NULL)
    ပွၵ်ႈ libcrun_make_error (err, errno, "cannot open `%s`", where);

  ret = libcrun_container_spec_json (! spec_options.rootless, &spec, err);
  ပေႃး (UNLIKELY (ret < 0))
    goto out;

  ပေႃး (fputs (spec, f) == EOF)
    ret = libcrun_make_error (err, errno, "cannot write to `%s`", where);
  ပေႃးဢမ်ႇ
    ret = 0;

out:
  close_ret = fclose (f);
  f = NULL;
  ပေႃး (close_ret < 0 && ret >= 0)
    ပွၵ်ႈ libcrun_make_error (err, errno, "cannot close `%s`", where);

  ပွၵ်ႈ ret;
}
