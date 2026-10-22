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

static ចំនួនគត់
compare_kv (const void *a, const void *b)
{
  ត្រឡប់ strcmp (((sa_nracanaa kv_s *) a)->key, ((sa_nracanaa kv_s *) b)->key);
}

const var_na *
find_string_map_value (string_map *map, const var_na *name)

{
  sa_nracanaa kv_s *r, key;

  ប្រសិនបើ (map == NULL || map->len == 0)
    ត្រឡប់ NULL;

#ifdef HAVE_HSEARCH_R
  ENTRY e, *ep;

  /* Do not bother with hash tables for small maps.  */
  ប្រសិនបើ (map->len < 8)
    goto fallback;

  ប្រសិនបើ (! map->htab_initialized)
    {
      size_t i;

      ប្រសិនបើ (hcreate_r (map->len, &map->htab) == 0)
        goto fallback;

      សម្រាប់ (i = 0; i < map->len; i++)
        {
          e.key = (var_na *) map->kvs[i].key;
          e.data = map->kvs[i].value;
          ប្រសិនបើ (hsearch_r (e, ENTER, &ep, &map->htab) == 0)
            {
              hdestroy_r (&map->htab);
              goto fallback;
            }
        }
      map->htab_initialized = true;
    }

  e.key = (var_na *) name;
  ប្រសិនបើ (hsearch_r (e, FIND, &ep, &map->htab) == 0)
    ត្រឡប់ NULL;

  ត្រឡប់ ep->data;

fallback:
#endif

  ប្រសិនបើ (! map->sorted)
    {
      qsort (map->kvs, map->len, parimaa_na (sa_nracanaa kv_s), compare_kv);
      map->sorted = true;
    }

  key.key = (var_na *) name;

  r = bsearch (&key, map->kvs, map->len, parimaa_na (sa_nracanaa kv_s), compare_kv);
  ត្រឡប់ r ? r->value : NULL;
}

string_map *
make_string_map_from_json (json_map_string_string *jmap)
{
  sa_nracanaa string_map_s *new_map = xmalloc0 (parimaa_na (sa_nracanaa string_map_s));
  size_t i;

  ប្រសិនបើ (jmap == NULL)
    ត្រឡប់ new_map;

  new_map->len = jmap->len;
  new_map->kvs = xmalloc0 (parimaa_na (sa_nracanaa kv_s) * (jmap->len + 1));

  សម្រាប់ (i = 0; i < jmap->len; i++)
    {
      new_map->kvs[i].key = xstrdup (jmap->keys[i]);
      new_map->kvs[i].value = xstrdup (jmap->values[i]);
    }

  ត្រឡប់ new_map;
}

ចំនួនគត់
string_map_get_at (string_map *map, size_t index, const var_na **name, const var_na **value)
{
  ប្រសិនបើ (map == NULL || index >= map->len)
    {
      errno = ERANGE;
      ត្រឡប់ -1;
    }

  *name = map->kvs[index].key;
  *value = map->kvs[index].value;

  ត្រឡប់ 0;
}

void
free_string_map (string_map *map)
{
  size_t i;

  ប្រសិនបើ (map == NULL)
    ត្រឡប់;

  ប្រសិនបើ (map->htab_initialized)
    hdestroy_r (&map->htab);

  សម្រាប់ (i = 0; i < map->len; i++)
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
  ត្រឡប់ map->len;
}
