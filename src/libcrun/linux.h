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
#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ "error.h"
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <argp.h>
#அந்தர்பாவ <sys/syscall.h>
#அந்தர்பாவ <ocispec/runtime_spec_schema_config_schema.h>
#அந்தர்பாவ "container.h"
#அந்தர்பாவ "status.h"

கட்டமைப்பு device_s
{
  மாறிலி எழுத்து *path;
  எழுத்து *type;
  முழு major;
  முழு minor;
  முழு mode;
  uid_t uid;
  gid_t gid;
};

நிலையான உள்வரி முழு
syscall_clone (unsigned long flags, வெறுமை *child_அடுக்கு)
{
#எனில் defined __s390__ || defined __CRIS__
  திரும்பு (முழு) syscall (__NR_clone, child_அடுக்கு, flags);
#இல்லையெனில்
  திரும்பு (முழு) syscall (__NR_clone, flags, child_அடுக்கு);
#endif
}

வகைவரை முழு (*container_entrypoint_t) (வெறுமை *args, எழுத்து *notify_socket, முழு sync_socket, libcrun_error_t *err);

வகைவரை முழு (*set_mounts_cb_t) (வெறுமை *args, libcrun_error_t *err);

கட்டமைப்பு libcrun_dirfd_s
{
  முழு *dirfd;
  bool joined;
};

pid_t libcrun_run_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, வெறுமை *args,
                                   முழு *sync_socket_out, கட்டமைப்பு libcrun_dirfd_s *dirfd, libcrun_error_t *err);
bool libcrun_container_delayed_pidns_fork (libcrun_container_t *container);
முழு libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err);
முழு get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, முழு *notify_socket_out,
                   libcrun_error_t *err);
முழு libcrun_set_mounts (கட்டமைப்பு container_entrypoint_s *args, libcrun_container_t *container, மாறிலி எழுத்து *rootfs,
                        set_mounts_cb_t cb, வெறுமை *cb_data, libcrun_error_t *err);
முழு libcrun_finalize_mounts (கட்டமைப்பு container_entrypoint_s *entrypoint_args, libcrun_container_t *container,
                             மாறிலி எழுத்து *rootfs, set_mounts_cb_t after_pivot_cb, வெறுமை *cb_data, libcrun_error_t *err);
முழு libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err);
முழு libcrun_do_pivot_வேர் (libcrun_container_t *container, bool no_pivot, எழுத்து **rootfs, libcrun_error_t *err);
முழு libcrun_reopen_dev_null (libcrun_error_t *err);
முழு libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);
முழு libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                      முழு no_new_privileges, bool no_exec, libcrun_error_t *err);
முழு libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **rlimits, size_t len,
                         libcrun_error_t *err);
முழு libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
முழு libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
முழு libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err);
முழு libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err);
முழு libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err);
முழு libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err);
முழு libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err);
முழு libcrun_join_process (libcrun_context_t *context, libcrun_container_t *container, pid_t pid_to_join,
                          libcrun_container_status_t *status, மாறிலி எழுத்து *cgroup, முழு detach,
                          runtime_spec_schema_config_schema_process *process, முழு *terminal_fd, libcrun_error_t *err);
முழு libcrun_linux_container_update (libcrun_container_status_t *status,
                                    மாறிலி எழுத்து *state_வேர்,
                                    runtime_spec_schema_config_linux_resources *resources,
                                    libcrun_error_t *err);
முழு libcrun_create_keyring (libcrun_container_t *container, மாறிலி எழுத்து *name, மாறிலி எழுத்து *label, libcrun_error_t *err);
முழு libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err);
முழு libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err);
முழு libcrun_container_do_bind_mount (libcrun_container_t *container, எழுத்து *mount_source, எழுத்து *mount_destination,
                                     எழுத்து **mount_options, size_t mount_options_len, libcrun_error_t *err);
முழு libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err);
முழு libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err);
முழு libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err);

முழு libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                        libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

முழு libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                     libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

முழு libcrun_find_namespace (மாறிலி எழுத்து *name);
எழுத்து *libcrun_get_external_descriptors (libcrun_container_t *container);
முழு libcrun_container_setgroups (libcrun_container_t *container,
                                 runtime_spec_schema_config_schema_process *process,
                                 libcrun_error_t *err);
முழு libcrun_kill_linux (libcrun_container_status_t *status, முழு சமிக்ஞை, libcrun_error_t *err);
முழு libcrun_create_final_userns (libcrun_container_t *container, libcrun_error_t *err);
முழு libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err);

முழு libcrun_create_dev (libcrun_container_t *container, முழு devfd,
                        முழு srcfd, கட்டமைப்பு device_s *device, bool binds,
                        bool ensure_parent_dir, libcrun_error_t *err);

முழு parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, எழுத்து *option, எழுத்து **out,
                                 size_t *len, libcrun_error_t *err);

எண்ணிக்கை
{
  LIBCRUN_INTELRDT_CREATE = (1 << 0),
  LIBCRUN_INTELRDT_UPDATE = (1 << 1),
  LIBCRUN_INTELRDT_MOVE = (1 << 2),
};

#define LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE (LIBCRUN_INTELRDT_CREATE | LIBCRUN_INTELRDT_UPDATE | LIBCRUN_INTELRDT_MOVE)

முழு libcrun_apply_intelrdt (மாறிலி எழுத்து *ctr_name, libcrun_container_t *container, pid_t pid, முழு actions, libcrun_error_t *err);

முழு libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);

முழு libcrun_destroy_intelrdt (மாறிலி எழுத்து *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err);

முழு libcrun_update_intel_rdt (மாறிலி எழுத்து *ctr_name, libcrun_container_t *container, மாறிலி எழுத்து *l3_cache_schema, மாறிலி எழுத்து *mem_bw_schema, எழுத்து *மாறிலி *schemata, libcrun_error_t *err);

முழு libcrun_safe_chdir (மாறிலி எழுத்து *path, libcrun_error_t *err);

முழு get_bind_mount (முழு dirfd, மாறிலி எழுத்து *src, bool recursive, bool rdonly, bool nofollow, unsigned long propagation, libcrun_error_t *err);

bool is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow);

முழு libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

முழு libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

கட்டமைப்பு libcrun_mount_flags_s
{
  எழுத்து *target;
  unsigned long flags;
};

முழு libcrun_restrict_mounts_in_container (pid_t pid, கட்டமைப்பு libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err);

#endif
