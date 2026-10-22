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

#اگر_غیر_تعریف_شدہ INTEL_RDT_H
#تعریف_کرو INTEL_RDT_H

#شامل <config.h>
#شامل <stdio.h>
#شامل <stdbool.h>
#شامل "error.h"

صحیح resctl_create (ثابت حرف *name, bool explicit_clos_id, bool *created, ثابت حرف *l3_cache_schema, ثابت حرف *mem_bw_schema, حرف *ثابت *schemata, libcrun_error_t *err);
صحیح resctl_move_task_مقصد (ثابت حرف *name, ثابت حرف *monitoring_name, pid_t pid, libcrun_error_t *err);
صحیح resctl_update (ثابت حرف *name, ثابت حرف *l3_cache_schema, ثابت حرف *mem_bw_schema, حرف *ثابت *schemata, libcrun_error_t *err);
صحیح resctl_destroy (ثابت حرف *name, libcrun_error_t *err);

صحیح resctl_destroy_monitoring_گروہ (ثابت حرف *resctrl_گروہ_name, ثابت حرف *container_id, libcrun_error_t *err);
صحیح resctl_create_monitoring_گروہ (ثابت حرف *resctrl_گروہ_name, ثابت حرف *container_id, libcrun_error_t *err);

#اگر_ختم
