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

#ಅನ್ತರ್ಭಾವ "crun.h"

ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ (*container_run_create_func_t) (libcrun_context_t *, libcrun_container_t *, unsigned ಪೂರ್ಣಾಙ್ಕ, libcrun_error_t *);
ಪ್ರಕಾರನಿರ್ದೇಶ unsigned ಪೂರ್ಣಾಙ್ಕ (*get_options_func_t) ();

ಪೂರ್ಣಾಙ್ಕ crun_run_create_internal (ಸಂರಚನಾ crun_global_arguments *global_args, ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv,
                              container_run_create_func_t container_run_create_func, get_options_func_t get_options_func,
                              libcrun_context_t *crun_context, ಸಂರಚನಾ argp *run_argp, ಧ್ರುವ ವರ್ಣ **config_file,
                              ಧ್ರುವ ವರ್ಣ **bundle, libcrun_error_t *err);

#endif
