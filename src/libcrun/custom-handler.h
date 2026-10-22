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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CUSTOM_HANDLER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CUSTOM_HANDLER_H

#ਅਨ੍ਤਰ੍ਭਾਵ "container.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>

ਬਣਤਰ custom_handler_s
{
  ਅਚਲ ਅੱਖਰ *name;
  ਅਚਲ ਅੱਖਰ *feature_string;

  ਅਚਲ ਅੱਖਰ *alias;

  ਅਚਲ bool supports_open_ਰੁੱਖ_namespace;

  ਪੂਰਨ (*ਲੱਦੋ) (ਖਾਲੀ **cookie, libcrun_error_t *err);
  ਪੂਰਨ (*unload) (ਖਾਲੀ *cookie, libcrun_error_t *err);

  ਪੂਰਨ (*ਲੜੀਵਾਰ_func) (ਖਾਲੀ *cookie, libcrun_container_t *container,
                   ਅਚਲ ਅੱਖਰ *pathname, ਅੱਖਰ *ਅਚਲ argv[]);

  ਪੂਰਨ (*exec_func) (ਖਾਲੀ *cookie, libcrun_container_t *container,
                    ਅਚਲ ਅੱਖਰ *pathname, ਅੱਖਰ *ਅਚਲ argv[]);

  ਪੂਰਨ (*configure_container) (ਖਾਲੀ *cookie, ਗਿਣਤੀ handler_configure_phase phase,
                              libcrun_context_t *context, libcrun_container_t *container,
                              ਅਚਲ ਅੱਖਰ *rootfs, libcrun_error_t *err);

  ਪੂਰਨ (*can_handle_container) (libcrun_container_t *container, libcrun_error_t *err);

  ਪੂਰਨ (*modify_oci_configuration) (ਖਾਲੀ *cookie, libcrun_context_t *context,
                                   runtime_spec_schema_config_schema *def,
                                   libcrun_error_t *err);

  ਪੂਰਨ (*close_fds) (ਖਾਲੀ *cookie, libcrun_container_t *container,
                    ਪੂਰਨ preserve_fds, libcrun_error_t *err);
};

ਬਣਤਰ custom_handler_manager_s;

LIBCRUN_ਜਨਤਕ ਬਣਤਰ custom_handler_manager_s *libcrun_handler_manager_create (libcrun_error_t *err);
LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_handler_manager_ਲੱਦੋ_directory (ਬਣਤਰ custom_handler_manager_s *manager, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err);
LIBCRUN_ਜਨਤਕ ਖਾਲੀ handler_manager_free (ਬਣਤਰ custom_handler_manager_s *manager);

LIBCRUN_ਜਨਤਕ ਬਣਤਰ custom_handler_s *handler_by_name (ਬਣਤਰ custom_handler_manager_s *manager, ਅਚਲ ਅੱਖਰ *name);
LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_handler_manager_print_feature_ਟੈਗ_ਸੂਚੀ (ਬਣਤਰ custom_handler_manager_s *manager, FILE *ਬਾਹਰਲਾ);
LIBCRUN_ਜਨਤਕ ਅੱਖਰ **libcrun_context_get_handler_feature_ਟੈਗ_ਸੂਚੀ (libcrun_context_t *ctx, libcrun_error_t *err);

ਬਣਤਰ custom_handler_instance_s
{
  ਬਣਤਰ custom_handler_s *vtable;
  ਖਾਲੀ *cookie;
};

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_configure_handler (ਬਣਤਰ custom_handler_manager_s *manager,
                                              libcrun_context_t *context,
                                              libcrun_container_t *container,
                                              ਬਣਤਰ custom_handler_instance_s **ਬਾਹਰਲਾ,
                                              libcrun_error_t *err);

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ custom_handler_s *(*ਲੜੀਵਾਰ_oci_get_handler_cb) ();

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_custom_handler_instance __attribute__ ((cleanup (cleanup_custom_handler_instancep)))

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cleanup_custom_handler_instancep (ਬਣਤਰ custom_handler_instance_s **p)
{
  ਬਣਤਰ custom_handler_instance_s *handler = (ਬਣਤਰ custom_handler_instance_s *) *p;
  ਜੇ (handler)
    {
      ਜੇ (handler->vtable)
        {
          libcrun_error_t tmp_err = ਨਿਰਾ;
          ਪੂਰਨ tmp_ret;

          tmp_ret = handler->vtable->unload (handler->cookie, &tmp_err);
          ਜੇ (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
        }
      ਮੁਕਤ (handler);
    }
}

#ਜੇ_ਅੰਤ
