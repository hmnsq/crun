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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <argp.h>
#ضمن <string.h>
#ضمن <unistd.h>
#ضمن <errno.h>

#ضمن "crun.h"

ساكن حرف doc[] = "OCI runtime";

بنية spec_options_s
{
  bool rootless;
};

تعداد
{
  OPTION_ROOTLESS = 1000
};

ساكن ثابت حرف *bundle;
ساكن ثابت حرف *fname;

ساكن بنية spec_options_s spec_options;

ساكن بنية argp_option options[] = { { "bundle", 'b', "DIR", 0, "path to the root of the bundle dir (default \".\")", 0 },
                                        { "file", 'f', "PATH", 0, "destination file", 0 },
                                        { "rootless", OPTION_ROOTLESS, 0, 0, "spec for the rootless case", 0 },
                                        {
                                            0,
                                        } };

ساكن حرف args_doc[] = "spec";

ساكن error_t
parse_opt (صحيح کلیل, حرف *arg, بنية argp_state *state)
{
  بدل (کلیل)
    {
    حالة 'b':
      bundle = argp_mandatory_argument (arg, state);
      اكسر;

    حالة 'f':
      fname = argp_mandatory_argument (arg, state);
      اكسر;

    حالة OPTION_ROOTLESS:
      spec_options.rootless = true;
      اكسر;

    افتراضي:
      أرجع ARGP_ERR_UNKNOWN;
    }

  أرجع 0;
}

ساكن بنية argp run_argp = { options, parse_opt, args_doc, doc, عدم, عدم, عدم };

صحيح
crun_command_spec (بنية crun_global_arguments *global_args, صحيح argc, حرف **argv, libcrun_error_t *err)
{
  صحيح first_arg;
  cleanup_context libcrun_context_t *crun_context = عدم;
  cleanup_file FILE *f = عدم;
  cleanup_free حرف *bundle_cleanup = عدم;
  cleanup_free حرف *spec = عدم;
  ثابت حرف *where;
  صحيح ret;
  صحيح close_ret;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &spec_options);
  crun_assert_n_args (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* Change dir only if -b or --bundle is defined and make sure the bundle is an absolute path.  */
  إذا (bundle != عدم)
    {
      إذا (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, عدم);
          إذا (bundle_cleanup == عدم)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      إذا (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  where = fname ? fname : "config.json";

  إذا (fname == عدم)
    {
      ret = access (where, F_OK);
      إذا (ret == 0)
        أرجع libcrun_make_error (err, 0, "`%s` already exists", where);
    }

  f = fopen (where, "w+e");
  إذا (f == عدم)
    أرجع libcrun_make_error (err, errno, "cannot open `%s`", where);

  ret = libcrun_container_spec_json (! spec_options.rootless, &spec, err);
  إذا (UNLIKELY (ret < 0))
    اذهب out;

  إذا (fputs (spec, f) == EOF)
    ret = libcrun_make_error (err, errno, "cannot write to `%s`", where);
  وإلا
    ret = 0;

out:
  close_ret = fclose (f);
  f = عدم;
  إذا (close_ret < 0 && ret >= 0)
    أرجع libcrun_make_error (err, errno, "cannot close `%s`", where);

  أرجع ret;
}
