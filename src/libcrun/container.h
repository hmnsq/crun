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

#अन्तर्भाव <config.h>
#अन्तर्भाव <ocispec/runtime_spec_schema_config_schema.h>
#अन्तर्भाव "error.h"
#अन्तर्भाव "string_map.h"

गणना handler_configure_phase
{
  HANDLER_CONFIGURE_BEFORE_MOUNTS = 1,
  HANDLER_CONFIGURE_AFTER_MOUNTS,
  HANDLER_CONFIGURE_MOUNTS,
  HANDLER_CONFIGURE_BEFORE_USERNS
};

संरचना custom_handler_manager_s;

संरचना libcrun_context_s
{
  अचर वर्ण *state_जरा;
  अचर वर्ण *id;
  अचर वर्ण *bundle;
  अचर वर्ण *console_socket;
  अचर वर्ण *pid_file;
  अचर वर्ण *notify_socket;
  अचर वर्ण *handler;
  वर्ण *owned_state_जरा;
  वर्ण *owned_id;
  वर्ण *owned_bundle;
  वर्ण *owned_console_socket;
  वर्ण *owned_pid_file;
  वर्ण *owned_notify_socket;
  वर्ण *owned_handler;
  पूर्णाङ्क preserve_fds;
  // For some use-cases we need differentiation between preserve_fds and listen_fds.
  // Following context variable makes sure we get exact value of listen_fds irrespective of preserve_fds.
  पूर्णाङ्क listen_fds;

  crun_output_handler output_handler;
  शून्य *output_handler_arg;

  पूर्णाङ्क fifo_exec_wait_fd;

  bool systemd_cgroup;
  bool detach;
  bool no_new_keyring;
  bool force_no_cgroup;
  bool no_pivot;
  bool no_open_रूख_namespace;

  वर्ण **argv;
  पूर्णाङ्क argc;

  संरचना custom_handler_manager_s *handler_manager;
};

गणना
{
  LIBCRUN_RUN_OPTIONS_PREFORK = 1 << 0,
  LIBCRUN_RUN_OPTIONS_KEEP = 1 << 1,
};

गणना
{
  LIBCRUN_CREATE_OPTIONS_PREFORK = 1 << 0,
};

संरचना libcrun_container_s
{
  /* Container parsed from the runtime json file.  */
  runtime_spec_schema_config_schema *container_def;

  uid_t host_uid;
  gid_t host_gid;

  /* Whether crun has CAP_SYS_PTRACE in its user namespace.  */
  bool host_has_cap_sys_ptrace;

  uid_t container_uid;
  gid_t container_gid;

  वर्ण *config_file;
  वर्ण *config_file_content;

  string_map *annotations;

  पूर्णाङ्क proc_fd;

  शून्य *private_data;
  शून्य (*cleanup_private_data) (शून्य *private_data);
  संरचना libcrun_context_s *context;
};

संरचना libcrun_container_status_s;
प्रकारनिर्देश संरचना libcrun_container_status_s libcrun_container_status_t;

प्रकारनिर्देश संरचना libcrun_container_s libcrun_container_t;
प्रकारनिर्देश संरचना libcrun_context_s libcrun_context_t;

संरचना container_entrypoint_s;
संरचना cgroup_info_s
{
  bool v1;
  bool v2;
  bool systemd;
  bool systemd_user;
};

संरचना seccomp_info_s
{
  bool enabled;
  वर्ण **actions;
  वर्ण **operators;
  वर्ण **archs;
};

संरचना apparmor_info_s
{
  bool enabled;
};

संरचना selinux_info_s
{
  bool enabled;
};

संरचना idmap_info_s
{
  bool enabled;
};

संरचना intel_rdt_s
{
  bool enabled;
};

संरचना net_devices_s
{
  bool enabled;
};

संरचना mount_ext_info_s
{
  संरचना idmap_info_s idmap;
};

संरचना memory_policy_info_s
{
  वर्ण **mode;
  वर्ण **flags;
};

संरचना linux_info_s
{
  वर्ण **namespaces;
  वर्ण **capabilities;
  संरचना cgroup_info_s cgroup;
  संरचना seccomp_info_s seccomp;
  संरचना apparmor_info_s apparmor;
  संरचना selinux_info_s selinux;
  संरचना mount_ext_info_s mount_ext;
  संरचना intel_rdt_s intel_rdt;
  संरचना net_devices_s net_devices;
  संरचना memory_policy_info_s memory_policy;
};

संरचना annotations_info_s
{
  वर्ण *io_github_seccomp_libseccomp_version;
  bool run_oci_crun_checkpoint_enabled;
  वर्ण *run_oci_crun_commit;
  वर्ण *run_oci_crun_version;
  bool run_oci_crun_wasm;
};

संरचना features_info_s
{
  वर्ण *oci_version_min;
  वर्ण *oci_version_max;
  वर्ण **hooks;
  वर्ण **mount_options;
  संरचना linux_info_s linux;
  संरचना annotations_info_s annotations;
  वर्ण **potentially_unsafe_annotations;
};

संरचना libcrun_checkpoint_restore_s
{
  वर्ण *image_path;
  वर्ण *work_path;
  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool shell_job;
  bool ext_unix_sk;
  bool detach;
  bool file_locks;
  अचर वर्ण *console_socket;
  वर्ण *parent_path;
  bool pre_dump;
  पूर्णाङ्क manage_cgroups_mode;
  पूर्णाङ्क network_lock_method;
  वर्ण *lsm_profile;
  वर्ण *lsm_mount_context;
};
प्रकारनिर्देश संरचना libcrun_checkpoint_restore_s libcrun_checkpoint_restore_t;

/* Public checkpoint/restore options.  Mirror of the definition in
   <libcrun.h>; keep the two in sync.  */
गणना libcrun_cr_cgroups_mode
{
  LIBCRUN_CR_CG_MODE_DEFAULT = 0, /* soft, matching runc */
  LIBCRUN_CR_CG_MODE_SOFT,
  LIBCRUN_CR_CG_MODE_IGNORE,
  LIBCRUN_CR_CG_MODE_FULL,
  LIBCRUN_CR_CG_MODE_STRICT,
};

गणना libcrun_cr_network_lock_method
{
  LIBCRUN_CR_NETWORK_LOCK_DEFAULT = 0, /* iptables */
  LIBCRUN_CR_NETWORK_LOCK_IPTABLES,
  LIBCRUN_CR_NETWORK_LOCK_NFTABLES,
  LIBCRUN_CR_NETWORK_LOCK_SKIP,
};

संरचना libcrun_checkpoint_restore_options_s
{
  माप_प्रकार struct_size;

  अचर वर्ण *image_path;
  अचर वर्ण *work_path;
  अचर वर्ण *parent_path;
  अचर वर्ण *console_socket;
  अचर वर्ण *lsm_profile;
  अचर वर्ण *lsm_mount_context;

  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool ext_unix_sk;
  bool shell_job;
  bool file_locks;
  bool pre_dump;
  bool detach;

  पूर्णाङ्क manage_cgroups_mode; /* enum libcrun_cr_cgroups_mode */
  पूर्णाङ्क network_lock_method; /* enum libcrun_cr_network_lock_method */
};

LIBCRUN_PUBLIC libcrun_container_t *libcrun_container_load_from_file (अचर वर्ण *path, libcrun_error_t *err);

LIBCRUN_PUBLIC libcrun_container_t *libcrun_container_load_from_memory (अचर वर्ण *json, libcrun_error_t *err);

LIBCRUN_PUBLIC शून्य libcrun_container_free (libcrun_container_t *);

/* Return the configuration of CONTAINER as a JSON string owned by CONTAINER.
   The document is generated on demand and cached, so the first call mutates
   CONTAINER and must not race with other users of the same handle.  */
LIBCRUN_PUBLIC अचर वर्ण *libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err);
LIBCRUN_PUBLIC अचर वर्ण *libcrun_container_get_annotation (libcrun_container_t *container, अचर वर्ण *साँचो);
LIBCRUN_PUBLIC माप_प्रकार libcrun_container_get_annotations_len (libcrun_container_t *container);
LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_get_annotation_at (libcrun_container_t *container, माप_प्रकार index,
                                                        अचर वर्ण **साँचो, अचर वर्ण **मान);
LIBCRUN_PUBLIC uid_t libcrun_container_get_uid (libcrun_container_t *container);
LIBCRUN_PUBLIC gid_t libcrun_container_get_gid (libcrun_container_t *container);

LIBCRUN_PUBLIC libcrun_context_t *libcrun_context_new (अचर वर्ण *id, अचर वर्ण *state_जरा, libcrun_error_t *err);
LIBCRUN_PUBLIC शून्य libcrun_context_free (libcrun_context_t *ctx);

LIBCRUN_PUBLIC शून्य libcrun_context_set_id (libcrun_context_t *ctx, अचर वर्ण *मान);
LIBCRUN_PUBLIC शून्य libcrun_context_set_state_जरा (libcrun_context_t *ctx, अचर वर्ण *मान);
LIBCRUN_PUBLIC अचर वर्ण *libcrun_context_get_state_जरा (libcrun_context_t *ctx);
LIBCRUN_PUBLIC शून्य libcrun_context_set_bundle (libcrun_context_t *ctx, अचर वर्ण *मान);
LIBCRUN_PUBLIC शून्य libcrun_context_set_console_socket (libcrun_context_t *ctx, अचर वर्ण *मान);
LIBCRUN_PUBLIC शून्य libcrun_context_set_pid_file (libcrun_context_t *ctx, अचर वर्ण *मान);
LIBCRUN_PUBLIC अचर वर्ण *libcrun_context_get_pid_file (libcrun_context_t *ctx);
LIBCRUN_PUBLIC शून्य libcrun_context_set_notify_socket (libcrun_context_t *ctx, अचर वर्ण *मान);
LIBCRUN_PUBLIC शून्य libcrun_context_set_handler (libcrun_context_t *ctx, अचर वर्ण *मान);
LIBCRUN_PUBLIC शून्य libcrun_context_set_preserve_fds (libcrun_context_t *ctx, पूर्णाङ्क preserve_fds);
LIBCRUN_PUBLIC पूर्णाङ्क libcrun_context_get_preserve_fds (libcrun_context_t *ctx);
LIBCRUN_PUBLIC शून्य libcrun_context_set_listen_fds (libcrun_context_t *ctx, पूर्णाङ्क listen_fds);
LIBCRUN_PUBLIC शून्य libcrun_context_set_args (libcrun_context_t *ctx, पूर्णाङ्क argc, वर्ण **argv);
LIBCRUN_PUBLIC शून्य libcrun_context_set_systemd_cgroup (libcrun_context_t *ctx, bool मान);
LIBCRUN_PUBLIC शून्य libcrun_context_set_detach (libcrun_context_t *ctx, bool मान);
LIBCRUN_PUBLIC शून्य libcrun_context_set_no_new_keyring (libcrun_context_t *ctx, bool मान);
LIBCRUN_PUBLIC शून्य libcrun_context_set_no_pivot (libcrun_context_t *ctx, bool मान);
LIBCRUN_PUBLIC शून्य libcrun_context_set_force_no_cgroup (libcrun_context_t *ctx, bool मान);
LIBCRUN_PUBLIC शून्य libcrun_context_set_output_handler (libcrun_context_t *ctx, crun_output_handler handler, शून्य *arg);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_context_init_logging (libcrun_context_t *ctx, अचर वर्ण *log, libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_run (libcrun_context_t *context, libcrun_container_t *container,
                                          अचिन्हित पूर्णाङ्क options, libcrun_error_t *error);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_delete (libcrun_context_t *context, अचर वर्ण *id, bool force,
                                             libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_kill (libcrun_context_t *context, अचर वर्ण *id, अचर वर्ण *सङ्केत,
                                           libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_killall (libcrun_context_t *context, अचर वर्ण *id, अचर वर्ण *सङ्केत,
                                              libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container,
                                             अचिन्हित पूर्णाङ्क options, libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_start (libcrun_context_t *context, अचर वर्ण *id, libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_state (libcrun_context_t *context, अचर वर्ण *id, खाता *out,
                                            libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_state_json (libcrun_context_t *context, अचर वर्ण *id, वर्ण **out,
                                                 libcrun_error_t *err);

पूर्णाङ्क libcrun_container_notify_handler (संरचना container_entrypoint_s *args,
                                      गणना handler_configure_phase phase,
                                      libcrun_container_t *container, अचर वर्ण *rootfs,
                                      libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_get_container_state_string (अचर वर्ण *id, libcrun_container_status_t *status,
                                                       अचर वर्ण *state_जरा, अचर वर्ण **container_status,
                                                       पूर्णाङ्क *running, libcrun_error_t *err);

संरचना libcrun_container_exec_options_s
{
  माप_प्रकार struct_size;
  runtime_spec_schema_config_schema_process *process;
  अचर वर्ण *path;
  अचर वर्ण *cgroup;
  bool merge_env;
};

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_load_process_from_file (अचर वर्ण *path,
                                                   runtime_spec_schema_config_schema_process **process,
                                                   libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_exec_with_options (libcrun_context_t *context, अचर वर्ण *id,
                                                        संरचना libcrun_container_exec_options_s *opts,
                                                        libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_exec (libcrun_context_t *context, अचर वर्ण *id,
                                           runtime_spec_schema_config_schema_process *process, libcrun_error_t *err);

संरचना libcrun_exec_options_s
{
  माप_प्रकार struct_size;
  अचर वर्ण *process_json; /* OCI Process JSON */
  अचर वर्ण *cgroup;
  bool merge_env;
};

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_exec_json (libcrun_context_t *context, अचर वर्ण *id, अचर वर्ण *process_json,
                                                libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_exec_with_options_json (libcrun_context_t *context, अचर वर्ण *id,
                                                             अचर संरचना libcrun_exec_options_s *opts,
                                                             libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_exec_process_file (libcrun_context_t *context, अचर वर्ण *id, अचर वर्ण *path,
                                                        libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_update (libcrun_context_t *context, अचर वर्ण *id, अचर वर्ण *content,
                                             माप_प्रकार len, libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_update_from_file (libcrun_context_t *context, अचर वर्ण *id, अचर वर्ण *file,
                                                       libcrun_error_t *err);

संरचना libcrun_update_मान_s
{
  अचर वर्ण *section;
  अचर वर्ण *name;
  bool numeric;
  अचर वर्ण *मान;
};

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_update_from_मानहरू (libcrun_context_t *context, अचर वर्ण *id,
                                                         संरचना libcrun_update_मान_s *मानहरू, माप_प्रकार len,
                                                         libcrun_error_t *err);

संरचना libcrun_intel_rdt_update
{
  अचर वर्ण *l3_cache_schema;
  अचर वर्ण *mem_bw_schema;
  वर्ण *अचर *schemata;
};

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_update_intel_rdt (libcrun_context_t *context, अचर वर्ण *id,
                                                       संरचना libcrun_intel_rdt_update *update, libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_get_features (libcrun_context_t *context, संरचना features_info_s **info,
                                                   libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_get_features_json (libcrun_context_t *context, वर्ण **out, libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_pause (libcrun_context_t *context, अचर वर्ण *id, libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_unpause (libcrun_context_t *context, अचर वर्ण *id, libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_checkpoint (libcrun_context_t *context, अचर वर्ण *id,
                                                 संरचना libcrun_checkpoint_restore_options_s *cr_options,
                                                 libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_restore (libcrun_context_t *context, अचर वर्ण *id,
                                              संरचना libcrun_checkpoint_restore_options_s *cr_options,
                                              libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_read_pids (libcrun_context_t *context, अचर वर्ण *id, bool recurse, pid_t **pids, libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_write_json_containers_list (libcrun_context_t *context, खाता *out, libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_list_json (libcrun_context_t *context, वर्ण **out, libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_add_mounts_from_file (libcrun_context_t *context, अचर वर्ण *id, अचर वर्ण *file,
                                                           libcrun_error_t *err);

LIBCRUN_PUBLIC पूर्णाङ्क libcrun_container_remove_mounts_from_file (libcrun_context_t *context, अचर वर्ण *id, अचर वर्ण *file,
                                                              libcrun_error_t *err);

// Not part of the public API, just a method in container.c we need to access from linux.c
शून्य get_जरा_in_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid,
                             uid_t *uid, gid_t *gid);

स्थिर अन्तःस्थ शून्य
cleanup_containerp (libcrun_container_t **c)
{
  libcrun_container_t *container = *c;
  libcrun_container_free (container);
}

#define cleanup_container __attribute__ ((cleanup (cleanup_containerp)))

स्थिर अन्तःस्थ शून्य
cleanup_struct_features_free (संरचना features_info_s **info)
{
  माप_प्रकार i;
  संरचना features_info_s *ptr;
  यदि (info == रिक्त || *info == रिक्त)
    फर्काऊ;

  ptr = *info;

  // Free oci_version_min if it is not NULL
  यदि (ptr->oci_version_min != रिक्त)
    {
      मुक्त (ptr->oci_version_min);
      ptr->oci_version_min = रिक्त; // Set to NULL after freeing
    }

  // Free oci_version_max if it is not NULL
  यदि (ptr->oci_version_max != रिक्त)
    {
      मुक्त (ptr->oci_version_max);
      ptr->oci_version_max = रिक्त; // Set to NULL after freeing
    }
  यदि (ptr->hooks != रिक्त)
    {
      लागि (i = 0; ptr->hooks[i] != रिक्त; i++)
        मुक्त (ptr->hooks[i]);
      मुक्त (ptr->hooks);
    }
  यदि (ptr->mount_options != रिक्त)
    {
      लागि (i = 0; ptr->mount_options[i] != रिक्त; i++)
        मुक्त (ptr->mount_options[i]);
      मुक्त (ptr->mount_options);
    }
  यदि (ptr->linux.namespaces != रिक्त)
    {
      लागि (i = 0; ptr->linux.namespaces[i] != रिक्त; i++)
        मुक्त (ptr->linux.namespaces[i]);
      मुक्त (ptr->linux.namespaces);
    }

  यदि (ptr->linux.capabilities != रिक्त)
    {
      लागि (माप_प्रकार i = 0; ptr->linux.capabilities[i] != रिक्त; i++)
        मुक्त (ptr->linux.capabilities[i]);
      मुक्त (ptr->linux.capabilities);
    }
  यदि (ptr->linux.seccomp.actions != रिक्त)
    {
      लागि (i = 0; ptr->linux.seccomp.actions[i] != रिक्त; i++)
        मुक्त (ptr->linux.seccomp.actions[i]);
      मुक्त (ptr->linux.seccomp.actions);
    }
  यदि (ptr->linux.seccomp.operators != रिक्त)
    {
      लागि (i = 0; ptr->linux.seccomp.operators[i] != रिक्त; i++)
        मुक्त (ptr->linux.seccomp.operators[i]);
      मुक्त (ptr->linux.seccomp.operators);
    }

  मुक्त (ptr);
  *info = रिक्त;
}

#define cleanup_struct_features __attribute__ ((cleanup (cleanup_struct_features_free)))

#endif
