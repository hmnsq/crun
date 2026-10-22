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

#অন্তর্ভাৱ <config.h>

#যদি HAVE_CRIU && HAVE_DLOPEN

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
ৱর্ণ *chroot_realpath (ধ্রুৱ ৱর্ণ *chroot, ধ্রুৱ ৱর্ণ *path, ৱর্ণ resolved_path[]);

/* Map the public cgroups mode (enum libcrun_cr_cgroups_mode) onto the CRIU enum.  */
স্থির গণনা criu_cg_mode
criu_cg_mode_from_public (পূর্ণাঙ্ক mode)
{
  ৱিকল্প (mode)
    {
    পক্ষ LIBCRUN_CR_CG_MODE_IGNORE:
      ঘূৰাই CRIU_CG_MODE_IGNORE;
    পক্ষ LIBCRUN_CR_CG_MODE_FULL:
      ঘূৰাই CRIU_CG_MODE_FULL;
    পক্ষ LIBCRUN_CR_CG_MODE_STRICT:
      ঘূৰাই CRIU_CG_MODE_STRICT;
    পক্ষ LIBCRUN_CR_CG_MODE_DEFAULT:
    পক্ষ LIBCRUN_CR_CG_MODE_SOFT:
    স্ৱভাৱ:
      /* Default to CRIU_CG_MODE_SOFT, just as runc.  */
      ঘূৰাই CRIU_CG_MODE_SOFT;
    }
}

/* Map the public network lock method (enum libcrun_cr_network_lock_method) onto the
   CRIU enum.  *LOCK is set to 0 when no explicit method must be set (CRIU default).
   A method that this build cannot honor is an error, never a silent fallback to
   the CRIU default.  */
স্থির পূর্ণাঙ্ক
criu_network_lock_from_public (পূর্ণাঙ্ক method, পূর্ণাঙ্ক *lock, libcrun_error_t *err)
{
  ৱিকল্প (method)
    {
    পক্ষ LIBCRUN_CR_NETWORK_LOCK_IPTABLES:
      *lock = CRIU_NETWORK_LOCK_IPTABLES;
      ঘূৰাই 0;
    পক্ষ LIBCRUN_CR_NETWORK_LOCK_NFTABLES:
      *lock = CRIU_NETWORK_LOCK_NFTABLES;
      ঘূৰাই 0;
    পক্ষ LIBCRUN_CR_NETWORK_LOCK_SKIP:
#  যদি CRIU_NETWORK_LOCK_SKIP_SUPPORT
      *lock = CRIU_NETWORK_LOCK_SKIP;
      ঘূৰাই 0;
#  অন্যথা
      ঘূৰাই crun_make_error (err, 0, "CRIU: the `skip` network lock method is not supported by this build");
#  endif
    পক্ষ LIBCRUN_CR_NETWORK_LOCK_DEFAULT:
      *lock = 0;
      ঘূৰাই 0;
    স্ৱভাৱ:
      ঘূৰাই crun_make_error (err, 0, "CRIU: unknown network lock method %d", method);
    }
}

স্থির ধ্রুৱ ৱর্ণ *console_socket = রিক্ত;

/* The external mounts CRIU restores, and the flags the configuration asks
   for each of them.  */
স্থির সংরচনা libcrun_mount_flags_s *restored_ext_mounts = রিক্ত;
স্থির size_t n_restored_ext_mounts = 0;

#  define LIBCRIU_MIN_VERSION 31500

সংরচনা libcriu_wrapper_s
{
  শূন্য *handle;
  পূর্ণাঙ্ক (*criu_add_ext_mount) (ধ্রুৱ ৱর্ণ *চাবি, ধ্রুৱ ৱর্ণ *val);
  পূর্ণাঙ্ক (*criu_add_external) (ধ্রুৱ ৱর্ণ *চাবি);
  পূর্ণাঙ্ক (*criu_add_inherit_fd) (পূর্ণাঙ্ক fd, ধ্রুৱ ৱর্ণ *চাবি);
  পূর্ণাঙ্ক (*criu_check_version) (পূর্ণাঙ্ক minimum);
  পূর্ণাঙ্ক (*criu_dump) (শূন্য);
  পূর্ণাঙ্ক (*criu_get_orphan_pts_master_fd) (শূন্য);
  পূর্ণাঙ্ক (*criu_init_opts) (শূন্য);
  পূর্ণাঙ্ক (*criu_notify_pid) (criu_notify_arg_t na);
#  ifdef CRIU_JOIN_NS_SUPPORT
  পূর্ণাঙ্ক (*criu_join_ns_add) (ধ্রুৱ ৱর্ণ *ns, ধ্রুৱ ৱর্ণ *ns_file, ধ্রুৱ ৱর্ণ *extra_opt);
#  endif
#  ifdef CRIU_PRE_DUMP_SUPPORT
  পূর্ণাঙ্ক (*criu_feature_check) (সংরচনা criu_feature_check *features, size_t size);
  পূর্ণাঙ্ক (*criu_pre_dump) (শূন্য);
#  endif
  পূর্ণাঙ্ক (*criu_restore_child) (শূন্য);
  পূর্ণাঙ্ক (*criu_set_freeze_cgroup) (ধ্রুৱ ৱর্ণ *name);
  শূন্য (*criu_set_file_locks) (bool file_locks);
  শূন্য (*criu_set_ext_unix_sk) (bool ext_unix_sk);
  পূর্ণাঙ্ক (*criu_set_log_file) (ধ্রুৱ ৱর্ণ *log_file);
  শূন্য (*criu_set_log_level) (পূর্ণাঙ্ক log_level);
  শূন্য (*criu_set_leave_running) (bool leave_running);
  শূন্য (*criu_set_manage_cgroups) (bool manage);
  শূন্য (*criu_set_manage_cgroups_mode) (গণনা criu_cg_mode mode);
  পূর্ণাঙ্ক (*criu_set_network_lock) (গণনা criu_network_lock_method method);
  শূন্য (*criu_set_notify_cb) (পূর্ণাঙ্ক (*cb) (ৱর্ণ *action, criu_notify_arg_t na));
  শূন্য (*criu_set_orphan_pts_master) (bool orphan_pts_master);
  শূন্য (*criu_set_images_dir_fd) (পূর্ণাঙ্ক fd);
  পূর্ণাঙ্ক (*criu_set_parent_images) (ধ্রুৱ ৱর্ণ *path);
  শূন্য (*criu_set_pid) (পূর্ণাঙ্ক pid);
  পূর্ণাঙ্ক (*criu_set_মূল) (ধ্রুৱ ৱর্ণ *মূল);
  পূর্ণাঙ্ক (*criu_add_cg_মূল) (ধ্রুৱ ৱর্ণ *ctrl, ধ্রুৱ ৱর্ণ *path);
  শূন্য (*criu_set_shell_job) (bool shell_job);
  শূন্য (*criu_set_tcp_established) (bool tcp_established);
  শূন্য (*criu_set_tcp_close) (bool tcp_close);
  শূন্য (*criu_set_track_mem) (bool track_mem);
  শূন্য (*criu_set_work_dir_fd) (পূর্ণাঙ্ক fd);
  পূর্ণাঙ্ক (*criu_set_lsm_profile) (ধ্রুৱ ৱর্ণ *name);
  পূর্ণাঙ্ক (*criu_set_lsm_mount_context) (ধ্রুৱ ৱর্ণ *name);
  পূর্ণাঙ্ক (*criu_set_config_file) (ধ্রুৱ ৱর্ণ *path);
};

স্থির সংরচনা libcriu_wrapper_s *libcriu_wrapper;

স্থির অন্তঃস্থ শূন্য
cleanup_wrapper (শূন্য *p)
{
  সংরচনা libcriu_wrapper_s **w;

  w = (সংরচনা libcriu_wrapper_s **) p;
  যদি (*w == রিক্ত)
    ঘূৰাই;

#  ifndef STATIC
  যদি ((*w)->handle)
    dlclose ((*w)->handle);
#  endif
  মুক্ত (*w);
  libcriu_wrapper = রিক্ত;

  যেতিয়ালৈ (n_restored_ext_mounts > 0)
    মুক্ত (restored_ext_mounts[--n_restored_ext_mounts].target);
  মুক্ত (restored_ext_mounts);
  restored_ext_mounts = রিক্ত;
}

#  define cleanup_wrapper __attribute__ ((cleanup (cleanup_wrapper)))

স্থির পূর্ণাঙ্ক
load_wrapper (সংরচনা libcriu_wrapper_s **wrapper_out, libcrun_error_t *err)
{
  cleanup_free সংরচনা libcriu_wrapper_s *wrapper = xmalloc0 (পরিমাণ (*wrapper));

#  ifdef STATIC
#    define LOAD_CRIU_FUNCTION(X, ALLOW_NULL) \
      wrapper->X = &X;
#  অন্যথা
#    define LOAD_CRIU_FUNCTION(X, ALLOW_NULL)                                                    \
      কৰা                                                                                         \
        {                                                                                        \
          wrapper->X = dlsym (wrapper->handle, #X);                                              \
          যদি (! ALLOW_NULL && wrapper->X == রিক্ত)                                                \
            {                                                                                    \
              dlclose (wrapper->handle);                                                         \
              ঘূৰাই crun_make_error (err, 0, "could not find symbol `%s` in `libcriu.so`", #X); \
            }                                                                                    \
      } যেতিয়ালৈ (0)
#  endif

#  ifndef STATIC
  wrapper->handle = dlopen ("libcriu.so.2", RTLD_NOW);
  যদি (wrapper->handle == রিক্ত)
    ঘূৰাই crun_make_error (err, 0, "could not load `libcriu.so.2`: `%s`", dlerror ());
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
  LOAD_CRIU_FUNCTION (criu_set_মূল, false);
  LOAD_CRIU_FUNCTION (criu_add_cg_মূল, false);
  LOAD_CRIU_FUNCTION (criu_set_shell_job, false);
  LOAD_CRIU_FUNCTION (criu_set_tcp_established, false);
  LOAD_CRIU_FUNCTION (criu_set_tcp_close, false);
  LOAD_CRIU_FUNCTION (criu_set_track_mem, false);
  LOAD_CRIU_FUNCTION (criu_set_work_dir_fd, false);
  LOAD_CRIU_FUNCTION (criu_set_lsm_profile, false);
  LOAD_CRIU_FUNCTION (criu_set_lsm_mount_context, false);
#  যদি ! defined STATIC || defined CRIU_CONFIG_FILE
  LOAD_CRIU_FUNCTION (criu_set_config_file, true);
#  endif

  libcriu_wrapper = *wrapper_out = wrapper;
  wrapper = রিক্ত;
#  undef LOAD_CRIU_FUNCTION
  ঘূৰাই 0;
}

স্থির শূন্য
add_restored_ext_mount (ধ্রুৱ ৱর্ণ *target, unsigned long flags)
{
  restored_ext_mounts = xrealloc (restored_ext_mounts, পরিমাণ (*restored_ext_mounts) * (n_restored_ext_mounts + 1));
  restored_ext_mounts[n_restored_ext_mounts].target = xstrdup (target);
  restored_ext_mounts[n_restored_ext_mounts].flags = flags;
  n_restored_ext_mounts++;
}

স্থির পূর্ণাঙ্ক
criu_notify (ৱর্ণ *action, criu_notify_arg_t na)
{
  যদি (strcmp (action, "post-restore") == 0)
    {
      /* CRIU takes the source of an external mount from us and its flags
       * from the image, i.e. from whatever mount was at that path in the
       * container when it was checkpointed.  The container can move its
       * mount points around, so before it is resumed make sure that each
       * source is mounted with the restrictions the configuration asks for. */
      libcrun_error_t tmp_err = রিক্ত;
      পূর্ণাঙ্ক ret;

      ret = libcrun_restrict_mounts_in_container (libcriu_wrapper->criu_notify_pid (na), restored_ext_mounts,
                                                  n_restored_ext_mounts, &tmp_err);
      যদি (UNLIKELY (ret < 0))
        {
          libcrun_error (tmp_err->status, "cannot set the flags of the restored mounts: %s", tmp_err->বাৰ্তা);
          libcrun_error_release (&tmp_err);
          ঘূৰাই ret;
        }
    }

  যদি (strncmp (action, "orphan-pts-master", 17) == 0)
    {
      /* CRIU sends us the master FD via the 'orphan-pts-master'
       * callback and we are passing it on to the '--console-socket'
       * if it exists. */
      cleanup_close পূর্ণাঙ্ক console_socket_fd = -1;
      libcrun_error_t tmp_err = রিক্ত;
      পূর্ণাঙ্ক master_fd;
      পূর্ণাঙ্ক ret;

      যদি (! console_socket)
        ঘূৰাই 0;

      master_fd = libcriu_wrapper->criu_get_orphan_pts_master_fd ();

      console_socket_fd = open_unix_domain_client_socket (console_socket, 0, &tmp_err);
      যদি (UNLIKELY (console_socket_fd < 0))
        {
          libcrun_error_release (&tmp_err);
          ঘূৰাই console_socket_fd;
        }
      ret = send_fd_to_socket (console_socket_fd, master_fd, &tmp_err);
      যদি (UNLIKELY (ret < 0))
        {
          libcrun_error_release (&tmp_err);
          ঘূৰাই ret;
        }
    }
  ঘূৰাই 0;
}

#  ifdef CRIU_PRE_DUMP_SUPPORT

স্থির পূর্ণাঙ্ক
criu_check_mem_track (libcrun_error_t *err)
{
  সংরচনা criu_feature_check features = { 0 };
  পূর্ণাঙ্ক ret;

  /* Right now we are only interested in checking memory tracking.
   * Memory tracking can be disabled at different levels. aarch64
   * for example has memory tracking not implemented. It could also
   * be not enabled on other architectures. Just ask CRIU if that
   * features exists. */

  features.mem_track = true;

  ret = libcriu_wrapper->criu_feature_check (&features, পরিমাণ (features));
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, 0, "CRIU feature checking failed: %d", ret);

  যদি (features.mem_track == true)
    ঘূৰাই 1;

  ঘূৰাই crun_make_error (err, 0, "CRIU memory tracking not supported");
}

#  endif

স্থির পূর্ণাঙ্ক
register_masked_paths_mounts (runtime_spec_schema_config_schema *def, libcrun_container_t *container,
                              সংরচনা libcriu_wrapper_s *libcriu_wrapper, bool is_restore, libcrun_error_t *err)
{
  cleanup_free ৱর্ণ *empty_dir_path = রিক্ত;
  bool shared_dir_registered = false;
  size_t i;
  পূর্ণাঙ্ক ret;

  বাবে (i = 0; i < def->linux->masked_paths_len; i++)
    {
      সংরচনা stat statbuf;
      ret = stat (def->linux->masked_paths[i], &statbuf);
      যদি (ret != 0)
        অনুৱর্ত;

      যদি (S_ISDIR (statbuf.st_mode))
        {
          যদি (! shared_dir_registered)
            {
              ret = get_shared_empty_directory_path (&empty_dir_path,
                                                     (container->context ? container->context->state_মূল : রিক্ত), err);
              যদি (UNLIKELY (ret < 0))
                ঘূৰাই ret;

              ret = libcriu_wrapper->criu_add_ext_mount (empty_dir_path, empty_dir_path);
              যদি (UNLIKELY (ret < 0))
                ঘূৰাই crun_make_error (err, -ret, "CRIU: failed adding external mount for shared empty directory `%s`", empty_dir_path);

              shared_dir_registered = true;
            }

          ret = libcriu_wrapper->criu_add_ext_mount (def->linux->masked_paths[i], empty_dir_path);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, -ret, "CRIU: failed adding external mount for masked directory `%s`", def->linux->masked_paths[i]);

          যদি (is_restore)
            add_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
      অন্যথা যদি (S_ISREG (statbuf.st_mode))
        {
          ধ্রুৱ ৱর্ণ *bind_target = is_restore ? "/dev/null" : def->linux->masked_paths[i];
          ret = libcriu_wrapper->criu_add_ext_mount (def->linux->masked_paths[i], bind_target);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", bind_target);

          যদি (is_restore)
            add_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
    }

  ঘূৰাই 0;
}

/* Parse one /proc/self/cgroup LINE into its subsystem, normalizing the name and
   stripping any "name=" prefix.  When SUBPATH_OUT is not NULL, the in-hierarchy
   path is stored there.  Returns the subsystem, or NULL if the line carries no
   controller and should be skipped.  LINE is modified in place.  */
স্থির ৱর্ণ *
parse_cgroup_subsystem (ৱর্ণ *সাৰি, ৱর্ণ **subpath_out)
{
  ৱর্ণ *subsystem, *subpath, *it;

  subsystem = strchr (সাৰি, ':') + 1;
  subpath = strchr (subsystem, ':') + 1;
  *(subpath - 1) = '\0';

  যদি (subsystem[0] == '\0')
    ঘূৰাই রিক্ত;

  it = strstr (subsystem, "name=");
  যদি (it)
    subsystem = it + 5;

  যদি (strcmp (subsystem, "net_prio,net_cls") == 0)
    subsystem = "net_cls,net_prio";
  যদি (strcmp (subsystem, "cpuacct,cpu") == 0)
    subsystem = "cpu,cpuacct";

  যদি (subpath_out)
    *subpath_out = subpath;
  ঘূৰাই subsystem;
}

স্থির পূর্ণাঙ্ক
restore_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free ৱর্ণ *content = রিক্ত;
  bool has_cgroup_mount = false;
  ৱর্ণ *saveptr = রিক্ত;
  পূর্ণাঙ্ক cgroup_mode;
  ৱর্ণ *from;
  পূর্ণাঙ্ক ret;
  uint32_t i;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    ঘূৰাই cgroup_mode;

  যদি (cgroup_mode == CGROUP_MODE_UNIFIED)
    ঘূৰাই 0;

  /* First check if there is actually a cgroup mount in the container. */
  বাবে (i = 0; i < def->mounts_len; i++)
    {
      ৱর্ণ *type = def->mounts[i]->type;
      যদি (type && strcmp (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          ভঙ্গ;
        }
    }

  যদি (! has_cgroup_mount)
    ঘূৰাই 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, রিক্ত, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  যদি (UNLIKELY (content == রিক্ত || content[0] == '\0'))
    ঘূৰাই crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  বাবে (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (রিক্ত, "\n", &saveptr))
    {
      cleanup_free ৱর্ণ *destination = রিক্ত;
      cleanup_free ৱর্ণ *source = রিক্ত;
      ৱর্ণ *subsystem;
      ৱর্ণ *subpath;

      subsystem = parse_cgroup_subsystem (from, &subpath);
      যদি (subsystem == রিক্ত)
        অনুৱর্ত;

      ret = append_paths (&source, err, CGROUP_মূল, subsystem, রিক্ত);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = append_paths (&destination, err, source, subpath, রিক্ত);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = libcriu_wrapper->criu_add_ext_mount (source, destination);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", destination);
    }

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
checkpoint_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free ৱর্ণ *content = রিক্ত;
  bool has_cgroup_mount = false;
  ৱর্ণ *saveptr = রিক্ত;
  ৱর্ণ *from;
  পূর্ণাঙ্ক ret;
  uint32_t i;

  /* First check if there is actually a cgroup mount in the container. */
  বাবে (i = 0; i < def->mounts_len; i++)
    {
      ৱর্ণ *type = def->mounts[i]->type;
      যদি (type && strcmp (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          ভঙ্গ;
        }
    }

  যদি (! has_cgroup_mount)
    ঘূৰাই 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, রিক্ত, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  যদি (UNLIKELY (content == রিক্ত || content[0] == '\0'))
    ঘূৰাই crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  বাবে (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (রিক্ত, "\n", &saveptr))
    {
      cleanup_free ৱর্ণ *source_path = রিক্ত;
      ৱর্ণ *subsystem;

      subsystem = parse_cgroup_subsystem (from, রিক্ত);
      যদি (subsystem == রিক্ত)
        অনুৱর্ত;

      ret = append_paths (&source_path, err, CGROUP_মূল, subsystem, রিক্ত);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = libcriu_wrapper->criu_add_ext_mount (source_path, source_path);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", source_path);
    }

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
handle_criu_config_file (libcrun_container_t *container, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;
  ধ্রুৱ ৱর্ণ *criu_config_annotation;
  ধ্রুৱ ৱর্ণ *config_file = CRIU_RUNC_CONFIG_FILE;

  criu_config_annotation = find_annotation (container, "org.criu.config");

  /* Ignore missing criu_set_config_file() API for compatibility with older CRIU versions,
   * and show an error only if config file is explicitly set with annotation.
   */
  যদি (libcriu_wrapper->criu_set_config_file == রিক্ত)
    {
      যদি (criu_config_annotation)
        ঘূৰাই crun_make_error (err, 0, "libcriu RPC config files supported in CRIU >= 4.2");

      ঘূৰাই 0;
    }

  যদি (criu_config_annotation)
    config_file = criu_config_annotation;
  অন্যথা যদি (access (CRIU_CRUN_CONFIG_FILE, F_OK) == 0)
    config_file = CRIU_CRUN_CONFIG_FILE;

  ret = libcriu_wrapper->criu_set_config_file (config_file);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, 0, "failed to set CRIU config file");

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
validate_criu_version (libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  // validate that the libcriu version is at least LIBCRIU_MIN_VERSION
  ret = libcriu_wrapper->criu_check_version (LIBCRIU_MIN_VERSION);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, -ret, "CRIU: failed checking version");

  যদি (ret == 0)
    ঘূৰাই crun_make_error (err, 0, "libcriu is too old");

  ঘূৰাই 0;
}

স্থির শূন্য
show_criu_log (ধ্রুৱ ৱর্ণ *work_path, ধ্রুৱ ৱর্ণ *log)
{
  cleanup_free ৱর্ণ *log_path = রিক্ত;
  libcrun_error_t tmp_err = রিক্ত;
  ৱর্ণ সাৰি[1024];
  FILE *f;

  যদি (UNLIKELY (append_paths (&log_path, &tmp_err, work_path, log, রিক্ত) < 0))
    {
      crun_error_release (&tmp_err);
      ঘূৰাই;
    }

  f = fopen (log_path, "r");
  যদি (f == রিক্ত)
    {
      যদি (errno != ENOENT)
        libcrun_error (errno, "Can't open CRIU log `%s`", log_path);
      ঘূৰাই;
    }

  /* Log with error verbosity as this is the default. */
  libcrun_error (0, "--- excerpt from CRIU log `%s`", log_path);
  যেতিয়ালৈ (fgets (সাৰি, পরিমাণ (সাৰি), f) != রিক্ত)
    যদি (strstr (সাৰি, "Error ") != রিক্ত)
      {
        সাৰি[strcspn (সাৰি, "\n")] = '\0';
        libcrun_error (0, "%s", সাৰি);
      }

  fclose (f);
  libcrun_error (0, "--- end of excerpt");
}

/* work_dir is the place CRIU will put its logfiles.  If not explicitly set, CRIU
 * will put the logfiles into the images_dir.  When set, create and open it and
 * hand the fd to CRIU; ownership of the returned fd stays with the caller.  */
স্থির পূর্ণাঙ্ক
setup_criu_work_dir (libcrun_checkpoint_restore_t *cr_options, পূর্ণাঙ্ক *work_fd_out, libcrun_error_t *err)
{
  পূর্ণাঙ্ক work_fd;

  যদি (cr_options->work_path == রিক্ত)
    {
      /* This is only for the error message later. */
      cr_options->work_path = cr_options->image_path;
      ঘূৰাই 0;
    }

  যদি (UNLIKELY ((mkdir (cr_options->work_path, 0700) == -1) && (errno != EEXIST)))
    ঘূৰাই crun_make_error (err, errno, "error creating CRIU work directory `%s`", cr_options->work_path);

  work_fd = open (cr_options->work_path, O_DIRECTORY | O_CLOEXEC);
  যদি (UNLIKELY (work_fd == -1))
    ঘূৰাই crun_make_error (err, errno, "error opening CRIU work directory `%s`", cr_options->work_path);

  libcriu_wrapper->criu_set_work_dir_fd (work_fd);
  *work_fd_out = work_fd;
  ঘূৰাই 0;
}

পূর্ণাঙ্ক
libcrun_container_checkpoint_linux_criu (libcrun_container_status_t *status, libcrun_container_t *container,
                                         libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper সংরচনা libcriu_wrapper_s *wrapper = রিক্ত;
  cleanup_free ৱর্ণ *descriptors_path = রিক্ত;
  cleanup_free ৱর্ণ *freezer_path = রিক্ত;
  cleanup_free ৱর্ণ *path = রিক্ত;
  cleanup_close পূর্ণাঙ্ক image_fd = -1;
  cleanup_close পূর্ণাঙ্ক work_fd = -1;
  পূর্ণাঙ্ক cgroup_mode;
  size_t i;
  পূর্ণাঙ্ক ret;

  ret = load_wrapper (&wrapper, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  যদি (geteuid ())
    ঘূৰাই crun_make_error (err, 0, "checkpointing requires root");

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
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_version (err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  যদি (UNLIKELY (cr_options->image_path == রিক্ত))
    ঘূৰাই crun_make_error (err, 0, "image path not set");

  ret = mkdir (cr_options->image_path, 0700);
  যদি (UNLIKELY ((ret == -1) && (errno != EEXIST)))
    ঘূৰাই crun_make_error (err, errno, "error creating checkpoint directory `%s`", cr_options->image_path);

  image_fd = open (cr_options->image_path, O_DIRECTORY | O_CLOEXEC);
  যদি (UNLIKELY (image_fd == -1))
    ঘূৰাই crun_make_error (err, errno, "error opening checkpoint directory `%s`", cr_options->image_path);

  libcriu_wrapper->criu_set_images_dir_fd (image_fd);

  /* Set up logging. */
  libcriu_wrapper->criu_set_log_level (4);
  libcriu_wrapper->criu_set_log_file (CRIU_CHECKPOINT_LOG_FILE);

  /* Set up CRIU config file */
  যদি (UNLIKELY (handle_criu_config_file (container, err)))
    ঘূৰাই -1;

  /* Setting the pid early as we can skip a lot of checkpoint setup if
   * we just do a pre-dump. The PID needs to be set always. Do it here.
   * The main process of the container is the process CRIU will checkpoint
   * and all of its children. */
  libcriu_wrapper->criu_set_pid (status->pid);

  ret = setup_criu_work_dir (cr_options, &work_fd, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

#  ifdef CRIU_PRE_DUMP_SUPPORT

  {
    পূর্ণাঙ্ক criu_can_mem_track = 0;
    /* If the user uses --pre-dump for the second time or does
     * a final dump from a previous pre-dump, setting parent_path
     * is necessary so that CRIU can find which pages have not
     * changed compared to the previous dump. */
    যদি (cr_options->parent_path != রিক্ত)
      {
        criu_can_mem_track = criu_check_mem_track (err);
        যদি (UNLIKELY (criu_can_mem_track == -1))
          ঘূৰাই -1;
        libcriu_wrapper->criu_set_track_mem (true);

        /* The parent path must be relative to image path (something like ../previous-dump).
           CRIU will fail with an unclear error message if the path is not right.
         */
        যদি (UNLIKELY (cr_options->parent_path[0] == '/'))
          ঘূৰাই crun_make_error (err, 0, "--parent-path must be relative");
        পূর্ণাঙ্ক is_dir = crun_dir_p_at (image_fd, cr_options->parent_path, false, err);
        যদি (UNLIKELY (is_dir <= 0))
          {
            যদি (is_dir < 0)
              ঘূৰাই crun_error_wrap (err, "invalid --parent-path");
            ঘূৰাই crun_make_error (err, ENOTDIR, "invalid --parent-path");
          }
        ret = libcriu_wrapper->criu_set_parent_images (cr_options->parent_path);
        যদি (UNLIKELY (ret != 0))
          ঘূৰাই crun_make_error (err, -ret, "error setting CRIU parent images path to `%s`", cr_options->parent_path);
      }

    যদি (cr_options->pre_dump)
      {
        যদি (criu_can_mem_track != 1)
          {
            criu_can_mem_track = criu_check_mem_track (err);
            যদি (UNLIKELY (criu_can_mem_track == -1))
              ঘূৰাই -1;
          }
        libcriu_wrapper->criu_set_track_mem (true);
        ret = libcriu_wrapper->criu_pre_dump ();
        যদি (UNLIKELY (ret != 0))
          {
            show_criu_log (cr_options->work_path, CRIU_CHECKPOINT_LOG_FILE);
            ঘূৰাই crun_make_error (err, 0, "CRIU pre-dump failed: %d", ret);
          }
        ঘূৰাই 0;
      }
  }
#  endif

  /* descriptors.json is needed during restore to correctly
   * reconnect stdin, stdout, stderr. */
  ret = append_paths (&descriptors_path, err, cr_options->image_path, DESCRIPTORS_FILENAME, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = write_file (descriptors_path, status->external_descriptors, safe_strlen (status->external_descriptors), err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_error_wrap (err, "error saving CRIU descriptors file");

  ret = append_paths (&path, err, status->bundle, status->rootfs, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = libcriu_wrapper->criu_set_মূল (path);
  যদি (UNLIKELY (ret != 0))
    ঘূৰাই crun_make_error (err, 0, "error setting CRIU root to `%s`", path);

  cgroup_mode = libcrun_get_cgroup_mode (err);
  যদি (UNLIKELY (cgroup_mode < 0))
    ঘূৰাই cgroup_mode;

  /* For cgroup v1 we need to tell CRIU to handle all cgroup mounts as external mounts. */
  যদি (cgroup_mode != CGROUP_MODE_UNIFIED)
    {
      ret = checkpoint_cgroup_v1_mount (def, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  /* Tell CRIU about external bind mounts. */
  বাবে (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      যদি (is_bind_mount (def->mounts[i], রিক্ত, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          ৱর্ণ buf[PATH_MAX];
          ধ্রুৱ ৱর্ণ *dest_in_মূল;

          যদি (nofollow)
            ঘূৰাই crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_in_মূল = chroot_realpath (status->rootfs, def->mounts[i]->destination, buf);
          যদি (UNLIKELY (dest_in_মূল == রিক্ত))
            ঘূৰাই crun_make_error (err, errno, "unable to resolve external bind mount `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          যদি (has_prefix (dest_in_মূল, status->rootfs))
            dest_in_মূল += safe_strlen (status->rootfs);

          ret = libcriu_wrapper->criu_add_ext_mount (dest_in_মূল, dest_in_মূল);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", def->mounts[i]->destination);
        }
    }

  ret = register_masked_paths_mounts (def, container, libcriu_wrapper, false, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

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

  বাবে (i = 0; i < def->linux->namespaces_len; i++)
    {
      পূর্ণাঙ্ক মান = libcrun_find_namespace (def->linux->namespaces[i]->type);
      যদি (UNLIKELY (মান < 0))
        ঘূৰাই crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      যদি (মান == CLONE_NEWNET && def->linux->namespaces[i]->path != রিক্ত)
        {
          cleanup_free ৱর্ণ *external = রিক্ত;
          সংরচনা stat statbuf;

          ret = stat (def->linux->namespaces[i]->path, &statbuf);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, errno, "unable to stat(): `%s`", def->linux->namespaces[i]->path);

          xasprintf (&external, "net[%ld]:" CRIU_EXT_NETNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_add_external (external);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }

      যদি (মান == CLONE_NEWPID && def->linux->namespaces[i]->path != রিক্ত)
        {
          cleanup_free ৱর্ণ *external = রিক্ত;
          সংরচনা stat statbuf;

          ret = stat (def->linux->namespaces[i]->path, &statbuf);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, errno, "unable to stat(): `%s`", def->linux->namespaces[i]->path);

          xasprintf (&external, "pid[%ld]:" CRIU_EXT_PIDNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_add_external (external);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }
    }

  /* Tell CRIU to use the freezer to pause all container processes. */
  যদি (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      /* This needs CRIU 3.14. */
      ret = append_paths (&freezer_path, err, CGROUP_মূল, status->cgroup_path, রিক্ত);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }
  অন্যথা
    {
      ret = append_paths (&freezer_path, err, CGROUP_মূল "/freezer", status->cgroup_path, রিক্ত);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  ret = libcriu_wrapper->criu_set_freeze_cgroup (freezer_path);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, -ret, "CRIU: failed setting freezer %d", ret);

  /* Set boolean options . */
  libcriu_wrapper->criu_set_leave_running (cr_options->leave_running);
  libcriu_wrapper->criu_set_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_set_shell_job (cr_options->shell_job);
  libcriu_wrapper->criu_set_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_set_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_set_orphan_pts_master (true);
  libcriu_wrapper->criu_set_manage_cgroups_mode (criu_cg_mode_from_public (cr_options->manage_cgroups_mode));

  libcriu_wrapper->criu_set_manage_cgroups (true);

  যদি (libcriu_wrapper->criu_set_network_lock)
    {
      পূর্ণাঙ্ক lock = 0;

      ret = criu_network_lock_from_public (cr_options->network_lock_method, &lock, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      যদি (lock > 0)
        {
          ret = libcriu_wrapper->criu_set_network_lock (lock);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, 0, "CRIU: failed setting network lock");
        }
    }

  ret = libcriu_wrapper->criu_dump ();
  যদি (UNLIKELY (ret != 0))
    {
      show_criu_log (cr_options->work_path, CRIU_CHECKPOINT_LOG_FILE);
      ঘূৰাই crun_make_error (err, ret < 0 ? -ret : 0, "CRIU checkpointing failed: %d", ret);
    }

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
prepare_restore_mounts_internal (runtime_spec_schema_config_schema *def, ৱর্ণ *মূল, ধ্রুৱ ৱর্ণ **mounted,
                                 size_t *n_mounted, libcrun_error_t *err)
{
  uint32_t i;

  /* Go through all mountpoints to be able to recreate missing mountpoints. */
  বাবে (i = 0; i < def->mounts_len; i++)
    {
      ৱর্ণ *dest = def->mounts[i]->destination;
      ৱর্ণ *type = def->mounts[i]->type;
      cleanup_close পূর্ণাঙ্ক মূল_fd = -1;
      bool nofollow = false;
      bool on_tmpfs = false;
      পূর্ণাঙ্ক is_dir = 1;
      size_t j;

      /* cgroup restore should be handled by CRIU itself */
      যদি (type && (strcmp (type, "cgroup") == 0 || strcmp (type, "cgroup2") == 0))
        অনুৱর্ত;

      /* Check if the mountpoint is on a tmpfs. CRIU restores
       * all tmpfs. We do need to recreate directories on a tmpfs. */
      size_t dest_len = strlen (dest);
      বাবে (j = 0; j < def->mounts_len; j++)
        {
          যদি (def->mounts[j]->type == রিক্ত || strcmp (def->mounts[j]->type, "tmpfs") != 0)
            অনুৱর্ত;
          size_t mount_len = strlen (def->mounts[j]->destination);
          যদি (mount_len < dest_len && dest[mount_len] == '/' && strncmp (dest, def->mounts[j]->destination, mount_len) == 0)
            {
              /* This is a mountpoint which is on a tmpfs.*/
              on_tmpfs = true;
              ভঙ্গ;
            }
        }

      যদি (on_tmpfs)
        অনুৱর্ত;

      /* For bind mounts check if the source is a file or a directory. */
      যদি (is_bind_mount (def->mounts[i], রিক্ত, &nofollow))
        {
          যদি (nofollow)
            ঘূৰাই crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          is_dir = crun_dir_p (def->mounts[i]->source, false, err);
          যদি (UNLIKELY (is_dir < 0))
            ঘূৰাই is_dir;
        }

      মূল_fd = open (মূল, O_RDONLY | O_CLOEXEC);
      যদি (UNLIKELY (মূল_fd == -1))
        ঘূৰাই crun_make_error (err, errno, "error opening container root directory `%s`", মূল);

      যদি (is_dir)
        {
          পূর্ণাঙ্ক ret;

          ret = crun_safe_ensure_directory_at (মূল_fd, মূল, dest, 0755, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
        }
      অন্যথা
        {
          পূর্ণাঙ্ক ret;

          ret = crun_safe_ensure_file_at (মূল_fd, মূল, dest, 0755, err);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই ret;
        }

      /* Mount a bind mount source now, so that the mountpoints of the
       * mounts inside it (e.g. a nested bind mount) are created in the
       * source, where CRIU expects them.  This also happens during the
       * initial container creation, as the mounts are done in order.  */
      যদি (is_bind_mount (def->mounts[i], রিক্ত, &nofollow))
        {
          cleanup_close পূর্ণাঙ্ক dst_fd = -1;
          proc_fd_path_t fd_path;

          dst_fd = safe_openat (মূল_fd, মূল, dest, O_PATH | O_CLOEXEC, 0, err);
          যদি (UNLIKELY (dst_fd < 0))
            ঘূৰাই dst_fd;

          /* Not recursive: the source might contain ROOT itself (e.g. when
           * it is the bundle directory), and it is not needed anyway.  */
          get_proc_self_fd_path (fd_path, dst_fd);
          যদি (UNLIKELY (mount (def->mounts[i]->source, fd_path, রিক্ত, MS_BIND, রিক্ত) < 0))
            ঘূৰাই crun_make_error (err, errno, "bind mount `%s` to `%s`", def->mounts[i]->source, dest);

          mounted[(*n_mounted)++] = dest;
        }
    }

  ঘূৰাই 0;
}

/* Move the current process back to CGROUPS, as read from /proc/self/cgroup.
   A failure is not fatal, so only warn about it.  */
স্থির শূন্য
move_back_to_cgroups (ধ্রুৱ ৱর্ণ *cgroups)
{
  libcrun_error_t tmp_err = রিক্ত;

  যদি (UNLIKELY (libcrun_move_self_to_cgroups (cgroups, &tmp_err) < 0))
    {
      libcrun_warning ("cannot move back to the original cgroup: %s", tmp_err->বাৰ্তা);
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
স্থির পূর্ণাঙ্ক
criu_restore_child_in_cgroup (ধ্রুৱ ৱর্ণ *cgroup_path, পূর্ণাঙ্ক *criu_ret, libcrun_error_t *err)
{
  sigset_t all_signals, old_mask;
  পূর্ণাঙ্ক wait_status = 0;
  pid_t pid;
  পূর্ণাঙ্ক ret;

  *criu_ret = -1;

  sigfillset (&all_signals);
  ret = sigprocmask (SIG_BLOCK, &all_signals, &old_mask);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "sigprocmask");

  /* Must be vfork: the child shares our memory space, so both *criu_ret and
     the error it creates are visible here once it is gone.  */
  pid = vfork ();
  যদি (UNLIKELY (pid < 0))
    {
      পূর্ণাঙ্ক saved_errno = errno;

      sigprocmask (SIG_SETMASK, &old_mask, রিক্ত);
      ঘূৰাই crun_make_error (err, saved_errno, "vfork");
    }

  যদি (pid == 0)
    {
      সংরচনা sigaction act;
      পূর্ণাঙ্ক i;

      /* The signal dispositions are not shared with the parent, so resetting
         them here does not affect it.  Keep the ignored signals ignored, as
         CRIU would inherit them if it was run directly by the caller.  */
      বাবে (i = 1; i < NSIG; i++)
        {
          যদি (sigaction (i, রিক্ত, &act) < 0 || act.sa_handler == SIG_IGN || act.sa_handler == SIG_DFL)
            অনুৱর্ত;

          memset (&act, 0, পরিমাণ (act));
          act.sa_handler = SIG_DFL;
          sigaction (i, &act, রিক্ত);
        }
      sigprocmask (SIG_SETMASK, &old_mask, রিক্ত);

      যদি (! is_empty_string (cgroup_path))
        {
          ret = libcrun_move_process_to_cgroup (0, 0, cgroup_path, false, err);
          যদি (UNLIKELY (ret < 0))
            _safe_exit (EXIT_FAILURE);
        }

      *criu_ret = libcriu_wrapper->criu_restore_child ();
      _safe_exit (EXIT_SUCCESS);
    }

  /* The child is gone by now, it is safe to handle signals again.  */
  sigprocmask (SIG_SETMASK, &old_mask, রিক্ত);

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  যদি (UNLIKELY (ret < 0))
    {
      /* The caller might have reaped the child already, e.g. when it ignores
         SIGCHLD.  Its result is still visible, so use it.  */
      যদি (errno != ECHILD)
        ঘূৰাই crun_make_error (err, errno, "waitpid for the CRIU restore process");

      /* The child might have failed before calling CRIU, and left an error.  */
      ঘূৰাই *err != রিক্ত ? -1 : 0;
    }

  ret = get_process_exit_status (wait_status);
  যদি (UNLIKELY (ret != EXIT_SUCCESS))
    {
      /* The child creates the error in the shared memory space, do not
         overwrite it with crun_make_error().  */
      যদি (*err == রিক্ত)
        ঘূৰাই crun_make_error (err, 0, "the CRIU restore process exited with status %d", ret);
      ঘূৰাই -1;
    }

  ঘূৰাই 0;
}

/* Recreate the mountpoints which do not exist in the container rootfs
 * mounted at ROOT, the same way it is done on the container creation.  */
স্থির পূর্ণাঙ্ক
prepare_restore_mounts (runtime_spec_schema_config_schema *def, ৱর্ণ *মূল, libcrun_error_t *err)
{
  cleanup_free ধ্রুৱ ৱর্ণ **mounted = xmalloc0 (পরিমাণ (ৱর্ণ *) * (def->mounts_len + 1));
  cleanup_close পূর্ণাঙ্ক মূল_fd = -1;
  size_t n_mounted = 0;
  পূর্ণাঙ্ক ret;

  ret = prepare_restore_mounts_internal (def, মূল, mounted, &n_mounted, err);

  /* The bind mounts done above are only needed to create the mountpoints,
   * and CRIU restores the mounts itself, so undo them, in reverse order. */
  যদি (n_mounted > 0)
    মূল_fd = open (মূল, O_PATH | O_CLOEXEC);
  যেতিয়ালৈ (মূল_fd >= 0 && n_mounted > 0)
    {
      libcrun_error_t tmp_err = রিক্ত;
      ধ্রুৱ ৱর্ণ *dest = mounted[--n_mounted];
      cleanup_close পূর্ণাঙ্ক dst_fd = -1;
      proc_fd_path_t fd_path;

      dst_fd = safe_openat (মূল_fd, মূল, dest, O_PATH | O_CLOEXEC, 0, &tmp_err);
      যদি (UNLIKELY (dst_fd < 0))
        {
          crun_error_release (&tmp_err);
          অনুৱর্ত;
        }
      get_proc_self_fd_path (fd_path, dst_fd);
      যদি (UNLIKELY (umount2 (fd_path, MNT_DETACH) < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "unmount `%s`", dest);
    }

  ঘূৰাই ret;
}

পূর্ণাঙ্ক
libcrun_container_restore_linux_criu (libcrun_container_status_t *status, libcrun_container_t *container,
                                      libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper সংরচনা libcriu_wrapper_s *wrapper = রিক্ত;
  cleanup_close পূর্ণাঙ্ক inherit_new_net_fd = -1;
  cleanup_close পূর্ণাঙ্ক inherit_new_pid_fd = -1;
  cleanup_close পূর্ণাঙ্ক image_fd = -1;
  cleanup_free ৱর্ণ *মূল = রিক্ত;
  cleanup_free ৱর্ণ *bundle_cleanup = রিক্ত;
  cleanup_free ৱর্ণ *own_cgroups = রিক্ত;
  cleanup_close পূর্ণাঙ্ক work_fd = -1;
  পূর্ণাঙ্ক cgroup_mode;
  পূর্ণাঙ্ক criu_ret;
  পূর্ণাঙ্ক ret_out;
  size_t i;
  পূর্ণাঙ্ক ret;

  ret = load_wrapper (&wrapper, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  যদি (geteuid ())
    ঘূৰাই crun_make_error (err, 0, "restoring requires root");

  ret = libcriu_wrapper->criu_init_opts ();
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_version (err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  যদি (UNLIKELY (cr_options->image_path == রিক্ত))
    ঘূৰাই crun_make_error (err, 0, "image path not set");

  image_fd = open (cr_options->image_path, O_DIRECTORY | O_CLOEXEC);
  যদি (UNLIKELY (image_fd == -1))
    ঘূৰাই crun_make_error (err, errno, "error opening checkpoint directory `%s`", cr_options->image_path);

  libcriu_wrapper->criu_set_images_dir_fd (image_fd);

  /* Load descriptors.json to tell CRIU where those FDs should be connected to. */
  {
    cleanup_free ৱর্ণ *descriptors_path = রিক্ত;
    cleanup_free ৱর্ণ *buffer = রিক্ত;
    json_object *doc = রিক্ত;

    ret = append_paths (&descriptors_path, err, cr_options->image_path, DESCRIPTORS_FILENAME, রিক্ত);
    যদি (UNLIKELY (ret < 0))
      ঘূৰাই ret;

    ret = read_all_file (descriptors_path, &buffer, রিক্ত, err);
    যদি (UNLIKELY (ret < 0))
      ঘূৰাই ret;

    /* descriptors.json contains a JSON array with strings
     * telling where 0, 1 and 2 have been initially been
     * pointing to. For each descriptor which points to
     * a pipe 'pipe:' we tell CRIU to reconnect that pipe
     * to the corresponding FD to have (especially) stdout
     * and stderr being correctly redirected. */
    ret = parse_json_file (&doc, buffer, রিক্ত, err);
    যদি (UNLIKELY (ret < 0))
      ঘূৰাই ret;

    যদি (json_object_is_type (doc, json_type_array))
      {
        size_t i, len = json_object_array_length (doc);

        /* len will probably always be 3 as crun is currently only
         * recording the destination of FD 0, 1 and 2. */
        বাবে (i = 0; i < len; ++i)
          {
            json_object *s = json_object_array_get_idx (doc, i);
            যদি (s && json_object_is_type (s, json_type_string))
              {
                ধ্রুৱ ৱর্ণ *str = json_object_get_string (s);
                যদি (has_prefix (str, "pipe:"))
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
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = setup_criu_work_dir (cr_options, &work_fd, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  যদি (cr_options->lsm_profile != রিক্ত)
    {
      ret = libcriu_wrapper->criu_set_lsm_profile (cr_options->lsm_profile);
      যদি (UNLIKELY (ret != 0))
        ঘূৰাই crun_make_error (err, -ret, "error setting LSM profile to `%s`", cr_options->lsm_profile);
    }

  যদি (cr_options->lsm_mount_context != রিক্ত)
    {
      ret = libcriu_wrapper->criu_set_lsm_mount_context (cr_options->lsm_mount_context);
      যদি (UNLIKELY (ret != 0))
        ঘূৰাই crun_make_error (err, -ret, "error setting LSM mount context to `%s`", cr_options->lsm_mount_context);
    }

  /* do realpath on root */
  bundle_cleanup = realpath (status->bundle, রিক্ত);
  যদি (UNLIKELY (bundle_cleanup == রিক্ত))
    bundle_cleanup = xstrdup (status->bundle);

  /* Tell CRIU about external bind mounts. */
  বাবে (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      যদি (is_bind_mount (def->mounts[i], রিক্ত, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          ৱর্ণ buf[PATH_MAX];
          ধ্রুৱ ৱর্ণ *dest_in_মূল;
          ধ্রুৱ ৱর্ণ *source = def->mounts[i]->source;
          cleanup_free ৱর্ণ *abs_source = রিক্ত;
          unsigned long flags = 0;
          size_t j;

          যদি (nofollow)
            ঘূৰাই crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_in_মূল = chroot_realpath (status->rootfs, def->mounts[i]->destination, buf);
          যদি (UNLIKELY (dest_in_মূল == রিক্ত))
            ঘূৰাই crun_make_error (err, errno, "unable to resolve external bind mount destination `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          যদি (has_prefix (dest_in_মূল, status->rootfs))
            dest_in_মূল += safe_strlen (status->rootfs);

          /* A relative source is relative to the bundle, while CRIU would
             resolve it relative to its own working directory.  */
          যদি (source && source[0] != '/')
            {
              ret = append_paths (&abs_source, err, bundle_cleanup, source, রিক্ত);
              যদি (UNLIKELY (ret < 0))
                ঘূৰাই ret;
              source = abs_source;
            }

          ret = libcriu_wrapper->criu_add_ext_mount (dest_in_মূল, source);
          যদি (UNLIKELY (ret < 0))
            ঘূৰাই crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", source);

          বাবে (j = 0; j < def->mounts[i]->options_len; j++)
            {
              ধ্রুৱ সংরচনা propagation_flags_s *prop = রিক্ত;

              যদি (def->mounts[i]->options[j])
                prop = libcrun_str2mount_flags (def->mounts[i]->options[j]);
              যদি (prop && ! prop->clear)
                flags |= prop->flags;
            }
          add_restored_ext_mount (dest_in_মূল, flags);
        }
    }

  ret = register_masked_paths_mounts (def, container, libcriu_wrapper, true, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  /* Mount the container rootfs for CRIU. */
  ret = append_paths (&মূল, err, bundle_cleanup, "criu-root", রিক্ত);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = mkdir (মূল, 0755);
  যদি (UNLIKELY (ret == -1))
    ঘূৰাই crun_make_error (err, errno, "error creating restore directory `%s`", মূল);

  ret = mount (status->rootfs, মূল, রিক্ত, MS_BIND | MS_REC, রিক্ত);
  যদি (UNLIKELY (ret == -1))
    {
      ret = crun_make_error (err, errno, "error mounting restore directory `%s`", মূল);
      গচ্ছ out;
    }

  /* During initial container creation, crun will create mountpoints
   * defined in config.json if they do not exist. If we are restoring
   * we need to make sure these mountpoints also exist.
   * This is not perfect, as this means crun will modify a rootfs
   * even if it marked as read-only, but runc already modifies
   * the rootfs in the same way. */

  ret = prepare_restore_mounts (def, মূল, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ out_umount;

  ret = libcriu_wrapper->criu_set_মূল (মূল);
  যদি (UNLIKELY (ret != 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU root to `%s`", মূল);
      গচ্ছ out_umount;
    }

  /* If a namespace defined in config.json we are telling
   * CRIU use that namespace when restoring the process tree.
   *
   * CRIU expects the information about the namespace like this:
   * --inherit-fd fd[<fd>]:<key>
   * The <key> needs to be the same as during checkpointing (extRootNetNS). */
  বাবে (i = 0; i < def->linux->namespaces_len; i++)
    {
      ধ্রুৱ পূর্ণাঙ্ক open_flags_for_inherit = O_RDONLY; /* Cannot be O_CLOEXEC as it is passed to the child process. */
      পূর্ণাঙ্ক মান = libcrun_find_namespace (def->linux->namespaces[i]->type);
      যদি (UNLIKELY (মান < 0))
        {
          ret = crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);
          গচ্ছ out_umount;
        }

      যদি (মান == CLONE_NEWNET && def->linux->namespaces[i]->path != রিক্ত)
        {
          inherit_new_net_fd = open (def->linux->namespaces[i]->path, open_flags_for_inherit);
          যদি (UNLIKELY (inherit_new_net_fd < 0))
            {
              ret = crun_make_error (err, errno, "unable to open(): `%s`", def->linux->namespaces[i]->path);
              গচ্ছ out_umount;
            }

          ret = libcriu_wrapper->criu_add_inherit_fd (inherit_new_net_fd, CRIU_EXT_NETNS);
          যদি (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              গচ্ছ out_umount;
            }
        }

      যদি (মান == CLONE_NEWPID && def->linux->namespaces[i]->path != রিক্ত)
        {
          inherit_new_pid_fd = open (def->linux->namespaces[i]->path, open_flags_for_inherit);
          যদি (UNLIKELY (inherit_new_pid_fd < 0))
            {
              ret = crun_make_error (err, errno, "unable to open(): `%s`", def->linux->namespaces[i]->path);
              গচ্ছ out_umount;
            }

          ret = libcriu_wrapper->criu_add_inherit_fd (inherit_new_pid_fd, CRIU_EXT_PIDNS);
          যদি (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              গচ্ছ out_umount;
            }
        }

#  ifdef CRIU_JOIN_NS_SUPPORT
      যদি (মান == CLONE_NEWTIME && def->linux->namespaces[i]->path != রিক্ত)
        {
          যদি (libcriu_wrapper->criu_join_ns_add == রিক্ত)
            {
              ret = crun_make_error (err, 0, "shared time namespace restore is supported in CRIU >= 3.16.1");
              গচ্ছ out_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_add ("time", def->linux->namespaces[i]->path, রিক্ত);
          যদি (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->path);
              গচ্ছ out_umount;
            }
        }

      যদি (মান == CLONE_NEWIPC && def->linux->namespaces[i]->path != রিক্ত)
        {
          যদি (libcriu_wrapper->criu_join_ns_add == রিক্ত)
            {
              ret = crun_make_error (err, 0, "shared ipc namespace restore is supported in CRIU >= 3.16.1");
              গচ্ছ out_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_add ("ipc", def->linux->namespaces[i]->path, রিক্ত);
          যদি (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->path);
              গচ্ছ out_umount;
            }
        }

      যদি (মান == CLONE_NEWUTS && def->linux->namespaces[i]->path != রিক্ত)
        {
          যদি (libcriu_wrapper->criu_join_ns_add == রিক্ত)
            {
              ret = crun_make_error (err, 0, "shared uts namespace restore is supported in CRIU >= 3.16.1");
              গচ্ছ out_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_add ("uts", def->linux->namespaces[i]->path, রিক্ত);
          যদি (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->path);
              গচ্ছ out_umount;
            }
        }
#  endif
    }

  /* Set up CRIU config file */
  ret = handle_criu_config_file (container, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ out_umount;

  /* Tell CRIU if cgroup v1 needs to be handled. */
  ret = restore_cgroup_v1_mount (def, err);
  যদি (UNLIKELY (ret < 0))
    গচ্ছ out_umount;

  console_socket = cr_options->console_socket;
  libcriu_wrapper->criu_set_notify_cb (criu_notify);

  /* Set boolean options . */
  libcriu_wrapper->criu_set_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_set_shell_job (cr_options->shell_job);
  libcriu_wrapper->criu_set_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_set_tcp_close (cr_options->tcp_close);
  libcriu_wrapper->criu_set_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_set_orphan_pts_master (true);

  যদি (status->cgroup_path)
    {
      cgroup_mode = libcrun_get_cgroup_mode (err);
      যদি (UNLIKELY (cgroup_mode < 0))
        {
          ret = cgroup_mode;
          গচ্ছ out_umount;
        }

      /* With cgroup v2, the cgroup path is only meaningful for the unified
         hierarchy, so only relocate that one.  A NULL controller would make
         CRIU relocate every hierarchy the container is in, named cgroup v1
         ones included.  As CRIU dumps such a hierarchy from wherever the
         container is in it, which is its root, as crun leaves named
         hierarchies alone, every checkpoint and restore would then copy the
         whole hierarchy into itself.  */
      ret = libcriu_wrapper->criu_add_cg_মূল (cgroup_mode == CGROUP_MODE_UNIFIED ? "" : রিক্ত,
                                               status->cgroup_path);
      যদি (UNLIKELY (ret != 0))
        {
          ret = crun_make_error (err, 0, "error setting CRIU cgroup root to `%s`", status->cgroup_path);
          গচ্ছ out_umount;
        }
    }

  libcriu_wrapper->criu_set_manage_cgroups_mode (criu_cg_mode_from_public (cr_options->manage_cgroups_mode));
  libcriu_wrapper->criu_set_manage_cgroups (true);

  যদি (libcriu_wrapper->criu_set_network_lock)
    {
      পূর্ণাঙ্ক lock = 0;

      ret = criu_network_lock_from_public (cr_options->network_lock_method, &lock, err);
      যদি (UNLIKELY (ret < 0))
        গচ্ছ out_umount;

      যদি (lock > 0)
        {
          ret = libcriu_wrapper->criu_set_network_lock (lock);
          যদি (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "CRIU: failed setting network lock");
              গচ্ছ out_umount;
            }
        }
    }

  libcriu_wrapper->criu_set_log_level (4);
  ret = libcriu_wrapper->criu_set_log_file (CRIU_RESTORE_LOG_FILE);
  যদি (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU log file to `%s`", CRIU_RESTORE_LOG_FILE);
      গচ্ছ out_umount;
    }

  /* criu_restore() returns the PID of the process of the restored process
   * tree. This PID will not be the same as status->pid if the container is
   * running in a PID namespace. But it will always be > 0. */
  যদি (cr_options->detach)
    {
      /* Nothing waits for the container, so there is no reason to take over
         this process: use a throw-away one.  It is used even when there is no
         cgroup to join, so that the container is not left as a child of a
         process which is not going to reap it.  */
      ret = criu_restore_child_in_cgroup (status->cgroup_path, &criu_ret, err);
      যদি (UNLIKELY (ret < 0))
        গচ্ছ out_umount;

      ret = criu_ret;
    }
  অন্যথা
    {
      /* Like runc does, put CRIU into the container cgroup for the time of
       * restore, so the restored tasks are created there. It is necessary in
       * the "ignore" mode, where CRIU does not deal with cgroups at all, and
       * does not hurt otherwise. As CRIU is our child, do it by moving
       * ourselves there, and move back once the restore is done. */
      যদি (! is_empty_string (status->cgroup_path))
        {
          ret = read_all_file (PROC_SELF_CGROUP, &own_cgroups, রিক্ত, err);
          যদি (UNLIKELY (ret < 0))
            গচ্ছ out_umount;

          ret = libcrun_move_process_to_cgroup (0, 0, status->cgroup_path, false, err);
          যদি (UNLIKELY (ret < 0))
            {
              /* Some of the cgroups might have been joined already.  */
              move_back_to_cgroups (own_cgroups);
              গচ্ছ out_umount;
            }
        }

      ret = libcriu_wrapper->criu_restore_child ();

      যদি (own_cgroups)
        move_back_to_cgroups (own_cgroups);
    }

  যদি (UNLIKELY (ret <= 0))
    {
      show_criu_log (cr_options->work_path, CRIU_RESTORE_LOG_FILE);
      ret = crun_make_error (err, 0, "CRIU restoring failed: %d", ret);
      গচ্ছ out_umount;
    }

  /* Update the status struct with the newly allocated PID. This will
   * be necessary later when moving the process into its cgroup. */
  status->pid = ret;
  ret = 0;

out_umount:
  ret_out = umount (মূল);
  যদি (UNLIKELY (ret_out == -1))
    {
      পূর্ণাঙ্ক saved_errno = errno;
      rmdir (মূল);
      যদি (ret < 0)
        ঘূৰাই crun_error_wrap (err, "error unmounting restore directory `%s`", মূল);
      অন্যথা
        ঘূৰাই crun_make_error (err, saved_errno, "error unmounting restore directory `%s`", মূল);
    }
out:
  ret_out = rmdir (মূল);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;
  যদি (UNLIKELY (ret_out == -1))
    ঘূৰাই crun_make_error (err, errno, "error removing restore directory `%s`", মূল);
  ঘূৰাই ret;
}
#endif
