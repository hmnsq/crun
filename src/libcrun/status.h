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

#اگر_تعریف_نشده وضعیت_H
#تعریف_کن وضعیت_H

#ضمن <config.h>
#ضمن <ocispec/runtime_spec_schema_config_schema.h>
#ضمن "error.h"
#ضمن "container.h"

بنية libcrun_container_list_s
{
  بنية libcrun_container_list_s *بعدی;
  حرف *name;
};
تعريفنوع بنية libcrun_container_list_s libcrun_container_list_t;

بنية libcrun_container_وضعیت_s
{
  pid_t pid;
  بیعلامت دراز دراز process_شروع_زمان;
  حرف *bundle;
  حرف *rootfs;
  حرف *cgroup_مسیر;
  حرف *scope;
  صحيح systemd_cgroup;
  حرف *created;
  صحيح detached;
  صحيح no_new_keyring;
  حرف *external_descriptors;
  حرف *owner;
};
تعريفنوع بنية libcrun_container_وضعیت_s libcrun_container_وضعیت_t;

LIBCRUN_عمومی فراغ libcrun_free_container_وضعیت (libcrun_container_وضعیت_t *وضعیت);
LIBCRUN_عمومی صحيح libcrun_write_container_وضعیت (ثابت حرف *state_ریشه, ثابت حرف *id,
                                                   libcrun_container_وضعیت_t *وضعیت, libcrun_error_t *err);
LIBCRUN_عمومی صحيح libcrun_read_container_وضعیت (libcrun_container_وضعیت_t *وضعیت, ثابت حرف *state_ریشه,
                                                  ثابت حرف *id, libcrun_error_t *err);
LIBCRUN_عمومی فراغ libcrun_free_containers_list (libcrun_container_list_t *list);
LIBCRUN_عمومی صحيح libcrun_is_container_running (libcrun_container_وضعیت_t *وضعیت, libcrun_error_t *err);
LIBCRUN_عمومی صحيح libcrun_get_state_directory (حرف **بیرون, ثابت حرف *state_ریشه, ثابت حرف *id, libcrun_error_t *err);
LIBCRUN_عمومی صحيح libcrun_container_delete_وضعیت (ثابت حرف *state_ریشه, ثابت حرف *id, libcrun_error_t *err);
LIBCRUN_عمومی صحيح libcrun_get_containers_list (libcrun_container_list_t **ret, ثابت حرف *state_ریشه,
                                                libcrun_error_t *err);

بنية libcrun_container_iter_s
{
  libcrun_container_list_t *cur;
};
تعريفنوع بنية libcrun_container_iter_s libcrun_container_iter_t;

LIBCRUN_عمومی صحيح libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **بیرون,
                                           libcrun_error_t *err);
LIBCRUN_عمومی libcrun_container_iter_t *libcrun_container_list_iter (libcrun_container_list_t *l);
LIBCRUN_عمومی bool libcrun_container_iter_بعدی (libcrun_container_iter_t *it, ثابت حرف **id);
LIBCRUN_عمومی فراغ libcrun_container_iter_free (libcrun_container_iter_t *it);
LIBCRUN_عمومی فراغ libcrun_container_list_free (libcrun_container_list_t *l);

تعريفنوع تعداد
{
  LIBCRUN_CONTAINER_وضعیت_CREATING = 0,
  LIBCRUN_CONTAINER_وضعیت_CREATED,
  LIBCRUN_CONTAINER_وضعیت_RUNNING,
  LIBCRUN_CONTAINER_وضعیت_STOPPED,
  LIBCRUN_CONTAINER_وضعیت_PAUSED,
} libcrun_container_state_t;

بنية libcrun_وضعیت_s;
تعريفنوع بنية libcrun_وضعیت_s libcrun_وضعیت_t;

LIBCRUN_عمومی صحيح libcrun_container_وضعیت_بارگیری (libcrun_context_t *context, ثابت حرف *id, libcrun_وضعیت_t **بیرون,
                                                  libcrun_error_t *err);
LIBCRUN_عمومی فراغ libcrun_container_وضعیت_free (libcrun_وضعیت_t *st);
/* Resolve the live state of the container described by ST.  The state is
   probed on the first call and cached afterwards; STATE and RUNNING can be
   NULL.  */
LIBCRUN_عمومی صحيح libcrun_وضعیت_get_state (libcrun_وضعیت_t *st, libcrun_container_state_t *state, صحيح *running,
                                             libcrun_error_t *err);
LIBCRUN_عمومی ثابت حرف *libcrun_container_state_مقصد_string (libcrun_container_state_t state);
LIBCRUN_عمومی pid_t libcrun_وضعیت_get_pid (libcrun_وضعیت_t *st);
LIBCRUN_عمومی ثابت حرف *libcrun_وضعیت_get_bundle (libcrun_وضعیت_t *st);
LIBCRUN_عمومی ثابت حرف *libcrun_وضعیت_get_rootfs (libcrun_وضعیت_t *st);
LIBCRUN_عمومی ثابت حرف *libcrun_وضعیت_get_created (libcrun_وضعیت_t *st);
LIBCRUN_عمومی ثابت حرف *libcrun_وضعیت_get_owner (libcrun_وضعیت_t *st);
LIBCRUN_عمومی ثابت حرف *libcrun_وضعیت_get_scope (libcrun_وضعیت_t *st);
LIBCRUN_عمومی ثابت حرف *libcrun_وضعیت_get_external_descriptors (libcrun_وضعیت_t *st);
LIBCRUN_عمومی صحيح libcrun_container_get_state_string (libcrun_context_t *context, ثابت حرف *id, ثابت حرف **بیرون,
                                                       صحيح *running, libcrun_error_t *err);
LIBCRUN_عمومی صحيح libcrun_get_state_dir (حرف **بیرون, ثابت حرف *state_ریشه, ثابت حرف *id, libcrun_error_t *err);

صحيح libcrun_وضعیت_check_directories (ثابت حرف *state_ریشه, ثابت حرف *id, libcrun_error_t *err);
صحيح libcrun_وضعیت_create_exec_fifo (ثابت حرف *state_ریشه, ثابت حرف *id, libcrun_error_t *err);
صحيح libcrun_وضعیت_write_exec_fifo (ثابت حرف *state_ریشه, ثابت حرف *id, libcrun_error_t *err);
صحيح libcrun_وضعیت_has_read_exec_fifo (ثابت حرف *state_ریشه, ثابت حرف *id, libcrun_error_t *err);
صحيح libcrun_check_pid_valid (libcrun_container_وضعیت_t *وضعیت, libcrun_error_t *err);
صحيح get_پیاپی_directory (حرف **بیرون, ثابت حرف *state_ریشه, libcrun_error_t *err);
صحيح get_shared_خالی_directory_مسیر (حرف **بیرون, ثابت حرف *state_ریشه, libcrun_error_t *err);

ساكن مضمن فراغ
libcrun_free_container_listp (فراغ *p)
{
  libcrun_container_list_t **l = (libcrun_container_list_t **) p;
  إذا (*l != عدم)
    libcrun_free_containers_list (*l);
}

#تعریف_کن cleanup_container_وضعیت __attribute__ ((cleanup (libcrun_free_container_وضعیت)))
#تعریف_کن cleanup_container_list __attribute__ ((cleanup (libcrun_free_container_listp)))

#پایان_اگر
