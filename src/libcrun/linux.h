/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019, 2021 Giuseppe Scrivano <giuseppe@scrivano.org>
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
#నిర్వచితం_కాకపోతే LINUX_H
#నిర్వచించు LINUX_H
#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ "error.h"
#అన్తర్భావ <errno.h>
#అన్తర్భావ <argp.h>
#అన్తర్భావ <sys/syscall.h>
#అన్తర్భావ <ocispec/runtime_spec_schema_config_schema.h>
#అన్తర్భావ "container.h"
#అన్తర్భావ "status.h"

నిర్మాణం device_s
{
  స్థిరరాశి అక్షరం *మార్గం;
  అక్షరం *type;
  పూర్ణాంకం major;
  పూర్ణాంకం minor;
  పూర్ణాంకం బహుళకం;
  uid_t uid;
  gid_t gid;
};

స్థిర అంతఃవరుస పూర్ణాంకం
syscall_clone (గుర్తులేని దీర్ఘ flags, శూన్య *బిడ్డ_దొంతర)
{
#ఒకవేళ defined __s390__ || defined __CRIS__
  తిరిగి (పూర్ణాంకం) syscall (__NR_clone, బిడ్డ_దొంతర, flags);
#లేకపోతే
  తిరిగి (పూర్ణాంకం) syscall (__NR_clone, flags, బిడ్డ_దొంతర);
#అయితే_ముగింపు
}

రకనిర్వచనం పూర్ణాంకం (*container_entrypoint_t) (శూన్య *వాదనలు, అక్షరం *notify_socket, పూర్ణాంకం sync_socket, libcrun_error_t *err);

రకనిర్వచనం పూర్ణాంకం (*సమితి_mounts_cb_t) (శూన్య *వాదనలు, libcrun_error_t *err);

నిర్మాణం libcrun_dirfd_s
{
  పూర్ణాంకం *dirfd;
  bool joined;
};

pid_t libcrun_నిరంతరం_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, శూన్య *వాదనలు,
                                   పూర్ణాంకం *sync_socket_బహిర్గతం, నిర్మాణం libcrun_dirfd_s *dirfd, libcrun_error_t *err);
bool libcrun_container_delayed_pidns_fork (libcrun_container_t *container);
పూర్ణాంకం libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err);
పూర్ణాంకం get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, పూర్ణాంకం *notify_socket_బహిర్గతం,
                   libcrun_error_t *err);
పూర్ణాంకం libcrun_సమితి_mounts (నిర్మాణం container_entrypoint_s *వాదనలు, libcrun_container_t *container, స్థిరరాశి అక్షరం *rootfs,
                        సమితి_mounts_cb_t cb, శూన్య *cb_దత్తాంశం, libcrun_error_t *err);
పూర్ణాంకం libcrun_finalize_mounts (నిర్మాణం container_entrypoint_s *entrypoint_వాదనలు, libcrun_container_t *container,
                             స్థిరరాశి అక్షరం *rootfs, సమితి_mounts_cb_t after_ఇరుసు_cb, శూన్య *cb_దత్తాంశం, libcrun_error_t *err);
పూర్ణాంకం libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err);
పూర్ణాంకం libcrun_do_ఇరుసు_మూలం (libcrun_container_t *container, bool no_ఇరుసు, అక్షరం **rootfs, libcrun_error_t *err);
పూర్ణాంకం libcrun_reopen_dev_null (libcrun_error_t *err);
పూర్ణాంకం libcrun_సమితి_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);
పూర్ణాంకం libcrun_సమితి_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                      పూర్ణాంకం no_new_privileges, bool no_exec, libcrun_error_t *err);
పూర్ణాంకం libcrun_సమితి_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **rlimits, size_t len,
                         libcrun_error_t *err);
పూర్ణాంకం libcrun_సమితి_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
పూర్ణాంకం libcrun_సమితి_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
పూర్ణాంకం libcrun_సమితి_hostname (libcrun_container_t *container, libcrun_error_t *err);
పూర్ణాంకం libcrun_సమితి_domainname (libcrun_container_t *container, libcrun_error_t *err);
పూర్ణాంకం libcrun_సమితి_oom (libcrun_container_t *container, libcrun_error_t *err);
పూర్ణాంకం libcrun_సమితి_sysctl (libcrun_container_t *container, libcrun_error_t *err);
పూర్ణాంకం libcrun_సమితి_terminal (libcrun_container_t *container, libcrun_error_t *err);
పూర్ణాంకం libcrun_join_process (libcrun_context_t *context, libcrun_container_t *container, pid_t pid_గమ్యం_join,
                          libcrun_container_స్థితి_t *స్థితి_2, స్థిరరాశి అక్షరం *cgroup, పూర్ణాంకం detach,
                          runtime_spec_schema_config_schema_process *process, పూర్ణాంకం *terminal_fd, libcrun_error_t *err);
పూర్ణాంకం libcrun_linux_container_update (libcrun_container_స్థితి_t *స్థితి_2,
                                    స్థిరరాశి అక్షరం *state_మూలం,
                                    runtime_spec_schema_config_linux_resources *resources,
                                    libcrun_error_t *err);
పూర్ణాంకం libcrun_create_keyring (libcrun_container_t *container, స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *label, libcrun_error_t *err);
పూర్ణాంకం libcrun_container_pause_linux (libcrun_container_స్థితి_t *స్థితి_2, libcrun_error_t *err);
పూర్ణాంకం libcrun_container_unpause_linux (libcrun_container_స్థితి_t *స్థితి_2, libcrun_error_t *err);
పూర్ణాంకం libcrun_container_do_bind_mount (libcrun_container_t *container, అక్షరం *mount_ఆధారం, అక్షరం *mount_destination,
                                     అక్షరం **mount_options, size_t mount_options_len, libcrun_error_t *err);
పూర్ణాంకం libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err);
పూర్ణాంకం libcrun_సమితి_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err);
పూర్ణాంకం libcrun_configure_జాలం (libcrun_container_t *container, libcrun_error_t *err);

పూర్ణాంకం libcrun_container_checkpoint_linux (libcrun_container_స్థితి_t *స్థితి_2, libcrun_container_t *container,
                                        libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

పూర్ణాంకం libcrun_container_restore_linux (libcrun_container_స్థితి_t *స్థితి_2, libcrun_container_t *container,
                                     libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

పూర్ణాంకం libcrun_కనుగొను_namespace (స్థిరరాశి అక్షరం *name);
అక్షరం *libcrun_get_external_descriptors (libcrun_container_t *container);
పూర్ణాంకం libcrun_container_setgroups (libcrun_container_t *container,
                                 runtime_spec_schema_config_schema_process *process,
                                 libcrun_error_t *err);
పూర్ణాంకం libcrun_kill_linux (libcrun_container_స్థితి_t *స్థితి_2, పూర్ణాంకం సంకేతం, libcrun_error_t *err);
పూర్ణాంకం libcrun_create_final_userns (libcrun_container_t *container, libcrun_error_t *err);
పూర్ణాంకం libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err);

పూర్ణాంకం libcrun_create_dev (libcrun_container_t *container, పూర్ణాంకం devfd,
                        పూర్ణాంకం srcfd, నిర్మాణం device_s *device, bool binds,
                        bool ensure_మాతృ_dir, libcrun_error_t *err);

పూర్ణాంకం parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, అక్షరం *option, అక్షరం **బహిర్గతం,
                                 size_t *len, libcrun_error_t *err);

గణన
{
  LIBCRUN_INTELRDT_CREATE = (1 << 0),
  LIBCRUN_INTELRDT_UPDATE = (1 << 1),
  LIBCRUN_INTELRDT_MOVE = (1 << 2),
};

#నిర్వచించు LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE (LIBCRUN_INTELRDT_CREATE | LIBCRUN_INTELRDT_UPDATE | LIBCRUN_INTELRDT_MOVE)

పూర్ణాంకం libcrun_apply_intelrdt (స్థిరరాశి అక్షరం *ctr_name, libcrun_container_t *container, pid_t pid, పూర్ణాంకం actions, libcrun_error_t *err);

పూర్ణాంకం libcrun_move_జాలం_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);

పూర్ణాంకం libcrun_destroy_intelrdt (స్థిరరాశి అక్షరం *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err);

పూర్ణాంకం libcrun_update_intel_rdt (స్థిరరాశి అక్షరం *ctr_name, libcrun_container_t *container, స్థిరరాశి అక్షరం *l3_cache_schema, స్థిరరాశి అక్షరం *mem_bw_schema, అక్షరం *స్థిరరాశి *schemata, libcrun_error_t *err);

పూర్ణాంకం libcrun_సురక్షిత_chdir (స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err);

పూర్ణాంకం get_bind_mount (పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *src, bool recursive, bool rdonly, bool nofollow, గుర్తులేని దీర్ఘ ప్రసరణ, libcrun_error_t *err);

bool is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow);

పూర్ణాంకం libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_స్థితి_t *స్థితి_2, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

పూర్ణాంకం libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_స్థితి_t *స్థితి_2, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

నిర్మాణం libcrun_mount_flags_s
{
  అక్షరం *లక్ష్యం;
  గుర్తులేని దీర్ఘ flags;
};

పూర్ణాంకం libcrun_restrict_mounts_లోపల_container (pid_t pid, నిర్మాణం libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err);

#అయితే_ముగింపు
