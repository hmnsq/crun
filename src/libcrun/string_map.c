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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "string_map.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <ocispec/runtime_spec_schema_config_schema.h>

ਬਣਤਰ kv_s
{
  ਅੱਖਰ *ਕੁੰਜੀ;
  ਅੱਖਰ *ਮੁੱਲ;
};

ਬਣਤਰ string_map_s
{
  size_t len;
  ਬਣਤਰ kv_s *kvs;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_HSEARCH_R
  ਬਣਤਰ hsearch_ਅੰਕੜੇ htab;
  bool htab_initialized;
#ਜੇ_ਅੰਤ

  bool sorted;
};

ਸਥਿਰ ਪੂਰਨ
compare_kv (ਅਚਲ ਖਾਲੀ *a, ਅਚਲ ਖਾਲੀ *b)
{
  ਵਾਪਸ strcmp (((ਬਣਤਰ kv_s *) a)->ਕੁੰਜੀ, ((ਬਣਤਰ kv_s *) b)->ਕੁੰਜੀ);
}

ਅਚਲ ਅੱਖਰ *
ਲੱਭੋ_string_map_ਮੁੱਲ (string_map *map, ਅਚਲ ਅੱਖਰ *name)

{
  ਬਣਤਰ kv_s *r, ਕੁੰਜੀ;

  ਜੇ (map == ਨਿਰਾ || map->len == 0)
    ਵਾਪਸ ਨਿਰਾ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_HSEARCH_R
  ਇੰਦਰਾਜ e, *ep;

  /* Do not bother with hash tables for small maps.  */
  ਜੇ (map->len < 8)
    ਜਾਓ fallback;

  ਜੇ (! map->htab_initialized)
    {
      size_t i;

      ਜੇ (hcreate_r (map->len, &map->htab) == 0)
        ਜਾਓ fallback;

      ਲਈ (i = 0; i < map->len; i++)
        {
          e.ਕੁੰਜੀ = (ਅੱਖਰ *) map->kvs[i].ਕੁੰਜੀ;
          e.ਅੰਕੜੇ = map->kvs[i].ਮੁੱਲ;
          ਜੇ (hsearch_r (e, ENTER, &ep, &map->htab) == 0)
            {
              hdestroy_r (&map->htab);
              ਜਾਓ fallback;
            }
        }
      map->htab_initialized = true;
    }

  e.ਕੁੰਜੀ = (ਅੱਖਰ *) name;
  ਜੇ (hsearch_r (e, ਲੱਭੋ, &ep, &map->htab) == 0)
    ਵਾਪਸ ਨਿਰਾ;

  ਵਾਪਸ ep->ਅੰਕੜੇ;

fallback:
#ਜੇ_ਅੰਤ

  ਜੇ (! map->sorted)
    {
      qsort (map->kvs, map->len, ਆਕਾਰ (ਬਣਤਰ kv_s), compare_kv);
      map->sorted = true;
    }

  ਕੁੰਜੀ.ਕੁੰਜੀ = (ਅੱਖਰ *) name;

  r = bsearch (&ਕੁੰਜੀ, map->kvs, map->len, ਆਕਾਰ (ਬਣਤਰ kv_s), compare_kv);
  ਵਾਪਸ r ? r->ਮੁੱਲ : ਨਿਰਾ;
}

string_map *
make_string_map_from_json (json_map_string_string *jmap)
{
  ਬਣਤਰ string_map_s *new_map = xmalloc0 (ਆਕਾਰ (ਬਣਤਰ string_map_s));
  size_t i;

  ਜੇ (jmap == ਨਿਰਾ)
    ਵਾਪਸ new_map;

  new_map->len = jmap->len;
  new_map->kvs = xmalloc0 (ਆਕਾਰ (ਬਣਤਰ kv_s) * (jmap->len + 1));

  ਲਈ (i = 0; i < jmap->len; i++)
    {
      new_map->kvs[i].ਕੁੰਜੀ = xstrdup (jmap->ਕੁੰਜੀਆਂ[i]);
      new_map->kvs[i].ਮੁੱਲ = xstrdup (jmap->ਮੁੱਲਾਂ[i]);
    }

  ਵਾਪਸ new_map;
}

ਪੂਰਨ
string_map_get_at (string_map *map, size_t ਸੂਚਕਾਂਕ, ਅਚਲ ਅੱਖਰ **name, ਅਚਲ ਅੱਖਰ **ਮੁੱਲ)
{
  ਜੇ (map == ਨਿਰਾ || ਸੂਚਕਾਂਕ >= map->len)
    {
      errno = ERANGE;
      ਵਾਪਸ -1;
    }

  *name = map->kvs[ਸੂਚਕਾਂਕ].ਕੁੰਜੀ;
  *ਮੁੱਲ = map->kvs[ਸੂਚਕਾਂਕ].ਮੁੱਲ;

  ਵਾਪਸ 0;
}

ਖਾਲੀ
free_string_map (string_map *map)
{
  size_t i;

  ਜੇ (map == ਨਿਰਾ)
    ਵਾਪਸ;

  ਜੇ (map->htab_initialized)
    hdestroy_r (&map->htab);

  ਲਈ (i = 0; i < map->len; i++)
    {
      ਮੁਕਤ (map->kvs[i].ਕੁੰਜੀ);
      ਮੁਕਤ (map->kvs[i].ਮੁੱਲ);
    }
  ਮੁਕਤ (map->kvs);
  ਮੁਕਤ (map);
}

size_t
string_map_ਮਾਪ (string_map *map)
{
  ਵਾਪਸ map->len;
}
