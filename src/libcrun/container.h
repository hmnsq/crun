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

#यदि_अपरिभाषितम् CONTAINER_H
#परिभाषय CONTAINER_H

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
  ध्रुव वर्ण *state_मूलम्;
  ध्रुव वर्ण *id;
  ध्रुव वर्ण *bundle;
  ध्रुव वर्ण *console_socket;
  ध्रुव वर्ण *pid_file;
  ध्रुव वर्ण *notify_socket;
  ध्रुव वर्ण *handler;
  वर्ण *owned_state_मूलम्;
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
  bool no_open_वृक्षः_namespace;

  वर्ण **argv;
  पूर्णाङ्क argc;

  संरचना custom_handler_manager_s *handler_manager;
};

गणना
{
  LIBCRUN_सततम्_OPTIONS_PREFORK = 1 << 0,
  LIBCRUN_सततम्_OPTIONS_KEEP = 1 << 1,
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

  शून्य *वैयक्तिकम्_दत्त;
  शून्य (*cleanup_वैयक्तिकम्_दत्त) (शून्य *वैयक्तिकम्_दत्त);
  संरचना libcrun_context_s *context;
};

संरचना libcrun_container_स्थितिः_s;
प्रकारनिर्देश संरचना libcrun_container_स्थितिः_s libcrun_container_स्थितिः_t;

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

संरचना स्मृतिः_policy_info_s
{
  वर्ण **बहुलकम्;
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
  संरचना स्मृतिः_policy_info_s स्मृतिः_policy;
};

संरचना annotations_info_s
{
  वर्ण *io_github_seccomp_libseccomp_संस्करणम्;
  bool सततम्_oci_crun_checkpoint_enabled;
  वर्ण *सततम्_oci_crun_commit;
  वर्ण *सततम्_oci_crun_संस्करणम्;
  bool सततम्_oci_crun_wasm;
};

संरचना features_info_s
{
  वर्ण *oci_संस्करणम्_min;
  वर्ण *oci_संस्करणम्_max;
  वर्ण **hooks;
  वर्ण **mount_options;
  संरचना linux_info_s linux;
  संरचना annotations_info_s annotations;
  वर्ण **potentially_unsafe_annotations;
};

संरचना libcrun_checkpoint_restore_s
{
  वर्ण *image_मार्गः;
  वर्ण *work_मार्गः;
  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool shell_job;
  bool ext_unix_sk;
  bool detach;
  bool file_locks;
  ध्रुव वर्ण *console_socket;
  वर्ण *parent_मार्गः;
  bool pre_dump;
  पूर्णाङ्क manage_cgroups_बहुलकम्;
  पूर्णाङ्क जालम्_तालकम्_विधानम्;
  वर्ण *lsm_profile;
  वर्ण *lsm_mount_context;
};
प्रकारनिर्देश संरचना libcrun_checkpoint_restore_s libcrun_checkpoint_restore_t;

/* Public checkpoint/restore options.  Mirror of the definition in
   <libcrun.h>; keep the two in sync.  */
गणना libcrun_cr_cgroups_बहुलकम्
{
  LIBCRUN_CR_CG_बहुलकम्_DEFAULT = 0, /* soft, matching runc */
  LIBCRUN_CR_CG_बहुलकम्_SOFT,
  LIBCRUN_CR_CG_बहुलकम्_IGNORE,
  LIBCRUN_CR_CG_बहुलकम्_पूर्णम्,
  LIBCRUN_CR_CG_बहुलकम्_STRICT,
};

गणना libcrun_cr_जालम्_तालकम्_विधानम्
{
  LIBCRUN_CR_जालम्_तालकम्_DEFAULT = 0, /* iptables */
  LIBCRUN_CR_जालम्_तालकम्_IPTABLES,
  LIBCRUN_CR_जालम्_तालकम्_NFTABLES,
  LIBCRUN_CR_जालम्_तालकम्_SKIP,
};

संरचना libcrun_checkpoint_restore_options_s
{
  size_t struct_आकार;

  ध्रुव वर्ण *image_मार्गः;
  ध्रुव वर्ण *work_मार्गः;
  ध्रुव वर्ण *parent_मार्गः;
  ध्रुव वर्ण *console_socket;
  ध्रुव वर्ण *lsm_profile;
  ध्रुव वर्ण *lsm_mount_context;

  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool ext_unix_sk;
  bool shell_job;
  bool file_locks;
  bool pre_dump;
  bool detach;

  पूर्णाङ्क manage_cgroups_बहुलकम्; /* enum libcrun_cr_cgroups_mode */
  पूर्णाङ्क जालम्_तालकम्_विधानम्; /* enum libcrun_cr_network_lock_method */
};

LIBCRUN_लौकिकम् libcrun_container_t *libcrun_container_आरोपय_from_file (ध्रुव वर्ण *मार्गः, libcrun_error_t *err);

LIBCRUN_लौकिकम् libcrun_container_t *libcrun_container_आरोपय_from_स्मृतिः (ध्रुव वर्ण *json, libcrun_error_t *err);

LIBCRUN_लौकिकम् शून्य libcrun_container_free (libcrun_container_t *);

/* Return the configuration of CONTAINER as a JSON string owned by CONTAINER.
   The document is generated on demand and cached, so the first call mutates
   CONTAINER and must not race with other users of the same handle.  */
LIBCRUN_लौकिकम् ध्रुव वर्ण *libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err);
LIBCRUN_लौकिकम् ध्रुव वर्ण *libcrun_container_get_annotation (libcrun_container_t *container, ध्रुव वर्ण *कुञ्चिका);
LIBCRUN_लौकिकम् size_t libcrun_container_get_annotations_len (libcrun_container_t *container);
LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_get_annotation_at (libcrun_container_t *container, size_t सूचकांक,
                                                        ध्रुव वर्ण **कुञ्चिका, ध्रुव वर्ण **मानम्);
LIBCRUN_लौकिकम् uid_t libcrun_container_get_uid (libcrun_container_t *container);
LIBCRUN_लौकिकम् gid_t libcrun_container_get_gid (libcrun_container_t *container);

LIBCRUN_लौकिकम् libcrun_context_t *libcrun_context_new (ध्रुव वर्ण *id, ध्रुव वर्ण *state_मूलम्, libcrun_error_t *err);
LIBCRUN_लौकिकम् शून्य libcrun_context_free (libcrun_context_t *ctx);

LIBCRUN_लौकिकम् शून्य libcrun_context_समुच्चयः_id (libcrun_context_t *ctx, ध्रुव वर्ण *मानम्);
LIBCRUN_लौकिकम् शून्य libcrun_context_समुच्चयः_state_मूलम् (libcrun_context_t *ctx, ध्रुव वर्ण *मानम्);
LIBCRUN_लौकिकम् ध्रुव वर्ण *libcrun_context_get_state_मूलम् (libcrun_context_t *ctx);
LIBCRUN_लौकिकम् शून्य libcrun_context_समुच्चयः_bundle (libcrun_context_t *ctx, ध्रुव वर्ण *मानम्);
LIBCRUN_लौकिकम् शून्य libcrun_context_समुच्चयः_console_socket (libcrun_context_t *ctx, ध्रुव वर्ण *मानम्);
LIBCRUN_लौकिकम् शून्य libcrun_context_समुच्चयः_pid_file (libcrun_context_t *ctx, ध्रुव वर्ण *मानम्);
LIBCRUN_लौकिकम् ध्रुव वर्ण *libcrun_context_get_pid_file (libcrun_context_t *ctx);
LIBCRUN_लौकिकम् शून्य libcrun_context_समुच्चयः_notify_socket (libcrun_context_t *ctx, ध्रुव वर्ण *मानम्);
LIBCRUN_लौकिकम् शून्य libcrun_context_समुच्चयः_handler (libcrun_context_t *ctx, ध्रुव वर्ण *मानम्);
LIBCRUN_लौकिकम् शून्य libcrun_context_समुच्चयः_preserve_fds (libcrun_context_t *ctx, पूर्णाङ्क preserve_fds);
LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_context_get_preserve_fds (libcrun_context_t *ctx);
LIBCRUN_लौकिकम् शून्य libcrun_context_समुच्चयः_listen_fds (libcrun_context_t *ctx, पूर्णाङ्क listen_fds);
LIBCRUN_लौकिकम् शून्य libcrun_context_समुच्चयः_तर्काः (libcrun_context_t *ctx, पूर्णाङ्क argc, वर्ण **argv);
LIBCRUN_लौकिकम् शून्य libcrun_context_समुच्चयः_systemd_cgroup (libcrun_context_t *ctx, bool मानम्);
LIBCRUN_लौकिकम् शून्य libcrun_context_समुच्चयः_detach (libcrun_context_t *ctx, bool मानम्);
LIBCRUN_लौकिकम् शून्य libcrun_context_समुच्चयः_no_new_keyring (libcrun_context_t *ctx, bool मानम्);
LIBCRUN_लौकिकम् शून्य libcrun_context_समुच्चयः_no_pivot (libcrun_context_t *ctx, bool मानम्);
LIBCRUN_लौकिकम् शून्य libcrun_context_समुच्चयः_force_no_cgroup (libcrun_context_t *ctx, bool मानम्);
LIBCRUN_लौकिकम् शून्य libcrun_context_समुच्चयः_output_handler (libcrun_context_t *ctx, crun_output_handler handler, शून्य *arg);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_context_init_logging (libcrun_context_t *ctx, ध्रुव वर्ण *लघुगणकः, libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_सततम् (libcrun_context_t *context, libcrun_container_t *container,
                                          अचिह्नित पूर्णाङ्क options, libcrun_error_t *error);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_delete (libcrun_context_t *context, ध्रुव वर्ण *id, bool force,
                                             libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_kill (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *सङ्केत,
                                           libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_killall (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *सङ्केत,
                                              libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container,
                                             अचिह्नित पूर्णाङ्क options, libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_start (libcrun_context_t *context, ध्रुव वर्ण *id, libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_state (libcrun_context_t *context, ध्रुव वर्ण *id, FILE *बहिर्गमनम्,
                                            libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_state_json (libcrun_context_t *context, ध्रुव वर्ण *id, वर्ण **बहिर्गमनम्,
                                                 libcrun_error_t *err);

पूर्णाङ्क libcrun_container_notify_handler (संरचना container_entrypoint_s *तर्काः,
                                      गणना handler_configure_phase phase,
                                      libcrun_container_t *container, ध्रुव वर्ण *rootfs,
                                      libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_get_container_state_string (ध्रुव वर्ण *id, libcrun_container_स्थितिः_t *स्थितिः,
                                                       ध्रुव वर्ण *state_मूलम्, ध्रुव वर्ण **container_स्थितिः,
                                                       पूर्णाङ्क *running, libcrun_error_t *err);

संरचना libcrun_container_exec_options_s
{
  size_t struct_आकार;
  runtime_spec_schema_config_schema_process *process;
  ध्रुव वर्ण *मार्गः;
  ध्रुव वर्ण *cgroup;
  bool merge_env;
};

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_आरोपय_process_from_file (ध्रुव वर्ण *मार्गः,
                                                   runtime_spec_schema_config_schema_process **process,
                                                   libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_exec_with_options (libcrun_context_t *context, ध्रुव वर्ण *id,
                                                        संरचना libcrun_container_exec_options_s *opts,
                                                        libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_exec (libcrun_context_t *context, ध्रुव वर्ण *id,
                                           runtime_spec_schema_config_schema_process *process, libcrun_error_t *err);

संरचना libcrun_exec_options_s
{
  size_t struct_आकार;
  ध्रुव वर्ण *process_json; /* OCI Process JSON */
  ध्रुव वर्ण *cgroup;
  bool merge_env;
};

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_exec_json (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *process_json,
                                                libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_exec_with_options_json (libcrun_context_t *context, ध्रुव वर्ण *id,
                                                             ध्रुव संरचना libcrun_exec_options_s *opts,
                                                             libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_exec_process_file (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *मार्गः,
                                                        libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_update (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *content,
                                             size_t len, libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_update_from_file (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *file,
                                                       libcrun_error_t *err);

संरचना libcrun_update_मानम्_s
{
  ध्रुव वर्ण *section;
  ध्रुव वर्ण *name;
  bool numeric;
  ध्रुव वर्ण *मानम्;
};

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_update_from_मानानि (libcrun_context_t *context, ध्रुव वर्ण *id,
                                                         संरचना libcrun_update_मानम्_s *मानानि, size_t len,
                                                         libcrun_error_t *err);

संरचना libcrun_intel_rdt_update
{
  ध्रुव वर्ण *l3_cache_schema;
  ध्रुव वर्ण *mem_bw_schema;
  वर्ण *ध्रुव *schemata;
};

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_update_intel_rdt (libcrun_context_t *context, ध्रुव वर्ण *id,
                                                       संरचना libcrun_intel_rdt_update *update, libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_get_features (libcrun_context_t *context, संरचना features_info_s **info,
                                                   libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_get_features_json (libcrun_context_t *context, वर्ण **बहिर्गमनम्, libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_pause (libcrun_context_t *context, ध्रुव वर्ण *id, libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_unpause (libcrun_context_t *context, ध्रुव वर्ण *id, libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_checkpoint (libcrun_context_t *context, ध्रुव वर्ण *id,
                                                 संरचना libcrun_checkpoint_restore_options_s *cr_options,
                                                 libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_restore (libcrun_context_t *context, ध्रुव वर्ण *id,
                                              संरचना libcrun_checkpoint_restore_options_s *cr_options,
                                              libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_read_pids (libcrun_context_t *context, ध्रुव वर्ण *id, bool recurse, pid_t **pids, libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_write_json_containers_list (libcrun_context_t *context, FILE *बहिर्गमनम्, libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_list_json (libcrun_context_t *context, वर्ण **बहिर्गमनम्, libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_सङ्कलय_mounts_from_file (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *file,
                                                           libcrun_error_t *err);

LIBCRUN_लौकिकम् पूर्णाङ्क libcrun_container_remove_mounts_from_file (libcrun_context_t *context, ध्रुव वर्ण *id, ध्रुव वर्ण *file,
                                                              libcrun_error_t *err);

// Not part of the public API, just a method in container.c we need to access from linux.c
शून्य get_मूलम्_अन्तर्_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid,
                             uid_t *uid, gid_t *gid);

स्थिर अन्तःस्थ शून्य
cleanup_containerp (libcrun_container_t **c)
{
  libcrun_container_t *container = *c;
  libcrun_container_free (container);
}

#परिभाषय cleanup_container __attribute__ ((cleanup (cleanup_containerp)))

स्थिर अन्तःस्थ शून्य
cleanup_struct_features_free (संरचना features_info_s **info)
{
  size_t i;
  संरचना features_info_s *ptr;
  यदि (info == रिक्त || *info == रिक्त)
    प्रत्यागम;

  ptr = *info;

  // Free oci_version_min if it is not NULL
  यदि (ptr->oci_संस्करणम्_min != रिक्त)
    {
      मुक्त (ptr->oci_संस्करणम्_min);
      ptr->oci_संस्करणम्_min = रिक्त; // Set to NULL after freeing
    }

  // Free oci_version_max if it is not NULL
  यदि (ptr->oci_संस्करणम्_max != रिक्त)
    {
      मुक्त (ptr->oci_संस्करणम्_max);
      ptr->oci_संस्करणम्_max = रिक्त; // Set to NULL after freeing
    }
  यदि (ptr->hooks != रिक्त)
    {
      प्रति (i = 0; ptr->hooks[i] != रिक्त; i++)
        मुक्त (ptr->hooks[i]);
      मुक्त (ptr->hooks);
    }
  यदि (ptr->mount_options != रिक्त)
    {
      प्रति (i = 0; ptr->mount_options[i] != रिक्त; i++)
        मुक्त (ptr->mount_options[i]);
      मुक्त (ptr->mount_options);
    }
  यदि (ptr->linux.namespaces != रिक्त)
    {
      प्रति (i = 0; ptr->linux.namespaces[i] != रिक्त; i++)
        मुक्त (ptr->linux.namespaces[i]);
      मुक्त (ptr->linux.namespaces);
    }

  यदि (ptr->linux.capabilities != रिक्त)
    {
      प्रति (size_t i = 0; ptr->linux.capabilities[i] != रिक्त; i++)
        मुक्त (ptr->linux.capabilities[i]);
      मुक्त (ptr->linux.capabilities);
    }
  यदि (ptr->linux.seccomp.actions != रिक्त)
    {
      प्रति (i = 0; ptr->linux.seccomp.actions[i] != रिक्त; i++)
        मुक्त (ptr->linux.seccomp.actions[i]);
      मुक्त (ptr->linux.seccomp.actions);
    }
  यदि (ptr->linux.seccomp.operators != रिक्त)
    {
      प्रति (i = 0; ptr->linux.seccomp.operators[i] != रिक्त; i++)
        मुक्त (ptr->linux.seccomp.operators[i]);
      मुक्त (ptr->linux.seccomp.operators);
    }

  मुक्त (ptr);
  *info = रिक्त;
}

#परिभाषय cleanup_struct_features __attribute__ ((cleanup (cleanup_struct_features_free)))

#यदि_अन्तः
