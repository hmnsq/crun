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

గణన
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

నిర్మాణం ps_options_s
{
  పూర్ణాంకం format;
};

గణన
{
  PS_పట్టిక = 100,
  PS_JSON,
};

స్థిర నిర్మాణం ps_options_s ps_options;

స్థిర నిర్మాణం argp_option options[] = { { "format", 'f', "FORMAT", 0, "select the output format", 0 },
                                        {
                                            0,
                                        } };

స్థిర అక్షరం వాదనలు_doc[] = "ps";

స్థిర error_t
parse_opt (పూర్ణాంకం కీలకం, అక్షరం *arg, నిర్మాణం argp_state *state arg_వాడని)
{
  ఎంపిక (కీలకం)
    {
    స్థితి 'f':
      ఒకవేళ (strcmp (arg, "table") == 0)
        ps_options.format = PS_పట్టిక;
      లేకపోతే ఒకవేళ (strcmp (arg, "json") == 0)
        ps_options.format = PS_JSON;
      లేకపోతే
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      ఆపు;

    అప్రమేయ:
      తిరిగి ARGP_ERR_UNKNOWN;
    }

  తిరిగి 0;
}

స్థిర నిర్మాణం argp నిరంతరం_argp = { options, parse_opt, వాదనలు_doc, doc, ఖాళీ, ఖాళీ, ఖాళీ };

పూర్ణాంకం
crun_command_ps (నిర్మాణం crun_global_arguments *global_వాదనలు, పూర్ణాంకం argc, అక్షరం **argv, libcrun_error_t *err)
{
  పూర్ణాంకం మొదటి_arg;
  పూర్ణాంకం ret;
  cleanup_free pid_t *pids = ఖాళీ;
  cleanup_context libcrun_context_t *crun_context = ఖాళీ;
  size_t i;

  ps_options.format = PS_పట్టిక;

  argp_parse (&నిరంతరం_argp, argc, argv, ARGP_లోపల_క్రమం, &మొదటి_arg, &ps_options);
  crun_assert_n_వాదనలు (argc - మొదటి_arg, 1, 1);

  crun_context = new_libcrun_context (global_వాదనలు);

  ret = init_libcrun_context (crun_context, argv[మొదటి_arg], global_వాదనలు, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcrun_container_read_pids (crun_context, argv[మొదటి_arg], true, &pids, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      libcrun_error_report_and_release (err);
      తిరిగి ret;
    }

  ఎంపిక (ps_options.format)
    {
    స్థితి PS_JSON:
      రాయి ("[\n");
      కొరకు (i = 0; pids && pids[i]; i++)
        రాయి ("  %d%s\n", pids[i], pids[i + 1] ? "," : "");
      రాయి ("]\n");
      ఆపు;

    స్థితి PS_పట్టిక:
      రాయి ("PID\n");
      కొరకు (i = 0; pids && pids[i]; i++)
        రాయి ("%d\n", pids[i]);
      ఆపు;
    }

  తిరిగి 0;
}
