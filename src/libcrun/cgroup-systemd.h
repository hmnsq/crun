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

#অন্তর্ভাৱ "container.h"
#অন্তর্ভাৱ <unistd.h>

#ifdef HAVE_SYSTEMD
বাহ্য পূর্ণাঙ্ক parse_sd_array (ৱর্ণ *s, ৱর্ণ **out, ৱর্ণ **next, libcrun_error_t *err);

বাহ্য ৱর্ণ *get_cgroup_scope_path (ধ্রুৱ ৱর্ণ *cgroup_path, ধ্রুৱ ৱর্ণ *scope);
#endif

বাহ্য সংরচনা libcrun_cgroup_manager cgroup_manager_systemd;

#endif
