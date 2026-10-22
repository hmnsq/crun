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

#ଅନ୍ତର୍ଭାଵ <stdbool.h>
#ଅନ୍ତର୍ଭାଵ <libcrun/libcrun.h>
#ଅନ୍ତର୍ଭାଵ "crun_utils.h"

ସଂରଚନା crun_global_arguments
{
  ଅକ୍ଷର *ମୂଳ;
  ଅକ୍ଷର *log;
  ଅକ୍ଷର *log_format;
  ଅଚଳ ଅକ୍ଷର *handler;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity;
  ଅକ୍ଷର **argv;

  bool command;
  bool option_systemd_cgroup;
  bool option_force_no_cgroup;
};

ଅକ୍ଷର *argp_mandatory_argument (ଅକ୍ଷର *arg, ସଂରଚନା argp_state *state);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_id_or_fail (ଅଚଳ ଅକ୍ଷର *str, ଅକ୍ଷର **endptr, ଅଚଳ ଅକ୍ଷର *kind);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା init_libcrun_context (libcrun_context_t *con, ଅଚଳ ଅକ୍ଷର *id, ସଂରଚନା crun_global_arguments *glob,
                          libcrun_error_t *err);
libcrun_context_t *new_libcrun_context (ସଂରଚନା crun_global_arguments *glob);
ଶୂନ୍ୟ crun_assert_n_args (ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା min, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max);
#endif
