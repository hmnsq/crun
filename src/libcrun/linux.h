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
#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include <errno.h>
#include <argp.h>
#include <sys/syscall.h>
#include <ocispec/runtime_spec_schema_config_schema.h>
#include "container.h"
#include "status.h"

struct device_s
{
  const char *path;
  char *type;
  정수 major;
  정수 minor;
  정수 mode;
  uid_t uid;
  gid_t gid;
};

static inline 정수
syscall_clone (unsigned long flags, void *child_stack)
{
#만약 defined __s390__ || defined __CRIS__
  반환 (정수) syscall (__NR_clone, child_stack, flags);
#아니면
  반환 (정수) syscall (__NR_clone, flags, child_stack);
#endif
}

typedef 정수 (*container_entrypoint_t) (void *args, char *notify_socket, 정수 sync_socket, libcrun_error_t *err);

typedef 정수 (*set_mounts_cb_t) (void *args, libcrun_error_t *err);

struct libcrun_dirfd_s
{
  정수 *dirfd;
  bool joined;
};

pid_t libcrun_run_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, void *args,
                                   정수 *sync_socket_out, struct libcrun_dirfd_s *dirfd, libcrun_error_t *err);
bool libcrun_container_delayed_pidns_fork (libcrun_container_t *container);
정수 libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err);
정수 get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, 정수 *notify_socket_out,
                   libcrun_error_t *err);
정수 libcrun_set_mounts (struct container_entrypoint_s *args, libcrun_container_t *container, const char *rootfs,
                        set_mounts_cb_t cb, void *cb_data, libcrun_error_t *err);
정수 libcrun_finalize_mounts (struct container_entrypoint_s *entrypoint_args, libcrun_container_t *container,
                             const char *rootfs, set_mounts_cb_t after_pivot_cb, void *cb_data, libcrun_error_t *err);
정수 libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err);
정수 libcrun_do_pivot_root (libcrun_container_t *container, bool no_pivot, char **rootfs, libcrun_error_t *err);
정수 libcrun_reopen_dev_null (libcrun_error_t *err);
정수 libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);
정수 libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                      정수 no_new_privileges, bool no_exec, libcrun_error_t *err);
정수 libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **rlimits, size_t len,
                         libcrun_error_t *err);
정수 libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
정수 libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
정수 libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err);
정수 libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err);
정수 libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err);
정수 libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err);
정수 libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err);
정수 libcrun_join_process (libcrun_context_t *context, libcrun_container_t *container, pid_t pid_to_join,
                          libcrun_container_status_t *status, const char *cgroup, 정수 detach,
                          runtime_spec_schema_config_schema_process *process, 정수 *terminal_fd, libcrun_error_t *err);
정수 libcrun_linux_container_update (libcrun_container_status_t *status,
                                    const char *state_root,
                                    runtime_spec_schema_config_linux_resources *resources,
                                    libcrun_error_t *err);
정수 libcrun_create_keyring (libcrun_container_t *container, const char *name, const char *label, libcrun_error_t *err);
정수 libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err);
정수 libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err);
정수 libcrun_container_do_bind_mount (libcrun_container_t *container, char *mount_source, char *mount_destination,
                                     char **mount_options, size_t mount_options_len, libcrun_error_t *err);
정수 libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err);
정수 libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err);
정수 libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err);

정수 libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                        libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

정수 libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                     libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

정수 libcrun_find_namespace (const char *name);
char *libcrun_get_external_descriptors (libcrun_container_t *container);
정수 libcrun_container_setgroups (libcrun_container_t *container,
                                 runtime_spec_schema_config_schema_process *process,
                                 libcrun_error_t *err);
정수 libcrun_kill_linux (libcrun_container_status_t *status, 정수 signal, libcrun_error_t *err);
정수 libcrun_create_final_userns (libcrun_container_t *container, libcrun_error_t *err);
정수 libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err);

정수 libcrun_create_dev (libcrun_container_t *container, 정수 devfd,
                        정수 srcfd, struct device_s *device, bool binds,
                        bool ensure_parent_dir, libcrun_error_t *err);

정수 parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, char *option, char **out,
                                 size_t *len, libcrun_error_t *err);

enum
{
  LIBCRUN_INTELRDT_CREATE = (1 << 0),
  LIBCRUN_INTELRDT_UPDATE = (1 << 1),
  LIBCRUN_INTELRDT_MOVE = (1 << 2),
};

#define LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE (LIBCRUN_INTELRDT_CREATE | LIBCRUN_INTELRDT_UPDATE | LIBCRUN_INTELRDT_MOVE)

정수 libcrun_apply_intelrdt (const char *ctr_name, libcrun_container_t *container, pid_t pid, 정수 actions, libcrun_error_t *err);

정수 libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);

정수 libcrun_destroy_intelrdt (const char *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err);

정수 libcrun_update_intel_rdt (const char *ctr_name, libcrun_container_t *container, const char *l3_cache_schema, const char *mem_bw_schema, char *const *schemata, libcrun_error_t *err);

정수 libcrun_safe_chdir (const char *path, libcrun_error_t *err);

정수 get_bind_mount (정수 dirfd, const char *src, bool recursive, bool rdonly, bool nofollow, unsigned long propagation, libcrun_error_t *err);

bool is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow);

정수 libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

정수 libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

struct libcrun_mount_flags_s
{
  char *target;
  unsigned long flags;
};

정수 libcrun_restrict_mounts_in_container (pid_t pid, struct libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err);

#endif
