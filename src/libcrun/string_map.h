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

#ifndef STRING_MAP_H
#define STRING_MAP_H

#ifndef _GNU_SOURCE
#  define _GNU_SOURCE
#endif

#अन्तर्भाव <config.h>

#ifdef HAVE_HSEARCH_R
#  include <search.h>
#endif

#अन्तर्भाव <ocispec/runtime_spec_schema_config_schema.h>
#अन्तर्भाव "error.h"

संरचना string_map_s;
प्रकारव्याख्या संरचना string_map_s string_map;

स्थिरांक अक्षर *find_string_map_मूल्य (string_map *map, स्थिरांक अक्षर *name);

string_map *make_string_map_from_json (json_map_string_string *jmap);

शून्य free_string_map (string_map *map);

पूर्णांक string_map_get_at (string_map *map, माप_प्रकार index, स्थिरांक अक्षर **name, स्थिरांक अक्षर **मूल्य);

माप_प्रकार string_map_size (string_map *map);

#endif
