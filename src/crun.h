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

#অন্তর্ভাৱ <stdbool.h>
#অন্তর্ভাৱ <libcrun/libcrun.h>
#অন্তর্ভাৱ "crun_utils.h"

সংরচনা crun_global_arguments
{
  ৱর্ণ *মূল;
  ৱর্ণ *log;
  ৱর্ণ *log_format;
  ধ্রুৱ ৱর্ণ *handler;

  পূর্ণাঙ্ক argc;
  পূর্ণাঙ্ক verbosity;
  ৱর্ণ **argv;

  bool command;
  bool option_systemd_cgroup;
  bool option_force_no_cgroup;
};

ৱর্ণ *argp_mandatory_argument (ৱর্ণ *arg, সংরচনা argp_state *state);
পূর্ণাঙ্ক parse_id_or_fail (ধ্রুৱ ৱর্ণ *str, ৱর্ণ **endptr, ধ্রুৱ ৱর্ণ *kind);
পূর্ণাঙ্ক init_libcrun_context (libcrun_context_t *con, ধ্রুৱ ৱর্ণ *id, সংরচনা crun_global_arguments *glob,
                          libcrun_error_t *err);
libcrun_context_t *new_libcrun_context (সংরচনা crun_global_arguments *glob);
শূন্য crun_assert_n_args (পূর্ণাঙ্ক n, পূর্ণাঙ্ক min, পূর্ণাঙ্ক max);
#endif
