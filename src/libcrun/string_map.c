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

#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ "string_map.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

#ಅನ್ತರ್ಭಾವ <ocispec/runtime_spec_schema_config_schema.h>

ರಚನೆ kv_s
{
  ಅಕ್ಷರ *ಕೀಲಿ;
  ಅಕ್ಷರ *ಮೌಲ್ಯ;
};

ರಚನೆ string_map_s
{
  size_t len;
  ರಚನೆ kv_s *kvs;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_HSEARCH_R
  ರಚನೆ hsearch_ದತ್ತಾಂಶ htab;
  bool htab_initialized;
#ಆದರೆ_ಅಂತ್ಯ

  bool sorted;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
compare_kv (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *b)
{
  ಹಿಂತಿರುಗಿ strcmp (((ರಚನೆ kv_s *) a)->ಕೀಲಿ, ((ರಚನೆ kv_s *) b)->ಕೀಲಿ);
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
ಕಂಡುಹಿಡಿ_string_map_ಮೌಲ್ಯ (string_map *map, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)

{
  ರಚನೆ kv_s *r, ಕೀಲಿ;

  ಆದರೆ (map == ಖಾಲಿ || map->len == 0)
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_HSEARCH_R
  ನಮೂದು e, *ep;

  /* Do not bother with hash tables for small maps.  */
  ಆದರೆ (map->len < 8)
    ಹೋಗು fallback;

  ಆದರೆ (! map->htab_initialized)
    {
      size_t i;

      ಆದರೆ (hcreate_r (map->len, &map->htab) == 0)
        ಹೋಗು fallback;

      ಗಾಗಿ (i = 0; i < map->len; i++)
        {
          e.ಕೀಲಿ = (ಅಕ್ಷರ *) map->kvs[i].ಕೀಲಿ;
          e.ದತ್ತಾಂಶ = map->kvs[i].ಮೌಲ್ಯ;
          ಆದರೆ (hsearch_r (e, ENTER, &ep, &map->htab) == 0)
            {
              hdestroy_r (&map->htab);
              ಹೋಗು fallback;
            }
        }
      map->htab_initialized = true;
    }

  e.ಕೀಲಿ = (ಅಕ್ಷರ *) name;
  ಆದರೆ (hsearch_r (e, ಕಂಡುಹಿಡಿ, &ep, &map->htab) == 0)
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

  ಹಿಂತಿರುಗಿ ep->ದತ್ತಾಂಶ;

fallback:
#ಆದರೆ_ಅಂತ್ಯ

  ಆದರೆ (! map->sorted)
    {
      qsort (map->kvs, map->len, ಗಾತ್ರ (ರಚನೆ kv_s), compare_kv);
      map->sorted = true;
    }

  ಕೀಲಿ.ಕೀಲಿ = (ಅಕ್ಷರ *) name;

  r = bsearch (&ಕೀಲಿ, map->kvs, map->len, ಗಾತ್ರ (ರಚನೆ kv_s), compare_kv);
  ಹಿಂತಿರುಗಿ r ? r->ಮೌಲ್ಯ : ಖಾಲಿ;
}

string_map *
make_string_map_from_json (json_map_string_string *jmap)
{
  ರಚನೆ string_map_s *new_map = xmalloc0 (ಗಾತ್ರ (ರಚನೆ string_map_s));
  size_t i;

  ಆದರೆ (jmap == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ new_map;

  new_map->len = jmap->len;
  new_map->kvs = xmalloc0 (ಗಾತ್ರ (ರಚನೆ kv_s) * (jmap->len + 1));

  ಗಾಗಿ (i = 0; i < jmap->len; i++)
    {
      new_map->kvs[i].ಕೀಲಿ = xstrdup (jmap->ಕೀಲಿಗಳು[i]);
      new_map->kvs[i].ಮೌಲ್ಯ = xstrdup (jmap->ಮೌಲ್ಯಗಳು[i]);
    }

  ಹಿಂತಿರುಗಿ new_map;
}

ಪೂರ್ಣಾಂಕ
string_map_get_at (string_map *map, size_t ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಮೌಲ್ಯ)
{
  ಆದರೆ (map == ಖಾಲಿ || ಸೂಚ್ಯಂಕ >= map->len)
    {
      errno = ERANGE;
      ಹಿಂತಿರುಗಿ -1;
    }

  *name = map->kvs[ಸೂಚ್ಯಂಕ].ಕೀಲಿ;
  *ಮೌಲ್ಯ = map->kvs[ಸೂಚ್ಯಂಕ].ಮೌಲ್ಯ;

  ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ
free_string_map (string_map *map)
{
  size_t i;

  ಆದರೆ (map == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;

  ಆದರೆ (map->htab_initialized)
    hdestroy_r (&map->htab);

  ಗಾಗಿ (i = 0; i < map->len; i++)
    {
      ಬಿಡುಗಡೆ (map->kvs[i].ಕೀಲಿ);
      ಬಿಡುಗಡೆ (map->kvs[i].ಮೌಲ್ಯ);
    }
  ಬಿಡುಗಡೆ (map->kvs);
  ಬಿಡುಗಡೆ (map);
}

size_t
string_map_ಗಾತ್ರ (string_map *map)
{
  ಹಿಂತಿರುಗಿ map->len;
}
