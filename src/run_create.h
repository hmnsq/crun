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

typedef puur_naa_nka (*container_run_create_func_t) (libcrun_context_t *, libcrun_container_t *, unsigned puur_naa_nka, libcrun_error_t *);
typedef unsigned puur_naa_nka (*get_options_func_t) ();

puur_naa_nka crun_run_create_internal (sa_nracanaa crun_global_arguments *global_args, puur_naa_nka argc, var_na **argv,
                              container_run_create_func_t container_run_create_func, get_options_func_t get_options_func,
                              libcrun_context_t *crun_context, sa_nracanaa argp *run_argp, const var_na **config_file,
                              const var_na **bundle, libcrun_error_t *err);

#endif
