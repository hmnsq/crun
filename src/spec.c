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

स्थिर वर्ण doc[] = "OCI runtime";

संरचना spec_options_s
{
  bool rootless;
};

गणना
{
  OPTION_ROOTLESS = 1000
};

स्थिर ध्रुव वर्ण *bundle;
स्थिर ध्रुव वर्ण *fname;

स्थिर संरचना spec_options_s spec_options;

स्थिर संरचना argp_option options[] = { { "bundle", 'b', "DIR", 0, "path to the root of the bundle dir (default \".\")", 0 },
                                        { "file", 'f', "PATH", 0, "destination file", 0 },
                                        { "rootless", OPTION_ROOTLESS, 0, 0, "spec for the rootless case", 0 },
                                        {
                                            0,
                                        } };

स्थिर वर्ण तर्काः_doc[] = "spec";

स्थिर error_t
parse_opt (पूर्णाङ्क कुञ्चिका, वर्ण *arg, संरचना argp_state *state)
{
  विकल्प (कुञ्चिका)
    {
    पक्ष 'b':
      bundle = argp_mandatory_तर्कः (arg, state);
      भङ्ग;

    पक्ष 'f':
      fname = argp_mandatory_तर्कः (arg, state);
      भङ्ग;

    पक्ष OPTION_ROOTLESS:
      spec_options.rootless = true;
      भङ्ग;

    स्वभाव:
      प्रत्यागम ARGP_ERR_UNKNOWN;
    }

  प्रत्यागम 0;
}

स्थिर संरचना argp सततम्_argp = { options, parse_opt, तर्काः_doc, doc, रिक्त, रिक्त, रिक्त };

पूर्णाङ्क
crun_command_spec (संरचना crun_global_arguments *global_तर्काः, पूर्णाङ्क argc, वर्ण **argv, libcrun_error_t *err)
{
  पूर्णाङ्क first_arg;
  cleanup_context libcrun_context_t *crun_context = रिक्त;
  cleanup_file FILE *f = रिक्त;
  cleanup_free वर्ण *bundle_cleanup = रिक्त;
  cleanup_free वर्ण *spec = रिक्त;
  ध्रुव वर्ण *where;
  पूर्णाङ्क ret;
  पूर्णाङ्क close_ret;

  argp_parse (&सततम्_argp, argc, argv, ARGP_अन्तर्_क्रमः, &first_arg, &spec_options);
  crun_assert_n_तर्काः (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_तर्काः);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_तर्काः, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* Change dir only if -b or --bundle is defined and make sure the bundle is an absolute path.  */
  यदि (bundle != रिक्त)
    {
      यदि (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, रिक्त);
          यदि (bundle_cleanup == रिक्त)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      यदि (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  where = fname ? fname : "config.json";

  यदि (fname == रिक्त)
    {
      ret = access (where, F_OK);
      यदि (ret == 0)
        प्रत्यागम libcrun_make_error (err, 0, "`%s` already exists", where);
    }

  f = fopen (where, "w+e");
  यदि (f == रिक्त)
    प्रत्यागम libcrun_make_error (err, errno, "cannot open `%s`", where);

  ret = libcrun_container_spec_json (! spec_options.rootless, &spec, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ बहिर्गमनम्;

  यदि (fputs (spec, f) == EOF)
    ret = libcrun_make_error (err, errno, "cannot write to `%s`", where);
  अन्यथा
    ret = 0;

बहिर्गमनम्:
  close_ret = fclose (f);
  f = रिक्त;
  यदि (close_ret < 0 && ret >= 0)
    प्रत्यागम libcrun_make_error (err, errno, "cannot close `%s`", where);

  प्रत्यागम ret;
}
