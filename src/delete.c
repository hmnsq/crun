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
#ଅନ୍ତର୍ଭାଵ <regex.h>

#ଅନ୍ତର୍ଭାଵ "crun.h"

ସ୍ଥିର ଅକ୍ଷର doc[] = "OCI runtime";

ଗଣନା
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

ସଂରଚନା delete_options_s
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା regex;
  bool force;
};

ସ୍ଥିର ସଂରଚନା delete_options_s delete_options;

ସ୍ଥିର ସଂରଚନା argp_option options[]
    = { { "force", 'f', 0, 0, "delete the container even if it is still running", 0 },
        { "regex", 'r', 0, 0, "the specified CONTAINER is a regular expression (delete multiple containers)", 0 },
        {
            0,
        } };

ସ୍ଥିର ଅକ୍ଷର args_doc[] = "delete CONTAINER";

ସ୍ଥିର error_t
parse_opt (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଚାବି, ଅକ୍ଷର *arg arg_unused, ସଂରଚନା argp_state *state arg_unused)
{
  ବାଛ (ଚାବି)
    {
    ସ୍ଥିତି 'f':
      delete_options.force = true;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 'r':
      delete_options.regex = true;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି ARGP_ଚାବି_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ ARGP_ERR_UNKNOWN;
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ସଂରଚନା argp run_argp = { options, parse_opt, args_doc, doc, ଖାଲି, ଖାଲି, ଖାଲି };

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
crun_command_delete (ସଂରଚନା crun_global_arguments *global_args, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = ଖାଲି;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &delete_options);
  crun_assert_n_args (argc - first_arg, 1, 1);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (delete_options.regex)
    {
      regex_t re;
      libcrun_container_list_t *list;
      libcrun_container_iter_t *it;
      ଅଚଳ ଅକ୍ଷର *name;

      ret = regcomp (&re, argv[first_arg], REG_EXTENDED | REG_NOSUB);
      ଯଦି (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "invalid regular expression %s", argv[first_arg]);

      ret = libcrun_container_list (crun_context, &list, err);
      ଯଦି (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "cannot read containers list");

      ପାଇଁ (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
        ଯଦି (regexec (&re, name, 0, ଖାଲି, 0) == 0)
          {
            ret = libcrun_container_delete (crun_context, name, delete_options.force, err);
            ଯଦି (UNLIKELY (ret < 0))
              libcrun_error_report_and_release (err);
          }

      libcrun_container_iter_free (it);
      libcrun_container_list_free (list);
      regfree (&re);
      ଫେରନ୍ତୁ 0;
    }

  ଫେରନ୍ତୁ libcrun_container_delete (crun_context, argv[first_arg], delete_options.force, err);
}
