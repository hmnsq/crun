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

#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "linux.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "status.h"
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <sched.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <sys/mount.h>
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H
#  include <linux/mount.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined HAVE_FSCONFIG_CMD_CREATE_LINUX_MOUNT_H || defined HAVE_FSCONFIG_CMD_CREATE_SYS_MOUNT_H
#  define HAVE_NEW_MOUNT_API
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <sys/prctl.h>
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CAP
#  include <sys/capability.h>
#ಆದರೆ_ಅಂತ್ಯ
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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ RLIMIT_RTTIME
#  define RLIMIT_RTTIME 15
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPEN_TREE_CLONE
#  define OPEN_TREE_CLONE 1
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPEN_TREE_CLOEXEC
#  define OPEN_TREE_CLOEXEC O_CLOEXEC
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPEN_ವೃಕ್ಷ_NAMESPACE
#  define OPEN_ವೃಕ್ಷ_NAMESPACE 2
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MOVE_MOUNT_F_EMPTY_PATH
#  define MOVE_MOUNT_F_EMPTY_PATH 0x00000004
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MOVE_MOUNT_T_EMPTY_PATH
#  define MOVE_MOUNT_T_EMPTY_PATH 0x00000040
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ FSOPEN_CLOEXEC
#  define FSOPEN_CLOEXEC 0x00000001
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ FSMOUNT_CLOEXEC
#  define FSMOUNT_CLOEXEC 0x00000001
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ FSCONFIG_CMD_CREATE
#  define FSCONFIG_CMD_CREATE 6
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು ALL_PROPAGATIONS_NO_REC (MS_SHARED | MS_PRIVATE | MS_SLAVE | MS_UNBINDABLE)
#ವ್ಯಾಖ್ಯಾನಿಸು ALL_PROPAGATIONS (MS_REC | ALL_PROPAGATIONS_NO_REC)

/* The per-mount flags a remount clears unless they are explicitly set.  */
#ವ್ಯಾಖ್ಯಾನಿಸು REMOUNT_CLEAR_FLAGS (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW)

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ST_NOSYMFOLLOW
#  define ST_NOSYMFOLLOW 0x2000
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು MOUNT_ATIME_FLAGS (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME)

/* Convert the ST_ flags reported by statfs(2) to the MS_ flags of mount(2).  */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ
mount_flags_from_statfs (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ f_flags)
{
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags = 0;

  ಆದರೆ (f_flags & ST_RDONLY)
    flags |= MS_RDONLY;
  ಆದರೆ (f_flags & ST_NOSUID)
    flags |= MS_NOSUID;
  ಆದರೆ (f_flags & ST_NODEV)
    flags |= MS_NODEV;
  ಆದರೆ (f_flags & ST_NOEXEC)
    flags |= MS_NOEXEC;
  ಆದರೆ (f_flags & ST_NOSYMFOLLOW)
    flags |= MS_NOSYMFOLLOW;
  ಆದರೆ (f_flags & ST_NOATIME)
    flags |= MS_NOATIME;
  ಆದರೆ (f_flags & ST_NODIRATIME)
    flags |= MS_NODIRATIME;
  ಆದರೆ (f_flags & ST_RELATIME)
    flags |= MS_RELATIME;
  /* There is no ST_ flag for strictatime: it is the mode when neither noatime
     nor relatime is set.  */
  ಆದರೆ (! (flags & (MS_NOATIME | MS_RELATIME)))
    flags |= MS_STRICTATIME;

  ಹಿಂತಿರುಗಿ flags;
}

ರಚನೆ remount_s
{
  ರಚನೆ remount_s *ಮುಂದಿನ;
  ಅಕ್ಷರ *ಗುರಿ;
  ಪೂರ್ಣಾಂಕ targetfd;
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;
  ಅಕ್ಷರ *ದತ್ತಾಂಶ;
};

ರಚನೆ ಖಾಸಗಿ_ದತ್ತಾಂಶ_s
{
  ರಚನೆ remount_s *remounts;

  /* Filled by libcrun_run_linux_container().  Useful to query what
     namespaces are available.  */
  ಪೂರ್ಣಾಂಕ unshare_flags;
  ಪೂರ್ಣಾಂಕ unshare_cgroupns;

  ಅಕ್ಷರ *host_notify_socket_ಮಾರ್ಗ;
  ಅಕ್ಷರ *container_notify_socket_ಮಾರ್ಗ;
  ಅಕ್ಷರ *unified_cgroup_ಮಾರ್ಗ;

  bool mount_dev_from_host;
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ rootfs_ಪ್ರಸರಣ;
  bool deny_setgroups;

  ಪೂರ್ಣಾಂಕ procfd;

  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs;
  ಪೂರ್ಣಾಂಕ rootfsfd;

  ಪೂರ್ಣಾಂಕ notify_socket_ವೃಕ್ಷ_fd;
  ಪೂರ್ಣಾಂಕ old_ಬೇರು_fd;

  ರಚನೆ libcrun_fd_map *mount_fds;
  ರಚನೆ libcrun_fd_map *dev_fds;
  ರಚನೆ libcrun_fd_map *needed_devs_fds;

  ಅಕ್ಷರ **copy_symlink_targets;
  size_t n_copy_symlink_targets;

  /* Used to save stdin, stdout, stderr during checkpointing to descriptors.json
   * and needed during restore. */
  ಅಕ್ಷರ *external_descriptors;

  /* Cached shared empty directory for masked paths optimization */
  ಪೂರ್ಣಾಂಕ maskdir_fd;
  ಅಕ್ಷರ *maskdir_proc_ಮಾರ್ಗ;
  bool maskdir_bind_failed;
  bool maskdir_move_failed;
  bool maskdir_warned;
  bool joined_mount_ns;
  bool needs_ಅಚ್ಚು;
  bool no_ಅಚ್ಚು;
  /* Set once the process root is the container rootfs and the host root
     can no longer be reached by path.  */
  bool host_ಬೇರು_switched;
  /* The process joining an existing PID namespace is created only once
     the container root is in place.  */
  bool delayed_pidns_fork;
  /* The rootfs path on the host, used to recognize bind mount sources
     inside the container rootfs.  */
  ಅಕ್ಷರ *host_rootfs;
};

ರಚನೆ linux_namespace_s
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ns_file;
  ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ;
};

ಸ್ಥಿರ ಶೂನ್ಯ
cleanup_ಖಾಸಗಿ_ದತ್ತಾಂಶ (ಶೂನ್ಯ *ಖಾಸಗಿ_ದತ್ತಾಂಶ)
{
  ರಚನೆ ಖಾಸಗಿ_ದತ್ತಾಂಶ_s *p = ಖಾಸಗಿ_ದತ್ತಾಂಶ;

  ಆದರೆ (p->rootfsfd >= 0)
    ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (p->rootfsfd));
  ಆದರೆ (p->procfd >= 0)
    ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (p->procfd));
  ಆದರೆ (p->maskdir_fd >= 0)
    ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (p->maskdir_fd));
  ಆದರೆ (p->old_ಬೇರು_fd >= 0)
    ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (p->old_ಬೇರು_fd));
  ಆದರೆ (p->mount_fds)
    cleanup_close_mapp (&(p->mount_fds));
  ಆದರೆ (p->dev_fds)
    cleanup_close_mapp (&(p->dev_fds));
  ಆದರೆ (p->needed_devs_fds)
    cleanup_close_mapp (&(p->needed_devs_fds));

  ಆದರೆ (p->copy_symlink_targets)
    {
      size_t i;
      ಗಾಗಿ (i = 0; i < p->n_copy_symlink_targets; i++)
        ಬಿಡುಗಡೆ (p->copy_symlink_targets[i]);
      ಬಿಡುಗಡೆ (p->copy_symlink_targets);
    }
  ಬಿಡುಗಡೆ (p->unified_cgroup_ಮಾರ್ಗ);
  ಬಿಡುಗಡೆ (p->host_notify_socket_ಮಾರ್ಗ);
  ಬಿಡುಗಡೆ (p->container_notify_socket_ಮಾರ್ಗ);
  ಬಿಡುಗಡೆ (p->external_descriptors);
  ಬಿಡುಗಡೆ (p->maskdir_proc_ಮಾರ್ಗ);
  ಬಿಡುಗಡೆ (p->host_rootfs);
  ಬಿಡುಗಡೆ (p);
}

ಸ್ಥಿರ ರಚನೆ ಖಾಸಗಿ_ದತ್ತಾಂಶ_s *
get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (ರಚನೆ libcrun_container_s *container)
{
  ಆದರೆ (container->ಖಾಸಗಿ_ದತ್ತಾಂಶ == ಖಾಲಿ)
    {
      ರಚನೆ ಖಾಸಗಿ_ದತ್ತಾಂಶ_s *p = xmalloc0 (ಗಾತ್ರ (*p));
      container->ಖಾಸಗಿ_ದತ್ತಾಂಶ = p;
      p->rootfsfd = -1;
      p->procfd = -1;
      p->notify_socket_ವೃಕ್ಷ_fd = -1;
      p->old_ಬೇರು_fd = -1;
      p->maskdir_fd = -1;
      container->cleanup_ಖಾಸಗಿ_ದತ್ತಾಂಶ = cleanup_ಖಾಸಗಿ_ದತ್ತಾಂಶ;
    }
  ಹಿಂತಿರುಗಿ container->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CLONE_NEWTIME
#  define CLONE_NEWTIME 0x00000080
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CLONE_NEWCGROUP
#  define CLONE_NEWCGROUP 0x02000000
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ AT_RECURSIVE
#  define AT_RECURSIVE 0x8000
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ರಚನೆ linux_namespace_s namespaces[] = { { "mount", "mnt", CLONE_NEWNS },
                                                 { "network", "net", CLONE_NEWNET },
                                                 { "ipc", "ipc", CLONE_NEWIPC },
                                                 { "pid", "pid", CLONE_NEWPID },
                                                 { "uts", "uts", CLONE_NEWUTS },
                                                 { "user", "user", CLONE_NEWUSER },
                                                 { "cgroup", "cgroup", CLONE_NEWCGROUP },
                                                 { "time", "time", CLONE_NEWTIME },
                                                 { ಖಾಲಿ, ಖಾಲಿ, 0 } };

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_and_reset (ಪೂರ್ಣಾಂಕ *old)
{
  ಪೂರ್ಣಾಂಕ tmp = *old;
  *old = -1;
  ಹಿಂತಿರುಗಿ tmp;
}

ಪೂರ್ಣಾಂಕ
libcrun_ಕಂಡುಹಿಡಿ_namespace (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
  ರಚನೆ linux_namespace_s *it;
  ಗಾಗಿ (it = namespaces; it->name; it++)
    ಆದರೆ (strcmp (it->name, name) == 0)
      ಹಿಂತಿರುಗಿ it->ಮೌಲ್ಯ;
  ಹಿಂತಿರುಗಿ -1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
syscall_keyctl_join (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
#ವ್ಯಾಖ್ಯಾನಿಸು KEYCTL_JOIN_SESSION_KEYRING 0x1
  ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) syscall (__NR_keyctl, KEYCTL_JOIN_SESSION_KEYRING, name, 0);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
syscall_pidfd_open (pid_t pid, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
#ಆದರೆ defined __NR_pidfd_open
  ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) syscall (__NR_pidfd_open, pid, flags);
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) pid;
  (ಶೂನ್ಯ) flags;
  errno = ENOSYS;
  ಹಿಂತಿರುಗಿ -1;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
syscall_pidfd_send_signal (ಪೂರ್ಣಾಂಕ pidfd, ಪೂರ್ಣಾಂಕ sig, siginfo_t *info, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
#ಆದರೆ defined __NR_pidfd_send_signal
  ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) syscall (__NR_pidfd_send_signal, pidfd, sig, info, flags);
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) pidfd;
  (ಶೂನ್ಯ) sig;
  (ಶೂನ್ಯ) info;
  (ಶೂನ್ಯ) flags;
  errno = ENOSYS;
  ಹಿಂತಿರುಗಿ -1;
#ಆದರೆ_ಅಂತ್ಯ
}

/* Translate MS_* mount flags to MOUNT_ATTR_* values for mount_setattr().  */
ಸ್ಥಿರ uint64_t
ms_flags_ಗಮ್ಯ_mount_attr (uint64_t ms_flags)
{
  uint64_t attr = 0;

  ಆದರೆ (ms_flags & MS_RDONLY)
    attr |= MOUNT_ATTR_RDONLY;
  ಆದರೆ (ms_flags & MS_NOSUID)
    attr |= MOUNT_ATTR_NOSUID;
  ಆದರೆ (ms_flags & MS_NODEV)
    attr |= MOUNT_ATTR_NODEV;
  ಆದರೆ (ms_flags & MS_NOEXEC)
    attr |= MOUNT_ATTR_NOEXEC;
  ಆದರೆ (ms_flags & MS_NOATIME)
    attr |= MOUNT_ATTR_NOATIME;
  ಆದರೆ (ms_flags & MS_STRICTATIME)
    attr |= MOUNT_ATTR_STRICTATIME;
  ಆದರೆ (ms_flags & MS_NODIRATIME)
    attr |= MOUNT_ATTR_NODIRATIME;
  ಆದರೆ (ms_flags & MS_NOSYMFOLLOW)
    attr |= MOUNT_ATTR_NOSYMFOLLOW;

  ಹಿಂತಿರುಗಿ attr;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_mount_setattr (bool recursive, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ, ಪೂರ್ಣಾಂಕ targetfd, uint64_t clear, uint64_t ಗಣ, libcrun_error_t *err)
{
  ರಚನೆ mount_attr_s attr = {
    0,
  };
  ಪೂರ್ಣಾಂಕ ret;

  ಗಣ &= ~MS_BIND;
  clear &= ~MS_BIND;

  attr.ಪ್ರಸರಣ = ಗಣ & ALL_PROPAGATIONS_NO_REC;
  attr.attr_set = ms_flags_ಗಮ್ಯ_mount_attr (ಗಣ & (~ALL_PROPAGATIONS));
  attr.attr_clr = ms_flags_ಗಮ್ಯ_mount_attr (clear & (~ALL_PROPAGATIONS));

  /* Like mount(2) does, if any atime flag is set, set all the atime ones
     anew: relatime is the default, noatime and strictatime override it
     (strictatime wins), and nodiratime is independent.  Otherwise, the
     atime flags of the mount are kept.  */
  ಆದರೆ (ಗಣ & (MS_NOATIME | MS_NODIRATIME | MS_RELATIME | MS_STRICTATIME))
    {
      attr.attr_set &= ~(MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME);
      ಆದರೆ (ಗಣ & MS_STRICTATIME)
        attr.attr_set |= MOUNT_ATTR_STRICTATIME;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಗಣ & MS_NOATIME)
        attr.attr_set |= MOUNT_ATTR_NOATIME;
      ಇಲ್ಲದಿದ್ದರೆ
        attr.attr_set |= MOUNT_ATTR_RELATIME;
      ಆದರೆ (ಗಣ & MS_NODIRATIME)
        attr.attr_set |= MOUNT_ATTR_NODIRATIME;
      attr.attr_clr |= MOUNT_ATTR__ATIME | MOUNT_ATTR_NODIRATIME;
    }
  /* The kernel rejects a partial MOUNT_ATTR__ATIME in attr_clr, so clearing
     any of the atime modes resets the mount to the relatime default.  */
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (clear & (MS_NOATIME | MS_RELATIME | MS_STRICTATIME))
    attr.attr_clr |= MOUNT_ATTR__ATIME;

  ret = syscall_mount_setattr (targetfd, "", (recursive ? AT_RECURSIVE : 0) | AT_ಖಾಲಿಗಳು_ಮಾರ್ಗ, &attr);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "mount_setattr `/%s`", ಗುರಿ);

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
get_bind_mount (ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *src, bool recursive, bool rdonly, bool nofollow, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪ್ರಸರಣ, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ open_ವೃಕ್ಷ_fd = -1;
  ರಚನೆ mount_attr_s attr = {
    0,
  };
  ಪೂರ್ಣಾಂಕ recursive_flag = (recursive ? AT_RECURSIVE : 0);
  ಪೂರ್ಣಾಂಕ ret;

  /* Several callers pass a source straight from the configuration, where it
     is optional, so it can be missing altogether.  Callers that mean the
     directory referred to by dirfd pass an empty string, never NULL.  */
  ಆದರೆ (UNLIKELY (src == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, EINVAL, "mount source is not specified");

  ಆದರೆ (rdonly)
    attr.attr_set = MS_RDONLY;

  attr.ಪ್ರಸರಣ = ಪ್ರಸರಣ;

  ಆದರೆ (dirfd < 0)
    dirfd = AT_FDCWD;

  errno = 0;
  open_ವೃಕ್ಷ_fd = syscall_open_ವೃಕ್ಷ (dirfd, src,
                                    AT_NO_AUTOMOUNT | OPEN_TREE_CLOEXEC
                                        | OPEN_TREE_CLONE | recursive_flag
                                        | (src[0] == '\0' ? AT_ಖಾಲಿಗಳು_ಮಾರ್ಗ : 0)
                                        | (nofollow ? AT_SYMLINK_NOFOLLOW : 0));
  ಆದರೆ (UNLIKELY (open_ವೃಕ್ಷ_fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open_tree `%s`", src);

  ret = syscall_mount_setattr (open_ವೃಕ್ಷ_fd, "", AT_ಖಾಲಿಗಳು_ಮಾರ್ಗ | recursive_flag, &attr);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "mount_setattr `%s`", src);

  ಹಿಂತಿರುಗಿ get_and_reset (&open_ವೃಕ್ಷ_fd);
}

ಪೂರ್ಣಾಂಕ
parse_idmapped_mount_option (runtime_spec_schema_config_schema *def, bool is_uids, ಅಕ್ಷರ *option, ಅಕ್ಷರ **ಹೊರಹೋಗು, size_t *len, libcrun_error_t *err)
{
  size_t written = 0, allocated = 256;
  cleanup_free ಅಕ್ಷರ *mappings = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *it;
  ಪೂರ್ಣಾಂಕ ret;

  mappings = xmalloc (allocated);

  ಗಾಗಿ (it = option; *it;)
    {
      bool relative = false;
      ದೀರ್ಘ ಮೌಲ್ಯ[3];
      size_t i;

      ಆದರೆ (*it == '\0')
        ನಿಲ್ಲಿಸು;

      ಆದರೆ (*it == '#')
        it++;

      ಆದರೆ (*it == '@')
        {
          relative = true;
          it++;
        }

      /* read a triplet: file system id - host id - size.  */
      ಗಾಗಿ (i = 0; i < 3; i++)
        {
          ಅಕ್ಷರ *endptr = ಖಾಲಿ;

          ಆದರೆ (i > 0 && *it == '-')
            it++;

          ಆದರೆ (*it == '\0')
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid mapping specified `%s`", option);

          errno = 0;
          ಮೌಲ್ಯ[i] = strtol (it, &endptr, 10);
          ಆದರೆ (errno || endptr == it)
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "invalid mapping specified `%s`", option);

          it = endptr;
        }

      ಆದರೆ (relative)
        {
          runtime_spec_schema_defs_id_mapping **mappings;
          size_t mappings_len;

          ಆದರೆ (def == ಖಾಲಿ
              || def->linux == ಖಾಲಿ
              || (is_uids && def->linux->uid_mappings_len == 0)
              || (! is_uids && def->linux->gid_mappings_len == 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "specified a relative mapping without user namespace mappings");

          mappings_len = (is_uids ? def->linux->uid_mappings_len : def->linux->gid_mappings_len);
          mappings = is_uids ? def->linux->uid_mappings : def->linux->gid_mappings;

          ಗಾಗಿ (i = 0; i < mappings_len; i++)
            ಆದರೆ (ಮೌಲ್ಯ[1] >= (int64_t) mappings[i]->container_id
                && ಮೌಲ್ಯ[1] < (int64_t) mappings[i]->container_id + (int64_t) mappings[i]->ಗಾತ್ರ_2)
              ನಿಲ್ಲಿಸು;

          ಆದರೆ (i == mappings_len)
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "could not find a user namespace mapping for the relative mapping `%s`", option);

          ಮೌಲ್ಯ[1] += mappings[i]->host_id - mappings[i]->container_id;
        }
      ಆದರೆ (written > allocated - 64)
        {
          allocated += 256;
          mappings = xrealloc (mappings, allocated);
        }

      ret = snprintf (mappings + written, allocated - written, "%ld %ld %ld\n", ಮೌಲ್ಯ[0], ಮೌಲ್ಯ[1], ಮೌಲ್ಯ[2]);
      ಆದರೆ (UNLIKELY (ret >= (ಪೂರ್ಣಾಂಕ) (allocated - written)))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: allocated buffer too small");

      written += ret;
    }
  *(mappings + written) = '\0';

  *len = written;

  *ಹೊರಹೋಗು = mappings;
  mappings = ಖಾಲಿ;

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
format_mount_mappings (ಅಕ್ಷರ **ಹೊರಹೋಗು,
                       runtime_spec_schema_defs_id_mapping **mappings,
                       size_t mappings_len,
                       size_t *written,
                       libcrun_error_t *err)
{
  /* 64 is more than enough room to print 3 uint32.  */
  ಸ್ಥಿರಾಂಕ size_t max_len_mapping = 64;
  cleanup_free ಅಕ್ಷರ *ret = ಖಾಲಿ;
  size_t s;

  *written = 0;

  ret = xmalloc (max_len_mapping * mappings_len + 1);
  ಗಾಗಿ (s = 0; s < mappings_len; s++)
    {
      size_t len;

      len = snprintf (ret + *written, max_len_mapping, "%" PRIu32 " %" PRIu32 " %" PRIu32 "\n",
                      mappings[s]->container_id,
                      mappings[s]->host_id,
                      mappings[s]->ಗಾತ್ರ_2);
      ಆದರೆ (UNLIKELY (len >= max_len_mapping))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: allocated buffer too small");

      *written += len;
    }
  *ಹೊರಹೋಗು = ret;
  ret = ಖಾಲಿ;
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
format_mount_mapping (ಅಕ್ಷರ **ret, uint32_t container_id, uint32_t host_id,
                      uint32_t ಗಾತ್ರ_2, size_t *written, libcrun_error_t *err)
{
  runtime_spec_schema_defs_id_mapping mapping = {
    .container_id = container_id,
    .host_id = host_id,
    .ಗಾತ್ರ_2 = ಗಾತ್ರ_2,
  };
  runtime_spec_schema_defs_id_mapping *mappings[] = {
    &mapping,
    ಖಾಲಿ,
  };

  ಹಿಂತಿರುಗಿ format_mount_mappings (ret, mappings, 1, written, err);
}

ಸ್ಥಿರ bool
has_same_mappings (runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt)
{
  size_t s;

  ಆದರೆ (def->linux == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ mnt->uid_mappings_len == 0 && mnt->gid_mappings_len == 0;

  ಆದರೆ (mnt->uid_mappings_len != def->linux->uid_mappings_len)
    ಹಿಂತಿರುಗಿ false;

  ಆದರೆ (mnt->gid_mappings_len != def->linux->gid_mappings_len)
    ಹಿಂತಿರುಗಿ false;

  ಗಾಗಿ (s = 0; s < mnt->uid_mappings_len; s++)
    {
      ಆದರೆ (mnt->uid_mappings[s]->container_id != def->linux->uid_mappings[s]->container_id)
        ಹಿಂತಿರುಗಿ false;
      ಆದರೆ (mnt->uid_mappings[s]->host_id != def->linux->uid_mappings[s]->host_id)
        ಹಿಂತಿರುಗಿ false;
      ಆದರೆ (mnt->uid_mappings[s]->ಗಾತ್ರ_2 != def->linux->uid_mappings[s]->ಗಾತ್ರ_2)
        ಹಿಂತಿರುಗಿ false;
    }

  ಗಾಗಿ (s = 0; s < mnt->gid_mappings_len; s++)
    {
      ಆದರೆ (mnt->gid_mappings[s]->container_id != def->linux->gid_mappings[s]->container_id)
        ಹಿಂತಿರುಗಿ false;
      ಆದರೆ (mnt->gid_mappings[s]->host_id != def->linux->gid_mappings[s]->host_id)
        ಹಿಂತಿರುಗಿ false;
      ಆದರೆ (mnt->gid_mappings[s]->ಗಾತ್ರ_2 != def->linux->gid_mappings[s]->ಗಾತ್ರ_2)
        ಹಿಂತಿರುಗಿ false;
    }

  ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ pid_t
maybe_create_userns_for_idmapped_mount (libcrun_container_t *container,
                                        runtime_spec_schema_config_schema *def,
                                        runtime_spec_schema_defs_mount *mnt,
                                        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *options, pid_t *pid_ಹೊರಹೋಗು,
                                        libcrun_error_t *err)
{
  cleanup_pid pid_t pid = -1;
  bool need_new_userns = mnt->uid_mappings_len ? ! has_same_mappings (def, mnt) : options != ಖಾಲಿ;

  ಆದರೆ (! need_new_userns)
    ಹಿಂತಿರುಗಿ 0;

  pid = syscall_clone (CLONE_NEWUSER | SIGCHLD, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (pid < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "clone");

  ಆದರೆ (pid == 0)
    {
      prctl (PR_ಗಣ_PDEATHSIG, SIGKILL);
      ಆಗ (1)
        pause ();
      ಸುರಕ್ಷಿತ_exit (EXIT_SUCCESS);
    }

  ಆದರೆ (mnt->uid_mappings_len)
    {
      cleanup_free ಅಕ್ಷರ *uid_map = ಖಾಲಿ;
      cleanup_free ಅಕ್ಷರ *gid_map = ಖಾಲಿ;
      size_t written = 0;
      ಪೂರ್ಣಾಂಕ ret;

      ret = format_mount_mappings (&uid_map, mnt->uid_mappings, mnt->uid_mappings_len, &written, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;

      fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
      ಆದರೆ (UNLIKELY (fd < 0))
        ಹಿಂತಿರುಗಿ fd;

      ret = ಸುರಕ್ಷಿತ_write (fd, "uid_map", uid_map, written, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = format_mount_mappings (&gid_map, mnt->gid_mappings, mnt->gid_mappings_len, &written, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      close_and_reset (&fd);

      fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
      ಆದರೆ (UNLIKELY (fd < 0))
        ಹಿಂತಿರುಗಿ fd;

      ret = ಸುರಕ್ಷಿತ_write (fd, "gid_map", gid_map, written, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      cleanup_free ಅಕ್ಷರ *dup_options = ಖಾಲಿ;
      ಅಕ್ಷರ *option, *saveptr = ಖಾಲಿ;

      ಆದರೆ (! options)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: no mappings found");

      dup_options = xstrdup (options);

      /* If there are no OCI mappings specified, then parse the annotation.  */
      ಗಾಗಿ (option = strtok_r (dup_options, ";", &saveptr); option; option = strtok_r (ಖಾಲಿ, ";", &saveptr))
        {
          cleanup_free ಅಕ್ಷರ *mappings = ಖಾಲಿ;
          cleanup_close ಪೂರ್ಣಾಂಕ map_fd = -1;
          bool is_uids = false;
          size_t len = 0;
          ಪೂರ್ಣಾಂಕ ret;

          ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (option, "uids="))
            {
              is_uids = true;
              map_fd = libcrun_open_proc_pid_file (container, pid, "uid_map", O_WRONLY, err);
              ಆದರೆ (UNLIKELY (map_fd < 0))
                ಹಿಂತಿರುಗಿ map_fd;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (option, "gids="))
            {
              map_fd = libcrun_open_proc_pid_file (container, pid, "gid_map", O_WRONLY, err);
              ಆದರೆ (UNLIKELY (map_fd < 0))
                ಹಿಂತಿರುಗಿ map_fd;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid option `%s` specified", option);

          ret = parse_idmapped_mount_option (def, is_uids, option + 5 /* strlen ("uids="), strlen ("gids=")*/, &mappings, &len, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;

          ret = ಸುರಕ್ಷಿತ_write (map_fd, is_uids ? "uid_map" : "gid_map", mappings, len, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }
    }

  *pid_ಹೊರಹೋಗು = pid;
  pid = -1;
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_create_keyring (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *label, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ labelfd = -1;
  bool label_ಗಣ = false;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (label)
    {
      labelfd = libcrun_open_proc_file (container, "self/attr/keycreate", O_WRONLY, err);
      ಆದರೆ (UNLIKELY (labelfd < 0))
        {
          ಆದರೆ (crun_error_get_errno (err) != ENOENT)
            ಹಿಂತಿರುಗಿ labelfd;

          crun_error_release (err);
        }

      ಆದರೆ (labelfd >= 0)
        {
          ret = ಲಿಖ (labelfd, label, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (label));
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "write to `/proc/self/attr/keycreate`");

          label_ಗಣ = true;
        }
    }

  ret = syscall_keyctl_join (name);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಆದರೆ (errno == ENOSYS)
        {
          libcrun_warning ("could not create a new keyring: keyctl_join is not supported");
          ret = 0;
          ಹೋಗು ಹೊರಹೋಗು;
        }
      ret = crun_make_error (err, errno, "join keyctl `%s`", name ? name : "");
      ಹೋಗು ಹೊರಹೋಗು;
    }

ಹೊರಹೋಗು:
  /* Best effort attempt to reset the SELinux label used for new keyrings.  */
  ಆದರೆ (label_ಗಣ && ಲಿಖ (labelfd, "", 0) < 0)
    {
      /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
    }
  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ
get_uid_gid_from_def (runtime_spec_schema_config_schema *def, uid_t *uid, gid_t *gid)
{
  *uid = 0;
  *gid = 0;

  ಆದರೆ (def->process && def->process->user)
    {
      ಆದರೆ (def->process->user->uid)
        *uid = def->process->user->uid;
      ಆದರೆ (def->process->user->gid)
        *gid = def->process->user->gid;
    }
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ
get_mount_flags (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ current_flags, ಪೂರ್ಣಾಂಕ *ಸಿಕ್ಕಿತು, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *extra_flags, uint64_t *rec_clear, uint64_t *rec_ಗಣ)
{
  ಸ್ಥಿರಾಂಕ ರಚನೆ ಪ್ರಸರಣ_flags_s *prop;

  prop = libcrun_str2mount_flags (name);

  ಆದರೆ (ಸಿಕ್ಕಿತು)
    *ಸಿಕ್ಕಿತು = prop ? 1 : 0;

  ಆದರೆ (prop == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  ಆದರೆ (prop->extra_flags & OPTION_RECURSIVE)
    {
      ಆದರೆ (rec_clear && prop->clear)
        *rec_clear |= prop->flags;

      ಆದರೆ (rec_ಗಣ && ! prop->clear)
        *rec_ಗಣ |= prop->flags;
    }

  ಆದರೆ (extra_flags)
    *extra_flags |= prop->extra_flags;

  ಆದರೆ (prop->clear)
    ಹಿಂತಿರುಗಿ current_flags & ~prop->flags;

  ಹಿಂತಿರುಗಿ current_flags | prop->flags;
}

/* Whether any of the mount options sets or clears a mount flag, other
   than MS_BIND, MS_REC and the propagation ones.  */
ಸ್ಥಿರ bool
has_mount_flag_options (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  ಗಾಗಿ (i = 0; i < mount->options_len; i++)
    {
      ಸ್ಥಿರಾಂಕ ರಚನೆ ಪ್ರಸರಣ_flags_s *prop;

      ಆದರೆ (mount->options[i] == ಖಾಲಿ)
        ಮುಂದುವರಿ;

      prop = libcrun_str2mount_flags (mount->options[i]);
      ಆದರೆ (prop && (prop->flags & ~(MS_BIND | ALL_PROPAGATIONS)))
        ಹಿಂತಿರುಗಿ true;
    }

  ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ
get_mount_flags_or_option (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ current_flags, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *extra_flags, ಅಕ್ಷರ **option, uint64_t *rec_clear, uint64_t *rec_ಗಣ)
{
  ಪೂರ್ಣಾಂಕ ಸಿಕ್ಕಿತು;
  __attribute__ ((ಬಳಸದ)) cleanup_free ಅಕ್ಷರ *prev = ಖಾಲಿ;
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

  /* A null element of the array of options in the configuration is parsed
     into a NULL, which is not an option to be honored.  */
  ಆದರೆ (name == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  flags = get_mount_flags (name, current_flags, &ಸಿಕ್ಕಿತು, extra_flags, rec_clear, rec_ಗಣ);
  ಆದರೆ (ಸಿಕ್ಕಿತು)
    {
      /* MS_SYNCHRONOUS and MS_DIRSYNC cannot be set through fsmount()
         attr_flags.  Pass them also as data so that fsconfig(SET_FLAG)
         can set SB_SYNCHRONOUS / SB_DIRSYNC on the superblock.  */
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ದತ್ತಾಂಶ_flag = ಖಾಲಿ;
      ಆದರೆ ((flags & MS_SYNCHRONOUS) && ! (current_flags & MS_SYNCHRONOUS))
        ದತ್ತಾಂಶ_flag = "sync";
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((flags & MS_DIRSYNC) && ! (current_flags & MS_DIRSYNC))
        ದತ್ತಾಂಶ_flag = "dirsync";

      ಆದರೆ (ದತ್ತಾಂಶ_flag)
        {
          prev = *option;
          ಆದರೆ (*option && **option)
            xasprintf (option, "%s,%s", *option, ದತ್ತಾಂಶ_flag);
          ಇಲ್ಲದಿದ್ದರೆ
            *option = xstrdup (ದತ್ತಾಂಶ_flag);
        }
      ಹಿಂತಿರುಗಿ flags;
    }

  prev = *option;
  ಆದರೆ (*option && **option)
    xasprintf (option, "%s,%s", *option, name);
  ಇಲ್ಲದಿದ್ದರೆ
    *option = xstrdup (name);

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
ಅಚ್ಚು_ಬೇರು (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *new_ಬೇರು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *put_old)
{
  ಹಿಂತಿರುಗಿ syscall (__NR_pivot_root, new_ಬೇರು, put_old);
}

ಸ್ಥಿರ ಶೂನ್ಯ
free_remount (ರಚನೆ remount_s *r)
{
  ಆದರೆ (r == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;
  ಆದರೆ (r->targetfd >= 0)
    close (r->targetfd);
  ಬಿಡುಗಡೆ (r->ದತ್ತಾಂಶ);
  ಬಿಡುಗಡೆ (r->ಗುರಿ);
  ಬಿಡುಗಡೆ (r);
}

ಸ್ಥಿರ ರಚನೆ remount_s *
make_remount (ಪೂರ್ಣಾಂಕ targetfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ದತ್ತಾಂಶ, ರಚನೆ remount_s *ಮುಂದಿನ)
{
  ರಚನೆ remount_s *ret = xmalloc (ಗಾತ್ರ (*ret));
  ret->ಗುರಿ = xstrdup (ಗುರಿ);
  ret->flags = flags;
  ret->ದತ್ತಾಂಶ = ದತ್ತಾಂಶ ? xstrdup (ದತ್ತಾಂಶ) : ಖಾಲಿ;
  ret->ಮುಂದಿನ = ಮುಂದಿನ;
  ret->targetfd = targetfd;
  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_remount (ಪೂರ್ಣಾಂಕ targetfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ದತ್ತಾಂಶ, libcrun_error_t *err)
{
  proc_fd_ಮಾರ್ಗ_t ಗುರಿ_ಬಫರ್;
  ಪೂರ್ಣಾಂಕ ret;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *real_ಗುರಿ = ಗುರಿ;

  ಆದರೆ (targetfd >= 0)
    {
      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಣ_flags = flags & ~(MS_REMOUNT | MS_BIND);
      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ clear_flags = REMOUNT_CLEAR_FLAGS & ~ಗಣ_flags;
      ret = do_mount_setattr (false, ಗುರಿ, targetfd, clear_flags, flags & ~MS_REMOUNT, err);
      ಆದರೆ (LIKELY (ret == 0))
        ಹಿಂತಿರುಗಿ 0;
      crun_error_release (err);

      get_proc_self_fd_ಮಾರ್ಗ (ಗುರಿ_ಬಫರ್, targetfd);
      real_ಗುರಿ = ಗುರಿ_ಬಫರ್;
    }

  /* Older kernels (seen on 4.18) fail with EINVAL if data is set when
     setting MS_RDONLY.  */
  ಆದರೆ (flags & (MS_REMOUNT | MS_RDONLY))
    ದತ್ತಾಂಶ = ಖಾಲಿ;

  ret = mount (ಖಾಲಿ, real_ಗುರಿ, ಖಾಲಿ, flags, ದತ್ತಾಂಶ);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ current_flags, remount_flags;
      ರಚನೆ statfs sfs;

      ret = statfs (real_ಗುರಿ, &sfs);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "statfs `%s`", real_ಗುರಿ);

      current_flags = mount_flags_from_statfs (sfs.f_flags);

      remount_flags = current_flags & (MS_NOSUID | MS_NODEV | MS_NOEXEC | MS_NOSYMFOLLOW);
      ಆದರೆ (! (flags & MOUNT_ATIME_FLAGS))
        remount_flags |= current_flags & MOUNT_ATIME_FLAGS;

      ಆದರೆ ((flags | remount_flags) != flags)
        {
          ret = mount (ಖಾಲಿ, real_ಗುರಿ, ಖಾಲಿ, flags | remount_flags, ದತ್ತಾಂಶ);
          ಆದರೆ (LIKELY (ret == 0))
            ಹಿಂತಿರುಗಿ 0;

          /* If it still fails and MS_RDONLY is present in the mount, try adding it.  */
          ಆದರೆ (current_flags & MS_RDONLY)
            ret = mount (ಖಾಲಿ, real_ಗುರಿ, ಖಾಲಿ, flags | remount_flags | MS_RDONLY, ದತ್ತಾಂಶ);
        }
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "remount `%s`", ಗುರಿ);
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
finalize_mounts (libcrun_container_t *container, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret = 0;
  ರಚನೆ remount_s *r = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->remounts;
  ಆಗ (r)
    {
      ರಚನೆ remount_s *ಮುಂದಿನ = r->ಮುಂದಿನ;

      /* Try mount_setattr() first to avoid the statfs+retry fallback.  */
      ಆದರೆ (r->targetfd >= 0 && (r->flags & MS_RDONLY))
        {
          /* Clear the flags which are not set, like do_remount does.  */
          ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಣ_flags = r->flags & ~(MS_REMOUNT | MS_BIND);
          ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ clear_flags = REMOUNT_CLEAR_FLAGS & ~ಗಣ_flags;

          ret = do_mount_setattr (false, r->ಗುರಿ, r->targetfd, clear_flags, r->flags & ~MS_REMOUNT, err);
          ಆದರೆ (LIKELY (ret == 0))
            {
              free_remount (r);
              r = ಮುಂದಿನ;
              ಮುಂದುವರಿ;
            }
          crun_error_release (err);
        }

      ret = do_remount (r->targetfd, r->ಗುರಿ, r->flags, r->ದತ್ತಾಂಶ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹೋಗು cleanup;

      free_remount (r);
      r = ಮುಂದಿನ;
    }

cleanup:
  ಆಗ (r)
    {
      ರಚನೆ remount_s *ಮುಂದಿನ = r->ಮುಂದಿನ;
      free_remount (r);
      r = ಮುಂದಿನ;
    }

  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->remounts = ಖಾಲಿ;
  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
open_mount_ಗುರಿ (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ_rel, libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfs;
  ಪೂರ್ಣಾಂಕ rootfsfd = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd;

  ಆದರೆ (rootfsfd < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid rootfs state");

  ಹಿಂತಿರುಗಿ ಸುರಕ್ಷಿತ_openat (rootfsfd, rootfs, ಗುರಿ_rel, O_ಮಾರ್ಗ | O_CLOEXEC, 0, err);
}

/* Attempt to open a mount of the specified type.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
fsopen_mount (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *type, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೂಲ_name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *labeltype, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *label, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ದತ್ತಾಂಶ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_NEW_MOUNT_API
  cleanup_close ಪೂರ್ಣಾಂಕ fsfd = -1;
  ಪೂರ್ಣಾಂಕ ret;

  fsfd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  ಆದರೆ (UNLIKELY (fsfd < 0))
    ಹಿಂತಿರುಗಿ fsfd;

  /* Best-effort: pseudo file systems (tmpfs, proc, ...) ignore "source",
     so a failure here is expected and harmless.  Real failures surface
     at FSCONFIG_CMD_CREATE below.  */
  (ಶೂನ್ಯ) syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, "source", ಮೂಲ_name ? ಮೂಲ_name : type, 0);

  ಆದರೆ (labeltype)
    {
      ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, labeltype, label, 0);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (ದತ್ತಾಂಶ && ದತ್ತಾಂಶ[0])
    {
      cleanup_free ಅಕ್ಷರ *ದತ್ತಾಂಶ_copy = xstrdup (ದತ್ತಾಂಶ);
      ಅಕ್ಷರ *saveptr = ಖಾಲಿ;
      ಅಕ್ಷರ *token;

      ಗಾಗಿ (token = strtok_r (ದತ್ತಾಂಶ_copy, ",", &saveptr); token; token = strtok_r (ಖಾಲಿ, ",", &saveptr))
        {
          ಅಕ್ಷರ *eq = strchr (token, '=');
          ಆದರೆ (eq)
            {
              *eq = '\0';

              ret = syscall_fsconfig (fsfd, FSCONFIG_SET_STRING, token, eq + 1, 0);
            }
          ಇಲ್ಲದಿದ್ದರೆ
            ret = syscall_fsconfig (fsfd, FSCONFIG_SET_FLAG, token, ಖಾಲಿ, 0);

          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }
    }

  ret = syscall_fsconfig (fsfd, FSCONFIG_CMD_CREATE, ಖಾಲಿ, ಖಾಲಿ, 0);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ syscall_fsmount (fsfd, FSMOUNT_CLOEXEC, 0);
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) type;
  (ಶೂನ್ಯ) ಮೂಲ_name;
  (ಶೂನ್ಯ) ದತ್ತಾಂಶ;
  errno = ENOSYS;
  ಹಿಂತಿರುಗಿ -1;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_procfd (ರಚನೆ ಖಾಸಗಿ_ದತ್ತಾಂಶ_s *ದತ್ತಾಂಶ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ fd;

  ಆದರೆ (ದತ್ತಾಂಶ->procfd >= 0)
    ಹಿಂತಿರುಗಿ ದತ್ತಾಂಶ->procfd;

  fd = fsopen_mount ("proc", ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ);
  ಆದರೆ (fd < 0)
    fd = open ("/proc", O_DIRECTORY | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `/proc`");

  ದತ್ತಾಂಶ->procfd = fd;
  ಹಿಂತಿರುಗಿ fd;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_old_ಬೇರು_fd (ರಚನೆ ಖಾಸಗಿ_ದತ್ತಾಂಶ_s *ದತ್ತಾಂಶ)
{
  ಆದರೆ (ದತ್ತಾಂಶ->old_ಬೇರು_fd >= 0)
    ಹಿಂತಿರುಗಿ ದತ್ತಾಂಶ->old_ಬೇರು_fd;

  ದತ್ತಾಂಶ->old_ಬೇರು_fd = open ("/", O_ಮಾರ್ಗ | O_CLOEXEC);
  ಹಿಂತಿರುಗಿ ದತ್ತಾಂಶ->old_ಬೇರು_fd;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
fs_move_mount_ಗಮ್ಯ (ಪೂರ್ಣಾಂಕ fd, ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_NEW_MOUNT_API
  ಆದರೆ (name)
    ಹಿಂತಿರುಗಿ syscall_move_mount (fd, "", dirfd, name, MOVE_MOUNT_F_EMPTY_PATH);

  ಹಿಂತಿರುಗಿ syscall_move_mount (fd, "", dirfd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) fd;
  (ಶೂನ್ಯ) dirfd;
  (ಶೂನ್ಯ) name;
  (ಶೂನ್ಯ) syscall_move_mount;
  errno = ENOSYS;
  ಹಿಂತಿರುಗಿ -1;
#ಆದರೆ_ಅಂತ್ಯ
}

ಎಣಿಕೆ
{
  /* Do not apply any label to the mount.  */
  LABEL_NONE = 0,
  /* Apply the label as a mount option.  */
  LABEL_MOUNT = 1,
  /* Apply the label using setxattr.  */
  LABEL_XATTR = 2,

  LABEL_ಮುಖವಾಡ = 0x0f,

  /* Do not defer a readonly remount — apply MS_RDONLY directly.  */
  MOUNT_NO_DEFERRED_REMOUNT = 0x10,

  /* Do not remount — keep the flags the mount already has.  */
  MOUNT_NO_REMOUNT = 0x20,
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_mount (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೂಲ, ಪೂರ್ಣಾಂಕ targetfd,
                     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fstype, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ mountflags,
                     ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ extra_flags, libcrun_error_t *err);

ಸ್ಥಿರ bool
mount_option_exists (runtime_spec_schema_defs_mount *mnt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *option)
{
  size_t i;
  ಗಾಗಿ (i = 0; i < mnt->options_len; i++)
    ಆದರೆ (mnt->options[i] != ಖಾಲಿ && strcmp (mnt->options[i], option) == 0)
      ಹಿಂತಿರುಗಿ true;
  ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ runtime_spec_schema_defs_mount *
ಕಂಡುಹಿಡಿ_mount_for (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *destination)
{
  size_t i;
  runtime_spec_schema_config_schema *def = container->container_def;

  ಗಾಗಿ (i = 0; i < def->mounts_len; i++)
    {
      ಆದರೆ (strcmp (def->mounts[i]->destination, destination) == 0)
        ಹಿಂತಿರುಗಿ def->mounts[i];
    }
  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
warn_tmpfs_fallback_once (ರಚನೆ ಖಾಸಗಿ_ದತ್ತಾಂಶ_s *ಖಾಸಗಿ_ದತ್ತಾಂಶ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *reason)
{
  ಆದರೆ (! ಖಾಸಗಿ_ದತ್ತಾಂಶ->maskdir_warned)
    {
      libcrun_warning ("Falling back to tmpfs for masked dirs (reason: %s)", reason);
      ಖಾಸಗಿ_ದತ್ತಾಂಶ->maskdir_warned = true;
    }
}

/* Get or create the cached shared empty directory for masked paths optimization.
 * Creates directory and FD once per container, caches /proc/self/fd path for fast mounting.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_shared_ಖಾಲಿಗಳು_dir_cached (libcrun_container_t *container, ಅಕ್ಷರ **proc_fd_ಮಾರ್ಗ, libcrun_error_t *err)
{
  ರಚನೆ ಖಾಸಗಿ_ದತ್ತಾಂಶ_s *ಖಾಸಗಿ_ದತ್ತಾಂಶ = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container);
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
  cleanup_free ಅಕ್ಷರ *ಖಾಲಿಗಳು_dir_ಮಾರ್ಗ = ಖಾಲಿ;
  proc_fd_ಮಾರ್ಗ_t fd_ಮಾರ್ಗ;
  ಪೂರ್ಣಾಂಕ ret;

  /* Fast path: return cached proc fd path if already set up */
  ಆದರೆ (ಖಾಸಗಿ_ದತ್ತಾಂಶ->maskdir_proc_ಮಾರ್ಗ != ಖಾಲಿ)
    {
      ಆದರೆ (proc_fd_ಮಾರ್ಗ)
        *proc_fd_ಮಾರ್ಗ = ಖಾಸಗಿ_ದತ್ತಾಂಶ->maskdir_proc_ಮಾರ್ಗ;
      ಹಿಂತಿರುಗಿ 0;
    }

  /* Slow path: create directory and cache everything once */
  ret = get_shared_ಖಾಲಿಗಳು_directory_ಮಾರ್ಗ (&ಖಾಲಿಗಳು_dir_ಮಾರ್ಗ, container->context->state_ಬೇರು, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  /* Open directory and cache FD (once per container) */
  fd = get_bind_mount (-1, ಖಾಲಿಗಳು_dir_ಮಾರ್ಗ, true, true, false, MS_PRIVATE, err);
  ಆದರೆ (fd < 0)
    ಹಿಂತಿರುಗಿ fd;

  get_self_fd_ಮಾರ್ಗ (fd_ಮಾರ್ಗ, fd);
  ಖಾಸಗಿ_ದತ್ತಾಂಶ->maskdir_proc_ಮಾರ್ಗ = xstrdup (fd_ಮಾರ್ಗ);

  ಖಾಸಗಿ_ದತ್ತಾಂಶ->maskdir_fd = fd;
  fd = -1; /* Don't auto-close */

  ಆದರೆ (proc_fd_ಮಾರ್ಗ)
    *proc_fd_ಮಾರ್ಗ = ಖಾಸಗಿ_ದತ್ತಾಂಶ->maskdir_proc_ಮಾರ್ಗ;
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ bool
is_dev_null (ಸ್ಥಿರಾಂಕ ರಚನೆ stat *st)
{
  /* 1:3 is the null device, see Documentation/admin-guide/devices.txt.  */
  ಹಿಂತಿರುಗಿ S_ISCHR (st->st_mode) && st->st_rdev == makedev (1, 3);
}

/* Check that fd refers to the null device.  Once the root is the container
   rootfs, /dev/null is whatever the rootfs provides there.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ensure_dev_null (ಪೂರ್ಣಾಂಕ fd, libcrun_error_t *err)
{
  ರಚನೆ stat st;

  ಆದರೆ (UNLIKELY (fstat (fd, &st) < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "stat `/dev/null`");

  ಆದರೆ (UNLIKELY (! is_dev_null (&st)))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "`/dev/null` is not the null device");

  ಹಿಂತಿರುಗಿ 0;
}

/* Whether FD is the root of a read-only tmpfs that can hold no inode
   other than its root, so it is safe to show in place of any directory.  */
ಸ್ಥಿರ bool
is_ಖಾಲಿಗಳು_ro_tmpfs (ಪೂರ್ಣಾಂಕ fd)
{
  ರಚನೆ statfs sfs;

  ಆದರೆ (fstatfs (fd, &sfs) < 0)
    ಹಿಂತಿರುಗಿ false;

  ಹಿಂತಿರುಗಿ sfs.f_type == TMPFS_MAGIC && (sfs.f_flags & ST_RDONLY) && sfs.f_files == 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
mount_masked_dir (libcrun_container_t *container, ಪೂರ್ಣಾಂಕ pathfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rel_ಮಾರ್ಗ, libcrun_error_t *err)
{
  ರಚನೆ ಖಾಸಗಿ_ದತ್ತಾಂಶ_s *ಖಾಸಗಿ_ದತ್ತಾಂಶ = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container);
  cleanup_close ಪೂರ್ಣಾಂಕ newfd = -1;
  libcrun_error_t tmp_err = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (ಖಾಸಗಿ_ದತ್ತಾಂಶ->maskdir_fd < 0)
    {
      ಆದರೆ (ಖಾಸಗಿ_ದತ್ತಾಂಶ->maskdir_bind_failed)
        ಹೋಗು fallback_ಗಮ್ಯ_tmpfs;

      ret = get_shared_ಖಾಲಿಗಳು_dir_cached (container, ಖಾಲಿ, &tmp_err);
      ಆದರೆ (ret < 0)
        {
          ಖಾಸಗಿ_ದತ್ತಾಂಶ->maskdir_bind_failed = true;
          warn_tmpfs_fallback_once (ಖಾಸಗಿ_ದತ್ತಾಂಶ, tmp_err->ಸಂದೇಶ);
          crun_error_release (&tmp_err);
          ಹೋಗು fallback_ಗಮ್ಯ_tmpfs;
        }
    }

  ಆದರೆ (ಖಾಸಗಿ_ದತ್ತಾಂಶ->maskdir_fd >= 0)
    {
      ಪೂರ್ಣಾಂಕ rootfsfd = ಖಾಸಗಿ_ದತ್ತಾಂಶ->rootfsfd;
      ರಚನೆ stat before, after;

      ret = fstat (ಖಾಸಗಿ_ದತ್ತಾಂಶ->maskdir_fd, &before);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fstat masked dir `%s`", rel_ಮಾರ್ಗ);

      ret = fs_move_mount_ಗಮ್ಯ (ಖಾಸಗಿ_ದತ್ತಾಂಶ->maskdir_fd, pathfd, ಖಾಲಿ);
      ಆದರೆ (LIKELY (ret == 0))
        {
          newfd = get_bind_mount (rootfsfd, rel_ಮಾರ್ಗ, true, true, false, MS_PRIVATE, err);
          ಆದರೆ (UNLIKELY (newfd < 0))
            ಹಿಂತಿರುಗಿ newfd;

          ret = fstat (newfd, &after);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fstat masked dir `%s`", rel_ಮಾರ್ಗ);

          ಆದರೆ (before.st_dev != after.st_dev || before.st_ino != after.st_ino)
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "race condition detected remounting masked path `/%s`", rel_ಮಾರ್ಗ);

          ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (ಖಾಸಗಿ_ದತ್ತಾಂಶ->maskdir_fd));
          ಖಾಸಗಿ_ದತ್ತಾಂಶ->maskdir_fd = newfd;
          newfd = -1;

          ಹಿಂತಿರುಗಿ 0;
        }

      /* tmp_err is NULL here: get_shared_empty_dir_cached() succeeded and did not set it. */
      crun_make_error (&tmp_err, errno, "move mount masked dir `%s`", rel_ಮಾರ್ಗ);

      ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (ಖಾಸಗಿ_ದತ್ತಾಂಶ->maskdir_fd));
      ಖಾಸಗಿ_ದತ್ತಾಂಶ->maskdir_fd = -1;
      ಖಾಸಗಿ_ದತ್ತಾಂಶ->maskdir_move_failed = true;
    }

  ಖಾಸಗಿ_ದತ್ತಾಂಶ->maskdir_bind_failed = true;
  warn_tmpfs_fallback_once (ಖಾಸಗಿ_ದತ್ತಾಂಶ, tmp_err ? tmp_err->ಸಂದೇಶ : "unknown error");
  crun_error_release (&tmp_err);

fallback_ಗಮ್ಯ_tmpfs:
  libcrun_debug ("using tmpfs fallback for %s", rel_ಮಾರ್ಗ);
  ret = do_mount (container, "tmpfs", pathfd, rel_ಮಾರ್ಗ, "tmpfs", MS_RDONLY, "nr_blocks=1,nr_inodes=1",
                  LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  /* Reuse this tmpfs for the other masked directories, so that they do not
     need a tmpfs instance each.  */
  ಆದರೆ (! ಖಾಸಗಿ_ದತ್ತಾಂಶ->maskdir_move_failed)
    {
      ret = get_bind_mount (ಖಾಸಗಿ_ದತ್ತಾಂಶ->rootfsfd, rel_ಮಾರ್ಗ, true, true, false, MS_PRIVATE, &tmp_err);
      ಆದರೆ (ret >= 0 && is_ಖಾಲಿಗಳು_ro_tmpfs (ret))
        ಖಾಸಗಿ_ದತ್ತಾಂಶ->maskdir_fd = ret;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (ret >= 0)
            ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (ret));
          ಖಾಸಗಿ_ದತ್ತಾಂಶ->maskdir_move_failed = true;
          crun_error_release (&tmp_err);
        }
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_masked_or_readonly_ಮಾರ್ಗ (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rel_ಮಾರ್ಗ, bool readonly, bool keep_flags,
                            libcrun_error_t *err)
{
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ mount_flags = 0;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfs;
  cleanup_close ಪೂರ್ಣಾಂಕ pathfd = -1;
  ರಚನೆ statfs sfs;
  ಪೂರ್ಣಾಂಕ ret;
  mode_t ಬಹುಲಕ;

  /* The paths come from linux.maskedPaths and linux.readonlyPaths, so a null
     entry in either array reaches us as a NULL pointer.  */
  ಆದರೆ (UNLIKELY (rel_ಮಾರ್ಗ == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, EINVAL, "invalid null path in `maskedPaths` or `readonlyPaths`");

  ಆದರೆ (rel_ಮಾರ್ಗ[0] == '/')
    rel_ಮಾರ್ಗ++;

  pathfd = ಸುರಕ್ಷಿತ_openat (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd, rootfs, rel_ಮಾರ್ಗ, O_ಮಾರ್ಗ | O_CLOEXEC, 0, err);
  ಆದರೆ (UNLIKELY (pathfd < 0))
    {
      errno = crun_error_get_errno (err);
      ಆದರೆ (errno != ENOENT && errno != EACCES)
        ಹಿಂತಿರುಗಿ pathfd;

      ಆದರೆ (errno == EACCES)
        libcrun_debug ("skipping inaccessible path `%s`", rel_ಮಾರ್ಗ);
      crun_error_release (err);
      ಹಿಂತಿರುಗಿ 0;
    }

  ಆದರೆ (readonly)
    {
      ಪೂರ್ಣಾಂಕ procfd = get_procfd (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container), err);
      proc_fd_ಮಾರ್ಗ_t ಮೂಲ_ಬಫರ್;
      cleanup_close ಪೂರ್ಣಾಂಕ mountfd = -1;

      ಆದರೆ (UNLIKELY (procfd < 0))
        ಹಿಂತಿರುಗಿ procfd;

      get_self_fd_ಮಾರ್ಗ (ಮೂಲ_ಬಫರ್, pathfd);

      /* Try open_tree + mount_setattr to apply MS_RDONLY atomically.
         Only when keep_flags is false, since keep_flags needs statfs
         to inherit parent mount flags.  */
      ಆದರೆ (! keep_flags)
        {
          mountfd = get_bind_mount (procfd, ಮೂಲ_ಬಫರ್, true, true, false, MS_PRIVATE, err);
          ಆದರೆ (mountfd >= 0)
            ret = fs_move_mount_ಗಮ್ಯ (mountfd, pathfd, ಖಾಲಿ);
          ಇಲ್ಲದಿದ್ದರೆ
            ret = -1;

          ಆದರೆ (ret < 0)
            crun_error_release (err);
        }

      ಆದರೆ (keep_flags || ret < 0)
        {
          mount_flags = MS_BIND | MS_PRIVATE | MS_RDONLY | MS_REC;
          ಆದರೆ (keep_flags)
            {
              cleanup_close ಪೂರ್ಣಾಂಕ sfd = openat (procfd, ಮೂಲ_ಬಫರ್, O_ಮಾರ್ಗ | O_CLOEXEC);
              ಆದರೆ (UNLIKELY (sfd < 0))
                ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "openat procfd for `%s`", rel_ಮಾರ್ಗ);

              ret = fstatfs (sfd, &sfs);
              ಆದರೆ (UNLIKELY (ret < 0))
                ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fstatfs `%s`", rel_ಮಾರ್ಗ);
              mount_flags = mount_flags | sfs.f_flags;

              /* Parent might contain MS_REMOUNT but the new readonly path is not
                 actually mounted.  Specifically in the case of /proc this will end
                 up with EINVAL therefore remove MS_REMOUNT if it is getting
                 inherited from the parent.  */
              mount_flags = mount_flags & ~MS_REMOUNT;
            }

          close_and_reset (&mountfd);
          mountfd = get_bind_mount (procfd, ಮೂಲ_ಬಫರ್, true, true, false, MS_PRIVATE, err);
          ಆದರೆ (mountfd >= 0)
            {
              ret = do_mount_setattr (true, rel_ಮಾರ್ಗ, mountfd, 0, mount_flags & ~(MS_BIND | MS_PRIVATE), err);
              ಆದರೆ (UNLIKELY (ret < 0))
                ಹಿಂತಿರುಗಿ ret;

              ret = fs_move_mount_ಗಮ್ಯ (mountfd, pathfd, ಖಾಲಿ);
              ಆದರೆ (UNLIKELY (ret < 0))
                ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "move mount for readonly path `%s`", rel_ಮಾರ್ಗ);
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              proc_fd_ಮಾರ್ಗ_t abs_ಮೂಲ;

              crun_error_release (err);
              get_proc_self_fd_ಮಾರ್ಗ (abs_ಮೂಲ, pathfd);
              ret = do_mount (container, abs_ಮೂಲ, pathfd, rel_ಮಾರ್ಗ, ಖಾಲಿ, mount_flags, ಖಾಲಿ,
                              LABEL_NONE, err);
              ಆದರೆ (UNLIKELY (ret < 0))
                ಹಿಂತಿರುಗಿ ret;
            }
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ret = get_file_type_fd (pathfd, &ಬಹುಲಕ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot stat `%s`", rel_ಮಾರ್ಗ);

      ಆದರೆ ((ಬಹುಲಕ & S_IFMT) == S_IFDIR)
        ret = mount_masked_dir (container, pathfd, rel_ಮಾರ್ಗ, err);
      ಇಲ್ಲದಿದ್ದರೆ
        {
          cleanup_close ಪೂರ್ಣಾಂಕ nullfd = -1;
          cleanup_close ಪೂರ್ಣಾಂಕ mountfd = -1;

          /* If the root is already the container rootfs, this is its
             /dev/null: mask with it only if it is the null device.  */
          nullfd = open ("/dev/null", O_ಮಾರ್ಗ | O_CLOEXEC);
          ಆದರೆ (UNLIKELY (nullfd < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `/dev/null`");

          ret = ensure_dev_null (nullfd, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;

          mountfd = get_bind_mount (nullfd, "", false, true, false, MS_PRIVATE, err);
          ಆದರೆ (mountfd >= 0)
            ret = fs_move_mount_ಗಮ್ಯ (mountfd, pathfd, ಖಾಲಿ);

          ಆದರೆ (mountfd < 0 || ret < 0)
            {
              proc_fd_ಮಾರ್ಗ_t null_ಮಾರ್ಗ;

              crun_error_release (err);
              get_proc_self_fd_ಮಾರ್ಗ (null_ಮಾರ್ಗ, nullfd);
              ret = do_mount (container, null_ಮಾರ್ಗ, pathfd, rel_ಮಾರ್ಗ, ಖಾಲಿ, MS_BIND | MS_RDONLY, ಖಾಲಿ,
                              LABEL_MOUNT | MOUNT_NO_DEFERRED_REMOUNT, err);
            }
        }
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
get_selinux_context_type (libcrun_container_t *container, libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *context_type;

  context_type = ಕಂಡುಹಿಡಿ_annotation (container, "run.oci.mount_context_type");
  ಆದರೆ (context_type)
    {
      ಆದರೆ (strcmp (context_type, "context") != 0
          && strcmp (context_type, "fscontext") != 0
          && strcmp (context_type, "defcontext") != 0
          && strcmp (context_type, "rootcontext") != 0)
        {
          crun_make_error (err, 0, "invalid mount context type `%s` (must be one of: context, fscontext, defcontext, rootcontext)", context_type);
          ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }
      ಹಿಂತಿರುಗಿ context_type;
    }

  ಹಿಂತಿರುಗಿ "context";
}

/* improve the error message on mount(2) failures.  It always returns the original ret.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
diagnose_mount_failure (ಪೂರ್ಣಾಂಕ ret, libcrun_error_t *err, libcrun_container_t *container,
                        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fstype)
{
  ಆದರೆ (fstype && strcmp (fstype, "cgroup2") == 0 && get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->unified_cgroup_ಮಾರ್ಗ)
    {
      ಆದರೆ (access (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->unified_cgroup_ಮಾರ್ಗ, R_OK) < 0 && errno == EACCES)
        {
          /* The current cgroup is not accessible.  */
          crun_error_wrap (err, "the current cgroup is not accessible (too restrictive umask?)");
          ಹಿಂತಿರುಗಿ ret;
        }
    }
  ಹಿಂತಿರುಗಿ ret;
}

/* When creating a fresh sysfs mount fails inside a user namespace, fall back to
   bind mounting the host /sys.  Returns 0 when the fallback handled the mount,
   > 0 when it does not apply (the caller must report the original failure),
   < 0 on error.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
sysfs_userns_fallback (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ,
                       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *real_ಗುರಿ, ಪೂರ್ಣಾಂಕ targetfd,
                       ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ mountflags, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fstype,
                       libcrun_error_t *err)
{
  runtime_spec_schema_defs_mount *cgroup_mount;
  cleanup_close ಪೂರ್ಣಾಂಕ mountfd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ sysfd = -1;
  ಪೂರ್ಣಾಂಕ sys_old_ಬೇರು_fd = -1;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (! ((mountflags & MS_RDONLY) && targetfd > 0 && fstype && strcmp (fstype, "sysfs") == 0))
    ಹಿಂತಿರುಗಿ 1;

  /* If we are running in an user namespace, just bind mount /sys if creating
     sysfs failed.  */
  ret = check_running_ಒಳ_user_namespace (err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (ret == 0)
    ಹಿಂತಿರುಗಿ 1;

  cgroup_mount = ಕಂಡುಹಿಡಿ_mount_for (container, "/sys/fs/cgroup");
  sys_old_ಬೇರು_fd = get_old_ಬೇರು_fd (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container));

  /* The bind brings the host mounts below /sys along.  The later do_remount
     only affects the top mount, so make the whole tree read-only here.  */
  ಆದರೆ (sys_old_ಬೇರು_fd >= 0)
    mountfd = get_bind_mount (sys_old_ಬೇರು_fd, "sys", true, true, false, MS_PRIVATE, err);
  ಆದರೆ (mountfd < 0)
    {
      crun_error_release (err);
      mountfd = get_bind_mount (AT_FDCWD, "/sys", true, true, false, MS_PRIVATE, err);
    }

  ಆದರೆ (cgroup_mount == ಖಾಲಿ)
    {
      ಆದರೆ (mountfd >= 0)
        {
          ret = fs_move_mount_ಗಮ್ಯ (mountfd, targetfd, ಖಾಲಿ);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "move mount `/sys` to `%s`", real_ಗುರಿ);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          crun_error_release (err);
          ಆದರೆ (sys_old_ಬೇರು_fd >= 0)
            {
              cleanup_free ಅಕ್ಷರ *sys_ಮಾರ್ಗ = ಖಾಲಿ;
              proc_fd_ಮಾರ್ಗ_t sys_ಬೇರು_ಮಾರ್ಗ;

              get_proc_self_fd_ಮಾರ್ಗ (sys_ಬೇರು_ಮಾರ್ಗ, sys_old_ಬೇರು_fd);
              xasprintf (&sys_ಮಾರ್ಗ, "%s/sys", sys_ಬೇರು_ಮಾರ್ಗ);
              ret = mount (sys_ಮಾರ್ಗ, real_ಗುರಿ, ಖಾಲಿ, MS_BIND | MS_REC, ಖಾಲಿ);
            }
          ಇಲ್ಲದಿದ್ದರೆ
            ret = mount ("/sys", real_ಗುರಿ, ಖಾಲಿ, MS_BIND | MS_REC, ಖಾಲಿ);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "bind mount `/sys` from the host");
        }

      sysfd = open_mount_ಗುರಿ (container, ಗುರಿ, err);
      ಆದರೆ (UNLIKELY (sysfd < 0))
        ಹಿಂತಿರುಗಿ sysfd;

      ret = do_remount (sysfd, ಗುರಿ,
                        MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                        ಖಾಲಿ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ಹಿಂತಿರುಗಿ do_masked_or_readonly_ಮಾರ್ಗ (container, "/sys/fs/cgroup", true, false, err);
    }

  ಆದರೆ (UNLIKELY (mountfd < 0))
    ಹಿಂತಿರುಗಿ mountfd;

  ret = fs_move_mount_ಗಮ್ಯ (mountfd, targetfd, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "move mount to `%s`", real_ಗುರಿ);

  sysfd = open_mount_ಗುರಿ (container, ಗುರಿ, err);
  ಆದರೆ (UNLIKELY (sysfd < 0))
    ಹಿಂತಿರುಗಿ sysfd;

  ret = do_remount (sysfd, ಗುರಿ,
                    MS_REMOUNT | MS_BIND | (mountflags & ~ALL_PROPAGATIONS),
                    ಖಾಲಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (mount_option_exists (cgroup_mount, "ro") || mount_option_exists (cgroup_mount, "rro"))
    {
      ret = do_masked_or_readonly_ಮಾರ್ಗ (container, "/sys/fs/cgroup", true, false, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ 0;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_FGETXATTR
/* Best-effort application of the SELinux security.selinux xattr to a
   freshly mounted target when LABEL_XATTR labeling is requested.  */
ಸ್ಥಿರ ಶೂನ್ಯ
apply_selinux_xattr (libcrun_container_t *container, ಪೂರ್ಣಾಂಕ fd, ಪೂರ್ಣಾಂಕ extra_flags, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *label)
{
  libcrun_error_t tmp_err = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ procfd;

  ಆದರೆ ((extra_flags & LABEL_ಮುಖವಾಡ) != LABEL_XATTR)
    ಹಿಂತಿರುಗಿ;

  /* Best-effort: swallow any error so it does not leak into the caller.  */
  procfd = get_procfd (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container), &tmp_err);
  ಆದರೆ (procfd >= 0)
    {
      proc_fd_ಮಾರ್ಗ_t proc_self_ಮಾರ್ಗ;
      cleanup_close ಪೂರ್ಣಾಂಕ xfd = -1;

      get_self_fd_ಮಾರ್ಗ (proc_self_ಮಾರ್ಗ, fd);
      xfd = openat (procfd, proc_self_ಮಾರ್ಗ, O_RDONLY | O_CLOEXEC);
      ಆದರೆ (xfd >= 0)
        (ಶೂನ್ಯ) fsetxattr (xfd, "security.selinux", label, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (label), 0);
    }
  crun_error_release (&tmp_err);
}
#ಆದರೆ_ಅಂತ್ಯ

/* After a mount replaced the rootfs (target is empty), the current
   targetfd sits underneath the new mount.  Re-enter the mount namespace
   and reopen the rootfs, refreshing both *fd and the cached rootfsfd.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_mount_replace_rootfs (libcrun_container_t *container, ಪೂರ್ಣಾಂಕ *fd, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ procfd = get_procfd (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container), err);
  ಪೂರ್ಣಾಂಕ tmp;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (UNLIKELY (procfd < 0))
    ಹಿಂತಿರುಗಿ procfd;

  {
    cleanup_close ಪೂರ್ಣಾಂಕ nsfd = openat (procfd, "self/ns/mnt", O_RDONLY | O_CLOEXEC);
    ಆದರೆ (UNLIKELY (nsfd < 0))
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `/proc/self/ns/mnt`");

    ret = setns (nsfd, CLONE_NEWNS);
    ಆದರೆ (UNLIKELY (ret < 0))
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "setns `CLONE_NEWNS`");
  }

  close_and_reset (fd);
  *fd = open (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfs, O_ಮಾರ್ಗ | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (*fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "reopen rootfs after mount on /");

  tmp = dup (*fd);
  ಆದರೆ (UNLIKELY (tmp < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "dup");

  ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd));
  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd = tmp;

  ಹಿಂತಿರುಗಿ 0;
}

/* Apply the propagation flags (MS_SHARED, MS_SLAVE, ...) to a mount,
   preferring mount_setattr when a target fd is available and falling
   back to mount(2).  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
apply_ಪ್ರಸರಣ_flags (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ, ಪೂರ್ಣಾಂಕ targetfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *real_ಗುರಿ,
                         ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ mountflags, libcrun_error_t *err)
{
  bool ಪ್ರಸರಣ_ಮುಗಿದ = false;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (targetfd >= 0)
    {
      libcrun_error_t tmp_err = ಖಾಲಿ;
      ret = do_mount_setattr ((mountflags & MS_REC) != 0, ಗುರಿ, targetfd, 0, mountflags & ALL_PROPAGATIONS, &tmp_err);
      ಆದರೆ (LIKELY (ret == 0))
        ಪ್ರಸರಣ_ಮುಗಿದ = true;
      ಇಲ್ಲದಿದ್ದರೆ
        crun_error_release (&tmp_err);
    }

  ಆದರೆ (! ಪ್ರಸರಣ_ಮುಗಿದ)
    {
      ret = mount (ಖಾಲಿ, real_ಗುರಿ, ಖಾಲಿ, mountflags & ALL_PROPAGATIONS, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "set propagation for `%s`", ಗುರಿ);
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* Perform (or defer) the remount needed to apply bind/read-only/single
   instance flags.  A read-write remount and MOUNT_NO_DEFERRED_REMOUNT are
   done immediately; otherwise the remount is queued to run later.  *fd is
   consumed when the remount is deferred.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ವೇಳಾಪಟ್ಟಿ_or_do_remount (libcrun_container_t *container, ಪೂರ್ಣಾಂಕ *fd, ಪೂರ್ಣಾಂಕ targetfd,
                        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *real_ಗುರಿ,
                        bool single_instance, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ mountflags,
                        ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ extra_flags, libcrun_error_t *err)
{
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ remount_flags = MS_REMOUNT | (single_instance ? 0 : MS_BIND) | (mountflags & ~ALL_PROPAGATIONS);
  ರಚನೆ remount_s *r;

  ಆದರೆ ((remount_flags & MS_RDONLY) == 0 || (extra_flags & MOUNT_NO_DEFERRED_REMOUNT))
    ಹಿಂತಿರುಗಿ do_remount (*fd >= 0 ? *fd : targetfd, real_ಗುರಿ, remount_flags, ದತ್ತಾಂಶ, err);

  ಆದರೆ (*fd < 0)
    {
      *fd = dup (targetfd);
      ಆದರೆ (UNLIKELY (*fd < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "dup `%d`", targetfd);
    }

  /* The remount owns the fd.  */
  r = make_remount (get_and_reset (fd), ಗುರಿ, remount_flags, ದತ್ತಾಂಶ, get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->remounts);
  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->remounts = r;

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_mount (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೂಲ, ಪೂರ್ಣಾಂಕ targetfd,
          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fstype, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ mountflags, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ,
          ಪೂರ್ಣಾಂಕ extra_flags, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *ದತ್ತಾಂಶ_with_label = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ದತ್ತಾಂಶ_without_label = ದತ್ತಾಂಶ;
  proc_fd_ಮಾರ್ಗ_t ಗುರಿ_ಬಫರ್;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *context_type = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ ms_move_fd = -1;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *real_ಗುರಿ = ಗುರಿ;
  bool single_instance = false;
  bool needs_remount = false;
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *label = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret = 0;

  ಆದರೆ (container->container_def->linux && container->container_def->linux->mount_label)
    label = container->container_def->linux->mount_label;
  ಇಲ್ಲದಿದ್ದರೆ
    extra_flags = (extra_flags & ~LABEL_ಮುಖವಾಡ) | LABEL_NONE;

  ಆದರೆ (targetfd >= 0)
    {
      get_proc_self_fd_ಮಾರ್ಗ (ಗುರಿ_ಬಫರ್, targetfd);
      real_ಗುರಿ = ಗುರಿ_ಬಫರ್;
      needs_remount = true;
    }

  ಆದರೆ ((extra_flags & LABEL_ಮುಖವಾಡ) == LABEL_MOUNT)
    {
      context_type = get_selinux_context_type (container, err);
      ಆದರೆ (UNLIKELY (context_type == ಖಾಲಿ))
        ಹಿಂತಿರುಗಿ -1;

      ret = ಕೂಡಿಸು_selinux_mount_label (&ದತ್ತಾಂಶ_with_label, ದತ್ತಾಂಶ, label, context_type, err);
      ಆದರೆ (ret < 0)
        ಹಿಂತಿರುಗಿ ret;
      ದತ್ತಾಂಶ = ದತ್ತಾಂಶ_with_label;
    }

  ಆದರೆ (mountflags & MS_MOVE)
    {
      ಆದರೆ ((mountflags & MS_BIND) || fstype)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: cannot use MS_MOVE with MS_BIND or fstype");

      ret = mount (ಮೂಲ, real_ಗುರಿ, ಖಾಲಿ, MS_MOVE, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "move mount `%s` to `%s`", ಮೂಲ, ಗುರಿ);
      mountflags &= ~MS_MOVE;

      /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
      ms_move_fd = open_mount_ಗುರಿ (container, ಗುರಿ, err);
      ಆದರೆ (UNLIKELY (ms_move_fd < 0))
        ಹಿಂತಿರುಗಿ ms_move_fd;
      targetfd = ms_move_fd;
    }

  ಆದರೆ ((fstype && fstype[0]) || (mountflags & MS_BIND))
    {
      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags = mountflags & ~(ALL_PROPAGATIONS_NO_REC | MS_RDONLY);

      ret = -1;

      ಆದರೆ (targetfd >= 0)
        {
          ಆದರೆ (mountflags & MS_BIND)
            {
              cleanup_close ಪೂರ್ಣಾಂಕ bindfd = -1;
              libcrun_error_t tmp_err = ಖಾಲಿ;

              bindfd = get_bind_mount (AT_FDCWD, ಮೂಲ,
                                       (mountflags & MS_REC) != 0,
                                       false, false, 0, &tmp_err);
              ಆದರೆ (bindfd < 0)
                {
                  ಪೂರ್ಣಾಂಕ old_ಬೇರು_fd = get_old_ಬೇರು_fd (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container));

                  ಆದರೆ (old_ಬೇರು_fd >= 0 && ಮೂಲ && ಮೂಲ[0] == '/')
                    {
                      crun_error_release (&tmp_err);
                      bindfd = get_bind_mount (old_ಬೇರು_fd, (ಮೂಲ[1] == '\0') ? "." : ಮೂಲ + 1,
                                               (mountflags & MS_REC) != 0,
                                               false, false, 0, &tmp_err);
                    }
                }
              ಆದರೆ (bindfd >= 0)
                ret = fs_move_mount_ಗಮ್ಯ (bindfd, targetfd, ಖಾಲಿ);
              ಆದರೆ (ret < 0)
                crun_error_release (&tmp_err);
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              cleanup_close ಪೂರ್ಣಾಂಕ newfs = fsopen_mount (fstype, ಮೂಲ, context_type, label, ದತ್ತಾಂಶ_without_label);
              ಆದರೆ (newfs >= 0)
                ret = fs_move_mount_ಗಮ್ಯ (newfs, targetfd, ಖಾಲಿ);
            }
        }

      ಆದರೆ (ret < 0)
        {
          ret = mount (ಮೂಲ, real_ಗುರಿ, fstype, flags, ದತ್ತಾಂಶ);
          ಆದರೆ (ret < 0 && (mountflags & MS_BIND) && ಮೂಲ && ಮೂಲ[0] == '/')
            {
              ಪೂರ್ಣಾಂಕ old_ಬೇರು_fd = get_old_ಬೇರು_fd (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container));

              ಆದರೆ (old_ಬೇರು_fd >= 0)
                {
                  cleanup_free ಅಕ್ಷರ *proc_ಮೂಲ_buf = ಖಾಲಿ;
                  proc_fd_ಮಾರ್ಗ_t old_ಬೇರು_ಮಾರ್ಗ;

                  get_proc_self_fd_ಮಾರ್ಗ (old_ಬೇರು_ಮಾರ್ಗ, old_ಬೇರು_fd);
                  xasprintf (&proc_ಮೂಲ_buf, "%s/%s", old_ಬೇರು_ಮಾರ್ಗ, ಮೂಲ + 1);
                  ret = mount (proc_ಮೂಲ_buf, real_ಗುರಿ, fstype, flags, ದತ್ತಾಂಶ);
                }
            }
        }
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          ಪೂರ್ಣಾಂಕ saved_errno = errno;

          ret = sysfs_userns_fallback (container, ಗುರಿ, real_ಗುರಿ, targetfd,
                                       mountflags, fstype, err);
          ಆದರೆ (ret == 0)
            ಹಿಂತಿರುಗಿ 0;
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;

          /* ret > 0: the fallback does not apply, report the original failure.  */
          ret = crun_make_error (err, saved_errno, "mount `%s` to `%s`", ಮೂಲ, ಗುರಿ);

          ಹಿಂತಿರುಗಿ diagnose_mount_failure (ret, err, container, fstype);
        }

      ಆದರೆ (targetfd >= 0)
        {
          /* We need to reopen the path as the previous targetfd is underneath the new mountpoint.  */
          fd = open_mount_ಗುರಿ (container, ಗುರಿ, err);
          ಆದರೆ (UNLIKELY (fd < 0))
            ಹಿಂತಿರುಗಿ fd;

          /* We are replacing the rootfs, reopen it.  */
          ಆದರೆ (is_ಖಾಲಿಗಳು_string (ಗುರಿ))
            {
              ret = do_mount_replace_rootfs (container, &fd, err);
              ಆದರೆ (UNLIKELY (ret < 0))
                ಹಿಂತಿರುಗಿ ret;
            }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_FGETXATTR
          apply_selinux_xattr (container, fd, extra_flags, label);
#ಆದರೆ_ಅಂತ್ಯ

          targetfd = fd;

          /* The bind mount shadowed the original targetfd, so refresh
             real_target to point to the reopened mountpoint.  Otherwise the
             classic mount(2) fallbacks below (used when mount_setattr is not
             available, e.g. kernels < 5.12) operate on the shadowed path and
             fail with EINVAL.  */
          get_proc_self_fd_ಮಾರ್ಗ (ಗುರಿ_ಬಫರ್, targetfd);
          real_ಗುರಿ = ಗುರಿ_ಬಫರ್;
        }
    }

  ಆದರೆ (mountflags & ALL_PROPAGATIONS_NO_REC)
    {
      ret = apply_ಪ್ರಸರಣ_flags (ಗುರಿ, targetfd, real_ಗುರಿ, mountflags, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (mountflags & (MS_BIND | MS_RDONLY))
    needs_remount = true;

  ಆದರೆ (ದತ್ತಾಂಶ && fstype && strcmp (fstype, "proc") == 0)
    {
      single_instance = true;
      needs_remount = true;
    }

  ಆದರೆ (needs_remount && ! (extra_flags & MOUNT_NO_REMOUNT))
    {
      ret = ವೇಳಾಪಟ್ಟಿ_or_do_remount (container, &fd, targetfd, ಗುರಿ, real_ಗುರಿ,
                                    single_instance, mountflags, ದತ್ತಾಂಶ, extra_flags, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ
try_umount (ಪೂರ್ಣಾಂಕ targetfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ)
{
  proc_fd_ಮಾರ್ಗ_t ಗುರಿ_ಬಫರ್;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *real_ಗುರಿ = ಗುರಿ;

  ಆದರೆ (targetfd >= 0)
    {
      get_proc_self_fd_ಮಾರ್ಗ (ಗುರಿ_ಬಫರ್, targetfd);
      real_ಗುರಿ = ಗುರಿ_ಬಫರ್;
    }

  umount2 (real_ಗುರಿ, MNT_DETACH);
}

ಸ್ಥಿರ bool
container_has_cgroupns (libcrun_container_t *container)
{
  ಹಿಂತಿರುಗಿ get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->unshare_flags & CLONE_NEWCGROUP;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_mount_cgroup_v2 (libcrun_container_t *container, ಪೂರ್ಣಾಂಕ targetfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ,
                    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ mountflags, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ದತ್ತಾಂಶ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ;

  cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (cgroup_ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

  ret = do_mount (container, "cgroup2", targetfd, ಗುರಿ, "cgroup2", mountflags, ದತ್ತಾಂಶ, LABEL_NONE, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಪೂರ್ಣಾಂಕ saved_errno = crun_error_get_errno (err);
      ಆದರೆ (saved_errno == EPERM || saved_errno == EBUSY)
        {
          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *unified_cgroup_ಮಾರ್ಗ;
          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *src_cgroup;

          crun_error_release (err);

          ಆದರೆ (saved_errno == EBUSY)
            {
              /* If we got EBUSY it means the cgroup file system is already mounted at the targetfd and we
                 cannot stack another one on top of it.  Attempt mounting a tmpfs below the cgroup mount.  */

              ret = do_mount (container, "tmpfs", targetfd, ಗುರಿ, "tmpfs", MS_PRIVATE, "nr_blocks=1,nr_inodes=1", LABEL_NONE, err);
              ಆದರೆ (LIKELY (ret == 0))
                {
                  ret = do_mount (container, "cgroup2", targetfd, ಗುರಿ, "cgroup2", mountflags, ದತ್ತಾಂಶ, LABEL_NONE, err);
                  ಆದರೆ (LIKELY (ret == 0))
                    ಹಿಂತಿರುಗಿ ret;

                  /* Best-effort cleanup for the tmpfs, if it fails there is nothing to worry about.  */
                  try_umount (targetfd, ಗುರಿ);
                }

              /* If the previous method failed, fall back to bind mounting the current cgroup.  */
              crun_error_release (err);
            }

          unified_cgroup_ಮಾರ್ಗ = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->unified_cgroup_ಮಾರ್ಗ;

          /* If everything else failed, bind mount from the current cgroup.  */
          src_cgroup = unified_cgroup_ಮಾರ್ಗ && container_has_cgroupns (container) ? unified_cgroup_ಮಾರ್ಗ : CGROUP_ಬೇರು;
          ಹಿಂತಿರುಗಿ do_mount (container, src_cgroup, targetfd, ಗುರಿ, ಖಾಲಿ,
                           MS_BIND | mountflags, ಖಾಲಿ, LABEL_NONE, err);
        }

      ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_mount_cgroup_systemd_v1 (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೂಲ, ಪೂರ್ಣಾಂಕ targetfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ,
                            ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ mountflags, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *subsystem = "systemd";
  cleanup_free ಅಕ್ಷರ *subsystem_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ tmpfsdirfd = -1;

  mountflags = mountflags & ~MS_BIND;

  ret = do_mount (container, ಮೂಲ, targetfd, ಗುರಿ, "tmpfs", mountflags, "size=1024k", LABEL_NONE, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  /* Get a reference to the newly created cgroup directory.  */
  tmpfsdirfd = open_mount_ಗುರಿ (container, ಗುರಿ, err);
  ಆದರೆ (UNLIKELY (tmpfsdirfd < 0))
    ಹಿಂತಿರುಗಿ tmpfsdirfd;
  targetfd = tmpfsdirfd;

  ret = mkdirat (targetfd, subsystem, 0755);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "mkdirat `%s`", subsystem);

  ret = append_paths (&subsystem_ಮಾರ್ಗ, err, ಗುರಿ, subsystem, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  fd = openat (targetfd, subsystem, O_CLOEXEC | O_ಮಾರ್ಗ | O_DIRECTORY | O_NOFOLLOW);
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "openat `%s`", subsystem_ಮಾರ್ಗ);

  ಹಿಂತಿರುಗಿ do_mount (container, "cgroup", fd, subsystem_ಮಾರ್ಗ, "cgroup", mountflags, "none,name=systemd,xattr", LABEL_NONE,
                   err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_mount_cgroup_v1 (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೂಲ, ಪೂರ್ಣಾಂಕ targetfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ,
                    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ mountflags, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  cleanup_free ಅಕ್ಷರ *content = ಖಾಲಿ;
  ಅಕ್ಷರ *from;
  cleanup_close ಪೂರ್ಣಾಂಕ tmpfsdirfd = -1;
  ಅಕ್ಷರ *saveptr = ಖಾಲಿ;

  ret = do_mount (container, ಮೂಲ, targetfd, ಗುರಿ, "tmpfs", mountflags & ~MS_RDONLY, "size=1024k", LABEL_MOUNT,
                  err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  tmpfsdirfd = open_mount_ಗುರಿ (container, ಗುರಿ, err);
  ಆದರೆ (UNLIKELY (tmpfsdirfd < 0))
    ಹಿಂತಿರುಗಿ tmpfsdirfd;
  targetfd = tmpfsdirfd;

  {
    ಪೂರ್ಣಾಂಕ procfd = get_procfd (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container), err);
    ಆದರೆ (UNLIKELY (procfd < 0))
      ಹಿಂತಿರುಗಿ procfd;

    ret = read_all_file_at (procfd, SELF_CGROUP, &content, ಖಾಲಿ, err);
  }
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (UNLIKELY (content == ಖಾಲಿ || content[0] == '\0'))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid content from `%s`", PROC_SELF_CGROUP);

  ಗಾಗಿ (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ಖಾಲಿ, "\n", &saveptr))
    {
      cleanup_free ಅಕ್ಷರ *ಮೂಲ_ಮಾರ್ಗ = ಖಾಲಿ;
      cleanup_free ಅಕ್ಷರ *ಮೂಲ_subsystem = ಖಾಲಿ;
      cleanup_free ಅಕ್ಷರ *subsystem_ಮಾರ್ಗ = ಖಾಲಿ;
      ಅಕ್ಷರ *subpath, *subsystem, *subsystem_fqn, *it;
      cleanup_close ಪೂರ್ಣಾಂಕ subsystemfd = -1;
      subsystem = strchr (from, ':') + 1;
      subpath = strchr (subsystem, ':') + 1;
      *(subpath - 1) = '\0';

      ಆದರೆ (subsystem[0] == '\0')
        ಮುಂದುವರಿ;

      /* subsystem_fqn includes name= for named hierarchies.  */
      subsystem_fqn = subsystem;

      it = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು (subsystem, "name=");
      ಆದರೆ (it)
        subsystem = it + 5;

      ಆದರೆ (strcmp (subsystem, "net_prio,net_cls") == 0)
        subsystem = "net_cls,net_prio";
      ಆದರೆ (strcmp (subsystem, "cpuacct,cpu") == 0)
        subsystem = "cpu,cpuacct";

      ret = append_paths (&ಮೂಲ_subsystem, err, CGROUP_ಬೇರು, subsystem, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      /* if there is already a mount specified, do not add a default one.  */
      ಆದರೆ (ಕಂಡುಹಿಡಿ_mount_for (container, ಮೂಲ_subsystem) != ಖಾಲಿ)
        ಮುಂದುವರಿ;

      ret = append_paths (&ಮೂಲ_ಮಾರ್ಗ, err, ಮೂಲ_subsystem, subpath, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = append_paths (&subsystem_ಮಾರ್ಗ, err, ಗುರಿ, subsystem, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = mkdirat (targetfd, subsystem, 0755);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "mkdirat `%s`", subsystem_ಮಾರ್ಗ);

      subsystemfd = openat (targetfd, subsystem, O_CLOEXEC | O_ಮಾರ್ಗ | O_DIRECTORY | O_NOFOLLOW);
      ಆದರೆ (UNLIKELY (subsystemfd < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", subsystem_ಮಾರ್ಗ);

      ಆದರೆ (container_has_cgroupns (container))
        {
          ret = do_mount (container, ಮೂಲ_ಮಾರ್ಗ, subsystemfd, subsystem_ಮಾರ್ಗ, "cgroup", mountflags, subsystem_fqn,
                          LABEL_NONE, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            {
              ಆದರೆ (crun_error_get_errno (err) == ENOENT || crun_error_get_errno (err) == ENODEV)
                {
                  /* We are trying to mount a subsystem that is not present.  */
                  crun_error_release (err);
                  ಮುಂದುವರಿ;
                }
              ಹಿಂತಿರುಗಿ ret;
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ret = do_mount (container, ಮೂಲ_ಮಾರ್ಗ, subsystemfd, subsystem_ಮಾರ್ಗ, ಖಾಲಿ, MS_BIND | mountflags, ಖಾಲಿ,
                          LABEL_NONE, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            {
              ಆದರೆ (crun_error_get_errno (err) != ENOENT)
                ಹಿಂತಿರುಗಿ ret;

              crun_error_release (err);

              /* We might already be in a container.  Mount the source subsystem.  */
              ret = do_mount (container, ಮೂಲ_subsystem, subsystemfd, subsystem_ಮಾರ್ಗ, ಖಾಲಿ, MS_BIND | mountflags,
                              ಖಾಲಿ, LABEL_NONE, err);
              ಆದರೆ (UNLIKELY (ret < 0))
                {
                  /* If it still fails with ENOENT, ignore the error as the controller might have been
                     dropped and doesn't exist.  */
                  ಆದರೆ (crun_error_get_errno (err) != ENOENT)
                    ಹಿಂತಿರುಗಿ ret;

                  crun_error_release (err);
                }
            }
        }
    }

  ret = libcrun_cgroups_create_symlinks (targetfd, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_mount_cgroup (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೂಲ, ಪೂರ್ಣಾಂಕ targetfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ,
                 ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ mountflags, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ದತ್ತಾಂಶ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ;

  cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
  ಆದರೆ (UNLIKELY (cgroup_ಬಹುಲಕ < 0))
    ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

  ಆಯ್ಕೆ (cgroup_ಬಹುಲಕ)
    {
    ಸ್ಥಿತಿ CGROUP_ಬಹುಲಕ_UNIFIED:
      ಹಿಂತಿರುಗಿ do_mount_cgroup_v2 (container, targetfd, ಗುರಿ, mountflags, ದತ್ತಾಂಶ, err);
    ಸ್ಥಿತಿ CGROUP_ಬಹುಲಕ_LEGACY:
    ಸ್ಥಿತಿ CGROUP_ಬಹುಲಕ_HYBRID:
      ಹಿಂತಿರುಗಿ do_mount_cgroup_v1 (container, ಮೂಲ, targetfd, ಗುರಿ, mountflags, err);
    }

  ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "unknown cgroup mode `%d`", cgroup_ಬಹುಲಕ);
}

ರಚನೆ device_s needed_devs[] = { { "/dev/null", "c", 1, 3, 0666, 0, 0 },
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
  ರಚನೆ ಖಾಸಗಿ_ದತ್ತಾಂಶ_s *ಖಾಸಗಿ_ದತ್ತಾಂಶ = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container);
  size_t i;

  ಆದರೆ (ಖಾಸಗಿ_ದತ್ತಾಂಶ->needed_devs_fds)
    {
      ರಚನೆ libcrun_fd_map *dev_fds = ಖಾಸಗಿ_ದತ್ತಾಂಶ->needed_devs_fds;

      ಗಾಗಿ (i = 0; needed_devs[i].ಮಾರ್ಗ; i++)
        {
          ಆದರೆ (i < dev_fds->nfds && dev_fds->fds[i] >= 0)
            ಮುಂದುವರಿ;

          ಪೂರ್ಣಾಂಕ fd = syscall_open_ವೃಕ್ಷ (AT_FDCWD, needed_devs[i].ಮಾರ್ಗ,
                                      OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
          ಆದರೆ (fd >= 0 && i < dev_fds->nfds)
            dev_fds->fds[i] = fd;
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fd >= 0)
            close (fd);
        }
    }

  ಆದರೆ (ಖಾಸಗಿ_ದತ್ತಾಂಶ->notify_socket_ವೃಕ್ಷ_fd < 0
      && ಖಾಸಗಿ_ದತ್ತಾಂಶ->host_notify_socket_ಮಾರ್ಗ)
    {
      ಪೂರ್ಣಾಂಕ fd = syscall_open_ವೃಕ್ಷ (AT_FDCWD, ಖಾಸಗಿ_ದತ್ತಾಂಶ->host_notify_socket_ಮಾರ್ಗ,
                                  OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
      ಆದರೆ (fd >= 0)
        ಖಾಸಗಿ_ದತ್ತಾಂಶ->notify_socket_ವೃಕ್ಷ_fd = fd;
    }
}

/* Check if the specified path is a direct child of /dev.  If it is
 return a pointer to the basename.  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
relative_ಮಾರ್ಗ_under_dev (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
  ಆದರೆ (ಮಾರ್ಗ[0] != '/')
    ಹಿಂತಿರುಗಿ ಮಾರ್ಗ;
  ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ (ಮಾರ್ಗ, "/dev/", 5) == 0)
    {
      ಆದರೆ (strchr (ಮಾರ್ಗ + 5, '/'))
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
      ಹಿಂತಿರುಗಿ ಮಾರ್ಗ + 5;
    }
  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
mknod_and_ಗಣ_attrs (libcrun_container_t *container, ಪೂರ್ಣಾಂಕ dirfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ರಚನೆ device_s *device,
                     mode_t type, dev_t dev, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
  proc_fd_ಮಾರ್ಗ_t fd_ಮಾರ್ಗ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *what = "new";
  bool exists = false;
  ರಚನೆ stat st;
  ಪೂರ್ಣಾಂಕ procfd;
  ಪೂರ್ಣಾಂಕ ret;

  ret = mknodat (dirfd, name, device->ಬಹುಲಕ | type, dev);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಆದರೆ (errno != EEXIST)
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "mknodat `%s`", device->ಮಾರ್ಗ);
      /* The runtime spec requires an error if the existing file
         does not match the requested device, so check it below.  */
      exists = true;
      what = "existing";
    }

  /* Get a handle and verify that it matches the expected inode type
     and major:minor before operating on it.  */
  fd = openat (dirfd, name, O_ಮಾರ್ಗ | O_NOFOLLOW | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open %s device `%s`", what, device->ಮಾರ್ಗ);

  ret = fstat (fd, &st);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fstat %s device `%s`", what, device->ಮಾರ್ಗ);

  ಆದರೆ (UNLIKELY ((st.st_mode & S_IFMT) != type))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "%s device `%s` has incorrect file type %#o, expected %#o",
                            what, device->ಮಾರ್ಗ, st.st_mode & S_IFMT, type);

  ಆದರೆ (UNLIKELY (type != S_IFIFO && st.st_rdev != dev))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "%s device `%s` has incorrect major:minor %u:%u, expected %u:%u",
                            what, device->ಮಾರ್ಗ, major (st.st_rdev), minor (st.st_rdev),
                            major (dev), minor (dev));

  /* Leave the existing inode's mode and owner as is.  */
  ಆದರೆ (exists)
    ಹಿಂತಿರುಗಿ 0;

  /* fchmod(2) does not work on O_PATH fds, so go through /proc.  */
  procfd = get_procfd (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container), err);
  ಆದರೆ (UNLIKELY (procfd < 0))
    ಹಿಂತಿರುಗಿ procfd;

  get_self_fd_ಮಾರ್ಗ (fd_ಮಾರ್ಗ, fd);
  ret = fchmodat (procfd, fd_ಮಾರ್ಗ, device->ಬಹುಲಕ, 0);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fchmod `%s`", device->ಮಾರ್ಗ);

  ret = fchownat (fd, "", device->uid, device->gid, AT_ಖಾಲಿಗಳು_ಮಾರ್ಗ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fchown `%s`", device->ಮಾರ್ಗ);

  ಹಿಂತಿರುಗಿ 1;
}

ಪೂರ್ಣಾಂಕ
libcrun_create_dev (libcrun_container_t *container, ಪೂರ್ಣಾಂಕ devfd, ಪೂರ್ಣಾಂಕ srcfd,
                    ರಚನೆ device_s *device, bool binds, bool ensure_ಪೋಷಕ_dir,
                    libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  dev_t dev;
  mode_t type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fullname = device->ಮಾರ್ಗ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfs;
  ಆದರೆ (is_ಖಾಲಿಗಳು_string (fullname))
    ಹಿಂತಿರುಗಿ crun_make_error (err, EINVAL, "device path is empty");
  // Normalize the path by removing trailing slashes.
  cleanup_free ಅಕ್ಷರ *normalized_ಮಾರ್ಗ = xstrdup (fullname);
  consume_trailing_slashes (normalized_ಮಾರ್ಗ);
  ಆದರೆ (normalized_ಮಾರ್ಗ[0] == '\0')
    strcpy (normalized_ಮಾರ್ಗ, "/");
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rel_dev = relative_ಮಾರ್ಗ_under_dev (normalized_ಮಾರ್ಗ);

  ಆದರೆ (binds)
    {
      cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;

      ಆದರೆ (rel_dev)
        {
          fd = openat (devfd, rel_dev, O_NOFOLLOW | O_CLOEXEC | O_ಮಾರ್ಗ | O_NONBLOCK);
          ಆದರೆ (UNLIKELY (fd < 0))
            {
              ಆದರೆ (errno == ENOENT)
                fd = openat (devfd, rel_dev, O_CREAT | O_NOFOLLOW | O_CLOEXEC | O_NONBLOCK, 0700);

              ಆದರೆ (UNLIKELY (fd < 0))
                ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "openat `%s`", device->ಮಾರ್ಗ);
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rel_ಮಾರ್ಗ = consume_slashes (normalized_ಮಾರ್ಗ);

          fd = crun_ಸುರಕ್ಷಿತ_create_and_open_ref_at (false, get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd, rootfs, rel_ಮಾರ್ಗ, 0755, err);
          ಆದರೆ (UNLIKELY (fd < 0))
            ಹಿಂತಿರುಗಿ fd;
        }

      ಆದರೆ (srcfd >= 0)
        {
          ret = do_mount_setattr (false, normalized_ಮಾರ್ಗ, srcfd, 0, MS_NOSUID | MS_NOEXEC, err);
          ಆದರೆ (LIKELY (ret == 0))
            {
              ret = syscall_move_mount (srcfd, "", fd, "", MOVE_MOUNT_T_EMPTY_PATH | MOVE_MOUNT_F_EMPTY_PATH);
              ಆದರೆ (LIKELY (ret >= 0))
                ಹಿಂತಿರುಗಿ 0;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            crun_error_release (err);
        }

      {
        cleanup_close ಪೂರ್ಣಾಂಕ mountfd = -1;

        /* Try open_tree+mount_setattr to apply flags atomically.  */
        mountfd = get_bind_mount (-1, fullname, false, false, false, MS_PRIVATE, err);
        ಆದರೆ (mountfd >= 0)
          {
            ret = do_mount_setattr (false, normalized_ಮಾರ್ಗ, mountfd, 0, MS_NOSUID | MS_NOEXEC, err);
            ಆದರೆ (LIKELY (ret == 0))
              {
                ret = fs_move_mount_ಗಮ್ಯ (mountfd, fd, ಖಾಲಿ);
                ಆದರೆ (LIKELY (ret == 0))
                  ಹಿಂತಿರುಗಿ 0;
              }
            ಇಲ್ಲದಿದ್ದರೆ
              crun_error_release (err);
          }
        ಇಲ್ಲದಿದ್ದರೆ
          crun_error_release (err);
      }

      ret = do_mount (container, fullname, fd, normalized_ಮಾರ್ಗ, ಖಾಲಿ, MS_BIND | MS_PRIVATE | MS_NOEXEC | MS_NOSUID, ಖಾಲಿ, LABEL_MOUNT, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      dev = makedev (device->major, device->minor);

      /* Check whether the path is directly under /dev.  Since we already have an open fd to /dev and mknodat(2)
         fails when the destination already exists or is a symlink, it is safe to use it directly.
         If it is not a direct child, then first get a fd to the dirfd.
      */
      ಆದರೆ (rel_dev)
        {
          ret = mknod_and_ಗಣ_attrs (container, devfd, rel_dev, device, type, dev, err);
          ಆದರೆ (ret <= 0)
            ಹಿಂತಿರುಗಿ ret;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          cleanup_close ಪೂರ್ಣಾಂಕ dirfd = -1;
          cleanup_free ಅಕ್ಷರ *dirname = ಖಾಲಿ;
          ಅಕ್ಷರ *basename, *ಸಿಕ್ಕಿತು;

          dirname = xstrdup (normalized_ಮಾರ್ಗ);

          ಸಿಕ್ಕಿತು = strrchr (dirname, '/');
          ಆದರೆ (ಸಿಕ್ಕಿತು)
            *ಸಿಕ್ಕಿತು = '\0';

          basename = ಸಿಕ್ಕಿತು ? ಸಿಕ್ಕಿತು + 1 : dirname;

          ಆದರೆ (dirname[0] == '\0')
            {
              dirfd = dup (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd);
              ಆದರೆ (UNLIKELY (dirfd < 0))
                ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "dup fd for `%s`", rootfs);
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              dirfd = ಸುರಕ್ಷಿತ_openat (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd, rootfs, dirname, O_DIRECTORY | O_ಮಾರ್ಗ | O_CLOEXEC, 0, err);
              ಆದರೆ (dirfd < 0 && ensure_ಪೋಷಕ_dir)
                {
                  crun_error_release (err);

                  dirfd = crun_ಸುರಕ್ಷಿತ_create_and_open_ref_at (true, get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd, rootfs, dirname, 0755, err);
                }
              ಆದರೆ (UNLIKELY (dirfd < 0))
                ಹಿಂತಿರುಗಿ dirfd;
            }

          ret = mknod_and_ಗಣ_attrs (container, dirfd, basename, device, type, dev, err);
          ಆದರೆ (ret <= 0)
            ಹಿಂತಿರುಗಿ ret;
        }
    }
  ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ symlink_s
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ;
  bool force;
};

ಸ್ಥಿರ ರಚನೆ symlink_s symlinks[] = { { "/proc/self/fd", "fd", false },
                                       { "/proc/self/fd/0", "stdin", false },
                                       { "/proc/self/fd/1", "stdout", false },
                                       { "/proc/self/fd/2", "stderr", false },
                                       { "/proc/kcore", "core", false },
                                       { "pts/ptmx", "ptmx", true },
                                       { ಖಾಲಿ, ಖಾಲಿ, false } };

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
create_missing_devs (libcrun_container_t *container, bool binds, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  size_t i;
  ರಚನೆ device_s *it;
  cleanup_close ಪೂರ್ಣಾಂಕ devfd = -1;
  runtime_spec_schema_config_schema *def = container->container_def;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfs;
  cleanup_close_map ರಚನೆ libcrun_fd_map *dev_fds = ಖಾಲಿ;
  cleanup_close_map ರಚನೆ libcrun_fd_map *needed_fds = ಖಾಲಿ;

  dev_fds = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->dev_fds;
  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->dev_fds = ಖಾಲಿ;

  needed_fds = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->needed_devs_fds;
  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->needed_devs_fds = ಖಾಲಿ;

  ಆದರೆ (! def || ! def->linux)
    ಹಿಂತಿರುಗಿ 0;

  devfd = ಸುರಕ್ಷಿತ_openat (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd, rootfs, "dev", O_CLOEXEC | O_ಮಾರ್ಗ | O_DIRECTORY, 0, err);
  ಆದರೆ (UNLIKELY (devfd < 0))
    ಹಿಂತಿರುಗಿ devfd;

  ಗಾಗಿ (i = 0; i < def->linux->devices_len; i++)
    {
      ರಚನೆ device_s device = {
        def->linux->devices[i]->ಮಾರ್ಗ,
        def->linux->devices[i]->type,
        def->linux->devices[i]->major,
        def->linux->devices[i]->minor,
        def->linux->devices[i]->file_ಬಹುಲಕ,
        def->linux->devices[i]->uid,
        def->linux->devices[i]->gid,
      };

      ಆದರೆ (! def->linux->devices[i]->file_ಬಹುಲಕ_present)
        device.ಬಹುಲಕ = 0666;
      ret = libcrun_create_dev (container, devfd, dev_fds->fds[i], &device, binds, true, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಗಾಗಿ (it = needed_devs, i = 0; it->ಮಾರ್ಗ; it++, i++)
    {
      cleanup_close ಪೂರ್ಣಾಂಕ srcfd = (needed_fds && i < needed_fds->nfds) ? needed_fds->fds[i] : -1;

      /* make sure the parent directory exists only on the first iteration.  */
      ret = libcrun_create_dev (container, devfd, srcfd, it, binds, it == needed_devs, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
      ಆದರೆ (srcfd >= 0)
        needed_fds->fds[i] = -1;
    }

  ಗಾಗಿ (i = 0; symlinks[i].ಗುರಿ; i++)
    {
    retry_symlink:
      ret = symlinkat (symlinks[i].ಮಾರ್ಗ, devfd, symlinks[i].ಗುರಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          ಪೂರ್ಣಾಂಕ saved_errno = errno;

          ಆದರೆ (errno == EEXIST && ! symlinks[i].force)
            ಮುಂದುವರಿ;

          /* If the symlink should be forced, make sure to unlink any existing file at the same path.  */
          ಆದರೆ (errno == EEXIST)
            {
            retry_unlink:
              ret = unlinkat (devfd, symlinks[i].ಗುರಿ, 0);
              ಆದರೆ (ret < 0 && errno == EISDIR)
                ret = unlinkat (devfd, symlinks[i].ಗುರಿ, AT_REMOVEDIR);
              ಆದರೆ (ret < 0 && errno == EBUSY)
                {
                  cleanup_close ಪೂರ್ಣಾಂಕ tfd = openat (devfd, symlinks[i].ಗುರಿ, O_CLOEXEC | O_ಮಾರ್ಗ | O_NOFOLLOW);
                  ಆದರೆ (tfd >= 0)
                    {
                      proc_fd_ಮಾರ್ಗ_t procpath;

                      get_proc_self_fd_ಮಾರ್ಗ (procpath, tfd);
                      ಆದರೆ (umount2 (procpath, MNT_DETACH) == 0)
                        ಹೋಗು retry_unlink;
                    }
                }
              ಆದರೆ (ret == 0)
                ಹೋಗು retry_symlink;
            }
          ಹಿಂತಿರುಗಿ crun_make_error (err, saved_errno, "symlinkat `/dev/%s`", symlinks[i].ಗುರಿ);
        }
    }

  ಆದರೆ (container->container_def->process && container->container_def->process->terminal)
    {
      ret = create_file_if_missing_at (devfd, "console", 0620, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* Advance to the next path component, skipping empty and "." ones.  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
ಮುಂದಿನ_ಮಾರ್ಗ_component (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p, size_t *len)
{
  ಗಾಗಿ (;;)
    {
      p = consume_slashes (p);
      *len = strcspn (p, "/");
      ಆದರೆ (*len != 1 || p[0] != '.')
        ಹಿಂತಿರುಗಿ p;
      p++;
    }
}

/* Whether A and B are the same path, ignoring repeated, leading and
   trailing slashes, and "." components.  ".." components are compared
   as they are, not resolved lexically: if a path component before them
   is a symlink, the path may refer to something else entirely, and
   treating it as a duplicate would leave it unmasked.  */
ಸ್ಥಿರ bool
same_ಮಾರ್ಗ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *a, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *b)
{
  size_t la, lb;

  ಗಾಗಿ (;;)
    {
      a = ಮುಂದಿನ_ಮಾರ್ಗ_component (a, &la);
      b = ಮುಂದಿನ_ಮಾರ್ಗ_component (b, &lb);
      ಆದರೆ (la != lb || memcmp (a, b, la) != 0)
        ಹಿಂತಿರುಗಿ false;
      ಆದರೆ (la == 0)
        ಹಿಂತಿರುಗಿ true;
      a += la;
      b += lb;
    }
}

/* Whether paths[i] is the same as one of the paths before it.  */
ಸ್ಥಿರ bool
is_duplicate_ಮಾರ್ಗ (ಅಕ್ಷರ **paths, size_t i)
{
  size_t j;

  ಆದರೆ (paths[i] == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ false;

  ಗಾಗಿ (j = 0; j < i; j++)
    ಆದರೆ (paths[j] && same_ಮಾರ್ಗ (paths[j], paths[i]))
      ಹಿಂತಿರುಗಿ true;

  ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_masked_and_readonly_paths (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;
  runtime_spec_schema_config_schema *def = container->container_def;

  ಆದರೆ (! def || ! def->linux)
    ಹಿಂತಿರುಗಿ 0;

  ಗಾಗಿ (i = 0; i < def->linux->masked_paths_len; i++)
    {
      ಆದರೆ (is_duplicate_ಮಾರ್ಗ (def->linux->masked_paths, i))
        ಮುಂದುವರಿ;

      ret = do_masked_or_readonly_ಮಾರ್ಗ (container, def->linux->masked_paths[i], false, false, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಗಾಗಿ (i = 0; i < def->linux->readonly_paths_len; i++)
    {
      ret = do_masked_or_readonly_ಮಾರ್ಗ (container, def->linux->readonly_paths[i], true, true, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_ಅಚ್ಚು (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ oldrootfd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ newrootfd = -1;
  ಪೂರ್ಣಾಂಕ ret;

  oldrootfd = open ("/", O_DIRECTORY | O_ಮಾರ್ಗ | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (oldrootfd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `/`");

  newrootfd = open (rootfs, O_DIRECTORY | O_ಮಾರ್ಗ | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (newrootfd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", rootfs);

  ret = fchdir (newrootfd);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = ಅಚ್ಚು_ಬೇರು (".", ".");
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "pivot_root");

  ret = fchdir (oldrootfd);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fchdir `%s`", rootfs);

  ret = do_mount (container, ಖಾಲಿ, -1, ".", ಖಾಲಿ, MS_REC | MS_PRIVATE, ಖಾಲಿ, LABEL_MOUNT, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = umount2 (".", MNT_DETACH);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "umount2 oldroot");

  ಮಾಡು
    {
      ret = umount2 (".", MNT_DETACH);
      ಆದರೆ (ret < 0 && errno == EINVAL)
        ನಿಲ್ಲಿಸು;
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "umount2 oldroot");
  } ಆಗ (ret == 0);

  ret = chdir ("/");
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "chdir to newroot");

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
append_tmpfs_ಬಹುಲಕ_if_missing (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, ಅಕ್ಷರ **ದತ್ತಾಂಶ, libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfs;
  bool ಖಾಲಿಗಳು_ದತ್ತಾಂಶ = is_ಖಾಲಿಗಳು_string (*ದತ್ತಾಂಶ);
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
  ರಚನೆ stat st;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (*ದತ್ತಾಂಶ != ಖಾಲಿ && ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು (*ದತ್ತಾಂಶ, "mode="))
    ಹಿಂತಿರುಗಿ 0;

  fd = ಸುರಕ್ಷಿತ_openat (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd, rootfs, mount->destination, O_CLOEXEC | O_ಮಾರ್ಗ, 0, err);
  ಆದರೆ (fd < 0)
    {
      ಆದರೆ (crun_error_get_errno (err) != ENOENT)
        ಹಿಂತಿರುಗಿ fd;

      crun_error_release (err);
      ಹಿಂತಿರುಗಿ 0;
    }
  ret = fstat (fd, &st);
  ಆದರೆ (ret < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fstat `%s`", mount->destination);

  xasprintf (ದತ್ತಾಂಶ, "%s%smode=%o", ಖಾಲಿಗಳು_ದತ್ತಾಂಶ ? "" : *ದತ್ತಾಂಶ, ಖಾಲಿಗಳು_ದತ್ತಾಂಶ ? "" : ",", st.st_mode & 07777);
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_default_flags (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *destination, ಅಕ್ಷರ **ದತ್ತಾಂಶ)
{
  ಆದರೆ (strcmp (destination, "/proc") == 0)
    ಹಿಂತಿರುಗಿ 0;
  ಆದರೆ (strcmp (destination, "/dev/cgroup") == 0 || strcmp (destination, "/sys/fs/cgroup") == 0)
    {
      *ದತ್ತಾಂಶ = xstrdup ("none,name=");
      ಹಿಂತಿರುಗಿ MS_NOEXEC | MS_NOSUID | MS_STRICTATIME;
    }
  ಆದರೆ (strcmp (destination, "/dev") == 0)
    {
      *ದತ್ತಾಂಶ = xstrdup ("mode=755");
      ಹಿಂತಿರುಗಿ MS_NOEXEC | MS_STRICTATIME;
    }
  ಆದರೆ (strcmp (destination, "/dev/shm") == 0)
    {
      *ದತ್ತಾಂಶ = xstrdup ("mode=1777,size=65536k");
      ಹಿಂತಿರುಗಿ MS_NOEXEC | MS_NOSUID | MS_NODEV;
    }
  ಆದರೆ (strcmp (destination, "/dev/mqueue") == 0)
    ಹಿಂತಿರುಗಿ MS_NOEXEC | MS_NOSUID | MS_NODEV;
  ಆದರೆ (strcmp (destination, "/dev/pts") == 0)
    {
      ಆದರೆ (container->host_uid == 0)
        *ದತ್ತಾಂಶ = xstrdup ("newinstance,ptmxmode=0666,mode=620,gid=5");
      ಇಲ್ಲದಿದ್ದರೆ
        *ದತ್ತಾಂಶ = xstrdup ("newinstance,ptmxmode=0666,mode=620");
      ಹಿಂತಿರುಗಿ MS_NOEXEC | MS_NOSUID;
    }
  ಆದರೆ (strcmp (destination, "/sys") == 0)
    ಹಿಂತಿರುಗಿ MS_NOEXEC | MS_NOSUID | MS_NODEV;

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
get_force_cgroup_v1_annotation (libcrun_container_t *container)
{
  ಹಿಂತಿರುಗಿ ಕಂಡುಹಿಡಿ_annotation (container, "run.oci.systemd.force_cgroup_v1");
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
check_valid_no_follow_file_system (ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *destination, libcrun_error_t *err)
{
  ರಚನೆ statfs sfs;

  ಪೂರ್ಣಾಂಕ ret = fstatfs (fd, &sfs);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "statfs `%s`", destination);

  ಆಯ್ಕೆ (sfs.f_type)
    {
    ಸ್ಥಿತಿ PROC_SUPER_MAGIC:
    ಸ್ಥಿತಿ SYSFS_MAGIC:
    ಸ್ಥಿತಿ DEVPTS_SUPER_MAGIC:
      ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "override of the mount destination `/%s` is not allowed", destination);
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಸುರಕ್ಷಿತ_create_symlink (ಪೂರ್ಣಾಂಕ rootfsfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *destination, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ ಪೋಷಕ_dir_fd = -1;
  cleanup_free ಅಕ್ಷರ *ಬಫರ್ = ಖಾಲಿ;
  ಅಕ್ಷರ *part;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (is_ಖಾಲಿಗಳು_string (destination))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "empty destination for symlink `%s`", ಗುರಿ);

  ಬಫರ್ = xstrdup (destination);
  part = dirname (ಬಫರ್);

  ಪೋಷಕ_dir_fd = crun_ಸುರಕ್ಷಿತ_create_and_open_ref_at (true, rootfsfd, rootfs, part, 0755, err);
  ಆದರೆ (UNLIKELY (ಪೋಷಕ_dir_fd < 0))
    ಹಿಂತಿರುಗಿ ಪೋಷಕ_dir_fd;

  /* It is safe to reuse the buffer since it was created with xstrdup (destination).  */
  strcpy (ಬಫರ್, destination);
  part = basename (ಬಫರ್);

  ret = symlinkat (ಗುರಿ, ಪೋಷಕ_dir_fd, part);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      /* If it exists, check if it has the same content, if so just ignore the error.  */
      ಆದರೆ (errno == EEXIST)
        {
          cleanup_free ಅಕ್ಷರ *link = ಖಾಲಿ;
          ssize_t len;

          len = ಸುರಕ್ಷಿತ_readlinkat (ಪೋಷಕ_dir_fd, part, &link, 0, err);
          ಆದರೆ (UNLIKELY (len < 0))
            ಹಿಂತಿರುಗಿ len;

          ಆದರೆ ((((size_t) len) == ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಗುರಿ)) && ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ (link, ಗುರಿ, len) == 0)
            ಹಿಂತಿರುಗಿ 0;

          ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "symlink `%s` already exists with a different content", destination);
        }
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "symlinkat `%s`", ಗುರಿ);
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
handle_copy_symlink (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs,
                     runtime_spec_schema_defs_mount *mount,
                     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *link_ಗುರಿ, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *ಗುರಿ = ಖಾಲಿ;

  ಆದರೆ (link_ಗುರಿ == ಖಾಲಿ)
    {
      ಪೂರ್ಣಾಂಕ old_ಬೇರು_fd = get_old_ಬೇರು_fd (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container));
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೂಲ = mount->ಮೂಲ;
      ssize_t len;

      ಆದರೆ (old_ಬೇರು_fd >= 0 && ಮೂಲ[0] == '/')
        len = ಸುರಕ್ಷಿತ_readlinkat (old_ಬೇರು_fd, ಮೂಲ + 1, &ಗುರಿ, 0, err);
      ಇಲ್ಲದಿದ್ದರೆ
        len = ಸುರಕ್ಷಿತ_readlinkat (AT_FDCWD, ಮೂಲ, &ಗುರಿ, 0, err);
      ಆದರೆ (UNLIKELY (len < 0))
        ಹಿಂತಿರುಗಿ len;
      link_ಗುರಿ = ಗುರಿ;
    }

  ಹಿಂತಿರುಗಿ ಸುರಕ್ಷಿತ_create_symlink (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd, rootfs,
                              link_ಗುರಿ, mount->destination, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
prepare_sysfs_or_proc_mount (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ,
                             ಪೂರ್ಣಾಂಕ *targetfd, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  /* Enforce sysfs and proc to be mounted on a regular directory.  */
  ret = openat (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd, ಗುರಿ,
                O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಆದರೆ (errno == ENOENT)
        {
          ಆದರೆ (strchr (ಗುರಿ, '/'))
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid target `%s`: it must be mounted at the root", ಗುರಿ);

          ret = mkdirat (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd, ಗುರಿ, 0755);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "mkdirat `%s`", ಗುರಿ);

          /* Try opening it again.  */
          ret = openat (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd, ಗುರಿ,
                        O_CLOEXEC | O_NOFOLLOW | O_DIRECTORY);
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (errno == ENOTDIR)
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "the target `/%s` is invalid", ಗುರಿ);

      ಆದರೆ (ret < 0)
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", ಗುರಿ);
    }

  *targetfd = ret;
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
handle_tmpcopyup (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ,
                  ಪೂರ್ಣಾಂಕ copy_from_fd, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ destfd, ret;
  cleanup_close ಪೂರ್ಣಾಂಕ tmpfd = copy_from_fd;

  ಗುರಿ = consume_slashes (ಗುರಿ);

  ಆದರೆ (is_ಖಾಲಿಗಳು_string (ಗುರಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "tmpcopyup on `/` is not supported");

  destfd = ಸುರಕ್ಷಿತ_openat (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd, rootfs, ಗುರಿ,
                        O_CLOEXEC | O_DIRECTORY, 0, err);
  ಆದರೆ (UNLIKELY (destfd < 0))
    ಹಿಂತಿರುಗಿ crun_error_wrap (err, "open `%s` to write for tmpcopyup", ಗುರಿ);

  // copy_recursive_fd_to_fd closes tmpfd and destfd
  ret = copy_recursive_fd_ಗಮ್ಯ_fd (tmpfd, destfd, ಗುರಿ, ಗುರಿ, err);
  tmpfd = -1;
  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_mount_label_how (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *type, bool is_sysfs_or_proc)
{
  ಆದರೆ (is_sysfs_or_proc)
    ಹಿಂತಿರುಗಿ LABEL_NONE;

  ಆದರೆ (strcmp (type, "mqueue") == 0)
    ಹಿಂತಿರುಗಿ LABEL_XATTR;

  ಹಿಂತಿರುಗಿ LABEL_MOUNT;
}

/* The kernel does not let a mount inherited from a more privileged mount
   namespace lose its ro, nosuid, nodev and noexec flags in a user
   namespace.  The bind mounts prepared from the host are not subject to
   that, so, for a container with a user namespace, return the flags of
   SOURCE that are to be kept, i.e. those not explicitly cleared by the
   mount options.  */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ
get_bind_mount_locked_flags (ಪೂರ್ಣಾಂಕ ಮೂಲ_mountfd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೂಲ, runtime_spec_schema_defs_mount *mount)
{
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cleared = 0;
  ರಚನೆ statfs sfs;
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (ಮೂಲ_mountfd >= 0)
    ret = fstatfs (ಮೂಲ_mountfd, &sfs);
  ಇಲ್ಲದಿದ್ದರೆ
    ret = statfs (ಮೂಲ, &sfs);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ 0;

  ಗಾಗಿ (i = 0; i < mount->options_len; i++)
    {
      ಸ್ಥಿರಾಂಕ ರಚನೆ ಪ್ರಸರಣ_flags_s *prop;

      ಆದರೆ (mount->options[i] == ಖಾಲಿ)
        ಮುಂದುವರಿ;

      prop = libcrun_str2mount_flags (mount->options[i]);
      ಆದರೆ (prop == ಖಾಲಿ || (prop->extra_flags & OPTION_RECURSIVE))
        ಮುಂದುವರಿ;

      ಆದರೆ (prop->clear)
        cleared |= prop->flags;
      ಇಲ್ಲದಿದ್ದರೆ
        cleared &= ~prop->flags;
    }

  ಹಿಂತಿರುಗಿ sfs.f_flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC) & ~cleared;
}

/* If PATH is lexically inside the container rootfs ROOTFS, return the part
   of it relative to ROOTFS, otherwise NULL.  Such a bind mount source refers
   to what is in the container rootfs at the time of the mount, including the
   mounts done before it, so it cannot be opened beforehand.  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
get_ಮಾರ್ಗ_ಒಳ_rootfs (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
  size_t len;

  ಆದರೆ (rootfs == ಖಾಲಿ || ಮಾರ್ಗ == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

  len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (rootfs);
  ಆಗ (len > 0 && rootfs[len - 1] == '/')
    len--;

  /* With "/" as the rootfs, every path is in it, and there is nothing to
     do differently.  */
  ಆದರೆ (len == 0)
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

  ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ (ಮಾರ್ಗ, rootfs, len) != 0 || (ಮಾರ್ಗ[len] != '/' && ಮಾರ್ಗ[len] != '\0'))
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;

  ಹಿಂತಿರುಗಿ consume_slashes (ಮಾರ್ಗ + len);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
process_single_mount (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs,
                      runtime_spec_schema_defs_mount *mount,
                      ರಚನೆ libcrun_fd_map *mount_fds, size_t mount_ಸೂಚ್ಯಂಕ,
                      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *systemd_cgroup_v1, libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ = consume_slashes (mount->destination);
  cleanup_close ಪೂರ್ಣಾಂಕ ಮೂಲ_mountfd = -1;
  cleanup_free ಅಕ್ಷರ *ದತ್ತಾಂಶ = ಖಾಲಿ;
  ಅಕ್ಷರ *type;
  ಅಕ್ಷರ *ಮೂಲ;
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags = 0;
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ extra_flags = 0;
  mode_t src_ಬಹುಲಕ = S_IFDIR;
  cleanup_close ಪೂರ್ಣಾಂಕ copy_from_fd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ targetfd = -1;
  bool mounted = false;
  bool is_sysfs_or_proc;
  uint64_t rec_clear = 0;
  uint64_t rec_ಗಣ = 0;
  ಪೂರ್ಣಾಂಕ no_remount = 0;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (mount_fds)
    ಮೂಲ_mountfd = get_and_reset (&(mount_fds->fds[mount_ಸೂಚ್ಯಂಕ]));

  type = mount->type;

  ಆದರೆ (mount->options == ಖಾಲಿ)
    flags = get_default_flags (container, mount->destination, &ದತ್ತಾಂಶ);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      size_t j;

      ಗಾಗಿ (j = 0; j < mount->options_len; j++)
        flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &ದತ್ತಾಂಶ, &rec_clear, &rec_ಗಣ);
    }

  ಆದರೆ (type == ಖಾಲಿ && (flags & MS_BIND) == 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid mount type for `%s`", mount->destination);

  ಆದರೆ ((flags & MS_BIND) && ಮೂಲ_mountfd < 0 && ! (extra_flags & OPTION_COPY_SYMLINK))
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rel_ಮೂಲ = get_ಮಾರ್ಗ_ಒಳ_rootfs (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->host_rootfs, mount->ಮೂಲ);

      ಆದರೆ (rel_ಮೂಲ)
        {
          ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪ್ರಸರಣ = (flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE)) ? 0 : MS_PRIVATE;

          ಮೂಲ_mountfd = get_bind_mount (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd, rel_ಮೂಲ, (flags & MS_REC) != 0,
                                           false, (extra_flags & OPTION_SRC_NOFOLLOW) != 0, ಪ್ರಸರಣ, err);
          ಆದರೆ (UNLIKELY (ಮೂಲ_mountfd < 0))
            ಹಿಂತಿರುಗಿ crun_error_wrap (err, "open mount source `%s`", mount->ಮೂಲ);
        }
    }

  ಆದರೆ ((flags & MS_BIND) && mount->ಮೂಲ && has_mount_flag_options (mount)
      && (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->unshare_flags & CLONE_NEWUSER))
    flags |= get_bind_mount_locked_flags (ಮೂಲ_mountfd, mount->ಮೂಲ, mount);

  ಆದರೆ (flags & MS_BIND)
    {
      /* A remount sets exactly the requested flags and clears any other
         one.  A bind mount without mount flag options (e.g. "bind",
         "rbind,rprivate") keeps the flags of the source, so it must not
         be remounted.  */
      ಆದರೆ (! has_mount_flag_options (mount))
        no_remount = MOUNT_NO_REMOUNT;

      ಆದರೆ (ಮಾರ್ಗ_is_slash_dev (mount->destination))
        get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->mount_dev_from_host = true;
      /* It is used only for error messages.  */
      type = "bind";
    }
  is_sysfs_or_proc = strcmp (type, "sysfs") == 0 || strcmp (type, "proc") == 0;

  ಆದರೆ (strcmp (type, "tmpfs") == 0)
    {
      ret = append_tmpfs_ಬಹುಲಕ_if_missing (container, mount, &ದತ್ತಾಂಶ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (mount->ಮೂಲ && (flags & MS_BIND))
    {
      proc_fd_ಮಾರ್ಗ_t proc_buf;
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = mount->ಮೂಲ;

      ಆದರೆ ((extra_flags & OPTION_COPY_SYMLINK) && (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_DEST_NOFOLLOW)))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "`copy-symlink` is mutually exclusive with `src-nofollow` and `dest-nofollow`");

      bool is_cached_symlink = (extra_flags & OPTION_COPY_SYMLINK)
                               && get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->copy_symlink_targets
                               && mount_ಸೂಚ್ಯಂಕ < get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->n_copy_symlink_targets
                               && get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->copy_symlink_targets[mount_ಸೂಚ್ಯಂಕ];

      /* When the source is a cached symlink, skip the pre-opened fd since
         get_bind_mount() followed the symlink and the fd points to the target.
         For regular files with copy-symlink, the pre-opened fd is still valid.  */
      ಆದರೆ (ಮೂಲ_mountfd >= 0 && ! is_cached_symlink)
        {
          get_self_fd_ಮಾರ್ಗ (proc_buf, ಮೂಲ_mountfd);
          ಮಾರ್ಗ = proc_buf;

          ret = get_file_type_at (ಮೂಲ_mountfd, &src_ಬಹುಲಕ, true, ಖಾಲಿ);
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_cached_symlink)
        {
          src_ಬಹುಲಕ = S_IFLNK;
          ret = 0;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಮಾರ್ಗ[0] == '/')
        {
          ಪೂರ್ಣಾಂಕ old_ಬೇರು_fd = get_old_ಬೇರು_fd (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container));
          bool nofollow = (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false;

          ಆದರೆ (old_ಬೇರು_fd >= 0)
            ret = get_file_type_at (old_ಬೇರು_fd, &src_ಬಹುಲಕ, nofollow, (ಮಾರ್ಗ[1] == '\0') ? ಖಾಲಿ : ಮಾರ್ಗ + 1);
          ಇಲ್ಲದಿದ್ದರೆ
            ret = get_file_type_at (AT_FDCWD, &src_ಬಹುಲಕ, nofollow, ಮಾರ್ಗ);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        ret = get_file_type (&src_ಬಹುಲಕ, (extra_flags & (OPTION_SRC_NOFOLLOW | OPTION_COPY_SYMLINK)) ? true : false, ಮಾರ್ಗ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot stat `%s`", ಮಾರ್ಗ);

      ಆದರೆ (S_ISLNK (src_ಬಹುಲಕ) && (extra_flags & OPTION_DEST_NOFOLLOW) && ಮೂಲ_mountfd < 0)
        {
          ಪೂರ್ಣಾಂಕ bind_dirfd = AT_FDCWD;
          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *bind_ಮೂಲ = mount->ಮೂಲ;
          ಪೂರ್ಣಾಂಕ old_ಬೇರು_fd = get_old_ಬೇರು_fd (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container));

          ಆದರೆ (old_ಬೇರು_fd >= 0 && mount->ಮೂಲ && mount->ಮೂಲ[0] == '/')
            {
              bind_dirfd = old_ಬೇರು_fd;
              bind_ಮೂಲ = (mount->ಮೂಲ[1] == '\0') ? "." : mount->ಮೂಲ + 1;
            }
          ret = get_bind_mount (bind_dirfd, bind_ಮೂಲ, true, true, extra_flags & OPTION_SRC_NOFOLLOW, MS_PRIVATE, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;

          ಮೂಲ_mountfd = ret;
        }
    }

  ಆದರೆ (S_ISLNK (src_ಬಹುಲಕ) && (extra_flags & OPTION_COPY_SYMLINK))
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cached_ಗುರಿ = ಖಾಲಿ;

      ಆದರೆ (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->copy_symlink_targets
          && mount_ಸೂಚ್ಯಂಕ < get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->n_copy_symlink_targets)
        cached_ಗುರಿ = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->copy_symlink_targets[mount_ಸೂಚ್ಯಂಕ];

      ret = handle_copy_symlink (container, rootfs, mount, cached_ಗುರಿ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      mounted = true;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_sysfs_or_proc)
    {
      ret = prepare_sysfs_or_proc_mount (container, ಗುರಿ, &targetfd, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      bool is_dir = S_ISDIR (src_ಬಹುಲಕ);

      ಆದರೆ (extra_flags & OPTION_DEST_NOFOLLOW)
        {
          /* If dest-nofollow is specified, expect the target to exist.  */
          ret = ಸುರಕ್ಷಿತ_openat (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd, rootfs, ಗುರಿ, O_ಮಾರ್ಗ | O_NOFOLLOW | O_CLOEXEC, 0, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
          targetfd = ret;

          ret = check_valid_no_follow_file_system (targetfd, ಗುರಿ, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
          ret = crun_ಸುರಕ್ಷಿತ_create_and_open_ref_at (is_dir, get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd, rootfs, ಗುರಿ, 0755, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
          targetfd = ret;
        }
    }

  ಆದರೆ (extra_flags & OPTION_TMPCOPYUP)
    {
      ಆದರೆ (strcmp (type, "tmpfs") != 0)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "tmpcopyup can be used only with tmpfs");

      /* targetfd is opened with O_PATH, reopen the fd so it can read.  */
      copy_from_fd = openat (targetfd, ".", O_CLOEXEC | O_RDONLY | O_DIRECTORY);
      ಆದರೆ (UNLIKELY (copy_from_fd < 0))
        {
          ಆದರೆ (errno != ENOTDIR)
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot reopen `%s`", ಗುರಿ);
        }
    }

  ಮೂಲ = mount->ಮೂಲ ? mount->ಮೂಲ : type;

  /* Check if there is already a mount for the requested file system.  */
  ಆದರೆ (! mounted && ಮೂಲ_mountfd >= 0)
    {
      ret = fs_move_mount_ಗಮ್ಯ (ಮೂಲ_mountfd, targetfd, ಖಾಲಿ);
      ಆದರೆ (LIKELY (ret == 0))
        {
          ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಉಳಿದ_flags = flags & ~MS_BIND;

          /* A remount is needed not only to set some flags, but also to
             clear the ones set on the source (e.g. "bind,dev" on a nodev
             source), in which case remaining_flags might be empty.  */
          ಆದರೆ (ಉಳಿದ_flags || has_mount_flag_options (mount))
            {
              ret = do_mount (container, ಖಾಲಿ, ಮೂಲ_mountfd, ಗುರಿ, ಖಾಲಿ, ಉಳಿದ_flags, ದತ್ತಾಂಶ, LABEL_NONE | no_remount, err);
              ಆದರೆ (UNLIKELY (ret < 0))
                ಹಿಂತಿರುಗಿ ret;
            }

          mounted = true;

          ಆದರೆ (is_ಖಾಲಿಗಳು_string (ಗುರಿ))
            {
              ಪೂರ್ಣಾಂಕ new_rootfsfd = open (rootfs, O_ಮಾರ್ಗ | O_CLOEXEC);
              ಆದರೆ (UNLIKELY (new_rootfsfd < 0))
                ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "reopen rootfs after mount on /");

              ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd));
              get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd = new_rootfsfd;
            }
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->delayed_pidns_fork && strcmp (type, "proc") == 0)
        {
          /* The process is not in the container PID namespace yet, so
             mount(2) would create a proc instance for the wrong one.  */
          ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "the proc mount `%s` was not opened in the target PID namespace", ಗುರಿ);
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->host_ಬೇರು_switched)
        {
          /* The host root is unreachable now -- even through a fd, since
             mount(2) requires the source to live in the current mount
             namespace.  The path-based fallback below would resolve SOURCE
             inside the container and bind the wrong tree.  */
          ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "move_mount `%s` to `%s`", ಮೂಲ, ಗುರಿ);
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (errno != ENOSYS)
        {
          /* ENOSYS only means the kernel lacks the new mount API.  Anything
             else is worth reporting: it is otherwise invisible, and it decides
             whether the mount is set up from the pre-opened fd or from the
             source path.  */
          libcrun_warning ("move_mount `%s` to `%s` failed, falling back to mount(2): %s",
                           ಮೂಲ, ಗುರಿ, strerror (errno));
        }
    }

  ಆದರೆ (! mounted)
    {
      ಆದರೆ (systemd_cgroup_v1 && strcmp (mount->destination, systemd_cgroup_v1) == 0)
        {
          /* Override the cgroup mount with a single named cgroup name=systemd.  */
          ret = do_mount_cgroup_systemd_v1 (container, ಮೂಲ, targetfd, ಗುರಿ, flags, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (type, "cgroup") == 0)
        {
          ret = do_mount_cgroup (container, ಮೂಲ, targetfd, ಗುರಿ, flags, ದತ್ತಾಂಶ, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಪೂರ್ಣಾಂಕ extra_flags = get_mount_label_how (type, is_sysfs_or_proc) | no_remount;

          ret = do_mount (container, ಮೂಲ, targetfd, ಗುರಿ, type, flags, ದತ್ತಾಂಶ, extra_flags, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }
    }

  ಆದರೆ (copy_from_fd >= 0)
    {
      // handle_tmpcopyup closes copy_from_fd
      ret = handle_tmpcopyup (container, rootfs, ಗುರಿ, copy_from_fd, err);
      copy_from_fd = -1;
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (rec_clear || rec_ಗಣ)
    {
      ಸ್ಥಿರಾಂಕ bool is_dir = S_ISDIR (src_ಬಹುಲಕ);
      cleanup_close ಪೂರ್ಣಾಂಕ dfd = -1;

      dfd = ಸುರಕ್ಷಿತ_openat (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd, rootfs, ಗುರಿ, O_RDONLY | O_ಮಾರ್ಗ | O_CLOEXEC | (is_dir ? O_DIRECTORY : 0), 0, err);
      ಆದರೆ (UNLIKELY (dfd < 0))
        ಹಿಂತಿರುಗಿ crun_error_wrap (err, "open mount target `/%s`", ಗುರಿ);

      ret = do_mount_setattr (true, ಗುರಿ, dfd, rec_clear, rec_ಗಣ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_mounts (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *systemd_cgroup_v1 = get_force_cgroup_v1_annotation (container);
  cleanup_close_map ರಚನೆ libcrun_fd_map *mount_fds = ಖಾಲಿ;
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  mount_fds = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->mount_fds;
  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->mount_fds = ಖಾಲಿ;

  ಗಾಗಿ (i = 0; i < def->mounts_len; i++)
    {
      ret = process_single_mount (container, rootfs, def->mounts[i], mount_fds, i, systemd_cgroup_v1, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಹಿಂತಿರುಗಿ 0;
}

/*
 * libcrun_container_do_bind_mount
 *
 *  Allows external plugins and handlers to perform bind `mounts` on container.
 *  returns: 0 if successful anything else states `error` and configures `err` with relevant error.
 */
ಪೂರ್ಣಾಂಕ
libcrun_container_do_bind_mount (libcrun_container_t *container, ಅಕ್ಷರ *mount_ಮೂಲ, ಅಕ್ಷರ *mount_destination, ಅಕ್ಷರ **mount_options, size_t mount_options_len, libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ = consume_slashes (mount_destination);
  cleanup_free ಅಕ್ಷರ *ದತ್ತಾಂಶ = ಖಾಲಿ;
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags = 0;
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ extra_flags = 0;
  cleanup_close ಪೂರ್ಣಾಂಕ targetfd = -1;
  ಪೂರ್ಣಾಂಕ is_dir = 1;
  uint64_t rec_clear = 0;
  uint64_t rec_ಗಣ = 0;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfs;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ ((get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd < 0) || (rootfs == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid rootfs state while performing bind mount from external plugin or handler");

  ಆದರೆ (mount_options == ಖಾಲಿ)
    flags = get_default_flags (container, mount_destination, &ದತ್ತಾಂಶ);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      size_t j;

      ಗಾಗಿ (j = 0; j < mount_options_len; j++)
        flags |= get_mount_flags_or_option (mount_options[j], flags, &extra_flags, &ದತ್ತಾಂಶ, &rec_clear, &rec_ಗಣ);
    }

  ಆದರೆ (ಮಾರ್ಗ_is_slash_dev (mount_destination))
    get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->mount_dev_from_host = true;

  ಆದರೆ (mount_ಮೂಲ && (flags & MS_BIND))
    {
      is_dir = crun_dir_p (mount_ಮೂಲ, false, err);
      ಆದರೆ (UNLIKELY (is_dir < 0))
        ಹಿಂತಿರುಗಿ is_dir;
    }

  /* Make sure any other directory/file is created and take a O_PATH reference to it.  */
  ret = crun_ಸುರಕ್ಷಿತ_create_and_open_ref_at (is_dir, get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd, rootfs, ಗುರಿ, 0755, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  targetfd = ret;

  ret = do_mount (container, mount_ಮೂಲ, targetfd, ಗುರಿ, "bind", flags, ದತ್ತಾಂಶ, LABEL_MOUNT, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ 0;
}

/*
  Open a fd to the NOTIFY_SOCKET end on the host.  If CONTAINER is NULL, then CONTEXT
  is used to retrieve the path to the socket.
*/
ಪೂರ್ಣಾಂಕ
get_notify_fd (libcrun_context_t *context, libcrun_container_t *container, ಪೂರ್ಣಾಂಕ *notify_socket_ಹೊರಹೋಗು, libcrun_error_t *err)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYSTEMD
  cleanup_close ಪೂರ್ಣಾಂಕ notify_fd = -1;
  cleanup_free ಅಕ್ಷರ *host_notify_socket_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *state_dir = ಖಾಲಿ;
  ಅಕ್ಷರ *host_ಮಾರ್ಗ = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (container && get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->host_notify_socket_ಮಾರ್ಗ)
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೋಷಕ_dir;

      ಪೋಷಕ_dir = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->host_notify_socket_ಮಾರ್ಗ;

      ret = append_paths (&host_notify_socket_ಮಾರ್ಗ, err, ಪೋಷಕ_dir, "notify", ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      host_ಮಾರ್ಗ = host_notify_socket_ಮಾರ್ಗ;
    }

  *notify_socket_ಹೊರಹೋಗು = -1;

  ಆದರೆ (host_ಮಾರ್ಗ == ಖಾಲಿ)
    {
      ret = libcrun_get_state_directory (&state_dir, context->state_ಬೇರು, context->id, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = append_paths (&host_notify_socket_ಮಾರ್ಗ, err, state_dir, "notify/notify", ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      host_ಮಾರ್ಗ = host_notify_socket_ಮಾರ್ಗ;
    }

  notify_fd = open_unix_domain_socket (host_ಮಾರ್ಗ, 1, err);
  ಆದರೆ (UNLIKELY (notify_fd < 0))
    ಹಿಂತಿರುಗಿ notify_fd;

  ಆದರೆ (UNLIKELY (chmod (host_ಮಾರ್ಗ, 0777) < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "chmod `%s`", host_ಮಾರ್ಗ);

#  ifdef HAVE_FGETXATTR
  ಆದರೆ (container && container->container_def->linux && container->container_def->linux->mount_label)
    {
      /* Ignore the error. The worst that can happen is that the container fails to notify it is ready.  */
      (ಶೂನ್ಯ) setxattr (host_ಮಾರ್ಗ, "security.selinux", container->container_def->linux->mount_label,
                       ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (container->container_def->linux->mount_label), 0);
    }
#  endif

  *notify_socket_ಹೊರಹೋಗು = get_and_reset (&notify_fd);
  ಹಿಂತಿರುಗಿ 1;
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) context;
  (ಶೂನ್ಯ) container;
  (ಶೂನ್ಯ) err;
  *notify_socket_ಹೊರಹೋಗು = -1;
  ಹಿಂತಿರುಗಿ 0;
#ಆದರೆ_ಅಂತ್ಯ
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYSTEMD
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_notify_socket (libcrun_container_t *container, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *notify_socket = container->context->notify_socket;
  cleanup_free ಅಕ್ಷರ *host_notify_socket_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *container_notify_socket_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *state_dir = ಖಾಲಿ;
  uid_t container_ಬೇರು_uid = -1;
  gid_t container_ಬೇರು_gid = -1;
  ಪೂರ್ಣಾಂಕ notify_socket_ವೃಕ್ಷ_fd;

  ಆದರೆ (notify_socket == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_ಬೇರು : ಖಾಲಿ),
                                     (container->context ? container->context->id : ಖಾಲಿ), err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = append_paths (&container_notify_socket_ಮಾರ್ಗ, err, "/", notify_socket, "notify", ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = append_paths (&host_notify_socket_ಮಾರ್ಗ, err, state_dir, "notify", ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = mkdir (host_notify_socket_ಮಾರ್ಗ, 0700);
  ಆದರೆ (ret < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "mkdir `%s`", host_notify_socket_ಮಾರ್ಗ);

  ಆದರೆ (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->unshare_flags & CLONE_NEWUSER)
    {
      get_ಬೇರು_ಒಳ_the_userns (container->container_def, 0, 0, &container_ಬೇರು_uid, &container_ಬೇರು_gid);
      ಆದರೆ (container_ಬೇರು_uid != ((uid_t) -1) && container_ಬೇರು_gid != ((gid_t) -1))
        {
          ret = chown (host_notify_socket_ಮಾರ್ಗ, container_ಬೇರು_uid, container_ಬೇರು_gid);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "chown `%s` to `%d:%d`", host_notify_socket_ಮಾರ್ಗ, container_ಬೇರು_uid, container_ಬೇರು_gid);
        }
    }

  notify_socket_ವೃಕ್ಷ_fd = syscall_open_ವೃಕ್ಷ (AT_FDCWD, host_notify_socket_ಮಾರ್ಗ, OPEN_TREE_CLONE | AT_RECURSIVE | OPEN_TREE_CLOEXEC);
  ಆದರೆ (notify_socket_ವೃಕ್ಷ_fd >= 0)
    /* open_tree worked */
    get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->notify_socket_ವೃಕ್ಷ_fd = notify_socket_ವೃಕ್ಷ_fd;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (errno == EPERM)
    /* this can happen when trying to run a rootless container; this function is called
       in the original namespace where the caller is _not_ CAP_SYS_ADMIN - in that case,
       do nothing, because the bind mount of host_notify_socket_path directly should succeed
       since it will be readable by the container user. */
    ;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (errno == ENOSYS)
    /* if open_tree(2) is not available, do nothing; we will try mount(2) in do_finalize_notify_socket */
    ;
  ಇಲ್ಲದಿದ್ದರೆ
    /* some other error */
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open_tree `%s`", host_notify_socket_ಮಾರ್ಗ);

  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->host_notify_socket_ಮಾರ್ಗ = host_notify_socket_ಮಾರ್ಗ;
  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->container_notify_socket_ಮಾರ್ಗ = container_notify_socket_ಮಾರ್ಗ;
  host_notify_socket_ಮಾರ್ಗ = container_notify_socket_ಮಾರ್ಗ = ಖಾಲಿ;
  ಹಿಂತಿರುಗಿ 0;
}
#ಆದರೆ_ಅಂತ್ಯ

/* Bind mount the notify socket directory into the rootfs.  It must run
   before pivot_root, while the host path is still reachable for the
   mount(2) fallback, and the destination is resolved inside the rootfs so
   that a symlink in the container image cannot point it elsewhere.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_finalize_notify_socket (libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  cleanup_free ಅಕ್ಷರ *host_notify_socket_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *container_notify_socket_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *container_notify_socket_ಮಾರ್ಗ_dir_alloc = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *container_notify_socket_ಮಾರ್ಗ_dir = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ notify_socket_ವೃಕ್ಷ_fd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ targetfd = -1;
  ಪೂರ್ಣಾಂಕ did_mount_with_move_mount = 0;

  host_notify_socket_ಮಾರ್ಗ = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->host_notify_socket_ಮಾರ್ಗ;
  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->host_notify_socket_ಮಾರ್ಗ = ಖಾಲಿ;

  container_notify_socket_ಮಾರ್ಗ = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->container_notify_socket_ಮಾರ್ಗ;
  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->container_notify_socket_ಮಾರ್ಗ = ಖಾಲಿ;

  ಆದರೆ (host_notify_socket_ಮಾರ್ಗ == ಖಾಲಿ || container_notify_socket_ಮಾರ್ಗ == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  ಆದರೆ (rootfs == ಖಾಲಿ)
    rootfs = "/";

  container_notify_socket_ಮಾರ್ಗ_dir_alloc = xstrdup (container_notify_socket_ಮಾರ್ಗ);
  container_notify_socket_ಮಾರ್ಗ_dir = consume_slashes (dirname (container_notify_socket_ಮಾರ್ಗ_dir_alloc));

  /* libcrun_set_mounts does not open the rootfs when there are no mounts.  */
  ಆದರೆ (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd < 0)
    {
      ret = open (rootfs, O_ಮಾರ್ಗ | O_CLOEXEC);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", rootfs);

      get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd = ret;
      get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfs = rootfs;
    }

  ret = crun_ಸುರಕ್ಷಿತ_create_and_open_ref_at (true, get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd, rootfs,
                                          container_notify_socket_ಮಾರ್ಗ_dir, 0755, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  targetfd = ret;

  notify_socket_ವೃಕ್ಷ_fd = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->notify_socket_ವೃಕ್ಷ_fd;
  /* the FD will be unconditionally closed at the end of this function due to cleanup_close above */
  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->notify_socket_ವೃಕ್ಷ_fd = -1;

  ಆದರೆ (notify_socket_ವೃಕ್ಷ_fd >= 0)
    {
      ret = fs_move_mount_ಗಮ್ಯ (notify_socket_ವೃಕ್ಷ_fd, targetfd, ಖಾಲಿ);
      ಆದರೆ (ret >= 0)
        {
          /* if move_mount(2) worked, make sure we don't try mount(2) */
          did_mount_with_move_mount = 1;

          /* the tree was cloned from the host mount namespace and is still in
             the peer group of its source, so make it private like the mount(2)
             fallback below does.  TARGETFD is now underneath the new mount,
             so reopen the target.  */
          close_and_reset (&targetfd);
          targetfd = open_mount_ಗುರಿ (container, container_notify_socket_ಮಾರ್ಗ_dir, err);
          ಆದರೆ (UNLIKELY (targetfd < 0))
            ಹಿಂತಿರುಗಿ targetfd;

          ret = do_mount (container, ಖಾಲಿ, targetfd, container_notify_socket_ಮಾರ್ಗ_dir, ಖಾಲಿ,
                          MS_REC | MS_PRIVATE, ಖಾಲಿ, LABEL_NONE, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (errno == ENOSYS)
        /* do nothing; we will try mount(2) next */
        ;
      ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "move_mount `%d` to `%s`", notify_socket_ವೃಕ್ಷ_fd,
                                container_notify_socket_ಮಾರ್ಗ_dir);
    }

  ಆದರೆ (! did_mount_with_move_mount)
    {
      ret = do_mount (container, host_notify_socket_ಮಾರ್ಗ, targetfd, container_notify_socket_ಮಾರ್ಗ_dir, ಖಾಲಿ,
                      MS_BIND | MS_REC | MS_PRIVATE, ಖಾಲಿ, LABEL_MOUNT, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
make_ಪೋಷಕ_mount_ಖಾಸಗಿ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ rootfsfd = -1;
  ರಚನೆ stat prev_st;
  proc_fd_ಮಾರ್ಗ_t proc_ಮಾರ್ಗ;

  /* rootfs could be a relative path.  */
  rootfsfd = open (rootfs, O_ಮಾರ್ಗ | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (rootfsfd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", rootfs);

  ಆದರೆ (UNLIKELY (fstat (rootfsfd, &prev_st) < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fstat `%s`", rootfs);

  ಗಾಗಿ (;;)
    {
      ರಚನೆ stat cur_st;
      libcrun_error_t tmp_err = ಖಾಲಿ;
      ಪೂರ್ಣಾಂಕ ret;
      cleanup_close ಪೂರ್ಣಾಂಕ parentfd = -1;

      ret = do_mount_setattr (false, rootfs, rootfsfd, 0, MS_PRIVATE, &tmp_err);
      ಆದರೆ (ret == 0)
        ಹಿಂತಿರುಗಿ 0;
      crun_error_release (&tmp_err);

      get_proc_self_fd_ಮಾರ್ಗ (proc_ಮಾರ್ಗ, rootfsfd);
      ret = mount (ಖಾಲಿ, proc_ಮಾರ್ಗ, ಖಾಲಿ, MS_PRIVATE, ಖಾಲಿ);
      ಆದರೆ (ret == 0)
        ಹಿಂತಿರುಗಿ 0;

      parentfd = openat (rootfsfd, "..", O_ಮಾರ್ಗ | O_CLOEXEC);
      ಆದರೆ (parentfd < 0)
        {
          ಪೂರ್ಣಾಂಕ saved_errno = errno;
          ret = faccessat (rootfsfd, "..", X_OK, AT_EACCESS);
          ಆದರೆ (ret != 0)
            ಹಿಂತಿರುಗಿ crun_make_error (err, EACCES, "make `%s` private: a component is not accessible", rootfs);
          ಹಿಂತಿರುಗಿ crun_make_error (err, saved_errno, "make `%s` private: cannot open component", rootfs);
        }

      ಆದರೆ (UNLIKELY (fstat (parentfd, &cur_st) < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fstat parent of `%s`", rootfs);

      /* Reached the root of the file system: ".." points to itself.  */
      ಆದರೆ (cur_st.st_dev == prev_st.st_dev && cur_st.st_ino == prev_st.st_ino)
        ನಿಲ್ಲಿಸು;

      prev_st = cur_st;
      close_and_reset (&rootfsfd);
      rootfsfd = get_and_reset (&parentfd);
    }

  ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "make `%s` private", rootfs);
}

ಪೂರ್ಣಾಂಕ
libcrun_ಗಣ_mounts (ರಚನೆ container_entrypoint_s *entrypoint_ವಾದಗಳು, libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs, ಗಣ_mounts_cb_t cb, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಪೂರ್ಣಾಂಕ is_user_ns = 0;
  ಪೂರ್ಣಾಂಕ ret = 0;

  ಆದರೆ (rootfs == ಖಾಲಿ || def->mounts == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  ret = open (rootfs, O_ಮಾರ್ಗ | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", rootfs);

  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd = ret;
  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfs = rootfs;

  // configure handler mounts
  ret = libcrun_container_notify_handler (entrypoint_ವಾದಗಳು, HANDLER_CONFIGURE_MOUNTS, container, rootfs, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_MOUNTS");

  ಆದರೆ (def->ಬೇರು->readonly)
    {
      ರಚನೆ remount_s *r;
      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ remount_flags = MS_REMOUNT | MS_BIND | MS_RDONLY;
      ಪೂರ್ಣಾಂಕ fd;

      fd = dup (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd);
      ಆದರೆ (UNLIKELY (fd < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "dup fd for `%s`", rootfs);

      r = make_remount (fd, rootfs, remount_flags, ಖಾಲಿ, get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->remounts);
      get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->remounts = r;
    }

  /* Mounts are already done before pivot_root in setup_mount_namespace.  */

  is_user_ns = (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->unshare_flags & CLONE_NEWUSER);
  ಆದರೆ (! is_user_ns)
    {
      is_user_ns = check_running_ಒಳ_user_namespace (err);
      ಆದರೆ (UNLIKELY (is_user_ns < 0))
        ಹಿಂತಿರುಗಿ is_user_ns;
    }

  ಆದರೆ (! get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->mount_dev_from_host)
    {
      ret = create_missing_devs (container, is_user_ns ? true : false, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  /* Notify the callback after all the mounts are ready but before making them read-only.  */
  ಆದರೆ (cb)
    {
      ret = cb (cb_ದತ್ತಾಂಶ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (def->process && def->process->cwd)
    {
      libcrun_error_t tmp_err = ಖಾಲಿ;
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rel_cwd = consume_slashes (def->process->cwd);
      /* Ignore errors here and let it fail later.  */
      (ಶೂನ್ಯ) crun_ಸುರಕ್ಷಿತ_ensure_directory_at (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd, rootfs, rel_cwd, 0755, &tmp_err);
      crun_error_release (&tmp_err);
    }

  ret = do_masked_and_readonly_paths (container, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
umount_or_hide (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ret = umount2 (ಗುರಿ, MNT_DETACH);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಪೂರ್ಣಾಂಕ saved_errno = errno;

      /* If the umount2 failed with EINVAL then the mount could
         be locked.  Hide it by mounting a tmpfs on top of it.  */
      ಆದರೆ (errno == EINVAL)
        {
          ret = mount (ಖಾಲಿ, ಗುರಿ, "tmpfs", 0, "size=0k");
          ಆದರೆ (LIKELY (ret == 0))
            ಹಿಂತಿರುಗಿ 0;
        }

      ಹಿಂತಿರುಗಿ crun_make_error (err, saved_errno, "umount `%s`", ಗುರಿ);
    }

  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
move_ಬೇರು (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ret = chdir (rootfs);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "chdir to `%s`", rootfs);

  ret = umount_or_hide ("/sys", err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = umount_or_hide ("/proc", err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = mount (rootfs, "/", "", MS_MOVE, "");
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "mount MS_MOVE to `/`");

  ret = chroot (".");
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "chroot to `%s`", rootfs);

  ret = chdir ("/");
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "chdir to `%s`", rootfs);

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_finalize_mounts (ರಚನೆ container_entrypoint_s *entrypoint_ವಾದಗಳು, libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs,
                         ಗಣ_mounts_cb_t after_ಅಚ್ಚು_cb, ಶೂನ್ಯ *cb_ದತ್ತಾಂಶ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  // configure handler mounts for phase: HANDLER_CONFIGURE_AFTER_MOUNTS
  ret = libcrun_container_notify_handler (entrypoint_ವಾದಗಳು, HANDLER_CONFIGURE_AFTER_MOUNTS, container, rootfs, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_error_wrap (err, "failed configuring mounts for handler at phase: HANDLER_CONFIGURE_AFTER_MOUNTS");

  ret = do_finalize_notify_socket (container, rootfs, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  close_and_reset (&(get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd));

  /* The host root must not stay reachable through /proc/self/fd once the
     container rootfs becomes the root: every path resolved by name from now
     on is controlled by the container image.  */
  close_and_reset (&(get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->old_ಬೇರು_fd));

  ಆದರೆ (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->needs_ಅಚ್ಚು)
    {
      get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->needs_ಅಚ್ಚು = false;

      ಆದರೆ (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->no_ಅಚ್ಚು)
        {
          ret = move_ಬೇರು (rootfs, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ret = do_ಅಚ್ಚು (container, rootfs, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }

      ret = do_mount (container, ಖಾಲಿ, -1, "/", ಖಾಲಿ,
                      get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfs_ಪ್ರಸರಣ,
                      ಖಾಲಿ, LABEL_MOUNT, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = chdir ("/");
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "chdir to `/`");
    }

  ಆದರೆ (after_ಅಚ್ಚು_cb)
    {
      ret = after_ಅಚ್ಚು_cb (cb_ದತ್ತಾಂಶ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ret = finalize_mounts (container, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
maybe_open_ವೃಕ್ಷ_namespace (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *rootfs, ಪೂರ್ಣಾಂಕ *ಹೊರಹೋಗು_fd, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ rootfs_fd = -1;
  ಪೂರ್ಣಾಂಕ ವೃಕ್ಷ_fd;

  *ಹೊರಹೋಗು_fd = -1;

  rootfs_fd = open (rootfs, O_DIRECTORY | O_ಮಾರ್ಗ | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (rootfs_fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", rootfs);

  ವೃಕ್ಷ_fd = syscall_open_ವೃಕ್ಷ (rootfs_fd, "",
                               OPEN_ವೃಕ್ಷ_NAMESPACE | OPEN_TREE_CLOEXEC
                                   | AT_ಖಾಲಿಗಳು_ಮಾರ್ಗ | AT_RECURSIVE);
  ಆದರೆ (ವೃಕ್ಷ_fd < 0)
    {
      ಆದರೆ (errno == EINVAL || errno == ENOSYS || errno == EPERM)
        ಹಿಂತಿರುಗಿ 0;
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open_tree `%s`", rootfs);
    }

  *ಹೊರಹೋಗು_fd = ವೃಕ್ಷ_fd;
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ರಚನೆ libcrun_fd_map *
get_fd_map (libcrun_container_t *container)
{
  ರಚನೆ libcrun_fd_map *mount_fds = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->mount_fds;

  ಆದರೆ (mount_fds == ಖಾಲಿ)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      mount_fds = make_libcrun_fd_map (def->mounts_len);
      get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->mount_fds = mount_fds;
    }
  ಹಿಂತಿರುಗಿ mount_fds;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
open_mount_ದ_type (libcrun_container_t *container,
                    runtime_spec_schema_defs_mount *mount,
                    libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free ಅಕ್ಷರ *mnt_ದತ್ತಾಂಶ = ಖಾಲಿ;
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ mnt_flags = 0, mnt_extra = 0;
  uint64_t mnt_rec_clear = 0, mnt_rec_ಗಣ = 0;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *label_type = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *label_val = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fstype;
  ಪೂರ್ಣಾಂಕ mnt_fd;
  size_t j;

  ಆದರೆ (mount->type == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ -1;

  fstype = mount->type;

  ಗಾಗಿ (j = 0; j < mount->options_len; j++)
    mnt_flags |= get_mount_flags_or_option (mount->options[j],
                                            mnt_flags, &mnt_extra, &mnt_ದತ್ತಾಂಶ,
                                            &mnt_rec_clear, &mnt_rec_ಗಣ);

  ಆದರೆ (def->linux && def->linux->mount_label)
    {
      bool is_sysfs_or_proc = strcmp (fstype, "sysfs") == 0
                              || strcmp (fstype, "proc") == 0;
      ಪೂರ್ಣಾಂಕ extra_flags = get_mount_label_how (fstype, is_sysfs_or_proc);
      ಆದರೆ (extra_flags == LABEL_MOUNT)
        {
          label_type = get_selinux_context_type (container, err);
          ಆದರೆ (label_type == ಖಾಲಿ)
            crun_error_release (err);
          ಇಲ್ಲದಿದ್ದರೆ
            label_val = def->linux->mount_label;
        }
    }

  ಆದರೆ (strcmp (fstype, "cgroup") == 0)
    {
      ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
      ಆದರೆ (cgroup_ಬಹುಲಕ == CGROUP_ಬಹುಲಕ_UNIFIED)
        fstype = "cgroup2";
      crun_error_release (err);
    }

  mnt_fd = fsopen_mount (fstype, mount->ಮೂಲ, label_type, label_val, mnt_ದತ್ತಾಂಶ);
  ಆದರೆ (mnt_fd < 0 && label_type)
    mnt_fd = fsopen_mount (fstype, mount->ಮೂಲ, ಖಾಲಿ, ಖಾಲಿ, mnt_ದತ್ತಾಂಶ);

  ಹಿಂತಿರುಗಿ mnt_fd;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LSMT_ROOT
#  define LSMT_ROOT 0xffffffffffffffffULL
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STATMOUNT_FS_TYPE
#  define STATMOUNT_FS_TYPE 0x00000020U
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STATMOUNT_MNT_ROOT
#  define STATMOUNT_MNT_ROOT 0x00000040U
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ STATMOUNT_MNT_POINT
#  define STATMOUNT_MNT_POINT 0x00000080U
#ಆದರೆ_ಅಂತ್ಯ

/* Declared here so that the build does not depend on the UAPI headers being
   recent enough; the layout is stable.  */
ರಚನೆ mnt_id_req_s
{
  uint32_t ಗಾತ್ರ_2;
  uint32_t spare;
  uint64_t mnt_id;
  uint64_t param;
  uint64_t mnt_ns_id;
};

ರಚನೆ statmount_s
{
  uint32_t ಗಾತ್ರ_2;
  uint32_t mnt_opts;
  uint64_t ಮುಖವಾಡ;
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
  ಅಕ್ಷರ str[];
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
syscall_listmount (ರಚನೆ mnt_id_req_s *req, uint64_t *ids, size_t nr, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __NR_listmount
  ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) syscall (__NR_listmount, req, ids, nr, flags);
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) req;
  (ಶೂನ್ಯ) ids;
  (ಶೂನ್ಯ) nr;
  (ಶೂನ್ಯ) flags;
  errno = ENOSYS;
  ಹಿಂತಿರುಗಿ -1;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
syscall_statmount (ರಚನೆ mnt_id_req_s *req, ರಚನೆ statmount_s *buf, size_t ಗಾತ್ರ_2, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __NR_statmount
  ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ) syscall (__NR_statmount, req, buf, ಗಾತ್ರ_2, flags);
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) req;
  (ಶೂನ್ಯ) buf;
  (ಶೂನ್ಯ) ಗಾತ್ರ_2;
  (ಶೂನ್ಯ) flags;
  errno = ENOSYS;
  ಹಿಂತಿರುಗಿ -1;
#ಆದರೆ_ಅಂತ್ಯ
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
mounts_ದೊರಕಿತು_pinned_mount_namespace (runtime_spec_schema_config_schema *def)
{
  cleanup_free uint64_t *ids = ಖಾಲಿ;
  size_t i, n_ids = 0, ಸಾಮರ್ಥ್ಯ = 128;
  bool has_absolute_bind = false;
  uint64_t ಕೊನೆಯ = 0;

  ಗಾಗಿ (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false, nofollow = false;

      ಆದರೆ (is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && def->mounts[i]->ಮೂಲ != ಖಾಲಿ && def->mounts[i]->ಮೂಲ[0] == '/')
        {
          has_absolute_bind = true;
          ನಿಲ್ಲಿಸು;
        }
    }
  ಆದರೆ (! has_absolute_bind)
    ಹಿಂತಿರುಗಿ false;

  ids = xmalloc (ಸಾಮರ್ಥ್ಯ * ಗಾತ್ರ (uint64_t));
  ಗಾಗಿ (;;)
    {
      ರಚನೆ mnt_id_req_s req = {
        .ಗಾತ್ರ_2 = ಗಾತ್ರ (req),
        .mnt_id = LSMT_ROOT,
        .param = ಕೊನೆಯ,
      };
      ಪೂರ್ಣಾಂಕ ret;

      ಆದರೆ (n_ids == ಸಾಮರ್ಥ್ಯ)
        {
          ಸಾಮರ್ಥ್ಯ *= 2;
          ids = xrealloc (ids, ಸಾಮರ್ಥ್ಯ * ಗಾತ್ರ (uint64_t));
        }

      ret = syscall_listmount (&req, ids + n_ids, ಸಾಮರ್ಥ್ಯ - n_ids, 0);
      ಆದರೆ (ret < 0)
        /* Without the mount table there is no way to tell; the kernel would
           reject the move_mount() anyway, so stay on the traditional path.  */
        ಹಿಂತಿರುಗಿ true;
      ಆದರೆ (ret == 0)
        ನಿಲ್ಲಿಸು;

      n_ids += (size_t) ret;
      ಕೊನೆಯ = ids[n_ids - 1];
    }

  ಗಾಗಿ (i = 0; i < n_ids; i++)
    {
      ಅಕ್ಷರ ಬಫರ್[4096] __attribute__ ((aligned (8)));
      ರಚನೆ statmount_s *sm = (ರಚನೆ statmount_s *) ಬಫರ್;
      ರಚನೆ mnt_id_req_s req = {
        .ಗಾತ್ರ_2 = ಗಾತ್ರ (req),
        .mnt_id = ids[i],
        .param = STATMOUNT_FS_TYPE | STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT,
      };
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಬೇರು, *ಬಿಂದು;
      size_t j;

      ಆದರೆ (syscall_statmount (&req, sm, ಗಾತ್ರ (ಬಫರ್), 0) < 0)
        ಮುಂದುವರಿ;
      ಆದರೆ ((sm->ಮುಖವಾಡ & (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT)) != (STATMOUNT_MNT_ROOT | STATMOUNT_MNT_POINT))
        ಮುಂದುವರಿ;

      /* The root of a pinned mount namespace reads as "mnt:[...]"; the other
         namespace types cannot create a loop.  */
      ಬೇರು = sm->str + sm->mnt_root;
      ಆದರೆ (! has_ಪೂರ್ವಪ್ರತ್ಯಯ (ಬೇರು, "mnt:["))
        ಮುಂದುವರಿ;

      ಬಿಂದು = sm->str + sm->mnt_point;

      ಗಾಗಿ (j = 0; j < def->mounts_len; j++)
        {
          bool recursive = false, nofollow = false;
          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೂಲ = def->mounts[j]->ಮೂಲ;
          size_t ಮೂಲ_len;

          ಆದರೆ (! is_bind_mount (def->mounts[j], &recursive, &nofollow))
            ಮುಂದುವರಿ;
          ಆದರೆ (ಮೂಲ == ಖಾಲಿ || ಮೂಲ[0] != '/')
            ಮುಂದುವರಿ;

          ಮೂಲ_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಮೂಲ);
          ಆಗ (ಮೂಲ_len > 1 && ಮೂಲ[ಮೂಲ_len - 1] == '/')
            ಮೂಲ_len--;

          ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ (ಬಿಂದು, ಮೂಲ, ಮೂಲ_len) != 0)
            ಮುಂದುವರಿ;

          /* The source itself is a pinned mount namespace.  */
          ಆದರೆ (ಬಿಂದು[ಮೂಲ_len] == '\0')
            ಹಿಂತಿರುಗಿ true;

          /* A recursive bind clones the subtree below the source as well, so
             a pin anywhere under it is picked up.  A plain bind clones the
             single mount, and cannot pick one up.  */
          ಆದರೆ (recursive && (ಮೂಲ_len == 1 || ಬಿಂದು[ಮೂಲ_len] == '/'))
            ಹಿಂತಿರುಗಿ true;
        }
    }

  ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
can_use_open_ವೃಕ್ಷ_namespace (libcrun_container_t *container)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ರಚನೆ libcrun_fd_map *mount_fds;
  bool has_hooks = def->hooks
                   && (def->hooks->prestart_len || def->hooks->create_runtime_len
                       || def->hooks->create_container_len);
  bool has_userns = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->unshare_flags & CLONE_NEWUSER;
  size_t i;

  ಆದರೆ (has_hooks || has_userns)
    ಹಿಂತಿರುಗಿ false;

  ಆದರೆ (mounts_ದೊರಕಿತು_pinned_mount_namespace (def))
    ಹಿಂತಿರುಗಿ false;

  ಆದರೆ (container->context->no_open_ವೃಕ್ಷ_namespace)
    ಹಿಂತಿರುಗಿ false;

  {
    libcrun_error_t tmp_err = ಖಾಲಿ;
    ಪೂರ್ಣಾಂಕ ಒಳ_userns = check_running_ಒಳ_user_namespace (&tmp_err);
    crun_error_release (&tmp_err);
    ಆದರೆ (ಒಳ_userns != 0)
      ಹಿಂತಿರುಗಿ false;
  }

  mount_fds = get_fd_map (container);
  ಗಾಗಿ (i = 0; i < def->mounts_len; i++)
    {
      ಆದರೆ (mount_fds->fds[i] < 0)
        {
          bool has_cached_ಗುರಿ = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->copy_symlink_targets
                                   && i < get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->n_copy_symlink_targets
                                   && get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->copy_symlink_targets[i];
          ಆದರೆ (! has_cached_ಗುರಿ)
            ಹಿಂತಿರುಗಿ false;
        }
    }

  ಹಿಂತಿರುಗಿ true;
}

/* Only tmpfs mounts inherit the mode of the directory they are mounted
   over, and only when the mount does not already request an explicit mode=
   option.  This mirrors append_tmpfs_mode_if_missing() used by the fallback
   mount() path.  File systems such as proc, sysfs and mqueue have a root
   inode mode defined by the kernel that must not be overridden.  */
ಸ್ಥಿರ bool
mount_inherits_mountpoint_ಬಹುಲಕ (runtime_spec_schema_defs_mount *mount)
{
  size_t i;

  ಆದರೆ (mount->type == ಖಾಲಿ || strcmp (mount->type, "tmpfs") != 0)
    ಹಿಂತಿರುಗಿ false;

  ಗಾಗಿ (i = 0; i < mount->options_len; i++)
    ಆದರೆ (mount->options[i] != ಖಾಲಿ && has_ಪೂರ್ವಪ್ರತ್ಯಯ (mount->options[i], "mode="))
      ಹಿಂತಿರುಗಿ false;

  ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಸಿದ್ಧತೆ_mount_namespace (libcrun_container_t *container, bool no_ಅಚ್ಚು, ಅಕ್ಷರ **rootfs, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ rootfs_ಪ್ರಸರಣ = 0;
  cleanup_close ಪೂರ್ಣಾಂಕ rootfsfd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ ವೃಕ್ಷ_fd = -1;
  libcrun_error_t tmp_err = ಖಾಲಿ;
  bool use_open_ವೃಕ್ಷ = false;
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (def->linux && def->linux->rootfs_ಪ್ರಸರಣ)
    rootfs_ಪ್ರಸರಣ = get_mount_flags (def->linux->rootfs_ಪ್ರಸರಣ, 0, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ);

  ಆದರೆ ((rootfs_ಪ್ರಸರಣ & ALL_PROPAGATIONS_NO_REC) == 0)
    rootfs_ಪ್ರಸರಣ = MS_REC | MS_PRIVATE;

  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfs_ಪ್ರಸರಣ = rootfs_ಪ್ರಸರಣ;

  ret = libcrun_container_enter_cgroup_ns (container, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಬಿಡುಗಡೆ (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->host_rootfs);
  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->host_rootfs = xstrdup (*rootfs);

  /* Pre-create mounts and cache paths before pivot_root,
     while the host file system is still reachable.  */
  ಗಾಗಿ (i = 0; i < def->mounts_len; i++)
    {
      ರಚನೆ libcrun_fd_map *mount_fds = get_fd_map (container);
      cleanup_close ಪೂರ್ಣಾಂಕ mnt_fd = -1;
      bool recursive_bind, nofollow;
      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ mnt_flags = 0;

      ಆದರೆ (mount_fds && mount_fds->fds[i] >= 0)
        ಮುಂದುವರಿ;

      /* Pre-read symlink targets for copy-symlink mounts while the host file system is still reachable.  */
      ಆದರೆ (def->mounts[i]->ಮೂಲ != ಖಾಲಿ
          && def->mounts[i]->ಮೂಲ[0] == '/')
        {
          cleanup_free ಅಕ್ಷರ *mnt_ದತ್ತಾಂಶ = ಖಾಲಿ;
          ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ mnt_extra = 0;
          uint64_t mnt_rec_clear = 0, mnt_rec_ಗಣ = 0;
          size_t j;

          ಗಾಗಿ (j = 0; j < def->mounts[i]->options_len; j++)
            mnt_flags |= get_mount_flags_or_option (def->mounts[i]->options[j],
                                                    mnt_flags, &mnt_extra, &mnt_ದತ್ತಾಂಶ,
                                                    &mnt_rec_clear, &mnt_rec_ಗಣ);

          ಆದರೆ (mnt_extra & OPTION_COPY_SYMLINK)
            {
              cleanup_free ಅಕ್ಷರ *ಗುರಿ = ಖಾಲಿ;
              libcrun_error_t tmp_err = ಖಾಲಿ;
              ssize_t len;

              len = ಸುರಕ್ಷಿತ_readlinkat (AT_FDCWD, def->mounts[i]->ಮೂಲ, &ಗುರಿ, 0, &tmp_err);
              ಆದರೆ (UNLIKELY (len < 0))
                crun_error_release (&tmp_err);

              ಆದರೆ (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->copy_symlink_targets == ಖಾಲಿ)
                {
                  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->copy_symlink_targets
                      = xmalloc0 (def->mounts_len * ಗಾತ್ರ (ಅಕ್ಷರ *));
                  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->n_copy_symlink_targets = def->mounts_len;
                }
              get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->copy_symlink_targets[i] = ಗುರಿ;
              ಗುರಿ = ಖಾಲಿ;
            }
        }

      ಆದರೆ (is_bind_mount (def->mounts[i], &recursive_bind, &nofollow))
        {
          bool has_cached_ಗುರಿ = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->copy_symlink_targets
                                   && i < get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->n_copy_symlink_targets
                                   && get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->copy_symlink_targets[i];

          ಆದರೆ (mount_fds && def->mounts[i]->ಮೂಲ != ಖಾಲಿ && ! has_cached_ಗುರಿ
              && get_ಮಾರ್ಗ_ಒಳ_rootfs (*rootfs, def->mounts[i]->ಮೂಲ) == ಖಾಲಿ)
            {
              libcrun_error_t tmp_err = ಖಾಲಿ;
              ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪ್ರಸರಣ = MS_PRIVATE;

              ಆದರೆ (mnt_flags & (MS_SHARED | MS_SLAVE | MS_UNBINDABLE))
                ಪ್ರಸರಣ = 0;

              mount_fds->fds[i] = get_bind_mount (AT_FDCWD, def->mounts[i]->ಮೂಲ,
                                                  recursive_bind, false, nofollow,
                                                  ಪ್ರಸರಣ, &tmp_err);
              ಆದರೆ (mount_fds->fds[i] < 0)
                crun_error_release (&tmp_err);
            }
          ಮುಂದುವರಿ;
        }

      mnt_fd = open_mount_ದ_type (container, def->mounts[i], err);
      ಆದರೆ (mnt_fd < 0)
        {
          crun_error_release (err);
          ಮುಂದುವರಿ;
        }

      ಆದರೆ (mount_inherits_mountpoint_ಬಹುಲಕ (def->mounts[i]))
        {
          cleanup_close ಪೂರ್ಣಾಂಕ destfd = -1;
          ರಚನೆ stat st;

          ಆದರೆ (rootfsfd < 0)
            {
              rootfsfd = open (*rootfs, O_ಮಾರ್ಗ | O_CLOEXEC);
              ಆದರೆ (UNLIKELY (rootfsfd < 0))
                ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", *rootfs);
            }

          destfd = ಸುರಕ್ಷಿತ_openat (rootfsfd, *rootfs, def->mounts[i]->destination, O_ಮಾರ್ಗ | O_CLOEXEC, 0, &tmp_err);
          ಆದರೆ (destfd < 0)
            crun_error_release (&tmp_err);
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fstat (destfd, &st) == 0)
            {
              proc_fd_ಮಾರ್ಗ_t fd_ಮಾರ್ಗ;
              ಪೂರ್ಣಾಂಕ procfd = get_procfd (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container), err);
              ಆದರೆ (UNLIKELY (procfd < 0))
                ಹಿಂತಿರುಗಿ procfd;

              get_self_fd_ಮಾರ್ಗ (fd_ಮಾರ್ಗ, mnt_fd);
              ret = fchmodat (procfd, fd_ಮಾರ್ಗ, st.st_mode & 07777, 0);
              (ಶೂನ್ಯ) ret;
            }
        }

      mount_fds->fds[i] = mnt_fd;
      mnt_fd = -1;
    }
  close_and_reset (&rootfsfd);

  ret = get_shared_ಖಾಲಿಗಳು_dir_cached (container, ಖಾಲಿ, &tmp_err);
  ಆದರೆ (ret < 0)
    {
      crun_error_release (&tmp_err);
      get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->maskdir_bind_failed = true;
    }

  ಆದರೆ (! get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->joined_mount_ns
      && can_use_open_ವೃಕ್ಷ_namespace (container))
    use_open_ವೃಕ್ಷ = true;

  ಆದರೆ (use_open_ವೃಕ್ಷ)
    {
      ret = maybe_open_ವೃಕ್ಷ_namespace (*rootfs, &ವೃಕ್ಷ_fd, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  /* Pre-open needed device fds and the notify socket while the host
     file system is still reachable.  After setns(OPEN_TREE_NAMESPACE)
     the host /dev is no longer accessible.  When tree_fd < 0 (no
     OPEN_TREE_NAMESPACE), this is retried after unshare(CLONE_NEWNS)
     below since open_tree needs CAP_SYS_ADMIN in the user namespace
     that owns the mount namespace.  */
  ಆದರೆ (ವೃಕ್ಷ_fd >= 0)
    preopen_needed_devs_and_notify (container);

  ಆದರೆ (ವೃಕ್ಷ_fd >= 0)
    {
      ret = setns (ವೃಕ್ಷ_fd, CLONE_NEWNS);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "setns `CLONE_NEWNS`");

      ret = mount (ಖಾಲಿ, "/", ಖಾಲಿ, MS_REMOUNT | MS_BIND, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "remount `/`");

      ret = do_mount (container, ಖಾಲಿ, -1, "/", ಖಾಲಿ, MS_REC | MS_PRIVATE, ಖಾಲಿ, LABEL_MOUNT, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = do_mount (container, ಖಾಲಿ, -1, "/", ಖಾಲಿ, rootfs_ಪ್ರಸರಣ, ಖಾಲಿ, LABEL_MOUNT, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->needs_ಅಚ್ಚು = false;
      get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->no_ಅಚ್ಚು = no_ಅಚ್ಚು;
      /* setns() made the container rootfs the process root, so an absolute
         source path no longer refers to the host.  */
      get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->host_ಬೇರು_switched = true;
      ಬಿಡುಗಡೆ (*rootfs);
      *rootfs = xstrdup ("/");
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (! get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->joined_mount_ns)
        {
          ret = unshare (CLONE_NEWNS);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
        }

      ret = do_mount (container, ಖಾಲಿ, -1, "/", ಖಾಲಿ, rootfs_ಪ್ರಸರಣ, ಖಾಲಿ, LABEL_MOUNT, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = make_ಪೋಷಕ_mount_ಖಾಸಗಿ (*rootfs, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = do_mount (container, *rootfs, -1, *rootfs, ಖಾಲಿ, MS_BIND | MS_REC | MS_PRIVATE, ಖಾಲಿ, LABEL_MOUNT, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->needs_ಅಚ್ಚು = true;
      get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->no_ಅಚ್ಚು = no_ಅಚ್ಚು;
    }

  /* Mount everything before pivot_root while host paths are still reachable.
     Use the pre-created fd when available, fall back to mount().  */
  ret = open (*rootfs, O_ಮಾರ್ಗ | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", *rootfs);
  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd = ret;
  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfs = *rootfs;

  ಆದರೆ (! container->context->force_no_cgroup)
    {
      ಪೂರ್ಣಾಂಕ cgroup_ಬಹುಲಕ = libcrun_get_cgroup_ಬಹುಲಕ (err);
      ಆದರೆ (UNLIKELY (cgroup_ಬಹುಲಕ < 0))
        ಹಿಂತಿರುಗಿ cgroup_ಬಹುಲಕ;

      ಆದರೆ (cgroup_ಬಹುಲಕ == CGROUP_ಬಹುಲಕ_UNIFIED)
        {
          ಅಕ್ಷರ *unified_cgroup_ಮಾರ್ಗ = ಖಾಲಿ;
          ಪೂರ್ಣಾಂಕ procfd = get_procfd (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container), err);
          ಆದರೆ (UNLIKELY (procfd < 0))
            ಹಿಂತಿರುಗಿ procfd;

          ret = libcrun_get_cgroup_process_at (procfd, &unified_cgroup_ಮಾರ್ಗ, true, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;

          get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->unified_cgroup_ಮಾರ್ಗ = unified_cgroup_ಮಾರ್ಗ;
        }
    }

  ret = do_mounts (container, *rootfs, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      close_and_reset (&get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd);
      get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfs = ಖಾಲಿ;
      ಹಿಂತಿರುಗಿ ret;
    }

  close_and_reset (&get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfsfd);
  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->rootfs = ಖಾಲಿ;

  ಆದರೆ (ವೃಕ್ಷ_fd < 0)
    preopen_needed_devs_and_notify (container);

  /* After pivot_root, /proc is not yet mounted.  */
  ret = get_procfd (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container), &tmp_err);
  ಆದರೆ (ret < 0)
    crun_error_release (&tmp_err);

  check_running_ಒಳ_user_namespace (&tmp_err);
  crun_error_release (&tmp_err);

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_do_ಅಚ್ಚು_ಬೇರು (libcrun_container_t *container, bool no_ಅಚ್ಚು, ಅಕ್ಷರ **rootfs, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (*rootfs == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  ಆದರೆ (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->unshare_flags & CLONE_NEWNS)
    {
      ret = ಸಿದ್ಧತೆ_mount_namespace (container, no_ಅಚ್ಚು, rootfs, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      /* If setup_mount_namespace used OPEN_TREE_NAMESPACE, rootfs is
         already set to "/".  Otherwise pivot_root is deferred until
         after the mounts are created.  */
      ಆದರೆ (strcmp (*rootfs, "/") == 0)
        {
          ret = chdir ("/");
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "chdir to `/`");
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ret = chroot (*rootfs);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "chroot to `%s`", *rootfs);

      ret = chdir ("/");
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "chdir to `/`");

      ಬಿಡುಗಡೆ (*rootfs);
      *rootfs = xstrdup ("/");
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* If one of stdin, stdout, stderr are pointing to /dev/null on
 * the outside of the container, this moves it to /dev/null inside
 * of the container. This needs to run after pivot/chroot-ing. */
ಪೂರ್ಣಾಂಕ
libcrun_reopen_dev_null (libcrun_error_t *err)
{
  ರಚನೆ stat statbuf;
  cleanup_close ಪೂರ್ಣಾಂಕ fd;
  ಪೂರ್ಣಾಂಕ ret;
  ಪೂರ್ಣಾಂಕ i;

  /* Open /dev/null inside of the container. */
  fd = open ("/dev/null", O_RDWR | O_CLOEXEC | O_NOFOLLOW);
  ಆದರೆ (UNLIKELY (fd == -1))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `/dev/null`");

  ret = ensure_dev_null (fd, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಗಾಗಿ (i = 0; i <= 2; i++)
    {
      ಆದರೆ (UNLIKELY (fstat (i, &statbuf) == -1))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "stat fd `%d`", i);
      ಆದರೆ (is_dev_null (&statbuf))
        {
          /* This FD is pointing to /dev/null. Point it to /dev/null inside
           * of the container. */
          ಆದರೆ (UNLIKELY (dup2 (fd, i) == -1))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "dup2 `%d`", i);
        }
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
uidgidmap_helper (ಅಕ್ಷರ *helper, pid_t pid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *map_file, libcrun_error_t *err)
{
#ವ್ಯಾಖ್ಯಾನಿಸು MAX_ವಾದಗಳು 20
  ಅಕ್ಷರ pid_fmt[16];
  ಅಕ್ಷರ *ವಾದಗಳು[MAX_ವಾದಗಳು + 1];
  ಅಕ್ಷರ *ಮುಂದಿನ;
  cleanup_free ಅಕ್ಷರ *map_file_copy = xstrdup (map_file);
  size_t nargs = 0;
  ಪೂರ್ಣಾಂಕ ret;

  ವಾದಗಳು[nargs++] = helper;

  ret = snprintf (pid_fmt, ಗಾತ್ರ (pid_fmt), "%d", pid);
  ಆದರೆ (UNLIKELY (ret >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (pid_fmt)))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

  ವಾದಗಳು[nargs++] = pid_fmt;
  ಮುಂದಿನ = map_file_copy;
  ಆಗ (nargs < MAX_ವಾದಗಳು)
    {
      ಅಕ್ಷರ *p = strsep (&ಮುಂದಿನ, " \n");
      ಆದರೆ (ಮುಂದಿನ == ಖಾಲಿ)
        ನಿಲ್ಲಿಸು;
      ವಾದಗಳು[nargs++] = p;
    }
  ವಾದಗಳು[nargs++] = ಖಾಲಿ;

  ret = ಸತತ_process (ವಾದಗಳು, err);
  ಆದರೆ (ret < 0)
    ಹಿಂತಿರುಗಿ ret;
  ಆದರೆ (ret > 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "`%s` exited with status %d", helper, ret);
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
newgidmap (pid_t pid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *map_file, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ uidgidmap_helper ("newgidmap", pid, map_file, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
newuidmap (pid_t pid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *map_file, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ uidgidmap_helper ("newuidmap", pid, map_file, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
deny_setgroups (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;

  fd = libcrun_open_proc_pid_file (container, pid, "setgroups", O_WRONLY, err);
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ fd;

  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (fd, "deny", 4));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "write to `/proc/%d/setgroups`", pid);

  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->deny_setgroups = true;
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
can_setgroups (libcrun_container_t *container, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  cleanup_free ಅಕ್ಷರ *content = ಖಾಲಿ;

  ಆದರೆ (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->deny_setgroups)
    ಹಿಂತಿರುಗಿ 0;

  ಆದರೆ (container->container_def->annotations)
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *annotation;

      /* Skip setgroups if the annotation is set to anything different than "0".  */
      annotation = ಕಂಡುಹಿಡಿ_annotation (container, "run.oci.keep_original_groups");
      ಆದರೆ (annotation)
        ಹಿಂತಿರುಗಿ strcmp (annotation, "0") == 0 ? 1 : 0;
    }

  {
    cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
    fd = libcrun_open_proc_file (container, "self/setgroups", O_RDONLY, err);
    ಆದರೆ (fd < 0)
      ret = fd;
    ಇಲ್ಲದಿದ್ದರೆ
      ret = read_all_fd (fd, "self/setgroups", &content, ಖಾಲಿ, err);
  }
  ಆದರೆ (ret < 0)
    {
      /* If the file does not exist, then the kernel does not support /proc/self/setgroups and setgroups can always be used.  */
      ಆದರೆ (crun_error_get_errno (err) == ENOENT)
        {
          crun_error_release (err);
          ಹಿಂತಿರುಗಿ 1;
        }
      ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ (content, "deny", 4) == 0 ? 0 : 1;
}

ಪೂರ್ಣಾಂಕ
libcrun_container_setgroups (libcrun_container_t *container,
                             runtime_spec_schema_config_schema_process *process,
                             libcrun_error_t *err)
{
  gid_t *additional_gids = ಖಾಲಿ;
  size_t additional_gids_len = 0;
  ಪೂರ್ಣಾಂಕ can_do_setgroups;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (process != ಖಾಲಿ && process->user != ಖಾಲಿ)
    {
      additional_gids = process->user->additional_gids;
      additional_gids_len = process->user->additional_gids_len;
    }

  can_do_setgroups = can_setgroups (container, err);
  ಆದರೆ (UNLIKELY (can_do_setgroups < 0))
    ಹಿಂತಿರುಗಿ can_do_setgroups;

  ಆದರೆ (can_do_setgroups == 0)
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *annotation;
      size_t i;

      annotation = ಕಂಡುಹಿಡಿ_annotation (container, "run.oci.keep_original_groups");
      ಆದರೆ (annotation && strcmp (annotation, "0") != 0)
        ಹಿಂತಿರುಗಿ 0;

      /* The supplementary groups cannot be changed.  Fail if any of the
         requested additional gids would not be in effect, but tolerate
         the primary gid since it is already granted by setgid.  */
      ಗಾಗಿ (i = 0; i < additional_gids_len; i++)
        ಆದರೆ (additional_gids[i] != process->user->gid)
          ಹಿಂತಿರುಗಿ crun_make_error (err, EPERM, "cannot set additional gid `%d`: setgroups is denied in the user namespace",
                                  (ಪೂರ್ಣಾಂಕ) additional_gids[i]);
      ಹಿಂತಿರುಗಿ 0;
    }

  ret = setgroups (additional_gids_len, additional_gids);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "setgroups");

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_container_enter_cgroup_ns (libcrun_container_t *container, libcrun_error_t *err)
{
  ಆದರೆ (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->unshare_cgroupns)
    {
      ಪೂರ್ಣಾಂಕ ret = unshare (CLONE_NEWCGROUP);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          ಆದರೆ (errno != EINVAL)
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "unshare (CLONE_NEWCGROUP)");
        }
    }
  ಹಿಂತಿರುಗಿ 0;
}

// determine whether the uid/gid mappings only contain a single entry
// that maps the host uid/gid on the process->user->uid/gid
ಪೂರ್ಣಾಂಕ
is_single_mapping (runtime_spec_schema_defs_id_mapping **mappings, size_t len,
                   uint32_t host_id, uint32_t container_id)
{
  ಆದರೆ (len != 1)
    ಹಿಂತಿರುಗಿ 0;

  ಆದರೆ (mappings[0]->ಗಾತ್ರ_2 != 1)
    ಹಿಂತಿರುಗಿ 0;

  ಆದರೆ (mappings[0]->container_id != container_id || mappings[0]->host_id != host_id)
    ಹಿಂತಿರುಗಿ 0;

  ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_id_map (libcrun_container_t *container, pid_t pid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *map_name,
              ಪೂರ್ಣಾಂಕ (*idmap_helper) (pid_t, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *, libcrun_error_t *), ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *helper_name,
              bool warn_on_failure, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *map, size_t map_len,
              runtime_spec_schema_defs_id_mapping **mappings, size_t mappings_len,
              uint32_t host_id, uint32_t container_id, bool skip_if_setgroups_denied,
              libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret = 0;

  ಆದರೆ (container->host_uid)
    ret = idmap_helper (pid, map, err);
  ಆದರೆ (container->host_uid == 0 || ret < 0)
    {
      cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;

      ಆದರೆ (ret < 0)
        {
          ಆದರೆ (warn_on_failure)
            libcrun_warning ("unable to invoke `%s`, will try creating a user namespace with single mapping as an alternative", helper_name);
          crun_error_release (err);
        }

      fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
      ಆದರೆ (UNLIKELY (fd < 0))
        ಹಿಂತಿರುಗಿ fd;

      ret = ಸುರಕ್ಷಿತ_write (fd, map_name, map, map_len, err);
      ಆದರೆ (ret < 0 && (! mappings_len || is_single_mapping (mappings, mappings_len, host_id, container_id)))
        {
          size_t single_mapping_len;
          cleanup_free ಅಕ್ಷರ *single_mapping = ಖಾಲಿ;
          crun_error_release (err);

          ಆದರೆ (! skip_if_setgroups_denied || ! get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->deny_setgroups)
            {
              ret = deny_setgroups (container, pid, err);
              ಆದರೆ (UNLIKELY (ret < 0))
                ಹಿಂತಿರುಗಿ ret;
            }

          ret = format_mount_mapping (&single_mapping, container_id, host_id, 1, &single_mapping_len, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;

          close_and_reset (&fd);

          fd = libcrun_open_proc_pid_file (container, pid, map_name, O_WRONLY, err);
          ಆದರೆ (UNLIKELY (fd < 0))
            ಹಿಂತಿರುಗಿ fd;

          ret = ಸುರಕ್ಷಿತ_write (fd, map_name, single_mapping, single_mapping_len, err);
        }
    }
  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
libcrun_ಗಣ_usernamespace (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *uid_map = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *gid_map = ಖಾಲಿ;
  size_t uid_map_len = 0, gid_map_len = 0;
  ಪೂರ್ಣಾಂಕ ret = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  ಆದರೆ ((get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->unshare_flags & CLONE_NEWUSER) == 0)
    ಹಿಂತಿರುಗಿ 0;

  ಆದರೆ (! def || ! def->linux)
    ಹಿಂತಿರುಗಿ 0;

  ಆದರೆ (def->linux->uid_mappings_len)
    {
      ret = format_mount_mappings (&uid_map, def->linux->uid_mappings, def->linux->uid_mappings_len, &uid_map_len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ret = format_default_id_mapping (&uid_map, container->container_uid, container->host_uid, container->host_uid, 1, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      uid_map_len = (size_t) ret;
      ಆದರೆ (uid_map == ಖಾಲಿ)
        {
          ret = format_mount_mapping (&uid_map, 0, container->host_uid, container->host_uid + 1, &uid_map_len, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }
    }

  ಆದರೆ (def->linux->gid_mappings_len)
    {
      ret = format_mount_mappings (&gid_map, def->linux->gid_mappings, def->linux->gid_mappings_len, &gid_map_len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ret = format_default_id_mapping (&gid_map, container->container_gid, container->host_uid, container->host_gid, 0, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      gid_map_len = (size_t) ret;
      ಆದರೆ (gid_map == ಖಾಲಿ)
        {
          ret = format_mount_mapping (&gid_map, 0, container->host_gid, container->host_gid + 1, &gid_map_len, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }
    }

  ret = write_id_map (container, pid, "gid_map", newgidmap, "newgidmap",
                      ! def->linux->uid_mappings_len, gid_map, gid_map_len,
                      def->linux->gid_mappings, def->linux->gid_mappings_len,
                      container->host_gid, container->container_gid, /* skip_if_setgroups_denied */ false, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = write_id_map (container, pid, "uid_map", newuidmap, "newuidmap",
                      ! def->linux->uid_mappings_len, uid_map, uid_map_len,
                      def->linux->uid_mappings, def->linux->uid_mappings_len,
                      container->host_uid, container->container_uid, /* skip_if_setgroups_denied */ true, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ 0;
}

#ವ್ಯಾಖ್ಯಾನಿಸು CAP_ಗಮ್ಯ_ಮುಖವಾಡ_0(x) (1L << ((x) & 31))
#ವ್ಯಾಖ್ಯಾನಿಸು CAP_ಗಮ್ಯ_ಮುಖವಾಡ_1(x) CAP_ಗಮ್ಯ_ಮುಖವಾಡ_0 (x - 32)

ರಚನೆ all_caps_s
{
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ effective[2];
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ permitted[2];
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ inheritable[2];
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ambient[2];
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ bounding[2];
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
has_cap_on (ಪೂರ್ಣಾಂಕ cap, ದೀರ್ಘ ಚಿಹ್ನೆರಹಿತ *caps)
{
  ಆದರೆ (cap < 32)
    ಹಿಂತಿರುಗಿ CAP_ಗಮ್ಯ_ಮುಖವಾಡ_0 (cap) & caps[0];
  ಹಿಂತಿರುಗಿ (CAP_ಗಮ್ಯ_ಮುಖವಾಡ_1 (cap) & caps[1]);
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cap_ಕೊನೆಯ_cap;

ಪೂರ್ಣಾಂಕ
libcrun_init_caps (libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
  ಪೂರ್ಣಾಂಕ ret;
  ಅಕ್ಷರ ಬಫರ್[32];
  fd = libcrun_open_proc_file (container, "sys/kernel/cap_last_cap", O_RDONLY, err);
  ಆದರೆ (fd < 0)
    ಹಿಂತಿರುಗಿ fd;
  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (fd, ಬಫರ್, ಗಾತ್ರ (ಬಫರ್) - 1));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "read from `/proc/sys/kernel/cap_last_cap`");
  ಬಫರ್[ret] = '\0';

  errno = 0;
  cap_ಕೊನೆಯ_cap = strtoul (ಬಫರ್, ಖಾಲಿ, 10);
  ಆದರೆ (errno != 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "strtoul `%s` from `/proc/sys/kernel/cap_last_cap`", ಬಫರ್);
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಗಣ_required_caps (ರಚನೆ all_caps_s *caps, uid_t uid, gid_t gid, ಪೂರ್ಣಾಂಕ no_new_privs, bool no_exec, libcrun_error_t *err)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CAP
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cap;
  ಪೂರ್ಣಾಂಕ ret;
  ರಚನೆ __user_cap_header_struct hdr = { LINUX_CAPABILITY_ಆವೃತ್ತಿ_3, 0 };
  ರಚನೆ __user_cap_data_struct ದತ್ತಾಂಶ[2] = { { 0 } };

  ಆದರೆ (cap_ಕೊನೆಯ_cap == 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: max number of capabilities not initialized");

  ಗಾಗಿ (cap = 0; cap <= cap_ಕೊನೆಯ_cap; cap++)
    ಆದರೆ (! has_cap_on (cap, caps->bounding))
      {
        ret = prctl (PR_CAPBSET_DROP, cap, 0, 0, 0);
        ಆದರೆ (UNLIKELY (ret < 0 && ! (errno == EINVAL)))
          ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "prctl drop bounding");
      }

  ದತ್ತಾಂಶ[0].effective = caps->effective[0];
  ದತ್ತಾಂಶ[1].effective = caps->effective[1];
  ದತ್ತಾಂಶ[0].inheritable = caps->inheritable[0];
  ದತ್ತಾಂಶ[1].inheritable = caps->inheritable[1];
  ದತ್ತಾಂಶ[0].permitted = caps->permitted[0];
  ದತ್ತಾಂಶ[1].permitted = caps->permitted[1];

  ret = prctl (PR_ಗಣ_KEEPCAPS, 1, 0, 0, 0);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "prctl set KEEPCAPS");

  /* -1 means "leave unchanged" for setresgid and setresuid.  */
  ಆದರೆ (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    ಹಿಂತಿರುಗಿ crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresgid (gid, gid, gid);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "setresgid to `%d`", gid);

  ret = setresuid (uid, uid, uid);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = capset (&hdr, ದತ್ತಾಂಶ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "capset");

#  ifdef PR_CAP_AMBIENT
  ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_CLEAR_ALL, 0, 0, 0);
  ಆದರೆ (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "prctl reset ambient");

  ಗಾಗಿ (cap = 0; cap <= cap_ಕೊನೆಯ_cap; cap++)
    ಆದರೆ (has_cap_on (cap, caps->ambient))
      {
        ret = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, cap, 0, 0);
        ಆದರೆ (UNLIKELY (ret < 0 && ! (errno == EINVAL || errno == EPERM)))
          ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "prctl ambient raise");
      }
#  endif

  /* If the workload runs in the current process, there is no execve(2) that resets KEEPCAPS and,
     for a non-root user, leaves only the ambient set in the permitted and effective sets.  */
  ಆದರೆ (no_exec)
    {
      ret = prctl (PR_ಗಣ_KEEPCAPS, 0, 0, 0, 0);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "prctl reset KEEPCAPS");

      ಆದರೆ (uid != 0)
        {
          ದತ್ತಾಂಶ[0].permitted = ದತ್ತಾಂಶ[1].permitted = 0;
#  ifdef PR_CAP_AMBIENT
          ಗಾಗಿ (cap = 0; cap <= cap_ಕೊನೆಯ_cap; cap++)
            ಆದರೆ (prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_IS_ಗಣ, cap, 0, 0) > 0)
              ದತ್ತಾಂಶ[cap / 32].permitted |= CAP_ಗಮ್ಯ_ಮುಖವಾಡ_0 (cap);
#  endif
          ದತ್ತಾಂಶ[0].effective = ದತ್ತಾಂಶ[0].permitted;
          ದತ್ತಾಂಶ[1].effective = ದತ್ತಾಂಶ[1].permitted;

          ret = capset (&hdr, ದತ್ತಾಂಶ);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "capset");
        }
    }
#ಆದರೆ_ಅಂತ್ಯ

  ಆದರೆ (no_new_privs)
    ಆದರೆ (UNLIKELY (prctl (PR_ಗಣ_NO_NEW_PRIVS, 1, 0, 0, 0) < 0))
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "prctl set no new privs");

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
read_caps (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ caps[2], ಅಕ್ಷರ **ಮೌಲ್ಯಗಳು, size_t len, libcrun_error_t *err)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CAP
  size_t i;
  ಗಾಗಿ (i = 0; i < len; i++)
    {
      cap_ಮೌಲ್ಯ_t cap;
      ಆದರೆ (cap_from_name (ಮೌಲ್ಯಗಳು[i], &cap) < 0)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "unknown capability `%s`", ಮೌಲ್ಯಗಳು[i]);

      ಆದರೆ (cap < 32)
        caps[0] |= CAP_ಗಮ್ಯ_ಮುಖವಾಡ_0 (cap);
      ಇಲ್ಲದಿದ್ದರೆ
        caps[1] |= CAP_ಗಮ್ಯ_ಮುಖವಾಡ_1 (cap);
    }
#ಇಲ್ಲದಿದ್ದರೆ
  caps[0] = 0;
  caps[1] = 0;
#ಆದರೆ_ಅಂತ್ಯ
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_ಗಣ_selinux_label (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  ಆದರೆ (proc->selinux_label)
    ಹಿಂತಿರುಗಿ ಗಣ_selinux_label (container, proc->selinux_label, now, err);

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_ಗಣ_apparmor_profile (libcrun_container_t *container, runtime_spec_schema_config_schema_process *proc, bool now, libcrun_error_t *err)
{
  ಆದರೆ (proc->apparmor_profile)
    ಹಿಂತಿರುಗಿ ಗಣ_apparmor_profile (container, proc->apparmor_profile, proc->no_new_privileges, now, err);
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_ಗಣ_caps (runtime_spec_schema_config_schema_process_capabilities *capabilities, uid_t uid, gid_t gid,
                  ಪೂರ್ಣಾಂಕ no_new_privileges, bool no_exec, libcrun_error_t *err)
{
  ರಚನೆ all_caps_s caps = {};
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (capabilities)
    {
      ret = read_caps (caps.effective, capabilities->effective, capabilities->effective_len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = read_caps (caps.inheritable, capabilities->inheritable, capabilities->inheritable_len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = read_caps (caps.ambient, capabilities->ambient, capabilities->ambient_len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = read_caps (caps.bounding, capabilities->bounding, capabilities->bounding_len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = read_caps (caps.permitted, capabilities->permitted, capabilities->permitted_len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಹಿಂತಿರುಗಿ ಗಣ_required_caps (&caps, uid, gid, no_new_privileges, no_exec, err);
}

ರಚನೆ rlimit_s
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
  ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ;
};

ರಚನೆ rlimit_s rlimits[] = { { "RLIMIT_AS", RLIMIT_AS },
                              { "RLIMIT_CORE", RLIMIT_CORE },
                              { "RLIMIT_CPU", RLIMIT_CPU },
                              { "RLIMIT_DATA", RLIMIT_ದತ್ತಾಂಶ },
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
                              { ಖಾಲಿ, 0 } };

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_rlimit_resource (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
  ರಚನೆ rlimit_s *it;
  ಗಾಗಿ (it = rlimits; it->name; it++)
    ಆದರೆ (strcmp (it->name, name) == 0)
      ಹಿಂತಿರುಗಿ it->ಮೌಲ್ಯ;
  ಹಿಂತಿರುಗಿ -1;
}

ಪೂರ್ಣಾಂಕ
libcrun_ಗಣ_rlimits (runtime_spec_schema_config_schema_process_rlimits_element **new_rlimits, size_t len,
                     libcrun_error_t *err)
{
  size_t i, j;
  /* Check for duplicate rlimit types.  */
  ಗಾಗಿ (i = 0; i < len; i++)
    {
      ಗಾಗಿ (j = i + 1; j < len; j++)
        {
          ಆದರೆ (strcmp (new_rlimits[i]->type, new_rlimits[j]->type) == 0)
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "duplicate rlimit `%s`", new_rlimits[i]->type);
        }
    }

  ಗಾಗಿ (i = 0; i < len; i++)
    {
      ರಚನೆ rlimit ಎಲ್ಲೆ;
      ಅಕ್ಷರ *type = new_rlimits[i]->type;
      ಪೂರ್ಣಾಂಕ resource = get_rlimit_resource (type);
      ಆದರೆ (UNLIKELY (resource < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid rlimit `%s`", type);
      ಎಲ್ಲೆ.rlim_cur = new_rlimits[i]->soft;
      ಎಲ್ಲೆ.rlim_max = new_rlimits[i]->hard;
      libcrun_debug ("Set rlimit: soft = `%llu`, hard = `%llu`",
                     (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ಎಲ್ಲೆ.rlim_cur,
                     (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ಎಲ್ಲೆ.rlim_max);
      ಆದರೆ (UNLIKELY (setrlimit (resource, &ಎಲ್ಲೆ) < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "setrlimit `%s`", type);
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_ಗಣ_hostname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಪೂರ್ಣಾಂಕ has_uts = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->unshare_flags & CLONE_NEWUTS;
  ಪೂರ್ಣಾಂಕ ret;
  ಆದರೆ (def->hostname == ಖಾಲಿ || def->hostname[0] == '\0')
    ಹಿಂತಿರುಗಿ 0;
  ಆದರೆ (! has_uts)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "hostname requires the UTS namespace");
  ret = sethostname (def->hostname, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (def->hostname));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "sethostname");
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_ಗಣ_domainname (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಪೂರ್ಣಾಂಕ has_uts = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->unshare_flags & CLONE_NEWUTS;
  ಪೂರ್ಣಾಂಕ ret;
  ಆದರೆ (is_ಖಾಲಿಗಳು_string (def->domainname))
    ಹಿಂತಿರುಗಿ 0;
  ಆದರೆ (! has_uts)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "domainname requires the UTS namespace");
  ret = setdomainname (def->domainname, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (def->domainname));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "setdomainname");
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_ಗಣ_oom (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
  ಪೂರ್ಣಾಂಕ ret;
  ಅಕ್ಷರ oom_ಬಫರ್[16];
  ಆದರೆ (def->process == ಖಾಲಿ || ! def->process->oom_score_adj_present)
    ಹಿಂತಿರುಗಿ 0;
  libcrun_debug ("Write OOM score adj: `%d`", def->process->oom_score_adj);

  ret = snprintf (oom_ಬಫರ್, ಗಾತ್ರ (oom_ಬಫರ್), "%i", def->process->oom_score_adj);
  ಆದರೆ (UNLIKELY (ret >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (oom_ಬಫರ್)))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

  fd = libcrun_open_proc_file (container, "self/oom_score_adj", O_RDWR, err);
  ಆದರೆ (fd < 0)
    ಹಿಂತಿರುಗಿ fd;
  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (fd, oom_ಬಫರ್, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (oom_ಬಫರ್)));
  ಆದರೆ (ret < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "write to `/proc/self/oom_score_adj`");
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *sysctlRequiringIPC[] = {
  "kernel/msgmax",
  "kernel/msgmnb",
  "kernel/msgmni",
  "kernel/sem",
  "kernel/shmall",
  "kernel/shmmax",
  "kernel/shmmni",
  "kernel/shm_rmid_forced",
  ಖಾಲಿ
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
validate_sysctl (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *original_ಕೀಲಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *original_ಮೌಲ್ಯ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ namespaces_created, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *namespace = "";

  /* The name is used as a relative path with openat(), so a leading '/' would
     make it escape /proc/sys altogether, and a ".." component could reach a
     sysctl other than the one being validated here.  */
  ಆದರೆ (name[0] == '/' || ಮಾರ್ಗ_has_dot_dot_component (name))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "the sysctl `%s` contains an invalid path", original_ಕೀಲಿ);

  ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (name, "fs/mqueue/"))
    {
      ಆದರೆ (namespaces_created & CLONE_NEWIPC)
        ಹಿಂತಿರುಗಿ 0;

      namespace = "IPC";
      ಹೋಗು fail;
    }

  ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (name, "kernel/"))
    {
      size_t i;

      ಗಾಗಿ (i = 0; sysctlRequiringIPC[i]; i++)
        ಆದರೆ (strcmp (sysctlRequiringIPC[i], name) == 0)
          {
            ಆದರೆ (namespaces_created & CLONE_NEWIPC)
              ಹಿಂತಿರುಗಿ 0;

            namespace = "IPC";
            ಹೋಗು fail;
          }

      ಆದರೆ (strcmp (name, "kernel/domainname") == 0)
        {
          // Value of sysctl `kernel/domainname` is going to
          // conflict with already set field `domainname` in
          // OCI spec, in such scenario crun will fail to prevent
          // unexpected behaviour for end user.
          ಆದರೆ (! is_ಖಾಲಿಗಳು_string (def->domainname) && (strcmp (original_ಮೌಲ್ಯ, def->domainname) != 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `domainname`", original_ಕೀಲಿ);

          ಆದರೆ (namespaces_created & CLONE_NEWUTS)
            ಹಿಂತಿರುಗಿ 0;

          namespace = "UTS";
          ಹೋಗು fail;
        }

      ಆದರೆ (strcmp (name, "kernel/hostname") == 0)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "the sysctl `%s` conflicts with OCI field `hostname`", original_ಕೀಲಿ);
    }
  ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (name, "net/"))
    {
      ಆದರೆ (namespaces_created & CLONE_NEWNET)
        ಹಿಂತಿರುಗಿ 0;

      namespace = "network";
      ಹೋಗು fail;
    }

  ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "the sysctl `%s` is not namespaced", original_ಕೀಲಿ);

fail:
  ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "the sysctl `%s` requires a new %s namespace", original_ಕೀಲಿ, namespace);
}

/* Best-effort attempt to give a better explanation why setting a sysctl could have failed. */
ಸ್ಥಿರ ಅಕ್ಷರ *
sysctl_error_reason (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ namespaces_created, ಪೂರ್ಣಾಂಕ errno_)
{

  ಆದರೆ (strcmp (name, "net.ipv4.ping_group_range") == 0 && (errno_ == EINVAL) && (namespaces_created & CLONE_NEWUSER))
    ಹಿಂತಿರುಗಿ xstrdup ("are all the IDs mapped in the user namespace?");

  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಪೂರ್ಣಾಂಕ
libcrun_ಗಣ_sysctl (libcrun_container_t *container, libcrun_error_t *err)
{
  size_t i;
  cleanup_close ಪೂರ್ಣಾಂಕ dirfd = -1;
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ namespaces_created = 0;
  runtime_spec_schema_config_schema *def = container->container_def;

  ಆದರೆ (def->linux == ಖಾಲಿ || def->linux->sysctl == ಖಾಲಿ || def->linux->sysctl->len == 0)
    ಹಿಂತಿರುಗಿ 0;

  ಗಾಗಿ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ;

      ಮೌಲ್ಯ = libcrun_ಕಂಡುಹಿಡಿ_namespace (def->linux->namespaces[i]->type);
      ಆದರೆ (UNLIKELY (ಮೌಲ್ಯ < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      namespaces_created |= ಮೌಲ್ಯ;
    }

  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container);
  dirfd = libcrun_open_proc_file (container, "sys", O_DIRECTORY | O_ಮಾರ್ಗ, err);
  ಆದರೆ (UNLIKELY (dirfd < 0))
    ಹಿಂತಿರುಗಿ dirfd;

  ಗಾಗಿ (i = 0; i < def->linux->sysctl->len; i++)
    {
      cleanup_free ಅಕ್ಷರ *name = ಖಾಲಿ;
      cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
      ಪೂರ್ಣಾಂಕ ret;

      /* A sysctl whose value is null in the document is parsed into a NULL,
         and there is nothing to write to the file.  */
      ಆದರೆ (def->linux->sysctl->ಕೀಲಿಗಳು[i] == ಖಾಲಿ || def->linux->sysctl->ಮೌಲ್ಯಗಳು[i] == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ crun_make_error (err, EINVAL, "sysctl value is not specified");

      name = libcrun_sysctl_ಕೀಲಿ_ಗಮ್ಯ_proc_ಮಾರ್ಗ (def->linux->sysctl->ಕೀಲಿಗಳು[i]);

      ret = validate_sysctl (def->linux->sysctl->ಕೀಲಿಗಳು[i], def->linux->sysctl->ಮೌಲ್ಯಗಳು[i], name, namespaces_created, def, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      fd = openat (dirfd, name, O_WRONLY | O_CLOEXEC);
      ಆದರೆ (UNLIKELY (fd < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `/proc/sys/%s`", name);

      ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (fd, def->linux->sysctl->ಮೌಲ್ಯಗಳು[i], ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (def->linux->sysctl->ಮೌಲ್ಯಗಳು[i])));
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          ಪೂರ್ಣಾಂಕ saved_errno = errno;
          cleanup_free ಅಕ್ಷರ *reason = ಖಾಲಿ;

          reason = sysctl_error_reason (def->linux->sysctl->ಕೀಲಿಗಳು[i], namespaces_created, saved_errno);
          ಹಿಂತಿರುಗಿ crun_make_error (err, saved_errno, "write to `/proc/sys/%s`%s%s%s", name, reason ? " (" : "", reason ?: "", reason ? ")" : "");
        }
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
open_terminal (ಅಕ್ಷರ **pty, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
  uid_t uid = 0;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (process && process->user)
    uid = process->user->uid;

  fd = libcrun_new_terminal (pty, err);
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ fd;

  ret = libcrun_ಗಣ_stdio (fd, *pty, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (uid)
    {
      /* The slave is now stdin, use the descriptor instead of resolving the path again.  */
      ret = fchown (0, uid, -1);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "chown `%s`", *pty);
    }

  ret = get_and_reset (&fd);
  ಹಿಂತಿರುಗಿ ret;
}

ಅಕ್ಷರ *
libcrun_get_external_descriptors (libcrun_container_t *container)
{
  ಹಿಂತಿರುಗಿ get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->external_descriptors;
}

ಪೂರ್ಣಾಂಕ
libcrun_save_external_descriptors (libcrun_container_t *container, libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *buf = ಖಾಲಿ;
  json_gen_ctx *gen = ಖಾಲಿ;
  size_t buf_len;
  ಪೂರ್ಣಾಂಕ ret;
  ಪೂರ್ಣಾಂಕ i;

  ಆದರೆ (! json_gen_init (&gen, ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "json_gen_init");

  ret = json_gen_array_open (gen);
  ಆದರೆ (UNLIKELY (ret != json_gen_ಸ್ಥಿತಿ_ok))
    ಹೋಗು json_error;

  /* Remember original stdin, stdout, stderr for container restore.  */
  ಗಾಗಿ (i = 0; i < 3; i++)
    {
      proc_fd_ಮಾರ್ಗ_t fd_ಮಾರ್ಗ;
      ಅಕ್ಷರ link_ಮಾರ್ಗ[PATH_MAX];

      get_proc_self_fd_ಮಾರ್ಗ (fd_ಮಾರ್ಗ, i);

      ret = readlink (fd_ಮಾರ್ಗ, link_ಮಾರ್ಗ, PATH_MAX - 1);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          /* The fd could not exist.  */
          ಆದರೆ (errno == ENOENT)
            {
              strcpy (link_ಮಾರ್ಗ, "/dev/null");
              ret = 9; /* strlen ("/dev/null").  */
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              json_gen_free (gen);
              ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "readlink `%s`", fd_ಮಾರ್ಗ);
            }
        }
      link_ಮಾರ್ಗ[ret] = 0;

      ret = json_gen_string (gen, link_ಮಾರ್ಗ, ret);
      ಆದರೆ (UNLIKELY (ret != json_gen_ಸ್ಥಿತಿ_ok))
        ಹೋಗು json_error;
    }

  ret = json_gen_array_close (gen);
  ಆದರೆ (UNLIKELY (ret != json_gen_ಸ್ಥಿತಿ_ok))
    ಹೋಗು json_error;

  ret = json_gen_get_buf (gen, &buf, &buf_len);
  ಆದರೆ (UNLIKELY (ret != json_gen_ಸ್ಥಿತಿ_ok))
    ಹೋಗು json_error;

  ಆದರೆ (buf)
    {
      ಅಕ್ಷರ *b = xmalloc (buf_len + 1);
      ಸ್ಮರಣೆ_ನಕಲು (b, buf, buf_len);
      b[buf_len] = '\0';
      get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->external_descriptors = b;
    }

  json_gen_free (gen);

  ಹಿಂತಿರುಗಿ 0;

json_error:
  ಆದರೆ (gen)
    json_gen_free (gen);
  ಹಿಂತಿರುಗಿ json_gen_error_ಗಮ್ಯ_crun_error (ret, err);
}

ಪೂರ್ಣಾಂಕ
libcrun_ಗಣ_terminal (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_free ಅಕ್ಷರ *pty = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (def->process == ಖಾಲಿ || ! def->process->terminal)
    ಹಿಂತಿರುಗಿ 0;

  fd = open_terminal (&pty, def->process, err);
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ fd;

  ಆದರೆ (def->process->console_ಗಾತ್ರ)
    {
      ret = libcrun_terminal_ಸಿದ್ಧತೆ_ಗಾತ್ರ (0, def->process->console_ಗಾತ್ರ->ಎತ್ತರ, def->process->console_ಗಾತ್ರ->ಅಗಲ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->mount_dev_from_host)
    {
      ret = do_mount (container, pty, -1, "/dev/console", ಖಾಲಿ, MS_BIND, ಖಾಲಿ, LABEL_MOUNT, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ret = unlink ("/dev/console");
      ಆದರೆ (UNLIKELY (ret < 0 && errno != ENOENT))
        {
          ಆದರೆ (errno == EROFS)
            {
              /* If the file system is read-only, fall back to a bind mount.  */
              ret = do_mount (container, pty, -1, "/dev/console", ಖಾಲಿ, MS_BIND, ಖಾಲಿ, LABEL_MOUNT, err);
              ಆದರೆ (UNLIKELY (ret < 0))
                ಹಿಂತಿರುಗಿ ret;

              ಹಿಂತಿರುಗಿ get_and_reset (&fd);
            }
          ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "unlink `/dev/console`");
        }

      ret = symlink (pty, "/dev/console");
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "symlink `/dev/console` -> `%s`", pty);
    }

  ಹಿಂತಿರುಗಿ get_and_reset (&fd);
}

ಸ್ಥಿರ bool
read_error_from_sync_socket (ಪೂರ್ಣಾಂಕ sync_socket_fd, ಪೂರ್ಣಾಂಕ *error, ಅಕ್ಷರ **str)
{
  cleanup_free ಅಕ್ಷರ *b = ಖಾಲಿ;
  size_t ಗಾತ್ರ_2;
  ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2 = 0;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (*error == 0)
    {
      ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (sync_socket_fd, &ಸಂಕೇತ_2, ಗಾತ್ರ (ಸಂಕೇತ_2)));
      ಆದರೆ (UNLIKELY (ret != (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (ಸಂಕೇತ_2)))
        ಹಿಂತಿರುಗಿ false;
      *error = ಸಂಕೇತ_2;
    }

  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (sync_socket_fd, &ಗಾತ್ರ_2, ಗಾತ್ರ (ಗಾತ್ರ_2)));
  ಆದರೆ (UNLIKELY (ret != (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (ಗಾತ್ರ_2)))
    ಹಿಂತಿರುಗಿ false;

  ಆದರೆ (ಗಾತ್ರ_2 == 0)
    ಹಿಂತಿರುಗಿ false;

  ಆದರೆ (ಗಾತ್ರ_2 > 1024)
    ಗಾತ್ರ_2 = 1024;

  b = xmalloc (ಗಾತ್ರ_2 + 1);
  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (sync_socket_fd, b, ಗಾತ್ರ_2));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ false;

  b[ret] = '\0';

  *str = b;
  b = ಖಾಲಿ;
  ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
send_error_ಗಮ್ಯ_sync_socket (ಪೂರ್ಣಾಂಕ sync_socket_fd, bool has_fd, libcrun_error_t *err)
{
  size_t ಗಾತ್ರ_2;
  ಪೂರ್ಣಾಂಕ ret;
  ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2;
  ಅಕ್ಷರ *ಸಂದೇಶ;

  ಆದರೆ (err == ಖಾಲಿ || *err == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ false;

  ಸಂಕೇತ_2 = crun_error_get_errno (err);

  ಆದರೆ (has_fd)
    {
      /* dummy terminal fd.  */
      ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (sync_socket_fd, "1", 1));
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ false;
    }

  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (sync_socket_fd, &ಸಂಕೇತ_2, ಗಾತ್ರ (ಸಂಕೇತ_2)));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ false;

  ಸಂದೇಶ = (*err)->ಸಂದೇಶ;
  ಗಾತ್ರ_2 = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಸಂದೇಶ) + 1;
  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (sync_socket_fd, &ಗಾತ್ರ_2, ಗಾತ್ರ (ಗಾತ್ರ_2)));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ false;

  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (sync_socket_fd, ಸಂದೇಶ, ಗಾತ್ರ_2));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ false;

  ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
send_success_ಗಮ್ಯ_sync_socket (ಪೂರ್ಣಾಂಕ sync_socket, libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ success = 0;
  ಪೂರ್ಣಾಂಕ ret;

  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (sync_socket, &success, ಗಾತ್ರ (success)));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "write to sync socket");

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ __attribute__ ((noreturn)) ಶೂನ್ಯ
send_error_ಗಮ್ಯ_sync_socket_and_die (ಪೂರ್ಣಾಂಕ sync_socket_fd, bool has_terminal, libcrun_error_t *err)
{
  ಅಕ್ಷರ *ಸಂದೇಶ;

  ಆದರೆ (err == ಖಾಲಿ || *err == ಖಾಲಿ)
    ಸುರಕ್ಷಿತ_exit (EXIT_FAILURE);

  ಆದರೆ (! send_error_ಗಮ್ಯ_sync_socket (sync_socket_fd, has_terminal, err))
    {
      errno = crun_error_get_errno (err);
      ಸಂದೇಶ = (*err)->ಸಂದೇಶ;
      libcrun_fail_with_error (errno, "%s", ಸಂದೇಶ);
    }
  ಸುರಕ್ಷಿತ_exit (EXIT_FAILURE);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
expect_success_from_sync_socket (ಪೂರ್ಣಾಂಕ sync_fd, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *err_str = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ res = 1;
  ಪೂರ್ಣಾಂಕ ret;

  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (sync_fd, &res, ಗಾತ್ರ (res)));
  ಆದರೆ (UNLIKELY (ret != ಗಾತ್ರ (res)))
    ಹಿಂತಿರುಗಿ crun_make_error (err, ret < 0 ? errno : 0, "read status from sync socket");

  ಆದರೆ (res == 0)
    ಹಿಂತಿರುಗಿ 0;

  ಆದರೆ (read_error_from_sync_socket (sync_fd, &res, &err_str))
    {
      ಆದರೆ (! is_ಖಾಲಿಗಳು_string (err_str))
        ಹಿಂತಿರುಗಿ crun_make_error (err, res, "%s", err_str);
    }

  ಹಿಂತಿರುಗಿ crun_make_error (err, res, "read from sync socket");
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
join_namespaces (runtime_spec_schema_config_schema *def, ಪೂರ್ಣಾಂಕ *namespaces_ಗಮ್ಯ_join, ಪೂರ್ಣಾಂಕ n_namespaces_ಗಮ್ಯ_join,
                 ಪೂರ್ಣಾಂಕ *namespaces_ಗಮ್ಯ_join_ಸೂಚ್ಯಂಕ, bool ignore_join_errors, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  ಪೂರ್ಣಾಂಕ i;

  ಗಾಗಿ (i = 0; i < n_namespaces_ಗಮ್ಯ_join; i++)
    {
      cleanup_free ಅಕ್ಷರ *cwd = ಖಾಲಿ;
      ಪೂರ್ಣಾಂಕ orig_ಸೂಚ್ಯಂಕ = namespaces_ಗಮ್ಯ_join_ಸೂಚ್ಯಂಕ[i];
      ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ;

      ಆದರೆ (namespaces_ಗಮ್ಯ_join[i] < 0)
        ಮುಂದುವರಿ;

      /* Skip the user namespace.  */
      ಮೌಲ್ಯ = libcrun_ಕಂಡುಹಿಡಿ_namespace (def->linux->namespaces[orig_ಸೂಚ್ಯಂಕ]->type);
      ಆದರೆ (ಮೌಲ್ಯ == CLONE_NEWUSER)
        ಮುಂದುವರಿ;

      ಆದರೆ (ಮೌಲ್ಯ == CLONE_NEWNS)
        {
          cwd = getcwd (ಖಾಲಿ, 0);
          ಆದರೆ (UNLIKELY (cwd == ಖಾಲಿ))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "getcwd");
        }

      libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[orig_ಸೂಚ್ಯಂಕ]->type, def->linux->namespaces[orig_ಸೂಚ್ಯಂಕ]->ಮಾರ್ಗ);
      ret = setns (namespaces_ಗಮ್ಯ_join[i], ಮೌಲ್ಯ);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          ಆದರೆ (ignore_join_errors)
            ಮುಂದುವರಿ;
          ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "setns `%s`", def->linux->namespaces[orig_ಸೂಚ್ಯಂಕ]->ಮಾರ್ಗ);
        }

      close_and_reset (&namespaces_ಗಮ್ಯ_join[i]);

      ಆದರೆ (ಮೌಲ್ಯ == CLONE_NEWNS)
        {
          ret = chdir (cwd);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "chdir `%s`", cwd);
        }
    }
  ಹಿಂತಿರುಗಿ 0;
}

#ವ್ಯಾಖ್ಯಾನಿಸು MAX_NAMESPACES 10

ರಚನೆ init_ಸ್ಥಿತಿ_s
{
  /* fd to the namespace to join.  */
  ಪೂರ್ಣಾಂಕ fd[MAX_NAMESPACES + 1];
  /* Index into def->linux->namespaces.  */
  ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ[MAX_NAMESPACES];
  /* CLONE_* value.  */
  ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ[MAX_NAMESPACES];
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
  ಪೂರ್ಣಾಂಕ userns_ಸೂಚ್ಯಂಕ;
  /* def->linux->namespaces userns.  */
  ಪೂರ್ಣಾಂಕ userns_ಸೂಚ್ಯಂಕ_origin;

  /* All namespaces created/joined by the container.  */
  ಪೂರ್ಣಾಂಕ all_namespaces;

  /* What namespaces are still missing to be created.  */
  ಪೂರ್ಣಾಂಕ namespaces_ಗಮ್ಯ_unshare;

  /* Index in fd[] for the pidns that must be joined before any
     other namespace.  */
  ಪೂರ್ಣಾಂಕ idx_pidns_ಗಮ್ಯ_join_immediately;

  /* Index in fd[] for the timens that must be joined before any
     other namespace.  */
  ಪೂರ್ಣಾಂಕ idx_timens_ಗಮ್ಯ_join_immediately;
};

ಶೂನ್ಯ
cleanup_free_init_statusp (ರಚನೆ init_ಸ್ಥಿತಿ_s *ns)
{
  size_t i;

  ಆದರೆ (ns == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ;

  ಗಾಗಿ (i = 0; i < ns->fd_len; i++)
    ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (ns->fd[i]));
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
configure_init_ಸ್ಥಿತಿ (ರಚನೆ init_ಸ್ಥಿತಿ_s *ns, libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  size_t i;

  ಗಾಗಿ (i = 0; i < MAX_NAMESPACES + 1; i++)
    ns->fd[i] = -1;

  ns->fd_len = 0;
  ns->all_namespaces = 0;
  ns->namespaces_ಗಮ್ಯ_unshare = 0;
  ns->join_pidns = false;
  ns->join_ipcns = false;
  ns->must_fork = false;
  ns->can_delay_pidns_fork = false;
  ns->delayed_userns_create = false;
  ns->userns_ಸೂಚ್ಯಂಕ = -1;
  ns->userns_ಸೂಚ್ಯಂಕ_origin = -1;
  ns->idx_pidns_ಗಮ್ಯ_join_immediately = -1;
  ns->idx_timens_ಗಮ್ಯ_join_immediately = -1;

  ಗಾಗಿ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ = libcrun_ಕಂಡುಹಿಡಿ_namespace (def->linux->namespaces[i]->type);
      ಆದರೆ (UNLIKELY (ಮೌಲ್ಯ < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      ಆದರೆ (ns->all_namespaces & ಮೌಲ್ಯ)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "duplicate namespace type: `%s`", def->linux->namespaces[i]->type);

      ns->all_namespaces |= ಮೌಲ್ಯ;

      ಆದರೆ (def->linux->namespaces[i]->ಮಾರ್ಗ == ಖಾಲಿ)
        {
          libcrun_debug ("Unsharing namespace: `%s`", def->linux->namespaces[i]->type);
          ns->namespaces_ಗಮ್ಯ_unshare |= ಮೌಲ್ಯ;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಪೂರ್ಣಾಂಕ fd;

          libcrun_debug ("Joining `%s` namespace: `%s`", def->linux->namespaces[i]->type, def->linux->namespaces[i]->ಮಾರ್ಗ);

          ಆದರೆ (ns->fd_len >= MAX_NAMESPACES)
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "too many namespaces to join");

          fd = open (def->linux->namespaces[i]->ಮಾರ್ಗ, O_RDONLY | O_CLOEXEC);
          ಆದರೆ (UNLIKELY (fd < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", def->linux->namespaces[i]->ಮಾರ್ಗ);

          ಆದರೆ (ಮೌಲ್ಯ == CLONE_NEWUSER)
            {
              ns->userns_ಸೂಚ್ಯಂಕ = ns->fd_len;
              ns->userns_ಸೂಚ್ಯಂಕ_origin = i;
            }

          ns->fd[ns->fd_len] = fd;
          ns->ಸೂಚ್ಯಂಕ[ns->fd_len] = i;
          ns->ಮೌಲ್ಯ[ns->fd_len] = ಮೌಲ್ಯ;
          ns->fd_len++;
          ns->fd[ns->fd_len] = -1;
        }
    }

  ಆದರೆ (container->host_uid && (ns->all_namespaces & CLONE_NEWUSER) == 0)
    {
      libcrun_warning ("non root user need to have an 'user' namespace");
      ns->all_namespaces |= CLONE_NEWUSER;
      ns->namespaces_ಗಮ್ಯ_unshare |= CLONE_NEWUSER;
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* Detect if root is available in the container.  */
ಸ್ಥಿರ bool
ಬೇರು_mapped_ಒಳ_container_p (runtime_spec_schema_defs_id_mapping **mappings, size_t len)
{
  size_t i;

  ಗಾಗಿ (i = 0; i < len; i++)
    ಆದರೆ (mappings[i]->container_id == 0)
      ಹಿಂತಿರುಗಿ true;

  ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ರಚನೆ libcrun_fd_map *
get_devices_fd_map (libcrun_container_t *container)
{
  ರಚನೆ libcrun_fd_map *dev_fds = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->dev_fds;

  ಆದರೆ (dev_fds == ಖಾಲಿ)
    {
      runtime_spec_schema_config_schema *def = container->container_def;
      size_t len = def->linux ? def->linux->devices_len : 0;

      dev_fds = make_libcrun_fd_map (len);
      get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->dev_fds = dev_fds;
    }
  ಹಿಂತಿರುಗಿ dev_fds;
}

#ವ್ಯಾಖ್ಯಾನಿಸು ಅಂಕೆ_NEEDED_DEVS (ಗಾತ್ರ (needed_devs) / ಗಾತ್ರ (needed_devs[0]) - 1)

ಸ್ಥಿರ ರಚನೆ libcrun_fd_map *
get_needed_devs_fd_map (libcrun_container_t *container)
{
  ರಚನೆ libcrun_fd_map *fds = get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->needed_devs_fds;

  ಆದರೆ (fds == ಖಾಲಿ)
    {
      fds = make_libcrun_fd_map (ಅಂಕೆ_NEEDED_DEVS);
      get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->needed_devs_fds = fds;
    }
  ಹಿಂತಿರುಗಿ fds;
}

bool
is_bind_mount (runtime_spec_schema_defs_mount *mnt, bool *recursive, bool *src_nofollow)
{
  bool ret = false;
  size_t i;

  ಆದರೆ (recursive)
    *recursive = false;

  ಆದರೆ (src_nofollow != ಖಾಲಿ)
    *src_nofollow = false;

  ಗಾಗಿ (i = 0; i < mnt->options_len; i++)
    {
      ಆದರೆ (mnt->options[i] == ಖಾಲಿ)
        ಮುಂದುವರಿ;

      ಆದರೆ (strcmp (mnt->options[i], "bind") == 0)
        {
          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          ಆದರೆ (src_nofollow == ಖಾಲಿ || *src_nofollow)
            ನಿಲ್ಲಿಸು;
        }
      ಆದರೆ (strcmp (mnt->options[i], "rbind") == 0)
        {
          ಆದರೆ (recursive)
            *recursive = true;

          ret = true;

          /* if src_nofollow is not specified, or already found, shortcut.  */
          ಆದರೆ (src_nofollow == ಖಾಲಿ || *src_nofollow)
            ನಿಲ್ಲಿಸು;
        }
      ಆದರೆ (src_nofollow && strcmp (mnt->options[i], "src-nofollow") == 0)
        *src_nofollow = true;
    }
  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಅಕ್ಷರ *
get_idmapped_option (runtime_spec_schema_defs_mount *mnt, bool *recursive)
{
  size_t i;

  ಗಾಗಿ (i = 0; i < mnt->options_len; i++)
    {
      ಆದರೆ (mnt->options[i] == ಖಾಲಿ)
        ಮುಂದುವರಿ;

      ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (mnt->options[i], "idmap"))
        {
          *recursive = false;
          ಹಿಂತಿರುಗಿ mnt->options[i];
        }
      ಆದರೆ (has_ಪೂರ್ವಪ್ರತ್ಯಯ (mnt->options[i], "ridmap"))
        {
          *recursive = true;
          ಹಿಂತಿರುಗಿ mnt->options[i];
        }
    }
  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
open_mount_type (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *type, ಪೂರ್ಣಾಂಕ *ಹೊರಹೋಗು_fd, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ fsopen_fd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ newfs_fd = -1;
  ಪೂರ್ಣಾಂಕ ret;

  fsopen_fd = syscall_fsopen (type, FSOPEN_CLOEXEC);
  ಆದರೆ (UNLIKELY (fsopen_fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fsopen `%s`", type);

  ret = syscall_fsconfig (fsopen_fd, FSCONFIG_CMD_CREATE, ಖಾಲಿ, ಖಾಲಿ, 0);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fsconfig create `%s`", type);

  newfs_fd = syscall_fsmount (fsopen_fd, FSMOUNT_CLOEXEC, 0);
  ಆದರೆ (UNLIKELY (newfs_fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fsmount `%s`", type);

  *ಹೊರಹೋಗು_fd = get_and_reset (&newfs_fd);
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
maybe_get_idmapped_mount (libcrun_container_t *container, runtime_spec_schema_config_schema *def, runtime_spec_schema_defs_mount *mnt, pid_t pid, ಪೂರ್ಣಾಂಕ *ಹೊರಹೋಗು_fd, bool *has_mappings_ಹೊರಹೋಗು, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ newfs_fd = -1;
  cleanup_pid pid_t created_pid = -1;
  ರಚನೆ mount_attr_s attr = {
    0,
  };
  bool recursive_bind_mount = false;
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *idmap_option;
  bool recursive = false;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *options = ಖಾಲಿ;
  bool has_mappings;
  ಪೂರ್ಣಾಂಕ ret;
  ಅಕ್ಷರ *extra_ಸಂದೇಶ = "";
  bool nofollow = false;

  *ಹೊರಹೋಗು_fd = -1;

  idmap_option = get_idmapped_option (mnt, &recursive);

  has_mappings = mnt->uid_mappings_len > 0 || mnt->gid_mappings_len > 0 || (idmap_option != ಖಾಲಿ);
  ಆದರೆ (has_mappings_ಹೊರಹೋಗು)
    *has_mappings_ಹೊರಹೋಗು = has_mappings;
  ಆದರೆ (! has_mappings)
    ಹಿಂತಿರುಗಿ 0;

  ಆದರೆ ((mnt->uid_mappings == ಖಾಲಿ) != (mnt->gid_mappings == ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid mappings specified for the mount on `%s`", mnt->destination);

  /* If there are options specified, create a new user namespace with the configured mappings.  */
  ಆದರೆ (idmap_option)
    {
      options = strchr (idmap_option, '=');
      ಆದರೆ (options)
        {
          /* Skip the '=' itself.  */
          options++;
          ಆದರೆ (options[0] == '\0')
            options = ಖಾಲಿ;
        }
    }

  ret = maybe_create_userns_for_idmapped_mount (container, def, mnt, options, &created_pid, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;
  ಆದರೆ (created_pid > 0)
    pid = created_pid;

  fd = libcrun_open_proc_pid_file (container, pid, "ns/user", O_RDONLY, err);
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ fd;

  ಆದರೆ (is_bind_mount (mnt, &recursive_bind_mount, &nofollow))
    {
      newfs_fd = syscall_open_ವೃಕ್ಷ (AT_FDCWD, mnt->ಮೂಲ, (recursive_bind_mount ? AT_RECURSIVE : 0) | AT_NO_AUTOMOUNT | (nofollow ? AT_SYMLINK_NOFOLLOW : 0) | OPEN_TREE_CLOEXEC | OPEN_TREE_CLONE);
      ಆದರೆ (UNLIKELY (newfs_fd < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", mnt->ಮೂಲ);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ret = open_mount_type (mnt->type, &newfs_fd, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  attr.attr_set = MOUNT_ATTR_IDMAP;
  attr.userns_fd = fd;

  ret = syscall_mount_setattr (newfs_fd, "", AT_ಖಾಲಿಗಳು_ಮಾರ್ಗ | (recursive ? AT_RECURSIVE : 0), &attr);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ಆದರೆ (errno == EINVAL)
        {
          extra_ಸಂದೇಶ = " (maybe the file system used doesn't support idmap mounts on this kernel?)";
        }
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "mount_setattr `%s`%s", mnt->destination, extra_ಸಂದೇಶ);
    }

  *ಹೊರಹೋಗು_fd = get_and_reset (&newfs_fd);
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ uid_t
get_id_ಒಳ_user_namespace (uid_t id, bool is_uid, runtime_spec_schema_config_schema *def)
{
  runtime_spec_schema_defs_id_mapping **mappings;
  size_t len;
  size_t i;

  ಆದರೆ (! def || ! def->linux)
    ಹೋಗು ನಿರ್ಗಮಿಸು;

  mappings = is_uid ? def->linux->uid_mappings : def->linux->gid_mappings;
  len = is_uid ? def->linux->uid_mappings_len : def->linux->gid_mappings_len;

  ಆದರೆ (! mappings)
    ಹೋಗು ನಿರ್ಗಮಿಸು;

  ಗಾಗಿ (i = 0; i < len; i++)
    {
      ಆದರೆ (mappings[i]->container_id <= id
          && id < mappings[i]->container_id + mappings[i]->ಗಾತ್ರ_2)
        ಹಿಂತಿರುಗಿ id - mappings[i]->container_id + mappings[i]->host_id;
    }
ನಿರ್ಗಮಿಸು:
  ಹಿಂತಿರುಗಿ is_uid ? get_ಅತಿಪ್ರವಾಹ_uid () : get_ಅತಿಪ್ರವಾಹ_gid ();
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
precreate_device (libcrun_container_t *container, ಪೂರ್ಣಾಂಕ devs_dirfd, size_t i, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  runtime_spec_schema_defs_linux_device *device;
  uid_t uid;
  gid_t gid;
  ಅಕ್ಷರ name[64];
  mode_t type;
  dev_t dev;
  ಪೂರ್ಣಾಂಕ ret;

  ret = snprintf (name, ಗಾತ್ರ (name), "%zu", i);
  ಆದರೆ (UNLIKELY (ret >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (name)))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

  device = def->linux->devices[i];

  type = (device->type[0] == 'b') ? S_IFBLK : ((device->type[0] == 'p') ? S_IFIFO : S_IFCHR);
  dev = makedev (device->major, device->minor);

  ret = mknodat (devs_dirfd, name, device->file_ಬಹುಲಕ | type, dev);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "mknod `%s`", device->ಮಾರ್ಗ);

  uid = get_id_ಒಳ_user_namespace (device->uid, true, def);
  gid = get_id_ಒಳ_user_namespace (device->gid, false, def);

  ret = fchownat (devs_dirfd, name, uid, gid, 0); /* lgtm [cpp/toctou-race-condition] */
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "chown `%s`", device->ಮಾರ್ಗ);

  ಹಿಂತಿರುಗಿ get_bind_mount (devs_dirfd, name, false, false, false, MS_PRIVATE, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
send_mounts (ಪೂರ್ಣಾಂಕ sync_socket_host, ರಚನೆ libcrun_fd_map *fds, size_t how_many, size_t ಒಟ್ಟು, libcrun_error_t *err)
{
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (sync_socket_host, &how_many, ಗಾತ್ರ (how_many)));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "write to sync socket");

  ಗಾಗಿ (i = 0; i < ಒಟ್ಟು; i++)
    {
      ಆದರೆ (fds->fds[i] >= 0)
        {
          ret = send_fd_ಗಮ್ಯ_socket_with_payload (sync_socket_host, fds->fds[i], (ಅಕ್ಷರ *) &i, ಗಾತ್ರ (i), err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }
    }
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
prepare_and_send_mount_mounts (libcrun_container_t *container, pid_t pid, ಪೂರ್ಣಾಂಕ sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map ರಚನೆ libcrun_fd_map *mount_fds = ಖಾಲಿ;
  bool has_userns = (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_free ಅಕ್ಷರ *rootfs = ಖಾಲಿ;
  size_t how_many = 0;
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (def->mounts_len == 0)
    ಹಿಂತಿರುಗಿ 0;

  mount_fds = make_libcrun_fd_map (def->mounts_len);

  ಆದರೆ (def->ಬೇರು && def->ಬೇರು->ಮಾರ್ಗ)
    rootfs = realpath (def->ಬೇರು->ಮಾರ್ಗ, ಖಾಲಿ);

  ಆದರೆ (! has_userns)
    {
      ಪೂರ್ಣಾಂಕ is_ಒಳ_userns = check_running_ಒಳ_user_namespace (err);
      ಆದರೆ (UNLIKELY (is_ಒಳ_userns < 0))
        ಹಿಂತಿರುಗಿ is_ಒಳ_userns;

      has_userns = is_ಒಳ_userns > 0;
    }

  ಗಾಗಿ (i = 0; i < def->mounts_len; i++)
    {
      bool recursive = false;
      bool nofollow = false;
      bool has_mappings = false;
      ಪೂರ್ಣಾಂಕ mount_fd = -1;

      ret = maybe_get_idmapped_mount (container, def, def->mounts[i], pid, &mount_fd, &has_mappings, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      /* If the mount has no mappings and there is not a different user namespace, create the mount later as part of the container setup.
         Skip copy-symlink mounts: open_tree follows the symlink, losing the
         link itself; let process_single_mount handle them.  */
      ಆದರೆ (mount_fd < 0 && (has_mappings || has_userns) && is_bind_mount (def->mounts[i], &recursive, &nofollow)
          && ! mount_option_exists (def->mounts[i], "copy-symlink")
          && get_ಮಾರ್ಗ_ಒಳ_rootfs (rootfs, def->mounts[i]->ಮೂಲ) == ಖಾಲಿ)
        {
          ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪ್ರಸರಣ = 0;

          ಆದರೆ (def->linux && def->linux->rootfs_ಪ್ರಸರಣ)
            ಪ್ರಸರಣ = get_mount_flags (def->linux->rootfs_ಪ್ರಸರಣ, 0, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ) & ALL_PROPAGATIONS_NO_REC;
          ಆದರೆ (ಪ್ರಸರಣ == 0)
            ಪ್ರಸರಣ = MS_PRIVATE;

          mount_fd = get_bind_mount (-1, def->mounts[i]->ಮೂಲ, recursive, false, nofollow, ಪ್ರಸರಣ, err);
          ಆದರೆ (UNLIKELY (mount_fd < 0))
            crun_error_release (err);
        }

      ಆದರೆ (mount_fd >= 0)
        how_many++;

      mount_fds->fds[i] = mount_fd;
    }

  ಹಿಂತಿರುಗಿ send_mounts (sync_socket_host, mount_fds, how_many, def->mounts_len, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
prepare_and_send_dev_mounts (libcrun_container_t *container, ಪೂರ್ಣಾಂಕ sync_socket_host, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close_map ರಚನೆ libcrun_fd_map *dev_fds = ಖಾಲಿ;
  bool has_userns = (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  cleanup_close ಪೂರ್ಣಾಂಕ current_mountns = -1;
  cleanup_free ಅಕ್ಷರ *state_dir = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *devs_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ devs_mountfd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ targetfd = -1;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *context_type = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *label = ಖಾಲಿ;
  size_t how_many = 0;
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;
  // To track whether the namespace has been changed.
  bool ns_ಬದಲಾಯಿತು = false;

  ಆದರೆ (def->linux == ಖಾಲಿ || def->linux->devices_len == 0)
    ಹಿಂತಿರುಗಿ 0;

  dev_fds = make_libcrun_fd_map (def->linux->devices_len);

  ಆದರೆ (! has_userns || is_ಖಾಲಿಗಳು_string (container->context->id) || geteuid () > 0)
    ಹಿಂತಿರುಗಿ send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);

  ret = libcrun_get_state_directory (&state_dir,
                                     (container->context ? container->context->state_ಬೇರು : ಖಾಲಿ),
                                     (container->context ? container->context->id : ಖಾಲಿ), err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = append_paths (&devs_ಮಾರ್ಗ, err, state_dir, "devs", ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = mkdir (devs_ಮಾರ್ಗ, 0700);
  ಆದರೆ (UNLIKELY (ret < 0 && errno != EEXIST))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "mkdir `%s`", devs_ಮಾರ್ಗ);

  current_mountns = libcrun_open_proc_file (container, "self/ns/mnt", O_RDONLY, err);
  ಆದರೆ (UNLIKELY (current_mountns < 0))
    ಹಿಂತಿರುಗಿ current_mountns;

  ret = unshare (CLONE_NEWNS);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "unshare `CLONE_NEWNS`");

  // This indicates that the mount namespace has been altered.
  ns_ಬದಲಾಯಿತು = true;

  ret = mount (ಖಾಲಿ, "/", ಖಾಲಿ, MS_REC | MS_PRIVATE, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "mount `MS_REC | MS_PRIVATE`");
      ಹೋಗು restore_mountns;
    }

  ಆದರೆ (container->container_def->linux && container->container_def->linux->mount_label)
    {
      label = container->container_def->linux->mount_label;
      context_type = get_selinux_context_type (container, err);
      ಆದರೆ (UNLIKELY (context_type == ಖಾಲಿ))
        {
          ret = -1;
          ಹೋಗು restore_mountns;
        }
    }

  devs_mountfd = fsopen_mount ("tmpfs", ಖಾಲಿ, context_type, label, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (devs_mountfd < 0))
    {
      ret = send_mounts (sync_socket_host, dev_fds, 0, def->linux->devices_len, err);
      ಹೋಗು restore_mountns;
    }

  targetfd = open (devs_ಮಾರ್ಗ, O_DIRECTORY | O_ಮಾರ್ಗ | O_CLOEXEC);
  ಆದರೆ (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_ಮಾರ್ಗ);
      ಹೋಗು restore_mountns;
    }

  ret = fs_move_mount_ಗಮ್ಯ (devs_mountfd, targetfd, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, errno, "fs_move_mount_to `%s`", devs_ಮಾರ್ಗ);
      ಹೋಗು restore_mountns;
    }

  close_and_reset (&targetfd);

  targetfd = openat (devs_mountfd, ".", O_DIRECTORY | O_ಮಾರ್ಗ | O_CLOEXEC);
  ಆದರೆ (targetfd < 0)
    {
      ret = crun_make_error (err, errno, "open `%s`", devs_ಮಾರ್ಗ);
      ಹೋಗು restore_mountns;
    }

  ಗಾಗಿ (i = 0; i < def->linux->devices_len; i++)
    {
      ret = precreate_device (container, targetfd, i, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          crun_error_release (err);
          ಮುಂದುವರಿ;
        }

      dev_fds->fds[i] = ret;

      ಆದರೆ (dev_fds->fds[i] >= 0)
        how_many++;
    }

  ret = send_mounts (sync_socket_host, dev_fds, how_many, def->linux->devices_len, err);
restore_mountns:
  ಆದರೆ (ns_ಬದಲಾಯಿತು && current_mountns >= 0)
    {
      ಪೂರ್ಣಾಂಕ setns_ret;
      setns_ret = setns (current_mountns, CLONE_NEWNS);
      ಆದರೆ (UNLIKELY (setns_ret < 0 && ret >= 0))
        {
          ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "setns `CLONE_NEWNS`");
        }
    }

  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
prepare_and_send_needed_dev_mounts (libcrun_container_t *container, ಪೂರ್ಣಾಂಕ sync_socket_host, libcrun_error_t *err)
{
  cleanup_close_map ರಚನೆ libcrun_fd_map *fds = ಖಾಲಿ;
  bool has_userns = (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->unshare_flags & CLONE_NEWUSER) ? true : false;
  size_t how_many = 0;
  size_t i;

  fds = make_libcrun_fd_map (ಅಂಕೆ_NEEDED_DEVS);

  ಆದರೆ (! has_userns || geteuid () > 0)
    ಹಿಂತಿರುಗಿ send_mounts (sync_socket_host, fds, 0, ಅಂಕೆ_NEEDED_DEVS, err);

  ಗಾಗಿ (i = 0; i < ಅಂಕೆ_NEEDED_DEVS && needed_devs[i].ಮಾರ್ಗ; i++)
    {
      ಪೂರ್ಣಾಂಕ fd = syscall_open_ವೃಕ್ಷ (AT_FDCWD, needed_devs[i].ಮಾರ್ಗ,
                                  OPEN_TREE_CLONE | OPEN_TREE_CLOEXEC);
      ಆದರೆ (fd >= 0)
        {
          fds->fds[i] = fd;
          how_many++;
        }
    }

  ಹಿಂತಿರುಗಿ send_mounts (sync_socket_host, fds, how_many, ಅಂಕೆ_NEEDED_DEVS, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
prepare_and_send_mounts (libcrun_container_t *container, pid_t pid, ಪೂರ್ಣಾಂಕ sync_socket_host, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ret = expect_success_from_sync_socket (sync_socket_host, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = prepare_and_send_mount_mounts (container, pid, sync_socket_host, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = prepare_and_send_dev_mounts (container, sync_socket_host, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = prepare_and_send_needed_dev_mounts (container, sync_socket_host, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
receive_mounts (ರಚನೆ libcrun_fd_map *fds, ಪೂರ್ಣಾಂಕ sync_socket_container, libcrun_error_t *err)
{
  size_t i, how_many = 0;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (fds->nfds == 0)
    ಹಿಂತಿರುಗಿ 0;

  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (sync_socket_container, &how_many, ಗಾತ್ರ (how_many)));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "read from sync socket");

  ಗಾಗಿ (i = 0; i < how_many; i++)
    {
      size_t ಸೂಚ್ಯಂಕ;

      ret = receive_fd_from_socket_with_payload (sync_socket_container, (ಅಕ್ಷರ *) &ಸೂಚ್ಯಂಕ, ಗಾತ್ರ (ಸೂಚ್ಯಂಕ), err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
      ಆದರೆ (ಸೂಚ್ಯಂಕ >= fds->nfds)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid mount data received");

      ಆದರೆ (fds->fds[ಸೂಚ್ಯಂಕ] >= 0)
        ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (fds->fds[ಸೂಚ್ಯಂಕ]));

      fds->fds[ಸೂಚ್ಯಂಕ] = ret;
    }

  ಹಿಂತಿರುಗಿ 0;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಗಣ_init_non_dumpable (libcrun_container_t *container, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (! container->host_has_cap_sys_ptrace)
    ಹಿಂತಿರುಗಿ 0;

  ret = prctl (PR_ಗಣ_DUMPABLE, 0, 0, 0, 0);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಗಣ_id_init (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  uid_t uid = 0;
  gid_t gid = 0;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (def->process && def->process->user && def->linux)
    {
      /*
        If it is running in a user namespace and root is not mapped
        use the UID/GID specified for running the container.
      */
      bool ಬೇರು_mapped = false;

      ಆದರೆ (def->linux->uid_mappings_len != 0)
        {
          ಬೇರು_mapped = ಬೇರು_mapped_ಒಳ_container_p (def->linux->uid_mappings, def->linux->uid_mappings_len);
          ಆದರೆ (! ಬೇರು_mapped)
            uid = def->process->user->uid;

          libcrun_debug ("Using mapped UID in container: `%d`", uid);
        }

      ಆದರೆ (def->linux->gid_mappings_len != 0)
        {
          ಬೇರು_mapped = ಬೇರು_mapped_ಒಳ_container_p (def->linux->gid_mappings, def->linux->gid_mappings_len);
          ಆದರೆ (! ಬೇರು_mapped)
            gid = def->process->user->gid;

          libcrun_debug ("Using mapped GID in container: `%d`", gid);
        }
    }

  /* -1 means "leave unchanged" for setresuid and setresgid.  */
  ಆದರೆ (UNLIKELY (uid == (uid_t) -1 || gid == (gid_t) -1))
    ಹಿಂತಿರುಗಿ crun_make_error (err, EINVAL, "invalid user `%u:%u`", uid, gid);

  ret = setresuid (uid, uid, uid);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "setresuid to `%d`", uid);

  ret = setresgid (gid, gid, gid);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "setresgid to `%d`", gid);

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ bool
is_proc_mount (runtime_spec_schema_defs_mount *mnt)
{
  ಹಿಂತಿರುಗಿ mnt->type && strcmp (mnt->type, "proc") == 0 && ! is_bind_mount (mnt, ಖಾಲಿ, ಖಾಲಿ);
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
open_proc_mounts_ಒಳ_pidns (libcrun_container_t *container, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ರಚನೆ libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close ಪೂರ್ಣಾಂಕ socket_ಪೋಷಕ = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ socket_ಮಗು = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ ಬೇರು_fd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ cwd_fd = -1;
  size_t i, missing = 0;
  ಪೂರ್ಣಾಂಕ saved_errno;
  ಪೂರ್ಣಾಂಕ sockets[2];
  pid_t pid;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (def->ಬೇರು == ಖಾಲಿ || def->ಬೇರು->ಮಾರ್ಗ == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  ಗಾಗಿ (i = 0; i < def->mounts_len; i++)
    ಆದರೆ (is_proc_mount (def->mounts[i]))
      missing++;
  ಆದರೆ (missing == 0)
    ಹಿಂತಿರುಗಿ 1;

  ಬೇರು_fd = open ("/", O_DIRECTORY | O_ಮಾರ್ಗ | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (ಬೇರು_fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `/`");

  cwd_fd = open (".", O_DIRECTORY | O_ಮಾರ್ಗ | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (cwd_fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `.`");

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sockets);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "socketpair");
  socket_ಪೋಷಕ = sockets[0];
  socket_ಮಗು = sockets[1];

  ret = chroot (def->ಬೇರು->ಮಾರ್ಗ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ 0;

  pid = fork ();
  saved_errno = errno;
  ಆದರೆ (pid == 0)
    {
      /* If an existing user namespace was joined, the process has no
         privileges on the current mount namespace.  */
      ಆದರೆ (get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->unshare_flags & CLONE_NEWUSER)
        (ಶೂನ್ಯ) unshare (CLONE_NEWNS);

      ಗಾಗಿ (i = 0; i < def->mounts_len; i++)
        {
          cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;

          ಆದರೆ (! is_proc_mount (def->mounts[i]))
            ಮುಂದುವರಿ;

          fd = open_mount_ದ_type (container, def->mounts[i], err);
          ಆದರೆ (fd < 0)
            ಸುರಕ್ಷಿತ_exit (EXIT_FAILURE);

          ret = send_fd_ಗಮ್ಯ_socket_with_payload (socket_ಮಗು, fd, (ಅಕ್ಷರ *) &i, ಗಾತ್ರ (i), err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಸುರಕ್ಷಿತ_exit (EXIT_FAILURE);
        }
      ಸುರಕ್ಷಿತ_exit (EXIT_SUCCESS);
    }

  ಆದರೆ (UNLIKELY (fchdir (ಬೇರು_fd) < 0 || chroot (".") < 0 || fchdir (cwd_fd) < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "restore the root directory");

  ಆದರೆ (UNLIKELY (pid < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, saved_errno, "fork");

  close_and_reset (&socket_ಮಗು);

  /* If for any reason the mounts cannot be opened, fall back to creating
     the process now.  */
  ಆಗ (missing > 0)
    {
      libcrun_error_t tmp_err = ಖಾಲಿ;

      ret = receive_fd_from_socket_with_payload (socket_ಪೋಷಕ, (ಅಕ್ಷರ *) &i, ಗಾತ್ರ (i), &tmp_err);
      ಆದರೆ (ret < 0)
        {
          crun_error_release (&tmp_err);
          ನಿಲ್ಲಿಸು;
        }
      ಆದರೆ (i >= mount_fds->nfds)
        {
          ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (ret));
          ನಿಲ್ಲಿಸು;
        }

      ಆದರೆ (mount_fds->fds[i] >= 0)
        ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (mount_fds->fds[i]));
      mount_fds->fds[i] = ret;
      missing--;
    }

  waitpid_ignore_stopped (pid, ಖಾಲಿ, 0);

  ಹಿಂತಿರುಗಿ missing == 0 ? 1 : 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
init_container (libcrun_container_t *container, ಪೂರ್ಣಾಂಕ sync_socket_container, ರಚನೆ init_ಸ್ಥಿತಿ_s *init_ಸ್ಥಿತಿ,
                libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ರಚನೆ libcrun_fd_map *mount_fds = get_fd_map (container);
  cleanup_close ಪೂರ್ಣಾಂಕ timens_offsets_fd = -1;
  bool delayed_pidns_fork = false;
  pid_t pid_container = 0;
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  /* Do it before joining any namespace, otherwise the process would be
     visible in a joined PID namespace while it is still dumpable.  */
  ret = ಗಣ_init_non_dumpable (container, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (init_ಸ್ಥಿತಿ->idx_pidns_ಗಮ್ಯ_join_immediately >= 0 || init_ಸ್ಥಿತಿ->idx_timens_ಗಮ್ಯ_join_immediately >= 0)
    {
      pid_t new_pid;

      ಆದರೆ (init_ಸ್ಥಿತಿ->idx_pidns_ಗಮ್ಯ_join_immediately >= 0)
        {
          ret = setns (init_ಸ್ಥಿತಿ->fd[init_ಸ್ಥಿತಿ->idx_pidns_ಗಮ್ಯ_join_immediately], CLONE_NEWPID);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "setns to target PID namespace");

          close_and_reset (&init_ಸ್ಥಿತಿ->fd[init_ಸ್ಥಿತಿ->idx_pidns_ಗಮ್ಯ_join_immediately]);
        }

      ಆದರೆ (init_ಸ್ಥಿತಿ->idx_timens_ಗಮ್ಯ_join_immediately >= 0)
        {
          ret = setns (init_ಸ್ಥಿತಿ->fd[init_ಸ್ಥಿತಿ->idx_timens_ಗಮ್ಯ_join_immediately], CLONE_NEWTIME);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "setns to target timens");

          close_and_reset (&init_ಸ್ಥಿತಿ->fd[init_ಸ್ಥಿತಿ->idx_timens_ಗಮ್ಯ_join_immediately]);
        }

      ಆದರೆ (init_ಸ್ಥಿತಿ->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_ಒಳ_pidns (container, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;

          delayed_pidns_fork = ret > 0;
        }

      ಆದರೆ (delayed_pidns_fork)
        {
          /* A zero PID tells the parent that the process is created later.  */
          new_pid = 0;

          ret = send_success_ಗಮ್ಯ_sync_socket (sync_socket_container, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;

          ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (sync_socket_container, &new_pid, ಗಾತ್ರ (new_pid)));
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "write to sync socket");
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          new_pid = fork ();
          ಆದರೆ (UNLIKELY (new_pid < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fork");
        }

      ಆದರೆ (new_pid)
        {
          /* Report the new PID to the parent and exit immediately.  */
          ret = send_success_ಗಮ್ಯ_sync_socket (sync_socket_container, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (sync_socket_container, &new_pid, ಗಾತ್ರ (new_pid)));
          ಆದರೆ (UNLIKELY (ret < 0))
            kill (new_pid, SIGKILL);

          ಸುರಕ್ಷಿತ_exit (0);
        }

      /* Wait for the parent to receive the new PID.  */
      ret = expect_success_from_sync_socket (sync_socket_container, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ret = libcrun_ಗಣ_oom (container, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (init_ಸ್ಥಿತಿ->fd_len > 0)
    {
      ret = join_namespaces (def, init_ಸ್ಥಿತಿ->fd, init_ಸ್ಥಿತಿ->fd_len, init_ಸ್ಥಿತಿ->ಸೂಚ್ಯಂಕ, true, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  /* If the container needs to join an existing PID namespace, take a reference to it
     before creating a new user namespace, as we could lose the access to the existing
     namespace.

     This cannot be done in the parent process (by `prepare_and_send_mounts`), since it is
     necessary to first join the target namespaces and then create the mount.
  */
  ಆದರೆ ((init_ಸ್ಥಿತಿ->all_namespaces & CLONE_NEWUSER) && (init_ಸ್ಥಿತಿ->join_pidns || init_ಸ್ಥಿತಿ->join_ipcns))
    {
      ಗಾಗಿ (i = 0; i < def->mounts_len; i++)
        {
          ಪೂರ್ಣಾಂಕ fd = -1;

          /* The mount type is optional in the OCI configuration, e.g. bind mounts
             are usually specified without it.  */
          ಆದರೆ (def->mounts[i]->type == ಖಾಲಿ)
            ಮುಂದುವರಿ;

          /* If for any reason the mount cannot be opened, ignore errors and continue.
             An error will be generated later if it is not possible to join the namespace.
          */
          ಆದರೆ (init_ಸ್ಥಿತಿ->join_pidns && ! delayed_pidns_fork && strcmp (def->mounts[i]->type, "proc") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->ಮೂಲ, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ);
          ಆದರೆ (init_ಸ್ಥಿತಿ->join_ipcns && strcmp (def->mounts[i]->type, "mqueue") == 0)
            fd = fsopen_mount (def->mounts[i]->type, def->mounts[i]->ಮೂಲ, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ);

          ಆದರೆ (fd >= 0)
            {
              ಆದರೆ (mount_fds->fds[i] >= 0)
                ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (mount_fds->fds[i]));
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
  ಆದರೆ (def->linux->ಸಮಯ_offsets)
    {
      timens_offsets_fd = libcrun_open_proc_file (container, "self/timens_offsets", O_WRONLY, err);
      ಆದರೆ (UNLIKELY (timens_offsets_fd < 0))
        ಹಿಂತಿರುಗಿ timens_offsets_fd;
    }

  ಆದರೆ (init_ಸ್ಥಿತಿ->all_namespaces & CLONE_NEWUSER)
    {
      ಆದರೆ (init_ಸ್ಥಿತಿ->delayed_userns_create)
        {
          libcrun_debug ("Unsharing user namespace");
          ret = unshare (CLONE_NEWUSER);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "unshare user namespace");

          init_ಸ್ಥಿತಿ->namespaces_ಗಮ್ಯ_unshare &= ~CLONE_NEWUSER;

          ret = send_success_ಗಮ್ಯ_sync_socket (sync_socket_container, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }

      ಆದರೆ (init_ಸ್ಥಿತಿ->userns_ಸೂಚ್ಯಂಕ < 0)
        {
          ret = expect_success_from_sync_socket (sync_socket_container, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* If we need to join another user namespace, do it immediately before creating any other namespace. */
          libcrun_debug ("Joining existing user namespace");
          ret = setns (init_ಸ್ಥಿತಿ->fd[init_ಸ್ಥಿತಿ->userns_ಸೂಚ್ಯಂಕ], CLONE_NEWUSER);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "setns to target user namespace `%s`",
                                    def->linux->namespaces[init_ಸ್ಥಿತಿ->userns_ಸೂಚ್ಯಂಕ_origin]->ಮಾರ್ಗ);
        }

      ret = ಗಣ_id_init (container, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      /* Changing the credentials resets the dumpable flag to the value of
         fs.suid_dumpable, so set it again.  */
      ret = ಗಣ_init_non_dumpable (container, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ret = join_namespaces (def, init_ಸ್ಥಿತಿ->fd, init_ಸ್ಥಿತಿ->fd_len, init_ಸ್ಥಿತಿ->ಸೂಚ್ಯಂಕ, false, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (init_ಸ್ಥಿತಿ->namespaces_ಗಮ್ಯ_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS))
    {
      /* New namespaces to create for the container.  */
      ret = unshare (init_ಸ್ಥಿತಿ->namespaces_ಗಮ್ಯ_unshare & ~(CLONE_NEWCGROUP | CLONE_NEWNS));
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "unshare");
    }

  ಆದರೆ (def->linux->ಸಮಯ_offsets)
    {
      ಅಕ್ಷರ fmt_ಬಫರ್[128];
      ಪೂರ್ಣಾಂಕ fd = timens_offsets_fd;

      ಆದರೆ (def->linux->ಸಮಯ_offsets->boottime)
        {
          ret = snprintf (fmt_ಬಫರ್, ಗಾತ್ರ (fmt_ಬಫರ್), "boottime %" PRIi64 " %" PRIu32, def->linux->ಸಮಯ_offsets->boottime->secs, def->linux->ಸಮಯ_offsets->boottime->nanosecs);
          ಆದರೆ (UNLIKELY (ret >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_ಬಫರ್)))
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

          libcrun_debug ("Using boot time offset: secs = `%lld`, nanosecs = `%d`", (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ) def->linux->ಸಮಯ_offsets->boottime->secs, def->linux->ಸಮಯ_offsets->boottime->nanosecs);
          ret = ಲಿಖ (fd, fmt_ಬಫರ್, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (fmt_ಬಫರ್));
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
      ಆದರೆ (def->linux->ಸಮಯ_offsets->monotonic)
        {
          libcrun_debug ("Using monotonic time offset: secs = `%lld`, nanosecs = `%d`", (ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ) def->linux->ಸಮಯ_offsets->monotonic->secs, def->linux->ಸಮಯ_offsets->monotonic->nanosecs);

          ret = snprintf (fmt_ಬಫರ್, ಗಾತ್ರ (fmt_ಬಫರ್), "monotonic %" PRIi64 " %" PRIu32, def->linux->ಸಮಯ_offsets->monotonic->secs, def->linux->ಸಮಯ_offsets->monotonic->nanosecs);
          ಆದರೆ (UNLIKELY (ret >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (fmt_ಬಫರ್)))
            ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

          ret = ಲಿಖ (fd, fmt_ಬಫರ್, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (fmt_ಬಫರ್));
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "write `/proc/self/timens_offsets`");
        }
    }

  ಆದರೆ (init_ಸ್ಥಿತಿ->must_fork)
    {
      ಆದರೆ (init_ಸ್ಥಿತಿ->can_delay_pidns_fork)
        {
          ret = open_proc_mounts_ಒಳ_pidns (container, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;

          delayed_pidns_fork = ret > 0;
        }

      ಆದರೆ (! delayed_pidns_fork)
        {
          /* A PID and a time namespace are joined when the new process is created.  */
          pid_container = fork ();
          ಆದರೆ (UNLIKELY (pid_container < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "cannot fork");
        }

      /* Report back the new PID.  A zero PID tells the parent that the process is created later.  */
      ಆದರೆ (pid_container || delayed_pidns_fork)
        {
          libcrun_debug ("Running container PID after fork: `%d`", pid_container);
          ret = send_success_ಗಮ್ಯ_sync_socket (sync_socket_container, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;

          ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (sync_socket_container, &pid_container, ಗಾತ್ರ (pid_container)));
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "write to sync socket");

          ಆದರೆ (pid_container)
            ಸುರಕ್ಷಿತ_exit (EXIT_SUCCESS);
        }

      ret = expect_success_from_sync_socket (sync_socket_container, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->delayed_pidns_fork = delayed_pidns_fork;

  ret = send_success_ಗಮ್ಯ_sync_socket (sync_socket_container, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  /* Receive the mounts sent by `prepare_and_send_mounts`.  */
  ret = receive_mounts (get_fd_map (container), sync_socket_container, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = receive_mounts (get_devices_fd_map (container), sync_socket_container, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = receive_mounts (get_needed_devs_fd_map (container), sync_socket_container, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_container_setgroups (container, container->container_def->process, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ 0;
}

bool
libcrun_container_delayed_pidns_fork (libcrun_container_t *container)
{
  ಹಿಂತಿರುಗಿ get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->delayed_pidns_fork;
}

ಪೂರ್ಣಾಂಕ
libcrun_handle_pidfd_receiver (pid_t pid, libcrun_container_t *container, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ client_fd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ pidfd = -1;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *v;

  v = ಕಂಡುಹಿಡಿ_annotation (container, "run.oci.pidfd_receiver");
  ಆದರೆ (v == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  pidfd = syscall_pidfd_open (pid, 0);
  ಆದರೆ (UNLIKELY (pidfd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "pidfd_open");

  client_fd = open_unix_domain_client_socket (v, 0, err);
  ಆದರೆ (UNLIKELY (client_fd < 0))
    ಹಿಂತಿರುಗಿ client_fd;

  ಹಿಂತಿರುಗಿ send_fd_ಗಮ್ಯ_socket (client_fd, pidfd, err);
}

ಸ್ಥಿರ bool
has_exec_cpu_affinity (runtime_spec_schema_config_schema_process *process)
{
  ಆದರೆ (process == ಖಾಲಿ || process->exec_cpu_affinity == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ false;
  ಹಿಂತಿರುಗಿ (! is_ಖಾಲಿಗಳು_string (process->exec_cpu_affinity->initial))
         || (! is_ಖಾಲಿಗಳು_string (process->exec_cpu_affinity->final));
}

pid_t
libcrun_ಸತತ_linux_container (libcrun_container_t *container, container_entrypoint_t entrypoint, ಶೂನ್ಯ *ವಾದಗಳು,
                             ಪೂರ್ಣಾಂಕ *sync_socket_ಹೊರಹೋಗು, ರಚನೆ libcrun_dirfd_s *cgroup_dirfd, libcrun_error_t *err)
{
  __attribute__ ((cleanup (cleanup_free_init_statusp))) ರಚನೆ init_ಸ್ಥಿತಿ_s init_ಸ್ಥಿತಿ;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ಪೂರ್ಣಾಂಕ sync_socket_container = -1;
  ಅಕ್ಷರ *notify_socket_env = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ sync_socket_host = -1;
  __attribute__ ((ಬಳಸದ)) cleanup_close ಪೂರ್ಣಾಂಕ restore_pidns = -1;
  ಪೂರ್ಣಾಂಕ ಮೊದಲ_clone_ವಾದಗಳು = 0;
  ಪೂರ್ಣಾಂಕ sync_socket[2];
  pid_t pid;
  bool new_userns;
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  libcrun_debug ("Running linux container");
  ret = configure_init_ಸ್ಥಿತಿ (&init_ಸ್ಥಿತಿ, container, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->unshare_flags = init_ಸ್ಥಿತಿ.all_namespaces;
  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->joined_mount_ns
      = (init_ಸ್ಥಿತಿ.all_namespaces & CLONE_NEWNS)
        && ! (init_ಸ್ಥಿತಿ.namespaces_ಗಮ್ಯ_unshare & CLONE_NEWNS);
  /* cgroup will be unshared later.  Once the process is in the correct cgroup.  */
  init_ಸ್ಥಿತಿ.all_namespaces &= ~CLONE_NEWCGROUP;
  get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->unshare_cgroupns = init_ಸ್ಥಿತಿ.namespaces_ಗಮ್ಯ_unshare & CLONE_NEWCGROUP;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "socketpair");

  sync_socket_host = sync_socket[0];
  sync_socket_container = sync_socket[1];

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYSTEMD
  ಆದರೆ (def->ಬೇರು)
    {
      ret = do_notify_socket (container, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }
#ಆದರೆ_ಅಂತ್ಯ

  get_uid_gid_from_def (container->container_def, &container->container_uid, &container->container_gid);

  /* This must be done before we enter a user namespace.  */
  ಆದರೆ (def->process)
    {
      ret = libcrun_ಗಣ_rlimits (def->process->rlimits, def->process->rlimits_len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ret = libcrun_ಗಣ_oom (container, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_container_notify_handler (ವಾದಗಳು, HANDLER_CONFIGURE_BEFORE_USERNS, container,
                                          container->container_def->ಬೇರು ? container->container_def->ಬೇರು->ಮಾರ್ಗ : ಖಾಲಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  /* If a new user namespace must be created, but there are other namespaces to join, then delay
     the userns creation after the namespaces are joined.  */
  init_ಸ್ಥಿತಿ.delayed_userns_create
      = (init_ಸ್ಥಿತಿ.all_namespaces & CLONE_NEWUSER) && init_ಸ್ಥಿತಿ.userns_ಸೂಚ್ಯಂಕ < 0 && init_ಸ್ಥಿತಿ.fd_len > 0;

  /* Check if special handling is required to join the namespaces.  The PID and time
     namespaces are to be joined before creating a new user namespace (as the process
     loses the privileges to do so after that), but after joining an existing one (as
     the process might not have the privileges to do so before that).  */
  new_userns = (init_ಸ್ಥಿತಿ.all_namespaces & CLONE_NEWUSER) && init_ಸ್ಥಿತಿ.userns_ಸೂಚ್ಯಂಕ < 0;
  ಗಾಗಿ (i = 0; i < init_ಸ್ಥಿತಿ.fd_len; i++)
    {
      ಆಯ್ಕೆ (init_ಸ್ಥಿತಿ.ಮೌಲ್ಯ[i])
        {
        ಸ್ಥಿತಿ CLONE_NEWIPC:
          ಆದರೆ (init_ಸ್ಥಿತಿ.all_namespaces & CLONE_NEWUSER)
            init_ಸ್ಥಿತಿ.join_ipcns = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ CLONE_NEWPID:
          /* The prestart, createRuntime and createContainer hooks need the PID of the
             process in the PID namespace before the pivot_root.  */
          init_ಸ್ಥಿತಿ.can_delay_pidns_fork = ! (def->hooks
                                                && (def->hooks->prestart_len || def->hooks->create_runtime_len
                                                    || def->hooks->create_container_len));
          ಆದರೆ (! new_userns)
            init_ಸ್ಥಿತಿ.must_fork = true;
          ಇಲ್ಲದಿದ್ದರೆ
            {
              init_ಸ್ಥಿತಿ.join_pidns = true;
              init_ಸ್ಥಿತಿ.idx_pidns_ಗಮ್ಯ_join_immediately = i;
              init_ಸ್ಥಿತಿ.namespaces_ಗಮ್ಯ_unshare &= ~CLONE_NEWPID;
            }
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ CLONE_NEWTIME:
          ಆದರೆ (! new_userns)
            init_ಸ್ಥಿತಿ.must_fork = true;
          ಇಲ್ಲದಿದ್ದರೆ
            {
              init_ಸ್ಥಿತಿ.idx_timens_ಗಮ್ಯ_join_immediately = i;
              init_ಸ್ಥಿತಿ.namespaces_ಗಮ್ಯ_unshare &= ~CLONE_NEWTIME;
            }
          ನಿಲ್ಲಿಸು;
        }
    }

  /* Before attempting any setns() or unshare(), a clone() is required to not touch the caller context
     that can be used later on for running hooks.  */

  ಆದರೆ ((init_ಸ್ಥಿತಿ.namespaces_ಗಮ್ಯ_unshare & CLONE_NEWUSER) && init_ಸ್ಥಿತಿ.fd_len == 0)
    {
      /* If a user namespace must be created and there are no other namespaces to join, create the userns alone.  */
      ಮೊದಲ_clone_ವಾದಗಳು = CLONE_NEWUSER;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((init_ಸ್ಥಿತಿ.all_namespaces & CLONE_NEWUSER) == 0)
    {
      /* If it doesn't create a user namespace or need to join one, create the new requested namespaces now. */
      ಮೊದಲ_clone_ವಾದಗಳು = init_ಸ್ಥಿತಿ.namespaces_ಗಮ್ಯ_unshare & ~(CLONE_NEWTIME | CLONE_NEWCGROUP | CLONE_NEWNS);
    }

  init_ಸ್ಥಿತಿ.namespaces_ಗಮ್ಯ_unshare &= ~ಮೊದಲ_clone_ವಾದಗಳು;

  /* Check if there are still namespaces that require a fork().  */
  ಆದರೆ (init_ಸ್ಥಿತಿ.namespaces_ಗಮ್ಯ_unshare & (CLONE_NEWPID | CLONE_NEWTIME))
    {
      /* If we need to make another fork(), make sure the NEWPID is always
         created as part of that.  */
      ಮೊದಲ_clone_ವಾದಗಳು &= ~CLONE_NEWPID;
      init_ಸ್ಥಿತಿ.namespaces_ಗಮ್ಯ_unshare |= CLONE_NEWPID;
      init_ಸ್ಥಿತಿ.must_fork = true;
      init_ಸ್ಥಿತಿ.can_delay_pidns_fork = false;
    }

  pid = -1;
  ಆದರೆ (cgroup_dirfd && *cgroup_dirfd->dirfd >= 0)
    {
      ರಚನೆ clone3_ವಾದಗಳು clone3_ವಾದಗಳು_2;
      memset (&clone3_ವಾದಗಳು_2, 0, ಗಾತ್ರ (clone3_ವಾದಗಳು_2));
      clone3_ವಾದಗಳು_2.exit_signal = SIGCHLD;
      clone3_ವಾದಗಳು_2.flags = ಮೊದಲ_clone_ವಾದಗಳು;

      clone3_ವಾದಗಳು_2.flags |= CLONE_INTO_CGROUP;
      clone3_ವಾದಗಳು_2.cgroup = *cgroup_dirfd->dirfd;

      pid = syscall_clone3 (&clone3_ವಾದಗಳು_2);
      ಆದರೆ (pid >= 0)
        cgroup_dirfd->joined = true;

      close_and_reset (cgroup_dirfd->dirfd);
    }
  /* fallback to clone() for any error.  */
  ಆದರೆ (pid < 0)
    {
      pid = syscall_clone (ಮೊದಲ_clone_ವಾದಗಳು | SIGCHLD, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (pid < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "clone");
    }

  ಆದರೆ (pid)
    {
      __attribute__ ((ಬಳಸದ)) cleanup_pid pid_t pid_ಗಮ್ಯ_clean = pid;

      /* this is safe to do because the std stream files were not changed since the clone().  */
      ret = libcrun_save_external_descriptors (container, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = close_and_reset (&sync_socket_container);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "close");

      /* any systemd notify socket open_tree FD is pointless to keep around in the parent */
      close_and_reset (&(get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->notify_socket_ವೃಕ್ಷ_fd));

      ಆದರೆ (init_ಸ್ಥಿತಿ.idx_pidns_ಗಮ್ಯ_join_immediately >= 0 || init_ಸ್ಥಿತಿ.idx_timens_ಗಮ್ಯ_join_immediately >= 0)
        {
          pid_t new_pid = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;

          ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (sync_socket_host, &new_pid, ಗಾತ್ರ (new_pid)));
          ಆದರೆ (UNLIKELY (ret != ಗಾತ್ರ (new_pid)))
            ಹಿಂತಿರುಗಿ crun_make_error (err, ret < 0 ? errno : 0, "read pid from sync socket");

          ಆದರೆ (new_pid == 0)
            get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->delayed_pidns_fork = true;
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* Cleanup the first process.  */
              ret = waitpid_ignore_stopped (pid, ಖಾಲಿ, 0);

              pid_ಗಮ್ಯ_clean = pid = new_pid;
            }

          ret = send_success_ಗಮ್ಯ_sync_socket (sync_socket_host, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }

      ಆದರೆ (init_ಸ್ಥಿತಿ.delayed_userns_create)
        {
          ret = expect_success_from_sync_socket (sync_socket_host, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }

      ಆದರೆ ((init_ಸ್ಥಿತಿ.all_namespaces & CLONE_NEWUSER) && init_ಸ್ಥಿತಿ.userns_ಸೂಚ್ಯಂಕ < 0)
        {
          ret = libcrun_ಗಣ_usernamespace (container, pid, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;

          ret = send_success_ಗಮ್ಯ_sync_socket (sync_socket_host, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }

      ಆದರೆ (init_ಸ್ಥಿತಿ.must_fork)
        {
          pid_t grandchild = 0;

          ret = expect_success_from_sync_socket (sync_socket_host, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;

          ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (sync_socket_host, &grandchild, ಗಾತ್ರ (grandchild)));
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "read pid from sync socket");

          ret = send_success_ಗಮ್ಯ_sync_socket (sync_socket_host, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;

          ಆದರೆ (grandchild == 0)
            get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->delayed_pidns_fork = true;
          ಇಲ್ಲದಿದ್ದರೆ
            {
              /* Cleanup the first process.  */
              waitpid_ignore_stopped (pid, ಖಾಲಿ, 0);

              pid_ಗಮ್ಯ_clean = pid = grandchild;
            }
        }

      /* They are received by `receive_mounts`.  */
      ret = prepare_and_send_mounts (container, pid, sync_socket_host, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = expect_success_from_sync_socket (sync_socket_host, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      *sync_socket_ಹೊರಹೋಗು = get_and_reset (&sync_socket_host);

      /* If the fork is delayed, it is done once the container process exists.  */
      ಆದರೆ (! get_ಖಾಸಗಿ_ದತ್ತಾಂಶ (container)->delayed_pidns_fork)
        {
          ret = libcrun_handle_pidfd_receiver (pid, container, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }

      pid_ಗಮ್ಯ_clean = 0;
      ಹಿಂತಿರುಗಿ pid;
    }

  /* Inside the container process.  */

  ret = close_and_reset (&sync_socket_host);
  ಆದರೆ (UNLIKELY (ret < 0))
    libcrun_fail_with_error (errno, "%s", "close sync socket");

  /* Initialize the new process and make sure to join/create all the required namespaces.  */
  ret = init_container (container, sync_socket_container, &init_ಸ್ಥಿತಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    send_error_ಗಮ್ಯ_sync_socket_and_die (sync_socket_container, false, err);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ret = send_success_ಗಮ್ಯ_sync_socket (sync_socket_container, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        libcrun_fail_with_error (crun_error_get_errno (err), "%s", (*err)->ಸಂದೇಶ);
    }

  /* Jump into the specified entrypoint.  */
  ಆದರೆ (container->context->notify_socket)
    xasprintf (&notify_socket_env, "NOTIFY_SOCKET=%s/notify", container->context->notify_socket);

  ret = entrypoint (ವಾದಗಳು, notify_socket_env, sync_socket_container, err);

  /* For most of the cases ENTRYPOINT returns only on an error, fallback here */
  /* Except for custom handlers which could perform a task and return with success */
  /* since custom handlers could or could not be {long-running, blocking} */
  ಆದರೆ (*err)
    libcrun_fail_with_error ((*err)->ಸ್ಥಿತಿ_2, "%s", (*err)->ಸಂದೇಶ);

  /* If we got here likely we returned from a custom handler eg. wasm, libkrun */
  /* Allow cleanup attributes to perform cleanup and exit with success if return code was 0 */
  ಆದರೆ (ret == 0)
    ಸುರಕ್ಷಿತ_exit (EXIT_SUCCESS);

  ಸುರಕ್ಷಿತ_exit (EXIT_FAILURE);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
join_process_ಪೋಷಕ_helper (libcrun_context_t *context,
                            libcrun_container_t *container,
                            runtime_spec_schema_config_schema_process *process,
                            pid_t ಮಗು_pid, ಪೂರ್ಣಾಂಕ sync_socket_fd,
                            libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2,
                            bool need_move_ಗಮ್ಯ_cgroup, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *sub_cgroup,
                            ಪೂರ್ಣಾಂಕ *terminal_fd, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  ಅಕ್ಷರ res;
  pid_t pid;
  cleanup_close ಪೂರ್ಣಾಂಕ sync_fd = sync_socket_fd;

  ಆದರೆ (terminal_fd)
    *terminal_fd = -1;

  /* Read the status and the PID from the child process.  */
  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (sync_fd, &res, 1));
  ಆದರೆ (UNLIKELY (ret != 1))
    ಹಿಂತಿರುಗಿ crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  ಆದರೆ (res != '0')
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "fail startup");

  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (sync_fd, &pid, ಗಾತ್ರ (pid)));
  ಆದರೆ (UNLIKELY (ret != ಗಾತ್ರ (pid)))
    ಹಿಂತಿರುಗಿ crun_make_error (err, ret < 0 ? errno : 0, "read from sync socket");

  /* Wait for the child pid so we ensure the grandchild gets properly reparented.  */
  ret = waitpid_ignore_stopped (ಮಗು_pid, ಖಾಲಿ, 0);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "waitpid for exec child pid");

  ಆದರೆ (process && process->exec_cpu_affinity)
    {
      ret = libcrun_ಗಣ_cpu_affinity_from_string (pid, process->exec_cpu_affinity->initial, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (! has_exec_cpu_affinity (process))
    {
      ret = libcrun_reset_cpu_affinity_ಮುಖವಾಡ (pid, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (need_move_ಗಮ್ಯ_cgroup)
    {
      cleanup_cgroup_ಸ್ಥಿತಿ ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ = libcrun_cgroup_make_ಸ್ಥಿತಿ (ಸ್ಥಿತಿ_2);
      cleanup_free ಅಕ್ಷರ *final_cgroup = ಖಾಲಿ;

      ಆದರೆ (sub_cgroup)
        {
          ret = append_paths (&final_cgroup, err, ಸ್ಥಿತಿ_2->cgroup_ಮಾರ್ಗ, sub_cgroup, ಖಾಲಿ);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹಿಂತಿರುಗಿ ret;
        }

      ret = libcrun_cgroup_join_process (cgroup_ಸ್ಥಿತಿ, final_cgroup ?: ಸ್ಥಿತಿ_2->cgroup_ಮಾರ್ಗ, pid, ಸ್ಥಿತಿ_2->pid, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  /* Set the scheduler for the process itself (it might have been set for its
     parent, but it is not inherited with SCHED_FLAG_RESET_ON_FORK, and
     SCHED_DEADLINE cannot be set there as it prevents forking).  */
  ret = libcrun_ಗಣ_scheduler (pid, process, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (process && process->exec_cpu_affinity)
    {
      ret = libcrun_ಗಣ_cpu_affinity_from_string (pid, process->exec_cpu_affinity->final, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ret = libcrun_apply_intelrdt (context->id, container, pid, LIBCRUN_INTELRDT_MOVE, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = libcrun_ಗಣ_io_ಆದ್ಯತೆ (pid, process, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  /* The write unblocks the grandchild process so it can run once we setup
     the cgroups.  */
  ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (sync_fd, &ret, ಗಾತ್ರ (ret)));
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "write to sync socket");

  ಆದರೆ (terminal_fd)
    {
      ret = receive_fd_from_socket (sync_fd, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          ಪೂರ್ಣಾಂಕ err_ಸಂಕೇತ = 0;
          cleanup_free ಅಕ್ಷರ *err_str = ಖಾಲಿ;

          ಆದರೆ (read_error_from_sync_socket (sync_fd, &err_ಸಂಕೇತ, &err_str))
            {
              ಆದರೆ (! is_ಖಾಲಿಗಳು_string (err_str))
                {
                  crun_error_release (err);
                  ಹಿಂತಿರುಗಿ crun_make_error (err, err_ಸಂಕೇತ, "%s", err_str);
                }
            }

          ಹಿಂತಿರುಗಿ crun_error_wrap (err, "receive terminal fd");
        }
      *terminal_fd = ret;
    }

  ಹಿಂತಿರುಗಿ pid;
}

/*
  try to join all the namespaces with a single call to setns using the target process pidfd.

  return codes:
  < 0 - on errors
  0   - the namespaces were not joined.
  > 0 - the namespaces were joined.
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
try_setns_with_pidfd (pid_t pid_ಗಮ್ಯ_join, libcrun_container_t *container, libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ಪೂರ್ಣಾಂಕ pidfd_pid_ಗಮ್ಯ_join = -1;
  ಪೂರ್ಣಾಂಕ all_flags = 0;
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  /* If there is any explicit namespace path to join, skip the setns_with_pidfd
     shortcut and join each namespace individually.  */
  ಆದರೆ (def->linux && def->linux->namespaces)
    {
      ಗಾಗಿ (i = 0; i < def->linux->namespaces_len; i++)
        ಆದರೆ (! is_ಖಾಲಿಗಳು_string (def->linux->namespaces[i]->ಮಾರ್ಗ))
          ಹಿಂತಿರುಗಿ 0;
    }

  pidfd_pid_ಗಮ್ಯ_join = syscall_pidfd_open (pid_ಗಮ್ಯ_join, 0);
  ಆದರೆ (UNLIKELY (pidfd_pid_ಗಮ್ಯ_join < 0))
    ಹಿಂತಿರುಗಿ 0;

  /* Validate that the pidfd really refers to the original container process.  */
  ret = libcrun_check_pid_valid (ಸ್ಥಿತಿ_2, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;
  ಆದರೆ (ret == 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, ESRCH, "container process not found, the pid was reused");

  ಗಾಗಿ (i = 0; namespaces[i].ns_file; i++)
    all_flags |= namespaces[i].ಮೌಲ್ಯ;

  ret = setns (pidfd_pid_ಗಮ್ಯ_join, all_flags);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ 0;

  ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
join_process_namespaces (libcrun_container_t *container, pid_t pid_ಗಮ್ಯ_join, libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ಪೂರ್ಣಾಂಕ fds_joined[MAX_NAMESPACES] = {
    0,
  };
  ಪೂರ್ಣಾಂಕ fds[MAX_NAMESPACES] = {
    -1,
  };
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  /* Try to join all namespaces in one shot with setns and pidfd.  */
  ret = try_setns_with_pidfd (pid_ಗಮ್ಯ_join, container, ಸ್ಥಿತಿ_2, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;
  /* Nothing left to do if the namespaces were joined.  */
  ಆದರೆ (LIKELY (ret > 0))
    ಹಿಂತಿರುಗಿ 0;

  /* If setns fails with the target pidfd, fall-back to join each namespace individually.  */

  ಆದರೆ (def->linux->namespaces_len >= MAX_NAMESPACES)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid configuration");

  ಗಾಗಿ (i = 0; namespaces[i].ns_file; i++)
    {
      cleanup_free ಅಕ್ಷರ *ns_ಮಾರ್ಗ = ಖಾಲಿ;

      xasprintf (&ns_ಮಾರ್ಗ, "%d/ns/%s", pid_ಗಮ್ಯ_join, namespaces[i].ns_file);
      fds[i] = libcrun_open_proc_file (container, ns_ಮಾರ್ಗ, O_RDONLY, err);
      ಆದರೆ (UNLIKELY (fds[i] < 0))
        {
          /* If the namespace doesn't exist, just ignore it.  */
          ಆದರೆ (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              ಮುಂದುವರಿ;
            }

          ret = fds[i];
          ಹೋಗು ನಿರ್ಗಮಿಸು;
        }
    }

  /* The namespace files were opened by PID.  Validate that the PID still refers
     to the original container process before joining any of them.  */
  ret = libcrun_check_pid_valid (ಸ್ಥಿತಿ_2, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹೋಗು ನಿರ್ಗಮಿಸು;
  ಆದರೆ (ret == 0)
    {
      ret = crun_make_error (err, ESRCH, "container process not found, the pid was reused");
      ಹೋಗು ನಿರ್ಗಮಿಸು;
    }

  ಗಾಗಿ (i = 0; namespaces[i].ns_file; i++)
    {
      ಆದರೆ (namespaces[i].ಮೌಲ್ಯ == CLONE_NEWUSER)
        ಮುಂದುವರಿ;

      ret = setns (fds[i], 0);
      ಆದರೆ (ret == 0)
        fds_joined[i] = 1;
    }
  ಗಾಗಿ (i = 0; namespaces[i].ns_file; i++)
    {
      ಆದರೆ (fds_joined[i])
        ಮುಂದುವರಿ;
      ret = setns (fds[i], 0);
      ಆದರೆ (ret == 0)
        fds_joined[i] = 1;
    }
  ಗಾಗಿ (i = 0; namespaces[i].ns_file; i++)
    {
      ಆದರೆ (fds_joined[i])
        ಮುಂದುವರಿ;
      ret = setns (fds[i], 0);
      ಆದರೆ (UNLIKELY (ret < 0 && errno != EINVAL))
        {
          size_t j;
          bool ಸಿಕ್ಕಿತು = false;

          ಗಾಗಿ (j = 0; j < def->linux->namespaces_len; j++)
            {
              ಆದರೆ (strcmp (namespaces[i].name, def->linux->namespaces[j]->type) == 0)
                {
                  ಸಿಕ್ಕಿತು = true;
                  ನಿಲ್ಲಿಸು;
                }
            }
          ಆದರೆ (! ಸಿಕ್ಕಿತು)
            {
              /* It was not requested to create this ns, so just ignore it.  */
              fds_joined[i] = 1;
              ಮುಂದುವರಿ;
            }

          ret = crun_make_error (err, errno, "setns `%s`", namespaces[i].ns_file);
          ಹೋಗು ನಿರ್ಗಮಿಸು;
        }
      fds_joined[i] = 1;
    }

  ret = 0;

ನಿರ್ಗಮಿಸು:
  ಗಾಗಿ (i = 0; namespaces[i].ns_file; i++)
    close_and_reset (&fds[i]);

  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
libcrun_join_process (libcrun_context_t *context,
                      libcrun_container_t *container,
                      pid_t pid_ಗಮ್ಯ_join,
                      libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2,
                      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *sub_cgroup,
                      ಪೂರ್ಣಾಂಕ detach,
                      runtime_spec_schema_config_schema_process *process,
                      ಪೂರ್ಣಾಂಕ *terminal_fd,
                      libcrun_error_t *err)
{
  pid_t pid;
  ಪೂರ್ಣಾಂಕ ret;
  ಪೂರ್ಣಾಂಕ sync_socket_fd[2];
  cleanup_close ಪೂರ್ಣಾಂಕ cgroup_dirfd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ sync_fd = -1;
  ರಚನೆ clone3_ವಾದಗಳು clone3_ವಾದಗಳು_2;
  bool need_move_ಗಮ್ಯ_cgroup;

  ಆದರೆ (! detach)
    {
      ret = prctl (PR_ಗಣ_ಮಗು_SUBREAPER, 1, 0, 0, 0);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "set child subreaper");
    }

  ಆದರೆ (ಮಾರ್ಗ_has_dot_dot_component (sub_cgroup))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid cgroup path `%s`: `..` components are not allowed", sub_cgroup);

  /* The caller checked only the container cgroup, but a sub-cgroup is managed
     by the container and the process would never run if it is frozen.  */
  ಆದರೆ (sub_cgroup && ! is_ಖಾಲಿಗಳು_string (ಸ್ಥಿತಿ_2->cgroup_ಮಾರ್ಗ))
    {
      cleanup_free ಅಕ್ಷರ *final_cgroup = ಖಾಲಿ;

      ret = append_paths (&final_cgroup, err, ಸ್ಥಿತಿ_2->cgroup_ಮಾರ್ಗ, sub_cgroup, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = libcrun_cgroup_ensure_not_frozen (final_cgroup, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "error creating socketpair");

  {
    cleanup_cgroup_ಸ್ಥಿತಿ ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ = ಖಾಲಿ;

    cgroup_ಸ್ಥಿತಿ = libcrun_cgroup_make_ಸ್ಥಿತಿ (ಸ್ಥಿತಿ_2);

    /* The cgroup can be joined directly only when there are no additional
       controllers not handled by cgroup v2.  */
    ಆದರೆ (get_force_cgroup_v1_annotation (container) == ಖಾಲಿ)
      {
        cgroup_dirfd = libcrun_get_cgroup_dirfd (cgroup_ಸ್ಥಿತಿ, sub_cgroup, err);
        ಆದರೆ (UNLIKELY (cgroup_dirfd < 0))
          crun_error_release (err);
      }
  }

  memset (&clone3_ವಾದಗಳು_2, 0, ಗಾತ್ರ (clone3_ವಾದಗಳು_2));
  clone3_ವಾದಗಳು_2.exit_signal = SIGCHLD;

  /* Do not join the cgroup immediately if an initial CPU affinity mask is specified, so that
     the process can set the cpu affinity before joining the target cgroup.  */
  ಆದರೆ (cgroup_dirfd < 0 || (process->exec_cpu_affinity && process->exec_cpu_affinity->initial))
    need_move_ಗಮ್ಯ_cgroup = true;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      need_move_ಗಮ್ಯ_cgroup = false;
      clone3_ವಾದಗಳು_2.flags |= CLONE_INTO_CGROUP;
      clone3_ವಾದಗಳು_2.cgroup = cgroup_dirfd;
    }

  pid = syscall_clone3 (&clone3_ವಾದಗಳು_2);

  ಆದರೆ (pid > 0 && ! (process->scheduler && process->scheduler->policy && strcmp (process->scheduler->policy, "SCHED_DEADLINE") == 0))
    {
      /* We need to set the scheduler as soon as possible after joining the cgroup,
         because if it is a RT scheduler, other processes in the container could already
         take the entire cpu time and stall the new process.  This is not done for
         SCHED_DEADLINE, which does not allow the process to fork.  */
      ret = libcrun_ಗಣ_scheduler (pid, process, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  /* On errors, fall back to fork().  */
  ಆದರೆ (pid < 0)
    {
      need_move_ಗಮ್ಯ_cgroup = true;

      pid = fork ();
      ಆದರೆ (UNLIKELY (pid < 0))
        {
          ret = crun_make_error (err, errno, "fork");
          ಹೋಗು ನಿರ್ಗಮಿಸು;
        }
    }

  ಆದರೆ (pid)
    {
      close_and_reset (&sync_socket_fd[1]);
      sync_fd = sync_socket_fd[0];
      ಹಿಂತಿರುಗಿ join_process_ಪೋಷಕ_helper (context, container,
                                         process, pid, sync_fd,
                                         ಸ್ಥಿತಿ_2, need_move_ಗಮ್ಯ_cgroup,
                                         sub_cgroup, terminal_fd, err);
    }

  close_and_reset (&sync_socket_fd[0]);
  sync_fd = sync_socket_fd[1];

  /* Not needed anymore, do not keep a reference to the host file system.  */
  close_and_reset (&cgroup_dirfd);

  ret = join_process_namespaces (container, pid_ಗಮ್ಯ_join, ಸ್ಥಿತಿ_2, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (sync_fd, "1", 1));
      libcrun_fail_with_error ((*err)->ಸ್ಥಿತಿ_2, "%s", (*err)->ಸಂದೇಶ);
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  ಆದರೆ (UNLIKELY (pid < 0))
    {
      ಪೂರ್ಣಾಂಕ saved_errno = errno;
      ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (sync_fd, "1", 1));
      libcrun_fail_with_error (saved_errno, "fork");
    }

  ಆದರೆ (pid)
    {
      /* Just return the PID to the parent helper and exit.  */
      ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (sync_fd, "0", 1));
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          ಸುರಕ್ಷಿತ_exit (EXIT_FAILURE);
        }

      ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಲಿಖ (sync_fd, &pid, ಗಾತ್ರ (pid)));
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          kill (pid, SIGKILL);
          ಸುರಕ್ಷಿತ_exit (EXIT_FAILURE);
        }

      ಸುರಕ್ಷಿತ_exit (EXIT_SUCCESS);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* Inside the grandchild process.  The real process
         used for the container.  */
      cleanup_free ಅಕ್ಷರ *pty = ಖಾಲಿ;
      ಪೂರ್ಣಾಂಕ r = -1;

      /* Wait for the parent to complete the setup (cgroup, scheduler, ...).
         An EOF means the parent failed, so do not go any further.  */
      ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (ಪಠ (sync_fd, &r, ಗಾತ್ರ (r)));
      ಆದರೆ (UNLIKELY (ret != ಗಾತ್ರ (r) || r < 0))
        ಸುರಕ್ಷಿತ_exit (EXIT_FAILURE);

      ret = setsid ();
      ಆದರೆ (ret < 0)
        {
          crun_make_error (err, errno, "setsid");
          send_error_ಗಮ್ಯ_sync_socket_and_die (sync_fd, true, err);
        }

      ಆದರೆ (terminal_fd)
        {
          cleanup_close ಪೂರ್ಣಾಂಕ ptmx_fd = -1;

          ret = ಗಣ_id_init (container, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            send_error_ಗಮ್ಯ_sync_socket_and_die (sync_fd, true, err);

          ptmx_fd = open_terminal (&pty, process, err);
          ಆದರೆ (UNLIKELY (ptmx_fd < 0))
            send_error_ಗಮ್ಯ_sync_socket_and_die (sync_fd, true, err);

          ret = send_fd_ಗಮ್ಯ_socket (sync_fd, ptmx_fd, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            send_error_ಗಮ್ಯ_sync_socket_and_die (sync_fd, true, err);
        }
    }

  ಹಿಂತಿರುಗಿ 0;

ನಿರ್ಗಮಿಸು:
  ಆದರೆ (sync_socket_fd[0] >= 0)
    ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (sync_socket_fd[0]));
  ಆದರೆ (sync_socket_fd[1] >= 0)
    ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (sync_socket_fd[1]));
  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
libcrun_linux_container_update (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  cleanup_cgroup_ಸ್ಥಿತಿ ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ = ಖಾಲಿ;

  cgroup_ಸ್ಥಿತಿ = libcrun_cgroup_make_ಸ್ಥಿತಿ (ಸ್ಥಿತಿ_2);

  ಹಿಂತಿರುಗಿ libcrun_update_cgroup_resources (cgroup_ಸ್ಥಿತಿ, state_ಬೇರು, resources, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libcrun_container_pause_unpause_linux (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, ಸ್ಥಿರಾಂಕ bool pause, libcrun_error_t *err)
{
  cleanup_cgroup_ಸ್ಥಿತಿ ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ = ಖಾಲಿ;

  cgroup_ಸ್ಥಿತಿ = libcrun_cgroup_make_ಸ್ಥಿತಿ (ಸ್ಥಿತಿ_2);

  ಹಿಂತಿರುಗಿ libcrun_cgroup_pause_unpause (cgroup_ಸ್ಥಿತಿ, pause, err);
}

ಪೂರ್ಣಾಂಕ
libcrun_container_pause_linux (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ libcrun_container_pause_unpause_linux (ಸ್ಥಿತಿ_2, true, err);
}

ಪೂರ್ಣಾಂಕ
libcrun_container_unpause_linux (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ libcrun_container_pause_unpause_linux (ಸ್ಥಿತಿ_2, false, err);
}

ಪೂರ್ಣಾಂಕ
libcrun_ಗಣ_personality (runtime_spec_schema_defs_linux_personality *p, libcrun_error_t *err)
{
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ persona = 0;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (strcmp (p->domain, "LINUX") == 0)
    persona = ಪ್ರತಿ_LINUX;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (p->domain, "LINUX32") == 0)
    persona = ಪ್ರತಿ_LINUX32;
  ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "unknown persona specified `%s`", p->domain);

  ret = personality (persona);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "personality to `%s`", p->domain);

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_configure_ಜಾಲ (libcrun_container_t *container, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  size_t i;
  bool configure_ಜಾಲ = false;
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ಪೂರ್ಣಾಂಕ sockfd = -1;

  ಗಾಗಿ (i = 0; i < def->linux->namespaces_len; i++)
    {
      ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ = libcrun_ಕಂಡುಹಿಡಿ_namespace (def->linux->namespaces[i]->type);
      ಆದರೆ (UNLIKELY (ಮೌಲ್ಯ < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid namespace type: `%s`", def->linux->namespaces[i]->type);

      ಆದರೆ (ಮೌಲ್ಯ == CLONE_NEWNET && def->linux->namespaces[i]->ಮಾರ್ಗ == ಖಾಲಿ)
        {
          configure_ಜಾಲ = true;
          ನಿಲ್ಲಿಸು;
        }
    }

  ಆದರೆ (! configure_ಜಾಲ)
    ಹಿಂತಿರುಗಿ 0;

  sockfd = socket (AF_INET, SOCK_DGRAM, 0);
  ಆದರೆ (LIKELY (sockfd >= 0))
    {
      ರಚನೆ ifreq ifr_lo = { .ifr_name = "lo", .ifr_flags = IFF_ಮೇಲೆ | IFF_RUNNING };

      ret = ioctl (sockfd, SIOCSIFFLAGS, &ifr_lo);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "ioctl SIOCSIFFLAGS");
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ರಚನೆ nlmsghdr *hdr_recv;
      ಅಕ್ಷರ buf[ಗಾತ್ರ (ರಚನೆ nlmsghdr) + ಗಾತ್ರ (ರಚನೆ nlmsgerr)];
      ರಚನೆ sockaddr_nl addr = {
        .nl_family = AF_NETLINK,
        .nl_pid = getpid (),
      };
      ರಚನೆ sockaddr_nl addr_ಗಮ್ಯ = {
        .nl_family = AF_NETLINK,
        .nl_pid = 0,
      };
      ರಚನೆ nlmsghdr hdr = {
        .nlmsg_len = ಗಾತ್ರ (ರಚನೆ nlmsghdr) + ಗಾತ್ರ (ರಚನೆ ifinfomsg),
        .nlmsg_type = RTM_NEWLINK,
        .nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK,
        .nlmsg_seq = 1,
        .nlmsg_pid = 0,
      };
      ರಚನೆ ifinfomsg ಸಂದೇಶ = {
        .ifi_family = AF_UNSPEC,
        .ifi_type = 0,
        .ifi_ಸೂಚ್ಯಂಕ = 1,
        .ifi_flags = IFF_ಮೇಲೆ,
        .ifi_change = IFF_ಮೇಲೆ,
      };

      *((ರಚನೆ nlmsghdr *) buf) = hdr;
      *((ರಚನೆ ifinfomsg *) (buf + ಗಾತ್ರ (ರಚನೆ nlmsghdr))) = ಸಂದೇಶ;

      sockfd = socket (PF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);
      ಆದರೆ (UNLIKELY (sockfd < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "socket PF_NETLINK");

      ret = bind (sockfd, (ರಚನೆ sockaddr *) &addr, ಗಾತ್ರ (addr));
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "bind PF_NETLINK");

      ret = sendto (sockfd, buf, ಗಾತ್ರ (buf), 0, (ರಚನೆ sockaddr *) &addr_ಗಮ್ಯ, ಗಾತ್ರ (addr_ಗಮ್ಯ));
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "sendto PF_NETLINK");

      ret = recvfrom (sockfd, buf, ಗಾತ್ರ (buf), 0, ಖಾಲಿ, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "recvfrom PF_NETLINK");

      hdr_recv = (ರಚನೆ nlmsghdr *) buf;

      ಆದರೆ (hdr_recv->nlmsg_type == NLMSG_ERROR)
        {
          ರಚನೆ nlmsgerr *err_ದತ್ತಾಂಶ = (ರಚನೆ nlmsgerr *) NLMSG_ದತ್ತಾಂಶ (hdr_recv);
          /* err_data->error set to 0 means success acknowledgement due to NLM_F_ACK  */
          ಆದರೆ (err_ದತ್ತಾಂಶ->error < 0)
            ಹಿಂತಿರುಗಿ crun_make_error (err, -err_ದತ್ತಾಂಶ->error, "netlink error while configuring network");
        }
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* Protection for attacks like CVE-2019-5736.  */
ಪೂರ್ಣಾಂಕ ensure_cloned_ದ್ವಿಮಾನ ();
__attribute__ ((constructor)) ಸ್ಥಿರ ಶೂನ್ಯ
libcrun_rexec (ಶೂನ್ಯ)
{
  ಆದರೆ (ensure_cloned_ದ್ವಿಮಾನ () < 0)
    {
      fprintf (stderr, "Failed to re-execute libcrun via memory file descriptor\n");
      ಸುರಕ್ಷಿತ_exit (EXIT_FAILURE);
    }
}

ಪೂರ್ಣಾಂಕ
libcrun_container_checkpoint_linux (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, libcrun_container_t *container,
                                    libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ libcrun_container_checkpoint_linux_criu (ಸ್ಥಿತಿ_2, container, cr_options, err);
}

ಪೂರ್ಣಾಂಕ
libcrun_container_restore_linux (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, libcrun_container_t *container,
                                 libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  ret = libcrun_container_restore_linux_criu (ಸ್ಥಿತಿ_2, container, cr_options, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ 0;
}

/* Fallback to use kill(2) on systems where pidfd is not available.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
libcrun_kill_linux_no_pidfd (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, bool check_pid, ಪೂರ್ಣಾಂಕ ಸಂಕೇತ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  /* There is still a possibility that the pid is killed between the check
     and the time we send the signal, but attempt to reduce the window of time when
     it is possible.  */
  ಆದರೆ (check_pid)
    {
      ret = libcrun_check_pid_valid (ಸ್ಥಿತಿ_2, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      /* The pid is not valid anymore, return an error.  */
      ಆದರೆ (ret == 0)
        {
          errno = ESRCH;
          ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "kill container");
        }
    }

  ret = kill (ಸ್ಥಿತಿ_2->pid, ಸಂಕೇತ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "kill container");
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_kill_linux (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, ಪೂರ್ಣಾಂಕ ಸಂಕೇತ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;
  cleanup_close ಪೂರ್ಣಾಂಕ pidfd = -1;

  pidfd = syscall_pidfd_open (ಸ್ಥಿತಿ_2->pid, 0);
  ಆದರೆ (UNLIKELY (pidfd < 0))
    {
      /* If pidfd_open is not supported, fallback to kill.  */
      ಆದರೆ (errno == ENOSYS)
        ಹಿಂತಿರುಗಿ libcrun_kill_linux_no_pidfd (ಸ್ಥಿತಿ_2, true, ಸಂಕೇತ, err);
      /* Kernels < 6.16 return EINVAL, newer ones ENOENT, when the pid
         is not a thread-group leader (i.e. it was reused).  */
      ಆದರೆ (errno == ESRCH || errno == ENOENT || errno == EINVAL)
        ಹಿಂತಿರುಗಿ crun_make_error (err, ESRCH, "process not running");
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open pidfd");
    }

  ret = libcrun_check_pid_valid (ಸ್ಥಿತಿ_2, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  /* The pid is not valid anymore, return an error.  */
  ಆದರೆ (ret == 0)
    {
      errno = ESRCH;
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "kill container");
    }

  ret = syscall_pidfd_send_signal (pidfd, ಸಂಕೇತ, ಖಾಲಿ, 0);
  ಆದರೆ (UNLIKELY (ret < 0))
    {
      /* If pidfd_send_signal is not supported, fallback to kill.  */
      ಆದರೆ (errno == ENOSYS)
        ಹಿಂತಿರುಗಿ libcrun_kill_linux_no_pidfd (ಸ್ಥಿತಿ_2, false, ಸಂಕೇತ, err);
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "send signal to pidfd");
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
libcrun_get_intelrdt_name (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ctr_name, runtime_spec_schema_config_schema *def, bool *explicit)
{
  ಆದರೆ (def == ಖಾಲಿ || def->linux == ಖಾಲಿ || def->linux->intel_rdt == ಖಾಲಿ || def->linux->intel_rdt->clos_id == ಖಾಲಿ)
    {
      ಆದರೆ (explicit)
        *explicit = false;
      ಹಿಂತಿರುಗಿ ctr_name;
    }

  ಆದರೆ (explicit)
    *explicit = true;
  ಹಿಂತಿರುಗಿ def->linux->intel_rdt->clos_id;
}

ಪೂರ್ಣಾಂಕ
libcrun_apply_intelrdt (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ctr_name, libcrun_container_t *container, pid_t pid, ಪೂರ್ಣಾಂಕ actions, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = ಖಾಲಿ;
  bool has_monitoring = false;
  bool explicit = false;
  bool created = false;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (container)
    def = container->container_def;

  ಆದರೆ (def == ಖಾಲಿ || def->linux == ಖಾಲಿ || def->linux->intel_rdt == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, &explicit);

  ಆದರೆ (actions & LIBCRUN_INTELRDT_CREATE)
    {
      ret = resctl_create (name, explicit, &created, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ಆದರೆ (def->linux->intel_rdt->enable_monitoring)
        {
          ret = resctl_create_monitoring_ಗುಂಪು (name, ctr_name, err);
          ಆದರೆ (UNLIKELY (ret < 0))
            ಹೋಗು fail;

          has_monitoring = true;
        }
    }

  ಆದರೆ (actions & LIBCRUN_INTELRDT_UPDATE)
    {
      ret = resctl_update (name, def->linux->intel_rdt->l3cache_schema, def->linux->intel_rdt->mem_bw_schema, def->linux->intel_rdt->schemata, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹೋಗು fail;
    }

  ಆದರೆ (actions & LIBCRUN_INTELRDT_MOVE)
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *monitoring_name = def->linux->intel_rdt->enable_monitoring ? ctr_name : ಖಾಲಿ;
      ret = resctl_move_task_ಗಮ್ಯ (name, monitoring_name, pid, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹೋಗು fail;
    }

  ಹಿಂತಿರುಗಿ 0;

fail:
  ಆದರೆ (has_monitoring)
    {
      libcrun_error_t tmp_err = ಖಾಲಿ;
      ಪೂರ್ಣಾಂಕ tmp_ret;

      tmp_ret = resctl_destroy_monitoring_ಗುಂಪು (name, ctr_name, &tmp_err);
      ಆದರೆ (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }

  /* Cleanup only if the resctl was created as part of this call.  */
  ಆದರೆ (created)
    {
      libcrun_error_t tmp_err = ಖಾಲಿ;
      ಪೂರ್ಣಾಂಕ tmp_ret;

      tmp_ret = resctl_destroy (name, &tmp_err);
      ಆದರೆ (tmp_ret < 0)
        crun_error_release (&tmp_err);
    }
  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
libcrun_destroy_intelrdt (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *container_id, runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  bool explicit = false;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *clos_id = libcrun_get_intelrdt_name (container_id, def, &explicit);

  ಆದರೆ (def && def->linux && def->linux->intel_rdt && def->linux->intel_rdt->enable_monitoring)
    {
      ಪೂರ್ಣಾಂಕ ret;

      ret = resctl_destroy_monitoring_ಗುಂಪು (clos_id, container_id, err);
      ಆದರೆ (ret < 0)
        ಹಿಂತಿರುಗಿ ret;
    }

  /* Do not destroy the clos_id if the name was set explicitly.  */
  ಆದರೆ (explicit)
    ಹಿಂತಿರುಗಿ 0;

  ಹಿಂತಿರುಗಿ resctl_destroy (clos_id, err);
}

ಪೂರ್ಣಾಂಕ
libcrun_update_intel_rdt (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ctr_name, libcrun_container_t *container, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *l3_cache_schema, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *mem_bw_schema, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *schemata, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;

  ಆದರೆ (container)
    def = container->container_def;

  ಆದರೆ (def == ಖಾಲಿ || def->linux == ಖಾಲಿ || def->linux->intel_rdt == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  name = libcrun_get_intelrdt_name (ctr_name, def, ಖಾಲಿ);

  ಹಿಂತಿರುಗಿ resctl_update (name, l3_cache_schema, mem_bw_schema, schemata, err);
}

/* Change the current directory and make sure the current working
   directory, once set, is accessible from the current mount
   namespace.  This check prevents container-escape issues like
   CVE-2024-21626.
   The current working directory cannot be longer than PATH_MAX.
*/
ಪೂರ್ಣಾಂಕ
libcrun_ಸುರಕ್ಷಿತ_chdir (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *ಬಫರ್ = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;

  ret = chdir (ಮಾರ್ಗ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "chdir to `%s`", ಮಾರ್ಗ);

  ಬಫರ್ = xmalloc (PATH_MAX);
  ret = syscall_getcwd (ಬಫರ್, PATH_MAX);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, -ret, "getcwd");

  /* Enforce that the returned path is an absolute path.  */
  ಆದರೆ (ret == 0 || ಬಫರ್[0] != '/')
    {
      ಆದರೆ (chdir ("/") < 0)
        {
          /* Braces around empty body, to fix warning for [-Wunused-result] and error for [-Werror=empty-body]. */
        }
      errno = ENOENT;

      /*
        The kernel prepends the string "(unreachable)" to the path
        when it is not reachable from the current mount namespace.
        Use it to give a better error message.
      */
#ವ್ಯಾಖ್ಯಾನಿಸು UNREACHABLE "(unreachable)"
#ವ್ಯಾಖ್ಯಾನಿಸು UNREACHABLE_LEN ((ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (UNREACHABLE) - 1)

      ಆದರೆ ((ret >= UNREACHABLE_LEN) && (memcmp (ಬಫರ್, UNREACHABLE, UNREACHABLE_LEN) == 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "the working directory is not accessible from the current namespace");

      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "the current working directory is not an absolute path");
    }
  ಹಿಂತಿರುಗಿ 0;
}

/* Run the callback in the mount namespace referred to by MNTNS_FD.  It is
   done in a separate function so that no variable of the caller is live
   across the vfork.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಸತತ_callback_ಒಳ_namespace (ಪೂರ್ಣಾಂಕ mntns_fd, ಪೂರ್ಣಾಂಕ (*callback) (ಶೂನ್ಯ *, libcrun_error_t *), ಶೂನ್ಯ *arg, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ wait_ಸ್ಥಿತಿ = 0;
  pid_t pid;
  ಪೂರ್ಣಾಂಕ ret;

  /* must be vfork to propagate the error from the child proc.  */
  pid = vfork ();
  ಆದರೆ (UNLIKELY (pid < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "vfork");

  ಆದರೆ (pid == 0)
    {
      ret = setns (mntns_fd, CLONE_NEWNS);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, 0, "setns to target pid");
          ಸುರಕ್ಷಿತ_exit (ret);
        }
      ret = chdir ("/");
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          /* Create an error for the parent proc.  */
          crun_make_error (err, errno, "chdir to `/`");
          ಸುರಕ್ಷಿತ_exit (ret);
        }

      ret = callback (arg, err);
      /* On failure (ret < 0) the created error will be
         used by the parent proc */
      ಸುರಕ್ಷಿತ_exit (ret);
    }

  ret = waitpid_ignore_stopped (pid, &wait_ಸ್ಥಿತಿ, 0);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "waitpid for exec child pid");

  /* The vfork() child shares the parent's memory space, so the error
     object populated by the child is available to the parent.
     Do not call crun_make_error() here. */
  ಹಿಂತಿರುಗಿ get_process_exit_ಸ್ಥಿತಿ (wait_ಸ್ಥಿತಿ);
}

/* Check that USERNS_FD refers to the user namespace configured for the container.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
check_container_userns (libcrun_container_t *container, ಪೂರ್ಣಾಂಕ userns_fd, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  bool new_userns = container->host_uid != 0;
  cleanup_close ಪೂರ್ಣಾಂಕ expected_fd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ ಪೋಷಕ_fd = -1;
  ರಚನೆ stat st, expected_st;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ = ಖಾಲಿ;
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (def->linux)
    ಗಾಗಿ (i = 0; i < def->linux->namespaces_len; i++)
      ಆದರೆ (libcrun_ಕಂಡುಹಿಡಿ_namespace (def->linux->namespaces[i]->type) == CLONE_NEWUSER)
        {
          ಮಾರ್ಗ = def->linux->namespaces[i]->ಮಾರ್ಗ;
          new_userns = ಮಾರ್ಗ == ಖಾಲಿ;
          ನಿಲ್ಲಿಸು;
        }

  ಆದರೆ (ಮಾರ್ಗ == ಖಾಲಿ)
    {
      expected_fd = libcrun_open_proc_file (container, "self/ns/user", O_RDONLY, err);
      ಆದರೆ (UNLIKELY (expected_fd < 0))
        ಹಿಂತಿರುಗಿ expected_fd;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      expected_fd = open (ಮಾರ್ಗ, O_RDONLY | O_CLOEXEC);
      ಆದರೆ (UNLIKELY (expected_fd < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", ಮಾರ್ಗ);
    }

  /* A user namespace created for the container is a child of the current one.  */
  ಆದರೆ (new_userns)
    {
      ಪೋಷಕ_fd = ioctl (userns_fd, NS_GET_ಪೋಷಕ);
      ಆದರೆ (UNLIKELY (ಪೋಷಕ_fd < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "get the parent of the user namespace");
      userns_fd = ಪೋಷಕ_fd;
    }

  ret = fstat (userns_fd, &st);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fstat user namespace");

  ret = fstat (expected_fd, &expected_st);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fstat user namespace");

  ಆದರೆ (st.st_dev != expected_st.st_dev || st.st_ino != expected_st.st_ino)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "the container process is not using the user namespace of the container");

  ಹಿಂತಿರುಗಿ 0;
}

/* Open the mount namespace of the container in MNTNS_FD and the user namespace
   owning it in OWNER_FD.  The container process could have moved to a mount
   namespace owned by a nested user namespace, where it has CAP_SYS_ADMIN.  A
   mount attached there is not protected by the MNT_LOCK_* flags the kernel
   sets on the mounts copied to a less privileged namespace, so the container
   could drop its ro, nosuid, nodev and noexec flags.  Make sure the mount
   namespace is owned by the user namespace of the container, so that the
   caller can join the same namespaces that were checked.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
open_container_mntns (libcrun_container_t *container, libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, ಪೂರ್ಣಾಂಕ *mntns_fd, ಪೂರ್ಣಾಂಕ *owner_fd, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ mntns = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ owner = -1;
  ಪೂರ್ಣಾಂಕ ret;

  mntns = libcrun_open_proc_pid_file (container, ಸ್ಥಿತಿ_2->pid, "ns/mnt", O_RDONLY, err);
  ಆದರೆ (UNLIKELY (mntns < 0))
    ಹಿಂತಿರುಗಿ mntns;

  /* Check if the container is still running after opening the namespace.  */
  ret = libcrun_is_container_running (ಸ್ಥಿತಿ_2, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (ret == 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "container not running");

  owner = ioctl (mntns, NS_GET_USERNS);
  ಆದರೆ (UNLIKELY (owner < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "get the owner of the mount namespace");

  ret = check_container_userns (container, owner, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  *mntns_fd = mntns;
  mntns = -1;
  *owner_fd = owner;
  owner = -1;
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಸತತ_ಒಳ_container_namespace (libcrun_container_t *container, libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, ಪೂರ್ಣಾಂಕ (*callback) (ಶೂನ್ಯ *, libcrun_error_t *), ಶೂನ್ಯ *arg, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ mntns_fd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ owner_fd = -1;
  ಪೂರ್ಣಾಂಕ ret;

  ret = open_container_mntns (container, ಸ್ಥಿತಿ_2, &mntns_fd, &owner_fd, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ ಸತತ_callback_ಒಳ_namespace (mntns_fd, callback, arg, err);
}

ರಚನೆ umount_ಒಳ_a_container_ವಾದಗಳು
{
  runtime_spec_schema_defs_mount **mounts;
  size_t len;
};

/* Detach the mount at DESTINATION, relative to the current root.  The file
   system tree is under the control of the container, so no symlink is
   followed, otherwise the container could choose what mount is detached.
   umount2 accepts only a path, so unmount the last component from its parent
   directory and then go back to the previous working directory.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
umount_nofollow (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *destination, libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ parentfd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ cwdfd = -1;
  ಅಕ್ಷರ ಬಫರ್[PATH_MAX];
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೋಷಕ_2, *name;
  ಅಕ್ಷರ *ಕೊನೆಯ_slash;
  size_t len;
  ಪೂರ್ಣಾಂಕ saved_errno;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (is_ಖಾಲಿಗಳು_string (destination))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "empty destination for mount");

  len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (destination);
  ಆದರೆ (UNLIKELY (len >= ಗಾತ್ರ (ಬಫರ್)))
    ಹಿಂತಿರುಗಿ crun_make_error (err, ENAMETOOLONG, "umount `%s`", destination);
  ಸ್ಮರಣೆ_ನಕಲು (ಬಫರ್, destination, len + 1);

  ಆಗ (len > 1 && ಬಫರ್[len - 1] == '/')
    ಬಫರ್[--len] = '\0';

  ಕೊನೆಯ_slash = strrchr (ಬಫರ್, '/');
  ಆದರೆ (ಕೊನೆಯ_slash == ಖಾಲಿ)
    {
      ಪೋಷಕ_2 = ".";
      name = ಬಫರ್;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕೊನೆಯ_slash == ಬಫರ್)
    {
      ಪೋಷಕ_2 = "/";
      name = ಬಫರ್[1] ? ಬಫರ್ + 1 : ".";
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      *ಕೊನೆಯ_slash = '\0';
      ಪೋಷಕ_2 = ಬಫರ್;
      name = ಕೊನೆಯ_slash + 1;
    }

  parentfd = syscall_openat2 (AT_FDCWD, ಪೋಷಕ_2, O_ಮಾರ್ಗ | O_DIRECTORY | O_CLOEXEC, 0,
                              RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS);
  ಆದರೆ (UNLIKELY (parentfd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open parent directory of `%s`", destination);

  cwdfd = open (".", O_ಮಾರ್ಗ | O_DIRECTORY | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (cwdfd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open current directory");

  ret = fchdir (parentfd);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fchdir to parent directory of `%s`", destination);

  ret = umount2 (name, MNT_DETACH | UMOUNT_NOFOLLOW);
  saved_errno = errno;

  ಆದರೆ (UNLIKELY (fchdir (cwdfd) < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fchdir to the previous working directory");

  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, saved_errno, "umount `%s`", destination);

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_umount_ಒಳ_a_container (ಶೂನ್ಯ *arg, libcrun_error_t *err)
{
  ರಚನೆ umount_ಒಳ_a_container_ವಾದಗಳು *ವಾದಗಳು = arg;
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  ಗಾಗಿ (i = 0; i < ವಾದಗಳು->len; i++)
    {
      ret = umount_nofollow (ವಾದಗಳು->mounts[i]->destination, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ mount_ಒಳ_a_container_ವಾದಗಳು
{
  runtime_spec_schema_defs_mount **mounts;
  ರಚನೆ libcrun_fd_map *fds;
  ಪೂರ್ಣಾಂಕ pidfd;
  ಪೂರ್ಣಾಂಕ owner_fd;
  size_t len;
};

/* Open DESTINATION, relative to the current root, creating it if it does not
   exist.  The file system tree is under the control of the container, so no
   symlink is followed, as it happens for `mounts remove`, otherwise a mount
   could be added that cannot be removed.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
open_destination_nofollow (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *destination, bool dir, libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ uint64_t resolve = RESOLVE_NO_SYMLINKS | RESOLVE_NO_MAGICLINKS;
  ಪೂರ್ಣಾಂಕ fd;
  ಪೂರ್ಣಾಂಕ ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_ಮಾರ್ಗ | O_CLOEXEC, 0, resolve);
  ಆದರೆ (LIKELY (fd >= 0))
    ಹಿಂತಿರುಗಿ fd;

  ಆದರೆ (errno != ENOENT)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", destination);

  ಆದರೆ (dir)
    ret = crun_ಸುರಕ್ಷಿತ_ensure_directory_at (AT_FDCWD, "/", destination, 0755, err);
  ಇಲ್ಲದಿದ್ದರೆ
    ret = crun_ಸುರಕ್ಷಿತ_ensure_file_at (AT_FDCWD, "/", destination, 0755, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  fd = syscall_openat2 (AT_FDCWD, destination, O_ಮಾರ್ಗ | O_CLOEXEC, 0, resolve);
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", destination);

  ಹಿಂತಿರುಗಿ fd;
}

/* Create the missing destinations of the mounts as the root user of the
   container.  The file systems of the container can be owned by its user
   namespace, where the root user of the host has no mapping, so that an
   inode it creates there would have no owner and mkdir fails with
   EOVERFLOW.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
create_destinations_ಒಳ_a_container (ಶೂನ್ಯ *arg, libcrun_error_t *err)
{
  ರಚನೆ mount_ಒಳ_a_container_ವಾದಗಳು *ವಾದಗಳು = arg;
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  ret = setns (ವಾದಗಳು->owner_fd, CLONE_NEWUSER);
  ಆದರೆ (ret == 0)
    {
      ret = setresgid (0, 0, 0);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "setresgid to `0`");

      ret = setresuid (0, 0, 0);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "setresuid to `0`");
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  ಗಾಗಿ (i = 0; i < ವಾದಗಳು->len; i++)
    {
      cleanup_close ಪೂರ್ಣಾಂಕ dest_fd = -1;
      ರಚನೆ stat st;

      ret = fstat (ವಾದಗಳು->fds->fds[i], &st);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (ವಾದಗಳು->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      ಆದರೆ (UNLIKELY (dest_fd < 0))
        ಹಿಂತಿರುಗಿ dest_fd;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_mount_ಒಳ_a_container (ಶೂನ್ಯ *arg, libcrun_error_t *err)
{
  ರಚನೆ mount_ಒಳ_a_container_ವಾದಗಳು *ವಾದಗಳು = arg;
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  ಗಾಗಿ (i = 0; i < ವಾದಗಳು->len; i++)
    {
      cleanup_close ಪೂರ್ಣಾಂಕ dest_fd = -1;
      ರಚನೆ stat st;

      ret = fstat (ವಾದಗಳು->fds->fds[i], &st);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fstat");

      dest_fd = open_destination_nofollow (ವಾದಗಳು->mounts[i]->destination, (st.st_mode & S_IFMT) == S_IFDIR, err);
      ಆದರೆ (UNLIKELY (dest_fd < 0))
        ಹಿಂತಿರುಗಿ dest_fd;

      ret = fs_move_mount_ಗಮ್ಯ (ವಾದಗಳು->fds->fds[i], dest_fd, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "move mount to `%s`", ವಾದಗಳು->mounts[i]->destination);
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* Join the namespaces of the container referred to by PIDFD, create the file
   system for each of MOUNTS that has no fd in FDS yet and send them, in order,
   to SYNC_FD.  It must be called from a child process.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
send_mounts_from_container_namespaces (libcrun_container_t *container, ಪೂರ್ಣಾಂಕ pidfd, runtime_spec_schema_defs_mount **mounts,
                                       ರಚನೆ libcrun_fd_map *fds, size_t len, ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ owner_fd, ಪೂರ್ಣಾಂಕ sync_fd, libcrun_error_t *err)
{
  pid_t pid;
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  /* The process is visible from the container once it joins its PID namespace.  */
  ret = prctl (PR_ಗಣ_DUMPABLE, 0, 0, 0, 0);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  /* Join the namespaces while still being privileged, then the user
     namespace that was checked and not the one the container process is in
     right now.  */
  ret = flags ? setns (pidfd, flags) : 0;
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "setns to target pid");

  ret = setns (owner_fd, CLONE_NEWUSER);
  ಆದರೆ (ret == 0)
    {
      /* CAP_SYS_ADMIN is needed in the user namespace owning the mount
         namespace.  Do not join the mount namespace of the container, any
         path used for the mount refers to the host, as it does for the
         mounts in the container configuration.  */
      ret = unshare (CLONE_NEWNS);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "unshare `CLONE_NEWNS`");
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (errno != EINVAL)
    {
      /* EINVAL means that the container runs in the current user namespace.  */
      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "setns to the user namespace of the container");
    }

  /* We need to fork once again to join the PID namespace.  */
  pid = fork ();
  ಆದರೆ (UNLIKELY (pid < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fork");

  ಆದರೆ (pid)
    {
      ಪೂರ್ಣಾಂಕ wait_ಸ್ಥಿತಿ = 0;

      /* The container can signal the process, do not hang if it is stopped.  */
      ret = ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (waitpid (pid, &wait_ಸ್ಥಿತಿ, WUNTRACED));
      ಆದರೆ (ret > 0 && WIFSTOPPED (wait_ಸ್ಥಿತಿ))
        {
          kill (pid, SIGKILL);
          ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (waitpid (pid, ಖಾಲಿ, 0));
          ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "the process creating the mounts was stopped");
        }
      ಸುರಕ್ಷಿತ_exit (EXIT_SUCCESS);
    }

  ret = ಗಣ_id_init (container, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = prctl (PR_ಗಣ_DUMPABLE, 0, 0, 0, 0);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "prctl (PR_SET_DUMPABLE)");

  ಗಾಗಿ (i = 0; i < len; i++)
    {
      cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;

      ಆದರೆ (fds->fds[i] >= 0)
        ಮುಂದುವರಿ;

      fd = open_mount_ದ_type (container, mounts[i], err);
      ಆದರೆ (UNLIKELY (fd < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open mount of type `%s` for `%s`", mounts[i]->type, mounts[i]->destination);

      ret = send_fd_ಗಮ್ಯ_socket (sync_fd, fd, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* Get in FLAGS the namespaces a file system can be bound to (together with
   the user namespace) that the container does not share with the current
   process.  setns requires CAP_SYS_ADMIN in the user namespace owning the
   target also for a namespace that is already the current one.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_namespaces_ಗಮ್ಯ_join (libcrun_container_t *container, pid_t pid, ಪೂರ್ಣಾಂಕ *flags, libcrun_error_t *err)
{
  ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ
  {
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
    ಪೂರ್ಣಾಂಕ flag;
  } namespaces[] = {
    { "ns/pid", CLONE_NEWPID },
    { "ns/net", CLONE_NEWNET },
    { "ns/ipc", CLONE_NEWIPC },
    { "ns/cgroup", CLONE_NEWCGROUP },
  };
  size_t i;

  *flags = 0;

  ಗಾಗಿ (i = 0; i < ಗಾತ್ರ (namespaces) / ಗಾತ್ರ (namespaces[0]); i++)
    {
      cleanup_close ಪೂರ್ಣಾಂಕ self_fd = -1;
      cleanup_close ಪೂರ್ಣಾಂಕ ಗುರಿ_fd = -1;
      ರಚನೆ stat self_st, ಗುರಿ_st;
      cleanup_free ಅಕ್ಷರ *self_ಮಾರ್ಗ = ಖಾಲಿ;
      ಪೂರ್ಣಾಂಕ ret;

      xasprintf (&self_ಮಾರ್ಗ, "self/%s", namespaces[i].name);

      self_fd = libcrun_open_proc_file (container, self_ಮಾರ್ಗ, O_RDONLY, err);
      ಆದರೆ (UNLIKELY (self_fd < 0))
        ಹಿಂತಿರುಗಿ self_fd;

      ಗುರಿ_fd = libcrun_open_proc_pid_file (container, pid, namespaces[i].name, O_RDONLY, err);
      ಆದರೆ (UNLIKELY (ಗುರಿ_fd < 0))
        ಹಿಂತಿರುಗಿ ಗುರಿ_fd;

      ret = fstat (self_fd, &self_st);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      ret = fstat (ಗುರಿ_fd, &ಗುರಿ_st);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "fstat `%s`", namespaces[i].name);

      ಆದರೆ (self_st.st_dev != ಗುರಿ_st.st_dev || self_st.st_ino != ಗುರಿ_st.st_ino)
        *flags |= namespaces[i].flag;
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* A file system is bound to the namespaces of the process creating it (e.g.
   procfs to its PID namespace), so create the ones still missing in FDS from
   a process that joined the namespaces of the container, as it happens for
   the mounts in the container configuration.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
open_mounts_ಒಳ_container_namespaces (libcrun_container_t *container, libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, ಪೂರ್ಣಾಂಕ owner_fd,
                                     runtime_spec_schema_defs_mount **mounts, ರಚನೆ libcrun_fd_map *fds, size_t len,
                                     libcrun_error_t *err)
{
  cleanup_close ಪೂರ್ಣಾಂಕ sync_fd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ pidfd = -1;
  ಪೂರ್ಣಾಂಕ sync_socket_fd[2];
  bool needed = false;
  ಪೂರ್ಣಾಂಕ flags;
  pid_t pid;
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  ಗಾಗಿ (i = 0; i < len; i++)
    {
      ಆದರೆ (fds->fds[i] >= 0)
        ಮುಂದುವರಿ;

      ಆದರೆ (mounts[i]->type == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid mount type for `%s`", mounts[i]->destination);

      needed = true;
    }

  ಆದರೆ (! needed)
    ಹಿಂತಿರುಗಿ 0;

  pidfd = syscall_pidfd_open (ಸ್ಥಿತಿ_2->pid, 0);
  ಆದರೆ (UNLIKELY (pidfd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "pidfd_open");

  /* Check if the container is still running after opening the pidfd.  */
  ret = libcrun_is_container_running (ಸ್ಥಿತಿ_2, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (ret == 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "container not running");

  ret = get_namespaces_ಗಮ್ಯ_join (container, ಸ್ಥಿತಿ_2->pid, &flags, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = socketpair (AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0, sync_socket_fd);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "error creating socketpair");

  sync_fd = sync_socket_fd[0];

  pid = fork ();
  ಆದರೆ (UNLIKELY (pid < 0))
    {
      ret = crun_make_error (err, errno, "fork");
      ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (sync_socket_fd[1]));
      ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (pid == 0)
    {
      close_and_reset (&sync_fd);

      ret = send_mounts_from_container_namespaces (container, pidfd, mounts, fds, len, flags, owner_fd, sync_socket_fd[1], err);
      ಆದರೆ (UNLIKELY (ret < 0))
        send_error_ಗಮ್ಯ_sync_socket_and_die (sync_socket_fd[1], true, err);

      ಸುರಕ್ಷಿತ_exit (EXIT_SUCCESS);
    }

  ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (close (sync_socket_fd[1]));

  ret = 0;
  ಗಾಗಿ (i = 0; i < len; i++)
    {
      ಆದರೆ (fds->fds[i] >= 0)
        ಮುಂದುವರಿ;

      ret = receive_fd_from_socket (sync_fd, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        {
          cleanup_free ಅಕ್ಷರ *err_str = ಖಾಲಿ;
          ಪೂರ್ಣಾಂಕ err_ಸಂಕೇತ = 0;

          ಆದರೆ (read_error_from_sync_socket (sync_fd, &err_ಸಂಕೇತ, &err_str) && ! is_ಖಾಲಿಗಳು_string (err_str))
            {
              crun_error_release (err);
              ret = crun_make_error (err, err_ಸಂಕೇತ, "%s", err_str);
            }
          ಇಲ್ಲದಿದ್ದರೆ
            ret = crun_error_wrap (err, "receive mount fd");
          ನಿಲ್ಲಿಸು;
        }

      fds->fds[i] = ret;
      ret = 0;
    }

  /* The child process reaps the process that joined the PID namespace of the container.  */
  ತಾತ್ಕಾಲಿಕ_FAILURE_RETRY (waitpid (pid, ಖಾಲಿ, 0));

  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ
get_runtime_mount_flags (libcrun_container_t *container, runtime_spec_schema_defs_mount *mount, uint64_t *rec_clear, uint64_t *rec_ಗಣ)
{
  cleanup_free ಅಕ್ಷರ *ದತ್ತಾಂಶ = ಖಾಲಿ;
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ extra_flags = 0;
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags = 0;
  size_t j;

  ಆದರೆ (mount->options == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ get_default_flags (container, mount->destination, &ದತ್ತಾಂಶ);

  ಗಾಗಿ (j = 0; j < mount->options_len; j++)
    flags |= get_mount_flags_or_option (mount->options[j], flags, &extra_flags, &ದತ್ತಾಂಶ, rec_clear, rec_ಗಣ);

  ಹಿಂತಿರುಗಿ flags;
}

ಪೂರ್ಣಾಂಕ
libcrun_make_runtime_mounts (libcrun_container_t *container, libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  ರಚನೆ mount_ಒಳ_a_container_ವಾದಗಳು ವಾದಗಳು;
  cleanup_close_map ರಚನೆ libcrun_fd_map *fds = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ pidfd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ mntns_fd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ owner_fd = -1;
  pid_t pid = ಸ್ಥಿತಿ_2->pid;
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  ret = open_container_mntns (container, ಸ್ಥಿತಿ_2, &mntns_fd, &owner_fd, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  fds = make_libcrun_fd_map (len);

  ಗಾಗಿ (i = 0; i < len; i++)
    {
      bool recursive = false;
      bool nofollow = false;

      /* Do not check whether the pid is valid or not.  run_in_container_namespace will validate it.  */
      ret = maybe_get_idmapped_mount (container, def, mounts[i], pid, &(fds->fds[i]), ಖಾಲಿ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ಆದರೆ (fds->fds[i] < 0 && is_bind_mount (mounts[i], &recursive, &nofollow))
        {
          uint64_t rec_clear = 0, rec_ಗಣ = 0;
          ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪ್ರಸರಣ;

          ಪ್ರಸರಣ = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_ಗಣ) & ALL_PROPAGATIONS_NO_REC;
          ಆದರೆ (ಪ್ರಸರಣ == 0)
            ಪ್ರಸರಣ = MS_PRIVATE;

          fds->fds[i] = get_bind_mount (-1, mounts[i]->ಮೂಲ, recursive, false, nofollow, ಪ್ರಸರಣ, err);
          ಆದರೆ (UNLIKELY (fds->fds[i] < 0))
            ಹಿಂತಿರುಗಿ fds->fds[i];
        }
    }

  /* All the remaining mounts are new file systems.  */
  ret = open_mounts_ಒಳ_container_namespaces (container, ಸ್ಥಿತಿ_2, owner_fd, mounts, fds, len, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಗಾಗಿ (i = 0; i < len; i++)
    {
      uint64_t rec_clear = 0;
      uint64_t rec_ಗಣ = 0;
      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

      flags = get_runtime_mount_flags (container, mounts[i], &rec_clear, &rec_ಗಣ);

      ret = do_mount_setattr (false, mounts[i]->destination, fds->fds[i], 0, flags, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = do_mount_setattr (true, mounts[i]->destination, fds->fds[i], rec_clear, rec_ಗಣ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ವಾದಗಳು.mounts = mounts;
  ವಾದಗಳು.fds = fds;
  ವಾದಗಳು.pidfd = pidfd;
  ವಾದಗಳು.owner_fd = owner_fd;
  ವಾದಗಳು.len = len;

  ret = ಸತತ_callback_ಒಳ_namespace (mntns_fd, create_destinations_ಒಳ_a_container, &ವಾದಗಳು, err);
  ಆದರೆ (UNLIKELY (ret > 0))
    ಹಿಂತಿರುಗಿ *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ ಸತತ_callback_ಒಳ_namespace (mntns_fd, do_mount_ಒಳ_a_container, &ವಾದಗಳು, err);
}

ಪೂರ್ಣಾಂಕ
libcrun_destroy_runtime_mounts (libcrun_container_t *container, libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, runtime_spec_schema_defs_mount **mounts, size_t len, libcrun_error_t *err)
{
  ರಚನೆ umount_ಒಳ_a_container_ವಾದಗಳು ವಾದಗಳು;

  ವಾದಗಳು.mounts = mounts;
  ವಾದಗಳು.len = len;

  ಹಿಂತಿರುಗಿ ಸತತ_ಒಳ_container_namespace (container, ಸ್ಥಿತಿ_2, do_umount_ಒಳ_a_container, &ವಾದಗಳು, err);
}

ರಚನೆ restrict_mounts_ಒಳ_a_container_ವಾದಗಳು
{
  ರಚನೆ libcrun_mount_flags_s *mounts;
  size_t len;
  ಪೂರ್ಣಾಂಕ rootfd;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_restrict_mounts_ಒಳ_a_container (ಶೂನ್ಯ *arg, libcrun_error_t *err)
{
  ರಚನೆ restrict_mounts_ಒಳ_a_container_ವಾದಗಳು *ವಾದಗಳು = arg;
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  /* The root of the container is not necessarily the root of the mount
     namespace.  */
  ret = fchdir (ವಾದಗಳು->rootfd);
  ಆದರೆ (LIKELY (ret == 0))
    ret = chroot (".");
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "chroot to the container root");

  ಗಾಗಿ (i = 0; i < ವಾದಗಳು->len; i++)
    {
      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags = ವಾದಗಳು->mounts[i].flags & (MS_RDONLY | MS_NOSUID | MS_NODEV | MS_NOEXEC);
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಗುರಿ = consume_slashes (ವಾದಗಳು->mounts[i].ಗುರಿ);
      cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
      ರಚನೆ statfs sfs;
      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ current_flags;

      ಆದರೆ (flags == 0)
        ಮುಂದುವರಿ;

      fd = ಸುರಕ್ಷಿತ_openat (AT_FDCWD, "/", ಗುರಿ, O_ಮಾರ್ಗ | O_CLOEXEC, 0, err);
      ಆದರೆ (UNLIKELY (fd < 0))
        {
          ಆದರೆ (crun_error_get_errno (err) == ENOENT)
            {
              crun_error_release (err);
              ಮುಂದುವರಿ;
            }
          ಹಿಂತಿರುಗಿ fd;
        }

      ret = fstatfs (fd, &sfs);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "statfs `/%s`", ಗುರಿ);

      current_flags = mount_flags_from_statfs (sfs.f_flags);
      ಆದರೆ ((current_flags & flags) == flags)
        ಮುಂದುವರಿ;

      ret = do_mount_setattr (false, ಗುರಿ, fd, 0, flags, err);
      ಆದರೆ (LIKELY (ret == 0))
        ಮುಂದುವರಿ;

      ಆದರೆ (crun_error_get_errno (err) != ENOSYS && crun_error_get_errno (err) != EINVAL)
        ಹಿಂತಿರುಗಿ ret;
      crun_error_release (err);

      /* Without mount_setattr(2), remount through the path: the container is
         not running yet, and the root was changed to its rootfs.  The remount
         clears the per-mount flags that are not specified, so pass the
         current ones as well.  */
      ret = mount (ಖಾಲಿ, ಗುರಿ, ಖಾಲಿ, MS_REMOUNT | MS_BIND | flags | current_flags, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "remount `/%s`", ಗುರಿ);
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* Make sure that each of MOUNTS, in the mount namespace of PID, has at least
   the read-only, nosuid, nodev and noexec flags that are requested for it.  */
ಪೂರ್ಣಾಂಕ
libcrun_restrict_mounts_ಒಳ_container (pid_t pid, ರಚನೆ libcrun_mount_flags_s *mounts, size_t len, libcrun_error_t *err)
{
  ರಚನೆ restrict_mounts_ಒಳ_a_container_ವಾದಗಳು ವಾದಗಳು;
  cleanup_close ಪೂರ್ಣಾಂಕ mntns_fd = -1;
  cleanup_close ಪೂರ್ಣಾಂಕ rootfd = -1;
  ಅಕ್ಷರ ಮಾರ್ಗ[64];
  ಪೂರ್ಣಾಂಕ ret;

  snprintf (ಮಾರ್ಗ, ಗಾತ್ರ (ಮಾರ್ಗ), "/proc/%d/ns/mnt", pid);
  mntns_fd = open (ಮಾರ್ಗ, O_RDONLY | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (mntns_fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", ಮಾರ್ಗ);

  snprintf (ಮಾರ್ಗ, ಗಾತ್ರ (ಮಾರ್ಗ), "/proc/%d/root", pid);
  rootfd = open (ಮಾರ್ಗ, O_ಮಾರ್ಗ | O_DIRECTORY | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (rootfd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s`", ಮಾರ್ಗ);

  ವಾದಗಳು.mounts = mounts;
  ವಾದಗಳು.len = len;
  ವಾದಗಳು.rootfd = rootfd;

  ret = ಸತತ_callback_ಒಳ_namespace (mntns_fd, do_restrict_mounts_ಒಳ_a_container, &ವಾದಗಳು, err);
  ಆದರೆ (UNLIKELY (ret > 0))
    ಹಿಂತಿರುಗಿ *err ? -1 : crun_make_error (err, 0, "the process in the container mount namespace exited with status %d", ret);

  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
libcrun_move_ಜಾಲ_devices (libcrun_container_t *container, pid_t pid, libcrun_error_t *err)
{
  runtime_spec_schema_config_schema *def = container->container_def;
  cleanup_close ಪೂರ್ಣಾಂಕ netns_fd = -1;
  size_t i;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (def == ಖಾಲಿ || def->linux == ಖಾಲಿ || def->linux->net_devices == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  cleanup_free ಅಕ್ಷರ *ns_ಮಾರ್ಗ = ಖಾಲಿ;

  xasprintf (&ns_ಮಾರ್ಗ, "%d/ns/net", pid);
  netns_fd = libcrun_open_proc_file (container, ns_ಮಾರ್ಗ, O_RDONLY, err);
  ಆದರೆ (UNLIKELY (netns_fd < 0))
    ಹಿಂತಿರುಗಿ netns_fd;

  ಗಾಗಿ (i = 0; i < def->linux->net_devices->len; i++)
    {
      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *new_name = def->linux->net_devices->ಮೌಲ್ಯಗಳು[i]->name ?: def->linux->net_devices->ಕೀಲಿಗಳು[i];

      ret = move_ಜಾಲ_device (def->linux->net_devices->ಕೀಲಿಗಳು[i], new_name, netns_fd, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ 0;
}
