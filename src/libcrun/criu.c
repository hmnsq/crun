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

#include <config.h>

#නම් HAVE_CRIU && HAVE_DLOPEN

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
var_na *chroot_realpath (const var_na *chroot, const var_na *path, var_na resolved_path[]);

/* Map the public cgroups mode (enum libcrun_cr_cgroups_mode) onto the CRIU enum.  */
static ga_nanaa criu_cg_mode
criu_cg_mode_from_public (පූර්ණ_සංඛ්‍යා mode)
{
  switch (mode)
    {
    pak_sa LIBCRUN_CR_CG_MODE_IGNORE:
      ආපසු CRIU_CG_MODE_IGNORE;
    pak_sa LIBCRUN_CR_CG_MODE_FULL:
      ආපසු CRIU_CG_MODE_FULL;
    pak_sa LIBCRUN_CR_CG_MODE_STRICT:
      ආපසු CRIU_CG_MODE_STRICT;
    pak_sa LIBCRUN_CR_CG_MODE_DEFAULT:
    pak_sa LIBCRUN_CR_CG_MODE_SOFT:
    default:
      /* Default to CRIU_CG_MODE_SOFT, just as runc.  */
      ආපසු CRIU_CG_MODE_SOFT;
    }
}

/* Map the public network lock method (enum libcrun_cr_network_lock_method) onto the
   CRIU enum.  *LOCK is set to 0 when no explicit method must be set (CRIU default).
   A method that this build cannot honor is an error, never a silent fallback to
   the CRIU default.  */
static පූර්ණ_සංඛ්‍යා
criu_network_lock_from_public (පූර්ණ_සංඛ්‍යා method, පූර්ණ_සංඛ්‍යා *lock, libcrun_error_t *err)
{
  switch (method)
    {
    pak_sa LIBCRUN_CR_NETWORK_LOCK_IPTABLES:
      *lock = CRIU_NETWORK_LOCK_IPTABLES;
      ආපසු 0;
    pak_sa LIBCRUN_CR_NETWORK_LOCK_NFTABLES:
      *lock = CRIU_NETWORK_LOCK_NFTABLES;
      ආපසු 0;
    pak_sa LIBCRUN_CR_NETWORK_LOCK_SKIP:
#  නම් CRIU_NETWORK_LOCK_SKIP_SUPPORT
      *lock = CRIU_NETWORK_LOCK_SKIP;
      ආපසු 0;
#  නැතහොත්
      ආපසු crun_make_error (err, 0, "CRIU: the `skip` network lock method is not supported by this build");
#  endif
    pak_sa LIBCRUN_CR_NETWORK_LOCK_DEFAULT:
      *lock = 0;
      ආපසු 0;
    default:
      ආපසු crun_make_error (err, 0, "CRIU: unknown network lock method %d", method);
    }
}

static const var_na *console_socket = NULL;

/* The external mounts CRIU restores, and the flags the configuration asks
   for each of them.  */
static sa_nracanaa libcrun_mount_flags_s *restored_ext_mounts = NULL;
static size_t n_restored_ext_mounts = 0;

#  define LIBCRIU_MIN_VERSION 31500

sa_nracanaa libcriu_wrapper_s
{
  void *handle;
  පූර්ණ_සංඛ්‍යා (*criu_add_ext_mount) (const var_na *key, const var_na *val);
  පූර්ණ_සංඛ්‍යා (*criu_add_external) (const var_na *key);
  පූර්ණ_සංඛ්‍යා (*criu_add_inherit_fd) (පූර්ණ_සංඛ්‍යා fd, const var_na *key);
  පූර්ණ_සංඛ්‍යා (*criu_check_version) (පූර්ණ_සංඛ්‍යා minimum);
  පූර්ණ_සංඛ්‍යා (*criu_dump) (void);
  පූර්ණ_සංඛ්‍යා (*criu_get_orphan_pts_master_fd) (void);
  පූර්ණ_සංඛ්‍යා (*criu_init_opts) (void);
  පූර්ණ_සංඛ්‍යා (*criu_notify_pid) (criu_notify_arg_t na);
#  ifdef CRIU_JOIN_NS_SUPPORT
  පූර්ණ_සංඛ්‍යා (*criu_join_ns_add) (const var_na *ns, const var_na *ns_file, const var_na *extra_opt);
#  endif
#  ifdef CRIU_PRE_DUMP_SUPPORT
  පූර්ණ_සංඛ්‍යා (*criu_feature_check) (sa_nracanaa criu_feature_check *features, size_t size);
  පූර්ණ_සංඛ්‍යා (*criu_pre_dump) (void);
#  endif
  පූර්ණ_සංඛ්‍යා (*criu_restore_child) (void);
  පූර්ණ_සංඛ්‍යා (*criu_set_freeze_cgroup) (const var_na *name);
  void (*criu_set_file_locks) (bool file_locks);
  void (*criu_set_ext_unix_sk) (bool ext_unix_sk);
  පූර්ණ_සංඛ්‍යා (*criu_set_log_file) (const var_na *log_file);
  void (*criu_set_log_level) (පූර්ණ_සංඛ්‍යා log_level);
  void (*criu_set_leave_running) (bool leave_running);
  void (*criu_set_manage_cgroups) (bool manage);
  void (*criu_set_manage_cgroups_mode) (ga_nanaa criu_cg_mode mode);
  පූර්ණ_සංඛ්‍යා (*criu_set_network_lock) (ga_nanaa criu_network_lock_method method);
  void (*criu_set_notify_cb) (පූර්ණ_සංඛ්‍යා (*cb) (var_na *action, criu_notify_arg_t na));
  void (*criu_set_orphan_pts_master) (bool orphan_pts_master);
  void (*criu_set_images_dir_fd) (පූර්ණ_සංඛ්‍යා fd);
  පූර්ණ_සංඛ්‍යා (*criu_set_parent_images) (const var_na *path);
  void (*criu_set_pid) (පූර්ණ_සංඛ්‍යා pid);
  පූර්ණ_සංඛ්‍යා (*criu_set_root) (const var_na *root);
  පූර්ණ_සංඛ්‍යා (*criu_add_cg_root) (const var_na *ctrl, const var_na *path);
  void (*criu_set_shell_job) (bool shell_job);
  void (*criu_set_tcp_established) (bool tcp_established);
  void (*criu_set_tcp_close) (bool tcp_close);
  void (*criu_set_track_mem) (bool track_mem);
  void (*criu_set_work_dir_fd) (පූර්ණ_සංඛ්‍යා fd);
  පූර්ණ_සංඛ්‍යා (*criu_set_lsm_profile) (const var_na *name);
  පූර්ණ_සංඛ්‍යා (*criu_set_lsm_mount_context) (const var_na *name);
  පූර්ණ_සංඛ්‍යා (*criu_set_config_file) (const var_na *path);
};

static sa_nracanaa libcriu_wrapper_s *libcriu_wrapper;

static anta_hstha void
cleanup_wrapper (void *p)
{
  sa_nracanaa libcriu_wrapper_s **w;

  w = (sa_nracanaa libcriu_wrapper_s **) p;
  නම් (*w == NULL)
    ආපසු;

#  ifndef STATIC
  නම් ((*w)->handle)
    dlclose ((*w)->handle);
#  endif
  free (*w);
  libcriu_wrapper = NULL;

  අතරතුර (n_restored_ext_mounts > 0)
    free (restored_ext_mounts[--n_restored_ext_mounts].target);
  free (restored_ext_mounts);
  restored_ext_mounts = NULL;
}

#  define cleanup_wrapper __attribute__ ((cleanup (cleanup_wrapper)))

static පූර්ණ_සංඛ්‍යා
load_wrapper (sa_nracanaa libcriu_wrapper_s **wrapper_out, libcrun_error_t *err)
{
  cleanup_free sa_nracanaa libcriu_wrapper_s *wrapper = xmalloc0 (parimaa_na (*wrapper));

#  ifdef STATIC
#    define LOAD_CRIU_FUNCTION(X, ALLOW_NULL) \
      wrapper->X = &X;
#  නැතහොත්
#    define LOAD_CRIU_FUNCTION(X, ALLOW_NULL)                                                    \
      do                                                                                         \
        {                                                                                        \
          wrapper->X = dlsym (wrapper->handle, #X);                                              \
          නම් (! ALLOW_NULL && wrapper->X == NULL)                                                \
            {                                                                                    \
              dlclose (wrapper->handle);                                                         \
              ආපසු crun_make_error (err, 0, "could not find symbol `%s` in `libcriu.so`", #X); \
            }                                                                                    \
      } අතරතුර (0)
#  endif

#  ifndef STATIC
  wrapper->handle = dlopen ("libcriu.so.2", RTLD_NOW);
  නම් (wrapper->handle == NULL)
    ආපසු crun_make_error (err, 0, "could not load `libcriu.so.2`: `%s`", dlerror ());
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
  LOAD_CRIU_FUNCTION (criu_set_root, false);
  LOAD_CRIU_FUNCTION (criu_add_cg_root, false);
  LOAD_CRIU_FUNCTION (criu_set_shell_job, false);
  LOAD_CRIU_FUNCTION (criu_set_tcp_established, false);
  LOAD_CRIU_FUNCTION (criu_set_tcp_close, false);
  LOAD_CRIU_FUNCTION (criu_set_track_mem, false);
  LOAD_CRIU_FUNCTION (criu_set_work_dir_fd, false);
  LOAD_CRIU_FUNCTION (criu_set_lsm_profile, false);
  LOAD_CRIU_FUNCTION (criu_set_lsm_mount_context, false);
#  නම් ! defined STATIC || defined CRIU_CONFIG_FILE
  LOAD_CRIU_FUNCTION (criu_set_config_file, true);
#  endif

  libcriu_wrapper = *wrapper_out = wrapper;
  wrapper = NULL;
#  undef LOAD_CRIU_FUNCTION
  ආපසු 0;
}

static void
add_restored_ext_mount (const var_na *target, unsigned long flags)
{
  restored_ext_mounts = xrealloc (restored_ext_mounts, parimaa_na (*restored_ext_mounts) * (n_restored_ext_mounts + 1));
  restored_ext_mounts[n_restored_ext_mounts].target = xstrdup (target);
  restored_ext_mounts[n_restored_ext_mounts].flags = flags;
  n_restored_ext_mounts++;
}

static පූර්ණ_සංඛ්‍යා
criu_notify (var_na *action, criu_notify_arg_t na)
{
  නම් (strcmp (action, "post-restore") == 0)
    {
      /* CRIU takes the source of an external mount from us and its flags
       * from the image, i.e. from whatever mount was at that path in the
       * container when it was checkpointed.  The container can move its
       * mount points around, so before it is resumed make sure that each
       * source is mounted with the restrictions the configuration asks for. */
      libcrun_error_t tmp_err = NULL;
      පූර්ණ_සංඛ්‍යා ret;

      ret = libcrun_restrict_mounts_in_container (libcriu_wrapper->criu_notify_pid (na), restored_ext_mounts,
                                                  n_restored_ext_mounts, &tmp_err);
      නම් (UNLIKELY (ret < 0))
        {
          libcrun_error (tmp_err->status, "cannot set the flags of the restored mounts: %s", tmp_err->msg);
          libcrun_error_release (&tmp_err);
          ආපසු ret;
        }
    }

  නම් (strncmp (action, "orphan-pts-master", 17) == 0)
    {
      /* CRIU sends us the master FD via the 'orphan-pts-master'
       * callback and we are passing it on to the '--console-socket'
       * if it exists. */
      cleanup_close පූර්ණ_සංඛ්‍යා console_socket_fd = -1;
      libcrun_error_t tmp_err = NULL;
      පූර්ණ_සංඛ්‍යා master_fd;
      පූර්ණ_සංඛ්‍යා ret;

      නම් (! console_socket)
        ආපසු 0;

      master_fd = libcriu_wrapper->criu_get_orphan_pts_master_fd ();

      console_socket_fd = open_unix_domain_client_socket (console_socket, 0, &tmp_err);
      නම් (UNLIKELY (console_socket_fd < 0))
        {
          libcrun_error_release (&tmp_err);
          ආපසු console_socket_fd;
        }
      ret = send_fd_to_socket (console_socket_fd, master_fd, &tmp_err);
      නම් (UNLIKELY (ret < 0))
        {
          libcrun_error_release (&tmp_err);
          ආපසු ret;
        }
    }
  ආපසු 0;
}

#  ifdef CRIU_PRE_DUMP_SUPPORT

static පූර්ණ_සංඛ්‍යා
criu_check_mem_track (libcrun_error_t *err)
{
  sa_nracanaa criu_feature_check features = { 0 };
  පූර්ණ_සංඛ්‍යා ret;

  /* Right now we are only interested in checking memory tracking.
   * Memory tracking can be disabled at different levels. aarch64
   * for example has memory tracking not implemented. It could also
   * be not enabled on other architectures. Just ask CRIU if that
   * features exists. */

  features.mem_track = true;

  ret = libcriu_wrapper->criu_feature_check (&features, parimaa_na (features));
  නම් (UNLIKELY (ret < 0))
    ආපසු crun_make_error (err, 0, "CRIU feature checking failed: %d", ret);

  නම් (features.mem_track == true)
    ආපසු 1;

  ආපසු crun_make_error (err, 0, "CRIU memory tracking not supported");
}

#  endif

static පූර්ණ_සංඛ්‍යා
register_masked_paths_mounts (runtime_spec_schema_config_schema *def, libcrun_container_t *container,
                              sa_nracanaa libcriu_wrapper_s *libcriu_wrapper, bool is_restore, libcrun_error_t *err)
{
  cleanup_free var_na *empty_dir_path = NULL;
  bool shared_dir_registered = false;
  size_t i;
  පූර්ණ_සංඛ්‍යා ret;

  සඳහා (i = 0; i < def->linux->masked_paths_len; i++)
    {
      sa_nracanaa stat statbuf;
      ret = stat (def->linux->masked_paths[i], &statbuf);
      නම් (ret != 0)
        continue;

      නම් (S_ISDIR (statbuf.st_mode))
        {
          නම් (! shared_dir_registered)
            {
              ret = get_shared_empty_directory_path (&empty_dir_path,
                                                     (container->context ? container->context->state_root : NULL), err);
              නම් (UNLIKELY (ret < 0))
                ආපසු ret;

              ret = libcriu_wrapper->criu_add_ext_mount (empty_dir_path, empty_dir_path);
              නම් (UNLIKELY (ret < 0))
                ආපසු crun_make_error (err, -ret, "CRIU: failed adding external mount for shared empty directory `%s`", empty_dir_path);

              shared_dir_registered = true;
            }

          ret = libcriu_wrapper->criu_add_ext_mount (def->linux->masked_paths[i], empty_dir_path);
          නම් (UNLIKELY (ret < 0))
            ආපසු crun_make_error (err, -ret, "CRIU: failed adding external mount for masked directory `%s`", def->linux->masked_paths[i]);

          නම් (is_restore)
            add_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
      නැතහොත් නම් (S_ISREG (statbuf.st_mode))
        {
          const var_na *bind_target = is_restore ? "/dev/null" : def->linux->masked_paths[i];
          ret = libcriu_wrapper->criu_add_ext_mount (def->linux->masked_paths[i], bind_target);
          නම් (UNLIKELY (ret < 0))
            ආපසු crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", bind_target);

          නම් (is_restore)
            add_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
    }

  ආපසු 0;
}

/* Parse one /proc/self/cgroup LINE into its subsystem, normalizing the name and
   stripping any "name=" prefix.  When SUBPATH_OUT is not NULL, the in-hierarchy
   path is stored there.  Returns the subsystem, or NULL if the line carries no
   controller and should be skipped.  LINE is modified in place.  */
static var_na *
parse_cgroup_subsystem (var_na *line, var_na **subpath_out)
{
  var_na *subsystem, *subpath, *it;

  subsystem = strchr (line, ':') + 1;
  subpath = strchr (subsystem, ':') + 1;
  *(subpath - 1) = '\0';

  නම් (subsystem[0] == '\0')
    ආපසු NULL;

  it = strstr (subsystem, "name=");
  නම් (it)
    subsystem = it + 5;

  නම් (strcmp (subsystem, "net_prio,net_cls") == 0)
    subsystem = "net_cls,net_prio";
  නම් (strcmp (subsystem, "cpuacct,cpu") == 0)
    subsystem = "cpu,cpuacct";

  නම් (subpath_out)
    *subpath_out = subpath;
  ආපසු subsystem;
}

static පූර්ණ_සංඛ්‍යා
restore_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free var_na *content = NULL;
  bool has_cgroup_mount = false;
  var_na *saveptr = NULL;
  පූර්ණ_සංඛ්‍යා cgroup_mode;
  var_na *from;
  පූර්ණ_සංඛ්‍යා ret;
  uint32_t i;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  නම් (UNLIKELY (cgroup_mode < 0))
    ආපසු cgroup_mode;

  නම් (cgroup_mode == CGROUP_MODE_UNIFIED)
    ආපසු 0;

  /* First check if there is actually a cgroup mount in the container. */
  සඳහා (i = 0; i < def->mounts_len; i++)
    {
      var_na *type = def->mounts[i]->type;
      නම් (type && strcmp (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          bha_nga;
        }
    }

  නම් (! has_cgroup_mount)
    ආපසු 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, NULL, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  නම් (UNLIKELY (content == NULL || content[0] == '\0'))
    ආපසු crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  සඳහා (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (NULL, "\n", &saveptr))
    {
      cleanup_free var_na *destination = NULL;
      cleanup_free var_na *source = NULL;
      var_na *subsystem;
      var_na *subpath;

      subsystem = parse_cgroup_subsystem (from, &subpath);
      නම් (subsystem == NULL)
        continue;

      ret = append_paths (&source, err, CGROUP_ROOT, subsystem, NULL);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;

      ret = append_paths (&destination, err, source, subpath, NULL);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;

      ret = libcriu_wrapper->criu_add_ext_mount (source, destination);
      නම් (UNLIKELY (ret < 0))
        ආපසු crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", destination);
    }

  ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා
checkpoint_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free var_na *content = NULL;
  bool has_cgroup_mount = false;
  var_na *saveptr = NULL;
  var_na *from;
  පූර්ණ_සංඛ්‍යා ret;
  uint32_t i;

  /* First check if there is actually a cgroup mount in the container. */
  සඳහා (i = 0; i < def->mounts_len; i++)
    {
      var_na *type = def->mounts[i]->type;
      නම් (type && strcmp (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          bha_nga;
        }
    }

  නම් (! has_cgroup_mount)
    ආපසු 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, NULL, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  නම් (UNLIKELY (content == NULL || content[0] == '\0'))
    ආපසු crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  සඳහා (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (NULL, "\n", &saveptr))
    {
      cleanup_free var_na *source_path = NULL;
      var_na *subsystem;

      subsystem = parse_cgroup_subsystem (from, NULL);
      නම් (subsystem == NULL)
        continue;

      ret = append_paths (&source_path, err, CGROUP_ROOT, subsystem, NULL);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;

      ret = libcriu_wrapper->criu_add_ext_mount (source_path, source_path);
      නම් (UNLIKELY (ret < 0))
        ආපසු crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", source_path);
    }

  ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා
handle_criu_config_file (libcrun_container_t *container, libcrun_error_t *err)
{
  පූර්ණ_සංඛ්‍යා ret;
  const var_na *criu_config_annotation;
  const var_na *config_file = CRIU_RUNC_CONFIG_FILE;

  criu_config_annotation = find_annotation (container, "org.criu.config");

  /* Ignore missing criu_set_config_file() API for compatibility with older CRIU versions,
   * and show an error only if config file is explicitly set with annotation.
   */
  නම් (libcriu_wrapper->criu_set_config_file == NULL)
    {
      නම් (criu_config_annotation)
        ආපසු crun_make_error (err, 0, "libcriu RPC config files supported in CRIU >= 4.2");

      ආපසු 0;
    }

  නම් (criu_config_annotation)
    config_file = criu_config_annotation;
  නැතහොත් නම් (access (CRIU_CRUN_CONFIG_FILE, F_OK) == 0)
    config_file = CRIU_CRUN_CONFIG_FILE;

  ret = libcriu_wrapper->criu_set_config_file (config_file);
  නම් (UNLIKELY (ret < 0))
    ආපසු crun_make_error (err, 0, "failed to set CRIU config file");

  ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා
validate_criu_version (libcrun_error_t *err)
{
  පූර්ණ_සංඛ්‍යා ret;

  // validate that the libcriu version is at least LIBCRIU_MIN_VERSION
  ret = libcriu_wrapper->criu_check_version (LIBCRIU_MIN_VERSION);
  නම් (UNLIKELY (ret < 0))
    ආපසු crun_make_error (err, -ret, "CRIU: failed checking version");

  නම් (ret == 0)
    ආපසු crun_make_error (err, 0, "libcriu is too old");

  ආපසු 0;
}

static void
show_criu_log (const var_na *work_path, const var_na *log)
{
  cleanup_free var_na *log_path = NULL;
  libcrun_error_t tmp_err = NULL;
  var_na line[1024];
  FILE *f;

  නම් (UNLIKELY (append_paths (&log_path, &tmp_err, work_path, log, NULL) < 0))
    {
      crun_error_release (&tmp_err);
      ආපසු;
    }

  f = fopen (log_path, "r");
  නම් (f == NULL)
    {
      නම් (errno != ENOENT)
        libcrun_error (errno, "Can't open CRIU log `%s`", log_path);
      ආපසු;
    }

  /* Log with error verbosity as this is the default. */
  libcrun_error (0, "--- excerpt from CRIU log `%s`", log_path);
  අතරතුර (fgets (line, parimaa_na (line), f) != NULL)
    නම් (strstr (line, "Error ") != NULL)
      {
        line[strcspn (line, "\n")] = '\0';
        libcrun_error (0, "%s", line);
      }

  fclose (f);
  libcrun_error (0, "--- end of excerpt");
}

/* work_dir is the place CRIU will put its logfiles.  If not explicitly set, CRIU
 * will put the logfiles into the images_dir.  When set, create and open it and
 * hand the fd to CRIU; ownership of the returned fd stays with the caller.  */
static පූර්ණ_සංඛ්‍යා
setup_criu_work_dir (libcrun_checkpoint_restore_t *cr_options, පූර්ණ_සංඛ්‍යා *work_fd_out, libcrun_error_t *err)
{
  පූර්ණ_සංඛ්‍යා work_fd;

  නම් (cr_options->work_path == NULL)
    {
      /* This is only for the error message later. */
      cr_options->work_path = cr_options->image_path;
      ආපසු 0;
    }

  නම් (UNLIKELY ((mkdir (cr_options->work_path, 0700) == -1) && (errno != EEXIST)))
    ආපසු crun_make_error (err, errno, "error creating CRIU work directory `%s`", cr_options->work_path);

  work_fd = open (cr_options->work_path, O_DIRECTORY | O_CLOEXEC);
  නම් (UNLIKELY (work_fd == -1))
    ආපසු crun_make_error (err, errno, "error opening CRIU work directory `%s`", cr_options->work_path);

  libcriu_wrapper->criu_set_work_dir_fd (work_fd);
  *work_fd_out = work_fd;
  ආපසු 0;
}

පූර්ණ_සංඛ්‍යා
libcrun_container_checkpoint_linux_criu (libcrun_container_status_t *status, libcrun_container_t *container,
                                         libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper sa_nracanaa libcriu_wrapper_s *wrapper = NULL;
  cleanup_free var_na *descriptors_path = NULL;
  cleanup_free var_na *freezer_path = NULL;
  cleanup_free var_na *path = NULL;
  cleanup_close පූර්ණ_සංඛ්‍යා image_fd = -1;
  cleanup_close පූර්ණ_සංඛ්‍යා work_fd = -1;
  පූර්ණ_සංඛ්‍යා cgroup_mode;
  size_t i;
  පූර්ණ_සංඛ්‍යා ret;

  ret = load_wrapper (&wrapper, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  නම් (geteuid ())
    ආපසු crun_make_error (err, 0, "checkpointing requires root");

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
  නම් (UNLIKELY (ret < 0))
    ආපසු crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_version (err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  නම් (UNLIKELY (cr_options->image_path == NULL))
    ආපසු crun_make_error (err, 0, "image path not set");

  ret = mkdir (cr_options->image_path, 0700);
  නම් (UNLIKELY ((ret == -1) && (errno != EEXIST)))
    ආපසු crun_make_error (err, errno, "error creating checkpoint directory `%s`", cr_options->image_path);

  image_fd = open (cr_options->image_path, O_DIRECTORY | O_CLOEXEC);
  නම් (UNLIKELY (image_fd == -1))
    ආපසු crun_make_error (err, errno, "error opening checkpoint directory `%s`", cr_options->image_path);

  libcriu_wrapper->criu_set_images_dir_fd (image_fd);

  /* Set up logging. */
  libcriu_wrapper->criu_set_log_level (4);
  libcriu_wrapper->criu_set_log_file (CRIU_CHECKPOINT_LOG_FILE);

  /* Set up CRIU config file */
  නම් (UNLIKELY (handle_criu_config_file (container, err)))
    ආපසු -1;

  /* Setting the pid early as we can skip a lot of checkpoint setup if
   * we just do a pre-dump. The PID needs to be set always. Do it here.
   * The main process of the container is the process CRIU will checkpoint
   * and all of its children. */
  libcriu_wrapper->criu_set_pid (status->pid);

  ret = setup_criu_work_dir (cr_options, &work_fd, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

#  ifdef CRIU_PRE_DUMP_SUPPORT

  {
    පූර්ණ_සංඛ්‍යා criu_can_mem_track = 0;
    /* If the user uses --pre-dump for the second time or does
     * a final dump from a previous pre-dump, setting parent_path
     * is necessary so that CRIU can find which pages have not
     * changed compared to the previous dump. */
    නම් (cr_options->parent_path != NULL)
      {
        criu_can_mem_track = criu_check_mem_track (err);
        නම් (UNLIKELY (criu_can_mem_track == -1))
          ආපසු -1;
        libcriu_wrapper->criu_set_track_mem (true);

        /* The parent path must be relative to image path (something like ../previous-dump).
           CRIU will fail with an unclear error message if the path is not right.
         */
        නම් (UNLIKELY (cr_options->parent_path[0] == '/'))
          ආපසු crun_make_error (err, 0, "--parent-path must be relative");
        පූර්ණ_සංඛ්‍යා is_dir = crun_dir_p_at (image_fd, cr_options->parent_path, false, err);
        නම් (UNLIKELY (is_dir <= 0))
          {
            නම් (is_dir < 0)
              ආපසු crun_error_wrap (err, "invalid --parent-path");
            ආපසු crun_make_error (err, ENOTDIR, "invalid --parent-path");
          }
        ret = libcriu_wrapper->criu_set_parent_images (cr_options->parent_path);
        නම් (UNLIKELY (ret != 0))
          ආපසු crun_make_error (err, -ret, "error setting CRIU parent images path to `%s`", cr_options->parent_path);
      }

    නම් (cr_options->pre_dump)
      {
        නම් (criu_can_mem_track != 1)
          {
            criu_can_mem_track = criu_check_mem_track (err);
            නම් (UNLIKELY (criu_can_mem_track == -1))
              ආපසු -1;
          }
        libcriu_wrapper->criu_set_track_mem (true);
        ret = libcriu_wrapper->criu_pre_dump ();
        නම් (UNLIKELY (ret != 0))
          {
            show_criu_log (cr_options->work_path, CRIU_CHECKPOINT_LOG_FILE);
            ආපසු crun_make_error (err, 0, "CRIU pre-dump failed: %d", ret);
          }
        ආපසු 0;
      }
  }
#  endif

  /* descriptors.json is needed during restore to correctly
   * reconnect stdin, stdout, stderr. */
  ret = append_paths (&descriptors_path, err, cr_options->image_path, DESCRIPTORS_FILENAME, NULL);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  ret = write_file (descriptors_path, status->external_descriptors, safe_strlen (status->external_descriptors), err);
  නම් (UNLIKELY (ret < 0))
    ආපසු crun_error_wrap (err, "error saving CRIU descriptors file");

  ret = append_paths (&path, err, status->bundle, status->rootfs, NULL);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  ret = libcriu_wrapper->criu_set_root (path);
  නම් (UNLIKELY (ret != 0))
    ආපසු crun_make_error (err, 0, "error setting CRIU root to `%s`", path);

  cgroup_mode = libcrun_get_cgroup_mode (err);
  නම් (UNLIKELY (cgroup_mode < 0))
    ආපසු cgroup_mode;

  /* For cgroup v1 we need to tell CRIU to handle all cgroup mounts as external mounts. */
  නම් (cgroup_mode != CGROUP_MODE_UNIFIED)
    {
      ret = checkpoint_cgroup_v1_mount (def, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }

  /* Tell CRIU about external bind mounts. */
  සඳහා (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      නම් (is_bind_mount (def->mounts[i], NULL, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          var_na buf[PATH_MAX];
          const var_na *dest_in_root;

          නම් (nofollow)
            ආපසු crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_in_root = chroot_realpath (status->rootfs, def->mounts[i]->destination, buf);
          නම් (UNLIKELY (dest_in_root == NULL))
            ආපසු crun_make_error (err, errno, "unable to resolve external bind mount `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          නම් (has_prefix (dest_in_root, status->rootfs))
            dest_in_root += safe_strlen (status->rootfs);

          ret = libcriu_wrapper->criu_add_ext_mount (dest_in_root, dest_in_root);
          නම් (UNLIKELY (ret < 0))
            ආපසු crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", def->mounts[i]->destination);
        }
    }

  ret = register_masked_paths_mounts (def, container, libcriu_wrapper, false, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

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

  සඳහා (i = 0; i < def->linux->namespaces_len; i++)
    {
      පූර්ණ_සංඛ්‍යා value = libcrun_find_namespace (def->linux->namespaces[i]->type);
      නම් (UNLIKELY (value < 0))
        ආපසු crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      නම් (value == CLONE_NEWNET && def->linux->namespaces[i]->path != NULL)
        {
          cleanup_free var_na *external = NULL;
          sa_nracanaa stat statbuf;

          ret = stat (def->linux->namespaces[i]->path, &statbuf);
          නම් (UNLIKELY (ret < 0))
            ආපසු crun_make_error (err, errno, "unable to stat(): `%s`", def->linux->namespaces[i]->path);

          xasprintf (&external, "net[%ld]:" CRIU_EXT_NETNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_add_external (external);
          නම් (UNLIKELY (ret < 0))
            ආපසු crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }

      නම් (value == CLONE_NEWPID && def->linux->namespaces[i]->path != NULL)
        {
          cleanup_free var_na *external = NULL;
          sa_nracanaa stat statbuf;

          ret = stat (def->linux->namespaces[i]->path, &statbuf);
          නම් (UNLIKELY (ret < 0))
            ආපසු crun_make_error (err, errno, "unable to stat(): `%s`", def->linux->namespaces[i]->path);

          xasprintf (&external, "pid[%ld]:" CRIU_EXT_PIDNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_add_external (external);
          නම් (UNLIKELY (ret < 0))
            ආපසු crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }
    }

  /* Tell CRIU to use the freezer to pause all container processes. */
  නම් (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      /* This needs CRIU 3.14. */
      ret = append_paths (&freezer_path, err, CGROUP_ROOT, status->cgroup_path, NULL);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }
  නැතහොත්
    {
      ret = append_paths (&freezer_path, err, CGROUP_ROOT "/freezer", status->cgroup_path, NULL);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;
    }

  ret = libcriu_wrapper->criu_set_freeze_cgroup (freezer_path);
  නම් (UNLIKELY (ret < 0))
    ආපසු crun_make_error (err, -ret, "CRIU: failed setting freezer %d", ret);

  /* Set boolean options . */
  libcriu_wrapper->criu_set_leave_running (cr_options->leave_running);
  libcriu_wrapper->criu_set_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_set_shell_job (cr_options->shell_job);
  libcriu_wrapper->criu_set_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_set_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_set_orphan_pts_master (true);
  libcriu_wrapper->criu_set_manage_cgroups_mode (criu_cg_mode_from_public (cr_options->manage_cgroups_mode));

  libcriu_wrapper->criu_set_manage_cgroups (true);

  නම් (libcriu_wrapper->criu_set_network_lock)
    {
      පූර්ණ_සංඛ්‍යා lock = 0;

      ret = criu_network_lock_from_public (cr_options->network_lock_method, &lock, err);
      නම් (UNLIKELY (ret < 0))
        ආපසු ret;

      නම් (lock > 0)
        {
          ret = libcriu_wrapper->criu_set_network_lock (lock);
          නම් (UNLIKELY (ret < 0))
            ආපසු crun_make_error (err, 0, "CRIU: failed setting network lock");
        }
    }

  ret = libcriu_wrapper->criu_dump ();
  නම් (UNLIKELY (ret != 0))
    {
      show_criu_log (cr_options->work_path, CRIU_CHECKPOINT_LOG_FILE);
      ආපසු crun_make_error (err, ret < 0 ? -ret : 0, "CRIU checkpointing failed: %d", ret);
    }

  ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා
prepare_restore_mounts_internal (runtime_spec_schema_config_schema *def, var_na *root, const var_na **mounted,
                                 size_t *n_mounted, libcrun_error_t *err)
{
  uint32_t i;

  /* Go through all mountpoints to be able to recreate missing mountpoints. */
  සඳහා (i = 0; i < def->mounts_len; i++)
    {
      var_na *dest = def->mounts[i]->destination;
      var_na *type = def->mounts[i]->type;
      cleanup_close පූර්ණ_සංඛ්‍යා root_fd = -1;
      bool nofollow = false;
      bool on_tmpfs = false;
      පූර්ණ_සංඛ්‍යා is_dir = 1;
      size_t j;

      /* cgroup restore should be handled by CRIU itself */
      නම් (type && (strcmp (type, "cgroup") == 0 || strcmp (type, "cgroup2") == 0))
        continue;

      /* Check if the mountpoint is on a tmpfs. CRIU restores
       * all tmpfs. We do need to recreate directories on a tmpfs. */
      size_t dest_len = strlen (dest);
      සඳහා (j = 0; j < def->mounts_len; j++)
        {
          නම් (def->mounts[j]->type == NULL || strcmp (def->mounts[j]->type, "tmpfs") != 0)
            continue;
          size_t mount_len = strlen (def->mounts[j]->destination);
          නම් (mount_len < dest_len && dest[mount_len] == '/' && strncmp (dest, def->mounts[j]->destination, mount_len) == 0)
            {
              /* This is a mountpoint which is on a tmpfs.*/
              on_tmpfs = true;
              bha_nga;
            }
        }

      නම් (on_tmpfs)
        continue;

      /* For bind mounts check if the source is a file or a directory. */
      නම් (is_bind_mount (def->mounts[i], NULL, &nofollow))
        {
          නම් (nofollow)
            ආපසු crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          is_dir = crun_dir_p (def->mounts[i]->source, false, err);
          නම් (UNLIKELY (is_dir < 0))
            ආපසු is_dir;
        }

      root_fd = open (root, O_RDONLY | O_CLOEXEC);
      නම් (UNLIKELY (root_fd == -1))
        ආපසු crun_make_error (err, errno, "error opening container root directory `%s`", root);

      නම් (is_dir)
        {
          පූර්ණ_සංඛ්‍යා ret;

          ret = crun_safe_ensure_directory_at (root_fd, root, dest, 0755, err);
          නම් (UNLIKELY (ret < 0))
            ආපසු ret;
        }
      නැතහොත්
        {
          පූර්ණ_සංඛ්‍යා ret;

          ret = crun_safe_ensure_file_at (root_fd, root, dest, 0755, err);
          නම් (UNLIKELY (ret < 0))
            ආපසු ret;
        }

      /* Mount a bind mount source now, so that the mountpoints of the
       * mounts inside it (e.g. a nested bind mount) are created in the
       * source, where CRIU expects them.  This also happens during the
       * initial container creation, as the mounts are done in order.  */
      නම් (is_bind_mount (def->mounts[i], NULL, &nofollow))
        {
          cleanup_close පූර්ණ_සංඛ්‍යා dst_fd = -1;
          proc_fd_path_t fd_path;

          dst_fd = safe_openat (root_fd, root, dest, O_PATH | O_CLOEXEC, 0, err);
          නම් (UNLIKELY (dst_fd < 0))
            ආපසු dst_fd;

          /* Not recursive: the source might contain ROOT itself (e.g. when
           * it is the bundle directory), and it is not needed anyway.  */
          get_proc_self_fd_path (fd_path, dst_fd);
          නම් (UNLIKELY (mount (def->mounts[i]->source, fd_path, NULL, MS_BIND, NULL) < 0))
            ආපසු crun_make_error (err, errno, "bind mount `%s` to `%s`", def->mounts[i]->source, dest);

          mounted[(*n_mounted)++] = dest;
        }
    }

  ආපසු 0;
}

/* Move the current process back to CGROUPS, as read from /proc/self/cgroup.
   A failure is not fatal, so only warn about it.  */
static void
move_back_to_cgroups (const var_na *cgroups)
{
  libcrun_error_t tmp_err = NULL;

  නම් (UNLIKELY (libcrun_move_self_to_cgroups (cgroups, &tmp_err) < 0))
    {
      libcrun_warning ("cannot move back to the original cgroup: %s", tmp_err->msg);
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
static පූර්ණ_සංඛ්‍යා
criu_restore_child_in_cgroup (const var_na *cgroup_path, පූර්ණ_සංඛ්‍යා *criu_ret, libcrun_error_t *err)
{
  sigset_t all_signals, old_mask;
  පූර්ණ_සංඛ්‍යා wait_status = 0;
  pid_t pid;
  පූර්ණ_සංඛ්‍යා ret;

  *criu_ret = -1;

  sigfillset (&all_signals);
  ret = sigprocmask (SIG_BLOCK, &all_signals, &old_mask);
  නම් (UNLIKELY (ret < 0))
    ආපසු crun_make_error (err, errno, "sigprocmask");

  /* Must be vfork: the child shares our memory space, so both *criu_ret and
     the error it creates are visible here once it is gone.  */
  pid = vfork ();
  නම් (UNLIKELY (pid < 0))
    {
      පූර්ණ_සංඛ්‍යා saved_errno = errno;

      sigprocmask (SIG_SETMASK, &old_mask, NULL);
      ආපසු crun_make_error (err, saved_errno, "vfork");
    }

  නම් (pid == 0)
    {
      sa_nracanaa sigaction act;
      පූර්ණ_සංඛ්‍යා i;

      /* The signal dispositions are not shared with the parent, so resetting
         them here does not affect it.  Keep the ignored signals ignored, as
         CRIU would inherit them if it was run directly by the caller.  */
      සඳහා (i = 1; i < NSIG; i++)
        {
          නම් (sigaction (i, NULL, &act) < 0 || act.sa_handler == SIG_IGN || act.sa_handler == SIG_DFL)
            continue;

          memset (&act, 0, parimaa_na (act));
          act.sa_handler = SIG_DFL;
          sigaction (i, &act, NULL);
        }
      sigprocmask (SIG_SETMASK, &old_mask, NULL);

      නම් (! is_empty_string (cgroup_path))
        {
          ret = libcrun_move_process_to_cgroup (0, 0, cgroup_path, false, err);
          නම් (UNLIKELY (ret < 0))
            _safe_exit (EXIT_FAILURE);
        }

      *criu_ret = libcriu_wrapper->criu_restore_child ();
      _safe_exit (EXIT_SUCCESS);
    }

  /* The child is gone by now, it is safe to handle signals again.  */
  sigprocmask (SIG_SETMASK, &old_mask, NULL);

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  නම් (UNLIKELY (ret < 0))
    {
      /* The caller might have reaped the child already, e.g. when it ignores
         SIGCHLD.  Its result is still visible, so use it.  */
      නම් (errno != ECHILD)
        ආපසු crun_make_error (err, errno, "waitpid for the CRIU restore process");

      /* The child might have failed before calling CRIU, and left an error.  */
      ආපසු *err != NULL ? -1 : 0;
    }

  ret = get_process_exit_status (wait_status);
  නම් (UNLIKELY (ret != EXIT_SUCCESS))
    {
      /* The child creates the error in the shared memory space, do not
         overwrite it with crun_make_error().  */
      නම් (*err == NULL)
        ආපසු crun_make_error (err, 0, "the CRIU restore process exited with status %d", ret);
      ආපසු -1;
    }

  ආපසු 0;
}

/* Recreate the mountpoints which do not exist in the container rootfs
 * mounted at ROOT, the same way it is done on the container creation.  */
static පූර්ණ_සංඛ්‍යා
prepare_restore_mounts (runtime_spec_schema_config_schema *def, var_na *root, libcrun_error_t *err)
{
  cleanup_free const var_na **mounted = xmalloc0 (parimaa_na (var_na *) * (def->mounts_len + 1));
  cleanup_close පූර්ණ_සංඛ්‍යා root_fd = -1;
  size_t n_mounted = 0;
  පූර්ණ_සංඛ්‍යා ret;

  ret = prepare_restore_mounts_internal (def, root, mounted, &n_mounted, err);

  /* The bind mounts done above are only needed to create the mountpoints,
   * and CRIU restores the mounts itself, so undo them, in reverse order. */
  නම් (n_mounted > 0)
    root_fd = open (root, O_PATH | O_CLOEXEC);
  අතරතුර (root_fd >= 0 && n_mounted > 0)
    {
      libcrun_error_t tmp_err = NULL;
      const var_na *dest = mounted[--n_mounted];
      cleanup_close පූර්ණ_සංඛ්‍යා dst_fd = -1;
      proc_fd_path_t fd_path;

      dst_fd = safe_openat (root_fd, root, dest, O_PATH | O_CLOEXEC, 0, &tmp_err);
      නම් (UNLIKELY (dst_fd < 0))
        {
          crun_error_release (&tmp_err);
          continue;
        }
      get_proc_self_fd_path (fd_path, dst_fd);
      නම් (UNLIKELY (umount2 (fd_path, MNT_DETACH) < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "unmount `%s`", dest);
    }

  ආපසු ret;
}

පූර්ණ_සංඛ්‍යා
libcrun_container_restore_linux_criu (libcrun_container_status_t *status, libcrun_container_t *container,
                                      libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper sa_nracanaa libcriu_wrapper_s *wrapper = NULL;
  cleanup_close පූර්ණ_සංඛ්‍යා inherit_new_net_fd = -1;
  cleanup_close පූර්ණ_සංඛ්‍යා inherit_new_pid_fd = -1;
  cleanup_close පූර්ණ_සංඛ්‍යා image_fd = -1;
  cleanup_free var_na *root = NULL;
  cleanup_free var_na *bundle_cleanup = NULL;
  cleanup_free var_na *own_cgroups = NULL;
  cleanup_close පූර්ණ_සංඛ්‍යා work_fd = -1;
  පූර්ණ_සංඛ්‍යා cgroup_mode;
  පූර්ණ_සංඛ්‍යා criu_ret;
  පූර්ණ_සංඛ්‍යා ret_out;
  size_t i;
  පූර්ණ_සංඛ්‍යා ret;

  ret = load_wrapper (&wrapper, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  නම් (geteuid ())
    ආපසු crun_make_error (err, 0, "restoring requires root");

  ret = libcriu_wrapper->criu_init_opts ();
  නම් (UNLIKELY (ret < 0))
    ආපසු crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_version (err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  නම් (UNLIKELY (cr_options->image_path == NULL))
    ආපසු crun_make_error (err, 0, "image path not set");

  image_fd = open (cr_options->image_path, O_DIRECTORY | O_CLOEXEC);
  නම් (UNLIKELY (image_fd == -1))
    ආපසු crun_make_error (err, errno, "error opening checkpoint directory `%s`", cr_options->image_path);

  libcriu_wrapper->criu_set_images_dir_fd (image_fd);

  /* Load descriptors.json to tell CRIU where those FDs should be connected to. */
  {
    cleanup_free var_na *descriptors_path = NULL;
    cleanup_free var_na *buffer = NULL;
    json_object *doc = NULL;

    ret = append_paths (&descriptors_path, err, cr_options->image_path, DESCRIPTORS_FILENAME, NULL);
    නම් (UNLIKELY (ret < 0))
      ආපසු ret;

    ret = read_all_file (descriptors_path, &buffer, NULL, err);
    නම් (UNLIKELY (ret < 0))
      ආපසු ret;

    /* descriptors.json contains a JSON array with strings
     * telling where 0, 1 and 2 have been initially been
     * pointing to. For each descriptor which points to
     * a pipe 'pipe:' we tell CRIU to reconnect that pipe
     * to the corresponding FD to have (especially) stdout
     * and stderr being correctly redirected. */
    ret = parse_json_file (&doc, buffer, NULL, err);
    නම් (UNLIKELY (ret < 0))
      ආපසු ret;

    නම් (json_object_is_type (doc, json_type_array))
      {
        size_t i, len = json_object_array_length (doc);

        /* len will probably always be 3 as crun is currently only
         * recording the destination of FD 0, 1 and 2. */
        සඳහා (i = 0; i < len; ++i)
          {
            json_object *s = json_object_array_get_idx (doc, i);
            නම් (s && json_object_is_type (s, json_type_string))
              {
                const var_na *str = json_object_get_string (s);
                නම් (has_prefix (str, "pipe:"))
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
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  ret = setup_criu_work_dir (cr_options, &work_fd, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  නම් (cr_options->lsm_profile != NULL)
    {
      ret = libcriu_wrapper->criu_set_lsm_profile (cr_options->lsm_profile);
      නම් (UNLIKELY (ret != 0))
        ආපසු crun_make_error (err, -ret, "error setting LSM profile to `%s`", cr_options->lsm_profile);
    }

  නම් (cr_options->lsm_mount_context != NULL)
    {
      ret = libcriu_wrapper->criu_set_lsm_mount_context (cr_options->lsm_mount_context);
      නම් (UNLIKELY (ret != 0))
        ආපසු crun_make_error (err, -ret, "error setting LSM mount context to `%s`", cr_options->lsm_mount_context);
    }

  /* do realpath on root */
  bundle_cleanup = realpath (status->bundle, NULL);
  නම් (UNLIKELY (bundle_cleanup == NULL))
    bundle_cleanup = xstrdup (status->bundle);

  /* Tell CRIU about external bind mounts. */
  සඳහා (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      නම් (is_bind_mount (def->mounts[i], NULL, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          var_na buf[PATH_MAX];
          const var_na *dest_in_root;
          const var_na *source = def->mounts[i]->source;
          cleanup_free var_na *abs_source = NULL;
          unsigned long flags = 0;
          size_t j;

          නම් (nofollow)
            ආපසු crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_in_root = chroot_realpath (status->rootfs, def->mounts[i]->destination, buf);
          නම් (UNLIKELY (dest_in_root == NULL))
            ආපසු crun_make_error (err, errno, "unable to resolve external bind mount destination `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          නම් (has_prefix (dest_in_root, status->rootfs))
            dest_in_root += safe_strlen (status->rootfs);

          /* A relative source is relative to the bundle, while CRIU would
             resolve it relative to its own working directory.  */
          නම් (source && source[0] != '/')
            {
              ret = append_paths (&abs_source, err, bundle_cleanup, source, NULL);
              නම් (UNLIKELY (ret < 0))
                ආපසු ret;
              source = abs_source;
            }

          ret = libcriu_wrapper->criu_add_ext_mount (dest_in_root, source);
          නම් (UNLIKELY (ret < 0))
            ආපසු crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", source);

          සඳහා (j = 0; j < def->mounts[i]->options_len; j++)
            {
              const sa_nracanaa propagation_flags_s *prop = NULL;

              නම් (def->mounts[i]->options[j])
                prop = libcrun_str2mount_flags (def->mounts[i]->options[j]);
              නම් (prop && ! prop->clear)
                flags |= prop->flags;
            }
          add_restored_ext_mount (dest_in_root, flags);
        }
    }

  ret = register_masked_paths_mounts (def, container, libcriu_wrapper, true, err);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  /* Mount the container rootfs for CRIU. */
  ret = append_paths (&root, err, bundle_cleanup, "criu-root", NULL);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;

  ret = mkdir (root, 0755);
  නම් (UNLIKELY (ret == -1))
    ආපසු crun_make_error (err, errno, "error creating restore directory `%s`", root);

  ret = mount (status->rootfs, root, NULL, MS_BIND | MS_REC, NULL);
  නම් (UNLIKELY (ret == -1))
    {
      ret = crun_make_error (err, errno, "error mounting restore directory `%s`", root);
      goto out;
    }

  /* During initial container creation, crun will create mountpoints
   * defined in config.json if they do not exist. If we are restoring
   * we need to make sure these mountpoints also exist.
   * This is not perfect, as this means crun will modify a rootfs
   * even if it marked as read-only, but runc already modifies
   * the rootfs in the same way. */

  ret = prepare_restore_mounts (def, root, err);
  නම් (UNLIKELY (ret < 0))
    goto out_umount;

  ret = libcriu_wrapper->criu_set_root (root);
  නම් (UNLIKELY (ret != 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU root to `%s`", root);
      goto out_umount;
    }

  /* If a namespace defined in config.json we are telling
   * CRIU use that namespace when restoring the process tree.
   *
   * CRIU expects the information about the namespace like this:
   * --inherit-fd fd[<fd>]:<key>
   * The <key> needs to be the same as during checkpointing (extRootNetNS). */
  සඳහා (i = 0; i < def->linux->namespaces_len; i++)
    {
      const පූර්ණ_සංඛ්‍යා open_flags_for_inherit = O_RDONLY; /* Cannot be O_CLOEXEC as it is passed to the child process. */
      පූර්ණ_සංඛ්‍යා value = libcrun_find_namespace (def->linux->namespaces[i]->type);
      නම් (UNLIKELY (value < 0))
        {
          ret = crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);
          goto out_umount;
        }

      නම් (value == CLONE_NEWNET && def->linux->namespaces[i]->path != NULL)
        {
          inherit_new_net_fd = open (def->linux->namespaces[i]->path, open_flags_for_inherit);
          නම් (UNLIKELY (inherit_new_net_fd < 0))
            {
              ret = crun_make_error (err, errno, "unable to open(): `%s`", def->linux->namespaces[i]->path);
              goto out_umount;
            }

          ret = libcriu_wrapper->criu_add_inherit_fd (inherit_new_net_fd, CRIU_EXT_NETNS);
          නම් (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              goto out_umount;
            }
        }

      නම් (value == CLONE_NEWPID && def->linux->namespaces[i]->path != NULL)
        {
          inherit_new_pid_fd = open (def->linux->namespaces[i]->path, open_flags_for_inherit);
          නම් (UNLIKELY (inherit_new_pid_fd < 0))
            {
              ret = crun_make_error (err, errno, "unable to open(): `%s`", def->linux->namespaces[i]->path);
              goto out_umount;
            }

          ret = libcriu_wrapper->criu_add_inherit_fd (inherit_new_pid_fd, CRIU_EXT_PIDNS);
          නම් (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              goto out_umount;
            }
        }

#  ifdef CRIU_JOIN_NS_SUPPORT
      නම් (value == CLONE_NEWTIME && def->linux->namespaces[i]->path != NULL)
        {
          නම් (libcriu_wrapper->criu_join_ns_add == NULL)
            {
              ret = crun_make_error (err, 0, "shared time namespace restore is supported in CRIU >= 3.16.1");
              goto out_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_add ("time", def->linux->namespaces[i]->path, NULL);
          නම් (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->path);
              goto out_umount;
            }
        }

      නම් (value == CLONE_NEWIPC && def->linux->namespaces[i]->path != NULL)
        {
          නම් (libcriu_wrapper->criu_join_ns_add == NULL)
            {
              ret = crun_make_error (err, 0, "shared ipc namespace restore is supported in CRIU >= 3.16.1");
              goto out_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_add ("ipc", def->linux->namespaces[i]->path, NULL);
          නම් (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->path);
              goto out_umount;
            }
        }

      නම් (value == CLONE_NEWUTS && def->linux->namespaces[i]->path != NULL)
        {
          නම් (libcriu_wrapper->criu_join_ns_add == NULL)
            {
              ret = crun_make_error (err, 0, "shared uts namespace restore is supported in CRIU >= 3.16.1");
              goto out_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_add ("uts", def->linux->namespaces[i]->path, NULL);
          නම් (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->path);
              goto out_umount;
            }
        }
#  endif
    }

  /* Set up CRIU config file */
  ret = handle_criu_config_file (container, err);
  නම් (UNLIKELY (ret < 0))
    goto out_umount;

  /* Tell CRIU if cgroup v1 needs to be handled. */
  ret = restore_cgroup_v1_mount (def, err);
  නම් (UNLIKELY (ret < 0))
    goto out_umount;

  console_socket = cr_options->console_socket;
  libcriu_wrapper->criu_set_notify_cb (criu_notify);

  /* Set boolean options . */
  libcriu_wrapper->criu_set_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_set_shell_job (cr_options->shell_job);
  libcriu_wrapper->criu_set_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_set_tcp_close (cr_options->tcp_close);
  libcriu_wrapper->criu_set_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_set_orphan_pts_master (true);

  නම් (status->cgroup_path)
    {
      cgroup_mode = libcrun_get_cgroup_mode (err);
      නම් (UNLIKELY (cgroup_mode < 0))
        {
          ret = cgroup_mode;
          goto out_umount;
        }

      /* With cgroup v2, the cgroup path is only meaningful for the unified
         hierarchy, so only relocate that one.  A NULL controller would make
         CRIU relocate every hierarchy the container is in, named cgroup v1
         ones included.  As CRIU dumps such a hierarchy from wherever the
         container is in it, which is its root, as crun leaves named
         hierarchies alone, every checkpoint and restore would then copy the
         whole hierarchy into itself.  */
      ret = libcriu_wrapper->criu_add_cg_root (cgroup_mode == CGROUP_MODE_UNIFIED ? "" : NULL,
                                               status->cgroup_path);
      නම් (UNLIKELY (ret != 0))
        {
          ret = crun_make_error (err, 0, "error setting CRIU cgroup root to `%s`", status->cgroup_path);
          goto out_umount;
        }
    }

  libcriu_wrapper->criu_set_manage_cgroups_mode (criu_cg_mode_from_public (cr_options->manage_cgroups_mode));
  libcriu_wrapper->criu_set_manage_cgroups (true);

  නම් (libcriu_wrapper->criu_set_network_lock)
    {
      පූර්ණ_සංඛ්‍යා lock = 0;

      ret = criu_network_lock_from_public (cr_options->network_lock_method, &lock, err);
      නම් (UNLIKELY (ret < 0))
        goto out_umount;

      නම් (lock > 0)
        {
          ret = libcriu_wrapper->criu_set_network_lock (lock);
          නම් (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "CRIU: failed setting network lock");
              goto out_umount;
            }
        }
    }

  libcriu_wrapper->criu_set_log_level (4);
  ret = libcriu_wrapper->criu_set_log_file (CRIU_RESTORE_LOG_FILE);
  නම් (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU log file to `%s`", CRIU_RESTORE_LOG_FILE);
      goto out_umount;
    }

  /* criu_restore() returns the PID of the process of the restored process
   * tree. This PID will not be the same as status->pid if the container is
   * running in a PID namespace. But it will always be > 0. */
  නම් (cr_options->detach)
    {
      /* Nothing waits for the container, so there is no reason to take over
         this process: use a throw-away one.  It is used even when there is no
         cgroup to join, so that the container is not left as a child of a
         process which is not going to reap it.  */
      ret = criu_restore_child_in_cgroup (status->cgroup_path, &criu_ret, err);
      නම් (UNLIKELY (ret < 0))
        goto out_umount;

      ret = criu_ret;
    }
  නැතහොත්
    {
      /* Like runc does, put CRIU into the container cgroup for the time of
       * restore, so the restored tasks are created there. It is necessary in
       * the "ignore" mode, where CRIU does not deal with cgroups at all, and
       * does not hurt otherwise. As CRIU is our child, do it by moving
       * ourselves there, and move back once the restore is done. */
      නම් (! is_empty_string (status->cgroup_path))
        {
          ret = read_all_file (PROC_SELF_CGROUP, &own_cgroups, NULL, err);
          නම් (UNLIKELY (ret < 0))
            goto out_umount;

          ret = libcrun_move_process_to_cgroup (0, 0, status->cgroup_path, false, err);
          නම් (UNLIKELY (ret < 0))
            {
              /* Some of the cgroups might have been joined already.  */
              move_back_to_cgroups (own_cgroups);
              goto out_umount;
            }
        }

      ret = libcriu_wrapper->criu_restore_child ();

      නම් (own_cgroups)
        move_back_to_cgroups (own_cgroups);
    }

  නම් (UNLIKELY (ret <= 0))
    {
      show_criu_log (cr_options->work_path, CRIU_RESTORE_LOG_FILE);
      ret = crun_make_error (err, 0, "CRIU restoring failed: %d", ret);
      goto out_umount;
    }

  /* Update the status struct with the newly allocated PID. This will
   * be necessary later when moving the process into its cgroup. */
  status->pid = ret;
  ret = 0;

out_umount:
  ret_out = umount (root);
  නම් (UNLIKELY (ret_out == -1))
    {
      පූර්ණ_සංඛ්‍යා saved_errno = errno;
      rmdir (root);
      නම් (ret < 0)
        ආපසු crun_error_wrap (err, "error unmounting restore directory `%s`", root);
      නැතහොත්
        ආපසු crun_make_error (err, saved_errno, "error unmounting restore directory `%s`", root);
    }
out:
  ret_out = rmdir (root);
  නම් (UNLIKELY (ret < 0))
    ආපසු ret;
  නම් (UNLIKELY (ret_out == -1))
    ආපසු crun_make_error (err, errno, "error removing restore directory `%s`", root);
  ආපසු ret;
}
#endif
