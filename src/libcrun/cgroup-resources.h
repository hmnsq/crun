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

#ಅನ್ತರ್ಭಾವ "container.h"
#ಅನ್ತರ್ಭಾವ "cgroup.h"
#ಅನ್ತರ್ಭಾವ <unistd.h>

ಸಂರಚನಾ default_dev_s
{
  ವರ್ಣ type;
  ಪೂರ್ಣಾಙ್ಕ major;
  ಪೂರ್ಣಾಙ್ಕ minor;
  ಧ್ರುವ ವರ್ಣ *access;
};
ಸಂರಚನಾ default_dev_s *get_default_devices ();

ಪೂರ್ಣಾಙ್ಕ check_memory_before_update (ಧ್ರುವ ವರ್ಣ *path, runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ update_cgroup_resources (ಧ್ರುವ ವರ್ಣ *path,
                             ಧ್ರುವ ವರ್ಣ *state_ಬೇರು,
                             runtime_spec_schema_config_linux_resources *resources,
                             bool need_devices,
                             libcrun_error_t *err);

ಸಂರಚನಾ bpf_program *create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                    libcrun_error_t *err);

#endif
