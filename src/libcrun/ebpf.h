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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ "error.h"
#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <argp.h>
#અન્તર્ભાવ <ocispec/runtime_spec_schema_config_schema.h>
#અન્તર્ભાવ "container.h"

#define SYS_FS_BPF "/sys/fs/bpf"
#define CRUN_BPF_DIR SYS_FS_BPF "/crun"

સંરચના bpf_program;

સંરચના bpf_program *bpf_program_new (size_t size);
સંરચના bpf_program *bpf_program_append (સંરચના bpf_program *p, શૂન્ય *data, size_t size);

સંરચના bpf_program *bpf_program_init_dev (સંરચના bpf_program *program, libcrun_error_t *err);
સંરચના bpf_program *bpf_program_append_dev (સંરચના bpf_program *program, અચલ અક્ષર *access, અક્ષર type, પૂર્ણાંક major,
                                            પૂર્ણાંક minor, bool accept, libcrun_error_t *err);
સંરચના bpf_program *bpf_program_complete_dev (સંરચના bpf_program *program, libcrun_error_t *err);

પૂર્ણાંક libcrun_ebpf_load (સંરચના bpf_program *program, પૂર્ણાંક dirfd, અચલ અક્ષર *pin, libcrun_error_t *err);
પૂર્ણાંક libcrun_ebpf_read_program (સંરચના bpf_program **program, અચલ અક્ષર *path, libcrun_error_t *err);
પૂર્ણાંક libcrun_ebpf_query_cgroup_progs (અચલ અક્ષર *cgroup_path, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err);
bool libcrun_ebpf_cmp_programs (સંરચના bpf_program *program1, સંરચના bpf_program *program2);

#endif
