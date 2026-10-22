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
#నిర్వచితం_కాకపోతే JSON_GEN_UTILS_H
#నిర్వచించు JSON_GEN_UTILS_H

#అన్తర్భావ <config.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <ocispec/json_common.h>
#అన్తర్భావ "utils.h"

/* Helpers to reduce the boilerplate around the json_gen_* API.  Each macro
   evaluates a json_gen_* call, stores the result in the local variable `r`
   and jumps to the `gen_error` label when the call fails.  A function using
   them must declare `int r;` and provide a `gen_error:` label.  */
#నిర్వచించు GEN_OR_FAIL(సమాసం)                     \
  చేయి                                          \
    {                                         \
      r = (సమాసం);                             \
      ఒకవేళ (UNLIKELY (r != json_gen_స్థితి_ok)) \
        వెళ్ళు gen_error;                       \
  } అయితే (0)

/* Emit a string literal (typically a map key).  */
#నిర్వచించు GEN_కీలకం(GEN, LIT) GEN_OR_FAIL (json_gen_string ((GEN), (LIT), పరిమాణం (LIT) - 1))

/* Emit a runtime string value.  */
#నిర్వచించు GEN_STR(GEN, STR) GEN_OR_FAIL (json_gen_string ((GEN), (STR), అక్షరశ్రేణి_పొడవు (STR)))

స్థిర అంతఃవరుస శూన్య
cleanup_json_genp (json_gen_ctx **p)
{
  ఒకవేళ (*p)
    json_gen_free (*p);
}
#నిర్వచించు cleanup_json_gen __attribute__ ((cleanup (cleanup_json_genp)))

#అయితే_ముగింపు
