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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CGROUP_SYSTEMD_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CGROUP_SYSTEMD_H

#ਅਨ੍ਤਰ੍ਭਾਵ "container.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYSTEMD
ਬਾਹਰੀ ਪੂਰਨ parse_sd_array (ਅੱਖਰ *s, ਅੱਖਰ **ਬਾਹਰਲਾ, ਅੱਖਰ **ਅਗਲਾ, libcrun_error_t *err);

ਬਾਹਰੀ ਅੱਖਰ *get_cgroup_scope_ਰਸਤਾ (ਅਚਲ ਅੱਖਰ *cgroup_ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *scope);
#ਜੇ_ਅੰਤ

ਬਾਹਰੀ ਬਣਤਰ libcrun_cgroup_manager cgroup_manager_systemd;

#ਜੇ_ਅੰਤ
