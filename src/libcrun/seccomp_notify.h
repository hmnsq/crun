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
#ifndef SECCOMP_NOTIFY_H
#define SECCOMP_NOTIFY_H

#ضمن <config.h>
#ضمن "error.h"

#𐤀𐤌 ! (HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES)
#  define SECCOMP_NOTIFY_SKIP_TYPEDEF
#endif
#ضمن "seccomp_notify_plugin.h"

بنية seccomp_notify_context_s;

LIBCRUN_PUBLIC صحيح libcrun_load_seccomp_notify_plugins (بنية seccomp_notify_context_s **out, ثابت حرف *plugins,
                                                        بنية libcrun_load_seccomp_notify_conf_s *conf,
                                                        libcrun_error_t *err);
LIBCRUN_PUBLIC صحيح libcrun_seccomp_notify_plugins (بنية seccomp_notify_context_s *ctx, صحيح seccomp_fd,
                                                   libcrun_error_t *err);
LIBCRUN_PUBLIC صحيح libcrun_free_seccomp_notify_plugins (بنية seccomp_notify_context_s *ctx, libcrun_error_t *err);

#define cleanup_seccomp_notify_context __attribute__ ((cleanup (cleanup_seccomp_notify_pluginsp)))
فراغ cleanup_seccomp_notify_pluginsp (فراغ *p);

#endif
