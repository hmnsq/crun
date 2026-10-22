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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ JSON_GEN_UTILS_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ JSON_GEN_UTILS_H

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ocispec/json_common.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

/* Helpers to reduce the boilerplate around the json_gen_* API.  Each macro
   evaluates a json_gen_* call, stores the result in the local variable `r`
   and jumps to the `gen_error` label when the call fails.  A function using
   them must declare `int r;` and provide a `gen_error:` label.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GEN_OR_FAIL(ਵਿਅੰਜਕ)                     \
  ਕਰੋ                                          \
    {                                         \
      r = (ਵਿਅੰਜਕ);                             \
      ਜੇ (UNLIKELY (r != json_gen_ਸਥਿਤੀ_ok)) \
        ਜਾਓ gen_error;                       \
  } ਜਦੋਂ (0)

/* Emit a string literal (typically a map key).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GEN_ਕੁੰਜੀ(GEN, LIT) GEN_OR_FAIL (json_gen_string ((GEN), (LIT), ਆਕਾਰ (LIT) - 1))

/* Emit a runtime string value.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GEN_STR(GEN, STR) GEN_OR_FAIL (json_gen_string ((GEN), (STR), ਲੜੀ_ਲੰਬਾਈ (STR)))

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cleanup_json_genp (json_gen_ctx **p)
{
  ਜੇ (*p)
    json_gen_free (*p);
}
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_json_gen __attribute__ ((cleanup (cleanup_json_genp)))

#ਜੇ_ਅੰਤ
