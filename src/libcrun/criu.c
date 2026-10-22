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
#నిర్వచించు GNU_ఆధారం

#అన్తర్భావ <config.h>

#ఒకవేళ HAVE_CRIU && HAVE_DLOPEN

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
అక్షరం *chroot_realpath (స్థిరరాశి అక్షరం *chroot, స్థిరరాశి అక్షరం *మార్గం, అక్షరం resolved_మార్గం[]);

/* Map the public cgroups mode (enum libcrun_cr_cgroups_mode) onto the CRIU enum.  */
స్థిర గణన criu_cg_బహుళకం
criu_cg_బహుళకం_from_ప్రజా (పూర్ణాంకం బహుళకం)
{
  ఎంపిక (బహుళకం)
    {
    స్థితి LIBCRUN_CR_CG_బహుళకం_IGNORE:
      తిరిగి CRIU_CG_బహుళకం_IGNORE;
    స్థితి LIBCRUN_CR_CG_బహుళకం_నిండిన:
      తిరిగి CRIU_CG_బహుళకం_నిండిన;
    స్థితి LIBCRUN_CR_CG_బహుళకం_STRICT:
      తిరిగి CRIU_CG_బహుళకం_STRICT;
    స్థితి LIBCRUN_CR_CG_బహుళకం_DEFAULT:
    స్థితి LIBCRUN_CR_CG_బహుళకం_SOFT:
    అప్రమేయ:
      /* Default to CRIU_CG_MODE_SOFT, just as runc.  */
      తిరిగి CRIU_CG_బహుళకం_SOFT;
    }
}

/* Map the public network lock method (enum libcrun_cr_network_lock_method) onto the
   CRIU enum.  *LOCK is set to 0 when no explicit method must be set (CRIU default).
   A method that this build cannot honor is an error, never a silent fallback to
   the CRIU default.  */
స్థిర పూర్ణాంకం
criu_జాలం_తాళం_from_ప్రజా (పూర్ణాంకం పద్ధతి, పూర్ణాంకం *తాళం, libcrun_error_t *err)
{
  ఎంపిక (పద్ధతి)
    {
    స్థితి LIBCRUN_CR_జాలం_తాళం_IPTABLES:
      *తాళం = CRIU_జాలం_తాళం_IPTABLES;
      తిరిగి 0;
    స్థితి LIBCRUN_CR_జాలం_తాళం_NFTABLES:
      *తాళం = CRIU_జాలం_తాళం_NFTABLES;
      తిరిగి 0;
    స్థితి LIBCRUN_CR_జాలం_తాళం_SKIP:
#  ఒకవేళ CRIU_జాలం_తాళం_SKIP_SUPPORT
      *తాళం = CRIU_జాలం_తాళం_SKIP;
      తిరిగి 0;
#  లేకపోతే
      తిరిగి crun_make_error (err, 0, "CRIU: the `skip` network lock method is not supported by this build");
#  endif
    స్థితి LIBCRUN_CR_జాలం_తాళం_DEFAULT:
      *తాళం = 0;
      తిరిగి 0;
    అప్రమేయ:
      తిరిగి crun_make_error (err, 0, "CRIU: unknown network lock method %d", పద్ధతి);
    }
}

స్థిర స్థిరరాశి అక్షరం *console_socket = ఖాళీ;

/* The external mounts CRIU restores, and the flags the configuration asks
   for each of them.  */
స్థిర నిర్మాణం libcrun_mount_flags_s *restored_ext_mounts = ఖాళీ;
స్థిర size_t n_restored_ext_mounts = 0;

#  define LIBCRIU_MIN_సంస్కరణ 31500

నిర్మాణం libcriu_wrapper_s
{
  శూన్య *handle;
  పూర్ణాంకం (*criu_కూడు_ext_mount) (స్థిరరాశి అక్షరం *కీలకం, స్థిరరాశి అక్షరం *val);
  పూర్ణాంకం (*criu_కూడు_external) (స్థిరరాశి అక్షరం *కీలకం);
  పూర్ణాంకం (*criu_కూడు_inherit_fd) (పూర్ణాంకం fd, స్థిరరాశి అక్షరం *కీలకం);
  పూర్ణాంకం (*criu_check_సంస్కరణ) (పూర్ణాంకం minimum);
  పూర్ణాంకం (*criu_dump) (శూన్య);
  పూర్ణాంకం (*criu_get_orphan_pts_master_fd) (శూన్య);
  పూర్ణాంకం (*criu_init_opts) (శూన్య);
  పూర్ణాంకం (*criu_notify_pid) (criu_notify_arg_t na);
#  ifdef CRIU_JOIN_NS_SUPPORT
  పూర్ణాంకం (*criu_join_ns_కూడు) (స్థిరరాశి అక్షరం *ns, స్థిరరాశి అక్షరం *ns_file, స్థిరరాశి అక్షరం *extra_opt);
#  endif
#  ifdef CRIU_PRE_DUMP_SUPPORT
  పూర్ణాంకం (*criu_feature_check) (నిర్మాణం criu_feature_check *features, size_t కొలత);
  పూర్ణాంకం (*criu_pre_dump) (శూన్య);
#  endif
  పూర్ణాంకం (*criu_restore_బిడ్డ) (శూన్య);
  పూర్ణాంకం (*criu_సమితి_freeze_cgroup) (స్థిరరాశి అక్షరం *name);
  శూన్య (*criu_సమితి_file_locks) (bool file_locks);
  శూన్య (*criu_సమితి_ext_unix_sk) (bool ext_unix_sk);
  పూర్ణాంకం (*criu_సమితి_log_file) (స్థిరరాశి అక్షరం *log_file);
  శూన్య (*criu_సమితి_log_level) (పూర్ణాంకం log_level);
  శూన్య (*criu_సమితి_leave_running) (bool leave_running);
  శూన్య (*criu_సమితి_manage_cgroups) (bool manage);
  శూన్య (*criu_సమితి_manage_cgroups_బహుళకం) (గణన criu_cg_బహుళకం బహుళకం);
  పూర్ణాంకం (*criu_సమితి_జాలం_తాళం) (గణన criu_జాలం_తాళం_పద్ధతి పద్ధతి);
  శూన్య (*criu_సమితి_notify_cb) (పూర్ణాంకం (*cb) (అక్షరం *action, criu_notify_arg_t na));
  శూన్య (*criu_సమితి_orphan_pts_master) (bool orphan_pts_master);
  శూన్య (*criu_సమితి_images_dir_fd) (పూర్ణాంకం fd);
  పూర్ణాంకం (*criu_సమితి_మాతృ_images) (స్థిరరాశి అక్షరం *మార్గం);
  శూన్య (*criu_సమితి_pid) (పూర్ణాంకం pid);
  పూర్ణాంకం (*criu_సమితి_మూలం) (స్థిరరాశి అక్షరం *మూలం);
  పూర్ణాంకం (*criu_కూడు_cg_మూలం) (స్థిరరాశి అక్షరం *ctrl, స్థిరరాశి అక్షరం *మార్గం);
  శూన్య (*criu_సమితి_shell_పని) (bool shell_పని);
  శూన్య (*criu_సమితి_tcp_established) (bool tcp_established);
  శూన్య (*criu_సమితి_tcp_close) (bool tcp_close);
  శూన్య (*criu_సమితి_track_mem) (bool track_mem);
  శూన్య (*criu_సమితి_work_dir_fd) (పూర్ణాంకం fd);
  పూర్ణాంకం (*criu_సమితి_lsm_profile) (స్థిరరాశి అక్షరం *name);
  పూర్ణాంకం (*criu_సమితి_lsm_mount_context) (స్థిరరాశి అక్షరం *name);
  పూర్ణాంకం (*criu_సమితి_config_file) (స్థిరరాశి అక్షరం *మార్గం);
};

స్థిర నిర్మాణం libcriu_wrapper_s *libcriu_wrapper;

స్థిర అంతఃవరుస శూన్య
cleanup_wrapper (శూన్య *p)
{
  నిర్మాణం libcriu_wrapper_s **w;

  w = (నిర్మాణం libcriu_wrapper_s **) p;
  ఒకవేళ (*w == ఖాళీ)
    తిరిగి;

#  ifndef STATIC
  ఒకవేళ ((*w)->handle)
    dlclose ((*w)->handle);
#  endif
  విడుదల (*w);
  libcriu_wrapper = ఖాళీ;

  అయితే (n_restored_ext_mounts > 0)
    విడుదల (restored_ext_mounts[--n_restored_ext_mounts].లక్ష్యం);
  విడుదల (restored_ext_mounts);
  restored_ext_mounts = ఖాళీ;
}

#  define cleanup_wrapper __attribute__ ((cleanup (cleanup_wrapper)))

స్థిర పూర్ణాంకం
ఎక్కించు_wrapper (నిర్మాణం libcriu_wrapper_s **wrapper_బహిర్గతం, libcrun_error_t *err)
{
  cleanup_free నిర్మాణం libcriu_wrapper_s *wrapper = xmalloc0 (పరిమాణం (*wrapper));

#  ifdef STATIC
#    define ఎక్కించు_CRIU_FUNCTION(X, ALLOW_NULL) \
      wrapper->X = &X;
#  లేకపోతే
#    define ఎక్కించు_CRIU_FUNCTION(X, ALLOW_NULL)                                                    \
      చేయి                                                                                         \
        {                                                                                        \
          wrapper->X = dlsym (wrapper->handle, #X);                                              \
          ఒకవేళ (! ALLOW_NULL && wrapper->X == ఖాళీ)                                                \
            {                                                                                    \
              dlclose (wrapper->handle);                                                         \
              తిరిగి crun_make_error (err, 0, "could not find symbol `%s` in `libcriu.so`", #X); \
            }                                                                                    \
      } అయితే (0)
#  endif

#  ifndef STATIC
  wrapper->handle = dlopen ("libcriu.so.2", RTLD_NOW);
  ఒకవేళ (wrapper->handle == ఖాళీ)
    తిరిగి crun_make_error (err, 0, "could not load `libcriu.so.2`: `%s`", dlerror ());
#  endif

  ఎక్కించు_CRIU_FUNCTION (criu_కూడు_ext_mount, false);
  ఎక్కించు_CRIU_FUNCTION (criu_కూడు_external, false);
  ఎక్కించు_CRIU_FUNCTION (criu_కూడు_inherit_fd, false);
  ఎక్కించు_CRIU_FUNCTION (criu_check_సంస్కరణ, false);
  ఎక్కించు_CRIU_FUNCTION (criu_dump, false);
  ఎక్కించు_CRIU_FUNCTION (criu_get_orphan_pts_master_fd, false);
  ఎక్కించు_CRIU_FUNCTION (criu_init_opts, false);
  ఎక్కించు_CRIU_FUNCTION (criu_notify_pid, false);

#  ifdef CRIU_JOIN_NS_SUPPORT
  /* criu_join_ns_add() API was introduced with CRIU version 3.16.1
   * Here we check if this API is available at build time to support
   * compiling with older version of CRIU, and at runtime to support
   * running crun with older versions of libcriu.so.2.
   */
  ఎక్కించు_CRIU_FUNCTION (criu_join_ns_కూడు, true);
#  endif

#  ifdef CRIU_PRE_DUMP_SUPPORT
  ఎక్కించు_CRIU_FUNCTION (criu_feature_check, false);
  ఎక్కించు_CRIU_FUNCTION (criu_pre_dump, false);
#  endif
  ఎక్కించు_CRIU_FUNCTION (criu_restore_బిడ్డ, false);
  ఎక్కించు_CRIU_FUNCTION (criu_సమితి_ext_unix_sk, false);
  ఎక్కించు_CRIU_FUNCTION (criu_సమితి_file_locks, false);
  ఎక్కించు_CRIU_FUNCTION (criu_సమితి_freeze_cgroup, false);
  ఎక్కించు_CRIU_FUNCTION (criu_సమితి_images_dir_fd, false);
  ఎక్కించు_CRIU_FUNCTION (criu_సమితి_leave_running, false);
  ఎక్కించు_CRIU_FUNCTION (criu_సమితి_log_file, false);
  ఎక్కించు_CRIU_FUNCTION (criu_సమితి_log_level, false);
  ఎక్కించు_CRIU_FUNCTION (criu_సమితి_manage_cgroups, false);
  ఎక్కించు_CRIU_FUNCTION (criu_సమితి_manage_cgroups_బహుళకం, false);
  ఎక్కించు_CRIU_FUNCTION (criu_సమితి_జాలం_తాళం, true);
  ఎక్కించు_CRIU_FUNCTION (criu_సమితి_notify_cb, false);
  ఎక్కించు_CRIU_FUNCTION (criu_సమితి_orphan_pts_master, false);
  ఎక్కించు_CRIU_FUNCTION (criu_సమితి_మాతృ_images, false);
  ఎక్కించు_CRIU_FUNCTION (criu_సమితి_pid, false);
  ఎక్కించు_CRIU_FUNCTION (criu_సమితి_మూలం, false);
  ఎక్కించు_CRIU_FUNCTION (criu_కూడు_cg_మూలం, false);
  ఎక్కించు_CRIU_FUNCTION (criu_సమితి_shell_పని, false);
  ఎక్కించు_CRIU_FUNCTION (criu_సమితి_tcp_established, false);
  ఎక్కించు_CRIU_FUNCTION (criu_సమితి_tcp_close, false);
  ఎక్కించు_CRIU_FUNCTION (criu_సమితి_track_mem, false);
  ఎక్కించు_CRIU_FUNCTION (criu_సమితి_work_dir_fd, false);
  ఎక్కించు_CRIU_FUNCTION (criu_సమితి_lsm_profile, false);
  ఎక్కించు_CRIU_FUNCTION (criu_సమితి_lsm_mount_context, false);
#  ఒకవేళ ! defined STATIC || defined CRIU_CONFIG_FILE
  ఎక్కించు_CRIU_FUNCTION (criu_సమితి_config_file, true);
#  endif

  libcriu_wrapper = *wrapper_బహిర్గతం = wrapper;
  wrapper = ఖాళీ;
#  undef ఎక్కించు_CRIU_FUNCTION
  తిరిగి 0;
}

స్థిర శూన్య
కూడు_restored_ext_mount (స్థిరరాశి అక్షరం *లక్ష్యం, గుర్తులేని దీర్ఘ flags)
{
  restored_ext_mounts = xrealloc (restored_ext_mounts, పరిమాణం (*restored_ext_mounts) * (n_restored_ext_mounts + 1));
  restored_ext_mounts[n_restored_ext_mounts].లక్ష్యం = xstrdup (లక్ష్యం);
  restored_ext_mounts[n_restored_ext_mounts].flags = flags;
  n_restored_ext_mounts++;
}

స్థిర పూర్ణాంకం
criu_notify (అక్షరం *action, criu_notify_arg_t na)
{
  ఒకవేళ (strcmp (action, "post-restore") == 0)
    {
      /* CRIU takes the source of an external mount from us and its flags
       * from the image, i.e. from whatever mount was at that path in the
       * container when it was checkpointed.  The container can move its
       * mount points around, so before it is resumed make sure that each
       * source is mounted with the restrictions the configuration asks for. */
      libcrun_error_t tmp_err = ఖాళీ;
      పూర్ణాంకం ret;

      ret = libcrun_restrict_mounts_లోపల_container (libcriu_wrapper->criu_notify_pid (na), restored_ext_mounts,
                                                  n_restored_ext_mounts, &tmp_err);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          libcrun_error (tmp_err->స్థితి_2, "cannot set the flags of the restored mounts: %s", tmp_err->సందేశం);
          libcrun_error_release (&tmp_err);
          తిరిగి ret;
        }
    }

  ఒకవేళ (అక్షరశ్రేణి_పోలిక (action, "orphan-pts-master", 17) == 0)
    {
      /* CRIU sends us the master FD via the 'orphan-pts-master'
       * callback and we are passing it on to the '--console-socket'
       * if it exists. */
      cleanup_close పూర్ణాంకం console_socket_fd = -1;
      libcrun_error_t tmp_err = ఖాళీ;
      పూర్ణాంకం master_fd;
      పూర్ణాంకం ret;

      ఒకవేళ (! console_socket)
        తిరిగి 0;

      master_fd = libcriu_wrapper->criu_get_orphan_pts_master_fd ();

      console_socket_fd = open_unix_domain_client_socket (console_socket, 0, &tmp_err);
      ఒకవేళ (UNLIKELY (console_socket_fd < 0))
        {
          libcrun_error_release (&tmp_err);
          తిరిగి console_socket_fd;
        }
      ret = send_fd_గమ్యం_socket (console_socket_fd, master_fd, &tmp_err);
      ఒకవేళ (UNLIKELY (ret < 0))
        {
          libcrun_error_release (&tmp_err);
          తిరిగి ret;
        }
    }
  తిరిగి 0;
}

#  ifdef CRIU_PRE_DUMP_SUPPORT

స్థిర పూర్ణాంకం
criu_check_mem_track (libcrun_error_t *err)
{
  నిర్మాణం criu_feature_check features = { 0 };
  పూర్ణాంకం ret;

  /* Right now we are only interested in checking memory tracking.
   * Memory tracking can be disabled at different levels. aarch64
   * for example has memory tracking not implemented. It could also
   * be not enabled on other architectures. Just ask CRIU if that
   * features exists. */

  features.mem_track = true;

  ret = libcriu_wrapper->criu_feature_check (&features, పరిమాణం (features));
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, 0, "CRIU feature checking failed: %d", ret);

  ఒకవేళ (features.mem_track == true)
    తిరిగి 1;

  తిరిగి crun_make_error (err, 0, "CRIU memory tracking not supported");
}

#  endif

స్థిర పూర్ణాంకం
రిజిస్టరు_masked_paths_mounts (runtime_spec_schema_config_schema *def, libcrun_container_t *container,
                              నిర్మాణం libcriu_wrapper_s *libcriu_wrapper, bool is_restore, libcrun_error_t *err)
{
  cleanup_free అక్షరం *ఖాళీలు_dir_మార్గం = ఖాళీ;
  bool shared_dir_registered = false;
  size_t i;
  పూర్ణాంకం ret;

  కొరకు (i = 0; i < def->linux->masked_paths_len; i++)
    {
      నిర్మాణం stat statbuf;
      ret = stat (def->linux->masked_paths[i], &statbuf);
      ఒకవేళ (ret != 0)
        కొనసాగు;

      ఒకవేళ (S_ISDIR (statbuf.st_mode))
        {
          ఒకవేళ (! shared_dir_registered)
            {
              ret = get_shared_ఖాళీలు_directory_మార్గం (&ఖాళీలు_dir_మార్గం,
                                                     (container->context ? container->context->state_మూలం : ఖాళీ), err);
              ఒకవేళ (UNLIKELY (ret < 0))
                తిరిగి ret;

              ret = libcriu_wrapper->criu_కూడు_ext_mount (ఖాళీలు_dir_మార్గం, ఖాళీలు_dir_మార్గం);
              ఒకవేళ (UNLIKELY (ret < 0))
                తిరిగి crun_make_error (err, -ret, "CRIU: failed adding external mount for shared empty directory `%s`", ఖాళీలు_dir_మార్గం);

              shared_dir_registered = true;
            }

          ret = libcriu_wrapper->criu_కూడు_ext_mount (def->linux->masked_paths[i], ఖాళీలు_dir_మార్గం);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, -ret, "CRIU: failed adding external mount for masked directory `%s`", def->linux->masked_paths[i]);

          ఒకవేళ (is_restore)
            కూడు_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
      లేకపోతే ఒకవేళ (S_ISREG (statbuf.st_mode))
        {
          స్థిరరాశి అక్షరం *bind_లక్ష్యం = is_restore ? "/dev/null" : def->linux->masked_paths[i];
          ret = libcriu_wrapper->criu_కూడు_ext_mount (def->linux->masked_paths[i], bind_లక్ష్యం);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", bind_లక్ష్యం);

          ఒకవేళ (is_restore)
            కూడు_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
    }

  తిరిగి 0;
}

/* Parse one /proc/self/cgroup LINE into its subsystem, normalizing the name and
   stripping any "name=" prefix.  When SUBPATH_OUT is not NULL, the in-hierarchy
   path is stored there.  Returns the subsystem, or NULL if the line carries no
   controller and should be skipped.  LINE is modified in place.  */
స్థిర అక్షరం *
parse_cgroup_subsystem (అక్షరం *పంక్తి, అక్షరం **subpath_బహిర్గతం)
{
  అక్షరం *subsystem, *subpath, *it;

  subsystem = strchr (పంక్తి, ':') + 1;
  subpath = strchr (subsystem, ':') + 1;
  *(subpath - 1) = '\0';

  ఒకవేళ (subsystem[0] == '\0')
    తిరిగి ఖాళీ;

  it = అక్షరశ్రేణి_శోధన (subsystem, "name=");
  ఒకవేళ (it)
    subsystem = it + 5;

  ఒకవేళ (strcmp (subsystem, "net_prio,net_cls") == 0)
    subsystem = "net_cls,net_prio";
  ఒకవేళ (strcmp (subsystem, "cpuacct,cpu") == 0)
    subsystem = "cpu,cpuacct";

  ఒకవేళ (subpath_బహిర్గతం)
    *subpath_బహిర్గతం = subpath;
  తిరిగి subsystem;
}

స్థిర పూర్ణాంకం
restore_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free అక్షరం *content = ఖాళీ;
  bool has_cgroup_mount = false;
  అక్షరం *saveptr = ఖాళీ;
  పూర్ణాంకం cgroup_బహుళకం;
  అక్షరం *from;
  పూర్ణాంకం ret;
  uint32_t i;

  cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (cgroup_బహుళకం < 0))
    తిరిగి cgroup_బహుళకం;

  ఒకవేళ (cgroup_బహుళకం == CGROUP_బహుళకం_UNIFIED)
    తిరిగి 0;

  /* First check if there is actually a cgroup mount in the container. */
  కొరకు (i = 0; i < def->mounts_len; i++)
    {
      అక్షరం *type = def->mounts[i]->type;
      ఒకవేళ (type && strcmp (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          ఆపు;
        }
    }

  ఒకవేళ (! has_cgroup_mount)
    తిరిగి 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, ఖాళీ, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (UNLIKELY (content == ఖాళీ || content[0] == '\0'))
    తిరిగి crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  కొరకు (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ఖాళీ, "\n", &saveptr))
    {
      cleanup_free అక్షరం *destination = ఖాళీ;
      cleanup_free అక్షరం *ఆధారం = ఖాళీ;
      అక్షరం *subsystem;
      అక్షరం *subpath;

      subsystem = parse_cgroup_subsystem (from, &subpath);
      ఒకవేళ (subsystem == ఖాళీ)
        కొనసాగు;

      ret = append_paths (&ఆధారం, err, CGROUP_మూలం, subsystem, ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = append_paths (&destination, err, ఆధారం, subpath, ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = libcriu_wrapper->criu_కూడు_ext_mount (ఆధారం, destination);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", destination);
    }

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
checkpoint_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free అక్షరం *content = ఖాళీ;
  bool has_cgroup_mount = false;
  అక్షరం *saveptr = ఖాళీ;
  అక్షరం *from;
  పూర్ణాంకం ret;
  uint32_t i;

  /* First check if there is actually a cgroup mount in the container. */
  కొరకు (i = 0; i < def->mounts_len; i++)
    {
      అక్షరం *type = def->mounts[i]->type;
      ఒకవేళ (type && strcmp (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          ఆపు;
        }
    }

  ఒకవేళ (! has_cgroup_mount)
    తిరిగి 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, ఖాళీ, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (UNLIKELY (content == ఖాళీ || content[0] == '\0'))
    తిరిగి crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  కొరకు (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ఖాళీ, "\n", &saveptr))
    {
      cleanup_free అక్షరం *ఆధారం_మార్గం = ఖాళీ;
      అక్షరం *subsystem;

      subsystem = parse_cgroup_subsystem (from, ఖాళీ);
      ఒకవేళ (subsystem == ఖాళీ)
        కొనసాగు;

      ret = append_paths (&ఆధారం_మార్గం, err, CGROUP_మూలం, subsystem, ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = libcriu_wrapper->criu_కూడు_ext_mount (ఆధారం_మార్గం, ఆధారం_మార్గం);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", ఆధారం_మార్గం);
    }

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
handle_criu_config_file (libcrun_container_t *container, libcrun_error_t *err)
{
  పూర్ణాంకం ret;
  స్థిరరాశి అక్షరం *criu_config_annotation;
  స్థిరరాశి అక్షరం *config_file = CRIU_RUNC_CONFIG_FILE;

  criu_config_annotation = కనుగొను_annotation (container, "org.criu.config");

  /* Ignore missing criu_set_config_file() API for compatibility with older CRIU versions,
   * and show an error only if config file is explicitly set with annotation.
   */
  ఒకవేళ (libcriu_wrapper->criu_సమితి_config_file == ఖాళీ)
    {
      ఒకవేళ (criu_config_annotation)
        తిరిగి crun_make_error (err, 0, "libcriu RPC config files supported in CRIU >= 4.2");

      తిరిగి 0;
    }

  ఒకవేళ (criu_config_annotation)
    config_file = criu_config_annotation;
  లేకపోతే ఒకవేళ (access (CRIU_CRUN_CONFIG_FILE, F_OK) == 0)
    config_file = CRIU_CRUN_CONFIG_FILE;

  ret = libcriu_wrapper->criu_సమితి_config_file (config_file);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, 0, "failed to set CRIU config file");

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
validate_criu_సంస్కరణ (libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  // validate that the libcriu version is at least LIBCRIU_MIN_VERSION
  ret = libcriu_wrapper->criu_check_సంస్కరణ (LIBCRIU_MIN_సంస్కరణ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, -ret, "CRIU: failed checking version");

  ఒకవేళ (ret == 0)
    తిరిగి crun_make_error (err, 0, "libcriu is too old");

  తిరిగి 0;
}

స్థిర శూన్య
show_criu_log (స్థిరరాశి అక్షరం *work_మార్గం, స్థిరరాశి అక్షరం *సంవర్గమానం)
{
  cleanup_free అక్షరం *log_మార్గం = ఖాళీ;
  libcrun_error_t tmp_err = ఖాళీ;
  అక్షరం పంక్తి[1024];
  FILE *f;

  ఒకవేళ (UNLIKELY (append_paths (&log_మార్గం, &tmp_err, work_మార్గం, సంవర్గమానం, ఖాళీ) < 0))
    {
      crun_error_release (&tmp_err);
      తిరిగి;
    }

  f = fopen (log_మార్గం, "r");
  ఒకవేళ (f == ఖాళీ)
    {
      ఒకవేళ (errno != ENOENT)
        libcrun_error (errno, "Can't open CRIU log `%s`", log_మార్గం);
      తిరిగి;
    }

  /* Log with error verbosity as this is the default. */
  libcrun_error (0, "--- excerpt from CRIU log `%s`", log_మార్గం);
  అయితే (fgets (పంక్తి, పరిమాణం (పంక్తి), f) != ఖాళీ)
    ఒకవేళ (అక్షరశ్రేణి_శోధన (పంక్తి, "Error ") != ఖాళీ)
      {
        పంక్తి[strcspn (పంక్తి, "\n")] = '\0';
        libcrun_error (0, "%s", పంక్తి);
      }

  fclose (f);
  libcrun_error (0, "--- end of excerpt");
}

/* work_dir is the place CRIU will put its logfiles.  If not explicitly set, CRIU
 * will put the logfiles into the images_dir.  When set, create and open it and
 * hand the fd to CRIU; ownership of the returned fd stays with the caller.  */
స్థిర పూర్ణాంకం
అమరిక_criu_work_dir (libcrun_checkpoint_restore_t *cr_options, పూర్ణాంకం *work_fd_బహిర్గతం, libcrun_error_t *err)
{
  పూర్ణాంకం work_fd;

  ఒకవేళ (cr_options->work_మార్గం == ఖాళీ)
    {
      /* This is only for the error message later. */
      cr_options->work_మార్గం = cr_options->image_మార్గం;
      తిరిగి 0;
    }

  ఒకవేళ (UNLIKELY ((mkdir (cr_options->work_మార్గం, 0700) == -1) && (errno != EEXIST)))
    తిరిగి crun_make_error (err, errno, "error creating CRIU work directory `%s`", cr_options->work_మార్గం);

  work_fd = open (cr_options->work_మార్గం, O_DIRECTORY | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (work_fd == -1))
    తిరిగి crun_make_error (err, errno, "error opening CRIU work directory `%s`", cr_options->work_మార్గం);

  libcriu_wrapper->criu_సమితి_work_dir_fd (work_fd);
  *work_fd_బహిర్గతం = work_fd;
  తిరిగి 0;
}

పూర్ణాంకం
libcrun_container_checkpoint_linux_criu (libcrun_container_స్థితి_t *స్థితి_2, libcrun_container_t *container,
                                         libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper నిర్మాణం libcriu_wrapper_s *wrapper = ఖాళీ;
  cleanup_free అక్షరం *descriptors_మార్గం = ఖాళీ;
  cleanup_free అక్షరం *freezer_మార్గం = ఖాళీ;
  cleanup_free అక్షరం *మార్గం = ఖాళీ;
  cleanup_close పూర్ణాంకం image_fd = -1;
  cleanup_close పూర్ణాంకం work_fd = -1;
  పూర్ణాంకం cgroup_బహుళకం;
  size_t i;
  పూర్ణాంకం ret;

  ret = ఎక్కించు_wrapper (&wrapper, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (geteuid ())
    తిరిగి crun_make_error (err, 0, "checkpointing requires root");

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
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_సంస్కరణ (err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (UNLIKELY (cr_options->image_మార్గం == ఖాళీ))
    తిరిగి crun_make_error (err, 0, "image path not set");

  ret = mkdir (cr_options->image_మార్గం, 0700);
  ఒకవేళ (UNLIKELY ((ret == -1) && (errno != EEXIST)))
    తిరిగి crun_make_error (err, errno, "error creating checkpoint directory `%s`", cr_options->image_మార్గం);

  image_fd = open (cr_options->image_మార్గం, O_DIRECTORY | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (image_fd == -1))
    తిరిగి crun_make_error (err, errno, "error opening checkpoint directory `%s`", cr_options->image_మార్గం);

  libcriu_wrapper->criu_సమితి_images_dir_fd (image_fd);

  /* Set up logging. */
  libcriu_wrapper->criu_సమితి_log_level (4);
  libcriu_wrapper->criu_సమితి_log_file (CRIU_CHECKPOINT_LOG_FILE);

  /* Set up CRIU config file */
  ఒకవేళ (UNLIKELY (handle_criu_config_file (container, err)))
    తిరిగి -1;

  /* Setting the pid early as we can skip a lot of checkpoint setup if
   * we just do a pre-dump. The PID needs to be set always. Do it here.
   * The main process of the container is the process CRIU will checkpoint
   * and all of its children. */
  libcriu_wrapper->criu_సమితి_pid (స్థితి_2->pid);

  ret = అమరిక_criu_work_dir (cr_options, &work_fd, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

#  ifdef CRIU_PRE_DUMP_SUPPORT

  {
    పూర్ణాంకం criu_can_mem_track = 0;
    /* If the user uses --pre-dump for the second time or does
     * a final dump from a previous pre-dump, setting parent_path
     * is necessary so that CRIU can find which pages have not
     * changed compared to the previous dump. */
    ఒకవేళ (cr_options->మాతృ_మార్గం != ఖాళీ)
      {
        criu_can_mem_track = criu_check_mem_track (err);
        ఒకవేళ (UNLIKELY (criu_can_mem_track == -1))
          తిరిగి -1;
        libcriu_wrapper->criu_సమితి_track_mem (true);

        /* The parent path must be relative to image path (something like ../previous-dump).
           CRIU will fail with an unclear error message if the path is not right.
         */
        ఒకవేళ (UNLIKELY (cr_options->మాతృ_మార్గం[0] == '/'))
          తిరిగి crun_make_error (err, 0, "--parent-path must be relative");
        పూర్ణాంకం is_dir = crun_dir_p_at (image_fd, cr_options->మాతృ_మార్గం, false, err);
        ఒకవేళ (UNLIKELY (is_dir <= 0))
          {
            ఒకవేళ (is_dir < 0)
              తిరిగి crun_error_wrap (err, "invalid --parent-path");
            తిరిగి crun_make_error (err, ENOTDIR, "invalid --parent-path");
          }
        ret = libcriu_wrapper->criu_సమితి_మాతృ_images (cr_options->మాతృ_మార్గం);
        ఒకవేళ (UNLIKELY (ret != 0))
          తిరిగి crun_make_error (err, -ret, "error setting CRIU parent images path to `%s`", cr_options->మాతృ_మార్గం);
      }

    ఒకవేళ (cr_options->pre_dump)
      {
        ఒకవేళ (criu_can_mem_track != 1)
          {
            criu_can_mem_track = criu_check_mem_track (err);
            ఒకవేళ (UNLIKELY (criu_can_mem_track == -1))
              తిరిగి -1;
          }
        libcriu_wrapper->criu_సమితి_track_mem (true);
        ret = libcriu_wrapper->criu_pre_dump ();
        ఒకవేళ (UNLIKELY (ret != 0))
          {
            show_criu_log (cr_options->work_మార్గం, CRIU_CHECKPOINT_LOG_FILE);
            తిరిగి crun_make_error (err, 0, "CRIU pre-dump failed: %d", ret);
          }
        తిరిగి 0;
      }
  }
#  endif

  /* descriptors.json is needed during restore to correctly
   * reconnect stdin, stdout, stderr. */
  ret = append_paths (&descriptors_మార్గం, err, cr_options->image_మార్గం, DESCRIPTORS_FILENAME, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = write_file (descriptors_మార్గం, స్థితి_2->external_descriptors, సురక్షిత_strlen (స్థితి_2->external_descriptors), err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_error_wrap (err, "error saving CRIU descriptors file");

  ret = append_paths (&మార్గం, err, స్థితి_2->bundle, స్థితి_2->rootfs, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = libcriu_wrapper->criu_సమితి_మూలం (మార్గం);
  ఒకవేళ (UNLIKELY (ret != 0))
    తిరిగి crun_make_error (err, 0, "error setting CRIU root to `%s`", మార్గం);

  cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
  ఒకవేళ (UNLIKELY (cgroup_బహుళకం < 0))
    తిరిగి cgroup_బహుళకం;

  /* For cgroup v1 we need to tell CRIU to handle all cgroup mounts as external mounts. */
  ఒకవేళ (cgroup_బహుళకం != CGROUP_బహుళకం_UNIFIED)
    {
      ret = checkpoint_cgroup_v1_mount (def, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  /* Tell CRIU about external bind mounts. */
  కొరకు (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      ఒకవేళ (is_bind_mount (def->mounts[i], ఖాళీ, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          అక్షరం buf[PATH_MAX];
          స్థిరరాశి అక్షరం *dest_లోపల_మూలం;

          ఒకవేళ (nofollow)
            తిరిగి crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_లోపల_మూలం = chroot_realpath (స్థితి_2->rootfs, def->mounts[i]->destination, buf);
          ఒకవేళ (UNLIKELY (dest_లోపల_మూలం == ఖాళీ))
            తిరిగి crun_make_error (err, errno, "unable to resolve external bind mount `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          ఒకవేళ (has_ఉపసర్గ (dest_లోపల_మూలం, స్థితి_2->rootfs))
            dest_లోపల_మూలం += సురక్షిత_strlen (స్థితి_2->rootfs);

          ret = libcriu_wrapper->criu_కూడు_ext_mount (dest_లోపల_మూలం, dest_లోపల_మూలం);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", def->mounts[i]->destination);
        }
    }

  ret = రిజిస్టరు_masked_paths_mounts (def, container, libcriu_wrapper, false, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

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

  కొరకు (i = 0; i < def->linux->namespaces_len; i++)
    {
      పూర్ణాంకం విలువ = libcrun_కనుగొను_namespace (def->linux->namespaces[i]->type);
      ఒకవేళ (UNLIKELY (విలువ < 0))
        తిరిగి crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      ఒకవేళ (విలువ == CLONE_NEWNET && def->linux->namespaces[i]->మార్గం != ఖాళీ)
        {
          cleanup_free అక్షరం *external = ఖాళీ;
          నిర్మాణం stat statbuf;

          ret = stat (def->linux->namespaces[i]->మార్గం, &statbuf);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "unable to stat(): `%s`", def->linux->namespaces[i]->మార్గం);

          xasprintf (&external, "net[%ld]:" CRIU_EXT_NETNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_కూడు_external (external);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }

      ఒకవేళ (విలువ == CLONE_NEWPID && def->linux->namespaces[i]->మార్గం != ఖాళీ)
        {
          cleanup_free అక్షరం *external = ఖాళీ;
          నిర్మాణం stat statbuf;

          ret = stat (def->linux->namespaces[i]->మార్గం, &statbuf);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, errno, "unable to stat(): `%s`", def->linux->namespaces[i]->మార్గం);

          xasprintf (&external, "pid[%ld]:" CRIU_EXT_PIDNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_కూడు_external (external);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }
    }

  /* Tell CRIU to use the freezer to pause all container processes. */
  ఒకవేళ (cgroup_బహుళకం == CGROUP_బహుళకం_UNIFIED)
    {
      /* This needs CRIU 3.14. */
      ret = append_paths (&freezer_మార్గం, err, CGROUP_మూలం, స్థితి_2->cgroup_మార్గం, ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }
  లేకపోతే
    {
      ret = append_paths (&freezer_మార్గం, err, CGROUP_మూలం "/freezer", స్థితి_2->cgroup_మార్గం, ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  ret = libcriu_wrapper->criu_సమితి_freeze_cgroup (freezer_మార్గం);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, -ret, "CRIU: failed setting freezer %d", ret);

  /* Set boolean options . */
  libcriu_wrapper->criu_సమితి_leave_running (cr_options->leave_running);
  libcriu_wrapper->criu_సమితి_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_సమితి_shell_పని (cr_options->shell_పని);
  libcriu_wrapper->criu_సమితి_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_సమితి_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_సమితి_orphan_pts_master (true);
  libcriu_wrapper->criu_సమితి_manage_cgroups_బహుళకం (criu_cg_బహుళకం_from_ప్రజా (cr_options->manage_cgroups_బహుళకం));

  libcriu_wrapper->criu_సమితి_manage_cgroups (true);

  ఒకవేళ (libcriu_wrapper->criu_సమితి_జాలం_తాళం)
    {
      పూర్ణాంకం తాళం = 0;

      ret = criu_జాలం_తాళం_from_ప్రజా (cr_options->జాలం_తాళం_పద్ధతి, &తాళం, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ఒకవేళ (తాళం > 0)
        {
          ret = libcriu_wrapper->criu_సమితి_జాలం_తాళం (తాళం);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, 0, "CRIU: failed setting network lock");
        }
    }

  ret = libcriu_wrapper->criu_dump ();
  ఒకవేళ (UNLIKELY (ret != 0))
    {
      show_criu_log (cr_options->work_మార్గం, CRIU_CHECKPOINT_LOG_FILE);
      తిరిగి crun_make_error (err, ret < 0 ? -ret : 0, "CRIU checkpointing failed: %d", ret);
    }

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
prepare_restore_mounts_అంతర్గత (runtime_spec_schema_config_schema *def, అక్షరం *మూలం, స్థిరరాశి అక్షరం **mounted,
                                 size_t *n_mounted, libcrun_error_t *err)
{
  uint32_t i;

  /* Go through all mountpoints to be able to recreate missing mountpoints. */
  కొరకు (i = 0; i < def->mounts_len; i++)
    {
      అక్షరం *dest = def->mounts[i]->destination;
      అక్షరం *type = def->mounts[i]->type;
      cleanup_close పూర్ణాంకం మూలం_fd = -1;
      bool nofollow = false;
      bool on_tmpfs = false;
      పూర్ణాంకం is_dir = 1;
      size_t j;

      /* cgroup restore should be handled by CRIU itself */
      ఒకవేళ (type && (strcmp (type, "cgroup") == 0 || strcmp (type, "cgroup2") == 0))
        కొనసాగు;

      /* Check if the mountpoint is on a tmpfs. CRIU restores
       * all tmpfs. We do need to recreate directories on a tmpfs. */
      size_t dest_len = అక్షరశ్రేణి_పొడవు (dest);
      కొరకు (j = 0; j < def->mounts_len; j++)
        {
          ఒకవేళ (def->mounts[j]->type == ఖాళీ || strcmp (def->mounts[j]->type, "tmpfs") != 0)
            కొనసాగు;
          size_t mount_len = అక్షరశ్రేణి_పొడవు (def->mounts[j]->destination);
          ఒకవేళ (mount_len < dest_len && dest[mount_len] == '/' && అక్షరశ్రేణి_పోలిక (dest, def->mounts[j]->destination, mount_len) == 0)
            {
              /* This is a mountpoint which is on a tmpfs.*/
              on_tmpfs = true;
              ఆపు;
            }
        }

      ఒకవేళ (on_tmpfs)
        కొనసాగు;

      /* For bind mounts check if the source is a file or a directory. */
      ఒకవేళ (is_bind_mount (def->mounts[i], ఖాళీ, &nofollow))
        {
          ఒకవేళ (nofollow)
            తిరిగి crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          is_dir = crun_dir_p (def->mounts[i]->ఆధారం, false, err);
          ఒకవేళ (UNLIKELY (is_dir < 0))
            తిరిగి is_dir;
        }

      మూలం_fd = open (మూలం, O_RDONLY | O_CLOEXEC);
      ఒకవేళ (UNLIKELY (మూలం_fd == -1))
        తిరిగి crun_make_error (err, errno, "error opening container root directory `%s`", మూలం);

      ఒకవేళ (is_dir)
        {
          పూర్ణాంకం ret;

          ret = crun_సురక్షిత_ensure_directory_at (మూలం_fd, మూలం, dest, 0755, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }
      లేకపోతే
        {
          పూర్ణాంకం ret;

          ret = crun_సురక్షిత_ensure_file_at (మూలం_fd, మూలం, dest, 0755, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి ret;
        }

      /* Mount a bind mount source now, so that the mountpoints of the
       * mounts inside it (e.g. a nested bind mount) are created in the
       * source, where CRIU expects them.  This also happens during the
       * initial container creation, as the mounts are done in order.  */
      ఒకవేళ (is_bind_mount (def->mounts[i], ఖాళీ, &nofollow))
        {
          cleanup_close పూర్ణాంకం dst_fd = -1;
          proc_fd_మార్గం_t fd_మార్గం;

          dst_fd = సురక్షిత_openat (మూలం_fd, మూలం, dest, O_మార్గం | O_CLOEXEC, 0, err);
          ఒకవేళ (UNLIKELY (dst_fd < 0))
            తిరిగి dst_fd;

          /* Not recursive: the source might contain ROOT itself (e.g. when
           * it is the bundle directory), and it is not needed anyway.  */
          get_proc_self_fd_మార్గం (fd_మార్గం, dst_fd);
          ఒకవేళ (UNLIKELY (mount (def->mounts[i]->ఆధారం, fd_మార్గం, ఖాళీ, MS_BIND, ఖాళీ) < 0))
            తిరిగి crun_make_error (err, errno, "bind mount `%s` to `%s`", def->mounts[i]->ఆధారం, dest);

          mounted[(*n_mounted)++] = dest;
        }
    }

  తిరిగి 0;
}

/* Move the current process back to CGROUPS, as read from /proc/self/cgroup.
   A failure is not fatal, so only warn about it.  */
స్థిర శూన్య
move_వెనుక_గమ్యం_cgroups (స్థిరరాశి అక్షరం *cgroups)
{
  libcrun_error_t tmp_err = ఖాళీ;

  ఒకవేళ (UNLIKELY (libcrun_move_self_గమ్యం_cgroups (cgroups, &tmp_err) < 0))
    {
      libcrun_warning ("cannot move back to the original cgroup: %s", tmp_err->సందేశం);
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
స్థిర పూర్ణాంకం
criu_restore_బిడ్డ_లోపల_cgroup (స్థిరరాశి అక్షరం *cgroup_మార్గం, పూర్ణాంకం *criu_ret, libcrun_error_t *err)
{
  sigset_t all_సంకేతాలు, old_ముసుగు;
  పూర్ణాంకం wait_స్థితి = 0;
  pid_t pid;
  పూర్ణాంకం ret;

  *criu_ret = -1;

  sigfillset (&all_సంకేతాలు);
  ret = sigprocmask (SIG_ఖండం, &all_సంకేతాలు, &old_ముసుగు);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "sigprocmask");

  /* Must be vfork: the child shares our memory space, so both *criu_ret and
     the error it creates are visible here once it is gone.  */
  pid = vfork ();
  ఒకవేళ (UNLIKELY (pid < 0))
    {
      పూర్ణాంకం saved_errno = errno;

      sigprocmask (SIG_SETMASK, &old_ముసుగు, ఖాళీ);
      తిరిగి crun_make_error (err, saved_errno, "vfork");
    }

  ఒకవేళ (pid == 0)
    {
      నిర్మాణం sigaction act;
      పూర్ణాంకం i;

      /* The signal dispositions are not shared with the parent, so resetting
         them here does not affect it.  Keep the ignored signals ignored, as
         CRIU would inherit them if it was run directly by the caller.  */
      కొరకు (i = 1; i < NSIG; i++)
        {
          ఒకవేళ (sigaction (i, ఖాళీ, &act) < 0 || act.sa_handler == SIG_IGN || act.sa_handler == SIG_DFL)
            కొనసాగు;

          memset (&act, 0, పరిమాణం (act));
          act.sa_handler = SIG_DFL;
          sigaction (i, &act, ఖాళీ);
        }
      sigprocmask (SIG_SETMASK, &old_ముసుగు, ఖాళీ);

      ఒకవేళ (! is_ఖాళీలు_string (cgroup_మార్గం))
        {
          ret = libcrun_move_process_గమ్యం_cgroup (0, 0, cgroup_మార్గం, false, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            సురక్షిత_exit (EXIT_FAILURE);
        }

      *criu_ret = libcriu_wrapper->criu_restore_బిడ్డ ();
      సురక్షిత_exit (EXIT_SUCCESS);
    }

  /* The child is gone by now, it is safe to handle signals again.  */
  sigprocmask (SIG_SETMASK, &old_ముసుగు, ఖాళీ);

  ret = waitpid_ignore_stopped (pid, &wait_స్థితి, 0);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      /* The caller might have reaped the child already, e.g. when it ignores
         SIGCHLD.  Its result is still visible, so use it.  */
      ఒకవేళ (errno != ECHILD)
        తిరిగి crun_make_error (err, errno, "waitpid for the CRIU restore process");

      /* The child might have failed before calling CRIU, and left an error.  */
      తిరిగి *err != ఖాళీ ? -1 : 0;
    }

  ret = get_process_exit_స్థితి (wait_స్థితి);
  ఒకవేళ (UNLIKELY (ret != EXIT_SUCCESS))
    {
      /* The child creates the error in the shared memory space, do not
         overwrite it with crun_make_error().  */
      ఒకవేళ (*err == ఖాళీ)
        తిరిగి crun_make_error (err, 0, "the CRIU restore process exited with status %d", ret);
      తిరిగి -1;
    }

  తిరిగి 0;
}

/* Recreate the mountpoints which do not exist in the container rootfs
 * mounted at ROOT, the same way it is done on the container creation.  */
స్థిర పూర్ణాంకం
prepare_restore_mounts (runtime_spec_schema_config_schema *def, అక్షరం *మూలం, libcrun_error_t *err)
{
  cleanup_free స్థిరరాశి అక్షరం **mounted = xmalloc0 (పరిమాణం (అక్షరం *) * (def->mounts_len + 1));
  cleanup_close పూర్ణాంకం మూలం_fd = -1;
  size_t n_mounted = 0;
  పూర్ణాంకం ret;

  ret = prepare_restore_mounts_అంతర్గత (def, మూలం, mounted, &n_mounted, err);

  /* The bind mounts done above are only needed to create the mountpoints,
   * and CRIU restores the mounts itself, so undo them, in reverse order. */
  ఒకవేళ (n_mounted > 0)
    మూలం_fd = open (మూలం, O_మార్గం | O_CLOEXEC);
  అయితే (మూలం_fd >= 0 && n_mounted > 0)
    {
      libcrun_error_t tmp_err = ఖాళీ;
      స్థిరరాశి అక్షరం *dest = mounted[--n_mounted];
      cleanup_close పూర్ణాంకం dst_fd = -1;
      proc_fd_మార్గం_t fd_మార్గం;

      dst_fd = సురక్షిత_openat (మూలం_fd, మూలం, dest, O_మార్గం | O_CLOEXEC, 0, &tmp_err);
      ఒకవేళ (UNLIKELY (dst_fd < 0))
        {
          crun_error_release (&tmp_err);
          కొనసాగు;
        }
      get_proc_self_fd_మార్గం (fd_మార్గం, dst_fd);
      ఒకవేళ (UNLIKELY (umount2 (fd_మార్గం, MNT_DETACH) < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "unmount `%s`", dest);
    }

  తిరిగి ret;
}

పూర్ణాంకం
libcrun_container_restore_linux_criu (libcrun_container_స్థితి_t *స్థితి_2, libcrun_container_t *container,
                                      libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper నిర్మాణం libcriu_wrapper_s *wrapper = ఖాళీ;
  cleanup_close పూర్ణాంకం inherit_new_net_fd = -1;
  cleanup_close పూర్ణాంకం inherit_new_pid_fd = -1;
  cleanup_close పూర్ణాంకం image_fd = -1;
  cleanup_free అక్షరం *మూలం = ఖాళీ;
  cleanup_free అక్షరం *bundle_cleanup = ఖాళీ;
  cleanup_free అక్షరం *own_cgroups = ఖాళీ;
  cleanup_close పూర్ణాంకం work_fd = -1;
  పూర్ణాంకం cgroup_బహుళకం;
  పూర్ణాంకం criu_ret;
  పూర్ణాంకం ret_బహిర్గతం;
  size_t i;
  పూర్ణాంకం ret;

  ret = ఎక్కించు_wrapper (&wrapper, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (geteuid ())
    తిరిగి crun_make_error (err, 0, "restoring requires root");

  ret = libcriu_wrapper->criu_init_opts ();
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_సంస్కరణ (err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (UNLIKELY (cr_options->image_మార్గం == ఖాళీ))
    తిరిగి crun_make_error (err, 0, "image path not set");

  image_fd = open (cr_options->image_మార్గం, O_DIRECTORY | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (image_fd == -1))
    తిరిగి crun_make_error (err, errno, "error opening checkpoint directory `%s`", cr_options->image_మార్గం);

  libcriu_wrapper->criu_సమితి_images_dir_fd (image_fd);

  /* Load descriptors.json to tell CRIU where those FDs should be connected to. */
  {
    cleanup_free అక్షరం *descriptors_మార్గం = ఖాళీ;
    cleanup_free అక్షరం *బఫరు = ఖాళీ;
    json_object *doc = ఖాళీ;

    ret = append_paths (&descriptors_మార్గం, err, cr_options->image_మార్గం, DESCRIPTORS_FILENAME, ఖాళీ);
    ఒకవేళ (UNLIKELY (ret < 0))
      తిరిగి ret;

    ret = read_all_file (descriptors_మార్గం, &బఫరు, ఖాళీ, err);
    ఒకవేళ (UNLIKELY (ret < 0))
      తిరిగి ret;

    /* descriptors.json contains a JSON array with strings
     * telling where 0, 1 and 2 have been initially been
     * pointing to. For each descriptor which points to
     * a pipe 'pipe:' we tell CRIU to reconnect that pipe
     * to the corresponding FD to have (especially) stdout
     * and stderr being correctly redirected. */
    ret = parse_json_file (&doc, బఫరు, ఖాళీ, err);
    ఒకవేళ (UNLIKELY (ret < 0))
      తిరిగి ret;

    ఒకవేళ (json_object_is_type (doc, json_type_array))
      {
        size_t i, len = json_object_array_నిడివి (doc);

        /* len will probably always be 3 as crun is currently only
         * recording the destination of FD 0, 1 and 2. */
        కొరకు (i = 0; i < len; ++i)
          {
            json_object *s = json_object_array_get_idx (doc, i);
            ఒకవేళ (s && json_object_is_type (s, json_type_string))
              {
                స్థిరరాశి అక్షరం *str = json_object_get_string (s);
                ఒకవేళ (has_ఉపసర్గ (str, "pipe:"))
                  libcriu_wrapper->criu_కూడు_inherit_fd (i, str);
              }
          }
      }
    json_object_put (doc);
  }

  /* Our std streams are the new external descriptors, as CRIU connects the
   * inherited pipes to them.  Record them now: once the container is
   * restored it is already running, and /proc/PID/fd is under its control. */
  ret = libcrun_save_external_descriptors (container, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = అమరిక_criu_work_dir (cr_options, &work_fd, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (cr_options->lsm_profile != ఖాళీ)
    {
      ret = libcriu_wrapper->criu_సమితి_lsm_profile (cr_options->lsm_profile);
      ఒకవేళ (UNLIKELY (ret != 0))
        తిరిగి crun_make_error (err, -ret, "error setting LSM profile to `%s`", cr_options->lsm_profile);
    }

  ఒకవేళ (cr_options->lsm_mount_context != ఖాళీ)
    {
      ret = libcriu_wrapper->criu_సమితి_lsm_mount_context (cr_options->lsm_mount_context);
      ఒకవేళ (UNLIKELY (ret != 0))
        తిరిగి crun_make_error (err, -ret, "error setting LSM mount context to `%s`", cr_options->lsm_mount_context);
    }

  /* do realpath on root */
  bundle_cleanup = realpath (స్థితి_2->bundle, ఖాళీ);
  ఒకవేళ (UNLIKELY (bundle_cleanup == ఖాళీ))
    bundle_cleanup = xstrdup (స్థితి_2->bundle);

  /* Tell CRIU about external bind mounts. */
  కొరకు (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      ఒకవేళ (is_bind_mount (def->mounts[i], ఖాళీ, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          అక్షరం buf[PATH_MAX];
          స్థిరరాశి అక్షరం *dest_లోపల_మూలం;
          స్థిరరాశి అక్షరం *ఆధారం = def->mounts[i]->ఆధారం;
          cleanup_free అక్షరం *abs_ఆధారం = ఖాళీ;
          గుర్తులేని దీర్ఘ flags = 0;
          size_t j;

          ఒకవేళ (nofollow)
            తిరిగి crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_లోపల_మూలం = chroot_realpath (స్థితి_2->rootfs, def->mounts[i]->destination, buf);
          ఒకవేళ (UNLIKELY (dest_లోపల_మూలం == ఖాళీ))
            తిరిగి crun_make_error (err, errno, "unable to resolve external bind mount destination `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          ఒకవేళ (has_ఉపసర్గ (dest_లోపల_మూలం, స్థితి_2->rootfs))
            dest_లోపల_మూలం += సురక్షిత_strlen (స్థితి_2->rootfs);

          /* A relative source is relative to the bundle, while CRIU would
             resolve it relative to its own working directory.  */
          ఒకవేళ (ఆధారం && ఆధారం[0] != '/')
            {
              ret = append_paths (&abs_ఆధారం, err, bundle_cleanup, ఆధారం, ఖాళీ);
              ఒకవేళ (UNLIKELY (ret < 0))
                తిరిగి ret;
              ఆధారం = abs_ఆధారం;
            }

          ret = libcriu_wrapper->criu_కూడు_ext_mount (dest_లోపల_మూలం, ఆధారం);
          ఒకవేళ (UNLIKELY (ret < 0))
            తిరిగి crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", ఆధారం);

          కొరకు (j = 0; j < def->mounts[i]->options_len; j++)
            {
              స్థిరరాశి నిర్మాణం ప్రసరణ_flags_s *prop = ఖాళీ;

              ఒకవేళ (def->mounts[i]->options[j])
                prop = libcrun_str2mount_flags (def->mounts[i]->options[j]);
              ఒకవేళ (prop && ! prop->clear)
                flags |= prop->flags;
            }
          కూడు_restored_ext_mount (dest_లోపల_మూలం, flags);
        }
    }

  ret = రిజిస్టరు_masked_paths_mounts (def, container, libcriu_wrapper, true, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  /* Mount the container rootfs for CRIU. */
  ret = append_paths (&మూలం, err, bundle_cleanup, "criu-root", ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = mkdir (మూలం, 0755);
  ఒకవేళ (UNLIKELY (ret == -1))
    తిరిగి crun_make_error (err, errno, "error creating restore directory `%s`", మూలం);

  ret = mount (స్థితి_2->rootfs, మూలం, ఖాళీ, MS_BIND | MS_REC, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret == -1))
    {
      ret = crun_make_error (err, errno, "error mounting restore directory `%s`", మూలం);
      వెళ్ళు బహిర్గతం;
    }

  /* During initial container creation, crun will create mountpoints
   * defined in config.json if they do not exist. If we are restoring
   * we need to make sure these mountpoints also exist.
   * This is not perfect, as this means crun will modify a rootfs
   * even if it marked as read-only, but runc already modifies
   * the rootfs in the same way. */

  ret = prepare_restore_mounts (def, మూలం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు బహిర్గతం_umount;

  ret = libcriu_wrapper->criu_సమితి_మూలం (మూలం);
  ఒకవేళ (UNLIKELY (ret != 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU root to `%s`", మూలం);
      వెళ్ళు బహిర్గతం_umount;
    }

  /* If a namespace defined in config.json we are telling
   * CRIU use that namespace when restoring the process tree.
   *
   * CRIU expects the information about the namespace like this:
   * --inherit-fd fd[<fd>]:<key>
   * The <key> needs to be the same as during checkpointing (extRootNetNS). */
  కొరకు (i = 0; i < def->linux->namespaces_len; i++)
    {
      స్థిరరాశి పూర్ణాంకం open_flags_for_inherit = O_RDONLY; /* Cannot be O_CLOEXEC as it is passed to the child process. */
      పూర్ణాంకం విలువ = libcrun_కనుగొను_namespace (def->linux->namespaces[i]->type);
      ఒకవేళ (UNLIKELY (విలువ < 0))
        {
          ret = crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);
          వెళ్ళు బహిర్గతం_umount;
        }

      ఒకవేళ (విలువ == CLONE_NEWNET && def->linux->namespaces[i]->మార్గం != ఖాళీ)
        {
          inherit_new_net_fd = open (def->linux->namespaces[i]->మార్గం, open_flags_for_inherit);
          ఒకవేళ (UNLIKELY (inherit_new_net_fd < 0))
            {
              ret = crun_make_error (err, errno, "unable to open(): `%s`", def->linux->namespaces[i]->మార్గం);
              వెళ్ళు బహిర్గతం_umount;
            }

          ret = libcriu_wrapper->criu_కూడు_inherit_fd (inherit_new_net_fd, CRIU_EXT_NETNS);
          ఒకవేళ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              వెళ్ళు బహిర్గతం_umount;
            }
        }

      ఒకవేళ (విలువ == CLONE_NEWPID && def->linux->namespaces[i]->మార్గం != ఖాళీ)
        {
          inherit_new_pid_fd = open (def->linux->namespaces[i]->మార్గం, open_flags_for_inherit);
          ఒకవేళ (UNLIKELY (inherit_new_pid_fd < 0))
            {
              ret = crun_make_error (err, errno, "unable to open(): `%s`", def->linux->namespaces[i]->మార్గం);
              వెళ్ళు బహిర్గతం_umount;
            }

          ret = libcriu_wrapper->criu_కూడు_inherit_fd (inherit_new_pid_fd, CRIU_EXT_PIDNS);
          ఒకవేళ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              వెళ్ళు బహిర్గతం_umount;
            }
        }

#  ifdef CRIU_JOIN_NS_SUPPORT
      ఒకవేళ (విలువ == CLONE_NEWTIME && def->linux->namespaces[i]->మార్గం != ఖాళీ)
        {
          ఒకవేళ (libcriu_wrapper->criu_join_ns_కూడు == ఖాళీ)
            {
              ret = crun_make_error (err, 0, "shared time namespace restore is supported in CRIU >= 3.16.1");
              వెళ్ళు బహిర్గతం_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_కూడు ("time", def->linux->namespaces[i]->మార్గం, ఖాళీ);
          ఒకవేళ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->మార్గం);
              వెళ్ళు బహిర్గతం_umount;
            }
        }

      ఒకవేళ (విలువ == CLONE_NEWIPC && def->linux->namespaces[i]->మార్గం != ఖాళీ)
        {
          ఒకవేళ (libcriu_wrapper->criu_join_ns_కూడు == ఖాళీ)
            {
              ret = crun_make_error (err, 0, "shared ipc namespace restore is supported in CRIU >= 3.16.1");
              వెళ్ళు బహిర్గతం_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_కూడు ("ipc", def->linux->namespaces[i]->మార్గం, ఖాళీ);
          ఒకవేళ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->మార్గం);
              వెళ్ళు బహిర్గతం_umount;
            }
        }

      ఒకవేళ (విలువ == CLONE_NEWUTS && def->linux->namespaces[i]->మార్గం != ఖాళీ)
        {
          ఒకవేళ (libcriu_wrapper->criu_join_ns_కూడు == ఖాళీ)
            {
              ret = crun_make_error (err, 0, "shared uts namespace restore is supported in CRIU >= 3.16.1");
              వెళ్ళు బహిర్గతం_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_కూడు ("uts", def->linux->namespaces[i]->మార్గం, ఖాళీ);
          ఒకవేళ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->మార్గం);
              వెళ్ళు బహిర్గతం_umount;
            }
        }
#  endif
    }

  /* Set up CRIU config file */
  ret = handle_criu_config_file (container, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు బహిర్గతం_umount;

  /* Tell CRIU if cgroup v1 needs to be handled. */
  ret = restore_cgroup_v1_mount (def, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    వెళ్ళు బహిర్గతం_umount;

  console_socket = cr_options->console_socket;
  libcriu_wrapper->criu_సమితి_notify_cb (criu_notify);

  /* Set boolean options . */
  libcriu_wrapper->criu_సమితి_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_సమితి_shell_పని (cr_options->shell_పని);
  libcriu_wrapper->criu_సమితి_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_సమితి_tcp_close (cr_options->tcp_close);
  libcriu_wrapper->criu_సమితి_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_సమితి_orphan_pts_master (true);

  ఒకవేళ (స్థితి_2->cgroup_మార్గం)
    {
      cgroup_బహుళకం = libcrun_get_cgroup_బహుళకం (err);
      ఒకవేళ (UNLIKELY (cgroup_బహుళకం < 0))
        {
          ret = cgroup_బహుళకం;
          వెళ్ళు బహిర్గతం_umount;
        }

      /* With cgroup v2, the cgroup path is only meaningful for the unified
         hierarchy, so only relocate that one.  A NULL controller would make
         CRIU relocate every hierarchy the container is in, named cgroup v1
         ones included.  As CRIU dumps such a hierarchy from wherever the
         container is in it, which is its root, as crun leaves named
         hierarchies alone, every checkpoint and restore would then copy the
         whole hierarchy into itself.  */
      ret = libcriu_wrapper->criu_కూడు_cg_మూలం (cgroup_బహుళకం == CGROUP_బహుళకం_UNIFIED ? "" : ఖాళీ,
                                               స్థితి_2->cgroup_మార్గం);
      ఒకవేళ (UNLIKELY (ret != 0))
        {
          ret = crun_make_error (err, 0, "error setting CRIU cgroup root to `%s`", స్థితి_2->cgroup_మార్గం);
          వెళ్ళు బహిర్గతం_umount;
        }
    }

  libcriu_wrapper->criu_సమితి_manage_cgroups_బహుళకం (criu_cg_బహుళకం_from_ప్రజా (cr_options->manage_cgroups_బహుళకం));
  libcriu_wrapper->criu_సమితి_manage_cgroups (true);

  ఒకవేళ (libcriu_wrapper->criu_సమితి_జాలం_తాళం)
    {
      పూర్ణాంకం తాళం = 0;

      ret = criu_జాలం_తాళం_from_ప్రజా (cr_options->జాలం_తాళం_పద్ధతి, &తాళం, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        వెళ్ళు బహిర్గతం_umount;

      ఒకవేళ (తాళం > 0)
        {
          ret = libcriu_wrapper->criu_సమితి_జాలం_తాళం (తాళం);
          ఒకవేళ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "CRIU: failed setting network lock");
              వెళ్ళు బహిర్గతం_umount;
            }
        }
    }

  libcriu_wrapper->criu_సమితి_log_level (4);
  ret = libcriu_wrapper->criu_సమితి_log_file (CRIU_RESTORE_LOG_FILE);
  ఒకవేళ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU log file to `%s`", CRIU_RESTORE_LOG_FILE);
      వెళ్ళు బహిర్గతం_umount;
    }

  /* criu_restore() returns the PID of the process of the restored process
   * tree. This PID will not be the same as status->pid if the container is
   * running in a PID namespace. But it will always be > 0. */
  ఒకవేళ (cr_options->detach)
    {
      /* Nothing waits for the container, so there is no reason to take over
         this process: use a throw-away one.  It is used even when there is no
         cgroup to join, so that the container is not left as a child of a
         process which is not going to reap it.  */
      ret = criu_restore_బిడ్డ_లోపల_cgroup (స్థితి_2->cgroup_మార్గం, &criu_ret, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        వెళ్ళు బహిర్గతం_umount;

      ret = criu_ret;
    }
  లేకపోతే
    {
      /* Like runc does, put CRIU into the container cgroup for the time of
       * restore, so the restored tasks are created there. It is necessary in
       * the "ignore" mode, where CRIU does not deal with cgroups at all, and
       * does not hurt otherwise. As CRIU is our child, do it by moving
       * ourselves there, and move back once the restore is done. */
      ఒకవేళ (! is_ఖాళీలు_string (స్థితి_2->cgroup_మార్గం))
        {
          ret = read_all_file (PROC_SELF_CGROUP, &own_cgroups, ఖాళీ, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            వెళ్ళు బహిర్గతం_umount;

          ret = libcrun_move_process_గమ్యం_cgroup (0, 0, స్థితి_2->cgroup_మార్గం, false, err);
          ఒకవేళ (UNLIKELY (ret < 0))
            {
              /* Some of the cgroups might have been joined already.  */
              move_వెనుక_గమ్యం_cgroups (own_cgroups);
              వెళ్ళు బహిర్గతం_umount;
            }
        }

      ret = libcriu_wrapper->criu_restore_బిడ్డ ();

      ఒకవేళ (own_cgroups)
        move_వెనుక_గమ్యం_cgroups (own_cgroups);
    }

  ఒకవేళ (UNLIKELY (ret <= 0))
    {
      show_criu_log (cr_options->work_మార్గం, CRIU_RESTORE_LOG_FILE);
      ret = crun_make_error (err, 0, "CRIU restoring failed: %d", ret);
      వెళ్ళు బహిర్గతం_umount;
    }

  /* Update the status struct with the newly allocated PID. This will
   * be necessary later when moving the process into its cgroup. */
  స్థితి_2->pid = ret;
  ret = 0;

బహిర్గతం_umount:
  ret_బహిర్గతం = umount (మూలం);
  ఒకవేళ (UNLIKELY (ret_బహిర్గతం == -1))
    {
      పూర్ణాంకం saved_errno = errno;
      rmdir (మూలం);
      ఒకవేళ (ret < 0)
        తిరిగి crun_error_wrap (err, "error unmounting restore directory `%s`", మూలం);
      లేకపోతే
        తిరిగి crun_make_error (err, saved_errno, "error unmounting restore directory `%s`", మూలం);
    }
బహిర్గతం:
  ret_బహిర్గతం = rmdir (మూలం);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;
  ఒకవేళ (UNLIKELY (ret_బహిర్గతం == -1))
    తిరిగి crun_make_error (err, errno, "error removing restore directory `%s`", మూలం);
  తిరిగి ret;
}
#అయితే_ముగింపు
