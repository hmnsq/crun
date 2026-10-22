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

#شامل <config.h>
#شامل <stdio.h>
#شامل <stdbool.h>
#شامل "error.h"

صحیح resctl_create (ثابت نویسه *name, bool explicit_clos_id, bool *created, ثابت نویسه *l3_cache_schema, ثابت نویسه *mem_bw_schema, نویسه *ثابت *schemata, libcrun_error_t *err);
صحیح resctl_move_task_to (ثابت نویسه *name, ثابت نویسه *monitoring_name, pid_t pid, libcrun_error_t *err);
صحیح resctl_update (ثابت نویسه *name, ثابت نویسه *l3_cache_schema, ثابت نویسه *mem_bw_schema, نویسه *ثابت *schemata, libcrun_error_t *err);
صحیح resctl_destroy (ثابت نویسه *name, libcrun_error_t *err);

صحیح resctl_destroy_monitoring_group (ثابت نویسه *resctrl_group_name, ثابت نویسه *container_id, libcrun_error_t *err);
صحیح resctl_create_monitoring_group (ثابت نویسه *resctrl_group_name, ثابت نویسه *container_id, libcrun_error_t *err);

#endif
