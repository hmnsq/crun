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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಸ್ಥಿತಿ_H
#ವ್ಯಾಖ್ಯಾನಿಸು ಸ್ಥಿತಿ_H

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <ocispec/runtime_spec_schema_config_schema.h>
#ಅನ್ತರ್ಭಾವ "error.h"
#ಅನ್ತರ್ಭಾವ "container.h"

ರಚನೆ libcrun_container_list_s
{
  ರಚನೆ libcrun_container_list_s *ಮುಂದಿನ;
  ಅಕ್ಷರ *name;
};
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ libcrun_container_list_s libcrun_container_list_t;

ರಚನೆ libcrun_container_ಸ್ಥಿತಿ_s
{
  pid_t pid;
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ process_ಆರಂಭ_ಸಮಯ;
  ಅಕ್ಷರ *bundle;
  ಅಕ್ಷರ *rootfs;
  ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ;
  ಅಕ್ಷರ *scope;
  ಪೂರ್ಣಾಂಕ systemd_cgroup;
  ಅಕ್ಷರ *created;
  ಪೂರ್ಣಾಂಕ detached;
  ಪೂರ್ಣಾಂಕ no_new_keyring;
  ಅಕ್ಷರ *external_descriptors;
  ಅಕ್ಷರ *owner;
};
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ libcrun_container_ಸ್ಥಿತಿ_s libcrun_container_ಸ್ಥಿತಿ_t;

LIBCRUN_ಬಹಿರಂಗ ಶೂನ್ಯ libcrun_free_container_ಸ್ಥಿತಿ (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2);
LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_write_container_ಸ್ಥಿತಿ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id,
                                                   libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, libcrun_error_t *err);
LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_read_container_ಸ್ಥಿತಿ (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು,
                                                  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err);
LIBCRUN_ಬಹಿರಂಗ ಶೂನ್ಯ libcrun_free_containers_list (libcrun_container_list_t *list);
LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_is_container_running (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, libcrun_error_t *err);
LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_get_state_directory (ಅಕ್ಷರ **ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err);
LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_delete_ಸ್ಥಿತಿ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err);
LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_get_containers_list (libcrun_container_list_t **ret, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು,
                                                libcrun_error_t *err);

ರಚನೆ libcrun_container_iter_s
{
  libcrun_container_list_t *cur;
};
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ libcrun_container_iter_s libcrun_container_iter_t;

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **ಹೊರಹೋಗು,
                                           libcrun_error_t *err);
LIBCRUN_ಬಹಿರಂಗ libcrun_container_iter_t *libcrun_container_list_iter (libcrun_container_list_t *l);
LIBCRUN_ಬಹಿರಂಗ bool libcrun_container_iter_ಮುಂದಿನ (libcrun_container_iter_t *it, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **id);
LIBCRUN_ಬಹಿರಂಗ ಶೂನ್ಯ libcrun_container_iter_free (libcrun_container_iter_t *it);
LIBCRUN_ಬಹಿರಂಗ ಶೂನ್ಯ libcrun_container_list_free (libcrun_container_list_t *l);

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ
{
  LIBCRUN_CONTAINER_ಸ್ಥಿತಿ_CREATING = 0,
  LIBCRUN_CONTAINER_ಸ್ಥಿತಿ_CREATED,
  LIBCRUN_CONTAINER_ಸ್ಥಿತಿ_RUNNING,
  LIBCRUN_CONTAINER_ಸ್ಥಿತಿ_STOPPED,
  LIBCRUN_CONTAINER_ಸ್ಥಿತಿ_PAUSED,
} libcrun_container_state_t;

ರಚನೆ libcrun_ಸ್ಥಿತಿ_s;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ libcrun_ಸ್ಥಿತಿ_s libcrun_ಸ್ಥಿತಿ_t;

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_ಸ್ಥಿತಿ_ತುಂಬು (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_ಸ್ಥಿತಿ_t **ಹೊರಹೋಗು,
                                                  libcrun_error_t *err);
LIBCRUN_ಬಹಿರಂಗ ಶೂನ್ಯ libcrun_container_ಸ್ಥಿತಿ_free (libcrun_ಸ್ಥಿತಿ_t *st);
/* Resolve the live state of the container described by ST.  The state is
   probed on the first call and cached afterwards; STATE and RUNNING can be
   NULL.  */
LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_ಸ್ಥಿತಿ_get_state (libcrun_ಸ್ಥಿತಿ_t *st, libcrun_container_state_t *state, ಪೂರ್ಣಾಂಕ *running,
                                             libcrun_error_t *err);
LIBCRUN_ಬಹಿರಂಗ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libcrun_container_state_ಗಮ್ಯ_string (libcrun_container_state_t state);
LIBCRUN_ಬಹಿರಂಗ pid_t libcrun_ಸ್ಥಿತಿ_get_pid (libcrun_ಸ್ಥಿತಿ_t *st);
LIBCRUN_ಬಹಿರಂಗ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libcrun_ಸ್ಥಿತಿ_get_bundle (libcrun_ಸ್ಥಿತಿ_t *st);
LIBCRUN_ಬಹಿರಂಗ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libcrun_ಸ್ಥಿತಿ_get_rootfs (libcrun_ಸ್ಥಿತಿ_t *st);
LIBCRUN_ಬಹಿರಂಗ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libcrun_ಸ್ಥಿತಿ_get_created (libcrun_ಸ್ಥಿತಿ_t *st);
LIBCRUN_ಬಹಿರಂಗ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libcrun_ಸ್ಥಿತಿ_get_owner (libcrun_ಸ್ಥಿತಿ_t *st);
LIBCRUN_ಬಹಿರಂಗ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libcrun_ಸ್ಥಿತಿ_get_scope (libcrun_ಸ್ಥಿತಿ_t *st);
LIBCRUN_ಬಹಿರಂಗ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libcrun_ಸ್ಥಿತಿ_get_external_descriptors (libcrun_ಸ್ಥಿತಿ_t *st);
LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_get_state_string (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಹೊರಹೋಗು,
                                                       ಪೂರ್ಣಾಂಕ *running, libcrun_error_t *err);
LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_get_state_dir (ಅಕ್ಷರ **ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_ಸ್ಥಿತಿ_check_directories (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_ಸ್ಥಿತಿ_create_exec_fifo (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_ಸ್ಥಿತಿ_write_exec_fifo (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_ಸ್ಥಿತಿ_has_read_exec_fifo (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_check_pid_valid (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ get_ಸತತ_directory (ಅಕ್ಷರ **ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ get_shared_ಖಾಲಿಗಳು_directory_ಮಾರ್ಗ (ಅಕ್ಷರ **ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, libcrun_error_t *err);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
libcrun_free_container_listp (ಶೂನ್ಯ *p)
{
  libcrun_container_list_t **l = (libcrun_container_list_t **) p;
  ಆದರೆ (*l != ಖಾಲಿ)
    libcrun_free_containers_list (*l);
}

#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_container_ಸ್ಥಿತಿ __attribute__ ((cleanup (libcrun_free_container_ಸ್ಥಿತಿ)))
#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_container_list __attribute__ ((cleanup (libcrun_free_container_listp)))

#ಆದರೆ_ಅಂತ್ಯ
