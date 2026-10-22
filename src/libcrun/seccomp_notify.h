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
#که_نه_تعریف_شوی SECCOMP_NOTIFY_H
#تعریف_کړه SECCOMP_NOTIFY_H

#ضمن <config.h>
#ضمن "error.h"

#إذا ! (HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES)
#  define SECCOMP_NOTIFY_SKIP_TYPEDEF
#د_که_پای
#ضمن "seccomp_notify_plugin.h"

بنية seccomp_notify_context_s;

LIBCRUN_عامه صحيح libcrun_بارول_seccomp_notify_plugins (بنية seccomp_notify_context_s **بهر, ثابت حرف *plugins,
                                                        بنية libcrun_بارول_seccomp_notify_conf_s *conf,
                                                        libcrun_error_t *err);
LIBCRUN_عامه صحيح libcrun_seccomp_notify_plugins (بنية seccomp_notify_context_s *ctx, صحيح seccomp_fd,
                                                   libcrun_error_t *err);
LIBCRUN_عامه صحيح libcrun_free_seccomp_notify_plugins (بنية seccomp_notify_context_s *ctx, libcrun_error_t *err);

#تعریف_کړه cleanup_seccomp_notify_context __attribute__ ((cleanup (cleanup_seccomp_notify_pluginsp)))
فراغ cleanup_seccomp_notify_pluginsp (فراغ *p);

#د_که_پای
