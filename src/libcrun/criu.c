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
#تعریف_کرو GNU_ماخذ

#شامل <config.h>

#اگر HAVE_CRIU && HAVE_DLOPEN

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
حرف *chroot_realpath (ثابت حرف *chroot, ثابت حرف *راستہ, حرف resolved_راستہ[]);

/* Map the public cgroups mode (enum libcrun_cr_cgroups_mode) onto the CRIU enum.  */
ساکن شمار criu_cg_منوال
criu_cg_منوال_from_عوامی (صحیح منوال)
{
  انتخاب (منوال)
    {
    حالت LIBCRUN_CR_CG_منوال_IGNORE:
      واپس CRIU_CG_منوال_IGNORE;
    حالت LIBCRUN_CR_CG_منوال_بھرا:
      واپس CRIU_CG_منوال_بھرا;
    حالت LIBCRUN_CR_CG_منوال_STRICT:
      واپس CRIU_CG_منوال_STRICT;
    حالت LIBCRUN_CR_CG_منوال_DEFAULT:
    حالت LIBCRUN_CR_CG_منوال_SOFT:
    طےشدہ:
      /* Default to CRIU_CG_MODE_SOFT, just as runc.  */
      واپس CRIU_CG_منوال_SOFT;
    }
}

/* Map the public network lock method (enum libcrun_cr_network_lock_method) onto the
   CRIU enum.  *LOCK is set to 0 when no explicit method must be set (CRIU default).
   A method that this build cannot honor is an error, never a silent fallback to
   the CRIU default.  */
ساکن صحیح
criu_جال_تالا_from_عوامی (صحیح طریقہ, صحیح *تالا, libcrun_error_t *err)
{
  انتخاب (طریقہ)
    {
    حالت LIBCRUN_CR_جال_تالا_IPTABLES:
      *تالا = CRIU_جال_تالا_IPTABLES;
      واپس 0;
    حالت LIBCRUN_CR_جال_تالا_NFTABLES:
      *تالا = CRIU_جال_تالا_NFTABLES;
      واپس 0;
    حالت LIBCRUN_CR_جال_تالا_SKIP:
#  اگر CRIU_جال_تالا_SKIP_SUPPORT
      *تالا = CRIU_جال_تالا_SKIP;
      واپس 0;
#  ورنہ
      واپس crun_make_error (err, 0, "CRIU: the `skip` network lock method is not supported by this build");
#  endif
    حالت LIBCRUN_CR_جال_تالا_DEFAULT:
      *تالا = 0;
      واپس 0;
    طےشدہ:
      واپس crun_make_error (err, 0, "CRIU: unknown network lock method %d", طریقہ);
    }
}

ساکن ثابت حرف *console_socket = تہی;

/* The external mounts CRIU restores, and the flags the configuration asks
   for each of them.  */
ساکن ساخت libcrun_mount_flags_s *restored_ext_mounts = تہی;
ساکن size_t n_restored_ext_mounts = 0;

#  define LIBCRIU_MIN_نسخہ 31500

ساخت libcriu_wrapper_s
{
  خالی *handle;
  صحیح (*criu_جوڑو_ext_mount) (ثابت حرف *کلید, ثابت حرف *val);
  صحیح (*criu_جوڑو_external) (ثابت حرف *کلید);
  صحیح (*criu_جوڑو_inherit_fd) (صحیح fd, ثابت حرف *کلید);
  صحیح (*criu_check_نسخہ) (صحیح minimum);
  صحیح (*criu_dump) (خالی);
  صحیح (*criu_get_orphan_pts_master_fd) (خالی);
  صحیح (*criu_init_opts) (خالی);
  صحیح (*criu_notify_pid) (criu_notify_arg_t na);
#  ifdef CRIU_JOIN_NS_SUPPORT
  صحیح (*criu_join_ns_جوڑو) (ثابت حرف *ns, ثابت حرف *ns_file, ثابت حرف *extra_opt);
#  endif
#  ifdef CRIU_PRE_DUMP_SUPPORT
  صحیح (*criu_feature_check) (ساخت criu_feature_check *features, size_t سائز);
  صحیح (*criu_pre_dump) (خالی);
#  endif
  صحیح (*criu_restore_بچہ) (خالی);
  صحیح (*criu_سیٹ_freeze_cgroup) (ثابت حرف *name);
  خالی (*criu_سیٹ_file_locks) (bool file_locks);
  خالی (*criu_سیٹ_ext_unix_sk) (bool ext_unix_sk);
  صحیح (*criu_سیٹ_log_file) (ثابت حرف *log_file);
  خالی (*criu_سیٹ_log_level) (صحیح log_level);
  خالی (*criu_سیٹ_leave_running) (bool leave_running);
  خالی (*criu_سیٹ_manage_cgroups) (bool manage);
  خالی (*criu_سیٹ_manage_cgroups_منوال) (شمار criu_cg_منوال منوال);
  صحیح (*criu_سیٹ_جال_تالا) (شمار criu_جال_تالا_طریقہ طریقہ);
  خالی (*criu_سیٹ_notify_cb) (صحیح (*cb) (حرف *action, criu_notify_arg_t na));
  خالی (*criu_سیٹ_orphan_pts_master) (bool orphan_pts_master);
  خالی (*criu_سیٹ_images_dir_fd) (صحیح fd);
  صحیح (*criu_سیٹ_والد_images) (ثابت حرف *راستہ);
  خالی (*criu_سیٹ_pid) (صحیح pid);
  صحیح (*criu_سیٹ_جڑ) (ثابت حرف *جڑ);
  صحیح (*criu_جوڑو_cg_جڑ) (ثابت حرف *ctrl, ثابت حرف *راستہ);
  خالی (*criu_سیٹ_shell_کام) (bool shell_کام);
  خالی (*criu_سیٹ_tcp_established) (bool tcp_established);
  خالی (*criu_سیٹ_tcp_close) (bool tcp_close);
  خالی (*criu_سیٹ_track_mem) (bool track_mem);
  خالی (*criu_سیٹ_work_dir_fd) (صحیح fd);
  صحیح (*criu_سیٹ_lsm_profile) (ثابت حرف *name);
  صحیح (*criu_سیٹ_lsm_mount_context) (ثابت حرف *name);
  صحیح (*criu_سیٹ_config_file) (ثابت حرف *راستہ);
};

ساکن ساخت libcriu_wrapper_s *libcriu_wrapper;

ساکن اندرونی خالی
cleanup_wrapper (خالی *p)
{
  ساخت libcriu_wrapper_s **w;

  w = (ساخت libcriu_wrapper_s **) p;
  اگر (*w == تہی)
    واپس;

#  ifndef STATIC
  اگر ((*w)->handle)
    dlclose ((*w)->handle);
#  endif
  آزاد (*w);
  libcriu_wrapper = تہی;

  جب (n_restored_ext_mounts > 0)
    آزاد (restored_ext_mounts[--n_restored_ext_mounts].ہدف);
  آزاد (restored_ext_mounts);
  restored_ext_mounts = تہی;
}

#  define cleanup_wrapper __attribute__ ((cleanup (cleanup_wrapper)))

ساکن صحیح
بھرو_wrapper (ساخت libcriu_wrapper_s **wrapper_برآمد, libcrun_error_t *err)
{
  cleanup_free ساخت libcriu_wrapper_s *wrapper = xmalloc0 (حجم (*wrapper));

#  ifdef STATIC
#    define بھرو_CRIU_FUNCTION(X, ALLOW_NULL) \
      wrapper->X = &X;
#  ورنہ
#    define بھرو_CRIU_FUNCTION(X, ALLOW_NULL)                                                    \
      کرو                                                                                         \
        {                                                                                        \
          wrapper->X = dlsym (wrapper->handle, #X);                                              \
          اگر (! ALLOW_NULL && wrapper->X == تہی)                                                \
            {                                                                                    \
              dlclose (wrapper->handle);                                                         \
              واپس crun_make_error (err, 0, "could not find symbol `%s` in `libcriu.so`", #X); \
            }                                                                                    \
      } جب (0)
#  endif

#  ifndef STATIC
  wrapper->handle = dlopen ("libcriu.so.2", RTLD_NOW);
  اگر (wrapper->handle == تہی)
    واپس crun_make_error (err, 0, "could not load `libcriu.so.2`: `%s`", dlerror ());
#  endif

  بھرو_CRIU_FUNCTION (criu_جوڑو_ext_mount, false);
  بھرو_CRIU_FUNCTION (criu_جوڑو_external, false);
  بھرو_CRIU_FUNCTION (criu_جوڑو_inherit_fd, false);
  بھرو_CRIU_FUNCTION (criu_check_نسخہ, false);
  بھرو_CRIU_FUNCTION (criu_dump, false);
  بھرو_CRIU_FUNCTION (criu_get_orphan_pts_master_fd, false);
  بھرو_CRIU_FUNCTION (criu_init_opts, false);
  بھرو_CRIU_FUNCTION (criu_notify_pid, false);

#  ifdef CRIU_JOIN_NS_SUPPORT
  /* criu_join_ns_add() API was introduced with CRIU version 3.16.1
   * Here we check if this API is available at build time to support
   * compiling with older version of CRIU, and at runtime to support
   * running crun with older versions of libcriu.so.2.
   */
  بھرو_CRIU_FUNCTION (criu_join_ns_جوڑو, true);
#  endif

#  ifdef CRIU_PRE_DUMP_SUPPORT
  بھرو_CRIU_FUNCTION (criu_feature_check, false);
  بھرو_CRIU_FUNCTION (criu_pre_dump, false);
#  endif
  بھرو_CRIU_FUNCTION (criu_restore_بچہ, false);
  بھرو_CRIU_FUNCTION (criu_سیٹ_ext_unix_sk, false);
  بھرو_CRIU_FUNCTION (criu_سیٹ_file_locks, false);
  بھرو_CRIU_FUNCTION (criu_سیٹ_freeze_cgroup, false);
  بھرو_CRIU_FUNCTION (criu_سیٹ_images_dir_fd, false);
  بھرو_CRIU_FUNCTION (criu_سیٹ_leave_running, false);
  بھرو_CRIU_FUNCTION (criu_سیٹ_log_file, false);
  بھرو_CRIU_FUNCTION (criu_سیٹ_log_level, false);
  بھرو_CRIU_FUNCTION (criu_سیٹ_manage_cgroups, false);
  بھرو_CRIU_FUNCTION (criu_سیٹ_manage_cgroups_منوال, false);
  بھرو_CRIU_FUNCTION (criu_سیٹ_جال_تالا, true);
  بھرو_CRIU_FUNCTION (criu_سیٹ_notify_cb, false);
  بھرو_CRIU_FUNCTION (criu_سیٹ_orphan_pts_master, false);
  بھرو_CRIU_FUNCTION (criu_سیٹ_والد_images, false);
  بھرو_CRIU_FUNCTION (criu_سیٹ_pid, false);
  بھرو_CRIU_FUNCTION (criu_سیٹ_جڑ, false);
  بھرو_CRIU_FUNCTION (criu_جوڑو_cg_جڑ, false);
  بھرو_CRIU_FUNCTION (criu_سیٹ_shell_کام, false);
  بھرو_CRIU_FUNCTION (criu_سیٹ_tcp_established, false);
  بھرو_CRIU_FUNCTION (criu_سیٹ_tcp_close, false);
  بھرو_CRIU_FUNCTION (criu_سیٹ_track_mem, false);
  بھرو_CRIU_FUNCTION (criu_سیٹ_work_dir_fd, false);
  بھرو_CRIU_FUNCTION (criu_سیٹ_lsm_profile, false);
  بھرو_CRIU_FUNCTION (criu_سیٹ_lsm_mount_context, false);
#  اگر ! defined STATIC || defined CRIU_CONFIG_FILE
  بھرو_CRIU_FUNCTION (criu_سیٹ_config_file, true);
#  endif

  libcriu_wrapper = *wrapper_برآمد = wrapper;
  wrapper = تہی;
#  undef بھرو_CRIU_FUNCTION
  واپس 0;
}

ساکن خالی
جوڑو_restored_ext_mount (ثابت حرف *ہدف, غیرعلامتی طویل flags)
{
  restored_ext_mounts = xrealloc (restored_ext_mounts, حجم (*restored_ext_mounts) * (n_restored_ext_mounts + 1));
  restored_ext_mounts[n_restored_ext_mounts].ہدف = xstrdup (ہدف);
  restored_ext_mounts[n_restored_ext_mounts].flags = flags;
  n_restored_ext_mounts++;
}

ساکن صحیح
criu_notify (حرف *action, criu_notify_arg_t na)
{
  اگر (strcmp (action, "post-restore") == 0)
    {
      /* CRIU takes the source of an external mount from us and its flags
       * from the image, i.e. from whatever mount was at that path in the
       * container when it was checkpointed.  The container can move its
       * mount points around, so before it is resumed make sure that each
       * source is mounted with the restrictions the configuration asks for. */
      libcrun_error_t tmp_err = تہی;
      صحیح ret;

      ret = libcrun_restrict_mounts_اندر_container (libcriu_wrapper->criu_notify_pid (na), restored_ext_mounts,
                                                  n_restored_ext_mounts, &tmp_err);
      اگر (UNLIKELY (ret < 0))
        {
          libcrun_error (tmp_err->حالت_2, "cannot set the flags of the restored mounts: %s", tmp_err->پیغام);
          libcrun_error_release (&tmp_err);
          واپس ret;
        }
    }

  اگر (لڑی_موازنہ (action, "orphan-pts-master", 17) == 0)
    {
      /* CRIU sends us the master FD via the 'orphan-pts-master'
       * callback and we are passing it on to the '--console-socket'
       * if it exists. */
      cleanup_close صحیح console_socket_fd = -1;
      libcrun_error_t tmp_err = تہی;
      صحیح master_fd;
      صحیح ret;

      اگر (! console_socket)
        واپس 0;

      master_fd = libcriu_wrapper->criu_get_orphan_pts_master_fd ();

      console_socket_fd = open_unix_domain_client_socket (console_socket, 0, &tmp_err);
      اگر (UNLIKELY (console_socket_fd < 0))
        {
          libcrun_error_release (&tmp_err);
          واپس console_socket_fd;
        }
      ret = send_fd_مقصد_socket (console_socket_fd, master_fd, &tmp_err);
      اگر (UNLIKELY (ret < 0))
        {
          libcrun_error_release (&tmp_err);
          واپس ret;
        }
    }
  واپس 0;
}

#  ifdef CRIU_PRE_DUMP_SUPPORT

ساکن صحیح
criu_check_mem_track (libcrun_error_t *err)
{
  ساخت criu_feature_check features = { 0 };
  صحیح ret;

  /* Right now we are only interested in checking memory tracking.
   * Memory tracking can be disabled at different levels. aarch64
   * for example has memory tracking not implemented. It could also
   * be not enabled on other architectures. Just ask CRIU if that
   * features exists. */

  features.mem_track = true;

  ret = libcriu_wrapper->criu_feature_check (&features, حجم (features));
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, 0, "CRIU feature checking failed: %d", ret);

  اگر (features.mem_track == true)
    واپس 1;

  واپس crun_make_error (err, 0, "CRIU memory tracking not supported");
}

#  endif

ساکن صحیح
رجسٹر_masked_paths_mounts (runtime_spec_schema_config_schema *def, libcrun_container_t *container,
                              ساخت libcriu_wrapper_s *libcriu_wrapper, bool is_restore, libcrun_error_t *err)
{
  cleanup_free حرف *خالی_خانے_dir_راستہ = تہی;
  bool shared_dir_registered = false;
  size_t i;
  صحیح ret;

  برائے (i = 0; i < def->linux->masked_paths_len; i++)
    {
      ساخت stat statbuf;
      ret = stat (def->linux->masked_paths[i], &statbuf);
      اگر (ret != 0)
        جاری;

      اگر (S_ISDIR (statbuf.st_mode))
        {
          اگر (! shared_dir_registered)
            {
              ret = get_shared_خالی_خانے_directory_راستہ (&خالی_خانے_dir_راستہ,
                                                     (container->context ? container->context->state_جڑ : تہی), err);
              اگر (UNLIKELY (ret < 0))
                واپس ret;

              ret = libcriu_wrapper->criu_جوڑو_ext_mount (خالی_خانے_dir_راستہ, خالی_خانے_dir_راستہ);
              اگر (UNLIKELY (ret < 0))
                واپس crun_make_error (err, -ret, "CRIU: failed adding external mount for shared empty directory `%s`", خالی_خانے_dir_راستہ);

              shared_dir_registered = true;
            }

          ret = libcriu_wrapper->criu_جوڑو_ext_mount (def->linux->masked_paths[i], خالی_خانے_dir_راستہ);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, -ret, "CRIU: failed adding external mount for masked directory `%s`", def->linux->masked_paths[i]);

          اگر (is_restore)
            جوڑو_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
      ورنہ اگر (S_ISREG (statbuf.st_mode))
        {
          ثابت حرف *bind_ہدف = is_restore ? "/dev/null" : def->linux->masked_paths[i];
          ret = libcriu_wrapper->criu_جوڑو_ext_mount (def->linux->masked_paths[i], bind_ہدف);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", bind_ہدف);

          اگر (is_restore)
            جوڑو_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
    }

  واپس 0;
}

/* Parse one /proc/self/cgroup LINE into its subsystem, normalizing the name and
   stripping any "name=" prefix.  When SUBPATH_OUT is not NULL, the in-hierarchy
   path is stored there.  Returns the subsystem, or NULL if the line carries no
   controller and should be skipped.  LINE is modified in place.  */
ساکن حرف *
parse_cgroup_subsystem (حرف *سطر, حرف **subpath_برآمد)
{
  حرف *subsystem, *subpath, *it;

  subsystem = strchr (سطر, ':') + 1;
  subpath = strchr (subsystem, ':') + 1;
  *(subpath - 1) = '\0';

  اگر (subsystem[0] == '\0')
    واپس تہی;

  it = لڑی_تلاش (subsystem, "name=");
  اگر (it)
    subsystem = it + 5;

  اگر (strcmp (subsystem, "net_prio,net_cls") == 0)
    subsystem = "net_cls,net_prio";
  اگر (strcmp (subsystem, "cpuacct,cpu") == 0)
    subsystem = "cpu,cpuacct";

  اگر (subpath_برآمد)
    *subpath_برآمد = subpath;
  واپس subsystem;
}

ساکن صحیح
restore_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free حرف *content = تہی;
  bool has_cgroup_mount = false;
  حرف *saveptr = تہی;
  صحیح cgroup_منوال;
  حرف *from;
  صحیح ret;
  uint32_t i;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  اگر (UNLIKELY (cgroup_منوال < 0))
    واپس cgroup_منوال;

  اگر (cgroup_منوال == CGROUP_منوال_UNIFIED)
    واپس 0;

  /* First check if there is actually a cgroup mount in the container. */
  برائے (i = 0; i < def->mounts_len; i++)
    {
      حرف *type = def->mounts[i]->type;
      اگر (type && strcmp (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          توڑو;
        }
    }

  اگر (! has_cgroup_mount)
    واپس 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, تہی, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (UNLIKELY (content == تہی || content[0] == '\0'))
    واپس crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  برائے (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (تہی, "\n", &saveptr))
    {
      cleanup_free حرف *destination = تہی;
      cleanup_free حرف *ماخذ = تہی;
      حرف *subsystem;
      حرف *subpath;

      subsystem = parse_cgroup_subsystem (from, &subpath);
      اگر (subsystem == تہی)
        جاری;

      ret = append_paths (&ماخذ, err, CGROUP_جڑ, subsystem, تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = append_paths (&destination, err, ماخذ, subpath, تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = libcriu_wrapper->criu_جوڑو_ext_mount (ماخذ, destination);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", destination);
    }

  واپس 0;
}

ساکن صحیح
checkpoint_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free حرف *content = تہی;
  bool has_cgroup_mount = false;
  حرف *saveptr = تہی;
  حرف *from;
  صحیح ret;
  uint32_t i;

  /* First check if there is actually a cgroup mount in the container. */
  برائے (i = 0; i < def->mounts_len; i++)
    {
      حرف *type = def->mounts[i]->type;
      اگر (type && strcmp (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          توڑو;
        }
    }

  اگر (! has_cgroup_mount)
    واپس 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, تہی, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (UNLIKELY (content == تہی || content[0] == '\0'))
    واپس crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  برائے (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (تہی, "\n", &saveptr))
    {
      cleanup_free حرف *ماخذ_راستہ = تہی;
      حرف *subsystem;

      subsystem = parse_cgroup_subsystem (from, تہی);
      اگر (subsystem == تہی)
        جاری;

      ret = append_paths (&ماخذ_راستہ, err, CGROUP_جڑ, subsystem, تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = libcriu_wrapper->criu_جوڑو_ext_mount (ماخذ_راستہ, ماخذ_راستہ);
      اگر (UNLIKELY (ret < 0))
        واپس crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", ماخذ_راستہ);
    }

  واپس 0;
}

ساکن صحیح
handle_criu_config_file (libcrun_container_t *container, libcrun_error_t *err)
{
  صحیح ret;
  ثابت حرف *criu_config_annotation;
  ثابت حرف *config_file = CRIU_RUNC_CONFIG_FILE;

  criu_config_annotation = ڈھونڈو_annotation (container, "org.criu.config");

  /* Ignore missing criu_set_config_file() API for compatibility with older CRIU versions,
   * and show an error only if config file is explicitly set with annotation.
   */
  اگر (libcriu_wrapper->criu_سیٹ_config_file == تہی)
    {
      اگر (criu_config_annotation)
        واپس crun_make_error (err, 0, "libcriu RPC config files supported in CRIU >= 4.2");

      واپس 0;
    }

  اگر (criu_config_annotation)
    config_file = criu_config_annotation;
  ورنہ اگر (access (CRIU_CRUN_CONFIG_FILE, F_OK) == 0)
    config_file = CRIU_CRUN_CONFIG_FILE;

  ret = libcriu_wrapper->criu_سیٹ_config_file (config_file);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, 0, "failed to set CRIU config file");

  واپس 0;
}

ساکن صحیح
validate_criu_نسخہ (libcrun_error_t *err)
{
  صحیح ret;

  // validate that the libcriu version is at least LIBCRIU_MIN_VERSION
  ret = libcriu_wrapper->criu_check_نسخہ (LIBCRIU_MIN_نسخہ);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, -ret, "CRIU: failed checking version");

  اگر (ret == 0)
    واپس crun_make_error (err, 0, "libcriu is too old");

  واپس 0;
}

ساکن خالی
show_criu_log (ثابت حرف *work_راستہ, ثابت حرف *لوگارتھم)
{
  cleanup_free حرف *log_راستہ = تہی;
  libcrun_error_t tmp_err = تہی;
  حرف سطر[1024];
  FILE *f;

  اگر (UNLIKELY (append_paths (&log_راستہ, &tmp_err, work_راستہ, لوگارتھم, تہی) < 0))
    {
      crun_error_release (&tmp_err);
      واپس;
    }

  f = fopen (log_راستہ, "r");
  اگر (f == تہی)
    {
      اگر (errno != ENOENT)
        libcrun_error (errno, "Can't open CRIU log `%s`", log_راستہ);
      واپس;
    }

  /* Log with error verbosity as this is the default. */
  libcrun_error (0, "--- excerpt from CRIU log `%s`", log_راستہ);
  جب (fgets (سطر, حجم (سطر), f) != تہی)
    اگر (لڑی_تلاش (سطر, "Error ") != تہی)
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
ساکن صحیح
تیاری_criu_work_dir (libcrun_checkpoint_restore_t *cr_options, صحیح *work_fd_برآمد, libcrun_error_t *err)
{
  صحیح work_fd;

  اگر (cr_options->work_راستہ == تہی)
    {
      /* This is only for the error message later. */
      cr_options->work_راستہ = cr_options->image_راستہ;
      واپس 0;
    }

  اگر (UNLIKELY ((mkdir (cr_options->work_راستہ, 0700) == -1) && (errno != EEXIST)))
    واپس crun_make_error (err, errno, "error creating CRIU work directory `%s`", cr_options->work_راستہ);

  work_fd = open (cr_options->work_راستہ, O_DIRECTORY | O_CLOEXEC);
  اگر (UNLIKELY (work_fd == -1))
    واپس crun_make_error (err, errno, "error opening CRIU work directory `%s`", cr_options->work_راستہ);

  libcriu_wrapper->criu_سیٹ_work_dir_fd (work_fd);
  *work_fd_برآمد = work_fd;
  واپس 0;
}

صحیح
libcrun_container_checkpoint_linux_criu (libcrun_container_حالت_t *حالت_2, libcrun_container_t *container,
                                         libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper ساخت libcriu_wrapper_s *wrapper = تہی;
  cleanup_free حرف *descriptors_راستہ = تہی;
  cleanup_free حرف *freezer_راستہ = تہی;
  cleanup_free حرف *راستہ = تہی;
  cleanup_close صحیح image_fd = -1;
  cleanup_close صحیح work_fd = -1;
  صحیح cgroup_منوال;
  size_t i;
  صحیح ret;

  ret = بھرو_wrapper (&wrapper, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (geteuid ())
    واپس crun_make_error (err, 0, "checkpointing requires root");

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
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_نسخہ (err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (UNLIKELY (cr_options->image_راستہ == تہی))
    واپس crun_make_error (err, 0, "image path not set");

  ret = mkdir (cr_options->image_راستہ, 0700);
  اگر (UNLIKELY ((ret == -1) && (errno != EEXIST)))
    واپس crun_make_error (err, errno, "error creating checkpoint directory `%s`", cr_options->image_راستہ);

  image_fd = open (cr_options->image_راستہ, O_DIRECTORY | O_CLOEXEC);
  اگر (UNLIKELY (image_fd == -1))
    واپس crun_make_error (err, errno, "error opening checkpoint directory `%s`", cr_options->image_راستہ);

  libcriu_wrapper->criu_سیٹ_images_dir_fd (image_fd);

  /* Set up logging. */
  libcriu_wrapper->criu_سیٹ_log_level (4);
  libcriu_wrapper->criu_سیٹ_log_file (CRIU_CHECKPOINT_LOG_FILE);

  /* Set up CRIU config file */
  اگر (UNLIKELY (handle_criu_config_file (container, err)))
    واپس -1;

  /* Setting the pid early as we can skip a lot of checkpoint setup if
   * we just do a pre-dump. The PID needs to be set always. Do it here.
   * The main process of the container is the process CRIU will checkpoint
   * and all of its children. */
  libcriu_wrapper->criu_سیٹ_pid (حالت_2->pid);

  ret = تیاری_criu_work_dir (cr_options, &work_fd, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

#  ifdef CRIU_PRE_DUMP_SUPPORT

  {
    صحیح criu_can_mem_track = 0;
    /* If the user uses --pre-dump for the second time or does
     * a final dump from a previous pre-dump, setting parent_path
     * is necessary so that CRIU can find which pages have not
     * changed compared to the previous dump. */
    اگر (cr_options->والد_راستہ != تہی)
      {
        criu_can_mem_track = criu_check_mem_track (err);
        اگر (UNLIKELY (criu_can_mem_track == -1))
          واپس -1;
        libcriu_wrapper->criu_سیٹ_track_mem (true);

        /* The parent path must be relative to image path (something like ../previous-dump).
           CRIU will fail with an unclear error message if the path is not right.
         */
        اگر (UNLIKELY (cr_options->والد_راستہ[0] == '/'))
          واپس crun_make_error (err, 0, "--parent-path must be relative");
        صحیح is_dir = crun_dir_p_at (image_fd, cr_options->والد_راستہ, false, err);
        اگر (UNLIKELY (is_dir <= 0))
          {
            اگر (is_dir < 0)
              واپس crun_error_wrap (err, "invalid --parent-path");
            واپس crun_make_error (err, ENOTDIR, "invalid --parent-path");
          }
        ret = libcriu_wrapper->criu_سیٹ_والد_images (cr_options->والد_راستہ);
        اگر (UNLIKELY (ret != 0))
          واپس crun_make_error (err, -ret, "error setting CRIU parent images path to `%s`", cr_options->والد_راستہ);
      }

    اگر (cr_options->pre_dump)
      {
        اگر (criu_can_mem_track != 1)
          {
            criu_can_mem_track = criu_check_mem_track (err);
            اگر (UNLIKELY (criu_can_mem_track == -1))
              واپس -1;
          }
        libcriu_wrapper->criu_سیٹ_track_mem (true);
        ret = libcriu_wrapper->criu_pre_dump ();
        اگر (UNLIKELY (ret != 0))
          {
            show_criu_log (cr_options->work_راستہ, CRIU_CHECKPOINT_LOG_FILE);
            واپس crun_make_error (err, 0, "CRIU pre-dump failed: %d", ret);
          }
        واپس 0;
      }
  }
#  endif

  /* descriptors.json is needed during restore to correctly
   * reconnect stdin, stdout, stderr. */
  ret = append_paths (&descriptors_راستہ, err, cr_options->image_راستہ, DESCRIPTORS_FILENAME, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = write_file (descriptors_راستہ, حالت_2->external_descriptors, محفوظ_strlen (حالت_2->external_descriptors), err);
  اگر (UNLIKELY (ret < 0))
    واپس crun_error_wrap (err, "error saving CRIU descriptors file");

  ret = append_paths (&راستہ, err, حالت_2->bundle, حالت_2->rootfs, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = libcriu_wrapper->criu_سیٹ_جڑ (راستہ);
  اگر (UNLIKELY (ret != 0))
    واپس crun_make_error (err, 0, "error setting CRIU root to `%s`", راستہ);

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  اگر (UNLIKELY (cgroup_منوال < 0))
    واپس cgroup_منوال;

  /* For cgroup v1 we need to tell CRIU to handle all cgroup mounts as external mounts. */
  اگر (cgroup_منوال != CGROUP_منوال_UNIFIED)
    {
      ret = checkpoint_cgroup_v1_mount (def, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  /* Tell CRIU about external bind mounts. */
  برائے (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      اگر (is_bind_mount (def->mounts[i], تہی, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          حرف buf[PATH_MAX];
          ثابت حرف *dest_اندر_جڑ;

          اگر (nofollow)
            واپس crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_اندر_جڑ = chroot_realpath (حالت_2->rootfs, def->mounts[i]->destination, buf);
          اگر (UNLIKELY (dest_اندر_جڑ == تہی))
            واپس crun_make_error (err, errno, "unable to resolve external bind mount `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          اگر (has_سابقہ (dest_اندر_جڑ, حالت_2->rootfs))
            dest_اندر_جڑ += محفوظ_strlen (حالت_2->rootfs);

          ret = libcriu_wrapper->criu_جوڑو_ext_mount (dest_اندر_جڑ, dest_اندر_جڑ);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", def->mounts[i]->destination);
        }
    }

  ret = رجسٹر_masked_paths_mounts (def, container, libcriu_wrapper, false, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

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

  برائے (i = 0; i < def->linux->namespaces_len; i++)
    {
      صحیح قدر = libcrun_ڈھونڈو_namespace (def->linux->namespaces[i]->type);
      اگر (UNLIKELY (قدر < 0))
        واپس crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      اگر (قدر == CLONE_NEWNET && def->linux->namespaces[i]->راستہ != تہی)
        {
          cleanup_free حرف *external = تہی;
          ساخت stat statbuf;

          ret = stat (def->linux->namespaces[i]->راستہ, &statbuf);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "unable to stat(): `%s`", def->linux->namespaces[i]->راستہ);

          xasprintf (&external, "net[%ld]:" CRIU_EXT_NETNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_جوڑو_external (external);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }

      اگر (قدر == CLONE_NEWPID && def->linux->namespaces[i]->راستہ != تہی)
        {
          cleanup_free حرف *external = تہی;
          ساخت stat statbuf;

          ret = stat (def->linux->namespaces[i]->راستہ, &statbuf);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, errno, "unable to stat(): `%s`", def->linux->namespaces[i]->راستہ);

          xasprintf (&external, "pid[%ld]:" CRIU_EXT_PIDNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_جوڑو_external (external);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }
    }

  /* Tell CRIU to use the freezer to pause all container processes. */
  اگر (cgroup_منوال == CGROUP_منوال_UNIFIED)
    {
      /* This needs CRIU 3.14. */
      ret = append_paths (&freezer_راستہ, err, CGROUP_جڑ, حالت_2->cgroup_راستہ, تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }
  ورنہ
    {
      ret = append_paths (&freezer_راستہ, err, CGROUP_جڑ "/freezer", حالت_2->cgroup_راستہ, تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  ret = libcriu_wrapper->criu_سیٹ_freeze_cgroup (freezer_راستہ);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, -ret, "CRIU: failed setting freezer %d", ret);

  /* Set boolean options . */
  libcriu_wrapper->criu_سیٹ_leave_running (cr_options->leave_running);
  libcriu_wrapper->criu_سیٹ_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_سیٹ_shell_کام (cr_options->shell_کام);
  libcriu_wrapper->criu_سیٹ_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_سیٹ_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_سیٹ_orphan_pts_master (true);
  libcriu_wrapper->criu_سیٹ_manage_cgroups_منوال (criu_cg_منوال_from_عوامی (cr_options->manage_cgroups_منوال));

  libcriu_wrapper->criu_سیٹ_manage_cgroups (true);

  اگر (libcriu_wrapper->criu_سیٹ_جال_تالا)
    {
      صحیح تالا = 0;

      ret = criu_جال_تالا_from_عوامی (cr_options->جال_تالا_طریقہ, &تالا, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      اگر (تالا > 0)
        {
          ret = libcriu_wrapper->criu_سیٹ_جال_تالا (تالا);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, 0, "CRIU: failed setting network lock");
        }
    }

  ret = libcriu_wrapper->criu_dump ();
  اگر (UNLIKELY (ret != 0))
    {
      show_criu_log (cr_options->work_راستہ, CRIU_CHECKPOINT_LOG_FILE);
      واپس crun_make_error (err, ret < 0 ? -ret : 0, "CRIU checkpointing failed: %d", ret);
    }

  واپس 0;
}

ساکن صحیح
prepare_restore_mounts_اندرونی (runtime_spec_schema_config_schema *def, حرف *جڑ, ثابت حرف **mounted,
                                 size_t *n_mounted, libcrun_error_t *err)
{
  uint32_t i;

  /* Go through all mountpoints to be able to recreate missing mountpoints. */
  برائے (i = 0; i < def->mounts_len; i++)
    {
      حرف *dest = def->mounts[i]->destination;
      حرف *type = def->mounts[i]->type;
      cleanup_close صحیح جڑ_fd = -1;
      bool nofollow = false;
      bool on_tmpfs = false;
      صحیح is_dir = 1;
      size_t j;

      /* cgroup restore should be handled by CRIU itself */
      اگر (type && (strcmp (type, "cgroup") == 0 || strcmp (type, "cgroup2") == 0))
        جاری;

      /* Check if the mountpoint is on a tmpfs. CRIU restores
       * all tmpfs. We do need to recreate directories on a tmpfs. */
      size_t dest_len = لڑی_لمبائی (dest);
      برائے (j = 0; j < def->mounts_len; j++)
        {
          اگر (def->mounts[j]->type == تہی || strcmp (def->mounts[j]->type, "tmpfs") != 0)
            جاری;
          size_t mount_len = لڑی_لمبائی (def->mounts[j]->destination);
          اگر (mount_len < dest_len && dest[mount_len] == '/' && لڑی_موازنہ (dest, def->mounts[j]->destination, mount_len) == 0)
            {
              /* This is a mountpoint which is on a tmpfs.*/
              on_tmpfs = true;
              توڑو;
            }
        }

      اگر (on_tmpfs)
        جاری;

      /* For bind mounts check if the source is a file or a directory. */
      اگر (is_bind_mount (def->mounts[i], تہی, &nofollow))
        {
          اگر (nofollow)
            واپس crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          is_dir = crun_dir_p (def->mounts[i]->ماخذ, false, err);
          اگر (UNLIKELY (is_dir < 0))
            واپس is_dir;
        }

      جڑ_fd = open (جڑ, O_RDONLY | O_CLOEXEC);
      اگر (UNLIKELY (جڑ_fd == -1))
        واپس crun_make_error (err, errno, "error opening container root directory `%s`", جڑ);

      اگر (is_dir)
        {
          صحیح ret;

          ret = crun_محفوظ_ensure_directory_at (جڑ_fd, جڑ, dest, 0755, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }
      ورنہ
        {
          صحیح ret;

          ret = crun_محفوظ_ensure_file_at (جڑ_fd, جڑ, dest, 0755, err);
          اگر (UNLIKELY (ret < 0))
            واپس ret;
        }

      /* Mount a bind mount source now, so that the mountpoints of the
       * mounts inside it (e.g. a nested bind mount) are created in the
       * source, where CRIU expects them.  This also happens during the
       * initial container creation, as the mounts are done in order.  */
      اگر (is_bind_mount (def->mounts[i], تہی, &nofollow))
        {
          cleanup_close صحیح dst_fd = -1;
          proc_fd_راستہ_t fd_راستہ;

          dst_fd = محفوظ_openat (جڑ_fd, جڑ, dest, O_راستہ | O_CLOEXEC, 0, err);
          اگر (UNLIKELY (dst_fd < 0))
            واپس dst_fd;

          /* Not recursive: the source might contain ROOT itself (e.g. when
           * it is the bundle directory), and it is not needed anyway.  */
          get_proc_self_fd_راستہ (fd_راستہ, dst_fd);
          اگر (UNLIKELY (mount (def->mounts[i]->ماخذ, fd_راستہ, تہی, MS_BIND, تہی) < 0))
            واپس crun_make_error (err, errno, "bind mount `%s` to `%s`", def->mounts[i]->ماخذ, dest);

          mounted[(*n_mounted)++] = dest;
        }
    }

  واپس 0;
}

/* Move the current process back to CGROUPS, as read from /proc/self/cgroup.
   A failure is not fatal, so only warn about it.  */
ساکن خالی
move_عقب_مقصد_cgroups (ثابت حرف *cgroups)
{
  libcrun_error_t tmp_err = تہی;

  اگر (UNLIKELY (libcrun_move_self_مقصد_cgroups (cgroups, &tmp_err) < 0))
    {
      libcrun_warning ("cannot move back to the original cgroup: %s", tmp_err->پیغام);
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
ساکن صحیح
criu_restore_بچہ_اندر_cgroup (ثابت حرف *cgroup_راستہ, صحیح *criu_ret, libcrun_error_t *err)
{
  sigset_t all_اشارات, old_نقاب;
  صحیح wait_حالت = 0;
  pid_t pid;
  صحیح ret;

  *criu_ret = -1;

  sigfillset (&all_اشارات);
  ret = sigprocmask (SIG_بلاک, &all_اشارات, &old_نقاب);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "sigprocmask");

  /* Must be vfork: the child shares our memory space, so both *criu_ret and
     the error it creates are visible here once it is gone.  */
  pid = vfork ();
  اگر (UNLIKELY (pid < 0))
    {
      صحیح saved_errno = errno;

      sigprocmask (SIG_SETMASK, &old_نقاب, تہی);
      واپس crun_make_error (err, saved_errno, "vfork");
    }

  اگر (pid == 0)
    {
      ساخت sigaction act;
      صحیح i;

      /* The signal dispositions are not shared with the parent, so resetting
         them here does not affect it.  Keep the ignored signals ignored, as
         CRIU would inherit them if it was run directly by the caller.  */
      برائے (i = 1; i < NSIG; i++)
        {
          اگر (sigaction (i, تہی, &act) < 0 || act.sa_handler == SIG_IGN || act.sa_handler == SIG_DFL)
            جاری;

          memset (&act, 0, حجم (act));
          act.sa_handler = SIG_DFL;
          sigaction (i, &act, تہی);
        }
      sigprocmask (SIG_SETMASK, &old_نقاب, تہی);

      اگر (! is_خالی_خانے_string (cgroup_راستہ))
        {
          ret = libcrun_move_process_مقصد_cgroup (0, 0, cgroup_راستہ, false, err);
          اگر (UNLIKELY (ret < 0))
            محفوظ_exit (EXIT_FAILURE);
        }

      *criu_ret = libcriu_wrapper->criu_restore_بچہ ();
      محفوظ_exit (EXIT_SUCCESS);
    }

  /* The child is gone by now, it is safe to handle signals again.  */
  sigprocmask (SIG_SETMASK, &old_نقاب, تہی);

  ret = waitpid_ignore_stopped (pid, &wait_حالت, 0);
  اگر (UNLIKELY (ret < 0))
    {
      /* The caller might have reaped the child already, e.g. when it ignores
         SIGCHLD.  Its result is still visible, so use it.  */
      اگر (errno != ECHILD)
        واپس crun_make_error (err, errno, "waitpid for the CRIU restore process");

      /* The child might have failed before calling CRIU, and left an error.  */
      واپس *err != تہی ? -1 : 0;
    }

  ret = get_process_exit_حالت (wait_حالت);
  اگر (UNLIKELY (ret != EXIT_SUCCESS))
    {
      /* The child creates the error in the shared memory space, do not
         overwrite it with crun_make_error().  */
      اگر (*err == تہی)
        واپس crun_make_error (err, 0, "the CRIU restore process exited with status %d", ret);
      واپس -1;
    }

  واپس 0;
}

/* Recreate the mountpoints which do not exist in the container rootfs
 * mounted at ROOT, the same way it is done on the container creation.  */
ساکن صحیح
prepare_restore_mounts (runtime_spec_schema_config_schema *def, حرف *جڑ, libcrun_error_t *err)
{
  cleanup_free ثابت حرف **mounted = xmalloc0 (حجم (حرف *) * (def->mounts_len + 1));
  cleanup_close صحیح جڑ_fd = -1;
  size_t n_mounted = 0;
  صحیح ret;

  ret = prepare_restore_mounts_اندرونی (def, جڑ, mounted, &n_mounted, err);

  /* The bind mounts done above are only needed to create the mountpoints,
   * and CRIU restores the mounts itself, so undo them, in reverse order. */
  اگر (n_mounted > 0)
    جڑ_fd = open (جڑ, O_راستہ | O_CLOEXEC);
  جب (جڑ_fd >= 0 && n_mounted > 0)
    {
      libcrun_error_t tmp_err = تہی;
      ثابت حرف *dest = mounted[--n_mounted];
      cleanup_close صحیح dst_fd = -1;
      proc_fd_راستہ_t fd_راستہ;

      dst_fd = محفوظ_openat (جڑ_fd, جڑ, dest, O_راستہ | O_CLOEXEC, 0, &tmp_err);
      اگر (UNLIKELY (dst_fd < 0))
        {
          crun_error_release (&tmp_err);
          جاری;
        }
      get_proc_self_fd_راستہ (fd_راستہ, dst_fd);
      اگر (UNLIKELY (umount2 (fd_راستہ, MNT_DETACH) < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "unmount `%s`", dest);
    }

  واپس ret;
}

صحیح
libcrun_container_restore_linux_criu (libcrun_container_حالت_t *حالت_2, libcrun_container_t *container,
                                      libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper ساخت libcriu_wrapper_s *wrapper = تہی;
  cleanup_close صحیح inherit_new_net_fd = -1;
  cleanup_close صحیح inherit_new_pid_fd = -1;
  cleanup_close صحیح image_fd = -1;
  cleanup_free حرف *جڑ = تہی;
  cleanup_free حرف *bundle_cleanup = تہی;
  cleanup_free حرف *own_cgroups = تہی;
  cleanup_close صحیح work_fd = -1;
  صحیح cgroup_منوال;
  صحیح criu_ret;
  صحیح ret_برآمد;
  size_t i;
  صحیح ret;

  ret = بھرو_wrapper (&wrapper, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (geteuid ())
    واپس crun_make_error (err, 0, "restoring requires root");

  ret = libcriu_wrapper->criu_init_opts ();
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_نسخہ (err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (UNLIKELY (cr_options->image_راستہ == تہی))
    واپس crun_make_error (err, 0, "image path not set");

  image_fd = open (cr_options->image_راستہ, O_DIRECTORY | O_CLOEXEC);
  اگر (UNLIKELY (image_fd == -1))
    واپس crun_make_error (err, errno, "error opening checkpoint directory `%s`", cr_options->image_راستہ);

  libcriu_wrapper->criu_سیٹ_images_dir_fd (image_fd);

  /* Load descriptors.json to tell CRIU where those FDs should be connected to. */
  {
    cleanup_free حرف *descriptors_راستہ = تہی;
    cleanup_free حرف *بفر = تہی;
    json_object *doc = تہی;

    ret = append_paths (&descriptors_راستہ, err, cr_options->image_راستہ, DESCRIPTORS_FILENAME, تہی);
    اگر (UNLIKELY (ret < 0))
      واپس ret;

    ret = read_all_file (descriptors_راستہ, &بفر, تہی, err);
    اگر (UNLIKELY (ret < 0))
      واپس ret;

    /* descriptors.json contains a JSON array with strings
     * telling where 0, 1 and 2 have been initially been
     * pointing to. For each descriptor which points to
     * a pipe 'pipe:' we tell CRIU to reconnect that pipe
     * to the corresponding FD to have (especially) stdout
     * and stderr being correctly redirected. */
    ret = parse_json_file (&doc, بفر, تہی, err);
    اگر (UNLIKELY (ret < 0))
      واپس ret;

    اگر (json_object_is_type (doc, json_type_array))
      {
        size_t i, len = json_object_array_لمبائی (doc);

        /* len will probably always be 3 as crun is currently only
         * recording the destination of FD 0, 1 and 2. */
        برائے (i = 0; i < len; ++i)
          {
            json_object *s = json_object_array_get_idx (doc, i);
            اگر (s && json_object_is_type (s, json_type_string))
              {
                ثابت حرف *str = json_object_get_string (s);
                اگر (has_سابقہ (str, "pipe:"))
                  libcriu_wrapper->criu_جوڑو_inherit_fd (i, str);
              }
          }
      }
    json_object_put (doc);
  }

  /* Our std streams are the new external descriptors, as CRIU connects the
   * inherited pipes to them.  Record them now: once the container is
   * restored it is already running, and /proc/PID/fd is under its control. */
  ret = libcrun_save_external_descriptors (container, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = تیاری_criu_work_dir (cr_options, &work_fd, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (cr_options->lsm_profile != تہی)
    {
      ret = libcriu_wrapper->criu_سیٹ_lsm_profile (cr_options->lsm_profile);
      اگر (UNLIKELY (ret != 0))
        واپس crun_make_error (err, -ret, "error setting LSM profile to `%s`", cr_options->lsm_profile);
    }

  اگر (cr_options->lsm_mount_context != تہی)
    {
      ret = libcriu_wrapper->criu_سیٹ_lsm_mount_context (cr_options->lsm_mount_context);
      اگر (UNLIKELY (ret != 0))
        واپس crun_make_error (err, -ret, "error setting LSM mount context to `%s`", cr_options->lsm_mount_context);
    }

  /* do realpath on root */
  bundle_cleanup = realpath (حالت_2->bundle, تہی);
  اگر (UNLIKELY (bundle_cleanup == تہی))
    bundle_cleanup = xstrdup (حالت_2->bundle);

  /* Tell CRIU about external bind mounts. */
  برائے (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      اگر (is_bind_mount (def->mounts[i], تہی, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          حرف buf[PATH_MAX];
          ثابت حرف *dest_اندر_جڑ;
          ثابت حرف *ماخذ = def->mounts[i]->ماخذ;
          cleanup_free حرف *abs_ماخذ = تہی;
          غیرعلامتی طویل flags = 0;
          size_t j;

          اگر (nofollow)
            واپس crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_اندر_جڑ = chroot_realpath (حالت_2->rootfs, def->mounts[i]->destination, buf);
          اگر (UNLIKELY (dest_اندر_جڑ == تہی))
            واپس crun_make_error (err, errno, "unable to resolve external bind mount destination `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          اگر (has_سابقہ (dest_اندر_جڑ, حالت_2->rootfs))
            dest_اندر_جڑ += محفوظ_strlen (حالت_2->rootfs);

          /* A relative source is relative to the bundle, while CRIU would
             resolve it relative to its own working directory.  */
          اگر (ماخذ && ماخذ[0] != '/')
            {
              ret = append_paths (&abs_ماخذ, err, bundle_cleanup, ماخذ, تہی);
              اگر (UNLIKELY (ret < 0))
                واپس ret;
              ماخذ = abs_ماخذ;
            }

          ret = libcriu_wrapper->criu_جوڑو_ext_mount (dest_اندر_جڑ, ماخذ);
          اگر (UNLIKELY (ret < 0))
            واپس crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", ماخذ);

          برائے (j = 0; j < def->mounts[i]->options_len; j++)
            {
              ثابت ساخت اشاعت_flags_s *prop = تہی;

              اگر (def->mounts[i]->options[j])
                prop = libcrun_str2mount_flags (def->mounts[i]->options[j]);
              اگر (prop && ! prop->clear)
                flags |= prop->flags;
            }
          جوڑو_restored_ext_mount (dest_اندر_جڑ, flags);
        }
    }

  ret = رجسٹر_masked_paths_mounts (def, container, libcriu_wrapper, true, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  /* Mount the container rootfs for CRIU. */
  ret = append_paths (&جڑ, err, bundle_cleanup, "criu-root", تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = mkdir (جڑ, 0755);
  اگر (UNLIKELY (ret == -1))
    واپس crun_make_error (err, errno, "error creating restore directory `%s`", جڑ);

  ret = mount (حالت_2->rootfs, جڑ, تہی, MS_BIND | MS_REC, تہی);
  اگر (UNLIKELY (ret == -1))
    {
      ret = crun_make_error (err, errno, "error mounting restore directory `%s`", جڑ);
      جاؤ برآمد;
    }

  /* During initial container creation, crun will create mountpoints
   * defined in config.json if they do not exist. If we are restoring
   * we need to make sure these mountpoints also exist.
   * This is not perfect, as this means crun will modify a rootfs
   * even if it marked as read-only, but runc already modifies
   * the rootfs in the same way. */

  ret = prepare_restore_mounts (def, جڑ, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ برآمد_umount;

  ret = libcriu_wrapper->criu_سیٹ_جڑ (جڑ);
  اگر (UNLIKELY (ret != 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU root to `%s`", جڑ);
      جاؤ برآمد_umount;
    }

  /* If a namespace defined in config.json we are telling
   * CRIU use that namespace when restoring the process tree.
   *
   * CRIU expects the information about the namespace like this:
   * --inherit-fd fd[<fd>]:<key>
   * The <key> needs to be the same as during checkpointing (extRootNetNS). */
  برائے (i = 0; i < def->linux->namespaces_len; i++)
    {
      ثابت صحیح open_flags_for_inherit = O_RDONLY; /* Cannot be O_CLOEXEC as it is passed to the child process. */
      صحیح قدر = libcrun_ڈھونڈو_namespace (def->linux->namespaces[i]->type);
      اگر (UNLIKELY (قدر < 0))
        {
          ret = crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);
          جاؤ برآمد_umount;
        }

      اگر (قدر == CLONE_NEWNET && def->linux->namespaces[i]->راستہ != تہی)
        {
          inherit_new_net_fd = open (def->linux->namespaces[i]->راستہ, open_flags_for_inherit);
          اگر (UNLIKELY (inherit_new_net_fd < 0))
            {
              ret = crun_make_error (err, errno, "unable to open(): `%s`", def->linux->namespaces[i]->راستہ);
              جاؤ برآمد_umount;
            }

          ret = libcriu_wrapper->criu_جوڑو_inherit_fd (inherit_new_net_fd, CRIU_EXT_NETNS);
          اگر (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              جاؤ برآمد_umount;
            }
        }

      اگر (قدر == CLONE_NEWPID && def->linux->namespaces[i]->راستہ != تہی)
        {
          inherit_new_pid_fd = open (def->linux->namespaces[i]->راستہ, open_flags_for_inherit);
          اگر (UNLIKELY (inherit_new_pid_fd < 0))
            {
              ret = crun_make_error (err, errno, "unable to open(): `%s`", def->linux->namespaces[i]->راستہ);
              جاؤ برآمد_umount;
            }

          ret = libcriu_wrapper->criu_جوڑو_inherit_fd (inherit_new_pid_fd, CRIU_EXT_PIDNS);
          اگر (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              جاؤ برآمد_umount;
            }
        }

#  ifdef CRIU_JOIN_NS_SUPPORT
      اگر (قدر == CLONE_NEWTIME && def->linux->namespaces[i]->راستہ != تہی)
        {
          اگر (libcriu_wrapper->criu_join_ns_جوڑو == تہی)
            {
              ret = crun_make_error (err, 0, "shared time namespace restore is supported in CRIU >= 3.16.1");
              جاؤ برآمد_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_جوڑو ("time", def->linux->namespaces[i]->راستہ, تہی);
          اگر (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->راستہ);
              جاؤ برآمد_umount;
            }
        }

      اگر (قدر == CLONE_NEWIPC && def->linux->namespaces[i]->راستہ != تہی)
        {
          اگر (libcriu_wrapper->criu_join_ns_جوڑو == تہی)
            {
              ret = crun_make_error (err, 0, "shared ipc namespace restore is supported in CRIU >= 3.16.1");
              جاؤ برآمد_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_جوڑو ("ipc", def->linux->namespaces[i]->راستہ, تہی);
          اگر (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->راستہ);
              جاؤ برآمد_umount;
            }
        }

      اگر (قدر == CLONE_NEWUTS && def->linux->namespaces[i]->راستہ != تہی)
        {
          اگر (libcriu_wrapper->criu_join_ns_جوڑو == تہی)
            {
              ret = crun_make_error (err, 0, "shared uts namespace restore is supported in CRIU >= 3.16.1");
              جاؤ برآمد_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_جوڑو ("uts", def->linux->namespaces[i]->راستہ, تہی);
          اگر (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->راستہ);
              جاؤ برآمد_umount;
            }
        }
#  endif
    }

  /* Set up CRIU config file */
  ret = handle_criu_config_file (container, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ برآمد_umount;

  /* Tell CRIU if cgroup v1 needs to be handled. */
  ret = restore_cgroup_v1_mount (def, err);
  اگر (UNLIKELY (ret < 0))
    جاؤ برآمد_umount;

  console_socket = cr_options->console_socket;
  libcriu_wrapper->criu_سیٹ_notify_cb (criu_notify);

  /* Set boolean options . */
  libcriu_wrapper->criu_سیٹ_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_سیٹ_shell_کام (cr_options->shell_کام);
  libcriu_wrapper->criu_سیٹ_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_سیٹ_tcp_close (cr_options->tcp_close);
  libcriu_wrapper->criu_سیٹ_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_سیٹ_orphan_pts_master (true);

  اگر (حالت_2->cgroup_راستہ)
    {
      cgroup_منوال = libcrun_get_cgroup_منوال (err);
      اگر (UNLIKELY (cgroup_منوال < 0))
        {
          ret = cgroup_منوال;
          جاؤ برآمد_umount;
        }

      /* With cgroup v2, the cgroup path is only meaningful for the unified
         hierarchy, so only relocate that one.  A NULL controller would make
         CRIU relocate every hierarchy the container is in, named cgroup v1
         ones included.  As CRIU dumps such a hierarchy from wherever the
         container is in it, which is its root, as crun leaves named
         hierarchies alone, every checkpoint and restore would then copy the
         whole hierarchy into itself.  */
      ret = libcriu_wrapper->criu_جوڑو_cg_جڑ (cgroup_منوال == CGROUP_منوال_UNIFIED ? "" : تہی,
                                               حالت_2->cgroup_راستہ);
      اگر (UNLIKELY (ret != 0))
        {
          ret = crun_make_error (err, 0, "error setting CRIU cgroup root to `%s`", حالت_2->cgroup_راستہ);
          جاؤ برآمد_umount;
        }
    }

  libcriu_wrapper->criu_سیٹ_manage_cgroups_منوال (criu_cg_منوال_from_عوامی (cr_options->manage_cgroups_منوال));
  libcriu_wrapper->criu_سیٹ_manage_cgroups (true);

  اگر (libcriu_wrapper->criu_سیٹ_جال_تالا)
    {
      صحیح تالا = 0;

      ret = criu_جال_تالا_from_عوامی (cr_options->جال_تالا_طریقہ, &تالا, err);
      اگر (UNLIKELY (ret < 0))
        جاؤ برآمد_umount;

      اگر (تالا > 0)
        {
          ret = libcriu_wrapper->criu_سیٹ_جال_تالا (تالا);
          اگر (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "CRIU: failed setting network lock");
              جاؤ برآمد_umount;
            }
        }
    }

  libcriu_wrapper->criu_سیٹ_log_level (4);
  ret = libcriu_wrapper->criu_سیٹ_log_file (CRIU_RESTORE_LOG_FILE);
  اگر (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU log file to `%s`", CRIU_RESTORE_LOG_FILE);
      جاؤ برآمد_umount;
    }

  /* criu_restore() returns the PID of the process of the restored process
   * tree. This PID will not be the same as status->pid if the container is
   * running in a PID namespace. But it will always be > 0. */
  اگر (cr_options->detach)
    {
      /* Nothing waits for the container, so there is no reason to take over
         this process: use a throw-away one.  It is used even when there is no
         cgroup to join, so that the container is not left as a child of a
         process which is not going to reap it.  */
      ret = criu_restore_بچہ_اندر_cgroup (حالت_2->cgroup_راستہ, &criu_ret, err);
      اگر (UNLIKELY (ret < 0))
        جاؤ برآمد_umount;

      ret = criu_ret;
    }
  ورنہ
    {
      /* Like runc does, put CRIU into the container cgroup for the time of
       * restore, so the restored tasks are created there. It is necessary in
       * the "ignore" mode, where CRIU does not deal with cgroups at all, and
       * does not hurt otherwise. As CRIU is our child, do it by moving
       * ourselves there, and move back once the restore is done. */
      اگر (! is_خالی_خانے_string (حالت_2->cgroup_راستہ))
        {
          ret = read_all_file (PROC_SELF_CGROUP, &own_cgroups, تہی, err);
          اگر (UNLIKELY (ret < 0))
            جاؤ برآمد_umount;

          ret = libcrun_move_process_مقصد_cgroup (0, 0, حالت_2->cgroup_راستہ, false, err);
          اگر (UNLIKELY (ret < 0))
            {
              /* Some of the cgroups might have been joined already.  */
              move_عقب_مقصد_cgroups (own_cgroups);
              جاؤ برآمد_umount;
            }
        }

      ret = libcriu_wrapper->criu_restore_بچہ ();

      اگر (own_cgroups)
        move_عقب_مقصد_cgroups (own_cgroups);
    }

  اگر (UNLIKELY (ret <= 0))
    {
      show_criu_log (cr_options->work_راستہ, CRIU_RESTORE_LOG_FILE);
      ret = crun_make_error (err, 0, "CRIU restoring failed: %d", ret);
      جاؤ برآمد_umount;
    }

  /* Update the status struct with the newly allocated PID. This will
   * be necessary later when moving the process into its cgroup. */
  حالت_2->pid = ret;
  ret = 0;

برآمد_umount:
  ret_برآمد = umount (جڑ);
  اگر (UNLIKELY (ret_برآمد == -1))
    {
      صحیح saved_errno = errno;
      rmdir (جڑ);
      اگر (ret < 0)
        واپس crun_error_wrap (err, "error unmounting restore directory `%s`", جڑ);
      ورنہ
        واپس crun_make_error (err, saved_errno, "error unmounting restore directory `%s`", جڑ);
    }
برآمد:
  ret_برآمد = rmdir (جڑ);
  اگر (UNLIKELY (ret < 0))
    واپس ret;
  اگر (UNLIKELY (ret_برآمد == -1))
    واپس crun_make_error (err, errno, "error removing restore directory `%s`", جڑ);
  واپس ret;
}
#اگر_ختم
