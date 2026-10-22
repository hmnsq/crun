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

#परिभाषय _GNU_SOURCE

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

#यदि_परिभाषितम् HAVE_HSEARCH_R
  संरचना hsearch_दत्त htab;
  bool htab_initialized;
#यदि_अन्तः

  bool sorted;
};

स्थिर पूर्णाङ्क
compare_kv (ध्रुव शून्य *a, ध्रुव शून्य *b)
{
  प्रत्यागम वाक्यं_तुलय (((संरचना kv_s *) a)->कुञ्चिका, ((संरचना kv_s *) b)->कुञ्चिका);
}

ध्रुव वर्ण *
find_string_map_मानम् (string_map *map, ध्रुव वर्ण *name)

{
  संरचना kv_s *r, कुञ्चिका;

  यदि (map == रिक्त || map->len == 0)
    प्रत्यागम रिक्त;

#यदि_परिभाषितम् HAVE_HSEARCH_R
  प्रविष्टिः e, *ep;

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
          e.कुञ्चिका = (वर्ण *) map->kvs[i].कुञ्चिका;
          e.दत्त = map->kvs[i].मानम्;
          यदि (hsearch_r (e, ENTER, &ep, &map->htab) == 0)
            {
              hdestroy_r (&map->htab);
              गच्छ fallback;
            }
        }
      map->htab_initialized = true;
    }

  e.कुञ्चिका = (वर्ण *) name;
  यदि (hsearch_r (e, FIND, &ep, &map->htab) == 0)
    प्रत्यागम रिक्त;

  प्रत्यागम ep->दत्त;

fallback:
#यदि_अन्तः

  यदि (! map->sorted)
    {
      शीघ्रं_क्रमय (map->kvs, map->len, परिमाण (संरचना kv_s), compare_kv);
      map->sorted = true;
    }

  कुञ्चिका.कुञ्चिका = (वर्ण *) name;

  r = bsearch (&कुञ्चिका, map->kvs, map->len, परिमाण (संरचना kv_s), compare_kv);
  प्रत्यागम r ? r->मानम् : रिक्त;
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
      new_map->kvs[i].कुञ्चिका = xstrdup (jmap->कुञ्चिकाः[i]);
      new_map->kvs[i].मानम् = xstrdup (jmap->मानानि[i]);
    }

  प्रत्यागम new_map;
}

पूर्णाङ्क
string_map_get_at (string_map *map, size_t सूचकांक, ध्रुव वर्ण **name, ध्रुव वर्ण **मानम्)
{
  यदि (map == रिक्त || सूचकांक >= map->len)
    {
      errno = ERANGE;
      प्रत्यागम -1;
    }

  *name = map->kvs[सूचकांक].कुञ्चिका;
  *मानम् = map->kvs[सूचकांक].मानम्;

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
      मुक्त (map->kvs[i].कुञ्चिका);
      मुक्त (map->kvs[i].मानम्);
    }
  मुक्त (map->kvs);
  मुक्त (map);
}

size_t
string_map_आकार (string_map *map)
{
  प्रत्यागम map->len;
}
