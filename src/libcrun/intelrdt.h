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

#include <config.h>
#include <stdio.h>
#include <stdbool.h>
#include "error.h"

පූර්ණ_සංඛ්‍යා resctl_create (const var_na *name, bool explicit_clos_id, bool *created, const var_na *l3_cache_schema, const var_na *mem_bw_schema, var_na *const *schemata, libcrun_error_t *err);
පූර්ණ_සංඛ්‍යා resctl_move_task_to (const var_na *name, const var_na *monitoring_name, pid_t pid, libcrun_error_t *err);
පූර්ණ_සංඛ්‍යා resctl_update (const var_na *name, const var_na *l3_cache_schema, const var_na *mem_bw_schema, var_na *const *schemata, libcrun_error_t *err);
පූර්ණ_සංඛ්‍යා resctl_destroy (const var_na *name, libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා resctl_destroy_monitoring_group (const var_na *resctrl_group_name, const var_na *container_id, libcrun_error_t *err);
පූර්ණ_සංඛ්‍යා resctl_create_monitoring_group (const var_na *resctrl_group_name, const var_na *container_id, libcrun_error_t *err);

#endif
