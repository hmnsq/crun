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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <argp.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <errno.h>

#అన్తర్భావ "crun.h"

స్థిర అక్షరం doc[] = "OCI runtime";

నిర్మాణం spec_options_s
{
  bool rootless;
};

గణన
{
  OPTION_ROOTLESS = 1000
};

స్థిర స్థిరరాశి అక్షరం *bundle;
స్థిర స్థిరరాశి అక్షరం *fname;

స్థిర నిర్మాణం spec_options_s spec_options;

స్థిర నిర్మాణం argp_option options[] = { { "bundle", 'b', "DIR", 0, "path to the root of the bundle dir (default \".\")", 0 },
                                        { "file", 'f', "PATH", 0, "destination file", 0 },
                                        { "rootless", OPTION_ROOTLESS, 0, 0, "spec for the rootless case", 0 },
                                        {
                                            0,
                                        } };

స్థిర అక్షరం వాదనలు_doc[] = "spec";

స్థిర error_t
parse_opt (పూర్ణాంకం కీలకం, అక్షరం *arg, నిర్మాణం argp_state *state)
{
  ఎంపిక (కీలకం)
    {
    స్థితి 'b':
      bundle = argp_mandatory_వాదం (arg, state);
      ఆపు;

    స్థితి 'f':
      fname = argp_mandatory_వాదం (arg, state);
      ఆపు;

    స్థితి OPTION_ROOTLESS:
      spec_options.rootless = true;
      ఆపు;

    అప్రమేయ:
      తిరిగి ARGP_ERR_UNKNOWN;
    }

  తిరిగి 0;
}

స్థిర నిర్మాణం argp నిరంతరం_argp = { options, parse_opt, వాదనలు_doc, doc, ఖాళీ, ఖాళీ, ఖాళీ };

పూర్ణాంకం
crun_command_spec (నిర్మాణం crun_global_arguments *global_వాదనలు, పూర్ణాంకం argc, అక్షరం **argv, libcrun_error_t *err)
{
  పూర్ణాంకం మొదటి_arg;
  cleanup_context libcrun_context_t *crun_context = ఖాళీ;
  cleanup_file FILE *f = ఖాళీ;
  cleanup_free అక్షరం *bundle_cleanup = ఖాళీ;
  cleanup_free అక్షరం *spec = ఖాళీ;
  స్థిరరాశి అక్షరం *where;
  పూర్ణాంకం ret;
  పూర్ణాంకం close_ret;

  argp_parse (&నిరంతరం_argp, argc, argv, ARGP_లోపల_క్రమం, &మొదటి_arg, &spec_options);
  crun_assert_n_వాదనలు (argc - మొదటి_arg, 0, 0);

  crun_context = new_libcrun_context (global_వాదనలు);

  ret = init_libcrun_context (crun_context, argv[మొదటి_arg], global_వాదనలు, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  /* Change dir only if -b or --bundle is defined and make sure the bundle is an absolute path.  */
  ఒకవేళ (bundle != ఖాళీ)
    {
      ఒకవేళ (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, ఖాళీ);
          ఒకవేళ (bundle_cleanup == ఖాళీ)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      ఒకవేళ (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  where = fname ? fname : "config.json";

  ఒకవేళ (fname == ఖాళీ)
    {
      ret = access (where, F_OK);
      ఒకవేళ (ret == 0)
        తిరిగి libcrun_make_error (err, 0, "`%s` already exists", where);
    }

  f = fopen (where, "w+e");
  ఒకవేళ (f == ఖాళీ)
    తిరిగి libcrun_make_error (err, errno, "cannot open `%s`", where);

  ret = libcrun_container_spec_json (! spec_options.rootless, &spec, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు బహిర్గతం;

  ఒకవేళ (fputs (spec, f) == EOF)
    ret = libcrun_make_error (err, errno, "cannot write to `%s`", where);
  లేకపోతే
    ret = 0;

బహిర్గతం:
  close_ret = fclose (f);
  f = ఖాళీ;
  ఒకవేళ (close_ret < 0 && ret >= 0)
    తిరిగి libcrun_make_error (err, errno, "cannot close `%s`", where);

  తిరిగి ret;
}
