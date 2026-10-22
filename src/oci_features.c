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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <argp.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "crun.h"

ਸਥਿਰ ਅੱਖਰ doc[] = "OCI runtime";

ਸਥਿਰ ਬਣਤਰ argp_option options[] = { { 0 } };

ਸਥਿਰ ਅੱਖਰ ਦਲੀਲਾਂ_doc[] = "features";

ਸਥਿਰ error_t
parse_opt (ਪੂਰਨ ਕੁੰਜੀ, ਅੱਖਰ *arg arg_ਅਣਵਰਤਿਆ, ਬਣਤਰ argp_state *state arg_ਅਣਵਰਤਿਆ)
{
  ਜੇ (ਕੁੰਜੀ != ARGP_ਕੁੰਜੀ_NO_ਦਲੀਲਾਂ)
    {
      ਵਾਪਸ ARGP_ERR_UNKNOWN;
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਬਣਤਰ argp ਲੜੀਵਾਰ_argp = { options, parse_opt, ਦਲੀਲਾਂ_doc, doc, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ };

ਪੂਰਨ
crun_command_features (ਬਣਤਰ crun_global_arguments *global_ਦਲੀਲਾਂ, ਪੂਰਨ argc, ਅੱਖਰ **argv, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *json = ਨਿਰਾ;
  ਪੂਰਨ ਪਹਿਲਾ_arg = 0, ret = 0;
  cleanup_context libcrun_context_t *crun_context = ਨਿਰਾ;

  argp_parse (&ਲੜੀਵਾਰ_argp, argc, argv, 0, 0, &options);

  crun_context = new_libcrun_context (global_ਦਲੀਲਾਂ);

  ret = init_libcrun_context (crun_context, argv[ਪਹਿਲਾ_arg], global_ਦਲੀਲਾਂ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcrun_container_get_features_json (crun_context, &json, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਲਿਖੋ ("%s", json);

  ਵਾਪਸ 0;
}
