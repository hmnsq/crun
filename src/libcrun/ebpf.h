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
#ifndef EBPF_H
#define EBPF_H

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব "error.h"
#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <argp.h>
#অন্তর্ভাব <ocispec/runtime_spec_schema_config_schema.h>
#অন্তর্ভাব "container.h"

#define SYS_FS_BPF "/sys/fs/bpf"
#define CRUN_BPF_DIR SYS_FS_BPF "/crun"

কাঠামো bpf_program;

কাঠামো bpf_program *bpf_program_new (size_t আকার_2);
কাঠামো bpf_program *bpf_program_append (কাঠামো bpf_program *p, শূন্য *তথ্য, size_t আকার_2);

কাঠামো bpf_program *bpf_program_init_dev (কাঠামো bpf_program *program, libcrun_error_t *err);
কাঠামো bpf_program *bpf_program_append_dev (কাঠামো bpf_program *program, ধ্রুবক অক্ষর *access, অক্ষর type, পূর্ণসংখ্যা major,
                                            পূর্ণসংখ্যা minor, bool accept, libcrun_error_t *err);
কাঠামো bpf_program *bpf_program_complete_dev (কাঠামো bpf_program *program, libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_ebpf_load (কাঠামো bpf_program *program, পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *pin, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_ebpf_read_program (কাঠামো bpf_program **program, ধ্রুবক অক্ষর *path, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_ebpf_query_cgroup_progs (ধ্রুবক অক্ষর *cgroup_path, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err);
bool libcrun_ebpf_cmp_programs (কাঠামো bpf_program *program1, কাঠামো bpf_program *program2);

#endif
