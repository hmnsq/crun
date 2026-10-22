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

#அந்தர்பாவ "container.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ <stdio.h>

கட்டமைப்பு custom_handler_s
{
  மாறிலி எழுத்து *name;
  மாறிலி எழுத்து *feature_string;

  மாறிலி எழுத்து *alias;

  மாறிலி bool supports_open_மரம்_namespace;

  முழு (*load) (வெறுமை **cookie, libcrun_error_t *err);
  முழு (*unload) (வெறுமை *cookie, libcrun_error_t *err);

  முழு (*run_func) (வெறுமை *cookie, libcrun_container_t *container,
                   மாறிலி எழுத்து *pathname, எழுத்து *மாறிலி argv[]);

  முழு (*exec_func) (வெறுமை *cookie, libcrun_container_t *container,
                    மாறிலி எழுத்து *pathname, எழுத்து *மாறிலி argv[]);

  முழு (*configure_container) (வெறுமை *cookie, எண்ணிக்கை handler_configure_phase phase,
                              libcrun_context_t *context, libcrun_container_t *container,
                              மாறிலி எழுத்து *rootfs, libcrun_error_t *err);

  முழு (*can_handle_container) (libcrun_container_t *container, libcrun_error_t *err);

  முழு (*modify_oci_configuration) (வெறுமை *cookie, libcrun_context_t *context,
                                   runtime_spec_schema_config_schema *def,
                                   libcrun_error_t *err);

  முழு (*close_fds) (வெறுமை *cookie, libcrun_container_t *container,
                    முழு preserve_fds, libcrun_error_t *err);
};

கட்டமைப்பு custom_handler_manager_s;

LIBCRUN_PUBLIC கட்டமைப்பு custom_handler_manager_s *libcrun_handler_manager_create (libcrun_error_t *err);
LIBCRUN_PUBLIC முழு libcrun_handler_manager_load_directory (கட்டமைப்பு custom_handler_manager_s *manager, மாறிலி எழுத்து *path, libcrun_error_t *err);
LIBCRUN_PUBLIC வெறுமை handler_manager_free (கட்டமைப்பு custom_handler_manager_s *manager);

LIBCRUN_PUBLIC கட்டமைப்பு custom_handler_s *handler_by_name (கட்டமைப்பு custom_handler_manager_s *manager, மாறிலி எழுத்து *name);
LIBCRUN_PUBLIC வெறுமை libcrun_handler_manager_print_feature_tags (கட்டமைப்பு custom_handler_manager_s *manager, FILE *out);
LIBCRUN_PUBLIC எழுத்து **libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err);

கட்டமைப்பு custom_handler_instance_s
{
  கட்டமைப்பு custom_handler_s *vtable;
  வெறுமை *cookie;
};

LIBCRUN_PUBLIC முழு libcrun_configure_handler (கட்டமைப்பு custom_handler_manager_s *manager,
                                              libcrun_context_t *context,
                                              libcrun_container_t *container,
                                              கட்டமைப்பு custom_handler_instance_s **out,
                                              libcrun_error_t *err);

வகைவரை கட்டமைப்பு custom_handler_s *(*run_oci_get_handler_cb) ();

#define cleanup_custom_handler_instance __attribute__ ((cleanup (cleanup_custom_handler_instancep)))

நிலையான உள்வரி வெறுமை
cleanup_custom_handler_instancep (கட்டமைப்பு custom_handler_instance_s **p)
{
  கட்டமைப்பு custom_handler_instance_s *handler = (கட்டமைப்பு custom_handler_instance_s *) *p;
  எனில் (handler)
    {
      எனில் (handler->vtable)
        {
          libcrun_error_t tmp_err = வெற்று;
          முழு tmp_ret;

          tmp_ret = handler->vtable->unload (handler->cookie, &tmp_err);
          எனில் (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
        }
      விடுவி (handler);
    }
}

#endif
