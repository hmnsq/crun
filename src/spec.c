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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <argp.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <errno.h>

#अन्तर्भाव "crun.h"

स्थिर अक्षर doc[] = "OCI runtime";

संरचना spec_options_s
{
  bool rootless;
};

गणना
{
  OPTION_ROOTLESS = 1000
};

स्थिर स्थिरांक अक्षर *bundle;
स्थिर स्थिरांक अक्षर *fname;

स्थिर संरचना spec_options_s spec_options;

स्थिर संरचना argp_option options[] = { { "bundle", 'b', "DIR", 0, "path to the root of the bundle dir (default \".\")", 0 },
                                        { "file", 'f', "PATH", 0, "destination file", 0 },
                                        { "rootless", OPTION_ROOTLESS, 0, 0, "spec for the rootless case", 0 },
                                        {
                                            0,
                                        } };

स्थिर अक्षर args_doc[] = "spec";

स्थिर error_t
parse_opt (पूर्णांक किल्ली, अक्षर *arg, संरचना argp_state *state)
{
  निवड (किल्ली)
    {
    स्थिती 'b':
      bundle = argp_mandatory_argument (arg, state);
      थांबा;

    स्थिती 'f':
      fname = argp_mandatory_argument (arg, state);
      थांबा;

    स्थिती OPTION_ROOTLESS:
      spec_options.rootless = true;
      थांबा;

    पूर्वनिर्धारित:
      परत ARGP_ERR_UNKNOWN;
    }

  परत 0;
}

स्थिर संरचना argp run_argp = { options, parse_opt, args_doc, doc, रिक्त, रिक्त, रिक्त };

पूर्णांक
crun_command_spec (संरचना crun_global_arguments *global_args, पूर्णांक argc, अक्षर **argv, libcrun_error_t *err)
{
  पूर्णांक first_arg;
  cleanup_context libcrun_context_t *crun_context = रिक्त;
  cleanup_file खाता *f = रिक्त;
  cleanup_free अक्षर *bundle_cleanup = रिक्त;
  cleanup_free अक्षर *spec = रिक्त;
  स्थिरांक अक्षर *where;
  पूर्णांक ret;
  पूर्णांक close_ret;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &spec_options);
  crun_assert_n_args (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  /* Change dir only if -b or --bundle is defined and make sure the bundle is an absolute path.  */
  जर (bundle != रिक्त)
    {
      जर (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, रिक्त);
          जर (bundle_cleanup == रिक्त)
            libcrun_fail_with_error (त्रुटि_सं, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      जर (स_बदलो (bundle) < 0)
        libcrun_fail_with_error (त्रुटि_सं, "chdir `%s` failed", bundle);
    }

  where = fname ? fname : "config.json";

  जर (fname == रिक्त)
    {
      ret = access (where, F_OK);
      जर (ret == 0)
        परत libcrun_make_error (err, 0, "`%s` already exists", where);
    }

  f = ख_खोलो (where, "w+e");
  जर (f == रिक्त)
    परत libcrun_make_error (err, त्रुटि_सं, "cannot open `%s`", where);

  ret = libcrun_container_spec_json (! spec_options.rootless, &spec, err);
  जर (UNLIKELY (ret < 0))
    जा out;

  जर (ख_माला_दो (spec, f) == खातापूर्ण)
    ret = libcrun_make_error (err, त्रुटि_सं, "cannot write to `%s`", where);
  अन्यथा
    ret = 0;

out:
  close_ret = ख_बंद (f);
  f = रिक्त;
  जर (close_ret < 0 && ret >= 0)
    परत libcrun_make_error (err, त्रुटि_सं, "cannot close `%s`", where);

  परत ret;
}
