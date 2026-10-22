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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CRUN_CLI_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CRUN_CLI_H

#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <libcrun/libcrun.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "crun_utils.h"

ਬਣਤਰ crun_global_arguments
{
  ਅੱਖਰ *ਜੜ੍ਹ;
  ਅੱਖਰ *ਲਘੂਗਣਕ;
  ਅੱਖਰ *log_format;
  ਅਚਲ ਅੱਖਰ *handler;

  ਪੂਰਨ argc;
  ਪੂਰਨ verbosity;
  ਅੱਖਰ **argv;

  bool command;
  bool option_systemd_cgroup;
  bool option_force_no_cgroup;
};

ਅੱਖਰ *argp_mandatory_ਦਲੀਲ (ਅੱਖਰ *arg, ਬਣਤਰ argp_state *state);
ਪੂਰਨ parse_id_or_fail (ਅਚਲ ਅੱਖਰ *str, ਅੱਖਰ **endptr, ਅਚਲ ਅੱਖਰ *ਕਿਸਮ);
ਪੂਰਨ init_libcrun_context (libcrun_context_t *con, ਅਚਲ ਅੱਖਰ *id, ਬਣਤਰ crun_global_arguments *glob,
                          libcrun_error_t *err);
libcrun_context_t *new_libcrun_context (ਬਣਤਰ crun_global_arguments *glob);
ਖਾਲੀ crun_assert_n_ਦਲੀਲਾਂ (ਪੂਰਨ n, ਪੂਰਨ min, ਪੂਰਨ max);
#ਜੇ_ਅੰਤ
