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

#include <config.h>
#include <ocispec/runtime_spec_schema_config_schema.h>
#include "error.h"
#include "string_map.h"

ga_nanaa handler_configure_phase
{
  HANDLER_CONFIGURE_BEFORE_MOUNTS = 1,
  HANDLER_CONFIGURE_AFTER_MOUNTS,
  HANDLER_CONFIGURE_MOUNTS,
  HANDLER_CONFIGURE_BEFORE_USERNS
};

sa_nracanaa custom_handler_manager_s;

sa_nracanaa libcrun_context_s
{
  const var_na *state_root;
  const var_na *id;
  const var_na *bundle;
  const var_na *console_socket;
  const var_na *pid_file;
  const var_na *notify_socket;
  const var_na *handler;
  var_na *owned_state_root;
  var_na *owned_id;
  var_na *owned_bundle;
  var_na *owned_console_socket;
  var_na *owned_pid_file;
  var_na *owned_notify_socket;
  var_na *owned_handler;
  จำนวนเต็ม preserve_fds;
  // For some use-cases we need differentiation between preserve_fds and listen_fds.
  // Following context variable makes sure we get exact value of listen_fds irrespective of preserve_fds.
  จำนวนเต็ม listen_fds;

  crun_output_handler output_handler;
  void *output_handler_arg;

  จำนวนเต็ม fifo_exec_wait_fd;

  bool systemd_cgroup;
  bool detach;
  bool no_new_keyring;
  bool force_no_cgroup;
  bool no_pivot;
  bool no_open_tree_namespace;

  var_na **argv;
  จำนวนเต็ม argc;

  sa_nracanaa custom_handler_manager_s *handler_manager;
};

ga_nanaa
{
  LIBCRUN_RUN_OPTIONS_PREFORK = 1 << 0,
  LIBCRUN_RUN_OPTIONS_KEEP = 1 << 1,
};

ga_nanaa
{
  LIBCRUN_CREATE_OPTIONS_PREFORK = 1 << 0,
};

sa_nracanaa libcrun_container_s
{
  /* Container parsed from the runtime json file.  */
  runtime_spec_schema_config_schema *container_def;

  uid_t host_uid;
  gid_t host_gid;

  /* Whether crun has CAP_SYS_PTRACE in its user namespace.  */
  bool host_has_cap_sys_ptrace;

  uid_t container_uid;
  gid_t container_gid;

  var_na *config_file;
  var_na *config_file_content;

  string_map *annotations;

  จำนวนเต็ม proc_fd;

  void *private_data;
  void (*cleanup_private_data) (void *private_data);
  sa_nracanaa libcrun_context_s *context;
};

sa_nracanaa libcrun_container_status_s;
typedef sa_nracanaa libcrun_container_status_s libcrun_container_status_t;

typedef sa_nracanaa libcrun_container_s libcrun_container_t;
typedef sa_nracanaa libcrun_context_s libcrun_context_t;

sa_nracanaa container_entrypoint_s;
sa_nracanaa cgroup_info_s
{
  bool v1;
  bool v2;
  bool systemd;
  bool systemd_user;
};

sa_nracanaa seccomp_info_s
{
  bool enabled;
  var_na **actions;
  var_na **operators;
  var_na **archs;
};

sa_nracanaa apparmor_info_s
{
  bool enabled;
};

sa_nracanaa selinux_info_s
{
  bool enabled;
};

sa_nracanaa idmap_info_s
{
  bool enabled;
};

sa_nracanaa intel_rdt_s
{
  bool enabled;
};

sa_nracanaa net_devices_s
{
  bool enabled;
};

sa_nracanaa mount_ext_info_s
{
  sa_nracanaa idmap_info_s idmap;
};

sa_nracanaa memory_policy_info_s
{
  var_na **mode;
  var_na **flags;
};

sa_nracanaa linux_info_s
{
  var_na **namespaces;
  var_na **capabilities;
  sa_nracanaa cgroup_info_s cgroup;
  sa_nracanaa seccomp_info_s seccomp;
  sa_nracanaa apparmor_info_s apparmor;
  sa_nracanaa selinux_info_s selinux;
  sa_nracanaa mount_ext_info_s mount_ext;
  sa_nracanaa intel_rdt_s intel_rdt;
  sa_nracanaa net_devices_s net_devices;
  sa_nracanaa memory_policy_info_s memory_policy;
};

sa_nracanaa annotations_info_s
{
  var_na *io_github_seccomp_libseccomp_version;
  bool run_oci_crun_checkpoint_enabled;
  var_na *run_oci_crun_commit;
  var_na *run_oci_crun_version;
  bool run_oci_crun_wasm;
};

sa_nracanaa features_info_s
{
  var_na *oci_version_min;
  var_na *oci_version_max;
  var_na **hooks;
  var_na **mount_options;
  sa_nracanaa linux_info_s linux;
  sa_nracanaa annotations_info_s annotations;
  var_na **potentially_unsafe_annotations;
};

sa_nracanaa libcrun_checkpoint_restore_s
{
  var_na *image_path;
  var_na *work_path;
  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool shell_job;
  bool ext_unix_sk;
  bool detach;
  bool file_locks;
  const var_na *console_socket;
  var_na *parent_path;
  bool pre_dump;
  จำนวนเต็ม manage_cgroups_mode;
  จำนวนเต็ม network_lock_method;
  var_na *lsm_profile;
  var_na *lsm_mount_context;
};
typedef sa_nracanaa libcrun_checkpoint_restore_s libcrun_checkpoint_restore_t;

/* Public checkpoint/restore options.  Mirror of the definition in
   <libcrun.h>; keep the two in sync.  */
ga_nanaa libcrun_cr_cgroups_mode
{
  LIBCRUN_CR_CG_MODE_DEFAULT = 0, /* soft, matching runc */
  LIBCRUN_CR_CG_MODE_SOFT,
  LIBCRUN_CR_CG_MODE_IGNORE,
  LIBCRUN_CR_CG_MODE_FULL,
  LIBCRUN_CR_CG_MODE_STRICT,
};

ga_nanaa libcrun_cr_network_lock_method
{
  LIBCRUN_CR_NETWORK_LOCK_DEFAULT = 0, /* iptables */
  LIBCRUN_CR_NETWORK_LOCK_IPTABLES,
  LIBCRUN_CR_NETWORK_LOCK_NFTABLES,
  LIBCRUN_CR_NETWORK_LOCK_SKIP,
};

sa_nracanaa libcrun_checkpoint_restore_options_s
{
  size_t struct_size;

  const var_na *image_path;
  const var_na *work_path;
  const var_na *parent_path;
  const var_na *console_socket;
  const var_na *lsm_profile;
  const var_na *lsm_mount_context;

  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool ext_unix_sk;
  bool shell_job;
  bool file_locks;
  bool pre_dump;
  bool detach;

  จำนวนเต็ม manage_cgroups_mode; /* enum libcrun_cr_cgroups_mode */
  จำนวนเต็ม network_lock_method; /* enum libcrun_cr_network_lock_method */
};

LIBCRUN_PUBLIC libcrun_container_t *libcrun_container_load_from_file (const var_na *path, libcrun_error_t *err);

LIBCRUN_PUBLIC libcrun_container_t *libcrun_container_load_from_memory (const var_na *json, libcrun_error_t *err);

LIBCRUN_PUBLIC void libcrun_container_free (libcrun_container_t *);

/* Return the configuration of CONTAINER as a JSON string owned by CONTAINER.
   The document is generated on demand and cached, so the first call mutates
   CONTAINER and must not race with other users of the same handle.  */
LIBCRUN_PUBLIC const var_na *libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err);
LIBCRUN_PUBLIC const var_na *libcrun_container_get_annotation (libcrun_container_t *container, const var_na *key);
LIBCRUN_PUBLIC size_t libcrun_container_get_annotations_len (libcrun_container_t *container);
LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_get_annotation_at (libcrun_container_t *container, size_t index,
                                                        const var_na **key, const var_na **value);
LIBCRUN_PUBLIC uid_t libcrun_container_get_uid (libcrun_container_t *container);
LIBCRUN_PUBLIC gid_t libcrun_container_get_gid (libcrun_container_t *container);

LIBCRUN_PUBLIC libcrun_context_t *libcrun_context_new (const var_na *id, const var_na *state_root, libcrun_error_t *err);
LIBCRUN_PUBLIC void libcrun_context_free (libcrun_context_t *ctx);

LIBCRUN_PUBLIC void libcrun_context_set_id (libcrun_context_t *ctx, const var_na *value);
LIBCRUN_PUBLIC void libcrun_context_set_state_root (libcrun_context_t *ctx, const var_na *value);
LIBCRUN_PUBLIC const var_na *libcrun_context_get_state_root (libcrun_context_t *ctx);
LIBCRUN_PUBLIC void libcrun_context_set_bundle (libcrun_context_t *ctx, const var_na *value);
LIBCRUN_PUBLIC void libcrun_context_set_console_socket (libcrun_context_t *ctx, const var_na *value);
LIBCRUN_PUBLIC void libcrun_context_set_pid_file (libcrun_context_t *ctx, const var_na *value);
LIBCRUN_PUBLIC const var_na *libcrun_context_get_pid_file (libcrun_context_t *ctx);
LIBCRUN_PUBLIC void libcrun_context_set_notify_socket (libcrun_context_t *ctx, const var_na *value);
LIBCRUN_PUBLIC void libcrun_context_set_handler (libcrun_context_t *ctx, const var_na *value);
LIBCRUN_PUBLIC void libcrun_context_set_preserve_fds (libcrun_context_t *ctx, จำนวนเต็ม preserve_fds);
LIBCRUN_PUBLIC จำนวนเต็ม libcrun_context_get_preserve_fds (libcrun_context_t *ctx);
LIBCRUN_PUBLIC void libcrun_context_set_listen_fds (libcrun_context_t *ctx, จำนวนเต็ม listen_fds);
LIBCRUN_PUBLIC void libcrun_context_set_args (libcrun_context_t *ctx, จำนวนเต็ม argc, var_na **argv);
LIBCRUN_PUBLIC void libcrun_context_set_systemd_cgroup (libcrun_context_t *ctx, bool value);
LIBCRUN_PUBLIC void libcrun_context_set_detach (libcrun_context_t *ctx, bool value);
LIBCRUN_PUBLIC void libcrun_context_set_no_new_keyring (libcrun_context_t *ctx, bool value);
LIBCRUN_PUBLIC void libcrun_context_set_no_pivot (libcrun_context_t *ctx, bool value);
LIBCRUN_PUBLIC void libcrun_context_set_force_no_cgroup (libcrun_context_t *ctx, bool value);
LIBCRUN_PUBLIC void libcrun_context_set_output_handler (libcrun_context_t *ctx, crun_output_handler handler, void *arg);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_context_init_logging (libcrun_context_t *ctx, const var_na *log, libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_run (libcrun_context_t *context, libcrun_container_t *container,
                                          unsigned จำนวนเต็ม options, libcrun_error_t *error);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_delete (libcrun_context_t *context, const var_na *id, bool force,
                                             libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_kill (libcrun_context_t *context, const var_na *id, const var_na *sa_nkeeta,
                                           libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_killall (libcrun_context_t *context, const var_na *id, const var_na *sa_nkeeta,
                                              libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container,
                                             unsigned จำนวนเต็ม options, libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_start (libcrun_context_t *context, const var_na *id, libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_state (libcrun_context_t *context, const var_na *id, FILE *out,
                                            libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_state_json (libcrun_context_t *context, const var_na *id, var_na **out,
                                                 libcrun_error_t *err);

จำนวนเต็ม libcrun_container_notify_handler (sa_nracanaa container_entrypoint_s *args,
                                      ga_nanaa handler_configure_phase phase,
                                      libcrun_container_t *container, const var_na *rootfs,
                                      libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_get_container_state_string (const var_na *id, libcrun_container_status_t *status,
                                                       const var_na *state_root, const var_na **container_status,
                                                       จำนวนเต็ม *running, libcrun_error_t *err);

sa_nracanaa libcrun_container_exec_options_s
{
  size_t struct_size;
  runtime_spec_schema_config_schema_process *process;
  const var_na *path;
  const var_na *cgroup;
  bool merge_env;
};

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_load_process_from_file (const var_na *path,
                                                   runtime_spec_schema_config_schema_process **process,
                                                   libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_exec_with_options (libcrun_context_t *context, const var_na *id,
                                                        sa_nracanaa libcrun_container_exec_options_s *opts,
                                                        libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_exec (libcrun_context_t *context, const var_na *id,
                                           runtime_spec_schema_config_schema_process *process, libcrun_error_t *err);

sa_nracanaa libcrun_exec_options_s
{
  size_t struct_size;
  const var_na *process_json; /* OCI Process JSON */
  const var_na *cgroup;
  bool merge_env;
};

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_exec_json (libcrun_context_t *context, const var_na *id, const var_na *process_json,
                                                libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_exec_with_options_json (libcrun_context_t *context, const var_na *id,
                                                             const sa_nracanaa libcrun_exec_options_s *opts,
                                                             libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_exec_process_file (libcrun_context_t *context, const var_na *id, const var_na *path,
                                                        libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_update (libcrun_context_t *context, const var_na *id, const var_na *content,
                                             size_t len, libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_update_from_file (libcrun_context_t *context, const var_na *id, const var_na *file,
                                                       libcrun_error_t *err);

sa_nracanaa libcrun_update_value_s
{
  const var_na *section;
  const var_na *name;
  bool numeric;
  const var_na *value;
};

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_update_from_values (libcrun_context_t *context, const var_na *id,
                                                         sa_nracanaa libcrun_update_value_s *values, size_t len,
                                                         libcrun_error_t *err);

sa_nracanaa libcrun_intel_rdt_update
{
  const var_na *l3_cache_schema;
  const var_na *mem_bw_schema;
  var_na *const *schemata;
};

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_update_intel_rdt (libcrun_context_t *context, const var_na *id,
                                                       sa_nracanaa libcrun_intel_rdt_update *update, libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_get_features (libcrun_context_t *context, sa_nracanaa features_info_s **info,
                                                   libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_get_features_json (libcrun_context_t *context, var_na **out, libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_pause (libcrun_context_t *context, const var_na *id, libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_unpause (libcrun_context_t *context, const var_na *id, libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_checkpoint (libcrun_context_t *context, const var_na *id,
                                                 sa_nracanaa libcrun_checkpoint_restore_options_s *cr_options,
                                                 libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_restore (libcrun_context_t *context, const var_na *id,
                                              sa_nracanaa libcrun_checkpoint_restore_options_s *cr_options,
                                              libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_read_pids (libcrun_context_t *context, const var_na *id, bool recurse, pid_t **pids, libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_write_json_containers_list (libcrun_context_t *context, FILE *out, libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_list_json (libcrun_context_t *context, var_na **out, libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_add_mounts_from_file (libcrun_context_t *context, const var_na *id, const var_na *file,
                                                           libcrun_error_t *err);

LIBCRUN_PUBLIC จำนวนเต็ม libcrun_container_remove_mounts_from_file (libcrun_context_t *context, const var_na *id, const var_na *file,
                                                              libcrun_error_t *err);

// Not part of the public API, just a method in container.c we need to access from linux.c
void get_root_in_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid,
                             uid_t *uid, gid_t *gid);

static anta_hstha void
cleanup_containerp (libcrun_container_t **c)
{
  libcrun_container_t *container = *c;
  libcrun_container_free (container);
}

#define cleanup_container __attribute__ ((cleanup (cleanup_containerp)))

static anta_hstha void
cleanup_struct_features_free (sa_nracanaa features_info_s **info)
{
  size_t i;
  sa_nracanaa features_info_s *ptr;
  ถ้า (info == NULL || *info == NULL)
    คืน;

  ptr = *info;

  // Free oci_version_min if it is not NULL
  ถ้า (ptr->oci_version_min != NULL)
    {
      free (ptr->oci_version_min);
      ptr->oci_version_min = NULL; // Set to NULL after freeing
    }

  // Free oci_version_max if it is not NULL
  ถ้า (ptr->oci_version_max != NULL)
    {
      free (ptr->oci_version_max);
      ptr->oci_version_max = NULL; // Set to NULL after freeing
    }
  ถ้า (ptr->hooks != NULL)
    {
      สำหรับ (i = 0; ptr->hooks[i] != NULL; i++)
        free (ptr->hooks[i]);
      free (ptr->hooks);
    }
  ถ้า (ptr->mount_options != NULL)
    {
      สำหรับ (i = 0; ptr->mount_options[i] != NULL; i++)
        free (ptr->mount_options[i]);
      free (ptr->mount_options);
    }
  ถ้า (ptr->linux.namespaces != NULL)
    {
      สำหรับ (i = 0; ptr->linux.namespaces[i] != NULL; i++)
        free (ptr->linux.namespaces[i]);
      free (ptr->linux.namespaces);
    }

  ถ้า (ptr->linux.capabilities != NULL)
    {
      สำหรับ (size_t i = 0; ptr->linux.capabilities[i] != NULL; i++)
        free (ptr->linux.capabilities[i]);
      free (ptr->linux.capabilities);
    }
  ถ้า (ptr->linux.seccomp.actions != NULL)
    {
      สำหรับ (i = 0; ptr->linux.seccomp.actions[i] != NULL; i++)
        free (ptr->linux.seccomp.actions[i]);
      free (ptr->linux.seccomp.actions);
    }
  ถ้า (ptr->linux.seccomp.operators != NULL)
    {
      สำหรับ (i = 0; ptr->linux.seccomp.operators[i] != NULL; i++)
        free (ptr->linux.seccomp.operators[i]);
      free (ptr->linux.seccomp.operators);
    }

  free (ptr);
  *info = NULL;
}

#define cleanup_struct_features __attribute__ ((cleanup (cleanup_struct_features_free)))

#endif
