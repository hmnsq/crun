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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <ocispec/runtime_spec_schema_config_schema.h>
#അന്തര്ഭാവ "error.h"
#അന്തര്ഭാവ "string_map.h"

എണ്ണൽ handler_configure_phase
{
  HANDLER_CONFIGURE_BEFORE_MOUNTS = 1,
  HANDLER_CONFIGURE_AFTER_MOUNTS,
  HANDLER_CONFIGURE_MOUNTS,
  HANDLER_CONFIGURE_BEFORE_USERNS
};

ഘടന custom_handler_manager_s;

ഘടന libcrun_context_s
{
  സ്ഥിരം അക്ഷരം *state_വേര്;
  സ്ഥിരം അക്ഷരം *id;
  സ്ഥിരം അക്ഷരം *bundle;
  സ്ഥിരം അക്ഷരം *console_socket;
  സ്ഥിരം അക്ഷരം *pid_file;
  സ്ഥിരം അക്ഷരം *notify_socket;
  സ്ഥിരം അക്ഷരം *handler;
  അക്ഷരം *owned_state_വേര്;
  അക്ഷരം *owned_id;
  അക്ഷരം *owned_bundle;
  അക്ഷരം *owned_console_socket;
  അക്ഷരം *owned_pid_file;
  അക്ഷരം *owned_notify_socket;
  അക്ഷരം *owned_handler;
  പൂർണ്ണസംഖ്യ preserve_fds;
  // For some use-cases we need differentiation between preserve_fds and listen_fds.
  // Following context variable makes sure we get exact value of listen_fds irrespective of preserve_fds.
  പൂർണ്ണസംഖ്യ listen_fds;

  crun_output_handler output_handler;
  ശൂന്യം *output_handler_arg;

  പൂർണ്ണസംഖ്യ fifo_exec_wait_fd;

  bool systemd_cgroup;
  bool detach;
  bool no_new_keyring;
  bool force_no_cgroup;
  bool no_pivot;
  bool no_open_വൃക്ഷം_namespace;

  അക്ഷരം **argv;
  പൂർണ്ണസംഖ്യ argc;

  ഘടന custom_handler_manager_s *handler_manager;
};

എണ്ണൽ
{
  LIBCRUN_RUN_OPTIONS_PREFORK = 1 << 0,
  LIBCRUN_RUN_OPTIONS_KEEP = 1 << 1,
};

എണ്ണൽ
{
  LIBCRUN_CREATE_OPTIONS_PREFORK = 1 << 0,
};

ഘടന libcrun_container_s
{
  /* Container parsed from the runtime json file.  */
  runtime_spec_schema_config_schema *container_def;

  uid_t host_uid;
  gid_t host_gid;

  /* Whether crun has CAP_SYS_PTRACE in its user namespace.  */
  bool host_has_cap_sys_ptrace;

  uid_t container_uid;
  gid_t container_gid;

  അക്ഷരം *config_file;
  അക്ഷരം *config_file_content;

  string_map *annotations;

  പൂർണ്ണസംഖ്യ proc_fd;

  ശൂന്യം *private_data;
  ശൂന്യം (*cleanup_private_data) (ശൂന്യം *private_data);
  ഘടന libcrun_context_s *context;
};

ഘടന libcrun_container_status_s;
തരനിർവചനം ഘടന libcrun_container_status_s libcrun_container_status_t;

തരനിർവചനം ഘടന libcrun_container_s libcrun_container_t;
തരനിർവചനം ഘടന libcrun_context_s libcrun_context_t;

ഘടന container_entrypoint_s;
ഘടന cgroup_info_s
{
  bool v1;
  bool v2;
  bool systemd;
  bool systemd_user;
};

ഘടന seccomp_info_s
{
  bool enabled;
  അക്ഷരം **actions;
  അക്ഷരം **operators;
  അക്ഷരം **archs;
};

ഘടന apparmor_info_s
{
  bool enabled;
};

ഘടന selinux_info_s
{
  bool enabled;
};

ഘടന idmap_info_s
{
  bool enabled;
};

ഘടന intel_rdt_s
{
  bool enabled;
};

ഘടന net_devices_s
{
  bool enabled;
};

ഘടന mount_ext_info_s
{
  ഘടന idmap_info_s idmap;
};

ഘടന memory_policy_info_s
{
  അക്ഷരം **mode;
  അക്ഷരം **flags;
};

ഘടന linux_info_s
{
  അക്ഷരം **namespaces;
  അക്ഷരം **capabilities;
  ഘടന cgroup_info_s cgroup;
  ഘടന seccomp_info_s seccomp;
  ഘടന apparmor_info_s apparmor;
  ഘടന selinux_info_s selinux;
  ഘടന mount_ext_info_s mount_ext;
  ഘടന intel_rdt_s intel_rdt;
  ഘടന net_devices_s net_devices;
  ഘടന memory_policy_info_s memory_policy;
};

ഘടന annotations_info_s
{
  അക്ഷരം *io_github_seccomp_libseccomp_version;
  bool run_oci_crun_checkpoint_enabled;
  അക്ഷരം *run_oci_crun_commit;
  അക്ഷരം *run_oci_crun_version;
  bool run_oci_crun_wasm;
};

ഘടന features_info_s
{
  അക്ഷരം *oci_version_min;
  അക്ഷരം *oci_version_max;
  അക്ഷരം **hooks;
  അക്ഷരം **mount_options;
  ഘടന linux_info_s linux;
  ഘടന annotations_info_s annotations;
  അക്ഷരം **potentially_unsafe_annotations;
};

ഘടന libcrun_checkpoint_restore_s
{
  അക്ഷരം *image_path;
  അക്ഷരം *work_path;
  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool shell_job;
  bool ext_unix_sk;
  bool detach;
  bool file_locks;
  സ്ഥിരം അക്ഷരം *console_socket;
  അക്ഷരം *parent_path;
  bool pre_dump;
  പൂർണ്ണസംഖ്യ manage_cgroups_mode;
  പൂർണ്ണസംഖ്യ network_lock_method;
  അക്ഷരം *lsm_profile;
  അക്ഷരം *lsm_mount_context;
};
തരനിർവചനം ഘടന libcrun_checkpoint_restore_s libcrun_checkpoint_restore_t;

/* Public checkpoint/restore options.  Mirror of the definition in
   <libcrun.h>; keep the two in sync.  */
എണ്ണൽ libcrun_cr_cgroups_mode
{
  LIBCRUN_CR_CG_MODE_DEFAULT = 0, /* soft, matching runc */
  LIBCRUN_CR_CG_MODE_SOFT,
  LIBCRUN_CR_CG_MODE_IGNORE,
  LIBCRUN_CR_CG_MODE_FULL,
  LIBCRUN_CR_CG_MODE_STRICT,
};

എണ്ണൽ libcrun_cr_network_lock_method
{
  LIBCRUN_CR_NETWORK_LOCK_DEFAULT = 0, /* iptables */
  LIBCRUN_CR_NETWORK_LOCK_IPTABLES,
  LIBCRUN_CR_NETWORK_LOCK_NFTABLES,
  LIBCRUN_CR_NETWORK_LOCK_SKIP,
};

ഘടന libcrun_checkpoint_restore_options_s
{
  size_t struct_size;

  സ്ഥിരം അക്ഷരം *image_path;
  സ്ഥിരം അക്ഷരം *work_path;
  സ്ഥിരം അക്ഷരം *parent_path;
  സ്ഥിരം അക്ഷരം *console_socket;
  സ്ഥിരം അക്ഷരം *lsm_profile;
  സ്ഥിരം അക്ഷരം *lsm_mount_context;

  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool ext_unix_sk;
  bool shell_job;
  bool file_locks;
  bool pre_dump;
  bool detach;

  പൂർണ്ണസംഖ്യ manage_cgroups_mode; /* enum libcrun_cr_cgroups_mode */
  പൂർണ്ണസംഖ്യ network_lock_method; /* enum libcrun_cr_network_lock_method */
};

LIBCRUN_PUBLIC libcrun_container_t *libcrun_container_load_from_file (സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err);

LIBCRUN_PUBLIC libcrun_container_t *libcrun_container_load_from_memory (സ്ഥിരം അക്ഷരം *json, libcrun_error_t *err);

LIBCRUN_PUBLIC ശൂന്യം libcrun_container_free (libcrun_container_t *);

/* Return the configuration of CONTAINER as a JSON string owned by CONTAINER.
   The document is generated on demand and cached, so the first call mutates
   CONTAINER and must not race with other users of the same handle.  */
LIBCRUN_PUBLIC സ്ഥിരം അക്ഷരം *libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err);
LIBCRUN_PUBLIC സ്ഥിരം അക്ഷരം *libcrun_container_get_annotation (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *താക്കോൽ);
LIBCRUN_PUBLIC size_t libcrun_container_get_annotations_len (libcrun_container_t *container);
LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_get_annotation_at (libcrun_container_t *container, size_t index,
                                                        സ്ഥിരം അക്ഷരം **താക്കോൽ, സ്ഥിരം അക്ഷരം **മൂല്യം);
LIBCRUN_PUBLIC uid_t libcrun_container_get_uid (libcrun_container_t *container);
LIBCRUN_PUBLIC gid_t libcrun_container_get_gid (libcrun_container_t *container);

LIBCRUN_PUBLIC libcrun_context_t *libcrun_context_new (സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *state_വേര്, libcrun_error_t *err);
LIBCRUN_PUBLIC ശൂന്യം libcrun_context_free (libcrun_context_t *ctx);

LIBCRUN_PUBLIC ശൂന്യം libcrun_context_set_id (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *മൂല്യം);
LIBCRUN_PUBLIC ശൂന്യം libcrun_context_set_state_വേര് (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *മൂല്യം);
LIBCRUN_PUBLIC സ്ഥിരം അക്ഷരം *libcrun_context_get_state_വേര് (libcrun_context_t *ctx);
LIBCRUN_PUBLIC ശൂന്യം libcrun_context_set_bundle (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *മൂല്യം);
LIBCRUN_PUBLIC ശൂന്യം libcrun_context_set_console_socket (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *മൂല്യം);
LIBCRUN_PUBLIC ശൂന്യം libcrun_context_set_pid_file (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *മൂല്യം);
LIBCRUN_PUBLIC സ്ഥിരം അക്ഷരം *libcrun_context_get_pid_file (libcrun_context_t *ctx);
LIBCRUN_PUBLIC ശൂന്യം libcrun_context_set_notify_socket (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *മൂല്യം);
LIBCRUN_PUBLIC ശൂന്യം libcrun_context_set_handler (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *മൂല്യം);
LIBCRUN_PUBLIC ശൂന്യം libcrun_context_set_preserve_fds (libcrun_context_t *ctx, പൂർണ്ണസംഖ്യ preserve_fds);
LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_context_get_preserve_fds (libcrun_context_t *ctx);
LIBCRUN_PUBLIC ശൂന്യം libcrun_context_set_listen_fds (libcrun_context_t *ctx, പൂർണ്ണസംഖ്യ listen_fds);
LIBCRUN_PUBLIC ശൂന്യം libcrun_context_set_args (libcrun_context_t *ctx, പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv);
LIBCRUN_PUBLIC ശൂന്യം libcrun_context_set_systemd_cgroup (libcrun_context_t *ctx, bool മൂല്യം);
LIBCRUN_PUBLIC ശൂന്യം libcrun_context_set_detach (libcrun_context_t *ctx, bool മൂല്യം);
LIBCRUN_PUBLIC ശൂന്യം libcrun_context_set_no_new_keyring (libcrun_context_t *ctx, bool മൂല്യം);
LIBCRUN_PUBLIC ശൂന്യം libcrun_context_set_no_pivot (libcrun_context_t *ctx, bool മൂല്യം);
LIBCRUN_PUBLIC ശൂന്യം libcrun_context_set_force_no_cgroup (libcrun_context_t *ctx, bool മൂല്യം);
LIBCRUN_PUBLIC ശൂന്യം libcrun_context_set_output_handler (libcrun_context_t *ctx, crun_output_handler handler, ശൂന്യം *arg);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_context_init_logging (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *log, libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_run (libcrun_context_t *context, libcrun_container_t *container,
                                          unsigned പൂർണ്ണസംഖ്യ options, libcrun_error_t *error);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_delete (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, bool force,
                                             libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_kill (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *സൂചന,
                                           libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_killall (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *സൂചന,
                                              libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container,
                                             unsigned പൂർണ്ണസംഖ്യ options, libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_start (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_state (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, FILE *out,
                                            libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_state_json (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, അക്ഷരം **out,
                                                 libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_container_notify_handler (ഘടന container_entrypoint_s *args,
                                      എണ്ണൽ handler_configure_phase phase,
                                      libcrun_container_t *container, സ്ഥിരം അക്ഷരം *rootfs,
                                      libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_get_container_state_string (സ്ഥിരം അക്ഷരം *id, libcrun_container_status_t *status,
                                                       സ്ഥിരം അക്ഷരം *state_വേര്, സ്ഥിരം അക്ഷരം **container_status,
                                                       പൂർണ്ണസംഖ്യ *running, libcrun_error_t *err);

ഘടന libcrun_container_exec_options_s
{
  size_t struct_size;
  runtime_spec_schema_config_schema_process *process;
  സ്ഥിരം അക്ഷരം *path;
  സ്ഥിരം അക്ഷരം *cgroup;
  bool merge_env;
};

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_load_process_from_file (സ്ഥിരം അക്ഷരം *path,
                                                   runtime_spec_schema_config_schema_process **process,
                                                   libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_exec_with_options (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id,
                                                        ഘടന libcrun_container_exec_options_s *opts,
                                                        libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_exec (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id,
                                           runtime_spec_schema_config_schema_process *process, libcrun_error_t *err);

ഘടന libcrun_exec_options_s
{
  size_t struct_size;
  സ്ഥിരം അക്ഷരം *process_json; /* OCI Process JSON */
  സ്ഥിരം അക്ഷരം *cgroup;
  bool merge_env;
};

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_exec_json (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *process_json,
                                                libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_exec_with_options_json (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id,
                                                             സ്ഥിരം ഘടന libcrun_exec_options_s *opts,
                                                             libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_exec_process_file (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *path,
                                                        libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_update (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *content,
                                             size_t len, libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_update_from_file (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *file,
                                                       libcrun_error_t *err);

ഘടന libcrun_update_മൂല്യം_s
{
  സ്ഥിരം അക്ഷരം *section;
  സ്ഥിരം അക്ഷരം *name;
  bool numeric;
  സ്ഥിരം അക്ഷരം *മൂല്യം;
};

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_update_from_മൂല്യങ്ങൾ (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id,
                                                         ഘടന libcrun_update_മൂല്യം_s *മൂല്യങ്ങൾ, size_t len,
                                                         libcrun_error_t *err);

ഘടന libcrun_intel_rdt_update
{
  സ്ഥിരം അക്ഷരം *l3_cache_schema;
  സ്ഥിരം അക്ഷരം *mem_bw_schema;
  അക്ഷരം *സ്ഥിരം *schemata;
};

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_update_intel_rdt (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id,
                                                       ഘടന libcrun_intel_rdt_update *update, libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_get_features (libcrun_context_t *context, ഘടന features_info_s **info,
                                                   libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_get_features_json (libcrun_context_t *context, അക്ഷരം **out, libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_pause (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_unpause (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_checkpoint (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id,
                                                 ഘടന libcrun_checkpoint_restore_options_s *cr_options,
                                                 libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_restore (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id,
                                              ഘടന libcrun_checkpoint_restore_options_s *cr_options,
                                              libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_read_pids (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, bool recurse, pid_t **pids, libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_write_json_containers_list (libcrun_context_t *context, FILE *out, libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_list_json (libcrun_context_t *context, അക്ഷരം **out, libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_add_mounts_from_file (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *file,
                                                           libcrun_error_t *err);

LIBCRUN_PUBLIC പൂർണ്ണസംഖ്യ libcrun_container_remove_mounts_from_file (libcrun_context_t *context, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *file,
                                                              libcrun_error_t *err);

// Not part of the public API, just a method in container.c we need to access from linux.c
ശൂന്യം get_വേര്_in_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid,
                             uid_t *uid, gid_t *gid);

സ്ഥിതം വരിയിൽ ശൂന്യം
cleanup_containerp (libcrun_container_t **c)
{
  libcrun_container_t *container = *c;
  libcrun_container_free (container);
}

#define cleanup_container __attribute__ ((cleanup (cleanup_containerp)))

സ്ഥിതം വരിയിൽ ശൂന്യം
cleanup_struct_features_free (ഘടന features_info_s **info)
{
  size_t i;
  ഘടന features_info_s *ptr;
  എങ്കിൽ (info == ശൂന്യ || *info == ശൂന്യ)
    തിരികെ;

  ptr = *info;

  // Free oci_version_min if it is not NULL
  എങ്കിൽ (ptr->oci_version_min != ശൂന്യ)
    {
      സ്വതന്ത്രം (ptr->oci_version_min);
      ptr->oci_version_min = ശൂന്യ; // Set to NULL after freeing
    }

  // Free oci_version_max if it is not NULL
  എങ്കിൽ (ptr->oci_version_max != ശൂന്യ)
    {
      സ്വതന്ത്രം (ptr->oci_version_max);
      ptr->oci_version_max = ശൂന്യ; // Set to NULL after freeing
    }
  എങ്കിൽ (ptr->hooks != ശൂന്യ)
    {
      വേണ്ടി (i = 0; ptr->hooks[i] != ശൂന്യ; i++)
        സ്വതന്ത്രം (ptr->hooks[i]);
      സ്വതന്ത്രം (ptr->hooks);
    }
  എങ്കിൽ (ptr->mount_options != ശൂന്യ)
    {
      വേണ്ടി (i = 0; ptr->mount_options[i] != ശൂന്യ; i++)
        സ്വതന്ത്രം (ptr->mount_options[i]);
      സ്വതന്ത്രം (ptr->mount_options);
    }
  എങ്കിൽ (ptr->linux.namespaces != ശൂന്യ)
    {
      വേണ്ടി (i = 0; ptr->linux.namespaces[i] != ശൂന്യ; i++)
        സ്വതന്ത്രം (ptr->linux.namespaces[i]);
      സ്വതന്ത്രം (ptr->linux.namespaces);
    }

  എങ്കിൽ (ptr->linux.capabilities != ശൂന്യ)
    {
      വേണ്ടി (size_t i = 0; ptr->linux.capabilities[i] != ശൂന്യ; i++)
        സ്വതന്ത്രം (ptr->linux.capabilities[i]);
      സ്വതന്ത്രം (ptr->linux.capabilities);
    }
  എങ്കിൽ (ptr->linux.seccomp.actions != ശൂന്യ)
    {
      വേണ്ടി (i = 0; ptr->linux.seccomp.actions[i] != ശൂന്യ; i++)
        സ്വതന്ത്രം (ptr->linux.seccomp.actions[i]);
      സ്വതന്ത്രം (ptr->linux.seccomp.actions);
    }
  എങ്കിൽ (ptr->linux.seccomp.operators != ശൂന്യ)
    {
      വേണ്ടി (i = 0; ptr->linux.seccomp.operators[i] != ശൂന്യ; i++)
        സ്വതന്ത്രം (ptr->linux.seccomp.operators[i]);
      സ്വതന്ത്രം (ptr->linux.seccomp.operators);
    }

  സ്വതന്ത്രം (ptr);
  *info = ശൂന്യ;
}

#define cleanup_struct_features __attribute__ ((cleanup (cleanup_struct_features_free)))

#endif
