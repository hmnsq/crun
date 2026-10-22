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

#ضمن <config.h>
#ضمن <stdlib.h>
#ضمن <errno.h>
#ضمن "string_map.h"
#ضمن "utils.h"

#ضمن <ocispec/runtime_spec_schema_config_schema.h>

بنية kv_s
{
  حرف *مفتاح;
  حرف *قيمة;
};

بنية string_map_s
{
  size_t len;
  بنية kv_s *kvs;

#ifdef HAVE_HSEARCH_R
  بنية hsearch_data htab;
  bool htab_initialized;
#endif

  bool sorted;
};

ساكن صحيح
compare_kv (ثابت فراغ *a, ثابت فراغ *b)
{
  أرجع strcmp (((بنية kv_s *) a)->مفتاح, ((بنية kv_s *) b)->مفتاح);
}

ثابت حرف *
find_string_map_قيمة (string_map *map, ثابت حرف *name)

{
  بنية kv_s *r, مفتاح;

  إذا (map == عدم || map->len == 0)
    أرجع عدم;

#ifdef HAVE_HSEARCH_R
  ENTRY e, *ep;

  /* Do not bother with hash tables for small maps.  */
  إذا (map->len < 8)
    اذهب fallback;

  إذا (! map->htab_initialized)
    {
      size_t i;

      إذا (hcreate_r (map->len, &map->htab) == 0)
        اذهب fallback;

      لكل (i = 0; i < map->len; i++)
        {
          e.مفتاح = (حرف *) map->kvs[i].مفتاح;
          e.data = map->kvs[i].قيمة;
          إذا (hsearch_r (e, ENTER, &ep, &map->htab) == 0)
            {
              hdestroy_r (&map->htab);
              اذهب fallback;
            }
        }
      map->htab_initialized = true;
    }

  e.مفتاح = (حرف *) name;
  إذا (hsearch_r (e, FIND, &ep, &map->htab) == 0)
    أرجع عدم;

  أرجع ep->data;

fallback:
#endif

  إذا (! map->sorted)
    {
      qsort (map->kvs, map->len, حجم (بنية kv_s), compare_kv);
      map->sorted = true;
    }

  مفتاح.مفتاح = (حرف *) name;

  r = bsearch (&مفتاح, map->kvs, map->len, حجم (بنية kv_s), compare_kv);
  أرجع r ? r->قيمة : عدم;
}

string_map *
make_string_map_from_json (json_map_string_string *jmap)
{
  بنية string_map_s *new_map = xmalloc0 (حجم (بنية string_map_s));
  size_t i;

  إذا (jmap == عدم)
    أرجع new_map;

  new_map->len = jmap->len;
  new_map->kvs = xmalloc0 (حجم (بنية kv_s) * (jmap->len + 1));

  لكل (i = 0; i < jmap->len; i++)
    {
      new_map->kvs[i].مفتاح = xstrdup (jmap->مفاتيح[i]);
      new_map->kvs[i].قيمة = xstrdup (jmap->قيم[i]);
    }

  أرجع new_map;
}

صحيح
string_map_get_at (string_map *map, size_t index, ثابت حرف **name, ثابت حرف **قيمة)
{
  إذا (map == عدم || index >= map->len)
    {
      errno = ERANGE;
      أرجع -1;
    }

  *name = map->kvs[index].مفتاح;
  *قيمة = map->kvs[index].قيمة;

  أرجع 0;
}

فراغ
free_string_map (string_map *map)
{
  size_t i;

  إذا (map == عدم)
    أرجع;

  إذا (map->htab_initialized)
    hdestroy_r (&map->htab);

  لكل (i = 0; i < map->len; i++)
    {
      حرر (map->kvs[i].مفتاح);
      حرر (map->kvs[i].قيمة);
    }
  حرر (map->kvs);
  حرر (map);
}

size_t
string_map_size (string_map *map)
{
  أرجع map->len;
}
