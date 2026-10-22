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

#شامل <config.h>
#شامل <ocispec/runtime_spec_schema_config_schema.h>
#شامل "error.h"
#شامل "string_map.h"

شمارش handler_configure_phase
{
  HANDLER_CONFIGURE_BEFORE_MOUNTS = 1,
  HANDLER_CONFIGURE_AFTER_MOUNTS,
  HANDLER_CONFIGURE_MOUNTS,
  HANDLER_CONFIGURE_BEFORE_USERNS
};

ساختار custom_handler_manager_s;

ساختار libcrun_context_s
{
  ثابت نویسه *state_ریشه;
  ثابت نویسه *id;
  ثابت نویسه *bundle;
  ثابت نویسه *console_socket;
  ثابت نویسه *pid_file;
  ثابت نویسه *notify_socket;
  ثابت نویسه *handler;
  نویسه *owned_state_ریشه;
  نویسه *owned_id;
  نویسه *owned_bundle;
  نویسه *owned_console_socket;
  نویسه *owned_pid_file;
  نویسه *owned_notify_socket;
  نویسه *owned_handler;
  صحیح preserve_fds;
  // For some use-cases we need differentiation between preserve_fds and listen_fds.
  // Following context variable makes sure we get exact value of listen_fds irrespective of preserve_fds.
  صحیح listen_fds;

  crun_output_handler output_handler;
  تهی *output_handler_arg;

  صحیح fifo_exec_wait_fd;

  bool systemd_cgroup;
  bool detach;
  bool no_new_keyring;
  bool force_no_cgroup;
  bool no_pivot;
  bool no_open_درخت_namespace;

  نویسه **argv;
  صحیح argc;

  ساختار custom_handler_manager_s *handler_manager;
};

شمارش
{
  LIBCRUN_RUN_OPTIONS_PREFORK = 1 << 0,
  LIBCRUN_RUN_OPTIONS_KEEP = 1 << 1,
};

شمارش
{
  LIBCRUN_CREATE_OPTIONS_PREFORK = 1 << 0,
};

ساختار libcrun_container_s
{
  /* Container parsed from the runtime json file.  */
  runtime_spec_schema_config_schema *container_def;

  uid_t host_uid;
  gid_t host_gid;

  /* Whether crun has CAP_SYS_PTRACE in its user namespace.  */
  bool host_has_cap_sys_ptrace;

  uid_t container_uid;
  gid_t container_gid;

  نویسه *config_file;
  نویسه *config_file_content;

  string_map *annotations;

  صحیح proc_fd;

  تهی *private_data;
  تهی (*cleanup_private_data) (تهی *private_data);
  ساختار libcrun_context_s *context;
};

ساختار libcrun_container_status_s;
تعریفنوع ساختار libcrun_container_status_s libcrun_container_status_t;

تعریفنوع ساختار libcrun_container_s libcrun_container_t;
تعریفنوع ساختار libcrun_context_s libcrun_context_t;

ساختار container_entrypoint_s;
ساختار cgroup_info_s
{
  bool v1;
  bool v2;
  bool systemd;
  bool systemd_user;
};

ساختار seccomp_info_s
{
  bool enabled;
  نویسه **actions;
  نویسه **operators;
  نویسه **archs;
};

ساختار apparmor_info_s
{
  bool enabled;
};

ساختار selinux_info_s
{
  bool enabled;
};

ساختار idmap_info_s
{
  bool enabled;
};

ساختار intel_rdt_s
{
  bool enabled;
};

ساختار net_devices_s
{
  bool enabled;
};

ساختار mount_ext_info_s
{
  ساختار idmap_info_s idmap;
};

ساختار memory_policy_info_s
{
  نویسه **mode;
  نویسه **flags;
};

ساختار linux_info_s
{
  نویسه **namespaces;
  نویسه **capabilities;
  ساختار cgroup_info_s cgroup;
  ساختار seccomp_info_s seccomp;
  ساختار apparmor_info_s apparmor;
  ساختار selinux_info_s selinux;
  ساختار mount_ext_info_s mount_ext;
  ساختار intel_rdt_s intel_rdt;
  ساختار net_devices_s net_devices;
  ساختار memory_policy_info_s memory_policy;
};

ساختار annotations_info_s
{
  نویسه *io_github_seccomp_libseccomp_version;
  bool run_oci_crun_checkpoint_enabled;
  نویسه *run_oci_crun_commit;
  نویسه *run_oci_crun_version;
  bool run_oci_crun_wasm;
};

ساختار features_info_s
{
  نویسه *oci_version_min;
  نویسه *oci_version_max;
  نویسه **hooks;
  نویسه **mount_options;
  ساختار linux_info_s linux;
  ساختار annotations_info_s annotations;
  نویسه **potentially_unsafe_annotations;
};

ساختار libcrun_checkpoint_restore_s
{
  نویسه *image_path;
  نویسه *work_path;
  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool shell_job;
  bool ext_unix_sk;
  bool detach;
  bool file_locks;
  ثابت نویسه *console_socket;
  نویسه *parent_path;
  bool pre_dump;
  صحیح manage_cgroups_mode;
  صحیح network_lock_method;
  نویسه *lsm_profile;
  نویسه *lsm_mount_context;
};
تعریفنوع ساختار libcrun_checkpoint_restore_s libcrun_checkpoint_restore_t;

/* Public checkpoint/restore options.  Mirror of the definition in
   <libcrun.h>; keep the two in sync.  */
شمارش libcrun_cr_cgroups_mode
{
  LIBCRUN_CR_CG_MODE_DEFAULT = 0, /* soft, matching runc */
  LIBCRUN_CR_CG_MODE_SOFT,
  LIBCRUN_CR_CG_MODE_IGNORE,
  LIBCRUN_CR_CG_MODE_FULL,
  LIBCRUN_CR_CG_MODE_STRICT,
};

شمارش libcrun_cr_network_lock_method
{
  LIBCRUN_CR_NETWORK_LOCK_DEFAULT = 0, /* iptables */
  LIBCRUN_CR_NETWORK_LOCK_IPTABLES,
  LIBCRUN_CR_NETWORK_LOCK_NFTABLES,
  LIBCRUN_CR_NETWORK_LOCK_SKIP,
};

ساختار libcrun_checkpoint_restore_options_s
{
  size_t struct_size;

  ثابت نویسه *image_path;
  ثابت نویسه *work_path;
  ثابت نویسه *parent_path;
  ثابت نویسه *console_socket;
  ثابت نویسه *lsm_profile;
  ثابت نویسه *lsm_mount_context;

  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool ext_unix_sk;
  bool shell_job;
  bool file_locks;
  bool pre_dump;
  bool detach;

  صحیح manage_cgroups_mode; /* enum libcrun_cr_cgroups_mode */
  صحیح network_lock_method; /* enum libcrun_cr_network_lock_method */
};

LIBCRUN_PUBLIC libcrun_container_t *libcrun_container_load_from_file (ثابت نویسه *path, libcrun_error_t *err);

LIBCRUN_PUBLIC libcrun_container_t *libcrun_container_load_from_memory (ثابت نویسه *json, libcrun_error_t *err);

LIBCRUN_PUBLIC تهی libcrun_container_free (libcrun_container_t *);

/* Return the configuration of CONTAINER as a JSON string owned by CONTAINER.
   The document is generated on demand and cached, so the first call mutates
   CONTAINER and must not race with other users of the same handle.  */
LIBCRUN_PUBLIC ثابت نویسه *libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err);
LIBCRUN_PUBLIC ثابت نویسه *libcrun_container_get_annotation (libcrun_container_t *container, ثابت نویسه *کلید);
LIBCRUN_PUBLIC size_t libcrun_container_get_annotations_len (libcrun_container_t *container);
LIBCRUN_PUBLIC صحیح libcrun_container_get_annotation_at (libcrun_container_t *container, size_t index,
                                                        ثابت نویسه **کلید, ثابت نویسه **مقدار);
LIBCRUN_PUBLIC uid_t libcrun_container_get_uid (libcrun_container_t *container);
LIBCRUN_PUBLIC gid_t libcrun_container_get_gid (libcrun_container_t *container);

LIBCRUN_PUBLIC libcrun_context_t *libcrun_context_new (ثابت نویسه *id, ثابت نویسه *state_ریشه, libcrun_error_t *err);
LIBCRUN_PUBLIC تهی libcrun_context_free (libcrun_context_t *ctx);

LIBCRUN_PUBLIC تهی libcrun_context_set_id (libcrun_context_t *ctx, ثابت نویسه *مقدار);
LIBCRUN_PUBLIC تهی libcrun_context_set_state_ریشه (libcrun_context_t *ctx, ثابت نویسه *مقدار);
LIBCRUN_PUBLIC ثابت نویسه *libcrun_context_get_state_ریشه (libcrun_context_t *ctx);
LIBCRUN_PUBLIC تهی libcrun_context_set_bundle (libcrun_context_t *ctx, ثابت نویسه *مقدار);
LIBCRUN_PUBLIC تهی libcrun_context_set_console_socket (libcrun_context_t *ctx, ثابت نویسه *مقدار);
LIBCRUN_PUBLIC تهی libcrun_context_set_pid_file (libcrun_context_t *ctx, ثابت نویسه *مقدار);
LIBCRUN_PUBLIC ثابت نویسه *libcrun_context_get_pid_file (libcrun_context_t *ctx);
LIBCRUN_PUBLIC تهی libcrun_context_set_notify_socket (libcrun_context_t *ctx, ثابت نویسه *مقدار);
LIBCRUN_PUBLIC تهی libcrun_context_set_handler (libcrun_context_t *ctx, ثابت نویسه *مقدار);
LIBCRUN_PUBLIC تهی libcrun_context_set_preserve_fds (libcrun_context_t *ctx, صحیح preserve_fds);
LIBCRUN_PUBLIC صحیح libcrun_context_get_preserve_fds (libcrun_context_t *ctx);
LIBCRUN_PUBLIC تهی libcrun_context_set_listen_fds (libcrun_context_t *ctx, صحیح listen_fds);
LIBCRUN_PUBLIC تهی libcrun_context_set_args (libcrun_context_t *ctx, صحیح argc, نویسه **argv);
LIBCRUN_PUBLIC تهی libcrun_context_set_systemd_cgroup (libcrun_context_t *ctx, bool مقدار);
LIBCRUN_PUBLIC تهی libcrun_context_set_detach (libcrun_context_t *ctx, bool مقدار);
LIBCRUN_PUBLIC تهی libcrun_context_set_no_new_keyring (libcrun_context_t *ctx, bool مقدار);
LIBCRUN_PUBLIC تهی libcrun_context_set_no_pivot (libcrun_context_t *ctx, bool مقدار);
LIBCRUN_PUBLIC تهی libcrun_context_set_force_no_cgroup (libcrun_context_t *ctx, bool مقدار);
LIBCRUN_PUBLIC تهی libcrun_context_set_output_handler (libcrun_context_t *ctx, crun_output_handler handler, تهی *arg);

LIBCRUN_PUBLIC صحیح libcrun_context_init_logging (libcrun_context_t *ctx, ثابت نویسه *log, libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_container_run (libcrun_context_t *context, libcrun_container_t *container,
                                          unsigned صحیح options, libcrun_error_t *error);

LIBCRUN_PUBLIC صحیح libcrun_container_delete (libcrun_context_t *context, ثابت نویسه *id, bool force,
                                             libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_container_kill (libcrun_context_t *context, ثابت نویسه *id, ثابت نویسه *سیگنال,
                                           libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_container_killall (libcrun_context_t *context, ثابت نویسه *id, ثابت نویسه *سیگنال,
                                              libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container,
                                             unsigned صحیح options, libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_container_start (libcrun_context_t *context, ثابت نویسه *id, libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_container_state (libcrun_context_t *context, ثابت نویسه *id, FILE *out,
                                            libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_container_state_json (libcrun_context_t *context, ثابت نویسه *id, نویسه **out,
                                                 libcrun_error_t *err);

صحیح libcrun_container_notify_handler (ساختار container_entrypoint_s *args,
                                      شمارش handler_configure_phase phase,
                                      libcrun_container_t *container, ثابت نویسه *rootfs,
                                      libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_get_container_state_string (ثابت نویسه *id, libcrun_container_status_t *status,
                                                       ثابت نویسه *state_ریشه, ثابت نویسه **container_status,
                                                       صحیح *running, libcrun_error_t *err);

ساختار libcrun_container_exec_options_s
{
  size_t struct_size;
  runtime_spec_schema_config_schema_process *process;
  ثابت نویسه *path;
  ثابت نویسه *cgroup;
  bool merge_env;
};

LIBCRUN_PUBLIC صحیح libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_load_process_from_file (ثابت نویسه *path,
                                                   runtime_spec_schema_config_schema_process **process,
                                                   libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_container_exec_with_options (libcrun_context_t *context, ثابت نویسه *id,
                                                        ساختار libcrun_container_exec_options_s *opts,
                                                        libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_container_exec (libcrun_context_t *context, ثابت نویسه *id,
                                           runtime_spec_schema_config_schema_process *process, libcrun_error_t *err);

ساختار libcrun_exec_options_s
{
  size_t struct_size;
  ثابت نویسه *process_json; /* OCI Process JSON */
  ثابت نویسه *cgroup;
  bool merge_env;
};

LIBCRUN_PUBLIC صحیح libcrun_container_exec_json (libcrun_context_t *context, ثابت نویسه *id, ثابت نویسه *process_json,
                                                libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_container_exec_with_options_json (libcrun_context_t *context, ثابت نویسه *id,
                                                             ثابت ساختار libcrun_exec_options_s *opts,
                                                             libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_container_exec_process_file (libcrun_context_t *context, ثابت نویسه *id, ثابت نویسه *path,
                                                        libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_container_update (libcrun_context_t *context, ثابت نویسه *id, ثابت نویسه *content,
                                             size_t len, libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_container_update_from_file (libcrun_context_t *context, ثابت نویسه *id, ثابت نویسه *file,
                                                       libcrun_error_t *err);

ساختار libcrun_update_مقدار_s
{
  ثابت نویسه *section;
  ثابت نویسه *name;
  bool numeric;
  ثابت نویسه *مقدار;
};

LIBCRUN_PUBLIC صحیح libcrun_container_update_from_مقادیر (libcrun_context_t *context, ثابت نویسه *id,
                                                         ساختار libcrun_update_مقدار_s *مقادیر, size_t len,
                                                         libcrun_error_t *err);

ساختار libcrun_intel_rdt_update
{
  ثابت نویسه *l3_cache_schema;
  ثابت نویسه *mem_bw_schema;
  نویسه *ثابت *schemata;
};

LIBCRUN_PUBLIC صحیح libcrun_container_update_intel_rdt (libcrun_context_t *context, ثابت نویسه *id,
                                                       ساختار libcrun_intel_rdt_update *update, libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_container_get_features (libcrun_context_t *context, ساختار features_info_s **info,
                                                   libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_container_get_features_json (libcrun_context_t *context, نویسه **out, libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_container_pause (libcrun_context_t *context, ثابت نویسه *id, libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_container_unpause (libcrun_context_t *context, ثابت نویسه *id, libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_container_checkpoint (libcrun_context_t *context, ثابت نویسه *id,
                                                 ساختار libcrun_checkpoint_restore_options_s *cr_options,
                                                 libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_container_restore (libcrun_context_t *context, ثابت نویسه *id,
                                              ساختار libcrun_checkpoint_restore_options_s *cr_options,
                                              libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_container_read_pids (libcrun_context_t *context, ثابت نویسه *id, bool recurse, pid_t **pids, libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_write_json_containers_list (libcrun_context_t *context, FILE *out, libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_container_list_json (libcrun_context_t *context, نویسه **out, libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_container_add_mounts_from_file (libcrun_context_t *context, ثابت نویسه *id, ثابت نویسه *file,
                                                           libcrun_error_t *err);

LIBCRUN_PUBLIC صحیح libcrun_container_remove_mounts_from_file (libcrun_context_t *context, ثابت نویسه *id, ثابت نویسه *file,
                                                              libcrun_error_t *err);

// Not part of the public API, just a method in container.c we need to access from linux.c
تهی get_ریشه_in_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid,
                             uid_t *uid, gid_t *gid);

ایستا درونخطی تهی
cleanup_containerp (libcrun_container_t **c)
{
  libcrun_container_t *container = *c;
  libcrun_container_free (container);
}

#define cleanup_container __attribute__ ((cleanup (cleanup_containerp)))

ایستا درونخطی تهی
cleanup_struct_features_free (ساختار features_info_s **info)
{
  size_t i;
  ساختار features_info_s *ptr;
  اگر (info == پوچ || *info == پوچ)
    بازگشت;

  ptr = *info;

  // Free oci_version_min if it is not NULL
  اگر (ptr->oci_version_min != پوچ)
    {
      آزاد (ptr->oci_version_min);
      ptr->oci_version_min = پوچ; // Set to NULL after freeing
    }

  // Free oci_version_max if it is not NULL
  اگر (ptr->oci_version_max != پوچ)
    {
      آزاد (ptr->oci_version_max);
      ptr->oci_version_max = پوچ; // Set to NULL after freeing
    }
  اگر (ptr->hooks != پوچ)
    {
      برای (i = 0; ptr->hooks[i] != پوچ; i++)
        آزاد (ptr->hooks[i]);
      آزاد (ptr->hooks);
    }
  اگر (ptr->mount_options != پوچ)
    {
      برای (i = 0; ptr->mount_options[i] != پوچ; i++)
        آزاد (ptr->mount_options[i]);
      آزاد (ptr->mount_options);
    }
  اگر (ptr->linux.namespaces != پوچ)
    {
      برای (i = 0; ptr->linux.namespaces[i] != پوچ; i++)
        آزاد (ptr->linux.namespaces[i]);
      آزاد (ptr->linux.namespaces);
    }

  اگر (ptr->linux.capabilities != پوچ)
    {
      برای (size_t i = 0; ptr->linux.capabilities[i] != پوچ; i++)
        آزاد (ptr->linux.capabilities[i]);
      آزاد (ptr->linux.capabilities);
    }
  اگر (ptr->linux.seccomp.actions != پوچ)
    {
      برای (i = 0; ptr->linux.seccomp.actions[i] != پوچ; i++)
        آزاد (ptr->linux.seccomp.actions[i]);
      آزاد (ptr->linux.seccomp.actions);
    }
  اگر (ptr->linux.seccomp.operators != پوچ)
    {
      برای (i = 0; ptr->linux.seccomp.operators[i] != پوچ; i++)
        آزاد (ptr->linux.seccomp.operators[i]);
      آزاد (ptr->linux.seccomp.operators);
    }

  آزاد (ptr);
  *info = پوچ;
}

#define cleanup_struct_features __attribute__ ((cleanup (cleanup_struct_features_free)))

#endif
