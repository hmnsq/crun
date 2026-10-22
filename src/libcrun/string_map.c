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

#घोषणा _GNU_SOURCE

#समावेश <config.h>
#समावेश <stdlib.h>
#समावेश <errno.h>
#समावेश "string_map.h"
#समावेश "utils.h"

#समावेश <ocispec/runtime_spec_schema_config_schema.h>

काष्ठा kv_s
{
  अक्षर *कुंजी;
  अक्षर *मूल्य;
};

काष्ठा string_map_s
{
  माप_प्रकार len;
  काष्ठा kv_s *kvs;

#अगर_घोषित HAVE_HSEARCH_R
  काष्ठा hsearch_तथ्य htab;
  bool htab_initialized;
#पूर्ण_अगर

  bool sorted;
};

अटल पूर्णांक
compare_kv (स्थिर व्योम *a, स्थिर व्योम *b)
{
  वापस म_भेद (((काष्ठा kv_s *) a)->कुंजी, ((काष्ठा kv_s *) b)->कुंजी);
}

स्थिर अक्षर *
find_string_map_मूल्य (string_map *map, स्थिर अक्षर *name)

{
  काष्ठा kv_s *r, कुंजी;

  अगर (map == शून्य || map->len == 0)
    वापस शून्य;

#अगर_घोषित HAVE_HSEARCH_R
  ENTRY e, *ep;

  /* Do not bother with hash tables for small maps.  */
  अगर (map->len < 8)
    जाओ fallback;

  अगर (! map->htab_initialized)
    {
      माप_प्रकार i;

      अगर (hcreate_r (map->len, &map->htab) == 0)
        जाओ fallback;

      क्रम (i = 0; i < map->len; i++)
        {
          e.कुंजी = (अक्षर *) map->kvs[i].कुंजी;
          e.तथ्य = map->kvs[i].मूल्य;
          अगर (hsearch_r (e, ENTER, &ep, &map->htab) == 0)
            {
              hdestroy_r (&map->htab);
              जाओ fallback;
            }
        }
      map->htab_initialized = true;
    }

  e.कुंजी = (अक्षर *) name;
  अगर (hsearch_r (e, FIND, &ep, &map->htab) == 0)
    वापस शून्य;

  वापस ep->तथ्य;

fallback:
#पूर्ण_अगर

  अगर (! map->sorted)
    {
      क्विक (map->kvs, map->len, माप (काष्ठा kv_s), compare_kv);
      map->sorted = true;
    }

  कुंजी.कुंजी = (अक्षर *) name;

  r = द्वा_खोज (&कुंजी, map->kvs, map->len, माप (काष्ठा kv_s), compare_kv);
  वापस r ? r->मूल्य : शून्य;
}

string_map *
make_string_map_from_json (json_map_string_string *jmap)
{
  काष्ठा string_map_s *new_map = xmalloc0 (माप (काष्ठा string_map_s));
  माप_प्रकार i;

  अगर (jmap == शून्य)
    वापस new_map;

  new_map->len = jmap->len;
  new_map->kvs = xmalloc0 (माप (काष्ठा kv_s) * (jmap->len + 1));

  क्रम (i = 0; i < jmap->len; i++)
    {
      new_map->kvs[i].कुंजी = xstrdup (jmap->कुंजियाँ[i]);
      new_map->kvs[i].मूल्य = xstrdup (jmap->मूल्यावली[i]);
    }

  वापस new_map;
}

पूर्णांक
string_map_get_at (string_map *map, माप_प्रकार सूचकांक, स्थिर अक्षर **name, स्थिर अक्षर **मूल्य)
{
  अगर (map == शून्य || सूचकांक >= map->len)
    {
      त्रुटि_सं = दुस्फल;
      वापस -1;
    }

  *name = map->kvs[सूचकांक].कुंजी;
  *मूल्य = map->kvs[सूचकांक].मूल्य;

  वापस 0;
}

व्योम
free_string_map (string_map *map)
{
  माप_प्रकार i;

  अगर (map == शून्य)
    वापस;

  अगर (map->htab_initialized)
    hdestroy_r (&map->htab);

  क्रम (i = 0; i < map->len; i++)
    {
      मुक्त (map->kvs[i].कुंजी);
      मुक्त (map->kvs[i].मूल्य);
    }
  मुक्त (map->kvs);
  मुक्त (map);
}

माप_प्रकार
string_map_आकार (string_map *map)
{
  वापस map->len;
}
