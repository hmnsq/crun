/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2024 Giuseppe Scrivano <giuseppe@scrivano.org>
 * crun is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * crun is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with crun.  If not, see <http://www.gnu.org/licenses/>.
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ JSON_GEN_UTILS_H
#ವ್ಯಾಖ್ಯಾನಿಸು JSON_GEN_UTILS_H

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <ocispec/json_common.h>
#ಅನ್ತರ್ಭಾವ "utils.h"

/* Helpers to reduce the boilerplate around the json_gen_* API.  Each macro
   evaluates a json_gen_* call, stores the result in the local variable `r`
   and jumps to the `gen_error` label when the call fails.  A function using
   them must declare `int r;` and provide a `gen_error:` label.  */
#ವ್ಯಾಖ್ಯಾನಿಸು GEN_OR_FAIL(ಅಭಿವ್ಯಕ್ತಿ)                     \
  ಮಾಡು                                          \
    {                                         \
      r = (ಅಭಿವ್ಯಕ್ತಿ);                             \
      ಆದರೆ (UNLIKELY (r != json_gen_ಸ್ಥಿತಿ_ok)) \
        ಹೋಗು gen_error;                       \
  } ಆಗ (0)

/* Emit a string literal (typically a map key).  */
#ವ್ಯಾಖ್ಯಾನಿಸು GEN_ಕೀಲಿ(GEN, LIT) GEN_OR_FAIL (json_gen_string ((GEN), (LIT), ಗಾತ್ರ (LIT) - 1))

/* Emit a runtime string value.  */
#ವ್ಯಾಖ್ಯಾನಿಸು GEN_STR(GEN, STR) GEN_OR_FAIL (json_gen_string ((GEN), (STR), ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (STR)))

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cleanup_json_genp (json_gen_ctx **p)
{
  ಆದರೆ (*p)
    json_gen_free (*p);
}
#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_json_gen __attribute__ ((cleanup (cleanup_json_genp)))

#ಆದರೆ_ಅಂತ್ಯ
