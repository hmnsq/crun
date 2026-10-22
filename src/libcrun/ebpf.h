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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ "error.h"
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <argp.h>
#അന്തര്ഭാവ <ocispec/runtime_spec_schema_config_schema.h>
#അന്തര്ഭാവ "container.h"

#define SYS_FS_BPF "/sys/fs/bpf"
#define CRUN_BPF_DIR SYS_FS_BPF "/crun"

ഘടന bpf_program;

ഘടന bpf_program *bpf_program_new (size_t size);
ഘടന bpf_program *bpf_program_append (ഘടന bpf_program *p, ശൂന്യം *data, size_t size);

ഘടന bpf_program *bpf_program_init_dev (ഘടന bpf_program *program, libcrun_error_t *err);
ഘടന bpf_program *bpf_program_append_dev (ഘടന bpf_program *program, സ്ഥിരം അക്ഷരം *access, അക്ഷരം type, പൂർണ്ണസംഖ്യ major,
                                            പൂർണ്ണസംഖ്യ minor, bool accept, libcrun_error_t *err);
ഘടന bpf_program *bpf_program_complete_dev (ഘടന bpf_program *program, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_ebpf_load (ഘടന bpf_program *program, പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *pin, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_ebpf_read_program (ഘടന bpf_program **program, സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_ebpf_query_cgroup_progs (സ്ഥിരം അക്ഷരം *cgroup_path, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err);
bool libcrun_ebpf_cmp_programs (ഘടന bpf_program *program1, ഘടന bpf_program *program2);

#endif
