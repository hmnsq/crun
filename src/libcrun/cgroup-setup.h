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
#यदि_अपरिभाषितम् CGROUP_सज्जता_H
#परिभाषय CGROUP_सज्जता_H

#अन्तर्भाव "container.h"
#अन्तर्भाव "utils.h"

पूर्णाङ्क enter_cgroup (पूर्णाङ्क cgroup_बहुलकम्, pid_t pid, pid_t init_pid, ध्रुव वर्ण *मार्गः,
                  bool create_if_missing, libcrun_error_t *err);

#यदि_अन्तः
