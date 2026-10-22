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
#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব "error.h"
#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <argp.h>
#অন্তর্ভাব <sys/syscall.h>
#অন্তর্ভাব <ocispec/runtime_spec_schema_config_schema.h>
#অন্তর্ভাব "container.h"
#অন্তর্ভাব "status.h"

কাঠামো device_s
{
  ধ্রুবক অক্ষর *path;
  অক্ষর *type;
  পূর্ণসংখ্যা major;
  পূর্ণসংখ্যা minor;
  পূর্ণসংখ্যা mode;
  uid_t uid;
  gid_t gid;
};

স্থির অন্তঃরেখ পূর্ণসংখ্যা
syscall_clone (unsigned long flags, শূন্য *child_স্তূপ)
{
#যদি defined __s390__ || defined __CRIS__
  ফেরত (পূর্ণসংখ্যা) syscall (__NR_clone, child_স্তূপ, flags);
#অন্যথা
  ফেরত (পূর্ণসংখ্যা) syscall (__NR_clone, flags, child_স্তূপ);
#endif
}

প্রকারলেখ পূর্ণসংখ্যা (*container_entrypoint_t) (শূন্য *args, অক্ষর *notify_socket, পূর্ণসংখ্যা sync_socket, libcrun_error_t *err);

প্রকারলেখ পূর্ণসংখ্যা (*set_mounts_cb_t) (শূন্য *args, libcrun_error_t *err);

কাঠামো libcrun_dirfd_s
{
  পূর্ণসংখ্যা *dirfd;
  bool joined;
};

pid_t libcrun_run_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, শূন্য *args,
                                   পূর্ণসংখ্যা *sync_socket_out, কাঠামো libcrun_dirfd_s *dirfd, libcrun_error_t *err);
bool libcrun_container_delayed_pidns_fork (libcrun_container_t *container);
পূর্ণসংখ্যা libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err);
পূর্ণসংখ্যা get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, পূর্ণসংখ্যা *notify_socket_out,
                   libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_set_mounts (কাঠামো container_entrypoint_s *args, libcrun_container_t *container, ধ্রুবক অক্ষর *rootfs,
                        set_mounts_cb_t cb, শূন্য *cb_তথ্য, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_finalize_mounts (কাঠামো container_entrypoint_s *entrypoint_args, libcrun_container_t *container,
                             ধ্রুবক অক্ষর *rootfs, set_mounts_cb_t after_pivot_cb, শূন্য *cb_তথ্য, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_do_pivot_মূল (libcrun_container_t *container, bool no_pivot, অক্ষর **rootfs, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_reopen_dev_null (libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                      পূর্ণসংখ্যা no_new_privileges, bool no_exec, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **rlimits, size_t len,
                         libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_join_process (libcrun_context_t *context, libcrun_container_t *container, pid_t pid_to_join,
                          libcrun_container_status_t *status, ধ্রুবক অক্ষর *cgroup, পূর্ণসংখ্যা detach,
                          runtime_spec_schema_config_schema_process *process, পূর্ণসংখ্যা *terminal_fd, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_linux_container_update (libcrun_container_status_t *status,
                                    ধ্রুবক অক্ষর *state_মূল,
                                    runtime_spec_schema_config_linux_resources *resources,
                                    libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_create_keyring (libcrun_container_t *container, ধ্রুবক অক্ষর *name, ধ্রুবক অক্ষর *label, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_container_do_bind_mount (libcrun_container_t *container, অক্ষর *mount_source, অক্ষর *mount_destination,
                                     অক্ষর **mount_options, size_t mount_options_len, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                        libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                     libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_find_namespace (ধ্রুবক অক্ষর *name);
অক্ষর *libcrun_get_external_descriptors (libcrun_container_t *container);
পূর্ণসংখ্যা libcrun_container_setgroups (libcrun_container_t *container,
                                 runtime_spec_schema_config_schema_process *process,
                                 libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_kill_linux (libcrun_container_status_t *status, পূর্ণসংখ্যা সংকেত, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_create_final_userns (libcrun_container_t *container, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_create_dev (libcrun_container_t *container, পূর্ণসংখ্যা devfd,
                        পূর্ণসংখ্যা srcfd, কাঠামো device_s *device, bool binds,
                        bool ensure_parent_dir, libcrun_error_t *err);

পূর্ণসংখ্যা parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, অক্ষর *option, অক্ষর **out,
                                 size_t *len, libcrun_error_t *err);

গণনা
{
  LIBCRUN_INTELRDT_CREATE = (1 << 0),
  LIBCRUN_INTELRDT_UPDATE = (1 << 1),
  LIBCRUN_INTELRDT_MOVE = (1 << 2),
};

#define LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE (LIBCRUN_INTELRDT_CREATE | LIBCRUN_INTELRDT_UPDATE | LIBCRUN_INTELRDT_MOVE)

পূর্ণসংখ্যা libcrun_apply_intelrdt (ধ্রুবক অক্ষর *ctr_name, libcrun_container_t *container, pid_t pid, পূর্ণসংখ্যা actions, libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_destroy_intelrdt (ধ্রুবক অক্ষর *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_update_intel_rdt (ধ্রুবক অক্ষর *ctr_name, libcrun_container_t *container, ধ্রুবক অক্ষর *l3_cache_schema, ধ্রুবক অক্ষর *mem_bw_schema, অক্ষর *ধ্রুবক *schemata, libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_safe_chdir (ধ্রুবক অক্ষর *path, libcrun_error_t *err);

পূর্ণসংখ্যা get_bind_mount (পূর্ণসংখ্যা dirfd, ধ্রুবক অক্ষর *src, bool recursive, bool rdonly, bool nofollow, unsigned long propagation, libcrun_error_t *err);

bool is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow);

পূর্ণসংখ্যা libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

পূর্ণসংখ্যা libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

কাঠামো libcrun_mount_flags_s
{
  অক্ষর *target;
  unsigned long flags;
};

পূর্ণসংখ্যা libcrun_restrict_mounts_in_container (pid_t pid, কাঠামো libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err);

#endif
