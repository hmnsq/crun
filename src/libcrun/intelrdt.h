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

#నిర్వచితం_కాకపోతే INTEL_RDT_H
#నిర్వచించు INTEL_RDT_H

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdbool.h>
#అన్తర్భావ "error.h"

పూర్ణాంకం resctl_create (స్థిరరాశి అక్షరం *name, bool explicit_clos_id, bool *created, స్థిరరాశి అక్షరం *l3_cache_schema, స్థిరరాశి అక్షరం *mem_bw_schema, అక్షరం *స్థిరరాశి *schemata, libcrun_error_t *err);
పూర్ణాంకం resctl_move_task_గమ్యం (స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *monitoring_name, pid_t pid, libcrun_error_t *err);
పూర్ణాంకం resctl_update (స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *l3_cache_schema, స్థిరరాశి అక్షరం *mem_bw_schema, అక్షరం *స్థిరరాశి *schemata, libcrun_error_t *err);
పూర్ణాంకం resctl_destroy (స్థిరరాశి అక్షరం *name, libcrun_error_t *err);

పూర్ణాంకం resctl_destroy_monitoring_సమూహం (స్థిరరాశి అక్షరం *resctrl_సమూహం_name, స్థిరరాశి అక్షరం *container_id, libcrun_error_t *err);
పూర్ణాంకం resctl_create_monitoring_సమూహం (స్థిరరాశి అక్షరం *resctrl_సమూహం_name, స్థిరరాశి అక్షరం *container_id, libcrun_error_t *err);

#అయితే_ముగింపు
