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
#ifndef CGROUP_SYSTEMD_H
#define CGROUP_SYSTEMD_H

#ಅನ್ತರ್ಭಾವ "container.h"
#ಅನ್ತರ್ಭಾವ <unistd.h>

#ifdef HAVE_SYSTEMD
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ parse_sd_array (ವರ್ಣ *s, ವರ್ಣ **out, ವರ್ಣ **next, libcrun_error_t *err);

ಬಾಹ್ಯ ವರ್ಣ *get_cgroup_scope_path (ಧ್ರುವ ವರ್ಣ *cgroup_path, ಧ್ರುವ ವರ್ಣ *scope);
#endif

ಬಾಹ್ಯ ಸಂರಚನಾ libcrun_cgroup_manager cgroup_manager_systemd;

#endif
