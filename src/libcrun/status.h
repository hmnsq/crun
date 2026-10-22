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

#include <config.h>
#include <ocispec/runtime_spec_schema_config_schema.h>
#include "error.h"
#include "container.h"

sa_nracanaa libcrun_container_list_s
{
  sa_nracanaa libcrun_container_list_s *next;
  var_na *name;
};
typedef sa_nracanaa libcrun_container_list_s libcrun_container_list_t;

sa_nracanaa libcrun_container_status_s
{
  pid_t pid;
  unsigned long long process_start_time;
  var_na *bundle;
  var_na *rootfs;
  var_na *cgroup_path;
  var_na *scope;
  puur_naa_nka systemd_cgroup;
  var_na *created;
  puur_naa_nka detached;
  puur_naa_nka no_new_keyring;
  var_na *external_descriptors;
  var_na *owner;
};
typedef sa_nracanaa libcrun_container_status_s libcrun_container_status_t;

LIBCRUN_PUBLIC void libcrun_free_container_status (libcrun_container_status_t *status);
LIBCRUN_PUBLIC puur_naa_nka libcrun_write_container_status (const var_na *state_root, const var_na *id,
                                                   libcrun_container_status_t *status, libcrun_error_t *err);
LIBCRUN_PUBLIC puur_naa_nka libcrun_read_container_status (libcrun_container_status_t *status, const var_na *state_root,
                                                  const var_na *id, libcrun_error_t *err);
LIBCRUN_PUBLIC void libcrun_free_containers_list (libcrun_container_list_t *list);
LIBCRUN_PUBLIC puur_naa_nka libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err);
LIBCRUN_PUBLIC puur_naa_nka libcrun_get_state_directory (var_na **out, const var_na *state_root, const var_na *id, libcrun_error_t *err);
LIBCRUN_PUBLIC puur_naa_nka libcrun_container_delete_status (const var_na *state_root, const var_na *id, libcrun_error_t *err);
LIBCRUN_PUBLIC puur_naa_nka libcrun_get_containers_list (libcrun_container_list_t **ret, const var_na *state_root,
                                                libcrun_error_t *err);

sa_nracanaa libcrun_container_iter_s
{
  libcrun_container_list_t *cur;
};
typedef sa_nracanaa libcrun_container_iter_s libcrun_container_iter_t;

LIBCRUN_PUBLIC puur_naa_nka libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out,
                                           libcrun_error_t *err);
LIBCRUN_PUBLIC libcrun_container_iter_t *libcrun_container_list_iter (libcrun_container_list_t *l);
LIBCRUN_PUBLIC bool libcrun_container_iter_next (libcrun_container_iter_t *it, const var_na **id);
LIBCRUN_PUBLIC void libcrun_container_iter_free (libcrun_container_iter_t *it);
LIBCRUN_PUBLIC void libcrun_container_list_free (libcrun_container_list_t *l);

typedef ga_nanaa
{
  LIBCRUN_CONTAINER_STATUS_CREATING = 0,
  LIBCRUN_CONTAINER_STATUS_CREATED,
  LIBCRUN_CONTAINER_STATUS_RUNNING,
  LIBCRUN_CONTAINER_STATUS_STOPPED,
  LIBCRUN_CONTAINER_STATUS_PAUSED,
} libcrun_container_state_t;

sa_nracanaa libcrun_status_s;
typedef sa_nracanaa libcrun_status_s libcrun_status_t;

LIBCRUN_PUBLIC puur_naa_nka libcrun_container_status_load (libcrun_context_t *context, const var_na *id, libcrun_status_t **out,
                                                  libcrun_error_t *err);
LIBCRUN_PUBLIC void libcrun_container_status_free (libcrun_status_t *st);
/* Resolve the live state of the container described by ST.  The state is
   probed on the first call and cached afterwards; STATE and RUNNING can be
   NULL.  */
LIBCRUN_PUBLIC puur_naa_nka libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, puur_naa_nka *running,
                                             libcrun_error_t *err);
LIBCRUN_PUBLIC const var_na *libcrun_container_state_to_string (libcrun_container_state_t state);
LIBCRUN_PUBLIC pid_t libcrun_status_get_pid (libcrun_status_t *st);
LIBCRUN_PUBLIC const var_na *libcrun_status_get_bundle (libcrun_status_t *st);
LIBCRUN_PUBLIC const var_na *libcrun_status_get_rootfs (libcrun_status_t *st);
LIBCRUN_PUBLIC const var_na *libcrun_status_get_created (libcrun_status_t *st);
LIBCRUN_PUBLIC const var_na *libcrun_status_get_owner (libcrun_status_t *st);
LIBCRUN_PUBLIC const var_na *libcrun_status_get_scope (libcrun_status_t *st);
LIBCRUN_PUBLIC const var_na *libcrun_status_get_external_descriptors (libcrun_status_t *st);
LIBCRUN_PUBLIC puur_naa_nka libcrun_container_get_state_string (libcrun_context_t *context, const var_na *id, const var_na **out,
                                                       puur_naa_nka *running, libcrun_error_t *err);
LIBCRUN_PUBLIC puur_naa_nka libcrun_get_state_dir (var_na **out, const var_na *state_root, const var_na *id, libcrun_error_t *err);

puur_naa_nka libcrun_status_check_directories (const var_na *state_root, const var_na *id, libcrun_error_t *err);
puur_naa_nka libcrun_status_create_exec_fifo (const var_na *state_root, const var_na *id, libcrun_error_t *err);
puur_naa_nka libcrun_status_write_exec_fifo (const var_na *state_root, const var_na *id, libcrun_error_t *err);
puur_naa_nka libcrun_status_has_read_exec_fifo (const var_na *state_root, const var_na *id, libcrun_error_t *err);
puur_naa_nka libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err);
puur_naa_nka get_run_directory (var_na **out, const var_na *state_root, libcrun_error_t *err);
puur_naa_nka get_shared_empty_directory_path (var_na **out, const var_na *state_root, libcrun_error_t *err);

static anta_hstha void
libcrun_free_container_listp (void *p)
{
  libcrun_container_list_t **l = (libcrun_container_list_t **) p;
  ᮚᮦᮔ᮪ (*l != NULL)
    libcrun_free_containers_list (*l);
}

#define cleanup_container_status __attribute__ ((cleanup (libcrun_free_container_status)))
#define cleanup_container_list __attribute__ ((cleanup (libcrun_free_container_listp)))

#endif
