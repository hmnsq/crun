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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdbool.h>
#ଅନ୍ତର୍ଭାଵ "error.h"

ପୂର୍ଣ୍ଣସଂଖ୍ୟା resctl_create (ଅଚଳ ଅକ୍ଷର *name, bool explicit_clos_id, bool *created, ଅଚଳ ଅକ୍ଷର *l3_cache_schema, ଅଚଳ ଅକ୍ଷର *mem_bw_schema, ଅକ୍ଷର *ଅଚଳ *schemata, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା resctl_move_task_to (ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଅକ୍ଷର *monitoring_name, pid_t pid, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା resctl_update (ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଅକ୍ଷର *l3_cache_schema, ଅଚଳ ଅକ୍ଷର *mem_bw_schema, ଅକ୍ଷର *ଅଚଳ *schemata, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା resctl_destroy (ଅଚଳ ଅକ୍ଷର *name, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା resctl_destroy_monitoring_group (ଅଚଳ ଅକ୍ଷର *resctrl_group_name, ଅଚଳ ଅକ୍ଷର *container_id, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା resctl_create_monitoring_group (ଅଚଳ ଅକ୍ଷର *resctrl_group_name, ଅଚଳ ଅକ୍ଷର *container_id, libcrun_error_t *err);

#endif
