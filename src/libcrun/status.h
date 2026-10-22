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

#నిర్వచితం_కాకపోతే స్థితి_H
#నిర్వచించు స్థితి_H

#అన్తర్భావ <config.h>
#అన్తర్భావ <ocispec/runtime_spec_schema_config_schema.h>
#అన్తర్భావ "error.h"
#అన్తర్భావ "container.h"

నిర్మాణం libcrun_container_list_s
{
  నిర్మాణం libcrun_container_list_s *తదుపరి;
  అక్షరం *name;
};
రకనిర్వచనం నిర్మాణం libcrun_container_list_s libcrun_container_list_t;

నిర్మాణం libcrun_container_స్థితి_s
{
  pid_t pid;
  గుర్తులేని దీర్ఘ దీర్ఘ process_ప్రారంభం_సమయం;
  అక్షరం *bundle;
  అక్షరం *rootfs;
  అక్షరం *cgroup_మార్గం;
  అక్షరం *scope;
  పూర్ణాంకం systemd_cgroup;
  అక్షరం *created;
  పూర్ణాంకం detached;
  పూర్ణాంకం no_new_keyring;
  అక్షరం *external_descriptors;
  అక్షరం *owner;
};
రకనిర్వచనం నిర్మాణం libcrun_container_స్థితి_s libcrun_container_స్థితి_t;

LIBCRUN_ప్రజా శూన్య libcrun_free_container_స్థితి (libcrun_container_స్థితి_t *స్థితి_2);
LIBCRUN_ప్రజా పూర్ణాంకం libcrun_write_container_స్థితి (స్థిరరాశి అక్షరం *state_మూలం, స్థిరరాశి అక్షరం *id,
                                                   libcrun_container_స్థితి_t *స్థితి_2, libcrun_error_t *err);
LIBCRUN_ప్రజా పూర్ణాంకం libcrun_read_container_స్థితి (libcrun_container_స్థితి_t *స్థితి_2, స్థిరరాశి అక్షరం *state_మూలం,
                                                  స్థిరరాశి అక్షరం *id, libcrun_error_t *err);
LIBCRUN_ప్రజా శూన్య libcrun_free_containers_list (libcrun_container_list_t *list);
LIBCRUN_ప్రజా పూర్ణాంకం libcrun_is_container_running (libcrun_container_స్థితి_t *స్థితి_2, libcrun_error_t *err);
LIBCRUN_ప్రజా పూర్ణాంకం libcrun_get_state_directory (అక్షరం **బహిర్గతం, స్థిరరాశి అక్షరం *state_మూలం, స్థిరరాశి అక్షరం *id, libcrun_error_t *err);
LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_delete_స్థితి (స్థిరరాశి అక్షరం *state_మూలం, స్థిరరాశి అక్షరం *id, libcrun_error_t *err);
LIBCRUN_ప్రజా పూర్ణాంకం libcrun_get_containers_list (libcrun_container_list_t **ret, స్థిరరాశి అక్షరం *state_మూలం,
                                                libcrun_error_t *err);

నిర్మాణం libcrun_container_iter_s
{
  libcrun_container_list_t *cur;
};
రకనిర్వచనం నిర్మాణం libcrun_container_iter_s libcrun_container_iter_t;

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_list (libcrun_context_t *context, libcrun_container_list_t **బహిర్గతం,
                                           libcrun_error_t *err);
LIBCRUN_ప్రజా libcrun_container_iter_t *libcrun_container_list_iter (libcrun_container_list_t *l);
LIBCRUN_ప్రజా bool libcrun_container_iter_తదుపరి (libcrun_container_iter_t *it, స్థిరరాశి అక్షరం **id);
LIBCRUN_ప్రజా శూన్య libcrun_container_iter_free (libcrun_container_iter_t *it);
LIBCRUN_ప్రజా శూన్య libcrun_container_list_free (libcrun_container_list_t *l);

రకనిర్వచనం గణన
{
  LIBCRUN_CONTAINER_స్థితి_CREATING = 0,
  LIBCRUN_CONTAINER_స్థితి_CREATED,
  LIBCRUN_CONTAINER_స్థితి_RUNNING,
  LIBCRUN_CONTAINER_స్థితి_STOPPED,
  LIBCRUN_CONTAINER_స్థితి_PAUSED,
} libcrun_container_state_t;

నిర్మాణం libcrun_స్థితి_s;
రకనిర్వచనం నిర్మాణం libcrun_స్థితి_s libcrun_స్థితి_t;

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_స్థితి_ఎక్కించు (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, libcrun_స్థితి_t **బహిర్గతం,
                                                  libcrun_error_t *err);
LIBCRUN_ప్రజా శూన్య libcrun_container_స్థితి_free (libcrun_స్థితి_t *st);
/* Resolve the live state of the container described by ST.  The state is
   probed on the first call and cached afterwards; STATE and RUNNING can be
   NULL.  */
LIBCRUN_ప్రజా పూర్ణాంకం libcrun_స్థితి_get_state (libcrun_స్థితి_t *st, libcrun_container_state_t *state, పూర్ణాంకం *running,
                                             libcrun_error_t *err);
LIBCRUN_ప్రజా స్థిరరాశి అక్షరం *libcrun_container_state_గమ్యం_string (libcrun_container_state_t state);
LIBCRUN_ప్రజా pid_t libcrun_స్థితి_get_pid (libcrun_స్థితి_t *st);
LIBCRUN_ప్రజా స్థిరరాశి అక్షరం *libcrun_స్థితి_get_bundle (libcrun_స్థితి_t *st);
LIBCRUN_ప్రజా స్థిరరాశి అక్షరం *libcrun_స్థితి_get_rootfs (libcrun_స్థితి_t *st);
LIBCRUN_ప్రజా స్థిరరాశి అక్షరం *libcrun_స్థితి_get_created (libcrun_స్థితి_t *st);
LIBCRUN_ప్రజా స్థిరరాశి అక్షరం *libcrun_స్థితి_get_owner (libcrun_స్థితి_t *st);
LIBCRUN_ప్రజా స్థిరరాశి అక్షరం *libcrun_స్థితి_get_scope (libcrun_స్థితి_t *st);
LIBCRUN_ప్రజా స్థిరరాశి అక్షరం *libcrun_స్థితి_get_external_descriptors (libcrun_స్థితి_t *st);
LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_get_state_string (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం **బహిర్గతం,
                                                       పూర్ణాంకం *running, libcrun_error_t *err);
LIBCRUN_ప్రజా పూర్ణాంకం libcrun_get_state_dir (అక్షరం **బహిర్గతం, స్థిరరాశి అక్షరం *state_మూలం, స్థిరరాశి అక్షరం *id, libcrun_error_t *err);

పూర్ణాంకం libcrun_స్థితి_check_directories (స్థిరరాశి అక్షరం *state_మూలం, స్థిరరాశి అక్షరం *id, libcrun_error_t *err);
పూర్ణాంకం libcrun_స్థితి_create_exec_fifo (స్థిరరాశి అక్షరం *state_మూలం, స్థిరరాశి అక్షరం *id, libcrun_error_t *err);
పూర్ణాంకం libcrun_స్థితి_write_exec_fifo (స్థిరరాశి అక్షరం *state_మూలం, స్థిరరాశి అక్షరం *id, libcrun_error_t *err);
పూర్ణాంకం libcrun_స్థితి_has_read_exec_fifo (స్థిరరాశి అక్షరం *state_మూలం, స్థిరరాశి అక్షరం *id, libcrun_error_t *err);
పూర్ణాంకం libcrun_check_pid_valid (libcrun_container_స్థితి_t *స్థితి_2, libcrun_error_t *err);
పూర్ణాంకం get_నిరంతరం_directory (అక్షరం **బహిర్గతం, స్థిరరాశి అక్షరం *state_మూలం, libcrun_error_t *err);
పూర్ణాంకం get_shared_ఖాళీలు_directory_మార్గం (అక్షరం **బహిర్గతం, స్థిరరాశి అక్షరం *state_మూలం, libcrun_error_t *err);

స్థిర అంతఃవరుస శూన్య
libcrun_free_container_listp (శూన్య *p)
{
  libcrun_container_list_t **l = (libcrun_container_list_t **) p;
  ఒకవేళ (*l != ఖాళీ)
    libcrun_free_containers_list (*l);
}

#నిర్వచించు cleanup_container_స్థితి __attribute__ ((cleanup (libcrun_free_container_స్థితి)))
#నిర్వచించు cleanup_container_list __attribute__ ((cleanup (libcrun_free_container_listp)))

#అయితే_ముగింపు
