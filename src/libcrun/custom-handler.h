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

#నిర్వచితం_కాకపోతే CUSTOM_HANDLER_H
#నిర్వచించు CUSTOM_HANDLER_H

#అన్తర్భావ "container.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ <stdio.h>

నిర్మాణం custom_handler_s
{
  స్థిరరాశి అక్షరం *name;
  స్థిరరాశి అక్షరం *feature_string;

  స్థిరరాశి అక్షరం *alias;

  స్థిరరాశి bool supports_open_వృక్షం_namespace;

  పూర్ణాంకం (*ఎక్కించు) (శూన్య **cookie, libcrun_error_t *err);
  పూర్ణాంకం (*unload) (శూన్య *cookie, libcrun_error_t *err);

  పూర్ణాంకం (*నిరంతరం_func) (శూన్య *cookie, libcrun_container_t *container,
                   స్థిరరాశి అక్షరం *pathname, అక్షరం *స్థిరరాశి argv[]);

  పూర్ణాంకం (*exec_func) (శూన్య *cookie, libcrun_container_t *container,
                    స్థిరరాశి అక్షరం *pathname, అక్షరం *స్థిరరాశి argv[]);

  పూర్ణాంకం (*configure_container) (శూన్య *cookie, గణన handler_configure_phase phase,
                              libcrun_context_t *context, libcrun_container_t *container,
                              స్థిరరాశి అక్షరం *rootfs, libcrun_error_t *err);

  పూర్ణాంకం (*can_handle_container) (libcrun_container_t *container, libcrun_error_t *err);

  పూర్ణాంకం (*modify_oci_configuration) (శూన్య *cookie, libcrun_context_t *context,
                                   runtime_spec_schema_config_schema *def,
                                   libcrun_error_t *err);

  పూర్ణాంకం (*close_fds) (శూన్య *cookie, libcrun_container_t *container,
                    పూర్ణాంకం preserve_fds, libcrun_error_t *err);
};

నిర్మాణం custom_handler_manager_s;

LIBCRUN_ప్రజా నిర్మాణం custom_handler_manager_s *libcrun_handler_manager_create (libcrun_error_t *err);
LIBCRUN_ప్రజా పూర్ణాంకం libcrun_handler_manager_ఎక్కించు_directory (నిర్మాణం custom_handler_manager_s *manager, స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err);
LIBCRUN_ప్రజా శూన్య handler_manager_free (నిర్మాణం custom_handler_manager_s *manager);

LIBCRUN_ప్రజా నిర్మాణం custom_handler_s *handler_by_name (నిర్మాణం custom_handler_manager_s *manager, స్థిరరాశి అక్షరం *name);
LIBCRUN_ప్రజా శూన్య libcrun_handler_manager_print_feature_ట్యాగులు (నిర్మాణం custom_handler_manager_s *manager, FILE *బహిర్గతం);
LIBCRUN_ప్రజా అక్షరం **libcrun_context_get_handler_feature_ట్యాగులు (libcrun_context_t *ctx, libcrun_error_t *err);

నిర్మాణం custom_handler_instance_s
{
  నిర్మాణం custom_handler_s *vtable;
  శూన్య *cookie;
};

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_configure_handler (నిర్మాణం custom_handler_manager_s *manager,
                                              libcrun_context_t *context,
                                              libcrun_container_t *container,
                                              నిర్మాణం custom_handler_instance_s **బహిర్గతం,
                                              libcrun_error_t *err);

రకనిర్వచనం నిర్మాణం custom_handler_s *(*నిరంతరం_oci_get_handler_cb) ();

#నిర్వచించు cleanup_custom_handler_instance __attribute__ ((cleanup (cleanup_custom_handler_instancep)))

స్థిర అంతఃవరుస శూన్య
cleanup_custom_handler_instancep (నిర్మాణం custom_handler_instance_s **p)
{
  నిర్మాణం custom_handler_instance_s *handler = (నిర్మాణం custom_handler_instance_s *) *p;
  ఒకవేళ (handler)
    {
      ఒకవేళ (handler->vtable)
        {
          libcrun_error_t tmp_err = ఖాళీ;
          పూర్ణాంకం tmp_ret;

          tmp_ret = handler->vtable->unload (handler->cookie, &tmp_err);
          ఒకవేళ (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
        }
      విడుదల (handler);
    }
}

#అయితే_ముగింపు
