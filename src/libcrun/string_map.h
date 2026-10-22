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

#جے_بنا_تعریف STRING_MAP_H
#تعریف_کرو STRING_MAP_H

#جے_بنا_تعریف GNU_سروت
#  define GNU_سروت
#جے_انت

#ضمن <config.h>

#جے_تعریف_شدہ HAVE_HSEARCH_R
#  include <search.h>
#جے_انت

#ضمن <ocispec/runtime_spec_schema_config_schema.h>
#ضمن "error.h"

بنية string_map_s;
تعريفنوع بنية string_map_s string_map;

ثابت حرف *لبھو_string_map_قدر (string_map *map, ثابت حرف *name);

string_map *make_string_map_from_json (json_map_string_string *jmap);

فراغ free_string_map (string_map *map);

صحيح string_map_get_at (string_map *map, size_t اشاریہ, ثابت حرف **name, ثابت حرف **قدر);

size_t string_map_ناپ (string_map *map);

#جے_انت
