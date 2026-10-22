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
#अगर_अघोषित LINUX_H
#घोषणा LINUX_H
#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश "error.h"
#समावेश <errno.h>
#समावेश <argp.h>
#समावेश <sys/syscall.h>
#समावेश <ocispec/runtime_spec_schema_config_schema.h>
#समावेश "container.h"
#समावेश "status.h"

काष्ठा device_s
{
  स्थिर अक्षर *path;
  अक्षर *type;
  पूर्णांक major;
  पूर्णांक minor;
  पूर्णांक mode;
  uid_t uid;
  gid_t gid;
};

अटल अंतरभूत पूर्णांक
syscall_clone (अचिन्हित दीर्घ flags, व्योम *child_ढेर)
{
#अगर defined __s390__ || defined __CRIS__
  वापस (पूर्णांक) syscall (__NR_clone, child_ढेर, flags);
#अन्यथा
  वापस (पूर्णांक) syscall (__NR_clone, flags, child_ढेर);
#पूर्ण_अगर
}

प्रकार पूर्णांक (*container_entrypoint_t) (व्योम *args, अक्षर *notify_socket, पूर्णांक sync_socket, libcrun_error_t *err);

प्रकार पूर्णांक (*set_mounts_cb_t) (व्योम *args, libcrun_error_t *err);

काष्ठा libcrun_dirfd_s
{
  पूर्णांक *dirfd;
  bool joined;
};

pid_t libcrun_run_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, व्योम *args,
                                   पूर्णांक *sync_socket_out, काष्ठा libcrun_dirfd_s *dirfd, libcrun_error_t *err);
bool libcrun_container_delayed_pidns_fork (libcrun_container_t *container);
पूर्णांक libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err);
पूर्णांक get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, पूर्णांक *notify_socket_out,
                   libcrun_error_t *err);
पूर्णांक libcrun_set_mounts (काष्ठा container_entrypoint_s *args, libcrun_container_t *container, स्थिर अक्षर *rootfs,
                        set_mounts_cb_t cb, व्योम *cb_तथ्य, libcrun_error_t *err);
पूर्णांक libcrun_finalize_mounts (काष्ठा container_entrypoint_s *entrypoint_args, libcrun_container_t *container,
                             स्थिर अक्षर *rootfs, set_mounts_cb_t after_pivot_cb, व्योम *cb_तथ्य, libcrun_error_t *err);
पूर्णांक libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err);
पूर्णांक libcrun_do_pivot_मूल (libcrun_container_t *container, bool no_pivot, अक्षर **rootfs, libcrun_error_t *err);
पूर्णांक libcrun_reopen_dev_null (libcrun_error_t *err);
पूर्णांक libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);
पूर्णांक libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                      पूर्णांक no_new_privileges, bool no_exec, libcrun_error_t *err);
पूर्णांक libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **rlimits, माप_प्रकार len,
                         libcrun_error_t *err);
पूर्णांक libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
पूर्णांक libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
पूर्णांक libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err);
पूर्णांक libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err);
पूर्णांक libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err);
पूर्णांक libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err);
पूर्णांक libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err);
पूर्णांक libcrun_join_process (libcrun_context_t *context, libcrun_container_t *container, pid_t pid_to_join,
                          libcrun_container_status_t *status, स्थिर अक्षर *cgroup, पूर्णांक detach,
                          runtime_spec_schema_config_schema_process *process, पूर्णांक *terminal_fd, libcrun_error_t *err);
पूर्णांक libcrun_linux_container_update (libcrun_container_status_t *status,
                                    स्थिर अक्षर *state_मूल,
                                    runtime_spec_schema_config_linux_resources *resources,
                                    libcrun_error_t *err);
पूर्णांक libcrun_create_keyring (libcrun_container_t *container, स्थिर अक्षर *name, स्थिर अक्षर *label, libcrun_error_t *err);
पूर्णांक libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err);
पूर्णांक libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err);
पूर्णांक libcrun_container_do_bind_mount (libcrun_container_t *container, अक्षर *mount_source, अक्षर *mount_destination,
                                     अक्षर **mount_options, माप_प्रकार mount_options_len, libcrun_error_t *err);
पूर्णांक libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err);
पूर्णांक libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err);
पूर्णांक libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err);

पूर्णांक libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                        libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

पूर्णांक libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                     libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

पूर्णांक libcrun_find_namespace (स्थिर अक्षर *name);
अक्षर *libcrun_get_external_descriptors (libcrun_container_t *container);
पूर्णांक libcrun_container_setgroups (libcrun_container_t *container,
                                 runtime_spec_schema_config_schema_process *process,
                                 libcrun_error_t *err);
पूर्णांक libcrun_kill_linux (libcrun_container_status_t *status, पूर्णांक संकेत, libcrun_error_t *err);
पूर्णांक libcrun_create_final_userns (libcrun_container_t *container, libcrun_error_t *err);
पूर्णांक libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err);

पूर्णांक libcrun_create_dev (libcrun_container_t *container, पूर्णांक devfd,
                        पूर्णांक srcfd, काष्ठा device_s *device, bool binds,
                        bool ensure_parent_dir, libcrun_error_t *err);

पूर्णांक parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, अक्षर *option, अक्षर **out,
                                 माप_प्रकार *len, libcrun_error_t *err);

क्रमागत
{
  LIBCRUN_INTELRDT_CREATE = (1 << 0),
  LIBCRUN_INTELRDT_UPDATE = (1 << 1),
  LIBCRUN_INTELRDT_MOVE = (1 << 2),
};

#घोषणा LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE (LIBCRUN_INTELRDT_CREATE | LIBCRUN_INTELRDT_UPDATE | LIBCRUN_INTELRDT_MOVE)

पूर्णांक libcrun_apply_intelrdt (स्थिर अक्षर *ctr_name, libcrun_container_t *container, pid_t pid, पूर्णांक actions, libcrun_error_t *err);

पूर्णांक libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);

पूर्णांक libcrun_destroy_intelrdt (स्थिर अक्षर *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err);

पूर्णांक libcrun_update_intel_rdt (स्थिर अक्षर *ctr_name, libcrun_container_t *container, स्थिर अक्षर *l3_cache_schema, स्थिर अक्षर *mem_bw_schema, अक्षर *स्थिर *schemata, libcrun_error_t *err);

पूर्णांक libcrun_safe_chdir (स्थिर अक्षर *path, libcrun_error_t *err);

पूर्णांक get_bind_mount (पूर्णांक dirfd, स्थिर अक्षर *src, bool recursive, bool rdonly, bool nofollow, अचिन्हित दीर्घ propagation, libcrun_error_t *err);

bool is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow);

पूर्णांक libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, माप_प्रकार len, libcrun_error_t *err);

पूर्णांक libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, माप_प्रकार len, libcrun_error_t *err);

काष्ठा libcrun_mount_flags_s
{
  अक्षर *target;
  अचिन्हित दीर्घ flags;
};

पूर्णांक libcrun_restrict_mounts_in_container (pid_t pid, काष्ठा libcrun_mount_flags_s *mounts, माप_प्रकार len, libcrun_error_t *err);

#पूर्ण_अगर
