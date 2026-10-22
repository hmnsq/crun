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
#अगर_अघोषित CGROUP_RESOURCES_H
#घोषणा CGROUP_RESOURCES_H

#समावेश "container.h"
#समावेश "cgroup.h"
#समावेश <unistd.h>

काष्ठा default_dev_s
{
  अक्षर type;
  पूर्णांक major;
  पूर्णांक minor;
  स्थिर अक्षर *access;
};
काष्ठा default_dev_s *get_default_devices ();

पूर्णांक check_memory_before_update (स्थिर अक्षर *path, runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err);

पूर्णांक update_cgroup_resources (स्थिर अक्षर *path,
                             स्थिर अक्षर *state_मूल,
                             runtime_spec_schema_config_linux_resources *resources,
                             bool need_devices,
                             libcrun_error_t *err);

काष्ठा bpf_program *create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, माप_प्रकार devs_len,
                                    libcrun_error_t *err);

#पूर्ण_अगर
