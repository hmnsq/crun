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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਲੜੀਵਾਰ_CREATE_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਲੜੀਵਾਰ_CREATE_H

#ਅਨ੍ਤਰ੍ਭਾਵ "crun.h"

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ (*container_ਲੜੀਵਾਰ_create_func_t) (libcrun_context_t *, libcrun_container_t *, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ, libcrun_error_t *);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ (*get_options_func_t) ();

ਪੂਰਨ crun_ਲੜੀਵਾਰ_create_ਅੰਦਰੂਨੀ (ਬਣਤਰ crun_global_arguments *global_ਦਲੀਲਾਂ, ਪੂਰਨ argc, ਅੱਖਰ **argv,
                              container_ਲੜੀਵਾਰ_create_func_t container_ਲੜੀਵਾਰ_create_func, get_options_func_t get_options_func,
                              libcrun_context_t *crun_context, ਬਣਤਰ argp *ਲੜੀਵਾਰ_argp, ਅਚਲ ਅੱਖਰ **config_file,
                              ਅਚਲ ਅੱਖਰ **bundle, libcrun_error_t *err);

#ਜੇ_ਅੰਤ
