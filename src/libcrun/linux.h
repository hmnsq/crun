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
#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव "error.h"
#अन्तर्भाव <errno.h>
#अन्तर्भाव <argp.h>
#अन्तर्भाव <sys/syscall.h>
#अन्तर्भाव <ocispec/runtime_spec_schema_config_schema.h>
#अन्तर्भाव "container.h"
#अन्तर्भाव "status.h"

संरचना device_s
{
  ध्रुव वर्ण *path;
  वर्ण *type;
  पूर्णाङ्क major;
  पूर्णाङ्क minor;
  पूर्णाङ्क mode;
  uid_t uid;
  gid_t gid;
};

स्थिर अन्तःस्थ पूर्णाङ्क
syscall_clone (अचिन्हित दीर्घ flags, शून्य *child_ढेर)
{
#यदि defined __s390__ || defined __CRIS__
  प्रत्यागम (पूर्णाङ्क) syscall (__NR_clone, child_ढेर, flags);
#अन्यथा
  प्रत्यागम (पूर्णाङ्क) syscall (__NR_clone, flags, child_ढेर);
#endif
}

प्रकारनिर्देश पूर्णाङ्क (*container_entrypoint_t) (शून्य *args, वर्ण *notify_socket, पूर्णाङ्क sync_socket, libcrun_error_t *err);

प्रकारनिर्देश पूर्णाङ्क (*set_mounts_cb_t) (शून्य *args, libcrun_error_t *err);

संरचना libcrun_dirfd_s
{
  पूर्णाङ्क *dirfd;
  bool joined;
};

pid_t libcrun_run_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, शून्य *args,
                                   पूर्णाङ्क *sync_socket_out, संरचना libcrun_dirfd_s *dirfd, libcrun_error_t *err);
bool libcrun_container_delayed_pidns_fork (libcrun_container_t *container);
पूर्णाङ्क libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err);
पूर्णाङ्क get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, पूर्णाङ्क *notify_socket_out,
                   libcrun_error_t *err);
पूर्णाङ्क libcrun_set_mounts (संरचना container_entrypoint_s *args, libcrun_container_t *container, ध्रुव वर्ण *rootfs,
                        set_mounts_cb_t cb, शून्य *cb_data, libcrun_error_t *err);
पूर्णाङ्क libcrun_finalize_mounts (संरचना container_entrypoint_s *entrypoint_args, libcrun_container_t *container,
                             ध्रुव वर्ण *rootfs, set_mounts_cb_t after_pivot_cb, शून्य *cb_data, libcrun_error_t *err);
पूर्णाङ्क libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err);
पूर्णाङ्क libcrun_do_pivot_मूल (libcrun_container_t *container, bool no_pivot, वर्ण **rootfs, libcrun_error_t *err);
पूर्णाङ्क libcrun_reopen_dev_null (libcrun_error_t *err);
पूर्णाङ्क libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);
पूर्णाङ्क libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                      पूर्णाङ्क no_new_privileges, bool no_exec, libcrun_error_t *err);
पूर्णाङ्क libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **rlimits, माप_प्रकार len,
                         libcrun_error_t *err);
पूर्णाङ्क libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
पूर्णाङ्क libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
पूर्णाङ्क libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err);
पूर्णाङ्क libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err);
पूर्णाङ्क libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err);
पूर्णाङ्क libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err);
पूर्णाङ्क libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err);
पूर्णाङ्क libcrun_join_process (libcrun_context_t *context, libcrun_container_t *container, pid_t pid_to_join,
                          libcrun_container_status_t *status, ध्रुव वर्ण *cgroup, पूर्णाङ्क detach,
                          runtime_spec_schema_config_schema_process *process, पूर्णाङ्क *terminal_fd, libcrun_error_t *err);
पूर्णाङ्क libcrun_linux_container_update (libcrun_container_status_t *status,
                                    ध्रुव वर्ण *state_मूल,
                                    runtime_spec_schema_config_linux_resources *resources,
                                    libcrun_error_t *err);
पूर्णाङ्क libcrun_create_keyring (libcrun_container_t *container, ध्रुव वर्ण *name, ध्रुव वर्ण *label, libcrun_error_t *err);
पूर्णाङ्क libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err);
पूर्णाङ्क libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err);
पूर्णाङ्क libcrun_container_do_bind_mount (libcrun_container_t *container, वर्ण *mount_source, वर्ण *mount_destination,
                                     वर्ण **mount_options, माप_प्रकार mount_options_len, libcrun_error_t *err);
पूर्णाङ्क libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err);
पूर्णाङ्क libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err);
पूर्णाङ्क libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err);

पूर्णाङ्क libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                        libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

पूर्णाङ्क libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                     libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

पूर्णाङ्क libcrun_find_namespace (ध्रुव वर्ण *name);
वर्ण *libcrun_get_external_descriptors (libcrun_container_t *container);
पूर्णाङ्क libcrun_container_setgroups (libcrun_container_t *container,
                                 runtime_spec_schema_config_schema_process *process,
                                 libcrun_error_t *err);
पूर्णाङ्क libcrun_kill_linux (libcrun_container_status_t *status, पूर्णाङ्क सङ्केत, libcrun_error_t *err);
पूर्णाङ्क libcrun_create_final_userns (libcrun_container_t *container, libcrun_error_t *err);
पूर्णाङ्क libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err);

पूर्णाङ्क libcrun_create_dev (libcrun_container_t *container, पूर्णाङ्क devfd,
                        पूर्णाङ्क srcfd, संरचना device_s *device, bool binds,
                        bool ensure_parent_dir, libcrun_error_t *err);

पूर्णाङ्क parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, वर्ण *option, वर्ण **out,
                                 माप_प्रकार *len, libcrun_error_t *err);

गणना
{
  LIBCRUN_INTELRDT_CREATE = (1 << 0),
  LIBCRUN_INTELRDT_UPDATE = (1 << 1),
  LIBCRUN_INTELRDT_MOVE = (1 << 2),
};

#define LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE (LIBCRUN_INTELRDT_CREATE | LIBCRUN_INTELRDT_UPDATE | LIBCRUN_INTELRDT_MOVE)

पूर्णाङ्क libcrun_apply_intelrdt (ध्रुव वर्ण *ctr_name, libcrun_container_t *container, pid_t pid, पूर्णाङ्क actions, libcrun_error_t *err);

पूर्णाङ्क libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);

पूर्णाङ्क libcrun_destroy_intelrdt (ध्रुव वर्ण *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err);

पूर्णाङ्क libcrun_update_intel_rdt (ध्रुव वर्ण *ctr_name, libcrun_container_t *container, ध्रुव वर्ण *l3_cache_schema, ध्रुव वर्ण *mem_bw_schema, वर्ण *ध्रुव *schemata, libcrun_error_t *err);

पूर्णाङ्क libcrun_safe_chdir (ध्रुव वर्ण *path, libcrun_error_t *err);

पूर्णाङ्क get_bind_mount (पूर्णाङ्क dirfd, ध्रुव वर्ण *src, bool recursive, bool rdonly, bool nofollow, अचिन्हित दीर्घ propagation, libcrun_error_t *err);

bool is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow);

पूर्णाङ्क libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, माप_प्रकार len, libcrun_error_t *err);

पूर्णाङ्क libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, माप_प्रकार len, libcrun_error_t *err);

संरचना libcrun_mount_flags_s
{
  वर्ण *target;
  अचिन्हित दीर्घ flags;
};

पूर्णाङ्क libcrun_restrict_mounts_in_container (pid_t pid, संरचना libcrun_mount_flags_s *mounts, माप_प्रकार len, libcrun_error_t *err);

#endif
