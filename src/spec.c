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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <argp.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <errno.h>

#ಅನ್ತರ್ಭಾವ "crun.h"

ಸ್ಥಿರ ವರ್ಣ doc[] = "OCI runtime";

ಸಂರಚನಾ spec_options_s
{
  bool rootless;
};

ಗಣನಾ
{
  OPTION_ROOTLESS = 1000
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *bundle;
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *fname;

ಸ್ಥಿರ ಸಂರಚನಾ spec_options_s spec_options;

ಸ್ಥಿರ ಸಂರಚನಾ argp_option options[] = { { "bundle", 'b', "DIR", 0, "path to the root of the bundle dir (default \".\")", 0 },
                                        { "file", 'f', "PATH", 0, "destination file", 0 },
                                        { "rootless", OPTION_ROOTLESS, 0, 0, "spec for the rootless case", 0 },
                                        {
                                            0,
                                        } };

ಸ್ಥಿರ ವರ್ಣ args_doc[] = "spec";

ಸ್ಥಿರ error_t
parse_opt (ಪೂರ್ಣಾಙ್ಕ ಕೀಲಿ, ವರ್ಣ *arg, ಸಂರಚನಾ argp_state *state)
{
  ವಿಕಲ್ಪ (ಕೀಲಿ)
    {
    ಪಕ್ಷ 'b':
      bundle = argp_mandatory_argument (arg, state);
      ಭಙ್ಗ;

    ಪಕ್ಷ 'f':
      fname = argp_mandatory_argument (arg, state);
      ಭಙ್ಗ;

    ಪಕ್ಷ OPTION_ROOTLESS:
      spec_options.rootless = true;
      ಭಙ್ಗ;

    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ ARGP_ERR_UNKNOWN;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಸಂರಚನಾ argp run_argp = { options, parse_opt, args_doc, doc, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ };

ಪೂರ್ಣಾಙ್ಕ
crun_command_spec (ಸಂರಚನಾ crun_global_arguments *global_args, ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ first_arg;
  cleanup_context libcrun_context_t *crun_context = ರಿಕ್ತ;
  cleanup_file FILE *f = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *bundle_cleanup = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *spec = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *where;
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಪೂರ್ಣಾಙ್ಕ close_ret;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &spec_options);
  crun_assert_n_args (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* Change dir only if -b or --bundle is defined and make sure the bundle is an absolute path.  */
  ಯದಿ (bundle != ರಿಕ್ತ)
    {
      ಯದಿ (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, ರಿಕ್ತ);
          ಯದಿ (bundle_cleanup == ರಿಕ್ತ)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      ಯದಿ (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  where = fname ? fname : "config.json";

  ಯದಿ (fname == ರಿಕ್ತ)
    {
      ret = access (where, F_OK);
      ಯದಿ (ret == 0)
        ಪ್ರತ್ಯಾಗಮ libcrun_make_error (err, 0, "`%s` already exists", where);
    }

  f = fopen (where, "w+e");
  ಯದಿ (f == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ libcrun_make_error (err, errno, "cannot open `%s`", where);

  ret = libcrun_container_spec_json (! spec_options.rootless, &spec, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ out;

  ಯದಿ (fputs (spec, f) == EOF)
    ret = libcrun_make_error (err, errno, "cannot write to `%s`", where);
  ಅನ್ಯಥಾ
    ret = 0;

out:
  close_ret = fclose (f);
  f = ರಿಕ್ತ;
  ಯದಿ (close_ret < 0 && ret >= 0)
    ಪ್ರತ್ಯಾಗಮ libcrun_make_error (err, errno, "cannot close `%s`", where);

  ಪ್ರತ್ಯಾಗಮ ret;
}
