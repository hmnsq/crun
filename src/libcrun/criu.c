/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2020 Adrian Reber <areber@redhat.com>
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
#define _GNU_SOURCE

#ಅನ್ತರ್ಭಾವ <config.h>

#ಯದಿ HAVE_CRIU && HAVE_DLOPEN

#  include <unistd.h>
#  include <sys/types.h>
#  include <criu/criu.h>
#  include <sched.h>
#  include <signal.h>
#  include <sys/stat.h>
#  include <sys/mount.h>
#  include <fcntl.h>

#  include "container.h"
#  include "linux.h"
#  include "status.h"
#  include "utils.h"
#  include "cgroup.h"
#  include "cgroup-utils.h"
#  include "mount_flags.h"

#  ifndef STATIC
#    include <dlfcn.h>
#  endif

#  define CRIU_CHECKPOINT_LOG_FILE "dump.log"
#  define CRIU_RESTORE_LOG_FILE "restore.log"
#  define DESCRIPTORS_FILENAME "descriptors.json"
#  define CRIU_RUNC_CONFIG_FILE "/etc/criu/runc.conf"
#  define CRIU_CRUN_CONFIG_FILE "/etc/criu/crun.conf"

#  define CRIU_EXT_NETNS "extRootNetNS"
#  define CRIU_EXT_PIDNS "extRootPidNS"

#  ifndef CLONE_NEWTIME
#    define CLONE_NEWTIME 0x00000080 /* New time namespace */
#  endif

/* Defined in chroot_realpath.c  */
ವರ್ಣ *chroot_realpath (ಧ್ರುವ ವರ್ಣ *chroot, ಧ್ರುವ ವರ್ಣ *path, ವರ್ಣ resolved_path[]);

/* Map the public cgroups mode (enum libcrun_cr_cgroups_mode) onto the CRIU enum.  */
ಸ್ಥಿರ ಗಣನಾ criu_cg_mode
criu_cg_mode_from_public (ಪೂರ್ಣಾಙ್ಕ mode)
{
  ವಿಕಲ್ಪ (mode)
    {
    ಪಕ್ಷ LIBCRUN_CR_CG_MODE_IGNORE:
      ಪ್ರತ್ಯಾಗಮ CRIU_CG_MODE_IGNORE;
    ಪಕ್ಷ LIBCRUN_CR_CG_MODE_FULL:
      ಪ್ರತ್ಯಾಗಮ CRIU_CG_MODE_FULL;
    ಪಕ್ಷ LIBCRUN_CR_CG_MODE_STRICT:
      ಪ್ರತ್ಯಾಗಮ CRIU_CG_MODE_STRICT;
    ಪಕ್ಷ LIBCRUN_CR_CG_MODE_DEFAULT:
    ಪಕ್ಷ LIBCRUN_CR_CG_MODE_SOFT:
    ಸ್ವಭಾವ:
      /* Default to CRIU_CG_MODE_SOFT, just as runc.  */
      ಪ್ರತ್ಯಾಗಮ CRIU_CG_MODE_SOFT;
    }
}

/* Map the public network lock method (enum libcrun_cr_network_lock_method) onto the
   CRIU enum.  *LOCK is set to 0 when no explicit method must be set (CRIU default).
   A method that this build cannot honor is an error, never a silent fallback to
   the CRIU default.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
criu_network_lock_from_public (ಪೂರ್ಣಾಙ್ಕ method, ಪೂರ್ಣಾಙ್ಕ *lock, libcrun_error_t *err)
{
  ವಿಕಲ್ಪ (method)
    {
    ಪಕ್ಷ LIBCRUN_CR_NETWORK_LOCK_IPTABLES:
      *lock = CRIU_NETWORK_LOCK_IPTABLES;
      ಪ್ರತ್ಯಾಗಮ 0;
    ಪಕ್ಷ LIBCRUN_CR_NETWORK_LOCK_NFTABLES:
      *lock = CRIU_NETWORK_LOCK_NFTABLES;
      ಪ್ರತ್ಯಾಗಮ 0;
    ಪಕ್ಷ LIBCRUN_CR_NETWORK_LOCK_SKIP:
#  ಯದಿ CRIU_NETWORK_LOCK_SKIP_SUPPORT
      *lock = CRIU_NETWORK_LOCK_SKIP;
      ಪ್ರತ್ಯಾಗಮ 0;
#  ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "CRIU: the `skip` network lock method is not supported by this build");
#  endif
    ಪಕ್ಷ LIBCRUN_CR_NETWORK_LOCK_DEFAULT:
      *lock = 0;
      ಪ್ರತ್ಯಾಗಮ 0;
    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "CRIU: unknown network lock method %d", method);
    }
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *console_socket = ರಿಕ್ತ;

/* The external mounts CRIU restores, and the flags the configuration asks
   for each of them.  */
ಸ್ಥಿರ ಸಂರಚನಾ libcrun_mount_flags_s *restored_ext_mounts = ರಿಕ್ತ;
ಸ್ಥಿರ size_t n_restored_ext_mounts = 0;

#  define LIBCRIU_MIN_VERSION 31500

ಸಂರಚನಾ libcriu_wrapper_s
{
  ಶೂನ್ಯ *handle;
  ಪೂರ್ಣಾಙ್ಕ (*criu_add_ext_mount) (ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ, ಧ್ರುವ ವರ್ಣ *val);
  ಪೂರ್ಣಾಙ್ಕ (*criu_add_external) (ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ);
  ಪೂರ್ಣಾಙ್ಕ (*criu_add_inherit_fd) (ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ);
  ಪೂರ್ಣಾಙ್ಕ (*criu_check_version) (ಪೂರ್ಣಾಙ್ಕ minimum);
  ಪೂರ್ಣಾಙ್ಕ (*criu_dump) (ಶೂನ್ಯ);
  ಪೂರ್ಣಾಙ್ಕ (*criu_get_orphan_pts_master_fd) (ಶೂನ್ಯ);
  ಪೂರ್ಣಾಙ್ಕ (*criu_init_opts) (ಶೂನ್ಯ);
  ಪೂರ್ಣಾಙ್ಕ (*criu_notify_pid) (criu_notify_arg_t na);
#  ifdef CRIU_JOIN_NS_SUPPORT
  ಪೂರ್ಣಾಙ್ಕ (*criu_join_ns_add) (ಧ್ರುವ ವರ್ಣ *ns, ಧ್ರುವ ವರ್ಣ *ns_file, ಧ್ರುವ ವರ್ಣ *extra_opt);
#  endif
#  ifdef CRIU_PRE_DUMP_SUPPORT
  ಪೂರ್ಣಾಙ್ಕ (*criu_feature_check) (ಸಂರಚನಾ criu_feature_check *features, size_t size);
  ಪೂರ್ಣಾಙ್ಕ (*criu_pre_dump) (ಶೂನ್ಯ);
#  endif
  ಪೂರ್ಣಾಙ್ಕ (*criu_restore_child) (ಶೂನ್ಯ);
  ಪೂರ್ಣಾಙ್ಕ (*criu_set_freeze_cgroup) (ಧ್ರುವ ವರ್ಣ *name);
  ಶೂನ್ಯ (*criu_set_file_locks) (bool file_locks);
  ಶೂನ್ಯ (*criu_set_ext_unix_sk) (bool ext_unix_sk);
  ಪೂರ್ಣಾಙ್ಕ (*criu_set_log_file) (ಧ್ರುವ ವರ್ಣ *log_file);
  ಶೂನ್ಯ (*criu_set_log_level) (ಪೂರ್ಣಾಙ್ಕ log_level);
  ಶೂನ್ಯ (*criu_set_leave_running) (bool leave_running);
  ಶೂನ್ಯ (*criu_set_manage_cgroups) (bool manage);
  ಶೂನ್ಯ (*criu_set_manage_cgroups_mode) (ಗಣನಾ criu_cg_mode mode);
  ಪೂರ್ಣಾಙ್ಕ (*criu_set_network_lock) (ಗಣನಾ criu_network_lock_method method);
  ಶೂನ್ಯ (*criu_set_notify_cb) (ಪೂರ್ಣಾಙ್ಕ (*cb) (ವರ್ಣ *action, criu_notify_arg_t na));
  ಶೂನ್ಯ (*criu_set_orphan_pts_master) (bool orphan_pts_master);
  ಶೂನ್ಯ (*criu_set_images_dir_fd) (ಪೂರ್ಣಾಙ್ಕ fd);
  ಪೂರ್ಣಾಙ್ಕ (*criu_set_parent_images) (ಧ್ರುವ ವರ್ಣ *path);
  ಶೂನ್ಯ (*criu_set_pid) (ಪೂರ್ಣಾಙ್ಕ pid);
  ಪೂರ್ಣಾಙ್ಕ (*criu_set_ಬೇರು) (ಧ್ರುವ ವರ್ಣ *ಬೇರು);
  ಪೂರ್ಣಾಙ್ಕ (*criu_add_cg_ಬೇರು) (ಧ್ರುವ ವರ್ಣ *ctrl, ಧ್ರುವ ವರ್ಣ *path);
  ಶೂನ್ಯ (*criu_set_shell_job) (bool shell_job);
  ಶೂನ್ಯ (*criu_set_tcp_established) (bool tcp_established);
  ಶೂನ್ಯ (*criu_set_tcp_close) (bool tcp_close);
  ಶೂನ್ಯ (*criu_set_track_mem) (bool track_mem);
  ಶೂನ್ಯ (*criu_set_work_dir_fd) (ಪೂರ್ಣಾಙ್ಕ fd);
  ಪೂರ್ಣಾಙ್ಕ (*criu_set_lsm_profile) (ಧ್ರುವ ವರ್ಣ *name);
  ಪೂರ್ಣಾಙ್ಕ (*criu_set_lsm_mount_context) (ಧ್ರುವ ವರ್ಣ *name);
  ಪೂರ್ಣಾಙ್ಕ (*criu_set_config_file) (ಧ್ರುವ ವರ್ಣ *path);
};

ಸ್ಥಿರ ಸಂರಚನಾ libcriu_wrapper_s *libcriu_wrapper;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
cleanup_wrapper (ಶೂನ್ಯ *p)
{
  ಸಂರಚನಾ libcriu_wrapper_s **w;

  w = (ಸಂರಚನಾ libcriu_wrapper_s **) p;
  ಯದಿ (*w == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ;

#  ifndef STATIC
  ಯದಿ ((*w)->handle)
    dlclose ((*w)->handle);
#  endif
  ಮುಕ್ತ (*w);
  libcriu_wrapper = ರಿಕ್ತ;

  ಯಾವತ್ (n_restored_ext_mounts > 0)
    ಮುಕ್ತ (restored_ext_mounts[--n_restored_ext_mounts].target);
  ಮುಕ್ತ (restored_ext_mounts);
  restored_ext_mounts = ರಿಕ್ತ;
}

#  define cleanup_wrapper __attribute__ ((cleanup (cleanup_wrapper)))

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
load_wrapper (ಸಂರಚನಾ libcriu_wrapper_s **wrapper_out, libcrun_error_t *err)
{
  cleanup_free ಸಂರಚನಾ libcriu_wrapper_s *wrapper = xmalloc0 (ಪರಿಮಾಣ (*wrapper));

#  ifdef STATIC
#    define LOAD_CRIU_FUNCTION(X, ALLOW_NULL) \
      wrapper->X = &X;
#  ಅನ್ಯಥಾ
#    define LOAD_CRIU_FUNCTION(X, ALLOW_NULL)                                                    \
      ಕುರು                                                                                         \
        {                                                                                        \
          wrapper->X = dlsym (wrapper->handle, #X);                                              \
          ಯದಿ (! ALLOW_NULL && wrapper->X == ರಿಕ್ತ)                                                \
            {                                                                                    \
              dlclose (wrapper->handle);                                                         \
              ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not find symbol `%s` in `libcriu.so`", #X); \
            }                                                                                    \
      } ಯಾವತ್ (0)
#  endif

#  ifndef STATIC
  wrapper->handle = dlopen ("libcriu.so.2", RTLD_NOW);
  ಯದಿ (wrapper->handle == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not load `libcriu.so.2`: `%s`", dlerror ());
#  endif

  LOAD_CRIU_FUNCTION (criu_add_ext_mount, false);
  LOAD_CRIU_FUNCTION (criu_add_external, false);
  LOAD_CRIU_FUNCTION (criu_add_inherit_fd, false);
  LOAD_CRIU_FUNCTION (criu_check_version, false);
  LOAD_CRIU_FUNCTION (criu_dump, false);
  LOAD_CRIU_FUNCTION (criu_get_orphan_pts_master_fd, false);
  LOAD_CRIU_FUNCTION (criu_init_opts, false);
  LOAD_CRIU_FUNCTION (criu_notify_pid, false);

#  ifdef CRIU_JOIN_NS_SUPPORT
  /* criu_join_ns_add() API was introduced with CRIU version 3.16.1
   * Here we check if this API is available at build time to support
   * compiling with older version of CRIU, and at runtime to support
   * running crun with older versions of libcriu.so.2.
   */
  LOAD_CRIU_FUNCTION (criu_join_ns_add, true);
#  endif

#  ifdef CRIU_PRE_DUMP_SUPPORT
  LOAD_CRIU_FUNCTION (criu_feature_check, false);
  LOAD_CRIU_FUNCTION (criu_pre_dump, false);
#  endif
  LOAD_CRIU_FUNCTION (criu_restore_child, false);
  LOAD_CRIU_FUNCTION (criu_set_ext_unix_sk, false);
  LOAD_CRIU_FUNCTION (criu_set_file_locks, false);
  LOAD_CRIU_FUNCTION (criu_set_freeze_cgroup, false);
  LOAD_CRIU_FUNCTION (criu_set_images_dir_fd, false);
  LOAD_CRIU_FUNCTION (criu_set_leave_running, false);
  LOAD_CRIU_FUNCTION (criu_set_log_file, false);
  LOAD_CRIU_FUNCTION (criu_set_log_level, false);
  LOAD_CRIU_FUNCTION (criu_set_manage_cgroups, false);
  LOAD_CRIU_FUNCTION (criu_set_manage_cgroups_mode, false);
  LOAD_CRIU_FUNCTION (criu_set_network_lock, true);
  LOAD_CRIU_FUNCTION (criu_set_notify_cb, false);
  LOAD_CRIU_FUNCTION (criu_set_orphan_pts_master, false);
  LOAD_CRIU_FUNCTION (criu_set_parent_images, false);
  LOAD_CRIU_FUNCTION (criu_set_pid, false);
  LOAD_CRIU_FUNCTION (criu_set_ಬೇರು, false);
  LOAD_CRIU_FUNCTION (criu_add_cg_ಬೇರು, false);
  LOAD_CRIU_FUNCTION (criu_set_shell_job, false);
  LOAD_CRIU_FUNCTION (criu_set_tcp_established, false);
  LOAD_CRIU_FUNCTION (criu_set_tcp_close, false);
  LOAD_CRIU_FUNCTION (criu_set_track_mem, false);
  LOAD_CRIU_FUNCTION (criu_set_work_dir_fd, false);
  LOAD_CRIU_FUNCTION (criu_set_lsm_profile, false);
  LOAD_CRIU_FUNCTION (criu_set_lsm_mount_context, false);
#  ಯದಿ ! defined STATIC || defined CRIU_CONFIG_FILE
  LOAD_CRIU_FUNCTION (criu_set_config_file, true);
#  endif

  libcriu_wrapper = *wrapper_out = wrapper;
  wrapper = ರಿಕ್ತ;
#  undef LOAD_CRIU_FUNCTION
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ
add_restored_ext_mount (ಧ್ರುವ ವರ್ಣ *target, unsigned long flags)
{
  restored_ext_mounts = xrealloc (restored_ext_mounts, ಪರಿಮಾಣ (*restored_ext_mounts) * (n_restored_ext_mounts + 1));
  restored_ext_mounts[n_restored_ext_mounts].target = xstrdup (target);
  restored_ext_mounts[n_restored_ext_mounts].flags = flags;
  n_restored_ext_mounts++;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
criu_notify (ವರ್ಣ *action, criu_notify_arg_t na)
{
  ಯದಿ (strcmp (action, "post-restore") == 0)
    {
      /* CRIU takes the source of an external mount from us and its flags
       * from the image, i.e. from whatever mount was at that path in the
       * container when it was checkpointed.  The container can move its
       * mount points around, so before it is resumed make sure that each
       * source is mounted with the restrictions the configuration asks for. */
      libcrun_error_t tmp_err = ರಿಕ್ತ;
      ಪೂರ್ಣಾಙ್ಕ ret;

      ret = libcrun_restrict_mounts_in_container (libcriu_wrapper->criu_notify_pid (na), restored_ext_mounts,
                                                  n_restored_ext_mounts, &tmp_err);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          libcrun_error (tmp_err->status, "cannot set the flags of the restored mounts: %s", tmp_err->ಸಂದೇಶ);
          libcrun_error_release (&tmp_err);
          ಪ್ರತ್ಯಾಗಮ ret;
        }
    }

  ಯದಿ (strncmp (action, "orphan-pts-master", 17) == 0)
    {
      /* CRIU sends us the master FD via the 'orphan-pts-master'
       * callback and we are passing it on to the '--console-socket'
       * if it exists. */
      cleanup_close ಪೂರ್ಣಾಙ್ಕ console_socket_fd = -1;
      libcrun_error_t tmp_err = ರಿಕ್ತ;
      ಪೂರ್ಣಾಙ್ಕ master_fd;
      ಪೂರ್ಣಾಙ್ಕ ret;

      ಯದಿ (! console_socket)
        ಪ್ರತ್ಯಾಗಮ 0;

      master_fd = libcriu_wrapper->criu_get_orphan_pts_master_fd ();

      console_socket_fd = open_unix_domain_client_socket (console_socket, 0, &tmp_err);
      ಯದಿ (UNLIKELY (console_socket_fd < 0))
        {
          libcrun_error_release (&tmp_err);
          ಪ್ರತ್ಯಾಗಮ console_socket_fd;
        }
      ret = send_fd_to_socket (console_socket_fd, master_fd, &tmp_err);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          libcrun_error_release (&tmp_err);
          ಪ್ರತ್ಯಾಗಮ ret;
        }
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

#  ifdef CRIU_PRE_DUMP_SUPPORT

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
criu_check_mem_track (libcrun_error_t *err)
{
  ಸಂರಚನಾ criu_feature_check features = { 0 };
  ಪೂರ್ಣಾಙ್ಕ ret;

  /* Right now we are only interested in checking memory tracking.
   * Memory tracking can be disabled at different levels. aarch64
   * for example has memory tracking not implemented. It could also
   * be not enabled on other architectures. Just ask CRIU if that
   * features exists. */

  features.mem_track = true;

  ret = libcriu_wrapper->criu_feature_check (&features, ಪರಿಮಾಣ (features));
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "CRIU feature checking failed: %d", ret);

  ಯದಿ (features.mem_track == true)
    ಪ್ರತ್ಯಾಗಮ 1;

  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "CRIU memory tracking not supported");
}

#  endif

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
register_masked_paths_mounts (runtime_spec_schema_config_schema *def, libcrun_container_t *container,
                              ಸಂರಚನಾ libcriu_wrapper_s *libcriu_wrapper, bool is_restore, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *empty_dir_path = ರಿಕ್ತ;
  bool shared_dir_registered = false;
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಪ್ರತಿ (i = 0; i < def->linux->masked_paths_len; i++)
    {
      ಸಂರಚನಾ stat statbuf;
      ret = stat (def->linux->masked_paths[i], &statbuf);
      ಯದಿ (ret != 0)
        ಅನುವರ್ತ;

      ಯದಿ (S_ISDIR (statbuf.st_mode))
        {
          ಯದಿ (! shared_dir_registered)
            {
              ret = get_shared_empty_directory_path (&empty_dir_path,
                                                     (container->context ? container->context->state_ಬೇರು : ರಿಕ್ತ), err);
              ಯದಿ (UNLIKELY (ret < 0))
                ಪ್ರತ್ಯಾಗಮ ret;

              ret = libcriu_wrapper->criu_add_ext_mount (empty_dir_path, empty_dir_path);
              ಯದಿ (UNLIKELY (ret < 0))
                ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "CRIU: failed adding external mount for shared empty directory `%s`", empty_dir_path);

              shared_dir_registered = true;
            }

          ret = libcriu_wrapper->criu_add_ext_mount (def->linux->masked_paths[i], empty_dir_path);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "CRIU: failed adding external mount for masked directory `%s`", def->linux->masked_paths[i]);

          ಯದಿ (is_restore)
            add_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
      ಅನ್ಯಥಾ ಯದಿ (S_ISREG (statbuf.st_mode))
        {
          ಧ್ರುವ ವರ್ಣ *bind_target = is_restore ? "/dev/null" : def->linux->masked_paths[i];
          ret = libcriu_wrapper->criu_add_ext_mount (def->linux->masked_paths[i], bind_target);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", bind_target);

          ಯದಿ (is_restore)
            add_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Parse one /proc/self/cgroup LINE into its subsystem, normalizing the name and
   stripping any "name=" prefix.  When SUBPATH_OUT is not NULL, the in-hierarchy
   path is stored there.  Returns the subsystem, or NULL if the line carries no
   controller and should be skipped.  LINE is modified in place.  */
ಸ್ಥಿರ ವರ್ಣ *
parse_cgroup_subsystem (ವರ್ಣ *ಸಾಲು, ವರ್ಣ **subpath_out)
{
  ವರ್ಣ *subsystem, *subpath, *it;

  subsystem = strchr (ಸಾಲು, ':') + 1;
  subpath = strchr (subsystem, ':') + 1;
  *(subpath - 1) = '\0';

  ಯದಿ (subsystem[0] == '\0')
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

  it = strstr (subsystem, "name=");
  ಯದಿ (it)
    subsystem = it + 5;

  ಯದಿ (strcmp (subsystem, "net_prio,net_cls") == 0)
    subsystem = "net_cls,net_prio";
  ಯದಿ (strcmp (subsystem, "cpuacct,cpu") == 0)
    subsystem = "cpu,cpuacct";

  ಯದಿ (subpath_out)
    *subpath_out = subpath;
  ಪ್ರತ್ಯಾಗಮ subsystem;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
restore_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *content = ರಿಕ್ತ;
  bool has_cgroup_mount = false;
  ವರ್ಣ *saveptr = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ cgroup_mode;
  ವರ್ಣ *from;
  ಪೂರ್ಣಾಙ್ಕ ret;
  uint32_t i;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (UNLIKELY (cgroup_mode < 0))
    ಪ್ರತ್ಯಾಗಮ cgroup_mode;

  ಯದಿ (cgroup_mode == CGROUP_MODE_UNIFIED)
    ಪ್ರತ್ಯಾಗಮ 0;

  /* First check if there is actually a cgroup mount in the container. */
  ಪ್ರತಿ (i = 0; i < def->mounts_len; i++)
    {
      ವರ್ಣ *type = def->mounts[i]->type;
      ಯದಿ (type && strcmp (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          ಭಙ್ಗ;
        }
    }

  ಯದಿ (! has_cgroup_mount)
    ಪ್ರತ್ಯಾಗಮ 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, ರಿಕ್ತ, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (UNLIKELY (content == ರಿಕ್ತ || content[0] == '\0'))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  ಪ್ರತಿ (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ರಿಕ್ತ, "\n", &saveptr))
    {
      cleanup_free ವರ್ಣ *destination = ರಿಕ್ತ;
      cleanup_free ವರ್ಣ *source = ರಿಕ್ತ;
      ವರ್ಣ *subsystem;
      ವರ್ಣ *subpath;

      subsystem = parse_cgroup_subsystem (from, &subpath);
      ಯದಿ (subsystem == ರಿಕ್ತ)
        ಅನುವರ್ತ;

      ret = append_paths (&source, err, CGROUP_ಬೇರು, subsystem, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = append_paths (&destination, err, source, subpath, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = libcriu_wrapper->criu_add_ext_mount (source, destination);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", destination);
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
checkpoint_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *content = ರಿಕ್ತ;
  bool has_cgroup_mount = false;
  ವರ್ಣ *saveptr = ರಿಕ್ತ;
  ವರ್ಣ *from;
  ಪೂರ್ಣಾಙ್ಕ ret;
  uint32_t i;

  /* First check if there is actually a cgroup mount in the container. */
  ಪ್ರತಿ (i = 0; i < def->mounts_len; i++)
    {
      ವರ್ಣ *type = def->mounts[i]->type;
      ಯದಿ (type && strcmp (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          ಭಙ್ಗ;
        }
    }

  ಯದಿ (! has_cgroup_mount)
    ಪ್ರತ್ಯಾಗಮ 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, ರಿಕ್ತ, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (UNLIKELY (content == ರಿಕ್ತ || content[0] == '\0'))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  ಪ್ರತಿ (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ರಿಕ್ತ, "\n", &saveptr))
    {
      cleanup_free ವರ್ಣ *source_path = ರಿಕ್ತ;
      ವರ್ಣ *subsystem;

      subsystem = parse_cgroup_subsystem (from, ರಿಕ್ತ);
      ಯದಿ (subsystem == ರಿಕ್ತ)
        ಅನುವರ್ತ;

      ret = append_paths (&source_path, err, CGROUP_ಬೇರು, subsystem, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = libcriu_wrapper->criu_add_ext_mount (source_path, source_path);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", source_path);
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
handle_criu_config_file (libcrun_container_t *container, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಧ್ರುವ ವರ್ಣ *criu_config_annotation;
  ಧ್ರುವ ವರ್ಣ *config_file = CRIU_RUNC_CONFIG_FILE;

  criu_config_annotation = find_annotation (container, "org.criu.config");

  /* Ignore missing criu_set_config_file() API for compatibility with older CRIU versions,
   * and show an error only if config file is explicitly set with annotation.
   */
  ಯದಿ (libcriu_wrapper->criu_set_config_file == ರಿಕ್ತ)
    {
      ಯದಿ (criu_config_annotation)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "libcriu RPC config files supported in CRIU >= 4.2");

      ಪ್ರತ್ಯಾಗಮ 0;
    }

  ಯದಿ (criu_config_annotation)
    config_file = criu_config_annotation;
  ಅನ್ಯಥಾ ಯದಿ (access (CRIU_CRUN_CONFIG_FILE, F_OK) == 0)
    config_file = CRIU_CRUN_CONFIG_FILE;

  ret = libcriu_wrapper->criu_set_config_file (config_file);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "failed to set CRIU config file");

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
validate_criu_version (libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  // validate that the libcriu version is at least LIBCRIU_MIN_VERSION
  ret = libcriu_wrapper->criu_check_version (LIBCRIU_MIN_VERSION);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "CRIU: failed checking version");

  ಯದಿ (ret == 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "libcriu is too old");

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ
show_criu_log (ಧ್ರುವ ವರ್ಣ *work_path, ಧ್ರುವ ವರ್ಣ *log)
{
  cleanup_free ವರ್ಣ *log_path = ರಿಕ್ತ;
  libcrun_error_t tmp_err = ರಿಕ್ತ;
  ವರ್ಣ ಸಾಲು[1024];
  FILE *f;

  ಯದಿ (UNLIKELY (append_paths (&log_path, &tmp_err, work_path, log, ರಿಕ್ತ) < 0))
    {
      crun_error_release (&tmp_err);
      ಪ್ರತ್ಯಾಗಮ;
    }

  f = fopen (log_path, "r");
  ಯದಿ (f == ರಿಕ್ತ)
    {
      ಯದಿ (errno != ENOENT)
        libcrun_error (errno, "Can't open CRIU log `%s`", log_path);
      ಪ್ರತ್ಯಾಗಮ;
    }

  /* Log with error verbosity as this is the default. */
  libcrun_error (0, "--- excerpt from CRIU log `%s`", log_path);
  ಯಾವತ್ (fgets (ಸಾಲು, ಪರಿಮಾಣ (ಸಾಲು), f) != ರಿಕ್ತ)
    ಯದಿ (strstr (ಸಾಲು, "Error ") != ರಿಕ್ತ)
      {
        ಸಾಲು[strcspn (ಸಾಲು, "\n")] = '\0';
        libcrun_error (0, "%s", ಸಾಲು);
      }

  fclose (f);
  libcrun_error (0, "--- end of excerpt");
}

/* work_dir is the place CRIU will put its logfiles.  If not explicitly set, CRIU
 * will put the logfiles into the images_dir.  When set, create and open it and
 * hand the fd to CRIU; ownership of the returned fd stays with the caller.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
setup_criu_work_dir (libcrun_checkpoint_restore_t *cr_options, ಪೂರ್ಣಾಙ್ಕ *work_fd_out, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ work_fd;

  ಯದಿ (cr_options->work_path == ರಿಕ್ತ)
    {
      /* This is only for the error message later. */
      cr_options->work_path = cr_options->image_path;
      ಪ್ರತ್ಯಾಗಮ 0;
    }

  ಯದಿ (UNLIKELY ((mkdir (cr_options->work_path, 0700) == -1) && (errno != EEXIST)))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "error creating CRIU work directory `%s`", cr_options->work_path);

  work_fd = open (cr_options->work_path, O_DIRECTORY | O_CLOEXEC);
  ಯದಿ (UNLIKELY (work_fd == -1))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "error opening CRIU work directory `%s`", cr_options->work_path);

  libcriu_wrapper->criu_set_work_dir_fd (work_fd);
  *work_fd_out = work_fd;
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_checkpoint_linux_criu (libcrun_container_status_t *status, libcrun_container_t *container,
                                         libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper ಸಂರಚನಾ libcriu_wrapper_s *wrapper = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *descriptors_path = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *freezer_path = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *path = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ image_fd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ work_fd = -1;
  ಪೂರ್ಣಾಙ್ಕ cgroup_mode;
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = load_wrapper (&wrapper, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (geteuid ())
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "checkpointing requires root");

  /* No CRIU version or feature checking yet. In configure.ac there
   * is a minimum CRIU version listed and so far it is good enough.
   *
   * The CRIU library also does not yet have an interface to CRIU
   * the version of the binary. Right now it is only possible to
   * query the version of the library via defines during buildtime.
   *
   * The whole CRIU library setup works this way, that the library
   * is only a wrapper around RPC calls to the actual library. So
   * if CRIU is updated and the SO of the library does not change,
   * and crun is not rebuilt against the newer version, the version
   * is still returning the values during buildtime and not from
   * the actual running CRIU binary. The RPC interface between the
   * library will not break, so no reason to worry, but it is not
   * possible to detect (via the library) which CRIU version is
   * actually being used. This needs to be added to CRIU upstream. */

  ret = libcriu_wrapper->criu_init_opts ();
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_version (err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (UNLIKELY (cr_options->image_path == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "image path not set");

  ret = mkdir (cr_options->image_path, 0700);
  ಯದಿ (UNLIKELY ((ret == -1) && (errno != EEXIST)))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "error creating checkpoint directory `%s`", cr_options->image_path);

  image_fd = open (cr_options->image_path, O_DIRECTORY | O_CLOEXEC);
  ಯದಿ (UNLIKELY (image_fd == -1))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "error opening checkpoint directory `%s`", cr_options->image_path);

  libcriu_wrapper->criu_set_images_dir_fd (image_fd);

  /* Set up logging. */
  libcriu_wrapper->criu_set_log_level (4);
  libcriu_wrapper->criu_set_log_file (CRIU_CHECKPOINT_LOG_FILE);

  /* Set up CRIU config file */
  ಯದಿ (UNLIKELY (handle_criu_config_file (container, err)))
    ಪ್ರತ್ಯಾಗಮ -1;

  /* Setting the pid early as we can skip a lot of checkpoint setup if
   * we just do a pre-dump. The PID needs to be set always. Do it here.
   * The main process of the container is the process CRIU will checkpoint
   * and all of its children. */
  libcriu_wrapper->criu_set_pid (status->pid);

  ret = setup_criu_work_dir (cr_options, &work_fd, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

#  ifdef CRIU_PRE_DUMP_SUPPORT

  {
    ಪೂರ್ಣಾಙ್ಕ criu_can_mem_track = 0;
    /* If the user uses --pre-dump for the second time or does
     * a final dump from a previous pre-dump, setting parent_path
     * is necessary so that CRIU can find which pages have not
     * changed compared to the previous dump. */
    ಯದಿ (cr_options->parent_path != ರಿಕ್ತ)
      {
        criu_can_mem_track = criu_check_mem_track (err);
        ಯದಿ (UNLIKELY (criu_can_mem_track == -1))
          ಪ್ರತ್ಯಾಗಮ -1;
        libcriu_wrapper->criu_set_track_mem (true);

        /* The parent path must be relative to image path (something like ../previous-dump).
           CRIU will fail with an unclear error message if the path is not right.
         */
        ಯದಿ (UNLIKELY (cr_options->parent_path[0] == '/'))
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "--parent-path must be relative");
        ಪೂರ್ಣಾಙ್ಕ is_dir = crun_dir_p_at (image_fd, cr_options->parent_path, false, err);
        ಯದಿ (UNLIKELY (is_dir <= 0))
          {
            ಯದಿ (is_dir < 0)
              ಪ್ರತ್ಯಾಗಮ crun_error_wrap (err, "invalid --parent-path");
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ENOTDIR, "invalid --parent-path");
          }
        ret = libcriu_wrapper->criu_set_parent_images (cr_options->parent_path);
        ಯದಿ (UNLIKELY (ret != 0))
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "error setting CRIU parent images path to `%s`", cr_options->parent_path);
      }

    ಯದಿ (cr_options->pre_dump)
      {
        ಯದಿ (criu_can_mem_track != 1)
          {
            criu_can_mem_track = criu_check_mem_track (err);
            ಯದಿ (UNLIKELY (criu_can_mem_track == -1))
              ಪ್ರತ್ಯಾಗಮ -1;
          }
        libcriu_wrapper->criu_set_track_mem (true);
        ret = libcriu_wrapper->criu_pre_dump ();
        ಯದಿ (UNLIKELY (ret != 0))
          {
            show_criu_log (cr_options->work_path, CRIU_CHECKPOINT_LOG_FILE);
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "CRIU pre-dump failed: %d", ret);
          }
        ಪ್ರತ್ಯಾಗಮ 0;
      }
  }
#  endif

  /* descriptors.json is needed during restore to correctly
   * reconnect stdin, stdout, stderr. */
  ret = append_paths (&descriptors_path, err, cr_options->image_path, DESCRIPTORS_FILENAME, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = write_file (descriptors_path, status->external_descriptors, safe_strlen (status->external_descriptors), err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_error_wrap (err, "error saving CRIU descriptors file");

  ret = append_paths (&path, err, status->bundle, status->rootfs, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcriu_wrapper->criu_set_ಬೇರು (path);
  ಯದಿ (UNLIKELY (ret != 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "error setting CRIU root to `%s`", path);

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (UNLIKELY (cgroup_mode < 0))
    ಪ್ರತ್ಯಾಗಮ cgroup_mode;

  /* For cgroup v1 we need to tell CRIU to handle all cgroup mounts as external mounts. */
  ಯದಿ (cgroup_mode != CGROUP_MODE_UNIFIED)
    {
      ret = checkpoint_cgroup_v1_mount (def, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  /* Tell CRIU about external bind mounts. */
  ಪ್ರತಿ (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      ಯದಿ (is_bind_mount (def->mounts[i], ರಿಕ್ತ, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          ವರ್ಣ buf[PATH_MAX];
          ಧ್ರುವ ವರ್ಣ *dest_in_ಬೇರು;

          ಯದಿ (nofollow)
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_in_ಬೇರು = chroot_realpath (status->rootfs, def->mounts[i]->destination, buf);
          ಯದಿ (UNLIKELY (dest_in_ಬೇರು == ರಿಕ್ತ))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "unable to resolve external bind mount `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          ಯದಿ (has_prefix (dest_in_ಬೇರು, status->rootfs))
            dest_in_ಬೇರು += safe_strlen (status->rootfs);

          ret = libcriu_wrapper->criu_add_ext_mount (dest_in_ಬೇರು, dest_in_ಬೇರು);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", def->mounts[i]->destination);
        }
    }

  ret = register_masked_paths_mounts (def, container, libcriu_wrapper, false, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* CRIU tries to checkpoint and restore all namespaces. However,
   * namespaces could be shared between containers in a pod.
   * To address this, CRIU provides support for external namespaces.
   * External namespaces allow to ignore the namespace during checkpoint
   * and restore the container into the existing namespaces.
   *
   * We are looking at config.json and if there is a path configured for
   * a namespace we are telling CRIU to ignore the namespace and
   * just restore the container into the existing namespace.
   *
   * In the case of Podman, a network namespace would be created via CNI.
   *
   * CRIU expects the information about an external namespace like this:
   * --external <namespace>[<inode>]:<key>
   */

  ಪ್ರತಿ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ = libcrun_find_namespace (def->linux->namespaces[i]->type);
      ಯದಿ (UNLIKELY (ಮೌಲ್ಯ < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      ಯದಿ (ಮೌಲ್ಯ == CLONE_NEWNET && def->linux->namespaces[i]->path != ರಿಕ್ತ)
        {
          cleanup_free ವರ್ಣ *external = ರಿಕ್ತ;
          ಸಂರಚನಾ stat statbuf;

          ret = stat (def->linux->namespaces[i]->path, &statbuf);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "unable to stat(): `%s`", def->linux->namespaces[i]->path);

          xasprintf (&external, "net[%ld]:" CRIU_EXT_NETNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_add_external (external);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }

      ಯದಿ (ಮೌಲ್ಯ == CLONE_NEWPID && def->linux->namespaces[i]->path != ರಿಕ್ತ)
        {
          cleanup_free ವರ್ಣ *external = ರಿಕ್ತ;
          ಸಂರಚನಾ stat statbuf;

          ret = stat (def->linux->namespaces[i]->path, &statbuf);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "unable to stat(): `%s`", def->linux->namespaces[i]->path);

          xasprintf (&external, "pid[%ld]:" CRIU_EXT_PIDNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_add_external (external);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }
    }

  /* Tell CRIU to use the freezer to pause all container processes. */
  ಯದಿ (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      /* This needs CRIU 3.14. */
      ret = append_paths (&freezer_path, err, CGROUP_ಬೇರು, status->cgroup_path, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಅನ್ಯಥಾ
    {
      ret = append_paths (&freezer_path, err, CGROUP_ಬೇರು "/freezer", status->cgroup_path, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = libcriu_wrapper->criu_set_freeze_cgroup (freezer_path);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "CRIU: failed setting freezer %d", ret);

  /* Set boolean options . */
  libcriu_wrapper->criu_set_leave_running (cr_options->leave_running);
  libcriu_wrapper->criu_set_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_set_shell_job (cr_options->shell_job);
  libcriu_wrapper->criu_set_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_set_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_set_orphan_pts_master (true);
  libcriu_wrapper->criu_set_manage_cgroups_mode (criu_cg_mode_from_public (cr_options->manage_cgroups_mode));

  libcriu_wrapper->criu_set_manage_cgroups (true);

  ಯದಿ (libcriu_wrapper->criu_set_network_lock)
    {
      ಪೂರ್ಣಾಙ್ಕ lock = 0;

      ret = criu_network_lock_from_public (cr_options->network_lock_method, &lock, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ಯದಿ (lock > 0)
        {
          ret = libcriu_wrapper->criu_set_network_lock (lock);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "CRIU: failed setting network lock");
        }
    }

  ret = libcriu_wrapper->criu_dump ();
  ಯದಿ (UNLIKELY (ret != 0))
    {
      show_criu_log (cr_options->work_path, CRIU_CHECKPOINT_LOG_FILE);
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ret < 0 ? -ret : 0, "CRIU checkpointing failed: %d", ret);
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
prepare_restore_mounts_internal (runtime_spec_schema_config_schema *def, ವರ್ಣ *ಬೇರು, ಧ್ರುವ ವರ್ಣ **mounted,
                                 size_t *n_mounted, libcrun_error_t *err)
{
  uint32_t i;

  /* Go through all mountpoints to be able to recreate missing mountpoints. */
  ಪ್ರತಿ (i = 0; i < def->mounts_len; i++)
    {
      ವರ್ಣ *dest = def->mounts[i]->destination;
      ವರ್ಣ *type = def->mounts[i]->type;
      cleanup_close ಪೂರ್ಣಾಙ್ಕ ಬೇರು_fd = -1;
      bool nofollow = false;
      bool on_tmpfs = false;
      ಪೂರ್ಣಾಙ್ಕ is_dir = 1;
      size_t j;

      /* cgroup restore should be handled by CRIU itself */
      ಯದಿ (type && (strcmp (type, "cgroup") == 0 || strcmp (type, "cgroup2") == 0))
        ಅನುವರ್ತ;

      /* Check if the mountpoint is on a tmpfs. CRIU restores
       * all tmpfs. We do need to recreate directories on a tmpfs. */
      size_t dest_len = strlen (dest);
      ಪ್ರತಿ (j = 0; j < def->mounts_len; j++)
        {
          ಯದಿ (def->mounts[j]->type == ರಿಕ್ತ || strcmp (def->mounts[j]->type, "tmpfs") != 0)
            ಅನುವರ್ತ;
          size_t mount_len = strlen (def->mounts[j]->destination);
          ಯದಿ (mount_len < dest_len && dest[mount_len] == '/' && strncmp (dest, def->mounts[j]->destination, mount_len) == 0)
            {
              /* This is a mountpoint which is on a tmpfs.*/
              on_tmpfs = true;
              ಭಙ್ಗ;
            }
        }

      ಯದಿ (on_tmpfs)
        ಅನುವರ್ತ;

      /* For bind mounts check if the source is a file or a directory. */
      ಯದಿ (is_bind_mount (def->mounts[i], ರಿಕ್ತ, &nofollow))
        {
          ಯದಿ (nofollow)
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          is_dir = crun_dir_p (def->mounts[i]->source, false, err);
          ಯದಿ (UNLIKELY (is_dir < 0))
            ಪ್ರತ್ಯಾಗಮ is_dir;
        }

      ಬೇರು_fd = open (ಬೇರು, O_RDONLY | O_CLOEXEC);
      ಯದಿ (UNLIKELY (ಬೇರು_fd == -1))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "error opening container root directory `%s`", ಬೇರು);

      ಯದಿ (is_dir)
        {
          ಪೂರ್ಣಾಙ್ಕ ret;

          ret = crun_safe_ensure_directory_at (ಬೇರು_fd, ಬೇರು, dest, 0755, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }
      ಅನ್ಯಥಾ
        {
          ಪೂರ್ಣಾಙ್ಕ ret;

          ret = crun_safe_ensure_file_at (ಬೇರು_fd, ಬೇರು, dest, 0755, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }

      /* Mount a bind mount source now, so that the mountpoints of the
       * mounts inside it (e.g. a nested bind mount) are created in the
       * source, where CRIU expects them.  This also happens during the
       * initial container creation, as the mounts are done in order.  */
      ಯದಿ (is_bind_mount (def->mounts[i], ರಿಕ್ತ, &nofollow))
        {
          cleanup_close ಪೂರ್ಣಾಙ್ಕ dst_fd = -1;
          proc_fd_path_t fd_path;

          dst_fd = safe_openat (ಬೇರು_fd, ಬೇರು, dest, O_PATH | O_CLOEXEC, 0, err);
          ಯದಿ (UNLIKELY (dst_fd < 0))
            ಪ್ರತ್ಯಾಗಮ dst_fd;

          /* Not recursive: the source might contain ROOT itself (e.g. when
           * it is the bundle directory), and it is not needed anyway.  */
          get_proc_self_fd_path (fd_path, dst_fd);
          ಯದಿ (UNLIKELY (mount (def->mounts[i]->source, fd_path, ರಿಕ್ತ, MS_BIND, ರಿಕ್ತ) < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "bind mount `%s` to `%s`", def->mounts[i]->source, dest);

          mounted[(*n_mounted)++] = dest;
        }
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Move the current process back to CGROUPS, as read from /proc/self/cgroup.
   A failure is not fatal, so only warn about it.  */
ಸ್ಥಿರ ಶೂನ್ಯ
move_back_to_cgroups (ಧ್ರುವ ವರ್ಣ *cgroups)
{
  libcrun_error_t tmp_err = ರಿಕ್ತ;

  ಯದಿ (UNLIKELY (libcrun_move_self_to_cgroups (cgroups, &tmp_err) < 0))
    {
      libcrun_warning ("cannot move back to the original cgroup: %s", tmp_err->ಸಂದೇಶ);
      crun_error_release (&tmp_err);
    }
}

/* Ask CRIU to restore from a throw-away process, which first joins CGROUP_PATH,
   when it is set, so that the tasks CRIU creates are placed there.

   It is used when the calling process must be left alone: it is neither moved
   to the container cgroup, so there is nothing to undo once the restore is
   over, nor does it become the parent of the container, as the restored
   process tree is a sibling of CRIU, hence a child of the process calling
   criu_restore_child().

   As the child shares the memory and the stack with the caller, all signals
   are blocked across the vfork, and the child resets the signal handlers
   before unblocking them, so that no handler of the caller can run there.
   This is a separate function so that no variable of the caller is live
   across the vfork.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
criu_restore_child_in_cgroup (ಧ್ರುವ ವರ್ಣ *cgroup_path, ಪೂರ್ಣಾಙ್ಕ *criu_ret, libcrun_error_t *err)
{
  sigset_t all_signals, old_mask;
  ಪೂರ್ಣಾಙ್ಕ wait_status = 0;
  pid_t pid;
  ಪೂರ್ಣಾಙ್ಕ ret;

  *criu_ret = -1;

  sigfillset (&all_signals);
  ret = sigprocmask (SIG_BLOCK, &all_signals, &old_mask);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "sigprocmask");

  /* Must be vfork: the child shares our memory space, so both *criu_ret and
     the error it creates are visible here once it is gone.  */
  pid = vfork ();
  ಯದಿ (UNLIKELY (pid < 0))
    {
      ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;

      sigprocmask (SIG_SETMASK, &old_mask, ರಿಕ್ತ);
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, saved_errno, "vfork");
    }

  ಯದಿ (pid == 0)
    {
      ಸಂರಚನಾ sigaction act;
      ಪೂರ್ಣಾಙ್ಕ i;

      /* The signal dispositions are not shared with the parent, so resetting
         them here does not affect it.  Keep the ignored signals ignored, as
         CRIU would inherit them if it was run directly by the caller.  */
      ಪ್ರತಿ (i = 1; i < NSIG; i++)
        {
          ಯದಿ (sigaction (i, ರಿಕ್ತ, &act) < 0 || act.sa_handler == SIG_IGN || act.sa_handler == SIG_DFL)
            ಅನುವರ್ತ;

          memset (&act, 0, ಪರಿಮಾಣ (act));
          act.sa_handler = SIG_DFL;
          sigaction (i, &act, ರಿಕ್ತ);
        }
      sigprocmask (SIG_SETMASK, &old_mask, ರಿಕ್ತ);

      ಯದಿ (! is_empty_string (cgroup_path))
        {
          ret = libcrun_move_process_to_cgroup (0, 0, cgroup_path, false, err);
          ಯದಿ (UNLIKELY (ret < 0))
            _safe_exit (EXIT_FAILURE);
        }

      *criu_ret = libcriu_wrapper->criu_restore_child ();
      _safe_exit (EXIT_SUCCESS);
    }

  /* The child is gone by now, it is safe to handle signals again.  */
  sigprocmask (SIG_SETMASK, &old_mask, ರಿಕ್ತ);

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      /* The caller might have reaped the child already, e.g. when it ignores
         SIGCHLD.  Its result is still visible, so use it.  */
      ಯದಿ (errno != ECHILD)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "waitpid for the CRIU restore process");

      /* The child might have failed before calling CRIU, and left an error.  */
      ಪ್ರತ್ಯಾಗಮ *err != ರಿಕ್ತ ? -1 : 0;
    }

  ret = get_process_exit_status (wait_status);
  ಯದಿ (UNLIKELY (ret != EXIT_SUCCESS))
    {
      /* The child creates the error in the shared memory space, do not
         overwrite it with crun_make_error().  */
      ಯದಿ (*err == ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "the CRIU restore process exited with status %d", ret);
      ಪ್ರತ್ಯಾಗಮ -1;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Recreate the mountpoints which do not exist in the container rootfs
 * mounted at ROOT, the same way it is done on the container creation.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
prepare_restore_mounts (runtime_spec_schema_config_schema *def, ವರ್ಣ *ಬೇರು, libcrun_error_t *err)
{
  cleanup_free ಧ್ರುವ ವರ್ಣ **mounted = xmalloc0 (ಪರಿಮಾಣ (ವರ್ಣ *) * (def->mounts_len + 1));
  cleanup_close ಪೂರ್ಣಾಙ್ಕ ಬೇರು_fd = -1;
  size_t n_mounted = 0;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = prepare_restore_mounts_internal (def, ಬೇರು, mounted, &n_mounted, err);

  /* The bind mounts done above are only needed to create the mountpoints,
   * and CRIU restores the mounts itself, so undo them, in reverse order. */
  ಯದಿ (n_mounted > 0)
    ಬೇರು_fd = open (ಬೇರು, O_PATH | O_CLOEXEC);
  ಯಾವತ್ (ಬೇರು_fd >= 0 && n_mounted > 0)
    {
      libcrun_error_t tmp_err = ರಿಕ್ತ;
      ಧ್ರುವ ವರ್ಣ *dest = mounted[--n_mounted];
      cleanup_close ಪೂರ್ಣಾಙ್ಕ dst_fd = -1;
      proc_fd_path_t fd_path;

      dst_fd = safe_openat (ಬೇರು_fd, ಬೇರು, dest, O_PATH | O_CLOEXEC, 0, &tmp_err);
      ಯದಿ (UNLIKELY (dst_fd < 0))
        {
          crun_error_release (&tmp_err);
          ಅನುವರ್ತ;
        }
      get_proc_self_fd_path (fd_path, dst_fd);
      ಯದಿ (UNLIKELY (umount2 (fd_path, MNT_DETACH) < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "unmount `%s`", dest);
    }

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_restore_linux_criu (libcrun_container_status_t *status, libcrun_container_t *container,
                                      libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper ಸಂರಚನಾ libcriu_wrapper_s *wrapper = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ inherit_new_net_fd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ inherit_new_pid_fd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ image_fd = -1;
  cleanup_free ವರ್ಣ *ಬೇರು = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *bundle_cleanup = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *own_cgroups = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ work_fd = -1;
  ಪೂರ್ಣಾಙ್ಕ cgroup_mode;
  ಪೂರ್ಣಾಙ್ಕ criu_ret;
  ಪೂರ್ಣಾಙ್ಕ ret_out;
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = load_wrapper (&wrapper, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (geteuid ())
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "restoring requires root");

  ret = libcriu_wrapper->criu_init_opts ();
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_version (err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (UNLIKELY (cr_options->image_path == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "image path not set");

  image_fd = open (cr_options->image_path, O_DIRECTORY | O_CLOEXEC);
  ಯದಿ (UNLIKELY (image_fd == -1))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "error opening checkpoint directory `%s`", cr_options->image_path);

  libcriu_wrapper->criu_set_images_dir_fd (image_fd);

  /* Load descriptors.json to tell CRIU where those FDs should be connected to. */
  {
    cleanup_free ವರ್ಣ *descriptors_path = ರಿಕ್ತ;
    cleanup_free ವರ್ಣ *buffer = ರಿಕ್ತ;
    json_object *doc = ರಿಕ್ತ;

    ret = append_paths (&descriptors_path, err, cr_options->image_path, DESCRIPTORS_FILENAME, ರಿಕ್ತ);
    ಯದಿ (UNLIKELY (ret < 0))
      ಪ್ರತ್ಯಾಗಮ ret;

    ret = read_all_file (descriptors_path, &buffer, ರಿಕ್ತ, err);
    ಯದಿ (UNLIKELY (ret < 0))
      ಪ್ರತ್ಯಾಗಮ ret;

    /* descriptors.json contains a JSON array with strings
     * telling where 0, 1 and 2 have been initially been
     * pointing to. For each descriptor which points to
     * a pipe 'pipe:' we tell CRIU to reconnect that pipe
     * to the corresponding FD to have (especially) stdout
     * and stderr being correctly redirected. */
    ret = parse_json_file (&doc, buffer, ರಿಕ್ತ, err);
    ಯದಿ (UNLIKELY (ret < 0))
      ಪ್ರತ್ಯಾಗಮ ret;

    ಯದಿ (json_object_is_type (doc, json_type_array))
      {
        size_t i, len = json_object_array_length (doc);

        /* len will probably always be 3 as crun is currently only
         * recording the destination of FD 0, 1 and 2. */
        ಪ್ರತಿ (i = 0; i < len; ++i)
          {
            json_object *s = json_object_array_get_idx (doc, i);
            ಯದಿ (s && json_object_is_type (s, json_type_string))
              {
                ಧ್ರುವ ವರ್ಣ *str = json_object_get_string (s);
                ಯದಿ (has_prefix (str, "pipe:"))
                  libcriu_wrapper->criu_add_inherit_fd (i, str);
              }
          }
      }
    json_object_put (doc);
  }

  /* Our std streams are the new external descriptors, as CRIU connects the
   * inherited pipes to them.  Record them now: once the container is
   * restored it is already running, and /proc/PID/fd is under its control. */
  ret = libcrun_save_external_descriptors (container, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = setup_criu_work_dir (cr_options, &work_fd, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (cr_options->lsm_profile != ರಿಕ್ತ)
    {
      ret = libcriu_wrapper->criu_set_lsm_profile (cr_options->lsm_profile);
      ಯದಿ (UNLIKELY (ret != 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "error setting LSM profile to `%s`", cr_options->lsm_profile);
    }

  ಯದಿ (cr_options->lsm_mount_context != ರಿಕ್ತ)
    {
      ret = libcriu_wrapper->criu_set_lsm_mount_context (cr_options->lsm_mount_context);
      ಯದಿ (UNLIKELY (ret != 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "error setting LSM mount context to `%s`", cr_options->lsm_mount_context);
    }

  /* do realpath on root */
  bundle_cleanup = realpath (status->bundle, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (bundle_cleanup == ರಿಕ್ತ))
    bundle_cleanup = xstrdup (status->bundle);

  /* Tell CRIU about external bind mounts. */
  ಪ್ರತಿ (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      ಯದಿ (is_bind_mount (def->mounts[i], ರಿಕ್ತ, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          ವರ್ಣ buf[PATH_MAX];
          ಧ್ರುವ ವರ್ಣ *dest_in_ಬೇರು;
          ಧ್ರುವ ವರ್ಣ *source = def->mounts[i]->source;
          cleanup_free ವರ್ಣ *abs_source = ರಿಕ್ತ;
          unsigned long flags = 0;
          size_t j;

          ಯದಿ (nofollow)
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_in_ಬೇರು = chroot_realpath (status->rootfs, def->mounts[i]->destination, buf);
          ಯದಿ (UNLIKELY (dest_in_ಬೇರು == ರಿಕ್ತ))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "unable to resolve external bind mount destination `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          ಯದಿ (has_prefix (dest_in_ಬೇರು, status->rootfs))
            dest_in_ಬೇರು += safe_strlen (status->rootfs);

          /* A relative source is relative to the bundle, while CRIU would
             resolve it relative to its own working directory.  */
          ಯದಿ (source && source[0] != '/')
            {
              ret = append_paths (&abs_source, err, bundle_cleanup, source, ರಿಕ್ತ);
              ಯದಿ (UNLIKELY (ret < 0))
                ಪ್ರತ್ಯಾಗಮ ret;
              source = abs_source;
            }

          ret = libcriu_wrapper->criu_add_ext_mount (dest_in_ಬೇರು, source);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", source);

          ಪ್ರತಿ (j = 0; j < def->mounts[i]->options_len; j++)
            {
              ಧ್ರುವ ಸಂರಚನಾ propagation_flags_s *prop = ರಿಕ್ತ;

              ಯದಿ (def->mounts[i]->options[j])
                prop = libcrun_str2mount_flags (def->mounts[i]->options[j]);
              ಯದಿ (prop && ! prop->clear)
                flags |= prop->flags;
            }
          add_restored_ext_mount (dest_in_ಬೇರು, flags);
        }
    }

  ret = register_masked_paths_mounts (def, container, libcriu_wrapper, true, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* Mount the container rootfs for CRIU. */
  ret = append_paths (&ಬೇರು, err, bundle_cleanup, "criu-root", ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = mkdir (ಬೇರು, 0755);
  ಯದಿ (UNLIKELY (ret == -1))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "error creating restore directory `%s`", ಬೇರು);

  ret = mount (status->rootfs, ಬೇರು, ರಿಕ್ತ, MS_BIND | MS_REC, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret == -1))
    {
      ret = crun_make_error (err, errno, "error mounting restore directory `%s`", ಬೇರು);
      ಗಚ್ಛ out;
    }

  /* During initial container creation, crun will create mountpoints
   * defined in config.json if they do not exist. If we are restoring
   * we need to make sure these mountpoints also exist.
   * This is not perfect, as this means crun will modify a rootfs
   * even if it marked as read-only, but runc already modifies
   * the rootfs in the same way. */

  ret = prepare_restore_mounts (def, ಬೇರು, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ out_umount;

  ret = libcriu_wrapper->criu_set_ಬೇರು (ಬೇರು);
  ಯದಿ (UNLIKELY (ret != 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU root to `%s`", ಬೇರು);
      ಗಚ್ಛ out_umount;
    }

  /* If a namespace defined in config.json we are telling
   * CRIU use that namespace when restoring the process tree.
   *
   * CRIU expects the information about the namespace like this:
   * --inherit-fd fd[<fd>]:<key>
   * The <key> needs to be the same as during checkpointing (extRootNetNS). */
  ಪ್ರತಿ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ open_flags_for_inherit = O_RDONLY; /* Cannot be O_CLOEXEC as it is passed to the child process. */
      ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ = libcrun_find_namespace (def->linux->namespaces[i]->type);
      ಯದಿ (UNLIKELY (ಮೌಲ್ಯ < 0))
        {
          ret = crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);
          ಗಚ್ಛ out_umount;
        }

      ಯದಿ (ಮೌಲ್ಯ == CLONE_NEWNET && def->linux->namespaces[i]->path != ರಿಕ್ತ)
        {
          inherit_new_net_fd = open (def->linux->namespaces[i]->path, open_flags_for_inherit);
          ಯದಿ (UNLIKELY (inherit_new_net_fd < 0))
            {
              ret = crun_make_error (err, errno, "unable to open(): `%s`", def->linux->namespaces[i]->path);
              ಗಚ್ಛ out_umount;
            }

          ret = libcriu_wrapper->criu_add_inherit_fd (inherit_new_net_fd, CRIU_EXT_NETNS);
          ಯದಿ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              ಗಚ್ಛ out_umount;
            }
        }

      ಯದಿ (ಮೌಲ್ಯ == CLONE_NEWPID && def->linux->namespaces[i]->path != ರಿಕ್ತ)
        {
          inherit_new_pid_fd = open (def->linux->namespaces[i]->path, open_flags_for_inherit);
          ಯದಿ (UNLIKELY (inherit_new_pid_fd < 0))
            {
              ret = crun_make_error (err, errno, "unable to open(): `%s`", def->linux->namespaces[i]->path);
              ಗಚ್ಛ out_umount;
            }

          ret = libcriu_wrapper->criu_add_inherit_fd (inherit_new_pid_fd, CRIU_EXT_PIDNS);
          ಯದಿ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              ಗಚ್ಛ out_umount;
            }
        }

#  ifdef CRIU_JOIN_NS_SUPPORT
      ಯದಿ (ಮೌಲ್ಯ == CLONE_NEWTIME && def->linux->namespaces[i]->path != ರಿಕ್ತ)
        {
          ಯದಿ (libcriu_wrapper->criu_join_ns_add == ರಿಕ್ತ)
            {
              ret = crun_make_error (err, 0, "shared time namespace restore is supported in CRIU >= 3.16.1");
              ಗಚ್ಛ out_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_add ("time", def->linux->namespaces[i]->path, ರಿಕ್ತ);
          ಯದಿ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->path);
              ಗಚ್ಛ out_umount;
            }
        }

      ಯದಿ (ಮೌಲ್ಯ == CLONE_NEWIPC && def->linux->namespaces[i]->path != ರಿಕ್ತ)
        {
          ಯದಿ (libcriu_wrapper->criu_join_ns_add == ರಿಕ್ತ)
            {
              ret = crun_make_error (err, 0, "shared ipc namespace restore is supported in CRIU >= 3.16.1");
              ಗಚ್ಛ out_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_add ("ipc", def->linux->namespaces[i]->path, ರಿಕ್ತ);
          ಯದಿ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->path);
              ಗಚ್ಛ out_umount;
            }
        }

      ಯದಿ (ಮೌಲ್ಯ == CLONE_NEWUTS && def->linux->namespaces[i]->path != ರಿಕ್ತ)
        {
          ಯದಿ (libcriu_wrapper->criu_join_ns_add == ರಿಕ್ತ)
            {
              ret = crun_make_error (err, 0, "shared uts namespace restore is supported in CRIU >= 3.16.1");
              ಗಚ್ಛ out_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_add ("uts", def->linux->namespaces[i]->path, ರಿಕ್ತ);
          ಯದಿ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->path);
              ಗಚ್ಛ out_umount;
            }
        }
#  endif
    }

  /* Set up CRIU config file */
  ret = handle_criu_config_file (container, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ out_umount;

  /* Tell CRIU if cgroup v1 needs to be handled. */
  ret = restore_cgroup_v1_mount (def, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ out_umount;

  console_socket = cr_options->console_socket;
  libcriu_wrapper->criu_set_notify_cb (criu_notify);

  /* Set boolean options . */
  libcriu_wrapper->criu_set_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_set_shell_job (cr_options->shell_job);
  libcriu_wrapper->criu_set_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_set_tcp_close (cr_options->tcp_close);
  libcriu_wrapper->criu_set_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_set_orphan_pts_master (true);

  ಯದಿ (status->cgroup_path)
    {
      cgroup_mode = libcrun_get_cgroup_mode (err);
      ಯದಿ (UNLIKELY (cgroup_mode < 0))
        {
          ret = cgroup_mode;
          ಗಚ್ಛ out_umount;
        }

      /* With cgroup v2, the cgroup path is only meaningful for the unified
         hierarchy, so only relocate that one.  A NULL controller would make
         CRIU relocate every hierarchy the container is in, named cgroup v1
         ones included.  As CRIU dumps such a hierarchy from wherever the
         container is in it, which is its root, as crun leaves named
         hierarchies alone, every checkpoint and restore would then copy the
         whole hierarchy into itself.  */
      ret = libcriu_wrapper->criu_add_cg_ಬೇರು (cgroup_mode == CGROUP_MODE_UNIFIED ? "" : ರಿಕ್ತ,
                                               status->cgroup_path);
      ಯದಿ (UNLIKELY (ret != 0))
        {
          ret = crun_make_error (err, 0, "error setting CRIU cgroup root to `%s`", status->cgroup_path);
          ಗಚ್ಛ out_umount;
        }
    }

  libcriu_wrapper->criu_set_manage_cgroups_mode (criu_cg_mode_from_public (cr_options->manage_cgroups_mode));
  libcriu_wrapper->criu_set_manage_cgroups (true);

  ಯದಿ (libcriu_wrapper->criu_set_network_lock)
    {
      ಪೂರ್ಣಾಙ್ಕ lock = 0;

      ret = criu_network_lock_from_public (cr_options->network_lock_method, &lock, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಗಚ್ಛ out_umount;

      ಯದಿ (lock > 0)
        {
          ret = libcriu_wrapper->criu_set_network_lock (lock);
          ಯದಿ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "CRIU: failed setting network lock");
              ಗಚ್ಛ out_umount;
            }
        }
    }

  libcriu_wrapper->criu_set_log_level (4);
  ret = libcriu_wrapper->criu_set_log_file (CRIU_RESTORE_LOG_FILE);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU log file to `%s`", CRIU_RESTORE_LOG_FILE);
      ಗಚ್ಛ out_umount;
    }

  /* criu_restore() returns the PID of the process of the restored process
   * tree. This PID will not be the same as status->pid if the container is
   * running in a PID namespace. But it will always be > 0. */
  ಯದಿ (cr_options->detach)
    {
      /* Nothing waits for the container, so there is no reason to take over
         this process: use a throw-away one.  It is used even when there is no
         cgroup to join, so that the container is not left as a child of a
         process which is not going to reap it.  */
      ret = criu_restore_child_in_cgroup (status->cgroup_path, &criu_ret, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಗಚ್ಛ out_umount;

      ret = criu_ret;
    }
  ಅನ್ಯಥಾ
    {
      /* Like runc does, put CRIU into the container cgroup for the time of
       * restore, so the restored tasks are created there. It is necessary in
       * the "ignore" mode, where CRIU does not deal with cgroups at all, and
       * does not hurt otherwise. As CRIU is our child, do it by moving
       * ourselves there, and move back once the restore is done. */
      ಯದಿ (! is_empty_string (status->cgroup_path))
        {
          ret = read_all_file (PROC_SELF_CGROUP, &own_cgroups, ರಿಕ್ತ, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಗಚ್ಛ out_umount;

          ret = libcrun_move_process_to_cgroup (0, 0, status->cgroup_path, false, err);
          ಯದಿ (UNLIKELY (ret < 0))
            {
              /* Some of the cgroups might have been joined already.  */
              move_back_to_cgroups (own_cgroups);
              ಗಚ್ಛ out_umount;
            }
        }

      ret = libcriu_wrapper->criu_restore_child ();

      ಯದಿ (own_cgroups)
        move_back_to_cgroups (own_cgroups);
    }

  ಯದಿ (UNLIKELY (ret <= 0))
    {
      show_criu_log (cr_options->work_path, CRIU_RESTORE_LOG_FILE);
      ret = crun_make_error (err, 0, "CRIU restoring failed: %d", ret);
      ಗಚ್ಛ out_umount;
    }

  /* Update the status struct with the newly allocated PID. This will
   * be necessary later when moving the process into its cgroup. */
  status->pid = ret;
  ret = 0;

out_umount:
  ret_out = umount (ಬೇರು);
  ಯದಿ (UNLIKELY (ret_out == -1))
    {
      ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
      rmdir (ಬೇರು);
      ಯದಿ (ret < 0)
        ಪ್ರತ್ಯಾಗಮ crun_error_wrap (err, "error unmounting restore directory `%s`", ಬೇರು);
      ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, saved_errno, "error unmounting restore directory `%s`", ಬೇರು);
    }
out:
  ret_out = rmdir (ಬೇರು);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;
  ಯದಿ (UNLIKELY (ret_out == -1))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "error removing restore directory `%s`", ಬೇರು);
  ಪ್ರತ್ಯಾಗಮ ret;
}
#endif
