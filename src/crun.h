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

#அந்தர்பாவ <stdbool.h>
#அந்தர்பாவ <libcrun/libcrun.h>
#அந்தர்பாவ "crun_utils.h"

கட்டமைப்பு crun_global_arguments
{
  எழுத்து *வேர்;
  எழுத்து *log;
  எழுத்து *log_format;
  மாறிலி எழுத்து *handler;

  முழு argc;
  முழு verbosity;
  எழுத்து **argv;

  bool command;
  bool option_systemd_cgroup;
  bool option_force_no_cgroup;
};

எழுத்து *argp_mandatory_argument (எழுத்து *arg, கட்டமைப்பு argp_state *state);
முழு parse_id_or_fail (மாறிலி எழுத்து *str, எழுத்து **endptr, மாறிலி எழுத்து *kind);
முழு init_libcrun_context (libcrun_context_t *con, மாறிலி எழுத்து *id, கட்டமைப்பு crun_global_arguments *glob,
                          libcrun_error_t *err);
libcrun_context_t *new_libcrun_context (கட்டமைப்பு crun_global_arguments *glob);
வெறுமை crun_assert_n_args (முழு n, முழு min, முழு max);
#endif
