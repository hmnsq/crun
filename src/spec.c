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

#شامل <config.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <argp.h>
#شامل <string.h>
#شامل <unistd.h>
#شامل <errno.h>

#شامل "crun.h"

ساکن حرف doc[] = "OCI runtime";

ساخت spec_options_s
{
  bool rootless;
};

شمار
{
  OPTION_ROOTLESS = 1000
};

ساکن ثابت حرف *bundle;
ساکن ثابت حرف *fname;

ساکن ساخت spec_options_s spec_options;

ساکن ساخت argp_option options[] = { { "bundle", 'b', "DIR", 0, "path to the root of the bundle dir (default \".\")", 0 },
                                        { "file", 'f', "PATH", 0, "destination file", 0 },
                                        { "rootless", OPTION_ROOTLESS, 0, 0, "spec for the rootless case", 0 },
                                        {
                                            0,
                                        } };

ساکن حرف دلائل_doc[] = "spec";

ساکن error_t
parse_opt (صحیح کلید, حرف *arg, ساخت argp_state *state)
{
  انتخاب (کلید)
    {
    حالت 'b':
      bundle = argp_mandatory_دلیل (arg, state);
      توڑو;

    حالت 'f':
      fname = argp_mandatory_دلیل (arg, state);
      توڑو;

    حالت OPTION_ROOTLESS:
      spec_options.rootless = true;
      توڑو;

    طےشدہ:
      واپس ARGP_ERR_UNKNOWN;
    }

  واپس 0;
}

ساکن ساخت argp تسلسل_argp = { options, parse_opt, دلائل_doc, doc, تہی, تہی, تہی };

صحیح
crun_command_spec (ساخت crun_global_arguments *global_دلائل, صحیح argc, حرف **argv, libcrun_error_t *err)
{
  صحیح پہلا_arg;
  cleanup_context libcrun_context_t *crun_context = تہی;
  cleanup_file FILE *f = تہی;
  cleanup_free حرف *bundle_cleanup = تہی;
  cleanup_free حرف *spec = تہی;
  ثابت حرف *where;
  صحیح ret;
  صحیح close_ret;

  argp_parse (&تسلسل_argp, argc, argv, ARGP_اندر_ترتیب, &پہلا_arg, &spec_options);
  crun_assert_n_دلائل (argc - پہلا_arg, 0, 0);

  crun_context = new_libcrun_context (global_دلائل);

  ret = init_libcrun_context (crun_context, argv[پہلا_arg], global_دلائل, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  /* Change dir only if -b or --bundle is defined and make sure the bundle is an absolute path.  */
  اگر (bundle != تہی)
    {
      اگر (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, تہی);
          اگر (bundle_cleanup == تہی)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      اگر (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  where = fname ? fname : "config.json";

  اگر (fname == تہی)
    {
      ret = access (where, F_OK);
      اگر (ret == 0)
        واپس libcrun_make_error (err, 0, "`%s` already exists", where);
    }

  f = fopen (where, "w+e");
  اگر (f == تہی)
    واپس libcrun_make_error (err, errno, "cannot open `%s`", where);

  ret = libcrun_container_spec_json (! spec_options.rootless, &spec, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ برآمد;

  اگر (fputs (spec, f) == EOF)
    ret = libcrun_make_error (err, errno, "cannot write to `%s`", where);
  ورنہ
    ret = 0;

برآمد:
  close_ret = fclose (f);
  f = تہی;
  اگر (close_ret < 0 && ret >= 0)
    واپس libcrun_make_error (err, errno, "cannot close `%s`", where);

  واپس ret;
}
