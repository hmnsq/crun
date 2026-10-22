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

#ifndef LIBCRUN_H
#define LIBCRUN_H

#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stddef.h>
#अन्तर्भाव <sys/types.h>

#ifdef __GNUC__
#  define LIBCRUN_PRINTF(a, b) __attribute__ ((format (म_लिखो, a, b)))
#  define LIBCRUN_NORETURN __attribute__ ((noreturn))
#नोचेद्यदि defined __STDC_VERSION__ && __STDC_VERSION__ >= 201112L
#  define LIBCRUN_PRINTF(a, b)
#  define LIBCRUN_NORETURN _Noreturn
#अन्यथा
#  define LIBCRUN_PRINTF(a, b)
#  define LIBCRUN_NORETURN
#endif

#ifdef __cplusplus
बाह्य "C" {
#endif

/* Opaque handles.  */
प्रकारनिर्देश संरचना libcrun_error_s *libcrun_error_t;
प्रकारनिर्देश संरचना libcrun_context_s libcrun_context_t;
प्रकारनिर्देश संरचना libcrun_container_s libcrun_container_t;
प्रकारनिर्देश संरचना libcrun_status_s libcrun_status_t;
प्रकारनिर्देश संरचना libcrun_container_list_s libcrun_container_list_t;
प्रकारनिर्देश संरचना libcrun_container_iter_s libcrun_container_iter_t;

गणना
{
  LIBCRUN_RUN_OPTIONS_PREFORK = 1 << 0,
  LIBCRUN_RUN_OPTIONS_KEEP = 1 << 1,
};

गणना
{
  LIBCRUN_CREATE_OPTIONS_PREFORK = 1 << 0,
};

गणना
{
  LIBCRUN_VERBOSITY_ERROR = 0,
  LIBCRUN_VERBOSITY_WARNING,
  LIBCRUN_VERBOSITY_DEBUG,
};

प्रकारनिर्देश गणना
{
  LIBCRUN_CONTAINER_STATUS_CREATING = 0,
  LIBCRUN_CONTAINER_STATUS_CREATED,
  LIBCRUN_CONTAINER_STATUS_RUNNING,
  LIBCRUN_CONTAINER_STATUS_STOPPED,
  LIBCRUN_CONTAINER_STATUS_PAUSED,
} libcrun_container_state_t;

प्रकारनिर्देश शून्य (*libcrun_output_handler) (पूर्णाङ्क errno_, ध्रुव वर्ण *संदेश, पूर्णाङ्क verbosity, शून्य *arg);

/* Error handling.  */
पूर्णाङ्क libcrun_error_get_status (libcrun_error_t err);
ध्रुव वर्ण *libcrun_error_get_message (libcrun_error_t err);
पूर्णाङ्क libcrun_error_release (libcrun_error_t *err);

/* Create an error object; returns a negative value derived from status.  */
पूर्णाङ्क libcrun_make_error (libcrun_error_t *err, पूर्णाङ्क status, ध्रुव वर्ण *fmt, ...) LIBCRUN_PRINTF (3, 4);

/* Report *err through the configured log handler and release it.  */
शून्य libcrun_error_report_and_release (libcrun_error_t *err);

/* Report the error and terminate the process.  */
शून्य libcrun_fail_with_error (पूर्णाङ्क errno_, ध्रुव वर्ण *fmt, ...) LIBCRUN_NORETURN LIBCRUN_PRINTF (2, 3);

/* Logging.  */
शून्य libcrun_set_verbosity (पूर्णाङ्क verbosity);
पूर्णाङ्क libcrun_get_verbosity (शून्य);
पूर्णाङ्क libcrun_set_log_format (ध्रुव वर्ण *format, libcrun_error_t *err);
शून्य libcrun_debug (ध्रुव वर्ण *fmt, ...) LIBCRUN_PRINTF (1, 2);
शून्य libcrun_warning (ध्रुव वर्ण *fmt, ...) LIBCRUN_PRINTF (1, 2);
शून्य libcrun_error (पूर्णाङ्क errno_, ध्रुव वर्ण *fmt, ...) LIBCRUN_PRINTF (2, 3);

/* Context.  */
libcrun_context_t *libcrun_context_new (ध्रुव वर्ण *id, ध्रुव वर्ण *state_मूल, libcrun_error_t *err);
शून्य libcrun_context_free (libcrun_context_t *ctx);

शून्य libcrun_context_set_id (libcrun_context_t *ctx, ध्रुव वर्ण *मूल्य);
शून्य libcrun_context_set_state_मूल (libcrun_context_t *ctx, ध्रुव वर्ण *मूल्य);
ध्रुव वर्ण *libcrun_context_get_state_मूल (libcrun_context_t *ctx);
शून्य libcrun_context_set_bundle (libcrun_context_t *ctx, ध्रुव वर्ण *मूल्य);
शून्य libcrun_context_set_console_socket (libcrun_context_t *ctx, ध्रुव वर्ण *मूल्य);
शून्य libcrun_context_set_pid_file (libcrun_context_t *ctx, ध्रुव वर्ण *मूल्य);
ध्रुव वर्ण *libcrun_context_get_pid_file (libcrun_context_t *ctx);
शून्य libcrun_context_set_notify_socket (libcrun_context_t *ctx, ध्रुव वर्ण *मूल्य);
शून्य libcrun_context_set_handler (libcrun_context_t *ctx, ध्रुव वर्ण *मूल्य);
शून्य libcrun_context_set_preserve_fds (libcrun_context_t *ctx, पूर्णाङ्क preserve_fds);
पूर्णाङ्क libcrun_context_get_preserve_fds (libcrun_context_t *ctx);
शून्य libcrun_context_set_listen_fds (libcrun_context_t *ctx, पूर्णाङ्क listen_fds);
शून्य libcrun_context_set_args (libcrun_context_t *ctx, पूर्णाङ्क argc, वर्ण **argv);
शून्य libcrun_context_set_systemd_cgroup (libcrun_context_t *ctx, bool मूल्य);
शून्य libcrun_context_set_detach (libcrun_context_t *ctx, bool मूल्य);
शून्य libcrun_context_set_no_new_keyring (libcrun_context_t *ctx, bool मूल्य);
शून्य libcrun_context_set_no_pivot (libcrun_context_t *ctx, bool मूल्य);
शून्य libcrun_context_set_force_no_cgroup (libcrun_context_t *ctx, bool मूल्य);
शून्य libcrun_context_set_output_handler (libcrun_context_t *ctx, libcrun_output_handler handler, शून्य *arg);
पूर्णाङ्क libcrun_context_init_logging (libcrun_context_t *ctx, ध्रुव वर्ण *log, libcrun_error_t *err);

/* Close the file descriptors inherited from the caller, honoring the
   preserve-fds and listen-fds settings in CTX.  */
पूर्णाङ्क libcrun_close_inherited_fds (libcrun_context_t *ctx, libcrun_error_t *err);

/* Container object (loaded from JSON, introspected via accessors).  */
libcrun_container_t *libcrun_container_load_from_file (ध्रुव वर्ण *path, libcrun_error_t *err);
libcrun_container_t *libcrun_container_load_from_memory (ध्रुव वर्ण *json, libcrun_error_t *err);
शून्य libcrun_container_free (libcrun_container_t *container);

/* Return the configuration of CONTAINER as a JSON string owned by CONTAINER.
   The document is generated on demand and cached, so the first call mutates
   CONTAINER and must not race with other users of the same handle.  */
ध्रुव वर्ण *libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err);
ध्रुव वर्ण *libcrun_container_get_annotation (libcrun_container_t *container, ध्रुव वर्ण *कुंजी);

/* Enumerate the annotations of container.  libcrun_container_get_annotation_at
   returns 0 and sets *key and *value (both owned by container) for an index
   below libcrun_container_get_annotations_len, -1 otherwise.  */
माप_प्रकार libcrun_container_get_annotations_len (libcrun_container_t *container);
पूर्णाङ्क libcrun_container_get_annotation_at (libcrun_container_t *container, माप_प्रकार index, ध्रुव वर्ण **कुंजी,
                                         ध्रुव वर्ण **मूल्य);

uid_t libcrun_container_get_uid (libcrun_container_t *container);
gid_t libcrun_container_get_gid (libcrun_container_t *container);

/* Lifecycle.  */
पूर्णाङ्क libcrun_container_run (libcrun_context_t *ctx, libcrun_container_t *container, अचिन्हित पूर्णाङ्क options,
                           libcrun_error_t *err);
पूर्णाङ्क libcrun_container_create (libcrun_context_t *ctx, libcrun_container_t *container, अचिन्हित पूर्णाङ्क options,
                              libcrun_error_t *err);
पूर्णाङ्क libcrun_container_start (libcrun_context_t *ctx, ध्रुव वर्ण *id, libcrun_error_t *err);
पूर्णाङ्क libcrun_container_delete (libcrun_context_t *ctx, ध्रुव वर्ण *id, bool force, libcrun_error_t *err);
पूर्णाङ्क libcrun_container_kill (libcrun_context_t *ctx, ध्रुव वर्ण *id, ध्रुव वर्ण *सङ्केत, libcrun_error_t *err);
पूर्णाङ्क libcrun_container_killall (libcrun_context_t *ctx, ध्रुव वर्ण *id, ध्रुव वर्ण *सङ्केत, libcrun_error_t *err);
पूर्णाङ्क libcrun_container_pause (libcrun_context_t *ctx, ध्रुव वर्ण *id, libcrun_error_t *err);
पूर्णाङ्क libcrun_container_unpause (libcrun_context_t *ctx, ध्रुव वर्ण *id, libcrun_error_t *err);
पूर्णाङ्क libcrun_container_update (libcrun_context_t *ctx, ध्रुव वर्ण *id, ध्रुव वर्ण *content, माप_प्रकार len,
                              libcrun_error_t *err);

/* JSON in, JSON out; caller frees *out with free().  */
पूर्णाङ्क libcrun_container_spec_json (bool मूल, वर्ण **out, libcrun_error_t *err);
पूर्णाङ्क libcrun_container_state_json (libcrun_context_t *ctx, ध्रुव वर्ण *id, वर्ण **out, libcrun_error_t *err);

/* Runtime features as an OCI features JSON document; caller frees *out with free().  */
पूर्णाङ्क libcrun_container_get_features_json (libcrun_context_t *ctx, वर्ण **out, libcrun_error_t *err);

संरचना libcrun_exec_options_s
{
  माप_प्रकार struct_size; /* set to sizeof (struct libcrun_exec_options_s) */
  ध्रुव वर्ण *process_json;
  ध्रुव वर्ण *cgroup;
  bool merge_env; /* merge the container environment with the one in process_json */
};

पूर्णाङ्क libcrun_container_exec_json (libcrun_context_t *ctx, ध्रुव वर्ण *id, ध्रुव वर्ण *process_json,
                                 libcrun_error_t *err);
पूर्णाङ्क libcrun_container_exec_with_options_json (libcrun_context_t *ctx, ध्रुव वर्ण *id,
                                              ध्रुव संरचना libcrun_exec_options_s *opts, libcrun_error_t *err);

/* Status.  */
पूर्णाङ्क libcrun_container_status_load (libcrun_context_t *ctx, ध्रुव वर्ण *id, libcrun_status_t **out,
                                   libcrun_error_t *err);
शून्य libcrun_container_status_free (libcrun_status_t *st);

/* Resolve the live state of the container described by st, by probing it
   (cgroup/fifo/pid).  The result is cached in st, so this costs at most one
   probe per status object.  state and running can be NULL.  */
पूर्णाङ्क libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, पूर्णाङ्क *running,
                              libcrun_error_t *err);

/* Return the OCI name of state ("creating"/"created"/"running"/"stopped"/"paused"),
   as a static string (do not free).  */
ध्रुव वर्ण *libcrun_container_state_to_string (libcrun_container_state_t state);

pid_t libcrun_status_get_pid (libcrun_status_t *st);
ध्रुव वर्ण *libcrun_status_get_bundle (libcrun_status_t *st);
ध्रुव वर्ण *libcrun_status_get_rootfs (libcrun_status_t *st);
ध्रुव वर्ण *libcrun_status_get_created (libcrun_status_t *st);
ध्रुव वर्ण *libcrun_status_get_owner (libcrun_status_t *st);
ध्रुव वर्ण *libcrun_status_get_scope (libcrun_status_t *st);
ध्रुव वर्ण *libcrun_status_get_external_descriptors (libcrun_status_t *st);

/* Live status string ("creating"/"created"/"running"/"stopped"/"paused"), obtained by
   probing the container (cgroup/fifo/pid) rather than reading the stored enum.  *out
   points to a static string (do not free).  If running is non-NULL it receives 1 when
   the container has a live process, 0 otherwise.  */
पूर्णाङ्क libcrun_container_get_state_string (libcrun_context_t *ctx, ध्रुव वर्ण *id, ध्रुव वर्ण **out, पूर्णाङ्क *running,
                                        libcrun_error_t *err);

/* Resolve the on-disk state directory for a container (id may be NULL for the state
   root itself); caller frees *out with free().  */
पूर्णाङ्क libcrun_get_state_dir (वर्ण **out, ध्रुव वर्ण *state_मूल, ध्रुव वर्ण *id, libcrun_error_t *err);

/* List containers under the state root.  Obtain a cursor with
   libcrun_container_list_iter and walk it with libcrun_container_iter_next, which
   stores a borrowed container id in *id and returns true until the list is exhausted.  */
पूर्णाङ्क libcrun_container_list (libcrun_context_t *ctx, libcrun_container_list_t **out, libcrun_error_t *err);
libcrun_container_iter_t *libcrun_container_list_iter (libcrun_container_list_t *l);
bool libcrun_container_iter_next (libcrun_container_iter_t *it, ध्रुव वर्ण **id);
शून्य libcrun_container_iter_free (libcrun_container_iter_t *it);
शून्य libcrun_container_list_free (libcrun_container_list_t *l);

/* All containers with their details as a JSON array; caller frees *out with free().  */
पूर्णाङ्क libcrun_container_list_json (libcrun_context_t *ctx, वर्ण **out, libcrun_error_t *err);

/* Update the resources of a running container from an OCI resources JSON file.  */
पूर्णाङ्क libcrun_container_update_from_file (libcrun_context_t *ctx, ध्रुव वर्ण *id, ध्रुव वर्ण *file,
                                        libcrun_error_t *err);

/* Update individual resource values.  */
संरचना libcrun_update_मूल्य_s
{
  ध्रुव वर्ण *section;
  ध्रुव वर्ण *name;
  bool numeric;
  ध्रुव वर्ण *मूल्य;
};

पूर्णाङ्क libcrun_container_update_from_मूल्यावली (libcrun_context_t *ctx, ध्रुव वर्ण *id,
                                          संरचना libcrun_update_मूल्य_s *मूल्यावली, माप_प्रकार len,
                                          libcrun_error_t *err);

/* Update the Intel RDT (L3 cache / memory bandwidth) schemata.  */
संरचना libcrun_intel_rdt_update
{
  ध्रुव वर्ण *l3_cache_schema;
  ध्रुव वर्ण *mem_bw_schema;
  वर्ण *ध्रुव *schemata;
};

पूर्णाङ्क libcrun_container_update_intel_rdt (libcrun_context_t *ctx, ध्रुव वर्ण *id,
                                        संरचना libcrun_intel_rdt_update *update, libcrun_error_t *err);

/* Add or remove mounts described by an OCI mounts JSON file.  */
पूर्णाङ्क libcrun_container_add_mounts_from_file (libcrun_context_t *ctx, ध्रुव वर्ण *id, ध्रुव वर्ण *file,
                                            libcrun_error_t *err);
पूर्णाङ्क libcrun_container_remove_mounts_from_file (libcrun_context_t *ctx, ध्रुव वर्ण *id, ध्रुव वर्ण *file,
                                               libcrun_error_t *err);

/* Enumerate the PIDs in a container; caller frees *pids with free().  */
पूर्णाङ्क libcrun_container_read_pids (libcrun_context_t *ctx, ध्रुव वर्ण *id, bool recurse, pid_t **pids,
                                 libcrun_error_t *err);

/* Checkpoint and restore (CRIU).  */
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
  माप_प्रकार struct_size; /* set to sizeof (struct libcrun_checkpoint_restore_options_s) */

  ध्रुव वर्ण *image_path;
  ध्रुव वर्ण *work_path;
  ध्रुव वर्ण *parent_path;
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

  पूर्णाङ्क manage_cgroups_mode; /* enum libcrun_cr_cgroups_mode */
  पूर्णाङ्क network_lock_method; /* enum libcrun_cr_network_lock_method */
};

पूर्णाङ्क libcrun_container_checkpoint (libcrun_context_t *ctx, ध्रुव वर्ण *id,
                                  संरचना libcrun_checkpoint_restore_options_s *opts, libcrun_error_t *err);
पूर्णाङ्क libcrun_container_restore (libcrun_context_t *ctx, ध्रुव वर्ण *id,
                               संरचना libcrun_checkpoint_restore_options_s *opts, libcrun_error_t *err);

/* Feature tags contributed by the loaded handlers, as a NULL-terminated array of
   strings (e.g. "wasm:wasmedge").  Returns NULL if there are none.  Caller frees the
   array and each string with free().  */
वर्ण **libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err);

#ifdef __cplusplus
}
#endif

#endif
