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

#جيڪڏهن_وصف_نه_ٿيل INTEL_RDT_H
#وصف_ڪريو INTEL_RDT_H

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <stdbool.h>
#ضمن "error.h"

صحيح resctl_create (ثابت حرف *name, bool explicit_clos_id, bool *created, ثابت حرف *l3_cache_schema, ثابت حرف *mem_bw_schema, حرف *ثابت *schemata, libcrun_error_t *err);
صحيح resctl_move_task_منزل (ثابت حرف *name, ثابت حرف *monitoring_name, pid_t pid, libcrun_error_t *err);
صحيح resctl_update (ثابت حرف *name, ثابت حرف *l3_cache_schema, ثابت حرف *mem_bw_schema, حرف *ثابت *schemata, libcrun_error_t *err);
صحيح resctl_destroy (ثابت حرف *name, libcrun_error_t *err);

صحيح resctl_destroy_monitoring_گروهه (ثابت حرف *resctrl_گروهه_name, ثابت حرف *container_id, libcrun_error_t *err);
صحيح resctl_create_monitoring_گروهه (ثابت حرف *resctrl_گروهه_name, ثابت حرف *container_id, libcrun_error_t *err);

#جيڪڏهن_پورو
