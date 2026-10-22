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
#परिभाषय _GNU_SOURCE

#अन्तर्भाव <config.h>

#यदि HAVE_CRIU && HAVE_DLOPEN

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
वर्ण *chroot_realpath (ध्रुव वर्ण *chroot, ध्रुव वर्ण *मार्गः, वर्ण resolved_मार्गः[]);

/* Map the public cgroups mode (enum libcrun_cr_cgroups_mode) onto the CRIU enum.  */
स्थिर गणना criu_cg_बहुलकम्
criu_cg_बहुलकम्_from_लौकिकम् (पूर्णाङ्क बहुलकम्)
{
  विकल्प (बहुलकम्)
    {
    पक्ष LIBCRUN_CR_CG_बहुलकम्_IGNORE:
      प्रत्यागम CRIU_CG_बहुलकम्_IGNORE;
    पक्ष LIBCRUN_CR_CG_बहुलकम्_पूर्णम्:
      प्रत्यागम CRIU_CG_बहुलकम्_पूर्णम्;
    पक्ष LIBCRUN_CR_CG_बहुलकम्_STRICT:
      प्रत्यागम CRIU_CG_बहुलकम्_STRICT;
    पक्ष LIBCRUN_CR_CG_बहुलकम्_DEFAULT:
    पक्ष LIBCRUN_CR_CG_बहुलकम्_SOFT:
    स्वभाव:
      /* Default to CRIU_CG_MODE_SOFT, just as runc.  */
      प्रत्यागम CRIU_CG_बहुलकम्_SOFT;
    }
}

/* Map the public network lock method (enum libcrun_cr_network_lock_method) onto the
   CRIU enum.  *LOCK is set to 0 when no explicit method must be set (CRIU default).
   A method that this build cannot honor is an error, never a silent fallback to
   the CRIU default.  */
स्थिर पूर्णाङ्क
criu_जालम्_तालकम्_from_लौकिकम् (पूर्णाङ्क विधानम्, पूर्णाङ्क *तालकम्, libcrun_error_t *err)
{
  विकल्प (विधानम्)
    {
    पक्ष LIBCRUN_CR_जालम्_तालकम्_IPTABLES:
      *तालकम् = CRIU_जालम्_तालकम्_IPTABLES;
      प्रत्यागम 0;
    पक्ष LIBCRUN_CR_जालम्_तालकम्_NFTABLES:
      *तालकम् = CRIU_जालम्_तालकम्_NFTABLES;
      प्रत्यागम 0;
    पक्ष LIBCRUN_CR_जालम्_तालकम्_SKIP:
#  यदि CRIU_जालम्_तालकम्_SKIP_SUPPORT
      *तालकम् = CRIU_जालम्_तालकम्_SKIP;
      प्रत्यागम 0;
#  अन्यथा
      प्रत्यागम crun_make_error (err, 0, "CRIU: the `skip` network lock method is not supported by this build");
#  endif
    पक्ष LIBCRUN_CR_जालम्_तालकम्_DEFAULT:
      *तालकम् = 0;
      प्रत्यागम 0;
    स्वभाव:
      प्रत्यागम crun_make_error (err, 0, "CRIU: unknown network lock method %d", विधानम्);
    }
}

स्थिर ध्रुव वर्ण *console_socket = रिक्त;

/* The external mounts CRIU restores, and the flags the configuration asks
   for each of them.  */
स्थिर संरचना libcrun_mount_flags_s *restored_ext_mounts = रिक्त;
स्थिर size_t n_restored_ext_mounts = 0;

#  define LIBCRIU_MIN_संस्करणम् 31500

संरचना libcriu_wrapper_s
{
  शून्य *handle;
  पूर्णाङ्क (*criu_सङ्कलय_ext_mount) (ध्रुव वर्ण *कुञ्चिका, ध्रुव वर्ण *val);
  पूर्णाङ्क (*criu_सङ्कलय_external) (ध्रुव वर्ण *कुञ्चिका);
  पूर्णाङ्क (*criu_सङ्कलय_inherit_fd) (पूर्णाङ्क fd, ध्रुव वर्ण *कुञ्चिका);
  पूर्णाङ्क (*criu_check_संस्करणम्) (पूर्णाङ्क minimum);
  पूर्णाङ्क (*criu_dump) (शून्य);
  पूर्णाङ्क (*criu_get_orphan_pts_master_fd) (शून्य);
  पूर्णाङ्क (*criu_init_opts) (शून्य);
  पूर्णाङ्क (*criu_notify_pid) (criu_notify_arg_t na);
#  ifdef CRIU_JOIN_NS_SUPPORT
  पूर्णाङ्क (*criu_join_ns_सङ्कलय) (ध्रुव वर्ण *ns, ध्रुव वर्ण *ns_file, ध्रुव वर्ण *extra_opt);
#  endif
#  ifdef CRIU_PRE_DUMP_SUPPORT
  पूर्णाङ्क (*criu_feature_check) (संरचना criu_feature_check *features, size_t आकार);
  पूर्णाङ्क (*criu_pre_dump) (शून्य);
#  endif
  पूर्णाङ्क (*criu_restore_child) (शून्य);
  पूर्णाङ्क (*criu_समुच्चयः_freeze_cgroup) (ध्रुव वर्ण *name);
  शून्य (*criu_समुच्चयः_file_locks) (bool file_locks);
  शून्य (*criu_समुच्चयः_ext_unix_sk) (bool ext_unix_sk);
  पूर्णाङ्क (*criu_समुच्चयः_log_file) (ध्रुव वर्ण *log_file);
  शून्य (*criu_समुच्चयः_log_level) (पूर्णाङ्क log_level);
  शून्य (*criu_समुच्चयः_leave_running) (bool leave_running);
  शून्य (*criu_समुच्चयः_manage_cgroups) (bool manage);
  शून्य (*criu_समुच्चयः_manage_cgroups_बहुलकम्) (गणना criu_cg_बहुलकम् बहुलकम्);
  पूर्णाङ्क (*criu_समुच्चयः_जालम्_तालकम्) (गणना criu_जालम्_तालकम्_विधानम् विधानम्);
  शून्य (*criu_समुच्चयः_notify_cb) (पूर्णाङ्क (*cb) (वर्ण *action, criu_notify_arg_t na));
  शून्य (*criu_समुच्चयः_orphan_pts_master) (bool orphan_pts_master);
  शून्य (*criu_समुच्चयः_images_dir_fd) (पूर्णाङ्क fd);
  पूर्णाङ्क (*criu_समुच्चयः_parent_images) (ध्रुव वर्ण *मार्गः);
  शून्य (*criu_समुच्चयः_pid) (पूर्णाङ्क pid);
  पूर्णाङ्क (*criu_समुच्चयः_मूलम्) (ध्रुव वर्ण *मूलम्);
  पूर्णाङ्क (*criu_सङ्कलय_cg_मूलम्) (ध्रुव वर्ण *ctrl, ध्रुव वर्ण *मार्गः);
  शून्य (*criu_समुच्चयः_shell_job) (bool shell_job);
  शून्य (*criu_समुच्चयः_tcp_established) (bool tcp_established);
  शून्य (*criu_समुच्चयः_tcp_close) (bool tcp_close);
  शून्य (*criu_समुच्चयः_track_mem) (bool track_mem);
  शून्य (*criu_समुच्चयः_work_dir_fd) (पूर्णाङ्क fd);
  पूर्णाङ्क (*criu_समुच्चयः_lsm_profile) (ध्रुव वर्ण *name);
  पूर्णाङ्क (*criu_समुच्चयः_lsm_mount_context) (ध्रुव वर्ण *name);
  पूर्णाङ्क (*criu_समुच्चयः_config_file) (ध्रुव वर्ण *मार्गः);
};

स्थिर संरचना libcriu_wrapper_s *libcriu_wrapper;

स्थिर अन्तःस्थ शून्य
cleanup_wrapper (शून्य *p)
{
  संरचना libcriu_wrapper_s **w;

  w = (संरचना libcriu_wrapper_s **) p;
  यदि (*w == रिक्त)
    प्रत्यागम;

#  ifndef STATIC
  यदि ((*w)->handle)
    dlclose ((*w)->handle);
#  endif
  मुक्त (*w);
  libcriu_wrapper = रिक्त;

  यावत् (n_restored_ext_mounts > 0)
    मुक्त (restored_ext_mounts[--n_restored_ext_mounts].target);
  मुक्त (restored_ext_mounts);
  restored_ext_mounts = रिक्त;
}

#  define cleanup_wrapper __attribute__ ((cleanup (cleanup_wrapper)))

स्थिर पूर्णाङ्क
आरोपय_wrapper (संरचना libcriu_wrapper_s **wrapper_बहिर्गमनम्, libcrun_error_t *err)
{
  cleanup_free संरचना libcriu_wrapper_s *wrapper = xmalloc0 (परिमाण (*wrapper));

#  ifdef STATIC
#    define आरोपय_CRIU_FUNCTION(X, ALLOW_NULL) \
      wrapper->X = &X;
#  अन्यथा
#    define आरोपय_CRIU_FUNCTION(X, ALLOW_NULL)                                                    \
      कुरु                                                                                         \
        {                                                                                        \
          wrapper->X = dlsym (wrapper->handle, #X);                                              \
          यदि (! ALLOW_NULL && wrapper->X == रिक्त)                                                \
            {                                                                                    \
              dlclose (wrapper->handle);                                                         \
              प्रत्यागम crun_make_error (err, 0, "could not find symbol `%s` in `libcriu.so`", #X); \
            }                                                                                    \
      } यावत् (0)
#  endif

#  ifndef STATIC
  wrapper->handle = dlopen ("libcriu.so.2", RTLD_NOW);
  यदि (wrapper->handle == रिक्त)
    प्रत्यागम crun_make_error (err, 0, "could not load `libcriu.so.2`: `%s`", dlerror ());
#  endif

  आरोपय_CRIU_FUNCTION (criu_सङ्कलय_ext_mount, false);
  आरोपय_CRIU_FUNCTION (criu_सङ्कलय_external, false);
  आरोपय_CRIU_FUNCTION (criu_सङ्कलय_inherit_fd, false);
  आरोपय_CRIU_FUNCTION (criu_check_संस्करणम्, false);
  आरोपय_CRIU_FUNCTION (criu_dump, false);
  आरोपय_CRIU_FUNCTION (criu_get_orphan_pts_master_fd, false);
  आरोपय_CRIU_FUNCTION (criu_init_opts, false);
  आरोपय_CRIU_FUNCTION (criu_notify_pid, false);

#  ifdef CRIU_JOIN_NS_SUPPORT
  /* criu_join_ns_add() API was introduced with CRIU version 3.16.1
   * Here we check if this API is available at build time to support
   * compiling with older version of CRIU, and at runtime to support
   * running crun with older versions of libcriu.so.2.
   */
  आरोपय_CRIU_FUNCTION (criu_join_ns_सङ्कलय, true);
#  endif

#  ifdef CRIU_PRE_DUMP_SUPPORT
  आरोपय_CRIU_FUNCTION (criu_feature_check, false);
  आरोपय_CRIU_FUNCTION (criu_pre_dump, false);
#  endif
  आरोपय_CRIU_FUNCTION (criu_restore_child, false);
  आरोपय_CRIU_FUNCTION (criu_समुच्चयः_ext_unix_sk, false);
  आरोपय_CRIU_FUNCTION (criu_समुच्चयः_file_locks, false);
  आरोपय_CRIU_FUNCTION (criu_समुच्चयः_freeze_cgroup, false);
  आरोपय_CRIU_FUNCTION (criu_समुच्चयः_images_dir_fd, false);
  आरोपय_CRIU_FUNCTION (criu_समुच्चयः_leave_running, false);
  आरोपय_CRIU_FUNCTION (criu_समुच्चयः_log_file, false);
  आरोपय_CRIU_FUNCTION (criu_समुच्चयः_log_level, false);
  आरोपय_CRIU_FUNCTION (criu_समुच्चयः_manage_cgroups, false);
  आरोपय_CRIU_FUNCTION (criu_समुच्चयः_manage_cgroups_बहुलकम्, false);
  आरोपय_CRIU_FUNCTION (criu_समुच्चयः_जालम्_तालकम्, true);
  आरोपय_CRIU_FUNCTION (criu_समुच्चयः_notify_cb, false);
  आरोपय_CRIU_FUNCTION (criu_समुच्चयः_orphan_pts_master, false);
  आरोपय_CRIU_FUNCTION (criu_समुच्चयः_parent_images, false);
  आरोपय_CRIU_FUNCTION (criu_समुच्चयः_pid, false);
  आरोपय_CRIU_FUNCTION (criu_समुच्चयः_मूलम्, false);
  आरोपय_CRIU_FUNCTION (criu_सङ्कलय_cg_मूलम्, false);
  आरोपय_CRIU_FUNCTION (criu_समुच्चयः_shell_job, false);
  आरोपय_CRIU_FUNCTION (criu_समुच्चयः_tcp_established, false);
  आरोपय_CRIU_FUNCTION (criu_समुच्चयः_tcp_close, false);
  आरोपय_CRIU_FUNCTION (criu_समुच्चयः_track_mem, false);
  आरोपय_CRIU_FUNCTION (criu_समुच्चयः_work_dir_fd, false);
  आरोपय_CRIU_FUNCTION (criu_समुच्चयः_lsm_profile, false);
  आरोपय_CRIU_FUNCTION (criu_समुच्चयः_lsm_mount_context, false);
#  यदि ! defined STATIC || defined CRIU_CONFIG_FILE
  आरोपय_CRIU_FUNCTION (criu_समुच्चयः_config_file, true);
#  endif

  libcriu_wrapper = *wrapper_बहिर्गमनम् = wrapper;
  wrapper = रिक्त;
#  undef आरोपय_CRIU_FUNCTION
  प्रत्यागम 0;
}

स्थिर शून्य
सङ्कलय_restored_ext_mount (ध्रुव वर्ण *target, अचिह्नित दीर्घ flags)
{
  restored_ext_mounts = xrealloc (restored_ext_mounts, परिमाण (*restored_ext_mounts) * (n_restored_ext_mounts + 1));
  restored_ext_mounts[n_restored_ext_mounts].target = xstrdup (target);
  restored_ext_mounts[n_restored_ext_mounts].flags = flags;
  n_restored_ext_mounts++;
}

स्थिर पूर्णाङ्क
criu_notify (वर्ण *action, criu_notify_arg_t na)
{
  यदि (वाक्यं_तुलय (action, "post-restore") == 0)
    {
      /* CRIU takes the source of an external mount from us and its flags
       * from the image, i.e. from whatever mount was at that path in the
       * container when it was checkpointed.  The container can move its
       * mount points around, so before it is resumed make sure that each
       * source is mounted with the restrictions the configuration asks for. */
      libcrun_error_t tmp_err = रिक्त;
      पूर्णाङ्क ret;

      ret = libcrun_restrict_mounts_अन्तर्_container (libcriu_wrapper->criu_notify_pid (na), restored_ext_mounts,
                                                  n_restored_ext_mounts, &tmp_err);
      यदि (UNLIKELY (ret < 0))
        {
          libcrun_error (tmp_err->स्थितिः, "cannot set the flags of the restored mounts: %s", tmp_err->सन्देशः);
          libcrun_error_release (&tmp_err);
          प्रत्यागम ret;
        }
    }

  यदि (वाक्ये_तुलय (action, "orphan-pts-master", 17) == 0)
    {
      /* CRIU sends us the master FD via the 'orphan-pts-master'
       * callback and we are passing it on to the '--console-socket'
       * if it exists. */
      cleanup_close पूर्णाङ्क console_socket_fd = -1;
      libcrun_error_t tmp_err = रिक्त;
      पूर्णाङ्क master_fd;
      पूर्णाङ्क ret;

      यदि (! console_socket)
        प्रत्यागम 0;

      master_fd = libcriu_wrapper->criu_get_orphan_pts_master_fd ();

      console_socket_fd = open_unix_domain_client_socket (console_socket, 0, &tmp_err);
      यदि (UNLIKELY (console_socket_fd < 0))
        {
          libcrun_error_release (&tmp_err);
          प्रत्यागम console_socket_fd;
        }
      ret = send_fd_गन्तव्यम्_socket (console_socket_fd, master_fd, &tmp_err);
      यदि (UNLIKELY (ret < 0))
        {
          libcrun_error_release (&tmp_err);
          प्रत्यागम ret;
        }
    }
  प्रत्यागम 0;
}

#  ifdef CRIU_PRE_DUMP_SUPPORT

स्थिर पूर्णाङ्क
criu_check_mem_track (libcrun_error_t *err)
{
  संरचना criu_feature_check features = { 0 };
  पूर्णाङ्क ret;

  /* Right now we are only interested in checking memory tracking.
   * Memory tracking can be disabled at different levels. aarch64
   * for example has memory tracking not implemented. It could also
   * be not enabled on other architectures. Just ask CRIU if that
   * features exists. */

  features.mem_track = true;

  ret = libcriu_wrapper->criu_feature_check (&features, परिमाण (features));
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, 0, "CRIU feature checking failed: %d", ret);

  यदि (features.mem_track == true)
    प्रत्यागम 1;

  प्रत्यागम crun_make_error (err, 0, "CRIU memory tracking not supported");
}

#  endif

स्थिर पूर्णाङ्क
पञ्जिकाकोशः_masked_paths_mounts (runtime_spec_schema_config_schema *def, libcrun_container_t *container,
                              संरचना libcriu_wrapper_s *libcriu_wrapper, bool is_restore, libcrun_error_t *err)
{
  cleanup_free वर्ण *रिक्तानि_dir_मार्गः = रिक्त;
  bool shared_dir_registered = false;
  size_t i;
  पूर्णाङ्क ret;

  प्रति (i = 0; i < def->linux->masked_paths_len; i++)
    {
      संरचना stat statbuf;
      ret = stat (def->linux->masked_paths[i], &statbuf);
      यदि (ret != 0)
        अनुवर्त;

      यदि (S_ISDIR (statbuf.st_mode))
        {
          यदि (! shared_dir_registered)
            {
              ret = get_shared_रिक्तानि_directory_मार्गः (&रिक्तानि_dir_मार्गः,
                                                     (container->context ? container->context->state_मूलम् : रिक्त), err);
              यदि (UNLIKELY (ret < 0))
                प्रत्यागम ret;

              ret = libcriu_wrapper->criu_सङ्कलय_ext_mount (रिक्तानि_dir_मार्गः, रिक्तानि_dir_मार्गः);
              यदि (UNLIKELY (ret < 0))
                प्रत्यागम crun_make_error (err, -ret, "CRIU: failed adding external mount for shared empty directory `%s`", रिक्तानि_dir_मार्गः);

              shared_dir_registered = true;
            }

          ret = libcriu_wrapper->criu_सङ्कलय_ext_mount (def->linux->masked_paths[i], रिक्तानि_dir_मार्गः);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, -ret, "CRIU: failed adding external mount for masked directory `%s`", def->linux->masked_paths[i]);

          यदि (is_restore)
            सङ्कलय_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
      अन्यथा यदि (S_ISREG (statbuf.st_mode))
        {
          ध्रुव वर्ण *bind_target = is_restore ? "/dev/null" : def->linux->masked_paths[i];
          ret = libcriu_wrapper->criu_सङ्कलय_ext_mount (def->linux->masked_paths[i], bind_target);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", bind_target);

          यदि (is_restore)
            सङ्कलय_restored_ext_mount (def->linux->masked_paths[i], MS_RDONLY);
        }
    }

  प्रत्यागम 0;
}

/* Parse one /proc/self/cgroup LINE into its subsystem, normalizing the name and
   stripping any "name=" prefix.  When SUBPATH_OUT is not NULL, the in-hierarchy
   path is stored there.  Returns the subsystem, or NULL if the line carries no
   controller and should be skipped.  LINE is modified in place.  */
स्थिर वर्ण *
parse_cgroup_subsystem (वर्ण *पङ्क्तिः, वर्ण **subpath_बहिर्गमनम्)
{
  वर्ण *subsystem, *subpath, *it;

  subsystem = strchr (पङ्क्तिः, ':') + 1;
  subpath = strchr (subsystem, ':') + 1;
  *(subpath - 1) = '\0';

  यदि (subsystem[0] == '\0')
    प्रत्यागम रिक्त;

  it = वाक्ये_अन्विष (subsystem, "name=");
  यदि (it)
    subsystem = it + 5;

  यदि (वाक्यं_तुलय (subsystem, "net_prio,net_cls") == 0)
    subsystem = "net_cls,net_prio";
  यदि (वाक्यं_तुलय (subsystem, "cpuacct,cpu") == 0)
    subsystem = "cpu,cpuacct";

  यदि (subpath_बहिर्गमनम्)
    *subpath_बहिर्गमनम् = subpath;
  प्रत्यागम subsystem;
}

स्थिर पूर्णाङ्क
restore_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free वर्ण *content = रिक्त;
  bool has_cgroup_mount = false;
  वर्ण *saveptr = रिक्त;
  पूर्णाङ्क cgroup_बहुलकम्;
  वर्ण *from;
  पूर्णाङ्क ret;
  uint32_t i;

  cgroup_बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
  यदि (UNLIKELY (cgroup_बहुलकम् < 0))
    प्रत्यागम cgroup_बहुलकम्;

  यदि (cgroup_बहुलकम् == CGROUP_बहुलकम्_UNIFIED)
    प्रत्यागम 0;

  /* First check if there is actually a cgroup mount in the container. */
  प्रति (i = 0; i < def->mounts_len; i++)
    {
      वर्ण *type = def->mounts[i]->type;
      यदि (type && वाक्यं_तुलय (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          भङ्ग;
        }
    }

  यदि (! has_cgroup_mount)
    प्रत्यागम 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (UNLIKELY (content == रिक्त || content[0] == '\0'))
    प्रत्यागम crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  प्रति (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (रिक्त, "\n", &saveptr))
    {
      cleanup_free वर्ण *destination = रिक्त;
      cleanup_free वर्ण *source = रिक्त;
      वर्ण *subsystem;
      वर्ण *subpath;

      subsystem = parse_cgroup_subsystem (from, &subpath);
      यदि (subsystem == रिक्त)
        अनुवर्त;

      ret = append_paths (&source, err, CGROUP_मूलम्, subsystem, रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = append_paths (&destination, err, source, subpath, रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = libcriu_wrapper->criu_सङ्कलय_ext_mount (source, destination);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", destination);
    }

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
checkpoint_cgroup_v1_mount (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  cleanup_free वर्ण *content = रिक्त;
  bool has_cgroup_mount = false;
  वर्ण *saveptr = रिक्त;
  वर्ण *from;
  पूर्णाङ्क ret;
  uint32_t i;

  /* First check if there is actually a cgroup mount in the container. */
  प्रति (i = 0; i < def->mounts_len; i++)
    {
      वर्ण *type = def->mounts[i]->type;
      यदि (type && वाक्यं_तुलय (type, "cgroup") == 0)
        {
          has_cgroup_mount = true;
          भङ्ग;
        }
    }

  यदि (! has_cgroup_mount)
    प्रत्यागम 0;

  ret = read_all_file (PROC_SELF_CGROUP, &content, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (UNLIKELY (content == रिक्त || content[0] == '\0'))
    प्रत्यागम crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  प्रति (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (रिक्त, "\n", &saveptr))
    {
      cleanup_free वर्ण *source_मार्गः = रिक्त;
      वर्ण *subsystem;

      subsystem = parse_cgroup_subsystem (from, रिक्त);
      यदि (subsystem == रिक्त)
        अनुवर्त;

      ret = append_paths (&source_मार्गः, err, CGROUP_मूलम्, subsystem, रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = libcriu_wrapper->criu_सङ्कलय_ext_mount (source_मार्गः, source_मार्गः);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", source_मार्गः);
    }

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
handle_criu_config_file (libcrun_container_t *container, libcrun_error_t *err)
{
  पूर्णाङ्क ret;
  ध्रुव वर्ण *criu_config_annotation;
  ध्रुव वर्ण *config_file = CRIU_RUNC_CONFIG_FILE;

  criu_config_annotation = find_annotation (container, "org.criu.config");

  /* Ignore missing criu_set_config_file() API for compatibility with older CRIU versions,
   * and show an error only if config file is explicitly set with annotation.
   */
  यदि (libcriu_wrapper->criu_समुच्चयः_config_file == रिक्त)
    {
      यदि (criu_config_annotation)
        प्रत्यागम crun_make_error (err, 0, "libcriu RPC config files supported in CRIU >= 4.2");

      प्रत्यागम 0;
    }

  यदि (criu_config_annotation)
    config_file = criu_config_annotation;
  अन्यथा यदि (access (CRIU_CRUN_CONFIG_FILE, F_OK) == 0)
    config_file = CRIU_CRUN_CONFIG_FILE;

  ret = libcriu_wrapper->criu_समुच्चयः_config_file (config_file);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, 0, "failed to set CRIU config file");

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
validate_criu_संस्करणम् (libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  // validate that the libcriu version is at least LIBCRIU_MIN_VERSION
  ret = libcriu_wrapper->criu_check_संस्करणम् (LIBCRIU_MIN_संस्करणम्);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, -ret, "CRIU: failed checking version");

  यदि (ret == 0)
    प्रत्यागम crun_make_error (err, 0, "libcriu is too old");

  प्रत्यागम 0;
}

स्थिर शून्य
show_criu_log (ध्रुव वर्ण *work_मार्गः, ध्रुव वर्ण *लघुगणकः)
{
  cleanup_free वर्ण *log_मार्गः = रिक्त;
  libcrun_error_t tmp_err = रिक्त;
  वर्ण पङ्क्तिः[1024];
  FILE *f;

  यदि (UNLIKELY (append_paths (&log_मार्गः, &tmp_err, work_मार्गः, लघुगणकः, रिक्त) < 0))
    {
      crun_error_release (&tmp_err);
      प्रत्यागम;
    }

  f = fopen (log_मार्गः, "r");
  यदि (f == रिक्त)
    {
      यदि (errno != ENOENT)
        libcrun_error (errno, "Can't open CRIU log `%s`", log_मार्गः);
      प्रत्यागम;
    }

  /* Log with error verbosity as this is the default. */
  libcrun_error (0, "--- excerpt from CRIU log `%s`", log_मार्गः);
  यावत् (fgets (पङ्क्तिः, परिमाण (पङ्क्तिः), f) != रिक्त)
    यदि (वाक्ये_अन्विष (पङ्क्तिः, "Error ") != रिक्त)
      {
        पङ्क्तिः[strcspn (पङ्क्तिः, "\n")] = '\0';
        libcrun_error (0, "%s", पङ्क्तिः);
      }

  fclose (f);
  libcrun_error (0, "--- end of excerpt");
}

/* work_dir is the place CRIU will put its logfiles.  If not explicitly set, CRIU
 * will put the logfiles into the images_dir.  When set, create and open it and
 * hand the fd to CRIU; ownership of the returned fd stays with the caller.  */
स्थिर पूर्णाङ्क
सज्जता_criu_work_dir (libcrun_checkpoint_restore_t *cr_options, पूर्णाङ्क *work_fd_बहिर्गमनम्, libcrun_error_t *err)
{
  पूर्णाङ्क work_fd;

  यदि (cr_options->work_मार्गः == रिक्त)
    {
      /* This is only for the error message later. */
      cr_options->work_मार्गः = cr_options->image_मार्गः;
      प्रत्यागम 0;
    }

  यदि (UNLIKELY ((mkdir (cr_options->work_मार्गः, 0700) == -1) && (errno != EEXIST)))
    प्रत्यागम crun_make_error (err, errno, "error creating CRIU work directory `%s`", cr_options->work_मार्गः);

  work_fd = उद्घाटय (cr_options->work_मार्गः, O_DIRECTORY | O_CLOEXEC);
  यदि (UNLIKELY (work_fd == -1))
    प्रत्यागम crun_make_error (err, errno, "error opening CRIU work directory `%s`", cr_options->work_मार्गः);

  libcriu_wrapper->criu_समुच्चयः_work_dir_fd (work_fd);
  *work_fd_बहिर्गमनम् = work_fd;
  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_container_checkpoint_linux_criu (libcrun_container_स्थितिः_t *स्थितिः, libcrun_container_t *container,
                                         libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper संरचना libcriu_wrapper_s *wrapper = रिक्त;
  cleanup_free वर्ण *descriptors_मार्गः = रिक्त;
  cleanup_free वर्ण *freezer_मार्गः = रिक्त;
  cleanup_free वर्ण *मार्गः = रिक्त;
  cleanup_close पूर्णाङ्क image_fd = -1;
  cleanup_close पूर्णाङ्क work_fd = -1;
  पूर्णाङ्क cgroup_बहुलकम्;
  size_t i;
  पूर्णाङ्क ret;

  ret = आरोपय_wrapper (&wrapper, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (geteuid ())
    प्रत्यागम crun_make_error (err, 0, "checkpointing requires root");

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
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_संस्करणम् (err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (UNLIKELY (cr_options->image_मार्गः == रिक्त))
    प्रत्यागम crun_make_error (err, 0, "image path not set");

  ret = mkdir (cr_options->image_मार्गः, 0700);
  यदि (UNLIKELY ((ret == -1) && (errno != EEXIST)))
    प्रत्यागम crun_make_error (err, errno, "error creating checkpoint directory `%s`", cr_options->image_मार्गः);

  image_fd = उद्घाटय (cr_options->image_मार्गः, O_DIRECTORY | O_CLOEXEC);
  यदि (UNLIKELY (image_fd == -1))
    प्रत्यागम crun_make_error (err, errno, "error opening checkpoint directory `%s`", cr_options->image_मार्गः);

  libcriu_wrapper->criu_समुच्चयः_images_dir_fd (image_fd);

  /* Set up logging. */
  libcriu_wrapper->criu_समुच्चयः_log_level (4);
  libcriu_wrapper->criu_समुच्चयः_log_file (CRIU_CHECKPOINT_LOG_FILE);

  /* Set up CRIU config file */
  यदि (UNLIKELY (handle_criu_config_file (container, err)))
    प्रत्यागम -1;

  /* Setting the pid early as we can skip a lot of checkpoint setup if
   * we just do a pre-dump. The PID needs to be set always. Do it here.
   * The main process of the container is the process CRIU will checkpoint
   * and all of its children. */
  libcriu_wrapper->criu_समुच्चयः_pid (स्थितिः->pid);

  ret = सज्जता_criu_work_dir (cr_options, &work_fd, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

#  ifdef CRIU_PRE_DUMP_SUPPORT

  {
    पूर्णाङ्क criu_can_mem_track = 0;
    /* If the user uses --pre-dump for the second time or does
     * a final dump from a previous pre-dump, setting parent_path
     * is necessary so that CRIU can find which pages have not
     * changed compared to the previous dump. */
    यदि (cr_options->parent_मार्गः != रिक्त)
      {
        criu_can_mem_track = criu_check_mem_track (err);
        यदि (UNLIKELY (criu_can_mem_track == -1))
          प्रत्यागम -1;
        libcriu_wrapper->criu_समुच्चयः_track_mem (true);

        /* The parent path must be relative to image path (something like ../previous-dump).
           CRIU will fail with an unclear error message if the path is not right.
         */
        यदि (UNLIKELY (cr_options->parent_मार्गः[0] == '/'))
          प्रत्यागम crun_make_error (err, 0, "--parent-path must be relative");
        पूर्णाङ्क is_dir = crun_dir_p_at (image_fd, cr_options->parent_मार्गः, false, err);
        यदि (UNLIKELY (is_dir <= 0))
          {
            यदि (is_dir < 0)
              प्रत्यागम crun_error_wrap (err, "invalid --parent-path");
            प्रत्यागम crun_make_error (err, ENOTDIR, "invalid --parent-path");
          }
        ret = libcriu_wrapper->criu_समुच्चयः_parent_images (cr_options->parent_मार्गः);
        यदि (UNLIKELY (ret != 0))
          प्रत्यागम crun_make_error (err, -ret, "error setting CRIU parent images path to `%s`", cr_options->parent_मार्गः);
      }

    यदि (cr_options->pre_dump)
      {
        यदि (criu_can_mem_track != 1)
          {
            criu_can_mem_track = criu_check_mem_track (err);
            यदि (UNLIKELY (criu_can_mem_track == -1))
              प्रत्यागम -1;
          }
        libcriu_wrapper->criu_समुच्चयः_track_mem (true);
        ret = libcriu_wrapper->criu_pre_dump ();
        यदि (UNLIKELY (ret != 0))
          {
            show_criu_log (cr_options->work_मार्गः, CRIU_CHECKPOINT_LOG_FILE);
            प्रत्यागम crun_make_error (err, 0, "CRIU pre-dump failed: %d", ret);
          }
        प्रत्यागम 0;
      }
  }
#  endif

  /* descriptors.json is needed during restore to correctly
   * reconnect stdin, stdout, stderr. */
  ret = append_paths (&descriptors_मार्गः, err, cr_options->image_मार्गः, DESCRIPTORS_FILENAME, रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = write_file (descriptors_मार्गः, स्थितिः->external_descriptors, safe_strlen (स्थितिः->external_descriptors), err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_error_wrap (err, "error saving CRIU descriptors file");

  ret = append_paths (&मार्गः, err, स्थितिः->bundle, स्थितिः->rootfs, रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = libcriu_wrapper->criu_समुच्चयः_मूलम् (मार्गः);
  यदि (UNLIKELY (ret != 0))
    प्रत्यागम crun_make_error (err, 0, "error setting CRIU root to `%s`", मार्गः);

  cgroup_बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
  यदि (UNLIKELY (cgroup_बहुलकम् < 0))
    प्रत्यागम cgroup_बहुलकम्;

  /* For cgroup v1 we need to tell CRIU to handle all cgroup mounts as external mounts. */
  यदि (cgroup_बहुलकम् != CGROUP_बहुलकम्_UNIFIED)
    {
      ret = checkpoint_cgroup_v1_mount (def, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  /* Tell CRIU about external bind mounts. */
  प्रति (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      यदि (is_bind_mount (def->mounts[i], रिक्त, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          वर्ण buf[PATH_MAX];
          ध्रुव वर्ण *dest_अन्तर्_मूलम्;

          यदि (nofollow)
            प्रत्यागम crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_अन्तर्_मूलम् = chroot_realpath (स्थितिः->rootfs, def->mounts[i]->destination, buf);
          यदि (UNLIKELY (dest_अन्तर्_मूलम् == रिक्त))
            प्रत्यागम crun_make_error (err, errno, "unable to resolve external bind mount `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          यदि (has_उपसर्गः (dest_अन्तर्_मूलम्, स्थितिः->rootfs))
            dest_अन्तर्_मूलम् += safe_strlen (स्थितिः->rootfs);

          ret = libcriu_wrapper->criu_सङ्कलय_ext_mount (dest_अन्तर्_मूलम्, dest_अन्तर्_मूलम्);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", def->mounts[i]->destination);
        }
    }

  ret = पञ्जिकाकोशः_masked_paths_mounts (def, container, libcriu_wrapper, false, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

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

  प्रति (i = 0; i < def->linux->namespaces_len; i++)
    {
      पूर्णाङ्क मानम् = libcrun_find_namespace (def->linux->namespaces[i]->type);
      यदि (UNLIKELY (मानम् < 0))
        प्रत्यागम crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      यदि (मानम् == CLONE_NEWNET && def->linux->namespaces[i]->मार्गः != रिक्त)
        {
          cleanup_free वर्ण *external = रिक्त;
          संरचना stat statbuf;

          ret = stat (def->linux->namespaces[i]->मार्गः, &statbuf);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "unable to stat(): `%s`", def->linux->namespaces[i]->मार्गः);

          xasprintf (&external, "net[%ld]:" CRIU_EXT_NETNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_सङ्कलय_external (external);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }

      यदि (मानम् == CLONE_NEWPID && def->linux->namespaces[i]->मार्गः != रिक्त)
        {
          cleanup_free वर्ण *external = रिक्त;
          संरचना stat statbuf;

          ret = stat (def->linux->namespaces[i]->मार्गः, &statbuf);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, errno, "unable to stat(): `%s`", def->linux->namespaces[i]->मार्गः);

          xasprintf (&external, "pid[%ld]:" CRIU_EXT_PIDNS, statbuf.st_ino);
          ret = libcriu_wrapper->criu_सङ्कलय_external (external);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", external);
        }
    }

  /* Tell CRIU to use the freezer to pause all container processes. */
  यदि (cgroup_बहुलकम् == CGROUP_बहुलकम्_UNIFIED)
    {
      /* This needs CRIU 3.14. */
      ret = append_paths (&freezer_मार्गः, err, CGROUP_मूलम्, स्थितिः->cgroup_मार्गः, रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }
  अन्यथा
    {
      ret = append_paths (&freezer_मार्गः, err, CGROUP_मूलम् "/freezer", स्थितिः->cgroup_मार्गः, रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  ret = libcriu_wrapper->criu_समुच्चयः_freeze_cgroup (freezer_मार्गः);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, -ret, "CRIU: failed setting freezer %d", ret);

  /* Set boolean options . */
  libcriu_wrapper->criu_समुच्चयः_leave_running (cr_options->leave_running);
  libcriu_wrapper->criu_समुच्चयः_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_समुच्चयः_shell_job (cr_options->shell_job);
  libcriu_wrapper->criu_समुच्चयः_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_समुच्चयः_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_समुच्चयः_orphan_pts_master (true);
  libcriu_wrapper->criu_समुच्चयः_manage_cgroups_बहुलकम् (criu_cg_बहुलकम्_from_लौकिकम् (cr_options->manage_cgroups_बहुलकम्));

  libcriu_wrapper->criu_समुच्चयः_manage_cgroups (true);

  यदि (libcriu_wrapper->criu_समुच्चयः_जालम्_तालकम्)
    {
      पूर्णाङ्क तालकम् = 0;

      ret = criu_जालम्_तालकम्_from_लौकिकम् (cr_options->जालम्_तालकम्_विधानम्, &तालकम्, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      यदि (तालकम् > 0)
        {
          ret = libcriu_wrapper->criu_समुच्चयः_जालम्_तालकम् (तालकम्);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, 0, "CRIU: failed setting network lock");
        }
    }

  ret = libcriu_wrapper->criu_dump ();
  यदि (UNLIKELY (ret != 0))
    {
      show_criu_log (cr_options->work_मार्गः, CRIU_CHECKPOINT_LOG_FILE);
      प्रत्यागम crun_make_error (err, ret < 0 ? -ret : 0, "CRIU checkpointing failed: %d", ret);
    }

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
prepare_restore_mounts_आन्तरिकम् (runtime_spec_schema_config_schema *def, वर्ण *मूलम्, ध्रुव वर्ण **mounted,
                                 size_t *n_mounted, libcrun_error_t *err)
{
  uint32_t i;

  /* Go through all mountpoints to be able to recreate missing mountpoints. */
  प्रति (i = 0; i < def->mounts_len; i++)
    {
      वर्ण *dest = def->mounts[i]->destination;
      वर्ण *type = def->mounts[i]->type;
      cleanup_close पूर्णाङ्क मूलम्_fd = -1;
      bool nofollow = false;
      bool on_tmpfs = false;
      पूर्णाङ्क is_dir = 1;
      size_t j;

      /* cgroup restore should be handled by CRIU itself */
      यदि (type && (वाक्यं_तुलय (type, "cgroup") == 0 || वाक्यं_तुलय (type, "cgroup2") == 0))
        अनुवर्त;

      /* Check if the mountpoint is on a tmpfs. CRIU restores
       * all tmpfs. We do need to recreate directories on a tmpfs. */
      size_t dest_len = वाक्यदैर्घ्यम् (dest);
      प्रति (j = 0; j < def->mounts_len; j++)
        {
          यदि (def->mounts[j]->type == रिक्त || वाक्यं_तुलय (def->mounts[j]->type, "tmpfs") != 0)
            अनुवर्त;
          size_t mount_len = वाक्यदैर्घ्यम् (def->mounts[j]->destination);
          यदि (mount_len < dest_len && dest[mount_len] == '/' && वाक्ये_तुलय (dest, def->mounts[j]->destination, mount_len) == 0)
            {
              /* This is a mountpoint which is on a tmpfs.*/
              on_tmpfs = true;
              भङ्ग;
            }
        }

      यदि (on_tmpfs)
        अनुवर्त;

      /* For bind mounts check if the source is a file or a directory. */
      यदि (is_bind_mount (def->mounts[i], रिक्त, &nofollow))
        {
          यदि (nofollow)
            प्रत्यागम crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          is_dir = crun_dir_p (def->mounts[i]->source, false, err);
          यदि (UNLIKELY (is_dir < 0))
            प्रत्यागम is_dir;
        }

      मूलम्_fd = उद्घाटय (मूलम्, O_RDONLY | O_CLOEXEC);
      यदि (UNLIKELY (मूलम्_fd == -1))
        प्रत्यागम crun_make_error (err, errno, "error opening container root directory `%s`", मूलम्);

      यदि (is_dir)
        {
          पूर्णाङ्क ret;

          ret = crun_safe_ensure_directory_at (मूलम्_fd, मूलम्, dest, 0755, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }
      अन्यथा
        {
          पूर्णाङ्क ret;

          ret = crun_safe_ensure_file_at (मूलम्_fd, मूलम्, dest, 0755, err);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम ret;
        }

      /* Mount a bind mount source now, so that the mountpoints of the
       * mounts inside it (e.g. a nested bind mount) are created in the
       * source, where CRIU expects them.  This also happens during the
       * initial container creation, as the mounts are done in order.  */
      यदि (is_bind_mount (def->mounts[i], रिक्त, &nofollow))
        {
          cleanup_close पूर्णाङ्क dst_fd = -1;
          proc_fd_मार्गः_t fd_मार्गः;

          dst_fd = safe_openat (मूलम्_fd, मूलम्, dest, O_मार्गः | O_CLOEXEC, 0, err);
          यदि (UNLIKELY (dst_fd < 0))
            प्रत्यागम dst_fd;

          /* Not recursive: the source might contain ROOT itself (e.g. when
           * it is the bundle directory), and it is not needed anyway.  */
          get_proc_self_fd_मार्गः (fd_मार्गः, dst_fd);
          यदि (UNLIKELY (mount (def->mounts[i]->source, fd_मार्गः, रिक्त, MS_BIND, रिक्त) < 0))
            प्रत्यागम crun_make_error (err, errno, "bind mount `%s` to `%s`", def->mounts[i]->source, dest);

          mounted[(*n_mounted)++] = dest;
        }
    }

  प्रत्यागम 0;
}

/* Move the current process back to CGROUPS, as read from /proc/self/cgroup.
   A failure is not fatal, so only warn about it.  */
स्थिर शून्य
move_पृष्ठभागः_गन्तव्यम्_cgroups (ध्रुव वर्ण *cgroups)
{
  libcrun_error_t tmp_err = रिक्त;

  यदि (UNLIKELY (libcrun_move_self_गन्तव्यम्_cgroups (cgroups, &tmp_err) < 0))
    {
      libcrun_warning ("cannot move back to the original cgroup: %s", tmp_err->सन्देशः);
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
स्थिर पूर्णाङ्क
criu_restore_child_अन्तर्_cgroup (ध्रुव वर्ण *cgroup_मार्गः, पूर्णाङ्क *criu_ret, libcrun_error_t *err)
{
  sigset_t all_सङ्केताः, old_आवरणम्;
  पूर्णाङ्क wait_स्थितिः = 0;
  pid_t pid;
  पूर्णाङ्क ret;

  *criu_ret = -1;

  sigfillset (&all_सङ्केताः);
  ret = sigprocmask (SIG_खण्डम्, &all_सङ्केताः, &old_आवरणम्);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "sigprocmask");

  /* Must be vfork: the child shares our memory space, so both *criu_ret and
     the error it creates are visible here once it is gone.  */
  pid = vfork ();
  यदि (UNLIKELY (pid < 0))
    {
      पूर्णाङ्क saved_errno = errno;

      sigprocmask (SIG_SETMASK, &old_आवरणम्, रिक्त);
      प्रत्यागम crun_make_error (err, saved_errno, "vfork");
    }

  यदि (pid == 0)
    {
      संरचना sigaction act;
      पूर्णाङ्क i;

      /* The signal dispositions are not shared with the parent, so resetting
         them here does not affect it.  Keep the ignored signals ignored, as
         CRIU would inherit them if it was run directly by the caller.  */
      प्रति (i = 1; i < NSIG; i++)
        {
          यदि (sigaction (i, रिक्त, &act) < 0 || act.sa_handler == SIG_IGN || act.sa_handler == SIG_DFL)
            अनुवर्त;

          स्मृतिं_पूरय (&act, 0, परिमाण (act));
          act.sa_handler = SIG_DFL;
          sigaction (i, &act, रिक्त);
        }
      sigprocmask (SIG_SETMASK, &old_आवरणम्, रिक्त);

      यदि (! is_रिक्तानि_string (cgroup_मार्गः))
        {
          ret = libcrun_move_process_गन्तव्यम्_cgroup (0, 0, cgroup_मार्गः, false, err);
          यदि (UNLIKELY (ret < 0))
            _safe_exit (EXIT_FAILURE);
        }

      *criu_ret = libcriu_wrapper->criu_restore_child ();
      _safe_exit (EXIT_SUCCESS);
    }

  /* The child is gone by now, it is safe to handle signals again.  */
  sigprocmask (SIG_SETMASK, &old_आवरणम्, रिक्त);

  ret = waitpid_ignore_stopped (pid, &wait_स्थितिः, 0);
  यदि (UNLIKELY (ret < 0))
    {
      /* The caller might have reaped the child already, e.g. when it ignores
         SIGCHLD.  Its result is still visible, so use it.  */
      यदि (errno != ECHILD)
        प्रत्यागम crun_make_error (err, errno, "waitpid for the CRIU restore process");

      /* The child might have failed before calling CRIU, and left an error.  */
      प्रत्यागम *err != रिक्त ? -1 : 0;
    }

  ret = get_process_exit_स्थितिः (wait_स्थितिः);
  यदि (UNLIKELY (ret != EXIT_SUCCESS))
    {
      /* The child creates the error in the shared memory space, do not
         overwrite it with crun_make_error().  */
      यदि (*err == रिक्त)
        प्रत्यागम crun_make_error (err, 0, "the CRIU restore process exited with status %d", ret);
      प्रत्यागम -1;
    }

  प्रत्यागम 0;
}

/* Recreate the mountpoints which do not exist in the container rootfs
 * mounted at ROOT, the same way it is done on the container creation.  */
स्थिर पूर्णाङ्क
prepare_restore_mounts (runtime_spec_schema_config_schema *def, वर्ण *मूलम्, libcrun_error_t *err)
{
  cleanup_free ध्रुव वर्ण **mounted = xmalloc0 (परिमाण (वर्ण *) * (def->mounts_len + 1));
  cleanup_close पूर्णाङ्क मूलम्_fd = -1;
  size_t n_mounted = 0;
  पूर्णाङ्क ret;

  ret = prepare_restore_mounts_आन्तरिकम् (def, मूलम्, mounted, &n_mounted, err);

  /* The bind mounts done above are only needed to create the mountpoints,
   * and CRIU restores the mounts itself, so undo them, in reverse order. */
  यदि (n_mounted > 0)
    मूलम्_fd = उद्घाटय (मूलम्, O_मार्गः | O_CLOEXEC);
  यावत् (मूलम्_fd >= 0 && n_mounted > 0)
    {
      libcrun_error_t tmp_err = रिक्त;
      ध्रुव वर्ण *dest = mounted[--n_mounted];
      cleanup_close पूर्णाङ्क dst_fd = -1;
      proc_fd_मार्गः_t fd_मार्गः;

      dst_fd = safe_openat (मूलम्_fd, मूलम्, dest, O_मार्गः | O_CLOEXEC, 0, &tmp_err);
      यदि (UNLIKELY (dst_fd < 0))
        {
          crun_error_release (&tmp_err);
          अनुवर्त;
        }
      get_proc_self_fd_मार्गः (fd_मार्गः, dst_fd);
      यदि (UNLIKELY (umount2 (fd_मार्गः, MNT_DETACH) < 0 && ret >= 0))
        ret = crun_make_error (err, errno, "unmount `%s`", dest);
    }

  प्रत्यागम ret;
}

पूर्णाङ्क
libcrun_container_restore_linux_criu (libcrun_container_स्थितिः_t *स्थितिः, libcrun_container_t *container,
                                      libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_wrapper संरचना libcriu_wrapper_s *wrapper = रिक्त;
  cleanup_close पूर्णाङ्क inherit_new_net_fd = -1;
  cleanup_close पूर्णाङ्क inherit_new_pid_fd = -1;
  cleanup_close पूर्णाङ्क image_fd = -1;
  cleanup_free वर्ण *मूलम् = रिक्त;
  cleanup_free वर्ण *bundle_cleanup = रिक्त;
  cleanup_free वर्ण *own_cgroups = रिक्त;
  cleanup_close पूर्णाङ्क work_fd = -1;
  पूर्णाङ्क cgroup_बहुलकम्;
  पूर्णाङ्क criu_ret;
  पूर्णाङ्क ret_बहिर्गमनम्;
  size_t i;
  पूर्णाङ्क ret;

  ret = आरोपय_wrapper (&wrapper, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (geteuid ())
    प्रत्यागम crun_make_error (err, 0, "restoring requires root");

  ret = libcriu_wrapper->criu_init_opts ();
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, 0, "CRIU init failed with %d", ret);

  ret = validate_criu_संस्करणम् (err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (UNLIKELY (cr_options->image_मार्गः == रिक्त))
    प्रत्यागम crun_make_error (err, 0, "image path not set");

  image_fd = उद्घाटय (cr_options->image_मार्गः, O_DIRECTORY | O_CLOEXEC);
  यदि (UNLIKELY (image_fd == -1))
    प्रत्यागम crun_make_error (err, errno, "error opening checkpoint directory `%s`", cr_options->image_मार्गः);

  libcriu_wrapper->criu_समुच्चयः_images_dir_fd (image_fd);

  /* Load descriptors.json to tell CRIU where those FDs should be connected to. */
  {
    cleanup_free वर्ण *descriptors_मार्गः = रिक्त;
    cleanup_free वर्ण *बफर = रिक्त;
    json_object *doc = रिक्त;

    ret = append_paths (&descriptors_मार्गः, err, cr_options->image_मार्गः, DESCRIPTORS_FILENAME, रिक्त);
    यदि (UNLIKELY (ret < 0))
      प्रत्यागम ret;

    ret = read_all_file (descriptors_मार्गः, &बफर, रिक्त, err);
    यदि (UNLIKELY (ret < 0))
      प्रत्यागम ret;

    /* descriptors.json contains a JSON array with strings
     * telling where 0, 1 and 2 have been initially been
     * pointing to. For each descriptor which points to
     * a pipe 'pipe:' we tell CRIU to reconnect that pipe
     * to the corresponding FD to have (especially) stdout
     * and stderr being correctly redirected. */
    ret = parse_json_file (&doc, बफर, रिक्त, err);
    यदि (UNLIKELY (ret < 0))
      प्रत्यागम ret;

    यदि (json_object_is_type (doc, json_type_array))
      {
        size_t i, len = json_object_array_दीर्घता (doc);

        /* len will probably always be 3 as crun is currently only
         * recording the destination of FD 0, 1 and 2. */
        प्रति (i = 0; i < len; ++i)
          {
            json_object *s = json_object_array_get_idx (doc, i);
            यदि (s && json_object_is_type (s, json_type_string))
              {
                ध्रुव वर्ण *str = json_object_get_string (s);
                यदि (has_उपसर्गः (str, "pipe:"))
                  libcriu_wrapper->criu_सङ्कलय_inherit_fd (i, str);
              }
          }
      }
    json_object_put (doc);
  }

  /* Our std streams are the new external descriptors, as CRIU connects the
   * inherited pipes to them.  Record them now: once the container is
   * restored it is already running, and /proc/PID/fd is under its control. */
  ret = libcrun_save_external_descriptors (container, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = सज्जता_criu_work_dir (cr_options, &work_fd, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (cr_options->lsm_profile != रिक्त)
    {
      ret = libcriu_wrapper->criu_समुच्चयः_lsm_profile (cr_options->lsm_profile);
      यदि (UNLIKELY (ret != 0))
        प्रत्यागम crun_make_error (err, -ret, "error setting LSM profile to `%s`", cr_options->lsm_profile);
    }

  यदि (cr_options->lsm_mount_context != रिक्त)
    {
      ret = libcriu_wrapper->criu_समुच्चयः_lsm_mount_context (cr_options->lsm_mount_context);
      यदि (UNLIKELY (ret != 0))
        प्रत्यागम crun_make_error (err, -ret, "error setting LSM mount context to `%s`", cr_options->lsm_mount_context);
    }

  /* do realpath on root */
  bundle_cleanup = realpath (स्थितिः->bundle, रिक्त);
  यदि (UNLIKELY (bundle_cleanup == रिक्त))
    bundle_cleanup = xstrdup (स्थितिः->bundle);

  /* Tell CRIU about external bind mounts. */
  प्रति (i = 0; i < def->mounts_len; i++)
    {
      bool nofollow = false;
      यदि (is_bind_mount (def->mounts[i], रिक्त, &nofollow))
        {
          /* We need to resolve mount destination inside container's root for CRIU to handle. */
          वर्ण buf[PATH_MAX];
          ध्रुव वर्ण *dest_अन्तर्_मूलम्;
          ध्रुव वर्ण *source = def->mounts[i]->source;
          cleanup_free वर्ण *abs_source = रिक्त;
          अचिह्नित दीर्घ flags = 0;
          size_t j;

          यदि (nofollow)
            प्रत्यागम crun_make_error (err, 0, "CRIU does not support `src-nofollow` for bind mounts");

          dest_अन्तर्_मूलम् = chroot_realpath (स्थितिः->rootfs, def->mounts[i]->destination, buf);
          यदि (UNLIKELY (dest_अन्तर्_मूलम् == रिक्त))
            प्रत्यागम crun_make_error (err, errno, "unable to resolve external bind mount destination `%s` under rootfs", def->mounts[i]->destination);

          /* When the rootfs is "/" or not set, chroot_realpath returns the
             path unchanged, so strip the prefix only when it is there.  */
          यदि (has_उपसर्गः (dest_अन्तर्_मूलम्, स्थितिः->rootfs))
            dest_अन्तर्_मूलम् += safe_strlen (स्थितिः->rootfs);

          /* A relative source is relative to the bundle, while CRIU would
             resolve it relative to its own working directory.  */
          यदि (source && source[0] != '/')
            {
              ret = append_paths (&abs_source, err, bundle_cleanup, source, रिक्त);
              यदि (UNLIKELY (ret < 0))
                प्रत्यागम ret;
              source = abs_source;
            }

          ret = libcriu_wrapper->criu_सङ्कलय_ext_mount (dest_अन्तर्_मूलम्, source);
          यदि (UNLIKELY (ret < 0))
            प्रत्यागम crun_make_error (err, -ret, "CRIU: failed adding external mount to `%s`", source);

          प्रति (j = 0; j < def->mounts[i]->options_len; j++)
            {
              ध्रुव संरचना सञ्चरणम्_flags_s *prop = रिक्त;

              यदि (def->mounts[i]->options[j])
                prop = libcrun_str2mount_flags (def->mounts[i]->options[j]);
              यदि (prop && ! prop->clear)
                flags |= prop->flags;
            }
          सङ्कलय_restored_ext_mount (dest_अन्तर्_मूलम्, flags);
        }
    }

  ret = पञ्जिकाकोशः_masked_paths_mounts (def, container, libcriu_wrapper, true, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  /* Mount the container rootfs for CRIU. */
  ret = append_paths (&मूलम्, err, bundle_cleanup, "criu-root", रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = mkdir (मूलम्, 0755);
  यदि (UNLIKELY (ret == -1))
    प्रत्यागम crun_make_error (err, errno, "error creating restore directory `%s`", मूलम्);

  ret = mount (स्थितिः->rootfs, मूलम्, रिक्त, MS_BIND | MS_REC, रिक्त);
  यदि (UNLIKELY (ret == -1))
    {
      ret = crun_make_error (err, errno, "error mounting restore directory `%s`", मूलम्);
      गच्छ बहिर्गमनम्;
    }

  /* During initial container creation, crun will create mountpoints
   * defined in config.json if they do not exist. If we are restoring
   * we need to make sure these mountpoints also exist.
   * This is not perfect, as this means crun will modify a rootfs
   * even if it marked as read-only, but runc already modifies
   * the rootfs in the same way. */

  ret = prepare_restore_mounts (def, मूलम्, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ बहिर्गमनम्_umount;

  ret = libcriu_wrapper->criu_समुच्चयः_मूलम् (मूलम्);
  यदि (UNLIKELY (ret != 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU root to `%s`", मूलम्);
      गच्छ बहिर्गमनम्_umount;
    }

  /* If a namespace defined in config.json we are telling
   * CRIU use that namespace when restoring the process tree.
   *
   * CRIU expects the information about the namespace like this:
   * --inherit-fd fd[<fd>]:<key>
   * The <key> needs to be the same as during checkpointing (extRootNetNS). */
  प्रति (i = 0; i < def->linux->namespaces_len; i++)
    {
      ध्रुव पूर्णाङ्क open_flags_for_inherit = O_RDONLY; /* Cannot be O_CLOEXEC as it is passed to the child process. */
      पूर्णाङ्क मानम् = libcrun_find_namespace (def->linux->namespaces[i]->type);
      यदि (UNLIKELY (मानम् < 0))
        {
          ret = crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);
          गच्छ बहिर्गमनम्_umount;
        }

      यदि (मानम् == CLONE_NEWNET && def->linux->namespaces[i]->मार्गः != रिक्त)
        {
          inherit_new_net_fd = उद्घाटय (def->linux->namespaces[i]->मार्गः, open_flags_for_inherit);
          यदि (UNLIKELY (inherit_new_net_fd < 0))
            {
              ret = crun_make_error (err, errno, "unable to open(): `%s`", def->linux->namespaces[i]->मार्गः);
              गच्छ बहिर्गमनम्_umount;
            }

          ret = libcriu_wrapper->criu_सङ्कलय_inherit_fd (inherit_new_net_fd, CRIU_EXT_NETNS);
          यदि (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              गच्छ बहिर्गमनम्_umount;
            }
        }

      यदि (मानम् == CLONE_NEWPID && def->linux->namespaces[i]->मार्गः != रिक्त)
        {
          inherit_new_pid_fd = उद्घाटय (def->linux->namespaces[i]->मार्गः, open_flags_for_inherit);
          यदि (UNLIKELY (inherit_new_pid_fd < 0))
            {
              ret = crun_make_error (err, errno, "unable to open(): `%s`", def->linux->namespaces[i]->मार्गः);
              गच्छ बहिर्गमनम्_umount;
            }

          ret = libcriu_wrapper->criu_सङ्कलय_inherit_fd (inherit_new_pid_fd, CRIU_EXT_PIDNS);
          यदि (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding fd");
              गच्छ बहिर्गमनम्_umount;
            }
        }

#  ifdef CRIU_JOIN_NS_SUPPORT
      यदि (मानम् == CLONE_NEWTIME && def->linux->namespaces[i]->मार्गः != रिक्त)
        {
          यदि (libcriu_wrapper->criu_join_ns_सङ्कलय == रिक्त)
            {
              ret = crun_make_error (err, 0, "shared time namespace restore is supported in CRIU >= 3.16.1");
              गच्छ बहिर्गमनम्_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_सङ्कलय ("time", def->linux->namespaces[i]->मार्गः, रिक्त);
          यदि (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->मार्गः);
              गच्छ बहिर्गमनम्_umount;
            }
        }

      यदि (मानम् == CLONE_NEWIPC && def->linux->namespaces[i]->मार्गः != रिक्त)
        {
          यदि (libcriu_wrapper->criu_join_ns_सङ्कलय == रिक्त)
            {
              ret = crun_make_error (err, 0, "shared ipc namespace restore is supported in CRIU >= 3.16.1");
              गच्छ बहिर्गमनम्_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_सङ्कलय ("ipc", def->linux->namespaces[i]->मार्गः, रिक्त);
          यदि (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->मार्गः);
              गच्छ बहिर्गमनम्_umount;
            }
        }

      यदि (मानम् == CLONE_NEWUTS && def->linux->namespaces[i]->मार्गः != रिक्त)
        {
          यदि (libcriu_wrapper->criu_join_ns_सङ्कलय == रिक्त)
            {
              ret = crun_make_error (err, 0, "shared uts namespace restore is supported in CRIU >= 3.16.1");
              गच्छ बहिर्गमनम्_umount;
            }

          ret = libcriu_wrapper->criu_join_ns_सङ्कलय ("uts", def->linux->namespaces[i]->मार्गः, रिक्त);
          यदि (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, -ret, "CRIU: failed adding external namespace `%s`", def->linux->namespaces[i]->मार्गः);
              गच्छ बहिर्गमनम्_umount;
            }
        }
#  endif
    }

  /* Set up CRIU config file */
  ret = handle_criu_config_file (container, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ बहिर्गमनम्_umount;

  /* Tell CRIU if cgroup v1 needs to be handled. */
  ret = restore_cgroup_v1_mount (def, err);
  यदि (UNLIKELY (ret < 0))
    गच्छ बहिर्गमनम्_umount;

  console_socket = cr_options->console_socket;
  libcriu_wrapper->criu_समुच्चयः_notify_cb (criu_notify);

  /* Set boolean options . */
  libcriu_wrapper->criu_समुच्चयः_ext_unix_sk (cr_options->ext_unix_sk);
  libcriu_wrapper->criu_समुच्चयः_shell_job (cr_options->shell_job);
  libcriu_wrapper->criu_समुच्चयः_tcp_established (cr_options->tcp_established);
  libcriu_wrapper->criu_समुच्चयः_tcp_close (cr_options->tcp_close);
  libcriu_wrapper->criu_समुच्चयः_file_locks (cr_options->file_locks);
  libcriu_wrapper->criu_समुच्चयः_orphan_pts_master (true);

  यदि (स्थितिः->cgroup_मार्गः)
    {
      cgroup_बहुलकम् = libcrun_get_cgroup_बहुलकम् (err);
      यदि (UNLIKELY (cgroup_बहुलकम् < 0))
        {
          ret = cgroup_बहुलकम्;
          गच्छ बहिर्गमनम्_umount;
        }

      /* With cgroup v2, the cgroup path is only meaningful for the unified
         hierarchy, so only relocate that one.  A NULL controller would make
         CRIU relocate every hierarchy the container is in, named cgroup v1
         ones included.  As CRIU dumps such a hierarchy from wherever the
         container is in it, which is its root, as crun leaves named
         hierarchies alone, every checkpoint and restore would then copy the
         whole hierarchy into itself.  */
      ret = libcriu_wrapper->criu_सङ्कलय_cg_मूलम् (cgroup_बहुलकम् == CGROUP_बहुलकम्_UNIFIED ? "" : रिक्त,
                                               स्थितिः->cgroup_मार्गः);
      यदि (UNLIKELY (ret != 0))
        {
          ret = crun_make_error (err, 0, "error setting CRIU cgroup root to `%s`", स्थितिः->cgroup_मार्गः);
          गच्छ बहिर्गमनम्_umount;
        }
    }

  libcriu_wrapper->criu_समुच्चयः_manage_cgroups_बहुलकम् (criu_cg_बहुलकम्_from_लौकिकम् (cr_options->manage_cgroups_बहुलकम्));
  libcriu_wrapper->criu_समुच्चयः_manage_cgroups (true);

  यदि (libcriu_wrapper->criu_समुच्चयः_जालम्_तालकम्)
    {
      पूर्णाङ्क तालकम् = 0;

      ret = criu_जालम्_तालकम्_from_लौकिकम् (cr_options->जालम्_तालकम्_विधानम्, &तालकम्, err);
      यदि (UNLIKELY (ret < 0))
        गच्छ बहिर्गमनम्_umount;

      यदि (तालकम् > 0)
        {
          ret = libcriu_wrapper->criu_समुच्चयः_जालम्_तालकम् (तालकम्);
          यदि (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "CRIU: failed setting network lock");
              गच्छ बहिर्गमनम्_umount;
            }
        }
    }

  libcriu_wrapper->criu_समुच्चयः_log_level (4);
  ret = libcriu_wrapper->criu_समुच्चयः_log_file (CRIU_RESTORE_LOG_FILE);
  यदि (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "error setting CRIU log file to `%s`", CRIU_RESTORE_LOG_FILE);
      गच्छ बहिर्गमनम्_umount;
    }

  /* criu_restore() returns the PID of the process of the restored process
   * tree. This PID will not be the same as status->pid if the container is
   * running in a PID namespace. But it will always be > 0. */
  यदि (cr_options->detach)
    {
      /* Nothing waits for the container, so there is no reason to take over
         this process: use a throw-away one.  It is used even when there is no
         cgroup to join, so that the container is not left as a child of a
         process which is not going to reap it.  */
      ret = criu_restore_child_अन्तर्_cgroup (स्थितिः->cgroup_मार्गः, &criu_ret, err);
      यदि (UNLIKELY (ret < 0))
        गच्छ बहिर्गमनम्_umount;

      ret = criu_ret;
    }
  अन्यथा
    {
      /* Like runc does, put CRIU into the container cgroup for the time of
       * restore, so the restored tasks are created there. It is necessary in
       * the "ignore" mode, where CRIU does not deal with cgroups at all, and
       * does not hurt otherwise. As CRIU is our child, do it by moving
       * ourselves there, and move back once the restore is done. */
      यदि (! is_रिक्तानि_string (स्थितिः->cgroup_मार्गः))
        {
          ret = read_all_file (PROC_SELF_CGROUP, &own_cgroups, रिक्त, err);
          यदि (UNLIKELY (ret < 0))
            गच्छ बहिर्गमनम्_umount;

          ret = libcrun_move_process_गन्तव्यम्_cgroup (0, 0, स्थितिः->cgroup_मार्गः, false, err);
          यदि (UNLIKELY (ret < 0))
            {
              /* Some of the cgroups might have been joined already.  */
              move_पृष्ठभागः_गन्तव्यम्_cgroups (own_cgroups);
              गच्छ बहिर्गमनम्_umount;
            }
        }

      ret = libcriu_wrapper->criu_restore_child ();

      यदि (own_cgroups)
        move_पृष्ठभागः_गन्तव्यम्_cgroups (own_cgroups);
    }

  यदि (UNLIKELY (ret <= 0))
    {
      show_criu_log (cr_options->work_मार्गः, CRIU_RESTORE_LOG_FILE);
      ret = crun_make_error (err, 0, "CRIU restoring failed: %d", ret);
      गच्छ बहिर्गमनम्_umount;
    }

  /* Update the status struct with the newly allocated PID. This will
   * be necessary later when moving the process into its cgroup. */
  स्थितिः->pid = ret;
  ret = 0;

बहिर्गमनम्_umount:
  ret_बहिर्गमनम् = umount (मूलम्);
  यदि (UNLIKELY (ret_बहिर्गमनम् == -1))
    {
      पूर्णाङ्क saved_errno = errno;
      rmdir (मूलम्);
      यदि (ret < 0)
        प्रत्यागम crun_error_wrap (err, "error unmounting restore directory `%s`", मूलम्);
      अन्यथा
        प्रत्यागम crun_make_error (err, saved_errno, "error unmounting restore directory `%s`", मूलम्);
    }
बहिर्गमनम्:
  ret_बहिर्गमनम् = rmdir (मूलम्);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;
  यदि (UNLIKELY (ret_बहिर्गमनम् == -1))
    प्रत्यागम crun_make_error (err, errno, "error removing restore directory `%s`", मूलम्);
  प्रत्यागम ret;
}
#यदि_अन्तः
