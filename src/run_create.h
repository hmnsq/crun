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

#include "crun.h"

typedef số_nguyên (*container_run_create_func_t) (libcrun_context_t *, libcrun_container_t *, unsigned số_nguyên, libcrun_error_t *);
typedef unsigned số_nguyên (*get_options_func_t) ();

số_nguyên crun_run_create_internal (struct crun_global_arguments *global_args, số_nguyên argc, char **argv,
                              container_run_create_func_t container_run_create_func, get_options_func_t get_options_func,
                              libcrun_context_t *crun_context, struct argp *run_argp, const char **config_file,
                              const char **bundle, libcrun_error_t *err);

#endif
