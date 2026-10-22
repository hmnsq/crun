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
#جيڪڏهن_وصف_نه_ٿيل EBPF_H
#وصف_ڪريو EBPF_H

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن "error.h"
#ضمن <errno.h>
#ضمن <argp.h>
#ضمن <ocispec/runtime_spec_schema_config_schema.h>
#ضمن "container.h"

#وصف_ڪريو SYS_FS_BPF "/sys/fs/bpf"
#وصف_ڪريو CRUN_BPF_DIR SYS_FS_BPF "/crun"

بنية bpf_پروگرام;

بنية bpf_پروگرام *bpf_پروگرام_new (size_t سائيز);
بنية bpf_پروگرام *bpf_پروگرام_append (بنية bpf_پروگرام *p, فراغ *معلومات, size_t سائيز);

بنية bpf_پروگرام *bpf_پروگرام_init_dev (بنية bpf_پروگرام *پروگرام, libcrun_error_t *err);
بنية bpf_پروگرام *bpf_پروگرام_append_dev (بنية bpf_پروگرام *پروگرام, ثابت حرف *access, حرف type, صحيح major,
                                            صحيح minor, bool accept, libcrun_error_t *err);
بنية bpf_پروگرام *bpf_پروگرام_complete_dev (بنية bpf_پروگرام *پروگرام, libcrun_error_t *err);

صحيح libcrun_ebpf_ڀريو (بنية bpf_پروگرام *پروگرام, صحيح dirfd, ثابت حرف *pin, libcrun_error_t *err);
صحيح libcrun_ebpf_read_پروگرام (بنية bpf_پروگرام **پروگرام, ثابت حرف *رستو, libcrun_error_t *err);
صحيح libcrun_ebpf_query_cgroup_progs (ثابت حرف *cgroup_رستو, uint32_t **progs_ٻاهريون, size_t *n_progs_ٻاهريون, libcrun_error_t *err);
bool libcrun_ebpf_cmp_programs (بنية bpf_پروگرام *program1, بنية bpf_پروگرام *program2);

#جيڪڏهن_پورو
