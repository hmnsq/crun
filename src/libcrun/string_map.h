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

#اگر_غیر_تعریف_شدہ STRING_MAP_H
#تعریف_کرو STRING_MAP_H

#اگر_غیر_تعریف_شدہ GNU_ماخذ
#  define GNU_ماخذ
#اگر_ختم

#شامل <config.h>

#اگر_تعریف_شدہ HAVE_HSEARCH_R
#  include <search.h>
#اگر_ختم

#شامل <ocispec/runtime_spec_schema_config_schema.h>
#شامل "error.h"

ساخت string_map_s;
قسمتعریف ساخت string_map_s string_map;

ثابت حرف *ڈھونڈو_string_map_قدر (string_map *map, ثابت حرف *name);

string_map *make_string_map_from_json (json_map_string_string *jmap);

خالی free_string_map (string_map *map);

صحیح string_map_get_at (string_map *map, size_t اشاریہ, ثابت حرف **name, ثابت حرف **قدر);

size_t string_map_سائز (string_map *map);

#اگر_ختم
