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

ସଂରଚନା pause_options_s
{
};

ସ୍ଥିର ସଂରଚନା pause_options_s pause_options;

ସ୍ଥିର ସଂରଚନା argp_option options[] = { {
    0,
} };

ସ୍ଥିର ଅକ୍ଷର args_doc[] = "pause CONTAINER";

ସ୍ଥିର error_t
parse_opt (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଚାବି, ଅକ୍ଷର *arg arg_unused, ସଂରଚନା argp_state *state arg_unused)
{
  ବାଛ (ଚାବି)
    {
    ସ୍ଥିତି ARGP_ଚାବି_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ ARGP_ERR_UNKNOWN;
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ସଂରଚନା argp run_argp = { options, parse_opt, args_doc, doc, ଖାଲି, ଖାଲି, ଖାଲି };

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
crun_command_pause (ସଂରଚନା crun_global_arguments *global_args, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = ଖାଲି;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &pause_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ libcrun_container_pause (crun_context, argv[first_arg], err);
}
