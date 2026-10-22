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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <ocispec/runtime_spec_schema_config_schema.h>
#ଅନ୍ତର୍ଭାଵ "error.h"
#ଅନ୍ତର୍ଭାଵ "container.h"

ସଂରଚନା libcrun_container_list_s
{
  ସଂରଚନା libcrun_container_list_s *next;
  ଅକ୍ଷର *name;
};
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା libcrun_container_list_s libcrun_container_list_t;

ସଂରଚନା libcrun_container_status_s
{
  pid_t pid;
  unsigned long long process_start_time;
  ଅକ୍ଷର *bundle;
  ଅକ୍ଷର *rootfs;
  ଅକ୍ଷର *cgroup_path;
  ଅକ୍ଷର *scope;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା systemd_cgroup;
  ଅକ୍ଷର *created;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା detached;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା no_new_keyring;
  ଅକ୍ଷର *external_descriptors;
  ଅକ୍ଷର *owner;
};
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା libcrun_container_status_s libcrun_container_status_t;

LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_free_container_status (libcrun_container_status_t *status);
LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_write_container_status (ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, ଅଚଳ ଅକ୍ଷର *id,
                                                   libcrun_container_status_t *status, libcrun_error_t *err);
LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_read_container_status (libcrun_container_status_t *status, ଅଚଳ ଅକ୍ଷର *state_ମୂଳ,
                                                  ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err);
LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_free_containers_list (libcrun_container_list_t *list);
LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err);
LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_get_state_directory (ଅକ୍ଷର **out, ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err);
LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_delete_status (ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err);
LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_get_containers_list (libcrun_container_list_t **ret, ଅଚଳ ଅକ୍ଷର *state_ମୂଳ,
                                                libcrun_error_t *err);

ସଂରଚନା libcrun_container_iter_s
{
  libcrun_container_list_t *cur;
};
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା libcrun_container_iter_s libcrun_container_iter_t;

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out,
                                           libcrun_error_t *err);
LIBCRUN_PUBLIC libcrun_container_iter_t *libcrun_container_list_iter (libcrun_container_list_t *l);
LIBCRUN_PUBLIC bool libcrun_container_iter_next (libcrun_container_iter_t *it, ଅଚଳ ଅକ୍ଷର **id);
LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_container_iter_free (libcrun_container_iter_t *it);
LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_container_list_free (libcrun_container_list_t *l);

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଗଣନା
{
  LIBCRUN_CONTAINER_STATUS_CREATING = 0,
  LIBCRUN_CONTAINER_STATUS_CREATED,
  LIBCRUN_CONTAINER_STATUS_RUNNING,
  LIBCRUN_CONTAINER_STATUS_STOPPED,
  LIBCRUN_CONTAINER_STATUS_PAUSED,
} libcrun_container_state_t;

ସଂରଚନା libcrun_status_s;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା libcrun_status_s libcrun_status_t;

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_status_load (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, libcrun_status_t **out,
                                                  libcrun_error_t *err);
LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_container_status_free (libcrun_status_t *st);
/* Resolve the live state of the container described by ST.  The state is
   probed on the first call and cached afterwards; STATE and RUNNING can be
   NULL.  */
LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *running,
                                             libcrun_error_t *err);
LIBCRUN_PUBLIC ଅଚଳ ଅକ୍ଷର *libcrun_container_state_to_string (libcrun_container_state_t state);
LIBCRUN_PUBLIC pid_t libcrun_status_get_pid (libcrun_status_t *st);
LIBCRUN_PUBLIC ଅଚଳ ଅକ୍ଷର *libcrun_status_get_bundle (libcrun_status_t *st);
LIBCRUN_PUBLIC ଅଚଳ ଅକ୍ଷର *libcrun_status_get_rootfs (libcrun_status_t *st);
LIBCRUN_PUBLIC ଅଚଳ ଅକ୍ଷର *libcrun_status_get_created (libcrun_status_t *st);
LIBCRUN_PUBLIC ଅଚଳ ଅକ୍ଷର *libcrun_status_get_owner (libcrun_status_t *st);
LIBCRUN_PUBLIC ଅଚଳ ଅକ୍ଷର *libcrun_status_get_scope (libcrun_status_t *st);
LIBCRUN_PUBLIC ଅଚଳ ଅକ୍ଷର *libcrun_status_get_external_descriptors (libcrun_status_t *st);
LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_get_state_string (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, ଅଚଳ ଅକ୍ଷର **out,
                                                       ପୂର୍ଣ୍ଣସଂଖ୍ୟା *running, libcrun_error_t *err);
LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_get_state_dir (ଅକ୍ଷର **out, ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_status_check_directories (ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_status_create_exec_fifo (ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_status_write_exec_fifo (ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_status_has_read_exec_fifo (ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_run_directory (ଅକ୍ଷର **out, ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_shared_empty_directory_path (ଅକ୍ଷର **out, ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, libcrun_error_t *err);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
libcrun_free_container_listp (ଶୂନ୍ୟ *p)
{
  libcrun_container_list_t **l = (libcrun_container_list_t **) p;
  ଯଦି (*l != ଖାଲି)
    libcrun_free_containers_list (*l);
}

#define cleanup_container_status __attribute__ ((cleanup (libcrun_free_container_status)))
#define cleanup_container_list __attribute__ ((cleanup (libcrun_free_container_listp)))

#endif
