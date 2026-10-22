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

#include "container.h"
#include "cgroup.h"
#include <unistd.h>

sa_nracanaa default_dev_s
{
  var_na type;
  පූර්ණ_සංඛ්‍යා major;
  පූර්ණ_සංඛ්‍යා minor;
  const var_na *access;
};
sa_nracanaa default_dev_s *get_default_devices ();

පූර්ණ_සංඛ්‍යා check_memory_before_update (const var_na *path, runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err);

පූර්ණ_සංඛ්‍යා update_cgroup_resources (const var_na *path,
                             const var_na *state_root,
                             runtime_spec_schema_config_linux_resources *resources,
                             bool need_devices,
                             libcrun_error_t *err);

sa_nracanaa bpf_program *create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                    libcrun_error_t *err);

#endif
