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
#اگر_تعریف_نشده EBPF_H
#تعریف_کن EBPF_H

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن "error.h"
#ضمن <errno.h>
#ضمن <argp.h>
#ضمن <ocispec/runtime_spec_schema_config_schema.h>
#ضمن "container.h"

#تعریف_کن SYS_FS_BPF "/sys/fs/bpf"
#تعریف_کن CRUN_BPF_DIR SYS_FS_BPF "/crun"

بنية bpf_برنامه;

بنية bpf_برنامه *bpf_برنامه_new (size_t اندازه);
بنية bpf_برنامه *bpf_برنامه_append (بنية bpf_برنامه *p, فراغ *داده, size_t اندازه);

بنية bpf_برنامه *bpf_برنامه_init_dev (بنية bpf_برنامه *برنامه, libcrun_error_t *err);
بنية bpf_برنامه *bpf_برنامه_append_dev (بنية bpf_برنامه *برنامه, ثابت حرف *access, حرف type, صحيح major,
                                            صحيح minor, bool accept, libcrun_error_t *err);
بنية bpf_برنامه *bpf_برنامه_complete_dev (بنية bpf_برنامه *برنامه, libcrun_error_t *err);

صحيح libcrun_ebpf_بارگیری (بنية bpf_برنامه *برنامه, صحيح dirfd, ثابت حرف *pin, libcrun_error_t *err);
صحيح libcrun_ebpf_read_برنامه (بنية bpf_برنامه **برنامه, ثابت حرف *مسیر, libcrun_error_t *err);
صحيح libcrun_ebpf_query_cgroup_progs (ثابت حرف *cgroup_مسیر, uint32_t **progs_بیرون, size_t *n_progs_بیرون, libcrun_error_t *err);
bool libcrun_ebpf_cmp_programs (بنية bpf_برنامه *program1, بنية bpf_برنامه *program2);

#پایان_اگر
