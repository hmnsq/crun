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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ "string_map.h"
#અન્તર્ભાવ "utils.h"

#અન્તર્ભાવ <ocispec/runtime_spec_schema_config_schema.h>

સંરચના kv_s
{
  અક્ષર *ચાવી;
  અક્ષર *કિંમત;
};

સંરચના string_map_s
{
  size_t len;
  સંરચના kv_s *kvs;

#ifdef HAVE_HSEARCH_R
  સંરચના hsearch_data htab;
  bool htab_initialized;
#endif

  bool sorted;
};

સ્થિર પૂર્ણાંક
compare_kv (અચલ શૂન્ય *a, અચલ શૂન્ય *b)
{
  પાછા strcmp (((સંરચના kv_s *) a)->ચાવી, ((સંરચના kv_s *) b)->ચાવી);
}

અચલ અક્ષર *
find_string_map_કિંમત (string_map *map, અચલ અક્ષર *name)

{
  સંરચના kv_s *r, ચાવી;

  જો (map == ખાલી || map->len == 0)
    પાછા ખાલી;

#ifdef HAVE_HSEARCH_R
  ENTRY e, *ep;

  /* Do not bother with hash tables for small maps.  */
  જો (map->len < 8)
    જાઓ fallback;

  જો (! map->htab_initialized)
    {
      size_t i;

      જો (hcreate_r (map->len, &map->htab) == 0)
        જાઓ fallback;

      માટે (i = 0; i < map->len; i++)
        {
          e.ચાવી = (અક્ષર *) map->kvs[i].ચાવી;
          e.data = map->kvs[i].કિંમત;
          જો (hsearch_r (e, ENTER, &ep, &map->htab) == 0)
            {
              hdestroy_r (&map->htab);
              જાઓ fallback;
            }
        }
      map->htab_initialized = true;
    }

  e.ચાવી = (અક્ષર *) name;
  જો (hsearch_r (e, FIND, &ep, &map->htab) == 0)
    પાછા ખાલી;

  પાછા ep->data;

fallback:
#endif

  જો (! map->sorted)
    {
      qsort (map->kvs, map->len, કદ (સંરચના kv_s), compare_kv);
      map->sorted = true;
    }

  ચાવી.ચાવી = (અક્ષર *) name;

  r = bsearch (&ચાવી, map->kvs, map->len, કદ (સંરચના kv_s), compare_kv);
  પાછા r ? r->કિંમત : ખાલી;
}

string_map *
make_string_map_from_json (json_map_string_string *jmap)
{
  સંરચના string_map_s *new_map = xmalloc0 (કદ (સંરચના string_map_s));
  size_t i;

  જો (jmap == ખાલી)
    પાછા new_map;

  new_map->len = jmap->len;
  new_map->kvs = xmalloc0 (કદ (સંરચના kv_s) * (jmap->len + 1));

  માટે (i = 0; i < jmap->len; i++)
    {
      new_map->kvs[i].ચાવી = xstrdup (jmap->ચાવીઓ[i]);
      new_map->kvs[i].કિંમત = xstrdup (jmap->કિંમતો[i]);
    }

  પાછા new_map;
}

પૂર્ણાંક
string_map_get_at (string_map *map, size_t index, અચલ અક્ષર **name, અચલ અક્ષર **કિંમત)
{
  જો (map == ખાલી || index >= map->len)
    {
      errno = ERANGE;
      પાછા -1;
    }

  *name = map->kvs[index].ચાવી;
  *કિંમત = map->kvs[index].કિંમત;

  પાછા 0;
}

શૂન્ય
free_string_map (string_map *map)
{
  size_t i;

  જો (map == ખાલી)
    પાછા;

  જો (map->htab_initialized)
    hdestroy_r (&map->htab);

  માટે (i = 0; i < map->len; i++)
    {
      મુક્ત (map->kvs[i].ચાવી);
      મુક્ત (map->kvs[i].કિંમત);
    }
  મુક્ત (map->kvs);
  મુક્ત (map);
}

size_t
string_map_size (string_map *map)
{
  પાછા map->len;
}
