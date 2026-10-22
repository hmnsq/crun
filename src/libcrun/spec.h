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
#ifndef SPEC_H
#define SPEC_H

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdbool.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব "error.h"

LIBCRUN_PUBLIC পূর্ণসংখ্যা libcrun_container_spec (bool মূল, FILE *out, libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণসংখ্যা libcrun_container_spec_json (bool মূল, অক্ষর **out, libcrun_error_t *err);

#endif
