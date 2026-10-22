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
#यदि_अपरिभाषितम् EBPF_H
#परिभाषय EBPF_H

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव "error.h"
#अन्तर्भाव <errno.h>
#अन्तर्भाव <argp.h>
#अन्तर्भाव <ocispec/runtime_spec_schema_config_schema.h>
#अन्तर्भाव "container.h"

#परिभाषय SYS_FS_BPF "/sys/fs/bpf"
#परिभाषय CRUN_BPF_DIR SYS_FS_BPF "/crun"

संरचना bpf_क्रमादेशः;

संरचना bpf_क्रमादेशः *bpf_क्रमादेशः_new (size_t आकार);
संरचना bpf_क्रमादेशः *bpf_क्रमादेशः_append (संरचना bpf_क्रमादेशः *p, शून्य *दत्त, size_t आकार);

संरचना bpf_क्रमादेशः *bpf_क्रमादेशः_init_dev (संरचना bpf_क्रमादेशः *क्रमादेशः, libcrun_error_t *err);
संरचना bpf_क्रमादेशः *bpf_क्रमादेशः_append_dev (संरचना bpf_क्रमादेशः *क्रमादेशः, ध्रुव वर्ण *access, वर्ण type, पूर्णाङ्क major,
                                            पूर्णाङ्क minor, bool accept, libcrun_error_t *err);
संरचना bpf_क्रमादेशः *bpf_क्रमादेशः_complete_dev (संरचना bpf_क्रमादेशः *क्रमादेशः, libcrun_error_t *err);

पूर्णाङ्क libcrun_ebpf_आरोपय (संरचना bpf_क्रमादेशः *क्रमादेशः, पूर्णाङ्क dirfd, ध्रुव वर्ण *pin, libcrun_error_t *err);
पूर्णाङ्क libcrun_ebpf_read_क्रमादेशः (संरचना bpf_क्रमादेशः **क्रमादेशः, ध्रुव वर्ण *मार्गः, libcrun_error_t *err);
पूर्णाङ्क libcrun_ebpf_query_cgroup_progs (ध्रुव वर्ण *cgroup_मार्गः, uint32_t **progs_बहिर्गमनम्, size_t *n_progs_बहिर्गमनम्, libcrun_error_t *err);
bool libcrun_ebpf_cmp_programs (संरचना bpf_क्रमादेशः *program1, संरचना bpf_क्रमादेशः *program2);

#यदि_अन्तः
