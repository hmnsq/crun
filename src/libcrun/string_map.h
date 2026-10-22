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

#यदि_अपरिभाषितम् STRING_MAP_H
#परिभाषय STRING_MAP_H

#यदि_अपरिभाषितम् _GNU_SOURCE
#  define _GNU_SOURCE
#यदि_अन्तः

#अन्तर्भाव <config.h>

#यदि_परिभाषितम् HAVE_HSEARCH_R
#  include <search.h>
#यदि_अन्तः

#अन्तर्भाव <ocispec/runtime_spec_schema_config_schema.h>
#अन्तर्भाव "error.h"

संरचना string_map_s;
प्रकारनिर्देश संरचना string_map_s string_map;

ध्रुव वर्ण *find_string_map_मानम् (string_map *map, ध्रुव वर्ण *name);

string_map *make_string_map_from_json (json_map_string_string *jmap);

शून्य free_string_map (string_map *map);

पूर्णाङ्क string_map_get_at (string_map *map, size_t सूचकांक, ध्रुव वर्ण **name, ध्रुव वर्ण **मानम्);

size_t string_map_आकार (string_map *map);

#यदि_अन्तः
