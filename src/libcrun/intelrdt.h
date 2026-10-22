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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdbool.h>
#অন্তর্ভাব "error.h"

পূর্ণসংখ্যা resctl_create (ধ্রুবক অক্ষর *name, bool explicit_clos_id, bool *created, ধ্রুবক অক্ষর *l3_cache_schema, ধ্রুবক অক্ষর *mem_bw_schema, অক্ষর *ধ্রুবক *schemata, libcrun_error_t *err);
পূর্ণসংখ্যা resctl_move_task_to (ধ্রুবক অক্ষর *name, ধ্রুবক অক্ষর *monitoring_name, pid_t pid, libcrun_error_t *err);
পূর্ণসংখ্যা resctl_update (ধ্রুবক অক্ষর *name, ধ্রুবক অক্ষর *l3_cache_schema, ধ্রুবক অক্ষর *mem_bw_schema, অক্ষর *ধ্রুবক *schemata, libcrun_error_t *err);
পূর্ণসংখ্যা resctl_destroy (ধ্রুবক অক্ষর *name, libcrun_error_t *err);

পূর্ণসংখ্যা resctl_destroy_monitoring_group (ধ্রুবক অক্ষর *resctrl_group_name, ধ্রুবক অক্ষর *container_id, libcrun_error_t *err);
পূর্ণসংখ্যা resctl_create_monitoring_group (ধ্রুবক অক্ষর *resctrl_group_name, ধ্রুবক অক্ষর *container_id, libcrun_error_t *err);

#endif
