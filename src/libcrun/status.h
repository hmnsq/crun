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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <ocispec/runtime_spec_schema_config_schema.h>
#ಅನ್ತರ್ಭಾವ "error.h"
#ಅನ್ತರ್ಭಾವ "container.h"

ಸಂರಚನಾ libcrun_container_list_s
{
  ಸಂರಚನಾ libcrun_container_list_s *next;
  ವರ್ಣ *name;
};
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ libcrun_container_list_s libcrun_container_list_t;

ಸಂರಚನಾ libcrun_container_status_s
{
  pid_t pid;
  unsigned long long process_start_time;
  ವರ್ಣ *bundle;
  ವರ್ಣ *rootfs;
  ವರ್ಣ *cgroup_path;
  ವರ್ಣ *scope;
  ಪೂರ್ಣಾಙ್ಕ systemd_cgroup;
  ವರ್ಣ *created;
  ಪೂರ್ಣಾಙ್ಕ detached;
  ಪೂರ್ಣಾಙ್ಕ no_new_keyring;
  ವರ್ಣ *external_descriptors;
  ವರ್ಣ *owner;
};
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ libcrun_container_status_s libcrun_container_status_t;

LIBCRUN_PUBLIC ಶೂನ್ಯ libcrun_free_container_status (libcrun_container_status_t *status);
LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_write_container_status (ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, ಧ್ರುವ ವರ್ಣ *id,
                                                   libcrun_container_status_t *status, libcrun_error_t *err);
LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_read_container_status (libcrun_container_status_t *status, ಧ್ರುವ ವರ್ಣ *state_ಬೇರು,
                                                  ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err);
LIBCRUN_PUBLIC ಶೂನ್ಯ libcrun_free_containers_list (libcrun_container_list_t *list);
LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err);
LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_get_state_directory (ವರ್ಣ **out, ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err);
LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_delete_status (ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err);
LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_get_containers_list (libcrun_container_list_t **ret, ಧ್ರುವ ವರ್ಣ *state_ಬೇರು,
                                                libcrun_error_t *err);

ಸಂರಚನಾ libcrun_container_iter_s
{
  libcrun_container_list_t *cur;
};
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ libcrun_container_iter_s libcrun_container_iter_t;

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out,
                                           libcrun_error_t *err);
LIBCRUN_PUBLIC libcrun_container_iter_t *libcrun_container_list_iter (libcrun_container_list_t *l);
LIBCRUN_PUBLIC bool libcrun_container_iter_next (libcrun_container_iter_t *it, ಧ್ರುವ ವರ್ಣ **id);
LIBCRUN_PUBLIC ಶೂನ್ಯ libcrun_container_iter_free (libcrun_container_iter_t *it);
LIBCRUN_PUBLIC ಶೂನ್ಯ libcrun_container_list_free (libcrun_container_list_t *l);

ಪ್ರಕಾರನಿರ್ದೇಶ ಗಣನಾ
{
  LIBCRUN_CONTAINER_STATUS_CREATING = 0,
  LIBCRUN_CONTAINER_STATUS_CREATED,
  LIBCRUN_CONTAINER_STATUS_RUNNING,
  LIBCRUN_CONTAINER_STATUS_STOPPED,
  LIBCRUN_CONTAINER_STATUS_PAUSED,
} libcrun_container_state_t;

ಸಂರಚನಾ libcrun_status_s;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ libcrun_status_s libcrun_status_t;

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_status_load (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, libcrun_status_t **out,
                                                  libcrun_error_t *err);
LIBCRUN_PUBLIC ಶೂನ್ಯ libcrun_container_status_free (libcrun_status_t *st);
/* Resolve the live state of the container described by ST.  The state is
   probed on the first call and cached afterwards; STATE and RUNNING can be
   NULL.  */
LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, ಪೂರ್ಣಾಙ್ಕ *running,
                                             libcrun_error_t *err);
LIBCRUN_PUBLIC ಧ್ರುವ ವರ್ಣ *libcrun_container_state_to_string (libcrun_container_state_t state);
LIBCRUN_PUBLIC pid_t libcrun_status_get_pid (libcrun_status_t *st);
LIBCRUN_PUBLIC ಧ್ರುವ ವರ್ಣ *libcrun_status_get_bundle (libcrun_status_t *st);
LIBCRUN_PUBLIC ಧ್ರುವ ವರ್ಣ *libcrun_status_get_rootfs (libcrun_status_t *st);
LIBCRUN_PUBLIC ಧ್ರುವ ವರ್ಣ *libcrun_status_get_created (libcrun_status_t *st);
LIBCRUN_PUBLIC ಧ್ರುವ ವರ್ಣ *libcrun_status_get_owner (libcrun_status_t *st);
LIBCRUN_PUBLIC ಧ್ರುವ ವರ್ಣ *libcrun_status_get_scope (libcrun_status_t *st);
LIBCRUN_PUBLIC ಧ್ರುವ ವರ್ಣ *libcrun_status_get_external_descriptors (libcrun_status_t *st);
LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_get_state_string (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, ಧ್ರುವ ವರ್ಣ **out,
                                                       ಪೂರ್ಣಾಙ್ಕ *running, libcrun_error_t *err);
LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_get_state_dir (ವರ್ಣ **out, ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_status_check_directories (ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_status_create_exec_fifo (ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_status_write_exec_fifo (ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_status_has_read_exec_fifo (ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ get_run_directory (ವರ್ಣ **out, ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ get_shared_empty_directory_path (ವರ್ಣ **out, ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, libcrun_error_t *err);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
libcrun_free_container_listp (ಶೂನ್ಯ *p)
{
  libcrun_container_list_t **l = (libcrun_container_list_t **) p;
  ಯದಿ (*l != ರಿಕ್ತ)
    libcrun_free_containers_list (*l);
}

#define cleanup_container_status __attribute__ ((cleanup (libcrun_free_container_status)))
#define cleanup_container_list __attribute__ ((cleanup (libcrun_free_container_listp)))

#endif
