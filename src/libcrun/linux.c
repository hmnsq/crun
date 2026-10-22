/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019 Giuseppe Scrivano <giuseppe@scrivano.org>
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

#ضمن <config.h>
#ضمن "linux.h"
#ضمن "utils.h"
#ضمن "status.h"
#ضمن <string.h>
#ضمن <sched.h>
#ضمن <fcntl.h>
#ضمن <unistd.h>
#ضمن <sys/mount.h>
#ifdef HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#endif
#إذا defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#endif

#ضمن <sys/prctl.h>
#ifdef HAVE_CAP
#  include <sys/capability.h>
#endif
#ضمن <sys/time.h>
#ضمن <sys/resource.h>
#ضمن <sys/sysmacros.h>
#ضمن <sys/types.h>
#ضمن <sys/stat.h>
#ضمن <grp.h>
#ضمن <signal.h>
#ضمن "terminal.h"
#ضمن "cgroup.h"
#ضمن "cgroup-utils.h"
#ضمن "status.h"
#ضمن "criu.h"
#ضمن "scheduler.h"
#ضمن "intelrdt.h"
#ضمن "io_priority.h"
#ضمن "net_device.h"

#ضمن <sys/socket.h>
#ضمن <libgen.h>
#ضمن <sys/wait.h>
#ضمن <sys/vfs.h>
#ضمن <sys/statvfs.h>
#ضمن <limits.h>
#ضمن <inttypes.h>
#ضمن <sys/personality.h>
#ضمن <net/if.h>
#ضمن <sys/xattr.h>
#ضمن <linux/netlink.h>
#ضمن <linux/rtnetlink.h>
#ضمن <sched.h>
#ضمن <linux/sched.h>
#ضمن <linux/magic.h>
#ضمن <linux/nsfs.h>

#ضمن <json-c/json.h>
#ضمن <ocispec/json_common.h>

#ضمن "mount_flags.h"
#ضمن "syscalls.h"

#ifndef RLIMIT_RTTIME
#  define RLIMIT_RTTIME 15
#endif

#ifndef OPEN_TREE_CLONE
#  define OPEN_TREE_CLONE 1
#endif

#ifndef OPEN_TREE_CLOEXEC
#  define OPEN_TREE_CLOEXEC O_CLOEXEC
#endif

#ifndef OPEN_شجرة_NAMESPACE
#  define OPEN_شجرة_NAMESPACE 2
#endif

#ifndef MOVE_MOUNT_F_EMPTY_PATH
#  define MOVE_MOUNT_F_EMPTY_PATH 0x00000004
#endif

#ifndef MOVE_MOUNT_T_EMPTY_PATH
#  define MOVE_MOUNT_T_EMPTY_PATH 0x00000040
#endif

#ifndef FSOPEN_CLOEXEC
#  define FSOPEN_CLOEXEC 0x00000001
#endif

#ifndef FSMOUNT_CLOEXEC
#  define FSMOUNT_CLOEXEC 0x00000001
#endif

#ifndef FSCONFIG_CMD_CREATE
#  define FSCONFIG_CMD_CREATE 6
#endif

#define ALL_PROPAGATIONS_NO_REC (MS_SHARED | MS_PRIVATE | MS_SLAVE | MS_UNBINDABLE)
#define ALL_PROPAGATIONS (MS_REC | ALL_PROPAGATIONS_NO_REC)

/* The per-mount flags a remount clears unless they are explicitly set.  */
#define REMOUNT_CLEAR_FLAGS (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW)

#ifndef ST_NOSYMFOLLOW
#  define ST_NOSYMFOLLOW 0x2000
#endif

#define MOUNT_ATIME_FLAGS (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME)

/* Convert the ST_ flags reported by statfs(2) to the MS_ flags of mount(2).  */
ساكن unsigned long
mount_flags_from_statfs (unsigned long f_flags)
{
  unsigned long flags = 0;

  إذا (f_flags & ST_RDONLY)
    flags |= MS_RDONLY;
  إذا (f_flags & ST_NOSUID)
    flags |= MS_NOSUID;
  إذا (f_flags & ST_NODEV)
    flags |= MS_NODEV;
  إذا (f_flags & ST_NOEXEC)
    flags |= MS_NOEXEC;
  إذا (f_flags & ST_NOSYMFOLLOW)
    flags |= MS_NOSYMFOLLOW;
  إذا (f_flags & ST_NOATIME)
    flags |= MS_NOATIME;
  إذا (f_flags & ST_NODIRATIME)
    flags |= MS_NODIRATIME;
  إذا (f_flags & ST_RELATIME)
    flags |= MS_RELATIME;
  /* There is no ST_ flag for strictatime: it is the mode when neither noatime
     nor relatime is set.  */
  إذا (! (flags & (MS_NOATIME | MS_RELATIME)))
    flags |= MS_STRICTATIME;

  إرجاع flags;
}

بنية remount_s
{
  بنية remount_s *next;
  حرف *target;
  صحيح targetfd;
  unsigned long flags;
  حرف *data;
};

بنية private_data_s
{
  بنية remount_s *remounts;

  /* Filled by libcrun_run_linux_container().  Useful to query what
     namespaces are available.  */
  صحيح unshare_flags;
  صحيح unshare_cgroupns;

  حرف *host_notify_socket_path;
  حرف *container_notify_socket_path;
  حرف *unified_cgroup_path;

  bool mount_dev_from_host;
  unsigned long rootfs_propagation;
  bool deny_setgroups;

  صحيح procfd;

  ثابت حرف *rootfs;
  صحيح rootfsfd;

  صحيح notify_socket_شجرة_fd;
  صحيح old_جذر_fd;

  بنية libcrun_fd_map *mount_fds;
  بنية libcrun_fd_map *dev_fds;
  بنية libcrun_fd_map *needed_devs_fds;

  حرف **copy_symlink_targets;
  size_t n_copy_symlink_targets;

  /* Used to save stdin, stdout, stderr during checkpointing to descriptors.json
   * and needed during restore. */
  حرف *external_descriptors;

  /* Cached shared empty directory for masked paths optimization */
  صحيح maskdir_fd;
  حرف *maskdir_proc_path;
  bool maskdir_bind_failed;
  bool maskdir_move_failed;
  bool maskdir_warned;
  bool joined_mount_ns;
  bool needs_pivot;
  bool no_pivot;
  /* Set once the process root is the container rootfs and the host root
     can no longer be reached by path.  */
  bool host_جذر_switched;
  /* The process joining an existing PID namespace is created only once
     the container root is in place.  */
  bool delayed_pidns_fork;
  /* The rootfs path on the host, used to recognize bind mount sources
     inside the container rootfs.  */
  حرف *host_rootfs;
};

بنية linux_namespace_s
{
  ثابت حرف *name;
  ثابت حرف *ns_file;
  صحيح قيمة;
};

ساكن فراغ
cleanup_private_data (فراغ *private_data)
{
  بنية private_data_s *p = private_data;

  إذا (p->rootfsfd >= 0)
    TEMP_FAILURE_RETRY (close (p->rootfsfd));
  إذا (p->procfd >= 0)
    TEMP_FAILURE_RETRY (close (p->procfd));
  إذا (p->maskdir_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->maskdir_fd));
  إذا (p->old_جذر_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->old_جذر_fd));
  إذا (p->mount_fds)
    cleanup_close_mapp (&(p->mount_fds));
  إذا (p->dev_fds)
    cleanup_close_mapp (&(p->dev_fds));
  إذا (p->needed_devs_fds)
    cleanup_close_mapp (&(p->needed_devs_fds));

  إذا (p->copy_symlink_targets)
    {
      size_t i;
      لكل (i = 0; i < p->n_copy_symlink_targets; i++)
        حرر (p->copy_symlink_targets[i]);
      حرر (p->copy_symlink_targets);
    }
  حرر (p->unified_cgroup_path);
  حرر (p->host_notify_socket_path);
  حرر (p->container_notify_socket_path);
  حرر (p->external_descriptors);
  حرر (p->maskdir_proc_path);
  حرر (p->host_rootfs);
  حرر (p);
}

ساكن بنية private_data_s *
get_private_data (بنية libcrun_container_s *container)
{
  إذا (container->private_data == عدم)
    {
      بنية private_data_s *p = xmalloc0 (حجم (*p));
      container->private_data = p;
      p->rootfsfd = -1;
      p->procfd = -1;
      p->notify_socket_شجرة_fd = -1;
      p->old_جذر_fd = -1;
      p->maskdir_fd = -1;
      container->cleanup_private_data = cleanup_private_data;
    }
  إرجاع container->private_data;
}

#ifndef CLONE_NEWTIME
#  define CLONE_NEWTIME 0x00000080
#endif
#ifndef CLONE_NEWCGROUP
#  define CLONE_NEWCGROUP 0x02000000
#endif
#ifndef AT_RECURSIVE
#  define AT_RECURSIVE 0x8000
#endif

ساكن بنية linux_namespace_s namespaces[] = { { "mount", "mnt", CLONE_NEWNS },
                                                 { "network", "net", CLONE_NEWNET },
                                                 { "ipc", "ipc", CLONE_NEWIPC },
                                                 { "pid", "pid", CLONE_NEWPID },
                                                 { "uts", "uts", CLONE_NEWUTS },
                                                 { "user", "user", CLONE_NEWUSER },
                                                 { "cgroup", "cgroup", CLONE_NEWCGROUP },
                                                 { "time", "time", CLONE_NEWTIME },
                                                 { عدم, عدم, 0 } };

ساكن صحيح
get_and_reset (صحيح *old)
{
  صحيح tmp = *old;
  *old = -1;
  إرجاع tmp;
}

صحيح
libcrun_find_namespace (ثابت حرف *name)
{
  بنية linux_namespace_s *it;
  لكل (it = namespaces; it->name; it++)
    إذا (strcmp (it->name, name) == 0)
      إرجاع it->قيمة;
  إرجاع -1;
}

ساكن صحيح
syscall_keyctl_join (ثابت حرف *name)
{
#define KEYCTL_JOIN_SESSION_KEYRING 0x1
  إرجاع (صحيح) syscall (__NR_keyctl, KEYCTL_JOIN_SESSION_KEYRING, name, 0);
}

ساكن صحيح
syscall_pidfd_open (pid_t pid, unsigned صحيح flags)
{
#إذا defined __NR_pidfd_open
  إرجاع (صحيح) syscall (__NR_pidfd_open, pid, flags);
#وإلا
  (فراغ) pid;
  (فراغ) flags;
  errno = ENOSYS;
  إرجاع -1;
#endif
}

ساكن صحيح
syscall_pidfd_send_signal (صحيح pidfd, صحيح sig, siginfo_t *info, unsigned صحيح flags)
{
#إذا defined __NR_pidfd_send_signal
  إرجاع (صحيح) syscall (__NR_pidfd_send_signal, pidfd, sig, info, flags);
#وإلا
  (فراغ) pidfd;
  (فراغ) sig;
  (فراغ) info;
  (فراغ) flags;
  errno = ENOSYS;
  إرجاع -1;
#endif
}

/* Translate MS_* mount flags to MOUNT_ATTR_* values for mount_setattr().  */
ساكن uint64_t
ms_flags_to_mount_attr (uint64_t ms_flags)
{
  uint64_t attr = 0;

  إذا (ms_flags & MS_RDONLY)
    attr |= MOUNT_ATTR_RDONLY;
  إذا (ms_flags & MS_NOSUID)
    attr |= MOUNT_ATTR_NOSUID;
  إذا (ms_flags & MS_NODEV)
    attr |= MOUNT_ATTR_NODEV;
  إذا (ms_flags & MS_NOEXEC)
    attr |= MOUNT_ATTR_NOEXEC;
  إذا (ms_flags & MS_NOATIME)
    attr |= MOUNT_ATTR_NOATIME;
  إذا (ms_flags & MS_STRICTATIME)
    attr |= MOUNT_ATTR_STRICTATIME;
  إذا (ms_flags & MS_NODIRATIME)
    attr |= MOUNT_ATTR_NODIRATIME;
  إذا (ms_flags & MS_NOSYMFOLLOW)
    attr |= MOUNT_ATTR_NOSYMFOLLOW;

  إرجاع attr;
}

ساكن صحيح
do_mount_setattr (bool recursive, ثابت حرف *target, صحيح targetfd, uint64_t clear, uint64_t set, libcrun_error_t *err)
{
  بنية mount_attr_s attr = {
    0,
  };
  صحيح ret;

  set &= ~MS_BIND;
  clear &= ~MS_BIND;

  attr.propagation = set & ALL_PROPAGATIONS_NO_REC;
  attr.attr_set = ms_flags_to_mount_attr (set & (~ALL_PROPAGATIONS));
  attr.attr_clr = ms_flags_to_mount_attr (clear & (~ALL_PROPAGATIONS));

  /* Like mount(2) does, if any atime flag is set, set all the atime ones
     anew: relatime is the default, noatime and strictatime override it
     (strictatime wins), and nodiratime is independent.  Otherwise, the
     atime flags of the mount are kept.  */
  إذا (set & (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME))
    {
      attr.attr_set &= ~(MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME);
      إذا (set & MS_STRICTATIME)
        attr.attr_set |= MOUNT_ATTR_STRICTATIME;
      وإلا إذا (set & MS_NOATIME)
        attr.attr_set |= MOUNT_ATTR_NOATIME;
      وإلا
        attr.attr_set |= MOUNT_ATTR_RELATIME;
      إذا (set & MS_NODIRATIME)
        attr.attr_set |= MOUNT_ATTR_NODIRATIME;
      attr.attr_clr |= MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME;
    }
  /* The kernel rejects a partial MOUNT_ATTR__ATIME in attr_clr, so clearing
     any of the atime modes resets the mount to the relatime default.  */
  وإلا إذا (clear & (MS_NOATIME | MS_RELATIME | MS_STRICTATIME))
    attr.attr_clr |= MOUNT_ATTR__ATIME;

  ret = syscall_mount_setattr (targetfd, "", (recursive ? AT_RECURSIVE : 0) | AT_EMPTY_PATH, &attr);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "mount_setattr `/%s`", target);

  إرجاع 0;
}

صحيح
get_bind_mount (صحيح dirfd, ثابت حرف *src, bool recursive, bool rdonly, bool nofollow, unsigned long propagation, libcrun_error_t *err)
{
  cleanup_close صحيح open_شجرة_fd = -1;
  بنية mount_attr_s attr = {
    0,
  };
  صحيح recursive_flag = (recursive ? AT_RECURSIVE : 0);
  صحيح ret;

  /* Several callers pass a source straight from the configuration, where it
     is optional, so it can be missing altogether.  Callers that mean the
     directory referred to by dirfd pass an empty string, never NULL.  */
  إذا (UNLIKELY (src == عدم))
    إرجاع crun_make_error (err, EINVAL, "mount source is not specified");

  إذا (rdonly)
    attr.attr_set = MS_RDONLY;

  attr.propagation = propagation;

  إذا (dirfd < 0)
    dirfd = AT_FDCWD;

  errno = 0;
  open_شجرة_fd = syscall_open_شجرة (dirfd, src,
                                    AT_NO_AUTOMOUNT | OPEN_TREE_CLOEXEC
                                        | OPEN_TREE_CLONE | recursive_flag
                                        | (src[0] == '\0' ? AT_EMPTY_PATH : 0)
                                        | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  إذا (UNLIKELY (open_شجرة_fd < 0))
    إرجاع crun_make_error (err, errno, "open_tree `%s`", src);

  ret = syscall_mount_setattr (open_شجرة_fd, "", AT_EMPTY_PATH | recursive_flag, &attr);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "mount_setattr `%s`", src);

  إرجاع get_and_reset (&open_شجرة_fd);
}

صحيح
parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, حرف *option, حرف **out, size_t *len, libcrun_error_t *err)
{
  size_t written = 0, allocated = 256;
  cleanup_free حرف *mappings = عدم;
  ثابت حرف *it;
  صحيح ret;

  mappings = xmalloc (allocated);

  لكل (it = option; *it;)
    {
      bool relative = false;
      long قيمة[3];
      size_t i;

      إذا (*it == '\0')
        اكسر;

      إذا (*it == '#')
        it++;

      إذا (*it == '@')
        {
          relative = true;
          it++;
        }

      /* read a triplet: file system id - host id - size.  */
      لكل (i = 0; i < 3; i++)
        {
          حرف *endptr = عدم;

          إذا (i > 0 && *it == '-')
            it++;

          إذا (*it == '\0')
            إرجاع crun_make_error (err, 0, "invalid mapping specified `%s`", option);

          errno = 0;
          قيمة[i] = strtol (it, &endptr, 10);
          إذا (errno || endptr == it)
            إرجاع crun_make_error (err, errno, "invalid mapping specified `%s`", option);

          it = endptr;
        }

      إذا (relative)
        {
          runtime_spec_schema_defs_id_mapping **mappings;
          size_t mappings_len;

          إذا (def == عدم
              || def->linux == عدم
              || (is_uids && def->linux->uid_mappings_len == 0)
              || (! is_uids && def->linux->gid_mappings_len == 0))
            إرجاع crun_make_error (err, 0, "specified a relative mapping without user namespace mappings");

          mappings_len = (is_uids ? def->linux->uid_mappings_len : def->linux->gid_mappings_len);
          mappings = is_uids ? def->linux->uid_mappings : def->linux->gid_mappings;

          لكل (i = 0; i < mappings_len; i++)
            إذا (قيمة[1] >= (int64_t) mappings[i]->container_id
                && قيمة[1] < (int64_t) mappings[i]->container_id + (int64_t) mappings[i]->size)
              اكسر;

          إذا (i == mappings_len)
            إرجاع crun_make_error (err, 0, "could not find a user namespace mapping for the relative mapping `%s`", option);

          قيمة[1] += mappings[i]->host_id - mappings[i]->container_id;
        }
      إذا (written > allocated - 64)
        {
          allocated += 256;
          mappings = xrealloc (mappings, allocated);
        }

      ret = snprintf (mappings + written, allocated - written, "%ld %ld %ld\n", قيمة[0], قيمة[1], قيمة[2]);
      إذا (UNLIKELY (ret >= (صحيح) (allocated - written)))
        إرجاع crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
    }
  *(mappings + written) = '\0';

  *len = written;

  *out = mappings;
  mappings = عدم;

  إرجاع 0;
}

ساكن صحيح
format_mount_mappings (حرف **out,
                       runtime_spec_schema_defs_id_mapping **mappings,
                       size_t mappings_len,
                       size_t *written,
                       libcrun_error_t *err)
{
  /* 64 is more than enough room to print 3 uint32.  */
  ثابت size_t max_len_mapping = 64;
  cleanup_free حرف *ret = عدم;
  size_t s;

  *written = 0;

  ret = xmalloc (max_len_mapping * mappings_len + 1);
  لكل (s = 0; s < mappings_len; s++)
    {
      size_t len;

      len = snprintf (ret + *written, max_len_mapping, "%" PRIu32 " %" PRIu32 " %" PRIu32 "\n",
                      mappings[s]->container_id,
                      mappings[s]->host_id,
                      mappings[s]->size);
      إذا (UNLIKELY (len >= max_len_mapping))
        إرجاع crun_make_error (err, 0, "internal error: allocated buffer too small");

      *written += len;
    }
  *out = ret;
  ret = عدم;
  إرجاع 0;
}

ساكن صحيح
format_mount_mapping (حرف **ret, uint32_t container_id, uint32_t host_id,
                      uint32_t size, size_t *written, libcrun_error_t *err)
{
  runtime_spec_schema_defs_id_mapping mapping = {
    .container_id = container_id,
    .host_id = host_id,
    .size = size,
  };
  runtime_spec_schema_defs_id_mapping *mappings[] = {
    &mapping,
    عدم,
  };

  إرجاع format_mount_mappings (ret, mappings, 1, written, err);
}

ساكن bool
has_same_mappings (runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt)
{
  size_t s;

  إذا (def->linux == عدم)
    إرجاع mnt->uid_mappings_len == 0 && mnt->gid_mappings_len == 0;

  إذا (mnt->uid_mappings_len != def->linux->uid_mappings_len)
    إرجاع false;

  إذا (mnt->gid_mappings_len != def->linux->gid_mappings_len)
    إرجاع false;

  لكل (s = 0; s < mnt->uid_mappings_len; s++)
    {
      إذا (mnt->uid_mappings[s]->container_id != def->linux->uid_mappings[s]->container_id)
        إرجاع false;
      إذا (mnt->uid_mappings[s]->host_id != def->linux->uid_mappings[s]->host_id)
        إرجاع false;
      إذا (mnt->uid_mappings[s]->size != def->linux->uid_mappings[s]->size)
        إرجاع false;
    }

  لكل (s = 0; s < mnt->gid_mappings_len; s++)
    {
      إذا (mnt->gid_mappings[s]->container_id != def->linux->gid_mappings[s]->container_id)
        إرجاع false;
      إذا (mnt->gid_mappings[s]->host_id != def->linux->gid_mappings[s]->host_id)
        إرجاع false;
      إذا (mnt->gid_mappings[s]->size != def->linux->gid_mappings[s]->size)
        إرجاع false;
    }

  إرجاع true;
}

ساكن pid_t
maybe_create_userns_for_idmapped_mount (libcrun_container_t *container,
                                        runtime_spec_schema_config_schema *def,
                                        runtime_spec_schema_defs_mount *mnt,
                                        ثابت حرف *options, pid_t *pid_out,
                                        libcrun_error_t *err)
{
  cleanup_pid pid_t pid = -1;
  bool need_new_userns = mnt->uid_mappings_len ? ! has_same_mappings (def, mnt) : options != عدم;

  إذا (! need_new_userns)
    إرجاع 0;

  pid = syscall_clone (CLONE_NEWUSER | SIGCHLD, عدم);
  إذا (UNLIKELY (pid < 0))
    إرجاع crun_make_error (err, errno, "clone");

  إذا (pid == 0)
    {
      prctl (PR_SET_PDEATHSIG, SIGKILL);
      بينما (1)
        pause ();
      _safe_exit (EXIT_SUCCESS);
    }

  إذا (mnt->uid_mappings_len)
    {
      cleanup_free حرف *uid_map = عدم;
      cleanup_free حرف *gid_map = عدم;
      size_t written = 0;
      صحيح ret;

      ret = format_mount_mappings (&uid_map, mnt->uid_mappings, mnt->uid_mappings_len, &written, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      cleanup_close صحيح fd = -1;

      fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
      إذا (UNLIKELY (fd < 0))
        إرجاع fd;

      ret = safe_write (fd, "uid_map", uid_map, written, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      ret = format_mount_mappings (&gid_map, mnt->gid_mappings, mnt->gid_mappings_len, &written, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      close_and_reset (&fd);

      fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
      إذا (UNLIKELY (fd < 0))
        إرجاع fd;

      ret = safe_write (fd, "gid_map", gid_map, written, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }
  وإلا
    {
      cleanup_free حرف *dup_options = عدم;
      حرف *option, *saveptr = عدم;

      إذا (! options)
        إرجاع crun_make_error (err, 0, "internal error: no mappings found");

      dup_options = xstrdup (options);

      /* If there are no OCI mappings specified, then parse the annotation.  */
      لكل (option = strtok_r (dup_options, ";", &saveptr); option; option = strtok_r (عدم, ";", &saveptr))
        {
          cleanup_free حرف *mappings = عدم;
          cleanup_close صحيح map_fd = -1;
          bool is_uids = false;
          size_t len = 0;
          صحيح ret;

          إذا (has_prefix (option, "uids="))
            {
              is_uids = true;
              map_fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
              إذا (UNLIKELY (map_fd < 0))
                إرجاع map_fd;
            }
          وإلا إذا (has_prefix (option, "gids="))
            {
              map_fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
              إذا (UNLIKELY (map_fd < 0))
                إرجاع map_fd;
            }
          وإلا
            إرجاع crun_make_error (err, 0, "invalid option `%s` specified", option);

          ret = parse_idmapped_mount_option (def, is_uids, option + 5 /* strlen ("uids="), strlen ("gids=")*/, &mappings, &len, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;

          ret = safe_write (map_fd, is_uids ? "uid_map" : "gid_map", mappings, len, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;
        }
    }

  *pid_out = pid;
  pid = -1;
  إرجاع 0;
}

صحيح
libcrun_create_keyring (libcrun_container_t *container, ثابت حرف *name, ثابت حرف *label, libcrun_error_t *err)
{
  cleanup_close صحيح labelfd = -1;
  bool label_set = false;
  صحيح ret;

  إذا (label)
    {
      labelfd = libcrun_open_proc_file (container, "self/attr/keycreate", O_WRONLY, err);
      إذا (UNLIKELY (labelfd < 0))
        {
          إذا (crun_error_get_errno (err) != ENOENT)
            إرجاع labelfd;

          crun_error_release (err);
        }

      إذا (labelfd >= 0)
        {
          ret = اكتب (labelfd, label, strlen (label));
          إذا (UNLIKELY (ret < 0))
            إرجاع crun_make_error (err, errno, "write to `/proc/self/attr/keycreate`");

          label_set = true;
        }
    }

  ret = syscall_keyctl_join (name);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (errno == ENOSYS)
        {
          libcrun_warning ("could not create a new keyring: keyctl_join is not supported");
          ret = 0;
          اذهب out;
        }
      ret = crun_make_error (err, errno, "join keyctl `%s`", name ? name : "");
      اذهب out;
    }

out:
  /* Best effort attempt to reset the SELinux label used for new keyrings.  */
  إذا (label_set && اكتب (labelfd, "", 0) < 0)
    {
      /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
    }
  إرجاع ret;
}

ساكن فراغ
get_uid_gid_from_def (runtime_spec_schema_config_schema *def, uid_t *uid, gid_t *gid)
{
  *uid = 0;
  *gid = 0;

  إذا (def->process && def->process->user)
    {
      إذا (def->process->user->uid)
        *uid = def->process->user->uid;
      إذا (def->process->user->gid)
        *gid = def->process->user->gid;
    }
}

ساكن unsigned long
get_mount_flags (ثابت حرف *name, صحيح current_flags, صحيح *found, unsigned long *extra_flags, uint64_t *rec_clear, uint64_t *rec_set)
{
  ثابت بنية propagation_flags_s *prop;

  prop = libcrun_str2mount_flags (name);

  إذا (found)
    *found = prop ? 1 : 0;

  إذا (prop == عدم)
    إرجاع 0;

  إذا (prop->extra_flags & OPTION_RECURSIVE)
    {
      إذا (rec_clear && prop->clear)
        *rec_clear |= prop->flags;

      إذا (rec_set && ! prop->clear)
        *rec_set |= prop->flags;
    }

  إذا (extra_flags)
    *extra_flags |= prop->extra_flags;

  إذا (prop->clear)
    إرجاع current_flags & ~prop->flags;

  إرجاع current_flags | prop->flags;
}

/* Whether any of the mount options sets or clears a mount flag, other
   than MS_BIND, MS_REC and the propagation ones.  */
ساكن bool
has_mount_flag_options (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  لكل (i = 0; i < mount->options_len; i++)
    {
      ثابت بنية propagation_flags_s *prop;

      إذا (mount->options[i] == عدم)
        استمر;

      prop = libcrun_str2mount_flags (mount->options[i]);
      إذا (prop && (prop->flags & ~(MS_BIND | ALL_PROPAGATIONS)))
        إرجاع true;
    }

  إرجاع false;
}

ساكن unsigned long
get_mount_flags_or_option (ثابت حرف *name, صحيح current_flags, unsigned long *extra_flags, حرف **option, uint64_t *rec_clear, uint64_t *rec_set)
{
  صحيح found;
  __attribute__ ((unused)) cleanup_free حرف *prev = عدم;
  unsigned long flags;

  /* A null element of the array of options in the configuration is parsed
     into a NULL, which is not an option to be honored.  */
  إذا (name == عدم)
    إرجاع 0;

  flags = get_mount_flags (name, current_flags, &found, extra_flags, rec_clear, rec_set);
  إذا (found)
    {
      /* MS_SYNCHRONOUS and MS_DIRSYNC cannot be set through fsmount()
         attr_flags.  Pass them also as data so that fsconfig(SET_FLAG)
         can set SB_SYNCHRONOUS / SB_DIRSYNC on the superblock.  */
      ثابت حرف *data_flag = عدم;
      إذا ((flags & MS_SYNCHRONOUS) && ! (current_flags & MS_SYNCHRONOUS))
        data_flag = "sync";
      وإلا إذا ((flags & MS_DIRSYNC) && ! (current_flags & MS_DIRSYNC))
        data_flag = "dirsync";

      إذا (data_flag)
        {
          prev = *option;
          إذا (*option && **option)
            xasprintf (option, "%s,%s", *option, data_flag);
          وإلا
            *option = xstrdup (data_flag);
        }
      إرجاع flags;
    }

  prev = *option;
  إذا (*option && **option)
    xasprintf (option, "%s,%s", *option, name);
  وإلا
    *option = xstrdup (name);

  إرجاع 0;
}

صحيح
pivot_جذر (ثابت حرف *new_جذر, ثابت حرف *put_old)
{
  إرجاع syscall (__NR_pivot_root, new_جذر, put_old);
}

ساكن فراغ
free_remount (بنية remount_s *r)
{
  إذا (r == عدم)
    إرجاع;
  إذا (r->targetfd >= 0)
    close (r->targetfd);
  حرر (r->data);
  حرر (r->target);
  حرر (r);
}

ساكن بنية remount_s *
make_remount (صحيح targetfd, ثابت حرف *target, unsigned long flags, ثابت حرف *data, بنية remount_s *next)
{
  بنية remount_s *ret = xmalloc (حجم (*ret));
  ret->target = xstrdup (target);
  ret->flags = flags;
  ret->data = data ? xstrdup (data) : عدم;
  ret->next = next;
  ret->targetfd = targetfd;
  إرجاع ret;
}

ساكن صحيح
do_remount (صحيح targetfd, ثابت حرف *target, unsigned long flags, ثابت حرف *data, libcrun_error_t *err)
{
  proc_fd_path_t target_buffer;
  صحيح ret;
  ثابت حرف *real_target = target;

  إذا (targetfd >= 0)
    {
      unsigned long set_flags = flags & ~(MS_REMOUNT | MS_BIND);
      unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;
      ret = do_mount_setattr (false, target, targetfd, clear_flags, flags & ~MS_REMOUNT, err);
      إذا (LIKELY (ret == 0))
        إرجاع 0;
      crun_error_release (err);

      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  /* Older kernels (seen on 4.18) fail with EINVAL if data is set when
     setting MS_RDONLY.  */
  إذا (flags & (MS_REMOUNT | MS_RDONLY))
    data = عدم;

  ret = mount (عدم, real_target, عدم, flags, data);
  إذا (UNLIKELY (ret < 0))
    {
      unsigned long current_flags, remount_flags;
      بنية statfs sfs;

      ret = statfs (real_target, &sfs);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "statfs `%s`", real_target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);

      remount_flags = current_flags & (MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW);
      إذا (! (flags & MOUNT_ATIME_FLAGS))
        remount_flags |= current_flags & MOUNT_ATIME_FLAGS;

      إذا ((flags | remount_flags) != flags)
        {
          ret = mount (عدم, real_target, عدم, flags | remount_flags, data);
          إذا (LIKELY (ret == 0))
            إرجاع 0;

          /* If it still fails and MS_RDONLY is present in the mount, try adding it.  */
          إذا (current_flags & MS_RDONLY)
            ret = mount (عدم, real_target, عدم, flags | remount_flags | MS_RDONLY, data);
        }
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "remount `%s`", target);
    }
  إرجاع 0;
}

ساكن صحيح
finalize_mounts (libcrun_container_t *container, libcrun_error_t *err)
{
  صحيح ret = 0;
  بنية remount_s *r = get_private_data (container)->remounts;
  بينما (r)
    {
      بنية remount_s *next = r->next;

      /* Try mount_setattr() first to avoid the statfs+retry fallback.  */
      إذا (r->targetfd >= 0 && (r->flags & MS_RDONLY))
        {
          /* Clear the flags which are not set, like do_remount does.  */
          unsigned long set_flags = r->flags & ~(MS_REMOUNT | MS_BIND);
          unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;

          ret = do_mount_setattr (false, r->target, r->targetfd, clear_flags, r->flags & ~MS_REMOUNT, err);
          إذا (LIKELY (ret == 0))
            {
              free_remount (r);
              r = next;
              استمر;
            }
          crun_error_release (err);
        }

      ret = do_remount (r->targetfd, r->target, r->flags, r->data, err);
      إذا (UNLIKELY (ret < 0))
        اذهب cleanup;

      free_remount (r);
      r = next;
    }

cleanup:
  بينما (r)
    {
      بنية remount_s *next = r->next;
      free_remount (r);
      r = next;
    }

  get_private_data (container)->remounts = عدم;
  إرجاع ret;
}

ساكن صحيح
open_mount_target (libcrun_container_t *container, ثابت حرف *target_rel, libcrun_error_t *err)
{
  ثابت حرف *rootfs = get_private_data (container)->rootfs;
  صحيح rootfsfd = get_private_data (container)->rootfsfd;

  إذا (rootfsfd < 0)
    إرجاع crun_make_error (err, 0, "invalid rootfs state");

  إرجاع safe_openat (rootfsfd, rootfs, target_rel, O_PATH | O_CLOEXEC, 0, err);
}

/* Attempt to open a mount of the specified type.  */
ساكن صحيح
fsopen_mount (ثابت حرف *type, ثابت حرف *source_name, ثابت حرف *labeltype, ثابت حرف *label, ثابت حرف *data)
{
#ifdef HAVE_NEW_MOUNT_API
  cleanup_close صحيح fsfd = -1;
  صحيح ret;

  fsfd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  إذا (UNLIKELY (fsfd < 0))
    إرجاع fsfd;

  /* Best-effort: pseudo file systems (tmpfs, proc, ...) ignore "source",
     so a failure here is expected and harmless.  Real failures surface
     at FSCONFIG_CMD_CREATE below.  */
  (فراغ) syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, "source", source_name ? source_name : type, 0);

  إذا (labeltype)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, labeltype, label, 0);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  إذا (data && data[0])
    {
      cleanup_free حرف *data_copy = xstrdup (data);
      حرف *saveptr = عدم;
      حرف *token;

      لكل (token = strtok_r (data_copy, ",", &saveptr); token; token = strtok_r (عدم, ",", &saveptr))
        {
          حرف *eq = strchr (token, '=');
          إذا (eq)
            {
              *eq = '\0';

              ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, token, eq + 1, 0);
            }
          وإلا
            ret = syscall_fsconfig (fsfd, FSCONFIG_SET_FLAG, token, عدم, 0);

          إذا (UNLIKELY (ret < 0))
            إرجاع ret;
        }
    }

  ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, عدم, عدم, 0);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  إرجاع syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
#وإلا
  (فراغ) type;
  (فراغ) source_name;
  (فراغ) data;
  errno = ENOSYS;
  إرجاع -1;
#endif
}

ساكن صحيح
get_procfd (بنية private_data_s *data, libcrun_error_t *err)
{
  صحيح fd;

  إذا (data->procfd >= 0)
    إرجاع data->procfd;

  fd = fsopen_mount ("proc", عدم, عدم, عدم, عدم);
  إذا (fd < 0)
    fd = open ("/proc", O_DIRECTORY | O_CLOEXEC);
  إذا (UNLIKELY (fd < 0))
    إرجاع crun_make_error (err, errno, "open `/proc`");

  data->procfd = fd;
  إرجاع fd;
}

ساكن صحيح
get_old_جذر_fd (بنية private_data_s *data)
{
  إذا (data->old_جذر_fd >= 0)
    إرجاع data->old_جذر_fd;

  data->old_جذر_fd = open ("/", O_PATH | O_CLOEXEC);
  إرجاع data->old_جذر_fd;
}

ساكن صحيح
fs_move_mount_to (صحيح fd, صحيح dirfd, ثابت حرف *name)
{
#ifdef HAVE_NEW_MOUNT_API
  إذا (name)
    إرجاع syscall_move_mount (fd, "", dirfd, name, MOVE_MOUNT_F_EMPTY_PATH);

  إرجاع syscall_move_mount (fd, "", dirfd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
#وإلا
  (فراغ) fd;
  (فراغ) dirfd;
  (فراغ) name;
  (فراغ) syscall_move_mount;
  errno = ENOSYS;
  إرجاع -1;
#endif
}

تعداد
{
  /* Do not apply any label to the mount.  */
  LABEL_NONE = 0,
  /* Apply the label as a mount option.  */
  LABEL_MOUNT = 1,
  /* Apply the label using setxattr.  */
  LABEL_XATTR = 2,

  LABEL_MASK = 0x0f,

  /* Do not defer a readonly remount — apply MS_RDONLY directly.  */
  MOUNT_NO_DEFERRED_REMOUNT = 0x10,

  /* Do not remount — keep the flags the mount already has.  */
  MOUNT_NO_REMOUNT = 0x20,
};

ساكن صحيح do_mount (libcrun_container_t *container, ثابت حرف *source, صحيح targetfd,
                     ثابت حرف *target, ثابت حرف *fstype, unsigned long mountflags,
                     ثابت فراغ *data, صحيح extra_flags, libcrun_error_t *err);

ساكن bool
mount_option_exists (runtime_spec_schema_defs_mount *mnt, ثابت حرف *option)
{
  size_t i;
  لكل (i = 0; i < mnt->options_len; i++)
    إذا (mnt->options[i] != عدم && strcmp (mnt->options[i], option) == 0)
      إرجاع true;
  إرجاع false;
}

ساكن runtime_spec_schema_defs_mount *
find_mount_for (libcrun_container_t *container, ثابت حرف *destination)
{
  size_t i;
  runtime_spec_schema_config_schema *def = container->container_def;

  لكل (i = 0; i < def->mounts_len; i++)
    {
      إذا (strcmp (def->mounts[i]->destination, destination) == 0)
        إرجاع def->mounts[i];
    }
  إرجاع عدم;
}

ساكن فراغ
warn_tmpfs_fallback_once (بنية private_data_s *private_data, ثابت حرف *reason)
{
  إذا (! private_data->maskdir_warned)
    {
      libcrun_warning ("Falling back to tmpfs for masked dirs (reason: %s)", reason);
      private_data->maskdir_warned = true;
    }
}

/* Get or create the cached shared empty directory for masked paths optimization.
 * Creates directory and FD once per container, caches /proc/self/fd path for fast mounting.
 */
ساكن صحيح
get_shared_empty_dir_cached (libcrun_container_t *container, حرف **proc_fd_path, libcrun_error_t *err)
{
  بنية private_data_s *private_data = get_private_data (container);
  cleanup_close صحيح fd = -1;
  cleanup_free حرف *empty_dir_path = عدم;
  proc_fd_path_t fd_path;
  صحيح ret;

  /* Fast path: return cached proc fd path if already set up */
  إذا (private_data->maskdir_proc_path != عدم)
    {
      إذا (proc_fd_path)
        *proc_fd_path = private_data->maskdir_proc_path;
      إرجاع 0;
    }

  /* Slow path: create directory and cache everything once */
  ret = get_shared_empty_directory_path (&empty_dir_path, container->context->state_جذر, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  /* Open directory and cache FD (once per container) */
  fd = get_bind_mount (-1, empty_dir_path, true, true, false, MS_PRIVATE, err);
  إذا (fd < 0)
    إرجاع fd;

  get_self_fd_path (fd_path, fd);
  private_data->maskdir_proc_path = xstrdup (fd_path);

  private_data->maskdir_fd = fd;
  fd = -1; /* Don't auto-close */

  إذا (proc_fd_path)
    *proc_fd_path = private_data->maskdir_proc_path;
  إرجاع 0;
}

ساكن bool
is_dev_null (ثابت بنية stat *st)
{
  /* 1:3 is the null device, see Documentation/admin-guide/devices.txt.  */
  إرجاع S_ISCHR (st->st_mode) && st->st_rdev == makedev (1, 3);
}

/* Check that fd refers to the null device.  Once the root is the container
   rootfs, /dev/null is whatever the rootfs provides there.  */
ساكن صحيح
ensure_dev_null (صحيح fd, libcrun_error_t *err)
{
  بنية stat st;

  إذا (UNLIKELY (fstat (fd, &st) < 0))
    إرجاع crun_make_error (err, errno, "stat `/dev/null`");

  إذا (UNLIKELY (! is_dev_null (&st)))
    إرجاع crun_make_error (err, 0, "`/dev/null` is not the null device");

  إرجاع 0;
}

/* Whether FD is the root of a read-only tmpfs that can hold no inode
   other than its root, so it is safe to show in place of any directory.  */
ساكن bool
is_empty_ro_tmpfs (صحيح fd)
{
  بنية statfs sfs;

  إذا (fstatfs (fd, &sfs) < 0)
    إرجاع false;

  إرجاع sfs.f_type == TMPFS_MAGIC && (sfs.f_flags & ST_RDONLY) && sfs.f_files == 1;
}

ساكن صحيح
mount_masked_dir (libcrun_container_t *container, صحيح pathfd, ثابت حرف *rel_path, libcrun_error_t *err)
{
  بنية private_data_s *private_data = get_private_data (container);
  cleanup_close صحيح newfd = -1;
  libcrun_error_t tmp_err = عدم;
  صحيح ret;

  إذا (private_data->maskdir_fd < 0)
    {
      إذا (private_data->maskdir_bind_failed)
        اذهب fallback_to_tmpfs;

      ret = get_shared_empty_dir_cached (container, عدم, &tmp_err);
      إذا (ret < 0)
        {
          private_data->maskdir_bind_failed = true;
          warn_tmpfs_fallback_once (private_data, tmp_err->رسالة);
          crun_error_release (&tmp_err);
          اذهب fallback_to_tmpfs;
        }
    }

  إذا (private_data->maskdir_fd >= 0)
    {
      صحيح rootfsfd = private_data->rootfsfd;
      بنية stat before, after;

      ret = fstat (private_data->maskdir_fd, &before);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

      ret = fs_move_mount_to (private_data->maskdir_fd, pathfd, عدم);
      إذا (LIKELY (ret == 0))
        {
          newfd = get_bind_mount (rootfsfd, rel_path, true, true, false, MS_PRIVATE, err);
          إذا (UNLIKELY (newfd < 0))
            إرجاع newfd;

          ret = fstat (newfd, &after);
          إذا (UNLIKELY (ret < 0))
            إرجاع crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

          إذا (before.st_dev != after.st_dev || before.st_ino != after.st_ino)
            إرجاع crun_make_error (err, 0, "race condition detected remounting masked path `/%s`", rel_path);

          TEMP_FAILURE_RETRY (close (private_data->maskdir_fd));
          private_data->maskdir_fd = newfd;
          newfd = -1;

          إرجاع 0;
        }

      /* tmp_err is NULL here: get_shared_empty_dir_cached() succeeded and did not set it. */
      crun_make_error (&tmp_err, errno, "move mount masked dir `%s`", rel_path);

      TEMP_FAILURE_RETRY (close (private_data->maskdir_fd));
      private_data->maskdir_fd = -1;
      private_data->maskdir_move_failed = true;
    }

  private_data->maskdir_bind_failed = true;
  warn_tmpfs_fallback_once (private_data, tmp_err ? tmp_err->رسالة : "unknown error");
  crun_error_release (&tmp_err);

fallback_to_tmpfs:
  libcrun_debug ("using tmpfs fallback for %s", rel_path);
  ret = do_mount (container, "tmpfs", pathfd, rel_path, "tmpfs", MS_RDONLY, "nr_blocks=1,nr_inodes=1",
                  LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  /* Reuse this tmpfs for the other masked directories, so that they do not
     need a tmpfs instance each.  */
  إذا (! private_data->maskdir_move_failed)
    {
      ret = get_bind_mount (private_data->rootfsfd, rel_path, true, true, false, MS_PRIVATE, &tmp_err);
      إذا (ret >= 0 && is_empty_ro_tmpfs (ret))
        private_data->maskdir_fd = ret;
      وإلا
        {
          إذا (ret >= 0)
            TEMP_FAILURE_RETRY (close (ret));
          private_data->maskdir_move_failed = true;
          crun_error_release (&tmp_err);
        }
    }

  إرجاع 0;
}

ساكن صحيح
do_masked_or_readonly_path (libcrun_container_t *container, ثابت حرف *rel_path, bool readonly, bool keep_flags,
                            libcrun_error_t *err)
{
  unsigned long mount_flags = 0;
  ثابت حرف *rootfs = get_private_data (container)->rootfs;
  cleanup_close صحيح pathfd = -1;
  بنية statfs sfs;
  صحيح ret;
  mode_t mode;

  /* The paths come from linux.maskedPaths and linux.readonlyPaths, so a null
     entry in either array reaches us as a NULL pointer.  */
  إذا (UNLIKELY (rel_path == عدم))
    إرجاع crun_make_error (err, EINVAL, "invalid null path in `maskedPaths` or `readonlyPaths`");

  إذا (rel_path[0] == '/')
    rel_path++;

  pathfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, rel_path, O_PATH | O_CLOEXEC, 0, err);
  إذا (UNLIKELY (pathfd < 0))
    {
      errno = crun_error_get_errno (err);
      إذا (errno != ENOENT && errno != EACCES)
        إرجاع pathfd;

      إذا (errno == EACCES)
        libcrun_debug ("skipping inaccessible path `%s`", rel_path);
      crun_error_release (err);
      إرجاع 0;
    }

  إذا (readonly)
    {
      صحيح procfd = get_procfd (get_private_data (container), err);
      proc_fd_path_t source_buffer;
      cleanup_close صحيح mountfd = -1;

      إذا (UNLIKELY (procfd < 0))
        إرجاع procfd;

      get_self_fd_path (source_buffer, pathfd);

      /* Try open_tree + mount_setattr to apply MS_RDONLY atomically.
         Only when keep_flags is false, since keep_flags needs statfs
         to inherit parent mount flags.  */
      إذا (! keep_flags)
        {
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          إذا (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, عدم);
          وإلا
            ret = -1;

          إذا (ret < 0)
            crun_error_release (err);
        }

      إذا (keep_flags || ret < 0)
        {
          mount_flags = MS_BIND | MS_PRIVATE | MS_RDONLY | MS_REC;
          إذا (keep_flags)
            {
              cleanup_close صحيح sfd = openat (procfd, source_buffer, O_PATH | O_CLOEXEC);
              إذا (UNLIKELY (sfd < 0))
                إرجاع crun_make_error (err, errno, "openat procfd for `%s`", rel_path);

              ret = fstatfs (sfd, &sfs);
              إذا (UNLIKELY (ret < 0))
                إرجاع crun_make_error (err, errno, "fstatfs `%s`", rel_path);
              mount_flags = mount_flags | sfs.f_flags;

              /* Parent might contain MS_REMOUNT but the new readonly path is not
                 actually mounted.  Specifically in the case of /proc this will end
                 up with EINVAL therefore remove MS_REMOUNT if it is getting
                 inherited from the parent.  */
              mount_flags = mount_flags & ~MS_REMOUNT;
            }

          close_and_reset (&mountfd);
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          إذا (mountfd >= 0)
            {
              ret = do_mount_setattr (true, rel_path, mountfd, 0, mount_flags & ~(MS_BIND | MS_PRIVATE), err);
              إذا (UNLIKELY (ret < 0))
                إرجاع ret;

              ret = fs_move_mount_to (mountfd, pathfd, عدم);
              إذا (UNLIKELY (ret < 0))
                إرجاع crun_make_error (err, errno, "move mount for readonly path `%s`", rel_path);
            }
          وإلا
            {
              proc_fd_path_t abs_source;

              crun_error_release (err);
              get_proc_self_fd_path (abs_source, pathfd);
              ret = do_mount (container, abs_source, pathfd, rel_path, عدم, mount_flags, عدم,
                              LABEL_NONE, err);
              إذا (UNLIKELY (ret < 0))
                إرجاع ret;
            }
        }
    }
  وإلا
    {
      ret = get_file_type_fd (pathfd, &mode);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "cannot stat `%s`", rel_path);

      إذا ((mode & S_IFMT) == S_IFDIR)
        ret = mount_masked_dir (container, pathfd, rel_path, err);
      وإلا
        {
          cleanup_close صحيح nullfd = -1;
          cleanup_close صحيح mountfd = -1;

          /* If the root is already the container rootfs, this is its
             /dev/null: mask with it only if it is the null device.  */
          nullfd = open ("/dev/null", O_PATH | O_CLOEXEC);
          إذا (UNLIKELY (nullfd < 0))
            إرجاع crun_make_error (err, errno, "open `/dev/null`");

          ret = ensure_dev_null (nullfd, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;

          mountfd = get_bind_mount (nullfd, "", false, true, false, MS_PRIVATE, err);
          إذا (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, عدم);

          إذا (mountfd < 0 || ret < 0)
            {
              proc_fd_path_t null_path;

              crun_error_release (err);
              get_proc_self_fd_path (null_path, nullfd);
              ret = do_mount (container, null_path, pathfd, rel_path, عدم, MS_BIND | MS_RDONLY, عدم,
                              LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
            }
        }
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }
  إرجاع 0;
}

ساكن ثابت حرف *
get_selinux_context_type (libcrun_container_t *container, libcrun_error_t *err)
{
  ثابت حرف *context_type;

  context_type = find_annotation (container, "run.oci.mount_context_type");
  إذا (context_type)
    {
      إذا (strcmp (context_type, "context") != 0
          && strcmp (context_type, "fscontext") != 0
          && strcmp (context_type, "defcontext") != 0
          && strcmp (context_type, "rootcontext") != 0)
        {
          crun_make_error (err, 0, "invalid mount context type `%s` (must be one of: context, fscontext, defcontext, rootcontext)", context_type);
          إرجاع عدم;
        }
      إرجاع context_type;
    }

  إرجاع "context";
}

/* improve the error message on mount(2) failures.  It always returns the original ret.  */
ساكن صحيح
diagnose_mount_failure (صحيح ret, libcrun_error_t *err, libcrun_container_t *container,
                        ثابت حرف *fstype)
{
  إذا (fstype && strcmp (fstype, "cgroup2") == 0 && get_private_data (container)->unified_cgroup_path)
    {
      إذا (access (get_private_data (container)->unified_cgroup_path, R_OK) < 0 && errno == EACCES)
        {
          /* The current cgroup is not accessible.  */
          crun_error_wrap (err, "the current cgroup is not accessible (too restrictive umask?)");
          إرجاع ret;
        }
    }
  إرجاع ret;
}

/* When creating a fresh sysfs mount fails inside a user namespace, fall back to
   bind mounting the host /sys.  Returns 0 when the fallback handled the mount,
   > 0 when it does not apply (the caller must report the original failure),
   < 0 on error.  */
ساكن صحيح
sysfs_userns_fallback (libcrun_container_t *container, ثابت حرف *target,
                       ثابت حرف *real_target, صحيح targetfd,
                       unsigned long mountflags, ثابت حرف *fstype,
                       libcrun_error_t *err)
{
  runtime_spec_schema_defs_mount *cgroup_mount;
  cleanup_close صحيح mountfd = -1;
  cleanup_close صحيح sysfd = -1;
  صحيح sys_old_جذر_fd = -1;
  صحيح ret;

  إذا (! ((mountflags & MS_RDONLY) && targetfd > 0 && fstype && strcmp (fstype, "sysfs") == 0))
    إرجاع 1;

  /* If we are running in an user namespace, just bind mount /sys if creating
     sysfs failed.  */
  ret = check_running_in_user_namespace (err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  إذا (ret == 0)
    إرجاع 1;

  cgroup_mount = find_mount_for (container, "/sys/fs/cgroup");
  sys_old_جذر_fd = get_old_جذر_fd (get_private_data (container));

  /* The bind brings the host mounts below /sys along.  The later do_remount
     only affects the top mount, so make the whole tree read-only here.  */
  إذا (sys_old_جذر_fd >= 0)
    mountfd = get_bind_mount (sys_old_جذر_fd, "sys", true, true, false, MS_PRIVATE, err);
  إذا (mountfd < 0)
    {
      crun_error_release (err);
      mountfd = get_bind_mount (AT_FDCWD, "/sys", true, true, false, MS_PRIVATE, err);
    }

  إذا (cgroup_mount == عدم)
    {
      إذا (mountfd >= 0)
        {
          ret = fs_move_mount_to (mountfd, targetfd, عدم);
          إذا (UNLIKELY (ret < 0))
            إرجاع crun_make_error (err, errno, "move mount `/sys` to `%s`", real_target);
        }
      وإلا
        {
          crun_error_release (err);
          إذا (sys_old_جذر_fd >= 0)
            {
              cleanup_free حرف *sys_path = عدم;
              proc_fd_path_t sys_جذر_path;

              get_proc_self_fd_path (sys_جذر_path, sys_old_جذر_fd);
              xasprintf (&sys_path, "%s/sys", sys_جذر_path);
              ret = mount (sys_path, real_target, عدم, MS_BIND | MS_REC, عدم);
            }
          وإلا
            ret = mount ("/sys", real_target, عدم, MS_BIND | MS_REC, عدم);
          إذا (UNLIKELY (ret < 0))
            إرجاع crun_make_error (err, errno, "bind mount `/sys` from the host");
        }

      sysfd = open_mount_target (container, target, err);
      إذا (UNLIKELY (sysfd < 0))
        إرجاع sysfd;

      ret = do_remount (sysfd, target,
                        MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                        عدم, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      إرجاع do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
    }

  إذا (UNLIKELY (mountfd < 0))
    إرجاع mountfd;

  ret = fs_move_mount_to (mountfd, targetfd, عدم);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "move mount to `%s`", real_target);

  sysfd = open_mount_target (container, target, err);
  إذا (UNLIKELY (sysfd < 0))
    إرجاع sysfd;

  ret = do_remount (sysfd, target,
                    MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                    عدم, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  إذا (mount_option_exists (cgroup_mount, "ro") || mount_option_exists (cgroup_mount, "rro"))
    {
      ret = do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  إرجاع 0;
}

#ifdef HAVE_FGETXATTR
/* Best-effort application of the SELinux security.selinux xattr to a
   freshly mounted target when LABEL_XATTR labeling is requested.  */
ساكن فراغ
apply_selinux_xattr (libcrun_container_t *container, صحيح fd, صحيح extra_flags, ثابت حرف *label)
{
  libcrun_error_t tmp_err = عدم;
  صحيح procfd;

  إذا ((extra_flags & LABEL_MASK) != LABEL_XATTR)
    إرجاع;

  /* Best-effort: swallow any error so it does not leak into the caller.  */
  procfd = get_procfd (get_private_data (container), &tmp_err);
  إذا (procfd >= 0)
    {
      proc_fd_path_t proc_self_path;
      cleanup_close صحيح xfd = -1;

      get_self_fd_path (proc_self_path, fd);
      xfd = openat (procfd, proc_self_path, O_RDONLY | O_CLOEXEC);
      إذا (xfd >= 0)
        (فراغ) fsetxattr (xfd, "security.selinux", label, strlen (label), 0);
    }
  crun_error_release (&tmp_err);
}
#endif

/* After a mount replaced the rootfs (target is empty), the current
   targetfd sits underneath the new mount.  Re-enter the mount namespace
   and reopen the rootfs, refreshing both *fd and the cached rootfsfd.  */
ساكن صحيح
do_mount_replace_rootfs (libcrun_container_t *container, صحيح *fd, libcrun_error_t *err)
{
  صحيح procfd = get_procfd (get_private_data (container), err);
  صحيح tmp;
  صحيح ret;

  إذا (UNLIKELY (procfd < 0))
    إرجاع procfd;

  {
    cleanup_close صحيح nsfd = openat (procfd, "self/ns/mnt", O_RDONLY | O_CLOEXEC);
    إذا (UNLIKELY (nsfd < 0))
      إرجاع crun_make_error (err, errno, "open `/proc/self/ns/mnt`");

    ret = setns (nsfd, CLONE_NEWNS);
    إذا (UNLIKELY (ret < 0))
      إرجاع crun_make_error (err, errno, "setns `CLONE_NEWNS`");
  }

  close_and_reset (fd);
  *fd = open (get_private_data (container)->rootfs, O_PATH | O_CLOEXEC);
  إذا (UNLIKELY (*fd < 0))
    إرجاع crun_make_error (err, errno, "reopen rootfs after mount on /");

  tmp = dup (*fd);
  إذا (UNLIKELY (tmp < 0))
    إرجاع crun_make_error (err, errno, "dup");

  TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
  get_private_data (container)->rootfsfd = tmp;

  إرجاع 0;
}

/* Apply the propagation flags (MS_SHARED, MS_SLAVE, ...) to a mount,
   preferring mount_setattr when a target fd is available and falling
   back to mount(2).  */
ساكن صحيح
apply_propagation_flags (ثابت حرف *target, صحيح targetfd, ثابت حرف *real_target,
                         unsigned long mountflags, libcrun_error_t *err)
{
  bool propagation_done = false;
  صحيح ret;

  إذا (targetfd >= 0)
    {
      libcrun_error_t tmp_err = عدم;
      ret = do_mount_setattr ((mountflags & MS_REC) != 0, target, targetfd, 0, mountflags & ALL_PROPAGATIONS, &tmp_err);
      إذا (LIKELY (ret == 0))
        propagation_done = true;
      وإلا
        crun_error_release (&tmp_err);
    }

  إذا (! propagation_done)
    {
      ret = mount (عدم, real_target, عدم, mountflags & ALL_PROPAGATIONS, عدم);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "set propagation for `%s`", target);
    }

  إرجاع 0;
}

/* Perform (or defer) the remount needed to apply bind/read-only/single
   instance flags.  A read-write remount and MOUNT_NO_DEFERRED_REMOUNT are
   done immediately; otherwise the remount is queued to run later.  *fd is
   consumed when the remount is deferred.  */
ساكن صحيح
schedule_or_do_remount (libcrun_container_t *container, صحيح *fd, صحيح targetfd,
                        ثابت حرف *target, ثابت حرف *real_target,
                        bool single_instance, unsigned long mountflags,
                        ثابت فراغ *data, صحيح extra_flags, libcrun_error_t *err)
{
  unsigned long remount_flags = MS_REMOUNT | (single_instance ? 0 : MS_BIND) | (mountflags & ~ALL_PROPAGATIONS);
  بنية remount_s *r;

  إذا ((remount_flags & MS_RDONLY) == 0 || (extra_flags & MOUNT_NO_DEFERRED_REMOUNT))
    إرجاع do_remount (*fd >= 0 ? *fd : targetfd, real_target, remount_flags, data, err);

  إذا (*fd < 0)
    {
      *fd = dup (targetfd);
      إذا (UNLIKELY (*fd < 0))
        إرجاع crun_make_error (err, errno, "dup `%d`", targetfd);
    }

  /* The remount owns the fd.  */
  r = make_remount (get_and_reset (fd), target, remount_flags, data, get_private_data (container)->remounts);
  get_private_data (container)->remounts = r;

  إرجاع 0;
}

ساكن صحيح
do_mount (libcrun_container_t *container, ثابت حرف *source, صحيح targetfd,
          ثابت حرف *target, ثابت حرف *fstype, unsigned long mountflags, ثابت فراغ *data,
          صحيح extra_flags, libcrun_error_t *err)
{
  cleanup_free حرف *data_with_label = عدم;
  ثابت حرف *data_without_label = data;
  proc_fd_path_t target_buffer;
  ثابت حرف *context_type = عدم;
  cleanup_close صحيح ms_move_fd = -1;
  ثابت حرف *real_target = target;
  bool single_instance = false;
  bool needs_remount = false;
  cleanup_close صحيح fd = -1;
  ثابت حرف *label = عدم;
  صحيح ret = 0;

  إذا (container->container_def->linux && container->container_def->linux->mount_label)
    label = container->container_def->linux->mount_label;
  وإلا
    extra_flags = (extra_flags & ~LABEL_MASK) | LABEL_NONE;

  إذا (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
      needs_remount = true;
    }

  إذا ((extra_flags & LABEL_MASK) == LABEL_MOUNT)
    {
      context_type = get_selinux_context_type (container, err);
      إذا (UNLIKELY (context_type == عدم))
        إرجاع -1;

      ret = add_selinux_mount_label (&data_with_label, data, label, context_type, err);
      إذا (ret < 0)
        إرجاع ret;
      data = data_with_label;
    }

  إذا (mountflags & MS_MOVE)
    {
      إذا ((mountflags & MS_BIND) || fstype)
        إرجاع crun_make_error (err, 0, "internal error: cannot use MS_MOVE with MS_BIND or fstype");

      ret = mount (source, real_target, عدم, MS_MOVE, عدم);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "move mount `%s` to `%s`", source, target);
      mountflags &= ~MS_MOVE;

      /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
      ms_move_fd = open_mount_target (container, target, err);
      إذا (UNLIKELY (ms_move_fd < 0))
        إرجاع ms_move_fd;
      targetfd = ms_move_fd;
    }

  إذا ((fstype && fstype[0]) || (mountflags & MS_BIND))
    {
      unsigned long flags = mountflags & ~(ALL_PROPAGATIONS_NO_REC | MS_RDONLY);

      ret = -1;

      إذا (targetfd >= 0)
        {
          إذا (mountflags & MS_BIND)
            {
              cleanup_close صحيح bindfd = -1;
              libcrun_error_t tmp_err = عدم;

              bindfd = get_bind_mount (AT_FDCWD, source,
                                       (mountflags & MS_REC) != 0,
                                       false, false, 0, &tmp_err);
              إذا (bindfd < 0)
                {
                  صحيح old_جذر_fd = get_old_جذر_fd (get_private_data (container));

                  إذا (old_جذر_fd >= 0 && source && source[0] == '/')
                    {
                      crun_error_release (&tmp_err);
                      bindfd = get_bind_mount (old_جذر_fd, (source[1] == '\0') ? "." : source + 1,
                                               (mountflags & MS_REC) != 0,
                                               false, false, 0, &tmp_err);
                    }
                }
              إذا (bindfd >= 0)
                ret = fs_move_mount_to (bindfd, targetfd, عدم);
              إذا (ret < 0)
                crun_error_release (&tmp_err);
            }
          وإلا
            {
              cleanup_close صحيح newfs = fsopen_mount (fstype, source, context_type, label, data_without_label);
              إذا (newfs >= 0)
                ret = fs_move_mount_to (newfs, targetfd, عدم);
            }
        }

      إذا (ret < 0)
        {
          ret = mount (source, real_target, fstype, flags, data);
          إذا (ret < 0 && (mountflags & MS_BIND) && source && source[0] == '/')
            {
              صحيح old_جذر_fd = get_old_جذر_fd (get_private_data (container));

              إذا (old_جذر_fd >= 0)
                {
                  cleanup_free حرف *proc_source_buf = عدم;
                  proc_fd_path_t old_جذر_path;

                  get_proc_self_fd_path (old_جذر_path, old_جذر_fd);
                  xasprintf (&proc_source_buf, "%s/%s", old_جذر_path, source + 1);
                  ret = mount (proc_source_buf, real_target, fstype, flags, data);
                }
            }
        }
      إذا (UNLIKELY (ret < 0))
        {
          صحيح saved_errno = errno;

          ret = sysfs_userns_fallback (container, target, real_target, targetfd,
                                       mountflags, fstype, err);
          إذا (ret == 0)
            إرجاع 0;
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;

          /* ret > 0: the fallback does not apply, report the original failure.  */
          ret = crun_make_error (err, saved_errno, "mount `%s` to `%s`", source, target);

          إرجاع diagnose_mount_failure (ret, err, container, fstype);
        }

      إذا (targetfd >= 0)
        {
          /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
          fd = open_mount_target (container, target, err);
          إذا (UNLIKELY (fd < 0))
            إرجاع fd;

          /* We are replacing the rootfs, reopen it.  */
          إذا (is_empty_string (target))
            {
              ret = do_mount_replace_rootfs (container, &fd, err);
              إذا (UNLIKELY (ret < 0))
                إرجاع ret;
            }

#ifdef HAVE_FGETXATTR
          apply_selinux_xattr (container, fd, extra_flags, label);
#endif

          targetfd = fd;

          /* The bind mount shadowed the original targetfd, so refresh
             real_target to point to the reopened mountpoint.  Otherwise the
             classic mount(2) fallbacks below (used when mount_setattr is not
             available, e.g. kernels < 5.12) operate on the shadowed path and
             fail with EINVAL.  */
          get_proc_self_fd_path (target_buffer, targetfd);
          real_target = target_buffer;
        }
    }

  إذا (mountflags & ALL_PROPAGATIONS_NO_REC)
    {
      ret = apply_propagation_flags (target, targetfd, real_target, mountflags, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  إذا (mountflags & (MS_BIND | MS_RDONLY))
    needs_remount = true;

  إذا (data && fstype && strcmp (fstype, "proc") == 0)
    {
      single_instance = true;
      needs_remount = true;
    }

  إذا (needs_remount && ! (extra_flags & MOUNT_NO_REMOUNT))
    {
      ret = schedule_or_do_remount (container, &fd, targetfd, target, real_target,
                                    single_instance, mountflags, data, extra_flags, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  إرجاع ret;
}

ساكن فراغ
try_umount (صحيح targetfd, ثابت حرف *target)
{
  proc_fd_path_t target_buffer;
  ثابت حرف *real_target = target;

  إذا (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  umount2 (real_target, MNT_DETACH);
}

ساكن bool
container_has_cgroupns (libcrun_container_t *container)
{
  إرجاع get_private_data (container)->unshare_flags & CLONE_NEWCGROUP;
}

ساكن صحيح
do_mount_cgroup_v2 (libcrun_container_t *container, صحيح targetfd, ثابت حرف *target,
                    unsigned long mountflags, ثابت حرف *data, libcrun_error_t *err)
{
  صحيح ret;
  صحيح cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  إذا (UNLIKELY (cgroup_mode < 0))
    إرجاع cgroup_mode;

  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
  إذا (UNLIKELY (ret < 0))
    {
      صحيح saved_errno = crun_error_get_errno (err);
      إذا (saved_errno == EPERM || saved_errno == EBUSY)
        {
          ثابت حرف *unified_cgroup_path;
          ثابت حرف *src_cgroup;

          crun_error_release (err);

          إذا (saved_errno == EBUSY)
            {
              /* If we got EBUSY it means the cgroup file system is already mounted at the targetfd and we
                 cannot stack another one on top of it.  Attempt mounting a tmpfs below the cgroup mount.  */

              ret = do_mount (container, "tmpfs", targetfd, target, "tmpfs", MS_PRIVATE, "nr_blocks=1,nr_inodes=1", LABEL_NONE, err);
              إذا (LIKELY (ret == 0))
                {
                  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
                  إذا (LIKELY (ret == 0))
                    إرجاع ret;

                  /* Best-effort cleanup for the tmpfs, if it fails there is nothing to worry about.  */
                  try_umount (targetfd, target);
                }

              /* If the previous method failed, fall back to bind mounting the current cgroup.  */
              crun_error_release (err);
            }

          unified_cgroup_path = get_private_data (container)->unified_cgroup_path;

          /* If everything else failed, bind mount from the current cgroup.  */
          src_cgroup = unified_cgroup_path && container_has_cgroupns (container) ? unified_cgroup_path : CGROUP_جذر;
          إرجاع do_mount (container, src_cgroup, targetfd, target, عدم,
                           MS_BIND | mountflags, عدم, LABEL_NONE, err);
        }

      إرجاع ret;
    }

  إرجاع 0;
}

ساكن صحيح
do_mount_cgroup_systemd_v1 (libcrun_container_t *container, ثابت حرف *source, صحيح targetfd, ثابت حرف *target,
                            unsigned long mountflags, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_close صحيح fd = -1;
  ثابت حرف *subsystem = "systemd";
  cleanup_free حرف *subsystem_path = عدم;
  cleanup_close صحيح tmpfsdirfd = -1;

  mountflags = mountflags & ~MS_BIND;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags, "size=1024k", LABEL_NONE, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  /* Get a reference to the newly created cgroup directory.  */
  tmpfsdirfd = open_mount_target (container, target, err);
  إذا (UNLIKELY (tmpfsdirfd < 0))
    إرجاع tmpfsdirfd;
  targetfd = tmpfsdirfd;

  ret = mkdirat (targetfd, subsystem, 0755);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "mkdirat `%s`", subsystem);

  ret = append_paths (&subsystem_path, err, target, subsystem, عدم);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  fd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
  إذا (UNLIKELY (fd < 0))
    إرجاع crun_make_error (err, errno, "openat `%s`", subsystem_path);

  إرجاع do_mount (container, "cgroup", fd, subsystem_path, "cgroup", mountflags, "none,name=systemd,xattr", LABEL_NONE,
                   err);
}

ساكن صحيح
do_mount_cgroup_v1 (libcrun_container_t *container, ثابت حرف *source, صحيح targetfd, ثابت حرف *target,
                    unsigned long mountflags, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_free حرف *content = عدم;
  حرف *from;
  cleanup_close صحيح tmpfsdirfd = -1;
  حرف *saveptr = عدم;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags & ~MS_RDONLY, "size=1024k", LABEL_MOUNT,
                  err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  tmpfsdirfd = open_mount_target (container, target, err);
  إذا (UNLIKELY (tmpfsdirfd < 0))
    إرجاع tmpfsdirfd;
  targetfd = tmpfsdirfd;

  {
    صحيح procfd = get_procfd (get_private_data (container), err);
    إذا (UNLIKELY (procfd < 0))
      إرجاع procfd;

    ret = read_all_file_at (procfd, SELF_CGROUP, &content, عدم, err);
  }
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  إذا (UNLIKELY (content == عدم || content[0] == '\0'))
    إرجاع crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  لكل (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (عدم, "\n", &saveptr))
    {
      cleanup_free حرف *source_path = عدم;
      cleanup_free حرف *source_subsystem = عدم;
      cleanup_free حرف *subsystem_path = عدم;
      حرف *subpath, *subsystem, *subsystem_fqn, *it;
      cleanup_close صحيح subsystemfd = -1;
      subsystem = strchr (from, ':') + 1;
      subpath = strchr (subsystem, ':') + 1;
      *(subpath - 1) = '\0';

      إذا (subsystem[0] == '\0')
        استمر;

      /* subsystem_fqn includes name= for named hierarchies.  */
      subsystem_fqn = subsystem;

      it = strstr (subsystem, "name=");
      إذا (it)
        subsystem = it + 5;

      إذا (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      إذا (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      ret = append_paths (&source_subsystem, err, CGROUP_جذر, subsystem, عدم);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      /* if there is already a mount specified, do not add a default one.  */
      إذا (find_mount_for (container, source_subsystem) != عدم)
        استمر;

      ret = append_paths (&source_path, err, source_subsystem, subpath, عدم);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      ret = append_paths (&subsystem_path, err, target, subsystem, عدم);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      ret = mkdirat (targetfd, subsystem, 0755);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "mkdirat `%s`", subsystem_path);

      subsystemfd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
      إذا (UNLIKELY (subsystemfd < 0))
        إرجاع crun_make_error (err, errno, "open `%s`", subsystem_path);

      إذا (container_has_cgroupns (container))
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, "cgroup", mountflags, subsystem_fqn,
                          LABEL_NONE, err);
          إذا (UNLIKELY (ret < 0))
            {
              إذا (crun_error_get_errno (err) == ENOENT || crun_error_get_errno (err) == ENODEV)
                {
                  /* We are trying to mount a subsystem that is not present.  */
                  crun_error_release (err);
                  استمر;
                }
              إرجاع ret;
            }
        }
      وإلا
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, عدم, MS_BIND | mountflags, عدم,
                          LABEL_NONE, err);
          إذا (UNLIKELY (ret < 0))
            {
              إذا (crun_error_get_errno (err) != ENOENT)
                إرجاع ret;

              crun_error_release (err);

              /* We might already be in a container.  Mount the source subsystem.  */
              ret = do_mount (container, source_subsystem, subsystemfd, subsystem_path, عدم, MS_BIND | mountflags,
                              عدم, LABEL_NONE, err);
              إذا (UNLIKELY (ret < 0))
                {
                  /* If it still fails with ENOENT, ignore the error as the controller might have been
                     dropped and doesn't exist.  */
                  إذا (crun_error_get_errno (err) != ENOENT)
                    إرجاع ret;

                  crun_error_release (err);
                }
            }
        }
    }

  ret = libcrun_cgroups_create_symlinks (targetfd, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  إرجاع 0;
}

ساكن صحيح
do_mount_cgroup (libcrun_container_t *container, ثابت حرف *source, صحيح targetfd, ثابت حرف *target,
                 unsigned long mountflags, ثابت حرف *data, libcrun_error_t *err)
{
  صحيح cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  إذا (UNLIKELY (cgroup_mode < 0))
    إرجاع cgroup_mode;

  بدل (cgroup_mode)
    {
    حالة CGROUP_MODE_UNIFIED:
      إرجاع do_mount_cgroup_v2 (container, targetfd, target, mountflags, data, err);
    حالة CGROUP_MODE_LEGACY:
    حالة CGROUP_MODE_HYBRID:
      إرجاع do_mount_cgroup_v1 (container, source, targetfd, target, mountflags, err);
    }

  إرجاع crun_make_error (err, 0, "unknown cgroup mode `%d`", cgroup_mode);
}

بنية device_s needed_devs[] = { { "/dev/null", "c", 1, 3, 0666, 0, 0 },
                                  { "/dev/zero", "c", 1, 5, 0666, 0, 0 },
                                  { "/dev/full", "c", 1, 7, 0666, 0, 0 },
                                  { "/dev/tty", "c", 5, 0, 0666, 0, 0 },
                                  { "/dev/random", "c", 1, 8, 0666, 0, 0 },
                                  { "/dev/urandom", "c", 1, 9, 0666, 0, 0 },
                                  {} };

/* Pre-open (via open_tree) the needed device fds and the notify socket
   while the host file system is still reachable.  Failures are ignored:
   the fds are optional optimizations and a later mount(2) fallback
   handles the missing entries.  */
ساكن فراغ
preopen_needed_devs_and_notify (libcrun_container_t *container)
{
  بنية private_data_s *private_data = get_private_data (container);
  size_t i;

  إذا (private_data->needed_devs_fds)
    {
      بنية libcrun_fd_map *dev_fds = private_data->needed_devs_fds;

      لكل (i = 0; needed_devs[i].path; i++)
        {
          إذا (i < dev_fds->nfds && dev_fds->fds[i] >= 0)
            استمر;

          صحيح fd = syscall_open_شجرة (AT_FDCWD, needed_devs[i].path,
                                      OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
          إذا (fd >= 0 && i < dev_fds->nfds)
            dev_fds->fds[i] = fd;
          وإلا إذا (fd >= 0)
            close (fd);
        }
    }

  إذا (private_data->notify_socket_شجرة_fd < 0
      && private_data->host_notify_socket_path)
    {
      صحيح fd = syscall_open_شجرة (AT_FDCWD, private_data->host_notify_socket_path,
                                  OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
      إذا (fd >= 0)
        private_data->notify_socket_شجرة_fd = fd;
    }
}

/* Check if the specified path is a direct child of /dev.  If it is
 return a pointer to the basename.  */
ساكن ثابت حرف *
relative_path_under_dev (ثابت حرف *path)
{
  إذا (path[0] != '/')
    إرجاع path;
  إذا (strncmp (path, "/dev/", 5) == 0)
    {
      إذا (strchr (path + 5, '/'))
        إرجاع عدم;
      إرجاع path + 5;
    }
  إرجاع عدم;
}

ساكن صحيح
mknod_and_set_attrs (libcrun_container_t *container, صحيح dirfd, ثابت حرف *name, بنية device_s *device,
                     mode_t type, dev_t dev, libcrun_error_t *err)
{
  cleanup_close صحيح fd = -1;
  proc_fd_path_t fd_path;
  ثابت حرف *what = "new";
  bool exists = false;
  بنية stat st;
  صحيح procfd;
  صحيح ret;

  ret = mknodat (dirfd, name, device->mode | type, dev);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (errno != EEXIST)
        إرجاع crun_make_error (err, errno, "mknodat `%s`", device->path);
      /* The runtime spec requires an error if the existing file
         does not match the requested device, so check it below.  */
      exists = true;
      what = "existing";
    }

  /* Get a handle and verify that it matches the expected inode type
     and major:minor before operating on it.  */
  fd = openat (dirfd, name, O_PATH | O_NOFOLLOW | O_CLOEXEC);
  إذا (UNLIKELY (fd < 0))
    إرجاع crun_make_error (err, errno, "open %s device `%s`", what, device->path);

  ret = fstat (fd, &st);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "fstat %s device `%s`", what, device->path);

  إذا (UNLIKELY ((st.st_mode & S_IFMT) != type))
    إرجاع crun_make_error (err, 0, "%s device `%s` has incorrect file type %#o, expected %#o",
                            what, device->path, st.st_mode & S_IFMT, type);

  إذا (UNLIKELY (type != S_IFIFO && st.st_rdev != dev))
    إرجاع crun_make_error (err, 0, "%s device `%s` has incorrect major:minor %u:%u, expected %u:%u",
                            what, device->path, major (st.st_rdev), minor (st.st_rdev),
                            major (dev), minor (dev));

  /* Leave the existing inode's mode and owner as is.  */
  إذا (exists)
    إرجاع 0;

  /* fchmod(2) does not work on O_PATH fds, so go through /proc.  */
  procfd = get_procfd (get_private_data (container), err);
  إذا (UNLIKELY (procfd < 0))
    إرجاع procfd;

  get_self_fd_path (fd_path, fd);
  ret = fchmodat (procfd, fd_path, device->mode, 0);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "fchmod `%s`", device->path);

  ret = fchownat (fd, "", device->uid, device->gid, AT_EMPTY_PATH);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "fchown `%s`", device->path);

  إرجاع 1;
}

صحيح
libcrun_create_dev (libcrun_container_t *container, صحيح devfd, صحيح srcfd,
                    بنية device_s *device, bool binds, bool ensure_parent_dir,
                    libcrun_error_t *err)
{
  صحيح ret;
  dev_t dev;
  mode_t type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  ثابت حرف *fullname = device->path;
  ثابت حرف *rootfs = get_private_data (container)->rootfs;
  إذا (is_empty_string (fullname))
    إرجاع crun_make_error (err, EINVAL, "device path is empty");
  // Normalize the path by removing trailing slashes.
  cleanup_free حرف *normalized_path = xstrdup (fullname);
  consume_trailing_slashes (normalized_path);
  إذا (normalized_path[0] == '\0')
    strcpy (normalized_path, "/");
  ثابت حرف *rel_dev = relative_path_under_dev (normalized_path);

  إذا (binds)
    {
      cleanup_close صحيح fd = -1;

      إذا (rel_dev)
        {
          fd = openat (devfd, rel_dev, O_NOFOLLOW | O_CLOEXEC | O_PATH | O_NONBLOCK);
          إذا (UNLIKELY (fd < 0))
            {
              إذا (errno == ENOENT)
                fd = openat (devfd, rel_dev, O_CREAT | O_NOFOLLOW | O_CLOEXEC | O_NONBLOCK, 0700);

              إذا (UNLIKELY (fd < 0))
                إرجاع crun_make_error (err, errno, "openat `%s`", device->path);
            }
        }
      وإلا
        {
          ثابت حرف *rel_path = consume_slashes (normalized_path);

          fd = crun_safe_create_and_open_ref_at (false, get_private_data (container)->rootfsfd, rootfs, rel_path, 0755, err);
          إذا (UNLIKELY (fd < 0))
            إرجاع fd;
        }

      إذا (srcfd >= 0)
        {
          ret = do_mount_setattr (false, normalized_path, srcfd, 0, MS_NOSUID | MS_NOEXEC, err);
          إذا (LIKELY (ret == 0))
            {
              ret = syscall_move_mount (srcfd, "", fd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
              إذا (LIKELY (ret >= 0))
                إرجاع 0;
            }
          وإلا
            crun_error_release (err);
        }

      {
        cleanup_close صحيح mountfd = -1;

        /* Try open_tree+mount_setattr to apply flags atomically.  */
        mountfd = get_bind_mount (-1, fullname, false, false, false, MS_PRIVATE, err);
        إذا (mountfd >= 0)
          {
            ret = do_mount_setattr (false, normalized_path, mountfd, 0, MS_NOSUID | MS_NOEXEC, err);
            إذا (LIKELY (ret == 0))
              {
                ret = fs_move_mount_to (mountfd, fd, عدم);
                إذا (LIKELY (ret == 0))
                  إرجاع 0;
              }
            وإلا
              crun_error_release (err);
          }
        وإلا
          crun_error_release (err);
      }

      ret = do_mount (container, fullname, fd, normalized_path, عدم, MS_BIND | MS_PRIVATE | MS_NOEXEC | MS_NOSUID, عدم, LABEL_MOUNT, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }
  وإلا
    {
      dev = makedev (device->major, device->minor);

      /* Check whether the path is directly under /dev.  Since we already have an open fd to /dev and mknodat(2)
         fails when the destination already exists or is a symlink, it is safe to use it directly.
         If it is not a direct child, then first get a fd to the dirfd.
      */
      إذا (rel_dev)
        {
          ret = mknod_and_set_attrs (container, devfd, rel_dev, device, type, dev, err);
          إذا (ret <= 0)
            إرجاع ret;
        }
      وإلا
        {
          cleanup_close صحيح dirfd = -1;
          cleanup_free حرف *dirname = عدم;
          حرف *basename, *found;

          dirname = xstrdup (normalized_path);

          found = strrchr (dirname, '/');
          إذا (found)
            *found = '\0';

          basename = found ? found + 1 : dirname;

          إذا (dirname[0] == '\0')
            {
              dirfd = dup (get_private_data (container)->rootfsfd);
              إذا (UNLIKELY (dirfd < 0))
                إرجاع crun_make_error (err, errno, "dup fd for `%s`", rootfs);
            }
          وإلا
            {
              dirfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, dirname, O_DIRECTORY | O_PATH | O_CLOEXEC, 0, err);
              إذا (dirfd < 0 && ensure_parent_dir)
                {
                  crun_error_release (err);

                  dirfd = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs, dirname, 0755, err);
                }
              إذا (UNLIKELY (dirfd < 0))
                إرجاع dirfd;
            }

          ret = mknod_and_set_attrs (container, dirfd, basename, device, type, dev, err);
          إذا (ret <= 0)
            إرجاع ret;
        }
    }
  إرجاع 0;
}

بنية symlink_s
{
  ثابت حرف *path;
  ثابت حرف *target;
  bool force;
};

ساكن بنية symlink_s symlinks[] = { { "/proc/self/fd", "fd", false },
                                       { "/proc/self/fd/0", "stdin", false },
                                       { "/proc/self/fd/1", "stdout", false },
                                       { "/proc/self/fd/2", "stderr", false },
                                       { "/proc/kcore", "core", false },
                                       { "pts/ptmx", "ptmx", true },
                                       { عدم, عدم, false } };

ساكن صحيح
create_missing_devs (libcrun_container_t *container, bool binds, libcrun_error_t *err)
{
  صحيح ret;
  size_t i;
  بنية device_s *it;
  cleanup_close صحيح devfd = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  ثابت حرف *rootfs = get_private_data (container)->rootfs;
  cleanup_close_map بنية libcrun_fd_map *dev_fds = عدم;
  cleanup_close_map بنية libcrun_fd_map *needed_fds = عدم;

  dev_fds = get_private_data (container)->dev_fds;
  get_private_data (container)->dev_fds = عدم;

  needed_fds = get_private_data (container)->needed_devs_fds;
  get_private_data (container)->needed_devs_fds = عدم;

  إذا (! def || ! def->linux)
    إرجاع 0;

  devfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, "dev", O_CLOEXEC | O_PATH | O_DIRECTORY, 0, err);
  إذا (UNLIKELY (devfd < 0))
    إرجاع devfd;

  لكل (i = 0; i < def->linux->devices_len; i++)
    {
      بنية device_s device = {
        def->linux->devices[i]->path,
        def->linux->devices[i]->type,
        def->linux->devices[i]->major,
        def->linux->devices[i]->minor,
        def->linux->devices[i]->file_mode,
        def->linux->devices[i]->uid,
        def->linux->devices[i]->gid,
      };

      إذا (! def->linux->devices[i]->file_mode_present)
        device.mode = 0666;
      ret = libcrun_create_dev (container, devfd, dev_fds->fds[i], &device, binds, true, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  لكل (it = needed_devs, i = 0; it->path; it++, i++)
    {
      cleanup_close صحيح srcfd = (needed_fds && i < needed_fds->nfds) ? needed_fds->fds[i] : -1;

      /* make sure the parent directory exists only on the first iteration.  */
      ret = libcrun_create_dev (container, devfd, srcfd, it, binds, it == needed_devs, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
      إذا (srcfd >= 0)
        needed_fds->fds[i] = -1;
    }

  لكل (i = 0; symlinks[i].target; i++)
    {
    retry_symlink:
      ret = symlinkat (symlinks[i].path, devfd, symlinks[i].target);
      إذا (UNLIKELY (ret < 0))
        {
          صحيح saved_errno = errno;

          إذا (errno == EEXIST && ! symlinks[i].force)
            استمر;

          /* If the symlink should be forced, make sure to unlink any existing file at the same path.  */
          إذا (errno == EEXIST)
            {
            retry_unlink:
              ret = unlinkat (devfd, symlinks[i].target, 0);
              إذا (ret < 0 && errno == EISDIR)
                ret = unlinkat (devfd, symlinks[i].target, AT_REMOVEDIR);
              إذا (ret < 0 && errno == EBUSY)
                {
                  cleanup_close صحيح tfd = openat (devfd, symlinks[i].target, O_CLOEXEC | O_PATH | O_NOFOLLOW);
                  إذا (tfd >= 0)
                    {
                      proc_fd_path_t procpath;

                      get_proc_self_fd_path (procpath, tfd);
                      إذا (umount2 (procpath, MNT_DETACH) == 0)
                        اذهب retry_unlink;
                    }
                }
              إذا (ret == 0)
                اذهب retry_symlink;
            }
          إرجاع crun_make_error (err, saved_errno, "symlinkat `/dev/%s`", symlinks[i].target);
        }
    }

  إذا (container->container_def->process && container->container_def->process->terminal)
    {
      ret = create_file_if_missing_at (devfd, "console", 0620, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  إرجاع 0;
}

/* Advance to the next path component, skipping empty and "." ones.  */
ساكن ثابت حرف *
next_path_component (ثابت حرف *p, size_t *len)
{
  لكل (;;)
    {
      p = consume_slashes (p);
      *len = strcspn (p, "/");
      إذا (*len != 1 || p[0] != '.')
        إرجاع p;
      p++;
    }
}

/* Whether A and B are the same path, ignoring repeated, leading and
   trailing slashes, and "." components.  ".." components are compared
   as they are, not resolved lexically: if a path component before them
   is a symlink, the path may refer to something else entirely, and
   treating it as a duplicate would leave it unmasked.  */
ساكن bool
same_path (ثابت حرف *a, ثابت حرف *b)
{
  size_t la, lb;

  لكل (;;)
    {
      a = next_path_component (a, &la);
      b = next_path_component (b, &lb);
      إذا (la != lb || memcmp (a, b, la) != 0)
        إرجاع false;
      إذا (la == 0)
        إرجاع true;
      a += la;
      b += lb;
    }
}

/* Whether paths[i] is the same as one of the paths before it.  */
ساكن bool
is_duplicate_path (حرف **paths, size_t i)
{
  size_t j;

  إذا (paths[i] == عدم)
    إرجاع false;

  لكل (j = 0; j < i; j++)
    إذا (paths[j] && same_path (paths[j], paths[i]))
      إرجاع true;

  إرجاع false;
}

ساكن صحيح
do_masked_and_readonly_paths (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  صحيح ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  إذا (! def || ! def->linux)
    إرجاع 0;

  لكل (i = 0; i < def->linux->masked_paths_len; i++)
    {
      إذا (is_duplicate_path (def->linux->masked_paths, i))
        استمر;

      ret = do_masked_or_readonly_path (container, def->linux->masked_paths[i], false, false, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }
  لكل (i = 0; i < def->linux->readonly_paths_len; i++)
    {
      ret = do_masked_or_readonly_path (container, def->linux->readonly_paths[i], true, true, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }
  إرجاع 0;
}

ساكن صحيح
do_pivot (libcrun_container_t *container, ثابت حرف *rootfs, libcrun_error_t *err)
{
  cleanup_close صحيح oldrootfd = -1;
  cleanup_close صحيح newrootfd = -1;
  صحيح ret;

  oldrootfd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  إذا (UNLIKELY (oldrootfd < 0))
    إرجاع crun_make_error (err, errno, "open `/`");

  newrootfd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  إذا (UNLIKELY (newrootfd < 0))
    إرجاع crun_make_error (err, errno, "open `%s`", rootfs);

  ret = fchdir (newrootfd);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = pivot_جذر (".", ".");
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "pivot_root");

  ret = fchdir (oldrootfd);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = do_mount (container, عدم, -1, ".", عدم, MS_REC | MS_PRIVATE, عدم, LABEL_MOUNT, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  ret = umount2 (".", MNT_DETACH);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "umount2 oldroot");

  افعل
    {
      ret = umount2 (".", MNT_DETACH);
      إذا (ret < 0 && errno == EINVAL)
        اكسر;
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "umount2 oldroot");
  } بينما (ret == 0);

  ret = chdir ("/");
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "chdir to newroot");

  إرجاع 0;
}

ساكن صحيح
append_tmpfs_mode_if_missing (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, حرف **data, libcrun_error_t *err)
{
  ثابت حرف *rootfs = get_private_data (container)->rootfs;
  bool empty_data = is_empty_string (*data);
  cleanup_close صحيح fd = -1;
  بنية stat st;
  صحيح ret;

  إذا (*data != عدم && strstr (*data, "mode="))
    إرجاع 0;

  fd = safe_openat (get_private_data (container)->rootfsfd, rootfs, mount->destination, O_CLOEXEC | O_PATH, 0, err);
  إذا (fd < 0)
    {
      إذا (crun_error_get_errno (err) != ENOENT)
        إرجاع fd;

      crun_error_release (err);
      إرجاع 0;
    }
  ret = fstat (fd, &st);
  إذا (ret < 0)
    إرجاع crun_make_error (err, errno, "fstat `%s`", mount->destination);

  xasprintf (data, "%s%smode=%o", empty_data ? "" : *data, empty_data ? "" : ",", st.st_mode & 07777);
  إرجاع 0;
}

ساكن صحيح
get_default_flags (libcrun_container_t *container, ثابت حرف *destination, حرف **data)
{
  إذا (strcmp (destination, "/proc") == 0)
    إرجاع 0;
  إذا (strcmp (destination, "/dev/cgroup") == 0 || strcmp (destination, "/sys/fs/cgroup") == 0)
    {
      *data = xstrdup ("none,name=");
      إرجاع MS_NOEXEC | MS_NOSUID | MS_STRICTATIME;
    }
  إذا (strcmp (destination, "/dev") == 0)
    {
      *data = xstrdup ("mode=755");
      إرجاع MS_NOEXEC | MS_STRICTATIME;
    }
  إذا (strcmp (destination, "/dev/shm") == 0)
    {
      *data = xstrdup ("mode=1777,size=65536k");
      إرجاع MS_NOEXEC | MS_NOSUID | MS_NODEV;
    }
  إذا (strcmp (destination, "/dev/mqueue") == 0)
    إرجاع MS_NOEXEC | MS_NOSUID | MS_NODEV;
  إذا (strcmp (destination, "/dev/pts") == 0)
    {
      إذا (container->host_uid == 0)
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620,gid=5");
      وإلا
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620");
      إرجاع MS_NOEXEC | MS_NOSUID;
    }
  إذا (strcmp (destination, "/sys") == 0)
    إرجاع MS_NOEXEC | MS_NOSUID | MS_NODEV;

  إرجاع 0;
}

ساكن ثابت حرف *
get_force_cgroup_v1_annotation (libcrun_container_t *container)
{
  إرجاع find_annotation (container, "run.oci.systemd.force_cgroup_v1");
}

ساكن صحيح
check_valid_no_follow_file_system (صحيح fd, ثابت حرف *destination, libcrun_error_t *err)
{
  بنية statfs sfs;

  صحيح ret = fstatfs (fd, &sfs);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "statfs `%s`", destination);

  بدل (sfs.f_type)
    {
    حالة PROC_SUPER_MAGIC:
    حالة SYSFS_MAGIC:
    حالة DEVPTS_SUPER_MAGIC:
      إرجاع crun_make_error (err, 0, "override of the mount destination `/%s` is not allowed", destination);
    }

  إرجاع 0;
}

ساكن صحيح
safe_create_symlink (صحيح rootfsfd, ثابت حرف *rootfs, ثابت حرف *target, ثابت حرف *destination, libcrun_error_t *err)
{
  cleanup_close صحيح parent_dir_fd = -1;
  cleanup_free حرف *buffer = عدم;
  حرف *part;
  صحيح ret;

  إذا (is_empty_string (destination))
    إرجاع crun_make_error (err, 0, "empty destination for symlink `%s`", target);

  buffer = xstrdup (destination);
  part = dirname (buffer);

  parent_dir_fd = crun_safe_create_and_open_ref_at (true, rootfsfd, rootfs, part, 0755, err);
  إذا (UNLIKELY (parent_dir_fd < 0))
    إرجاع parent_dir_fd;

  /* It is safe to reuse the buffer since it was created with xstrdup (destination).  */
  strcpy (buffer, destination);
  part = basename (buffer);

  ret = symlinkat (target, parent_dir_fd, part);
  إذا (UNLIKELY (ret < 0))
    {
      /* If it exists, check if it has the same content, if so just ignore the error.  */
      إذا (errno == EEXIST)
        {
          cleanup_free حرف *link = عدم;
          ssize_t len;

          len = safe_readlinkat (parent_dir_fd, part, &link, 0, err);
          إذا (UNLIKELY (len < 0))
            إرجاع len;

          إذا ((((size_t) len) == strlen (target)) && strncmp (link, target, len) == 0)
            إرجاع 0;

          إرجاع crun_make_error (err, 0, "symlink `%s` already exists with a different content", destination);
        }
      إرجاع crun_make_error (err, errno, "symlinkat `%s`", target);
    }

  إرجاع 0;
}

ساكن صحيح
handle_copy_symlink (libcrun_container_t *container, ثابت حرف *rootfs,
                     runtime_spec_schema_defs_mount *mount,
                     ثابت حرف *link_target, libcrun_error_t *err)
{
  cleanup_free حرف *target = عدم;

  إذا (link_target == عدم)
    {
      صحيح old_جذر_fd = get_old_جذر_fd (get_private_data (container));
      ثابت حرف *source = mount->source;
      ssize_t len;

      إذا (old_جذر_fd >= 0 && source[0] == '/')
        len = safe_readlinkat (old_جذر_fd, source + 1, &target, 0, err);
      وإلا
        len = safe_readlinkat (AT_FDCWD, source, &target, 0, err);
      إذا (UNLIKELY (len < 0))
        إرجاع len;
      link_target = target;
    }

  إرجاع safe_create_symlink (get_private_data (container)->rootfsfd, rootfs,
                              link_target, mount->destination, err);
}

ساكن صحيح
prepare_sysfs_or_proc_mount (libcrun_container_t *container, ثابت حرف *target,
                             صحيح *targetfd, libcrun_error_t *err)
{
  صحيح ret;

  /* Enforce sysfs and proc to be mounted on a regular directory.  */
  ret = openat (get_private_data (container)->rootfsfd, target,
                O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (errno == ENOENT)
        {
          إذا (strchr (target, '/'))
            إرجاع crun_make_error (err, 0, "invalid target `%s`: it must be mounted at the root", target);

          ret = mkdirat (get_private_data (container)->rootfsfd, target, 0755);
          إذا (UNLIKELY (ret < 0))
            إرجاع crun_make_error (err, errno, "mkdirat `%s`", target);

          /* Try opening it again.  */
          ret = openat (get_private_data (container)->rootfsfd, target,
                        O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
        }
      وإلا إذا (errno == ENOTDIR)
        إرجاع crun_make_error (err, errno, "the target `/%s` is invalid", target);

      إذا (ret < 0)
        إرجاع crun_make_error (err, errno, "open `%s`", target);
    }

  *targetfd = ret;
  إرجاع 0;
}

ساكن صحيح
handle_tmpcopyup (libcrun_container_t *container, ثابت حرف *rootfs, ثابت حرف *target,
                  صحيح copy_from_fd, libcrun_error_t *err)
{
  صحيح destfd, ret;
  cleanup_close صحيح tmpfd = copy_from_fd;

  target = consume_slashes (target);

  إذا (is_empty_string (target))
    إرجاع crun_make_error (err, 0, "tmpcopyup on `/` is not supported");

  destfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target,
                        O_CLOEXEC | O_DIRECTORY, 0, err);
  إذا (UNLIKELY (destfd < 0))
    إرجاع crun_error_wrap (err, "open `%s` to write for tmpcopyup", target);

  // copy_recursive_fd_to_fd closes tmpfd and destfd
  ret = copy_recursive_fd_to_fd (tmpfd, destfd, target, target, err);
  tmpfd = -1;
  إرجاع ret;
}

ساكن صحيح
get_mount_label_how (ثابت حرف *type, bool is_sysfs_or_proc)
{
  إذا (is_sysfs_or_proc)
    إرجاع LABEL_NONE;

  إذا (strcmp (type, "mqueue") == 0)
    إرجاع LABEL_XATTR;

  إرجاع LABEL_MOUNT;
}

/* The kernel does not let a mount inherited from a more privileged mount
   namespace lose its ro, nosuid, nodev and noexec flags in a user
   namespace.  The bind mounts prepared from the host are not subject to
   that, so, for a container with a user namespace, return the flags of
   SOURCE that are to be kept, i.e. those not explicitly cleared by the
   mount options.  */
ساكن unsigned long
get_bind_mount_locked_flags (صحيح source_mountfd, ثابت حرف *source, runtime_spec_schema_defs_mount *mount)
{
  unsigned long cleared = 0;
  بنية statfs sfs;
  size_t i;
  صحيح ret;

  إذا (source_mountfd >= 0)
    ret = fstatfs (source_mountfd, &sfs);
  وإلا
    ret = statfs (source, &sfs);
  إذا (UNLIKELY (ret < 0))
    إرجاع 0;

  لكل (i = 0; i < mount->options_len; i++)
    {
      ثابت بنية propagation_flags_s *prop;

      إذا (mount->options[i] == عدم)
        استمر;

      prop = libcrun_str2mount_flags (mount->options[i]);
      إذا (prop == عدم || (prop->extra_flags & OPTION_RECURSIVE))
        استمر;

      إذا (prop->clear)
        cleared |= prop->flags;
      وإلا
        cleared &= ~prop->flags;
    }

  إرجاع sfs.f_flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC) & ~cleared;
}

/* If PATH is lexically inside the container rootfs ROOTFS, return the part
   of it relative to ROOTFS, otherwise NULL.  Such a bind mount source refers
   to what is in the container rootfs at the time of the mount, including the
   mounts done before it, so it cannot be opened beforehand.  */
ساكن ثابت حرف *
get_path_in_rootfs (ثابت حرف *rootfs, ثابت حرف *path)
{
  size_t len;

  إذا (rootfs == عدم || path == عدم)
    إرجاع عدم;

  len = strlen (rootfs);
  بينما (len > 0 && rootfs[len - 1] == '/')
    len--;

  /* With "/" as the rootfs, every path is in it, and there is nothing to
     do differently.  */
  إذا (len == 0)
    إرجاع عدم;

  إذا (strncmp (path, rootfs, len) != 0 || (path[len] != '/' && path[len] != '\0'))
    إرجاع عدم;

  إرجاع consume_slashes (path + len);
}

ساكن صحيح
process_single_mount (libcrun_container_t *container, ثابت حرف *rootfs,
                      runtime_spec_schema_defs_mount *mount,
                      بنية libcrun_fd_map *mount_fds, size_t mount_index,
                      ثابت حرف *systemd_cgroup_v1, libcrun_error_t *err)
{
  ثابت حرف *target = consume_slashes (mount->destination);
  cleanup_close صحيح source_mountfd = -1;
  cleanup_free حرف *data = عدم;
  حرف *type;
  حرف *source;
  unsigned long flags = 0;
  unsigned long extra_flags = 0;
  mode_t src_mode = S_IFDIR;
  cleanup_close صحيح copy_from_fd = -1;
  cleanup_close صحيح targetfd = -1;
  bool mounted = false;
  bool is_sysfs_or_proc;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  صحيح no_remount = 0;
  صحيح ret;

  إذا (mount_fds)
    source_mountfd = get_and_reset (&(mount_fds->fds[mount_index]));

  type = mount->type;

  إذا (mount->options == عدم)
    flags = get_default_flags (container, mount->destination, &data);
  وإلا
    {
      size_t j;

      لكل (j = 0; j < mount->options_len; j++)
        flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  إذا (type == عدم && (flags & MS_BIND) == 0)
    إرجاع crun_make_error (err, 0, "invalid mount type for `%s`", mount->destination);

  إذا ((flags & MS_BIND) && source_mountfd < 0 && ! (extra_flags & OPTION_COPY_SYMLINK))
    {
      ثابت حرف *rel_source = get_path_in_rootfs (get_private_data (container)->host_rootfs, mount->source);

      إذا (rel_source)
        {
          unsigned long propagation = (flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE)) ? 0 : MS_PRIVATE;

          source_mountfd = get_bind_mount (get_private_data (container)->rootfsfd, rel_source, (flags & MS_REC) != 0,
                                           false, (extra_flags & OPTION_SRC_NOFOLLOW) != 0, propagation, err);
          إذا (UNLIKELY (source_mountfd < 0))
            إرجاع crun_error_wrap (err, "open mount source `%s`", mount->source);
        }
    }

  إذا ((flags & MS_BIND) && mount->source && has_mount_flag_options (mount)
      && (get_private_data (container)->unshare_flags & CLONE_NEWUSER))
    flags |= get_bind_mount_locked_flags (source_mountfd, mount->source, mount);

  إذا (flags & MS_BIND)
    {
      /* A remount sets exactly the requested flags and clears any other
         one.  A bind mount without mount flag options (e.g. "bind",
         "rbind,rprivate") keeps the flags of the source, so it must not
         be remounted.  */
      إذا (! has_mount_flag_options (mount))
        no_remount = MOUNT_NO_REMOUNT;

      إذا (path_is_slash_dev (mount->destination))
        get_private_data (container)->mount_dev_from_host = true;
      /* It is used only for error messages.  */
      type = "bind";
    }
  is_sysfs_or_proc = strcmp (type, "sysfs") == 0 || strcmp (type, "proc") == 0;

  إذا (strcmp (type, "tmpfs") == 0)
    {
      ret = append_tmpfs_mode_if_missing (container, mount, &data, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  إذا (mount->source && (flags & MS_BIND))
    {
      proc_fd_path_t proc_buf;
      ثابت حرف *path = mount->source;

      إذا ((extra_flags & OPTION_COPY_SYMLINK) && (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_DEST_NOFOLLOW)))
        إرجاع crun_make_error (err, 0, "`copy-symlink` is mutually exclusive with `src-nofollow` and `dest-nofollow`");

      bool is_cached_symlink = (extra_flags & OPTION_COPY_SYMLINK)
                               && get_private_data (container)->copy_symlink_targets
                               && mount_index < get_private_data (container)->n_copy_symlink_targets
                               && get_private_data (container)->copy_symlink_targets[mount_index];

      /* When the source is a cached symlink, skip the pre-opened fd since
         get_bind_mount() followed the symlink and the fd points to the target.
         For regular files with copy-symlink, the pre-opened fd is still valid.  */
      إذا (source_mountfd >= 0 && ! is_cached_symlink)
        {
          get_self_fd_path (proc_buf, source_mountfd);
          path = proc_buf;

          ret = get_file_type_at (source_mountfd, &src_mode, true, عدم);
        }
      وإلا إذا (is_cached_symlink)
        {
          src_mode = S_IFLNK;
          ret = 0;
        }
      وإلا إذا (path[0] == '/')
        {
          صحيح old_جذر_fd = get_old_جذر_fd (get_private_data (container));
          bool nofollow = (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false;

          إذا (old_جذر_fd >= 0)
            ret = get_file_type_at (old_جذر_fd, &src_mode, nofollow, (path[1] == '\0') ? عدم : path + 1);
          وإلا
            ret = get_file_type_at (AT_FDCWD, &src_mode, nofollow, path);
        }
      وإلا
        ret = get_file_type (&src_mode, (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false, path);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "cannot stat `%s`", path);

      إذا (S_ISLNK (src_mode) && (extra_flags & OPTION_DEST_NOFOLLOW) && source_mountfd < 0)
        {
          صحيح bind_dirfd = AT_FDCWD;
          ثابت حرف *bind_source = mount->source;
          صحيح old_جذر_fd = get_old_جذر_fd (get_private_data (container));

          إذا (old_جذر_fd >= 0 && mount->source && mount->source[0] == '/')
            {
              bind_dirfd = old_جذر_fd;
              bind_source = (mount->source[1] == '\0') ? "." : mount->source + 1;
            }
          ret = get_bind_mount (bind_dirfd, bind_source, true, true, extra_flags & OPTION_SRC_NOFOLLOW, MS_PRIVATE, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;

          source_mountfd = ret;
        }
    }

  إذا (S_ISLNK (src_mode) && (extra_flags & OPTION_COPY_SYMLINK))
    {
      ثابت حرف *cached_target = عدم;

      إذا (get_private_data (container)->copy_symlink_targets
          && mount_index < get_private_data (container)->n_copy_symlink_targets)
        cached_target = get_private_data (container)->copy_symlink_targets[mount_index];

      ret = handle_copy_symlink (container, rootfs, mount, cached_target, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      mounted = true;
    }
  وإلا إذا (is_sysfs_or_proc)
    {
      ret = prepare_sysfs_or_proc_mount (container, target, &targetfd, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }
  وإلا
    {
      bool is_dir = S_ISDIR (src_mode);

      إذا (extra_flags & OPTION_DEST_NOFOLLOW)
        {
          /* If dest-nofollow is specified, expect the target to exist.  */
          ret = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_PATH | O_NOFOLLOW | O_CLOEXEC, 0, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;
          targetfd = ret;

          ret = check_valid_no_follow_file_system (targetfd, target, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;
        }
      وإلا
        {
          /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
          ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;
          targetfd = ret;
        }
    }

  إذا (extra_flags & OPTION_TMPCOPYUP)
    {
      إذا (strcmp (type, "tmpfs") != 0)
        إرجاع crun_make_error (err, 0, "tmpcopyup can be used only with tmpfs");

      /* targetfd is opened with O_PATH, reopen the fd so it can read.  */
      copy_from_fd = openat (targetfd, ".", O_CLOEXEC | O_RDONLY | O_DIRECTORY);
      إذا (UNLIKELY (copy_from_fd < 0))
        {
          إذا (errno != ENOTDIR)
            إرجاع crun_make_error (err, errno, "cannot reopen `%s`", target);
        }
    }

  source = mount->source ? mount->source : type;

  /* Check if there is already a mount for the requested file system.  */
  إذا (! mounted && source_mountfd >= 0)
    {
      ret = fs_move_mount_to (source_mountfd, targetfd, عدم);
      إذا (LIKELY (ret == 0))
        {
          unsigned long remaining_flags = flags & ~MS_BIND;

          /* A remount is needed not only to set some flags, but also to
             clear the ones set on the source (e.g. "bind,dev" on a nodev
             source), in which case remaining_flags might be empty.  */
          إذا (remaining_flags || has_mount_flag_options (mount))
            {
              ret = do_mount (container, عدم, source_mountfd, target, عدم, remaining_flags, data, LABEL_NONE | no_remount, err);
              إذا (UNLIKELY (ret < 0))
                إرجاع ret;
            }

          mounted = true;

          إذا (is_empty_string (target))
            {
              صحيح new_rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
              إذا (UNLIKELY (new_rootfsfd < 0))
                إرجاع crun_make_error (err, errno, "reopen rootfs after mount on /");

              TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
              get_private_data (container)->rootfsfd = new_rootfsfd;
            }
        }
      وإلا إذا (get_private_data (container)->delayed_pidns_fork && strcmp (type, "proc") == 0)
        {
          /* The process is not in the container PID namespace yet, so
             mount(2) would create a proc instance for the wrong one.  */
          إرجاع crun_make_error (err, 0, "the proc mount `%s` was not opened in the target PID namespace", target);
        }
      وإلا إذا (get_private_data (container)->host_جذر_switched)
        {
          /* The host root is unreachable now -- even through a fd, since
             mount(2) requires the source to live in the current mount
             namespace.  The path-based fallback below would resolve SOURCE
             inside the container and bind the wrong tree.  */
          إرجاع crun_make_error (err, errno, "move_mount `%s` to `%s`", source, target);
        }
      وإلا إذا (errno != ENOSYS)
        {
          /* ENOSYS only means the kernel lacks the new mount API.  Anything
             else is worth reporting: it is otherwise invisible, and it decides
             whether the mount is set up from the pre-opened fd or from the
             source path.  */
          libcrun_warning ("move_mount `%s` to `%s` failed, falling back to mount(2): %s",
                           source, target, strerror (errno));
        }
    }

  إذا (! mounted)
    {
      إذا (systemd_cgroup_v1 && strcmp (mount->destination, systemd_cgroup_v1) == 0)
        {
          /* Override the cgroup mount with a single named cgroup name=systemd.  */
          ret = do_mount_cgroup_systemd_v1 (container, source, targetfd, target, flags, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;
        }
      وإلا إذا (strcmp (type, "cgroup") == 0)
        {
          ret = do_mount_cgroup (container, source, targetfd, target, flags, data, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;
        }
      وإلا
        {
          صحيح extra_flags = get_mount_label_how (type, is_sysfs_or_proc) | no_remount;

          ret = do_mount (container, source, targetfd, target, type, flags, data, extra_flags, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;
        }
    }

  إذا (copy_from_fd >= 0)
    {
      // handle_tmpcopyup closes copy_from_fd
      ret = handle_tmpcopyup (container, rootfs, target, copy_from_fd, err);
      copy_from_fd = -1;
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  إذا (rec_clear || rec_set)
    {
      ثابت bool is_dir = S_ISDIR (src_mode);
      cleanup_close صحيح dfd = -1;

      dfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_RDONLY | O_PATH | O_CLOEXEC | (is_dir ? O_DIRECTORY : 0), 0, err);
      إذا (UNLIKELY (dfd < 0))
        إرجاع crun_error_wrap (err, "open mount target `/%s`", target);

      ret = do_mount_setattr (true, target, dfd, rec_clear, rec_set, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  إرجاع 0;
}

ساكن صحيح
do_mounts (libcrun_container_t *container, ثابت حرف *rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ثابت حرف *systemd_cgroup_v1 = get_force_cgroup_v1_annotation (container);
  cleanup_close_map بنية libcrun_fd_map *mount_fds = عدم;
  size_t i;
  صحيح ret;

  mount_fds = get_private_data (container)->mount_fds;
  get_private_data (container)->mount_fds = عدم;

  لكل (i = 0; i < def->mounts_len; i++)
    {
      ret = process_single_mount (container, rootfs, def->mounts[i], mount_fds, i, systemd_cgroup_v1, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }
  إرجاع 0;
}

/*
 * libcrun_container_do_bind_mount
 *
 *  Allows external plugins and handlers to perform bind `mounts` on container.
 *  returns: 0 if successful anything else states `error` and configures `err` with relevant error.
 */
صحيح
libcrun_container_do_bind_mount (libcrun_container_t *container, حرف *mount_source, حرف *mount_destination, حرف **mount_options, size_t mount_options_len, libcrun_error_t *err)
{
  ثابت حرف *target = consume_slashes (mount_destination);
  cleanup_free حرف *data = عدم;
  unsigned long flags = 0;
  unsigned long extra_flags = 0;
  cleanup_close صحيح targetfd = -1;
  صحيح is_dir = 1;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  ثابت حرف *rootfs = get_private_data (container)->rootfs;
  صحيح ret;

  إذا ((get_private_data (container)->rootfsfd < 0) || (rootfs == عدم))
    إرجاع crun_make_error (err, 0, "invalid rootfs state while performing bind mount from external plugin or handler");

  إذا (mount_options == عدم)
    flags = get_default_flags (container, mount_destination, &data);
  وإلا
    {
      size_t j;

      لكل (j = 0; j < mount_options_len; j++)
        flags |= get_mount_flags_or_option (mount_options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  إذا (path_is_slash_dev (mount_destination))
    get_private_data (container)->mount_dev_from_host = true;

  إذا (mount_source && (flags & MS_BIND))
    {
      is_dir = crun_dir_p (mount_source, false, err);
      إذا (UNLIKELY (is_dir < 0))
        إرجاع is_dir;
    }

  /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
  ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  targetfd = ret;

  ret = do_mount (container, mount_source, targetfd, target, "bind", flags, data, LABEL_MOUNT, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  إرجاع 0;
}

/*
  Open a fd to the NOTIFY_SOCKET end on the host.  If CONTAINER is NULL, then CONTEXT
  is used to retrieve the path to the socket.
*/
صحيح
get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, صحيح *notify_socket_out, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  cleanup_close صحيح notify_fd = -1;
  cleanup_free حرف *host_notify_socket_path = عدم;
  cleanup_free حرف *state_dir = عدم;
  حرف *host_path = عدم;
  صحيح ret;

  إذا (container && get_private_data (container)->host_notify_socket_path)
    {
      ثابت حرف *parent_dir;

      parent_dir = get_private_data (container)->host_notify_socket_path;

      ret = append_paths (&host_notify_socket_path, err, parent_dir, "notify", عدم);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      host_path = host_notify_socket_path;
    }

  *notify_socket_out = -1;

  إذا (host_path == عدم)
    {
      ret = libcrun_get_state_directory (&state_dir, context->state_جذر, context->id, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      ret = append_paths (&host_notify_socket_path, err, state_dir, "notify/notify", عدم);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      host_path = host_notify_socket_path;
    }

  notify_fd = open_unix_domain_socket (host_path, 1, err);
  إذا (UNLIKELY (notify_fd < 0))
    إرجاع notify_fd;

  إذا (UNLIKELY (chmod (host_path, 0777) < 0))
    إرجاع crun_make_error (err, errno, "chmod `%s`", host_path);

#  ifdef HAVE_FGETXATTR
  إذا (container && container->container_def->linux && container->container_def->linux->mount_label)
    {
      /* Ignore the error. The worst that can happen is that the container fails to notify it is ready.  */
      (فراغ) setxattr (host_path, "security.selinux", container->container_def->linux->mount_label,
                       strlen (container->container_def->linux->mount_label), 0);
    }
#  endif

  *notify_socket_out = get_and_reset (&notify_fd);
  إرجاع 1;
#وإلا
  (فراغ) context;
  (فراغ) container;
  (فراغ) err;
  *notify_socket_out = -1;
  إرجاع 0;
#endif
}

#ifdef HAVE_SYSTEMD
ساكن صحيح
do_notify_socket (libcrun_container_t *container, libcrun_error_t *err)
{
  صحيح ret;
  ثابت حرف *notify_socket = container->context->notify_socket;
  cleanup_free حرف *host_notify_socket_path = عدم;
  cleanup_free حرف *container_notify_socket_path = عدم;
  cleanup_free حرف *state_dir = عدم;
  uid_t container_جذر_uid = -1;
  gid_t container_جذر_gid = -1;
  صحيح notify_socket_شجرة_fd;

  إذا (notify_socket == عدم)
    إرجاع 0;

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_جذر : عدم),
                                     (container->context ? container->context->id : عدم), err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  ret = append_paths (&container_notify_socket_path, err, "/", notify_socket, "notify", عدم);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  ret = append_paths (&host_notify_socket_path, err, state_dir, "notify", عدم);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  ret = mkdir (host_notify_socket_path, 0700);
  إذا (ret < 0)
    إرجاع crun_make_error (err, errno, "mkdir `%s`", host_notify_socket_path);

  إذا (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
    {
      get_جذر_in_the_userns (container->container_def, 0, 0, &container_جذر_uid, &container_جذر_gid);
      إذا (container_جذر_uid != ((uid_t) -1) && container_جذر_gid != ((gid_t) -1))
        {
          ret = chown (host_notify_socket_path, container_جذر_uid, container_جذر_gid);
          إذا (UNLIKELY (ret < 0))
            إرجاع crun_make_error (err, errno, "chown `%s` to `%d:%d`", host_notify_socket_path, container_جذر_uid, container_جذر_gid);
        }
    }

  notify_socket_شجرة_fd = syscall_open_شجرة (AT_FDCWD, host_notify_socket_path, OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
  إذا (notify_socket_شجرة_fd >= 0)
    /* open_tree worked */
    get_private_data (container)->notify_socket_شجرة_fd = notify_socket_شجرة_fd;
  وإلا إذا (errno == EPERM)
    /* this can happen when trying to run a rootless container; this function is called
       in the original namespace where the caller is _not_ CAP_SYS_ADMIN - in that case,
       do nothing, because the bind mount of host_notify_socket_path directly should succeed
       since it will be readable by the container user. */
    ;
  وإلا إذا (errno == ENOSYS)
    /* if open_tree(2) is not available, do nothing; we will try mount(2) in do_finalize_notify_socket */
    ;
  وإلا
    /* some other error */
    إرجاع crun_make_error (err, errno, "open_tree `%s`", host_notify_socket_path);

  get_private_data (container)->host_notify_socket_path = host_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = container_notify_socket_path;
  host_notify_socket_path = container_notify_socket_path = عدم;
  إرجاع 0;
}
#endif

/* Bind mount the notify socket directory into the rootfs.  It must run
   before pivot_root, while the host path is still reachable for the
   mount(2) fallback, and the destination is resolved inside the rootfs so
   that a symlink in the container image cannot point it elsewhere.  */
ساكن صحيح
do_finalize_notify_socket (libcrun_container_t *container, ثابت حرف *rootfs, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_free حرف *host_notify_socket_path = عدم;
  cleanup_free حرف *container_notify_socket_path = عدم;
  cleanup_free حرف *container_notify_socket_path_dir_alloc = عدم;
  ثابت حرف *container_notify_socket_path_dir = عدم;
  cleanup_close صحيح notify_socket_شجرة_fd = -1;
  cleanup_close صحيح targetfd = -1;
  صحيح did_mount_with_move_mount = 0;

  host_notify_socket_path = get_private_data (container)->host_notify_socket_path;
  get_private_data (container)->host_notify_socket_path = عدم;

  container_notify_socket_path = get_private_data (container)->container_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = عدم;

  إذا (host_notify_socket_path == عدم || container_notify_socket_path == عدم)
    إرجاع 0;

  إذا (rootfs == عدم)
    rootfs = "/";

  container_notify_socket_path_dir_alloc = xstrdup (container_notify_socket_path);
  container_notify_socket_path_dir = consume_slashes (dirname (container_notify_socket_path_dir_alloc));

  /* libcrun_set_mounts does not open the rootfs when there are no mounts.  */
  إذا (get_private_data (container)->rootfsfd < 0)
    {
      ret = open (rootfs, O_PATH | O_CLOEXEC);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "open `%s`", rootfs);

      get_private_data (container)->rootfsfd = ret;
      get_private_data (container)->rootfs = rootfs;
    }

  ret = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs,
                                          container_notify_socket_path_dir, 0755, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  targetfd = ret;

  notify_socket_شجرة_fd = get_private_data (container)->notify_socket_شجرة_fd;
  /* the FD will be unconditionally closed at the end of this function due to cleanup_close above */
  get_private_data (container)->notify_socket_شجرة_fd = -1;

  إذا (notify_socket_شجرة_fd >= 0)
    {
      ret = fs_move_mount_to (notify_socket_شجرة_fd, targetfd, عدم);
      إذا (ret >= 0)
        {
          /* if move_mount(2) worked, make sure we don't try mount(2) */
          did_mount_with_move_mount = 1;

          /* the tree was cloned from the host mount namespace and is still in
             the peer group of its source, so make it private like the mount(2)
             fallback below does.  TARGETFD is now underneath the new mount,
             so reopen the target.  */
          close_and_reset (&targetfd);
          targetfd = open_mount_target (container, container_notify_socket_path_dir, err);
          إذا (UNLIKELY (targetfd < 0))
            إرجاع targetfd;

          ret = do_mount (container, عدم, targetfd, container_notify_socket_path_dir, عدم,
                          MS_REC | MS_PRIVATE, عدم, LABEL_NONE, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;
        }
      وإلا إذا (errno == ENOSYS)
        /* do nothing; we will try mount(2) next */
        ;
      وإلا
        إرجاع crun_make_error (err, errno, "move_mount `%d` to `%s`", notify_socket_شجرة_fd,
                                container_notify_socket_path_dir);
    }

  إذا (! did_mount_with_move_mount)
    {
      ret = do_mount (container, host_notify_socket_path, targetfd, container_notify_socket_path_dir, عدم,
                      MS_BIND | MS_REC | MS_PRIVATE, عدم, LABEL_MOUNT, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  إرجاع 0;
}

ساكن صحيح
make_parent_mount_private (ثابت حرف *rootfs, libcrun_error_t *err)
{
  cleanup_close صحيح rootfsfd = -1;
  بنية stat prev_st;
  proc_fd_path_t proc_path;

  /* rootfs could be a relative path.  */
  rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
  إذا (UNLIKELY (rootfsfd < 0))
    إرجاع crun_make_error (err, errno, "open `%s`", rootfs);

  إذا (UNLIKELY (fstat (rootfsfd, &prev_st) < 0))
    إرجاع crun_make_error (err, errno, "fstat `%s`", rootfs);

  لكل (;;)
    {
      بنية stat cur_st;
      libcrun_error_t tmp_err = عدم;
      صحيح ret;
      cleanup_close صحيح parentfd = -1;

      ret = do_mount_setattr (false, rootfs, rootfsfd, 0, MS_PRIVATE, &tmp_err);
      إذا (ret == 0)
        إرجاع 0;
      crun_error_release (&tmp_err);

      get_proc_self_fd_path (proc_path, rootfsfd);
      ret = mount (عدم, proc_path, عدم, MS_PRIVATE, عدم);
      إذا (ret == 0)
        إرجاع 0;

      parentfd = openat (rootfsfd, "..", O_PATH | O_CLOEXEC);
      إذا (parentfd < 0)
        {
          صحيح saved_errno = errno;
          ret = faccessat (rootfsfd, "..", X_OK, AT_EACCESS);
          إذا (ret != 0)
            إرجاع crun_make_error (err, EACCES, "make `%s` private: a component is not accessible", rootfs);
          إرجاع crun_make_error (err, saved_errno, "make `%s` private: cannot open component", rootfs);
        }

      إذا (UNLIKELY (fstat (parentfd, &cur_st) < 0))
        إرجاع crun_make_error (err, errno, "fstat parent of `%s`", rootfs);

      /* Reached the root of the file system: ".." points to itself.  */
      إذا (cur_st.st_dev == prev_st.st_dev && cur_st.st_ino == prev_st.st_ino)
        اكسر;

      prev_st = cur_st;
      close_and_reset (&rootfsfd);
      rootfsfd = get_and_reset (&parentfd);
    }

  إرجاع crun_make_error (err, 0, "make `%s` private", rootfs);
}

صحيح
libcrun_set_mounts (بنية container_entrypoint_s *entrypoint_args, libcrun_container_t *container, ثابت حرف *rootfs, set_mounts_cb_t cb, فراغ *cb_data, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح is_user_ns = 0;
  صحيح ret = 0;

  إذا (rootfs == عدم || def->mounts == عدم)
    إرجاع 0;

  ret = open (rootfs, O_PATH | O_CLOEXEC);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "open `%s`", rootfs);

  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = rootfs;

  // configure handler mounts
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_MOUNTS, container, rootfs, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_MOUNTS");

  إذا (def->جذر->readonly)
    {
      بنية remount_s *r;
      unsigned long remount_flags = MS_REMOUNT | MS_BIND | MS_RDONLY;
      صحيح fd;

      fd = dup (get_private_data (container)->rootfsfd);
      إذا (UNLIKELY (fd < 0))
        إرجاع crun_make_error (err, errno, "dup fd for `%s`", rootfs);

      r = make_remount (fd, rootfs, remount_flags, عدم, get_private_data (container)->remounts);
      get_private_data (container)->remounts = r;
    }

  /* Mounts are already done before pivot_root in setup_mount_namespace.  */

  is_user_ns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER);
  إذا (! is_user_ns)
    {
      is_user_ns = check_running_in_user_namespace (err);
      إذا (UNLIKELY (is_user_ns < 0))
        إرجاع is_user_ns;
    }

  إذا (! get_private_data (container)->mount_dev_from_host)
    {
      ret = create_missing_devs (container, is_user_ns ? true : false, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  /* Notify the callback after all the mounts are ready but before making them read-only.  */
  إذا (cb)
    {
      ret = cb (cb_data, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  إذا (def->process && def->process->cwd)
    {
      libcrun_error_t tmp_err = عدم;
      ثابت حرف *rel_cwd = consume_slashes (def->process->cwd);
      /* Ignore errors here and let it fail later.  */
      (فراغ) crun_safe_ensure_directory_at (get_private_data (container)->rootfsfd, rootfs, rel_cwd, 0755, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ret = do_masked_and_readonly_paths (container, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  إرجاع 0;
}

ساكن صحيح
umount_or_hide (ثابت حرف *target, libcrun_error_t *err)
{
  صحيح ret;

  ret = umount2 (target, MNT_DETACH);
  إذا (UNLIKELY (ret < 0))
    {
      صحيح saved_errno = errno;

      /* If the umount2 failed with EINVAL then the mount could
         be locked.  Hide it by mounting a tmpfs on top of it.  */
      إذا (errno == EINVAL)
        {
          ret = mount (عدم, target, "tmpfs", 0, "size=0k");
          إذا (LIKELY (ret == 0))
            إرجاع 0;
        }

      إرجاع crun_make_error (err, saved_errno, "umount `%s`", target);
    }

  إرجاع ret;
}

ساكن صحيح
move_جذر (ثابت حرف *rootfs, libcrun_error_t *err)
{
  صحيح ret;

  ret = chdir (rootfs);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "chdir to `%s`", rootfs);

  ret = umount_or_hide ("/sys", err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  ret = umount_or_hide ("/proc", err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  ret = mount (rootfs, "/", "", MS_MOVE, "");
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "mount MS_MOVE to `/`");

  ret = chroot (".");
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "chroot to `%s`", rootfs);

  ret = chdir ("/");
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "chdir to `%s`", rootfs);

  إرجاع 0;
}

صحيح
libcrun_finalize_mounts (بنية container_entrypoint_s *entrypoint_args, libcrun_container_t *container, ثابت حرف *rootfs,
                         set_mounts_cb_t after_pivot_cb, فراغ *cb_data, libcrun_error_t *err)
{
  صحيح ret;

  // configure handler mounts for phase: HANDLER_CONFIGURE_AFTER_MOUNTS
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_AFTER_MOUNTS, container, rootfs, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_AFTER_MOUNTS");

  ret = do_finalize_notify_socket (container, rootfs, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  close_and_reset (&(get_private_data (container)->rootfsfd));

  /* The host root must not stay reachable through /proc/self/fd once the
     container rootfs becomes the root: every path resolved by name from now
     on is controlled by the container image.  */
  close_and_reset (&(get_private_data (container)->old_جذر_fd));

  إذا (get_private_data (container)->needs_pivot)
    {
      get_private_data (container)->needs_pivot = false;

      إذا (get_private_data (container)->no_pivot)
        {
          ret = move_جذر (rootfs, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;
        }
      وإلا
        {
          ret = do_pivot (container, rootfs, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;
        }

      ret = do_mount (container, عدم, -1, "/", عدم,
                      get_private_data (container)->rootfs_propagation,
                      عدم, LABEL_MOUNT, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      ret = chdir ("/");
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "chdir to `/`");
    }

  إذا (after_pivot_cb)
    {
      ret = after_pivot_cb (cb_data, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  ret = finalize_mounts (container, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  إرجاع 0;
}

ساكن صحيح
maybe_open_شجرة_namespace (ثابت حرف *rootfs, صحيح *out_fd, libcrun_error_t *err)
{
  cleanup_close صحيح rootfs_fd = -1;
  صحيح شجرة_fd;

  *out_fd = -1;

  rootfs_fd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  إذا (UNLIKELY (rootfs_fd < 0))
    إرجاع crun_make_error (err, errno, "open `%s`", rootfs);

  شجرة_fd = syscall_open_شجرة (rootfs_fd, "",
                               OPEN_شجرة_NAMESPACE | OPEN_TREE_CLOEXEC
                                   | AT_EMPTY_PATH | AT_RECURSIVE);
  إذا (شجرة_fd < 0)
    {
      إذا (errno == EINVAL || errno == ENOSYS || errno == EPERM)
        إرجاع 0;
      إرجاع crun_make_error (err, errno, "open_tree `%s`", rootfs);
    }

  *out_fd = شجرة_fd;
  إرجاع 0;
}

ساكن بنية libcrun_fd_map *
get_fd_map (libcrun_container_t *container)
{
  بنية libcrun_fd_map *mount_fds = get_private_data (container)->mount_fds;

  إذا (mount_fds == عدم)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      mount_fds = make_libcrun_fd_map (def->mounts_len);
      get_private_data (container)->mount_fds = mount_fds;
    }
  إرجاع mount_fds;
}

ساكن صحيح
open_mount_of_type (libcrun_container_t *container,
                    runtime_spec_schema_defs_mount *mount,
                    libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free حرف *mnt_data = عدم;
  unsigned long mnt_flags = 0, mnt_extra = 0;
  uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
  ثابت حرف *label_type = عدم;
  ثابت حرف *label_val = عدم;
  ثابت حرف *fstype;
  صحيح mnt_fd;
  size_t j;

  إذا (mount->type == عدم)
    إرجاع -1;

  fstype = mount->type;

  لكل (j = 0; j < mount->options_len; j++)
    mnt_flags |= get_mount_flags_or_option (mount->options[j],
                                            mnt_flags, &mnt_extra, &mnt_data,
                                            &mnt_rec_clear, &mnt_rec_set);

  إذا (def->linux && def->linux->mount_label)
    {
      bool is_sysfs_or_proc = strcmp (fstype, "sysfs") == 0
                              || strcmp (fstype, "proc") == 0;
      صحيح extra_flags = get_mount_label_how (fstype, is_sysfs_or_proc);
      إذا (extra_flags == LABEL_MOUNT)
        {
          label_type = get_selinux_context_type (container, err);
          إذا (label_type == عدم)
            crun_error_release (err);
          وإلا
            label_val = def->linux->mount_label;
        }
    }

  إذا (strcmp (fstype, "cgroup") == 0)
    {
      صحيح cgroup_mode = libcrun_get_cgroup_mode (err);
      إذا (cgroup_mode == CGROUP_MODE_UNIFIED)
        fstype = "cgroup2";
      crun_error_release (err);
    }

  mnt_fd = fsopen_mount (fstype, mount->source, label_type, label_val, mnt_data);
  إذا (mnt_fd < 0 && label_type)
    mnt_fd = fsopen_mount (fstype, mount->source, عدم, عدم, mnt_data);

  إرجاع mnt_fd;
}

#ifndef LSMT_ROOT
#  define LSMT_ROOT 0xffffffffffffffffULL
#endif
#ifndef STATMOUNT_FS_TYPE
#  define STATMOUNT_FS_TYPE 0x00000020U
#endif
#ifndef STATMOUNT_MNT_ROOT
#  define STATMOUNT_MNT_ROOT 0x00000040U
#endif
#ifndef STATMOUNT_MNT_POINT
#  define STATMOUNT_MNT_POINT 0x00000080U
#endif

/* Declared here so that the build does not depend on the UAPI headers being
   recent enough; the layout is stable.  */
بنية mnt_id_req_s
{
  uint32_t size;
  uint32_t spare;
  uint64_t mnt_id;
  uint64_t param;
  uint64_t mnt_ns_id;
};

بنية statmount_s
{
  uint32_t size;
  uint32_t mnt_opts;
  uint64_t mask;
  uint32_t sb_dev_major;
  uint32_t sb_dev_minor;
  uint64_t sb_magic;
  uint32_t sb_flags;
  uint32_t fs_type;
  uint64_t mnt_id;
  uint64_t mnt_parent_id;
  uint32_t mnt_id_old;
  uint32_t mnt_parent_id_old;
  uint64_t mnt_attr;
  uint64_t mnt_propagation;
  uint64_t mnt_peer_group;
  uint64_t mnt_master;
  uint64_t propagate_from;
  uint32_t mnt_root;
  uint32_t mnt_point;
  uint64_t mnt_ns_id;
  uint64_t __spare2[49];
  حرف str[];
};

ساكن صحيح
syscall_listmount (بنية mnt_id_req_s *req, uint64_t *ids, size_t nr, unsigned صحيح flags)
{
#ifdef __NR_listmount
  إرجاع (صحيح) syscall (__NR_listmount, req, ids, nr, flags);
#وإلا
  (فراغ) req;
  (فراغ) ids;
  (فراغ) nr;
  (فراغ) flags;
  errno = ENOSYS;
  إرجاع -1;
#endif
}

ساكن صحيح
syscall_statmount (بنية mnt_id_req_s *req, بنية statmount_s *buf, size_t size, unsigned صحيح flags)
{
#ifdef __NR_statmount
  إرجاع (صحيح) syscall (__NR_statmount, req, buf, size, flags);
#وإلا
  (فراغ) req;
  (فراغ) buf;
  (فراغ) size;
  (فراغ) flags;
  errno = ENOSYS;
  إرجاع -1;
#endif
}

/* move_mount(2) refuses to attach a tree that contains a bind mount of a mount
   namespace: check_for_nsfs_mounts() in the kernel rejects it with ELOOP.  A
   recursive clone of such a source picks one up, and there is no way back:
   once setns() has run the host tree is unreachable both by path and through a
   fd saved beforehand, since mount(2) wants the source to live in the current
   mount namespace.  snapd pins mount namespaces under /run/snapd/ns, so a
   container binding the host root hits this on any Ubuntu host.

   Return true when a mount namespace is pinned at or below the source of a
   bind mount.  listmount(2) and statmount(2) are used instead of parsing
   /proc/self/mountinfo, which escapes characters such as spaces in the paths
   being compared; they predate OPEN_TREE_NAMESPACE, so they are available
   wherever this code path can be taken at all.  */
ساكن bool
mounts_hit_pinned_mount_namespace (runtime_spec_schema_config_schema *def)
{
  cleanup_free uint64_t *ids = عدم;
  size_t i, n_ids = 0, capacity = 128;
  bool has_absolute_bind = false;
  uint64_t last = 0;

  لكل (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false, nofollow = false;

      إذا (is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && def->mounts[i]->source != عدم && def->mounts[i]->source[0] == '/')
        {
          has_absolute_bind = true;
          اكسر;
        }
    }
  إذا (! has_absolute_bind)
    إرجاع false;

  ids = xmalloc (capacity * حجم (uint64_t));
  لكل (;;)
    {
      بنية mnt_id_req_s req = {
        .size = حجم (req),
        .mnt_id = LSMT_ROOT,
        .param = last,
      };
      صحيح ret;

      إذا (n_ids == capacity)
        {
          capacity *= 2;
          ids = xrealloc (ids, capacity * حجم (uint64_t));
        }

      ret = syscall_listmount (&req, ids + n_ids, capacity - n_ids, 0);
      إذا (ret < 0)
        /* Without the mount table there is no way to tell; the kernel would
           reject the move_mount() anyway, so stay on the traditional path.  */
        إرجاع true;
      إذا (ret == 0)
        اكسر;

      n_ids += (size_t) ret;
      last = ids[n_ids - 1];
    }

  لكل (i = 0; i < n_ids; i++)
    {
      حرف buffer[4096] __attribute__ ((aligned (8)));
      بنية statmount_s *sm = (بنية statmount_s *) buffer;
      بنية mnt_id_req_s req = {
        .size = حجم (req),
        .mnt_id = ids[i],
        .param = STATMOUNT_FS_TYPE | STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT,
      };
      ثابت حرف *جذر, *point;
      size_t j;

      إذا (syscall_statmount (&req, sm, حجم (buffer), 0) < 0)
        استمر;
      إذا ((sm->mask & (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT)) != (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT))
        استمر;

      /* The root of a pinned mount namespace reads as "mnt:[...]"; the other
         namespace types cannot create a loop.  */
      جذر = sm->str + sm->mnt_root;
      إذا (! has_prefix (جذر, "mnt:["))
        استمر;

      point = sm->str + sm->mnt_point;

      لكل (j = 0; j < def->mounts_len; j++)
        {
          bool recursive = false, nofollow = false;
          ثابت حرف *source = def->mounts[j]->source;
          size_t source_len;

          إذا (! is_bind_mount (def->mounts[j], &recursive, &nofollow))
            استمر;
          إذا (source == عدم || source[0] != '/')
            استمر;

          source_len = strlen (source);
          بينما (source_len > 1 && source[source_len - 1] == '/')
            source_len--;

          إذا (strncmp (point, source, source_len) != 0)
            استمر;

          /* The source itself is a pinned mount namespace.  */
          إذا (point[source_len] == '\0')
            إرجاع true;

          /* A recursive bind clones the subtree below the source as well, so
             a pin anywhere under it is picked up.  A plain bind clones the
             single mount, and cannot pick one up.  */
          إذا (recursive && (source_len == 1 || point[source_len] == '/'))
            إرجاع true;
        }
    }

  إرجاع false;
}

ساكن bool
can_use_open_شجرة_namespace (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  بنية libcrun_fd_map *mount_fds;
  bool has_hooks = def->hooks
                   && (def->hooks->prestart_len || def->hooks->create_runtime_len
                       || def->hooks->create_container_len);
  bool has_userns = get_private_data (container)->unshare_flags & CLONE_NEWUSER;
  size_t i;

  إذا (has_hooks || has_userns)
    إرجاع false;

  إذا (mounts_hit_pinned_mount_namespace (def))
    إرجاع false;

  إذا (container->context->no_open_شجرة_namespace)
    إرجاع false;

  {
    libcrun_error_t tmp_err = عدم;
    صحيح in_userns = check_running_in_user_namespace (&tmp_err);
    crun_error_release (&tmp_err);
    إذا (in_userns != 0)
      إرجاع false;
  }

  mount_fds = get_fd_map (container);
  لكل (i = 0; i < def->mounts_len; i++)
    {
      إذا (mount_fds->fds[i] < 0)
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];
          إذا (! has_cached_target)
            إرجاع false;
        }
    }

  إرجاع true;
}

/* Only tmpfs mounts inherit the mode of the directory they are mounted
   over, and only when the mount does not already request an explicit mode=
   option.  This mirrors append_tmpfs_mode_if_missing() used by the fallback
   mount() path.  File systems such as proc, sysfs and mqueue have a root
   inode mode defined by the kernel that must not be overridden.  */
ساكن bool
mount_inherits_mountpoint_mode (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  إذا (mount->type == عدم || strcmp (mount->type, "tmpfs") != 0)
    إرجاع false;

  لكل (i = 0; i < mount->options_len; i++)
    إذا (mount->options[i] != عدم && has_prefix (mount->options[i], "mode="))
      إرجاع false;

  إرجاع true;
}

ساكن صحيح
setup_mount_namespace (libcrun_container_t *container, bool no_pivot, حرف **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  unsigned long rootfs_propagation = 0;
  cleanup_close صحيح rootfsfd = -1;
  cleanup_close صحيح شجرة_fd = -1;
  libcrun_error_t tmp_err = عدم;
  bool use_open_شجرة = false;
  size_t i;
  صحيح ret;

  إذا (def->linux && def->linux->rootfs_propagation)
    rootfs_propagation = get_mount_flags (def->linux->rootfs_propagation, 0, عدم, عدم, عدم, عدم);

  إذا ((rootfs_propagation & ALL_PROPAGATIONS_NO_REC) == 0)
    rootfs_propagation = MS_REC | MS_PRIVATE;

  get_private_data (container)->rootfs_propagation = rootfs_propagation;

  ret = libcrun_container_enter_cgroup_ns (container, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  حرر (get_private_data (container)->host_rootfs);
  get_private_data (container)->host_rootfs = xstrdup (*rootfs);

  /* Pre-create mounts and cache paths before pivot_root,
     while the host file system is still reachable.  */
  لكل (i = 0; i < def->mounts_len; i++)
    {
      بنية libcrun_fd_map *mount_fds = get_fd_map (container);
      cleanup_close صحيح mnt_fd = -1;
      bool recursive_bind, nofollow;
      unsigned long mnt_flags = 0;

      إذا (mount_fds && mount_fds->fds[i] >= 0)
        استمر;

      /* Pre-read symlink targets for copy-symlink mounts while the host file system is still reachable.  */
      إذا (def->mounts[i]->source != عدم
          && def->mounts[i]->source[0] == '/')
        {
          cleanup_free حرف *mnt_data = عدم;
          unsigned long mnt_extra = 0;
          uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
          size_t j;

          لكل (j = 0; j < def->mounts[i]->options_len; j++)
            mnt_flags |= get_mount_flags_or_option (def->mounts[i]->options[j],
                                                    mnt_flags, &mnt_extra, &mnt_data,
                                                    &mnt_rec_clear, &mnt_rec_set);

          إذا (mnt_extra & OPTION_COPY_SYMLINK)
            {
              cleanup_free حرف *target = عدم;
              libcrun_error_t tmp_err = عدم;
              ssize_t len;

              len = safe_readlinkat (AT_FDCWD, def->mounts[i]->source, &target, 0, &tmp_err);
              إذا (UNLIKELY (len < 0))
                crun_error_release (&tmp_err);

              إذا (get_private_data (container)->copy_symlink_targets == عدم)
                {
                  get_private_data (container)->copy_symlink_targets
                      = xmalloc0 (def->mounts_len * حجم (حرف *));
                  get_private_data (container)->n_copy_symlink_targets = def->mounts_len;
                }
              get_private_data (container)->copy_symlink_targets[i] = target;
              target = عدم;
            }
        }

      إذا (is_bind_mount (def->mounts[i], &recursive_bind, &nofollow))
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];

          إذا (mount_fds && def->mounts[i]->source != عدم && ! has_cached_target
              && get_path_in_rootfs (*rootfs, def->mounts[i]->source) == عدم)
            {
              libcrun_error_t tmp_err = عدم;
              unsigned long propagation = MS_PRIVATE;

              إذا (mnt_flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE))
                propagation = 0;

              mount_fds->fds[i] = get_bind_mount (AT_FDCWD, def->mounts[i]->source,
                                                  recursive_bind, false, nofollow,
                                                  propagation, &tmp_err);
              إذا (mount_fds->fds[i] < 0)
                crun_error_release (&tmp_err);
            }
          استمر;
        }

      mnt_fd = open_mount_of_type (container, def->mounts[i], err);
      إذا (mnt_fd < 0)
        {
          crun_error_release (err);
          استمر;
        }

      إذا (mount_inherits_mountpoint_mode (def->mounts[i]))
        {
          cleanup_close صحيح destfd = -1;
          بنية stat st;

          إذا (rootfsfd < 0)
            {
              rootfsfd = open (*rootfs, O_PATH | O_CLOEXEC);
              إذا (UNLIKELY (rootfsfd < 0))
                إرجاع crun_make_error (err, errno, "open `%s`", *rootfs);
            }

          destfd = safe_openat (rootfsfd, *rootfs, def->mounts[i]->destination, O_PATH | O_CLOEXEC, 0, &tmp_err);
          إذا (destfd < 0)
            crun_error_release (&tmp_err);
          وإلا إذا (fstat (destfd, &st) == 0)
            {
              proc_fd_path_t fd_path;
              صحيح procfd = get_procfd (get_private_data (container), err);
              إذا (UNLIKELY (procfd < 0))
                إرجاع procfd;

              get_self_fd_path (fd_path, mnt_fd);
              ret = fchmodat (procfd, fd_path, st.st_mode & 07777, 0);
              (فراغ) ret;
            }
        }

      mount_fds->fds[i] = mnt_fd;
      mnt_fd = -1;
    }
  close_and_reset (&rootfsfd);

  ret = get_shared_empty_dir_cached (container, عدم, &tmp_err);
  إذا (ret < 0)
    {
      crun_error_release (&tmp_err);
      get_private_data (container)->maskdir_bind_failed = true;
    }

  إذا (! get_private_data (container)->joined_mount_ns
      && can_use_open_شجرة_namespace (container))
    use_open_شجرة = true;

  إذا (use_open_شجرة)
    {
      ret = maybe_open_شجرة_namespace (*rootfs, &شجرة_fd, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  /* Pre-open needed device fds and the notify socket while the host
     file system is still reachable.  After setns(OPEN_TREE_NAMESPACE)
     the host /dev is no longer accessible.  When tree_fd < 0 (no
     OPEN_TREE_NAMESPACE), this is retried after unshare(CLONE_NEWNS)
     below since open_tree needs CAP_SYS_ADMIN in the user namespace
     that owns the mount namespace.  */
  إذا (شجرة_fd >= 0)
    preopen_needed_devs_and_notify (container);

  إذا (شجرة_fd >= 0)
    {
      ret = setns (شجرة_fd, CLONE_NEWNS);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "setns `CLONE_NEWNS`");

      ret = mount (عدم, "/", عدم, MS_REMOUNT | MS_BIND, عدم);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "remount `/`");

      ret = do_mount (container, عدم, -1, "/", عدم, MS_REC | MS_PRIVATE, عدم, LABEL_MOUNT, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      ret = do_mount (container, عدم, -1, "/", عدم, rootfs_propagation, عدم, LABEL_MOUNT, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      get_private_data (container)->needs_pivot = false;
      get_private_data (container)->no_pivot = no_pivot;
      /* setns() made the container rootfs the process root, so an absolute
         source path no longer refers to the host.  */
      get_private_data (container)->host_جذر_switched = true;
      حرر (*rootfs);
      *rootfs = xstrdup ("/");
    }
  وإلا
    {
      إذا (! get_private_data (container)->joined_mount_ns)
        {
          ret = unshare (CLONE_NEWNS);
          إذا (UNLIKELY (ret < 0))
            إرجاع crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
        }

      ret = do_mount (container, عدم, -1, "/", عدم, rootfs_propagation, عدم, LABEL_MOUNT, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      ret = make_parent_mount_private (*rootfs, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      ret = do_mount (container, *rootfs, -1, *rootfs, عدم, MS_BIND | MS_REC | MS_PRIVATE, عدم, LABEL_MOUNT, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      get_private_data (container)->needs_pivot = true;
      get_private_data (container)->no_pivot = no_pivot;
    }

  /* Mount everything before pivot_root while host paths are still reachable.
     Use the pre-created fd when available, fall back to mount().  */
  ret = open (*rootfs, O_PATH | O_CLOEXEC);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "open `%s`", *rootfs);
  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = *rootfs;

  إذا (! container->context->force_no_cgroup)
    {
      صحيح cgroup_mode = libcrun_get_cgroup_mode (err);
      إذا (UNLIKELY (cgroup_mode < 0))
        إرجاع cgroup_mode;

      إذا (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          حرف *unified_cgroup_path = عدم;
          صحيح procfd = get_procfd (get_private_data (container), err);
          إذا (UNLIKELY (procfd < 0))
            إرجاع procfd;

          ret = libcrun_get_cgroup_process_at (procfd, &unified_cgroup_path, true, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;

          get_private_data (container)->unified_cgroup_path = unified_cgroup_path;
        }
    }

  ret = do_mounts (container, *rootfs, err);
  إذا (UNLIKELY (ret < 0))
    {
      close_and_reset (&get_private_data (container)->rootfsfd);
      get_private_data (container)->rootfs = عدم;
      إرجاع ret;
    }

  close_and_reset (&get_private_data (container)->rootfsfd);
  get_private_data (container)->rootfs = عدم;

  إذا (شجرة_fd < 0)
    preopen_needed_devs_and_notify (container);

  /* After pivot_root, /proc is not yet mounted.  */
  ret = get_procfd (get_private_data (container), &tmp_err);
  إذا (ret < 0)
    crun_error_release (&tmp_err);

  check_running_in_user_namespace (&tmp_err);
  crun_error_release (&tmp_err);

  إرجاع 0;
}

صحيح
libcrun_do_pivot_جذر (libcrun_container_t *container, bool no_pivot, حرف **rootfs, libcrun_error_t *err)
{
  صحيح ret;

  إذا (*rootfs == عدم)
    إرجاع 0;

  إذا (get_private_data (container)->unshare_flags & CLONE_NEWNS)
    {
      ret = setup_mount_namespace (container, no_pivot, rootfs, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      /* If setup_mount_namespace used OPEN_TREE_NAMESPACE, rootfs is
         already set to "/".  Otherwise pivot_root is deferred until
         after the mounts are created.  */
      إذا (strcmp (*rootfs, "/") == 0)
        {
          ret = chdir ("/");
          إذا (UNLIKELY (ret < 0))
            إرجاع crun_make_error (err, errno, "chdir to `/`");
        }
    }
  وإلا
    {
      ret = chroot (*rootfs);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "chroot to `%s`", *rootfs);

      ret = chdir ("/");
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "chdir to `/`");

      حرر (*rootfs);
      *rootfs = xstrdup ("/");
    }

  إرجاع 0;
}

/* If one of stdin, stdout, stderr are pointing to /dev/null on
 * the outside of the container, this moves it to /dev/null inside
 * of the container. This needs to run after pivot/chroot-ing. */
صحيح
libcrun_reopen_dev_null (libcrun_error_t *err)
{
  بنية stat statbuf;
  cleanup_close صحيح fd;
  صحيح ret;
  صحيح i;

  /* Open /dev/null inside of the container. */
  fd = open ("/dev/null", O_RDWR | O_CLOEXEC | O_NOFOLLOW);
  إذا (UNLIKELY (fd == -1))
    إرجاع crun_make_error (err, errno, "open `/dev/null`");

  ret = ensure_dev_null (fd, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  لكل (i = 0; i <= 2; i++)
    {
      إذا (UNLIKELY (fstat (i, &statbuf) == -1))
        إرجاع crun_make_error (err, errno, "stat fd `%d`", i);
      إذا (is_dev_null (&statbuf))
        {
          /* This FD is pointing to /dev/null. Point it to /dev/null inside
           * of the container. */
          إذا (UNLIKELY (dup2 (fd, i) == -1))
            إرجاع crun_make_error (err, errno, "dup2 `%d`", i);
        }
    }
  إرجاع 0;
}

ساكن صحيح
uidgidmap_helper (حرف *helper, pid_t pid, ثابت حرف *map_file, libcrun_error_t *err)
{
#define MAX_ARGS 20
  حرف pid_fmt[16];
  حرف *args[MAX_ARGS + 1];
  حرف *next;
  cleanup_free حرف *map_file_copy = xstrdup (map_file);
  size_t nargs = 0;
  صحيح ret;

  args[nargs++] = helper;

  ret = snprintf (pid_fmt, حجم (pid_fmt), "%d", pid);
  إذا (UNLIKELY (ret >= (صحيح) حجم (pid_fmt)))
    إرجاع crun_make_error (err, 0, "internal error: static buffer too small");

  args[nargs++] = pid_fmt;
  next = map_file_copy;
  بينما (nargs < MAX_ARGS)
    {
      حرف *p = strsep (&next, " \n");
      إذا (next == عدم)
        اكسر;
      args[nargs++] = p;
    }
  args[nargs++] = عدم;

  ret = run_process (args, err);
  إذا (ret < 0)
    إرجاع ret;
  إذا (ret > 0)
    إرجاع crun_make_error (err, 0, "`%s` exited with status %d", helper, ret);
  إرجاع 0;
}

ساكن صحيح
newgidmap (pid_t pid, ثابت حرف *map_file, libcrun_error_t *err)
{
  إرجاع uidgidmap_helper ("newgidmap", pid, map_file, err);
}

ساكن صحيح
newuidmap (pid_t pid, ثابت حرف *map_file, libcrun_error_t *err)
{
  إرجاع uidgidmap_helper ("newuidmap", pid, map_file, err);
}

ساكن صحيح
deny_setgroups (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_close صحيح fd = -1;

  fd = libcrun_open_proc_pid_file (container, pid, "setgroups", O_WRONLY, err);
  إذا (UNLIKELY (fd < 0))
    إرجاع fd;

  ret = TEMP_FAILURE_RETRY (اكتب (fd, "deny", 4));
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "write to `/proc/%d/setgroups`", pid);

  get_private_data (container)->deny_setgroups = true;
  إرجاع 0;
}

ساكن صحيح
can_setgroups (libcrun_container_t *container, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_free حرف *content = عدم;

  إذا (get_private_data (container)->deny_setgroups)
    إرجاع 0;

  إذا (container->container_def->annotations)
    {
      ثابت حرف *annotation;

      /* Skip setgroups if the annotation is set to anything different than "0".  */
      annotation = find_annotation (container, "run.oci.keep_original_groups");
      إذا (annotation)
        إرجاع strcmp (annotation, "0") == 0 ? 1 : 0;
    }

  {
    cleanup_close صحيح fd = -1;
    fd = libcrun_open_proc_file (container, "self/setgroups", O_RDONLY, err);
    إذا (fd < 0)
      ret = fd;
    وإلا
      ret = read_all_fd (fd, "self/setgroups", &content, عدم, err);
  }
  إذا (ret < 0)
    {
      /* If the file does not exist, then the kernel does not support /proc/self/setgroups and setgroups can always be used.  */
      إذا (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          إرجاع 1;
        }
      إرجاع ret;
    }

  إرجاع strncmp (content, "deny", 4) == 0 ? 0 : 1;
}

صحيح
libcrun_container_setgroups (libcrun_container_t *container,
                             runtime_spec_schema_config_schema_process *process,
                             libcrun_error_t *err)
{
  gid_t *additional_gids = عدم;
  size_t additional_gids_len = 0;
  صحيح can_do_setgroups;
  صحيح ret;

  إذا (process != عدم && process->user != عدم)
    {
      additional_gids = process->user->additional_gids;
      additional_gids_len = process->user->additional_gids_len;
    }

  can_do_setgroups = can_setgroups (container, err);
  إذا (UNLIKELY (can_do_setgroups < 0))
    إرجاع can_do_setgroups;

  إذا (can_do_setgroups == 0)
    {
      ثابت حرف *annotation;
      size_t i;

      annotation = find_annotation (container, "run.oci.keep_original_groups");
      إذا (annotation && strcmp (annotation, "0") != 0)
        إرجاع 0;

      /* The supplementary groups cannot be changed.  Fail if any of the
         requested additional gids would not be in effect, but tolerate
         the primary gid since it is already granted by setgid.  */
      لكل (i = 0; i < additional_gids_len; i++)
        إذا (additional_gids[i] != process->user->gid)
          إرجاع crun_make_error (err, EPERM, "cannot set additional gid `%d`: setgroups is denied in the user namespace",
                                  (صحيح) additional_gids[i]);
      إرجاع 0;
    }

  ret = setgroups (additional_gids_len, additional_gids);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "setgroups");

  إرجاع 0;
}

صحيح
libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err)
{
  إذا (get_private_data (container)->unshare_cgroupns)
    {
      صحيح ret = unshare (CLONE_NEWCGROUP);
      إذا (UNLIKELY (ret < 0))
        {
          إذا (errno != EINVAL)
            إرجاع crun_make_error (err, errno, "unshare (CLONE_NEWCGROUP)");
        }
    }
  إرجاع 0;
}

// determine whether the uid/gid mappings only contain a single entry
// that maps the host uid/gid on the process->user->uid/gid
صحيح
is_single_mapping (runtime_spec_schema_defs_id_mapping **mappings, size_t len,
                   uint32_t host_id, uint32_t container_id)
{
  إذا (len != 1)
    إرجاع 0;

  إذا (mappings[0]->size != 1)
    إرجاع 0;

  إذا (mappings[0]->container_id != container_id || mappings[0]->host_id != host_id)
    إرجاع 0;

  إرجاع 1;
}

ساكن صحيح
write_id_map (libcrun_container_t *container, pid_t pid, ثابت حرف *map_name,
              صحيح (*idmap_helper) (pid_t, ثابت حرف *, libcrun_error_t *), ثابت حرف *helper_name,
              bool warn_on_failure, ثابت حرف *map, size_t map_len,
              runtime_spec_schema_defs_id_mapping **mappings, size_t mappings_len,
              uint32_t host_id, uint32_t container_id, bool skip_if_setgroups_denied,
              libcrun_error_t *err)
{
  صحيح ret = 0;

  إذا (container->host_uid)
    ret = idmap_helper (pid, map, err);
  إذا (container->host_uid == 0 || ret < 0)
    {
      cleanup_close صحيح fd = -1;

      إذا (ret < 0)
        {
          إذا (warn_on_failure)
            libcrun_warning ("unable to invoke `%s`, will try creating a user namespace with single mapping as an alternative", helper_name);
          crun_error_release (err);
        }

      fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
      إذا (UNLIKELY (fd < 0))
        إرجاع fd;

      ret = safe_write (fd, map_name, map, map_len, err);
      إذا (ret < 0 && (! mappings_len || is_single_mapping (mappings, mappings_len, host_id, container_id)))
        {
          size_t single_mapping_len;
          cleanup_free حرف *single_mapping = عدم;
          crun_error_release (err);

          إذا (! skip_if_setgroups_denied || ! get_private_data (container)->deny_setgroups)
            {
              ret = deny_setgroups (container, pid, err);
              إذا (UNLIKELY (ret < 0))
                إرجاع ret;
            }

          ret = format_mount_mapping (&single_mapping, container_id, host_id, 1, &single_mapping_len, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;

          close_and_reset (&fd);

          fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
          إذا (UNLIKELY (fd < 0))
            إرجاع fd;

          ret = safe_write (fd, map_name, single_mapping, single_mapping_len, err);
        }
    }
  إرجاع ret;
}

صحيح
libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  cleanup_free حرف *uid_map = عدم;
  cleanup_free حرف *gid_map = عدم;
  size_t uid_map_len = 0, gid_map_len = 0;
  صحيح ret = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  إذا ((get_private_data (container)->unshare_flags & CLONE_NEWUSER) == 0)
    إرجاع 0;

  إذا (! def || ! def->linux)
    إرجاع 0;

  إذا (def->linux->uid_mappings_len)
    {
      ret = format_mount_mappings (&uid_map, def->linux->uid_mappings, def->linux->uid_mappings_len, &uid_map_len, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }
  وإلا
    {
      ret = format_default_id_mapping (&uid_map, container->container_uid, container->host_uid, container->host_uid, 1, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      uid_map_len = (size_t) ret;
      إذا (uid_map == عدم)
        {
          ret = format_mount_mapping (&uid_map, 0, container->host_uid, container->host_uid + 1, &uid_map_len, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;
        }
    }

  إذا (def->linux->gid_mappings_len)
    {
      ret = format_mount_mappings (&gid_map, def->linux->gid_mappings, def->linux->gid_mappings_len, &gid_map_len, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }
  وإلا
    {
      ret = format_default_id_mapping (&gid_map, container->container_gid, container->host_uid, container->host_gid, 0, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      gid_map_len = (size_t) ret;
      إذا (gid_map == عدم)
        {
          ret = format_mount_mapping (&gid_map, 0, container->host_gid, container->host_gid + 1, &gid_map_len, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;
        }
    }

  ret = write_id_map (container, pid, "gid_map", newgidmap, "newgidmap",
                      ! def->linux->uid_mappings_len, gid_map, gid_map_len,
                      def->linux->gid_mappings, def->linux->gid_mappings_len,
                      container->host_gid, container->container_gid, /* skip_if_setgroups_denied */ false, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  ret = write_id_map (container, pid, "uid_map", newuidmap, "newuidmap",
                      ! def->linux->uid_mappings_len, uid_map, uid_map_len,
                      def->linux->uid_mappings, def->linux->uid_mappings_len,
                      container->host_uid, container->container_uid, /* skip_if_setgroups_denied */ true, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  إرجاع 0;
}

#define CAP_TO_MASK_0(x) (1L << ((x) & 31))
#define CAP_TO_MASK_1(x) CAP_TO_MASK_0 (x - 32)

بنية all_caps_s
{
  unsigned long effective[2];
  unsigned long permitted[2];
  unsigned long inheritable[2];
  unsigned long ambient[2];
  unsigned long bounding[2];
};

ساكن صحيح
has_cap_on (صحيح cap, long unsigned *caps)
{
  إذا (cap < 32)
    إرجاع CAP_TO_MASK_0 (cap) & caps[0];
  إرجاع (CAP_TO_MASK_1 (cap) & caps[1]);
}

ساكن unsigned long cap_last_cap;

صحيح
libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close صحيح fd = -1;
  صحيح ret;
  حرف buffer[32];
  fd = libcrun_open_proc_file (container, "sys/kernel/cap_last_cap", O_RDONLY, err);
  إذا (fd < 0)
    إرجاع fd;
  ret = TEMP_FAILURE_RETRY (اقرأ (fd, buffer, حجم (buffer) - 1));
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "read from `/proc/sys/kernel/cap_last_cap`");
  buffer[ret] = '\0';

  errno = 0;
  cap_last_cap = strtoul (buffer, عدم, 10);
  إذا (errno != 0)
    إرجاع crun_make_error (err, errno, "strtoul `%s` from `/proc/sys/kernel/cap_last_cap`", buffer);
  إرجاع 0;
}

ساكن صحيح
set_required_caps (بنية all_caps_s *caps, uid_t uid, gid_t gid, صحيح no_new_privs, bool no_exec, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  unsigned long cap;
  صحيح ret;
  بنية __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  بنية __user_cap_data_struct data[2] = { { 0 } };

  إذا (cap_last_cap == 0)
    إرجاع crun_make_error (err, 0, "internal error: max number of capabilities not initialized");

  لكل (cap = 0; cap <= cap_last_cap; cap++)
    إذا (! has_cap_on (cap, caps->bounding))
      {
        ret = prctl (PR_CAPBSET_DROP, cap, 0, 0, 0);
        إذا (UNLIKELY (ret < 0 && ! (errno == EINVAL)))
          إرجاع crun_make_error (err, errno, "prctl drop bounding");
      }

  data[0].effective = caps->effective[0];
  data[1].effective = caps->effective[1];
  data[0].inheritable = caps->inheritable[0];
  data[1].inheritable = caps->inheritable[1];
  data[0].permitted = caps->permitted[0];
  data[1].permitted = caps->permitted[1];

  ret = prctl (PR_SET_KEEPCAPS, 1, 0, 0, 0);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "prctl set KEEPCAPS");

  /* -1 means "leave unchanged" for setresgid and setresuid.  */
  إذا (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    إرجاع crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresgid (gid, gid, gid);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "setresgid to `%d`", gid);

  ret = setresuid (uid, uid, uid);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = capset (&hdr, data);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "capset");

#  ifdef PR_CAP_AMBIENT
  ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_CLEAR_ALL, 0, 0, 0);
  إذا (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
    إرجاع crun_make_error (err, errno, "prctl reset ambient");

  لكل (cap = 0; cap <= cap_last_cap; cap++)
    إذا (has_cap_on (cap, caps->ambient))
      {
        ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, cap, 0, 0);
        إذا (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
          إرجاع crun_make_error (err, errno, "prctl ambient raise");
      }
#  endif

  /* If the workload runs in the current process, there is no execve(2) that resets KEEPCAPS and,
     for a non-root user, leaves only the ambient set in the permitted and effective sets.  */
  إذا (no_exec)
    {
      ret = prctl (PR_SET_KEEPCAPS, 0, 0, 0, 0);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "prctl reset KEEPCAPS");

      إذا (uid != 0)
        {
          data[0].permitted = data[1].permitted = 0;
#  ifdef PR_CAP_AMBIENT
          لكل (cap = 0; cap <= cap_last_cap; cap++)
            إذا (prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_IS_SET, cap, 0, 0) > 0)
              data[cap / 32].permitted |= CAP_TO_MASK_0 (cap);
#  endif
          data[0].effective = data[0].permitted;
          data[1].effective = data[1].permitted;

          ret = capset (&hdr, data);
          إذا (UNLIKELY (ret < 0))
            إرجاع crun_make_error (err, errno, "capset");
        }
    }
#endif

  إذا (no_new_privs)
    إذا (UNLIKELY (prctl (PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0))
      إرجاع crun_make_error (err, errno, "prctl set no new privs");

  إرجاع 0;
}

ساكن صحيح
read_caps (unsigned long caps[2], حرف **قيم, size_t len, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  size_t i;
  لكل (i = 0; i < len; i++)
    {
      cap_قيمة_t cap;
      إذا (cap_from_name (قيم[i], &cap) < 0)
        إرجاع crun_make_error (err, 0, "unknown capability `%s`", قيم[i]);

      إذا (cap < 32)
        caps[0] |= CAP_TO_MASK_0 (cap);
      وإلا
        caps[1] |= CAP_TO_MASK_1 (cap);
    }
#وإلا
  caps[0] = 0;
  caps[1] = 0;
#endif
  إرجاع 0;
}

صحيح
libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  إذا (proc->selinux_label)
    إرجاع set_selinux_label (container, proc->selinux_label, now, err);

  إرجاع 0;
}

صحيح
libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  إذا (proc->apparmor_profile)
    إرجاع set_apparmor_profile (container, proc->apparmor_profile, proc->no_new_privileges, now, err);
  إرجاع 0;
}

صحيح
libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                  صحيح no_new_privileges, bool no_exec, libcrun_error_t *err)
{
  بنية all_caps_s caps = {};
  صحيح ret;

  إذا (capabilities)
    {
      ret = read_caps (caps.effective, capabilities->effective, capabilities->effective_len, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      ret = read_caps (caps.inheritable, capabilities->inheritable, capabilities->inheritable_len, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      ret = read_caps (caps.ambient, capabilities->ambient, capabilities->ambient_len, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      ret = read_caps (caps.bounding, capabilities->bounding, capabilities->bounding_len, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      ret = read_caps (caps.permitted, capabilities->permitted, capabilities->permitted_len, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }
  إرجاع set_required_caps (&caps, uid, gid, no_new_privileges, no_exec, err);
}

بنية rlimit_s
{
  ثابت حرف *name;
  صحيح قيمة;
};

بنية rlimit_s rlimits[] = { { "RLIMIT_AS", RLIMIT_AS },
                              { "RLIMIT_CORE", RLIMIT_CORE },
                              { "RLIMIT_CPU", RLIMIT_CPU },
                              { "RLIMIT_DATA", RLIMIT_DATA },
                              { "RLIMIT_FSIZE", RLIMIT_FSIZE },
                              { "RLIMIT_LOCKS", RLIMIT_LOCKS },
                              { "RLIMIT_MEMLOCK", RLIMIT_MEMLOCK },
                              { "RLIMIT_MSGQUEUE", RLIMIT_MSGQUEUE },
                              { "RLIMIT_NICE", RLIMIT_NICE },
                              { "RLIMIT_NOFILE", RLIMIT_NOFILE },
                              { "RLIMIT_NPROC", RLIMIT_NPROC },
                              { "RLIMIT_RSS", RLIMIT_RSS },
                              { "RLIMIT_RTPRIO", RLIMIT_RTPRIO },
                              { "RLIMIT_RTTIME", RLIMIT_RTTIME },
                              { "RLIMIT_SIGPENDING", RLIMIT_SIGPENDING },
                              { "RLIMIT_STACK", RLIMIT_مكدس },
                              { عدم, 0 } };

ساكن صحيح
get_rlimit_resource (ثابت حرف *name)
{
  بنية rlimit_s *it;
  لكل (it = rlimits; it->name; it++)
    إذا (strcmp (it->name, name) == 0)
      إرجاع it->قيمة;
  إرجاع -1;
}

صحيح
libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **new_rlimits, size_t len,
                     libcrun_error_t *err)
{
  size_t i, j;
  /* Check for duplicate rlimit types.  */
  لكل (i = 0; i < len; i++)
    {
      لكل (j = i + 1; j < len; j++)
        {
          إذا (strcmp (new_rlimits[i]->type, new_rlimits[j]->type) == 0)
            إرجاع crun_make_error (err, 0, "duplicate rlimit `%s`", new_rlimits[i]->type);
        }
    }

  لكل (i = 0; i < len; i++)
    {
      بنية rlimit limit;
      حرف *type = new_rlimits[i]->type;
      صحيح resource = get_rlimit_resource (type);
      إذا (UNLIKELY (resource < 0))
        إرجاع crun_make_error (err, 0, "invalid rlimit `%s`", type);
      limit.rlim_cur = new_rlimits[i]->soft;
      limit.rlim_max = new_rlimits[i]->hard;
      libcrun_debug ("Set rlimit: soft = `%llu`, hard = `%llu`",
                     (unsigned long long) limit.rlim_cur,
                     (unsigned long long) limit.rlim_max);
      إذا (UNLIKELY (setrlimit (resource, &limit) < 0))
        إرجاع crun_make_error (err, errno, "setrlimit `%s`", type);
    }
  إرجاع 0;
}

صحيح
libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  صحيح ret;
  إذا (def->hostname == عدم || def->hostname[0] == '\0')
    إرجاع 0;
  إذا (! has_uts)
    إرجاع crun_make_error (err, 0, "hostname requires the UTS namespace");
  ret = sethostname (def->hostname, strlen (def->hostname));
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "sethostname");
  إرجاع 0;
}

صحيح
libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  صحيح ret;
  إذا (is_empty_string (def->domainname))
    إرجاع 0;
  إذا (! has_uts)
    إرجاع crun_make_error (err, 0, "domainname requires the UTS namespace");
  ret = setdomainname (def->domainname, strlen (def->domainname));
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "setdomainname");
  إرجاع 0;
}

صحيح
libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close صحيح fd = -1;
  صحيح ret;
  حرف oom_buffer[16];
  إذا (def->process == عدم || ! def->process->oom_score_adj_present)
    إرجاع 0;
  libcrun_debug ("Write OOM score adj: `%d`", def->process->oom_score_adj);

  ret = snprintf (oom_buffer, حجم (oom_buffer), "%i", def->process->oom_score_adj);
  إذا (UNLIKELY (ret >= (صحيح) حجم (oom_buffer)))
    إرجاع crun_make_error (err, 0, "internal error: static buffer too small");

  fd = libcrun_open_proc_file (container, "self/oom_score_adj", O_RDWR, err);
  إذا (fd < 0)
    إرجاع fd;
  ret = TEMP_FAILURE_RETRY (اكتب (fd, oom_buffer, strlen (oom_buffer)));
  إذا (ret < 0)
    إرجاع crun_make_error (err, errno, "write to `/proc/self/oom_score_adj`");
  إرجاع 0;
}

ثابت حرف *sysctlRequiringIPC[] = {
  "kernel/msgmax",
  "kernel/msgmnb",
  "kernel/msgmni",
  "kernel/sem",
  "kernel/shmall",
  "kernel/shmmax",
  "kernel/shmmni",
  "kernel/shm_rmid_forced",
  عدم
};

ساكن صحيح
validate_sysctl (ثابت حرف *original_مفتاح, ثابت حرف *original_قيمة, ثابت حرف *name, unsigned long namespaces_created, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  ثابت حرف *namespace = "";

  /* The name is used as a relative path with openat(), so a leading '/' would
     make it escape /proc/sys altogether, and a ".." component could reach a
     sysctl other than the one being validated here.  */
  إذا (name[0] == '/' || path_has_dot_dot_component (name))
    إرجاع crun_make_error (err, 0, "the sysctl `%s` contains an invalid path", original_مفتاح);

  إذا (has_prefix (name, "fs/mqueue/"))
    {
      إذا (namespaces_created & CLONE_NEWIPC)
        إرجاع 0;

      namespace = "IPC";
      اذهب fail;
    }

  إذا (has_prefix (name, "kernel/"))
    {
      size_t i;

      لكل (i = 0; sysctlRequiringIPC[i]; i++)
        إذا (strcmp (sysctlRequiringIPC[i], name) == 0)
          {
            إذا (namespaces_created & CLONE_NEWIPC)
              إرجاع 0;

            namespace = "IPC";
            اذهب fail;
          }

      إذا (strcmp (name, "kernel/domainname") == 0)
        {
          // Value of sysctl `kernel/domainname` is going to
          // conflict with already set field `domainname` in
          // OCI spec, in such scenario crun will fail to prevent
          // unexpected behaviour for end user.
          إذا (! is_empty_string (def->domainname) && (strcmp (original_قيمة, def->domainname) != 0))
            إرجاع crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `domainname`", original_مفتاح);

          إذا (namespaces_created & CLONE_NEWUTS)
            إرجاع 0;

          namespace = "UTS";
          اذهب fail;
        }

      إذا (strcmp (name, "kernel/hostname") == 0)
        إرجاع crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `hostname`", original_مفتاح);
    }
  إذا (has_prefix (name, "net/"))
    {
      إذا (namespaces_created & CLONE_NEWNET)
        إرجاع 0;

      namespace = "network";
      اذهب fail;
    }

  إرجاع crun_make_error (err, 0, "the sysctl `%s` is not namespaced", original_مفتاح);

fail:
  إرجاع crun_make_error (err, 0, "the sysctl `%s` requires a new %s namespace", original_مفتاح, namespace);
}

/* Best-effort attempt to give a better explanation why setting a sysctl could have failed. */
ساكن حرف *
sysctl_error_reason (ثابت حرف *name, صحيح namespaces_created, صحيح errno_)
{

  إذا (strcmp (name, "net.ipv4.ping_group_range") == 0 && (errno_ == EINVAL) && (namespaces_created & CLONE_NEWUSER))
    إرجاع xstrdup ("are all the IDs mapped in the user namespace?");

  إرجاع عدم;
}

صحيح
libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  cleanup_close صحيح dirfd = -1;
  unsigned long namespaces_created = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  إذا (def->linux == عدم || def->linux->sysctl == عدم || def->linux->sysctl->len == 0)
    إرجاع 0;

  لكل (i = 0; i < def->linux->namespaces_len; i++)
    {
      صحيح قيمة;

      قيمة = libcrun_find_namespace (def->linux->namespaces[i]->type);
      إذا (UNLIKELY (قيمة < 0))
        إرجاع crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      namespaces_created |= قيمة;
    }

  get_private_data (container);
  dirfd = libcrun_open_proc_file (container, "sys", O_DIRECTORY | O_PATH, err);
  إذا (UNLIKELY (dirfd < 0))
    إرجاع dirfd;

  لكل (i = 0; i < def->linux->sysctl->len; i++)
    {
      cleanup_free حرف *name = عدم;
      cleanup_close صحيح fd = -1;
      صحيح ret;

      /* A sysctl whose value is null in the document is parsed into a NULL,
         and there is nothing to write to the file.  */
      إذا (def->linux->sysctl->مفاتيح[i] == عدم || def->linux->sysctl->قيم[i] == عدم)
        إرجاع crun_make_error (err, EINVAL, "sysctl value is not specified");

      name = libcrun_sysctl_مفتاح_to_proc_path (def->linux->sysctl->مفاتيح[i]);

      ret = validate_sysctl (def->linux->sysctl->مفاتيح[i], def->linux->sysctl->قيم[i], name, namespaces_created, def, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      إذا (UNLIKELY (fd < 0))
        إرجاع crun_make_error (err, errno, "open `/proc/sys/%s`", name);

      ret = TEMP_FAILURE_RETRY (اكتب (fd, def->linux->sysctl->قيم[i], strlen (def->linux->sysctl->قيم[i])));
      إذا (UNLIKELY (ret < 0))
        {
          صحيح saved_errno = errno;
          cleanup_free حرف *reason = عدم;

          reason = sysctl_error_reason (def->linux->sysctl->مفاتيح[i], namespaces_created, saved_errno);
          إرجاع crun_make_error (err, saved_errno, "write to `/proc/sys/%s`%s%s%s", name, reason ? " (" : "", reason ?: "", reason ? ")" : "");
        }
    }
  إرجاع 0;
}

ساكن صحيح
open_terminal (حرف **pty, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  cleanup_close صحيح fd = -1;
  uid_t uid = 0;
  صحيح ret;

  إذا (process && process->user)
    uid = process->user->uid;

  fd = libcrun_new_terminal (pty, err);
  إذا (UNLIKELY (fd < 0))
    إرجاع fd;

  ret = libcrun_set_stdio (fd, *pty, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  إذا (uid)
    {
      /* The slave is now stdin, use the descriptor instead of resolving the path again.  */
      ret = fchown (0, uid, -1);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "chown `%s`", *pty);
    }

  ret = get_and_reset (&fd);
  إرجاع ret;
}

حرف *
libcrun_get_external_descriptors (libcrun_container_t *container)
{
  إرجاع get_private_data (container)->external_descriptors;
}

صحيح
libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err)
{
  ثابت حرف *buf = عدم;
  json_gen_ctx *gen = عدم;
  size_t buf_len;
  صحيح ret;
  صحيح i;

  إذا (! json_gen_init (&gen, عدم))
    إرجاع crun_make_error (err, errno, "json_gen_init");

  ret = json_gen_array_open (gen);
  إذا (UNLIKELY (ret != json_gen_status_ok))
    اذهب json_error;

  /* Remember original stdin, stdout, stderr for container restore.  */
  لكل (i = 0; i < 3; i++)
    {
      proc_fd_path_t fd_path;
      حرف link_path[PATH_MAX];

      get_proc_self_fd_path (fd_path, i);

      ret = readlink (fd_path, link_path, PATH_MAX - 1);
      إذا (UNLIKELY (ret < 0))
        {
          /* The fd could not exist.  */
          إذا (errno == ENOENT)
            {
              strcpy (link_path, "/dev/null");
              ret = 9; /* strlen ("/dev/null").  */
            }
          وإلا
            {
              json_gen_free (gen);
              إرجاع crun_make_error (err, errno, "readlink `%s`", fd_path);
            }
        }
      link_path[ret] = 0;

      ret = json_gen_string (gen, link_path, ret);
      إذا (UNLIKELY (ret != json_gen_status_ok))
        اذهب json_error;
    }

  ret = json_gen_array_close (gen);
  إذا (UNLIKELY (ret != json_gen_status_ok))
    اذهب json_error;

  ret = json_gen_get_buf (gen, &buf, &buf_len);
  إذا (UNLIKELY (ret != json_gen_status_ok))
    اذهب json_error;

  إذا (buf)
    {
      حرف *b = xmalloc (buf_len + 1);
      memcpy (b, buf, buf_len);
      b[buf_len] = '\0';
      get_private_data (container)->external_descriptors = b;
    }

  json_gen_free (gen);

  إرجاع 0;

json_error:
  إذا (gen)
    json_gen_free (gen);
  إرجاع json_gen_error_to_crun_error (ret, err);
}

صحيح
libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free حرف *pty = عدم;
  cleanup_close صحيح fd = -1;
  صحيح ret;

  إذا (def->process == عدم || ! def->process->terminal)
    إرجاع 0;

  fd = open_terminal (&pty, def->process, err);
  إذا (UNLIKELY (fd < 0))
    إرجاع fd;

  إذا (def->process->console_size)
    {
      ret = libcrun_terminal_setup_size (0, def->process->console_size->height, def->process->console_size->width, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  إذا (get_private_data (container)->mount_dev_from_host)
    {
      ret = do_mount (container, pty, -1, "/dev/console", عدم, MS_BIND, عدم, LABEL_MOUNT, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }
  وإلا
    {
      ret = unlink ("/dev/console");
      إذا (UNLIKELY (ret < 0 && errno != ENOENT))
        {
          إذا (errno == EROFS)
            {
              /* If the file system is read-only, fall back to a bind mount.  */
              ret = do_mount (container, pty, -1, "/dev/console", عدم, MS_BIND, عدم, LABEL_MOUNT, err);
              إذا (UNLIKELY (ret < 0))
                إرجاع ret;

              إرجاع get_and_reset (&fd);
            }
          إرجاع crun_make_error (err, errno, "unlink `/dev/console`");
        }

      ret = symlink (pty, "/dev/console");
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "symlink `/dev/console` -> `%s`", pty);
    }

  إرجاع get_and_reset (&fd);
}

ساكن bool
read_error_from_sync_socket (صحيح sync_socket_fd, صحيح *error, حرف **str)
{
  cleanup_free حرف *b = عدم;
  size_t size;
  صحيح code = 0;
  صحيح ret;

  إذا (*error == 0)
    {
      ret = TEMP_FAILURE_RETRY (اقرأ (sync_socket_fd, &code, حجم (code)));
      إذا (UNLIKELY (ret != (صحيح) حجم (code)))
        إرجاع false;
      *error = code;
    }

  ret = TEMP_FAILURE_RETRY (اقرأ (sync_socket_fd, &size, حجم (size)));
  إذا (UNLIKELY (ret != (صحيح) حجم (size)))
    إرجاع false;

  إذا (size == 0)
    إرجاع false;

  إذا (size > 1024)
    size = 1024;

  b = xmalloc (size + 1);
  ret = TEMP_FAILURE_RETRY (اقرأ (sync_socket_fd, b, size));
  إذا (UNLIKELY (ret < 0))
    إرجاع false;

  b[ret] = '\0';

  *str = b;
  b = عدم;
  إرجاع true;
}

ساكن bool
send_error_to_sync_socket (صحيح sync_socket_fd, bool has_fd, libcrun_error_t *err)
{
  size_t size;
  صحيح ret;
  صحيح code;
  حرف *رسالة;

  إذا (err == عدم || *err == عدم)
    إرجاع false;

  code = crun_error_get_errno (err);

  إذا (has_fd)
    {
      /* dummy terminal fd.  */
      ret = TEMP_FAILURE_RETRY (اكتب (sync_socket_fd, "1", 1));
      إذا (UNLIKELY (ret < 0))
        إرجاع false;
    }

  ret = TEMP_FAILURE_RETRY (اكتب (sync_socket_fd, &code, حجم (code)));
  إذا (UNLIKELY (ret < 0))
    إرجاع false;

  رسالة = (*err)->رسالة;
  size = strlen (رسالة) + 1;
  ret = TEMP_FAILURE_RETRY (اكتب (sync_socket_fd, &size, حجم (size)));
  إذا (UNLIKELY (ret < 0))
    إرجاع false;

  ret = TEMP_FAILURE_RETRY (اكتب (sync_socket_fd, رسالة, size));
  إذا (UNLIKELY (ret < 0))
    إرجاع false;

  إرجاع true;
}

ساكن مضمن صحيح
send_success_to_sync_socket (صحيح sync_socket, libcrun_error_t *err)
{
  ثابت صحيح success = 0;
  صحيح ret;

  ret = TEMP_FAILURE_RETRY (اكتب (sync_socket, &success, حجم (success)));
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "write to sync socket");

  إرجاع 0;
}

ساكن __attribute__ ((noreturn)) فراغ
send_error_to_sync_socket_and_die (صحيح sync_socket_fd, bool has_terminal, libcrun_error_t *err)
{
  حرف *رسالة;

  إذا (err == عدم || *err == عدم)
    _safe_exit (EXIT_FAILURE);

  إذا (! send_error_to_sync_socket (sync_socket_fd, has_terminal, err))
    {
      errno = crun_error_get_errno (err);
      رسالة = (*err)->رسالة;
      libcrun_fail_with_error (errno, "%s", رسالة);
    }
  _safe_exit (EXIT_FAILURE);
}

ساكن صحيح
expect_success_from_sync_socket (صحيح sync_fd, libcrun_error_t *err)
{
  cleanup_free حرف *err_str = عدم;
  صحيح res = 1;
  صحيح ret;

  ret = TEMP_FAILURE_RETRY (اقرأ (sync_fd, &res, حجم (res)));
  إذا (UNLIKELY (ret != حجم (res)))
    إرجاع crun_make_error (err, ret < 0 ? errno : 0, "read status from sync socket");

  إذا (res == 0)
    إرجاع 0;

  إذا (read_error_from_sync_socket (sync_fd, &res, &err_str))
    {
      إذا (! is_empty_string (err_str))
        إرجاع crun_make_error (err, res, "%s", err_str);
    }

  إرجاع crun_make_error (err, res, "read from sync socket");
}

ساكن صحيح
join_namespaces (runtime_spec_schema_config_schema *def, صحيح *namespaces_to_join, صحيح n_namespaces_to_join,
                 صحيح *namespaces_to_join_index, bool ignore_join_errors, libcrun_error_t *err)
{
  صحيح ret;
  صحيح i;

  لكل (i = 0; i < n_namespaces_to_join; i++)
    {
      cleanup_free حرف *cwd = عدم;
      صحيح orig_index = namespaces_to_join_index[i];
      صحيح قيمة;

      إذا (namespaces_to_join[i] < 0)
        استمر;

      /* Skip the user namespace.  */
      قيمة = libcrun_find_namespace (def->linux->namespaces[orig_index]->type);
      إذا (قيمة == CLONE_NEWUSER)
        استمر;

      إذا (قيمة == CLONE_NEWNS)
        {
          cwd = getcwd (عدم, 0);
          إذا (UNLIKELY (cwd == عدم))
            إرجاع crun_make_error (err, errno, "getcwd");
        }

      libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[orig_index]->type, def->linux->namespaces[orig_index]->path);
      ret = setns (namespaces_to_join[i], قيمة);
      إذا (UNLIKELY (ret < 0))
        {
          إذا (ignore_join_errors)
            استمر;
          إرجاع crun_make_error (err, errno, "setns `%s`", def->linux->namespaces[orig_index]->path);
        }

      close_and_reset (&namespaces_to_join[i]);

      إذا (قيمة == CLONE_NEWNS)
        {
          ret = chdir (cwd);
          إذا (UNLIKELY (ret < 0))
            إرجاع crun_make_error (err, errno, "chdir `%s`", cwd);
        }
    }
  إرجاع 0;
}

#define MAX_NAMESPACES 10

بنية init_status_s
{
  /* fd to the namespace to join.  */
  صحيح fd[MAX_NAMESPACES + 1];
  /* Index into def->linux->namespaces.  */
  صحيح index[MAX_NAMESPACES];
  /* CLONE_* value.  */
  صحيح قيمة[MAX_NAMESPACES];
  /* How many namespaces to join.  */
  size_t fd_len;

  bool join_pidns;
  bool join_ipcns;

  /* Must create the user namespace after joining
     some existing namespaces.  */
  bool delayed_userns_create;

  /* Need to fork again once in the container.  */
  bool must_fork;

  /* The fork is needed only to join an existing PID namespace, so it
     can wait until the container root is in place.  */
  bool can_delay_pidns_fork;

  /* fd index for userns.  */
  صحيح userns_index;
  /* def->linux->namespaces userns.  */
  صحيح userns_index_origin;

  /* All namespaces created/joined by the container.  */
  صحيح all_namespaces;

  /* What namespaces are still missing to be created.  */
  صحيح namespaces_to_unshare;

  /* Index in fd[] for the pidns that must be joined before any
     other namespace.  */
  صحيح idx_pidns_to_join_immediately;

  /* Index in fd[] for the timens that must be joined before any
     other namespace.  */
  صحيح idx_timens_to_join_immediately;
};

فراغ
cleanup_free_init_statusp (بنية init_status_s *ns)
{
  size_t i;

  إذا (ns == عدم)
    إرجاع;

  لكل (i = 0; i < ns->fd_len; i++)
    TEMP_FAILURE_RETRY (close (ns->fd[i]));
}

ساكن صحيح
configure_init_status (بنية init_status_s *ns, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  size_t i;

  لكل (i = 0; i < MAX_NAMESPACES + 1; i++)
    ns->fd[i] = -1;

  ns->fd_len = 0;
  ns->all_namespaces = 0;
  ns->namespaces_to_unshare = 0;
  ns->join_pidns = false;
  ns->join_ipcns = false;
  ns->must_fork = false;
  ns->can_delay_pidns_fork = false;
  ns->delayed_userns_create = false;
  ns->userns_index = -1;
  ns->userns_index_origin = -1;
  ns->idx_pidns_to_join_immediately = -1;
  ns->idx_timens_to_join_immediately = -1;

  لكل (i = 0; i < def->linux->namespaces_len; i++)
    {
      صحيح قيمة = libcrun_find_namespace (def->linux->namespaces[i]->type);
      إذا (UNLIKELY (قيمة < 0))
        إرجاع crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      إذا (ns->all_namespaces & قيمة)
        إرجاع crun_make_error (err, 0, "duplicate namespace type: `%s`", def->linux->namespaces[i]->type);

      ns->all_namespaces |= قيمة;

      إذا (def->linux->namespaces[i]->path == عدم)
        {
          libcrun_debug ("Unsharing namespace: `%s`", def->linux->namespaces[i]->type);
          ns->namespaces_to_unshare |= قيمة;
        }
      وإلا
        {
          صحيح fd;

          libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[i]->type, def->linux->namespaces[i]->path);

          إذا (ns->fd_len >= MAX_NAMESPACES)
            إرجاع crun_make_error (err, 0, "too many namespaces to join");

          fd = open (def->linux->namespaces[i]->path, O_RDONLY | O_CLOEXEC);
          إذا (UNLIKELY (fd < 0))
            إرجاع crun_make_error (err, errno, "open `%s`", def->linux->namespaces[i]->path);

          إذا (قيمة == CLONE_NEWUSER)
            {
              ns->userns_index = ns->fd_len;
              ns->userns_index_origin = i;
            }

          ns->fd[ns->fd_len] = fd;
          ns->index[ns->fd_len] = i;
          ns->قيمة[ns->fd_len] = قيمة;
          ns->fd_len++;
          ns->fd[ns->fd_len] = -1;
        }
    }

  إذا (container->host_uid && (ns->all_namespaces & CLONE_NEWUSER) == 0)
    {
      libcrun_warning ("non root user need to have an 'user' namespace");
      ns->all_namespaces |= CLONE_NEWUSER;
      ns->namespaces_to_unshare |= CLONE_NEWUSER;
    }

  إرجاع 0;
}

/* Detect if root is available in the container.  */
ساكن bool
جذر_mapped_in_container_p (runtime_spec_schema_defs_id_mapping **mappings, size_t len)
{
  size_t i;

  لكل (i = 0; i < len; i++)
    إذا (mappings[i]->container_id == 0)
      إرجاع true;

  إرجاع false;
}

ساكن بنية libcrun_fd_map *
get_devices_fd_map (libcrun_container_t *container)
{
  بنية libcrun_fd_map *dev_fds = get_private_data (container)->dev_fds;

  إذا (dev_fds == عدم)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t len = def->linux ? def->linux->devices_len : 0;

      dev_fds = make_libcrun_fd_map (len);
      get_private_data (container)->dev_fds = dev_fds;
    }
  إرجاع dev_fds;
}

#define رقم_NEEDED_DEVS (حجم (needed_devs) / حجم (needed_devs[0]) - 1)

ساكن بنية libcrun_fd_map *
get_needed_devs_fd_map (libcrun_container_t *container)
{
  بنية libcrun_fd_map *fds = get_private_data (container)->needed_devs_fds;

  إذا (fds == عدم)
    {
      fds = make_libcrun_fd_map (رقم_NEEDED_DEVS);
      get_private_data (container)->needed_devs_fds = fds;
    }
  إرجاع fds;
}

bool
is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow)
{
  bool ret = false;
  size_t i;

  إذا (recursive)
    *recursive = false;

  إذا (src_nofollow != عدم)
    *src_nofollow = false;

  لكل (i = 0; i < mnt->options_len; i++)
    {
      إذا (mnt->options[i] == عدم)
        استمر;

      إذا (strcmp (mnt->options[i], "bind") == 0)
        {
          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          إذا (src_nofollow == عدم || *src_nofollow)
            اكسر;
        }
      إذا (strcmp (mnt->options[i], "rbind") == 0)
        {
          إذا (recursive)
            *recursive = true;

          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          إذا (src_nofollow == عدم || *src_nofollow)
            اكسر;
        }
      إذا (src_nofollow && strcmp (mnt->options[i], "src-nofollow") == 0)
        *src_nofollow = true;
    }
  إرجاع ret;
}

ساكن حرف *
get_idmapped_option (runtime_spec_schema_defs_mount *mnt, bool *recursive)
{
  size_t i;

  لكل (i = 0; i < mnt->options_len; i++)
    {
      إذا (mnt->options[i] == عدم)
        استمر;

      إذا (has_prefix (mnt->options[i], "idmap"))
        {
          *recursive = false;
          إرجاع mnt->options[i];
        }
      إذا (has_prefix (mnt->options[i], "ridmap"))
        {
          *recursive = true;
          إرجاع mnt->options[i];
        }
    }
  إرجاع عدم;
}

ساكن صحيح
open_mount_type (ثابت حرف *type, صحيح *out_fd, libcrun_error_t *err)
{
  cleanup_close صحيح fsopen_fd = -1;
  cleanup_close صحيح newfs_fd = -1;
  صحيح ret;

  fsopen_fd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  إذا (UNLIKELY (fsopen_fd < 0))
    إرجاع crun_make_error (err, errno, "fsopen `%s`", type);

  ret = syscall_fsconfig (fsopen_fd, FSCONFIG_CMD_CREATE, عدم, عدم, 0);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "fsconfig create `%s`", type);

  newfs_fd = syscall_fsmount (fsopen_fd, FSMOUNT_CLOEXEC, 0);
  إذا (UNLIKELY (newfs_fd < 0))
    إرجاع crun_make_error (err, errno, "fsmount `%s`", type);

  *out_fd = get_and_reset (&newfs_fd);
  إرجاع 0;
}

ساكن صحيح
maybe_get_idmapped_mount (libcrun_container_t *container, runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt, pid_t pid, صحيح *out_fd, bool *has_mappings_out, libcrun_error_t *err)
{
  cleanup_close صحيح newfs_fd = -1;
  cleanup_pid pid_t created_pid = -1;
  بنية mount_attr_s attr = {
    0,
  };
  bool recursive_bind_mount = false;
  cleanup_close صحيح fd = -1;
  ثابت حرف *idmap_option;
  bool recursive = false;
  ثابت حرف *options = عدم;
  bool has_mappings;
  صحيح ret;
  حرف *extra_رسالة = "";
  bool nofollow = false;

  *out_fd = -1;

  idmap_option = get_idmapped_option (mnt, &recursive);

  has_mappings = mnt->uid_mappings_len > 0 || mnt->gid_mappings_len > 0 || (idmap_option != عدم);
  إذا (has_mappings_out)
    *has_mappings_out = has_mappings;
  إذا (! has_mappings)
    إرجاع 0;

  إذا ((mnt->uid_mappings == عدم) != (mnt->gid_mappings == عدم))
    إرجاع crun_make_error (err, 0, "invalid mappings specified for the mount on `%s`", mnt->destination);

  /* If there are options specified, create a new user namespace with the configured mappings.  */
  إذا (idmap_option)
    {
      options = strchr (idmap_option, '=');
      إذا (options)
        {
          /* Skip the '=' itself.  */
          options++;
          إذا (options[0] == '\0')
            options = عدم;
        }
    }

  ret = maybe_create_userns_for_idmapped_mount (container, def, mnt, options, &created_pid, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;
  إذا (created_pid > 0)
    pid = created_pid;

  fd = libcrun_open_proc_pid_file (container, pid, "ns/user", O_RDONLY, err);
  إذا (UNLIKELY (fd < 0))
    إرجاع fd;

  إذا (is_bind_mount (mnt, &recursive_bind_mount, &nofollow))
    {
      newfs_fd = syscall_open_شجرة (AT_FDCWD, mnt->source, (recursive_bind_mount ? AT_RECURSIVE : 0) | AT_NO_AUTOMOUNT | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | OPEN_TREE_CLOEXEC | OPEN_TREE_CLONE);
      إذا (UNLIKELY (newfs_fd < 0))
        إرجاع crun_make_error (err, errno, "open `%s`", mnt->source);
    }
  وإلا
    {
      ret = open_mount_type (mnt->type, &newfs_fd, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  attr.attr_set = MOUNT_ATTR_IDMAP;
  attr.userns_fd = fd;

  ret = syscall_mount_setattr (newfs_fd, "", AT_EMPTY_PATH | (recursive ? AT_RECURSIVE : 0), &attr);
  إذا (UNLIKELY (ret < 0))
    {
      إذا (errno == EINVAL)
        {
          extra_رسالة = " (maybe the file system used doesn't support idmap mounts on this kernel?)";
        }
      إرجاع crun_make_error (err, errno, "mount_setattr `%s`%s", mnt->destination, extra_رسالة);
    }

  *out_fd = get_and_reset (&newfs_fd);
  إرجاع 0;
}

ساكن uid_t
get_id_in_user_namespace (uid_t id, bool is_uid, runtime_spec_schema_config_schema *def)
{
  runtime_spec_schema_defs_id_mapping **mappings;
  size_t len;
  size_t i;

  إذا (! def || ! def->linux)
    اذهب exit;

  mappings = is_uid ? def->linux->uid_mappings : def->linux->gid_mappings;
  len = is_uid ? def->linux->uid_mappings_len : def->linux->gid_mappings_len;

  إذا (! mappings)
    اذهب exit;

  لكل (i = 0; i < len; i++)
    {
      إذا (mappings[i]->container_id <= id
          && id < mappings[i]->container_id + mappings[i]->size)
        إرجاع id - mappings[i]->container_id + mappings[i]->host_id;
    }
exit:
  إرجاع is_uid ? get_overflow_uid () : get_overflow_gid ();
}

ساكن صحيح
precreate_device (libcrun_container_t *container, صحيح devs_dirfd, size_t i, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  runtime_spec_schema_defs_linux_device *device;
  uid_t uid;
  gid_t gid;
  حرف name[64];
  mode_t type;
  dev_t dev;
  صحيح ret;

  ret = snprintf (name, حجم (name), "%zu", i);
  إذا (UNLIKELY (ret >= (صحيح) حجم (name)))
    إرجاع crun_make_error (err, 0, "internal error: static buffer too small");

  device = def->linux->devices[i];

  type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  dev = makedev (device->major, device->minor);

  ret = mknodat (devs_dirfd, name, device->file_mode | type, dev);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "mknod `%s`", device->path);

  uid = get_id_in_user_namespace (device->uid, true, def);
  gid = get_id_in_user_namespace (device->gid, false, def);

  ret = fchownat (devs_dirfd, name, uid, gid, 0); /* lgtm [cpp/toctou-race-condition] */
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "chown `%s`", device->path);

  إرجاع get_bind_mount (devs_dirfd, name, false, false, false, MS_PRIVATE, err);
}

ساكن صحيح
send_mounts (صحيح sync_socket_host, بنية libcrun_fd_map *fds, size_t how_many, size_t total, libcrun_error_t *err)
{
  size_t i;
  صحيح ret;

  ret = TEMP_FAILURE_RETRY (اكتب (sync_socket_host, &how_many, حجم (how_many)));
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "write to sync socket");

  لكل (i = 0; i < total; i++)
    {
      إذا (fds->fds[i] >= 0)
        {
          ret = send_fd_to_socket_with_payload (sync_socket_host, fds->fds[i], (حرف *) &i, حجم (i), err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;
        }
    }
  إرجاع 0;
}

ساكن صحيح
prepare_and_send_mount_mounts (libcrun_container_t *container, pid_t pid, صحيح sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map بنية libcrun_fd_map *mount_fds = عدم;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_free حرف *rootfs = عدم;
  size_t how_many = 0;
  size_t i;
  صحيح ret;

  إذا (def->mounts_len == 0)
    إرجاع 0;

  mount_fds = make_libcrun_fd_map (def->mounts_len);

  إذا (def->جذر && def->جذر->path)
    rootfs = realpath (def->جذر->path, عدم);

  إذا (! has_userns)
    {
      صحيح is_in_userns = check_running_in_user_namespace (err);
      إذا (UNLIKELY (is_in_userns < 0))
        إرجاع is_in_userns;

      has_userns = is_in_userns > 0;
    }

  لكل (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false;
      bool nofollow = false;
      bool has_mappings = false;
      صحيح mount_fd = -1;

      ret = maybe_get_idmapped_mount (container, def, def->mounts[i], pid, &mount_fd, &has_mappings, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      /* If the mount has no mappings and there is not a different user namespace, create the mount later as part of the container setup.
         Skip copy-symlink mounts: open_tree follows the symlink, losing the
         link itself; let process_single_mount handle them.  */
      إذا (mount_fd < 0 && (has_mappings || has_userns) && is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && ! mount_option_exists (def->mounts[i], "copy-symlink")
          && get_path_in_rootfs (rootfs, def->mounts[i]->source) == عدم)
        {
          unsigned long propagation = 0;

          إذا (def->linux && def->linux->rootfs_propagation)
            propagation = get_mount_flags (def->linux->rootfs_propagation, 0, عدم, عدم, عدم, عدم) & ALL_PROPAGATIONS_NO_REC;
          إذا (propagation == 0)
            propagation = MS_PRIVATE;

          mount_fd = get_bind_mount (-1, def->mounts[i]->source, recursive, false, nofollow, propagation, err);
          إذا (UNLIKELY (mount_fd < 0))
            crun_error_release (err);
        }

      إذا (mount_fd >= 0)
        how_many++;

      mount_fds->fds[i] = mount_fd;
    }

  إرجاع send_mounts (sync_socket_host, mount_fds, how_many, def->mounts_len, err);
}

ساكن صحيح
prepare_and_send_dev_mounts (libcrun_container_t *container, صحيح sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map بنية libcrun_fd_map *dev_fds = عدم;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_close صحيح current_mountns = -1;
  cleanup_free حرف *state_dir = عدم;
  cleanup_free حرف *devs_path = عدم;
  cleanup_close صحيح devs_mountfd = -1;
  cleanup_close صحيح targetfd = -1;
  ثابت حرف *context_type = عدم;
  ثابت حرف *label = عدم;
  size_t how_many = 0;
  size_t i;
  صحيح ret;
  // To track whether the namespace has been changed.
  bool ns_changed = false;

  إذا (def->linux == عدم || def->linux->devices_len == 0)
    إرجاع 0;

  dev_fds = make_libcrun_fd_map (def->linux->devices_len);

  إذا (! has_userns || is_empty_string (container->context->id) || geteuid () > 0)
    إرجاع send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_جذر : عدم),
                                     (container->context ? container->context->id : عدم), err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  ret = append_paths (&devs_path, err, state_dir, "devs", عدم);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  ret = mkdir (devs_path, 0700);
  إذا (UNLIKELY (ret < 0 && errno != EEXIST))
    إرجاع crun_make_error (err, errno, "mkdir `%s`", devs_path);

  current_mountns = libcrun_open_proc_file (container, "self/ns/mnt", O_RDONLY, err);
  إذا (UNLIKELY (current_mountns < 0))
    إرجاع current_mountns;

  ret = unshare (CLONE_NEWNS);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "unshare `CLONE_NEWNS`");

  // This indicates that the mount namespace has been altered.
  ns_changed = true;

  ret = mount (عدم, "/", عدم, MS_REC | MS_PRIVATE, عدم);
  إذا (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "mount `MS_REC | MS_PRIVATE`");
      اذهب restore_mountns;
    }

  إذا (container->container_def->linux && container->container_def->linux->mount_label)
    {
      label = container->container_def->linux->mount_label;
      context_type = get_selinux_context_type (container, err);
      إذا (UNLIKELY (context_type == عدم))
        {
          ret = -1;
          اذهب restore_mountns;
        }
    }

  devs_mountfd = fsopen_mount ("tmpfs", عدم, context_type, label, عدم);
  إذا (UNLIKELY (devs_mountfd < 0))
    {
      ret = send_mounts (sync_socket_host, dev_fds, 0, def->linux->devices_len, err);
      اذهب restore_mountns;
    }

  targetfd = open (devs_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  إذا (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      اذهب restore_mountns;
    }

  ret = fs_move_mount_to (devs_mountfd, targetfd, عدم);
  إذا (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "fs_move_mount_to `%s`", devs_path);
      اذهب restore_mountns;
    }

  close_and_reset (&targetfd);

  targetfd = openat (devs_mountfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  إذا (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      اذهب restore_mountns;
    }

  لكل (i = 0; i < def->linux->devices_len; i++)
    {
      ret = precreate_device (container, targetfd, i, err);
      إذا (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          استمر;
        }

      dev_fds->fds[i] = ret;

      إذا (dev_fds->fds[i] >= 0)
        how_many++;
    }

  ret = send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);
restore_mountns:
  إذا (ns_changed && current_mountns >= 0)
    {
      صحيح setns_ret;
      setns_ret = setns (current_mountns, CLONE_NEWNS);
      إذا (UNLIKELY (setns_ret < 0 && ret >= 0))
        {
          إرجاع crun_make_error (err, errno, "setns `CLONE_NEWNS`");
        }
    }

  إرجاع ret;
}

ساكن صحيح
prepare_and_send_needed_dev_mounts (libcrun_container_t *container, صحيح sync_socket_host, libcrun_error_t *err)
{
  cleanup_close_map بنية libcrun_fd_map *fds = عدم;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  size_t how_many = 0;
  size_t i;

  fds = make_libcrun_fd_map (رقم_NEEDED_DEVS);

  إذا (! has_userns || geteuid () > 0)
    إرجاع send_mounts (sync_socket_host, fds, 0, رقم_NEEDED_DEVS, err);

  لكل (i = 0; i < رقم_NEEDED_DEVS && needed_devs[i].path; i++)
    {
      صحيح fd = syscall_open_شجرة (AT_FDCWD, needed_devs[i].path,
                                  OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
      إذا (fd >= 0)
        {
          fds->fds[i] = fd;
          how_many++;
        }
    }

  إرجاع send_mounts (sync_socket_host, fds, how_many, رقم_NEEDED_DEVS, err);
}

ساكن صحيح
prepare_and_send_mounts (libcrun_container_t *container, pid_t pid, صحيح sync_socket_host, libcrun_error_t *err)
{
  صحيح ret;

  ret = expect_success_from_sync_socket (sync_socket_host, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  ret = prepare_and_send_mount_mounts (container, pid, sync_socket_host, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  ret = prepare_and_send_dev_mounts (container, sync_socket_host, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  ret = prepare_and_send_needed_dev_mounts (container, sync_socket_host, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  إرجاع 0;
}

ساكن صحيح
receive_mounts (بنية libcrun_fd_map *fds, صحيح sync_socket_container, libcrun_error_t *err)
{
  size_t i, how_many = 0;
  صحيح ret;

  إذا (fds->nfds == 0)
    إرجاع 0;

  ret = TEMP_FAILURE_RETRY (اقرأ (sync_socket_container, &how_many, حجم (how_many)));
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "read from sync socket");

  لكل (i = 0; i < how_many; i++)
    {
      size_t index;

      ret = receive_fd_from_socket_with_payload (sync_socket_container, (حرف *) &index, حجم (index), err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
      إذا (index >= fds->nfds)
        إرجاع crun_make_error (err, 0, "invalid mount data received");

      إذا (fds->fds[index] >= 0)
        TEMP_FAILURE_RETRY (close (fds->fds[index]));

      fds->fds[index] = ret;
    }

  إرجاع 0;
}

/* Make the init process non-dumpable, so that its /proc files cannot be
   accessed from the namespaces it joins, e.g. by other processes in a joined
   PID namespace while it still has the host root and credentials.

   Do not do it if crun has no CAP_SYS_PTRACE: until the container process
   is executed, accessing /proc files of a non-dumpable init (e.g. to join
   its namespaces with crun exec before the container is started) requires
   CAP_SYS_PTRACE in the user namespace crun runs in.  The capability check
   is done before creating the container user namespace, where the process
   has all capabilities.  */
ساكن صحيح
set_init_non_dumpable (libcrun_container_t *container, libcrun_error_t *err)
{
  صحيح ret;

  إذا (! container->host_has_cap_sys_ptrace)
    إرجاع 0;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  إرجاع 0;
}

ساكن صحيح
set_id_init (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  uid_t uid = 0;
  gid_t gid = 0;
  صحيح ret;

  إذا (def->process && def->process->user && def->linux)
    {
      /*
        If it is running in a user namespace and root is not mapped
        use the UID/GID specified for running the container.
      */
      bool جذر_mapped = false;

      إذا (def->linux->uid_mappings_len != 0)
        {
          جذر_mapped = جذر_mapped_in_container_p (def->linux->uid_mappings, def->linux->uid_mappings_len);
          إذا (! جذر_mapped)
            uid = def->process->user->uid;

          libcrun_debug ("Using mapped UID in container: `%d`", uid);
        }

      إذا (def->linux->gid_mappings_len != 0)
        {
          جذر_mapped = جذر_mapped_in_container_p (def->linux->gid_mappings, def->linux->gid_mappings_len);
          إذا (! جذر_mapped)
            gid = def->process->user->gid;

          libcrun_debug ("Using mapped GID in container: `%d`", gid);
        }
    }

  /* -1 means "leave unchanged" for setresuid and setresgid.  */
  إذا (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    إرجاع crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresuid (uid, uid, uid);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = setresgid (gid, gid, gid);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "setresgid to `%d`", gid);

  إرجاع 0;
}

ساكن bool
is_proc_mount (runtime_spec_schema_defs_mount *mnt)
{
  إرجاع mnt->type && strcmp (mnt->type, "proc") == 0 && ! is_bind_mount (mnt, عدم, عدم);
}

/* Any process in a PID namespace can read the mount table of the others
   through /proc/PID/mountinfo, so a process joining an existing PID
   namespace must not be created while it still sees the host mounts.
   The only step that needs a process in the target PID namespace before
   the container root is in place is creating the proc mounts: open them
   from a short-lived child and keep the fds in the mounts fd map.  The
   child is created with the container rootfs as its root directory, so
   that it does not expose the host mounts either.

   Return 1 if all the proc mounts were opened and the fork can be delayed,
   0 if the process must be created now.  */
ساكن صحيح
open_proc_mounts_in_pidns (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  بنية libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close صحيح socket_parent = -1;
  cleanup_close صحيح socket_child = -1;
  cleanup_close صحيح جذر_fd = -1;
  cleanup_close صحيح cwd_fd = -1;
  size_t i, missing = 0;
  صحيح saved_errno;
  صحيح sockets[2];
  pid_t pid;
  صحيح ret;

  إذا (def->جذر == عدم || def->جذر->path == عدم)
    إرجاع 0;

  لكل (i = 0; i < def->mounts_len; i++)
    إذا (is_proc_mount (def->mounts[i]))
      missing++;
  إذا (missing == 0)
    إرجاع 1;

  جذر_fd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  إذا (UNLIKELY (جذر_fd < 0))
    إرجاع crun_make_error (err, errno, "open `/`");

  cwd_fd = open (".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  إذا (UNLIKELY (cwd_fd < 0))
    إرجاع crun_make_error (err, errno, "open `.`");

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sockets);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "socketpair");
  socket_parent = sockets[0];
  socket_child = sockets[1];

  ret = chroot (def->جذر->path);
  إذا (UNLIKELY (ret < 0))
    إرجاع 0;

  pid = fork ();
  saved_errno = errno;
  إذا (pid == 0)
    {
      /* If an existing user namespace was joined, the process has no
         privileges on the current mount namespace.  */
      إذا (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
        (فراغ) unshare (CLONE_NEWNS);

      لكل (i = 0; i < def->mounts_len; i++)
        {
          cleanup_close صحيح fd = -1;

          إذا (! is_proc_mount (def->mounts[i]))
            استمر;

          fd = open_mount_of_type (container, def->mounts[i], err);
          إذا (fd < 0)
            _safe_exit (EXIT_FAILURE);

          ret = send_fd_to_socket_with_payload (socket_child, fd, (حرف *) &i, حجم (i), err);
          إذا (UNLIKELY (ret < 0))
            _safe_exit (EXIT_FAILURE);
        }
      _safe_exit (EXIT_SUCCESS);
    }

  إذا (UNLIKELY (fchdir (جذر_fd) < 0 || chroot (".") < 0 || fchdir (cwd_fd) < 0))
    إرجاع crun_make_error (err, errno, "restore the root directory");

  إذا (UNLIKELY (pid < 0))
    إرجاع crun_make_error (err, saved_errno, "fork");

  close_and_reset (&socket_child);

  /* If for any reason the mounts cannot be opened, fall back to creating
     the process now.  */
  بينما (missing > 0)
    {
      libcrun_error_t tmp_err = عدم;

      ret = receive_fd_from_socket_with_payload (socket_parent, (حرف *) &i, حجم (i), &tmp_err);
      إذا (ret < 0)
        {
          crun_error_release (&tmp_err);
          اكسر;
        }
      إذا (i >= mount_fds->nfds)
        {
          TEMP_FAILURE_RETRY (close (ret));
          اكسر;
        }

      إذا (mount_fds->fds[i] >= 0)
        TEMP_FAILURE_RETRY (close (mount_fds->fds[i]));
      mount_fds->fds[i] = ret;
      missing--;
    }

  waitpid_ignore_stopped (pid, عدم, 0);

  إرجاع missing == 0 ? 1 : 0;
}

ساكن صحيح
init_container (libcrun_container_t *container, صحيح sync_socket_container, بنية init_status_s *init_status,
                libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  بنية libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close صحيح timens_offsets_fd = -1;
  bool delayed_pidns_fork = false;
  pid_t pid_container = 0;
  size_t i;
  صحيح ret;

  /* Do it before joining any namespace, otherwise the process would be
     visible in a joined PID namespace while it is still dumpable.  */
  ret = set_init_non_dumpable (container, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  إذا (init_status->idx_pidns_to_join_immediately >= 0 || init_status->idx_timens_to_join_immediately >= 0)
    {
      pid_t new_pid;

      إذا (init_status->idx_pidns_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_pidns_to_join_immediately], CLONE_NEWPID);
          إذا (UNLIKELY (ret < 0))
            إرجاع crun_make_error (err, errno, "setns to target PID namespace");

          close_and_reset (&init_status->fd[init_status->idx_pidns_to_join_immediately]);
        }

      إذا (init_status->idx_timens_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_timens_to_join_immediately], CLONE_NEWTIME);
          إذا (UNLIKELY (ret < 0))
            إرجاع crun_make_error (err, errno, "setns to target timens");

          close_and_reset (&init_status->fd[init_status->idx_timens_to_join_immediately]);
        }

      إذا (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;

          delayed_pidns_fork = ret > 0;
        }

      إذا (delayed_pidns_fork)
        {
          /* A zero PID tells the parent that the process is created later.  */
          new_pid = 0;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;

          ret = TEMP_FAILURE_RETRY (اكتب (sync_socket_container, &new_pid, حجم (new_pid)));
          إذا (UNLIKELY (ret < 0))
            إرجاع crun_make_error (err, errno, "write to sync socket");
        }
      وإلا
        {
          new_pid = fork ();
          إذا (UNLIKELY (new_pid < 0))
            إرجاع crun_make_error (err, errno, "fork");
        }

      إذا (new_pid)
        {
          /* Report the new PID to the parent and exit immediately.  */
          ret = send_success_to_sync_socket (sync_socket_container, err);
          إذا (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          ret = TEMP_FAILURE_RETRY (اكتب (sync_socket_container, &new_pid, حجم (new_pid)));
          إذا (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          _safe_exit (0);
        }

      /* Wait for the parent to receive the new PID.  */
      ret = expect_success_from_sync_socket (sync_socket_container, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  ret = libcrun_set_oom (container, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  إذا (init_status->fd_len > 0)
    {
      ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, true, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  /* If the container needs to join an existing PID namespace, take a reference to it
     before creating a new user namespace, as we could lose the access to the existing
     namespace.

     This cannot be done in the parent process (by `prepare_and_send_mounts`), since it is
     necessary to first join the target namespaces and then create the mount.
  */
  إذا ((init_status->all_namespaces & CLONE_NEWUSER) && (init_status->join_pidns || init_status->join_ipcns))
    {
      لكل (i = 0; i < def->mounts_len; i++)
        {
          صحيح fd = -1;

          /* The mount type is optional in the OCI configuration, e.g. bind mounts
             are usually specified without it.  */
          إذا (def->mounts[i]->type == عدم)
            استمر;

          /* If for any reason the mount cannot be opened, ignore errors and continue.
             An error will be generated later if it is not possible to join the namespace.
          */
          إذا (init_status->join_pidns && ! delayed_pidns_fork && strcmp (def->mounts[i]->type, "proc") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, عدم, عدم, عدم);
          إذا (init_status->join_ipcns && strcmp (def->mounts[i]->type, "mqueue") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, عدم, عدم, عدم);

          إذا (fd >= 0)
            {
              إذا (mount_fds->fds[i] >= 0)
                TEMP_FAILURE_RETRY (close (mount_fds->fds[i]));
              mount_fds->fds[i] = fd;
            }
        }
    }

  /* Open the time namespace offsets file now, as it might not be possible to
     open it once the process has switched to the user namespace ids: the
     process is not dumpable then, so its /proc files are owned by the host
     root.  The offsets are written to the time namespace the process creates
     for its children at the time of the write, and the permission to do so
     is checked against the credentials of the opener.  */
  إذا (def->linux->time_offsets)
    {
      timens_offsets_fd = libcrun_open_proc_file (container, "self/timens_offsets", O_WRONLY, err);
      إذا (UNLIKELY (timens_offsets_fd < 0))
        إرجاع timens_offsets_fd;
    }

  إذا (init_status->all_namespaces & CLONE_NEWUSER)
    {
      إذا (init_status->delayed_userns_create)
        {
          libcrun_debug ("Unsharing user namespace");
          ret = unshare (CLONE_NEWUSER);
          إذا (UNLIKELY (ret < 0))
            إرجاع crun_make_error (err, errno, "unshare user namespace");

          init_status->namespaces_to_unshare &= ~CLONE_NEWUSER;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;
        }

      إذا (init_status->userns_index < 0)
        {
          ret = expect_success_from_sync_socket (sync_socket_container, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;
        }
      وإلا
        {
          /* If we need to join another user namespace, do it immediately before creating any other namespace. */
          libcrun_debug ("Joining existing user namespace");
          ret = setns (init_status->fd[init_status->userns_index], CLONE_NEWUSER);
          إذا (UNLIKELY (ret < 0))
            إرجاع crun_make_error (err, errno, "setns to target user namespace `%s`",
                                    def->linux->namespaces[init_status->userns_index_origin]->path);
        }

      ret = set_id_init (container, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      /* Changing the credentials resets the dumpable flag to the value of
         fs.suid_dumpable, so set it again.  */
      ret = set_init_non_dumpable (container, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, false, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  إذا (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS))
    {
      /* New namespaces to create for the container.  */
      ret = unshare (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS));
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "unshare");
    }

  إذا (def->linux->time_offsets)
    {
      حرف fmt_buffer[128];
      صحيح fd = timens_offsets_fd;

      إذا (def->linux->time_offsets->boottime)
        {
          ret = snprintf (fmt_buffer, حجم (fmt_buffer), "boottime %" PRIi64 " %" PRIu32, def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          إذا (UNLIKELY (ret >= (صحيح) حجم (fmt_buffer)))
            إرجاع crun_make_error (err, 0, "internal error: static buffer too small");

          libcrun_debug ("Using boot time offset: secs = `%lld`, nanosecs = `%d`", (long long صحيح) def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          ret = اكتب (fd, fmt_buffer, strlen (fmt_buffer));
          إذا (UNLIKELY (ret < 0))
            إرجاع crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
      إذا (def->linux->time_offsets->monotonic)
        {
          libcrun_debug ("Using monotonic time offset: secs = `%lld`, nanosecs = `%d`", (long long صحيح) def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);

          ret = snprintf (fmt_buffer, حجم (fmt_buffer), "monotonic %" PRIi64 " %" PRIu32, def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);
          إذا (UNLIKELY (ret >= (صحيح) حجم (fmt_buffer)))
            إرجاع crun_make_error (err, 0, "internal error: static buffer too small");

          ret = اكتب (fd, fmt_buffer, strlen (fmt_buffer));
          إذا (UNLIKELY (ret < 0))
            إرجاع crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
    }

  إذا (init_status->must_fork)
    {
      إذا (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;

          delayed_pidns_fork = ret > 0;
        }

      إذا (! delayed_pidns_fork)
        {
          /* A PID and a time namespace are joined when the new process is created.  */
          pid_container = fork ();
          إذا (UNLIKELY (pid_container < 0))
            إرجاع crun_make_error (err, errno, "cannot fork");
        }

      /* Report back the new PID.  A zero PID tells the parent that the process is created later.  */
      إذا (pid_container || delayed_pidns_fork)
        {
          libcrun_debug ("Running container PID after fork: `%d`", pid_container);
          ret = send_success_to_sync_socket (sync_socket_container, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;

          ret = TEMP_FAILURE_RETRY (اكتب (sync_socket_container, &pid_container, حجم (pid_container)));
          إذا (UNLIKELY (ret < 0))
            إرجاع crun_make_error (err, errno, "write to sync socket");

          إذا (pid_container)
            _safe_exit (EXIT_SUCCESS);
        }

      ret = expect_success_from_sync_socket (sync_socket_container, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  get_private_data (container)->delayed_pidns_fork = delayed_pidns_fork;

  ret = send_success_to_sync_socket (sync_socket_container, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  /* Receive the mounts sent by `prepare_and_send_mounts`.  */
  ret = receive_mounts (get_fd_map (container), sync_socket_container, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  ret = receive_mounts (get_devices_fd_map (container), sync_socket_container, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  ret = receive_mounts (get_needed_devs_fd_map (container), sync_socket_container, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  ret = libcrun_container_setgroups (container, container->container_def->process, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  إرجاع 0;
}

bool
libcrun_container_delayed_pidns_fork (libcrun_container_t *container)
{
  إرجاع get_private_data (container)->delayed_pidns_fork;
}

صحيح
libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close صحيح client_fd = -1;
  cleanup_close صحيح pidfd = -1;
  ثابت حرف *v;

  v = find_annotation (container, "run.oci.pidfd_receiver");
  إذا (v == عدم)
    إرجاع 0;

  pidfd = syscall_pidfd_open (pid, 0);
  إذا (UNLIKELY (pidfd < 0))
    إرجاع crun_make_error (err, errno, "pidfd_open");

  client_fd = open_unix_domain_client_socket (v, 0, err);
  إذا (UNLIKELY (client_fd < 0))
    إرجاع client_fd;

  إرجاع send_fd_to_socket (client_fd, pidfd, err);
}

ساكن bool
has_exec_cpu_affinity (runtime_spec_schema_config_schema_process *process)
{
  إذا (process == عدم || process->exec_cpu_affinity == عدم)
    إرجاع false;
  إرجاع (! is_empty_string (process->exec_cpu_affinity->initial))
         || (! is_empty_string (process->exec_cpu_affinity->final));
}

pid_t
libcrun_run_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, فراغ *args,
                             صحيح *sync_socket_out, بنية libcrun_dirfd_s *cgroup_dirfd, libcrun_error_t *err)
{
  __attribute__ ((cleanup (cleanup_free_init_statusp))) بنية init_status_s init_status;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close صحيح sync_socket_container = -1;
  حرف *notify_socket_env = عدم;
  cleanup_close صحيح sync_socket_host = -1;
  __attribute__ ((unused)) cleanup_close صحيح restore_pidns = -1;
  صحيح first_clone_args = 0;
  صحيح sync_socket[2];
  pid_t pid;
  bool new_userns;
  size_t i;
  صحيح ret;

  libcrun_debug ("Running linux container");
  ret = configure_init_status (&init_status, container, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  get_private_data (container)->unshare_flags = init_status.all_namespaces;
  get_private_data (container)->joined_mount_ns
      = (init_status.all_namespaces & CLONE_NEWNS)
        && ! (init_status.namespaces_to_unshare & CLONE_NEWNS);
  /* cgroup will be unshared later.  Once the process is in the correct cgroup.  */
  init_status.all_namespaces &= ~CLONE_NEWCGROUP;
  get_private_data (container)->unshare_cgroupns = init_status.namespaces_to_unshare & CLONE_NEWCGROUP;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "socketpair");

  sync_socket_host = sync_socket[0];
  sync_socket_container = sync_socket[1];

#ifdef HAVE_SYSTEMD
  إذا (def->جذر)
    {
      ret = do_notify_socket (container, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }
#endif

  get_uid_gid_from_def (container->container_def, &container->container_uid, &container->container_gid);

  /* This must be done before we enter a user namespace.  */
  إذا (def->process)
    {
      ret = libcrun_set_rlimits (def->process->rlimits, def->process->rlimits_len, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  ret = libcrun_set_oom (container, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  ret = libcrun_container_notify_handler (args, HANDLER_CONFIGURE_BEFORE_USERNS, container,
                                          container->container_def->جذر ? container->container_def->جذر->path : عدم, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  /* If a new user namespace must be created, but there are other namespaces to join, then delay
     the userns creation after the namespaces are joined.  */
  init_status.delayed_userns_create
      = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0 && init_status.fd_len > 0;

  /* Check if special handling is required to join the namespaces.  The PID and time
     namespaces are to be joined before creating a new user namespace (as the process
     loses the privileges to do so after that), but after joining an existing one (as
     the process might not have the privileges to do so before that).  */
  new_userns = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0;
  لكل (i = 0; i < init_status.fd_len; i++)
    {
      بدل (init_status.قيمة[i])
        {
        حالة CLONE_NEWIPC:
          إذا (init_status.all_namespaces & CLONE_NEWUSER)
            init_status.join_ipcns = true;
          اكسر;

        حالة CLONE_NEWPID:
          /* The prestart, createRuntime and createContainer hooks need the PID of the
             process in the PID namespace before the pivot_root.  */
          init_status.can_delay_pidns_fork = ! (def->hooks
                                                && (def->hooks->prestart_len || def->hooks->create_runtime_len
                                                    || def->hooks->create_container_len));
          إذا (! new_userns)
            init_status.must_fork = true;
          وإلا
            {
              init_status.join_pidns = true;
              init_status.idx_pidns_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWPID;
            }
          اكسر;

        حالة CLONE_NEWTIME:
          إذا (! new_userns)
            init_status.must_fork = true;
          وإلا
            {
              init_status.idx_timens_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWTIME;
            }
          اكسر;
        }
    }

  /* Before attempting any setns() or unshare(), a clone() is required to not touch the caller context
     that can be used later on for running hooks.  */

  إذا ((init_status.namespaces_to_unshare & CLONE_NEWUSER) && init_status.fd_len == 0)
    {
      /* If a user namespace must be created and there are no other namespaces to join, create the userns alone.  */
      first_clone_args = CLONE_NEWUSER;
    }
  وإلا إذا ((init_status.all_namespaces & CLONE_NEWUSER) == 0)
    {
      /* If it doesn't create a user namespace or need to join one, create the new requested namespaces now. */
      first_clone_args = init_status.namespaces_to_unshare & ~(CLONE_NEWTIME | CLONE_NEWCGROUP | CLONE_NEWNS);
    }

  init_status.namespaces_to_unshare &= ~first_clone_args;

  /* Check if there are still namespaces that require a fork().  */
  إذا (init_status.namespaces_to_unshare & (CLONE_NEWPID | CLONE_NEWTIME))
    {
      /* If we need to make another fork(), make sure the NEWPID is always
         created as part of that.  */
      first_clone_args &= ~CLONE_NEWPID;
      init_status.namespaces_to_unshare |= CLONE_NEWPID;
      init_status.must_fork = true;
      init_status.can_delay_pidns_fork = false;
    }

  pid = -1;
  إذا (cgroup_dirfd && *cgroup_dirfd->dirfd >= 0)
    {
      بنية _clone3_args clone3_args;
      memset (&clone3_args, 0, حجم (clone3_args));
      clone3_args.exit_signal = SIGCHLD;
      clone3_args.flags = first_clone_args;

      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = *cgroup_dirfd->dirfd;

      pid = syscall_clone3 (&clone3_args);
      إذا (pid >= 0)
        cgroup_dirfd->joined = true;

      close_and_reset (cgroup_dirfd->dirfd);
    }
  /* fallback to clone() for any error.  */
  إذا (pid < 0)
    {
      pid = syscall_clone (first_clone_args | SIGCHLD, عدم);
      إذا (UNLIKELY (pid < 0))
        إرجاع crun_make_error (err, errno, "clone");
    }

  إذا (pid)
    {
      __attribute__ ((unused)) cleanup_pid pid_t pid_to_clean = pid;

      /* this is safe to do because the std stream files were not changed since the clone().  */
      ret = libcrun_save_external_descriptors (container, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      ret = close_and_reset (&sync_socket_container);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "close");

      /* any systemd notify socket open_tree FD is pointless to keep around in the parent */
      close_and_reset (&(get_private_data (container)->notify_socket_شجرة_fd));

      إذا (init_status.idx_pidns_to_join_immediately >= 0 || init_status.idx_timens_to_join_immediately >= 0)
        {
          pid_t new_pid = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;

          ret = TEMP_FAILURE_RETRY (اقرأ (sync_socket_host, &new_pid, حجم (new_pid)));
          إذا (UNLIKELY (ret != حجم (new_pid)))
            إرجاع crun_make_error (err, ret < 0 ? errno : 0, "read pid from sync socket");

          إذا (new_pid == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          وإلا
            {
              /* Cleanup the first process.  */
              ret = waitpid_ignore_stopped (pid, عدم, 0);

              pid_to_clean = pid = new_pid;
            }

          ret = send_success_to_sync_socket (sync_socket_host, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;
        }

      إذا (init_status.delayed_userns_create)
        {
          ret = expect_success_from_sync_socket (sync_socket_host, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;
        }

      إذا ((init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0)
        {
          ret = libcrun_set_usernamespace (container, pid, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;

          ret = send_success_to_sync_socket (sync_socket_host, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;
        }

      إذا (init_status.must_fork)
        {
          pid_t grandchild = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;

          ret = TEMP_FAILURE_RETRY (اقرأ (sync_socket_host, &grandchild, حجم (grandchild)));
          إذا (UNLIKELY (ret < 0))
            إرجاع crun_make_error (err, errno, "read pid from sync socket");

          ret = send_success_to_sync_socket (sync_socket_host, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;

          إذا (grandchild == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          وإلا
            {
              /* Cleanup the first process.  */
              waitpid_ignore_stopped (pid, عدم, 0);

              pid_to_clean = pid = grandchild;
            }
        }

      /* They are received by `receive_mounts`.  */
      ret = prepare_and_send_mounts (container, pid, sync_socket_host, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      ret = expect_success_from_sync_socket (sync_socket_host, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      *sync_socket_out = get_and_reset (&sync_socket_host);

      /* If the fork is delayed, it is done once the container process exists.  */
      إذا (! get_private_data (container)->delayed_pidns_fork)
        {
          ret = libcrun_handle_pidfd_receiver (pid, container, err);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;
        }

      pid_to_clean = 0;
      إرجاع pid;
    }

  /* Inside the container process.  */

  ret = close_and_reset (&sync_socket_host);
  إذا (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "%s", "close sync socket");

  /* Initialize the new process and make sure to join/create all the required namespaces.  */
  ret = init_container (container, sync_socket_container, &init_status, err);
  إذا (UNLIKELY (ret < 0))
    send_error_to_sync_socket_and_die (sync_socket_container, false, err);
  وإلا
    {
      ret = send_success_to_sync_socket (sync_socket_container, err);
      إذا (UNLIKELY (ret < 0))
        libcrun_fail_with_error (crun_error_get_errno (err), "%s", (*err)->رسالة);
    }

  /* Jump into the specified entrypoint.  */
  إذا (container->context->notify_socket)
    xasprintf (&notify_socket_env, "NOTIFY_SOCKET=%s/notify", container->context->notify_socket);

  ret = entrypoint (args, notify_socket_env, sync_socket_container, err);

  /* For most of the cases ENTRYPOINT returns only on an error, fallback here */
  /* Except for custom handlers which could perform a task and return with success */
  /* since custom handlers could or could not be {long-running, blocking} */
  إذا (*err)
    libcrun_fail_with_error ((*err)->status, "%s", (*err)->رسالة);

  /* If we got here likely we returned from a custom handler eg. wasm, libkrun */
  /* Allow cleanup attributes to perform cleanup and exit with success if return code was 0 */
  إذا (ret == 0)
    _safe_exit (EXIT_SUCCESS);

  _safe_exit (EXIT_FAILURE);
}

ساكن صحيح
join_process_parent_helper (libcrun_context_t *context,
                            libcrun_container_t *container,
                            runtime_spec_schema_config_schema_process *process,
                            pid_t child_pid, صحيح sync_socket_fd,
                            libcrun_container_status_t *status,
                            bool need_move_to_cgroup, ثابت حرف *sub_cgroup,
                            صحيح *terminal_fd, libcrun_error_t *err)
{
  صحيح ret;
  حرف res;
  pid_t pid;
  cleanup_close صحيح sync_fd = sync_socket_fd;

  إذا (terminal_fd)
    *terminal_fd = -1;

  /* Read the status and the PID from the child process.  */
  ret = TEMP_FAILURE_RETRY (اقرأ (sync_fd, &res, 1));
  إذا (UNLIKELY (ret != 1))
    إرجاع crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  إذا (res != '0')
    إرجاع crun_make_error (err, 0, "fail startup");

  ret = TEMP_FAILURE_RETRY (اقرأ (sync_fd, &pid, حجم (pid)));
  إذا (UNLIKELY (ret != حجم (pid)))
    إرجاع crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  /* Wait for the child pid so we ensure the grandchild gets properly reparented.  */
  ret = waitpid_ignore_stopped (child_pid, عدم, 0);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "waitpid for exec child pid");

  إذا (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->initial, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  إذا (! has_exec_cpu_affinity (process))
    {
      ret = libcrun_reset_cpu_affinity_mask (pid, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  إذا (need_move_to_cgroup)
    {
      cleanup_cgroup_status بنية libcrun_cgroup_status *cgroup_status = libcrun_cgroup_make_status (status);
      cleanup_free حرف *final_cgroup = عدم;

      إذا (sub_cgroup)
        {
          ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, عدم);
          إذا (UNLIKELY (ret < 0))
            إرجاع ret;
        }

      ret = libcrun_cgroup_join_process (cgroup_status, final_cgroup ?: status->cgroup_path, pid, status->pid, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  /* Set the scheduler for the process itself (it might have been set for its
     parent, but it is not inherited with SCHED_FLAG_RESET_ON_FORK, and
     SCHED_DEADLINE cannot be set there as it prevents forking).  */
  ret = libcrun_set_scheduler (pid, process, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  إذا (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->final, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_MOVE, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  ret = libcrun_set_io_priority (pid, process, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  /* The write unblocks the grandchild process so it can run once we setup
     the cgroups.  */
  ret = TEMP_FAILURE_RETRY (اكتب (sync_fd, &ret, حجم (ret)));
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "write to sync socket");

  إذا (terminal_fd)
    {
      ret = receive_fd_from_socket (sync_fd, err);
      إذا (UNLIKELY (ret < 0))
        {
          صحيح err_code = 0;
          cleanup_free حرف *err_str = عدم;

          إذا (read_error_from_sync_socket (sync_fd, &err_code, &err_str))
            {
              إذا (! is_empty_string (err_str))
                {
                  crun_error_release (err);
                  إرجاع crun_make_error (err, err_code, "%s", err_str);
                }
            }

          إرجاع crun_error_wrap (err, "receive terminal fd");
        }
      *terminal_fd = ret;
    }

  إرجاع pid;
}

/*
  try to join all the namespaces with a single call to setns using the target process pidfd.

  return codes:
  < 0 - on errors
  0   - the namespaces were not joined.
  > 0 - the namespaces were joined.
*/
ساكن صحيح
try_setns_with_pidfd (pid_t pid_to_join, libcrun_container_t *container, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close صحيح pidfd_pid_to_join = -1;
  صحيح all_flags = 0;
  size_t i;
  صحيح ret;

  /* If there is any explicit namespace path to join, skip the setns_with_pidfd
     shortcut and join each namespace individually.  */
  إذا (def->linux && def->linux->namespaces)
    {
      لكل (i = 0; i < def->linux->namespaces_len; i++)
        إذا (! is_empty_string (def->linux->namespaces[i]->path))
          إرجاع 0;
    }

  pidfd_pid_to_join = syscall_pidfd_open (pid_to_join, 0);
  إذا (UNLIKELY (pidfd_pid_to_join < 0))
    إرجاع 0;

  /* Validate that the pidfd really refers to the original container process.  */
  ret = libcrun_check_pid_valid (status, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;
  إذا (ret == 0)
    إرجاع crun_make_error (err, ESRCH, "container process not found, the pid was reused");

  لكل (i = 0; namespaces[i].ns_file; i++)
    all_flags |= namespaces[i].قيمة;

  ret = setns (pidfd_pid_to_join, all_flags);
  إذا (UNLIKELY (ret < 0))
    إرجاع 0;

  إرجاع 1;
}

ساكن صحيح
join_process_namespaces (libcrun_container_t *container, pid_t pid_to_join, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  صحيح fds_joined[MAX_NAMESPACES] = {
    0,
  };
  صحيح fds[MAX_NAMESPACES] = {
    -1,
  };
  size_t i;
  صحيح ret;

  /* Try to join all namespaces in one shot with setns and pidfd.  */
  ret = try_setns_with_pidfd (pid_to_join, container, status, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;
  /* Nothing left to do if the namespaces were joined.  */
  إذا (LIKELY (ret > 0))
    إرجاع 0;

  /* If setns fails with the target pidfd, fall-back to join each namespace individually.  */

  إذا (def->linux->namespaces_len >= MAX_NAMESPACES)
    إرجاع crun_make_error (err, 0, "invalid configuration");

  لكل (i = 0; namespaces[i].ns_file; i++)
    {
      cleanup_free حرف *ns_path = عدم;

      xasprintf (&ns_path, "%d/ns/%s", pid_to_join, namespaces[i].ns_file);
      fds[i] = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
      إذا (UNLIKELY (fds[i] < 0))
        {
          /* If the namespace doesn't exist, just ignore it.  */
          إذا (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              استمر;
            }

          ret = fds[i];
          اذهب exit;
        }
    }

  /* The namespace files were opened by PID.  Validate that the PID still refers
     to the original container process before joining any of them.  */
  ret = libcrun_check_pid_valid (status, err);
  إذا (UNLIKELY (ret < 0))
    اذهب exit;
  إذا (ret == 0)
    {
      ret = crun_make_error (err, ESRCH, "container process not found, the pid was reused");
      اذهب exit;
    }

  لكل (i = 0; namespaces[i].ns_file; i++)
    {
      إذا (namespaces[i].قيمة == CLONE_NEWUSER)
        استمر;

      ret = setns (fds[i], 0);
      إذا (ret == 0)
        fds_joined[i] = 1;
    }
  لكل (i = 0; namespaces[i].ns_file; i++)
    {
      إذا (fds_joined[i])
        استمر;
      ret = setns (fds[i], 0);
      إذا (ret == 0)
        fds_joined[i] = 1;
    }
  لكل (i = 0; namespaces[i].ns_file; i++)
    {
      إذا (fds_joined[i])
        استمر;
      ret = setns (fds[i], 0);
      إذا (UNLIKELY (ret < 0 && errno != EINVAL))
        {
          size_t j;
          bool found = false;

          لكل (j = 0; j < def->linux->namespaces_len; j++)
            {
              إذا (strcmp (namespaces[i].name, def->linux->namespaces[j]->type) == 0)
                {
                  found = true;
                  اكسر;
                }
            }
          إذا (! found)
            {
              /* It was not requested to create this ns, so just ignore it.  */
              fds_joined[i] = 1;
              استمر;
            }

          ret = crun_make_error (err, errno, "setns `%s`", namespaces[i].ns_file);
          اذهب exit;
        }
      fds_joined[i] = 1;
    }

  ret = 0;

exit:
  لكل (i = 0; namespaces[i].ns_file; i++)
    close_and_reset (&fds[i]);

  إرجاع ret;
}

صحيح
libcrun_join_process (libcrun_context_t *context,
                      libcrun_container_t *container,
                      pid_t pid_to_join,
                      libcrun_container_status_t *status,
                      ثابت حرف *sub_cgroup,
                      صحيح detach,
                      runtime_spec_schema_config_schema_process *process,
                      صحيح *terminal_fd,
                      libcrun_error_t *err)
{
  pid_t pid;
  صحيح ret;
  صحيح sync_socket_fd[2];
  cleanup_close صحيح cgroup_dirfd = -1;
  cleanup_close صحيح sync_fd = -1;
  بنية _clone3_args clone3_args;
  bool need_move_to_cgroup;

  إذا (! detach)
    {
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "set child subreaper");
    }

  إذا (path_has_dot_dot_component (sub_cgroup))
    إرجاع crun_make_error (err, 0, "invalid cgroup path `%s`: `..` components are not allowed", sub_cgroup);

  /* The caller checked only the container cgroup, but a sub-cgroup is managed
     by the container and the process would never run if it is frozen.  */
  إذا (sub_cgroup && ! is_empty_string (status->cgroup_path))
    {
      cleanup_free حرف *final_cgroup = عدم;

      ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, عدم);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      ret = libcrun_cgroup_ensure_not_frozen (final_cgroup, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "error creating socketpair");

  {
    cleanup_cgroup_status بنية libcrun_cgroup_status *cgroup_status = عدم;

    cgroup_status = libcrun_cgroup_make_status (status);

    /* The cgroup can be joined directly only when there are no additional
       controllers not handled by cgroup v2.  */
    إذا (get_force_cgroup_v1_annotation (container) == عدم)
      {
        cgroup_dirfd = libcrun_get_cgroup_dirfd (cgroup_status, sub_cgroup, err);
        إذا (UNLIKELY (cgroup_dirfd < 0))
          crun_error_release (err);
      }
  }

  memset (&clone3_args, 0, حجم (clone3_args));
  clone3_args.exit_signal = SIGCHLD;

  /* Do not join the cgroup immediately if an initial CPU affinity mask is specified, so that
     the process can set the cpu affinity before joining the target cgroup.  */
  إذا (cgroup_dirfd < 0 || (process->exec_cpu_affinity && process->exec_cpu_affinity->initial))
    need_move_to_cgroup = true;
  وإلا
    {
      need_move_to_cgroup = false;
      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = cgroup_dirfd;
    }

  pid = syscall_clone3 (&clone3_args);

  إذا (pid > 0 && ! (process->scheduler && process->scheduler->policy && strcmp (process->scheduler->policy, "SCHED_DEADLINE") == 0))
    {
      /* We need to set the scheduler as soon as possible after joining the cgroup,
         because if it is a RT scheduler, other processes in the container could already
         take the entire cpu time and stall the new process.  This is not done for
         SCHED_DEADLINE, which does not allow the process to fork.  */
      ret = libcrun_set_scheduler (pid, process, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  /* On errors, fall back to fork().  */
  إذا (pid < 0)
    {
      need_move_to_cgroup = true;

      pid = fork ();
      إذا (UNLIKELY (pid < 0))
        {
          ret = crun_make_error (err, errno, "fork");
          اذهب exit;
        }
    }

  إذا (pid)
    {
      close_and_reset (&sync_socket_fd[1]);
      sync_fd = sync_socket_fd[0];
      إرجاع join_process_parent_helper (context, container,
                                         process, pid, sync_fd,
                                         status, need_move_to_cgroup,
                                         sub_cgroup, terminal_fd, err);
    }

  close_and_reset (&sync_socket_fd[0]);
  sync_fd = sync_socket_fd[1];

  /* Not needed anymore, do not keep a reference to the host file system.  */
  close_and_reset (&cgroup_dirfd);

  ret = join_process_namespaces (container, pid_to_join, status, err);
  إذا (UNLIKELY (ret < 0))
    {
      TEMP_FAILURE_RETRY (اكتب (sync_fd, "1", 1));
      libcrun_fail_with_error ((*err)->status, "%s", (*err)->رسالة);
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  إذا (UNLIKELY (pid < 0))
    {
      صحيح saved_errno = errno;
      TEMP_FAILURE_RETRY (اكتب (sync_fd, "1", 1));
      libcrun_fail_with_error (saved_errno, "fork");
    }

  إذا (pid)
    {
      /* Just return the PID to the parent helper and exit.  */
      ret = TEMP_FAILURE_RETRY (اكتب (sync_fd, "0", 1));
      إذا (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      ret = TEMP_FAILURE_RETRY (اكتب (sync_fd, &pid, حجم (pid)));
      إذا (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      _safe_exit (EXIT_SUCCESS);
    }
  وإلا
    {
      /* Inside the grandchild process.  The real process
         used for the container.  */
      cleanup_free حرف *pty = عدم;
      صحيح r = -1;

      /* Wait for the parent to complete the setup (cgroup, scheduler, ...).
         An EOF means the parent failed, so do not go any further.  */
      ret = TEMP_FAILURE_RETRY (اقرأ (sync_fd, &r, حجم (r)));
      إذا (UNLIKELY (ret != حجم (r) || r < 0))
        _safe_exit (EXIT_FAILURE);

      ret = setsid ();
      إذا (ret < 0)
        {
          crun_make_error (err, errno, "setsid");
          send_error_to_sync_socket_and_die (sync_fd, true, err);
        }

      إذا (terminal_fd)
        {
          cleanup_close صحيح ptmx_fd = -1;

          ret = set_id_init (container, err);
          إذا (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ptmx_fd = open_terminal (&pty, process, err);
          إذا (UNLIKELY (ptmx_fd < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ret = send_fd_to_socket (sync_fd, ptmx_fd, err);
          إذا (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);
        }
    }

  إرجاع 0;

exit:
  إذا (sync_socket_fd[0] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[0]));
  إذا (sync_socket_fd[1] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
  إرجاع ret;
}

صحيح
libcrun_linux_container_update (libcrun_container_status_t *status, ثابت حرف *state_جذر, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_cgroup_status بنية libcrun_cgroup_status *cgroup_status = عدم;

  cgroup_status = libcrun_cgroup_make_status (status);

  إرجاع libcrun_update_cgroup_resources (cgroup_status, state_جذر, resources, err);
}

ساكن صحيح
libcrun_container_pause_unpause_linux (libcrun_container_status_t *status, ثابت bool pause, libcrun_error_t *err)
{
  cleanup_cgroup_status بنية libcrun_cgroup_status *cgroup_status = عدم;

  cgroup_status = libcrun_cgroup_make_status (status);

  إرجاع libcrun_cgroup_pause_unpause (cgroup_status, pause, err);
}

صحيح
libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  إرجاع libcrun_container_pause_unpause_linux (status, true, err);
}

صحيح
libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  إرجاع libcrun_container_pause_unpause_linux (status, false, err);
}

صحيح
libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err)
{
  unsigned long persona = 0;
  صحيح ret;

  إذا (strcmp (p->domain, "LINUX") == 0)
    persona = PER_LINUX;
  وإلا إذا (strcmp (p->domain, "LINUX32") == 0)
    persona = PER_LINUX32;
  وإلا
    إرجاع crun_make_error (err, 0, "unknown persona specified `%s`", p->domain);

  ret = personality (persona);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, 0, "personality to `%s`", p->domain);

  إرجاع 0;
}

صحيح
libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err)
{
  صحيح ret;
  size_t i;
  bool configure_network = false;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close صحيح sockfd = -1;

  لكل (i = 0; i < def->linux->namespaces_len; i++)
    {
      صحيح قيمة = libcrun_find_namespace (def->linux->namespaces[i]->type);
      إذا (UNLIKELY (قيمة < 0))
        إرجاع crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      إذا (قيمة == CLONE_NEWNET && def->linux->namespaces[i]->path == عدم)
        {
          configure_network = true;
          اكسر;
        }
    }

  إذا (! configure_network)
    إرجاع 0;

  sockfd = socket (AF_INET, SOCK_DGRAM, 0);
  إذا (LIKELY (sockfd >= 0))
    {
      بنية ifreq ifr_lo = { .ifr_name = "lo", .ifr_flags = IFF_UP | IFF_RUNNING };

      ret = ioctl (sockfd, SIOCSIFFLAGS, &ifr_lo);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "ioctl SIOCSIFFLAGS");
    }
  وإلا
    {
      بنية nlmsghdr *hdr_recv;
      حرف buf[حجم (بنية nlmsghdr) + حجم (بنية nlmsgerr)];
      بنية sockaddr_nl addr = {
        .nl_family = AF_NETLINK,
        .nl_pid = getpid (),
      };
      بنية sockaddr_nl addr_to = {
        .nl_family = AF_NETLINK,
        .nl_pid = 0,
      };
      بنية nlmsghdr hdr = {
        .nlmsg_len = حجم (بنية nlmsghdr) + حجم (بنية ifinfomsg),
        .nlmsg_type = RTM_NEWLINK,
        .nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK,
        .nlmsg_seq = 1,
        .nlmsg_pid = 0,
      };
      بنية ifinfomsg رسالة = {
        .ifi_family = AF_UNSPEC,
        .ifi_type = 0,
        .ifi_index = 1,
        .ifi_flags = IFF_UP,
        .ifi_change = IFF_UP,
      };

      *((بنية nlmsghdr *) buf) = hdr;
      *((بنية ifinfomsg *) (buf + حجم (بنية nlmsghdr))) = رسالة;

      sockfd = socket (PF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);
      إذا (UNLIKELY (sockfd < 0))
        إرجاع crun_make_error (err, errno, "socket PF_NETLINK");

      ret = bind (sockfd, (بنية sockaddr *) &addr, حجم (addr));
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "bind PF_NETLINK");

      ret = sendto (sockfd, buf, حجم (buf), 0, (بنية sockaddr *) &addr_to, حجم (addr_to));
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "sendto PF_NETLINK");

      ret = recvfrom (sockfd, buf, حجم (buf), 0, عدم, عدم);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "recvfrom PF_NETLINK");

      hdr_recv = (بنية nlmsghdr *) buf;

      إذا (hdr_recv->nlmsg_type == NLMSG_ERROR)
        {
          بنية nlmsgerr *err_data = (بنية nlmsgerr *) NLMSG_DATA (hdr_recv);
          /* err_data->error set to 0 means success acknowledgement due to NLM_F_ACK  */
          إذا (err_data->error < 0)
            إرجاع crun_make_error (err, -err_data->error, "netlink error while configuring network");
        }
    }

  إرجاع 0;
}

/* Protection for attacks like CVE-2019-5736.  */
صحيح ensure_cloned_ثنائي ();
__attribute__ ((constructor)) ساكن فراغ
libcrun_rexec (فراغ)
{
  إذا (ensure_cloned_ثنائي () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      _safe_exit (EXIT_FAILURE);
    }
}

صحيح
libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                    libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  إرجاع libcrun_container_checkpoint_linux_criu (status, container, cr_options, err);
}

صحيح
libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                 libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  صحيح ret;
  ret = libcrun_container_restore_linux_criu (status, container, cr_options, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  إرجاع 0;
}

/* Fallback to use kill(2) on systems where pidfd is not available.  */
ساكن صحيح
libcrun_kill_linux_no_pidfd (libcrun_container_status_t *status, bool check_pid, صحيح إشارة, libcrun_error_t *err)
{
  صحيح ret;

  /* There is still a possibility that the pid is killed between the check
     and the time we send the signal, but attempt to reduce the window of time when
     it is possible.  */
  إذا (check_pid)
    {
      ret = libcrun_check_pid_valid (status, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      /* The pid is not valid anymore, return an error.  */
      إذا (ret == 0)
        {
          errno = ESRCH;
          إرجاع crun_make_error (err, errno, "kill container");
        }
    }

  ret = kill (status->pid, إشارة);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "kill container");
  إرجاع 0;
}

صحيح
libcrun_kill_linux (libcrun_container_status_t *status, صحيح إشارة, libcrun_error_t *err)
{
  صحيح ret;
  cleanup_close صحيح pidfd = -1;

  pidfd = syscall_pidfd_open (status->pid, 0);
  إذا (UNLIKELY (pidfd < 0))
    {
      /* If pidfd_open is not supported, fallback to kill.  */
      إذا (errno == ENOSYS)
        إرجاع libcrun_kill_linux_no_pidfd (status, true, إشارة, err);
      /* Kernels < 6.16 return EINVAL, newer ones ENOENT, when the pid
         is not a thread-group leader (i.e. it was reused).  */
      إذا (errno == ESRCH || errno == ENOENT || errno == EINVAL)
        إرجاع crun_make_error (err, ESRCH, "process not running");
      إرجاع crun_make_error (err, errno, "open pidfd");
    }

  ret = libcrun_check_pid_valid (status, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  /* The pid is not valid anymore, return an error.  */
  إذا (ret == 0)
    {
      errno = ESRCH;
      إرجاع crun_make_error (err, errno, "kill container");
    }

  ret = syscall_pidfd_send_signal (pidfd, إشارة, عدم, 0);
  إذا (UNLIKELY (ret < 0))
    {
      /* If pidfd_send_signal is not supported, fallback to kill.  */
      إذا (errno == ENOSYS)
        إرجاع libcrun_kill_linux_no_pidfd (status, false, إشارة, err);
      إرجاع crun_make_error (err, errno, "send signal to pidfd");
    }

  إرجاع 0;
}

ساكن ثابت حرف *
libcrun_get_intelrdt_name (ثابت حرف *ctr_name, runtime_spec_schema_config_schema *def, bool *explicit)
{
  إذا (def == عدم || def->linux == عدم || def->linux->intel_rdt == عدم || def->linux->intel_rdt->clos_id == عدم)
    {
      إذا (explicit)
        *explicit = false;
      إرجاع ctr_name;
    }

  إذا (explicit)
    *explicit = true;
  إرجاع def->linux->intel_rdt->clos_id;
}

صحيح
libcrun_apply_intelrdt (ثابت حرف *ctr_name, libcrun_container_t *container, pid_t pid, صحيح actions, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = عدم;
  bool has_monitoring = false;
  bool explicit = false;
  bool created = false;
  ثابت حرف *name;
  صحيح ret;

  إذا (container)
    def = container->container_def;

  إذا (def == عدم || def->linux == عدم || def->linux->intel_rdt == عدم)
    إرجاع 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, &explicit);

  إذا (actions & LIBCRUN_INTELRDT_CREATE)
    {
      ret = resctl_create (name, explicit, &created, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      إذا (def->linux->intel_rdt->enable_monitoring)
        {
          ret = resctl_create_monitoring_group (name, ctr_name, err);
          إذا (UNLIKELY (ret < 0))
            اذهب fail;

          has_monitoring = true;
        }
    }

  إذا (actions & LIBCRUN_INTELRDT_UPDATE)
    {
      ret = resctl_update (name, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      إذا (UNLIKELY (ret < 0))
        اذهب fail;
    }

  إذا (actions & LIBCRUN_INTELRDT_MOVE)
    {
      ثابت حرف *monitoring_name = def->linux->intel_rdt->enable_monitoring ? ctr_name : عدم;
      ret = resctl_move_task_to (name, monitoring_name, pid, err);
      إذا (UNLIKELY (ret < 0))
        اذهب fail;
    }

  إرجاع 0;

fail:
  إذا (has_monitoring)
    {
      libcrun_error_t tmp_err = عدم;
      صحيح tmp_ret;

      tmp_ret = resctl_destroy_monitoring_group (name, ctr_name, &tmp_err);
      إذا (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }

  /* Cleanup only if the resctl was created as part of this call.  */
  إذا (created)
    {
      libcrun_error_t tmp_err = عدم;
      صحيح tmp_ret;

      tmp_ret = resctl_destroy (name, &tmp_err);
      إذا (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }
  إرجاع ret;
}

صحيح
libcrun_destroy_intelrdt (ثابت حرف *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  bool explicit = false;
  ثابت حرف *clos_id = libcrun_get_intelrdt_name (container_id, def, &explicit);

  إذا (def && def->linux && def->linux->intel_rdt && def->linux->intel_rdt->enable_monitoring)
    {
      صحيح ret;

      ret = resctl_destroy_monitoring_group (clos_id, container_id, err);
      إذا (ret < 0)
        إرجاع ret;
    }

  /* Do not destroy the clos_id if the name was set explicitly.  */
  إذا (explicit)
    إرجاع 0;

  إرجاع resctl_destroy (clos_id, err);
}

صحيح
libcrun_update_intel_rdt (ثابت حرف *ctr_name, libcrun_container_t *container, ثابت حرف *l3_cache_schema, ثابت حرف *mem_bw_schema, حرف *ثابت *schemata, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = عدم;
  ثابت حرف *name;

  إذا (container)
    def = container->container_def;

  إذا (def == عدم || def->linux == عدم || def->linux->intel_rdt == عدم)
    إرجاع 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, عدم);

  إرجاع resctl_update (name, l3_cache_schema, mem_bw_schema, schemata, err);
}

/* Change the current directory and make sure the current working
   directory, once set, is accessible from the current mount
   namespace.  This check prevents container-escape issues like
   CVE-2024-21626.
   The current working directory cannot be longer than PATH_MAX.
*/
صحيح
libcrun_safe_chdir (ثابت حرف *path, libcrun_error_t *err)
{
  cleanup_free حرف *buffer = عدم;
  صحيح ret;

  ret = chdir (path);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "chdir to `%s`", path);

  buffer = xmalloc (PATH_MAX);
  ret = syscall_getcwd (buffer, PATH_MAX);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, -ret, "getcwd");

  /* Enforce that the returned path is an absolute path.  */
  إذا (ret == 0 || buffer[0] != '/')
    {
      إذا (chdir ("/") < 0)
        {
          /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
        }
      errno = ENOENT;

      /*
        The kernel prepends the string "(unreachable)" to the path
        when it is not reachable from the current mount namespace.
        Use it to give a better error message.
      */
#define UNREACHABLE "(unreachable)"
#define UNREACHABLE_LEN ((صحيح) حجم (UNREACHABLE) - 1)

      إذا ((ret >= UNREACHABLE_LEN) && (memcmp (buffer, UNREACHABLE, UNREACHABLE_LEN) == 0))
        إرجاع crun_make_error (err, errno, "the working directory is not accessible from the current namespace");

      إرجاع crun_make_error (err, errno, "the current working directory is not an absolute path");
    }
  إرجاع 0;
}

/* Run the callback in the mount namespace referred to by MNTNS_FD.  It is
   done in a separate function so that no variable of the caller is live
   across the vfork.  */
ساكن صحيح
run_callback_in_namespace (صحيح mntns_fd, صحيح (*callback) (فراغ *, libcrun_error_t *), فراغ *arg, libcrun_error_t *err)
{
  صحيح wait_status = 0;
  pid_t pid;
  صحيح ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  إذا (UNLIKELY (pid < 0))
    إرجاع crun_make_error (err, errno, "vfork");

  إذا (pid == 0)
    {
      ret = setns (mntns_fd, CLONE_NEWNS);
      إذا (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, 0, "setns to target pid");
          _safe_exit (ret);
        }
      ret = chdir ("/");
      إذا (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, errno, "chdir to `/`");
          _safe_exit (ret);
        }

      ret = callback (arg, err);
      /* On failure (ret < 0) the created error will be
         used by the parent proc */
      _safe_exit (ret);
    }

  ret = waitpid_ignore_stopped (pid, &wait_status, 0);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "waitpid for exec child pid");

  /* The vfork() child shares the parent's memory space, so the error
     object populated by the child is available to the parent.
     Do not call crun_make_error() here. */
  إرجاع get_process_exit_status (wait_status);
}

/* Check that USERNS_FD refers to the user namespace configured for the container.  */
ساكن صحيح
check_container_userns (libcrun_container_t *container, صحيح userns_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  bool new_userns = container->host_uid != 0;
  cleanup_close صحيح expected_fd = -1;
  cleanup_close صحيح parent_fd = -1;
  بنية stat st, expected_st;
  ثابت حرف *path = عدم;
  size_t i;
  صحيح ret;

  إذا (def->linux)
    لكل (i = 0; i < def->linux->namespaces_len; i++)
      إذا (libcrun_find_namespace (def->linux->namespaces[i]->type) == CLONE_NEWUSER)
        {
          path = def->linux->namespaces[i]->path;
          new_userns = path == عدم;
          اكسر;
        }

  إذا (path == عدم)
    {
      expected_fd = libcrun_open_proc_file (container, "self/ns/user", O_RDONLY, err);
      إذا (UNLIKELY (expected_fd < 0))
        إرجاع expected_fd;
    }
  وإلا
    {
      expected_fd = open (path, O_RDONLY | O_CLOEXEC);
      إذا (UNLIKELY (expected_fd < 0))
        إرجاع crun_make_error (err, errno, "open `%s`", path);
    }

  /* A user namespace created for the container is a child of the current one.  */
  إذا (new_userns)
    {
      parent_fd = ioctl (userns_fd, NS_GET_PARENT);
      إذا (UNLIKELY (parent_fd < 0))
        إرجاع crun_make_error (err, errno, "get the parent of the user namespace");
      userns_fd = parent_fd;
    }

  ret = fstat (userns_fd, &st);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "fstat user namespace");

  ret = fstat (expected_fd, &expected_st);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "fstat user namespace");

  إذا (st.st_dev != expected_st.st_dev || st.st_ino != expected_st.st_ino)
    إرجاع crun_make_error (err, 0, "the container process is not using the user namespace of the container");

  إرجاع 0;
}

/* Open the mount namespace of the container in MNTNS_FD and the user namespace
   owning it in OWNER_FD.  The container process could have moved to a mount
   namespace owned by a nested user namespace, where it has CAP_SYS_ADMIN.  A
   mount attached there is not protected by the MNT_LOCK_* flags the kernel
   sets on the mounts copied to a less privileged namespace, so the container
   could drop its ro, nosuid, nodev and noexec flags.  Make sure the mount
   namespace is owned by the user namespace of the container, so that the
   caller can join the same namespaces that were checked.  */
ساكن صحيح
open_container_mntns (libcrun_container_t *container, libcrun_container_status_t *status, صحيح *mntns_fd, صحيح *owner_fd, libcrun_error_t *err)
{
  cleanup_close صحيح mntns = -1;
  cleanup_close صحيح owner = -1;
  صحيح ret;

  mntns = libcrun_open_proc_pid_file (container, status->pid, "ns/mnt", O_RDONLY, err);
  إذا (UNLIKELY (mntns < 0))
    إرجاع mntns;

  /* Check if the container is still running after opening the namespace.  */
  ret = libcrun_is_container_running (status, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  إذا (ret == 0)
    إرجاع crun_make_error (err, 0, "container not running");

  owner = ioctl (mntns, NS_GET_USERNS);
  إذا (UNLIKELY (owner < 0))
    إرجاع crun_make_error (err, errno, "get the owner of the mount namespace");

  ret = check_container_userns (container, owner, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  *mntns_fd = mntns;
  mntns = -1;
  *owner_fd = owner;
  owner = -1;
  إرجاع 0;
}

ساكن صحيح
run_in_container_namespace (libcrun_container_t *container, libcrun_container_status_t *status, صحيح (*callback) (فراغ *, libcrun_error_t *), فراغ *arg, libcrun_error_t *err)
{
  cleanup_close صحيح mntns_fd = -1;
  cleanup_close صحيح owner_fd = -1;
  صحيح ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  إرجاع run_callback_in_namespace (mntns_fd, callback, arg, err);
}

بنية umount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  size_t len;
};

/* Detach the mount at DESTINATION, relative to the current root.  The file
   system tree is under the control of the container, so no symlink is
   followed, otherwise the container could choose what mount is detached.
   umount2 accepts only a path, so unmount the last component from its parent
   directory and then go back to the previous working directory.  */
ساكن صحيح
umount_nofollow (ثابت حرف *destination, libcrun_error_t *err)
{
  cleanup_close صحيح parentfd = -1;
  cleanup_close صحيح cwdfd = -1;
  حرف buffer[PATH_MAX];
  ثابت حرف *parent, *name;
  حرف *last_slash;
  size_t len;
  صحيح saved_errno;
  صحيح ret;

  إذا (is_empty_string (destination))
    إرجاع crun_make_error (err, 0, "empty destination for mount");

  len = strlen (destination);
  إذا (UNLIKELY (len >= حجم (buffer)))
    إرجاع crun_make_error (err, ENAMETOOLONG, "umount `%s`", destination);
  memcpy (buffer, destination, len + 1);

  بينما (len > 1 && buffer[len - 1] == '/')
    buffer[--len] = '\0';

  last_slash = strrchr (buffer, '/');
  إذا (last_slash == عدم)
    {
      parent = ".";
      name = buffer;
    }
  وإلا إذا (last_slash == buffer)
    {
      parent = "/";
      name = buffer[1] ? buffer + 1 : ".";
    }
  وإلا
    {
      *last_slash = '\0';
      parent = buffer;
      name = last_slash + 1;
    }

  parentfd = syscall_openat2 (AT_FDCWD, parent, O_PATH | O_DIRECTORY | O_CLOEXEC, 0,
                              RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS);
  إذا (UNLIKELY (parentfd < 0))
    إرجاع crun_make_error (err, errno, "open parent directory of `%s`", destination);

  cwdfd = open (".", O_PATH | O_DIRECTORY | O_CLOEXEC);
  إذا (UNLIKELY (cwdfd < 0))
    إرجاع crun_make_error (err, errno, "open current directory");

  ret = fchdir (parentfd);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "fchdir to parent directory of `%s`", destination);

  ret = umount2 (name, MNT_DETACH | UMOUNT_NOFOLLOW);
  saved_errno = errno;

  إذا (UNLIKELY (fchdir (cwdfd) < 0))
    إرجاع crun_make_error (err, errno, "fchdir to the previous working directory");

  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, saved_errno, "umount `%s`", destination);

  إرجاع 0;
}

ساكن صحيح
do_umount_in_a_container (فراغ *arg, libcrun_error_t *err)
{
  بنية umount_in_a_container_args *args = arg;
  size_t i;
  صحيح ret;

  لكل (i = 0; i < args->len; i++)
    {
      ret = umount_nofollow (args->mounts[i]->destination, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  إرجاع 0;
}

بنية mount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  بنية libcrun_fd_map *fds;
  صحيح pidfd;
  صحيح owner_fd;
  size_t len;
};

/* Open DESTINATION, relative to the current root, creating it if it does not
   exist.  The file system tree is under the control of the container, so no
   symlink is followed, as it happens for `mounts remove`, otherwise a mount
   could be added that cannot be removed.  */
ساكن صحيح
open_destination_nofollow (ثابت حرف *destination, bool dir, libcrun_error_t *err)
{
  ثابت uint64_t resolve = RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS;
  صحيح fd;
  صحيح ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  إذا (LIKELY (fd >= 0))
    إرجاع fd;

  إذا (errno != ENOENT)
    إرجاع crun_make_error (err, errno, "open `%s`", destination);

  إذا (dir)
    ret = crun_safe_ensure_directory_at (AT_FDCWD, "/", destination, 0755, err);
  وإلا
    ret = crun_safe_ensure_file_at (AT_FDCWD, "/", destination, 0755, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  إذا (UNLIKELY (fd < 0))
    إرجاع crun_make_error (err, errno, "open `%s`", destination);

  إرجاع fd;
}

/* Create the missing destinations of the mounts as the root user of the
   container.  The file systems of the container can be owned by its user
   namespace, where the root user of the host has no mapping, so that an
   inode it creates there would have no owner and mkdir fails with
   EOVERFLOW.  */
ساكن صحيح
create_destinations_in_a_container (فراغ *arg, libcrun_error_t *err)
{
  بنية mount_in_a_container_args *args = arg;
  size_t i;
  صحيح ret;

  ret = setns (args->owner_fd, CLONE_NEWUSER);
  إذا (ret == 0)
    {
      ret = setresgid (0, 0, 0);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "setresgid to `0`");

      ret = setresuid (0, 0, 0);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "setresuid to `0`");
    }
  وإلا إذا (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      إرجاع crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  لكل (i = 0; i < args->len; i++)
    {
      cleanup_close صحيح dest_fd = -1;
      بنية stat st;

      ret = fstat (args->fds->fds[i], &st);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      إذا (UNLIKELY (dest_fd < 0))
        إرجاع dest_fd;
    }

  إرجاع 0;
}

ساكن صحيح
do_mount_in_a_container (فراغ *arg, libcrun_error_t *err)
{
  بنية mount_in_a_container_args *args = arg;
  size_t i;
  صحيح ret;

  لكل (i = 0; i < args->len; i++)
    {
      cleanup_close صحيح dest_fd = -1;
      بنية stat st;

      ret = fstat (args->fds->fds[i], &st);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      إذا (UNLIKELY (dest_fd < 0))
        إرجاع dest_fd;

      ret = fs_move_mount_to (args->fds->fds[i], dest_fd, عدم);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "move mount to `%s`", args->mounts[i]->destination);
    }

  إرجاع 0;
}

/* Join the namespaces of the container referred to by PIDFD, create the file
   system for each of MOUNTS that has no fd in FDS yet and send them, in order,
   to SYNC_FD.  It must be called from a child process.  */
ساكن صحيح
send_mounts_from_container_namespaces (libcrun_container_t *container, صحيح pidfd, runtime_spec_schema_defs_mount **mounts,
                                       بنية libcrun_fd_map *fds, size_t len, صحيح flags, صحيح owner_fd, صحيح sync_fd, libcrun_error_t *err)
{
  pid_t pid;
  size_t i;
  صحيح ret;

  /* The process is visible from the container once it joins its PID namespace.  */
  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  /* Join the namespaces while still being privileged, then the user
     namespace that was checked and not the one the container process is in
     right now.  */
  ret = flags ? setns (pidfd, flags) : 0;
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "setns to target pid");

  ret = setns (owner_fd, CLONE_NEWUSER);
  إذا (ret == 0)
    {
      /* CAP_SYS_ADMIN is needed in the user namespace owning the mount
         namespace.  Do not join the mount namespace of the container, any
         path used for the mount refers to the host, as it does for the
         mounts in the container configuration.  */
      ret = unshare (CLONE_NEWNS);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
    }
  وإلا إذا (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      إرجاع crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  إذا (UNLIKELY (pid < 0))
    إرجاع crun_make_error (err, errno, "fork");

  إذا (pid)
    {
      صحيح wait_status = 0;

      /* The container can signal the process, do not hang if it is stopped.  */
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &wait_status, WUNTRACED));
      إذا (ret > 0 && WIFSTOPPED (wait_status))
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, عدم, 0));
          إرجاع crun_make_error (err, 0, "the process creating the mounts was stopped");
        }
      _safe_exit (EXIT_SUCCESS);
    }

  ret = set_id_init (container, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  لكل (i = 0; i < len; i++)
    {
      cleanup_close صحيح fd = -1;

      إذا (fds->fds[i] >= 0)
        استمر;

      fd = open_mount_of_type (container, mounts[i], err);
      إذا (UNLIKELY (fd < 0))
        إرجاع crun_make_error (err, errno, "open mount of type `%s` for `%s`", mounts[i]->type, mounts[i]->destination);

      ret = send_fd_to_socket (sync_fd, fd, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  إرجاع 0;
}

/* Get in FLAGS the namespaces a file system can be bound to (together with
   the user namespace) that the container does not share with the current
   process.  setns requires CAP_SYS_ADMIN in the user namespace owning the
   target also for a namespace that is already the current one.  */
ساكن صحيح
get_namespaces_to_join (libcrun_container_t *container, pid_t pid, صحيح *flags, libcrun_error_t *err)
{
  ساكن ثابت بنية
  {
    ثابت حرف *name;
    صحيح flag;
  } namespaces[] = {
    { "ns/pid", CLONE_NEWPID },
    { "ns/net", CLONE_NEWNET },
    { "ns/ipc", CLONE_NEWIPC },
    { "ns/cgroup", CLONE_NEWCGROUP },
  };
  size_t i;

  *flags = 0;

  لكل (i = 0; i < حجم (namespaces) / حجم (namespaces[0]); i++)
    {
      cleanup_close صحيح self_fd = -1;
      cleanup_close صحيح target_fd = -1;
      بنية stat self_st, target_st;
      cleanup_free حرف *self_path = عدم;
      صحيح ret;

      xasprintf (&self_path, "self/%s", namespaces[i].name);

      self_fd = libcrun_open_proc_file (container, self_path, O_RDONLY, err);
      إذا (UNLIKELY (self_fd < 0))
        إرجاع self_fd;

      target_fd = libcrun_open_proc_pid_file (container, pid, namespaces[i].name, O_RDONLY, err);
      إذا (UNLIKELY (target_fd < 0))
        إرجاع target_fd;

      ret = fstat (self_fd, &self_st);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      ret = fstat (target_fd, &target_st);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      إذا (self_st.st_dev != target_st.st_dev || self_st.st_ino != target_st.st_ino)
        *flags |= namespaces[i].flag;
    }

  إرجاع 0;
}

/* A file system is bound to the namespaces of the process creating it (e.g.
   procfs to its PID namespace), so create the ones still missing in FDS from
   a process that joined the namespaces of the container, as it happens for
   the mounts in the container configuration.  */
ساكن صحيح
open_mounts_in_container_namespaces (libcrun_container_t *container, libcrun_container_status_t *status, صحيح owner_fd,
                                     runtime_spec_schema_defs_mount **mounts, بنية libcrun_fd_map *fds, size_t len,
                                     libcrun_error_t *err)
{
  cleanup_close صحيح sync_fd = -1;
  cleanup_close صحيح pidfd = -1;
  صحيح sync_socket_fd[2];
  bool needed = false;
  صحيح flags;
  pid_t pid;
  size_t i;
  صحيح ret;

  لكل (i = 0; i < len; i++)
    {
      إذا (fds->fds[i] >= 0)
        استمر;

      إذا (mounts[i]->type == عدم)
        إرجاع crun_make_error (err, 0, "invalid mount type for `%s`", mounts[i]->destination);

      needed = true;
    }

  إذا (! needed)
    إرجاع 0;

  pidfd = syscall_pidfd_open (status->pid, 0);
  إذا (UNLIKELY (pidfd < 0))
    إرجاع crun_make_error (err, errno, "pidfd_open");

  /* Check if the container is still running after opening the pidfd.  */
  ret = libcrun_is_container_running (status, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  إذا (ret == 0)
    إرجاع crun_make_error (err, 0, "container not running");

  ret = get_namespaces_to_join (container, status->pid, &flags, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "error creating socketpair");

  sync_fd = sync_socket_fd[0];

  pid = fork ();
  إذا (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
      إرجاع ret;
    }

  إذا (pid == 0)
    {
      close_and_reset (&sync_fd);

      ret = send_mounts_from_container_namespaces (container, pidfd, mounts, fds, len, flags, owner_fd, sync_socket_fd[1], err);
      إذا (UNLIKELY (ret < 0))
        send_error_to_sync_socket_and_die (sync_socket_fd[1], true, err);

      _safe_exit (EXIT_SUCCESS);
    }

  TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));

  ret = 0;
  لكل (i = 0; i < len; i++)
    {
      إذا (fds->fds[i] >= 0)
        استمر;

      ret = receive_fd_from_socket (sync_fd, err);
      إذا (UNLIKELY (ret < 0))
        {
          cleanup_free حرف *err_str = عدم;
          صحيح err_code = 0;

          إذا (read_error_from_sync_socket (sync_fd, &err_code, &err_str) && ! is_empty_string (err_str))
            {
              crun_error_release (err);
              ret = crun_make_error (err, err_code, "%s", err_str);
            }
          وإلا
            ret = crun_error_wrap (err, "receive mount fd");
          اكسر;
        }

      fds->fds[i] = ret;
      ret = 0;
    }

  /* The child process reaps the process that joined the PID namespace of the container.  */
  TEMP_FAILURE_RETRY (waitpid (pid, عدم, 0));

  إرجاع ret;
}

ساكن unsigned long
get_runtime_mount_flags (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, uint64_t *rec_clear, uint64_t *rec_set)
{
  cleanup_free حرف *data = عدم;
  unsigned long extra_flags = 0;
  unsigned long flags = 0;
  size_t j;

  إذا (mount->options == عدم)
    إرجاع get_default_flags (container, mount->destination, &data);

  لكل (j = 0; j < mount->options_len; j++)
    flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, rec_clear, rec_set);

  إرجاع flags;
}

صحيح
libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  بنية mount_in_a_container_args args;
  cleanup_close_map بنية libcrun_fd_map *fds = عدم;
  cleanup_close صحيح pidfd = -1;
  cleanup_close صحيح mntns_fd = -1;
  cleanup_close صحيح owner_fd = -1;
  pid_t pid = status->pid;
  size_t i;
  صحيح ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  fds = make_libcrun_fd_map (len);

  لكل (i = 0; i < len; i++)
    {
      bool recursive = false;
      bool nofollow = false;

      /* Do not check whether the pid is valid or not.  run_in_container_namespace will validate it.  */
      ret = maybe_get_idmapped_mount (container, def, mounts[i], pid, &(fds->fds[i]), عدم, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      إذا (fds->fds[i] < 0 && is_bind_mount (mounts[i], &recursive, &nofollow))
        {
          uint64_t rec_clear = 0, rec_set = 0;
          unsigned long propagation;

          propagation = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set) & ALL_PROPAGATIONS_NO_REC;
          إذا (propagation == 0)
            propagation = MS_PRIVATE;

          fds->fds[i] = get_bind_mount (-1, mounts[i]->source, recursive, false, nofollow, propagation, err);
          إذا (UNLIKELY (fds->fds[i] < 0))
            إرجاع fds->fds[i];
        }
    }

  /* All the remaining mounts are new file systems.  */
  ret = open_mounts_in_container_namespaces (container, status, owner_fd, mounts, fds, len, err);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  لكل (i = 0; i < len; i++)
    {
      uint64_t rec_clear = 0;
      uint64_t rec_set = 0;
      unsigned long flags;

      flags = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set);

      ret = do_mount_setattr (false, mounts[i]->destination, fds->fds[i], 0, flags, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;

      ret = do_mount_setattr (true, mounts[i]->destination, fds->fds[i], rec_clear, rec_set, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  args.mounts = mounts;
  args.fds = fds;
  args.pidfd = pidfd;
  args.owner_fd = owner_fd;
  args.len = len;

  ret = run_callback_in_namespace (mntns_fd, create_destinations_in_a_container, &args, err);
  إذا (UNLIKELY (ret > 0))
    إرجاع *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  إرجاع run_callback_in_namespace (mntns_fd, do_mount_in_a_container, &args, err);
}

صحيح
libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  بنية umount_in_a_container_args args;

  args.mounts = mounts;
  args.len = len;

  إرجاع run_in_container_namespace (container, status, do_umount_in_a_container, &args, err);
}

بنية restrict_mounts_in_a_container_args
{
  بنية libcrun_mount_flags_s *mounts;
  size_t len;
  صحيح rootfd;
};

ساكن صحيح
do_restrict_mounts_in_a_container (فراغ *arg, libcrun_error_t *err)
{
  بنية restrict_mounts_in_a_container_args *args = arg;
  size_t i;
  صحيح ret;

  /* The root of the container is not necessarily the root of the mount
     namespace.  */
  ret = fchdir (args->rootfd);
  إذا (LIKELY (ret == 0))
    ret = chroot (".");
  إذا (UNLIKELY (ret < 0))
    إرجاع crun_make_error (err, errno, "chroot to the container root");

  لكل (i = 0; i < args->len; i++)
    {
      unsigned long flags = args->mounts[i].flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC);
      ثابت حرف *target = consume_slashes (args->mounts[i].target);
      cleanup_close صحيح fd = -1;
      بنية statfs sfs;
      unsigned long current_flags;

      إذا (flags == 0)
        استمر;

      fd = safe_openat (AT_FDCWD, "/", target, O_PATH | O_CLOEXEC, 0, err);
      إذا (UNLIKELY (fd < 0))
        {
          إذا (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              استمر;
            }
          إرجاع fd;
        }

      ret = fstatfs (fd, &sfs);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "statfs `/%s`", target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);
      إذا ((current_flags & flags) == flags)
        استمر;

      ret = do_mount_setattr (false, target, fd, 0, flags, err);
      إذا (LIKELY (ret == 0))
        استمر;

      إذا (crun_error_get_errno (err) != ENOSYS && crun_error_get_errno (err) != EINVAL)
        إرجاع ret;
      crun_error_release (err);

      /* Without mount_setattr(2), remount through the path: the container is
         not running yet, and the root was changed to its rootfs.  The remount
         clears the per-mount flags that are not specified, so pass the
         current ones as well.  */
      ret = mount (عدم, target, عدم, MS_REMOUNT | MS_BIND | flags | current_flags, عدم);
      إذا (UNLIKELY (ret < 0))
        إرجاع crun_make_error (err, errno, "remount `/%s`", target);
    }

  إرجاع 0;
}

/* Make sure that each of MOUNTS, in the mount namespace of PID, has at least
   the read-only, nosuid, nodev and noexec flags that are requested for it.  */
صحيح
libcrun_restrict_mounts_in_container (pid_t pid, بنية libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err)
{
  بنية restrict_mounts_in_a_container_args args;
  cleanup_close صحيح mntns_fd = -1;
  cleanup_close صحيح rootfd = -1;
  حرف path[64];
  صحيح ret;

  snprintf (path, حجم (path), "/proc/%d/ns/mnt", pid);
  mntns_fd = open (path, O_RDONLY | O_CLOEXEC);
  إذا (UNLIKELY (mntns_fd < 0))
    إرجاع crun_make_error (err, errno, "open `%s`", path);

  snprintf (path, حجم (path), "/proc/%d/root", pid);
  rootfd = open (path, O_PATH | O_DIRECTORY | O_CLOEXEC);
  إذا (UNLIKELY (rootfd < 0))
    إرجاع crun_make_error (err, errno, "open `%s`", path);

  args.mounts = mounts;
  args.len = len;
  args.rootfd = rootfd;

  ret = run_callback_in_namespace (mntns_fd, do_restrict_mounts_in_a_container, &args, err);
  إذا (UNLIKELY (ret > 0))
    إرجاع *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);

  إرجاع ret;
}

صحيح
libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close صحيح netns_fd = -1;
  size_t i;
  صحيح ret;

  إذا (def == عدم || def->linux == عدم || def->linux->net_devices == عدم)
    إرجاع 0;

  cleanup_free حرف *ns_path = عدم;

  xasprintf (&ns_path, "%d/ns/net", pid);
  netns_fd = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
  إذا (UNLIKELY (netns_fd < 0))
    إرجاع netns_fd;

  لكل (i = 0; i < def->linux->net_devices->len; i++)
    {
      ثابت حرف *new_name = def->linux->net_devices->قيم[i]->name ?: def->linux->net_devices->مفاتيح[i];

      ret = move_network_device (def->linux->net_devices->مفاتيح[i], new_name, netns_fd, err);
      إذا (UNLIKELY (ret < 0))
        إرجاع ret;
    }

  إرجاع 0;
}
