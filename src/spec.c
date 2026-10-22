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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <argp.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <errno.h>

#അന്തര്ഭാവ "crun.h"

സ്ഥിതം അക്ഷരം doc[] = "OCI runtime";

ഘടന spec_options_s
{
  bool rootless;
};

എണ്ണൽ
{
  OPTION_ROOTLESS = 1000
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *bundle;
സ്ഥിതം സ്ഥിരം അക്ഷരം *fname;

സ്ഥിതം ഘടന spec_options_s spec_options;

സ്ഥിതം ഘടന argp_option options[] = { { "bundle", 'b', "DIR", 0, "path to the root of the bundle dir (default \".\")", 0 },
                                        { "file", 'f', "PATH", 0, "destination file", 0 },
                                        { "rootless", OPTION_ROOTLESS, 0, 0, "spec for the rootless case", 0 },
                                        {
                                            0,
                                        } };

സ്ഥിതം അക്ഷരം args_doc[] = "spec";

സ്ഥിതം error_t
parse_opt (പൂർണ്ണസംഖ്യ താക്കോൽ, അക്ഷരം *arg, ഘടന argp_state *state)
{
  തിരഞ്ഞെടുപ്പ് (താക്കോൽ)
    {
    അവസ്ഥ 'b':
      bundle = argp_mandatory_argument (arg, state);
      നിർത്തുക;

    അവസ്ഥ 'f':
      fname = argp_mandatory_argument (arg, state);
      നിർത്തുക;

    അവസ്ഥ OPTION_ROOTLESS:
      spec_options.rootless = true;
      നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
      തിരികെ ARGP_ERR_UNKNOWN;
    }

  തിരികെ 0;
}

സ്ഥിതം ഘടന argp run_argp = { options, parse_opt, args_doc, doc, ശൂന്യ, ശൂന്യ, ശൂന്യ };

പൂർണ്ണസംഖ്യ
crun_command_spec (ഘടന crun_global_arguments *global_args, പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ first_arg;
  cleanup_context libcrun_context_t *crun_context = ശൂന്യ;
  cleanup_file FILE *f = ശൂന്യ;
  cleanup_free അക്ഷരം *bundle_cleanup = ശൂന്യ;
  cleanup_free അക്ഷരം *spec = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *where;
  പൂർണ്ണസംഖ്യ ret;
  പൂർണ്ണസംഖ്യ close_ret;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &spec_options);
  crun_assert_n_args (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  /* Change dir only if -b or --bundle is defined and make sure the bundle is an absolute path.  */
  എങ്കിൽ (bundle != ശൂന്യ)
    {
      എങ്കിൽ (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, ശൂന്യ);
          എങ്കിൽ (bundle_cleanup == ശൂന്യ)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      എങ്കിൽ (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  where = fname ? fname : "config.json";

  എങ്കിൽ (fname == ശൂന്യ)
    {
      ret = access (where, F_OK);
      എങ്കിൽ (ret == 0)
        തിരികെ libcrun_make_error (err, 0, "`%s` already exists", where);
    }

  f = fopen (where, "w+e");
  എങ്കിൽ (f == ശൂന്യ)
    തിരികെ libcrun_make_error (err, errno, "cannot open `%s`", where);

  ret = libcrun_container_spec_json (! spec_options.rootless, &spec, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക out;

  എങ്കിൽ (fputs (spec, f) == EOF)
    ret = libcrun_make_error (err, errno, "cannot write to `%s`", where);
  അല്ലെങ്കിൽ
    ret = 0;

out:
  close_ret = fclose (f);
  f = ശൂന്യ;
  എങ്കിൽ (close_ret < 0 && ret >= 0)
    തിരികെ libcrun_make_error (err, errno, "cannot close `%s`", where);

  തിരികെ ret;
}
