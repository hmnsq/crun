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

#அந்தர்பாவ "container.h"
#அந்தர்பாவ "cgroup.h"
#அந்தர்பாவ <unistd.h>

கட்டமைப்பு default_dev_s
{
  எழுத்து type;
  முழு major;
  முழு minor;
  மாறிலி எழுத்து *access;
};
கட்டமைப்பு default_dev_s *get_default_devices ();

முழு check_memory_before_update (மாறிலி எழுத்து *path, runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err);

முழு update_cgroup_resources (மாறிலி எழுத்து *path,
                             மாறிலி எழுத்து *state_வேர்,
                             runtime_spec_schema_config_linux_resources *resources,
                             bool need_devices,
                             libcrun_error_t *err);

கட்டமைப்பு bpf_program *create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                    libcrun_error_t *err);

#endif
