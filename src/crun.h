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
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CRUN_CLI_H
#ವ್ಯಾಖ್ಯಾನಿಸು CRUN_CLI_H

#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <libcrun/libcrun.h>
#ಅನ್ತರ್ಭಾವ "crun_utils.h"

ರಚನೆ crun_global_arguments
{
  ಅಕ್ಷರ *ಬೇರು;
  ಅಕ್ಷರ *ಲಘುಗಣಕ;
  ಅಕ್ಷರ *log_format;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *handler;

  ಪೂರ್ಣಾಂಕ argc;
  ಪೂರ್ಣಾಂಕ verbosity;
  ಅಕ್ಷರ **argv;

  bool command;
  bool option_systemd_cgroup;
  bool option_force_no_cgroup;
};

ಅಕ್ಷರ *argp_mandatory_ವಾದ (ಅಕ್ಷರ *arg, ರಚನೆ argp_state *state);
ಪೂರ್ಣಾಂಕ parse_id_or_fail (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str, ಅಕ್ಷರ **endptr, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬಗೆ);
ಪೂರ್ಣಾಂಕ init_libcrun_context (libcrun_context_t *con, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ರಚನೆ crun_global_arguments *glob,
                          libcrun_error_t *err);
libcrun_context_t *new_libcrun_context (ರಚನೆ crun_global_arguments *glob);
ಶೂನ್ಯ crun_assert_n_ವಾದಗಳು (ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ min, ಪೂರ್ಣಾಂಕ max);
#ಆದರೆ_ಅಂತ್ಯ
