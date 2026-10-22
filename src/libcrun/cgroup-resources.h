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
#నిర్వచితం_కాకపోతే CGROUP_RESOURCES_H
#నిర్వచించు CGROUP_RESOURCES_H

#అన్తర్భావ "container.h"
#అన్తర్భావ "cgroup.h"
#అన్తర్భావ <unistd.h>

నిర్మాణం default_dev_s
{
  అక్షరం type;
  పూర్ణాంకం major;
  పూర్ణాంకం minor;
  స్థిరరాశి అక్షరం *access;
};
నిర్మాణం default_dev_s *get_default_devices ();

పూర్ణాంకం check_జ్ఞాపకం_before_update (స్థిరరాశి అక్షరం *మార్గం, runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err);

పూర్ణాంకం update_cgroup_resources (స్థిరరాశి అక్షరం *మార్గం,
                             స్థిరరాశి అక్షరం *state_మూలం,
                             runtime_spec_schema_config_linux_resources *resources,
                             bool need_devices,
                             libcrun_error_t *err);

నిర్మాణం bpf_కార్యక్రమం *create_dev_bpf (runtime_spec_schema_defs_linux_device_cgroup **devs, size_t devs_len,
                                    libcrun_error_t *err);

#అయితే_ముగింపు
