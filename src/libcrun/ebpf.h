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

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include <errno.h>
#include <argp.h>
#include <ocispec/runtime_spec_schema_config_schema.h>
#include "container.h"

#define SYS_FS_BPF "/sys/fs/bpf"
#define CRUN_BPF_DIR SYS_FS_BPF "/crun"

sa_nracanaa bpf_program;

sa_nracanaa bpf_program *bpf_program_new (size_t size);
sa_nracanaa bpf_program *bpf_program_append (sa_nracanaa bpf_program *p, void *data, size_t size);

sa_nracanaa bpf_program *bpf_program_init_dev (sa_nracanaa bpf_program *program, libcrun_error_t *err);
sa_nracanaa bpf_program *bpf_program_append_dev (sa_nracanaa bpf_program *program, const var_na *access, var_na type, ကိန်းပြည့် major,
                                            ကိန်းပြည့် minor, bool accept, libcrun_error_t *err);
sa_nracanaa bpf_program *bpf_program_complete_dev (sa_nracanaa bpf_program *program, libcrun_error_t *err);

ကိန်းပြည့် libcrun_ebpf_load (sa_nracanaa bpf_program *program, ကိန်းပြည့် dirfd, const var_na *pin, libcrun_error_t *err);
ကိန်းပြည့် libcrun_ebpf_read_program (sa_nracanaa bpf_program **program, const var_na *path, libcrun_error_t *err);
ကိန်းပြည့် libcrun_ebpf_query_cgroup_progs (const var_na *cgroup_path, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err);
bool libcrun_ebpf_cmp_programs (sa_nracanaa bpf_program *program1, sa_nracanaa bpf_program *program2);

#endif
