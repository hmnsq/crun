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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STRING_MAP_H
#ವ್ಯಾಖ್ಯಾನಿಸು STRING_MAP_H

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GNU_ಮೂಲ
#  define GNU_ಮೂಲ
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <config.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_HSEARCH_R
#  include <search.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <ocispec/runtime_spec_schema_config_schema.h>
#ಅನ್ತರ್ಭಾವ "error.h"

ರಚನೆ string_map_s;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ string_map_s string_map;

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕಂಡುಹಿಡಿ_string_map_ಮೌಲ್ಯ (string_map *map, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name);

string_map *make_string_map_from_json (json_map_string_string *jmap);

ಶೂನ್ಯ free_string_map (string_map *map);

ಪೂರ್ಣಾಂಕ string_map_get_at (string_map *map, size_t ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಮೌಲ್ಯ);

size_t string_map_ಗಾತ್ರ (string_map *map);

#ಆದರೆ_ಅಂತ್ಯ
