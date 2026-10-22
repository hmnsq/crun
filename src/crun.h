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
#నిర్వచితం_కాకపోతే CRUN_CLI_H
#నిర్వచించు CRUN_CLI_H

#అన్తర్భావ <stdbool.h>
#అన్తర్భావ <libcrun/libcrun.h>
#అన్తర్భావ "crun_utils.h"

నిర్మాణం crun_global_arguments
{
  అక్షరం *మూలం;
  అక్షరం *సంవర్గమానం;
  అక్షరం *log_format;
  స్థిరరాశి అక్షరం *handler;

  పూర్ణాంకం argc;
  పూర్ణాంకం verbosity;
  అక్షరం **argv;

  bool command;
  bool option_systemd_cgroup;
  bool option_force_no_cgroup;
};

అక్షరం *argp_mandatory_వాదం (అక్షరం *arg, నిర్మాణం argp_state *state);
పూర్ణాంకం parse_id_or_fail (స్థిరరాశి అక్షరం *str, అక్షరం **endptr, స్థిరరాశి అక్షరం *రకం);
పూర్ణాంకం init_libcrun_context (libcrun_context_t *con, స్థిరరాశి అక్షరం *id, నిర్మాణం crun_global_arguments *glob,
                          libcrun_error_t *err);
libcrun_context_t *new_libcrun_context (నిర్మాణం crun_global_arguments *glob);
శూన్య crun_assert_n_వాదనలు (పూర్ణాంకం n, పూర్ణాంకం min, పూర్ణాంకం max);
#అయితే_ముగింపు
