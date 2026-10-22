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

நிலையான கட்டமைப்பு argp_option options[] = {
  0,
};

நிலையான எழுத்து args_doc[] = "mounts [add|remove] CONTAINER FILE";

நிலையான error_t
parse_opt (முழு சாவி, எழுத்து *arg arg_unused, கட்டமைப்பு argp_state *state arg_unused)
{
  தேர்வு (சாவி)
    {
    இயல்பு:
      திரும்பு ARGP_ERR_UNKNOWN;
    }

  திரும்பு 0;
}

நிலையான கட்டமைப்பு argp run_argp = { options, parse_opt, args_doc, doc, வெற்று, வெற்று, வெற்று };

முழு
crun_command_mounts (கட்டமைப்பு crun_global_arguments *global_args, முழு argc, எழுத்து **argv, libcrun_error_t *err)
{
  முழு first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = வெற்று;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, வெற்று);
  crun_assert_n_args (argc - first_arg, 3, 3);

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (strcmp (argv[first_arg], "add") == 0)
    திரும்பு libcrun_container_add_mounts_from_file (crun_context, argv[first_arg + 1], argv[first_arg + 2], err);
  இல்லையெனில் எனில் (strcmp (argv[first_arg], "remove") == 0)
    திரும்பு libcrun_container_remove_mounts_from_file (crun_context, argv[first_arg + 1], argv[first_arg + 2], err);

  திரும்பு crun_make_error (err, 0, "unknown command %s", argv[first_arg]);
}
