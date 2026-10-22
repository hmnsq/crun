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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ "error.h"
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <argp.h>
#அந்தர்பாவ <ocispec/runtime_spec_schema_config_schema.h>
#அந்தர்பாவ "container.h"

#define SYS_FS_BPF "/sys/fs/bpf"
#define CRUN_BPF_DIR SYS_FS_BPF "/crun"

கட்டமைப்பு bpf_program;

கட்டமைப்பு bpf_program *bpf_program_new (size_t size);
கட்டமைப்பு bpf_program *bpf_program_append (கட்டமைப்பு bpf_program *p, வெறுமை *data, size_t size);

கட்டமைப்பு bpf_program *bpf_program_init_dev (கட்டமைப்பு bpf_program *program, libcrun_error_t *err);
கட்டமைப்பு bpf_program *bpf_program_append_dev (கட்டமைப்பு bpf_program *program, மாறிலி எழுத்து *access, எழுத்து type, முழு major,
                                            முழு minor, bool accept, libcrun_error_t *err);
கட்டமைப்பு bpf_program *bpf_program_complete_dev (கட்டமைப்பு bpf_program *program, libcrun_error_t *err);

முழு libcrun_ebpf_load (கட்டமைப்பு bpf_program *program, முழு dirfd, மாறிலி எழுத்து *pin, libcrun_error_t *err);
முழு libcrun_ebpf_read_program (கட்டமைப்பு bpf_program **program, மாறிலி எழுத்து *path, libcrun_error_t *err);
முழு libcrun_ebpf_query_cgroup_progs (மாறிலி எழுத்து *cgroup_path, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err);
bool libcrun_ebpf_cmp_programs (கட்டமைப்பு bpf_program *program1, கட்டமைப்பு bpf_program *program2);

#endif
