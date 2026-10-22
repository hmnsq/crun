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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CUSTOM_HANDLER_H
#ವ್ಯಾಖ್ಯಾನಿಸು CUSTOM_HANDLER_H

#ಅನ್ತರ್ಭಾವ "container.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>

ರಚನೆ custom_handler_s
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *feature_string;

  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *alias;

  ಸ್ಥಿರಾಂಕ bool supports_open_ವೃಕ್ಷ_namespace;

  ಪೂರ್ಣಾಂಕ (*ತುಂಬು) (ಶೂನ್ಯ **cookie, libcrun_error_t *err);
  ಪೂರ್ಣಾಂಕ (*unload) (ಶೂನ್ಯ *cookie, libcrun_error_t *err);

  ಪೂರ್ಣಾಂಕ (*ಸತತ_func) (ಶೂನ್ಯ *cookie, libcrun_container_t *container,
                   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathname, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ argv[]);

  ಪೂರ್ಣಾಂಕ (*exec_func) (ಶೂನ್ಯ *cookie, libcrun_container_t *container,
                    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pathname, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ argv[]);

  ಪೂರ್ಣಾಂಕ (*configure_container) (ಶೂನ್ಯ *cookie, ಎಣಿಕೆ handler_configure_phase phase,
                              libcrun_context_t *context, libcrun_container_t *container,
                              ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs, libcrun_error_t *err);

  ಪೂರ್ಣಾಂಕ (*can_handle_container) (libcrun_container_t *container, libcrun_error_t *err);

  ಪೂರ್ಣಾಂಕ (*modify_oci_configuration) (ಶೂನ್ಯ *cookie, libcrun_context_t *context,
                                   runtime_spec_schema_config_schema *def,
                                   libcrun_error_t *err);

  ಪೂರ್ಣಾಂಕ (*close_fds) (ಶೂನ್ಯ *cookie, libcrun_container_t *container,
                    ಪೂರ್ಣಾಂಕ preserve_fds, libcrun_error_t *err);
};

ರಚನೆ custom_handler_manager_s;

LIBCRUN_ಬಹಿರಂಗ ರಚನೆ custom_handler_manager_s *libcrun_handler_manager_create (libcrun_error_t *err);
LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_handler_manager_ತುಂಬು_directory (ರಚನೆ custom_handler_manager_s *manager, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err);
LIBCRUN_ಬಹಿರಂಗ ಶೂನ್ಯ handler_manager_free (ರಚನೆ custom_handler_manager_s *manager);

LIBCRUN_ಬಹಿರಂಗ ರಚನೆ custom_handler_s *handler_by_name (ರಚನೆ custom_handler_manager_s *manager, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name);
LIBCRUN_ಬಹಿರಂಗ ಶೂನ್ಯ libcrun_handler_manager_print_feature_ಟ್ಯಾಗುಗಳು (ರಚನೆ custom_handler_manager_s *manager, FILE *ಹೊರಹೋಗು);
LIBCRUN_ಬಹಿರಂಗ ಅಕ್ಷರ **libcrun_context_get_handler_feature_ಟ್ಯಾಗುಗಳು (libcrun_context_t *ctx, libcrun_error_t *err);

ರಚನೆ custom_handler_instance_s
{
  ರಚನೆ custom_handler_s *vtable;
  ಶೂನ್ಯ *cookie;
};

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_configure_handler (ರಚನೆ custom_handler_manager_s *manager,
                                              libcrun_context_t *context,
                                              libcrun_container_t *container,
                                              ರಚನೆ custom_handler_instance_s **ಹೊರಹೋಗು,
                                              libcrun_error_t *err);

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ custom_handler_s *(*ಸತತ_oci_get_handler_cb) ();

#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_custom_handler_instance __attribute__ ((cleanup (cleanup_custom_handler_instancep)))

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cleanup_custom_handler_instancep (ರಚನೆ custom_handler_instance_s **p)
{
  ರಚನೆ custom_handler_instance_s *handler = (ರಚನೆ custom_handler_instance_s *) *p;
  ಆದರೆ (handler)
    {
      ಆದರೆ (handler->vtable)
        {
          libcrun_error_t tmp_err = ಖಾಲಿ;
          ಪೂರ್ಣಾಂಕ tmp_ret;

          tmp_ret = handler->vtable->unload (handler->cookie, &tmp_err);
          ಆದರೆ (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
        }
      ಬಿಡುಗಡೆ (handler);
    }
}

#ಆದರೆ_ಅಂತ್ಯ
