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

#اگر_غیر_تعریف_شدہ CUSTOM_HANDLER_H
#تعریف_کرو CUSTOM_HANDLER_H

#شامل "container.h"
#شامل "utils.h"
#شامل <stdio.h>

ساخت custom_handler_s
{
  ثابت حرف *name;
  ثابت حرف *feature_string;

  ثابت حرف *alias;

  ثابت bool supports_open_درخت_namespace;

  صحیح (*بھرو) (خالی **cookie, libcrun_error_t *err);
  صحیح (*unload) (خالی *cookie, libcrun_error_t *err);

  صحیح (*تسلسل_func) (خالی *cookie, libcrun_container_t *container,
                   ثابت حرف *pathname, حرف *ثابت argv[]);

  صحیح (*exec_func) (خالی *cookie, libcrun_container_t *container,
                    ثابت حرف *pathname, حرف *ثابت argv[]);

  صحیح (*configure_container) (خالی *cookie, شمار handler_configure_phase phase,
                              libcrun_context_t *context, libcrun_container_t *container,
                              ثابت حرف *rootfs, libcrun_error_t *err);

  صحیح (*can_handle_container) (libcrun_container_t *container, libcrun_error_t *err);

  صحیح (*modify_oci_configuration) (خالی *cookie, libcrun_context_t *context,
                                   runtime_spec_schema_config_schema *def,
                                   libcrun_error_t *err);

  صحیح (*close_fds) (خالی *cookie, libcrun_container_t *container,
                    صحیح preserve_fds, libcrun_error_t *err);
};

ساخت custom_handler_manager_s;

LIBCRUN_عوامی ساخت custom_handler_manager_s *libcrun_handler_manager_create (libcrun_error_t *err);
LIBCRUN_عوامی صحیح libcrun_handler_manager_بھرو_directory (ساخت custom_handler_manager_s *manager, ثابت حرف *راستہ, libcrun_error_t *err);
LIBCRUN_عوامی خالی handler_manager_free (ساخت custom_handler_manager_s *manager);

LIBCRUN_عوامی ساخت custom_handler_s *handler_by_name (ساخت custom_handler_manager_s *manager, ثابت حرف *name);
LIBCRUN_عوامی خالی libcrun_handler_manager_print_feature_لیبل_فہرست (ساخت custom_handler_manager_s *manager, FILE *برآمد);
LIBCRUN_عوامی حرف **libcrun_context_get_handler_feature_لیبل_فہرست (libcrun_context_t *ctx, libcrun_error_t *err);

ساخت custom_handler_instance_s
{
  ساخت custom_handler_s *vtable;
  خالی *cookie;
};

LIBCRUN_عوامی صحیح libcrun_configure_handler (ساخت custom_handler_manager_s *manager,
                                              libcrun_context_t *context,
                                              libcrun_container_t *container,
                                              ساخت custom_handler_instance_s **برآمد,
                                              libcrun_error_t *err);

قسمتعریف ساخت custom_handler_s *(*تسلسل_oci_get_handler_cb) ();

#تعریف_کرو cleanup_custom_handler_instance __attribute__ ((cleanup (cleanup_custom_handler_instancep)))

ساکن اندرونی خالی
cleanup_custom_handler_instancep (ساخت custom_handler_instance_s **p)
{
  ساخت custom_handler_instance_s *handler = (ساخت custom_handler_instance_s *) *p;
  اگر (handler)
    {
      اگر (handler->vtable)
        {
          libcrun_error_t tmp_err = تہی;
          صحیح tmp_ret;

          tmp_ret = handler->vtable->unload (handler->cookie, &tmp_err);
          اگر (UNLIKELY (tmp_ret < 0))
            crun_error_release (&tmp_err);
        }
      آزاد (handler);
    }
}

#اگر_ختم
