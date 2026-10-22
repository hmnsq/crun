/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019, 2026 Giuseppe Scrivano <giuseppe@scrivano.org>
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

/*
 * Public API for libcrun.  This is the only header external consumers should
 * include:
 *
 *   #include <libcrun.h>
 *
 * All handles are opaque; OCI data crosses the boundary as JSON.  Functions
 * return int (0/positive on success, negative on error) and take a trailing
 * libcrun_error_t *err; loaders return a handle or NULL.  On error *err is set
 * and the caller releases it with libcrun_error_release.
 */

#که_نه_تعریف_شوی LIBCRUN_H
#تعریف_کړه LIBCRUN_H

#ضمن <stdbool.h>
#ضمن <stddef.h>
#ضمن <sys/types.h>

#که_تعریف_شوی __GNUC__
#  define LIBCRUN_PRINTF(a, b) __attribute__ ((format (چاپ_کړه, a, b)))
#  define LIBCRUN_NORETURN __attribute__ ((noreturn))
#وإلاإذا defined __STDC_VERSION__ && __STDC_VERSION__ >= 201112L
#  define LIBCRUN_PRINTF(a, b)
#  define LIBCRUN_NORETURN _Noreturn
#وإلا
#  define LIBCRUN_PRINTF(a, b)
#  define LIBCRUN_NORETURN
#د_که_پای

#که_تعریف_شوی __cplusplus
خارجي "C" {
#د_که_پای

/* Opaque handles.  */
تعريفنوع بنية libcrun_error_s *libcrun_error_t;
تعريفنوع بنية libcrun_context_s libcrun_context_t;
تعريفنوع بنية libcrun_container_s libcrun_container_t;
تعريفنوع بنية libcrun_حالت_s libcrun_حالت_t;
تعريفنوع بنية libcrun_container_list_s libcrun_container_list_t;
تعريفنوع بنية libcrun_container_iter_s libcrun_container_iter_t;

تعداد
{
  LIBCRUN_پرلپسې_OPTIONS_PREFORK = 1 << 0,
  LIBCRUN_پرلپسې_OPTIONS_KEEP = 1 << 1,
};

تعداد
{
  LIBCRUN_CREATE_OPTIONS_PREFORK = 1 << 0,
};

تعداد
{
  LIBCRUN_VERBOSITY_ERROR = 0,
  LIBCRUN_VERBOSITY_WARNING,
  LIBCRUN_VERBOSITY_DEBUG,
};

تعريفنوع تعداد
{
  LIBCRUN_CONTAINER_حالت_CREATING = 0,
  LIBCRUN_CONTAINER_حالت_CREATED,
  LIBCRUN_CONTAINER_حالت_RUNNING,
  LIBCRUN_CONTAINER_حالت_STOPPED,
  LIBCRUN_CONTAINER_حالت_PAUSED,
} libcrun_container_state_t;

تعريفنوع فراغ (*libcrun_output_handler) (صحيح errno_, ثابت حرف *پیغام, صحيح verbosity, فراغ *arg);

/* Error handling.  */
صحيح libcrun_error_get_حالت (libcrun_error_t err);
ثابت حرف *libcrun_error_get_پیغام (libcrun_error_t err);
صحيح libcrun_error_release (libcrun_error_t *err);

/* Create an error object; returns a negative value derived from status.  */
صحيح libcrun_make_error (libcrun_error_t *err, صحيح حالت, ثابت حرف *fmt, ...) LIBCRUN_PRINTF (3, 4);

/* Report *err through the configured log handler and release it.  */
فراغ libcrun_error_report_and_release (libcrun_error_t *err);

/* Report the error and terminate the process.  */
فراغ libcrun_fail_with_error (صحيح errno_, ثابت حرف *fmt, ...) LIBCRUN_NORETURN LIBCRUN_PRINTF (2, 3);

/* Logging.  */
فراغ libcrun_سېټ_verbosity (صحيح verbosity);
صحيح libcrun_get_verbosity (فراغ);
صحيح libcrun_سېټ_log_format (ثابت حرف *format, libcrun_error_t *err);
فراغ libcrun_debug (ثابت حرف *fmt, ...) LIBCRUN_PRINTF (1, 2);
فراغ libcrun_warning (ثابت حرف *fmt, ...) LIBCRUN_PRINTF (1, 2);
فراغ libcrun_error (صحيح errno_, ثابت حرف *fmt, ...) LIBCRUN_PRINTF (2, 3);

/* Context.  */
libcrun_context_t *libcrun_context_new (ثابت حرف *id, ثابت حرف *state_ریښه, libcrun_error_t *err);
فراغ libcrun_context_free (libcrun_context_t *ctx);

فراغ libcrun_context_سېټ_id (libcrun_context_t *ctx, ثابت حرف *ارزښت);
فراغ libcrun_context_سېټ_state_ریښه (libcrun_context_t *ctx, ثابت حرف *ارزښت);
ثابت حرف *libcrun_context_get_state_ریښه (libcrun_context_t *ctx);
فراغ libcrun_context_سېټ_bundle (libcrun_context_t *ctx, ثابت حرف *ارزښت);
فراغ libcrun_context_سېټ_console_socket (libcrun_context_t *ctx, ثابت حرف *ارزښت);
فراغ libcrun_context_سېټ_pid_file (libcrun_context_t *ctx, ثابت حرف *ارزښت);
ثابت حرف *libcrun_context_get_pid_file (libcrun_context_t *ctx);
فراغ libcrun_context_سېټ_notify_socket (libcrun_context_t *ctx, ثابت حرف *ارزښت);
فراغ libcrun_context_سېټ_handler (libcrun_context_t *ctx, ثابت حرف *ارزښت);
فراغ libcrun_context_سېټ_preserve_fds (libcrun_context_t *ctx, صحيح preserve_fds);
صحيح libcrun_context_get_preserve_fds (libcrun_context_t *ctx);
فراغ libcrun_context_سېټ_listen_fds (libcrun_context_t *ctx, صحيح listen_fds);
فراغ libcrun_context_سېټ_دلیلونه (libcrun_context_t *ctx, صحيح argc, حرف **argv);
فراغ libcrun_context_سېټ_systemd_cgroup (libcrun_context_t *ctx, bool ارزښت);
فراغ libcrun_context_سېټ_detach (libcrun_context_t *ctx, bool ارزښت);
فراغ libcrun_context_سېټ_no_new_keyring (libcrun_context_t *ctx, bool ارزښت);
فراغ libcrun_context_سېټ_no_محور (libcrun_context_t *ctx, bool ارزښت);
فراغ libcrun_context_سېټ_force_no_cgroup (libcrun_context_t *ctx, bool ارزښت);
فراغ libcrun_context_سېټ_output_handler (libcrun_context_t *ctx, libcrun_output_handler handler, فراغ *arg);
صحيح libcrun_context_init_logging (libcrun_context_t *ctx, ثابت حرف *لوګاریتم, libcrun_error_t *err);

/* Close the file descriptors inherited from the caller, honoring the
   preserve-fds and listen-fds settings in CTX.  */
صحيح libcrun_close_inherited_fds (libcrun_context_t *ctx, libcrun_error_t *err);

/* Container object (loaded from JSON, introspected via accessors).  */
libcrun_container_t *libcrun_container_بارول_from_file (ثابت حرف *لار, libcrun_error_t *err);
libcrun_container_t *libcrun_container_بارول_from_حافظه (ثابت حرف *json, libcrun_error_t *err);
فراغ libcrun_container_free (libcrun_container_t *container);

/* Return the configuration of CONTAINER as a JSON string owned by CONTAINER.
   The document is generated on demand and cached, so the first call mutates
   CONTAINER and must not race with other users of the same handle.  */
ثابت حرف *libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err);
ثابت حرف *libcrun_container_get_annotation (libcrun_container_t *container, ثابت حرف *کلی);

/* Enumerate the annotations of container.  libcrun_container_get_annotation_at
   returns 0 and sets *key and *value (both owned by container) for an index
   below libcrun_container_get_annotations_len, -1 otherwise.  */
size_t libcrun_container_get_annotations_len (libcrun_container_t *container);
صحيح libcrun_container_get_annotation_at (libcrun_container_t *container, size_t شاخص, ثابت حرف **کلی,
                                         ثابت حرف **ارزښت);

uid_t libcrun_container_get_uid (libcrun_container_t *container);
gid_t libcrun_container_get_gid (libcrun_container_t *container);

/* Lifecycle.  */
صحيح libcrun_container_پرلپسې (libcrun_context_t *ctx, libcrun_container_t *container, بینښه صحيح options,
                           libcrun_error_t *err);
صحيح libcrun_container_create (libcrun_context_t *ctx, libcrun_container_t *container, بینښه صحيح options,
                              libcrun_error_t *err);
صحيح libcrun_container_پیل (libcrun_context_t *ctx, ثابت حرف *id, libcrun_error_t *err);
صحيح libcrun_container_delete (libcrun_context_t *ctx, ثابت حرف *id, bool force, libcrun_error_t *err);
صحيح libcrun_container_kill (libcrun_context_t *ctx, ثابت حرف *id, ثابت حرف *إشارة, libcrun_error_t *err);
صحيح libcrun_container_killall (libcrun_context_t *ctx, ثابت حرف *id, ثابت حرف *إشارة, libcrun_error_t *err);
صحيح libcrun_container_pause (libcrun_context_t *ctx, ثابت حرف *id, libcrun_error_t *err);
صحيح libcrun_container_unpause (libcrun_context_t *ctx, ثابت حرف *id, libcrun_error_t *err);
صحيح libcrun_container_update (libcrun_context_t *ctx, ثابت حرف *id, ثابت حرف *content, size_t len,
                              libcrun_error_t *err);

/* JSON in, JSON out; caller frees *out with free().  */
صحيح libcrun_container_spec_json (bool ریښه, حرف **بهر, libcrun_error_t *err);
صحيح libcrun_container_state_json (libcrun_context_t *ctx, ثابت حرف *id, حرف **بهر, libcrun_error_t *err);

/* Runtime features as an OCI features JSON document; caller frees *out with free().  */
صحيح libcrun_container_get_features_json (libcrun_context_t *ctx, حرف **بهر, libcrun_error_t *err);

بنية libcrun_exec_options_s
{
  size_t struct_اندازه; /* set to sizeof (struct libcrun_exec_options_s) */
  ثابت حرف *process_json;
  ثابت حرف *cgroup;
  bool یوځای_env; /* merge the container environment with the one in process_json */
};

صحيح libcrun_container_exec_json (libcrun_context_t *ctx, ثابت حرف *id, ثابت حرف *process_json,
                                 libcrun_error_t *err);
صحيح libcrun_container_exec_with_options_json (libcrun_context_t *ctx, ثابت حرف *id,
                                              ثابت بنية libcrun_exec_options_s *opts, libcrun_error_t *err);

/* Status.  */
صحيح libcrun_container_حالت_بارول (libcrun_context_t *ctx, ثابت حرف *id, libcrun_حالت_t **بهر,
                                   libcrun_error_t *err);
فراغ libcrun_container_حالت_free (libcrun_حالت_t *st);

/* Resolve the live state of the container described by st, by probing it
   (cgroup/fifo/pid).  The result is cached in st, so this costs at most one
   probe per status object.  state and running can be NULL.  */
صحيح libcrun_حالت_get_state (libcrun_حالت_t *st, libcrun_container_state_t *state, صحيح *running,
                              libcrun_error_t *err);

/* Return the OCI name of state ("creating"/"created"/"running"/"stopped"/"paused"),
   as a static string (do not free).  */
ثابت حرف *libcrun_container_state_منزل_string (libcrun_container_state_t state);

pid_t libcrun_حالت_get_pid (libcrun_حالت_t *st);
ثابت حرف *libcrun_حالت_get_bundle (libcrun_حالت_t *st);
ثابت حرف *libcrun_حالت_get_rootfs (libcrun_حالت_t *st);
ثابت حرف *libcrun_حالت_get_created (libcrun_حالت_t *st);
ثابت حرف *libcrun_حالت_get_owner (libcrun_حالت_t *st);
ثابت حرف *libcrun_حالت_get_scope (libcrun_حالت_t *st);
ثابت حرف *libcrun_حالت_get_external_descriptors (libcrun_حالت_t *st);

/* Live status string ("creating"/"created"/"running"/"stopped"/"paused"), obtained by
   probing the container (cgroup/fifo/pid) rather than reading the stored enum.  *out
   points to a static string (do not free).  If running is non-NULL it receives 1 when
   the container has a live process, 0 otherwise.  */
صحيح libcrun_container_get_state_string (libcrun_context_t *ctx, ثابت حرف *id, ثابت حرف **بهر, صحيح *running,
                                        libcrun_error_t *err);

/* Resolve the on-disk state directory for a container (id may be NULL for the state
   root itself); caller frees *out with free().  */
صحيح libcrun_get_state_dir (حرف **بهر, ثابت حرف *state_ریښه, ثابت حرف *id, libcrun_error_t *err);

/* List containers under the state root.  Obtain a cursor with
   libcrun_container_list_iter and walk it with libcrun_container_iter_next, which
   stores a borrowed container id in *id and returns true until the list is exhausted.  */
صحيح libcrun_container_list (libcrun_context_t *ctx, libcrun_container_list_t **بهر, libcrun_error_t *err);
libcrun_container_iter_t *libcrun_container_list_iter (libcrun_container_list_t *l);
bool libcrun_container_iter_بل (libcrun_container_iter_t *it, ثابت حرف **id);
فراغ libcrun_container_iter_free (libcrun_container_iter_t *it);
فراغ libcrun_container_list_free (libcrun_container_list_t *l);

/* All containers with their details as a JSON array; caller frees *out with free().  */
صحيح libcrun_container_list_json (libcrun_context_t *ctx, حرف **بهر, libcrun_error_t *err);

/* Update the resources of a running container from an OCI resources JSON file.  */
صحيح libcrun_container_update_from_file (libcrun_context_t *ctx, ثابت حرف *id, ثابت حرف *file,
                                        libcrun_error_t *err);

/* Update individual resource values.  */
بنية libcrun_update_ارزښت_s
{
  ثابت حرف *section;
  ثابت حرف *name;
  bool numeric;
  ثابت حرف *ارزښت;
};

صحيح libcrun_container_update_from_ارزښتونه (libcrun_context_t *ctx, ثابت حرف *id,
                                          بنية libcrun_update_ارزښت_s *ارزښتونه, size_t len,
                                          libcrun_error_t *err);

/* Update the Intel RDT (L3 cache / memory bandwidth) schemata.  */
بنية libcrun_intel_rdt_update
{
  ثابت حرف *l3_cache_schema;
  ثابت حرف *mem_bw_schema;
  حرف *ثابت *schemata;
};

صحيح libcrun_container_update_intel_rdt (libcrun_context_t *ctx, ثابت حرف *id,
                                        بنية libcrun_intel_rdt_update *update, libcrun_error_t *err);

/* Add or remove mounts described by an OCI mounts JSON file.  */
صحيح libcrun_container_جمع_کړه_mounts_from_file (libcrun_context_t *ctx, ثابت حرف *id, ثابت حرف *file,
                                            libcrun_error_t *err);
صحيح libcrun_container_remove_mounts_from_file (libcrun_context_t *ctx, ثابت حرف *id, ثابت حرف *file,
                                               libcrun_error_t *err);

/* Enumerate the PIDs in a container; caller frees *pids with free().  */
صحيح libcrun_container_read_pids (libcrun_context_t *ctx, ثابت حرف *id, bool recurse, pid_t **pids,
                                 libcrun_error_t *err);

/* Checkpoint and restore (CRIU).  */
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
  size_t struct_اندازه; /* set to sizeof (struct libcrun_checkpoint_restore_options_s) */

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

صحيح libcrun_container_checkpoint (libcrun_context_t *ctx, ثابت حرف *id,
                                  بنية libcrun_checkpoint_restore_options_s *opts, libcrun_error_t *err);
صحيح libcrun_container_restore (libcrun_context_t *ctx, ثابت حرف *id,
                               بنية libcrun_checkpoint_restore_options_s *opts, libcrun_error_t *err);

/* Feature tags contributed by the loaded handlers, as a NULL-terminated array of
   strings (e.g. "wasm:wasmedge").  Returns NULL if there are none.  Caller frees the
   array and each string with free().  */
حرف **libcrun_context_get_handler_feature_نښانونه (libcrun_context_t *ctx, libcrun_error_t *err);

#که_تعریف_شوی __cplusplus
}
#د_که_پای

#د_که_پای
