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

কাঠামো libcrun_container_list_s
{
  কাঠামো libcrun_container_list_s *পরবর্তী;
  অক্ষর *name;
};
প্রকারলেখ কাঠামো libcrun_container_list_s libcrun_container_list_t;

কাঠামো libcrun_container_status_s
{
  pid_t pid;
  unsigned long long process_start_time;
  অক্ষর *bundle;
  অক্ষর *rootfs;
  অক্ষর *cgroup_path;
  অক্ষর *scope;
  পূর্ণসংখ্যা systemd_cgroup;
  অক্ষর *created;
  পূর্ণসংখ্যা detached;
  পূর্ণসংখ্যা no_new_keyring;
  অক্ষর *external_descriptors;
  অক্ষর *owner;
};
প্রকারলেখ কাঠামো libcrun_container_status_s libcrun_container_status_t;

LIBCRUN_PUBLIC শূন্য libcrun_free_container_status (libcrun_container_status_t *status);
LIBCRUN_PUBLIC পূর্ণসংখ্যা libcrun_write_container_status (ধ্রুবক অক্ষর *state_মূল, ধ্রুবক অক্ষর *id,
                                                   libcrun_container_status_t *status, libcrun_error_t *err);
LIBCRUN_PUBLIC পূর্ণসংখ্যা libcrun_read_container_status (libcrun_container_status_t *status, ধ্রুবক অক্ষর *state_মূল,
                                                  ধ্রুবক অক্ষর *id, libcrun_error_t *err);
LIBCRUN_PUBLIC শূন্য libcrun_free_containers_list (libcrun_container_list_t *list);
LIBCRUN_PUBLIC পূর্ণসংখ্যা libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err);
LIBCRUN_PUBLIC পূর্ণসংখ্যা libcrun_get_state_directory (অক্ষর **out, ধ্রুবক অক্ষর *state_মূল, ধ্রুবক অক্ষর *id, libcrun_error_t *err);
LIBCRUN_PUBLIC পূর্ণসংখ্যা libcrun_container_delete_status (ধ্রুবক অক্ষর *state_মূল, ধ্রুবক অক্ষর *id, libcrun_error_t *err);
LIBCRUN_PUBLIC পূর্ণসংখ্যা libcrun_get_containers_list (libcrun_container_list_t **ret, ধ্রুবক অক্ষর *state_মূল,
                                                libcrun_error_t *err);

কাঠামো libcrun_container_iter_s
{
  libcrun_container_list_t *cur;
};
প্রকারলেখ কাঠামো libcrun_container_iter_s libcrun_container_iter_t;

LIBCRUN_PUBLIC পূর্ণসংখ্যা libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out,
                                           libcrun_error_t *err);
LIBCRUN_PUBLIC libcrun_container_iter_t *libcrun_container_list_iter (libcrun_container_list_t *l);
LIBCRUN_PUBLIC bool libcrun_container_iter_পরবর্তী (libcrun_container_iter_t *it, ধ্রুবক অক্ষর **id);
LIBCRUN_PUBLIC শূন্য libcrun_container_iter_free (libcrun_container_iter_t *it);
LIBCRUN_PUBLIC শূন্য libcrun_container_list_free (libcrun_container_list_t *l);

প্রকারলেখ গণনা
{
  LIBCRUN_CONTAINER_STATUS_CREATING = 0,
  LIBCRUN_CONTAINER_STATUS_CREATED,
  LIBCRUN_CONTAINER_STATUS_RUNNING,
  LIBCRUN_CONTAINER_STATUS_STOPPED,
  LIBCRUN_CONTAINER_STATUS_PAUSED,
} libcrun_container_state_t;

কাঠামো libcrun_status_s;
প্রকারলেখ কাঠামো libcrun_status_s libcrun_status_t;

LIBCRUN_PUBLIC পূর্ণসংখ্যা libcrun_container_status_load (libcrun_context_t *context, ধ্রুবক অক্ষর *id, libcrun_status_t **out,
                                                  libcrun_error_t *err);
LIBCRUN_PUBLIC শূন্য libcrun_container_status_free (libcrun_status_t *st);
/* Resolve the live state of the container described by ST.  The state is
   probed on the first call and cached afterwards; STATE and RUNNING can be
   NULL.  */
LIBCRUN_PUBLIC পূর্ণসংখ্যা libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, পূর্ণসংখ্যা *running,
                                             libcrun_error_t *err);
LIBCRUN_PUBLIC ধ্রুবক অক্ষর *libcrun_container_state_to_string (libcrun_container_state_t state);
LIBCRUN_PUBLIC pid_t libcrun_status_get_pid (libcrun_status_t *st);
LIBCRUN_PUBLIC ধ্রুবক অক্ষর *libcrun_status_get_bundle (libcrun_status_t *st);
LIBCRUN_PUBLIC ধ্রুবক অক্ষর *libcrun_status_get_rootfs (libcrun_status_t *st);
LIBCRUN_PUBLIC ধ্রুবক অক্ষর *libcrun_status_get_created (libcrun_status_t *st);
LIBCRUN_PUBLIC ধ্রুবক অক্ষর *libcrun_status_get_owner (libcrun_status_t *st);
LIBCRUN_PUBLIC ধ্রুবক অক্ষর *libcrun_status_get_scope (libcrun_status_t *st);
LIBCRUN_PUBLIC ধ্রুবক অক্ষর *libcrun_status_get_external_descriptors (libcrun_status_t *st);
LIBCRUN_PUBLIC পূর্ণসংখ্যা libcrun_container_get_state_string (libcrun_context_t *context, ধ্রুবক অক্ষর *id, ধ্রুবক অক্ষর **out,
                                                       পূর্ণসংখ্যা *running, libcrun_error_t *err);
LIBCRUN_PUBLIC পূর্ণসংখ্যা libcrun_get_state_dir (অক্ষর **out, ধ্রুবক অক্ষর *state_মূল, ধ্রুবক অক্ষর *id, libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_status_check_directories (ধ্রুবক অক্ষর *state_মূল, ধ্রুবক অক্ষর *id, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_status_create_exec_fifo (ধ্রুবক অক্ষর *state_মূল, ধ্রুবক অক্ষর *id, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_status_write_exec_fifo (ধ্রুবক অক্ষর *state_মূল, ধ্রুবক অক্ষর *id, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_status_has_read_exec_fifo (ধ্রুবক অক্ষর *state_মূল, ধ্রুবক অক্ষর *id, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err);
পূর্ণসংখ্যা get_run_directory (অক্ষর **out, ধ্রুবক অক্ষর *state_মূল, libcrun_error_t *err);
পূর্ণসংখ্যা get_shared_empty_directory_path (অক্ষর **out, ধ্রুবক অক্ষর *state_মূল, libcrun_error_t *err);

স্থির অন্তঃরেখ শূন্য
libcrun_free_container_listp (শূন্য *p)
{
  libcrun_container_list_t **l = (libcrun_container_list_t **) p;
  যদি (*l != ফাঁকা)
    libcrun_free_containers_list (*l);
}

#define cleanup_container_status __attribute__ ((cleanup (libcrun_free_container_status)))
#define cleanup_container_list __attribute__ ((cleanup (libcrun_free_container_listp)))

#endif
