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
#شامل <config.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل "error.h"
#شامل <errno.h>
#شامل <argp.h>
#شامل <sys/syscall.h>
#شامل <ocispec/runtime_spec_schema_config_schema.h>
#شامل "container.h"
#شامل "status.h"

ساختار device_s
{
  ثابت نویسه *path;
  نویسه *type;
  صحیح major;
  صحیح minor;
  صحیح mode;
  uid_t uid;
  gid_t gid;
};

ایستا درونخطی صحیح
syscall_clone (unsigned long flags, تهی *child_پشته)
{
#اگر defined __s390__ || defined __CRIS__
  بازگشت (صحیح) syscall (__NR_clone, child_پشته, flags);
#وگرنه
  بازگشت (صحیح) syscall (__NR_clone, flags, child_پشته);
#endif
}

تعریفنوع صحیح (*container_entrypoint_t) (تهی *args, نویسه *notify_socket, صحیح sync_socket, libcrun_error_t *err);

تعریفنوع صحیح (*set_mounts_cb_t) (تهی *args, libcrun_error_t *err);

ساختار libcrun_dirfd_s
{
  صحیح *dirfd;
  bool joined;
};

pid_t libcrun_run_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, تهی *args,
                                   صحیح *sync_socket_out, ساختار libcrun_dirfd_s *dirfd, libcrun_error_t *err);
bool libcrun_container_delayed_pidns_fork (libcrun_container_t *container);
صحیح libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err);
صحیح get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, صحیح *notify_socket_out,
                   libcrun_error_t *err);
صحیح libcrun_set_mounts (ساختار container_entrypoint_s *args, libcrun_container_t *container, ثابت نویسه *rootfs,
                        set_mounts_cb_t cb, تهی *cb_data, libcrun_error_t *err);
صحیح libcrun_finalize_mounts (ساختار container_entrypoint_s *entrypoint_args, libcrun_container_t *container,
                             ثابت نویسه *rootfs, set_mounts_cb_t after_pivot_cb, تهی *cb_data, libcrun_error_t *err);
صحیح libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err);
صحیح libcrun_do_pivot_ریشه (libcrun_container_t *container, bool no_pivot, نویسه **rootfs, libcrun_error_t *err);
صحیح libcrun_reopen_dev_null (libcrun_error_t *err);
صحیح libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);
صحیح libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                      صحیح no_new_privileges, bool no_exec, libcrun_error_t *err);
صحیح libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **rlimits, size_t len,
                         libcrun_error_t *err);
صحیح libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
صحیح libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
صحیح libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err);
صحیح libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err);
صحیح libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err);
صحیح libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err);
صحیح libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err);
صحیح libcrun_join_process (libcrun_context_t *context, libcrun_container_t *container, pid_t pid_to_join,
                          libcrun_container_status_t *status, ثابت نویسه *cgroup, صحیح detach,
                          runtime_spec_schema_config_schema_process *process, صحیح *terminal_fd, libcrun_error_t *err);
صحیح libcrun_linux_container_update (libcrun_container_status_t *status,
                                    ثابت نویسه *state_ریشه,
                                    runtime_spec_schema_config_linux_resources *resources,
                                    libcrun_error_t *err);
صحیح libcrun_create_keyring (libcrun_container_t *container, ثابت نویسه *name, ثابت نویسه *label, libcrun_error_t *err);
صحیح libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err);
صحیح libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err);
صحیح libcrun_container_do_bind_mount (libcrun_container_t *container, نویسه *mount_source, نویسه *mount_destination,
                                     نویسه **mount_options, size_t mount_options_len, libcrun_error_t *err);
صحیح libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err);
صحیح libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err);
صحیح libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err);

صحیح libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                        libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

صحیح libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                     libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

صحیح libcrun_find_namespace (ثابت نویسه *name);
نویسه *libcrun_get_external_descriptors (libcrun_container_t *container);
صحیح libcrun_container_setgroups (libcrun_container_t *container,
                                 runtime_spec_schema_config_schema_process *process,
                                 libcrun_error_t *err);
صحیح libcrun_kill_linux (libcrun_container_status_t *status, صحیح سیگنال, libcrun_error_t *err);
صحیح libcrun_create_final_userns (libcrun_container_t *container, libcrun_error_t *err);
صحیح libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err);

صحیح libcrun_create_dev (libcrun_container_t *container, صحیح devfd,
                        صحیح srcfd, ساختار device_s *device, bool binds,
                        bool ensure_parent_dir, libcrun_error_t *err);

صحیح parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, نویسه *option, نویسه **out,
                                 size_t *len, libcrun_error_t *err);

شمارش
{
  LIBCRUN_INTELRDT_CREATE = (1 << 0),
  LIBCRUN_INTELRDT_UPDATE = (1 << 1),
  LIBCRUN_INTELRDT_MOVE = (1 << 2),
};

#define LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE (LIBCRUN_INTELRDT_CREATE | LIBCRUN_INTELRDT_UPDATE | LIBCRUN_INTELRDT_MOVE)

صحیح libcrun_apply_intelrdt (ثابت نویسه *ctr_name, libcrun_container_t *container, pid_t pid, صحیح actions, libcrun_error_t *err);

صحیح libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);

صحیح libcrun_destroy_intelrdt (ثابت نویسه *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err);

صحیح libcrun_update_intel_rdt (ثابت نویسه *ctr_name, libcrun_container_t *container, ثابت نویسه *l3_cache_schema, ثابت نویسه *mem_bw_schema, نویسه *ثابت *schemata, libcrun_error_t *err);

صحیح libcrun_safe_chdir (ثابت نویسه *path, libcrun_error_t *err);

صحیح get_bind_mount (صحیح dirfd, ثابت نویسه *src, bool recursive, bool rdonly, bool nofollow, unsigned long propagation, libcrun_error_t *err);

bool is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow);

صحیح libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

صحیح libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

ساختار libcrun_mount_flags_s
{
  نویسه *target;
  unsigned long flags;
};

صحیح libcrun_restrict_mounts_in_container (pid_t pid, ساختار libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err);

#endif
