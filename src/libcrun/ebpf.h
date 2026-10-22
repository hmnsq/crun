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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ "error.h"
#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <argp.h>
#ଅନ୍ତର୍ଭାଵ <ocispec/runtime_spec_schema_config_schema.h>
#ଅନ୍ତର୍ଭାଵ "container.h"

#define SYS_FS_BPF "/sys/fs/bpf"
#define CRUN_BPF_DIR SYS_FS_BPF "/crun"

ସଂରଚନା bpf_program;

ସଂରଚନା bpf_program *bpf_program_new (size_t size);
ସଂରଚନା bpf_program *bpf_program_append (ସଂରଚନା bpf_program *p, ଶୂନ୍ୟ *data, size_t size);

ସଂରଚନା bpf_program *bpf_program_init_dev (ସଂରଚନା bpf_program *program, libcrun_error_t *err);
ସଂରଚନା bpf_program *bpf_program_append_dev (ସଂରଚନା bpf_program *program, ଅଚଳ ଅକ୍ଷର *access, ଅକ୍ଷର type, ପୂର୍ଣ୍ଣସଂଖ୍ୟା major,
                                            ପୂର୍ଣ୍ଣସଂଖ୍ୟା minor, bool accept, libcrun_error_t *err);
ସଂରଚନା bpf_program *bpf_program_complete_dev (ସଂରଚନା bpf_program *program, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_ebpf_load (ସଂରଚନା bpf_program *program, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *pin, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_ebpf_read_program (ସଂରଚନା bpf_program **program, ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_ebpf_query_cgroup_progs (ଅଚଳ ଅକ୍ଷର *cgroup_path, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err);
bool libcrun_ebpf_cmp_programs (ସଂରଚନା bpf_program *program1, ସଂରଚନା bpf_program *program2);

#endif
