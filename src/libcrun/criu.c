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

#அந்தர்பாவ <config.h>

#எனில் HAVE_CRIU && HAVE_DLOPEN

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
எழுத்து *chroot_realpath (மாறிலி எழுத்து *chroot, மாறிலி எழுத்து *path, எழுத்து resolved_path[]);

/* Map the public cgroups mode (enum libcrun_cr_cgroups_mode) onto the CRIU enum.  */
நிலையான எண்ணிக்கை criu_cg_mode
criu_cg_mode_from_public (முழு mode)
{
  தேர்வு (mode)
    {
    நிலை LIBCRUN_CR_CG_MODE_IGNORE:
      திரும்பு CRIU_CG_MODE_IGNORE;
    நிலை LIBCRUN_CR_CG_MODE_FULL:
      திரும்பு CRIU_CG_MODE_FULL;
    நிலை LIBCRUN_CR_CG_MODE_STRICT:
      திரும்பு CRIU_CG_MODE_STRICT;
    நிலை LIBCRUN_CR_CG_MODE_DEFAULT:
    நிலை LIBCRUN_CR_CG_MODE_SOFT:
    இயல்பு:
      /* Default to CRIU_CG_MODE_SOFT, just as runc.  */
      திரும்பு CRIU_CG_MODE_SOFT;
    }
}

/* Map the public network lock method (enum libcrun_cr_network_lock_method) onto the
   CRIU enum.  *LOCK is set to 0 when no explicit method must be set (CRIU default).
   A method that this build cannot honor is an error, never a silent fallback to
   the CRIU default.  */
நிலையான முழு
criu_network_lock_from_public (முழு method, முழு *lock, libcrun_error_t *err)
{
  தேர்வு (method)
    {
    நிலை LIBCRUN_CR_NETWORK_LOCK_IPTABLES:
      *lock = CRIU_NETWORK_LOCK_IPTABLES;
      திரும்பு 0;
    நிலை LIBCRUN_CR_NETWORK_LOCK_NFTABLES:
      *lock = CRIU_NETWORK_LOCK_NFTABLES;
      திரும்பு 0;
    நிலை LIBCRUN_CR_NETWORK_LOCK_SKIP:
#  எனில் CRIU_NETWORK_LOCK_SKIP_SUPPORT
      *lock = CRIU_NETWORK_LOCK_SKIP;
      திரும்பு 0;
#  இல்லையெனில்
      திரும்பு crun_make_error (err, 0, "CRIU: the `skip` network lock method is not supported by this build");
#  endif
    நிலை LIBCRUN_CR_NETWORK_LOCK_DEFAULT:
      *lock = 0;
      திரும்பு 0;
    இயல்பு:
      திரும்பு crun_make_error (err, 0, "CRIU: unknown network lock method %d", method);
    }
}

நிலையான மாறிலி எழுத்து *console_socket = வெற்று;

/* The external mounts CRIU restores, and the flags the configuration asks
   for each of them.  */
நிலையான கட்டமைப்பு libcrun_mount_flags_s *restored_ext_mounts = வெற்று;
நிலையான size_t n_restored_ext_mounts = 0;

#  define LIBCRIU_MIN_VERSION 31500

கட்டமைப்பு libcriu_wrapper_s
{
  வெறுமை *handle;
  முழு (*criu_add_ext_mount) (மாறிலி எழுத்து *சாவி, மாறிலி எழுத்து *val);
  முழு (*criu_add_external) (மாறிலி எழுத்து *சாவி);
  முழு (*criu_add_inherit_fd) (முழு fd, மாறிலி எழுத்து *சாவி);
  முழு (*criu_check_version) (முழு minimum);
  முழு (*criu_dump) (வெறுமை);
  முழு (*criu_get_orphan_pts_master_fd) (வெறுமை);
  முழு (*criu_init_opts) (வெறுமை);
  முழு (*criu_notify_pid) (criu_notify_arg_t na);
#  ifdef CRIU_JOIN_NS_SUPPORT
  முழு (*criu_join_ns_add) (மாறிலி எழுத்து *ns, மாறிலி எழுத்து *ns_file, மாறிலி எழுத்து *extra_opt);
#  endif
#  ifdef CRIU_PRE_DUMP_SUPPORT
  முழு (*criu_feature_check) (கட்டமைப்பு criu_feature_check *features, size_t size);
  முழு (*criu_pre_dump) (வெறுமை);
#  endif
  முழு (*criu_restore_child) (வெறுமை);
  முழு (*criu_set_freeze_cgroup) (மாறிலி எழுத்து *name);
  வெறுமை (*criu_set_file_locks) (bool file_locks);
  வெறுமை (*criu_set_ext_unix_sk) (bool ext_unix_sk);
  முழு (*criu_set_log_file) (மாறிலி எழுத்து *log_file);
  வெறுமை (*criu_set_log_level) (முழு log_level);
  வெறுமை (*criu_set_leave_running) (bool leave_running);
  வெறுமை (*criu_set_manage_cgroups) (bool manage);
  வெறுமை (*criu_set_manage_cgroups_mode) (எண்ணிக்கை criu_cg_mode mode);
  முழு (*criu_set_network_lock) (எண்ணிக்கை criu_network_lock_method method);
  வெறுமை (*criu_set_notify_cb) (முழு (*cb) (எழுத்து *action, criu_notify_arg_t na));
  வெறுமை (*criu_set_orphan_pts_master) (bool orphan_pts_master);
  வெறுமை (*criu_set_images_dir_fd) (முழு fd);
  முழு (*criu_set_parent_images) (மாறிலி எழுத்து *path);
  வெறுமை (*criu_set_pid) (முழு pid);
  முழு (*criu_set_வேர்) (மாறிலி எழுத்து *வேர்);
  முழு (*criu_add_cg_வேர்) (மாறிலி எழுத்து *ctrl, மாறிலி எழுத்து *path);
  வெறுமை (*criu_set_shell_job) (bool shell_job);
  வெறுமை (*criu_set_tcp_established) (bool tcp_established);
  வெறுமை (*criu_set_tcp_close) (bool tcp_close);
  வெறுமை (*criu_set_track_mem) (bool track_mem);
  வெறுமை (*criu_set_work_dir_fd) (முழு fd);
  முழு (*criu_set_lsm_profile) (மாறிலி எழுத்து *name);
  முழு (*criu_set_lsm_mount_context) (மாறிலி எழுத்து *name);
  முழு (*criu_set_config_file) (மாறிலி எழுத்து *path);
};

நிலையான கட்டமைப்பு libcriu_wrapper_s *libcriu_wrapper;

நிலையான உள்வரி வெறுமை
cleanup_wrapper (வெறுமை *p)
{
  கட்டமைப்பு libcriu_wrapper_s **w;

  w = (கட்டமைப்பு libcriu_wrapper_s **) p;
  எனில் (*w == வெற்று)
    திரும்பு;

#  ifndef STATIC
  எனில் ((*w)->handle)
    dlclose ((*w)->handle);
#  endif
  விடுவி (*w);
  libcriu_wrapper = வெற்று;

  போது (n_restored_ext_mounts > 0)
    விடுவி (restored_ext_mounts[--n_restored_ext_mounts].target);
  விடுவி (restored_ext_mounts);
  restored_ext_mounts = வெற்று;
}

#  define cleanup_wrapper __attribute__ ((cleanup (cleanup_wrapper)))

நிலையான முழு
load_wrapper (கட்டமைப்பு libcriu_wrapper_s **wrapper_out, libcrun_error_t *err)
{
  cleanup_free கட்டமைப்பு libcriu_wrapper_s *wrapper = xmalloc0 (அளவு (*wrapper));

#  ifdef STATIC
#    define LOAD_CRIU_FUNCTION(X, ALLOW_NULL) \
      wrapper->X = &X;
#  இல்லையெனில்
#    define LOAD_CRIU_FUNCTION(X, ALLOW_NULL)                                                    \
      செய்                                                                                         \
        {                                                                                        \
          wrapper->X = dlsym (wrapper->handle, #X);                                              \
          எனில் (! ALLOW_NULL && wrapper->X == வெற்று)                                                \
            {                                                                                    \
              dlclose (wrapper->handle);                                                         \
              திரும்பு crun_make_error (err, 0, "could not find symbol `%s` in `libcriu.so`", #X); \
            }                                                                                    \
      } போது (0)
#  endif

#  ifndef STATIC
  wrapper->handle = dlopen ("libcriu.so.2", RTLD_NOW);
  எனில் (wrapper->handle == வெற்று)
    திரும்பு crun_make_error (err, 0, "could not load `libcriu.so.2`: `%s`", dlerror ());
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
  LOAD_CRIU_FUNCTION (criu_set_வேர், false);
  LOAD_CRIU_FUNCTION (criu_add_cg_வேர், false);
  LOAD_CRIU_FUNCTION (criu_set_shell_job, false);
  LOAD_CRIU_FUNCTION (criu_set_tcp_established, false);
  LOAD_CRIU_FUNCTION (criu_set_tcp_close, false);
  LOAD_CRIU_FUNCTION (criu_set_track_mem, false);
  LOAD_CRIU_FUNCTION (criu_set_work_dir_fd, false);
  LOAD_CRIU_FUNCTION (criu_set_lsm_profile, false);
  LOAD_CRIU_FUNCTION (criu_set_lsm_mount_context, false);
#  எனில் ! defined STATIC || defined CRIU_CONFIG_FILE
  LOAD_CRIU_FUNCTION (criu_set_config_file, true);
#  endif

  libcriu_wrapper = *wrapper_out = wrapper;
  wrapper = வெற்று;
#  undef LOAD_CRIU_FUNCTION
  திரும்பு 0;
}

நிலையான வெறுமை
add_restored_ext_mount (மாறிலி எழுத்து *target, unsigned long flags)
{
  restored_ext_mounts = xrealloc (restored_ext_mounts, அளவு (*restored_ext_mounts) * (n_restored_ext_mounts + 1));
  restored_ext_mounts[n_restored_ext_mounts].target = xstrdup (target);
  restored_ext_mounts[n_restored_ext_mounts].flags = flags;
  n_restored_ext_mounts++;
}

நிலையான முழு
criu_notify (எழுத்து *action, criu_notify_arg_t na)
{
  எனில் (strcmp (action, "post-restore") == 0)
    {
      /* CRIU takes the source of an external mount from us and its flags
       * from the image, i.e. from whatever mount was at that path in the
       * container when it was checkpointed.  The container can move its
       * mount points around, so before it is resumed make sure that each
       * source is mounted with the restrictions the configuration asks for. */
      libcrun_error_t tmp_err = வெற்று;
      முழு ret;

      ret = libcrun_restrict_mounts_in_container (libcriu_wrapper->criu_notify_pid (na), restored_ext_mounts,
                                                  n_restored_ext_mounts, &tmp_err);
      எனில் (UNLIKELY (ret < 0))
        {
          libcrun_error (tmp_err->status, "cannot set the flags of the restored mounts: %s", tmp_err->செய்தி);
          libcrun_error_release (&tmp_err);
          திரும்பு ret;
        }
    }

  எனில் (strncmp (action, "orphan-pts-master", 17) == 0)
    {
      /* CRIU sends us the master FD via the 'orphan-pts-master'
       * callback and we are passing it on to the '--console-socket'
       * if it exists. */
      cleanup_close முழு console_socket_fd = -1;
      libcrun_error_t tmp_err = வெற்று;
      முழு master_fd;
      முழு ret;

      எனில் (! console_socket)
        திரும்பு 0;

      master_fd = libcriu_wrapper->criu_get_orphan_pts_master_fd ();

      console_socket_fd = open_unix_domain_client_socket (console_socket, 0, &tmp_err);
      எனில் (UNLIKELY (console_socket_fd < 0))
        {
          libcrun_error_release (&tmp_err);
          திரும்பு console_socket_fd;
        }
      ret = send_fd_to_socket (console_socket_fd, master_fd, &tmp_err);
      எனில் (UNLIKELY (ret < 0))
        {
          libcrun_error_release (&tmp_err);
          திரும்பு ret;
        }
    }
  திரும்பு 0;
}

#  ifdef CRIU_PRE_DUMP_SUPPORT

நிலையான முழு
criu_check_mem_track (libcrun_error_t *err)
{
  கட்டமைப்பு criu_feature_check features = { 0 };
  முழு ret;

  /* Right now we are only interested in checking memory tracking.
   * Memory tracking can be disabled at different levels. aarch64
   * for example has memory tracking not implemented. It could also
   * be not enabled on other architectures. Just ask CRIU if that
   * features exists. */

  features.mem_track = true;

  ret = libcriu_wrapper->criu_feature_check (&features, அளவு (features));
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, 0, "CRIU feature checking failed: %d", ret);

  எனில் (features.mem_track == true)
    திரும்பு 1;

  திரும்பு crun_make_error (err, 0, "CRIU memory tracking not supported");
}

#  endif

நிலையான முழு
register_masked_paths_mounts (runtime_spec_schema_config_schema *def, libcrun_container_t *container,
                              கட்டமைப்பு libcriu_wrapper_s *libcriu_wrapper, bool is_restore, libcrun_error_t *err)
{
  cleanup_free எழுத்து *empty_dir_path = வெற்று;
  bool shared_dir_registered = false;
  size_t i;
  முழு ret;

  க்கு (i = 0; i < def->linux->masked_paths_len; i++)
    {
      கட்டமைப்பு stat statbuf;
      ret = stat (def->linux->masked_paths[i], &statbuf);
      எனில் (ret != 0)
        தொடர்;

      எனில் (S_ISDIR (statbuf.st_mode))
        {
          எனில் (! shared_dir_registered)
            {
              ret = get_shared_empty_directory_path (&empty_dir_path,
                                                     (container->context ? container->context->state_வேர் : வெற்று), err);
              எனில் (UNLIKELY (ret < 0))
                திரும்பு ret;

              ret = libcriu_wrapper->criu_add_ext_mount (empty_dir_path, empty_dir_path);
              எனில் (UNLIKELY (ret < 0))
                திரும்பு crun_make_error (err, -ret, "CRIU: failed adding external mount for shared empty directory `%s`", empty_dir_path);

              shared_dir_registered = true;
            }

          ret = libcriu_wrapper->criu_add_ext_mount (def->linux->masked_paths[i], empty_dir_path);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, -ret, "CRIU: failed adding external mount for masked directory `%s`", def->linux->masked_paths[i]);

          எனில் (is_restore)
            add_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
      இல்லையெனில் எனில் (S_ISREG (statbuf.st_mode))
        {
          மாறிலி எழுத்து *bind_target = is_restore ? "/dev/null" : def->linux->masked_paths[i];
          ret = libcriu_wrapper->criu_add_ext_mount (def->linux->masked_paths[i], bind_target);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", bind_target);

          எனில் (is_restore)
            add_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
    }

  திரும்பு 0;
}

/* Parse one /proc/self/cgroup LINE into its subsystem, normalizing the name and
   stripping any "name=" prefix.  When SUBPATH_OUT is not NULL, the in-hierarchy
   path is stored there.  Returns the subsystem, or NULL if the line carries no
   controller and should be skipped.  LINE is modified in place.  */
நிலையான எழுத்து *
parse_cgroup_subsystem (எழுத்து *வரி, எழுத்து **subpath_out)
{
  எழுத்து *subsystem, *subpath, *it;

  subsystem = strchr (வரி, ':') + 1;
  subpath = strchr (subsystem, ':') + 1;
  *(subpath - 1) = '\0';

  எனில் (subsystem[0] == '\0')
    திரும்பு வெற்று;

  it = strstr (subsystem, "name=");
  எனில் (it)
    subsystem = it + 5;

  எனில் (strcmp (subsystem, "net_prio,net_cls") == 0)
    subsystem = "net_cls,net_prio";
  எனில் (strcmp (subsystem, "cpuacct,cpu") == 0)
    subsystem = "cpu,cpuacct";

  எனில் (subpath_out)
    *subpath_out = subpath;
  திரும்பு subsystem;
}

நிலையான முழு
restore_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free எழுத்து *content = வெற்று;
  bool has_cgroup_mount = false;
  எழுத்து *saveptr = வெற்று;
  முழு cgroup_mode;
  எழுத்து *from;
  முழு ret;
  uint32_t i;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  எனில் (UNLIKELY (cgroup_mode < 0))
    திரும்பு cgroup_mode;

  எனில் (cgroup_mode == CGROUP_MODE_UNIFIED)
    திரும்பு 0;

  /* First check if there is actually a cgroup mount in the container. */
  க்கு (i = 0; i < def->mounts_len; i++)
    {
      எழுத்து *type = def->mounts[i]->type;
      எனில் (type && strcmp (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          நிறுத்து;
        }
    }

  எனில் (! has_cgroup_mount)
    திரும்பு 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, வெற்று, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (UNLIKELY (content == வெற்று || content[0] == '\0'))
    திரும்பு crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  க்கு (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (வெற்று, "\n", &saveptr))
    {
      cleanup_free எழுத்து *destination = வெற்று;
      cleanup_free எழுத்து *source = வெற்று;
      எழுத்து *subsystem;
      எழுத்து *subpath;

      subsystem = parse_cgroup_subsystem (from, &subpath);
      எனில் (subsystem == வெற்று)
        தொடர்;

      ret = append_paths (&source, err, CGROUP_வேர், subsystem, வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = append_paths (&destination, err, source, subpath, வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = libcriu_wrapper->criu_add_ext_mount (source, destination);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", destination);
    }

  திரும்பு 0;
}

நிலையான முழு
checkpoint_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free எழுத்து *content = வெற்று;
  bool has_cgroup_mount = false;
  எழுத்து *saveptr = வெற்று;
  எழுத்து *from;
  முழு ret;
  uint32_t i;

  /* First check if there is actually a cgroup mount in the container. */
  க்கு (i = 0; i < def->mounts_len; i++)
    {
      எழுத்து *type = def->mounts[i]->type;
      எனில் (type && strcmp (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          நிறுத்து;
        }
    }

  எனில் (! has_cgroup_mount)
    திரும்பு 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, வெற்று, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (UNLIKELY (content == வெற்று || content[0] == '\0'))
    திரும்பு crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  க்கு (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (வெற்று, "\n", &saveptr))
    {
      cleanup_free எழுத்து *source_path = வெற்று;
      எழுத்து *subsystem;

      subsystem = parse_cgroup_subsystem (from, வெற்று);
      எனில் (subsystem == வெற்று)
        தொடர்;

      ret = append_paths (&source_path, err, CGROUP_வேர், subsystem, வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = libcriu_wrapper->criu_add_ext_mount (source_path, source_path);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", source_path);
    }

  திரும்பு 0;
}

நிலையான முழு
handle_criu_config_file (libcrun_container_t *container, libcrun_error_t *err)
{
  முழு ret;
  மாறிலி எழுத்து *criu_config_annotation;
  மாறிலி எழுத்து *config_file = CRIU_RUNC_CONFIG_FILE;

  criu_config_annotation = find_annotation (container, "org.criu.config");

  /* Ignore missing criu_set_config_file() API for compatibility with older CRIU versions,
   * and show an error only if config file is explicitly set with annotation.
   */
  எனில் (libcriu_wrapper->criu_set_config_file == வெற்று)
    {
      எனில் (criu_config_annotation)
        திரும்பு crun_make_error (err, 0, "libcriu RPC config files supported in CRIU >= 4.2");

      திரும்பு 0;
    }

  எனில் (criu_config_annotation)
    config_file = criu_config_annotation;
  இல்லையெனில் எனில் (access (CRIU_CRUN_CONFIG_FILE, F_OK) == 0)
    config_file = CRIU_CRUN_CONFIG_FILE;

  ret = libcriu_wrapper->criu_set_config_file (config_file);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, 0, "failed to set CRIU config file");

  திரும்பு 0;
}

நிலையான முழு
validate_criu_version (libcrun_error_t *err)
{
  முழு ret;

  // validate that the libcriu version is at least LIBCRIU_MIN_VERSION
  ret = libcriu_wrapper->criu_check_version (LIBCRIU_MIN_VERSION);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, -ret, "CRIU: failed checking version");

  எனில் (ret == 0)
    திரும்பு crun_make_error (err, 0, "libcriu is too old");

  திரும்பு 0;
}

நிலையான வெறுமை
show_criu_log (மாறிலி எழுத்து *work_path, மாறிலி எழுத்து *log)
{
  cleanup_free எழுத்து *log_path = வெற்று;
  libcrun_error_t tmp_err = வெற்று;
  எழுத்து வரி[1024];
  FILE *f;

  எனில் (UNLIKELY (append_paths (&log_path, &tmp_err, work_path, log, வெற்று) < 0))
    {
      crun_error_release (&tmp_err);
      திரும்பு;
    }

  f = fopen (log_path, "r");
  எனில் (f == வெற்று)
    {
      எனில் (errno != ENOENT)
        libcrun_error (errno, "Can't open CRIU log `%s`", log_path);
      திரும்பு;
    }

  /* Log with error verbosity as this is the default. */
  libcrun_error (0, "--- excerpt from CRIU log `%s`", log_path);
  போது (fgets (வரி, அளவு (வரி), f) != வெற்று)
    எனில் (strstr (வரி, "Error ") != வெற்று)
      {
        வரி[strcspn (வரி, "\n")] = '\0';
        libcrun_error (0, "%s", வரி);
      }

  fclose (f);
  libcrun_error (0, "--- end of excerpt");
}

/* work_dir is the place CRIU will put its logfiles.  If not explicitly set, CRIU
 * will put the logfiles into the images_dir.  When set, create and open it and
 * hand the fd to CRIU; ownership of the returned fd stays with the caller.  */
நிலையான முழு
setup_criu_work_dir (libcrun_checkpoint_restore_t *cr_options, முழு *work_fd_out, libcrun_error_t *err)
{
  முழு work_fd;

  எனில் (cr_options->work_path == வெற்று)
    {
      /* This is only for the error message later. */
      cr_options->work_path = cr_options->image_path;
      திரும்பு 0;
    }

  எனில் (UNLIKELY ((mkdir (cr_options->work_path, 0700) == -1) && (errno != EEXIST)))
    திரும்பு crun_make_error (err, errno, "error creating CRIU work directory `%s`", cr_options->work_path);

  work_fd = open (cr_options->work_path, O_DIRECTORY | O_CLOEXEC);
  எனில் (UNLIKELY (work_fd == -1))
    திரும்பு crun_make_error (err, errno, "error opening CRIU work directory `%s`", cr_options->work_path);

  libcriu_wrapper->criu_set_work_dir_fd (work_fd);
  *work_fd_out = work_fd;
  திரும்பு 0;
}

முழு
libcrun_container_checkpoint_linux_criu (libcrun_container_status_t *status, libcrun_container_t *container,
                                         libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper கட்டமைப்பு libcriu_wrapper_s *wrapper = வெற்று;
  cleanup_free எழுத்து *descriptors_path = வெற்று;
  cleanup_free எழுத்து *freezer_path = வெற்று;
  cleanup_free எழுத்து *path = வெற்று;
  cleanup_close முழு image_fd = -1;
  cleanup_close முழு work_fd = -1;
  முழு cgroup_mode;
  size_t i;
  முழு ret;

  ret = load_wrapper (&wrapper, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (geteuid ())
    திரும்பு crun_make_error (err, 0, "checkpointing requires root");

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
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_version (err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (UNLIKELY (cr_options->image_path == வெற்று))
    திரும்பு crun_make_error (err, 0, "image path not set");

  ret = mkdir (cr_options->image_path, 0700);
  எனில் (UNLIKELY ((ret == -1) && (errno != EEXIST)))
    திரும்பு crun_make_error (err, errno, "error creating checkpoint directory `%s`", cr_options->image_path);

  image_fd = open (cr_options->image_path, O_DIRECTORY | O_CLOEXEC);
  எனில் (UNLIKELY (image_fd == -1))
    திரும்பு crun_make_error (err, errno, "error opening checkpoint directory `%s`", cr_options->image_path);

  libcriu_wrapper->criu_set_images_dir_fd (image_fd);

  /* Set up logging. */
  libcriu_wrapper->criu_set_log_level (4);
  libcriu_wrapper->criu_set_log_file (CRIU_CHECKPOINT_LOG_FILE);

  /* Set up CRIU config file */
  எனில் (UNLIKELY (handle_criu_config_file (container, err)))
    திரும்பு -1;

  /* Setting the pid early as we can skip a lot of checkpoint setup if
   * we just do a pre-dump. The PID needs to be set always. Do it here.
   * The main process of the container is the process CRIU will checkpoint
   * and all of its children. */
  libcriu_wrapper->criu_set_pid (status->pid);

  ret = setup_criu_work_dir (cr_options, &work_fd, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

#  ifdef CRIU_PRE_DUMP_SUPPORT

  {
    முழு criu_can_mem_track = 0;
    /* If the user uses --pre-dump for the second time or does
     * a final dump from a previous pre-dump, setting parent_path
     * is necessary so that CRIU can find which pages have not
     * changed compared to the previous dump. */
    எனில் (cr_options->parent_path != வெற்று)
      {
        criu_can_mem_track = criu_check_mem_track (err);
        எனில் (UNLIKELY (criu_can_mem_track == -1))
          திரும்பு -1;
        libcriu_wrapper->criu_set_track_mem (true);

        /* The parent path must be relative to image path (something like ../previous-dump).
           CRIU will fail with an unclear error message if the path is not right.
         */
        எனில் (UNLIKELY (cr_options->parent_path[0] == '/'))
          திரும்பு crun_make_error (err, 0, "--parent-path must be relative");
        முழு is_dir = crun_dir_p_at (image_fd, cr_options->parent_path, false, err);
        எனில் (UNLIKELY (is_dir <= 0))
          {
            எனில் (is_dir < 0)
              திரும்பு crun_error_wrap (err, "invalid --parent-path");
            திரும்பு crun_make_error (err, ENOTDIR, "invalid --parent-path");
          }
        ret = libcriu_wrapper->criu_set_parent_images (cr_options->parent_path);
        எனில் (UNLIKELY (ret != 0))
          திரும்பு crun_make_error (err, -ret, "error setting CRIU parent images path to `%s`", cr_options->parent_path);
      }

    எனில் (cr_options->pre_dump)
      {
        எனில் (criu_can_mem_track != 1)
          {
            criu_can_mem_track = criu_check_mem_track (err);
            எனில் (UNLIKELY (criu_can_mem_track == -1))
              திரும்பு -1;
          }
        libcriu_wrapper->criu_set_track_mem (true);
        ret = libcriu_wrapper->criu_pre_dump ();
        எனில் (UNLIKELY (ret != 0))
          {
            show_criu_log (cr_options->work_path, CRIU_CHECKPOINT_LOG_FILE);
            திரும்பு crun_make_error (err, 0, "CRIU pre-dump failed: %d", ret);
          }
        திரும்பு 0;
      }
  }
#  endif

  /* descriptors.json is needed during restore to correctly
   * reconnect stdin, stdout, stderr. */
  ret = append_paths (&descriptors_path, err, cr_options->image_path, DESCRIPTORS_FILENAME, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = write_file (descriptors_path, status->external_descriptors, safe_strlen (status->external_descriptors), err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_error_wrap (err, "error saving CRIU descriptors file");

  ret = append_paths (&path, err, status->bundle, status->rootfs, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = libcriu_wrapper->criu_set_வேர் (path);
  எனில் (UNLIKELY (ret != 0))
    திரும்பு crun_make_error (err, 0, "error setting CRIU root to `%s`", path);

  cgroup_mode = libcrun_get_cgroup_mode (err);
  எனில் (UNLIKELY (cgroup_mode < 0))
    திரும்பு cgroup_mode;

  /* For cgroup v1 we need to tell CRIU to handle all cgroup mounts as external mounts. */
  எனில் (cgroup_mode != CGROUP_MODE_UNIFIED)
    {
      ret = checkpoint_cgroup_v1_mount (def, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  /* Tell CRIU about external bind mounts. */
  க்கு (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      எனில் (is_bind_mount (def->mounts[i], வெற்று, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          எழுத்து buf[PATH_MAX];
          மாறிலி எழுத்து *dest_in_வேர்;

          எனில் (nofollow)
            திரும்பு crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_in_வேர் = chroot_realpath (status->rootfs, def->mounts[i]->destination, buf);
          எனில் (UNLIKELY (dest_in_வேர் == வெற்று))
            திரும்பு crun_make_error (err, errno, "unable to resolve external bind mount `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          எனில் (has_prefix (dest_in_வேர், status->rootfs))
            dest_in_வேர் += safe_strlen (status->rootfs);

          ret = libcriu_wrapper->criu_add_ext_mount (dest_in_வேர், dest_in_வேர்);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", def->mounts[i]->destination);
        }
    }

  ret = register_masked_paths_mounts (def, container, libcriu_wrapper, false, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

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

  க்கு (i = 0; i < def->linux->namespaces_len; i++)
    {
      முழு மதிப்பு = libcrun_find_namespace (def->linux->namespaces[i]->type);
      எனில் (UNLIKELY (மதிப்பு < 0))
        திரும்பு crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      எனில் (மதிப்பு == CLONE_NEWNET && def->linux->namespaces[i]->path != வெற்று)
        {
          cleanup_free எழுத்து *external = வெற்று;
          கட்டமைப்பு stat statbuf;

          ret = stat (def->linux->namespaces[i]->path, &statbuf);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "unable to stat(): `%s`", def->linux->namespaces[i]->path);

          xasprintf (&external, "net[%ld]:" CRIU_EXT_NETNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_add_external (external);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }

      எனில் (மதிப்பு == CLONE_NEWPID && def->linux->namespaces[i]->path != வெற்று)
        {
          cleanup_free எழுத்து *external = வெற்று;
          கட்டமைப்பு stat statbuf;

          ret = stat (def->linux->namespaces[i]->path, &statbuf);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, errno, "unable to stat(): `%s`", def->linux->namespaces[i]->path);

          xasprintf (&external, "pid[%ld]:" CRIU_EXT_PIDNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_add_external (external);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }
    }

  /* Tell CRIU to use the freezer to pause all container processes. */
  எனில் (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      /* This needs CRIU 3.14. */
      ret = append_paths (&freezer_path, err, CGROUP_வேர், status->cgroup_path, வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }
  இல்லையெனில்
    {
      ret = append_paths (&freezer_path, err, CGROUP_வேர் "/freezer", status->cgroup_path, வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  ret = libcriu_wrapper->criu_set_freeze_cgroup (freezer_path);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, -ret, "CRIU: failed setting freezer %d", ret);

  /* Set boolean options . */
  libcriu_wrapper->criu_set_leave_running (cr_options->leave_running);
  libcriu_wrapper->criu_set_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_set_shell_job (cr_options->shell_job);
  libcriu_wrapper->criu_set_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_set_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_set_orphan_pts_master (true);
  libcriu_wrapper->criu_set_manage_cgroups_mode (criu_cg_mode_from_public (cr_options->manage_cgroups_mode));

  libcriu_wrapper->criu_set_manage_cgroups (true);

  எனில் (libcriu_wrapper->criu_set_network_lock)
    {
      முழு lock = 0;

      ret = criu_network_lock_from_public (cr_options->network_lock_method, &lock, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      எனில் (lock > 0)
        {
          ret = libcriu_wrapper->criu_set_network_lock (lock);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, 0, "CRIU: failed setting network lock");
        }
    }

  ret = libcriu_wrapper->criu_dump ();
  எனில் (UNLIKELY (ret != 0))
    {
      show_criu_log (cr_options->work_path, CRIU_CHECKPOINT_LOG_FILE);
      திரும்பு crun_make_error (err, ret < 0 ? -ret : 0, "CRIU checkpointing failed: %d", ret);
    }

  திரும்பு 0;
}

நிலையான முழு
prepare_restore_mounts_internal (runtime_spec_schema_config_schema *def, எழுத்து *வேர், மாறிலி எழுத்து **mounted,
                                 size_t *n_mounted, libcrun_error_t *err)
{
  uint32_t i;

  /* Go through all mountpoints to be able to recreate missing mountpoints. */
  க்கு (i = 0; i < def->mounts_len; i++)
    {
      எழுத்து *dest = def->mounts[i]->destination;
      எழுத்து *type = def->mounts[i]->type;
      cleanup_close முழு வேர்_fd = -1;
      bool nofollow = false;
      bool on_tmpfs = false;
      முழு is_dir = 1;
      size_t j;

      /* cgroup restore should be handled by CRIU itself */
      எனில் (type && (strcmp (type, "cgroup") == 0 || strcmp (type, "cgroup2") == 0))
        தொடர்;

      /* Check if the mountpoint is on a tmpfs. CRIU restores
       * all tmpfs. We do need to recreate directories on a tmpfs. */
      size_t dest_len = strlen (dest);
      க்கு (j = 0; j < def->mounts_len; j++)
        {
          எனில் (def->mounts[j]->type == வெற்று || strcmp (def->mounts[j]->type, "tmpfs") != 0)
            தொடர்;
          size_t mount_len = strlen (def->mounts[j]->destination);
          எனில் (mount_len < dest_len && dest[mount_len] == '/' && strncmp (dest, def->mounts[j]->destination, mount_len) == 0)
            {
              /* This is a mountpoint which is on a tmpfs.*/
              on_tmpfs = true;
              நிறுத்து;
            }
        }

      எனில் (on_tmpfs)
        தொடர்;

      /* For bind mounts check if the source is a file or a directory. */
      எனில் (is_bind_mount (def->mounts[i], வெற்று, &nofollow))
        {
          எனில் (nofollow)
            திரும்பு crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          is_dir = crun_dir_p (def->mounts[i]->source, false, err);
          எனில் (UNLIKELY (is_dir < 0))
            திரும்பு is_dir;
        }

      வேர்_fd = open (வேர், O_RDONLY | O_CLOEXEC);
      எனில் (UNLIKELY (வேர்_fd == -1))
        திரும்பு crun_make_error (err, errno, "error opening container root directory `%s`", வேர்);

      எனில் (is_dir)
        {
          முழு ret;

          ret = crun_safe_ensure_directory_at (வேர்_fd, வேர், dest, 0755, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }
      இல்லையெனில்
        {
          முழு ret;

          ret = crun_safe_ensure_file_at (வேர்_fd, வேர், dest, 0755, err);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு ret;
        }

      /* Mount a bind mount source now, so that the mountpoints of the
       * mounts inside it (e.g. a nested bind mount) are created in the
       * source, where CRIU expects them.  This also happens during the
       * initial container creation, as the mounts are done in order.  */
      எனில் (is_bind_mount (def->mounts[i], வெற்று, &nofollow))
        {
          cleanup_close முழு dst_fd = -1;
          proc_fd_path_t fd_path;

          dst_fd = safe_openat (வேர்_fd, வேர், dest, O_PATH | O_CLOEXEC, 0, err);
          எனில் (UNLIKELY (dst_fd < 0))
            திரும்பு dst_fd;

          /* Not recursive: the source might contain ROOT itself (e.g. when
           * it is the bundle directory), and it is not needed anyway.  */
          get_proc_self_fd_path (fd_path, dst_fd);
          எனில் (UNLIKELY (mount (def->mounts[i]->source, fd_path, வெற்று, MS_BIND, வெற்று) < 0))
            திரும்பு crun_make_error (err, errno, "bind mount `%s` to `%s`", def->mounts[i]->source, dest);

          mounted[(*n_mounted)++] = dest;
        }
    }

  திரும்பு 0;
}

/* Move the current process back to CGROUPS, as read from /proc/self/cgroup.
   A failure is not fatal, so only warn about it.  */
நிலையான வெறுமை
move_back_to_cgroups (மாறிலி எழுத்து *cgroups)
{
  libcrun_error_t tmp_err = வெற்று;

  எனில் (UNLIKELY (libcrun_move_self_to_cgroups (cgroups, &tmp_err) < 0))
    {
      libcrun_warning ("cannot move back to the original cgroup: %s", tmp_err->செய்தி);
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
நிலையான முழு
criu_restore_child_in_cgroup (மாறிலி எழுத்து *cgroup_path, முழு *criu_ret, libcrun_error_t *err)
{
  sigset_t all_signals, old_mask;
  முழு wait_status = 0;
  pid_t pid;
  முழு ret;

  *criu_ret = -1;

  sigfillset (&all_signals);
  ret = sigprocmask (SIG_BLOCK, &all_signals, &old_mask);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "sigprocmask");

  /* Must be vfork: the child shares our memory space, so both *criu_ret and
     the error it creates are visible here once it is gone.  */
  pid = vfork ();
  எனில் (UNLIKELY (pid < 0))
    {
      முழு saved_errno = errno;

      sigprocmask (SIG_SETMASK, &old_mask, வெற்று);
      திரும்பு crun_make_error (err, saved_errno, "vfork");
    }

  எனில் (pid == 0)
    {
      கட்டமைப்பு sigaction act;
      முழு i;

      /* The signal dispositions are not shared with the parent, so resetting
         them here does not affect it.  Keep the ignored signals ignored, as
         CRIU would inherit them if it was run directly by the caller.  */
      க்கு (i = 1; i < NSIG; i++)
        {
          எனில் (sigaction (i, வெற்று, &act) < 0 || act.sa_handler == SIG_IGN || act.sa_handler == SIG_DFL)
            தொடர்;

          memset (&act, 0, அளவு (act));
          act.sa_handler = SIG_DFL;
          sigaction (i, &act, வெற்று);
        }
      sigprocmask (SIG_SETMASK, &old_mask, வெற்று);

      எனில் (! is_empty_string (cgroup_path))
        {
          ret = libcrun_move_process_to_cgroup (0, 0, cgroup_path, false, err);
          எனில் (UNLIKELY (ret < 0))
            _safe_exit (EXIT_FAILURE);
        }

      *criu_ret = libcriu_wrapper->criu_restore_child ();
      _safe_exit (EXIT_SUCCESS);
    }

  /* The child is gone by now, it is safe to handle signals again.  */
  sigprocmask (SIG_SETMASK, &old_mask, வெற்று);

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  எனில் (UNLIKELY (ret < 0))
    {
      /* The caller might have reaped the child already, e.g. when it ignores
         SIGCHLD.  Its result is still visible, so use it.  */
      எனில் (errno != ECHILD)
        திரும்பு crun_make_error (err, errno, "waitpid for the CRIU restore process");

      /* The child might have failed before calling CRIU, and left an error.  */
      திரும்பு *err != வெற்று ? -1 : 0;
    }

  ret = get_process_exit_status (wait_status);
  எனில் (UNLIKELY (ret != EXIT_SUCCESS))
    {
      /* The child creates the error in the shared memory space, do not
         overwrite it with crun_make_error().  */
      எனில் (*err == வெற்று)
        திரும்பு crun_make_error (err, 0, "the CRIU restore process exited with status %d", ret);
      திரும்பு -1;
    }

  திரும்பு 0;
}

/* Recreate the mountpoints which do not exist in the container rootfs
 * mounted at ROOT, the same way it is done on the container creation.  */
நிலையான முழு
prepare_restore_mounts (runtime_spec_schema_config_schema *def, எழுத்து *வேர், libcrun_error_t *err)
{
  cleanup_free மாறிலி எழுத்து **mounted = xmalloc0 (அளவு (எழுத்து *) * (def->mounts_len + 1));
  cleanup_close முழு வேர்_fd = -1;
  size_t n_mounted = 0;
  முழு ret;

  ret = prepare_restore_mounts_internal (def, வேர், mounted, &n_mounted, err);

  /* The bind mounts done above are only needed to create the mountpoints,
   * and CRIU restores the mounts itself, so undo them, in reverse order. */
  எனில் (n_mounted > 0)
    வேர்_fd = open (வேர், O_PATH | O_CLOEXEC);
  போது (வேர்_fd >= 0 && n_mounted > 0)
    {
      libcrun_error_t tmp_err = வெற்று;
      மாறிலி எழுத்து *dest = mounted[--n_mounted];
      cleanup_close முழு dst_fd = -1;
      proc_fd_path_t fd_path;

      dst_fd = safe_openat (வேர்_fd, வேர், dest, O_PATH | O_CLOEXEC, 0, &tmp_err);
      எனில் (UNLIKELY (dst_fd < 0))
        {
          crun_error_release (&tmp_err);
          தொடர்;
        }
      get_proc_self_fd_path (fd_path, dst_fd);
      எனில் (UNLIKELY (umount2 (fd_path, MNT_DETACH) < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "unmount `%s`", dest);
    }

  திரும்பு ret;
}

முழு
libcrun_container_restore_linux_criu (libcrun_container_status_t *status, libcrun_container_t *container,
                                      libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper கட்டமைப்பு libcriu_wrapper_s *wrapper = வெற்று;
  cleanup_close முழு inherit_new_net_fd = -1;
  cleanup_close முழு inherit_new_pid_fd = -1;
  cleanup_close முழு image_fd = -1;
  cleanup_free எழுத்து *வேர் = வெற்று;
  cleanup_free எழுத்து *bundle_cleanup = வெற்று;
  cleanup_free எழுத்து *own_cgroups = வெற்று;
  cleanup_close முழு work_fd = -1;
  முழு cgroup_mode;
  முழு criu_ret;
  முழு ret_out;
  size_t i;
  முழு ret;

  ret = load_wrapper (&wrapper, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (geteuid ())
    திரும்பு crun_make_error (err, 0, "restoring requires root");

  ret = libcriu_wrapper->criu_init_opts ();
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_version (err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (UNLIKELY (cr_options->image_path == வெற்று))
    திரும்பு crun_make_error (err, 0, "image path not set");

  image_fd = open (cr_options->image_path, O_DIRECTORY | O_CLOEXEC);
  எனில் (UNLIKELY (image_fd == -1))
    திரும்பு crun_make_error (err, errno, "error opening checkpoint directory `%s`", cr_options->image_path);

  libcriu_wrapper->criu_set_images_dir_fd (image_fd);

  /* Load descriptors.json to tell CRIU where those FDs should be connected to. */
  {
    cleanup_free எழுத்து *descriptors_path = வெற்று;
    cleanup_free எழுத்து *buffer = வெற்று;
    json_object *doc = வெற்று;

    ret = append_paths (&descriptors_path, err, cr_options->image_path, DESCRIPTORS_FILENAME, வெற்று);
    எனில் (UNLIKELY (ret < 0))
      திரும்பு ret;

    ret = read_all_file (descriptors_path, &buffer, வெற்று, err);
    எனில் (UNLIKELY (ret < 0))
      திரும்பு ret;

    /* descriptors.json contains a JSON array with strings
     * telling where 0, 1 and 2 have been initially been
     * pointing to. For each descriptor which points to
     * a pipe 'pipe:' we tell CRIU to reconnect that pipe
     * to the corresponding FD to have (especially) stdout
     * and stderr being correctly redirected. */
    ret = parse_json_file (&doc, buffer, வெற்று, err);
    எனில் (UNLIKELY (ret < 0))
      திரும்பு ret;

    எனில் (json_object_is_type (doc, json_type_array))
      {
        size_t i, len = json_object_array_length (doc);

        /* len will probably always be 3 as crun is currently only
         * recording the destination of FD 0, 1 and 2. */
        க்கு (i = 0; i < len; ++i)
          {
            json_object *s = json_object_array_get_idx (doc, i);
            எனில் (s && json_object_is_type (s, json_type_string))
              {
                மாறிலி எழுத்து *str = json_object_get_string (s);
                எனில் (has_prefix (str, "pipe:"))
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
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = setup_criu_work_dir (cr_options, &work_fd, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (cr_options->lsm_profile != வெற்று)
    {
      ret = libcriu_wrapper->criu_set_lsm_profile (cr_options->lsm_profile);
      எனில் (UNLIKELY (ret != 0))
        திரும்பு crun_make_error (err, -ret, "error setting LSM profile to `%s`", cr_options->lsm_profile);
    }

  எனில் (cr_options->lsm_mount_context != வெற்று)
    {
      ret = libcriu_wrapper->criu_set_lsm_mount_context (cr_options->lsm_mount_context);
      எனில் (UNLIKELY (ret != 0))
        திரும்பு crun_make_error (err, -ret, "error setting LSM mount context to `%s`", cr_options->lsm_mount_context);
    }

  /* do realpath on root */
  bundle_cleanup = realpath (status->bundle, வெற்று);
  எனில் (UNLIKELY (bundle_cleanup == வெற்று))
    bundle_cleanup = xstrdup (status->bundle);

  /* Tell CRIU about external bind mounts. */
  க்கு (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      எனில் (is_bind_mount (def->mounts[i], வெற்று, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          எழுத்து buf[PATH_MAX];
          மாறிலி எழுத்து *dest_in_வேர்;
          மாறிலி எழுத்து *source = def->mounts[i]->source;
          cleanup_free எழுத்து *abs_source = வெற்று;
          unsigned long flags = 0;
          size_t j;

          எனில் (nofollow)
            திரும்பு crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_in_வேர் = chroot_realpath (status->rootfs, def->mounts[i]->destination, buf);
          எனில் (UNLIKELY (dest_in_வேர் == வெற்று))
            திரும்பு crun_make_error (err, errno, "unable to resolve external bind mount destination `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          எனில் (has_prefix (dest_in_வேர், status->rootfs))
            dest_in_வேர் += safe_strlen (status->rootfs);

          /* A relative source is relative to the bundle, while CRIU would
             resolve it relative to its own working directory.  */
          எனில் (source && source[0] != '/')
            {
              ret = append_paths (&abs_source, err, bundle_cleanup, source, வெற்று);
              எனில் (UNLIKELY (ret < 0))
                திரும்பு ret;
              source = abs_source;
            }

          ret = libcriu_wrapper->criu_add_ext_mount (dest_in_வேர், source);
          எனில் (UNLIKELY (ret < 0))
            திரும்பு crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", source);

          க்கு (j = 0; j < def->mounts[i]->options_len; j++)
            {
              மாறிலி கட்டமைப்பு propagation_flags_s *prop = வெற்று;

              எனில் (def->mounts[i]->options[j])
                prop = libcrun_str2mount_flags (def->mounts[i]->options[j]);
              எனில் (prop && ! prop->clear)
                flags |= prop->flags;
            }
          add_restored_ext_mount (dest_in_வேர், flags);
        }
    }

  ret = register_masked_paths_mounts (def, container, libcriu_wrapper, true, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  /* Mount the container rootfs for CRIU. */
  ret = append_paths (&வேர், err, bundle_cleanup, "criu-root", வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = mkdir (வேர், 0755);
  எனில் (UNLIKELY (ret == -1))
    திரும்பு crun_make_error (err, errno, "error creating restore directory `%s`", வேர்);

  ret = mount (status->rootfs, வேர், வெற்று, MS_BIND | MS_REC, வெற்று);
  எனில் (UNLIKELY (ret == -1))
    {
      ret = crun_make_error (err, errno, "error mounting restore directory `%s`", வேர்);
      செல் out;
    }

  /* During initial container creation, crun will create mountpoints
   * defined in config.json if they do not exist. If we are restoring
   * we need to make sure these mountpoints also exist.
   * This is not perfect, as this means crun will modify a rootfs
   * even if it marked as read-only, but runc already modifies
   * the rootfs in the same way. */

  ret = prepare_restore_mounts (def, வேர், err);
  எனில் (UNLIKELY (ret < 0))
    செல் out_umount;

  ret = libcriu_wrapper->criu_set_வேர் (வேர்);
  எனில் (UNLIKELY (ret != 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU root to `%s`", வேர்);
      செல் out_umount;
    }

  /* If a namespace defined in config.json we are telling
   * CRIU use that namespace when restoring the process tree.
   *
   * CRIU expects the information about the namespace like this:
   * --inherit-fd fd[<fd>]:<key>
   * The <key> needs to be the same as during checkpointing (extRootNetNS). */
  க்கு (i = 0; i < def->linux->namespaces_len; i++)
    {
      மாறிலி முழு open_flags_for_inherit = O_RDONLY; /* Cannot be O_CLOEXEC as it is passed to the child process. */
      முழு மதிப்பு = libcrun_find_namespace (def->linux->namespaces[i]->type);
      எனில் (UNLIKELY (மதிப்பு < 0))
        {
          ret = crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);
          செல் out_umount;
        }

      எனில் (மதிப்பு == CLONE_NEWNET && def->linux->namespaces[i]->path != வெற்று)
        {
          inherit_new_net_fd = open (def->linux->namespaces[i]->path, open_flags_for_inherit);
          எனில் (UNLIKELY (inherit_new_net_fd < 0))
            {
              ret = crun_make_error (err, errno, "unable to open(): `%s`", def->linux->namespaces[i]->path);
              செல் out_umount;
            }

          ret = libcriu_wrapper->criu_add_inherit_fd (inherit_new_net_fd, CRIU_EXT_NETNS);
          எனில் (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              செல் out_umount;
            }
        }

      எனில் (மதிப்பு == CLONE_NEWPID && def->linux->namespaces[i]->path != வெற்று)
        {
          inherit_new_pid_fd = open (def->linux->namespaces[i]->path, open_flags_for_inherit);
          எனில் (UNLIKELY (inherit_new_pid_fd < 0))
            {
              ret = crun_make_error (err, errno, "unable to open(): `%s`", def->linux->namespaces[i]->path);
              செல் out_umount;
            }

          ret = libcriu_wrapper->criu_add_inherit_fd (inherit_new_pid_fd, CRIU_EXT_PIDNS);
          எனில் (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              செல் out_umount;
            }
        }

#  ifdef CRIU_JOIN_NS_SUPPORT
      எனில் (மதிப்பு == CLONE_NEWTIME && def->linux->namespaces[i]->path != வெற்று)
        {
          எனில் (libcriu_wrapper->criu_join_ns_add == வெற்று)
            {
              ret = crun_make_error (err, 0, "shared time namespace restore is supported in CRIU >= 3.16.1");
              செல் out_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_add ("time", def->linux->namespaces[i]->path, வெற்று);
          எனில் (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->path);
              செல் out_umount;
            }
        }

      எனில் (மதிப்பு == CLONE_NEWIPC && def->linux->namespaces[i]->path != வெற்று)
        {
          எனில் (libcriu_wrapper->criu_join_ns_add == வெற்று)
            {
              ret = crun_make_error (err, 0, "shared ipc namespace restore is supported in CRIU >= 3.16.1");
              செல் out_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_add ("ipc", def->linux->namespaces[i]->path, வெற்று);
          எனில் (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->path);
              செல் out_umount;
            }
        }

      எனில் (மதிப்பு == CLONE_NEWUTS && def->linux->namespaces[i]->path != வெற்று)
        {
          எனில் (libcriu_wrapper->criu_join_ns_add == வெற்று)
            {
              ret = crun_make_error (err, 0, "shared uts namespace restore is supported in CRIU >= 3.16.1");
              செல் out_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_add ("uts", def->linux->namespaces[i]->path, வெற்று);
          எனில் (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->path);
              செல் out_umount;
            }
        }
#  endif
    }

  /* Set up CRIU config file */
  ret = handle_criu_config_file (container, err);
  எனில் (UNLIKELY (ret < 0))
    செல் out_umount;

  /* Tell CRIU if cgroup v1 needs to be handled. */
  ret = restore_cgroup_v1_mount (def, err);
  எனில் (UNLIKELY (ret < 0))
    செல் out_umount;

  console_socket = cr_options->console_socket;
  libcriu_wrapper->criu_set_notify_cb (criu_notify);

  /* Set boolean options . */
  libcriu_wrapper->criu_set_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_set_shell_job (cr_options->shell_job);
  libcriu_wrapper->criu_set_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_set_tcp_close (cr_options->tcp_close);
  libcriu_wrapper->criu_set_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_set_orphan_pts_master (true);

  எனில் (status->cgroup_path)
    {
      cgroup_mode = libcrun_get_cgroup_mode (err);
      எனில் (UNLIKELY (cgroup_mode < 0))
        {
          ret = cgroup_mode;
          செல் out_umount;
        }

      /* With cgroup v2, the cgroup path is only meaningful for the unified
         hierarchy, so only relocate that one.  A NULL controller would make
         CRIU relocate every hierarchy the container is in, named cgroup v1
         ones included.  As CRIU dumps such a hierarchy from wherever the
         container is in it, which is its root, as crun leaves named
         hierarchies alone, every checkpoint and restore would then copy the
         whole hierarchy into itself.  */
      ret = libcriu_wrapper->criu_add_cg_வேர் (cgroup_mode == CGROUP_MODE_UNIFIED ? "" : வெற்று,
                                               status->cgroup_path);
      எனில் (UNLIKELY (ret != 0))
        {
          ret = crun_make_error (err, 0, "error setting CRIU cgroup root to `%s`", status->cgroup_path);
          செல் out_umount;
        }
    }

  libcriu_wrapper->criu_set_manage_cgroups_mode (criu_cg_mode_from_public (cr_options->manage_cgroups_mode));
  libcriu_wrapper->criu_set_manage_cgroups (true);

  எனில் (libcriu_wrapper->criu_set_network_lock)
    {
      முழு lock = 0;

      ret = criu_network_lock_from_public (cr_options->network_lock_method, &lock, err);
      எனில் (UNLIKELY (ret < 0))
        செல் out_umount;

      எனில் (lock > 0)
        {
          ret = libcriu_wrapper->criu_set_network_lock (lock);
          எனில் (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "CRIU: failed setting network lock");
              செல் out_umount;
            }
        }
    }

  libcriu_wrapper->criu_set_log_level (4);
  ret = libcriu_wrapper->criu_set_log_file (CRIU_RESTORE_LOG_FILE);
  எனில் (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU log file to `%s`", CRIU_RESTORE_LOG_FILE);
      செல் out_umount;
    }

  /* criu_restore() returns the PID of the process of the restored process
   * tree. This PID will not be the same as status->pid if the container is
   * running in a PID namespace. But it will always be > 0. */
  எனில் (cr_options->detach)
    {
      /* Nothing waits for the container, so there is no reason to take over
         this process: use a throw-away one.  It is used even when there is no
         cgroup to join, so that the container is not left as a child of a
         process which is not going to reap it.  */
      ret = criu_restore_child_in_cgroup (status->cgroup_path, &criu_ret, err);
      எனில் (UNLIKELY (ret < 0))
        செல் out_umount;

      ret = criu_ret;
    }
  இல்லையெனில்
    {
      /* Like runc does, put CRIU into the container cgroup for the time of
       * restore, so the restored tasks are created there. It is necessary in
       * the "ignore" mode, where CRIU does not deal with cgroups at all, and
       * does not hurt otherwise. As CRIU is our child, do it by moving
       * ourselves there, and move back once the restore is done. */
      எனில் (! is_empty_string (status->cgroup_path))
        {
          ret = read_all_file (PROC_SELF_CGROUP, &own_cgroups, வெற்று, err);
          எனில் (UNLIKELY (ret < 0))
            செல் out_umount;

          ret = libcrun_move_process_to_cgroup (0, 0, status->cgroup_path, false, err);
          எனில் (UNLIKELY (ret < 0))
            {
              /* Some of the cgroups might have been joined already.  */
              move_back_to_cgroups (own_cgroups);
              செல் out_umount;
            }
        }

      ret = libcriu_wrapper->criu_restore_child ();

      எனில் (own_cgroups)
        move_back_to_cgroups (own_cgroups);
    }

  எனில் (UNLIKELY (ret <= 0))
    {
      show_criu_log (cr_options->work_path, CRIU_RESTORE_LOG_FILE);
      ret = crun_make_error (err, 0, "CRIU restoring failed: %d", ret);
      செல் out_umount;
    }

  /* Update the status struct with the newly allocated PID. This will
   * be necessary later when moving the process into its cgroup. */
  status->pid = ret;
  ret = 0;

out_umount:
  ret_out = umount (வேர்);
  எனில் (UNLIKELY (ret_out == -1))
    {
      முழு saved_errno = errno;
      rmdir (வேர்);
      எனில் (ret < 0)
        திரும்பு crun_error_wrap (err, "error unmounting restore directory `%s`", வேர்);
      இல்லையெனில்
        திரும்பு crun_make_error (err, saved_errno, "error unmounting restore directory `%s`", வேர்);
    }
out:
  ret_out = rmdir (வேர்);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;
  எனில் (UNLIKELY (ret_out == -1))
    திரும்பு crun_make_error (err, errno, "error removing restore directory `%s`", வேர்);
  திரும்பு ret;
}
#endif
