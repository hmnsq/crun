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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <argp.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <errno.h>

#அந்தர்பாவ "crun.h"

நிலையான எழுத்து doc[] = "OCI runtime";

எண்ணிக்கை
{
  OPTION_CONSOLE_SOCKET = 1000,
  OPTION_PID_FILE,
  OPTION_NO_SUBREAPER,
  OPTION_NO_NEW_KEYRING,
  OPTION_PRESERVE_FDS
};

கட்டமைப்பு ps_options_s
{
  முழு format;
};

எண்ணிக்கை
{
  PS_TABLE = 100,
  PS_JSON,
};

நிலையான கட்டமைப்பு ps_options_s ps_options;

நிலையான கட்டமைப்பு argp_option options[] = { { "format", 'f', "FORMAT", 0, "select the output format", 0 },
                                        {
                                            0,
                                        } };

நிலையான எழுத்து args_doc[] = "ps";

நிலையான error_t
parse_opt (முழு சாவி, எழுத்து *arg, கட்டமைப்பு argp_state *state arg_unused)
{
  தேர்வு (சாவி)
    {
    நிலை 'f':
      எனில் (strcmp (arg, "table") == 0)
        ps_options.format = PS_TABLE;
      இல்லையெனில் எனில் (strcmp (arg, "json") == 0)
        ps_options.format = PS_JSON;
      இல்லையெனில்
        libcrun_fail_with_error (0, "invalid format `%s`", arg);
      நிறுத்து;

    இயல்பு:
      திரும்பு ARGP_ERR_UNKNOWN;
    }

  திரும்பு 0;
}

நிலையான கட்டமைப்பு argp run_argp = { options, parse_opt, args_doc, doc, வெற்று, வெற்று, வெற்று };

முழு
crun_command_ps (கட்டமைப்பு crun_global_arguments *global_args, முழு argc, எழுத்து **argv, libcrun_error_t *err)
{
  முழு first_arg;
  முழு ret;
  cleanup_free pid_t *pids = வெற்று;
  cleanup_context libcrun_context_t *crun_context = வெற்று;
  size_t i;

  ps_options.format = PS_TABLE;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &ps_options);
  crun_assert_n_args (argc - first_arg, 1, 1);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcrun_container_read_pids (crun_context, argv[first_arg], true, &pids, err);
  எனில் (UNLIKELY (ret < 0))
    {
      libcrun_error_report_and_release (err);
      திரும்பு ret;
    }

  தேர்வு (ps_options.format)
    {
    நிலை PS_JSON:
      printf ("[\n");
      க்கு (i = 0; pids && pids[i]; i++)
        printf ("  %d%s\n", pids[i], pids[i + 1] ? "," : "");
      printf ("]\n");
      நிறுத்து;

    நிலை PS_TABLE:
      printf ("PID\n");
      க்கு (i = 0; pids && pids[i]; i++)
        printf ("%d\n", pids[i]);
      நிறுத்து;
    }

  திரும்பு 0;
}
