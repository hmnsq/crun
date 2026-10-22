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
#నిర్వచితం_కాకపోతే SECCOMP_NOTIFY_H
#నిర్వచించు SECCOMP_NOTIFY_H

#అన్తర్భావ <config.h>
#అన్తర్భావ "error.h"

#ఒకవేళ ! (HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES)
#  define SECCOMP_NOTIFY_SKIP_TYPEDEF
#అయితే_ముగింపు
#అన్తర్భావ "seccomp_notify_plugin.h"

నిర్మాణం seccomp_notify_context_s;

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_ఎక్కించు_seccomp_notify_plugins (నిర్మాణం seccomp_notify_context_s **బహిర్గతం, స్థిరరాశి అక్షరం *plugins,
                                                        నిర్మాణం libcrun_ఎక్కించు_seccomp_notify_conf_s *conf,
                                                        libcrun_error_t *err);
LIBCRUN_ప్రజా పూర్ణాంకం libcrun_seccomp_notify_plugins (నిర్మాణం seccomp_notify_context_s *ctx, పూర్ణాంకం seccomp_fd,
                                                   libcrun_error_t *err);
LIBCRUN_ప్రజా పూర్ణాంకం libcrun_free_seccomp_notify_plugins (నిర్మాణం seccomp_notify_context_s *ctx, libcrun_error_t *err);

#నిర్వచించు cleanup_seccomp_notify_context __attribute__ ((cleanup (cleanup_seccomp_notify_pluginsp)))
శూన్య cleanup_seccomp_notify_pluginsp (శూన్య *p);

#అయితే_ముగింపు
