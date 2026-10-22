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

#جے_بنا_تعریف حالت_H
#تعریف_کرو حالت_H

#ضمن <config.h>
#ضمن <ocispec/runtime_spec_schema_config_schema.h>
#ضمن "error.h"
#ضمن "container.h"

بنية libcrun_container_list_s
{
  بنية libcrun_container_list_s *اگلا;
  حرف *name;
};
تعريفنوع بنية libcrun_container_list_s libcrun_container_list_t;

بنية libcrun_container_حالت_s
{
  pid_t pid;
  بنانشانی لما لما process_شروع_ویلا;
  حرف *bundle;
  حرف *rootfs;
  حرف *cgroup_رستہ;
  حرف *scope;
  صحيح systemd_cgroup;
  حرف *created;
  صحيح detached;
  صحيح no_new_keyring;
  حرف *external_descriptors;
  حرف *owner;
};
تعريفنوع بنية libcrun_container_حالت_s libcrun_container_حالت_t;

LIBCRUN_عوامی فراغ libcrun_free_container_حالت (libcrun_container_حالت_t *حالت);
LIBCRUN_عوامی صحيح libcrun_write_container_حالت (ثابت حرف *state_جڑ, ثابت حرف *id,
                                                   libcrun_container_حالت_t *حالت, libcrun_error_t *err);
LIBCRUN_عوامی صحيح libcrun_read_container_حالت (libcrun_container_حالت_t *حالت, ثابت حرف *state_جڑ,
                                                  ثابت حرف *id, libcrun_error_t *err);
LIBCRUN_عوامی فراغ libcrun_free_containers_list (libcrun_container_list_t *list);
LIBCRUN_عوامی صحيح libcrun_is_container_running (libcrun_container_حالت_t *حالت, libcrun_error_t *err);
LIBCRUN_عوامی صحيح libcrun_get_state_directory (حرف **باہرلا, ثابت حرف *state_جڑ, ثابت حرف *id, libcrun_error_t *err);
LIBCRUN_عوامی صحيح libcrun_container_delete_حالت (ثابت حرف *state_جڑ, ثابت حرف *id, libcrun_error_t *err);
LIBCRUN_عوامی صحيح libcrun_get_containers_list (libcrun_container_list_t **ret, ثابت حرف *state_جڑ,
                                                libcrun_error_t *err);

بنية libcrun_container_iter_s
{
  libcrun_container_list_t *cur;
};
تعريفنوع بنية libcrun_container_iter_s libcrun_container_iter_t;

LIBCRUN_عوامی صحيح libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **باہرلا,
                                           libcrun_error_t *err);
LIBCRUN_عوامی libcrun_container_iter_t *libcrun_container_list_iter (libcrun_container_list_t *l);
LIBCRUN_عوامی bool libcrun_container_iter_اگلا (libcrun_container_iter_t *it, ثابت حرف **id);
LIBCRUN_عوامی فراغ libcrun_container_iter_free (libcrun_container_iter_t *it);
LIBCRUN_عوامی فراغ libcrun_container_list_free (libcrun_container_list_t *l);

تعريفنوع تعداد
{
  LIBCRUN_CONTAINER_حالت_CREATING = 0,
  LIBCRUN_CONTAINER_حالت_CREATED,
  LIBCRUN_CONTAINER_حالت_RUNNING,
  LIBCRUN_CONTAINER_حالت_STOPPED,
  LIBCRUN_CONTAINER_حالت_PAUSED,
} libcrun_container_state_t;

بنية libcrun_حالت_s;
تعريفنوع بنية libcrun_حالت_s libcrun_حالت_t;

LIBCRUN_عوامی صحيح libcrun_container_حالت_لدو (libcrun_context_t *context, ثابت حرف *id, libcrun_حالت_t **باہرلا,
                                                  libcrun_error_t *err);
LIBCRUN_عوامی فراغ libcrun_container_حالت_free (libcrun_حالت_t *st);
/* Resolve the live state of the container described by ST.  The state is
   probed on the first call and cached afterwards; STATE and RUNNING can be
   NULL.  */
LIBCRUN_عوامی صحيح libcrun_حالت_get_state (libcrun_حالت_t *st, libcrun_container_state_t *state, صحيح *running,
                                             libcrun_error_t *err);
LIBCRUN_عوامی ثابت حرف *libcrun_container_state_منزل_string (libcrun_container_state_t state);
LIBCRUN_عوامی pid_t libcrun_حالت_get_pid (libcrun_حالت_t *st);
LIBCRUN_عوامی ثابت حرف *libcrun_حالت_get_bundle (libcrun_حالت_t *st);
LIBCRUN_عوامی ثابت حرف *libcrun_حالت_get_rootfs (libcrun_حالت_t *st);
LIBCRUN_عوامی ثابت حرف *libcrun_حالت_get_created (libcrun_حالت_t *st);
LIBCRUN_عوامی ثابت حرف *libcrun_حالت_get_owner (libcrun_حالت_t *st);
LIBCRUN_عوامی ثابت حرف *libcrun_حالت_get_scope (libcrun_حالت_t *st);
LIBCRUN_عوامی ثابت حرف *libcrun_حالت_get_external_descriptors (libcrun_حالت_t *st);
LIBCRUN_عوامی صحيح libcrun_container_get_state_string (libcrun_context_t *context, ثابت حرف *id, ثابت حرف **باہرلا,
                                                       صحيح *running, libcrun_error_t *err);
LIBCRUN_عوامی صحيح libcrun_get_state_dir (حرف **باہرلا, ثابت حرف *state_جڑ, ثابت حرف *id, libcrun_error_t *err);

صحيح libcrun_حالت_check_directories (ثابت حرف *state_جڑ, ثابت حرف *id, libcrun_error_t *err);
صحيح libcrun_حالت_create_exec_fifo (ثابت حرف *state_جڑ, ثابت حرف *id, libcrun_error_t *err);
صحيح libcrun_حالت_write_exec_fifo (ثابت حرف *state_جڑ, ثابت حرف *id, libcrun_error_t *err);
صحيح libcrun_حالت_has_read_exec_fifo (ثابت حرف *state_جڑ, ثابت حرف *id, libcrun_error_t *err);
صحيح libcrun_check_pid_valid (libcrun_container_حالت_t *حالت, libcrun_error_t *err);
صحيح get_لگاتار_directory (حرف **باہرلا, ثابت حرف *state_جڑ, libcrun_error_t *err);
صحيح get_shared_خالی_تھاواں_directory_رستہ (حرف **باہرلا, ثابت حرف *state_جڑ, libcrun_error_t *err);

ساكن مضمن فراغ
libcrun_free_container_listp (فراغ *p)
{
  libcrun_container_list_t **l = (libcrun_container_list_t **) p;
  إذا (*l != عدم)
    libcrun_free_containers_list (*l);
}

#تعریف_کرو cleanup_container_حالت __attribute__ ((cleanup (libcrun_free_container_حالت)))
#تعریف_کرو cleanup_container_list __attribute__ ((cleanup (libcrun_free_container_listp)))

#جے_انت
