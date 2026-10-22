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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <ocispec/json_common.h>
#অন্তর্ভাৱ "utils.h"

/* Helpers to reduce the boilerplate around the json_gen_* API.  Each macro
   evaluates a json_gen_* call, stores the result in the local variable `r`
   and jumps to the `gen_error` label when the call fails.  A function using
   them must declare `int r;` and provide a `gen_error:` label.  */
#define GEN_OR_FAIL(ৰাশিমালা)                     \
  কৰা                                          \
    {                                         \
      r = (ৰাশিমালা);                             \
      যদি (UNLIKELY (r != json_gen_status_ok)) \
        গচ্ছ gen_error;                       \
  } যেতিয়ালৈ (0)

/* Emit a string literal (typically a map key).  */
#define GEN_চাবি(GEN, LIT) GEN_OR_FAIL (json_gen_string ((GEN), (LIT), পরিমাণ (LIT) - 1))

/* Emit a runtime string value.  */
#define GEN_STR(GEN, STR) GEN_OR_FAIL (json_gen_string ((GEN), (STR), strlen (STR)))

স্থির অন্তঃস্থ শূন্য
cleanup_json_genp (json_gen_ctx **p)
{
  যদি (*p)
    json_gen_free (*p);
}
#define cleanup_json_gen __attribute__ ((cleanup (cleanup_json_genp)))

#endif
