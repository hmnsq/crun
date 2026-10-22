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
#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ "error.h"
#অন্তর্ভাৱ <errno.h>
#অন্তর্ভাৱ <argp.h>
#অন্তর্ভাৱ <sys/syscall.h>
#অন্তর্ভাৱ <ocispec/runtime_spec_schema_config_schema.h>
#অন্তর্ভাৱ "container.h"
#অন্তর্ভাৱ "status.h"

সংরচনা device_s
{
  ধ্রুৱ ৱর্ণ *path;
  ৱর্ণ *type;
  পূর্ণাঙ্ক major;
  পূর্ণাঙ্ক minor;
  পূর্ণাঙ্ক mode;
  uid_t uid;
  gid_t gid;
};

স্থির অন্তঃস্থ পূর্ণাঙ্ক
syscall_clone (unsigned long flags, শূন্য *child_স্তূপ)
{
#যদি defined __s390__ || defined __CRIS__
  ঘূৰাই (পূর্ণাঙ্ক) syscall (__NR_clone, child_স্তূপ, flags);
#অন্যথা
  ঘূৰাই (পূর্ণাঙ্ক) syscall (__NR_clone, flags, child_স্তূপ);
#endif
}

প্রকারনির্দেশ পূর্ণাঙ্ক (*container_entrypoint_t) (শূন্য *args, ৱর্ণ *notify_socket, পূর্ণাঙ্ক sync_socket, libcrun_error_t *err);

প্রকারনির্দেশ পূর্ণাঙ্ক (*set_mounts_cb_t) (শূন্য *args, libcrun_error_t *err);

সংরচনা libcrun_dirfd_s
{
  পূর্ণাঙ্ক *dirfd;
  bool joined;
};

pid_t libcrun_run_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, শূন্য *args,
                                   পূর্ণাঙ্ক *sync_socket_out, সংরচনা libcrun_dirfd_s *dirfd, libcrun_error_t *err);
bool libcrun_container_delayed_pidns_fork (libcrun_container_t *container);
পূর্ণাঙ্ক libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err);
পূর্ণাঙ্ক get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, পূর্ণাঙ্ক *notify_socket_out,
                   libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_set_mounts (সংরচনা container_entrypoint_s *args, libcrun_container_t *container, ধ্রুৱ ৱর্ণ *rootfs,
                        set_mounts_cb_t cb, শূন্য *cb_data, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_finalize_mounts (সংরচনা container_entrypoint_s *entrypoint_args, libcrun_container_t *container,
                             ধ্রুৱ ৱর্ণ *rootfs, set_mounts_cb_t after_pivot_cb, শূন্য *cb_data, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_do_pivot_মূল (libcrun_container_t *container, bool no_pivot, ৱর্ণ **rootfs, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_reopen_dev_null (libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                      পূর্ণাঙ্ক no_new_privileges, bool no_exec, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **rlimits, size_t len,
                         libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_join_process (libcrun_context_t *context, libcrun_container_t *container, pid_t pid_to_join,
                          libcrun_container_status_t *status, ধ্রুৱ ৱর্ণ *cgroup, পূর্ণাঙ্ক detach,
                          runtime_spec_schema_config_schema_process *process, পূর্ণাঙ্ক *terminal_fd, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_linux_container_update (libcrun_container_status_t *status,
                                    ধ্রুৱ ৱর্ণ *state_মূল,
                                    runtime_spec_schema_config_linux_resources *resources,
                                    libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_create_keyring (libcrun_container_t *container, ধ্রুৱ ৱর্ণ *name, ধ্রুৱ ৱর্ণ *label, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_container_do_bind_mount (libcrun_container_t *container, ৱর্ণ *mount_source, ৱর্ণ *mount_destination,
                                     ৱর্ণ **mount_options, size_t mount_options_len, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                        libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                     libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_find_namespace (ধ্রুৱ ৱর্ণ *name);
ৱর্ণ *libcrun_get_external_descriptors (libcrun_container_t *container);
পূর্ণাঙ্ক libcrun_container_setgroups (libcrun_container_t *container,
                                 runtime_spec_schema_config_schema_process *process,
                                 libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_kill_linux (libcrun_container_status_t *status, পূর্ণাঙ্ক সঙ্কেত, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_create_final_userns (libcrun_container_t *container, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_create_dev (libcrun_container_t *container, পূর্ণাঙ্ক devfd,
                        পূর্ণাঙ্ক srcfd, সংরচনা device_s *device, bool binds,
                        bool ensure_parent_dir, libcrun_error_t *err);

পূর্ণাঙ্ক parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, ৱর্ণ *option, ৱর্ণ **out,
                                 size_t *len, libcrun_error_t *err);

গণনা
{
  LIBCRUN_INTELRDT_CREATE = (1 << 0),
  LIBCRUN_INTELRDT_UPDATE = (1 << 1),
  LIBCRUN_INTELRDT_MOVE = (1 << 2),
};

#define LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE (LIBCRUN_INTELRDT_CREATE | LIBCRUN_INTELRDT_UPDATE | LIBCRUN_INTELRDT_MOVE)

পূর্ণাঙ্ক libcrun_apply_intelrdt (ধ্রুৱ ৱর্ণ *ctr_name, libcrun_container_t *container, pid_t pid, পূর্ণাঙ্ক actions, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_destroy_intelrdt (ধ্রুৱ ৱর্ণ *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_update_intel_rdt (ধ্রুৱ ৱর্ণ *ctr_name, libcrun_container_t *container, ধ্রুৱ ৱর্ণ *l3_cache_schema, ধ্রুৱ ৱর্ণ *mem_bw_schema, ৱর্ণ *ধ্রুৱ *schemata, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_safe_chdir (ধ্রুৱ ৱর্ণ *path, libcrun_error_t *err);

পূর্ণাঙ্ক get_bind_mount (পূর্ণাঙ্ক dirfd, ধ্রুৱ ৱর্ণ *src, bool recursive, bool rdonly, bool nofollow, unsigned long propagation, libcrun_error_t *err);

bool is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow);

পূর্ণাঙ্ক libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

সংরচনা libcrun_mount_flags_s
{
  ৱর্ণ *target;
  unsigned long flags;
};

পূর্ণাঙ্ক libcrun_restrict_mounts_in_container (pid_t pid, সংরচনা libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err);

#endif
