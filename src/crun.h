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
  वर्ण *मूळ;
  वर्ण *log;
  वर्ण *log_format;
  ध्रुव वर्ण *handler;

  पूर्णाङ्क argc;
  पूर्णाङ्क verbosity;
  वर्ण **argv;

  bool command;
  bool option_systemd_cgroup;
  bool option_force_no_cgroup;
};

वर्ण *argp_mandatory_argument (वर्ण *arg, संरचना argp_state *state);
पूर्णाङ्क parse_id_or_fail (ध्रुव वर्ण *str, वर्ण **endptr, ध्रुव वर्ण *kind);
पूर्णाङ्क init_libcrun_context (libcrun_context_t *con, ध्रुव वर्ण *id, संरचना crun_global_arguments *glob,
                          libcrun_error_t *err);
libcrun_context_t *new_libcrun_context (संरचना crun_global_arguments *glob);
शून्य crun_assert_n_args (पूर्णाङ्क n, पूर्णाङ्क min, पूर्णाङ्क max);
#endif
