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
#اگر_غیر_تعریف_شدہ EBPF_H
#تعریف_کرو EBPF_H

#شامل <config.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل "error.h"
#شامل <errno.h>
#شامل <argp.h>
#شامل <ocispec/runtime_spec_schema_config_schema.h>
#شامل "container.h"

#تعریف_کرو SYS_FS_BPF "/sys/fs/bpf"
#تعریف_کرو CRUN_BPF_DIR SYS_FS_BPF "/crun"

ساخت bpf_پروگرام;

ساخت bpf_پروگرام *bpf_پروگرام_new (size_t سائز);
ساخت bpf_پروگرام *bpf_پروگرام_append (ساخت bpf_پروگرام *p, خالی *ڈیٹا, size_t سائز);

ساخت bpf_پروگرام *bpf_پروگرام_init_dev (ساخت bpf_پروگرام *پروگرام, libcrun_error_t *err);
ساخت bpf_پروگرام *bpf_پروگرام_append_dev (ساخت bpf_پروگرام *پروگرام, ثابت حرف *access, حرف type, صحیح major,
                                            صحیح minor, bool accept, libcrun_error_t *err);
ساخت bpf_پروگرام *bpf_پروگرام_complete_dev (ساخت bpf_پروگرام *پروگرام, libcrun_error_t *err);

صحیح libcrun_ebpf_بھرو (ساخت bpf_پروگرام *پروگرام, صحیح dirfd, ثابت حرف *pin, libcrun_error_t *err);
صحیح libcrun_ebpf_read_پروگرام (ساخت bpf_پروگرام **پروگرام, ثابت حرف *راستہ, libcrun_error_t *err);
صحیح libcrun_ebpf_query_cgroup_progs (ثابت حرف *cgroup_راستہ, uint32_t **progs_برآمد, size_t *n_progs_برآمد, libcrun_error_t *err);
bool libcrun_ebpf_cmp_programs (ساخت bpf_پروگرام *program1, ساخت bpf_پروگرام *program2);

#اگر_ختم
