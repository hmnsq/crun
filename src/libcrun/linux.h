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
#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ "error.h"
#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <argp.h>
#ଅନ୍ତର୍ଭାଵ <sys/syscall.h>
#ଅନ୍ତର୍ଭାଵ <ocispec/runtime_spec_schema_config_schema.h>
#ଅନ୍ତର୍ଭାଵ "container.h"
#ଅନ୍ତର୍ଭାଵ "status.h"

ସଂରଚନା device_s
{
  ଅଚଳ ଅକ୍ଷର *path;
  ଅକ୍ଷର *type;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା major;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା minor;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode;
  uid_t uid;
  gid_t gid;
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
syscall_clone (unsigned long flags, ଶୂନ୍ୟ *child_ଗଦା)
{
#ଯଦି defined __s390__ || defined __CRIS__
  ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) syscall (__NR_clone, child_ଗଦା, flags);
#ଅନ୍ୟଥା
  ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) syscall (__NR_clone, flags, child_ଗଦା);
#endif
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*container_entrypoint_t) (ଶୂନ୍ୟ *args, ଅକ୍ଷର *notify_socket, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_socket, libcrun_error_t *err);

ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*set_mounts_cb_t) (ଶୂନ୍ୟ *args, libcrun_error_t *err);

ସଂରଚନା libcrun_dirfd_s
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା *dirfd;
  bool joined;
};

pid_t libcrun_run_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, ଶୂନ୍ୟ *args,
                                   ପୂର୍ଣ୍ଣସଂଖ୍ୟା *sync_socket_out, ସଂରଚନା libcrun_dirfd_s *dirfd, libcrun_error_t *err);
bool libcrun_container_delayed_pidns_fork (libcrun_container_t *container);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *notify_socket_out,
                   libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_set_mounts (ସଂରଚନା container_entrypoint_s *args, libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *rootfs,
                        set_mounts_cb_t cb, ଶୂନ୍ୟ *cb_data, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_finalize_mounts (ସଂରଚନା container_entrypoint_s *entrypoint_args, libcrun_container_t *container,
                             ଅଚଳ ଅକ୍ଷର *rootfs, set_mounts_cb_t after_pivot_cb, ଶୂନ୍ୟ *cb_data, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_do_pivot_ମୂଳ (libcrun_container_t *container, bool no_pivot, ଅକ୍ଷର **rootfs, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_reopen_dev_null (libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                      ପୂର୍ଣ୍ଣସଂଖ୍ୟା no_new_privileges, bool no_exec, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **rlimits, size_t len,
                         libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_join_process (libcrun_context_t *context, libcrun_container_t *container, pid_t pid_to_join,
                          libcrun_container_status_t *status, ଅଚଳ ଅକ୍ଷର *cgroup, ପୂର୍ଣ୍ଣସଂଖ୍ୟା detach,
                          runtime_spec_schema_config_schema_process *process, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *terminal_fd, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_linux_container_update (libcrun_container_status_t *status,
                                    ଅଚଳ ଅକ୍ଷର *state_ମୂଳ,
                                    runtime_spec_schema_config_linux_resources *resources,
                                    libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_create_keyring (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଅକ୍ଷର *label, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_do_bind_mount (libcrun_container_t *container, ଅକ୍ଷର *mount_source, ଅକ୍ଷର *mount_destination,
                                     ଅକ୍ଷର **mount_options, size_t mount_options_len, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                        libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                     libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_find_namespace (ଅଚଳ ଅକ୍ଷର *name);
ଅକ୍ଷର *libcrun_get_external_descriptors (libcrun_container_t *container);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_setgroups (libcrun_container_t *container,
                                 runtime_spec_schema_config_schema_process *process,
                                 libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_kill_linux (libcrun_container_status_t *status, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ସଙ୍କେତ, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_create_final_userns (libcrun_container_t *container, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_create_dev (libcrun_container_t *container, ପୂର୍ଣ୍ଣସଂଖ୍ୟା devfd,
                        ପୂର୍ଣ୍ଣସଂଖ୍ୟା srcfd, ସଂରଚନା device_s *device, bool binds,
                        bool ensure_parent_dir, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, ଅକ୍ଷର *option, ଅକ୍ଷର **out,
                                 size_t *len, libcrun_error_t *err);

ଗଣନା
{
  LIBCRUN_INTELRDT_CREATE = (1 << 0),
  LIBCRUN_INTELRDT_UPDATE = (1 << 1),
  LIBCRUN_INTELRDT_MOVE = (1 << 2),
};

#define LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE (LIBCRUN_INTELRDT_CREATE | LIBCRUN_INTELRDT_UPDATE | LIBCRUN_INTELRDT_MOVE)

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_apply_intelrdt (ଅଚଳ ଅକ୍ଷର *ctr_name, libcrun_container_t *container, pid_t pid, ପୂର୍ଣ୍ଣସଂଖ୍ୟା actions, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_destroy_intelrdt (ଅଚଳ ଅକ୍ଷର *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_update_intel_rdt (ଅଚଳ ଅକ୍ଷର *ctr_name, libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *l3_cache_schema, ଅଚଳ ଅକ୍ଷର *mem_bw_schema, ଅକ୍ଷର *ଅଚଳ *schemata, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_safe_chdir (ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_bind_mount (ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd, ଅଚଳ ଅକ୍ଷର *src, bool recursive, bool rdonly, bool nofollow, unsigned long propagation, libcrun_error_t *err);

bool is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

ସଂରଚନା libcrun_mount_flags_s
{
  ଅକ୍ଷର *target;
  unsigned long flags;
};

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_restrict_mounts_in_container (pid_t pid, ସଂରଚନା libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err);

#endif
