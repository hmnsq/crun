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
#جيڪڏهن_وصف_نه_ٿيل CGROUP_SYSTEMD_H
#وصف_ڪريو CGROUP_SYSTEMD_H

#ضمن "container.h"
#ضمن <unistd.h>

#جيڪڏهن_وصف_ٿيل HAVE_SYSTEMD
خارجي صحيح parse_sd_array (حرف *s, حرف **ٻاهريون, حرف **ايندڙ, libcrun_error_t *err);

خارجي حرف *get_cgroup_scope_رستو (ثابت حرف *cgroup_رستو, ثابت حرف *scope);
#جيڪڏهن_پورو

خارجي بنية libcrun_cgroup_manager cgroup_manager_systemd;

#جيڪڏهن_پورو
