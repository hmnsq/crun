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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ "string_map.h"
#அந்தர்பாவ "utils.h"

#அந்தர்பாவ <ocispec/runtime_spec_schema_config_schema.h>

கட்டமைப்பு kv_s
{
  எழுத்து *சாவி;
  எழுத்து *மதிப்பு;
};

கட்டமைப்பு string_map_s
{
  size_t len;
  கட்டமைப்பு kv_s *kvs;

#ifdef HAVE_HSEARCH_R
  கட்டமைப்பு hsearch_data htab;
  bool htab_initialized;
#endif

  bool sorted;
};

நிலையான முழு
compare_kv (மாறிலி வெறுமை *a, மாறிலி வெறுமை *b)
{
  திரும்பு strcmp (((கட்டமைப்பு kv_s *) a)->சாவி, ((கட்டமைப்பு kv_s *) b)->சாவி);
}

மாறிலி எழுத்து *
find_string_map_மதிப்பு (string_map *map, மாறிலி எழுத்து *name)

{
  கட்டமைப்பு kv_s *r, சாவி;

  எனில் (map == வெற்று || map->len == 0)
    திரும்பு வெற்று;

#ifdef HAVE_HSEARCH_R
  ENTRY e, *ep;

  /* Do not bother with hash tables for small maps.  */
  எனில் (map->len < 8)
    செல் fallback;

  எனில் (! map->htab_initialized)
    {
      size_t i;

      எனில் (hcreate_r (map->len, &map->htab) == 0)
        செல் fallback;

      க்கு (i = 0; i < map->len; i++)
        {
          e.சாவி = (எழுத்து *) map->kvs[i].சாவி;
          e.data = map->kvs[i].மதிப்பு;
          எனில் (hsearch_r (e, ENTER, &ep, &map->htab) == 0)
            {
              hdestroy_r (&map->htab);
              செல் fallback;
            }
        }
      map->htab_initialized = true;
    }

  e.சாவி = (எழுத்து *) name;
  எனில் (hsearch_r (e, FIND, &ep, &map->htab) == 0)
    திரும்பு வெற்று;

  திரும்பு ep->data;

fallback:
#endif

  எனில் (! map->sorted)
    {
      qsort (map->kvs, map->len, அளவு (கட்டமைப்பு kv_s), compare_kv);
      map->sorted = true;
    }

  சாவி.சாவி = (எழுத்து *) name;

  r = bsearch (&சாவி, map->kvs, map->len, அளவு (கட்டமைப்பு kv_s), compare_kv);
  திரும்பு r ? r->மதிப்பு : வெற்று;
}

string_map *
make_string_map_from_json (json_map_string_string *jmap)
{
  கட்டமைப்பு string_map_s *new_map = xmalloc0 (அளவு (கட்டமைப்பு string_map_s));
  size_t i;

  எனில் (jmap == வெற்று)
    திரும்பு new_map;

  new_map->len = jmap->len;
  new_map->kvs = xmalloc0 (அளவு (கட்டமைப்பு kv_s) * (jmap->len + 1));

  க்கு (i = 0; i < jmap->len; i++)
    {
      new_map->kvs[i].சாவி = xstrdup (jmap->சாவிகள்[i]);
      new_map->kvs[i].மதிப்பு = xstrdup (jmap->மதிப்புகள்[i]);
    }

  திரும்பு new_map;
}

முழு
string_map_get_at (string_map *map, size_t index, மாறிலி எழுத்து **name, மாறிலி எழுத்து **மதிப்பு)
{
  எனில் (map == வெற்று || index >= map->len)
    {
      errno = ERANGE;
      திரும்பு -1;
    }

  *name = map->kvs[index].சாவி;
  *மதிப்பு = map->kvs[index].மதிப்பு;

  திரும்பு 0;
}

வெறுமை
free_string_map (string_map *map)
{
  size_t i;

  எனில் (map == வெற்று)
    திரும்பு;

  எனில் (map->htab_initialized)
    hdestroy_r (&map->htab);

  க்கு (i = 0; i < map->len; i++)
    {
      விடுவி (map->kvs[i].சாவி);
      விடுவி (map->kvs[i].மதிப்பு);
    }
  விடுவி (map->kvs);
  விடுவி (map);
}

size_t
string_map_size (string_map *map)
{
  திரும்பு map->len;
}
