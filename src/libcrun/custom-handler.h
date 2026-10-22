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

#ifndef CUSTOM_HANDLER_H
#define CUSTOM_HANDLER_H

#ಅನ್ತರ್ಭಾವ "container.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>

ಸಂರಚನಾ custom_handler_s
{
  ಧ್ರುವ ವರ್ಣ *name;
  ಧ್ರುವ ವರ್ಣ *feature_string;

  ಧ್ರುವ ವರ್ಣ *alias;

  ಧ್ರುವ bool supports_open_ವೃಕ್ಷ_namespace;

  ಪೂರ್ಣಾಙ್ಕ (*load) (ಶೂನ್ಯ **cookie, libcrun_error_t *err);
  ಪೂರ್ಣಾಙ್ಕ (*unload) (ಶೂನ್ಯ *cookie, libcrun_error_t *err);

  ಪೂರ್ಣಾಙ್ಕ (*run_func) (ಶೂನ್ಯ *cookie, libcrun_container_t *container,
                   ಧ್ರುವ ವರ್ಣ *pathname, ವರ್ಣ *ಧ್ರುವ argv[]);

  ಪೂರ್ಣಾಙ್ಕ (*exec_func) (ಶೂನ್ಯ *cookie, libcrun_container_t *container,
                    ಧ್ರುವ ವರ್ಣ *pathname, ವರ್ಣ *ಧ್ರುವ argv[]);

  ಪೂರ್ಣಾಙ್ಕ (*configure_container) (ಶೂನ್ಯ *cookie, ಗಣನಾ handler_configure_phase phase,
                              libcrun_context_t *context, libcrun_container_t *container,
                              ಧ್ರುವ ವರ್ಣ *rootfs, libcrun_error_t *err);

  ಪೂರ್ಣಾಙ್ಕ (*can_handle_container) (libcrun_container_t *container, libcrun_error_t *err);

  ಪೂರ್ಣಾಙ್ಕ (*modify_oci_configuration) (ಶೂನ್ಯ *cookie, libcrun_context_t *context,
                                   runtime_spec_schema_config_schema *def,
                                   libcrun_error_t *err);

  ಪೂರ್ಣಾಙ್ಕ (*close_fds) (ಶೂನ್ಯ *cookie, libcrun_container_t *container,
                    ಪೂರ್ಣಾಙ್ಕ preserve_fds, libcrun_error_t *err);
};

ಸಂರಚನಾ custom_handler_manager_s;

LIBCRUN_PUBLIC ಸಂರಚನಾ custom_handler_manager_s *libcrun_handler_manager_create (libcrun_error_t *err);
LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_handler_manager_load_directory (ಸಂರಚನಾ custom_handler_manager_s *manager, ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err);
LIBCRUN_PUBLIC ಶೂನ್ಯ handler_manager_free (ಸಂರಚನಾ custom_handler_manager_s *manager);

LIBCRUN_PUBLIC ಸಂರಚನಾ custom_handler_s *handler_by_name (ಸಂರಚನಾ custom_handler_manager_s *manager, ಧ್ರುವ ವರ್ಣ *name);
LIBCRUN_PUBLIC ಶೂನ್ಯ libcrun_handler_manager_print_feature_tags (ಸಂರಚನಾ custom_handler_manager_s *manager, FILE *out);
LIBCRUN_PUBLIC ವರ್ಣ **libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err);

ಸಂರಚನಾ custom_handler_instance_s
{
  ಸಂರಚನಾ custom_handler_s *vtable;
  ಶೂನ್ಯ *cookie;
};

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_configure_handler (ಸಂರಚನಾ custom_handler_manager_s *manager,
                                              libcrun_context_t *context,
                                              libcrun_container_t *container,
                                              ಸಂರಚನಾ custom_handler_instance_s **out,
                                              libcrun_error_t *err);

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ custom_handler_s *(*run_oci_get_handler_cb) ();

#define cleanup_custom_handler_instance __attribute__ ((cleanup (cleanup_custom_handler_instancep)))

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
cleanup_custom_handler_instancep (ಸಂರಚನಾ custom_handler_instance_s **p)
{
  ಸಂರಚನಾ custom_handler_instance_s *handler = (ಸಂರಚನಾ custom_handler_instance_s *) *p;
  ಯದಿ (handler)
    {
      ಯದಿ (handler->vtable)
        {
          libcrun_error_t tmp_err = ರಿಕ್ತ;
          ಪೂರ್ಣಾಙ್ಕ tmp_ret;

          tmp_ret = handler->vtable->unload (handler->cookie, &tmp_err);
          ಯದಿ (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
        }
      ಮುಕ್ತ (handler);
    }
}

#endif
