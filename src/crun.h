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

#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <libcrun/libcrun.h>
#अन्तर्भाव "crun_utils.h"

संरचना crun_global_arguments
{
  अक्षर *मूळ;
  अक्षर *log;
  अक्षर *log_format;
  स्थिरांक अक्षर *handler;

  पूर्णांक argc;
  पूर्णांक verbosity;
  अक्षर **argv;

  bool command;
  bool option_systemd_cgroup;
  bool option_force_no_cgroup;
};

अक्षर *argp_mandatory_argument (अक्षर *arg, संरचना argp_state *state);
पूर्णांक parse_id_or_fail (स्थिरांक अक्षर *str, अक्षर **endptr, स्थिरांक अक्षर *kind);
पूर्णांक init_libcrun_context (libcrun_context_t *con, स्थिरांक अक्षर *id, संरचना crun_global_arguments *glob,
                          libcrun_error_t *err);
libcrun_context_t *new_libcrun_context (संरचना crun_global_arguments *glob);
शून्य crun_assert_n_args (पूर्णांक n, पूर्णांक min, पूर्णांक max);
#endif
