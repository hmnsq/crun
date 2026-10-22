/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2023 Giuseppe Scrivano <giuseppe@scrivano.org>
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

#ifndef INTEL_RDT_H
#define INTEL_RDT_H

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdbool.h>
#અન્તર્ભાવ "error.h"

પૂર્ણાંક resctl_create (અચલ અક્ષર *name, bool explicit_clos_id, bool *created, અચલ અક્ષર *l3_cache_schema, અચલ અક્ષર *mem_bw_schema, અક્ષર *અચલ *schemata, libcrun_error_t *err);
પૂર્ણાંક resctl_move_task_to (અચલ અક્ષર *name, અચલ અક્ષર *monitoring_name, pid_t pid, libcrun_error_t *err);
પૂર્ણાંક resctl_update (અચલ અક્ષર *name, અચલ અક્ષર *l3_cache_schema, અચલ અક્ષર *mem_bw_schema, અક્ષર *અચલ *schemata, libcrun_error_t *err);
પૂર્ણાંક resctl_destroy (અચલ અક્ષર *name, libcrun_error_t *err);

પૂર્ણાંક resctl_destroy_monitoring_group (અચલ અક્ષર *resctrl_group_name, અચલ અક્ષર *container_id, libcrun_error_t *err);
પૂર્ણાંક resctl_create_monitoring_group (અચલ અક્ષર *resctrl_group_name, અચલ અક્ષર *container_id, libcrun_error_t *err);

#endif
