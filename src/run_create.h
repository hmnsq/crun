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
#ifndef RUN_CREATE_H
#define RUN_CREATE_H

#अन्तर्भाव "crun.h"

प्रकारव्याख्या पूर्णांक (*container_run_create_func_t) (libcrun_context_t *, libcrun_container_t *, अचिन्हित पूर्णांक, libcrun_error_t *);
प्रकारव्याख्या अचिन्हित पूर्णांक (*get_options_func_t) ();

पूर्णांक crun_run_create_internal (संरचना crun_global_arguments *global_args, पूर्णांक argc, अक्षर **argv,
                              container_run_create_func_t container_run_create_func, get_options_func_t get_options_func,
                              libcrun_context_t *crun_context, संरचना argp *run_argp, स्थिरांक अक्षर **config_file,
                              स्थिरांक अक्षर **bundle, libcrun_error_t *err);

#endif
