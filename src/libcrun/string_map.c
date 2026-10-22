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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ "string_map.h"
#അന്തര്ഭാവ "utils.h"

#അന്തര്ഭാവ <ocispec/runtime_spec_schema_config_schema.h>

ഘടന kv_s
{
  അക്ഷരം *താക്കോൽ;
  അക്ഷരം *മൂല്യം;
};

ഘടന string_map_s
{
  size_t len;
  ഘടന kv_s *kvs;

#ifdef HAVE_HSEARCH_R
  ഘടന hsearch_data htab;
  bool htab_initialized;
#endif

  bool sorted;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ
compare_kv (സ്ഥിരം ശൂന്യം *a, സ്ഥിരം ശൂന്യം *b)
{
  തിരികെ strcmp (((ഘടന kv_s *) a)->താക്കോൽ, ((ഘടന kv_s *) b)->താക്കോൽ);
}

സ്ഥിരം അക്ഷരം *
find_string_map_മൂല്യം (string_map *map, സ്ഥിരം അക്ഷരം *name)

{
  ഘടന kv_s *r, താക്കോൽ;

  എങ്കിൽ (map == ശൂന്യ || map->len == 0)
    തിരികെ ശൂന്യ;

#ifdef HAVE_HSEARCH_R
  ENTRY e, *ep;

  /* Do not bother with hash tables for small maps.  */
  എങ്കിൽ (map->len < 8)
    പോകുക fallback;

  എങ്കിൽ (! map->htab_initialized)
    {
      size_t i;

      എങ്കിൽ (hcreate_r (map->len, &map->htab) == 0)
        പോകുക fallback;

      വേണ്ടി (i = 0; i < map->len; i++)
        {
          e.താക്കോൽ = (അക്ഷരം *) map->kvs[i].താക്കോൽ;
          e.data = map->kvs[i].മൂല്യം;
          എങ്കിൽ (hsearch_r (e, ENTER, &ep, &map->htab) == 0)
            {
              hdestroy_r (&map->htab);
              പോകുക fallback;
            }
        }
      map->htab_initialized = true;
    }

  e.താക്കോൽ = (അക്ഷരം *) name;
  എങ്കിൽ (hsearch_r (e, FIND, &ep, &map->htab) == 0)
    തിരികെ ശൂന്യ;

  തിരികെ ep->data;

fallback:
#endif

  എങ്കിൽ (! map->sorted)
    {
      qsort (map->kvs, map->len, വലുപ്പം (ഘടന kv_s), compare_kv);
      map->sorted = true;
    }

  താക്കോൽ.താക്കോൽ = (അക്ഷരം *) name;

  r = bsearch (&താക്കോൽ, map->kvs, map->len, വലുപ്പം (ഘടന kv_s), compare_kv);
  തിരികെ r ? r->മൂല്യം : ശൂന്യ;
}

string_map *
make_string_map_from_json (json_map_string_string *jmap)
{
  ഘടന string_map_s *new_map = xmalloc0 (വലുപ്പം (ഘടന string_map_s));
  size_t i;

  എങ്കിൽ (jmap == ശൂന്യ)
    തിരികെ new_map;

  new_map->len = jmap->len;
  new_map->kvs = xmalloc0 (വലുപ്പം (ഘടന kv_s) * (jmap->len + 1));

  വേണ്ടി (i = 0; i < jmap->len; i++)
    {
      new_map->kvs[i].താക്കോൽ = xstrdup (jmap->താക്കോലുകൾ[i]);
      new_map->kvs[i].മൂല്യം = xstrdup (jmap->മൂല്യങ്ങൾ[i]);
    }

  തിരികെ new_map;
}

പൂർണ്ണസംഖ്യ
string_map_get_at (string_map *map, size_t index, സ്ഥിരം അക്ഷരം **name, സ്ഥിരം അക്ഷരം **മൂല്യം)
{
  എങ്കിൽ (map == ശൂന്യ || index >= map->len)
    {
      errno = ERANGE;
      തിരികെ -1;
    }

  *name = map->kvs[index].താക്കോൽ;
  *മൂല്യം = map->kvs[index].മൂല്യം;

  തിരികെ 0;
}

ശൂന്യം
free_string_map (string_map *map)
{
  size_t i;

  എങ്കിൽ (map == ശൂന്യ)
    തിരികെ;

  എങ്കിൽ (map->htab_initialized)
    hdestroy_r (&map->htab);

  വേണ്ടി (i = 0; i < map->len; i++)
    {
      സ്വതന്ത്രം (map->kvs[i].താക്കോൽ);
      സ്വതന്ത്രം (map->kvs[i].മൂല്യം);
    }
  സ്വതന്ത്രം (map->kvs);
  സ്വതന്ത്രം (map);
}

size_t
string_map_size (string_map *map)
{
  തിരികെ map->len;
}
