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
#اگر_تعریف_نشده پیاپی_CREATE_H
#تعریف_کن پیاپی_CREATE_H

#ضمن "crun.h"

تعريفنوع صحيح (*container_پیاپی_create_func_t) (libcrun_context_t *, libcrun_container_t *, بیعلامت صحيح, libcrun_error_t *);
تعريفنوع بیعلامت صحيح (*get_options_func_t) ();

صحيح crun_پیاپی_create_داخلی (بنية crun_global_arguments *global_آرگومانها, صحيح argc, حرف **argv,
                              container_پیاپی_create_func_t container_پیاپی_create_func, get_options_func_t get_options_func,
                              libcrun_context_t *crun_context, بنية argp *پیاپی_argp, ثابت حرف **config_file,
                              ثابت حرف **bundle, libcrun_error_t *err);

#پایان_اگر
