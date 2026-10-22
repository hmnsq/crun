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

#அந்தர்பாவ "crun.h"

நிலையான எழுத்து doc[] = "OCI runtime";

கட்டமைப்பு spec_options_s
{
  bool rootless;
};

எண்ணிக்கை
{
  OPTION_ROOTLESS = 1000
};

நிலையான மாறிலி எழுத்து *bundle;
நிலையான மாறிலி எழுத்து *fname;

நிலையான கட்டமைப்பு spec_options_s spec_options;

நிலையான கட்டமைப்பு argp_option options[] = { { "bundle", 'b', "DIR", 0, "path to the root of the bundle dir (default \".\")", 0 },
                                        { "file", 'f', "PATH", 0, "destination file", 0 },
                                        { "rootless", OPTION_ROOTLESS, 0, 0, "spec for the rootless case", 0 },
                                        {
                                            0,
                                        } };

நிலையான எழுத்து args_doc[] = "spec";

நிலையான error_t
parse_opt (முழு சாவி, எழுத்து *arg, கட்டமைப்பு argp_state *state)
{
  தேர்வு (சாவி)
    {
    நிலை 'b':
      bundle = argp_mandatory_argument (arg, state);
      நிறுத்து;

    நிலை 'f':
      fname = argp_mandatory_argument (arg, state);
      நிறுத்து;

    நிலை OPTION_ROOTLESS:
      spec_options.rootless = true;
      நிறுத்து;

    இயல்பு:
      திரும்பு ARGP_ERR_UNKNOWN;
    }

  திரும்பு 0;
}

நிலையான கட்டமைப்பு argp run_argp = { options, parse_opt, args_doc, doc, வெற்று, வெற்று, வெற்று };

முழு
crun_command_spec (கட்டமைப்பு crun_global_arguments *global_args, முழு argc, எழுத்து **argv, libcrun_error_t *err)
{
  முழு first_arg;
  cleanup_context libcrun_context_t *crun_context = வெற்று;
  cleanup_file FILE *f = வெற்று;
  cleanup_free எழுத்து *bundle_cleanup = வெற்று;
  cleanup_free எழுத்து *spec = வெற்று;
  மாறிலி எழுத்து *where;
  முழு ret;
  முழு close_ret;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &spec_options);
  crun_assert_n_args (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  /* Change dir only if -b or --bundle is defined and make sure the bundle is an absolute path.  */
  எனில் (bundle != வெற்று)
    {
      எனில் (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, வெற்று);
          எனில் (bundle_cleanup == வெற்று)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      எனில் (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  where = fname ? fname : "config.json";

  எனில் (fname == வெற்று)
    {
      ret = access (where, F_OK);
      எனில் (ret == 0)
        திரும்பு libcrun_make_error (err, 0, "`%s` already exists", where);
    }

  f = fopen (where, "w+e");
  எனில் (f == வெற்று)
    திரும்பு libcrun_make_error (err, errno, "cannot open `%s`", where);

  ret = libcrun_container_spec_json (! spec_options.rootless, &spec, err);
  எனில் (UNLIKELY (ret < 0))
    செல் out;

  எனில் (fputs (spec, f) == EOF)
    ret = libcrun_make_error (err, errno, "cannot write to `%s`", where);
  இல்லையெனில்
    ret = 0;

out:
  close_ret = fclose (f);
  f = வெற்று;
  எனில் (close_ret < 0 && ret >= 0)
    திரும்பு libcrun_make_error (err, errno, "cannot close `%s`", where);

  திரும்பு ret;
}
