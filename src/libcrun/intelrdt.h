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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <stdbool.h>
#অন্তর্ভাৱ "error.h"

পূর্ণাঙ্ক resctl_create (ধ্রুৱ ৱর্ণ *name, bool explicit_clos_id, bool *created, ধ্রুৱ ৱর্ণ *l3_cache_schema, ধ্রুৱ ৱর্ণ *mem_bw_schema, ৱর্ণ *ধ্রুৱ *schemata, libcrun_error_t *err);
পূর্ণাঙ্ক resctl_move_task_to (ধ্রুৱ ৱর্ণ *name, ধ্রুৱ ৱর্ণ *monitoring_name, pid_t pid, libcrun_error_t *err);
পূর্ণাঙ্ক resctl_update (ধ্রুৱ ৱর্ণ *name, ধ্রুৱ ৱর্ণ *l3_cache_schema, ধ্রুৱ ৱর্ণ *mem_bw_schema, ৱর্ণ *ধ্রুৱ *schemata, libcrun_error_t *err);
পূর্ণাঙ্ক resctl_destroy (ধ্রুৱ ৱর্ণ *name, libcrun_error_t *err);

পূর্ণাঙ্ক resctl_destroy_monitoring_group (ধ্রুৱ ৱর্ণ *resctrl_group_name, ধ্রুৱ ৱর্ণ *container_id, libcrun_error_t *err);
পূর্ণাঙ্ক resctl_create_monitoring_group (ধ্রুৱ ৱর্ণ *resctrl_group_name, ধ্রুৱ ৱর্ণ *container_id, libcrun_error_t *err);

#endif
