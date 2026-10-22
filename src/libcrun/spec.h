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
#यदि_अपरिभाषितम् SPEC_H
#परिभाषय SPEC_H

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव "error.h"

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_spec (bool मूलम्, FILE *बहिर्गमनम्, libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_spec_json (bool मूलम्, वर्ण **बहिर्गमनम्, libcrun_error_t *err);

#यदि_अन्तः
