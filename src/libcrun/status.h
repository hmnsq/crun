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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਸਥਿਤੀ_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸਥਿਤੀ_H

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ocispec/runtime_spec_schema_config_schema.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "error.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "container.h"

ਬਣਤਰ libcrun_container_list_s
{
  ਬਣਤਰ libcrun_container_list_s *ਅਗਲਾ;
  ਅੱਖਰ *name;
};
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ libcrun_container_list_s libcrun_container_list_t;

ਬਣਤਰ libcrun_container_ਸਥਿਤੀ_s
{
  pid_t pid;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ process_ਸ਼ੁਰੂ_ਸਮਾਂ;
  ਅੱਖਰ *bundle;
  ਅੱਖਰ *rootfs;
  ਅੱਖਰ *cgroup_ਰਸਤਾ;
  ਅੱਖਰ *scope;
  ਪੂਰਨ systemd_cgroup;
  ਅੱਖਰ *created;
  ਪੂਰਨ detached;
  ਪੂਰਨ no_new_keyring;
  ਅੱਖਰ *external_descriptors;
  ਅੱਖਰ *owner;
};
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ libcrun_container_ਸਥਿਤੀ_s libcrun_container_ਸਥਿਤੀ_t;

LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_free_container_ਸਥਿਤੀ (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ);
LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_write_container_ਸਥਿਤੀ (ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ *id,
                                                   libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, libcrun_error_t *err);
LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_read_container_ਸਥਿਤੀ (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ,
                                                  ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err);
LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_free_containers_list (libcrun_container_list_t *list);
LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_is_container_running (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, libcrun_error_t *err);
LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_get_state_directory (ਅੱਖਰ **ਬਾਹਰਲਾ, ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err);
LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_delete_ਸਥਿਤੀ (ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err);
LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_get_containers_list (libcrun_container_list_t **ret, ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ,
                                                libcrun_error_t *err);

ਬਣਤਰ libcrun_container_iter_s
{
  libcrun_container_list_t *cur;
};
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ libcrun_container_iter_s libcrun_container_iter_t;

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **ਬਾਹਰਲਾ,
                                           libcrun_error_t *err);
LIBCRUN_ਜਨਤਕ libcrun_container_iter_t *libcrun_container_list_iter (libcrun_container_list_t *l);
LIBCRUN_ਜਨਤਕ bool libcrun_container_iter_ਅਗਲਾ (libcrun_container_iter_t *it, ਅਚਲ ਅੱਖਰ **id);
LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_container_iter_free (libcrun_container_iter_t *it);
LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_container_list_free (libcrun_container_list_t *l);

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ
{
  LIBCRUN_CONTAINER_ਸਥਿਤੀ_CREATING = 0,
  LIBCRUN_CONTAINER_ਸਥਿਤੀ_CREATED,
  LIBCRUN_CONTAINER_ਸਥਿਤੀ_RUNNING,
  LIBCRUN_CONTAINER_ਸਥਿਤੀ_STOPPED,
  LIBCRUN_CONTAINER_ਸਥਿਤੀ_PAUSED,
} libcrun_container_state_t;

ਬਣਤਰ libcrun_ਸਥਿਤੀ_s;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ libcrun_ਸਥਿਤੀ_s libcrun_ਸਥਿਤੀ_t;

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_ਸਥਿਤੀ_ਲੱਦੋ (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, libcrun_ਸਥਿਤੀ_t **ਬਾਹਰਲਾ,
                                                  libcrun_error_t *err);
LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_container_ਸਥਿਤੀ_free (libcrun_ਸਥਿਤੀ_t *st);
/* Resolve the live state of the container described by ST.  The state is
   probed on the first call and cached afterwards; STATE and RUNNING can be
   NULL.  */
LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_ਸਥਿਤੀ_get_state (libcrun_ਸਥਿਤੀ_t *st, libcrun_container_state_t *state, ਪੂਰਨ *running,
                                             libcrun_error_t *err);
LIBCRUN_ਜਨਤਕ ਅਚਲ ਅੱਖਰ *libcrun_container_state_ਮੰਜ਼ਿਲ_string (libcrun_container_state_t state);
LIBCRUN_ਜਨਤਕ pid_t libcrun_ਸਥਿਤੀ_get_pid (libcrun_ਸਥਿਤੀ_t *st);
LIBCRUN_ਜਨਤਕ ਅਚਲ ਅੱਖਰ *libcrun_ਸਥਿਤੀ_get_bundle (libcrun_ਸਥਿਤੀ_t *st);
LIBCRUN_ਜਨਤਕ ਅਚਲ ਅੱਖਰ *libcrun_ਸਥਿਤੀ_get_rootfs (libcrun_ਸਥਿਤੀ_t *st);
LIBCRUN_ਜਨਤਕ ਅਚਲ ਅੱਖਰ *libcrun_ਸਥਿਤੀ_get_created (libcrun_ਸਥਿਤੀ_t *st);
LIBCRUN_ਜਨਤਕ ਅਚਲ ਅੱਖਰ *libcrun_ਸਥਿਤੀ_get_owner (libcrun_ਸਥਿਤੀ_t *st);
LIBCRUN_ਜਨਤਕ ਅਚਲ ਅੱਖਰ *libcrun_ਸਥਿਤੀ_get_scope (libcrun_ਸਥਿਤੀ_t *st);
LIBCRUN_ਜਨਤਕ ਅਚਲ ਅੱਖਰ *libcrun_ਸਥਿਤੀ_get_external_descriptors (libcrun_ਸਥਿਤੀ_t *st);
LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_get_state_string (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ **ਬਾਹਰਲਾ,
                                                       ਪੂਰਨ *running, libcrun_error_t *err);
LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_get_state_dir (ਅੱਖਰ **ਬਾਹਰਲਾ, ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err);

ਪੂਰਨ libcrun_ਸਥਿਤੀ_check_directories (ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err);
ਪੂਰਨ libcrun_ਸਥਿਤੀ_create_exec_fifo (ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err);
ਪੂਰਨ libcrun_ਸਥਿਤੀ_write_exec_fifo (ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err);
ਪੂਰਨ libcrun_ਸਥਿਤੀ_has_read_exec_fifo (ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err);
ਪੂਰਨ libcrun_check_pid_valid (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, libcrun_error_t *err);
ਪੂਰਨ get_ਲੜੀਵਾਰ_directory (ਅੱਖਰ **ਬਾਹਰਲਾ, ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, libcrun_error_t *err);
ਪੂਰਨ get_shared_ਖਾਲੀ_ਥਾਵਾਂ_directory_ਰਸਤਾ (ਅੱਖਰ **ਬਾਹਰਲਾ, ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, libcrun_error_t *err);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
libcrun_free_container_listp (ਖਾਲੀ *p)
{
  libcrun_container_list_t **l = (libcrun_container_list_t **) p;
  ਜੇ (*l != ਨਿਰਾ)
    libcrun_free_containers_list (*l);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_container_ਸਥਿਤੀ __attribute__ ((cleanup (libcrun_free_container_ਸਥਿਤੀ)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_container_list __attribute__ ((cleanup (libcrun_free_container_listp)))

#ਜੇ_ਅੰਤ
