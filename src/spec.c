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

ಸ್ಥಿರ ಅಕ್ಷರ doc[] = "OCI runtime";

ರಚನೆ spec_options_s
{
  bool rootless;
};

ಎಣಿಕೆ
{
  OPTION_ROOTLESS = 1000
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *bundle;
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fname;

ಸ್ಥಿರ ರಚನೆ spec_options_s spec_options;

ಸ್ಥಿರ ರಚನೆ argp_option options[] = { { "bundle", 'b', "DIR", 0, "path to the root of the bundle dir (default \".\")", 0 },
                                        { "file", 'f', "PATH", 0, "destination file", 0 },
                                        { "rootless", OPTION_ROOTLESS, 0, 0, "spec for the rootless case", 0 },
                                        {
                                            0,
                                        } };

ಸ್ಥಿರ ಅಕ್ಷರ ವಾದಗಳು_doc[] = "spec";

ಸ್ಥಿರ error_t
parse_opt (ಪೂರ್ಣಾಂಕ ಕೀಲಿ, ಅಕ್ಷರ *arg, ರಚನೆ argp_state *state)
{
  ಆಯ್ಕೆ (ಕೀಲಿ)
    {
    ಸ್ಥಿತಿ 'b':
      bundle = argp_mandatory_ವಾದ (arg, state);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'f':
      fname = argp_mandatory_ವಾದ (arg, state);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ OPTION_ROOTLESS:
      spec_options.rootless = true;
      ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ ARGP_ERR_UNKNOWN;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ರಚನೆ argp ಸತತ_argp = { options, parse_opt, ವಾದಗಳು_doc, doc, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ };

ಪೂರ್ಣಾಂಕ
crun_command_spec (ರಚನೆ crun_global_arguments *global_ವಾದಗಳು, ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ಮೊದಲ_arg;
  cleanup_context libcrun_context_t *crun_context = ಖಾಲಿ;
  cleanup_file FILE *f = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *bundle_cleanup = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *spec = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *where;
  ಪೂರ್ಣಾಂಕ ret;
  ಪೂರ್ಣಾಂಕ close_ret;

  argp_parse (&ಸತತ_argp, argc, argv, ARGP_ಒಳ_ಕ್ರಮ, &ಮೊದಲ_arg, &spec_options);
  crun_assert_n_ವಾದಗಳು (argc - ಮೊದಲ_arg, 0, 0);

  crun_context = new_libcrun_context (global_ವಾದಗಳು);

  ret = init_libcrun_context (crun_context, argv[ಮೊದಲ_arg], global_ವಾದಗಳು, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  /* Change dir only if -b or --bundle is defined and make sure the bundle is an absolute path.  */
  ಆದರೆ (bundle != ಖಾಲಿ)
    {
      ಆದರೆ (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, ಖಾಲಿ);
          ಆದರೆ (bundle_cleanup == ಖಾಲಿ)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      ಆದರೆ (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  where = fname ? fname : "config.json";

  ಆದರೆ (fname == ಖಾಲಿ)
    {
      ret = access (where, F_OK);
      ಆದರೆ (ret == 0)
        ಹಿಂತಿರುಗಿ libcrun_make_error (err, 0, "`%s` already exists", where);
    }

  f = fopen (where, "w+e");
  ಆದರೆ (f == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ libcrun_make_error (err, errno, "cannot open `%s`", where);

  ret = libcrun_container_spec_json (! spec_options.rootless, &spec, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು ಹೊರಹೋಗು;

  ಆದರೆ (fputs (spec, f) == EOF)
    ret = libcrun_make_error (err, errno, "cannot write to `%s`", where);
  ಇಲ್ಲದಿದ್ದರೆ
    ret = 0;

ಹೊರಹೋಗು:
  close_ret = fclose (f);
  f = ಖಾಲಿ;
  ಆದರೆ (close_ret < 0 && ret >= 0)
    ಹಿಂತಿರುಗಿ libcrun_make_error (err, errno, "cannot close `%s`", where);

  ಹಿಂತಿರುಗಿ ret;
}
