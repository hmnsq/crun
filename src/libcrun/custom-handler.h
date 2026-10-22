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

#অন্তর্ভাব "container.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব <stdio.h>

কাঠামো custom_handler_s
{
  ধ্রুবক অক্ষর *name;
  ধ্রুবক অক্ষর *feature_string;

  ধ্রুবক অক্ষর *alias;

  ধ্রুবক bool supports_open_বৃক্ষ_namespace;

  পূর্ণসংখ্যা (*load) (শূন্য **cookie, libcrun_error_t *err);
  পূর্ণসংখ্যা (*unload) (শূন্য *cookie, libcrun_error_t *err);

  পূর্ণসংখ্যা (*run_func) (শূন্য *cookie, libcrun_container_t *container,
                   ধ্রুবক অক্ষর *pathname, অক্ষর *ধ্রুবক argv[]);

  পূর্ণসংখ্যা (*exec_func) (শূন্য *cookie, libcrun_container_t *container,
                    ধ্রুবক অক্ষর *pathname, অক্ষর *ধ্রুবক argv[]);

  পূর্ণসংখ্যা (*configure_container) (শূন্য *cookie, গণনা handler_configure_phase phase,
                              libcrun_context_t *context, libcrun_container_t *container,
                              ধ্রুবক অক্ষর *rootfs, libcrun_error_t *err);

  পূর্ণসংখ্যা (*can_handle_container) (libcrun_container_t *container, libcrun_error_t *err);

  পূর্ণসংখ্যা (*modify_oci_configuration) (শূন্য *cookie, libcrun_context_t *context,
                                   runtime_spec_schema_config_schema *def,
                                   libcrun_error_t *err);

  পূর্ণসংখ্যা (*close_fds) (শূন্য *cookie, libcrun_container_t *container,
                    পূর্ণসংখ্যা preserve_fds, libcrun_error_t *err);
};

কাঠামো custom_handler_manager_s;

LIBCRUN_PUBLIC কাঠামো custom_handler_manager_s *libcrun_handler_manager_create (libcrun_error_t *err);
LIBCRUN_PUBLIC পূর্ণসংখ্যা libcrun_handler_manager_load_directory (কাঠামো custom_handler_manager_s *manager, ধ্রুবক অক্ষর *path, libcrun_error_t *err);
LIBCRUN_PUBLIC শূন্য handler_manager_free (কাঠামো custom_handler_manager_s *manager);

LIBCRUN_PUBLIC কাঠামো custom_handler_s *handler_by_name (কাঠামো custom_handler_manager_s *manager, ধ্রুবক অক্ষর *name);
LIBCRUN_PUBLIC শূন্য libcrun_handler_manager_print_feature_tags (কাঠামো custom_handler_manager_s *manager, FILE *out);
LIBCRUN_PUBLIC অক্ষর **libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err);

কাঠামো custom_handler_instance_s
{
  কাঠামো custom_handler_s *vtable;
  শূন্য *cookie;
};

LIBCRUN_PUBLIC পূর্ণসংখ্যা libcrun_configure_handler (কাঠামো custom_handler_manager_s *manager,
                                              libcrun_context_t *context,
                                              libcrun_container_t *container,
                                              কাঠামো custom_handler_instance_s **out,
                                              libcrun_error_t *err);

প্রকারলেখ কাঠামো custom_handler_s *(*run_oci_get_handler_cb) ();

#define cleanup_custom_handler_instance __attribute__ ((cleanup (cleanup_custom_handler_instancep)))

স্থির অন্তঃরেখ শূন্য
cleanup_custom_handler_instancep (কাঠামো custom_handler_instance_s **p)
{
  কাঠামো custom_handler_instance_s *handler = (কাঠামো custom_handler_instance_s *) *p;
  যদি (handler)
    {
      যদি (handler->vtable)
        {
          libcrun_error_t tmp_err = ফাঁকা;
          পূর্ণসংখ্যা tmp_ret;

          tmp_ret = handler->vtable->unload (handler->cookie, &tmp_err);
          যদি (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
        }
      মুক্ত (handler);
    }
}

#endif
