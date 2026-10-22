/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2019 Giuseppe Scrivano <giuseppe@scrivano.org>
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
#నిర్వచితం_కాకపోతే EBPF_H
#నిర్వచించు EBPF_H

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ "error.h"
#అన్తర్భావ <errno.h>
#అన్తర్భావ <argp.h>
#అన్తర్భావ <ocispec/runtime_spec_schema_config_schema.h>
#అన్తర్భావ "container.h"

#నిర్వచించు SYS_FS_BPF "/sys/fs/bpf"
#నిర్వచించు CRUN_BPF_DIR SYS_FS_BPF "/crun"

నిర్మాణం bpf_కార్యక్రమం;

నిర్మాణం bpf_కార్యక్రమం *bpf_కార్యక్రమం_new (size_t కొలత);
నిర్మాణం bpf_కార్యక్రమం *bpf_కార్యక్రమం_append (నిర్మాణం bpf_కార్యక్రమం *p, శూన్య *దత్తాంశం, size_t కొలత);

నిర్మాణం bpf_కార్యక్రమం *bpf_కార్యక్రమం_init_dev (నిర్మాణం bpf_కార్యక్రమం *కార్యక్రమం, libcrun_error_t *err);
నిర్మాణం bpf_కార్యక్రమం *bpf_కార్యక్రమం_append_dev (నిర్మాణం bpf_కార్యక్రమం *కార్యక్రమం, స్థిరరాశి అక్షరం *access, అక్షరం type, పూర్ణాంకం major,
                                            పూర్ణాంకం minor, bool accept, libcrun_error_t *err);
నిర్మాణం bpf_కార్యక్రమం *bpf_కార్యక్రమం_complete_dev (నిర్మాణం bpf_కార్యక్రమం *కార్యక్రమం, libcrun_error_t *err);

పూర్ణాంకం libcrun_ebpf_ఎక్కించు (నిర్మాణం bpf_కార్యక్రమం *కార్యక్రమం, పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *pin, libcrun_error_t *err);
పూర్ణాంకం libcrun_ebpf_read_కార్యక్రమం (నిర్మాణం bpf_కార్యక్రమం **కార్యక్రమం, స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err);
పూర్ణాంకం libcrun_ebpf_query_cgroup_progs (స్థిరరాశి అక్షరం *cgroup_మార్గం, uint32_t **progs_బహిర్గతం, size_t *n_progs_బహిర్గతం, libcrun_error_t *err);
bool libcrun_ebpf_cmp_programs (నిర్మాణం bpf_కార్యక్రమం *program1, నిర్మాణం bpf_కార్యక్రమం *program2);

#అయితే_ముగింపు
