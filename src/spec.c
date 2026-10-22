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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <argp.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <errno.h>

#অন্তর্ভাব "crun.h"

স্থির বর্ণ doc[] = "OCI runtime";

সংরচনা spec_options_s
{
  bool rootless;
};

গণনা
{
  OPTION_ROOTLESS = 1000
};

স্থির ধ্রুব বর্ণ *bundle;
স্থির ধ্রুব বর্ণ *fname;

স্থির সংরচনা spec_options_s spec_options;

স্থির সংরচনা argp_option options[] = { { "bundle", 'b', "DIR", 0, "path to the root of the bundle dir (default \".\")", 0 },
                                        { "file", 'f', "PATH", 0, "destination file", 0 },
                                        { "rootless", OPTION_ROOTLESS, 0, 0, "spec for the rootless case", 0 },
                                        {
                                            0,
                                        } };

স্থির বর্ণ args_doc[] = "spec";

স্থির error_t
parse_opt (পূর্ণাঙ্ক চাবি, বর্ণ *arg, সংরচনা argp_state *state)
{
  বিকল্প (চাবি)
    {
    পক্ষ 'b':
      bundle = argp_mandatory_argument (arg, state);
      ভঙ্গ;

    পক্ষ 'f':
      fname = argp_mandatory_argument (arg, state);
      ভঙ্গ;

    পক্ষ OPTION_ROOTLESS:
      spec_options.rootless = true;
      ভঙ্গ;

    স্বভাব:
      প্রত্যাগম ARGP_ERR_UNKNOWN;
    }

  প্রত্যাগম 0;
}

স্থির সংরচনা argp run_argp = { options, parse_opt, args_doc, doc, রিক্ত, রিক্ত, রিক্ত };

পূর্ণাঙ্ক
crun_command_spec (সংরচনা crun_global_arguments *global_args, পূর্ণাঙ্ক argc, বর্ণ **argv, libcrun_error_t *err)
{
  পূর্ণাঙ্ক first_arg;
  cleanup_context libcrun_context_t *crun_context = রিক্ত;
  cleanup_file FILE *f = রিক্ত;
  cleanup_free বর্ণ *bundle_cleanup = রিক্ত;
  cleanup_free বর্ণ *spec = রিক্ত;
  ধ্রুব বর্ণ *where;
  পূর্ণাঙ্ক ret;
  পূর্ণাঙ্ক close_ret;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &spec_options);
  crun_assert_n_args (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  /* Change dir only if -b or --bundle is defined and make sure the bundle is an absolute path.  */
  যদি (bundle != রিক্ত)
    {
      যদি (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, রিক্ত);
          যদি (bundle_cleanup == রিক্ত)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      যদি (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  where = fname ? fname : "config.json";

  যদি (fname == রিক্ত)
    {
      ret = access (where, F_OK);
      যদি (ret == 0)
        প্রত্যাগম libcrun_make_error (err, 0, "`%s` already exists", where);
    }

  f = fopen (where, "w+e");
  যদি (f == রিক্ত)
    প্রত্যাগম libcrun_make_error (err, errno, "cannot open `%s`", where);

  ret = libcrun_container_spec_json (! spec_options.rootless, &spec, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ out;

  যদি (fputs (spec, f) == EOF)
    ret = libcrun_make_error (err, errno, "cannot write to `%s`", where);
  অন্যথা
    ret = 0;

out:
  close_ret = fclose (f);
  f = রিক্ত;
  যদি (close_ret < 0 && ret >= 0)
    প্রত্যাগম libcrun_make_error (err, errno, "cannot close `%s`", where);

  প্রত্যাগম ret;
}
