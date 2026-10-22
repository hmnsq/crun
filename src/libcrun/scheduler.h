/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019, 2021 Giuseppe Scrivano <giuseppe@scrivano.org>
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
#ifndef SCHEDULER_H
#define SCHEDULER_H
#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ "error.h"
#অন্তর্ভাৱ "container.h"
#অন্তর্ভাৱ "status.h"

পূর্ণাঙ্ক libcrun_set_scheduler (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_set_cpu_affinity_from_string (pid_t pid, ধ্রুৱ ৱর্ণ *str, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_reset_cpu_affinity_mask (pid_t pid, libcrun_error_t *err);

#endif
