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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن "error.h"
#ضمن <errno.h>
#ضمن <argp.h>
#ضمن <ocispec/runtime_spec_schema_config_schema.h>
#ضمن "container.h"

#define SYS_FS_BPF "/sys/fs/bpf"
#define CRUN_BPF_DIR SYS_FS_BPF "/crun"

بنية bpf_program;

بنية bpf_program *bpf_program_new (size_t size);
بنية bpf_program *bpf_program_append (بنية bpf_program *p, فراغ *data, size_t size);

بنية bpf_program *bpf_program_init_dev (بنية bpf_program *program, libcrun_error_t *err);
بنية bpf_program *bpf_program_append_dev (بنية bpf_program *program, ثابت حرف *access, حرف type, صحيح major,
                                            صحيح minor, bool accept, libcrun_error_t *err);
بنية bpf_program *bpf_program_complete_dev (بنية bpf_program *program, libcrun_error_t *err);

صحيح libcrun_ebpf_load (بنية bpf_program *program, صحيح dirfd, ثابت حرف *pin, libcrun_error_t *err);
صحيح libcrun_ebpf_read_program (بنية bpf_program **program, ثابت حرف *path, libcrun_error_t *err);
صحيح libcrun_ebpf_query_cgroup_progs (ثابت حرف *cgroup_path, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err);
bool libcrun_ebpf_cmp_programs (بنية bpf_program *program1, بنية bpf_program *program2);

#endif
