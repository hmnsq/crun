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

#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <libcrun/libcrun.h>
#ಅನ್ತರ್ಭಾವ "crun_utils.h"

ಸಂರಚನಾ crun_global_arguments
{
  ವರ್ಣ *ಬೇರು;
  ವರ್ಣ *log;
  ವರ್ಣ *log_format;
  ಧ್ರುವ ವರ್ಣ *handler;

  ಪೂರ್ಣಾಙ್ಕ argc;
  ಪೂರ್ಣಾಙ್ಕ verbosity;
  ವರ್ಣ **argv;

  bool command;
  bool option_systemd_cgroup;
  bool option_force_no_cgroup;
};

ವರ್ಣ *argp_mandatory_argument (ವರ್ಣ *arg, ಸಂರಚನಾ argp_state *state);
ಪೂರ್ಣಾಙ್ಕ parse_id_or_fail (ಧ್ರುವ ವರ್ಣ *str, ವರ್ಣ **endptr, ಧ್ರುವ ವರ್ಣ *kind);
ಪೂರ್ಣಾಙ್ಕ init_libcrun_context (libcrun_context_t *con, ಧ್ರುವ ವರ್ಣ *id, ಸಂರಚನಾ crun_global_arguments *glob,
                          libcrun_error_t *err);
libcrun_context_t *new_libcrun_context (ಸಂರಚನಾ crun_global_arguments *glob);
ಶೂನ್ಯ crun_assert_n_args (ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ min, ಪೂರ್ಣಾಙ್ಕ max);
#endif
