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
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LINUX_H
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_H
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

ರಚನೆ device_s
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ;
  ಅಕ್ಷರ *type;
  ಪೂರ್ಣಾಂಕ major;
  ಪೂರ್ಣಾಂಕ minor;
  ಪೂರ್ಣಾಂಕ ಬಹುಲಕ;
  uid_t uid;
  gid_t gid;
};

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
syscall_clone (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags, ಶೂನ್ಯ *ಮಗು_ರಾಶಿ)
{
#ಆದರೆ defined __s390__ || defined __CRIS__
  ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) syscall (__NR_clone, ಮಗು_ರಾಶಿ, flags);
#ಇಲ್ಲದಿದ್ದರೆ
  ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) syscall (__NR_clone, flags, ಮಗು_ರಾಶಿ);
#ಆದರೆ_ಅಂತ್ಯ
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*container_entrypoint_t) (ಶೂನ್ಯ *ವಾದಗಳು, ಅಕ್ಷರ *notify_socket, ಪೂರ್ಣಾಂಕ sync_socket, libcrun_error_t *err);

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*ಗಣ_mounts_cb_t) (ಶೂನ್ಯ *ವಾದಗಳು, libcrun_error_t *err);

ರಚನೆ libcrun_dirfd_s
{
  ಪೂರ್ಣಾಂಕ *dirfd;
  bool joined;
};

pid_t libcrun_ಸತತ_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, ಶೂನ್ಯ *ವಾದಗಳು,
                                   ಪೂರ್ಣಾಂಕ *sync_socket_ಹೊರಹೋಗು, ರಚನೆ libcrun_dirfd_s *dirfd, libcrun_error_t *err);
bool libcrun_container_delayed_pidns_fork (libcrun_container_t *container);
ಪೂರ್ಣಾಂಕ libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, ಪೂರ್ಣಾಂಕ *notify_socket_ಹೊರಹೋಗು,
                   libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_ಗಣ_mounts (ರಚನೆ container_entrypoint_s *ವಾದಗಳು, libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs,
                        ಗಣ_mounts_cb_t cb, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_finalize_mounts (ರಚನೆ container_entrypoint_s *entrypoint_ವಾದಗಳು, libcrun_container_t *container,
                             ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs, ಗಣ_mounts_cb_t after_ಅಚ್ಚು_cb, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_do_ಅಚ್ಚು_ಬೇರು (libcrun_container_t *container, bool no_ಅಚ್ಚು, ಅಕ್ಷರ **rootfs, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_reopen_dev_null (libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_ಗಣ_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_ಗಣ_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                      ಪೂರ್ಣಾಂಕ no_new_privileges, bool no_exec, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_ಗಣ_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **rlimits, size_t len,
                         libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_ಗಣ_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_ಗಣ_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_ಗಣ_hostname (libcrun_container_t *container, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_ಗಣ_domainname (libcrun_container_t *container, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_ಗಣ_oom (libcrun_container_t *container, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_ಗಣ_sysctl (libcrun_container_t *container, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_ಗಣ_terminal (libcrun_container_t *container, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_join_process (libcrun_context_t *context, libcrun_container_t *container, pid_t pid_ಗಮ್ಯ_join,
                          libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cgroup, ಪೂರ್ಣಾಂಕ detach,
                          runtime_spec_schema_config_schema_process *process, ಪೂರ್ಣಾಂಕ *terminal_fd, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_linux_container_update (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2,
                                    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು,
                                    runtime_spec_schema_config_linux_resources *resources,
                                    libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_create_keyring (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *label, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_container_pause_linux (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_container_unpause_linux (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_container_do_bind_mount (libcrun_container_t *container, ಅಕ್ಷರ *mount_ಮೂಲ, ಅಕ್ಷರ *mount_destination,
                                     ಅಕ್ಷರ **mount_options, size_t mount_options_len, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_ಗಣ_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_configure_ಜಾಲ (libcrun_container_t *container, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_container_checkpoint_linux (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, libcrun_container_t *container,
                                        libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_container_restore_linux (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, libcrun_container_t *container,
                                     libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_ಕಂಡುಹಿಡಿ_namespace (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name);
ಅಕ್ಷರ *libcrun_get_external_descriptors (libcrun_container_t *container);
ಪೂರ್ಣಾಂಕ libcrun_container_setgroups (libcrun_container_t *container,
                                 runtime_spec_schema_config_schema_process *process,
                                 libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_kill_linux (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, ಪೂರ್ಣಾಂಕ ಸಂಕೇತ, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_create_final_userns (libcrun_container_t *container, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_create_dev (libcrun_container_t *container, ಪೂರ್ಣಾಂಕ devfd,
                        ಪೂರ್ಣಾಂಕ srcfd, ರಚನೆ device_s *device, bool binds,
                        bool ensure_ಪೋಷಕ_dir, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, ಅಕ್ಷರ *option, ಅಕ್ಷರ **ಹೊರಹೋಗು,
                                 size_t *len, libcrun_error_t *err);

ಎಣಿಕೆ
{
  LIBCRUN_INTELRDT_CREATE = (1 << 0),
  LIBCRUN_INTELRDT_UPDATE = (1 << 1),
  LIBCRUN_INTELRDT_MOVE = (1 << 2),
};

#ವ್ಯಾಖ್ಯಾನಿಸು LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE (LIBCRUN_INTELRDT_CREATE | LIBCRUN_INTELRDT_UPDATE | LIBCRUN_INTELRDT_MOVE)

ಪೂರ್ಣಾಂಕ libcrun_apply_intelrdt (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ctr_name, libcrun_container_t *container, pid_t pid, ಪೂರ್ಣಾಂಕ actions, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_move_ಜಾಲ_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_destroy_intelrdt (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_update_intel_rdt (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ctr_name, libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *l3_cache_schema, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *mem_bw_schema, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *schemata, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_ಸುರಕ್ಷಿತ_chdir (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ get_bind_mount (ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *src, bool recursive, bool rdonly, bool nofollow, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪ್ರಸರಣ, libcrun_error_t *err);

bool is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow);

ಪೂರ್ಣಾಂಕ libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

ರಚನೆ libcrun_mount_flags_s
{
  ಅಕ್ಷರ *ಗುರಿ;
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;
};

ಪೂರ್ಣಾಂಕ libcrun_restrict_mounts_ಒಳ_container (pid_t pid, ರಚನೆ libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err);

#ಆದರೆ_ಅಂತ್ಯ
