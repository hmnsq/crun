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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <argp.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <errno.h>

#અન્તર્ભાવ "crun.h"

સ્થિર અક્ષર doc[] = "OCI runtime";

સંરચના spec_options_s
{
  bool rootless;
};

ગણના
{
  OPTION_ROOTLESS = 1000
};

સ્થિર અચલ અક્ષર *bundle;
સ્થિર અચલ અક્ષર *fname;

સ્થિર સંરચના spec_options_s spec_options;

સ્થિર સંરચના argp_option options[] = { { "bundle", 'b', "DIR", 0, "path to the root of the bundle dir (default \".\")", 0 },
                                        { "file", 'f', "PATH", 0, "destination file", 0 },
                                        { "rootless", OPTION_ROOTLESS, 0, 0, "spec for the rootless case", 0 },
                                        {
                                            0,
                                        } };

સ્થિર અક્ષર args_doc[] = "spec";

સ્થિર error_t
parse_opt (પૂર્ણાંક ચાવી, અક્ષર *arg, સંરચના argp_state *state)
{
  પસંદગી (ચાવી)
    {
    સ્થિતિ 'b':
      bundle = argp_mandatory_argument (arg, state);
      તોડો;

    સ્થિતિ 'f':
      fname = argp_mandatory_argument (arg, state);
      તોડો;

    સ્થિતિ OPTION_ROOTLESS:
      spec_options.rootless = true;
      તોડો;

    મૂળભૂત:
      પાછા ARGP_ERR_UNKNOWN;
    }

  પાછા 0;
}

સ્થિર સંરચના argp run_argp = { options, parse_opt, args_doc, doc, ખાલી, ખાલી, ખાલી };

પૂર્ણાંક
crun_command_spec (સંરચના crun_global_arguments *global_args, પૂર્ણાંક argc, અક્ષર **argv, libcrun_error_t *err)
{
  પૂર્ણાંક first_arg;
  cleanup_context libcrun_context_t *crun_context = ખાલી;
  cleanup_file FILE *f = ખાલી;
  cleanup_free અક્ષર *bundle_cleanup = ખાલી;
  cleanup_free અક્ષર *spec = ખાલી;
  અચલ અક્ષર *where;
  પૂર્ણાંક ret;
  પૂર્ણાંક close_ret;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &spec_options);
  crun_assert_n_args (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  /* Change dir only if -b or --bundle is defined and make sure the bundle is an absolute path.  */
  જો (bundle != ખાલી)
    {
      જો (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, ખાલી);
          જો (bundle_cleanup == ખાલી)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      જો (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  where = fname ? fname : "config.json";

  જો (fname == ખાલી)
    {
      ret = access (where, F_OK);
      જો (ret == 0)
        પાછા libcrun_make_error (err, 0, "`%s` already exists", where);
    }

  f = fopen (where, "w+e");
  જો (f == ખાલી)
    પાછા libcrun_make_error (err, errno, "cannot open `%s`", where);

  ret = libcrun_container_spec_json (! spec_options.rootless, &spec, err);
  જો (UNLIKELY (ret < 0))
    જાઓ out;

  જો (fputs (spec, f) == EOF)
    ret = libcrun_make_error (err, errno, "cannot write to `%s`", where);
  નહીંતર
    ret = 0;

out:
  close_ret = fclose (f);
  f = ખાલી;
  જો (close_ret < 0 && ret >= 0)
    પાછા libcrun_make_error (err, errno, "cannot close `%s`", where);

  પાછા ret;
}
