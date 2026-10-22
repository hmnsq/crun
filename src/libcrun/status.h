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

#यदि_अपरिभाषितम् स्थितिः_H
#परिभाषय स्थितिः_H

#अन्तर्भाव <config.h>
#अन्तर्भाव <ocispec/runtime_spec_schema_config_schema.h>
#अन्तर्भाव "error.h"
#अन्तर्भाव "container.h"

संरचना libcrun_container_list_s
{
  संरचना libcrun_container_list_s *अग्र;
  वर्ण *name;
};
प्रकारनिर्देश संरचना libcrun_container_list_s libcrun_container_list_t;

संरचना libcrun_container_स्थितिः_s
{
  pid_t pid;
  अचिह्नित दीर्घ दीर्घ process_start_time;
  वर्ण *bundle;
  वर्ण *rootfs;
  वर्ण *cgroup_मार्गः;
  वर्ण *scope;
  पूर्णाङ्क systemd_cgroup;
  वर्ण *created;
  पूर्णाङ्क detached;
  पूर्णाङ्क no_new_keyring;
  वर्ण *external_descriptors;
  वर्ण *owner;
};
प्रकारनिर्देश संरचना libcrun_container_स्थितिः_s libcrun_container_स्थितिः_t;

LIBCRUN_लौकिकम् शून्य libcrun_free_container_स्थितिः (libcrun_container_स्थितिः_t *स्थितिः);
LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_write_container_स्थितिः (ध्रुव वर्ण *state_मूलम्, ध्रुव वर्ण *id,
                                                   libcrun_container_स्थितिः_t *स्थितिः, libcrun_error_t *err);
LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_read_container_स्थितिः (libcrun_container_स्थितिः_t *स्थितिः, ध्रुव वर्ण *state_मूलम्,
                                                  ध्रुव वर्ण *id, libcrun_error_t *err);
LIBCRUN_लौकिकम् शून्य libcrun_free_containers_list (libcrun_container_list_t *list);
LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_is_container_running (libcrun_container_स्थितिः_t *स्थितिः, libcrun_error_t *err);
LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_get_state_directory (वर्ण **बहिर्गमनम्, ध्रुव वर्ण *state_मूलम्, ध्रुव वर्ण *id, libcrun_error_t *err);
LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_delete_स्थितिः (ध्रुव वर्ण *state_मूलम्, ध्रुव वर्ण *id, libcrun_error_t *err);
LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_get_containers_list (libcrun_container_list_t **ret, ध्रुव वर्ण *state_मूलम्,
                                                libcrun_error_t *err);

संरचना libcrun_container_iter_s
{
  libcrun_container_list_t *cur;
};
प्रकारनिर्देश संरचना libcrun_container_iter_s libcrun_container_iter_t;

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **बहिर्गमनम्,
                                           libcrun_error_t *err);
LIBCRUN_लौकिकम् libcrun_container_iter_t *libcrun_container_list_iter (libcrun_container_list_t *l);
LIBCRUN_लौकिकम् bool libcrun_container_iter_अग्र (libcrun_container_iter_t *it, ध्रुव वर्ण **id);
LIBCRUN_लौकिकम् शून्य libcrun_container_iter_free (libcrun_container_iter_t *it);
LIBCRUN_लौकिकम् शून्य libcrun_container_list_free (libcrun_container_list_t *l);

प्रकारनिर्देश गणना
{
  LIBCRUN_CONTAINER_स्थितिः_CREATING = 0,
  LIBCRUN_CONTAINER_स्थितिः_CREATED,
  LIBCRUN_CONTAINER_स्थितिः_RUNNING,
  LIBCRUN_CONTAINER_स्थितिः_STOPPED,
  LIBCRUN_CONTAINER_स्थितिः_PAUSED,
} libcrun_container_state_t;

संरचना libcrun_स्थितिः_s;
प्रकारनिर्देश संरचना libcrun_स्थितिः_s libcrun_स्थितिः_t;

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_स्थितिः_आरोपय (libcrun_context_t *context, ध्रुव वर्ण *id, libcrun_स्थितिः_t **बहिर्गमनम्,
                                                  libcrun_error_t *err);
LIBCRUN_लौकिकम् शून्य libcrun_container_स्थितिः_free (libcrun_स्थितिः_t *st);
/* Resolve the live state of the container described by ST.  The state is
   probed on the first call and cached afterwards; STATE and RUNNING can be
   NULL.  */
LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_स्थितिः_get_state (libcrun_स्थितिः_t *st, libcrun_container_state_t *state, पूर्णाङ्क *running,
                                             libcrun_error_t *err);
LIBCRUN_लौकिकम् ध्रुव वर्ण *libcrun_container_state_गन्तव्यम्_string (libcrun_container_state_t state);
LIBCRUN_लौकिकम् pid_t libcrun_स्थितिः_get_pid (libcrun_स्थितिः_t *st);
LIBCRUN_लौकिकम् ध्रुव वर्ण *libcrun_स्थितिः_get_bundle (libcrun_स्थितिः_t *st);
LIBCRUN_लौकिकम् ध्रुव वर्ण *libcrun_स्थितिः_get_rootfs (libcrun_स्थितिः_t *st);
LIBCRUN_लौकिकम् ध्रुव वर्ण *libcrun_स्थितिः_get_created (libcrun_स्थितिः_t *st);
LIBCRUN_लौकिकम् ध्रुव वर्ण *libcrun_स्थितिः_get_owner (libcrun_स्थितिः_t *st);
LIBCRUN_लौकिकम् ध्रुव वर्ण *libcrun_स्थितिः_get_scope (libcrun_स्थितिः_t *st);
LIBCRUN_लौकिकम् ध्रुव वर्ण *libcrun_स्थितिः_get_external_descriptors (libcrun_स्थितिः_t *st);
LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_get_state_string (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण **बहिर्गमनम्,
                                                       पूर्णाङ्क *running, libcrun_error_t *err);
LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_get_state_dir (वर्ण **बहिर्गमनम्, ध्रुव वर्ण *state_मूलम्, ध्रुव वर्ण *id, libcrun_error_t *err);

पूर्णाङ्क libcrun_स्थितिः_check_directories (ध्रुव वर्ण *state_मूलम्, ध्रुव वर्ण *id, libcrun_error_t *err);
पूर्णाङ्क libcrun_स्थितिः_create_exec_fifo (ध्रुव वर्ण *state_मूलम्, ध्रुव वर्ण *id, libcrun_error_t *err);
पूर्णाङ्क libcrun_स्थितिः_write_exec_fifo (ध्रुव वर्ण *state_मूलम्, ध्रुव वर्ण *id, libcrun_error_t *err);
पूर्णाङ्क libcrun_स्थितिः_has_read_exec_fifo (ध्रुव वर्ण *state_मूलम्, ध्रुव वर्ण *id, libcrun_error_t *err);
पूर्णाङ्क libcrun_check_pid_valid (libcrun_container_स्थितिः_t *स्थितिः, libcrun_error_t *err);
पूर्णाङ्क get_सततम्_directory (वर्ण **बहिर्गमनम्, ध्रुव वर्ण *state_मूलम्, libcrun_error_t *err);
पूर्णाङ्क get_shared_रिक्तानि_directory_मार्गः (वर्ण **बहिर्गमनम्, ध्रुव वर्ण *state_मूलम्, libcrun_error_t *err);

स्थिर अन्तःस्थ शून्य
libcrun_free_container_listp (शून्य *p)
{
  libcrun_container_list_t **l = (libcrun_container_list_t **) p;
  यदि (*l != रिक्त)
    libcrun_free_containers_list (*l);
}

#परिभाषय cleanup_container_स्थितिः __attribute__ ((cleanup (libcrun_free_container_स्थितिः)))
#परिभाषय cleanup_container_list __attribute__ ((cleanup (libcrun_free_container_listp)))

#यदि_अन्तः
