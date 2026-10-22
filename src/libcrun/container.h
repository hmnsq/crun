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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <ocispec/runtime_spec_schema_config_schema.h>
#অন্তর্ভাব "error.h"
#অন্তর্ভাব "string_map.h"

গণনা handler_configure_phase
{
  HANDLER_CONFIGURE_BEFORE_MOUNTS = 1,
  HANDLER_CONFIGURE_AFTER_MOUNTS,
  HANDLER_CONFIGURE_MOUNTS,
  HANDLER_CONFIGURE_BEFORE_USERNS
};

সংরচনা custom_handler_manager_s;

সংরচনা libcrun_context_s
{
  ধ্রুব বর্ণ *state_মূল;
  ধ্রুব বর্ণ *id;
  ধ্রুব বর্ণ *bundle;
  ধ্রুব বর্ণ *console_socket;
  ধ্রুব বর্ণ *pid_file;
  ধ্রুব বর্ণ *notify_socket;
  ধ্রুব বর্ণ *handler;
  বর্ণ *owned_state_মূল;
  বর্ণ *owned_id;
  বর্ণ *owned_bundle;
  বর্ণ *owned_console_socket;
  বর্ণ *owned_pid_file;
  বর্ণ *owned_notify_socket;
  বর্ণ *owned_handler;
  পূর্ণাঙ্ক preserve_fds;
  // For some use-cases we need differentiation between preserve_fds and listen_fds.
  // Following context variable makes sure we get exact value of listen_fds irrespective of preserve_fds.
  পূর্ণাঙ্ক listen_fds;

  crun_output_handler output_handler;
  শূন্য *output_handler_arg;

  পূর্ণাঙ্ক fifo_exec_wait_fd;

  bool systemd_cgroup;
  bool detach;
  bool no_new_keyring;
  bool force_no_cgroup;
  bool no_pivot;
  bool no_open_বৃক্ষ_namespace;

  বর্ণ **argv;
  পূর্ণাঙ্ক argc;

  সংরচনা custom_handler_manager_s *handler_manager;
};

গণনা
{
  LIBCRUN_RUN_OPTIONS_PREFORK = 1 << 0,
  LIBCRUN_RUN_OPTIONS_KEEP = 1 << 1,
};

গণনা
{
  LIBCRUN_CREATE_OPTIONS_PREFORK = 1 << 0,
};

সংরচনা libcrun_container_s
{
  /* Container parsed from the runtime json file.  */
  runtime_spec_schema_config_schema *container_def;

  uid_t host_uid;
  gid_t host_gid;

  /* Whether crun has CAP_SYS_PTRACE in its user namespace.  */
  bool host_has_cap_sys_ptrace;

  uid_t container_uid;
  gid_t container_gid;

  বর্ণ *config_file;
  বর্ণ *config_file_content;

  string_map *annotations;

  পূর্ণাঙ্ক proc_fd;

  শূন্য *private_data;
  শূন্য (*cleanup_private_data) (শূন্য *private_data);
  সংরচনা libcrun_context_s *context;
};

সংরচনা libcrun_container_status_s;
প্রকারনির্দেশ সংরচনা libcrun_container_status_s libcrun_container_status_t;

প্রকারনির্দেশ সংরচনা libcrun_container_s libcrun_container_t;
প্রকারনির্দেশ সংরচনা libcrun_context_s libcrun_context_t;

সংরচনা container_entrypoint_s;
সংরচনা cgroup_info_s
{
  bool v1;
  bool v2;
  bool systemd;
  bool systemd_user;
};

সংরচনা seccomp_info_s
{
  bool enabled;
  বর্ণ **actions;
  বর্ণ **operators;
  বর্ণ **archs;
};

সংরচনা apparmor_info_s
{
  bool enabled;
};

সংরচনা selinux_info_s
{
  bool enabled;
};

সংরচনা idmap_info_s
{
  bool enabled;
};

সংরচনা intel_rdt_s
{
  bool enabled;
};

সংরচনা net_devices_s
{
  bool enabled;
};

সংরচনা mount_ext_info_s
{
  সংরচনা idmap_info_s idmap;
};

সংরচনা memory_policy_info_s
{
  বর্ণ **mode;
  বর্ণ **flags;
};

সংরচনা linux_info_s
{
  বর্ণ **namespaces;
  বর্ণ **capabilities;
  সংরচনা cgroup_info_s cgroup;
  সংরচনা seccomp_info_s seccomp;
  সংরচনা apparmor_info_s apparmor;
  সংরচনা selinux_info_s selinux;
  সংরচনা mount_ext_info_s mount_ext;
  সংরচনা intel_rdt_s intel_rdt;
  সংরচনা net_devices_s net_devices;
  সংরচনা memory_policy_info_s memory_policy;
};

সংরচনা annotations_info_s
{
  বর্ণ *io_github_seccomp_libseccomp_version;
  bool run_oci_crun_checkpoint_enabled;
  বর্ণ *run_oci_crun_commit;
  বর্ণ *run_oci_crun_version;
  bool run_oci_crun_wasm;
};

সংরচনা features_info_s
{
  বর্ণ *oci_version_min;
  বর্ণ *oci_version_max;
  বর্ণ **hooks;
  বর্ণ **mount_options;
  সংরচনা linux_info_s linux;
  সংরচনা annotations_info_s annotations;
  বর্ণ **potentially_unsafe_annotations;
};

সংরচনা libcrun_checkpoint_restore_s
{
  বর্ণ *image_path;
  বর্ণ *work_path;
  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool shell_job;
  bool ext_unix_sk;
  bool detach;
  bool file_locks;
  ধ্রুব বর্ণ *console_socket;
  বর্ণ *parent_path;
  bool pre_dump;
  পূর্ণাঙ্ক manage_cgroups_mode;
  পূর্ণাঙ্ক network_lock_method;
  বর্ণ *lsm_profile;
  বর্ণ *lsm_mount_context;
};
প্রকারনির্দেশ সংরচনা libcrun_checkpoint_restore_s libcrun_checkpoint_restore_t;

/* Public checkpoint/restore options.  Mirror of the definition in
   <libcrun.h>; keep the two in sync.  */
গণনা libcrun_cr_cgroups_mode
{
  LIBCRUN_CR_CG_MODE_DEFAULT = 0, /* soft, matching runc */
  LIBCRUN_CR_CG_MODE_SOFT,
  LIBCRUN_CR_CG_MODE_IGNORE,
  LIBCRUN_CR_CG_MODE_FULL,
  LIBCRUN_CR_CG_MODE_STRICT,
};

গণনা libcrun_cr_network_lock_method
{
  LIBCRUN_CR_NETWORK_LOCK_DEFAULT = 0, /* iptables */
  LIBCRUN_CR_NETWORK_LOCK_IPTABLES,
  LIBCRUN_CR_NETWORK_LOCK_NFTABLES,
  LIBCRUN_CR_NETWORK_LOCK_SKIP,
};

সংরচনা libcrun_checkpoint_restore_options_s
{
  size_t struct_size;

  ধ্রুব বর্ণ *image_path;
  ধ্রুব বর্ণ *work_path;
  ধ্রুব বর্ণ *parent_path;
  ধ্রুব বর্ণ *console_socket;
  ধ্রুব বর্ণ *lsm_profile;
  ধ্রুব বর্ণ *lsm_mount_context;

  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool ext_unix_sk;
  bool shell_job;
  bool file_locks;
  bool pre_dump;
  bool detach;

  পূর্ণাঙ্ক manage_cgroups_mode; /* enum libcrun_cr_cgroups_mode */
  পূর্ণাঙ্ক network_lock_method; /* enum libcrun_cr_network_lock_method */
};

LIBCRUN_PUBLIC libcrun_container_t *libcrun_container_load_from_file (ধ্রুব বর্ণ *path, libcrun_error_t *err);

LIBCRUN_PUBLIC libcrun_container_t *libcrun_container_load_from_memory (ধ্রুব বর্ণ *json, libcrun_error_t *err);

LIBCRUN_PUBLIC শূন্য libcrun_container_free (libcrun_container_t *);

/* Return the configuration of CONTAINER as a JSON string owned by CONTAINER.
   The document is generated on demand and cached, so the first call mutates
   CONTAINER and must not race with other users of the same handle.  */
LIBCRUN_PUBLIC ধ্রুব বর্ণ *libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err);
LIBCRUN_PUBLIC ধ্রুব বর্ণ *libcrun_container_get_annotation (libcrun_container_t *container, ধ্রুব বর্ণ *চাবি);
LIBCRUN_PUBLIC size_t libcrun_container_get_annotations_len (libcrun_container_t *container);
LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_get_annotation_at (libcrun_container_t *container, size_t index,
                                                        ধ্রুব বর্ণ **চাবি, ধ্রুব বর্ণ **মান);
LIBCRUN_PUBLIC uid_t libcrun_container_get_uid (libcrun_container_t *container);
LIBCRUN_PUBLIC gid_t libcrun_container_get_gid (libcrun_container_t *container);

LIBCRUN_PUBLIC libcrun_context_t *libcrun_context_new (ধ্রুব বর্ণ *id, ধ্রুব বর্ণ *state_মূল, libcrun_error_t *err);
LIBCRUN_PUBLIC শূন্য libcrun_context_free (libcrun_context_t *ctx);

LIBCRUN_PUBLIC শূন্য libcrun_context_set_id (libcrun_context_t *ctx, ধ্রুব বর্ণ *মান);
LIBCRUN_PUBLIC শূন্য libcrun_context_set_state_মূল (libcrun_context_t *ctx, ধ্রুব বর্ণ *মান);
LIBCRUN_PUBLIC ধ্রুব বর্ণ *libcrun_context_get_state_মূল (libcrun_context_t *ctx);
LIBCRUN_PUBLIC শূন্য libcrun_context_set_bundle (libcrun_context_t *ctx, ধ্রুব বর্ণ *মান);
LIBCRUN_PUBLIC শূন্য libcrun_context_set_console_socket (libcrun_context_t *ctx, ধ্রুব বর্ণ *মান);
LIBCRUN_PUBLIC শূন্য libcrun_context_set_pid_file (libcrun_context_t *ctx, ধ্রুব বর্ণ *মান);
LIBCRUN_PUBLIC ধ্রুব বর্ণ *libcrun_context_get_pid_file (libcrun_context_t *ctx);
LIBCRUN_PUBLIC শূন্য libcrun_context_set_notify_socket (libcrun_context_t *ctx, ধ্রুব বর্ণ *মান);
LIBCRUN_PUBLIC শূন্য libcrun_context_set_handler (libcrun_context_t *ctx, ধ্রুব বর্ণ *মান);
LIBCRUN_PUBLIC শূন্য libcrun_context_set_preserve_fds (libcrun_context_t *ctx, পূর্ণাঙ্ক preserve_fds);
LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_context_get_preserve_fds (libcrun_context_t *ctx);
LIBCRUN_PUBLIC শূন্য libcrun_context_set_listen_fds (libcrun_context_t *ctx, পূর্ণাঙ্ক listen_fds);
LIBCRUN_PUBLIC শূন্য libcrun_context_set_args (libcrun_context_t *ctx, পূর্ণাঙ্ক argc, বর্ণ **argv);
LIBCRUN_PUBLIC শূন্য libcrun_context_set_systemd_cgroup (libcrun_context_t *ctx, bool মান);
LIBCRUN_PUBLIC শূন্য libcrun_context_set_detach (libcrun_context_t *ctx, bool মান);
LIBCRUN_PUBLIC শূন্য libcrun_context_set_no_new_keyring (libcrun_context_t *ctx, bool মান);
LIBCRUN_PUBLIC শূন্য libcrun_context_set_no_pivot (libcrun_context_t *ctx, bool মান);
LIBCRUN_PUBLIC শূন্য libcrun_context_set_force_no_cgroup (libcrun_context_t *ctx, bool মান);
LIBCRUN_PUBLIC শূন্য libcrun_context_set_output_handler (libcrun_context_t *ctx, crun_output_handler handler, শূন্য *arg);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_context_init_logging (libcrun_context_t *ctx, ধ্রুব বর্ণ *log, libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_run (libcrun_context_t *context, libcrun_container_t *container,
                                          unsigned পূর্ণাঙ্ক options, libcrun_error_t *error);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_delete (libcrun_context_t *context, ধ্রুব বর্ণ *id, bool force,
                                             libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_kill (libcrun_context_t *context, ধ্রুব বর্ণ *id, ধ্রুব বর্ণ *সঙ্কেত,
                                           libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_killall (libcrun_context_t *context, ধ্রুব বর্ণ *id, ধ্রুব বর্ণ *সঙ্কেত,
                                              libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container,
                                             unsigned পূর্ণাঙ্ক options, libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_start (libcrun_context_t *context, ধ্রুব বর্ণ *id, libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_state (libcrun_context_t *context, ধ্রুব বর্ণ *id, FILE *out,
                                            libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_state_json (libcrun_context_t *context, ধ্রুব বর্ণ *id, বর্ণ **out,
                                                 libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_container_notify_handler (সংরচনা container_entrypoint_s *args,
                                      গণনা handler_configure_phase phase,
                                      libcrun_container_t *container, ধ্রুব বর্ণ *rootfs,
                                      libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_get_container_state_string (ধ্রুব বর্ণ *id, libcrun_container_status_t *status,
                                                       ধ্রুব বর্ণ *state_মূল, ধ্রুব বর্ণ **container_status,
                                                       পূর্ণাঙ্ক *running, libcrun_error_t *err);

সংরচনা libcrun_container_exec_options_s
{
  size_t struct_size;
  runtime_spec_schema_config_schema_process *process;
  ধ্রুব বর্ণ *path;
  ধ্রুব বর্ণ *cgroup;
  bool merge_env;
};

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_load_process_from_file (ধ্রুব বর্ণ *path,
                                                   runtime_spec_schema_config_schema_process **process,
                                                   libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_exec_with_options (libcrun_context_t *context, ধ্রুব বর্ণ *id,
                                                        সংরচনা libcrun_container_exec_options_s *opts,
                                                        libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_exec (libcrun_context_t *context, ধ্রুব বর্ণ *id,
                                           runtime_spec_schema_config_schema_process *process, libcrun_error_t *err);

সংরচনা libcrun_exec_options_s
{
  size_t struct_size;
  ধ্রুব বর্ণ *process_json; /* OCI Process JSON */
  ধ্রুব বর্ণ *cgroup;
  bool merge_env;
};

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_exec_json (libcrun_context_t *context, ধ্রুব বর্ণ *id, ধ্রুব বর্ণ *process_json,
                                                libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_exec_with_options_json (libcrun_context_t *context, ধ্রুব বর্ণ *id,
                                                             ধ্রুব সংরচনা libcrun_exec_options_s *opts,
                                                             libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_exec_process_file (libcrun_context_t *context, ধ্রুব বর্ণ *id, ধ্রুব বর্ণ *path,
                                                        libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_update (libcrun_context_t *context, ধ্রুব বর্ণ *id, ধ্রুব বর্ণ *content,
                                             size_t len, libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_update_from_file (libcrun_context_t *context, ধ্রুব বর্ণ *id, ধ্রুব বর্ণ *file,
                                                       libcrun_error_t *err);

সংরচনা libcrun_update_মান_s
{
  ধ্রুব বর্ণ *section;
  ধ্রুব বর্ণ *name;
  bool numeric;
  ধ্রুব বর্ণ *মান;
};

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_update_from_মানসমূহ (libcrun_context_t *context, ধ্রুব বর্ণ *id,
                                                         সংরচনা libcrun_update_মান_s *মানসমূহ, size_t len,
                                                         libcrun_error_t *err);

সংরচনা libcrun_intel_rdt_update
{
  ধ্রুব বর্ণ *l3_cache_schema;
  ধ্রুব বর্ণ *mem_bw_schema;
  বর্ণ *ধ্রুব *schemata;
};

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_update_intel_rdt (libcrun_context_t *context, ধ্রুব বর্ণ *id,
                                                       সংরচনা libcrun_intel_rdt_update *update, libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_get_features (libcrun_context_t *context, সংরচনা features_info_s **info,
                                                   libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_get_features_json (libcrun_context_t *context, বর্ণ **out, libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_pause (libcrun_context_t *context, ধ্রুব বর্ণ *id, libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_unpause (libcrun_context_t *context, ধ্রুব বর্ণ *id, libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_checkpoint (libcrun_context_t *context, ধ্রুব বর্ণ *id,
                                                 সংরচনা libcrun_checkpoint_restore_options_s *cr_options,
                                                 libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_restore (libcrun_context_t *context, ধ্রুব বর্ণ *id,
                                              সংরচনা libcrun_checkpoint_restore_options_s *cr_options,
                                              libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_read_pids (libcrun_context_t *context, ধ্রুব বর্ণ *id, bool recurse, pid_t **pids, libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_write_json_containers_list (libcrun_context_t *context, FILE *out, libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_list_json (libcrun_context_t *context, বর্ণ **out, libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_add_mounts_from_file (libcrun_context_t *context, ধ্রুব বর্ণ *id, ধ্রুব বর্ণ *file,
                                                           libcrun_error_t *err);

LIBCRUN_PUBLIC পূর্ণাঙ্ক libcrun_container_remove_mounts_from_file (libcrun_context_t *context, ধ্রুব বর্ণ *id, ধ্রুব বর্ণ *file,
                                                              libcrun_error_t *err);

// Not part of the public API, just a method in container.c we need to access from linux.c
শূন্য get_মূল_in_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid,
                             uid_t *uid, gid_t *gid);

স্থির অন্তঃস্থ শূন্য
cleanup_containerp (libcrun_container_t **c)
{
  libcrun_container_t *container = *c;
  libcrun_container_free (container);
}

#define cleanup_container __attribute__ ((cleanup (cleanup_containerp)))

স্থির অন্তঃস্থ শূন্য
cleanup_struct_features_free (সংরচনা features_info_s **info)
{
  size_t i;
  সংরচনা features_info_s *ptr;
  যদি (info == রিক্ত || *info == রিক্ত)
    প্রত্যাগম;

  ptr = *info;

  // Free oci_version_min if it is not NULL
  যদি (ptr->oci_version_min != রিক্ত)
    {
      মুক্ত (ptr->oci_version_min);
      ptr->oci_version_min = রিক্ত; // Set to NULL after freeing
    }

  // Free oci_version_max if it is not NULL
  যদি (ptr->oci_version_max != রিক্ত)
    {
      মুক্ত (ptr->oci_version_max);
      ptr->oci_version_max = রিক্ত; // Set to NULL after freeing
    }
  যদি (ptr->hooks != রিক্ত)
    {
      প্রতি (i = 0; ptr->hooks[i] != রিক্ত; i++)
        মুক্ত (ptr->hooks[i]);
      মুক্ত (ptr->hooks);
    }
  যদি (ptr->mount_options != রিক্ত)
    {
      প্রতি (i = 0; ptr->mount_options[i] != রিক্ত; i++)
        মুক্ত (ptr->mount_options[i]);
      মুক্ত (ptr->mount_options);
    }
  যদি (ptr->linux.namespaces != রিক্ত)
    {
      প্রতি (i = 0; ptr->linux.namespaces[i] != রিক্ত; i++)
        মুক্ত (ptr->linux.namespaces[i]);
      মুক্ত (ptr->linux.namespaces);
    }

  যদি (ptr->linux.capabilities != রিক্ত)
    {
      প্রতি (size_t i = 0; ptr->linux.capabilities[i] != রিক্ত; i++)
        মুক্ত (ptr->linux.capabilities[i]);
      মুক্ত (ptr->linux.capabilities);
    }
  যদি (ptr->linux.seccomp.actions != রিক্ত)
    {
      প্রতি (i = 0; ptr->linux.seccomp.actions[i] != রিক্ত; i++)
        মুক্ত (ptr->linux.seccomp.actions[i]);
      মুক্ত (ptr->linux.seccomp.actions);
    }
  যদি (ptr->linux.seccomp.operators != রিক্ত)
    {
      প্রতি (i = 0; ptr->linux.seccomp.operators[i] != রিক্ত; i++)
        মুক্ত (ptr->linux.seccomp.operators[i]);
      মুক্ত (ptr->linux.seccomp.operators);
    }

  মুক্ত (ptr);
  *info = রিক্ত;
}

#define cleanup_struct_features __attribute__ ((cleanup (cleanup_struct_features_free)))

#endif
