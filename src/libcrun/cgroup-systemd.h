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

#અન્તર્ભાવ "container.h"
#અન્તર્ભાવ <unistd.h>

#ifdef HAVE_SYSTEMD
બાહ્ય પૂર્ણાંક parse_sd_array (અક્ષર *s, અક્ષર **out, અક્ષર **next, libcrun_error_t *err);

બાહ્ય અક્ષર *get_cgroup_scope_path (અચલ અક્ષર *cgroup_path, અચલ અક્ષર *scope);
#endif

બાહ્ય સંરચના libcrun_cgroup_manager cgroup_manager_systemd;

#endif
