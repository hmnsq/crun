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
#నిర్వచితం_కాకపోతే UTILS_H
#నిర్వచించు UTILS_H

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <errno.h>
#అన్తర్భావ <argp.h>
#అన్తర్భావ "error.h"
#అన్తర్భావ <dirent.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <signal.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <ocispec/runtime_spec_schema_config_schema.h>
#అన్తర్భావ <sys/wait.h>
#అన్తర్భావ "container.h"

#నిర్వచితం_కాకపోతే తాత్కాలిక_FAILURE_RETRY
#  define తాత్కాలిక_FAILURE_RETRY(వ్యక్తీకరణ)      \
    (__extension__ ({                         \
      దీర్ఘ పూర్ణాంకం __result;                      \
      చేయి                                      \
        __result = (దీర్ఘ పూర్ణాంకం) (వ్యక్తీకరణ);   \
      అయితే (__result < 0 && errno == EINTR); \
      __result;                               \
    }))
#అయితే_ముగింపు

#నిర్వచించు cleanup_file __attribute__ ((cleanup (cleanup_filep)))
#నిర్వచించు cleanup_free __attribute__ ((cleanup (cleanup_freep)))
#నిర్వచించు cleanup_close __attribute__ ((cleanup (cleanup_closep)))
#నిర్వచించు cleanup_close_vec __attribute__ ((cleanup (cleanup_close_vecp)))
#నిర్వచించు cleanup_close_map __attribute__ ((cleanup (cleanup_close_mapp)))
#నిర్వచించు cleanup_dir __attribute__ ((cleanup (cleanup_dirp)))
#నిర్వచించు arg_వాడని __attribute__ ((వాడని))
#నిర్వచించు cleanup_pid __attribute__ ((cleanup (cleanup_pidp)))
#నిర్వచించు cleanup_mmap __attribute__ ((cleanup (cleanup_mmapp)))

#నిర్వచించు LIKELY(x) __builtin_expect ((x), 1)
#నిర్వచించు UNLIKELY(x) __builtin_expect ((x), 0)

#నిర్వచించు WRITE_FILE_DEFAULT_FLAGS (O_CLOEXEC | O_CREAT | O_TRUNC | O_WRONLY)

__attribute__ ((జ్ఞాపకం_ఇవ్వు)) స్థిర అంతఃవరుస శూన్య *
xmalloc (size_t కొలత)
{
  శూన్య *res = జ్ఞాపకం_ఇవ్వు (కొలత);
  ఒకవేళ (UNLIKELY (res == ఖాళీ))
    OOM ();
  తిరిగి res;
}

__attribute__ ((జ్ఞాపకం_ఇవ్వు)) స్థిర అంతఃవరుస శూన్య *
xmalloc0 (size_t కొలత)
{
  శూన్య *res = సున్నా_జ్ఞాపకం_ఇవ్వు (1, కొలత);
  ఒకవేళ (UNLIKELY (res == ఖాళీ))
    OOM ();
  తిరిగి res;
}

__attribute__ ((జ్ఞాపకం_ఇవ్వు)) స్థిర అంతఃవరుస శూన్య *
xrealloc (శూన్య *ptr, size_t కొలత)
{
  శూన్య *res = realloc (ptr, కొలత);
  ఒకవేళ (UNLIKELY (res == ఖాళీ))
    OOM ();
  తిరిగి res;
}

స్థిర అంతఃవరుస శూన్య
cleanup_freep (శూన్య *p)
{
  శూన్య **pp = (శూన్య **) p;
  విడుదల (*pp);
}

స్థిర అంతఃవరుస శూన్య
cleanup_filep (FILE **f)
{
  FILE *file = *f;
  ఒకవేళ (file)
    (శూన్య) fclose (file);
}

స్థిర అంతఃవరుస శూన్య
cleanup_closep (శూన్య *p)
{
  పూర్ణాంకం *pp = (పూర్ణాంకం *) p;
  ఒకవేళ (*pp >= 0)
    తాత్కాలిక_FAILURE_RETRY (close (*pp));
}

స్థిర అంతఃవరుస శూన్య
cleanup_pidp (శూన్య *p)
{
  pid_t *pp = (pid_t *) p;
  ఒకవేళ (*pp > 0)
    {
      తాత్కాలిక_FAILURE_RETRY (kill (*pp, SIGKILL));
      తాత్కాలిక_FAILURE_RETRY (waitpid (*pp, ఖాళీ, 0));
    }
}

నిర్మాణం libcrun_mmap_s
{
  శూన్య *addr;
  size_t నిడివి;
};

పూర్ణాంకం libcrun_mmap (నిర్మాణం libcrun_mmap_s **ret, శూన్య *addr, size_t నిడివి,
                  పూర్ణాంకం prot, పూర్ణాంకం flags, పూర్ణాంకం fd, off_t స్థానభ్రంశం,
                  libcrun_error_t *err);

పూర్ణాంకం libcrun_munmap (నిర్మాణం libcrun_mmap_s *mmap, libcrun_error_t *err);

స్థిర అంతఃవరుస శూన్య
cleanup_mmapp (శూన్య *p)
{
  పూర్ణాంకం ret;
  libcrun_error_t tmp_err = ఖాళీ;
  నిర్మాణం libcrun_mmap_s **mm;

  mm = (నిర్మాణం libcrun_mmap_s **) p;
  ఒకవేళ (*mm == ఖాళీ)
    తిరిగి;

  ret = libcrun_munmap (*mm, &tmp_err);
  ఒకవేళ (UNLIKELY (ret < 0))
    crun_error_release (&tmp_err);
}

నిర్మాణం libcrun_fd_map
{
  size_t nfds;
  పూర్ణాంకం fds[];
};

స్థిర అంతఃవరుస నిర్మాణం libcrun_fd_map *
make_libcrun_fd_map (size_t len)
{
  నిర్మాణం libcrun_fd_map *ret;
  size_t i;

  ret = (నిర్మాణం libcrun_fd_map *) xmalloc (పరిమాణం (*ret) + పరిమాణం (పూర్ణాంకం) * len);
  ret->nfds = len;
  కొరకు (i = 0; i < len; i++)
    ret->fds[i] = -1;

  తిరిగి ret;
}

స్థిర అంతఃవరుస శూన్య
cleanup_close_mapp (నిర్మాణం libcrun_fd_map **p)
{
  నిర్మాణం libcrun_fd_map *m = *p;
  size_t i;

  ఒకవేళ (m == ఖాళీ)
    తిరిగి;

  కొరకు (i = 0; i < m->nfds; i++)
    ఒకవేళ (m->fds[i] >= 0)
      తాత్కాలిక_FAILURE_RETRY (close (m->fds[i]));

  విడుదల (m);
}

స్థిర అంతఃవరుస శూన్య
cleanup_close_vecp (పూర్ణాంకం **p)
{
  పూర్ణాంకం *pp = *p;
  పూర్ణాంకం i;

  ఒకవేళ (pp == ఖాళీ)
    తిరిగి;

  కొరకు (i = 0; pp[i] >= 0; i++)
    తాత్కాలిక_FAILURE_RETRY (close (pp[i]));
}

స్థిర అంతఃవరుస శూన్య
cleanup_dirp (DIR **p)
{
  DIR *dir = *p;
  ఒకవేళ (dir)
    closedir (dir);
}

స్థిర అంతఃవరుస పూర్ణాంకం
close_and_reset (పూర్ణాంకం *fd)
{
  పూర్ణాంకం ret = 0;
  ఒకవేళ (*fd >= 0)
    {
      ret = తాత్కాలిక_FAILURE_RETRY (close (*fd));
      ఒకవేళ (LIKELY (ret == 0))
        *fd = -1;
    }
  తిరిగి ret;
}

స్థిర అంతఃవరుస అక్షరం *
xstrdup (స్థిరరాశి అక్షరం *str)
{
  అక్షరం *ret;
  ఒకవేళ (str == ఖాళీ)
    తిరిగి ఖాళీ;

  ret = strdup (str);
  ఒకవేళ (ret == ఖాళీ)
    OOM ();

  తిరిగి ret;
}

శూన్య consume_trailing_slashes (అక్షరం *మార్గం);

#నిర్వచించు consume_slashes(t) \
  ({                       \
    typeof (t) _s = (t);   \
    అయితే (*_s == '/')     \
      _s++;                \
    _s;                    \
  })

స్థిర అంతఃవరుస bool
మార్గం_is_slash_dev (స్థిరరాశి అక్షరం *మార్గం)
{
  మార్గం = consume_slashes (మార్గం);

  ఒకవేళ (అక్షరశ్రేణి_పోలిక (మార్గం, "dev", 3))
    తిరిగి false;

  మార్గం += 3;

  /* Check there are only '/' left.  */
  కొరకు (; *మార్గం; మార్గం++)
    ఒకవేళ (*మార్గం != '/')
      తిరిగి false;

  తిరిగి true;
}

పూర్ణాంకం xasprintf (అక్షరం **str, స్థిరరాశి అక్షరం *fmt, ...) __attribute__ ((format (రాయి, 2, 3)));

పూర్ణాంకం crun_మార్గం_exists (స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err);

పూర్ణాంకం write_file_at_with_flags (పూర్ణాంకం dirfd, పూర్ణాంకం flags, mode_t బహుళకం, స్థిరరాశి అక్షరం *name, స్థిరరాశి శూన్య *దత్తాంశం, size_t len, libcrun_error_t *err);

స్థిర అంతఃవరుస పూర్ణాంకం
write_file (స్థిరరాశి అక్షరం *name, స్థిరరాశి శూన్య *దత్తాంశం, size_t len, libcrun_error_t *err)
{
  తిరిగి write_file_at_with_flags (AT_FDCWD, WRITE_FILE_DEFAULT_FLAGS, 0700, name, దత్తాంశం, len, err);
}

స్థిర అంతఃవరుస పూర్ణాంకం
write_file_at (పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *name, స్థిరరాశి శూన్య *దత్తాంశం, size_t len, libcrun_error_t *err)
{
  తిరిగి write_file_at_with_flags (dirfd, WRITE_FILE_DEFAULT_FLAGS, 0700, name, దత్తాంశం, len, err);
}

పూర్ణాంకం crun_ensure_directory (స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం బహుళకం, bool nofollow, libcrun_error_t *err);

పూర్ణాంకం crun_ensure_directory_at (పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం బహుళకం, bool nofollow, libcrun_error_t *err);

పూర్ణాంకం crun_సురక్షిత_create_and_open_ref_at (bool dir, పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *dirpath, స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం బహుళకం, libcrun_error_t *err);

పూర్ణాంకం crun_సురక్షిత_ensure_directory_at (పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *dirpath, స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం బహుళకం,
                                   libcrun_error_t *err);

పూర్ణాంకం crun_సురక్షిత_ensure_file_at (పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *dirpath, స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం బహుళకం,
                              libcrun_error_t *err);

పూర్ణాంకం crun_dir_p (స్థిరరాశి అక్షరం *మార్గం, bool nofollow, libcrun_error_t *err);

పూర్ణాంకం crun_dir_p_at (పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *మార్గం, bool nofollow, libcrun_error_t *err);

పూర్ణాంకం detach_process ();

పూర్ణాంకం create_file_if_missing_at (పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *file, mode_t బహుళకం, libcrun_error_t *err);

పూర్ణాంకం check_running_లోపల_user_namespace (libcrun_error_t *err);

పూర్ణాంకం సమితి_selinux_label (libcrun_container_t *container, స్థిరరాశి అక్షరం *label, bool now, libcrun_error_t *err);

పూర్ణాంకం కూడు_selinux_mount_label (అక్షరం **ret, స్థిరరాశి అక్షరం *దత్తాంశం, స్థిరరాశి అక్షరం *label, స్థిరరాశి అక్షరం *context_type, libcrun_error_t *err);

పూర్ణాంకం సమితి_apparmor_profile (libcrun_container_t *container, స్థిరరాశి అక్షరం *profile, bool no_new_privileges, bool now, libcrun_error_t *err);

పూర్ణాంకం read_all_fd_with_కొలత_hint (పూర్ణాంకం fd, స్థిరరాశి అక్షరం *description, అక్షరం **బహిర్గతం, size_t *len, size_t hint, libcrun_error_t *err);

పూర్ణాంకం read_all_fd_with_max_కొలత (పూర్ణాంకం fd, స్థిరరాశి అక్షరం *description, అక్షరం **బహిర్గతం, size_t *len, size_t max_కొలత, libcrun_error_t *err);

స్థిర అంతఃవరుస పూర్ణాంకం
read_all_fd (పూర్ణాంకం fd, స్థిరరాశి అక్షరం *description, అక్షరం **బహిర్గతం, size_t *len, libcrun_error_t *err)
{
  తిరిగి read_all_fd_with_కొలత_hint (fd, description, బహిర్గతం, len, 0, err);
}

పూర్ణాంకం get_realpath_గమ్యం_file (పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *మార్గం_name, అక్షరం **absolute_మార్గం, libcrun_error_t *err);

పూర్ణాంకం read_all_file (స్థిరరాశి అక్షరం *మార్గం, అక్షరం **బహిర్గతం, size_t *len, libcrun_error_t *err);

పూర్ణాంకం read_all_file_at (పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *మార్గం, అక్షరం **బహిర్గతం, size_t *len, libcrun_error_t *err);

పూర్ణాంకం open_unix_domain_client_socket (స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం dgram, libcrun_error_t *err);

పూర్ణాంకం open_unix_domain_socket (స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం dgram, libcrun_error_t *err);

పూర్ణాంకం send_fd_గమ్యం_socket (పూర్ణాంకం server, పూర్ణాంకం fd, libcrun_error_t *err);

పూర్ణాంకం send_fd_గమ్యం_socket_with_payload (పూర్ణాంకం server, పూర్ణాంకం fd, స్థిరరాశి అక్షరం *payload, size_t payload_len, libcrun_error_t *err);

పూర్ణాంకం create_socket_pair (పూర్ణాంకం *pair, libcrun_error_t *err);

పూర్ణాంకం receive_fd_from_socket (పూర్ణాంకం from, libcrun_error_t *err);

పూర్ణాంకం receive_fd_from_socket_with_payload (పూర్ణాంకం from, అక్షరం *payload, size_t payload_len, libcrun_error_t *err);

పూర్ణాంకం create_signalfd (sigset_t *ముసుగు, libcrun_error_t *err);

పూర్ణాంకం epoll_helper (పూర్ణాంకం *లోపల_fds, పూర్ణాంకం *లోపల_levelfds, పూర్ణాంకం *బహిర్గతం_fds, పూర్ణాంకం *బహిర్గతం_levelfds, libcrun_error_t *err);

పూర్ణాంకం copy_from_fd_గమ్యం_fd (పూర్ణాంకం src, పూర్ణాంకం dst, పూర్ణాంకం consume, libcrun_error_t *err);

పూర్ణాంకం నిరంతరం_process (అక్షరం **వాదనలు, libcrun_error_t *err);

పూర్ణాంకం format_default_id_mapping (అక్షరం **బహిర్గతం, uid_t container_id, uid_t host_uid, uid_t host_id, పూర్ణాంకం is_uid, libcrun_error_t *err);

పూర్ణాంకం నిరంతరం_process_with_stdin_timeout_envp (అక్షరం *మార్గం, అక్షరం **వాదనలు, స్థిరరాశి అక్షరం *cwd, పూర్ణాంకం timeout, అక్షరం **envp,
                                         అక్షరం *stdin, size_t stdin_len, పూర్ణాంకం బహిర్గతం_fd, పూర్ణాంకం err_fd, bool can_ignore_chdir_errors, libcrun_error_t *err);

పూర్ణాంకం mark_or_close_fds_ge_than (libcrun_container_t *container, పూర్ణాంకం n, bool close_now, libcrun_error_t *err);

శూన్య get_current_timestamp (అక్షరం *బహిర్గతం, size_t len);

పూర్ణాంకం సమితి_blocking_fd (పూర్ణాంకం fd, bool blocking, libcrun_error_t *err);

/* Exported for the language bindings (python/, lua/), which parse a
   process JSON blob before calling libcrun_container_exec.  */
LIBCRUN_ప్రజా పూర్ణాంకం parse_json_file (json_object **బహిర్గతం, స్థిరరాశి అక్షరం *jsondata, నిర్మాణం parser_context *ctx, libcrun_error_t *err);

/* Check that the string STR has the prefix PREFIX.  */
స్థిర అంతఃవరుస పూర్ణాంకం
has_ఉపసర్గ (స్థిరరాశి అక్షరం *str, స్థిరరాశి అక్షరం *ఉపసర్గ)
{
  size_t ఉపసర్గ_len;

  ఒకవేళ (! str || ! ఉపసర్గ)
    తిరిగి 0;

  ఉపసర్గ_len = అక్షరశ్రేణి_పొడవు (ఉపసర్గ);
  తిరిగి అక్షరశ్రేణి_పొడవు (str) >= ఉపసర్గ_len && memcmp (str, ఉపసర్గ, ఉపసర్గ_len) == 0;
}

/* Check that the string STR has the prefix PREFIX/ (followed by a slash).  */
స్థిర అంతఃవరుస పూర్ణాంకం
has_dir_ఉపసర్గ (స్థిరరాశి అక్షరం *str, స్థిరరాశి అక్షరం *ఉపసర్గ)
{
  size_t ఉపసర్గ_len;

  ఒకవేళ (! str || ! ఉపసర్గ)
    తిరిగి 0;

  ఉపసర్గ_len = అక్షరశ్రేణి_పొడవు (ఉపసర్గ);
  తిరిగి అక్షరశ్రేణి_పొడవు (str) > ఉపసర్గ_len && memcmp (str, ఉపసర్గ, ఉపసర్గ_len) == 0 && str[ఉపసర్గ_len] == '/';
}

పూర్ణాంకం కనుగొను_executable (అక్షరం **exec_మార్గం, స్థిరరాశి అక్షరం *executable_మార్గం, స్థిరరాశి అక్షరం *cwd, libcrun_error_t *err);

పూర్ణాంకం copy_recursive_fd_గమ్యం_fd (పూర్ణాంకం srcfd, పూర్ణాంకం destfd, స్థిరరాశి అక్షరం *srcname, స్థిరరాశి అక్షరం *destname, libcrun_error_t *err);

పూర్ణాంకం సమితి_home_env (uid_t uid);

పూర్ణాంకం libcrun_initialize_selinux (libcrun_container_t *container, libcrun_error_t *err);

పూర్ణాంకం libcrun_initialize_apparmor (libcrun_error_t *err);

స్థిరరాశి అక్షరం *కనుగొను_annotation (libcrun_container_t *container, స్థిరరాశి అక్షరం *name);

పూర్ణాంకం get_file_type_at (పూర్ణాంకం dirfd, mode_t *బహుళకం, bool nofollow, స్థిరరాశి అక్షరం *మార్గం);

పూర్ణాంకం get_file_type (mode_t *బహుళకం, bool nofollow, స్థిరరాశి అక్షరం *మార్గం);

పూర్ణాంకం get_file_type_fd (పూర్ణాంకం fd, mode_t *బహుళకం);

అక్షరం *get_user_name (uid_t uid);

పూర్ణాంకం సురక్షిత_openat (పూర్ణాంకం dirfd, స్థిరరాశి అక్షరం *rootfs, స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం flags, పూర్ణాంకం బహుళకం,
                 libcrun_error_t *err);

పూర్ణాంకం సురక్షిత_write (పూర్ణాంకం fd, స్థిరరాశి అక్షరం *fname, స్థిరరాశి శూన్య *buf, size_t లెక్క, libcrun_error_t *err);

పూర్ణాంకం append_paths (అక్షరం **బహిర్గతం, libcrun_error_t *err, ...) __attribute__ ((sentinel));

పూర్ణాంకం str2sig (స్థిరరాశి అక్షరం *name);

పూర్ణాంకం base64_decode (స్థిరరాశి అక్షరం *iptr, size_t isize, అక్షరం *optr, size_t osize, size_t *nbytes);
పూర్ణాంకం has_suffix (స్థిరరాశి అక్షరం *ఆధారం, స్థిరరాశి అక్షరం *suffix);
అక్షరం *libcrun_sysctl_కీలకం_గమ్యం_proc_మార్గం (స్థిరరాశి అక్షరం *కీలకం);
అక్షరం *str_join_array (పూర్ణాంకం స్థానభ్రంశం, size_t కొలత, అక్షరం *స్థిరరాశి array[], స్థిరరాశి అక్షరం *joint);

ssize_t సురక్షిత_readlinkat (పూర్ణాంకం dfd, స్థిరరాశి అక్షరం *name, అక్షరం **బఫరు, ssize_t hint, libcrun_error_t *err);

అక్షరం **read_dir_entries (స్థిరరాశి అక్షరం *మార్గం, libcrun_error_t *err);

స్థిర అంతఃవరుస bool
is_ఖాళీలు_string (స్థిరరాశి అక్షరం *s)
{
  తిరిగి s == ఖాళీ || s[0] == '\0';
}

/* Like strlen(), but a NULL string has length 0.  */
స్థిర అంతఃవరుస size_t
సురక్షిత_strlen (స్థిరరాశి అక్షరం *s)
{
  తిరిగి s == ఖాళీ ? 0 : అక్షరశ్రేణి_పొడవు (s);
}

స్థిర అంతఃవరుస bool
మార్గం_has_dot_dot_component (స్థిరరాశి అక్షరం *మార్గం)
{
  ఒకవేళ (మార్గం == ఖాళీ)
    తిరిగి false;
  cleanup_free అక్షరం *npath = xstrdup (మార్గం);
  అక్షరం *cur, *it;

  cur = npath;
  it = strchr (cur, '/');
  అయితే (cur)
    {
      ఒకవేళ (it)
        *it = '\0';

      ఒకవేళ (strcmp (cur, "..") == 0)
        తిరిగి true;

      ఒకవేళ (it == ఖాళీ)
        ఆపు;

      cur = consume_slashes (it + 1);
      *it = '/';
      it = strchr (cur, '/');
    }
  తిరిగి false;
}

స్థిర అంతఃవరుస పూర్ణాంకం
waitpid_ignore_stopped (pid_t pid, పూర్ణాంకం *స్థితి_2, పూర్ణాంకం options)
{
  పూర్ణాంకం ret, s;
  చేయి
    {
      s = 0;
      ret = తాత్కాలిక_FAILURE_RETRY (waitpid (pid, &s, options));
      ఒకవేళ (ret < 0)
        తిరిగి ret;
  } అయితే (WIFSTOPPED (s) || WIFCONTINUED (s));

  ఒకవేళ (స్థితి_2)
    *స్థితి_2 = s;

  తిరిగి ret;
}

స్థిర అంతఃవరుస పూర్ణాంకం
get_process_exit_స్థితి (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (WIFEXITED (స్థితి_2))
    తిరిగి WEXITSTATUS (స్థితి_2);
  ఒకవేళ (WIFSIGNALED (స్థితి_2))
    తిరిగి 128 + WTERMSIG (స్థితి_2);

  తిరిగి -1;
}

uid_t get_అధికప్రవాహం_uid (శూన్య);
gid_t get_అధికప్రవాహం_gid (శూన్య);

/* Adapted from systemd.  Include space for the NUL byte.  */
#నిర్వచించు DECIMAL_STR_MAX(type)                                        \
  ((size_t) 2U + (పరిమాణం (type) <= 1 ? 3U : పరిమాణం (type) <= 2 ? 5U  \
                                        : పరిమాణం (type) <= 4   ? 10U \
                                        : పరిమాణం (type) <= 8   ? 20U \
                                                               : పరిమాణం (పూర్ణాంకం[-2 * (పరిమాణం (type) > 8)])))

#నిర్వచించు _STRLEN(s) (పరిమాణం (s) - 1)

/* _STRLEN("self") < DECIMAL_STR_MAX (pid_t), so we don't need to calculate the length of both.  */
#నిర్వచించు PROC_PID_FD_STRLEN (_STRLEN ("/proc/") + DECIMAL_STR_MAX (pid_t) \
                            + _STRLEN ("/fd/") + DECIMAL_STR_MAX (పూర్ణాంకం))

/* A buffer long enough to hold either /proc/self/fd/$FD or a /proc/$PID/fd/$FD path.  */
రకనిర్వచనం అక్షరం proc_fd_మార్గం_t[PROC_PID_FD_STRLEN];

#నిర్వచనం_రద్దు _STRLEN

స్థిర అంతఃవరుస శూన్య
get_proc_fd_మార్గం (proc_fd_మార్గం_t మార్గం, pid_t pid, పూర్ణాంకం fd)
{
  స్థిరరాశి size_t max_len = పరిమాణం (proc_fd_మార్గం_t);
  size_t n;

  ఒకవేళ (pid)
    n = snprintf (మార్గం, max_len, "/proc/%d/fd/%d", pid, fd);
  లేకపోతే
    n = snprintf (మార్గం, max_len, "/proc/self/fd/%d", fd);

  ఒకవేళ (UNLIKELY (n >= max_len))
    abort ();
}

స్థిర అంతఃవరుస శూన్య
get_proc_self_fd_మార్గం (proc_fd_మార్గం_t మార్గం, పూర్ణాంకం fd)
{
  get_proc_fd_మార్గం (మార్గం, 0, fd);
}

స్థిర అంతఃవరుస శూన్య
get_self_fd_మార్గం (proc_fd_మార్గం_t మార్గం, పూర్ణాంకం fd)
{
  snprintf (మార్గం, పరిమాణం (proc_fd_మార్గం_t), "self/fd/%d", fd);
}

స్థిర అంతఃవరుస పూర్ణాంకం
validate_options (గుర్తులేని పూర్ణాంకం specified_options, గుర్తులేని పూర్ణాంకం supported_options, libcrun_error_t *err)
{
  ఒకవేళ (! ! (~supported_options & specified_options))
    తిరిగి crun_make_error (err, 0, "internal error: unknown options %d", specified_options);
  తిరిగి 0;
}

బాహ్య పూర్ణాంకం cpuset_string_గమ్యం_bitmask (స్థిరరాశి అక్షరం *str, అక్షరం **బహిర్గతం, size_t *బహిర్గతం_కొలత, libcrun_error_t *err);

/*
 * A channel_fd_pair takes care of copying data between two file descriptors.
 * The two file descriptors are expected to be set to non-blocking mode.
 * The channel_fd_pair will buffer data read from the input file descriptor and
 * write it to the output file descriptor.  If the output file descriptor is not
 * ready to accept the data, the channel_fd_pair will buffer the data until it
 * can be written.
 */
నిర్మాణం channel_fd_pair;

నిర్మాణం channel_fd_pair *channel_fd_pair_new (పూర్ణాంకం లోపల_fd, పూర్ణాంకం బహిర్గతం_fd, size_t కొలత);

శూన్య channel_fd_pair_free (నిర్మాణం channel_fd_pair *channel);

/* Process the data in the channel_fd_pair.  This function will read data from
 * the input file descriptor and write it to the output file descriptor.  If
 * the output file descriptor is not ready to accept the data, the data will be
 * buffered.  If epollfd is provided, the in_fd and out_fd will be registered
 * and unregistered as necessary.
 */
పూర్ణాంకం channel_fd_pair_process (నిర్మాణం channel_fd_pair *channel, పూర్ణాంకం epollfd, libcrun_error_t *err);

స్థిర అంతఃవరుస శూన్య
cleanup_channel_fd_pairp (శూన్య *p)
{
  నిర్మాణం channel_fd_pair **pp = (నిర్మాణం channel_fd_pair **) p;
  ఒకవేళ (*pp == ఖాళీ)
    తిరిగి;

  channel_fd_pair_free (*pp);
}
#నిర్వచించు cleanup_channel_fd_pair __attribute__ ((cleanup (cleanup_channel_fd_pairp)))

పూర్ణాంకం libcrun_get_cached_proc_fd (libcrun_container_t *container, libcrun_error_t *err);
పూర్ణాంకం libcrun_open_proc_file (libcrun_container_t *container, స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం flags, libcrun_error_t *err);
పూర్ణాంకం libcrun_open_proc_pid_file (libcrun_container_t *container, pid_t pid, స్థిరరాశి అక్షరం *మార్గం, పూర్ణాంకం flags, libcrun_error_t *err);

#అయితే_ముగింపు
