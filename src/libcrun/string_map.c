/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2025 Giuseppe Scrivano <giuseppe@scrivano.org>
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

#define _GNU_SOURCE

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव "string_map.h"
#अन्तर्भाव "utils.h"

#अन्तर्भाव <ocispec/runtime_spec_schema_config_schema.h>

संरचना kv_s
{
  वर्ण *किल्ली;
  वर्ण *मूल्य;
};

संरचना string_map_s
{
  size_t len;
  संरचना kv_s *kvs;

#ifdef HAVE_HSEARCH_R
  संरचना hsearch_data htab;
  bool htab_initialized;
#endif

  bool sorted;
};

स्थिर पूर्णाङ्क
compare_kv (ध्रुव शून्य *a, ध्रुव शून्य *b)
{
  प्रत्यागम strcmp (((संरचना kv_s *) a)->किल्ली, ((संरचना kv_s *) b)->किल्ली);
}

ध्रुव वर्ण *
find_string_map_मूल्य (string_map *map, ध्रुव वर्ण *name)

{
  संरचना kv_s *r, किल्ली;

  यदि (map == रिक्त || map->len == 0)
    प्रत्यागम रिक्त;

#ifdef HAVE_HSEARCH_R
  ENTRY e, *ep;

  /* Do not bother with hash tables for small maps.  */
  यदि (map->len < 8)
    गच्छ fallback;

  यदि (! map->htab_initialized)
    {
      size_t i;

      यदि (hcreate_r (map->len, &map->htab) == 0)
        गच्छ fallback;

      प्रति (i = 0; i < map->len; i++)
        {
          e.किल्ली = (वर्ण *) map->kvs[i].किल्ली;
          e.data = map->kvs[i].मूल्य;
          यदि (hsearch_r (e, ENTER, &ep, &map->htab) == 0)
            {
              hdestroy_r (&map->htab);
              गच्छ fallback;
            }
        }
      map->htab_initialized = true;
    }

  e.किल्ली = (वर्ण *) name;
  यदि (hsearch_r (e, FIND, &ep, &map->htab) == 0)
    प्रत्यागम रिक्त;

  प्रत्यागम ep->data;

fallback:
#endif

  यदि (! map->sorted)
    {
      qsort (map->kvs, map->len, परिमाण (संरचना kv_s), compare_kv);
      map->sorted = true;
    }

  किल्ली.किल्ली = (वर्ण *) name;

  r = bsearch (&किल्ली, map->kvs, map->len, परिमाण (संरचना kv_s), compare_kv);
  प्रत्यागम r ? r->मूल्य : रिक्त;
}

string_map *
make_string_map_from_json (json_map_string_string *jmap)
{
  संरचना string_map_s *new_map = xmalloc0 (परिमाण (संरचना string_map_s));
  size_t i;

  यदि (jmap == रिक्त)
    प्रत्यागम new_map;

  new_map->len = jmap->len;
  new_map->kvs = xmalloc0 (परिमाण (संरचना kv_s) * (jmap->len + 1));

  प्रति (i = 0; i < jmap->len; i++)
    {
      new_map->kvs[i].किल्ली = xstrdup (jmap->किल्ल्या[i]);
      new_map->kvs[i].मूल्य = xstrdup (jmap->मूल्ये[i]);
    }

  प्रत्यागम new_map;
}

पूर्णाङ्क
string_map_get_at (string_map *map, size_t index, ध्रुव वर्ण **name, ध्रुव वर्ण **मूल्य)
{
  यदि (map == रिक्त || index >= map->len)
    {
      errno = ERANGE;
      प्रत्यागम -1;
    }

  *name = map->kvs[index].किल्ली;
  *मूल्य = map->kvs[index].मूल्य;

  प्रत्यागम 0;
}

शून्य
free_string_map (string_map *map)
{
  size_t i;

  यदि (map == रिक्त)
    प्रत्यागम;

  यदि (map->htab_initialized)
    hdestroy_r (&map->htab);

  प्रति (i = 0; i < map->len; i++)
    {
      मुक्त (map->kvs[i].किल्ली);
      मुक्त (map->kvs[i].मूल्य);
    }
  मुक्त (map->kvs);
  मुक्त (map);
}

size_t
string_map_size (string_map *map)
{
  प्रत्यागम map->len;
}
