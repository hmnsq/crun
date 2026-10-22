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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ EBPF_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EBPF_H

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "error.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <argp.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ocispec/runtime_spec_schema_config_schema.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "container.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYS_FS_BPF "/sys/fs/bpf"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CRUN_BPF_DIR SYS_FS_BPF "/crun"

ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ;

ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *bpf_ਪ੍ਰੋਗਰਾਮ_new (size_t ਮਾਪ);
ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *bpf_ਪ੍ਰੋਗਰਾਮ_append (ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *p, ਖਾਲੀ *ਅੰਕੜੇ, size_t ਮਾਪ);

ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *bpf_ਪ੍ਰੋਗਰਾਮ_init_dev (ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *ਪ੍ਰੋਗਰਾਮ, libcrun_error_t *err);
ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *bpf_ਪ੍ਰੋਗਰਾਮ_append_dev (ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *ਪ੍ਰੋਗਰਾਮ, ਅਚਲ ਅੱਖਰ *access, ਅੱਖਰ type, ਪੂਰਨ major,
                                            ਪੂਰਨ minor, bool accept, libcrun_error_t *err);
ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *bpf_ਪ੍ਰੋਗਰਾਮ_complete_dev (ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *ਪ੍ਰੋਗਰਾਮ, libcrun_error_t *err);

ਪੂਰਨ libcrun_ebpf_ਲੱਦੋ (ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *ਪ੍ਰੋਗਰਾਮ, ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *pin, libcrun_error_t *err);
ਪੂਰਨ libcrun_ebpf_read_ਪ੍ਰੋਗਰਾਮ (ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ **ਪ੍ਰੋਗਰਾਮ, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err);
ਪੂਰਨ libcrun_ebpf_query_cgroup_progs (ਅਚਲ ਅੱਖਰ *cgroup_ਰਸਤਾ, uint32_t **progs_ਬਾਹਰਲਾ, size_t *n_progs_ਬਾਹਰਲਾ, libcrun_error_t *err);
bool libcrun_ebpf_cmp_programs (ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *program1, ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *program2);

#ਜੇ_ਅੰਤ
