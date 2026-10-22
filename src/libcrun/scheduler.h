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
#నిర్వచితం_కాకపోతే SCHEDULER_H
#నిర్వచించు SCHEDULER_H
#అన్తర్భావ <config.h>
#అన్తర్భావ "error.h"
#అన్తర్భావ "container.h"
#అన్తర్భావ "status.h"

పూర్ణాంకం libcrun_సమితి_scheduler (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err);

పూర్ణాంకం libcrun_సమితి_cpu_affinity_from_string (pid_t pid, స్థిరరాశి అక్షరం *str, libcrun_error_t *err);

పూర్ణాంకం libcrun_reset_cpu_affinity_ముసుగు (pid_t pid, libcrun_error_t *err);

#అయితే_ముగింపు
