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

ଗଣନା
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

ସଂରଚନା ps_options_s
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା format;
};

ଗଣନା
{
  PS_TABLE = 100,
  PS_JSON,
};

ସ୍ଥିର ସଂରଚନା ps_options_s ps_options;

ସ୍ଥିର ସଂରଚନା argp_option options[] = { { "format", 'f', "FORMAT", 0, "select the output format", 0 },
                                        {
                                            0,
                                        } };

ସ୍ଥିର ଅକ୍ଷର args_doc[] = "ps";

ସ୍ଥିର error_t
parse_opt (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଚାବି, ଅକ୍ଷର *arg, ସଂରଚନା argp_state *state arg_unused)
{
  ବାଛ (ଚାବି)
    {
    ସ୍ଥିତି 'f':
      ଯଦି (strcmp (arg, "table") == 0)
        ps_options.format = PS_TABLE;
      ଅନ୍ୟଥା ଯଦି (strcmp (arg, "json") == 0)
        ps_options.format = PS_JSON;
      ଅନ୍ୟଥା
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ ARGP_ERR_UNKNOWN;
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ସଂରଚନା argp run_argp = { options, parse_opt, args_doc, doc, ଖାଲି, ଖାଲି, ଖାଲି };

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
crun_command_ps (ସଂରଚନା crun_global_arguments *global_args, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_arg;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  cleanup_free pid_t *pids = ଖାଲି;
  cleanup_context libcrun_context_t *crun_context = ଖାଲି;
  size_t i;

  ps_options.format = PS_TABLE;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &ps_options);
  crun_assert_n_args (argc - first_arg, 1, 1);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_container_read_pids (crun_context, argv[first_arg], true, &pids, err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      libcrun_error_report_and_release (err);
      ଫେରନ୍ତୁ ret;
    }

  ବାଛ (ps_options.format)
    {
    ସ୍ଥିତି PS_JSON:
      printf ("[\n");
      ପାଇଁ (i = 0; pids && pids[i]; i++)
        printf ("  %d%s\n", pids[i], pids[i + 1] ? "," : "");
      printf ("]\n");
      ଭାଙ୍ଗ;

    ସ୍ଥିତି PS_TABLE:
      printf ("PID\n");
      ପାଇଁ (i = 0; pids && pids[i]; i++)
        printf ("%d\n", pids[i]);
      ଭାଙ୍ଗ;
    }

  ଫେରନ୍ତୁ 0;
}
