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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CGROUP_ਤਿਆਰੀ_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CGROUP_ਤਿਆਰੀ_H

#ਅਨ੍ਤਰ੍ਭਾਵ "container.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"

ਪੂਰਨ enter_cgroup (ਪੂਰਨ cgroup_ਬਹੁਲਕ, pid_t pid, pid_t init_pid, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ,
                  bool create_if_missing, libcrun_error_t *err);

#ਜੇ_ਅੰਤ
