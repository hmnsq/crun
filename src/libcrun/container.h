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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CONTAINER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CONTAINER_H

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ocispec/runtime_spec_schema_config_schema.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "error.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "string_map.h"

ਗਿਣਤੀ handler_configure_phase
{
  HANDLER_CONFIGURE_BEFORE_MOUNTS = 1,
  HANDLER_CONFIGURE_AFTER_MOUNTS,
  HANDLER_CONFIGURE_MOUNTS,
  HANDLER_CONFIGURE_BEFORE_USERNS
};

ਬਣਤਰ custom_handler_manager_s;

ਬਣਤਰ libcrun_context_s
{
  ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ;
  ਅਚਲ ਅੱਖਰ *id;
  ਅਚਲ ਅੱਖਰ *bundle;
  ਅਚਲ ਅੱਖਰ *console_socket;
  ਅਚਲ ਅੱਖਰ *pid_file;
  ਅਚਲ ਅੱਖਰ *notify_socket;
  ਅਚਲ ਅੱਖਰ *handler;
  ਅੱਖਰ *owned_state_ਜੜ੍ਹ;
  ਅੱਖਰ *owned_id;
  ਅੱਖਰ *owned_bundle;
  ਅੱਖਰ *owned_console_socket;
  ਅੱਖਰ *owned_pid_file;
  ਅੱਖਰ *owned_notify_socket;
  ਅੱਖਰ *owned_handler;
  ਪੂਰਨ preserve_fds;
  // For some use-cases we need differentiation between preserve_fds and listen_fds.
  // Following context variable makes sure we get exact value of listen_fds irrespective of preserve_fds.
  ਪੂਰਨ listen_fds;

  crun_output_handler output_handler;
  ਖਾਲੀ *output_handler_arg;

  ਪੂਰਨ fifo_exec_wait_fd;

  bool systemd_cgroup;
  bool detach;
  bool no_new_keyring;
  bool force_no_cgroup;
  bool no_ਧੁਰਾ;
  bool no_open_ਰੁੱਖ_namespace;

  ਅੱਖਰ **argv;
  ਪੂਰਨ argc;

  ਬਣਤਰ custom_handler_manager_s *handler_manager;
};

ਗਿਣਤੀ
{
  LIBCRUN_ਲੜੀਵਾਰ_OPTIONS_PREFORK = 1 << 0,
  LIBCRUN_ਲੜੀਵਾਰ_OPTIONS_KEEP = 1 << 1,
};

ਗਿਣਤੀ
{
  LIBCRUN_CREATE_OPTIONS_PREFORK = 1 << 0,
};

ਬਣਤਰ libcrun_container_s
{
  /* Container parsed from the runtime json file.  */
  runtime_spec_schema_config_schema *container_def;

  uid_t host_uid;
  gid_t host_gid;

  /* Whether crun has CAP_SYS_PTRACE in its user namespace.  */
  bool host_has_cap_sys_ptrace;

  uid_t container_uid;
  gid_t container_gid;

  ਅੱਖਰ *config_file;
  ਅੱਖਰ *config_file_content;

  string_map *annotations;

  ਪੂਰਨ proc_fd;

  ਖਾਲੀ *ਨਿੱਜੀ_ਅੰਕੜੇ;
  ਖਾਲੀ (*cleanup_ਨਿੱਜੀ_ਅੰਕੜੇ) (ਖਾਲੀ *ਨਿੱਜੀ_ਅੰਕੜੇ);
  ਬਣਤਰ libcrun_context_s *context;
};

ਬਣਤਰ libcrun_container_ਸਥਿਤੀ_s;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ libcrun_container_ਸਥਿਤੀ_s libcrun_container_ਸਥਿਤੀ_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ libcrun_container_s libcrun_container_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ libcrun_context_s libcrun_context_t;

ਬਣਤਰ container_entrypoint_s;
ਬਣਤਰ cgroup_info_s
{
  bool v1;
  bool v2;
  bool systemd;
  bool systemd_user;
};

ਬਣਤਰ seccomp_info_s
{
  bool enabled;
  ਅੱਖਰ **actions;
  ਅੱਖਰ **operators;
  ਅੱਖਰ **archs;
};

ਬਣਤਰ apparmor_info_s
{
  bool enabled;
};

ਬਣਤਰ selinux_info_s
{
  bool enabled;
};

ਬਣਤਰ idmap_info_s
{
  bool enabled;
};

ਬਣਤਰ intel_rdt_s
{
  bool enabled;
};

ਬਣਤਰ net_devices_s
{
  bool enabled;
};

ਬਣਤਰ mount_ext_info_s
{
  ਬਣਤਰ idmap_info_s idmap;
};

ਬਣਤਰ ਸਮ੍ਰਿਤੀ_policy_info_s
{
  ਅੱਖਰ **ਬਹੁਲਕ;
  ਅੱਖਰ **flags;
};

ਬਣਤਰ linux_info_s
{
  ਅੱਖਰ **namespaces;
  ਅੱਖਰ **capabilities;
  ਬਣਤਰ cgroup_info_s cgroup;
  ਬਣਤਰ seccomp_info_s seccomp;
  ਬਣਤਰ apparmor_info_s apparmor;
  ਬਣਤਰ selinux_info_s selinux;
  ਬਣਤਰ mount_ext_info_s mount_ext;
  ਬਣਤਰ intel_rdt_s intel_rdt;
  ਬਣਤਰ net_devices_s net_devices;
  ਬਣਤਰ ਸਮ੍ਰਿਤੀ_policy_info_s ਸਮ੍ਰਿਤੀ_policy;
};

ਬਣਤਰ annotations_info_s
{
  ਅੱਖਰ *io_github_seccomp_libseccomp_ਸੰਸਕਰਣ;
  bool ਲੜੀਵਾਰ_oci_crun_checkpoint_enabled;
  ਅੱਖਰ *ਲੜੀਵਾਰ_oci_crun_commit;
  ਅੱਖਰ *ਲੜੀਵਾਰ_oci_crun_ਸੰਸਕਰਣ;
  bool ਲੜੀਵਾਰ_oci_crun_wasm;
};

ਬਣਤਰ features_info_s
{
  ਅੱਖਰ *oci_ਸੰਸਕਰਣ_min;
  ਅੱਖਰ *oci_ਸੰਸਕਰਣ_max;
  ਅੱਖਰ **hooks;
  ਅੱਖਰ **mount_options;
  ਬਣਤਰ linux_info_s linux;
  ਬਣਤਰ annotations_info_s annotations;
  ਅੱਖਰ **potentially_unsafe_annotations;
};

ਬਣਤਰ libcrun_checkpoint_restore_s
{
  ਅੱਖਰ *image_ਰਸਤਾ;
  ਅੱਖਰ *work_ਰਸਤਾ;
  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool shell_ਕੰਮ;
  bool ext_unix_sk;
  bool detach;
  bool file_locks;
  ਅਚਲ ਅੱਖਰ *console_socket;
  ਅੱਖਰ *ਮਾਪਾ_ਰਸਤਾ;
  bool pre_dump;
  ਪੂਰਨ manage_cgroups_ਬਹੁਲਕ;
  ਪੂਰਨ ਜਾਲ_ਤਾਲਾ_ਵਿਧੀ;
  ਅੱਖਰ *lsm_profile;
  ਅੱਖਰ *lsm_mount_context;
};
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ libcrun_checkpoint_restore_s libcrun_checkpoint_restore_t;

/* Public checkpoint/restore options.  Mirror of the definition in
   <libcrun.h>; keep the two in sync.  */
ਗਿਣਤੀ libcrun_cr_cgroups_ਬਹੁਲਕ
{
  LIBCRUN_CR_CG_ਬਹੁਲਕ_DEFAULT = 0, /* soft, matching runc */
  LIBCRUN_CR_CG_ਬਹੁਲਕ_SOFT,
  LIBCRUN_CR_CG_ਬਹੁਲਕ_IGNORE,
  LIBCRUN_CR_CG_ਬਹੁਲਕ_ਭਰਿਆ,
  LIBCRUN_CR_CG_ਬਹੁਲਕ_STRICT,
};

ਗਿਣਤੀ libcrun_cr_ਜਾਲ_ਤਾਲਾ_ਵਿਧੀ
{
  LIBCRUN_CR_ਜਾਲ_ਤਾਲਾ_DEFAULT = 0, /* iptables */
  LIBCRUN_CR_ਜਾਲ_ਤਾਲਾ_IPTABLES,
  LIBCRUN_CR_ਜਾਲ_ਤਾਲਾ_NFTABLES,
  LIBCRUN_CR_ਜਾਲ_ਤਾਲਾ_SKIP,
};

ਬਣਤਰ libcrun_checkpoint_restore_options_s
{
  size_t struct_ਮਾਪ;

  ਅਚਲ ਅੱਖਰ *image_ਰਸਤਾ;
  ਅਚਲ ਅੱਖਰ *work_ਰਸਤਾ;
  ਅਚਲ ਅੱਖਰ *ਮਾਪਾ_ਰਸਤਾ;
  ਅਚਲ ਅੱਖਰ *console_socket;
  ਅਚਲ ਅੱਖਰ *lsm_profile;
  ਅਚਲ ਅੱਖਰ *lsm_mount_context;

  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool ext_unix_sk;
  bool shell_ਕੰਮ;
  bool file_locks;
  bool pre_dump;
  bool detach;

  ਪੂਰਨ manage_cgroups_ਬਹੁਲਕ; /* enum libcrun_cr_cgroups_mode */
  ਪੂਰਨ ਜਾਲ_ਤਾਲਾ_ਵਿਧੀ; /* enum libcrun_cr_network_lock_method */
};

LIBCRUN_ਜਨਤਕ libcrun_container_t *libcrun_container_ਲੱਦੋ_from_file (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ libcrun_container_t *libcrun_container_ਲੱਦੋ_from_ਸਮ੍ਰਿਤੀ (ਅਚਲ ਅੱਖਰ *json, libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_container_free (libcrun_container_t *);

/* Return the configuration of CONTAINER as a JSON string owned by CONTAINER.
   The document is generated on demand and cached, so the first call mutates
   CONTAINER and must not race with other users of the same handle.  */
LIBCRUN_ਜਨਤਕ ਅਚਲ ਅੱਖਰ *libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err);
LIBCRUN_ਜਨਤਕ ਅਚਲ ਅੱਖਰ *libcrun_container_get_annotation (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ);
LIBCRUN_ਜਨਤਕ size_t libcrun_container_get_annotations_len (libcrun_container_t *container);
LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_get_annotation_at (libcrun_container_t *container, size_t ਸੂਚਕਾਂਕ,
                                                        ਅਚਲ ਅੱਖਰ **ਕੁੰਜੀ, ਅਚਲ ਅੱਖਰ **ਮੁੱਲ);
LIBCRUN_ਜਨਤਕ uid_t libcrun_container_get_uid (libcrun_container_t *container);
LIBCRUN_ਜਨਤਕ gid_t libcrun_container_get_gid (libcrun_container_t *container);

LIBCRUN_ਜਨਤਕ libcrun_context_t *libcrun_context_new (ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, libcrun_error_t *err);
LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_context_free (libcrun_context_t *ctx);

LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_context_ਸੈੱਟ_id (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ);
LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_context_ਸੈੱਟ_state_ਜੜ੍ਹ (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ);
LIBCRUN_ਜਨਤਕ ਅਚਲ ਅੱਖਰ *libcrun_context_get_state_ਜੜ੍ਹ (libcrun_context_t *ctx);
LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_context_ਸੈੱਟ_bundle (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ);
LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_context_ਸੈੱਟ_console_socket (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ);
LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_context_ਸੈੱਟ_pid_file (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ);
LIBCRUN_ਜਨਤਕ ਅਚਲ ਅੱਖਰ *libcrun_context_get_pid_file (libcrun_context_t *ctx);
LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_context_ਸੈੱਟ_notify_socket (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ);
LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_context_ਸੈੱਟ_handler (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ);
LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_context_ਸੈੱਟ_preserve_fds (libcrun_context_t *ctx, ਪੂਰਨ preserve_fds);
LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_context_get_preserve_fds (libcrun_context_t *ctx);
LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_context_ਸੈੱਟ_listen_fds (libcrun_context_t *ctx, ਪੂਰਨ listen_fds);
LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_context_ਸੈੱਟ_ਦਲੀਲਾਂ (libcrun_context_t *ctx, ਪੂਰਨ argc, ਅੱਖਰ **argv);
LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_context_ਸੈੱਟ_systemd_cgroup (libcrun_context_t *ctx, bool ਮੁੱਲ);
LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_context_ਸੈੱਟ_detach (libcrun_context_t *ctx, bool ਮੁੱਲ);
LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_context_ਸੈੱਟ_no_new_keyring (libcrun_context_t *ctx, bool ਮੁੱਲ);
LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_context_ਸੈੱਟ_no_ਧੁਰਾ (libcrun_context_t *ctx, bool ਮੁੱਲ);
LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_context_ਸੈੱਟ_force_no_cgroup (libcrun_context_t *ctx, bool ਮੁੱਲ);
LIBCRUN_ਜਨਤਕ ਖਾਲੀ libcrun_context_ਸੈੱਟ_output_handler (libcrun_context_t *ctx, crun_output_handler handler, ਖਾਲੀ *arg);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_context_init_logging (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *ਲਘੂਗਣਕ, libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_ਲੜੀਵਾਰ (libcrun_context_t *context, libcrun_container_t *container,
                                          ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ options, libcrun_error_t *error);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_delete (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, bool force,
                                             libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_kill (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *ਸੰਕੇਤ,
                                           libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_killall (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *ਸੰਕੇਤ,
                                              libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container,
                                             ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ options, libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_ਸ਼ੁਰੂ (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_state (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, FILE *ਬਾਹਰਲਾ,
                                            libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_state_json (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, ਅੱਖਰ **ਬਾਹਰਲਾ,
                                                 libcrun_error_t *err);

ਪੂਰਨ libcrun_container_notify_handler (ਬਣਤਰ container_entrypoint_s *ਦਲੀਲਾਂ,
                                      ਗਿਣਤੀ handler_configure_phase phase,
                                      libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *rootfs,
                                      libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_get_container_state_string (ਅਚਲ ਅੱਖਰ *id, libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ,
                                                       ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ **container_ਸਥਿਤੀ,
                                                       ਪੂਰਨ *running, libcrun_error_t *err);

ਬਣਤਰ libcrun_container_exec_options_s
{
  size_t struct_ਮਾਪ;
  runtime_spec_schema_config_schema_process *process;
  ਅਚਲ ਅੱਖਰ *ਰਸਤਾ;
  ਅਚਲ ਅੱਖਰ *cgroup;
  bool ਮਿਲਾਓ_env;
};

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_ਲੱਦੋ_process_from_file (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ,
                                                   runtime_spec_schema_config_schema_process **process,
                                                   libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_exec_with_options (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id,
                                                        ਬਣਤਰ libcrun_container_exec_options_s *opts,
                                                        libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_exec (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id,
                                           runtime_spec_schema_config_schema_process *process, libcrun_error_t *err);

ਬਣਤਰ libcrun_exec_options_s
{
  size_t struct_ਮਾਪ;
  ਅਚਲ ਅੱਖਰ *process_json; /* OCI Process JSON */
  ਅਚਲ ਅੱਖਰ *cgroup;
  bool ਮਿਲਾਓ_env;
};

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_exec_json (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *process_json,
                                                libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_exec_with_options_json (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id,
                                                             ਅਚਲ ਬਣਤਰ libcrun_exec_options_s *opts,
                                                             libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_exec_process_file (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ,
                                                        libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_update (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *content,
                                             size_t len, libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_update_from_file (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *file,
                                                       libcrun_error_t *err);

ਬਣਤਰ libcrun_update_ਮੁੱਲ_s
{
  ਅਚਲ ਅੱਖਰ *section;
  ਅਚਲ ਅੱਖਰ *name;
  bool numeric;
  ਅਚਲ ਅੱਖਰ *ਮੁੱਲ;
};

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_update_from_ਮੁੱਲਾਂ (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id,
                                                         ਬਣਤਰ libcrun_update_ਮੁੱਲ_s *ਮੁੱਲਾਂ, size_t len,
                                                         libcrun_error_t *err);

ਬਣਤਰ libcrun_intel_rdt_update
{
  ਅਚਲ ਅੱਖਰ *l3_cache_schema;
  ਅਚਲ ਅੱਖਰ *mem_bw_schema;
  ਅੱਖਰ *ਅਚਲ *schemata;
};

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_update_intel_rdt (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id,
                                                       ਬਣਤਰ libcrun_intel_rdt_update *update, libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_get_features (libcrun_context_t *context, ਬਣਤਰ features_info_s **info,
                                                   libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_get_features_json (libcrun_context_t *context, ਅੱਖਰ **ਬਾਹਰਲਾ, libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_pause (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_unpause (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_checkpoint (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id,
                                                 ਬਣਤਰ libcrun_checkpoint_restore_options_s *cr_options,
                                                 libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_restore (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id,
                                              ਬਣਤਰ libcrun_checkpoint_restore_options_s *cr_options,
                                              libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_read_pids (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, bool recurse, pid_t **pids, libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_write_json_containers_list (libcrun_context_t *context, FILE *ਬਾਹਰਲਾ, libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_list_json (libcrun_context_t *context, ਅੱਖਰ **ਬਾਹਰਲਾ, libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_ਜੋੜੋ_mounts_from_file (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *file,
                                                           libcrun_error_t *err);

LIBCRUN_ਜਨਤਕ ਪੂਰਨ libcrun_container_remove_mounts_from_file (libcrun_context_t *context, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *file,
                                                              libcrun_error_t *err);

// Not part of the public API, just a method in container.c we need to access from linux.c
ਖਾਲੀ get_ਜੜ੍ਹ_ਵਿੱਚ_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid,
                             uid_t *uid, gid_t *gid);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cleanup_containerp (libcrun_container_t **c)
{
  libcrun_container_t *container = *c;
  libcrun_container_free (container);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_container __attribute__ ((cleanup (cleanup_containerp)))

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cleanup_struct_features_free (ਬਣਤਰ features_info_s **info)
{
  size_t i;
  ਬਣਤਰ features_info_s *ptr;
  ਜੇ (info == ਨਿਰਾ || *info == ਨਿਰਾ)
    ਵਾਪਸ;

  ptr = *info;

  // Free oci_version_min if it is not NULL
  ਜੇ (ptr->oci_ਸੰਸਕਰਣ_min != ਨਿਰਾ)
    {
      ਮੁਕਤ (ptr->oci_ਸੰਸਕਰਣ_min);
      ptr->oci_ਸੰਸਕਰਣ_min = ਨਿਰਾ; // Set to NULL after freeing
    }

  // Free oci_version_max if it is not NULL
  ਜੇ (ptr->oci_ਸੰਸਕਰਣ_max != ਨਿਰਾ)
    {
      ਮੁਕਤ (ptr->oci_ਸੰਸਕਰਣ_max);
      ptr->oci_ਸੰਸਕਰਣ_max = ਨਿਰਾ; // Set to NULL after freeing
    }
  ਜੇ (ptr->hooks != ਨਿਰਾ)
    {
      ਲਈ (i = 0; ptr->hooks[i] != ਨਿਰਾ; i++)
        ਮੁਕਤ (ptr->hooks[i]);
      ਮੁਕਤ (ptr->hooks);
    }
  ਜੇ (ptr->mount_options != ਨਿਰਾ)
    {
      ਲਈ (i = 0; ptr->mount_options[i] != ਨਿਰਾ; i++)
        ਮੁਕਤ (ptr->mount_options[i]);
      ਮੁਕਤ (ptr->mount_options);
    }
  ਜੇ (ptr->linux.namespaces != ਨਿਰਾ)
    {
      ਲਈ (i = 0; ptr->linux.namespaces[i] != ਨਿਰਾ; i++)
        ਮੁਕਤ (ptr->linux.namespaces[i]);
      ਮੁਕਤ (ptr->linux.namespaces);
    }

  ਜੇ (ptr->linux.capabilities != ਨਿਰਾ)
    {
      ਲਈ (size_t i = 0; ptr->linux.capabilities[i] != ਨਿਰਾ; i++)
        ਮੁਕਤ (ptr->linux.capabilities[i]);
      ਮੁਕਤ (ptr->linux.capabilities);
    }
  ਜੇ (ptr->linux.seccomp.actions != ਨਿਰਾ)
    {
      ਲਈ (i = 0; ptr->linux.seccomp.actions[i] != ਨਿਰਾ; i++)
        ਮੁਕਤ (ptr->linux.seccomp.actions[i]);
      ਮੁਕਤ (ptr->linux.seccomp.actions);
    }
  ਜੇ (ptr->linux.seccomp.operators != ਨਿਰਾ)
    {
      ਲਈ (i = 0; ptr->linux.seccomp.operators[i] != ਨਿਰਾ; i++)
        ਮੁਕਤ (ptr->linux.seccomp.operators[i]);
      ਮੁਕਤ (ptr->linux.seccomp.operators);
    }

  ਮੁਕਤ (ptr);
  *info = ਨਿਰਾ;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cleanup_struct_features __attribute__ ((cleanup (cleanup_struct_features_free)))

#ਜੇ_ਅੰਤ
