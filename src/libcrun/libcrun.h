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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LIBCRUN_H
#ವ್ಯಾಖ್ಯಾನಿಸು LIBCRUN_H

#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __GNUC__
#  define LIBCRUN_PRINTF(a, b) __attribute__ ((format (ಬರೆ, a, b)))
#  define LIBCRUN_NORETURN __attribute__ ((noreturn))
#ಅಥವಾಆದರೆ defined __STDC_VERSION__ && __STDC_VERSION__ >= 201112L
#  define LIBCRUN_PRINTF(a, b)
#  define LIBCRUN_NORETURN _Noreturn
#ಇಲ್ಲದಿದ್ದರೆ
#  define LIBCRUN_PRINTF(a, b)
#  define LIBCRUN_NORETURN
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
ಬಾಹ್ಯ "C" {
#ಆದರೆ_ಅಂತ್ಯ

/* Opaque handles.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ libcrun_error_s *libcrun_error_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ libcrun_context_s libcrun_context_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ libcrun_container_s libcrun_container_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ libcrun_ಸ್ಥಿತಿ_s libcrun_ಸ್ಥಿತಿ_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ libcrun_container_list_s libcrun_container_list_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ libcrun_container_iter_s libcrun_container_iter_t;

ಎಣಿಕೆ
{
  LIBCRUN_ಸತತ_OPTIONS_PREFORK = 1 << 0,
  LIBCRUN_ಸತತ_OPTIONS_KEEP = 1 << 1,
};

ಎಣಿಕೆ
{
  LIBCRUN_CREATE_OPTIONS_PREFORK = 1 << 0,
};

ಎಣಿಕೆ
{
  LIBCRUN_VERBOSITY_ERROR = 0,
  LIBCRUN_VERBOSITY_WARNING,
  LIBCRUN_VERBOSITY_DEBUG,
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ
{
  LIBCRUN_CONTAINER_ಸ್ಥಿತಿ_CREATING = 0,
  LIBCRUN_CONTAINER_ಸ್ಥಿತಿ_CREATED,
  LIBCRUN_CONTAINER_ಸ್ಥಿತಿ_RUNNING,
  LIBCRUN_CONTAINER_ಸ್ಥಿತಿ_STOPPED,
  LIBCRUN_CONTAINER_ಸ್ಥಿತಿ_PAUSED,
} libcrun_container_state_t;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*libcrun_output_handler) (ಪೂರ್ಣಾಂಕ errno_, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, ಪೂರ್ಣಾಂಕ verbosity, ಶೂನ್ಯ *arg);

/* Error handling.  */
ಪೂರ್ಣಾಂಕ libcrun_error_get_ಸ್ಥಿತಿ (libcrun_error_t err);
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libcrun_error_get_ಸಂದೇಶ (libcrun_error_t err);
ಪೂರ್ಣಾಂಕ libcrun_error_release (libcrun_error_t *err);

/* Create an error object; returns a negative value derived from status.  */
ಪೂರ್ಣಾಂಕ libcrun_make_error (libcrun_error_t *err, ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...) LIBCRUN_PRINTF (3, 4);

/* Report *err through the configured log handler and release it.  */
ಶೂನ್ಯ libcrun_error_report_and_release (libcrun_error_t *err);

/* Report the error and terminate the process.  */
ಶೂನ್ಯ libcrun_fail_with_error (ಪೂರ್ಣಾಂಕ errno_, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...) LIBCRUN_NORETURN LIBCRUN_PRINTF (2, 3);

/* Logging.  */
ಶೂನ್ಯ libcrun_ಗಣ_verbosity (ಪೂರ್ಣಾಂಕ verbosity);
ಪೂರ್ಣಾಂಕ libcrun_get_verbosity (ಶೂನ್ಯ);
ಪೂರ್ಣಾಂಕ libcrun_ಗಣ_log_format (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *format, libcrun_error_t *err);
ಶೂನ್ಯ libcrun_debug (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...) LIBCRUN_PRINTF (1, 2);
ಶೂನ್ಯ libcrun_warning (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...) LIBCRUN_PRINTF (1, 2);
ಶೂನ್ಯ libcrun_error (ಪೂರ್ಣಾಂಕ errno_, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...) LIBCRUN_PRINTF (2, 3);

/* Context.  */
libcrun_context_t *libcrun_context_new (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, libcrun_error_t *err);
ಶೂನ್ಯ libcrun_context_free (libcrun_context_t *ctx);

ಶೂನ್ಯ libcrun_context_ಗಣ_id (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ);
ಶೂನ್ಯ libcrun_context_ಗಣ_state_ಬೇರು (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ);
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libcrun_context_get_state_ಬೇರು (libcrun_context_t *ctx);
ಶೂನ್ಯ libcrun_context_ಗಣ_bundle (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ);
ಶೂನ್ಯ libcrun_context_ಗಣ_console_socket (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ);
ಶೂನ್ಯ libcrun_context_ಗಣ_pid_file (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ);
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libcrun_context_get_pid_file (libcrun_context_t *ctx);
ಶೂನ್ಯ libcrun_context_ಗಣ_notify_socket (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ);
ಶೂನ್ಯ libcrun_context_ಗಣ_handler (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ);
ಶೂನ್ಯ libcrun_context_ಗಣ_preserve_fds (libcrun_context_t *ctx, ಪೂರ್ಣಾಂಕ preserve_fds);
ಪೂರ್ಣಾಂಕ libcrun_context_get_preserve_fds (libcrun_context_t *ctx);
ಶೂನ್ಯ libcrun_context_ಗಣ_listen_fds (libcrun_context_t *ctx, ಪೂರ್ಣಾಂಕ listen_fds);
ಶೂನ್ಯ libcrun_context_ಗಣ_ವಾದಗಳು (libcrun_context_t *ctx, ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv);
ಶೂನ್ಯ libcrun_context_ಗಣ_systemd_cgroup (libcrun_context_t *ctx, bool ಮೌಲ್ಯ);
ಶೂನ್ಯ libcrun_context_ಗಣ_detach (libcrun_context_t *ctx, bool ಮೌಲ್ಯ);
ಶೂನ್ಯ libcrun_context_ಗಣ_no_new_keyring (libcrun_context_t *ctx, bool ಮೌಲ್ಯ);
ಶೂನ್ಯ libcrun_context_ಗಣ_no_ಅಚ್ಚು (libcrun_context_t *ctx, bool ಮೌಲ್ಯ);
ಶೂನ್ಯ libcrun_context_ಗಣ_force_no_cgroup (libcrun_context_t *ctx, bool ಮೌಲ್ಯ);
ಶೂನ್ಯ libcrun_context_ಗಣ_output_handler (libcrun_context_t *ctx, libcrun_output_handler handler, ಶೂನ್ಯ *arg);
ಪೂರ್ಣಾಂಕ libcrun_context_init_logging (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಲಘುಗಣಕ, libcrun_error_t *err);

/* Close the file descriptors inherited from the caller, honoring the
   preserve-fds and listen-fds settings in CTX.  */
ಪೂರ್ಣಾಂಕ libcrun_close_inherited_fds (libcrun_context_t *ctx, libcrun_error_t *err);

/* Container object (loaded from JSON, introspected via accessors).  */
libcrun_container_t *libcrun_container_ತುಂಬು_from_file (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err);
libcrun_container_t *libcrun_container_ತುಂಬು_from_ಸ್ಮರಣೆ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *json, libcrun_error_t *err);
ಶೂನ್ಯ libcrun_container_free (libcrun_container_t *container);

/* Return the configuration of CONTAINER as a JSON string owned by CONTAINER.
   The document is generated on demand and cached, so the first call mutates
   CONTAINER and must not race with other users of the same handle.  */
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err);
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libcrun_container_get_annotation (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ);

/* Enumerate the annotations of container.  libcrun_container_get_annotation_at
   returns 0 and sets *key and *value (both owned by container) for an index
   below libcrun_container_get_annotations_len, -1 otherwise.  */
size_t libcrun_container_get_annotations_len (libcrun_container_t *container);
ಪೂರ್ಣಾಂಕ libcrun_container_get_annotation_at (libcrun_container_t *container, size_t ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಕೀಲಿ,
                                         ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಮೌಲ್ಯ);

uid_t libcrun_container_get_uid (libcrun_container_t *container);
gid_t libcrun_container_get_gid (libcrun_container_t *container);

/* Lifecycle.  */
ಪೂರ್ಣಾಂಕ libcrun_container_ಸತತ (libcrun_context_t *ctx, libcrun_container_t *container, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ options,
                           libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_container_create (libcrun_context_t *ctx, libcrun_container_t *container, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ options,
                              libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_container_ಆರಂಭ (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_container_delete (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, bool force, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_container_kill (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂಕೇತ, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_container_killall (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂಕೇತ, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_container_pause (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_container_unpause (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_container_update (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *content, size_t len,
                              libcrun_error_t *err);

/* JSON in, JSON out; caller frees *out with free().  */
ಪೂರ್ಣಾಂಕ libcrun_container_spec_json (bool ಬೇರು, ಅಕ್ಷರ **ಹೊರಹೋಗು, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_container_state_json (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಅಕ್ಷರ **ಹೊರಹೋಗು, libcrun_error_t *err);

/* Runtime features as an OCI features JSON document; caller frees *out with free().  */
ಪೂರ್ಣಾಂಕ libcrun_container_get_features_json (libcrun_context_t *ctx, ಅಕ್ಷರ **ಹೊರಹೋಗು, libcrun_error_t *err);

ರಚನೆ libcrun_exec_options_s
{
  size_t struct_ಗಾತ್ರ; /* set to sizeof (struct libcrun_exec_options_s) */
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *process_json;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cgroup;
  bool ವಿಲೀನ_env; /* merge the container environment with the one in process_json */
};

ಪೂರ್ಣಾಂಕ libcrun_container_exec_json (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *process_json,
                                 libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_container_exec_with_options_json (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id,
                                              ಸ್ಥಿರಾಂಕ ರಚನೆ libcrun_exec_options_s *opts, libcrun_error_t *err);

/* Status.  */
ಪೂರ್ಣಾಂಕ libcrun_container_ಸ್ಥಿತಿ_ತುಂಬು (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_ಸ್ಥಿತಿ_t **ಹೊರಹೋಗು,
                                   libcrun_error_t *err);
ಶೂನ್ಯ libcrun_container_ಸ್ಥಿತಿ_free (libcrun_ಸ್ಥಿತಿ_t *st);

/* Resolve the live state of the container described by st, by probing it
   (cgroup/fifo/pid).  The result is cached in st, so this costs at most one
   probe per status object.  state and running can be NULL.  */
ಪೂರ್ಣಾಂಕ libcrun_ಸ್ಥಿತಿ_get_state (libcrun_ಸ್ಥಿತಿ_t *st, libcrun_container_state_t *state, ಪೂರ್ಣಾಂಕ *running,
                              libcrun_error_t *err);

/* Return the OCI name of state ("creating"/"created"/"running"/"stopped"/"paused"),
   as a static string (do not free).  */
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libcrun_container_state_ಗಮ್ಯ_string (libcrun_container_state_t state);

pid_t libcrun_ಸ್ಥಿತಿ_get_pid (libcrun_ಸ್ಥಿತಿ_t *st);
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libcrun_ಸ್ಥಿತಿ_get_bundle (libcrun_ಸ್ಥಿತಿ_t *st);
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libcrun_ಸ್ಥಿತಿ_get_rootfs (libcrun_ಸ್ಥಿತಿ_t *st);
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libcrun_ಸ್ಥಿತಿ_get_created (libcrun_ಸ್ಥಿತಿ_t *st);
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libcrun_ಸ್ಥಿತಿ_get_owner (libcrun_ಸ್ಥಿತಿ_t *st);
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libcrun_ಸ್ಥಿತಿ_get_scope (libcrun_ಸ್ಥಿತಿ_t *st);
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *libcrun_ಸ್ಥಿತಿ_get_external_descriptors (libcrun_ಸ್ಥಿತಿ_t *st);

/* Live status string ("creating"/"created"/"running"/"stopped"/"paused"), obtained by
   probing the container (cgroup/fifo/pid) rather than reading the stored enum.  *out
   points to a static string (do not free).  If running is non-NULL it receives 1 when
   the container has a live process, 0 otherwise.  */
ಪೂರ್ಣಾಂಕ libcrun_container_get_state_string (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಹೊರಹೋಗು, ಪೂರ್ಣಾಂಕ *running,
                                        libcrun_error_t *err);

/* Resolve the on-disk state directory for a container (id may be NULL for the state
   root itself); caller frees *out with free().  */
ಪೂರ್ಣಾಂಕ libcrun_get_state_dir (ಅಕ್ಷರ **ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, libcrun_error_t *err);

/* List containers under the state root.  Obtain a cursor with
   libcrun_container_list_iter and walk it with libcrun_container_iter_next, which
   stores a borrowed container id in *id and returns true until the list is exhausted.  */
ಪೂರ್ಣಾಂಕ libcrun_container_list (libcrun_context_t *ctx, libcrun_container_list_t **ಹೊರಹೋಗು, libcrun_error_t *err);
libcrun_container_iter_t *libcrun_container_list_iter (libcrun_container_list_t *l);
bool libcrun_container_iter_ಮುಂದಿನ (libcrun_container_iter_t *it, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **id);
ಶೂನ್ಯ libcrun_container_iter_free (libcrun_container_iter_t *it);
ಶೂನ್ಯ libcrun_container_list_free (libcrun_container_list_t *l);

/* All containers with their details as a JSON array; caller frees *out with free().  */
ಪೂರ್ಣಾಂಕ libcrun_container_list_json (libcrun_context_t *ctx, ಅಕ್ಷರ **ಹೊರಹೋಗು, libcrun_error_t *err);

/* Update the resources of a running container from an OCI resources JSON file.  */
ಪೂರ್ಣಾಂಕ libcrun_container_update_from_file (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file,
                                        libcrun_error_t *err);

/* Update individual resource values.  */
ರಚನೆ libcrun_update_ಮೌಲ್ಯ_s
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *section;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
  bool numeric;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ;
};

ಪೂರ್ಣಾಂಕ libcrun_container_update_from_ಮೌಲ್ಯಗಳು (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id,
                                          ರಚನೆ libcrun_update_ಮೌಲ್ಯ_s *ಮೌಲ್ಯಗಳು, size_t len,
                                          libcrun_error_t *err);

/* Update the Intel RDT (L3 cache / memory bandwidth) schemata.  */
ರಚನೆ libcrun_intel_rdt_update
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *l3_cache_schema;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *mem_bw_schema;
  ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *schemata;
};

ಪೂರ್ಣಾಂಕ libcrun_container_update_intel_rdt (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id,
                                        ರಚನೆ libcrun_intel_rdt_update *update, libcrun_error_t *err);

/* Add or remove mounts described by an OCI mounts JSON file.  */
ಪೂರ್ಣಾಂಕ libcrun_container_ಕೂಡಿಸು_mounts_from_file (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file,
                                            libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_container_remove_mounts_from_file (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file,
                                               libcrun_error_t *err);

/* Enumerate the PIDs in a container; caller frees *pids with free().  */
ಪೂರ್ಣಾಂಕ libcrun_container_read_pids (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id, bool recurse, pid_t **pids,
                                 libcrun_error_t *err);

/* Checkpoint and restore (CRIU).  */
ಎಣಿಕೆ libcrun_cr_cgroups_ಬಹುಲಕ
{
  LIBCRUN_CR_CG_ಬಹುಲಕ_DEFAULT = 0, /* soft, matching runc */
  LIBCRUN_CR_CG_ಬಹುಲಕ_SOFT,
  LIBCRUN_CR_CG_ಬಹುಲಕ_IGNORE,
  LIBCRUN_CR_CG_ಬಹುಲಕ_ತುಂಬಿದ,
  LIBCRUN_CR_CG_ಬಹುಲಕ_STRICT,
};

ಎಣಿಕೆ libcrun_cr_ಜಾಲ_ಬೀಗ_ವಿಧಾನ
{
  LIBCRUN_CR_ಜಾಲ_ಬೀಗ_DEFAULT = 0, /* iptables */
  LIBCRUN_CR_ಜಾಲ_ಬೀಗ_IPTABLES,
  LIBCRUN_CR_ಜಾಲ_ಬೀಗ_NFTABLES,
  LIBCRUN_CR_ಜಾಲ_ಬೀಗ_SKIP,
};

ರಚನೆ libcrun_checkpoint_restore_options_s
{
  size_t struct_ಗಾತ್ರ; /* set to sizeof (struct libcrun_checkpoint_restore_options_s) */

  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *image_ಮಾರ್ಗ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *work_ಮಾರ್ಗ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೋಷಕ_ಮಾರ್ಗ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *console_socket;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *lsm_profile;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *lsm_mount_context;

  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool ext_unix_sk;
  bool shell_ಕೆಲಸ;
  bool file_locks;
  bool pre_dump;
  bool detach;

  ಪೂರ್ಣಾಂಕ manage_cgroups_ಬಹುಲಕ; /* enum libcrun_cr_cgroups_mode */
  ಪೂರ್ಣಾಂಕ ಜಾಲ_ಬೀಗ_ವಿಧಾನ; /* enum libcrun_cr_network_lock_method */
};

ಪೂರ್ಣಾಂಕ libcrun_container_checkpoint (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id,
                                  ರಚನೆ libcrun_checkpoint_restore_options_s *opts, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_container_restore (libcrun_context_t *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id,
                               ರಚನೆ libcrun_checkpoint_restore_options_s *opts, libcrun_error_t *err);

/* Feature tags contributed by the loaded handlers, as a NULL-terminated array of
   strings (e.g. "wasm:wasmedge").  Returns NULL if there are none.  Caller frees the
   array and each string with free().  */
ಅಕ್ಷರ **libcrun_context_get_handler_feature_ಟ್ಯಾಗುಗಳು (libcrun_context_t *ctx, libcrun_error_t *err);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ
