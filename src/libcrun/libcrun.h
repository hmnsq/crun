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

#నిర్వచితం_కాకపోతే LIBCRUN_H
#నిర్వచించు LIBCRUN_H

#అన్తర్భావ <stdbool.h>
#అన్తర్భావ <stddef.h>
#అన్తర్భావ <sys/types.h>

#నిర్వచితమైతే __GNUC__
#  define LIBCRUN_PRINTF(a, b) __attribute__ ((format (రాయి, a, b)))
#  define LIBCRUN_NORETURN __attribute__ ((noreturn))
#లేదావేళ defined __STDC_VERSION__ && __STDC_VERSION__ >= 201112L
#  define LIBCRUN_PRINTF(a, b)
#  define LIBCRUN_NORETURN _Noreturn
#లేకపోతే
#  define LIBCRUN_PRINTF(a, b)
#  define LIBCRUN_NORETURN
#అయితే_ముగింపు

#నిర్వచితమైతే __cplusplus
బాహ్య "C" {
#అయితే_ముగింపు

/* Opaque handles.  */
రకనిర్వచనం నిర్మాణం libcrun_error_s *libcrun_error_t;
రకనిర్వచనం నిర్మాణం libcrun_context_s libcrun_context_t;
రకనిర్వచనం నిర్మాణం libcrun_container_s libcrun_container_t;
రకనిర్వచనం నిర్మాణం libcrun_స్థితి_s libcrun_స్థితి_t;
రకనిర్వచనం నిర్మాణం libcrun_container_list_s libcrun_container_list_t;
రకనిర్వచనం నిర్మాణం libcrun_container_iter_s libcrun_container_iter_t;

గణన
{
  LIBCRUN_నిరంతరం_OPTIONS_PREFORK = 1 << 0,
  LIBCRUN_నిరంతరం_OPTIONS_KEEP = 1 << 1,
};

గణన
{
  LIBCRUN_CREATE_OPTIONS_PREFORK = 1 << 0,
};

గణన
{
  LIBCRUN_VERBOSITY_ERROR = 0,
  LIBCRUN_VERBOSITY_WARNING,
  LIBCRUN_VERBOSITY_DEBUG,
};

రకనిర్వచనం గణన
{
  LIBCRUN_CONTAINER_స్థితి_CREATING = 0,
  LIBCRUN_CONTAINER_స్థితి_CREATED,
  LIBCRUN_CONTAINER_స్థితి_RUNNING,
  LIBCRUN_CONTAINER_స్థితి_STOPPED,
  LIBCRUN_CONTAINER_స్థితి_PAUSED,
} libcrun_container_state_t;

రకనిర్వచనం శూన్య (*libcrun_output_handler) (పూర్ణాంకం errno_, స్థిరరాశి అక్షరం *సందేశం, పూర్ణాంకం verbosity, శూన్య *arg);

/* Error handling.  */
పూర్ణాంకం libcrun_error_get_స్థితి (libcrun_error_t err);
స్థిరరాశి అక్షరం *libcrun_error_get_సందేశం (libcrun_error_t err);
పూర్ణాంకం libcrun_error_release (libcrun_error_t *err);

/* Create an error object; returns a negative value derived from status.  */
పూర్ణాంకం libcrun_make_error (libcrun_error_t *err, పూర్ణాంకం స్థితి_2, స్థిరరాశి అక్షరం *fmt, ...) LIBCRUN_PRINTF (3, 4);

/* Report *err through the configured log handler and release it.  */
శూన్య libcrun_error_report_and_release (libcrun_error_t *err);

/* Report the error and terminate the process.  */
శూన్య libcrun_fail_with_error (పూర్ణాంకం errno_, స్థిరరాశి అక్షరం *fmt, ...) LIBCRUN_NORETURN LIBCRUN_PRINTF (2, 3);

/* Logging.  */
శూన్య libcrun_సమితి_verbosity (పూర్ణాంకం verbosity);
పూర్ణాంకం libcrun_get_verbosity (శూన్య);
పూర్ణాంకం libcrun_సమితి_log_format (స్థిరరాశి అక్షరం *format, libcrun_error_t *err);
శూన్య libcrun_debug (స్థిరరాశి అక్షరం *fmt, ...) LIBCRUN_PRINTF (1, 2);
శూన్య libcrun_warning (స్థిరరాశి అక్షరం *fmt, ...) LIBCRUN_PRINTF (1, 2);
శూన్య libcrun_error (పూర్ణాంకం errno_, స్థిరరాశి అక్షరం *fmt, ...) LIBCRUN_PRINTF (2, 3);

/* Context.  */
libcrun_context_t *libcrun_context_new (స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *state_మూలం, libcrun_error_t *err);
శూన్య libcrun_context_free (libcrun_context_t *ctx);

శూన్య libcrun_context_సమితి_id (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *విలువ);
శూన్య libcrun_context_సమితి_state_మూలం (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *విలువ);
స్థిరరాశి అక్షరం *libcrun_context_get_state_మూలం (libcrun_context_t *ctx);
శూన్య libcrun_context_సమితి_bundle (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *విలువ);
శూన్య libcrun_context_సమితి_console_socket (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *విలువ);
శూన్య libcrun_context_సమితి_pid_file (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *విలువ);
స్థిరరాశి అక్షరం *libcrun_context_get_pid_file (libcrun_context_t *ctx);
శూన్య libcrun_context_సమితి_notify_socket (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *విలువ);
శూన్య libcrun_context_సమితి_handler (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *విలువ);
శూన్య libcrun_context_సమితి_preserve_fds (libcrun_context_t *ctx, పూర్ణాంకం preserve_fds);
పూర్ణాంకం libcrun_context_get_preserve_fds (libcrun_context_t *ctx);
శూన్య libcrun_context_సమితి_listen_fds (libcrun_context_t *ctx, పూర్ణాంకం listen_fds);
శూన్య libcrun_context_సమితి_వాదనలు (libcrun_context_t *ctx, పూర్ణాంకం argc, అక్షరం **argv);
శూన్య libcrun_context_సమితి_systemd_cgroup (libcrun_context_t *ctx, bool విలువ);
శూన్య libcrun_context_సమితి_detach (libcrun_context_t *ctx, bool విలువ);
శూన్య libcrun_context_సమితి_no_new_keyring (libcrun_context_t *ctx, bool విలువ);
శూన్య libcrun_context_సమితి_no_ఇరుసు (libcrun_context_t *ctx, bool విలువ);
శూన్య libcrun_context_సమితి_force_no_cgroup (libcrun_context_t *ctx, bool విలువ);
శూన్య libcrun_context_సమితి_output_handler (libcrun_context_t *ctx, libcrun_output_handler handler, శూన్య *arg);
పూర్ణాంకం libcrun_context_init_logging (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *సంవర్గమానం, libcrun_error_t *err);

/* Close the file descriptors inherited from the caller, honoring the
   preserve-fds and listen-fds settings in CTX.  */
పూర్ణాంకం libcrun_close_inherited_fds (libcrun_context_t *ctx, libcrun_error_t *err);

/* Container object (loaded from JSON, introspected via accessors).  */
libcrun_container_t *libcrun_container_ఎక్కించు_from_file (స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err);
libcrun_container_t *libcrun_container_ఎక్కించు_from_జ్ఞాపకం (స్థిరరాశి అక్షరం *json, libcrun_error_t *err);
శూన్య libcrun_container_free (libcrun_container_t *container);

/* Return the configuration of CONTAINER as a JSON string owned by CONTAINER.
   The document is generated on demand and cached, so the first call mutates
   CONTAINER and must not race with other users of the same handle.  */
స్థిరరాశి అక్షరం *libcrun_container_get_config_json (libcrun_container_t *container, libcrun_error_t *err);
స్థిరరాశి అక్షరం *libcrun_container_get_annotation (libcrun_container_t *container, స్థిరరాశి అక్షరం *కీలకం);

/* Enumerate the annotations of container.  libcrun_container_get_annotation_at
   returns 0 and sets *key and *value (both owned by container) for an index
   below libcrun_container_get_annotations_len, -1 otherwise.  */
size_t libcrun_container_get_annotations_len (libcrun_container_t *container);
పూర్ణాంకం libcrun_container_get_annotation_at (libcrun_container_t *container, size_t సూచీ, స్థిరరాశి అక్షరం **కీలకం,
                                         స్థిరరాశి అక్షరం **విలువ);

uid_t libcrun_container_get_uid (libcrun_container_t *container);
gid_t libcrun_container_get_gid (libcrun_container_t *container);

/* Lifecycle.  */
పూర్ణాంకం libcrun_container_నిరంతరం (libcrun_context_t *ctx, libcrun_container_t *container, గుర్తులేని పూర్ణాంకం options,
                           libcrun_error_t *err);
పూర్ణాంకం libcrun_container_create (libcrun_context_t *ctx, libcrun_container_t *container, గుర్తులేని పూర్ణాంకం options,
                              libcrun_error_t *err);
పూర్ణాంకం libcrun_container_ప్రారంభం (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *id, libcrun_error_t *err);
పూర్ణాంకం libcrun_container_delete (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *id, bool force, libcrun_error_t *err);
పూర్ణాంకం libcrun_container_kill (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *సంకేతం, libcrun_error_t *err);
పూర్ణాంకం libcrun_container_killall (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *సంకేతం, libcrun_error_t *err);
పూర్ణాంకం libcrun_container_pause (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *id, libcrun_error_t *err);
పూర్ణాంకం libcrun_container_unpause (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *id, libcrun_error_t *err);
పూర్ణాంకం libcrun_container_update (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *content, size_t len,
                              libcrun_error_t *err);

/* JSON in, JSON out; caller frees *out with free().  */
పూర్ణాంకం libcrun_container_spec_json (bool మూలం, అక్షరం **బహిర్గతం, libcrun_error_t *err);
పూర్ణాంకం libcrun_container_state_json (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *id, అక్షరం **బహిర్గతం, libcrun_error_t *err);

/* Runtime features as an OCI features JSON document; caller frees *out with free().  */
పూర్ణాంకం libcrun_container_get_features_json (libcrun_context_t *ctx, అక్షరం **బహిర్గతం, libcrun_error_t *err);

నిర్మాణం libcrun_exec_options_s
{
  size_t struct_కొలత; /* set to sizeof (struct libcrun_exec_options_s) */
  స్థిరరాశి అక్షరం *process_json;
  స్థిరరాశి అక్షరం *cgroup;
  bool విలీనం_env; /* merge the container environment with the one in process_json */
};

పూర్ణాంకం libcrun_container_exec_json (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *process_json,
                                 libcrun_error_t *err);
పూర్ణాంకం libcrun_container_exec_with_options_json (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *id,
                                              స్థిరరాశి నిర్మాణం libcrun_exec_options_s *opts, libcrun_error_t *err);

/* Status.  */
పూర్ణాంకం libcrun_container_స్థితి_ఎక్కించు (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *id, libcrun_స్థితి_t **బహిర్గతం,
                                   libcrun_error_t *err);
శూన్య libcrun_container_స్థితి_free (libcrun_స్థితి_t *st);

/* Resolve the live state of the container described by st, by probing it
   (cgroup/fifo/pid).  The result is cached in st, so this costs at most one
   probe per status object.  state and running can be NULL.  */
పూర్ణాంకం libcrun_స్థితి_get_state (libcrun_స్థితి_t *st, libcrun_container_state_t *state, పూర్ణాంకం *running,
                              libcrun_error_t *err);

/* Return the OCI name of state ("creating"/"created"/"running"/"stopped"/"paused"),
   as a static string (do not free).  */
స్థిరరాశి అక్షరం *libcrun_container_state_గమ్యం_string (libcrun_container_state_t state);

pid_t libcrun_స్థితి_get_pid (libcrun_స్థితి_t *st);
స్థిరరాశి అక్షరం *libcrun_స్థితి_get_bundle (libcrun_స్థితి_t *st);
స్థిరరాశి అక్షరం *libcrun_స్థితి_get_rootfs (libcrun_స్థితి_t *st);
స్థిరరాశి అక్షరం *libcrun_స్థితి_get_created (libcrun_స్థితి_t *st);
స్థిరరాశి అక్షరం *libcrun_స్థితి_get_owner (libcrun_స్థితి_t *st);
స్థిరరాశి అక్షరం *libcrun_స్థితి_get_scope (libcrun_స్థితి_t *st);
స్థిరరాశి అక్షరం *libcrun_స్థితి_get_external_descriptors (libcrun_స్థితి_t *st);

/* Live status string ("creating"/"created"/"running"/"stopped"/"paused"), obtained by
   probing the container (cgroup/fifo/pid) rather than reading the stored enum.  *out
   points to a static string (do not free).  If running is non-NULL it receives 1 when
   the container has a live process, 0 otherwise.  */
పూర్ణాంకం libcrun_container_get_state_string (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం **బహిర్గతం, పూర్ణాంకం *running,
                                        libcrun_error_t *err);

/* Resolve the on-disk state directory for a container (id may be NULL for the state
   root itself); caller frees *out with free().  */
పూర్ణాంకం libcrun_get_state_dir (అక్షరం **బహిర్గతం, స్థిరరాశి అక్షరం *state_మూలం, స్థిరరాశి అక్షరం *id, libcrun_error_t *err);

/* List containers under the state root.  Obtain a cursor with
   libcrun_container_list_iter and walk it with libcrun_container_iter_next, which
   stores a borrowed container id in *id and returns true until the list is exhausted.  */
పూర్ణాంకం libcrun_container_list (libcrun_context_t *ctx, libcrun_container_list_t **బహిర్గతం, libcrun_error_t *err);
libcrun_container_iter_t *libcrun_container_list_iter (libcrun_container_list_t *l);
bool libcrun_container_iter_తదుపరి (libcrun_container_iter_t *it, స్థిరరాశి అక్షరం **id);
శూన్య libcrun_container_iter_free (libcrun_container_iter_t *it);
శూన్య libcrun_container_list_free (libcrun_container_list_t *l);

/* All containers with their details as a JSON array; caller frees *out with free().  */
పూర్ణాంకం libcrun_container_list_json (libcrun_context_t *ctx, అక్షరం **బహిర్గతం, libcrun_error_t *err);

/* Update the resources of a running container from an OCI resources JSON file.  */
పూర్ణాంకం libcrun_container_update_from_file (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *file,
                                        libcrun_error_t *err);

/* Update individual resource values.  */
నిర్మాణం libcrun_update_విలువ_s
{
  స్థిరరాశి అక్షరం *section;
  స్థిరరాశి అక్షరం *name;
  bool numeric;
  స్థిరరాశి అక్షరం *విలువ;
};

పూర్ణాంకం libcrun_container_update_from_విలువలు (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *id,
                                          నిర్మాణం libcrun_update_విలువ_s *విలువలు, size_t len,
                                          libcrun_error_t *err);

/* Update the Intel RDT (L3 cache / memory bandwidth) schemata.  */
నిర్మాణం libcrun_intel_rdt_update
{
  స్థిరరాశి అక్షరం *l3_cache_schema;
  స్థిరరాశి అక్షరం *mem_bw_schema;
  అక్షరం *స్థిరరాశి *schemata;
};

పూర్ణాంకం libcrun_container_update_intel_rdt (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *id,
                                        నిర్మాణం libcrun_intel_rdt_update *update, libcrun_error_t *err);

/* Add or remove mounts described by an OCI mounts JSON file.  */
పూర్ణాంకం libcrun_container_కూడు_mounts_from_file (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *file,
                                            libcrun_error_t *err);
పూర్ణాంకం libcrun_container_remove_mounts_from_file (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *id, స్థిరరాశి అక్షరం *file,
                                               libcrun_error_t *err);

/* Enumerate the PIDs in a container; caller frees *pids with free().  */
పూర్ణాంకం libcrun_container_read_pids (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *id, bool recurse, pid_t **pids,
                                 libcrun_error_t *err);

/* Checkpoint and restore (CRIU).  */
గణన libcrun_cr_cgroups_బహుళకం
{
  LIBCRUN_CR_CG_బహుళకం_DEFAULT = 0, /* soft, matching runc */
  LIBCRUN_CR_CG_బహుళకం_SOFT,
  LIBCRUN_CR_CG_బహుళకం_IGNORE,
  LIBCRUN_CR_CG_బహుళకం_నిండిన,
  LIBCRUN_CR_CG_బహుళకం_STRICT,
};

గణన libcrun_cr_జాలం_తాళం_పద్ధతి
{
  LIBCRUN_CR_జాలం_తాళం_DEFAULT = 0, /* iptables */
  LIBCRUN_CR_జాలం_తాళం_IPTABLES,
  LIBCRUN_CR_జాలం_తాళం_NFTABLES,
  LIBCRUN_CR_జాలం_తాళం_SKIP,
};

నిర్మాణం libcrun_checkpoint_restore_options_s
{
  size_t struct_కొలత; /* set to sizeof (struct libcrun_checkpoint_restore_options_s) */

  స్థిరరాశి అక్షరం *image_మార్గం;
  స్థిరరాశి అక్షరం *work_మార్గం;
  స్థిరరాశి అక్షరం *మాతృ_మార్గం;
  స్థిరరాశి అక్షరం *console_socket;
  స్థిరరాశి అక్షరం *lsm_profile;
  స్థిరరాశి అక్షరం *lsm_mount_context;

  bool leave_running;
  bool tcp_established;
  bool tcp_close;
  bool ext_unix_sk;
  bool shell_పని;
  bool file_locks;
  bool pre_dump;
  bool detach;

  పూర్ణాంకం manage_cgroups_బహుళకం; /* enum libcrun_cr_cgroups_mode */
  పూర్ణాంకం జాలం_తాళం_పద్ధతి; /* enum libcrun_cr_network_lock_method */
};

పూర్ణాంకం libcrun_container_checkpoint (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *id,
                                  నిర్మాణం libcrun_checkpoint_restore_options_s *opts, libcrun_error_t *err);
పూర్ణాంకం libcrun_container_restore (libcrun_context_t *ctx, స్థిరరాశి అక్షరం *id,
                               నిర్మాణం libcrun_checkpoint_restore_options_s *opts, libcrun_error_t *err);

/* Feature tags contributed by the loaded handlers, as a NULL-terminated array of
   strings (e.g. "wasm:wasmedge").  Returns NULL if there are none.  Caller frees the
   array and each string with free().  */
అక్షరం **libcrun_context_get_handler_feature_ట్యాగులు (libcrun_context_t *ctx, libcrun_error_t *err);

#నిర్వచితమైతే __cplusplus
}
#అయితే_ముగింపు

#అయితే_ముగింపు
