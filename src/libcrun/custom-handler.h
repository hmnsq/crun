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

#جيڪڏهن_وصف_نه_ٿيل CUSTOM_HANDLER_H
#وصف_ڪريو CUSTOM_HANDLER_H

#ضمن "container.h"
#ضمن "utils.h"
#ضمن <stdio.h>

بنية custom_handler_s
{
  ثابت حرف *name;
  ثابت حرف *feature_string;

  ثابت حرف *alias;

  ثابت bool supports_open_وڻ_namespace;

  صحيح (*ڀريو) (فراغ **cookie, libcrun_error_t *err);
  صحيح (*unload) (فراغ *cookie, libcrun_error_t *err);

  صحيح (*لاڳيتو_func) (فراغ *cookie, libcrun_container_t *container,
                   ثابت حرف *pathname, حرف *ثابت argv[]);

  صحيح (*exec_func) (فراغ *cookie, libcrun_container_t *container,
                    ثابت حرف *pathname, حرف *ثابت argv[]);

  صحيح (*configure_container) (فراغ *cookie, تعداد handler_configure_phase phase,
                              libcrun_context_t *context, libcrun_container_t *container,
                              ثابت حرف *rootfs, libcrun_error_t *err);

  صحيح (*can_handle_container) (libcrun_container_t *container, libcrun_error_t *err);

  صحيح (*modify_oci_configuration) (فراغ *cookie, libcrun_context_t *context,
                                   runtime_spec_schema_config_schema *def,
                                   libcrun_error_t *err);

  صحيح (*close_fds) (فراغ *cookie, libcrun_container_t *container,
                    صحيح preserve_fds, libcrun_error_t *err);
};

بنية custom_handler_manager_s;

LIBCRUN_عوامي بنية custom_handler_manager_s *libcrun_handler_manager_create (libcrun_error_t *err);
LIBCRUN_عوامي صحيح libcrun_handler_manager_ڀريو_directory (بنية custom_handler_manager_s *manager, ثابت حرف *رستو, libcrun_error_t *err);
LIBCRUN_عوامي فراغ handler_manager_free (بنية custom_handler_manager_s *manager);

LIBCRUN_عوامي بنية custom_handler_s *handler_by_name (بنية custom_handler_manager_s *manager, ثابت حرف *name);
LIBCRUN_عوامي فراغ libcrun_handler_manager_print_feature_ٽيگ_فهرست (بنية custom_handler_manager_s *manager, FILE *ٻاهريون);
LIBCRUN_عوامي حرف **libcrun_context_get_handler_feature_ٽيگ_فهرست (libcrun_context_t *ctx, libcrun_error_t *err);

بنية custom_handler_instance_s
{
  بنية custom_handler_s *vtable;
  فراغ *cookie;
};

LIBCRUN_عوامي صحيح libcrun_configure_handler (بنية custom_handler_manager_s *manager,
                                              libcrun_context_t *context,
                                              libcrun_container_t *container,
                                              بنية custom_handler_instance_s **ٻاهريون,
                                              libcrun_error_t *err);

تعريفنوع بنية custom_handler_s *(*لاڳيتو_oci_get_handler_cb) ();

#وصف_ڪريو cleanup_custom_handler_instance __attribute__ ((cleanup (cleanup_custom_handler_instancep)))

ساكن مضمن فراغ
cleanup_custom_handler_instancep (بنية custom_handler_instance_s **p)
{
  بنية custom_handler_instance_s *handler = (بنية custom_handler_instance_s *) *p;
  إذا (handler)
    {
      إذا (handler->vtable)
        {
          libcrun_error_t tmp_err = عدم;
          صحيح tmp_ret;

          tmp_ret = handler->vtable->unload (handler->cookie, &tmp_err);
          إذا (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
        }
      حرر (handler);
    }
}

#جيڪڏهن_پورو
