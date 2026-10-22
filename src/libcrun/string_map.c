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

#నిర్వచించు GNU_ఆధారం

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <errno.h>
#అన్తర్భావ "string_map.h"
#అన్తర్భావ "utils.h"

#అన్తర్భావ <ocispec/runtime_spec_schema_config_schema.h>

నిర్మాణం kv_s
{
  అక్షరం *కీలకం;
  అక్షరం *విలువ;
};

నిర్మాణం string_map_s
{
  size_t len;
  నిర్మాణం kv_s *kvs;

#నిర్వచితమైతే HAVE_HSEARCH_R
  నిర్మాణం hsearch_దత్తాంశం htab;
  bool htab_initialized;
#అయితే_ముగింపు

  bool sorted;
};

స్థిర పూర్ణాంకం
compare_kv (స్థిరరాశి శూన్య *a, స్థిరరాశి శూన్య *b)
{
  తిరిగి strcmp (((నిర్మాణం kv_s *) a)->కీలకం, ((నిర్మాణం kv_s *) b)->కీలకం);
}

స్థిరరాశి అక్షరం *
కనుగొను_string_map_విలువ (string_map *map, స్థిరరాశి అక్షరం *name)

{
  నిర్మాణం kv_s *r, కీలకం;

  ఒకవేళ (map == ఖాళీ || map->len == 0)
    తిరిగి ఖాళీ;

#నిర్వచితమైతే HAVE_HSEARCH_R
  నమోదు_2 e, *ep;

  /* Do not bother with hash tables for small maps.  */
  ఒకవేళ (map->len < 8)
    వెళ్ళు fallback;

  ఒకవేళ (! map->htab_initialized)
    {
      size_t i;

      ఒకవేళ (hcreate_r (map->len, &map->htab) == 0)
        వెళ్ళు fallback;

      కొరకు (i = 0; i < map->len; i++)
        {
          e.కీలకం = (అక్షరం *) map->kvs[i].కీలకం;
          e.దత్తాంశం = map->kvs[i].విలువ;
          ఒకవేళ (hsearch_r (e, ENTER, &ep, &map->htab) == 0)
            {
              hdestroy_r (&map->htab);
              వెళ్ళు fallback;
            }
        }
      map->htab_initialized = true;
    }

  e.కీలకం = (అక్షరం *) name;
  ఒకవేళ (hsearch_r (e, కనుగొను, &ep, &map->htab) == 0)
    తిరిగి ఖాళీ;

  తిరిగి ep->దత్తాంశం;

fallback:
#అయితే_ముగింపు

  ఒకవేళ (! map->sorted)
    {
      qsort (map->kvs, map->len, పరిమాణం (నిర్మాణం kv_s), compare_kv);
      map->sorted = true;
    }

  కీలకం.కీలకం = (అక్షరం *) name;

  r = bsearch (&కీలకం, map->kvs, map->len, పరిమాణం (నిర్మాణం kv_s), compare_kv);
  తిరిగి r ? r->విలువ : ఖాళీ;
}

string_map *
make_string_map_from_json (json_map_string_string *jmap)
{
  నిర్మాణం string_map_s *new_map = xmalloc0 (పరిమాణం (నిర్మాణం string_map_s));
  size_t i;

  ఒకవేళ (jmap == ఖాళీ)
    తిరిగి new_map;

  new_map->len = jmap->len;
  new_map->kvs = xmalloc0 (పరిమాణం (నిర్మాణం kv_s) * (jmap->len + 1));

  కొరకు (i = 0; i < jmap->len; i++)
    {
      new_map->kvs[i].కీలకం = xstrdup (jmap->కీలకాలు[i]);
      new_map->kvs[i].విలువ = xstrdup (jmap->విలువలు[i]);
    }

  తిరిగి new_map;
}

పూర్ణాంకం
string_map_get_at (string_map *map, size_t సూచీ, స్థిరరాశి అక్షరం **name, స్థిరరాశి అక్షరం **విలువ)
{
  ఒకవేళ (map == ఖాళీ || సూచీ >= map->len)
    {
      errno = ERANGE;
      తిరిగి -1;
    }

  *name = map->kvs[సూచీ].కీలకం;
  *విలువ = map->kvs[సూచీ].విలువ;

  తిరిగి 0;
}

శూన్య
free_string_map (string_map *map)
{
  size_t i;

  ఒకవేళ (map == ఖాళీ)
    తిరిగి;

  ఒకవేళ (map->htab_initialized)
    hdestroy_r (&map->htab);

  కొరకు (i = 0; i < map->len; i++)
    {
      విడుదల (map->kvs[i].కీలకం);
      విడుదల (map->kvs[i].విలువ);
    }
  విడుదల (map->kvs);
  విడుదల (map);
}

size_t
string_map_కొలత (string_map *map)
{
  తిరిగి map->len;
}
