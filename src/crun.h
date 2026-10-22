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
#ifndef CRUN_CLI_H
#define CRUN_CLI_H

#ضمن <stdbool.h>
#ضمن <libcrun/libcrun.h>
#ضمن "crun_utils.h"

بنية crun_global_arguments
{
  حرف *يىلتىز;
  حرف *log;
  حرف *log_format;
  ثابت حرف *handler;

  صحيح argc;
  صحيح verbosity;
  حرف **argv;

  bool command;
  bool option_systemd_cgroup;
  bool option_force_no_cgroup;
};

حرف *argp_mandatory_argument (حرف *arg, بنية argp_state *state);
صحيح parse_id_or_fail (ثابت حرف *str, حرف **endptr, ثابت حرف *kind);
صحيح init_libcrun_context (libcrun_context_t *con, ثابت حرف *id, بنية crun_global_arguments *glob,
                          libcrun_error_t *err);
libcrun_context_t *new_libcrun_context (بنية crun_global_arguments *glob);
فراغ crun_assert_n_args (صحيح n, صحيح min, صحيح max);
#endif
