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

#अगर_अघोषित STATUS_H
#घोषणा STATUS_H

#समावेश <config.h>
#समावेश <ocispec/runtime_spec_schema_config_schema.h>
#समावेश "error.h"
#समावेश "container.h"

काष्ठा libcrun_container_list_s
{
  काष्ठा libcrun_container_list_s *अग्र;
  अक्षर *name;
};
प्रकार काष्ठा libcrun_container_list_s libcrun_container_list_t;

काष्ठा libcrun_container_status_s
{
  pid_t pid;
  अचिन्हित दीर्घ दीर्घ process_start_time;
  अक्षर *bundle;
  अक्षर *rootfs;
  अक्षर *cgroup_path;
  अक्षर *scope;
  पूर्णांक systemd_cgroup;
  अक्षर *created;
  पूर्णांक detached;
  पूर्णांक no_new_keyring;
  अक्षर *external_descriptors;
  अक्षर *owner;
};
प्रकार काष्ठा libcrun_container_status_s libcrun_container_status_t;

LIBCRUN_PUBLIC व्योम libcrun_free_container_status (libcrun_container_status_t *status);
LIBCRUN_PUBLIC पूर्णांक libcrun_write_container_status (स्थिर अक्षर *state_मूल, स्थिर अक्षर *id,
                                                   libcrun_container_status_t *status, libcrun_error_t *err);
LIBCRUN_PUBLIC पूर्णांक libcrun_read_container_status (libcrun_container_status_t *status, स्थिर अक्षर *state_मूल,
                                                  स्थिर अक्षर *id, libcrun_error_t *err);
LIBCRUN_PUBLIC व्योम libcrun_free_containers_list (libcrun_container_list_t *list);
LIBCRUN_PUBLIC पूर्णांक libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err);
LIBCRUN_PUBLIC पूर्णांक libcrun_get_state_directory (अक्षर **out, स्थिर अक्षर *state_मूल, स्थिर अक्षर *id, libcrun_error_t *err);
LIBCRUN_PUBLIC पूर्णांक libcrun_container_delete_status (स्थिर अक्षर *state_मूल, स्थिर अक्षर *id, libcrun_error_t *err);
LIBCRUN_PUBLIC पूर्णांक libcrun_get_containers_list (libcrun_container_list_t **ret, स्थिर अक्षर *state_मूल,
                                                libcrun_error_t *err);

काष्ठा libcrun_container_iter_s
{
  libcrun_container_list_t *cur;
};
प्रकार काष्ठा libcrun_container_iter_s libcrun_container_iter_t;

LIBCRUN_PUBLIC पूर्णांक libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out,
                                           libcrun_error_t *err);
LIBCRUN_PUBLIC libcrun_container_iter_t *libcrun_container_list_iter (libcrun_container_list_t *l);
LIBCRUN_PUBLIC bool libcrun_container_iter_अग्र (libcrun_container_iter_t *it, स्थिर अक्षर **id);
LIBCRUN_PUBLIC व्योम libcrun_container_iter_free (libcrun_container_iter_t *it);
LIBCRUN_PUBLIC व्योम libcrun_container_list_free (libcrun_container_list_t *l);

प्रकार क्रमागत
{
  LIBCRUN_CONTAINER_STATUS_CREATING = 0,
  LIBCRUN_CONTAINER_STATUS_CREATED,
  LIBCRUN_CONTAINER_STATUS_RUNNING,
  LIBCRUN_CONTAINER_STATUS_STOPPED,
  LIBCRUN_CONTAINER_STATUS_PAUSED,
} libcrun_container_state_t;

काष्ठा libcrun_status_s;
प्रकार काष्ठा libcrun_status_s libcrun_status_t;

LIBCRUN_PUBLIC पूर्णांक libcrun_container_status_load (libcrun_context_t *context, स्थिर अक्षर *id, libcrun_status_t **out,
                                                  libcrun_error_t *err);
LIBCRUN_PUBLIC व्योम libcrun_container_status_free (libcrun_status_t *st);
/* Resolve the live state of the container described by ST.  The state is
   probed on the first call and cached afterwards; STATE and RUNNING can be
   NULL.  */
LIBCRUN_PUBLIC पूर्णांक libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, पूर्णांक *running,
                                             libcrun_error_t *err);
LIBCRUN_PUBLIC स्थिर अक्षर *libcrun_container_state_to_string (libcrun_container_state_t state);
LIBCRUN_PUBLIC pid_t libcrun_status_get_pid (libcrun_status_t *st);
LIBCRUN_PUBLIC स्थिर अक्षर *libcrun_status_get_bundle (libcrun_status_t *st);
LIBCRUN_PUBLIC स्थिर अक्षर *libcrun_status_get_rootfs (libcrun_status_t *st);
LIBCRUN_PUBLIC स्थिर अक्षर *libcrun_status_get_created (libcrun_status_t *st);
LIBCRUN_PUBLIC स्थिर अक्षर *libcrun_status_get_owner (libcrun_status_t *st);
LIBCRUN_PUBLIC स्थिर अक्षर *libcrun_status_get_scope (libcrun_status_t *st);
LIBCRUN_PUBLIC स्थिर अक्षर *libcrun_status_get_external_descriptors (libcrun_status_t *st);
LIBCRUN_PUBLIC पूर्णांक libcrun_container_get_state_string (libcrun_context_t *context, स्थिर अक्षर *id, स्थिर अक्षर **out,
                                                       पूर्णांक *running, libcrun_error_t *err);
LIBCRUN_PUBLIC पूर्णांक libcrun_get_state_dir (अक्षर **out, स्थिर अक्षर *state_मूल, स्थिर अक्षर *id, libcrun_error_t *err);

पूर्णांक libcrun_status_check_directories (स्थिर अक्षर *state_मूल, स्थिर अक्षर *id, libcrun_error_t *err);
पूर्णांक libcrun_status_create_exec_fifo (स्थिर अक्षर *state_मूल, स्थिर अक्षर *id, libcrun_error_t *err);
पूर्णांक libcrun_status_write_exec_fifo (स्थिर अक्षर *state_मूल, स्थिर अक्षर *id, libcrun_error_t *err);
पूर्णांक libcrun_status_has_read_exec_fifo (स्थिर अक्षर *state_मूल, स्थिर अक्षर *id, libcrun_error_t *err);
पूर्णांक libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err);
पूर्णांक get_run_directory (अक्षर **out, स्थिर अक्षर *state_मूल, libcrun_error_t *err);
पूर्णांक get_shared_empty_directory_path (अक्षर **out, स्थिर अक्षर *state_मूल, libcrun_error_t *err);

अटल अंतरभूत व्योम
libcrun_free_container_listp (व्योम *p)
{
  libcrun_container_list_t **l = (libcrun_container_list_t **) p;
  अगर (*l != शून्य)
    libcrun_free_containers_list (*l);
}

#घोषणा cleanup_container_status __attribute__ ((cleanup (libcrun_free_container_status)))
#घोषणा cleanup_container_list __attribute__ ((cleanup (libcrun_free_container_listp)))

#पूर्ण_अगर
