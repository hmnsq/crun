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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdbool.h>
#அந்தர்பாவ "error.h"

முழு resctl_create (மாறிலி எழுத்து *name, bool explicit_clos_id, bool *created, மாறிலி எழுத்து *l3_cache_schema, மாறிலி எழுத்து *mem_bw_schema, எழுத்து *மாறிலி *schemata, libcrun_error_t *err);
முழு resctl_move_task_to (மாறிலி எழுத்து *name, மாறிலி எழுத்து *monitoring_name, pid_t pid, libcrun_error_t *err);
முழு resctl_update (மாறிலி எழுத்து *name, மாறிலி எழுத்து *l3_cache_schema, மாறிலி எழுத்து *mem_bw_schema, எழுத்து *மாறிலி *schemata, libcrun_error_t *err);
முழு resctl_destroy (மாறிலி எழுத்து *name, libcrun_error_t *err);

முழு resctl_destroy_monitoring_group (மாறிலி எழுத்து *resctrl_group_name, மாறிலி எழுத்து *container_id, libcrun_error_t *err);
முழு resctl_create_monitoring_group (மாறிலி எழுத்து *resctrl_group_name, மாறிலி எழுத்து *container_id, libcrun_error_t *err);

#endif
