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

#ଅନ୍ତର୍ଭାଵ "crun.h"

ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*container_run_create_func_t) (libcrun_context_t *, libcrun_container_t *, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା, libcrun_error_t *);
ପ୍ରକାରବ୍ୟାଖ୍ୟା unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*get_options_func_t) ();

ପୂର୍ଣ୍ଣସଂଖ୍ୟା crun_run_create_internal (ସଂରଚନା crun_global_arguments *global_args, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv,
                              container_run_create_func_t container_run_create_func, get_options_func_t get_options_func,
                              libcrun_context_t *crun_context, ସଂରଚନା argp *run_argp, ଅଚଳ ଅକ୍ଷର **config_file,
                              ଅଚଳ ଅକ୍ଷର **bundle, libcrun_error_t *err);

#endif
