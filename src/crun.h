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

#include <stdbool.h>
#include <libcrun/libcrun.h>
#include "crun_utils.h"

sa_nracanaa crun_global_arguments
{
  var_na *root;
  var_na *log;
  var_na *log_format;
  const var_na *handler;

  ကိန်းပြည့် argc;
  ကိန်းပြည့် verbosity;
  var_na **argv;

  bool command;
  bool option_systemd_cgroup;
  bool option_force_no_cgroup;
};

var_na *argp_mandatory_argument (var_na *arg, sa_nracanaa argp_state *state);
ကိန်းပြည့် parse_id_or_fail (const var_na *str, var_na **endptr, const var_na *kind);
ကိန်းပြည့် init_libcrun_context (libcrun_context_t *con, const var_na *id, sa_nracanaa crun_global_arguments *glob,
                          libcrun_error_t *err);
libcrun_context_t *new_libcrun_context (sa_nracanaa crun_global_arguments *glob);
void crun_assert_n_args (ကိန်းပြည့် n, ကိန်းပြည့် min, ကိန်းပြည့် max);
#endif
