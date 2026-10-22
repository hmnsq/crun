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
#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ "error.h"
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <argp.h>
#ಅನ್ತರ್ಭಾವ <sys/syscall.h>
#ಅನ್ತರ್ಭಾವ <ocispec/runtime_spec_schema_config_schema.h>
#ಅನ್ತರ್ಭಾವ "container.h"
#ಅನ್ತರ್ಭಾವ "status.h"

ಸಂರಚನಾ device_s
{
  ಧ್ರುವ ವರ್ಣ *path;
  ವರ್ಣ *type;
  ಪೂರ್ಣಾಙ್ಕ major;
  ಪೂರ್ಣಾಙ್ಕ minor;
  ಪೂರ್ಣಾಙ್ಕ mode;
  uid_t uid;
  gid_t gid;
};

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
syscall_clone (unsigned long flags, ಶೂನ್ಯ *child_ರಾಶಿ)
{
#ಯದಿ defined __s390__ || defined __CRIS__
  ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) syscall (__NR_clone, child_ರಾಶಿ, flags);
#ಅನ್ಯಥಾ
  ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) syscall (__NR_clone, flags, child_ರಾಶಿ);
#endif
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ (*container_entrypoint_t) (ಶೂನ್ಯ *args, ವರ್ಣ *notify_socket, ಪೂರ್ಣಾಙ್ಕ sync_socket, libcrun_error_t *err);

ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ (*set_mounts_cb_t) (ಶೂನ್ಯ *args, libcrun_error_t *err);

ಸಂರಚನಾ libcrun_dirfd_s
{
  ಪೂರ್ಣಾಙ್ಕ *dirfd;
  bool joined;
};

pid_t libcrun_run_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, ಶೂನ್ಯ *args,
                                   ಪೂರ್ಣಾಙ್ಕ *sync_socket_out, ಸಂರಚನಾ libcrun_dirfd_s *dirfd, libcrun_error_t *err);
bool libcrun_container_delayed_pidns_fork (libcrun_container_t *container);
ಪೂರ್ಣಾಙ್ಕ libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, ಪೂರ್ಣಾಙ್ಕ *notify_socket_out,
                   libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_set_mounts (ಸಂರಚನಾ container_entrypoint_s *args, libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *rootfs,
                        set_mounts_cb_t cb, ಶೂನ್ಯ *cb_data, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_finalize_mounts (ಸಂರಚನಾ container_entrypoint_s *entrypoint_args, libcrun_container_t *container,
                             ಧ್ರುವ ವರ್ಣ *rootfs, set_mounts_cb_t after_pivot_cb, ಶೂನ್ಯ *cb_data, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_do_pivot_ಬೇರು (libcrun_container_t *container, bool no_pivot, ವರ್ಣ **rootfs, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_reopen_dev_null (libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                      ಪೂರ್ಣಾಙ್ಕ no_new_privileges, bool no_exec, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **rlimits, size_t len,
                         libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_join_process (libcrun_context_t *context, libcrun_container_t *container, pid_t pid_to_join,
                          libcrun_container_status_t *status, ಧ್ರುವ ವರ್ಣ *cgroup, ಪೂರ್ಣಾಙ್ಕ detach,
                          runtime_spec_schema_config_schema_process *process, ಪೂರ್ಣಾಙ್ಕ *terminal_fd, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_linux_container_update (libcrun_container_status_t *status,
                                    ಧ್ರುವ ವರ್ಣ *state_ಬೇರು,
                                    runtime_spec_schema_config_linux_resources *resources,
                                    libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_create_keyring (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *label, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_container_do_bind_mount (libcrun_container_t *container, ವರ್ಣ *mount_source, ವರ್ಣ *mount_destination,
                                     ವರ್ಣ **mount_options, size_t mount_options_len, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                        libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                     libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_find_namespace (ಧ್ರುವ ವರ್ಣ *name);
ವರ್ಣ *libcrun_get_external_descriptors (libcrun_container_t *container);
ಪೂರ್ಣಾಙ್ಕ libcrun_container_setgroups (libcrun_container_t *container,
                                 runtime_spec_schema_config_schema_process *process,
                                 libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_kill_linux (libcrun_container_status_t *status, ಪೂರ್ಣಾಙ್ಕ ಸಙ್ಕೇತ, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_create_final_userns (libcrun_container_t *container, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_create_dev (libcrun_container_t *container, ಪೂರ್ಣಾಙ್ಕ devfd,
                        ಪೂರ್ಣಾಙ್ಕ srcfd, ಸಂರಚನಾ device_s *device, bool binds,
                        bool ensure_parent_dir, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, ವರ್ಣ *option, ವರ್ಣ **out,
                                 size_t *len, libcrun_error_t *err);

ಗಣನಾ
{
  LIBCRUN_INTELRDT_CREATE = (1 << 0),
  LIBCRUN_INTELRDT_UPDATE = (1 << 1),
  LIBCRUN_INTELRDT_MOVE = (1 << 2),
};

#define LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE (LIBCRUN_INTELRDT_CREATE | LIBCRUN_INTELRDT_UPDATE | LIBCRUN_INTELRDT_MOVE)

ಪೂರ್ಣಾಙ್ಕ libcrun_apply_intelrdt (ಧ್ರುವ ವರ್ಣ *ctr_name, libcrun_container_t *container, pid_t pid, ಪೂರ್ಣಾಙ್ಕ actions, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_destroy_intelrdt (ಧ್ರುವ ವರ್ಣ *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_update_intel_rdt (ಧ್ರುವ ವರ್ಣ *ctr_name, libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *l3_cache_schema, ಧ್ರುವ ವರ್ಣ *mem_bw_schema, ವರ್ಣ *ಧ್ರುವ *schemata, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_safe_chdir (ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ get_bind_mount (ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *src, bool recursive, bool rdonly, bool nofollow, unsigned long propagation, libcrun_error_t *err);

bool is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow);

ಪೂರ್ಣಾಙ್ಕ libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

ಸಂರಚನಾ libcrun_mount_flags_s
{
  ವರ್ಣ *target;
  unsigned long flags;
};

ಪೂರ್ಣಾಙ್ಕ libcrun_restrict_mounts_in_container (pid_t pid, ಸಂರಚನಾ libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err);

#endif
