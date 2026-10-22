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

#અન્તર્ભાવ "container.h"
#અન્તર્ભાવ "cgroup.h"
#અન્તર્ભાવ <unistd.h>

સંરચના default_dev_s
{
  અક્ષર type;
  પૂર્ણાંક major;
  પૂર્ણાંક minor;
  અચલ અક્ષર *access;
};
સંરચના default_dev_s *get_default_devices ();

પૂર્ણાંક check_memory_before_update (અચલ અક્ષર *path, runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err);

પૂર્ણાંક update_cgroup_resources (અચલ અક્ષર *path,
                             અચલ અક્ષર *state_મૂળ,
                             runtime_spec_schema_config_linux_resources *resources,
                             bool need_devices,
                             libcrun_error_t *err);

સંરચના bpf_program *create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                    libcrun_error_t *err);

#endif
