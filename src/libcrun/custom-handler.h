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

#അന്തര്ഭാവ "container.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ <stdio.h>

ഘടന custom_handler_s
{
  സ്ഥിരം അക്ഷരം *name;
  സ്ഥിരം അക്ഷരം *feature_string;

  സ്ഥിരം അക്ഷരം *alias;

  സ്ഥിരം bool supports_open_വൃക്ഷം_namespace;

  പൂർണ്ണസംഖ്യ (*load) (ശൂന്യം **cookie, libcrun_error_t *err);
  പൂർണ്ണസംഖ്യ (*unload) (ശൂന്യം *cookie, libcrun_error_t *err);

  പൂർണ്ണസംഖ്യ (*run_func) (ശൂന്യം *cookie, libcrun_container_t *container,
                   സ്ഥിരം അക്ഷരം *pathname, അക്ഷരം *സ്ഥിരം argv[]);

  പൂർണ്ണസംഖ്യ (*exec_func) (ശൂന്യം *cookie, libcrun_container_t *container,
                    സ്ഥിരം അക്ഷരം *pathname, അക്ഷരം *സ്ഥിരം argv[]);

  പൂർണ്ണസംഖ്യ (*configure_container) (ശൂന്യം *cookie, എണ്ണൽ handler_configure_phase phase,
                              libcrun_context_t *context, libcrun_container_t *container,
                              സ്ഥിരം അക്ഷരം *rootfs, libcrun_error_t *err);

  പൂർണ്ണസംഖ്യ (*can_handle_container) (libcrun_container_t *container, libcrun_error_t *err);

  പൂർണ്ണസംഖ്യ (*modify_oci_configuration) (ശൂന്യം *cookie, libcrun_context_t *context,
                                   runtime_spec_schema_config_schema *def,
                                   libcrun_error_t *err);

  പൂർണ്ണസംഖ്യ (*close_fds) (ശൂന്യം *cookie, libcrun_container_t *container,
                    പൂർണ്ണസംഖ്യ preserve_fds, libcrun_error_t *err);
};

ഘടന custom_handler_manager_s;

LIBCRUN_PUBLIC ഘടന custom_handler_manager_s *libcrun_handler_manager_create (libcrun_error_t *err);
LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_handler_manager_load_directory (ഘടന custom_handler_manager_s *manager, സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err);
LIBCRUN_PUBLIC ശൂന്യം handler_manager_free (ഘടന custom_handler_manager_s *manager);

LIBCRUN_PUBLIC ഘടന custom_handler_s *handler_by_name (ഘടന custom_handler_manager_s *manager, സ്ഥിരം അക്ഷരം *name);
LIBCRUN_PUBLIC ശൂന്യം libcrun_handler_manager_print_feature_tags (ഘടന custom_handler_manager_s *manager, FILE *out);
LIBCRUN_PUBLIC അക്ഷരം **libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err);

ഘടന custom_handler_instance_s
{
  ഘടന custom_handler_s *vtable;
  ശൂന്യം *cookie;
};

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_configure_handler (ഘടന custom_handler_manager_s *manager,
                                              libcrun_context_t *context,
                                              libcrun_container_t *container,
                                              ഘടന custom_handler_instance_s **out,
                                              libcrun_error_t *err);

തരനിർവചനം ഘടന custom_handler_s *(*run_oci_get_handler_cb) ();

#define cleanup_custom_handler_instance __attribute__ ((cleanup (cleanup_custom_handler_instancep)))

സ്ഥിതം വരിയിൽ ശൂന്യം
cleanup_custom_handler_instancep (ഘടന custom_handler_instance_s **p)
{
  ഘടന custom_handler_instance_s *handler = (ഘടന custom_handler_instance_s *) *p;
  എങ്കിൽ (handler)
    {
      എങ്കിൽ (handler->vtable)
        {
          libcrun_error_t tmp_err = ശൂന്യ;
          പൂർണ്ണസംഖ്യ tmp_ret;

          tmp_ret = handler->vtable->unload (handler->cookie, &tmp_err);
          എങ്കിൽ (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
        }
      സ്വതന്ത്രം (handler);
    }
}

#endif
