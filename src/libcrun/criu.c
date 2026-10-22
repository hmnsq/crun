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

#અન્તર્ભાવ <config.h>

#જો HAVE_CRIU && HAVE_DLOPEN

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
અક્ષર *chroot_realpath (અચલ અક્ષર *chroot, અચલ અક્ષર *path, અક્ષર resolved_path[]);

/* Map the public cgroups mode (enum libcrun_cr_cgroups_mode) onto the CRIU enum.  */
સ્થિર ગણના criu_cg_mode
criu_cg_mode_from_public (પૂર્ણાંક mode)
{
  પસંદગી (mode)
    {
    સ્થિતિ LIBCRUN_CR_CG_MODE_IGNORE:
      પાછા CRIU_CG_MODE_IGNORE;
    સ્થિતિ LIBCRUN_CR_CG_MODE_FULL:
      પાછા CRIU_CG_MODE_FULL;
    સ્થિતિ LIBCRUN_CR_CG_MODE_STRICT:
      પાછા CRIU_CG_MODE_STRICT;
    સ્થિતિ LIBCRUN_CR_CG_MODE_DEFAULT:
    સ્થિતિ LIBCRUN_CR_CG_MODE_SOFT:
    મૂળભૂત:
      /* Default to CRIU_CG_MODE_SOFT, just as runc.  */
      પાછા CRIU_CG_MODE_SOFT;
    }
}

/* Map the public network lock method (enum libcrun_cr_network_lock_method) onto the
   CRIU enum.  *LOCK is set to 0 when no explicit method must be set (CRIU default).
   A method that this build cannot honor is an error, never a silent fallback to
   the CRIU default.  */
સ્થિર પૂર્ણાંક
criu_network_lock_from_public (પૂર્ણાંક method, પૂર્ણાંક *lock, libcrun_error_t *err)
{
  પસંદગી (method)
    {
    સ્થિતિ LIBCRUN_CR_NETWORK_LOCK_IPTABLES:
      *lock = CRIU_NETWORK_LOCK_IPTABLES;
      પાછા 0;
    સ્થિતિ LIBCRUN_CR_NETWORK_LOCK_NFTABLES:
      *lock = CRIU_NETWORK_LOCK_NFTABLES;
      પાછા 0;
    સ્થિતિ LIBCRUN_CR_NETWORK_LOCK_SKIP:
#  જો CRIU_NETWORK_LOCK_SKIP_SUPPORT
      *lock = CRIU_NETWORK_LOCK_SKIP;
      પાછા 0;
#  નહીંતર
      પાછા crun_make_error (err, 0, "CRIU: the `skip` network lock method is not supported by this build");
#  endif
    સ્થિતિ LIBCRUN_CR_NETWORK_LOCK_DEFAULT:
      *lock = 0;
      પાછા 0;
    મૂળભૂત:
      પાછા crun_make_error (err, 0, "CRIU: unknown network lock method %d", method);
    }
}

સ્થિર અચલ અક્ષર *console_socket = ખાલી;

/* The external mounts CRIU restores, and the flags the configuration asks
   for each of them.  */
સ્થિર સંરચના libcrun_mount_flags_s *restored_ext_mounts = ખાલી;
સ્થિર size_t n_restored_ext_mounts = 0;

#  define LIBCRIU_MIN_VERSION 31500

સંરચના libcriu_wrapper_s
{
  શૂન્ય *handle;
  પૂર્ણાંક (*criu_add_ext_mount) (અચલ અક્ષર *ચાવી, અચલ અક્ષર *val);
  પૂર્ણાંક (*criu_add_external) (અચલ અક્ષર *ચાવી);
  પૂર્ણાંક (*criu_add_inherit_fd) (પૂર્ણાંક fd, અચલ અક્ષર *ચાવી);
  પૂર્ણાંક (*criu_check_version) (પૂર્ણાંક minimum);
  પૂર્ણાંક (*criu_dump) (શૂન્ય);
  પૂર્ણાંક (*criu_get_orphan_pts_master_fd) (શૂન્ય);
  પૂર્ણાંક (*criu_init_opts) (શૂન્ય);
  પૂર્ણાંક (*criu_notify_pid) (criu_notify_arg_t na);
#  ifdef CRIU_JOIN_NS_SUPPORT
  પૂર્ણાંક (*criu_join_ns_add) (અચલ અક્ષર *ns, અચલ અક્ષર *ns_file, અચલ અક્ષર *extra_opt);
#  endif
#  ifdef CRIU_PRE_DUMP_SUPPORT
  પૂર્ણાંક (*criu_feature_check) (સંરચના criu_feature_check *features, size_t size);
  પૂર્ણાંક (*criu_pre_dump) (શૂન્ય);
#  endif
  પૂર્ણાંક (*criu_restore_child) (શૂન્ય);
  પૂર્ણાંક (*criu_set_freeze_cgroup) (અચલ અક્ષર *name);
  શૂન્ય (*criu_set_file_locks) (bool file_locks);
  શૂન્ય (*criu_set_ext_unix_sk) (bool ext_unix_sk);
  પૂર્ણાંક (*criu_set_log_file) (અચલ અક્ષર *log_file);
  શૂન્ય (*criu_set_log_level) (પૂર્ણાંક log_level);
  શૂન્ય (*criu_set_leave_running) (bool leave_running);
  શૂન્ય (*criu_set_manage_cgroups) (bool manage);
  શૂન્ય (*criu_set_manage_cgroups_mode) (ગણના criu_cg_mode mode);
  પૂર્ણાંક (*criu_set_network_lock) (ગણના criu_network_lock_method method);
  શૂન્ય (*criu_set_notify_cb) (પૂર્ણાંક (*cb) (અક્ષર *action, criu_notify_arg_t na));
  શૂન્ય (*criu_set_orphan_pts_master) (bool orphan_pts_master);
  શૂન્ય (*criu_set_images_dir_fd) (પૂર્ણાંક fd);
  પૂર્ણાંક (*criu_set_parent_images) (અચલ અક્ષર *path);
  શૂન્ય (*criu_set_pid) (પૂર્ણાંક pid);
  પૂર્ણાંક (*criu_set_મૂળ) (અચલ અક્ષર *મૂળ);
  પૂર્ણાંક (*criu_add_cg_મૂળ) (અચલ અક્ષર *ctrl, અચલ અક્ષર *path);
  શૂન્ય (*criu_set_shell_job) (bool shell_job);
  શૂન્ય (*criu_set_tcp_established) (bool tcp_established);
  શૂન્ય (*criu_set_tcp_close) (bool tcp_close);
  શૂન્ય (*criu_set_track_mem) (bool track_mem);
  શૂન્ય (*criu_set_work_dir_fd) (પૂર્ણાંક fd);
  પૂર્ણાંક (*criu_set_lsm_profile) (અચલ અક્ષર *name);
  પૂર્ણાંક (*criu_set_lsm_mount_context) (અચલ અક્ષર *name);
  પૂર્ણાંક (*criu_set_config_file) (અચલ અક્ષર *path);
};

સ્થિર સંરચના libcriu_wrapper_s *libcriu_wrapper;

સ્થિર અંતઃરેખ શૂન્ય
cleanup_wrapper (શૂન્ય *p)
{
  સંરચના libcriu_wrapper_s **w;

  w = (સંરચના libcriu_wrapper_s **) p;
  જો (*w == ખાલી)
    પાછા;

#  ifndef STATIC
  જો ((*w)->handle)
    dlclose ((*w)->handle);
#  endif
  મુક્ત (*w);
  libcriu_wrapper = ખાલી;

  જ્યારે (n_restored_ext_mounts > 0)
    મુક્ત (restored_ext_mounts[--n_restored_ext_mounts].target);
  મુક્ત (restored_ext_mounts);
  restored_ext_mounts = ખાલી;
}

#  define cleanup_wrapper __attribute__ ((cleanup (cleanup_wrapper)))

સ્થિર પૂર્ણાંક
load_wrapper (સંરચના libcriu_wrapper_s **wrapper_out, libcrun_error_t *err)
{
  cleanup_free સંરચના libcriu_wrapper_s *wrapper = xmalloc0 (કદ (*wrapper));

#  ifdef STATIC
#    define LOAD_CRIU_FUNCTION(X, ALLOW_NULL) \
      wrapper->X = &X;
#  નહીંતર
#    define LOAD_CRIU_FUNCTION(X, ALLOW_NULL)                                                    \
      કરો                                                                                         \
        {                                                                                        \
          wrapper->X = dlsym (wrapper->handle, #X);                                              \
          જો (! ALLOW_NULL && wrapper->X == ખાલી)                                                \
            {                                                                                    \
              dlclose (wrapper->handle);                                                         \
              પાછા crun_make_error (err, 0, "could not find symbol `%s` in `libcriu.so`", #X); \
            }                                                                                    \
      } જ્યારે (0)
#  endif

#  ifndef STATIC
  wrapper->handle = dlopen ("libcriu.so.2", RTLD_NOW);
  જો (wrapper->handle == ખાલી)
    પાછા crun_make_error (err, 0, "could not load `libcriu.so.2`: `%s`", dlerror ());
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
  LOAD_CRIU_FUNCTION (criu_set_મૂળ, false);
  LOAD_CRIU_FUNCTION (criu_add_cg_મૂળ, false);
  LOAD_CRIU_FUNCTION (criu_set_shell_job, false);
  LOAD_CRIU_FUNCTION (criu_set_tcp_established, false);
  LOAD_CRIU_FUNCTION (criu_set_tcp_close, false);
  LOAD_CRIU_FUNCTION (criu_set_track_mem, false);
  LOAD_CRIU_FUNCTION (criu_set_work_dir_fd, false);
  LOAD_CRIU_FUNCTION (criu_set_lsm_profile, false);
  LOAD_CRIU_FUNCTION (criu_set_lsm_mount_context, false);
#  જો ! defined STATIC || defined CRIU_CONFIG_FILE
  LOAD_CRIU_FUNCTION (criu_set_config_file, true);
#  endif

  libcriu_wrapper = *wrapper_out = wrapper;
  wrapper = ખાલી;
#  undef LOAD_CRIU_FUNCTION
  પાછા 0;
}

સ્થિર શૂન્ય
add_restored_ext_mount (અચલ અક્ષર *target, unsigned long flags)
{
  restored_ext_mounts = xrealloc (restored_ext_mounts, કદ (*restored_ext_mounts) * (n_restored_ext_mounts + 1));
  restored_ext_mounts[n_restored_ext_mounts].target = xstrdup (target);
  restored_ext_mounts[n_restored_ext_mounts].flags = flags;
  n_restored_ext_mounts++;
}

સ્થિર પૂર્ણાંક
criu_notify (અક્ષર *action, criu_notify_arg_t na)
{
  જો (strcmp (action, "post-restore") == 0)
    {
      /* CRIU takes the source of an external mount from us and its flags
       * from the image, i.e. from whatever mount was at that path in the
       * container when it was checkpointed.  The container can move its
       * mount points around, so before it is resumed make sure that each
       * source is mounted with the restrictions the configuration asks for. */
      libcrun_error_t tmp_err = ખાલી;
      પૂર્ણાંક ret;

      ret = libcrun_restrict_mounts_in_container (libcriu_wrapper->criu_notify_pid (na), restored_ext_mounts,
                                                  n_restored_ext_mounts, &tmp_err);
      જો (UNLIKELY (ret < 0))
        {
          libcrun_error (tmp_err->status, "cannot set the flags of the restored mounts: %s", tmp_err->સંદેશ);
          libcrun_error_release (&tmp_err);
          પાછા ret;
        }
    }

  જો (strncmp (action, "orphan-pts-master", 17) == 0)
    {
      /* CRIU sends us the master FD via the 'orphan-pts-master'
       * callback and we are passing it on to the '--console-socket'
       * if it exists. */
      cleanup_close પૂર્ણાંક console_socket_fd = -1;
      libcrun_error_t tmp_err = ખાલી;
      પૂર્ણાંક master_fd;
      પૂર્ણાંક ret;

      જો (! console_socket)
        પાછા 0;

      master_fd = libcriu_wrapper->criu_get_orphan_pts_master_fd ();

      console_socket_fd = open_unix_domain_client_socket (console_socket, 0, &tmp_err);
      જો (UNLIKELY (console_socket_fd < 0))
        {
          libcrun_error_release (&tmp_err);
          પાછા console_socket_fd;
        }
      ret = send_fd_to_socket (console_socket_fd, master_fd, &tmp_err);
      જો (UNLIKELY (ret < 0))
        {
          libcrun_error_release (&tmp_err);
          પાછા ret;
        }
    }
  પાછા 0;
}

#  ifdef CRIU_PRE_DUMP_SUPPORT

સ્થિર પૂર્ણાંક
criu_check_mem_track (libcrun_error_t *err)
{
  સંરચના criu_feature_check features = { 0 };
  પૂર્ણાંક ret;

  /* Right now we are only interested in checking memory tracking.
   * Memory tracking can be disabled at different levels. aarch64
   * for example has memory tracking not implemented. It could also
   * be not enabled on other architectures. Just ask CRIU if that
   * features exists. */

  features.mem_track = true;

  ret = libcriu_wrapper->criu_feature_check (&features, કદ (features));
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, 0, "CRIU feature checking failed: %d", ret);

  જો (features.mem_track == true)
    પાછા 1;

  પાછા crun_make_error (err, 0, "CRIU memory tracking not supported");
}

#  endif

સ્થિર પૂર્ણાંક
register_masked_paths_mounts (runtime_spec_schema_config_schema *def, libcrun_container_t *container,
                              સંરચના libcriu_wrapper_s *libcriu_wrapper, bool is_restore, libcrun_error_t *err)
{
  cleanup_free અક્ષર *empty_dir_path = ખાલી;
  bool shared_dir_registered = false;
  size_t i;
  પૂર્ણાંક ret;

  માટે (i = 0; i < def->linux->masked_paths_len; i++)
    {
      સંરચના stat statbuf;
      ret = stat (def->linux->masked_paths[i], &statbuf);
      જો (ret != 0)
        ચાલુ;

      જો (S_ISDIR (statbuf.st_mode))
        {
          જો (! shared_dir_registered)
            {
              ret = get_shared_empty_directory_path (&empty_dir_path,
                                                     (container->context ? container->context->state_મૂળ : ખાલી), err);
              જો (UNLIKELY (ret < 0))
                પાછા ret;

              ret = libcriu_wrapper->criu_add_ext_mount (empty_dir_path, empty_dir_path);
              જો (UNLIKELY (ret < 0))
                પાછા crun_make_error (err, -ret, "CRIU: failed adding external mount for shared empty directory `%s`", empty_dir_path);

              shared_dir_registered = true;
            }

          ret = libcriu_wrapper->criu_add_ext_mount (def->linux->masked_paths[i], empty_dir_path);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, -ret, "CRIU: failed adding external mount for masked directory `%s`", def->linux->masked_paths[i]);

          જો (is_restore)
            add_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
      નહીંતર જો (S_ISREG (statbuf.st_mode))
        {
          અચલ અક્ષર *bind_target = is_restore ? "/dev/null" : def->linux->masked_paths[i];
          ret = libcriu_wrapper->criu_add_ext_mount (def->linux->masked_paths[i], bind_target);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", bind_target);

          જો (is_restore)
            add_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
    }

  પાછા 0;
}

/* Parse one /proc/self/cgroup LINE into its subsystem, normalizing the name and
   stripping any "name=" prefix.  When SUBPATH_OUT is not NULL, the in-hierarchy
   path is stored there.  Returns the subsystem, or NULL if the line carries no
   controller and should be skipped.  LINE is modified in place.  */
સ્થિર અક્ષર *
parse_cgroup_subsystem (અક્ષર *લીટી, અક્ષર **subpath_out)
{
  અક્ષર *subsystem, *subpath, *it;

  subsystem = strchr (લીટી, ':') + 1;
  subpath = strchr (subsystem, ':') + 1;
  *(subpath - 1) = '\0';

  જો (subsystem[0] == '\0')
    પાછા ખાલી;

  it = strstr (subsystem, "name=");
  જો (it)
    subsystem = it + 5;

  જો (strcmp (subsystem, "net_prio,net_cls") == 0)
    subsystem = "net_cls,net_prio";
  જો (strcmp (subsystem, "cpuacct,cpu") == 0)
    subsystem = "cpu,cpuacct";

  જો (subpath_out)
    *subpath_out = subpath;
  પાછા subsystem;
}

સ્થિર પૂર્ણાંક
restore_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free અક્ષર *content = ખાલી;
  bool has_cgroup_mount = false;
  અક્ષર *saveptr = ખાલી;
  પૂર્ણાંક cgroup_mode;
  અક્ષર *from;
  પૂર્ણાંક ret;
  uint32_t i;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  જો (UNLIKELY (cgroup_mode < 0))
    પાછા cgroup_mode;

  જો (cgroup_mode == CGROUP_MODE_UNIFIED)
    પાછા 0;

  /* First check if there is actually a cgroup mount in the container. */
  માટે (i = 0; i < def->mounts_len; i++)
    {
      અક્ષર *type = def->mounts[i]->type;
      જો (type && strcmp (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          તોડો;
        }
    }

  જો (! has_cgroup_mount)
    પાછા 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, ખાલી, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (UNLIKELY (content == ખાલી || content[0] == '\0'))
    પાછા crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  માટે (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ખાલી, "\n", &saveptr))
    {
      cleanup_free અક્ષર *destination = ખાલી;
      cleanup_free અક્ષર *source = ખાલી;
      અક્ષર *subsystem;
      અક્ષર *subpath;

      subsystem = parse_cgroup_subsystem (from, &subpath);
      જો (subsystem == ખાલી)
        ચાલુ;

      ret = append_paths (&source, err, CGROUP_મૂળ, subsystem, ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = append_paths (&destination, err, source, subpath, ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = libcriu_wrapper->criu_add_ext_mount (source, destination);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", destination);
    }

  પાછા 0;
}

સ્થિર પૂર્ણાંક
checkpoint_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free અક્ષર *content = ખાલી;
  bool has_cgroup_mount = false;
  અક્ષર *saveptr = ખાલી;
  અક્ષર *from;
  પૂર્ણાંક ret;
  uint32_t i;

  /* First check if there is actually a cgroup mount in the container. */
  માટે (i = 0; i < def->mounts_len; i++)
    {
      અક્ષર *type = def->mounts[i]->type;
      જો (type && strcmp (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          તોડો;
        }
    }

  જો (! has_cgroup_mount)
    પાછા 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, ખાલી, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (UNLIKELY (content == ખાલી || content[0] == '\0'))
    પાછા crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  માટે (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ખાલી, "\n", &saveptr))
    {
      cleanup_free અક્ષર *source_path = ખાલી;
      અક્ષર *subsystem;

      subsystem = parse_cgroup_subsystem (from, ખાલી);
      જો (subsystem == ખાલી)
        ચાલુ;

      ret = append_paths (&source_path, err, CGROUP_મૂળ, subsystem, ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = libcriu_wrapper->criu_add_ext_mount (source_path, source_path);
      જો (UNLIKELY (ret < 0))
        પાછા crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", source_path);
    }

  પાછા 0;
}

સ્થિર પૂર્ણાંક
handle_criu_config_file (libcrun_container_t *container, libcrun_error_t *err)
{
  પૂર્ણાંક ret;
  અચલ અક્ષર *criu_config_annotation;
  અચલ અક્ષર *config_file = CRIU_RUNC_CONFIG_FILE;

  criu_config_annotation = find_annotation (container, "org.criu.config");

  /* Ignore missing criu_set_config_file() API for compatibility with older CRIU versions,
   * and show an error only if config file is explicitly set with annotation.
   */
  જો (libcriu_wrapper->criu_set_config_file == ખાલી)
    {
      જો (criu_config_annotation)
        પાછા crun_make_error (err, 0, "libcriu RPC config files supported in CRIU >= 4.2");

      પાછા 0;
    }

  જો (criu_config_annotation)
    config_file = criu_config_annotation;
  નહીંતર જો (access (CRIU_CRUN_CONFIG_FILE, F_OK) == 0)
    config_file = CRIU_CRUN_CONFIG_FILE;

  ret = libcriu_wrapper->criu_set_config_file (config_file);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, 0, "failed to set CRIU config file");

  પાછા 0;
}

સ્થિર પૂર્ણાંક
validate_criu_version (libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  // validate that the libcriu version is at least LIBCRIU_MIN_VERSION
  ret = libcriu_wrapper->criu_check_version (LIBCRIU_MIN_VERSION);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, -ret, "CRIU: failed checking version");

  જો (ret == 0)
    પાછા crun_make_error (err, 0, "libcriu is too old");

  પાછા 0;
}

સ્થિર શૂન્ય
show_criu_log (અચલ અક્ષર *work_path, અચલ અક્ષર *log)
{
  cleanup_free અક્ષર *log_path = ખાલી;
  libcrun_error_t tmp_err = ખાલી;
  અક્ષર લીટી[1024];
  FILE *f;

  જો (UNLIKELY (append_paths (&log_path, &tmp_err, work_path, log, ખાલી) < 0))
    {
      crun_error_release (&tmp_err);
      પાછા;
    }

  f = fopen (log_path, "r");
  જો (f == ખાલી)
    {
      જો (errno != ENOENT)
        libcrun_error (errno, "Can't open CRIU log `%s`", log_path);
      પાછા;
    }

  /* Log with error verbosity as this is the default. */
  libcrun_error (0, "--- excerpt from CRIU log `%s`", log_path);
  જ્યારે (fgets (લીટી, કદ (લીટી), f) != ખાલી)
    જો (strstr (લીટી, "Error ") != ખાલી)
      {
        લીટી[strcspn (લીટી, "\n")] = '\0';
        libcrun_error (0, "%s", લીટી);
      }

  fclose (f);
  libcrun_error (0, "--- end of excerpt");
}

/* work_dir is the place CRIU will put its logfiles.  If not explicitly set, CRIU
 * will put the logfiles into the images_dir.  When set, create and open it and
 * hand the fd to CRIU; ownership of the returned fd stays with the caller.  */
સ્થિર પૂર્ણાંક
setup_criu_work_dir (libcrun_checkpoint_restore_t *cr_options, પૂર્ણાંક *work_fd_out, libcrun_error_t *err)
{
  પૂર્ણાંક work_fd;

  જો (cr_options->work_path == ખાલી)
    {
      /* This is only for the error message later. */
      cr_options->work_path = cr_options->image_path;
      પાછા 0;
    }

  જો (UNLIKELY ((mkdir (cr_options->work_path, 0700) == -1) && (errno != EEXIST)))
    પાછા crun_make_error (err, errno, "error creating CRIU work directory `%s`", cr_options->work_path);

  work_fd = open (cr_options->work_path, O_DIRECTORY | O_CLOEXEC);
  જો (UNLIKELY (work_fd == -1))
    પાછા crun_make_error (err, errno, "error opening CRIU work directory `%s`", cr_options->work_path);

  libcriu_wrapper->criu_set_work_dir_fd (work_fd);
  *work_fd_out = work_fd;
  પાછા 0;
}

પૂર્ણાંક
libcrun_container_checkpoint_linux_criu (libcrun_container_status_t *status, libcrun_container_t *container,
                                         libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper સંરચના libcriu_wrapper_s *wrapper = ખાલી;
  cleanup_free અક્ષર *descriptors_path = ખાલી;
  cleanup_free અક્ષર *freezer_path = ખાલી;
  cleanup_free અક્ષર *path = ખાલી;
  cleanup_close પૂર્ણાંક image_fd = -1;
  cleanup_close પૂર્ણાંક work_fd = -1;
  પૂર્ણાંક cgroup_mode;
  size_t i;
  પૂર્ણાંક ret;

  ret = load_wrapper (&wrapper, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (geteuid ())
    પાછા crun_make_error (err, 0, "checkpointing requires root");

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
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_version (err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (UNLIKELY (cr_options->image_path == ખાલી))
    પાછા crun_make_error (err, 0, "image path not set");

  ret = mkdir (cr_options->image_path, 0700);
  જો (UNLIKELY ((ret == -1) && (errno != EEXIST)))
    પાછા crun_make_error (err, errno, "error creating checkpoint directory `%s`", cr_options->image_path);

  image_fd = open (cr_options->image_path, O_DIRECTORY | O_CLOEXEC);
  જો (UNLIKELY (image_fd == -1))
    પાછા crun_make_error (err, errno, "error opening checkpoint directory `%s`", cr_options->image_path);

  libcriu_wrapper->criu_set_images_dir_fd (image_fd);

  /* Set up logging. */
  libcriu_wrapper->criu_set_log_level (4);
  libcriu_wrapper->criu_set_log_file (CRIU_CHECKPOINT_LOG_FILE);

  /* Set up CRIU config file */
  જો (UNLIKELY (handle_criu_config_file (container, err)))
    પાછા -1;

  /* Setting the pid early as we can skip a lot of checkpoint setup if
   * we just do a pre-dump. The PID needs to be set always. Do it here.
   * The main process of the container is the process CRIU will checkpoint
   * and all of its children. */
  libcriu_wrapper->criu_set_pid (status->pid);

  ret = setup_criu_work_dir (cr_options, &work_fd, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

#  ifdef CRIU_PRE_DUMP_SUPPORT

  {
    પૂર્ણાંક criu_can_mem_track = 0;
    /* If the user uses --pre-dump for the second time or does
     * a final dump from a previous pre-dump, setting parent_path
     * is necessary so that CRIU can find which pages have not
     * changed compared to the previous dump. */
    જો (cr_options->parent_path != ખાલી)
      {
        criu_can_mem_track = criu_check_mem_track (err);
        જો (UNLIKELY (criu_can_mem_track == -1))
          પાછા -1;
        libcriu_wrapper->criu_set_track_mem (true);

        /* The parent path must be relative to image path (something like ../previous-dump).
           CRIU will fail with an unclear error message if the path is not right.
         */
        જો (UNLIKELY (cr_options->parent_path[0] == '/'))
          પાછા crun_make_error (err, 0, "--parent-path must be relative");
        પૂર્ણાંક is_dir = crun_dir_p_at (image_fd, cr_options->parent_path, false, err);
        જો (UNLIKELY (is_dir <= 0))
          {
            જો (is_dir < 0)
              પાછા crun_error_wrap (err, "invalid --parent-path");
            પાછા crun_make_error (err, ENOTDIR, "invalid --parent-path");
          }
        ret = libcriu_wrapper->criu_set_parent_images (cr_options->parent_path);
        જો (UNLIKELY (ret != 0))
          પાછા crun_make_error (err, -ret, "error setting CRIU parent images path to `%s`", cr_options->parent_path);
      }

    જો (cr_options->pre_dump)
      {
        જો (criu_can_mem_track != 1)
          {
            criu_can_mem_track = criu_check_mem_track (err);
            જો (UNLIKELY (criu_can_mem_track == -1))
              પાછા -1;
          }
        libcriu_wrapper->criu_set_track_mem (true);
        ret = libcriu_wrapper->criu_pre_dump ();
        જો (UNLIKELY (ret != 0))
          {
            show_criu_log (cr_options->work_path, CRIU_CHECKPOINT_LOG_FILE);
            પાછા crun_make_error (err, 0, "CRIU pre-dump failed: %d", ret);
          }
        પાછા 0;
      }
  }
#  endif

  /* descriptors.json is needed during restore to correctly
   * reconnect stdin, stdout, stderr. */
  ret = append_paths (&descriptors_path, err, cr_options->image_path, DESCRIPTORS_FILENAME, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = write_file (descriptors_path, status->external_descriptors, safe_strlen (status->external_descriptors), err);
  જો (UNLIKELY (ret < 0))
    પાછા crun_error_wrap (err, "error saving CRIU descriptors file");

  ret = append_paths (&path, err, status->bundle, status->rootfs, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = libcriu_wrapper->criu_set_મૂળ (path);
  જો (UNLIKELY (ret != 0))
    પાછા crun_make_error (err, 0, "error setting CRIU root to `%s`", path);

  cgroup_mode = libcrun_get_cgroup_mode (err);
  જો (UNLIKELY (cgroup_mode < 0))
    પાછા cgroup_mode;

  /* For cgroup v1 we need to tell CRIU to handle all cgroup mounts as external mounts. */
  જો (cgroup_mode != CGROUP_MODE_UNIFIED)
    {
      ret = checkpoint_cgroup_v1_mount (def, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  /* Tell CRIU about external bind mounts. */
  માટે (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      જો (is_bind_mount (def->mounts[i], ખાલી, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          અક્ષર buf[PATH_MAX];
          અચલ અક્ષર *dest_in_મૂળ;

          જો (nofollow)
            પાછા crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_in_મૂળ = chroot_realpath (status->rootfs, def->mounts[i]->destination, buf);
          જો (UNLIKELY (dest_in_મૂળ == ખાલી))
            પાછા crun_make_error (err, errno, "unable to resolve external bind mount `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          જો (has_prefix (dest_in_મૂળ, status->rootfs))
            dest_in_મૂળ += safe_strlen (status->rootfs);

          ret = libcriu_wrapper->criu_add_ext_mount (dest_in_મૂળ, dest_in_મૂળ);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", def->mounts[i]->destination);
        }
    }

  ret = register_masked_paths_mounts (def, container, libcriu_wrapper, false, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

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

  માટે (i = 0; i < def->linux->namespaces_len; i++)
    {
      પૂર્ણાંક કિંમત = libcrun_find_namespace (def->linux->namespaces[i]->type);
      જો (UNLIKELY (કિંમત < 0))
        પાછા crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      જો (કિંમત == CLONE_NEWNET && def->linux->namespaces[i]->path != ખાલી)
        {
          cleanup_free અક્ષર *external = ખાલી;
          સંરચના stat statbuf;

          ret = stat (def->linux->namespaces[i]->path, &statbuf);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "unable to stat(): `%s`", def->linux->namespaces[i]->path);

          xasprintf (&external, "net[%ld]:" CRIU_EXT_NETNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_add_external (external);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }

      જો (કિંમત == CLONE_NEWPID && def->linux->namespaces[i]->path != ખાલી)
        {
          cleanup_free અક્ષર *external = ખાલી;
          સંરચના stat statbuf;

          ret = stat (def->linux->namespaces[i]->path, &statbuf);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, errno, "unable to stat(): `%s`", def->linux->namespaces[i]->path);

          xasprintf (&external, "pid[%ld]:" CRIU_EXT_PIDNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_add_external (external);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }
    }

  /* Tell CRIU to use the freezer to pause all container processes. */
  જો (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      /* This needs CRIU 3.14. */
      ret = append_paths (&freezer_path, err, CGROUP_મૂળ, status->cgroup_path, ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }
  નહીંતર
    {
      ret = append_paths (&freezer_path, err, CGROUP_મૂળ "/freezer", status->cgroup_path, ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  ret = libcriu_wrapper->criu_set_freeze_cgroup (freezer_path);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, -ret, "CRIU: failed setting freezer %d", ret);

  /* Set boolean options . */
  libcriu_wrapper->criu_set_leave_running (cr_options->leave_running);
  libcriu_wrapper->criu_set_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_set_shell_job (cr_options->shell_job);
  libcriu_wrapper->criu_set_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_set_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_set_orphan_pts_master (true);
  libcriu_wrapper->criu_set_manage_cgroups_mode (criu_cg_mode_from_public (cr_options->manage_cgroups_mode));

  libcriu_wrapper->criu_set_manage_cgroups (true);

  જો (libcriu_wrapper->criu_set_network_lock)
    {
      પૂર્ણાંક lock = 0;

      ret = criu_network_lock_from_public (cr_options->network_lock_method, &lock, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      જો (lock > 0)
        {
          ret = libcriu_wrapper->criu_set_network_lock (lock);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, 0, "CRIU: failed setting network lock");
        }
    }

  ret = libcriu_wrapper->criu_dump ();
  જો (UNLIKELY (ret != 0))
    {
      show_criu_log (cr_options->work_path, CRIU_CHECKPOINT_LOG_FILE);
      પાછા crun_make_error (err, ret < 0 ? -ret : 0, "CRIU checkpointing failed: %d", ret);
    }

  પાછા 0;
}

સ્થિર પૂર્ણાંક
prepare_restore_mounts_internal (runtime_spec_schema_config_schema *def, અક્ષર *મૂળ, અચલ અક્ષર **mounted,
                                 size_t *n_mounted, libcrun_error_t *err)
{
  uint32_t i;

  /* Go through all mountpoints to be able to recreate missing mountpoints. */
  માટે (i = 0; i < def->mounts_len; i++)
    {
      અક્ષર *dest = def->mounts[i]->destination;
      અક્ષર *type = def->mounts[i]->type;
      cleanup_close પૂર્ણાંક મૂળ_fd = -1;
      bool nofollow = false;
      bool on_tmpfs = false;
      પૂર્ણાંક is_dir = 1;
      size_t j;

      /* cgroup restore should be handled by CRIU itself */
      જો (type && (strcmp (type, "cgroup") == 0 || strcmp (type, "cgroup2") == 0))
        ચાલુ;

      /* Check if the mountpoint is on a tmpfs. CRIU restores
       * all tmpfs. We do need to recreate directories on a tmpfs. */
      size_t dest_len = strlen (dest);
      માટે (j = 0; j < def->mounts_len; j++)
        {
          જો (def->mounts[j]->type == ખાલી || strcmp (def->mounts[j]->type, "tmpfs") != 0)
            ચાલુ;
          size_t mount_len = strlen (def->mounts[j]->destination);
          જો (mount_len < dest_len && dest[mount_len] == '/' && strncmp (dest, def->mounts[j]->destination, mount_len) == 0)
            {
              /* This is a mountpoint which is on a tmpfs.*/
              on_tmpfs = true;
              તોડો;
            }
        }

      જો (on_tmpfs)
        ચાલુ;

      /* For bind mounts check if the source is a file or a directory. */
      જો (is_bind_mount (def->mounts[i], ખાલી, &nofollow))
        {
          જો (nofollow)
            પાછા crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          is_dir = crun_dir_p (def->mounts[i]->source, false, err);
          જો (UNLIKELY (is_dir < 0))
            પાછા is_dir;
        }

      મૂળ_fd = open (મૂળ, O_RDONLY | O_CLOEXEC);
      જો (UNLIKELY (મૂળ_fd == -1))
        પાછા crun_make_error (err, errno, "error opening container root directory `%s`", મૂળ);

      જો (is_dir)
        {
          પૂર્ણાંક ret;

          ret = crun_safe_ensure_directory_at (મૂળ_fd, મૂળ, dest, 0755, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }
      નહીંતર
        {
          પૂર્ણાંક ret;

          ret = crun_safe_ensure_file_at (મૂળ_fd, મૂળ, dest, 0755, err);
          જો (UNLIKELY (ret < 0))
            પાછા ret;
        }

      /* Mount a bind mount source now, so that the mountpoints of the
       * mounts inside it (e.g. a nested bind mount) are created in the
       * source, where CRIU expects them.  This also happens during the
       * initial container creation, as the mounts are done in order.  */
      જો (is_bind_mount (def->mounts[i], ખાલી, &nofollow))
        {
          cleanup_close પૂર્ણાંક dst_fd = -1;
          proc_fd_path_t fd_path;

          dst_fd = safe_openat (મૂળ_fd, મૂળ, dest, O_PATH | O_CLOEXEC, 0, err);
          જો (UNLIKELY (dst_fd < 0))
            પાછા dst_fd;

          /* Not recursive: the source might contain ROOT itself (e.g. when
           * it is the bundle directory), and it is not needed anyway.  */
          get_proc_self_fd_path (fd_path, dst_fd);
          જો (UNLIKELY (mount (def->mounts[i]->source, fd_path, ખાલી, MS_BIND, ખાલી) < 0))
            પાછા crun_make_error (err, errno, "bind mount `%s` to `%s`", def->mounts[i]->source, dest);

          mounted[(*n_mounted)++] = dest;
        }
    }

  પાછા 0;
}

/* Move the current process back to CGROUPS, as read from /proc/self/cgroup.
   A failure is not fatal, so only warn about it.  */
સ્થિર શૂન્ય
move_back_to_cgroups (અચલ અક્ષર *cgroups)
{
  libcrun_error_t tmp_err = ખાલી;

  જો (UNLIKELY (libcrun_move_self_to_cgroups (cgroups, &tmp_err) < 0))
    {
      libcrun_warning ("cannot move back to the original cgroup: %s", tmp_err->સંદેશ);
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
સ્થિર પૂર્ણાંક
criu_restore_child_in_cgroup (અચલ અક્ષર *cgroup_path, પૂર્ણાંક *criu_ret, libcrun_error_t *err)
{
  sigset_t all_signals, old_mask;
  પૂર્ણાંક wait_status = 0;
  pid_t pid;
  પૂર્ણાંક ret;

  *criu_ret = -1;

  sigfillset (&all_signals);
  ret = sigprocmask (SIG_BLOCK, &all_signals, &old_mask);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "sigprocmask");

  /* Must be vfork: the child shares our memory space, so both *criu_ret and
     the error it creates are visible here once it is gone.  */
  pid = vfork ();
  જો (UNLIKELY (pid < 0))
    {
      પૂર્ણાંક saved_errno = errno;

      sigprocmask (SIG_SETMASK, &old_mask, ખાલી);
      પાછા crun_make_error (err, saved_errno, "vfork");
    }

  જો (pid == 0)
    {
      સંરચના sigaction act;
      પૂર્ણાંક i;

      /* The signal dispositions are not shared with the parent, so resetting
         them here does not affect it.  Keep the ignored signals ignored, as
         CRIU would inherit them if it was run directly by the caller.  */
      માટે (i = 1; i < NSIG; i++)
        {
          જો (sigaction (i, ખાલી, &act) < 0 || act.sa_handler == SIG_IGN || act.sa_handler == SIG_DFL)
            ચાલુ;

          memset (&act, 0, કદ (act));
          act.sa_handler = SIG_DFL;
          sigaction (i, &act, ખાલી);
        }
      sigprocmask (SIG_SETMASK, &old_mask, ખાલી);

      જો (! is_empty_string (cgroup_path))
        {
          ret = libcrun_move_process_to_cgroup (0, 0, cgroup_path, false, err);
          જો (UNLIKELY (ret < 0))
            _safe_exit (EXIT_FAILURE);
        }

      *criu_ret = libcriu_wrapper->criu_restore_child ();
      _safe_exit (EXIT_SUCCESS);
    }

  /* The child is gone by now, it is safe to handle signals again.  */
  sigprocmask (SIG_SETMASK, &old_mask, ખાલી);

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  જો (UNLIKELY (ret < 0))
    {
      /* The caller might have reaped the child already, e.g. when it ignores
         SIGCHLD.  Its result is still visible, so use it.  */
      જો (errno != ECHILD)
        પાછા crun_make_error (err, errno, "waitpid for the CRIU restore process");

      /* The child might have failed before calling CRIU, and left an error.  */
      પાછા *err != ખાલી ? -1 : 0;
    }

  ret = get_process_exit_status (wait_status);
  જો (UNLIKELY (ret != EXIT_SUCCESS))
    {
      /* The child creates the error in the shared memory space, do not
         overwrite it with crun_make_error().  */
      જો (*err == ખાલી)
        પાછા crun_make_error (err, 0, "the CRIU restore process exited with status %d", ret);
      પાછા -1;
    }

  પાછા 0;
}

/* Recreate the mountpoints which do not exist in the container rootfs
 * mounted at ROOT, the same way it is done on the container creation.  */
સ્થિર પૂર્ણાંક
prepare_restore_mounts (runtime_spec_schema_config_schema *def, અક્ષર *મૂળ, libcrun_error_t *err)
{
  cleanup_free અચલ અક્ષર **mounted = xmalloc0 (કદ (અક્ષર *) * (def->mounts_len + 1));
  cleanup_close પૂર્ણાંક મૂળ_fd = -1;
  size_t n_mounted = 0;
  પૂર્ણાંક ret;

  ret = prepare_restore_mounts_internal (def, મૂળ, mounted, &n_mounted, err);

  /* The bind mounts done above are only needed to create the mountpoints,
   * and CRIU restores the mounts itself, so undo them, in reverse order. */
  જો (n_mounted > 0)
    મૂળ_fd = open (મૂળ, O_PATH | O_CLOEXEC);
  જ્યારે (મૂળ_fd >= 0 && n_mounted > 0)
    {
      libcrun_error_t tmp_err = ખાલી;
      અચલ અક્ષર *dest = mounted[--n_mounted];
      cleanup_close પૂર્ણાંક dst_fd = -1;
      proc_fd_path_t fd_path;

      dst_fd = safe_openat (મૂળ_fd, મૂળ, dest, O_PATH | O_CLOEXEC, 0, &tmp_err);
      જો (UNLIKELY (dst_fd < 0))
        {
          crun_error_release (&tmp_err);
          ચાલુ;
        }
      get_proc_self_fd_path (fd_path, dst_fd);
      જો (UNLIKELY (umount2 (fd_path, MNT_DETACH) < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "unmount `%s`", dest);
    }

  પાછા ret;
}

પૂર્ણાંક
libcrun_container_restore_linux_criu (libcrun_container_status_t *status, libcrun_container_t *container,
                                      libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper સંરચના libcriu_wrapper_s *wrapper = ખાલી;
  cleanup_close પૂર્ણાંક inherit_new_net_fd = -1;
  cleanup_close પૂર્ણાંક inherit_new_pid_fd = -1;
  cleanup_close પૂર્ણાંક image_fd = -1;
  cleanup_free અક્ષર *મૂળ = ખાલી;
  cleanup_free અક્ષર *bundle_cleanup = ખાલી;
  cleanup_free અક્ષર *own_cgroups = ખાલી;
  cleanup_close પૂર્ણાંક work_fd = -1;
  પૂર્ણાંક cgroup_mode;
  પૂર્ણાંક criu_ret;
  પૂર્ણાંક ret_out;
  size_t i;
  પૂર્ણાંક ret;

  ret = load_wrapper (&wrapper, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (geteuid ())
    પાછા crun_make_error (err, 0, "restoring requires root");

  ret = libcriu_wrapper->criu_init_opts ();
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_version (err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (UNLIKELY (cr_options->image_path == ખાલી))
    પાછા crun_make_error (err, 0, "image path not set");

  image_fd = open (cr_options->image_path, O_DIRECTORY | O_CLOEXEC);
  જો (UNLIKELY (image_fd == -1))
    પાછા crun_make_error (err, errno, "error opening checkpoint directory `%s`", cr_options->image_path);

  libcriu_wrapper->criu_set_images_dir_fd (image_fd);

  /* Load descriptors.json to tell CRIU where those FDs should be connected to. */
  {
    cleanup_free અક્ષર *descriptors_path = ખાલી;
    cleanup_free અક્ષર *buffer = ખાલી;
    json_object *doc = ખાલી;

    ret = append_paths (&descriptors_path, err, cr_options->image_path, DESCRIPTORS_FILENAME, ખાલી);
    જો (UNLIKELY (ret < 0))
      પાછા ret;

    ret = read_all_file (descriptors_path, &buffer, ખાલી, err);
    જો (UNLIKELY (ret < 0))
      પાછા ret;

    /* descriptors.json contains a JSON array with strings
     * telling where 0, 1 and 2 have been initially been
     * pointing to. For each descriptor which points to
     * a pipe 'pipe:' we tell CRIU to reconnect that pipe
     * to the corresponding FD to have (especially) stdout
     * and stderr being correctly redirected. */
    ret = parse_json_file (&doc, buffer, ખાલી, err);
    જો (UNLIKELY (ret < 0))
      પાછા ret;

    જો (json_object_is_type (doc, json_type_array))
      {
        size_t i, len = json_object_array_length (doc);

        /* len will probably always be 3 as crun is currently only
         * recording the destination of FD 0, 1 and 2. */
        માટે (i = 0; i < len; ++i)
          {
            json_object *s = json_object_array_get_idx (doc, i);
            જો (s && json_object_is_type (s, json_type_string))
              {
                અચલ અક્ષર *str = json_object_get_string (s);
                જો (has_prefix (str, "pipe:"))
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
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = setup_criu_work_dir (cr_options, &work_fd, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (cr_options->lsm_profile != ખાલી)
    {
      ret = libcriu_wrapper->criu_set_lsm_profile (cr_options->lsm_profile);
      જો (UNLIKELY (ret != 0))
        પાછા crun_make_error (err, -ret, "error setting LSM profile to `%s`", cr_options->lsm_profile);
    }

  જો (cr_options->lsm_mount_context != ખાલી)
    {
      ret = libcriu_wrapper->criu_set_lsm_mount_context (cr_options->lsm_mount_context);
      જો (UNLIKELY (ret != 0))
        પાછા crun_make_error (err, -ret, "error setting LSM mount context to `%s`", cr_options->lsm_mount_context);
    }

  /* do realpath on root */
  bundle_cleanup = realpath (status->bundle, ખાલી);
  જો (UNLIKELY (bundle_cleanup == ખાલી))
    bundle_cleanup = xstrdup (status->bundle);

  /* Tell CRIU about external bind mounts. */
  માટે (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      જો (is_bind_mount (def->mounts[i], ખાલી, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          અક્ષર buf[PATH_MAX];
          અચલ અક્ષર *dest_in_મૂળ;
          અચલ અક્ષર *source = def->mounts[i]->source;
          cleanup_free અક્ષર *abs_source = ખાલી;
          unsigned long flags = 0;
          size_t j;

          જો (nofollow)
            પાછા crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_in_મૂળ = chroot_realpath (status->rootfs, def->mounts[i]->destination, buf);
          જો (UNLIKELY (dest_in_મૂળ == ખાલી))
            પાછા crun_make_error (err, errno, "unable to resolve external bind mount destination `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          જો (has_prefix (dest_in_મૂળ, status->rootfs))
            dest_in_મૂળ += safe_strlen (status->rootfs);

          /* A relative source is relative to the bundle, while CRIU would
             resolve it relative to its own working directory.  */
          જો (source && source[0] != '/')
            {
              ret = append_paths (&abs_source, err, bundle_cleanup, source, ખાલી);
              જો (UNLIKELY (ret < 0))
                પાછા ret;
              source = abs_source;
            }

          ret = libcriu_wrapper->criu_add_ext_mount (dest_in_મૂળ, source);
          જો (UNLIKELY (ret < 0))
            પાછા crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", source);

          માટે (j = 0; j < def->mounts[i]->options_len; j++)
            {
              અચલ સંરચના propagation_flags_s *prop = ખાલી;

              જો (def->mounts[i]->options[j])
                prop = libcrun_str2mount_flags (def->mounts[i]->options[j]);
              જો (prop && ! prop->clear)
                flags |= prop->flags;
            }
          add_restored_ext_mount (dest_in_મૂળ, flags);
        }
    }

  ret = register_masked_paths_mounts (def, container, libcriu_wrapper, true, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  /* Mount the container rootfs for CRIU. */
  ret = append_paths (&મૂળ, err, bundle_cleanup, "criu-root", ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = mkdir (મૂળ, 0755);
  જો (UNLIKELY (ret == -1))
    પાછા crun_make_error (err, errno, "error creating restore directory `%s`", મૂળ);

  ret = mount (status->rootfs, મૂળ, ખાલી, MS_BIND | MS_REC, ખાલી);
  જો (UNLIKELY (ret == -1))
    {
      ret = crun_make_error (err, errno, "error mounting restore directory `%s`", મૂળ);
      જાઓ out;
    }

  /* During initial container creation, crun will create mountpoints
   * defined in config.json if they do not exist. If we are restoring
   * we need to make sure these mountpoints also exist.
   * This is not perfect, as this means crun will modify a rootfs
   * even if it marked as read-only, but runc already modifies
   * the rootfs in the same way. */

  ret = prepare_restore_mounts (def, મૂળ, err);
  જો (UNLIKELY (ret < 0))
    જાઓ out_umount;

  ret = libcriu_wrapper->criu_set_મૂળ (મૂળ);
  જો (UNLIKELY (ret != 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU root to `%s`", મૂળ);
      જાઓ out_umount;
    }

  /* If a namespace defined in config.json we are telling
   * CRIU use that namespace when restoring the process tree.
   *
   * CRIU expects the information about the namespace like this:
   * --inherit-fd fd[<fd>]:<key>
   * The <key> needs to be the same as during checkpointing (extRootNetNS). */
  માટે (i = 0; i < def->linux->namespaces_len; i++)
    {
      અચલ પૂર્ણાંક open_flags_for_inherit = O_RDONLY; /* Cannot be O_CLOEXEC as it is passed to the child process. */
      પૂર્ણાંક કિંમત = libcrun_find_namespace (def->linux->namespaces[i]->type);
      જો (UNLIKELY (કિંમત < 0))
        {
          ret = crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);
          જાઓ out_umount;
        }

      જો (કિંમત == CLONE_NEWNET && def->linux->namespaces[i]->path != ખાલી)
        {
          inherit_new_net_fd = open (def->linux->namespaces[i]->path, open_flags_for_inherit);
          જો (UNLIKELY (inherit_new_net_fd < 0))
            {
              ret = crun_make_error (err, errno, "unable to open(): `%s`", def->linux->namespaces[i]->path);
              જાઓ out_umount;
            }

          ret = libcriu_wrapper->criu_add_inherit_fd (inherit_new_net_fd, CRIU_EXT_NETNS);
          જો (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              જાઓ out_umount;
            }
        }

      જો (કિંમત == CLONE_NEWPID && def->linux->namespaces[i]->path != ખાલી)
        {
          inherit_new_pid_fd = open (def->linux->namespaces[i]->path, open_flags_for_inherit);
          જો (UNLIKELY (inherit_new_pid_fd < 0))
            {
              ret = crun_make_error (err, errno, "unable to open(): `%s`", def->linux->namespaces[i]->path);
              જાઓ out_umount;
            }

          ret = libcriu_wrapper->criu_add_inherit_fd (inherit_new_pid_fd, CRIU_EXT_PIDNS);
          જો (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              જાઓ out_umount;
            }
        }

#  ifdef CRIU_JOIN_NS_SUPPORT
      જો (કિંમત == CLONE_NEWTIME && def->linux->namespaces[i]->path != ખાલી)
        {
          જો (libcriu_wrapper->criu_join_ns_add == ખાલી)
            {
              ret = crun_make_error (err, 0, "shared time namespace restore is supported in CRIU >= 3.16.1");
              જાઓ out_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_add ("time", def->linux->namespaces[i]->path, ખાલી);
          જો (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->path);
              જાઓ out_umount;
            }
        }

      જો (કિંમત == CLONE_NEWIPC && def->linux->namespaces[i]->path != ખાલી)
        {
          જો (libcriu_wrapper->criu_join_ns_add == ખાલી)
            {
              ret = crun_make_error (err, 0, "shared ipc namespace restore is supported in CRIU >= 3.16.1");
              જાઓ out_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_add ("ipc", def->linux->namespaces[i]->path, ખાલી);
          જો (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->path);
              જાઓ out_umount;
            }
        }

      જો (કિંમત == CLONE_NEWUTS && def->linux->namespaces[i]->path != ખાલી)
        {
          જો (libcriu_wrapper->criu_join_ns_add == ખાલી)
            {
              ret = crun_make_error (err, 0, "shared uts namespace restore is supported in CRIU >= 3.16.1");
              જાઓ out_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_add ("uts", def->linux->namespaces[i]->path, ખાલી);
          જો (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->path);
              જાઓ out_umount;
            }
        }
#  endif
    }

  /* Set up CRIU config file */
  ret = handle_criu_config_file (container, err);
  જો (UNLIKELY (ret < 0))
    જાઓ out_umount;

  /* Tell CRIU if cgroup v1 needs to be handled. */
  ret = restore_cgroup_v1_mount (def, err);
  જો (UNLIKELY (ret < 0))
    જાઓ out_umount;

  console_socket = cr_options->console_socket;
  libcriu_wrapper->criu_set_notify_cb (criu_notify);

  /* Set boolean options . */
  libcriu_wrapper->criu_set_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_set_shell_job (cr_options->shell_job);
  libcriu_wrapper->criu_set_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_set_tcp_close (cr_options->tcp_close);
  libcriu_wrapper->criu_set_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_set_orphan_pts_master (true);

  જો (status->cgroup_path)
    {
      cgroup_mode = libcrun_get_cgroup_mode (err);
      જો (UNLIKELY (cgroup_mode < 0))
        {
          ret = cgroup_mode;
          જાઓ out_umount;
        }

      /* With cgroup v2, the cgroup path is only meaningful for the unified
         hierarchy, so only relocate that one.  A NULL controller would make
         CRIU relocate every hierarchy the container is in, named cgroup v1
         ones included.  As CRIU dumps such a hierarchy from wherever the
         container is in it, which is its root, as crun leaves named
         hierarchies alone, every checkpoint and restore would then copy the
         whole hierarchy into itself.  */
      ret = libcriu_wrapper->criu_add_cg_મૂળ (cgroup_mode == CGROUP_MODE_UNIFIED ? "" : ખાલી,
                                               status->cgroup_path);
      જો (UNLIKELY (ret != 0))
        {
          ret = crun_make_error (err, 0, "error setting CRIU cgroup root to `%s`", status->cgroup_path);
          જાઓ out_umount;
        }
    }

  libcriu_wrapper->criu_set_manage_cgroups_mode (criu_cg_mode_from_public (cr_options->manage_cgroups_mode));
  libcriu_wrapper->criu_set_manage_cgroups (true);

  જો (libcriu_wrapper->criu_set_network_lock)
    {
      પૂર્ણાંક lock = 0;

      ret = criu_network_lock_from_public (cr_options->network_lock_method, &lock, err);
      જો (UNLIKELY (ret < 0))
        જાઓ out_umount;

      જો (lock > 0)
        {
          ret = libcriu_wrapper->criu_set_network_lock (lock);
          જો (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "CRIU: failed setting network lock");
              જાઓ out_umount;
            }
        }
    }

  libcriu_wrapper->criu_set_log_level (4);
  ret = libcriu_wrapper->criu_set_log_file (CRIU_RESTORE_LOG_FILE);
  જો (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU log file to `%s`", CRIU_RESTORE_LOG_FILE);
      જાઓ out_umount;
    }

  /* criu_restore() returns the PID of the process of the restored process
   * tree. This PID will not be the same as status->pid if the container is
   * running in a PID namespace. But it will always be > 0. */
  જો (cr_options->detach)
    {
      /* Nothing waits for the container, so there is no reason to take over
         this process: use a throw-away one.  It is used even when there is no
         cgroup to join, so that the container is not left as a child of a
         process which is not going to reap it.  */
      ret = criu_restore_child_in_cgroup (status->cgroup_path, &criu_ret, err);
      જો (UNLIKELY (ret < 0))
        જાઓ out_umount;

      ret = criu_ret;
    }
  નહીંતર
    {
      /* Like runc does, put CRIU into the container cgroup for the time of
       * restore, so the restored tasks are created there. It is necessary in
       * the "ignore" mode, where CRIU does not deal with cgroups at all, and
       * does not hurt otherwise. As CRIU is our child, do it by moving
       * ourselves there, and move back once the restore is done. */
      જો (! is_empty_string (status->cgroup_path))
        {
          ret = read_all_file (PROC_SELF_CGROUP, &own_cgroups, ખાલી, err);
          જો (UNLIKELY (ret < 0))
            જાઓ out_umount;

          ret = libcrun_move_process_to_cgroup (0, 0, status->cgroup_path, false, err);
          જો (UNLIKELY (ret < 0))
            {
              /* Some of the cgroups might have been joined already.  */
              move_back_to_cgroups (own_cgroups);
              જાઓ out_umount;
            }
        }

      ret = libcriu_wrapper->criu_restore_child ();

      જો (own_cgroups)
        move_back_to_cgroups (own_cgroups);
    }

  જો (UNLIKELY (ret <= 0))
    {
      show_criu_log (cr_options->work_path, CRIU_RESTORE_LOG_FILE);
      ret = crun_make_error (err, 0, "CRIU restoring failed: %d", ret);
      જાઓ out_umount;
    }

  /* Update the status struct with the newly allocated PID. This will
   * be necessary later when moving the process into its cgroup. */
  status->pid = ret;
  ret = 0;

out_umount:
  ret_out = umount (મૂળ);
  જો (UNLIKELY (ret_out == -1))
    {
      પૂર્ણાંક saved_errno = errno;
      rmdir (મૂળ);
      જો (ret < 0)
        પાછા crun_error_wrap (err, "error unmounting restore directory `%s`", મૂળ);
      નહીંતર
        પાછા crun_make_error (err, saved_errno, "error unmounting restore directory `%s`", મૂળ);
    }
out:
  ret_out = rmdir (મૂળ);
  જો (UNLIKELY (ret < 0))
    પાછા ret;
  જો (UNLIKELY (ret_out == -1))
    પાછા crun_make_error (err, errno, "error removing restore directory `%s`", મૂળ);
  પાછા ret;
}
#endif
