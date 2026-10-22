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

#include "container.h"
#include "utils.h"
#include <stdio.h>

sa_nracanaa custom_handler_s
{
  const var_na *name;
  const var_na *feature_string;

  const var_na *alias;

  const bool supports_open_tree_namespace;

  puur_naa_nka (*load) (void **cookie, libcrun_error_t *err);
  puur_naa_nka (*unload) (void *cookie, libcrun_error_t *err);

  puur_naa_nka (*run_func) (void *cookie, libcrun_container_t *container,
                   const var_na *pathname, var_na *const argv[]);

  puur_naa_nka (*exec_func) (void *cookie, libcrun_container_t *container,
                    const var_na *pathname, var_na *const argv[]);

  puur_naa_nka (*configure_container) (void *cookie, ga_nanaa handler_configure_phase phase,
                              libcrun_context_t *context, libcrun_container_t *container,
                              const var_na *rootfs, libcrun_error_t *err);

  puur_naa_nka (*can_handle_container) (libcrun_container_t *container, libcrun_error_t *err);

  puur_naa_nka (*modify_oci_configuration) (void *cookie, libcrun_context_t *context,
                                   runtime_spec_schema_config_schema *def,
                                   libcrun_error_t *err);

  puur_naa_nka (*close_fds) (void *cookie, libcrun_container_t *container,
                    puur_naa_nka preserve_fds, libcrun_error_t *err);
};

sa_nracanaa custom_handler_manager_s;

LIBCRUN_PUBLIC sa_nracanaa custom_handler_manager_s *libcrun_handler_manager_create (libcrun_error_t *err);
LIBCRUN_PUBLIC puur_naa_nka libcrun_handler_manager_load_directory (sa_nracanaa custom_handler_manager_s *manager, const var_na *path, libcrun_error_t *err);
LIBCRUN_PUBLIC void handler_manager_free (sa_nracanaa custom_handler_manager_s *manager);

LIBCRUN_PUBLIC sa_nracanaa custom_handler_s *handler_by_name (sa_nracanaa custom_handler_manager_s *manager, const var_na *name);
LIBCRUN_PUBLIC void libcrun_handler_manager_print_feature_tags (sa_nracanaa custom_handler_manager_s *manager, FILE *out);
LIBCRUN_PUBLIC var_na **libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err);

sa_nracanaa custom_handler_instance_s
{
  sa_nracanaa custom_handler_s *vtable;
  void *cookie;
};

LIBCRUN_PUBLIC puur_naa_nka libcrun_configure_handler (sa_nracanaa custom_handler_manager_s *manager,
                                              libcrun_context_t *context,
                                              libcrun_container_t *container,
                                              sa_nracanaa custom_handler_instance_s **out,
                                              libcrun_error_t *err);

typedef sa_nracanaa custom_handler_s *(*run_oci_get_handler_cb) ();

#define cleanup_custom_handler_instance __attribute__ ((cleanup (cleanup_custom_handler_instancep)))

static anta_hstha void
cleanup_custom_handler_instancep (sa_nracanaa custom_handler_instance_s **p)
{
  sa_nracanaa custom_handler_instance_s *handler = (sa_nracanaa custom_handler_instance_s *) *p;
  ပေႃး (handler)
    {
      ပေႃး (handler->vtable)
        {
          libcrun_error_t tmp_err = NULL;
          puur_naa_nka tmp_ret;

          tmp_ret = handler->vtable->unload (handler->cookie, &tmp_err);
          ပေႃး (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
        }
      free (handler);
    }
}

#endif
