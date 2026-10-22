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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <ocispec/runtime_spec_schema_config_schema.h>
#ಅನ್ತರ್ಭಾವ "error.h"
#ಅನ್ತರ್ಭಾವ "string_map.h"

ಗಣನಾ handler_configure_phase
{
  HANDLER_CONFIGURE_BEFORE_MOUNTS = 1,
  HANDLER_CONFIGURE_AFTER_MOUNTS,
  HANDLER_CONFIGURE_MOUNTS,
  HANDLER_CONFIGURE_BEFORE_USERNS
};

ಸಂರಚನಾ custom_handler_manager_s;

ಸಂರಚನಾ libcrun_context_s
{
  ಧ್ರುವ ವರ್ಣ *state_ಬೇರು;
  ಧ್ರುವ ವರ್ಣ *id;
  ಧ್ರುವ ವರ್ಣ *bundle;
  ಧ್ರುವ ವರ್ಣ *console_socket;
  ಧ್ರುವ ವರ್ಣ *pid_file;
  ಧ್ರುವ ವರ್ಣ *notify_socket;
  ಧ್ರುವ ವರ್ಣ *handler;
  ವರ್ಣ *owned_state_ಬೇರು;
  ವರ್ಣ *owned_id;
  ವರ್ಣ *owned_bundle;
  ವರ್ಣ *owned_console_socket;
  ವರ್ಣ *owned_pid_file;
  ವರ್ಣ *owned_notify_socket;
  ವರ್ಣ *owned_handler;
  ಪೂರ್ಣಾಙ್ಕ preserve_fds;
  // For some use-cases we need differentiation between preserve_fds and listen_fds.
  // Following context variable makes sure we get exact value of listen_fds irrespective of preserve_fds.
  ಪೂರ್ಣಾಙ್ಕ listen_fds;

  crun_output_handler output_handler;
  ಶೂನ್ಯ *output_handler_arg;

  ಪೂರ್ಣಾಙ್ಕ fifo_exec_wait_fd;

  bool systemd_cgroup;
  bool detach;
  bool no_new_keyring;
  bool force_no_cgroup;
  bool no_pivot;
  bool no_open_ವೃಕ್ಷ_namespace;

  ವರ್ಣ **argv;
  ಪೂರ್ಣಾಙ್ಕ argc;

  ಸಂರಚನಾ custom_handler_manager_s *handler_manager;
};

ಗಣನಾ
{
  LIBCRUN_RUN_OPTIONS_PREFORK = 1 << 0,
  LIBCRUN_RUN_OPTIONS_KEEP = 1 << 1,
};

ಗಣನಾ
{
  LIBCRUN_CREATE_OPTIONS_PREFORK = 1 << 0,
};

ಸಂರಚನಾ libcrun_container_s
{
  /* Container parsed from the runtime json file.  */
  runtime_spec_schema_config_schema *container_def;

  uid_t host_uid;
  gid_t host_gid;

  /* Whether crun has CAP_SYS_PTRACE in its user namespace.  */
  bool host_has_cap_sys_ptrace;

  uid_t container_uid;
  gid_t container_gid;

  ವರ್ಣ *config_file;
  ವರ್ಣ *config_file_content;

  string_map *annotations;

  ಪೂರ್ಣಾಙ್ಕ proc_fd;

  ಶೂನ್ಯ *private_data;
  ಶೂನ್ಯ (*cleanup_private_data) (ಶೂನ್ಯ *private_data);
  ಸಂರಚನಾ libcrun_context_s *context;
};

ಸಂರಚನಾ libcrun_container_status_s;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ libcrun_container_status_s libcrun_container_status_t;

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ libcrun_container_s libcrun_container_t;
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ libcrun_context_s libcrun_context_t;

ಸಂರಚನಾ container_entrypoint_s;
ಸಂರಚನಾ cgroup_info_s
{
  bool v1;
  bool v2;
  bool systemd;
  bool systemd_user;
};

ಸಂರಚನಾ seccomp_info_s
{
  bool enabled;
  ವರ್ಣ **actions;
  ವರ್ಣ **operators;
  ವರ್ಣ **archs;
};

ಸಂರಚನಾ apparmor_info_s
{
  bool enabled;
};

ಸಂರಚನಾ selinux_info_s
{
  bool enabled;
};

ಸಂರಚನಾ idmap_info_s
{
  bool enabled;
};

ಸಂರಚನಾ intel_rdt_s
{
  bool enabled;
};

ಸಂರಚನಾ net_devices_s
{
  bool enabled;
};

ಸಂರಚನಾ mount_ext_info_s
{
  ಸಂರಚನಾ idmap_info_s idmap;
};

ಸಂರಚನಾ memory_policy_info_s
{
  ವರ್ಣ **mode;
  ವರ್ಣ **flags;
};

ಸಂರಚನಾ linux_info_s
{
  ವರ್ಣ **namespaces;
  ವರ್ಣ **capabilities;
  ಸಂರಚನಾ cgroup_info_s cgroup;
  ಸಂರಚನಾ seccomp_info_s seccomp;
  ಸಂರಚನಾ apparmor_info_s apparmor;
  ಸಂರಚನಾ selinux_info_s selinux;
  ಸಂರಚನಾ mount_ext_info_s mount_ext;
  ಸಂರಚನಾ intel_rdt_s intel_rdt;
  ಸಂರಚನಾ net_devices_s net_devices;
  ಸಂರಚನಾ memory_policy_info_s memory_policy;
};

ಸಂರಚನಾ annotations_info_s
{
  ವರ್ಣ *io_github_seccomp_libseccomp_version;
  bool run_oci_crun_checkpoint_enabled;
  ವರ್ಣ *run_oci_crun_commit;
  ವರ್ಣ *run_oci_crun_version;
  bool run_oci_crun_wasm;
};

ಸಂರಚನಾ features_info_s
{
  ವರ್ಣ *oci_version_min;
  ವರ್ಣ *oci_version_max;
  ವರ್ಣ **hooks;
  ವರ್ಣ **mount_options;
  ಸಂರಚನಾ linux_info_s linux;
  ಸಂರಚನಾ annotations_info_s annotations;
  ವರ್ಣ **potentially_unsafe_annotations;
};

ಸಂರಚನಾ libcrun_checkpoint_restore_s
{
  ವರ್ಣ *image_path;
  ವರ್ಣ *work_path;
  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool shell_job;
  bool ext_unix_sk;
  bool detach;
  bool file_locks;
  ಧ್ರುವ ವರ್ಣ *console_socket;
  ವರ್ಣ *parent_path;
  bool pre_dump;
  ಪೂರ್ಣಾಙ್ಕ manage_cgroups_mode;
  ಪೂರ್ಣಾಙ್ಕ network_lock_method;
  ವರ್ಣ *lsm_profile;
  ವರ್ಣ *lsm_mount_context;
};
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ libcrun_checkpoint_restore_s libcrun_checkpoint_restore_t;

/* Public checkpoint/restore options.  Mirror of the definition in
   <libcrun.h>; keep the two in sync.  */
ಗಣನಾ libcrun_cr_cgroups_mode
{
  LIBCRUN_CR_CG_MODE_DEFAULT = 0, /* soft, matching runc */
  LIBCRUN_CR_CG_MODE_SOFT,
  LIBCRUN_CR_CG_MODE_IGNORE,
  LIBCRUN_CR_CG_MODE_FULL,
  LIBCRUN_CR_CG_MODE_STRICT,
};

ಗಣನಾ libcrun_cr_network_lock_method
{
  LIBCRUN_CR_NETWORK_LOCK_DEFAULT = 0, /* iptables */
  LIBCRUN_CR_NETWORK_LOCK_IPTABLES,
  LIBCRUN_CR_NETWORK_LOCK_NFTABLES,
  LIBCRUN_CR_NETWORK_LOCK_SKIP,
};

ಸಂರಚನಾ libcrun_checkpoint_restore_options_s
{
  size_t struct_size;

  ಧ್ರುವ ವರ್ಣ *image_path;
  ಧ್ರುವ ವರ್ಣ *work_path;
  ಧ್ರುವ ವರ್ಣ *parent_path;
  ಧ್ರುವ ವರ್ಣ *console_socket;
  ಧ್ರುವ ವರ್ಣ *lsm_profile;
  ಧ್ರುವ ವರ್ಣ *lsm_mount_context;

  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool ext_unix_sk;
  bool shell_job;
  bool file_locks;
  bool pre_dump;
  bool detach;

  ಪೂರ್ಣಾಙ್ಕ manage_cgroups_mode; /* enum libcrun_cr_cgroups_mode */
  ಪೂರ್ಣಾಙ್ಕ network_lock_method; /* enum libcrun_cr_network_lock_method */
};

LIBCRUN_PUBLIC libcrun_container_t *libcrun_container_load_from_file (ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err);

LIBCRUN_PUBLIC libcrun_container_t *libcrun_container_load_from_memory (ಧ್ರುವ ವರ್ಣ *json, libcrun_error_t *err);

LIBCRUN_PUBLIC ಶೂನ್ಯ libcrun_container_free (libcrun_container_t *);

/* Return the configuration of CONTAINER as a JSON string owned by CONTAINER.
   The document is generated on demand and cached, so the first call mutates
   CONTAINER and must not race with other users of the same handle.  */
LIBCRUN_PUBLIC ಧ್ರುವ ವರ್ಣ *libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err);
LIBCRUN_PUBLIC ಧ್ರುವ ವರ್ಣ *libcrun_container_get_annotation (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ);
LIBCRUN_PUBLIC size_t libcrun_container_get_annotations_len (libcrun_container_t *container);
LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_get_annotation_at (libcrun_container_t *container, size_t index,
                                                        ಧ್ರುವ ವರ್ಣ **ಕೀಲಿ, ಧ್ರುವ ವರ್ಣ **ಮೌಲ್ಯ);
LIBCRUN_PUBLIC uid_t libcrun_container_get_uid (libcrun_container_t *container);
LIBCRUN_PUBLIC gid_t libcrun_container_get_gid (libcrun_container_t *container);

LIBCRUN_PUBLIC libcrun_context_t *libcrun_context_new (ಧ್ರುವ ವರ್ಣ *id, ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, libcrun_error_t *err);
LIBCRUN_PUBLIC ಶೂನ್ಯ libcrun_context_free (libcrun_context_t *ctx);

LIBCRUN_PUBLIC ಶೂನ್ಯ libcrun_context_set_id (libcrun_context_t *ctx, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ);
LIBCRUN_PUBLIC ಶೂನ್ಯ libcrun_context_set_state_ಬೇರು (libcrun_context_t *ctx, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ);
LIBCRUN_PUBLIC ಧ್ರುವ ವರ್ಣ *libcrun_context_get_state_ಬೇರು (libcrun_context_t *ctx);
LIBCRUN_PUBLIC ಶೂನ್ಯ libcrun_context_set_bundle (libcrun_context_t *ctx, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ);
LIBCRUN_PUBLIC ಶೂನ್ಯ libcrun_context_set_console_socket (libcrun_context_t *ctx, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ);
LIBCRUN_PUBLIC ಶೂನ್ಯ libcrun_context_set_pid_file (libcrun_context_t *ctx, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ);
LIBCRUN_PUBLIC ಧ್ರುವ ವರ್ಣ *libcrun_context_get_pid_file (libcrun_context_t *ctx);
LIBCRUN_PUBLIC ಶೂನ್ಯ libcrun_context_set_notify_socket (libcrun_context_t *ctx, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ);
LIBCRUN_PUBLIC ಶೂನ್ಯ libcrun_context_set_handler (libcrun_context_t *ctx, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ);
LIBCRUN_PUBLIC ಶೂನ್ಯ libcrun_context_set_preserve_fds (libcrun_context_t *ctx, ಪೂರ್ಣಾಙ್ಕ preserve_fds);
LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_context_get_preserve_fds (libcrun_context_t *ctx);
LIBCRUN_PUBLIC ಶೂನ್ಯ libcrun_context_set_listen_fds (libcrun_context_t *ctx, ಪೂರ್ಣಾಙ್ಕ listen_fds);
LIBCRUN_PUBLIC ಶೂನ್ಯ libcrun_context_set_args (libcrun_context_t *ctx, ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv);
LIBCRUN_PUBLIC ಶೂನ್ಯ libcrun_context_set_systemd_cgroup (libcrun_context_t *ctx, bool ಮೌಲ್ಯ);
LIBCRUN_PUBLIC ಶೂನ್ಯ libcrun_context_set_detach (libcrun_context_t *ctx, bool ಮೌಲ್ಯ);
LIBCRUN_PUBLIC ಶೂನ್ಯ libcrun_context_set_no_new_keyring (libcrun_context_t *ctx, bool ಮೌಲ್ಯ);
LIBCRUN_PUBLIC ಶೂನ್ಯ libcrun_context_set_no_pivot (libcrun_context_t *ctx, bool ಮೌಲ್ಯ);
LIBCRUN_PUBLIC ಶೂನ್ಯ libcrun_context_set_force_no_cgroup (libcrun_context_t *ctx, bool ಮೌಲ್ಯ);
LIBCRUN_PUBLIC ಶೂನ್ಯ libcrun_context_set_output_handler (libcrun_context_t *ctx, crun_output_handler handler, ಶೂನ್ಯ *arg);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_context_init_logging (libcrun_context_t *ctx, ಧ್ರುವ ವರ್ಣ *log, libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_run (libcrun_context_t *context, libcrun_container_t *container,
                                          unsigned ಪೂರ್ಣಾಙ್ಕ options, libcrun_error_t *error);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_delete (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, bool force,
                                             libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_kill (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, ಧ್ರುವ ವರ್ಣ *ಸಙ್ಕೇತ,
                                           libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_killall (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, ಧ್ರುವ ವರ್ಣ *ಸಙ್ಕೇತ,
                                              libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container,
                                             unsigned ಪೂರ್ಣಾಙ್ಕ options, libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_start (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_state (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, FILE *out,
                                            libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_state_json (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, ವರ್ಣ **out,
                                                 libcrun_error_t *err);

ಪೂರ್ಣಾಙ್ಕ libcrun_container_notify_handler (ಸಂರಚನಾ container_entrypoint_s *args,
                                      ಗಣನಾ handler_configure_phase phase,
                                      libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *rootfs,
                                      libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_get_container_state_string (ಧ್ರುವ ವರ್ಣ *id, libcrun_container_status_t *status,
                                                       ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, ಧ್ರುವ ವರ್ಣ **container_status,
                                                       ಪೂರ್ಣಾಙ್ಕ *running, libcrun_error_t *err);

ಸಂರಚನಾ libcrun_container_exec_options_s
{
  size_t struct_size;
  runtime_spec_schema_config_schema_process *process;
  ಧ್ರುವ ವರ್ಣ *path;
  ಧ್ರುವ ವರ್ಣ *cgroup;
  bool merge_env;
};

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_load_process_from_file (ಧ್ರುವ ವರ್ಣ *path,
                                                   runtime_spec_schema_config_schema_process **process,
                                                   libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_exec_with_options (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id,
                                                        ಸಂರಚನಾ libcrun_container_exec_options_s *opts,
                                                        libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_exec (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id,
                                           runtime_spec_schema_config_schema_process *process, libcrun_error_t *err);

ಸಂರಚನಾ libcrun_exec_options_s
{
  size_t struct_size;
  ಧ್ರುವ ವರ್ಣ *process_json; /* OCI Process JSON */
  ಧ್ರುವ ವರ್ಣ *cgroup;
  bool merge_env;
};

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_exec_json (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, ಧ್ರುವ ವರ್ಣ *process_json,
                                                libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_exec_with_options_json (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id,
                                                             ಧ್ರುವ ಸಂರಚನಾ libcrun_exec_options_s *opts,
                                                             libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_exec_process_file (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, ಧ್ರುವ ವರ್ಣ *path,
                                                        libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_update (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, ಧ್ರುವ ವರ್ಣ *content,
                                             size_t len, libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_update_from_file (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, ಧ್ರುವ ವರ್ಣ *file,
                                                       libcrun_error_t *err);

ಸಂರಚನಾ libcrun_update_ಮೌಲ್ಯ_s
{
  ಧ್ರುವ ವರ್ಣ *section;
  ಧ್ರುವ ವರ್ಣ *name;
  bool numeric;
  ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ;
};

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_update_from_ಮೌಲ್ಯಗಳು (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id,
                                                         ಸಂರಚನಾ libcrun_update_ಮೌಲ್ಯ_s *ಮೌಲ್ಯಗಳು, size_t len,
                                                         libcrun_error_t *err);

ಸಂರಚನಾ libcrun_intel_rdt_update
{
  ಧ್ರುವ ವರ್ಣ *l3_cache_schema;
  ಧ್ರುವ ವರ್ಣ *mem_bw_schema;
  ವರ್ಣ *ಧ್ರುವ *schemata;
};

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_update_intel_rdt (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id,
                                                       ಸಂರಚನಾ libcrun_intel_rdt_update *update, libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_get_features (libcrun_context_t *context, ಸಂರಚನಾ features_info_s **info,
                                                   libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_get_features_json (libcrun_context_t *context, ವರ್ಣ **out, libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_pause (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_unpause (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_checkpoint (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id,
                                                 ಸಂರಚನಾ libcrun_checkpoint_restore_options_s *cr_options,
                                                 libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_restore (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id,
                                              ಸಂರಚನಾ libcrun_checkpoint_restore_options_s *cr_options,
                                              libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_read_pids (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, bool recurse, pid_t **pids, libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_write_json_containers_list (libcrun_context_t *context, FILE *out, libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_list_json (libcrun_context_t *context, ವರ್ಣ **out, libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_add_mounts_from_file (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, ಧ್ರುವ ವರ್ಣ *file,
                                                           libcrun_error_t *err);

LIBCRUN_PUBLIC ಪೂರ್ಣಾಙ್ಕ libcrun_container_remove_mounts_from_file (libcrun_context_t *context, ಧ್ರುವ ವರ್ಣ *id, ಧ್ರುವ ವರ್ಣ *file,
                                                              libcrun_error_t *err);

// Not part of the public API, just a method in container.c we need to access from linux.c
ಶೂನ್ಯ get_ಬೇರು_in_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid,
                             uid_t *uid, gid_t *gid);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
cleanup_containerp (libcrun_container_t **c)
{
  libcrun_container_t *container = *c;
  libcrun_container_free (container);
}

#define cleanup_container __attribute__ ((cleanup (cleanup_containerp)))

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
cleanup_struct_features_free (ಸಂರಚನಾ features_info_s **info)
{
  size_t i;
  ಸಂರಚನಾ features_info_s *ptr;
  ಯದಿ (info == ರಿಕ್ತ || *info == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ;

  ptr = *info;

  // Free oci_version_min if it is not NULL
  ಯದಿ (ptr->oci_version_min != ರಿಕ್ತ)
    {
      ಮುಕ್ತ (ptr->oci_version_min);
      ptr->oci_version_min = ರಿಕ್ತ; // Set to NULL after freeing
    }

  // Free oci_version_max if it is not NULL
  ಯದಿ (ptr->oci_version_max != ರಿಕ್ತ)
    {
      ಮುಕ್ತ (ptr->oci_version_max);
      ptr->oci_version_max = ರಿಕ್ತ; // Set to NULL after freeing
    }
  ಯದಿ (ptr->hooks != ರಿಕ್ತ)
    {
      ಪ್ರತಿ (i = 0; ptr->hooks[i] != ರಿಕ್ತ; i++)
        ಮುಕ್ತ (ptr->hooks[i]);
      ಮುಕ್ತ (ptr->hooks);
    }
  ಯದಿ (ptr->mount_options != ರಿಕ್ತ)
    {
      ಪ್ರತಿ (i = 0; ptr->mount_options[i] != ರಿಕ್ತ; i++)
        ಮುಕ್ತ (ptr->mount_options[i]);
      ಮುಕ್ತ (ptr->mount_options);
    }
  ಯದಿ (ptr->linux.namespaces != ರಿಕ್ತ)
    {
      ಪ್ರತಿ (i = 0; ptr->linux.namespaces[i] != ರಿಕ್ತ; i++)
        ಮುಕ್ತ (ptr->linux.namespaces[i]);
      ಮುಕ್ತ (ptr->linux.namespaces);
    }

  ಯದಿ (ptr->linux.capabilities != ರಿಕ್ತ)
    {
      ಪ್ರತಿ (size_t i = 0; ptr->linux.capabilities[i] != ರಿಕ್ತ; i++)
        ಮುಕ್ತ (ptr->linux.capabilities[i]);
      ಮುಕ್ತ (ptr->linux.capabilities);
    }
  ಯದಿ (ptr->linux.seccomp.actions != ರಿಕ್ತ)
    {
      ಪ್ರತಿ (i = 0; ptr->linux.seccomp.actions[i] != ರಿಕ್ತ; i++)
        ಮುಕ್ತ (ptr->linux.seccomp.actions[i]);
      ಮುಕ್ತ (ptr->linux.seccomp.actions);
    }
  ಯದಿ (ptr->linux.seccomp.operators != ರಿಕ್ತ)
    {
      ಪ್ರತಿ (i = 0; ptr->linux.seccomp.operators[i] != ರಿಕ್ತ; i++)
        ಮುಕ್ತ (ptr->linux.seccomp.operators[i]);
      ಮುಕ್ತ (ptr->linux.seccomp.operators);
    }

  ಮುಕ್ತ (ptr);
  *info = ರಿಕ್ತ;
}

#define cleanup_struct_features __attribute__ ((cleanup (cleanup_struct_features_free)))

#endif
