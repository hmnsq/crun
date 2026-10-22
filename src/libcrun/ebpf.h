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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव "error.h"
#अन्तर्भाव <errno.h>
#अन्तर्भाव <argp.h>
#अन्तर्भाव <ocispec/runtime_spec_schema_config_schema.h>
#अन्तर्भाव "container.h"

#define SYS_FS_BPF "/sys/fs/bpf"
#define CRUN_BPF_DIR SYS_FS_BPF "/crun"

संरचना bpf_program;

संरचना bpf_program *bpf_program_new (माप_प्रकार size);
संरचना bpf_program *bpf_program_append (संरचना bpf_program *p, शून्य *data, माप_प्रकार size);

संरचना bpf_program *bpf_program_init_dev (संरचना bpf_program *program, libcrun_error_t *err);
संरचना bpf_program *bpf_program_append_dev (संरचना bpf_program *program, स्थिरांक अक्षर *access, अक्षर type, पूर्णांक major,
                                            पूर्णांक minor, bool accept, libcrun_error_t *err);
संरचना bpf_program *bpf_program_complete_dev (संरचना bpf_program *program, libcrun_error_t *err);

पूर्णांक libcrun_ebpf_load (संरचना bpf_program *program, पूर्णांक dirfd, स्थिरांक अक्षर *pin, libcrun_error_t *err);
पूर्णांक libcrun_ebpf_read_program (संरचना bpf_program **program, स्थिरांक अक्षर *path, libcrun_error_t *err);
पूर्णांक libcrun_ebpf_query_cgroup_progs (स्थिरांक अक्षर *cgroup_path, uint32_t **progs_out, माप_प्रकार *n_progs_out, libcrun_error_t *err);
bool libcrun_ebpf_cmp_programs (संरचना bpf_program *program1, संरचना bpf_program *program2);

#endif
