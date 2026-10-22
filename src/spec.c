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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश <argp.h>
#समावेश <string.h>
#समावेश <unistd.h>
#समावेश <errno.h>

#समावेश "crun.h"

अटल अक्षर doc[] = "OCI runtime";

काष्ठा spec_options_s
{
  bool rootless;
};

क्रमागत
{
  OPTION_ROOTLESS = 1000
};

अटल स्थिर अक्षर *bundle;
अटल स्थिर अक्षर *fname;

अटल काष्ठा spec_options_s spec_options;

अटल काष्ठा argp_option options[] = { { "bundle", 'b', "DIR", 0, "path to the root of the bundle dir (default \".\")", 0 },
                                        { "file", 'f', "PATH", 0, "destination file", 0 },
                                        { "rootless", OPTION_ROOTLESS, 0, 0, "spec for the rootless case", 0 },
                                        {
                                            0,
                                        } };

अटल अक्षर args_doc[] = "spec";

अटल error_t
parse_opt (पूर्णांक कुंजी, अक्षर *arg, काष्ठा argp_state *state)
{
  चयन (कुंजी)
    {
    हाल 'b':
      bundle = argp_mandatory_argument (arg, state);
      अवरोध;

    हाल 'f':
      fname = argp_mandatory_argument (arg, state);
      अवरोध;

    हाल OPTION_ROOTLESS:
      spec_options.rootless = true;
      अवरोध;

    शेष:
      वापस ARGP_ERR_UNKNOWN;
    }

  वापस 0;
}

अटल काष्ठा argp run_argp = { options, parse_opt, args_doc, doc, शून्य, शून्य, शून्य };

पूर्णांक
crun_command_spec (काष्ठा crun_global_arguments *global_args, पूर्णांक argc, अक्षर **argv, libcrun_error_t *err)
{
  पूर्णांक first_arg;
  cleanup_context libcrun_context_t *crun_context = शून्य;
  cleanup_file खाता *f = शून्य;
  cleanup_free अक्षर *bundle_cleanup = शून्य;
  cleanup_free अक्षर *spec = शून्य;
  स्थिर अक्षर *where;
  पूर्णांक ret;
  पूर्णांक close_ret;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &spec_options);
  crun_assert_n_args (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  /* Change dir only if -b or --bundle is defined and make sure the bundle is an absolute path.  */
  अगर (bundle != शून्य)
    {
      अगर (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, शून्य);
          अगर (bundle_cleanup == शून्य)
            libcrun_fail_with_error (त्रुटि_सं, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      अगर (स_बदलो (bundle) < 0)
        libcrun_fail_with_error (त्रुटि_सं, "chdir `%s` failed", bundle);
    }

  where = fname ? fname : "config.json";

  अगर (fname == शून्य)
    {
      ret = access (where, F_OK);
      अगर (ret == 0)
        वापस libcrun_make_error (err, 0, "`%s` already exists", where);
    }

  f = ख_खोलो (where, "w+e");
  अगर (f == शून्य)
    वापस libcrun_make_error (err, त्रुटि_सं, "cannot open `%s`", where);

  ret = libcrun_container_spec_json (! spec_options.rootless, &spec, err);
  अगर (UNLIKELY (ret < 0))
    जाओ out;

  अगर (ख_माला_दो (spec, f) == खातापूर्ण)
    ret = libcrun_make_error (err, त्रुटि_सं, "cannot write to `%s`", where);
  अन्यथा
    ret = 0;

out:
  close_ret = ख_बंद (f);
  f = शून्य;
  अगर (close_ret < 0 && ret >= 0)
    वापस libcrun_make_error (err, त्रुटि_सं, "cannot close `%s`", where);

  वापस ret;
}
