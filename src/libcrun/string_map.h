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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ STRING_MAP_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STRING_MAP_H

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GNU_ਸਰੋਤ
#  define GNU_ਸਰੋਤ
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_HSEARCH_R
#  include <search.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <ocispec/runtime_spec_schema_config_schema.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "error.h"

ਬਣਤਰ string_map_s;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ string_map_s string_map;

ਅਚਲ ਅੱਖਰ *ਲੱਭੋ_string_map_ਮੁੱਲ (string_map *map, ਅਚਲ ਅੱਖਰ *name);

string_map *make_string_map_from_json (json_map_string_string *jmap);

ਖਾਲੀ free_string_map (string_map *map);

ਪੂਰਨ string_map_get_at (string_map *map, size_t ਸੂਚਕਾਂਕ, ਅਚਲ ਅੱਖਰ **name, ਅਚਲ ਅੱਖਰ **ਮੁੱਲ);

size_t string_map_ਮਾਪ (string_map *map);

#ਜੇ_ਅੰਤ
