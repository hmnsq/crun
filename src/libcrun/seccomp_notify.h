/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2020 Giuseppe Scrivano <giuseppe@scrivano.org>
 * crun is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * crun is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with crun.  If not, see <http://www.gnu.org/licenses/>.
 */
#अगर_अघोषित SECCOMP_NOTIFY_H
#घोषणा SECCOMP_NOTIFY_H

#समावेश <config.h>
#समावेश "error.h"

#अगर ! (HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES)
#  define SECCOMP_NOTIFY_SKIP_TYPEDEF
#पूर्ण_अगर
#समावेश "seccomp_notify_plugin.h"

काष्ठा seccomp_notify_context_s;

LIBCRUN_PUBLIC पूर्णांक libcrun_load_seccomp_notify_plugins (काष्ठा seccomp_notify_context_s **out, स्थिर अक्षर *plugins,
                                                        काष्ठा libcrun_load_seccomp_notify_conf_s *conf,
                                                        libcrun_error_t *err);
LIBCRUN_PUBLIC पूर्णांक libcrun_seccomp_notify_plugins (काष्ठा seccomp_notify_context_s *ctx, पूर्णांक seccomp_fd,
                                                   libcrun_error_t *err);
LIBCRUN_PUBLIC पूर्णांक libcrun_free_seccomp_notify_plugins (काष्ठा seccomp_notify_context_s *ctx, libcrun_error_t *err);

#घोषणा cleanup_seccomp_notify_context __attribute__ ((cleanup (cleanup_seccomp_notify_pluginsp)))
व्योम cleanup_seccomp_notify_pluginsp (व्योम *p);

#पूर्ण_अगर
