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
#অন্তর্ভাব <regex.h>

#অন্তর্ভাব "crun.h"

স্থির অক্ষর doc[] = "OCI runtime";

কাঠামো pause_options_s
{
};

স্থির কাঠামো pause_options_s pause_options;

স্থির কাঠামো argp_option options[] = { {
    0,
} };

স্থির অক্ষর args_doc[] = "pause CONTAINER";

স্থির error_t
parse_opt (পূর্ণসংখ্যা চাবি, অক্ষর *arg arg_unused, কাঠামো argp_state *state arg_unused)
{
  নির্বাচন (চাবি)
    {
    ক্ষেত্র ARGP_চাবি_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    পূর্বনির্ধারিত:
      ফেরত ARGP_ERR_UNKNOWN;
    }

  ফেরত 0;
}

স্থির কাঠামো argp run_argp = { options, parse_opt, args_doc, doc, ফাঁকা, ফাঁকা, ফাঁকা };

পূর্ণসংখ্যা
crun_command_pause (কাঠামো crun_global_arguments *global_args, পূর্ণসংখ্যা argc, অক্ষর **argv, libcrun_error_t *err)
{
  পূর্ণসংখ্যা first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = ফাঁকা;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &pause_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ফেরত libcrun_container_pause (crun_context, argv[first_arg], err);
}
