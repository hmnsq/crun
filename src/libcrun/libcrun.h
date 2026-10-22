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

#അന്തര്ഭാവ <stdbool.h>
#അന്തര്ഭാവ <stddef.h>
#അന്തര്ഭാവ <sys/types.h>

#ifdef __GNUC__
#  define LIBCRUN_PRINTF(a, b) __attribute__ ((format (printf, a, b)))
#  define LIBCRUN_NORETURN __attribute__ ((noreturn))
#അല്ലെങ്കിലെങ്കിൽ defined __STDC_VERSION__ && __STDC_VERSION__ >= 201112L
#  define LIBCRUN_PRINTF(a, b)
#  define LIBCRUN_NORETURN _Noreturn
#അല്ലെങ്കിൽ
#  define LIBCRUN_PRINTF(a, b)
#  define LIBCRUN_NORETURN
#endif

#ifdef __cplusplus
ബാഹ്യ "C" {
#endif

/* Opaque handles.  */
തരനിർവചനം ഘടന libcrun_error_s *libcrun_error_t;
തരനിർവചനം ഘടന libcrun_context_s libcrun_context_t;
തരനിർവചനം ഘടന libcrun_container_s libcrun_container_t;
തരനിർവചനം ഘടന libcrun_status_s libcrun_status_t;
തരനിർവചനം ഘടന libcrun_container_list_s libcrun_container_list_t;
തരനിർവചനം ഘടന libcrun_container_iter_s libcrun_container_iter_t;

എണ്ണൽ
{
  LIBCRUN_RUN_OPTIONS_PREFORK = 1 << 0,
  LIBCRUN_RUN_OPTIONS_KEEP = 1 << 1,
};

എണ്ണൽ
{
  LIBCRUN_CREATE_OPTIONS_PREFORK = 1 << 0,
};

എണ്ണൽ
{
  LIBCRUN_VERBOSITY_ERROR = 0,
  LIBCRUN_VERBOSITY_WARNING,
  LIBCRUN_VERBOSITY_DEBUG,
};

തരനിർവചനം എണ്ണൽ
{
  LIBCRUN_CONTAINER_STATUS_CREATING = 0,
  LIBCRUN_CONTAINER_STATUS_CREATED,
  LIBCRUN_CONTAINER_STATUS_RUNNING,
  LIBCRUN_CONTAINER_STATUS_STOPPED,
  LIBCRUN_CONTAINER_STATUS_PAUSED,
} libcrun_container_state_t;

തരനിർവചനം ശൂന്യം (*libcrun_output_handler) (പൂർണ്ണസംഖ്യ errno_, സ്ഥിരം അക്ഷരം *സന്ദേശം, പൂർണ്ണസംഖ്യ verbosity, ശൂന്യം *arg);

/* Error handling.  */
പൂർണ്ണസംഖ്യ libcrun_error_get_status (libcrun_error_t err);
സ്ഥിരം അക്ഷരം *libcrun_error_get_message (libcrun_error_t err);
പൂർണ്ണസംഖ്യ libcrun_error_release (libcrun_error_t *err);

/* Create an error object; returns a negative value derived from status.  */
പൂർണ്ണസംഖ്യ libcrun_make_error (libcrun_error_t *err, പൂർണ്ണസംഖ്യ status, സ്ഥിരം അക്ഷരം *fmt, ...) LIBCRUN_PRINTF (3, 4);

/* Report *err through the configured log handler and release it.  */
ശൂന്യം libcrun_error_report_and_release (libcrun_error_t *err);

/* Report the error and terminate the process.  */
ശൂന്യം libcrun_fail_with_error (പൂർണ്ണസംഖ്യ errno_, സ്ഥിരം അക്ഷരം *fmt, ...) LIBCRUN_NORETURN LIBCRUN_PRINTF (2, 3);

/* Logging.  */
ശൂന്യം libcrun_set_verbosity (പൂർണ്ണസംഖ്യ verbosity);
പൂർണ്ണസംഖ്യ libcrun_get_verbosity (ശൂന്യം);
പൂർണ്ണസംഖ്യ libcrun_set_log_format (സ്ഥിരം അക്ഷരം *format, libcrun_error_t *err);
ശൂന്യം libcrun_debug (സ്ഥിരം അക്ഷരം *fmt, ...) LIBCRUN_PRINTF (1, 2);
ശൂന്യം libcrun_warning (സ്ഥിരം അക്ഷരം *fmt, ...) LIBCRUN_PRINTF (1, 2);
ശൂന്യം libcrun_error (പൂർണ്ണസംഖ്യ errno_, സ്ഥിരം അക്ഷരം *fmt, ...) LIBCRUN_PRINTF (2, 3);

/* Context.  */
libcrun_context_t *libcrun_context_new (സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *state_വേര്, libcrun_error_t *err);
ശൂന്യം libcrun_context_free (libcrun_context_t *ctx);

ശൂന്യം libcrun_context_set_id (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *മൂല്യം);
ശൂന്യം libcrun_context_set_state_വേര് (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *മൂല്യം);
സ്ഥിരം അക്ഷരം *libcrun_context_get_state_വേര് (libcrun_context_t *ctx);
ശൂന്യം libcrun_context_set_bundle (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *മൂല്യം);
ശൂന്യം libcrun_context_set_console_socket (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *മൂല്യം);
ശൂന്യം libcrun_context_set_pid_file (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *മൂല്യം);
സ്ഥിരം അക്ഷരം *libcrun_context_get_pid_file (libcrun_context_t *ctx);
ശൂന്യം libcrun_context_set_notify_socket (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *മൂല്യം);
ശൂന്യം libcrun_context_set_handler (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *മൂല്യം);
ശൂന്യം libcrun_context_set_preserve_fds (libcrun_context_t *ctx, പൂർണ്ണസംഖ്യ preserve_fds);
പൂർണ്ണസംഖ്യ libcrun_context_get_preserve_fds (libcrun_context_t *ctx);
ശൂന്യം libcrun_context_set_listen_fds (libcrun_context_t *ctx, പൂർണ്ണസംഖ്യ listen_fds);
ശൂന്യം libcrun_context_set_args (libcrun_context_t *ctx, പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv);
ശൂന്യം libcrun_context_set_systemd_cgroup (libcrun_context_t *ctx, bool മൂല്യം);
ശൂന്യം libcrun_context_set_detach (libcrun_context_t *ctx, bool മൂല്യം);
ശൂന്യം libcrun_context_set_no_new_keyring (libcrun_context_t *ctx, bool മൂല്യം);
ശൂന്യം libcrun_context_set_no_pivot (libcrun_context_t *ctx, bool മൂല്യം);
ശൂന്യം libcrun_context_set_force_no_cgroup (libcrun_context_t *ctx, bool മൂല്യം);
ശൂന്യം libcrun_context_set_output_handler (libcrun_context_t *ctx, libcrun_output_handler handler, ശൂന്യം *arg);
പൂർണ്ണസംഖ്യ libcrun_context_init_logging (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *log, libcrun_error_t *err);

/* Close the file descriptors inherited from the caller, honoring the
   preserve-fds and listen-fds settings in CTX.  */
പൂർണ്ണസംഖ്യ libcrun_close_inherited_fds (libcrun_context_t *ctx, libcrun_error_t *err);

/* Container object (loaded from JSON, introspected via accessors).  */
libcrun_container_t *libcrun_container_load_from_file (സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err);
libcrun_container_t *libcrun_container_load_from_memory (സ്ഥിരം അക്ഷരം *json, libcrun_error_t *err);
ശൂന്യം libcrun_container_free (libcrun_container_t *container);

/* Return the configuration of CONTAINER as a JSON string owned by CONTAINER.
   The document is generated on demand and cached, so the first call mutates
   CONTAINER and must not race with other users of the same handle.  */
സ്ഥിരം അക്ഷരം *libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err);
സ്ഥിരം അക്ഷരം *libcrun_container_get_annotation (libcrun_container_t *container, സ്ഥിരം അക്ഷരം *താക്കോൽ);

/* Enumerate the annotations of container.  libcrun_container_get_annotation_at
   returns 0 and sets *key and *value (both owned by container) for an index
   below libcrun_container_get_annotations_len, -1 otherwise.  */
size_t libcrun_container_get_annotations_len (libcrun_container_t *container);
പൂർണ്ണസംഖ്യ libcrun_container_get_annotation_at (libcrun_container_t *container, size_t index, സ്ഥിരം അക്ഷരം **താക്കോൽ,
                                         സ്ഥിരം അക്ഷരം **മൂല്യം);

uid_t libcrun_container_get_uid (libcrun_container_t *container);
gid_t libcrun_container_get_gid (libcrun_container_t *container);

/* Lifecycle.  */
പൂർണ്ണസംഖ്യ libcrun_container_run (libcrun_context_t *ctx, libcrun_container_t *container, unsigned പൂർണ്ണസംഖ്യ options,
                           libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_container_create (libcrun_context_t *ctx, libcrun_container_t *container, unsigned പൂർണ്ണസംഖ്യ options,
                              libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_container_start (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_container_delete (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *id, bool force, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_container_kill (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *സൂചന, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_container_killall (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *സൂചന, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_container_pause (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_container_unpause (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_container_update (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *content, size_t len,
                              libcrun_error_t *err);

/* JSON in, JSON out; caller frees *out with free().  */
പൂർണ്ണസംഖ്യ libcrun_container_spec_json (bool വേര്, അക്ഷരം **out, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_container_state_json (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *id, അക്ഷരം **out, libcrun_error_t *err);

/* Runtime features as an OCI features JSON document; caller frees *out with free().  */
പൂർണ്ണസംഖ്യ libcrun_container_get_features_json (libcrun_context_t *ctx, അക്ഷരം **out, libcrun_error_t *err);

ഘടന libcrun_exec_options_s
{
  size_t struct_size; /* set to sizeof (struct libcrun_exec_options_s) */
  സ്ഥിരം അക്ഷരം *process_json;
  സ്ഥിരം അക്ഷരം *cgroup;
  bool merge_env; /* merge the container environment with the one in process_json */
};

പൂർണ്ണസംഖ്യ libcrun_container_exec_json (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *process_json,
                                 libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_container_exec_with_options_json (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *id,
                                              സ്ഥിരം ഘടന libcrun_exec_options_s *opts, libcrun_error_t *err);

/* Status.  */
പൂർണ്ണസംഖ്യ libcrun_container_status_load (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *id, libcrun_status_t **out,
                                   libcrun_error_t *err);
ശൂന്യം libcrun_container_status_free (libcrun_status_t *st);

/* Resolve the live state of the container described by st, by probing it
   (cgroup/fifo/pid).  The result is cached in st, so this costs at most one
   probe per status object.  state and running can be NULL.  */
പൂർണ്ണസംഖ്യ libcrun_status_get_state (libcrun_status_t *st, libcrun_container_state_t *state, പൂർണ്ണസംഖ്യ *running,
                              libcrun_error_t *err);

/* Return the OCI name of state ("creating"/"created"/"running"/"stopped"/"paused"),
   as a static string (do not free).  */
സ്ഥിരം അക്ഷരം *libcrun_container_state_to_string (libcrun_container_state_t state);

pid_t libcrun_status_get_pid (libcrun_status_t *st);
സ്ഥിരം അക്ഷരം *libcrun_status_get_bundle (libcrun_status_t *st);
സ്ഥിരം അക്ഷരം *libcrun_status_get_rootfs (libcrun_status_t *st);
സ്ഥിരം അക്ഷരം *libcrun_status_get_created (libcrun_status_t *st);
സ്ഥിരം അക്ഷരം *libcrun_status_get_owner (libcrun_status_t *st);
സ്ഥിരം അക്ഷരം *libcrun_status_get_scope (libcrun_status_t *st);
സ്ഥിരം അക്ഷരം *libcrun_status_get_external_descriptors (libcrun_status_t *st);

/* Live status string ("creating"/"created"/"running"/"stopped"/"paused"), obtained by
   probing the container (cgroup/fifo/pid) rather than reading the stored enum.  *out
   points to a static string (do not free).  If running is non-NULL it receives 1 when
   the container has a live process, 0 otherwise.  */
പൂർണ്ണസംഖ്യ libcrun_container_get_state_string (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം **out, പൂർണ്ണസംഖ്യ *running,
                                        libcrun_error_t *err);

/* Resolve the on-disk state directory for a container (id may be NULL for the state
   root itself); caller frees *out with free().  */
പൂർണ്ണസംഖ്യ libcrun_get_state_dir (അക്ഷരം **out, സ്ഥിരം അക്ഷരം *state_വേര്, സ്ഥിരം അക്ഷരം *id, libcrun_error_t *err);

/* List containers under the state root.  Obtain a cursor with
   libcrun_container_list_iter and walk it with libcrun_container_iter_next, which
   stores a borrowed container id in *id and returns true until the list is exhausted.  */
പൂർണ്ണസംഖ്യ libcrun_container_list (libcrun_context_t *ctx, libcrun_container_list_t **out, libcrun_error_t *err);
libcrun_container_iter_t *libcrun_container_list_iter (libcrun_container_list_t *l);
bool libcrun_container_iter_next (libcrun_container_iter_t *it, സ്ഥിരം അക്ഷരം **id);
ശൂന്യം libcrun_container_iter_free (libcrun_container_iter_t *it);
ശൂന്യം libcrun_container_list_free (libcrun_container_list_t *l);

/* All containers with their details as a JSON array; caller frees *out with free().  */
പൂർണ്ണസംഖ്യ libcrun_container_list_json (libcrun_context_t *ctx, അക്ഷരം **out, libcrun_error_t *err);

/* Update the resources of a running container from an OCI resources JSON file.  */
പൂർണ്ണസംഖ്യ libcrun_container_update_from_file (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *file,
                                        libcrun_error_t *err);

/* Update individual resource values.  */
ഘടന libcrun_update_മൂല്യം_s
{
  സ്ഥിരം അക്ഷരം *section;
  സ്ഥിരം അക്ഷരം *name;
  bool numeric;
  സ്ഥിരം അക്ഷരം *മൂല്യം;
};

പൂർണ്ണസംഖ്യ libcrun_container_update_from_മൂല്യങ്ങൾ (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *id,
                                          ഘടന libcrun_update_മൂല്യം_s *മൂല്യങ്ങൾ, size_t len,
                                          libcrun_error_t *err);

/* Update the Intel RDT (L3 cache / memory bandwidth) schemata.  */
ഘടന libcrun_intel_rdt_update
{
  സ്ഥിരം അക്ഷരം *l3_cache_schema;
  സ്ഥിരം അക്ഷരം *mem_bw_schema;
  അക്ഷരം *സ്ഥിരം *schemata;
};

പൂർണ്ണസംഖ്യ libcrun_container_update_intel_rdt (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *id,
                                        ഘടന libcrun_intel_rdt_update *update, libcrun_error_t *err);

/* Add or remove mounts described by an OCI mounts JSON file.  */
പൂർണ്ണസംഖ്യ libcrun_container_add_mounts_from_file (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *file,
                                            libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_container_remove_mounts_from_file (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *id, സ്ഥിരം അക്ഷരം *file,
                                               libcrun_error_t *err);

/* Enumerate the PIDs in a container; caller frees *pids with free().  */
പൂർണ്ണസംഖ്യ libcrun_container_read_pids (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *id, bool recurse, pid_t **pids,
                                 libcrun_error_t *err);

/* Checkpoint and restore (CRIU).  */
എണ്ണൽ libcrun_cr_cgroups_mode
{
  LIBCRUN_CR_CG_MODE_DEFAULT = 0, /* soft, matching runc */
  LIBCRUN_CR_CG_MODE_SOFT,
  LIBCRUN_CR_CG_MODE_IGNORE,
  LIBCRUN_CR_CG_MODE_FULL,
  LIBCRUN_CR_CG_MODE_STRICT,
};

എണ്ണൽ libcrun_cr_network_lock_method
{
  LIBCRUN_CR_NETWORK_LOCK_DEFAULT = 0, /* iptables */
  LIBCRUN_CR_NETWORK_LOCK_IPTABLES,
  LIBCRUN_CR_NETWORK_LOCK_NFTABLES,
  LIBCRUN_CR_NETWORK_LOCK_SKIP,
};

ഘടന libcrun_checkpoint_restore_options_s
{
  size_t struct_size; /* set to sizeof (struct libcrun_checkpoint_restore_options_s) */

  സ്ഥിരം അക്ഷരം *image_path;
  സ്ഥിരം അക്ഷരം *work_path;
  സ്ഥിരം അക്ഷരം *parent_path;
  സ്ഥിരം അക്ഷരം *console_socket;
  സ്ഥിരം അക്ഷരം *lsm_profile;
  സ്ഥിരം അക്ഷരം *lsm_mount_context;

  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool ext_unix_sk;
  bool shell_job;
  bool file_locks;
  bool pre_dump;
  bool detach;

  പൂർണ്ണസംഖ്യ manage_cgroups_mode; /* enum libcrun_cr_cgroups_mode */
  പൂർണ്ണസംഖ്യ network_lock_method; /* enum libcrun_cr_network_lock_method */
};

പൂർണ്ണസംഖ്യ libcrun_container_checkpoint (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *id,
                                  ഘടന libcrun_checkpoint_restore_options_s *opts, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_container_restore (libcrun_context_t *ctx, സ്ഥിരം അക്ഷരം *id,
                               ഘടന libcrun_checkpoint_restore_options_s *opts, libcrun_error_t *err);

/* Feature tags contributed by the loaded handlers, as a NULL-terminated array of
   strings (e.g. "wasm:wasmedge").  Returns NULL if there are none.  Caller frees the
   array and each string with free().  */
അക്ഷരം **libcrun_context_get_handler_feature_tags (libcrun_context_t *ctx, libcrun_error_t *err);

#ifdef __cplusplus
}
#endif

#endif
