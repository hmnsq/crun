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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CONTAINER_H
#ವ್ಯಾಖ್ಯಾನಿಸು CONTAINER_H

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <ocispec/runtime_spec_schema_config_schema.h>
#ಅನ್ತರ್ಭಾವ "error.h"
#ಅನ್ತರ್ಭಾವ "string_map.h"

ಎಣಿಕೆ handler_configure_phase
{
  HANDLER_CONFIGURE_BEFORE_MOUNTS = 1,
  HANDLER_CONFIGURE_AFTER_MOUNTS,
  HANDLER_CONFIGURE_MOUNTS,
  HANDLER_CONFIGURE_BEFORE_USERNS
};

ರಚನೆ custom_handler_manager_s;

ರಚನೆ libcrun_context_s
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *bundle;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *console_socket;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pid_file;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *notify_socket;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *handler;
  ಅಕ್ಷರ *owned_state_ಬೇರು;
  ಅಕ್ಷರ *owned_id;
  ಅಕ್ಷರ *owned_bundle;
  ಅಕ್ಷರ *owned_console_socket;
  ಅಕ್ಷರ *owned_pid_file;
  ಅಕ್ಷರ *owned_notify_socket;
  ಅಕ್ಷರ *owned_handler;
  ಪೂರ್ಣಾಂಕ preserve_fds;
  // For some use-cases we need differentiation between preserve_fds and listen_fds.
  // Following context variable makes sure we get exact value of listen_fds irrespective of preserve_fds.
  ಪೂರ್ಣಾಂಕ listen_fds;

  crun_output_handler output_handler;
  ಶೂನ್ಯ *output_handler_arg;

  ಪೂರ್ಣಾಂಕ fifo_exec_wait_fd;

  bool systemd_cgroup;
  bool detach;
  bool no_new_keyring;
  bool force_no_cgroup;
  bool no_ಅಚ್ಚು;
  bool no_open_ವೃಕ್ಷ_namespace;

  ಅಕ್ಷರ **argv;
  ಪೂರ್ಣಾಂಕ argc;

  ರಚನೆ custom_handler_manager_s *handler_manager;
};

ಎಣಿಕೆ
{
  LIBCRUN_ಸತತ_OPTIONS_PREFORK = 1 << 0,
  LIBCRUN_ಸತತ_OPTIONS_KEEP = 1 << 1,
};

ಎಣಿಕೆ
{
  LIBCRUN_CREATE_OPTIONS_PREFORK = 1 << 0,
};

ರಚನೆ libcrun_container_s
{
  /* Container parsed from the runtime json file.  */
  runtime_spec_schema_config_schema *container_def;

  uid_t host_uid;
  gid_t host_gid;

  /* Whether crun has CAP_SYS_PTRACE in its user namespace.  */
  bool host_has_cap_sys_ptrace;

  uid_t container_uid;
  gid_t container_gid;

  ಅಕ್ಷರ *config_file;
  ಅಕ್ಷರ *config_file_content;

  string_map *annotations;

  ಪೂರ್ಣಾಂಕ proc_fd;

  ಶೂನ್ಯ *ಖಾಸಗಿ_ದತ್ತಾಂಶ;
  ಶೂನ್ಯ (*cleanup_ಖಾಸಗಿ_ದತ್ತಾಂಶ) (ಶೂನ್ಯ *ಖಾಸಗಿ_ದತ್ತಾಂಶ);
  ರಚನೆ libcrun_context_s *context;
};

ರಚನೆ libcrun_container_ಸ್ಥಿತಿ_s;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ libcrun_container_ಸ್ಥಿತಿ_s libcrun_container_ಸ್ಥಿತಿ_t;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ libcrun_container_s libcrun_container_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ libcrun_context_s libcrun_context_t;

ರಚನೆ container_entrypoint_s;
ರಚನೆ cgroup_info_s
{
  bool v1;
  bool v2;
  bool systemd;
  bool systemd_user;
};

ರಚನೆ seccomp_info_s
{
  bool enabled;
  ಅಕ್ಷರ **actions;
  ಅಕ್ಷರ **operators;
  ಅಕ್ಷರ **archs;
};

ರಚನೆ apparmor_info_s
{
  bool enabled;
};

ರಚನೆ selinux_info_s
{
  bool enabled;
};

ರಚನೆ idmap_info_s
{
  bool enabled;
};

ರಚನೆ intel_rdt_s
{
  bool enabled;
};

ರಚನೆ net_devices_s
{
  bool enabled;
};

ರಚನೆ mount_ext_info_s
{
  ರಚನೆ idmap_info_s idmap;
};

ರಚನೆ ಸ್ಮರಣೆ_policy_info_s
{
  ಅಕ್ಷರ **ಬಹುಲಕ;
  ಅಕ್ಷರ **flags;
};

ರಚನೆ linux_info_s
{
  ಅಕ್ಷರ **namespaces;
  ಅಕ್ಷರ **capabilities;
  ರಚನೆ cgroup_info_s cgroup;
  ರಚನೆ seccomp_info_s seccomp;
  ರಚನೆ apparmor_info_s apparmor;
  ರಚನೆ selinux_info_s selinux;
  ರಚನೆ mount_ext_info_s mount_ext;
  ರಚನೆ intel_rdt_s intel_rdt;
  ರಚನೆ net_devices_s net_devices;
  ರಚನೆ ಸ್ಮರಣೆ_policy_info_s ಸ್ಮರಣೆ_policy;
};

ರಚನೆ annotations_info_s
{
  ಅಕ್ಷರ *io_github_seccomp_libseccomp_ಆವೃತ್ತಿ;
  bool ಸತತ_oci_crun_checkpoint_enabled;
  ಅಕ್ಷರ *ಸತತ_oci_crun_commit;
  ಅಕ್ಷರ *ಸತತ_oci_crun_ಆವೃತ್ತಿ;
  bool ಸತತ_oci_crun_wasm;
};

ರಚನೆ features_info_s
{
  ಅಕ್ಷರ *oci_ಆವೃತ್ತಿ_min;
  ಅಕ್ಷರ *oci_ಆವೃತ್ತಿ_max;
  ಅಕ್ಷರ **hooks;
  ಅಕ್ಷರ **mount_options;
  ರಚನೆ linux_info_s linux;
  ರಚನೆ annotations_info_s annotations;
  ಅಕ್ಷರ **potentially_unsafe_annotations;
};

ರಚನೆ libcrun_checkpoint_restore_s
{
  ಅಕ್ಷರ *image_ಮಾರ್ಗ;
  ಅಕ್ಷರ *work_ಮಾರ್ಗ;
  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool shell_ಕೆಲಸ;
  bool ext_unix_sk;
  bool detach;
  bool file_locks;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *console_socket;
  ಅಕ್ಷರ *ಪೋಷಕ_ಮಾರ್ಗ;
  bool pre_dump;
  ಪೂರ್ಣಾಂಕ manage_cgroups_ಬಹುಲಕ;
  ಪೂರ್ಣಾಂಕ ಜಾಲ_ಬೀಗ_ವಿಧಾನ;
  ಅಕ್ಷರ *lsm_profile;
  ಅಕ್ಷರ *lsm_mount_context;
};
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ libcrun_checkpoint_restore_s libcrun_checkpoint_restore_t;

/* Public checkpoint/restore options.  Mirror of the definition in
   <libcrun.h>; keep the two in sync.  */
ಎಣಿಕೆ libcrun_cr_cgroups_ಬಹುಲಕ
{
  LIBCRUN_CR_CG_ಬಹುಲಕ_DEFAULT = 0, /* soft, matching runc */
  LIBCRUN_CR_CG_ಬಹುಲಕ_SOFT,
  LIBCRUN_CR_CG_ಬಹುಲಕ_IGNORE,
  LIBCRUN_CR_CG_ಬಹುಲಕ_ತುಂಬಿದ,
  LIBCRUN_CR_CG_ಬಹುಲಕ_STRICT,
};

ಎಣಿಕೆ libcrun_cr_ಜಾಲ_ಬೀಗ_ವಿಧಾನ
{
  LIBCRUN_CR_ಜಾಲ_ಬೀಗ_DEFAULT = 0, /* iptables */
  LIBCRUN_CR_ಜಾಲ_ಬೀಗ_IPTABLES,
  LIBCRUN_CR_ಜಾಲ_ಬೀಗ_NFTABLES,
  LIBCRUN_CR_ಜಾಲ_ಬೀಗ_SKIP,
};

ರಚನೆ libcrun_checkpoint_restore_options_s
{
  size_t struct_ಗಾತ್ರ;

  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *image_ಮಾರ್ಗ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *work_ಮಾರ್ಗ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೋಷಕ_ಮಾರ್ಗ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *console_socket;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *lsm_profile;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *lsm_mount_context;

  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool ext_unix_sk;
  bool shell_ಕೆಲಸ;
  bool file_locks;
  bool pre_dump;
  bool detach;

  ಪೂರ್ಣಾಂಕ manage_cgroups_ಬಹುಲಕ; /* enum libcrun_cr_cgroups_mode */
  ಪೂರ್ಣಾಂಕ ಜಾಲ_ಬೀಗ_ವಿಧಾನ; /* enum libcrun_cr_network_lock_method */
};

LIBCRUN_ಬಹಿರಂಗ libcrun_container_t *libcrun_container_ತುಂಬು_from_file (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ libcrun_container_t *libcrun_container_ತುಂಬು_from_ಸ್ಮರಣೆ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *json, libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಶೂನ್ಯ libcrun_container_free (libcrun_container_t *);

/* Return the configuration of CONTAINER as a JSON string owned by CONTAINER.
   The document is generated on demand and cached, so the first call mutates
   CONTAINER and must not race with other users of the same handle.  */
LIBCRUN_ಬಹಿರಂಗ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err);
LIBCRUN_ಬಹಿರಂಗ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libcrun_container_get_annotation (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ);
LIBCRUN_ಬಹಿರಂಗ size_t libcrun_container_get_annotations_len (libcrun_container_t *container);
LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_get_annotation_at (libcrun_container_t *container, size_t ಸೂಚ್ಯಂಕ,
                                                        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಮೌಲ್ಯ);
LIBCRUN_ಬಹಿರಂಗ uid_t libcrun_container_get_uid (libcrun_container_t *container);
LIBCRUN_ಬಹಿರಂಗ gid_t libcrun_container_get_gid (libcrun_container_t *container);

LIBCRUN_ಬಹಿರಂಗ libcrun_context_t *libcrun_context_new (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, libcrun_error_t *err);
LIBCRUN_ಬಹಿರಂಗ ಶೂನ್ಯ libcrun_context_free (libcrun_context_t *ctx);

LIBCRUN_ಬಹಿರಂಗ ಶೂನ್ಯ libcrun_context_ಗಣ_id (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ);
LIBCRUN_ಬಹಿರಂಗ ಶೂನ್ಯ libcrun_context_ಗಣ_state_ಬೇರು (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ);
LIBCRUN_ಬಹಿರಂಗ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libcrun_context_get_state_ಬೇರು (libcrun_context_t *ctx);
LIBCRUN_ಬಹಿರಂಗ ಶೂನ್ಯ libcrun_context_ಗಣ_bundle (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ);
LIBCRUN_ಬಹಿರಂಗ ಶೂನ್ಯ libcrun_context_ಗಣ_console_socket (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ);
LIBCRUN_ಬಹಿರಂಗ ಶೂನ್ಯ libcrun_context_ಗಣ_pid_file (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ);
LIBCRUN_ಬಹಿರಂಗ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libcrun_context_get_pid_file (libcrun_context_t *ctx);
LIBCRUN_ಬಹಿರಂಗ ಶೂನ್ಯ libcrun_context_ಗಣ_notify_socket (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ);
LIBCRUN_ಬಹಿರಂಗ ಶೂನ್ಯ libcrun_context_ಗಣ_handler (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ);
LIBCRUN_ಬಹಿರಂಗ ಶೂನ್ಯ libcrun_context_ಗಣ_preserve_fds (libcrun_context_t *ctx, ಪೂರ್ಣಾಂಕ preserve_fds);
LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_context_get_preserve_fds (libcrun_context_t *ctx);
LIBCRUN_ಬಹಿರಂಗ ಶೂನ್ಯ libcrun_context_ಗಣ_listen_fds (libcrun_context_t *ctx, ಪೂರ್ಣಾಂಕ listen_fds);
LIBCRUN_ಬಹಿರಂಗ ಶೂನ್ಯ libcrun_context_ಗಣ_ವಾದಗಳು (libcrun_context_t *ctx, ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv);
LIBCRUN_ಬಹಿರಂಗ ಶೂನ್ಯ libcrun_context_ಗಣ_systemd_cgroup (libcrun_context_t *ctx, bool ಮೌಲ್ಯ);
LIBCRUN_ಬಹಿರಂಗ ಶೂನ್ಯ libcrun_context_ಗಣ_detach (libcrun_context_t *ctx, bool ಮೌಲ್ಯ);
LIBCRUN_ಬಹಿರಂಗ ಶೂನ್ಯ libcrun_context_ಗಣ_no_new_keyring (libcrun_context_t *ctx, bool ಮೌಲ್ಯ);
LIBCRUN_ಬಹಿರಂಗ ಶೂನ್ಯ libcrun_context_ಗಣ_no_ಅಚ್ಚು (libcrun_context_t *ctx, bool ಮೌಲ್ಯ);
LIBCRUN_ಬಹಿರಂಗ ಶೂನ್ಯ libcrun_context_ಗಣ_force_no_cgroup (libcrun_context_t *ctx, bool ಮೌಲ್ಯ);
LIBCRUN_ಬಹಿರಂಗ ಶೂನ್ಯ libcrun_context_ಗಣ_output_handler (libcrun_context_t *ctx, crun_output_handler handler, ಶೂನ್ಯ *arg);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_context_init_logging (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಲಘುಗಣಕ, libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_ಸತತ (libcrun_context_t *context, libcrun_container_t *container,
                                          ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ options, libcrun_error_t *error);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_delete (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, bool force,
                                             libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_kill (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂಕೇತ,
                                           libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_killall (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂಕೇತ,
                                              libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container,
                                             ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ options, libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_ಆರಂಭ (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_state (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, FILE *ಹೊರಹೋಗು,
                                            libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_state_json (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಅಕ್ಷರ **ಹೊರಹೋಗು,
                                                 libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_container_notify_handler (ರಚನೆ container_entrypoint_s *ವಾದಗಳು,
                                      ಎಣಿಕೆ handler_configure_phase phase,
                                      libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs,
                                      libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_get_container_state_string (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2,
                                                       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **container_ಸ್ಥಿತಿ,
                                                       ಪೂರ್ಣಾಂಕ *running, libcrun_error_t *err);

ರಚನೆ libcrun_container_exec_options_s
{
  size_t struct_ಗಾತ್ರ;
  runtime_spec_schema_config_schema_process *process;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cgroup;
  bool ವಿಲೀನ_env;
};

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_ತುಂಬು_process_from_file (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ,
                                                   runtime_spec_schema_config_schema_process **process,
                                                   libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_exec_with_options (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id,
                                                        ರಚನೆ libcrun_container_exec_options_s *opts,
                                                        libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_exec (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id,
                                           runtime_spec_schema_config_schema_process *process, libcrun_error_t *err);

ರಚನೆ libcrun_exec_options_s
{
  size_t struct_ಗಾತ್ರ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *process_json; /* OCI Process JSON */
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cgroup;
  bool ವಿಲೀನ_env;
};

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_exec_json (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *process_json,
                                                libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_exec_with_options_json (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id,
                                                             ಸ್ಥಿರಾಂಕ ರಚನೆ libcrun_exec_options_s *opts,
                                                             libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_exec_process_file (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ,
                                                        libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_update (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *content,
                                             size_t len, libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_update_from_file (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file,
                                                       libcrun_error_t *err);

ರಚನೆ libcrun_update_ಮೌಲ್ಯ_s
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *section;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
  bool numeric;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ;
};

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_update_from_ಮೌಲ್ಯಗಳು (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id,
                                                         ರಚನೆ libcrun_update_ಮೌಲ್ಯ_s *ಮೌಲ್ಯಗಳು, size_t len,
                                                         libcrun_error_t *err);

ರಚನೆ libcrun_intel_rdt_update
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *l3_cache_schema;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *mem_bw_schema;
  ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *schemata;
};

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_update_intel_rdt (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id,
                                                       ರಚನೆ libcrun_intel_rdt_update *update, libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_get_features (libcrun_context_t *context, ರಚನೆ features_info_s **info,
                                                   libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_get_features_json (libcrun_context_t *context, ಅಕ್ಷರ **ಹೊರಹೋಗು, libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_pause (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_unpause (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_checkpoint (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id,
                                                 ರಚನೆ libcrun_checkpoint_restore_options_s *cr_options,
                                                 libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_restore (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id,
                                              ರಚನೆ libcrun_checkpoint_restore_options_s *cr_options,
                                              libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_read_pids (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, bool recurse, pid_t **pids, libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_write_json_containers_list (libcrun_context_t *context, FILE *ಹೊರಹೋಗು, libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_list_json (libcrun_context_t *context, ಅಕ್ಷರ **ಹೊರಹೋಗು, libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_ಕೂಡಿಸು_mounts_from_file (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file,
                                                           libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_remove_mounts_from_file (libcrun_context_t *context, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file,
                                                              libcrun_error_t *err);

// Not part of the public API, just a method in container.c we need to access from linux.c
ಶೂನ್ಯ get_ಬೇರು_ಒಳ_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid,
                             uid_t *uid, gid_t *gid);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cleanup_containerp (libcrun_container_t **c)
{
  libcrun_container_t *container = *c;
  libcrun_container_free (container);
}

#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_container __attribute__ ((cleanup (cleanup_containerp)))

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cleanup_struct_features_free (ರಚನೆ features_info_s **info)
{
  size_t i;
  ರಚನೆ features_info_s *ptr;
  ಆದರೆ (info == ಖಾಲಿ || *info == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;

  ptr = *info;

  // Free oci_version_min if it is not NULL
  ಆದರೆ (ptr->oci_ಆವೃತ್ತಿ_min != ಖಾಲಿ)
    {
      ಬಿಡುಗಡೆ (ptr->oci_ಆವೃತ್ತಿ_min);
      ptr->oci_ಆವೃತ್ತಿ_min = ಖಾಲಿ; // Set to NULL after freeing
    }

  // Free oci_version_max if it is not NULL
  ಆದರೆ (ptr->oci_ಆವೃತ್ತಿ_max != ಖಾಲಿ)
    {
      ಬಿಡುಗಡೆ (ptr->oci_ಆವೃತ್ತಿ_max);
      ptr->oci_ಆವೃತ್ತಿ_max = ಖಾಲಿ; // Set to NULL after freeing
    }
  ಆದರೆ (ptr->hooks != ಖಾಲಿ)
    {
      ಗಾಗಿ (i = 0; ptr->hooks[i] != ಖಾಲಿ; i++)
        ಬಿಡುಗಡೆ (ptr->hooks[i]);
      ಬಿಡುಗಡೆ (ptr->hooks);
    }
  ಆದರೆ (ptr->mount_options != ಖಾಲಿ)
    {
      ಗಾಗಿ (i = 0; ptr->mount_options[i] != ಖಾಲಿ; i++)
        ಬಿಡುಗಡೆ (ptr->mount_options[i]);
      ಬಿಡುಗಡೆ (ptr->mount_options);
    }
  ಆದರೆ (ptr->linux.namespaces != ಖಾಲಿ)
    {
      ಗಾಗಿ (i = 0; ptr->linux.namespaces[i] != ಖಾಲಿ; i++)
        ಬಿಡುಗಡೆ (ptr->linux.namespaces[i]);
      ಬಿಡುಗಡೆ (ptr->linux.namespaces);
    }

  ಆದರೆ (ptr->linux.capabilities != ಖಾಲಿ)
    {
      ಗಾಗಿ (size_t i = 0; ptr->linux.capabilities[i] != ಖಾಲಿ; i++)
        ಬಿಡುಗಡೆ (ptr->linux.capabilities[i]);
      ಬಿಡುಗಡೆ (ptr->linux.capabilities);
    }
  ಆದರೆ (ptr->linux.seccomp.actions != ಖಾಲಿ)
    {
      ಗಾಗಿ (i = 0; ptr->linux.seccomp.actions[i] != ಖಾಲಿ; i++)
        ಬಿಡುಗಡೆ (ptr->linux.seccomp.actions[i]);
      ಬಿಡುಗಡೆ (ptr->linux.seccomp.actions);
    }
  ಆದರೆ (ptr->linux.seccomp.operators != ಖಾಲಿ)
    {
      ಗಾಗಿ (i = 0; ptr->linux.seccomp.operators[i] != ಖಾಲಿ; i++)
        ಬಿಡುಗಡೆ (ptr->linux.seccomp.operators[i]);
      ಬಿಡುಗಡೆ (ptr->linux.seccomp.operators);
    }

  ಬಿಡುಗಡೆ (ptr);
  *info = ಖಾಲಿ;
}

#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_struct_features __attribute__ ((cleanup (cleanup_struct_features_free)))

#ಆದರೆ_ಅಂತ್ಯ
