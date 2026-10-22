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
#अगर_अघोषित CGROUP_SYSTEMD_H
#घोषणा CGROUP_SYSTEMD_H

#समावेश "container.h"
#समावेश <unistd.h>

#अगर_घोषित HAVE_SYSTEMD
बाह्य पूर्णांक parse_sd_array (अक्षर *s, अक्षर **out, अक्षर **अग्र, libcrun_error_t *err);

बाह्य अक्षर *get_cgroup_scope_path (स्थिर अक्षर *cgroup_path, स्थिर अक्षर *scope);
#पूर्ण_अगर

बाह्य काष्ठा libcrun_cgroup_manager cgroup_manager_systemd;

#पूर्ण_अगर
