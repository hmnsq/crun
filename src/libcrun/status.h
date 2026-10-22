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

#شامل <config.h>
#شامل <ocispec/runtime_spec_schema_config_schema.h>
#شامل "error.h"
#شامل "container.h"

ساختار libcrun_container_list_s
{
  ساختار libcrun_container_list_s *next;
  نویسه *name;
};
تعریفنوع ساختار libcrun_container_list_s libcrun_container_list_t;

ساختار libcrun_container_status_s
{
  pid_t pid;
  unsigned long long process_start_time;
  نویسه *bundle;
  نویسه *rootfs;
  نویسه *cgroup_path;
  نویسه *scope;
  صحیح systemd_cgroup;
  نویسه *created;
  صحیح detached;
  صحیح no_new_keyring;
  نویسه *external_descriptors;
  نویسه *owner;
};
تعریفنوع ساختار libcrun_container_status_s libcrun_container_status_t;

LIBCRUN_PUBLIC تهی libcrun_free_container_status (libcrun_container_status_t *status);
LIBCRUN_PUBLIC صحیح libcrun_write_container_status (ثابت نویسه *state_ریشه, ثابت نویسه *id,
                                                   libcrun_container_status_t *status, libcrun_error_t *err);
LIBCRUN_PUBLIC صحیح libcrun_read_container_status (libcrun_container_status_t *status, ثابت نویسه *state_ریشه,
                                                  ثابت نویسه *id, libcrun_error_t *err);
LIBCRUN_PUBLIC تهی libcrun_free_containers_list (libcrun_container_list_t *list);
LIBCRUN_PUBLIC صحیح libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err);
LIBCRUN_PUBLIC صحیح libcrun_get_state_directory (نویسه **out, ثابت نویسه *state_ریشه, ثابت نویسه *id, libcrun_error_t *err);
LIBCRUN_PUBLIC صحیح libcrun_container_delete_status (ثابت نویسه *state_ریشه, ثابت نویسه *id, libcrun_error_t *err);
LIBCRUN_PUBLIC صحیح libcrun_get_containers_list (libcrun_container_list_t **ret, ثابت نویسه *state_ریشه,
                                                libcrun_error_t *err);

ساختار libcrun_container_iter_s
{
  libcrun_container_list_t *cur;
};
تعریفنوع ساختار libcrun_container_iter_s libcrun_container_iter_t;

LIBCRUN_PUBLIC صحیح libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out,
                                           libcrun_error_t *err);
LIBCRUN_PUBLIC libcrun_container_iter_t *libcrun_container_list_iter (libcrun_container_list_t *l);
LIBCRUN_PUBLIC bool libcrun_container_iter_next (libcrun_container_iter_t *it, ثابت نویسه **id);
LIBCRUN_PUBLIC تهی libcrun_container_iter_free (libcrun_container_iter_t *it);
LIBCRUN_PUBLIC تهی libcrun_container_list_free (libcrun_container_list_t *l);

تعریفنوع شمارش
{
  LIBCRUN_CONTAINER_STATUS_CREATING = 0,
  LIBCRUN_CONTAINER_STATUS_CREATED,
  LIBCRUN_CONTAINER_STATUS_RUNNING,
  LIBCRUN_CONTAINER_STATUS_STOPPED,
  LIBCRUN_CONTAINER_STATUS_PAUSED,
} libcrun_container_state_t;

ساختار libcrun_status_s;
تعریفنوع ساختار libcrun_status_s libcrun_status_t;

LIBCRUN_PUBLIC صحیح libcrun_container_status_load (libcrun_context_t *context, ثابت نویسه *id, libcrun_status_t **out,
                                                  libcrun_error_t *err);
LIBCRUN_PUBLIC تهی libcrun_container_status_free (libcrun_status_t *st);
/* Resolve the live state of the container described by ST.  The state is
   probed on the first call and cached afterwards; STATE and RUNNING can be
   NULL.  */
LIBCRUN_PUBLIC صحیح libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, صحیح *running,
                                             libcrun_error_t *err);
LIBCRUN_PUBLIC ثابت نویسه *libcrun_container_state_to_string (libcrun_container_state_t state);
LIBCRUN_PUBLIC pid_t libcrun_status_get_pid (libcrun_status_t *st);
LIBCRUN_PUBLIC ثابت نویسه *libcrun_status_get_bundle (libcrun_status_t *st);
LIBCRUN_PUBLIC ثابت نویسه *libcrun_status_get_rootfs (libcrun_status_t *st);
LIBCRUN_PUBLIC ثابت نویسه *libcrun_status_get_created (libcrun_status_t *st);
LIBCRUN_PUBLIC ثابت نویسه *libcrun_status_get_owner (libcrun_status_t *st);
LIBCRUN_PUBLIC ثابت نویسه *libcrun_status_get_scope (libcrun_status_t *st);
LIBCRUN_PUBLIC ثابت نویسه *libcrun_status_get_external_descriptors (libcrun_status_t *st);
LIBCRUN_PUBLIC صحیح libcrun_container_get_state_string (libcrun_context_t *context, ثابت نویسه *id, ثابت نویسه **out,
                                                       صحیح *running, libcrun_error_t *err);
LIBCRUN_PUBLIC صحیح libcrun_get_state_dir (نویسه **out, ثابت نویسه *state_ریشه, ثابت نویسه *id, libcrun_error_t *err);

صحیح libcrun_status_check_directories (ثابت نویسه *state_ریشه, ثابت نویسه *id, libcrun_error_t *err);
صحیح libcrun_status_create_exec_fifo (ثابت نویسه *state_ریشه, ثابت نویسه *id, libcrun_error_t *err);
صحیح libcrun_status_write_exec_fifo (ثابت نویسه *state_ریشه, ثابت نویسه *id, libcrun_error_t *err);
صحیح libcrun_status_has_read_exec_fifo (ثابت نویسه *state_ریشه, ثابت نویسه *id, libcrun_error_t *err);
صحیح libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err);
صحیح get_run_directory (نویسه **out, ثابت نویسه *state_ریشه, libcrun_error_t *err);
صحیح get_shared_empty_directory_path (نویسه **out, ثابت نویسه *state_ریشه, libcrun_error_t *err);

ایستا درونخطی تهی
libcrun_free_container_listp (تهی *p)
{
  libcrun_container_list_t **l = (libcrun_container_list_t **) p;
  اگر (*l != پوچ)
    libcrun_free_containers_list (*l);
}

#define cleanup_container_status __attribute__ ((cleanup (libcrun_free_container_status)))
#define cleanup_container_list __attribute__ ((cleanup (libcrun_free_container_listp)))

#endif
