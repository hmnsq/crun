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
#నిర్వచితం_కాకపోతే SPEC_H
#నిర్వచించు SPEC_H

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdbool.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ "error.h"

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_spec (bool మూలం, FILE *బహిర్గతం, libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_spec_json (bool మూలం, అక్షరం **బహిర్గతం, libcrun_error_t *err);

#అయితే_ముగింపు
