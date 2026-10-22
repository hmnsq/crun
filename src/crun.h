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
#अगर_अघोषित CRUN_CLI_H
#घोषणा CRUN_CLI_H

#समावेश <stdbool.h>
#समावेश <libcrun/libcrun.h>
#समावेश "crun_utils.h"

काष्ठा crun_global_arguments
{
  अक्षर *मूल;
  अक्षर *log;
  अक्षर *log_format;
  स्थिर अक्षर *handler;

  पूर्णांक argc;
  पूर्णांक verbosity;
  अक्षर **argv;

  bool command;
  bool option_systemd_cgroup;
  bool option_force_no_cgroup;
};

अक्षर *argp_mandatory_argument (अक्षर *arg, काष्ठा argp_state *state);
पूर्णांक parse_id_or_fail (स्थिर अक्षर *str, अक्षर **endptr, स्थिर अक्षर *kind);
पूर्णांक init_libcrun_context (libcrun_context_t *con, स्थिर अक्षर *id, काष्ठा crun_global_arguments *glob,
                          libcrun_error_t *err);
libcrun_context_t *new_libcrun_context (काष्ठा crun_global_arguments *glob);
व्योम crun_assert_n_args (पूर्णांक n, पूर्णांक min, पूर्णांक max);
#पूर्ण_अगर
