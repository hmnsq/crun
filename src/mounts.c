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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <argp.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <errno.h>

#ಅನ್ತರ್ಭಾವ "crun.h"

ಸ್ಥಿರ ಅಕ್ಷರ doc[] = "OCI runtime";

ಸ್ಥಿರ ರಚನೆ argp_option options[] = {
  0,
};

ಸ್ಥಿರ ಅಕ್ಷರ ವಾದಗಳು_doc[] = "mounts [add|remove] CONTAINER FILE";

ಸ್ಥಿರ error_t
parse_opt (ಪೂರ್ಣಾಂಕ ಕೀಲಿ, ಅಕ್ಷರ *arg arg_ಬಳಸದ, ರಚನೆ argp_state *state arg_ಬಳಸದ)
{
  ಆಯ್ಕೆ (ಕೀಲಿ)
    {
    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ ARGP_ERR_UNKNOWN;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ರಚನೆ argp ಸತತ_argp = { options, parse_opt, ವಾದಗಳು_doc, doc, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ };

ಪೂರ್ಣಾಂಕ
crun_command_mounts (ರಚನೆ crun_global_arguments *global_ವಾದಗಳು, ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ಮೊದಲ_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = ಖಾಲಿ;

  argp_parse (&ಸತತ_argp, argc, argv, ARGP_ಒಳ_ಕ್ರಮ, &ಮೊದಲ_arg, ಖಾಲಿ);
  crun_assert_n_ವಾದಗಳು (argc - ಮೊದಲ_arg, 3, 3);

  crun_context = new_libcrun_context (global_ವಾದಗಳು);

  ret = init_libcrun_context (crun_context, argv[ಮೊದಲ_arg], global_ವಾದಗಳು, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (strcmp (argv[ಮೊದಲ_arg], "add") == 0)
    ಹಿಂತಿರುಗಿ libcrun_container_ಕೂಡಿಸು_mounts_from_file (crun_context, argv[ಮೊದಲ_arg + 1], argv[ಮೊದಲ_arg + 2], err);
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (argv[ಮೊದಲ_arg], "remove") == 0)
    ಹಿಂತಿರುಗಿ libcrun_container_remove_mounts_from_file (crun_context, argv[ಮೊದಲ_arg + 1], argv[ಮೊದಲ_arg + 2], err);

  ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "unknown command %s", argv[ಮೊದಲ_arg]);
}
