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
#અન્તર્ભાવ <signal.h>
#અન્તર્ભાવ <regex.h>

#અન્તર્ભાવ "crun.h"

સ્થિર અક્ષર doc[] = "OCI runtime";

ગણના
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

સંરચના kill_options_s
{
  bool all;
  bool regex;
};

સ્થિર સંરચના kill_options_s kill_options;

સ્થિર સંરચના argp_option options[]
    = { { "all", 'a', 0, 0, "kill all the processes", 0 },
        { "regex", 'r', 0, 0, "the specified CONTAINER is a regular expression (kill multiple containers)", 0 },
        {
            0,
        } };

સ્થિર અક્ષર args_doc[] = "kill CONTAINER [SIGNAL]";

સ્થિર error_t
parse_opt (પૂર્ણાંક ચાવી, અક્ષર *arg arg_unused, સંરચના argp_state *state arg_unused)
{
  પસંદગી (ચાવી)
    {
    સ્થિતિ 'a':
      kill_options.all = true;
      તોડો;

    સ્થિતિ 'r':
      kill_options.regex = true;
      તોડો;

    સ્થિતિ ARGP_ચાવી_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    મૂળભૂત:
      પાછા ARGP_ERR_UNKNOWN;
    }

  પાછા 0;
}

સ્થિર સંરચના argp run_argp = { options, parse_opt, args_doc, doc, ખાલી, ખાલી, ખાલી };

પૂર્ણાંક
crun_command_kill (સંરચના crun_global_arguments *global_args, પૂર્ણાંક argc, અક્ષર **argv, libcrun_error_t *err)
{
  પૂર્ણાંક first_arg = 0, ret;
  અચલ અક્ષર *સંકેત;
  cleanup_context libcrun_context_t *crun_context = ખાલી;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &kill_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  સંકેત = "SIGTERM";
  જો (argc - first_arg > 1)
    સંકેત = argv[first_arg + 1];

  જો (kill_options.regex)
    {
      regex_t re;
      libcrun_container_list_t *list;
      libcrun_container_iter_t *it;
      અચલ અક્ષર *name;

      ret = regcomp (&re, argv[first_arg], REG_EXTENDED | REG_NOSUB);
      જો (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "invalid regular expression %s", argv[first_arg]);

      ret = libcrun_container_list (crun_context, &list, err);
      જો (UNLIKELY (ret < 0))
        libcrun_fail_with_error (0, "cannot read containers list");

      માટે (it = libcrun_container_list_iter (list); libcrun_container_iter_next (it, &name);)
        જો (regexec (&re, name, 0, ખાલી, 0) == 0)
          {
            ret = libcrun_container_kill (crun_context, name, સંકેત, err);
            જો (UNLIKELY (ret < 0))
              libcrun_error_report_and_release (err);
          }

      libcrun_container_iter_free (it);
      libcrun_container_list_free (list);
      regfree (&re);
      પાછા 0;
    }

  જો (kill_options.all)
    પાછા libcrun_container_killall (crun_context, argv[first_arg], સંકેત, err);

  પાછા libcrun_container_kill (crun_context, argv[first_arg], સંકેત, err);
}
