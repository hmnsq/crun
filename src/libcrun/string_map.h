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

#অন্তর্ভাৱ <config.h>

#ifdef HAVE_HSEARCH_R
#  include <search.h>
#endif

#অন্তর্ভাৱ <ocispec/runtime_spec_schema_config_schema.h>
#অন্তর্ভাৱ "error.h"

সংরচনা string_map_s;
প্রকারনির্দেশ সংরচনা string_map_s string_map;

ধ্রুৱ ৱর্ণ *find_string_map_মান (string_map *map, ধ্রুৱ ৱর্ণ *name);

string_map *make_string_map_from_json (json_map_string_string *jmap);

শূন্য free_string_map (string_map *map);

পূর্ণাঙ্ক string_map_get_at (string_map *map, size_t index, ধ্রুৱ ৱর্ণ **name, ধ্রুৱ ৱর্ণ **মান);

size_t string_map_size (string_map *map);

#endif
