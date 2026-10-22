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
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SECCOMP_NOTIFY_PLUGINPLUGIN_H

#  include <linux/seccomp.h>

ರಚನೆ libcrun_ತುಂಬು_seccomp_notify_conf_s
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *runtime_ಬೇರು_ಮಾರ್ಗ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *bundle_ಮಾರ್ಗ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *oci_config_ಮಾರ್ಗ;
};

/* The plugin doesn't know how to handle the request.  */
#  define ಸತತ_OCI_SECCOMP_NOTIFY_HANDLE_NOT_HANDLED 0
/* The plugin filled the response and it is ready to write.  */
#  define ಸತತ_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE 1
/* The plugin will handle the request and write directly to the fd.  */
#  define ಸತತ_OCI_SECCOMP_NOTIFY_HANDLE_DELAYED_RESPONSE 2
/* Specify SECCOMP_USER_NOTIF_FLAG_CONTINUE in the flags.  */
#  define ಸತತ_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE_AND_CONTINUE 3

#  ifndef SECCOMP_NOTIFY_SKIP_TYPEDEF

/* Configure the plugin.  Return an opaque pointer that will be used for successive calls.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*ಸತತ_oci_seccomp_notify_ಆರಂಭ_cb) (ಶೂನ್ಯ **opaque, ರಚನೆ libcrun_ತುಂಬು_seccomp_notify_conf_s *conf,
                                                size_t ಗಾತ್ರ_configuration);

/* Try to handle a single request.  It MUST be defined.
   HANDLED specifies how the request was handled by the plugin:
   0: not handled, try next plugin or return ENOTSUP if it is the last plugin.
   RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE: sresp filled and ready to be notified to seccomp.
   RUN_OCI_SECCOMP_NOTIFY_HANDLE_DELAYED_RESPONSE: the notification will be handled internally by the plugin and
   forwarded to seccomp_fd. It is useful for asynchronous handling.
*/
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*ಸತತ_oci_seccomp_notify_handle_request_cb) (ಶೂನ್ಯ *opaque, ರಚನೆ seccomp_notif_sizes *sizes,
                                                         ರಚನೆ seccomp_notif *sreq, ರಚನೆ seccomp_notif_resp *sresp,
                                                         ಪೂರ್ಣಾಂಕ seccomp_fd, ಪೂರ್ಣಾಂಕ *handled);

/* Stop the plugin.  The opaque value is the return value from run_oci_seccomp_notify_start.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*ಸತತ_oci_seccomp_notify_stop_cb) (ಶೂನ್ಯ *opaque);

/* Retrieve the API version used by the plugin.  It MUST return 1. */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*ಸತತ_oci_seccomp_notify_plugin_ಆವೃತ್ತಿ_cb) ();

#  endif

#ಆದರೆ_ಅಂತ್ಯ
