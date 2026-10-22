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

#شامل <stdbool.h>
#شامل <libcrun/libcrun.h>
#شامل "crun_utils.h"

ساختار crun_global_arguments
{
  نویسه *ریشه;
  نویسه *log;
  نویسه *log_format;
  ثابت نویسه *handler;

  صحیح argc;
  صحیح verbosity;
  نویسه **argv;

  bool command;
  bool option_systemd_cgroup;
  bool option_force_no_cgroup;
};

نویسه *argp_mandatory_argument (نویسه *arg, ساختار argp_state *state);
صحیح parse_id_or_fail (ثابت نویسه *str, نویسه **endptr, ثابت نویسه *kind);
صحیح init_libcrun_context (libcrun_context_t *con, ثابت نویسه *id, ساختار crun_global_arguments *glob,
                          libcrun_error_t *err);
libcrun_context_t *new_libcrun_context (ساختار crun_global_arguments *glob);
تهی crun_assert_n_args (صحیح n, صحیح min, صحیح max);
#endif
