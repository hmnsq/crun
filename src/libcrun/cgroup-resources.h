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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CGROUP_RESOURCES_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CGROUP_RESOURCES_H

#ਅਨ੍ਤਰ੍ਭਾਵ "container.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cgroup.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>

ਬਣਤਰ default_dev_s
{
  ਅੱਖਰ type;
  ਪੂਰਨ major;
  ਪੂਰਨ minor;
  ਅਚਲ ਅੱਖਰ *access;
};
ਬਣਤਰ default_dev_s *get_default_devices ();

ਪੂਰਨ check_ਸਮ੍ਰਿਤੀ_before_update (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err);

ਪੂਰਨ update_cgroup_resources (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ,
                             ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ,
                             runtime_spec_schema_config_linux_resources *resources,
                             bool need_devices,
                             libcrun_error_t *err);

ਬਣਤਰ bpf_ਪ੍ਰੋਗਰਾਮ *create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                    libcrun_error_t *err);

#ਜੇ_ਅੰਤ
