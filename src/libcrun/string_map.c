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
  अक्षर *किल्ली;
  अक्षर *मूल्य;
};

संरचना string_map_s
{
  माप_प्रकार len;
  संरचना kv_s *kvs;

#ifdef HAVE_HSEARCH_R
  संरचना hsearch_data htab;
  bool htab_initialized;
#endif

  bool sorted;
};

स्थिर पूर्णांक
compare_kv (स्थिरांक शून्य *a, स्थिरांक शून्य *b)
{
  परत म_भेद (((संरचना kv_s *) a)->किल्ली, ((संरचना kv_s *) b)->किल्ली);
}

स्थिरांक अक्षर *
find_string_map_मूल्य (string_map *map, स्थिरांक अक्षर *name)

{
  संरचना kv_s *r, किल्ली;

  जर (map == रिक्त || map->len == 0)
    परत रिक्त;

#ifdef HAVE_HSEARCH_R
  ENTRY e, *ep;

  /* Do not bother with hash tables for small maps.  */
  जर (map->len < 8)
    जा fallback;

  जर (! map->htab_initialized)
    {
      माप_प्रकार i;

      जर (hcreate_r (map->len, &map->htab) == 0)
        जा fallback;

      साठी (i = 0; i < map->len; i++)
        {
          e.किल्ली = (अक्षर *) map->kvs[i].किल्ली;
          e.data = map->kvs[i].मूल्य;
          जर (hsearch_r (e, ENTER, &ep, &map->htab) == 0)
            {
              hdestroy_r (&map->htab);
              जा fallback;
            }
        }
      map->htab_initialized = true;
    }

  e.किल्ली = (अक्षर *) name;
  जर (hsearch_r (e, FIND, &ep, &map->htab) == 0)
    परत रिक्त;

  परत ep->data;

fallback:
#endif

  जर (! map->sorted)
    {
      क्विक (map->kvs, map->len, आकार (संरचना kv_s), compare_kv);
      map->sorted = true;
    }

  किल्ली.किल्ली = (अक्षर *) name;

  r = द्वा_खोज (&किल्ली, map->kvs, map->len, आकार (संरचना kv_s), compare_kv);
  परत r ? r->मूल्य : रिक्त;
}

string_map *
make_string_map_from_json (json_map_string_string *jmap)
{
  संरचना string_map_s *new_map = xmalloc0 (आकार (संरचना string_map_s));
  माप_प्रकार i;

  जर (jmap == रिक्त)
    परत new_map;

  new_map->len = jmap->len;
  new_map->kvs = xmalloc0 (आकार (संरचना kv_s) * (jmap->len + 1));

  साठी (i = 0; i < jmap->len; i++)
    {
      new_map->kvs[i].किल्ली = xstrdup (jmap->किल्ल्या[i]);
      new_map->kvs[i].मूल्य = xstrdup (jmap->मूल्ये[i]);
    }

  परत new_map;
}

पूर्णांक
string_map_get_at (string_map *map, माप_प्रकार index, स्थिरांक अक्षर **name, स्थिरांक अक्षर **मूल्य)
{
  जर (map == रिक्त || index >= map->len)
    {
      त्रुटि_सं = दुस्फल;
      परत -1;
    }

  *name = map->kvs[index].किल्ली;
  *मूल्य = map->kvs[index].मूल्य;

  परत 0;
}

शून्य
free_string_map (string_map *map)
{
  माप_प्रकार i;

  जर (map == रिक्त)
    परत;

  जर (map->htab_initialized)
    hdestroy_r (&map->htab);

  साठी (i = 0; i < map->len; i++)
    {
      मुक्त (map->kvs[i].किल्ली);
      मुक्त (map->kvs[i].मूल्य);
    }
  मुक्त (map->kvs);
  मुक्त (map);
}

माप_प्रकार
string_map_size (string_map *map)
{
  परत map->len;
}
