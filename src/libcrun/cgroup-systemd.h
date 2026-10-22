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

#ଅନ୍ତର୍ଭାଵ "container.h"
#ଅନ୍ତର୍ଭାଵ <unistd.h>

#ifdef HAVE_SYSTEMD
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_sd_array (ଅକ୍ଷର *s, ଅକ୍ଷର **out, ଅକ୍ଷର **next, libcrun_error_t *err);

ବାହ୍ୟ ଅକ୍ଷର *get_cgroup_scope_path (ଅଚଳ ଅକ୍ଷର *cgroup_path, ଅଚଳ ଅକ୍ଷର *scope);
#endif

ବାହ୍ୟ ସଂରଚନା libcrun_cgroup_manager cgroup_manager_systemd;

#endif
