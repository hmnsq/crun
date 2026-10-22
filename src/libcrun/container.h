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

#अगर_अघोषित CONTAINER_H
#घोषणा CONTAINER_H

#समावेश <config.h>
#समावेश <ocispec/runtime_spec_schema_config_schema.h>
#समावेश "error.h"
#समावेश "string_map.h"

क्रमागत handler_configure_phase
{
  HANDLER_CONFIGURE_BEFORE_MOUNTS = 1,
  HANDLER_CONFIGURE_AFTER_MOUNTS,
  HANDLER_CONFIGURE_MOUNTS,
  HANDLER_CONFIGURE_BEFORE_USERNS
};

काष्ठा custom_handler_manager_s;

काष्ठा libcrun_context_s
{
  स्थिर अक्षर *state_मूल;
  स्थिर अक्षर *id;
  स्थिर अक्षर *bundle;
  स्थिर अक्षर *console_socket;
  स्थिर अक्षर *pid_file;
  स्थिर अक्षर *notify_socket;
  स्थिर अक्षर *handler;
  अक्षर *owned_state_मूल;
  अक्षर *owned_id;
  अक्षर *owned_bundle;
  अक्षर *owned_console_socket;
  अक्षर *owned_pid_file;
  अक्षर *owned_notify_socket;
  अक्षर *owned_handler;
  पूर्णांक preserve_fds;
  // For some use-cases we need differentiation between preserve_fds and listen_fds.
  // Following context variable makes sure we get exact value of listen_fds irrespective of preserve_fds.
  पूर्णांक listen_fds;

  crun_output_handler output_handler;
  व्योम *output_handler_arg;

  पूर्णांक fifo_exec_wait_fd;

  bool systemd_cgroup;
  bool detach;
  bool no_new_keyring;
  bool force_no_cgroup;
  bool no_pivot;
  bool no_open_वृक्ष_namespace;

  अक्षर **argv;
  पूर्णांक argc;

  काष्ठा custom_handler_manager_s *handler_manager;
};

क्रमागत
{
  LIBCRUN_RUN_OPTIONS_PREFORK = 1 << 0,
  LIBCRUN_RUN_OPTIONS_KEEP = 1 << 1,
};

क्रमागत
{
  LIBCRUN_CREATE_OPTIONS_PREFORK = 1 << 0,
};

काष्ठा libcrun_container_s
{
  /* Container parsed from the runtime json file.  */
  runtime_spec_schema_config_schema *container_def;

  uid_t host_uid;
  gid_t host_gid;

  /* Whether crun has CAP_SYS_PTRACE in its user namespace.  */
  bool host_has_cap_sys_ptrace;

  uid_t container_uid;
  gid_t container_gid;

  अक्षर *config_file;
  अक्षर *config_file_content;

  string_map *annotations;

  पूर्णांक proc_fd;

  व्योम *private_तथ्य;
  व्योम (*cleanup_private_तथ्य) (व्योम *private_तथ्य);
  काष्ठा libcrun_context_s *context;
};

काष्ठा libcrun_container_status_s;
प्रकार काष्ठा libcrun_container_status_s libcrun_container_status_t;

प्रकार काष्ठा libcrun_container_s libcrun_container_t;
प्रकार काष्ठा libcrun_context_s libcrun_context_t;

काष्ठा container_entrypoint_s;
काष्ठा cgroup_info_s
{
  bool v1;
  bool v2;
  bool systemd;
  bool systemd_user;
};

काष्ठा seccomp_info_s
{
  bool enabled;
  अक्षर **actions;
  अक्षर **operators;
  अक्षर **archs;
};

काष्ठा apparmor_info_s
{
  bool enabled;
};

काष्ठा selinux_info_s
{
  bool enabled;
};

काष्ठा idmap_info_s
{
  bool enabled;
};

काष्ठा intel_rdt_s
{
  bool enabled;
};

काष्ठा net_devices_s
{
  bool enabled;
};

काष्ठा mount_ext_info_s
{
  काष्ठा idmap_info_s idmap;
};

काष्ठा memory_policy_info_s
{
  अक्षर **mode;
  अक्षर **flags;
};

काष्ठा linux_info_s
{
  अक्षर **namespaces;
  अक्षर **capabilities;
  काष्ठा cgroup_info_s cgroup;
  काष्ठा seccomp_info_s seccomp;
  काष्ठा apparmor_info_s apparmor;
  काष्ठा selinux_info_s selinux;
  काष्ठा mount_ext_info_s mount_ext;
  काष्ठा intel_rdt_s intel_rdt;
  काष्ठा net_devices_s net_devices;
  काष्ठा memory_policy_info_s memory_policy;
};

काष्ठा annotations_info_s
{
  अक्षर *io_github_seccomp_libseccomp_version;
  bool run_oci_crun_checkpoint_enabled;
  अक्षर *run_oci_crun_commit;
  अक्षर *run_oci_crun_version;
  bool run_oci_crun_wasm;
};

काष्ठा features_info_s
{
  अक्षर *oci_version_min;
  अक्षर *oci_version_max;
  अक्षर **hooks;
  अक्षर **mount_options;
  काष्ठा linux_info_s linux;
  काष्ठा annotations_info_s annotations;
  अक्षर **potentially_unsafe_annotations;
};

काष्ठा libcrun_checkpoint_restore_s
{
  अक्षर *image_path;
  अक्षर *work_path;
  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool shell_job;
  bool ext_unix_sk;
  bool detach;
  bool file_locks;
  स्थिर अक्षर *console_socket;
  अक्षर *parent_path;
  bool pre_dump;
  पूर्णांक manage_cgroups_mode;
  पूर्णांक network_lock_method;
  अक्षर *lsm_profile;
  अक्षर *lsm_mount_context;
};
प्रकार काष्ठा libcrun_checkpoint_restore_s libcrun_checkpoint_restore_t;

/* Public checkpoint/restore options.  Mirror of the definition in
   <libcrun.h>; keep the two in sync.  */
क्रमागत libcrun_cr_cgroups_mode
{
  LIBCRUN_CR_CG_MODE_DEFAULT = 0, /* soft, matching runc */
  LIBCRUN_CR_CG_MODE_SOFT,
  LIBCRUN_CR_CG_MODE_IGNORE,
  LIBCRUN_CR_CG_MODE_FULL,
  LIBCRUN_CR_CG_MODE_STRICT,
};

क्रमागत libcrun_cr_network_lock_method
{
  LIBCRUN_CR_NETWORK_LOCK_DEFAULT = 0, /* iptables */
  LIBCRUN_CR_NETWORK_LOCK_IPTABLES,
  LIBCRUN_CR_NETWORK_LOCK_NFTABLES,
  LIBCRUN_CR_NETWORK_LOCK_SKIP,
};

काष्ठा libcrun_checkpoint_restore_options_s
{
  माप_प्रकार struct_आकार;

  स्थिर अक्षर *image_path;
  स्थिर अक्षर *work_path;
  स्थिर अक्षर *parent_path;
  स्थिर अक्षर *console_socket;
  स्थिर अक्षर *lsm_profile;
  स्थिर अक्षर *lsm_mount_context;

  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool ext_unix_sk;
  bool shell_job;
  bool file_locks;
  bool pre_dump;
  bool detach;

  पूर्णांक manage_cgroups_mode; /* enum libcrun_cr_cgroups_mode */
  पूर्णांक network_lock_method; /* enum libcrun_cr_network_lock_method */
};

LIBCRUN_PUBLIC libcrun_container_t *libcrun_container_load_from_file (स्थिर अक्षर *path, libcrun_error_t *err);

LIBCRUN_PUBLIC libcrun_container_t *libcrun_container_load_from_memory (स्थिर अक्षर *json, libcrun_error_t *err);

LIBCRUN_PUBLIC व्योम libcrun_container_free (libcrun_container_t *);

/* Return the configuration of CONTAINER as a JSON string owned by CONTAINER.
   The document is generated on demand and cached, so the first call mutates
   CONTAINER and must not race with other users of the same handle.  */
LIBCRUN_PUBLIC स्थिर अक्षर *libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err);
LIBCRUN_PUBLIC स्थिर अक्षर *libcrun_container_get_annotation (libcrun_container_t *container, स्थिर अक्षर *कुंजी);
LIBCRUN_PUBLIC माप_प्रकार libcrun_container_get_annotations_len (libcrun_container_t *container);
LIBCRUN_PUBLIC पूर्णांक libcrun_container_get_annotation_at (libcrun_container_t *container, माप_प्रकार सूचकांक,
                                                        स्थिर अक्षर **कुंजी, स्थिर अक्षर **मूल्य);
LIBCRUN_PUBLIC uid_t libcrun_container_get_uid (libcrun_container_t *container);
LIBCRUN_PUBLIC gid_t libcrun_container_get_gid (libcrun_container_t *container);

LIBCRUN_PUBLIC libcrun_context_t *libcrun_context_new (स्थिर अक्षर *id, स्थिर अक्षर *state_मूल, libcrun_error_t *err);
LIBCRUN_PUBLIC व्योम libcrun_context_free (libcrun_context_t *ctx);

LIBCRUN_PUBLIC व्योम libcrun_context_set_id (libcrun_context_t *ctx, स्थिर अक्षर *मूल्य);
LIBCRUN_PUBLIC व्योम libcrun_context_set_state_मूल (libcrun_context_t *ctx, स्थिर अक्षर *मूल्य);
LIBCRUN_PUBLIC स्थिर अक्षर *libcrun_context_get_state_मूल (libcrun_context_t *ctx);
LIBCRUN_PUBLIC व्योम libcrun_context_set_bundle (libcrun_context_t *ctx, स्थिर अक्षर *मूल्य);
LIBCRUN_PUBLIC व्योम libcrun_context_set_console_socket (libcrun_context_t *ctx, स्थिर अक्षर *मूल्य);
LIBCRUN_PUBLIC व्योम libcrun_context_set_pid_file (libcrun_context_t *ctx, स्थिर अक्षर *मूल्य);
LIBCRUN_PUBLIC स्थिर अक्षर *libcrun_context_get_pid_file (libcrun_context_t *ctx);
LIBCRUN_PUBLIC व्योम libcrun_context_set_notify_socket (libcrun_context_t *ctx, स्थिर अक्षर *मूल्य);
LIBCRUN_PUBLIC व्योम libcrun_context_set_handler (libcrun_context_t *ctx, स्थिर अक्षर *मूल्य);
LIBCRUN_PUBLIC व्योम libcrun_context_set_preserve_fds (libcrun_context_t *ctx, पूर्णांक preserve_fds);
LIBCRUN_PUBLIC पूर्णांक libcrun_context_get_preserve_fds (libcrun_context_t *ctx);
LIBCRUN_PUBLIC व्योम libcrun_context_set_listen_fds (libcrun_context_t *ctx, पूर्णांक listen_fds);
LIBCRUN_PUBLIC व्योम libcrun_context_set_args (libcrun_context_t *ctx, पूर्णांक argc, अक्षर **argv);
LIBCRUN_PUBLIC व्योम libcrun_context_set_systemd_cgroup (libcrun_context_t *ctx, bool मूल्य);
LIBCRUN_PUBLIC व्योम libcrun_context_set_detach (libcrun_context_t *ctx, bool मूल्य);
LIBCRUN_PUBLIC व्योम libcrun_context_set_no_new_keyring (libcrun_context_t *ctx, bool मूल्य);
LIBCRUN_PUBLIC व्योम libcrun_context_set_no_pivot (libcrun_context_t *ctx, bool मूल्य);
LIBCRUN_PUBLIC व्योम libcrun_context_set_force_no_cgroup (libcrun_context_t *ctx, bool मूल्य);
LIBCRUN_PUBLIC व्योम libcrun_context_set_output_handler (libcrun_context_t *ctx, crun_output_handler handler, व्योम *arg);

LIBCRUN_PUBLIC पूर्णांक libcrun_context_init_logging (libcrun_context_t *ctx, स्थिर अक्षर *log, libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_container_run (libcrun_context_t *context, libcrun_container_t *container,
                                          अचिन्हित पूर्णांक options, libcrun_error_t *error);

LIBCRUN_PUBLIC पूर्णांक libcrun_container_delete (libcrun_context_t *context, स्थिर अक्षर *id, bool force,
                                             libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_container_kill (libcrun_context_t *context, स्थिर अक्षर *id, स्थिर अक्षर *संकेत,
                                           libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_container_killall (libcrun_context_t *context, स्थिर अक्षर *id, स्थिर अक्षर *संकेत,
                                              libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container,
                                             अचिन्हित पूर्णांक options, libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_container_start (libcrun_context_t *context, स्थिर अक्षर *id, libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_container_state (libcrun_context_t *context, स्थिर अक्षर *id, खाता *out,
                                            libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_container_state_json (libcrun_context_t *context, स्थिर अक्षर *id, अक्षर **out,
                                                 libcrun_error_t *err);

पूर्णांक libcrun_container_notify_handler (काष्ठा container_entrypoint_s *args,
                                      क्रमागत handler_configure_phase phase,
                                      libcrun_container_t *container, स्थिर अक्षर *rootfs,
                                      libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_get_container_state_string (स्थिर अक्षर *id, libcrun_container_status_t *status,
                                                       स्थिर अक्षर *state_मूल, स्थिर अक्षर **container_status,
                                                       पूर्णांक *running, libcrun_error_t *err);

काष्ठा libcrun_container_exec_options_s
{
  माप_प्रकार struct_आकार;
  runtime_spec_schema_config_schema_process *process;
  स्थिर अक्षर *path;
  स्थिर अक्षर *cgroup;
  bool merge_env;
};

LIBCRUN_PUBLIC पूर्णांक libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_load_process_from_file (स्थिर अक्षर *path,
                                                   runtime_spec_schema_config_schema_process **process,
                                                   libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_container_exec_with_options (libcrun_context_t *context, स्थिर अक्षर *id,
                                                        काष्ठा libcrun_container_exec_options_s *opts,
                                                        libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_container_exec (libcrun_context_t *context, स्थिर अक्षर *id,
                                           runtime_spec_schema_config_schema_process *process, libcrun_error_t *err);

काष्ठा libcrun_exec_options_s
{
  माप_प्रकार struct_आकार;
  स्थिर अक्षर *process_json; /* OCI Process JSON */
  स्थिर अक्षर *cgroup;
  bool merge_env;
};

LIBCRUN_PUBLIC पूर्णांक libcrun_container_exec_json (libcrun_context_t *context, स्थिर अक्षर *id, स्थिर अक्षर *process_json,
                                                libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_container_exec_with_options_json (libcrun_context_t *context, स्थिर अक्षर *id,
                                                             स्थिर काष्ठा libcrun_exec_options_s *opts,
                                                             libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_container_exec_process_file (libcrun_context_t *context, स्थिर अक्षर *id, स्थिर अक्षर *path,
                                                        libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_container_update (libcrun_context_t *context, स्थिर अक्षर *id, स्थिर अक्षर *content,
                                             माप_प्रकार len, libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_container_update_from_file (libcrun_context_t *context, स्थिर अक्षर *id, स्थिर अक्षर *file,
                                                       libcrun_error_t *err);

काष्ठा libcrun_update_मूल्य_s
{
  स्थिर अक्षर *section;
  स्थिर अक्षर *name;
  bool numeric;
  स्थिर अक्षर *मूल्य;
};

LIBCRUN_PUBLIC पूर्णांक libcrun_container_update_from_मूल्यावली (libcrun_context_t *context, स्थिर अक्षर *id,
                                                         काष्ठा libcrun_update_मूल्य_s *मूल्यावली, माप_प्रकार len,
                                                         libcrun_error_t *err);

काष्ठा libcrun_intel_rdt_update
{
  स्थिर अक्षर *l3_cache_schema;
  स्थिर अक्षर *mem_bw_schema;
  अक्षर *स्थिर *schemata;
};

LIBCRUN_PUBLIC पूर्णांक libcrun_container_update_intel_rdt (libcrun_context_t *context, स्थिर अक्षर *id,
                                                       काष्ठा libcrun_intel_rdt_update *update, libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_container_get_features (libcrun_context_t *context, काष्ठा features_info_s **info,
                                                   libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_container_get_features_json (libcrun_context_t *context, अक्षर **out, libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_container_pause (libcrun_context_t *context, स्थिर अक्षर *id, libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_container_unpause (libcrun_context_t *context, स्थिर अक्षर *id, libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_container_checkpoint (libcrun_context_t *context, स्थिर अक्षर *id,
                                                 काष्ठा libcrun_checkpoint_restore_options_s *cr_options,
                                                 libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_container_restore (libcrun_context_t *context, स्थिर अक्षर *id,
                                              काष्ठा libcrun_checkpoint_restore_options_s *cr_options,
                                              libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_container_read_pids (libcrun_context_t *context, स्थिर अक्षर *id, bool recurse, pid_t **pids, libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_write_json_containers_list (libcrun_context_t *context, खाता *out, libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_container_list_json (libcrun_context_t *context, अक्षर **out, libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_container_add_mounts_from_file (libcrun_context_t *context, स्थिर अक्षर *id, स्थिर अक्षर *file,
                                                           libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णांक libcrun_container_remove_mounts_from_file (libcrun_context_t *context, स्थिर अक्षर *id, स्थिर अक्षर *file,
                                                              libcrun_error_t *err);

// Not part of the public API, just a method in container.c we need to access from linux.c
व्योम get_मूल_in_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid,
                             uid_t *uid, gid_t *gid);

अटल अंतरभूत व्योम
cleanup_containerp (libcrun_container_t **c)
{
  libcrun_container_t *container = *c;
  libcrun_container_free (container);
}

#घोषणा cleanup_container __attribute__ ((cleanup (cleanup_containerp)))

अटल अंतरभूत व्योम
cleanup_struct_features_free (काष्ठा features_info_s **info)
{
  माप_प्रकार i;
  काष्ठा features_info_s *ptr;
  अगर (info == शून्य || *info == शून्य)
    वापस;

  ptr = *info;

  // Free oci_version_min if it is not NULL
  अगर (ptr->oci_version_min != शून्य)
    {
      मुक्त (ptr->oci_version_min);
      ptr->oci_version_min = शून्य; // Set to NULL after freeing
    }

  // Free oci_version_max if it is not NULL
  अगर (ptr->oci_version_max != शून्य)
    {
      मुक्त (ptr->oci_version_max);
      ptr->oci_version_max = शून्य; // Set to NULL after freeing
    }
  अगर (ptr->hooks != शून्य)
    {
      क्रम (i = 0; ptr->hooks[i] != शून्य; i++)
        मुक्त (ptr->hooks[i]);
      मुक्त (ptr->hooks);
    }
  अगर (ptr->mount_options != शून्य)
    {
      क्रम (i = 0; ptr->mount_options[i] != शून्य; i++)
        मुक्त (ptr->mount_options[i]);
      मुक्त (ptr->mount_options);
    }
  अगर (ptr->linux.namespaces != शून्य)
    {
      क्रम (i = 0; ptr->linux.namespaces[i] != शून्य; i++)
        मुक्त (ptr->linux.namespaces[i]);
      मुक्त (ptr->linux.namespaces);
    }

  अगर (ptr->linux.capabilities != शून्य)
    {
      क्रम (माप_प्रकार i = 0; ptr->linux.capabilities[i] != शून्य; i++)
        मुक्त (ptr->linux.capabilities[i]);
      मुक्त (ptr->linux.capabilities);
    }
  अगर (ptr->linux.seccomp.actions != शून्य)
    {
      क्रम (i = 0; ptr->linux.seccomp.actions[i] != शून्य; i++)
        मुक्त (ptr->linux.seccomp.actions[i]);
      मुक्त (ptr->linux.seccomp.actions);
    }
  अगर (ptr->linux.seccomp.operators != शून्य)
    {
      क्रम (i = 0; ptr->linux.seccomp.operators[i] != शून्य; i++)
        मुक्त (ptr->linux.seccomp.operators[i]);
      मुक्त (ptr->linux.seccomp.operators);
    }

  मुक्त (ptr);
  *info = शून्य;
}

#घोषणा cleanup_struct_features __attribute__ ((cleanup (cleanup_struct_features_free)))

#पूर्ण_अगर
