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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SECCOMP_NOTIFY_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SECCOMP_NOTIFY_H

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "error.h"

#ਜੇ ! (HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES)
#  define SECCOMP_NOTIFY_SKIP_TYPEDEF
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ "seccomp_notify_plugin.h"

ਬਣਤਰ seccomp_notify_context_s;

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_ਲੱਦੋ_seccomp_notify_plugins (ਬਣਤਰ seccomp_notify_context_s **ਬਾਹਰਲਾ, ਅਚਲ ਅੱਖਰ *plugins,
                                                        ਬਣਤਰ libcrun_ਲੱਦੋ_seccomp_notify_conf_s *conf,
                                                        libcrun_error_t *err);
LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_seccomp_notify_plugins (ਬਣਤਰ seccomp_notify_context_s *ctx, ਪੂਰਨ seccomp_fd,
                                                   libcrun_error_t *err);
LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_free_seccomp_notify_plugins (ਬਣਤਰ seccomp_notify_context_s *ctx, libcrun_error_t *err);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_seccomp_notify_context __attribute__ ((cleanup (cleanup_seccomp_notify_pluginsp)))
ਖਾਲੀ cleanup_seccomp_notify_pluginsp (ਖਾਲੀ *p);

#ਜੇ_ਅੰਤ
