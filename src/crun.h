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
#اگر_غیر_تعریف_شدہ CRUN_CLI_H
#تعریف_کرو CRUN_CLI_H

#شامل <stdbool.h>
#شامل <libcrun/libcrun.h>
#شامل "crun_utils.h"

ساخت crun_global_arguments
{
  حرف *جڑ;
  حرف *لوگارتھم;
  حرف *log_format;
  ثابت حرف *handler;

  صحیح argc;
  صحیح verbosity;
  حرف **argv;

  bool command;
  bool option_systemd_cgroup;
  bool option_force_no_cgroup;
};

حرف *argp_mandatory_دلیل (حرف *arg, ساخت argp_state *state);
صحیح parse_id_or_fail (ثابت حرف *str, حرف **endptr, ثابت حرف *قسم);
صحیح init_libcrun_context (libcrun_context_t *con, ثابت حرف *id, ساخت crun_global_arguments *glob,
                          libcrun_error_t *err);
libcrun_context_t *new_libcrun_context (ساخت crun_global_arguments *glob);
خالی crun_assert_n_دلائل (صحیح n, صحیح min, صحیح max);
#اگر_ختم
