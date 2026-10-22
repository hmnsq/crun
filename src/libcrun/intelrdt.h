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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INTEL_RDT_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INTEL_RDT_H

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "error.h"

ਪੂਰਨ resctl_create (ਅਚਲ ਅੱਖਰ *name, bool explicit_clos_id, bool *created, ਅਚਲ ਅੱਖਰ *l3_cache_schema, ਅਚਲ ਅੱਖਰ *mem_bw_schema, ਅੱਖਰ *ਅਚਲ *schemata, libcrun_error_t *err);
ਪੂਰਨ resctl_move_task_ਮੰਜ਼ਿਲ (ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *monitoring_name, pid_t pid, libcrun_error_t *err);
ਪੂਰਨ resctl_update (ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *l3_cache_schema, ਅਚਲ ਅੱਖਰ *mem_bw_schema, ਅੱਖਰ *ਅਚਲ *schemata, libcrun_error_t *err);
ਪੂਰਨ resctl_destroy (ਅਚਲ ਅੱਖਰ *name, libcrun_error_t *err);

ਪੂਰਨ resctl_destroy_monitoring_ਸਮੂਹ (ਅਚਲ ਅੱਖਰ *resctrl_ਸਮੂਹ_name, ਅਚਲ ਅੱਖਰ *container_id, libcrun_error_t *err);
ਪੂਰਨ resctl_create_monitoring_ਸਮੂਹ (ਅਚਲ ਅੱਖਰ *resctrl_ਸਮੂਹ_name, ਅਚਲ ਅੱਖਰ *container_id, libcrun_error_t *err);

#ਜੇ_ਅੰਤ
