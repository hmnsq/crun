/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019 Giuseppe Scrivano <giuseppe@scrivano.org>
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

#ifndef STATUS_H
#define STATUS_H

#अन्तर्भाव <config.h>
#अन्तर्भाव <ocispec/runtime_spec_schema_config_schema.h>
#अन्तर्भाव "error.h"
#अन्तर्भाव "container.h"

संरचना libcrun_container_list_s
{
  संरचना libcrun_container_list_s *next;
  वर्ण *name;
};
प्रकारनिर्देश संरचना libcrun_container_list_s libcrun_container_list_t;

संरचना libcrun_container_status_s
{
  pid_t pid;
  अचिन्हित दीर्घ दीर्घ process_start_time;
  वर्ण *bundle;
  वर्ण *rootfs;
  वर्ण *cgroup_path;
  वर्ण *scope;
  पूर्णाङ्क systemd_cgroup;
  वर्ण *created;
  पूर्णाङ्क detached;
  पूर्णाङ्क no_new_keyring;
  वर्ण *external_descriptors;
  वर्ण *owner;
};
प्रकारनिर्देश संरचना libcrun_container_status_s libcrun_container_status_t;

LIBCRUN_PUBLIC शून्य libcrun_free_container_status (libcrun_container_status_t *status);
LIBCRUN_PUBLIC पूर्णाङ्क libcrun_write_container_status (अचर वर्ण *state_जरा, अचर वर्ण *id,
                                                   libcrun_container_status_t *status, libcrun_error_t *err);
LIBCRUN_PUBLIC पूर्णाङ्क libcrun_read_container_status (libcrun_container_status_t *status, अचर वर्ण *state_जरा,
                                                  अचर वर्ण *id, libcrun_error_t *err);
LIBCRUN_PUBLIC शून्य libcrun_free_containers_list (libcrun_container_list_t *list);
LIBCRUN_PUBLIC पूर्णाङ्क libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err);
LIBCRUN_PUBLIC पूर्णाङ्क libcrun_get_state_directory (वर्ण **out, अचर वर्ण *state_जरा, अचर वर्ण *id, libcrun_error_t *err);
LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_delete_status (अचर वर्ण *state_जरा, अचर वर्ण *id, libcrun_error_t *err);
LIBCRUN_PUBLIC पूर्णाङ्क libcrun_get_containers_list (libcrun_container_list_t **ret, अचर वर्ण *state_जरा,
                                                libcrun_error_t *err);

संरचना libcrun_container_iter_s
{
  libcrun_container_list_t *cur;
};
प्रकारनिर्देश संरचना libcrun_container_iter_s libcrun_container_iter_t;

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out,
                                           libcrun_error_t *err);
LIBCRUN_PUBLIC libcrun_container_iter_t *libcrun_container_list_iter (libcrun_container_list_t *l);
LIBCRUN_PUBLIC bool libcrun_container_iter_next (libcrun_container_iter_t *it, अचर वर्ण **id);
LIBCRUN_PUBLIC शून्य libcrun_container_iter_free (libcrun_container_iter_t *it);
LIBCRUN_PUBLIC शून्य libcrun_container_list_free (libcrun_container_list_t *l);

प्रकारनिर्देश गणना
{
  LIBCRUN_CONTAINER_STATUS_CREATING = 0,
  LIBCRUN_CONTAINER_STATUS_CREATED,
  LIBCRUN_CONTAINER_STATUS_RUNNING,
  LIBCRUN_CONTAINER_STATUS_STOPPED,
  LIBCRUN_CONTAINER_STATUS_PAUSED,
} libcrun_container_state_t;

संरचना libcrun_status_s;
प्रकारनिर्देश संरचना libcrun_status_s libcrun_status_t;

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_status_load (libcrun_context_t *context, अचर वर्ण *id, libcrun_status_t **out,
                                                  libcrun_error_t *err);
LIBCRUN_PUBLIC शून्य libcrun_container_status_free (libcrun_status_t *st);
/* Resolve the live state of the container described by ST.  The state is
   probed on the first call and cached afterwards; STATE and RUNNING can be
   NULL.  */
LIBCRUN_PUBLIC पूर्णाङ्क libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, पूर्णाङ्क *running,
                                             libcrun_error_t *err);
LIBCRUN_PUBLIC अचर वर्ण *libcrun_container_state_to_string (libcrun_container_state_t state);
LIBCRUN_PUBLIC pid_t libcrun_status_get_pid (libcrun_status_t *st);
LIBCRUN_PUBLIC अचर वर्ण *libcrun_status_get_bundle (libcrun_status_t *st);
LIBCRUN_PUBLIC अचर वर्ण *libcrun_status_get_rootfs (libcrun_status_t *st);
LIBCRUN_PUBLIC अचर वर्ण *libcrun_status_get_created (libcrun_status_t *st);
LIBCRUN_PUBLIC अचर वर्ण *libcrun_status_get_owner (libcrun_status_t *st);
LIBCRUN_PUBLIC अचर वर्ण *libcrun_status_get_scope (libcrun_status_t *st);
LIBCRUN_PUBLIC अचर वर्ण *libcrun_status_get_external_descriptors (libcrun_status_t *st);
LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_get_state_string (libcrun_context_t *context, अचर वर्ण *id, अचर वर्ण **out,
                                                       पूर्णाङ्क *running, libcrun_error_t *err);
LIBCRUN_PUBLIC पूर्णाङ्क libcrun_get_state_dir (वर्ण **out, अचर वर्ण *state_जरा, अचर वर्ण *id, libcrun_error_t *err);

पूर्णाङ्क libcrun_status_check_directories (अचर वर्ण *state_जरा, अचर वर्ण *id, libcrun_error_t *err);
पूर्णाङ्क libcrun_status_create_exec_fifo (अचर वर्ण *state_जरा, अचर वर्ण *id, libcrun_error_t *err);
पूर्णाङ्क libcrun_status_write_exec_fifo (अचर वर्ण *state_जरा, अचर वर्ण *id, libcrun_error_t *err);
पूर्णाङ्क libcrun_status_has_read_exec_fifo (अचर वर्ण *state_जरा, अचर वर्ण *id, libcrun_error_t *err);
पूर्णाङ्क libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err);
पूर्णाङ्क get_run_directory (वर्ण **out, अचर वर्ण *state_जरा, libcrun_error_t *err);
पूर्णाङ्क get_shared_empty_directory_path (वर्ण **out, अचर वर्ण *state_जरा, libcrun_error_t *err);

स्थिर अन्तःस्थ शून्य
libcrun_free_container_listp (शून्य *p)
{
  libcrun_container_list_t **l = (libcrun_container_list_t **) p;
  यदि (*l != रिक्त)
    libcrun_free_containers_list (*l);
}

#define cleanup_container_status __attribute__ ((cleanup (libcrun_free_container_status)))
#define cleanup_container_list __attribute__ ((cleanup (libcrun_free_container_listp)))

#endif
