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

#که_نه_تعریف_شوی CONTAINER_H
#تعریف_کړه CONTAINER_H

#ضمن <config.h>
#ضمن <ocispec/runtime_spec_schema_config_schema.h>
#ضمن "error.h"
#ضمن "string_map.h"

تعداد handler_configure_phase
{
  HANDLER_CONFIGURE_BEFORE_MOUNTS = 1,
  HANDLER_CONFIGURE_AFTER_MOUNTS,
  HANDLER_CONFIGURE_MOUNTS,
  HANDLER_CONFIGURE_BEFORE_USERNS
};

بنية custom_handler_manager_s;

بنية libcrun_context_s
{
  ثابت حرف *state_ریښه;
  ثابت حرف *id;
  ثابت حرف *bundle;
  ثابت حرف *console_socket;
  ثابت حرف *pid_file;
  ثابت حرف *notify_socket;
  ثابت حرف *handler;
  حرف *owned_state_ریښه;
  حرف *owned_id;
  حرف *owned_bundle;
  حرف *owned_console_socket;
  حرف *owned_pid_file;
  حرف *owned_notify_socket;
  حرف *owned_handler;
  صحيح preserve_fds;
  // For some use-cases we need differentiation between preserve_fds and listen_fds.
  // Following context variable makes sure we get exact value of listen_fds irrespective of preserve_fds.
  صحيح listen_fds;

  crun_output_handler output_handler;
  فراغ *output_handler_arg;

  صحيح fifo_exec_wait_fd;

  bool systemd_cgroup;
  bool detach;
  bool no_new_keyring;
  bool force_no_cgroup;
  bool no_محور;
  bool no_open_ونه_namespace;

  حرف **argv;
  صحيح argc;

  بنية custom_handler_manager_s *handler_manager;
};

تعداد
{
  LIBCRUN_پرلپسې_OPTIONS_PREFORK = 1 << 0,
  LIBCRUN_پرلپسې_OPTIONS_KEEP = 1 << 1,
};

تعداد
{
  LIBCRUN_CREATE_OPTIONS_PREFORK = 1 << 0,
};

بنية libcrun_container_s
{
  /* Container parsed from the runtime json file.  */
  runtime_spec_schema_config_schema *container_def;

  uid_t host_uid;
  gid_t host_gid;

  /* Whether crun has CAP_SYS_PTRACE in its user namespace.  */
  bool host_has_cap_sys_ptrace;

  uid_t container_uid;
  gid_t container_gid;

  حرف *config_file;
  حرف *config_file_content;

  string_map *annotations;

  صحيح proc_fd;

  فراغ *شخصي_معلومات;
  فراغ (*cleanup_شخصي_معلومات) (فراغ *شخصي_معلومات);
  بنية libcrun_context_s *context;
};

بنية libcrun_container_حالت_s;
تعريفنوع بنية libcrun_container_حالت_s libcrun_container_حالت_t;

تعريفنوع بنية libcrun_container_s libcrun_container_t;
تعريفنوع بنية libcrun_context_s libcrun_context_t;

بنية container_entrypoint_s;
بنية cgroup_info_s
{
  bool v1;
  bool v2;
  bool systemd;
  bool systemd_user;
};

بنية seccomp_info_s
{
  bool enabled;
  حرف **actions;
  حرف **operators;
  حرف **archs;
};

بنية apparmor_info_s
{
  bool enabled;
};

بنية selinux_info_s
{
  bool enabled;
};

بنية idmap_info_s
{
  bool enabled;
};

بنية intel_rdt_s
{
  bool enabled;
};

بنية net_devices_s
{
  bool enabled;
};

بنية mount_ext_info_s
{
  بنية idmap_info_s idmap;
};

بنية حافظه_policy_info_s
{
  حرف **ډېرپېښ;
  حرف **flags;
};

بنية linux_info_s
{
  حرف **namespaces;
  حرف **capabilities;
  بنية cgroup_info_s cgroup;
  بنية seccomp_info_s seccomp;
  بنية apparmor_info_s apparmor;
  بنية selinux_info_s selinux;
  بنية mount_ext_info_s mount_ext;
  بنية intel_rdt_s intel_rdt;
  بنية net_devices_s net_devices;
  بنية حافظه_policy_info_s حافظه_policy;
};

بنية annotations_info_s
{
  حرف *io_github_seccomp_libseccomp_نسخه;
  bool پرلپسې_oci_crun_checkpoint_enabled;
  حرف *پرلپسې_oci_crun_commit;
  حرف *پرلپسې_oci_crun_نسخه;
  bool پرلپسې_oci_crun_wasm;
};

بنية features_info_s
{
  حرف *oci_نسخه_min;
  حرف *oci_نسخه_max;
  حرف **hooks;
  حرف **mount_options;
  بنية linux_info_s linux;
  بنية annotations_info_s annotations;
  حرف **potentially_unsafe_annotations;
};

بنية libcrun_checkpoint_restore_s
{
  حرف *image_لار;
  حرف *work_لار;
  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool shell_دنده;
  bool ext_unix_sk;
  bool detach;
  bool file_locks;
  ثابت حرف *console_socket;
  حرف *پلار_لار;
  bool pre_dump;
  صحيح manage_cgroups_ډېرپېښ;
  صحيح شبکه_قلف_لاره;
  حرف *lsm_profile;
  حرف *lsm_mount_context;
};
تعريفنوع بنية libcrun_checkpoint_restore_s libcrun_checkpoint_restore_t;

/* Public checkpoint/restore options.  Mirror of the definition in
   <libcrun.h>; keep the two in sync.  */
تعداد libcrun_cr_cgroups_ډېرپېښ
{
  LIBCRUN_CR_CG_ډېرپېښ_DEFAULT = 0, /* soft, matching runc */
  LIBCRUN_CR_CG_ډېرپېښ_SOFT,
  LIBCRUN_CR_CG_ډېرپېښ_IGNORE,
  LIBCRUN_CR_CG_ډېرپېښ_ډک,
  LIBCRUN_CR_CG_ډېرپېښ_STRICT,
};

تعداد libcrun_cr_شبکه_قلف_لاره
{
  LIBCRUN_CR_شبکه_قلف_DEFAULT = 0, /* iptables */
  LIBCRUN_CR_شبکه_قلف_IPTABLES,
  LIBCRUN_CR_شبکه_قلف_NFTABLES,
  LIBCRUN_CR_شبکه_قلف_SKIP,
};

بنية libcrun_checkpoint_restore_options_s
{
  size_t struct_اندازه;

  ثابت حرف *image_لار;
  ثابت حرف *work_لار;
  ثابت حرف *پلار_لار;
  ثابت حرف *console_socket;
  ثابت حرف *lsm_profile;
  ثابت حرف *lsm_mount_context;

  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool ext_unix_sk;
  bool shell_دنده;
  bool file_locks;
  bool pre_dump;
  bool detach;

  صحيح manage_cgroups_ډېرپېښ; /* enum libcrun_cr_cgroups_mode */
  صحيح شبکه_قلف_لاره; /* enum libcrun_cr_network_lock_method */
};

LIBCRUN_عامه libcrun_container_t *libcrun_container_بارول_from_file (ثابت حرف *لار, libcrun_error_t *err);

LIBCRUN_عامه libcrun_container_t *libcrun_container_بارول_from_حافظه (ثابت حرف *json, libcrun_error_t *err);

LIBCRUN_عامه فراغ libcrun_container_free (libcrun_container_t *);

/* Return the configuration of CONTAINER as a JSON string owned by CONTAINER.
   The document is generated on demand and cached, so the first call mutates
   CONTAINER and must not race with other users of the same handle.  */
LIBCRUN_عامه ثابت حرف *libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err);
LIBCRUN_عامه ثابت حرف *libcrun_container_get_annotation (libcrun_container_t *container, ثابت حرف *کلی);
LIBCRUN_عامه size_t libcrun_container_get_annotations_len (libcrun_container_t *container);
LIBCRUN_عامه صحيح libcrun_container_get_annotation_at (libcrun_container_t *container, size_t شاخص,
                                                        ثابت حرف **کلی, ثابت حرف **ارزښت);
LIBCRUN_عامه uid_t libcrun_container_get_uid (libcrun_container_t *container);
LIBCRUN_عامه gid_t libcrun_container_get_gid (libcrun_container_t *container);

LIBCRUN_عامه libcrun_context_t *libcrun_context_new (ثابت حرف *id, ثابت حرف *state_ریښه, libcrun_error_t *err);
LIBCRUN_عامه فراغ libcrun_context_free (libcrun_context_t *ctx);

LIBCRUN_عامه فراغ libcrun_context_سېټ_id (libcrun_context_t *ctx, ثابت حرف *ارزښت);
LIBCRUN_عامه فراغ libcrun_context_سېټ_state_ریښه (libcrun_context_t *ctx, ثابت حرف *ارزښت);
LIBCRUN_عامه ثابت حرف *libcrun_context_get_state_ریښه (libcrun_context_t *ctx);
LIBCRUN_عامه فراغ libcrun_context_سېټ_bundle (libcrun_context_t *ctx, ثابت حرف *ارزښت);
LIBCRUN_عامه فراغ libcrun_context_سېټ_console_socket (libcrun_context_t *ctx, ثابت حرف *ارزښت);
LIBCRUN_عامه فراغ libcrun_context_سېټ_pid_file (libcrun_context_t *ctx, ثابت حرف *ارزښت);
LIBCRUN_عامه ثابت حرف *libcrun_context_get_pid_file (libcrun_context_t *ctx);
LIBCRUN_عامه فراغ libcrun_context_سېټ_notify_socket (libcrun_context_t *ctx, ثابت حرف *ارزښت);
LIBCRUN_عامه فراغ libcrun_context_سېټ_handler (libcrun_context_t *ctx, ثابت حرف *ارزښت);
LIBCRUN_عامه فراغ libcrun_context_سېټ_preserve_fds (libcrun_context_t *ctx, صحيح preserve_fds);
LIBCRUN_عامه صحيح libcrun_context_get_preserve_fds (libcrun_context_t *ctx);
LIBCRUN_عامه فراغ libcrun_context_سېټ_listen_fds (libcrun_context_t *ctx, صحيح listen_fds);
LIBCRUN_عامه فراغ libcrun_context_سېټ_دلیلونه (libcrun_context_t *ctx, صحيح argc, حرف **argv);
LIBCRUN_عامه فراغ libcrun_context_سېټ_systemd_cgroup (libcrun_context_t *ctx, bool ارزښت);
LIBCRUN_عامه فراغ libcrun_context_سېټ_detach (libcrun_context_t *ctx, bool ارزښت);
LIBCRUN_عامه فراغ libcrun_context_سېټ_no_new_keyring (libcrun_context_t *ctx, bool ارزښت);
LIBCRUN_عامه فراغ libcrun_context_سېټ_no_محور (libcrun_context_t *ctx, bool ارزښت);
LIBCRUN_عامه فراغ libcrun_context_سېټ_force_no_cgroup (libcrun_context_t *ctx, bool ارزښت);
LIBCRUN_عامه فراغ libcrun_context_سېټ_output_handler (libcrun_context_t *ctx, crun_output_handler handler, فراغ *arg);

LIBCRUN_عامه صحيح libcrun_context_init_logging (libcrun_context_t *ctx, ثابت حرف *لوګاریتم, libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_container_پرلپسې (libcrun_context_t *context, libcrun_container_t *container,
                                          بینښه صحيح options, libcrun_error_t *error);

LIBCRUN_عامه صحيح libcrun_container_delete (libcrun_context_t *context, ثابت حرف *id, bool force,
                                             libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_container_kill (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *إشارة,
                                           libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_container_killall (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *إشارة,
                                              libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_container_create (libcrun_context_t *context, libcrun_container_t *container,
                                             بینښه صحيح options, libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_container_پیل (libcrun_context_t *context, ثابت حرف *id, libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_container_state (libcrun_context_t *context, ثابت حرف *id, FILE *بهر,
                                            libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_container_state_json (libcrun_context_t *context, ثابت حرف *id, حرف **بهر,
                                                 libcrun_error_t *err);

صحيح libcrun_container_notify_handler (بنية container_entrypoint_s *دلیلونه,
                                      تعداد handler_configure_phase phase,
                                      libcrun_container_t *container, ثابت حرف *rootfs,
                                      libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_get_container_state_string (ثابت حرف *id, libcrun_container_حالت_t *حالت,
                                                       ثابت حرف *state_ریښه, ثابت حرف **container_حالت,
                                                       صحيح *running, libcrun_error_t *err);

بنية libcrun_container_exec_options_s
{
  size_t struct_اندازه;
  runtime_spec_schema_config_schema_process *process;
  ثابت حرف *لار;
  ثابت حرف *cgroup;
  bool یوځای_env;
};

LIBCRUN_عامه صحيح libcrun_close_inherited_fds (libcrun_context_t *context, libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_بارول_process_from_file (ثابت حرف *لار,
                                                   runtime_spec_schema_config_schema_process **process,
                                                   libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_container_exec_with_options (libcrun_context_t *context, ثابت حرف *id,
                                                        بنية libcrun_container_exec_options_s *opts,
                                                        libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_container_exec (libcrun_context_t *context, ثابت حرف *id,
                                           runtime_spec_schema_config_schema_process *process, libcrun_error_t *err);

بنية libcrun_exec_options_s
{
  size_t struct_اندازه;
  ثابت حرف *process_json; /* OCI Process JSON */
  ثابت حرف *cgroup;
  bool یوځای_env;
};

LIBCRUN_عامه صحيح libcrun_container_exec_json (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *process_json,
                                                libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_container_exec_with_options_json (libcrun_context_t *context, ثابت حرف *id,
                                                             ثابت بنية libcrun_exec_options_s *opts,
                                                             libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_container_exec_process_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *لار,
                                                        libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_container_update (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *content,
                                             size_t len, libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_container_update_from_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *file,
                                                       libcrun_error_t *err);

بنية libcrun_update_ارزښت_s
{
  ثابت حرف *section;
  ثابت حرف *name;
  bool numeric;
  ثابت حرف *ارزښت;
};

LIBCRUN_عامه صحيح libcrun_container_update_from_ارزښتونه (libcrun_context_t *context, ثابت حرف *id,
                                                         بنية libcrun_update_ارزښت_s *ارزښتونه, size_t len,
                                                         libcrun_error_t *err);

بنية libcrun_intel_rdt_update
{
  ثابت حرف *l3_cache_schema;
  ثابت حرف *mem_bw_schema;
  حرف *ثابت *schemata;
};

LIBCRUN_عامه صحيح libcrun_container_update_intel_rdt (libcrun_context_t *context, ثابت حرف *id,
                                                       بنية libcrun_intel_rdt_update *update, libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_container_get_features (libcrun_context_t *context, بنية features_info_s **info,
                                                   libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_container_get_features_json (libcrun_context_t *context, حرف **بهر, libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_container_pause (libcrun_context_t *context, ثابت حرف *id, libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_container_unpause (libcrun_context_t *context, ثابت حرف *id, libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_container_checkpoint (libcrun_context_t *context, ثابت حرف *id,
                                                 بنية libcrun_checkpoint_restore_options_s *cr_options,
                                                 libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_container_restore (libcrun_context_t *context, ثابت حرف *id,
                                              بنية libcrun_checkpoint_restore_options_s *cr_options,
                                              libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_container_read_pids (libcrun_context_t *context, ثابت حرف *id, bool recurse, pid_t **pids, libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_write_json_containers_list (libcrun_context_t *context, FILE *بهر, libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_container_list_json (libcrun_context_t *context, حرف **بهر, libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_container_جمع_کړه_mounts_from_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *file,
                                                           libcrun_error_t *err);

LIBCRUN_عامه صحيح libcrun_container_remove_mounts_from_file (libcrun_context_t *context, ثابت حرف *id, ثابت حرف *file,
                                                              libcrun_error_t *err);

// Not part of the public API, just a method in container.c we need to access from linux.c
فراغ get_ریښه_دننه_the_userns (runtime_spec_schema_config_schema *def, uid_t host_uid, gid_t host_gid,
                             uid_t *uid, gid_t *gid);

ساكن مضمن فراغ
cleanup_containerp (libcrun_container_t **c)
{
  libcrun_container_t *container = *c;
  libcrun_container_free (container);
}

#تعریف_کړه cleanup_container __attribute__ ((cleanup (cleanup_containerp)))

ساكن مضمن فراغ
cleanup_struct_features_free (بنية features_info_s **info)
{
  size_t i;
  بنية features_info_s *ptr;
  إذا (info == عدم || *info == عدم)
    أرجع;

  ptr = *info;

  // Free oci_version_min if it is not NULL
  إذا (ptr->oci_نسخه_min != عدم)
    {
      حرر (ptr->oci_نسخه_min);
      ptr->oci_نسخه_min = عدم; // Set to NULL after freeing
    }

  // Free oci_version_max if it is not NULL
  إذا (ptr->oci_نسخه_max != عدم)
    {
      حرر (ptr->oci_نسخه_max);
      ptr->oci_نسخه_max = عدم; // Set to NULL after freeing
    }
  إذا (ptr->hooks != عدم)
    {
      لكل (i = 0; ptr->hooks[i] != عدم; i++)
        حرر (ptr->hooks[i]);
      حرر (ptr->hooks);
    }
  إذا (ptr->mount_options != عدم)
    {
      لكل (i = 0; ptr->mount_options[i] != عدم; i++)
        حرر (ptr->mount_options[i]);
      حرر (ptr->mount_options);
    }
  إذا (ptr->linux.namespaces != عدم)
    {
      لكل (i = 0; ptr->linux.namespaces[i] != عدم; i++)
        حرر (ptr->linux.namespaces[i]);
      حرر (ptr->linux.namespaces);
    }

  إذا (ptr->linux.capabilities != عدم)
    {
      لكل (size_t i = 0; ptr->linux.capabilities[i] != عدم; i++)
        حرر (ptr->linux.capabilities[i]);
      حرر (ptr->linux.capabilities);
    }
  إذا (ptr->linux.seccomp.actions != عدم)
    {
      لكل (i = 0; ptr->linux.seccomp.actions[i] != عدم; i++)
        حرر (ptr->linux.seccomp.actions[i]);
      حرر (ptr->linux.seccomp.actions);
    }
  إذا (ptr->linux.seccomp.operators != عدم)
    {
      لكل (i = 0; ptr->linux.seccomp.operators[i] != عدم; i++)
        حرر (ptr->linux.seccomp.operators[i]);
      حرر (ptr->linux.seccomp.operators);
    }

  حرر (ptr);
  *info = عدم;
}

#تعریف_کړه cleanup_struct_features __attribute__ ((cleanup (cleanup_struct_features_free)))

#د_که_پای
