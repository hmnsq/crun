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
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CGROUP_SYSTEMD_H
#ವ್ಯಾಖ್ಯಾನಿಸು CGROUP_SYSTEMD_H

#ಅನ್ತರ್ಭಾವ "container.h"
#ಅನ್ತರ್ಭಾವ <unistd.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYSTEMD
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ parse_sd_array (ಅಕ್ಷರ *s, ಅಕ್ಷರ **ಹೊರಹೋಗು, ಅಕ್ಷರ **ಮುಂದಿನ, libcrun_error_t *err);

ಬಾಹ್ಯ ಅಕ್ಷರ *get_cgroup_scope_ಮಾರ್ಗ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *scope);
#ಆದರೆ_ಅಂತ್ಯ

ಬಾಹ್ಯ ರಚನೆ libcrun_cgroup_manager cgroup_manager_systemd;

#ಆದರೆ_ಅಂತ್ಯ
