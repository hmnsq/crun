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

সংরচনা bpf_program;

সংরচনা bpf_program *bpf_program_new (size_t size);
সংরচনা bpf_program *bpf_program_append (সংরচনা bpf_program *p, শূন্য *data, size_t size);

সংরচনা bpf_program *bpf_program_init_dev (সংরচনা bpf_program *program, libcrun_error_t *err);
সংরচনা bpf_program *bpf_program_append_dev (সংরচনা bpf_program *program, ধ্রুব বর্ণ *access, বর্ণ type, পূর্ণাঙ্ক major,
                                            পূর্ণাঙ্ক minor, bool accept, libcrun_error_t *err);
সংরচনা bpf_program *bpf_program_complete_dev (সংরচনা bpf_program *program, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_ebpf_load (সংরচনা bpf_program *program, পূর্ণাঙ্ক dirfd, ধ্রুব বর্ণ *pin, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_ebpf_read_program (সংরচনা bpf_program **program, ধ্রুব বর্ণ *path, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_ebpf_query_cgroup_progs (ধ্রুব বর্ণ *cgroup_path, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err);
bool libcrun_ebpf_cmp_programs (সংরচনা bpf_program *program1, সংরচনা bpf_program *program2);

#endif
