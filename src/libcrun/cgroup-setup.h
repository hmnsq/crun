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
#अगर_अघोषित CGROUP_SETUP_H
#घोषणा CGROUP_SETUP_H

#समावेश "container.h"
#समावेश "utils.h"

पूर्णांक enter_cgroup (पूर्णांक cgroup_mode, pid_t pid, pid_t init_pid, स्थिर अक्षर *path,
                  bool create_if_missing, libcrun_error_t *err);

#पूर्ण_अगर
