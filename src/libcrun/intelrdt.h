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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ "error.h"

ಪೂರ್ಣಾಙ್ಕ resctl_create (ಧ್ರುವ ವರ್ಣ *name, bool explicit_clos_id, bool *created, ಧ್ರುವ ವರ್ಣ *l3_cache_schema, ಧ್ರುವ ವರ್ಣ *mem_bw_schema, ವರ್ಣ *ಧ್ರುವ *schemata, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ resctl_move_task_to (ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *monitoring_name, pid_t pid, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ resctl_update (ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *l3_cache_schema, ಧ್ರುವ ವರ್ಣ *mem_bw_schema, ವರ್ಣ *ಧ್ರುವ *schemata, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ resctl_destroy (ಧ್ರುವ ವರ್ಣ *name, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ resctl_destroy_monitoring_group (ಧ್ರುವ ವರ್ಣ *resctrl_group_name, ಧ್ರುವ ವರ್ಣ *container_id, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ resctl_create_monitoring_group (ಧ್ರುವ ವರ್ಣ *resctrl_group_name, ಧ್ರುವ ವರ್ಣ *container_id, libcrun_error_t *err);

#endif
