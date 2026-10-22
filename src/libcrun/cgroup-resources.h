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
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CGROUP_RESOURCES_H
#ವ್ಯಾಖ್ಯಾನಿಸು CGROUP_RESOURCES_H

#ಅನ್ತರ್ಭಾವ "container.h"
#ಅನ್ತರ್ಭಾವ "cgroup.h"
#ಅನ್ತರ್ಭಾವ <unistd.h>

ರಚನೆ default_dev_s
{
  ಅಕ್ಷರ type;
  ಪೂರ್ಣಾಂಕ major;
  ಪೂರ್ಣಾಂಕ minor;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *access;
};
ರಚನೆ default_dev_s *get_default_devices ();

ಪೂರ್ಣಾಂಕ check_ಸ್ಮರಣೆ_before_update (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ update_cgroup_resources (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ,
                             ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು,
                             runtime_spec_schema_config_linux_resources *resources,
                             bool need_devices,
                             libcrun_error_t *err);

ರಚನೆ bpf_ಕಾರ್ಯಕ್ರಮ *create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                    libcrun_error_t *err);

#ಆದರೆ_ಅಂತ್ಯ
