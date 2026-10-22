/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019, 2020 Giuseppe Scrivano <giuseppe@scrivano.org>
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
#جيڪڏهن_وصف_نه_ٿيل SPEC_H
#وصف_ڪريو SPEC_H

#ضمن <config.h>
#ضمن <stdbool.h>
#ضمن <stdio.h>
#ضمن "error.h"

LIBCRUN_عوامي صحيح libcrun_container_spec (bool پاڙ, FILE *ٻاهريون, libcrun_error_t *err);

LIBCRUN_عوامي صحيح libcrun_container_spec_json (bool پاڙ, حرف **ٻاهريون, libcrun_error_t *err);

#جيڪڏهن_پورو
