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

#અન્તર્ભાવ <stdbool.h>
#અન્તર્ભાવ <libcrun/libcrun.h>
#અન્તર્ભાવ "crun_utils.h"

સંરચના crun_global_arguments
{
  અક્ષર *મૂળ;
  અક્ષર *log;
  અક્ષર *log_format;
  અચલ અક્ષર *handler;

  પૂર્ણાંક argc;
  પૂર્ણાંક verbosity;
  અક્ષર **argv;

  bool command;
  bool option_systemd_cgroup;
  bool option_force_no_cgroup;
};

અક્ષર *argp_mandatory_argument (અક્ષર *arg, સંરચના argp_state *state);
પૂર્ણાંક parse_id_or_fail (અચલ અક્ષર *str, અક્ષર **endptr, અચલ અક્ષર *kind);
પૂર્ણાંક init_libcrun_context (libcrun_context_t *con, અચલ અક્ષર *id, સંરચના crun_global_arguments *glob,
                          libcrun_error_t *err);
libcrun_context_t *new_libcrun_context (સંરચના crun_global_arguments *glob);
શૂન્ય crun_assert_n_args (પૂર્ણાંક n, પૂર્ણાંક min, પૂર્ણાંક max);
#endif
