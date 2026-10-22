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
  वर्ण *कुञ्चिका;
  वर्ण *मानम्;
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
  पटिनिवत्त strcmp (((संरचना kv_s *) a)->कुञ्चिका, ((संरचना kv_s *) b)->कुञ्चिका);
}

ध्रुव वर्ण *
find_string_map_मानम् (string_map *map, ध्रुव वर्ण *name)

{
  संरचना kv_s *r, कुञ्चिका;

  सचे (map == रिक्त || map->len == 0)
    पटिनिवत्त रिक्त;

#ifdef HAVE_HSEARCH_R
  ENTRY e, *ep;

  /* Do not bother with hash tables for small maps.  */
  सचे (map->len < 8)
    गच्छ fallback;

  सचे (! map->htab_initialized)
    {
      size_t i;

      सचे (hcreate_r (map->len, &map->htab) == 0)
        गच्छ fallback;

      अत्थाय (i = 0; i < map->len; i++)
        {
          e.कुञ्चिका = (वर्ण *) map->kvs[i].कुञ्चिका;
          e.data = map->kvs[i].मानम्;
          सचे (hsearch_r (e, ENTER, &ep, &map->htab) == 0)
            {
              hdestroy_r (&map->htab);
              गच्छ fallback;
            }
        }
      map->htab_initialized = true;
    }

  e.कुञ्चिका = (वर्ण *) name;
  सचे (hsearch_r (e, FIND, &ep, &map->htab) == 0)
    पटिनिवत्त रिक्त;

  पटिनिवत्त ep->data;

fallback:
#endif

  सचे (! map->sorted)
    {
      qsort (map->kvs, map->len, परिमाण (संरचना kv_s), compare_kv);
      map->sorted = true;
    }

  कुञ्चिका.कुञ्चिका = (वर्ण *) name;

  r = bsearch (&कुञ्चिका, map->kvs, map->len, परिमाण (संरचना kv_s), compare_kv);
  पटिनिवत्त r ? r->मानम् : रिक्त;
}

string_map *
make_string_map_from_json (json_map_string_string *jmap)
{
  संरचना string_map_s *new_map = xmalloc0 (परिमाण (संरचना string_map_s));
  size_t i;

  सचे (jmap == रिक्त)
    पटिनिवत्त new_map;

  new_map->len = jmap->len;
  new_map->kvs = xmalloc0 (परिमाण (संरचना kv_s) * (jmap->len + 1));

  अत्थाय (i = 0; i < jmap->len; i++)
    {
      new_map->kvs[i].कुञ्चिका = xstrdup (jmap->कुञ्चिकाः[i]);
      new_map->kvs[i].मानम् = xstrdup (jmap->मानानि[i]);
    }

  पटिनिवत्त new_map;
}

पूर्णाङ्क
string_map_get_at (string_map *map, size_t index, ध्रुव वर्ण **name, ध्रुव वर्ण **मानम्)
{
  सचे (map == रिक्त || index >= map->len)
    {
      errno = ERANGE;
      पटिनिवत्त -1;
    }

  *name = map->kvs[index].कुञ्चिका;
  *मानम् = map->kvs[index].मानम्;

  पटिनिवत्त 0;
}

शून्य
free_string_map (string_map *map)
{
  size_t i;

  सचे (map == रिक्त)
    पटिनिवत्त;

  सचे (map->htab_initialized)
    hdestroy_r (&map->htab);

  अत्थाय (i = 0; i < map->len; i++)
    {
      मुक्त (map->kvs[i].कुञ्चिका);
      मुक्त (map->kvs[i].मानम्);
    }
  मुक्त (map->kvs);
  मुक्त (map);
}

size_t
string_map_size (string_map *map)
{
  पटिनिवत्त map->len;
}
