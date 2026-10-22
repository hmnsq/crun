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
#اگر_غیر_تعریف_شدہ SPEC_H
#تعریف_کرو SPEC_H

#شامل <config.h>
#شامل <stdbool.h>
#شامل <stdio.h>
#شامل "error.h"

LIBCRUN_عوامی صحیح libcrun_container_spec (bool جڑ, FILE *برآمد, libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_container_spec_json (bool جڑ, حرف **برآمد, libcrun_error_t *err);

#اگر_ختم
