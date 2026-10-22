/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019 Giuseppe Scrivano <giuseppe@scrivano.org>
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
#ifndef CGROUP_RESOURCES_H
#define CGROUP_RESOURCES_H

#അന്തര്ഭാവ "container.h"
#അന്തര്ഭാവ "cgroup.h"
#അന്തര്ഭാവ <unistd.h>

ഘടന default_dev_s
{
  അക്ഷരം type;
  പൂർണ്ണസംഖ്യ major;
  പൂർണ്ണസംഖ്യ minor;
  സ്ഥിരം അക്ഷരം *access;
};
ഘടന default_dev_s *get_default_devices ();

പൂർണ്ണസംഖ്യ check_memory_before_update (സ്ഥിരം അക്ഷരം *path, runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err);

പൂർണ്ണസംഖ്യ update_cgroup_resources (സ്ഥിരം അക്ഷരം *path,
                             സ്ഥിരം അക്ഷരം *state_വേര്,
                             runtime_spec_schema_config_linux_resources *resources,
                             bool need_devices,
                             libcrun_error_t *err);

ഘടന bpf_program *create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                    libcrun_error_t *err);

#endif
