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
  वर्ण *साँचो;
  वर्ण *मान;
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

स्थिर पूर्णाङ्क
compare_kv (अचर शून्य *a, अचर शून्य *b)
{
  फर्काऊ म_भेद (((संरचना kv_s *) a)->साँचो, ((संरचना kv_s *) b)->साँचो);
}

अचर वर्ण *
find_string_map_मान (string_map *map, अचर वर्ण *name)

{
  संरचना kv_s *r, साँचो;

  यदि (map == रिक्त || map->len == 0)
    फर्काऊ रिक्त;

#ifdef HAVE_HSEARCH_R
  ENTRY e, *ep;

  /* Do not bother with hash tables for small maps.  */
  यदि (map->len < 8)
    गच्छ fallback;

  यदि (! map->htab_initialized)
    {
      माप_प्रकार i;

      यदि (hcreate_r (map->len, &map->htab) == 0)
        गच्छ fallback;

      लागि (i = 0; i < map->len; i++)
        {
          e.साँचो = (वर्ण *) map->kvs[i].साँचो;
          e.data = map->kvs[i].मान;
          यदि (hsearch_r (e, ENTER, &ep, &map->htab) == 0)
            {
              hdestroy_r (&map->htab);
              गच्छ fallback;
            }
        }
      map->htab_initialized = true;
    }

  e.साँचो = (वर्ण *) name;
  यदि (hsearch_r (e, FIND, &ep, &map->htab) == 0)
    फर्काऊ रिक्त;

  फर्काऊ ep->data;

fallback:
#endif

  यदि (! map->sorted)
    {
      क्विक (map->kvs, map->len, परिमाण (संरचना kv_s), compare_kv);
      map->sorted = true;
    }

  साँचो.साँचो = (वर्ण *) name;

  r = द्वा_खोज (&साँचो, map->kvs, map->len, परिमाण (संरचना kv_s), compare_kv);
  फर्काऊ r ? r->मान : रिक्त;
}

string_map *
make_string_map_from_json (json_map_string_string *jmap)
{
  संरचना string_map_s *new_map = xmalloc0 (परिमाण (संरचना string_map_s));
  माप_प्रकार i;

  यदि (jmap == रिक्त)
    फर्काऊ new_map;

  new_map->len = jmap->len;
  new_map->kvs = xmalloc0 (परिमाण (संरचना kv_s) * (jmap->len + 1));

  लागि (i = 0; i < jmap->len; i++)
    {
      new_map->kvs[i].साँचो = xstrdup (jmap->साँचोहरू[i]);
      new_map->kvs[i].मान = xstrdup (jmap->मानहरू[i]);
    }

  फर्काऊ new_map;
}

पूर्णाङ्क
string_map_get_at (string_map *map, माप_प्रकार index, अचर वर्ण **name, अचर वर्ण **मान)
{
  यदि (map == रिक्त || index >= map->len)
    {
      त्रुटि_सं = दुस्फल;
      फर्काऊ -1;
    }

  *name = map->kvs[index].साँचो;
  *मान = map->kvs[index].मान;

  फर्काऊ 0;
}

शून्य
free_string_map (string_map *map)
{
  माप_प्रकार i;

  यदि (map == रिक्त)
    फर्काऊ;

  यदि (map->htab_initialized)
    hdestroy_r (&map->htab);

  लागि (i = 0; i < map->len; i++)
    {
      मुक्त (map->kvs[i].साँचो);
      मुक्त (map->kvs[i].मान);
    }
  मुक्त (map->kvs);
  मुक्त (map);
}

माप_प्रकार
string_map_size (string_map *map)
{
  फर्काऊ map->len;
}
