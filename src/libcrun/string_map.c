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

#include <config.h>
#include <stdlib.h>
#include <errno.h>
#include "string_map.h"
#include "utils.h"

#include <ocispec/runtime_spec_schema_config_schema.h>

sa_nracanaa kv_s
{
  var_na *key;
  var_na *value;
};

sa_nracanaa string_map_s
{
  size_t len;
  sa_nracanaa kv_s *kvs;

#ifdef HAVE_HSEARCH_R
  sa_nracanaa hsearch_data htab;
  bool htab_initialized;
#endif

  bool sorted;
};

static ကိန်းပြည့်
compare_kv (const void *a, const void *b)
{
  ပြန်ပေး strcmp (((sa_nracanaa kv_s *) a)->key, ((sa_nracanaa kv_s *) b)->key);
}

const var_na *
find_string_map_value (string_map *map, const var_na *name)

{
  sa_nracanaa kv_s *r, key;

  အကယ်၍ (map == NULL || map->len == 0)
    ပြန်ပေး NULL;

#ifdef HAVE_HSEARCH_R
  ENTRY e, *ep;

  /* Do not bother with hash tables for small maps.  */
  အကယ်၍ (map->len < 8)
    goto fallback;

  အကယ်၍ (! map->htab_initialized)
    {
      size_t i;

      အကယ်၍ (hcreate_r (map->len, &map->htab) == 0)
        goto fallback;

      အတွက် (i = 0; i < map->len; i++)
        {
          e.key = (var_na *) map->kvs[i].key;
          e.data = map->kvs[i].value;
          အကယ်၍ (hsearch_r (e, ENTER, &ep, &map->htab) == 0)
            {
              hdestroy_r (&map->htab);
              goto fallback;
            }
        }
      map->htab_initialized = true;
    }

  e.key = (var_na *) name;
  အကယ်၍ (hsearch_r (e, FIND, &ep, &map->htab) == 0)
    ပြန်ပေး NULL;

  ပြန်ပေး ep->data;

fallback:
#endif

  အကယ်၍ (! map->sorted)
    {
      qsort (map->kvs, map->len, parimaa_na (sa_nracanaa kv_s), compare_kv);
      map->sorted = true;
    }

  key.key = (var_na *) name;

  r = bsearch (&key, map->kvs, map->len, parimaa_na (sa_nracanaa kv_s), compare_kv);
  ပြန်ပေး r ? r->value : NULL;
}

string_map *
make_string_map_from_json (json_map_string_string *jmap)
{
  sa_nracanaa string_map_s *new_map = xmalloc0 (parimaa_na (sa_nracanaa string_map_s));
  size_t i;

  အကယ်၍ (jmap == NULL)
    ပြန်ပေး new_map;

  new_map->len = jmap->len;
  new_map->kvs = xmalloc0 (parimaa_na (sa_nracanaa kv_s) * (jmap->len + 1));

  အတွက် (i = 0; i < jmap->len; i++)
    {
      new_map->kvs[i].key = xstrdup (jmap->keys[i]);
      new_map->kvs[i].value = xstrdup (jmap->values[i]);
    }

  ပြန်ပေး new_map;
}

ကိန်းပြည့်
string_map_get_at (string_map *map, size_t index, const var_na **name, const var_na **value)
{
  အကယ်၍ (map == NULL || index >= map->len)
    {
      errno = ERANGE;
      ပြန်ပေး -1;
    }

  *name = map->kvs[index].key;
  *value = map->kvs[index].value;

  ပြန်ပေး 0;
}

void
free_string_map (string_map *map)
{
  size_t i;

  အကယ်၍ (map == NULL)
    ပြန်ပေး;

  အကယ်၍ (map->htab_initialized)
    hdestroy_r (&map->htab);

  အတွက် (i = 0; i < map->len; i++)
    {
      free (map->kvs[i].key);
      free (map->kvs[i].value);
    }
  free (map->kvs);
  free (map);
}

size_t
string_map_size (string_map *map)
{
  ပြန်ပေး map->len;
}
