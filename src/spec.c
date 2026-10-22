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

ایستا نویسه doc[] = "OCI runtime";

ساختار spec_options_s
{
  bool rootless;
};

شمارش
{
  OPTION_ROOTLESS = 1000
};

ایستا ثابت نویسه *bundle;
ایستا ثابت نویسه *fname;

ایستا ساختار spec_options_s spec_options;

ایستا ساختار argp_option options[] = { { "bundle", 'b', "DIR", 0, "path to the root of the bundle dir (default \".\")", 0 },
                                        { "file", 'f', "PATH", 0, "destination file", 0 },
                                        { "rootless", OPTION_ROOTLESS, 0, 0, "spec for the rootless case", 0 },
                                        {
                                            0,
                                        } };

ایستا نویسه args_doc[] = "spec";

ایستا error_t
parse_opt (صحیح کلید, نویسه *arg, ساختار argp_state *state)
{
  انتخاب (کلید)
    {
    حالت 'b':
      bundle = argp_mandatory_argument (arg, state);
      بشکن;

    حالت 'f':
      fname = argp_mandatory_argument (arg, state);
      بشکن;

    حالت OPTION_ROOTLESS:
      spec_options.rootless = true;
      بشکن;

    پیشفرض:
      بازگشت ARGP_ERR_UNKNOWN;
    }

  بازگشت 0;
}

ایستا ساختار argp run_argp = { options, parse_opt, args_doc, doc, پوچ, پوچ, پوچ };

صحیح
crun_command_spec (ساختار crun_global_arguments *global_args, صحیح argc, نویسه **argv, libcrun_error_t *err)
{
  صحیح first_arg;
  cleanup_context libcrun_context_t *crun_context = پوچ;
  cleanup_file FILE *f = پوچ;
  cleanup_free نویسه *bundle_cleanup = پوچ;
  cleanup_free نویسه *spec = پوچ;
  ثابت نویسه *where;
  صحیح ret;
  صحیح close_ret;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &spec_options);
  crun_assert_n_args (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  /* Change dir only if -b or --bundle is defined and make sure the bundle is an absolute path.  */
  اگر (bundle != پوچ)
    {
      اگر (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, پوچ);
          اگر (bundle_cleanup == پوچ)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      اگر (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  where = fname ? fname : "config.json";

  اگر (fname == پوچ)
    {
      ret = access (where, F_OK);
      اگر (ret == 0)
        بازگشت libcrun_make_error (err, 0, "`%s` already exists", where);
    }

  f = fopen (where, "w+e");
  اگر (f == پوچ)
    بازگشت libcrun_make_error (err, errno, "cannot open `%s`", where);

  ret = libcrun_container_spec_json (! spec_options.rootless, &spec, err);
  اگر (UNLIKELY (ret < 0))
    برو out;

  اگر (fputs (spec, f) == EOF)
    ret = libcrun_make_error (err, errno, "cannot write to `%s`", where);
  وگرنه
    ret = 0;

out:
  close_ret = fclose (f);
  f = پوچ;
  اگر (close_ret < 0 && ret >= 0)
    بازگشت libcrun_make_error (err, errno, "cannot close `%s`", where);

  بازگشت ret;
}
