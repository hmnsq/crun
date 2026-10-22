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
#که_نه_تعریف_شوی EBPF_H
#تعریف_کړه EBPF_H

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن "error.h"
#ضمن <errno.h>
#ضمن <argp.h>
#ضمن <ocispec/runtime_spec_schema_config_schema.h>
#ضمن "container.h"

#تعریف_کړه SYS_FS_BPF "/sys/fs/bpf"
#تعریف_کړه CRUN_BPF_DIR SYS_FS_BPF "/crun"

بنية bpf_پروګرام;

بنية bpf_پروګرام *bpf_پروګرام_new (size_t اندازه);
بنية bpf_پروګرام *bpf_پروګرام_append (بنية bpf_پروګرام *p, فراغ *معلومات, size_t اندازه);

بنية bpf_پروګرام *bpf_پروګرام_init_dev (بنية bpf_پروګرام *پروګرام, libcrun_error_t *err);
بنية bpf_پروګرام *bpf_پروګرام_append_dev (بنية bpf_پروګرام *پروګرام, ثابت حرف *access, حرف type, صحيح major,
                                            صحيح minor, bool accept, libcrun_error_t *err);
بنية bpf_پروګرام *bpf_پروګرام_complete_dev (بنية bpf_پروګرام *پروګرام, libcrun_error_t *err);

صحيح libcrun_ebpf_بارول (بنية bpf_پروګرام *پروګرام, صحيح dirfd, ثابت حرف *pin, libcrun_error_t *err);
صحيح libcrun_ebpf_read_پروګرام (بنية bpf_پروګرام **پروګرام, ثابت حرف *لار, libcrun_error_t *err);
صحيح libcrun_ebpf_query_cgroup_progs (ثابت حرف *cgroup_لار, uint32_t **progs_بهر, size_t *n_progs_بهر, libcrun_error_t *err);
bool libcrun_ebpf_cmp_programs (بنية bpf_پروګرام *program1, بنية bpf_پروګرام *program2);

#د_که_پای
