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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "error.h"

#എങ്കിൽ ! (HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES)
#  define SECCOMP_NOTIFY_SKIP_TYPEDEF
#endif
#അന്തര്ഭാവ "seccomp_notify_plugin.h"

ഘടന seccomp_notify_context_s;

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_load_seccomp_notify_plugins (ഘടന seccomp_notify_context_s **out, സ്ഥിരം അക്ഷരം *plugins,
                                                        ഘടന libcrun_load_seccomp_notify_conf_s *conf,
                                                        libcrun_error_t *err);
LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_seccomp_notify_plugins (ഘടന seccomp_notify_context_s *ctx, പൂർണ്ണസംഖ്യ seccomp_fd,
                                                   libcrun_error_t *err);
LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_free_seccomp_notify_plugins (ഘടന seccomp_notify_context_s *ctx, libcrun_error_t *err);

#define cleanup_seccomp_notify_context __attribute__ ((cleanup (cleanup_seccomp_notify_pluginsp)))
ശൂന്യം cleanup_seccomp_notify_pluginsp (ശൂന്യം *p);

#endif
