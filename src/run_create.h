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

#அந்தர்பாவ "crun.h"

வகைவரை முழு (*container_run_create_func_t) (libcrun_context_t *, libcrun_container_t *, unsigned முழு, libcrun_error_t *);
வகைவரை unsigned முழு (*get_options_func_t) ();

முழு crun_run_create_internal (கட்டமைப்பு crun_global_arguments *global_args, முழு argc, எழுத்து **argv,
                              container_run_create_func_t container_run_create_func, get_options_func_t get_options_func,
                              libcrun_context_t *crun_context, கட்டமைப்பு argp *run_argp, மாறிலி எழுத்து **config_file,
                              மாறிலி எழுத்து **bundle, libcrun_error_t *err);

#endif
