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
#अगर_अघोषित EBPF_H
#घोषणा EBPF_H

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश "error.h"
#समावेश <errno.h>
#समावेश <argp.h>
#समावेश <ocispec/runtime_spec_schema_config_schema.h>
#समावेश "container.h"

#घोषणा SYS_FS_BPF "/sys/fs/bpf"
#घोषणा CRUN_BPF_DIR SYS_FS_BPF "/crun"

काष्ठा bpf_program;

काष्ठा bpf_program *bpf_program_new (माप_प्रकार आकार);
काष्ठा bpf_program *bpf_program_append (काष्ठा bpf_program *p, व्योम *तथ्य, माप_प्रकार आकार);

काष्ठा bpf_program *bpf_program_init_dev (काष्ठा bpf_program *program, libcrun_error_t *err);
काष्ठा bpf_program *bpf_program_append_dev (काष्ठा bpf_program *program, स्थिर अक्षर *access, अक्षर type, पूर्णांक major,
                                            पूर्णांक minor, bool accept, libcrun_error_t *err);
काष्ठा bpf_program *bpf_program_complete_dev (काष्ठा bpf_program *program, libcrun_error_t *err);

पूर्णांक libcrun_ebpf_load (काष्ठा bpf_program *program, पूर्णांक dirfd, स्थिर अक्षर *pin, libcrun_error_t *err);
पूर्णांक libcrun_ebpf_read_program (काष्ठा bpf_program **program, स्थिर अक्षर *path, libcrun_error_t *err);
पूर्णांक libcrun_ebpf_query_cgroup_progs (स्थिर अक्षर *cgroup_path, uint32_t **progs_out, माप_प्रकार *n_progs_out, libcrun_error_t *err);
bool libcrun_ebpf_cmp_programs (काष्ठा bpf_program *program1, काष्ठा bpf_program *program2);

#पूर्ण_अगर
