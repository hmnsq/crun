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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ "error.h"
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <argp.h>
#ಅನ್ತರ್ಭಾವ <ocispec/runtime_spec_schema_config_schema.h>
#ಅನ್ತರ್ಭಾವ "container.h"

#define SYS_FS_BPF "/sys/fs/bpf"
#define CRUN_BPF_DIR SYS_FS_BPF "/crun"

ಸಂರಚನಾ bpf_program;

ಸಂರಚನಾ bpf_program *bpf_program_new (size_t size);
ಸಂರಚನಾ bpf_program *bpf_program_append (ಸಂರಚನಾ bpf_program *p, ಶೂನ್ಯ *data, size_t size);

ಸಂರಚನಾ bpf_program *bpf_program_init_dev (ಸಂರಚನಾ bpf_program *program, libcrun_error_t *err);
ಸಂರಚನಾ bpf_program *bpf_program_append_dev (ಸಂರಚನಾ bpf_program *program, ಧ್ರುವ ವರ್ಣ *access, ವರ್ಣ type, ಪೂರ್ಣಾಙ್ಕ major,
                                            ಪೂರ್ಣಾಙ್ಕ minor, bool accept, libcrun_error_t *err);
ಸಂರಚನಾ bpf_program *bpf_program_complete_dev (ಸಂರಚನಾ bpf_program *program, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_ebpf_load (ಸಂರಚನಾ bpf_program *program, ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *pin, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_ebpf_read_program (ಸಂರಚನಾ bpf_program **program, ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_ebpf_query_cgroup_progs (ಧ್ರುವ ವರ್ಣ *cgroup_path, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err);
bool libcrun_ebpf_cmp_programs (ಸಂರಚನಾ bpf_program *program1, ಸಂರಚನಾ bpf_program *program2);

#endif
