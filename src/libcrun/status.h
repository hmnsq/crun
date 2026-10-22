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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <ocispec/runtime_spec_schema_config_schema.h>
#અન્તર્ભાવ "error.h"
#અન્તર્ભાવ "container.h"

સંરચના libcrun_container_list_s
{
  સંરચના libcrun_container_list_s *next;
  અક્ષર *name;
};
પ્રકારવ્યાખ્યા સંરચના libcrun_container_list_s libcrun_container_list_t;

સંરચના libcrun_container_status_s
{
  pid_t pid;
  unsigned long long process_start_time;
  અક્ષર *bundle;
  અક્ષર *rootfs;
  અક્ષર *cgroup_path;
  અક્ષર *scope;
  પૂર્ણાંક systemd_cgroup;
  અક્ષર *created;
  પૂર્ણાંક detached;
  પૂર્ણાંક no_new_keyring;
  અક્ષર *external_descriptors;
  અક્ષર *owner;
};
પ્રકારવ્યાખ્યા સંરચના libcrun_container_status_s libcrun_container_status_t;

LIBCRUN_PUBLIC શૂન્ય libcrun_free_container_status (libcrun_container_status_t *status);
LIBCRUN_PUBLIC પૂર્ણાંક libcrun_write_container_status (અચલ અક્ષર *state_મૂળ, અચલ અક્ષર *id,
                                                   libcrun_container_status_t *status, libcrun_error_t *err);
LIBCRUN_PUBLIC પૂર્ણાંક libcrun_read_container_status (libcrun_container_status_t *status, અચલ અક્ષર *state_મૂળ,
                                                  અચલ અક્ષર *id, libcrun_error_t *err);
LIBCRUN_PUBLIC શૂન્ય libcrun_free_containers_list (libcrun_container_list_t *list);
LIBCRUN_PUBLIC પૂર્ણાંક libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err);
LIBCRUN_PUBLIC પૂર્ણાંક libcrun_get_state_directory (અક્ષર **out, અચલ અક્ષર *state_મૂળ, અચલ અક્ષર *id, libcrun_error_t *err);
LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_delete_status (અચલ અક્ષર *state_મૂળ, અચલ અક્ષર *id, libcrun_error_t *err);
LIBCRUN_PUBLIC પૂર્ણાંક libcrun_get_containers_list (libcrun_container_list_t **ret, અચલ અક્ષર *state_મૂળ,
                                                libcrun_error_t *err);

સંરચના libcrun_container_iter_s
{
  libcrun_container_list_t *cur;
};
પ્રકારવ્યાખ્યા સંરચના libcrun_container_iter_s libcrun_container_iter_t;

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out,
                                           libcrun_error_t *err);
LIBCRUN_PUBLIC libcrun_container_iter_t *libcrun_container_list_iter (libcrun_container_list_t *l);
LIBCRUN_PUBLIC bool libcrun_container_iter_next (libcrun_container_iter_t *it, અચલ અક્ષર **id);
LIBCRUN_PUBLIC શૂન્ય libcrun_container_iter_free (libcrun_container_iter_t *it);
LIBCRUN_PUBLIC શૂન્ય libcrun_container_list_free (libcrun_container_list_t *l);

પ્રકારવ્યાખ્યા ગણના
{
  LIBCRUN_CONTAINER_STATUS_CREATING = 0,
  LIBCRUN_CONTAINER_STATUS_CREATED,
  LIBCRUN_CONTAINER_STATUS_RUNNING,
  LIBCRUN_CONTAINER_STATUS_STOPPED,
  LIBCRUN_CONTAINER_STATUS_PAUSED,
} libcrun_container_state_t;

સંરચના libcrun_status_s;
પ્રકારવ્યાખ્યા સંરચના libcrun_status_s libcrun_status_t;

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_status_load (libcrun_context_t *context, અચલ અક્ષર *id, libcrun_status_t **out,
                                                  libcrun_error_t *err);
LIBCRUN_PUBLIC શૂન્ય libcrun_container_status_free (libcrun_status_t *st);
/* Resolve the live state of the container described by ST.  The state is
   probed on the first call and cached afterwards; STATE and RUNNING can be
   NULL.  */
LIBCRUN_PUBLIC પૂર્ણાંક libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, પૂર્ણાંક *running,
                                             libcrun_error_t *err);
LIBCRUN_PUBLIC અચલ અક્ષર *libcrun_container_state_to_string (libcrun_container_state_t state);
LIBCRUN_PUBLIC pid_t libcrun_status_get_pid (libcrun_status_t *st);
LIBCRUN_PUBLIC અચલ અક્ષર *libcrun_status_get_bundle (libcrun_status_t *st);
LIBCRUN_PUBLIC અચલ અક્ષર *libcrun_status_get_rootfs (libcrun_status_t *st);
LIBCRUN_PUBLIC અચલ અક્ષર *libcrun_status_get_created (libcrun_status_t *st);
LIBCRUN_PUBLIC અચલ અક્ષર *libcrun_status_get_owner (libcrun_status_t *st);
LIBCRUN_PUBLIC અચલ અક્ષર *libcrun_status_get_scope (libcrun_status_t *st);
LIBCRUN_PUBLIC અચલ અક્ષર *libcrun_status_get_external_descriptors (libcrun_status_t *st);
LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_get_state_string (libcrun_context_t *context, અચલ અક્ષર *id, અચલ અક્ષર **out,
                                                       પૂર્ણાંક *running, libcrun_error_t *err);
LIBCRUN_PUBLIC પૂર્ણાંક libcrun_get_state_dir (અક્ષર **out, અચલ અક્ષર *state_મૂળ, અચલ અક્ષર *id, libcrun_error_t *err);

પૂર્ણાંક libcrun_status_check_directories (અચલ અક્ષર *state_મૂળ, અચલ અક્ષર *id, libcrun_error_t *err);
પૂર્ણાંક libcrun_status_create_exec_fifo (અચલ અક્ષર *state_મૂળ, અચલ અક્ષર *id, libcrun_error_t *err);
પૂર્ણાંક libcrun_status_write_exec_fifo (અચલ અક્ષર *state_મૂળ, અચલ અક્ષર *id, libcrun_error_t *err);
પૂર્ણાંક libcrun_status_has_read_exec_fifo (અચલ અક્ષર *state_મૂળ, અચલ અક્ષર *id, libcrun_error_t *err);
પૂર્ણાંક libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err);
પૂર્ણાંક get_run_directory (અક્ષર **out, અચલ અક્ષર *state_મૂળ, libcrun_error_t *err);
પૂર્ણાંક get_shared_empty_directory_path (અક્ષર **out, અચલ અક્ષર *state_મૂળ, libcrun_error_t *err);

સ્થિર અંતઃરેખ શૂન્ય
libcrun_free_container_listp (શૂન્ય *p)
{
  libcrun_container_list_t **l = (libcrun_container_list_t **) p;
  જો (*l != ખાલી)
    libcrun_free_containers_list (*l);
}

#define cleanup_container_status __attribute__ ((cleanup (libcrun_free_container_status)))
#define cleanup_container_list __attribute__ ((cleanup (libcrun_free_container_listp)))

#endif
