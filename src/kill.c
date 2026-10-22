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
#అన్తర్భావ <signal.h>
#అన్తర్భావ <regex.h>

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

నిర్మాణం kill_options_s
{
  bool all;
  bool regex;
};

స్థిర నిర్మాణం kill_options_s kill_options;

స్థిర నిర్మాణం argp_option options[]
    = { { "all", 'a', 0, 0, "kill all the processes", 0 },
        { "regex", 'r', 0, 0, "the specified CONTAINER is a regular expression (kill multiple containers)", 0 },
        {
            0,
        } };

స్థిర అక్షరం వాదనలు_doc[] = "kill CONTAINER [SIGNAL]";

స్థిర error_t
parse_opt (పూర్ణాంకం కీలకం, అక్షరం *arg arg_వాడని, నిర్మాణం argp_state *state arg_వాడని)
{
  ఎంపిక (కీలకం)
    {
    స్థితి 'a':
      kill_options.all = true;
      ఆపు;

    స్థితి 'r':
      kill_options.regex = true;
      ఆపు;

    స్థితి ARGP_కీలకం_NO_వాదనలు:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    అప్రమేయ:
      తిరిగి ARGP_ERR_UNKNOWN;
    }

  తిరిగి 0;
}

స్థిర నిర్మాణం argp నిరంతరం_argp = { options, parse_opt, వాదనలు_doc, doc, ఖాళీ, ఖాళీ, ఖాళీ };

పూర్ణాంకం
crun_command_kill (నిర్మాణం crun_global_arguments *global_వాదనలు, పూర్ణాంకం argc, అక్షరం **argv, libcrun_error_t *err)
{
  పూర్ణాంకం మొదటి_arg = 0, ret;
  స్థిరరాశి అక్షరం *సంకేతం;
  cleanup_context libcrun_context_t *crun_context = ఖాళీ;

  argp_parse (&నిరంతరం_argp, argc, argv, ARGP_లోపల_క్రమం, &మొదటి_arg, &kill_options);
  crun_assert_n_వాదనలు (argc - మొదటి_arg, 1, 2);

  crun_context = new_libcrun_context (global_వాదనలు);

  ret = init_libcrun_context (crun_context, argv[మొదటి_arg], global_వాదనలు, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  సంకేతం = "SIGTERM";
  ఒకవేళ (argc - మొదటి_arg > 1)
    సంకేతం = argv[మొదటి_arg + 1];

  ఒకవేళ (kill_options.regex)
    {
      regex_t re;
      libcrun_container_list_t *list;
      libcrun_container_iter_t *it;
      స్థిరరాశి అక్షరం *name;

      ret = regcomp (&re, argv[మొదటి_arg], REG_EXTENDED | REG_NOSUB);
      ఒకవేళ (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "invalid regular expression %s", argv[మొదటి_arg]);

      ret = libcrun_container_list (crun_context, &list, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "cannot read containers list");

      కొరకు (it = libcrun_container_list_iter (list); libcrun_container_iter_తదుపరి (it, &name);)
        ఒకవేళ (regexec (&re, name, 0, ఖాళీ, 0) == 0)
          {
            ret = libcrun_container_kill (crun_context, name, సంకేతం, err);
            ఒకవేళ (UNLIKELY (ret < 0))
              libcrun_error_report_and_release (err);
          }

      libcrun_container_iter_free (it);
      libcrun_container_list_free (list);
      regfree (&re);
      తిరిగి 0;
    }

  ఒకవేళ (kill_options.all)
    తిరిగి libcrun_container_killall (crun_context, argv[మొదటి_arg], సంకేతం, err);

  తిరిగి libcrun_container_kill (crun_context, argv[మొదటి_arg], సంకేతం, err);
}
