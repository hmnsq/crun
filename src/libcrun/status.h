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

#اگر_غیر_تعریف_شدہ حالت_H
#تعریف_کرو حالت_H

#شامل <config.h>
#شامل <ocispec/runtime_spec_schema_config_schema.h>
#شامل "error.h"
#شامل "container.h"

ساخت libcrun_container_list_s
{
  ساخت libcrun_container_list_s *اگلا;
  حرف *name;
};
قسمتعریف ساخت libcrun_container_list_s libcrun_container_list_t;

ساخت libcrun_container_حالت_s
{
  pid_t pid;
  غیرعلامتی طویل طویل process_آغاز_وقت;
  حرف *bundle;
  حرف *rootfs;
  حرف *cgroup_راستہ;
  حرف *scope;
  صحیح systemd_cgroup;
  حرف *created;
  صحیح detached;
  صحیح no_new_keyring;
  حرف *external_descriptors;
  حرف *owner;
};
قسمتعریف ساخت libcrun_container_حالت_s libcrun_container_حالت_t;

LIBCRUN_عوامی خالی libcrun_free_container_حالت (libcrun_container_حالت_t *حالت_2);
LIBCRUN_عوامی صحیح libcrun_write_container_حالت (ثابت حرف *state_جڑ, ثابت حرف *id,
                                                   libcrun_container_حالت_t *حالت_2, libcrun_error_t *err);
LIBCRUN_عوامی صحیح libcrun_read_container_حالت (libcrun_container_حالت_t *حالت_2, ثابت حرف *state_جڑ,
                                                  ثابت حرف *id, libcrun_error_t *err);
LIBCRUN_عوامی خالی libcrun_free_containers_list (libcrun_container_list_t *list);
LIBCRUN_عوامی صحیح libcrun_is_container_running (libcrun_container_حالت_t *حالت_2, libcrun_error_t *err);
LIBCRUN_عوامی صحیح libcrun_get_state_directory (حرف **برآمد, ثابت حرف *state_جڑ, ثابت حرف *id, libcrun_error_t *err);
LIBCRUN_عوامی صحیح libcrun_container_delete_حالت (ثابت حرف *state_جڑ, ثابت حرف *id, libcrun_error_t *err);
LIBCRUN_عوامی صحیح libcrun_get_containers_list (libcrun_container_list_t **ret, ثابت حرف *state_جڑ,
                                                libcrun_error_t *err);

ساخت libcrun_container_iter_s
{
  libcrun_container_list_t *cur;
};
قسمتعریف ساخت libcrun_container_iter_s libcrun_container_iter_t;

LIBCRUN_عوامی صحیح libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **برآمد,
                                           libcrun_error_t *err);
LIBCRUN_عوامی libcrun_container_iter_t *libcrun_container_list_iter (libcrun_container_list_t *l);
LIBCRUN_عوامی bool libcrun_container_iter_اگلا (libcrun_container_iter_t *it, ثابت حرف **id);
LIBCRUN_عوامی خالی libcrun_container_iter_free (libcrun_container_iter_t *it);
LIBCRUN_عوامی خالی libcrun_container_list_free (libcrun_container_list_t *l);

قسمتعریف شمار
{
  LIBCRUN_CONTAINER_حالت_CREATING = 0,
  LIBCRUN_CONTAINER_حالت_CREATED,
  LIBCRUN_CONTAINER_حالت_RUNNING,
  LIBCRUN_CONTAINER_حالت_STOPPED,
  LIBCRUN_CONTAINER_حالت_PAUSED,
} libcrun_container_state_t;

ساخت libcrun_حالت_s;
قسمتعریف ساخت libcrun_حالت_s libcrun_حالت_t;

LIBCRUN_عوامی صحیح libcrun_container_حالت_بھرو (libcrun_context_t *context, ثابت حرف *id, libcrun_حالت_t **برآمد,
                                                  libcrun_error_t *err);
LIBCRUN_عوامی خالی libcrun_container_حالت_free (libcrun_حالت_t *st);
/* Resolve the live state of the container described by ST.  The state is
   probed on the first call and cached afterwards; STATE and RUNNING can be
   NULL.  */
LIBCRUN_عوامی صحیح libcrun_حالت_get_state (libcrun_حالت_t *st, libcrun_container_state_t *state, صحیح *running,
                                             libcrun_error_t *err);
LIBCRUN_عوامی ثابت حرف *libcrun_container_state_مقصد_string (libcrun_container_state_t state);
LIBCRUN_عوامی pid_t libcrun_حالت_get_pid (libcrun_حالت_t *st);
LIBCRUN_عوامی ثابت حرف *libcrun_حالت_get_bundle (libcrun_حالت_t *st);
LIBCRUN_عوامی ثابت حرف *libcrun_حالت_get_rootfs (libcrun_حالت_t *st);
LIBCRUN_عوامی ثابت حرف *libcrun_حالت_get_created (libcrun_حالت_t *st);
LIBCRUN_عوامی ثابت حرف *libcrun_حالت_get_owner (libcrun_حالت_t *st);
LIBCRUN_عوامی ثابت حرف *libcrun_حالت_get_scope (libcrun_حالت_t *st);
LIBCRUN_عوامی ثابت حرف *libcrun_حالت_get_external_descriptors (libcrun_حالت_t *st);
LIBCRUN_عوامی صحیح libcrun_container_get_state_string (libcrun_context_t *context, ثابت حرف *id, ثابت حرف **برآمد,
                                                       صحیح *running, libcrun_error_t *err);
LIBCRUN_عوامی صحیح libcrun_get_state_dir (حرف **برآمد, ثابت حرف *state_جڑ, ثابت حرف *id, libcrun_error_t *err);

صحیح libcrun_حالت_check_directories (ثابت حرف *state_جڑ, ثابت حرف *id, libcrun_error_t *err);
صحیح libcrun_حالت_create_exec_fifo (ثابت حرف *state_جڑ, ثابت حرف *id, libcrun_error_t *err);
صحیح libcrun_حالت_write_exec_fifo (ثابت حرف *state_جڑ, ثابت حرف *id, libcrun_error_t *err);
صحیح libcrun_حالت_has_read_exec_fifo (ثابت حرف *state_جڑ, ثابت حرف *id, libcrun_error_t *err);
صحیح libcrun_check_pid_valid (libcrun_container_حالت_t *حالت_2, libcrun_error_t *err);
صحیح get_تسلسل_directory (حرف **برآمد, ثابت حرف *state_جڑ, libcrun_error_t *err);
صحیح get_shared_خالی_خانے_directory_راستہ (حرف **برآمد, ثابت حرف *state_جڑ, libcrun_error_t *err);

ساکن اندرونی خالی
libcrun_free_container_listp (خالی *p)
{
  libcrun_container_list_t **l = (libcrun_container_list_t **) p;
  اگر (*l != تہی)
    libcrun_free_containers_list (*l);
}

#تعریف_کرو cleanup_container_حالت __attribute__ ((cleanup (libcrun_free_container_حالت)))
#تعریف_کرو cleanup_container_list __attribute__ ((cleanup (libcrun_free_container_listp)))

#اگر_ختم
