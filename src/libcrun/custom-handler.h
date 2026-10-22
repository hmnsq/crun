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

#ଅନ୍ତର୍ଭାଵ "container.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>

ସଂରଚନା custom_handler_s
{
  ଅଚଳ ଅକ୍ଷର *name;
  ଅଚଳ ଅକ୍ଷର *feature_string;

  ଅଚଳ ଅକ୍ଷର *alias;

  ଅଚଳ bool supports_open_ବୃକ୍ଷ_namespace;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*load) (ଶୂନ୍ୟ **cookie, libcrun_error_t *err);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*unload) (ଶୂନ୍ୟ *cookie, libcrun_error_t *err);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*run_func) (ଶୂନ୍ୟ *cookie, libcrun_container_t *container,
                   ଅଚଳ ଅକ୍ଷର *pathname, ଅକ୍ଷର *ଅଚଳ argv[]);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*exec_func) (ଶୂନ୍ୟ *cookie, libcrun_container_t *container,
                    ଅଚଳ ଅକ୍ଷର *pathname, ଅକ୍ଷର *ଅଚଳ argv[]);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*configure_container) (ଶୂନ୍ୟ *cookie, ଗଣନା handler_configure_phase phase,
                              libcrun_context_t *context, libcrun_container_t *container,
                              ଅଚଳ ଅକ୍ଷର *rootfs, libcrun_error_t *err);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*can_handle_container) (libcrun_container_t *container, libcrun_error_t *err);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*modify_oci_configuration) (ଶୂନ୍ୟ *cookie, libcrun_context_t *context,
                                   runtime_spec_schema_config_schema *def,
                                   libcrun_error_t *err);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*close_fds) (ଶୂନ୍ୟ *cookie, libcrun_container_t *container,
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା preserve_fds, libcrun_error_t *err);
};

ସଂରଚନା custom_handler_manager_s;

LIBCRUN_PUBLIC ସଂରଚନା custom_handler_manager_s *libcrun_handler_manager_create (libcrun_error_t *err);
LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_handler_manager_load_directory (ସଂରଚନା custom_handler_manager_s *manager, ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err);
LIBCRUN_PUBLIC ଶୂନ୍ୟ handler_manager_free (ସଂରଚନା custom_handler_manager_s *manager);

LIBCRUN_PUBLIC ସଂରଚନା custom_handler_s *handler_by_name (ସଂରଚନା custom_handler_manager_s *manager, ଅଚଳ ଅକ୍ଷର *name);
LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_handler_manager_print_feature_tags (ସଂରଚନା custom_handler_manager_s *manager, FILE *out);
LIBCRUN_PUBLIC ଅକ୍ଷର **libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err);

ସଂରଚନା custom_handler_instance_s
{
  ସଂରଚନା custom_handler_s *vtable;
  ଶୂନ୍ୟ *cookie;
};

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_configure_handler (ସଂରଚନା custom_handler_manager_s *manager,
                                              libcrun_context_t *context,
                                              libcrun_container_t *container,
                                              ସଂରଚନା custom_handler_instance_s **out,
                                              libcrun_error_t *err);

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା custom_handler_s *(*run_oci_get_handler_cb) ();

#define cleanup_custom_handler_instance __attribute__ ((cleanup (cleanup_custom_handler_instancep)))

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
cleanup_custom_handler_instancep (ସଂରଚନା custom_handler_instance_s **p)
{
  ସଂରଚନା custom_handler_instance_s *handler = (ସଂରଚନା custom_handler_instance_s *) *p;
  ଯଦି (handler)
    {
      ଯଦି (handler->vtable)
        {
          libcrun_error_t tmp_err = ଖାଲି;
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା tmp_ret;

          tmp_ret = handler->vtable->unload (handler->cookie, &tmp_err);
          ଯଦି (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
        }
      ମୁକ୍ତ (handler);
    }
}

#endif
