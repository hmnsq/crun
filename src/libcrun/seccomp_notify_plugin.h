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
#अगर_अघोषित SECCOMP_NOTIFY_PLUGINPLUGIN_H

#  include <linux/seccomp.h>

काष्ठा libcrun_load_seccomp_notify_conf_s
{
  स्थिर अक्षर *runtime_मूल_path;
  स्थिर अक्षर *name;
  स्थिर अक्षर *bundle_path;
  स्थिर अक्षर *oci_config_path;
};

/* The plugin doesn't know how to handle the request.  */
#  define RUN_OCI_SECCOMP_NOTIFY_HANDLE_NOT_HANDLED 0
/* The plugin filled the response and it is ready to write.  */
#  define RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE 1
/* The plugin will handle the request and write directly to the fd.  */
#  define RUN_OCI_SECCOMP_NOTIFY_HANDLE_DELAYED_RESPONSE 2
/* Specify SECCOMP_USER_NOTIF_FLAG_CONTINUE in the flags.  */
#  define RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE_AND_CONTINUE 3

#  ifndef SECCOMP_NOTIFY_SKIP_TYPEDEF

/* Configure the plugin.  Return an opaque pointer that will be used for successive calls.  */
प्रकार पूर्णांक (*run_oci_seccomp_notify_start_cb) (व्योम **opaque, काष्ठा libcrun_load_seccomp_notify_conf_s *conf,
                                                माप_प्रकार आकार_configuration);

/* Try to handle a single request.  It MUST be defined.
   HANDLED specifies how the request was handled by the plugin:
   0: not handled, try next plugin or return ENOTSUP if it is the last plugin.
   RUN_OCI_SECCOMP_NOTIFY_HANDLE_SEND_RESPONSE: sresp filled and ready to be notified to seccomp.
   RUN_OCI_SECCOMP_NOTIFY_HANDLE_DELAYED_RESPONSE: the notification will be handled internally by the plugin and
   forwarded to seccomp_fd. It is useful for asynchronous handling.
*/
प्रकार पूर्णांक (*run_oci_seccomp_notify_handle_request_cb) (व्योम *opaque, काष्ठा seccomp_notif_sizes *sizes,
                                                         काष्ठा seccomp_notif *sreq, काष्ठा seccomp_notif_resp *sresp,
                                                         पूर्णांक seccomp_fd, पूर्णांक *handled);

/* Stop the plugin.  The opaque value is the return value from run_oci_seccomp_notify_start.  */
प्रकार पूर्णांक (*run_oci_seccomp_notify_stop_cb) (व्योम *opaque);

/* Retrieve the API version used by the plugin.  It MUST return 1. */
प्रकार पूर्णांक (*run_oci_seccomp_notify_plugin_version_cb) ();

#  endif

#पूर्ण_अगर
