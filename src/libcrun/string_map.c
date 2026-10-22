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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <errno.h>
#অন্তর্ভাব "string_map.h"
#অন্তর্ভাব "utils.h"

#অন্তর্ভাব <ocispec/runtime_spec_schema_config_schema.h>

কাঠামো kv_s
{
  অক্ষর *চাবি;
  অক্ষর *মান;
};

কাঠামো string_map_s
{
  size_t len;
  কাঠামো kv_s *kvs;

#ifdef HAVE_HSEARCH_R
  কাঠামো hsearch_তথ্য htab;
  bool htab_initialized;
#endif

  bool sorted;
};

স্থির পূর্ণসংখ্যা
compare_kv (ধ্রুবক শূন্য *a, ধ্রুবক শূন্য *b)
{
  ফেরত strcmp (((কাঠামো kv_s *) a)->চাবি, ((কাঠামো kv_s *) b)->চাবি);
}

ধ্রুবক অক্ষর *
find_string_map_মান (string_map *map, ধ্রুবক অক্ষর *name)

{
  কাঠামো kv_s *r, চাবি;

  যদি (map == ফাঁকা || map->len == 0)
    ফেরত ফাঁকা;

#ifdef HAVE_HSEARCH_R
  ENTRY e, *ep;

  /* Do not bother with hash tables for small maps.  */
  যদি (map->len < 8)
    যাও fallback;

  যদি (! map->htab_initialized)
    {
      size_t i;

      যদি (hcreate_r (map->len, &map->htab) == 0)
        যাও fallback;

      জন্য (i = 0; i < map->len; i++)
        {
          e.চাবি = (অক্ষর *) map->kvs[i].চাবি;
          e.তথ্য = map->kvs[i].মান;
          যদি (hsearch_r (e, ENTER, &ep, &map->htab) == 0)
            {
              hdestroy_r (&map->htab);
              যাও fallback;
            }
        }
      map->htab_initialized = true;
    }

  e.চাবি = (অক্ষর *) name;
  যদি (hsearch_r (e, FIND, &ep, &map->htab) == 0)
    ফেরত ফাঁকা;

  ফেরত ep->তথ্য;

fallback:
#endif

  যদি (! map->sorted)
    {
      qsort (map->kvs, map->len, আকার (কাঠামো kv_s), compare_kv);
      map->sorted = true;
    }

  চাবি.চাবি = (অক্ষর *) name;

  r = bsearch (&চাবি, map->kvs, map->len, আকার (কাঠামো kv_s), compare_kv);
  ফেরত r ? r->মান : ফাঁকা;
}

string_map *
make_string_map_from_json (json_map_string_string *jmap)
{
  কাঠামো string_map_s *new_map = xmalloc0 (আকার (কাঠামো string_map_s));
  size_t i;

  যদি (jmap == ফাঁকা)
    ফেরত new_map;

  new_map->len = jmap->len;
  new_map->kvs = xmalloc0 (আকার (কাঠামো kv_s) * (jmap->len + 1));

  জন্য (i = 0; i < jmap->len; i++)
    {
      new_map->kvs[i].চাবি = xstrdup (jmap->চাবিগুলি[i]);
      new_map->kvs[i].মান = xstrdup (jmap->মানসমূহ[i]);
    }

  ফেরত new_map;
}

পূর্ণসংখ্যা
string_map_get_at (string_map *map, size_t সূচক, ধ্রুবক অক্ষর **name, ধ্রুবক অক্ষর **মান)
{
  যদি (map == ফাঁকা || সূচক >= map->len)
    {
      errno = ERANGE;
      ফেরত -1;
    }

  *name = map->kvs[সূচক].চাবি;
  *মান = map->kvs[সূচক].মান;

  ফেরত 0;
}

শূন্য
free_string_map (string_map *map)
{
  size_t i;

  যদি (map == ফাঁকা)
    ফেরত;

  যদি (map->htab_initialized)
    hdestroy_r (&map->htab);

  জন্য (i = 0; i < map->len; i++)
    {
      মুক্ত (map->kvs[i].চাবি);
      মুক্ত (map->kvs[i].মান);
    }
  মুক্ত (map->kvs);
  মুক্ত (map);
}

size_t
string_map_আকার (string_map *map)
{
  ফেরত map->len;
}
