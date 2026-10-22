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

struct crun_global_arguments
{
  char *root;
  char *log;
  char *log_format;
  const char *handler;

  정수 argc;
  정수 verbosity;
  char **argv;

  bool command;
  bool option_systemd_cgroup;
  bool option_force_no_cgroup;
};

char *argp_mandatory_argument (char *arg, struct argp_state *state);
정수 parse_id_or_fail (const char *str, char **endptr, const char *kind);
정수 init_libcrun_context (libcrun_context_t *con, const char *id, struct crun_global_arguments *glob,
                          libcrun_error_t *err);
libcrun_context_t *new_libcrun_context (struct crun_global_arguments *glob);
void crun_assert_n_args (정수 n, 정수 min, 정수 max);
#endif
