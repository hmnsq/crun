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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଯଦି HAVE_CRIU && HAVE_DLOPEN

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
ଅକ୍ଷର *chroot_realpath (ଅଚଳ ଅକ୍ଷର *chroot, ଅଚଳ ଅକ୍ଷର *path, ଅକ୍ଷର resolved_path[]);

/* Map the public cgroups mode (enum libcrun_cr_cgroups_mode) onto the CRIU enum.  */
ସ୍ଥିର ଗଣନା criu_cg_mode
criu_cg_mode_from_public (ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode)
{
  ବାଛ (mode)
    {
    ସ୍ଥିତି LIBCRUN_CR_CG_MODE_IGNORE:
      ଫେରନ୍ତୁ CRIU_CG_MODE_IGNORE;
    ସ୍ଥିତି LIBCRUN_CR_CG_MODE_FULL:
      ଫେରନ୍ତୁ CRIU_CG_MODE_FULL;
    ସ୍ଥିତି LIBCRUN_CR_CG_MODE_STRICT:
      ଫେରନ୍ତୁ CRIU_CG_MODE_STRICT;
    ସ୍ଥିତି LIBCRUN_CR_CG_MODE_DEFAULT:
    ସ୍ଥିତି LIBCRUN_CR_CG_MODE_SOFT:
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      /* Default to CRIU_CG_MODE_SOFT, just as runc.  */
      ଫେରନ୍ତୁ CRIU_CG_MODE_SOFT;
    }
}

/* Map the public network lock method (enum libcrun_cr_network_lock_method) onto the
   CRIU enum.  *LOCK is set to 0 when no explicit method must be set (CRIU default).
   A method that this build cannot honor is an error, never a silent fallback to
   the CRIU default.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
criu_network_lock_from_public (ପୂର୍ଣ୍ଣସଂଖ୍ୟା method, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *lock, libcrun_error_t *err)
{
  ବାଛ (method)
    {
    ସ୍ଥିତି LIBCRUN_CR_NETWORK_LOCK_IPTABLES:
      *lock = CRIU_NETWORK_LOCK_IPTABLES;
      ଫେରନ୍ତୁ 0;
    ସ୍ଥିତି LIBCRUN_CR_NETWORK_LOCK_NFTABLES:
      *lock = CRIU_NETWORK_LOCK_NFTABLES;
      ଫେରନ୍ତୁ 0;
    ସ୍ଥିତି LIBCRUN_CR_NETWORK_LOCK_SKIP:
#  ଯଦି CRIU_NETWORK_LOCK_SKIP_SUPPORT
      *lock = CRIU_NETWORK_LOCK_SKIP;
      ଫେରନ୍ତୁ 0;
#  ଅନ୍ୟଥା
      ଫେରନ୍ତୁ crun_make_error (err, 0, "CRIU: the `skip` network lock method is not supported by this build");
#  endif
    ସ୍ଥିତି LIBCRUN_CR_NETWORK_LOCK_DEFAULT:
      *lock = 0;
      ଫେରନ୍ତୁ 0;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ crun_make_error (err, 0, "CRIU: unknown network lock method %d", method);
    }
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *console_socket = ଖାଲି;

/* The external mounts CRIU restores, and the flags the configuration asks
   for each of them.  */
ସ୍ଥିର ସଂରଚନା libcrun_mount_flags_s *restored_ext_mounts = ଖାଲି;
ସ୍ଥିର size_t n_restored_ext_mounts = 0;

#  define LIBCRIU_MIN_VERSION 31500

ସଂରଚନା libcriu_wrapper_s
{
  ଶୂନ୍ୟ *handle;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*criu_add_ext_mount) (ଅଚଳ ଅକ୍ଷର *ଚାବି, ଅଚଳ ଅକ୍ଷର *val);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*criu_add_external) (ଅଚଳ ଅକ୍ଷର *ଚାବି);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*criu_add_inherit_fd) (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଅକ୍ଷର *ଚାବି);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*criu_check_version) (ପୂର୍ଣ୍ଣସଂଖ୍ୟା minimum);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*criu_dump) (ଶୂନ୍ୟ);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*criu_get_orphan_pts_master_fd) (ଶୂନ୍ୟ);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*criu_init_opts) (ଶୂନ୍ୟ);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*criu_notify_pid) (criu_notify_arg_t na);
#  ifdef CRIU_JOIN_NS_SUPPORT
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*criu_join_ns_add) (ଅଚଳ ଅକ୍ଷର *ns, ଅଚଳ ଅକ୍ଷର *ns_file, ଅଚଳ ଅକ୍ଷର *extra_opt);
#  endif
#  ifdef CRIU_PRE_DUMP_SUPPORT
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*criu_feature_check) (ସଂରଚନା criu_feature_check *features, size_t size);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*criu_pre_dump) (ଶୂନ୍ୟ);
#  endif
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*criu_restore_child) (ଶୂନ୍ୟ);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*criu_set_freeze_cgroup) (ଅଚଳ ଅକ୍ଷର *name);
  ଶୂନ୍ୟ (*criu_set_file_locks) (bool file_locks);
  ଶୂନ୍ୟ (*criu_set_ext_unix_sk) (bool ext_unix_sk);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*criu_set_log_file) (ଅଚଳ ଅକ୍ଷର *log_file);
  ଶୂନ୍ୟ (*criu_set_log_level) (ପୂର୍ଣ୍ଣସଂଖ୍ୟା log_level);
  ଶୂନ୍ୟ (*criu_set_leave_running) (bool leave_running);
  ଶୂନ୍ୟ (*criu_set_manage_cgroups) (bool manage);
  ଶୂନ୍ୟ (*criu_set_manage_cgroups_mode) (ଗଣନା criu_cg_mode mode);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*criu_set_network_lock) (ଗଣନା criu_network_lock_method method);
  ଶୂନ୍ୟ (*criu_set_notify_cb) (ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*cb) (ଅକ୍ଷର *action, criu_notify_arg_t na));
  ଶୂନ୍ୟ (*criu_set_orphan_pts_master) (bool orphan_pts_master);
  ଶୂନ୍ୟ (*criu_set_images_dir_fd) (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*criu_set_parent_images) (ଅଚଳ ଅକ୍ଷର *path);
  ଶୂନ୍ୟ (*criu_set_pid) (ପୂର୍ଣ୍ଣସଂଖ୍ୟା pid);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*criu_set_ମୂଳ) (ଅଚଳ ଅକ୍ଷର *ମୂଳ);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*criu_add_cg_ମୂଳ) (ଅଚଳ ଅକ୍ଷର *ctrl, ଅଚଳ ଅକ୍ଷର *path);
  ଶୂନ୍ୟ (*criu_set_shell_job) (bool shell_job);
  ଶୂନ୍ୟ (*criu_set_tcp_established) (bool tcp_established);
  ଶୂନ୍ୟ (*criu_set_tcp_close) (bool tcp_close);
  ଶୂନ୍ୟ (*criu_set_track_mem) (bool track_mem);
  ଶୂନ୍ୟ (*criu_set_work_dir_fd) (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*criu_set_lsm_profile) (ଅଚଳ ଅକ୍ଷର *name);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*criu_set_lsm_mount_context) (ଅଚଳ ଅକ୍ଷର *name);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*criu_set_config_file) (ଅଚଳ ଅକ୍ଷର *path);
};

ସ୍ଥିର ସଂରଚନା libcriu_wrapper_s *libcriu_wrapper;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
cleanup_wrapper (ଶୂନ୍ୟ *p)
{
  ସଂରଚନା libcriu_wrapper_s **w;

  w = (ସଂରଚନା libcriu_wrapper_s **) p;
  ଯଦି (*w == ଖାଲି)
    ଫେରନ୍ତୁ;

#  ifndef STATIC
  ଯଦି ((*w)->handle)
    dlclose ((*w)->handle);
#  endif
  ମୁକ୍ତ (*w);
  libcriu_wrapper = ଖାଲି;

  ଯେତେବେଳେ (n_restored_ext_mounts > 0)
    ମୁକ୍ତ (restored_ext_mounts[--n_restored_ext_mounts].target);
  ମୁକ୍ତ (restored_ext_mounts);
  restored_ext_mounts = ଖାଲି;
}

#  define cleanup_wrapper __attribute__ ((cleanup (cleanup_wrapper)))

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
load_wrapper (ସଂରଚନା libcriu_wrapper_s **wrapper_out, libcrun_error_t *err)
{
  cleanup_free ସଂରଚନା libcriu_wrapper_s *wrapper = xmalloc0 (ଆକାର (*wrapper));

#  ifdef STATIC
#    define LOAD_CRIU_FUNCTION(X, ALLOW_NULL) \
      wrapper->X = &X;
#  ଅନ୍ୟଥା
#    define LOAD_CRIU_FUNCTION(X, ALLOW_NULL)                                                    \
      କର                                                                                         \
        {                                                                                        \
          wrapper->X = dlsym (wrapper->handle, #X);                                              \
          ଯଦି (! ALLOW_NULL && wrapper->X == ଖାଲି)                                                \
            {                                                                                    \
              dlclose (wrapper->handle);                                                         \
              ଫେରନ୍ତୁ crun_make_error (err, 0, "could not find symbol `%s` in `libcriu.so`", #X); \
            }                                                                                    \
      } ଯେତେବେଳେ (0)
#  endif

#  ifndef STATIC
  wrapper->handle = dlopen ("libcriu.so.2", RTLD_NOW);
  ଯଦି (wrapper->handle == ଖାଲି)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "could not load `libcriu.so.2`: `%s`", dlerror ());
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
  LOAD_CRIU_FUNCTION (criu_set_ମୂଳ, false);
  LOAD_CRIU_FUNCTION (criu_add_cg_ମୂଳ, false);
  LOAD_CRIU_FUNCTION (criu_set_shell_job, false);
  LOAD_CRIU_FUNCTION (criu_set_tcp_established, false);
  LOAD_CRIU_FUNCTION (criu_set_tcp_close, false);
  LOAD_CRIU_FUNCTION (criu_set_track_mem, false);
  LOAD_CRIU_FUNCTION (criu_set_work_dir_fd, false);
  LOAD_CRIU_FUNCTION (criu_set_lsm_profile, false);
  LOAD_CRIU_FUNCTION (criu_set_lsm_mount_context, false);
#  ଯଦି ! defined STATIC || defined CRIU_CONFIG_FILE
  LOAD_CRIU_FUNCTION (criu_set_config_file, true);
#  endif

  libcriu_wrapper = *wrapper_out = wrapper;
  wrapper = ଖାଲି;
#  undef LOAD_CRIU_FUNCTION
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ
add_restored_ext_mount (ଅଚଳ ଅକ୍ଷର *target, unsigned long flags)
{
  restored_ext_mounts = xrealloc (restored_ext_mounts, ଆକାର (*restored_ext_mounts) * (n_restored_ext_mounts + 1));
  restored_ext_mounts[n_restored_ext_mounts].target = xstrdup (target);
  restored_ext_mounts[n_restored_ext_mounts].flags = flags;
  n_restored_ext_mounts++;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
criu_notify (ଅକ୍ଷର *action, criu_notify_arg_t na)
{
  ଯଦି (strcmp (action, "post-restore") == 0)
    {
      /* CRIU takes the source of an external mount from us and its flags
       * from the image, i.e. from whatever mount was at that path in the
       * container when it was checkpointed.  The container can move its
       * mount points around, so before it is resumed make sure that each
       * source is mounted with the restrictions the configuration asks for. */
      libcrun_error_t tmp_err = ଖାଲି;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

      ret = libcrun_restrict_mounts_in_container (libcriu_wrapper->criu_notify_pid (na), restored_ext_mounts,
                                                  n_restored_ext_mounts, &tmp_err);
      ଯଦି (UNLIKELY (ret < 0))
        {
          libcrun_error (tmp_err->status, "cannot set the flags of the restored mounts: %s", tmp_err->ସନ୍ଦେଶ);
          libcrun_error_release (&tmp_err);
          ଫେରନ୍ତୁ ret;
        }
    }

  ଯଦି (strncmp (action, "orphan-pts-master", 17) == 0)
    {
      /* CRIU sends us the master FD via the 'orphan-pts-master'
       * callback and we are passing it on to the '--console-socket'
       * if it exists. */
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା console_socket_fd = -1;
      libcrun_error_t tmp_err = ଖାଲି;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା master_fd;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

      ଯଦି (! console_socket)
        ଫେରନ୍ତୁ 0;

      master_fd = libcriu_wrapper->criu_get_orphan_pts_master_fd ();

      console_socket_fd = open_unix_domain_client_socket (console_socket, 0, &tmp_err);
      ଯଦି (UNLIKELY (console_socket_fd < 0))
        {
          libcrun_error_release (&tmp_err);
          ଫେରନ୍ତୁ console_socket_fd;
        }
      ret = send_fd_to_socket (console_socket_fd, master_fd, &tmp_err);
      ଯଦି (UNLIKELY (ret < 0))
        {
          libcrun_error_release (&tmp_err);
          ଫେରନ୍ତୁ ret;
        }
    }
  ଫେରନ୍ତୁ 0;
}

#  ifdef CRIU_PRE_DUMP_SUPPORT

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
criu_check_mem_track (libcrun_error_t *err)
{
  ସଂରଚନା criu_feature_check features = { 0 };
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  /* Right now we are only interested in checking memory tracking.
   * Memory tracking can be disabled at different levels. aarch64
   * for example has memory tracking not implemented. It could also
   * be not enabled on other architectures. Just ask CRIU if that
   * features exists. */

  features.mem_track = true;

  ret = libcriu_wrapper->criu_feature_check (&features, ଆକାର (features));
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "CRIU feature checking failed: %d", ret);

  ଯଦି (features.mem_track == true)
    ଫେରନ୍ତୁ 1;

  ଫେରନ୍ତୁ crun_make_error (err, 0, "CRIU memory tracking not supported");
}

#  endif

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
register_masked_paths_mounts (runtime_spec_schema_config_schema *def, libcrun_container_t *container,
                              ସଂରଚନା libcriu_wrapper_s *libcriu_wrapper, bool is_restore, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *empty_dir_path = ଖାଲି;
  bool shared_dir_registered = false;
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ପାଇଁ (i = 0; i < def->linux->masked_paths_len; i++)
    {
      ସଂରଚନା stat statbuf;
      ret = stat (def->linux->masked_paths[i], &statbuf);
      ଯଦି (ret != 0)
        ଜାରି;

      ଯଦି (S_ISDIR (statbuf.st_mode))
        {
          ଯଦି (! shared_dir_registered)
            {
              ret = get_shared_empty_directory_path (&empty_dir_path,
                                                     (container->context ? container->context->state_ମୂଳ : ଖାଲି), err);
              ଯଦି (UNLIKELY (ret < 0))
                ଫେରନ୍ତୁ ret;

              ret = libcriu_wrapper->criu_add_ext_mount (empty_dir_path, empty_dir_path);
              ଯଦି (UNLIKELY (ret < 0))
                ଫେରନ୍ତୁ crun_make_error (err, -ret, "CRIU: failed adding external mount for shared empty directory `%s`", empty_dir_path);

              shared_dir_registered = true;
            }

          ret = libcriu_wrapper->criu_add_ext_mount (def->linux->masked_paths[i], empty_dir_path);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, -ret, "CRIU: failed adding external mount for masked directory `%s`", def->linux->masked_paths[i]);

          ଯଦି (is_restore)
            add_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
      ଅନ୍ୟଥା ଯଦି (S_ISREG (statbuf.st_mode))
        {
          ଅଚଳ ଅକ୍ଷର *bind_target = is_restore ? "/dev/null" : def->linux->masked_paths[i];
          ret = libcriu_wrapper->criu_add_ext_mount (def->linux->masked_paths[i], bind_target);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", bind_target);

          ଯଦି (is_restore)
            add_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
    }

  ଫେରନ୍ତୁ 0;
}

/* Parse one /proc/self/cgroup LINE into its subsystem, normalizing the name and
   stripping any "name=" prefix.  When SUBPATH_OUT is not NULL, the in-hierarchy
   path is stored there.  Returns the subsystem, or NULL if the line carries no
   controller and should be skipped.  LINE is modified in place.  */
ସ୍ଥିର ଅକ୍ଷର *
parse_cgroup_subsystem (ଅକ୍ଷର *ଧାଡ଼ି, ଅକ୍ଷର **subpath_out)
{
  ଅକ୍ଷର *subsystem, *subpath, *it;

  subsystem = strchr (ଧାଡ଼ି, ':') + 1;
  subpath = strchr (subsystem, ':') + 1;
  *(subpath - 1) = '\0';

  ଯଦି (subsystem[0] == '\0')
    ଫେରନ୍ତୁ ଖାଲି;

  it = strstr (subsystem, "name=");
  ଯଦି (it)
    subsystem = it + 5;

  ଯଦି (strcmp (subsystem, "net_prio,net_cls") == 0)
    subsystem = "net_cls,net_prio";
  ଯଦି (strcmp (subsystem, "cpuacct,cpu") == 0)
    subsystem = "cpu,cpuacct";

  ଯଦି (subpath_out)
    *subpath_out = subpath;
  ଫେରନ୍ତୁ subsystem;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
restore_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *content = ଖାଲି;
  bool has_cgroup_mount = false;
  ଅକ୍ଷର *saveptr = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode;
  ଅକ୍ଷର *from;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  uint32_t i;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ଯଦି (UNLIKELY (cgroup_mode < 0))
    ଫେରନ୍ତୁ cgroup_mode;

  ଯଦି (cgroup_mode == CGROUP_MODE_UNIFIED)
    ଫେରନ୍ତୁ 0;

  /* First check if there is actually a cgroup mount in the container. */
  ପାଇଁ (i = 0; i < def->mounts_len; i++)
    {
      ଅକ୍ଷର *type = def->mounts[i]->type;
      ଯଦି (type && strcmp (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          ଭାଙ୍ଗ;
        }
    }

  ଯଦି (! has_cgroup_mount)
    ଫେରନ୍ତୁ 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, ଖାଲି, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (UNLIKELY (content == ଖାଲି || content[0] == '\0'))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  ପାଇଁ (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ଖାଲି, "\n", &saveptr))
    {
      cleanup_free ଅକ୍ଷର *destination = ଖାଲି;
      cleanup_free ଅକ୍ଷର *source = ଖାଲି;
      ଅକ୍ଷର *subsystem;
      ଅକ୍ଷର *subpath;

      subsystem = parse_cgroup_subsystem (from, &subpath);
      ଯଦି (subsystem == ଖାଲି)
        ଜାରି;

      ret = append_paths (&source, err, CGROUP_ମୂଳ, subsystem, ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = append_paths (&destination, err, source, subpath, ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = libcriu_wrapper->criu_add_ext_mount (source, destination);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", destination);
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
checkpoint_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *content = ଖାଲି;
  bool has_cgroup_mount = false;
  ଅକ୍ଷର *saveptr = ଖାଲି;
  ଅକ୍ଷର *from;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  uint32_t i;

  /* First check if there is actually a cgroup mount in the container. */
  ପାଇଁ (i = 0; i < def->mounts_len; i++)
    {
      ଅକ୍ଷର *type = def->mounts[i]->type;
      ଯଦି (type && strcmp (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          ଭାଙ୍ଗ;
        }
    }

  ଯଦି (! has_cgroup_mount)
    ଫେରନ୍ତୁ 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, ଖାଲି, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (UNLIKELY (content == ଖାଲି || content[0] == '\0'))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  ପାଇଁ (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ଖାଲି, "\n", &saveptr))
    {
      cleanup_free ଅକ୍ଷର *source_path = ଖାଲି;
      ଅକ୍ଷର *subsystem;

      subsystem = parse_cgroup_subsystem (from, ଖାଲି);
      ଯଦି (subsystem == ଖାଲି)
        ଜାରି;

      ret = append_paths (&source_path, err, CGROUP_ମୂଳ, subsystem, ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = libcriu_wrapper->criu_add_ext_mount (source_path, source_path);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", source_path);
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
handle_criu_config_file (libcrun_container_t *container, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ଅଚଳ ଅକ୍ଷର *criu_config_annotation;
  ଅଚଳ ଅକ୍ଷର *config_file = CRIU_RUNC_CONFIG_FILE;

  criu_config_annotation = find_annotation (container, "org.criu.config");

  /* Ignore missing criu_set_config_file() API for compatibility with older CRIU versions,
   * and show an error only if config file is explicitly set with annotation.
   */
  ଯଦି (libcriu_wrapper->criu_set_config_file == ଖାଲି)
    {
      ଯଦି (criu_config_annotation)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "libcriu RPC config files supported in CRIU >= 4.2");

      ଫେରନ୍ତୁ 0;
    }

  ଯଦି (criu_config_annotation)
    config_file = criu_config_annotation;
  ଅନ୍ୟଥା ଯଦି (access (CRIU_CRUN_CONFIG_FILE, F_OK) == 0)
    config_file = CRIU_CRUN_CONFIG_FILE;

  ret = libcriu_wrapper->criu_set_config_file (config_file);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "failed to set CRIU config file");

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
validate_criu_version (libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  // validate that the libcriu version is at least LIBCRIU_MIN_VERSION
  ret = libcriu_wrapper->criu_check_version (LIBCRIU_MIN_VERSION);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, -ret, "CRIU: failed checking version");

  ଯଦି (ret == 0)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "libcriu is too old");

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ
show_criu_log (ଅଚଳ ଅକ୍ଷର *work_path, ଅଚଳ ଅକ୍ଷର *log)
{
  cleanup_free ଅକ୍ଷର *log_path = ଖାଲି;
  libcrun_error_t tmp_err = ଖାଲି;
  ଅକ୍ଷର ଧାଡ଼ି[1024];
  FILE *f;

  ଯଦି (UNLIKELY (append_paths (&log_path, &tmp_err, work_path, log, ଖାଲି) < 0))
    {
      crun_error_release (&tmp_err);
      ଫେରନ୍ତୁ;
    }

  f = fopen (log_path, "r");
  ଯଦି (f == ଖାଲି)
    {
      ଯଦି (errno != ENOENT)
        libcrun_error (errno, "Can't open CRIU log `%s`", log_path);
      ଫେରନ୍ତୁ;
    }

  /* Log with error verbosity as this is the default. */
  libcrun_error (0, "--- excerpt from CRIU log `%s`", log_path);
  ଯେତେବେଳେ (fgets (ଧାଡ଼ି, ଆକାର (ଧାଡ଼ି), f) != ଖାଲି)
    ଯଦି (strstr (ଧାଡ଼ି, "Error ") != ଖାଲି)
      {
        ଧାଡ଼ି[strcspn (ଧାଡ଼ି, "\n")] = '\0';
        libcrun_error (0, "%s", ଧାଡ଼ି);
      }

  fclose (f);
  libcrun_error (0, "--- end of excerpt");
}

/* work_dir is the place CRIU will put its logfiles.  If not explicitly set, CRIU
 * will put the logfiles into the images_dir.  When set, create and open it and
 * hand the fd to CRIU; ownership of the returned fd stays with the caller.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
setup_criu_work_dir (libcrun_checkpoint_restore_t *cr_options, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *work_fd_out, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା work_fd;

  ଯଦି (cr_options->work_path == ଖାଲି)
    {
      /* This is only for the error message later. */
      cr_options->work_path = cr_options->image_path;
      ଫେରନ୍ତୁ 0;
    }

  ଯଦି (UNLIKELY ((mkdir (cr_options->work_path, 0700) == -1) && (errno != EEXIST)))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "error creating CRIU work directory `%s`", cr_options->work_path);

  work_fd = open (cr_options->work_path, O_DIRECTORY | O_CLOEXEC);
  ଯଦି (UNLIKELY (work_fd == -1))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "error opening CRIU work directory `%s`", cr_options->work_path);

  libcriu_wrapper->criu_set_work_dir_fd (work_fd);
  *work_fd_out = work_fd;
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_checkpoint_linux_criu (libcrun_container_status_t *status, libcrun_container_t *container,
                                         libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper ସଂରଚନା libcriu_wrapper_s *wrapper = ଖାଲି;
  cleanup_free ଅକ୍ଷର *descriptors_path = ଖାଲି;
  cleanup_free ଅକ୍ଷର *freezer_path = ଖାଲି;
  cleanup_free ଅକ୍ଷର *path = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା image_fd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା work_fd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode;
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = load_wrapper (&wrapper, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (geteuid ())
    ଫେରନ୍ତୁ crun_make_error (err, 0, "checkpointing requires root");

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
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_version (err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (UNLIKELY (cr_options->image_path == ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "image path not set");

  ret = mkdir (cr_options->image_path, 0700);
  ଯଦି (UNLIKELY ((ret == -1) && (errno != EEXIST)))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "error creating checkpoint directory `%s`", cr_options->image_path);

  image_fd = open (cr_options->image_path, O_DIRECTORY | O_CLOEXEC);
  ଯଦି (UNLIKELY (image_fd == -1))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "error opening checkpoint directory `%s`", cr_options->image_path);

  libcriu_wrapper->criu_set_images_dir_fd (image_fd);

  /* Set up logging. */
  libcriu_wrapper->criu_set_log_level (4);
  libcriu_wrapper->criu_set_log_file (CRIU_CHECKPOINT_LOG_FILE);

  /* Set up CRIU config file */
  ଯଦି (UNLIKELY (handle_criu_config_file (container, err)))
    ଫେରନ୍ତୁ -1;

  /* Setting the pid early as we can skip a lot of checkpoint setup if
   * we just do a pre-dump. The PID needs to be set always. Do it here.
   * The main process of the container is the process CRIU will checkpoint
   * and all of its children. */
  libcriu_wrapper->criu_set_pid (status->pid);

  ret = setup_criu_work_dir (cr_options, &work_fd, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

#  ifdef CRIU_PRE_DUMP_SUPPORT

  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା criu_can_mem_track = 0;
    /* If the user uses --pre-dump for the second time or does
     * a final dump from a previous pre-dump, setting parent_path
     * is necessary so that CRIU can find which pages have not
     * changed compared to the previous dump. */
    ଯଦି (cr_options->parent_path != ଖାଲି)
      {
        criu_can_mem_track = criu_check_mem_track (err);
        ଯଦି (UNLIKELY (criu_can_mem_track == -1))
          ଫେରନ୍ତୁ -1;
        libcriu_wrapper->criu_set_track_mem (true);

        /* The parent path must be relative to image path (something like ../previous-dump).
           CRIU will fail with an unclear error message if the path is not right.
         */
        ଯଦି (UNLIKELY (cr_options->parent_path[0] == '/'))
          ଫେରନ୍ତୁ crun_make_error (err, 0, "--parent-path must be relative");
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_dir = crun_dir_p_at (image_fd, cr_options->parent_path, false, err);
        ଯଦି (UNLIKELY (is_dir <= 0))
          {
            ଯଦି (is_dir < 0)
              ଫେରନ୍ତୁ crun_error_wrap (err, "invalid --parent-path");
            ଫେରନ୍ତୁ crun_make_error (err, ENOTDIR, "invalid --parent-path");
          }
        ret = libcriu_wrapper->criu_set_parent_images (cr_options->parent_path);
        ଯଦି (UNLIKELY (ret != 0))
          ଫେରନ୍ତୁ crun_make_error (err, -ret, "error setting CRIU parent images path to `%s`", cr_options->parent_path);
      }

    ଯଦି (cr_options->pre_dump)
      {
        ଯଦି (criu_can_mem_track != 1)
          {
            criu_can_mem_track = criu_check_mem_track (err);
            ଯଦି (UNLIKELY (criu_can_mem_track == -1))
              ଫେରନ୍ତୁ -1;
          }
        libcriu_wrapper->criu_set_track_mem (true);
        ret = libcriu_wrapper->criu_pre_dump ();
        ଯଦି (UNLIKELY (ret != 0))
          {
            show_criu_log (cr_options->work_path, CRIU_CHECKPOINT_LOG_FILE);
            ଫେରନ୍ତୁ crun_make_error (err, 0, "CRIU pre-dump failed: %d", ret);
          }
        ଫେରନ୍ତୁ 0;
      }
  }
#  endif

  /* descriptors.json is needed during restore to correctly
   * reconnect stdin, stdout, stderr. */
  ret = append_paths (&descriptors_path, err, cr_options->image_path, DESCRIPTORS_FILENAME, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = write_file (descriptors_path, status->external_descriptors, safe_strlen (status->external_descriptors), err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_error_wrap (err, "error saving CRIU descriptors file");

  ret = append_paths (&path, err, status->bundle, status->rootfs, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcriu_wrapper->criu_set_ମୂଳ (path);
  ଯଦି (UNLIKELY (ret != 0))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "error setting CRIU root to `%s`", path);

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ଯଦି (UNLIKELY (cgroup_mode < 0))
    ଫେରନ୍ତୁ cgroup_mode;

  /* For cgroup v1 we need to tell CRIU to handle all cgroup mounts as external mounts. */
  ଯଦି (cgroup_mode != CGROUP_MODE_UNIFIED)
    {
      ret = checkpoint_cgroup_v1_mount (def, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  /* Tell CRIU about external bind mounts. */
  ପାଇଁ (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      ଯଦି (is_bind_mount (def->mounts[i], ଖାଲି, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          ଅକ୍ଷର buf[PATH_MAX];
          ଅଚଳ ଅକ୍ଷର *dest_in_ମୂଳ;

          ଯଦି (nofollow)
            ଫେରନ୍ତୁ crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_in_ମୂଳ = chroot_realpath (status->rootfs, def->mounts[i]->destination, buf);
          ଯଦି (UNLIKELY (dest_in_ମୂଳ == ଖାଲି))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "unable to resolve external bind mount `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          ଯଦି (has_prefix (dest_in_ମୂଳ, status->rootfs))
            dest_in_ମୂଳ += safe_strlen (status->rootfs);

          ret = libcriu_wrapper->criu_add_ext_mount (dest_in_ମୂଳ, dest_in_ମୂଳ);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", def->mounts[i]->destination);
        }
    }

  ret = register_masked_paths_mounts (def, container, libcriu_wrapper, false, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

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

  ପାଇଁ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ = libcrun_find_namespace (def->linux->namespaces[i]->type);
      ଯଦି (UNLIKELY (ମୂଲ୍ୟ < 0))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      ଯଦି (ମୂଲ୍ୟ == CLONE_NEWNET && def->linux->namespaces[i]->path != ଖାଲି)
        {
          cleanup_free ଅକ୍ଷର *external = ଖାଲି;
          ସଂରଚନା stat statbuf;

          ret = stat (def->linux->namespaces[i]->path, &statbuf);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "unable to stat(): `%s`", def->linux->namespaces[i]->path);

          xasprintf (&external, "net[%ld]:" CRIU_EXT_NETNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_add_external (external);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }

      ଯଦି (ମୂଲ୍ୟ == CLONE_NEWPID && def->linux->namespaces[i]->path != ଖାଲି)
        {
          cleanup_free ଅକ୍ଷର *external = ଖାଲି;
          ସଂରଚନା stat statbuf;

          ret = stat (def->linux->namespaces[i]->path, &statbuf);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "unable to stat(): `%s`", def->linux->namespaces[i]->path);

          xasprintf (&external, "pid[%ld]:" CRIU_EXT_PIDNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_add_external (external);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }
    }

  /* Tell CRIU to use the freezer to pause all container processes. */
  ଯଦି (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      /* This needs CRIU 3.14. */
      ret = append_paths (&freezer_path, err, CGROUP_ମୂଳ, status->cgroup_path, ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଅନ୍ୟଥା
    {
      ret = append_paths (&freezer_path, err, CGROUP_ମୂଳ "/freezer", status->cgroup_path, ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ret = libcriu_wrapper->criu_set_freeze_cgroup (freezer_path);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, -ret, "CRIU: failed setting freezer %d", ret);

  /* Set boolean options . */
  libcriu_wrapper->criu_set_leave_running (cr_options->leave_running);
  libcriu_wrapper->criu_set_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_set_shell_job (cr_options->shell_job);
  libcriu_wrapper->criu_set_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_set_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_set_orphan_pts_master (true);
  libcriu_wrapper->criu_set_manage_cgroups_mode (criu_cg_mode_from_public (cr_options->manage_cgroups_mode));

  libcriu_wrapper->criu_set_manage_cgroups (true);

  ଯଦି (libcriu_wrapper->criu_set_network_lock)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା lock = 0;

      ret = criu_network_lock_from_public (cr_options->network_lock_method, &lock, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ଯଦି (lock > 0)
        {
          ret = libcriu_wrapper->criu_set_network_lock (lock);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, 0, "CRIU: failed setting network lock");
        }
    }

  ret = libcriu_wrapper->criu_dump ();
  ଯଦି (UNLIKELY (ret != 0))
    {
      show_criu_log (cr_options->work_path, CRIU_CHECKPOINT_LOG_FILE);
      ଫେରନ୍ତୁ crun_make_error (err, ret < 0 ? -ret : 0, "CRIU checkpointing failed: %d", ret);
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
prepare_restore_mounts_internal (runtime_spec_schema_config_schema *def, ଅକ୍ଷର *ମୂଳ, ଅଚଳ ଅକ୍ଷର **mounted,
                                 size_t *n_mounted, libcrun_error_t *err)
{
  uint32_t i;

  /* Go through all mountpoints to be able to recreate missing mountpoints. */
  ପାଇଁ (i = 0; i < def->mounts_len; i++)
    {
      ଅକ୍ଷର *dest = def->mounts[i]->destination;
      ଅକ୍ଷର *type = def->mounts[i]->type;
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଳ_fd = -1;
      bool nofollow = false;
      bool on_tmpfs = false;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_dir = 1;
      size_t j;

      /* cgroup restore should be handled by CRIU itself */
      ଯଦି (type && (strcmp (type, "cgroup") == 0 || strcmp (type, "cgroup2") == 0))
        ଜାରି;

      /* Check if the mountpoint is on a tmpfs. CRIU restores
       * all tmpfs. We do need to recreate directories on a tmpfs. */
      size_t dest_len = strlen (dest);
      ପାଇଁ (j = 0; j < def->mounts_len; j++)
        {
          ଯଦି (def->mounts[j]->type == ଖାଲି || strcmp (def->mounts[j]->type, "tmpfs") != 0)
            ଜାରି;
          size_t mount_len = strlen (def->mounts[j]->destination);
          ଯଦି (mount_len < dest_len && dest[mount_len] == '/' && strncmp (dest, def->mounts[j]->destination, mount_len) == 0)
            {
              /* This is a mountpoint which is on a tmpfs.*/
              on_tmpfs = true;
              ଭାଙ୍ଗ;
            }
        }

      ଯଦି (on_tmpfs)
        ଜାରି;

      /* For bind mounts check if the source is a file or a directory. */
      ଯଦି (is_bind_mount (def->mounts[i], ଖାଲି, &nofollow))
        {
          ଯଦି (nofollow)
            ଫେରନ୍ତୁ crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          is_dir = crun_dir_p (def->mounts[i]->source, false, err);
          ଯଦି (UNLIKELY (is_dir < 0))
            ଫେରନ୍ତୁ is_dir;
        }

      ମୂଳ_fd = open (ମୂଳ, O_RDONLY | O_CLOEXEC);
      ଯଦି (UNLIKELY (ମୂଳ_fd == -1))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "error opening container root directory `%s`", ମୂଳ);

      ଯଦି (is_dir)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

          ret = crun_safe_ensure_directory_at (ମୂଳ_fd, ମୂଳ, dest, 0755, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }
      ଅନ୍ୟଥା
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

          ret = crun_safe_ensure_file_at (ମୂଳ_fd, ମୂଳ, dest, 0755, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }

      /* Mount a bind mount source now, so that the mountpoints of the
       * mounts inside it (e.g. a nested bind mount) are created in the
       * source, where CRIU expects them.  This also happens during the
       * initial container creation, as the mounts are done in order.  */
      ଯଦି (is_bind_mount (def->mounts[i], ଖାଲି, &nofollow))
        {
          cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dst_fd = -1;
          proc_fd_path_t fd_path;

          dst_fd = safe_openat (ମୂଳ_fd, ମୂଳ, dest, O_PATH | O_CLOEXEC, 0, err);
          ଯଦି (UNLIKELY (dst_fd < 0))
            ଫେରନ୍ତୁ dst_fd;

          /* Not recursive: the source might contain ROOT itself (e.g. when
           * it is the bundle directory), and it is not needed anyway.  */
          get_proc_self_fd_path (fd_path, dst_fd);
          ଯଦି (UNLIKELY (mount (def->mounts[i]->source, fd_path, ଖାଲି, MS_BIND, ଖାଲି) < 0))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "bind mount `%s` to `%s`", def->mounts[i]->source, dest);

          mounted[(*n_mounted)++] = dest;
        }
    }

  ଫେରନ୍ତୁ 0;
}

/* Move the current process back to CGROUPS, as read from /proc/self/cgroup.
   A failure is not fatal, so only warn about it.  */
ସ୍ଥିର ଶୂନ୍ୟ
move_back_to_cgroups (ଅଚଳ ଅକ୍ଷର *cgroups)
{
  libcrun_error_t tmp_err = ଖାଲି;

  ଯଦି (UNLIKELY (libcrun_move_self_to_cgroups (cgroups, &tmp_err) < 0))
    {
      libcrun_warning ("cannot move back to the original cgroup: %s", tmp_err->ସନ୍ଦେଶ);
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
criu_restore_child_in_cgroup (ଅଚଳ ଅକ୍ଷର *cgroup_path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *criu_ret, libcrun_error_t *err)
{
  sigset_t all_signals, old_mask;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା wait_status = 0;
  pid_t pid;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  *criu_ret = -1;

  sigfillset (&all_signals);
  ret = sigprocmask (SIG_BLOCK, &all_signals, &old_mask);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "sigprocmask");

  /* Must be vfork: the child shares our memory space, so both *criu_ret and
     the error it creates are visible here once it is gone.  */
  pid = vfork ();
  ଯଦି (UNLIKELY (pid < 0))
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;

      sigprocmask (SIG_SETMASK, &old_mask, ଖାଲି);
      ଫେରନ୍ତୁ crun_make_error (err, saved_errno, "vfork");
    }

  ଯଦି (pid == 0)
    {
      ସଂରଚନା sigaction act;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

      /* The signal dispositions are not shared with the parent, so resetting
         them here does not affect it.  Keep the ignored signals ignored, as
         CRIU would inherit them if it was run directly by the caller.  */
      ପାଇଁ (i = 1; i < NSIG; i++)
        {
          ଯଦି (sigaction (i, ଖାଲି, &act) < 0 || act.sa_handler == SIG_IGN || act.sa_handler == SIG_DFL)
            ଜାରି;

          memset (&act, 0, ଆକାର (act));
          act.sa_handler = SIG_DFL;
          sigaction (i, &act, ଖାଲି);
        }
      sigprocmask (SIG_SETMASK, &old_mask, ଖାଲି);

      ଯଦି (! is_empty_string (cgroup_path))
        {
          ret = libcrun_move_process_to_cgroup (0, 0, cgroup_path, false, err);
          ଯଦି (UNLIKELY (ret < 0))
            _safe_exit (EXIT_FAILURE);
        }

      *criu_ret = libcriu_wrapper->criu_restore_child ();
      _safe_exit (EXIT_SUCCESS);
    }

  /* The child is gone by now, it is safe to handle signals again.  */
  sigprocmask (SIG_SETMASK, &old_mask, ଖାଲି);

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  ଯଦି (UNLIKELY (ret < 0))
    {
      /* The caller might have reaped the child already, e.g. when it ignores
         SIGCHLD.  Its result is still visible, so use it.  */
      ଯଦି (errno != ECHILD)
        ଫେରନ୍ତୁ crun_make_error (err, errno, "waitpid for the CRIU restore process");

      /* The child might have failed before calling CRIU, and left an error.  */
      ଫେରନ୍ତୁ *err != ଖାଲି ? -1 : 0;
    }

  ret = get_process_exit_status (wait_status);
  ଯଦି (UNLIKELY (ret != EXIT_SUCCESS))
    {
      /* The child creates the error in the shared memory space, do not
         overwrite it with crun_make_error().  */
      ଯଦି (*err == ଖାଲି)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "the CRIU restore process exited with status %d", ret);
      ଫେରନ୍ତୁ -1;
    }

  ଫେରନ୍ତୁ 0;
}

/* Recreate the mountpoints which do not exist in the container rootfs
 * mounted at ROOT, the same way it is done on the container creation.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
prepare_restore_mounts (runtime_spec_schema_config_schema *def, ଅକ୍ଷର *ମୂଳ, libcrun_error_t *err)
{
  cleanup_free ଅଚଳ ଅକ୍ଷର **mounted = xmalloc0 (ଆକାର (ଅକ୍ଷର *) * (def->mounts_len + 1));
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଳ_fd = -1;
  size_t n_mounted = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = prepare_restore_mounts_internal (def, ମୂଳ, mounted, &n_mounted, err);

  /* The bind mounts done above are only needed to create the mountpoints,
   * and CRIU restores the mounts itself, so undo them, in reverse order. */
  ଯଦି (n_mounted > 0)
    ମୂଳ_fd = open (ମୂଳ, O_PATH | O_CLOEXEC);
  ଯେତେବେଳେ (ମୂଳ_fd >= 0 && n_mounted > 0)
    {
      libcrun_error_t tmp_err = ଖାଲି;
      ଅଚଳ ଅକ୍ଷର *dest = mounted[--n_mounted];
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dst_fd = -1;
      proc_fd_path_t fd_path;

      dst_fd = safe_openat (ମୂଳ_fd, ମୂଳ, dest, O_PATH | O_CLOEXEC, 0, &tmp_err);
      ଯଦି (UNLIKELY (dst_fd < 0))
        {
          crun_error_release (&tmp_err);
          ଜାରି;
        }
      get_proc_self_fd_path (fd_path, dst_fd);
      ଯଦି (UNLIKELY (umount2 (fd_path, MNT_DETACH) < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "unmount `%s`", dest);
    }

  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_container_restore_linux_criu (libcrun_container_status_t *status, libcrun_container_t *container,
                                      libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper ସଂରଚନା libcriu_wrapper_s *wrapper = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା inherit_new_net_fd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା inherit_new_pid_fd = -1;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା image_fd = -1;
  cleanup_free ଅକ୍ଷର *ମୂଳ = ଖାଲି;
  cleanup_free ଅକ୍ଷର *bundle_cleanup = ଖାଲି;
  cleanup_free ଅକ୍ଷର *own_cgroups = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା work_fd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା criu_ret;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret_out;
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = load_wrapper (&wrapper, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (geteuid ())
    ଫେରନ୍ତୁ crun_make_error (err, 0, "restoring requires root");

  ret = libcriu_wrapper->criu_init_opts ();
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_version (err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (UNLIKELY (cr_options->image_path == ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "image path not set");

  image_fd = open (cr_options->image_path, O_DIRECTORY | O_CLOEXEC);
  ଯଦି (UNLIKELY (image_fd == -1))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "error opening checkpoint directory `%s`", cr_options->image_path);

  libcriu_wrapper->criu_set_images_dir_fd (image_fd);

  /* Load descriptors.json to tell CRIU where those FDs should be connected to. */
  {
    cleanup_free ଅକ୍ଷର *descriptors_path = ଖାଲି;
    cleanup_free ଅକ୍ଷର *buffer = ଖାଲି;
    json_object *doc = ଖାଲି;

    ret = append_paths (&descriptors_path, err, cr_options->image_path, DESCRIPTORS_FILENAME, ଖାଲି);
    ଯଦି (UNLIKELY (ret < 0))
      ଫେରନ୍ତୁ ret;

    ret = read_all_file (descriptors_path, &buffer, ଖାଲି, err);
    ଯଦି (UNLIKELY (ret < 0))
      ଫେରନ୍ତୁ ret;

    /* descriptors.json contains a JSON array with strings
     * telling where 0, 1 and 2 have been initially been
     * pointing to. For each descriptor which points to
     * a pipe 'pipe:' we tell CRIU to reconnect that pipe
     * to the corresponding FD to have (especially) stdout
     * and stderr being correctly redirected. */
    ret = parse_json_file (&doc, buffer, ଖାଲି, err);
    ଯଦି (UNLIKELY (ret < 0))
      ଫେରନ୍ତୁ ret;

    ଯଦି (json_object_is_type (doc, json_type_array))
      {
        size_t i, len = json_object_array_length (doc);

        /* len will probably always be 3 as crun is currently only
         * recording the destination of FD 0, 1 and 2. */
        ପାଇଁ (i = 0; i < len; ++i)
          {
            json_object *s = json_object_array_get_idx (doc, i);
            ଯଦି (s && json_object_is_type (s, json_type_string))
              {
                ଅଚଳ ଅକ୍ଷର *str = json_object_get_string (s);
                ଯଦି (has_prefix (str, "pipe:"))
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
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = setup_criu_work_dir (cr_options, &work_fd, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (cr_options->lsm_profile != ଖାଲି)
    {
      ret = libcriu_wrapper->criu_set_lsm_profile (cr_options->lsm_profile);
      ଯଦି (UNLIKELY (ret != 0))
        ଫେରନ୍ତୁ crun_make_error (err, -ret, "error setting LSM profile to `%s`", cr_options->lsm_profile);
    }

  ଯଦି (cr_options->lsm_mount_context != ଖାଲି)
    {
      ret = libcriu_wrapper->criu_set_lsm_mount_context (cr_options->lsm_mount_context);
      ଯଦି (UNLIKELY (ret != 0))
        ଫେରନ୍ତୁ crun_make_error (err, -ret, "error setting LSM mount context to `%s`", cr_options->lsm_mount_context);
    }

  /* do realpath on root */
  bundle_cleanup = realpath (status->bundle, ଖାଲି);
  ଯଦି (UNLIKELY (bundle_cleanup == ଖାଲି))
    bundle_cleanup = xstrdup (status->bundle);

  /* Tell CRIU about external bind mounts. */
  ପାଇଁ (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      ଯଦି (is_bind_mount (def->mounts[i], ଖାଲି, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          ଅକ୍ଷର buf[PATH_MAX];
          ଅଚଳ ଅକ୍ଷର *dest_in_ମୂଳ;
          ଅଚଳ ଅକ୍ଷର *source = def->mounts[i]->source;
          cleanup_free ଅକ୍ଷର *abs_source = ଖାଲି;
          unsigned long flags = 0;
          size_t j;

          ଯଦି (nofollow)
            ଫେରନ୍ତୁ crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_in_ମୂଳ = chroot_realpath (status->rootfs, def->mounts[i]->destination, buf);
          ଯଦି (UNLIKELY (dest_in_ମୂଳ == ଖାଲି))
            ଫେରନ୍ତୁ crun_make_error (err, errno, "unable to resolve external bind mount destination `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          ଯଦି (has_prefix (dest_in_ମୂଳ, status->rootfs))
            dest_in_ମୂଳ += safe_strlen (status->rootfs);

          /* A relative source is relative to the bundle, while CRIU would
             resolve it relative to its own working directory.  */
          ଯଦି (source && source[0] != '/')
            {
              ret = append_paths (&abs_source, err, bundle_cleanup, source, ଖାଲି);
              ଯଦି (UNLIKELY (ret < 0))
                ଫେରନ୍ତୁ ret;
              source = abs_source;
            }

          ret = libcriu_wrapper->criu_add_ext_mount (dest_in_ମୂଳ, source);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", source);

          ପାଇଁ (j = 0; j < def->mounts[i]->options_len; j++)
            {
              ଅଚଳ ସଂରଚନା propagation_flags_s *prop = ଖାଲି;

              ଯଦି (def->mounts[i]->options[j])
                prop = libcrun_str2mount_flags (def->mounts[i]->options[j]);
              ଯଦି (prop && ! prop->clear)
                flags |= prop->flags;
            }
          add_restored_ext_mount (dest_in_ମୂଳ, flags);
        }
    }

  ret = register_masked_paths_mounts (def, container, libcriu_wrapper, true, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  /* Mount the container rootfs for CRIU. */
  ret = append_paths (&ମୂଳ, err, bundle_cleanup, "criu-root", ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = mkdir (ମୂଳ, 0755);
  ଯଦି (UNLIKELY (ret == -1))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "error creating restore directory `%s`", ମୂଳ);

  ret = mount (status->rootfs, ମୂଳ, ଖାଲି, MS_BIND | MS_REC, ଖାଲି);
  ଯଦି (UNLIKELY (ret == -1))
    {
      ret = crun_make_error (err, errno, "error mounting restore directory `%s`", ମୂଳ);
      ଯାଅ out;
    }

  /* During initial container creation, crun will create mountpoints
   * defined in config.json if they do not exist. If we are restoring
   * we need to make sure these mountpoints also exist.
   * This is not perfect, as this means crun will modify a rootfs
   * even if it marked as read-only, but runc already modifies
   * the rootfs in the same way. */

  ret = prepare_restore_mounts (def, ମୂଳ, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ out_umount;

  ret = libcriu_wrapper->criu_set_ମୂଳ (ମୂଳ);
  ଯଦି (UNLIKELY (ret != 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU root to `%s`", ମୂଳ);
      ଯାଅ out_umount;
    }

  /* If a namespace defined in config.json we are telling
   * CRIU use that namespace when restoring the process tree.
   *
   * CRIU expects the information about the namespace like this:
   * --inherit-fd fd[<fd>]:<key>
   * The <key> needs to be the same as during checkpointing (extRootNetNS). */
  ପାଇଁ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା open_flags_for_inherit = O_RDONLY; /* Cannot be O_CLOEXEC as it is passed to the child process. */
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ = libcrun_find_namespace (def->linux->namespaces[i]->type);
      ଯଦି (UNLIKELY (ମୂଲ୍ୟ < 0))
        {
          ret = crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);
          ଯାଅ out_umount;
        }

      ଯଦି (ମୂଲ୍ୟ == CLONE_NEWNET && def->linux->namespaces[i]->path != ଖାଲି)
        {
          inherit_new_net_fd = open (def->linux->namespaces[i]->path, open_flags_for_inherit);
          ଯଦି (UNLIKELY (inherit_new_net_fd < 0))
            {
              ret = crun_make_error (err, errno, "unable to open(): `%s`", def->linux->namespaces[i]->path);
              ଯାଅ out_umount;
            }

          ret = libcriu_wrapper->criu_add_inherit_fd (inherit_new_net_fd, CRIU_EXT_NETNS);
          ଯଦି (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              ଯାଅ out_umount;
            }
        }

      ଯଦି (ମୂଲ୍ୟ == CLONE_NEWPID && def->linux->namespaces[i]->path != ଖାଲି)
        {
          inherit_new_pid_fd = open (def->linux->namespaces[i]->path, open_flags_for_inherit);
          ଯଦି (UNLIKELY (inherit_new_pid_fd < 0))
            {
              ret = crun_make_error (err, errno, "unable to open(): `%s`", def->linux->namespaces[i]->path);
              ଯାଅ out_umount;
            }

          ret = libcriu_wrapper->criu_add_inherit_fd (inherit_new_pid_fd, CRIU_EXT_PIDNS);
          ଯଦି (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              ଯାଅ out_umount;
            }
        }

#  ifdef CRIU_JOIN_NS_SUPPORT
      ଯଦି (ମୂଲ୍ୟ == CLONE_NEWTIME && def->linux->namespaces[i]->path != ଖାଲି)
        {
          ଯଦି (libcriu_wrapper->criu_join_ns_add == ଖାଲି)
            {
              ret = crun_make_error (err, 0, "shared time namespace restore is supported in CRIU >= 3.16.1");
              ଯାଅ out_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_add ("time", def->linux->namespaces[i]->path, ଖାଲି);
          ଯଦି (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->path);
              ଯାଅ out_umount;
            }
        }

      ଯଦି (ମୂଲ୍ୟ == CLONE_NEWIPC && def->linux->namespaces[i]->path != ଖାଲି)
        {
          ଯଦି (libcriu_wrapper->criu_join_ns_add == ଖାଲି)
            {
              ret = crun_make_error (err, 0, "shared ipc namespace restore is supported in CRIU >= 3.16.1");
              ଯାଅ out_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_add ("ipc", def->linux->namespaces[i]->path, ଖାଲି);
          ଯଦି (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->path);
              ଯାଅ out_umount;
            }
        }

      ଯଦି (ମୂଲ୍ୟ == CLONE_NEWUTS && def->linux->namespaces[i]->path != ଖାଲି)
        {
          ଯଦି (libcriu_wrapper->criu_join_ns_add == ଖାଲି)
            {
              ret = crun_make_error (err, 0, "shared uts namespace restore is supported in CRIU >= 3.16.1");
              ଯାଅ out_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_add ("uts", def->linux->namespaces[i]->path, ଖାଲି);
          ଯଦି (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->path);
              ଯାଅ out_umount;
            }
        }
#  endif
    }

  /* Set up CRIU config file */
  ret = handle_criu_config_file (container, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ out_umount;

  /* Tell CRIU if cgroup v1 needs to be handled. */
  ret = restore_cgroup_v1_mount (def, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ out_umount;

  console_socket = cr_options->console_socket;
  libcriu_wrapper->criu_set_notify_cb (criu_notify);

  /* Set boolean options . */
  libcriu_wrapper->criu_set_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_set_shell_job (cr_options->shell_job);
  libcriu_wrapper->criu_set_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_set_tcp_close (cr_options->tcp_close);
  libcriu_wrapper->criu_set_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_set_orphan_pts_master (true);

  ଯଦି (status->cgroup_path)
    {
      cgroup_mode = libcrun_get_cgroup_mode (err);
      ଯଦି (UNLIKELY (cgroup_mode < 0))
        {
          ret = cgroup_mode;
          ଯାଅ out_umount;
        }

      /* With cgroup v2, the cgroup path is only meaningful for the unified
         hierarchy, so only relocate that one.  A NULL controller would make
         CRIU relocate every hierarchy the container is in, named cgroup v1
         ones included.  As CRIU dumps such a hierarchy from wherever the
         container is in it, which is its root, as crun leaves named
         hierarchies alone, every checkpoint and restore would then copy the
         whole hierarchy into itself.  */
      ret = libcriu_wrapper->criu_add_cg_ମୂଳ (cgroup_mode == CGROUP_MODE_UNIFIED ? "" : ଖାଲି,
                                               status->cgroup_path);
      ଯଦି (UNLIKELY (ret != 0))
        {
          ret = crun_make_error (err, 0, "error setting CRIU cgroup root to `%s`", status->cgroup_path);
          ଯାଅ out_umount;
        }
    }

  libcriu_wrapper->criu_set_manage_cgroups_mode (criu_cg_mode_from_public (cr_options->manage_cgroups_mode));
  libcriu_wrapper->criu_set_manage_cgroups (true);

  ଯଦି (libcriu_wrapper->criu_set_network_lock)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା lock = 0;

      ret = criu_network_lock_from_public (cr_options->network_lock_method, &lock, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଯାଅ out_umount;

      ଯଦି (lock > 0)
        {
          ret = libcriu_wrapper->criu_set_network_lock (lock);
          ଯଦି (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "CRIU: failed setting network lock");
              ଯାଅ out_umount;
            }
        }
    }

  libcriu_wrapper->criu_set_log_level (4);
  ret = libcriu_wrapper->criu_set_log_file (CRIU_RESTORE_LOG_FILE);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU log file to `%s`", CRIU_RESTORE_LOG_FILE);
      ଯାଅ out_umount;
    }

  /* criu_restore() returns the PID of the process of the restored process
   * tree. This PID will not be the same as status->pid if the container is
   * running in a PID namespace. But it will always be > 0. */
  ଯଦି (cr_options->detach)
    {
      /* Nothing waits for the container, so there is no reason to take over
         this process: use a throw-away one.  It is used even when there is no
         cgroup to join, so that the container is not left as a child of a
         process which is not going to reap it.  */
      ret = criu_restore_child_in_cgroup (status->cgroup_path, &criu_ret, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଯାଅ out_umount;

      ret = criu_ret;
    }
  ଅନ୍ୟଥା
    {
      /* Like runc does, put CRIU into the container cgroup for the time of
       * restore, so the restored tasks are created there. It is necessary in
       * the "ignore" mode, where CRIU does not deal with cgroups at all, and
       * does not hurt otherwise. As CRIU is our child, do it by moving
       * ourselves there, and move back once the restore is done. */
      ଯଦି (! is_empty_string (status->cgroup_path))
        {
          ret = read_all_file (PROC_SELF_CGROUP, &own_cgroups, ଖାଲି, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଯାଅ out_umount;

          ret = libcrun_move_process_to_cgroup (0, 0, status->cgroup_path, false, err);
          ଯଦି (UNLIKELY (ret < 0))
            {
              /* Some of the cgroups might have been joined already.  */
              move_back_to_cgroups (own_cgroups);
              ଯାଅ out_umount;
            }
        }

      ret = libcriu_wrapper->criu_restore_child ();

      ଯଦି (own_cgroups)
        move_back_to_cgroups (own_cgroups);
    }

  ଯଦି (UNLIKELY (ret <= 0))
    {
      show_criu_log (cr_options->work_path, CRIU_RESTORE_LOG_FILE);
      ret = crun_make_error (err, 0, "CRIU restoring failed: %d", ret);
      ଯାଅ out_umount;
    }

  /* Update the status struct with the newly allocated PID. This will
   * be necessary later when moving the process into its cgroup. */
  status->pid = ret;
  ret = 0;

out_umount:
  ret_out = umount (ମୂଳ);
  ଯଦି (UNLIKELY (ret_out == -1))
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା saved_errno = errno;
      rmdir (ମୂଳ);
      ଯଦି (ret < 0)
        ଫେରନ୍ତୁ crun_error_wrap (err, "error unmounting restore directory `%s`", ମୂଳ);
      ଅନ୍ୟଥା
        ଫେରନ୍ତୁ crun_make_error (err, saved_errno, "error unmounting restore directory `%s`", ମୂଳ);
    }
out:
  ret_out = rmdir (ମୂଳ);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;
  ଯଦି (UNLIKELY (ret_out == -1))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "error removing restore directory `%s`", ମୂଳ);
  ଫେରନ୍ତୁ ret;
}
#endif
