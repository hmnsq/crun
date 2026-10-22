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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <errno.h>
#অন্তর্ভাৱ "string_map.h"
#অন্তর্ভাৱ "utils.h"

#অন্তর্ভাৱ <ocispec/runtime_spec_schema_config_schema.h>

সংরচনা kv_s
{
  ৱর্ণ *চাবি;
  ৱর্ণ *মান;
};

সংরচনা string_map_s
{
  size_t len;
  সংরচনা kv_s *kvs;

#ifdef HAVE_HSEARCH_R
  সংরচনা hsearch_data htab;
  bool htab_initialized;
#endif

  bool sorted;
};

স্থির পূর্ণাঙ্ক
compare_kv (ধ্রুৱ শূন্য *a, ধ্রুৱ শূন্য *b)
{
  ঘূৰাই strcmp (((সংরচনা kv_s *) a)->চাবি, ((সংরচনা kv_s *) b)->চাবি);
}

ধ্রুৱ ৱর্ণ *
find_string_map_মান (string_map *map, ধ্রুৱ ৱর্ণ *name)

{
  সংরচনা kv_s *r, চাবি;

  যদি (map == রিক্ত || map->len == 0)
    ঘূৰাই রিক্ত;

#ifdef HAVE_HSEARCH_R
  ENTRY e, *ep;

  /* Do not bother with hash tables for small maps.  */
  যদি (map->len < 8)
    গচ্ছ fallback;

  যদি (! map->htab_initialized)
    {
      size_t i;

      যদি (hcreate_r (map->len, &map->htab) == 0)
        গচ্ছ fallback;

      বাবে (i = 0; i < map->len; i++)
        {
          e.চাবি = (ৱর্ণ *) map->kvs[i].চাবি;
          e.data = map->kvs[i].মান;
          যদি (hsearch_r (e, ENTER, &ep, &map->htab) == 0)
            {
              hdestroy_r (&map->htab);
              গচ্ছ fallback;
            }
        }
      map->htab_initialized = true;
    }

  e.চাবি = (ৱর্ণ *) name;
  যদি (hsearch_r (e, FIND, &ep, &map->htab) == 0)
    ঘূৰাই রিক্ত;

  ঘূৰাই ep->data;

fallback:
#endif

  যদি (! map->sorted)
    {
      qsort (map->kvs, map->len, পরিমাণ (সংরচনা kv_s), compare_kv);
      map->sorted = true;
    }

  চাবি.চাবি = (ৱর্ণ *) name;

  r = bsearch (&চাবি, map->kvs, map->len, পরিমাণ (সংরচনা kv_s), compare_kv);
  ঘূৰাই r ? r->মান : রিক্ত;
}

string_map *
make_string_map_from_json (json_map_string_string *jmap)
{
  সংরচনা string_map_s *new_map = xmalloc0 (পরিমাণ (সংরচনা string_map_s));
  size_t i;

  যদি (jmap == রিক্ত)
    ঘূৰাই new_map;

  new_map->len = jmap->len;
  new_map->kvs = xmalloc0 (পরিমাণ (সংরচনা kv_s) * (jmap->len + 1));

  বাবে (i = 0; i < jmap->len; i++)
    {
      new_map->kvs[i].চাবি = xstrdup (jmap->চাবিগুলি[i]);
      new_map->kvs[i].মান = xstrdup (jmap->মানসমূহ[i]);
    }

  ঘূৰাই new_map;
}

পূর্ণাঙ্ক
string_map_get_at (string_map *map, size_t index, ধ্রুৱ ৱর্ণ **name, ধ্রুৱ ৱর্ণ **মান)
{
  যদি (map == রিক্ত || index >= map->len)
    {
      errno = ERANGE;
      ঘূৰাই -1;
    }

  *name = map->kvs[index].চাবি;
  *মান = map->kvs[index].মান;

  ঘূৰাই 0;
}

শূন্য
free_string_map (string_map *map)
{
  size_t i;

  যদি (map == রিক্ত)
    ঘূৰাই;

  যদি (map->htab_initialized)
    hdestroy_r (&map->htab);

  বাবে (i = 0; i < map->len; i++)
    {
      মুক্ত (map->kvs[i].চাবি);
      মুক্ত (map->kvs[i].মান);
    }
  মুক্ত (map->kvs);
  মুক্ত (map);
}

size_t
string_map_size (string_map *map)
{
  ঘূৰাই map->len;
}
