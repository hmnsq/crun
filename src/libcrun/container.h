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

#ifndef CONTAINER_H
#define CONTAINER_H

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <ocispec/runtime_spec_schema_config_schema.h>
#அந்தர்பாவ "error.h"
#அந்தர்பாவ "string_map.h"

எண்ணிக்கை handler_configure_phase
{
  HANDLER_CONFIGURE_BEFORE_MOUNTS = 1,
  HANDLER_CONFIGURE_AFTER_MOUNTS,
  HANDLER_CONFIGURE_MOUNTS,
  HANDLER_CONFIGURE_BEFORE_USERNS
};

கட்டமைப்பு custom_handler_manager_s;

கட்டமைப்பு libcrun_context_s
{
  மாறிலி எழுத்து *state_வேர்;
  மாறிலி எழுத்து *id;
  மாறிலி எழுத்து *bundle;
  மாறிலி எழுத்து *console_socket;
  மாறிலி எழுத்து *pid_file;
  மாறிலி எழுத்து *notify_socket;
  மாறிலி எழுத்து *handler;
  எழுத்து *owned_state_வேர்;
  எழுத்து *owned_id;
  எழுத்து *owned_bundle;
  எழுத்து *owned_console_socket;
  எழுத்து *owned_pid_file;
  எழுத்து *owned_notify_socket;
  எழுத்து *owned_handler;
  முழு preserve_fds;
  // For some use-cases we need differentiation between preserve_fds and listen_fds.
  // Following context variable makes sure we get exact value of listen_fds irrespective of preserve_fds.
  முழு listen_fds;

  crun_output_handler output_handler;
  வெறுமை *output_handler_arg;

  முழு fifo_exec_wait_fd;

  bool systemd_cgroup;
  bool detach;
  bool no_new_keyring;
  bool force_no_cgroup;
  bool no_pivot;
  bool no_open_மரம்_namespace;

  எழுத்து **argv;
  முழு argc;

  கட்டமைப்பு custom_handler_manager_s *handler_manager;
};

எண்ணிக்கை
{
  LIBCRUN_RUN_OPTIONS_PREFORK = 1 << 0,
  LIBCRUN_RUN_OPTIONS_KEEP = 1 << 1,
};

எண்ணிக்கை
{
  LIBCRUN_CREATE_OPTIONS_PREFORK = 1 << 0,
};

கட்டமைப்பு libcrun_container_s
{
  /* Container parsed from the runtime json file.  */
  runtime_spec_schema_config_schema *container_def;

  uid_t host_uid;
  gid_t host_gid;

  /* Whether crun has CAP_SYS_PTRACE in its user namespace.  */
  bool host_has_cap_sys_ptrace;

  uid_t container_uid;
  gid_t container_gid;

  எழுத்து *config_file;
  எழுத்து *config_file_content;

  string_map *annotations;

  முழு proc_fd;

  வெறுமை *private_data;
  வெறுமை (*cleanup_private_data) (வெறுமை *private_data);
  கட்டமைப்பு libcrun_context_s *context;
};

கட்டமைப்பு libcrun_container_status_s;
வகைவரை கட்டமைப்பு libcrun_container_status_s libcrun_container_status_t;

வகைவரை கட்டமைப்பு libcrun_container_s libcrun_container_t;
வகைவரை கட்டமைப்பு libcrun_context_s libcrun_context_t;

கட்டமைப்பு container_entrypoint_s;
கட்டமைப்பு cgroup_info_s
{
  bool v1;
  bool v2;
  bool systemd;
  bool systemd_user;
};

கட்டமைப்பு seccomp_info_s
{
  bool enabled;
  எழுத்து **actions;
  எழுத்து **operators;
  எழுத்து **archs;
};

கட்டமைப்பு apparmor_info_s
{
  bool enabled;
};

கட்டமைப்பு selinux_info_s
{
  bool enabled;
};

கட்டமைப்பு idmap_info_s
{
  bool enabled;
};

கட்டமைப்பு intel_rdt_s
{
  bool enabled;
};

கட்டமைப்பு net_devices_s
{
  bool enabled;
};

கட்டமைப்பு mount_ext_info_s
{
  கட்டமைப்பு idmap_info_s idmap;
};

கட்டமைப்பு memory_policy_info_s
{
  எழுத்து **mode;
  எழுத்து **flags;
};

கட்டமைப்பு linux_info_s
{
  எழுத்து **namespaces;
  எழுத்து **capabilities;
  கட்டமைப்பு cgroup_info_s cgroup;
  கட்டமைப்பு seccomp_info_s seccomp;
  கட்டமைப்பு apparmor_info_s apparmor;
  கட்டமைப்பு selinux_info_s selinux;
  கட்டமைப்பு mount_ext_info_s mount_ext;
  கட்டமைப்பு intel_rdt_s intel_rdt;
  கட்டமைப்பு net_devices_s net_devices;
  கட்டமைப்பு memory_policy_info_s memory_policy;
};

கட்டமைப்பு annotations_info_s
{
  எழுத்து *io_github_seccomp_libseccomp_version;
  bool run_oci_crun_checkpoint_enabled;
  எழுத்து *run_oci_crun_commit;
  எழுத்து *run_oci_crun_version;
  bool run_oci_crun_wasm;
};

கட்டமைப்பு features_info_s
{
  எழுத்து *oci_version_min;
  எழுத்து *oci_version_max;
  எழுத்து **hooks;
  எழுத்து **mount_options;
  கட்டமைப்பு linux_info_s linux;
  கட்டமைப்பு annotations_info_s annotations;
  எழுத்து **potentially_unsafe_annotations;
};

கட்டமைப்பு libcrun_checkpoint_restore_s
{
  எழுத்து *image_path;
  எழுத்து *work_path;
  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool shell_job;
  bool ext_unix_sk;
  bool detach;
  bool file_locks;
  மாறிலி எழுத்து *console_socket;
  எழுத்து *parent_path;
  bool pre_dump;
  முழு manage_cgroups_mode;
  முழு network_lock_method;
  எழுத்து *lsm_profile;
  எழுத்து *lsm_mount_context;
};
வகைவரை கட்டமைப்பு libcrun_checkpoint_restore_s libcrun_checkpoint_restore_t;

/* Public checkpoint/restore options.  Mirror of the definition in
   <libcrun.h>; keep the two in sync.  */
எண்ணிக்கை libcrun_cr_cgroups_mode
{
  LIBCRUN_CR_CG_MODE_DEFAULT = 0, /* soft, matching runc */
  LIBCRUN_CR_CG_MODE_SOFT,
  LIBCRUN_CR_CG_MODE_IGNORE,
  LIBCRUN_CR_CG_MODE_FULL,
  LIBCRUN_CR_CG_MODE_STRICT,
};

எண்ணிக்கை libcrun_cr_network_lock_method
{
  LIBCRUN_CR_NETWORK_LOCK_DEFAULT = 0, /* iptables */
  LIBCRUN_CR_NETWORK_LOCK_IPTABLES,
  LIBCRUN_CR_NETWORK_LOCK_NFTABLES,
  LIBCRUN_CR_NETWORK_LOCK_SKIP,
};

கட்டமைப்பு libcrun_checkpoint_restore_options_s
{
  size_t struct_size;

  மாறிலி எழுத்து *image_path;
  மாறிலி எழுத்து *work_path;
  மாறிலி எழுத்து *parent_path;
  மாறிலி எழுத்து *console_socket;
  மாறிலி எழுத்து *lsm_profile;
  மாறிலி எழுத்து *lsm_mount_context;

  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool ext_unix_sk;
  bool shell_job;
  bool file_locks;
  bool pre_dump;
  bool detach;

  முழு manage_cgroups_mode; /* enum libcrun_cr_cgroups_mode */
  முழு network_lock_method; /* enum libcrun_cr_network_lock_method */
};

LIBCRUN_PUBLIC libcrun_container_t *libcrun_container_load_from_file (மாறிலி எழுத்து *path, libcrun_error_t *err);

LIBCRUN_PUBLIC libcrun_container_t *libcrun_container_load_from_memory (மாறிலி எழுத்து *json, libcrun_error_t *err);

LIBCRUN_PUBLIC வெறுமை libcrun_container_free (libcrun_container_t *);

/* Return the configuration of CONTAINER as a JSON string owned by CONTAINER.
   The document is generated on demand and cached, so the first call mutates
   CONTAINER and must not race with other users of the same handle.  */
LIBCRUN_PUBLIC மாறிலி எழுத்து *libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err);
LIBCRUN_PUBLIC மாறிலி எழுத்து *libcrun_container_get_annotation (libcrun_container_t *container, மாறிலி எழுத்து *சாவி);
LIBCRUN_PUBLIC size_t libcrun_container_get_annotations_len (libcrun_container_t *container);
LIBCRUN_PUBLIC முழு libcrun_container_get_annotation_at (libcrun_container_t *container, size_t index,
                                                        மாறிலி எழுத்து **சாவி, மாறிலி எழுத்து **மதிப்பு);
LIBCRUN_PUBLIC uid_t libcrun_container_get_uid (libcrun_container_t *container);
LIBCRUN_PUBLIC gid_t libcrun_container_get_gid (libcrun_container_t *container);

LIBCRUN_PUBLIC libcrun_context_t *libcrun_context_new (மாறிலி எழுத்து *id, மாறிலி எழுத்து *state_வேர், libcrun_error_t *err);
LIBCRUN_PUBLIC வெறுமை libcrun_context_free (libcrun_context_t *ctx);

LIBCRUN_PUBLIC வெறுமை libcrun_context_set_id (libcrun_context_t *ctx, மாறிலி எழுத்து *மதிப்பு);
LIBCRUN_PUBLIC வெறுமை libcrun_context_set_state_வேர் (libcrun_context_t *ctx, மாறிலி எழுத்து *மதிப்பு);
LIBCRUN_PUBLIC மாறிலி எழுத்து *libcrun_context_get_state_வேர் (libcrun_context_t *ctx);
LIBCRUN_PUBLIC வெறுமை libcrun_context_set_bundle (libcrun_context_t *ctx, மாறிலி எழுத்து *மதிப்பு);
LIBCRUN_PUBLIC வெறுமை libcrun_context_set_console_socket (libcrun_context_t *ctx, மாறிலி எழுத்து *மதிப்பு);
LIBCRUN_PUBLIC வெறுமை libcrun_context_set_pid_file (libcrun_context_t *ctx, மாறிலி எழுத்து *மதிப்பு);
LIBCRUN_PUBLIC மாறிலி எழுத்து *libcrun_context_get_pid_file (libcrun_context_t *ctx);
LIBCRUN_PUBLIC வெறுமை libcrun_context_set_notify_socket (libcrun_context_t *ctx, மாறிலி எழுத்து *மதிப்பு);
LIBCRUN_PUBLIC வெறுமை libcrun_context_set_handler (libcrun_context_t *ctx, மாறிலி எழுத்து *மதிப்பு);
LIBCRUN_PUBLIC வெறுமை libcrun_context_set_preserve_fds (libcrun_context_t *ctx, முழு preserve_fds);
LIBCRUN_PUBLIC முழு libcrun_context_get_preserve_fds (libcrun_context_t *ctx);
LIBCRUN_PUBLIC வெறுமை libcrun_context_set_listen_fds (libcrun_context_t *ctx, முழு listen_fds);
LIBCRUN_PUBLIC வெறுமை libcrun_context_set_args (libcrun_context_t *ctx, முழு argc, எழுத்து **argv);
LIBCRUN_PUBLIC வெறுமை libcrun_context_set_systemd_cgroup (libcrun_context_t *ctx, bool மதிப்பு);
LIBCRUN_PUBLIC வெறுமை libcrun_context_set_detach (libcrun_context_t *ctx, bool மதிப்பு);
LIBCRUN_PUBLIC வெறுமை libcrun_context_set_no_new_keyring (libcrun_context_t *ctx, bool மதிப்பு);
LIBCRUN_PUBLIC வெறுமை libcrun_context_set_no_pivot (libcrun_context_t *ctx, bool மதிப்பு);
LIBCRUN_PUBLIC வெறுமை libcrun_context_set_force_no_cgroup (libcrun_context_t *ctx, bool மதிப்பு);
LIBCRUN_PUBLIC வெறுமை libcrun_context_set_output_handler (libcrun_context_t *ctx, crun_output_handler handler, வெறுமை *arg);

LIBCRUN_PUBLIC முழு libcrun_context_init_logging (libcrun_context_t *ctx, மாறிலி எழுத்து *log, libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_container_run (libcrun_context_t *context, libcrun_container_t *container,
                                          unsigned முழு options, libcrun_error_t *error);

LIBCRUN_PUBLIC முழு libcrun_container_delete (libcrun_context_t *context, மாறிலி எழுத்து *id, bool force,
                                             libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_container_kill (libcrun_context_t *context, மாறிலி எழுத்து *id, மாறிலி எழுத்து *சமிக்ஞை,
                                           libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_container_killall (libcrun_context_t *context, மாறிலி எழுத்து *id, மாறிலி எழுத்து *சமிக்ஞை,
                                              libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container,
                                             unsigned முழு options, libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_container_start (libcrun_context_t *context, மாறிலி எழுத்து *id, libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_container_state (libcrun_context_t *context, மாறிலி எழுத்து *id, FILE *out,
                                            libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_container_state_json (libcrun_context_t *context, மாறிலி எழுத்து *id, எழுத்து **out,
                                                 libcrun_error_t *err);

முழு libcrun_container_notify_handler (கட்டமைப்பு container_entrypoint_s *args,
                                      எண்ணிக்கை handler_configure_phase phase,
                                      libcrun_container_t *container, மாறிலி எழுத்து *rootfs,
                                      libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_get_container_state_string (மாறிலி எழுத்து *id, libcrun_container_status_t *status,
                                                       மாறிலி எழுத்து *state_வேர், மாறிலி எழுத்து **container_status,
                                                       முழு *running, libcrun_error_t *err);

கட்டமைப்பு libcrun_container_exec_options_s
{
  size_t struct_size;
  runtime_spec_schema_config_schema_process *process;
  மாறிலி எழுத்து *path;
  மாறிலி எழுத்து *cgroup;
  bool merge_env;
};

LIBCRUN_PUBLIC முழு libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_load_process_from_file (மாறிலி எழுத்து *path,
                                                   runtime_spec_schema_config_schema_process **process,
                                                   libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_container_exec_with_options (libcrun_context_t *context, மாறிலி எழுத்து *id,
                                                        கட்டமைப்பு libcrun_container_exec_options_s *opts,
                                                        libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_container_exec (libcrun_context_t *context, மாறிலி எழுத்து *id,
                                           runtime_spec_schema_config_schema_process *process, libcrun_error_t *err);

கட்டமைப்பு libcrun_exec_options_s
{
  size_t struct_size;
  மாறிலி எழுத்து *process_json; /* OCI Process JSON */
  மாறிலி எழுத்து *cgroup;
  bool merge_env;
};

LIBCRUN_PUBLIC முழு libcrun_container_exec_json (libcrun_context_t *context, மாறிலி எழுத்து *id, மாறிலி எழுத்து *process_json,
                                                libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_container_exec_with_options_json (libcrun_context_t *context, மாறிலி எழுத்து *id,
                                                             மாறிலி கட்டமைப்பு libcrun_exec_options_s *opts,
                                                             libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_container_exec_process_file (libcrun_context_t *context, மாறிலி எழுத்து *id, மாறிலி எழுத்து *path,
                                                        libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_container_update (libcrun_context_t *context, மாறிலி எழுத்து *id, மாறிலி எழுத்து *content,
                                             size_t len, libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_container_update_from_file (libcrun_context_t *context, மாறிலி எழுத்து *id, மாறிலி எழுத்து *file,
                                                       libcrun_error_t *err);

கட்டமைப்பு libcrun_update_மதிப்பு_s
{
  மாறிலி எழுத்து *section;
  மாறிலி எழுத்து *name;
  bool numeric;
  மாறிலி எழுத்து *மதிப்பு;
};

LIBCRUN_PUBLIC முழு libcrun_container_update_from_மதிப்புகள் (libcrun_context_t *context, மாறிலி எழுத்து *id,
                                                         கட்டமைப்பு libcrun_update_மதிப்பு_s *மதிப்புகள், size_t len,
                                                         libcrun_error_t *err);

கட்டமைப்பு libcrun_intel_rdt_update
{
  மாறிலி எழுத்து *l3_cache_schema;
  மாறிலி எழுத்து *mem_bw_schema;
  எழுத்து *மாறிலி *schemata;
};

LIBCRUN_PUBLIC முழு libcrun_container_update_intel_rdt (libcrun_context_t *context, மாறிலி எழுத்து *id,
                                                       கட்டமைப்பு libcrun_intel_rdt_update *update, libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_container_get_features (libcrun_context_t *context, கட்டமைப்பு features_info_s **info,
                                                   libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_container_get_features_json (libcrun_context_t *context, எழுத்து **out, libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_container_pause (libcrun_context_t *context, மாறிலி எழுத்து *id, libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_container_unpause (libcrun_context_t *context, மாறிலி எழுத்து *id, libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_container_checkpoint (libcrun_context_t *context, மாறிலி எழுத்து *id,
                                                 கட்டமைப்பு libcrun_checkpoint_restore_options_s *cr_options,
                                                 libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_container_restore (libcrun_context_t *context, மாறிலி எழுத்து *id,
                                              கட்டமைப்பு libcrun_checkpoint_restore_options_s *cr_options,
                                              libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_container_read_pids (libcrun_context_t *context, மாறிலி எழுத்து *id, bool recurse, pid_t **pids, libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_write_json_containers_list (libcrun_context_t *context, FILE *out, libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_container_list_json (libcrun_context_t *context, எழுத்து **out, libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_container_add_mounts_from_file (libcrun_context_t *context, மாறிலி எழுத்து *id, மாறிலி எழுத்து *file,
                                                           libcrun_error_t *err);

LIBCRUN_PUBLIC முழு libcrun_container_remove_mounts_from_file (libcrun_context_t *context, மாறிலி எழுத்து *id, மாறிலி எழுத்து *file,
                                                              libcrun_error_t *err);

// Not part of the public API, just a method in container.c we need to access from linux.c
வெறுமை get_வேர்_in_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid,
                             uid_t *uid, gid_t *gid);

நிலையான உள்வரி வெறுமை
cleanup_containerp (libcrun_container_t **c)
{
  libcrun_container_t *container = *c;
  libcrun_container_free (container);
}

#define cleanup_container __attribute__ ((cleanup (cleanup_containerp)))

நிலையான உள்வரி வெறுமை
cleanup_struct_features_free (கட்டமைப்பு features_info_s **info)
{
  size_t i;
  கட்டமைப்பு features_info_s *ptr;
  எனில் (info == வெற்று || *info == வெற்று)
    திரும்பு;

  ptr = *info;

  // Free oci_version_min if it is not NULL
  எனில் (ptr->oci_version_min != வெற்று)
    {
      விடுவி (ptr->oci_version_min);
      ptr->oci_version_min = வெற்று; // Set to NULL after freeing
    }

  // Free oci_version_max if it is not NULL
  எனில் (ptr->oci_version_max != வெற்று)
    {
      விடுவி (ptr->oci_version_max);
      ptr->oci_version_max = வெற்று; // Set to NULL after freeing
    }
  எனில் (ptr->hooks != வெற்று)
    {
      க்கு (i = 0; ptr->hooks[i] != வெற்று; i++)
        விடுவி (ptr->hooks[i]);
      விடுவி (ptr->hooks);
    }
  எனில் (ptr->mount_options != வெற்று)
    {
      க்கு (i = 0; ptr->mount_options[i] != வெற்று; i++)
        விடுவி (ptr->mount_options[i]);
      விடுவி (ptr->mount_options);
    }
  எனில் (ptr->linux.namespaces != வெற்று)
    {
      க்கு (i = 0; ptr->linux.namespaces[i] != வெற்று; i++)
        விடுவி (ptr->linux.namespaces[i]);
      விடுவி (ptr->linux.namespaces);
    }

  எனில் (ptr->linux.capabilities != வெற்று)
    {
      க்கு (size_t i = 0; ptr->linux.capabilities[i] != வெற்று; i++)
        விடுவி (ptr->linux.capabilities[i]);
      விடுவி (ptr->linux.capabilities);
    }
  எனில் (ptr->linux.seccomp.actions != வெற்று)
    {
      க்கு (i = 0; ptr->linux.seccomp.actions[i] != வெற்று; i++)
        விடுவி (ptr->linux.seccomp.actions[i]);
      விடுவி (ptr->linux.seccomp.actions);
    }
  எனில் (ptr->linux.seccomp.operators != வெற்று)
    {
      க்கு (i = 0; ptr->linux.seccomp.operators[i] != வெற்று; i++)
        விடுவி (ptr->linux.seccomp.operators[i]);
      விடுவி (ptr->linux.seccomp.operators);
    }

  விடுவி (ptr);
  *info = வெற்று;
}

#define cleanup_struct_features __attribute__ ((cleanup (cleanup_struct_features_free)))

#endif
