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

प्रकारनिर्देश पूर्णाङ्क (*container_run_create_func_t) (libcrun_context_t *, libcrun_container_t *, अचिन्हित पूर्णाङ्क, libcrun_error_t *);
प्रकारनिर्देश अचिन्हित पूर्णाङ्क (*get_options_func_t) ();

पूर्णाङ्क crun_run_create_internal (संरचना crun_global_arguments *global_args, पूर्णाङ्क argc, वर्ण **argv,
                              container_run_create_func_t container_run_create_func, get_options_func_t get_options_func,
                              libcrun_context_t *crun_context, संरचना argp *run_argp, ध्रुव वर्ण **config_file,
                              ध्रुव वर्ण **bundle, libcrun_error_t *err);

#endif
