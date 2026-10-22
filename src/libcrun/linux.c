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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "linux.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "status.h"
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <sched.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <sys/mount.h>
#ifdef HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#endif
#ಯದಿ defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#endif

#ಅನ್ತರ್ಭಾವ <sys/prctl.h>
#ifdef HAVE_CAP
#  include <sys/capability.h>
#endif
#ಅನ್ತರ್ಭಾವ <sys/time.h>
#ಅನ್ತರ್ಭಾವ <sys/resource.h>
#ಅನ್ತರ್ಭಾವ <sys/sysmacros.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <grp.h>
#ಅನ್ತರ್ಭಾವ <signal.h>
#ಅನ್ತರ್ಭಾವ "terminal.h"
#ಅನ್ತರ್ಭಾವ "cgroup.h"
#ಅನ್ತರ್ಭಾವ "cgroup-utils.h"
#ಅನ್ತರ್ಭಾವ "status.h"
#ಅನ್ತರ್ಭಾವ "criu.h"
#ಅನ್ತರ್ಭಾವ "scheduler.h"
#ಅನ್ತರ್ಭಾವ "intelrdt.h"
#ಅನ್ತರ್ಭಾವ "io_priority.h"
#ಅನ್ತರ್ಭಾವ "net_device.h"

#ಅನ್ತರ್ಭಾವ <sys/socket.h>
#ಅನ್ತರ್ಭಾವ <libgen.h>
#ಅನ್ತರ್ಭಾವ <sys/wait.h>
#ಅನ್ತರ್ಭಾವ <sys/vfs.h>
#ಅನ್ತರ್ಭಾವ <sys/statvfs.h>
#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ <inttypes.h>
#ಅನ್ತರ್ಭಾವ <sys/personality.h>
#ಅನ್ತರ್ಭಾವ <net/if.h>
#ಅನ್ತರ್ಭಾವ <sys/xattr.h>
#ಅನ್ತರ್ಭಾವ <linux/netlink.h>
#ಅನ್ತರ್ಭಾವ <linux/rtnetlink.h>
#ಅನ್ತರ್ಭಾವ <sched.h>
#ಅನ್ತರ್ಭಾವ <linux/sched.h>
#ಅನ್ತರ್ಭಾವ <linux/magic.h>
#ಅನ್ತರ್ಭಾವ <linux/nsfs.h>

#ಅನ್ತರ್ಭಾವ <json-c/json.h>
#ಅನ್ತರ್ಭಾವ <ocispec/json_common.h>

#ಅನ್ತರ್ಭಾವ "mount_flags.h"
#ಅನ್ತರ್ಭಾವ "syscalls.h"

#ifndef RLIMIT_RTTIME
#  define RLIMIT_RTTIME 15
#endif

#ifndef OPEN_TREE_CLONE
#  define OPEN_TREE_CLONE 1
#endif

#ifndef OPEN_TREE_CLOEXEC
#  define OPEN_TREE_CLOEXEC O_CLOEXEC
#endif

#ifndef OPEN_ವೃಕ್ಷ_NAMESPACE
#  define OPEN_ವೃಕ್ಷ_NAMESPACE 2
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
ಸ್ಥಿರ unsigned long
mount_flags_from_statfs (unsigned long f_flags)
{
  unsigned long flags = 0;

  ಯದಿ (f_flags & ST_RDONLY)
    flags |= MS_RDONLY;
  ಯದಿ (f_flags & ST_NOSUID)
    flags |= MS_NOSUID;
  ಯದಿ (f_flags & ST_NODEV)
    flags |= MS_NODEV;
  ಯದಿ (f_flags & ST_NOEXEC)
    flags |= MS_NOEXEC;
  ಯದಿ (f_flags & ST_NOSYMFOLLOW)
    flags |= MS_NOSYMFOLLOW;
  ಯದಿ (f_flags & ST_NOATIME)
    flags |= MS_NOATIME;
  ಯದಿ (f_flags & ST_NODIRATIME)
    flags |= MS_NODIRATIME;
  ಯದಿ (f_flags & ST_RELATIME)
    flags |= MS_RELATIME;
  /* There is no ST_ flag for strictatime: it is the mode when neither noatime
     nor relatime is set.  */
  ಯದಿ (! (flags & (MS_NOATIME | MS_RELATIME)))
    flags |= MS_STRICTATIME;

  ಪ್ರತ್ಯಾಗಮ flags;
}

ಸಂರಚನಾ remount_s
{
  ಸಂರಚನಾ remount_s *next;
  ವರ್ಣ *target;
  ಪೂರ್ಣಾಙ್ಕ targetfd;
  unsigned long flags;
  ವರ್ಣ *data;
};

ಸಂರಚನಾ private_data_s
{
  ಸಂರಚನಾ remount_s *remounts;

  /* Filled by libcrun_run_linux_container().  Useful to query what
     namespaces are available.  */
  ಪೂರ್ಣಾಙ್ಕ unshare_flags;
  ಪೂರ್ಣಾಙ್ಕ unshare_cgroupns;

  ವರ್ಣ *host_notify_socket_path;
  ವರ್ಣ *container_notify_socket_path;
  ವರ್ಣ *unified_cgroup_path;

  bool mount_dev_from_host;
  unsigned long rootfs_propagation;
  bool deny_setgroups;

  ಪೂರ್ಣಾಙ್ಕ procfd;

  ಧ್ರುವ ವರ್ಣ *rootfs;
  ಪೂರ್ಣಾಙ್ಕ rootfsfd;

  ಪೂರ್ಣಾಙ್ಕ notify_socket_ವೃಕ್ಷ_fd;
  ಪೂರ್ಣಾಙ್ಕ old_ಬೇರು_fd;

  ಸಂರಚನಾ libcrun_fd_map *mount_fds;
  ಸಂರಚನಾ libcrun_fd_map *dev_fds;
  ಸಂರಚನಾ libcrun_fd_map *needed_devs_fds;

  ವರ್ಣ **copy_symlink_targets;
  size_t n_copy_symlink_targets;

  /* Used to save stdin, stdout, stderr during checkpointing to descriptors.json
   * and needed during restore. */
  ವರ್ಣ *external_descriptors;

  /* Cached shared empty directory for masked paths optimization */
  ಪೂರ್ಣಾಙ್ಕ maskdir_fd;
  ವರ್ಣ *maskdir_proc_path;
  bool maskdir_bind_failed;
  bool maskdir_move_failed;
  bool maskdir_warned;
  bool joined_mount_ns;
  bool needs_pivot;
  bool no_pivot;
  /* Set once the process root is the container rootfs and the host root
     can no longer be reached by path.  */
  bool host_ಬೇರು_switched;
  /* The process joining an existing PID namespace is created only once
     the container root is in place.  */
  bool delayed_pidns_fork;
  /* The rootfs path on the host, used to recognize bind mount sources
     inside the container rootfs.  */
  ವರ್ಣ *host_rootfs;
};

ಸಂರಚನಾ linux_namespace_s
{
  ಧ್ರುವ ವರ್ಣ *name;
  ಧ್ರುವ ವರ್ಣ *ns_file;
  ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ;
};

ಸ್ಥಿರ ಶೂನ್ಯ
cleanup_private_data (ಶೂನ್ಯ *private_data)
{
  ಸಂರಚನಾ private_data_s *p = private_data;

  ಯದಿ (p->rootfsfd >= 0)
    TEMP_FAILURE_RETRY (close (p->rootfsfd));
  ಯದಿ (p->procfd >= 0)
    TEMP_FAILURE_RETRY (close (p->procfd));
  ಯದಿ (p->maskdir_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->maskdir_fd));
  ಯದಿ (p->old_ಬೇರು_fd >= 0)
    TEMP_FAILURE_RETRY (close (p->old_ಬೇರು_fd));
  ಯದಿ (p->mount_fds)
    cleanup_close_mapp (&(p->mount_fds));
  ಯದಿ (p->dev_fds)
    cleanup_close_mapp (&(p->dev_fds));
  ಯದಿ (p->needed_devs_fds)
    cleanup_close_mapp (&(p->needed_devs_fds));

  ಯದಿ (p->copy_symlink_targets)
    {
      size_t i;
      ಪ್ರತಿ (i = 0; i < p->n_copy_symlink_targets; i++)
        ಮುಕ್ತ (p->copy_symlink_targets[i]);
      ಮುಕ್ತ (p->copy_symlink_targets);
    }
  ಮುಕ್ತ (p->unified_cgroup_path);
  ಮುಕ್ತ (p->host_notify_socket_path);
  ಮುಕ್ತ (p->container_notify_socket_path);
  ಮುಕ್ತ (p->external_descriptors);
  ಮುಕ್ತ (p->maskdir_proc_path);
  ಮುಕ್ತ (p->host_rootfs);
  ಮುಕ್ತ (p);
}

ಸ್ಥಿರ ಸಂರಚನಾ private_data_s *
get_private_data (ಸಂರಚನಾ libcrun_container_s *container)
{
  ಯದಿ (container->private_data == ರಿಕ್ತ)
    {
      ಸಂರಚನಾ private_data_s *p = xmalloc0 (ಪರಿಮಾಣ (*p));
      container->private_data = p;
      p->rootfsfd = -1;
      p->procfd = -1;
      p->notify_socket_ವೃಕ್ಷ_fd = -1;
      p->old_ಬೇರು_fd = -1;
      p->maskdir_fd = -1;
      container->cleanup_private_data = cleanup_private_data;
    }
  ಪ್ರತ್ಯಾಗಮ container->private_data;
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

ಸ್ಥಿರ ಸಂರಚನಾ linux_namespace_s namespaces[] = { { "mount", "mnt", CLONE_NEWNS },
                                                 { "network", "net", CLONE_NEWNET },
                                                 { "ipc", "ipc", CLONE_NEWIPC },
                                                 { "pid", "pid", CLONE_NEWPID },
                                                 { "uts", "uts", CLONE_NEWUTS },
                                                 { "user", "user", CLONE_NEWUSER },
                                                 { "cgroup", "cgroup", CLONE_NEWCGROUP },
                                                 { "time", "time", CLONE_NEWTIME },
                                                 { ರಿಕ್ತ, ರಿಕ್ತ, 0 } };

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_and_reset (ಪೂರ್ಣಾಙ್ಕ *old)
{
  ಪೂರ್ಣಾಙ್ಕ tmp = *old;
  *old = -1;
  ಪ್ರತ್ಯಾಗಮ tmp;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_find_namespace (ಧ್ರುವ ವರ್ಣ *name)
{
  ಸಂರಚನಾ linux_namespace_s *it;
  ಪ್ರತಿ (it = namespaces; it->name; it++)
    ಯದಿ (strcmp (it->name, name) == 0)
      ಪ್ರತ್ಯಾಗಮ it->ಮೌಲ್ಯ;
  ಪ್ರತ್ಯಾಗಮ -1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
syscall_keyctl_join (ಧ್ರುವ ವರ್ಣ *name)
{
#define KEYCTL_JOIN_SESSION_KEYRING 0x1
  ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) syscall (__NR_keyctl, KEYCTL_JOIN_SESSION_KEYRING, name, 0);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
syscall_pidfd_open (pid_t pid, unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
#ಯದಿ defined __NR_pidfd_open
  ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) syscall (__NR_pidfd_open, pid, flags);
#ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) pid;
  (ಶೂನ್ಯ) flags;
  errno = ENOSYS;
  ಪ್ರತ್ಯಾಗಮ -1;
#endif
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
syscall_pidfd_send_signal (ಪೂರ್ಣಾಙ್ಕ pidfd, ಪೂರ್ಣಾಙ್ಕ sig, siginfo_t *info, unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
#ಯದಿ defined __NR_pidfd_send_signal
  ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) syscall (__NR_pidfd_send_signal, pidfd, sig, info, flags);
#ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) pidfd;
  (ಶೂನ್ಯ) sig;
  (ಶೂನ್ಯ) info;
  (ಶೂನ್ಯ) flags;
  errno = ENOSYS;
  ಪ್ರತ್ಯಾಗಮ -1;
#endif
}

/* Translate MS_* mount flags to MOUNT_ATTR_* values for mount_setattr().  */
ಸ್ಥಿರ uint64_t
ms_flags_to_mount_attr (uint64_t ms_flags)
{
  uint64_t attr = 0;

  ಯದಿ (ms_flags & MS_RDONLY)
    attr |= MOUNT_ATTR_RDONLY;
  ಯದಿ (ms_flags & MS_NOSUID)
    attr |= MOUNT_ATTR_NOSUID;
  ಯದಿ (ms_flags & MS_NODEV)
    attr |= MOUNT_ATTR_NODEV;
  ಯದಿ (ms_flags & MS_NOEXEC)
    attr |= MOUNT_ATTR_NOEXEC;
  ಯದಿ (ms_flags & MS_NOATIME)
    attr |= MOUNT_ATTR_NOATIME;
  ಯದಿ (ms_flags & MS_STRICTATIME)
    attr |= MOUNT_ATTR_STRICTATIME;
  ಯದಿ (ms_flags & MS_NODIRATIME)
    attr |= MOUNT_ATTR_NODIRATIME;
  ಯದಿ (ms_flags & MS_NOSYMFOLLOW)
    attr |= MOUNT_ATTR_NOSYMFOLLOW;

  ಪ್ರತ್ಯಾಗಮ attr;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_mount_setattr (bool recursive, ಧ್ರುವ ವರ್ಣ *target, ಪೂರ್ಣಾಙ್ಕ targetfd, uint64_t clear, uint64_t set, libcrun_error_t *err)
{
  ಸಂರಚನಾ mount_attr_s attr = {
    0,
  };
  ಪೂರ್ಣಾಙ್ಕ ret;

  set &= ~MS_BIND;
  clear &= ~MS_BIND;

  attr.propagation = set & ALL_PROPAGATIONS_NO_REC;
  attr.attr_set = ms_flags_to_mount_attr (set & (~ALL_PROPAGATIONS));
  attr.attr_clr = ms_flags_to_mount_attr (clear & (~ALL_PROPAGATIONS));

  /* Like mount(2) does, if any atime flag is set, set all the atime ones
     anew: relatime is the default, noatime and strictatime override it
     (strictatime wins), and nodiratime is independent.  Otherwise, the
     atime flags of the mount are kept.  */
  ಯದಿ (set & (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME))
    {
      attr.attr_set &= ~(MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME);
      ಯದಿ (set & MS_STRICTATIME)
        attr.attr_set |= MOUNT_ATTR_STRICTATIME;
      ಅನ್ಯಥಾ ಯದಿ (set & MS_NOATIME)
        attr.attr_set |= MOUNT_ATTR_NOATIME;
      ಅನ್ಯಥಾ
        attr.attr_set |= MOUNT_ATTR_RELATIME;
      ಯದಿ (set & MS_NODIRATIME)
        attr.attr_set |= MOUNT_ATTR_NODIRATIME;
      attr.attr_clr |= MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME;
    }
  /* The kernel rejects a partial MOUNT_ATTR__ATIME in attr_clr, so clearing
     any of the atime modes resets the mount to the relatime default.  */
  ಅನ್ಯಥಾ ಯದಿ (clear & (MS_NOATIME | MS_RELATIME | MS_STRICTATIME))
    attr.attr_clr |= MOUNT_ATTR__ATIME;

  ret = syscall_mount_setattr (targetfd, "", (recursive ? AT_RECURSIVE : 0) | AT_EMPTY_PATH, &attr);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "mount_setattr `/%s`", target);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
get_bind_mount (ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *src, bool recursive, bool rdonly, bool nofollow, unsigned long propagation, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ open_ವೃಕ್ಷ_fd = -1;
  ಸಂರಚನಾ mount_attr_s attr = {
    0,
  };
  ಪೂರ್ಣಾಙ್ಕ recursive_flag = (recursive ? AT_RECURSIVE : 0);
  ಪೂರ್ಣಾಙ್ಕ ret;

  /* Several callers pass a source straight from the configuration, where it
     is optional, so it can be missing altogether.  Callers that mean the
     directory referred to by dirfd pass an empty string, never NULL.  */
  ಯದಿ (UNLIKELY (src == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EINVAL, "mount source is not specified");

  ಯದಿ (rdonly)
    attr.attr_set = MS_RDONLY;

  attr.propagation = propagation;

  ಯದಿ (dirfd < 0)
    dirfd = AT_FDCWD;

  errno = 0;
  open_ವೃಕ್ಷ_fd = syscall_open_ವೃಕ್ಷ (dirfd, src,
                                    AT_NO_AUTOMOUNT | OPEN_TREE_CLOEXEC
                                        | OPEN_TREE_CLONE | recursive_flag
                                        | (src[0] == '\0' ? AT_EMPTY_PATH : 0)
                                        | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  ಯದಿ (UNLIKELY (open_ವೃಕ್ಷ_fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open_tree `%s`", src);

  ret = syscall_mount_setattr (open_ವೃಕ್ಷ_fd, "", AT_EMPTY_PATH | recursive_flag, &attr);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "mount_setattr `%s`", src);

  ಪ್ರತ್ಯಾಗಮ get_and_reset (&open_ವೃಕ್ಷ_fd);
}

ಪೂರ್ಣಾಙ್ಕ
parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, ವರ್ಣ *option, ವರ್ಣ **out, size_t *len, libcrun_error_t *err)
{
  size_t written = 0, allocated = 256;
  cleanup_free ವರ್ಣ *mappings = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *it;
  ಪೂರ್ಣಾಙ್ಕ ret;

  mappings = xmalloc (allocated);

  ಪ್ರತಿ (it = option; *it;)
    {
      bool relative = false;
      long ಮೌಲ್ಯ[3];
      size_t i;

      ಯದಿ (*it == '\0')
        ಭಙ್ಗ;

      ಯದಿ (*it == '#')
        it++;

      ಯದಿ (*it == '@')
        {
          relative = true;
          it++;
        }

      /* read a triplet: file system id - host id - size.  */
      ಪ್ರತಿ (i = 0; i < 3; i++)
        {
          ವರ್ಣ *endptr = ರಿಕ್ತ;

          ಯದಿ (i > 0 && *it == '-')
            it++;

          ಯದಿ (*it == '\0')
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid mapping specified `%s`", option);

          errno = 0;
          ಮೌಲ್ಯ[i] = strtol (it, &endptr, 10);
          ಯದಿ (errno || endptr == it)
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "invalid mapping specified `%s`", option);

          it = endptr;
        }

      ಯದಿ (relative)
        {
          runtime_spec_schema_defs_id_mapping **mappings;
          size_t mappings_len;

          ಯದಿ (def == ರಿಕ್ತ
              || def->linux == ರಿಕ್ತ
              || (is_uids && def->linux->uid_mappings_len == 0)
              || (! is_uids && def->linux->gid_mappings_len == 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "specified a relative mapping without user namespace mappings");

          mappings_len = (is_uids ? def->linux->uid_mappings_len : def->linux->gid_mappings_len);
          mappings = is_uids ? def->linux->uid_mappings : def->linux->gid_mappings;

          ಪ್ರತಿ (i = 0; i < mappings_len; i++)
            ಯದಿ (ಮೌಲ್ಯ[1] >= (int64_t) mappings[i]->container_id
                && ಮೌಲ್ಯ[1] < (int64_t) mappings[i]->container_id + (int64_t) mappings[i]->size)
              ಭಙ್ಗ;

          ಯದಿ (i == mappings_len)
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "could not find a user namespace mapping for the relative mapping `%s`", option);

          ಮೌಲ್ಯ[1] += mappings[i]->host_id - mappings[i]->container_id;
        }
      ಯದಿ (written > allocated - 64)
        {
          allocated += 256;
          mappings = xrealloc (mappings, allocated);
        }

      ret = snprintf (mappings + written, allocated - written, "%ld %ld %ld\n", ಮೌಲ್ಯ[0], ಮೌಲ್ಯ[1], ಮೌಲ್ಯ[2]);
      ಯದಿ (UNLIKELY (ret >= (ಪೂರ್ಣಾಙ್ಕ) (allocated - written)))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
    }
  *(mappings + written) = '\0';

  *len = written;

  *out = mappings;
  mappings = ರಿಕ್ತ;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
format_mount_mappings (ವರ್ಣ **out,
                       runtime_spec_schema_defs_id_mapping **mappings,
                       size_t mappings_len,
                       size_t *written,
                       libcrun_error_t *err)
{
  /* 64 is more than enough room to print 3 uint32.  */
  ಧ್ರುವ size_t max_len_mapping = 64;
  cleanup_free ವರ್ಣ *ret = ರಿಕ್ತ;
  size_t s;

  *written = 0;

  ret = xmalloc (max_len_mapping * mappings_len + 1);
  ಪ್ರತಿ (s = 0; s < mappings_len; s++)
    {
      size_t len;

      len = snprintf (ret + *written, max_len_mapping, "%" PRIu32 " %" PRIu32 " %" PRIu32 "\n",
                      mappings[s]->container_id,
                      mappings[s]->host_id,
                      mappings[s]->size);
      ಯದಿ (UNLIKELY (len >= max_len_mapping))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: allocated buffer too small");

      *written += len;
    }
  *out = ret;
  ret = ರಿಕ್ತ;
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
format_mount_mapping (ವರ್ಣ **ret, uint32_t container_id, uint32_t host_id,
                      uint32_t size, size_t *written, libcrun_error_t *err)
{
  runtime_spec_schema_defs_id_mapping mapping = {
    .container_id = container_id,
    .host_id = host_id,
    .size = size,
  };
  runtime_spec_schema_defs_id_mapping *mappings[] = {
    &mapping,
    ರಿಕ್ತ,
  };

  ಪ್ರತ್ಯಾಗಮ format_mount_mappings (ret, mappings, 1, written, err);
}

ಸ್ಥಿರ bool
has_same_mappings (runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt)
{
  size_t s;

  ಯದಿ (def->linux == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ mnt->uid_mappings_len == 0 && mnt->gid_mappings_len == 0;

  ಯದಿ (mnt->uid_mappings_len != def->linux->uid_mappings_len)
    ಪ್ರತ್ಯಾಗಮ false;

  ಯದಿ (mnt->gid_mappings_len != def->linux->gid_mappings_len)
    ಪ್ರತ್ಯಾಗಮ false;

  ಪ್ರತಿ (s = 0; s < mnt->uid_mappings_len; s++)
    {
      ಯದಿ (mnt->uid_mappings[s]->container_id != def->linux->uid_mappings[s]->container_id)
        ಪ್ರತ್ಯಾಗಮ false;
      ಯದಿ (mnt->uid_mappings[s]->host_id != def->linux->uid_mappings[s]->host_id)
        ಪ್ರತ್ಯಾಗಮ false;
      ಯದಿ (mnt->uid_mappings[s]->size != def->linux->uid_mappings[s]->size)
        ಪ್ರತ್ಯಾಗಮ false;
    }

  ಪ್ರತಿ (s = 0; s < mnt->gid_mappings_len; s++)
    {
      ಯದಿ (mnt->gid_mappings[s]->container_id != def->linux->gid_mappings[s]->container_id)
        ಪ್ರತ್ಯಾಗಮ false;
      ಯದಿ (mnt->gid_mappings[s]->host_id != def->linux->gid_mappings[s]->host_id)
        ಪ್ರತ್ಯಾಗಮ false;
      ಯದಿ (mnt->gid_mappings[s]->size != def->linux->gid_mappings[s]->size)
        ಪ್ರತ್ಯಾಗಮ false;
    }

  ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ pid_t
maybe_create_userns_for_idmapped_mount (libcrun_container_t *container,
                                        runtime_spec_schema_config_schema *def,
                                        runtime_spec_schema_defs_mount *mnt,
                                        ಧ್ರುವ ವರ್ಣ *options, pid_t *pid_out,
                                        libcrun_error_t *err)
{
  cleanup_pid pid_t pid = -1;
  bool need_new_userns = mnt->uid_mappings_len ? ! has_same_mappings (def, mnt) : options != ರಿಕ್ತ;

  ಯದಿ (! need_new_userns)
    ಪ್ರತ್ಯಾಗಮ 0;

  pid = syscall_clone (CLONE_NEWUSER | SIGCHLD, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (pid < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "clone");

  ಯದಿ (pid == 0)
    {
      prctl (PR_SET_PDEATHSIG, SIGKILL);
      ಯಾವತ್ (1)
        pause ();
      _safe_exit (EXIT_SUCCESS);
    }

  ಯದಿ (mnt->uid_mappings_len)
    {
      cleanup_free ವರ್ಣ *uid_map = ರಿಕ್ತ;
      cleanup_free ವರ್ಣ *gid_map = ರಿಕ್ತ;
      size_t written = 0;
      ಪೂರ್ಣಾಙ್ಕ ret;

      ret = format_mount_mappings (&uid_map, mnt->uid_mappings, mnt->uid_mappings_len, &written, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;

      fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
      ಯದಿ (UNLIKELY (fd < 0))
        ಪ್ರತ್ಯಾಗಮ fd;

      ret = safe_write (fd, "uid_map", uid_map, written, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = format_mount_mappings (&gid_map, mnt->gid_mappings, mnt->gid_mappings_len, &written, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      close_and_reset (&fd);

      fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
      ಯದಿ (UNLIKELY (fd < 0))
        ಪ್ರತ್ಯಾಗಮ fd;

      ret = safe_write (fd, "gid_map", gid_map, written, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಅನ್ಯಥಾ
    {
      cleanup_free ವರ್ಣ *dup_options = ರಿಕ್ತ;
      ವರ್ಣ *option, *saveptr = ರಿಕ್ತ;

      ಯದಿ (! options)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: no mappings found");

      dup_options = xstrdup (options);

      /* If there are no OCI mappings specified, then parse the annotation.  */
      ಪ್ರತಿ (option = strtok_r (dup_options, ";", &saveptr); option; option = strtok_r (ರಿಕ್ತ, ";", &saveptr))
        {
          cleanup_free ವರ್ಣ *mappings = ರಿಕ್ತ;
          cleanup_close ಪೂರ್ಣಾಙ್ಕ map_fd = -1;
          bool is_uids = false;
          size_t len = 0;
          ಪೂರ್ಣಾಙ್ಕ ret;

          ಯದಿ (has_prefix (option, "uids="))
            {
              is_uids = true;
              map_fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
              ಯದಿ (UNLIKELY (map_fd < 0))
                ಪ್ರತ್ಯಾಗಮ map_fd;
            }
          ಅನ್ಯಥಾ ಯದಿ (has_prefix (option, "gids="))
            {
              map_fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
              ಯದಿ (UNLIKELY (map_fd < 0))
                ಪ್ರತ್ಯಾಗಮ map_fd;
            }
          ಅನ್ಯಥಾ
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid option `%s` specified", option);

          ret = parse_idmapped_mount_option (def, is_uids, option + 5 /* strlen ("uids="), strlen ("gids=")*/, &mappings, &len, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;

          ret = safe_write (map_fd, is_uids ? "uid_map" : "gid_map", mappings, len, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }
    }

  *pid_out = pid;
  pid = -1;
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_create_keyring (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *label, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ labelfd = -1;
  bool label_set = false;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (label)
    {
      labelfd = libcrun_open_proc_file (container, "self/attr/keycreate", O_WRONLY, err);
      ಯದಿ (UNLIKELY (labelfd < 0))
        {
          ಯದಿ (crun_error_get_errno (err) != ENOENT)
            ಪ್ರತ್ಯಾಗಮ labelfd;

          crun_error_release (err);
        }

      ಯದಿ (labelfd >= 0)
        {
          ret = ಲಿಖ (labelfd, label, strlen (label));
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "write to `/proc/self/attr/keycreate`");

          label_set = true;
        }
    }

  ret = syscall_keyctl_join (name);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ಯದಿ (errno == ENOSYS)
        {
          libcrun_warning ("could not create a new keyring: keyctl_join is not supported");
          ret = 0;
          ಗಚ್ಛ out;
        }
      ret = crun_make_error (err, errno, "join keyctl `%s`", name ? name : "");
      ಗಚ್ಛ out;
    }

out:
  /* Best effort attempt to reset the SELinux label used for new keyrings.  */
  ಯದಿ (label_set && ಲಿಖ (labelfd, "", 0) < 0)
    {
      /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
    }
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ
get_uid_gid_from_def (runtime_spec_schema_config_schema *def, uid_t *uid, gid_t *gid)
{
  *uid = 0;
  *gid = 0;

  ಯದಿ (def->process && def->process->user)
    {
      ಯದಿ (def->process->user->uid)
        *uid = def->process->user->uid;
      ಯದಿ (def->process->user->gid)
        *gid = def->process->user->gid;
    }
}

ಸ್ಥಿರ unsigned long
get_mount_flags (ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ current_flags, ಪೂರ್ಣಾಙ್ಕ *found, unsigned long *extra_flags, uint64_t *rec_clear, uint64_t *rec_set)
{
  ಧ್ರುವ ಸಂರಚನಾ propagation_flags_s *prop;

  prop = libcrun_str2mount_flags (name);

  ಯದಿ (found)
    *found = prop ? 1 : 0;

  ಯದಿ (prop == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯದಿ (prop->extra_flags & OPTION_RECURSIVE)
    {
      ಯದಿ (rec_clear && prop->clear)
        *rec_clear |= prop->flags;

      ಯದಿ (rec_set && ! prop->clear)
        *rec_set |= prop->flags;
    }

  ಯದಿ (extra_flags)
    *extra_flags |= prop->extra_flags;

  ಯದಿ (prop->clear)
    ಪ್ರತ್ಯಾಗಮ current_flags & ~prop->flags;

  ಪ್ರತ್ಯಾಗಮ current_flags | prop->flags;
}

/* Whether any of the mount options sets or clears a mount flag, other
   than MS_BIND, MS_REC and the propagation ones.  */
ಸ್ಥಿರ bool
has_mount_flag_options (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  ಪ್ರತಿ (i = 0; i < mount->options_len; i++)
    {
      ಧ್ರುವ ಸಂರಚನಾ propagation_flags_s *prop;

      ಯದಿ (mount->options[i] == ರಿಕ್ತ)
        ಅನುವರ್ತ;

      prop = libcrun_str2mount_flags (mount->options[i]);
      ಯದಿ (prop && (prop->flags & ~(MS_BIND | ALL_PROPAGATIONS)))
        ಪ್ರತ್ಯಾಗಮ true;
    }

  ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ unsigned long
get_mount_flags_or_option (ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ current_flags, unsigned long *extra_flags, ವರ್ಣ **option, uint64_t *rec_clear, uint64_t *rec_set)
{
  ಪೂರ್ಣಾಙ್ಕ found;
  __attribute__ ((unused)) cleanup_free ವರ್ಣ *prev = ರಿಕ್ತ;
  unsigned long flags;

  /* A null element of the array of options in the configuration is parsed
     into a NULL, which is not an option to be honored.  */
  ಯದಿ (name == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  flags = get_mount_flags (name, current_flags, &found, extra_flags, rec_clear, rec_set);
  ಯದಿ (found)
    {
      /* MS_SYNCHRONOUS and MS_DIRSYNC cannot be set through fsmount()
         attr_flags.  Pass them also as data so that fsconfig(SET_FLAG)
         can set SB_SYNCHRONOUS / SB_DIRSYNC on the superblock.  */
      ಧ್ರುವ ವರ್ಣ *data_flag = ರಿಕ್ತ;
      ಯದಿ ((flags & MS_SYNCHRONOUS) && ! (current_flags & MS_SYNCHRONOUS))
        data_flag = "sync";
      ಅನ್ಯಥಾ ಯದಿ ((flags & MS_DIRSYNC) && ! (current_flags & MS_DIRSYNC))
        data_flag = "dirsync";

      ಯದಿ (data_flag)
        {
          prev = *option;
          ಯದಿ (*option && **option)
            xasprintf (option, "%s,%s", *option, data_flag);
          ಅನ್ಯಥಾ
            *option = xstrdup (data_flag);
        }
      ಪ್ರತ್ಯಾಗಮ flags;
    }

  prev = *option;
  ಯದಿ (*option && **option)
    xasprintf (option, "%s,%s", *option, name);
  ಅನ್ಯಥಾ
    *option = xstrdup (name);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
pivot_ಬೇರು (ಧ್ರುವ ವರ್ಣ *new_ಬೇರು, ಧ್ರುವ ವರ್ಣ *put_old)
{
  ಪ್ರತ್ಯಾಗಮ syscall (__NR_pivot_root, new_ಬೇರು, put_old);
}

ಸ್ಥಿರ ಶೂನ್ಯ
free_remount (ಸಂರಚನಾ remount_s *r)
{
  ಯದಿ (r == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ;
  ಯದಿ (r->targetfd >= 0)
    close (r->targetfd);
  ಮುಕ್ತ (r->data);
  ಮುಕ್ತ (r->target);
  ಮುಕ್ತ (r);
}

ಸ್ಥಿರ ಸಂರಚನಾ remount_s *
make_remount (ಪೂರ್ಣಾಙ್ಕ targetfd, ಧ್ರುವ ವರ್ಣ *target, unsigned long flags, ಧ್ರುವ ವರ್ಣ *data, ಸಂರಚನಾ remount_s *next)
{
  ಸಂರಚನಾ remount_s *ret = xmalloc (ಪರಿಮಾಣ (*ret));
  ret->target = xstrdup (target);
  ret->flags = flags;
  ret->data = data ? xstrdup (data) : ರಿಕ್ತ;
  ret->next = next;
  ret->targetfd = targetfd;
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_remount (ಪೂರ್ಣಾಙ್ಕ targetfd, ಧ್ರುವ ವರ್ಣ *target, unsigned long flags, ಧ್ರುವ ವರ್ಣ *data, libcrun_error_t *err)
{
  proc_fd_path_t target_buffer;
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಧ್ರುವ ವರ್ಣ *real_target = target;

  ಯದಿ (targetfd >= 0)
    {
      unsigned long set_flags = flags & ~(MS_REMOUNT | MS_BIND);
      unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;
      ret = do_mount_setattr (false, target, targetfd, clear_flags, flags & ~MS_REMOUNT, err);
      ಯದಿ (LIKELY (ret == 0))
        ಪ್ರತ್ಯಾಗಮ 0;
      crun_error_release (err);

      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  /* Older kernels (seen on 4.18) fail with EINVAL if data is set when
     setting MS_RDONLY.  */
  ಯದಿ (flags & (MS_REMOUNT | MS_RDONLY))
    data = ರಿಕ್ತ;

  ret = mount (ರಿಕ್ತ, real_target, ರಿಕ್ತ, flags, data);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      unsigned long current_flags, remount_flags;
      ಸಂರಚನಾ statfs sfs;

      ret = statfs (real_target, &sfs);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "statfs `%s`", real_target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);

      remount_flags = current_flags & (MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW);
      ಯದಿ (! (flags & MOUNT_ATIME_FLAGS))
        remount_flags |= current_flags & MOUNT_ATIME_FLAGS;

      ಯದಿ ((flags | remount_flags) != flags)
        {
          ret = mount (ರಿಕ್ತ, real_target, ರಿಕ್ತ, flags | remount_flags, data);
          ಯದಿ (LIKELY (ret == 0))
            ಪ್ರತ್ಯಾಗಮ 0;

          /* If it still fails and MS_RDONLY is present in the mount, try adding it.  */
          ಯದಿ (current_flags & MS_RDONLY)
            ret = mount (ರಿಕ್ತ, real_target, ರಿಕ್ತ, flags | remount_flags | MS_RDONLY, data);
        }
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "remount `%s`", target);
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
finalize_mounts (libcrun_container_t *container, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret = 0;
  ಸಂರಚನಾ remount_s *r = get_private_data (container)->remounts;
  ಯಾವತ್ (r)
    {
      ಸಂರಚನಾ remount_s *next = r->next;

      /* Try mount_setattr() first to avoid the statfs+retry fallback.  */
      ಯದಿ (r->targetfd >= 0 && (r->flags & MS_RDONLY))
        {
          /* Clear the flags which are not set, like do_remount does.  */
          unsigned long set_flags = r->flags & ~(MS_REMOUNT | MS_BIND);
          unsigned long clear_flags = REMOUNT_CLEAR_FLAGS & ~set_flags;

          ret = do_mount_setattr (false, r->target, r->targetfd, clear_flags, r->flags & ~MS_REMOUNT, err);
          ಯದಿ (LIKELY (ret == 0))
            {
              free_remount (r);
              r = next;
              ಅನುವರ್ತ;
            }
          crun_error_release (err);
        }

      ret = do_remount (r->targetfd, r->target, r->flags, r->data, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಗಚ್ಛ cleanup;

      free_remount (r);
      r = next;
    }

cleanup:
  ಯಾವತ್ (r)
    {
      ಸಂರಚನಾ remount_s *next = r->next;
      free_remount (r);
      r = next;
    }

  get_private_data (container)->remounts = ರಿಕ್ತ;
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
open_mount_target (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *target_rel, libcrun_error_t *err)
{
  ಧ್ರುವ ವರ್ಣ *rootfs = get_private_data (container)->rootfs;
  ಪೂರ್ಣಾಙ್ಕ rootfsfd = get_private_data (container)->rootfsfd;

  ಯದಿ (rootfsfd < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid rootfs state");

  ಪ್ರತ್ಯಾಗಮ safe_openat (rootfsfd, rootfs, target_rel, O_PATH | O_CLOEXEC, 0, err);
}

/* Attempt to open a mount of the specified type.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
fsopen_mount (ಧ್ರುವ ವರ್ಣ *type, ಧ್ರುವ ವರ್ಣ *source_name, ಧ್ರುವ ವರ್ಣ *labeltype, ಧ್ರುವ ವರ್ಣ *label, ಧ್ರುವ ವರ್ಣ *data)
{
#ifdef HAVE_NEW_MOUNT_API
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fsfd = -1;
  ಪೂರ್ಣಾಙ್ಕ ret;

  fsfd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  ಯದಿ (UNLIKELY (fsfd < 0))
    ಪ್ರತ್ಯಾಗಮ fsfd;

  /* Best-effort: pseudo file systems (tmpfs, proc, ...) ignore "source",
     so a failure here is expected and harmless.  Real failures surface
     at FSCONFIG_CMD_CREATE below.  */
  (ಶೂನ್ಯ) syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, "source", source_name ? source_name : type, 0);

  ಯದಿ (labeltype)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, labeltype, label, 0);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (data && data[0])
    {
      cleanup_free ವರ್ಣ *data_copy = xstrdup (data);
      ವರ್ಣ *saveptr = ರಿಕ್ತ;
      ವರ್ಣ *token;

      ಪ್ರತಿ (token = strtok_r (data_copy, ",", &saveptr); token; token = strtok_r (ರಿಕ್ತ, ",", &saveptr))
        {
          ವರ್ಣ *eq = strchr (token, '=');
          ಯದಿ (eq)
            {
              *eq = '\0';

              ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, token, eq + 1, 0);
            }
          ಅನ್ಯಥಾ
            ret = syscall_fsconfig (fsfd, FSCONFIG_SET_FLAG, token, ರಿಕ್ತ, 0);

          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }
    }

  ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, ರಿಕ್ತ, ರಿಕ್ತ, 0);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
#ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) type;
  (ಶೂನ್ಯ) source_name;
  (ಶೂನ್ಯ) data;
  errno = ENOSYS;
  ಪ್ರತ್ಯಾಗಮ -1;
#endif
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_procfd (ಸಂರಚನಾ private_data_s *data, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ fd;

  ಯದಿ (data->procfd >= 0)
    ಪ್ರತ್ಯಾಗಮ data->procfd;

  fd = fsopen_mount ("proc", ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ);
  ಯದಿ (fd < 0)
    fd = open ("/proc", O_DIRECTORY | O_CLOEXEC);
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `/proc`");

  data->procfd = fd;
  ಪ್ರತ್ಯಾಗಮ fd;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_old_ಬೇರು_fd (ಸಂರಚನಾ private_data_s *data)
{
  ಯದಿ (data->old_ಬೇರು_fd >= 0)
    ಪ್ರತ್ಯಾಗಮ data->old_ಬೇರು_fd;

  data->old_ಬೇರು_fd = open ("/", O_PATH | O_CLOEXEC);
  ಪ್ರತ್ಯಾಗಮ data->old_ಬೇರು_fd;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
fs_move_mount_to (ಪೂರ್ಣಾಙ್ಕ fd, ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *name)
{
#ifdef HAVE_NEW_MOUNT_API
  ಯದಿ (name)
    ಪ್ರತ್ಯಾಗಮ syscall_move_mount (fd, "", dirfd, name, MOVE_MOUNT_F_EMPTY_PATH);

  ಪ್ರತ್ಯಾಗಮ syscall_move_mount (fd, "", dirfd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
#ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) fd;
  (ಶೂನ್ಯ) dirfd;
  (ಶೂನ್ಯ) name;
  (ಶೂನ್ಯ) syscall_move_mount;
  errno = ENOSYS;
  ಪ್ರತ್ಯಾಗಮ -1;
#endif
}

ಗಣನಾ
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

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ do_mount (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *source, ಪೂರ್ಣಾಙ್ಕ targetfd,
                     ಧ್ರುವ ವರ್ಣ *target, ಧ್ರುವ ವರ್ಣ *fstype, unsigned long mountflags,
                     ಧ್ರುವ ಶೂನ್ಯ *data, ಪೂರ್ಣಾಙ್ಕ extra_flags, libcrun_error_t *err);

ಸ್ಥಿರ bool
mount_option_exists (runtime_spec_schema_defs_mount *mnt, ಧ್ರುವ ವರ್ಣ *option)
{
  size_t i;
  ಪ್ರತಿ (i = 0; i < mnt->options_len; i++)
    ಯದಿ (mnt->options[i] != ರಿಕ್ತ && strcmp (mnt->options[i], option) == 0)
      ಪ್ರತ್ಯಾಗಮ true;
  ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ runtime_spec_schema_defs_mount *
find_mount_for (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *destination)
{
  size_t i;
  runtime_spec_schema_config_schema *def = container->container_def;

  ಪ್ರತಿ (i = 0; i < def->mounts_len; i++)
    {
      ಯದಿ (strcmp (def->mounts[i]->destination, destination) == 0)
        ಪ್ರತ್ಯಾಗಮ def->mounts[i];
    }
  ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
warn_tmpfs_fallback_once (ಸಂರಚನಾ private_data_s *private_data, ಧ್ರುವ ವರ್ಣ *reason)
{
  ಯದಿ (! private_data->maskdir_warned)
    {
      libcrun_warning ("Falling back to tmpfs for masked dirs (reason: %s)", reason);
      private_data->maskdir_warned = true;
    }
}

/* Get or create the cached shared empty directory for masked paths optimization.
 * Creates directory and FD once per container, caches /proc/self/fd path for fast mounting.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_shared_empty_dir_cached (libcrun_container_t *container, ವರ್ಣ **proc_fd_path, libcrun_error_t *err)
{
  ಸಂರಚನಾ private_data_s *private_data = get_private_data (container);
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
  cleanup_free ವರ್ಣ *empty_dir_path = ರಿಕ್ತ;
  proc_fd_path_t fd_path;
  ಪೂರ್ಣಾಙ್ಕ ret;

  /* Fast path: return cached proc fd path if already set up */
  ಯದಿ (private_data->maskdir_proc_path != ರಿಕ್ತ)
    {
      ಯದಿ (proc_fd_path)
        *proc_fd_path = private_data->maskdir_proc_path;
      ಪ್ರತ್ಯಾಗಮ 0;
    }

  /* Slow path: create directory and cache everything once */
  ret = get_shared_empty_directory_path (&empty_dir_path, container->context->state_ಬೇರು, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* Open directory and cache FD (once per container) */
  fd = get_bind_mount (-1, empty_dir_path, true, true, false, MS_PRIVATE, err);
  ಯದಿ (fd < 0)
    ಪ್ರತ್ಯಾಗಮ fd;

  get_self_fd_path (fd_path, fd);
  private_data->maskdir_proc_path = xstrdup (fd_path);

  private_data->maskdir_fd = fd;
  fd = -1; /* Don't auto-close */

  ಯದಿ (proc_fd_path)
    *proc_fd_path = private_data->maskdir_proc_path;
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ bool
is_dev_null (ಧ್ರುವ ಸಂರಚನಾ stat *st)
{
  /* 1:3 is the null device, see Documentation/admin-guide/devices.txt.  */
  ಪ್ರತ್ಯಾಗಮ S_ISCHR (st->st_mode) && st->st_rdev == makedev (1, 3);
}

/* Check that fd refers to the null device.  Once the root is the container
   rootfs, /dev/null is whatever the rootfs provides there.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
ensure_dev_null (ಪೂರ್ಣಾಙ್ಕ fd, libcrun_error_t *err)
{
  ಸಂರಚನಾ stat st;

  ಯದಿ (UNLIKELY (fstat (fd, &st) < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "stat `/dev/null`");

  ಯದಿ (UNLIKELY (! is_dev_null (&st)))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "`/dev/null` is not the null device");

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Whether FD is the root of a read-only tmpfs that can hold no inode
   other than its root, so it is safe to show in place of any directory.  */
ಸ್ಥಿರ bool
is_empty_ro_tmpfs (ಪೂರ್ಣಾಙ್ಕ fd)
{
  ಸಂರಚನಾ statfs sfs;

  ಯದಿ (fstatfs (fd, &sfs) < 0)
    ಪ್ರತ್ಯಾಗಮ false;

  ಪ್ರತ್ಯಾಗಮ sfs.f_type == TMPFS_MAGIC && (sfs.f_flags & ST_RDONLY) && sfs.f_files == 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
mount_masked_dir (libcrun_container_t *container, ಪೂರ್ಣಾಙ್ಕ pathfd, ಧ್ರುವ ವರ್ಣ *rel_path, libcrun_error_t *err)
{
  ಸಂರಚನಾ private_data_s *private_data = get_private_data (container);
  cleanup_close ಪೂರ್ಣಾಙ್ಕ newfd = -1;
  libcrun_error_t tmp_err = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (private_data->maskdir_fd < 0)
    {
      ಯದಿ (private_data->maskdir_bind_failed)
        ಗಚ್ಛ fallback_to_tmpfs;

      ret = get_shared_empty_dir_cached (container, ರಿಕ್ತ, &tmp_err);
      ಯದಿ (ret < 0)
        {
          private_data->maskdir_bind_failed = true;
          warn_tmpfs_fallback_once (private_data, tmp_err->ಸಂದೇಶ);
          crun_error_release (&tmp_err);
          ಗಚ್ಛ fallback_to_tmpfs;
        }
    }

  ಯದಿ (private_data->maskdir_fd >= 0)
    {
      ಪೂರ್ಣಾಙ್ಕ rootfsfd = private_data->rootfsfd;
      ಸಂರಚನಾ stat before, after;

      ret = fstat (private_data->maskdir_fd, &before);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

      ret = fs_move_mount_to (private_data->maskdir_fd, pathfd, ರಿಕ್ತ);
      ಯದಿ (LIKELY (ret == 0))
        {
          newfd = get_bind_mount (rootfsfd, rel_path, true, true, false, MS_PRIVATE, err);
          ಯದಿ (UNLIKELY (newfd < 0))
            ಪ್ರತ್ಯಾಗಮ newfd;

          ret = fstat (newfd, &after);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fstat masked dir `%s`", rel_path);

          ಯದಿ (before.st_dev != after.st_dev || before.st_ino != after.st_ino)
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "race condition detected remounting masked path `/%s`", rel_path);

          TEMP_FAILURE_RETRY (close (private_data->maskdir_fd));
          private_data->maskdir_fd = newfd;
          newfd = -1;

          ಪ್ರತ್ಯಾಗಮ 0;
        }

      /* tmp_err is NULL here: get_shared_empty_dir_cached() succeeded and did not set it. */
      crun_make_error (&tmp_err, errno, "move mount masked dir `%s`", rel_path);

      TEMP_FAILURE_RETRY (close (private_data->maskdir_fd));
      private_data->maskdir_fd = -1;
      private_data->maskdir_move_failed = true;
    }

  private_data->maskdir_bind_failed = true;
  warn_tmpfs_fallback_once (private_data, tmp_err ? tmp_err->ಸಂದೇಶ : "unknown error");
  crun_error_release (&tmp_err);

fallback_to_tmpfs:
  libcrun_debug ("using tmpfs fallback for %s", rel_path);
  ret = do_mount (container, "tmpfs", pathfd, rel_path, "tmpfs", MS_RDONLY, "nr_blocks=1,nr_inodes=1",
                  LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* Reuse this tmpfs for the other masked directories, so that they do not
     need a tmpfs instance each.  */
  ಯದಿ (! private_data->maskdir_move_failed)
    {
      ret = get_bind_mount (private_data->rootfsfd, rel_path, true, true, false, MS_PRIVATE, &tmp_err);
      ಯದಿ (ret >= 0 && is_empty_ro_tmpfs (ret))
        private_data->maskdir_fd = ret;
      ಅನ್ಯಥಾ
        {
          ಯದಿ (ret >= 0)
            TEMP_FAILURE_RETRY (close (ret));
          private_data->maskdir_move_failed = true;
          crun_error_release (&tmp_err);
        }
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_masked_or_readonly_path (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *rel_path, bool readonly, bool keep_flags,
                            libcrun_error_t *err)
{
  unsigned long mount_flags = 0;
  ಧ್ರುವ ವರ್ಣ *rootfs = get_private_data (container)->rootfs;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ pathfd = -1;
  ಸಂರಚನಾ statfs sfs;
  ಪೂರ್ಣಾಙ್ಕ ret;
  mode_t mode;

  /* The paths come from linux.maskedPaths and linux.readonlyPaths, so a null
     entry in either array reaches us as a NULL pointer.  */
  ಯದಿ (UNLIKELY (rel_path == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EINVAL, "invalid null path in `maskedPaths` or `readonlyPaths`");

  ಯದಿ (rel_path[0] == '/')
    rel_path++;

  pathfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, rel_path, O_PATH | O_CLOEXEC, 0, err);
  ಯದಿ (UNLIKELY (pathfd < 0))
    {
      errno = crun_error_get_errno (err);
      ಯದಿ (errno != ENOENT && errno != EACCES)
        ಪ್ರತ್ಯಾಗಮ pathfd;

      ಯದಿ (errno == EACCES)
        libcrun_debug ("skipping inaccessible path `%s`", rel_path);
      crun_error_release (err);
      ಪ್ರತ್ಯಾಗಮ 0;
    }

  ಯದಿ (readonly)
    {
      ಪೂರ್ಣಾಙ್ಕ procfd = get_procfd (get_private_data (container), err);
      proc_fd_path_t source_buffer;
      cleanup_close ಪೂರ್ಣಾಙ್ಕ mountfd = -1;

      ಯದಿ (UNLIKELY (procfd < 0))
        ಪ್ರತ್ಯಾಗಮ procfd;

      get_self_fd_path (source_buffer, pathfd);

      /* Try open_tree + mount_setattr to apply MS_RDONLY atomically.
         Only when keep_flags is false, since keep_flags needs statfs
         to inherit parent mount flags.  */
      ಯದಿ (! keep_flags)
        {
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          ಯದಿ (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, ರಿಕ್ತ);
          ಅನ್ಯಥಾ
            ret = -1;

          ಯದಿ (ret < 0)
            crun_error_release (err);
        }

      ಯದಿ (keep_flags || ret < 0)
        {
          mount_flags = MS_BIND | MS_PRIVATE | MS_RDONLY | MS_REC;
          ಯದಿ (keep_flags)
            {
              cleanup_close ಪೂರ್ಣಾಙ್ಕ sfd = openat (procfd, source_buffer, O_PATH | O_CLOEXEC);
              ಯದಿ (UNLIKELY (sfd < 0))
                ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "openat procfd for `%s`", rel_path);

              ret = fstatfs (sfd, &sfs);
              ಯದಿ (UNLIKELY (ret < 0))
                ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fstatfs `%s`", rel_path);
              mount_flags = mount_flags | sfs.f_flags;

              /* Parent might contain MS_REMOUNT but the new readonly path is not
                 actually mounted.  Specifically in the case of /proc this will end
                 up with EINVAL therefore remove MS_REMOUNT if it is getting
                 inherited from the parent.  */
              mount_flags = mount_flags & ~MS_REMOUNT;
            }

          close_and_reset (&mountfd);
          mountfd = get_bind_mount (procfd, source_buffer, true, true, false, MS_PRIVATE, err);
          ಯದಿ (mountfd >= 0)
            {
              ret = do_mount_setattr (true, rel_path, mountfd, 0, mount_flags & ~(MS_BIND | MS_PRIVATE), err);
              ಯದಿ (UNLIKELY (ret < 0))
                ಪ್ರತ್ಯಾಗಮ ret;

              ret = fs_move_mount_to (mountfd, pathfd, ರಿಕ್ತ);
              ಯದಿ (UNLIKELY (ret < 0))
                ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "move mount for readonly path `%s`", rel_path);
            }
          ಅನ್ಯಥಾ
            {
              proc_fd_path_t abs_source;

              crun_error_release (err);
              get_proc_self_fd_path (abs_source, pathfd);
              ret = do_mount (container, abs_source, pathfd, rel_path, ರಿಕ್ತ, mount_flags, ರಿಕ್ತ,
                              LABEL_NONE, err);
              ಯದಿ (UNLIKELY (ret < 0))
                ಪ್ರತ್ಯಾಗಮ ret;
            }
        }
    }
  ಅನ್ಯಥಾ
    {
      ret = get_file_type_fd (pathfd, &mode);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot stat `%s`", rel_path);

      ಯದಿ ((mode & S_IFMT) == S_IFDIR)
        ret = mount_masked_dir (container, pathfd, rel_path, err);
      ಅನ್ಯಥಾ
        {
          cleanup_close ಪೂರ್ಣಾಙ್ಕ nullfd = -1;
          cleanup_close ಪೂರ್ಣಾಙ್ಕ mountfd = -1;

          /* If the root is already the container rootfs, this is its
             /dev/null: mask with it only if it is the null device.  */
          nullfd = open ("/dev/null", O_PATH | O_CLOEXEC);
          ಯದಿ (UNLIKELY (nullfd < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `/dev/null`");

          ret = ensure_dev_null (nullfd, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;

          mountfd = get_bind_mount (nullfd, "", false, true, false, MS_PRIVATE, err);
          ಯದಿ (mountfd >= 0)
            ret = fs_move_mount_to (mountfd, pathfd, ರಿಕ್ತ);

          ಯದಿ (mountfd < 0 || ret < 0)
            {
              proc_fd_path_t null_path;

              crun_error_release (err);
              get_proc_self_fd_path (null_path, nullfd);
              ret = do_mount (container, null_path, pathfd, rel_path, ರಿಕ್ತ, MS_BIND | MS_RDONLY, ರಿಕ್ತ,
                              LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
            }
        }
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
get_selinux_context_type (libcrun_container_t *container, libcrun_error_t *err)
{
  ಧ್ರುವ ವರ್ಣ *context_type;

  context_type = find_annotation (container, "run.oci.mount_context_type");
  ಯದಿ (context_type)
    {
      ಯದಿ (strcmp (context_type, "context") != 0
          && strcmp (context_type, "fscontext") != 0
          && strcmp (context_type, "defcontext") != 0
          && strcmp (context_type, "rootcontext") != 0)
        {
          crun_make_error (err, 0, "invalid mount context type `%s` (must be one of: context, fscontext, defcontext, rootcontext)", context_type);
          ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }
      ಪ್ರತ್ಯಾಗಮ context_type;
    }

  ಪ್ರತ್ಯಾಗಮ "context";
}

/* improve the error message on mount(2) failures.  It always returns the original ret.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
diagnose_mount_failure (ಪೂರ್ಣಾಙ್ಕ ret, libcrun_error_t *err, libcrun_container_t *container,
                        ಧ್ರುವ ವರ್ಣ *fstype)
{
  ಯದಿ (fstype && strcmp (fstype, "cgroup2") == 0 && get_private_data (container)->unified_cgroup_path)
    {
      ಯದಿ (access (get_private_data (container)->unified_cgroup_path, R_OK) < 0 && errno == EACCES)
        {
          /* The current cgroup is not accessible.  */
          crun_error_wrap (err, "the current cgroup is not accessible (too restrictive umask?)");
          ಪ್ರತ್ಯಾಗಮ ret;
        }
    }
  ಪ್ರತ್ಯಾಗಮ ret;
}

/* When creating a fresh sysfs mount fails inside a user namespace, fall back to
   bind mounting the host /sys.  Returns 0 when the fallback handled the mount,
   > 0 when it does not apply (the caller must report the original failure),
   < 0 on error.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
sysfs_userns_fallback (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *target,
                       ಧ್ರುವ ವರ್ಣ *real_target, ಪೂರ್ಣಾಙ್ಕ targetfd,
                       unsigned long mountflags, ಧ್ರುವ ವರ್ಣ *fstype,
                       libcrun_error_t *err)
{
  runtime_spec_schema_defs_mount *cgroup_mount;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ mountfd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ sysfd = -1;
  ಪೂರ್ಣಾಙ್ಕ sys_old_ಬೇರು_fd = -1;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (! ((mountflags & MS_RDONLY) && targetfd > 0 && fstype && strcmp (fstype, "sysfs") == 0))
    ಪ್ರತ್ಯಾಗಮ 1;

  /* If we are running in an user namespace, just bind mount /sys if creating
     sysfs failed.  */
  ret = check_running_in_user_namespace (err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (ret == 0)
    ಪ್ರತ್ಯಾಗಮ 1;

  cgroup_mount = find_mount_for (container, "/sys/fs/cgroup");
  sys_old_ಬೇರು_fd = get_old_ಬೇರು_fd (get_private_data (container));

  /* The bind brings the host mounts below /sys along.  The later do_remount
     only affects the top mount, so make the whole tree read-only here.  */
  ಯದಿ (sys_old_ಬೇರು_fd >= 0)
    mountfd = get_bind_mount (sys_old_ಬೇರು_fd, "sys", true, true, false, MS_PRIVATE, err);
  ಯದಿ (mountfd < 0)
    {
      crun_error_release (err);
      mountfd = get_bind_mount (AT_FDCWD, "/sys", true, true, false, MS_PRIVATE, err);
    }

  ಯದಿ (cgroup_mount == ರಿಕ್ತ)
    {
      ಯದಿ (mountfd >= 0)
        {
          ret = fs_move_mount_to (mountfd, targetfd, ರಿಕ್ತ);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "move mount `/sys` to `%s`", real_target);
        }
      ಅನ್ಯಥಾ
        {
          crun_error_release (err);
          ಯದಿ (sys_old_ಬೇರು_fd >= 0)
            {
              cleanup_free ವರ್ಣ *sys_path = ರಿಕ್ತ;
              proc_fd_path_t sys_ಬೇರು_path;

              get_proc_self_fd_path (sys_ಬೇರು_path, sys_old_ಬೇರು_fd);
              xasprintf (&sys_path, "%s/sys", sys_ಬೇರು_path);
              ret = mount (sys_path, real_target, ರಿಕ್ತ, MS_BIND | MS_REC, ರಿಕ್ತ);
            }
          ಅನ್ಯಥಾ
            ret = mount ("/sys", real_target, ರಿಕ್ತ, MS_BIND | MS_REC, ರಿಕ್ತ);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "bind mount `/sys` from the host");
        }

      sysfd = open_mount_target (container, target, err);
      ಯದಿ (UNLIKELY (sysfd < 0))
        ಪ್ರತ್ಯಾಗಮ sysfd;

      ret = do_remount (sysfd, target,
                        MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                        ರಿಕ್ತ, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ಪ್ರತ್ಯಾಗಮ do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
    }

  ಯದಿ (UNLIKELY (mountfd < 0))
    ಪ್ರತ್ಯಾಗಮ mountfd;

  ret = fs_move_mount_to (mountfd, targetfd, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "move mount to `%s`", real_target);

  sysfd = open_mount_target (container, target, err);
  ಯದಿ (UNLIKELY (sysfd < 0))
    ಪ್ರತ್ಯಾಗಮ sysfd;

  ret = do_remount (sysfd, target,
                    MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                    ರಿಕ್ತ, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (mount_option_exists (cgroup_mount, "ro") || mount_option_exists (cgroup_mount, "rro"))
    {
      ret = do_masked_or_readonly_path (container, "/sys/fs/cgroup", true, false, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

#ifdef HAVE_FGETXATTR
/* Best-effort application of the SELinux security.selinux xattr to a
   freshly mounted target when LABEL_XATTR labeling is requested.  */
ಸ್ಥಿರ ಶೂನ್ಯ
apply_selinux_xattr (libcrun_container_t *container, ಪೂರ್ಣಾಙ್ಕ fd, ಪೂರ್ಣಾಙ್ಕ extra_flags, ಧ್ರುವ ವರ್ಣ *label)
{
  libcrun_error_t tmp_err = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ procfd;

  ಯದಿ ((extra_flags & LABEL_MASK) != LABEL_XATTR)
    ಪ್ರತ್ಯಾಗಮ;

  /* Best-effort: swallow any error so it does not leak into the caller.  */
  procfd = get_procfd (get_private_data (container), &tmp_err);
  ಯದಿ (procfd >= 0)
    {
      proc_fd_path_t proc_self_path;
      cleanup_close ಪೂರ್ಣಾಙ್ಕ xfd = -1;

      get_self_fd_path (proc_self_path, fd);
      xfd = openat (procfd, proc_self_path, O_RDONLY | O_CLOEXEC);
      ಯದಿ (xfd >= 0)
        (ಶೂನ್ಯ) fsetxattr (xfd, "security.selinux", label, strlen (label), 0);
    }
  crun_error_release (&tmp_err);
}
#endif

/* After a mount replaced the rootfs (target is empty), the current
   targetfd sits underneath the new mount.  Re-enter the mount namespace
   and reopen the rootfs, refreshing both *fd and the cached rootfsfd.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_mount_replace_rootfs (libcrun_container_t *container, ಪೂರ್ಣಾಙ್ಕ *fd, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ procfd = get_procfd (get_private_data (container), err);
  ಪೂರ್ಣಾಙ್ಕ tmp;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (UNLIKELY (procfd < 0))
    ಪ್ರತ್ಯಾಗಮ procfd;

  {
    cleanup_close ಪೂರ್ಣಾಙ್ಕ nsfd = openat (procfd, "self/ns/mnt", O_RDONLY | O_CLOEXEC);
    ಯದಿ (UNLIKELY (nsfd < 0))
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `/proc/self/ns/mnt`");

    ret = setns (nsfd, CLONE_NEWNS);
    ಯದಿ (UNLIKELY (ret < 0))
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "setns `CLONE_NEWNS`");
  }

  close_and_reset (fd);
  *fd = open (get_private_data (container)->rootfs, O_PATH | O_CLOEXEC);
  ಯದಿ (UNLIKELY (*fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "reopen rootfs after mount on /");

  tmp = dup (*fd);
  ಯದಿ (UNLIKELY (tmp < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "dup");

  TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
  get_private_data (container)->rootfsfd = tmp;

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Apply the propagation flags (MS_SHARED, MS_SLAVE, ...) to a mount,
   preferring mount_setattr when a target fd is available and falling
   back to mount(2).  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
apply_propagation_flags (ಧ್ರುವ ವರ್ಣ *target, ಪೂರ್ಣಾಙ್ಕ targetfd, ಧ್ರುವ ವರ್ಣ *real_target,
                         unsigned long mountflags, libcrun_error_t *err)
{
  bool propagation_done = false;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (targetfd >= 0)
    {
      libcrun_error_t tmp_err = ರಿಕ್ತ;
      ret = do_mount_setattr ((mountflags & MS_REC) != 0, target, targetfd, 0, mountflags & ALL_PROPAGATIONS, &tmp_err);
      ಯದಿ (LIKELY (ret == 0))
        propagation_done = true;
      ಅನ್ಯಥಾ
        crun_error_release (&tmp_err);
    }

  ಯದಿ (! propagation_done)
    {
      ret = mount (ರಿಕ್ತ, real_target, ರಿಕ್ತ, mountflags & ALL_PROPAGATIONS, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "set propagation for `%s`", target);
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Perform (or defer) the remount needed to apply bind/read-only/single
   instance flags.  A read-write remount and MOUNT_NO_DEFERRED_REMOUNT are
   done immediately; otherwise the remount is queued to run later.  *fd is
   consumed when the remount is deferred.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
schedule_or_do_remount (libcrun_container_t *container, ಪೂರ್ಣಾಙ್ಕ *fd, ಪೂರ್ಣಾಙ್ಕ targetfd,
                        ಧ್ರುವ ವರ್ಣ *target, ಧ್ರುವ ವರ್ಣ *real_target,
                        bool single_instance, unsigned long mountflags,
                        ಧ್ರುವ ಶೂನ್ಯ *data, ಪೂರ್ಣಾಙ್ಕ extra_flags, libcrun_error_t *err)
{
  unsigned long remount_flags = MS_REMOUNT | (single_instance ? 0 : MS_BIND) | (mountflags & ~ALL_PROPAGATIONS);
  ಸಂರಚನಾ remount_s *r;

  ಯದಿ ((remount_flags & MS_RDONLY) == 0 || (extra_flags & MOUNT_NO_DEFERRED_REMOUNT))
    ಪ್ರತ್ಯಾಗಮ do_remount (*fd >= 0 ? *fd : targetfd, real_target, remount_flags, data, err);

  ಯದಿ (*fd < 0)
    {
      *fd = dup (targetfd);
      ಯದಿ (UNLIKELY (*fd < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "dup `%d`", targetfd);
    }

  /* The remount owns the fd.  */
  r = make_remount (get_and_reset (fd), target, remount_flags, data, get_private_data (container)->remounts);
  get_private_data (container)->remounts = r;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_mount (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *source, ಪೂರ್ಣಾಙ್ಕ targetfd,
          ಧ್ರುವ ವರ್ಣ *target, ಧ್ರುವ ವರ್ಣ *fstype, unsigned long mountflags, ಧ್ರುವ ಶೂನ್ಯ *data,
          ಪೂರ್ಣಾಙ್ಕ extra_flags, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *data_with_label = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *data_without_label = data;
  proc_fd_path_t target_buffer;
  ಧ್ರುವ ವರ್ಣ *context_type = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ ms_move_fd = -1;
  ಧ್ರುವ ವರ್ಣ *real_target = target;
  bool single_instance = false;
  bool needs_remount = false;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
  ಧ್ರುವ ವರ್ಣ *label = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret = 0;

  ಯದಿ (container->container_def->linux && container->container_def->linux->mount_label)
    label = container->container_def->linux->mount_label;
  ಅನ್ಯಥಾ
    extra_flags = (extra_flags & ~LABEL_MASK) | LABEL_NONE;

  ಯದಿ (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
      needs_remount = true;
    }

  ಯದಿ ((extra_flags & LABEL_MASK) == LABEL_MOUNT)
    {
      context_type = get_selinux_context_type (container, err);
      ಯದಿ (UNLIKELY (context_type == ರಿಕ್ತ))
        ಪ್ರತ್ಯಾಗಮ -1;

      ret = add_selinux_mount_label (&data_with_label, data, label, context_type, err);
      ಯದಿ (ret < 0)
        ಪ್ರತ್ಯಾಗಮ ret;
      data = data_with_label;
    }

  ಯದಿ (mountflags & MS_MOVE)
    {
      ಯದಿ ((mountflags & MS_BIND) || fstype)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: cannot use MS_MOVE with MS_BIND or fstype");

      ret = mount (source, real_target, ರಿಕ್ತ, MS_MOVE, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "move mount `%s` to `%s`", source, target);
      mountflags &= ~MS_MOVE;

      /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
      ms_move_fd = open_mount_target (container, target, err);
      ಯದಿ (UNLIKELY (ms_move_fd < 0))
        ಪ್ರತ್ಯಾಗಮ ms_move_fd;
      targetfd = ms_move_fd;
    }

  ಯದಿ ((fstype && fstype[0]) || (mountflags & MS_BIND))
    {
      unsigned long flags = mountflags & ~(ALL_PROPAGATIONS_NO_REC | MS_RDONLY);

      ret = -1;

      ಯದಿ (targetfd >= 0)
        {
          ಯದಿ (mountflags & MS_BIND)
            {
              cleanup_close ಪೂರ್ಣಾಙ್ಕ bindfd = -1;
              libcrun_error_t tmp_err = ರಿಕ್ತ;

              bindfd = get_bind_mount (AT_FDCWD, source,
                                       (mountflags & MS_REC) != 0,
                                       false, false, 0, &tmp_err);
              ಯದಿ (bindfd < 0)
                {
                  ಪೂರ್ಣಾಙ್ಕ old_ಬೇರು_fd = get_old_ಬೇರು_fd (get_private_data (container));

                  ಯದಿ (old_ಬೇರು_fd >= 0 && source && source[0] == '/')
                    {
                      crun_error_release (&tmp_err);
                      bindfd = get_bind_mount (old_ಬೇರು_fd, (source[1] == '\0') ? "." : source + 1,
                                               (mountflags & MS_REC) != 0,
                                               false, false, 0, &tmp_err);
                    }
                }
              ಯದಿ (bindfd >= 0)
                ret = fs_move_mount_to (bindfd, targetfd, ರಿಕ್ತ);
              ಯದಿ (ret < 0)
                crun_error_release (&tmp_err);
            }
          ಅನ್ಯಥಾ
            {
              cleanup_close ಪೂರ್ಣಾಙ್ಕ newfs = fsopen_mount (fstype, source, context_type, label, data_without_label);
              ಯದಿ (newfs >= 0)
                ret = fs_move_mount_to (newfs, targetfd, ರಿಕ್ತ);
            }
        }

      ಯದಿ (ret < 0)
        {
          ret = mount (source, real_target, fstype, flags, data);
          ಯದಿ (ret < 0 && (mountflags & MS_BIND) && source && source[0] == '/')
            {
              ಪೂರ್ಣಾಙ್ಕ old_ಬೇರು_fd = get_old_ಬೇರು_fd (get_private_data (container));

              ಯದಿ (old_ಬೇರು_fd >= 0)
                {
                  cleanup_free ವರ್ಣ *proc_source_buf = ರಿಕ್ತ;
                  proc_fd_path_t old_ಬೇರು_path;

                  get_proc_self_fd_path (old_ಬೇರು_path, old_ಬೇರು_fd);
                  xasprintf (&proc_source_buf, "%s/%s", old_ಬೇರು_path, source + 1);
                  ret = mount (proc_source_buf, real_target, fstype, flags, data);
                }
            }
        }
      ಯದಿ (UNLIKELY (ret < 0))
        {
          ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;

          ret = sysfs_userns_fallback (container, target, real_target, targetfd,
                                       mountflags, fstype, err);
          ಯದಿ (ret == 0)
            ಪ್ರತ್ಯಾಗಮ 0;
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;

          /* ret > 0: the fallback does not apply, report the original failure.  */
          ret = crun_make_error (err, saved_errno, "mount `%s` to `%s`", source, target);

          ಪ್ರತ್ಯಾಗಮ diagnose_mount_failure (ret, err, container, fstype);
        }

      ಯದಿ (targetfd >= 0)
        {
          /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
          fd = open_mount_target (container, target, err);
          ಯದಿ (UNLIKELY (fd < 0))
            ಪ್ರತ್ಯಾಗಮ fd;

          /* We are replacing the rootfs, reopen it.  */
          ಯದಿ (is_empty_string (target))
            {
              ret = do_mount_replace_rootfs (container, &fd, err);
              ಯದಿ (UNLIKELY (ret < 0))
                ಪ್ರತ್ಯಾಗಮ ret;
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

  ಯದಿ (mountflags & ALL_PROPAGATIONS_NO_REC)
    {
      ret = apply_propagation_flags (target, targetfd, real_target, mountflags, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (mountflags & (MS_BIND | MS_RDONLY))
    needs_remount = true;

  ಯದಿ (data && fstype && strcmp (fstype, "proc") == 0)
    {
      single_instance = true;
      needs_remount = true;
    }

  ಯದಿ (needs_remount && ! (extra_flags & MOUNT_NO_REMOUNT))
    {
      ret = schedule_or_do_remount (container, &fd, targetfd, target, real_target,
                                    single_instance, mountflags, data, extra_flags, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ
try_umount (ಪೂರ್ಣಾಙ್ಕ targetfd, ಧ್ರುವ ವರ್ಣ *target)
{
  proc_fd_path_t target_buffer;
  ಧ್ರುವ ವರ್ಣ *real_target = target;

  ಯದಿ (targetfd >= 0)
    {
      get_proc_self_fd_path (target_buffer, targetfd);
      real_target = target_buffer;
    }

  umount2 (real_target, MNT_DETACH);
}

ಸ್ಥಿರ bool
container_has_cgroupns (libcrun_container_t *container)
{
  ಪ್ರತ್ಯಾಗಮ get_private_data (container)->unshare_flags & CLONE_NEWCGROUP;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_mount_cgroup_v2 (libcrun_container_t *container, ಪೂರ್ಣಾಙ್ಕ targetfd, ಧ್ರುವ ವರ್ಣ *target,
                    unsigned long mountflags, ಧ್ರುವ ವರ್ಣ *data, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಪೂರ್ಣಾಙ್ಕ cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (UNLIKELY (cgroup_mode < 0))
    ಪ್ರತ್ಯಾಗಮ cgroup_mode;

  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ಪೂರ್ಣಾಙ್ಕ saved_errno = crun_error_get_errno (err);
      ಯದಿ (saved_errno == EPERM || saved_errno == EBUSY)
        {
          ಧ್ರುವ ವರ್ಣ *unified_cgroup_path;
          ಧ್ರುವ ವರ್ಣ *src_cgroup;

          crun_error_release (err);

          ಯದಿ (saved_errno == EBUSY)
            {
              /* If we got EBUSY it means the cgroup file system is already mounted at the targetfd and we
                 cannot stack another one on top of it.  Attempt mounting a tmpfs below the cgroup mount.  */

              ret = do_mount (container, "tmpfs", targetfd, target, "tmpfs", MS_PRIVATE, "nr_blocks=1,nr_inodes=1", LABEL_NONE, err);
              ಯದಿ (LIKELY (ret == 0))
                {
                  ret = do_mount (container, "cgroup2", targetfd, target, "cgroup2", mountflags, data, LABEL_NONE, err);
                  ಯದಿ (LIKELY (ret == 0))
                    ಪ್ರತ್ಯಾಗಮ ret;

                  /* Best-effort cleanup for the tmpfs, if it fails there is nothing to worry about.  */
                  try_umount (targetfd, target);
                }

              /* If the previous method failed, fall back to bind mounting the current cgroup.  */
              crun_error_release (err);
            }

          unified_cgroup_path = get_private_data (container)->unified_cgroup_path;

          /* If everything else failed, bind mount from the current cgroup.  */
          src_cgroup = unified_cgroup_path && container_has_cgroupns (container) ? unified_cgroup_path : CGROUP_ಬೇರು;
          ಪ್ರತ್ಯಾಗಮ do_mount (container, src_cgroup, targetfd, target, ರಿಕ್ತ,
                           MS_BIND | mountflags, ರಿಕ್ತ, LABEL_NONE, err);
        }

      ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_mount_cgroup_systemd_v1 (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *source, ಪೂರ್ಣಾಙ್ಕ targetfd, ಧ್ರುವ ವರ್ಣ *target,
                            unsigned long mountflags, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
  ಧ್ರುವ ವರ್ಣ *subsystem = "systemd";
  cleanup_free ವರ್ಣ *subsystem_path = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ tmpfsdirfd = -1;

  mountflags = mountflags & ~MS_BIND;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags, "size=1024k", LABEL_NONE, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* Get a reference to the newly created cgroup directory.  */
  tmpfsdirfd = open_mount_target (container, target, err);
  ಯದಿ (UNLIKELY (tmpfsdirfd < 0))
    ಪ್ರತ್ಯಾಗಮ tmpfsdirfd;
  targetfd = tmpfsdirfd;

  ret = mkdirat (targetfd, subsystem, 0755);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "mkdirat `%s`", subsystem);

  ret = append_paths (&subsystem_path, err, target, subsystem, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  fd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "openat `%s`", subsystem_path);

  ಪ್ರತ್ಯಾಗಮ do_mount (container, "cgroup", fd, subsystem_path, "cgroup", mountflags, "none,name=systemd,xattr", LABEL_NONE,
                   err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_mount_cgroup_v1 (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *source, ಪೂರ್ಣಾಙ್ಕ targetfd, ಧ್ರುವ ವರ್ಣ *target,
                    unsigned long mountflags, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  cleanup_free ವರ್ಣ *content = ರಿಕ್ತ;
  ವರ್ಣ *from;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ tmpfsdirfd = -1;
  ವರ್ಣ *saveptr = ರಿಕ್ತ;

  ret = do_mount (container, source, targetfd, target, "tmpfs", mountflags & ~MS_RDONLY, "size=1024k", LABEL_MOUNT,
                  err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  tmpfsdirfd = open_mount_target (container, target, err);
  ಯದಿ (UNLIKELY (tmpfsdirfd < 0))
    ಪ್ರತ್ಯಾಗಮ tmpfsdirfd;
  targetfd = tmpfsdirfd;

  {
    ಪೂರ್ಣಾಙ್ಕ procfd = get_procfd (get_private_data (container), err);
    ಯದಿ (UNLIKELY (procfd < 0))
      ಪ್ರತ್ಯಾಗಮ procfd;

    ret = read_all_file_at (procfd, SELF_CGROUP, &content, ರಿಕ್ತ, err);
  }
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (UNLIKELY (content == ರಿಕ್ತ || content[0] == '\0'))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  ಪ್ರತಿ (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ರಿಕ್ತ, "\n", &saveptr))
    {
      cleanup_free ವರ್ಣ *source_path = ರಿಕ್ತ;
      cleanup_free ವರ್ಣ *source_subsystem = ರಿಕ್ತ;
      cleanup_free ವರ್ಣ *subsystem_path = ರಿಕ್ತ;
      ವರ್ಣ *subpath, *subsystem, *subsystem_fqn, *it;
      cleanup_close ಪೂರ್ಣಾಙ್ಕ subsystemfd = -1;
      subsystem = strchr (from, ':') + 1;
      subpath = strchr (subsystem, ':') + 1;
      *(subpath - 1) = '\0';

      ಯದಿ (subsystem[0] == '\0')
        ಅನುವರ್ತ;

      /* subsystem_fqn includes name= for named hierarchies.  */
      subsystem_fqn = subsystem;

      it = strstr (subsystem, "name=");
      ಯದಿ (it)
        subsystem = it + 5;

      ಯದಿ (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      ಯದಿ (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      ret = append_paths (&source_subsystem, err, CGROUP_ಬೇರು, subsystem, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      /* if there is already a mount specified, do not add a default one.  */
      ಯದಿ (find_mount_for (container, source_subsystem) != ರಿಕ್ತ)
        ಅನುವರ್ತ;

      ret = append_paths (&source_path, err, source_subsystem, subpath, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = append_paths (&subsystem_path, err, target, subsystem, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = mkdirat (targetfd, subsystem, 0755);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "mkdirat `%s`", subsystem_path);

      subsystemfd = openat (targetfd, subsystem, O_CLOEXEC | O_PATH | O_DIRECTORY | O_NOFOLLOW);
      ಯದಿ (UNLIKELY (subsystemfd < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", subsystem_path);

      ಯದಿ (container_has_cgroupns (container))
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, "cgroup", mountflags, subsystem_fqn,
                          LABEL_NONE, err);
          ಯದಿ (UNLIKELY (ret < 0))
            {
              ಯದಿ (crun_error_get_errno (err) == ENOENT || crun_error_get_errno (err) == ENODEV)
                {
                  /* We are trying to mount a subsystem that is not present.  */
                  crun_error_release (err);
                  ಅನುವರ್ತ;
                }
              ಪ್ರತ್ಯಾಗಮ ret;
            }
        }
      ಅನ್ಯಥಾ
        {
          ret = do_mount (container, source_path, subsystemfd, subsystem_path, ರಿಕ್ತ, MS_BIND | mountflags, ರಿಕ್ತ,
                          LABEL_NONE, err);
          ಯದಿ (UNLIKELY (ret < 0))
            {
              ಯದಿ (crun_error_get_errno (err) != ENOENT)
                ಪ್ರತ್ಯಾಗಮ ret;

              crun_error_release (err);

              /* We might already be in a container.  Mount the source subsystem.  */
              ret = do_mount (container, source_subsystem, subsystemfd, subsystem_path, ರಿಕ್ತ, MS_BIND | mountflags,
                              ರಿಕ್ತ, LABEL_NONE, err);
              ಯದಿ (UNLIKELY (ret < 0))
                {
                  /* If it still fails with ENOENT, ignore the error as the controller might have been
                     dropped and doesn't exist.  */
                  ಯದಿ (crun_error_get_errno (err) != ENOENT)
                    ಪ್ರತ್ಯಾಗಮ ret;

                  crun_error_release (err);
                }
            }
        }
    }

  ret = libcrun_cgroups_create_symlinks (targetfd, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_mount_cgroup (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *source, ಪೂರ್ಣಾಙ್ಕ targetfd, ಧ್ರುವ ವರ್ಣ *target,
                 unsigned long mountflags, ಧ್ರುವ ವರ್ಣ *data, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ cgroup_mode;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ಯದಿ (UNLIKELY (cgroup_mode < 0))
    ಪ್ರತ್ಯಾಗಮ cgroup_mode;

  ವಿಕಲ್ಪ (cgroup_mode)
    {
    ಪಕ್ಷ CGROUP_MODE_UNIFIED:
      ಪ್ರತ್ಯಾಗಮ do_mount_cgroup_v2 (container, targetfd, target, mountflags, data, err);
    ಪಕ್ಷ CGROUP_MODE_LEGACY:
    ಪಕ್ಷ CGROUP_MODE_HYBRID:
      ಪ್ರತ್ಯಾಗಮ do_mount_cgroup_v1 (container, source, targetfd, target, mountflags, err);
    }

  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "unknown cgroup mode `%d`", cgroup_mode);
}

ಸಂರಚನಾ device_s needed_devs[] = { { "/dev/null", "c", 1, 3, 0666, 0, 0 },
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
ಸ್ಥಿರ ಶೂನ್ಯ
preopen_needed_devs_and_notify (libcrun_container_t *container)
{
  ಸಂರಚನಾ private_data_s *private_data = get_private_data (container);
  size_t i;

  ಯದಿ (private_data->needed_devs_fds)
    {
      ಸಂರಚನಾ libcrun_fd_map *dev_fds = private_data->needed_devs_fds;

      ಪ್ರತಿ (i = 0; needed_devs[i].path; i++)
        {
          ಯದಿ (i < dev_fds->nfds && dev_fds->fds[i] >= 0)
            ಅನುವರ್ತ;

          ಪೂರ್ಣಾಙ್ಕ fd = syscall_open_ವೃಕ್ಷ (AT_FDCWD, needed_devs[i].path,
                                      OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
          ಯದಿ (fd >= 0 && i < dev_fds->nfds)
            dev_fds->fds[i] = fd;
          ಅನ್ಯಥಾ ಯದಿ (fd >= 0)
            close (fd);
        }
    }

  ಯದಿ (private_data->notify_socket_ವೃಕ್ಷ_fd < 0
      && private_data->host_notify_socket_path)
    {
      ಪೂರ್ಣಾಙ್ಕ fd = syscall_open_ವೃಕ್ಷ (AT_FDCWD, private_data->host_notify_socket_path,
                                  OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
      ಯದಿ (fd >= 0)
        private_data->notify_socket_ವೃಕ್ಷ_fd = fd;
    }
}

/* Check if the specified path is a direct child of /dev.  If it is
 return a pointer to the basename.  */
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
relative_path_under_dev (ಧ್ರುವ ವರ್ಣ *path)
{
  ಯದಿ (path[0] != '/')
    ಪ್ರತ್ಯಾಗಮ path;
  ಯದಿ (strncmp (path, "/dev/", 5) == 0)
    {
      ಯದಿ (strchr (path + 5, '/'))
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
      ಪ್ರತ್ಯಾಗಮ path + 5;
    }
  ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
mknod_and_set_attrs (libcrun_container_t *container, ಪೂರ್ಣಾಙ್ಕ dirfd, ಧ್ರುವ ವರ್ಣ *name, ಸಂರಚನಾ device_s *device,
                     mode_t type, dev_t dev, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
  proc_fd_path_t fd_path;
  ಧ್ರುವ ವರ್ಣ *what = "new";
  bool exists = false;
  ಸಂರಚನಾ stat st;
  ಪೂರ್ಣಾಙ್ಕ procfd;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = mknodat (dirfd, name, device->mode | type, dev);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ಯದಿ (errno != EEXIST)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "mknodat `%s`", device->path);
      /* The runtime spec requires an error if the existing file
         does not match the requested device, so check it below.  */
      exists = true;
      what = "existing";
    }

  /* Get a handle and verify that it matches the expected inode type
     and major:minor before operating on it.  */
  fd = openat (dirfd, name, O_PATH | O_NOFOLLOW | O_CLOEXEC);
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open %s device `%s`", what, device->path);

  ret = fstat (fd, &st);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fstat %s device `%s`", what, device->path);

  ಯದಿ (UNLIKELY ((st.st_mode & S_IFMT) != type))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "%s device `%s` has incorrect file type %#o, expected %#o",
                            what, device->path, st.st_mode & S_IFMT, type);

  ಯದಿ (UNLIKELY (type != S_IFIFO && st.st_rdev != dev))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "%s device `%s` has incorrect major:minor %u:%u, expected %u:%u",
                            what, device->path, major (st.st_rdev), minor (st.st_rdev),
                            major (dev), minor (dev));

  /* Leave the existing inode's mode and owner as is.  */
  ಯದಿ (exists)
    ಪ್ರತ್ಯಾಗಮ 0;

  /* fchmod(2) does not work on O_PATH fds, so go through /proc.  */
  procfd = get_procfd (get_private_data (container), err);
  ಯದಿ (UNLIKELY (procfd < 0))
    ಪ್ರತ್ಯಾಗಮ procfd;

  get_self_fd_path (fd_path, fd);
  ret = fchmodat (procfd, fd_path, device->mode, 0);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fchmod `%s`", device->path);

  ret = fchownat (fd, "", device->uid, device->gid, AT_EMPTY_PATH);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fchown `%s`", device->path);

  ಪ್ರತ್ಯಾಗಮ 1;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_create_dev (libcrun_container_t *container, ಪೂರ್ಣಾಙ್ಕ devfd, ಪೂರ್ಣಾಙ್ಕ srcfd,
                    ಸಂರಚನಾ device_s *device, bool binds, bool ensure_parent_dir,
                    libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  dev_t dev;
  mode_t type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  ಧ್ರುವ ವರ್ಣ *fullname = device->path;
  ಧ್ರುವ ವರ್ಣ *rootfs = get_private_data (container)->rootfs;
  ಯದಿ (is_empty_string (fullname))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EINVAL, "device path is empty");
  // Normalize the path by removing trailing slashes.
  cleanup_free ವರ್ಣ *normalized_path = xstrdup (fullname);
  consume_trailing_slashes (normalized_path);
  ಯದಿ (normalized_path[0] == '\0')
    strcpy (normalized_path, "/");
  ಧ್ರುವ ವರ್ಣ *rel_dev = relative_path_under_dev (normalized_path);

  ಯದಿ (binds)
    {
      cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;

      ಯದಿ (rel_dev)
        {
          fd = openat (devfd, rel_dev, O_NOFOLLOW | O_CLOEXEC | O_PATH | O_NONBLOCK);
          ಯದಿ (UNLIKELY (fd < 0))
            {
              ಯದಿ (errno == ENOENT)
                fd = openat (devfd, rel_dev, O_CREAT | O_NOFOLLOW | O_CLOEXEC | O_NONBLOCK, 0700);

              ಯದಿ (UNLIKELY (fd < 0))
                ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "openat `%s`", device->path);
            }
        }
      ಅನ್ಯಥಾ
        {
          ಧ್ರುವ ವರ್ಣ *rel_path = consume_slashes (normalized_path);

          fd = crun_safe_create_and_open_ref_at (false, get_private_data (container)->rootfsfd, rootfs, rel_path, 0755, err);
          ಯದಿ (UNLIKELY (fd < 0))
            ಪ್ರತ್ಯಾಗಮ fd;
        }

      ಯದಿ (srcfd >= 0)
        {
          ret = do_mount_setattr (false, normalized_path, srcfd, 0, MS_NOSUID | MS_NOEXEC, err);
          ಯದಿ (LIKELY (ret == 0))
            {
              ret = syscall_move_mount (srcfd, "", fd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
              ಯದಿ (LIKELY (ret >= 0))
                ಪ್ರತ್ಯಾಗಮ 0;
            }
          ಅನ್ಯಥಾ
            crun_error_release (err);
        }

      {
        cleanup_close ಪೂರ್ಣಾಙ್ಕ mountfd = -1;

        /* Try open_tree+mount_setattr to apply flags atomically.  */
        mountfd = get_bind_mount (-1, fullname, false, false, false, MS_PRIVATE, err);
        ಯದಿ (mountfd >= 0)
          {
            ret = do_mount_setattr (false, normalized_path, mountfd, 0, MS_NOSUID | MS_NOEXEC, err);
            ಯದಿ (LIKELY (ret == 0))
              {
                ret = fs_move_mount_to (mountfd, fd, ರಿಕ್ತ);
                ಯದಿ (LIKELY (ret == 0))
                  ಪ್ರತ್ಯಾಗಮ 0;
              }
            ಅನ್ಯಥಾ
              crun_error_release (err);
          }
        ಅನ್ಯಥಾ
          crun_error_release (err);
      }

      ret = do_mount (container, fullname, fd, normalized_path, ರಿಕ್ತ, MS_BIND | MS_PRIVATE | MS_NOEXEC | MS_NOSUID, ರಿಕ್ತ, LABEL_MOUNT, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಅನ್ಯಥಾ
    {
      dev = makedev (device->major, device->minor);

      /* Check whether the path is directly under /dev.  Since we already have an open fd to /dev and mknodat(2)
         fails when the destination already exists or is a symlink, it is safe to use it directly.
         If it is not a direct child, then first get a fd to the dirfd.
      */
      ಯದಿ (rel_dev)
        {
          ret = mknod_and_set_attrs (container, devfd, rel_dev, device, type, dev, err);
          ಯದಿ (ret <= 0)
            ಪ್ರತ್ಯಾಗಮ ret;
        }
      ಅನ್ಯಥಾ
        {
          cleanup_close ಪೂರ್ಣಾಙ್ಕ dirfd = -1;
          cleanup_free ವರ್ಣ *dirname = ರಿಕ್ತ;
          ವರ್ಣ *basename, *found;

          dirname = xstrdup (normalized_path);

          found = strrchr (dirname, '/');
          ಯದಿ (found)
            *found = '\0';

          basename = found ? found + 1 : dirname;

          ಯದಿ (dirname[0] == '\0')
            {
              dirfd = dup (get_private_data (container)->rootfsfd);
              ಯದಿ (UNLIKELY (dirfd < 0))
                ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "dup fd for `%s`", rootfs);
            }
          ಅನ್ಯಥಾ
            {
              dirfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, dirname, O_DIRECTORY | O_PATH | O_CLOEXEC, 0, err);
              ಯದಿ (dirfd < 0 && ensure_parent_dir)
                {
                  crun_error_release (err);

                  dirfd = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs, dirname, 0755, err);
                }
              ಯದಿ (UNLIKELY (dirfd < 0))
                ಪ್ರತ್ಯಾಗಮ dirfd;
            }

          ret = mknod_and_set_attrs (container, dirfd, basename, device, type, dev, err);
          ಯದಿ (ret <= 0)
            ಪ್ರತ್ಯಾಗಮ ret;
        }
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ symlink_s
{
  ಧ್ರುವ ವರ್ಣ *path;
  ಧ್ರುವ ವರ್ಣ *target;
  bool force;
};

ಸ್ಥಿರ ಸಂರಚನಾ symlink_s symlinks[] = { { "/proc/self/fd", "fd", false },
                                       { "/proc/self/fd/0", "stdin", false },
                                       { "/proc/self/fd/1", "stdout", false },
                                       { "/proc/self/fd/2", "stderr", false },
                                       { "/proc/kcore", "core", false },
                                       { "pts/ptmx", "ptmx", true },
                                       { ರಿಕ್ತ, ರಿಕ್ತ, false } };

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
create_missing_devs (libcrun_container_t *container, bool binds, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  size_t i;
  ಸಂರಚನಾ device_s *it;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ devfd = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  ಧ್ರುವ ವರ್ಣ *rootfs = get_private_data (container)->rootfs;
  cleanup_close_map ಸಂರಚನಾ libcrun_fd_map *dev_fds = ರಿಕ್ತ;
  cleanup_close_map ಸಂರಚನಾ libcrun_fd_map *needed_fds = ರಿಕ್ತ;

  dev_fds = get_private_data (container)->dev_fds;
  get_private_data (container)->dev_fds = ರಿಕ್ತ;

  needed_fds = get_private_data (container)->needed_devs_fds;
  get_private_data (container)->needed_devs_fds = ರಿಕ್ತ;

  ಯದಿ (! def || ! def->linux)
    ಪ್ರತ್ಯಾಗಮ 0;

  devfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, "dev", O_CLOEXEC | O_PATH | O_DIRECTORY, 0, err);
  ಯದಿ (UNLIKELY (devfd < 0))
    ಪ್ರತ್ಯಾಗಮ devfd;

  ಪ್ರತಿ (i = 0; i < def->linux->devices_len; i++)
    {
      ಸಂರಚನಾ device_s device = {
        def->linux->devices[i]->path,
        def->linux->devices[i]->type,
        def->linux->devices[i]->major,
        def->linux->devices[i]->minor,
        def->linux->devices[i]->file_mode,
        def->linux->devices[i]->uid,
        def->linux->devices[i]->gid,
      };

      ಯದಿ (! def->linux->devices[i]->file_mode_present)
        device.mode = 0666;
      ret = libcrun_create_dev (container, devfd, dev_fds->fds[i], &device, binds, true, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತಿ (it = needed_devs, i = 0; it->path; it++, i++)
    {
      cleanup_close ಪೂರ್ಣಾಙ್ಕ srcfd = (needed_fds && i < needed_fds->nfds) ? needed_fds->fds[i] : -1;

      /* make sure the parent directory exists only on the first iteration.  */
      ret = libcrun_create_dev (container, devfd, srcfd, it, binds, it == needed_devs, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
      ಯದಿ (srcfd >= 0)
        needed_fds->fds[i] = -1;
    }

  ಪ್ರತಿ (i = 0; symlinks[i].target; i++)
    {
    retry_symlink:
      ret = symlinkat (symlinks[i].path, devfd, symlinks[i].target);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;

          ಯದಿ (errno == EEXIST && ! symlinks[i].force)
            ಅನುವರ್ತ;

          /* If the symlink should be forced, make sure to unlink any existing file at the same path.  */
          ಯದಿ (errno == EEXIST)
            {
            retry_unlink:
              ret = unlinkat (devfd, symlinks[i].target, 0);
              ಯದಿ (ret < 0 && errno == EISDIR)
                ret = unlinkat (devfd, symlinks[i].target, AT_REMOVEDIR);
              ಯದಿ (ret < 0 && errno == EBUSY)
                {
                  cleanup_close ಪೂರ್ಣಾಙ್ಕ tfd = openat (devfd, symlinks[i].target, O_CLOEXEC | O_PATH | O_NOFOLLOW);
                  ಯದಿ (tfd >= 0)
                    {
                      proc_fd_path_t procpath;

                      get_proc_self_fd_path (procpath, tfd);
                      ಯದಿ (umount2 (procpath, MNT_DETACH) == 0)
                        ಗಚ್ಛ retry_unlink;
                    }
                }
              ಯದಿ (ret == 0)
                ಗಚ್ಛ retry_symlink;
            }
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, saved_errno, "symlinkat `/dev/%s`", symlinks[i].target);
        }
    }

  ಯದಿ (container->container_def->process && container->container_def->process->terminal)
    {
      ret = create_file_if_missing_at (devfd, "console", 0620, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Advance to the next path component, skipping empty and "." ones.  */
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
next_path_component (ಧ್ರುವ ವರ್ಣ *p, size_t *len)
{
  ಪ್ರತಿ (;;)
    {
      p = consume_slashes (p);
      *len = strcspn (p, "/");
      ಯದಿ (*len != 1 || p[0] != '.')
        ಪ್ರತ್ಯಾಗಮ p;
      p++;
    }
}

/* Whether A and B are the same path, ignoring repeated, leading and
   trailing slashes, and "." components.  ".." components are compared
   as they are, not resolved lexically: if a path component before them
   is a symlink, the path may refer to something else entirely, and
   treating it as a duplicate would leave it unmasked.  */
ಸ್ಥಿರ bool
same_path (ಧ್ರುವ ವರ್ಣ *a, ಧ್ರುವ ವರ್ಣ *b)
{
  size_t la, lb;

  ಪ್ರತಿ (;;)
    {
      a = next_path_component (a, &la);
      b = next_path_component (b, &lb);
      ಯದಿ (la != lb || memcmp (a, b, la) != 0)
        ಪ್ರತ್ಯಾಗಮ false;
      ಯದಿ (la == 0)
        ಪ್ರತ್ಯಾಗಮ true;
      a += la;
      b += lb;
    }
}

/* Whether paths[i] is the same as one of the paths before it.  */
ಸ್ಥಿರ bool
is_duplicate_path (ವರ್ಣ **paths, size_t i)
{
  size_t j;

  ಯದಿ (paths[i] == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ false;

  ಪ್ರತಿ (j = 0; j < i; j++)
    ಯದಿ (paths[j] && same_path (paths[j], paths[i]))
      ಪ್ರತ್ಯಾಗಮ true;

  ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_masked_and_readonly_paths (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  ಯದಿ (! def || ! def->linux)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಪ್ರತಿ (i = 0; i < def->linux->masked_paths_len; i++)
    {
      ಯದಿ (is_duplicate_path (def->linux->masked_paths, i))
        ಅನುವರ್ತ;

      ret = do_masked_or_readonly_path (container, def->linux->masked_paths[i], false, false, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಪ್ರತಿ (i = 0; i < def->linux->readonly_paths_len; i++)
    {
      ret = do_masked_or_readonly_path (container, def->linux->readonly_paths[i], true, true, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_pivot (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *rootfs, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ oldrootfd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ newrootfd = -1;
  ಪೂರ್ಣಾಙ್ಕ ret;

  oldrootfd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  ಯದಿ (UNLIKELY (oldrootfd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `/`");

  newrootfd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ಯದಿ (UNLIKELY (newrootfd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", rootfs);

  ret = fchdir (newrootfd);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = pivot_ಬೇರು (".", ".");
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "pivot_root");

  ret = fchdir (oldrootfd);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = do_mount (container, ರಿಕ್ತ, -1, ".", ರಿಕ್ತ, MS_REC | MS_PRIVATE, ರಿಕ್ತ, LABEL_MOUNT, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = umount2 (".", MNT_DETACH);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "umount2 oldroot");

  ಕುರು
    {
      ret = umount2 (".", MNT_DETACH);
      ಯದಿ (ret < 0 && errno == EINVAL)
        ಭಙ್ಗ;
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "umount2 oldroot");
  } ಯಾವತ್ (ret == 0);

  ret = chdir ("/");
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "chdir to newroot");

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
append_tmpfs_mode_if_missing (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, ವರ್ಣ **data, libcrun_error_t *err)
{
  ಧ್ರುವ ವರ್ಣ *rootfs = get_private_data (container)->rootfs;
  bool empty_data = is_empty_string (*data);
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
  ಸಂರಚನಾ stat st;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (*data != ರಿಕ್ತ && strstr (*data, "mode="))
    ಪ್ರತ್ಯಾಗಮ 0;

  fd = safe_openat (get_private_data (container)->rootfsfd, rootfs, mount->destination, O_CLOEXEC | O_PATH, 0, err);
  ಯದಿ (fd < 0)
    {
      ಯದಿ (crun_error_get_errno (err) != ENOENT)
        ಪ್ರತ್ಯಾಗಮ fd;

      crun_error_release (err);
      ಪ್ರತ್ಯಾಗಮ 0;
    }
  ret = fstat (fd, &st);
  ಯದಿ (ret < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fstat `%s`", mount->destination);

  xasprintf (data, "%s%smode=%o", empty_data ? "" : *data, empty_data ? "" : ",", st.st_mode & 07777);
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_default_flags (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *destination, ವರ್ಣ **data)
{
  ಯದಿ (strcmp (destination, "/proc") == 0)
    ಪ್ರತ್ಯಾಗಮ 0;
  ಯದಿ (strcmp (destination, "/dev/cgroup") == 0 || strcmp (destination, "/sys/fs/cgroup") == 0)
    {
      *data = xstrdup ("none,name=");
      ಪ್ರತ್ಯಾಗಮ MS_NOEXEC | MS_NOSUID | MS_STRICTATIME;
    }
  ಯದಿ (strcmp (destination, "/dev") == 0)
    {
      *data = xstrdup ("mode=755");
      ಪ್ರತ್ಯಾಗಮ MS_NOEXEC | MS_STRICTATIME;
    }
  ಯದಿ (strcmp (destination, "/dev/shm") == 0)
    {
      *data = xstrdup ("mode=1777,size=65536k");
      ಪ್ರತ್ಯಾಗಮ MS_NOEXEC | MS_NOSUID | MS_NODEV;
    }
  ಯದಿ (strcmp (destination, "/dev/mqueue") == 0)
    ಪ್ರತ್ಯಾಗಮ MS_NOEXEC | MS_NOSUID | MS_NODEV;
  ಯದಿ (strcmp (destination, "/dev/pts") == 0)
    {
      ಯದಿ (container->host_uid == 0)
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620,gid=5");
      ಅನ್ಯಥಾ
        *data = xstrdup ("newinstance,ptmxmode=0666,mode=620");
      ಪ್ರತ್ಯಾಗಮ MS_NOEXEC | MS_NOSUID;
    }
  ಯದಿ (strcmp (destination, "/sys") == 0)
    ಪ್ರತ್ಯಾಗಮ MS_NOEXEC | MS_NOSUID | MS_NODEV;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
get_force_cgroup_v1_annotation (libcrun_container_t *container)
{
  ಪ್ರತ್ಯಾಗಮ find_annotation (container, "run.oci.systemd.force_cgroup_v1");
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
check_valid_no_follow_file_system (ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ವರ್ಣ *destination, libcrun_error_t *err)
{
  ಸಂರಚನಾ statfs sfs;

  ಪೂರ್ಣಾಙ್ಕ ret = fstatfs (fd, &sfs);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "statfs `%s`", destination);

  ವಿಕಲ್ಪ (sfs.f_type)
    {
    ಪಕ್ಷ PROC_SUPER_MAGIC:
    ಪಕ್ಷ SYSFS_MAGIC:
    ಪಕ್ಷ DEVPTS_SUPER_MAGIC:
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "override of the mount destination `/%s` is not allowed", destination);
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
safe_create_symlink (ಪೂರ್ಣಾಙ್ಕ rootfsfd, ಧ್ರುವ ವರ್ಣ *rootfs, ಧ್ರುವ ವರ್ಣ *target, ಧ್ರುವ ವರ್ಣ *destination, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ parent_dir_fd = -1;
  cleanup_free ವರ್ಣ *buffer = ರಿಕ್ತ;
  ವರ್ಣ *part;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (is_empty_string (destination))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "empty destination for symlink `%s`", target);

  buffer = xstrdup (destination);
  part = dirname (buffer);

  parent_dir_fd = crun_safe_create_and_open_ref_at (true, rootfsfd, rootfs, part, 0755, err);
  ಯದಿ (UNLIKELY (parent_dir_fd < 0))
    ಪ್ರತ್ಯಾಗಮ parent_dir_fd;

  /* It is safe to reuse the buffer since it was created with xstrdup (destination).  */
  strcpy (buffer, destination);
  part = basename (buffer);

  ret = symlinkat (target, parent_dir_fd, part);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      /* If it exists, check if it has the same content, if so just ignore the error.  */
      ಯದಿ (errno == EEXIST)
        {
          cleanup_free ವರ್ಣ *link = ರಿಕ್ತ;
          ssize_t len;

          len = safe_readlinkat (parent_dir_fd, part, &link, 0, err);
          ಯದಿ (UNLIKELY (len < 0))
            ಪ್ರತ್ಯಾಗಮ len;

          ಯದಿ ((((size_t) len) == strlen (target)) && strncmp (link, target, len) == 0)
            ಪ್ರತ್ಯಾಗಮ 0;

          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "symlink `%s` already exists with a different content", destination);
        }
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "symlinkat `%s`", target);
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
handle_copy_symlink (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *rootfs,
                     runtime_spec_schema_defs_mount *mount,
                     ಧ್ರುವ ವರ್ಣ *link_target, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *target = ರಿಕ್ತ;

  ಯದಿ (link_target == ರಿಕ್ತ)
    {
      ಪೂರ್ಣಾಙ್ಕ old_ಬೇರು_fd = get_old_ಬೇರು_fd (get_private_data (container));
      ಧ್ರುವ ವರ್ಣ *source = mount->source;
      ssize_t len;

      ಯದಿ (old_ಬೇರು_fd >= 0 && source[0] == '/')
        len = safe_readlinkat (old_ಬೇರು_fd, source + 1, &target, 0, err);
      ಅನ್ಯಥಾ
        len = safe_readlinkat (AT_FDCWD, source, &target, 0, err);
      ಯದಿ (UNLIKELY (len < 0))
        ಪ್ರತ್ಯಾಗಮ len;
      link_target = target;
    }

  ಪ್ರತ್ಯಾಗಮ safe_create_symlink (get_private_data (container)->rootfsfd, rootfs,
                              link_target, mount->destination, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
prepare_sysfs_or_proc_mount (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *target,
                             ಪೂರ್ಣಾಙ್ಕ *targetfd, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  /* Enforce sysfs and proc to be mounted on a regular directory.  */
  ret = openat (get_private_data (container)->rootfsfd, target,
                O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ಯದಿ (errno == ENOENT)
        {
          ಯದಿ (strchr (target, '/'))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid target `%s`: it must be mounted at the root", target);

          ret = mkdirat (get_private_data (container)->rootfsfd, target, 0755);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "mkdirat `%s`", target);

          /* Try opening it again.  */
          ret = openat (get_private_data (container)->rootfsfd, target,
                        O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
        }
      ಅನ್ಯಥಾ ಯದಿ (errno == ENOTDIR)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "the target `/%s` is invalid", target);

      ಯದಿ (ret < 0)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", target);
    }

  *targetfd = ret;
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
handle_tmpcopyup (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *rootfs, ಧ್ರುವ ವರ್ಣ *target,
                  ಪೂರ್ಣಾಙ್ಕ copy_from_fd, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ destfd, ret;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ tmpfd = copy_from_fd;

  target = consume_slashes (target);

  ಯದಿ (is_empty_string (target))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "tmpcopyup on `/` is not supported");

  destfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target,
                        O_CLOEXEC | O_DIRECTORY, 0, err);
  ಯದಿ (UNLIKELY (destfd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_error_wrap (err, "open `%s` to write for tmpcopyup", target);

  // copy_recursive_fd_to_fd closes tmpfd and destfd
  ret = copy_recursive_fd_to_fd (tmpfd, destfd, target, target, err);
  tmpfd = -1;
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_mount_label_how (ಧ್ರುವ ವರ್ಣ *type, bool is_sysfs_or_proc)
{
  ಯದಿ (is_sysfs_or_proc)
    ಪ್ರತ್ಯಾಗಮ LABEL_NONE;

  ಯದಿ (strcmp (type, "mqueue") == 0)
    ಪ್ರತ್ಯಾಗಮ LABEL_XATTR;

  ಪ್ರತ್ಯಾಗಮ LABEL_MOUNT;
}

/* The kernel does not let a mount inherited from a more privileged mount
   namespace lose its ro, nosuid, nodev and noexec flags in a user
   namespace.  The bind mounts prepared from the host are not subject to
   that, so, for a container with a user namespace, return the flags of
   SOURCE that are to be kept, i.e. those not explicitly cleared by the
   mount options.  */
ಸ್ಥಿರ unsigned long
get_bind_mount_locked_flags (ಪೂರ್ಣಾಙ್ಕ source_mountfd, ಧ್ರುವ ವರ್ಣ *source, runtime_spec_schema_defs_mount *mount)
{
  unsigned long cleared = 0;
  ಸಂರಚನಾ statfs sfs;
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (source_mountfd >= 0)
    ret = fstatfs (source_mountfd, &sfs);
  ಅನ್ಯಥಾ
    ret = statfs (source, &sfs);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ 0;

  ಪ್ರತಿ (i = 0; i < mount->options_len; i++)
    {
      ಧ್ರುವ ಸಂರಚನಾ propagation_flags_s *prop;

      ಯದಿ (mount->options[i] == ರಿಕ್ತ)
        ಅನುವರ್ತ;

      prop = libcrun_str2mount_flags (mount->options[i]);
      ಯದಿ (prop == ರಿಕ್ತ || (prop->extra_flags & OPTION_RECURSIVE))
        ಅನುವರ್ತ;

      ಯದಿ (prop->clear)
        cleared |= prop->flags;
      ಅನ್ಯಥಾ
        cleared &= ~prop->flags;
    }

  ಪ್ರತ್ಯಾಗಮ sfs.f_flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC) & ~cleared;
}

/* If PATH is lexically inside the container rootfs ROOTFS, return the part
   of it relative to ROOTFS, otherwise NULL.  Such a bind mount source refers
   to what is in the container rootfs at the time of the mount, including the
   mounts done before it, so it cannot be opened beforehand.  */
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
get_path_in_rootfs (ಧ್ರುವ ವರ್ಣ *rootfs, ಧ್ರುವ ವರ್ಣ *path)
{
  size_t len;

  ಯದಿ (rootfs == ರಿಕ್ತ || path == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

  len = strlen (rootfs);
  ಯಾವತ್ (len > 0 && rootfs[len - 1] == '/')
    len--;

  /* With "/" as the rootfs, every path is in it, and there is nothing to
     do differently.  */
  ಯದಿ (len == 0)
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

  ಯದಿ (strncmp (path, rootfs, len) != 0 || (path[len] != '/' && path[len] != '\0'))
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

  ಪ್ರತ್ಯಾಗಮ consume_slashes (path + len);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
process_single_mount (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *rootfs,
                      runtime_spec_schema_defs_mount *mount,
                      ಸಂರಚನಾ libcrun_fd_map *mount_fds, size_t mount_index,
                      ಧ್ರುವ ವರ್ಣ *systemd_cgroup_v1, libcrun_error_t *err)
{
  ಧ್ರುವ ವರ್ಣ *target = consume_slashes (mount->destination);
  cleanup_close ಪೂರ್ಣಾಙ್ಕ source_mountfd = -1;
  cleanup_free ವರ್ಣ *data = ರಿಕ್ತ;
  ವರ್ಣ *type;
  ವರ್ಣ *source;
  unsigned long flags = 0;
  unsigned long extra_flags = 0;
  mode_t src_mode = S_IFDIR;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ copy_from_fd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ targetfd = -1;
  bool mounted = false;
  bool is_sysfs_or_proc;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  ಪೂರ್ಣಾಙ್ಕ no_remount = 0;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (mount_fds)
    source_mountfd = get_and_reset (&(mount_fds->fds[mount_index]));

  type = mount->type;

  ಯದಿ (mount->options == ರಿಕ್ತ)
    flags = get_default_flags (container, mount->destination, &data);
  ಅನ್ಯಥಾ
    {
      size_t j;

      ಪ್ರತಿ (j = 0; j < mount->options_len; j++)
        flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  ಯದಿ (type == ರಿಕ್ತ && (flags & MS_BIND) == 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid mount type for `%s`", mount->destination);

  ಯದಿ ((flags & MS_BIND) && source_mountfd < 0 && ! (extra_flags & OPTION_COPY_SYMLINK))
    {
      ಧ್ರುವ ವರ್ಣ *rel_source = get_path_in_rootfs (get_private_data (container)->host_rootfs, mount->source);

      ಯದಿ (rel_source)
        {
          unsigned long propagation = (flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE)) ? 0 : MS_PRIVATE;

          source_mountfd = get_bind_mount (get_private_data (container)->rootfsfd, rel_source, (flags & MS_REC) != 0,
                                           false, (extra_flags & OPTION_SRC_NOFOLLOW) != 0, propagation, err);
          ಯದಿ (UNLIKELY (source_mountfd < 0))
            ಪ್ರತ್ಯಾಗಮ crun_error_wrap (err, "open mount source `%s`", mount->source);
        }
    }

  ಯದಿ ((flags & MS_BIND) && mount->source && has_mount_flag_options (mount)
      && (get_private_data (container)->unshare_flags & CLONE_NEWUSER))
    flags |= get_bind_mount_locked_flags (source_mountfd, mount->source, mount);

  ಯದಿ (flags & MS_BIND)
    {
      /* A remount sets exactly the requested flags and clears any other
         one.  A bind mount without mount flag options (e.g. "bind",
         "rbind,rprivate") keeps the flags of the source, so it must not
         be remounted.  */
      ಯದಿ (! has_mount_flag_options (mount))
        no_remount = MOUNT_NO_REMOUNT;

      ಯದಿ (path_is_slash_dev (mount->destination))
        get_private_data (container)->mount_dev_from_host = true;
      /* It is used only for error messages.  */
      type = "bind";
    }
  is_sysfs_or_proc = strcmp (type, "sysfs") == 0 || strcmp (type, "proc") == 0;

  ಯದಿ (strcmp (type, "tmpfs") == 0)
    {
      ret = append_tmpfs_mode_if_missing (container, mount, &data, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (mount->source && (flags & MS_BIND))
    {
      proc_fd_path_t proc_buf;
      ಧ್ರುವ ವರ್ಣ *path = mount->source;

      ಯದಿ ((extra_flags & OPTION_COPY_SYMLINK) && (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_DEST_NOFOLLOW)))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "`copy-symlink` is mutually exclusive with `src-nofollow` and `dest-nofollow`");

      bool is_cached_symlink = (extra_flags & OPTION_COPY_SYMLINK)
                               && get_private_data (container)->copy_symlink_targets
                               && mount_index < get_private_data (container)->n_copy_symlink_targets
                               && get_private_data (container)->copy_symlink_targets[mount_index];

      /* When the source is a cached symlink, skip the pre-opened fd since
         get_bind_mount() followed the symlink and the fd points to the target.
         For regular files with copy-symlink, the pre-opened fd is still valid.  */
      ಯದಿ (source_mountfd >= 0 && ! is_cached_symlink)
        {
          get_self_fd_path (proc_buf, source_mountfd);
          path = proc_buf;

          ret = get_file_type_at (source_mountfd, &src_mode, true, ರಿಕ್ತ);
        }
      ಅನ್ಯಥಾ ಯದಿ (is_cached_symlink)
        {
          src_mode = S_IFLNK;
          ret = 0;
        }
      ಅನ್ಯಥಾ ಯದಿ (path[0] == '/')
        {
          ಪೂರ್ಣಾಙ್ಕ old_ಬೇರು_fd = get_old_ಬೇರು_fd (get_private_data (container));
          bool nofollow = (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false;

          ಯದಿ (old_ಬೇರು_fd >= 0)
            ret = get_file_type_at (old_ಬೇರು_fd, &src_mode, nofollow, (path[1] == '\0') ? ರಿಕ್ತ : path + 1);
          ಅನ್ಯಥಾ
            ret = get_file_type_at (AT_FDCWD, &src_mode, nofollow, path);
        }
      ಅನ್ಯಥಾ
        ret = get_file_type (&src_mode, (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false, path);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot stat `%s`", path);

      ಯದಿ (S_ISLNK (src_mode) && (extra_flags & OPTION_DEST_NOFOLLOW) && source_mountfd < 0)
        {
          ಪೂರ್ಣಾಙ್ಕ bind_dirfd = AT_FDCWD;
          ಧ್ರುವ ವರ್ಣ *bind_source = mount->source;
          ಪೂರ್ಣಾಙ್ಕ old_ಬೇರು_fd = get_old_ಬೇರು_fd (get_private_data (container));

          ಯದಿ (old_ಬೇರು_fd >= 0 && mount->source && mount->source[0] == '/')
            {
              bind_dirfd = old_ಬೇರು_fd;
              bind_source = (mount->source[1] == '\0') ? "." : mount->source + 1;
            }
          ret = get_bind_mount (bind_dirfd, bind_source, true, true, extra_flags & OPTION_SRC_NOFOLLOW, MS_PRIVATE, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;

          source_mountfd = ret;
        }
    }

  ಯದಿ (S_ISLNK (src_mode) && (extra_flags & OPTION_COPY_SYMLINK))
    {
      ಧ್ರುವ ವರ್ಣ *cached_target = ರಿಕ್ತ;

      ಯದಿ (get_private_data (container)->copy_symlink_targets
          && mount_index < get_private_data (container)->n_copy_symlink_targets)
        cached_target = get_private_data (container)->copy_symlink_targets[mount_index];

      ret = handle_copy_symlink (container, rootfs, mount, cached_target, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      mounted = true;
    }
  ಅನ್ಯಥಾ ಯದಿ (is_sysfs_or_proc)
    {
      ret = prepare_sysfs_or_proc_mount (container, target, &targetfd, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಅನ್ಯಥಾ
    {
      bool is_dir = S_ISDIR (src_mode);

      ಯದಿ (extra_flags & OPTION_DEST_NOFOLLOW)
        {
          /* If dest-nofollow is specified, expect the target to exist.  */
          ret = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_PATH | O_NOFOLLOW | O_CLOEXEC, 0, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
          targetfd = ret;

          ret = check_valid_no_follow_file_system (targetfd, target, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }
      ಅನ್ಯಥಾ
        {
          /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
          ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
          targetfd = ret;
        }
    }

  ಯದಿ (extra_flags & OPTION_TMPCOPYUP)
    {
      ಯದಿ (strcmp (type, "tmpfs") != 0)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "tmpcopyup can be used only with tmpfs");

      /* targetfd is opened with O_PATH, reopen the fd so it can read.  */
      copy_from_fd = openat (targetfd, ".", O_CLOEXEC | O_RDONLY | O_DIRECTORY);
      ಯದಿ (UNLIKELY (copy_from_fd < 0))
        {
          ಯದಿ (errno != ENOTDIR)
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot reopen `%s`", target);
        }
    }

  source = mount->source ? mount->source : type;

  /* Check if there is already a mount for the requested file system.  */
  ಯದಿ (! mounted && source_mountfd >= 0)
    {
      ret = fs_move_mount_to (source_mountfd, targetfd, ರಿಕ್ತ);
      ಯದಿ (LIKELY (ret == 0))
        {
          unsigned long remaining_flags = flags & ~MS_BIND;

          /* A remount is needed not only to set some flags, but also to
             clear the ones set on the source (e.g. "bind,dev" on a nodev
             source), in which case remaining_flags might be empty.  */
          ಯದಿ (remaining_flags || has_mount_flag_options (mount))
            {
              ret = do_mount (container, ರಿಕ್ತ, source_mountfd, target, ರಿಕ್ತ, remaining_flags, data, LABEL_NONE | no_remount, err);
              ಯದಿ (UNLIKELY (ret < 0))
                ಪ್ರತ್ಯಾಗಮ ret;
            }

          mounted = true;

          ಯದಿ (is_empty_string (target))
            {
              ಪೂರ್ಣಾಙ್ಕ new_rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
              ಯದಿ (UNLIKELY (new_rootfsfd < 0))
                ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "reopen rootfs after mount on /");

              TEMP_FAILURE_RETRY (close (get_private_data (container)->rootfsfd));
              get_private_data (container)->rootfsfd = new_rootfsfd;
            }
        }
      ಅನ್ಯಥಾ ಯದಿ (get_private_data (container)->delayed_pidns_fork && strcmp (type, "proc") == 0)
        {
          /* The process is not in the container PID namespace yet, so
             mount(2) would create a proc instance for the wrong one.  */
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "the proc mount `%s` was not opened in the target PID namespace", target);
        }
      ಅನ್ಯಥಾ ಯದಿ (get_private_data (container)->host_ಬೇರು_switched)
        {
          /* The host root is unreachable now -- even through a fd, since
             mount(2) requires the source to live in the current mount
             namespace.  The path-based fallback below would resolve SOURCE
             inside the container and bind the wrong tree.  */
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "move_mount `%s` to `%s`", source, target);
        }
      ಅನ್ಯಥಾ ಯದಿ (errno != ENOSYS)
        {
          /* ENOSYS only means the kernel lacks the new mount API.  Anything
             else is worth reporting: it is otherwise invisible, and it decides
             whether the mount is set up from the pre-opened fd or from the
             source path.  */
          libcrun_warning ("move_mount `%s` to `%s` failed, falling back to mount(2): %s",
                           source, target, strerror (errno));
        }
    }

  ಯದಿ (! mounted)
    {
      ಯದಿ (systemd_cgroup_v1 && strcmp (mount->destination, systemd_cgroup_v1) == 0)
        {
          /* Override the cgroup mount with a single named cgroup name=systemd.  */
          ret = do_mount_cgroup_systemd_v1 (container, source, targetfd, target, flags, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }
      ಅನ್ಯಥಾ ಯದಿ (strcmp (type, "cgroup") == 0)
        {
          ret = do_mount_cgroup (container, source, targetfd, target, flags, data, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }
      ಅನ್ಯಥಾ
        {
          ಪೂರ್ಣಾಙ್ಕ extra_flags = get_mount_label_how (type, is_sysfs_or_proc) | no_remount;

          ret = do_mount (container, source, targetfd, target, type, flags, data, extra_flags, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }
    }

  ಯದಿ (copy_from_fd >= 0)
    {
      // handle_tmpcopyup closes copy_from_fd
      ret = handle_tmpcopyup (container, rootfs, target, copy_from_fd, err);
      copy_from_fd = -1;
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (rec_clear || rec_set)
    {
      ಧ್ರುವ bool is_dir = S_ISDIR (src_mode);
      cleanup_close ಪೂರ್ಣಾಙ್ಕ dfd = -1;

      dfd = safe_openat (get_private_data (container)->rootfsfd, rootfs, target, O_RDONLY | O_PATH | O_CLOEXEC | (is_dir ? O_DIRECTORY : 0), 0, err);
      ಯದಿ (UNLIKELY (dfd < 0))
        ಪ್ರತ್ಯಾಗಮ crun_error_wrap (err, "open mount target `/%s`", target);

      ret = do_mount_setattr (true, target, dfd, rec_clear, rec_set, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_mounts (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಧ್ರುವ ವರ್ಣ *systemd_cgroup_v1 = get_force_cgroup_v1_annotation (container);
  cleanup_close_map ಸಂರಚನಾ libcrun_fd_map *mount_fds = ರಿಕ್ತ;
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  mount_fds = get_private_data (container)->mount_fds;
  get_private_data (container)->mount_fds = ರಿಕ್ತ;

  ಪ್ರತಿ (i = 0; i < def->mounts_len; i++)
    {
      ret = process_single_mount (container, rootfs, def->mounts[i], mount_fds, i, systemd_cgroup_v1, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * libcrun_container_do_bind_mount
 *
 *  Allows external plugins and handlers to perform bind `mounts` on container.
 *  returns: 0 if successful anything else states `error` and configures `err` with relevant error.
 */
ಪೂರ್ಣಾಙ್ಕ
libcrun_container_do_bind_mount (libcrun_container_t *container, ವರ್ಣ *mount_source, ವರ್ಣ *mount_destination, ವರ್ಣ **mount_options, size_t mount_options_len, libcrun_error_t *err)
{
  ಧ್ರುವ ವರ್ಣ *target = consume_slashes (mount_destination);
  cleanup_free ವರ್ಣ *data = ರಿಕ್ತ;
  unsigned long flags = 0;
  unsigned long extra_flags = 0;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ targetfd = -1;
  ಪೂರ್ಣಾಙ್ಕ is_dir = 1;
  uint64_t rec_clear = 0;
  uint64_t rec_set = 0;
  ಧ್ರುವ ವರ್ಣ *rootfs = get_private_data (container)->rootfs;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ ((get_private_data (container)->rootfsfd < 0) || (rootfs == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid rootfs state while performing bind mount from external plugin or handler");

  ಯದಿ (mount_options == ರಿಕ್ತ)
    flags = get_default_flags (container, mount_destination, &data);
  ಅನ್ಯಥಾ
    {
      size_t j;

      ಪ್ರತಿ (j = 0; j < mount_options_len; j++)
        flags |= get_mount_flags_or_option (mount_options[j], flags, &extra_flags, &data, &rec_clear, &rec_set);
    }

  ಯದಿ (path_is_slash_dev (mount_destination))
    get_private_data (container)->mount_dev_from_host = true;

  ಯದಿ (mount_source && (flags & MS_BIND))
    {
      is_dir = crun_dir_p (mount_source, false, err);
      ಯದಿ (UNLIKELY (is_dir < 0))
        ಪ್ರತ್ಯಾಗಮ is_dir;
    }

  /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
  ret = crun_safe_create_and_open_ref_at (is_dir, get_private_data (container)->rootfsfd, rootfs, target, 0755, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  targetfd = ret;

  ret = do_mount (container, mount_source, targetfd, target, "bind", flags, data, LABEL_MOUNT, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ 0;
}

/*
  Open a fd to the NOTIFY_SOCKET end on the host.  If CONTAINER is NULL, then CONTEXT
  is used to retrieve the path to the socket.
*/
ಪೂರ್ಣಾಙ್ಕ
get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, ಪೂರ್ಣಾಙ್ಕ *notify_socket_out, libcrun_error_t *err)
{
#ifdef HAVE_SYSTEMD
  cleanup_close ಪೂರ್ಣಾಙ್ಕ notify_fd = -1;
  cleanup_free ವರ್ಣ *host_notify_socket_path = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *state_dir = ರಿಕ್ತ;
  ವರ್ಣ *host_path = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (container && get_private_data (container)->host_notify_socket_path)
    {
      ಧ್ರುವ ವರ್ಣ *parent_dir;

      parent_dir = get_private_data (container)->host_notify_socket_path;

      ret = append_paths (&host_notify_socket_path, err, parent_dir, "notify", ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      host_path = host_notify_socket_path;
    }

  *notify_socket_out = -1;

  ಯದಿ (host_path == ರಿಕ್ತ)
    {
      ret = libcrun_get_state_directory (&state_dir, context->state_ಬೇರು, context->id, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = append_paths (&host_notify_socket_path, err, state_dir, "notify/notify", ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      host_path = host_notify_socket_path;
    }

  notify_fd = open_unix_domain_socket (host_path, 1, err);
  ಯದಿ (UNLIKELY (notify_fd < 0))
    ಪ್ರತ್ಯಾಗಮ notify_fd;

  ಯದಿ (UNLIKELY (chmod (host_path, 0777) < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "chmod `%s`", host_path);

#  ifdef HAVE_FGETXATTR
  ಯದಿ (container && container->container_def->linux && container->container_def->linux->mount_label)
    {
      /* Ignore the error. The worst that can happen is that the container fails to notify it is ready.  */
      (ಶೂನ್ಯ) setxattr (host_path, "security.selinux", container->container_def->linux->mount_label,
                       strlen (container->container_def->linux->mount_label), 0);
    }
#  endif

  *notify_socket_out = get_and_reset (&notify_fd);
  ಪ್ರತ್ಯಾಗಮ 1;
#ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) context;
  (ಶೂನ್ಯ) container;
  (ಶೂನ್ಯ) err;
  *notify_socket_out = -1;
  ಪ್ರತ್ಯಾಗಮ 0;
#endif
}

#ifdef HAVE_SYSTEMD
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_notify_socket (libcrun_container_t *container, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಧ್ರುವ ವರ್ಣ *notify_socket = container->context->notify_socket;
  cleanup_free ವರ್ಣ *host_notify_socket_path = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *container_notify_socket_path = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *state_dir = ರಿಕ್ತ;
  uid_t container_ಬೇರು_uid = -1;
  gid_t container_ಬೇರು_gid = -1;
  ಪೂರ್ಣಾಙ್ಕ notify_socket_ವೃಕ್ಷ_fd;

  ಯದಿ (notify_socket == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_ಬೇರು : ರಿಕ್ತ),
                                     (container->context ? container->context->id : ರಿಕ್ತ), err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = append_paths (&container_notify_socket_path, err, "/", notify_socket, "notify", ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = append_paths (&host_notify_socket_path, err, state_dir, "notify", ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = mkdir (host_notify_socket_path, 0700);
  ಯದಿ (ret < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "mkdir `%s`", host_notify_socket_path);

  ಯದಿ (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
    {
      get_ಬೇರು_in_the_userns (container->container_def, 0, 0, &container_ಬೇರು_uid, &container_ಬೇರು_gid);
      ಯದಿ (container_ಬೇರು_uid != ((uid_t) -1) && container_ಬೇರು_gid != ((gid_t) -1))
        {
          ret = chown (host_notify_socket_path, container_ಬೇರು_uid, container_ಬೇರು_gid);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "chown `%s` to `%d:%d`", host_notify_socket_path, container_ಬೇರು_uid, container_ಬೇರು_gid);
        }
    }

  notify_socket_ವೃಕ್ಷ_fd = syscall_open_ವೃಕ್ಷ (AT_FDCWD, host_notify_socket_path, OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
  ಯದಿ (notify_socket_ವೃಕ್ಷ_fd >= 0)
    /* open_tree worked */
    get_private_data (container)->notify_socket_ವೃಕ್ಷ_fd = notify_socket_ವೃಕ್ಷ_fd;
  ಅನ್ಯಥಾ ಯದಿ (errno == EPERM)
    /* this can happen when trying to run a rootless container; this function is called
       in the original namespace where the caller is _not_ CAP_SYS_ADMIN - in that case,
       do nothing, because the bind mount of host_notify_socket_path directly should succeed
       since it will be readable by the container user. */
    ;
  ಅನ್ಯಥಾ ಯದಿ (errno == ENOSYS)
    /* if open_tree(2) is not available, do nothing; we will try mount(2) in do_finalize_notify_socket */
    ;
  ಅನ್ಯಥಾ
    /* some other error */
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open_tree `%s`", host_notify_socket_path);

  get_private_data (container)->host_notify_socket_path = host_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = container_notify_socket_path;
  host_notify_socket_path = container_notify_socket_path = ರಿಕ್ತ;
  ಪ್ರತ್ಯಾಗಮ 0;
}
#endif

/* Bind mount the notify socket directory into the rootfs.  It must run
   before pivot_root, while the host path is still reachable for the
   mount(2) fallback, and the destination is resolved inside the rootfs so
   that a symlink in the container image cannot point it elsewhere.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_finalize_notify_socket (libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *rootfs, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  cleanup_free ವರ್ಣ *host_notify_socket_path = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *container_notify_socket_path = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *container_notify_socket_path_dir_alloc = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *container_notify_socket_path_dir = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ notify_socket_ವೃಕ್ಷ_fd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ targetfd = -1;
  ಪೂರ್ಣಾಙ್ಕ did_mount_with_move_mount = 0;

  host_notify_socket_path = get_private_data (container)->host_notify_socket_path;
  get_private_data (container)->host_notify_socket_path = ರಿಕ್ತ;

  container_notify_socket_path = get_private_data (container)->container_notify_socket_path;
  get_private_data (container)->container_notify_socket_path = ರಿಕ್ತ;

  ಯದಿ (host_notify_socket_path == ರಿಕ್ತ || container_notify_socket_path == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯದಿ (rootfs == ರಿಕ್ತ)
    rootfs = "/";

  container_notify_socket_path_dir_alloc = xstrdup (container_notify_socket_path);
  container_notify_socket_path_dir = consume_slashes (dirname (container_notify_socket_path_dir_alloc));

  /* libcrun_set_mounts does not open the rootfs when there are no mounts.  */
  ಯದಿ (get_private_data (container)->rootfsfd < 0)
    {
      ret = open (rootfs, O_PATH | O_CLOEXEC);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", rootfs);

      get_private_data (container)->rootfsfd = ret;
      get_private_data (container)->rootfs = rootfs;
    }

  ret = crun_safe_create_and_open_ref_at (true, get_private_data (container)->rootfsfd, rootfs,
                                          container_notify_socket_path_dir, 0755, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  targetfd = ret;

  notify_socket_ವೃಕ್ಷ_fd = get_private_data (container)->notify_socket_ವೃಕ್ಷ_fd;
  /* the FD will be unconditionally closed at the end of this function due to cleanup_close above */
  get_private_data (container)->notify_socket_ವೃಕ್ಷ_fd = -1;

  ಯದಿ (notify_socket_ವೃಕ್ಷ_fd >= 0)
    {
      ret = fs_move_mount_to (notify_socket_ವೃಕ್ಷ_fd, targetfd, ರಿಕ್ತ);
      ಯದಿ (ret >= 0)
        {
          /* if move_mount(2) worked, make sure we don't try mount(2) */
          did_mount_with_move_mount = 1;

          /* the tree was cloned from the host mount namespace and is still in
             the peer group of its source, so make it private like the mount(2)
             fallback below does.  TARGETFD is now underneath the new mount,
             so reopen the target.  */
          close_and_reset (&targetfd);
          targetfd = open_mount_target (container, container_notify_socket_path_dir, err);
          ಯದಿ (UNLIKELY (targetfd < 0))
            ಪ್ರತ್ಯಾಗಮ targetfd;

          ret = do_mount (container, ರಿಕ್ತ, targetfd, container_notify_socket_path_dir, ರಿಕ್ತ,
                          MS_REC | MS_PRIVATE, ರಿಕ್ತ, LABEL_NONE, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }
      ಅನ್ಯಥಾ ಯದಿ (errno == ENOSYS)
        /* do nothing; we will try mount(2) next */
        ;
      ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "move_mount `%d` to `%s`", notify_socket_ವೃಕ್ಷ_fd,
                                container_notify_socket_path_dir);
    }

  ಯದಿ (! did_mount_with_move_mount)
    {
      ret = do_mount (container, host_notify_socket_path, targetfd, container_notify_socket_path_dir, ರಿಕ್ತ,
                      MS_BIND | MS_REC | MS_PRIVATE, ರಿಕ್ತ, LABEL_MOUNT, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
make_parent_mount_private (ಧ್ರುವ ವರ್ಣ *rootfs, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ rootfsfd = -1;
  ಸಂರಚನಾ stat prev_st;
  proc_fd_path_t proc_path;

  /* rootfs could be a relative path.  */
  rootfsfd = open (rootfs, O_PATH | O_CLOEXEC);
  ಯದಿ (UNLIKELY (rootfsfd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", rootfs);

  ಯದಿ (UNLIKELY (fstat (rootfsfd, &prev_st) < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fstat `%s`", rootfs);

  ಪ್ರತಿ (;;)
    {
      ಸಂರಚನಾ stat cur_st;
      libcrun_error_t tmp_err = ರಿಕ್ತ;
      ಪೂರ್ಣಾಙ್ಕ ret;
      cleanup_close ಪೂರ್ಣಾಙ್ಕ parentfd = -1;

      ret = do_mount_setattr (false, rootfs, rootfsfd, 0, MS_PRIVATE, &tmp_err);
      ಯದಿ (ret == 0)
        ಪ್ರತ್ಯಾಗಮ 0;
      crun_error_release (&tmp_err);

      get_proc_self_fd_path (proc_path, rootfsfd);
      ret = mount (ರಿಕ್ತ, proc_path, ರಿಕ್ತ, MS_PRIVATE, ರಿಕ್ತ);
      ಯದಿ (ret == 0)
        ಪ್ರತ್ಯಾಗಮ 0;

      parentfd = openat (rootfsfd, "..", O_PATH | O_CLOEXEC);
      ಯದಿ (parentfd < 0)
        {
          ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
          ret = faccessat (rootfsfd, "..", X_OK, AT_EACCESS);
          ಯದಿ (ret != 0)
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EACCES, "make `%s` private: a component is not accessible", rootfs);
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, saved_errno, "make `%s` private: cannot open component", rootfs);
        }

      ಯದಿ (UNLIKELY (fstat (parentfd, &cur_st) < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fstat parent of `%s`", rootfs);

      /* Reached the root of the file system: ".." points to itself.  */
      ಯದಿ (cur_st.st_dev == prev_st.st_dev && cur_st.st_ino == prev_st.st_ino)
        ಭಙ್ಗ;

      prev_st = cur_st;
      close_and_reset (&rootfsfd);
      rootfsfd = get_and_reset (&parentfd);
    }

  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "make `%s` private", rootfs);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_set_mounts (ಸಂರಚನಾ container_entrypoint_s *entrypoint_args, libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *rootfs, set_mounts_cb_t cb, ಶೂನ್ಯ *cb_data, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಪೂರ್ಣಾಙ್ಕ is_user_ns = 0;
  ಪೂರ್ಣಾಙ್ಕ ret = 0;

  ಯದಿ (rootfs == ರಿಕ್ತ || def->mounts == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  ret = open (rootfs, O_PATH | O_CLOEXEC);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", rootfs);

  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = rootfs;

  // configure handler mounts
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_MOUNTS, container, rootfs, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_MOUNTS");

  ಯದಿ (def->ಬೇರು->readonly)
    {
      ಸಂರಚನಾ remount_s *r;
      unsigned long remount_flags = MS_REMOUNT | MS_BIND | MS_RDONLY;
      ಪೂರ್ಣಾಙ್ಕ fd;

      fd = dup (get_private_data (container)->rootfsfd);
      ಯದಿ (UNLIKELY (fd < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "dup fd for `%s`", rootfs);

      r = make_remount (fd, rootfs, remount_flags, ರಿಕ್ತ, get_private_data (container)->remounts);
      get_private_data (container)->remounts = r;
    }

  /* Mounts are already done before pivot_root in setup_mount_namespace.  */

  is_user_ns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER);
  ಯದಿ (! is_user_ns)
    {
      is_user_ns = check_running_in_user_namespace (err);
      ಯದಿ (UNLIKELY (is_user_ns < 0))
        ಪ್ರತ್ಯಾಗಮ is_user_ns;
    }

  ಯದಿ (! get_private_data (container)->mount_dev_from_host)
    {
      ret = create_missing_devs (container, is_user_ns ? true : false, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  /* Notify the callback after all the mounts are ready but before making them read-only.  */
  ಯದಿ (cb)
    {
      ret = cb (cb_data, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (def->process && def->process->cwd)
    {
      libcrun_error_t tmp_err = ರಿಕ್ತ;
      ಧ್ರುವ ವರ್ಣ *rel_cwd = consume_slashes (def->process->cwd);
      /* Ignore errors here and let it fail later.  */
      (ಶೂನ್ಯ) crun_safe_ensure_directory_at (get_private_data (container)->rootfsfd, rootfs, rel_cwd, 0755, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ret = do_masked_and_readonly_paths (container, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
umount_or_hide (ಧ್ರುವ ವರ್ಣ *target, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = umount2 (target, MNT_DETACH);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;

      /* If the umount2 failed with EINVAL then the mount could
         be locked.  Hide it by mounting a tmpfs on top of it.  */
      ಯದಿ (errno == EINVAL)
        {
          ret = mount (ರಿಕ್ತ, target, "tmpfs", 0, "size=0k");
          ಯದಿ (LIKELY (ret == 0))
            ಪ್ರತ್ಯಾಗಮ 0;
        }

      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, saved_errno, "umount `%s`", target);
    }

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
move_ಬೇರು (ಧ್ರುವ ವರ್ಣ *rootfs, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = chdir (rootfs);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "chdir to `%s`", rootfs);

  ret = umount_or_hide ("/sys", err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = umount_or_hide ("/proc", err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = mount (rootfs, "/", "", MS_MOVE, "");
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "mount MS_MOVE to `/`");

  ret = chroot (".");
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "chroot to `%s`", rootfs);

  ret = chdir ("/");
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "chdir to `%s`", rootfs);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_finalize_mounts (ಸಂರಚನಾ container_entrypoint_s *entrypoint_args, libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *rootfs,
                         set_mounts_cb_t after_pivot_cb, ಶೂನ್ಯ *cb_data, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  // configure handler mounts for phase: HANDLER_CONFIGURE_AFTER_MOUNTS
  ret = libcrun_container_notify_handler (entrypoint_args, HANDLER_CONFIGURE_AFTER_MOUNTS, container, rootfs, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_AFTER_MOUNTS");

  ret = do_finalize_notify_socket (container, rootfs, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  close_and_reset (&(get_private_data (container)->rootfsfd));

  /* The host root must not stay reachable through /proc/self/fd once the
     container rootfs becomes the root: every path resolved by name from now
     on is controlled by the container image.  */
  close_and_reset (&(get_private_data (container)->old_ಬೇರು_fd));

  ಯದಿ (get_private_data (container)->needs_pivot)
    {
      get_private_data (container)->needs_pivot = false;

      ಯದಿ (get_private_data (container)->no_pivot)
        {
          ret = move_ಬೇರು (rootfs, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }
      ಅನ್ಯಥಾ
        {
          ret = do_pivot (container, rootfs, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }

      ret = do_mount (container, ರಿಕ್ತ, -1, "/", ರಿಕ್ತ,
                      get_private_data (container)->rootfs_propagation,
                      ರಿಕ್ತ, LABEL_MOUNT, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = chdir ("/");
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "chdir to `/`");
    }

  ಯದಿ (after_pivot_cb)
    {
      ret = after_pivot_cb (cb_data, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = finalize_mounts (container, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
maybe_open_ವೃಕ್ಷ_namespace (ಧ್ರುವ ವರ್ಣ *rootfs, ಪೂರ್ಣಾಙ್ಕ *out_fd, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ rootfs_fd = -1;
  ಪೂರ್ಣಾಙ್ಕ ವೃಕ್ಷ_fd;

  *out_fd = -1;

  rootfs_fd = open (rootfs, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ಯದಿ (UNLIKELY (rootfs_fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", rootfs);

  ವೃಕ್ಷ_fd = syscall_open_ವೃಕ್ಷ (rootfs_fd, "",
                               OPEN_ವೃಕ್ಷ_NAMESPACE | OPEN_TREE_CLOEXEC
                                   | AT_EMPTY_PATH | AT_RECURSIVE);
  ಯದಿ (ವೃಕ್ಷ_fd < 0)
    {
      ಯದಿ (errno == EINVAL || errno == ENOSYS || errno == EPERM)
        ಪ್ರತ್ಯಾಗಮ 0;
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open_tree `%s`", rootfs);
    }

  *out_fd = ವೃಕ್ಷ_fd;
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಸಂರಚನಾ libcrun_fd_map *
get_fd_map (libcrun_container_t *container)
{
  ಸಂರಚನಾ libcrun_fd_map *mount_fds = get_private_data (container)->mount_fds;

  ಯದಿ (mount_fds == ರಿಕ್ತ)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      mount_fds = make_libcrun_fd_map (def->mounts_len);
      get_private_data (container)->mount_fds = mount_fds;
    }
  ಪ್ರತ್ಯಾಗಮ mount_fds;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
open_mount_of_type (libcrun_container_t *container,
                    runtime_spec_schema_defs_mount *mount,
                    libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free ವರ್ಣ *mnt_data = ರಿಕ್ತ;
  unsigned long mnt_flags = 0, mnt_extra = 0;
  uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
  ಧ್ರುವ ವರ್ಣ *label_type = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *label_val = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *fstype;
  ಪೂರ್ಣಾಙ್ಕ mnt_fd;
  size_t j;

  ಯದಿ (mount->type == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ -1;

  fstype = mount->type;

  ಪ್ರತಿ (j = 0; j < mount->options_len; j++)
    mnt_flags |= get_mount_flags_or_option (mount->options[j],
                                            mnt_flags, &mnt_extra, &mnt_data,
                                            &mnt_rec_clear, &mnt_rec_set);

  ಯದಿ (def->linux && def->linux->mount_label)
    {
      bool is_sysfs_or_proc = strcmp (fstype, "sysfs") == 0
                              || strcmp (fstype, "proc") == 0;
      ಪೂರ್ಣಾಙ್ಕ extra_flags = get_mount_label_how (fstype, is_sysfs_or_proc);
      ಯದಿ (extra_flags == LABEL_MOUNT)
        {
          label_type = get_selinux_context_type (container, err);
          ಯದಿ (label_type == ರಿಕ್ತ)
            crun_error_release (err);
          ಅನ್ಯಥಾ
            label_val = def->linux->mount_label;
        }
    }

  ಯದಿ (strcmp (fstype, "cgroup") == 0)
    {
      ಪೂರ್ಣಾಙ್ಕ cgroup_mode = libcrun_get_cgroup_mode (err);
      ಯದಿ (cgroup_mode == CGROUP_MODE_UNIFIED)
        fstype = "cgroup2";
      crun_error_release (err);
    }

  mnt_fd = fsopen_mount (fstype, mount->source, label_type, label_val, mnt_data);
  ಯದಿ (mnt_fd < 0 && label_type)
    mnt_fd = fsopen_mount (fstype, mount->source, ರಿಕ್ತ, ರಿಕ್ತ, mnt_data);

  ಪ್ರತ್ಯಾಗಮ mnt_fd;
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
ಸಂರಚನಾ mnt_id_req_s
{
  uint32_t size;
  uint32_t spare;
  uint64_t mnt_id;
  uint64_t param;
  uint64_t mnt_ns_id;
};

ಸಂರಚನಾ statmount_s
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
  ವರ್ಣ str[];
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
syscall_listmount (ಸಂರಚನಾ mnt_id_req_s *req, uint64_t *ids, size_t nr, unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
#ifdef __NR_listmount
  ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) syscall (__NR_listmount, req, ids, nr, flags);
#ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) req;
  (ಶೂನ್ಯ) ids;
  (ಶೂನ್ಯ) nr;
  (ಶೂನ್ಯ) flags;
  errno = ENOSYS;
  ಪ್ರತ್ಯಾಗಮ -1;
#endif
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
syscall_statmount (ಸಂರಚನಾ mnt_id_req_s *req, ಸಂರಚನಾ statmount_s *buf, size_t size, unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
#ifdef __NR_statmount
  ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) syscall (__NR_statmount, req, buf, size, flags);
#ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) req;
  (ಶೂನ್ಯ) buf;
  (ಶೂನ್ಯ) size;
  (ಶೂನ್ಯ) flags;
  errno = ENOSYS;
  ಪ್ರತ್ಯಾಗಮ -1;
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
ಸ್ಥಿರ bool
mounts_hit_pinned_mount_namespace (runtime_spec_schema_config_schema *def)
{
  cleanup_free uint64_t *ids = ರಿಕ್ತ;
  size_t i, n_ids = 0, capacity = 128;
  bool has_absolute_bind = false;
  uint64_t last = 0;

  ಪ್ರತಿ (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false, nofollow = false;

      ಯದಿ (is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && def->mounts[i]->source != ರಿಕ್ತ && def->mounts[i]->source[0] == '/')
        {
          has_absolute_bind = true;
          ಭಙ್ಗ;
        }
    }
  ಯದಿ (! has_absolute_bind)
    ಪ್ರತ್ಯಾಗಮ false;

  ids = xmalloc (capacity * ಪರಿಮಾಣ (uint64_t));
  ಪ್ರತಿ (;;)
    {
      ಸಂರಚನಾ mnt_id_req_s req = {
        .size = ಪರಿಮಾಣ (req),
        .mnt_id = LSMT_ROOT,
        .param = last,
      };
      ಪೂರ್ಣಾಙ್ಕ ret;

      ಯದಿ (n_ids == capacity)
        {
          capacity *= 2;
          ids = xrealloc (ids, capacity * ಪರಿಮಾಣ (uint64_t));
        }

      ret = syscall_listmount (&req, ids + n_ids, capacity - n_ids, 0);
      ಯದಿ (ret < 0)
        /* Without the mount table there is no way to tell; the kernel would
           reject the move_mount() anyway, so stay on the traditional path.  */
        ಪ್ರತ್ಯಾಗಮ true;
      ಯದಿ (ret == 0)
        ಭಙ್ಗ;

      n_ids += (size_t) ret;
      last = ids[n_ids - 1];
    }

  ಪ್ರತಿ (i = 0; i < n_ids; i++)
    {
      ವರ್ಣ buffer[4096] __attribute__ ((aligned (8)));
      ಸಂರಚನಾ statmount_s *sm = (ಸಂರಚನಾ statmount_s *) buffer;
      ಸಂರಚನಾ mnt_id_req_s req = {
        .size = ಪರಿಮಾಣ (req),
        .mnt_id = ids[i],
        .param = STATMOUNT_FS_TYPE | STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT,
      };
      ಧ್ರುವ ವರ್ಣ *ಬೇರು, *point;
      size_t j;

      ಯದಿ (syscall_statmount (&req, sm, ಪರಿಮಾಣ (buffer), 0) < 0)
        ಅನುವರ್ತ;
      ಯದಿ ((sm->mask & (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT)) != (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT))
        ಅನುವರ್ತ;

      /* The root of a pinned mount namespace reads as "mnt:[...]"; the other
         namespace types cannot create a loop.  */
      ಬೇರು = sm->str + sm->mnt_root;
      ಯದಿ (! has_prefix (ಬೇರು, "mnt:["))
        ಅನುವರ್ತ;

      point = sm->str + sm->mnt_point;

      ಪ್ರತಿ (j = 0; j < def->mounts_len; j++)
        {
          bool recursive = false, nofollow = false;
          ಧ್ರುವ ವರ್ಣ *source = def->mounts[j]->source;
          size_t source_len;

          ಯದಿ (! is_bind_mount (def->mounts[j], &recursive, &nofollow))
            ಅನುವರ್ತ;
          ಯದಿ (source == ರಿಕ್ತ || source[0] != '/')
            ಅನುವರ್ತ;

          source_len = strlen (source);
          ಯಾವತ್ (source_len > 1 && source[source_len - 1] == '/')
            source_len--;

          ಯದಿ (strncmp (point, source, source_len) != 0)
            ಅನುವರ್ತ;

          /* The source itself is a pinned mount namespace.  */
          ಯದಿ (point[source_len] == '\0')
            ಪ್ರತ್ಯಾಗಮ true;

          /* A recursive bind clones the subtree below the source as well, so
             a pin anywhere under it is picked up.  A plain bind clones the
             single mount, and cannot pick one up.  */
          ಯದಿ (recursive && (source_len == 1 || point[source_len] == '/'))
            ಪ್ರತ್ಯಾಗಮ true;
        }
    }

  ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
can_use_open_ವೃಕ್ಷ_namespace (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಸಂರಚನಾ libcrun_fd_map *mount_fds;
  bool has_hooks = def->hooks
                   && (def->hooks->prestart_len || def->hooks->create_runtime_len
                       || def->hooks->create_container_len);
  bool has_userns = get_private_data (container)->unshare_flags & CLONE_NEWUSER;
  size_t i;

  ಯದಿ (has_hooks || has_userns)
    ಪ್ರತ್ಯಾಗಮ false;

  ಯದಿ (mounts_hit_pinned_mount_namespace (def))
    ಪ್ರತ್ಯಾಗಮ false;

  ಯದಿ (container->context->no_open_ವೃಕ್ಷ_namespace)
    ಪ್ರತ್ಯಾಗಮ false;

  {
    libcrun_error_t tmp_err = ರಿಕ್ತ;
    ಪೂರ್ಣಾಙ್ಕ in_userns = check_running_in_user_namespace (&tmp_err);
    crun_error_release (&tmp_err);
    ಯದಿ (in_userns != 0)
      ಪ್ರತ್ಯಾಗಮ false;
  }

  mount_fds = get_fd_map (container);
  ಪ್ರತಿ (i = 0; i < def->mounts_len; i++)
    {
      ಯದಿ (mount_fds->fds[i] < 0)
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];
          ಯದಿ (! has_cached_target)
            ಪ್ರತ್ಯಾಗಮ false;
        }
    }

  ಪ್ರತ್ಯಾಗಮ true;
}

/* Only tmpfs mounts inherit the mode of the directory they are mounted
   over, and only when the mount does not already request an explicit mode=
   option.  This mirrors append_tmpfs_mode_if_missing() used by the fallback
   mount() path.  File systems such as proc, sysfs and mqueue have a root
   inode mode defined by the kernel that must not be overridden.  */
ಸ್ಥಿರ bool
mount_inherits_mountpoint_mode (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  ಯದಿ (mount->type == ರಿಕ್ತ || strcmp (mount->type, "tmpfs") != 0)
    ಪ್ರತ್ಯಾಗಮ false;

  ಪ್ರತಿ (i = 0; i < mount->options_len; i++)
    ಯದಿ (mount->options[i] != ರಿಕ್ತ && has_prefix (mount->options[i], "mode="))
      ಪ್ರತ್ಯಾಗಮ false;

  ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
setup_mount_namespace (libcrun_container_t *container, bool no_pivot, ವರ್ಣ **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  unsigned long rootfs_propagation = 0;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ rootfsfd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ ವೃಕ್ಷ_fd = -1;
  libcrun_error_t tmp_err = ರಿಕ್ತ;
  bool use_open_ವೃಕ್ಷ = false;
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (def->linux && def->linux->rootfs_propagation)
    rootfs_propagation = get_mount_flags (def->linux->rootfs_propagation, 0, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ);

  ಯದಿ ((rootfs_propagation & ALL_PROPAGATIONS_NO_REC) == 0)
    rootfs_propagation = MS_REC | MS_PRIVATE;

  get_private_data (container)->rootfs_propagation = rootfs_propagation;

  ret = libcrun_container_enter_cgroup_ns (container, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಮುಕ್ತ (get_private_data (container)->host_rootfs);
  get_private_data (container)->host_rootfs = xstrdup (*rootfs);

  /* Pre-create mounts and cache paths before pivot_root,
     while the host file system is still reachable.  */
  ಪ್ರತಿ (i = 0; i < def->mounts_len; i++)
    {
      ಸಂರಚನಾ libcrun_fd_map *mount_fds = get_fd_map (container);
      cleanup_close ಪೂರ್ಣಾಙ್ಕ mnt_fd = -1;
      bool recursive_bind, nofollow;
      unsigned long mnt_flags = 0;

      ಯದಿ (mount_fds && mount_fds->fds[i] >= 0)
        ಅನುವರ್ತ;

      /* Pre-read symlink targets for copy-symlink mounts while the host file system is still reachable.  */
      ಯದಿ (def->mounts[i]->source != ರಿಕ್ತ
          && def->mounts[i]->source[0] == '/')
        {
          cleanup_free ವರ್ಣ *mnt_data = ರಿಕ್ತ;
          unsigned long mnt_extra = 0;
          uint64_t mnt_rec_clear = 0, mnt_rec_set = 0;
          size_t j;

          ಪ್ರತಿ (j = 0; j < def->mounts[i]->options_len; j++)
            mnt_flags |= get_mount_flags_or_option (def->mounts[i]->options[j],
                                                    mnt_flags, &mnt_extra, &mnt_data,
                                                    &mnt_rec_clear, &mnt_rec_set);

          ಯದಿ (mnt_extra & OPTION_COPY_SYMLINK)
            {
              cleanup_free ವರ್ಣ *target = ರಿಕ್ತ;
              libcrun_error_t tmp_err = ರಿಕ್ತ;
              ssize_t len;

              len = safe_readlinkat (AT_FDCWD, def->mounts[i]->source, &target, 0, &tmp_err);
              ಯದಿ (UNLIKELY (len < 0))
                crun_error_release (&tmp_err);

              ಯದಿ (get_private_data (container)->copy_symlink_targets == ರಿಕ್ತ)
                {
                  get_private_data (container)->copy_symlink_targets
                      = xmalloc0 (def->mounts_len * ಪರಿಮಾಣ (ವರ್ಣ *));
                  get_private_data (container)->n_copy_symlink_targets = def->mounts_len;
                }
              get_private_data (container)->copy_symlink_targets[i] = target;
              target = ರಿಕ್ತ;
            }
        }

      ಯದಿ (is_bind_mount (def->mounts[i], &recursive_bind, &nofollow))
        {
          bool has_cached_target = get_private_data (container)->copy_symlink_targets
                                   && i < get_private_data (container)->n_copy_symlink_targets
                                   && get_private_data (container)->copy_symlink_targets[i];

          ಯದಿ (mount_fds && def->mounts[i]->source != ರಿಕ್ತ && ! has_cached_target
              && get_path_in_rootfs (*rootfs, def->mounts[i]->source) == ರಿಕ್ತ)
            {
              libcrun_error_t tmp_err = ರಿಕ್ತ;
              unsigned long propagation = MS_PRIVATE;

              ಯದಿ (mnt_flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE))
                propagation = 0;

              mount_fds->fds[i] = get_bind_mount (AT_FDCWD, def->mounts[i]->source,
                                                  recursive_bind, false, nofollow,
                                                  propagation, &tmp_err);
              ಯದಿ (mount_fds->fds[i] < 0)
                crun_error_release (&tmp_err);
            }
          ಅನುವರ್ತ;
        }

      mnt_fd = open_mount_of_type (container, def->mounts[i], err);
      ಯದಿ (mnt_fd < 0)
        {
          crun_error_release (err);
          ಅನುವರ್ತ;
        }

      ಯದಿ (mount_inherits_mountpoint_mode (def->mounts[i]))
        {
          cleanup_close ಪೂರ್ಣಾಙ್ಕ destfd = -1;
          ಸಂರಚನಾ stat st;

          ಯದಿ (rootfsfd < 0)
            {
              rootfsfd = open (*rootfs, O_PATH | O_CLOEXEC);
              ಯದಿ (UNLIKELY (rootfsfd < 0))
                ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", *rootfs);
            }

          destfd = safe_openat (rootfsfd, *rootfs, def->mounts[i]->destination, O_PATH | O_CLOEXEC, 0, &tmp_err);
          ಯದಿ (destfd < 0)
            crun_error_release (&tmp_err);
          ಅನ್ಯಥಾ ಯದಿ (fstat (destfd, &st) == 0)
            {
              proc_fd_path_t fd_path;
              ಪೂರ್ಣಾಙ್ಕ procfd = get_procfd (get_private_data (container), err);
              ಯದಿ (UNLIKELY (procfd < 0))
                ಪ್ರತ್ಯಾಗಮ procfd;

              get_self_fd_path (fd_path, mnt_fd);
              ret = fchmodat (procfd, fd_path, st.st_mode & 07777, 0);
              (ಶೂನ್ಯ) ret;
            }
        }

      mount_fds->fds[i] = mnt_fd;
      mnt_fd = -1;
    }
  close_and_reset (&rootfsfd);

  ret = get_shared_empty_dir_cached (container, ರಿಕ್ತ, &tmp_err);
  ಯದಿ (ret < 0)
    {
      crun_error_release (&tmp_err);
      get_private_data (container)->maskdir_bind_failed = true;
    }

  ಯದಿ (! get_private_data (container)->joined_mount_ns
      && can_use_open_ವೃಕ್ಷ_namespace (container))
    use_open_ವೃಕ್ಷ = true;

  ಯದಿ (use_open_ವೃಕ್ಷ)
    {
      ret = maybe_open_ವೃಕ್ಷ_namespace (*rootfs, &ವೃಕ್ಷ_fd, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  /* Pre-open needed device fds and the notify socket while the host
     file system is still reachable.  After setns(OPEN_TREE_NAMESPACE)
     the host /dev is no longer accessible.  When tree_fd < 0 (no
     OPEN_TREE_NAMESPACE), this is retried after unshare(CLONE_NEWNS)
     below since open_tree needs CAP_SYS_ADMIN in the user namespace
     that owns the mount namespace.  */
  ಯದಿ (ವೃಕ್ಷ_fd >= 0)
    preopen_needed_devs_and_notify (container);

  ಯದಿ (ವೃಕ್ಷ_fd >= 0)
    {
      ret = setns (ವೃಕ್ಷ_fd, CLONE_NEWNS);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "setns `CLONE_NEWNS`");

      ret = mount (ರಿಕ್ತ, "/", ರಿಕ್ತ, MS_REMOUNT | MS_BIND, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "remount `/`");

      ret = do_mount (container, ರಿಕ್ತ, -1, "/", ರಿಕ್ತ, MS_REC | MS_PRIVATE, ರಿಕ್ತ, LABEL_MOUNT, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = do_mount (container, ರಿಕ್ತ, -1, "/", ರಿಕ್ತ, rootfs_propagation, ರಿಕ್ತ, LABEL_MOUNT, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      get_private_data (container)->needs_pivot = false;
      get_private_data (container)->no_pivot = no_pivot;
      /* setns() made the container rootfs the process root, so an absolute
         source path no longer refers to the host.  */
      get_private_data (container)->host_ಬೇರು_switched = true;
      ಮುಕ್ತ (*rootfs);
      *rootfs = xstrdup ("/");
    }
  ಅನ್ಯಥಾ
    {
      ಯದಿ (! get_private_data (container)->joined_mount_ns)
        {
          ret = unshare (CLONE_NEWNS);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
        }

      ret = do_mount (container, ರಿಕ್ತ, -1, "/", ರಿಕ್ತ, rootfs_propagation, ರಿಕ್ತ, LABEL_MOUNT, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = make_parent_mount_private (*rootfs, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = do_mount (container, *rootfs, -1, *rootfs, ರಿಕ್ತ, MS_BIND | MS_REC | MS_PRIVATE, ರಿಕ್ತ, LABEL_MOUNT, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      get_private_data (container)->needs_pivot = true;
      get_private_data (container)->no_pivot = no_pivot;
    }

  /* Mount everything before pivot_root while host paths are still reachable.
     Use the pre-created fd when available, fall back to mount().  */
  ret = open (*rootfs, O_PATH | O_CLOEXEC);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", *rootfs);
  get_private_data (container)->rootfsfd = ret;
  get_private_data (container)->rootfs = *rootfs;

  ಯದಿ (! container->context->force_no_cgroup)
    {
      ಪೂರ್ಣಾಙ್ಕ cgroup_mode = libcrun_get_cgroup_mode (err);
      ಯದಿ (UNLIKELY (cgroup_mode < 0))
        ಪ್ರತ್ಯಾಗಮ cgroup_mode;

      ಯದಿ (cgroup_mode == CGROUP_MODE_UNIFIED)
        {
          ವರ್ಣ *unified_cgroup_path = ರಿಕ್ತ;
          ಪೂರ್ಣಾಙ್ಕ procfd = get_procfd (get_private_data (container), err);
          ಯದಿ (UNLIKELY (procfd < 0))
            ಪ್ರತ್ಯಾಗಮ procfd;

          ret = libcrun_get_cgroup_process_at (procfd, &unified_cgroup_path, true, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;

          get_private_data (container)->unified_cgroup_path = unified_cgroup_path;
        }
    }

  ret = do_mounts (container, *rootfs, err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      close_and_reset (&get_private_data (container)->rootfsfd);
      get_private_data (container)->rootfs = ರಿಕ್ತ;
      ಪ್ರತ್ಯಾಗಮ ret;
    }

  close_and_reset (&get_private_data (container)->rootfsfd);
  get_private_data (container)->rootfs = ರಿಕ್ತ;

  ಯದಿ (ವೃಕ್ಷ_fd < 0)
    preopen_needed_devs_and_notify (container);

  /* After pivot_root, /proc is not yet mounted.  */
  ret = get_procfd (get_private_data (container), &tmp_err);
  ಯದಿ (ret < 0)
    crun_error_release (&tmp_err);

  check_running_in_user_namespace (&tmp_err);
  crun_error_release (&tmp_err);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_do_pivot_ಬೇರು (libcrun_container_t *container, bool no_pivot, ವರ್ಣ **rootfs, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (*rootfs == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯದಿ (get_private_data (container)->unshare_flags & CLONE_NEWNS)
    {
      ret = setup_mount_namespace (container, no_pivot, rootfs, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      /* If setup_mount_namespace used OPEN_TREE_NAMESPACE, rootfs is
         already set to "/".  Otherwise pivot_root is deferred until
         after the mounts are created.  */
      ಯದಿ (strcmp (*rootfs, "/") == 0)
        {
          ret = chdir ("/");
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "chdir to `/`");
        }
    }
  ಅನ್ಯಥಾ
    {
      ret = chroot (*rootfs);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "chroot to `%s`", *rootfs);

      ret = chdir ("/");
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "chdir to `/`");

      ಮುಕ್ತ (*rootfs);
      *rootfs = xstrdup ("/");
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* If one of stdin, stdout, stderr are pointing to /dev/null on
 * the outside of the container, this moves it to /dev/null inside
 * of the container. This needs to run after pivot/chroot-ing. */
ಪೂರ್ಣಾಙ್ಕ
libcrun_reopen_dev_null (libcrun_error_t *err)
{
  ಸಂರಚನಾ stat statbuf;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd;
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಪೂರ್ಣಾಙ್ಕ i;

  /* Open /dev/null inside of the container. */
  fd = open ("/dev/null", O_RDWR | O_CLOEXEC | O_NOFOLLOW);
  ಯದಿ (UNLIKELY (fd == -1))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `/dev/null`");

  ret = ensure_dev_null (fd, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತಿ (i = 0; i <= 2; i++)
    {
      ಯದಿ (UNLIKELY (fstat (i, &statbuf) == -1))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "stat fd `%d`", i);
      ಯದಿ (is_dev_null (&statbuf))
        {
          /* This FD is pointing to /dev/null. Point it to /dev/null inside
           * of the container. */
          ಯದಿ (UNLIKELY (dup2 (fd, i) == -1))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "dup2 `%d`", i);
        }
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
uidgidmap_helper (ವರ್ಣ *helper, pid_t pid, ಧ್ರುವ ವರ್ಣ *map_file, libcrun_error_t *err)
{
#define MAX_ARGS 20
  ವರ್ಣ pid_fmt[16];
  ವರ್ಣ *args[MAX_ARGS + 1];
  ವರ್ಣ *next;
  cleanup_free ವರ್ಣ *map_file_copy = xstrdup (map_file);
  size_t nargs = 0;
  ಪೂರ್ಣಾಙ್ಕ ret;

  args[nargs++] = helper;

  ret = snprintf (pid_fmt, ಪರಿಮಾಣ (pid_fmt), "%d", pid);
  ಯದಿ (UNLIKELY (ret >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (pid_fmt)))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

  args[nargs++] = pid_fmt;
  next = map_file_copy;
  ಯಾವತ್ (nargs < MAX_ARGS)
    {
      ವರ್ಣ *p = strsep (&next, " \n");
      ಯದಿ (next == ರಿಕ್ತ)
        ಭಙ್ಗ;
      args[nargs++] = p;
    }
  args[nargs++] = ರಿಕ್ತ;

  ret = run_process (args, err);
  ಯದಿ (ret < 0)
    ಪ್ರತ್ಯಾಗಮ ret;
  ಯದಿ (ret > 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "`%s` exited with status %d", helper, ret);
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
newgidmap (pid_t pid, ಧ್ರುವ ವರ್ಣ *map_file, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ uidgidmap_helper ("newgidmap", pid, map_file, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
newuidmap (pid_t pid, ಧ್ರುವ ವರ್ಣ *map_file, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ uidgidmap_helper ("newuidmap", pid, map_file, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
deny_setgroups (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;

  fd = libcrun_open_proc_pid_file (container, pid, "setgroups", O_WRONLY, err);
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ fd;

  ret = TEMP_FAILURE_RETRY (ಲಿಖ (fd, "deny", 4));
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "write to `/proc/%d/setgroups`", pid);

  get_private_data (container)->deny_setgroups = true;
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
can_setgroups (libcrun_container_t *container, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  cleanup_free ವರ್ಣ *content = ರಿಕ್ತ;

  ಯದಿ (get_private_data (container)->deny_setgroups)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯದಿ (container->container_def->annotations)
    {
      ಧ್ರುವ ವರ್ಣ *annotation;

      /* Skip setgroups if the annotation is set to anything different than "0".  */
      annotation = find_annotation (container, "run.oci.keep_original_groups");
      ಯದಿ (annotation)
        ಪ್ರತ್ಯಾಗಮ strcmp (annotation, "0") == 0 ? 1 : 0;
    }

  {
    cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
    fd = libcrun_open_proc_file (container, "self/setgroups", O_RDONLY, err);
    ಯದಿ (fd < 0)
      ret = fd;
    ಅನ್ಯಥಾ
      ret = read_all_fd (fd, "self/setgroups", &content, ರಿಕ್ತ, err);
  }
  ಯದಿ (ret < 0)
    {
      /* If the file does not exist, then the kernel does not support /proc/self/setgroups and setgroups can always be used.  */
      ಯದಿ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ಪ್ರತ್ಯಾಗಮ 1;
        }
      ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ strncmp (content, "deny", 4) == 0 ? 0 : 1;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_setgroups (libcrun_container_t *container,
                             runtime_spec_schema_config_schema_process *process,
                             libcrun_error_t *err)
{
  gid_t *additional_gids = ರಿಕ್ತ;
  size_t additional_gids_len = 0;
  ಪೂರ್ಣಾಙ್ಕ can_do_setgroups;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (process != ರಿಕ್ತ && process->user != ರಿಕ್ತ)
    {
      additional_gids = process->user->additional_gids;
      additional_gids_len = process->user->additional_gids_len;
    }

  can_do_setgroups = can_setgroups (container, err);
  ಯದಿ (UNLIKELY (can_do_setgroups < 0))
    ಪ್ರತ್ಯಾಗಮ can_do_setgroups;

  ಯದಿ (can_do_setgroups == 0)
    {
      ಧ್ರುವ ವರ್ಣ *annotation;
      size_t i;

      annotation = find_annotation (container, "run.oci.keep_original_groups");
      ಯದಿ (annotation && strcmp (annotation, "0") != 0)
        ಪ್ರತ್ಯಾಗಮ 0;

      /* The supplementary groups cannot be changed.  Fail if any of the
         requested additional gids would not be in effect, but tolerate
         the primary gid since it is already granted by setgid.  */
      ಪ್ರತಿ (i = 0; i < additional_gids_len; i++)
        ಯದಿ (additional_gids[i] != process->user->gid)
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EPERM, "cannot set additional gid `%d`: setgroups is denied in the user namespace",
                                  (ಪೂರ್ಣಾಙ್ಕ) additional_gids[i]);
      ಪ್ರತ್ಯಾಗಮ 0;
    }

  ret = setgroups (additional_gids_len, additional_gids);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "setgroups");

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err)
{
  ಯದಿ (get_private_data (container)->unshare_cgroupns)
    {
      ಪೂರ್ಣಾಙ್ಕ ret = unshare (CLONE_NEWCGROUP);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          ಯದಿ (errno != EINVAL)
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "unshare (CLONE_NEWCGROUP)");
        }
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

// determine whether the uid/gid mappings only contain a single entry
// that maps the host uid/gid on the process->user->uid/gid
ಪೂರ್ಣಾಙ್ಕ
is_single_mapping (runtime_spec_schema_defs_id_mapping **mappings, size_t len,
                   uint32_t host_id, uint32_t container_id)
{
  ಯದಿ (len != 1)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯದಿ (mappings[0]->size != 1)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯದಿ (mappings[0]->container_id != container_id || mappings[0]->host_id != host_id)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_id_map (libcrun_container_t *container, pid_t pid, ಧ್ರುವ ವರ್ಣ *map_name,
              ಪೂರ್ಣಾಙ್ಕ (*idmap_helper) (pid_t, ಧ್ರುವ ವರ್ಣ *, libcrun_error_t *), ಧ್ರುವ ವರ್ಣ *helper_name,
              bool warn_on_failure, ಧ್ರುವ ವರ್ಣ *map, size_t map_len,
              runtime_spec_schema_defs_id_mapping **mappings, size_t mappings_len,
              uint32_t host_id, uint32_t container_id, bool skip_if_setgroups_denied,
              libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret = 0;

  ಯದಿ (container->host_uid)
    ret = idmap_helper (pid, map, err);
  ಯದಿ (container->host_uid == 0 || ret < 0)
    {
      cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;

      ಯದಿ (ret < 0)
        {
          ಯದಿ (warn_on_failure)
            libcrun_warning ("unable to invoke `%s`, will try creating a user namespace with single mapping as an alternative", helper_name);
          crun_error_release (err);
        }

      fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
      ಯದಿ (UNLIKELY (fd < 0))
        ಪ್ರತ್ಯಾಗಮ fd;

      ret = safe_write (fd, map_name, map, map_len, err);
      ಯದಿ (ret < 0 && (! mappings_len || is_single_mapping (mappings, mappings_len, host_id, container_id)))
        {
          size_t single_mapping_len;
          cleanup_free ವರ್ಣ *single_mapping = ರಿಕ್ತ;
          crun_error_release (err);

          ಯದಿ (! skip_if_setgroups_denied || ! get_private_data (container)->deny_setgroups)
            {
              ret = deny_setgroups (container, pid, err);
              ಯದಿ (UNLIKELY (ret < 0))
                ಪ್ರತ್ಯಾಗಮ ret;
            }

          ret = format_mount_mapping (&single_mapping, container_id, host_id, 1, &single_mapping_len, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;

          close_and_reset (&fd);

          fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
          ಯದಿ (UNLIKELY (fd < 0))
            ಪ್ರತ್ಯಾಗಮ fd;

          ret = safe_write (fd, map_name, single_mapping, single_mapping_len, err);
        }
    }
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_set_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *uid_map = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *gid_map = ರಿಕ್ತ;
  size_t uid_map_len = 0, gid_map_len = 0;
  ಪೂರ್ಣಾಙ್ಕ ret = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  ಯದಿ ((get_private_data (container)->unshare_flags & CLONE_NEWUSER) == 0)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯದಿ (! def || ! def->linux)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯದಿ (def->linux->uid_mappings_len)
    {
      ret = format_mount_mappings (&uid_map, def->linux->uid_mappings, def->linux->uid_mappings_len, &uid_map_len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಅನ್ಯಥಾ
    {
      ret = format_default_id_mapping (&uid_map, container->container_uid, container->host_uid, container->host_uid, 1, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      uid_map_len = (size_t) ret;
      ಯದಿ (uid_map == ರಿಕ್ತ)
        {
          ret = format_mount_mapping (&uid_map, 0, container->host_uid, container->host_uid + 1, &uid_map_len, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }
    }

  ಯದಿ (def->linux->gid_mappings_len)
    {
      ret = format_mount_mappings (&gid_map, def->linux->gid_mappings, def->linux->gid_mappings_len, &gid_map_len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಅನ್ಯಥಾ
    {
      ret = format_default_id_mapping (&gid_map, container->container_gid, container->host_uid, container->host_gid, 0, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      gid_map_len = (size_t) ret;
      ಯದಿ (gid_map == ರಿಕ್ತ)
        {
          ret = format_mount_mapping (&gid_map, 0, container->host_gid, container->host_gid + 1, &gid_map_len, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }
    }

  ret = write_id_map (container, pid, "gid_map", newgidmap, "newgidmap",
                      ! def->linux->uid_mappings_len, gid_map, gid_map_len,
                      def->linux->gid_mappings, def->linux->gid_mappings_len,
                      container->host_gid, container->container_gid, /* skip_if_setgroups_denied */ false, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = write_id_map (container, pid, "uid_map", newuidmap, "newuidmap",
                      ! def->linux->uid_mappings_len, uid_map, uid_map_len,
                      def->linux->uid_mappings, def->linux->uid_mappings_len,
                      container->host_uid, container->container_uid, /* skip_if_setgroups_denied */ true, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ 0;
}

#define CAP_TO_MASK_0(x) (1L << ((x) & 31))
#define CAP_TO_MASK_1(x) CAP_TO_MASK_0 (x - 32)

ಸಂರಚನಾ all_caps_s
{
  unsigned long effective[2];
  unsigned long permitted[2];
  unsigned long inheritable[2];
  unsigned long ambient[2];
  unsigned long bounding[2];
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
has_cap_on (ಪೂರ್ಣಾಙ್ಕ cap, long unsigned *caps)
{
  ಯದಿ (cap < 32)
    ಪ್ರತ್ಯಾಗಮ CAP_TO_MASK_0 (cap) & caps[0];
  ಪ್ರತ್ಯಾಗಮ (CAP_TO_MASK_1 (cap) & caps[1]);
}

ಸ್ಥಿರ unsigned long cap_last_cap;

ಪೂರ್ಣಾಙ್ಕ
libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
  ಪೂರ್ಣಾಙ್ಕ ret;
  ವರ್ಣ buffer[32];
  fd = libcrun_open_proc_file (container, "sys/kernel/cap_last_cap", O_RDONLY, err);
  ಯದಿ (fd < 0)
    ಪ್ರತ್ಯಾಗಮ fd;
  ret = TEMP_FAILURE_RETRY (ಪಠ (fd, buffer, ಪರಿಮಾಣ (buffer) - 1));
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "read from `/proc/sys/kernel/cap_last_cap`");
  buffer[ret] = '\0';

  errno = 0;
  cap_last_cap = strtoul (buffer, ರಿಕ್ತ, 10);
  ಯದಿ (errno != 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "strtoul `%s` from `/proc/sys/kernel/cap_last_cap`", buffer);
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
set_required_caps (ಸಂರಚನಾ all_caps_s *caps, uid_t uid, gid_t gid, ಪೂರ್ಣಾಙ್ಕ no_new_privs, bool no_exec, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  unsigned long cap;
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಸಂರಚನಾ __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  ಸಂರಚನಾ __user_cap_data_struct data[2] = { { 0 } };

  ಯದಿ (cap_last_cap == 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: max number of capabilities not initialized");

  ಪ್ರತಿ (cap = 0; cap <= cap_last_cap; cap++)
    ಯದಿ (! has_cap_on (cap, caps->bounding))
      {
        ret = prctl (PR_CAPBSET_DROP, cap, 0, 0, 0);
        ಯದಿ (UNLIKELY (ret < 0 && ! (errno == EINVAL)))
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "prctl drop bounding");
      }

  data[0].effective = caps->effective[0];
  data[1].effective = caps->effective[1];
  data[0].inheritable = caps->inheritable[0];
  data[1].inheritable = caps->inheritable[1];
  data[0].permitted = caps->permitted[0];
  data[1].permitted = caps->permitted[1];

  ret = prctl (PR_SET_KEEPCAPS, 1, 0, 0, 0);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "prctl set KEEPCAPS");

  /* -1 means "leave unchanged" for setresgid and setresuid.  */
  ಯದಿ (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresgid (gid, gid, gid);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "setresgid to `%d`", gid);

  ret = setresuid (uid, uid, uid);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = capset (&hdr, data);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "capset");

#  ifdef PR_CAP_AMBIENT
  ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_CLEAR_ALL, 0, 0, 0);
  ಯದಿ (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "prctl reset ambient");

  ಪ್ರತಿ (cap = 0; cap <= cap_last_cap; cap++)
    ಯದಿ (has_cap_on (cap, caps->ambient))
      {
        ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, cap, 0, 0);
        ಯದಿ (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "prctl ambient raise");
      }
#  endif

  /* If the workload runs in the current process, there is no execve(2) that resets KEEPCAPS and,
     for a non-root user, leaves only the ambient set in the permitted and effective sets.  */
  ಯದಿ (no_exec)
    {
      ret = prctl (PR_SET_KEEPCAPS, 0, 0, 0, 0);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "prctl reset KEEPCAPS");

      ಯದಿ (uid != 0)
        {
          data[0].permitted = data[1].permitted = 0;
#  ifdef PR_CAP_AMBIENT
          ಪ್ರತಿ (cap = 0; cap <= cap_last_cap; cap++)
            ಯದಿ (prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_IS_SET, cap, 0, 0) > 0)
              data[cap / 32].permitted |= CAP_TO_MASK_0 (cap);
#  endif
          data[0].effective = data[0].permitted;
          data[1].effective = data[1].permitted;

          ret = capset (&hdr, data);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "capset");
        }
    }
#endif

  ಯದಿ (no_new_privs)
    ಯದಿ (UNLIKELY (prctl (PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0))
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "prctl set no new privs");

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
read_caps (unsigned long caps[2], ವರ್ಣ **ಮೌಲ್ಯಗಳು, size_t len, libcrun_error_t *err)
{
#ifdef HAVE_CAP
  size_t i;
  ಪ್ರತಿ (i = 0; i < len; i++)
    {
      cap_ಮೌಲ್ಯ_t cap;
      ಯದಿ (cap_from_name (ಮೌಲ್ಯಗಳು[i], &cap) < 0)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "unknown capability `%s`", ಮೌಲ್ಯಗಳು[i]);

      ಯದಿ (cap < 32)
        caps[0] |= CAP_TO_MASK_0 (cap);
      ಅನ್ಯಥಾ
        caps[1] |= CAP_TO_MASK_1 (cap);
    }
#ಅನ್ಯಥಾ
  caps[0] = 0;
  caps[1] = 0;
#endif
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_set_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  ಯದಿ (proc->selinux_label)
    ಪ್ರತ್ಯಾಗಮ set_selinux_label (container, proc->selinux_label, now, err);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_set_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  ಯದಿ (proc->apparmor_profile)
    ಪ್ರತ್ಯಾಗಮ set_apparmor_profile (container, proc->apparmor_profile, proc->no_new_privileges, now, err);
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_set_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                  ಪೂರ್ಣಾಙ್ಕ no_new_privileges, bool no_exec, libcrun_error_t *err)
{
  ಸಂರಚನಾ all_caps_s caps = {};
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (capabilities)
    {
      ret = read_caps (caps.effective, capabilities->effective, capabilities->effective_len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = read_caps (caps.inheritable, capabilities->inheritable, capabilities->inheritable_len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = read_caps (caps.ambient, capabilities->ambient, capabilities->ambient_len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = read_caps (caps.bounding, capabilities->bounding, capabilities->bounding_len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = read_caps (caps.permitted, capabilities->permitted, capabilities->permitted_len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಪ್ರತ್ಯಾಗಮ set_required_caps (&caps, uid, gid, no_new_privileges, no_exec, err);
}

ಸಂರಚನಾ rlimit_s
{
  ಧ್ರುವ ವರ್ಣ *name;
  ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ;
};

ಸಂರಚನಾ rlimit_s rlimits[] = { { "RLIMIT_AS", RLIMIT_AS },
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
                              { "RLIMIT_STACK", RLIMIT_ರಾಶಿ },
                              { ರಿಕ್ತ, 0 } };

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_rlimit_resource (ಧ್ರುವ ವರ್ಣ *name)
{
  ಸಂರಚನಾ rlimit_s *it;
  ಪ್ರತಿ (it = rlimits; it->name; it++)
    ಯದಿ (strcmp (it->name, name) == 0)
      ಪ್ರತ್ಯಾಗಮ it->ಮೌಲ್ಯ;
  ಪ್ರತ್ಯಾಗಮ -1;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_set_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **new_rlimits, size_t len,
                     libcrun_error_t *err)
{
  size_t i, j;
  /* Check for duplicate rlimit types.  */
  ಪ್ರತಿ (i = 0; i < len; i++)
    {
      ಪ್ರತಿ (j = i + 1; j < len; j++)
        {
          ಯದಿ (strcmp (new_rlimits[i]->type, new_rlimits[j]->type) == 0)
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "duplicate rlimit `%s`", new_rlimits[i]->type);
        }
    }

  ಪ್ರತಿ (i = 0; i < len; i++)
    {
      ಸಂರಚನಾ rlimit limit;
      ವರ್ಣ *type = new_rlimits[i]->type;
      ಪೂರ್ಣಾಙ್ಕ resource = get_rlimit_resource (type);
      ಯದಿ (UNLIKELY (resource < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid rlimit `%s`", type);
      limit.rlim_cur = new_rlimits[i]->soft;
      limit.rlim_max = new_rlimits[i]->hard;
      libcrun_debug ("Set rlimit: soft = `%llu`, hard = `%llu`",
                     (unsigned long long) limit.rlim_cur,
                     (unsigned long long) limit.rlim_max);
      ಯದಿ (UNLIKELY (setrlimit (resource, &limit) < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "setrlimit `%s`", type);
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_set_hostname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಪೂರ್ಣಾಙ್ಕ has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಯದಿ (def->hostname == ರಿಕ್ತ || def->hostname[0] == '\0')
    ಪ್ರತ್ಯಾಗಮ 0;
  ಯದಿ (! has_uts)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "hostname requires the UTS namespace");
  ret = sethostname (def->hostname, strlen (def->hostname));
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "sethostname");
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_set_domainname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಪೂರ್ಣಾಙ್ಕ has_uts = get_private_data (container)->unshare_flags & CLONE_NEWUTS;
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಯದಿ (is_empty_string (def->domainname))
    ಪ್ರತ್ಯಾಗಮ 0;
  ಯದಿ (! has_uts)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "domainname requires the UTS namespace");
  ret = setdomainname (def->domainname, strlen (def->domainname));
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "setdomainname");
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_set_oom (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
  ಪೂರ್ಣಾಙ್ಕ ret;
  ವರ್ಣ oom_buffer[16];
  ಯದಿ (def->process == ರಿಕ್ತ || ! def->process->oom_score_adj_present)
    ಪ್ರತ್ಯಾಗಮ 0;
  libcrun_debug ("Write OOM score adj: `%d`", def->process->oom_score_adj);

  ret = snprintf (oom_buffer, ಪರಿಮಾಣ (oom_buffer), "%i", def->process->oom_score_adj);
  ಯದಿ (UNLIKELY (ret >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (oom_buffer)))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

  fd = libcrun_open_proc_file (container, "self/oom_score_adj", O_RDWR, err);
  ಯದಿ (fd < 0)
    ಪ್ರತ್ಯಾಗಮ fd;
  ret = TEMP_FAILURE_RETRY (ಲಿಖ (fd, oom_buffer, strlen (oom_buffer)));
  ಯದಿ (ret < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "write to `/proc/self/oom_score_adj`");
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಧ್ರುವ ವರ್ಣ *sysctlRequiringIPC[] = {
  "kernel/msgmax",
  "kernel/msgmnb",
  "kernel/msgmni",
  "kernel/sem",
  "kernel/shmall",
  "kernel/shmmax",
  "kernel/shmmni",
  "kernel/shm_rmid_forced",
  ರಿಕ್ತ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
validate_sysctl (ಧ್ರುವ ವರ್ಣ *original_ಕೀಲಿ, ಧ್ರುವ ವರ್ಣ *original_ಮೌಲ್ಯ, ಧ್ರುವ ವರ್ಣ *name, unsigned long namespaces_created, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  ಧ್ರುವ ವರ್ಣ *namespace = "";

  /* The name is used as a relative path with openat(), so a leading '/' would
     make it escape /proc/sys altogether, and a ".." component could reach a
     sysctl other than the one being validated here.  */
  ಯದಿ (name[0] == '/' || path_has_dot_dot_component (name))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "the sysctl `%s` contains an invalid path", original_ಕೀಲಿ);

  ಯದಿ (has_prefix (name, "fs/mqueue/"))
    {
      ಯದಿ (namespaces_created & CLONE_NEWIPC)
        ಪ್ರತ್ಯಾಗಮ 0;

      namespace = "IPC";
      ಗಚ್ಛ fail;
    }

  ಯದಿ (has_prefix (name, "kernel/"))
    {
      size_t i;

      ಪ್ರತಿ (i = 0; sysctlRequiringIPC[i]; i++)
        ಯದಿ (strcmp (sysctlRequiringIPC[i], name) == 0)
          {
            ಯದಿ (namespaces_created & CLONE_NEWIPC)
              ಪ್ರತ್ಯಾಗಮ 0;

            namespace = "IPC";
            ಗಚ್ಛ fail;
          }

      ಯದಿ (strcmp (name, "kernel/domainname") == 0)
        {
          // Value of sysctl `kernel/domainname` is going to
          // conflict with already set field `domainname` in
          // OCI spec, in such scenario crun will fail to prevent
          // unexpected behaviour for end user.
          ಯದಿ (! is_empty_string (def->domainname) && (strcmp (original_ಮೌಲ್ಯ, def->domainname) != 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `domainname`", original_ಕೀಲಿ);

          ಯದಿ (namespaces_created & CLONE_NEWUTS)
            ಪ್ರತ್ಯಾಗಮ 0;

          namespace = "UTS";
          ಗಚ್ಛ fail;
        }

      ಯದಿ (strcmp (name, "kernel/hostname") == 0)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `hostname`", original_ಕೀಲಿ);
    }
  ಯದಿ (has_prefix (name, "net/"))
    {
      ಯದಿ (namespaces_created & CLONE_NEWNET)
        ಪ್ರತ್ಯಾಗಮ 0;

      namespace = "network";
      ಗಚ್ಛ fail;
    }

  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "the sysctl `%s` is not namespaced", original_ಕೀಲಿ);

fail:
  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "the sysctl `%s` requires a new %s namespace", original_ಕೀಲಿ, namespace);
}

/* Best-effort attempt to give a better explanation why setting a sysctl could have failed. */
ಸ್ಥಿರ ವರ್ಣ *
sysctl_error_reason (ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ namespaces_created, ಪೂರ್ಣಾಙ್ಕ errno_)
{

  ಯದಿ (strcmp (name, "net.ipv4.ping_group_range") == 0 && (errno_ == EINVAL) && (namespaces_created & CLONE_NEWUSER))
    ಪ್ರತ್ಯಾಗಮ xstrdup ("are all the IDs mapped in the user namespace?");

  ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_set_sysctl (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ dirfd = -1;
  unsigned long namespaces_created = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  ಯದಿ (def->linux == ರಿಕ್ತ || def->linux->sysctl == ರಿಕ್ತ || def->linux->sysctl->len == 0)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಪ್ರತಿ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ;

      ಮೌಲ್ಯ = libcrun_find_namespace (def->linux->namespaces[i]->type);
      ಯದಿ (UNLIKELY (ಮೌಲ್ಯ < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      namespaces_created |= ಮೌಲ್ಯ;
    }

  get_private_data (container);
  dirfd = libcrun_open_proc_file (container, "sys", O_DIRECTORY | O_PATH, err);
  ಯದಿ (UNLIKELY (dirfd < 0))
    ಪ್ರತ್ಯಾಗಮ dirfd;

  ಪ್ರತಿ (i = 0; i < def->linux->sysctl->len; i++)
    {
      cleanup_free ವರ್ಣ *name = ರಿಕ್ತ;
      cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
      ಪೂರ್ಣಾಙ್ಕ ret;

      /* A sysctl whose value is null in the document is parsed into a NULL,
         and there is nothing to write to the file.  */
      ಯದಿ (def->linux->sysctl->ಕೀಲಿಗಳು[i] == ರಿಕ್ತ || def->linux->sysctl->ಮೌಲ್ಯಗಳು[i] == ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EINVAL, "sysctl value is not specified");

      name = libcrun_sysctl_ಕೀಲಿ_to_proc_path (def->linux->sysctl->ಕೀಲಿಗಳು[i]);

      ret = validate_sysctl (def->linux->sysctl->ಕೀಲಿಗಳು[i], def->linux->sysctl->ಮೌಲ್ಯಗಳು[i], name, namespaces_created, def, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      ಯದಿ (UNLIKELY (fd < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `/proc/sys/%s`", name);

      ret = TEMP_FAILURE_RETRY (ಲಿಖ (fd, def->linux->sysctl->ಮೌಲ್ಯಗಳು[i], strlen (def->linux->sysctl->ಮೌಲ್ಯಗಳು[i])));
      ಯದಿ (UNLIKELY (ret < 0))
        {
          ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
          cleanup_free ವರ್ಣ *reason = ರಿಕ್ತ;

          reason = sysctl_error_reason (def->linux->sysctl->ಕೀಲಿಗಳು[i], namespaces_created, saved_errno);
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, saved_errno, "write to `/proc/sys/%s`%s%s%s", name, reason ? " (" : "", reason ?: "", reason ? ")" : "");
        }
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
open_terminal (ವರ್ಣ **pty, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
  uid_t uid = 0;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (process && process->user)
    uid = process->user->uid;

  fd = libcrun_new_terminal (pty, err);
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ fd;

  ret = libcrun_set_stdio (fd, *pty, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (uid)
    {
      /* The slave is now stdin, use the descriptor instead of resolving the path again.  */
      ret = fchown (0, uid, -1);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "chown `%s`", *pty);
    }

  ret = get_and_reset (&fd);
  ಪ್ರತ್ಯಾಗಮ ret;
}

ವರ್ಣ *
libcrun_get_external_descriptors (libcrun_container_t *container)
{
  ಪ್ರತ್ಯಾಗಮ get_private_data (container)->external_descriptors;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err)
{
  ಧ್ರುವ ವರ್ಣ *buf = ರಿಕ್ತ;
  json_gen_ctx *gen = ರಿಕ್ತ;
  size_t buf_len;
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಪೂರ್ಣಾಙ್ಕ i;

  ಯದಿ (! json_gen_init (&gen, ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "json_gen_init");

  ret = json_gen_array_open (gen);
  ಯದಿ (UNLIKELY (ret != json_gen_status_ok))
    ಗಚ್ಛ json_error;

  /* Remember original stdin, stdout, stderr for container restore.  */
  ಪ್ರತಿ (i = 0; i < 3; i++)
    {
      proc_fd_path_t fd_path;
      ವರ್ಣ link_path[PATH_MAX];

      get_proc_self_fd_path (fd_path, i);

      ret = readlink (fd_path, link_path, PATH_MAX - 1);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          /* The fd could not exist.  */
          ಯದಿ (errno == ENOENT)
            {
              strcpy (link_path, "/dev/null");
              ret = 9; /* strlen ("/dev/null").  */
            }
          ಅನ್ಯಥಾ
            {
              json_gen_free (gen);
              ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "readlink `%s`", fd_path);
            }
        }
      link_path[ret] = 0;

      ret = json_gen_string (gen, link_path, ret);
      ಯದಿ (UNLIKELY (ret != json_gen_status_ok))
        ಗಚ್ಛ json_error;
    }

  ret = json_gen_array_close (gen);
  ಯದಿ (UNLIKELY (ret != json_gen_status_ok))
    ಗಚ್ಛ json_error;

  ret = json_gen_get_buf (gen, &buf, &buf_len);
  ಯದಿ (UNLIKELY (ret != json_gen_status_ok))
    ಗಚ್ಛ json_error;

  ಯದಿ (buf)
    {
      ವರ್ಣ *b = xmalloc (buf_len + 1);
      memcpy (b, buf, buf_len);
      b[buf_len] = '\0';
      get_private_data (container)->external_descriptors = b;
    }

  json_gen_free (gen);

  ಪ್ರತ್ಯಾಗಮ 0;

json_error:
  ಯದಿ (gen)
    json_gen_free (gen);
  ಪ್ರತ್ಯಾಗಮ json_gen_error_to_crun_error (ret, err);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_set_terminal (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free ವರ್ಣ *pty = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (def->process == ರಿಕ್ತ || ! def->process->terminal)
    ಪ್ರತ್ಯಾಗಮ 0;

  fd = open_terminal (&pty, def->process, err);
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ fd;

  ಯದಿ (def->process->console_size)
    {
      ret = libcrun_terminal_setup_size (0, def->process->console_size->height, def->process->console_size->width, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (get_private_data (container)->mount_dev_from_host)
    {
      ret = do_mount (container, pty, -1, "/dev/console", ರಿಕ್ತ, MS_BIND, ರಿಕ್ತ, LABEL_MOUNT, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಅನ್ಯಥಾ
    {
      ret = unlink ("/dev/console");
      ಯದಿ (UNLIKELY (ret < 0 && errno != ENOENT))
        {
          ಯದಿ (errno == EROFS)
            {
              /* If the file system is read-only, fall back to a bind mount.  */
              ret = do_mount (container, pty, -1, "/dev/console", ರಿಕ್ತ, MS_BIND, ರಿಕ್ತ, LABEL_MOUNT, err);
              ಯದಿ (UNLIKELY (ret < 0))
                ಪ್ರತ್ಯಾಗಮ ret;

              ಪ್ರತ್ಯಾಗಮ get_and_reset (&fd);
            }
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "unlink `/dev/console`");
        }

      ret = symlink (pty, "/dev/console");
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "symlink `/dev/console` -> `%s`", pty);
    }

  ಪ್ರತ್ಯಾಗಮ get_and_reset (&fd);
}

ಸ್ಥಿರ bool
read_error_from_sync_socket (ಪೂರ್ಣಾಙ್ಕ sync_socket_fd, ಪೂರ್ಣಾಙ್ಕ *error, ವರ್ಣ **str)
{
  cleanup_free ವರ್ಣ *b = ರಿಕ್ತ;
  size_t size;
  ಪೂರ್ಣಾಙ್ಕ code = 0;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (*error == 0)
    {
      ret = TEMP_FAILURE_RETRY (ಪಠ (sync_socket_fd, &code, ಪರಿಮಾಣ (code)));
      ಯದಿ (UNLIKELY (ret != (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (code)))
        ಪ್ರತ್ಯಾಗಮ false;
      *error = code;
    }

  ret = TEMP_FAILURE_RETRY (ಪಠ (sync_socket_fd, &size, ಪರಿಮಾಣ (size)));
  ಯದಿ (UNLIKELY (ret != (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (size)))
    ಪ್ರತ್ಯಾಗಮ false;

  ಯದಿ (size == 0)
    ಪ್ರತ್ಯಾಗಮ false;

  ಯದಿ (size > 1024)
    size = 1024;

  b = xmalloc (size + 1);
  ret = TEMP_FAILURE_RETRY (ಪಠ (sync_socket_fd, b, size));
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ false;

  b[ret] = '\0';

  *str = b;
  b = ರಿಕ್ತ;
  ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
send_error_to_sync_socket (ಪೂರ್ಣಾಙ್ಕ sync_socket_fd, bool has_fd, libcrun_error_t *err)
{
  size_t size;
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಪೂರ್ಣಾಙ್ಕ code;
  ವರ್ಣ *ಸಂದೇಶ;

  ಯದಿ (err == ರಿಕ್ತ || *err == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ false;

  code = crun_error_get_errno (err);

  ಯದಿ (has_fd)
    {
      /* dummy terminal fd.  */
      ret = TEMP_FAILURE_RETRY (ಲಿಖ (sync_socket_fd, "1", 1));
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ false;
    }

  ret = TEMP_FAILURE_RETRY (ಲಿಖ (sync_socket_fd, &code, ಪರಿಮಾಣ (code)));
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ false;

  ಸಂದೇಶ = (*err)->ಸಂದೇಶ;
  size = strlen (ಸಂದೇಶ) + 1;
  ret = TEMP_FAILURE_RETRY (ಲಿಖ (sync_socket_fd, &size, ಪರಿಮಾಣ (size)));
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ false;

  ret = TEMP_FAILURE_RETRY (ಲಿಖ (sync_socket_fd, ಸಂದೇಶ, size));
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ false;

  ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
send_success_to_sync_socket (ಪೂರ್ಣಾಙ್ಕ sync_socket, libcrun_error_t *err)
{
  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ success = 0;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = TEMP_FAILURE_RETRY (ಲಿಖ (sync_socket, &success, ಪರಿಮಾಣ (success)));
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "write to sync socket");

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ __attribute__ ((noreturn)) ಶೂನ್ಯ
send_error_to_sync_socket_and_die (ಪೂರ್ಣಾಙ್ಕ sync_socket_fd, bool has_terminal, libcrun_error_t *err)
{
  ವರ್ಣ *ಸಂದೇಶ;

  ಯದಿ (err == ರಿಕ್ತ || *err == ರಿಕ್ತ)
    _safe_exit (EXIT_FAILURE);

  ಯದಿ (! send_error_to_sync_socket (sync_socket_fd, has_terminal, err))
    {
      errno = crun_error_get_errno (err);
      ಸಂದೇಶ = (*err)->ಸಂದೇಶ;
      libcrun_fail_with_error (errno, "%s", ಸಂದೇಶ);
    }
  _safe_exit (EXIT_FAILURE);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
expect_success_from_sync_socket (ಪೂರ್ಣಾಙ್ಕ sync_fd, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *err_str = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ res = 1;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = TEMP_FAILURE_RETRY (ಪಠ (sync_fd, &res, ಪರಿಮಾಣ (res)));
  ಯದಿ (UNLIKELY (ret != ಪರಿಮಾಣ (res)))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ret < 0 ? errno : 0, "read status from sync socket");

  ಯದಿ (res == 0)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯದಿ (read_error_from_sync_socket (sync_fd, &res, &err_str))
    {
      ಯದಿ (! is_empty_string (err_str))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, res, "%s", err_str);
    }

  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, res, "read from sync socket");
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
join_namespaces (runtime_spec_schema_config_schema *def, ಪೂರ್ಣಾಙ್ಕ *namespaces_to_join, ಪೂರ್ಣಾಙ್ಕ n_namespaces_to_join,
                 ಪೂರ್ಣಾಙ್ಕ *namespaces_to_join_index, bool ignore_join_errors, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಪೂರ್ಣಾಙ್ಕ i;

  ಪ್ರತಿ (i = 0; i < n_namespaces_to_join; i++)
    {
      cleanup_free ವರ್ಣ *cwd = ರಿಕ್ತ;
      ಪೂರ್ಣಾಙ್ಕ orig_index = namespaces_to_join_index[i];
      ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ;

      ಯದಿ (namespaces_to_join[i] < 0)
        ಅನುವರ್ತ;

      /* Skip the user namespace.  */
      ಮೌಲ್ಯ = libcrun_find_namespace (def->linux->namespaces[orig_index]->type);
      ಯದಿ (ಮೌಲ್ಯ == CLONE_NEWUSER)
        ಅನುವರ್ತ;

      ಯದಿ (ಮೌಲ್ಯ == CLONE_NEWNS)
        {
          cwd = getcwd (ರಿಕ್ತ, 0);
          ಯದಿ (UNLIKELY (cwd == ರಿಕ್ತ))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "getcwd");
        }

      libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[orig_index]->type, def->linux->namespaces[orig_index]->path);
      ret = setns (namespaces_to_join[i], ಮೌಲ್ಯ);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          ಯದಿ (ignore_join_errors)
            ಅನುವರ್ತ;
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "setns `%s`", def->linux->namespaces[orig_index]->path);
        }

      close_and_reset (&namespaces_to_join[i]);

      ಯದಿ (ಮೌಲ್ಯ == CLONE_NEWNS)
        {
          ret = chdir (cwd);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "chdir `%s`", cwd);
        }
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

#define MAX_NAMESPACES 10

ಸಂರಚನಾ init_status_s
{
  /* fd to the namespace to join.  */
  ಪೂರ್ಣಾಙ್ಕ fd[MAX_NAMESPACES + 1];
  /* Index into def->linux->namespaces.  */
  ಪೂರ್ಣಾಙ್ಕ index[MAX_NAMESPACES];
  /* CLONE_* value.  */
  ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ[MAX_NAMESPACES];
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
  ಪೂರ್ಣಾಙ್ಕ userns_index;
  /* def->linux->namespaces userns.  */
  ಪೂರ್ಣಾಙ್ಕ userns_index_origin;

  /* All namespaces created/joined by the container.  */
  ಪೂರ್ಣಾಙ್ಕ all_namespaces;

  /* What namespaces are still missing to be created.  */
  ಪೂರ್ಣಾಙ್ಕ namespaces_to_unshare;

  /* Index in fd[] for the pidns that must be joined before any
     other namespace.  */
  ಪೂರ್ಣಾಙ್ಕ idx_pidns_to_join_immediately;

  /* Index in fd[] for the timens that must be joined before any
     other namespace.  */
  ಪೂರ್ಣಾಙ್ಕ idx_timens_to_join_immediately;
};

ಶೂನ್ಯ
cleanup_free_init_statusp (ಸಂರಚನಾ init_status_s *ns)
{
  size_t i;

  ಯದಿ (ns == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ;

  ಪ್ರತಿ (i = 0; i < ns->fd_len; i++)
    TEMP_FAILURE_RETRY (close (ns->fd[i]));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
configure_init_status (ಸಂರಚನಾ init_status_s *ns, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  size_t i;

  ಪ್ರತಿ (i = 0; i < MAX_NAMESPACES + 1; i++)
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

  ಪ್ರತಿ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ = libcrun_find_namespace (def->linux->namespaces[i]->type);
      ಯದಿ (UNLIKELY (ಮೌಲ್ಯ < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      ಯದಿ (ns->all_namespaces & ಮೌಲ್ಯ)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "duplicate namespace type: `%s`", def->linux->namespaces[i]->type);

      ns->all_namespaces |= ಮೌಲ್ಯ;

      ಯದಿ (def->linux->namespaces[i]->path == ರಿಕ್ತ)
        {
          libcrun_debug ("Unsharing namespace: `%s`", def->linux->namespaces[i]->type);
          ns->namespaces_to_unshare |= ಮೌಲ್ಯ;
        }
      ಅನ್ಯಥಾ
        {
          ಪೂರ್ಣಾಙ್ಕ fd;

          libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[i]->type, def->linux->namespaces[i]->path);

          ಯದಿ (ns->fd_len >= MAX_NAMESPACES)
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "too many namespaces to join");

          fd = open (def->linux->namespaces[i]->path, O_RDONLY | O_CLOEXEC);
          ಯದಿ (UNLIKELY (fd < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", def->linux->namespaces[i]->path);

          ಯದಿ (ಮೌಲ್ಯ == CLONE_NEWUSER)
            {
              ns->userns_index = ns->fd_len;
              ns->userns_index_origin = i;
            }

          ns->fd[ns->fd_len] = fd;
          ns->index[ns->fd_len] = i;
          ns->ಮೌಲ್ಯ[ns->fd_len] = ಮೌಲ್ಯ;
          ns->fd_len++;
          ns->fd[ns->fd_len] = -1;
        }
    }

  ಯದಿ (container->host_uid && (ns->all_namespaces & CLONE_NEWUSER) == 0)
    {
      libcrun_warning ("non root user need to have an 'user' namespace");
      ns->all_namespaces |= CLONE_NEWUSER;
      ns->namespaces_to_unshare |= CLONE_NEWUSER;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Detect if root is available in the container.  */
ಸ್ಥಿರ bool
ಬೇರು_mapped_in_container_p (runtime_spec_schema_defs_id_mapping **mappings, size_t len)
{
  size_t i;

  ಪ್ರತಿ (i = 0; i < len; i++)
    ಯದಿ (mappings[i]->container_id == 0)
      ಪ್ರತ್ಯಾಗಮ true;

  ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಸಂರಚನಾ libcrun_fd_map *
get_devices_fd_map (libcrun_container_t *container)
{
  ಸಂರಚನಾ libcrun_fd_map *dev_fds = get_private_data (container)->dev_fds;

  ಯದಿ (dev_fds == ರಿಕ್ತ)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t len = def->linux ? def->linux->devices_len : 0;

      dev_fds = make_libcrun_fd_map (len);
      get_private_data (container)->dev_fds = dev_fds;
    }
  ಪ್ರತ್ಯಾಗಮ dev_fds;
}

#define ಅಂಕೆ_NEEDED_DEVS (ಪರಿಮಾಣ (needed_devs) / ಪರಿಮಾಣ (needed_devs[0]) - 1)

ಸ್ಥಿರ ಸಂರಚನಾ libcrun_fd_map *
get_needed_devs_fd_map (libcrun_container_t *container)
{
  ಸಂರಚನಾ libcrun_fd_map *fds = get_private_data (container)->needed_devs_fds;

  ಯದಿ (fds == ರಿಕ್ತ)
    {
      fds = make_libcrun_fd_map (ಅಂಕೆ_NEEDED_DEVS);
      get_private_data (container)->needed_devs_fds = fds;
    }
  ಪ್ರತ್ಯಾಗಮ fds;
}

bool
is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow)
{
  bool ret = false;
  size_t i;

  ಯದಿ (recursive)
    *recursive = false;

  ಯದಿ (src_nofollow != ರಿಕ್ತ)
    *src_nofollow = false;

  ಪ್ರತಿ (i = 0; i < mnt->options_len; i++)
    {
      ಯದಿ (mnt->options[i] == ರಿಕ್ತ)
        ಅನುವರ್ತ;

      ಯದಿ (strcmp (mnt->options[i], "bind") == 0)
        {
          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          ಯದಿ (src_nofollow == ರಿಕ್ತ || *src_nofollow)
            ಭಙ್ಗ;
        }
      ಯದಿ (strcmp (mnt->options[i], "rbind") == 0)
        {
          ಯದಿ (recursive)
            *recursive = true;

          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          ಯದಿ (src_nofollow == ರಿಕ್ತ || *src_nofollow)
            ಭಙ್ಗ;
        }
      ಯದಿ (src_nofollow && strcmp (mnt->options[i], "src-nofollow") == 0)
        *src_nofollow = true;
    }
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ವರ್ಣ *
get_idmapped_option (runtime_spec_schema_defs_mount *mnt, bool *recursive)
{
  size_t i;

  ಪ್ರತಿ (i = 0; i < mnt->options_len; i++)
    {
      ಯದಿ (mnt->options[i] == ರಿಕ್ತ)
        ಅನುವರ್ತ;

      ಯದಿ (has_prefix (mnt->options[i], "idmap"))
        {
          *recursive = false;
          ಪ್ರತ್ಯಾಗಮ mnt->options[i];
        }
      ಯದಿ (has_prefix (mnt->options[i], "ridmap"))
        {
          *recursive = true;
          ಪ್ರತ್ಯಾಗಮ mnt->options[i];
        }
    }
  ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
open_mount_type (ಧ್ರುವ ವರ್ಣ *type, ಪೂರ್ಣಾಙ್ಕ *out_fd, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fsopen_fd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ newfs_fd = -1;
  ಪೂರ್ಣಾಙ್ಕ ret;

  fsopen_fd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  ಯದಿ (UNLIKELY (fsopen_fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fsopen `%s`", type);

  ret = syscall_fsconfig (fsopen_fd, FSCONFIG_CMD_CREATE, ರಿಕ್ತ, ರಿಕ್ತ, 0);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fsconfig create `%s`", type);

  newfs_fd = syscall_fsmount (fsopen_fd, FSMOUNT_CLOEXEC, 0);
  ಯದಿ (UNLIKELY (newfs_fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fsmount `%s`", type);

  *out_fd = get_and_reset (&newfs_fd);
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
maybe_get_idmapped_mount (libcrun_container_t *container, runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt, pid_t pid, ಪೂರ್ಣಾಙ್ಕ *out_fd, bool *has_mappings_out, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ newfs_fd = -1;
  cleanup_pid pid_t created_pid = -1;
  ಸಂರಚನಾ mount_attr_s attr = {
    0,
  };
  bool recursive_bind_mount = false;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
  ಧ್ರುವ ವರ್ಣ *idmap_option;
  bool recursive = false;
  ಧ್ರುವ ವರ್ಣ *options = ರಿಕ್ತ;
  bool has_mappings;
  ಪೂರ್ಣಾಙ್ಕ ret;
  ವರ್ಣ *extra_ಸಂದೇಶ = "";
  bool nofollow = false;

  *out_fd = -1;

  idmap_option = get_idmapped_option (mnt, &recursive);

  has_mappings = mnt->uid_mappings_len > 0 || mnt->gid_mappings_len > 0 || (idmap_option != ರಿಕ್ತ);
  ಯದಿ (has_mappings_out)
    *has_mappings_out = has_mappings;
  ಯದಿ (! has_mappings)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಯದಿ ((mnt->uid_mappings == ರಿಕ್ತ) != (mnt->gid_mappings == ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid mappings specified for the mount on `%s`", mnt->destination);

  /* If there are options specified, create a new user namespace with the configured mappings.  */
  ಯದಿ (idmap_option)
    {
      options = strchr (idmap_option, '=');
      ಯದಿ (options)
        {
          /* Skip the '=' itself.  */
          options++;
          ಯದಿ (options[0] == '\0')
            options = ರಿಕ್ತ;
        }
    }

  ret = maybe_create_userns_for_idmapped_mount (container, def, mnt, options, &created_pid, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;
  ಯದಿ (created_pid > 0)
    pid = created_pid;

  fd = libcrun_open_proc_pid_file (container, pid, "ns/user", O_RDONLY, err);
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ fd;

  ಯದಿ (is_bind_mount (mnt, &recursive_bind_mount, &nofollow))
    {
      newfs_fd = syscall_open_ವೃಕ್ಷ (AT_FDCWD, mnt->source, (recursive_bind_mount ? AT_RECURSIVE : 0) | AT_NO_AUTOMOUNT | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | OPEN_TREE_CLOEXEC | OPEN_TREE_CLONE);
      ಯದಿ (UNLIKELY (newfs_fd < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", mnt->source);
    }
  ಅನ್ಯಥಾ
    {
      ret = open_mount_type (mnt->type, &newfs_fd, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  attr.attr_set = MOUNT_ATTR_IDMAP;
  attr.userns_fd = fd;

  ret = syscall_mount_setattr (newfs_fd, "", AT_EMPTY_PATH | (recursive ? AT_RECURSIVE : 0), &attr);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ಯದಿ (errno == EINVAL)
        {
          extra_ಸಂದೇಶ = " (maybe the file system used doesn't support idmap mounts on this kernel?)";
        }
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "mount_setattr `%s`%s", mnt->destination, extra_ಸಂದೇಶ);
    }

  *out_fd = get_and_reset (&newfs_fd);
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ uid_t
get_id_in_user_namespace (uid_t id, bool is_uid, runtime_spec_schema_config_schema *def)
{
  runtime_spec_schema_defs_id_mapping **mappings;
  size_t len;
  size_t i;

  ಯದಿ (! def || ! def->linux)
    ಗಚ್ಛ exit;

  mappings = is_uid ? def->linux->uid_mappings : def->linux->gid_mappings;
  len = is_uid ? def->linux->uid_mappings_len : def->linux->gid_mappings_len;

  ಯದಿ (! mappings)
    ಗಚ್ಛ exit;

  ಪ್ರತಿ (i = 0; i < len; i++)
    {
      ಯದಿ (mappings[i]->container_id <= id
          && id < mappings[i]->container_id + mappings[i]->size)
        ಪ್ರತ್ಯಾಗಮ id - mappings[i]->container_id + mappings[i]->host_id;
    }
exit:
  ಪ್ರತ್ಯಾಗಮ is_uid ? get_overflow_uid () : get_overflow_gid ();
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
precreate_device (libcrun_container_t *container, ಪೂರ್ಣಾಙ್ಕ devs_dirfd, size_t i, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  runtime_spec_schema_defs_linux_device *device;
  uid_t uid;
  gid_t gid;
  ವರ್ಣ name[64];
  mode_t type;
  dev_t dev;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = snprintf (name, ಪರಿಮಾಣ (name), "%zu", i);
  ಯದಿ (UNLIKELY (ret >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (name)))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

  device = def->linux->devices[i];

  type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  dev = makedev (device->major, device->minor);

  ret = mknodat (devs_dirfd, name, device->file_mode | type, dev);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "mknod `%s`", device->path);

  uid = get_id_in_user_namespace (device->uid, true, def);
  gid = get_id_in_user_namespace (device->gid, false, def);

  ret = fchownat (devs_dirfd, name, uid, gid, 0); /* lgtm [cpp/toctou-race-condition] */
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "chown `%s`", device->path);

  ಪ್ರತ್ಯಾಗಮ get_bind_mount (devs_dirfd, name, false, false, false, MS_PRIVATE, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
send_mounts (ಪೂರ್ಣಾಙ್ಕ sync_socket_host, ಸಂರಚನಾ libcrun_fd_map *fds, size_t how_many, size_t total, libcrun_error_t *err)
{
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = TEMP_FAILURE_RETRY (ಲಿಖ (sync_socket_host, &how_many, ಪರಿಮಾಣ (how_many)));
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "write to sync socket");

  ಪ್ರತಿ (i = 0; i < total; i++)
    {
      ಯದಿ (fds->fds[i] >= 0)
        {
          ret = send_fd_to_socket_with_payload (sync_socket_host, fds->fds[i], (ವರ್ಣ *) &i, ಪರಿಮಾಣ (i), err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
prepare_and_send_mount_mounts (libcrun_container_t *container, pid_t pid, ಪೂರ್ಣಾಙ್ಕ sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map ಸಂರಚನಾ libcrun_fd_map *mount_fds = ರಿಕ್ತ;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_free ವರ್ಣ *rootfs = ರಿಕ್ತ;
  size_t how_many = 0;
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (def->mounts_len == 0)
    ಪ್ರತ್ಯಾಗಮ 0;

  mount_fds = make_libcrun_fd_map (def->mounts_len);

  ಯದಿ (def->ಬೇರು && def->ಬೇರು->path)
    rootfs = realpath (def->ಬೇರು->path, ರಿಕ್ತ);

  ಯದಿ (! has_userns)
    {
      ಪೂರ್ಣಾಙ್ಕ is_in_userns = check_running_in_user_namespace (err);
      ಯದಿ (UNLIKELY (is_in_userns < 0))
        ಪ್ರತ್ಯಾಗಮ is_in_userns;

      has_userns = is_in_userns > 0;
    }

  ಪ್ರತಿ (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false;
      bool nofollow = false;
      bool has_mappings = false;
      ಪೂರ್ಣಾಙ್ಕ mount_fd = -1;

      ret = maybe_get_idmapped_mount (container, def, def->mounts[i], pid, &mount_fd, &has_mappings, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      /* If the mount has no mappings and there is not a different user namespace, create the mount later as part of the container setup.
         Skip copy-symlink mounts: open_tree follows the symlink, losing the
         link itself; let process_single_mount handle them.  */
      ಯದಿ (mount_fd < 0 && (has_mappings || has_userns) && is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && ! mount_option_exists (def->mounts[i], "copy-symlink")
          && get_path_in_rootfs (rootfs, def->mounts[i]->source) == ರಿಕ್ತ)
        {
          unsigned long propagation = 0;

          ಯದಿ (def->linux && def->linux->rootfs_propagation)
            propagation = get_mount_flags (def->linux->rootfs_propagation, 0, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ) & ALL_PROPAGATIONS_NO_REC;
          ಯದಿ (propagation == 0)
            propagation = MS_PRIVATE;

          mount_fd = get_bind_mount (-1, def->mounts[i]->source, recursive, false, nofollow, propagation, err);
          ಯದಿ (UNLIKELY (mount_fd < 0))
            crun_error_release (err);
        }

      ಯದಿ (mount_fd >= 0)
        how_many++;

      mount_fds->fds[i] = mount_fd;
    }

  ಪ್ರತ್ಯಾಗಮ send_mounts (sync_socket_host, mount_fds, how_many, def->mounts_len, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
prepare_and_send_dev_mounts (libcrun_container_t *container, ಪೂರ್ಣಾಙ್ಕ sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map ಸಂರಚನಾ libcrun_fd_map *dev_fds = ರಿಕ್ತ;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ current_mountns = -1;
  cleanup_free ವರ್ಣ *state_dir = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *devs_path = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ devs_mountfd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ targetfd = -1;
  ಧ್ರುವ ವರ್ಣ *context_type = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *label = ರಿಕ್ತ;
  size_t how_many = 0;
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;
  // To track whether the namespace has been changed.
  bool ns_changed = false;

  ಯದಿ (def->linux == ರಿಕ್ತ || def->linux->devices_len == 0)
    ಪ್ರತ್ಯಾಗಮ 0;

  dev_fds = make_libcrun_fd_map (def->linux->devices_len);

  ಯದಿ (! has_userns || is_empty_string (container->context->id) || geteuid () > 0)
    ಪ್ರತ್ಯಾಗಮ send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_ಬೇರು : ರಿಕ್ತ),
                                     (container->context ? container->context->id : ರಿಕ್ತ), err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = append_paths (&devs_path, err, state_dir, "devs", ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = mkdir (devs_path, 0700);
  ಯದಿ (UNLIKELY (ret < 0 && errno != EEXIST))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "mkdir `%s`", devs_path);

  current_mountns = libcrun_open_proc_file (container, "self/ns/mnt", O_RDONLY, err);
  ಯದಿ (UNLIKELY (current_mountns < 0))
    ಪ್ರತ್ಯಾಗಮ current_mountns;

  ret = unshare (CLONE_NEWNS);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "unshare `CLONE_NEWNS`");

  // This indicates that the mount namespace has been altered.
  ns_changed = true;

  ret = mount (ರಿಕ್ತ, "/", ರಿಕ್ತ, MS_REC | MS_PRIVATE, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "mount `MS_REC | MS_PRIVATE`");
      ಗಚ್ಛ restore_mountns;
    }

  ಯದಿ (container->container_def->linux && container->container_def->linux->mount_label)
    {
      label = container->container_def->linux->mount_label;
      context_type = get_selinux_context_type (container, err);
      ಯದಿ (UNLIKELY (context_type == ರಿಕ್ತ))
        {
          ret = -1;
          ಗಚ್ಛ restore_mountns;
        }
    }

  devs_mountfd = fsopen_mount ("tmpfs", ರಿಕ್ತ, context_type, label, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (devs_mountfd < 0))
    {
      ret = send_mounts (sync_socket_host, dev_fds, 0, def->linux->devices_len, err);
      ಗಚ್ಛ restore_mountns;
    }

  targetfd = open (devs_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ಯದಿ (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      ಗಚ್ಛ restore_mountns;
    }

  ret = fs_move_mount_to (devs_mountfd, targetfd, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "fs_move_mount_to `%s`", devs_path);
      ಗಚ್ಛ restore_mountns;
    }

  close_and_reset (&targetfd);

  targetfd = openat (devs_mountfd, ".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  ಯದಿ (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_path);
      ಗಚ್ಛ restore_mountns;
    }

  ಪ್ರತಿ (i = 0; i < def->linux->devices_len; i++)
    {
      ret = precreate_device (container, targetfd, i, err);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          ಅನುವರ್ತ;
        }

      dev_fds->fds[i] = ret;

      ಯದಿ (dev_fds->fds[i] >= 0)
        how_many++;
    }

  ret = send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);
restore_mountns:
  ಯದಿ (ns_changed && current_mountns >= 0)
    {
      ಪೂರ್ಣಾಙ್ಕ setns_ret;
      setns_ret = setns (current_mountns, CLONE_NEWNS);
      ಯದಿ (UNLIKELY (setns_ret < 0 && ret >= 0))
        {
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "setns `CLONE_NEWNS`");
        }
    }

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
prepare_and_send_needed_dev_mounts (libcrun_container_t *container, ಪೂರ್ಣಾಙ್ಕ sync_socket_host, libcrun_error_t *err)
{
  cleanup_close_map ಸಂರಚನಾ libcrun_fd_map *fds = ರಿಕ್ತ;
  bool has_userns = (get_private_data (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  size_t how_many = 0;
  size_t i;

  fds = make_libcrun_fd_map (ಅಂಕೆ_NEEDED_DEVS);

  ಯದಿ (! has_userns || geteuid () > 0)
    ಪ್ರತ್ಯಾಗಮ send_mounts (sync_socket_host, fds, 0, ಅಂಕೆ_NEEDED_DEVS, err);

  ಪ್ರತಿ (i = 0; i < ಅಂಕೆ_NEEDED_DEVS && needed_devs[i].path; i++)
    {
      ಪೂರ್ಣಾಙ್ಕ fd = syscall_open_ವೃಕ್ಷ (AT_FDCWD, needed_devs[i].path,
                                  OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
      ಯದಿ (fd >= 0)
        {
          fds->fds[i] = fd;
          how_many++;
        }
    }

  ಪ್ರತ್ಯಾಗಮ send_mounts (sync_socket_host, fds, how_many, ಅಂಕೆ_NEEDED_DEVS, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
prepare_and_send_mounts (libcrun_container_t *container, pid_t pid, ಪೂರ್ಣಾಙ್ಕ sync_socket_host, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = expect_success_from_sync_socket (sync_socket_host, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = prepare_and_send_mount_mounts (container, pid, sync_socket_host, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = prepare_and_send_dev_mounts (container, sync_socket_host, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = prepare_and_send_needed_dev_mounts (container, sync_socket_host, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
receive_mounts (ಸಂರಚನಾ libcrun_fd_map *fds, ಪೂರ್ಣಾಙ್ಕ sync_socket_container, libcrun_error_t *err)
{
  size_t i, how_many = 0;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (fds->nfds == 0)
    ಪ್ರತ್ಯಾಗಮ 0;

  ret = TEMP_FAILURE_RETRY (ಪಠ (sync_socket_container, &how_many, ಪರಿಮಾಣ (how_many)));
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "read from sync socket");

  ಪ್ರತಿ (i = 0; i < how_many; i++)
    {
      size_t index;

      ret = receive_fd_from_socket_with_payload (sync_socket_container, (ವರ್ಣ *) &index, ಪರಿಮಾಣ (index), err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
      ಯದಿ (index >= fds->nfds)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid mount data received");

      ಯದಿ (fds->fds[index] >= 0)
        TEMP_FAILURE_RETRY (close (fds->fds[index]));

      fds->fds[index] = ret;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
set_init_non_dumpable (libcrun_container_t *container, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (! container->host_has_cap_sys_ptrace)
    ಪ್ರತ್ಯಾಗಮ 0;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
set_id_init (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  uid_t uid = 0;
  gid_t gid = 0;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (def->process && def->process->user && def->linux)
    {
      /*
        If it is running in a user namespace and root is not mapped
        use the UID/GID specified for running the container.
      */
      bool ಬೇರು_mapped = false;

      ಯದಿ (def->linux->uid_mappings_len != 0)
        {
          ಬೇರು_mapped = ಬೇರು_mapped_in_container_p (def->linux->uid_mappings, def->linux->uid_mappings_len);
          ಯದಿ (! ಬೇರು_mapped)
            uid = def->process->user->uid;

          libcrun_debug ("Using mapped UID in container: `%d`", uid);
        }

      ಯದಿ (def->linux->gid_mappings_len != 0)
        {
          ಬೇರು_mapped = ಬೇರು_mapped_in_container_p (def->linux->gid_mappings, def->linux->gid_mappings_len);
          ಯದಿ (! ಬೇರು_mapped)
            gid = def->process->user->gid;

          libcrun_debug ("Using mapped GID in container: `%d`", gid);
        }
    }

  /* -1 means "leave unchanged" for setresuid and setresgid.  */
  ಯದಿ (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresuid (uid, uid, uid);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = setresgid (gid, gid, gid);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "setresgid to `%d`", gid);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ bool
is_proc_mount (runtime_spec_schema_defs_mount *mnt)
{
  ಪ್ರತ್ಯಾಗಮ mnt->type && strcmp (mnt->type, "proc") == 0 && ! is_bind_mount (mnt, ರಿಕ್ತ, ರಿಕ್ತ);
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
open_proc_mounts_in_pidns (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಸಂರಚನಾ libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close ಪೂರ್ಣಾಙ್ಕ socket_parent = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ socket_child = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ ಬೇರು_fd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ cwd_fd = -1;
  size_t i, missing = 0;
  ಪೂರ್ಣಾಙ್ಕ saved_errno;
  ಪೂರ್ಣಾಙ್ಕ sockets[2];
  pid_t pid;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (def->ಬೇರು == ರಿಕ್ತ || def->ಬೇರು->path == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಪ್ರತಿ (i = 0; i < def->mounts_len; i++)
    ಯದಿ (is_proc_mount (def->mounts[i]))
      missing++;
  ಯದಿ (missing == 0)
    ಪ್ರತ್ಯಾಗಮ 1;

  ಬೇರು_fd = open ("/", O_DIRECTORY | O_PATH | O_CLOEXEC);
  ಯದಿ (UNLIKELY (ಬೇರು_fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `/`");

  cwd_fd = open (".", O_DIRECTORY | O_PATH | O_CLOEXEC);
  ಯದಿ (UNLIKELY (cwd_fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `.`");

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sockets);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "socketpair");
  socket_parent = sockets[0];
  socket_child = sockets[1];

  ret = chroot (def->ಬೇರು->path);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ 0;

  pid = fork ();
  saved_errno = errno;
  ಯದಿ (pid == 0)
    {
      /* If an existing user namespace was joined, the process has no
         privileges on the current mount namespace.  */
      ಯದಿ (get_private_data (container)->unshare_flags & CLONE_NEWUSER)
        (ಶೂನ್ಯ) unshare (CLONE_NEWNS);

      ಪ್ರತಿ (i = 0; i < def->mounts_len; i++)
        {
          cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;

          ಯದಿ (! is_proc_mount (def->mounts[i]))
            ಅನುವರ್ತ;

          fd = open_mount_of_type (container, def->mounts[i], err);
          ಯದಿ (fd < 0)
            _safe_exit (EXIT_FAILURE);

          ret = send_fd_to_socket_with_payload (socket_child, fd, (ವರ್ಣ *) &i, ಪರಿಮಾಣ (i), err);
          ಯದಿ (UNLIKELY (ret < 0))
            _safe_exit (EXIT_FAILURE);
        }
      _safe_exit (EXIT_SUCCESS);
    }

  ಯದಿ (UNLIKELY (fchdir (ಬೇರು_fd) < 0 || chroot (".") < 0 || fchdir (cwd_fd) < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "restore the root directory");

  ಯದಿ (UNLIKELY (pid < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, saved_errno, "fork");

  close_and_reset (&socket_child);

  /* If for any reason the mounts cannot be opened, fall back to creating
     the process now.  */
  ಯಾವತ್ (missing > 0)
    {
      libcrun_error_t tmp_err = ರಿಕ್ತ;

      ret = receive_fd_from_socket_with_payload (socket_parent, (ವರ್ಣ *) &i, ಪರಿಮಾಣ (i), &tmp_err);
      ಯದಿ (ret < 0)
        {
          crun_error_release (&tmp_err);
          ಭಙ್ಗ;
        }
      ಯದಿ (i >= mount_fds->nfds)
        {
          TEMP_FAILURE_RETRY (close (ret));
          ಭಙ್ಗ;
        }

      ಯದಿ (mount_fds->fds[i] >= 0)
        TEMP_FAILURE_RETRY (close (mount_fds->fds[i]));
      mount_fds->fds[i] = ret;
      missing--;
    }

  waitpid_ignore_stopped (pid, ರಿಕ್ತ, 0);

  ಪ್ರತ್ಯಾಗಮ missing == 0 ? 1 : 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
init_container (libcrun_container_t *container, ಪೂರ್ಣಾಙ್ಕ sync_socket_container, ಸಂರಚನಾ init_status_s *init_status,
                libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಸಂರಚನಾ libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close ಪೂರ್ಣಾಙ್ಕ timens_offsets_fd = -1;
  bool delayed_pidns_fork = false;
  pid_t pid_container = 0;
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  /* Do it before joining any namespace, otherwise the process would be
     visible in a joined PID namespace while it is still dumpable.  */
  ret = set_init_non_dumpable (container, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (init_status->idx_pidns_to_join_immediately >= 0 || init_status->idx_timens_to_join_immediately >= 0)
    {
      pid_t new_pid;

      ಯದಿ (init_status->idx_pidns_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_pidns_to_join_immediately], CLONE_NEWPID);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "setns to target PID namespace");

          close_and_reset (&init_status->fd[init_status->idx_pidns_to_join_immediately]);
        }

      ಯದಿ (init_status->idx_timens_to_join_immediately >= 0)
        {
          ret = setns (init_status->fd[init_status->idx_timens_to_join_immediately], CLONE_NEWTIME);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "setns to target timens");

          close_and_reset (&init_status->fd[init_status->idx_timens_to_join_immediately]);
        }

      ಯದಿ (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;

          delayed_pidns_fork = ret > 0;
        }

      ಯದಿ (delayed_pidns_fork)
        {
          /* A zero PID tells the parent that the process is created later.  */
          new_pid = 0;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;

          ret = TEMP_FAILURE_RETRY (ಲಿಖ (sync_socket_container, &new_pid, ಪರಿಮಾಣ (new_pid)));
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "write to sync socket");
        }
      ಅನ್ಯಥಾ
        {
          new_pid = fork ();
          ಯದಿ (UNLIKELY (new_pid < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fork");
        }

      ಯದಿ (new_pid)
        {
          /* Report the new PID to the parent and exit immediately.  */
          ret = send_success_to_sync_socket (sync_socket_container, err);
          ಯದಿ (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          ret = TEMP_FAILURE_RETRY (ಲಿಖ (sync_socket_container, &new_pid, ಪರಿಮಾಣ (new_pid)));
          ಯದಿ (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          _safe_exit (0);
        }

      /* Wait for the parent to receive the new PID.  */
      ret = expect_success_from_sync_socket (sync_socket_container, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = libcrun_set_oom (container, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (init_status->fd_len > 0)
    {
      ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, true, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  /* If the container needs to join an existing PID namespace, take a reference to it
     before creating a new user namespace, as we could lose the access to the existing
     namespace.

     This cannot be done in the parent process (by `prepare_and_send_mounts`), since it is
     necessary to first join the target namespaces and then create the mount.
  */
  ಯದಿ ((init_status->all_namespaces & CLONE_NEWUSER) && (init_status->join_pidns || init_status->join_ipcns))
    {
      ಪ್ರತಿ (i = 0; i < def->mounts_len; i++)
        {
          ಪೂರ್ಣಾಙ್ಕ fd = -1;

          /* The mount type is optional in the OCI configuration, e.g. bind mounts
             are usually specified without it.  */
          ಯದಿ (def->mounts[i]->type == ರಿಕ್ತ)
            ಅನುವರ್ತ;

          /* If for any reason the mount cannot be opened, ignore errors and continue.
             An error will be generated later if it is not possible to join the namespace.
          */
          ಯದಿ (init_status->join_pidns && ! delayed_pidns_fork && strcmp (def->mounts[i]->type, "proc") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ);
          ಯದಿ (init_status->join_ipcns && strcmp (def->mounts[i]->type, "mqueue") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->source, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ);

          ಯದಿ (fd >= 0)
            {
              ಯದಿ (mount_fds->fds[i] >= 0)
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
  ಯದಿ (def->linux->time_offsets)
    {
      timens_offsets_fd = libcrun_open_proc_file (container, "self/timens_offsets", O_WRONLY, err);
      ಯದಿ (UNLIKELY (timens_offsets_fd < 0))
        ಪ್ರತ್ಯಾಗಮ timens_offsets_fd;
    }

  ಯದಿ (init_status->all_namespaces & CLONE_NEWUSER)
    {
      ಯದಿ (init_status->delayed_userns_create)
        {
          libcrun_debug ("Unsharing user namespace");
          ret = unshare (CLONE_NEWUSER);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "unshare user namespace");

          init_status->namespaces_to_unshare &= ~CLONE_NEWUSER;

          ret = send_success_to_sync_socket (sync_socket_container, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }

      ಯದಿ (init_status->userns_index < 0)
        {
          ret = expect_success_from_sync_socket (sync_socket_container, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }
      ಅನ್ಯಥಾ
        {
          /* If we need to join another user namespace, do it immediately before creating any other namespace. */
          libcrun_debug ("Joining existing user namespace");
          ret = setns (init_status->fd[init_status->userns_index], CLONE_NEWUSER);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "setns to target user namespace `%s`",
                                    def->linux->namespaces[init_status->userns_index_origin]->path);
        }

      ret = set_id_init (container, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      /* Changing the credentials resets the dumpable flag to the value of
         fs.suid_dumpable, so set it again.  */
      ret = set_init_non_dumpable (container, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = join_namespaces (def, init_status->fd, init_status->fd_len, init_status->index, false, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS))
    {
      /* New namespaces to create for the container.  */
      ret = unshare (init_status->namespaces_to_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS));
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "unshare");
    }

  ಯದಿ (def->linux->time_offsets)
    {
      ವರ್ಣ fmt_buffer[128];
      ಪೂರ್ಣಾಙ್ಕ fd = timens_offsets_fd;

      ಯದಿ (def->linux->time_offsets->boottime)
        {
          ret = snprintf (fmt_buffer, ಪರಿಮಾಣ (fmt_buffer), "boottime %" PRIi64 " %" PRIu32, def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          ಯದಿ (UNLIKELY (ret >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buffer)))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

          libcrun_debug ("Using boot time offset: secs = `%lld`, nanosecs = `%d`", (long long ಪೂರ್ಣಾಙ್ಕ) def->linux->time_offsets->boottime->secs, def->linux->time_offsets->boottime->nanosecs);
          ret = ಲಿಖ (fd, fmt_buffer, strlen (fmt_buffer));
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
      ಯದಿ (def->linux->time_offsets->monotonic)
        {
          libcrun_debug ("Using monotonic time offset: secs = `%lld`, nanosecs = `%d`", (long long ಪೂರ್ಣಾಙ್ಕ) def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);

          ret = snprintf (fmt_buffer, ಪರಿಮಾಣ (fmt_buffer), "monotonic %" PRIi64 " %" PRIu32, def->linux->time_offsets->monotonic->secs, def->linux->time_offsets->monotonic->nanosecs);
          ಯದಿ (UNLIKELY (ret >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (fmt_buffer)))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

          ret = ಲಿಖ (fd, fmt_buffer, strlen (fmt_buffer));
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
    }

  ಯದಿ (init_status->must_fork)
    {
      ಯದಿ (init_status->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_in_pidns (container, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;

          delayed_pidns_fork = ret > 0;
        }

      ಯದಿ (! delayed_pidns_fork)
        {
          /* A PID and a time namespace are joined when the new process is created.  */
          pid_container = fork ();
          ಯದಿ (UNLIKELY (pid_container < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "cannot fork");
        }

      /* Report back the new PID.  A zero PID tells the parent that the process is created later.  */
      ಯದಿ (pid_container || delayed_pidns_fork)
        {
          libcrun_debug ("Running container PID after fork: `%d`", pid_container);
          ret = send_success_to_sync_socket (sync_socket_container, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;

          ret = TEMP_FAILURE_RETRY (ಲಿಖ (sync_socket_container, &pid_container, ಪರಿಮಾಣ (pid_container)));
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "write to sync socket");

          ಯದಿ (pid_container)
            _safe_exit (EXIT_SUCCESS);
        }

      ret = expect_success_from_sync_socket (sync_socket_container, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  get_private_data (container)->delayed_pidns_fork = delayed_pidns_fork;

  ret = send_success_to_sync_socket (sync_socket_container, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* Receive the mounts sent by `prepare_and_send_mounts`.  */
  ret = receive_mounts (get_fd_map (container), sync_socket_container, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = receive_mounts (get_devices_fd_map (container), sync_socket_container, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = receive_mounts (get_needed_devs_fd_map (container), sync_socket_container, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_container_setgroups (container, container->container_def->process, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ 0;
}

bool
libcrun_container_delayed_pidns_fork (libcrun_container_t *container)
{
  ಪ್ರತ್ಯಾಗಮ get_private_data (container)->delayed_pidns_fork;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ client_fd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ pidfd = -1;
  ಧ್ರುವ ವರ್ಣ *v;

  v = find_annotation (container, "run.oci.pidfd_receiver");
  ಯದಿ (v == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  pidfd = syscall_pidfd_open (pid, 0);
  ಯದಿ (UNLIKELY (pidfd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "pidfd_open");

  client_fd = open_unix_domain_client_socket (v, 0, err);
  ಯದಿ (UNLIKELY (client_fd < 0))
    ಪ್ರತ್ಯಾಗಮ client_fd;

  ಪ್ರತ್ಯಾಗಮ send_fd_to_socket (client_fd, pidfd, err);
}

ಸ್ಥಿರ bool
has_exec_cpu_affinity (runtime_spec_schema_config_schema_process *process)
{
  ಯದಿ (process == ರಿಕ್ತ || process->exec_cpu_affinity == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ false;
  ಪ್ರತ್ಯಾಗಮ (! is_empty_string (process->exec_cpu_affinity->initial))
         || (! is_empty_string (process->exec_cpu_affinity->final));
}

pid_t
libcrun_run_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, ಶೂನ್ಯ *args,
                             ಪೂರ್ಣಾಙ್ಕ *sync_socket_out, ಸಂರಚನಾ libcrun_dirfd_s *cgroup_dirfd, libcrun_error_t *err)
{
  __attribute__ ((cleanup (cleanup_free_init_statusp))) ಸಂರಚನಾ init_status_s init_status;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ sync_socket_container = -1;
  ವರ್ಣ *notify_socket_env = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ sync_socket_host = -1;
  __attribute__ ((unused)) cleanup_close ಪೂರ್ಣಾಙ್ಕ restore_pidns = -1;
  ಪೂರ್ಣಾಙ್ಕ first_clone_args = 0;
  ಪೂರ್ಣಾಙ್ಕ sync_socket[2];
  pid_t pid;
  bool new_userns;
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  libcrun_debug ("Running linux container");
  ret = configure_init_status (&init_status, container, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  get_private_data (container)->unshare_flags = init_status.all_namespaces;
  get_private_data (container)->joined_mount_ns
      = (init_status.all_namespaces & CLONE_NEWNS)
        && ! (init_status.namespaces_to_unshare & CLONE_NEWNS);
  /* cgroup will be unshared later.  Once the process is in the correct cgroup.  */
  init_status.all_namespaces &= ~CLONE_NEWCGROUP;
  get_private_data (container)->unshare_cgroupns = init_status.namespaces_to_unshare & CLONE_NEWCGROUP;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "socketpair");

  sync_socket_host = sync_socket[0];
  sync_socket_container = sync_socket[1];

#ifdef HAVE_SYSTEMD
  ಯದಿ (def->ಬೇರು)
    {
      ret = do_notify_socket (container, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }
#endif

  get_uid_gid_from_def (container->container_def, &container->container_uid, &container->container_gid);

  /* This must be done before we enter a user namespace.  */
  ಯದಿ (def->process)
    {
      ret = libcrun_set_rlimits (def->process->rlimits, def->process->rlimits_len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = libcrun_set_oom (container, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_container_notify_handler (args, HANDLER_CONFIGURE_BEFORE_USERNS, container,
                                          container->container_def->ಬೇರು ? container->container_def->ಬೇರು->path : ರಿಕ್ತ, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* If a new user namespace must be created, but there are other namespaces to join, then delay
     the userns creation after the namespaces are joined.  */
  init_status.delayed_userns_create
      = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0 && init_status.fd_len > 0;

  /* Check if special handling is required to join the namespaces.  The PID and time
     namespaces are to be joined before creating a new user namespace (as the process
     loses the privileges to do so after that), but after joining an existing one (as
     the process might not have the privileges to do so before that).  */
  new_userns = (init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0;
  ಪ್ರತಿ (i = 0; i < init_status.fd_len; i++)
    {
      ವಿಕಲ್ಪ (init_status.ಮೌಲ್ಯ[i])
        {
        ಪಕ್ಷ CLONE_NEWIPC:
          ಯದಿ (init_status.all_namespaces & CLONE_NEWUSER)
            init_status.join_ipcns = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ CLONE_NEWPID:
          /* The prestart, createRuntime and createContainer hooks need the PID of the
             process in the PID namespace before the pivot_root.  */
          init_status.can_delay_pidns_fork = ! (def->hooks
                                                && (def->hooks->prestart_len || def->hooks->create_runtime_len
                                                    || def->hooks->create_container_len));
          ಯದಿ (! new_userns)
            init_status.must_fork = true;
          ಅನ್ಯಥಾ
            {
              init_status.join_pidns = true;
              init_status.idx_pidns_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWPID;
            }
          ಭಙ್ಗ;

        ಪಕ್ಷ CLONE_NEWTIME:
          ಯದಿ (! new_userns)
            init_status.must_fork = true;
          ಅನ್ಯಥಾ
            {
              init_status.idx_timens_to_join_immediately = i;
              init_status.namespaces_to_unshare &= ~CLONE_NEWTIME;
            }
          ಭಙ್ಗ;
        }
    }

  /* Before attempting any setns() or unshare(), a clone() is required to not touch the caller context
     that can be used later on for running hooks.  */

  ಯದಿ ((init_status.namespaces_to_unshare & CLONE_NEWUSER) && init_status.fd_len == 0)
    {
      /* If a user namespace must be created and there are no other namespaces to join, create the userns alone.  */
      first_clone_args = CLONE_NEWUSER;
    }
  ಅನ್ಯಥಾ ಯದಿ ((init_status.all_namespaces & CLONE_NEWUSER) == 0)
    {
      /* If it doesn't create a user namespace or need to join one, create the new requested namespaces now. */
      first_clone_args = init_status.namespaces_to_unshare & ~(CLONE_NEWTIME | CLONE_NEWCGROUP | CLONE_NEWNS);
    }

  init_status.namespaces_to_unshare &= ~first_clone_args;

  /* Check if there are still namespaces that require a fork().  */
  ಯದಿ (init_status.namespaces_to_unshare & (CLONE_NEWPID | CLONE_NEWTIME))
    {
      /* If we need to make another fork(), make sure the NEWPID is always
         created as part of that.  */
      first_clone_args &= ~CLONE_NEWPID;
      init_status.namespaces_to_unshare |= CLONE_NEWPID;
      init_status.must_fork = true;
      init_status.can_delay_pidns_fork = false;
    }

  pid = -1;
  ಯದಿ (cgroup_dirfd && *cgroup_dirfd->dirfd >= 0)
    {
      ಸಂರಚನಾ _clone3_args clone3_args;
      memset (&clone3_args, 0, ಪರಿಮಾಣ (clone3_args));
      clone3_args.exit_signal = SIGCHLD;
      clone3_args.flags = first_clone_args;

      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = *cgroup_dirfd->dirfd;

      pid = syscall_clone3 (&clone3_args);
      ಯದಿ (pid >= 0)
        cgroup_dirfd->joined = true;

      close_and_reset (cgroup_dirfd->dirfd);
    }
  /* fallback to clone() for any error.  */
  ಯದಿ (pid < 0)
    {
      pid = syscall_clone (first_clone_args | SIGCHLD, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (pid < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "clone");
    }

  ಯದಿ (pid)
    {
      __attribute__ ((unused)) cleanup_pid pid_t pid_to_clean = pid;

      /* this is safe to do because the std stream files were not changed since the clone().  */
      ret = libcrun_save_external_descriptors (container, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = close_and_reset (&sync_socket_container);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "close");

      /* any systemd notify socket open_tree FD is pointless to keep around in the parent */
      close_and_reset (&(get_private_data (container)->notify_socket_ವೃಕ್ಷ_fd));

      ಯದಿ (init_status.idx_pidns_to_join_immediately >= 0 || init_status.idx_timens_to_join_immediately >= 0)
        {
          pid_t new_pid = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;

          ret = TEMP_FAILURE_RETRY (ಪಠ (sync_socket_host, &new_pid, ಪರಿಮಾಣ (new_pid)));
          ಯದಿ (UNLIKELY (ret != ಪರಿಮಾಣ (new_pid)))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ret < 0 ? errno : 0, "read pid from sync socket");

          ಯದಿ (new_pid == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          ಅನ್ಯಥಾ
            {
              /* Cleanup the first process.  */
              ret = waitpid_ignore_stopped (pid, ರಿಕ್ತ, 0);

              pid_to_clean = pid = new_pid;
            }

          ret = send_success_to_sync_socket (sync_socket_host, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }

      ಯದಿ (init_status.delayed_userns_create)
        {
          ret = expect_success_from_sync_socket (sync_socket_host, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }

      ಯದಿ ((init_status.all_namespaces & CLONE_NEWUSER) && init_status.userns_index < 0)
        {
          ret = libcrun_set_usernamespace (container, pid, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;

          ret = send_success_to_sync_socket (sync_socket_host, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }

      ಯದಿ (init_status.must_fork)
        {
          pid_t grandchild = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;

          ret = TEMP_FAILURE_RETRY (ಪಠ (sync_socket_host, &grandchild, ಪರಿಮಾಣ (grandchild)));
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "read pid from sync socket");

          ret = send_success_to_sync_socket (sync_socket_host, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;

          ಯದಿ (grandchild == 0)
            get_private_data (container)->delayed_pidns_fork = true;
          ಅನ್ಯಥಾ
            {
              /* Cleanup the first process.  */
              waitpid_ignore_stopped (pid, ರಿಕ್ತ, 0);

              pid_to_clean = pid = grandchild;
            }
        }

      /* They are received by `receive_mounts`.  */
      ret = prepare_and_send_mounts (container, pid, sync_socket_host, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = expect_success_from_sync_socket (sync_socket_host, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      *sync_socket_out = get_and_reset (&sync_socket_host);

      /* If the fork is delayed, it is done once the container process exists.  */
      ಯದಿ (! get_private_data (container)->delayed_pidns_fork)
        {
          ret = libcrun_handle_pidfd_receiver (pid, container, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }

      pid_to_clean = 0;
      ಪ್ರತ್ಯಾಗಮ pid;
    }

  /* Inside the container process.  */

  ret = close_and_reset (&sync_socket_host);
  ಯದಿ (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "%s", "close sync socket");

  /* Initialize the new process and make sure to join/create all the required namespaces.  */
  ret = init_container (container, sync_socket_container, &init_status, err);
  ಯದಿ (UNLIKELY (ret < 0))
    send_error_to_sync_socket_and_die (sync_socket_container, false, err);
  ಅನ್ಯಥಾ
    {
      ret = send_success_to_sync_socket (sync_socket_container, err);
      ಯದಿ (UNLIKELY (ret < 0))
        libcrun_fail_with_error (crun_error_get_errno (err), "%s", (*err)->ಸಂದೇಶ);
    }

  /* Jump into the specified entrypoint.  */
  ಯದಿ (container->context->notify_socket)
    xasprintf (&notify_socket_env, "NOTIFY_SOCKET=%s/notify", container->context->notify_socket);

  ret = entrypoint (args, notify_socket_env, sync_socket_container, err);

  /* For most of the cases ENTRYPOINT returns only on an error, fallback here */
  /* Except for custom handlers which could perform a task and return with success */
  /* since custom handlers could or could not be {long-running, blocking} */
  ಯದಿ (*err)
    libcrun_fail_with_error ((*err)->status, "%s", (*err)->ಸಂದೇಶ);

  /* If we got here likely we returned from a custom handler eg. wasm, libkrun */
  /* Allow cleanup attributes to perform cleanup and exit with success if return code was 0 */
  ಯದಿ (ret == 0)
    _safe_exit (EXIT_SUCCESS);

  _safe_exit (EXIT_FAILURE);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
join_process_parent_helper (libcrun_context_t *context,
                            libcrun_container_t *container,
                            runtime_spec_schema_config_schema_process *process,
                            pid_t child_pid, ಪೂರ್ಣಾಙ್ಕ sync_socket_fd,
                            libcrun_container_status_t *status,
                            bool need_move_to_cgroup, ಧ್ರುವ ವರ್ಣ *sub_cgroup,
                            ಪೂರ್ಣಾಙ್ಕ *terminal_fd, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  ವರ್ಣ res;
  pid_t pid;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ sync_fd = sync_socket_fd;

  ಯದಿ (terminal_fd)
    *terminal_fd = -1;

  /* Read the status and the PID from the child process.  */
  ret = TEMP_FAILURE_RETRY (ಪಠ (sync_fd, &res, 1));
  ಯದಿ (UNLIKELY (ret != 1))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  ಯದಿ (res != '0')
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "fail startup");

  ret = TEMP_FAILURE_RETRY (ಪಠ (sync_fd, &pid, ಪರಿಮಾಣ (pid)));
  ಯದಿ (UNLIKELY (ret != ಪರಿಮಾಣ (pid)))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  /* Wait for the child pid so we ensure the grandchild gets properly reparented.  */
  ret = waitpid_ignore_stopped (child_pid, ರಿಕ್ತ, 0);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "waitpid for exec child pid");

  ಯದಿ (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->initial, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (! has_exec_cpu_affinity (process))
    {
      ret = libcrun_reset_cpu_affinity_mask (pid, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (need_move_to_cgroup)
    {
      cleanup_cgroup_status ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status = libcrun_cgroup_make_status (status);
      cleanup_free ವರ್ಣ *final_cgroup = ರಿಕ್ತ;

      ಯದಿ (sub_cgroup)
        {
          ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, ರಿಕ್ತ);
          ಯದಿ (UNLIKELY (ret < 0))
            ಪ್ರತ್ಯಾಗಮ ret;
        }

      ret = libcrun_cgroup_join_process (cgroup_status, final_cgroup ?: status->cgroup_path, pid, status->pid, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  /* Set the scheduler for the process itself (it might have been set for its
     parent, but it is not inherited with SCHED_FLAG_RESET_ON_FORK, and
     SCHED_DEADLINE cannot be set there as it prevents forking).  */
  ret = libcrun_set_scheduler (pid, process, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (process && process->exec_cpu_affinity)
    {
      ret = libcrun_set_cpu_affinity_from_string (pid, process->exec_cpu_affinity->final, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_MOVE, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = libcrun_set_io_priority (pid, process, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* The write unblocks the grandchild process so it can run once we setup
     the cgroups.  */
  ret = TEMP_FAILURE_RETRY (ಲಿಖ (sync_fd, &ret, ಪರಿಮಾಣ (ret)));
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "write to sync socket");

  ಯದಿ (terminal_fd)
    {
      ret = receive_fd_from_socket (sync_fd, err);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          ಪೂರ್ಣಾಙ್ಕ err_code = 0;
          cleanup_free ವರ್ಣ *err_str = ರಿಕ್ತ;

          ಯದಿ (read_error_from_sync_socket (sync_fd, &err_code, &err_str))
            {
              ಯದಿ (! is_empty_string (err_str))
                {
                  crun_error_release (err);
                  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, err_code, "%s", err_str);
                }
            }

          ಪ್ರತ್ಯಾಗಮ crun_error_wrap (err, "receive terminal fd");
        }
      *terminal_fd = ret;
    }

  ಪ್ರತ್ಯಾಗಮ pid;
}

/*
  try to join all the namespaces with a single call to setns using the target process pidfd.

  return codes:
  < 0 - on errors
  0   - the namespaces were not joined.
  > 0 - the namespaces were joined.
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
try_setns_with_pidfd (pid_t pid_to_join, libcrun_container_t *container, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ pidfd_pid_to_join = -1;
  ಪೂರ್ಣಾಙ್ಕ all_flags = 0;
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  /* If there is any explicit namespace path to join, skip the setns_with_pidfd
     shortcut and join each namespace individually.  */
  ಯದಿ (def->linux && def->linux->namespaces)
    {
      ಪ್ರತಿ (i = 0; i < def->linux->namespaces_len; i++)
        ಯದಿ (! is_empty_string (def->linux->namespaces[i]->path))
          ಪ್ರತ್ಯಾಗಮ 0;
    }

  pidfd_pid_to_join = syscall_pidfd_open (pid_to_join, 0);
  ಯದಿ (UNLIKELY (pidfd_pid_to_join < 0))
    ಪ್ರತ್ಯಾಗಮ 0;

  /* Validate that the pidfd really refers to the original container process.  */
  ret = libcrun_check_pid_valid (status, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;
  ಯದಿ (ret == 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ESRCH, "container process not found, the pid was reused");

  ಪ್ರತಿ (i = 0; namespaces[i].ns_file; i++)
    all_flags |= namespaces[i].ಮೌಲ್ಯ;

  ret = setns (pidfd_pid_to_join, all_flags);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ 0;

  ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
join_process_namespaces (libcrun_container_t *container, pid_t pid_to_join, libcrun_container_status_t *status, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಪೂರ್ಣಾಙ್ಕ fds_joined[MAX_NAMESPACES] = {
    0,
  };
  ಪೂರ್ಣಾಙ್ಕ fds[MAX_NAMESPACES] = {
    -1,
  };
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  /* Try to join all namespaces in one shot with setns and pidfd.  */
  ret = try_setns_with_pidfd (pid_to_join, container, status, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;
  /* Nothing left to do if the namespaces were joined.  */
  ಯದಿ (LIKELY (ret > 0))
    ಪ್ರತ್ಯಾಗಮ 0;

  /* If setns fails with the target pidfd, fall-back to join each namespace individually.  */

  ಯದಿ (def->linux->namespaces_len >= MAX_NAMESPACES)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid configuration");

  ಪ್ರತಿ (i = 0; namespaces[i].ns_file; i++)
    {
      cleanup_free ವರ್ಣ *ns_path = ರಿಕ್ತ;

      xasprintf (&ns_path, "%d/ns/%s", pid_to_join, namespaces[i].ns_file);
      fds[i] = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
      ಯದಿ (UNLIKELY (fds[i] < 0))
        {
          /* If the namespace doesn't exist, just ignore it.  */
          ಯದಿ (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              ಅನುವರ್ತ;
            }

          ret = fds[i];
          ಗಚ್ಛ exit;
        }
    }

  /* The namespace files were opened by PID.  Validate that the PID still refers
     to the original container process before joining any of them.  */
  ret = libcrun_check_pid_valid (status, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಗಚ್ಛ exit;
  ಯದಿ (ret == 0)
    {
      ret = crun_make_error (err, ESRCH, "container process not found, the pid was reused");
      ಗಚ್ಛ exit;
    }

  ಪ್ರತಿ (i = 0; namespaces[i].ns_file; i++)
    {
      ಯದಿ (namespaces[i].ಮೌಲ್ಯ == CLONE_NEWUSER)
        ಅನುವರ್ತ;

      ret = setns (fds[i], 0);
      ಯದಿ (ret == 0)
        fds_joined[i] = 1;
    }
  ಪ್ರತಿ (i = 0; namespaces[i].ns_file; i++)
    {
      ಯದಿ (fds_joined[i])
        ಅನುವರ್ತ;
      ret = setns (fds[i], 0);
      ಯದಿ (ret == 0)
        fds_joined[i] = 1;
    }
  ಪ್ರತಿ (i = 0; namespaces[i].ns_file; i++)
    {
      ಯದಿ (fds_joined[i])
        ಅನುವರ್ತ;
      ret = setns (fds[i], 0);
      ಯದಿ (UNLIKELY (ret < 0 && errno != EINVAL))
        {
          size_t j;
          bool found = false;

          ಪ್ರತಿ (j = 0; j < def->linux->namespaces_len; j++)
            {
              ಯದಿ (strcmp (namespaces[i].name, def->linux->namespaces[j]->type) == 0)
                {
                  found = true;
                  ಭಙ್ಗ;
                }
            }
          ಯದಿ (! found)
            {
              /* It was not requested to create this ns, so just ignore it.  */
              fds_joined[i] = 1;
              ಅನುವರ್ತ;
            }

          ret = crun_make_error (err, errno, "setns `%s`", namespaces[i].ns_file);
          ಗಚ್ಛ exit;
        }
      fds_joined[i] = 1;
    }

  ret = 0;

exit:
  ಪ್ರತಿ (i = 0; namespaces[i].ns_file; i++)
    close_and_reset (&fds[i]);

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_join_process (libcrun_context_t *context,
                      libcrun_container_t *container,
                      pid_t pid_to_join,
                      libcrun_container_status_t *status,
                      ಧ್ರುವ ವರ್ಣ *sub_cgroup,
                      ಪೂರ್ಣಾಙ್ಕ detach,
                      runtime_spec_schema_config_schema_process *process,
                      ಪೂರ್ಣಾಙ್ಕ *terminal_fd,
                      libcrun_error_t *err)
{
  pid_t pid;
  ಪೂರ್ಣಾಙ್ಕ ret;
  ಪೂರ್ಣಾಙ್ಕ sync_socket_fd[2];
  cleanup_close ಪೂರ್ಣಾಙ್ಕ cgroup_dirfd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ sync_fd = -1;
  ಸಂರಚನಾ _clone3_args clone3_args;
  bool need_move_to_cgroup;

  ಯದಿ (! detach)
    {
      ret = prctl (PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "set child subreaper");
    }

  ಯದಿ (path_has_dot_dot_component (sub_cgroup))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid cgroup path `%s`: `..` components are not allowed", sub_cgroup);

  /* The caller checked only the container cgroup, but a sub-cgroup is managed
     by the container and the process would never run if it is frozen.  */
  ಯದಿ (sub_cgroup && ! is_empty_string (status->cgroup_path))
    {
      cleanup_free ವರ್ಣ *final_cgroup = ರಿಕ್ತ;

      ret = append_paths (&final_cgroup, err, status->cgroup_path, sub_cgroup, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = libcrun_cgroup_ensure_not_frozen (final_cgroup, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "error creating socketpair");

  {
    cleanup_cgroup_status ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status = ರಿಕ್ತ;

    cgroup_status = libcrun_cgroup_make_status (status);

    /* The cgroup can be joined directly only when there are no additional
       controllers not handled by cgroup v2.  */
    ಯದಿ (get_force_cgroup_v1_annotation (container) == ರಿಕ್ತ)
      {
        cgroup_dirfd = libcrun_get_cgroup_dirfd (cgroup_status, sub_cgroup, err);
        ಯದಿ (UNLIKELY (cgroup_dirfd < 0))
          crun_error_release (err);
      }
  }

  memset (&clone3_args, 0, ಪರಿಮಾಣ (clone3_args));
  clone3_args.exit_signal = SIGCHLD;

  /* Do not join the cgroup immediately if an initial CPU affinity mask is specified, so that
     the process can set the cpu affinity before joining the target cgroup.  */
  ಯದಿ (cgroup_dirfd < 0 || (process->exec_cpu_affinity && process->exec_cpu_affinity->initial))
    need_move_to_cgroup = true;
  ಅನ್ಯಥಾ
    {
      need_move_to_cgroup = false;
      clone3_args.flags |= CLONE_INTO_CGROUP;
      clone3_args.cgroup = cgroup_dirfd;
    }

  pid = syscall_clone3 (&clone3_args);

  ಯದಿ (pid > 0 && ! (process->scheduler && process->scheduler->policy && strcmp (process->scheduler->policy, "SCHED_DEADLINE") == 0))
    {
      /* We need to set the scheduler as soon as possible after joining the cgroup,
         because if it is a RT scheduler, other processes in the container could already
         take the entire cpu time and stall the new process.  This is not done for
         SCHED_DEADLINE, which does not allow the process to fork.  */
      ret = libcrun_set_scheduler (pid, process, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  /* On errors, fall back to fork().  */
  ಯದಿ (pid < 0)
    {
      need_move_to_cgroup = true;

      pid = fork ();
      ಯದಿ (UNLIKELY (pid < 0))
        {
          ret = crun_make_error (err, errno, "fork");
          ಗಚ್ಛ exit;
        }
    }

  ಯದಿ (pid)
    {
      close_and_reset (&sync_socket_fd[1]);
      sync_fd = sync_socket_fd[0];
      ಪ್ರತ್ಯಾಗಮ join_process_parent_helper (context, container,
                                         process, pid, sync_fd,
                                         status, need_move_to_cgroup,
                                         sub_cgroup, terminal_fd, err);
    }

  close_and_reset (&sync_socket_fd[0]);
  sync_fd = sync_socket_fd[1];

  /* Not needed anymore, do not keep a reference to the host file system.  */
  close_and_reset (&cgroup_dirfd);

  ret = join_process_namespaces (container, pid_to_join, status, err);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      TEMP_FAILURE_RETRY (ಲಿಖ (sync_fd, "1", 1));
      libcrun_fail_with_error ((*err)->status, "%s", (*err)->ಸಂದೇಶ);
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  ಯದಿ (UNLIKELY (pid < 0))
    {
      ಪೂರ್ಣಾಙ್ಕ saved_errno = errno;
      TEMP_FAILURE_RETRY (ಲಿಖ (sync_fd, "1", 1));
      libcrun_fail_with_error (saved_errno, "fork");
    }

  ಯದಿ (pid)
    {
      /* Just return the PID to the parent helper and exit.  */
      ret = TEMP_FAILURE_RETRY (ಲಿಖ (sync_fd, "0", 1));
      ಯದಿ (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      ret = TEMP_FAILURE_RETRY (ಲಿಖ (sync_fd, &pid, ಪರಿಮಾಣ (pid)));
      ಯದಿ (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          _safe_exit (EXIT_FAILURE);
        }

      _safe_exit (EXIT_SUCCESS);
    }
  ಅನ್ಯಥಾ
    {
      /* Inside the grandchild process.  The real process
         used for the container.  */
      cleanup_free ವರ್ಣ *pty = ರಿಕ್ತ;
      ಪೂರ್ಣಾಙ್ಕ r = -1;

      /* Wait for the parent to complete the setup (cgroup, scheduler, ...).
         An EOF means the parent failed, so do not go any further.  */
      ret = TEMP_FAILURE_RETRY (ಪಠ (sync_fd, &r, ಪರಿಮಾಣ (r)));
      ಯದಿ (UNLIKELY (ret != ಪರಿಮಾಣ (r) || r < 0))
        _safe_exit (EXIT_FAILURE);

      ret = setsid ();
      ಯದಿ (ret < 0)
        {
          crun_make_error (err, errno, "setsid");
          send_error_to_sync_socket_and_die (sync_fd, true, err);
        }

      ಯದಿ (terminal_fd)
        {
          cleanup_close ಪೂರ್ಣಾಙ್ಕ ptmx_fd = -1;

          ret = set_id_init (container, err);
          ಯದಿ (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ptmx_fd = open_terminal (&pty, process, err);
          ಯದಿ (UNLIKELY (ptmx_fd < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);

          ret = send_fd_to_socket (sync_fd, ptmx_fd, err);
          ಯದಿ (UNLIKELY (ret < 0))
            send_error_to_sync_socket_and_die (sync_fd, true, err);
        }
    }

  ಪ್ರತ್ಯಾಗಮ 0;

exit:
  ಯದಿ (sync_socket_fd[0] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[0]));
  ಯದಿ (sync_socket_fd[1] >= 0)
    TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_linux_container_update (libcrun_container_status_t *status, ಧ್ರುವ ವರ್ಣ *state_ಬೇರು, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_cgroup_status ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status = ರಿಕ್ತ;

  cgroup_status = libcrun_cgroup_make_status (status);

  ಪ್ರತ್ಯಾಗಮ libcrun_update_cgroup_resources (cgroup_status, state_ಬೇರು, resources, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libcrun_container_pause_unpause_linux (libcrun_container_status_t *status, ಧ್ರುವ bool pause, libcrun_error_t *err)
{
  cleanup_cgroup_status ಸಂರಚನಾ libcrun_cgroup_status *cgroup_status = ರಿಕ್ತ;

  cgroup_status = libcrun_cgroup_make_status (status);

  ಪ್ರತ್ಯಾಗಮ libcrun_cgroup_pause_unpause (cgroup_status, pause, err);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_pause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ libcrun_container_pause_unpause_linux (status, true, err);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_unpause_linux (libcrun_container_status_t *status, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ libcrun_container_pause_unpause_linux (status, false, err);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_set_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err)
{
  unsigned long persona = 0;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (strcmp (p->domain, "LINUX") == 0)
    persona = PER_LINUX;
  ಅನ್ಯಥಾ ಯದಿ (strcmp (p->domain, "LINUX32") == 0)
    persona = PER_LINUX32;
  ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "unknown persona specified `%s`", p->domain);

  ret = personality (persona);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "personality to `%s`", p->domain);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_configure_network (libcrun_container_t *container, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  size_t i;
  bool configure_network = false;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ sockfd = -1;

  ಪ್ರತಿ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ = libcrun_find_namespace (def->linux->namespaces[i]->type);
      ಯದಿ (UNLIKELY (ಮೌಲ್ಯ < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      ಯದಿ (ಮೌಲ್ಯ == CLONE_NEWNET && def->linux->namespaces[i]->path == ರಿಕ್ತ)
        {
          configure_network = true;
          ಭಙ್ಗ;
        }
    }

  ಯದಿ (! configure_network)
    ಪ್ರತ್ಯಾಗಮ 0;

  sockfd = socket (AF_INET, SOCK_DGRAM, 0);
  ಯದಿ (LIKELY (sockfd >= 0))
    {
      ಸಂರಚನಾ ifreq ifr_lo = { .ifr_name = "lo", .ifr_flags = IFF_UP | IFF_RUNNING };

      ret = ioctl (sockfd, SIOCSIFFLAGS, &ifr_lo);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "ioctl SIOCSIFFLAGS");
    }
  ಅನ್ಯಥಾ
    {
      ಸಂರಚನಾ nlmsghdr *hdr_recv;
      ವರ್ಣ buf[ಪರಿಮಾಣ (ಸಂರಚನಾ nlmsghdr) + ಪರಿಮಾಣ (ಸಂರಚನಾ nlmsgerr)];
      ಸಂರಚನಾ sockaddr_nl addr = {
        .nl_family = AF_NETLINK,
        .nl_pid = getpid (),
      };
      ಸಂರಚನಾ sockaddr_nl addr_to = {
        .nl_family = AF_NETLINK,
        .nl_pid = 0,
      };
      ಸಂರಚನಾ nlmsghdr hdr = {
        .nlmsg_len = ಪರಿಮಾಣ (ಸಂರಚನಾ nlmsghdr) + ಪರಿಮಾಣ (ಸಂರಚನಾ ifinfomsg),
        .nlmsg_type = RTM_NEWLINK,
        .nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK,
        .nlmsg_seq = 1,
        .nlmsg_pid = 0,
      };
      ಸಂರಚನಾ ifinfomsg ಸಂದೇಶ = {
        .ifi_family = AF_UNSPEC,
        .ifi_type = 0,
        .ifi_index = 1,
        .ifi_flags = IFF_UP,
        .ifi_change = IFF_UP,
      };

      *((ಸಂರಚನಾ nlmsghdr *) buf) = hdr;
      *((ಸಂರಚನಾ ifinfomsg *) (buf + ಪರಿಮಾಣ (ಸಂರಚನಾ nlmsghdr))) = ಸಂದೇಶ;

      sockfd = socket (PF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);
      ಯದಿ (UNLIKELY (sockfd < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "socket PF_NETLINK");

      ret = bind (sockfd, (ಸಂರಚನಾ sockaddr *) &addr, ಪರಿಮಾಣ (addr));
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "bind PF_NETLINK");

      ret = sendto (sockfd, buf, ಪರಿಮಾಣ (buf), 0, (ಸಂರಚನಾ sockaddr *) &addr_to, ಪರಿಮಾಣ (addr_to));
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "sendto PF_NETLINK");

      ret = recvfrom (sockfd, buf, ಪರಿಮಾಣ (buf), 0, ರಿಕ್ತ, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "recvfrom PF_NETLINK");

      hdr_recv = (ಸಂರಚನಾ nlmsghdr *) buf;

      ಯದಿ (hdr_recv->nlmsg_type == NLMSG_ERROR)
        {
          ಸಂರಚನಾ nlmsgerr *err_data = (ಸಂರಚನಾ nlmsgerr *) NLMSG_DATA (hdr_recv);
          /* err_data->error set to 0 means success acknowledgement due to NLM_F_ACK  */
          ಯದಿ (err_data->error < 0)
            ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -err_data->error, "netlink error while configuring network");
        }
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Protection for attacks like CVE-2019-5736.  */
ಪೂರ್ಣಾಙ್ಕ ensure_cloned_ದ್ವಿಮಾನ ();
__attribute__ ((constructor)) ಸ್ಥಿರ ಶೂನ್ಯ
libcrun_rexec (ಶೂನ್ಯ)
{
  ಯದಿ (ensure_cloned_ದ್ವಿಮಾನ () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      _safe_exit (EXIT_FAILURE);
    }
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_checkpoint_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                    libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ libcrun_container_checkpoint_linux_criu (status, container, cr_options, err);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_container_restore_linux (libcrun_container_status_t *status, libcrun_container_t *container,
                                 libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  ret = libcrun_container_restore_linux_criu (status, container, cr_options, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Fallback to use kill(2) on systems where pidfd is not available.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
libcrun_kill_linux_no_pidfd (libcrun_container_status_t *status, bool check_pid, ಪೂರ್ಣಾಙ್ಕ ಸಙ್ಕೇತ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  /* There is still a possibility that the pid is killed between the check
     and the time we send the signal, but attempt to reduce the window of time when
     it is possible.  */
  ಯದಿ (check_pid)
    {
      ret = libcrun_check_pid_valid (status, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      /* The pid is not valid anymore, return an error.  */
      ಯದಿ (ret == 0)
        {
          errno = ESRCH;
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "kill container");
        }
    }

  ret = kill (status->pid, ಸಙ್ಕೇತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "kill container");
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_kill_linux (libcrun_container_status_t *status, ಪೂರ್ಣಾಙ್ಕ ಸಙ್ಕೇತ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ pidfd = -1;

  pidfd = syscall_pidfd_open (status->pid, 0);
  ಯದಿ (UNLIKELY (pidfd < 0))
    {
      /* If pidfd_open is not supported, fallback to kill.  */
      ಯದಿ (errno == ENOSYS)
        ಪ್ರತ್ಯಾಗಮ libcrun_kill_linux_no_pidfd (status, true, ಸಙ್ಕೇತ, err);
      /* Kernels < 6.16 return EINVAL, newer ones ENOENT, when the pid
         is not a thread-group leader (i.e. it was reused).  */
      ಯದಿ (errno == ESRCH || errno == ENOENT || errno == EINVAL)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ESRCH, "process not running");
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open pidfd");
    }

  ret = libcrun_check_pid_valid (status, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  /* The pid is not valid anymore, return an error.  */
  ಯದಿ (ret == 0)
    {
      errno = ESRCH;
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "kill container");
    }

  ret = syscall_pidfd_send_signal (pidfd, ಸಙ್ಕೇತ, ರಿಕ್ತ, 0);
  ಯದಿ (UNLIKELY (ret < 0))
    {
      /* If pidfd_send_signal is not supported, fallback to kill.  */
      ಯದಿ (errno == ENOSYS)
        ಪ್ರತ್ಯಾಗಮ libcrun_kill_linux_no_pidfd (status, false, ಸಙ್ಕೇತ, err);
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "send signal to pidfd");
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
libcrun_get_intelrdt_name (ಧ್ರುವ ವರ್ಣ *ctr_name, runtime_spec_schema_config_schema *def, bool *explicit)
{
  ಯದಿ (def == ರಿಕ್ತ || def->linux == ರಿಕ್ತ || def->linux->intel_rdt == ರಿಕ್ತ || def->linux->intel_rdt->clos_id == ರಿಕ್ತ)
    {
      ಯದಿ (explicit)
        *explicit = false;
      ಪ್ರತ್ಯಾಗಮ ctr_name;
    }

  ಯದಿ (explicit)
    *explicit = true;
  ಪ್ರತ್ಯಾಗಮ def->linux->intel_rdt->clos_id;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_apply_intelrdt (ಧ್ರುವ ವರ್ಣ *ctr_name, libcrun_container_t *container, pid_t pid, ಪೂರ್ಣಾಙ್ಕ actions, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = ರಿಕ್ತ;
  bool has_monitoring = false;
  bool explicit = false;
  bool created = false;
  ಧ್ರುವ ವರ್ಣ *name;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (container)
    def = container->container_def;

  ಯದಿ (def == ರಿಕ್ತ || def->linux == ರಿಕ್ತ || def->linux->intel_rdt == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, &explicit);

  ಯದಿ (actions & LIBCRUN_INTELRDT_CREATE)
    {
      ret = resctl_create (name, explicit, &created, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ಯದಿ (def->linux->intel_rdt->enable_monitoring)
        {
          ret = resctl_create_monitoring_group (name, ctr_name, err);
          ಯದಿ (UNLIKELY (ret < 0))
            ಗಚ್ಛ fail;

          has_monitoring = true;
        }
    }

  ಯದಿ (actions & LIBCRUN_INTELRDT_UPDATE)
    {
      ret = resctl_update (name, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಗಚ್ಛ fail;
    }

  ಯದಿ (actions & LIBCRUN_INTELRDT_MOVE)
    {
      ಧ್ರುವ ವರ್ಣ *monitoring_name = def->linux->intel_rdt->enable_monitoring ? ctr_name : ರಿಕ್ತ;
      ret = resctl_move_task_to (name, monitoring_name, pid, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಗಚ್ಛ fail;
    }

  ಪ್ರತ್ಯಾಗಮ 0;

fail:
  ಯದಿ (has_monitoring)
    {
      libcrun_error_t tmp_err = ರಿಕ್ತ;
      ಪೂರ್ಣಾಙ್ಕ tmp_ret;

      tmp_ret = resctl_destroy_monitoring_group (name, ctr_name, &tmp_err);
      ಯದಿ (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }

  /* Cleanup only if the resctl was created as part of this call.  */
  ಯದಿ (created)
    {
      libcrun_error_t tmp_err = ರಿಕ್ತ;
      ಪೂರ್ಣಾಙ್ಕ tmp_ret;

      tmp_ret = resctl_destroy (name, &tmp_err);
      ಯದಿ (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_destroy_intelrdt (ಧ್ರುವ ವರ್ಣ *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  bool explicit = false;
  ಧ್ರುವ ವರ್ಣ *clos_id = libcrun_get_intelrdt_name (container_id, def, &explicit);

  ಯದಿ (def && def->linux && def->linux->intel_rdt && def->linux->intel_rdt->enable_monitoring)
    {
      ಪೂರ್ಣಾಙ್ಕ ret;

      ret = resctl_destroy_monitoring_group (clos_id, container_id, err);
      ಯದಿ (ret < 0)
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  /* Do not destroy the clos_id if the name was set explicitly.  */
  ಯದಿ (explicit)
    ಪ್ರತ್ಯಾಗಮ 0;

  ಪ್ರತ್ಯಾಗಮ resctl_destroy (clos_id, err);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_update_intel_rdt (ಧ್ರುವ ವರ್ಣ *ctr_name, libcrun_container_t *container, ಧ್ರುವ ವರ್ಣ *l3_cache_schema, ಧ್ರುವ ವರ್ಣ *mem_bw_schema, ವರ್ಣ *ಧ್ರುವ *schemata, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *name;

  ಯದಿ (container)
    def = container->container_def;

  ಯದಿ (def == ರಿಕ್ತ || def->linux == ರಿಕ್ತ || def->linux->intel_rdt == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, ರಿಕ್ತ);

  ಪ್ರತ್ಯಾಗಮ resctl_update (name, l3_cache_schema, mem_bw_schema, schemata, err);
}

/* Change the current directory and make sure the current working
   directory, once set, is accessible from the current mount
   namespace.  This check prevents container-escape issues like
   CVE-2024-21626.
   The current working directory cannot be longer than PATH_MAX.
*/
ಪೂರ್ಣಾಙ್ಕ
libcrun_safe_chdir (ಧ್ರುವ ವರ್ಣ *path, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *buffer = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = chdir (path);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "chdir to `%s`", path);

  buffer = xmalloc (PATH_MAX);
  ret = syscall_getcwd (buffer, PATH_MAX);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, -ret, "getcwd");

  /* Enforce that the returned path is an absolute path.  */
  ಯದಿ (ret == 0 || buffer[0] != '/')
    {
      ಯದಿ (chdir ("/") < 0)
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
#define UNREACHABLE_LEN ((ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (UNREACHABLE) - 1)

      ಯದಿ ((ret >= UNREACHABLE_LEN) && (memcmp (buffer, UNREACHABLE, UNREACHABLE_LEN) == 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "the working directory is not accessible from the current namespace");

      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "the current working directory is not an absolute path");
    }
  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Run the callback in the mount namespace referred to by MNTNS_FD.  It is
   done in a separate function so that no variable of the caller is live
   across the vfork.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
run_callback_in_namespace (ಪೂರ್ಣಾಙ್ಕ mntns_fd, ಪೂರ್ಣಾಙ್ಕ (*callback) (ಶೂನ್ಯ *, libcrun_error_t *), ಶೂನ್ಯ *arg, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ wait_status = 0;
  pid_t pid;
  ಪೂರ್ಣಾಙ್ಕ ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  ಯದಿ (UNLIKELY (pid < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "vfork");

  ಯದಿ (pid == 0)
    {
      ret = setns (mntns_fd, CLONE_NEWNS);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, 0, "setns to target pid");
          _safe_exit (ret);
        }
      ret = chdir ("/");
      ಯದಿ (UNLIKELY (ret < 0))
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
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "waitpid for exec child pid");

  /* The vfork() child shares the parent's memory space, so the error
     object populated by the child is available to the parent.
     Do not call crun_make_error() here. */
  ಪ್ರತ್ಯಾಗಮ get_process_exit_status (wait_status);
}

/* Check that USERNS_FD refers to the user namespace configured for the container.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
check_container_userns (libcrun_container_t *container, ಪೂರ್ಣಾಙ್ಕ userns_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  bool new_userns = container->host_uid != 0;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ expected_fd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ parent_fd = -1;
  ಸಂರಚನಾ stat st, expected_st;
  ಧ್ರುವ ವರ್ಣ *path = ರಿಕ್ತ;
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (def->linux)
    ಪ್ರತಿ (i = 0; i < def->linux->namespaces_len; i++)
      ಯದಿ (libcrun_find_namespace (def->linux->namespaces[i]->type) == CLONE_NEWUSER)
        {
          path = def->linux->namespaces[i]->path;
          new_userns = path == ರಿಕ್ತ;
          ಭಙ್ಗ;
        }

  ಯದಿ (path == ರಿಕ್ತ)
    {
      expected_fd = libcrun_open_proc_file (container, "self/ns/user", O_RDONLY, err);
      ಯದಿ (UNLIKELY (expected_fd < 0))
        ಪ್ರತ್ಯಾಗಮ expected_fd;
    }
  ಅನ್ಯಥಾ
    {
      expected_fd = open (path, O_RDONLY | O_CLOEXEC);
      ಯದಿ (UNLIKELY (expected_fd < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", path);
    }

  /* A user namespace created for the container is a child of the current one.  */
  ಯದಿ (new_userns)
    {
      parent_fd = ioctl (userns_fd, NS_GET_PARENT);
      ಯದಿ (UNLIKELY (parent_fd < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "get the parent of the user namespace");
      userns_fd = parent_fd;
    }

  ret = fstat (userns_fd, &st);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fstat user namespace");

  ret = fstat (expected_fd, &expected_st);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fstat user namespace");

  ಯದಿ (st.st_dev != expected_st.st_dev || st.st_ino != expected_st.st_ino)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "the container process is not using the user namespace of the container");

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Open the mount namespace of the container in MNTNS_FD and the user namespace
   owning it in OWNER_FD.  The container process could have moved to a mount
   namespace owned by a nested user namespace, where it has CAP_SYS_ADMIN.  A
   mount attached there is not protected by the MNT_LOCK_* flags the kernel
   sets on the mounts copied to a less privileged namespace, so the container
   could drop its ro, nosuid, nodev and noexec flags.  Make sure the mount
   namespace is owned by the user namespace of the container, so that the
   caller can join the same namespaces that were checked.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
open_container_mntns (libcrun_container_t *container, libcrun_container_status_t *status, ಪೂರ್ಣಾಙ್ಕ *mntns_fd, ಪೂರ್ಣಾಙ್ಕ *owner_fd, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ mntns = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ owner = -1;
  ಪೂರ್ಣಾಙ್ಕ ret;

  mntns = libcrun_open_proc_pid_file (container, status->pid, "ns/mnt", O_RDONLY, err);
  ಯದಿ (UNLIKELY (mntns < 0))
    ಪ್ರತ್ಯಾಗಮ mntns;

  /* Check if the container is still running after opening the namespace.  */
  ret = libcrun_is_container_running (status, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (ret == 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "container not running");

  owner = ioctl (mntns, NS_GET_USERNS);
  ಯದಿ (UNLIKELY (owner < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "get the owner of the mount namespace");

  ret = check_container_userns (container, owner, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  *mntns_fd = mntns;
  mntns = -1;
  *owner_fd = owner;
  owner = -1;
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
run_in_container_namespace (libcrun_container_t *container, libcrun_container_status_t *status, ಪೂರ್ಣಾಙ್ಕ (*callback) (ಶೂನ್ಯ *, libcrun_error_t *), ಶೂನ್ಯ *arg, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ mntns_fd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ owner_fd = -1;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ run_callback_in_namespace (mntns_fd, callback, arg, err);
}

ಸಂರಚನಾ umount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  size_t len;
};

/* Detach the mount at DESTINATION, relative to the current root.  The file
   system tree is under the control of the container, so no symlink is
   followed, otherwise the container could choose what mount is detached.
   umount2 accepts only a path, so unmount the last component from its parent
   directory and then go back to the previous working directory.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
umount_nofollow (ಧ್ರುವ ವರ್ಣ *destination, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ parentfd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ cwdfd = -1;
  ವರ್ಣ buffer[PATH_MAX];
  ಧ್ರುವ ವರ್ಣ *parent, *name;
  ವರ್ಣ *last_slash;
  size_t len;
  ಪೂರ್ಣಾಙ್ಕ saved_errno;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (is_empty_string (destination))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "empty destination for mount");

  len = strlen (destination);
  ಯದಿ (UNLIKELY (len >= ಪರಿಮಾಣ (buffer)))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, ENAMETOOLONG, "umount `%s`", destination);
  memcpy (buffer, destination, len + 1);

  ಯಾವತ್ (len > 1 && buffer[len - 1] == '/')
    buffer[--len] = '\0';

  last_slash = strrchr (buffer, '/');
  ಯದಿ (last_slash == ರಿಕ್ತ)
    {
      parent = ".";
      name = buffer;
    }
  ಅನ್ಯಥಾ ಯದಿ (last_slash == buffer)
    {
      parent = "/";
      name = buffer[1] ? buffer + 1 : ".";
    }
  ಅನ್ಯಥಾ
    {
      *last_slash = '\0';
      parent = buffer;
      name = last_slash + 1;
    }

  parentfd = syscall_openat2 (AT_FDCWD, parent, O_PATH | O_DIRECTORY | O_CLOEXEC, 0,
                              RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS);
  ಯದಿ (UNLIKELY (parentfd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open parent directory of `%s`", destination);

  cwdfd = open (".", O_PATH | O_DIRECTORY | O_CLOEXEC);
  ಯದಿ (UNLIKELY (cwdfd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open current directory");

  ret = fchdir (parentfd);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fchdir to parent directory of `%s`", destination);

  ret = umount2 (name, MNT_DETACH | UMOUNT_NOFOLLOW);
  saved_errno = errno;

  ಯದಿ (UNLIKELY (fchdir (cwdfd) < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fchdir to the previous working directory");

  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, saved_errno, "umount `%s`", destination);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_umount_in_a_container (ಶೂನ್ಯ *arg, libcrun_error_t *err)
{
  ಸಂರಚನಾ umount_in_a_container_args *args = arg;
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಪ್ರತಿ (i = 0; i < args->len; i++)
    {
      ret = umount_nofollow (args->mounts[i]->destination, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ mount_in_a_container_args
{
  runtime_spec_schema_defs_mount **mounts;
  ಸಂರಚನಾ libcrun_fd_map *fds;
  ಪೂರ್ಣಾಙ್ಕ pidfd;
  ಪೂರ್ಣಾಙ್ಕ owner_fd;
  size_t len;
};

/* Open DESTINATION, relative to the current root, creating it if it does not
   exist.  The file system tree is under the control of the container, so no
   symlink is followed, as it happens for `mounts remove`, otherwise a mount
   could be added that cannot be removed.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
open_destination_nofollow (ಧ್ರುವ ವರ್ಣ *destination, bool dir, libcrun_error_t *err)
{
  ಧ್ರುವ uint64_t resolve = RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS;
  ಪೂರ್ಣಾಙ್ಕ fd;
  ಪೂರ್ಣಾಙ್ಕ ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  ಯದಿ (LIKELY (fd >= 0))
    ಪ್ರತ್ಯಾಗಮ fd;

  ಯದಿ (errno != ENOENT)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", destination);

  ಯದಿ (dir)
    ret = crun_safe_ensure_directory_at (AT_FDCWD, "/", destination, 0755, err);
  ಅನ್ಯಥಾ
    ret = crun_safe_ensure_file_at (AT_FDCWD, "/", destination, 0755, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_PATH | O_CLOEXEC, 0, resolve);
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", destination);

  ಪ್ರತ್ಯಾಗಮ fd;
}

/* Create the missing destinations of the mounts as the root user of the
   container.  The file systems of the container can be owned by its user
   namespace, where the root user of the host has no mapping, so that an
   inode it creates there would have no owner and mkdir fails with
   EOVERFLOW.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
create_destinations_in_a_container (ಶೂನ್ಯ *arg, libcrun_error_t *err)
{
  ಸಂರಚನಾ mount_in_a_container_args *args = arg;
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = setns (args->owner_fd, CLONE_NEWUSER);
  ಯದಿ (ret == 0)
    {
      ret = setresgid (0, 0, 0);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "setresgid to `0`");

      ret = setresuid (0, 0, 0);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "setresuid to `0`");
    }
  ಅನ್ಯಥಾ ಯದಿ (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  ಪ್ರತಿ (i = 0; i < args->len; i++)
    {
      cleanup_close ಪೂರ್ಣಾಙ್ಕ dest_fd = -1;
      ಸಂರಚನಾ stat st;

      ret = fstat (args->fds->fds[i], &st);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      ಯದಿ (UNLIKELY (dest_fd < 0))
        ಪ್ರತ್ಯಾಗಮ dest_fd;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_mount_in_a_container (ಶೂನ್ಯ *arg, libcrun_error_t *err)
{
  ಸಂರಚನಾ mount_in_a_container_args *args = arg;
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಪ್ರತಿ (i = 0; i < args->len; i++)
    {
      cleanup_close ಪೂರ್ಣಾಙ್ಕ dest_fd = -1;
      ಸಂರಚನಾ stat st;

      ret = fstat (args->fds->fds[i], &st);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (args->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      ಯದಿ (UNLIKELY (dest_fd < 0))
        ಪ್ರತ್ಯಾಗಮ dest_fd;

      ret = fs_move_mount_to (args->fds->fds[i], dest_fd, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "move mount to `%s`", args->mounts[i]->destination);
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Join the namespaces of the container referred to by PIDFD, create the file
   system for each of MOUNTS that has no fd in FDS yet and send them, in order,
   to SYNC_FD.  It must be called from a child process.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
send_mounts_from_container_namespaces (libcrun_container_t *container, ಪೂರ್ಣಾಙ್ಕ pidfd, runtime_spec_schema_defs_mount **mounts,
                                       ಸಂರಚನಾ libcrun_fd_map *fds, size_t len, ಪೂರ್ಣಾಙ್ಕ flags, ಪೂರ್ಣಾಙ್ಕ owner_fd, ಪೂರ್ಣಾಙ್ಕ sync_fd, libcrun_error_t *err)
{
  pid_t pid;
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  /* The process is visible from the container once it joins its PID namespace.  */
  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  /* Join the namespaces while still being privileged, then the user
     namespace that was checked and not the one the container process is in
     right now.  */
  ret = flags ? setns (pidfd, flags) : 0;
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "setns to target pid");

  ret = setns (owner_fd, CLONE_NEWUSER);
  ಯದಿ (ret == 0)
    {
      /* CAP_SYS_ADMIN is needed in the user namespace owning the mount
         namespace.  Do not join the mount namespace of the container, any
         path used for the mount refers to the host, as it does for the
         mounts in the container configuration.  */
      ret = unshare (CLONE_NEWNS);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
    }
  ಅನ್ಯಥಾ ಯದಿ (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  ಯದಿ (UNLIKELY (pid < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fork");

  ಯದಿ (pid)
    {
      ಪೂರ್ಣಾಙ್ಕ wait_status = 0;

      /* The container can signal the process, do not hang if it is stopped.  */
      ret = TEMP_FAILURE_RETRY (waitpid (pid, &wait_status, WUNTRACED));
      ಯದಿ (ret > 0 && WIFSTOPPED (wait_status))
        {
          kill (pid, SIGKILL);
          TEMP_FAILURE_RETRY (waitpid (pid, ರಿಕ್ತ, 0));
          ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "the process creating the mounts was stopped");
        }
      _safe_exit (EXIT_SUCCESS);
    }

  ret = set_id_init (container, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = prctl (PR_SET_DUMPABLE, 0, 0, 0, 0);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  ಪ್ರತಿ (i = 0; i < len; i++)
    {
      cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;

      ಯದಿ (fds->fds[i] >= 0)
        ಅನುವರ್ತ;

      fd = open_mount_of_type (container, mounts[i], err);
      ಯದಿ (UNLIKELY (fd < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open mount of type `%s` for `%s`", mounts[i]->type, mounts[i]->destination);

      ret = send_fd_to_socket (sync_fd, fd, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Get in FLAGS the namespaces a file system can be bound to (together with
   the user namespace) that the container does not share with the current
   process.  setns requires CAP_SYS_ADMIN in the user namespace owning the
   target also for a namespace that is already the current one.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_namespaces_to_join (libcrun_container_t *container, pid_t pid, ಪೂರ್ಣಾಙ್ಕ *flags, libcrun_error_t *err)
{
  ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ
  {
    ಧ್ರುವ ವರ್ಣ *name;
    ಪೂರ್ಣಾಙ್ಕ flag;
  } namespaces[] = {
    { "ns/pid", CLONE_NEWPID },
    { "ns/net", CLONE_NEWNET },
    { "ns/ipc", CLONE_NEWIPC },
    { "ns/cgroup", CLONE_NEWCGROUP },
  };
  size_t i;

  *flags = 0;

  ಪ್ರತಿ (i = 0; i < ಪರಿಮಾಣ (namespaces) / ಪರಿಮಾಣ (namespaces[0]); i++)
    {
      cleanup_close ಪೂರ್ಣಾಙ್ಕ self_fd = -1;
      cleanup_close ಪೂರ್ಣಾಙ್ಕ target_fd = -1;
      ಸಂರಚನಾ stat self_st, target_st;
      cleanup_free ವರ್ಣ *self_path = ರಿಕ್ತ;
      ಪೂರ್ಣಾಙ್ಕ ret;

      xasprintf (&self_path, "self/%s", namespaces[i].name);

      self_fd = libcrun_open_proc_file (container, self_path, O_RDONLY, err);
      ಯದಿ (UNLIKELY (self_fd < 0))
        ಪ್ರತ್ಯಾಗಮ self_fd;

      target_fd = libcrun_open_proc_pid_file (container, pid, namespaces[i].name, O_RDONLY, err);
      ಯದಿ (UNLIKELY (target_fd < 0))
        ಪ್ರತ್ಯಾಗಮ target_fd;

      ret = fstat (self_fd, &self_st);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      ret = fstat (target_fd, &target_st);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      ಯದಿ (self_st.st_dev != target_st.st_dev || self_st.st_ino != target_st.st_ino)
        *flags |= namespaces[i].flag;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* A file system is bound to the namespaces of the process creating it (e.g.
   procfs to its PID namespace), so create the ones still missing in FDS from
   a process that joined the namespaces of the container, as it happens for
   the mounts in the container configuration.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
open_mounts_in_container_namespaces (libcrun_container_t *container, libcrun_container_status_t *status, ಪೂರ್ಣಾಙ್ಕ owner_fd,
                                     runtime_spec_schema_defs_mount **mounts, ಸಂರಚನಾ libcrun_fd_map *fds, size_t len,
                                     libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಙ್ಕ sync_fd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ pidfd = -1;
  ಪೂರ್ಣಾಙ್ಕ sync_socket_fd[2];
  bool needed = false;
  ಪೂರ್ಣಾಙ್ಕ flags;
  pid_t pid;
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಪ್ರತಿ (i = 0; i < len; i++)
    {
      ಯದಿ (fds->fds[i] >= 0)
        ಅನುವರ್ತ;

      ಯದಿ (mounts[i]->type == ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid mount type for `%s`", mounts[i]->destination);

      needed = true;
    }

  ಯದಿ (! needed)
    ಪ್ರತ್ಯಾಗಮ 0;

  pidfd = syscall_pidfd_open (status->pid, 0);
  ಯದಿ (UNLIKELY (pidfd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "pidfd_open");

  /* Check if the container is still running after opening the pidfd.  */
  ret = libcrun_is_container_running (status, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (ret == 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "container not running");

  ret = get_namespaces_to_join (container, status->pid, &flags, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "error creating socketpair");

  sync_fd = sync_socket_fd[0];

  pid = fork ();
  ಯದಿ (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));
      ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (pid == 0)
    {
      close_and_reset (&sync_fd);

      ret = send_mounts_from_container_namespaces (container, pidfd, mounts, fds, len, flags, owner_fd, sync_socket_fd[1], err);
      ಯದಿ (UNLIKELY (ret < 0))
        send_error_to_sync_socket_and_die (sync_socket_fd[1], true, err);

      _safe_exit (EXIT_SUCCESS);
    }

  TEMP_FAILURE_RETRY (close (sync_socket_fd[1]));

  ret = 0;
  ಪ್ರತಿ (i = 0; i < len; i++)
    {
      ಯದಿ (fds->fds[i] >= 0)
        ಅನುವರ್ತ;

      ret = receive_fd_from_socket (sync_fd, err);
      ಯದಿ (UNLIKELY (ret < 0))
        {
          cleanup_free ವರ್ಣ *err_str = ರಿಕ್ತ;
          ಪೂರ್ಣಾಙ್ಕ err_code = 0;

          ಯದಿ (read_error_from_sync_socket (sync_fd, &err_code, &err_str) && ! is_empty_string (err_str))
            {
              crun_error_release (err);
              ret = crun_make_error (err, err_code, "%s", err_str);
            }
          ಅನ್ಯಥಾ
            ret = crun_error_wrap (err, "receive mount fd");
          ಭಙ್ಗ;
        }

      fds->fds[i] = ret;
      ret = 0;
    }

  /* The child process reaps the process that joined the PID namespace of the container.  */
  TEMP_FAILURE_RETRY (waitpid (pid, ರಿಕ್ತ, 0));

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ unsigned long
get_runtime_mount_flags (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, uint64_t *rec_clear, uint64_t *rec_set)
{
  cleanup_free ವರ್ಣ *data = ರಿಕ್ತ;
  unsigned long extra_flags = 0;
  unsigned long flags = 0;
  size_t j;

  ಯದಿ (mount->options == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ get_default_flags (container, mount->destination, &data);

  ಪ್ರತಿ (j = 0; j < mount->options_len; j++)
    flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &data, rec_clear, rec_set);

  ಪ್ರತ್ಯಾಗಮ flags;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಸಂರಚನಾ mount_in_a_container_args args;
  cleanup_close_map ಸಂರಚನಾ libcrun_fd_map *fds = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ pidfd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ mntns_fd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ owner_fd = -1;
  pid_t pid = status->pid;
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = open_container_mntns (container, status, &mntns_fd, &owner_fd, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  fds = make_libcrun_fd_map (len);

  ಪ್ರತಿ (i = 0; i < len; i++)
    {
      bool recursive = false;
      bool nofollow = false;

      /* Do not check whether the pid is valid or not.  run_in_container_namespace will validate it.  */
      ret = maybe_get_idmapped_mount (container, def, mounts[i], pid, &(fds->fds[i]), ರಿಕ್ತ, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ಯದಿ (fds->fds[i] < 0 && is_bind_mount (mounts[i], &recursive, &nofollow))
        {
          uint64_t rec_clear = 0, rec_set = 0;
          unsigned long propagation;

          propagation = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set) & ALL_PROPAGATIONS_NO_REC;
          ಯದಿ (propagation == 0)
            propagation = MS_PRIVATE;

          fds->fds[i] = get_bind_mount (-1, mounts[i]->source, recursive, false, nofollow, propagation, err);
          ಯದಿ (UNLIKELY (fds->fds[i] < 0))
            ಪ್ರತ್ಯಾಗಮ fds->fds[i];
        }
    }

  /* All the remaining mounts are new file systems.  */
  ret = open_mounts_in_container_namespaces (container, status, owner_fd, mounts, fds, len, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತಿ (i = 0; i < len; i++)
    {
      uint64_t rec_clear = 0;
      uint64_t rec_set = 0;
      unsigned long flags;

      flags = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_set);

      ret = do_mount_setattr (false, mounts[i]->destination, fds->fds[i], 0, flags, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = do_mount_setattr (true, mounts[i]->destination, fds->fds[i], rec_clear, rec_set, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  args.mounts = mounts;
  args.fds = fds;
  args.pidfd = pidfd;
  args.owner_fd = owner_fd;
  args.len = len;

  ret = run_callback_in_namespace (mntns_fd, create_destinations_in_a_container, &args, err);
  ಯದಿ (UNLIKELY (ret > 0))
    ಪ್ರತ್ಯಾಗಮ *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ run_callback_in_namespace (mntns_fd, do_mount_in_a_container, &args, err);
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_status_t *status, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  ಸಂರಚನಾ umount_in_a_container_args args;

  args.mounts = mounts;
  args.len = len;

  ಪ್ರತ್ಯಾಗಮ run_in_container_namespace (container, status, do_umount_in_a_container, &args, err);
}

ಸಂರಚನಾ restrict_mounts_in_a_container_args
{
  ಸಂರಚನಾ libcrun_mount_flags_s *mounts;
  size_t len;
  ಪೂರ್ಣಾಙ್ಕ rootfd;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_restrict_mounts_in_a_container (ಶೂನ್ಯ *arg, libcrun_error_t *err)
{
  ಸಂರಚನಾ restrict_mounts_in_a_container_args *args = arg;
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  /* The root of the container is not necessarily the root of the mount
     namespace.  */
  ret = fchdir (args->rootfd);
  ಯದಿ (LIKELY (ret == 0))
    ret = chroot (".");
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "chroot to the container root");

  ಪ್ರತಿ (i = 0; i < args->len; i++)
    {
      unsigned long flags = args->mounts[i].flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC);
      ಧ್ರುವ ವರ್ಣ *target = consume_slashes (args->mounts[i].target);
      cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
      ಸಂರಚನಾ statfs sfs;
      unsigned long current_flags;

      ಯದಿ (flags == 0)
        ಅನುವರ್ತ;

      fd = safe_openat (AT_FDCWD, "/", target, O_PATH | O_CLOEXEC, 0, err);
      ಯದಿ (UNLIKELY (fd < 0))
        {
          ಯದಿ (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              ಅನುವರ್ತ;
            }
          ಪ್ರತ್ಯಾಗಮ fd;
        }

      ret = fstatfs (fd, &sfs);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "statfs `/%s`", target);

      current_flags = mount_flags_from_statfs (sfs.f_flags);
      ಯದಿ ((current_flags & flags) == flags)
        ಅನುವರ್ತ;

      ret = do_mount_setattr (false, target, fd, 0, flags, err);
      ಯದಿ (LIKELY (ret == 0))
        ಅನುವರ್ತ;

      ಯದಿ (crun_error_get_errno (err) != ENOSYS && crun_error_get_errno (err) != EINVAL)
        ಪ್ರತ್ಯಾಗಮ ret;
      crun_error_release (err);

      /* Without mount_setattr(2), remount through the path: the container is
         not running yet, and the root was changed to its rootfs.  The remount
         clears the per-mount flags that are not specified, so pass the
         current ones as well.  */
      ret = mount (ರಿಕ್ತ, target, ರಿಕ್ತ, MS_REMOUNT | MS_BIND | flags | current_flags, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "remount `/%s`", target);
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Make sure that each of MOUNTS, in the mount namespace of PID, has at least
   the read-only, nosuid, nodev and noexec flags that are requested for it.  */
ಪೂರ್ಣಾಙ್ಕ
libcrun_restrict_mounts_in_container (pid_t pid, ಸಂರಚನಾ libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err)
{
  ಸಂರಚನಾ restrict_mounts_in_a_container_args args;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ mntns_fd = -1;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ rootfd = -1;
  ವರ್ಣ path[64];
  ಪೂರ್ಣಾಙ್ಕ ret;

  snprintf (path, ಪರಿಮಾಣ (path), "/proc/%d/ns/mnt", pid);
  mntns_fd = open (path, O_RDONLY | O_CLOEXEC);
  ಯದಿ (UNLIKELY (mntns_fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", path);

  snprintf (path, ಪರಿಮಾಣ (path), "/proc/%d/root", pid);
  rootfd = open (path, O_PATH | O_DIRECTORY | O_CLOEXEC);
  ಯದಿ (UNLIKELY (rootfd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s`", path);

  args.mounts = mounts;
  args.len = len;
  args.rootfd = rootfd;

  ret = run_callback_in_namespace (mntns_fd, do_restrict_mounts_in_a_container, &args, err);
  ಯದಿ (UNLIKELY (ret > 0))
    ಪ್ರತ್ಯಾಗಮ *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_move_network_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ netns_fd = -1;
  size_t i;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (def == ರಿಕ್ತ || def->linux == ರಿಕ್ತ || def->linux->net_devices == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  cleanup_free ವರ್ಣ *ns_path = ರಿಕ್ತ;

  xasprintf (&ns_path, "%d/ns/net", pid);
  netns_fd = libcrun_open_proc_file (container, ns_path, O_RDONLY, err);
  ಯದಿ (UNLIKELY (netns_fd < 0))
    ಪ್ರತ್ಯಾಗಮ netns_fd;

  ಪ್ರತಿ (i = 0; i < def->linux->net_devices->len; i++)
    {
      ಧ್ರುವ ವರ್ಣ *new_name = def->linux->net_devices->ಮೌಲ್ಯಗಳು[i]->name ?: def->linux->net_devices->ಕೀಲಿಗಳು[i];

      ret = move_network_device (def->linux->net_devices->ಕೀಲಿಗಳು[i], new_name, netns_fd, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}
