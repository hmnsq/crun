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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <ocispec/runtime_spec_schema_config_schema.h>
#அந்தர்பாவ "error.h"
#அந்தர்பாவ "container.h"

கட்டமைப்பு libcrun_container_list_s
{
  கட்டமைப்பு libcrun_container_list_s *next;
  எழுத்து *name;
};
வகைவரை கட்டமைப்பு libcrun_container_list_s libcrun_container_list_t;

கட்டமைப்பு libcrun_container_status_s
{
  pid_t pid;
  unsigned long long process_start_time;
  எழுத்து *bundle;
  எழுத்து *rootfs;
  எழுத்து *cgroup_path;
  எழுத்து *scope;
  முழு systemd_cgroup;
  எழுத்து *created;
  முழு detached;
  முழு no_new_keyring;
  எழுத்து *external_descriptors;
  எழுத்து *owner;
};
வகைவரை கட்டமைப்பு libcrun_container_status_s libcrun_container_status_t;

LIBCRUN_PUBLIC வெறுமை libcrun_free_container_status (libcrun_container_status_t *status);
LIBCRUN_PUBLIC முழு libcrun_write_container_status (மாறிலி எழுத்து *state_வேர், மாறிலி எழுத்து *id,
                                                   libcrun_container_status_t *status, libcrun_error_t *err);
LIBCRUN_PUBLIC முழு libcrun_read_container_status (libcrun_container_status_t *status, மாறிலி எழுத்து *state_வேர்,
                                                  மாறிலி எழுத்து *id, libcrun_error_t *err);
LIBCRUN_PUBLIC வெறுமை libcrun_free_containers_list (libcrun_container_list_t *list);
LIBCRUN_PUBLIC முழு libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err);
LIBCRUN_PUBLIC முழு libcrun_get_state_directory (எழுத்து **out, மாறிலி எழுத்து *state_வேர், மாறிலி எழுத்து *id, libcrun_error_t *err);
LIBCRUN_PUBLIC முழு libcrun_container_delete_status (மாறிலி எழுத்து *state_வேர், மாறிலி எழுத்து *id, libcrun_error_t *err);
LIBCRUN_PUBLIC முழு libcrun_get_containers_list (libcrun_container_list_t **ret, மாறிலி எழுத்து *state_வேர்,
                                                libcrun_error_t *err);

கட்டமைப்பு libcrun_container_iter_s
{
  libcrun_container_list_t *cur;
};
வகைவரை கட்டமைப்பு libcrun_container_iter_s libcrun_container_iter_t;

LIBCRUN_PUBLIC முழு libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out,
                                           libcrun_error_t *err);
LIBCRUN_PUBLIC libcrun_container_iter_t *libcrun_container_list_iter (libcrun_container_list_t *l);
LIBCRUN_PUBLIC bool libcrun_container_iter_next (libcrun_container_iter_t *it, மாறிலி எழுத்து **id);
LIBCRUN_PUBLIC வெறுமை libcrun_container_iter_free (libcrun_container_iter_t *it);
LIBCRUN_PUBLIC வெறுமை libcrun_container_list_free (libcrun_container_list_t *l);

வகைவரை எண்ணிக்கை
{
  LIBCRUN_CONTAINER_STATUS_CREATING = 0,
  LIBCRUN_CONTAINER_STATUS_CREATED,
  LIBCRUN_CONTAINER_STATUS_RUNNING,
  LIBCRUN_CONTAINER_STATUS_STOPPED,
  LIBCRUN_CONTAINER_STATUS_PAUSED,
} libcrun_container_state_t;

கட்டமைப்பு libcrun_status_s;
வகைவரை கட்டமைப்பு libcrun_status_s libcrun_status_t;

LIBCRUN_PUBLIC முழு libcrun_container_status_load (libcrun_context_t *context, மாறிலி எழுத்து *id, libcrun_status_t **out,
                                                  libcrun_error_t *err);
LIBCRUN_PUBLIC வெறுமை libcrun_container_status_free (libcrun_status_t *st);
/* Resolve the live state of the container described by ST.  The state is
   probed on the first call and cached afterwards; STATE and RUNNING can be
   NULL.  */
LIBCRUN_PUBLIC முழு libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, முழு *running,
                                             libcrun_error_t *err);
LIBCRUN_PUBLIC மாறிலி எழுத்து *libcrun_container_state_to_string (libcrun_container_state_t state);
LIBCRUN_PUBLIC pid_t libcrun_status_get_pid (libcrun_status_t *st);
LIBCRUN_PUBLIC மாறிலி எழுத்து *libcrun_status_get_bundle (libcrun_status_t *st);
LIBCRUN_PUBLIC மாறிலி எழுத்து *libcrun_status_get_rootfs (libcrun_status_t *st);
LIBCRUN_PUBLIC மாறிலி எழுத்து *libcrun_status_get_created (libcrun_status_t *st);
LIBCRUN_PUBLIC மாறிலி எழுத்து *libcrun_status_get_owner (libcrun_status_t *st);
LIBCRUN_PUBLIC மாறிலி எழுத்து *libcrun_status_get_scope (libcrun_status_t *st);
LIBCRUN_PUBLIC மாறிலி எழுத்து *libcrun_status_get_external_descriptors (libcrun_status_t *st);
LIBCRUN_PUBLIC முழு libcrun_container_get_state_string (libcrun_context_t *context, மாறிலி எழுத்து *id, மாறிலி எழுத்து **out,
                                                       முழு *running, libcrun_error_t *err);
LIBCRUN_PUBLIC முழு libcrun_get_state_dir (எழுத்து **out, மாறிலி எழுத்து *state_வேர், மாறிலி எழுத்து *id, libcrun_error_t *err);

முழு libcrun_status_check_directories (மாறிலி எழுத்து *state_வேர், மாறிலி எழுத்து *id, libcrun_error_t *err);
முழு libcrun_status_create_exec_fifo (மாறிலி எழுத்து *state_வேர், மாறிலி எழுத்து *id, libcrun_error_t *err);
முழு libcrun_status_write_exec_fifo (மாறிலி எழுத்து *state_வேர், மாறிலி எழுத்து *id, libcrun_error_t *err);
முழு libcrun_status_has_read_exec_fifo (மாறிலி எழுத்து *state_வேர், மாறிலி எழுத்து *id, libcrun_error_t *err);
முழு libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err);
முழு get_run_directory (எழுத்து **out, மாறிலி எழுத்து *state_வேர், libcrun_error_t *err);
முழு get_shared_empty_directory_path (எழுத்து **out, மாறிலி எழுத்து *state_வேர், libcrun_error_t *err);

நிலையான உள்வரி வெறுமை
libcrun_free_container_listp (வெறுமை *p)
{
  libcrun_container_list_t **l = (libcrun_container_list_t **) p;
  எனில் (*l != வெற்று)
    libcrun_free_containers_list (*l);
}

#define cleanup_container_status __attribute__ ((cleanup (libcrun_free_container_status)))
#define cleanup_container_list __attribute__ ((cleanup (libcrun_free_container_listp)))

#endif
