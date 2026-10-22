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
#यदि_अपरिभाषितम् SECCOMP_NOTIFY_H
#परिभाषय SECCOMP_NOTIFY_H

#अन्तर्भाव <config.h>
#अन्तर्भाव "error.h"

#यदि ! (HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES)
#  define SECCOMP_NOTIFY_SKIP_TYPEDEF
#यदि_अन्तः
#अन्तर्भाव "seccomp_notify_plugin.h"

संरचना seccomp_notify_context_s;

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_आरोपय_seccomp_notify_plugins (संरचना seccomp_notify_context_s **बहिर्गमनम्, ध्रुव वर्ण *plugins,
                                                        संरचना libcrun_आरोपय_seccomp_notify_conf_s *conf,
                                                        libcrun_error_t *err);
LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_seccomp_notify_plugins (संरचना seccomp_notify_context_s *ctx, पूर्णाङ्क seccomp_fd,
                                                   libcrun_error_t *err);
LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_free_seccomp_notify_plugins (संरचना seccomp_notify_context_s *ctx, libcrun_error_t *err);

#परिभाषय cleanup_seccomp_notify_context __attribute__ ((cleanup (cleanup_seccomp_notify_pluginsp)))
शून्य cleanup_seccomp_notify_pluginsp (शून्य *p);

#यदि_अन्तः
