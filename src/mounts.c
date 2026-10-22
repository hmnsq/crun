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

স্থির কাঠামো argp_option options[] = {
  0,
};

স্থির অক্ষর args_doc[] = "mounts [add|remove] CONTAINER FILE";

স্থির error_t
parse_opt (পূর্ণসংখ্যা চাবি, অক্ষর *arg arg_unused, কাঠামো argp_state *state arg_unused)
{
  নির্বাচন (চাবি)
    {
    পূর্বনির্ধারিত:
      ফেরত ARGP_ERR_UNKNOWN;
    }

  ফেরত 0;
}

স্থির কাঠামো argp run_argp = { options, parse_opt, args_doc, doc, ফাঁকা, ফাঁকা, ফাঁকা };

পূর্ণসংখ্যা
crun_command_mounts (কাঠামো crun_global_arguments *global_args, পূর্ণসংখ্যা argc, অক্ষর **argv, libcrun_error_t *err)
{
  পূর্ণসংখ্যা first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = ফাঁকা;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, ফাঁকা);
  crun_assert_n_args (argc - first_arg, 3, 3);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  যদি (strcmp (argv[first_arg], "add") == 0)
    ফেরত libcrun_container_add_mounts_from_file (crun_context, argv[first_arg + 1], argv[first_arg + 2], err);
  অন্যথা যদি (strcmp (argv[first_arg], "remove") == 0)
    ফেরত libcrun_container_remove_mounts_from_file (crun_context, argv[first_arg + 1], argv[first_arg + 2], err);

  ফেরত crun_make_error (err, 0, "unknown command %s", argv[first_arg]);
}
