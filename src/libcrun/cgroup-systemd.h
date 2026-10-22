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

#شامل "container.h"
#شامل <unistd.h>

#ifdef HAVE_SYSTEMD
بیرونی صحیح parse_sd_array (نویسه *s, نویسه **out, نویسه **next, libcrun_error_t *err);

بیرونی نویسه *get_cgroup_scope_path (ثابت نویسه *cgroup_path, ثابت نویسه *scope);
#endif

بیرونی ساختار libcrun_cgroup_manager cgroup_manager_systemd;

#endif
