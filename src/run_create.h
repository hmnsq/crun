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
#నిర్వచితం_కాకపోతే నిరంతరం_CREATE_H
#నిర్వచించు నిరంతరం_CREATE_H

#అన్తర్భావ "crun.h"

రకనిర్వచనం పూర్ణాంకం (*container_నిరంతరం_create_func_t) (libcrun_context_t *, libcrun_container_t *, గుర్తులేని పూర్ణాంకం, libcrun_error_t *);
రకనిర్వచనం గుర్తులేని పూర్ణాంకం (*get_options_func_t) ();

పూర్ణాంకం crun_నిరంతరం_create_అంతర్గత (నిర్మాణం crun_global_arguments *global_వాదనలు, పూర్ణాంకం argc, అక్షరం **argv,
                              container_నిరంతరం_create_func_t container_నిరంతరం_create_func, get_options_func_t get_options_func,
                              libcrun_context_t *crun_context, నిర్మాణం argp *నిరంతరం_argp, స్థిరరాశి అక్షరం **config_file,
                              స్థిరరాశి అక్షరం **bundle, libcrun_error_t *err);

#అయితే_ముగింపు
