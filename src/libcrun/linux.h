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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LINUX_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_H
#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "error.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <argp.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/syscall.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ocispec/runtime_spec_schema_config_schema.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "container.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "status.h"

ਬਣਤਰ device_s
{
  ਅਚਲ ਅੱਖਰ *ਰਸਤਾ;
  ਅੱਖਰ *type;
  ਪੂਰਨ major;
  ਪੂਰਨ minor;
  ਪੂਰਨ ਬਹੁਲਕ;
  uid_t uid;
  gid_t gid;
};

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
syscall_clone (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags, ਖਾਲੀ *ਬੱਚਾ_ਢੇਰ)
{
#ਜੇ defined __s390__ || defined __CRIS__
  ਵਾਪਸ (ਪੂਰਨ) syscall (__NR_clone, ਬੱਚਾ_ਢੇਰ, flags);
#ਨਹੀਂਤਾਂ
  ਵਾਪਸ (ਪੂਰਨ) syscall (__NR_clone, flags, ਬੱਚਾ_ਢੇਰ);
#ਜੇ_ਅੰਤ
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ (*container_entrypoint_t) (ਖਾਲੀ *ਦਲੀਲਾਂ, ਅੱਖਰ *notify_socket, ਪੂਰਨ sync_socket, libcrun_error_t *err);

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ (*ਸੈੱਟ_mounts_cb_t) (ਖਾਲੀ *ਦਲੀਲਾਂ, libcrun_error_t *err);

ਬਣਤਰ libcrun_dirfd_s
{
  ਪੂਰਨ *dirfd;
  bool joined;
};

pid_t libcrun_ਲੜੀਵਾਰ_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, ਖਾਲੀ *ਦਲੀਲਾਂ,
                                   ਪੂਰਨ *sync_socket_ਬਾਹਰਲਾ, ਬਣਤਰ libcrun_dirfd_s *dirfd, libcrun_error_t *err);
bool libcrun_container_delayed_pidns_fork (libcrun_container_t *container);
ਪੂਰਨ libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err);
ਪੂਰਨ get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, ਪੂਰਨ *notify_socket_ਬਾਹਰਲਾ,
                   libcrun_error_t *err);
ਪੂਰਨ libcrun_ਸੈੱਟ_mounts (ਬਣਤਰ container_entrypoint_s *ਦਲੀਲਾਂ, libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *rootfs,
                        ਸੈੱਟ_mounts_cb_t cb, ਖਾਲੀ *cb_ਅੰਕੜੇ, libcrun_error_t *err);
ਪੂਰਨ libcrun_finalize_mounts (ਬਣਤਰ container_entrypoint_s *entrypoint_ਦਲੀਲਾਂ, libcrun_container_t *container,
                             ਅਚਲ ਅੱਖਰ *rootfs, ਸੈੱਟ_mounts_cb_t after_ਧੁਰਾ_cb, ਖਾਲੀ *cb_ਅੰਕੜੇ, libcrun_error_t *err);
ਪੂਰਨ libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err);
ਪੂਰਨ libcrun_do_ਧੁਰਾ_ਜੜ੍ਹ (libcrun_container_t *container, bool no_ਧੁਰਾ, ਅੱਖਰ **rootfs, libcrun_error_t *err);
ਪੂਰਨ libcrun_reopen_dev_null (libcrun_error_t *err);
ਪੂਰਨ libcrun_ਸੈੱਟ_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);
ਪੂਰਨ libcrun_ਸੈੱਟ_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                      ਪੂਰਨ no_new_privileges, bool no_exec, libcrun_error_t *err);
ਪੂਰਨ libcrun_ਸੈੱਟ_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **rlimits, size_t len,
                         libcrun_error_t *err);
ਪੂਰਨ libcrun_ਸੈੱਟ_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
ਪੂਰਨ libcrun_ਸੈੱਟ_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
ਪੂਰਨ libcrun_ਸੈੱਟ_hostname (libcrun_container_t *container, libcrun_error_t *err);
ਪੂਰਨ libcrun_ਸੈੱਟ_domainname (libcrun_container_t *container, libcrun_error_t *err);
ਪੂਰਨ libcrun_ਸੈੱਟ_oom (libcrun_container_t *container, libcrun_error_t *err);
ਪੂਰਨ libcrun_ਸੈੱਟ_sysctl (libcrun_container_t *container, libcrun_error_t *err);
ਪੂਰਨ libcrun_ਸੈੱਟ_terminal (libcrun_container_t *container, libcrun_error_t *err);
ਪੂਰਨ libcrun_join_process (libcrun_context_t *context, libcrun_container_t *container, pid_t pid_ਮੰਜ਼ਿਲ_join,
                          libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, ਅਚਲ ਅੱਖਰ *cgroup, ਪੂਰਨ detach,
                          runtime_spec_schema_config_schema_process *process, ਪੂਰਨ *terminal_fd, libcrun_error_t *err);
ਪੂਰਨ libcrun_linux_container_update (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ,
                                    ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ,
                                    runtime_spec_schema_config_linux_resources *resources,
                                    libcrun_error_t *err);
ਪੂਰਨ libcrun_create_keyring (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *label, libcrun_error_t *err);
ਪੂਰਨ libcrun_container_pause_linux (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, libcrun_error_t *err);
ਪੂਰਨ libcrun_container_unpause_linux (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, libcrun_error_t *err);
ਪੂਰਨ libcrun_container_do_bind_mount (libcrun_container_t *container, ਅੱਖਰ *mount_ਸਰੋਤ, ਅੱਖਰ *mount_destination,
                                     ਅੱਖਰ **mount_options, size_t mount_options_len, libcrun_error_t *err);
ਪੂਰਨ libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err);
ਪੂਰਨ libcrun_ਸੈੱਟ_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err);
ਪੂਰਨ libcrun_configure_ਜਾਲ (libcrun_container_t *container, libcrun_error_t *err);

ਪੂਰਨ libcrun_container_checkpoint_linux (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, libcrun_container_t *container,
                                        libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

ਪੂਰਨ libcrun_container_restore_linux (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, libcrun_container_t *container,
                                     libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

ਪੂਰਨ libcrun_ਲੱਭੋ_namespace (ਅਚਲ ਅੱਖਰ *name);
ਅੱਖਰ *libcrun_get_external_descriptors (libcrun_container_t *container);
ਪੂਰਨ libcrun_container_setgroups (libcrun_container_t *container,
                                 runtime_spec_schema_config_schema_process *process,
                                 libcrun_error_t *err);
ਪੂਰਨ libcrun_kill_linux (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, ਪੂਰਨ ਸੰਕੇਤ, libcrun_error_t *err);
ਪੂਰਨ libcrun_create_final_userns (libcrun_container_t *container, libcrun_error_t *err);
ਪੂਰਨ libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err);

ਪੂਰਨ libcrun_create_dev (libcrun_container_t *container, ਪੂਰਨ devfd,
                        ਪੂਰਨ srcfd, ਬਣਤਰ device_s *device, bool binds,
                        bool ensure_ਮਾਪਾ_dir, libcrun_error_t *err);

ਪੂਰਨ parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, ਅੱਖਰ *option, ਅੱਖਰ **ਬਾਹਰਲਾ,
                                 size_t *len, libcrun_error_t *err);

ਗਿਣਤੀ
{
  LIBCRUN_INTELRDT_CREATE = (1 << 0),
  LIBCRUN_INTELRDT_UPDATE = (1 << 1),
  LIBCRUN_INTELRDT_MOVE = (1 << 2),
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE (LIBCRUN_INTELRDT_CREATE | LIBCRUN_INTELRDT_UPDATE | LIBCRUN_INTELRDT_MOVE)

ਪੂਰਨ libcrun_apply_intelrdt (ਅਚਲ ਅੱਖਰ *ctr_name, libcrun_container_t *container, pid_t pid, ਪੂਰਨ actions, libcrun_error_t *err);

ਪੂਰਨ libcrun_move_ਜਾਲ_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);

ਪੂਰਨ libcrun_destroy_intelrdt (ਅਚਲ ਅੱਖਰ *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err);

ਪੂਰਨ libcrun_update_intel_rdt (ਅਚਲ ਅੱਖਰ *ctr_name, libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *l3_cache_schema, ਅਚਲ ਅੱਖਰ *mem_bw_schema, ਅੱਖਰ *ਅਚਲ *schemata, libcrun_error_t *err);

ਪੂਰਨ libcrun_ਸੁਰੱਖਿਅਤ_chdir (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err);

ਪੂਰਨ get_bind_mount (ਪੂਰਨ dirfd, ਅਚਲ ਅੱਖਰ *src, bool recursive, bool rdonly, bool nofollow, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪ੍ਰਸਾਰ, libcrun_error_t *err);

bool is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow);

ਪੂਰਨ libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

ਪੂਰਨ libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

ਬਣਤਰ libcrun_mount_flags_s
{
  ਅੱਖਰ *ਨਿਸ਼ਾਨਾ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;
};

ਪੂਰਨ libcrun_restrict_mounts_ਵਿੱਚ_container (pid_t pid, ਬਣਤਰ libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err);

#ਜੇ_ਅੰਤ
