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

#నిర్వచితం_కాకపోతే STRING_MAP_H
#నిర్వచించు STRING_MAP_H

#నిర్వచితం_కాకపోతే GNU_ఆధారం
#  define GNU_ఆధారం
#అయితే_ముగింపు

#అన్తర్భావ <config.h>

#నిర్వచితమైతే HAVE_HSEARCH_R
#  include <search.h>
#అయితే_ముగింపు

#అన్తర్భావ <ocispec/runtime_spec_schema_config_schema.h>
#అన్తర్భావ "error.h"

నిర్మాణం string_map_s;
రకనిర్వచనం నిర్మాణం string_map_s string_map;

స్థిరరాశి అక్షరం *కనుగొను_string_map_విలువ (string_map *map, స్థిరరాశి అక్షరం *name);

string_map *make_string_map_from_json (json_map_string_string *jmap);

శూన్య free_string_map (string_map *map);

పూర్ణాంకం string_map_get_at (string_map *map, size_t సూచీ, స్థిరరాశి అక్షరం **name, స్థిరరాశి అక్షరం **విలువ);

size_t string_map_కొలత (string_map *map);

#అయితే_ముగింపు
