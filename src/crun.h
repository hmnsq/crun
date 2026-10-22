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

#অন্তর্ভাব <stdbool.h>
#অন্তর্ভাব <libcrun/libcrun.h>
#অন্তর্ভাব "crun_utils.h"

কাঠামো crun_global_arguments
{
  অক্ষর *মূল;
  অক্ষর *log;
  অক্ষর *log_format;
  ধ্রুবক অক্ষর *handler;

  পূর্ণসংখ্যা argc;
  পূর্ণসংখ্যা verbosity;
  অক্ষর **argv;

  bool command;
  bool option_systemd_cgroup;
  bool option_force_no_cgroup;
};

অক্ষর *argp_mandatory_argument (অক্ষর *arg, কাঠামো argp_state *state);
পূর্ণসংখ্যা parse_id_or_fail (ধ্রুবক অক্ষর *str, অক্ষর **endptr, ধ্রুবক অক্ষর *kind);
পূর্ণসংখ্যা init_libcrun_context (libcrun_context_t *con, ধ্রুবক অক্ষর *id, কাঠামো crun_global_arguments *glob,
                          libcrun_error_t *err);
libcrun_context_t *new_libcrun_context (কাঠামো crun_global_arguments *glob);
শূন্য crun_assert_n_args (পূর্ণসংখ্যা n, পূর্ণসংখ্যা min, পূর্ণসংখ্যা max);
#endif
