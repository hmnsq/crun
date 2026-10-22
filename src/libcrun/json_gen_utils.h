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
#ifndef JSON_GEN_UTILS_H
#define JSON_GEN_UTILS_H

#ضمن <config.h>
#ضمن <string.h>
#ضمن <ocispec/json_common.h>
#ضمن "utils.h"

/* Helpers to reduce the boilerplate around the json_gen_* API.  Each macro
   evaluates a json_gen_* call, stores the result in the local variable `r`
   and jumps to the `gen_error` label when the call fails.  A function using
   them must declare `int r;` and provide a `gen_error:` label.  */
#define GEN_OR_FAIL(دەربڕین)                     \
  افعل                                          \
    {                                         \
      r = (دەربڕین);                             \
      إذا (UNLIKELY (r != json_gen_status_ok)) \
        اذهب gen_error;                       \
  } طالما (0)

/* Emit a string literal (typically a map key).  */
#define GEN_کلیل(GEN, LIT) GEN_OR_FAIL (json_gen_string ((GEN), (LIT), حجم (LIT) - 1))

/* Emit a runtime string value.  */
#define GEN_STR(GEN, STR) GEN_OR_FAIL (json_gen_string ((GEN), (STR), strlen (STR)))

ساكن مضمن فراغ
cleanup_json_genp (json_gen_ctx **p)
{
  إذا (*p)
    json_gen_free (*p);
}
#define cleanup_json_gen __attribute__ ((cleanup (cleanup_json_genp)))

#endif
