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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <argp.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <errno.h>

#অন্তর্ভাব "crun.h"

স্থির অক্ষর doc[] = "OCI runtime";

কাঠামো spec_options_s
{
  bool rootless;
};

গণনা
{
  OPTION_ROOTLESS = 1000
};

স্থির ধ্রুবক অক্ষর *bundle;
স্থির ধ্রুবক অক্ষর *fname;

স্থির কাঠামো spec_options_s spec_options;

স্থির কাঠামো argp_option options[] = { { "bundle", 'b', "DIR", 0, "path to the root of the bundle dir (default \".\")", 0 },
                                        { "file", 'f', "PATH", 0, "destination file", 0 },
                                        { "rootless", OPTION_ROOTLESS, 0, 0, "spec for the rootless case", 0 },
                                        {
                                            0,
                                        } };

স্থির অক্ষর args_doc[] = "spec";

স্থির error_t
parse_opt (পূর্ণসংখ্যা চাবি, অক্ষর *arg, কাঠামো argp_state *state)
{
  নির্বাচন (চাবি)
    {
    ক্ষেত্র 'b':
      bundle = argp_mandatory_argument (arg, state);
      বিরতি;

    ক্ষেত্র 'f':
      fname = argp_mandatory_argument (arg, state);
      বিরতি;

    ক্ষেত্র OPTION_ROOTLESS:
      spec_options.rootless = true;
      বিরতি;

    পূর্বনির্ধারিত:
      ফেরত ARGP_ERR_UNKNOWN;
    }

  ফেরত 0;
}

স্থির কাঠামো argp run_argp = { options, parse_opt, args_doc, doc, ফাঁকা, ফাঁকা, ফাঁকা };

পূর্ণসংখ্যা
crun_command_spec (কাঠামো crun_global_arguments *global_args, পূর্ণসংখ্যা argc, অক্ষর **argv, libcrun_error_t *err)
{
  পূর্ণসংখ্যা first_arg;
  cleanup_context libcrun_context_t *crun_context = ফাঁকা;
  cleanup_file FILE *f = ফাঁকা;
  cleanup_free অক্ষর *bundle_cleanup = ফাঁকা;
  cleanup_free অক্ষর *spec = ফাঁকা;
  ধ্রুবক অক্ষর *where;
  পূর্ণসংখ্যা ret;
  পূর্ণসংখ্যা close_ret;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &spec_options);
  crun_assert_n_args (argc - first_arg, 0, 0);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  /* Change dir only if -b or --bundle is defined and make sure the bundle is an absolute path.  */
  যদি (bundle != ফাঁকা)
    {
      যদি (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, ফাঁকা);
          যদি (bundle_cleanup == ফাঁকা)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      যদি (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  where = fname ? fname : "config.json";

  যদি (fname == ফাঁকা)
    {
      ret = access (where, F_OK);
      যদি (ret == 0)
        ফেরত libcrun_make_error (err, 0, "`%s` already exists", where);
    }

  f = fopen (where, "w+e");
  যদি (f == ফাঁকা)
    ফেরত libcrun_make_error (err, errno, "cannot open `%s`", where);

  ret = libcrun_container_spec_json (! spec_options.rootless, &spec, err);
  যদি (UNLIKELY (ret < 0))
    যাও out;

  যদি (fputs (spec, f) == EOF)
    ret = libcrun_make_error (err, errno, "cannot write to `%s`", where);
  অন্যথা
    ret = 0;

out:
  close_ret = fclose (f);
  f = ফাঁকা;
  যদি (close_ret < 0 && ret >= 0)
    ফেরত libcrun_make_error (err, errno, "cannot close `%s`", where);

  ফেরত ret;
}
