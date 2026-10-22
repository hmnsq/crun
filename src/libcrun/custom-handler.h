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

#અન્તર્ભાવ "container.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ <stdio.h>

સંરચના custom_handler_s
{
  અચલ અક્ષર *name;
  અચલ અક્ષર *feature_string;

  અચલ અક્ષર *alias;

  અચલ bool supports_open_વૃક્ષ_namespace;

  પૂર્ણાંક (*load) (શૂન્ય **cookie, libcrun_error_t *err);
  પૂર્ણાંક (*unload) (શૂન્ય *cookie, libcrun_error_t *err);

  પૂર્ણાંક (*run_func) (શૂન્ય *cookie, libcrun_container_t *container,
                   અચલ અક્ષર *pathname, અક્ષર *અચલ argv[]);

  પૂર્ણાંક (*exec_func) (શૂન્ય *cookie, libcrun_container_t *container,
                    અચલ અક્ષર *pathname, અક્ષર *અચલ argv[]);

  પૂર્ણાંક (*configure_container) (શૂન્ય *cookie, ગણના handler_configure_phase phase,
                              libcrun_context_t *context, libcrun_container_t *container,
                              અચલ અક્ષર *rootfs, libcrun_error_t *err);

  પૂર્ણાંક (*can_handle_container) (libcrun_container_t *container, libcrun_error_t *err);

  પૂર્ણાંક (*modify_oci_configuration) (શૂન્ય *cookie, libcrun_context_t *context,
                                   runtime_spec_schema_config_schema *def,
                                   libcrun_error_t *err);

  પૂર્ણાંક (*close_fds) (શૂન્ય *cookie, libcrun_container_t *container,
                    પૂર્ણાંક preserve_fds, libcrun_error_t *err);
};

સંરચના custom_handler_manager_s;

LIBCRUN_PUBLIC સંરચના custom_handler_manager_s *libcrun_handler_manager_create (libcrun_error_t *err);
LIBCRUN_PUBLIC પૂર્ણાંક libcrun_handler_manager_load_directory (સંરચના custom_handler_manager_s *manager, અચલ અક્ષર *path, libcrun_error_t *err);
LIBCRUN_PUBLIC શૂન્ય handler_manager_free (સંરચના custom_handler_manager_s *manager);

LIBCRUN_PUBLIC સંરચના custom_handler_s *handler_by_name (સંરચના custom_handler_manager_s *manager, અચલ અક્ષર *name);
LIBCRUN_PUBLIC શૂન્ય libcrun_handler_manager_print_feature_tags (સંરચના custom_handler_manager_s *manager, FILE *out);
LIBCRUN_PUBLIC અક્ષર **libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err);

સંરચના custom_handler_instance_s
{
  સંરચના custom_handler_s *vtable;
  શૂન્ય *cookie;
};

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_configure_handler (સંરચના custom_handler_manager_s *manager,
                                              libcrun_context_t *context,
                                              libcrun_container_t *container,
                                              સંરચના custom_handler_instance_s **out,
                                              libcrun_error_t *err);

પ્રકારવ્યાખ્યા સંરચના custom_handler_s *(*run_oci_get_handler_cb) ();

#define cleanup_custom_handler_instance __attribute__ ((cleanup (cleanup_custom_handler_instancep)))

સ્થિર અંતઃરેખ શૂન્ય
cleanup_custom_handler_instancep (સંરચના custom_handler_instance_s **p)
{
  સંરચના custom_handler_instance_s *handler = (સંરચના custom_handler_instance_s *) *p;
  જો (handler)
    {
      જો (handler->vtable)
        {
          libcrun_error_t tmp_err = ખાલી;
          પૂર્ણાંક tmp_ret;

          tmp_ret = handler->vtable->unload (handler->cookie, &tmp_err);
          જો (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
        }
      મુક્ત (handler);
    }
}

#endif
