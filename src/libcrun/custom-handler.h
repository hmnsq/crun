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

#অন্তর্ভাৱ "container.h"
#অন্তর্ভাৱ "utils.h"
#অন্তর্ভাৱ <stdio.h>

সংরচনা custom_handler_s
{
  ধ্রুৱ ৱর্ণ *name;
  ধ্রুৱ ৱর্ণ *feature_string;

  ধ্রুৱ ৱর্ণ *alias;

  ধ্রুৱ bool supports_open_বৃক্ষ_namespace;

  পূর্ণাঙ্ক (*load) (শূন্য **cookie, libcrun_error_t *err);
  পূর্ণাঙ্ক (*unload) (শূন্য *cookie, libcrun_error_t *err);

  পূর্ণাঙ্ক (*run_func) (শূন্য *cookie, libcrun_container_t *container,
                   ধ্রুৱ ৱর্ণ *pathname, ৱর্ণ *ধ্রুৱ argv[]);

  পূর্ণাঙ্ক (*exec_func) (শূন্য *cookie, libcrun_container_t *container,
                    ধ্রুৱ ৱর্ণ *pathname, ৱর্ণ *ধ্রুৱ argv[]);

  পূর্ণাঙ্ক (*configure_container) (শূন্য *cookie, গণনা handler_configure_phase phase,
                              libcrun_context_t *context, libcrun_container_t *container,
                              ধ্রুৱ ৱর্ণ *rootfs, libcrun_error_t *err);

  পূর্ণাঙ্ক (*can_handle_container) (libcrun_container_t *container, libcrun_error_t *err);

  পূর্ণাঙ্ক (*modify_oci_configuration) (শূন্য *cookie, libcrun_context_t *context,
                                   runtime_spec_schema_config_schema *def,
                                   libcrun_error_t *err);

  পূর্ণাঙ্ক (*close_fds) (শূন্য *cookie, libcrun_container_t *container,
                    পূর্ণাঙ্ক preserve_fds, libcrun_error_t *err);
};

সংরচনা custom_handler_manager_s;

LIBCRUN_PUBLIC সংরচনা custom_handler_manager_s *libcrun_handler_manager_create (libcrun_error_t *err);
LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_handler_manager_load_directory (সংরচনা custom_handler_manager_s *manager, ধ্রুৱ ৱর্ণ *path, libcrun_error_t *err);
LIBCRUN_PUBLIC শূন্য handler_manager_free (সংরচনা custom_handler_manager_s *manager);

LIBCRUN_PUBLIC সংরচনা custom_handler_s *handler_by_name (সংরচনা custom_handler_manager_s *manager, ধ্রুৱ ৱর্ণ *name);
LIBCRUN_PUBLIC শূন্য libcrun_handler_manager_print_feature_tags (সংরচনা custom_handler_manager_s *manager, FILE *out);
LIBCRUN_PUBLIC ৱর্ণ **libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err);

সংরচনা custom_handler_instance_s
{
  সংরচনা custom_handler_s *vtable;
  শূন্য *cookie;
};

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_configure_handler (সংরচনা custom_handler_manager_s *manager,
                                              libcrun_context_t *context,
                                              libcrun_container_t *container,
                                              সংরচনা custom_handler_instance_s **out,
                                              libcrun_error_t *err);

প্রকারনির্দেশ সংরচনা custom_handler_s *(*run_oci_get_handler_cb) ();

#define cleanup_custom_handler_instance __attribute__ ((cleanup (cleanup_custom_handler_instancep)))

স্থির অন্তঃস্থ শূন্য
cleanup_custom_handler_instancep (সংরচনা custom_handler_instance_s **p)
{
  সংরচনা custom_handler_instance_s *handler = (সংরচনা custom_handler_instance_s *) *p;
  যদি (handler)
    {
      যদি (handler->vtable)
        {
          libcrun_error_t tmp_err = রিক্ত;
          পূর্ণাঙ্ক tmp_ret;

          tmp_ret = handler->vtable->unload (handler->cookie, &tmp_err);
          যদি (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
        }
      মুক্ত (handler);
    }
}

#endif
