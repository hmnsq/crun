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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਜੇ HAVE_CRIU && HAVE_DLOPEN

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
ਅੱਖਰ *chroot_realpath (ਅਚਲ ਅੱਖਰ *chroot, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਅੱਖਰ resolved_ਰਸਤਾ[]);

/* Map the public cgroups mode (enum libcrun_cr_cgroups_mode) onto the CRIU enum.  */
ਸਥਿਰ ਗਿਣਤੀ criu_cg_ਬਹੁਲਕ
criu_cg_ਬਹੁਲਕ_from_ਜਨਤਕ (ਪੂਰਨ ਬਹੁਲਕ)
{
  ਚੋਣ (ਬਹੁਲਕ)
    {
    ਹਾਲਤ LIBCRUN_CR_CG_ਬਹੁਲਕ_IGNORE:
      ਵਾਪਸ CRIU_CG_ਬਹੁਲਕ_IGNORE;
    ਹਾਲਤ LIBCRUN_CR_CG_ਬਹੁਲਕ_ਭਰਿਆ:
      ਵਾਪਸ CRIU_CG_ਬਹੁਲਕ_ਭਰਿਆ;
    ਹਾਲਤ LIBCRUN_CR_CG_ਬਹੁਲਕ_STRICT:
      ਵਾਪਸ CRIU_CG_ਬਹੁਲਕ_STRICT;
    ਹਾਲਤ LIBCRUN_CR_CG_ਬਹੁਲਕ_DEFAULT:
    ਹਾਲਤ LIBCRUN_CR_CG_ਬਹੁਲਕ_SOFT:
    ਮੂਲ:
      /* Default to CRIU_CG_MODE_SOFT, just as runc.  */
      ਵਾਪਸ CRIU_CG_ਬਹੁਲਕ_SOFT;
    }
}

/* Map the public network lock method (enum libcrun_cr_network_lock_method) onto the
   CRIU enum.  *LOCK is set to 0 when no explicit method must be set (CRIU default).
   A method that this build cannot honor is an error, never a silent fallback to
   the CRIU default.  */
ਸਥਿਰ ਪੂਰਨ
criu_ਜਾਲ_ਤਾਲਾ_from_ਜਨਤਕ (ਪੂਰਨ ਵਿਧੀ, ਪੂਰਨ *ਤਾਲਾ, libcrun_error_t *err)
{
  ਚੋਣ (ਵਿਧੀ)
    {
    ਹਾਲਤ LIBCRUN_CR_ਜਾਲ_ਤਾਲਾ_IPTABLES:
      *ਤਾਲਾ = CRIU_ਜਾਲ_ਤਾਲਾ_IPTABLES;
      ਵਾਪਸ 0;
    ਹਾਲਤ LIBCRUN_CR_ਜਾਲ_ਤਾਲਾ_NFTABLES:
      *ਤਾਲਾ = CRIU_ਜਾਲ_ਤਾਲਾ_NFTABLES;
      ਵਾਪਸ 0;
    ਹਾਲਤ LIBCRUN_CR_ਜਾਲ_ਤਾਲਾ_SKIP:
#  ਜੇ CRIU_ਜਾਲ_ਤਾਲਾ_SKIP_SUPPORT
      *ਤਾਲਾ = CRIU_ਜਾਲ_ਤਾਲਾ_SKIP;
      ਵਾਪਸ 0;
#  ਨਹੀਂਤਾਂ
      ਵਾਪਸ crun_make_error (err, 0, "CRIU: the `skip` network lock method is not supported by this build");
#  endif
    ਹਾਲਤ LIBCRUN_CR_ਜਾਲ_ਤਾਲਾ_DEFAULT:
      *ਤਾਲਾ = 0;
      ਵਾਪਸ 0;
    ਮੂਲ:
      ਵਾਪਸ crun_make_error (err, 0, "CRIU: unknown network lock method %d", ਵਿਧੀ);
    }
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *console_socket = ਨਿਰਾ;

/* The external mounts CRIU restores, and the flags the configuration asks
   for each of them.  */
ਸਥਿਰ ਬਣਤਰ libcrun_mount_flags_s *restored_ext_mounts = ਨਿਰਾ;
ਸਥਿਰ size_t n_restored_ext_mounts = 0;

#  define LIBCRIU_MIN_ਸੰਸਕਰਣ 31500

ਬਣਤਰ libcriu_wrapper_s
{
  ਖਾਲੀ *handle;
  ਪੂਰਨ (*criu_ਜੋੜੋ_ext_mount) (ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ, ਅਚਲ ਅੱਖਰ *val);
  ਪੂਰਨ (*criu_ਜੋੜੋ_external) (ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ);
  ਪੂਰਨ (*criu_ਜੋੜੋ_inherit_fd) (ਪੂਰਨ fd, ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ);
  ਪੂਰਨ (*criu_check_ਸੰਸਕਰਣ) (ਪੂਰਨ minimum);
  ਪੂਰਨ (*criu_dump) (ਖਾਲੀ);
  ਪੂਰਨ (*criu_get_orphan_pts_master_fd) (ਖਾਲੀ);
  ਪੂਰਨ (*criu_init_opts) (ਖਾਲੀ);
  ਪੂਰਨ (*criu_notify_pid) (criu_notify_arg_t na);
#  ifdef CRIU_JOIN_NS_SUPPORT
  ਪੂਰਨ (*criu_join_ns_ਜੋੜੋ) (ਅਚਲ ਅੱਖਰ *ns, ਅਚਲ ਅੱਖਰ *ns_file, ਅਚਲ ਅੱਖਰ *extra_opt);
#  endif
#  ifdef CRIU_PRE_DUMP_SUPPORT
  ਪੂਰਨ (*criu_feature_check) (ਬਣਤਰ criu_feature_check *features, size_t ਮਾਪ);
  ਪੂਰਨ (*criu_pre_dump) (ਖਾਲੀ);
#  endif
  ਪੂਰਨ (*criu_restore_ਬੱਚਾ) (ਖਾਲੀ);
  ਪੂਰਨ (*criu_ਸੈੱਟ_freeze_cgroup) (ਅਚਲ ਅੱਖਰ *name);
  ਖਾਲੀ (*criu_ਸੈੱਟ_file_locks) (bool file_locks);
  ਖਾਲੀ (*criu_ਸੈੱਟ_ext_unix_sk) (bool ext_unix_sk);
  ਪੂਰਨ (*criu_ਸੈੱਟ_log_file) (ਅਚਲ ਅੱਖਰ *log_file);
  ਖਾਲੀ (*criu_ਸੈੱਟ_log_level) (ਪੂਰਨ log_level);
  ਖਾਲੀ (*criu_ਸੈੱਟ_leave_running) (bool leave_running);
  ਖਾਲੀ (*criu_ਸੈੱਟ_manage_cgroups) (bool manage);
  ਖਾਲੀ (*criu_ਸੈੱਟ_manage_cgroups_ਬਹੁਲਕ) (ਗਿਣਤੀ criu_cg_ਬਹੁਲਕ ਬਹੁਲਕ);
  ਪੂਰਨ (*criu_ਸੈੱਟ_ਜਾਲ_ਤਾਲਾ) (ਗਿਣਤੀ criu_ਜਾਲ_ਤਾਲਾ_ਵਿਧੀ ਵਿਧੀ);
  ਖਾਲੀ (*criu_ਸੈੱਟ_notify_cb) (ਪੂਰਨ (*cb) (ਅੱਖਰ *action, criu_notify_arg_t na));
  ਖਾਲੀ (*criu_ਸੈੱਟ_orphan_pts_master) (bool orphan_pts_master);
  ਖਾਲੀ (*criu_ਸੈੱਟ_images_dir_fd) (ਪੂਰਨ fd);
  ਪੂਰਨ (*criu_ਸੈੱਟ_ਮਾਪਾ_images) (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ);
  ਖਾਲੀ (*criu_ਸੈੱਟ_pid) (ਪੂਰਨ pid);
  ਪੂਰਨ (*criu_ਸੈੱਟ_ਜੜ੍ਹ) (ਅਚਲ ਅੱਖਰ *ਜੜ੍ਹ);
  ਪੂਰਨ (*criu_ਜੋੜੋ_cg_ਜੜ੍ਹ) (ਅਚਲ ਅੱਖਰ *ctrl, ਅਚਲ ਅੱਖਰ *ਰਸਤਾ);
  ਖਾਲੀ (*criu_ਸੈੱਟ_shell_ਕੰਮ) (bool shell_ਕੰਮ);
  ਖਾਲੀ (*criu_ਸੈੱਟ_tcp_established) (bool tcp_established);
  ਖਾਲੀ (*criu_ਸੈੱਟ_tcp_close) (bool tcp_close);
  ਖਾਲੀ (*criu_ਸੈੱਟ_track_mem) (bool track_mem);
  ਖਾਲੀ (*criu_ਸੈੱਟ_work_dir_fd) (ਪੂਰਨ fd);
  ਪੂਰਨ (*criu_ਸੈੱਟ_lsm_profile) (ਅਚਲ ਅੱਖਰ *name);
  ਪੂਰਨ (*criu_ਸੈੱਟ_lsm_mount_context) (ਅਚਲ ਅੱਖਰ *name);
  ਪੂਰਨ (*criu_ਸੈੱਟ_config_file) (ਅਚਲ ਅੱਖਰ *ਰਸਤਾ);
};

ਸਥਿਰ ਬਣਤਰ libcriu_wrapper_s *libcriu_wrapper;

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
cleanup_wrapper (ਖਾਲੀ *p)
{
  ਬਣਤਰ libcriu_wrapper_s **w;

  w = (ਬਣਤਰ libcriu_wrapper_s **) p;
  ਜੇ (*w == ਨਿਰਾ)
    ਵਾਪਸ;

#  ifndef STATIC
  ਜੇ ((*w)->handle)
    dlclose ((*w)->handle);
#  endif
  ਮੁਕਤ (*w);
  libcriu_wrapper = ਨਿਰਾ;

  ਜਦੋਂ (n_restored_ext_mounts > 0)
    ਮੁਕਤ (restored_ext_mounts[--n_restored_ext_mounts].ਨਿਸ਼ਾਨਾ);
  ਮੁਕਤ (restored_ext_mounts);
  restored_ext_mounts = ਨਿਰਾ;
}

#  define cleanup_wrapper __attribute__ ((cleanup (cleanup_wrapper)))

ਸਥਿਰ ਪੂਰਨ
ਲੱਦੋ_wrapper (ਬਣਤਰ libcriu_wrapper_s **wrapper_ਬਾਹਰਲਾ, libcrun_error_t *err)
{
  cleanup_free ਬਣਤਰ libcriu_wrapper_s *wrapper = xmalloc0 (ਆਕਾਰ (*wrapper));

#  ifdef STATIC
#    define ਲੱਦੋ_CRIU_FUNCTION(X, ALLOW_NULL) \
      wrapper->X = &X;
#  ਨਹੀਂਤਾਂ
#    define ਲੱਦੋ_CRIU_FUNCTION(X, ALLOW_NULL)                                                    \
      ਕਰੋ                                                                                         \
        {                                                                                        \
          wrapper->X = dlsym (wrapper->handle, #X);                                              \
          ਜੇ (! ALLOW_NULL && wrapper->X == ਨਿਰਾ)                                                \
            {                                                                                    \
              dlclose (wrapper->handle);                                                         \
              ਵਾਪਸ crun_make_error (err, 0, "could not find symbol `%s` in `libcriu.so`", #X); \
            }                                                                                    \
      } ਜਦੋਂ (0)
#  endif

#  ifndef STATIC
  wrapper->handle = dlopen ("libcriu.so.2", RTLD_NOW);
  ਜੇ (wrapper->handle == ਨਿਰਾ)
    ਵਾਪਸ crun_make_error (err, 0, "could not load `libcriu.so.2`: `%s`", dlerror ());
#  endif

  ਲੱਦੋ_CRIU_FUNCTION (criu_ਜੋੜੋ_ext_mount, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਜੋੜੋ_external, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਜੋੜੋ_inherit_fd, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_check_ਸੰਸਕਰਣ, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_dump, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_get_orphan_pts_master_fd, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_init_opts, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_notify_pid, false);

#  ifdef CRIU_JOIN_NS_SUPPORT
  /* criu_join_ns_add() API was introduced with CRIU version 3.16.1
   * Here we check if this API is available at build time to support
   * compiling with older version of CRIU, and at runtime to support
   * running crun with older versions of libcriu.so.2.
   */
  ਲੱਦੋ_CRIU_FUNCTION (criu_join_ns_ਜੋੜੋ, true);
#  endif

#  ifdef CRIU_PRE_DUMP_SUPPORT
  ਲੱਦੋ_CRIU_FUNCTION (criu_feature_check, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_pre_dump, false);
#  endif
  ਲੱਦੋ_CRIU_FUNCTION (criu_restore_ਬੱਚਾ, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਸੈੱਟ_ext_unix_sk, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਸੈੱਟ_file_locks, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਸੈੱਟ_freeze_cgroup, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਸੈੱਟ_images_dir_fd, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਸੈੱਟ_leave_running, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਸੈੱਟ_log_file, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਸੈੱਟ_log_level, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਸੈੱਟ_manage_cgroups, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਸੈੱਟ_manage_cgroups_ਬਹੁਲਕ, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਸੈੱਟ_ਜਾਲ_ਤਾਲਾ, true);
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਸੈੱਟ_notify_cb, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਸੈੱਟ_orphan_pts_master, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਸੈੱਟ_ਮਾਪਾ_images, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਸੈੱਟ_pid, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਸੈੱਟ_ਜੜ੍ਹ, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਜੋੜੋ_cg_ਜੜ੍ਹ, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਸੈੱਟ_shell_ਕੰਮ, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਸੈੱਟ_tcp_established, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਸੈੱਟ_tcp_close, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਸੈੱਟ_track_mem, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਸੈੱਟ_work_dir_fd, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਸੈੱਟ_lsm_profile, false);
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਸੈੱਟ_lsm_mount_context, false);
#  ਜੇ ! defined STATIC || defined CRIU_CONFIG_FILE
  ਲੱਦੋ_CRIU_FUNCTION (criu_ਸੈੱਟ_config_file, true);
#  endif

  libcriu_wrapper = *wrapper_ਬਾਹਰਲਾ = wrapper;
  wrapper = ਨਿਰਾ;
#  undef ਲੱਦੋ_CRIU_FUNCTION
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ
ਜੋੜੋ_restored_ext_mount (ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags)
{
  restored_ext_mounts = xrealloc (restored_ext_mounts, ਆਕਾਰ (*restored_ext_mounts) * (n_restored_ext_mounts + 1));
  restored_ext_mounts[n_restored_ext_mounts].ਨਿਸ਼ਾਨਾ = xstrdup (ਨਿਸ਼ਾਨਾ);
  restored_ext_mounts[n_restored_ext_mounts].flags = flags;
  n_restored_ext_mounts++;
}

ਸਥਿਰ ਪੂਰਨ
criu_notify (ਅੱਖਰ *action, criu_notify_arg_t na)
{
  ਜੇ (strcmp (action, "post-restore") == 0)
    {
      /* CRIU takes the source of an external mount from us and its flags
       * from the image, i.e. from whatever mount was at that path in the
       * container when it was checkpointed.  The container can move its
       * mount points around, so before it is resumed make sure that each
       * source is mounted with the restrictions the configuration asks for. */
      libcrun_error_t tmp_err = ਨਿਰਾ;
      ਪੂਰਨ ret;

      ret = libcrun_restrict_mounts_ਵਿੱਚ_container (libcriu_wrapper->criu_notify_pid (na), restored_ext_mounts,
                                                  n_restored_ext_mounts, &tmp_err);
      ਜੇ (UNLIKELY (ret < 0))
        {
          libcrun_error (tmp_err->ਸਥਿਤੀ, "cannot set the flags of the restored mounts: %s", tmp_err->ਸੁਨੇਹਾ);
          libcrun_error_release (&tmp_err);
          ਵਾਪਸ ret;
        }
    }

  ਜੇ (ਲੜੀ_ਤੁਲਨਾ (action, "orphan-pts-master", 17) == 0)
    {
      /* CRIU sends us the master FD via the 'orphan-pts-master'
       * callback and we are passing it on to the '--console-socket'
       * if it exists. */
      cleanup_close ਪੂਰਨ console_socket_fd = -1;
      libcrun_error_t tmp_err = ਨਿਰਾ;
      ਪੂਰਨ master_fd;
      ਪੂਰਨ ret;

      ਜੇ (! console_socket)
        ਵਾਪਸ 0;

      master_fd = libcriu_wrapper->criu_get_orphan_pts_master_fd ();

      console_socket_fd = open_unix_domain_client_socket (console_socket, 0, &tmp_err);
      ਜੇ (UNLIKELY (console_socket_fd < 0))
        {
          libcrun_error_release (&tmp_err);
          ਵਾਪਸ console_socket_fd;
        }
      ret = send_fd_ਮੰਜ਼ਿਲ_socket (console_socket_fd, master_fd, &tmp_err);
      ਜੇ (UNLIKELY (ret < 0))
        {
          libcrun_error_release (&tmp_err);
          ਵਾਪਸ ret;
        }
    }
  ਵਾਪਸ 0;
}

#  ifdef CRIU_PRE_DUMP_SUPPORT

ਸਥਿਰ ਪੂਰਨ
criu_check_mem_track (libcrun_error_t *err)
{
  ਬਣਤਰ criu_feature_check features = { 0 };
  ਪੂਰਨ ret;

  /* Right now we are only interested in checking memory tracking.
   * Memory tracking can be disabled at different levels. aarch64
   * for example has memory tracking not implemented. It could also
   * be not enabled on other architectures. Just ask CRIU if that
   * features exists. */

  features.mem_track = true;

  ret = libcriu_wrapper->criu_feature_check (&features, ਆਕਾਰ (features));
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, 0, "CRIU feature checking failed: %d", ret);

  ਜੇ (features.mem_track == true)
    ਵਾਪਸ 1;

  ਵਾਪਸ crun_make_error (err, 0, "CRIU memory tracking not supported");
}

#  endif

ਸਥਿਰ ਪੂਰਨ
ਰਜਿਸਟਰ_masked_paths_mounts (runtime_spec_schema_config_schema *def, libcrun_container_t *container,
                              ਬਣਤਰ libcriu_wrapper_s *libcriu_wrapper, bool is_restore, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *ਖਾਲੀ_ਥਾਵਾਂ_dir_ਰਸਤਾ = ਨਿਰਾ;
  bool shared_dir_registered = false;
  size_t i;
  ਪੂਰਨ ret;

  ਲਈ (i = 0; i < def->linux->masked_paths_len; i++)
    {
      ਬਣਤਰ stat statbuf;
      ret = stat (def->linux->masked_paths[i], &statbuf);
      ਜੇ (ret != 0)
        ਜਾਰੀ;

      ਜੇ (S_ISDIR (statbuf.st_mode))
        {
          ਜੇ (! shared_dir_registered)
            {
              ret = get_shared_ਖਾਲੀ_ਥਾਵਾਂ_directory_ਰਸਤਾ (&ਖਾਲੀ_ਥਾਵਾਂ_dir_ਰਸਤਾ,
                                                     (container->context ? container->context->state_ਜੜ੍ਹ : ਨਿਰਾ), err);
              ਜੇ (UNLIKELY (ret < 0))
                ਵਾਪਸ ret;

              ret = libcriu_wrapper->criu_ਜੋੜੋ_ext_mount (ਖਾਲੀ_ਥਾਵਾਂ_dir_ਰਸਤਾ, ਖਾਲੀ_ਥਾਵਾਂ_dir_ਰਸਤਾ);
              ਜੇ (UNLIKELY (ret < 0))
                ਵਾਪਸ crun_make_error (err, -ret, "CRIU: failed adding external mount for shared empty directory `%s`", ਖਾਲੀ_ਥਾਵਾਂ_dir_ਰਸਤਾ);

              shared_dir_registered = true;
            }

          ret = libcriu_wrapper->criu_ਜੋੜੋ_ext_mount (def->linux->masked_paths[i], ਖਾਲੀ_ਥਾਵਾਂ_dir_ਰਸਤਾ);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, -ret, "CRIU: failed adding external mount for masked directory `%s`", def->linux->masked_paths[i]);

          ਜੇ (is_restore)
            ਜੋੜੋ_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
      ਨਹੀਂਤਾਂ ਜੇ (S_ISREG (statbuf.st_mode))
        {
          ਅਚਲ ਅੱਖਰ *bind_ਨਿਸ਼ਾਨਾ = is_restore ? "/dev/null" : def->linux->masked_paths[i];
          ret = libcriu_wrapper->criu_ਜੋੜੋ_ext_mount (def->linux->masked_paths[i], bind_ਨਿਸ਼ਾਨਾ);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", bind_ਨਿਸ਼ਾਨਾ);

          ਜੇ (is_restore)
            ਜੋੜੋ_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
    }

  ਵਾਪਸ 0;
}

/* Parse one /proc/self/cgroup LINE into its subsystem, normalizing the name and
   stripping any "name=" prefix.  When SUBPATH_OUT is not NULL, the in-hierarchy
   path is stored there.  Returns the subsystem, or NULL if the line carries no
   controller and should be skipped.  LINE is modified in place.  */
ਸਥਿਰ ਅੱਖਰ *
parse_cgroup_subsystem (ਅੱਖਰ *ਸਤਰ, ਅੱਖਰ **subpath_ਬਾਹਰਲਾ)
{
  ਅੱਖਰ *subsystem, *subpath, *it;

  subsystem = strchr (ਸਤਰ, ':') + 1;
  subpath = strchr (subsystem, ':') + 1;
  *(subpath - 1) = '\0';

  ਜੇ (subsystem[0] == '\0')
    ਵਾਪਸ ਨਿਰਾ;

  it = ਲੜੀ_ਖੋਜ (subsystem, "name=");
  ਜੇ (it)
    subsystem = it + 5;

  ਜੇ (strcmp (subsystem, "net_prio,net_cls") == 0)
    subsystem = "net_cls,net_prio";
  ਜੇ (strcmp (subsystem, "cpuacct,cpu") == 0)
    subsystem = "cpu,cpuacct";

  ਜੇ (subpath_ਬਾਹਰਲਾ)
    *subpath_ਬਾਹਰਲਾ = subpath;
  ਵਾਪਸ subsystem;
}

ਸਥਿਰ ਪੂਰਨ
restore_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *content = ਨਿਰਾ;
  bool has_cgroup_mount = false;
  ਅੱਖਰ *saveptr = ਨਿਰਾ;
  ਪੂਰਨ cgroup_ਬਹੁਲਕ;
  ਅੱਖਰ *from;
  ਪੂਰਨ ret;
  uint32_t i;

  cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (cgroup_ਬਹੁਲਕ < 0))
    ਵਾਪਸ cgroup_ਬਹੁਲਕ;

  ਜੇ (cgroup_ਬਹੁਲਕ == CGROUP_ਬਹੁਲਕ_UNIFIED)
    ਵਾਪਸ 0;

  /* First check if there is actually a cgroup mount in the container. */
  ਲਈ (i = 0; i < def->mounts_len; i++)
    {
      ਅੱਖਰ *type = def->mounts[i]->type;
      ਜੇ (type && strcmp (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          ਰੋਕੋ;
        }
    }

  ਜੇ (! has_cgroup_mount)
    ਵਾਪਸ 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, ਨਿਰਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (UNLIKELY (content == ਨਿਰਾ || content[0] == '\0'))
    ਵਾਪਸ crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  ਲਈ (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ਨਿਰਾ, "\n", &saveptr))
    {
      cleanup_free ਅੱਖਰ *destination = ਨਿਰਾ;
      cleanup_free ਅੱਖਰ *ਸਰੋਤ = ਨਿਰਾ;
      ਅੱਖਰ *subsystem;
      ਅੱਖਰ *subpath;

      subsystem = parse_cgroup_subsystem (from, &subpath);
      ਜੇ (subsystem == ਨਿਰਾ)
        ਜਾਰੀ;

      ret = append_paths (&ਸਰੋਤ, err, CGROUP_ਜੜ੍ਹ, subsystem, ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = append_paths (&destination, err, ਸਰੋਤ, subpath, ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = libcriu_wrapper->criu_ਜੋੜੋ_ext_mount (ਸਰੋਤ, destination);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", destination);
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
checkpoint_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *content = ਨਿਰਾ;
  bool has_cgroup_mount = false;
  ਅੱਖਰ *saveptr = ਨਿਰਾ;
  ਅੱਖਰ *from;
  ਪੂਰਨ ret;
  uint32_t i;

  /* First check if there is actually a cgroup mount in the container. */
  ਲਈ (i = 0; i < def->mounts_len; i++)
    {
      ਅੱਖਰ *type = def->mounts[i]->type;
      ਜੇ (type && strcmp (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          ਰੋਕੋ;
        }
    }

  ਜੇ (! has_cgroup_mount)
    ਵਾਪਸ 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, ਨਿਰਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (UNLIKELY (content == ਨਿਰਾ || content[0] == '\0'))
    ਵਾਪਸ crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  ਲਈ (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ਨਿਰਾ, "\n", &saveptr))
    {
      cleanup_free ਅੱਖਰ *ਸਰੋਤ_ਰਸਤਾ = ਨਿਰਾ;
      ਅੱਖਰ *subsystem;

      subsystem = parse_cgroup_subsystem (from, ਨਿਰਾ);
      ਜੇ (subsystem == ਨਿਰਾ)
        ਜਾਰੀ;

      ret = append_paths (&ਸਰੋਤ_ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, subsystem, ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = libcriu_wrapper->criu_ਜੋੜੋ_ext_mount (ਸਰੋਤ_ਰਸਤਾ, ਸਰੋਤ_ਰਸਤਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", ਸਰੋਤ_ਰਸਤਾ);
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
handle_criu_config_file (libcrun_container_t *container, libcrun_error_t *err)
{
  ਪੂਰਨ ret;
  ਅਚਲ ਅੱਖਰ *criu_config_annotation;
  ਅਚਲ ਅੱਖਰ *config_file = CRIU_RUNC_CONFIG_FILE;

  criu_config_annotation = ਲੱਭੋ_annotation (container, "org.criu.config");

  /* Ignore missing criu_set_config_file() API for compatibility with older CRIU versions,
   * and show an error only if config file is explicitly set with annotation.
   */
  ਜੇ (libcriu_wrapper->criu_ਸੈੱਟ_config_file == ਨਿਰਾ)
    {
      ਜੇ (criu_config_annotation)
        ਵਾਪਸ crun_make_error (err, 0, "libcriu RPC config files supported in CRIU >= 4.2");

      ਵਾਪਸ 0;
    }

  ਜੇ (criu_config_annotation)
    config_file = criu_config_annotation;
  ਨਹੀਂਤਾਂ ਜੇ (access (CRIU_CRUN_CONFIG_FILE, F_OK) == 0)
    config_file = CRIU_CRUN_CONFIG_FILE;

  ret = libcriu_wrapper->criu_ਸੈੱਟ_config_file (config_file);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, 0, "failed to set CRIU config file");

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
validate_criu_ਸੰਸਕਰਣ (libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  // validate that the libcriu version is at least LIBCRIU_MIN_VERSION
  ret = libcriu_wrapper->criu_check_ਸੰਸਕਰਣ (LIBCRIU_MIN_ਸੰਸਕਰਣ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, -ret, "CRIU: failed checking version");

  ਜੇ (ret == 0)
    ਵਾਪਸ crun_make_error (err, 0, "libcriu is too old");

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ
show_criu_log (ਅਚਲ ਅੱਖਰ *work_ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *ਲਘੂਗਣਕ)
{
  cleanup_free ਅੱਖਰ *log_ਰਸਤਾ = ਨਿਰਾ;
  libcrun_error_t tmp_err = ਨਿਰਾ;
  ਅੱਖਰ ਸਤਰ[1024];
  FILE *f;

  ਜੇ (UNLIKELY (append_paths (&log_ਰਸਤਾ, &tmp_err, work_ਰਸਤਾ, ਲਘੂਗਣਕ, ਨਿਰਾ) < 0))
    {
      crun_error_release (&tmp_err);
      ਵਾਪਸ;
    }

  f = fopen (log_ਰਸਤਾ, "r");
  ਜੇ (f == ਨਿਰਾ)
    {
      ਜੇ (errno != ENOENT)
        libcrun_error (errno, "Can't open CRIU log `%s`", log_ਰਸਤਾ);
      ਵਾਪਸ;
    }

  /* Log with error verbosity as this is the default. */
  libcrun_error (0, "--- excerpt from CRIU log `%s`", log_ਰਸਤਾ);
  ਜਦੋਂ (fgets (ਸਤਰ, ਆਕਾਰ (ਸਤਰ), f) != ਨਿਰਾ)
    ਜੇ (ਲੜੀ_ਖੋਜ (ਸਤਰ, "Error ") != ਨਿਰਾ)
      {
        ਸਤਰ[strcspn (ਸਤਰ, "\n")] = '\0';
        libcrun_error (0, "%s", ਸਤਰ);
      }

  fclose (f);
  libcrun_error (0, "--- end of excerpt");
}

/* work_dir is the place CRIU will put its logfiles.  If not explicitly set, CRIU
 * will put the logfiles into the images_dir.  When set, create and open it and
 * hand the fd to CRIU; ownership of the returned fd stays with the caller.  */
ਸਥਿਰ ਪੂਰਨ
ਤਿਆਰੀ_criu_work_dir (libcrun_checkpoint_restore_t *cr_options, ਪੂਰਨ *work_fd_ਬਾਹਰਲਾ, libcrun_error_t *err)
{
  ਪੂਰਨ work_fd;

  ਜੇ (cr_options->work_ਰਸਤਾ == ਨਿਰਾ)
    {
      /* This is only for the error message later. */
      cr_options->work_ਰਸਤਾ = cr_options->image_ਰਸਤਾ;
      ਵਾਪਸ 0;
    }

  ਜੇ (UNLIKELY ((mkdir (cr_options->work_ਰਸਤਾ, 0700) == -1) && (errno != EEXIST)))
    ਵਾਪਸ crun_make_error (err, errno, "error creating CRIU work directory `%s`", cr_options->work_ਰਸਤਾ);

  work_fd = open (cr_options->work_ਰਸਤਾ, O_DIRECTORY | O_CLOEXEC);
  ਜੇ (UNLIKELY (work_fd == -1))
    ਵਾਪਸ crun_make_error (err, errno, "error opening CRIU work directory `%s`", cr_options->work_ਰਸਤਾ);

  libcriu_wrapper->criu_ਸੈੱਟ_work_dir_fd (work_fd);
  *work_fd_ਬਾਹਰਲਾ = work_fd;
  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_container_checkpoint_linux_criu (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, libcrun_container_t *container,
                                         libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper ਬਣਤਰ libcriu_wrapper_s *wrapper = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *descriptors_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *freezer_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *ਰਸਤਾ = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ image_fd = -1;
  cleanup_close ਪੂਰਨ work_fd = -1;
  ਪੂਰਨ cgroup_ਬਹੁਲਕ;
  size_t i;
  ਪੂਰਨ ret;

  ret = ਲੱਦੋ_wrapper (&wrapper, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (geteuid ())
    ਵਾਪਸ crun_make_error (err, 0, "checkpointing requires root");

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
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_ਸੰਸਕਰਣ (err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (UNLIKELY (cr_options->image_ਰਸਤਾ == ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, 0, "image path not set");

  ret = mkdir (cr_options->image_ਰਸਤਾ, 0700);
  ਜੇ (UNLIKELY ((ret == -1) && (errno != EEXIST)))
    ਵਾਪਸ crun_make_error (err, errno, "error creating checkpoint directory `%s`", cr_options->image_ਰਸਤਾ);

  image_fd = open (cr_options->image_ਰਸਤਾ, O_DIRECTORY | O_CLOEXEC);
  ਜੇ (UNLIKELY (image_fd == -1))
    ਵਾਪਸ crun_make_error (err, errno, "error opening checkpoint directory `%s`", cr_options->image_ਰਸਤਾ);

  libcriu_wrapper->criu_ਸੈੱਟ_images_dir_fd (image_fd);

  /* Set up logging. */
  libcriu_wrapper->criu_ਸੈੱਟ_log_level (4);
  libcriu_wrapper->criu_ਸੈੱਟ_log_file (CRIU_CHECKPOINT_LOG_FILE);

  /* Set up CRIU config file */
  ਜੇ (UNLIKELY (handle_criu_config_file (container, err)))
    ਵਾਪਸ -1;

  /* Setting the pid early as we can skip a lot of checkpoint setup if
   * we just do a pre-dump. The PID needs to be set always. Do it here.
   * The main process of the container is the process CRIU will checkpoint
   * and all of its children. */
  libcriu_wrapper->criu_ਸੈੱਟ_pid (ਸਥਿਤੀ->pid);

  ret = ਤਿਆਰੀ_criu_work_dir (cr_options, &work_fd, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

#  ifdef CRIU_PRE_DUMP_SUPPORT

  {
    ਪੂਰਨ criu_can_mem_track = 0;
    /* If the user uses --pre-dump for the second time or does
     * a final dump from a previous pre-dump, setting parent_path
     * is necessary so that CRIU can find which pages have not
     * changed compared to the previous dump. */
    ਜੇ (cr_options->ਮਾਪਾ_ਰਸਤਾ != ਨਿਰਾ)
      {
        criu_can_mem_track = criu_check_mem_track (err);
        ਜੇ (UNLIKELY (criu_can_mem_track == -1))
          ਵਾਪਸ -1;
        libcriu_wrapper->criu_ਸੈੱਟ_track_mem (true);

        /* The parent path must be relative to image path (something like ../previous-dump).
           CRIU will fail with an unclear error message if the path is not right.
         */
        ਜੇ (UNLIKELY (cr_options->ਮਾਪਾ_ਰਸਤਾ[0] == '/'))
          ਵਾਪਸ crun_make_error (err, 0, "--parent-path must be relative");
        ਪੂਰਨ is_dir = crun_dir_p_at (image_fd, cr_options->ਮਾਪਾ_ਰਸਤਾ, false, err);
        ਜੇ (UNLIKELY (is_dir <= 0))
          {
            ਜੇ (is_dir < 0)
              ਵਾਪਸ crun_error_wrap (err, "invalid --parent-path");
            ਵਾਪਸ crun_make_error (err, ENOTDIR, "invalid --parent-path");
          }
        ret = libcriu_wrapper->criu_ਸੈੱਟ_ਮਾਪਾ_images (cr_options->ਮਾਪਾ_ਰਸਤਾ);
        ਜੇ (UNLIKELY (ret != 0))
          ਵਾਪਸ crun_make_error (err, -ret, "error setting CRIU parent images path to `%s`", cr_options->ਮਾਪਾ_ਰਸਤਾ);
      }

    ਜੇ (cr_options->pre_dump)
      {
        ਜੇ (criu_can_mem_track != 1)
          {
            criu_can_mem_track = criu_check_mem_track (err);
            ਜੇ (UNLIKELY (criu_can_mem_track == -1))
              ਵਾਪਸ -1;
          }
        libcriu_wrapper->criu_ਸੈੱਟ_track_mem (true);
        ret = libcriu_wrapper->criu_pre_dump ();
        ਜੇ (UNLIKELY (ret != 0))
          {
            show_criu_log (cr_options->work_ਰਸਤਾ, CRIU_CHECKPOINT_LOG_FILE);
            ਵਾਪਸ crun_make_error (err, 0, "CRIU pre-dump failed: %d", ret);
          }
        ਵਾਪਸ 0;
      }
  }
#  endif

  /* descriptors.json is needed during restore to correctly
   * reconnect stdin, stdout, stderr. */
  ret = append_paths (&descriptors_ਰਸਤਾ, err, cr_options->image_ਰਸਤਾ, DESCRIPTORS_FILENAME, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = write_file (descriptors_ਰਸਤਾ, ਸਥਿਤੀ->external_descriptors, ਸੁਰੱਖਿਅਤ_strlen (ਸਥਿਤੀ->external_descriptors), err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_error_wrap (err, "error saving CRIU descriptors file");

  ret = append_paths (&ਰਸਤਾ, err, ਸਥਿਤੀ->bundle, ਸਥਿਤੀ->rootfs, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = libcriu_wrapper->criu_ਸੈੱਟ_ਜੜ੍ਹ (ਰਸਤਾ);
  ਜੇ (UNLIKELY (ret != 0))
    ਵਾਪਸ crun_make_error (err, 0, "error setting CRIU root to `%s`", ਰਸਤਾ);

  cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
  ਜੇ (UNLIKELY (cgroup_ਬਹੁਲਕ < 0))
    ਵਾਪਸ cgroup_ਬਹੁਲਕ;

  /* For cgroup v1 we need to tell CRIU to handle all cgroup mounts as external mounts. */
  ਜੇ (cgroup_ਬਹੁਲਕ != CGROUP_ਬਹੁਲਕ_UNIFIED)
    {
      ret = checkpoint_cgroup_v1_mount (def, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  /* Tell CRIU about external bind mounts. */
  ਲਈ (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      ਜੇ (is_bind_mount (def->mounts[i], ਨਿਰਾ, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          ਅੱਖਰ buf[PATH_MAX];
          ਅਚਲ ਅੱਖਰ *dest_ਵਿੱਚ_ਜੜ੍ਹ;

          ਜੇ (nofollow)
            ਵਾਪਸ crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_ਵਿੱਚ_ਜੜ੍ਹ = chroot_realpath (ਸਥਿਤੀ->rootfs, def->mounts[i]->destination, buf);
          ਜੇ (UNLIKELY (dest_ਵਿੱਚ_ਜੜ੍ਹ == ਨਿਰਾ))
            ਵਾਪਸ crun_make_error (err, errno, "unable to resolve external bind mount `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          ਜੇ (has_ਅਗੇਤਰ (dest_ਵਿੱਚ_ਜੜ੍ਹ, ਸਥਿਤੀ->rootfs))
            dest_ਵਿੱਚ_ਜੜ੍ਹ += ਸੁਰੱਖਿਅਤ_strlen (ਸਥਿਤੀ->rootfs);

          ret = libcriu_wrapper->criu_ਜੋੜੋ_ext_mount (dest_ਵਿੱਚ_ਜੜ੍ਹ, dest_ਵਿੱਚ_ਜੜ੍ਹ);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", def->mounts[i]->destination);
        }
    }

  ret = ਰਜਿਸਟਰ_masked_paths_mounts (def, container, libcriu_wrapper, false, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

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

  ਲਈ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ਪੂਰਨ ਮੁੱਲ = libcrun_ਲੱਭੋ_namespace (def->linux->namespaces[i]->type);
      ਜੇ (UNLIKELY (ਮੁੱਲ < 0))
        ਵਾਪਸ crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      ਜੇ (ਮੁੱਲ == CLONE_NEWNET && def->linux->namespaces[i]->ਰਸਤਾ != ਨਿਰਾ)
        {
          cleanup_free ਅੱਖਰ *external = ਨਿਰਾ;
          ਬਣਤਰ stat statbuf;

          ret = stat (def->linux->namespaces[i]->ਰਸਤਾ, &statbuf);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "unable to stat(): `%s`", def->linux->namespaces[i]->ਰਸਤਾ);

          xasprintf (&external, "net[%ld]:" CRIU_EXT_NETNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_ਜੋੜੋ_external (external);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }

      ਜੇ (ਮੁੱਲ == CLONE_NEWPID && def->linux->namespaces[i]->ਰਸਤਾ != ਨਿਰਾ)
        {
          cleanup_free ਅੱਖਰ *external = ਨਿਰਾ;
          ਬਣਤਰ stat statbuf;

          ret = stat (def->linux->namespaces[i]->ਰਸਤਾ, &statbuf);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, errno, "unable to stat(): `%s`", def->linux->namespaces[i]->ਰਸਤਾ);

          xasprintf (&external, "pid[%ld]:" CRIU_EXT_PIDNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_ਜੋੜੋ_external (external);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }
    }

  /* Tell CRIU to use the freezer to pause all container processes. */
  ਜੇ (cgroup_ਬਹੁਲਕ == CGROUP_ਬਹੁਲਕ_UNIFIED)
    {
      /* This needs CRIU 3.14. */
      ret = append_paths (&freezer_ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ, ਸਥਿਤੀ->cgroup_ਰਸਤਾ, ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }
  ਨਹੀਂਤਾਂ
    {
      ret = append_paths (&freezer_ਰਸਤਾ, err, CGROUP_ਜੜ੍ਹ "/freezer", ਸਥਿਤੀ->cgroup_ਰਸਤਾ, ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ret = libcriu_wrapper->criu_ਸੈੱਟ_freeze_cgroup (freezer_ਰਸਤਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, -ret, "CRIU: failed setting freezer %d", ret);

  /* Set boolean options . */
  libcriu_wrapper->criu_ਸੈੱਟ_leave_running (cr_options->leave_running);
  libcriu_wrapper->criu_ਸੈੱਟ_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_ਸੈੱਟ_shell_ਕੰਮ (cr_options->shell_ਕੰਮ);
  libcriu_wrapper->criu_ਸੈੱਟ_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_ਸੈੱਟ_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_ਸੈੱਟ_orphan_pts_master (true);
  libcriu_wrapper->criu_ਸੈੱਟ_manage_cgroups_ਬਹੁਲਕ (criu_cg_ਬਹੁਲਕ_from_ਜਨਤਕ (cr_options->manage_cgroups_ਬਹੁਲਕ));

  libcriu_wrapper->criu_ਸੈੱਟ_manage_cgroups (true);

  ਜੇ (libcriu_wrapper->criu_ਸੈੱਟ_ਜਾਲ_ਤਾਲਾ)
    {
      ਪੂਰਨ ਤਾਲਾ = 0;

      ret = criu_ਜਾਲ_ਤਾਲਾ_from_ਜਨਤਕ (cr_options->ਜਾਲ_ਤਾਲਾ_ਵਿਧੀ, &ਤਾਲਾ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ਜੇ (ਤਾਲਾ > 0)
        {
          ret = libcriu_wrapper->criu_ਸੈੱਟ_ਜਾਲ_ਤਾਲਾ (ਤਾਲਾ);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, 0, "CRIU: failed setting network lock");
        }
    }

  ret = libcriu_wrapper->criu_dump ();
  ਜੇ (UNLIKELY (ret != 0))
    {
      show_criu_log (cr_options->work_ਰਸਤਾ, CRIU_CHECKPOINT_LOG_FILE);
      ਵਾਪਸ crun_make_error (err, ret < 0 ? -ret : 0, "CRIU checkpointing failed: %d", ret);
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
prepare_restore_mounts_ਅੰਦਰੂਨੀ (runtime_spec_schema_config_schema *def, ਅੱਖਰ *ਜੜ੍ਹ, ਅਚਲ ਅੱਖਰ **mounted,
                                 size_t *n_mounted, libcrun_error_t *err)
{
  uint32_t i;

  /* Go through all mountpoints to be able to recreate missing mountpoints. */
  ਲਈ (i = 0; i < def->mounts_len; i++)
    {
      ਅੱਖਰ *dest = def->mounts[i]->destination;
      ਅੱਖਰ *type = def->mounts[i]->type;
      cleanup_close ਪੂਰਨ ਜੜ੍ਹ_fd = -1;
      bool nofollow = false;
      bool on_tmpfs = false;
      ਪੂਰਨ is_dir = 1;
      size_t j;

      /* cgroup restore should be handled by CRIU itself */
      ਜੇ (type && (strcmp (type, "cgroup") == 0 || strcmp (type, "cgroup2") == 0))
        ਜਾਰੀ;

      /* Check if the mountpoint is on a tmpfs. CRIU restores
       * all tmpfs. We do need to recreate directories on a tmpfs. */
      size_t dest_len = ਲੜੀ_ਲੰਬਾਈ (dest);
      ਲਈ (j = 0; j < def->mounts_len; j++)
        {
          ਜੇ (def->mounts[j]->type == ਨਿਰਾ || strcmp (def->mounts[j]->type, "tmpfs") != 0)
            ਜਾਰੀ;
          size_t mount_len = ਲੜੀ_ਲੰਬਾਈ (def->mounts[j]->destination);
          ਜੇ (mount_len < dest_len && dest[mount_len] == '/' && ਲੜੀ_ਤੁਲਨਾ (dest, def->mounts[j]->destination, mount_len) == 0)
            {
              /* This is a mountpoint which is on a tmpfs.*/
              on_tmpfs = true;
              ਰੋਕੋ;
            }
        }

      ਜੇ (on_tmpfs)
        ਜਾਰੀ;

      /* For bind mounts check if the source is a file or a directory. */
      ਜੇ (is_bind_mount (def->mounts[i], ਨਿਰਾ, &nofollow))
        {
          ਜੇ (nofollow)
            ਵਾਪਸ crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          is_dir = crun_dir_p (def->mounts[i]->ਸਰੋਤ, false, err);
          ਜੇ (UNLIKELY (is_dir < 0))
            ਵਾਪਸ is_dir;
        }

      ਜੜ੍ਹ_fd = open (ਜੜ੍ਹ, O_RDONLY | O_CLOEXEC);
      ਜੇ (UNLIKELY (ਜੜ੍ਹ_fd == -1))
        ਵਾਪਸ crun_make_error (err, errno, "error opening container root directory `%s`", ਜੜ੍ਹ);

      ਜੇ (is_dir)
        {
          ਪੂਰਨ ret;

          ret = crun_ਸੁਰੱਖਿਅਤ_ensure_directory_at (ਜੜ੍ਹ_fd, ਜੜ੍ਹ, dest, 0755, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }
      ਨਹੀਂਤਾਂ
        {
          ਪੂਰਨ ret;

          ret = crun_ਸੁਰੱਖਿਅਤ_ensure_file_at (ਜੜ੍ਹ_fd, ਜੜ੍ਹ, dest, 0755, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ ret;
        }

      /* Mount a bind mount source now, so that the mountpoints of the
       * mounts inside it (e.g. a nested bind mount) are created in the
       * source, where CRIU expects them.  This also happens during the
       * initial container creation, as the mounts are done in order.  */
      ਜੇ (is_bind_mount (def->mounts[i], ਨਿਰਾ, &nofollow))
        {
          cleanup_close ਪੂਰਨ dst_fd = -1;
          proc_fd_ਰਸਤਾ_t fd_ਰਸਤਾ;

          dst_fd = ਸੁਰੱਖਿਅਤ_openat (ਜੜ੍ਹ_fd, ਜੜ੍ਹ, dest, O_ਰਸਤਾ | O_CLOEXEC, 0, err);
          ਜੇ (UNLIKELY (dst_fd < 0))
            ਵਾਪਸ dst_fd;

          /* Not recursive: the source might contain ROOT itself (e.g. when
           * it is the bundle directory), and it is not needed anyway.  */
          get_proc_self_fd_ਰਸਤਾ (fd_ਰਸਤਾ, dst_fd);
          ਜੇ (UNLIKELY (mount (def->mounts[i]->ਸਰੋਤ, fd_ਰਸਤਾ, ਨਿਰਾ, MS_BIND, ਨਿਰਾ) < 0))
            ਵਾਪਸ crun_make_error (err, errno, "bind mount `%s` to `%s`", def->mounts[i]->ਸਰੋਤ, dest);

          mounted[(*n_mounted)++] = dest;
        }
    }

  ਵਾਪਸ 0;
}

/* Move the current process back to CGROUPS, as read from /proc/self/cgroup.
   A failure is not fatal, so only warn about it.  */
ਸਥਿਰ ਖਾਲੀ
move_ਪਿੱਠ_ਮੰਜ਼ਿਲ_cgroups (ਅਚਲ ਅੱਖਰ *cgroups)
{
  libcrun_error_t tmp_err = ਨਿਰਾ;

  ਜੇ (UNLIKELY (libcrun_move_self_ਮੰਜ਼ਿਲ_cgroups (cgroups, &tmp_err) < 0))
    {
      libcrun_warning ("cannot move back to the original cgroup: %s", tmp_err->ਸੁਨੇਹਾ);
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
ਸਥਿਰ ਪੂਰਨ
criu_restore_ਬੱਚਾ_ਵਿੱਚ_cgroup (ਅਚਲ ਅੱਖਰ *cgroup_ਰਸਤਾ, ਪੂਰਨ *criu_ret, libcrun_error_t *err)
{
  sigset_t all_ਸੰਕੇਤ, old_ਨਕਾਬ;
  ਪੂਰਨ wait_ਸਥਿਤੀ = 0;
  pid_t pid;
  ਪੂਰਨ ret;

  *criu_ret = -1;

  sigfillset (&all_ਸੰਕੇਤ);
  ret = sigprocmask (SIG_ਖੰਡ, &all_ਸੰਕੇਤ, &old_ਨਕਾਬ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "sigprocmask");

  /* Must be vfork: the child shares our memory space, so both *criu_ret and
     the error it creates are visible here once it is gone.  */
  pid = vfork ();
  ਜੇ (UNLIKELY (pid < 0))
    {
      ਪੂਰਨ saved_errno = errno;

      sigprocmask (SIG_SETMASK, &old_ਨਕਾਬ, ਨਿਰਾ);
      ਵਾਪਸ crun_make_error (err, saved_errno, "vfork");
    }

  ਜੇ (pid == 0)
    {
      ਬਣਤਰ sigaction act;
      ਪੂਰਨ i;

      /* The signal dispositions are not shared with the parent, so resetting
         them here does not affect it.  Keep the ignored signals ignored, as
         CRIU would inherit them if it was run directly by the caller.  */
      ਲਈ (i = 1; i < NSIG; i++)
        {
          ਜੇ (sigaction (i, ਨਿਰਾ, &act) < 0 || act.sa_handler == SIG_IGN || act.sa_handler == SIG_DFL)
            ਜਾਰੀ;

          memset (&act, 0, ਆਕਾਰ (act));
          act.sa_handler = SIG_DFL;
          sigaction (i, &act, ਨਿਰਾ);
        }
      sigprocmask (SIG_SETMASK, &old_ਨਕਾਬ, ਨਿਰਾ);

      ਜੇ (! is_ਖਾਲੀ_ਥਾਵਾਂ_string (cgroup_ਰਸਤਾ))
        {
          ret = libcrun_move_process_ਮੰਜ਼ਿਲ_cgroup (0, 0, cgroup_ਰਸਤਾ, false, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਸੁਰੱਖਿਅਤ_exit (EXIT_FAILURE);
        }

      *criu_ret = libcriu_wrapper->criu_restore_ਬੱਚਾ ();
      ਸੁਰੱਖਿਅਤ_exit (EXIT_SUCCESS);
    }

  /* The child is gone by now, it is safe to handle signals again.  */
  sigprocmask (SIG_SETMASK, &old_ਨਕਾਬ, ਨਿਰਾ);

  ret = waitpid_ignore_stopped (pid, &wait_ਸਥਿਤੀ, 0);
  ਜੇ (UNLIKELY (ret < 0))
    {
      /* The caller might have reaped the child already, e.g. when it ignores
         SIGCHLD.  Its result is still visible, so use it.  */
      ਜੇ (errno != ECHILD)
        ਵਾਪਸ crun_make_error (err, errno, "waitpid for the CRIU restore process");

      /* The child might have failed before calling CRIU, and left an error.  */
      ਵਾਪਸ *err != ਨਿਰਾ ? -1 : 0;
    }

  ret = get_process_exit_ਸਥਿਤੀ (wait_ਸਥਿਤੀ);
  ਜੇ (UNLIKELY (ret != EXIT_SUCCESS))
    {
      /* The child creates the error in the shared memory space, do not
         overwrite it with crun_make_error().  */
      ਜੇ (*err == ਨਿਰਾ)
        ਵਾਪਸ crun_make_error (err, 0, "the CRIU restore process exited with status %d", ret);
      ਵਾਪਸ -1;
    }

  ਵਾਪਸ 0;
}

/* Recreate the mountpoints which do not exist in the container rootfs
 * mounted at ROOT, the same way it is done on the container creation.  */
ਸਥਿਰ ਪੂਰਨ
prepare_restore_mounts (runtime_spec_schema_config_schema *def, ਅੱਖਰ *ਜੜ੍ਹ, libcrun_error_t *err)
{
  cleanup_free ਅਚਲ ਅੱਖਰ **mounted = xmalloc0 (ਆਕਾਰ (ਅੱਖਰ *) * (def->mounts_len + 1));
  cleanup_close ਪੂਰਨ ਜੜ੍ਹ_fd = -1;
  size_t n_mounted = 0;
  ਪੂਰਨ ret;

  ret = prepare_restore_mounts_ਅੰਦਰੂਨੀ (def, ਜੜ੍ਹ, mounted, &n_mounted, err);

  /* The bind mounts done above are only needed to create the mountpoints,
   * and CRIU restores the mounts itself, so undo them, in reverse order. */
  ਜੇ (n_mounted > 0)
    ਜੜ੍ਹ_fd = open (ਜੜ੍ਹ, O_ਰਸਤਾ | O_CLOEXEC);
  ਜਦੋਂ (ਜੜ੍ਹ_fd >= 0 && n_mounted > 0)
    {
      libcrun_error_t tmp_err = ਨਿਰਾ;
      ਅਚਲ ਅੱਖਰ *dest = mounted[--n_mounted];
      cleanup_close ਪੂਰਨ dst_fd = -1;
      proc_fd_ਰਸਤਾ_t fd_ਰਸਤਾ;

      dst_fd = ਸੁਰੱਖਿਅਤ_openat (ਜੜ੍ਹ_fd, ਜੜ੍ਹ, dest, O_ਰਸਤਾ | O_CLOEXEC, 0, &tmp_err);
      ਜੇ (UNLIKELY (dst_fd < 0))
        {
          crun_error_release (&tmp_err);
          ਜਾਰੀ;
        }
      get_proc_self_fd_ਰਸਤਾ (fd_ਰਸਤਾ, dst_fd);
      ਜੇ (UNLIKELY (umount2 (fd_ਰਸਤਾ, MNT_DETACH) < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "unmount `%s`", dest);
    }

  ਵਾਪਸ ret;
}

ਪੂਰਨ
libcrun_container_restore_linux_criu (libcrun_container_ਸਥਿਤੀ_t *ਸਥਿਤੀ, libcrun_container_t *container,
                                      libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper ਬਣਤਰ libcriu_wrapper_s *wrapper = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ inherit_new_net_fd = -1;
  cleanup_close ਪੂਰਨ inherit_new_pid_fd = -1;
  cleanup_close ਪੂਰਨ image_fd = -1;
  cleanup_free ਅੱਖਰ *ਜੜ੍ਹ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *bundle_cleanup = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *own_cgroups = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ work_fd = -1;
  ਪੂਰਨ cgroup_ਬਹੁਲਕ;
  ਪੂਰਨ criu_ret;
  ਪੂਰਨ ret_ਬਾਹਰਲਾ;
  size_t i;
  ਪੂਰਨ ret;

  ret = ਲੱਦੋ_wrapper (&wrapper, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (geteuid ())
    ਵਾਪਸ crun_make_error (err, 0, "restoring requires root");

  ret = libcriu_wrapper->criu_init_opts ();
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_ਸੰਸਕਰਣ (err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (UNLIKELY (cr_options->image_ਰਸਤਾ == ਨਿਰਾ))
    ਵਾਪਸ crun_make_error (err, 0, "image path not set");

  image_fd = open (cr_options->image_ਰਸਤਾ, O_DIRECTORY | O_CLOEXEC);
  ਜੇ (UNLIKELY (image_fd == -1))
    ਵਾਪਸ crun_make_error (err, errno, "error opening checkpoint directory `%s`", cr_options->image_ਰਸਤਾ);

  libcriu_wrapper->criu_ਸੈੱਟ_images_dir_fd (image_fd);

  /* Load descriptors.json to tell CRIU where those FDs should be connected to. */
  {
    cleanup_free ਅੱਖਰ *descriptors_ਰਸਤਾ = ਨਿਰਾ;
    cleanup_free ਅੱਖਰ *ਬਫਰ = ਨਿਰਾ;
    json_object *doc = ਨਿਰਾ;

    ret = append_paths (&descriptors_ਰਸਤਾ, err, cr_options->image_ਰਸਤਾ, DESCRIPTORS_FILENAME, ਨਿਰਾ);
    ਜੇ (UNLIKELY (ret < 0))
      ਵਾਪਸ ret;

    ret = read_all_file (descriptors_ਰਸਤਾ, &ਬਫਰ, ਨਿਰਾ, err);
    ਜੇ (UNLIKELY (ret < 0))
      ਵਾਪਸ ret;

    /* descriptors.json contains a JSON array with strings
     * telling where 0, 1 and 2 have been initially been
     * pointing to. For each descriptor which points to
     * a pipe 'pipe:' we tell CRIU to reconnect that pipe
     * to the corresponding FD to have (especially) stdout
     * and stderr being correctly redirected. */
    ret = parse_json_file (&doc, ਬਫਰ, ਨਿਰਾ, err);
    ਜੇ (UNLIKELY (ret < 0))
      ਵਾਪਸ ret;

    ਜੇ (json_object_is_type (doc, json_type_array))
      {
        size_t i, len = json_object_array_ਲੰਮਾਈ (doc);

        /* len will probably always be 3 as crun is currently only
         * recording the destination of FD 0, 1 and 2. */
        ਲਈ (i = 0; i < len; ++i)
          {
            json_object *s = json_object_array_get_idx (doc, i);
            ਜੇ (s && json_object_is_type (s, json_type_string))
              {
                ਅਚਲ ਅੱਖਰ *str = json_object_get_string (s);
                ਜੇ (has_ਅਗੇਤਰ (str, "pipe:"))
                  libcriu_wrapper->criu_ਜੋੜੋ_inherit_fd (i, str);
              }
          }
      }
    json_object_put (doc);
  }

  /* Our std streams are the new external descriptors, as CRIU connects the
   * inherited pipes to them.  Record them now: once the container is
   * restored it is already running, and /proc/PID/fd is under its control. */
  ret = libcrun_save_external_descriptors (container, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = ਤਿਆਰੀ_criu_work_dir (cr_options, &work_fd, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (cr_options->lsm_profile != ਨਿਰਾ)
    {
      ret = libcriu_wrapper->criu_ਸੈੱਟ_lsm_profile (cr_options->lsm_profile);
      ਜੇ (UNLIKELY (ret != 0))
        ਵਾਪਸ crun_make_error (err, -ret, "error setting LSM profile to `%s`", cr_options->lsm_profile);
    }

  ਜੇ (cr_options->lsm_mount_context != ਨਿਰਾ)
    {
      ret = libcriu_wrapper->criu_ਸੈੱਟ_lsm_mount_context (cr_options->lsm_mount_context);
      ਜੇ (UNLIKELY (ret != 0))
        ਵਾਪਸ crun_make_error (err, -ret, "error setting LSM mount context to `%s`", cr_options->lsm_mount_context);
    }

  /* do realpath on root */
  bundle_cleanup = realpath (ਸਥਿਤੀ->bundle, ਨਿਰਾ);
  ਜੇ (UNLIKELY (bundle_cleanup == ਨਿਰਾ))
    bundle_cleanup = xstrdup (ਸਥਿਤੀ->bundle);

  /* Tell CRIU about external bind mounts. */
  ਲਈ (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      ਜੇ (is_bind_mount (def->mounts[i], ਨਿਰਾ, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          ਅੱਖਰ buf[PATH_MAX];
          ਅਚਲ ਅੱਖਰ *dest_ਵਿੱਚ_ਜੜ੍ਹ;
          ਅਚਲ ਅੱਖਰ *ਸਰੋਤ = def->mounts[i]->ਸਰੋਤ;
          cleanup_free ਅੱਖਰ *abs_ਸਰੋਤ = ਨਿਰਾ;
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags = 0;
          size_t j;

          ਜੇ (nofollow)
            ਵਾਪਸ crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_ਵਿੱਚ_ਜੜ੍ਹ = chroot_realpath (ਸਥਿਤੀ->rootfs, def->mounts[i]->destination, buf);
          ਜੇ (UNLIKELY (dest_ਵਿੱਚ_ਜੜ੍ਹ == ਨਿਰਾ))
            ਵਾਪਸ crun_make_error (err, errno, "unable to resolve external bind mount destination `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          ਜੇ (has_ਅਗੇਤਰ (dest_ਵਿੱਚ_ਜੜ੍ਹ, ਸਥਿਤੀ->rootfs))
            dest_ਵਿੱਚ_ਜੜ੍ਹ += ਸੁਰੱਖਿਅਤ_strlen (ਸਥਿਤੀ->rootfs);

          /* A relative source is relative to the bundle, while CRIU would
             resolve it relative to its own working directory.  */
          ਜੇ (ਸਰੋਤ && ਸਰੋਤ[0] != '/')
            {
              ret = append_paths (&abs_ਸਰੋਤ, err, bundle_cleanup, ਸਰੋਤ, ਨਿਰਾ);
              ਜੇ (UNLIKELY (ret < 0))
                ਵਾਪਸ ret;
              ਸਰੋਤ = abs_ਸਰੋਤ;
            }

          ret = libcriu_wrapper->criu_ਜੋੜੋ_ext_mount (dest_ਵਿੱਚ_ਜੜ੍ਹ, ਸਰੋਤ);
          ਜੇ (UNLIKELY (ret < 0))
            ਵਾਪਸ crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", ਸਰੋਤ);

          ਲਈ (j = 0; j < def->mounts[i]->options_len; j++)
            {
              ਅਚਲ ਬਣਤਰ ਪ੍ਰਸਾਰ_flags_s *prop = ਨਿਰਾ;

              ਜੇ (def->mounts[i]->options[j])
                prop = libcrun_str2mount_flags (def->mounts[i]->options[j]);
              ਜੇ (prop && ! prop->clear)
                flags |= prop->flags;
            }
          ਜੋੜੋ_restored_ext_mount (dest_ਵਿੱਚ_ਜੜ੍ਹ, flags);
        }
    }

  ret = ਰਜਿਸਟਰ_masked_paths_mounts (def, container, libcriu_wrapper, true, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  /* Mount the container rootfs for CRIU. */
  ret = append_paths (&ਜੜ੍ਹ, err, bundle_cleanup, "criu-root", ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = mkdir (ਜੜ੍ਹ, 0755);
  ਜੇ (UNLIKELY (ret == -1))
    ਵਾਪਸ crun_make_error (err, errno, "error creating restore directory `%s`", ਜੜ੍ਹ);

  ret = mount (ਸਥਿਤੀ->rootfs, ਜੜ੍ਹ, ਨਿਰਾ, MS_BIND | MS_REC, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret == -1))
    {
      ret = crun_make_error (err, errno, "error mounting restore directory `%s`", ਜੜ੍ਹ);
      ਜਾਓ ਬਾਹਰਲਾ;
    }

  /* During initial container creation, crun will create mountpoints
   * defined in config.json if they do not exist. If we are restoring
   * we need to make sure these mountpoints also exist.
   * This is not perfect, as this means crun will modify a rootfs
   * even if it marked as read-only, but runc already modifies
   * the rootfs in the same way. */

  ret = prepare_restore_mounts (def, ਜੜ੍ਹ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ ਬਾਹਰਲਾ_umount;

  ret = libcriu_wrapper->criu_ਸੈੱਟ_ਜੜ੍ਹ (ਜੜ੍ਹ);
  ਜੇ (UNLIKELY (ret != 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU root to `%s`", ਜੜ੍ਹ);
      ਜਾਓ ਬਾਹਰਲਾ_umount;
    }

  /* If a namespace defined in config.json we are telling
   * CRIU use that namespace when restoring the process tree.
   *
   * CRIU expects the information about the namespace like this:
   * --inherit-fd fd[<fd>]:<key>
   * The <key> needs to be the same as during checkpointing (extRootNetNS). */
  ਲਈ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ਅਚਲ ਪੂਰਨ open_flags_for_inherit = O_RDONLY; /* Cannot be O_CLOEXEC as it is passed to the child process. */
      ਪੂਰਨ ਮੁੱਲ = libcrun_ਲੱਭੋ_namespace (def->linux->namespaces[i]->type);
      ਜੇ (UNLIKELY (ਮੁੱਲ < 0))
        {
          ret = crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);
          ਜਾਓ ਬਾਹਰਲਾ_umount;
        }

      ਜੇ (ਮੁੱਲ == CLONE_NEWNET && def->linux->namespaces[i]->ਰਸਤਾ != ਨਿਰਾ)
        {
          inherit_new_net_fd = open (def->linux->namespaces[i]->ਰਸਤਾ, open_flags_for_inherit);
          ਜੇ (UNLIKELY (inherit_new_net_fd < 0))
            {
              ret = crun_make_error (err, errno, "unable to open(): `%s`", def->linux->namespaces[i]->ਰਸਤਾ);
              ਜਾਓ ਬਾਹਰਲਾ_umount;
            }

          ret = libcriu_wrapper->criu_ਜੋੜੋ_inherit_fd (inherit_new_net_fd, CRIU_EXT_NETNS);
          ਜੇ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              ਜਾਓ ਬਾਹਰਲਾ_umount;
            }
        }

      ਜੇ (ਮੁੱਲ == CLONE_NEWPID && def->linux->namespaces[i]->ਰਸਤਾ != ਨਿਰਾ)
        {
          inherit_new_pid_fd = open (def->linux->namespaces[i]->ਰਸਤਾ, open_flags_for_inherit);
          ਜੇ (UNLIKELY (inherit_new_pid_fd < 0))
            {
              ret = crun_make_error (err, errno, "unable to open(): `%s`", def->linux->namespaces[i]->ਰਸਤਾ);
              ਜਾਓ ਬਾਹਰਲਾ_umount;
            }

          ret = libcriu_wrapper->criu_ਜੋੜੋ_inherit_fd (inherit_new_pid_fd, CRIU_EXT_PIDNS);
          ਜੇ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              ਜਾਓ ਬਾਹਰਲਾ_umount;
            }
        }

#  ifdef CRIU_JOIN_NS_SUPPORT
      ਜੇ (ਮੁੱਲ == CLONE_NEWTIME && def->linux->namespaces[i]->ਰਸਤਾ != ਨਿਰਾ)
        {
          ਜੇ (libcriu_wrapper->criu_join_ns_ਜੋੜੋ == ਨਿਰਾ)
            {
              ret = crun_make_error (err, 0, "shared time namespace restore is supported in CRIU >= 3.16.1");
              ਜਾਓ ਬਾਹਰਲਾ_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_ਜੋੜੋ ("time", def->linux->namespaces[i]->ਰਸਤਾ, ਨਿਰਾ);
          ਜੇ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->ਰਸਤਾ);
              ਜਾਓ ਬਾਹਰਲਾ_umount;
            }
        }

      ਜੇ (ਮੁੱਲ == CLONE_NEWIPC && def->linux->namespaces[i]->ਰਸਤਾ != ਨਿਰਾ)
        {
          ਜੇ (libcriu_wrapper->criu_join_ns_ਜੋੜੋ == ਨਿਰਾ)
            {
              ret = crun_make_error (err, 0, "shared ipc namespace restore is supported in CRIU >= 3.16.1");
              ਜਾਓ ਬਾਹਰਲਾ_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_ਜੋੜੋ ("ipc", def->linux->namespaces[i]->ਰਸਤਾ, ਨਿਰਾ);
          ਜੇ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->ਰਸਤਾ);
              ਜਾਓ ਬਾਹਰਲਾ_umount;
            }
        }

      ਜੇ (ਮੁੱਲ == CLONE_NEWUTS && def->linux->namespaces[i]->ਰਸਤਾ != ਨਿਰਾ)
        {
          ਜੇ (libcriu_wrapper->criu_join_ns_ਜੋੜੋ == ਨਿਰਾ)
            {
              ret = crun_make_error (err, 0, "shared uts namespace restore is supported in CRIU >= 3.16.1");
              ਜਾਓ ਬਾਹਰਲਾ_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_ਜੋੜੋ ("uts", def->linux->namespaces[i]->ਰਸਤਾ, ਨਿਰਾ);
          ਜੇ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->ਰਸਤਾ);
              ਜਾਓ ਬਾਹਰਲਾ_umount;
            }
        }
#  endif
    }

  /* Set up CRIU config file */
  ret = handle_criu_config_file (container, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ ਬਾਹਰਲਾ_umount;

  /* Tell CRIU if cgroup v1 needs to be handled. */
  ret = restore_cgroup_v1_mount (def, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਜਾਓ ਬਾਹਰਲਾ_umount;

  console_socket = cr_options->console_socket;
  libcriu_wrapper->criu_ਸੈੱਟ_notify_cb (criu_notify);

  /* Set boolean options . */
  libcriu_wrapper->criu_ਸੈੱਟ_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_ਸੈੱਟ_shell_ਕੰਮ (cr_options->shell_ਕੰਮ);
  libcriu_wrapper->criu_ਸੈੱਟ_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_ਸੈੱਟ_tcp_close (cr_options->tcp_close);
  libcriu_wrapper->criu_ਸੈੱਟ_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_ਸੈੱਟ_orphan_pts_master (true);

  ਜੇ (ਸਥਿਤੀ->cgroup_ਰਸਤਾ)
    {
      cgroup_ਬਹੁਲਕ = libcrun_get_cgroup_ਬਹੁਲਕ (err);
      ਜੇ (UNLIKELY (cgroup_ਬਹੁਲਕ < 0))
        {
          ret = cgroup_ਬਹੁਲਕ;
          ਜਾਓ ਬਾਹਰਲਾ_umount;
        }

      /* With cgroup v2, the cgroup path is only meaningful for the unified
         hierarchy, so only relocate that one.  A NULL controller would make
         CRIU relocate every hierarchy the container is in, named cgroup v1
         ones included.  As CRIU dumps such a hierarchy from wherever the
         container is in it, which is its root, as crun leaves named
         hierarchies alone, every checkpoint and restore would then copy the
         whole hierarchy into itself.  */
      ret = libcriu_wrapper->criu_ਜੋੜੋ_cg_ਜੜ੍ਹ (cgroup_ਬਹੁਲਕ == CGROUP_ਬਹੁਲਕ_UNIFIED ? "" : ਨਿਰਾ,
                                               ਸਥਿਤੀ->cgroup_ਰਸਤਾ);
      ਜੇ (UNLIKELY (ret != 0))
        {
          ret = crun_make_error (err, 0, "error setting CRIU cgroup root to `%s`", ਸਥਿਤੀ->cgroup_ਰਸਤਾ);
          ਜਾਓ ਬਾਹਰਲਾ_umount;
        }
    }

  libcriu_wrapper->criu_ਸੈੱਟ_manage_cgroups_ਬਹੁਲਕ (criu_cg_ਬਹੁਲਕ_from_ਜਨਤਕ (cr_options->manage_cgroups_ਬਹੁਲਕ));
  libcriu_wrapper->criu_ਸੈੱਟ_manage_cgroups (true);

  ਜੇ (libcriu_wrapper->criu_ਸੈੱਟ_ਜਾਲ_ਤਾਲਾ)
    {
      ਪੂਰਨ ਤਾਲਾ = 0;

      ret = criu_ਜਾਲ_ਤਾਲਾ_from_ਜਨਤਕ (cr_options->ਜਾਲ_ਤਾਲਾ_ਵਿਧੀ, &ਤਾਲਾ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਜਾਓ ਬਾਹਰਲਾ_umount;

      ਜੇ (ਤਾਲਾ > 0)
        {
          ret = libcriu_wrapper->criu_ਸੈੱਟ_ਜਾਲ_ਤਾਲਾ (ਤਾਲਾ);
          ਜੇ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "CRIU: failed setting network lock");
              ਜਾਓ ਬਾਹਰਲਾ_umount;
            }
        }
    }

  libcriu_wrapper->criu_ਸੈੱਟ_log_level (4);
  ret = libcriu_wrapper->criu_ਸੈੱਟ_log_file (CRIU_RESTORE_LOG_FILE);
  ਜੇ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU log file to `%s`", CRIU_RESTORE_LOG_FILE);
      ਜਾਓ ਬਾਹਰਲਾ_umount;
    }

  /* criu_restore() returns the PID of the process of the restored process
   * tree. This PID will not be the same as status->pid if the container is
   * running in a PID namespace. But it will always be > 0. */
  ਜੇ (cr_options->detach)
    {
      /* Nothing waits for the container, so there is no reason to take over
         this process: use a throw-away one.  It is used even when there is no
         cgroup to join, so that the container is not left as a child of a
         process which is not going to reap it.  */
      ret = criu_restore_ਬੱਚਾ_ਵਿੱਚ_cgroup (ਸਥਿਤੀ->cgroup_ਰਸਤਾ, &criu_ret, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਜਾਓ ਬਾਹਰਲਾ_umount;

      ret = criu_ret;
    }
  ਨਹੀਂਤਾਂ
    {
      /* Like runc does, put CRIU into the container cgroup for the time of
       * restore, so the restored tasks are created there. It is necessary in
       * the "ignore" mode, where CRIU does not deal with cgroups at all, and
       * does not hurt otherwise. As CRIU is our child, do it by moving
       * ourselves there, and move back once the restore is done. */
      ਜੇ (! is_ਖਾਲੀ_ਥਾਵਾਂ_string (ਸਥਿਤੀ->cgroup_ਰਸਤਾ))
        {
          ret = read_all_file (PROC_SELF_CGROUP, &own_cgroups, ਨਿਰਾ, err);
          ਜੇ (UNLIKELY (ret < 0))
            ਜਾਓ ਬਾਹਰਲਾ_umount;

          ret = libcrun_move_process_ਮੰਜ਼ਿਲ_cgroup (0, 0, ਸਥਿਤੀ->cgroup_ਰਸਤਾ, false, err);
          ਜੇ (UNLIKELY (ret < 0))
            {
              /* Some of the cgroups might have been joined already.  */
              move_ਪਿੱਠ_ਮੰਜ਼ਿਲ_cgroups (own_cgroups);
              ਜਾਓ ਬਾਹਰਲਾ_umount;
            }
        }

      ret = libcriu_wrapper->criu_restore_ਬੱਚਾ ();

      ਜੇ (own_cgroups)
        move_ਪਿੱਠ_ਮੰਜ਼ਿਲ_cgroups (own_cgroups);
    }

  ਜੇ (UNLIKELY (ret <= 0))
    {
      show_criu_log (cr_options->work_ਰਸਤਾ, CRIU_RESTORE_LOG_FILE);
      ret = crun_make_error (err, 0, "CRIU restoring failed: %d", ret);
      ਜਾਓ ਬਾਹਰਲਾ_umount;
    }

  /* Update the status struct with the newly allocated PID. This will
   * be necessary later when moving the process into its cgroup. */
  ਸਥਿਤੀ->pid = ret;
  ret = 0;

ਬਾਹਰਲਾ_umount:
  ret_ਬਾਹਰਲਾ = umount (ਜੜ੍ਹ);
  ਜੇ (UNLIKELY (ret_ਬਾਹਰਲਾ == -1))
    {
      ਪੂਰਨ saved_errno = errno;
      rmdir (ਜੜ੍ਹ);
      ਜੇ (ret < 0)
        ਵਾਪਸ crun_error_wrap (err, "error unmounting restore directory `%s`", ਜੜ੍ਹ);
      ਨਹੀਂਤਾਂ
        ਵਾਪਸ crun_make_error (err, saved_errno, "error unmounting restore directory `%s`", ਜੜ੍ਹ);
    }
ਬਾਹਰਲਾ:
  ret_ਬਾਹਰਲਾ = rmdir (ਜੜ੍ਹ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;
  ਜੇ (UNLIKELY (ret_ਬਾਹਰਲਾ == -1))
    ਵਾਪਸ crun_make_error (err, errno, "error removing restore directory `%s`", ਜੜ੍ਹ);
  ਵਾਪਸ ret;
}
#ਜੇ_ਅੰਤ
