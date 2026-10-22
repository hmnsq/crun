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

#अन्तर्भाव "container.h"
#अन्तर्भाव "cgroup.h"
#अन्तर्भाव <unistd.h>

संरचना default_dev_s
{
  अक्षर type;
  पूर्णांक major;
  पूर्णांक minor;
  स्थिरांक अक्षर *access;
};
संरचना default_dev_s *get_default_devices ();

पूर्णांक check_memory_before_update (स्थिरांक अक्षर *path, runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err);

पूर्णांक update_cgroup_resources (स्थिरांक अक्षर *path,
                             स्थिरांक अक्षर *state_मूळ,
                             runtime_spec_schema_config_linux_resources *resources,
                             bool need_devices,
                             libcrun_error_t *err);

संरचना bpf_program *create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, माप_प्रकार devs_len,
                                    libcrun_error_t *err);

#endif
