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
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ EBPF_H
#ವ್ಯಾಖ್ಯಾನಿಸು EBPF_H

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ "error.h"
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <argp.h>
#ಅನ್ತರ್ಭಾವ <ocispec/runtime_spec_schema_config_schema.h>
#ಅನ್ತರ್ಭಾವ "container.h"

#ವ್ಯಾಖ್ಯಾನಿಸು SYS_FS_BPF "/sys/fs/bpf"
#ವ್ಯಾಖ್ಯಾನಿಸು CRUN_BPF_DIR SYS_FS_BPF "/crun"

ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ;

ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *bpf_ಕಾರ್ಯಕ್ರಮ_new (size_t ಗಾತ್ರ_2);
ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *bpf_ಕಾರ್ಯಕ್ರಮ_append (ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *p, ಶೂನ್ಯ *ದತ್ತಾಂಶ, size_t ಗಾತ್ರ_2);

ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *bpf_ಕಾರ್ಯಕ್ರಮ_init_dev (ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *ಕಾರ್ಯಕ್ರಮ, libcrun_error_t *err);
ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *bpf_ಕಾರ್ಯಕ್ರಮ_append_dev (ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *ಕಾರ್ಯಕ್ರಮ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *access, ಅಕ್ಷರ type, ಪೂರ್ಣಾಂಕ major,
                                            ಪೂರ್ಣಾಂಕ minor, bool accept, libcrun_error_t *err);
ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *bpf_ಕಾರ್ಯಕ್ರಮ_complete_dev (ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *ಕಾರ್ಯಕ್ರಮ, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_ebpf_ತುಂಬು (ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *ಕಾರ್ಯಕ್ರಮ, ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pin, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_ebpf_read_ಕಾರ್ಯಕ್ರಮ (ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ **ಕಾರ್ಯಕ್ರಮ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_ebpf_query_cgroup_progs (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ, uint32_t **progs_ಹೊರಹೋಗು, size_t *n_progs_ಹೊರಹೋಗು, libcrun_error_t *err);
bool libcrun_ebpf_cmp_programs (ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *program1, ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *program2);

#ಆದರೆ_ಅಂತ್ಯ
