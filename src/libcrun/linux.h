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
#اگر_غیر_تعریف_شدہ LINUX_H
#تعریف_کرو LINUX_H
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

ساخت device_s
{
  ثابت حرف *راستہ;
  حرف *type;
  صحیح major;
  صحیح minor;
  صحیح منوال;
  uid_t uid;
  gid_t gid;
};

ساکن اندرونی صحیح
syscall_clone (غیرعلامتی طویل flags, خالی *بچہ_ڈھیر)
{
#اگر defined __s390__ || defined __CRIS__
  واپس (صحیح) syscall (__NR_clone, بچہ_ڈھیر, flags);
#ورنہ
  واپس (صحیح) syscall (__NR_clone, flags, بچہ_ڈھیر);
#اگر_ختم
}

قسمتعریف صحیح (*container_entrypoint_t) (خالی *دلائل, حرف *notify_socket, صحیح sync_socket, libcrun_error_t *err);

قسمتعریف صحیح (*سیٹ_mounts_cb_t) (خالی *دلائل, libcrun_error_t *err);

ساخت libcrun_dirfd_s
{
  صحیح *dirfd;
  bool joined;
};

pid_t libcrun_تسلسل_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, خالی *دلائل,
                                   صحیح *sync_socket_برآمد, ساخت libcrun_dirfd_s *dirfd, libcrun_error_t *err);
bool libcrun_container_delayed_pidns_fork (libcrun_container_t *container);
صحیح libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err);
صحیح get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, صحیح *notify_socket_برآمد,
                   libcrun_error_t *err);
صحیح libcrun_سیٹ_mounts (ساخت container_entrypoint_s *دلائل, libcrun_container_t *container, ثابت حرف *rootfs,
                        سیٹ_mounts_cb_t cb, خالی *cb_ڈیٹا, libcrun_error_t *err);
صحیح libcrun_finalize_mounts (ساخت container_entrypoint_s *entrypoint_دلائل, libcrun_container_t *container,
                             ثابت حرف *rootfs, سیٹ_mounts_cb_t after_محور_cb, خالی *cb_ڈیٹا, libcrun_error_t *err);
صحیح libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err);
صحیح libcrun_do_محور_جڑ (libcrun_container_t *container, bool no_محور, حرف **rootfs, libcrun_error_t *err);
صحیح libcrun_reopen_dev_null (libcrun_error_t *err);
صحیح libcrun_سیٹ_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);
صحیح libcrun_سیٹ_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                      صحیح no_new_privileges, bool no_exec, libcrun_error_t *err);
صحیح libcrun_سیٹ_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **rlimits, size_t len,
                         libcrun_error_t *err);
صحیح libcrun_سیٹ_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
صحیح libcrun_سیٹ_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err);
صحیح libcrun_سیٹ_hostname (libcrun_container_t *container, libcrun_error_t *err);
صحیح libcrun_سیٹ_domainname (libcrun_container_t *container, libcrun_error_t *err);
صحیح libcrun_سیٹ_oom (libcrun_container_t *container, libcrun_error_t *err);
صحیح libcrun_سیٹ_sysctl (libcrun_container_t *container, libcrun_error_t *err);
صحیح libcrun_سیٹ_terminal (libcrun_container_t *container, libcrun_error_t *err);
صحیح libcrun_join_process (libcrun_context_t *context, libcrun_container_t *container, pid_t pid_مقصد_join,
                          libcrun_container_حالت_t *حالت_2, ثابت حرف *cgroup, صحیح detach,
                          runtime_spec_schema_config_schema_process *process, صحیح *terminal_fd, libcrun_error_t *err);
صحیح libcrun_linux_container_update (libcrun_container_حالت_t *حالت_2,
                                    ثابت حرف *state_جڑ,
                                    runtime_spec_schema_config_linux_resources *resources,
                                    libcrun_error_t *err);
صحیح libcrun_create_keyring (libcrun_container_t *container, ثابت حرف *name, ثابت حرف *label, libcrun_error_t *err);
صحیح libcrun_container_pause_linux (libcrun_container_حالت_t *حالت_2, libcrun_error_t *err);
صحیح libcrun_container_unpause_linux (libcrun_container_حالت_t *حالت_2, libcrun_error_t *err);
صحیح libcrun_container_do_bind_mount (libcrun_container_t *container, حرف *mount_ماخذ, حرف *mount_destination,
                                     حرف **mount_options, size_t mount_options_len, libcrun_error_t *err);
صحیح libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err);
صحیح libcrun_سیٹ_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err);
صحیح libcrun_configure_جال (libcrun_container_t *container, libcrun_error_t *err);

صحیح libcrun_container_checkpoint_linux (libcrun_container_حالت_t *حالت_2, libcrun_container_t *container,
                                        libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

صحیح libcrun_container_restore_linux (libcrun_container_حالت_t *حالت_2, libcrun_container_t *container,
                                     libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

صحیح libcrun_ڈھونڈو_namespace (ثابت حرف *name);
حرف *libcrun_get_external_descriptors (libcrun_container_t *container);
صحیح libcrun_container_setgroups (libcrun_container_t *container,
                                 runtime_spec_schema_config_schema_process *process,
                                 libcrun_error_t *err);
صحیح libcrun_kill_linux (libcrun_container_حالت_t *حالت_2, صحیح اشارہ, libcrun_error_t *err);
صحیح libcrun_create_final_userns (libcrun_container_t *container, libcrun_error_t *err);
صحیح libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err);

صحیح libcrun_create_dev (libcrun_container_t *container, صحیح devfd,
                        صحیح srcfd, ساخت device_s *device, bool binds,
                        bool ensure_والد_dir, libcrun_error_t *err);

صحیح parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, حرف *option, حرف **برآمد,
                                 size_t *len, libcrun_error_t *err);

شمار
{
  LIBCRUN_INTELRDT_CREATE = (1 << 0),
  LIBCRUN_INTELRDT_UPDATE = (1 << 1),
  LIBCRUN_INTELRDT_MOVE = (1 << 2),
};

#تعریف_کرو LIBCRUN_INTELRDT_CREATE_UPDATE_MOVE (LIBCRUN_INTELRDT_CREATE | LIBCRUN_INTELRDT_UPDATE | LIBCRUN_INTELRDT_MOVE)

صحیح libcrun_apply_intelrdt (ثابت حرف *ctr_name, libcrun_container_t *container, pid_t pid, صحیح actions, libcrun_error_t *err);

صحیح libcrun_move_جال_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err);

صحیح libcrun_destroy_intelrdt (ثابت حرف *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err);

صحیح libcrun_update_intel_rdt (ثابت حرف *ctr_name, libcrun_container_t *container, ثابت حرف *l3_cache_schema, ثابت حرف *mem_bw_schema, حرف *ثابت *schemata, libcrun_error_t *err);

صحیح libcrun_محفوظ_chdir (ثابت حرف *راستہ, libcrun_error_t *err);

صحیح get_bind_mount (صحیح dirfd, ثابت حرف *src, bool recursive, bool rdonly, bool nofollow, غیرعلامتی طویل اشاعت, libcrun_error_t *err);

bool is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow);

صحیح libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_حالت_t *حالت_2, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

صحیح libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_حالت_t *حالت_2, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err);

ساخت libcrun_mount_flags_s
{
  حرف *ہدف;
  غیرعلامتی طویل flags;
};

صحیح libcrun_restrict_mounts_اندر_container (pid_t pid, ساخت libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err);

#اگر_ختم
