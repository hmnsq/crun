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
#که_نه_تعریف_شوی CGROUP_SYSTEMD_H
#تعریف_کړه CGROUP_SYSTEMD_H

#ضمن "container.h"
#ضمن <unistd.h>

#که_تعریف_شوی HAVE_SYSTEMD
خارجي صحيح parse_sd_array (حرف *s, حرف **بهر, حرف **بل, libcrun_error_t *err);

خارجي حرف *get_cgroup_scope_لار (ثابت حرف *cgroup_لار, ثابت حرف *scope);
#د_که_پای

خارجي بنية libcrun_cgroup_manager cgroup_manager_systemd;

#د_که_پای
