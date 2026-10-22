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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LIBCRUN_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LIBCRUN_H

#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stddef.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __GNUC__
#  define LIBCRUN_PRINTF(a, b) __attribute__ ((format (ਲਿਖੋ, a, b)))
#  define LIBCRUN_NORETURN __attribute__ ((noreturn))
#ਜਾਂਜੇ defined __STDC_VERSION__ && __STDC_VERSION__ >= 201112L
#  define LIBCRUN_PRINTF(a, b)
#  define LIBCRUN_NORETURN _Noreturn
#ਨਹੀਂਤਾਂ
#  define LIBCRUN_PRINTF(a, b)
#  define LIBCRUN_NORETURN
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ

/* Opaque handles.  */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ libcrun_error_s *libcrun_error_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ libcrun_context_s libcrun_context_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ libcrun_container_s libcrun_container_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ libcrun_ਸਥਿਤੀ_s libcrun_ਸਥਿਤੀ_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ libcrun_container_list_s libcrun_container_list_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ libcrun_container_iter_s libcrun_container_iter_t;

ਗਿਣਤੀ
{
  LIBCRUN_ਲੜੀਵਾਰ_OPTIONS_PREFORK = 1 << 0,
  LIBCRUN_ਲੜੀਵਾਰ_OPTIONS_KEEP = 1 << 1,
};

ਗਿਣਤੀ
{
  LIBCRUN_CREATE_OPTIONS_PREFORK = 1 << 0,
};

ਗਿਣਤੀ
{
  LIBCRUN_VERBOSITY_ERROR = 0,
  LIBCRUN_VERBOSITY_WARNING,
  LIBCRUN_VERBOSITY_DEBUG,
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ
{
  LIBCRUN_CONTAINER_ਸਥਿਤੀ_CREATING = 0,
  LIBCRUN_CONTAINER_ਸਥਿਤੀ_CREATED,
  LIBCRUN_CONTAINER_ਸਥਿਤੀ_RUNNING,
  LIBCRUN_CONTAINER_ਸਥਿਤੀ_STOPPED,
  LIBCRUN_CONTAINER_ਸਥਿਤੀ_PAUSED,
} libcrun_container_state_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*libcrun_output_handler) (ਪੂਰਨ errno_, ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ਪੂਰਨ verbosity, ਖਾਲੀ *arg);

/* Error handling.  */
ਪੂਰਨ libcrun_error_get_ਸਥਿਤੀ (libcrun_error_t err);
ਅਚਲ ਅੱਖਰ *libcrun_error_get_ਸੁਨੇਹਾ (libcrun_error_t err);
ਪੂਰਨ libcrun_error_release (libcrun_error_t *err);

/* Create an error object; returns a negative value derived from status.  */
ਪੂਰਨ libcrun_make_error (libcrun_error_t *err, ਪੂਰਨ ਸਥਿਤੀ, ਅਚਲ ਅੱਖਰ *fmt, ...) LIBCRUN_PRINTF (3, 4);

/* Report *err through the configured log handler and release it.  */
ਖਾਲੀ libcrun_error_report_and_release (libcrun_error_t *err);

/* Report the error and terminate the process.  */
ਖਾਲੀ libcrun_fail_with_error (ਪੂਰਨ errno_, ਅਚਲ ਅੱਖਰ *fmt, ...) LIBCRUN_NORETURN LIBCRUN_PRINTF (2, 3);

/* Logging.  */
ਖਾਲੀ libcrun_ਸੈੱਟ_verbosity (ਪੂਰਨ verbosity);
ਪੂਰਨ libcrun_get_verbosity (ਖਾਲੀ);
ਪੂਰਨ libcrun_ਸੈੱਟ_log_format (ਅਚਲ ਅੱਖਰ *format, libcrun_error_t *err);
ਖਾਲੀ libcrun_debug (ਅਚਲ ਅੱਖਰ *fmt, ...) LIBCRUN_PRINTF (1, 2);
ਖਾਲੀ libcrun_warning (ਅਚਲ ਅੱਖਰ *fmt, ...) LIBCRUN_PRINTF (1, 2);
ਖਾਲੀ libcrun_error (ਪੂਰਨ errno_, ਅਚਲ ਅੱਖਰ *fmt, ...) LIBCRUN_PRINTF (2, 3);

/* Context.  */
libcrun_context_t *libcrun_context_new (ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, libcrun_error_t *err);
ਖਾਲੀ libcrun_context_free (libcrun_context_t *ctx);

ਖਾਲੀ libcrun_context_ਸੈੱਟ_id (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ);
ਖਾਲੀ libcrun_context_ਸੈੱਟ_state_ਜੜ੍ਹ (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ);
ਅਚਲ ਅੱਖਰ *libcrun_context_get_state_ਜੜ੍ਹ (libcrun_context_t *ctx);
ਖਾਲੀ libcrun_context_ਸੈੱਟ_bundle (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ);
ਖਾਲੀ libcrun_context_ਸੈੱਟ_console_socket (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ);
ਖਾਲੀ libcrun_context_ਸੈੱਟ_pid_file (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ);
ਅਚਲ ਅੱਖਰ *libcrun_context_get_pid_file (libcrun_context_t *ctx);
ਖਾਲੀ libcrun_context_ਸੈੱਟ_notify_socket (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ);
ਖਾਲੀ libcrun_context_ਸੈੱਟ_handler (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ);
ਖਾਲੀ libcrun_context_ਸੈੱਟ_preserve_fds (libcrun_context_t *ctx, ਪੂਰਨ preserve_fds);
ਪੂਰਨ libcrun_context_get_preserve_fds (libcrun_context_t *ctx);
ਖਾਲੀ libcrun_context_ਸੈੱਟ_listen_fds (libcrun_context_t *ctx, ਪੂਰਨ listen_fds);
ਖਾਲੀ libcrun_context_ਸੈੱਟ_ਦਲੀਲਾਂ (libcrun_context_t *ctx, ਪੂਰਨ argc, ਅੱਖਰ **argv);
ਖਾਲੀ libcrun_context_ਸੈੱਟ_systemd_cgroup (libcrun_context_t *ctx, bool ਮੁੱਲ);
ਖਾਲੀ libcrun_context_ਸੈੱਟ_detach (libcrun_context_t *ctx, bool ਮੁੱਲ);
ਖਾਲੀ libcrun_context_ਸੈੱਟ_no_new_keyring (libcrun_context_t *ctx, bool ਮੁੱਲ);
ਖਾਲੀ libcrun_context_ਸੈੱਟ_no_ਧੁਰਾ (libcrun_context_t *ctx, bool ਮੁੱਲ);
ਖਾਲੀ libcrun_context_ਸੈੱਟ_force_no_cgroup (libcrun_context_t *ctx, bool ਮੁੱਲ);
ਖਾਲੀ libcrun_context_ਸੈੱਟ_output_handler (libcrun_context_t *ctx, libcrun_output_handler handler, ਖਾਲੀ *arg);
ਪੂਰਨ libcrun_context_init_logging (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *ਲਘੂਗਣਕ, libcrun_error_t *err);

/* Close the file descriptors inherited from the caller, honoring the
   preserve-fds and listen-fds settings in CTX.  */
ਪੂਰਨ libcrun_close_inherited_fds (libcrun_context_t *ctx, libcrun_error_t *err);

/* Container object (loaded from JSON, introspected via accessors).  */
libcrun_container_t *libcrun_container_ਲੱਦੋ_from_file (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, libcrun_error_t *err);
libcrun_container_t *libcrun_container_ਲੱਦੋ_from_ਸਮ੍ਰਿਤੀ (ਅਚਲ ਅੱਖਰ *json, libcrun_error_t *err);
ਖਾਲੀ libcrun_container_free (libcrun_container_t *container);

/* Return the configuration of CONTAINER as a JSON string owned by CONTAINER.
   The document is generated on demand and cached, so the first call mutates
   CONTAINER and must not race with other users of the same handle.  */
ਅਚਲ ਅੱਖਰ *libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err);
ਅਚਲ ਅੱਖਰ *libcrun_container_get_annotation (libcrun_container_t *container, ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ);

/* Enumerate the annotations of container.  libcrun_container_get_annotation_at
   returns 0 and sets *key and *value (both owned by container) for an index
   below libcrun_container_get_annotations_len, -1 otherwise.  */
size_t libcrun_container_get_annotations_len (libcrun_container_t *container);
ਪੂਰਨ libcrun_container_get_annotation_at (libcrun_container_t *container, size_t ਸੂਚਕਾਂਕ, ਅਚਲ ਅੱਖਰ **ਕੁੰਜੀ,
                                         ਅਚਲ ਅੱਖਰ **ਮੁੱਲ);

uid_t libcrun_container_get_uid (libcrun_container_t *container);
gid_t libcrun_container_get_gid (libcrun_container_t *container);

/* Lifecycle.  */
ਪੂਰਨ libcrun_container_ਲੜੀਵਾਰ (libcrun_context_t *ctx, libcrun_container_t *container, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ options,
                           libcrun_error_t *err);
ਪੂਰਨ libcrun_container_create (libcrun_context_t *ctx, libcrun_container_t *container, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ options,
                              libcrun_error_t *err);
ਪੂਰਨ libcrun_container_ਸ਼ੁਰੂ (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err);
ਪੂਰਨ libcrun_container_delete (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *id, bool force, libcrun_error_t *err);
ਪੂਰਨ libcrun_container_kill (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *ਸੰਕੇਤ, libcrun_error_t *err);
ਪੂਰਨ libcrun_container_killall (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *ਸੰਕੇਤ, libcrun_error_t *err);
ਪੂਰਨ libcrun_container_pause (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err);
ਪੂਰਨ libcrun_container_unpause (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err);
ਪੂਰਨ libcrun_container_update (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *content, size_t len,
                              libcrun_error_t *err);

/* JSON in, JSON out; caller frees *out with free().  */
ਪੂਰਨ libcrun_container_spec_json (bool ਜੜ੍ਹ, ਅੱਖਰ **ਬਾਹਰਲਾ, libcrun_error_t *err);
ਪੂਰਨ libcrun_container_state_json (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *id, ਅੱਖਰ **ਬਾਹਰਲਾ, libcrun_error_t *err);

/* Runtime features as an OCI features JSON document; caller frees *out with free().  */
ਪੂਰਨ libcrun_container_get_features_json (libcrun_context_t *ctx, ਅੱਖਰ **ਬਾਹਰਲਾ, libcrun_error_t *err);

ਬਣਤਰ libcrun_exec_options_s
{
  size_t struct_ਮਾਪ; /* set to sizeof (struct libcrun_exec_options_s) */
  ਅਚਲ ਅੱਖਰ *process_json;
  ਅਚਲ ਅੱਖਰ *cgroup;
  bool ਮਿਲਾਓ_env; /* merge the container environment with the one in process_json */
};

ਪੂਰਨ libcrun_container_exec_json (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *process_json,
                                 libcrun_error_t *err);
ਪੂਰਨ libcrun_container_exec_with_options_json (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *id,
                                              ਅਚਲ ਬਣਤਰ libcrun_exec_options_s *opts, libcrun_error_t *err);

/* Status.  */
ਪੂਰਨ libcrun_container_ਸਥਿਤੀ_ਲੱਦੋ (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *id, libcrun_ਸਥਿਤੀ_t **ਬਾਹਰਲਾ,
                                   libcrun_error_t *err);
ਖਾਲੀ libcrun_container_ਸਥਿਤੀ_free (libcrun_ਸਥਿਤੀ_t *st);

/* Resolve the live state of the container described by st, by probing it
   (cgroup/fifo/pid).  The result is cached in st, so this costs at most one
   probe per status object.  state and running can be NULL.  */
ਪੂਰਨ libcrun_ਸਥਿਤੀ_get_state (libcrun_ਸਥਿਤੀ_t *st, libcrun_container_state_t *state, ਪੂਰਨ *running,
                              libcrun_error_t *err);

/* Return the OCI name of state ("creating"/"created"/"running"/"stopped"/"paused"),
   as a static string (do not free).  */
ਅਚਲ ਅੱਖਰ *libcrun_container_state_ਮੰਜ਼ਿਲ_string (libcrun_container_state_t state);

pid_t libcrun_ਸਥਿਤੀ_get_pid (libcrun_ਸਥਿਤੀ_t *st);
ਅਚਲ ਅੱਖਰ *libcrun_ਸਥਿਤੀ_get_bundle (libcrun_ਸਥਿਤੀ_t *st);
ਅਚਲ ਅੱਖਰ *libcrun_ਸਥਿਤੀ_get_rootfs (libcrun_ਸਥਿਤੀ_t *st);
ਅਚਲ ਅੱਖਰ *libcrun_ਸਥਿਤੀ_get_created (libcrun_ਸਥਿਤੀ_t *st);
ਅਚਲ ਅੱਖਰ *libcrun_ਸਥਿਤੀ_get_owner (libcrun_ਸਥਿਤੀ_t *st);
ਅਚਲ ਅੱਖਰ *libcrun_ਸਥਿਤੀ_get_scope (libcrun_ਸਥਿਤੀ_t *st);
ਅਚਲ ਅੱਖਰ *libcrun_ਸਥਿਤੀ_get_external_descriptors (libcrun_ਸਥਿਤੀ_t *st);

/* Live status string ("creating"/"created"/"running"/"stopped"/"paused"), obtained by
   probing the container (cgroup/fifo/pid) rather than reading the stored enum.  *out
   points to a static string (do not free).  If running is non-NULL it receives 1 when
   the container has a live process, 0 otherwise.  */
ਪੂਰਨ libcrun_container_get_state_string (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ **ਬਾਹਰਲਾ, ਪੂਰਨ *running,
                                        libcrun_error_t *err);

/* Resolve the on-disk state directory for a container (id may be NULL for the state
   root itself); caller frees *out with free().  */
ਪੂਰਨ libcrun_get_state_dir (ਅੱਖਰ **ਬਾਹਰਲਾ, ਅਚਲ ਅੱਖਰ *state_ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ *id, libcrun_error_t *err);

/* List containers under the state root.  Obtain a cursor with
   libcrun_container_list_iter and walk it with libcrun_container_iter_next, which
   stores a borrowed container id in *id and returns true until the list is exhausted.  */
ਪੂਰਨ libcrun_container_list (libcrun_context_t *ctx, libcrun_container_list_t **ਬਾਹਰਲਾ, libcrun_error_t *err);
libcrun_container_iter_t *libcrun_container_list_iter (libcrun_container_list_t *l);
bool libcrun_container_iter_ਅਗਲਾ (libcrun_container_iter_t *it, ਅਚਲ ਅੱਖਰ **id);
ਖਾਲੀ libcrun_container_iter_free (libcrun_container_iter_t *it);
ਖਾਲੀ libcrun_container_list_free (libcrun_container_list_t *l);

/* All containers with their details as a JSON array; caller frees *out with free().  */
ਪੂਰਨ libcrun_container_list_json (libcrun_context_t *ctx, ਅੱਖਰ **ਬਾਹਰਲਾ, libcrun_error_t *err);

/* Update the resources of a running container from an OCI resources JSON file.  */
ਪੂਰਨ libcrun_container_update_from_file (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *file,
                                        libcrun_error_t *err);

/* Update individual resource values.  */
ਬਣਤਰ libcrun_update_ਮੁੱਲ_s
{
  ਅਚਲ ਅੱਖਰ *section;
  ਅਚਲ ਅੱਖਰ *name;
  bool numeric;
  ਅਚਲ ਅੱਖਰ *ਮੁੱਲ;
};

ਪੂਰਨ libcrun_container_update_from_ਮੁੱਲਾਂ (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *id,
                                          ਬਣਤਰ libcrun_update_ਮੁੱਲ_s *ਮੁੱਲਾਂ, size_t len,
                                          libcrun_error_t *err);

/* Update the Intel RDT (L3 cache / memory bandwidth) schemata.  */
ਬਣਤਰ libcrun_intel_rdt_update
{
  ਅਚਲ ਅੱਖਰ *l3_cache_schema;
  ਅਚਲ ਅੱਖਰ *mem_bw_schema;
  ਅੱਖਰ *ਅਚਲ *schemata;
};

ਪੂਰਨ libcrun_container_update_intel_rdt (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *id,
                                        ਬਣਤਰ libcrun_intel_rdt_update *update, libcrun_error_t *err);

/* Add or remove mounts described by an OCI mounts JSON file.  */
ਪੂਰਨ libcrun_container_ਜੋੜੋ_mounts_from_file (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *file,
                                            libcrun_error_t *err);
ਪੂਰਨ libcrun_container_remove_mounts_from_file (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *id, ਅਚਲ ਅੱਖਰ *file,
                                               libcrun_error_t *err);

/* Enumerate the PIDs in a container; caller frees *pids with free().  */
ਪੂਰਨ libcrun_container_read_pids (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *id, bool recurse, pid_t **pids,
                                 libcrun_error_t *err);

/* Checkpoint and restore (CRIU).  */
ਗਿਣਤੀ libcrun_cr_cgroups_ਬਹੁਲਕ
{
  LIBCRUN_CR_CG_ਬਹੁਲਕ_DEFAULT = 0, /* soft, matching runc */
  LIBCRUN_CR_CG_ਬਹੁਲਕ_SOFT,
  LIBCRUN_CR_CG_ਬਹੁਲਕ_IGNORE,
  LIBCRUN_CR_CG_ਬਹੁਲਕ_ਭਰਿਆ,
  LIBCRUN_CR_CG_ਬਹੁਲਕ_STRICT,
};

ਗਿਣਤੀ libcrun_cr_ਜਾਲ_ਤਾਲਾ_ਵਿਧੀ
{
  LIBCRUN_CR_ਜਾਲ_ਤਾਲਾ_DEFAULT = 0, /* iptables */
  LIBCRUN_CR_ਜਾਲ_ਤਾਲਾ_IPTABLES,
  LIBCRUN_CR_ਜਾਲ_ਤਾਲਾ_NFTABLES,
  LIBCRUN_CR_ਜਾਲ_ਤਾਲਾ_SKIP,
};

ਬਣਤਰ libcrun_checkpoint_restore_options_s
{
  size_t struct_ਮਾਪ; /* set to sizeof (struct libcrun_checkpoint_restore_options_s) */

  ਅਚਲ ਅੱਖਰ *image_ਰਸਤਾ;
  ਅਚਲ ਅੱਖਰ *work_ਰਸਤਾ;
  ਅਚਲ ਅੱਖਰ *ਮਾਪਾ_ਰਸਤਾ;
  ਅਚਲ ਅੱਖਰ *console_socket;
  ਅਚਲ ਅੱਖਰ *lsm_profile;
  ਅਚਲ ਅੱਖਰ *lsm_mount_context;

  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool ext_unix_sk;
  bool shell_ਕੰਮ;
  bool file_locks;
  bool pre_dump;
  bool detach;

  ਪੂਰਨ manage_cgroups_ਬਹੁਲਕ; /* enum libcrun_cr_cgroups_mode */
  ਪੂਰਨ ਜਾਲ_ਤਾਲਾ_ਵਿਧੀ; /* enum libcrun_cr_network_lock_method */
};

ਪੂਰਨ libcrun_container_checkpoint (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *id,
                                  ਬਣਤਰ libcrun_checkpoint_restore_options_s *opts, libcrun_error_t *err);
ਪੂਰਨ libcrun_container_restore (libcrun_context_t *ctx, ਅਚਲ ਅੱਖਰ *id,
                               ਬਣਤਰ libcrun_checkpoint_restore_options_s *opts, libcrun_error_t *err);

/* Feature tags contributed by the loaded handlers, as a NULL-terminated array of
   strings (e.g. "wasm:wasmedge").  Returns NULL if there are none.  Caller frees the
   array and each string with free().  */
ਅੱਖਰ **libcrun_context_get_handler_feature_ਟੈਗ_ਸੂਚੀ (libcrun_context_t *ctx, libcrun_error_t *err);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ
