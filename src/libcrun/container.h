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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <ocispec/runtime_spec_schema_config_schema.h>
#ଅନ୍ତର୍ଭାଵ "error.h"
#ଅନ୍ତର୍ଭାଵ "string_map.h"

ଗଣନା handler_configure_phase
{
  HANDLER_CONFIGURE_BEFORE_MOUNTS = 1,
  HANDLER_CONFIGURE_AFTER_MOUNTS,
  HANDLER_CONFIGURE_MOUNTS,
  HANDLER_CONFIGURE_BEFORE_USERNS
};

ସଂରଚନା custom_handler_manager_s;

ସଂରଚନା libcrun_context_s
{
  ଅଚଳ ଅକ୍ଷର *state_ମୂଳ;
  ଅଚଳ ଅକ୍ଷର *id;
  ଅଚଳ ଅକ୍ଷର *bundle;
  ଅଚଳ ଅକ୍ଷର *console_socket;
  ଅଚଳ ଅକ୍ଷର *pid_file;
  ଅଚଳ ଅକ୍ଷର *notify_socket;
  ଅଚଳ ଅକ୍ଷର *handler;
  ଅକ୍ଷର *owned_state_ମୂଳ;
  ଅକ୍ଷର *owned_id;
  ଅକ୍ଷର *owned_bundle;
  ଅକ୍ଷର *owned_console_socket;
  ଅକ୍ଷର *owned_pid_file;
  ଅକ୍ଷର *owned_notify_socket;
  ଅକ୍ଷର *owned_handler;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା preserve_fds;
  // For some use-cases we need differentiation between preserve_fds and listen_fds.
  // Following context variable makes sure we get exact value of listen_fds irrespective of preserve_fds.
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା listen_fds;

  crun_output_handler output_handler;
  ଶୂନ୍ୟ *output_handler_arg;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fifo_exec_wait_fd;

  bool systemd_cgroup;
  bool detach;
  bool no_new_keyring;
  bool force_no_cgroup;
  bool no_pivot;
  bool no_open_ବୃକ୍ଷ_namespace;

  ଅକ୍ଷର **argv;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc;

  ସଂରଚନା custom_handler_manager_s *handler_manager;
};

ଗଣନା
{
  LIBCRUN_RUN_OPTIONS_PREFORK = 1 << 0,
  LIBCRUN_RUN_OPTIONS_KEEP = 1 << 1,
};

ଗଣନା
{
  LIBCRUN_CREATE_OPTIONS_PREFORK = 1 << 0,
};

ସଂରଚନା libcrun_container_s
{
  /* Container parsed from the runtime json file.  */
  runtime_spec_schema_config_schema *container_def;

  uid_t host_uid;
  gid_t host_gid;

  /* Whether crun has CAP_SYS_PTRACE in its user namespace.  */
  bool host_has_cap_sys_ptrace;

  uid_t container_uid;
  gid_t container_gid;

  ଅକ୍ଷର *config_file;
  ଅକ୍ଷର *config_file_content;

  string_map *annotations;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା proc_fd;

  ଶୂନ୍ୟ *private_data;
  ଶୂନ୍ୟ (*cleanup_private_data) (ଶୂନ୍ୟ *private_data);
  ସଂରଚନା libcrun_context_s *context;
};

ସଂରଚନା libcrun_container_status_s;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା libcrun_container_status_s libcrun_container_status_t;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା libcrun_container_s libcrun_container_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା libcrun_context_s libcrun_context_t;

ସଂରଚନା container_entrypoint_s;
ସଂରଚନା cgroup_info_s
{
  bool v1;
  bool v2;
  bool systemd;
  bool systemd_user;
};

ସଂରଚନା seccomp_info_s
{
  bool enabled;
  ଅକ୍ଷର **actions;
  ଅକ୍ଷର **operators;
  ଅକ୍ଷର **archs;
};

ସଂରଚନା apparmor_info_s
{
  bool enabled;
};

ସଂରଚନା selinux_info_s
{
  bool enabled;
};

ସଂରଚନା idmap_info_s
{
  bool enabled;
};

ସଂରଚନା intel_rdt_s
{
  bool enabled;
};

ସଂରଚନା net_devices_s
{
  bool enabled;
};

ସଂରଚନା mount_ext_info_s
{
  ସଂରଚନା idmap_info_s idmap;
};

ସଂରଚନା memory_policy_info_s
{
  ଅକ୍ଷର **mode;
  ଅକ୍ଷର **flags;
};

ସଂରଚନା linux_info_s
{
  ଅକ୍ଷର **namespaces;
  ଅକ୍ଷର **capabilities;
  ସଂରଚନା cgroup_info_s cgroup;
  ସଂରଚନା seccomp_info_s seccomp;
  ସଂରଚନା apparmor_info_s apparmor;
  ସଂରଚନା selinux_info_s selinux;
  ସଂରଚନା mount_ext_info_s mount_ext;
  ସଂରଚନା intel_rdt_s intel_rdt;
  ସଂରଚନା net_devices_s net_devices;
  ସଂରଚନା memory_policy_info_s memory_policy;
};

ସଂରଚନା annotations_info_s
{
  ଅକ୍ଷର *io_github_seccomp_libseccomp_version;
  bool run_oci_crun_checkpoint_enabled;
  ଅକ୍ଷର *run_oci_crun_commit;
  ଅକ୍ଷର *run_oci_crun_version;
  bool run_oci_crun_wasm;
};

ସଂରଚନା features_info_s
{
  ଅକ୍ଷର *oci_version_min;
  ଅକ୍ଷର *oci_version_max;
  ଅକ୍ଷର **hooks;
  ଅକ୍ଷର **mount_options;
  ସଂରଚନା linux_info_s linux;
  ସଂରଚନା annotations_info_s annotations;
  ଅକ୍ଷର **potentially_unsafe_annotations;
};

ସଂରଚନା libcrun_checkpoint_restore_s
{
  ଅକ୍ଷର *image_path;
  ଅକ୍ଷର *work_path;
  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool shell_job;
  bool ext_unix_sk;
  bool detach;
  bool file_locks;
  ଅଚଳ ଅକ୍ଷର *console_socket;
  ଅକ୍ଷର *parent_path;
  bool pre_dump;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା manage_cgroups_mode;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା network_lock_method;
  ଅକ୍ଷର *lsm_profile;
  ଅକ୍ଷର *lsm_mount_context;
};
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା libcrun_checkpoint_restore_s libcrun_checkpoint_restore_t;

/* Public checkpoint/restore options.  Mirror of the definition in
   <libcrun.h>; keep the two in sync.  */
ଗଣନା libcrun_cr_cgroups_mode
{
  LIBCRUN_CR_CG_MODE_DEFAULT = 0, /* soft, matching runc */
  LIBCRUN_CR_CG_MODE_SOFT,
  LIBCRUN_CR_CG_MODE_IGNORE,
  LIBCRUN_CR_CG_MODE_FULL,
  LIBCRUN_CR_CG_MODE_STRICT,
};

ଗଣନା libcrun_cr_network_lock_method
{
  LIBCRUN_CR_NETWORK_LOCK_DEFAULT = 0, /* iptables */
  LIBCRUN_CR_NETWORK_LOCK_IPTABLES,
  LIBCRUN_CR_NETWORK_LOCK_NFTABLES,
  LIBCRUN_CR_NETWORK_LOCK_SKIP,
};

ସଂରଚନା libcrun_checkpoint_restore_options_s
{
  size_t struct_size;

  ଅଚଳ ଅକ୍ଷର *image_path;
  ଅଚଳ ଅକ୍ଷର *work_path;
  ଅଚଳ ଅକ୍ଷର *parent_path;
  ଅଚଳ ଅକ୍ଷର *console_socket;
  ଅଚଳ ଅକ୍ଷର *lsm_profile;
  ଅଚଳ ଅକ୍ଷର *lsm_mount_context;

  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool ext_unix_sk;
  bool shell_job;
  bool file_locks;
  bool pre_dump;
  bool detach;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା manage_cgroups_mode; /* enum libcrun_cr_cgroups_mode */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା network_lock_method; /* enum libcrun_cr_network_lock_method */
};

LIBCRUN_PUBLIC libcrun_container_t *libcrun_container_load_from_file (ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err);

LIBCRUN_PUBLIC libcrun_container_t *libcrun_container_load_from_memory (ଅଚଳ ଅକ୍ଷର *json, libcrun_error_t *err);

LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_container_free (libcrun_container_t *);

/* Return the configuration of CONTAINER as a JSON string owned by CONTAINER.
   The document is generated on demand and cached, so the first call mutates
   CONTAINER and must not race with other users of the same handle.  */
LIBCRUN_PUBLIC ଅଚଳ ଅକ୍ଷର *libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err);
LIBCRUN_PUBLIC ଅଚଳ ଅକ୍ଷର *libcrun_container_get_annotation (libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *ଚାବି);
LIBCRUN_PUBLIC size_t libcrun_container_get_annotations_len (libcrun_container_t *container);
LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_get_annotation_at (libcrun_container_t *container, size_t index,
                                                        ଅଚଳ ଅକ୍ଷର **ଚାବି, ଅଚଳ ଅକ୍ଷର **ମୂଲ୍ୟ);
LIBCRUN_PUBLIC uid_t libcrun_container_get_uid (libcrun_container_t *container);
LIBCRUN_PUBLIC gid_t libcrun_container_get_gid (libcrun_container_t *container);

LIBCRUN_PUBLIC libcrun_context_t *libcrun_context_new (ଅଚଳ ଅକ୍ଷର *id, ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, libcrun_error_t *err);
LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_context_free (libcrun_context_t *ctx);

LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_context_set_id (libcrun_context_t *ctx, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ);
LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_context_set_state_ମୂଳ (libcrun_context_t *ctx, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ);
LIBCRUN_PUBLIC ଅଚଳ ଅକ୍ଷର *libcrun_context_get_state_ମୂଳ (libcrun_context_t *ctx);
LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_context_set_bundle (libcrun_context_t *ctx, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ);
LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_context_set_console_socket (libcrun_context_t *ctx, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ);
LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_context_set_pid_file (libcrun_context_t *ctx, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ);
LIBCRUN_PUBLIC ଅଚଳ ଅକ୍ଷର *libcrun_context_get_pid_file (libcrun_context_t *ctx);
LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_context_set_notify_socket (libcrun_context_t *ctx, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ);
LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_context_set_handler (libcrun_context_t *ctx, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ);
LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_context_set_preserve_fds (libcrun_context_t *ctx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା preserve_fds);
LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_context_get_preserve_fds (libcrun_context_t *ctx);
LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_context_set_listen_fds (libcrun_context_t *ctx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା listen_fds);
LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_context_set_args (libcrun_context_t *ctx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv);
LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_context_set_systemd_cgroup (libcrun_context_t *ctx, bool ମୂଲ୍ୟ);
LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_context_set_detach (libcrun_context_t *ctx, bool ମୂଲ୍ୟ);
LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_context_set_no_new_keyring (libcrun_context_t *ctx, bool ମୂଲ୍ୟ);
LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_context_set_no_pivot (libcrun_context_t *ctx, bool ମୂଲ୍ୟ);
LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_context_set_force_no_cgroup (libcrun_context_t *ctx, bool ମୂଲ୍ୟ);
LIBCRUN_PUBLIC ଶୂନ୍ୟ libcrun_context_set_output_handler (libcrun_context_t *ctx, crun_output_handler handler, ଶୂନ୍ୟ *arg);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_context_init_logging (libcrun_context_t *ctx, ଅଚଳ ଅକ୍ଷର *log, libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_run (libcrun_context_t *context, libcrun_container_t *container,
                                          unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା options, libcrun_error_t *error);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_delete (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, bool force,
                                             libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_kill (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, ଅଚଳ ଅକ୍ଷର *ସଙ୍କେତ,
                                           libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_killall (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, ଅଚଳ ଅକ୍ଷର *ସଙ୍କେତ,
                                              libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container,
                                             unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା options, libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_start (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_state (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, FILE *out,
                                            libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_state_json (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, ଅକ୍ଷର **out,
                                                 libcrun_error_t *err);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_notify_handler (ସଂରଚନା container_entrypoint_s *args,
                                      ଗଣନା handler_configure_phase phase,
                                      libcrun_container_t *container, ଅଚଳ ଅକ୍ଷର *rootfs,
                                      libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_get_container_state_string (ଅଚଳ ଅକ୍ଷର *id, libcrun_container_status_t *status,
                                                       ଅଚଳ ଅକ୍ଷର *state_ମୂଳ, ଅଚଳ ଅକ୍ଷର **container_status,
                                                       ପୂର୍ଣ୍ଣସଂଖ୍ୟା *running, libcrun_error_t *err);

ସଂରଚନା libcrun_container_exec_options_s
{
  size_t struct_size;
  runtime_spec_schema_config_schema_process *process;
  ଅଚଳ ଅକ୍ଷର *path;
  ଅଚଳ ଅକ୍ଷର *cgroup;
  bool merge_env;
};

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_load_process_from_file (ଅଚଳ ଅକ୍ଷର *path,
                                                   runtime_spec_schema_config_schema_process **process,
                                                   libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_exec_with_options (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id,
                                                        ସଂରଚନା libcrun_container_exec_options_s *opts,
                                                        libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_exec (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id,
                                           runtime_spec_schema_config_schema_process *process, libcrun_error_t *err);

ସଂରଚନା libcrun_exec_options_s
{
  size_t struct_size;
  ଅଚଳ ଅକ୍ଷର *process_json; /* OCI Process JSON */
  ଅଚଳ ଅକ୍ଷର *cgroup;
  bool merge_env;
};

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_exec_json (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, ଅଚଳ ଅକ୍ଷର *process_json,
                                                libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_exec_with_options_json (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id,
                                                             ଅଚଳ ସଂରଚନା libcrun_exec_options_s *opts,
                                                             libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_exec_process_file (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, ଅଚଳ ଅକ୍ଷର *path,
                                                        libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_update (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, ଅଚଳ ଅକ୍ଷର *content,
                                             size_t len, libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_update_from_file (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, ଅଚଳ ଅକ୍ଷର *file,
                                                       libcrun_error_t *err);

ସଂରଚନା libcrun_update_ମୂଲ୍ୟ_s
{
  ଅଚଳ ଅକ୍ଷର *section;
  ଅଚଳ ଅକ୍ଷର *name;
  bool numeric;
  ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ;
};

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_update_from_ମୂଲ୍ୟଗୁଡ଼ିକ (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id,
                                                         ସଂରଚନା libcrun_update_ମୂଲ୍ୟ_s *ମୂଲ୍ୟଗୁଡ଼ିକ, size_t len,
                                                         libcrun_error_t *err);

ସଂରଚନା libcrun_intel_rdt_update
{
  ଅଚଳ ଅକ୍ଷର *l3_cache_schema;
  ଅଚଳ ଅକ୍ଷର *mem_bw_schema;
  ଅକ୍ଷର *ଅଚଳ *schemata;
};

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_update_intel_rdt (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id,
                                                       ସଂରଚନା libcrun_intel_rdt_update *update, libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_get_features (libcrun_context_t *context, ସଂରଚନା features_info_s **info,
                                                   libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_get_features_json (libcrun_context_t *context, ଅକ୍ଷର **out, libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_pause (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_unpause (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_checkpoint (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id,
                                                 ସଂରଚନା libcrun_checkpoint_restore_options_s *cr_options,
                                                 libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_restore (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id,
                                              ସଂରଚନା libcrun_checkpoint_restore_options_s *cr_options,
                                              libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_read_pids (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, bool recurse, pid_t **pids, libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_write_json_containers_list (libcrun_context_t *context, FILE *out, libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_list_json (libcrun_context_t *context, ଅକ୍ଷର **out, libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_add_mounts_from_file (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, ଅଚଳ ଅକ୍ଷର *file,
                                                           libcrun_error_t *err);

LIBCRUN_PUBLIC ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_container_remove_mounts_from_file (libcrun_context_t *context, ଅଚଳ ଅକ୍ଷର *id, ଅଚଳ ଅକ୍ଷର *file,
                                                              libcrun_error_t *err);

// Not part of the public API, just a method in container.c we need to access from linux.c
ଶୂନ୍ୟ get_ମୂଳ_in_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid,
                             uid_t *uid, gid_t *gid);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
cleanup_containerp (libcrun_container_t **c)
{
  libcrun_container_t *container = *c;
  libcrun_container_free (container);
}

#define cleanup_container __attribute__ ((cleanup (cleanup_containerp)))

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
cleanup_struct_features_free (ସଂରଚନା features_info_s **info)
{
  size_t i;
  ସଂରଚନା features_info_s *ptr;
  ଯଦି (info == ଖାଲି || *info == ଖାଲି)
    ଫେରନ୍ତୁ;

  ptr = *info;

  // Free oci_version_min if it is not NULL
  ଯଦି (ptr->oci_version_min != ଖାଲି)
    {
      ମୁକ୍ତ (ptr->oci_version_min);
      ptr->oci_version_min = ଖାଲି; // Set to NULL after freeing
    }

  // Free oci_version_max if it is not NULL
  ଯଦି (ptr->oci_version_max != ଖାଲି)
    {
      ମୁକ୍ତ (ptr->oci_version_max);
      ptr->oci_version_max = ଖାଲି; // Set to NULL after freeing
    }
  ଯଦି (ptr->hooks != ଖାଲି)
    {
      ପାଇଁ (i = 0; ptr->hooks[i] != ଖାଲି; i++)
        ମୁକ୍ତ (ptr->hooks[i]);
      ମୁକ୍ତ (ptr->hooks);
    }
  ଯଦି (ptr->mount_options != ଖାଲି)
    {
      ପାଇଁ (i = 0; ptr->mount_options[i] != ଖାଲି; i++)
        ମୁକ୍ତ (ptr->mount_options[i]);
      ମୁକ୍ତ (ptr->mount_options);
    }
  ଯଦି (ptr->linux.namespaces != ଖାଲି)
    {
      ପାଇଁ (i = 0; ptr->linux.namespaces[i] != ଖାଲି; i++)
        ମୁକ୍ତ (ptr->linux.namespaces[i]);
      ମୁକ୍ତ (ptr->linux.namespaces);
    }

  ଯଦି (ptr->linux.capabilities != ଖାଲି)
    {
      ପାଇଁ (size_t i = 0; ptr->linux.capabilities[i] != ଖାଲି; i++)
        ମୁକ୍ତ (ptr->linux.capabilities[i]);
      ମୁକ୍ତ (ptr->linux.capabilities);
    }
  ଯଦି (ptr->linux.seccomp.actions != ଖାଲି)
    {
      ପାଇଁ (i = 0; ptr->linux.seccomp.actions[i] != ଖାଲି; i++)
        ମୁକ୍ତ (ptr->linux.seccomp.actions[i]);
      ମୁକ୍ତ (ptr->linux.seccomp.actions);
    }
  ଯଦି (ptr->linux.seccomp.operators != ଖାଲି)
    {
      ପାଇଁ (i = 0; ptr->linux.seccomp.operators[i] != ଖାଲି; i++)
        ମୁକ୍ତ (ptr->linux.seccomp.operators[i]);
      ମୁକ୍ତ (ptr->linux.seccomp.operators);
    }

  ମୁକ୍ତ (ptr);
  *info = ଖାଲି;
}

#define cleanup_struct_features __attribute__ ((cleanup (cleanup_struct_features_free)))

#endif
