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

#अन्तर्भाव "container.h"
#अन्तर्भाव <unistd.h>

#ifdef HAVE_SYSTEMD
बाह्य पूर्णाङ्क parse_sd_array (वर्ण *s, वर्ण **out, वर्ण **next, libcrun_error_t *err);

बाह्य वर्ण *get_cgroup_scope_path (ध्रुव वर्ण *cgroup_path, ध्रुव वर्ण *scope);
#endif

बाह्य संरचना libcrun_cgroup_manager cgroup_manager_systemd;

#endif
