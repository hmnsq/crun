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
#جے_بنا_تعریف LINUX_H
#تعریف_کرو LINUX_H
#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن "error.h"
#ضمن <errno.h>
#ضمن <argp.h>
#ضمن <sys/syscall.h>
#ضمن <ocispec/runtime_spec_schema_config_schema.h>
#ضمن "container.h"
#ضمن "status.h"

بنية device_s
{
  ثابت حرف *رستہ;
  حرف *type;
  صحيح major;
  صحيح minor;
  صحيح منوال;
  uid_t uid;
  gid_t gid;
};

ساكن مضمن صحيح
syscall_clone (بنانشانی لما flags, فراغ *بچہ_ڈھیر)
{
#إذا defined __s390__ || defined __CRIS__
  أرجع (صحيح) syscall (__NR_clone, بچہ_ڈھیر, flags);
#وإلا
  أرجع (صحيح) syscall (__NR_clone, flags, بچہ_ڈھیر);
#جے_انت
}

تعريفنوع صحيح (*container_entrypoint_t) (فراغ *دلیلاں, حرف *notify_socket, صحيح sync_socket, libcrun_error_t *err);

تعريفنوع صحيح (*سیٹ_mounts_cb_t) (فراغ *دلیلاں, libcrun_error_t *err);

بنية libcrun_dirfd_s
{
  صحيح *dirfd;
  bool joined;
};

pid_t libcrun_لگاتار_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, فراغ *دلیلاں,
                                   صحيح *sync_socket_باہرلا, بنية libcrun_dirfd_s *dirfd, libcrun_error_t *err);
bool libcrun_container_delayed_pidns_fork (libcrun_container_t *container);
صحيح libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err);
صحيح get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, صحيح *notify_socket_باہرلا,
                   libcrun_error_t *err);
صحيح libcrun_سیٹ_mounts (بنية container_entrypoint_s *دلیلاں, libcrun_container_t *container, ثابت حرف *rootfs,
                        سیٹ_mounts_cb_t cb, فراغ *cb_انکڑے, libcrun_error_t *err);
صحيح libcrun_finalize_mounts (بنية container_entrypoint_s *entrypoint_دلیلاں, libcrun_container_t *container,
                             ثابت حرف *rootfs, سیٹ_mounts_cb_t after_دھرا_cb, فراغ *cb_انکڑے, libcrun_error_t *err);
صحيح libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err);
صحيح libcrun_do_دھرا_جڑ (libcrun_container_t *container, bool no_دھرا, حرف **rootfs, libcrun_error_t *err);
صحيح libcrun_reopen_dev_null (libcrun_error_t *err);
صحيح libcrun_سیٹ_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);
صحيح libcrun_سیٹ_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                      صحيح no_new_privileges, bool no_exec, libcrun_error_t *err);
صحيح libcrun_سیٹ_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **rlimits, size_t len,
                         libcrun_error_t *err);
صحيح libcrun_سیٹ_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
صحيح libcrun_سیٹ_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
صحيح libcrun_سیٹ_hostname (libcrun_container_t *container, libcrun_error_t *err);
صحيح libcrun_سیٹ_domainname (libcrun_container_t *container, libcrun_error_t *err);
صحيح libcrun_سیٹ_oom (libcrun_container_t *container, libcrun_error_t *err);
صحيح libcrun_سیٹ_sysctl (libcrun_container_t *container, libcrun_error_t *err);
صحيح libcrun_سیٹ_terminal (libcrun_container_t *container, libcrun_error_t *err);
صحيح libcrun_join_process (libcrun_context_t *context, libcrun_container_t *container, pid_t pid_منزل_join,
                          libcrun_container_حالت_t *حالت, ثابت حرف *cgroup, صحيح detach,
                          runtime_spec_schema_config_schema_process *process, صحيح *terminal_fd, libcrun_error_t *err);
صحيح libcrun_linux_container_update (libcrun_container_حالت_t *حالت,
                                    ثابت حرف *state_جڑ,
                                    runtime_spec_schema_config_linux_resources *resources,
                                    libcrun_error_t *err);
صحيح libcrun_create_keyring (libcrun_container_t *container, ثابت حرف *name, ثابت حرف *label, libcrun_error_t *err);
صحيح libcrun_container_pause_linux (libcrun_container_حالت_t *حالت, libcrun_error_t *err);
صحيح libcrun_container_unpause_linux (libcrun_container_حالت_t *حالت, libcrun_error_t *err);
صحيح libcrun_container_do_bind_mount (libcrun_container_t *container, حرف *mount_سروت, حرف *mount_destination,
                                     حرف **mount_options, size_t mount_options_len, libcrun_error_t *err);
صحيح libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err);
صحيح libcrun_سیٹ_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err);
صحيح libcrun_configure_جال (libcrun_container_t *container, libcrun_error_t *err);

صحيح libcrun_container_checkpoint_linux (libcrun_container_حالت_t *حالت, libcrun_container_t *container,
                                        libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

صحيح libcrun_container_restore_linux (libcrun_container_حالت_t *حالت, libcrun_container_t *container,
                                     libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

صحيح libcrun_لبھو_namespace (ثابت حرف *name);
حرف *libcrun_get_external_descriptors (libcrun_container_t *container);
صحيح libcrun_container_setgroups (libcrun_container_t *container,
                                 runtime_spec_schema_config_schema_process *process,
                                 libcrun_error_t *err);
صحيح libcrun_kill_linux (libcrun_container_حالت_t *حالت, صحيح إشارة, libcrun_error_t *err);
صحيح libcrun_create_final_userns (libcrun_container_t *container, libcrun_error_t *err);
صحيح libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err);

صحيح libcrun_create_dev (libcrun_container_t *container, صحيح devfd,
                        صحيح srcfd, بنية device_s *device, bool binds,
                        bool ensure_ماپا_dir, libcrun_error_t *err);

صحيح parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, حرف *option, حرف **باہرلا,
                                 size_t *len, libcrun_error_t *err);

تعداد
{
  LIBCRUN_INTELRDT_CREATE = (1 << 0),
  LIBCRUN_INTELRDT_UPDATE = (1 << 1),
  LIBCRUN_INTELRDT_MOVE = (1 << 2),
};

#تعریف_کرو LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE (LIBCRUN_INTELRDT_CREATE | LIBCRUN_INTELRDT_UPDATE | LIBCRUN_INTELRDT_MOVE)

صحيح libcrun_apply_intelrdt (ثابت حرف *ctr_name, libcrun_container_t *container, pid_t pid, صحيح actions, libcrun_error_t *err);

صحيح libcrun_move_جال_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);

صحيح libcrun_destroy_intelrdt (ثابت حرف *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err);

صحيح libcrun_update_intel_rdt (ثابت حرف *ctr_name, libcrun_container_t *container, ثابت حرف *l3_cache_schema, ثابت حرف *mem_bw_schema, حرف *ثابت *schemata, libcrun_error_t *err);

صحيح libcrun_محفوظ_chdir (ثابت حرف *رستہ, libcrun_error_t *err);

صحيح get_bind_mount (صحيح dirfd, ثابت حرف *src, bool recursive, bool rdonly, bool nofollow, بنانشانی لما پسار, libcrun_error_t *err);

bool is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow);

صحيح libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_حالت_t *حالت, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

صحيح libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_حالت_t *حالت, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

بنية libcrun_mount_flags_s
{
  حرف *نشانہ;
  بنانشانی لما flags;
};

صحيح libcrun_restrict_mounts_وچ_container (pid_t pid, بنية libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err);

#جے_انت
