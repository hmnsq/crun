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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ "string_map.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"

#ଅନ୍ତର୍ଭାଵ <ocispec/runtime_spec_schema_config_schema.h>

ସଂରଚନା kv_s
{
  ଅକ୍ଷର *ଚାବି;
  ଅକ୍ଷର *ମୂଲ୍ୟ;
};

ସଂରଚନା string_map_s
{
  size_t len;
  ସଂରଚନା kv_s *kvs;

#ifdef HAVE_HSEARCH_R
  ସଂରଚନା hsearch_data htab;
  bool htab_initialized;
#endif

  bool sorted;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
compare_kv (ଅଚଳ ଶୂନ୍ୟ *a, ଅଚଳ ଶୂନ୍ୟ *b)
{
  ଫେରନ୍ତୁ strcmp (((ସଂରଚନା kv_s *) a)->ଚାବି, ((ସଂରଚନା kv_s *) b)->ଚାବି);
}

ଅଚଳ ଅକ୍ଷର *
find_string_map_ମୂଲ୍ୟ (string_map *map, ଅଚଳ ଅକ୍ଷର *name)

{
  ସଂରଚନା kv_s *r, ଚାବି;

  ଯଦି (map == ଖାଲି || map->len == 0)
    ଫେରନ୍ତୁ ଖାଲି;

#ifdef HAVE_HSEARCH_R
  ENTRY e, *ep;

  /* Do not bother with hash tables for small maps.  */
  ଯଦି (map->len < 8)
    ଯାଅ fallback;

  ଯଦି (! map->htab_initialized)
    {
      size_t i;

      ଯଦି (hcreate_r (map->len, &map->htab) == 0)
        ଯାଅ fallback;

      ପାଇଁ (i = 0; i < map->len; i++)
        {
          e.ଚାବି = (ଅକ୍ଷର *) map->kvs[i].ଚାବି;
          e.data = map->kvs[i].ମୂଲ୍ୟ;
          ଯଦି (hsearch_r (e, ENTER, &ep, &map->htab) == 0)
            {
              hdestroy_r (&map->htab);
              ଯାଅ fallback;
            }
        }
      map->htab_initialized = true;
    }

  e.ଚାବି = (ଅକ୍ଷର *) name;
  ଯଦି (hsearch_r (e, FIND, &ep, &map->htab) == 0)
    ଫେରନ୍ତୁ ଖାଲି;

  ଫେରନ୍ତୁ ep->data;

fallback:
#endif

  ଯଦି (! map->sorted)
    {
      qsort (map->kvs, map->len, ଆକାର (ସଂରଚନା kv_s), compare_kv);
      map->sorted = true;
    }

  ଚାବି.ଚାବି = (ଅକ୍ଷର *) name;

  r = bsearch (&ଚାବି, map->kvs, map->len, ଆକାର (ସଂରଚନା kv_s), compare_kv);
  ଫେରନ୍ତୁ r ? r->ମୂଲ୍ୟ : ଖାଲି;
}

string_map *
make_string_map_from_json (json_map_string_string *jmap)
{
  ସଂରଚନା string_map_s *new_map = xmalloc0 (ଆକାର (ସଂରଚନା string_map_s));
  size_t i;

  ଯଦି (jmap == ଖାଲି)
    ଫେରନ୍ତୁ new_map;

  new_map->len = jmap->len;
  new_map->kvs = xmalloc0 (ଆକାର (ସଂରଚନା kv_s) * (jmap->len + 1));

  ପାଇଁ (i = 0; i < jmap->len; i++)
    {
      new_map->kvs[i].ଚାବି = xstrdup (jmap->ଚାବିଗୁଡ଼ିକ[i]);
      new_map->kvs[i].ମୂଲ୍ୟ = xstrdup (jmap->ମୂଲ୍ୟଗୁଡ଼ିକ[i]);
    }

  ଫେରନ୍ତୁ new_map;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
string_map_get_at (string_map *map, size_t index, ଅଚଳ ଅକ୍ଷର **name, ଅଚଳ ଅକ୍ଷର **ମୂଲ୍ୟ)
{
  ଯଦି (map == ଖାଲି || index >= map->len)
    {
      errno = ERANGE;
      ଫେରନ୍ତୁ -1;
    }

  *name = map->kvs[index].ଚାବି;
  *ମୂଲ୍ୟ = map->kvs[index].ମୂଲ୍ୟ;

  ଫେରନ୍ତୁ 0;
}

ଶୂନ୍ୟ
free_string_map (string_map *map)
{
  size_t i;

  ଯଦି (map == ଖାଲି)
    ଫେରନ୍ତୁ;

  ଯଦି (map->htab_initialized)
    hdestroy_r (&map->htab);

  ପାଇଁ (i = 0; i < map->len; i++)
    {
      ମୁକ୍ତ (map->kvs[i].ଚାବି);
      ମୁକ୍ତ (map->kvs[i].ମୂଲ୍ୟ);
    }
  ମୁକ୍ତ (map->kvs);
  ମୁକ୍ତ (map);
}

size_t
string_map_size (string_map *map)
{
  ଫେରନ୍ତୁ map->len;
}
