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

#جيڪڏهن_وصف_نه_ٿيل STRING_MAP_H
#وصف_ڪريو STRING_MAP_H

#جيڪڏهن_وصف_نه_ٿيل GNU_ذريعو
#  define GNU_ذريعو
#جيڪڏهن_پورو

#ضمن <config.h>

#جيڪڏهن_وصف_ٿيل HAVE_HSEARCH_R
#  include <search.h>
#جيڪڏهن_پورو

#ضمن <ocispec/runtime_spec_schema_config_schema.h>
#ضمن "error.h"

بنية string_map_s;
تعريفنوع بنية string_map_s string_map;

ثابت حرف *ڳوليو_string_map_قدر (string_map *map, ثابت حرف *name);

string_map *make_string_map_from_json (json_map_string_string *jmap);

فراغ free_string_map (string_map *map);

صحيح string_map_get_at (string_map *map, size_t اشاريو, ثابت حرف **name, ثابت حرف **قدر);

size_t string_map_سائيز (string_map *map);

#جيڪڏهن_پورو
