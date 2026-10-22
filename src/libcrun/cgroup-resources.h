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
#اگر_غیر_تعریف_شدہ CGROUP_RESOURCES_H
#تعریف_کرو CGROUP_RESOURCES_H

#شامل "container.h"
#شامل "cgroup.h"
#شامل <unistd.h>

ساخت default_dev_s
{
  حرف type;
  صحیح major;
  صحیح minor;
  ثابت حرف *access;
};
ساخت default_dev_s *get_default_devices ();

صحیح check_حافظہ_before_update (ثابت حرف *راستہ, runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err);

صحیح update_cgroup_resources (ثابت حرف *راستہ,
                             ثابت حرف *state_جڑ,
                             runtime_spec_schema_config_linux_resources *resources,
                             bool need_devices,
                             libcrun_error_t *err);

ساخت bpf_پروگرام *create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                    libcrun_error_t *err);

#اگر_ختم
