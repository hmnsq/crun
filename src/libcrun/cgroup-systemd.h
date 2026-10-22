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

#অন্তর্ভাব "container.h"
#অন্তর্ভাব <unistd.h>

#ifdef HAVE_SYSTEMD
বাহ্যিক পূর্ণসংখ্যা parse_sd_array (অক্ষর *s, অক্ষর **out, অক্ষর **পরবর্তী, libcrun_error_t *err);

বাহ্যিক অক্ষর *get_cgroup_scope_path (ধ্রুবক অক্ষর *cgroup_path, ধ্রুবক অক্ষর *scope);
#endif

বাহ্যিক কাঠামো libcrun_cgroup_manager cgroup_manager_systemd;

#endif
