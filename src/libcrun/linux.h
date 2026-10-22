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
#ifndef LINUX_H
#define LINUX_H
#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ "error.h"
#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <argp.h>
#અન્તર્ભાવ <sys/syscall.h>
#અન્તર્ભાવ <ocispec/runtime_spec_schema_config_schema.h>
#અન્તર્ભાવ "container.h"
#અન્તર્ભાવ "status.h"

સંરચના device_s
{
  અચલ અક્ષર *path;
  અક્ષર *type;
  પૂર્ણાંક major;
  પૂર્ણાંક minor;
  પૂર્ણાંક mode;
  uid_t uid;
  gid_t gid;
};

સ્થિર અંતઃરેખ પૂર્ણાંક
syscall_clone (unsigned long flags, શૂન્ય *child_ઢગલો)
{
#જો defined __s390__ || defined __CRIS__
  પાછા (પૂર્ણાંક) syscall (__NR_clone, child_ઢગલો, flags);
#નહીંતર
  પાછા (પૂર્ણાંક) syscall (__NR_clone, flags, child_ઢગલો);
#endif
}

પ્રકારવ્યાખ્યા પૂર્ણાંક (*container_entrypoint_t) (શૂન્ય *args, અક્ષર *notify_socket, પૂર્ણાંક sync_socket, libcrun_error_t *err);

પ્રકારવ્યાખ્યા પૂર્ણાંક (*set_mounts_cb_t) (શૂન્ય *args, libcrun_error_t *err);

સંરચના libcrun_dirfd_s
{
  પૂર્ણાંક *dirfd;
  bool joined;
};

pid_t libcrun_run_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, શૂન્ય *args,
                                   પૂર્ણાંક *sync_socket_out, સંરચના libcrun_dirfd_s *dirfd, libcrun_error_t *err);
bool libcrun_container_delayed_pidns_fork (libcrun_container_t *container);
પૂર્ણાંક libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err);
પૂર્ણાંક get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, પૂર્ણાંક *notify_socket_out,
                   libcrun_error_t *err);
પૂર્ણાંક libcrun_set_mounts (સંરચના container_entrypoint_s *args, libcrun_container_t *container, અચલ અક્ષર *rootfs,
                        set_mounts_cb_t cb, શૂન્ય *cb_data, libcrun_error_t *err);
પૂર્ણાંક libcrun_finalize_mounts (સંરચના container_entrypoint_s *entrypoint_args, libcrun_container_t *container,
                             અચલ અક્ષર *rootfs, set_mounts_cb_t after_pivot_cb, શૂન્ય *cb_data, libcrun_error_t *err);
પૂર્ણાંક libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err);
પૂર્ણાંક libcrun_do_pivot_મૂળ (libcrun_container_t *container, bool no_pivot, અક્ષર **rootfs, libcrun_error_t *err);
પૂર્ણાંક libcrun_reopen_dev_null (libcrun_error_t *err);
પૂર્ણાંક libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);
પૂર્ણાંક libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                      પૂર્ણાંક no_new_privileges, bool no_exec, libcrun_error_t *err);
પૂર્ણાંક libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **rlimits, size_t len,
                         libcrun_error_t *err);
પૂર્ણાંક libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
પૂર્ણાંક libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
પૂર્ણાંક libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err);
પૂર્ણાંક libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err);
પૂર્ણાંક libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err);
પૂર્ણાંક libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err);
પૂર્ણાંક libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err);
પૂર્ણાંક libcrun_join_process (libcrun_context_t *context, libcrun_container_t *container, pid_t pid_to_join,
                          libcrun_container_status_t *status, અચલ અક્ષર *cgroup, પૂર્ણાંક detach,
                          runtime_spec_schema_config_schema_process *process, પૂર્ણાંક *terminal_fd, libcrun_error_t *err);
પૂર્ણાંક libcrun_linux_container_update (libcrun_container_status_t *status,
                                    અચલ અક્ષર *state_મૂળ,
                                    runtime_spec_schema_config_linux_resources *resources,
                                    libcrun_error_t *err);
પૂર્ણાંક libcrun_create_keyring (libcrun_container_t *container, અચલ અક્ષર *name, અચલ અક્ષર *label, libcrun_error_t *err);
પૂર્ણાંક libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err);
પૂર્ણાંક libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err);
પૂર્ણાંક libcrun_container_do_bind_mount (libcrun_container_t *container, અક્ષર *mount_source, અક્ષર *mount_destination,
                                     અક્ષર **mount_options, size_t mount_options_len, libcrun_error_t *err);
પૂર્ણાંક libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err);
પૂર્ણાંક libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err);
પૂર્ણાંક libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err);

પૂર્ણાંક libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                        libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

પૂર્ણાંક libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                     libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

પૂર્ણાંક libcrun_find_namespace (અચલ અક્ષર *name);
અક્ષર *libcrun_get_external_descriptors (libcrun_container_t *container);
પૂર્ણાંક libcrun_container_setgroups (libcrun_container_t *container,
                                 runtime_spec_schema_config_schema_process *process,
                                 libcrun_error_t *err);
પૂર્ણાંક libcrun_kill_linux (libcrun_container_status_t *status, પૂર્ણાંક સંકેત, libcrun_error_t *err);
પૂર્ણાંક libcrun_create_final_userns (libcrun_container_t *container, libcrun_error_t *err);
પૂર્ણાંક libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err);

પૂર્ણાંક libcrun_create_dev (libcrun_container_t *container, પૂર્ણાંક devfd,
                        પૂર્ણાંક srcfd, સંરચના device_s *device, bool binds,
                        bool ensure_parent_dir, libcrun_error_t *err);

પૂર્ણાંક parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, અક્ષર *option, અક્ષર **out,
                                 size_t *len, libcrun_error_t *err);

ગણના
{
  LIBCRUN_INTELRDT_CREATE = (1 << 0),
  LIBCRUN_INTELRDT_UPDATE = (1 << 1),
  LIBCRUN_INTELRDT_MOVE = (1 << 2),
};

#define LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE (LIBCRUN_INTELRDT_CREATE | LIBCRUN_INTELRDT_UPDATE | LIBCRUN_INTELRDT_MOVE)

પૂર્ણાંક libcrun_apply_intelrdt (અચલ અક્ષર *ctr_name, libcrun_container_t *container, pid_t pid, પૂર્ણાંક actions, libcrun_error_t *err);

પૂર્ણાંક libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);

પૂર્ણાંક libcrun_destroy_intelrdt (અચલ અક્ષર *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err);

પૂર્ણાંક libcrun_update_intel_rdt (અચલ અક્ષર *ctr_name, libcrun_container_t *container, અચલ અક્ષર *l3_cache_schema, અચલ અક્ષર *mem_bw_schema, અક્ષર *અચલ *schemata, libcrun_error_t *err);

પૂર્ણાંક libcrun_safe_chdir (અચલ અક્ષર *path, libcrun_error_t *err);

પૂર્ણાંક get_bind_mount (પૂર્ણાંક dirfd, અચલ અક્ષર *src, bool recursive, bool rdonly, bool nofollow, unsigned long propagation, libcrun_error_t *err);

bool is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow);

પૂર્ણાંક libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

પૂર્ણાંક libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

સંરચના libcrun_mount_flags_s
{
  અક્ષર *target;
  unsigned long flags;
};

પૂર્ણાંક libcrun_restrict_mounts_in_container (pid_t pid, સંરચના libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err);

#endif
