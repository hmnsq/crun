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
#اگر_غیر_تعریف_شدہ JSON_GEN_UTILS_H
#تعریف_کرو JSON_GEN_UTILS_H

#شامل <config.h>
#شامل <string.h>
#شامل <ocispec/json_common.h>
#شامل "utils.h"

/* Helpers to reduce the boilerplate around the json_gen_* API.  Each macro
   evaluates a json_gen_* call, stores the result in the local variable `r`
   and jumps to the `gen_error` label when the call fails.  A function using
   them must declare `int r;` and provide a `gen_error:` label.  */
#تعریف_کرو GEN_OR_FAIL(اظہاریہ)                     \
  کرو                                          \
    {                                         \
      r = (اظہاریہ);                             \
      اگر (UNLIKELY (r != json_gen_حالت_ok)) \
        جاؤ gen_error;                       \
  } جب (0)

/* Emit a string literal (typically a map key).  */
#تعریف_کرو GEN_کلید(GEN, LIT) GEN_OR_FAIL (json_gen_string ((GEN), (LIT), حجم (LIT) - 1))

/* Emit a runtime string value.  */
#تعریف_کرو GEN_STR(GEN, STR) GEN_OR_FAIL (json_gen_string ((GEN), (STR), لڑی_لمبائی (STR)))

ساکن اندرونی خالی
cleanup_json_genp (json_gen_ctx **p)
{
  اگر (*p)
    json_gen_free (*p);
}
#تعریف_کرو cleanup_json_gen __attribute__ ((cleanup (cleanup_json_genp)))

#اگر_ختم
