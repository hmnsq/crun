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

#अगर_अघोषित CUSTOM_HANDLER_H
#घोषणा CUSTOM_HANDLER_H

#समावेश "container.h"
#समावेश "utils.h"
#समावेश <stdio.h>

काष्ठा custom_handler_s
{
  स्थिर अक्षर *name;
  स्थिर अक्षर *feature_string;

  स्थिर अक्षर *alias;

  स्थिर bool supports_open_वृक्ष_namespace;

  पूर्णांक (*load) (व्योम **cookie, libcrun_error_t *err);
  पूर्णांक (*unload) (व्योम *cookie, libcrun_error_t *err);

  पूर्णांक (*run_func) (व्योम *cookie, libcrun_container_t *container,
                   स्थिर अक्षर *pathname, अक्षर *स्थिर argv[]);

  पूर्णांक (*exec_func) (व्योम *cookie, libcrun_container_t *container,
                    स्थिर अक्षर *pathname, अक्षर *स्थिर argv[]);

  पूर्णांक (*configure_container) (व्योम *cookie, क्रमागत handler_configure_phase phase,
                              libcrun_context_t *context, libcrun_container_t *container,
                              स्थिर अक्षर *rootfs, libcrun_error_t *err);

  पूर्णांक (*can_handle_container) (libcrun_container_t *container, libcrun_error_t *err);

  पूर्णांक (*modify_oci_configuration) (व्योम *cookie, libcrun_context_t *context,
                                   runtime_spec_schema_config_schema *def,
                                   libcrun_error_t *err);

  पूर्णांक (*close_fds) (व्योम *cookie, libcrun_container_t *container,
                    पूर्णांक preserve_fds, libcrun_error_t *err);
};

काष्ठा custom_handler_manager_s;

LIBCRUN_PUBLIC काष्ठा custom_handler_manager_s *libcrun_handler_manager_create (libcrun_error_t *err);
LIBCRUN_PUBLIC पूर्णांक libcrun_handler_manager_load_directory (काष्ठा custom_handler_manager_s *manager, स्थिर अक्षर *path, libcrun_error_t *err);
LIBCRUN_PUBLIC व्योम handler_manager_free (काष्ठा custom_handler_manager_s *manager);

LIBCRUN_PUBLIC काष्ठा custom_handler_s *handler_by_name (काष्ठा custom_handler_manager_s *manager, स्थिर अक्षर *name);
LIBCRUN_PUBLIC व्योम libcrun_handler_manager_print_feature_tags (काष्ठा custom_handler_manager_s *manager, खाता *out);
LIBCRUN_PUBLIC अक्षर **libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err);

काष्ठा custom_handler_instance_s
{
  काष्ठा custom_handler_s *vtable;
  व्योम *cookie;
};

LIBCRUN_PUBLIC पूर्णांक libcrun_configure_handler (काष्ठा custom_handler_manager_s *manager,
                                              libcrun_context_t *context,
                                              libcrun_container_t *container,
                                              काष्ठा custom_handler_instance_s **out,
                                              libcrun_error_t *err);

प्रकार काष्ठा custom_handler_s *(*run_oci_get_handler_cb) ();

#घोषणा cleanup_custom_handler_instance __attribute__ ((cleanup (cleanup_custom_handler_instancep)))

अटल अंतरभूत व्योम
cleanup_custom_handler_instancep (काष्ठा custom_handler_instance_s **p)
{
  काष्ठा custom_handler_instance_s *handler = (काष्ठा custom_handler_instance_s *) *p;
  अगर (handler)
    {
      अगर (handler->vtable)
        {
          libcrun_error_t tmp_err = शून्य;
          पूर्णांक tmp_ret;

          tmp_ret = handler->vtable->unload (handler->cookie, &tmp_err);
          अगर (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
        }
      मुक्त (handler);
    }
}

#पूर्ण_अगर
