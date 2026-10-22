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

#تعریف_کرو GNU_ماخذ

#شامل <config.h>
#شامل <stdlib.h>
#شامل <errno.h>
#شامل "string_map.h"
#شامل "utils.h"

#شامل <ocispec/runtime_spec_schema_config_schema.h>

ساخت kv_s
{
  حرف *کلید;
  حرف *قدر;
};

ساخت string_map_s
{
  size_t len;
  ساخت kv_s *kvs;

#اگر_تعریف_شدہ HAVE_HSEARCH_R
  ساخت hsearch_ڈیٹا htab;
  bool htab_initialized;
#اگر_ختم

  bool sorted;
};

ساکن صحیح
compare_kv (ثابت خالی *a, ثابت خالی *b)
{
  واپس strcmp (((ساخت kv_s *) a)->کلید, ((ساخت kv_s *) b)->کلید);
}

ثابت حرف *
ڈھونڈو_string_map_قدر (string_map *map, ثابت حرف *name)

{
  ساخت kv_s *r, کلید;

  اگر (map == تہی || map->len == 0)
    واپس تہی;

#اگر_تعریف_شدہ HAVE_HSEARCH_R
  اندراج_2 e, *ep;

  /* Do not bother with hash tables for small maps.  */
  اگر (map->len < 8)
    جاؤ fallback;

  اگر (! map->htab_initialized)
    {
      size_t i;

      اگر (hcreate_r (map->len, &map->htab) == 0)
        جاؤ fallback;

      برائے (i = 0; i < map->len; i++)
        {
          e.کلید = (حرف *) map->kvs[i].کلید;
          e.ڈیٹا = map->kvs[i].قدر;
          اگر (hsearch_r (e, ENTER, &ep, &map->htab) == 0)
            {
              hdestroy_r (&map->htab);
              جاؤ fallback;
            }
        }
      map->htab_initialized = true;
    }

  e.کلید = (حرف *) name;
  اگر (hsearch_r (e, ڈھونڈو, &ep, &map->htab) == 0)
    واپس تہی;

  واپس ep->ڈیٹا;

fallback:
#اگر_ختم

  اگر (! map->sorted)
    {
      qsort (map->kvs, map->len, حجم (ساخت kv_s), compare_kv);
      map->sorted = true;
    }

  کلید.کلید = (حرف *) name;

  r = bsearch (&کلید, map->kvs, map->len, حجم (ساخت kv_s), compare_kv);
  واپس r ? r->قدر : تہی;
}

string_map *
make_string_map_from_json (json_map_string_string *jmap)
{
  ساخت string_map_s *new_map = xmalloc0 (حجم (ساخت string_map_s));
  size_t i;

  اگر (jmap == تہی)
    واپس new_map;

  new_map->len = jmap->len;
  new_map->kvs = xmalloc0 (حجم (ساخت kv_s) * (jmap->len + 1));

  برائے (i = 0; i < jmap->len; i++)
    {
      new_map->kvs[i].کلید = xstrdup (jmap->کلیدیں[i]);
      new_map->kvs[i].قدر = xstrdup (jmap->اقدار[i]);
    }

  واپس new_map;
}

صحیح
string_map_get_at (string_map *map, size_t اشاریہ, ثابت حرف **name, ثابت حرف **قدر)
{
  اگر (map == تہی || اشاریہ >= map->len)
    {
      errno = ERANGE;
      واپس -1;
    }

  *name = map->kvs[اشاریہ].کلید;
  *قدر = map->kvs[اشاریہ].قدر;

  واپس 0;
}

خالی
free_string_map (string_map *map)
{
  size_t i;

  اگر (map == تہی)
    واپس;

  اگر (map->htab_initialized)
    hdestroy_r (&map->htab);

  برائے (i = 0; i < map->len; i++)
    {
      آزاد (map->kvs[i].کلید);
      آزاد (map->kvs[i].قدر);
    }
  آزاد (map->kvs);
  آزاد (map);
}

size_t
string_map_سائز (string_map *map)
{
  واپس map->len;
}
