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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <argp.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "crun.h"

ਸਥਿਰ ਅੱਖਰ doc[] = "OCI runtime";

ਬਣਤਰ spec_options_s
{
  bool rootless;
};

ਗਿਣਤੀ
{
  OPTION_ROOTLESS = 1000
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *bundle;
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *fname;

ਸਥਿਰ ਬਣਤਰ spec_options_s spec_options;

ਸਥਿਰ ਬਣਤਰ argp_option options[] = { { "bundle", 'b', "DIR", 0, "path to the root of the bundle dir (default \".\")", 0 },
                                        { "file", 'f', "PATH", 0, "destination file", 0 },
                                        { "rootless", OPTION_ROOTLESS, 0, 0, "spec for the rootless case", 0 },
                                        {
                                            0,
                                        } };

ਸਥਿਰ ਅੱਖਰ ਦਲੀਲਾਂ_doc[] = "spec";

ਸਥਿਰ error_t
parse_opt (ਪੂਰਨ ਕੁੰਜੀ, ਅੱਖਰ *arg, ਬਣਤਰ argp_state *state)
{
  ਚੋਣ (ਕੁੰਜੀ)
    {
    ਹਾਲਤ 'b':
      bundle = argp_mandatory_ਦਲੀਲ (arg, state);
      ਰੋਕੋ;

    ਹਾਲਤ 'f':
      fname = argp_mandatory_ਦਲੀਲ (arg, state);
      ਰੋਕੋ;

    ਹਾਲਤ OPTION_ROOTLESS:
      spec_options.rootless = true;
      ਰੋਕੋ;

    ਮੂਲ:
      ਵਾਪਸ ARGP_ERR_UNKNOWN;
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਬਣਤਰ argp ਲੜੀਵਾਰ_argp = { options, parse_opt, ਦਲੀਲਾਂ_doc, doc, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ };

ਪੂਰਨ
crun_command_spec (ਬਣਤਰ crun_global_arguments *global_ਦਲੀਲਾਂ, ਪੂਰਨ argc, ਅੱਖਰ **argv, libcrun_error_t *err)
{
  ਪੂਰਨ ਪਹਿਲਾ_arg;
  cleanup_context libcrun_context_t *crun_context = ਨਿਰਾ;
  cleanup_file FILE *f = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *bundle_cleanup = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *spec = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *where;
  ਪੂਰਨ ret;
  ਪੂਰਨ close_ret;

  argp_parse (&ਲੜੀਵਾਰ_argp, argc, argv, ARGP_ਵਿੱਚ_ਕ੍ਰਮ, &ਪਹਿਲਾ_arg, &spec_options);
  crun_assert_n_ਦਲੀਲਾਂ (argc - ਪਹਿਲਾ_arg, 0, 0);

  crun_context = new_libcrun_context (global_ਦਲੀਲਾਂ);

  ret = init_libcrun_context (crun_context, argv[ਪਹਿਲਾ_arg], global_ਦਲੀਲਾਂ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  /* Change dir only if -b or --bundle is defined and make sure the bundle is an absolute path.  */
  ਜੇ (bundle != ਨਿਰਾ)
    {
      ਜੇ (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, ਨਿਰਾ);
          ਜੇ (bundle_cleanup == ਨਿਰਾ)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      ਜੇ (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  where = fname ? fname : "config.json";

  ਜੇ (fname == ਨਿਰਾ)
    {
      ret = access (where, F_OK);
      ਜੇ (ret == 0)
        ਵਾਪਸ libcrun_make_error (err, 0, "`%s` already exists", where);
    }

  f = fopen (where, "w+e");
  ਜੇ (f == ਨਿਰਾ)
    ਵਾਪਸ libcrun_make_error (err, errno, "cannot open `%s`", where);

  ret = libcrun_container_spec_json (! spec_options.rootless, &spec, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ ਬਾਹਰਲਾ;

  ਜੇ (fputs (spec, f) == EOF)
    ret = libcrun_make_error (err, errno, "cannot write to `%s`", where);
  ਨਹੀਂਤਾਂ
    ret = 0;

ਬਾਹਰਲਾ:
  close_ret = fclose (f);
  f = ਨਿਰਾ;
  ਜੇ (close_ret < 0 && ret >= 0)
    ਵਾਪਸ libcrun_make_error (err, errno, "cannot close `%s`", where);

  ਵਾਪਸ ret;
}
