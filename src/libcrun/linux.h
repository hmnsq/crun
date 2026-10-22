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
#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ "error.h"
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <argp.h>
#അന്തര്ഭാവ <sys/syscall.h>
#അന്തര്ഭാവ <ocispec/runtime_spec_schema_config_schema.h>
#അന്തര്ഭാവ "container.h"
#അന്തര്ഭാവ "status.h"

ഘടന device_s
{
  സ്ഥിരം അക്ഷരം *path;
  അക്ഷരം *type;
  പൂർണ്ണസംഖ്യ major;
  പൂർണ്ണസംഖ്യ minor;
  പൂർണ്ണസംഖ്യ mode;
  uid_t uid;
  gid_t gid;
};

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
syscall_clone (unsigned long flags, ശൂന്യം *child_അട്ടി)
{
#എങ്കിൽ defined __s390__ || defined __CRIS__
  തിരികെ (പൂർണ്ണസംഖ്യ) syscall (__NR_clone, child_അട്ടി, flags);
#അല്ലെങ്കിൽ
  തിരികെ (പൂർണ്ണസംഖ്യ) syscall (__NR_clone, flags, child_അട്ടി);
#endif
}

തരനിർവചനം പൂർണ്ണസംഖ്യ (*container_entrypoint_t) (ശൂന്യം *args, അക്ഷരം *notify_socket, പൂർണ്ണസംഖ്യ sync_socket, libcrun_error_t *err);

തരനിർവചനം പൂർണ്ണസംഖ്യ (*set_mounts_cb_t) (ശൂന്യം *args, libcrun_error_t *err);

ഘടന libcrun_dirfd_s
{
  പൂർണ്ണസംഖ്യ *dirfd;
  bool joined;
};

pid_t libcrun_run_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, ശൂന്യം *args,
                                   പൂർണ്ണസംഖ്യ *sync_socket_out, ഘടന libcrun_dirfd_s *dirfd, libcrun_error_t *err);
bool libcrun_container_delayed_pidns_fork (libcrun_container_t *container);
പൂർണ്ണസംഖ്യ libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, പൂർണ്ണസംഖ്യ *notify_socket_out,
                   libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_set_mounts (ഘടന container_entrypoint_s *args, libcrun_container_t *container, സ്ഥിരം അക്ഷരം *rootfs,
                        set_mounts_cb_t cb, ശൂന്യം *cb_data, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_finalize_mounts (ഘടന container_entrypoint_s *entrypoint_args, libcrun_container_t *container,
                             സ്ഥിരം അക്ഷരം *rootfs, set_mounts_cb_t after_pivot_cb, ശൂന്യം *cb_data, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_do_pivot_വേര് (libcrun_container_t *container, bool no_pivot, അക്ഷരം **rootfs, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_reopen_dev_null (libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                      പൂർണ്ണസംഖ്യ no_new_privileges, bool no_exec, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **rlimits, size_t len,
                         libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_join_process (libcrun_context_t *context, libcrun_container_t *container, pid_t pid_to_join,
                          libcrun_container_status_t *status, സ്ഥിരം അക്ഷരം *cgroup, പൂർണ്ണസംഖ്യ detach,
                          runtime_spec_schema_config_schema_process *process, പൂർണ്ണസംഖ്യ *terminal_fd, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_linux_container_update (libcrun_container_status_t *status,
                                    സ്ഥിരം അക്ഷരം *state_വേര്,
                                    runtime_spec_schema_config_linux_resources *resources,
                                    libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_create_keyring (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *label, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_container_do_bind_mount (libcrun_container_t *container, അക്ഷരം *mount_source, അക്ഷരം *mount_destination,
                                     അക്ഷരം **mount_options, size_t mount_options_len, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                        libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                     libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_find_namespace (സ്ഥിരം അക്ഷരം *name);
അക്ഷരം *libcrun_get_external_descriptors (libcrun_container_t *container);
പൂർണ്ണസംഖ്യ libcrun_container_setgroups (libcrun_container_t *container,
                                 runtime_spec_schema_config_schema_process *process,
                                 libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_kill_linux (libcrun_container_status_t *status, പൂർണ്ണസംഖ്യ സൂചന, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_create_final_userns (libcrun_container_t *container, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_create_dev (libcrun_container_t *container, പൂർണ്ണസംഖ്യ devfd,
                        പൂർണ്ണസംഖ്യ srcfd, ഘടന device_s *device, bool binds,
                        bool ensure_parent_dir, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, അക്ഷരം *option, അക്ഷരം **out,
                                 size_t *len, libcrun_error_t *err);

എണ്ണൽ
{
  LIBCRUN_INTELRDT_CREATE = (1 << 0),
  LIBCRUN_INTELRDT_UPDATE = (1 << 1),
  LIBCRUN_INTELRDT_MOVE = (1 << 2),
};

#define LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE (LIBCRUN_INTELRDT_CREATE | LIBCRUN_INTELRDT_UPDATE | LIBCRUN_INTELRDT_MOVE)

പൂർണ്ണസംഖ്യ libcrun_apply_intelrdt (സ്ഥിരം അക്ഷരം *ctr_name, libcrun_container_t *container, pid_t pid, പൂർണ്ണസംഖ്യ actions, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_destroy_intelrdt (സ്ഥിരം അക്ഷരം *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_update_intel_rdt (സ്ഥിരം അക്ഷരം *ctr_name, libcrun_container_t *container, സ്ഥിരം അക്ഷരം *l3_cache_schema, സ്ഥിരം അക്ഷരം *mem_bw_schema, അക്ഷരം *സ്ഥിരം *schemata, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_safe_chdir (സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ get_bind_mount (പൂർണ്ണസംഖ്യ dirfd, സ്ഥിരം അക്ഷരം *src, bool recursive, bool rdonly, bool nofollow, unsigned long propagation, libcrun_error_t *err);

bool is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow);

പൂർണ്ണസംഖ്യ libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

ഘടന libcrun_mount_flags_s
{
  അക്ഷരം *target;
  unsigned long flags;
};

പൂർണ്ണസംഖ്യ libcrun_restrict_mounts_in_container (pid_t pid, ഘടന libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err);

#endif
