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
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SECCOMP_NOTIFY_H
#ವ್ಯಾಖ್ಯಾನಿಸು SECCOMP_NOTIFY_H

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "error.h"

#ಆದರೆ ! (HAVE_DLOPEN && HAVE_SECCOMP_GET_NOTIF_SIZES)
#  define SECCOMP_NOTIFY_SKIP_TYPEDEF
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ "seccomp_notify_plugin.h"

ರಚನೆ seccomp_notify_context_s;

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_ತುಂಬು_seccomp_notify_plugins (ರಚನೆ seccomp_notify_context_s **ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *plugins,
                                                        ರಚನೆ libcrun_ತುಂಬು_seccomp_notify_conf_s *conf,
                                                        libcrun_error_t *err);
LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_seccomp_notify_plugins (ರಚನೆ seccomp_notify_context_s *ctx, ಪೂರ್ಣಾಂಕ seccomp_fd,
                                                   libcrun_error_t *err);
LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_free_seccomp_notify_plugins (ರಚನೆ seccomp_notify_context_s *ctx, libcrun_error_t *err);

#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_seccomp_notify_context __attribute__ ((cleanup (cleanup_seccomp_notify_pluginsp)))
ಶೂನ್ಯ cleanup_seccomp_notify_pluginsp (ಶೂನ್ಯ *p);

#ಆದರೆ_ಅಂತ್ಯ
