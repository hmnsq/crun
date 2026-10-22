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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <argp.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>

#ଅନ୍ତର୍ଭାଵ "crun.h"

ସ୍ଥିର ଅକ୍ଷର doc[] = "OCI runtime";

ସଂରଚନା spec_options_s
{
  bool rootless;
};

ଗଣନା
{
  OPTION_ROOTLESS = 1000
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *bundle;
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *fname;

ସ୍ଥିର ସଂରଚନା spec_options_s spec_options;

ସ୍ଥିର ସଂରଚନା argp_option options[] = { { "bundle", 'b', "DIR", 0, "path to the root of the bundle dir (default \".\")", 0 },
                                        { "file", 'f', "PATH", 0, "destination file", 0 },
                                        { "rootless", OPTION_ROOTLESS, 0, 0, "spec for the rootless case", 0 },
                                        {
                                            0,
                                        } };

ସ୍ଥିର ଅକ୍ଷର args_doc[] = "spec";

ସ୍ଥିର error_t
parse_opt (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଚାବି, ଅକ୍ଷର *arg, ସଂରଚନା argp_state *state)
{
  ବାଛ (ଚାବି)
    {
    ସ୍ଥିତି 'b':
      bundle = argp_mandatory_argument (arg, state);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 'f':
      fname = argp_mandatory_argument (arg, state);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି OPTION_ROOTLESS:
      spec_options.rootless = true;
      ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ ARGP_ERR_UNKNOWN;
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ସଂରଚନା argp run_argp = { options, parse_opt, args_doc, doc, ଖାଲି, ଖାଲି, ଖାଲି };

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
crun_command_spec (ସଂରଚନା crun_global_arguments *global_args, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_arg;
  cleanup_context libcrun_context_t *crun_context = ଖାଲି;
  cleanup_file FILE *f = ଖାଲି;
  cleanup_free ଅକ୍ଷର *bundle_cleanup = ଖାଲି;
  cleanup_free ଅକ୍ଷର *spec = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *where;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା close_ret;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &spec_options);
  crun_assert_n_args (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  /* Change dir only if -b or --bundle is defined and make sure the bundle is an absolute path.  */
  ଯଦି (bundle != ଖାଲି)
    {
      ଯଦି (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, ଖାଲି);
          ଯଦି (bundle_cleanup == ଖାଲି)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      ଯଦି (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  where = fname ? fname : "config.json";

  ଯଦି (fname == ଖାଲି)
    {
      ret = access (where, F_OK);
      ଯଦି (ret == 0)
        ଫେରନ୍ତୁ libcrun_make_error (err, 0, "`%s` already exists", where);
    }

  f = fopen (where, "w+e");
  ଯଦି (f == ଖାଲି)
    ଫେରନ୍ତୁ libcrun_make_error (err, errno, "cannot open `%s`", where);

  ret = libcrun_container_spec_json (! spec_options.rootless, &spec, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ out;

  ଯଦି (fputs (spec, f) == EOF)
    ret = libcrun_make_error (err, errno, "cannot write to `%s`", where);
  ଅନ୍ୟଥା
    ret = 0;

out:
  close_ret = fclose (f);
  f = ଖାଲି;
  ଯଦି (close_ret < 0 && ret >= 0)
    ଫେରନ୍ତୁ libcrun_make_error (err, errno, "cannot close `%s`", where);

  ଫେରନ୍ତୁ ret;
}
