/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019 Giuseppe Scrivano <giuseppe@scrivano.org>
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

#اگر_غیر_تعریف_شدہ CONTAINER_H
#تعریف_کرو CONTAINER_H

#شامل <config.h>
#شامل <ocispec/runtime_spec_schema_config_schema.h>
#شامل "error.h"
#شامل "string_map.h"

شمار handler_configure_phase
{
  HANDLER_CONFIGURE_BEFORE_MOUNTS = 1,
  HANDLER_CONFIGURE_AFTER_MOUNTS,
  HANDLER_CONFIGURE_MOUNTS,
  HANDLER_CONFIGURE_BEFORE_USERNS
};

ساخت custom_handler_manager_s;

ساخت libcrun_context_s
{
  ثابت حرف *state_جڑ;
  ثابت حرف *id;
  ثابت حرف *bundle;
  ثابت حرف *console_socket;
  ثابت حرف *pid_file;
  ثابت حرف *notify_socket;
  ثابت حرف *handler;
  حرف *owned_state_جڑ;
  حرف *owned_id;
  حرف *owned_bundle;
  حرف *owned_console_socket;
  حرف *owned_pid_file;
  حرف *owned_notify_socket;
  حرف *owned_handler;
  صحیح preserve_fds;
  // For some use-cases we need differentiation between preserve_fds and listen_fds.
  // Following context variable makes sure we get exact value of listen_fds irrespective of preserve_fds.
  صحیح listen_fds;

  crun_output_handler output_handler;
  خالی *output_handler_arg;

  صحیح fifo_exec_wait_fd;

  bool systemd_cgroup;
  bool detach;
  bool no_new_keyring;
  bool force_no_cgroup;
  bool no_محور;
  bool no_open_درخت_namespace;

  حرف **argv;
  صحیح argc;

  ساخت custom_handler_manager_s *handler_manager;
};

شمار
{
  LIBCRUN_تسلسل_OPTIONS_PREFORK = 1 << 0,
  LIBCRUN_تسلسل_OPTIONS_KEEP = 1 << 1,
};

شمار
{
  LIBCRUN_CREATE_OPTIONS_PREFORK = 1 << 0,
};

ساخت libcrun_container_s
{
  /* Container parsed from the runtime json file.  */
  runtime_spec_schema_config_schema *container_def;

  uid_t host_uid;
  gid_t host_gid;

  /* Whether crun has CAP_SYS_PTRACE in its user namespace.  */
  bool host_has_cap_sys_ptrace;

  uid_t container_uid;
  gid_t container_gid;

  حرف *config_file;
  حرف *config_file_content;

  string_map *annotations;

  صحیح proc_fd;

  خالی *نجی_ڈیٹا;
  خالی (*cleanup_نجی_ڈیٹا) (خالی *نجی_ڈیٹا);
  ساخت libcrun_context_s *context;
};

ساخت libcrun_container_حالت_s;
قسمتعریف ساخت libcrun_container_حالت_s libcrun_container_حالت_t;

قسمتعریف ساخت libcrun_container_s libcrun_container_t;
قسمتعریف ساخت libcrun_context_s libcrun_context_t;

ساخت container_entrypoint_s;
ساخت cgroup_info_s
{
  bool v1;
  bool v2;
  bool systemd;
  bool systemd_user;
};

ساخت seccomp_info_s
{
  bool enabled;
  حرف **actions;
  حرف **operators;
  حرف **archs;
};

ساخت apparmor_info_s
{
  bool enabled;
};

ساخت selinux_info_s
{
  bool enabled;
};

ساخت idmap_info_s
{
  bool enabled;
};

ساخت intel_rdt_s
{
  bool enabled;
};

ساخت net_devices_s
{
  bool enabled;
};

ساخت mount_ext_info_s
{
  ساخت idmap_info_s idmap;
};

ساخت حافظہ_policy_info_s
{
  حرف **منوال;
  حرف **flags;
};

ساخت linux_info_s
{
  حرف **namespaces;
  حرف **capabilities;
  ساخت cgroup_info_s cgroup;
  ساخت seccomp_info_s seccomp;
  ساخت apparmor_info_s apparmor;
  ساخت selinux_info_s selinux;
  ساخت mount_ext_info_s mount_ext;
  ساخت intel_rdt_s intel_rdt;
  ساخت net_devices_s net_devices;
  ساخت حافظہ_policy_info_s حافظہ_policy;
};

ساخت annotations_info_s
{
  حرف *io_github_seccomp_libseccomp_نسخہ;
  bool تسلسل_oci_crun_checkpoint_enabled;
  حرف *تسلسل_oci_crun_commit;
  حرف *تسلسل_oci_crun_نسخہ;
  bool تسلسل_oci_crun_wasm;
};

ساخت features_info_s
{
  حرف *oci_نسخہ_min;
  حرف *oci_نسخہ_max;
  حرف **hooks;
  حرف **mount_options;
  ساخت linux_info_s linux;
  ساخت annotations_info_s annotations;
  حرف **potentially_unsafe_annotations;
};

ساخت libcrun_checkpoint_restore_s
{
  حرف *image_راستہ;
  حرف *work_راستہ;
  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool shell_کام;
  bool ext_unix_sk;
  bool detach;
  bool file_locks;
  ثابت حرف *console_socket;
  حرف *والد_راستہ;
  bool pre_dump;
  صحیح manage_cgroups_منوال;
  صحیح جال_تالا_طریقہ;
  حرف *lsm_profile;
  حرف *lsm_mount_context;
};
قسمتعریف ساخت libcrun_checkpoint_restore_s libcrun_checkpoint_restore_t;

/* Public checkpoint/restore options.  Mirror of the definition in
   <libcrun.h>; keep the two in sync.  */
شمار libcrun_cr_cgroups_منوال
{
  LIBCRUN_CR_CG_منوال_DEFAULT = 0, /* soft, matching runc */
  LIBCRUN_CR_CG_منوال_SOFT,
  LIBCRUN_CR_CG_منوال_IGNORE,
  LIBCRUN_CR_CG_منوال_بھرا,
  LIBCRUN_CR_CG_منوال_STRICT,
};

شمار libcrun_cr_جال_تالا_طریقہ
{
  LIBCRUN_CR_جال_تالا_DEFAULT = 0, /* iptables */
  LIBCRUN_CR_جال_تالا_IPTABLES,
  LIBCRUN_CR_جال_تالا_NFTABLES,
  LIBCRUN_CR_جال_تالا_SKIP,
};

ساخت libcrun_checkpoint_restore_options_s
{
  size_t struct_سائز;

  ثابت حرف *image_راستہ;
  ثابت حرف *work_راستہ;
  ثابت حرف *والد_راستہ;
  ثابت حرف *console_socket;
  ثابت حرف *lsm_profile;
  ثابت حرف *lsm_mount_context;

  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool ext_unix_sk;
  bool shell_کام;
  bool file_locks;
  bool pre_dump;
  bool detach;

  صحیح manage_cgroups_منوال; /* enum libcrun_cr_cgroups_mode */
  صحیح جال_تالا_طریقہ; /* enum libcrun_cr_network_lock_method */
};

LIBCRUN_عوامی libcrun_container_t *libcrun_container_بھرو_from_file (ثابت حرف *راستہ, libcrun_error_t *err);

LIBCRUN_عوامی libcrun_container_t *libcrun_container_بھرو_from_حافظہ (ثابت حرف *json, libcrun_error_t *err);

LIBCRUN_عوامی خالی libcrun_container_free (libcrun_container_t *);

/* Return the configuration of CONTAINER as a JSON string owned by CONTAINER.
   The document is generated on demand and cached, so the first call mutates
   CONTAINER and must not race with other users of the same handle.  */
LIBCRUN_عوامی ثابت حرف *libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err);
LIBCRUN_عوامی ثابت حرف *libcrun_container_get_annotation (libcrun_container_t *container, ثابت حرف *کلید);
LIBCRUN_عوامی size_t libcrun_container_get_annotations_len (libcrun_container_t *container);
LIBCRUN_عوامی صحیح libcrun_container_get_annotation_at (libcrun_container_t *container, size_t اشاریہ,
                                                        ثابت حرف **کلید, ثابت حرف **قدر);
LIBCRUN_عوامی uid_t libcrun_container_get_uid (libcrun_container_t *container);
LIBCRUN_عوامی gid_t libcrun_container_get_gid (libcrun_container_t *container);

LIBCRUN_عوامی libcrun_context_t *libcrun_context_new (ثابت حرف *id, ثابت حرف *state_جڑ, libcrun_error_t *err);
LIBCRUN_عوامی خالی libcrun_context_free (libcrun_context_t *ctx);

LIBCRUN_عوامی خالی libcrun_context_سیٹ_id (libcrun_context_t *ctx, ثابت حرف *قدر);
LIBCRUN_عوامی خالی libcrun_context_سیٹ_state_جڑ (libcrun_context_t *ctx, ثابت حرف *قدر);
LIBCRUN_عوامی ثابت حرف *libcrun_context_get_state_جڑ (libcrun_context_t *ctx);
LIBCRUN_عوامی خالی libcrun_context_سیٹ_bundle (libcrun_context_t *ctx, ثابت حرف *قدر);
LIBCRUN_عوامی خالی libcrun_context_سیٹ_console_socket (libcrun_context_t *ctx, ثابت حرف *قدر);
LIBCRUN_عوامی خالی libcrun_context_سیٹ_pid_file (libcrun_context_t *ctx, ثابت حرف *قدر);
LIBCRUN_عوامی ثابت حرف *libcrun_context_get_pid_file (libcrun_context_t *ctx);
LIBCRUN_عوامی خالی libcrun_context_سیٹ_notify_socket (libcrun_context_t *ctx, ثابت حرف *قدر);
LIBCRUN_عوامی خالی libcrun_context_سیٹ_handler (libcrun_context_t *ctx, ثابت حرف *قدر);
LIBCRUN_عوامی خالی libcrun_context_سیٹ_preserve_fds (libcrun_context_t *ctx, صحیح preserve_fds);
LIBCRUN_عوامی صحیح libcrun_context_get_preserve_fds (libcrun_context_t *ctx);
LIBCRUN_عوامی خالی libcrun_context_سیٹ_listen_fds (libcrun_context_t *ctx, صحیح listen_fds);
LIBCRUN_عوامی خالی libcrun_context_سیٹ_دلائل (libcrun_context_t *ctx, صحیح argc, حرف **argv);
LIBCRUN_عوامی خالی libcrun_context_سیٹ_systemd_cgroup (libcrun_context_t *ctx, bool قدر);
LIBCRUN_عوامی خالی libcrun_context_سیٹ_detach (libcrun_context_t *ctx, bool قدر);
LIBCRUN_عوامی خالی libcrun_context_سیٹ_no_new_keyring (libcrun_context_t *ctx, bool قدر);
LIBCRUN_عوامی خالی libcrun_context_سیٹ_no_محور (libcrun_context_t *ctx, bool قدر);
LIBCRUN_عوامی خالی libcrun_context_سیٹ_force_no_cgroup (libcrun_context_t *ctx, bool قدر);
LIBCRUN_عوامی خالی libcrun_context_سیٹ_output_handler (libcrun_context_t *ctx, crun_output_handler handler, خالی *arg);

LIBCRUN_عوامی صحیح libcrun_context_init_logging (libcrun_context_t *ctx, ثابت حرف *لوگارتھم, libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_container_تسلسل (libcrun_context_t *context, libcrun_container_t *container,
                                          غیرعلامتی صحیح options, libcrun_error_t *error);

LIBCRUN_عوامی صحیح libcrun_container_delete (libcrun_context_t *context, ثابت حرف *id, bool force,
                                             libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_container_kill (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *اشارہ,
                                           libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_container_killall (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *اشارہ,
                                              libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container,
                                             غیرعلامتی صحیح options, libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_container_آغاز (libcrun_context_t *context, ثابت حرف *id, libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_container_state (libcrun_context_t *context, ثابت حرف *id, FILE *برآمد,
                                            libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_container_state_json (libcrun_context_t *context, ثابت حرف *id, حرف **برآمد,
                                                 libcrun_error_t *err);

صحیح libcrun_container_notify_handler (ساخت container_entrypoint_s *دلائل,
                                      شمار handler_configure_phase phase,
                                      libcrun_container_t *container, ثابت حرف *rootfs,
                                      libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_get_container_state_string (ثابت حرف *id, libcrun_container_حالت_t *حالت_2,
                                                       ثابت حرف *state_جڑ, ثابت حرف **container_حالت,
                                                       صحیح *running, libcrun_error_t *err);

ساخت libcrun_container_exec_options_s
{
  size_t struct_سائز;
  runtime_spec_schema_config_schema_process *process;
  ثابت حرف *راستہ;
  ثابت حرف *cgroup;
  bool ملاؤ_env;
};

LIBCRUN_عوامی صحیح libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_بھرو_process_from_file (ثابت حرف *راستہ,
                                                   runtime_spec_schema_config_schema_process **process,
                                                   libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_container_exec_with_options (libcrun_context_t *context, ثابت حرف *id,
                                                        ساخت libcrun_container_exec_options_s *opts,
                                                        libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_container_exec (libcrun_context_t *context, ثابت حرف *id,
                                           runtime_spec_schema_config_schema_process *process, libcrun_error_t *err);

ساخت libcrun_exec_options_s
{
  size_t struct_سائز;
  ثابت حرف *process_json; /* OCI Process JSON */
  ثابت حرف *cgroup;
  bool ملاؤ_env;
};

LIBCRUN_عوامی صحیح libcrun_container_exec_json (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *process_json,
                                                libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_container_exec_with_options_json (libcrun_context_t *context, ثابت حرف *id,
                                                             ثابت ساخت libcrun_exec_options_s *opts,
                                                             libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_container_exec_process_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *راستہ,
                                                        libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_container_update (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *content,
                                             size_t len, libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_container_update_from_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *file,
                                                       libcrun_error_t *err);

ساخت libcrun_update_قدر_s
{
  ثابت حرف *section;
  ثابت حرف *name;
  bool numeric;
  ثابت حرف *قدر;
};

LIBCRUN_عوامی صحیح libcrun_container_update_from_اقدار (libcrun_context_t *context, ثابت حرف *id,
                                                         ساخت libcrun_update_قدر_s *اقدار, size_t len,
                                                         libcrun_error_t *err);

ساخت libcrun_intel_rdt_update
{
  ثابت حرف *l3_cache_schema;
  ثابت حرف *mem_bw_schema;
  حرف *ثابت *schemata;
};

LIBCRUN_عوامی صحیح libcrun_container_update_intel_rdt (libcrun_context_t *context, ثابت حرف *id,
                                                       ساخت libcrun_intel_rdt_update *update, libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_container_get_features (libcrun_context_t *context, ساخت features_info_s **info,
                                                   libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_container_get_features_json (libcrun_context_t *context, حرف **برآمد, libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_container_pause (libcrun_context_t *context, ثابت حرف *id, libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_container_unpause (libcrun_context_t *context, ثابت حرف *id, libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_container_checkpoint (libcrun_context_t *context, ثابت حرف *id,
                                                 ساخت libcrun_checkpoint_restore_options_s *cr_options,
                                                 libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_container_restore (libcrun_context_t *context, ثابت حرف *id,
                                              ساخت libcrun_checkpoint_restore_options_s *cr_options,
                                              libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_container_read_pids (libcrun_context_t *context, ثابت حرف *id, bool recurse, pid_t **pids, libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_write_json_containers_list (libcrun_context_t *context, FILE *برآمد, libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_container_list_json (libcrun_context_t *context, حرف **برآمد, libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_container_جوڑو_mounts_from_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *file,
                                                           libcrun_error_t *err);

LIBCRUN_عوامی صحیح libcrun_container_remove_mounts_from_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *file,
                                                              libcrun_error_t *err);

// Not part of the public API, just a method in container.c we need to access from linux.c
خالی get_جڑ_اندر_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid,
                             uid_t *uid, gid_t *gid);

ساکن اندرونی خالی
cleanup_containerp (libcrun_container_t **c)
{
  libcrun_container_t *container = *c;
  libcrun_container_free (container);
}

#تعریف_کرو cleanup_container __attribute__ ((cleanup (cleanup_containerp)))

ساکن اندرونی خالی
cleanup_struct_features_free (ساخت features_info_s **info)
{
  size_t i;
  ساخت features_info_s *ptr;
  اگر (info == تہی || *info == تہی)
    واپس;

  ptr = *info;

  // Free oci_version_min if it is not NULL
  اگر (ptr->oci_نسخہ_min != تہی)
    {
      آزاد (ptr->oci_نسخہ_min);
      ptr->oci_نسخہ_min = تہی; // Set to NULL after freeing
    }

  // Free oci_version_max if it is not NULL
  اگر (ptr->oci_نسخہ_max != تہی)
    {
      آزاد (ptr->oci_نسخہ_max);
      ptr->oci_نسخہ_max = تہی; // Set to NULL after freeing
    }
  اگر (ptr->hooks != تہی)
    {
      برائے (i = 0; ptr->hooks[i] != تہی; i++)
        آزاد (ptr->hooks[i]);
      آزاد (ptr->hooks);
    }
  اگر (ptr->mount_options != تہی)
    {
      برائے (i = 0; ptr->mount_options[i] != تہی; i++)
        آزاد (ptr->mount_options[i]);
      آزاد (ptr->mount_options);
    }
  اگر (ptr->linux.namespaces != تہی)
    {
      برائے (i = 0; ptr->linux.namespaces[i] != تہی; i++)
        آزاد (ptr->linux.namespaces[i]);
      آزاد (ptr->linux.namespaces);
    }

  اگر (ptr->linux.capabilities != تہی)
    {
      برائے (size_t i = 0; ptr->linux.capabilities[i] != تہی; i++)
        آزاد (ptr->linux.capabilities[i]);
      آزاد (ptr->linux.capabilities);
    }
  اگر (ptr->linux.seccomp.actions != تہی)
    {
      برائے (i = 0; ptr->linux.seccomp.actions[i] != تہی; i++)
        آزاد (ptr->linux.seccomp.actions[i]);
      آزاد (ptr->linux.seccomp.actions);
    }
  اگر (ptr->linux.seccomp.operators != تہی)
    {
      برائے (i = 0; ptr->linux.seccomp.operators[i] != تہی; i++)
        آزاد (ptr->linux.seccomp.operators[i]);
      آزاد (ptr->linux.seccomp.operators);
    }

  آزاد (ptr);
  *info = تہی;
}

#تعریف_کرو cleanup_struct_features __attribute__ ((cleanup (cleanup_struct_features_free)))

#اگر_ختم
