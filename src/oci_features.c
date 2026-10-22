/*
 * crun - OCI runtime written in C
 *
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

ಸ್ಥಿರ ರಚನೆ argp_option options[] = { { 0 } };

ಸ್ಥಿರ ಅಕ್ಷರ ವಾದಗಳು_doc[] = "features";

ಸ್ಥಿರ error_t
parse_opt (ಪೂರ್ಣಾಂಕ ಕೀಲಿ, ಅಕ್ಷರ *arg arg_ಬಳಸದ, ರಚನೆ argp_state *state arg_ಬಳಸದ)
{
  ಆದರೆ (ಕೀಲಿ != ARGP_ಕೀಲಿ_NO_ವಾದಗಳು)
    {
      ಹಿಂತಿರುಗಿ ARGP_ERR_UNKNOWN;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ರಚನೆ argp ಸತತ_argp = { options, parse_opt, ವಾದಗಳು_doc, doc, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ };

ಪೂರ್ಣಾಂಕ
crun_command_features (ರಚನೆ crun_global_arguments *global_ವಾದಗಳು, ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *json = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ಮೊದಲ_arg = 0, ret = 0;
  cleanup_context libcrun_context_t *crun_context = ಖಾಲಿ;

  argp_parse (&ಸತತ_argp, argc, argv, 0, 0, &options);

  crun_context = new_libcrun_context (global_ವಾದಗಳು);

  ret = init_libcrun_context (crun_context, argv[ಮೊದಲ_arg], global_ವಾದಗಳು, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_container_get_features_json (crun_context, &json, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಬರೆ ("%s", json);

  ಹಿಂತಿರುಗಿ 0;
}
