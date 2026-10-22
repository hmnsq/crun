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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ "string_map.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

#ಅನ್ತರ್ಭಾವ <ocispec/runtime_spec_schema_config_schema.h>

ಸಂರಚನಾ kv_s
{
  ವರ್ಣ *ಕೀಲಿ;
  ವರ್ಣ *ಮೌಲ್ಯ;
};

ಸಂರಚನಾ string_map_s
{
  size_t len;
  ಸಂರಚನಾ kv_s *kvs;

#ifdef HAVE_HSEARCH_R
  ಸಂರಚನಾ hsearch_data htab;
  bool htab_initialized;
#endif

  bool sorted;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
compare_kv (ಧ್ರುವ ಶೂನ್ಯ *a, ಧ್ರುವ ಶೂನ್ಯ *b)
{
  ಪ್ರತ್ಯಾಗಮ strcmp (((ಸಂರಚನಾ kv_s *) a)->ಕೀಲಿ, ((ಸಂರಚನಾ kv_s *) b)->ಕೀಲಿ);
}

ಧ್ರುವ ವರ್ಣ *
find_string_map_ಮೌಲ್ಯ (string_map *map, ಧ್ರುವ ವರ್ಣ *name)

{
  ಸಂರಚನಾ kv_s *r, ಕೀಲಿ;

  ಯದಿ (map == ರಿಕ್ತ || map->len == 0)
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

#ifdef HAVE_HSEARCH_R
  ENTRY e, *ep;

  /* Do not bother with hash tables for small maps.  */
  ಯದಿ (map->len < 8)
    ಗಚ್ಛ fallback;

  ಯದಿ (! map->htab_initialized)
    {
      size_t i;

      ಯದಿ (hcreate_r (map->len, &map->htab) == 0)
        ಗಚ್ಛ fallback;

      ಪ್ರತಿ (i = 0; i < map->len; i++)
        {
          e.ಕೀಲಿ = (ವರ್ಣ *) map->kvs[i].ಕೀಲಿ;
          e.data = map->kvs[i].ಮೌಲ್ಯ;
          ಯದಿ (hsearch_r (e, ENTER, &ep, &map->htab) == 0)
            {
              hdestroy_r (&map->htab);
              ಗಚ್ಛ fallback;
            }
        }
      map->htab_initialized = true;
    }

  e.ಕೀಲಿ = (ವರ್ಣ *) name;
  ಯದಿ (hsearch_r (e, FIND, &ep, &map->htab) == 0)
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

  ಪ್ರತ್ಯಾಗಮ ep->data;

fallback:
#endif

  ಯದಿ (! map->sorted)
    {
      qsort (map->kvs, map->len, ಪರಿಮಾಣ (ಸಂರಚನಾ kv_s), compare_kv);
      map->sorted = true;
    }

  ಕೀಲಿ.ಕೀಲಿ = (ವರ್ಣ *) name;

  r = bsearch (&ಕೀಲಿ, map->kvs, map->len, ಪರಿಮಾಣ (ಸಂರಚನಾ kv_s), compare_kv);
  ಪ್ರತ್ಯಾಗಮ r ? r->ಮೌಲ್ಯ : ರಿಕ್ತ;
}

string_map *
make_string_map_from_json (json_map_string_string *jmap)
{
  ಸಂರಚನಾ string_map_s *new_map = xmalloc0 (ಪರಿಮಾಣ (ಸಂರಚನಾ string_map_s));
  size_t i;

  ಯದಿ (jmap == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ new_map;

  new_map->len = jmap->len;
  new_map->kvs = xmalloc0 (ಪರಿಮಾಣ (ಸಂರಚನಾ kv_s) * (jmap->len + 1));

  ಪ್ರತಿ (i = 0; i < jmap->len; i++)
    {
      new_map->kvs[i].ಕೀಲಿ = xstrdup (jmap->ಕೀಲಿಗಳು[i]);
      new_map->kvs[i].ಮೌಲ್ಯ = xstrdup (jmap->ಮೌಲ್ಯಗಳು[i]);
    }

  ಪ್ರತ್ಯಾಗಮ new_map;
}

ಪೂರ್ಣಾಙ್ಕ
string_map_get_at (string_map *map, size_t index, ಧ್ರುವ ವರ್ಣ **name, ಧ್ರುವ ವರ್ಣ **ಮೌಲ್ಯ)
{
  ಯದಿ (map == ರಿಕ್ತ || index >= map->len)
    {
      errno = ERANGE;
      ಪ್ರತ್ಯಾಗಮ -1;
    }

  *name = map->kvs[index].ಕೀಲಿ;
  *ಮೌಲ್ಯ = map->kvs[index].ಮೌಲ್ಯ;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಶೂನ್ಯ
free_string_map (string_map *map)
{
  size_t i;

  ಯದಿ (map == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ;

  ಯದಿ (map->htab_initialized)
    hdestroy_r (&map->htab);

  ಪ್ರತಿ (i = 0; i < map->len; i++)
    {
      ಮುಕ್ತ (map->kvs[i].ಕೀಲಿ);
      ಮುಕ್ತ (map->kvs[i].ಮೌಲ್ಯ);
    }
  ಮುಕ್ತ (map->kvs);
  ಮುಕ್ತ (map);
}

size_t
string_map_size (string_map *map)
{
  ಪ್ರತ್ಯಾಗಮ map->len;
}
