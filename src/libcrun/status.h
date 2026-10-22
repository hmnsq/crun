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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <ocispec/runtime_spec_schema_config_schema.h>
#অন্তর্ভাব "error.h"
#অন্তর্ভাব "container.h"

সংরচনা libcrun_container_list_s
{
  সংরচনা libcrun_container_list_s *next;
  বর্ণ *name;
};
প্রকারনির্দেশ সংরচনা libcrun_container_list_s libcrun_container_list_t;

সংরচনা libcrun_container_status_s
{
  pid_t pid;
  unsigned long long process_start_time;
  বর্ণ *bundle;
  বর্ণ *rootfs;
  বর্ণ *cgroup_path;
  বর্ণ *scope;
  পূর্ণাঙ্ক systemd_cgroup;
  বর্ণ *created;
  পূর্ণাঙ্ক detached;
  পূর্ণাঙ্ক no_new_keyring;
  বর্ণ *external_descriptors;
  বর্ণ *owner;
};
প্রকারনির্দেশ সংরচনা libcrun_container_status_s libcrun_container_status_t;

LIBCRUN_PUBLIC শূন্য libcrun_free_container_status (libcrun_container_status_t *status);
LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_write_container_status (ধ্রুব বর্ণ *state_মূল, ধ্রুব বর্ণ *id,
                                                   libcrun_container_status_t *status, libcrun_error_t *err);
LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_read_container_status (libcrun_container_status_t *status, ধ্রুব বর্ণ *state_মূল,
                                                  ধ্রুব বর্ণ *id, libcrun_error_t *err);
LIBCRUN_PUBLIC শূন্য libcrun_free_containers_list (libcrun_container_list_t *list);
LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err);
LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_get_state_directory (বর্ণ **out, ধ্রুব বর্ণ *state_মূল, ধ্রুব বর্ণ *id, libcrun_error_t *err);
LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_delete_status (ধ্রুব বর্ণ *state_মূল, ধ্রুব বর্ণ *id, libcrun_error_t *err);
LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_get_containers_list (libcrun_container_list_t **ret, ধ্রুব বর্ণ *state_মূল,
                                                libcrun_error_t *err);

সংরচনা libcrun_container_iter_s
{
  libcrun_container_list_t *cur;
};
প্রকারনির্দেশ সংরচনা libcrun_container_iter_s libcrun_container_iter_t;

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out,
                                           libcrun_error_t *err);
LIBCRUN_PUBLIC libcrun_container_iter_t *libcrun_container_list_iter (libcrun_container_list_t *l);
LIBCRUN_PUBLIC bool libcrun_container_iter_next (libcrun_container_iter_t *it, ধ্রুব বর্ণ **id);
LIBCRUN_PUBLIC শূন্য libcrun_container_iter_free (libcrun_container_iter_t *it);
LIBCRUN_PUBLIC শূন্য libcrun_container_list_free (libcrun_container_list_t *l);

প্রকারনির্দেশ গণনা
{
  LIBCRUN_CONTAINER_STATUS_CREATING = 0,
  LIBCRUN_CONTAINER_STATUS_CREATED,
  LIBCRUN_CONTAINER_STATUS_RUNNING,
  LIBCRUN_CONTAINER_STATUS_STOPPED,
  LIBCRUN_CONTAINER_STATUS_PAUSED,
} libcrun_container_state_t;

সংরচনা libcrun_status_s;
প্রকারনির্দেশ সংরচনা libcrun_status_s libcrun_status_t;

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_status_load (libcrun_context_t *context, ধ্রুব বর্ণ *id, libcrun_status_t **out,
                                                  libcrun_error_t *err);
LIBCRUN_PUBLIC শূন্য libcrun_container_status_free (libcrun_status_t *st);
/* Resolve the live state of the container described by ST.  The state is
   probed on the first call and cached afterwards; STATE and RUNNING can be
   NULL.  */
LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, পূর্ণাঙ্ক *running,
                                             libcrun_error_t *err);
LIBCRUN_PUBLIC ধ্রুব বর্ণ *libcrun_container_state_to_string (libcrun_container_state_t state);
LIBCRUN_PUBLIC pid_t libcrun_status_get_pid (libcrun_status_t *st);
LIBCRUN_PUBLIC ধ্রুব বর্ণ *libcrun_status_get_bundle (libcrun_status_t *st);
LIBCRUN_PUBLIC ধ্রুব বর্ণ *libcrun_status_get_rootfs (libcrun_status_t *st);
LIBCRUN_PUBLIC ধ্রুব বর্ণ *libcrun_status_get_created (libcrun_status_t *st);
LIBCRUN_PUBLIC ধ্রুব বর্ণ *libcrun_status_get_owner (libcrun_status_t *st);
LIBCRUN_PUBLIC ধ্রুব বর্ণ *libcrun_status_get_scope (libcrun_status_t *st);
LIBCRUN_PUBLIC ধ্রুব বর্ণ *libcrun_status_get_external_descriptors (libcrun_status_t *st);
LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_get_state_string (libcrun_context_t *context, ধ্রুব বর্ণ *id, ধ্রুব বর্ণ **out,
                                                       পূর্ণাঙ্ক *running, libcrun_error_t *err);
LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_get_state_dir (বর্ণ **out, ধ্রুব বর্ণ *state_মূল, ধ্রুব বর্ণ *id, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_status_check_directories (ধ্রুব বর্ণ *state_মূল, ধ্রুব বর্ণ *id, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_status_create_exec_fifo (ধ্রুব বর্ণ *state_মূল, ধ্রুব বর্ণ *id, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_status_write_exec_fifo (ধ্রুব বর্ণ *state_মূল, ধ্রুব বর্ণ *id, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_status_has_read_exec_fifo (ধ্রুব বর্ণ *state_মূল, ধ্রুব বর্ণ *id, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err);
পূর্ণাঙ্ক get_run_directory (বর্ণ **out, ধ্রুব বর্ণ *state_মূল, libcrun_error_t *err);
পূর্ণাঙ্ক get_shared_empty_directory_path (বর্ণ **out, ধ্রুব বর্ণ *state_মূল, libcrun_error_t *err);

স্থির অন্তঃস্থ শূন্য
libcrun_free_container_listp (শূন্য *p)
{
  libcrun_container_list_t **l = (libcrun_container_list_t **) p;
  যদি (*l != রিক্ত)
    libcrun_free_containers_list (*l);
}

#define cleanup_container_status __attribute__ ((cleanup (libcrun_free_container_status)))
#define cleanup_container_list __attribute__ ((cleanup (libcrun_free_container_listp)))

#endif
