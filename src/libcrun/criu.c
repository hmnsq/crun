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
#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ

#ಅನ್ತರ್ಭಾವ <config.h>

#ಆದರೆ HAVE_CRIU && HAVE_DLOPEN

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
ಅಕ್ಷರ *chroot_realpath (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *chroot, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಅಕ್ಷರ resolved_ಮಾರ್ಗ[]);

/* Map the public cgroups mode (enum libcrun_cr_cgroups_mode) onto the CRIU enum.  */
ಸ್ಥಿರ ಎಣಿಕೆ criu_cg_ಬಹುಲಕ
criu_cg_ಬಹುಲಕ_from_ಬಹಿರಂಗ (ಪೂರ್ಣಾಂಕ ಬಹುಲಕ)
{
  ಆಯ್ಕೆ (ಬಹುಲಕ)
    {
    ಸ್ಥಿತಿ LIBCRUN_CR_CG_ಬಹುಲಕ_IGNORE:
      ಹಿಂತಿರುಗಿ CRIU_CG_ಬಹುಲಕ_IGNORE;
    ಸ್ಥಿತಿ LIBCRUN_CR_CG_ಬಹುಲಕ_ತುಂಬಿದ:
      ಹಿಂತಿರುಗಿ CRIU_CG_ಬಹುಲಕ_ತುಂಬಿದ;
    ಸ್ಥಿತಿ LIBCRUN_CR_CG_ಬಹುಲಕ_STRICT:
      ಹಿಂತಿರುಗಿ CRIU_CG_ಬಹುಲಕ_STRICT;
    ಸ್ಥಿತಿ LIBCRUN_CR_CG_ಬಹುಲಕ_DEFAULT:
    ಸ್ಥಿತಿ LIBCRUN_CR_CG_ಬಹುಲಕ_SOFT:
    ಪೂರ್ವನಿಯೋಜಿತ:
      /* Default to CRIU_CG_MODE_SOFT, just as runc.  */
      ಹಿಂತಿರುಗಿ CRIU_CG_ಬಹುಲಕ_SOFT;
    }
}

/* Map the public network lock method (enum libcrun_cr_network_lock_method) onto the
   CRIU enum.  *LOCK is set to 0 when no explicit method must be set (CRIU default).
   A method that this build cannot honor is an error, never a silent fallback to
   the CRIU default.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
criu_ಜಾಲ_ಬೀಗ_from_ಬಹಿರಂಗ (ಪೂರ್ಣಾಂಕ ವಿಧಾನ, ಪೂರ್ಣಾಂಕ *ಬೀಗ, libcrun_error_t *err)
{
  ಆಯ್ಕೆ (ವಿಧಾನ)
    {
    ಸ್ಥಿತಿ LIBCRUN_CR_ಜಾಲ_ಬೀಗ_IPTABLES:
      *ಬೀಗ = CRIU_ಜಾಲ_ಬೀಗ_IPTABLES;
      ಹಿಂತಿರುಗಿ 0;
    ಸ್ಥಿತಿ LIBCRUN_CR_ಜಾಲ_ಬೀಗ_NFTABLES:
      *ಬೀಗ = CRIU_ಜಾಲ_ಬೀಗ_NFTABLES;
      ಹಿಂತಿರುಗಿ 0;
    ಸ್ಥಿತಿ LIBCRUN_CR_ಜಾಲ_ಬೀಗ_SKIP:
#  ಆದರೆ CRIU_ಜಾಲ_ಬೀಗ_SKIP_SUPPORT
      *ಬೀಗ = CRIU_ಜಾಲ_ಬೀಗ_SKIP;
      ಹಿಂತಿರುಗಿ 0;
#  ಇಲ್ಲದಿದ್ದರೆ
      ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "CRIU: the `skip` network lock method is not supported by this build");
#  endif
    ಸ್ಥಿತಿ LIBCRUN_CR_ಜಾಲ_ಬೀಗ_DEFAULT:
      *ಬೀಗ = 0;
      ಹಿಂತಿರುಗಿ 0;
    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "CRIU: unknown network lock method %d", ವಿಧಾನ);
    }
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *console_socket = ಖಾಲಿ;

/* The external mounts CRIU restores, and the flags the configuration asks
   for each of them.  */
ಸ್ಥಿರ ರಚನೆ libcrun_mount_flags_s *restored_ext_mounts = ಖಾಲಿ;
ಸ್ಥಿರ size_t n_restored_ext_mounts = 0;

#  define LIBCRIU_MIN_ಆವೃತ್ತಿ 31500

ರಚನೆ libcriu_wrapper_s
{
  ಶೂನ್ಯ *handle;
  ಪೂರ್ಣಾಂಕ (*criu_ಕೂಡಿಸು_ext_mount) (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *val);
  ಪೂರ್ಣಾಂಕ (*criu_ಕೂಡಿಸು_external) (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ);
  ಪೂರ್ಣಾಂಕ (*criu_ಕೂಡಿಸು_inherit_fd) (ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ);
  ಪೂರ್ಣಾಂಕ (*criu_check_ಆವೃತ್ತಿ) (ಪೂರ್ಣಾಂಕ minimum);
  ಪೂರ್ಣಾಂಕ (*criu_dump) (ಶೂನ್ಯ);
  ಪೂರ್ಣಾಂಕ (*criu_get_orphan_pts_master_fd) (ಶೂನ್ಯ);
  ಪೂರ್ಣಾಂಕ (*criu_init_opts) (ಶೂನ್ಯ);
  ಪೂರ್ಣಾಂಕ (*criu_notify_pid) (criu_notify_arg_t na);
#  ifdef CRIU_JOIN_NS_SUPPORT
  ಪೂರ್ಣಾಂಕ (*criu_join_ns_ಕೂಡಿಸು) (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ns, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ns_file, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *extra_opt);
#  endif
#  ifdef CRIU_PRE_DUMP_SUPPORT
  ಪೂರ್ಣಾಂಕ (*criu_feature_check) (ರಚನೆ criu_feature_check *features, size_t ಗಾತ್ರ_2);
  ಪೂರ್ಣಾಂಕ (*criu_pre_dump) (ಶೂನ್ಯ);
#  endif
  ಪೂರ್ಣಾಂಕ (*criu_restore_ಮಗು) (ಶೂನ್ಯ);
  ಪೂರ್ಣಾಂಕ (*criu_ಗಣ_freeze_cgroup) (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name);
  ಶೂನ್ಯ (*criu_ಗಣ_file_locks) (bool file_locks);
  ಶೂನ್ಯ (*criu_ಗಣ_ext_unix_sk) (bool ext_unix_sk);
  ಪೂರ್ಣಾಂಕ (*criu_ಗಣ_log_file) (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *log_file);
  ಶೂನ್ಯ (*criu_ಗಣ_log_level) (ಪೂರ್ಣಾಂಕ log_level);
  ಶೂನ್ಯ (*criu_ಗಣ_leave_running) (bool leave_running);
  ಶೂನ್ಯ (*criu_ಗಣ_manage_cgroups) (bool manage);
  ಶೂನ್ಯ (*criu_ಗಣ_manage_cgroups_ಬಹುಲಕ) (ಎಣಿಕೆ criu_cg_ಬಹುಲಕ ಬಹುಲಕ);
  ಪೂರ್ಣಾಂಕ (*criu_ಗಣ_ಜಾಲ_ಬೀಗ) (ಎಣಿಕೆ criu_ಜಾಲ_ಬೀಗ_ವಿಧಾನ ವಿಧಾನ);
  ಶೂನ್ಯ (*criu_ಗಣ_notify_cb) (ಪೂರ್ಣಾಂಕ (*cb) (ಅಕ್ಷರ *action, criu_notify_arg_t na));
  ಶೂನ್ಯ (*criu_ಗಣ_orphan_pts_master) (bool orphan_pts_master);
  ಶೂನ್ಯ (*criu_ಗಣ_images_dir_fd) (ಪೂರ್ಣಾಂಕ fd);
  ಪೂರ್ಣಾಂಕ (*criu_ಗಣ_ಪೋಷಕ_images) (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ);
  ಶೂನ್ಯ (*criu_ಗಣ_pid) (ಪೂರ್ಣಾಂಕ pid);
  ಪೂರ್ಣಾಂಕ (*criu_ಗಣ_ಬೇರು) (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬೇರು);
  ಪೂರ್ಣಾಂಕ (*criu_ಕೂಡಿಸು_cg_ಬೇರು) (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ctrl, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ);
  ಶೂನ್ಯ (*criu_ಗಣ_shell_ಕೆಲಸ) (bool shell_ಕೆಲಸ);
  ಶೂನ್ಯ (*criu_ಗಣ_tcp_established) (bool tcp_established);
  ಶೂನ್ಯ (*criu_ಗಣ_tcp_close) (bool tcp_close);
  ಶೂನ್ಯ (*criu_ಗಣ_track_mem) (bool track_mem);
  ಶೂನ್ಯ (*criu_ಗಣ_work_dir_fd) (ಪೂರ್ಣಾಂಕ fd);
  ಪೂರ್ಣಾಂಕ (*criu_ಗಣ_lsm_profile) (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name);
  ಪೂರ್ಣಾಂಕ (*criu_ಗಣ_lsm_mount_context) (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name);
  ಪೂರ್ಣಾಂಕ (*criu_ಗಣ_config_file) (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ);
};

ಸ್ಥಿರ ರಚನೆ libcriu_wrapper_s *libcriu_wrapper;

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cleanup_wrapper (ಶೂನ್ಯ *p)
{
  ರಚನೆ libcriu_wrapper_s **w;

  w = (ರಚನೆ libcriu_wrapper_s **) p;
  ಆದರೆ (*w == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;

#  ifndef STATIC
  ಆದರೆ ((*w)->handle)
    dlclose ((*w)->handle);
#  endif
  ಬಿಡುಗಡೆ (*w);
  libcriu_wrapper = ಖಾಲಿ;

  ಆಗ (n_restored_ext_mounts > 0)
    ಬಿಡುಗಡೆ (restored_ext_mounts[--n_restored_ext_mounts].ಗುರಿ);
  ಬಿಡುಗಡೆ (restored_ext_mounts);
  restored_ext_mounts = ಖಾಲಿ;
}

#  define cleanup_wrapper __attribute__ ((cleanup (cleanup_wrapper)))

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ತುಂಬು_wrapper (ರಚನೆ libcriu_wrapper_s **wrapper_ಹೊರಹೋಗು, libcrun_error_t *err)
{
  cleanup_free ರಚನೆ libcriu_wrapper_s *wrapper = xmalloc0 (ಗಾತ್ರ (*wrapper));

#  ifdef STATIC
#    define ತುಂಬು_CRIU_FUNCTION(X, ALLOW_NULL) \
      wrapper->X = &X;
#  ಇಲ್ಲದಿದ್ದರೆ
#    define ತುಂಬು_CRIU_FUNCTION(X, ALLOW_NULL)                                                    \
      ಮಾಡು                                                                                         \
        {                                                                                        \
          wrapper->X = dlsym (wrapper->handle, #X);                                              \
          ಆದರೆ (! ALLOW_NULL && wrapper->X == ಖಾಲಿ)                                                \
            {                                                                                    \
              dlclose (wrapper->handle);                                                         \
              ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "could not find symbol `%s` in `libcriu.so`", #X); \
            }                                                                                    \
      } ಆಗ (0)
#  endif

#  ifndef STATIC
  wrapper->handle = dlopen ("libcriu.so.2", RTLD_NOW);
  ಆದರೆ (wrapper->handle == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "could not load `libcriu.so.2`: `%s`", dlerror ());
#  endif

  ತುಂಬು_CRIU_FUNCTION (criu_ಕೂಡಿಸು_ext_mount, false);
  ತುಂಬು_CRIU_FUNCTION (criu_ಕೂಡಿಸು_external, false);
  ತುಂಬು_CRIU_FUNCTION (criu_ಕೂಡಿಸು_inherit_fd, false);
  ತುಂಬು_CRIU_FUNCTION (criu_check_ಆವೃತ್ತಿ, false);
  ತುಂಬು_CRIU_FUNCTION (criu_dump, false);
  ತುಂಬು_CRIU_FUNCTION (criu_get_orphan_pts_master_fd, false);
  ತುಂಬು_CRIU_FUNCTION (criu_init_opts, false);
  ತುಂಬು_CRIU_FUNCTION (criu_notify_pid, false);

#  ifdef CRIU_JOIN_NS_SUPPORT
  /* criu_join_ns_add() API was introduced with CRIU version 3.16.1
   * Here we check if this API is available at build time to support
   * compiling with older version of CRIU, and at runtime to support
   * running crun with older versions of libcriu.so.2.
   */
  ತುಂಬು_CRIU_FUNCTION (criu_join_ns_ಕೂಡಿಸು, true);
#  endif

#  ifdef CRIU_PRE_DUMP_SUPPORT
  ತುಂಬು_CRIU_FUNCTION (criu_feature_check, false);
  ತುಂಬು_CRIU_FUNCTION (criu_pre_dump, false);
#  endif
  ತುಂಬು_CRIU_FUNCTION (criu_restore_ಮಗು, false);
  ತುಂಬು_CRIU_FUNCTION (criu_ಗಣ_ext_unix_sk, false);
  ತುಂಬು_CRIU_FUNCTION (criu_ಗಣ_file_locks, false);
  ತುಂಬು_CRIU_FUNCTION (criu_ಗಣ_freeze_cgroup, false);
  ತುಂಬು_CRIU_FUNCTION (criu_ಗಣ_images_dir_fd, false);
  ತುಂಬು_CRIU_FUNCTION (criu_ಗಣ_leave_running, false);
  ತುಂಬು_CRIU_FUNCTION (criu_ಗಣ_log_file, false);
  ತುಂಬು_CRIU_FUNCTION (criu_ಗಣ_log_level, false);
  ತುಂಬು_CRIU_FUNCTION (criu_ಗಣ_manage_cgroups, false);
  ತುಂಬು_CRIU_FUNCTION (criu_ಗಣ_manage_cgroups_ಬಹುಲಕ, false);
  ತುಂಬು_CRIU_FUNCTION (criu_ಗಣ_ಜಾಲ_ಬೀಗ, true);
  ತುಂಬು_CRIU_FUNCTION (criu_ಗಣ_notify_cb, false);
  ತುಂಬು_CRIU_FUNCTION (criu_ಗಣ_orphan_pts_master, false);
  ತುಂಬು_CRIU_FUNCTION (criu_ಗಣ_ಪೋಷಕ_images, false);
  ತುಂಬು_CRIU_FUNCTION (criu_ಗಣ_pid, false);
  ತುಂಬು_CRIU_FUNCTION (criu_ಗಣ_ಬೇರು, false);
  ತುಂಬು_CRIU_FUNCTION (criu_ಕೂಡಿಸು_cg_ಬೇರು, false);
  ತುಂಬು_CRIU_FUNCTION (criu_ಗಣ_shell_ಕೆಲಸ, false);
  ತುಂಬು_CRIU_FUNCTION (criu_ಗಣ_tcp_established, false);
  ತುಂಬು_CRIU_FUNCTION (criu_ಗಣ_tcp_close, false);
  ತುಂಬು_CRIU_FUNCTION (criu_ಗಣ_track_mem, false);
  ತುಂಬು_CRIU_FUNCTION (criu_ಗಣ_work_dir_fd, false);
  ತುಂಬು_CRIU_FUNCTION (criu_ಗಣ_lsm_profile, false);
  ತುಂಬು_CRIU_FUNCTION (criu_ಗಣ_lsm_mount_context, false);
#  ಆದರೆ ! defined STATIC || defined CRIU_CONFIG_FILE
  ತುಂಬು_CRIU_FUNCTION (criu_ಗಣ_config_file, true);
#  endif

  libcriu_wrapper = *wrapper_ಹೊರಹೋಗು = wrapper;
  wrapper = ಖಾಲಿ;
#  undef ತುಂಬು_CRIU_FUNCTION
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಕೂಡಿಸು_restored_ext_mount (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags)
{
  restored_ext_mounts = xrealloc (restored_ext_mounts, ಗಾತ್ರ (*restored_ext_mounts) * (n_restored_ext_mounts + 1));
  restored_ext_mounts[n_restored_ext_mounts].ಗುರಿ = xstrdup (ಗುರಿ);
  restored_ext_mounts[n_restored_ext_mounts].flags = flags;
  n_restored_ext_mounts++;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
criu_notify (ಅಕ್ಷರ *action, criu_notify_arg_t na)
{
  ಆದರೆ (strcmp (action, "post-restore") == 0)
    {
      /* CRIU takes the source of an external mount from us and its flags
       * from the image, i.e. from whatever mount was at that path in the
       * container when it was checkpointed.  The container can move its
       * mount points around, so before it is resumed make sure that each
       * source is mounted with the restrictions the configuration asks for. */
      libcrun_error_t tmp_err = ಖಾಲಿ;
      ಪೂರ್ಣಾಂಕ ret;

      ret = libcrun_restrict_mounts_ಒಳ_container (libcriu_wrapper->criu_notify_pid (na), restored_ext_mounts,
                                                  n_restored_ext_mounts, &tmp_err);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          libcrun_error (tmp_err->ಸ್ಥಿತಿ_2, "cannot set the flags of the restored mounts: %s", tmp_err->ಸಂದೇಶ);
          libcrun_error_release (&tmp_err);
          ಹಿಂತಿರುಗಿ ret;
        }
    }

  ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ (action, "orphan-pts-master", 17) == 0)
    {
      /* CRIU sends us the master FD via the 'orphan-pts-master'
       * callback and we are passing it on to the '--console-socket'
       * if it exists. */
      cleanup_close ಪೂರ್ಣಾಂಕ console_socket_fd = -1;
      libcrun_error_t tmp_err = ಖಾಲಿ;
      ಪೂರ್ಣಾಂಕ master_fd;
      ಪೂರ್ಣಾಂಕ ret;

      ಆದರೆ (! console_socket)
        ಹಿಂತಿರುಗಿ 0;

      master_fd = libcriu_wrapper->criu_get_orphan_pts_master_fd ();

      console_socket_fd = open_unix_domain_client_socket (console_socket, 0, &tmp_err);
      ಆದರೆ (UNLIKELY (console_socket_fd < 0))
        {
          libcrun_error_release (&tmp_err);
          ಹಿಂತಿರುಗಿ console_socket_fd;
        }
      ret = send_fd_ಗಮ್ಯ_socket (console_socket_fd, master_fd, &tmp_err);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          libcrun_error_release (&tmp_err);
          ಹಿಂತಿರುಗಿ ret;
        }
    }
  ಹಿಂತಿರುಗಿ 0;
}

#  ifdef CRIU_PRE_DUMP_SUPPORT

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
criu_check_mem_track (libcrun_error_t *err)
{
  ರಚನೆ criu_feature_check features = { 0 };
  ಪೂರ್ಣಾಂಕ ret;

  /* Right now we are only interested in checking memory tracking.
   * Memory tracking can be disabled at different levels. aarch64
   * for example has memory tracking not implemented. It could also
   * be not enabled on other architectures. Just ask CRIU if that
   * features exists. */

  features.mem_track = true;

  ret = libcriu_wrapper->criu_feature_check (&features, ಗಾತ್ರ (features));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "CRIU feature checking failed: %d", ret);

  ಆದರೆ (features.mem_track == true)
    ಹಿಂತಿರುಗಿ 1;

  ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "CRIU memory tracking not supported");
}

#  endif

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ರಿಜಿಸ್ಟರ್_masked_paths_mounts (runtime_spec_schema_config_schema *def, libcrun_container_t *container,
                              ರಚನೆ libcriu_wrapper_s *libcriu_wrapper, bool is_restore, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *ಖಾಲಿಗಳು_dir_ಮಾರ್ಗ = ಖಾಲಿ;
  bool shared_dir_registered = false;
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  ಗಾಗಿ (i = 0; i < def->linux->masked_paths_len; i++)
    {
      ರಚನೆ stat statbuf;
      ret = stat (def->linux->masked_paths[i], &statbuf);
      ಆದರೆ (ret != 0)
        ಮುಂದುವರಿ;

      ಆದರೆ (S_ISDIR (statbuf.st_mode))
        {
          ಆದರೆ (! shared_dir_registered)
            {
              ret = get_shared_ಖಾಲಿಗಳು_directory_ಮಾರ್ಗ (&ಖಾಲಿಗಳು_dir_ಮಾರ್ಗ,
                                                     (container->context ? container->context->state_ಬೇರು : ಖಾಲಿ), err);
              ಆದರೆ (UNLIKELY (ret < 0))
                ಹಿಂತಿರುಗಿ ret;

              ret = libcriu_wrapper->criu_ಕೂಡಿಸು_ext_mount (ಖಾಲಿಗಳು_dir_ಮಾರ್ಗ, ಖಾಲಿಗಳು_dir_ಮಾರ್ಗ);
              ಆದರೆ (UNLIKELY (ret < 0))
                ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "CRIU: failed adding external mount for shared empty directory `%s`", ಖಾಲಿಗಳು_dir_ಮಾರ್ಗ);

              shared_dir_registered = true;
            }

          ret = libcriu_wrapper->criu_ಕೂಡಿಸು_ext_mount (def->linux->masked_paths[i], ಖಾಲಿಗಳು_dir_ಮಾರ್ಗ);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "CRIU: failed adding external mount for masked directory `%s`", def->linux->masked_paths[i]);

          ಆದರೆ (is_restore)
            ಕೂಡಿಸು_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (S_ISREG (statbuf.st_mode))
        {
          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *bind_ಗುರಿ = is_restore ? "/dev/null" : def->linux->masked_paths[i];
          ret = libcriu_wrapper->criu_ಕೂಡಿಸು_ext_mount (def->linux->masked_paths[i], bind_ಗುರಿ);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", bind_ಗುರಿ);

          ಆದರೆ (is_restore)
            ಕೂಡಿಸು_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* Parse one /proc/self/cgroup LINE into its subsystem, normalizing the name and
   stripping any "name=" prefix.  When SUBPATH_OUT is not NULL, the in-hierarchy
   path is stored there.  Returns the subsystem, or NULL if the line carries no
   controller and should be skipped.  LINE is modified in place.  */
ಸ್ಥಿರ ಅಕ್ಷರ *
parse_cgroup_subsystem (ಅಕ್ಷರ *ಸಾಲು, ಅಕ್ಷರ **subpath_ಹೊರಹೋಗು)
{
  ಅಕ್ಷರ *subsystem, *subpath, *it;

  subsystem = strchr (ಸಾಲು, ':') + 1;
  subpath = strchr (subsystem, ':') + 1;
  *(subpath - 1) = '\0';

  ಆದರೆ (subsystem[0] == '\0')
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

  it = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು (subsystem, "name=");
  ಆದರೆ (it)
    subsystem = it + 5;

  ಆದರೆ (strcmp (subsystem, "net_prio,net_cls") == 0)
    subsystem = "net_cls,net_prio";
  ಆದರೆ (strcmp (subsystem, "cpuacct,cpu") == 0)
    subsystem = "cpu,cpuacct";

  ಆದರೆ (subpath_ಹೊರಹೋಗು)
    *subpath_ಹೊರಹೋಗು = subpath;
  ಹಿಂತಿರುಗಿ subsystem;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
restore_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *content = ಖಾಲಿ;
  bool has_cgroup_mount = false;
  ಅಕ್ಷರ *saveptr = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ;
  ಅಕ್ಷರ *from;
  ಪೂರ್ಣಾಂಕ ret;
  uint32_t i;

  cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (cgroup_ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

  ಆದರೆ (cgroup_ಬಹುಲಕ == CGROUP_ಬಹುಲಕ_UNIFIED)
    ಹಿಂತಿರುಗಿ 0;

  /* First check if there is actually a cgroup mount in the container. */
  ಗಾಗಿ (i = 0; i < def->mounts_len; i++)
    {
      ಅಕ್ಷರ *type = def->mounts[i]->type;
      ಆದರೆ (type && strcmp (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          ನಿಲ್ಲಿಸು;
        }
    }

  ಆದರೆ (! has_cgroup_mount)
    ಹಿಂತಿರುಗಿ 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, ಖಾಲಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (UNLIKELY (content == ಖಾಲಿ || content[0] == '\0'))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  ಗಾಗಿ (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ಖಾಲಿ, "\n", &saveptr))
    {
      cleanup_free ಅಕ್ಷರ *destination = ಖಾಲಿ;
      cleanup_free ಅಕ್ಷರ *ಮೂಲ = ಖಾಲಿ;
      ಅಕ್ಷರ *subsystem;
      ಅಕ್ಷರ *subpath;

      subsystem = parse_cgroup_subsystem (from, &subpath);
      ಆದರೆ (subsystem == ಖಾಲಿ)
        ಮುಂದುವರಿ;

      ret = append_paths (&ಮೂಲ, err, CGROUP_ಬೇರು, subsystem, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = append_paths (&destination, err, ಮೂಲ, subpath, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = libcriu_wrapper->criu_ಕೂಡಿಸು_ext_mount (ಮೂಲ, destination);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", destination);
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
checkpoint_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *content = ಖಾಲಿ;
  bool has_cgroup_mount = false;
  ಅಕ್ಷರ *saveptr = ಖಾಲಿ;
  ಅಕ್ಷರ *from;
  ಪೂರ್ಣಾಂಕ ret;
  uint32_t i;

  /* First check if there is actually a cgroup mount in the container. */
  ಗಾಗಿ (i = 0; i < def->mounts_len; i++)
    {
      ಅಕ್ಷರ *type = def->mounts[i]->type;
      ಆದರೆ (type && strcmp (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          ನಿಲ್ಲಿಸು;
        }
    }

  ಆದರೆ (! has_cgroup_mount)
    ಹಿಂತಿರುಗಿ 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, ಖಾಲಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (UNLIKELY (content == ಖಾಲಿ || content[0] == '\0'))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  ಗಾಗಿ (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ಖಾಲಿ, "\n", &saveptr))
    {
      cleanup_free ಅಕ್ಷರ *ಮೂಲ_ಮಾರ್ಗ = ಖಾಲಿ;
      ಅಕ್ಷರ *subsystem;

      subsystem = parse_cgroup_subsystem (from, ಖಾಲಿ);
      ಆದರೆ (subsystem == ಖಾಲಿ)
        ಮುಂದುವರಿ;

      ret = append_paths (&ಮೂಲ_ಮಾರ್ಗ, err, CGROUP_ಬೇರು, subsystem, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = libcriu_wrapper->criu_ಕೂಡಿಸು_ext_mount (ಮೂಲ_ಮಾರ್ಗ, ಮೂಲ_ಮಾರ್ಗ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", ಮೂಲ_ಮಾರ್ಗ);
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
handle_criu_config_file (libcrun_container_t *container, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *criu_config_annotation;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *config_file = CRIU_RUNC_CONFIG_FILE;

  criu_config_annotation = ಕಂಡುಹಿಡಿ_annotation (container, "org.criu.config");

  /* Ignore missing criu_set_config_file() API for compatibility with older CRIU versions,
   * and show an error only if config file is explicitly set with annotation.
   */
  ಆದರೆ (libcriu_wrapper->criu_ಗಣ_config_file == ಖಾಲಿ)
    {
      ಆದರೆ (criu_config_annotation)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "libcriu RPC config files supported in CRIU >= 4.2");

      ಹಿಂತಿರುಗಿ 0;
    }

  ಆದರೆ (criu_config_annotation)
    config_file = criu_config_annotation;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (access (CRIU_CRUN_CONFIG_FILE, F_OK) == 0)
    config_file = CRIU_CRUN_CONFIG_FILE;

  ret = libcriu_wrapper->criu_ಗಣ_config_file (config_file);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "failed to set CRIU config file");

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
validate_criu_ಆವೃತ್ತಿ (libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  // validate that the libcriu version is at least LIBCRIU_MIN_VERSION
  ret = libcriu_wrapper->criu_check_ಆವೃತ್ತಿ (LIBCRIU_MIN_ಆವೃತ್ತಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "CRIU: failed checking version");

  ಆದರೆ (ret == 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "libcriu is too old");

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ
show_criu_log (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *work_ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಲಘುಗಣಕ)
{
  cleanup_free ಅಕ್ಷರ *log_ಮಾರ್ಗ = ಖಾಲಿ;
  libcrun_error_t tmp_err = ಖಾಲಿ;
  ಅಕ್ಷರ ಸಾಲು[1024];
  FILE *f;

  ಆದರೆ (UNLIKELY (append_paths (&log_ಮಾರ್ಗ, &tmp_err, work_ಮಾರ್ಗ, ಲಘುಗಣಕ, ಖಾಲಿ) < 0))
    {
      crun_error_release (&tmp_err);
      ಹಿಂತಿರುಗಿ;
    }

  f = fopen (log_ಮಾರ್ಗ, "r");
  ಆದರೆ (f == ಖಾಲಿ)
    {
      ಆದರೆ (errno != ENOENT)
        libcrun_error (errno, "Can't open CRIU log `%s`", log_ಮಾರ್ಗ);
      ಹಿಂತಿರುಗಿ;
    }

  /* Log with error verbosity as this is the default. */
  libcrun_error (0, "--- excerpt from CRIU log `%s`", log_ಮಾರ್ಗ);
  ಆಗ (fgets (ಸಾಲು, ಗಾತ್ರ (ಸಾಲು), f) != ಖಾಲಿ)
    ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು (ಸಾಲು, "Error ") != ಖಾಲಿ)
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಸಿದ್ಧತೆ_criu_work_dir (libcrun_checkpoint_restore_t *cr_options, ಪೂರ್ಣಾಂಕ *work_fd_ಹೊರಹೋಗು, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ work_fd;

  ಆದರೆ (cr_options->work_ಮಾರ್ಗ == ಖಾಲಿ)
    {
      /* This is only for the error message later. */
      cr_options->work_ಮಾರ್ಗ = cr_options->image_ಮಾರ್ಗ;
      ಹಿಂತಿರುಗಿ 0;
    }

  ಆದರೆ (UNLIKELY ((mkdir (cr_options->work_ಮಾರ್ಗ, 0700) == -1) && (errno != EEXIST)))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "error creating CRIU work directory `%s`", cr_options->work_ಮಾರ್ಗ);

  work_fd = open (cr_options->work_ಮಾರ್ಗ, O_DIRECTORY | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (work_fd == -1))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "error opening CRIU work directory `%s`", cr_options->work_ಮಾರ್ಗ);

  libcriu_wrapper->criu_ಗಣ_work_dir_fd (work_fd);
  *work_fd_ಹೊರಹೋಗು = work_fd;
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_container_checkpoint_linux_criu (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, libcrun_container_t *container,
                                         libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper ರಚನೆ libcriu_wrapper_s *wrapper = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *descriptors_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *freezer_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ image_fd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ work_fd = -1;
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ;
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  ret = ತುಂಬು_wrapper (&wrapper, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (geteuid ())
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "checkpointing requires root");

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
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_ಆವೃತ್ತಿ (err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (UNLIKELY (cr_options->image_ಮಾರ್ಗ == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "image path not set");

  ret = mkdir (cr_options->image_ಮಾರ್ಗ, 0700);
  ಆದರೆ (UNLIKELY ((ret == -1) && (errno != EEXIST)))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "error creating checkpoint directory `%s`", cr_options->image_ಮಾರ್ಗ);

  image_fd = open (cr_options->image_ಮಾರ್ಗ, O_DIRECTORY | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (image_fd == -1))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "error opening checkpoint directory `%s`", cr_options->image_ಮಾರ್ಗ);

  libcriu_wrapper->criu_ಗಣ_images_dir_fd (image_fd);

  /* Set up logging. */
  libcriu_wrapper->criu_ಗಣ_log_level (4);
  libcriu_wrapper->criu_ಗಣ_log_file (CRIU_CHECKPOINT_LOG_FILE);

  /* Set up CRIU config file */
  ಆದರೆ (UNLIKELY (handle_criu_config_file (container, err)))
    ಹಿಂತಿರುಗಿ -1;

  /* Setting the pid early as we can skip a lot of checkpoint setup if
   * we just do a pre-dump. The PID needs to be set always. Do it here.
   * The main process of the container is the process CRIU will checkpoint
   * and all of its children. */
  libcriu_wrapper->criu_ಗಣ_pid (ಸ್ಥಿತಿ_2->pid);

  ret = ಸಿದ್ಧತೆ_criu_work_dir (cr_options, &work_fd, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

#  ifdef CRIU_PRE_DUMP_SUPPORT

  {
    ಪೂರ್ಣಾಂಕ criu_can_mem_track = 0;
    /* If the user uses --pre-dump for the second time or does
     * a final dump from a previous pre-dump, setting parent_path
     * is necessary so that CRIU can find which pages have not
     * changed compared to the previous dump. */
    ಆದರೆ (cr_options->ಪೋಷಕ_ಮಾರ್ಗ != ಖಾಲಿ)
      {
        criu_can_mem_track = criu_check_mem_track (err);
        ಆದರೆ (UNLIKELY (criu_can_mem_track == -1))
          ಹಿಂತಿರುಗಿ -1;
        libcriu_wrapper->criu_ಗಣ_track_mem (true);

        /* The parent path must be relative to image path (something like ../previous-dump).
           CRIU will fail with an unclear error message if the path is not right.
         */
        ಆದರೆ (UNLIKELY (cr_options->ಪೋಷಕ_ಮಾರ್ಗ[0] == '/'))
          ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "--parent-path must be relative");
        ಪೂರ್ಣಾಂಕ is_dir = crun_dir_p_at (image_fd, cr_options->ಪೋಷಕ_ಮಾರ್ಗ, false, err);
        ಆದರೆ (UNLIKELY (is_dir <= 0))
          {
            ಆದರೆ (is_dir < 0)
              ಹಿಂತಿರುಗಿ crun_error_wrap (err, "invalid --parent-path");
            ಹಿಂತಿರುಗಿ crun_make_error (err, ENOTDIR, "invalid --parent-path");
          }
        ret = libcriu_wrapper->criu_ಗಣ_ಪೋಷಕ_images (cr_options->ಪೋಷಕ_ಮಾರ್ಗ);
        ಆದರೆ (UNLIKELY (ret != 0))
          ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "error setting CRIU parent images path to `%s`", cr_options->ಪೋಷಕ_ಮಾರ್ಗ);
      }

    ಆದರೆ (cr_options->pre_dump)
      {
        ಆದರೆ (criu_can_mem_track != 1)
          {
            criu_can_mem_track = criu_check_mem_track (err);
            ಆದರೆ (UNLIKELY (criu_can_mem_track == -1))
              ಹಿಂತಿರುಗಿ -1;
          }
        libcriu_wrapper->criu_ಗಣ_track_mem (true);
        ret = libcriu_wrapper->criu_pre_dump ();
        ಆದರೆ (UNLIKELY (ret != 0))
          {
            show_criu_log (cr_options->work_ಮಾರ್ಗ, CRIU_CHECKPOINT_LOG_FILE);
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "CRIU pre-dump failed: %d", ret);
          }
        ಹಿಂತಿರುಗಿ 0;
      }
  }
#  endif

  /* descriptors.json is needed during restore to correctly
   * reconnect stdin, stdout, stderr. */
  ret = append_paths (&descriptors_ಮಾರ್ಗ, err, cr_options->image_ಮಾರ್ಗ, DESCRIPTORS_FILENAME, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = write_file (descriptors_ಮಾರ್ಗ, ಸ್ಥಿತಿ_2->external_descriptors, ಸುರಕ್ಷಿತ_strlen (ಸ್ಥಿತಿ_2->external_descriptors), err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_error_wrap (err, "error saving CRIU descriptors file");

  ret = append_paths (&ಮಾರ್ಗ, err, ಸ್ಥಿತಿ_2->bundle, ಸ್ಥಿತಿ_2->rootfs, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcriu_wrapper->criu_ಗಣ_ಬೇರು (ಮಾರ್ಗ);
  ಆದರೆ (UNLIKELY (ret != 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "error setting CRIU root to `%s`", ಮಾರ್ಗ);

  cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (cgroup_ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

  /* For cgroup v1 we need to tell CRIU to handle all cgroup mounts as external mounts. */
  ಆದರೆ (cgroup_ಬಹುಲಕ != CGROUP_ಬಹುಲಕ_UNIFIED)
    {
      ret = checkpoint_cgroup_v1_mount (def, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  /* Tell CRIU about external bind mounts. */
  ಗಾಗಿ (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      ಆದರೆ (is_bind_mount (def->mounts[i], ಖಾಲಿ, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          ಅಕ್ಷರ buf[PATH_MAX];
          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dest_ಒಳ_ಬೇರು;

          ಆದರೆ (nofollow)
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_ಒಳ_ಬೇರು = chroot_realpath (ಸ್ಥಿತಿ_2->rootfs, def->mounts[i]->destination, buf);
          ಆದರೆ (UNLIKELY (dest_ಒಳ_ಬೇರು == ಖಾಲಿ))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "unable to resolve external bind mount `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (dest_ಒಳ_ಬೇರು, ಸ್ಥಿತಿ_2->rootfs))
            dest_ಒಳ_ಬೇರು += ಸುರಕ್ಷಿತ_strlen (ಸ್ಥಿತಿ_2->rootfs);

          ret = libcriu_wrapper->criu_ಕೂಡಿಸು_ext_mount (dest_ಒಳ_ಬೇರು, dest_ಒಳ_ಬೇರು);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", def->mounts[i]->destination);
        }
    }

  ret = ರಿಜಿಸ್ಟರ್_masked_paths_mounts (def, container, libcriu_wrapper, false, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

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

  ಗಾಗಿ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ = libcrun_ಕಂಡುಹಿಡಿ_namespace (def->linux->namespaces[i]->type);
      ಆದರೆ (UNLIKELY (ಮೌಲ್ಯ < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      ಆದರೆ (ಮೌಲ್ಯ == CLONE_NEWNET && def->linux->namespaces[i]->ಮಾರ್ಗ != ಖಾಲಿ)
        {
          cleanup_free ಅಕ್ಷರ *external = ಖಾಲಿ;
          ರಚನೆ stat statbuf;

          ret = stat (def->linux->namespaces[i]->ಮಾರ್ಗ, &statbuf);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "unable to stat(): `%s`", def->linux->namespaces[i]->ಮಾರ್ಗ);

          xasprintf (&external, "net[%ld]:" CRIU_EXT_NETNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_ಕೂಡಿಸು_external (external);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }

      ಆದರೆ (ಮೌಲ್ಯ == CLONE_NEWPID && def->linux->namespaces[i]->ಮಾರ್ಗ != ಖಾಲಿ)
        {
          cleanup_free ಅಕ್ಷರ *external = ಖಾಲಿ;
          ರಚನೆ stat statbuf;

          ret = stat (def->linux->namespaces[i]->ಮಾರ್ಗ, &statbuf);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "unable to stat(): `%s`", def->linux->namespaces[i]->ಮಾರ್ಗ);

          xasprintf (&external, "pid[%ld]:" CRIU_EXT_PIDNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_ಕೂಡಿಸು_external (external);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }
    }

  /* Tell CRIU to use the freezer to pause all container processes. */
  ಆದರೆ (cgroup_ಬಹುಲಕ == CGROUP_ಬಹುಲಕ_UNIFIED)
    {
      /* This needs CRIU 3.14. */
      ret = append_paths (&freezer_ಮಾರ್ಗ, err, CGROUP_ಬೇರು, ಸ್ಥಿತಿ_2->cgroup_ಮಾರ್ಗ, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ret = append_paths (&freezer_ಮಾರ್ಗ, err, CGROUP_ಬೇರು "/freezer", ಸ್ಥಿತಿ_2->cgroup_ಮಾರ್ಗ, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ret = libcriu_wrapper->criu_ಗಣ_freeze_cgroup (freezer_ಮಾರ್ಗ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "CRIU: failed setting freezer %d", ret);

  /* Set boolean options . */
  libcriu_wrapper->criu_ಗಣ_leave_running (cr_options->leave_running);
  libcriu_wrapper->criu_ಗಣ_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_ಗಣ_shell_ಕೆಲಸ (cr_options->shell_ಕೆಲಸ);
  libcriu_wrapper->criu_ಗಣ_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_ಗಣ_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_ಗಣ_orphan_pts_master (true);
  libcriu_wrapper->criu_ಗಣ_manage_cgroups_ಬಹುಲಕ (criu_cg_ಬಹುಲಕ_from_ಬಹಿರಂಗ (cr_options->manage_cgroups_ಬಹುಲಕ));

  libcriu_wrapper->criu_ಗಣ_manage_cgroups (true);

  ಆದರೆ (libcriu_wrapper->criu_ಗಣ_ಜಾಲ_ಬೀಗ)
    {
      ಪೂರ್ಣಾಂಕ ಬೀಗ = 0;

      ret = criu_ಜಾಲ_ಬೀಗ_from_ಬಹಿರಂಗ (cr_options->ಜಾಲ_ಬೀಗ_ವಿಧಾನ, &ಬೀಗ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ಆದರೆ (ಬೀಗ > 0)
        {
          ret = libcriu_wrapper->criu_ಗಣ_ಜಾಲ_ಬೀಗ (ಬೀಗ);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "CRIU: failed setting network lock");
        }
    }

  ret = libcriu_wrapper->criu_dump ();
  ಆದರೆ (UNLIKELY (ret != 0))
    {
      show_criu_log (cr_options->work_ಮಾರ್ಗ, CRIU_CHECKPOINT_LOG_FILE);
      ಹಿಂತಿರುಗಿ crun_make_error (err, ret < 0 ? -ret : 0, "CRIU checkpointing failed: %d", ret);
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
prepare_restore_mounts_ಆಂತರಿಕ (runtime_spec_schema_config_schema *def, ಅಕ್ಷರ *ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **mounted,
                                 size_t *n_mounted, libcrun_error_t *err)
{
  uint32_t i;

  /* Go through all mountpoints to be able to recreate missing mountpoints. */
  ಗಾಗಿ (i = 0; i < def->mounts_len; i++)
    {
      ಅಕ್ಷರ *dest = def->mounts[i]->destination;
      ಅಕ್ಷರ *type = def->mounts[i]->type;
      cleanup_close ಪೂರ್ಣಾಂಕ ಬೇರು_fd = -1;
      bool nofollow = false;
      bool on_tmpfs = false;
      ಪೂರ್ಣಾಂಕ is_dir = 1;
      size_t j;

      /* cgroup restore should be handled by CRIU itself */
      ಆದರೆ (type && (strcmp (type, "cgroup") == 0 || strcmp (type, "cgroup2") == 0))
        ಮುಂದುವರಿ;

      /* Check if the mountpoint is on a tmpfs. CRIU restores
       * all tmpfs. We do need to recreate directories on a tmpfs. */
      size_t dest_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (dest);
      ಗಾಗಿ (j = 0; j < def->mounts_len; j++)
        {
          ಆದರೆ (def->mounts[j]->type == ಖಾಲಿ || strcmp (def->mounts[j]->type, "tmpfs") != 0)
            ಮುಂದುವರಿ;
          size_t mount_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (def->mounts[j]->destination);
          ಆದರೆ (mount_len < dest_len && dest[mount_len] == '/' && ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ (dest, def->mounts[j]->destination, mount_len) == 0)
            {
              /* This is a mountpoint which is on a tmpfs.*/
              on_tmpfs = true;
              ನಿಲ್ಲಿಸು;
            }
        }

      ಆದರೆ (on_tmpfs)
        ಮುಂದುವರಿ;

      /* For bind mounts check if the source is a file or a directory. */
      ಆದರೆ (is_bind_mount (def->mounts[i], ಖಾಲಿ, &nofollow))
        {
          ಆದರೆ (nofollow)
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          is_dir = crun_dir_p (def->mounts[i]->ಮೂಲ, false, err);
          ಆದರೆ (UNLIKELY (is_dir < 0))
            ಹಿಂತಿರುಗಿ is_dir;
        }

      ಬೇರು_fd = open (ಬೇರು, O_RDONLY | O_CLOEXEC);
      ಆದರೆ (UNLIKELY (ಬೇರು_fd == -1))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "error opening container root directory `%s`", ಬೇರು);

      ಆದರೆ (is_dir)
        {
          ಪೂರ್ಣಾಂಕ ret;

          ret = crun_ಸುರಕ್ಷಿತ_ensure_directory_at (ಬೇರು_fd, ಬೇರು, dest, 0755, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಪೂರ್ಣಾಂಕ ret;

          ret = crun_ಸುರಕ್ಷಿತ_ensure_file_at (ಬೇರು_fd, ಬೇರು, dest, 0755, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }

      /* Mount a bind mount source now, so that the mountpoints of the
       * mounts inside it (e.g. a nested bind mount) are created in the
       * source, where CRIU expects them.  This also happens during the
       * initial container creation, as the mounts are done in order.  */
      ಆದರೆ (is_bind_mount (def->mounts[i], ಖಾಲಿ, &nofollow))
        {
          cleanup_close ಪೂರ್ಣಾಂಕ dst_fd = -1;
          proc_fd_ಮಾರ್ಗ_t fd_ಮಾರ್ಗ;

          dst_fd = ಸುರಕ್ಷಿತ_openat (ಬೇರು_fd, ಬೇರು, dest, O_ಮಾರ್ಗ | O_CLOEXEC, 0, err);
          ಆದರೆ (UNLIKELY (dst_fd < 0))
            ಹಿಂತಿರುಗಿ dst_fd;

          /* Not recursive: the source might contain ROOT itself (e.g. when
           * it is the bundle directory), and it is not needed anyway.  */
          get_proc_self_fd_ಮಾರ್ಗ (fd_ಮಾರ್ಗ, dst_fd);
          ಆದರೆ (UNLIKELY (mount (def->mounts[i]->ಮೂಲ, fd_ಮಾರ್ಗ, ಖಾಲಿ, MS_BIND, ಖಾಲಿ) < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "bind mount `%s` to `%s`", def->mounts[i]->ಮೂಲ, dest);

          mounted[(*n_mounted)++] = dest;
        }
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* Move the current process back to CGROUPS, as read from /proc/self/cgroup.
   A failure is not fatal, so only warn about it.  */
ಸ್ಥಿರ ಶೂನ್ಯ
move_ಹಿಂಭಾಗ_ಗಮ್ಯ_cgroups (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cgroups)
{
  libcrun_error_t tmp_err = ಖಾಲಿ;

  ಆದರೆ (UNLIKELY (libcrun_move_self_ಗಮ್ಯ_cgroups (cgroups, &tmp_err) < 0))
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
criu_restore_ಮಗು_ಒಳ_cgroup (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ *criu_ret, libcrun_error_t *err)
{
  sigset_t all_ಸಂಕೇತಗಳು, old_ಮುಖವಾಡ;
  ಪೂರ್ಣಾಂಕ wait_ಸ್ಥಿತಿ = 0;
  pid_t pid;
  ಪೂರ್ಣಾಂಕ ret;

  *criu_ret = -1;

  sigfillset (&all_ಸಂಕೇತಗಳು);
  ret = sigprocmask (SIG_ಖಂಡ, &all_ಸಂಕೇತಗಳು, &old_ಮುಖವಾಡ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "sigprocmask");

  /* Must be vfork: the child shares our memory space, so both *criu_ret and
     the error it creates are visible here once it is gone.  */
  pid = vfork ();
  ಆದರೆ (UNLIKELY (pid < 0))
    {
      ಪೂರ್ಣಾಂಕ saved_errno = errno;

      sigprocmask (SIG_SETMASK, &old_ಮುಖವಾಡ, ಖಾಲಿ);
      ಹಿಂತಿರುಗಿ crun_make_error (err, saved_errno, "vfork");
    }

  ಆದರೆ (pid == 0)
    {
      ರಚನೆ sigaction act;
      ಪೂರ್ಣಾಂಕ i;

      /* The signal dispositions are not shared with the parent, so resetting
         them here does not affect it.  Keep the ignored signals ignored, as
         CRIU would inherit them if it was run directly by the caller.  */
      ಗಾಗಿ (i = 1; i < NSIG; i++)
        {
          ಆದರೆ (sigaction (i, ಖಾಲಿ, &act) < 0 || act.sa_handler == SIG_IGN || act.sa_handler == SIG_DFL)
            ಮುಂದುವರಿ;

          memset (&act, 0, ಗಾತ್ರ (act));
          act.sa_handler = SIG_DFL;
          sigaction (i, &act, ಖಾಲಿ);
        }
      sigprocmask (SIG_SETMASK, &old_ಮುಖವಾಡ, ಖಾಲಿ);

      ಆದರೆ (! is_ಖಾಲಿಗಳು_string (cgroup_ಮಾರ್ಗ))
        {
          ret = libcrun_move_process_ಗಮ್ಯ_cgroup (0, 0, cgroup_ಮಾರ್ಗ, false, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಸುರಕ್ಷಿತ_exit (EXIT_FAILURE);
        }

      *criu_ret = libcriu_wrapper->criu_restore_ಮಗು ();
      ಸುರಕ್ಷಿತ_exit (EXIT_SUCCESS);
    }

  /* The child is gone by now, it is safe to handle signals again.  */
  sigprocmask (SIG_SETMASK, &old_ಮುಖವಾಡ, ಖಾಲಿ);

  ret = waitpid_ignore_stopped (pid, &wait_ಸ್ಥಿತಿ, 0);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      /* The caller might have reaped the child already, e.g. when it ignores
         SIGCHLD.  Its result is still visible, so use it.  */
      ಆದರೆ (errno != ECHILD)
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "waitpid for the CRIU restore process");

      /* The child might have failed before calling CRIU, and left an error.  */
      ಹಿಂತಿರುಗಿ *err != ಖಾಲಿ ? -1 : 0;
    }

  ret = get_process_exit_ಸ್ಥಿತಿ (wait_ಸ್ಥಿತಿ);
  ಆದರೆ (UNLIKELY (ret != EXIT_SUCCESS))
    {
      /* The child creates the error in the shared memory space, do not
         overwrite it with crun_make_error().  */
      ಆದರೆ (*err == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "the CRIU restore process exited with status %d", ret);
      ಹಿಂತಿರುಗಿ -1;
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* Recreate the mountpoints which do not exist in the container rootfs
 * mounted at ROOT, the same way it is done on the container creation.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
prepare_restore_mounts (runtime_spec_schema_config_schema *def, ಅಕ್ಷರ *ಬೇರು, libcrun_error_t *err)
{
  cleanup_free ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **mounted = xmalloc0 (ಗಾತ್ರ (ಅಕ್ಷರ *) * (def->mounts_len + 1));
  cleanup_close ಪೂರ್ಣಾಂಕ ಬೇರು_fd = -1;
  size_t n_mounted = 0;
  ಪೂರ್ಣಾಂಕ ret;

  ret = prepare_restore_mounts_ಆಂತರಿಕ (def, ಬೇರು, mounted, &n_mounted, err);

  /* The bind mounts done above are only needed to create the mountpoints,
   * and CRIU restores the mounts itself, so undo them, in reverse order. */
  ಆದರೆ (n_mounted > 0)
    ಬೇರು_fd = open (ಬೇರು, O_ಮಾರ್ಗ | O_CLOEXEC);
  ಆಗ (ಬೇರು_fd >= 0 && n_mounted > 0)
    {
      libcrun_error_t tmp_err = ಖಾಲಿ;
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dest = mounted[--n_mounted];
      cleanup_close ಪೂರ್ಣಾಂಕ dst_fd = -1;
      proc_fd_ಮಾರ್ಗ_t fd_ಮಾರ್ಗ;

      dst_fd = ಸುರಕ್ಷಿತ_openat (ಬೇರು_fd, ಬೇರು, dest, O_ಮಾರ್ಗ | O_CLOEXEC, 0, &tmp_err);
      ಆದರೆ (UNLIKELY (dst_fd < 0))
        {
          crun_error_release (&tmp_err);
          ಮುಂದುವರಿ;
        }
      get_proc_self_fd_ಮಾರ್ಗ (fd_ಮಾರ್ಗ, dst_fd);
      ಆದರೆ (UNLIKELY (umount2 (fd_ಮಾರ್ಗ, MNT_DETACH) < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "unmount `%s`", dest);
    }

  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
libcrun_container_restore_linux_criu (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, libcrun_container_t *container,
                                      libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper ರಚನೆ libcriu_wrapper_s *wrapper = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ inherit_new_net_fd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ inherit_new_pid_fd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ image_fd = -1;
  cleanup_free ಅಕ್ಷರ *ಬೇರು = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *bundle_cleanup = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *own_cgroups = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ work_fd = -1;
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ;
  ಪೂರ್ಣಾಂಕ criu_ret;
  ಪೂರ್ಣಾಂಕ ret_ಹೊರಹೋಗು;
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  ret = ತುಂಬು_wrapper (&wrapper, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (geteuid ())
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "restoring requires root");

  ret = libcriu_wrapper->criu_init_opts ();
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_ಆವೃತ್ತಿ (err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (UNLIKELY (cr_options->image_ಮಾರ್ಗ == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "image path not set");

  image_fd = open (cr_options->image_ಮಾರ್ಗ, O_DIRECTORY | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (image_fd == -1))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "error opening checkpoint directory `%s`", cr_options->image_ಮಾರ್ಗ);

  libcriu_wrapper->criu_ಗಣ_images_dir_fd (image_fd);

  /* Load descriptors.json to tell CRIU where those FDs should be connected to. */
  {
    cleanup_free ಅಕ್ಷರ *descriptors_ಮಾರ್ಗ = ಖಾಲಿ;
    cleanup_free ಅಕ್ಷರ *ಬಫರ್ = ಖಾಲಿ;
    json_object *doc = ಖಾಲಿ;

    ret = append_paths (&descriptors_ಮಾರ್ಗ, err, cr_options->image_ಮಾರ್ಗ, DESCRIPTORS_FILENAME, ಖಾಲಿ);
    ಆದರೆ (UNLIKELY (ret < 0))
      ಹಿಂತಿರುಗಿ ret;

    ret = read_all_file (descriptors_ಮಾರ್ಗ, &ಬಫರ್, ಖಾಲಿ, err);
    ಆದರೆ (UNLIKELY (ret < 0))
      ಹಿಂತಿರುಗಿ ret;

    /* descriptors.json contains a JSON array with strings
     * telling where 0, 1 and 2 have been initially been
     * pointing to. For each descriptor which points to
     * a pipe 'pipe:' we tell CRIU to reconnect that pipe
     * to the corresponding FD to have (especially) stdout
     * and stderr being correctly redirected. */
    ret = parse_json_file (&doc, ಬಫರ್, ಖಾಲಿ, err);
    ಆದರೆ (UNLIKELY (ret < 0))
      ಹಿಂತಿರುಗಿ ret;

    ಆದರೆ (json_object_is_type (doc, json_type_array))
      {
        size_t i, len = json_object_array_ಉದ್ದಳತೆ (doc);

        /* len will probably always be 3 as crun is currently only
         * recording the destination of FD 0, 1 and 2. */
        ಗಾಗಿ (i = 0; i < len; ++i)
          {
            json_object *s = json_object_array_get_idx (doc, i);
            ಆದರೆ (s && json_object_is_type (s, json_type_string))
              {
                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str = json_object_get_string (s);
                ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (str, "pipe:"))
                  libcriu_wrapper->criu_ಕೂಡಿಸು_inherit_fd (i, str);
              }
          }
      }
    json_object_put (doc);
  }

  /* Our std streams are the new external descriptors, as CRIU connects the
   * inherited pipes to them.  Record them now: once the container is
   * restored it is already running, and /proc/PID/fd is under its control. */
  ret = libcrun_save_external_descriptors (container, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = ಸಿದ್ಧತೆ_criu_work_dir (cr_options, &work_fd, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (cr_options->lsm_profile != ಖಾಲಿ)
    {
      ret = libcriu_wrapper->criu_ಗಣ_lsm_profile (cr_options->lsm_profile);
      ಆದರೆ (UNLIKELY (ret != 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "error setting LSM profile to `%s`", cr_options->lsm_profile);
    }

  ಆದರೆ (cr_options->lsm_mount_context != ಖಾಲಿ)
    {
      ret = libcriu_wrapper->criu_ಗಣ_lsm_mount_context (cr_options->lsm_mount_context);
      ಆದರೆ (UNLIKELY (ret != 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "error setting LSM mount context to `%s`", cr_options->lsm_mount_context);
    }

  /* do realpath on root */
  bundle_cleanup = realpath (ಸ್ಥಿತಿ_2->bundle, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (bundle_cleanup == ಖಾಲಿ))
    bundle_cleanup = xstrdup (ಸ್ಥಿತಿ_2->bundle);

  /* Tell CRIU about external bind mounts. */
  ಗಾಗಿ (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      ಆದರೆ (is_bind_mount (def->mounts[i], ಖಾಲಿ, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          ಅಕ್ಷರ buf[PATH_MAX];
          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dest_ಒಳ_ಬೇರು;
          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೂಲ = def->mounts[i]->ಮೂಲ;
          cleanup_free ಅಕ್ಷರ *abs_ಮೂಲ = ಖಾಲಿ;
          ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags = 0;
          size_t j;

          ಆದರೆ (nofollow)
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_ಒಳ_ಬೇರು = chroot_realpath (ಸ್ಥಿತಿ_2->rootfs, def->mounts[i]->destination, buf);
          ಆದರೆ (UNLIKELY (dest_ಒಳ_ಬೇರು == ಖಾಲಿ))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "unable to resolve external bind mount destination `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (dest_ಒಳ_ಬೇರು, ಸ್ಥಿತಿ_2->rootfs))
            dest_ಒಳ_ಬೇರು += ಸುರಕ್ಷಿತ_strlen (ಸ್ಥಿತಿ_2->rootfs);

          /* A relative source is relative to the bundle, while CRIU would
             resolve it relative to its own working directory.  */
          ಆದರೆ (ಮೂಲ && ಮೂಲ[0] != '/')
            {
              ret = append_paths (&abs_ಮೂಲ, err, bundle_cleanup, ಮೂಲ, ಖಾಲಿ);
              ಆದರೆ (UNLIKELY (ret < 0))
                ಹಿಂತಿರುಗಿ ret;
              ಮೂಲ = abs_ಮೂಲ;
            }

          ret = libcriu_wrapper->criu_ಕೂಡಿಸು_ext_mount (dest_ಒಳ_ಬೇರು, ಮೂಲ);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", ಮೂಲ);

          ಗಾಗಿ (j = 0; j < def->mounts[i]->options_len; j++)
            {
              ಸ್ಥಿರಾಂಕ ರಚನೆ ಪ್ರಸರಣ_flags_s *prop = ಖಾಲಿ;

              ಆದರೆ (def->mounts[i]->options[j])
                prop = libcrun_str2mount_flags (def->mounts[i]->options[j]);
              ಆದರೆ (prop && ! prop->clear)
                flags |= prop->flags;
            }
          ಕೂಡಿಸು_restored_ext_mount (dest_ಒಳ_ಬೇರು, flags);
        }
    }

  ret = ರಿಜಿಸ್ಟರ್_masked_paths_mounts (def, container, libcriu_wrapper, true, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  /* Mount the container rootfs for CRIU. */
  ret = append_paths (&ಬೇರು, err, bundle_cleanup, "criu-root", ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = mkdir (ಬೇರು, 0755);
  ಆದರೆ (UNLIKELY (ret == -1))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "error creating restore directory `%s`", ಬೇರು);

  ret = mount (ಸ್ಥಿತಿ_2->rootfs, ಬೇರು, ಖಾಲಿ, MS_BIND | MS_REC, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret == -1))
    {
      ret = crun_make_error (err, errno, "error mounting restore directory `%s`", ಬೇರು);
      ಹೋಗು ಹೊರಹೋಗು;
    }

  /* During initial container creation, crun will create mountpoints
   * defined in config.json if they do not exist. If we are restoring
   * we need to make sure these mountpoints also exist.
   * This is not perfect, as this means crun will modify a rootfs
   * even if it marked as read-only, but runc already modifies
   * the rootfs in the same way. */

  ret = prepare_restore_mounts (def, ಬೇರು, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು ಹೊರಹೋಗು_umount;

  ret = libcriu_wrapper->criu_ಗಣ_ಬೇರು (ಬೇರು);
  ಆದರೆ (UNLIKELY (ret != 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU root to `%s`", ಬೇರು);
      ಹೋಗು ಹೊರಹೋಗು_umount;
    }

  /* If a namespace defined in config.json we are telling
   * CRIU use that namespace when restoring the process tree.
   *
   * CRIU expects the information about the namespace like this:
   * --inherit-fd fd[<fd>]:<key>
   * The <key> needs to be the same as during checkpointing (extRootNetNS). */
  ಗಾಗಿ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ open_flags_for_inherit = O_RDONLY; /* Cannot be O_CLOEXEC as it is passed to the child process. */
      ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ = libcrun_ಕಂಡುಹಿಡಿ_namespace (def->linux->namespaces[i]->type);
      ಆದರೆ (UNLIKELY (ಮೌಲ್ಯ < 0))
        {
          ret = crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);
          ಹೋಗು ಹೊರಹೋಗು_umount;
        }

      ಆದರೆ (ಮೌಲ್ಯ == CLONE_NEWNET && def->linux->namespaces[i]->ಮಾರ್ಗ != ಖಾಲಿ)
        {
          inherit_new_net_fd = open (def->linux->namespaces[i]->ಮಾರ್ಗ, open_flags_for_inherit);
          ಆದರೆ (UNLIKELY (inherit_new_net_fd < 0))
            {
              ret = crun_make_error (err, errno, "unable to open(): `%s`", def->linux->namespaces[i]->ಮಾರ್ಗ);
              ಹೋಗು ಹೊರಹೋಗು_umount;
            }

          ret = libcriu_wrapper->criu_ಕೂಡಿಸು_inherit_fd (inherit_new_net_fd, CRIU_EXT_NETNS);
          ಆದರೆ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              ಹೋಗು ಹೊರಹೋಗು_umount;
            }
        }

      ಆದರೆ (ಮೌಲ್ಯ == CLONE_NEWPID && def->linux->namespaces[i]->ಮಾರ್ಗ != ಖಾಲಿ)
        {
          inherit_new_pid_fd = open (def->linux->namespaces[i]->ಮಾರ್ಗ, open_flags_for_inherit);
          ಆದರೆ (UNLIKELY (inherit_new_pid_fd < 0))
            {
              ret = crun_make_error (err, errno, "unable to open(): `%s`", def->linux->namespaces[i]->ಮಾರ್ಗ);
              ಹೋಗು ಹೊರಹೋಗು_umount;
            }

          ret = libcriu_wrapper->criu_ಕೂಡಿಸು_inherit_fd (inherit_new_pid_fd, CRIU_EXT_PIDNS);
          ಆದರೆ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              ಹೋಗು ಹೊರಹೋಗು_umount;
            }
        }

#  ifdef CRIU_JOIN_NS_SUPPORT
      ಆದರೆ (ಮೌಲ್ಯ == CLONE_NEWTIME && def->linux->namespaces[i]->ಮಾರ್ಗ != ಖಾಲಿ)
        {
          ಆದರೆ (libcriu_wrapper->criu_join_ns_ಕೂಡಿಸು == ಖಾಲಿ)
            {
              ret = crun_make_error (err, 0, "shared time namespace restore is supported in CRIU >= 3.16.1");
              ಹೋಗು ಹೊರಹೋಗು_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_ಕೂಡಿಸು ("time", def->linux->namespaces[i]->ಮಾರ್ಗ, ಖಾಲಿ);
          ಆದರೆ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->ಮಾರ್ಗ);
              ಹೋಗು ಹೊರಹೋಗು_umount;
            }
        }

      ಆದರೆ (ಮೌಲ್ಯ == CLONE_NEWIPC && def->linux->namespaces[i]->ಮಾರ್ಗ != ಖಾಲಿ)
        {
          ಆದರೆ (libcriu_wrapper->criu_join_ns_ಕೂಡಿಸು == ಖಾಲಿ)
            {
              ret = crun_make_error (err, 0, "shared ipc namespace restore is supported in CRIU >= 3.16.1");
              ಹೋಗು ಹೊರಹೋಗು_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_ಕೂಡಿಸು ("ipc", def->linux->namespaces[i]->ಮಾರ್ಗ, ಖಾಲಿ);
          ಆದರೆ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->ಮಾರ್ಗ);
              ಹೋಗು ಹೊರಹೋಗು_umount;
            }
        }

      ಆದರೆ (ಮೌಲ್ಯ == CLONE_NEWUTS && def->linux->namespaces[i]->ಮಾರ್ಗ != ಖಾಲಿ)
        {
          ಆದರೆ (libcriu_wrapper->criu_join_ns_ಕೂಡಿಸು == ಖಾಲಿ)
            {
              ret = crun_make_error (err, 0, "shared uts namespace restore is supported in CRIU >= 3.16.1");
              ಹೋಗು ಹೊರಹೋಗು_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_ಕೂಡಿಸು ("uts", def->linux->namespaces[i]->ಮಾರ್ಗ, ಖಾಲಿ);
          ಆದರೆ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->ಮಾರ್ಗ);
              ಹೋಗು ಹೊರಹೋಗು_umount;
            }
        }
#  endif
    }

  /* Set up CRIU config file */
  ret = handle_criu_config_file (container, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು ಹೊರಹೋಗು_umount;

  /* Tell CRIU if cgroup v1 needs to be handled. */
  ret = restore_cgroup_v1_mount (def, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು ಹೊರಹೋಗು_umount;

  console_socket = cr_options->console_socket;
  libcriu_wrapper->criu_ಗಣ_notify_cb (criu_notify);

  /* Set boolean options . */
  libcriu_wrapper->criu_ಗಣ_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_ಗಣ_shell_ಕೆಲಸ (cr_options->shell_ಕೆಲಸ);
  libcriu_wrapper->criu_ಗಣ_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_ಗಣ_tcp_close (cr_options->tcp_close);
  libcriu_wrapper->criu_ಗಣ_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_ಗಣ_orphan_pts_master (true);

  ಆದರೆ (ಸ್ಥಿತಿ_2->cgroup_ಮಾರ್ಗ)
    {
      cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
      ಆದರೆ (UNLIKELY (cgroup_ಬಹುಲಕ < 0))
        {
          ret = cgroup_ಬಹುಲಕ;
          ಹೋಗು ಹೊರಹೋಗು_umount;
        }

      /* With cgroup v2, the cgroup path is only meaningful for the unified
         hierarchy, so only relocate that one.  A NULL controller would make
         CRIU relocate every hierarchy the container is in, named cgroup v1
         ones included.  As CRIU dumps such a hierarchy from wherever the
         container is in it, which is its root, as crun leaves named
         hierarchies alone, every checkpoint and restore would then copy the
         whole hierarchy into itself.  */
      ret = libcriu_wrapper->criu_ಕೂಡಿಸು_cg_ಬೇರು (cgroup_ಬಹುಲಕ == CGROUP_ಬಹುಲಕ_UNIFIED ? "" : ಖಾಲಿ,
                                               ಸ್ಥಿತಿ_2->cgroup_ಮಾರ್ಗ);
      ಆದರೆ (UNLIKELY (ret != 0))
        {
          ret = crun_make_error (err, 0, "error setting CRIU cgroup root to `%s`", ಸ್ಥಿತಿ_2->cgroup_ಮಾರ್ಗ);
          ಹೋಗು ಹೊರಹೋಗು_umount;
        }
    }

  libcriu_wrapper->criu_ಗಣ_manage_cgroups_ಬಹುಲಕ (criu_cg_ಬಹುಲಕ_from_ಬಹಿರಂಗ (cr_options->manage_cgroups_ಬಹುಲಕ));
  libcriu_wrapper->criu_ಗಣ_manage_cgroups (true);

  ಆದರೆ (libcriu_wrapper->criu_ಗಣ_ಜಾಲ_ಬೀಗ)
    {
      ಪೂರ್ಣಾಂಕ ಬೀಗ = 0;

      ret = criu_ಜಾಲ_ಬೀಗ_from_ಬಹಿರಂಗ (cr_options->ಜಾಲ_ಬೀಗ_ವಿಧಾನ, &ಬೀಗ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹೋಗು ಹೊರಹೋಗು_umount;

      ಆದರೆ (ಬೀಗ > 0)
        {
          ret = libcriu_wrapper->criu_ಗಣ_ಜಾಲ_ಬೀಗ (ಬೀಗ);
          ಆದರೆ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "CRIU: failed setting network lock");
              ಹೋಗು ಹೊರಹೋಗು_umount;
            }
        }
    }

  libcriu_wrapper->criu_ಗಣ_log_level (4);
  ret = libcriu_wrapper->criu_ಗಣ_log_file (CRIU_RESTORE_LOG_FILE);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU log file to `%s`", CRIU_RESTORE_LOG_FILE);
      ಹೋಗು ಹೊರಹೋಗು_umount;
    }

  /* criu_restore() returns the PID of the process of the restored process
   * tree. This PID will not be the same as status->pid if the container is
   * running in a PID namespace. But it will always be > 0. */
  ಆದರೆ (cr_options->detach)
    {
      /* Nothing waits for the container, so there is no reason to take over
         this process: use a throw-away one.  It is used even when there is no
         cgroup to join, so that the container is not left as a child of a
         process which is not going to reap it.  */
      ret = criu_restore_ಮಗು_ಒಳ_cgroup (ಸ್ಥಿತಿ_2->cgroup_ಮಾರ್ಗ, &criu_ret, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹೋಗು ಹೊರಹೋಗು_umount;

      ret = criu_ret;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* Like runc does, put CRIU into the container cgroup for the time of
       * restore, so the restored tasks are created there. It is necessary in
       * the "ignore" mode, where CRIU does not deal with cgroups at all, and
       * does not hurt otherwise. As CRIU is our child, do it by moving
       * ourselves there, and move back once the restore is done. */
      ಆದರೆ (! is_ಖಾಲಿಗಳು_string (ಸ್ಥಿತಿ_2->cgroup_ಮಾರ್ಗ))
        {
          ret = read_all_file (PROC_SELF_CGROUP, &own_cgroups, ಖಾಲಿ, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹೋಗು ಹೊರಹೋಗು_umount;

          ret = libcrun_move_process_ಗಮ್ಯ_cgroup (0, 0, ಸ್ಥಿತಿ_2->cgroup_ಮಾರ್ಗ, false, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            {
              /* Some of the cgroups might have been joined already.  */
              move_ಹಿಂಭಾಗ_ಗಮ್ಯ_cgroups (own_cgroups);
              ಹೋಗು ಹೊರಹೋಗು_umount;
            }
        }

      ret = libcriu_wrapper->criu_restore_ಮಗು ();

      ಆದರೆ (own_cgroups)
        move_ಹಿಂಭಾಗ_ಗಮ್ಯ_cgroups (own_cgroups);
    }

  ಆದರೆ (UNLIKELY (ret <= 0))
    {
      show_criu_log (cr_options->work_ಮಾರ್ಗ, CRIU_RESTORE_LOG_FILE);
      ret = crun_make_error (err, 0, "CRIU restoring failed: %d", ret);
      ಹೋಗು ಹೊರಹೋಗು_umount;
    }

  /* Update the status struct with the newly allocated PID. This will
   * be necessary later when moving the process into its cgroup. */
  ಸ್ಥಿತಿ_2->pid = ret;
  ret = 0;

ಹೊರಹೋಗು_umount:
  ret_ಹೊರಹೋಗು = umount (ಬೇರು);
  ಆದರೆ (UNLIKELY (ret_ಹೊರಹೋಗು == -1))
    {
      ಪೂರ್ಣಾಂಕ saved_errno = errno;
      rmdir (ಬೇರು);
      ಆದರೆ (ret < 0)
        ಹಿಂತಿರುಗಿ crun_error_wrap (err, "error unmounting restore directory `%s`", ಬೇರು);
      ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ crun_make_error (err, saved_errno, "error unmounting restore directory `%s`", ಬೇರು);
    }
ಹೊರಹೋಗು:
  ret_ಹೊರಹೋಗು = rmdir (ಬೇರು);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;
  ಆದರೆ (UNLIKELY (ret_ಹೊರಹೋಗು == -1))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "error removing restore directory `%s`", ಬೇರು);
  ಹಿಂತಿರುಗಿ ret;
}
#ಆದರೆ_ಅಂತ್ಯ
