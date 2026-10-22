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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <ocispec/runtime_spec_schema_config_schema.h>
#અન્તર્ભાવ "error.h"
#અન્તર્ભાવ "string_map.h"

ગણના handler_configure_phase
{
  HANDLER_CONFIGURE_BEFORE_MOUNTS = 1,
  HANDLER_CONFIGURE_AFTER_MOUNTS,
  HANDLER_CONFIGURE_MOUNTS,
  HANDLER_CONFIGURE_BEFORE_USERNS
};

સંરચના custom_handler_manager_s;

સંરચના libcrun_context_s
{
  અચલ અક્ષર *state_મૂળ;
  અચલ અક્ષર *id;
  અચલ અક્ષર *bundle;
  અચલ અક્ષર *console_socket;
  અચલ અક્ષર *pid_file;
  અચલ અક્ષર *notify_socket;
  અચલ અક્ષર *handler;
  અક્ષર *owned_state_મૂળ;
  અક્ષર *owned_id;
  અક્ષર *owned_bundle;
  અક્ષર *owned_console_socket;
  અક્ષર *owned_pid_file;
  અક્ષર *owned_notify_socket;
  અક્ષર *owned_handler;
  પૂર્ણાંક preserve_fds;
  // For some use-cases we need differentiation between preserve_fds and listen_fds.
  // Following context variable makes sure we get exact value of listen_fds irrespective of preserve_fds.
  પૂર્ણાંક listen_fds;

  crun_output_handler output_handler;
  શૂન્ય *output_handler_arg;

  પૂર્ણાંક fifo_exec_wait_fd;

  bool systemd_cgroup;
  bool detach;
  bool no_new_keyring;
  bool force_no_cgroup;
  bool no_pivot;
  bool no_open_વૃક્ષ_namespace;

  અક્ષર **argv;
  પૂર્ણાંક argc;

  સંરચના custom_handler_manager_s *handler_manager;
};

ગણના
{
  LIBCRUN_RUN_OPTIONS_PREFORK = 1 << 0,
  LIBCRUN_RUN_OPTIONS_KEEP = 1 << 1,
};

ગણના
{
  LIBCRUN_CREATE_OPTIONS_PREFORK = 1 << 0,
};

સંરચના libcrun_container_s
{
  /* Container parsed from the runtime json file.  */
  runtime_spec_schema_config_schema *container_def;

  uid_t host_uid;
  gid_t host_gid;

  /* Whether crun has CAP_SYS_PTRACE in its user namespace.  */
  bool host_has_cap_sys_ptrace;

  uid_t container_uid;
  gid_t container_gid;

  અક્ષર *config_file;
  અક્ષર *config_file_content;

  string_map *annotations;

  પૂર્ણાંક proc_fd;

  શૂન્ય *private_data;
  શૂન્ય (*cleanup_private_data) (શૂન્ય *private_data);
  સંરચના libcrun_context_s *context;
};

સંરચના libcrun_container_status_s;
પ્રકારવ્યાખ્યા સંરચના libcrun_container_status_s libcrun_container_status_t;

પ્રકારવ્યાખ્યા સંરચના libcrun_container_s libcrun_container_t;
પ્રકારવ્યાખ્યા સંરચના libcrun_context_s libcrun_context_t;

સંરચના container_entrypoint_s;
સંરચના cgroup_info_s
{
  bool v1;
  bool v2;
  bool systemd;
  bool systemd_user;
};

સંરચના seccomp_info_s
{
  bool enabled;
  અક્ષર **actions;
  અક્ષર **operators;
  અક્ષર **archs;
};

સંરચના apparmor_info_s
{
  bool enabled;
};

સંરચના selinux_info_s
{
  bool enabled;
};

સંરચના idmap_info_s
{
  bool enabled;
};

સંરચના intel_rdt_s
{
  bool enabled;
};

સંરચના net_devices_s
{
  bool enabled;
};

સંરચના mount_ext_info_s
{
  સંરચના idmap_info_s idmap;
};

સંરચના memory_policy_info_s
{
  અક્ષર **mode;
  અક્ષર **flags;
};

સંરચના linux_info_s
{
  અક્ષર **namespaces;
  અક્ષર **capabilities;
  સંરચના cgroup_info_s cgroup;
  સંરચના seccomp_info_s seccomp;
  સંરચના apparmor_info_s apparmor;
  સંરચના selinux_info_s selinux;
  સંરચના mount_ext_info_s mount_ext;
  સંરચના intel_rdt_s intel_rdt;
  સંરચના net_devices_s net_devices;
  સંરચના memory_policy_info_s memory_policy;
};

સંરચના annotations_info_s
{
  અક્ષર *io_github_seccomp_libseccomp_version;
  bool run_oci_crun_checkpoint_enabled;
  અક્ષર *run_oci_crun_commit;
  અક્ષર *run_oci_crun_version;
  bool run_oci_crun_wasm;
};

સંરચના features_info_s
{
  અક્ષર *oci_version_min;
  અક્ષર *oci_version_max;
  અક્ષર **hooks;
  અક્ષર **mount_options;
  સંરચના linux_info_s linux;
  સંરચના annotations_info_s annotations;
  અક્ષર **potentially_unsafe_annotations;
};

સંરચના libcrun_checkpoint_restore_s
{
  અક્ષર *image_path;
  અક્ષર *work_path;
  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool shell_job;
  bool ext_unix_sk;
  bool detach;
  bool file_locks;
  અચલ અક્ષર *console_socket;
  અક્ષર *parent_path;
  bool pre_dump;
  પૂર્ણાંક manage_cgroups_mode;
  પૂર્ણાંક network_lock_method;
  અક્ષર *lsm_profile;
  અક્ષર *lsm_mount_context;
};
પ્રકારવ્યાખ્યા સંરચના libcrun_checkpoint_restore_s libcrun_checkpoint_restore_t;

/* Public checkpoint/restore options.  Mirror of the definition in
   <libcrun.h>; keep the two in sync.  */
ગણના libcrun_cr_cgroups_mode
{
  LIBCRUN_CR_CG_MODE_DEFAULT = 0, /* soft, matching runc */
  LIBCRUN_CR_CG_MODE_SOFT,
  LIBCRUN_CR_CG_MODE_IGNORE,
  LIBCRUN_CR_CG_MODE_FULL,
  LIBCRUN_CR_CG_MODE_STRICT,
};

ગણના libcrun_cr_network_lock_method
{
  LIBCRUN_CR_NETWORK_LOCK_DEFAULT = 0, /* iptables */
  LIBCRUN_CR_NETWORK_LOCK_IPTABLES,
  LIBCRUN_CR_NETWORK_LOCK_NFTABLES,
  LIBCRUN_CR_NETWORK_LOCK_SKIP,
};

સંરચના libcrun_checkpoint_restore_options_s
{
  size_t struct_size;

  અચલ અક્ષર *image_path;
  અચલ અક્ષર *work_path;
  અચલ અક્ષર *parent_path;
  અચલ અક્ષર *console_socket;
  અચલ અક્ષર *lsm_profile;
  અચલ અક્ષર *lsm_mount_context;

  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool ext_unix_sk;
  bool shell_job;
  bool file_locks;
  bool pre_dump;
  bool detach;

  પૂર્ણાંક manage_cgroups_mode; /* enum libcrun_cr_cgroups_mode */
  પૂર્ણાંક network_lock_method; /* enum libcrun_cr_network_lock_method */
};

LIBCRUN_PUBLIC libcrun_container_t *libcrun_container_load_from_file (અચલ અક્ષર *path, libcrun_error_t *err);

LIBCRUN_PUBLIC libcrun_container_t *libcrun_container_load_from_memory (અચલ અક્ષર *json, libcrun_error_t *err);

LIBCRUN_PUBLIC શૂન્ય libcrun_container_free (libcrun_container_t *);

/* Return the configuration of CONTAINER as a JSON string owned by CONTAINER.
   The document is generated on demand and cached, so the first call mutates
   CONTAINER and must not race with other users of the same handle.  */
LIBCRUN_PUBLIC અચલ અક્ષર *libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err);
LIBCRUN_PUBLIC અચલ અક્ષર *libcrun_container_get_annotation (libcrun_container_t *container, અચલ અક્ષર *ચાવી);
LIBCRUN_PUBLIC size_t libcrun_container_get_annotations_len (libcrun_container_t *container);
LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_get_annotation_at (libcrun_container_t *container, size_t index,
                                                        અચલ અક્ષર **ચાવી, અચલ અક્ષર **કિંમત);
LIBCRUN_PUBLIC uid_t libcrun_container_get_uid (libcrun_container_t *container);
LIBCRUN_PUBLIC gid_t libcrun_container_get_gid (libcrun_container_t *container);

LIBCRUN_PUBLIC libcrun_context_t *libcrun_context_new (અચલ અક્ષર *id, અચલ અક્ષર *state_મૂળ, libcrun_error_t *err);
LIBCRUN_PUBLIC શૂન્ય libcrun_context_free (libcrun_context_t *ctx);

LIBCRUN_PUBLIC શૂન્ય libcrun_context_set_id (libcrun_context_t *ctx, અચલ અક્ષર *કિંમત);
LIBCRUN_PUBLIC શૂન્ય libcrun_context_set_state_મૂળ (libcrun_context_t *ctx, અચલ અક્ષર *કિંમત);
LIBCRUN_PUBLIC અચલ અક્ષર *libcrun_context_get_state_મૂળ (libcrun_context_t *ctx);
LIBCRUN_PUBLIC શૂન્ય libcrun_context_set_bundle (libcrun_context_t *ctx, અચલ અક્ષર *કિંમત);
LIBCRUN_PUBLIC શૂન્ય libcrun_context_set_console_socket (libcrun_context_t *ctx, અચલ અક્ષર *કિંમત);
LIBCRUN_PUBLIC શૂન્ય libcrun_context_set_pid_file (libcrun_context_t *ctx, અચલ અક્ષર *કિંમત);
LIBCRUN_PUBLIC અચલ અક્ષર *libcrun_context_get_pid_file (libcrun_context_t *ctx);
LIBCRUN_PUBLIC શૂન્ય libcrun_context_set_notify_socket (libcrun_context_t *ctx, અચલ અક્ષર *કિંમત);
LIBCRUN_PUBLIC શૂન્ય libcrun_context_set_handler (libcrun_context_t *ctx, અચલ અક્ષર *કિંમત);
LIBCRUN_PUBLIC શૂન્ય libcrun_context_set_preserve_fds (libcrun_context_t *ctx, પૂર્ણાંક preserve_fds);
LIBCRUN_PUBLIC પૂર્ણાંક libcrun_context_get_preserve_fds (libcrun_context_t *ctx);
LIBCRUN_PUBLIC શૂન્ય libcrun_context_set_listen_fds (libcrun_context_t *ctx, પૂર્ણાંક listen_fds);
LIBCRUN_PUBLIC શૂન્ય libcrun_context_set_args (libcrun_context_t *ctx, પૂર્ણાંક argc, અક્ષર **argv);
LIBCRUN_PUBLIC શૂન્ય libcrun_context_set_systemd_cgroup (libcrun_context_t *ctx, bool કિંમત);
LIBCRUN_PUBLIC શૂન્ય libcrun_context_set_detach (libcrun_context_t *ctx, bool કિંમત);
LIBCRUN_PUBLIC શૂન્ય libcrun_context_set_no_new_keyring (libcrun_context_t *ctx, bool કિંમત);
LIBCRUN_PUBLIC શૂન્ય libcrun_context_set_no_pivot (libcrun_context_t *ctx, bool કિંમત);
LIBCRUN_PUBLIC શૂન્ય libcrun_context_set_force_no_cgroup (libcrun_context_t *ctx, bool કિંમત);
LIBCRUN_PUBLIC શૂન્ય libcrun_context_set_output_handler (libcrun_context_t *ctx, crun_output_handler handler, શૂન્ય *arg);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_context_init_logging (libcrun_context_t *ctx, અચલ અક્ષર *log, libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_run (libcrun_context_t *context, libcrun_container_t *container,
                                          unsigned પૂર્ણાંક options, libcrun_error_t *error);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_delete (libcrun_context_t *context, અચલ અક્ષર *id, bool force,
                                             libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_kill (libcrun_context_t *context, અચલ અક્ષર *id, અચલ અક્ષર *સંકેત,
                                           libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_killall (libcrun_context_t *context, અચલ અક્ષર *id, અચલ અક્ષર *સંકેત,
                                              libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container,
                                             unsigned પૂર્ણાંક options, libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_start (libcrun_context_t *context, અચલ અક્ષર *id, libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_state (libcrun_context_t *context, અચલ અક્ષર *id, FILE *out,
                                            libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_state_json (libcrun_context_t *context, અચલ અક્ષર *id, અક્ષર **out,
                                                 libcrun_error_t *err);

પૂર્ણાંક libcrun_container_notify_handler (સંરચના container_entrypoint_s *args,
                                      ગણના handler_configure_phase phase,
                                      libcrun_container_t *container, અચલ અક્ષર *rootfs,
                                      libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_get_container_state_string (અચલ અક્ષર *id, libcrun_container_status_t *status,
                                                       અચલ અક્ષર *state_મૂળ, અચલ અક્ષર **container_status,
                                                       પૂર્ણાંક *running, libcrun_error_t *err);

સંરચના libcrun_container_exec_options_s
{
  size_t struct_size;
  runtime_spec_schema_config_schema_process *process;
  અચલ અક્ષર *path;
  અચલ અક્ષર *cgroup;
  bool merge_env;
};

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_load_process_from_file (અચલ અક્ષર *path,
                                                   runtime_spec_schema_config_schema_process **process,
                                                   libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_exec_with_options (libcrun_context_t *context, અચલ અક્ષર *id,
                                                        સંરચના libcrun_container_exec_options_s *opts,
                                                        libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_exec (libcrun_context_t *context, અચલ અક્ષર *id,
                                           runtime_spec_schema_config_schema_process *process, libcrun_error_t *err);

સંરચના libcrun_exec_options_s
{
  size_t struct_size;
  અચલ અક્ષર *process_json; /* OCI Process JSON */
  અચલ અક્ષર *cgroup;
  bool merge_env;
};

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_exec_json (libcrun_context_t *context, અચલ અક્ષર *id, અચલ અક્ષર *process_json,
                                                libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_exec_with_options_json (libcrun_context_t *context, અચલ અક્ષર *id,
                                                             અચલ સંરચના libcrun_exec_options_s *opts,
                                                             libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_exec_process_file (libcrun_context_t *context, અચલ અક્ષર *id, અચલ અક્ષર *path,
                                                        libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_update (libcrun_context_t *context, અચલ અક્ષર *id, અચલ અક્ષર *content,
                                             size_t len, libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_update_from_file (libcrun_context_t *context, અચલ અક્ષર *id, અચલ અક્ષર *file,
                                                       libcrun_error_t *err);

સંરચના libcrun_update_કિંમત_s
{
  અચલ અક્ષર *section;
  અચલ અક્ષર *name;
  bool numeric;
  અચલ અક્ષર *કિંમત;
};

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_update_from_કિંમતો (libcrun_context_t *context, અચલ અક્ષર *id,
                                                         સંરચના libcrun_update_કિંમત_s *કિંમતો, size_t len,
                                                         libcrun_error_t *err);

સંરચના libcrun_intel_rdt_update
{
  અચલ અક્ષર *l3_cache_schema;
  અચલ અક્ષર *mem_bw_schema;
  અક્ષર *અચલ *schemata;
};

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_update_intel_rdt (libcrun_context_t *context, અચલ અક્ષર *id,
                                                       સંરચના libcrun_intel_rdt_update *update, libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_get_features (libcrun_context_t *context, સંરચના features_info_s **info,
                                                   libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_get_features_json (libcrun_context_t *context, અક્ષર **out, libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_pause (libcrun_context_t *context, અચલ અક્ષર *id, libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_unpause (libcrun_context_t *context, અચલ અક્ષર *id, libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_checkpoint (libcrun_context_t *context, અચલ અક્ષર *id,
                                                 સંરચના libcrun_checkpoint_restore_options_s *cr_options,
                                                 libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_restore (libcrun_context_t *context, અચલ અક્ષર *id,
                                              સંરચના libcrun_checkpoint_restore_options_s *cr_options,
                                              libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_read_pids (libcrun_context_t *context, અચલ અક્ષર *id, bool recurse, pid_t **pids, libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_write_json_containers_list (libcrun_context_t *context, FILE *out, libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_list_json (libcrun_context_t *context, અક્ષર **out, libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_add_mounts_from_file (libcrun_context_t *context, અચલ અક્ષર *id, અચલ અક્ષર *file,
                                                           libcrun_error_t *err);

LIBCRUN_PUBLIC પૂર્ણાંક libcrun_container_remove_mounts_from_file (libcrun_context_t *context, અચલ અક્ષર *id, અચલ અક્ષર *file,
                                                              libcrun_error_t *err);

// Not part of the public API, just a method in container.c we need to access from linux.c
શૂન્ય get_મૂળ_in_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid,
                             uid_t *uid, gid_t *gid);

સ્થિર અંતઃરેખ શૂન્ય
cleanup_containerp (libcrun_container_t **c)
{
  libcrun_container_t *container = *c;
  libcrun_container_free (container);
}

#define cleanup_container __attribute__ ((cleanup (cleanup_containerp)))

સ્થિર અંતઃરેખ શૂન્ય
cleanup_struct_features_free (સંરચના features_info_s **info)
{
  size_t i;
  સંરચના features_info_s *ptr;
  જો (info == ખાલી || *info == ખાલી)
    પાછા;

  ptr = *info;

  // Free oci_version_min if it is not NULL
  જો (ptr->oci_version_min != ખાલી)
    {
      મુક્ત (ptr->oci_version_min);
      ptr->oci_version_min = ખાલી; // Set to NULL after freeing
    }

  // Free oci_version_max if it is not NULL
  જો (ptr->oci_version_max != ખાલી)
    {
      મુક્ત (ptr->oci_version_max);
      ptr->oci_version_max = ખાલી; // Set to NULL after freeing
    }
  જો (ptr->hooks != ખાલી)
    {
      માટે (i = 0; ptr->hooks[i] != ખાલી; i++)
        મુક્ત (ptr->hooks[i]);
      મુક્ત (ptr->hooks);
    }
  જો (ptr->mount_options != ખાલી)
    {
      માટે (i = 0; ptr->mount_options[i] != ખાલી; i++)
        મુક્ત (ptr->mount_options[i]);
      મુક્ત (ptr->mount_options);
    }
  જો (ptr->linux.namespaces != ખાલી)
    {
      માટે (i = 0; ptr->linux.namespaces[i] != ખાલી; i++)
        મુક્ત (ptr->linux.namespaces[i]);
      મુક્ત (ptr->linux.namespaces);
    }

  જો (ptr->linux.capabilities != ખાલી)
    {
      માટે (size_t i = 0; ptr->linux.capabilities[i] != ખાલી; i++)
        મુક્ત (ptr->linux.capabilities[i]);
      મુક્ત (ptr->linux.capabilities);
    }
  જો (ptr->linux.seccomp.actions != ખાલી)
    {
      માટે (i = 0; ptr->linux.seccomp.actions[i] != ખાલી; i++)
        મુક્ત (ptr->linux.seccomp.actions[i]);
      મુક્ત (ptr->linux.seccomp.actions);
    }
  જો (ptr->linux.seccomp.operators != ખાલી)
    {
      માટે (i = 0; ptr->linux.seccomp.operators[i] != ખાલી; i++)
        મુક્ત (ptr->linux.seccomp.operators[i]);
      મુક્ત (ptr->linux.seccomp.operators);
    }

  મુક્ત (ptr);
  *info = ખાલી;
}

#define cleanup_struct_features __attribute__ ((cleanup (cleanup_struct_features_free)))

#endif
