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
#घोषणा _GNU_SOURCE

#समावेश <config.h>

#अगर HAVE_CRIU && HAVE_DLOPEN

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
अक्षर *chroot_realpath (स्थिर अक्षर *chroot, स्थिर अक्षर *path, अक्षर resolved_path[]);

/* Map the public cgroups mode (enum libcrun_cr_cgroups_mode) onto the CRIU enum.  */
अटल क्रमागत criu_cg_mode
criu_cg_mode_from_public (पूर्णांक mode)
{
  चयन (mode)
    {
    हाल LIBCRUN_CR_CG_MODE_IGNORE:
      वापस CRIU_CG_MODE_IGNORE;
    हाल LIBCRUN_CR_CG_MODE_FULL:
      वापस CRIU_CG_MODE_FULL;
    हाल LIBCRUN_CR_CG_MODE_STRICT:
      वापस CRIU_CG_MODE_STRICT;
    हाल LIBCRUN_CR_CG_MODE_DEFAULT:
    हाल LIBCRUN_CR_CG_MODE_SOFT:
    शेष:
      /* Default to CRIU_CG_MODE_SOFT, just as runc.  */
      वापस CRIU_CG_MODE_SOFT;
    }
}

/* Map the public network lock method (enum libcrun_cr_network_lock_method) onto the
   CRIU enum.  *LOCK is set to 0 when no explicit method must be set (CRIU default).
   A method that this build cannot honor is an error, never a silent fallback to
   the CRIU default.  */
अटल पूर्णांक
criu_network_lock_from_public (पूर्णांक method, पूर्णांक *lock, libcrun_error_t *err)
{
  चयन (method)
    {
    हाल LIBCRUN_CR_NETWORK_LOCK_IPTABLES:
      *lock = CRIU_NETWORK_LOCK_IPTABLES;
      वापस 0;
    हाल LIBCRUN_CR_NETWORK_LOCK_NFTABLES:
      *lock = CRIU_NETWORK_LOCK_NFTABLES;
      वापस 0;
    हाल LIBCRUN_CR_NETWORK_LOCK_SKIP:
#  अगर CRIU_NETWORK_LOCK_SKIP_SUPPORT
      *lock = CRIU_NETWORK_LOCK_SKIP;
      वापस 0;
#  अन्यथा
      वापस crun_make_error (err, 0, "CRIU: the `skip` network lock method is not supported by this build");
#  endif
    हाल LIBCRUN_CR_NETWORK_LOCK_DEFAULT:
      *lock = 0;
      वापस 0;
    शेष:
      वापस crun_make_error (err, 0, "CRIU: unknown network lock method %d", method);
    }
}

अटल स्थिर अक्षर *console_socket = शून्य;

/* The external mounts CRIU restores, and the flags the configuration asks
   for each of them.  */
अटल काष्ठा libcrun_mount_flags_s *restored_ext_mounts = शून्य;
अटल माप_प्रकार n_restored_ext_mounts = 0;

#  define LIBCRIU_MIN_VERSION 31500

काष्ठा libcriu_wrapper_s
{
  व्योम *handle;
  पूर्णांक (*criu_add_ext_mount) (स्थिर अक्षर *कुंजी, स्थिर अक्षर *val);
  पूर्णांक (*criu_add_external) (स्थिर अक्षर *कुंजी);
  पूर्णांक (*criu_add_inherit_fd) (पूर्णांक fd, स्थिर अक्षर *कुंजी);
  पूर्णांक (*criu_check_version) (पूर्णांक minimum);
  पूर्णांक (*criu_dump) (व्योम);
  पूर्णांक (*criu_get_orphan_pts_master_fd) (व्योम);
  पूर्णांक (*criu_init_opts) (व्योम);
  पूर्णांक (*criu_notify_pid) (criu_notify_arg_t na);
#  ifdef CRIU_JOIN_NS_SUPPORT
  पूर्णांक (*criu_join_ns_add) (स्थिर अक्षर *ns, स्थिर अक्षर *ns_file, स्थिर अक्षर *extra_opt);
#  endif
#  ifdef CRIU_PRE_DUMP_SUPPORT
  पूर्णांक (*criu_feature_check) (काष्ठा criu_feature_check *features, माप_प्रकार आकार);
  पूर्णांक (*criu_pre_dump) (व्योम);
#  endif
  पूर्णांक (*criu_restore_child) (व्योम);
  पूर्णांक (*criu_set_freeze_cgroup) (स्थिर अक्षर *name);
  व्योम (*criu_set_file_locks) (bool file_locks);
  व्योम (*criu_set_ext_unix_sk) (bool ext_unix_sk);
  पूर्णांक (*criu_set_log_file) (स्थिर अक्षर *log_file);
  व्योम (*criu_set_log_level) (पूर्णांक log_level);
  व्योम (*criu_set_leave_running) (bool leave_running);
  व्योम (*criu_set_manage_cgroups) (bool manage);
  व्योम (*criu_set_manage_cgroups_mode) (क्रमागत criu_cg_mode mode);
  पूर्णांक (*criu_set_network_lock) (क्रमागत criu_network_lock_method method);
  व्योम (*criu_set_notify_cb) (पूर्णांक (*cb) (अक्षर *action, criu_notify_arg_t na));
  व्योम (*criu_set_orphan_pts_master) (bool orphan_pts_master);
  व्योम (*criu_set_images_dir_fd) (पूर्णांक fd);
  पूर्णांक (*criu_set_parent_images) (स्थिर अक्षर *path);
  व्योम (*criu_set_pid) (पूर्णांक pid);
  पूर्णांक (*criu_set_मूल) (स्थिर अक्षर *मूल);
  पूर्णांक (*criu_add_cg_मूल) (स्थिर अक्षर *ctrl, स्थिर अक्षर *path);
  व्योम (*criu_set_shell_job) (bool shell_job);
  व्योम (*criu_set_tcp_established) (bool tcp_established);
  व्योम (*criu_set_tcp_close) (bool tcp_close);
  व्योम (*criu_set_track_mem) (bool track_mem);
  व्योम (*criu_set_work_dir_fd) (पूर्णांक fd);
  पूर्णांक (*criu_set_lsm_profile) (स्थिर अक्षर *name);
  पूर्णांक (*criu_set_lsm_mount_context) (स्थिर अक्षर *name);
  पूर्णांक (*criu_set_config_file) (स्थिर अक्षर *path);
};

अटल काष्ठा libcriu_wrapper_s *libcriu_wrapper;

अटल अंतरभूत व्योम
cleanup_wrapper (व्योम *p)
{
  काष्ठा libcriu_wrapper_s **w;

  w = (काष्ठा libcriu_wrapper_s **) p;
  अगर (*w == शून्य)
    वापस;

#  ifndef STATIC
  अगर ((*w)->handle)
    dlclose ((*w)->handle);
#  endif
  मुक्त (*w);
  libcriu_wrapper = शून्य;

  जबतक (n_restored_ext_mounts > 0)
    मुक्त (restored_ext_mounts[--n_restored_ext_mounts].target);
  मुक्त (restored_ext_mounts);
  restored_ext_mounts = शून्य;
}

#  define cleanup_wrapper __attribute__ ((cleanup (cleanup_wrapper)))

अटल पूर्णांक
load_wrapper (काष्ठा libcriu_wrapper_s **wrapper_out, libcrun_error_t *err)
{
  cleanup_free काष्ठा libcriu_wrapper_s *wrapper = xmalloc0 (माप (*wrapper));

#  ifdef STATIC
#    define LOAD_CRIU_FUNCTION(X, ALLOW_NULL) \
      wrapper->X = &X;
#  अन्यथा
#    define LOAD_CRIU_FUNCTION(X, ALLOW_NULL)                                                    \
      करो                                                                                         \
        {                                                                                        \
          wrapper->X = dlsym (wrapper->handle, #X);                                              \
          अगर (! ALLOW_NULL && wrapper->X == शून्य)                                                \
            {                                                                                    \
              dlclose (wrapper->handle);                                                         \
              वापस crun_make_error (err, 0, "could not find symbol `%s` in `libcriu.so`", #X); \
            }                                                                                    \
      } जबतक (0)
#  endif

#  ifndef STATIC
  wrapper->handle = dlopen ("libcriu.so.2", RTLD_NOW);
  अगर (wrapper->handle == शून्य)
    वापस crun_make_error (err, 0, "could not load `libcriu.so.2`: `%s`", dlerror ());
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
  LOAD_CRIU_FUNCTION (criu_set_मूल, false);
  LOAD_CRIU_FUNCTION (criu_add_cg_मूल, false);
  LOAD_CRIU_FUNCTION (criu_set_shell_job, false);
  LOAD_CRIU_FUNCTION (criu_set_tcp_established, false);
  LOAD_CRIU_FUNCTION (criu_set_tcp_close, false);
  LOAD_CRIU_FUNCTION (criu_set_track_mem, false);
  LOAD_CRIU_FUNCTION (criu_set_work_dir_fd, false);
  LOAD_CRIU_FUNCTION (criu_set_lsm_profile, false);
  LOAD_CRIU_FUNCTION (criu_set_lsm_mount_context, false);
#  अगर ! defined STATIC || defined CRIU_CONFIG_FILE
  LOAD_CRIU_FUNCTION (criu_set_config_file, true);
#  endif

  libcriu_wrapper = *wrapper_out = wrapper;
  wrapper = शून्य;
#  undef LOAD_CRIU_FUNCTION
  वापस 0;
}

अटल व्योम
add_restored_ext_mount (स्थिर अक्षर *target, अचिन्हित दीर्घ flags)
{
  restored_ext_mounts = xrealloc (restored_ext_mounts, माप (*restored_ext_mounts) * (n_restored_ext_mounts + 1));
  restored_ext_mounts[n_restored_ext_mounts].target = xstrdup (target);
  restored_ext_mounts[n_restored_ext_mounts].flags = flags;
  n_restored_ext_mounts++;
}

अटल पूर्णांक
criu_notify (अक्षर *action, criu_notify_arg_t na)
{
  अगर (म_भेद (action, "post-restore") == 0)
    {
      /* CRIU takes the source of an external mount from us and its flags
       * from the image, i.e. from whatever mount was at that path in the
       * container when it was checkpointed.  The container can move its
       * mount points around, so before it is resumed make sure that each
       * source is mounted with the restrictions the configuration asks for. */
      libcrun_error_t tmp_err = शून्य;
      पूर्णांक ret;

      ret = libcrun_restrict_mounts_in_container (libcriu_wrapper->criu_notify_pid (na), restored_ext_mounts,
                                                  n_restored_ext_mounts, &tmp_err);
      अगर (UNLIKELY (ret < 0))
        {
          libcrun_error (tmp_err->status, "cannot set the flags of the restored mounts: %s", tmp_err->संदेश);
          libcrun_error_release (&tmp_err);
          वापस ret;
        }
    }

  अगर (म_भेदन (action, "orphan-pts-master", 17) == 0)
    {
      /* CRIU sends us the master FD via the 'orphan-pts-master'
       * callback and we are passing it on to the '--console-socket'
       * if it exists. */
      cleanup_close पूर्णांक console_socket_fd = -1;
      libcrun_error_t tmp_err = शून्य;
      पूर्णांक master_fd;
      पूर्णांक ret;

      अगर (! console_socket)
        वापस 0;

      master_fd = libcriu_wrapper->criu_get_orphan_pts_master_fd ();

      console_socket_fd = open_unix_domain_client_socket (console_socket, 0, &tmp_err);
      अगर (UNLIKELY (console_socket_fd < 0))
        {
          libcrun_error_release (&tmp_err);
          वापस console_socket_fd;
        }
      ret = send_fd_to_socket (console_socket_fd, master_fd, &tmp_err);
      अगर (UNLIKELY (ret < 0))
        {
          libcrun_error_release (&tmp_err);
          वापस ret;
        }
    }
  वापस 0;
}

#  ifdef CRIU_PRE_DUMP_SUPPORT

अटल पूर्णांक
criu_check_mem_track (libcrun_error_t *err)
{
  काष्ठा criu_feature_check features = { 0 };
  पूर्णांक ret;

  /* Right now we are only interested in checking memory tracking.
   * Memory tracking can be disabled at different levels. aarch64
   * for example has memory tracking not implemented. It could also
   * be not enabled on other architectures. Just ask CRIU if that
   * features exists. */

  features.mem_track = true;

  ret = libcriu_wrapper->criu_feature_check (&features, माप (features));
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, 0, "CRIU feature checking failed: %d", ret);

  अगर (features.mem_track == true)
    वापस 1;

  वापस crun_make_error (err, 0, "CRIU memory tracking not supported");
}

#  endif

अटल पूर्णांक
register_masked_paths_mounts (runtime_spec_schema_config_schema *def, libcrun_container_t *container,
                              काष्ठा libcriu_wrapper_s *libcriu_wrapper, bool is_restore, libcrun_error_t *err)
{
  cleanup_free अक्षर *empty_dir_path = शून्य;
  bool shared_dir_registered = false;
  माप_प्रकार i;
  पूर्णांक ret;

  क्रम (i = 0; i < def->linux->masked_paths_len; i++)
    {
      काष्ठा stat statbuf;
      ret = stat (def->linux->masked_paths[i], &statbuf);
      अगर (ret != 0)
        जारी;

      अगर (S_ISDIR (statbuf.st_mode))
        {
          अगर (! shared_dir_registered)
            {
              ret = get_shared_empty_directory_path (&empty_dir_path,
                                                     (container->context ? container->context->state_मूल : शून्य), err);
              अगर (UNLIKELY (ret < 0))
                वापस ret;

              ret = libcriu_wrapper->criu_add_ext_mount (empty_dir_path, empty_dir_path);
              अगर (UNLIKELY (ret < 0))
                वापस crun_make_error (err, -ret, "CRIU: failed adding external mount for shared empty directory `%s`", empty_dir_path);

              shared_dir_registered = true;
            }

          ret = libcriu_wrapper->criu_add_ext_mount (def->linux->masked_paths[i], empty_dir_path);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, -ret, "CRIU: failed adding external mount for masked directory `%s`", def->linux->masked_paths[i]);

          अगर (is_restore)
            add_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
      अन्यथा अगर (S_ISREG (statbuf.st_mode))
        {
          स्थिर अक्षर *bind_target = is_restore ? "/dev/null" : def->linux->masked_paths[i];
          ret = libcriu_wrapper->criu_add_ext_mount (def->linux->masked_paths[i], bind_target);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", bind_target);

          अगर (is_restore)
            add_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
    }

  वापस 0;
}

/* Parse one /proc/self/cgroup LINE into its subsystem, normalizing the name and
   stripping any "name=" prefix.  When SUBPATH_OUT is not NULL, the in-hierarchy
   path is stored there.  Returns the subsystem, or NULL if the line carries no
   controller and should be skipped.  LINE is modified in place.  */
अटल अक्षर *
parse_cgroup_subsystem (अक्षर *पंक्ति, अक्षर **subpath_out)
{
  अक्षर *subsystem, *subpath, *it;

  subsystem = म_अक्षर (पंक्ति, ':') + 1;
  subpath = म_अक्षर (subsystem, ':') + 1;
  *(subpath - 1) = '\0';

  अगर (subsystem[0] == '\0')
    वापस शून्य;

  it = म_माला (subsystem, "name=");
  अगर (it)
    subsystem = it + 5;

  अगर (म_भेद (subsystem, "net_prio,net_cls") == 0)
    subsystem = "net_cls,net_prio";
  अगर (म_भेद (subsystem, "cpuacct,cpu") == 0)
    subsystem = "cpu,cpuacct";

  अगर (subpath_out)
    *subpath_out = subpath;
  वापस subsystem;
}

अटल पूर्णांक
restore_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free अक्षर *content = शून्य;
  bool has_cgroup_mount = false;
  अक्षर *saveptr = शून्य;
  पूर्णांक cgroup_mode;
  अक्षर *from;
  पूर्णांक ret;
  uint32_t i;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  अगर (UNLIKELY (cgroup_mode < 0))
    वापस cgroup_mode;

  अगर (cgroup_mode == CGROUP_MODE_UNIFIED)
    वापस 0;

  /* First check if there is actually a cgroup mount in the container. */
  क्रम (i = 0; i < def->mounts_len; i++)
    {
      अक्षर *type = def->mounts[i]->type;
      अगर (type && म_भेद (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          अवरोध;
        }
    }

  अगर (! has_cgroup_mount)
    वापस 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, शून्य, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (UNLIKELY (content == शून्य || content[0] == '\0'))
    वापस crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  क्रम (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (शून्य, "\n", &saveptr))
    {
      cleanup_free अक्षर *destination = शून्य;
      cleanup_free अक्षर *source = शून्य;
      अक्षर *subsystem;
      अक्षर *subpath;

      subsystem = parse_cgroup_subsystem (from, &subpath);
      अगर (subsystem == शून्य)
        जारी;

      ret = append_paths (&source, err, CGROUP_मूल, subsystem, शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = append_paths (&destination, err, source, subpath, शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = libcriu_wrapper->criu_add_ext_mount (source, destination);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", destination);
    }

  वापस 0;
}

अटल पूर्णांक
checkpoint_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free अक्षर *content = शून्य;
  bool has_cgroup_mount = false;
  अक्षर *saveptr = शून्य;
  अक्षर *from;
  पूर्णांक ret;
  uint32_t i;

  /* First check if there is actually a cgroup mount in the container. */
  क्रम (i = 0; i < def->mounts_len; i++)
    {
      अक्षर *type = def->mounts[i]->type;
      अगर (type && म_भेद (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          अवरोध;
        }
    }

  अगर (! has_cgroup_mount)
    वापस 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, शून्य, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (UNLIKELY (content == शून्य || content[0] == '\0'))
    वापस crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  क्रम (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (शून्य, "\n", &saveptr))
    {
      cleanup_free अक्षर *source_path = शून्य;
      अक्षर *subsystem;

      subsystem = parse_cgroup_subsystem (from, शून्य);
      अगर (subsystem == शून्य)
        जारी;

      ret = append_paths (&source_path, err, CGROUP_मूल, subsystem, शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = libcriu_wrapper->criu_add_ext_mount (source_path, source_path);
      अगर (UNLIKELY (ret < 0))
        वापस crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", source_path);
    }

  वापस 0;
}

अटल पूर्णांक
handle_criu_config_file (libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णांक ret;
  स्थिर अक्षर *criu_config_annotation;
  स्थिर अक्षर *config_file = CRIU_RUNC_CONFIG_FILE;

  criu_config_annotation = find_annotation (container, "org.criu.config");

  /* Ignore missing criu_set_config_file() API for compatibility with older CRIU versions,
   * and show an error only if config file is explicitly set with annotation.
   */
  अगर (libcriu_wrapper->criu_set_config_file == शून्य)
    {
      अगर (criu_config_annotation)
        वापस crun_make_error (err, 0, "libcriu RPC config files supported in CRIU >= 4.2");

      वापस 0;
    }

  अगर (criu_config_annotation)
    config_file = criu_config_annotation;
  अन्यथा अगर (access (CRIU_CRUN_CONFIG_FILE, F_OK) == 0)
    config_file = CRIU_CRUN_CONFIG_FILE;

  ret = libcriu_wrapper->criu_set_config_file (config_file);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, 0, "failed to set CRIU config file");

  वापस 0;
}

अटल पूर्णांक
validate_criu_version (libcrun_error_t *err)
{
  पूर्णांक ret;

  // validate that the libcriu version is at least LIBCRIU_MIN_VERSION
  ret = libcriu_wrapper->criu_check_version (LIBCRIU_MIN_VERSION);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, -ret, "CRIU: failed checking version");

  अगर (ret == 0)
    वापस crun_make_error (err, 0, "libcriu is too old");

  वापस 0;
}

अटल व्योम
show_criu_log (स्थिर अक्षर *work_path, स्थिर अक्षर *log)
{
  cleanup_free अक्षर *log_path = शून्य;
  libcrun_error_t tmp_err = शून्य;
  अक्षर पंक्ति[1024];
  खाता *f;

  अगर (UNLIKELY (append_paths (&log_path, &tmp_err, work_path, log, शून्य) < 0))
    {
      crun_error_release (&tmp_err);
      वापस;
    }

  f = ख_खोलो (log_path, "r");
  अगर (f == शून्य)
    {
      अगर (त्रुटि_सं != ENOENT)
        libcrun_error (त्रुटि_सं, "Can't open CRIU log `%s`", log_path);
      वापस;
    }

  /* Log with error verbosity as this is the default. */
  libcrun_error (0, "--- excerpt from CRIU log `%s`", log_path);
  जबतक (ख_माला_लो (पंक्ति, माप (पंक्ति), f) != शून्य)
    अगर (म_माला (पंक्ति, "Error ") != शून्य)
      {
        पंक्ति[म_खोज (पंक्ति, "\n")] = '\0';
        libcrun_error (0, "%s", पंक्ति);
      }

  ख_बंद (f);
  libcrun_error (0, "--- end of excerpt");
}

/* work_dir is the place CRIU will put its logfiles.  If not explicitly set, CRIU
 * will put the logfiles into the images_dir.  When set, create and open it and
 * hand the fd to CRIU; ownership of the returned fd stays with the caller.  */
अटल पूर्णांक
setup_criu_work_dir (libcrun_checkpoint_restore_t *cr_options, पूर्णांक *work_fd_out, libcrun_error_t *err)
{
  पूर्णांक work_fd;

  अगर (cr_options->work_path == शून्य)
    {
      /* This is only for the error message later. */
      cr_options->work_path = cr_options->image_path;
      वापस 0;
    }

  अगर (UNLIKELY ((सूची_गढ़ो (cr_options->work_path, 0700) == -1) && (त्रुटि_सं != EEXIST)))
    वापस crun_make_error (err, त्रुटि_सं, "error creating CRIU work directory `%s`", cr_options->work_path);

  work_fd = खोलो (cr_options->work_path, O_DIRECTORY | O_CLOEXEC);
  अगर (UNLIKELY (work_fd == -1))
    वापस crun_make_error (err, त्रुटि_सं, "error opening CRIU work directory `%s`", cr_options->work_path);

  libcriu_wrapper->criu_set_work_dir_fd (work_fd);
  *work_fd_out = work_fd;
  वापस 0;
}

पूर्णांक
libcrun_container_checkpoint_linux_criu (libcrun_container_status_t *status, libcrun_container_t *container,
                                         libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper काष्ठा libcriu_wrapper_s *wrapper = शून्य;
  cleanup_free अक्षर *descriptors_path = शून्य;
  cleanup_free अक्षर *freezer_path = शून्य;
  cleanup_free अक्षर *path = शून्य;
  cleanup_close पूर्णांक image_fd = -1;
  cleanup_close पूर्णांक work_fd = -1;
  पूर्णांक cgroup_mode;
  माप_प्रकार i;
  पूर्णांक ret;

  ret = load_wrapper (&wrapper, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (geteuid ())
    वापस crun_make_error (err, 0, "checkpointing requires root");

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
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_version (err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (UNLIKELY (cr_options->image_path == शून्य))
    वापस crun_make_error (err, 0, "image path not set");

  ret = सूची_गढ़ो (cr_options->image_path, 0700);
  अगर (UNLIKELY ((ret == -1) && (त्रुटि_सं != EEXIST)))
    वापस crun_make_error (err, त्रुटि_सं, "error creating checkpoint directory `%s`", cr_options->image_path);

  image_fd = खोलो (cr_options->image_path, O_DIRECTORY | O_CLOEXEC);
  अगर (UNLIKELY (image_fd == -1))
    वापस crun_make_error (err, त्रुटि_सं, "error opening checkpoint directory `%s`", cr_options->image_path);

  libcriu_wrapper->criu_set_images_dir_fd (image_fd);

  /* Set up logging. */
  libcriu_wrapper->criu_set_log_level (4);
  libcriu_wrapper->criu_set_log_file (CRIU_CHECKPOINT_LOG_FILE);

  /* Set up CRIU config file */
  अगर (UNLIKELY (handle_criu_config_file (container, err)))
    वापस -1;

  /* Setting the pid early as we can skip a lot of checkpoint setup if
   * we just do a pre-dump. The PID needs to be set always. Do it here.
   * The main process of the container is the process CRIU will checkpoint
   * and all of its children. */
  libcriu_wrapper->criu_set_pid (status->pid);

  ret = setup_criu_work_dir (cr_options, &work_fd, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

#  ifdef CRIU_PRE_DUMP_SUPPORT

  {
    पूर्णांक criu_can_mem_track = 0;
    /* If the user uses --pre-dump for the second time or does
     * a final dump from a previous pre-dump, setting parent_path
     * is necessary so that CRIU can find which pages have not
     * changed compared to the previous dump. */
    अगर (cr_options->parent_path != शून्य)
      {
        criu_can_mem_track = criu_check_mem_track (err);
        अगर (UNLIKELY (criu_can_mem_track == -1))
          वापस -1;
        libcriu_wrapper->criu_set_track_mem (true);

        /* The parent path must be relative to image path (something like ../previous-dump).
           CRIU will fail with an unclear error message if the path is not right.
         */
        अगर (UNLIKELY (cr_options->parent_path[0] == '/'))
          वापस crun_make_error (err, 0, "--parent-path must be relative");
        पूर्णांक is_dir = crun_dir_p_at (image_fd, cr_options->parent_path, false, err);
        अगर (UNLIKELY (is_dir <= 0))
          {
            अगर (is_dir < 0)
              वापस crun_error_wrap (err, "invalid --parent-path");
            वापस crun_make_error (err, ENOTDIR, "invalid --parent-path");
          }
        ret = libcriu_wrapper->criu_set_parent_images (cr_options->parent_path);
        अगर (UNLIKELY (ret != 0))
          वापस crun_make_error (err, -ret, "error setting CRIU parent images path to `%s`", cr_options->parent_path);
      }

    अगर (cr_options->pre_dump)
      {
        अगर (criu_can_mem_track != 1)
          {
            criu_can_mem_track = criu_check_mem_track (err);
            अगर (UNLIKELY (criu_can_mem_track == -1))
              वापस -1;
          }
        libcriu_wrapper->criu_set_track_mem (true);
        ret = libcriu_wrapper->criu_pre_dump ();
        अगर (UNLIKELY (ret != 0))
          {
            show_criu_log (cr_options->work_path, CRIU_CHECKPOINT_LOG_FILE);
            वापस crun_make_error (err, 0, "CRIU pre-dump failed: %d", ret);
          }
        वापस 0;
      }
  }
#  endif

  /* descriptors.json is needed during restore to correctly
   * reconnect stdin, stdout, stderr. */
  ret = append_paths (&descriptors_path, err, cr_options->image_path, DESCRIPTORS_FILENAME, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = write_file (descriptors_path, status->external_descriptors, safe_strlen (status->external_descriptors), err);
  अगर (UNLIKELY (ret < 0))
    वापस crun_error_wrap (err, "error saving CRIU descriptors file");

  ret = append_paths (&path, err, status->bundle, status->rootfs, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = libcriu_wrapper->criu_set_मूल (path);
  अगर (UNLIKELY (ret != 0))
    वापस crun_make_error (err, 0, "error setting CRIU root to `%s`", path);

  cgroup_mode = libcrun_get_cgroup_mode (err);
  अगर (UNLIKELY (cgroup_mode < 0))
    वापस cgroup_mode;

  /* For cgroup v1 we need to tell CRIU to handle all cgroup mounts as external mounts. */
  अगर (cgroup_mode != CGROUP_MODE_UNIFIED)
    {
      ret = checkpoint_cgroup_v1_mount (def, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  /* Tell CRIU about external bind mounts. */
  क्रम (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      अगर (is_bind_mount (def->mounts[i], शून्य, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          अक्षर buf[PATH_MAX];
          स्थिर अक्षर *dest_in_मूल;

          अगर (nofollow)
            वापस crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_in_मूल = chroot_realpath (status->rootfs, def->mounts[i]->destination, buf);
          अगर (UNLIKELY (dest_in_मूल == शून्य))
            वापस crun_make_error (err, त्रुटि_सं, "unable to resolve external bind mount `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          अगर (has_prefix (dest_in_मूल, status->rootfs))
            dest_in_मूल += safe_strlen (status->rootfs);

          ret = libcriu_wrapper->criu_add_ext_mount (dest_in_मूल, dest_in_मूल);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", def->mounts[i]->destination);
        }
    }

  ret = register_masked_paths_mounts (def, container, libcriu_wrapper, false, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

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

  क्रम (i = 0; i < def->linux->namespaces_len; i++)
    {
      पूर्णांक मूल्य = libcrun_find_namespace (def->linux->namespaces[i]->type);
      अगर (UNLIKELY (मूल्य < 0))
        वापस crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      अगर (मूल्य == CLONE_NEWNET && def->linux->namespaces[i]->path != शून्य)
        {
          cleanup_free अक्षर *external = शून्य;
          काष्ठा stat statbuf;

          ret = stat (def->linux->namespaces[i]->path, &statbuf);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "unable to stat(): `%s`", def->linux->namespaces[i]->path);

          xasprintf (&external, "net[%ld]:" CRIU_EXT_NETNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_add_external (external);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }

      अगर (मूल्य == CLONE_NEWPID && def->linux->namespaces[i]->path != शून्य)
        {
          cleanup_free अक्षर *external = शून्य;
          काष्ठा stat statbuf;

          ret = stat (def->linux->namespaces[i]->path, &statbuf);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, त्रुटि_सं, "unable to stat(): `%s`", def->linux->namespaces[i]->path);

          xasprintf (&external, "pid[%ld]:" CRIU_EXT_PIDNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_add_external (external);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }
    }

  /* Tell CRIU to use the freezer to pause all container processes. */
  अगर (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      /* This needs CRIU 3.14. */
      ret = append_paths (&freezer_path, err, CGROUP_मूल, status->cgroup_path, शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }
  अन्यथा
    {
      ret = append_paths (&freezer_path, err, CGROUP_मूल "/freezer", status->cgroup_path, शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  ret = libcriu_wrapper->criu_set_freeze_cgroup (freezer_path);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, -ret, "CRIU: failed setting freezer %d", ret);

  /* Set boolean options . */
  libcriu_wrapper->criu_set_leave_running (cr_options->leave_running);
  libcriu_wrapper->criu_set_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_set_shell_job (cr_options->shell_job);
  libcriu_wrapper->criu_set_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_set_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_set_orphan_pts_master (true);
  libcriu_wrapper->criu_set_manage_cgroups_mode (criu_cg_mode_from_public (cr_options->manage_cgroups_mode));

  libcriu_wrapper->criu_set_manage_cgroups (true);

  अगर (libcriu_wrapper->criu_set_network_lock)
    {
      पूर्णांक lock = 0;

      ret = criu_network_lock_from_public (cr_options->network_lock_method, &lock, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      अगर (lock > 0)
        {
          ret = libcriu_wrapper->criu_set_network_lock (lock);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, 0, "CRIU: failed setting network lock");
        }
    }

  ret = libcriu_wrapper->criu_dump ();
  अगर (UNLIKELY (ret != 0))
    {
      show_criu_log (cr_options->work_path, CRIU_CHECKPOINT_LOG_FILE);
      वापस crun_make_error (err, ret < 0 ? -ret : 0, "CRIU checkpointing failed: %d", ret);
    }

  वापस 0;
}

अटल पूर्णांक
prepare_restore_mounts_internal (runtime_spec_schema_config_schema *def, अक्षर *मूल, स्थिर अक्षर **mounted,
                                 माप_प्रकार *n_mounted, libcrun_error_t *err)
{
  uint32_t i;

  /* Go through all mountpoints to be able to recreate missing mountpoints. */
  क्रम (i = 0; i < def->mounts_len; i++)
    {
      अक्षर *dest = def->mounts[i]->destination;
      अक्षर *type = def->mounts[i]->type;
      cleanup_close पूर्णांक मूल_fd = -1;
      bool nofollow = false;
      bool on_tmpfs = false;
      पूर्णांक is_dir = 1;
      माप_प्रकार j;

      /* cgroup restore should be handled by CRIU itself */
      अगर (type && (म_भेद (type, "cgroup") == 0 || म_भेद (type, "cgroup2") == 0))
        जारी;

      /* Check if the mountpoint is on a tmpfs. CRIU restores
       * all tmpfs. We do need to recreate directories on a tmpfs. */
      माप_प्रकार dest_len = म_माप (dest);
      क्रम (j = 0; j < def->mounts_len; j++)
        {
          अगर (def->mounts[j]->type == शून्य || म_भेद (def->mounts[j]->type, "tmpfs") != 0)
            जारी;
          माप_प्रकार mount_len = म_माप (def->mounts[j]->destination);
          अगर (mount_len < dest_len && dest[mount_len] == '/' && म_भेदन (dest, def->mounts[j]->destination, mount_len) == 0)
            {
              /* This is a mountpoint which is on a tmpfs.*/
              on_tmpfs = true;
              अवरोध;
            }
        }

      अगर (on_tmpfs)
        जारी;

      /* For bind mounts check if the source is a file or a directory. */
      अगर (is_bind_mount (def->mounts[i], शून्य, &nofollow))
        {
          अगर (nofollow)
            वापस crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          is_dir = crun_dir_p (def->mounts[i]->source, false, err);
          अगर (UNLIKELY (is_dir < 0))
            वापस is_dir;
        }

      मूल_fd = खोलो (मूल, O_RDONLY | O_CLOEXEC);
      अगर (UNLIKELY (मूल_fd == -1))
        वापस crun_make_error (err, त्रुटि_सं, "error opening container root directory `%s`", मूल);

      अगर (is_dir)
        {
          पूर्णांक ret;

          ret = crun_safe_ensure_directory_at (मूल_fd, मूल, dest, 0755, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }
      अन्यथा
        {
          पूर्णांक ret;

          ret = crun_safe_ensure_file_at (मूल_fd, मूल, dest, 0755, err);
          अगर (UNLIKELY (ret < 0))
            वापस ret;
        }

      /* Mount a bind mount source now, so that the mountpoints of the
       * mounts inside it (e.g. a nested bind mount) are created in the
       * source, where CRIU expects them.  This also happens during the
       * initial container creation, as the mounts are done in order.  */
      अगर (is_bind_mount (def->mounts[i], शून्य, &nofollow))
        {
          cleanup_close पूर्णांक dst_fd = -1;
          proc_fd_path_t fd_path;

          dst_fd = safe_openat (मूल_fd, मूल, dest, O_PATH | O_CLOEXEC, 0, err);
          अगर (UNLIKELY (dst_fd < 0))
            वापस dst_fd;

          /* Not recursive: the source might contain ROOT itself (e.g. when
           * it is the bundle directory), and it is not needed anyway.  */
          get_proc_self_fd_path (fd_path, dst_fd);
          अगर (UNLIKELY (mount (def->mounts[i]->source, fd_path, शून्य, MS_BIND, शून्य) < 0))
            वापस crun_make_error (err, त्रुटि_सं, "bind mount `%s` to `%s`", def->mounts[i]->source, dest);

          mounted[(*n_mounted)++] = dest;
        }
    }

  वापस 0;
}

/* Move the current process back to CGROUPS, as read from /proc/self/cgroup.
   A failure is not fatal, so only warn about it.  */
अटल व्योम
move_back_to_cgroups (स्थिर अक्षर *cgroups)
{
  libcrun_error_t tmp_err = शून्य;

  अगर (UNLIKELY (libcrun_move_self_to_cgroups (cgroups, &tmp_err) < 0))
    {
      libcrun_warning ("cannot move back to the original cgroup: %s", tmp_err->संदेश);
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
अटल पूर्णांक
criu_restore_child_in_cgroup (स्थिर अक्षर *cgroup_path, पूर्णांक *criu_ret, libcrun_error_t *err)
{
  sigset_t all_signals, old_mask;
  पूर्णांक wait_status = 0;
  pid_t pid;
  पूर्णांक ret;

  *criu_ret = -1;

  sigfillset (&all_signals);
  ret = sigprocmask (SIG_BLOCK, &all_signals, &old_mask);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "sigprocmask");

  /* Must be vfork: the child shares our memory space, so both *criu_ret and
     the error it creates are visible here once it is gone.  */
  pid = vfork ();
  अगर (UNLIKELY (pid < 0))
    {
      पूर्णांक saved_errno = त्रुटि_सं;

      sigprocmask (SIG_SETMASK, &old_mask, शून्य);
      वापस crun_make_error (err, saved_errno, "vfork");
    }

  अगर (pid == 0)
    {
      काष्ठा sigaction act;
      पूर्णांक i;

      /* The signal dispositions are not shared with the parent, so resetting
         them here does not affect it.  Keep the ignored signals ignored, as
         CRIU would inherit them if it was run directly by the caller.  */
      क्रम (i = 1; i < NSIG; i++)
        {
          अगर (sigaction (i, शून्य, &act) < 0 || act.sa_handler == संक_छोड़ो || act.sa_handler == संक_पूर्व)
            जारी;

          स_रखो (&act, 0, माप (act));
          act.sa_handler = संक_पूर्व;
          sigaction (i, &act, शून्य);
        }
      sigprocmask (SIG_SETMASK, &old_mask, शून्य);

      अगर (! is_empty_string (cgroup_path))
        {
          ret = libcrun_move_process_to_cgroup (0, 0, cgroup_path, false, err);
          अगर (UNLIKELY (ret < 0))
            _safe_exit (निकास_त्रुटि);
        }

      *criu_ret = libcriu_wrapper->criu_restore_child ();
      _safe_exit (निकास_सफल);
    }

  /* The child is gone by now, it is safe to handle signals again.  */
  sigprocmask (SIG_SETMASK, &old_mask, शून्य);

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  अगर (UNLIKELY (ret < 0))
    {
      /* The caller might have reaped the child already, e.g. when it ignores
         SIGCHLD.  Its result is still visible, so use it.  */
      अगर (त्रुटि_सं != ECHILD)
        वापस crun_make_error (err, त्रुटि_सं, "waitpid for the CRIU restore process");

      /* The child might have failed before calling CRIU, and left an error.  */
      वापस *err != शून्य ? -1 : 0;
    }

  ret = get_process_exit_status (wait_status);
  अगर (UNLIKELY (ret != निकास_सफल))
    {
      /* The child creates the error in the shared memory space, do not
         overwrite it with crun_make_error().  */
      अगर (*err == शून्य)
        वापस crun_make_error (err, 0, "the CRIU restore process exited with status %d", ret);
      वापस -1;
    }

  वापस 0;
}

/* Recreate the mountpoints which do not exist in the container rootfs
 * mounted at ROOT, the same way it is done on the container creation.  */
अटल पूर्णांक
prepare_restore_mounts (runtime_spec_schema_config_schema *def, अक्षर *मूल, libcrun_error_t *err)
{
  cleanup_free स्थिर अक्षर **mounted = xmalloc0 (माप (अक्षर *) * (def->mounts_len + 1));
  cleanup_close पूर्णांक मूल_fd = -1;
  माप_प्रकार n_mounted = 0;
  पूर्णांक ret;

  ret = prepare_restore_mounts_internal (def, मूल, mounted, &n_mounted, err);

  /* The bind mounts done above are only needed to create the mountpoints,
   * and CRIU restores the mounts itself, so undo them, in reverse order. */
  अगर (n_mounted > 0)
    मूल_fd = खोलो (मूल, O_PATH | O_CLOEXEC);
  जबतक (मूल_fd >= 0 && n_mounted > 0)
    {
      libcrun_error_t tmp_err = शून्य;
      स्थिर अक्षर *dest = mounted[--n_mounted];
      cleanup_close पूर्णांक dst_fd = -1;
      proc_fd_path_t fd_path;

      dst_fd = safe_openat (मूल_fd, मूल, dest, O_PATH | O_CLOEXEC, 0, &tmp_err);
      अगर (UNLIKELY (dst_fd < 0))
        {
          crun_error_release (&tmp_err);
          जारी;
        }
      get_proc_self_fd_path (fd_path, dst_fd);
      अगर (UNLIKELY (umount2 (fd_path, MNT_DETACH) < 0 && ret >= 0))
        ret = crun_make_error (err, त्रुटि_सं, "unmount `%s`", dest);
    }

  वापस ret;
}

पूर्णांक
libcrun_container_restore_linux_criu (libcrun_container_status_t *status, libcrun_container_t *container,
                                      libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper काष्ठा libcriu_wrapper_s *wrapper = शून्य;
  cleanup_close पूर्णांक inherit_new_net_fd = -1;
  cleanup_close पूर्णांक inherit_new_pid_fd = -1;
  cleanup_close पूर्णांक image_fd = -1;
  cleanup_free अक्षर *मूल = शून्य;
  cleanup_free अक्षर *bundle_cleanup = शून्य;
  cleanup_free अक्षर *own_cgroups = शून्य;
  cleanup_close पूर्णांक work_fd = -1;
  पूर्णांक cgroup_mode;
  पूर्णांक criu_ret;
  पूर्णांक ret_out;
  माप_प्रकार i;
  पूर्णांक ret;

  ret = load_wrapper (&wrapper, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (geteuid ())
    वापस crun_make_error (err, 0, "restoring requires root");

  ret = libcriu_wrapper->criu_init_opts ();
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_version (err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (UNLIKELY (cr_options->image_path == शून्य))
    वापस crun_make_error (err, 0, "image path not set");

  image_fd = खोलो (cr_options->image_path, O_DIRECTORY | O_CLOEXEC);
  अगर (UNLIKELY (image_fd == -1))
    वापस crun_make_error (err, त्रुटि_सं, "error opening checkpoint directory `%s`", cr_options->image_path);

  libcriu_wrapper->criu_set_images_dir_fd (image_fd);

  /* Load descriptors.json to tell CRIU where those FDs should be connected to. */
  {
    cleanup_free अक्षर *descriptors_path = शून्य;
    cleanup_free अक्षर *बफर = शून्य;
    json_object *doc = शून्य;

    ret = append_paths (&descriptors_path, err, cr_options->image_path, DESCRIPTORS_FILENAME, शून्य);
    अगर (UNLIKELY (ret < 0))
      वापस ret;

    ret = read_all_file (descriptors_path, &बफर, शून्य, err);
    अगर (UNLIKELY (ret < 0))
      वापस ret;

    /* descriptors.json contains a JSON array with strings
     * telling where 0, 1 and 2 have been initially been
     * pointing to. For each descriptor which points to
     * a pipe 'pipe:' we tell CRIU to reconnect that pipe
     * to the corresponding FD to have (especially) stdout
     * and stderr being correctly redirected. */
    ret = parse_json_file (&doc, बफर, शून्य, err);
    अगर (UNLIKELY (ret < 0))
      वापस ret;

    अगर (json_object_is_type (doc, json_type_array))
      {
        माप_प्रकार i, len = json_object_array_लंबाई (doc);

        /* len will probably always be 3 as crun is currently only
         * recording the destination of FD 0, 1 and 2. */
        क्रम (i = 0; i < len; ++i)
          {
            json_object *s = json_object_array_get_idx (doc, i);
            अगर (s && json_object_is_type (s, json_type_string))
              {
                स्थिर अक्षर *str = json_object_get_string (s);
                अगर (has_prefix (str, "pipe:"))
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
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = setup_criu_work_dir (cr_options, &work_fd, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (cr_options->lsm_profile != शून्य)
    {
      ret = libcriu_wrapper->criu_set_lsm_profile (cr_options->lsm_profile);
      अगर (UNLIKELY (ret != 0))
        वापस crun_make_error (err, -ret, "error setting LSM profile to `%s`", cr_options->lsm_profile);
    }

  अगर (cr_options->lsm_mount_context != शून्य)
    {
      ret = libcriu_wrapper->criu_set_lsm_mount_context (cr_options->lsm_mount_context);
      अगर (UNLIKELY (ret != 0))
        वापस crun_make_error (err, -ret, "error setting LSM mount context to `%s`", cr_options->lsm_mount_context);
    }

  /* do realpath on root */
  bundle_cleanup = realpath (status->bundle, शून्य);
  अगर (UNLIKELY (bundle_cleanup == शून्य))
    bundle_cleanup = xstrdup (status->bundle);

  /* Tell CRIU about external bind mounts. */
  क्रम (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      अगर (is_bind_mount (def->mounts[i], शून्य, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          अक्षर buf[PATH_MAX];
          स्थिर अक्षर *dest_in_मूल;
          स्थिर अक्षर *source = def->mounts[i]->source;
          cleanup_free अक्षर *abs_source = शून्य;
          अचिन्हित दीर्घ flags = 0;
          माप_प्रकार j;

          अगर (nofollow)
            वापस crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_in_मूल = chroot_realpath (status->rootfs, def->mounts[i]->destination, buf);
          अगर (UNLIKELY (dest_in_मूल == शून्य))
            वापस crun_make_error (err, त्रुटि_सं, "unable to resolve external bind mount destination `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          अगर (has_prefix (dest_in_मूल, status->rootfs))
            dest_in_मूल += safe_strlen (status->rootfs);

          /* A relative source is relative to the bundle, while CRIU would
             resolve it relative to its own working directory.  */
          अगर (source && source[0] != '/')
            {
              ret = append_paths (&abs_source, err, bundle_cleanup, source, शून्य);
              अगर (UNLIKELY (ret < 0))
                वापस ret;
              source = abs_source;
            }

          ret = libcriu_wrapper->criu_add_ext_mount (dest_in_मूल, source);
          अगर (UNLIKELY (ret < 0))
            वापस crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", source);

          क्रम (j = 0; j < def->mounts[i]->options_len; j++)
            {
              स्थिर काष्ठा propagation_flags_s *prop = शून्य;

              अगर (def->mounts[i]->options[j])
                prop = libcrun_str2mount_flags (def->mounts[i]->options[j]);
              अगर (prop && ! prop->clear)
                flags |= prop->flags;
            }
          add_restored_ext_mount (dest_in_मूल, flags);
        }
    }

  ret = register_masked_paths_mounts (def, container, libcriu_wrapper, true, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  /* Mount the container rootfs for CRIU. */
  ret = append_paths (&मूल, err, bundle_cleanup, "criu-root", शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = सूची_गढ़ो (मूल, 0755);
  अगर (UNLIKELY (ret == -1))
    वापस crun_make_error (err, त्रुटि_सं, "error creating restore directory `%s`", मूल);

  ret = mount (status->rootfs, मूल, शून्य, MS_BIND | MS_REC, शून्य);
  अगर (UNLIKELY (ret == -1))
    {
      ret = crun_make_error (err, त्रुटि_सं, "error mounting restore directory `%s`", मूल);
      जाओ out;
    }

  /* During initial container creation, crun will create mountpoints
   * defined in config.json if they do not exist. If we are restoring
   * we need to make sure these mountpoints also exist.
   * This is not perfect, as this means crun will modify a rootfs
   * even if it marked as read-only, but runc already modifies
   * the rootfs in the same way. */

  ret = prepare_restore_mounts (def, मूल, err);
  अगर (UNLIKELY (ret < 0))
    जाओ out_umount;

  ret = libcriu_wrapper->criu_set_मूल (मूल);
  अगर (UNLIKELY (ret != 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU root to `%s`", मूल);
      जाओ out_umount;
    }

  /* If a namespace defined in config.json we are telling
   * CRIU use that namespace when restoring the process tree.
   *
   * CRIU expects the information about the namespace like this:
   * --inherit-fd fd[<fd>]:<key>
   * The <key> needs to be the same as during checkpointing (extRootNetNS). */
  क्रम (i = 0; i < def->linux->namespaces_len; i++)
    {
      स्थिर पूर्णांक open_flags_for_inherit = O_RDONLY; /* Cannot be O_CLOEXEC as it is passed to the child process. */
      पूर्णांक मूल्य = libcrun_find_namespace (def->linux->namespaces[i]->type);
      अगर (UNLIKELY (मूल्य < 0))
        {
          ret = crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);
          जाओ out_umount;
        }

      अगर (मूल्य == CLONE_NEWNET && def->linux->namespaces[i]->path != शून्य)
        {
          inherit_new_net_fd = खोलो (def->linux->namespaces[i]->path, open_flags_for_inherit);
          अगर (UNLIKELY (inherit_new_net_fd < 0))
            {
              ret = crun_make_error (err, त्रुटि_सं, "unable to open(): `%s`", def->linux->namespaces[i]->path);
              जाओ out_umount;
            }

          ret = libcriu_wrapper->criu_add_inherit_fd (inherit_new_net_fd, CRIU_EXT_NETNS);
          अगर (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              जाओ out_umount;
            }
        }

      अगर (मूल्य == CLONE_NEWPID && def->linux->namespaces[i]->path != शून्य)
        {
          inherit_new_pid_fd = खोलो (def->linux->namespaces[i]->path, open_flags_for_inherit);
          अगर (UNLIKELY (inherit_new_pid_fd < 0))
            {
              ret = crun_make_error (err, त्रुटि_सं, "unable to open(): `%s`", def->linux->namespaces[i]->path);
              जाओ out_umount;
            }

          ret = libcriu_wrapper->criu_add_inherit_fd (inherit_new_pid_fd, CRIU_EXT_PIDNS);
          अगर (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              जाओ out_umount;
            }
        }

#  ifdef CRIU_JOIN_NS_SUPPORT
      अगर (मूल्य == CLONE_NEWTIME && def->linux->namespaces[i]->path != शून्य)
        {
          अगर (libcriu_wrapper->criu_join_ns_add == शून्य)
            {
              ret = crun_make_error (err, 0, "shared time namespace restore is supported in CRIU >= 3.16.1");
              जाओ out_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_add ("time", def->linux->namespaces[i]->path, शून्य);
          अगर (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->path);
              जाओ out_umount;
            }
        }

      अगर (मूल्य == CLONE_NEWIPC && def->linux->namespaces[i]->path != शून्य)
        {
          अगर (libcriu_wrapper->criu_join_ns_add == शून्य)
            {
              ret = crun_make_error (err, 0, "shared ipc namespace restore is supported in CRIU >= 3.16.1");
              जाओ out_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_add ("ipc", def->linux->namespaces[i]->path, शून्य);
          अगर (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->path);
              जाओ out_umount;
            }
        }

      अगर (मूल्य == CLONE_NEWUTS && def->linux->namespaces[i]->path != शून्य)
        {
          अगर (libcriu_wrapper->criu_join_ns_add == शून्य)
            {
              ret = crun_make_error (err, 0, "shared uts namespace restore is supported in CRIU >= 3.16.1");
              जाओ out_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_add ("uts", def->linux->namespaces[i]->path, शून्य);
          अगर (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->path);
              जाओ out_umount;
            }
        }
#  endif
    }

  /* Set up CRIU config file */
  ret = handle_criu_config_file (container, err);
  अगर (UNLIKELY (ret < 0))
    जाओ out_umount;

  /* Tell CRIU if cgroup v1 needs to be handled. */
  ret = restore_cgroup_v1_mount (def, err);
  अगर (UNLIKELY (ret < 0))
    जाओ out_umount;

  console_socket = cr_options->console_socket;
  libcriu_wrapper->criu_set_notify_cb (criu_notify);

  /* Set boolean options . */
  libcriu_wrapper->criu_set_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_set_shell_job (cr_options->shell_job);
  libcriu_wrapper->criu_set_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_set_tcp_close (cr_options->tcp_close);
  libcriu_wrapper->criu_set_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_set_orphan_pts_master (true);

  अगर (status->cgroup_path)
    {
      cgroup_mode = libcrun_get_cgroup_mode (err);
      अगर (UNLIKELY (cgroup_mode < 0))
        {
          ret = cgroup_mode;
          जाओ out_umount;
        }

      /* With cgroup v2, the cgroup path is only meaningful for the unified
         hierarchy, so only relocate that one.  A NULL controller would make
         CRIU relocate every hierarchy the container is in, named cgroup v1
         ones included.  As CRIU dumps such a hierarchy from wherever the
         container is in it, which is its root, as crun leaves named
         hierarchies alone, every checkpoint and restore would then copy the
         whole hierarchy into itself.  */
      ret = libcriu_wrapper->criu_add_cg_मूल (cgroup_mode == CGROUP_MODE_UNIFIED ? "" : शून्य,
                                               status->cgroup_path);
      अगर (UNLIKELY (ret != 0))
        {
          ret = crun_make_error (err, 0, "error setting CRIU cgroup root to `%s`", status->cgroup_path);
          जाओ out_umount;
        }
    }

  libcriu_wrapper->criu_set_manage_cgroups_mode (criu_cg_mode_from_public (cr_options->manage_cgroups_mode));
  libcriu_wrapper->criu_set_manage_cgroups (true);

  अगर (libcriu_wrapper->criu_set_network_lock)
    {
      पूर्णांक lock = 0;

      ret = criu_network_lock_from_public (cr_options->network_lock_method, &lock, err);
      अगर (UNLIKELY (ret < 0))
        जाओ out_umount;

      अगर (lock > 0)
        {
          ret = libcriu_wrapper->criu_set_network_lock (lock);
          अगर (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "CRIU: failed setting network lock");
              जाओ out_umount;
            }
        }
    }

  libcriu_wrapper->criu_set_log_level (4);
  ret = libcriu_wrapper->criu_set_log_file (CRIU_RESTORE_LOG_FILE);
  अगर (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU log file to `%s`", CRIU_RESTORE_LOG_FILE);
      जाओ out_umount;
    }

  /* criu_restore() returns the PID of the process of the restored process
   * tree. This PID will not be the same as status->pid if the container is
   * running in a PID namespace. But it will always be > 0. */
  अगर (cr_options->detach)
    {
      /* Nothing waits for the container, so there is no reason to take over
         this process: use a throw-away one.  It is used even when there is no
         cgroup to join, so that the container is not left as a child of a
         process which is not going to reap it.  */
      ret = criu_restore_child_in_cgroup (status->cgroup_path, &criu_ret, err);
      अगर (UNLIKELY (ret < 0))
        जाओ out_umount;

      ret = criu_ret;
    }
  अन्यथा
    {
      /* Like runc does, put CRIU into the container cgroup for the time of
       * restore, so the restored tasks are created there. It is necessary in
       * the "ignore" mode, where CRIU does not deal with cgroups at all, and
       * does not hurt otherwise. As CRIU is our child, do it by moving
       * ourselves there, and move back once the restore is done. */
      अगर (! is_empty_string (status->cgroup_path))
        {
          ret = read_all_file (PROC_SELF_CGROUP, &own_cgroups, शून्य, err);
          अगर (UNLIKELY (ret < 0))
            जाओ out_umount;

          ret = libcrun_move_process_to_cgroup (0, 0, status->cgroup_path, false, err);
          अगर (UNLIKELY (ret < 0))
            {
              /* Some of the cgroups might have been joined already.  */
              move_back_to_cgroups (own_cgroups);
              जाओ out_umount;
            }
        }

      ret = libcriu_wrapper->criu_restore_child ();

      अगर (own_cgroups)
        move_back_to_cgroups (own_cgroups);
    }

  अगर (UNLIKELY (ret <= 0))
    {
      show_criu_log (cr_options->work_path, CRIU_RESTORE_LOG_FILE);
      ret = crun_make_error (err, 0, "CRIU restoring failed: %d", ret);
      जाओ out_umount;
    }

  /* Update the status struct with the newly allocated PID. This will
   * be necessary later when moving the process into its cgroup. */
  status->pid = ret;
  ret = 0;

out_umount:
  ret_out = umount (मूल);
  अगर (UNLIKELY (ret_out == -1))
    {
      पूर्णांक saved_errno = त्रुटि_सं;
      सूची_हटाओ (मूल);
      अगर (ret < 0)
        वापस crun_error_wrap (err, "error unmounting restore directory `%s`", मूल);
      अन्यथा
        वापस crun_make_error (err, saved_errno, "error unmounting restore directory `%s`", मूल);
    }
out:
  ret_out = सूची_हटाओ (मूल);
  अगर (UNLIKELY (ret < 0))
    वापस ret;
  अगर (UNLIKELY (ret_out == -1))
    वापस crun_make_error (err, त्रुटि_सं, "error removing restore directory `%s`", मूल);
  वापस ret;
}
#पूर्ण_अगर
