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

#अन्तर्भाव "container.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव <stdio.h>

संरचना custom_handler_s
{
  अचर वर्ण *name;
  अचर वर्ण *feature_string;

  अचर वर्ण *alias;

  अचर bool supports_open_रूख_namespace;

  पूर्णाङ्क (*load) (शून्य **cookie, libcrun_error_t *err);
  पूर्णाङ्क (*unload) (शून्य *cookie, libcrun_error_t *err);

  पूर्णाङ्क (*run_func) (शून्य *cookie, libcrun_container_t *container,
                   अचर वर्ण *pathname, वर्ण *अचर argv[]);

  पूर्णाङ्क (*exec_func) (शून्य *cookie, libcrun_container_t *container,
                    अचर वर्ण *pathname, वर्ण *अचर argv[]);

  पूर्णाङ्क (*configure_container) (शून्य *cookie, गणना handler_configure_phase phase,
                              libcrun_context_t *context, libcrun_container_t *container,
                              अचर वर्ण *rootfs, libcrun_error_t *err);

  पूर्णाङ्क (*can_handle_container) (libcrun_container_t *container, libcrun_error_t *err);

  पूर्णाङ्क (*modify_oci_configuration) (शून्य *cookie, libcrun_context_t *context,
                                   runtime_spec_schema_config_schema *def,
                                   libcrun_error_t *err);

  पूर्णाङ्क (*close_fds) (शून्य *cookie, libcrun_container_t *container,
                    पूर्णाङ्क preserve_fds, libcrun_error_t *err);
};

संरचना custom_handler_manager_s;

LIBCRUN_PUBLIC संरचना custom_handler_manager_s *libcrun_handler_manager_create (libcrun_error_t *err);
LIBCRUN_PUBLIC पूर्णाङ्क libcrun_handler_manager_load_directory (संरचना custom_handler_manager_s *manager, अचर वर्ण *path, libcrun_error_t *err);
LIBCRUN_PUBLIC शून्य handler_manager_free (संरचना custom_handler_manager_s *manager);

LIBCRUN_PUBLIC संरचना custom_handler_s *handler_by_name (संरचना custom_handler_manager_s *manager, अचर वर्ण *name);
LIBCRUN_PUBLIC शून्य libcrun_handler_manager_print_feature_tags (संरचना custom_handler_manager_s *manager, खाता *out);
LIBCRUN_PUBLIC वर्ण **libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err);

संरचना custom_handler_instance_s
{
  संरचना custom_handler_s *vtable;
  शून्य *cookie;
};

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_configure_handler (संरचना custom_handler_manager_s *manager,
                                              libcrun_context_t *context,
                                              libcrun_container_t *container,
                                              संरचना custom_handler_instance_s **out,
                                              libcrun_error_t *err);

प्रकारनिर्देश संरचना custom_handler_s *(*run_oci_get_handler_cb) ();

#define cleanup_custom_handler_instance __attribute__ ((cleanup (cleanup_custom_handler_instancep)))

स्थिर अन्तःस्थ शून्य
cleanup_custom_handler_instancep (संरचना custom_handler_instance_s **p)
{
  संरचना custom_handler_instance_s *handler = (संरचना custom_handler_instance_s *) *p;
  यदि (handler)
    {
      यदि (handler->vtable)
        {
          libcrun_error_t tmp_err = रिक्त;
          पूर्णाङ्क tmp_ret;

          tmp_ret = handler->vtable->unload (handler->cookie, &tmp_err);
          यदि (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
        }
      मुक्त (handler);
    }
}

#endif
