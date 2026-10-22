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

#شامل "container.h"
#شامل "utils.h"
#شامل <stdio.h>

ساختار custom_handler_s
{
  ثابت نویسه *name;
  ثابت نویسه *feature_string;

  ثابت نویسه *alias;

  ثابت bool supports_open_درخت_namespace;

  صحیح (*load) (تهی **cookie, libcrun_error_t *err);
  صحیح (*unload) (تهی *cookie, libcrun_error_t *err);

  صحیح (*run_func) (تهی *cookie, libcrun_container_t *container,
                   ثابت نویسه *pathname, نویسه *ثابت argv[]);

  صحیح (*exec_func) (تهی *cookie, libcrun_container_t *container,
                    ثابت نویسه *pathname, نویسه *ثابت argv[]);

  صحیح (*configure_container) (تهی *cookie, شمارش handler_configure_phase phase,
                              libcrun_context_t *context, libcrun_container_t *container,
                              ثابت نویسه *rootfs, libcrun_error_t *err);

  صحیح (*can_handle_container) (libcrun_container_t *container, libcrun_error_t *err);

  صحیح (*modify_oci_configuration) (تهی *cookie, libcrun_context_t *context,
                                   runtime_spec_schema_config_schema *def,
                                   libcrun_error_t *err);

  صحیح (*close_fds) (تهی *cookie, libcrun_container_t *container,
                    صحیح preserve_fds, libcrun_error_t *err);
};

ساختار custom_handler_manager_s;

LIBCRUN_PUBLIC ساختار custom_handler_manager_s *libcrun_handler_manager_create (libcrun_error_t *err);
LIBCRUN_PUBLIC صحیح libcrun_handler_manager_load_directory (ساختار custom_handler_manager_s *manager, ثابت نویسه *path, libcrun_error_t *err);
LIBCRUN_PUBLIC تهی handler_manager_free (ساختار custom_handler_manager_s *manager);

LIBCRUN_PUBLIC ساختار custom_handler_s *handler_by_name (ساختار custom_handler_manager_s *manager, ثابت نویسه *name);
LIBCRUN_PUBLIC تهی libcrun_handler_manager_print_feature_tags (ساختار custom_handler_manager_s *manager, FILE *out);
LIBCRUN_PUBLIC نویسه **libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err);

ساختار custom_handler_instance_s
{
  ساختار custom_handler_s *vtable;
  تهی *cookie;
};

LIBCRUN_PUBLIC صحیح libcrun_configure_handler (ساختار custom_handler_manager_s *manager,
                                              libcrun_context_t *context,
                                              libcrun_container_t *container,
                                              ساختار custom_handler_instance_s **out,
                                              libcrun_error_t *err);

تعریفنوع ساختار custom_handler_s *(*run_oci_get_handler_cb) ();

#define cleanup_custom_handler_instance __attribute__ ((cleanup (cleanup_custom_handler_instancep)))

ایستا درونخطی تهی
cleanup_custom_handler_instancep (ساختار custom_handler_instance_s **p)
{
  ساختار custom_handler_instance_s *handler = (ساختار custom_handler_instance_s *) *p;
  اگر (handler)
    {
      اگر (handler->vtable)
        {
          libcrun_error_t tmp_err = پوچ;
          صحیح tmp_ret;

          tmp_ret = handler->vtable->unload (handler->cookie, &tmp_err);
          اگر (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
        }
      آزاد (handler);
    }
}

#endif
