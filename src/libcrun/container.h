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

#నిర్వచితం_కాకపోతే CONTAINER_H
#నిర్వచించు CONTAINER_H

#అన్తర్భావ <config.h>
#అన్తర్భావ <ocispec/runtime_spec_schema_config_schema.h>
#అన్తర్భావ "error.h"
#అన్తర్భావ "string_map.h"

గణన handler_configure_phase
{
  HANDLER_CONFIGURE_BEFORE_MOUNTS = 1,
  HANDLER_CONFIGURE_AFTER_MOUNTS,
  HANDLER_CONFIGURE_MOUNTS,
  HANDLER_CONFIGURE_BEFORE_USERNS
};

నిర్మాణం custom_handler_manager_s;

నిర్మాణం libcrun_context_s
{
  స్థిరరాశి అక్షరం *state_మూలం;
  స్థిరరాశి అక్షరం *id;
  స్థిరరాశి అక్షరం *bundle;
  స్థిరరాశి అక్షరం *console_socket;
  స్థిరరాశి అక్షరం *pid_file;
  స్థిరరాశి అక్షరం *notify_socket;
  స్థిరరాశి అక్షరం *handler;
  అక్షరం *owned_state_మూలం;
  అక్షరం *owned_id;
  అక్షరం *owned_bundle;
  అక్షరం *owned_console_socket;
  అక్షరం *owned_pid_file;
  అక్షరం *owned_notify_socket;
  అక్షరం *owned_handler;
  పూర్ణాంకం preserve_fds;
  // For some use-cases we need differentiation between preserve_fds and listen_fds.
  // Following context variable makes sure we get exact value of listen_fds irrespective of preserve_fds.
  పూర్ణాంకం listen_fds;

  crun_output_handler output_handler;
  శూన్య *output_handler_arg;

  పూర్ణాంకం fifo_exec_wait_fd;

  bool systemd_cgroup;
  bool detach;
  bool no_new_keyring;
  bool force_no_cgroup;
  bool no_ఇరుసు;
  bool no_open_వృక్షం_namespace;

  అక్షరం **argv;
  పూర్ణాంకం argc;

  నిర్మాణం custom_handler_manager_s *handler_manager;
};

గణన
{
  LIBCRUN_నిరంతరం_OPTIONS_PREFORK = 1 << 0,
  LIBCRUN_నిరంతరం_OPTIONS_KEEP = 1 << 1,
};

గణన
{
  LIBCRUN_CREATE_OPTIONS_PREFORK = 1 << 0,
};

నిర్మాణం libcrun_container_s
{
  /* Container parsed from the runtime json file.  */
  runtime_spec_schema_config_schema *container_def;

  uid_t host_uid;
  gid_t host_gid;

  /* Whether crun has CAP_SYS_PTRACE in its user namespace.  */
  bool host_has_cap_sys_ptrace;

  uid_t container_uid;
  gid_t container_gid;

  అక్షరం *config_file;
  అక్షరం *config_file_content;

  string_map *annotations;

  పూర్ణాంకం proc_fd;

  శూన్య *వ్యక్తిగత_దత్తాంశం;
  శూన్య (*cleanup_వ్యక్తిగత_దత్తాంశం) (శూన్య *వ్యక్తిగత_దత్తాంశం);
  నిర్మాణం libcrun_context_s *context;
};

నిర్మాణం libcrun_container_స్థితి_s;
రకనిర్వచనం నిర్మాణం libcrun_container_స్థితి_s libcrun_container_స్థితి_t;

రకనిర్వచనం నిర్మాణం libcrun_container_s libcrun_container_t;
రకనిర్వచనం నిర్మాణం libcrun_context_s libcrun_context_t;

నిర్మాణం container_entrypoint_s;
నిర్మాణం cgroup_info_s
{
  bool v1;
  bool v2;
  bool systemd;
  bool systemd_user;
};

నిర్మాణం seccomp_info_s
{
  bool enabled;
  అక్షరం **actions;
  అక్షరం **operators;
  అక్షరం **archs;
};

నిర్మాణం apparmor_info_s
{
  bool enabled;
};

నిర్మాణం selinux_info_s
{
  bool enabled;
};

నిర్మాణం idmap_info_s
{
  bool enabled;
};

నిర్మాణం intel_rdt_s
{
  bool enabled;
};

నిర్మాణం net_devices_s
{
  bool enabled;
};

నిర్మాణం mount_ext_info_s
{
  నిర్మాణం idmap_info_s idmap;
};

నిర్మాణం జ్ఞాపకం_policy_info_s
{
  అక్షరం **బహుళకం;
  అక్షరం **flags;
};

నిర్మాణం linux_info_s
{
  అక్షరం **namespaces;
  అక్షరం **capabilities;
  నిర్మాణం cgroup_info_s cgroup;
  నిర్మాణం seccomp_info_s seccomp;
  నిర్మాణం apparmor_info_s apparmor;
  నిర్మాణం selinux_info_s selinux;
  నిర్మాణం mount_ext_info_s mount_ext;
  నిర్మాణం intel_rdt_s intel_rdt;
  నిర్మాణం net_devices_s net_devices;
  నిర్మాణం జ్ఞాపకం_policy_info_s జ్ఞాపకం_policy;
};

నిర్మాణం annotations_info_s
{
  అక్షరం *io_github_seccomp_libseccomp_సంస్కరణ;
  bool నిరంతరం_oci_crun_checkpoint_enabled;
  అక్షరం *నిరంతరం_oci_crun_commit;
  అక్షరం *నిరంతరం_oci_crun_సంస్కరణ;
  bool నిరంతరం_oci_crun_wasm;
};

నిర్మాణం features_info_s
{
  అక్షరం *oci_సంస్కరణ_min;
  అక్షరం *oci_సంస్కరణ_max;
  అక్షరం **hooks;
  అక్షరం **mount_options;
  నిర్మాణం linux_info_s linux;
  నిర్మాణం annotations_info_s annotations;
  అక్షరం **potentially_unsafe_annotations;
};

నిర్మాణం libcrun_checkpoint_restore_s
{
  అక్షరం *image_మార్గం;
  అక్షరం *work_మార్గం;
  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool shell_పని;
  bool ext_unix_sk;
  bool detach;
  bool file_locks;
  స్థిరరాశి అక్షరం *console_socket;
  అక్షరం *మాతృ_మార్గం;
  bool pre_dump;
  పూర్ణాంకం manage_cgroups_బహుళకం;
  పూర్ణాంకం జాలం_తాళం_పద్ధతి;
  అక్షరం *lsm_profile;
  అక్షరం *lsm_mount_context;
};
రకనిర్వచనం నిర్మాణం libcrun_checkpoint_restore_s libcrun_checkpoint_restore_t;

/* Public checkpoint/restore options.  Mirror of the definition in
   <libcrun.h>; keep the two in sync.  */
గణన libcrun_cr_cgroups_బహుళకం
{
  LIBCRUN_CR_CG_బహుళకం_DEFAULT = 0, /* soft, matching runc */
  LIBCRUN_CR_CG_బహుళకం_SOFT,
  LIBCRUN_CR_CG_బహుళకం_IGNORE,
  LIBCRUN_CR_CG_బహుళకం_నిండిన,
  LIBCRUN_CR_CG_బహుళకం_STRICT,
};

గణన libcrun_cr_జాలం_తాళం_పద్ధతి
{
  LIBCRUN_CR_జాలం_తాళం_DEFAULT = 0, /* iptables */
  LIBCRUN_CR_జాలం_తాళం_IPTABLES,
  LIBCRUN_CR_జాలం_తాళం_NFTABLES,
  LIBCRUN_CR_జాలం_తాళం_SKIP,
};

నిర్మాణం libcrun_checkpoint_restore_options_s
{
  size_t struct_కొలత;

  స్థిరరాశి అక్షరం *image_మార్గం;
  స్థిరరాశి అక్షరం *work_మార్గం;
  స్థిరరాశి అక్షరం *మాతృ_మార్గం;
  స్థిరరాశి అక్షరం *console_socket;
  స్థిరరాశి అక్షరం *lsm_profile;
  స్థిరరాశి అక్షరం *lsm_mount_context;

  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool ext_unix_sk;
  bool shell_పని;
  bool file_locks;
  bool pre_dump;
  bool detach;

  పూర్ణాంకం manage_cgroups_బహుళకం; /* enum libcrun_cr_cgroups_mode */
  పూర్ణాంకం జాలం_తాళం_పద్ధతి; /* enum libcrun_cr_network_lock_method */
};

LIBCRUN_ప్రజా libcrun_container_t *libcrun_container_ఎక్కించు_from_file (స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err);

LIBCRUN_ప్రజా libcrun_container_t *libcrun_container_ఎక్కించు_from_జ్ఞాపకం (స్థిరరాశి అక్షరం *json, libcrun_error_t *err);

LIBCRUN_ప్రజా శూన్య libcrun_container_free (libcrun_container_t *);

/* Return the configuration of CONTAINER as a JSON string owned by CONTAINER.
   The document is generated on demand and cached, so the first call mutates
   CONTAINER and must not race with other users of the same handle.  */
LIBCRUN_ప్రజా స్థిరరాశి అక్షరం *libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err);
LIBCRUN_ప్రజా స్థిరరాశి అక్షరం *libcrun_container_get_annotation (libcrun_container_t *container, స్థిరరాశి అక్షరం *కీలకం);
LIBCRUN_ప్రజా size_t libcrun_container_get_annotations_len (libcrun_container_t *container);
LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_get_annotation_at (libcrun_container_t *container, size_t సూచీ,
                                                        స్థిరరాశి అక్షరం **కీలకం, స్థిరరాశి అక్షరం **విలువ);
LIBCRUN_ప్రజా uid_t libcrun_container_get_uid (libcrun_container_t *container);
LIBCRUN_ప్రజా gid_t libcrun_container_get_gid (libcrun_container_t *container);

LIBCRUN_ప్రజా libcrun_context_t *libcrun_context_new (స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *state_మూలం, libcrun_error_t *err);
LIBCRUN_ప్రజా శూన్య libcrun_context_free (libcrun_context_t *ctx);

LIBCRUN_ప్రజా శూన్య libcrun_context_సమితి_id (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *విలువ);
LIBCRUN_ప్రజా శూన్య libcrun_context_సమితి_state_మూలం (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *విలువ);
LIBCRUN_ప్రజా స్థిరరాశి అక్షరం *libcrun_context_get_state_మూలం (libcrun_context_t *ctx);
LIBCRUN_ప్రజా శూన్య libcrun_context_సమితి_bundle (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *విలువ);
LIBCRUN_ప్రజా శూన్య libcrun_context_సమితి_console_socket (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *విలువ);
LIBCRUN_ప్రజా శూన్య libcrun_context_సమితి_pid_file (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *విలువ);
LIBCRUN_ప్రజా స్థిరరాశి అక్షరం *libcrun_context_get_pid_file (libcrun_context_t *ctx);
LIBCRUN_ప్రజా శూన్య libcrun_context_సమితి_notify_socket (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *విలువ);
LIBCRUN_ప్రజా శూన్య libcrun_context_సమితి_handler (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *విలువ);
LIBCRUN_ప్రజా శూన్య libcrun_context_సమితి_preserve_fds (libcrun_context_t *ctx, పూర్ణాంకం preserve_fds);
LIBCRUN_ప్రజా పూర్ణాంకం libcrun_context_get_preserve_fds (libcrun_context_t *ctx);
LIBCRUN_ప్రజా శూన్య libcrun_context_సమితి_listen_fds (libcrun_context_t *ctx, పూర్ణాంకం listen_fds);
LIBCRUN_ప్రజా శూన్య libcrun_context_సమితి_వాదనలు (libcrun_context_t *ctx, పూర్ణాంకం argc, అక్షరం **argv);
LIBCRUN_ప్రజా శూన్య libcrun_context_సమితి_systemd_cgroup (libcrun_context_t *ctx, bool విలువ);
LIBCRUN_ప్రజా శూన్య libcrun_context_సమితి_detach (libcrun_context_t *ctx, bool విలువ);
LIBCRUN_ప్రజా శూన్య libcrun_context_సమితి_no_new_keyring (libcrun_context_t *ctx, bool విలువ);
LIBCRUN_ప్రజా శూన్య libcrun_context_సమితి_no_ఇరుసు (libcrun_context_t *ctx, bool విలువ);
LIBCRUN_ప్రజా శూన్య libcrun_context_సమితి_force_no_cgroup (libcrun_context_t *ctx, bool విలువ);
LIBCRUN_ప్రజా శూన్య libcrun_context_సమితి_output_handler (libcrun_context_t *ctx, crun_output_handler handler, శూన్య *arg);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_context_init_logging (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *సంవర్గమానం, libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_నిరంతరం (libcrun_context_t *context, libcrun_container_t *container,
                                          గుర్తులేని పూర్ణాంకం options, libcrun_error_t *error);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_delete (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, bool force,
                                             libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_kill (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *సంకేతం,
                                           libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_killall (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *సంకేతం,
                                              libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container,
                                             గుర్తులేని పూర్ణాంకం options, libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_ప్రారంభం (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_state (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, FILE *బహిర్గతం,
                                            libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_state_json (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, అక్షరం **బహిర్గతం,
                                                 libcrun_error_t *err);

పూర్ణాంకం libcrun_container_notify_handler (నిర్మాణం container_entrypoint_s *వాదనలు,
                                      గణన handler_configure_phase phase,
                                      libcrun_container_t *container, స్థిరరాశి అక్షరం *rootfs,
                                      libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_get_container_state_string (స్థిరరాశి అక్షరం *id, libcrun_container_స్థితి_t *స్థితి_2,
                                                       స్థిరరాశి అక్షరం *state_మూలం, స్థిరరాశి అక్షరం **container_స్థితి,
                                                       పూర్ణాంకం *running, libcrun_error_t *err);

నిర్మాణం libcrun_container_exec_options_s
{
  size_t struct_కొలత;
  runtime_spec_schema_config_schema_process *process;
  స్థిరరాశి అక్షరం *మార్గం;
  స్థిరరాశి అక్షరం *cgroup;
  bool విలీనం_env;
};

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_ఎక్కించు_process_from_file (స్థిరరాశి అక్షరం *మార్గం,
                                                   runtime_spec_schema_config_schema_process **process,
                                                   libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_exec_with_options (libcrun_context_t *context, స్థిరరాశి అక్షరం *id,
                                                        నిర్మాణం libcrun_container_exec_options_s *opts,
                                                        libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_exec (libcrun_context_t *context, స్థిరరాశి అక్షరం *id,
                                           runtime_spec_schema_config_schema_process *process, libcrun_error_t *err);

నిర్మాణం libcrun_exec_options_s
{
  size_t struct_కొలత;
  స్థిరరాశి అక్షరం *process_json; /* OCI Process JSON */
  స్థిరరాశి అక్షరం *cgroup;
  bool విలీనం_env;
};

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_exec_json (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *process_json,
                                                libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_exec_with_options_json (libcrun_context_t *context, స్థిరరాశి అక్షరం *id,
                                                             స్థిరరాశి నిర్మాణం libcrun_exec_options_s *opts,
                                                             libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_exec_process_file (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *మార్గం,
                                                        libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_update (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *content,
                                             size_t len, libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_update_from_file (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *file,
                                                       libcrun_error_t *err);

నిర్మాణం libcrun_update_విలువ_s
{
  స్థిరరాశి అక్షరం *section;
  స్థిరరాశి అక్షరం *name;
  bool numeric;
  స్థిరరాశి అక్షరం *విలువ;
};

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_update_from_విలువలు (libcrun_context_t *context, స్థిరరాశి అక్షరం *id,
                                                         నిర్మాణం libcrun_update_విలువ_s *విలువలు, size_t len,
                                                         libcrun_error_t *err);

నిర్మాణం libcrun_intel_rdt_update
{
  స్థిరరాశి అక్షరం *l3_cache_schema;
  స్థిరరాశి అక్షరం *mem_bw_schema;
  అక్షరం *స్థిరరాశి *schemata;
};

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_update_intel_rdt (libcrun_context_t *context, స్థిరరాశి అక్షరం *id,
                                                       నిర్మాణం libcrun_intel_rdt_update *update, libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_get_features (libcrun_context_t *context, నిర్మాణం features_info_s **info,
                                                   libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_get_features_json (libcrun_context_t *context, అక్షరం **బహిర్గతం, libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_pause (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_unpause (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_checkpoint (libcrun_context_t *context, స్థిరరాశి అక్షరం *id,
                                                 నిర్మాణం libcrun_checkpoint_restore_options_s *cr_options,
                                                 libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_restore (libcrun_context_t *context, స్థిరరాశి అక్షరం *id,
                                              నిర్మాణం libcrun_checkpoint_restore_options_s *cr_options,
                                              libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_read_pids (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, bool recurse, pid_t **pids, libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_write_json_containers_list (libcrun_context_t *context, FILE *బహిర్గతం, libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_list_json (libcrun_context_t *context, అక్షరం **బహిర్గతం, libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_కూడు_mounts_from_file (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *file,
                                                           libcrun_error_t *err);

LIBCRUN_ప్రజా పూర్ణాంకం libcrun_container_remove_mounts_from_file (libcrun_context_t *context, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *file,
                                                              libcrun_error_t *err);

// Not part of the public API, just a method in container.c we need to access from linux.c
శూన్య get_మూలం_లోపల_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid,
                             uid_t *uid, gid_t *gid);

స్థిర అంతఃవరుస శూన్య
cleanup_containerp (libcrun_container_t **c)
{
  libcrun_container_t *container = *c;
  libcrun_container_free (container);
}

#నిర్వచించు cleanup_container __attribute__ ((cleanup (cleanup_containerp)))

స్థిర అంతఃవరుస శూన్య
cleanup_struct_features_free (నిర్మాణం features_info_s **info)
{
  size_t i;
  నిర్మాణం features_info_s *ptr;
  ఒకవేళ (info == ఖాళీ || *info == ఖాళీ)
    తిరిగి;

  ptr = *info;

  // Free oci_version_min if it is not NULL
  ఒకవేళ (ptr->oci_సంస్కరణ_min != ఖాళీ)
    {
      విడుదల (ptr->oci_సంస్కరణ_min);
      ptr->oci_సంస్కరణ_min = ఖాళీ; // Set to NULL after freeing
    }

  // Free oci_version_max if it is not NULL
  ఒకవేళ (ptr->oci_సంస్కరణ_max != ఖాళీ)
    {
      విడుదల (ptr->oci_సంస్కరణ_max);
      ptr->oci_సంస్కరణ_max = ఖాళీ; // Set to NULL after freeing
    }
  ఒకవేళ (ptr->hooks != ఖాళీ)
    {
      కొరకు (i = 0; ptr->hooks[i] != ఖాళీ; i++)
        విడుదల (ptr->hooks[i]);
      విడుదల (ptr->hooks);
    }
  ఒకవేళ (ptr->mount_options != ఖాళీ)
    {
      కొరకు (i = 0; ptr->mount_options[i] != ఖాళీ; i++)
        విడుదల (ptr->mount_options[i]);
      విడుదల (ptr->mount_options);
    }
  ఒకవేళ (ptr->linux.namespaces != ఖాళీ)
    {
      కొరకు (i = 0; ptr->linux.namespaces[i] != ఖాళీ; i++)
        విడుదల (ptr->linux.namespaces[i]);
      విడుదల (ptr->linux.namespaces);
    }

  ఒకవేళ (ptr->linux.capabilities != ఖాళీ)
    {
      కొరకు (size_t i = 0; ptr->linux.capabilities[i] != ఖాళీ; i++)
        విడుదల (ptr->linux.capabilities[i]);
      విడుదల (ptr->linux.capabilities);
    }
  ఒకవేళ (ptr->linux.seccomp.actions != ఖాళీ)
    {
      కొరకు (i = 0; ptr->linux.seccomp.actions[i] != ఖాళీ; i++)
        విడుదల (ptr->linux.seccomp.actions[i]);
      విడుదల (ptr->linux.seccomp.actions);
    }
  ఒకవేళ (ptr->linux.seccomp.operators != ఖాళీ)
    {
      కొరకు (i = 0; ptr->linux.seccomp.operators[i] != ఖాళీ; i++)
        విడుదల (ptr->linux.seccomp.operators[i]);
      విడుదల (ptr->linux.seccomp.operators);
    }

  విడుదల (ptr);
  *info = ఖాళీ;
}

#నిర్వచించు cleanup_struct_features __attribute__ ((cleanup (cleanup_struct_features_free)))

#అయితే_ముగింపు
