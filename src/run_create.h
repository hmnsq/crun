/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019 Giuseppe Scrivano <giuseppe@scrivano.org>
 * crun is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * crun is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with crun.  If not, see <http://www.gnu.org/licenses/>.
 */
#جيڪڏهن_وصف_نه_ٿيل لاڳيتو_CREATE_H
#وصف_ڪريو لاڳيتو_CREATE_H

#ضمن "crun.h"

تعريفنوع صحيح (*container_لاڳيتو_create_func_t) (libcrun_context_t *, libcrun_container_t *, بنانشان صحيح, libcrun_error_t *);
تعريفنوع بنانشان صحيح (*get_options_func_t) ();

صحيح crun_لاڳيتو_create_اندروني (بنية crun_global_arguments *global_دليلون, صحيح argc, حرف **argv,
                              container_لاڳيتو_create_func_t container_لاڳيتو_create_func, get_options_func_t get_options_func,
                              libcrun_context_t *crun_context, بنية argp *لاڳيتو_argp, ثابت حرف **config_file,
                              ثابت حرف **bundle, libcrun_error_t *err);

#جيڪڏهن_پورو
