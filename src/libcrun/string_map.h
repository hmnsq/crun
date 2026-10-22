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

#اگر_تعریف_نشده STRING_MAP_H
#تعریف_کن STRING_MAP_H

#اگر_تعریف_نشده GNU_منبع
#  define GNU_منبع
#پایان_اگر

#ضمن <config.h>

#اگر_تعریف_شده HAVE_HSEARCH_R
#  include <search.h>
#پایان_اگر

#ضمن <ocispec/runtime_spec_schema_config_schema.h>
#ضمن "error.h"

بنية string_map_s;
تعريفنوع بنية string_map_s string_map;

ثابت حرف *پیدا_کن_string_map_مقدار (string_map *map, ثابت حرف *name);

string_map *make_string_map_from_json (json_map_string_string *jmap);

فراغ free_string_map (string_map *map);

صحيح string_map_get_at (string_map *map, size_t شاخص, ثابت حرف **name, ثابت حرف **مقدار);

size_t string_map_اندازه (string_map *map);

#پایان_اگر
