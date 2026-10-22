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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <ocispec/runtime_spec_schema_config_schema.h>
#അന്തര്ഭാവ "error.h"
#അന്തര്ഭാവ "container.h"

ഘടന libcrun_container_list_s
{
  ഘടന libcrun_container_list_s *next;
  അക്ഷരം *name;
};
തരനിർവചനം ഘടന libcrun_container_list_s libcrun_container_list_t;

ഘടന libcrun_container_status_s
{
  pid_t pid;
  unsigned long long process_start_time;
  അക്ഷരം *bundle;
  അക്ഷരം *rootfs;
  അക്ഷരം *cgroup_path;
  അക്ഷരം *scope;
  പൂർണ്ണസംഖ്യ systemd_cgroup;
  അക്ഷരം *created;
  പൂർണ്ണസംഖ്യ detached;
  പൂർണ്ണസംഖ്യ no_new_keyring;
  അക്ഷരം *external_descriptors;
  അക്ഷരം *owner;
};
തരനിർവചനം ഘടന libcrun_container_status_s libcrun_container_status_t;

LIBCRUN_PUBLIC ശൂന്യം libcrun_free_container_status (libcrun_container_status_t *status);
LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_write_container_status (സ്ഥിരം അക്ഷരം *state_വേര്, സ്ഥിരം അക്ഷരം *id,
                                                   libcrun_container_status_t *status, libcrun_error_t *err);
LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_read_container_status (libcrun_container_status_t *status, സ്ഥിരം അക്ഷരം *state_വേര്,
                                                  സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err);
LIBCRUN_PUBLIC ശൂന്യം libcrun_free_containers_list (libcrun_container_list_t *list);
LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_is_container_running (libcrun_container_status_t *status, libcrun_error_t *err);
LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_get_state_directory (അക്ഷരം **out, സ്ഥിരം അക്ഷരം *state_വേര്, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err);
LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_delete_status (സ്ഥിരം അക്ഷരം *state_വേര്, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err);
LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_get_containers_list (libcrun_container_list_t **ret, സ്ഥിരം അക്ഷരം *state_വേര്,
                                                libcrun_error_t *err);

ഘടന libcrun_container_iter_s
{
  libcrun_container_list_t *cur;
};
തരനിർവചനം ഘടന libcrun_container_iter_s libcrun_container_iter_t;

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **out,
                                           libcrun_error_t *err);
LIBCRUN_PUBLIC libcrun_container_iter_t *libcrun_container_list_iter (libcrun_container_list_t *l);
LIBCRUN_PUBLIC bool libcrun_container_iter_next (libcrun_container_iter_t *it, സ്ഥിരം അക്ഷരം **id);
LIBCRUN_PUBLIC ശൂന്യം libcrun_container_iter_free (libcrun_container_iter_t *it);
LIBCRUN_PUBLIC ശൂന്യം libcrun_container_list_free (libcrun_container_list_t *l);

തരനിർവചനം എണ്ണൽ
{
  LIBCRUN_CONTAINER_STATUS_CREATING = 0,
  LIBCRUN_CONTAINER_STATUS_CREATED,
  LIBCRUN_CONTAINER_STATUS_RUNNING,
  LIBCRUN_CONTAINER_STATUS_STOPPED,
  LIBCRUN_CONTAINER_STATUS_PAUSED,
} libcrun_container_state_t;

ഘടന libcrun_status_s;
തരനിർവചനം ഘടന libcrun_status_s libcrun_status_t;

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_status_load (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, libcrun_status_t **out,
                                                  libcrun_error_t *err);
LIBCRUN_PUBLIC ശൂന്യം libcrun_container_status_free (libcrun_status_t *st);
/* Resolve the live state of the container described by ST.  The state is
   probed on the first call and cached afterwards; STATE and RUNNING can be
   NULL.  */
LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, പൂർണ്ണസംഖ്യ *running,
                                             libcrun_error_t *err);
LIBCRUN_PUBLIC സ്ഥിരം അക്ഷരം *libcrun_container_state_to_string (libcrun_container_state_t state);
LIBCRUN_PUBLIC pid_t libcrun_status_get_pid (libcrun_status_t *st);
LIBCRUN_PUBLIC സ്ഥിരം അക്ഷരം *libcrun_status_get_bundle (libcrun_status_t *st);
LIBCRUN_PUBLIC സ്ഥിരം അക്ഷരം *libcrun_status_get_rootfs (libcrun_status_t *st);
LIBCRUN_PUBLIC സ്ഥിരം അക്ഷരം *libcrun_status_get_created (libcrun_status_t *st);
LIBCRUN_PUBLIC സ്ഥിരം അക്ഷരം *libcrun_status_get_owner (libcrun_status_t *st);
LIBCRUN_PUBLIC സ്ഥിരം അക്ഷരം *libcrun_status_get_scope (libcrun_status_t *st);
LIBCRUN_PUBLIC സ്ഥിരം അക്ഷരം *libcrun_status_get_external_descriptors (libcrun_status_t *st);
LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_get_state_string (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം **out,
                                                       പൂർണ്ണസംഖ്യ *running, libcrun_error_t *err);
LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_get_state_dir (അക്ഷരം **out, സ്ഥിരം അക്ഷരം *state_വേര്, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_status_check_directories (സ്ഥിരം അക്ഷരം *state_വേര്, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_status_create_exec_fifo (സ്ഥിരം അക്ഷരം *state_വേര്, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_status_write_exec_fifo (സ്ഥിരം അക്ഷരം *state_വേര്, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_status_has_read_exec_fifo (സ്ഥിരം അക്ഷരം *state_വേര്, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_check_pid_valid (libcrun_container_status_t *status, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ get_run_directory (അക്ഷരം **out, സ്ഥിരം അക്ഷരം *state_വേര്, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ get_shared_empty_directory_path (അക്ഷരം **out, സ്ഥിരം അക്ഷരം *state_വേര്, libcrun_error_t *err);

സ്ഥിതം വരിയിൽ ശൂന്യം
libcrun_free_container_listp (ശൂന്യം *p)
{
  libcrun_container_list_t **l = (libcrun_container_list_t **) p;
  എങ്കിൽ (*l != ശൂന്യ)
    libcrun_free_containers_list (*l);
}

#define cleanup_container_status __attribute__ ((cleanup (libcrun_free_container_status)))
#define cleanup_container_list __attribute__ ((cleanup (libcrun_free_container_listp)))

#endif
