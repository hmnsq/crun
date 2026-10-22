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
#تعریف_کن GNU_منبع

#ضمن <config.h>

#إذا HAVE_CRIU && HAVE_DLOPEN

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
حرف *chroot_realpath (ثابت حرف *chroot, ثابت حرف *مسیر, حرف resolved_مسیر[]);

/* Map the public cgroups mode (enum libcrun_cr_cgroups_mode) onto the CRIU enum.  */
ساكن تعداد criu_cg_نما
criu_cg_نما_from_عمومی (صحيح نما)
{
  بدل (نما)
    {
    حالة LIBCRUN_CR_CG_نما_IGNORE:
      أرجع CRIU_CG_نما_IGNORE;
    حالة LIBCRUN_CR_CG_نما_پر:
      أرجع CRIU_CG_نما_پر;
    حالة LIBCRUN_CR_CG_نما_STRICT:
      أرجع CRIU_CG_نما_STRICT;
    حالة LIBCRUN_CR_CG_نما_DEFAULT:
    حالة LIBCRUN_CR_CG_نما_SOFT:
    افتراضي:
      /* Default to CRIU_CG_MODE_SOFT, just as runc.  */
      أرجع CRIU_CG_نما_SOFT;
    }
}

/* Map the public network lock method (enum libcrun_cr_network_lock_method) onto the
   CRIU enum.  *LOCK is set to 0 when no explicit method must be set (CRIU default).
   A method that this build cannot honor is an error, never a silent fallback to
   the CRIU default.  */
ساكن صحيح
criu_شبکه_قفل_from_عمومی (صحيح روش, صحيح *قفل, libcrun_error_t *err)
{
  بدل (روش)
    {
    حالة LIBCRUN_CR_شبکه_قفل_IPTABLES:
      *قفل = CRIU_شبکه_قفل_IPTABLES;
      أرجع 0;
    حالة LIBCRUN_CR_شبکه_قفل_NFTABLES:
      *قفل = CRIU_شبکه_قفل_NFTABLES;
      أرجع 0;
    حالة LIBCRUN_CR_شبکه_قفل_SKIP:
#  إذا CRIU_شبکه_قفل_SKIP_SUPPORT
      *قفل = CRIU_شبکه_قفل_SKIP;
      أرجع 0;
#  وإلا
      أرجع crun_make_error (err, 0, "CRIU: the `skip` network lock method is not supported by this build");
#  endif
    حالة LIBCRUN_CR_شبکه_قفل_DEFAULT:
      *قفل = 0;
      أرجع 0;
    افتراضي:
      أرجع crun_make_error (err, 0, "CRIU: unknown network lock method %d", روش);
    }
}

ساكن ثابت حرف *console_socket = عدم;

/* The external mounts CRIU restores, and the flags the configuration asks
   for each of them.  */
ساكن بنية libcrun_mount_flags_s *restored_ext_mounts = عدم;
ساكن size_t n_restored_ext_mounts = 0;

#  define LIBCRIU_MIN_نسخه 31500

بنية libcriu_wrapper_s
{
  فراغ *handle;
  صحيح (*criu_جمع_کن_ext_mount) (ثابت حرف *کلید, ثابت حرف *val);
  صحيح (*criu_جمع_کن_external) (ثابت حرف *کلید);
  صحيح (*criu_جمع_کن_inherit_fd) (صحيح fd, ثابت حرف *کلید);
  صحيح (*criu_check_نسخه) (صحيح minimum);
  صحيح (*criu_dump) (فراغ);
  صحيح (*criu_get_orphan_pts_master_fd) (فراغ);
  صحيح (*criu_init_opts) (فراغ);
  صحيح (*criu_notify_pid) (criu_notify_arg_t na);
#  ifdef CRIU_JOIN_NS_SUPPORT
  صحيح (*criu_join_ns_جمع_کن) (ثابت حرف *ns, ثابت حرف *ns_file, ثابت حرف *extra_opt);
#  endif
#  ifdef CRIU_PRE_DUMP_SUPPORT
  صحيح (*criu_feature_check) (بنية criu_feature_check *features, size_t اندازه);
  صحيح (*criu_pre_dump) (فراغ);
#  endif
  صحيح (*criu_restore_فرزند) (فراغ);
  صحيح (*criu_مجموعه_freeze_cgroup) (ثابت حرف *name);
  فراغ (*criu_مجموعه_file_locks) (bool file_locks);
  فراغ (*criu_مجموعه_ext_unix_sk) (bool ext_unix_sk);
  صحيح (*criu_مجموعه_log_file) (ثابت حرف *log_file);
  فراغ (*criu_مجموعه_log_level) (صحيح log_level);
  فراغ (*criu_مجموعه_leave_running) (bool leave_running);
  فراغ (*criu_مجموعه_manage_cgroups) (bool manage);
  فراغ (*criu_مجموعه_manage_cgroups_نما) (تعداد criu_cg_نما نما);
  صحيح (*criu_مجموعه_شبکه_قفل) (تعداد criu_شبکه_قفل_روش روش);
  فراغ (*criu_مجموعه_notify_cb) (صحيح (*cb) (حرف *action, criu_notify_arg_t na));
  فراغ (*criu_مجموعه_orphan_pts_master) (bool orphan_pts_master);
  فراغ (*criu_مجموعه_images_dir_fd) (صحيح fd);
  صحيح (*criu_مجموعه_والد_images) (ثابت حرف *مسیر);
  فراغ (*criu_مجموعه_pid) (صحيح pid);
  صحيح (*criu_مجموعه_ریشه) (ثابت حرف *ریشه);
  صحيح (*criu_جمع_کن_cg_ریشه) (ثابت حرف *ctrl, ثابت حرف *مسیر);
  فراغ (*criu_مجموعه_shell_کار) (bool shell_کار);
  فراغ (*criu_مجموعه_tcp_established) (bool tcp_established);
  فراغ (*criu_مجموعه_tcp_close) (bool tcp_close);
  فراغ (*criu_مجموعه_track_mem) (bool track_mem);
  فراغ (*criu_مجموعه_work_dir_fd) (صحيح fd);
  صحيح (*criu_مجموعه_lsm_profile) (ثابت حرف *name);
  صحيح (*criu_مجموعه_lsm_mount_context) (ثابت حرف *name);
  صحيح (*criu_مجموعه_config_file) (ثابت حرف *مسیر);
};

ساكن بنية libcriu_wrapper_s *libcriu_wrapper;

ساكن مضمن فراغ
cleanup_wrapper (فراغ *p)
{
  بنية libcriu_wrapper_s **w;

  w = (بنية libcriu_wrapper_s **) p;
  إذا (*w == عدم)
    أرجع;

#  ifndef STATIC
  إذا ((*w)->handle)
    dlclose ((*w)->handle);
#  endif
  حرر (*w);
  libcriu_wrapper = عدم;

  طالما (n_restored_ext_mounts > 0)
    حرر (restored_ext_mounts[--n_restored_ext_mounts].هدف);
  حرر (restored_ext_mounts);
  restored_ext_mounts = عدم;
}

#  define cleanup_wrapper __attribute__ ((cleanup (cleanup_wrapper)))

ساكن صحيح
بارگیری_wrapper (بنية libcriu_wrapper_s **wrapper_بیرون, libcrun_error_t *err)
{
  cleanup_free بنية libcriu_wrapper_s *wrapper = xmalloc0 (حجم (*wrapper));

#  ifdef STATIC
#    define بارگیری_CRIU_FUNCTION(X, ALLOW_NULL) \
      wrapper->X = &X;
#  وإلا
#    define بارگیری_CRIU_FUNCTION(X, ALLOW_NULL)                                                    \
      افعل                                                                                         \
        {                                                                                        \
          wrapper->X = dlsym (wrapper->handle, #X);                                              \
          إذا (! ALLOW_NULL && wrapper->X == عدم)                                                \
            {                                                                                    \
              dlclose (wrapper->handle);                                                         \
              أرجع crun_make_error (err, 0, "could not find symbol `%s` in `libcriu.so`", #X); \
            }                                                                                    \
      } طالما (0)
#  endif

#  ifndef STATIC
  wrapper->handle = dlopen ("libcriu.so.2", RTLD_NOW);
  إذا (wrapper->handle == عدم)
    أرجع crun_make_error (err, 0, "could not load `libcriu.so.2`: `%s`", dlerror ());
#  endif

  بارگیری_CRIU_FUNCTION (criu_جمع_کن_ext_mount, false);
  بارگیری_CRIU_FUNCTION (criu_جمع_کن_external, false);
  بارگیری_CRIU_FUNCTION (criu_جمع_کن_inherit_fd, false);
  بارگیری_CRIU_FUNCTION (criu_check_نسخه, false);
  بارگیری_CRIU_FUNCTION (criu_dump, false);
  بارگیری_CRIU_FUNCTION (criu_get_orphan_pts_master_fd, false);
  بارگیری_CRIU_FUNCTION (criu_init_opts, false);
  بارگیری_CRIU_FUNCTION (criu_notify_pid, false);

#  ifdef CRIU_JOIN_NS_SUPPORT
  /* criu_join_ns_add() API was introduced with CRIU version 3.16.1
   * Here we check if this API is available at build time to support
   * compiling with older version of CRIU, and at runtime to support
   * running crun with older versions of libcriu.so.2.
   */
  بارگیری_CRIU_FUNCTION (criu_join_ns_جمع_کن, true);
#  endif

#  ifdef CRIU_PRE_DUMP_SUPPORT
  بارگیری_CRIU_FUNCTION (criu_feature_check, false);
  بارگیری_CRIU_FUNCTION (criu_pre_dump, false);
#  endif
  بارگیری_CRIU_FUNCTION (criu_restore_فرزند, false);
  بارگیری_CRIU_FUNCTION (criu_مجموعه_ext_unix_sk, false);
  بارگیری_CRIU_FUNCTION (criu_مجموعه_file_locks, false);
  بارگیری_CRIU_FUNCTION (criu_مجموعه_freeze_cgroup, false);
  بارگیری_CRIU_FUNCTION (criu_مجموعه_images_dir_fd, false);
  بارگیری_CRIU_FUNCTION (criu_مجموعه_leave_running, false);
  بارگیری_CRIU_FUNCTION (criu_مجموعه_log_file, false);
  بارگیری_CRIU_FUNCTION (criu_مجموعه_log_level, false);
  بارگیری_CRIU_FUNCTION (criu_مجموعه_manage_cgroups, false);
  بارگیری_CRIU_FUNCTION (criu_مجموعه_manage_cgroups_نما, false);
  بارگیری_CRIU_FUNCTION (criu_مجموعه_شبکه_قفل, true);
  بارگیری_CRIU_FUNCTION (criu_مجموعه_notify_cb, false);
  بارگیری_CRIU_FUNCTION (criu_مجموعه_orphan_pts_master, false);
  بارگیری_CRIU_FUNCTION (criu_مجموعه_والد_images, false);
  بارگیری_CRIU_FUNCTION (criu_مجموعه_pid, false);
  بارگیری_CRIU_FUNCTION (criu_مجموعه_ریشه, false);
  بارگیری_CRIU_FUNCTION (criu_جمع_کن_cg_ریشه, false);
  بارگیری_CRIU_FUNCTION (criu_مجموعه_shell_کار, false);
  بارگیری_CRIU_FUNCTION (criu_مجموعه_tcp_established, false);
  بارگیری_CRIU_FUNCTION (criu_مجموعه_tcp_close, false);
  بارگیری_CRIU_FUNCTION (criu_مجموعه_track_mem, false);
  بارگیری_CRIU_FUNCTION (criu_مجموعه_work_dir_fd, false);
  بارگیری_CRIU_FUNCTION (criu_مجموعه_lsm_profile, false);
  بارگیری_CRIU_FUNCTION (criu_مجموعه_lsm_mount_context, false);
#  إذا ! defined STATIC || defined CRIU_CONFIG_FILE
  بارگیری_CRIU_FUNCTION (criu_مجموعه_config_file, true);
#  endif

  libcriu_wrapper = *wrapper_بیرون = wrapper;
  wrapper = عدم;
#  undef بارگیری_CRIU_FUNCTION
  أرجع 0;
}

ساكن فراغ
جمع_کن_restored_ext_mount (ثابت حرف *هدف, بیعلامت دراز flags)
{
  restored_ext_mounts = xrealloc (restored_ext_mounts, حجم (*restored_ext_mounts) * (n_restored_ext_mounts + 1));
  restored_ext_mounts[n_restored_ext_mounts].هدف = xstrdup (هدف);
  restored_ext_mounts[n_restored_ext_mounts].flags = flags;
  n_restored_ext_mounts++;
}

ساكن صحيح
criu_notify (حرف *action, criu_notify_arg_t na)
{
  إذا (strcmp (action, "post-restore") == 0)
    {
      /* CRIU takes the source of an external mount from us and its flags
       * from the image, i.e. from whatever mount was at that path in the
       * container when it was checkpointed.  The container can move its
       * mount points around, so before it is resumed make sure that each
       * source is mounted with the restrictions the configuration asks for. */
      libcrun_error_t tmp_err = عدم;
      صحيح ret;

      ret = libcrun_restrict_mounts_درون_container (libcriu_wrapper->criu_notify_pid (na), restored_ext_mounts,
                                                  n_restored_ext_mounts, &tmp_err);
      إذا (UNLIKELY (ret < 0))
        {
          libcrun_error (tmp_err->وضعیت, "cannot set the flags of the restored mounts: %s", tmp_err->پیام);
          libcrun_error_release (&tmp_err);
          أرجع ret;
        }
    }

  إذا (مقایسه_رشته (action, "orphan-pts-master", 17) == 0)
    {
      /* CRIU sends us the master FD via the 'orphan-pts-master'
       * callback and we are passing it on to the '--console-socket'
       * if it exists. */
      cleanup_close صحيح console_socket_fd = -1;
      libcrun_error_t tmp_err = عدم;
      صحيح master_fd;
      صحيح ret;

      إذا (! console_socket)
        أرجع 0;

      master_fd = libcriu_wrapper->criu_get_orphan_pts_master_fd ();

      console_socket_fd = open_unix_domain_client_socket (console_socket, 0, &tmp_err);
      إذا (UNLIKELY (console_socket_fd < 0))
        {
          libcrun_error_release (&tmp_err);
          أرجع console_socket_fd;
        }
      ret = send_fd_مقصد_socket (console_socket_fd, master_fd, &tmp_err);
      إذا (UNLIKELY (ret < 0))
        {
          libcrun_error_release (&tmp_err);
          أرجع ret;
        }
    }
  أرجع 0;
}

#  ifdef CRIU_PRE_DUMP_SUPPORT

ساكن صحيح
criu_check_mem_track (libcrun_error_t *err)
{
  بنية criu_feature_check features = { 0 };
  صحيح ret;

  /* Right now we are only interested in checking memory tracking.
   * Memory tracking can be disabled at different levels. aarch64
   * for example has memory tracking not implemented. It could also
   * be not enabled on other architectures. Just ask CRIU if that
   * features exists. */

  features.mem_track = true;

  ret = libcriu_wrapper->criu_feature_check (&features, حجم (features));
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, 0, "CRIU feature checking failed: %d", ret);

  إذا (features.mem_track == true)
    أرجع 1;

  أرجع crun_make_error (err, 0, "CRIU memory tracking not supported");
}

#  endif

ساكن صحيح
ثبات_masked_paths_mounts (runtime_spec_schema_config_schema *def, libcrun_container_t *container,
                              بنية libcriu_wrapper_s *libcriu_wrapper, bool is_restore, libcrun_error_t *err)
{
  cleanup_free حرف *خالی_dir_مسیر = عدم;
  bool shared_dir_registered = false;
  size_t i;
  صحيح ret;

  لكل (i = 0; i < def->linux->masked_paths_len; i++)
    {
      بنية stat statbuf;
      ret = stat (def->linux->masked_paths[i], &statbuf);
      إذا (ret != 0)
        استمر;

      إذا (S_ISDIR (statbuf.st_mode))
        {
          إذا (! shared_dir_registered)
            {
              ret = get_shared_خالی_directory_مسیر (&خالی_dir_مسیر,
                                                     (container->context ? container->context->state_ریشه : عدم), err);
              إذا (UNLIKELY (ret < 0))
                أرجع ret;

              ret = libcriu_wrapper->criu_جمع_کن_ext_mount (خالی_dir_مسیر, خالی_dir_مسیر);
              إذا (UNLIKELY (ret < 0))
                أرجع crun_make_error (err, -ret, "CRIU: failed adding external mount for shared empty directory `%s`", خالی_dir_مسیر);

              shared_dir_registered = true;
            }

          ret = libcriu_wrapper->criu_جمع_کن_ext_mount (def->linux->masked_paths[i], خالی_dir_مسیر);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, -ret, "CRIU: failed adding external mount for masked directory `%s`", def->linux->masked_paths[i]);

          إذا (is_restore)
            جمع_کن_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
      وإلا إذا (S_ISREG (statbuf.st_mode))
        {
          ثابت حرف *bind_هدف = is_restore ? "/dev/null" : def->linux->masked_paths[i];
          ret = libcriu_wrapper->criu_جمع_کن_ext_mount (def->linux->masked_paths[i], bind_هدف);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", bind_هدف);

          إذا (is_restore)
            جمع_کن_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
    }

  أرجع 0;
}

/* Parse one /proc/self/cgroup LINE into its subsystem, normalizing the name and
   stripping any "name=" prefix.  When SUBPATH_OUT is not NULL, the in-hierarchy
   path is stored there.  Returns the subsystem, or NULL if the line carries no
   controller and should be skipped.  LINE is modified in place.  */
ساكن حرف *
parse_cgroup_subsystem (حرف *سطر, حرف **subpath_بیرون)
{
  حرف *subsystem, *subpath, *it;

  subsystem = strchr (سطر, ':') + 1;
  subpath = strchr (subsystem, ':') + 1;
  *(subpath - 1) = '\0';

  إذا (subsystem[0] == '\0')
    أرجع عدم;

  it = جستجوی_رشته (subsystem, "name=");
  إذا (it)
    subsystem = it + 5;

  إذا (strcmp (subsystem, "net_prio,net_cls") == 0)
    subsystem = "net_cls,net_prio";
  إذا (strcmp (subsystem, "cpuacct,cpu") == 0)
    subsystem = "cpu,cpuacct";

  إذا (subpath_بیرون)
    *subpath_بیرون = subpath;
  أرجع subsystem;
}

ساكن صحيح
restore_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free حرف *content = عدم;
  bool has_cgroup_mount = false;
  حرف *saveptr = عدم;
  صحيح cgroup_نما;
  حرف *from;
  صحيح ret;
  uint32_t i;

  cgroup_نما = libcrun_get_cgroup_نما (err);
  إذا (UNLIKELY (cgroup_نما < 0))
    أرجع cgroup_نما;

  إذا (cgroup_نما == CGROUP_نما_UNIFIED)
    أرجع 0;

  /* First check if there is actually a cgroup mount in the container. */
  لكل (i = 0; i < def->mounts_len; i++)
    {
      حرف *type = def->mounts[i]->type;
      إذا (type && strcmp (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          اكسر;
        }
    }

  إذا (! has_cgroup_mount)
    أرجع 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, عدم, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (UNLIKELY (content == عدم || content[0] == '\0'))
    أرجع crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  لكل (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (عدم, "\n", &saveptr))
    {
      cleanup_free حرف *destination = عدم;
      cleanup_free حرف *منبع = عدم;
      حرف *subsystem;
      حرف *subpath;

      subsystem = parse_cgroup_subsystem (from, &subpath);
      إذا (subsystem == عدم)
        استمر;

      ret = append_paths (&منبع, err, CGROUP_ریشه, subsystem, عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = append_paths (&destination, err, منبع, subpath, عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = libcriu_wrapper->criu_جمع_کن_ext_mount (منبع, destination);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", destination);
    }

  أرجع 0;
}

ساكن صحيح
checkpoint_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free حرف *content = عدم;
  bool has_cgroup_mount = false;
  حرف *saveptr = عدم;
  حرف *from;
  صحيح ret;
  uint32_t i;

  /* First check if there is actually a cgroup mount in the container. */
  لكل (i = 0; i < def->mounts_len; i++)
    {
      حرف *type = def->mounts[i]->type;
      إذا (type && strcmp (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          اكسر;
        }
    }

  إذا (! has_cgroup_mount)
    أرجع 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, عدم, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (UNLIKELY (content == عدم || content[0] == '\0'))
    أرجع crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  لكل (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (عدم, "\n", &saveptr))
    {
      cleanup_free حرف *منبع_مسیر = عدم;
      حرف *subsystem;

      subsystem = parse_cgroup_subsystem (from, عدم);
      إذا (subsystem == عدم)
        استمر;

      ret = append_paths (&منبع_مسیر, err, CGROUP_ریشه, subsystem, عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = libcriu_wrapper->criu_جمع_کن_ext_mount (منبع_مسیر, منبع_مسیر);
      إذا (UNLIKELY (ret < 0))
        أرجع crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", منبع_مسیر);
    }

  أرجع 0;
}

ساكن صحيح
handle_criu_config_file (libcrun_container_t *container, libcrun_error_t *err)
{
  صحيح ret;
  ثابت حرف *criu_config_annotation;
  ثابت حرف *config_file = CRIU_RUNC_CONFIG_FILE;

  criu_config_annotation = پیدا_کن_annotation (container, "org.criu.config");

  /* Ignore missing criu_set_config_file() API for compatibility with older CRIU versions,
   * and show an error only if config file is explicitly set with annotation.
   */
  إذا (libcriu_wrapper->criu_مجموعه_config_file == عدم)
    {
      إذا (criu_config_annotation)
        أرجع crun_make_error (err, 0, "libcriu RPC config files supported in CRIU >= 4.2");

      أرجع 0;
    }

  إذا (criu_config_annotation)
    config_file = criu_config_annotation;
  وإلا إذا (access (CRIU_CRUN_CONFIG_FILE, F_OK) == 0)
    config_file = CRIU_CRUN_CONFIG_FILE;

  ret = libcriu_wrapper->criu_مجموعه_config_file (config_file);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, 0, "failed to set CRIU config file");

  أرجع 0;
}

ساكن صحيح
validate_criu_نسخه (libcrun_error_t *err)
{
  صحيح ret;

  // validate that the libcriu version is at least LIBCRIU_MIN_VERSION
  ret = libcriu_wrapper->criu_check_نسخه (LIBCRIU_MIN_نسخه);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, -ret, "CRIU: failed checking version");

  إذا (ret == 0)
    أرجع crun_make_error (err, 0, "libcriu is too old");

  أرجع 0;
}

ساكن فراغ
show_criu_log (ثابت حرف *work_مسیر, ثابت حرف *لگاریتم)
{
  cleanup_free حرف *log_مسیر = عدم;
  libcrun_error_t tmp_err = عدم;
  حرف سطر[1024];
  FILE *f;

  إذا (UNLIKELY (append_paths (&log_مسیر, &tmp_err, work_مسیر, لگاریتم, عدم) < 0))
    {
      crun_error_release (&tmp_err);
      أرجع;
    }

  f = fopen (log_مسیر, "r");
  إذا (f == عدم)
    {
      إذا (errno != ENOENT)
        libcrun_error (errno, "Can't open CRIU log `%s`", log_مسیر);
      أرجع;
    }

  /* Log with error verbosity as this is the default. */
  libcrun_error (0, "--- excerpt from CRIU log `%s`", log_مسیر);
  طالما (fgets (سطر, حجم (سطر), f) != عدم)
    إذا (جستجوی_رشته (سطر, "Error ") != عدم)
      {
        سطر[strcspn (سطر, "\n")] = '\0';
        libcrun_error (0, "%s", سطر);
      }

  fclose (f);
  libcrun_error (0, "--- end of excerpt");
}

/* work_dir is the place CRIU will put its logfiles.  If not explicitly set, CRIU
 * will put the logfiles into the images_dir.  When set, create and open it and
 * hand the fd to CRIU; ownership of the returned fd stays with the caller.  */
ساكن صحيح
راهاندازی_criu_work_dir (libcrun_checkpoint_restore_t *cr_options, صحيح *work_fd_بیرون, libcrun_error_t *err)
{
  صحيح work_fd;

  إذا (cr_options->work_مسیر == عدم)
    {
      /* This is only for the error message later. */
      cr_options->work_مسیر = cr_options->image_مسیر;
      أرجع 0;
    }

  إذا (UNLIKELY ((mkdir (cr_options->work_مسیر, 0700) == -1) && (errno != EEXIST)))
    أرجع crun_make_error (err, errno, "error creating CRIU work directory `%s`", cr_options->work_مسیر);

  work_fd = open (cr_options->work_مسیر, O_DIRECTORY | O_CLOEXEC);
  إذا (UNLIKELY (work_fd == -1))
    أرجع crun_make_error (err, errno, "error opening CRIU work directory `%s`", cr_options->work_مسیر);

  libcriu_wrapper->criu_مجموعه_work_dir_fd (work_fd);
  *work_fd_بیرون = work_fd;
  أرجع 0;
}

صحيح
libcrun_container_checkpoint_linux_criu (libcrun_container_وضعیت_t *وضعیت, libcrun_container_t *container,
                                         libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper بنية libcriu_wrapper_s *wrapper = عدم;
  cleanup_free حرف *descriptors_مسیر = عدم;
  cleanup_free حرف *freezer_مسیر = عدم;
  cleanup_free حرف *مسیر = عدم;
  cleanup_close صحيح image_fd = -1;
  cleanup_close صحيح work_fd = -1;
  صحيح cgroup_نما;
  size_t i;
  صحيح ret;

  ret = بارگیری_wrapper (&wrapper, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (geteuid ())
    أرجع crun_make_error (err, 0, "checkpointing requires root");

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
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_نسخه (err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (UNLIKELY (cr_options->image_مسیر == عدم))
    أرجع crun_make_error (err, 0, "image path not set");

  ret = mkdir (cr_options->image_مسیر, 0700);
  إذا (UNLIKELY ((ret == -1) && (errno != EEXIST)))
    أرجع crun_make_error (err, errno, "error creating checkpoint directory `%s`", cr_options->image_مسیر);

  image_fd = open (cr_options->image_مسیر, O_DIRECTORY | O_CLOEXEC);
  إذا (UNLIKELY (image_fd == -1))
    أرجع crun_make_error (err, errno, "error opening checkpoint directory `%s`", cr_options->image_مسیر);

  libcriu_wrapper->criu_مجموعه_images_dir_fd (image_fd);

  /* Set up logging. */
  libcriu_wrapper->criu_مجموعه_log_level (4);
  libcriu_wrapper->criu_مجموعه_log_file (CRIU_CHECKPOINT_LOG_FILE);

  /* Set up CRIU config file */
  إذا (UNLIKELY (handle_criu_config_file (container, err)))
    أرجع -1;

  /* Setting the pid early as we can skip a lot of checkpoint setup if
   * we just do a pre-dump. The PID needs to be set always. Do it here.
   * The main process of the container is the process CRIU will checkpoint
   * and all of its children. */
  libcriu_wrapper->criu_مجموعه_pid (وضعیت->pid);

  ret = راهاندازی_criu_work_dir (cr_options, &work_fd, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

#  ifdef CRIU_PRE_DUMP_SUPPORT

  {
    صحيح criu_can_mem_track = 0;
    /* If the user uses --pre-dump for the second time or does
     * a final dump from a previous pre-dump, setting parent_path
     * is necessary so that CRIU can find which pages have not
     * changed compared to the previous dump. */
    إذا (cr_options->والد_مسیر != عدم)
      {
        criu_can_mem_track = criu_check_mem_track (err);
        إذا (UNLIKELY (criu_can_mem_track == -1))
          أرجع -1;
        libcriu_wrapper->criu_مجموعه_track_mem (true);

        /* The parent path must be relative to image path (something like ../previous-dump).
           CRIU will fail with an unclear error message if the path is not right.
         */
        إذا (UNLIKELY (cr_options->والد_مسیر[0] == '/'))
          أرجع crun_make_error (err, 0, "--parent-path must be relative");
        صحيح is_dir = crun_dir_p_at (image_fd, cr_options->والد_مسیر, false, err);
        إذا (UNLIKELY (is_dir <= 0))
          {
            إذا (is_dir < 0)
              أرجع crun_error_wrap (err, "invalid --parent-path");
            أرجع crun_make_error (err, ENOTDIR, "invalid --parent-path");
          }
        ret = libcriu_wrapper->criu_مجموعه_والد_images (cr_options->والد_مسیر);
        إذا (UNLIKELY (ret != 0))
          أرجع crun_make_error (err, -ret, "error setting CRIU parent images path to `%s`", cr_options->والد_مسیر);
      }

    إذا (cr_options->pre_dump)
      {
        إذا (criu_can_mem_track != 1)
          {
            criu_can_mem_track = criu_check_mem_track (err);
            إذا (UNLIKELY (criu_can_mem_track == -1))
              أرجع -1;
          }
        libcriu_wrapper->criu_مجموعه_track_mem (true);
        ret = libcriu_wrapper->criu_pre_dump ();
        إذا (UNLIKELY (ret != 0))
          {
            show_criu_log (cr_options->work_مسیر, CRIU_CHECKPOINT_LOG_FILE);
            أرجع crun_make_error (err, 0, "CRIU pre-dump failed: %d", ret);
          }
        أرجع 0;
      }
  }
#  endif

  /* descriptors.json is needed during restore to correctly
   * reconnect stdin, stdout, stderr. */
  ret = append_paths (&descriptors_مسیر, err, cr_options->image_مسیر, DESCRIPTORS_FILENAME, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = write_file (descriptors_مسیر, وضعیت->external_descriptors, امن_strlen (وضعیت->external_descriptors), err);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_error_wrap (err, "error saving CRIU descriptors file");

  ret = append_paths (&مسیر, err, وضعیت->bundle, وضعیت->rootfs, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcriu_wrapper->criu_مجموعه_ریشه (مسیر);
  إذا (UNLIKELY (ret != 0))
    أرجع crun_make_error (err, 0, "error setting CRIU root to `%s`", مسیر);

  cgroup_نما = libcrun_get_cgroup_نما (err);
  إذا (UNLIKELY (cgroup_نما < 0))
    أرجع cgroup_نما;

  /* For cgroup v1 we need to tell CRIU to handle all cgroup mounts as external mounts. */
  إذا (cgroup_نما != CGROUP_نما_UNIFIED)
    {
      ret = checkpoint_cgroup_v1_mount (def, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  /* Tell CRIU about external bind mounts. */
  لكل (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      إذا (is_bind_mount (def->mounts[i], عدم, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          حرف buf[PATH_MAX];
          ثابت حرف *dest_درون_ریشه;

          إذا (nofollow)
            أرجع crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_درون_ریشه = chroot_realpath (وضعیت->rootfs, def->mounts[i]->destination, buf);
          إذا (UNLIKELY (dest_درون_ریشه == عدم))
            أرجع crun_make_error (err, errno, "unable to resolve external bind mount `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          إذا (has_پیشوند (dest_درون_ریشه, وضعیت->rootfs))
            dest_درون_ریشه += امن_strlen (وضعیت->rootfs);

          ret = libcriu_wrapper->criu_جمع_کن_ext_mount (dest_درون_ریشه, dest_درون_ریشه);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", def->mounts[i]->destination);
        }
    }

  ret = ثبات_masked_paths_mounts (def, container, libcriu_wrapper, false, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

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

  لكل (i = 0; i < def->linux->namespaces_len; i++)
    {
      صحيح مقدار = libcrun_پیدا_کن_namespace (def->linux->namespaces[i]->type);
      إذا (UNLIKELY (مقدار < 0))
        أرجع crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      إذا (مقدار == CLONE_NEWNET && def->linux->namespaces[i]->مسیر != عدم)
        {
          cleanup_free حرف *external = عدم;
          بنية stat statbuf;

          ret = stat (def->linux->namespaces[i]->مسیر, &statbuf);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "unable to stat(): `%s`", def->linux->namespaces[i]->مسیر);

          xasprintf (&external, "net[%ld]:" CRIU_EXT_NETNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_جمع_کن_external (external);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }

      إذا (مقدار == CLONE_NEWPID && def->linux->namespaces[i]->مسیر != عدم)
        {
          cleanup_free حرف *external = عدم;
          بنية stat statbuf;

          ret = stat (def->linux->namespaces[i]->مسیر, &statbuf);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, errno, "unable to stat(): `%s`", def->linux->namespaces[i]->مسیر);

          xasprintf (&external, "pid[%ld]:" CRIU_EXT_PIDNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_جمع_کن_external (external);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }
    }

  /* Tell CRIU to use the freezer to pause all container processes. */
  إذا (cgroup_نما == CGROUP_نما_UNIFIED)
    {
      /* This needs CRIU 3.14. */
      ret = append_paths (&freezer_مسیر, err, CGROUP_ریشه, وضعیت->cgroup_مسیر, عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  وإلا
    {
      ret = append_paths (&freezer_مسیر, err, CGROUP_ریشه "/freezer", وضعیت->cgroup_مسیر, عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = libcriu_wrapper->criu_مجموعه_freeze_cgroup (freezer_مسیر);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, -ret, "CRIU: failed setting freezer %d", ret);

  /* Set boolean options . */
  libcriu_wrapper->criu_مجموعه_leave_running (cr_options->leave_running);
  libcriu_wrapper->criu_مجموعه_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_مجموعه_shell_کار (cr_options->shell_کار);
  libcriu_wrapper->criu_مجموعه_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_مجموعه_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_مجموعه_orphan_pts_master (true);
  libcriu_wrapper->criu_مجموعه_manage_cgroups_نما (criu_cg_نما_from_عمومی (cr_options->manage_cgroups_نما));

  libcriu_wrapper->criu_مجموعه_manage_cgroups (true);

  إذا (libcriu_wrapper->criu_مجموعه_شبکه_قفل)
    {
      صحيح قفل = 0;

      ret = criu_شبکه_قفل_from_عمومی (cr_options->شبکه_قفل_روش, &قفل, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      إذا (قفل > 0)
        {
          ret = libcriu_wrapper->criu_مجموعه_شبکه_قفل (قفل);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, 0, "CRIU: failed setting network lock");
        }
    }

  ret = libcriu_wrapper->criu_dump ();
  إذا (UNLIKELY (ret != 0))
    {
      show_criu_log (cr_options->work_مسیر, CRIU_CHECKPOINT_LOG_FILE);
      أرجع crun_make_error (err, ret < 0 ? -ret : 0, "CRIU checkpointing failed: %d", ret);
    }

  أرجع 0;
}

ساكن صحيح
prepare_restore_mounts_داخلی (runtime_spec_schema_config_schema *def, حرف *ریشه, ثابت حرف **mounted,
                                 size_t *n_mounted, libcrun_error_t *err)
{
  uint32_t i;

  /* Go through all mountpoints to be able to recreate missing mountpoints. */
  لكل (i = 0; i < def->mounts_len; i++)
    {
      حرف *dest = def->mounts[i]->destination;
      حرف *type = def->mounts[i]->type;
      cleanup_close صحيح ریشه_fd = -1;
      bool nofollow = false;
      bool on_tmpfs = false;
      صحيح is_dir = 1;
      size_t j;

      /* cgroup restore should be handled by CRIU itself */
      إذا (type && (strcmp (type, "cgroup") == 0 || strcmp (type, "cgroup2") == 0))
        استمر;

      /* Check if the mountpoint is on a tmpfs. CRIU restores
       * all tmpfs. We do need to recreate directories on a tmpfs. */
      size_t dest_len = طول_رشته (dest);
      لكل (j = 0; j < def->mounts_len; j++)
        {
          إذا (def->mounts[j]->type == عدم || strcmp (def->mounts[j]->type, "tmpfs") != 0)
            استمر;
          size_t mount_len = طول_رشته (def->mounts[j]->destination);
          إذا (mount_len < dest_len && dest[mount_len] == '/' && مقایسه_رشته (dest, def->mounts[j]->destination, mount_len) == 0)
            {
              /* This is a mountpoint which is on a tmpfs.*/
              on_tmpfs = true;
              اكسر;
            }
        }

      إذا (on_tmpfs)
        استمر;

      /* For bind mounts check if the source is a file or a directory. */
      إذا (is_bind_mount (def->mounts[i], عدم, &nofollow))
        {
          إذا (nofollow)
            أرجع crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          is_dir = crun_dir_p (def->mounts[i]->منبع, false, err);
          إذا (UNLIKELY (is_dir < 0))
            أرجع is_dir;
        }

      ریشه_fd = open (ریشه, O_RDONLY | O_CLOEXEC);
      إذا (UNLIKELY (ریشه_fd == -1))
        أرجع crun_make_error (err, errno, "error opening container root directory `%s`", ریشه);

      إذا (is_dir)
        {
          صحيح ret;

          ret = crun_امن_ensure_directory_at (ریشه_fd, ریشه, dest, 0755, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
      وإلا
        {
          صحيح ret;

          ret = crun_امن_ensure_file_at (ریشه_fd, ریشه, dest, 0755, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }

      /* Mount a bind mount source now, so that the mountpoints of the
       * mounts inside it (e.g. a nested bind mount) are created in the
       * source, where CRIU expects them.  This also happens during the
       * initial container creation, as the mounts are done in order.  */
      إذا (is_bind_mount (def->mounts[i], عدم, &nofollow))
        {
          cleanup_close صحيح dst_fd = -1;
          proc_fd_مسیر_t fd_مسیر;

          dst_fd = امن_openat (ریشه_fd, ریشه, dest, O_مسیر | O_CLOEXEC, 0, err);
          إذا (UNLIKELY (dst_fd < 0))
            أرجع dst_fd;

          /* Not recursive: the source might contain ROOT itself (e.g. when
           * it is the bundle directory), and it is not needed anyway.  */
          get_proc_self_fd_مسیر (fd_مسیر, dst_fd);
          إذا (UNLIKELY (mount (def->mounts[i]->منبع, fd_مسیر, عدم, MS_BIND, عدم) < 0))
            أرجع crun_make_error (err, errno, "bind mount `%s` to `%s`", def->mounts[i]->منبع, dest);

          mounted[(*n_mounted)++] = dest;
        }
    }

  أرجع 0;
}

/* Move the current process back to CGROUPS, as read from /proc/self/cgroup.
   A failure is not fatal, so only warn about it.  */
ساكن فراغ
move_پشت_مقصد_cgroups (ثابت حرف *cgroups)
{
  libcrun_error_t tmp_err = عدم;

  إذا (UNLIKELY (libcrun_move_self_مقصد_cgroups (cgroups, &tmp_err) < 0))
    {
      libcrun_warning ("cannot move back to the original cgroup: %s", tmp_err->پیام);
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
ساكن صحيح
criu_restore_فرزند_درون_cgroup (ثابت حرف *cgroup_مسیر, صحيح *criu_ret, libcrun_error_t *err)
{
  sigset_t all_سیگنالها, old_ماسک;
  صحيح wait_وضعیت = 0;
  pid_t pid;
  صحيح ret;

  *criu_ret = -1;

  sigfillset (&all_سیگنالها);
  ret = sigprocmask (SIG_بلوک, &all_سیگنالها, &old_ماسک);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "sigprocmask");

  /* Must be vfork: the child shares our memory space, so both *criu_ret and
     the error it creates are visible here once it is gone.  */
  pid = vfork ();
  إذا (UNLIKELY (pid < 0))
    {
      صحيح saved_errno = errno;

      sigprocmask (SIG_SETMASK, &old_ماسک, عدم);
      أرجع crun_make_error (err, saved_errno, "vfork");
    }

  إذا (pid == 0)
    {
      بنية sigaction act;
      صحيح i;

      /* The signal dispositions are not shared with the parent, so resetting
         them here does not affect it.  Keep the ignored signals ignored, as
         CRIU would inherit them if it was run directly by the caller.  */
      لكل (i = 1; i < NSIG; i++)
        {
          إذا (sigaction (i, عدم, &act) < 0 || act.sa_handler == SIG_IGN || act.sa_handler == SIG_DFL)
            استمر;

          memset (&act, 0, حجم (act));
          act.sa_handler = SIG_DFL;
          sigaction (i, &act, عدم);
        }
      sigprocmask (SIG_SETMASK, &old_ماسک, عدم);

      إذا (! is_خالی_string (cgroup_مسیر))
        {
          ret = libcrun_move_process_مقصد_cgroup (0, 0, cgroup_مسیر, false, err);
          إذا (UNLIKELY (ret < 0))
            امن_exit (EXIT_FAILURE);
        }

      *criu_ret = libcriu_wrapper->criu_restore_فرزند ();
      امن_exit (EXIT_SUCCESS);
    }

  /* The child is gone by now, it is safe to handle signals again.  */
  sigprocmask (SIG_SETMASK, &old_ماسک, عدم);

  ret = waitpid_ignore_stopped (pid, &wait_وضعیت, 0);
  إذا (UNLIKELY (ret < 0))
    {
      /* The caller might have reaped the child already, e.g. when it ignores
         SIGCHLD.  Its result is still visible, so use it.  */
      إذا (errno != ECHILD)
        أرجع crun_make_error (err, errno, "waitpid for the CRIU restore process");

      /* The child might have failed before calling CRIU, and left an error.  */
      أرجع *err != عدم ? -1 : 0;
    }

  ret = get_process_exit_وضعیت (wait_وضعیت);
  إذا (UNLIKELY (ret != EXIT_SUCCESS))
    {
      /* The child creates the error in the shared memory space, do not
         overwrite it with crun_make_error().  */
      إذا (*err == عدم)
        أرجع crun_make_error (err, 0, "the CRIU restore process exited with status %d", ret);
      أرجع -1;
    }

  أرجع 0;
}

/* Recreate the mountpoints which do not exist in the container rootfs
 * mounted at ROOT, the same way it is done on the container creation.  */
ساكن صحيح
prepare_restore_mounts (runtime_spec_schema_config_schema *def, حرف *ریشه, libcrun_error_t *err)
{
  cleanup_free ثابت حرف **mounted = xmalloc0 (حجم (حرف *) * (def->mounts_len + 1));
  cleanup_close صحيح ریشه_fd = -1;
  size_t n_mounted = 0;
  صحيح ret;

  ret = prepare_restore_mounts_داخلی (def, ریشه, mounted, &n_mounted, err);

  /* The bind mounts done above are only needed to create the mountpoints,
   * and CRIU restores the mounts itself, so undo them, in reverse order. */
  إذا (n_mounted > 0)
    ریشه_fd = open (ریشه, O_مسیر | O_CLOEXEC);
  طالما (ریشه_fd >= 0 && n_mounted > 0)
    {
      libcrun_error_t tmp_err = عدم;
      ثابت حرف *dest = mounted[--n_mounted];
      cleanup_close صحيح dst_fd = -1;
      proc_fd_مسیر_t fd_مسیر;

      dst_fd = امن_openat (ریشه_fd, ریشه, dest, O_مسیر | O_CLOEXEC, 0, &tmp_err);
      إذا (UNLIKELY (dst_fd < 0))
        {
          crun_error_release (&tmp_err);
          استمر;
        }
      get_proc_self_fd_مسیر (fd_مسیر, dst_fd);
      إذا (UNLIKELY (umount2 (fd_مسیر, MNT_DETACH) < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "unmount `%s`", dest);
    }

  أرجع ret;
}

صحيح
libcrun_container_restore_linux_criu (libcrun_container_وضعیت_t *وضعیت, libcrun_container_t *container,
                                      libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper بنية libcriu_wrapper_s *wrapper = عدم;
  cleanup_close صحيح inherit_new_net_fd = -1;
  cleanup_close صحيح inherit_new_pid_fd = -1;
  cleanup_close صحيح image_fd = -1;
  cleanup_free حرف *ریشه = عدم;
  cleanup_free حرف *bundle_cleanup = عدم;
  cleanup_free حرف *own_cgroups = عدم;
  cleanup_close صحيح work_fd = -1;
  صحيح cgroup_نما;
  صحيح criu_ret;
  صحيح ret_بیرون;
  size_t i;
  صحيح ret;

  ret = بارگیری_wrapper (&wrapper, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (geteuid ())
    أرجع crun_make_error (err, 0, "restoring requires root");

  ret = libcriu_wrapper->criu_init_opts ();
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_نسخه (err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (UNLIKELY (cr_options->image_مسیر == عدم))
    أرجع crun_make_error (err, 0, "image path not set");

  image_fd = open (cr_options->image_مسیر, O_DIRECTORY | O_CLOEXEC);
  إذا (UNLIKELY (image_fd == -1))
    أرجع crun_make_error (err, errno, "error opening checkpoint directory `%s`", cr_options->image_مسیر);

  libcriu_wrapper->criu_مجموعه_images_dir_fd (image_fd);

  /* Load descriptors.json to tell CRIU where those FDs should be connected to. */
  {
    cleanup_free حرف *descriptors_مسیر = عدم;
    cleanup_free حرف *میانگیر = عدم;
    json_object *doc = عدم;

    ret = append_paths (&descriptors_مسیر, err, cr_options->image_مسیر, DESCRIPTORS_FILENAME, عدم);
    إذا (UNLIKELY (ret < 0))
      أرجع ret;

    ret = read_all_file (descriptors_مسیر, &میانگیر, عدم, err);
    إذا (UNLIKELY (ret < 0))
      أرجع ret;

    /* descriptors.json contains a JSON array with strings
     * telling where 0, 1 and 2 have been initially been
     * pointing to. For each descriptor which points to
     * a pipe 'pipe:' we tell CRIU to reconnect that pipe
     * to the corresponding FD to have (especially) stdout
     * and stderr being correctly redirected. */
    ret = parse_json_file (&doc, میانگیر, عدم, err);
    إذا (UNLIKELY (ret < 0))
      أرجع ret;

    إذا (json_object_is_type (doc, json_type_array))
      {
        size_t i, len = json_object_array_درازی (doc);

        /* len will probably always be 3 as crun is currently only
         * recording the destination of FD 0, 1 and 2. */
        لكل (i = 0; i < len; ++i)
          {
            json_object *s = json_object_array_get_idx (doc, i);
            إذا (s && json_object_is_type (s, json_type_string))
              {
                ثابت حرف *str = json_object_get_string (s);
                إذا (has_پیشوند (str, "pipe:"))
                  libcriu_wrapper->criu_جمع_کن_inherit_fd (i, str);
              }
          }
      }
    json_object_put (doc);
  }

  /* Our std streams are the new external descriptors, as CRIU connects the
   * inherited pipes to them.  Record them now: once the container is
   * restored it is already running, and /proc/PID/fd is under its control. */
  ret = libcrun_save_external_descriptors (container, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = راهاندازی_criu_work_dir (cr_options, &work_fd, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (cr_options->lsm_profile != عدم)
    {
      ret = libcriu_wrapper->criu_مجموعه_lsm_profile (cr_options->lsm_profile);
      إذا (UNLIKELY (ret != 0))
        أرجع crun_make_error (err, -ret, "error setting LSM profile to `%s`", cr_options->lsm_profile);
    }

  إذا (cr_options->lsm_mount_context != عدم)
    {
      ret = libcriu_wrapper->criu_مجموعه_lsm_mount_context (cr_options->lsm_mount_context);
      إذا (UNLIKELY (ret != 0))
        أرجع crun_make_error (err, -ret, "error setting LSM mount context to `%s`", cr_options->lsm_mount_context);
    }

  /* do realpath on root */
  bundle_cleanup = realpath (وضعیت->bundle, عدم);
  إذا (UNLIKELY (bundle_cleanup == عدم))
    bundle_cleanup = xstrdup (وضعیت->bundle);

  /* Tell CRIU about external bind mounts. */
  لكل (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      إذا (is_bind_mount (def->mounts[i], عدم, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          حرف buf[PATH_MAX];
          ثابت حرف *dest_درون_ریشه;
          ثابت حرف *منبع = def->mounts[i]->منبع;
          cleanup_free حرف *abs_منبع = عدم;
          بیعلامت دراز flags = 0;
          size_t j;

          إذا (nofollow)
            أرجع crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_درون_ریشه = chroot_realpath (وضعیت->rootfs, def->mounts[i]->destination, buf);
          إذا (UNLIKELY (dest_درون_ریشه == عدم))
            أرجع crun_make_error (err, errno, "unable to resolve external bind mount destination `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          إذا (has_پیشوند (dest_درون_ریشه, وضعیت->rootfs))
            dest_درون_ریشه += امن_strlen (وضعیت->rootfs);

          /* A relative source is relative to the bundle, while CRIU would
             resolve it relative to its own working directory.  */
          إذا (منبع && منبع[0] != '/')
            {
              ret = append_paths (&abs_منبع, err, bundle_cleanup, منبع, عدم);
              إذا (UNLIKELY (ret < 0))
                أرجع ret;
              منبع = abs_منبع;
            }

          ret = libcriu_wrapper->criu_جمع_کن_ext_mount (dest_درون_ریشه, منبع);
          إذا (UNLIKELY (ret < 0))
            أرجع crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", منبع);

          لكل (j = 0; j < def->mounts[i]->options_len; j++)
            {
              ثابت بنية انتشار_flags_s *prop = عدم;

              إذا (def->mounts[i]->options[j])
                prop = libcrun_str2mount_flags (def->mounts[i]->options[j]);
              إذا (prop && ! prop->clear)
                flags |= prop->flags;
            }
          جمع_کن_restored_ext_mount (dest_درون_ریشه, flags);
        }
    }

  ret = ثبات_masked_paths_mounts (def, container, libcriu_wrapper, true, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* Mount the container rootfs for CRIU. */
  ret = append_paths (&ریشه, err, bundle_cleanup, "criu-root", عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = mkdir (ریشه, 0755);
  إذا (UNLIKELY (ret == -1))
    أرجع crun_make_error (err, errno, "error creating restore directory `%s`", ریشه);

  ret = mount (وضعیت->rootfs, ریشه, عدم, MS_BIND | MS_REC, عدم);
  إذا (UNLIKELY (ret == -1))
    {
      ret = crun_make_error (err, errno, "error mounting restore directory `%s`", ریشه);
      اذهب بیرون;
    }

  /* During initial container creation, crun will create mountpoints
   * defined in config.json if they do not exist. If we are restoring
   * we need to make sure these mountpoints also exist.
   * This is not perfect, as this means crun will modify a rootfs
   * even if it marked as read-only, but runc already modifies
   * the rootfs in the same way. */

  ret = prepare_restore_mounts (def, ریشه, err);
  إذا (UNLIKELY (ret < 0))
    اذهب بیرون_umount;

  ret = libcriu_wrapper->criu_مجموعه_ریشه (ریشه);
  إذا (UNLIKELY (ret != 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU root to `%s`", ریشه);
      اذهب بیرون_umount;
    }

  /* If a namespace defined in config.json we are telling
   * CRIU use that namespace when restoring the process tree.
   *
   * CRIU expects the information about the namespace like this:
   * --inherit-fd fd[<fd>]:<key>
   * The <key> needs to be the same as during checkpointing (extRootNetNS). */
  لكل (i = 0; i < def->linux->namespaces_len; i++)
    {
      ثابت صحيح open_flags_for_inherit = O_RDONLY; /* Cannot be O_CLOEXEC as it is passed to the child process. */
      صحيح مقدار = libcrun_پیدا_کن_namespace (def->linux->namespaces[i]->type);
      إذا (UNLIKELY (مقدار < 0))
        {
          ret = crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);
          اذهب بیرون_umount;
        }

      إذا (مقدار == CLONE_NEWNET && def->linux->namespaces[i]->مسیر != عدم)
        {
          inherit_new_net_fd = open (def->linux->namespaces[i]->مسیر, open_flags_for_inherit);
          إذا (UNLIKELY (inherit_new_net_fd < 0))
            {
              ret = crun_make_error (err, errno, "unable to open(): `%s`", def->linux->namespaces[i]->مسیر);
              اذهب بیرون_umount;
            }

          ret = libcriu_wrapper->criu_جمع_کن_inherit_fd (inherit_new_net_fd, CRIU_EXT_NETNS);
          إذا (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              اذهب بیرون_umount;
            }
        }

      إذا (مقدار == CLONE_NEWPID && def->linux->namespaces[i]->مسیر != عدم)
        {
          inherit_new_pid_fd = open (def->linux->namespaces[i]->مسیر, open_flags_for_inherit);
          إذا (UNLIKELY (inherit_new_pid_fd < 0))
            {
              ret = crun_make_error (err, errno, "unable to open(): `%s`", def->linux->namespaces[i]->مسیر);
              اذهب بیرون_umount;
            }

          ret = libcriu_wrapper->criu_جمع_کن_inherit_fd (inherit_new_pid_fd, CRIU_EXT_PIDNS);
          إذا (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              اذهب بیرون_umount;
            }
        }

#  ifdef CRIU_JOIN_NS_SUPPORT
      إذا (مقدار == CLONE_NEWTIME && def->linux->namespaces[i]->مسیر != عدم)
        {
          إذا (libcriu_wrapper->criu_join_ns_جمع_کن == عدم)
            {
              ret = crun_make_error (err, 0, "shared time namespace restore is supported in CRIU >= 3.16.1");
              اذهب بیرون_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_جمع_کن ("time", def->linux->namespaces[i]->مسیر, عدم);
          إذا (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->مسیر);
              اذهب بیرون_umount;
            }
        }

      إذا (مقدار == CLONE_NEWIPC && def->linux->namespaces[i]->مسیر != عدم)
        {
          إذا (libcriu_wrapper->criu_join_ns_جمع_کن == عدم)
            {
              ret = crun_make_error (err, 0, "shared ipc namespace restore is supported in CRIU >= 3.16.1");
              اذهب بیرون_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_جمع_کن ("ipc", def->linux->namespaces[i]->مسیر, عدم);
          إذا (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->مسیر);
              اذهب بیرون_umount;
            }
        }

      إذا (مقدار == CLONE_NEWUTS && def->linux->namespaces[i]->مسیر != عدم)
        {
          إذا (libcriu_wrapper->criu_join_ns_جمع_کن == عدم)
            {
              ret = crun_make_error (err, 0, "shared uts namespace restore is supported in CRIU >= 3.16.1");
              اذهب بیرون_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_جمع_کن ("uts", def->linux->namespaces[i]->مسیر, عدم);
          إذا (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->مسیر);
              اذهب بیرون_umount;
            }
        }
#  endif
    }

  /* Set up CRIU config file */
  ret = handle_criu_config_file (container, err);
  إذا (UNLIKELY (ret < 0))
    اذهب بیرون_umount;

  /* Tell CRIU if cgroup v1 needs to be handled. */
  ret = restore_cgroup_v1_mount (def, err);
  إذا (UNLIKELY (ret < 0))
    اذهب بیرون_umount;

  console_socket = cr_options->console_socket;
  libcriu_wrapper->criu_مجموعه_notify_cb (criu_notify);

  /* Set boolean options . */
  libcriu_wrapper->criu_مجموعه_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_مجموعه_shell_کار (cr_options->shell_کار);
  libcriu_wrapper->criu_مجموعه_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_مجموعه_tcp_close (cr_options->tcp_close);
  libcriu_wrapper->criu_مجموعه_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_مجموعه_orphan_pts_master (true);

  إذا (وضعیت->cgroup_مسیر)
    {
      cgroup_نما = libcrun_get_cgroup_نما (err);
      إذا (UNLIKELY (cgroup_نما < 0))
        {
          ret = cgroup_نما;
          اذهب بیرون_umount;
        }

      /* With cgroup v2, the cgroup path is only meaningful for the unified
         hierarchy, so only relocate that one.  A NULL controller would make
         CRIU relocate every hierarchy the container is in, named cgroup v1
         ones included.  As CRIU dumps such a hierarchy from wherever the
         container is in it, which is its root, as crun leaves named
         hierarchies alone, every checkpoint and restore would then copy the
         whole hierarchy into itself.  */
      ret = libcriu_wrapper->criu_جمع_کن_cg_ریشه (cgroup_نما == CGROUP_نما_UNIFIED ? "" : عدم,
                                               وضعیت->cgroup_مسیر);
      إذا (UNLIKELY (ret != 0))
        {
          ret = crun_make_error (err, 0, "error setting CRIU cgroup root to `%s`", وضعیت->cgroup_مسیر);
          اذهب بیرون_umount;
        }
    }

  libcriu_wrapper->criu_مجموعه_manage_cgroups_نما (criu_cg_نما_from_عمومی (cr_options->manage_cgroups_نما));
  libcriu_wrapper->criu_مجموعه_manage_cgroups (true);

  إذا (libcriu_wrapper->criu_مجموعه_شبکه_قفل)
    {
      صحيح قفل = 0;

      ret = criu_شبکه_قفل_from_عمومی (cr_options->شبکه_قفل_روش, &قفل, err);
      إذا (UNLIKELY (ret < 0))
        اذهب بیرون_umount;

      إذا (قفل > 0)
        {
          ret = libcriu_wrapper->criu_مجموعه_شبکه_قفل (قفل);
          إذا (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "CRIU: failed setting network lock");
              اذهب بیرون_umount;
            }
        }
    }

  libcriu_wrapper->criu_مجموعه_log_level (4);
  ret = libcriu_wrapper->criu_مجموعه_log_file (CRIU_RESTORE_LOG_FILE);
  إذا (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU log file to `%s`", CRIU_RESTORE_LOG_FILE);
      اذهب بیرون_umount;
    }

  /* criu_restore() returns the PID of the process of the restored process
   * tree. This PID will not be the same as status->pid if the container is
   * running in a PID namespace. But it will always be > 0. */
  إذا (cr_options->detach)
    {
      /* Nothing waits for the container, so there is no reason to take over
         this process: use a throw-away one.  It is used even when there is no
         cgroup to join, so that the container is not left as a child of a
         process which is not going to reap it.  */
      ret = criu_restore_فرزند_درون_cgroup (وضعیت->cgroup_مسیر, &criu_ret, err);
      إذا (UNLIKELY (ret < 0))
        اذهب بیرون_umount;

      ret = criu_ret;
    }
  وإلا
    {
      /* Like runc does, put CRIU into the container cgroup for the time of
       * restore, so the restored tasks are created there. It is necessary in
       * the "ignore" mode, where CRIU does not deal with cgroups at all, and
       * does not hurt otherwise. As CRIU is our child, do it by moving
       * ourselves there, and move back once the restore is done. */
      إذا (! is_خالی_string (وضعیت->cgroup_مسیر))
        {
          ret = read_all_file (PROC_SELF_CGROUP, &own_cgroups, عدم, err);
          إذا (UNLIKELY (ret < 0))
            اذهب بیرون_umount;

          ret = libcrun_move_process_مقصد_cgroup (0, 0, وضعیت->cgroup_مسیر, false, err);
          إذا (UNLIKELY (ret < 0))
            {
              /* Some of the cgroups might have been joined already.  */
              move_پشت_مقصد_cgroups (own_cgroups);
              اذهب بیرون_umount;
            }
        }

      ret = libcriu_wrapper->criu_restore_فرزند ();

      إذا (own_cgroups)
        move_پشت_مقصد_cgroups (own_cgroups);
    }

  إذا (UNLIKELY (ret <= 0))
    {
      show_criu_log (cr_options->work_مسیر, CRIU_RESTORE_LOG_FILE);
      ret = crun_make_error (err, 0, "CRIU restoring failed: %d", ret);
      اذهب بیرون_umount;
    }

  /* Update the status struct with the newly allocated PID. This will
   * be necessary later when moving the process into its cgroup. */
  وضعیت->pid = ret;
  ret = 0;

بیرون_umount:
  ret_بیرون = umount (ریشه);
  إذا (UNLIKELY (ret_بیرون == -1))
    {
      صحيح saved_errno = errno;
      rmdir (ریشه);
      إذا (ret < 0)
        أرجع crun_error_wrap (err, "error unmounting restore directory `%s`", ریشه);
      وإلا
        أرجع crun_make_error (err, saved_errno, "error unmounting restore directory `%s`", ریشه);
    }
بیرون:
  ret_بیرون = rmdir (ریشه);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;
  إذا (UNLIKELY (ret_بیرون == -1))
    أرجع crun_make_error (err, errno, "error removing restore directory `%s`", ریشه);
  أرجع ret;
}
#پایان_اگر
