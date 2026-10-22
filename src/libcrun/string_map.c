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

#شامل <config.h>
#شامل <stdlib.h>
#شامل <errno.h>
#شامل "string_map.h"
#شامل "utils.h"

#شامل <ocispec/runtime_spec_schema_config_schema.h>

ساختار kv_s
{
  نویسه *کلید;
  نویسه *مقدار;
};

ساختار string_map_s
{
  size_t len;
  ساختار kv_s *kvs;

#ifdef HAVE_HSEARCH_R
  ساختار hsearch_data htab;
  bool htab_initialized;
#endif

  bool sorted;
};

ایستا صحیح
compare_kv (ثابت تهی *a, ثابت تهی *b)
{
  بازگشت strcmp (((ساختار kv_s *) a)->کلید, ((ساختار kv_s *) b)->کلید);
}

ثابت نویسه *
find_string_map_مقدار (string_map *map, ثابت نویسه *name)

{
  ساختار kv_s *r, کلید;

  اگر (map == پوچ || map->len == 0)
    بازگشت پوچ;

#ifdef HAVE_HSEARCH_R
  ENTRY e, *ep;

  /* Do not bother with hash tables for small maps.  */
  اگر (map->len < 8)
    برو fallback;

  اگر (! map->htab_initialized)
    {
      size_t i;

      اگر (hcreate_r (map->len, &map->htab) == 0)
        برو fallback;

      برای (i = 0; i < map->len; i++)
        {
          e.کلید = (نویسه *) map->kvs[i].کلید;
          e.data = map->kvs[i].مقدار;
          اگر (hsearch_r (e, ENTER, &ep, &map->htab) == 0)
            {
              hdestroy_r (&map->htab);
              برو fallback;
            }
        }
      map->htab_initialized = true;
    }

  e.کلید = (نویسه *) name;
  اگر (hsearch_r (e, FIND, &ep, &map->htab) == 0)
    بازگشت پوچ;

  بازگشت ep->data;

fallback:
#endif

  اگر (! map->sorted)
    {
      qsort (map->kvs, map->len, اندازه (ساختار kv_s), compare_kv);
      map->sorted = true;
    }

  کلید.کلید = (نویسه *) name;

  r = bsearch (&کلید, map->kvs, map->len, اندازه (ساختار kv_s), compare_kv);
  بازگشت r ? r->مقدار : پوچ;
}

string_map *
make_string_map_from_json (json_map_string_string *jmap)
{
  ساختار string_map_s *new_map = xmalloc0 (اندازه (ساختار string_map_s));
  size_t i;

  اگر (jmap == پوچ)
    بازگشت new_map;

  new_map->len = jmap->len;
  new_map->kvs = xmalloc0 (اندازه (ساختار kv_s) * (jmap->len + 1));

  برای (i = 0; i < jmap->len; i++)
    {
      new_map->kvs[i].کلید = xstrdup (jmap->کلیدها[i]);
      new_map->kvs[i].مقدار = xstrdup (jmap->مقادیر[i]);
    }

  بازگشت new_map;
}

صحیح
string_map_get_at (string_map *map, size_t index, ثابت نویسه **name, ثابت نویسه **مقدار)
{
  اگر (map == پوچ || index >= map->len)
    {
      errno = ERANGE;
      بازگشت -1;
    }

  *name = map->kvs[index].کلید;
  *مقدار = map->kvs[index].مقدار;

  بازگشت 0;
}

تهی
free_string_map (string_map *map)
{
  size_t i;

  اگر (map == پوچ)
    بازگشت;

  اگر (map->htab_initialized)
    hdestroy_r (&map->htab);

  برای (i = 0; i < map->len; i++)
    {
      آزاد (map->kvs[i].کلید);
      آزاد (map->kvs[i].مقدار);
    }
  آزاد (map->kvs);
  آزاد (map);
}

size_t
string_map_size (string_map *map)
{
  بازگشت map->len;
}
